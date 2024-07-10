/****************************************************************************
** File Name:      mmimtv_setting.h                                         *
** Author:         xin.li                                                   *
** Date:           2008-12                                                  *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mtv OSD display.       *
****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 12/2008        xin.li           Create
****************************************************************************/
#ifdef CMMB_SUPPORT
#ifndef _MMIMTV_OSD_DISPLAY_H_
#define _MMIMTV_OSD_DISPLAY_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmidc_gui.h"
#include "mmimtv_position.h"
#include "mmimtv_data.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef enum
{
    MMIMTV_ICON_MUTE = 0,
//#ifndef MBBMS_SUPPORT
    MMIMTV_ICON_RECORD,
//#endif
    MMIMTV_ICON_SNAPSHOT,
    MMIMTV_ICON_ESG,
    //TO DO MMIMTV_ICON_STEREO,
    MMIMTV_ICON_BRIGHTNESS,
	MMIMTV_ICON_PROGRAM_NAME
}MMIMTV_ICON_E;

#define     MMIMTV_SYSTIME_ALL               0xffffffff
#define     MMIMTV_BROADCAST_MAX_LEVEL       4
/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenShortcutIconScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseShortcutIconScene(void);


/*****************************************************************************/
// 	Description : open MMIMTV_MAIN_MENU scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenMainMenuScene(uint32 param);

/*****************************************************************************/
// 	Description : close MMIMTV_MAIN_MENU scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseMainMenuScene(void);

/*****************************************************************************/
// 	Description : open OSD_SEARCH_MENU scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSearchMenuScene(uint32 param);

/*****************************************************************************/
// 	Description : close OSD_SEARCH_MENU scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSearchMenuScene(void);

/*****************************************************************************/
// 	Description :open MMIMTV_SERVICE_LIST scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenServiceListScene(uint32 param);

/*****************************************************************************/
// 	Description : close MMIMTV_SERVICE_LIST scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseServiceListScene(void);


/*****************************************************************************/
// 	Description : open MMIMTV_SERVICE_LIST_OPT scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenServiceListOptScene(uint32 param);

/*****************************************************************************/
// 	Description : close MMIMTV_SERVICE_LIST_OPT scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseServiceListOptScene(void);

/*****************************************************************************/
// 	Description : open MMIMTV_NET_LIST scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenNetListScene(uint32 param);

/*****************************************************************************/
// 	Description : close MMIMTV_NET_LIST scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseNetListScene(void);


/*****************************************************************************/
// 	Description : open MMIMTV_NET_LIST_OPT scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenNetListOptScene(uint32 item_index);

/*****************************************************************************/
// 	Description : close MMIMTV_NET_LIST_OPT scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseNetListOptScene(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearScreen(void);

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSettingScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSettingScene(void);

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenAudioLanguageScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseAudioLanguageScene(void);

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSubtitleSettingScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSubtitleSettingScene(void);

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenAudioModeScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseAudioModeScene(void);

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenZoomModeScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseZoomModeScene(void);

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenRecordStoreScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseRecordStoreScene(void);

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSnapShotStoreScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSnapShotStoreScene(void);

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenTransparencySettingScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseTransparencySettingScene(void);

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenDisplayTimeSettingScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseDisplayTimeSettingScene(void);

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenWatchTipSettingScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseWatchTipSettingScene(void);

/*****************************************************************************/
// 	Description : open setting scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenBroadcastBgPicSettingScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseBroadcastBgPicSettingScene(void);

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author:
//	Note: 通用确认场景，左软键"确认"，右软键"取消"
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonQueryScene(uint32 param);

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author:
//	Note: open by text
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonQuerySceneByTextPtr(MMI_STRING_T *text_ptr);

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author: xin.li
//	Note: 右软键为"退出"的询问场景
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonQuerySceneWithSRExit(uint32 param);

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonTipScene(uint32 param);

/*****************************************************************************/
// 	Description : open common tip scene by text
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonTipSceneByTextPtr(MMI_STRING_T *text_ptr);

/*****************************************************************************/
// 	Description : open common confirm scene
//	Global resource dependence :
//  Author:
//	Note: 确定后消失
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonConfirmScene(uint32 param);

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonWaitWithNoCancelScene(uint32 param);

/*****************************************************************************/
// 	Description : open OSD_ENTER_WARNING scene
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseCommonMsgBox(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenEBListScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseEBListScene(void);

/*****************************************************************************/
// 	Description : open record list scene
//	Global resource dependence :
//  Author:
//	Note: 直接使用资源管理器打开列表窗口，MTV不维护录像列表
/*****************************************************************************/
PUBLIC void MMIMTV_OpenRecordListScene(void);

/*****************************************************************************/
// 	Description : open MMIMTV_QUIT_QUERY scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenCommonWaitScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayServiceNumber(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearServiceNumber(void);

/*****************************************************************************/
// 	Description : display input num when num switch
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayInputNum(uint32 num);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayProgramHint(BOOLEAN is_new);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearProgramHint(void);

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenHelpScene(uint32 param);

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseHelpScene(void);

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayVolumeProgress(MMIMTV_VOLUME_E volume);

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSettingPictureScene(uint32 param);

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSettingPictureScene(void);

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenBookRecordScene(uint32 param);

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseBookRecordScene(void);

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenEditRecordFileTimeScene(uint32 param);

/*****************************************************************************/
// 	Description : open help scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseEditRecordFileTimeScene(void);

/*****************************************************************************/
// 	Description : open OSD_MAIN_MENU scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenManualSearchInputScene(uint32 param);

/*****************************************************************************/
// 	Description : update OSD_MAIN_MENU scene
//	Global resource dependence :
//  Author: xin.li
//	Note: 仅更新softkey，根据EDITBOX情况显示“删除”或“返回”
/*****************************************************************************/
PUBLIC void MMIMTV_UpdateManualSearchInputScene(void);

/*****************************************************************************/
// 	Description : open OSD_MAIN_MENU scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseManualSearchInputScene(void);

/*****************************************************************************/
// 	Description : open MMIMTV_NET_LIST_OPT scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSystemInforScene(uint32 param);

/*****************************************************************************/
// 	Description : close MMIMTV_NET_LIST_OPT scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSystemInforScene(void);

/*****************************************************************************/
// 	Description : open CA info scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSystemCAInfoScene(uint32 param);

/*****************************************************************************/
// 	Description : close CA info scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSystemCAInfoScene(void);

/*****************************************************************************/
// 	Description : open SW info scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSystemSWInfoScene(uint32 param);

/*****************************************************************************/
// 	Description : close SW info scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSystemSWInfoScene(void);

/*****************************************************************************/
// 	Description : open KDA info scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSystemKDAInfoScene(uint32 param);

/*****************************************************************************/
// 	Description : open KDA info scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSystemKDAInfoScene(void);

/*****************************************************************************/
// 	Description : open MMIMTV_NET_LIST_OPT scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenOtherFunctionScene(uint32 param);

/*****************************************************************************/
// 	Description : close MMIMTV_NET_LIST_OPT scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseOtherFunctionScene(void);

/*****************************************************************************/
// 	Description : open OSD_RECORD_PLAY scene
//	Global resource dependence :
//  Author: xin.li
//	Note: 打开本地回放场景
/*****************************************************************************/
PUBLIC void MMIMTV_OpenRecordPlayScene(uint32 param);

/*****************************************************************************/
// 	Description : close OSD_RECORD_PLAY scene
//	Global resource dependence :
//  Author: xin.li
//	Note: 关闭本地回放场景
/*****************************************************************************/
PUBLIC void MMIMTV_CloseRecordPlayScene(void);

/*****************************************************************************/
// 	Description : open OSD_RECORD_PLAY scene
//	Global resource dependence :
//  Author: xin.li
//	Note: 更新本地回放场景，主要是软键文字显示更新
/*****************************************************************************/
PUBLIC void MMIMTV_UpdateRecordPlayScene(void);

/*****************************************************************************/
// 	Description : display record play time count
//	Global resource dependence :
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayRecordPlayTime(uint32 time_count);

/*****************************************************************************/
// 	Description : display record play progress
//	Global resource dependence :
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayRecordPlayProgress(uint32 time_count, uint32 time_total);

/*****************************************************************************/
// 	Description : get icon handle
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC DC_ICONS_OP* MMIMTV_GetIconHandle(void);

/*****************************************************************************/
// 	Description : display soft key base hor or ver screen
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_GetSoftKeyRect(
                                  GUI_RECT_T *left_rect, 
                                  GUI_RECT_T *middle_rect,
                                  GUI_RECT_T *right_rect
                                  );

/*****************************************************************************/
// 	Description : close MMIMTV_SERVICE_LIST scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_GetChangeNetArrowRect(
                                         GUI_RECT_T *left_rect,
                                         GUI_RECT_T *right_rect
                                         );

/*****************************************************************************/
// 	Description : get progress rect
//	Global resource dependence :
//  Author:
//	Note: if progress pannel is not opened, rect info is null
/*****************************************************************************/
PUBLIC void MMIMTV_GetProgressRect(GUI_RECT_T *rect);

/*****************************************************************************/
// 	Description : get volume progress rect
//	Global resource dependence :
//  Author:
//	Note: if volume progress pannel is not opened, rect info is null
/*****************************************************************************/
PUBLIC void MMIMTV_GetVolumeProgressRect(GUI_RECT_T *rect);

/*****************************************************************************/
//  Description : stop text scroll timer
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIMTV_GetDisappearTimer(void);

/*****************************************************************************/
//  Description : stop text scroll timer
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ResetDisappearTimer(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_GetChangeServiceArrowRect(
                                             GUI_RECT_T *left_rect,
                                             GUI_RECT_T *right_rect
                                             );

/*****************************************************************************/
//  Description : stop text scroll timer
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_StopSysTimer(uint32 type);

/*****************************************************************************/
//  Description : stop text scroll timer
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_HandleSystimer(void);

/*****************************************************************************/
// 	Description : 启动数字键切换频道定时器
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_StartNumSwitchTimer(void);

/*****************************************************************************/
// 	Description : 停止数字键切换频道定时器
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_StopNumSwitchTimer(void);

/*****************************************************************************/
//  Description : get switch play timer id
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIMTV_GetSwitchPlayTimerId(void);

/*****************************************************************************/
//  Description : get switch play timer id
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_HandleSwitchPlayTimer(void);

/*****************************************************************************/
//  Description : reset switch play timer count
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ResetSwitchPlayTimer(void);

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearVolumeProgress(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CreateWatchTimeTipTimer(void);

/*****************************************************************************/
// 	Description : open MMIMTV_SHORTCUT_ICON scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearWatchTip(void);

/*****************************************************************************/
// 	Description : fill rect in mtv
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_NewOSDDisplayData(void);

/*****************************************************************************/
// 	Description : fill rect in mtv
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteOSDDisplayData(void);

/*****************************************************************************/
// 	Description : open OSD_MAIN_MENU scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseESGListScene(uint32 param);

/*****************************************************************************/
// 	Description : open OSD_MAIN_MENU scene
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenESGListScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_GetChangeESGServiceArrowRect(GUI_RECT_T *left_rect, 
                                                GUI_RECT_T *right_rect);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_GetChangeESGDateArrowRect(GUI_RECT_T *left_rect,
                                             GUI_RECT_T *right_rect);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenEBListOptScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseEBListOptScene(void);

/*****************************************************************************/
// 	Description : open EB detail scene via index
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenEBListContentScene(uint32 param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseEBListContentScene(void);

/*****************************************************************************/
// 	Description : open switch service tab ctrl
//	Global resource dependence :
//  Author: xin.li
//	Note: 频道切换栏 for mbbms2.0
/*****************************************************************************/
PUBLIC void MMIMTV_OpenSwitchServiceTab(uint32 param);

/*****************************************************************************/
// 	Description : open switch service tab ctrl
//	Global resource dependence :
//  Author: xin.li
//	Note: 频道切换栏 for mbbms2.0
/*****************************************************************************/
PUBLIC void MMIMTV_CloseSwitchServiceTab(void);

/*****************************************************************************/
// 	Description : is opened switch service tab ctrl
//	Global resource dependence :
//  Author: xin.li
//	Note: 频道切换栏 for mbbms2.0
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_IsOpenSwitchServiceTab(void);

/*****************************************************************************/
// 	Description : 播放连接等待
//	Global resource dependence :
//  Author:
//	Note: MBBMS使用
/*****************************************************************************/
PUBLIC void MMIMTV_OpenPlayConnectingScene(void);

/*****************************************************************************/
// 	Description : 更新连接等待
//	Global resource dependence :
//  Author:
//	Note: MBBMS使用
/*****************************************************************************/
PUBLIC void MMIMTV_UpdatePlayConnectingScene(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note: MBBMS使用
/*****************************************************************************/
PUBLIC void MMIMTV_ClosePlayConnectingScene(void);

/*****************************************************************************/
// 	Description : display audio service tip
//	Global resource dependence :
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayAudioServiceTip(GUI_LCD_DEV_INFO *dev_ptr);

#ifdef MBBMS_SUPPORT
/*****************************************************************************/
// 	Description : 3,4级紧急广播询问
//	Global resource dependence :
//  Author:
//	Note: MBBMS使用
/*****************************************************************************/
PUBLIC void MMIMTV_OpenNewEBQueryScene(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note: MBBMS使用
/*****************************************************************************/
PUBLIC void MMIMTV_CloseNewEBQueryScene(void);
#endif

/*****************************************************************************/
// 	Description : 一直显示频道号，无3秒定时器
//	Global resource dependence :
//  Author:
//	Note: 为MBBMS移动Case增加
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayServiceNumberAlways(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayRecordTip(uint32 time);

/*****************************************************************************/
// 	Description : clear recording tip
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearRecordTip(void);

/*****************************************************************************/
// 	Description : 提示录像已保存
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayRecordSaveTip(void);

/*****************************************************************************/
// 	Description : 提示截图已保存
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisplaySnapshotTip(void);

/*****************************************************************************/
// 	Description : 刷新文件已保存区域
//	Global resource dependence :
//  Author:
//	Note: 文件已保存提示有：录制文件、捕捉图像
/*****************************************************************************/
PUBLIC void MMIMTV_ClearFileSavedTip(void);

/*****************************************************************************/
// 	Description : get signal icon id for status bar
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIMTV_GetTVStatusImageID(uint32 signal);

/*****************************************************************************/
// 	Description : display eb current index and total number
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisplayEBCountInfo(uint16 cur_index, uint16 total_num);

/*****************************************************************************/
// 	Description :start the status bar disable timer
//	Global resource dependence : none
//  Author: yexiong.liu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_StartStbDisableTimer(void);

/*****************************************************************************/
// 	Description : start the status bar disable timer
//	Global resource dependence : none
//  Author:yexiong.liu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_StopStbDisableTimer(void);

/*****************************************************************************/
//  Description : get stb disable timer id
//  Global resource dependence : 
//  Author: yexiong.liu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIMTV_GetStbDisableTimerId(void);

/*****************************************************************************/
//  Description : get switch play timer id
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_HandleStbDisableTimer(void);

/*****************************************************************************/
//  Description : Disable Stb
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DisableStb(void);
/*****************************************************************************/
//  Description : enable Stb
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_EnableStb(void);



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIMTV_MMIMTV_DISPLAY_H_ */
#endif
