/*****************************************************************************
** File Name:      mmiatv_wintab.c                                           *
** Author:                                                                   *
** Date:           2010-05	                                                 *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe atv window table            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010        xin.li           Create									 *
*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_atv_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmidisplay_data.h"
#include "window_parse.h"
#include "mmk_type.h"
#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_default.h"
#include "mmi_theme.h"
#include "mmitheme_pos.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmipub.h"
#include "guicommon.h"
#include "guilcd.h"
#include "guires.h"
#include "gui_ucs2b_converter.h"
#include "guiedit.h"
#include "guistatusbar.h"
#include "guilistbox.h"
#include "guitext.h"
#include "guimenu.h"
#include "guiwin.h"
#include "guibutton.h"
#include "guiform.h"
#include "guiownerdraw.h"
#include "guilabel.h"
#include "guistring.h"
#include "mmk_timer.h"
#include "mmk_msg.h"
#include "mmk_app.h"
#include "cafcontrol.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "block_mem.h"
#include "block_mem_def.h"
#include "mmi_default.h"
#include "mmiidle_export.h"
#include "mmi_modu_main.h"
#include "lcd_backlight.h"
#include "mmisrvaud_api.h"
#include "mmisrvmgr.h"
#include "mmiset_display.h"
#include "mmicom_panel.h"

#include "guiblock.h"
#include "mmiatv_export.h"
#include "mmiatv_main.h"
#include "mmiatv_wintab.h"
#include "mmiatv_data.h"
#include "mmiatv_id.h"
#include "mmiatv_nv.h"
#include "mmiatv_text.h"
#include "mmiatv_image.h"
#include "mmiatv_menutable.h"
#include "mmiatv_position.h"
#include "guidropdownlist.h"
#include "mmiudisk_export.h"
#include "mmibt_export.h"
#include "mmiidle_statusbar.h"
#include "mmisd_export.h"
#include "mmimultim_image.h"

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define MMIATV_NORMAL_FONT  MMI_DEFAULT_NORMAL_FONT

#define MMIATV_SYSTIME_SERVICE_NUMBER           (1 << 0)
#define MMIATV_SYSTIME_COMMON_TIP               (1 << 1)
#define MMIATV_SYSTIME_VOLUME_PROGRESS          (1 << 2)
#define MMIATV_SYSTIME_BRIGHT_PROGRESS          (1 << 3)
#define MMIATV_SYSTIME_ALL                      0xffffffff

#define MMIATV_SYSTEM_TIMER_UNIT                500
#define MMIATV_2S_TIMEOUT                       2
#define MMIATV_3S_TIMEOUT                       3

#define MMIATV_LCD_CONTRSAT_MAX                 100                     //亮度最大值
#define MMIATV_BRIGHTNESS_MAX                   MMISET_CONTRAST_SEVEN   //亮度最大级别
#define MMIATV_VOLUME_MAX                       MMISRVAUD_VOLUME_LEVEL_MAX         //音量最大级别

#define MMIATV_PANEL_BG_COLOR                   0x0020 
/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
LOCAL MMIATV_INFO_T         s_atv_current_para = {0};       //atv应用状态信息
// LOCAL BOOLEAN               s_is_inner_lcd_switch = FALSE;  //是否是内部触发的lcd_switch
LOCAL MMIFILE_DEVICE_E      s_dev_ok_type[MMI_DEVICE_NUM] = {0}; //add by charlie.wang for multi sd card 2011.02.21

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : enter atv, init data, setting, ref etc.
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InitATV(void);

/*****************************************************************************/
//  Description : exit atv
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void ReleaseATV(void);

/*****************************************************************************/
//  Description : close atv relative window when exit
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void CloseRelativeWin(void);

/*****************************************************************************/
//  Description : play previous service
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void PlayPreService(void);

/*****************************************************************************/
//  Description : play next service
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void PlayNextService(void);

/*****************************************************************************/
//  Description : Handle get focus/lose focus interrupt
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void HandleLoseFocus(void);

/*****************************************************************************/
//  Description : handle lcd switch msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void HandleLcdSwitchMsg(void);

/*****************************************************************************/
//  Description : half/full screen switch
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void SwitchScreenMode(void);

/*****************************************************************************/
//  Description : set inner_lcd_switch flag
//  Global resource dependence : none
//  Author: xin.li
//  Note: 设置窗口旋屏时会触发lcd_switch消息，需要与外部G-Sensor等触发区分开来
/*****************************************************************************/
// LOCAL void SetInnerLcdSwitchFlag(BOOLEAN is_inner_switch);

/*****************************************************************************/
//  Description : is inner inner_lcd_switch
//  Global resource dependence : none
//  Author: xin.li
//  Note: 判断是否是内部触发的lcd_switch消息
/*****************************************************************************/
// LOCAL BOOLEAN IsInnerLcdSwitch(void);

/*****************************************************************************/
//  Description : set volume callback
//  Global resource dependence : none
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void SetVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr);

/*****************************************************************************/
//  Description : set volume callback
//  Global resource dependence : none
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void SetBrightnessCallback(MMICOM_PANEL_CALLBACK_T *para_ptr);

/*****************************************************************************/
//  Description : clear rect with atv color key or common background img
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void ClearRect(GUI_RECT_T rect, BOOLEAN is_fill_img);

/*****************************************************************************/
//  Description : clear full screen
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void ClearFullScreen(void);

/*****************************************************************************/
//  Description : create system timer
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void CreateSysTimer(uint32 type);

/*****************************************************************************/
//  Description : stop system timer
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void StopSysTimer(uint32 type);

/*****************************************************************************/
//  Description : display service number
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void DisplayServiceNumCallback(GUIOWNDRAW_INFO_T *owner_draw_ptr);

/*****************************************************************************/
//  Description : display search progress callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void DisplaySearchProgressCallback(GUIOWNDRAW_INFO_T *owner_draw_ptr);

/*****************************************************************************/
//  Description : 录制结束的后处理
//  Global resource dependence :
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void HandleATVRecordEnd(int32 type);

/*****************************************************************************/
//  Description : handle main win msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainWinMsg(
                                    MMI_WIN_ID_T win_id,
                                    MMI_MESSAGE_ID_E msg_id,
                                    DPARAM param
                                    );

/*****************************************************************************/
//  Description : handle main player win timer msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void HandleTimerMsg(uint8 timer_id);

#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
/*****************************************************************************/
//  Description : set Atv layout for 32x16_220X176 and 32x16_128X160
//  Global resource dependence :
//  Author: yanxian.zhou
//  Note: 
/*****************************************************************************/
LOCAL void SetAtvLayoutParam(void);
#else //6530  form
/*****************************************************************************/
//  Description : set form control param
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void SetFormParam(void);
#endif

/*****************************************************************************/
//  Description : button callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E VolButtonCallback(void);

/*****************************************************************************/
//  Description : button callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PreButtonCallback(void);

/*****************************************************************************/
//  Description : screen switch button callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ScreenButtonCallback(void);

/*****************************************************************************/
//  Description : screen switch button callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NextButtonCallback(void);

/*****************************************************************************/
//  Description : screen switch button callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RecordButtonCallback(void);

/*****************************************************************************/
//  Description : option button callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OptionButtonCallback(void);

/*****************************************************************************/
//  Description : option button callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ReturnButtonCallback(void);

/*****************************************************************************/
//  Description : 开启录像列表窗口
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void OpenRecordListWin(void);

/*****************************************************************************/
//  Description : handle main opt menu win msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainOptWinMsg(
                                       MMI_WIN_ID_T win_id,
                                       MMI_MESSAGE_ID_E msg_id,
                                       DPARAM param
                                       );

/*****************************************************************************/
//  Description : handle service list win msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleServiceListWinMsg(
                                           MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param
                                           );

/*****************************************************************************/
//  Description : handle favorite service list win msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFavoriteServiceListWinMsg(
                                                   MMI_WIN_ID_T win_id,
                                                   MMI_MESSAGE_ID_E msg_id,
                                                   DPARAM param
                                                   );
#ifndef MMI_PDA_SUPPORT//norflashcut_atv
/*****************************************************************************/
//  Description : handle favorite service list opt menu win msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFavoriteServiceListOptWinMsg(
                                                      MMI_WIN_ID_T win_id,
                                                      MMI_MESSAGE_ID_E msg_id,
                                                      DPARAM param
                                                      );
#endif

/*****************************************************************************/
//  Description : delete query win
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeleteQueryWinMsg(
                                           MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param
                                           );

/*****************************************************************************/
//  Description : delete all query win
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeleteAllQueryWinMsg(
                                              MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM param
                                              );

/*****************************************************************************/
//  Description : handle favorite service name edit win msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFavoriteServiceNameEidtWinMsg(
                                                       MMI_WIN_ID_T win_id,
                                                       MMI_MESSAGE_ID_E msg_id,
                                                       DPARAM param
                                                       );

/*****************************************************************************/
//  Description : handle select region win msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectRegionWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );
#ifndef MMI_PDA_SUPPORT //norflashcut_atv
#ifndef MMI_ATV_MINI_SUPPORT  //6530_MINI
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: aoke.hu
// Note:      
/*****************************************************************************/
LOCAL void AppendSettingListData(void);

/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: aoke.hu
// Note:      
/*****************************************************************************/
LOCAL void SaveSettingValue(void);

/*****************************************************************************/
//  Description : 设置窗口处理函数
//  Global resource dependence : 
//  Author: aoke.hu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          );
#endif //MMI_ATV_MINI_SUPPORT  //6530_MINI
#endif

/*****************************************************************************/
//  Description : handle favorite service name edit win msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHelpWinMsg(
                                    MMI_WIN_ID_T win_id,
                                    MMI_MESSAGE_ID_E msg_id,
                                    DPARAM param
                                    );

/*****************************************************************************/
//  Description : append common list item
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void LoadServiceList(uint16 start_index,
                           uint16 end_index,
                           BOOLEAN is_load_data);

/*****************************************************************************/
//  Description : load service list
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void LoadFavoriteServiceList(uint32 start_index,
                                   uint32 end_index,
                                   BOOLEAN is_load_data);

/*****************************************************************************/
//  Description : load atv region list
//  Global resource dependence : none
//  Author: xin.li
//  Note: 加载国家、地区列表
/*****************************************************************************/
LOCAL void LoadRegionList(uint32 start_index,
                          uint32 end_index,
                          BOOLEAN is_load_data);

/*****************************************************************************/
//  Description : set mute
//  Global resource dependence : none
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void SetMute(void);

/*****************************************************************************/
//  Description : display record tip
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordTip(void);

/*****************************************************************************/
//  Description : ATV字符的style风格
//  Global resource dependence : 
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL GUISTR_STYLE_T GetCommonTextStyle(void);

/*****************************************************************************/
//  Description : ATV字符的style风格
//  Global resource dependence : 
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL GUISTR_STATE_T GetCommonTextState(void);

/*****************************************************************************/
//  Description : 获取ATV通用字符的宽度
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetStringWidth(MMI_STRING_T  *string);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 更新一直显示的字串
/*****************************************************************************/
LOCAL void UpdateAlwaysTip(void);

//PDA style UI
#ifdef MMI_PDA_SUPPORT

/*****************************************************************************/
//  Description : 我的收藏 长按弹出选项消息处理.
//  Global resource dependence : 
//  Author: chunyou.zhu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFavoriteServiceListLongOkOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     );

/*****************************************************************************/
//  Description : 频道列表 长按选项消息处理.
//  Global resource dependence : 
//  Author: chunyou.zhu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleServiceListLongOkOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     );


#endif //MMI_PDA_SUPPORT

/*****************************************************************************/
//  Description : 设置播放器角度,禁止旋转
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetAtvWinAngle(LCD_ANGLE_E angle);

/*****************************************************************************/
//  Description : 使主界面消失
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisappearATVWin(MMI_WIN_ID_T  win_id,
                          BOOLEAN is_disappear,
                          BOOLEAN is_fresh);

/*****************************************************************************/
//  Description : create panel timer
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void CreatePanelTimer(void);

/*****************************************************************************/
//  Description : stop panel timer
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void StopPanelTimer(void);

/*****************************************************************************/
//  Description : turn brightness
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void adjustBrightness(MMICOM_PANEL_OPEN_TYPE_E turn_bright_type);

/*****************************************************************************/
//  Description : Handle get focusinterrupt
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void HandleGetFocus(void);

/*****************************************************************************/
//  Description : LoadMainOptMenu
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void LoadMainOptMenu(void);

/*****************************************************************************/
//  Description : handle main win direct key msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDirectKeyMsg(MMI_WIN_ID_T win_id,
                                    MMI_MESSAGE_ID_E msg_id,
                                    DPARAM param);

/*****************************************************************************/
//  Description : handle main win shortcut key msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShortCutKeyMsg(MMI_WIN_ID_T win_id,
                                    MMI_MESSAGE_ID_E msg_id,
                                    DPARAM param);

/*****************************************************************************/
//  Description : HandleFSMMsg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFSMMsg(MMI_WIN_ID_T win_id,
                                    MMI_MESSAGE_ID_E msg_id,
                                    DPARAM param);

/*****************************************************************************/
//  Description : stop record timer
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void StopRecordTimer(void);

/*****************************************************************************/
//  Description : start record timer
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void StartRecordTimer(void);

/*****************************************************************************/
//  Description : 视频播放区域清为透明色
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void ClearVideoRect(BOOLEAN is_clear_always);

/*****************************************************************************/
//  Description : 设置窗口处理函数
//  Global resource dependence : 
//  Author: aoke.hu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleATVSettingWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          );
/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/
#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmiatv_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
#include "mmiatv_id.def"    
};
#undef WIN_ID_DEF

//main win tab
WINDOW_TABLE(MMIATV_MAIN_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainWinMsg),
    WIN_ID(MMIATV_MAIN_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

#ifndef MMI_PDA_SUPPORT
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
WINDOW_TABLE(MMIATV_MAIN_WIN_TAB_RORM_H) = 
{      
    CREATE_OWNDRAW_CTRL(MMIATV_VIDEO_OWNDRAW_CTRL_ID, (uint32)DisplayServiceNumCallback),
    CREATE_OWNDRAW_CTRL(MMIATV_PROGRESS_OWNDRAW_CTRL_ID, (uint32)DisplaySearchProgressCallback),        
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIATV_TIP_LABEL_CTRL_ID),     
    CREATE_BUTTON_CTRL(PNULL, MMIATV_BUTTON_OPTION_CTRL_ID),
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)
    CREATE_BUTTON_CTRL(PNULL, MMIATV_BUTTON_PRE_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMIATV_BUTTON_NEXT_CTRL_ID), 
#else
    CREATE_BUTTON_CTRL(IMAGE_COMMON_BTN_PREV, MMIATV_BUTTON_PRE_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_COMMON_BTN_NEXT, MMIATV_BUTTON_NEXT_CTRL_ID), 
#endif
    CREATE_BUTTON_CTRL(PNULL, MMIATV_BUTTON_RETURN_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMIATV_BUTTON_OPTION_CTRL_ID),
    END_WIN             
};
WINDOW_TABLE(MMIATV_MAIN_WIN_TAB_RORM_V) = 
{      
    END_WIN
};
#else

WINDOW_TABLE(MMIATV_MAIN_WIN_TAB_RORM_V) = 
{      
    WIN_STATUSBAR,
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIATV_FORM_CTRL_ID),
        CHILD_OWNDRAW_CTRL(FALSE, MMIATV_VIDEO_OWNDRAW_CTRL_ID, MMIATV_FORM_CTRL_ID, (uint32)DisplayServiceNumCallback),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIATV_TIP_LABEL_CTRL_ID, MMIATV_FORM_CTRL_ID),
        CHILD_OWNDRAW_CTRL(FALSE, MMIATV_PROGRESS_OWNDRAW_CTRL_ID, MMIATV_FORM_CTRL_ID, (uint32)DisplaySearchProgressCallback),
        CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIATV_BUTTON_AREA_FORM_CTRL_ID,MMIATV_FORM_CTRL_ID),
            CHILD_OWNDRAW_CTRL(FALSE, MMIATV_BTN_OWNER_DRAW_CTRL_ID, MMIATV_BUTTON_AREA_FORM_CTRL_ID, PNULL),
            CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIATV_BUTTON_FORM_CTRL_ID,MMIATV_BUTTON_AREA_FORM_CTRL_ID),
#if (!defined MMI_RES_LOW_COST) || (!defined MMI_ATV_MINI_SUPPORT)                
                CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_PLAYER_BTN_VOL, MMIATV_BUTTON_VOL_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
#endif              
                CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_PREV, MMIATV_BUTTON_PRE_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
#if (!defined MMI_RES_LOW_COST) || (!defined MMI_ATV_MINI_SUPPORT)               
                CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_FULLSCREEN, MMIATV_BUTTON_SCREEN_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
#endif              
                CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_NEXT, MMIATV_BUTTON_NEXT_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
#ifdef MMI_ATV_RECORD_SUPPORT
#ifdef TOUCHPANEL_TYPE
                CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_RECORD, MMIATV_BUTTON_RECORD_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
#endif
#endif
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    END_WIN
};

//horizontal layout form for main win
WINDOW_TABLE(MMIATV_MAIN_WIN_TAB_RORM_H) = 
{      
    WIN_STATUSBAR,
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIATV_FORM_CTRL_ID),
        CHILD_OWNDRAW_CTRL(FALSE, MMIATV_VIDEO_OWNDRAW_CTRL_ID, MMIATV_FORM_CTRL_ID, (uint32)DisplayServiceNumCallback),
        CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIATV_PANEL_FORM_CTRL_ID,MMIATV_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIATV_TIP_LABEL_CTRL_ID, MMIATV_PANEL_FORM_CTRL_ID),
            CHILD_OWNDRAW_CTRL(FALSE, MMIATV_PROGRESS_OWNDRAW_CTRL_ID, MMIATV_PANEL_FORM_CTRL_ID, (uint32)DisplaySearchProgressCallback),
            CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIATV_BUTTON_AREA_FORM_CTRL_ID,MMIATV_PANEL_FORM_CTRL_ID),
                CHILD_OWNDRAW_CTRL(FALSE, MMIATV_BTN_OWNER_DRAW_CTRL_ID, MMIATV_BUTTON_AREA_FORM_CTRL_ID, PNULL),
                CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIATV_BUTTON_FORM_CTRL_ID,MMIATV_BUTTON_AREA_FORM_CTRL_ID),
                    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_OPTION, MMIATV_BUTTON_OPTION_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
#if (!defined MMI_RES_LOW_COST) || (!defined MMI_ATV_MINI_SUPPORT)                
                    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_PLAYER_BTN_VOL, MMIATV_BUTTON_VOL_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
#endif              
                    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_PREV, MMIATV_BUTTON_PRE_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
#if (!defined MMI_RES_LOW_COST) || (!defined MMI_ATV_MINI_SUPPORT)                
                    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_FULLSCREEN, MMIATV_BUTTON_SCREEN_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
#endif 
                    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_NEXT, MMIATV_BUTTON_NEXT_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
#ifdef MMI_ATV_RECORD_SUPPORT
    #ifdef TOUCHPANEL_TYPE
                    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_RECORD, MMIATV_BUTTON_RECORD_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
    #endif
#endif
                    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_RETURN, MMIATV_BUTTON_RETURN_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
    END_WIN
};
#endif
#else //pda style
//vertical layout form for main win
WINDOW_TABLE(MMIATV_MAIN_WIN_TAB_RORM_V) = 
{      
    WIN_STATUSBAR,
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIATV_FORM_CTRL_ID),
        CHILD_OWNDRAW_CTRL(FALSE, MMIATV_VIDEO_OWNDRAW_CTRL_ID, MMIATV_FORM_CTRL_ID, (uint32)DisplayServiceNumCallback),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIATV_TIP_LABEL_CTRL_ID, MMIATV_FORM_CTRL_ID),
        CHILD_OWNDRAW_CTRL(FALSE, MMIATV_PROGRESS_OWNDRAW_CTRL_ID, MMIATV_FORM_CTRL_ID, (uint32)DisplaySearchProgressCallback),  
        CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIATV_BUTTON_AREA_FORM_CTRL_ID,MMIATV_FORM_CTRL_ID),
            CHILD_OWNDRAW_CTRL(FALSE, MMIATV_BTN_OWNER_DRAW_CTRL_ID, MMIATV_BUTTON_AREA_FORM_CTRL_ID, PNULL),
            CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIATV_BUTTON_FORM_CTRL_ID,MMIATV_BUTTON_AREA_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_PLAYER_BTN_VOL, MMIATV_BUTTON_VOL_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_PREV, MMIATV_BUTTON_PRE_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_FULLSCREEN, MMIATV_BUTTON_SCREEN_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_NEXT, MMIATV_BUTTON_NEXT_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
#ifdef MMI_ATV_RECORD_SUPPORT
                CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_RECORD, MMIATV_BUTTON_RECORD_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),    
#endif
        WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    END_WIN
};

//horizontal layout form for main win
WINDOW_TABLE(MMIATV_MAIN_WIN_TAB_RORM_H) = 
{      
    WIN_STATUSBAR,
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIATV_FORM_CTRL_ID),
        CHILD_OWNDRAW_CTRL(FALSE, MMIATV_VIDEO_OWNDRAW_CTRL_ID, MMIATV_FORM_CTRL_ID, (uint32)DisplayServiceNumCallback),
        CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIATV_PANEL_FORM_CTRL_ID,MMIATV_FORM_CTRL_ID),

            CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIATV_TIP_LABEL_CTRL_ID, MMIATV_PANEL_FORM_CTRL_ID),
            CHILD_OWNDRAW_CTRL(FALSE, MMIATV_PROGRESS_OWNDRAW_CTRL_ID, MMIATV_PANEL_FORM_CTRL_ID, (uint32)DisplaySearchProgressCallback),
 
            CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIATV_BUTTON_AREA_FORM_CTRL_ID,MMIATV_PANEL_FORM_CTRL_ID),
                CHILD_OWNDRAW_CTRL(FALSE, MMIATV_BTN_OWNER_DRAW_CTRL_ID, MMIATV_BUTTON_AREA_FORM_CTRL_ID, PNULL),
                CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIATV_BUTTON_FORM_CTRL_ID,MMIATV_BUTTON_AREA_FORM_CTRL_ID),
                    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_OPTION, MMIATV_BUTTON_OPTION_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
                    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_PLAYER_BTN_VOL, MMIATV_BUTTON_VOL_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
                    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_PREV, MMIATV_BUTTON_PRE_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
                    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_FULLSCREEN, MMIATV_BUTTON_SCREEN_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
                    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_NEXT, MMIATV_BUTTON_NEXT_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
#ifdef MMI_ATV_RECORD_SUPPORT
                    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_RECORD, MMIATV_BUTTON_RECORD_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
#endif
                    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_RETURN, MMIATV_BUTTON_RETURN_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
    END_WIN
};

//Istyle vertical layout form
#if defined(MMI_ISTYLE_SUPPORT)
WINDOW_TABLE(MMIATV_ISTYLE_MAIN_WIN_TAB_RORM_V) = 
{      
    WIN_STATUSBAR,
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIATV_FORM_CTRL_ID),
        CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIATV_ISTYLE_TITLE_FORM_CTRL_ID, MMIATV_FORM_CTRL_ID),
            CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_RETURN, MMIATV_BUTTON_RETURN_CTRL_ID, MMIATV_ISTYLE_TITLE_FORM_CTRL_ID),
            CHILD_OWNDRAW_CTRL(FALSE,MMIATV_ISTYLE_DUMY_CTRL_ID, MMIATV_ISTYLE_TITLE_FORM_CTRL_ID,PNULL),
            CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_OPTION, MMIATV_BUTTON_OPTION_CTRL_ID, MMIATV_ISTYLE_TITLE_FORM_CTRL_ID),
    
        CHILD_OWNDRAW_CTRL(FALSE, MMIATV_VIDEO_OWNDRAW_CTRL_ID, MMIATV_FORM_CTRL_ID, (uint32)DisplayServiceNumCallback),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIATV_TIP_LABEL_CTRL_ID, MMIATV_FORM_CTRL_ID),
        CHILD_OWNDRAW_CTRL(FALSE, MMIATV_PROGRESS_OWNDRAW_CTRL_ID, MMIATV_FORM_CTRL_ID, (uint32)DisplaySearchProgressCallback),
    
        CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIATV_BUTTON_AREA_FORM_CTRL_ID,MMIATV_FORM_CTRL_ID),
            CHILD_OWNDRAW_CTRL(FALSE, MMIATV_BTN_OWNER_DRAW_CTRL_ID, MMIATV_BUTTON_AREA_FORM_CTRL_ID, PNULL),
            CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIATV_BUTTON_FORM_CTRL_ID,MMIATV_BUTTON_AREA_FORM_CTRL_ID),   
                CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_PLAYER_BTN_VOL, MMIATV_BUTTON_VOL_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_PREV, MMIATV_BUTTON_PRE_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_FULLSCREEN, MMIATV_BUTTON_SCREEN_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_NEXT, MMIATV_BUTTON_NEXT_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
#ifdef MMI_ATV_RECORD_SUPPORT
                CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_BTN_RECORD, MMIATV_BUTTON_RECORD_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID),
#endif
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
#endif//MMI_PDA_SUPPORT

WINDOW_TABLE(MMIATV_MAIN_OPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainOptWinMsg),
    WIN_ID(MMIATV_MAIN_OPT_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
    WIN_TITLE(STXT_OPTION),
    CREATE_MENU_CTRL(MMIATV_MAIN_OPT_MENU,MMIATV_MAIN_OPT_MENU_CTRL_ID),
#else
    CREATE_POPMENU_CTRL(MMIATV_MAIN_OPT_MENU,MMIATV_MAIN_OPT_MENU_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE(MMIATV_SERVICE_LIST_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleServiceListWinMsg),
    WIN_ID(MMIATV_SERVICE_LIST_WIN_ID),
#ifndef MMI_ATV_MINI_SUPPORT  //6530_MINI
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_COMMON_COMMON_CHANNEL_LIST),
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_ADD_FAVORITE, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIATV_SERVICE_LIST_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIATV_FAVORITE_SERVICE_LIST_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleFavoriteServiceListWinMsg),
    WIN_ID(MMIATV_FAVORITE_SERVICE_LIST_WIN_ID),
#ifndef MMI_ATV_MINI_SUPPORT  //6530_MINI
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_MAINMENU_FAVORITE),
    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_DELALL, TXT_NULL),
    #else
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    #endif //MMI_PDA_SUPPORT
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIATV_FAVORITE_SERVICE_LIST_CTRL_ID),
    END_WIN
};
#ifndef MMI_PDA_SUPPORT//norflashcut_atv
WINDOW_TABLE(MMIATV_FAVORITE_SERVICE_LIST_OPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleFavoriteServiceListOptWinMsg),
    WIN_ID(MMIATV_FAVORITE_SERVICE_LIST_OPT_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MMIATV_FAVORITE_LIST_OPT_MENU,MMIATV_FAVORITE_OPT_MENU_CTRL_ID),
    END_WIN
};
#endif

WINDOW_TABLE(MMIATV_FAVORITE_NAME_EDIT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleFavoriteServiceNameEidtWinMsg),
    WIN_ID(MMIATV_FAVORITE_NAME_EDIT_WIN_ID),
//    WIN_STATUSBAR,
    WIN_TITLE(TXT_EDIT),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#else
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    CREATE_EDIT_TEXT_CTRL(MMIATV_NAME_MAX_LEN, MMIATV_EDIT_FAVORITE_NAME_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIATV_SELECT_REGION_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleSelectRegionWinMsg),
    WIN_ID(MMIATV_SELECT_REGION_WIN_ID),
#ifndef MMI_ATV_MINI_SUPPORT  //6530_MINI
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_ATV_SELECT_REGION),
    #ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
    #else
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
    #endif //MMI_PDA_SUPPORT
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIATV_SELECT_REGION_LIST_CTRL_ID),
    END_WIN
};

#ifndef MMI_PDA_SUPPORT //norflashcut_atv
#ifndef MMI_ATV_MINI_SUPPORT  //6530_MINI
WINDOW_TABLE(MMIATV_SETTING_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleATVSettingWinMsg),
    WIN_ID(MMIATV_SETTING_WIN_ID),
#ifndef MMI_ATV_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(STXT_MAIN_SETTING),
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIATV_SETTING_FORM_CTRL_ID),
#ifdef MMIATV_RECORD_3GP_SUPPORT 
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIATV_RECORD_TYPE_FORM_CTRL_ID,MMIATV_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIATV_LABEL_RECORD_TYPE_CTRL_ID,MMIATV_RECORD_TYPE_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIATV_DROPDOWNLIST_RECORD_TYPE_CTRL_ID,MMIATV_RECORD_TYPE_FORM_CTRL_ID),
#endif
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIATV_RECORD_DEV_FORM_CTRL_ID,MMIATV_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIATV_LABEL_RECORD_DEV_CTRL_ID,MMIATV_RECORD_DEV_FORM_CTRL_ID),
        CHILD_DROPDOWNLIST_CTRL(TRUE,MMIATV_DROPDOWNLIST_RECORD_DEV_CTRL_ID,MMIATV_RECORD_DEV_FORM_CTRL_ID),

    END_WIN
};
#endif //end of MMI_ATV_MINI_SUPPORT  //6530_MINI
#endif

WINDOW_TABLE(MMIATV_HELP_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleHelpWinMsg),
    WIN_ID(MMIATV_HELP_WIN_ID),
#ifndef MMI_ATV_MINI_SUPPORT  //6530_MINI
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_HELP),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(MMIATV_HELP_TEXT_CTRL_ID),
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
//我的收藏长按选项窗口
WINDOW_TABLE( MMIATV_FAVORITE_SERVICE_LIST_LONGOK_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleFavoriteServiceListLongOkOptMenuWinMsg),
    WIN_ID(MMIATV_FAVORITE_SERVICE_LIST_LONGOK_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MMIATV_FAVORITE_SERVICE_LIST_LONGOK_OPT_MENU,MMIATV_FAVORITE_SERVICE_LIST_LONGOK_OPT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//频道列表长按选项窗口
WINDOW_TABLE( MMIATV_SERVICE_LIST_LONGOK_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleServiceListLongOkOptMenuWinMsg),
    WIN_ID(MMIATV_SERVICE_LIST_LONGOK_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MMIATV_SERVICE_LIST_LONGOK_OPT_MENU,MMIATV_SERVICE_LIST_LONGOK_OPT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif //MMI_PDA_SUPPORT

//atv region name table
LOCAL const MMI_TEXT_ID_T s_region_name_table[ATV_REGION_MAX] = {
    TXT_ATV_REGION_CHINA,
    TXT_ATV_REGION_HONGKONG,
    TXT_ATV_REGION_SHENZHEN,
    TXT_ATV_REGION_CAMBODIA,
    TXT_ATV_REGION_CANADA,
    TXT_ATV_REGION_CHILE,
    TXT_ATV_REGION_AFGHANISTAN,
    TXT_ATV_REGION_ARGENTINA,
    TXT_ATV_REGION_BRAZIL,
    TXT_ATV_REGION_INDONESIA,
    TXT_ATV_REGION_JAPAN,
    TXT_ATV_REGION_KOREA,
    TXT_ATV_REGION_LAOS,
    TXT_ATV_REGION_MALAYSIA,
    TXT_COMMON_WORLD_MEXICO,
    TXT_ATV_REGION_MYANMAR,
    TXT_ATV_REGION_PHILIPPINES,
    TXT_SINGAPORE,
    TXT_ATV_REGION_SOUTHAFRICA,
    TXT_ATV_REGION_TAIWAN,
    TXT_ATV_REGION_THAILAND,
    TXT_ATV_REGION_TURKEY,
    TXT_ATV_REGION_UAE,
    TXT_ATV_REGION_UK,
    TXT_ATV_REGION_USA,
    TXT_ATV_REGION_VENEZUELA,
    TXT_ATV_REGION_VIETNAM,
    TXT_ATV_REGION_WESTERNEUROPE,
    TXT_ATV_REGION_SPAIN,
    TXT_ATV_REGION_PORTUGAL,
    TXT_ATV_REGION_PAKISTAN,
    TXT_ATV_REGION_INDIA,
    TXT_ATV_REGION_AUSTRALIA,
    TXT_ATV_REGION_PARAGURY,
    TXT_ATV_REGION_URUGUAY,
    TXT_ATV_REGION_BULGARIA,
    TXT_ATV_REGION_CONGO,
    TXT_ATV_REGION_EGYPT,
    TXT_ATV_REGION_GERMANY,
    TXT_ATV_REGION_IRAQ,
    TXT_ATV_REGION_IRAN,
    TXT_ATV_REGION_HUNGRY,
    TXT_ATV_REGION_LIBYA,
    TXT_ATV_REGION_LEBANON,
    TXT_ATV_REGION_POLAND,
    TXT_ATV_REGION_ROMANIA,
    TXT_ATV_REGION_SANDIARABIA,
    TXT_ATV_REGION_TUNISIA,
    TXT_ATV_REGION_RUSSIA
};


/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Register atv win id
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_ATV, (const uint8**)mmiatv_id_name_arr);
}

/*****************************************************************************/
//  Description : open atv player main win
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_OpenATVMainWin(void)
{
    MMIPUB_OpenTempWaitWin();
#ifdef UI_P3D_SUPPORT
    MMI_Disable3DEffect(MMI_3D_EFFECT_ATV);
#endif  

    MMK_CreateWin((uint32*)MMIATV_MAIN_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : whether atv is open
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_IsATVOpen(void)   //通过状态机状态判断
{
    return MMK_IsOpenWin(MMIATV_MAIN_WIN_ID);
}

/*****************************************************************************/
//  Description : exit atv
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_Exit(void)
{
    if(MMK_IsOpenWin(MMIATV_MAIN_WIN_ID))
    {
        MMIPUB_CloseQuerytWin(PNULL);   //必须关掉期间开启的询问窗口
    }
    MMK_CloseMiddleWin(MMIATV_MAIN_WIN_ID, MMIATV_END_WIN_ID);

}

/*****************************************************************************/
//  Description : 录制结束的后处理
//  Global resource dependence :
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void HandleATVRecordEnd(int32 type)
{
    MMI_STRING_T text_str = {0};
    MMI_STRING_T lable_str = {0};
    MMI_STRING_T disk_str = {0};
    wchar        display_str[MMIATV_FULL_PATH_LEN +1] = {0};
    uint16       name_len = 0;
    wchar        space[2]={0x20,0};
    MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;

    SCI_TRACE_LOW("[ATV] HandleATVRecordEnd type = %d", type);
    
	MMIATV_EnableLcdSwitch(TRUE);
     
    
    if(type < 0) //致命错误
    {
        MMK_CloseWin(MMIATV_MAIN_WIN_ID);
        return;
    }
    else if(type == 0)
    {
        //显示空间不足加文件名
        MMIATV_DisplayCommonTipAlways(TXT_NO_SPACE);
		SCI_TRACE_LOW("[ATV] HandleATVRecordEnd TXT_NO_SPACE");
        
    }
    else
    {
        MMIAPISET_GetLanguageType(&lang_type);
        //显示文件名
        MMI_GetLabelTextByLang(TXT_ATV_RECORD_SAVED, &lable_str);
        MMI_WSTRNCPY(display_str,MMIATV_FULL_PATH_LEN,lable_str.wstr_ptr,lable_str.wstr_len,lable_str.wstr_len);
        MMI_GetLabelTextByLang(MMIATV_GetRealRecordDeviceName(), &disk_str);
    
        if( MMISET_LANGUAGE_SIMP_CHINESE == lang_type
            || MMISET_LANGUAGE_TRAD_CHINESE == lang_type)
        {
            name_len = MIN(disk_str.wstr_len, (uint16)(MMIATV_FULL_PATH_LEN - lable_str.wstr_len)); /*lint !e666*/
            MMIAPICOM_Wstrncat(display_str,disk_str.wstr_ptr,name_len);
            name_len = MIN(MMIAPICOM_Wstrlen(MMIATV_GetRecordFileName()), (uint16)(MMIATV_FULL_PATH_LEN - lable_str.wstr_len-disk_str.wstr_len)); /*lint !e666*/
            MMIAPICOM_Wstrncat(display_str,MMIATV_GetRecordFileName(),name_len);
        }
        else
        {
            MMIAPICOM_Wstrncat(display_str,space,1);
            name_len = MIN(disk_str.wstr_len, (uint16)(MMIATV_FULL_PATH_LEN - lable_str.wstr_len - 1)); /*lint !e666*/
            MMIAPICOM_Wstrncat(display_str,disk_str.wstr_ptr,name_len);
            MMIAPICOM_Wstrncat(display_str,space,1);
            name_len = MIN(MMIAPICOM_Wstrlen(MMIATV_GetRecordFileName()), (uint16)(MMIATV_FULL_PATH_LEN - lable_str.wstr_len-1-disk_str.wstr_len-1)); /*lint !e666*/
            MMIAPICOM_Wstrncat(display_str,MMIATV_GetRecordFileName(),name_len);
        }
        text_str.wstr_ptr = display_str;
        text_str.wstr_len = MMIAPICOM_Wstrlen(display_str);
        MMIATV_DisplayCommonTipByStr(&text_str);
    }
               
    MMIATV_UpdateButtonStatus(TRUE);
	if (!MMIATV_FSMPlay(s_atv_current_para.atv_handle, MMIATV_GetCurrentServiceID()))    
    {
        MMIATV_DisplayCommonTip(TXT_COMM_PLAY_ERROR);       
    }
    //MMK_PostMsg(MMIATV_MAIN_WIN_ID,MSG_APP_ATV_PLAY_SERVICE,PNULL,PNULL);    
}

/*****************************************************************************/
//  Description : handle main win direct key msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDirectKeyMsg(MMI_WIN_ID_T win_id,
                                    MMI_MESSAGE_ID_E msg_id,
                                    DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    switch(msg_id) 
    {   
    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
        PlayPreService();
        break;
        
    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        PlayNextService();
        break;
        
    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
        MMIATV_TurnVolume(MMICOM_PANEL_OPEN_INC);
        break;
        
    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        MMIATV_TurnVolume(MMICOM_PANEL_OPEN_DEC);
        break;
        
    case MSG_APP_UPSIDE:
    case MSG_KEYREPEAT_UPSIDE:
        MMIATV_TurnVolume(MMICOM_PANEL_OPEN_INC);
        break;
        
    case MSG_APP_DOWNSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
        MMIATV_TurnVolume(MMICOM_PANEL_OPEN_DEC);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}


/*****************************************************************************/
//  Description : handle main win shortcut key msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShortCutKeyMsg(MMI_WIN_ID_T win_id,
                                    MMI_MESSAGE_ID_E msg_id,
                                    DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    switch(msg_id) 
    {
    case MSG_APP_HASH:
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    case MSG_KEYDOWN_J:
#endif
        SetMute();
        break;
        
    case MSG_APP_STAR:
        if(MMIATV_IsRecording())
        {
            //必须先停录制
            MMIATV_DisplayCommonTip(TXT_ATV_STOP_RECORD_FISRT);//提示停录制
        }
        else if(MMIATV_IsSearching())
        {
            //搜索中不响应*键
        }
#ifndef MMI_PDA_SUPPORT
        else
        {
            MMK_CreateWin((uint32*)MMIATV_HELP_WIN_TAB,PNULL);
        }
#endif
        break;

    case MSG_APP_8:
#ifdef MMI_ATV_RECORD_SUPPORT
        if (!s_atv_current_para.is_display_panel)
        {
            StopPanelTimer();
            DisappearATVWin(win_id,FALSE,TRUE);
        }
        RecordButtonCallback();
#endif
        break;
    case MSG_APP_ATV_BRIGHTNESS:
    case MSG_APP_5:   
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    case MSG_KEYDOWN_G:
#endif               
            adjustBrightness(MMICOM_PANEL_OPEN_NONE);            
        break;

    case MSG_APP_4:
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    case MSG_KEYDOWN_F:
#endif
        adjustBrightness(MMICOM_PANEL_OPEN_DEC);
        break;

    case MSG_APP_6:
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    case MSG_KEYDOWN_H:
#endif
        adjustBrightness(MMICOM_PANEL_OPEN_INC);
        break; 

    default:
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : HandleFSMMsg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFSMMsg(MMI_WIN_ID_T win_id,
                                    MMI_MESSAGE_ID_E msg_id,
                                    DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    SCI_TRACE_LOW("[ATV] HandleFSMMsg msg_id = %d",msg_id);
    switch(msg_id) 
    {
    case MSG_ATV_NO_SPACE:
         if(MMI_DEVICE_NUM <= MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD))
        {
            MMIATV_DisplayCommonTip(TXT_NO_SD_CARD_ALERT);
        }
        else
        {
            MMIATV_DisplayCommonTip(TXT_NO_SPACE);
        }
        break;

    case MSG_APP_ATV_FATAL_ERROR:       
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_SCAN_PROCESS_IND:
        {
            ATV_CHN_INFO_T *chn_info_ptr = (ATV_CHN_INFO_T*) param;
            if (PNULL != chn_info_ptr)
            {
                SCI_TRACE_LOW("[ATV] HandleFSMMsg chn_is_valid = %d, search_cur_num = %d,search_total_num = %d, chn_info_ptr->chn_id = %d",chn_info_ptr->chn_is_valid,chn_info_ptr->search_cur_num,chn_info_ptr->search_total_num,chn_info_ptr->chn_id);
                if(0 != chn_info_ptr->chn_is_valid)
                {
                    //get a service
                    MMIATV_AddServiceNodeByID(chn_info_ptr->search_valid_num, chn_info_ptr->chn_id);
                }
                //update search progress
                MMIATV_DisplaySearchProgress(chn_info_ptr->search_cur_num, chn_info_ptr->search_total_num);
       
            }
        }
        break;

    case MSG_APP_ATV_RECORD_END:
		StopRecordTimer(); 
        if (PNULL != param)
        {
            HandleATVRecordEnd(*(int32 *)param);                
        }
          
        break;  

    case MSG_APP_ATV_SEARCH_CNF:
        //disable lcd switch
        MMIATV_EnableLcdSwitch(TRUE);
        MMIATV_ClearCommonTip();
        MMIATV_ClearSearchProgress();
    #ifndef MMI_PDA_SUPPORT
        MMIATV_UpdateButtonStatus(FALSE);
    #else
        MMIATV_UpdateButtonStatus(TRUE);
    #endif    
        MMK_PostMsg(MMIATV_MAIN_WIN_ID,MSG_APP_ATV_PLAY_SERVICE,PNULL,PNULL);        
        MMIATV_WriteServiceListFile();
        break;

    case MSG_APP_ATV_PLAY_SERVICE:
        if (MMK_IsFocusWin(win_id))
        {       
            GUI_LCD_DEV_INFO        lcd_dev_info = {0};
            GUIBLOCK_SetType(MMIATV_OSD_ALPHA, MMIATV_TRANSPARENT_COLOR, GUIBLOCK_TYPE_OSD, &lcd_dev_info);
            if (MMIATV_FSMPlay(s_atv_current_para.atv_handle, MMIATV_GetCurrentServiceID()))
            {
                MMIATV_DisplayServiceNum(); 
                UpdateAlwaysTip();
            }
            else
            {
                MMIATV_DisplayCommonTip(TXT_COMM_PLAY_ERROR);       
            }               
            MMIATV_UpdateButtonStatus(TRUE);
        }
        break;
        
    case MSG_APP_ATV_WEAK_SINGAL:
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
        if (MMIATV_IsSearching())
        {
            return result;//do not display weak signal tip during searching
        }
#endif
        MMIATV_DisplayCommonTipAlways(TXT_ATV_WEAK_SIGNAL);
        s_atv_current_para.always_text_id = TXT_ATV_WEAK_SIGNAL;
        if(s_atv_current_para.is_display_panel)
        {
            MAIN_SetTVSignalState();
        }
        break;
    
    case MSG_APP_ATV_NORMAL_SINGAL:
        MMIATV_ClearCommonTip();
        s_atv_current_para.always_text_id = TXT_NULL;
        if(s_atv_current_para.is_display_panel)
        {
            MAIN_SetTVSignalState();
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : handle main win msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainWinMsg(
                                    MMI_WIN_ID_T win_id,
                                    MMI_MESSAGE_ID_E msg_id,
                                    DPARAM param
                                    )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;   
    MMICOM_PANEL_CALLBACK_T para ={0};   
    GUI_LCD_DEV_INFO        lcd_dev_info = {0};

    msg_id = MMIAPICOM_ConvertKeyMsg(msg_id);
    SCI_TRACE_LOW("[MMIATV]: HandleMainWinMsg msg_id = 0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		SCI_MEMSET(&s_atv_current_para,0,sizeof( MMIATV_INFO_T));
        MMK_SetWinNeedCopyLcdInLoseFocus(win_id,FALSE);            
        //init eviroment
        MMIDEFAULT_TurnOnBackLight(); 
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
#ifdef MMI_ATV_SWITCH_SCREEN_ON_OPEN  //6530_MINI  进入后直接全屏播放
        if ( MMIATV_MAIN_WIN_ID == win_id && LCD_ANGLE_0 == MMK_GetScreenAngle())
       {
            MMK_SetWinSupportAngle(MMIATV_MAIN_WIN_ID, WIN_SUPPORT_ANGLE_90);
            MMK_SetWinAngle(MMIATV_MAIN_WIN_ID, LCD_ANGLE_90, TRUE);
       }
#endif
		GUIBLOCK_SetType(MMIATV_OSD_ALPHA, MMIATV_TRANSPARENT_COLOR, GUIBLOCK_TYPE_OSD, &lcd_dev_info);
        s_atv_current_para.atv_handle = MMIATV_FSMCreate();
            if (PNULL == s_atv_current_para.atv_handle)
            {
                MMK_CloseWin(win_id);
                break;
            }       
        
#ifdef PDA_UI_DROPDOWN_WIN
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_INVALID, TRUE); 
#endif
        //dynamic create form layout of win
#if defined MMI_PDA_SUPPORT  && defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            MMK_CreateCtrlByWinTabEx(win_id, MMITHEME_IsMainScreenLandscape() ? (uint32 *)MMIATV_MAIN_WIN_TAB_RORM_H : (uint32 *)MMIATV_ISTYLE_MAIN_WIN_TAB_RORM_V);
        }
        else
#endif
        {
            MMK_CreateCtrlByWinTabEx(win_id, MMITHEME_IsMainScreenLandscape() ? (uint32 *)MMIATV_MAIN_WIN_TAB_RORM_H : (uint32 *)MMIATV_MAIN_WIN_TAB_RORM_V);
        }
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
        SetAtvLayoutParam();
#else
        SetFormParam();
#endif
            s_atv_current_para.is_display_panel = TRUE; 
    
            s_atv_current_para.is_bright_panel = FALSE;
            s_atv_current_para.text_id = TXT_NULL;
            s_atv_current_para.always_text_id = TXT_NULL;
            if (MMITHEME_IsMainScreenLandscape())
            {
                CreatePanelTimer();
            }

#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif

        //enter atv
        if(!InitATV())
        {
            MMK_CloseWin(win_id);
            break;
        }
        //tv signal on status bar
        MAIN_SetTVSignalState();
        break;

    case MSG_ATV_SENSOR:        
        if (!MMIATV_IsRecording()
            && !MMIATV_IsSearching())
        {
            MMK_SetWinSupportAngle(win_id,WIN_SUPPORT_ANGLE_ALL);
        }       
        break;

    case MSG_FULL_PAINT:
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
        {
//            GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN}; //@pclint 578
            GUI_RECT_T Full_rect_h = ATV_FULLSCREEN_RECT_H;
            GUI_FillRect(&lcd_dev_info, Full_rect_h, MMI_BLACK_COLOR);
        }
#endif
        
        if (s_atv_current_para.is_bright_panel
            && MMK_IsFocusWin(win_id))
        {
            MMK_PostMsg(MMIATV_MAIN_WIN_ID, MSG_APP_ATV_BRIGHTNESS, PNULL, NULL);   
            s_atv_current_para.is_bright_panel = FALSE;
        }
        break;  
    
    case MSG_GET_FOCUS:
        GUIBLOCK_SetType(MMIATV_OSD_ALPHA, MMIATV_TRANSPARENT_COLOR, GUIBLOCK_TYPE_OSD, &lcd_dev_info);
        MMIDEFAULT_TurnOnBackLight(); 
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);     
        HandleGetFocus();        
        //init brightness
        para.cur_value = MMIATV_GetBrightness();
        SetBrightnessCallback(&para);
        break;
        
    case MSG_LOSE_FOCUS:
        GUIBLOCK_ResetMainType();   
        HandleLoseFocus();
        //reset brightness
        MMIAPISET_ResetContrast(MAIN_LCD_ID);
        break;

    case MSG_LCD_SWITCH:        
        HandleLcdSwitchMsg();
        break;
        
    case MSG_TIMER:
        HandleTimerMsg(*(uint8*)param);
        break;
        
    case MSG_CTL_MIDSK:
    //case MSG_CTL_PENOK:
    case MSG_APP_WEB:
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
        if (!s_atv_current_para.is_display_panel)
        {
            StopPanelTimer();
            DisappearATVWin(win_id,FALSE,TRUE);          
        }
#else
        SwitchScreenMode();
#endif
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_APP_MENU:
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if(!MMIATV_IsSearching())
        {
            if(MMIATV_IsRecording())
            {
                //必须先停录制
                MMIATV_DisplayCommonTip(TXT_ATV_STOP_RECORD_FISRT);//提示停录制
            }
            else
            {
                MMK_CreateWin((uint32*)MMIATV_MAIN_OPT_WIN_TAB,PNULL);
            } 
        }
        
        break;
        
    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:        
    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:        
    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:        
    case MSG_APP_UPSIDE:
    case MSG_KEYREPEAT_UPSIDE:        
    case MSG_APP_DOWNSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
        result = HandleDirectKeyMsg(win_id,msg_id,param);
        break;

    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        if (!s_atv_current_para.is_display_panel)
        {
            StopPanelTimer();
            DisappearATVWin(win_id,FALSE,TRUE);
        }
        else if(MMITHEME_IsMainScreenLandscape())
        {
            StopPanelTimer();
            CreatePanelTimer();         
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527       
    
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //SCI_TRACE_LOW:"[MMIATV]: HandleMainWinMsg  MSG_APP_CANCEL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_1196_112_2_18_2_3_16_82,(uint8*)"");
        if(MMIATV_IsSearching())
        {
            //stop search
            MMIATV_StopSearchService(s_atv_current_para.atv_handle);
            MMK_PostMsg(MMIATV_MAIN_WIN_ID,MSG_APP_ATV_PLAY_SERVICE,PNULL,PNULL);   
        }
#if(!defined MMI_RES_ORIENT_LANDSCAPE) && (!defined MMI_ATV_MINI_SUPPORT)
		else if (MMITHEME_IsMainScreenLandscape() 
                    && (GUILCD_GetInitAngle(GUI_MAIN_LCD_ID) != GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID)))
		{		
			if (!s_atv_current_para.is_display_panel)
			{
				s_atv_current_para.is_display_panel = TRUE;
			}
			SwitchScreenMode();
		}
#endif
        else
		{
			MMK_CloseWin(win_id);        
		}
        break;
        
    case MSG_KEYLONG_RED:
        //关机时需要关闭atv
        MMK_CloseWin(win_id);
        result = MMI_RESULT_FALSE;
        break;
        
    case MSG_CLOSE_WINDOW:  
        GUIBLOCK_ResetMainType();
        StopPanelTimer();
        //exit atv
        ReleaseATV();
        MMIATV_FSMRelease(s_atv_current_para.atv_handle);
		s_atv_current_para.atv_handle = 0;
        //restore enviroment
        MAIN_SetTVSignalFalse();
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_ATV);
#endif    
        break;

    case MSG_APP_HASH:        
    case MSG_APP_STAR:        
    case MSG_APP_8:
    case MSG_APP_ATV_BRIGHTNESS:
    case MSG_APP_5:      
    case MSG_APP_4:
    case MSG_APP_6:     
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    case MSG_KEYDOWN_J:
    case MSG_KEYDOWN_G:
    case MSG_KEYDOWN_F:
    case MSG_KEYDOWN_H:
#endif     
        result = HandleShortCutKeyMsg(win_id,msg_id,param);           
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if(MMIATV_VIDEO_OWNDRAW_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id
            && MMIATV_IsPlaying())
        {
            SwitchScreenMode();
        }
        break;  
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_ATV_PLAY_SERVICE:  
    case MSG_APP_ATV_FATAL_ERROR:       
    case MSG_APP_SCAN_PROCESS_IND:      
    case MSG_APP_ATV_RECORD_END:       
    case MSG_APP_ATV_SEARCH_CNF:    
    case MSG_APP_ATV_WEAK_SINGAL:
    case MSG_APP_ATV_NORMAL_SINGAL:
    case MSG_ATV_NO_SPACE:
        result = HandleFSMMsg(win_id,msg_id,param);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}



/*****************************************************************************/
//  Description : handle main player win timer msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void HandleTimerMsg(uint8 timer_id)
{
    if(s_atv_current_para.record_timer_id == timer_id)
    {
        DisplayRecordTip();
    }
    else if(s_atv_current_para.systimer_id == timer_id)
    {
        //service num
        if(MMIATV_SYSTIME_SERVICE_NUMBER == (s_atv_current_para.systimer_type & MMIATV_SYSTIME_SERVICE_NUMBER))
        {
            s_atv_current_para.service_num_count ++;
            if((MMIATV_2S_TIMEOUT * 2) == s_atv_current_para.service_num_count)
            {
                //SCI_TRACE_LOW("[MMIATV]: HandleTimerMsg MMIATV_SYSTIME_SERVICE_NUMBER timer_id=%d", s_atv_current_para.systimer_id);
                MMIATV_ClearServiceNum(TRUE);
                s_atv_current_para.service_num_count = 0;
            }
        }

        //common tip
        if(MMIATV_SYSTIME_COMMON_TIP == (s_atv_current_para.systimer_type & MMIATV_SYSTIME_COMMON_TIP))
        {
            s_atv_current_para.common_tip_count ++;
            if((MMIATV_2S_TIMEOUT * 2) == s_atv_current_para.common_tip_count)
            {
                //SCI_TRACE_LOW("[MMIATV]: HandleTimerMsg MMIATV_SYSTIME_COMMON_TIP timer_id=%d", s_atv_current_para.systimer_id);
                MMIATV_ClearCommonTip();
                UpdateAlwaysTip();
                s_atv_current_para.common_tip_count = 0;
            }
        }
    }
    else if (s_atv_current_para.panel_timer_id == timer_id)
    {
        if (MMITHEME_IsMainScreenLandscape()
            && (MMIATV_IsPlaying()
            || MMIATV_IsSettingCHN()
			|| MMIATV_IsRecording()))
        {
            DisappearATVWin(MMIATV_MAIN_WIN_ID,TRUE,TRUE);
        }       
        else if (MMITHEME_IsMainScreenLandscape()
            && s_atv_current_para.is_display_panel)
        {
            s_atv_current_para.panel_timer_id = 0;
            CreatePanelTimer();
        }
        else
        {
            s_atv_current_para.panel_timer_id = 0;
        }
    }
}

#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
/*****************************************************************************/
//  Description : set Atv layout for 32X16_220X and 32x16_128X160
//  Global resource dependence :
//  Author: yanxian.zhou
//  Note: 
/*****************************************************************************/
LOCAL void SetAtvLayoutParam(void)
{
    GUI_BG_T bg = {0};
    GUI_BG_T fg = {0};

    GUI_RECT_T option_rect_h = ATV_LAYOUT_OPT_RECT_H;
    GUI_RECT_T pre_rect_h = ATV_LAYOUT_PREV_RECT_H;
    GUI_RECT_T next_rect_h = ATV_LAYOUT_NEXT_RECT_H;
    GUI_RECT_T return_rect_h = ATV_LAYOUT_RET_RECT_H;
    GUI_RECT_T progress_rect_h = ATV_LAYOUT_PROC_RECT_H;
    GUI_RECT_T num_rect_h = ATV_LAYOUT_NUM_RECT_H;
    GUI_RECT_T tip_rect_h = ATV_LAYOUT_TIP_RECT_H;

    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T Full_rect_h = ATV_FULLSCREEN_RECT_H;

    GUI_FillRect(&lcd_dev_info, Full_rect_h, MMI_BLACK_COLOR);
    //button callback
#ifdef TOUCH_PANEL_SUPPORT
    GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_OPTION_CTRL_ID, OptionButtonCallback);
    GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_PRE_CTRL_ID, PreButtonCallback);
    GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_NEXT_CTRL_ID, NextButtonCallback);
    GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_RETURN_CTRL_ID, ReturnButtonCallback); 
#endif
    
    GUIBUTTON_SetRect(MMIATV_BUTTON_OPTION_CTRL_ID, &option_rect_h);
    GUIBUTTON_SetRect(MMIATV_BUTTON_PRE_CTRL_ID, &pre_rect_h);
    GUIBUTTON_SetRect(MMIATV_BUTTON_NEXT_CTRL_ID, &next_rect_h);
    GUIBUTTON_SetRect(MMIATV_BUTTON_RETURN_CTRL_ID, &return_rect_h);

    GUILABEL_SetRect(MMIATV_TIP_LABEL_CTRL_ID, &tip_rect_h, FALSE);

    GUIOWNDRAW_SetRect(MMIATV_PROGRESS_OWNDRAW_CTRL_ID, &progress_rect_h);
    GUIOWNDRAW_SetRect(MMIATV_VIDEO_OWNDRAW_CTRL_ID, &num_rect_h);
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)
    bg.bg_type = GUI_BG_IMG;
    bg.img_id  = IMAGE_ATV_BTN_BG;

    GUIBUTTON_SetBg(MMIATV_BUTTON_OPTION_CTRL_ID, &bg);
    GUIBUTTON_SetBg(MMIATV_BUTTON_PRE_CTRL_ID, &bg);
    GUIBUTTON_SetBg(MMIATV_BUTTON_NEXT_CTRL_ID, &bg);
    GUIBUTTON_SetBg(MMIATV_BUTTON_RETURN_CTRL_ID, &bg);

    fg.bg_type = GUI_BG_IMG;
    fg.img_id  = IMAGE_COMMON_BTN_OPTION;
    GUIBUTTON_SetFg(MMIATV_BUTTON_OPTION_CTRL_ID, &fg);
    fg.img_id  = IMAGE_COMMON_BTN_PREV;
    GUIBUTTON_SetFg(MMIATV_BUTTON_PRE_CTRL_ID, &fg);
    fg.img_id  = IMAGE_COMMON_BTN_NEXT;
    GUIBUTTON_SetFg(MMIATV_BUTTON_NEXT_CTRL_ID, &fg);
    fg.img_id  = IMAGE_COMMON_BTN_RETURN;
    GUIBUTTON_SetFg(MMIATV_BUTTON_RETURN_CTRL_ID, &fg);
#else
    GUIBUTTON_SetTextId(MMIATV_BUTTON_OPTION_CTRL_ID, STXT_OPTION);
    GUIBUTTON_SetTextId(MMIATV_BUTTON_RETURN_CTRL_ID, STXT_RETURN);
	GUIBUTTON_SetTextAlign(MMIATV_BUTTON_OPTION_CTRL_ID, ALIGN_RVMIDDLE);
	GUIBUTTON_SetTextAlign(MMIATV_BUTTON_RETURN_CTRL_ID, ALIGN_RVMIDDLE);
#endif
}
#else  //6530 form
/*****************************************************************************/
//  Description : set form control param
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void SetFormParam(void)
{
    MMI_HANDLE_T win_id = MMIATV_MAIN_WIN_ID;
    GUI_BG_T    bg = {0};
    GUI_BG_T    statusbar_bg = {0};
    uint16      v_space = 0;
    GUIFORM_CHILD_WIDTH_T button_width = {0};
    GUI_BOTH_RECT_T both_rect = {0};
    GUI_RECT_T rect = {0};
    uint16  button_height = 0;
    uint16  process_height = MMIATV_PROGRESS_GROOVE_HEIGHT;
    uint16  h_space = 0;
    uint16 button_area_height = 0;
#ifndef MMI_PDA_SUPPORT
    uint16 softkey_height = MMITHEME_GetSoftkeyHeight();
#else
    uint16 softkey_height = 0;
#endif
    //set statusbar background color
    statusbar_bg.bg_type = GUI_BG_COLOR;
    statusbar_bg.img_id = MMI_BLACK_COLOR;
    GUIWIN_SetStbBg(win_id, statusbar_bg);


    //set form not support slide
    GUIFORM_IsSlide(MMIATV_FORM_CTRL_ID,FALSE);
    GUIFORM_IsSlide(MMIATV_BUTTON_FORM_CTRL_ID,FALSE);
    GUIFORM_IsSlide(MMIATV_BUTTON_AREA_FORM_CTRL_ID,FALSE);
    GUIFORM_IsSlide(MMIATV_PANEL_FORM_CTRL_ID,FALSE);
    
    //set form background color
    bg.bg_type = GUI_BG_COLOR;
    bg.color = MMI_BLACK_COLOR;
    GUIFORM_SetBg(MMIATV_FORM_CTRL_ID, &bg);
    GUIFORM_SetMargin(MMIATV_FORM_CTRL_ID, 0);

    

    
    
    both_rect = MMITHEME_GetFullScreenBothRect();   
    

    GUIBUTTON_PermitBorder(MMIATV_BUTTON_OPTION_CTRL_ID, FALSE);
    GUIBUTTON_PermitBorder(MMIATV_BUTTON_VOL_CTRL_ID, FALSE);
    GUIBUTTON_PermitBorder(MMIATV_BUTTON_PRE_CTRL_ID, FALSE);
    GUIBUTTON_PermitBorder(MMIATV_BUTTON_SCREEN_CTRL_ID, FALSE);
    GUIBUTTON_PermitBorder(MMIATV_BUTTON_NEXT_CTRL_ID, FALSE);
    GUIBUTTON_PermitBorder(MMIATV_BUTTON_RECORD_CTRL_ID, FALSE);
    GUIBUTTON_PermitBorder(MMIATV_BUTTON_RETURN_CTRL_ID, FALSE);

    GUIBUTTON_SetRunSheen(MMIATV_BUTTON_OPTION_CTRL_ID,FALSE);
    GUIBUTTON_SetRunSheen(MMIATV_BUTTON_VOL_CTRL_ID,FALSE);
    GUIBUTTON_SetRunSheen(MMIATV_BUTTON_PRE_CTRL_ID,FALSE);
    GUIBUTTON_SetRunSheen(MMIATV_BUTTON_SCREEN_CTRL_ID,FALSE);
    GUIBUTTON_SetRunSheen(MMIATV_BUTTON_NEXT_CTRL_ID,FALSE);
    GUIBUTTON_SetRunSheen(MMIATV_BUTTON_RECORD_CTRL_ID,FALSE);
    GUIBUTTON_SetRunSheen(MMIATV_BUTTON_RETURN_CTRL_ID,FALSE);

    if(MMITHEME_IsMainScreenLandscape())
    {       

        bg.bg_type = GUI_BG_COLOR;  
        bg.color = MMI_BLACK_COLOR;
        //bg.img_id = IMAGE_COMMON_BTN_BG;  
        GUIFORM_SetBg(MMIATV_PANEL_FORM_CTRL_ID, &bg);
        GUIFORM_SetMargin(MMIATV_PANEL_FORM_CTRL_ID, 0);

        rect.left = 0;
        rect.top = MMIATV_VIDEO_OWNDRAW_HEIGHT_H + MMITHEME_GetStatusBarHeight();
        rect.right = both_rect.h_rect.right;
        rect.bottom = both_rect.h_rect.bottom;
        GUIFORM_SetRect(MMIATV_PANEL_FORM_CTRL_ID, &rect);   

        bg.bg_type = GUI_BG_IMG;    
        bg.img_id = IMAGE_COMMON_BTN_BG;
        GUIFORM_SetBg(MMIATV_BUTTON_FORM_CTRL_ID, &bg);
        GUIFORM_SetMargin(MMIATV_BUTTON_FORM_CTRL_ID, 0);

        //设置form区域
        rect.left = 0;
        rect.top = MMITHEME_GetStatusBarHeight();
        rect.right = both_rect.h_rect.right;
        rect.bottom = both_rect.h_rect.bottom;
        GUIFORM_SetRect(MMIATV_FORM_CTRL_ID, &rect);   
        //横屏模式参数设置       

        //设置form区域  
        button_area_height = MMIATV_BUTTONFORM_H_HEIGHT;
        GUIRES_GetImgWidthHeight(PNULL, &button_height, IMAGE_COMMON_BTN_PLAY, MMIATV_MAIN_WIN_ID); 
        v_space = (button_area_height - button_height + 1)/2;

        rect.left = 0;
        rect.top = both_rect.h_rect.bottom - button_area_height + 1;
        rect.right = both_rect.h_rect.right;
        rect.bottom = both_rect.h_rect.bottom;
        GUIFORM_SetRect(MMIATV_BUTTON_AREA_FORM_CTRL_ID, &rect);  
        v_space = 0;
        GUIFORM_SetChildSpace(MMIATV_FORM_CTRL_ID, MMIATV_BUTTON_AREA_FORM_CTRL_ID, PNULL, &v_space);

        rect.left = 0;
        rect.top = rect.top + v_space;
        rect.right = both_rect.h_rect.right;
        rect.bottom = both_rect.h_rect.bottom;
        GUIFORM_SetRect(MMIATV_BUTTON_FORM_CTRL_ID, &rect);  
        v_space = 0;
        GUIFORM_SetChildSpace(MMIATV_BUTTON_AREA_FORM_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID, PNULL, &v_space);
       

        //video
        GUIOWNDRAW_SetHeight(MMIATV_VIDEO_OWNDRAW_CTRL_ID, MMIATV_VIDEO_OWNDRAW_HEIGHT_H);
        
        //tip lable
        v_space = MMIATV_PROGRESSBAR_H_SPACE;
        GUIFORM_SetChildSpace(MMIATV_FORM_CTRL_ID, MMIATV_TIP_LABEL_CTRL_ID, PNULL, &v_space);
        GUILABEL_SetFont(MMIATV_TIP_LABEL_CTRL_ID, MMIATV_NORMAL_FONT, MMI_GRAY_WHITE_COLOR);
        GUILABEL_SetMargin(MMIATV_TIP_LABEL_CTRL_ID, 0);
        
        //progress
        v_space = MMIATV_PROGRESSBAR_H_SPACE;
        GUIFORM_SetChildSpace(MMIATV_FORM_CTRL_ID, MMIATV_PROGRESS_OWNDRAW_CTRL_ID, PNULL, &v_space);
        GUIOWNDRAW_SetHeight(MMIATV_PROGRESS_OWNDRAW_CTRL_ID, process_height);
        
        //button callback
        GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_OPTION_CTRL_ID, OptionButtonCallback);
        GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_VOL_CTRL_ID, VolButtonCallback);
        GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_PRE_CTRL_ID, PreButtonCallback);
        GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_SCREEN_CTRL_ID, ScreenButtonCallback);
        GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_NEXT_CTRL_ID, NextButtonCallback);
#ifdef MMI_ATV_RECORD_SUPPORT
#ifdef TOUCHPANEL_TYPE
        GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_RECORD_CTRL_ID, RecordButtonCallback);
#endif
#endif
        GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_RETURN_CTRL_ID, ReturnButtonCallback);
      
        GUIFORM_SetAlign(MMIATV_BUTTON_FORM_CTRL_ID, GUIFORM_CHILD_ALIGN_HMIDDLE);
        button_width.type = GUIFORM_CHILD_WIDTH_FIXED;        
        v_space = (button_area_height - button_height + 1)/2;
        GUIOWNDRAW_SetHeight(MMIATV_BTN_OWNER_DRAW_CTRL_ID,v_space);
#if (!defined MMI_RES_LOW_COST) || (!defined MMI_ATV_MINI_SUPPORT)
        //option
        h_space =  MMIATV_FORM_BUTTON_SPACE_H;
        GUIFORM_SetChildSpace(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_OPTION_CTRL_ID, &h_space, PNULL);       
        GUIRES_GetImgWidthHeight(&button_width.add_data, PNULL, IMAGE_COMMON_PLAYER_BTN_VOL, MMIATV_MAIN_WIN_ID);       
        GUIFORM_SetChildWidth(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_OPTION_CTRL_ID, &button_width);

        //vol
        h_space =  MMIATV_BUTTON_BUTTON_SPACE_H;
        GUIFORM_SetChildSpace(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_VOL_CTRL_ID, &h_space, PNULL);      
        GUIRES_GetImgWidthHeight(&button_width.add_data, PNULL, IMAGE_COMMON_PLAYER_BTN_VOL, MMIATV_MAIN_WIN_ID);       
        GUIFORM_SetChildWidth(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_VOL_CTRL_ID, &button_width);
#endif
        //pre
        h_space = MMIATV_BUTTON_BUTTON_SPACE_H;
        GUIFORM_SetChildSpace(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_PRE_CTRL_ID,  &h_space, PNULL);     
        GUIRES_GetImgWidthHeight(&button_width.add_data, PNULL, IMAGE_COMMON_BTN_PREV, MMIATV_MAIN_WIN_ID);   
        GUIFORM_SetChildWidth(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_PRE_CTRL_ID, &button_width);
#if !defined(MMI_RES_ORIENT_LANDSCAPE)
#if (!defined MMI_RES_LOW_COST) || (!defined MMI_ATV_MINI_SUPPORT)
        //screen
        h_space = MMIATV_BUTTON_BUTTON_SPACE_H;
        GUIFORM_SetChildSpace(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_SCREEN_CTRL_ID, &h_space, PNULL);       
        GUIRES_GetImgWidthHeight(&button_width.add_data, PNULL, IMAGE_COMMON_BTN_FULLSCREEN, MMIATV_MAIN_WIN_ID); 
        GUIFORM_SetChildWidth(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_SCREEN_CTRL_ID, &button_width);
#endif
#endif
        //next
        h_space = MMIATV_BUTTON_BUTTON_SPACE_H;
        GUIFORM_SetChildSpace(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_NEXT_CTRL_ID, &h_space, PNULL);     
        GUIRES_GetImgWidthHeight(&button_width.add_data, PNULL, IMAGE_COMMON_BTN_NEXT, MMIATV_MAIN_WIN_ID); 
        GUIFORM_SetChildWidth(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_NEXT_CTRL_ID, &button_width);
#ifdef TOUCHPANEL_TYPE
        //snapshot
        h_space = MMIATV_BUTTON_BUTTON_SPACE_H;
        GUIFORM_SetChildSpace(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_RECORD_CTRL_ID, &h_space, PNULL);       
        GUIRES_GetImgWidthHeight(&button_width.add_data, PNULL, IMAGE_COMMON_BTN_RECORD, MMIATV_MAIN_WIN_ID); 
        GUIFORM_SetChildWidth(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_RECORD_CTRL_ID, &button_width);      
#endif
#if (!defined MMI_RES_LOW_COST) || (!defined MMI_ATV_MINI_SUPPORT)        
        //return
        h_space = MMIATV_BUTTON_BUTTON_SPACE_H;
        GUIFORM_SetChildSpace(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_RETURN_CTRL_ID, &h_space, PNULL);       
        GUIRES_GetImgWidthHeight(&button_width.add_data, PNULL, IMAGE_COMMON_BTN_RECORD, MMIATV_MAIN_WIN_ID); 
        GUIFORM_SetChildWidth(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_RETURN_CTRL_ID, &button_width);        
#endif       
      
    }
    else
    {   

        bg.bg_type = GUI_BG_COLOR;  
        bg.img_id = MMI_BLACK_COLOR;
        GUIFORM_SetBg(MMIATV_BUTTON_FORM_CTRL_ID, &bg);
        GUIFORM_SetMargin(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_FORM_BUTTON_SPACE);    

        bg.bg_type = GUI_BG_COLOR;
        bg.color = MMI_BLACK_COLOR;
        GUIFORM_SetBg(MMIATV_BUTTON_AREA_FORM_CTRL_ID, &bg);
        GUIFORM_SetMargin(MMIATV_BUTTON_AREA_FORM_CTRL_ID, 0); 

        //设置form区域
        rect.left = 0;
        rect.top = MMITHEME_GetStatusBarHeight();
        rect.right = both_rect.v_rect.right;
        rect.bottom = both_rect.v_rect.bottom - softkey_height;
        GUIFORM_SetRect(MMIATV_FORM_CTRL_ID, &rect);   

         //竖屏模式参数设置
        GUIRES_GetImgWidthHeight(PNULL, &button_area_height, IMAGE_COMMON_BTN_BG, MMIATV_MAIN_WIN_ID);
        GUIRES_GetImgWidthHeight(PNULL, &button_height, IMAGE_COMMON_BTN_PLAY, MMIATV_MAIN_WIN_ID);         

        rect.left = 0;
        rect.top = both_rect.v_rect.bottom - softkey_height - button_area_height + 1;
        rect.right = both_rect.v_rect.right;
        rect.bottom = both_rect.v_rect.bottom - softkey_height;
        GUIFORM_SetRect(MMIATV_BUTTON_AREA_FORM_CTRL_ID, &rect);
        v_space = 0;
        GUIFORM_SetChildSpace(MMIATV_FORM_CTRL_ID, MMIATV_BUTTON_AREA_FORM_CTRL_ID, PNULL, &v_space);

        v_space = (button_area_height - button_height + 1)/2;
        rect.left = 0;
        rect.top = rect.top + v_space;
        rect.right = both_rect.v_rect.right;
        rect.bottom = both_rect.v_rect.bottom - softkey_height;
        GUIFORM_SetRect(MMIATV_BUTTON_FORM_CTRL_ID, &rect);  
        v_space = 0;
        GUIFORM_SetChildSpace(MMIATV_BUTTON_AREA_FORM_CTRL_ID, MMIATV_BUTTON_FORM_CTRL_ID, PNULL, &v_space);

        //video
#if defined(MMI_ISTYLE_SUPPORT)        
        if(MMITHEME_IsIstyle()) //Istyle布局稍有差异
        {
            uint16  btn_img_width = 0;
            //uint16  btn_img_height = 0;
            GUIFORM_CHILD_HEIGHT_T  istyle_form_height = {0}; 
            GUIFORM_CHILD_WIDTH_T   istyle_btn_width = {0};
            
            GUIRES_GetImgWidthHeight(&btn_img_width, PNULL, IMAGE_COMMON_BTN_OPTION, win_id);
            
            //set istyle title form height
            GUIFORM_SetMargin(MMIATV_ISTYLE_TITLE_FORM_CTRL_ID, 0);
            istyle_form_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            istyle_form_height.add_data = MMIATV_ISTYLE_TITLE_HEIGHT;
            GUIFORM_SetChildHeight(MMIATV_FORM_CTRL_ID, MMIATV_ISTYLE_TITLE_FORM_CTRL_ID, &istyle_form_height);
            
            //button layout
            istyle_btn_width.type = GUIFORM_CHILD_WIDTH_FIXED;
            istyle_btn_width.add_data = btn_img_width;
            GUIFORM_SetChildWidth(MMIATV_ISTYLE_TITLE_FORM_CTRL_ID,MMIATV_BUTTON_RETURN_CTRL_ID,&istyle_btn_width);
            GUIFORM_SetChildWidth(MMIATV_ISTYLE_TITLE_FORM_CTRL_ID,MMIATV_BUTTON_OPTION_CTRL_ID,&istyle_btn_width);
            
            //set owndraw height
            v_space = 0;
            GUIFORM_SetChildSpace(MMIATV_FORM_CTRL_ID, MMIATV_VIDEO_OWNDRAW_CTRL_ID, PNULL, &v_space);
            GUIOWNDRAW_SetHeight(MMIATV_VIDEO_OWNDRAW_CTRL_ID, MMIATV_VIDEO_OWNDRAW_HEIGHT - MMIATV_ISTYLE_TITLE_HEIGHT);
        }
        else
#endif			
        {
            GUIOWNDRAW_SetHeight(MMIATV_VIDEO_OWNDRAW_CTRL_ID, MMIATV_VIDEO_OWNDRAW_HEIGHT);
        }
        
        v_space = MMIATV_PROGRESSBAR_SPACE;
        //tip lable
        GUIFORM_SetChildSpace(MMIATV_FORM_CTRL_ID, MMIATV_TIP_LABEL_CTRL_ID, PNULL, &v_space);
        GUILABEL_SetFont(MMIATV_TIP_LABEL_CTRL_ID, MMIATV_NORMAL_FONT, MMI_GRAY_WHITE_COLOR);
        GUILABEL_SetMargin(MMIATV_TIP_LABEL_CTRL_ID, 0);
        
        //progress
        v_space = MMIATV_PROGRESSBAR_SPACE;
        GUIFORM_SetChildSpace(MMIATV_FORM_CTRL_ID, MMIATV_PROGRESS_OWNDRAW_CTRL_ID, PNULL, &v_space);
        GUIOWNDRAW_SetHeight(MMIATV_PROGRESS_OWNDRAW_CTRL_ID, process_height);
        
        //button callback
        GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_VOL_CTRL_ID, VolButtonCallback);
        GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_PRE_CTRL_ID, PreButtonCallback);
        GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_SCREEN_CTRL_ID, ScreenButtonCallback);
        GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_NEXT_CTRL_ID, NextButtonCallback);
#ifdef MMI_ATV_RECORD_SUPPORT
#ifdef TOUCHPANEL_TYPE
        GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_RECORD_CTRL_ID, RecordButtonCallback);       
#endif
#endif
#if defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_RETURN_CTRL_ID, ReturnButtonCallback);
            GUIBUTTON_SetCallBackFunc(MMIATV_BUTTON_OPTION_CTRL_ID, OptionButtonCallback);
        }
#endif        

        v_space = (button_area_height - button_height + 1)/2;
        GUIOWNDRAW_SetHeight(MMIATV_BTN_OWNER_DRAW_CTRL_ID,v_space);

        //buttons display
        GUIFORM_SetAlign(MMIATV_BUTTON_FORM_CTRL_ID,GUIFORM_CHILD_ALIGN_HMIDDLE);
        button_width.type = GUIFORM_CHILD_WIDTH_FIXED;      
        

        //vol
        h_space =  MMIATV_FORM_BUTTON_SPACE;
        GUIFORM_SetChildSpace(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_VOL_CTRL_ID, &h_space, PNULL);      
        GUIRES_GetImgWidthHeight(&button_width.add_data, PNULL, IMAGE_COMMON_PLAYER_BTN_VOL, MMIATV_MAIN_WIN_ID);       
        GUIFORM_SetChildWidth(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_VOL_CTRL_ID, &button_width);

        //pre
        h_space = MMIATV_BUTTON_BUTTON_SPACE;
        GUIFORM_SetChildSpace(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_PRE_CTRL_ID,  &h_space, PNULL);     
        GUIRES_GetImgWidthHeight(&button_width.add_data, PNULL, IMAGE_COMMON_BTN_PREV, MMIATV_MAIN_WIN_ID);   
        GUIFORM_SetChildWidth(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_PRE_CTRL_ID, &button_width);

        //screen
        h_space = MMIATV_BUTTON_BUTTON_SPACE;
        GUIFORM_SetChildSpace(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_SCREEN_CTRL_ID, &h_space, PNULL);       
        GUIRES_GetImgWidthHeight(&button_width.add_data, PNULL, IMAGE_COMMON_BTN_FULLSCREEN, MMIATV_MAIN_WIN_ID); 
        GUIFORM_SetChildWidth(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_SCREEN_CTRL_ID, &button_width);

        //next
        h_space = MMIATV_BUTTON_BUTTON_SPACE;
        GUIFORM_SetChildSpace(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_NEXT_CTRL_ID, &h_space, PNULL);     
        GUIRES_GetImgWidthHeight(&button_width.add_data, PNULL, IMAGE_COMMON_BTN_NEXT, MMIATV_MAIN_WIN_ID); 
        GUIFORM_SetChildWidth(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_NEXT_CTRL_ID, &button_width);
#ifdef TOUCHPANEL_TYPE
        //snapshot
        h_space = MMIATV_BUTTON_BUTTON_SPACE;
        GUIFORM_SetChildSpace(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_RECORD_CTRL_ID, &h_space, PNULL);       
        GUIRES_GetImgWidthHeight(&button_width.add_data, PNULL, IMAGE_COMMON_BTN_RECORD, MMIATV_MAIN_WIN_ID); 
        GUIFORM_SetChildWidth(MMIATV_BUTTON_FORM_CTRL_ID, MMIATV_BUTTON_RECORD_CTRL_ID, &button_width);             
#endif
    }
}
#endif //MMI_ATV_MINI_SUPPORT
/*****************************************************************************/
//  Description : button callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E VolButtonCallback(void)
{
    MMI_RESULT_E return_code = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"[MMIATV]:VolButtonCallback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_1624_112_2_18_2_3_16_84,(uint8*)"");    

    MMIATV_TurnVolume(MMICOM_PANEL_OPEN_NONE);
    
    return return_code;
}

/*****************************************************************************/
//  Description : button callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PreButtonCallback(void)
{
    MMI_RESULT_E return_code = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"[MMIATV]:PreButtonCallback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_1643_112_2_18_2_3_16_85,(uint8*)"");
    
    PlayPreService();
    
    return return_code;
}

/*****************************************************************************/
//  Description : screen switch button callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ScreenButtonCallback(void)
{
    MMI_RESULT_E return_code = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"[MMIATV]:ScreenButtonCallback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_1662_112_2_18_2_3_16_86,(uint8*)"");
    
    
    SwitchScreenMode();
    
    return return_code;
}

/*****************************************************************************/
//  Description : screen switch button callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NextButtonCallback(void)
{
    MMI_RESULT_E return_code = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"[MMIATV]:NextButtonCallback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_1681_112_2_18_2_3_17_87,(uint8*)"");
    
    
    PlayNextService();
    
    return return_code;
}

/*****************************************************************************/
//  Description : screen switch button callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RecordButtonCallback(void)
{
    MMI_RESULT_E return_code = MMI_RESULT_FALSE;
    
    //SCI_TRACE_LOW:"[MMIATV]:RecordButtonCallback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_1700_112_2_18_2_3_17_88,(uint8*)"");
    if(MMIATV_IsPlaying())
    {
        //调用录制接口      
        if(MMIATV_FSMRecord(s_atv_current_para.atv_handle))
        {                   
            MMIATV_EnableLcdSwitch(FALSE);
            StartRecordTimer();
            //SCI_TRACE_LOW:"[MMIATV]: MMIATV_StartRecord success!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_MAIN_1023_112_2_18_2_3_11_73,(uint8*)"");
        }
        else 
        {           
             MMIATV_DisplayCommonTip(TXT_ATV_START_RECORD_ERROR);                     
        }   
        
        //MMIATV_SetPlayStatus(MMIATV_STATUS_RECORDING);
        return_code = MMI_RESULT_TRUE;
    }
    else if(MMIATV_IsRecording())
    {
        MMIATV_FSMStopRecord(s_atv_current_para.atv_handle,FALSE);
        StopRecordTimer();                  
        MMIATV_SaveRecordFileName(); //save record file name
		        
        //MMIATV_PlayService(s_atv_current_para.atv_handle, MMIATV_GetCurrentServiceID());
    }
    
    return return_code;
}

/*****************************************************************************/
//  Description : option button callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OptionButtonCallback(void)
{
    MMI_RESULT_E return_code = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"[MMIATV]:OptionButtonCallback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_1729_112_2_18_2_3_17_89,(uint8*)"");
    if (MMIATV_IsRecording())
    {
		//必须先停止录制
		MMIATV_DisplayCommonTip(TXT_ATV_STOP_RECORD_FISRT);
    } 
    else
    {
        MMK_CreateWin((uint32*)MMIATV_MAIN_OPT_WIN_TAB,PNULL);
	}
       
    
    return return_code;
}

/*****************************************************************************/
//  Description : option button callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ReturnButtonCallback(void)
{
    MMI_RESULT_E return_code = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"[MMIATV]:ReturnButtonCallback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_1748_112_2_18_2_3_17_90,(uint8*)"");
   
    MMK_PostMsg(MMIATV_MAIN_WIN_ID, MSG_APP_CANCEL, PNULL, NULL);
    
    return return_code;
}

/*****************************************************************************/
//  Description : 开启录像列表窗口
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void OpenRecordListWin(void)
{
    MMIFILE_DEVICE_E dev = MMIATV_GetRecordDevice();
    //BOOLEAN udisk_status = FALSE;
    //BOOLEAN sd_status = FALSE;
    const wchar*    file_dev_ptr = PNULL;
    uint16          file_dev_len = 0;
    wchar record_path[MMIATV_FULL_PATH_LEN] = {0};
    uint16 record_path_len = MMIATV_FULL_PATH_LEN;
    MMIFILE_DEVICE_E file_dev  = MMI_DEVICE_UDISK;
    
    file_dev = MMIAPIFMM_GetValidateDevice(dev);
    if(file_dev >= MMI_DEVICE_NUM)
    {
        file_dev = MMI_DEVICE_UDISK;
    }
    
    file_dev_ptr = MMIAPIFMM_GetDevicePath(file_dev);
    file_dev_len = MMIAPIFMM_GetDevicePathLen(file_dev);
        
    MMIAPIFMM_CombineFullPath(file_dev_ptr, 
                file_dev_len, 
                MMIMULTIM_DIR_MOVIE, 
                MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MOVIE),
                PNULL, 
                0, 
                record_path, &record_path_len);

    MMIAPIFMM_OpenExplorerExt(record_path,record_path_len, PNULL,0, FALSE, MMIFMM_FILE_TYPE(MMIFMM_MOVIE_ALL) | MMIFMM_MEDIA_TYPE(MMIFMM_MOVIE_MP4) | MMIFMM_MEDIA_TYPE(MMIFMM_MOVIE_AVI) | MMIFMM_MEDIA_TYPE(MMIFMM_MOVIE_3GP));
}

/*****************************************************************************/
//  Description : LoadMainOptMenu
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void LoadMainOptMenu(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMIATV_MAIN_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id = 0;
    MMIATV_SERVICE_NODE_T *node_ptr = PNULL;
    
    node_ptr = MMIATV_GetServiceNodeViaIndex(s_atv_current_para.current_index);
    if(PNULL == node_ptr)
    {
        GUIMENU_SetItemGrayed(ctrl_id, MMIATV_MAIN_OPT_MENU, MMIATV_ID_MENU_ADD_FAVORITE, TRUE);
    }
#ifdef MMI_GUI_STYLE_TYPICAL
#ifndef MMIATV_RECORD_3GP_SUPPORT
    if(MMIAPIFMM_GetFirstValidDevice() >= MMI_DEVICE_NUM)
    {
        //无法设置录制类型，同时无可用存储器，灰化设置菜单项
        GUIMENU_SetItemGrayed(ctrl_id, MMIATV_MAIN_OPT_MENU, MMIATV_ID_MENU_SETTING, TRUE);
    }
#endif
#else
    {
        GUIMENU_TITLE_INFO_T title_info = {0};
        MMI_STRING_T title_str = {0}; 
        GUIMENU_BUTTON_INFO_T   button_info = {0};
        GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};
        MMIFILE_DEVICE_E        file_dev = MMI_DEVICE_NUM;
        MMIFILE_DEVICE_E        valid_dev = MMI_DEVICE_NUM;
        MMIFILE_DEVICE_E        device = MMI_DEVICE_UDISK;
        uint32                  i = 0;

        //store status sub context menu title
        SCI_MEMSET(&title_str, 0, sizeof(title_str));
        MMI_GetLabelTextByLang(TXT_ATV_RECORD_DEVICE, &title_str);
        title_info.is_static = TRUE;
        title_info.title_ptr = &title_str;
        title_info.sub_title_ptr = PNULL;
        title_info.group_id = MMIATV_RECORD_STORE_STATUS;
        GUIMENU_SetMenuTitleEx(ctrl_id, &title_info);
        
        //store status sub context menu button
        SCI_MEMSET(&button_info, 0, sizeof(button_info));
        button_info.is_static   = TRUE;
        button_info.group_id = MMIATV_RECORD_STORE_STATUS;
        button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctrl_id, &button_info);
    
        group_id = MMIATV_MAIN_OPT_MENU;
        file_dev = MMIATV_GetRecordDevice();
        valid_dev = MMIAPIFMM_GetValidateDevice(file_dev);
        if(valid_dev < MMI_DEVICE_NUM)
        {
            group_id = MMIATV_RECORD_STORE_STATUS;
            i = 0;
            for(device = MMI_DEVICE_UDISK; device < MMI_DEVICE_NUM; device++)
            {
                uint32          used_space_low = 0;
                uint32          used_space_high = 0;
                uint32          total_space_high = 0;
                uint32          total_space_low = 0;
                uint32          free_space_low = 0;
                uint32          free_space_high = 0;

                MMIAPIFMM_GetDeviceFreeSpace((uint16 *)MMIAPIFMM_GetDevicePath(device) , 
                    MMIAPIFMM_GetDevicePathLen(device) , 
                    &free_space_high, 
                    &free_space_low);
                
                MMIAPIFMM_GetDeviceUsedSpace(MMIAPIFMM_GetDevicePath(device) , 
                    MMIAPIFMM_GetDevicePathLen(device) ,
                    &used_space_high, 
                    &used_space_low);
                total_space_low  =  used_space_low + free_space_low;  
                if ((total_space_low < used_space_low) || (total_space_low < free_space_low))
                {
                    total_space_high =  used_space_high + free_space_high +1;
                }
                else
                {
                    total_space_high =  used_space_high + free_space_high;
                }               
                menu_id = MMIATV_ID_MENU_RECORD_UDISK + device - MMI_DEVICE_UDISK;
                if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device))
                    && (total_space_high >0 || MMIATV_RECORD_MIN_SPACE_SIZE <= free_space_low))//bug146685
                {
                    GUIMENU_SetItemVisible(ctrl_id,group_id,menu_id,TRUE); 
                    i++;
                }
                else
                {
                    GUIMENU_SetItemVisible(ctrl_id,group_id,menu_id,FALSE);
                }
            }
            if (i <= 1)
            {
                menu_id = MMIATV_ID_MENU_RECORD_DEVICE;
                group_id = MMIATV_MAIN_OPT_MENU;
                GUIMENU_SetItemVisible(ctrl_id,group_id,menu_id,FALSE);

            }
            else
            {
                menu_id = MMIATV_ID_MENU_RECORD_DEVICE;
                group_id = MMIATV_MAIN_OPT_MENU;
                GUIMENU_SetItemVisible(ctrl_id,group_id,menu_id,TRUE);
                pop_item_info.is_static = TRUE;
                pop_item_info.ctrl_id = ctrl_id;
                pop_item_info.group_id = MMIATV_RECORD_STORE_STATUS;
            
                if(MMIAPIFMM_GetDeviceTypeStatus(file_dev))
                {
                    pop_item_info.menu_id = MMIATV_ID_MENU_RECORD_UDISK + file_dev - MMI_DEVICE_UDISK;
                }
                else
                {
                    pop_item_info.menu_id = MMIATV_ID_MENU_RECORD_UDISK + valid_dev - MMI_DEVICE_UDISK;
                }
                GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
            }
        }
        else
        {
            menu_id = MMIATV_ID_MENU_RECORD_DEVICE;
            //GUIMENU_SetItemVisible(ctrl_id,group_id,menu_id,FALSE);
            GUIMENU_SetItemGrayed(ctrl_id, group_id, menu_id, TRUE);
        }
#ifdef MMI_PDA_SUPPORT
#ifdef MMIATV_RECORD_3GP_SUPPORT
        SCI_MEMSET(&title_str, 0, sizeof(title_str));
        MMI_GetLabelTextByLang(TXT_ATV_RECORD_FILE_TYPE, &title_str);
        title_info.is_static = TRUE;
        title_info.title_ptr = &title_str;
        title_info.sub_title_ptr = PNULL;
        title_info.group_id = MMIATV_VIDEO_TYPE;
        GUIMENU_SetMenuTitleEx(ctrl_id, &title_info);
    
        //store status sub context menu button
        SCI_MEMSET(&button_info, 0, sizeof(button_info));
        button_info.is_static   = TRUE;
        button_info.group_id = MMIATV_VIDEO_TYPE;
        button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctrl_id, &button_info);

        //set menu text
        group_id = MMIATV_VIDEO_TYPE;            

        menu_id = MMIATV_ID_MENU_VIDEO_TYPE_3GP;
        SCI_MEMSET(&title_str, 0, sizeof(title_str));
        title_str.wstr_ptr = &MMIATV_VIDEO_TYPE_STRING_3GP;
        title_str.wstr_len = MMIATV_VIDEO_TYPE_STRING_3GP_LEN;
        GUIMENU_SetStaticItem(ctrl_id, group_id, menu_id, &title_str, IMAGE_NULL);

        menu_id = MMIATV_ID_MENU_VIDEO_TYPE_AVI;
        SCI_MEMSET(&title_str, 0, sizeof(title_str));
        title_str.wstr_ptr = &MMIATV_VIDEO_TYPE_STRING_AVI;
        title_str.wstr_len = MMIATV_VIDEO_TYPE_STRING_AVI_LEN;
        GUIMENU_SetStaticItem(ctrl_id, group_id, menu_id, &title_str, IMAGE_NULL);

        pop_item_info.is_static = TRUE;
        pop_item_info.ctrl_id = ctrl_id;
        pop_item_info.group_id = MMIATV_VIDEO_TYPE;            
        if(MMIATV_VIDEO_TYPE_AVI == MMIATV_GetVideoFileType())
        {
            pop_item_info.menu_id = MMIATV_ID_MENU_VIDEO_TYPE_AVI;
        }
        else
        {
            pop_item_info.menu_id = MMIATV_ID_MENU_VIDEO_TYPE_3GP;
        }
        GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);            
#endif //MMIATV_RECORD_3GP_SUPPORT
#endif //MMI_PDA_SUPPORT
    }
#endif //MMI_GUI_STYLE_TYPICAL
}
/*****************************************************************************/
//  Description : handle main opt menu win msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainOptWinMsg(
                                       MMI_WIN_ID_T win_id,
                                       MMI_MESSAGE_ID_E msg_id,
                                       DPARAM param
                                       )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIATV_MAIN_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id = 0;
    MMIATV_SERVICE_NODE_T *node_ptr = PNULL;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        LoadMainOptMenu();
        MMK_SetAtvCtrl(win_id, ctrl_id);        
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch(menu_id)
        {
        case MMIATV_ID_MENU_ADD_FAVORITE:
            node_ptr = MMIATV_GetServiceNodeViaIndex(s_atv_current_para.current_index);
            if(PNULL != node_ptr)
            {
                if(MMIATV_GetFavoriteServiceListNum() < MMIATV_FAVORITE_MAX_NUM)
                {
					if(!MMIATV_AddFavoriteServiceNode(node_ptr))
					{
						MMIATV_OpenCommonAlertWin(TXT_ATV_FAVORITE_SERVICE_HAS_EXIST, IMAGE_PUBWIN_FAIL);
						break;
					}
                    MMIATV_WriteFavoriteServiceListFile();
                    //refresh favorite list
                    MMK_PostMsg(MMIATV_FAVORITE_SERVICE_LIST_WIN_ID, MSG_APP_ATV_REFRESH_LIST, PNULL, NULL);
                    
                    #ifdef MMI_PDA_SUPPORT
                    MMIPUB_DisplayTipsIDEx(PNULL, TXT_ATV_ADD_FAVORITE_SERVICE_OK, MMIPUB_TIPS_DIS_BOTTOM, PNULL);
                    #else
                    MMIATV_OpenCommonAlertWin(TXT_ATV_ADD_FAVORITE_SERVICE_OK, IMAGE_PUBWIN_SUCCESS);
                    #endif
                }
                else
                {
                    #ifdef MMI_PDA_SUPPORT
                    MMIPUB_DisplayTipsIDEx(PNULL, TXT_ATV_FAVORITE_SERVICE_FULL, MMIPUB_TIPS_DIS_BOTTOM, PNULL);
                    #else
                    MMIATV_OpenCommonAlertWin(TXT_ATV_FAVORITE_SERVICE_FULL, IMAGE_PUBWIN_FAIL);
                    #endif
                }
            }
            break;
            
        case MMIATV_ID_MENU_FAVORITE_SERVICE_LIST:
            MMK_CreateWin((uint32*)MMIATV_FAVORITE_SERVICE_LIST_WIN_TAB,PNULL);
            break;
#ifdef MMI_ATV_RECORD_SUPPORT
        case MMIATV_ID_MENU_RECORD_LIST:
            OpenRecordListWin();
            break;
#endif
        case MMIATV_ID_MENU_SERVICE_LIST:
            MMK_CreateWin((uint32*)MMIATV_SERVICE_LIST_WIN_TAB,PNULL);
            break;
            
        case MMIATV_ID_MENU_SEARCH_SERVICE:
            s_atv_current_para.is_need_start_search = TRUE;
            break;
            
        case MMIATV_ID_MENU_SELECT_REGION:
            MMK_CreateWin((uint32*)MMIATV_SELECT_REGION_WIN_TAB,PNULL);
            break;
            
        #ifndef MMI_PDA_SUPPORT
        #ifndef MMI_ATV_MINI_SUPPORT  //6530_MINI
        case MMIATV_ID_MENU_SETTING:
            MMK_CreateWin((uint32*)MMIATV_SETTING_WIN_TAB,PNULL);  
            break;
        #endif  
        #endif //MMI_PDA_SUPPORT
            
        case MMIATV_ID_MENU_BRIGHTNESS:
            MMK_CloseWin(win_id);
            s_atv_current_para.is_bright_panel = TRUE;            
            break;
 #ifndef MMI_PDA_SUPPORT           
        case MMIATV_ID_MENU_HELP:
            MMK_CreateWin((uint32*)MMIATV_HELP_WIN_TAB,PNULL);
            break;
#endif
        #ifdef MMI_PDA_SUPPORT
        #ifdef MMIATV_RECORD_3GP_SUPPORT
        case MMIATV_ID_MENU_VIDEO_TYPE_3GP:
            MMIATV_SetVideoFileType((MMIATV_VIDEO_FILE_TYPE_E)MMIATV_VIDEO_TYPE_3GP);
            MMIATV_SaveSetting();  
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            MMK_CloseWin(win_id);
            break;
            
        case MMIATV_ID_MENU_VIDEO_TYPE_AVI:
            MMIATV_SetVideoFileType((MMIATV_VIDEO_FILE_TYPE_E)MMIATV_VIDEO_TYPE_AVI);
            MMIATV_SaveSetting();  
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            MMK_CloseWin(win_id);
            break;
        #endif //MMIATV_RECORD_3GP_SUPPORT
        #endif//MMI_PDA_SUPPORT
            
        default:
            #ifdef MMI_ATV_RECORD_SUPPORT
            if(menu_id >= MMIATV_ID_MENU_RECORD_UDISK && menu_id <= MMIATV_ID_MENU_RECORD_UDISK_MAX)
            {
                //set storage menu
                MMIATV_SetRecordDevice(MMI_DEVICE_UDISK + menu_id - MMIATV_ID_MENU_RECORD_UDISK);
                MMIATV_SaveSetting();    
                //MMIRECORD_SetRecordStorage(MMI_DEVICE_UDISK + menu_id - ID_RECORD_UDISK);
                //xin.li_clean_popwin
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                MMK_CloseWin(win_id);
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIATV]: HandleMainOptWinMsg:menu_id 0x%x is error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_2065_112_2_18_2_3_17_91,(uint8*)"d", menu_id);
            }
            #else
            //SCI_TRACE_LOW:"[MMIATV]: HandleMainOptWinMsg:menu_id 0x%x is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_2068_112_2_18_2_3_17_92,(uint8*)"d", menu_id);
            #endif //MMI_PDA_SUPPORT
            break;
        }
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : handle service list win msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleServiceListWinMsg(MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param
                                           )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIATV_SERVICE_LIST_CTRL_ID;
    uint32          index = 0;
    uint16          list_num = 0;
    MMIATV_SERVICE_NODE_T *cursor = PNULL;
    GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = PNULL;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        list_num = MMIATV_GetServiceListNum();
        GUILIST_SetMaxItem(ctrl_id, list_num, TRUE);
        
        //load service list
        if(0 == list_num)
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        }
        else
        {
            LoadServiceList(0, list_num -1, FALSE);
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
        
        if(PNULL != need_item_data_ptr)
        {
            index = need_item_data_ptr->item_index;
            LoadServiceList(index, index, TRUE);
        }
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        //play service
        index = GUILIST_GetCurItemIndex(ctrl_id);
        cursor = MMIATV_GetServiceNodeViaIndex(index);
        if(PNULL != cursor)
        {
            s_atv_current_para.current_index = index;
            MMIATV_SetCurrentServiceID(cursor->service_id);
            MMK_PostMsg(MMIATV_MAIN_WIN_ID,MSG_APP_ATV_PLAY_SERVICE,PNULL,PNULL);              
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        //add favorite
        #ifndef MMI_PDA_SUPPORT
        if(MMIATV_GetServiceListNum() > 0)
        {
            index = GUILIST_GetCurItemIndex(ctrl_id);
            cursor = MMIATV_GetServiceNodeViaIndex(index);
            if(PNULL != cursor)
            {
                if(MMIATV_GetFavoriteServiceListNum() < MMIATV_FAVORITE_MAX_NUM)
                {
					if(!MMIATV_AddFavoriteServiceNode(cursor))
					{
						MMIATV_OpenCommonAlertWin(TXT_ATV_FAVORITE_SERVICE_HAS_EXIST, IMAGE_PUBWIN_FAIL);
						break;
					}
                    MMIATV_WriteFavoriteServiceListFile();
                    
                    //refresh favorite list
                    MMK_PostMsg(MMIATV_FAVORITE_SERVICE_LIST_WIN_ID, MSG_APP_ATV_REFRESH_LIST, PNULL, NULL);
                    MMIATV_OpenCommonAlertWin(TXT_ATV_ADD_FAVORITE_SERVICE_OK, IMAGE_PUBWIN_SUCCESS);
                }
                else
                {
                    MMIATV_OpenCommonAlertWin(TXT_ATV_FAVORITE_SERVICE_FULL, IMAGE_PUBWIN_FAIL);
                }
            }
        }
        #endif //MMI_PDA_SUPPORT
        break;
    #ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        //长按List弹出context menu菜单
        MMK_CreateWin((uint32*)MMIATV_SERVICE_LIST_LONGOK_POPMENU_WIN_TAB,PNULL);
        break;
    #endif //MMI_PDA_SUPPORT
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : handle favorite service list win msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFavoriteServiceListWinMsg(
                                                   MMI_WIN_ID_T win_id,
                                                   MMI_MESSAGE_ID_E msg_id,
                                                   DPARAM param
                                                   )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIATV_FAVORITE_SERVICE_LIST_CTRL_ID;
    uint32          index = 0;
    uint16          list_num = 0;
    MMIATV_SERVICE_NODE_T *cursor = PNULL;
    GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = PNULL;
    MMI_WIN_ID_T        query_win_id                          = MMIATV_QUERY_WIN_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        list_num = MMIATV_GetFavoriteServiceListNum();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUILIST_SetMaxItem(ctrl_id, list_num, TRUE);
        
        //load favorite service list
        if(0 == list_num)
        {
            #ifdef MMI_PDA_SUPPORT
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            #else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            #endif //MMI_PDA_SUPPORT
        }
        else
        {
            LoadFavoriteServiceList(0, list_num -1, FALSE);
        }

        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
        
        if(PNULL != need_item_data_ptr)
        {
            index = need_item_data_ptr->item_index;
            LoadFavoriteServiceList(index, index, TRUE);
        }
        break;
        
    case MSG_APP_ATV_REFRESH_LIST:
        GUILIST_RemoveAllItems(ctrl_id);
        list_num = MMIATV_GetFavoriteServiceListNum();
        GUILIST_SetMaxItem(ctrl_id, list_num, TRUE);

        //refresh list
        if(0 == list_num)
        {
            #ifdef MMI_PDA_SUPPORT
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            #else
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            #endif //MMI_PDA_SUPPORT
        }
        else
        {
            LoadFavoriteServiceList(0, list_num -1, FALSE);
        }
        break;
        
    /*lint -e825*/
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
                //SCI_TRACE_LOW:"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_2277_112_2_18_2_3_18_93,(uint8*)"");
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //middle softkey处理
                //SCI_TRACE_LOW:"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_2282_112_2_18_2_3_18_94,(uint8*)"");
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE_ALL_QUERY, 
                    IMAGE_PUBWIN_QUERY,         
                    &query_win_id,
                    HandleDeleteAllQueryWinMsg);
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //right softkey处理
                //SCI_TRACE_LOW:"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_2291_112_2_18_2_3_18_95,(uint8*)"");
            }
            break;
        }
#endif //MMI_PDA_SUPPORT
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        //play service
        index = GUILIST_GetCurItemIndex(ctrl_id);
        cursor = MMIATV_GetFavoriteServiceNodeViaIndex(index);
        if(PNULL != cursor)
        {
            s_atv_current_para.current_index = MMIATV_UpdateCurrentPlayIndexByServiceId(cursor->service_id);
            MMIATV_SetCurrentServiceID(cursor->service_id);
            MMK_PostMsg(MMIATV_MAIN_WIN_ID,MSG_APP_ATV_PLAY_SERVICE,PNULL,PNULL);           
            MMK_CloseWin(win_id);
        }
        break; /*lint -e825*/
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifndef MMI_PDA_SUPPORT
        if(MMIATV_GetFavoriteServiceListNum() > 0)
        {
            MMK_CreateWin((uint32*)MMIATV_FAVORITE_SERVICE_LIST_OPT_WIN_TAB,PNULL);
        }
#endif //MMI_PDA_SUPPORT
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        //长按List弹出context menu菜单
        MMK_CreateWin((uint32*)MMIATV_FAVORITE_SERVICE_LIST_LONGOK_POPMENU_WIN_TAB,PNULL);
        break;
#endif //MMI_PDA_SUPPORT
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}
#ifndef MMI_PDA_SUPPORT//norflashcut_atv
/*****************************************************************************/
// 	Description : handle favorite service list opt menu win msg
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFavoriteServiceListOptWinMsg(
                                                      MMI_WIN_ID_T win_id,
                                                      MMI_MESSAGE_ID_E msg_id,
                                                      DPARAM param
                                                      )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIATV_FAVORITE_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id = 0;
    MMI_WIN_ID_T        query_win_id = MMIATV_QUERY_WIN_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch(menu_id)
        {
        case MMIATV_ID_MENU_EDIT:
            MMK_CreateWin((uint32*)MMIATV_FAVORITE_NAME_EDIT_WIN_TAB,PNULL);
            break;
            
        case MMIATV_ID_MENU_DELETE:
            MMIPUB_OpenQueryWinByTextId(TXT_COMMON_CONFIRM_DELETE, 
                IMAGE_PUBWIN_QUERY,         
                &query_win_id,
                HandleDeleteQueryWinMsg);
            break;
            
        case MMIATV_ID_MENU_DELETE_ALL:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE_ALL_QUERY, 
                IMAGE_PUBWIN_QUERY,         
                &query_win_id,
                HandleDeleteAllQueryWinMsg);
            break;
            
        default:
            //SCI_TRACE_LOW:"[MMIATV]: HandleFavoriteServiceListOptWinMsg:menu_id 0x%x is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_2388_112_2_18_2_3_18_96,(uint8*)"d", menu_id);
            break;
        }
        
        MMK_CloseWin(win_id);
        break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
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
//  Description : delete query win
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeleteQueryWinMsg(
                                           MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param
                                           )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    uint32              index = 0;
    MMIATV_SERVICE_NODE_T *cur_node_ptr = PNULL;
    
    switch(msg_id)
    {
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //delete current service
        index = GUILIST_GetCurItemIndex(MMIATV_FAVORITE_SERVICE_LIST_CTRL_ID);
        cur_node_ptr = MMIATV_GetFavoriteServiceNodeViaIndex(index);
        if(PNULL != cur_node_ptr)
        {
            MMIATV_DeleteFavoriteServiceNode(cur_node_ptr);
            MMIATV_WriteFavoriteServiceListFile();
           // xin.li_clean_popwin
           // MMIATV_OpenCommonAlertWin(TXT_DELETED, IMAGE_PUBWIN_SUCCESS);
        }
        
        //refresh favorite list
        MMK_PostMsg(MMIATV_FAVORITE_SERVICE_LIST_WIN_ID, MSG_APP_ATV_REFRESH_LIST, PNULL, NULL);
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : delete all query win
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeleteAllQueryWinMsg(
                                              MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM param
                                              )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //delete all service in favorite list
        MMIATV_ClearFavoriteServiceList();
        MMIATV_WriteFavoriteServiceListFile();
        //xin.li_clean_popwin
        //MMIATV_OpenCommonAlertWin(TXT_DELETED, IMAGE_PUBWIN_SUCCESS);
        
        //refresh favorite list
        MMK_PostMsg(MMIATV_FAVORITE_SERVICE_LIST_WIN_ID, MSG_APP_ATV_REFRESH_LIST, PNULL, NULL);
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : handle favorite service name edit win msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFavoriteServiceNameEidtWinMsg(MMI_WIN_ID_T win_id,
                                                       MMI_MESSAGE_ID_E msg_id,
                                                       DPARAM param
                                                       )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIATV_EDIT_FAVORITE_NAME_CTRL_ID;
    uint32          index = 0;
    MMI_STRING_T    text_str = {0};
    MMI_STRING_T    new_str = {0};
    MMIATV_SERVICE_NODE_T *cur_node_ptr = PNULL;
    GUIIM_TYPE_T    allow_im = GUIIM_TYPE_ABC | GUIIM_TYPE_ENGLISH | GUIIM_TYPE_DIGITAL | GUIIM_TYPE_SMART; /*lint !e655*/
    
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //get service name
        index = GUILIST_GetCurItemIndex(MMIATV_FAVORITE_SERVICE_LIST_CTRL_ID);
        cur_node_ptr = MMIATV_GetFavoriteServiceNodeViaIndex(index);
        if(PNULL != cur_node_ptr)
        {
            text_str.wstr_ptr = cur_node_ptr->service_name;
            text_str.wstr_len = MMIAPICOM_Wstrlen(text_str.wstr_ptr);
        }
        
        //set string
        GUIEDIT_SetMaxLen(ctrl_id, MMIATV_NAME_MAX_LEN);
        GUIEDIT_SetIm(ctrl_id, allow_im, GUIIM_TYPE_ABC);
        GUIEDIT_SetString(ctrl_id, text_str.wstr_ptr, text_str.wstr_len);
        MMK_SetAtvCtrl(win_id, ctrl_id);
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
                //left button
                MMK_PostMsg(win_id, MSG_APP_OK, PNULL, 0);
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //middle button
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //right button
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
            }
            break;
        }
#endif //MMI_PDA_SUPPORT
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        //get string info
        GUIEDIT_GetString(ctrl_id, &new_str);
        if(0 == new_str.wstr_len)
        {
            MMIATV_OpenCommonAlertWin(TXT_COMM_INPUT_NULL, IMAGE_PUBWIN_WARNING);
            break;
        }
        else if (MMIAPICOM_IsIncludeInvalidChar(new_str.wstr_ptr,  new_str.wstr_len ))
        {
            MMIATV_OpenCommonAlertWin(TXT_MMS_INVALID_INPUT, IMAGE_PUBWIN_WARNING);
            break;          
        }
        //save service name
        index = GUILIST_GetCurItemIndex(MMIATV_FAVORITE_SERVICE_LIST_CTRL_ID);
        cur_node_ptr = MMIATV_GetFavoriteServiceNodeViaIndex(index);
        if(PNULL != cur_node_ptr)
        {
            SCI_MEMSET(cur_node_ptr->service_name, 0, sizeof(wchar)*MMIATV_NAME_MAX_LEN);
            MMIAPICOM_Wstrcpy(cur_node_ptr->service_name, new_str.wstr_ptr);
            MMIATV_WriteFavoriteServiceListFile();
            
            //refresh favorite list
            MMK_PostMsg(MMIATV_FAVORITE_SERVICE_LIST_WIN_ID, MSG_APP_ATV_REFRESH_LIST, PNULL, NULL);
        }
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : handle select region win msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectRegionWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIATV_SELECT_REGION_LIST_CTRL_ID;
    uint32          index = 0;
    GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = PNULL;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, ATV_REGION_MAX, TRUE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        //load region list
        LoadRegionList(0, ATV_REGION_MAX -1, FALSE);
        GUILIST_SetCurItemIndex(ctrl_id, MMIATV_GetRegion());

        //for CR:NEWMS00126070, istyle风格下，menu为空，仍有显示问题，yaye.jiang
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
        
        if(PNULL != need_item_data_ptr)
        {
            index = need_item_data_ptr->item_index;
            LoadRegionList(index, index, TRUE);
        }
        break;
        
    /*lint -e825*/
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
                //SCI_TRACE_LOW:"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_2659_112_2_18_2_3_18_97,(uint8*)"");
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //middle softkey处理
                //SCI_TRACE_LOW:"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_2664_112_2_18_2_3_18_98,(uint8*)"");
                
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //right softkey处理
                //SCI_TRACE_LOW:"HandleTestWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_2670_112_2_18_2_3_18_99,(uint8*)"");
                MMK_CloseWin(win_id);
            }
            break;
        }
#endif //MMI_PDA_SUPPORT
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:    
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        //save region
        index = GUILIST_GetCurItemIndex(ctrl_id);
        MMIATV_SaveRegion((ATV_REGION_E)index);
        //xin.li_clean_popwin
        //MMIATV_OpenCommonAlertWin(TXT_SET_SUCCESS, IMAGE_PUBWIN_SUCCESS);
        MMK_CloseWin(win_id);
        break; /*lint -e825*/
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}
#ifndef MMI_PDA_SUPPORT //norflashcut_atv
#ifndef MMI_ATV_MINI_SUPPORT  //6530_MINI
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: aoke.hu
// Note:      
/*****************************************************************************/
LOCAL void AppendSettingListData(void)
{
    MMI_STRING_T label_text = {0};
    //BOOLEAN udisk_status = FALSE;
    //BOOLEAN sd_status = FALSE;
    MMI_STRING_T item_array[2] = {0};
    MMI_STRING_T storage_array[MMI_DEVICE_NUM] = {0};
    //MMI_STRING_T item_string = {0};
    uint32        i = 0;
    uint32        current_index = 0;
    MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_UDISK;
    BOOLEAN       dev_ok = FALSE;
    
#ifdef MMIATV_RECORD_3GP_SUPPORT
    GUIFORM_SetStyle(MMIATV_RECORD_TYPE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_ATV_RECORD_FILE_TYPE, &label_text);
    GUILABEL_SetText(MMIATV_LABEL_RECORD_TYPE_CTRL_ID, &label_text, FALSE);
    item_array[i].wstr_ptr = &MMIATV_VIDEO_TYPE_STRING_3GP;
    item_array[i].wstr_len = MMIATV_VIDEO_TYPE_STRING_3GP_LEN;
    i++;
    item_array[i].wstr_ptr = &MMIATV_VIDEO_TYPE_STRING_AVI;
    item_array[i].wstr_len = MMIATV_VIDEO_TYPE_STRING_AVI_LEN;
    i++;
    GUIDROPDOWNLIST_AppendItemArray(MMIATV_DROPDOWNLIST_RECORD_TYPE_CTRL_ID, item_array, i);
    current_index = (uint32)MMIATV_GetVideoFileType();
    GUIDROPDOWNLIST_SetCurItemIndex(MMIATV_DROPDOWNLIST_RECORD_TYPE_CTRL_ID, current_index);
#endif

    GUIFORM_SetStyle(MMIATV_RECORD_DEV_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    i = 0;
    current_index = 0;
    SCI_MEMSET(item_array,  0,sizeof(item_array));
    SCI_MEMSET(&label_text, 0,sizeof(label_text));
    //SCI_MEMSET(&item_string,0,sizeof(item_string));

    SCI_MEMSET(s_dev_ok_type, 0, sizeof(uint8)*MMI_DEVICE_NUM);
    do
    {
        if(MMIAPIFMM_GetDeviceTypeStatus(fmm_dev))
        {
            s_dev_ok_type[i] = fmm_dev;
            
            dev_ok = TRUE;
                
            //SCI_MEMSET(&item_string, 0, sizeof(MMI_STRING_T));
            
            MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(fmm_dev),&storage_array[i]); 
            storage_array[i].wstr_len = MIN(DROPDOWNLIST_STRING_MAX_NUM,storage_array[i].wstr_len);
            //MMIAPICOM_Wstrncpy(storage_array[i].wstr_ptr,item_string.wstr_ptr,storage_array[i].wstr_len);
            if(MMIATV_GetRecordDevice() == fmm_dev)
            {
                current_index = i;
            }
            i++;
        }
    }while(++fmm_dev < MMI_DEVICE_NUM);

    if(dev_ok)
    {
        MMI_GetLabelTextByLang(TXT_ATV_RECORD_DEVICE, &label_text);
        GUILABEL_SetText(MMIATV_LABEL_RECORD_DEV_CTRL_ID, &label_text, FALSE);
        
        GUIDROPDOWNLIST_AppendItemArray(MMIATV_DROPDOWNLIST_RECORD_DEV_CTRL_ID, storage_array, i);
        GUIDROPDOWNLIST_SetCurItemIndex(MMIATV_DROPDOWNLIST_RECORD_DEV_CTRL_ID, current_index);
    }
}

/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: aoke.hu
// Note:      
/*****************************************************************************/
LOCAL void SaveSettingValue(void)
{
    MMI_CTRL_ID_T    ctrl_id =MMIATV_DROPDOWNLIST_RECORD_TYPE_CTRL_ID;
    uint16           list_index = 0;
    uint16           total_num = 0;
    //MMIFILE_DEVICE_E  dev = MMIATV_GetRecordDevice();
    
#ifdef MMIATV_RECORD_3GP_SUPPORT
    list_index = GUIDROPDOWNLIST_GetCurItemIndex(ctrl_id);
    if(list_index < MMIATV_VIDEO_TYPE_MAX)
    {
        MMIATV_SetVideoFileType((MMIATV_VIDEO_FILE_TYPE_E)list_index);
    }
#endif
    ctrl_id =MMIATV_DROPDOWNLIST_RECORD_DEV_CTRL_ID;
    total_num = GUIDROPDOWNLIST_GetTotalItemNum(ctrl_id);
    if(total_num > 1)
    {
        list_index = GUIDROPDOWNLIST_GetCurItemIndex(ctrl_id);
        MMIATV_SetRecordDevice((s_dev_ok_type[list_index]));
    }
    MMIATV_SaveSetting();
}

/*****************************************************************************/
//  Description : 设置窗口处理函数
//  Global resource dependence : 
//  Author: aoke.hu
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleATVSettingWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E     recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T    ctrl_id =MMIATV_DROPDOWNLIST_RECORD_DEV_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendSettingListData();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_OK:     
    case MSG_APP_WEB:
    case MSG_APP_OK:
        SaveSettingValue();
        MMK_CloseWin(win_id);
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
#endif //end of MMI_ATV_MINI_SUPPORT  //6530_MINI
#endif


/*****************************************************************************/
//  Description : handle favorite service name edit win msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHelpWinMsg(MMI_WIN_ID_T win_id,
                                    MMI_MESSAGE_ID_E msg_id,
                                    DPARAM param
                                    )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIATV_HELP_TEXT_CTRL_ID;
    MMI_STRING_T    text={0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_ATV_RECORD_SUPPORT  //6530_MINI //暂时去掉record，待修改完成后打开
#ifndef   MMI_PDA_SUPPORT
        MMI_GetLabelTextByLang(TXT_ATV_HELP_DETAIL_ID, &text);
#else
        MMI_GetLabelTextByLang(TXT_ATV_HELP_DETAIL_NO_KEY, &text);
#endif
#else
		MMI_GetLabelTextByLang(TXT_ATV_HELP_DETAIL_NO_RECORD, &text);
#endif
        GUITEXT_SetString(ctrl_id, text.wstr_ptr, text.wstr_len, FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : append common list item
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void LoadServiceList(uint16 start_index,
                           uint16 end_index,
                           BOOLEAN is_load_data)
{
    GUILIST_ITEM_T      item_t = {0};       /*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};    /*lint !e64*/
    MMI_CTRL_ID_T       ctrl_id = MMIATV_SERVICE_LIST_CTRL_ID;
    MMIATV_SERVICE_NODE_T *cursor = PNULL;
    uint16 i = 0;
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    
    if(start_index > end_index)
    {
        return;
    }
    
    if(0 == MMIATV_GetServiceListNum())
    {
        return;
    }
    
    for(i = start_index; i <= end_index && i <= (MMIATV_GetServiceListNum() -1); i++)
    {
        if(is_load_data)
        {
            cursor = MMIATV_GetServiceNodeViaIndex(i);
            if(PNULL != cursor)
            {
                SCI_MEMSET(&item_data, 0, sizeof(item_data));
                item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;        
                item_data.item_content[0].item_data.text_buffer.wstr_ptr = cursor->service_name;
                item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(cursor->service_name);
                
                GUILIST_SetItemData(ctrl_id, &item_data, i);
            }
        }
        else
        {
            GUILIST_AppendItem(ctrl_id, &item_t);
        }
    }
}

/*****************************************************************************/
/*****************************************************************************/
LOCAL void LoadFavoriteServiceList(uint32 start_index,
                                   uint32 end_index,
                                   BOOLEAN is_load_data)
{
    GUILIST_ITEM_T      item_t = {0};       /*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};    /*lint !e64*/
    MMI_CTRL_ID_T       ctrl_id = MMIATV_FAVORITE_SERVICE_LIST_CTRL_ID;
    MMIATV_SERVICE_NODE_T *cursor = PNULL;
    uint16 i = 0;
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    
    if(start_index > end_index)
    {
        return;
    }
    
    if(0 == MMIATV_GetFavoriteServiceListNum())
    {
        return;
    }
    
    for(i = start_index; i <= end_index && i <= (MMIATV_GetFavoriteServiceListNum() -1); i++)
    {
        if(is_load_data)
        {
            cursor = MMIATV_GetFavoriteServiceNodeViaIndex(i);
            if(PNULL != cursor)
            {
                SCI_MEMSET(&item_data, 0, sizeof(item_data));
                item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;        
                item_data.item_content[0].item_data.text_buffer.wstr_ptr = cursor->service_name;
                item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(cursor->service_name);
                
                GUILIST_SetItemData(ctrl_id, &item_data, i);
            }
        }
        else
        {
            GUILIST_AppendItem(ctrl_id, &item_t);
        }
    }
}

/*****************************************************************************/
//  Description : load atv region list
//  Global resource dependence : none
//  Author: xin.li
//  Note: 加载国家、地区列表
/*****************************************************************************/
LOCAL void LoadRegionList(uint32  start_index,
                          uint32  end_index,
                          BOOLEAN is_load_data)
{
    MMI_STRING_T        text = {0};
    GUILIST_ITEM_T      item_t = {0};       /*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};    /*lint !e64*/
    MMI_CTRL_ID_T       ctrl_id = MMIATV_SELECT_REGION_LIST_CTRL_ID;
    uint16 i = 0;
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    
    if(start_index > end_index)
    {
        return;
    }
    
    if(end_index >= ATV_REGION_MAX)
    {
        return;
    }
    
    for(i = start_index; i <= end_index; i++)
    {
        if(is_load_data)
        {
            MMI_GetLabelTextByLang(s_region_name_table[i], &text);
            SCI_MEMSET(&item_data, 0, sizeof(item_data));
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;        
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = text.wstr_ptr;
            item_data.item_content[0].item_data.text_buffer.wstr_len = text.wstr_len;
            
            GUILIST_SetItemData(ctrl_id, &item_data, i);
        }
        else
        {
            GUILIST_AppendItem(ctrl_id, &item_t);
        }
    }
}

/*****************************************************************************/
//  Description : open alert win by text_id
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_OpenCommonAlertWin(MMI_TEXT_ID_T     text_id,
                                      MMI_IMAGE_ID_T    image_id)
{
#if ((defined MMI_ATV_MINI_SUPPORT) &&((defined MAINLCD_DEV_SIZE_128X160)||(defined MAINLCD_DEV_SIZE_176X220)))
	MMIPUB_SOFTKEY_STYLE_E atv_softkey_style = MMIPUB_SOFTKEY_NONE;
#else 
	MMIPUB_SOFTKEY_STYLE_E atv_softkey_style = MMIPUB_SOFTKEY_ONE;
#endif 
    MMIPUB_OpenAlertWinByTextId(PNULL,
        text_id,
        TXT_NULL,
        image_id,
        PNULL,
        PNULL,
        atv_softkey_style,//MMIPUB_SOFTKEY_ONE
        PNULL);   
}

/*****************************************************************************/
//  Description : clear rect with atv color key or common background img
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void ClearRect(GUI_RECT_T rect, BOOLEAN is_fill_img)
{
    GUI_LCD_DEV_INFO dev_info = {0};
    GUI_RECT_T       fill_rect = {0};
    MMI_IMAGE_ID_T   img_id = IMAGE_COMMON_BG;
    
    if(MMK_IsFocusWin(MMIATV_MAIN_WIN_ID))
    {
        if(is_fill_img)
        {
            //fill with common background img
            fill_rect = rect;
            GUIRES_DisplayImg(PNULL,
                &fill_rect,
                PNULL,
                MMIATV_MAIN_WIN_ID,
                img_id,
                &dev_info);
        }
        else
        {
            //fill with tv color key
            GUI_FillRect(&dev_info, rect, MMIATV_TRANSPARENT_COLOR);
        }
    }
}

/*****************************************************************************/
//  Description : clear full screen
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void ClearFullScreen(void)
{
    GUI_RECT_T rect = {0};
    
    rect = MMITHEME_GetFullScreenRect();
    ClearRect(rect, FALSE);
}


/*****************************************************************************/
//  Description : create panel timer
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void CreatePanelTimer(void)
{
    if (0 != s_atv_current_para.panel_timer_id)
    {
        MMK_StopTimer(s_atv_current_para.panel_timer_id);
        s_atv_current_para.panel_timer_id = 0;
    }
    s_atv_current_para.panel_timer_id = MMK_CreateWinTimer(MMIATV_MAIN_WIN_ID, MMIATV_2S_TIMEOUT*1000, FALSE);
}

/*****************************************************************************/
//  Description : stop panel timer
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void StopPanelTimer(void)
{
    if (0 != s_atv_current_para.panel_timer_id)
    {
        MMK_StopTimer(s_atv_current_para.panel_timer_id);
        s_atv_current_para.panel_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : create system timer
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void CreateSysTimer(uint32 type)
{
    //SCI_TRACE_LOW("[MMIATV]: CreateSysTimer timer_id=%d, type=%d,", s_atv_current_para.systimer_id, type);
    
    if(0 == s_atv_current_para.systimer_id)
    {        
        s_atv_current_para.systimer_id = MMK_CreateWinTimer(MMIATV_MAIN_WIN_ID, MMIATV_SYSTEM_TIMER_UNIT, TRUE);
        //SCI_TRACE_LOW("[MMIATV]: CreateSysTimer MMK_CreateWinTimer timer_id = %d", s_atv_current_para.systimer_id);
    }
    //SCI_TRACE_LOW("[MMIATV]: CreateSysTimer type = %d", type);
    
    s_atv_current_para.systimer_type = s_atv_current_para.systimer_type | type;
}

/*****************************************************************************/
//  Description : stop system timer
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void StopSysTimer(uint32 type)
{
    BOOLEAN ret = FALSE;
    
    //SCI_TRACE_LOW("[MMIATV]: StopSysTimer timer_id=%d, type=%d,", s_atv_current_para.systimer_id, type);
    
    type = ~type;
    s_atv_current_para.systimer_type = s_atv_current_para.systimer_type & type;
    
    if (0 < s_atv_current_para.systimer_id && 0 == s_atv_current_para.systimer_type)
    {
        ret = MMK_StopTimer(s_atv_current_para.systimer_id);
        //SCI_TRACE_LOW("[MMIATV]: StopSysTimer timer_id = %d,  MMK_StopTimer return=%d", s_atv_current_para.systimer_id, ret);
        s_atv_current_para.systimer_id = 0;
    }
}

/*****************************************************************************/
//  Description : Display normal string
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void DisplayString(
                         GUI_RECT_T rect,
                         const MMI_STRING_T *text_str,
                         GUI_FONT_T font,
                         BOOLEAN is_align_right,
                         BOOLEAN need_background    //背景色
                         )
{
    if(MMK_IsFocusWin(MMIATV_MAIN_WIN_ID) && PNULL != text_str)
    {
        GUI_LCD_DEV_INFO dev_info_ptr= {0};
        GUISTR_STYLE_T      text_style = GetCommonTextStyle();
        GUISTR_STATE_T      state = GetCommonTextState();

        //background color
        if(need_background)
        {
            state = state | GUISTR_STATE_BACKGROUND;
            text_style.bg_color = MMI_DARK_GRAY_COLOR;
        }

        //rect
        if(is_align_right)
        {
            rect.left = MAX(rect.left, rect.right - GUISTR_GetStringWidth(&text_style,text_str,state)); /*lint !e666*/
        }

        // display
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&dev_info_ptr,
            (const GUI_RECT_T      *)&rect,       //the fixed display area
            (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
            text_str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
    }
}

/*****************************************************************************/
//  Description : display common tip by string
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_DisplayCommonTipByStr(MMI_STRING_T *text_str_ptr)
{
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)
    GUI_RECT_T      rect_fullscreen = ATV_LAYOUT_COMMONTIP_RECT_H;
#else
    GUI_RECT_T      rect_fullscreen = {0,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT - MMIATV_COMMON_TIP_HEIGHT,
                                       MMIATV_FULLSCREEN_VIDEO_WIDTH -  ATV_OPT_WIDTH_H,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT -1};
#endif
#else  //6530
    GUI_RECT_T      rect_fullscreen = {0,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT-MMIATV_COMMON_TIP_HEIGHT,
                                       MMIATV_FULLSCREEN_VIDEO_WIDTH -1,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT -1};
#endif    //MMI_ATV_MINI_SUPPORT  //6530_MINI

    if(PNULL == text_str_ptr)
    {
        return;
    }
#ifndef UI_MULTILAYER_SUPPORT
	if(!MMIAPICOM_IsPanelOpened())
#endif
	{
		if(s_atv_current_para.is_display_panel)
		{
			//normal
			if(MMITHEME_IsMainScreenLandscape())
			{       
#ifndef MMI_ATV_MINI_SUPPORT  //6530_MINI
				GUI_BG_T bg = {0};
				bg.bg_type = GUI_BG_COLOR;  
				bg.color = MMIATV_PANEL_BG_COLOR;   
				GUIFORM_SetBg(MMIATV_PANEL_FORM_CTRL_ID, &bg);
#endif
			}
			GUILABEL_SetText(MMIATV_TIP_LABEL_CTRL_ID, text_str_ptr, FALSE);
			GUILABEL_SetFont(MMIATV_TIP_LABEL_CTRL_ID, MMI_DEFAULT_SMALL_FONT, MMI_GRAY_WHITE_COLOR);
			if (MMK_IsFocusWin(MMIATV_MAIN_WIN_ID))
			{       
				MMK_SendMsg(MMIATV_MAIN_WIN_ID, MSG_FULL_PAINT,PNULL);
			}
		//  GUILABEL_SetVisible(MMIATV_TIP_LABEL_CTRL_ID, TRUE, TRUE);  
		}
		else if (MMK_IsFocusWin(MMIATV_MAIN_WIN_ID))
		{
			//full screen       
			ClearRect(rect_fullscreen, FALSE);
			DisplayString(rect_fullscreen, text_str_ptr, MMIATV_NORMAL_FONT, FALSE, FALSE);
		}

		CreateSysTimer(MMIATV_SYSTIME_COMMON_TIP);
		s_atv_current_para.common_tip_count = 0;
	}

}

/*****************************************************************************/
//  Description : display common tip
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_DisplayCommonTip(MMI_TEXT_ID_T text_id)
{
    MMI_STRING_T    text_str = {0};
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)
    GUI_RECT_T      rect_fullscreen = ATV_LAYOUT_COMMONTIP_RECT_H;
#else
    GUI_RECT_T      rect_fullscreen = {0,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT-MMIATV_COMMON_TIP_HEIGHT-MMIATV_COMMON_TIP_HEIGHT_ADJUST,
                                       MMIATV_FULLSCREEN_VIDEO_WIDTH -  ATV_OPT_WIDTH_H,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT -1};
#endif
#else  //6530
    GUI_RECT_T      rect_fullscreen = {0,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT-MMIATV_COMMON_TIP_HEIGHT,
                                       MMIATV_FULLSCREEN_VIDEO_WIDTH -1,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT -1};
#endif   //MMI_ATV_MINI_SUPPORT  //6530_MINI
    s_atv_current_para.text_id = text_id;
    MMI_GetLabelTextByLang(text_id, &text_str);
#ifndef UI_MULTILAYER_SUPPORT
	if(!MMIAPICOM_IsPanelOpened())
#endif
	{ 
		if(s_atv_current_para.is_display_panel)
		{
			//normal
			if(MMITHEME_IsMainScreenLandscape())
			{
#ifndef MMI_ATV_MINI_SUPPORT
				GUI_BG_T bg = {0};
				bg.bg_type = GUI_BG_COLOR;  
				bg.color = MMIATV_PANEL_BG_COLOR;   
				GUIFORM_SetBg(MMIATV_PANEL_FORM_CTRL_ID, &bg);
#endif
			}
			GUILABEL_SetFont(MMIATV_TIP_LABEL_CTRL_ID, MMI_DEFAULT_SMALL_FONT, MMI_GRAY_WHITE_COLOR);
			GUILABEL_SetText(MMIATV_TIP_LABEL_CTRL_ID, &text_str, FALSE);
			if (MMK_IsFocusWin(MMIATV_MAIN_WIN_ID))
			{
				MMK_SendMsg(MMIATV_MAIN_WIN_ID, MSG_FULL_PAINT,PNULL);
			}
		//  GUILABEL_SetVisible(MMIATV_TIP_LABEL_CTRL_ID, TRUE, TRUE);
		}
		else if (MMK_IsFocusWin(MMIATV_MAIN_WIN_ID))
		{
			//full screen
			ClearRect(rect_fullscreen, FALSE);
			DisplayString(rect_fullscreen, &text_str, MMIATV_NORMAL_FONT, FALSE, FALSE);

		}
		CreateSysTimer(MMIATV_SYSTIME_COMMON_TIP);
		s_atv_current_para.common_tip_count = 0;
	}
}

/*****************************************************************************/
//  Description : display common tip all the time
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_DisplayCommonTipAlways(MMI_TEXT_ID_T text_id)
{
    MMI_STRING_T    text_str = {0};
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)
    GUI_RECT_T      rect_fullscreen = ATV_LAYOUT_COMMONTIP_RECT_H;
#else
    GUI_RECT_T      rect_fullscreen = {0,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT-MMIATV_COMMON_TIP_HEIGHT,
                                       MMIATV_FULLSCREEN_VIDEO_WIDTH -  ATV_OPT_WIDTH_H,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT -1};
#endif
#else  //6530
    GUI_RECT_T      rect_fullscreen = {0,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT-MMIATV_COMMON_TIP_HEIGHT,
                                       MMIATV_FULLSCREEN_VIDEO_WIDTH -1,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT -1};
#endif   //MMI_ATV_MINI_SUPPORT  //6530_MINI   
    SCI_TRACE_LOW("MMIATV_DisplayCommonTipAlways s_atv_current_para.is_display_panel = %d",s_atv_current_para.is_display_panel);
    s_atv_current_para.text_id = text_id;
    MMI_GetLabelTextByLang(text_id, &text_str);
#ifndef UI_MULTILAYER_SUPPORT
	if(!MMIAPICOM_IsPanelOpened())
#endif
	{
		if(s_atv_current_para.is_display_panel)
		{
			if(MMITHEME_IsMainScreenLandscape())
			{
#ifndef MMI_ATV_MINI_SUPPORT  //6530_MINI
				GUI_BG_T bg = {0};
				bg.bg_type = GUI_BG_COLOR;  
				bg.color = MMIATV_PANEL_BG_COLOR;   
				GUIFORM_SetBg(MMIATV_PANEL_FORM_CTRL_ID, &bg);
#endif
			}
			GUILABEL_SetFont(MMIATV_TIP_LABEL_CTRL_ID, MMI_DEFAULT_SMALL_FONT, MMI_GRAY_WHITE_COLOR);
			GUILABEL_SetText(MMIATV_TIP_LABEL_CTRL_ID, &text_str, FALSE);
			if (MMK_IsFocusWin(MMIATV_MAIN_WIN_ID))
			{
				MMK_SendMsg(MMIATV_MAIN_WIN_ID, MSG_FULL_PAINT,PNULL);
			}       
        
		}
		else if (MMK_IsFocusWin(MMIATV_MAIN_WIN_ID))
		{
			//full screen
			ClearRect(rect_fullscreen, FALSE);
			DisplayString(rect_fullscreen, &text_str, MMIATV_NORMAL_FONT, FALSE, FALSE);
		}
    } 
  
}

/*****************************************************************************/
//  Description : clear common tip rect
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_ClearCommonTip(void)
{
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)
    GUI_RECT_T      rect_fullscreen = ATV_LAYOUT_COMMONTIP_RECT_H;
#else
    GUI_RECT_T      rect_fullscreen = {0,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT-MMIATV_COMMON_TIP_HEIGHT-MMIATV_COMMON_TIP_HEIGHT_ADJUST,
                                       MMIATV_FULLSCREEN_VIDEO_WIDTH -  ATV_OPT_WIDTH_H,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT -1};
#endif
#else  //6530
    GUI_RECT_T      rect_fullscreen = {0,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT-MMIATV_COMMON_TIP_HEIGHT,
                                       MMIATV_FULLSCREEN_VIDEO_WIDTH -1,
                                       MMIATV_FULLSCREEN_VIDEO_HEIGHT -1};
#endif //MMI_ATV_MINI_SUPPORT  //6530_MINI
    MMI_STRING_T text_str = {0}; 
    s_atv_current_para.text_id = TXT_NULL;
#ifdef UI_MULTILAYER_SUPPORT
    if(s_atv_current_para.is_display_panel)
    {
        //normal
        if(MMITHEME_IsMainScreenLandscape())
        {
#ifndef MMI_ATV_MINI_SUPPORT  //6530_MINI
            GUI_BG_T bg = {0};
            bg.bg_type = GUI_BG_COLOR;  
            bg.color = MMI_BLACK_COLOR; 
            GUIFORM_SetBg(MMIATV_PANEL_FORM_CTRL_ID, &bg);
#endif
        }
        GUILABEL_SetText(MMIATV_TIP_LABEL_CTRL_ID, &text_str, FALSE);
        if (MMK_IsFocusWin(MMIATV_MAIN_WIN_ID))
        {       
            MMK_SendMsg(MMIATV_MAIN_WIN_ID, MSG_FULL_PAINT,PNULL);
        }

    }
    else if (MMK_IsFocusWin(MMIATV_MAIN_WIN_ID))
    {
        //full screen
		ClearRect(rect_fullscreen, FALSE);
    }
	StopSysTimer(MMIATV_SYSTIME_COMMON_TIP);
	s_atv_current_para.common_tip_count = 0;
#else
	if(MMIAPICOM_IsPanelOpened())
	{
		CreateSysTimer(MMIATV_SYSTIME_COMMON_TIP);
		s_atv_current_para.common_tip_count = 0;
	}
	else
	{
		if(MMITHEME_IsMainScreenLandscape())
		{       
			GUI_BG_T bg = {0};
			bg.bg_type = GUI_BG_COLOR;  
			bg.color = MMI_BLACK_COLOR; 
			GUIFORM_SetBg(MMIATV_PANEL_FORM_CTRL_ID, &bg);
		}
		GUILABEL_SetText(MMIATV_TIP_LABEL_CTRL_ID, &text_str, FALSE);		
		if (MMK_IsFocusWin(MMIATV_MAIN_WIN_ID))
		{
            MMK_SendMsg(MMIATV_MAIN_WIN_ID, MSG_FULL_PAINT,PNULL);			
			ClearRect(rect_fullscreen, FALSE);			
		}
	}
#endif
}

/*****************************************************************************/
//  Description : display record tip
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordTip(void)
{
    char str_buffer_arr[MMIATV_STRING_BUFFER_SIZE] = {0};
    static uint32 record_time = 0;
    uint16 str_len = 0;
    uint16  red_width=0;
    uint16  red_height = 0;
    wchar  time_wstr[MMIATV_STRING_BUFFER_SIZE] = {0};
    wchar max_time_text[] = {'8', '8', ':','8', '8', ':','8', '8',0};
    uint16 max_time_height = 0;
    uint16 max_time_width = 0;
    MMI_STRING_T max_time_str = {0};
    MMI_STRING_T time_str = {0};
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
    GUI_RECT_T  record_rect_mini = ATV_LAYOUT_NUM_RECT_H;
#endif
    GUI_RECT_T  record_rect = {0};
    int16  bmp_space = 2;
    GUI_POINT_T dis_point = {0};
    MMI_IMAGE_ID_T image_id = IMAGE_COMMON_RECORD_TIP;//IMG_DC_RECORD;
    GUI_LCD_DEV_INFO dev_info = {0};
    DRECORDER_RECORD_INFO_T record_info= {0};
    uint32      rect_x          = 0;
    uint32      rect_y          = 0;
    uint32      rect_w          = 0;
    uint32      rect_h          = 0;

    if(!MMK_IsFocusWin(MMIATV_MAIN_WIN_ID))
    {
        return;
    }

    //get bmp width and height
    GUIRES_GetImgWidthHeight(&red_width,&red_height,image_id,MMIATV_MAIN_WIN_ID);
    //max_time str
    max_time_str.wstr_ptr = max_time_text;
    max_time_str.wstr_len = MMIAPICOM_Wstrlen(max_time_text);
    max_time_height = GUI_GetStringHeight(MMIATV_NORMAL_FONT,max_time_str.wstr_ptr,max_time_str.wstr_len);
    max_time_width = (MMIATV_NORMAL_FONT + 2)*8;//GetStringWidth(&max_time_str);

    if (s_atv_current_para.is_display_panel)
    {
        GUIOWNDRAW_GetRect(MMIATV_VIDEO_OWNDRAW_CTRL_ID, &record_rect);
    }
    else
    {
        record_rect = MMITHEME_GetFullScreenRect();
    }
    
    if(!MMITHEME_IsMainScreenLandscape())
    {
        //vertical
        rect_x = MMIATV_VIDEO_LEFT;
        rect_y = MMIATV_VIDEO_TOP;
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            rect_y += (MMIATV_ISTYLE_TITLE_HEIGHT/2);
        }
#endif
        rect_w = MMIATV_VIDEO_WIDTH;
        rect_h = MMIATV_VIDEO_HEIGHT;
        MMIATV_GetAlign8Offset(&rect_w,&rect_h,&rect_x,&rect_y);
        record_rect.top = rect_y;
    }
    record_rect.left += 2;
    record_rect.right = record_rect.left + red_width + bmp_space +  max_time_width;
    record_rect.bottom = record_rect.top + MAX(red_height,max_time_height);
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
	record_rect.left = record_rect_mini.left;
	record_rect.right = record_rect_mini.right;
	record_rect.top = record_rect_mini.top;
	record_rect.bottom = record_rect_mini.bottom;
#endif
    ClearRect(record_rect, FALSE);
    
    //time str
    DRECORDER_GetRecordInfo(&record_info);
    //SCI_TRACE_LOW:"[MMIATV]: DisplayRecordTip media_time =%ld"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_3402_112_2_18_2_3_20_100,(uint8*)"d",record_info.media_time);
    record_time = record_info.media_time/1000;
    str_len = sprintf(str_buffer_arr, "%.2lu:%.2lu:%.2lu", record_time/60/60, record_time/60%60, record_time%60);
    MMIAPICOM_StrToWstr((uint8 *)str_buffer_arr, time_wstr);
    time_str.wstr_ptr = time_wstr;
    time_str.wstr_len = str_len;
    //display  RED  bitmap
    if((record_time % 2) == 0)
    {
        dis_point.x = record_rect.left;
        dis_point.y = record_rect.top + (record_rect.bottom - record_rect.top - red_height)/2;
        GUIRES_DisplayImg(&dis_point,PNULL,PNULL,MMIATV_MAIN_WIN_ID,image_id,&dev_info);
    }
    
    //display REC and times strings
    record_rect.left += red_width + bmp_space;
    DisplayString(record_rect,&time_str,MMIATV_NORMAL_FONT, FALSE,TRUE);
}

/*****************************************************************************/
//  Description : update control button state
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_UpdateButtonStatus(BOOLEAN is_refresh_all)
{
    MMI_CTRL_ID_T   vol_ctrl_id = MMIATV_BUTTON_VOL_CTRL_ID;
    MMI_CTRL_ID_T   pre_ctrl_id = MMIATV_BUTTON_PRE_CTRL_ID;
    MMI_CTRL_ID_T   screen_ctrl_id = MMIATV_BUTTON_SCREEN_CTRL_ID;
    MMI_CTRL_ID_T   next_ctrl_id = MMIATV_BUTTON_NEXT_CTRL_ID;
    MMI_CTRL_ID_T   record_ctrl_id = MMIATV_BUTTON_RECORD_CTRL_ID;
    MMI_CTRL_ID_T   option_ctrl_id = MMIATV_BUTTON_OPTION_CTRL_ID;
	uint32			total_num = MMIATV_GetServiceListNum();
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)
    GUI_BG_T gray_bg = {0};
#endif
#endif //MMI_ATV_MINI_SUPPORT  //6530_MINI
    GUI_BG_T    vol_para = {0};
    GUI_BG_T    pre_para = {0};
    GUI_BG_T    screen_para = {0};
    GUI_BG_T    next_para = {0};
    GUI_BG_T    record_para = {0};
    GUI_BG_T    option_para = {0};
#ifdef MMI_PDA_SUPPORT
    MMI_CTRL_ID_T   return_ctrl_id = MMIATV_BUTTON_RETURN_CTRL_ID;
    GUI_BG_T    return_para = {0};
    GUI_BG_T    button_bg_none = {0};


    button_bg_none.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(vol_ctrl_id,&button_bg_none);
    GUIBUTTON_SetPressedBg(vol_ctrl_id,&button_bg_none);
    GUIBUTTON_SetBg(pre_ctrl_id,&button_bg_none);
    GUIBUTTON_SetPressedBg(pre_ctrl_id,&button_bg_none);
    GUIBUTTON_SetBg(screen_ctrl_id,&button_bg_none);
    GUIBUTTON_SetPressedBg(screen_ctrl_id,&button_bg_none);
    GUIBUTTON_SetBg(next_ctrl_id,&button_bg_none);
    GUIBUTTON_SetPressedBg(next_ctrl_id,&button_bg_none);
    GUIBUTTON_SetBg(record_ctrl_id,&button_bg_none);
    GUIBUTTON_SetPressedBg(record_ctrl_id,&button_bg_none);
    GUIBUTTON_SetBg(option_ctrl_id,&button_bg_none);
    GUIBUTTON_SetPressedBg(option_ctrl_id,&button_bg_none);
    GUIBUTTON_SetBg(return_ctrl_id,&button_bg_none);
    GUIBUTTON_SetPressedBg(return_ctrl_id,&button_bg_none);
#endif
    //volume button
    vol_para.bg_type = GUI_BG_IMG;
    if(MMISET_VOL_ZERO == MMIAPISET_GetMultimVolume())
    {
        if(MMIATV_IsSearching())
        {
#if (!defined MMI_RES_LOW_COST) || (!defined MMI_ATV_MINI_SUPPORT)          
            vol_para.img_id = IMAGE_COMMON_PLAYER_BTN_VOL_MUTE_GREY;
            GUIBUTTON_SetGrayed(vol_ctrl_id, TRUE, &vol_para, TRUE);
#endif        
        }
        else
        {
#if (!defined MMI_RES_LOW_COST) || (!defined MMI_ATV_MINI_SUPPORT)         
            vol_para.img_id = IMAGE_COMMON_PLAYER_BTN_VOL_MUTE;
            GUIBUTTON_SetGrayed(vol_ctrl_id, FALSE, &vol_para, TRUE);
#endif
#ifndef MMI_PDA_SUPPORT
            GUIBUTTON_SetBg(vol_ctrl_id, &vol_para);
#else
            GUIBUTTON_SetFg(vol_ctrl_id,&vol_para); 
            vol_para.img_id = IMAGE_COMMON_PLAYER_BTN_VOL_MUTE_FOCUS;
            GUIBUTTON_SetPressedFg(vol_ctrl_id, &vol_para);
#endif
            GUIBUTTON_Update(vol_ctrl_id);
        }
    }
    else
    {
        if(MMIATV_IsSearching())
        {
#if (!defined MMI_RES_LOW_COST) || (!defined MMI_ATV_MINI_SUPPORT)        
            vol_para.img_id = IMAGE_COMMON_PLAYER_BTN_VOL_GREY;
            GUIBUTTON_SetGrayed(vol_ctrl_id, TRUE, &vol_para, TRUE);
#endif        
        }
        else
        {
#if (!defined MMI_RES_LOW_COST) || (!defined MMI_ATV_MINI_SUPPORT)         
            vol_para.img_id = IMAGE_COMMON_PLAYER_BTN_VOL;
#endif            
            GUIBUTTON_SetGrayed(vol_ctrl_id, FALSE, &vol_para, TRUE);
#ifndef MMI_PDA_SUPPORT
            GUIBUTTON_SetBg(vol_ctrl_id, &vol_para);
#else
            GUIBUTTON_SetFg(vol_ctrl_id,&vol_para); 
            vol_para.img_id = IMAGE_COMMON_PLAYER_BTN_VOL_FOCUS;
            GUIBUTTON_SetPressedFg(vol_ctrl_id, &vol_para);
#endif
            GUIBUTTON_Update(vol_ctrl_id);
        }
        
        //volume digit
        //DisplayPicture(MMIATV_BTN_VOLUME_DIGIT_LEFT, MMIATV_BTN_VOLUME_DIGIT_TOP, IMAGE_COMMON_PLAYER_VOL_DIGIT_1 + MMIATV_GetVolume() -1);
    }
    
    //pre and next button
    pre_para.bg_type = GUI_BG_IMG;
    next_para.bg_type = GUI_BG_IMG;
    if(0 == total_num
		|| 1== total_num
        || MMIATV_IsSearching())
    {
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)
        gray_bg.bg_type = GUI_BG_IMG;
        gray_bg.img_id = IMAGE_ATV_BTN_BG;
        GUIBUTTON_SetGrayed(pre_ctrl_id, TRUE, &gray_bg, TRUE);
        GUIBUTTON_SetGrayed(next_ctrl_id, TRUE, &gray_bg, TRUE);
        //pre
        pre_para.img_id = IMAGE_COMMON_BTN_PREV_GREY;
        GUIBUTTON_SetGrayedFg(pre_ctrl_id, TRUE, &pre_para, TRUE);
        //next
        next_para.img_id = IMAGE_COMMON_BTN_NEXT_GREY;
        GUIBUTTON_SetGrayedFg(next_ctrl_id, TRUE, &next_para, TRUE);
#else       
        //pre
        pre_para.img_id = IMAGE_COMMON_BTN_PREV_GREY;
        GUIBUTTON_SetGrayed(pre_ctrl_id, TRUE, &pre_para, TRUE);
        //next
        next_para.img_id = IMAGE_COMMON_BTN_NEXT_GREY;
        GUIBUTTON_SetGrayed(next_ctrl_id, TRUE, &next_para, TRUE);
#endif
#else
        //pre
        pre_para.img_id = IMAGE_COMMON_BTN_PREV_GREY;
        GUIBUTTON_SetGrayed(pre_ctrl_id, TRUE, &pre_para, TRUE);
        //next
        next_para.img_id = IMAGE_COMMON_BTN_NEXT_GREY;
        GUIBUTTON_SetGrayed(next_ctrl_id, TRUE, &next_para, TRUE);
#endif  //MMI_ATV_MINI_SUPPORT  //6530_MINI
    }
    else
    {
        //pre
        pre_para.img_id = IMAGE_COMMON_BTN_PREV;
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)
        GUIBUTTON_SetGrayed(pre_ctrl_id, FALSE, &pre_para, FALSE);
#else
		GUIBUTTON_SetGrayed(pre_ctrl_id, FALSE, &pre_para, TRUE);
#endif
#else
        GUIBUTTON_SetGrayed(pre_ctrl_id, FALSE, &pre_para, TRUE);
#endif
#ifdef MMI_PDA_SUPPORT
        GUIBUTTON_SetFg(pre_ctrl_id,&pre_para); 
        pre_para.img_id = IMAGE_COMMON_BTN_PREV_FOCUS;
        GUIBUTTON_SetPressedFg(pre_ctrl_id, &pre_para);
#endif
        //next
        next_para.img_id = IMAGE_COMMON_BTN_NEXT;
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)
        GUIBUTTON_SetGrayed(next_ctrl_id, FALSE, &next_para, FALSE);
#else
        GUIBUTTON_SetGrayed(next_ctrl_id, FALSE, &next_para, TRUE);
#endif
#else
        GUIBUTTON_SetGrayed(next_ctrl_id, FALSE, &next_para, TRUE);
#endif
#ifdef MMI_PDA_SUPPORT
        GUIBUTTON_SetFg(next_ctrl_id,&next_para); 
        next_para.img_id = IMAGE_COMMON_BTN_NEXT_FOCUS;
        GUIBUTTON_SetPressedFg(next_ctrl_id, &next_para);
#endif
    }
    
    //screen switch button
    screen_para.bg_type = GUI_BG_IMG;
    if(MMIATV_IsSearching())
    {
#if (!defined MMI_RES_LOW_COST) || (!defined MMI_ATV_MINI_SUPPORT)      
        screen_para.img_id = IMAGE_COMMON_BTN_FULLSCREEN_GRAY;
        GUIBUTTON_SetGrayed(screen_ctrl_id, TRUE, &screen_para, TRUE);
#endif  
    }
    else
    {
#if (!defined MMI_RES_LOW_COST) || (!defined MMI_ATV_MINI_SUPPORT)   

        if(MMK_IsWindowLandscape(MMIATV_MAIN_WIN_ID))
        {
            screen_para.img_id = IMAGE_COMMON_BTN_FULLSCREEN_OFF;            
        }
        else
        {
            screen_para.img_id = IMAGE_COMMON_BTN_FULLSCREEN;           
        }   
                GUIBUTTON_SetGrayed(screen_ctrl_id, FALSE, &screen_para, TRUE);
#endif
#ifdef MMI_PDA_SUPPORT
        GUIBUTTON_SetFg(screen_ctrl_id,&screen_para); 
        if(MMK_IsWindowLandscape(MMIATV_MAIN_WIN_ID))
        {         
            screen_para.img_id=IMAGE_COMMON_BTN_FULLSCREEN_OFF_FOCUS;           
        }
        else
        {            
            screen_para.img_id=IMAGE_COMMON_BTN_FULLSCREEN_FOCUS;           
        }   
        GUIBUTTON_SetPressedFg(screen_ctrl_id, &screen_para);
#endif
    }

    //snapshot button
    record_para.bg_type = GUI_BG_IMG;
#ifdef TOUCHPANEL_TYPE
    if(MMIATV_IsSearching())
    {
        record_para.img_id = IMAGE_COMMON_BTN_RECORD_GREY;
        GUIBUTTON_SetGrayed(record_ctrl_id, TRUE, &record_para, TRUE);
    }
    else
    {
        record_para.img_id = IMAGE_COMMON_BTN_RECORD;
        GUIBUTTON_SetGrayed(record_ctrl_id, FALSE, &record_para, TRUE);
#ifdef MMI_PDA_SUPPORT
        GUIBUTTON_SetFg(record_ctrl_id,&record_para); 
        record_para.img_id = IMAGE_COMMON_BTN_RECORD_FOCUS;
        GUIBUTTON_SetPressedFg(record_ctrl_id, &record_para);
#endif
    }
#endif

    //softkey
    if(MMITHEME_IsMainScreenLandscape())
    {
        //横屏下无softkey，增加option_button/return_button
        option_para.bg_type = GUI_BG_IMG;
        if(MMIATV_IsSearching())
        {         
        option_para.img_id = IMAGE_COMMON_BTN_OPTION_GREY;
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)
        gray_bg.bg_type = GUI_BG_IMG;
        gray_bg.img_id = IMAGE_ATV_BTN_BG;
        GUIBUTTON_SetGrayed(option_ctrl_id, FALSE, &gray_bg, FALSE);
        GUIBUTTON_SetGrayedFg(option_ctrl_id, TRUE, &option_para, TRUE);
#else
		GUIBUTTON_SetGrayedFg(option_ctrl_id, TRUE, PNULL, TRUE);
#endif 
#else
        GUIBUTTON_SetGrayed(option_ctrl_id, TRUE, &option_para, TRUE);
#endif
        }
        else
        {
            option_para.img_id = IMAGE_COMMON_BTN_OPTION;
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
#if (defined MAINLCD_DEV_SIZE_220X176) || (defined MAINLCD_DEV_SIZE_160X128)
            GUIBUTTON_SetGrayed(option_ctrl_id, FALSE, &option_para, FALSE);
#else
            GUIBUTTON_SetGrayed(option_ctrl_id, FALSE, &option_para, TRUE);
#endif
#else
            GUIBUTTON_SetGrayed(option_ctrl_id, FALSE, &option_para, TRUE);
#endif
#ifdef MMI_PDA_SUPPORT
            GUIBUTTON_SetFg(option_ctrl_id,&option_para); 
            option_para.img_id = IMAGE_COMMON_BTN_OPTION_FOCUS;
            GUIBUTTON_SetPressedFg(option_ctrl_id, &option_para);
#endif
        }

#ifdef MMI_PDA_SUPPORT
        return_para.bg_type = GUI_BG_IMG;
        return_para.img_id = IMAGE_COMMON_BTN_RETURN;
        GUIBUTTON_SetFg(return_ctrl_id,&return_para); 
        return_para.img_id = IMAGE_COMMON_BTN_RETURN_FOCUS;
        GUIBUTTON_SetPressedFg(return_ctrl_id, &return_para);
#endif
    }
    else
    {
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            //opt btn
            option_para.bg_type = GUI_BG_IMG;
            if(MMIATV_IsSearching())
            {
                option_para.img_id = IMAGE_COMMON_BTN_OPTION_GREY;
                GUIBUTTON_SetGrayed(option_ctrl_id, TRUE, &option_para, TRUE);
            }
            else
            {
                option_para.img_id = IMAGE_COMMON_BTN_OPTION;
                GUIBUTTON_SetGrayed(option_ctrl_id, FALSE, &option_para, TRUE);
                
                GUIBUTTON_SetFg(option_ctrl_id,&option_para); 
                option_para.img_id = IMAGE_COMMON_BTN_OPTION_FOCUS;
                GUIBUTTON_SetPressedFg(option_ctrl_id, &option_para);
            }
            
            //return btn
            return_para.bg_type = GUI_BG_IMG;
            return_para.img_id = IMAGE_COMMON_BTN_RETURN;
            GUIBUTTON_SetFg(return_ctrl_id,&return_para); 
            return_para.img_id = IMAGE_COMMON_BTN_RETURN_FOCUS;
            GUIBUTTON_SetPressedFg(return_ctrl_id, &return_para);
        }
#endif

        //竖屏下更新softkey
        if(MMIATV_IsSearching())
        {
            GUIWIN_SetSoftkeyTextId(MMIATV_MAIN_WIN_ID, TXT_NULL, TXT_NULL, STXT_STOP, TRUE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(MMIATV_MAIN_WIN_ID, STXT_OPTION, TXT_NULL, STXT_EXIT, TRUE);
        }
    }
    
    if(is_refresh_all)
    {
        //强制刷新
        MMK_SendMsg(MMIATV_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
}

/*****************************************************************************/
//  Description : display service search progress
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_DisplaySearchProgress(uint32 cur_num, uint32 total_num)
{
    if(MMIATV_IsSearching())
    {
        
        //通过自绘控件Callback来显示
        GUIOWNDRAW_Update(MMIATV_PROGRESS_OWNDRAW_CTRL_ID);
        MMIATV_DisplayCommonTipAlways(TXT_COMMON_SEARCHING);
    }
}

/*****************************************************************************/
//  Description : display search progress callback
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void DisplaySearchProgressCallback(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T       groove_rect = {0};
    GUI_RECT_T       progress_rect = {0};
    int16            progress_offset = 0;
    uint32           cur_num = MMIATV_GetSearchCurrentNum();
    uint32           total_num = MMIATV_GetSearchTotalNum();
    uint32           progress_width = 0;
 
    if(0 == total_num)
    {
        return;
    }
    if(!MMIATV_IsSearching())
    {
        return;
    }

    //get owndraw rect
    GUIOWNDRAW_GetDisplayRect(MMIATV_PROGRESS_OWNDRAW_CTRL_ID, &groove_rect);
#ifndef MMI_ATV_MINI_SUPPORT  //6530_MINI
    if (TXT_NULL == s_atv_current_para.text_id)
    {
        GUI_FillRect(&lcd_dev_info, groove_rect, MMI_BLACK_COLOR);  
    }
    else
    {
        GUI_FillRect(&lcd_dev_info, groove_rect, MMIATV_PANEL_BG_COLOR);    
    }
#endif //MMI_ATV_MINI_SUPPORT  //6530_MINI
    //ClearRect(groove_rect, TRUE);
    groove_rect.left = groove_rect.left + MMIATV_PROGRESS_LEFT_SPACE;
    groove_rect.right= groove_rect.right - MMIATV_PROGRESS_LEFT_SPACE;
    progress_width = groove_rect.right - groove_rect.left + 1;

    GUIRES_DisplayImg(PNULL,
        &groove_rect,
        PNULL,
        MMIATV_MAIN_WIN_ID,
        IMAGE_COMMON_PLAYER_PROCESS_GROOVE,
        &lcd_dev_info);
    
    //display progress
    progress_rect.left = groove_rect.left;
    progress_rect.top  = groove_rect.top;
    progress_rect.bottom = groove_rect.bottom;
    
    progress_offset = (cur_num * progress_width) / total_num;

    progress_rect.right = MIN(progress_rect.left + progress_offset, groove_rect.right);
    if(MMK_IsFocusWin(MMIATV_MAIN_WIN_ID))
    {
        GUIRES_DisplayImg(PNULL,
            &progress_rect,
            PNULL,
            MMIATV_MAIN_WIN_ID,
            IMAGE_COMMON_PLAYER_PROCESS,
            &lcd_dev_info);
    }
}

/*****************************************************************************/
//  Description : clear search progress
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_ClearSearchProgress(void)
{
    GUI_RECT_T  groove_rect = {0};
    
    GUIOWNDRAW_GetDisplayRect(MMIATV_PROGRESS_OWNDRAW_CTRL_ID, &groove_rect);
    //ClearRect(groove_rect, TRUE);
    if (TXT_NULL == s_atv_current_para.text_id)
    {
        GUI_FillRect(MMITHEME_GetDefaultLcdDev(), groove_rect, MMI_BLACK_COLOR);
    }
    else
    {
        GUI_FillRect(MMITHEME_GetDefaultLcdDev(), groove_rect, MMIATV_PANEL_BG_COLOR);
    }
// #endif//MMI_PDA_SUPPORT
}

/*****************************************************************************/
//  Description : display service number
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_DisplayServiceNum(void)
{
    SCI_TRACE_LOW("[ATV] MMIATV_DisplayServiceNum s_atv_current_para.is_display_panel = %d",s_atv_current_para.is_display_panel);
    if(s_atv_current_para.is_display_panel)
    {
        //通过自绘控件Callback来显示
        GUIOWNDRAW_Update(MMIATV_VIDEO_OWNDRAW_CTRL_ID);
    }
    else
    {
        //direct display
        DisplayServiceNumCallback(PNULL);
    }
}

/*****************************************************************************/
//  Description : display service number
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void DisplayServiceNumCallback(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    uint32       total_num = 0;
    MMI_STRING_T text_str = {0};
    char         str_info[MMIATV_NAME_MAX_LEN] = {0};
    wchar        wstr_info[MMIATV_NAME_MAX_LEN] = {0};
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
    uint8          signal_level = 0;
    MMI_IMAGE_ID_T image_id     = IMAGE_NULL;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T  rect_signal = ATV_LAYOUT_SIGNAL_RECT_H;
    GUI_RECT_T  rect_fullscreen = ATV_LAYOUT_NUM_RECT_H;
#else  //6530
    GUI_RECT_T  rect_fullscreen = {MMIATV_FULLSCREEN_VIDEO_WIDTH/2,
                                   0,
                                   MMIATV_FULLSCREEN_VIDEO_WIDTH -1,
                                   MMIATV_COMMON_TIP_HEIGHT};
#endif  //MMI_ATV_MINI_SUPPORT  //6530_MINI
    uint32   video_rect_x    = 0;
    uint32   video_rect_y    = 0;
    uint32   video_rect_w    = 0;
    uint32   video_rect_h    = 0;
    GUI_RECT_T  real_rect = {0};
    GUI_RECT_T  real_video_rect = {0};
    
    SCI_TRACE_LOW("[ATV] DisplayServiceNumCallback");

    if(!MMK_IsFocusWin(MMIATV_MAIN_WIN_ID))
    {
        return;
    }
    
    //calc number
    total_num = MMIATV_GetServiceListNum();
    if(total_num > 0)
    {
        sprintf(str_info, "CH%d%s%d", (s_atv_current_para.current_index+1), "/", total_num);
    }
    else
    {
        sprintf(str_info, "CH%d", 0);
    }
    MMIAPICOM_StrToWstr((const uint8*)str_info, wstr_info);
    text_str.wstr_ptr = wstr_info;
    text_str.wstr_len = MMIAPICOM_Wstrlen(wstr_info);
    
    if(s_atv_current_para.is_display_panel)
    {
        //normal
        if(MMITHEME_IsMainScreenLandscape())
        {
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI 
            video_rect_x = rect_fullscreen.left;
            video_rect_y = rect_fullscreen.right;
            video_rect_w = rect_fullscreen.right - rect_fullscreen.left + 1;
            video_rect_h = ATV_LAYOUT_NUM_HEIGHT_H; 
#else  //6530
            //horizontal
            video_rect_x = rect_fullscreen.left;
            video_rect_y = MMIATV_VIDEO_TOP_H;
            video_rect_w = rect_fullscreen.right - rect_fullscreen.left + 1;
            video_rect_h = MMIATV_VIDEO_HEIGHT_H;           
#endif  //MMI_ATV_MINI_SUPPORT  //6530_MINI         
        }
        else
        {
            //vertical
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
            if(MMITHEME_IsIstyle())
            {
                video_rect_y = MMIATV_VIDEO_TOP + (MMIATV_ISTYLE_TITLE_HEIGHT/2);
            }
            else
#endif
            {
                video_rect_y = MMIATV_VIDEO_TOP;
            }
            
            video_rect_x = MMIATV_VIDEO_LEFT;
            video_rect_w = MMIATV_VIDEO_WIDTH;
            video_rect_h = MMIATV_VIDEO_HEIGHT;
        }
        MMIATV_GetAlign8Offset(&video_rect_w,&video_rect_h,&video_rect_x,&video_rect_y);
        real_video_rect.left = video_rect_x;
        real_video_rect.top = video_rect_y;
        real_video_rect.right = video_rect_x + video_rect_w - 1;
        real_video_rect.bottom = video_rect_y + video_rect_h - 1;
        
        real_rect = real_video_rect;
        real_rect.left = real_rect.left + ((int16)video_rect_w / 2);   //右半区域显示频道号，左半区域显示录像TIP
        real_rect.bottom = real_rect.top + MMIATV_COMMON_TIP_HEIGHT;
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
	real_video_rect = rect_fullscreen;
#endif
        //区域清为透明色
        ClearRect(real_video_rect, FALSE);
    }
    else
    {
        //full screen
        real_rect = rect_fullscreen;
    }
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
	real_rect = rect_fullscreen;
#ifndef UI_MULTILAYER_SUPPORT
    ClearRect(rect_signal, FALSE);
#endif
    MMIAPIATV_GetTVSignalLevel(&signal_level);
    image_id = MMIAPIATV_GetTVStatusImageID(signal_level);
    GUIRES_DisplayImg(PNULL,
        &rect_signal,
        PNULL,
        MMIATV_MAIN_WIN_ID,
        image_id,
        &lcd_dev_info);
#endif //MMI_ATV_MINI_SUPPORT  //6530_MINI
    //display
    if(MMIATV_IsPlaying()
        || MMIATV_IsSettingCHN())
    {
        ClearRect(real_rect, FALSE);
        DisplayString(real_rect, &text_str, MMIATV_NORMAL_FONT, TRUE, TRUE);
        
        CreateSysTimer(MMIATV_SYSTIME_SERVICE_NUMBER);
        s_atv_current_para.service_num_count = 0;
    }
}
 
/*****************************************************************************/
//  Description : clear service number
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_ClearServiceNum(BOOLEAN is_focus)
{
    uint32      rect_x          = 0;
    uint32      rect_y          = 0;
    uint32      rect_w          = 0;
    uint32      rect_h          = 0;
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
    GUI_RECT_T  rect_fullscreen = ATV_LAYOUT_NUM_RECT_H;
    GUI_RECT_T  rect_signal = ATV_LAYOUT_SIGNAL_RECT_H;
#else  //6530
    GUI_RECT_T  rect_fullscreen = {MMIATV_FULLSCREEN_VIDEO_WIDTH/2,
                                   0,
                                   MMIATV_FULLSCREEN_VIDEO_WIDTH -1,
                                   MMIATV_COMMON_TIP_HEIGHT};
#endif //MMI_ATV_MINI_SUPPORT  //6530_MINI
    GUI_RECT_T  real_rect = {0};
    GUI_LCD_DEV_INFO dev_info = {0};
    
    if(s_atv_current_para.is_display_panel)
    {
        //normal
        if(MMITHEME_IsMainScreenLandscape())
        {
            //horizontal
            rect_x = rect_fullscreen.left;
            rect_y = MMIATV_VIDEO_TOP_H;
            rect_w = rect_fullscreen.right - rect_fullscreen.left + 1;
            rect_h = MMIATV_VIDEO_HEIGHT_H;           
        }
        else
        {
            //vertical
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
            if(MMITHEME_IsIstyle())
            {
                rect_y = MMIATV_VIDEO_TOP + (MMIATV_ISTYLE_TITLE_HEIGHT/2);
            }
            else
#endif
            {
                rect_y = MMIATV_VIDEO_TOP;
            }
            
            rect_x = MMIATV_VIDEO_LEFT;
            
            rect_w = MMIATV_VIDEO_WIDTH;
            rect_h = MMIATV_VIDEO_HEIGHT;
        }
        MMIATV_GetAlign8Offset(&rect_w,&rect_h,&rect_x,&rect_y);
        real_rect.left = rect_x;
        real_rect.top = rect_y;
        real_rect.right = rect_x + rect_w - 1;
       
        real_rect.left = real_rect.left + ((int16)rect_w / 2); //右半区域显示频道号，左半区域显示录像TIP
        real_rect.bottom = real_rect.top + MMIATV_COMMON_TIP_HEIGHT;     
    }
    else
    {
        //full screen
        real_rect = rect_fullscreen;
    }
    
    if(is_focus)
    {
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
        real_rect = rect_fullscreen;
        ClearRect(real_rect, FALSE);
        ClearRect(rect_signal, FALSE);
#else
        ClearRect(real_rect, FALSE);
#endif  //MMI_ATV_MINI_SUPPORT  //6530_MINI
    }
    else
    {
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
		real_rect = rect_fullscreen;
        GUI_FillRect(&dev_info, rect_signal, MMIATV_TRANSPARENT_COLOR);
        GUI_FillRect(&dev_info, real_rect, MMIATV_TRANSPARENT_COLOR);
#else
        GUI_FillRect(&dev_info, real_rect, MMIATV_TRANSPARENT_COLOR);
#endif
    }
    
    StopSysTimer(MMIATV_SYSTIME_SERVICE_NUMBER);
    s_atv_current_para.service_num_count = 0;
}

/*****************************************************************************/
//  Description : enter atv, init data, setting, ref etc.
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InitATV(void)
{
    uint32           service_id = 0;
    MMICOM_PANEL_CALLBACK_T para ={0};
   
    
    //SCI_TRACE_LOW:"[MMIATV]: InitATV"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_3964_112_2_18_2_3_21_101,(uint8*)"");
    
    //init brightness
//     para.cur_value = MMIATV_GetBrightness();
//     SetBrightnessCallback(&para);   
    
    //init status    
    s_atv_current_para.current_index = 0;
    s_atv_current_para.is_display_panel = TRUE;   
    
    //init ref
    if(!MMIATV_InitATV(s_atv_current_para.atv_handle,&service_id))
    {
        MMIATV_OpenCommonAlertWin(TXT_ATV_OPEN_FAIL, IMAGE_PUBWIN_FAIL);
        return FALSE;
    }  
    else if(MMIATV_IsSearching())
    {
        //update control button and soft key
        MMIATV_DisplayCommonTipAlways(TXT_COMMON_SEARCHING);
        MMIATV_UpdateButtonStatus(TRUE);
    }
    else
    {
        s_atv_current_para.current_index = MMIATV_UpdateCurrentPlayIndexByServiceId(service_id);
        MMIATV_SetCurrentServiceID(service_id);
        //play
        MMK_PostMsg(MMIATV_MAIN_WIN_ID,MSG_APP_ATV_PLAY_SERVICE,PNULL,PNULL);      
    }
	//init brightness
	para.cur_value = MMIATV_GetBrightness();
    SetBrightnessCallback(&para);  
   
    return TRUE;
}

/*****************************************************************************/
//  Description : exit atv
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void ReleaseATV(void)
{
    MMIATV_ReleaseATV(s_atv_current_para.atv_handle);
    ClearVideoRect(FALSE);  
    MMIATV_EnableLcdSwitch(TRUE);   

    //reset brightness
    MMIAPISET_ResetContrast(MAIN_LCD_ID);    
    //release data
    StopSysTimer(MMIATV_SYSTIME_ALL);
    StopRecordTimer();

    CloseRelativeWin();    

}

/*****************************************************************************/
//  Description : close atv relative window when exit
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void CloseRelativeWin(void)
{
    if(MMK_IsOpenWin(MMIATV_SERVICE_LIST_WIN_ID))
    {
        MMK_CloseWin(MMIATV_SERVICE_LIST_WIN_ID);
    }
    
    if(MMK_IsOpenWin(MMIATV_FAVORITE_SERVICE_LIST_WIN_ID))
    {
        MMK_CloseWin(MMIATV_FAVORITE_SERVICE_LIST_WIN_ID);
    }
    
    if(MMK_IsOpenWin(MMIATV_QUERY_WIN_ID))
    {
        MMK_CloseWin(MMIATV_QUERY_WIN_ID);
    }
    
    if(MMK_IsOpenWin(MMIATV_FAVORITE_NAME_EDIT_WIN_ID))
    {
        MMK_CloseWin(MMIATV_FAVORITE_NAME_EDIT_WIN_ID);
    }
    
    if(MMK_IsOpenWin(MMIATV_SELECT_REGION_WIN_ID))
    {
        MMK_CloseWin(MMIATV_SELECT_REGION_WIN_ID);
    }
    
    if(MMK_IsOpenWin(MMIATV_HELP_WIN_ID))
    {
        MMK_CloseWin(MMIATV_HELP_WIN_ID);
    }

    if(MMK_IsOpenWin(MMIATV_SETTING_WIN_ID))
    {
        MMK_CloseWin(MMIATV_SETTING_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : play previous service
//  Global resource dependence : none
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void PlayPreService(void)
{    
    if(!MMK_IsFocusWin(MMIATV_MAIN_WIN_ID))
    {
        return;
    }  
    if(MMIATV_IsSearching() || MMIATV_IsSettingCHN())
    {
        return;
    }
    else if (MMIATV_IsRecording())
    {
        MMIATV_DisplayCommonTip(TXT_ATV_STOP_RECORD_FISRT);//提示停录制
        return;
    }
 
    
    if(MMIATV_PlayPreService(s_atv_current_para.atv_handle,&(s_atv_current_para.current_index)))
    {
        MMIATV_DisplayServiceNum();
    }
    else
    {
        MMIATV_DisplayCommonTip(TXT_COMM_PLAY_ERROR);       
    }  
    
}

/*****************************************************************************/
//  Description : play next service
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void PlayNextService(void)
{      
    if(!MMK_IsFocusWin(MMIATV_MAIN_WIN_ID))
    {
        return;
    }  

    if(MMIATV_IsSearching() || MMIATV_IsSettingCHN())
    {
        return;
    }
    else if (MMIATV_IsRecording())
    {
        MMIATV_DisplayCommonTip(TXT_ATV_STOP_RECORD_FISRT);//提示停录制
        return;
    }

    
    if(MMIATV_PlayNextService(s_atv_current_para.atv_handle,&(s_atv_current_para.current_index)))
    {
        MMIATV_DisplayServiceNum();
    }
    else
    {
        MMIATV_DisplayCommonTip(TXT_COMM_PLAY_ERROR);       
    }
}

/*****************************************************************************/
//  Description : Handle get focusinterrupt
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void HandleGetFocus(void)
{
    ClearFullScreen();      
    if (s_atv_current_para.is_display_panel)
    {
        CreatePanelTimer();
    }
    
    if(!MMIATV_FSMOpen(s_atv_current_para.atv_handle))
    {
        MMIATV_OpenCommonAlertWin(TXT_ATV_OPEN_FAIL, IMAGE_PUBWIN_FAIL);
        MMK_CloseWin(MMIATV_MAIN_WIN_ID);
        return;
    }
      
     
    //resume play or search
    if(s_atv_current_para.is_need_start_search)
    {
        s_atv_current_para.is_need_start_search = FALSE;
        //reset current play index
        s_atv_current_para.current_index = 0;
        
        //start search
        MMIATV_StartSearchService(s_atv_current_para.atv_handle);
        if(MMIATV_IsSearching())
        {
            if(!s_atv_current_para.is_display_panel)
            {
    #if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
                if(MMITHEME_IsIstyle())
                {
                    MMK_CreateCtrlByWinTabEx(MMIATV_MAIN_WIN_ID, MMITHEME_IsMainScreenLandscape() ? (uint32 *)MMIATV_MAIN_WIN_TAB_RORM_H : (uint32 *)MMIATV_ISTYLE_MAIN_WIN_TAB_RORM_V);
                }
                else
    #endif
                {
                    MMK_CreateCtrlByWinTabEx(MMIATV_MAIN_WIN_ID, MMITHEME_IsMainScreenLandscape() ? (uint32 *)MMIATV_MAIN_WIN_TAB_RORM_H : (uint32 *)MMIATV_MAIN_WIN_TAB_RORM_V);
                }
    #ifdef PDA_UI_DROPDOWN_WIN
                GUIWIN_SetStbState(MMIATV_MAIN_WIN_ID, GUISTATUSBAR_STATE_INVALID, TRUE); 
    #endif
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
                SetAtvLayoutParam();
#else
                SetFormParam();
#endif //end of MMI_ATV_MINI_SUPPORT  //6530_MINI
                s_atv_current_para.is_display_panel = TRUE;
                CreatePanelTimer();
            }
            //update control button and soft key            
            MMIATV_DisplayCommonTipAlways(TXT_COMMON_SEARCHING);
            MMIATV_UpdateButtonStatus(TRUE);               
        }
    }
    else
    {  
        MMK_PostMsg(MMIATV_MAIN_WIN_ID,MSG_APP_ATV_PLAY_SERVICE,PNULL,PNULL); 
        
    }
      
}
/*****************************************************************************/
//  Description : Handle lose focus interrupt
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void HandleLoseFocus(void)
{  
    StopPanelTimer();
    //stop play or search
    MMIATV_ExitATV(s_atv_current_para.atv_handle);   
    StopRecordTimer();
    ClearVideoRect(FALSE);  
    //MMIATV_EnableLcdSwitch(TRUE); 
    //close dc
    
}

/*****************************************************************************/
//  Description : handle lcd switch msg
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void HandleLcdSwitchMsg(void)
{
    MMI_HANDLE_T   win_id = MMIATV_MAIN_WIN_ID;    
    
    StopPanelTimer();
    if(MMIAPICOM_IsPanelOpened())
    {
        MMIAPICOM_ClosePanelChildWin();
    }
    //stop play
    MMIATV_FSMStop(s_atv_current_para.atv_handle);
    GUIBLOCK_ResetMainType();   

#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {
            MMK_CreateCtrlByWinTabEx(win_id, MMITHEME_IsMainScreenLandscape() ? (uint32 *)MMIATV_MAIN_WIN_TAB_RORM_H : (uint32 *)MMIATV_ISTYLE_MAIN_WIN_TAB_RORM_V);
        }
        else
#endif
        {
            MMK_CreateCtrlByWinTabEx(win_id, MMITHEME_IsMainScreenLandscape() ? (uint32 *)MMIATV_MAIN_WIN_TAB_RORM_H : (uint32 *)MMIATV_MAIN_WIN_TAB_RORM_V);
        }
#ifdef PDA_UI_DROPDOWN_WIN
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_INVALID, TRUE); 
#endif
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
        SetAtvLayoutParam();
#else
        SetFormParam();
#endif

        
        s_atv_current_para.is_display_panel = TRUE;
        
        UpdateAlwaysTip();
        MMIATV_UpdateButtonStatus(TRUE);
        
        //转到横屏默认全屏幕播放
        if(MMITHEME_IsMainScreenLandscape())
        {
//            MMIATV_SetScreenMode(MMIATV_SCREEN_FULLSCREEN);   
            CreatePanelTimer();
        }
//        else
//        {           
//            MMIATV_SetScreenMode(MMIATV_SCREEN_NORMAL);              
//        }
                
        //replay service
		if (0 != s_atv_current_para.atv_handle)
		{
			MMK_PostMsg(MMIATV_MAIN_WIN_ID,MSG_APP_ATV_PLAY_SERVICE,PNULL,PNULL);
		}
        
    
        //MMIATV_PlayService(MMIATV_GetCurrentServiceID());
    
}

/*****************************************************************************/
//  Description : 设置播放器角度,禁止旋转
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetAtvWinAngle(LCD_ANGLE_E angle)
{
    MMI_WIN_ID_T win_id = MMIATV_MAIN_WIN_ID;
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
//  Description : half/full screen switch
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void SwitchScreenMode(void)
{
    MMI_HANDLE_T   win_id = MMIATV_MAIN_WIN_ID;
    LCD_ANGLE_E lcd_angle=0;
    
    if (!s_atv_current_para.is_display_panel)
    {
        StopPanelTimer();
        DisappearATVWin(win_id,FALSE,TRUE);
        
        return;
    }
	
#ifndef MMI_ATV_SWITCH_SCREEN_MODE_SUPPORT    
    if(MMIATV_IsSearching())
    {
        return;
    }
    else if (MMIATV_IsRecording())
    {
        MMIATV_DisplayCommonTip(TXT_ATV_STOP_RECORD_FISRT);//提示停录制
        return;
    }

    
    lcd_angle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);

    if (!MMITHEME_IsMainScreenLandscape())
    {
                
        //全屏播放都是横屏，非横屏需要旋屏
        switch(lcd_angle) {
        case LCD_ANGLE_0:
            SetAtvWinAngle(LCD_ANGLE_90);   
            break;
        case LCD_ANGLE_90:
            SetAtvWinAngle(LCD_ANGLE_0);    
            break;
        case LCD_ANGLE_180:
            SetAtvWinAngle(LCD_ANGLE_270); 
            break;
        case LCD_ANGLE_270:
            SetAtvWinAngle(LCD_ANGLE_180); 
            break;
        default:
            break;
        }
       
    }
    else
    {
        if(lcd_angle != GUILCD_GetInitAngle(GUI_MAIN_LCD_ID))
        {
            SetAtvWinAngle(GUILCD_GetInitAngle(GUI_MAIN_LCD_ID));
        }
        else
        {
            StopPanelTimer();
            DisappearATVWin(win_id,TRUE,TRUE);
        }
        
    }
#endif
}

/*****************************************************************************/
//  Description : set volume callback
//  Global resource dependence : none
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void SetVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr)
{
    GUI_BG_T vol_bg = {0};
    MMISRV_HANDLE_T audio_handle = MMIATV_GetAudioHandle();
    
    if(NULL == para_ptr)
    {
        return;
    }
    
    vol_bg.bg_type = GUI_BG_IMG; 
    //SCI_TRACE_LOW:"[MMIATV]: SetVolumeCallback  volume = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_4661_112_2_18_2_3_23_123,(uint8*)"d", para_ptr->cur_value);
    MMIAPISET_SetMultimVolume(para_ptr->cur_value);
    
    //set volume
    if(audio_handle > 0)
    {
        MMISRVAUD_SetVolume(audio_handle, para_ptr->cur_value);
    }
#ifndef MMI_PDA_SUPPORT
    if(MMISET_VOL_ZERO == para_ptr->cur_value)
    {
#if (!defined MMI_RES_LOW_COST) || (!defined MMI_ATV_MINI_SUPPORT)  
        vol_bg.img_id = IMAGE_COMMON_PLAYER_BTN_VOL_MUTE;
#endif        
        GUIBUTTON_SetBg(MMIATV_BUTTON_VOL_CTRL_ID, &vol_bg);
        GUIBUTTON_Update(MMIATV_BUTTON_VOL_CTRL_ID);
    }
    else
    {
#if (!defined MMI_RES_LOW_COST) || (!defined MMI_ATV_MINI_SUPPORT)  
        vol_bg.img_id = IMAGE_COMMON_PLAYER_BTN_VOL;
#endif        
        GUIBUTTON_SetBg(MMIATV_BUTTON_VOL_CTRL_ID, &vol_bg);
        GUIBUTTON_Update(MMIATV_BUTTON_VOL_CTRL_ID);
    }
#else
    MMIATV_UpdateButtonStatus(FALSE);
#endif
}

/*****************************************************************************/
//  Description : turn volume
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_TurnVolume(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type)
{
    MMI_WIN_ID_T    win_id = MMIATV_MAIN_WIN_ID;
    MMICOM_PANEL_INFO_T vol_panel = {0};  
    
    
    if(MMIATV_IsSearching())
    {
        return;
    }    
    
    vol_panel.x = -1;
    vol_panel.y = -1;
    vol_panel.panel_type = MMICOM_PANEL_VOLUME;
    vol_panel.open_type = turn_vol_type;
    vol_panel.min_value  = 0;
    vol_panel.max_value  = MMIATV_VOLUME_MAX;
    vol_panel.cur_value  = MMIAPISET_GetMultimVolume();
    vol_panel.last_value = MMIAPISET_GetLastMultimVolume();
    vol_panel.set_callback = SetVolumeCallback;
    MMIAPICOM_OpenPanelChildWin(win_id, &vol_panel);

}
/*****************************************************************************/
//  Description : turn brightness
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void adjustBrightness(MMICOM_PANEL_OPEN_TYPE_E turn_bright_type)
{
    MMICOM_PANEL_INFO_T bright_panel = {0};

    if(MMIATV_IsSearching())
    {
        return; //搜索中不响应亮度调节
    }    

    bright_panel.x = -1;
    bright_panel.y = -1;

    bright_panel.cur_value  = MMIATV_GetBrightness();
    bright_panel.panel_type = MMICOM_PANEL_BRIGHTNESS;
    bright_panel.min_value  = 0;
    bright_panel.max_value  = MMIATV_BRIGHTNESS_MAX;
    bright_panel.set_callback = SetBrightnessCallback;
    bright_panel.open_type = turn_bright_type;

    MMIAPICOM_OpenPanelChildWin(MMIATV_MAIN_WIN_ID,&bright_panel);
}
/*****************************************************************************/
//  Description : set volume callback
//  Global resource dependence : none
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void SetBrightnessCallback(MMICOM_PANEL_CALLBACK_T *para_ptr)
{
    if(NULL == para_ptr)
    {
        return;
    }

    //SCI_TRACE_LOW:"[MMIATV]: SetBrightnessCallback  brightness = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_4753_112_2_18_2_3_23_125,(uint8*)"d", para_ptr->cur_value);
    MMIATV_SetBrightness(para_ptr->cur_value);
    
    //set brightness
    LCD_SetBackLightBrightness((MMIATV_LCD_CONTRSAT_MAX * para_ptr->cur_value) / MMIATV_BRIGHTNESS_MAX);
}

/*****************************************************************************/
//  Description : 设置主播放窗口是否允许旋屏
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIATV_EnableLcdSwitch(BOOLEAN is_enable)
{
#ifndef MMI_ATV_SWITCH_SCREEN_ON_OPEN
    MMI_HANDLE_T  win_id = MMIATV_MAIN_WIN_ID;
    SCI_TRACE_LOW("atv MMIATV_EnableLcdSwitch ");
    MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_CUR);
#endif
}

/*****************************************************************************/
//  Description : 视频播放区域清为透明色
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void ClearVideoRect(BOOLEAN is_clear_always)
{
    GUI_RECT_T video_rect = {0};
    GUI_LCD_DEV_INFO dev_info = {0};
    
    if(s_atv_current_para.is_display_panel)
    {
        GUIOWNDRAW_GetRect(MMIATV_VIDEO_OWNDRAW_CTRL_ID, &video_rect); 
    }
    else
    {
        //clear full screen
        video_rect = MMITHEME_GetFullScreenRect();
    }
    if(is_clear_always)
    {
        GUI_FillRect(&dev_info, video_rect, MMIATV_TRANSPARENT_COLOR);
    }
    else
    {
        ClearRect(video_rect, FALSE);
    }
}

/*****************************************************************************/
//  Description : set mute
//  Global resource dependence : none
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void SetMute(void)
{
    MMICOM_PANEL_CALLBACK_T para = {0};
    uint8                   vol = MMIAPISET_GetMultimVolume();

    if(MMIATV_IsSearching())
    {
        return;
    }

    if(MMIAPICOM_SwitchPanelValue(MMICOM_PANEL_VOLUME))
    {
        return;
    }
    else
    {
        if(MMISET_VOL_ZERO == vol)
        {
            para.cur_value = MMIAPISET_GetLastMultimVolume();
            SetVolumeCallback(&para);
        }
        else
        {
            para.cur_value = 0;
            para.last_value = MMIAPISET_GetLastMultimVolume();
            SetVolumeCallback(&para);
        }
    }

    return;
}

/*****************************************************************************/
//  Description : ATV字符的style风格
//  Global resource dependence : 
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL GUISTR_STYLE_T GetCommonTextStyle(void)
{
    GUISTR_STYLE_T      text_style = {0};
    
    // text style
    text_style.char_space = 1;
    text_style.line_space = 1;
    text_style.effect = FONT_EFFECT_CUSTOM;
    text_style.font = MMIATV_NORMAL_FONT;
    text_style.font_color = MMI_GRAY_WHITE_COLOR;

    return text_style;
}

/*****************************************************************************/
//  Description : ATV字符的style风格
//  Global resource dependence : 
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL GUISTR_STATE_T GetCommonTextState(void)
{
    GUISTR_STATE_T  state = GUISTR_STATE_EFFECT | GUISTR_STATE_NOT_HALF_CHAR | GUISTR_STATE_WORDBREAK;

    return state;
}

/*****************************************************************************/
//  Description : 获取ATV通用字符的宽度
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetStringWidth(MMI_STRING_T  *string)
{
    GUISTR_STYLE_T style = GetCommonTextStyle();
    GUISTR_STATE_T state = GetCommonTextState();

    return GUISTR_GetStringWidth(&style, string, state);
}

/*****************************************************************************/
//  Description : start record timer
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void StartRecordTimer(void)
{
    if(0 != s_atv_current_para.record_timer_id)
    {
        MMK_StopTimer(s_atv_current_para.record_timer_id);
    }
    s_atv_current_para.record_timer_id = MMK_CreateTimer(MMI_500MSECONDS, TRUE);  
}

/*****************************************************************************/
//  Description : stop record timer
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void StopRecordTimer(void)
{
    if(0 != s_atv_current_para.record_timer_id)
    {
        MMK_StopTimer(s_atv_current_para.record_timer_id);
        s_atv_current_para.record_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 更新一直显示的字串
/*****************************************************************************/
LOCAL void UpdateAlwaysTip(void)
{
    MMI_TEXT_ID_T  text_id = s_atv_current_para.always_text_id;
    if(0 != text_id && TXT_NULL != text_id)
    {
        MMIATV_DisplayCommonTipAlways(text_id);
    }
}

//PDA style UI
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 我的收藏 长按弹出选项消息处理.
//  Global resource dependence : 
//  Author: chunyou.zhu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFavoriteServiceListLongOkOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E                result                          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id                         = MMIATV_FAVORITE_SERVICE_LIST_LONGOK_OPT_CTRL_ID;
    //MMI_CTRL_ID_T               list_ctrl_id                    = MMIATV_FAVORITE_SERVICE_LIST_CTRL_ID;
    MMI_MENU_ID_T               menu_id                         = 0;
    MMI_MENU_GROUP_ID_T         group_id                        = MMIATV_FAVORITE_SERVICE_LIST_LONGOK_OPT_MENU;      
    GUIMENU_TITLE_INFO_T        title_info                      = {0};
    GUIMENU_BUTTON_INFO_T       button_info                     = {0};    
    MMI_STRING_T                main_title                       = {0};
    MMI_STRING_T                sub_title                         = {0};
    uint32                             list_num                          = 0;
    uint32                             index                               = 0;
    MMIATV_SERVICE_NODE_T*   cur_node_ptr            = PNULL;
    MMI_WIN_ID_T        query_win_id                          = MMIATV_QUERY_WIN_ID;
    
    //SCI_TRACE_LOW:"[VP] HandleFavoriteServiceListLongOkOptMenuWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_5059_112_2_18_2_3_24_128,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_FULL_PAINT:
        list_num = MMIATV_GetFavoriteServiceListNum();
        if (0 == list_num)
        {
            MMIAPICOM_EnableGrayed(win_id,group_id,MMIATV_ID_MENU_EDIT,TRUE);
            MMIAPICOM_EnableGrayed(win_id,group_id,MMIATV_ID_MENU_DELETE,TRUE);
        }
        
        //set title
        index = GUILIST_GetCurItemIndex(MMIATV_FAVORITE_SERVICE_LIST_CTRL_ID);
        cur_node_ptr = MMIATV_GetFavoriteServiceNodeViaIndex(index);
        if(PNULL != cur_node_ptr)
        {
            main_title.wstr_ptr = cur_node_ptr->service_name;
            main_title.wstr_len = MMIAPICOM_Wstrlen(main_title.wstr_ptr);
        }
        title_info.is_static    = TRUE;
        title_info.group_id     = group_id;
        title_info.title_ptr    = &main_title;
        title_info.sub_title_ptr = &sub_title; 
        GUIMENU_SetMenuTitleEx(ctrl_id, &title_info);
        
        //set context menu button style
        button_info.is_static   = TRUE;
        button_info.group_id    = group_id;
        button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctrl_id, &button_info);
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_MIDSK:  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        
        //SCI_TRACE_LOW:"[VP] HandleFavoriteServiceListLongOkOptMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_5099_112_2_18_2_3_24_129,(uint8*)"d",menu_id);
        switch (menu_id)
        {
        case MMIATV_ID_MENU_EDIT:
            MMK_CreateWin((uint32*)MMIATV_FAVORITE_NAME_EDIT_WIN_TAB,PNULL);
            break;
            
        case MMIATV_ID_MENU_DELETE:
            MMIPUB_OpenQueryWinByTextId(TXT_COMMON_CONFIRM_DELETE, 
                IMAGE_PUBWIN_QUERY,         
                &query_win_id,
                HandleDeleteQueryWinMsg);
            break;        
            
        default:
            break;
        } 
        MMK_CloseWin(win_id);
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

/*****************************************************************************/
//  Description : 频道列表 长按选项消息处理.
//  Global resource dependence : 
//  Author: chunyou.zhu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleServiceListLongOkOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E                result                          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id                         = MMIATV_SERVICE_LIST_LONGOK_OPT_CTRL_ID;
    //MMI_CTRL_ID_T               list_ctrl_id                    = MMIATV_SERVICE_LIST_CTRL_ID;
    MMI_MENU_ID_T               menu_id                         = 0;
    MMI_MENU_GROUP_ID_T         group_id                        = MMIATV_SERVICE_LIST_LONGOK_OPT_MENU;      
    GUIMENU_TITLE_INFO_T        title_info                      = {0};
    GUIMENU_BUTTON_INFO_T       button_info                     = {0};    
    MMI_STRING_T                main_title                       = {0};
    MMI_STRING_T                sub_title                         = {0};
    uint32                             list_num                          = 0;
    uint32                             index                               = 0;
    MMIATV_SERVICE_NODE_T*   cur_node_ptr            = PNULL;
    
    //SCI_TRACE_LOW:"[VP] HandleServiceListLongOkOptMenuWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_5154_112_2_18_2_3_24_130,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_FULL_PAINT:
        list_num = MMIATV_GetServiceListNum();
        if (0 == list_num)
        {
            MMIAPICOM_EnableGrayed(win_id,group_id,MMIATV_ID_MENU_ADD_FAVORITE,TRUE);
        }
        
        //set title
        index = GUILIST_GetCurItemIndex(MMIATV_SERVICE_LIST_CTRL_ID);
        cur_node_ptr = MMIATV_GetServiceNodeViaIndex(index);
        if(PNULL != cur_node_ptr)
        {
            main_title.wstr_ptr = cur_node_ptr->service_name;
            main_title.wstr_len = MMIAPICOM_Wstrlen(main_title.wstr_ptr);
        }
        title_info.is_static    = TRUE;
        title_info.group_id     = group_id;
        title_info.title_ptr    = &main_title;
        title_info.sub_title_ptr = &sub_title; 
        GUIMENU_SetMenuTitleEx(ctrl_id, &title_info);
        
        //set context menu button style
        button_info.is_static   = TRUE;
        button_info.group_id    = group_id;
        button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctrl_id, &button_info);
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_MIDSK:  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        
        //SCI_TRACE_LOW:"[VP] HandleServiceListLongOkOptMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_WINTAB_5193_112_2_18_2_3_24_131,(uint8*)"d",menu_id);
        switch (menu_id)
        {
        case MMIATV_ID_MENU_ADD_FAVORITE:
            if(MMIATV_GetServiceListNum() > 0)
            {
                index = GUILIST_GetCurItemIndex(MMIATV_SERVICE_LIST_CTRL_ID);
                cur_node_ptr = MMIATV_GetServiceNodeViaIndex(index);
                if(PNULL != cur_node_ptr)
                {
                    if(MMIATV_GetFavoriteServiceListNum() < MMIATV_FAVORITE_MAX_NUM)
                    {
                        MMIATV_AddFavoriteServiceNode(cur_node_ptr);
                        MMIATV_WriteFavoriteServiceListFile();
                        
                        //refresh favorite list
                        MMK_PostMsg(MMIATV_FAVORITE_SERVICE_LIST_WIN_ID, MSG_APP_ATV_REFRESH_LIST, PNULL, NULL);
                        //MMIATV_OpenCommonAlertWin(TXT_ATV_ADD_FAVORITE_SERVICE_OK, IMAGE_PUBWIN_SUCCESS);
                    }
                    else
                    {
                        MMIATV_OpenCommonAlertWin(TXT_ATV_FAVORITE_SERVICE_FULL, IMAGE_PUBWIN_FAIL);
                    }
                }
            }
            break;
     
        default:
            break;
        } 
        MMK_CloseWin(win_id);
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

#endif //MMI_PDA_SUPPORT

/*****************************************************************************/
//  Description : 使主界面消失
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisappearATVWin(MMI_WIN_ID_T  win_id,
                          BOOLEAN is_disappear,
                          BOOLEAN is_fresh)
{
    //GUI_RECT_T lcd_rect = {0};
    
    IGUICTRL_T* ctrl_handle = PNULL;  
    GUI_BG_T bg = {0};
    
    if (!MMK_IsFocusWin(win_id))
    {
        return;
    }        
    SCI_TRACE_LOW("DisappearATVWin is_disappear = %d",is_disappear);
    ClearFullScreen(); 

    if (is_disappear)
    {       
        s_atv_current_para.is_display_panel = FALSE;
#ifdef MMI_ATV_MINI_SUPPORT    
        GUIBUTTON_SetVisible(MMIATV_BUTTON_OPTION_CTRL_ID, FALSE, is_fresh);
        GUIBUTTON_SetVisible(MMIATV_BUTTON_PRE_CTRL_ID, FALSE, is_fresh);
        GUIBUTTON_SetVisible(MMIATV_BUTTON_NEXT_CTRL_ID, FALSE, is_fresh);
        GUIBUTTON_SetVisible(MMIATV_BUTTON_RETURN_CTRL_ID, FALSE, is_fresh);
		GUIOWNDRAW_SetVisible(MMIATV_VIDEO_OWNDRAW_CTRL_ID, FALSE, is_fresh);
		MMIATV_ClearCommonTip();
#endif
		s_atv_current_para.is_display_panel = FALSE;
        GUIFORM_SetVisible(MMIATV_FORM_CTRL_ID,FALSE,is_fresh);          
              
        if (!MMITHEME_IsMainScreenLandscape())   //竖屏需要设置softkey
        {     
            GUIWIN_SetSoftkeyTPEnable(win_id, FALSE);
            GUIWIN_SetSoftkeyVisible(win_id,FALSE);    
            
        }    

        //设置状态栏不可见
        ctrl_handle = MMK_GetCtrlPtrByWin(MMK_GetWinHandle(MMK_GetFirstAppletHandle(), win_id), MMITHEME_GetStatusBarCtrlId());
        if(0 != ctrl_handle)
        {
            IGUICTRL_SetState(ctrl_handle, GUICTRL_STATE_DISABLE_ACTIVE | GUICTRL_STATE_INVISIBLE | GUICTRL_STATE_DISABLE_TP, TRUE);
        }
        ClearFullScreen(); 
        
    }
    else 
    {
        if (!MMITHEME_IsMainScreenLandscape())
        {            
            GUIWIN_SetSoftkeyTPEnable(win_id, TRUE);
            GUIWIN_SetSoftkeyVisible(win_id,TRUE);
        }
        else
        {
            bg.bg_type = GUI_BG_COLOR;  
            bg.color = MMI_BLACK_COLOR;
            //bg.img_id = IMAGE_COMMON_BTN_BG;  
            GUIFORM_SetBg(MMIATV_PANEL_FORM_CTRL_ID, &bg);
        }
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI        
        GUIBUTTON_SetVisible(MMIATV_BUTTON_OPTION_CTRL_ID, TRUE, is_fresh);
        GUIBUTTON_SetVisible(MMIATV_BUTTON_PRE_CTRL_ID, TRUE, is_fresh);
        GUIBUTTON_SetVisible(MMIATV_BUTTON_NEXT_CTRL_ID, TRUE, is_fresh);
        GUIBUTTON_SetVisible(MMIATV_BUTTON_RETURN_CTRL_ID, TRUE, is_fresh);
		GUIOWNDRAW_SetVisible(MMIATV_VIDEO_OWNDRAW_CTRL_ID, TRUE, is_fresh);
#endif
        GUIFORM_SetVisible(MMIATV_FORM_CTRL_ID,TRUE,is_fresh);
        s_atv_current_para.is_display_panel = TRUE;

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
        }
        CreatePanelTimer();
        
    }

    if (MMIATV_SYSTIME_COMMON_TIP != (s_atv_current_para.systimer_type & MMIATV_SYSTIME_COMMON_TIP))
    {
        MMIATV_ClearCommonTip();
        if (TXT_NULL != s_atv_current_para.text_id)
        {
            MMIATV_DisplayCommonTip(s_atv_current_para.text_id);
        }       
    }
    UpdateAlwaysTip();
    if(MMIATV_SYSTIME_SERVICE_NUMBER == (s_atv_current_para.systimer_type & MMIATV_SYSTIME_SERVICE_NUMBER))
    {
        MMIATV_DisplayServiceNum();
    }
	if (MMIATV_IsRecording())
	{
		DisplayRecordTip();
	}
    if (is_fresh)
    {
        MMIAPICOM_UpdatePanel();  //如果打开音量/亮度控制面板，则需要更新
    }    
}
