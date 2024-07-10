/*****************************************************************************
** File Name:      mmiset_wintab.h                                           *
** Author:                                                                   *
** Date:           2006/09/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe setting definitions          *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18    kelly.li          Create
******************************************************************************/

#ifndef _MMISET_WINTAB_H_
#define _MMISET_WINTAB_H_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_type.h"
//#include "mmiset.h"
#include "mmiset_internal.h"
#include "mmipub.h"
#include "guibutton.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/



typedef enum
{
    MMISET_IDLE_TIME_DATE,
    MMISET_IDLE_NETWORK_NAME1,
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD ||  defined MMI_MULTI_SIM_SYS_DUAL                                                             
    MMISET_IDLE_NETWORK_NAME2,
#endif
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD  
    MMISET_IDLE_NETWORK_NAME3,
#endif   
#if defined MMI_MULTI_SIM_SYS_QUAD  
    MMISET_IDLE_NETWORK_NAME4,
#endif
#ifdef DPHONE_SUPPORT
    MMISET_IDLE_TODAY_SCHEDULE,    
    MMISET_IDLE_COUNTDOWN,  //count down
    MMISET_IDLE_DISPLAY_MAX_ITEM,
#else    
    MMISET_IDLE_TODAY_SCHEDULE,  
#ifdef COUNTEDTIME_SUPPORT
    MMISET_IDLE_COUNTDOWN,  //count down
#endif    
#ifdef MMI_AUDIO_PLAYER_SUPPORT
    MMISET_IDLE_MP3,    
#endif    
#ifdef FM_SUPPORT
    MMISET_IDLE_FM, 
#endif    
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
#if !(defined(MMI_DISPLAYSET_MINI_SUPPORT) && defined(MMI_ENABLE_DCD)) 
    MMISET_IDLE_CUSTOM_WORD,
#endif
#endif
#ifdef MMI_IDLE_STKTEXT_SUPPORT
    MMISET_IDLE_STK_IDLE_TEXT,     //stk idle text
#endif
#ifdef MMI_WIFI_SUPPORT
    MMIIDLE_LINE_WLAN, // wlan added by rong.gu
#endif    
    //MMISET_IDLE_SHORTCUT,     //shortcut icon
    MMISET_IDLE_DISPLAY_MAX_ITEM
#endif 
}MMISET_DILE_DISPLAY_E;

typedef enum
{
    GUIMENU_STYLE_INDEX_ICON,
    GUIMENU_STYLE_INDEX_TITLE_ICON,
    GUIMENU_STYLE_INDEX_SLIDE_PAGE,
#ifdef CUBE_MAIN_MENU_SUPPORT
    GUIMENU_STYLE_INDEX_CUBE,
#endif

#ifdef MENU_CRYSTALCUBE_SUPPORT
    GUIMENU_STYLE_INDEX_CRYSTAL_CUBE,
#endif

#ifdef MENU_CYCLONE_SUPPORT
    GUIMENU_STYLE_INDEX_CYCLONE,
#endif
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
	GUIMENU_STYLE_INDEX_GO,
#else
#ifdef TRAPEZOID_MAIN_MENU_SUPPORT
    GUIMENU_STYLE_INDEX_TRAPEZOID,
#endif

#ifdef SPHERE_MAIN_MENU_SUPPORT
    GUIMENU_STYLE_INDEX_SPHERE,
#endif

#ifdef CYLINDER_MAIN_MENU_SUPPORT
    GUIMENU_STYLE_INDEX_CYLINDER,
#endif

#ifdef BINARY_STAR_MAIN_MENU_SUPPORT
    GUIMENU_STYLE_INDEX_BINARY_STAR,
#endif    
#ifdef SHUTTER_MAIN_MENU_SUPPORT
    GUIMENU_STYLE_INDEX_SHUTTER,
#endif    

#ifdef CHARIOT_MAIN_MENU_SUPPORT
    GUIMENU_STYLE_INDEX_CHARIOT,
#endif 
#endif
#ifdef MENU_SWEETS_SUPPORT
    GUIMENU_STYLE_INDEX_SWEETS,
#endif
    GUIMENU_STYLE_INDEX_MAX
}GUIMENU_STYLE_INDEX_E;

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register set module nv len and max item
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_RegNv(void);

/*****************************************************************************/
// 	Description : open validate privacy password window
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_OpenVailidatePwdWin(
                                       MMISET_PRIVACY_CALLBACK_DATA* p_call_back_data
                                       );
                                       
/*****************************************************************************/
// 	Description : 恢复默认值
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_ResetDirectionKeyInfo(void);

/*****************************************************************************/
// 	Description : clean user date
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_CleanUserData(void);

/*****************************************************************************/
// 	Description : Register SET menu group
//	Global resource dependence : none
//  Author: KELLY.LI
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_RegMenuGroup(void);

#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
/*****************************************************************************/
// 	Description : close dual sim set cfnrc window
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_CloseDualsysCFNRCWin(MN_DUAL_SYS_E dual_sys, BOOLEAN  is_action_success, MN_SS_ERROR_CODE_E  error_code);
#endif

/*****************************************************************************/
// 	Description : Get call divert/bar/wating/hide id ctrl id
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMISET_GetDivertBarWatingHideidCtrlId(void);

/*****************************************************************************/
// 	Description : is open dual sys CFNRC
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsOpenDualSysCfnrc(void);
/*****************************************************************************/
// 	Description : 恢复默认值
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_ResetIdleShortcutInfo(void);

#ifdef FDN_SUPPORT
/*****************************************************************************/
//  Description : Open FDN Win
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_OpenFDNWin(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
// 	Description : to open pin win
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_OpenPinWin( MN_DUAL_SYS_E dual_sys );

/*****************************************************************************/
// 	Description : to open pin2 win
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_OpenPin2Win( MN_DUAL_SYS_E dual_sys );

#ifdef MMI_DUALMODE_ENABLE 
/*****************************************************************************/
// Description : reset network when resume factory setting
// Global resource dependence : 
// Author:kun.yu
// Note: 2009-12-28
/*****************************************************************************/
PUBLIC void MMISET_ResetNetWorkType(BOOLEAN only_write_nv);

/*****************************************************************************/
// Description : set switch rat state
// Global resource dependence : 
// Author:kun.yu
// Note: 2010-02-04
/*****************************************************************************/
PUBLIC void MMISET_SetSwitchRatState(BOOLEAN state);
#endif

#if 0//def TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// Description : reset touch vibrate setting
// Global resource dependence : 
// Author:kun.yu
// Note: 2010-06-23
/*****************************************************************************/
PUBLIC void MMISET_ResetTouchVibrateSetting(void);
#endif

#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
///*****************************************************************************/
////    Description : MMISET_EnterMultiSimSettingMenu
////    Global resource dependence : 
////  Author:
////    Note: 
///*****************************************************************************/
PUBLIC void MMISET_EnterMultiSimSettingMenu(void);
#endif

/*****************************************************************************/
//  Description : open network connect win
//  Global resource dependence : none
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_OpenNetworkConnectWin(
                                         MN_DUAL_SYS_E dual_sys,
                                         MMI_WIN_ID_T win_id,
                                         MMI_TEXT_ID_T text_id,
                                         MMIPUB_SOFTKEY_STYLE_E  softkey_style,  //softkey style
                                         MMIPUB_HANDLE_FUNC      handle_func     //handle function
                                         );

/*****************************************************************************/
//  Description : open blacklit edit win  
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_OpenBlacklistEditWin(MMIPB_BCD_NUMBER_T *orginal_addr_ptr);

/*****************************************************************************/
//  Description : open longpower menu  
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_LongpowerCreatmenu(void);

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
//  Description : MMISET_ResumePreLivewallpaper
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_ResumePreLivewallpaper(void);
#endif

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
//  Description : MMISET_ResumePreLivewallpaper
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_ResumePDAPreLivewallpaper(void);
#endif
#endif

/*****************************************************************************/
//  Description : append settings item 1 LINE
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
PUBLIC void AppendPdaSettingsItem1Line(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 uint32         user_data   //用户数据
                                 ); 
								     
/*****************************************************************************/
//  Description : append settings item 2 LINE2
//  Global resource dependence : 
//  Author: rong.gu 
//  Note: 
/*****************************************************************************/
PUBLIC void AppendPdaSettingsItem2Line(
                                       MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                       MMI_TEXT_ID_T  text_1,     //主文本
                                       MMI_STRING_T  text_str,     //副文本
                                       MMI_IMAGE_ID_T image_id,   //图标ID
                                       uint32         user_data   //用户数据
                                       );

#ifdef SCREENSAVER_SUPPORT                                 
/*****************************************************************************/
//  Description : create screensaver window
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMISETAPI_CreateScreensaverWin(void);
/*****************************************************************************/
//  Description : set ScreenSaver content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void SetScreenSaverParam(void);  

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:juan.wu
//	Note: 
/*****************************************************************************/
PUBLIC  MMI_RESULT_E MMISET_SetOkButtonCallback(void);
/*****************************************************************************/
//  Description : SetCancelButtonCallback
//  Global resource dependence : none
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_SetCancelButtonCallback(void);
#endif  

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: maryxiao
//  Note: 
// 
/*****************************************************************************/
PUBLIC void MMISET_CreateThemePubWin(void);
                                 
/*****************************************************************************/
//  Description : handle flight mode win msg
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_HandleFlightmodeWinMsg(
                                                  MMI_WIN_ID_T     win_id,
                                                  MMI_MESSAGE_ID_E msg_id,
                                                  DPARAM           param
                                                  );

