/*****************************************************************************
** File Name:      mmiset_shortcut.c                                         *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 1/2009        Jassmine              Creat
******************************************************************************/

#define _MMISET_SHORTCUT_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
//#include "mmiset.h"
#include "mmiset_internal.h"
#include "mmiset_nv.h"
#include "mmiset_id.h"
#include "mmiset_text.h"
#include "mmisms_text.h"
#include "mmipub.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"
#include "mmi_nv.h"
#include "mmiset_image.h"
#include "mmiidle_export.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmidc_export.h"
#include "mmifm_export.h"
//#include "mmigame.h"
//#include "mmigame_export.h"
#include "mmifmm_export.h"
#include "mmicalendar_export.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmialarm_export.h"
#ifdef EBOOK_SUPPORT
#include "mmiebook_export.h"
#endif
#include "mmisms_export.h"
#include "mmiset_wintab.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "mmicl_export.h"
#include "mmicalc_export.h"
#include "mmipicview_export.h"
#include "mmipicview_text.h"
#include "mmi_mainmenu_export.h"
#include "mmienvset_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
//#ifdef DCD_SUPPORT   //bug 2099158
#include "mmidcd_export.h"
//#endif
//#include "mmiss_export.h"
#ifdef CMMB_SUPPORT
#include "mmimtv_text.h"
#include "mmimtv_export.h"
#endif

#ifdef CMCC_UI_STYLE
#include "mmidm_config.h"
#endif

#ifdef SEARCH_SUPPORT
#include "mmisearch_export.h"
#endif
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#include "mmiconnection_text.h"
#endif
#ifdef ASP_SUPPORT
#include "mmiasp_export.h"
#include "mmiasp_text.h"
#endif
#ifdef ATV_SUPPORT
#include "mmiatv_export.h"
#include "mmiatv_text.h"
#endif
#include "guiform.h"
#include "guisetlist.h"
#include "guires.h"
#include "guictrl_api.h"
#include "mmiudisk_export.h"
#ifdef MCARE_V31_SUPPORT
#include "Mcare_Interface.h"
#include "McfInterface.h"
#endif
#if defined MMIWIDGET_SUPPORT || defined MMI_GRID_IDLE_SUPPORT
#include "mmiwidget.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

LOCAL const MMI_TEXT_ID_T s_mmiset_shortcut_title[] = {
                                                TXT_SET_FUNCKEY_SL,
                                                TXT_SET_FUNCKEY_RIGHT,
                                                TXT_SET_DIRECTIONKEY_UP,
                                                TXT_SET_DIRECTIONKEY_DOWN,
                                                TXT_SET_DIRECTIONKEY_LEFT,
                                                TXT_SET_DIRECTIONKEY_RIGHT,
                                                TXT_SET_IDLE_SHORTCUT1,
                                                TXT_SET_IDLE_SHORTCUT2,
                                                TXT_SET_IDLE_SHORTCUT3,
                                                TXT_SET_IDLE_SHORTCUT4,
                                                TXT_SET_IDLE_SHORTCUT5};

LOCAL const MMI_IMAGE_ID_T s_shortcut_list_bmp_id[] = 
{
    IMAGE_SET_SL,
    IMAGE_SET_SR,
    IMAGE_SET_DIRKEY_UP,
    IMAGE_SET_DIRKEY_DOWN,
    IMAGE_SET_DIRKEY_LEFT,
    IMAGE_SET_DIRKEY_RIGHT,
    IMAGE_SET_IDLE_SHORTCUT1,
    IMAGE_SET_IDLE_SHORTCUT2,
    IMAGE_SET_IDLE_SHORTCUT3,
    IMAGE_SET_IDLE_SHORTCUT4,
    IMAGE_SET_IDLE_SHORTCUT5
};

#ifdef CMCC_IDLE_DISPLAY    //移动定制规范的待机界面显示

//widget风格的快捷键表
LOCAL MMISET_SHORTCUT_ACTION_E s_widget_shortcut_arr[] =
{
    MMISET_SHORTCUT_ACTION_OK,
    MMISET_SHORTCUT_ACTION_CANCEL,
    MMISET_SHORTCUT_ACTION_UP,  //可以自定义，不一定是wap了
    MMISET_SHORTCUT_ACTION_DOWN,
    MMISET_SHORTCUT_ACTION_LEFT,
    MMISET_SHORTCUT_ACTION_RIGHT,
    MMISET_SHORTCUT_ACTION_SMART_0,
    MMISET_SHORTCUT_ACTION_SMART_1,
    MMISET_SHORTCUT_ACTION_SMART_2,
    MMISET_SHORTCUT_ACTION_SMART_3,
    MMISET_SHORTCUT_ACTION_SMART_4
};

//智能风格的快捷键表
LOCAL MMISET_SHORTCUT_ACTION_E s_smart_shortcut_arr[] =
{
    MMISET_SHORTCUT_ACTION_OK,
    MMISET_SHORTCUT_ACTION_CANCEL,
    MMISET_SHORTCUT_ACTION_SMART_0,
    MMISET_SHORTCUT_ACTION_SMART_1,
    MMISET_SHORTCUT_ACTION_SMART_2,
    MMISET_SHORTCUT_ACTION_SMART_3,
    MMISET_SHORTCUT_ACTION_SMART_4
};

//显示DCD的基本风格的快捷键表
LOCAL uint32 s_withdcd_shortcut_arr[]=
{
    MMISET_SHORTCUT_ACTION_CANCEL,
    MMISET_SHORTCUT_ACTION_DOWN
};

//不显示DCD的基本风格的快捷键表
LOCAL uint32 s_nodcd_shortcut_arr[]=
{
    MMISET_SHORTCUT_ACTION_CANCEL,
    MMISET_SHORTCUT_ACTION_DOWN,
    MMISET_SHORTCUT_ACTION_LEFT,
    MMISET_SHORTCUT_ACTION_RIGHT
};

//快捷键表汇总
LOCAL MMISET_SHORTCUT_TABLE_T s_shortcut_table[MMISET_SHORTCUT_TABLE_MAX] =
{
    {s_smart_shortcut_arr, ARR_SIZE(s_smart_shortcut_arr)},
    {s_widget_shortcut_arr, ARR_SIZE(s_widget_shortcut_arr)},        
    //{s_withdcd_shortcut_arr, ARR_SIZE(s_withdcd_shortcut_arr)},/*lint !e64*/
    {s_nodcd_shortcut_arr, ARR_SIZE(s_nodcd_shortcut_arr)}/*lint !e64*/
};/*lint !e64*/

#else   //CMCC_IDLE_DISPLAY

//widget风格的快捷键表
LOCAL MMISET_SHORTCUT_ACTION_E s_widget_shortcut_arr[] =
{
    MMISET_SHORTCUT_ACTION_LEFT,
    MMISET_SHORTCUT_ACTION_RIGHT,
    MMISET_SHORTCUT_ACTION_UP,  //可以自定义，不一定是wap了
    MMISET_SHORTCUT_ACTION_DOWN,
    MMISET_SHORTCUT_ACTION_OK,
    MMISET_SHORTCUT_ACTION_CANCEL,
    MMISET_SHORTCUT_ACTION_SMART_0,
    MMISET_SHORTCUT_ACTION_SMART_1,
    MMISET_SHORTCUT_ACTION_SMART_2,
    MMISET_SHORTCUT_ACTION_SMART_3,
    MMISET_SHORTCUT_ACTION_SMART_4
};

LOCAL MMISET_SHORTCUT_ACTION_E s_smart_shortcut_arr[] =
{
    MMISET_SHORTCUT_ACTION_OK,
    MMISET_SHORTCUT_ACTION_CANCEL,
    MMISET_SHORTCUT_ACTION_SMART_0,
    MMISET_SHORTCUT_ACTION_SMART_1,
    MMISET_SHORTCUT_ACTION_SMART_2,
    MMISET_SHORTCUT_ACTION_SMART_3,
    MMISET_SHORTCUT_ACTION_SMART_4
};

LOCAL uint32 s_withdcd_shortcut_arr[]=
{
    MMISET_SHORTCUT_ACTION_CANCEL,
    MMISET_SHORTCUT_ACTION_UP,  //可以自定义，不一定是wap了
    MMISET_SHORTCUT_ACTION_DOWN
};

LOCAL uint32 s_nodcd_shortcut_arr[]=
{
    MMISET_SHORTCUT_ACTION_CANCEL,
    MMISET_SHORTCUT_ACTION_UP,  //可以自定义，不一定是wap了
    MMISET_SHORTCUT_ACTION_DOWN,
    MMISET_SHORTCUT_ACTION_LEFT,
    MMISET_SHORTCUT_ACTION_RIGHT
};

LOCAL MMISET_SHORTCUT_TABLE_T s_shortcut_table[MMISET_SHORTCUT_TABLE_MAX] =
{
    {s_smart_shortcut_arr, ARR_SIZE(s_smart_shortcut_arr)},
    {s_widget_shortcut_arr, ARR_SIZE(s_widget_shortcut_arr)},         
    //{s_withdcd_shortcut_arr, ARR_SIZE(s_withdcd_shortcut_arr)},
    {s_nodcd_shortcut_arr, ARR_SIZE(s_nodcd_shortcut_arr)}
};

#endif  //CMCC_IDLE_DISPLAY

LOCAL const MMI_TEXT_ID_T s_shortcut_list_text_id[MMISET_SHORTCUT_MAX] = 
{
    TXT_NULL,//temp for null,later is TXT_COMMON_MAINMENU,
#ifdef TOUCH_PANEL_SUPPORT
    TXT_COMM_DIALPAD,
#endif
    TXT_ENG_TITLE,
    STXT_MAIN_CL,
    TXT_COMMUNICATION_MESSAGE,
    TXT_COMMON_WRITE_SMS,
#ifdef MMI_AUDIO_PLAYER_SUPPORT
    TXT_SET_SHORTCUT_MP3,
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
    TXT_COMMON_VIDEOPLAYER,
#endif
#endif
#ifdef FM_SUPPORT
    TXT_FM,
#endif
#ifdef  CAMERA_SUPPORT
    TXT_ENTERTAIMENT_CAMERA, 
#endif    
    //TXT_SET_SHORTCUT_RING,
    TXT_SETTINGS_ENVIONMENT,
    TXT_ALARM_CLOCK,
    //TXT_ENTERTAIMENT_GAME,
    
    TXT_EXPLORER,
#ifdef PIC_VIEWER_SUPPORT
    TXT_COMMON_PIC,
#endif
#ifdef BLUETOOTH_SUPPORT
    TXT_BLUETOOTH,
#endif
#ifdef CALENDAR_SUPPORT
    TXT_IDLE_CALENDAR,
#endif
    TXT_TOOLS_CALCULATOR,
#ifdef BROWSER_SUPPORT
    TXT_COMMUNICATION_BROWSER,
#endif
#ifdef EBOOK_SUPPORT
    TXT_COMM_EBOOK,
#endif
    TXT_INBOX,
#ifdef JAVA_SUPPORT
    TXT_JAVA,
#endif
#ifdef MMI_RECORD_SUPPORT
    TXT_RECORDER,
#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE  
#if defined(MMI_MULTI_SIM_SYS_DUAL)
    TXT_SET_DUAL_SIM_SETTING,
#else
    TXT_SET_MULTI_SIM_SETTING,
#endif
#endif
#ifdef CMMB_SUPPORT
    TXT_MTV_MOBILETV,
#endif
#ifdef MCARE_V31_SUPPORT
	TXT_TMC_APP,			    //MCARE	
	TXT_BRW_APP,			    //QBRW		
#endif
#ifdef SEARCH_SUPPORT
    TXT_SEARCH,
#endif
#ifdef MMI_WIFI_SUPPORT
    TXT_WIFI,
#endif
#ifdef ASP_SUPPORT
    TXT_MMIASP_ANSWER_PHONE,
    TXT_MMIASP_AUTOPLAY_MSGS,
#endif
#ifndef LOW_MEMORY_SUPPORT_16X16
#ifdef ATV_SUPPORT
    TXT_ATV_ANALOGTV,
#endif
#endif
};

#ifdef MMI_COMMON_IDLE_IMG_SOFTKEY_SUPPORT
LOCAL const MMI_IMAGE_ID_T s_shortcut_list_img_id[MMISET_SHORTCUT_MAX] =
{
	IMAGE_IDLE_SHORTCUT_MAINMENU,
#ifdef TOUCH_PANEL_SUPPORT
	IMAGE_IDLE_SHORTCUT_DIAL,
#endif
	IMAGE_IDLE_SHORTCUT_PB,
	IMAGE_IDLE_SHORTCUT_CL,
	IMAGE_IDLE_SHORTCUT_MESSAGE,
	IMAGE_IDLE_SHORTCUT_WRITE_MSG,
#ifdef MMI_AUDIO_PLAYER_SUPPORT
	IMAGE_IDLE_SHORTCUT_MP3,
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
	IMAGE_IDLE_SHORTCUT_MP4,
#endif
#endif
#ifdef FM_SUPPORT
	IMAGE_IDLE_SHORTCUT_FM,
#endif
#ifdef CAMERA_SUPPORT
	IMAGE_IDLE_SHORTCUT_DC,
#endif
	IMAGE_IDLE_SHORTCUT_PROFILES,
	IMAGE_IDLE_SHORTCUT_ALARM,
	IMAGE_IDLE_SHORTCUT_FAVORITE,
#ifdef PIC_VIEWER_SUPPORT            
	IMAGE_IDLE_SHORTCUT_PIC,
#endif            
#ifdef BLUETOOTH_SUPPORT
	IMAGE_IDLE_SHORTCUT_BT,
#endif
#ifdef CALENDAR_SUPPORT
	IMAGE_IDLE_SHORTCUT_CALENDAR,
#endif
#ifdef CALCULATOR_SUPPORT
	IMAGE_IDLE_SHORTCUT_CALCULATOR,
#endif 
#ifdef BROWSER_SUPPORT
	IMAGE_IDLE_SHORTCUT_WEB,	
#endif                                    
#ifdef EBOOK_SUPPORT
	IMAGE_IDLE_SHORTCUT_EBOOK,
#endif
	IMAGE_IDLE_SHORTCUT_INBOX,
#ifdef JAVA_SUPPORT
	IMAGE_IDLE_SHORTCUT_JAVA,
#endif
#ifdef MMI_RECORD_SUPPORT
	IMAGE_IDLE_SHORTCUT_RECORD,
#endif  
#ifndef MMI_MULTI_SIM_SYS_SINGLE
#if defined(MMI_MULTI_SIM_SYS_DUAL)
	IMAGE_IDLE_SHORTCUT_DUALSIM,
#else
	IMAGE_IDLE_SHORTCUT_MULTISIM,
#endif
#endif
#ifdef CMCC_UI_STYLE
	IMAGE_IDLE_SHORTCUT_FETION,    //飞信
	//IMAGE_IDLE_SHORTCUT_MC,      //音乐随身听
	IMAGE_IDLE_SHORTCUT_CALENDAR,
	IMAGE_IDLE_SHORTCUT_MAP,//xuefangjiang add 20101104
#endif
#ifdef CMMB_SUPPORT
	IMAGE_IDLE_SHORTCUT_TV,
#endif
#ifdef MCARE_V31_SUPPORT
	IMAGE_MCARE_SHORTCUT_MCARE,	    //MCARE
	IMAGE_MCARE_SHORTCUT_QB,	    //QBRW		
#endif                            
#ifdef SEARCH_SUPPORT
	IMAGE_IDLE_SHORTCUT_SEARCH,
#endif
#ifdef MMI_WIFI_SUPPORT
	IMAGE_IDLE_SHORTCUT_WIFI,
#endif
#ifdef ASP_SUPPORT
	IMAGE_IDLE_SHORTCUT_ASP,
	IMAGE_IDLE_SHORTCUT_AUTOPLAY,
#endif
#ifdef ATV_SUPPORT
	IMAGE_IDLE_SHORTCUT_ATV,
#endif
};
#endif
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef void(*SHORTCUT_FUNC)(void);
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set idle shortcut 
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void SetShortcutInfo(
                           uint16          action_index,
                           uint16          selected
                           );

/*****************************************************************************/
//  Description : get current shortcut table type
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL MMISET_SHORTCUT_TABLE_TYPE_E GetCurShortcutTableType(void);

/*****************************************************************************/
//  Description : get shortcut action type and offset
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void GetShorcutActionTypeAndOffset(
                                         uint32 action_index,
                                         uint32 *action_type_ptr,
                                         uint32 *action_offset_ptr
                                         );

/*****************************************************************************/
//  Description : get enable shortcut item by action index
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL uint32 GetEnableShortcutIndex(
                                    uint32 action_index
                                    );

/*****************************************************************************/
//  Description : to handle phone setting shortcut window message
//  Global resource dependence : 
//  Author:applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetShortcutWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );
/*****************************************************************************/
//  Description : 从nv得到方向键信息
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void GetDirectionKeyInfo(
                               MMISET_DIRECTIONKEY_SHORTCUT_T *direction_info_ptr
                               );
/*****************************************************************************/
//  Description : 填充list item
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void AppendOneLineIconTextListItem(
                                         MMI_CTRL_ID_T  ctrl_id,
                                         MMI_IMAGE_ID_T icon_id,
                                         MMI_TEXT_ID_T  text_id
                                         );
/*****************************************************************************/
//  Description : To get enabled function key shortcut info 
//  Global resource dependence : 
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void GetEnabledFuncKeyShortcutInfo(
                                         MMISET_SHORTCUT_TYPE_E  *funckey_info_ptr
                                         );
/*****************************************************************************/
//  Description : 恢复默认值
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void ResetDirectionKeyInfo(
                                 MMISET_DIRECTIONKEY_SHORTCUT_T *direction_info_ptr
                                 );
/*****************************************************************************/
//  Description : 从nv得到方向键信息
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMISET_SHORTCUT_TYPE_E GetDirectionKey(
                             MMISET_DIRKEY_E index
                             );
/*****************************************************************************/
//  Description : 从nv得到功能键信息
//  Global resource dependence : 
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL MMISET_SHORTCUT_TYPE_E GetFunctionKey(
                                            MMISET_FUNCKEY_E index
                                            );
/*****************************************************************************/
//  Description : 设置方向键信息到nv
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void SetDirectionKey(
                           uint16 index,
                           uint16 selected
                           );
/*****************************************************************************/
//  Description : Set function key shortcut info to nv
//  Global resource dependence : 
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void SetFuncKeyShortcutInfo(
                                  uint16 index,
                                  uint16 selected
                                  );
#if 0
/*****************************************************************************/
//  Description : to handle direction key sub window
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleShortcutSubWindow(
                                                MMI_WIN_ID_T    win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param
                                                );
#endif
/*****************************************************************************/
//  Description : to open selected shortcut win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void OpenSelectedShortcutWin(
                                   MMI_WIN_ID_T             win_id,
                                   MMISET_SHORTCUT_TYPE_E   selected    
                                   );
/*****************************************************************************/
//  Description : 恢复默认值
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void ResetIdleShortcutInfo(
                                 MMISET_IDLE_SHORTCUT_T *idle_shortcut
                                 );
/*****************************************************************************/
//  Description : 从nv得到idle快捷键信息
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void GetIdleShortcutInfo(
                               MMISET_IDLE_SHORTCUT_T *idle_shortcut_ptr
                               );
/*****************************************************************************/
//  Description : set idle shortcut 
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetIdleShortcutInfo(
                              uint16          index,
                              uint16          selected
                              );


LOCAL void EnterMessageMainMenuWin( void);
LOCAL void EnterAspMainMenuWin( void);
LOCAL void EnterPBMainMenuWin( void);
LOCAL void EnterWritingNewMessage( void);
LOCAL void EnterProfilesSettingWin( void);
LOCAL void EnterWebWin( void);
LOCAL void EnterSmsInboxWin( void);
LOCAL void EnterMp3PlayMenu( void);
LOCAL void EnterAlarmMenu( void);    
#ifdef ASP_SUPPORT
LOCAL void EnterAutoPlayAspMessages( void);
#endif

/*****************************************************************************/
//  Description : set shortcut setlist ctrl param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetShortcutSetlistParam(MMI_WIN_ID_T    win_id);
/*****************************************************************************/
//  Description :设置所有setlist控件的索引值
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetAllShortcutindex(MMI_CTRL_ID_T ctrl_id, uint16 index);

/*****************************************************************************/
//  Description :保存所有setlist的设定值
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SaveAllShortcutSettings(void);

/*****************************************************************************/
//  Description : enter record main win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void EnterRecordMainWin(void);
/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
// the window for phone setting shortcut
WINDOW_TABLE( MMISET_SET_SHORTCUT_WIN_TAB ) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleSetShortcutWindow ),    
        WIN_ID( MMISET_SET_SHORTCUT_WIN_ID ),
        WIN_TITLE(TXT_COMMON_SPEEDKEY_SET),
        //CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_SHORTCUT_LIST_CTRL_ID),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_SET_SHORTCUT_FORM_CTRL_ID),
            CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMISET_SET_SHORTCUT_SL_FORM_CTRL_ID, MMISET_SET_SHORTCUT_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_SET_SL, MMISET_SET_SHORTCUT_SL_BOTTON_CTRL_ID, MMISET_SET_SHORTCUT_SL_FORM_CTRL_ID),
                CHILD_SETLIST_CTRL(TRUE, MMISET_SET_SHORTCUT_SL_CTRL_ID, MMISET_SET_SHORTCUT_SL_FORM_CTRL_ID),

            CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMISET_SET_SHORTCUT_SR_FORM_CTRL_ID, MMISET_SET_SHORTCUT_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_SET_SR, MMISET_SET_SHORTCUT_SR_BOTTON_CTRL_ID, MMISET_SET_SHORTCUT_SR_FORM_CTRL_ID),
                CHILD_SETLIST_CTRL(TRUE, MMISET_SET_SHORTCUT_SR_CTRL_ID, MMISET_SET_SHORTCUT_SR_FORM_CTRL_ID),

            CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMISET_SET_SHORTCUT_DIRKEY_UP_FORM_CTRL_ID, MMISET_SET_SHORTCUT_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_SET_DIRKEY_UP, MMISET_SET_SHORTCUT_DIRKEY_UP_BOTTON_CTRL_ID, MMISET_SET_SHORTCUT_DIRKEY_UP_FORM_CTRL_ID),
                CHILD_SETLIST_CTRL(TRUE, MMISET_SET_SHORTCUT_DIRKEY_UP_CTRL_ID, MMISET_SET_SHORTCUT_DIRKEY_UP_FORM_CTRL_ID),

            CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMISET_SET_SHORTCUT_DIRKEY_DOWN_FORM_CTRL_ID, MMISET_SET_SHORTCUT_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_SET_DIRKEY_DOWN, MMISET_SET_SHORTCUT_DIRKEY_DOWN_BOTTON_CTRL_ID, MMISET_SET_SHORTCUT_DIRKEY_DOWN_FORM_CTRL_ID),
                CHILD_SETLIST_CTRL(TRUE, MMISET_SET_SHORTCUT_DIRKEY_DOWN_CTRL_ID, MMISET_SET_SHORTCUT_DIRKEY_DOWN_FORM_CTRL_ID),

            CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMISET_SET_SHORTCUT_DIRKEY_LEFT_FORM_CTRL_ID, MMISET_SET_SHORTCUT_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_SET_DIRKEY_LEFT, MMISET_SET_SHORTCUT_DIRKEY_LEFT_BOTTON_CTRL_ID, MMISET_SET_SHORTCUT_DIRKEY_LEFT_FORM_CTRL_ID),
                CHILD_SETLIST_CTRL(TRUE, MMISET_SET_SHORTCUT_DIRKEY_LEFT_CTRL_ID, MMISET_SET_SHORTCUT_DIRKEY_LEFT_FORM_CTRL_ID),

            CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMISET_SET_SHORTCUT_DIRKEY_RIGHT_FORM_CTRL_ID, MMISET_SET_SHORTCUT_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_SET_DIRKEY_RIGHT, MMISET_SET_SHORTCUT_DIRKEY_RIGHT_BOTTON_CTRL_ID, MMISET_SET_SHORTCUT_DIRKEY_RIGHT_FORM_CTRL_ID),
                CHILD_SETLIST_CTRL(TRUE, MMISET_SET_SHORTCUT_DIRKEY_RIGHT_CTRL_ID, MMISET_SET_SHORTCUT_DIRKEY_RIGHT_FORM_CTRL_ID),

            CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMISET_SET_IDLE_SHORTCUT1_FORM_CTRL_ID, MMISET_SET_SHORTCUT_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_SET_IDLE_SHORTCUT1, MMISET_SET_IDLE_SHORTCUT1_BOTTON_CTRL_ID, MMISET_SET_IDLE_SHORTCUT1_FORM_CTRL_ID),
                CHILD_SETLIST_CTRL(TRUE, MMISET_SET_IDLE_SHORTCUT1_CTRL_ID, MMISET_SET_IDLE_SHORTCUT1_FORM_CTRL_ID),

            CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMISET_SET_IDLE_SHORTCUT2_FORM_CTRL_ID, MMISET_SET_SHORTCUT_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_SET_IDLE_SHORTCUT2, MMISET_SET_IDLE_SHORTCUT2_BOTTON_CTRL_ID, MMISET_SET_IDLE_SHORTCUT2_FORM_CTRL_ID),
                CHILD_SETLIST_CTRL(TRUE, MMISET_SET_IDLE_SHORTCUT2_CTRL_ID, MMISET_SET_IDLE_SHORTCUT2_FORM_CTRL_ID),

            CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMISET_SET_IDLE_SHORTCUT3_FORM_CTRL_ID, MMISET_SET_SHORTCUT_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_SET_IDLE_SHORTCUT3, MMISET_SET_IDLE_SHORTCUT3_BOTTON_CTRL_ID, MMISET_SET_IDLE_SHORTCUT3_FORM_CTRL_ID),
                CHILD_SETLIST_CTRL(TRUE, MMISET_SET_IDLE_SHORTCUT3_CTRL_ID, MMISET_SET_IDLE_SHORTCUT3_FORM_CTRL_ID),

            CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMISET_SET_IDLE_SHORTCUT4_FORM_CTRL_ID, MMISET_SET_SHORTCUT_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_SET_IDLE_SHORTCUT4, MMISET_SET_IDLE_SHORTCUT4_BOTTON_CTRL_ID, MMISET_SET_IDLE_SHORTCUT4_FORM_CTRL_ID),
                CHILD_SETLIST_CTRL(TRUE, MMISET_SET_IDLE_SHORTCUT4_CTRL_ID, MMISET_SET_IDLE_SHORTCUT4_FORM_CTRL_ID),

            CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMISET_SET_IDLE_SHORTCUT5_FORM_CTRL_ID, MMISET_SET_SHORTCUT_FORM_CTRL_ID),
                CHILD_BUTTON_CTRL(FALSE, IMAGE_SET_IDLE_SHORTCUT5, MMISET_SET_IDLE_SHORTCUT5_BOTTON_CTRL_ID, MMISET_SET_IDLE_SHORTCUT5_FORM_CTRL_ID),
                CHILD_SETLIST_CTRL(TRUE, MMISET_SET_IDLE_SHORTCUT5_CTRL_ID, MMISET_SET_IDLE_SHORTCUT5_FORM_CTRL_ID),

        WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),     
        END_WIN
};

#if 0
//方向键设置二级菜单
WINDOW_TABLE( MMISET_SHORTCUT_SUB_WIN_TAB ) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC( (uint32)HandleShortcutSubWindow ),    
        WIN_ID( MMISET_SHORTCUT_SUB_WIN_ID ),
        WIN_TITLE(TXT_NULL),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SHORTCUT_SUB_LIST_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get idle shortcut
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMISET_IDLE_SHORTCUT_T MMIAPISET_GetIdleShortcut(void)
{
    MMISET_IDLE_SHORTCUT_T idle_shortcut = {0};/*lint !e64*/
    GetIdleShortcutInfo(&idle_shortcut);
    return (idle_shortcut);
}

/*****************************************************************************/
//  Description : get current shortcut table type
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL MMISET_SHORTCUT_TABLE_TYPE_E GetCurShortcutTableType(void)
{
    MMISET_SHORTCUT_TABLE_TYPE_E table_type = MMISET_SHORTCUT_TABLE_SMART;

    if (MMISET_IDLE_STYLE_SMART == MMIAPISET_GetIdleStyle())
    {
        table_type = MMISET_SHORTCUT_TABLE_SMART;
    }
    else
    #if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
        if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())
        {
            table_type = MMISET_SHORTCUT_TABLE_WIDGET;          
        }
        else
    #endif
    {
        /*if (MMIAPIDCD_DisplayIsValid())
        {
            table_type = MMISET_SHORTCUT_TABLE_WITHDCD;
        }
        else
        {
            table_type = MMISET_SHORTCUT_TABLE_NODCD;
        }*/
        if(MMISET_IDLE_STYLE_COMMON == MMIAPISET_GetIdleStyle())
        {
            table_type = MMISET_SHORTCUT_TABLE_NORMAL;
        }
    }

    return table_type;
}

/*****************************************************************************/
//  Description : to handle phone setting shortcut window message
//  Global resource dependence : 
//  Author:applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetShortcutWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
   
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;    
    SET_CONTROL_ID_E            set_ctrl_id   = 0;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW: 
        for(set_ctrl_id = MMISET_SET_SHORTCUT_SL_FORM_CTRL_ID; set_ctrl_id <= MMISET_SET_IDLE_SHORTCUT5_FORM_CTRL_ID; set_ctrl_id++)
        {
            GUIFORM_SetStyle(set_ctrl_id,GUIFORM_STYLE_UNIT);
        }
        SetShortcutSetlistParam(win_id);
        MMK_SetAtvCtrl(win_id, MMISET_SET_SHORTCUT_FORM_CTRL_ID);//maryxiao cr229232        
        break;
            
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        SaveAllShortcutSettings();
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_RED:
        SaveAllShortcutSettings();
        recode = MMI_RESULT_FALSE;
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;  
}
/*****************************************************************************/
//  Description : 填充list item
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void AppendOneLineIconTextListItem(
                                         MMI_CTRL_ID_T  ctrl_id,
                                         MMI_IMAGE_ID_T icon_id,
                                         MMI_TEXT_ID_T  text_id
                                         )
{
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;

    item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = icon_id;
    
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = text_id;


    GUILIST_AppendItem( ctrl_id, &item_t );
}
#if 0
/*****************************************************************************/
//  Description : to handle direction key sub window
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleShortcutSubWindow(
                                                MMI_WIN_ID_T    win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param
                                                )
{
    uint32              i = 0;
    uint16              index = 0;
    uint16              selected = 0;   
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    SET_MMI_TEXT_ID_E   text_id = TXT_SET_DIRECTIONKEY_UP;
    // MMISET_IDLE_SHORTCUT_T  idle_shortcut = {0};/*lint !e64*/
    // MMISET_IDLE_STYLE_E         idle_style = MMISET_IDLE_STYLE_SMART;
    // MMISET_SHORTCUT_TYPE_E      fk_info_arr[MMISET_FUNCKEY_MAX] = {0,0};/*lint !e64*/
    MMISET_SHORTCUT_TABLE_TYPE_E cur_table_type = MMISET_SHORTCUT_TABLE_SMART;
    MMISET_SHORTCUT_ACTION_E action_index = MMISET_SHORTCUT_ACTION_LEFT;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SHORTCUT_SUB_LIST_CTRL_ID, MMISET_SHORTCUT_MAX, FALSE );//max item 20
        for (i = 0; i < MMISET_SHORTCUT_MAX; i++)
        {
            MMIAPISET_AppendListItemByTextId( s_shortcut_list_text_id[i], TXT_COMMON_OK, MMISET_SHORTCUT_SUB_LIST_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO );
        }
        index = GUILIST_GetCurItemIndex(MMISET_SET_SHORTCUT_LIST_CTRL_ID);
        cur_table_type = GetCurShortcutTableType();
        action_index = s_shortcut_table[cur_table_type].table_arr[index];
        selected = GetEnableShortcutIndex(action_index);
        text_id = s_mmiset_shortcut_title[action_index];/*lint !e64*/
        GUILIST_SetSelectedItem(MMISET_SHORTCUT_SUB_LIST_CTRL_ID, selected, TRUE);
        GUILIST_SetCurItemIndex(MMISET_SHORTCUT_SUB_LIST_CTRL_ID, selected);
        GUILIST_SetTopItemIndex(MMISET_SHORTCUT_SUB_LIST_CTRL_ID,selected);
        MMK_SetAtvCtrl(win_id, MMISET_SHORTCUT_SUB_LIST_CTRL_ID);       
        
        GUIWIN_SetTitleTextId(win_id, text_id, FALSE);
    
        break;

        case MSG_CTL_MIDSK:
     case MSG_APP_OK:
     case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
        index = GUILIST_GetCurItemIndex(MMISET_SET_SHORTCUT_LIST_CTRL_ID);
        selected = GUILIST_GetCurItemIndex(MMISET_SHORTCUT_SUB_LIST_CTRL_ID);
        cur_table_type = GetCurShortcutTableType();
        action_index = s_shortcut_table[cur_table_type].table_arr[index];
        SetShortcutInfo(action_index, selected);
        
        MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
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
    return result;
}
#endif
/*****************************************************************************/
//  Description : 恢复默认值
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void ResetDirectionKeyInfo(
                                 MMISET_DIRECTIONKEY_SHORTCUT_T *direction_info_ptr
                                 )
{
    //SCI_ASSERT(PNULL != direction_info_ptr);
    if(PNULL == direction_info_ptr)
    {
        //SCI_TRACE_LOW:"mmisetting_assert ResetDirectionKeyInfo"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SHORTCUT_766_112_2_18_2_52_10_331,(uint8*)"");
        return;
    } 
    direction_info_ptr->direction_shortcut[MMISET_DIRKEY_LEFT]  = MMISET_SHORTCUT_SMS; //写SMS
    direction_info_ptr->direction_shortcut[MMISET_DIRKEY_RIGHT] = MMISET_SHORTCUT_PROFILE;      //情景模式
#ifdef MMI_AUDIO_PLAYER_SUPPORT
    direction_info_ptr->direction_shortcut[MMISET_DIRKEY_UP]    = MMISET_SHORTCUT_MP3;      //MP3
#else
    direction_info_ptr->direction_shortcut[MMISET_DIRKEY_UP]    = MMISET_SHORTCUT_PB;
#endif
#ifndef MMI_PHONESET_MINI_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE  
    direction_info_ptr->direction_shortcut[MMISET_DIRKEY_DOWN]  = MMISET_SHORTCUT_DUALSIM_SETTING;      //双卡双带设置
#else
    direction_info_ptr->direction_shortcut[MMISET_DIRKEY_DOWN]  = MMISET_SHORTCUT_CL;//cr227486 maryxiao
#endif
#else
    direction_info_ptr->direction_shortcut[MMISET_DIRKEY_DOWN]  = MMISET_SHORTCUT_ALARM;//yongsheng.wang
#endif
}
/*****************************************************************************/
//  Description : 从nv得到方向键信息
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void GetDirectionKeyInfo(
                               MMISET_DIRECTIONKEY_SHORTCUT_T *direction_info_ptr
                               )
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    //SCI_ASSERT(PNULL != direction_info_ptr);
    if(PNULL == direction_info_ptr)
    {
        //SCI_TRACE_LOW:"mmisetting_assert GetDirectionKeyInfo"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SHORTCUT_795_112_2_18_2_52_10_332,(uint8*)"");
        return;
    } 
    MMINV_READ(MMINV_DIRECTIONKEY_INFO,direction_info_ptr,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //默认值
        ResetDirectionKeyInfo(direction_info_ptr);
        MMINV_WRITE(MMINV_DIRECTIONKEY_INFO,direction_info_ptr);
    }    
}
/*****************************************************************************/
//  Description : 从nv得到方向键信息
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMISET_SHORTCUT_TYPE_E GetDirectionKey(
                             MMISET_DIRKEY_E index
                             )
{
    MMISET_DIRECTIONKEY_SHORTCUT_T   direction_info = {0};/*lint !e64*/
    MN_RETURN_RESULT_E          return_value = MN_RETURN_FAILURE;

   // SCI_ASSERT(MMISET_DIRKEY_MAX > index);
    if(MMISET_DIRKEY_MAX <= index)
    {
        //SCI_TRACE_LOW:"mmisetting_assert GetDirectionKey index %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SHORTCUT_820_112_2_18_2_52_10_333,(uint8*)"d", index);
        return MMISET_SHORTCUT_PB;
    } 
    MMINV_READ(MMINV_DIRECTIONKEY_INFO,&direction_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //默认值
        ResetDirectionKeyInfo(&direction_info);
        MMINV_WRITE(MMINV_DIRECTIONKEY_INFO,&direction_info);
    }    
    return (direction_info.direction_shortcut[index]);
}
/*****************************************************************************/
//  Description : 设置方向键信息到nv
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void SetDirectionKey(
                           uint16 index,
                           uint16 selected
                           )
{
    MMISET_DIRECTIONKEY_SHORTCUT_T   direction_info = {0};/*lint !e64*/
    MN_RETURN_RESULT_E          return_value = MN_RETURN_FAILURE;

    //SCI_ASSERT(MMISET_DIRKEY_MAX > index);
    if(MMISET_DIRKEY_MAX <= index)
    {
        //SCI_TRACE_LOW:"mmisetting_assert SetDirectionKey index %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SHORTCUT_847_112_2_18_2_52_10_334,(uint8*)"d", index);
        return;
    } 
    MMINV_READ(MMINV_DIRECTIONKEY_INFO,&direction_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //默认值
        ResetDirectionKeyInfo(&direction_info);
    }    
    direction_info.direction_shortcut[index] = selected;/*lint !e64*/
    MMINV_WRITE(MMINV_DIRECTIONKEY_INFO,&direction_info);
}
/*****************************************************************************/
//  Description : set idle shortcut 
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void SetIdleShortcutInfo(
                              uint16          index,
                              uint16          selected
                              )
{    
    MMISET_IDLE_SHORTCUT_T   idle_shortcut = {0};/*lint !e64*/
    MN_RETURN_RESULT_E          return_value = MN_RETURN_FAILURE;

    // SCI_ASSERT(MMISET_IDLE_SHORTCUT_MAX_NUM > index);
    if(MMISET_IDLE_SHORTCUT_MAX_NUM <= index)
    {
        //SCI_TRACE_LOW:"mmisetting_assert SetIdleShortcutInfo index %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SHORTCUT_874_112_2_18_2_52_10_335,(uint8*)"d", index);
        return;
    } 

    MMINV_READ(MMINV_SET_IDLE_SHORTCUT,&idle_shortcut,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //默认值
        ResetIdleShortcutInfo(&idle_shortcut);
    }    
    idle_shortcut.idle_shortcut[index] = selected;/*lint !e64*/
    MMINV_WRITE(MMINV_SET_IDLE_SHORTCUT,&idle_shortcut);   
    
}

/*****************************************************************************/
//  Description : set idle shortcut 
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void SetShortcutInfo(
                           uint16          action_index,
                           uint16          selected
                           )
{
    uint32 action_type = 0;
    uint32 action_offset = 0;

    GetShorcutActionTypeAndOffset(action_index, &action_type, &action_offset);
    switch (action_type)
    {
    case MMISET_SHORTCUT_ACTION_TYPE_SMART:
        SetIdleShortcutInfo(action_offset, selected);
        break;
        
    case MMISET_SHORTCUT_ACTION_TYPE_DIRKEY:
        SetDirectionKey(action_offset, selected);
        break;
        
    case MMISET_SHORTCUT_ACTION_TYPE_FUNCKEY:
        SetFuncKeyShortcutInfo(action_offset, selected);
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : 恢复默认值
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void ResetIdleShortcutInfo(
                                 MMISET_IDLE_SHORTCUT_T *idle_shortcut_ptr
                                 )
{
    // SCI_ASSERT(PNULL != idle_shortcut_ptr);
    if(PNULL == idle_shortcut_ptr)
    {
        //SCI_TRACE_LOW:"mmisetting_assert ResetIdleShortcutInfo"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SHORTCUT_932_112_2_18_2_52_10_336,(uint8*)"");
        return;
    } 
#ifdef CMCC_UI_STYLE
    idle_shortcut_ptr->idle_shortcut[0] = MMISET_SHORTCUT_MESSAGE;
#ifdef TOUCH_PANEL_SUPPORT
    idle_shortcut_ptr->idle_shortcut[1] = MMISET_SHORTCUT_DIAL;
#else
    idle_shortcut_ptr->idle_shortcut[1] = MMISET_SHORTCUT_MAP;
#endif
    idle_shortcut_ptr->idle_shortcut[2] = MMISET_SHORTCUT_ALARM;
    idle_shortcut_ptr->idle_shortcut[3] = MMISET_SHORTCUT_FETION;
#if (MMISET_IDLE_SHORTCUT_MAX_NUM > 4)
#ifdef BROWSER_SUPPORT
    idle_shortcut_ptr->idle_shortcut[4] = MMISET_SHORTCUT_WEB;
//#else
//    idle_shortcut_ptr->idle_shortcut[4] = MMISET_SHORTCUT_MP3;
#endif
#endif
#else
    idle_shortcut_ptr->idle_shortcut[0] = MMISET_SHORTCUT_SMS;
#ifdef BROWSER_SUPPORT
    idle_shortcut_ptr->idle_shortcut[1] = MMISET_SHORTCUT_WEB;
#elif (defined CAMERA_SUPPORT) //arvin zhang added for bugzilla 2945
    idle_shortcut_ptr->idle_shortcut[1] = MMISET_SHORTCUT_CAMERA;
#else 
    idle_shortcut_ptr->idle_shortcut[1] = MMISET_SHORTCUT_CL;
#endif
#ifdef MCARE_V31_SUPPORT
    idle_shortcut_ptr->idle_shortcut[2] =MMISET_SHORTCUT_MCARE;		    //MCARE	
    idle_shortcut_ptr->idle_shortcut[3] = MMISET_SHORTCUT_QBRW;		    //QBRW		
#else
    idle_shortcut_ptr->idle_shortcut[2] = MMISET_SHORTCUT_ALARM;
    idle_shortcut_ptr->idle_shortcut[3] = MMISET_SHORTCUT_PROFILE;
#endif
#if (MMISET_IDLE_SHORTCUT_MAX_NUM > 4)
#ifdef BLUETOOTH_SUPPORT
    idle_shortcut_ptr->idle_shortcut[4] = MMISET_SHORTCUT_BT;
#else
   idle_shortcut_ptr->idle_shortcut[4] = MMISET_SHORTCUT_PB;
#endif 
#endif
#endif 

    
}
/*****************************************************************************/
//  Description : 从nv得到idle快捷键信息
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void GetIdleShortcutInfo(
                               MMISET_IDLE_SHORTCUT_T *idle_shortcut_ptr
                               )
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    //SCI_ASSERT(PNULL != idle_shortcut_ptr);
    if(PNULL == idle_shortcut_ptr)
    {
        //SCI_TRACE_LOW:"mmisetting_assert GetIdleShortcutInfo"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SHORTCUT_986_112_2_18_2_52_10_337,(uint8*)"");
        return;
    } 
    MMINV_READ(MMINV_SET_IDLE_SHORTCUT,idle_shortcut_ptr,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //默认值
        ResetIdleShortcutInfo(idle_shortcut_ptr);
        MMINV_WRITE(MMINV_SET_IDLE_SHORTCUT,idle_shortcut_ptr);
    }    
}

/*****************************************************************************/
//  Description : To get enabled function key shortcut info 
//  Global resource dependence : 
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void GetEnabledFuncKeyShortcutInfo(
                                         MMISET_SHORTCUT_TYPE_E  *funckey_info_ptr
                                         )
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
#ifdef CMCC_UI_STYLE
    MMISET_SHORTCUT_TYPE_E  fk_info[MMISET_FUNCKEY_MAX] = {MMISET_SHORTCUT_MONERNET, MMISET_SHORTCUT_MAINMENU};
#else
    MMISET_SHORTCUT_TYPE_E  fk_info[MMISET_FUNCKEY_MAX] = {MMISET_SHORTCUT_MAINMENU,MMISET_SHORTCUT_PB};
#endif    
    // SCI_ASSERT( funckey_info_ptr != PNULL);
    if( PNULL == funckey_info_ptr)
    {
        //SCI_TRACE_LOW:"mmisetting_assert GetEnabledFuncKeyShortcutInfo"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SHORTCUT_1015_112_2_18_2_52_10_338,(uint8*)"");
        return;
    } 
    //read funtkey shortcut settings info from NV
    MMINV_READ(MMINV_SET_FUNCKEY_SHORTCUT_INFO, funckey_info_ptr, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_FUNCKEY_SHORTCUT_INFO, fk_info);
        MMI_MEMCPY(funckey_info_ptr, MMISET_FUNCKEY_MAX*sizeof(MMISET_SHORTCUT_TYPE_E),
            fk_info,MMISET_FUNCKEY_MAX*sizeof(MMISET_SHORTCUT_TYPE_E),
            MMISET_FUNCKEY_MAX*sizeof(MMISET_SHORTCUT_TYPE_E));
    }
}
/*****************************************************************************/
//  Description : Set function key shortcut info to nv
//  Global resource dependence : 
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void SetFuncKeyShortcutInfo(
                                  uint16 index,
                                  uint16 selected
                                  )
{
    MMISET_SHORTCUT_TYPE_E  fk_info_arr[MMISET_FUNCKEY_MAX] = {MMISET_SHORTCUT_START,MMISET_SHORTCUT_START};
    MMISET_SHORTCUT_TYPE_E  type    = MMISET_SHORTCUT_START;
    
    type = (MMISET_SHORTCUT_TYPE_E)selected;

    
    //SCI_ASSERT(MMISET_FUNCKEY_MAX > index);
    //SCI_ASSERT(MMISET_SHORTCUT_MAX > type);
    if((MMISET_FUNCKEY_MAX <= index) || MMISET_SHORTCUT_MAX <=type)
    {
        //SCI_TRACE_LOW:"mmisetting_assert SetFuncKeyShortcutInfo index %d, type %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SHORTCUT_1048_112_2_18_2_52_10_339,(uint8*)"dd",index, type);
        return;
    } 
    
    GetEnabledFuncKeyShortcutInfo(fk_info_arr); 
    fk_info_arr[index] = type;
    
    MMINV_WRITE(MMINV_SET_FUNCKEY_SHORTCUT_INFO, fk_info_arr);
}
/*****************************************************************************/
//  Description : 从nv得到功能键信息
//  Global resource dependence : 
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL MMISET_SHORTCUT_TYPE_E GetFunctionKey(
                                            MMISET_FUNCKEY_E index
                                            )
{
    MMISET_SHORTCUT_TYPE_E  funckey_info_arr[MMISET_FUNCKEY_MAX] = {MMISET_SHORTCUT_START,MMISET_SHORTCUT_START};

    //SCI_ASSERT(MMISET_FUNCKEY_MAX > index);
    if(MMISET_FUNCKEY_MAX <= index)
    {
        //SCI_TRACE_LOW:"mmisetting_assert GetFunctionKey index %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SHORTCUT_1070_112_2_18_2_52_10_340,(uint8*)"d", index);
        return MMISET_SHORTCUT_PB;
    }
    GetEnabledFuncKeyShortcutInfo(funckey_info_arr);

    return (funckey_info_arr[index]);
}
/*****************************************************************************/
//  Description : to open direction key win
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenDirectionKeyWin(
                                       MMI_WIN_ID_T    win_id,
                                       MMISET_DIRKEY_E index
                                       )
{
    MMISET_SHORTCUT_TYPE_E  selected    =   MMISET_SHORTCUT_START;
                                                                
    selected = (MMISET_SHORTCUT_TYPE_E)GetDirectionKey(index);

    if((index == MMISET_DIRKEY_LEFT) || (index == MMISET_DIRKEY_RIGHT) )
    {
        if(MMIAPIDCD_IsIdleDisplay())
        {
            //SCI_TRACE_LOW:"MMIAPISET_OpenDirectionKeyWin, dcd is open and skip the key msg!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SHORTCUT_1094_112_2_18_2_52_10_341,(uint8*)"");
            //MMIDLE_RestartSsTimer(win_id);
            return;
        }
    }
	
    //SCI_TRACE_LOW:"MMIAPISET_OpenDirectionKeyWin selected = %d, index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SHORTCUT_1100_112_2_18_2_52_10_342,(uint8*)"dd", selected,index);
	
#ifdef CMCC_IDLE_DISPLAY
    if ((MMISET_DIRKEY_UP == index)
        && (MMISET_IDLE_STYLE_COMMON == MMIAPISET_GetIdleStyle()))
    {
        //移动定制，基本风格上键固定分配给浏览器
        EnterWebWin();
        return;
    }
#endif
    OpenSelectedShortcutWin(win_id, selected);  
}

/*****************************************************************************/
//  Description : enter record main win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void EnterRecordMainWin(void)
{
	MMIAPIRECORD_OpenMainWin(PNULL);
}

/*****************************************************************************/
//  Description : to open selected shortcut win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void OpenSelectedShortcutWin(
                                   MMI_WIN_ID_T             win_id,
                                   MMISET_SHORTCUT_TYPE_E   selected    
                                   )
{
    int32  i =0;
    const SHORTCUT_FUNC func_arr[] = {
                                MMIAPIMENU_CreatMainMenu,/*lint !e64*/
#ifdef TOUCH_PANEL_SUPPORT
                                MMIAPIIDLE_EnterDialWin,
#endif
                                EnterPBMainMenuWin,
                               MMIAPICL_OpenMainMenuWindow,/*lint !e64*/
                                EnterMessageMainMenuWin,
                                EnterWritingNewMessage,
#ifdef MMI_AUDIO_PLAYER_SUPPORT
                                EnterMp3PlayMenu,
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
                                MMIAPIVP_MainEntry,
#endif
#endif
#ifdef FM_SUPPORT
                                MMIAPIFM_OpenMainWin,
#endif                                
#ifdef  CAMERA_SUPPORT
                                MMIAPIDC_OpenPhotoWin,/*lint !e64*/
#endif
                               // MMISET_EnterRingSettingMenu,
                                EnterProfilesSettingWin,
                                EnterAlarmMenu,
                                MMIAPIFMM_OpenExplorer,
#ifdef PIC_VIEWER_SUPPORT                                
                                MMIAPIPICVIEW_OpenPicViewer,
#endif
#ifdef BLUETOOTH_SUPPORT
                                MMIAPIBT_OpenMainMenuWin,
#endif
#ifdef CALENDAR_SUPPORT
                                MMIAPICALEND_OpenMainWin,
#endif
                                MMIAPICALC_OpenMainWin,
#ifdef BROWSER_SUPPORT
                                EnterWebWin,
#endif
#ifdef EBOOK_SUPPORT
                                MMIAPIEBOOK_ManagerWinMsg,  /*lint !e64 */
#endif
                                EnterSmsInboxWin,
#ifdef JAVA_SUPPORT
                                MMIAPIJAVA_OpenMainMenu,
#endif                              
#ifdef MMI_RECORD_SUPPORT
                                EnterRecordMainWin,
#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE  
                                MMISET_EnterMultiSimSettingMenu,
#endif
#ifdef CMCC_UI_STYLE
                                MMIAPISET_EnterFetionWin,
#ifdef CALENDAR_SUPPORT
                                MMIAPICALEND_OpenMainWin,//xuefangjiang add for 8800g delete music net
#endif
                                MMIAPISET_EnterMapWin,
                                MMIAPISET_EnterMonternetWin,                               
#endif
#ifdef CMMB_SUPPORT
                                MMIAPIMTV_OpenMTVPlayer,/*lint !e64 */
#endif

#ifdef MCARE_V31_SUPPORT
                                MCareV31_Main_Entry,
                        		MCareV31_QQBrowser_Shortcut,
#endif

#ifdef SEARCH_SUPPORT
                                MMIAPISEARCH_Entry,
#endif
#ifdef MMI_WIFI_SUPPORT
                                MMIAPIWIFI_OpenMainWin,
#endif
#ifdef ASP_SUPPORT
                                EnterAspMainMenuWin,
                                EnterAutoPlayAspMessages,
#endif
#ifndef LOW_MEMORY_SUPPORT_16X16
#ifdef ATV_SUPPORT
                                MMIAPIATV_OpenATV,/*lint !e64 */
#endif
#endif
                                };
                                                                


    for ( i =0; i < MMISET_SHORTCUT_MAX; i++)
    {
        if ( selected == i)
        {
            if (MMISET_SHORTCUT_SMSINBOX == selected)
            {
                func_arr[selected]();
                break;
            }
            else if (MMISET_SHORTCUT_PB == selected)
            {
                func_arr[selected]();
                break;
                             
            }
            else if (MMISET_SHORTCUT_DOCUMENT == selected)
            {
                func_arr[selected]();
                break;           
            }
 #ifdef EBOOK_SUPPORT
            else if (MMISET_SHORTCUT_EBOOK == selected)
            {   
                if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_EBOOK))
                {
                    MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_EBOOK, MMIAPIEBOOK_ValidatePrivacyAppEntry);
                }
                else
                {
                    //func_arr[selected](TRUE);   /*lint !e119 */                    
                    void (*pFunction)(BOOLEAN) = func_arr[selected];   /*lint !e64 !e119 */  
                    pFunction(TRUE);                    
                    break;
                }          
            }
#endif
#ifdef CALENDAR_SUPPORT
            else if (MMISET_SHORTCUT_CALENDAR == selected)
            {
                if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_CALENDAR))
                {
                    MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CALENDAR, func_arr[selected]);
                }
                else
                {
                    func_arr[selected]();
                    break;
                }               
            }
#endif
            else if (MMISET_SHORTCUT_MESSAGE == selected)
            {
                func_arr[selected]();
                break;
            }
#ifdef  CAMERA_SUPPORT                    
            else if (MMISET_SHORTCUT_CAMERA == selected)
            {
                func_arr[selected]();
                break;
            }
#endif            
            else
            {
                func_arr[selected]();
                break;
            }
        }
    }
}
/*****************************************************************************/
//  Description : to open function key win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenFuncKeyWin(
                                  MMI_WIN_ID_T      win_id,
                                  MMISET_FUNCKEY_E  index
                                  )
{   
    OpenSelectedShortcutWin(win_id, GetFunctionKey(index)); 
    //SCI_TRACE_LOW:"MMIAPISET_OpenFuncKeyWin "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SHORTCUT_1280_112_2_18_2_52_11_343,(uint8*)"");
}
/*****************************************************************************/
//  Description : open shortcut win
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenShortcutWin(
                                      uint16        shortcut_index, //from 0
                                      MMI_WIN_ID_T  win_id
                                      )
{
    MMISET_IDLE_SHORTCUT_T  idle_shortcut   =   {0};/*lint !e64*/

    idle_shortcut = MMIAPISET_GetIdleShortcut();

    OpenSelectedShortcutWin(win_id,idle_shortcut.idle_shortcut[shortcut_index]);    
}

/*****************************************************************************/
//  Description : to open selected shortcut win
//  Global resource dependence : 
//  Author:cheng.luo
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenSelectedShortcutWin(
                                              MMI_WIN_ID_T             win_id,
                                              MMISET_SHORTCUT_TYPE_E   selected    
                                              )
{
    OpenSelectedShortcutWin(win_id, selected);
}

/*****************************************************************************/
//  Description : 恢复默认值
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_ResetDirectionKeyInfo(void)
{
    MMISET_DIRECTIONKEY_SHORTCUT_T  direction_info  =   {0};/*lint !e64*/

    ResetDirectionKeyInfo(&direction_info);
    MMINV_WRITE(MMINV_DIRECTIONKEY_INFO,&direction_info);
}


/*****************************************************************************/
//  Description : 恢复默认值
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_ResetIdleShortcutInfo(void)
{
    MMISET_IDLE_SHORTCUT_T   idle_shortcut = {0};/*lint !e64*/

    ResetIdleShortcutInfo(&idle_shortcut);
    MMINV_WRITE(MMINV_SET_IDLE_SHORTCUT,&idle_shortcut);
}

/*****************************************************************************/
//  Description : to enter message main menu win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void EnterMessageMainMenuWin( void)
{
    MMIAPISMS_OpenMessageMainMenuWin();
}

/*****************************************************************************/
//  Description : to enter message main menu win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void EnterAspMainMenuWin( void)
{
#ifdef ASP_SUPPORT
    MMIAPIASP_EnterAsp();
#endif
}
#ifdef ASP_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void EnterAutoPlayAspMessages( void)
{
    MMIAPIASP_AutoPlayRecordMessages();
}
#endif
/*****************************************************************************/
//  Description : to enter PB main menu win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void EnterPBMainMenuWin( void)
{
    MMIAPIPB_OpenListAllGroupWin();
}
/*****************************************************************************/
//  Description : to enter writing new message win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void EnterWritingNewMessage( void)
{
    MMIAPISMS_WriteNewMessage(MN_DUAL_SYS_MAX, PNULL, PNULL, 0);
}
/*****************************************************************************/
//  Description : to enter profiles setting win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void EnterProfilesSettingWin( void)
{
    MMIENVSET_OpenMainMenuWindow();
}

/*****************************************************************************/
//  Description : to enter web win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void EnterWebWin( void)
{
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/

    entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.url_ptr = PNULL;
    entry_param.user_agent_ptr = PNULL;
    
    MMIAPIBROWSER_Entry(&entry_param);
#endif
}

/*****************************************************************************/
//  Description : to enter sms inbox win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void EnterSmsInboxWin( void)
{
    MMIAPISMS_OpenSmsBox(MMISMS_BOX_MT); 
}



/*****************************************************************************/
//  Description : to open alarm main menu
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL void EnterAlarmMenu( void)
{
    MMIAPIALM_OpenMainWin();
}

/*****************************************************************************/
//  Description : to open mp3 main play menu
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL void EnterMp3PlayMenu( void)
{
    MMIAPIMP3_OpenMp3Player();
}

/*****************************************************************************/
//  Description : to get text id of left shortcut main win
//  Global resource dependence : none
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetFSMainTextId(MMI_TEXT_ID_T * left_key_id, MMI_TEXT_ID_T * right_key_id)
{     
    #ifdef CMCC_UI_STYLE
    //MO一键上网标记在web键
    *left_key_id = TXT_COMMON_OK;
    *right_key_id = TXT_COMMON_MAINMENU;

    #elif defined(BAR_PHONE_WIDGET) 
    *left_key_id = TXT_COMMON_MAINMENU;
    *right_key_id = TXT_IDLE_BUSINESS_MODE;
    
    #else
    {
        MMISET_SHORTCUT_TYPE_E  fk_info_arr[MMISET_FUNCKEY_MAX] = {MMISET_SHORTCUT_START,MMISET_SHORTCUT_START};
        GetEnabledFuncKeyShortcutInfo(fk_info_arr); 
        if (MMISET_IDLE_STYLE_COMMON == MMIAPISET_GetIdleStyle())
        {
            //基本风格左软键硬绑定为主菜单
            *left_key_id = TXT_COMM_DIALPAD;
        }
        else
        {
            *left_key_id = s_shortcut_list_text_id[fk_info_arr[0]];
        }
        *right_key_id = s_shortcut_list_text_id[fk_info_arr[1]];
    }
    #endif
}


#ifdef MMI_COMMON_IDLE_IMG_SOFTKEY_SUPPORT
PUBLIC void MMIAPISET_GetFSMainIMGId(MMI_IMAGE_ID_T * left_key_id, MMI_IMAGE_ID_T * right_key_id)
{     
        MMISET_SHORTCUT_TYPE_E  fk_info_arr[MMISET_FUNCKEY_MAX] = {MMISET_SHORTCUT_START,MMISET_SHORTCUT_START};
        GetEnabledFuncKeyShortcutInfo(fk_info_arr); 
        if (MMISET_IDLE_STYLE_COMMON == MMIAPISET_GetIdleStyle())
        {
            //基本风格左软键硬绑定为主菜单
            *left_key_id = IMAGE_IDLE_SHORTCUT_MAINMENU;
        }
        else
        {
            *left_key_id = s_shortcut_list_img_id[fk_info_arr[0]];
        }
        *right_key_id = s_shortcut_list_img_id[fk_info_arr[1]];
}
#endif
/*****************************************************************************/
//  Description : open phone setting shortcut window
//  Global resource dependence : 
//  Author: applezhang
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_EnterSettingShortcutWin(void)
{   
    MMK_CreateWin((uint32 *)MMISET_SET_SHORTCUT_WIN_TAB, PNULL);       
}

/*****************************************************************************/
//  Description : get shortcut action type and offset
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void GetShorcutActionTypeAndOffset(
                                         uint32 action_index,
                                         uint32 *action_type_ptr,
                                         uint32 *action_offset_ptr
                                         )
{
    //SCI_ASSERT(PNULL != action_type_ptr);
    //SCI_ASSERT(PNULL != action_offset_ptr);
    if(PNULL == action_type_ptr || PNULL == action_offset_ptr)
    {
        //SCI_TRACE_LOW:"mmisetting_assert GetShorcutActionTypeAndOffset"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SHORTCUT_1491_112_2_18_2_52_11_344,(uint8*)"");
        return;
    } 
    if (action_index < MMISET_SHORTCUT_ACTION_FUNCKEY_END)
    {
        *action_type_ptr = MMISET_SHORTCUT_ACTION_TYPE_FUNCKEY;
        *action_offset_ptr = action_index;
    }
    else if (action_index < MMISET_SHORTCUT_ACTION_DIRKEY_END)
    {
        *action_type_ptr = MMISET_SHORTCUT_ACTION_TYPE_DIRKEY;
        *action_offset_ptr = (action_index-MMISET_SHORTCUT_ACTION_FUNCKEY_END);
    }
    else
    {
        *action_type_ptr = MMISET_SHORTCUT_ACTION_TYPE_SMART;
        *action_offset_ptr = (action_index-MMISET_SHORTCUT_ACTION_DIRKEY_END);
    }
}

/*****************************************************************************/
//  Description : get enable shortcut item by action index
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL uint32 GetEnableShortcutIndex(
                                    uint32 action_index
                                    )
{
    MMISET_DIRECTIONKEY_SHORTCUT_T   directionkey_info = {0};/*lint !e64*/
    MMISET_IDLE_SHORTCUT_T  idle_shortcut = {0};/*lint !e64*/
    MMISET_SHORTCUT_TYPE_E  fk_info_arr[MMISET_FUNCKEY_MAX] = {0}; /*lint !e64*/   
    uint32 index = 0;
    uint32 action_type = 0;
    uint32 action_offset = 0;

    GetShorcutActionTypeAndOffset(action_index, &action_type, &action_offset);
    switch (action_type)
    {
    case MMISET_SHORTCUT_ACTION_TYPE_SMART:
        GetIdleShortcutInfo(&idle_shortcut);
        index = idle_shortcut.idle_shortcut[action_offset];
        break;
        
    case MMISET_SHORTCUT_ACTION_TYPE_DIRKEY:
        GetDirectionKeyInfo(&directionkey_info);
        index = directionkey_info.direction_shortcut[action_offset];
        break;
        
    case MMISET_SHORTCUT_ACTION_TYPE_FUNCKEY:
        GetEnabledFuncKeyShortcutInfo(fk_info_arr);
        index = fk_info_arr[action_offset];
        break;

    default:
        break;
    }
    return index;
}
#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : to enter Fetion win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISET_EnterFetionWin( void)
{
#ifdef JAVA_SUPPORT_SUN
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return;
    }
    MMIAPIJAVA_LaunchPreloadMidlet("Fetion");//主菜单中直接打开应用接口
#endif
}

/*****************************************************************************/
//  Description : to enter Fetion win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISET_EnterMusicWin( void)
{
#ifdef JAVA_SUPPORT_SUN
    MMIAPIJAVA_LaunchPreloadMidlet("Music");//主菜单中直接打开应用接口
#endif
}
/*****************************************************************************/
//  Description : to enter web win
//  Global resource dependence : 
//  Author:hongying wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterMonternetWin( void)
{
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/

    entry_param.type = MMIBROWSER_ACCESS_URL;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.url_ptr = DM_WAP_START_PAGE;
    entry_param.user_agent_ptr = PNULL;
    entry_param.is_browser_type_customized = TRUE;
    entry_param.browser_type = MMIBROWSER_TYPE_DORADO;
    
    MMIAPIBROWSER_Entry(&entry_param);
#endif
}

/*****************************************************************************/
//  Description : to enter web win
//  Global resource dependence : 
//  Author:hongying wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterPocketHallWin( void)
{
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/

    entry_param.type = MMIBROWSER_ACCESS_URL;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.url_ptr = "http://wap.monternet.com/?cp22=v22yyt";//修改网上营业厅地址
    entry_param.user_agent_ptr = PNULL;
    
    MMIAPIBROWSER_Entry(&entry_param);
#endif
}

/*****************************************************************************/
//  Description : to enter stock win
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterStockWin( void)
{
#ifdef JAVA_SUPPORT_SUN
    MMIAPIJAVA_LaunchPreloadMidlet("Paper");//主菜单中直接打开应用接口
#endif
}

/*****************************************************************************/
//  Description : to enter Map win
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterMapWin( void)
{
#ifdef JAVA_SUPPORT_SUN
    MMIAPIJAVA_LaunchPreloadMidlet("Map");//主菜单中直接打开应用接口
#endif
}

#endif

/*****************************************************************************/
//  Description : set shortcut setlist ctrl param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetShortcutSetlistParam(MMI_WIN_ID_T    win_id)
{
    uint32  i = 0;
    uint16 loop = 0;
    MMISET_SHORTCUT_TABLE_TYPE_E cur_table_type = MMISET_SHORTCUT_TABLE_SMART;
    GUIFORM_CHILD_WIDTH_T width = {0};
    uint32 index = 0; 
    MMI_CTRL_ID_T shortcut_child_form_ctrl_id[]=
    {
        MMISET_SET_SHORTCUT_SL_FORM_CTRL_ID,
        MMISET_SET_SHORTCUT_SR_FORM_CTRL_ID,
        MMISET_SET_SHORTCUT_DIRKEY_UP_FORM_CTRL_ID,
        MMISET_SET_SHORTCUT_DIRKEY_DOWN_FORM_CTRL_ID,
        MMISET_SET_SHORTCUT_DIRKEY_LEFT_FORM_CTRL_ID,
        MMISET_SET_SHORTCUT_DIRKEY_RIGHT_FORM_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT1_FORM_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT2_FORM_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT3_FORM_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT4_FORM_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT5_FORM_CTRL_ID
    };
    MMI_CTRL_ID_T shortcut_child_button_ctrl_id[]=
    {
        MMISET_SET_SHORTCUT_SL_BOTTON_CTRL_ID,
        MMISET_SET_SHORTCUT_SR_BOTTON_CTRL_ID,
        MMISET_SET_SHORTCUT_DIRKEY_UP_BOTTON_CTRL_ID,
        MMISET_SET_SHORTCUT_DIRKEY_DOWN_BOTTON_CTRL_ID,
        MMISET_SET_SHORTCUT_DIRKEY_LEFT_BOTTON_CTRL_ID,
        MMISET_SET_SHORTCUT_DIRKEY_RIGHT_BOTTON_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT1_BOTTON_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT2_BOTTON_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT3_BOTTON_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT4_BOTTON_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT5_BOTTON_CTRL_ID,
    };
    MMI_CTRL_ID_T shortcut_child_setlist_ctrl_id[]=
    {
        MMISET_SET_SHORTCUT_SL_CTRL_ID,
        MMISET_SET_SHORTCUT_SR_CTRL_ID,
        MMISET_SET_SHORTCUT_DIRKEY_UP_CTRL_ID,
        MMISET_SET_SHORTCUT_DIRKEY_DOWN_CTRL_ID,        
        MMISET_SET_SHORTCUT_DIRKEY_LEFT_CTRL_ID,
        MMISET_SET_SHORTCUT_DIRKEY_RIGHT_CTRL_ID,        
        MMISET_SET_IDLE_SHORTCUT1_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT2_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT3_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT4_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT5_CTRL_ID    
    };
	
    GUIFORM_IsAutoDir(MMISET_SET_SHORTCUT_FORM_CTRL_ID,TRUE);

    width.type = GUIFORM_CHILD_WIDTH_FIXED;
	GUIRES_GetImgWidthHeight(&width.add_data,PNULL,IMAGE_SET_SL,win_id);
	
	width.add_data = (uint16)(width.add_data + 5);
    for (i = 0; i < MMISET_SHORTCUT_ACTION_MAX; i++)
    {
        //set button width
        GUIFORM_SetChildWidth(shortcut_child_form_ctrl_id[i], shortcut_child_button_ctrl_id[i], &width);

        //set button not handle tp
        GUIAPICTRL_SetState(shortcut_child_button_ctrl_id[i],GUICTRL_STATE_DISABLE_TP,TRUE);

        for(loop = 0; loop < MMISET_SHORTCUT_MAX;loop++)
        {
            GUISETLIST_AddItemById(shortcut_child_setlist_ctrl_id[i], s_shortcut_list_text_id[loop]);
        }
        index = GetAllShortcutindex(shortcut_child_setlist_ctrl_id[i], i);        
        GUISETLIST_SetCurIndex(shortcut_child_setlist_ctrl_id[i], index);
        GUISETLIST_SetTitleTextId(shortcut_child_setlist_ctrl_id[i],s_mmiset_shortcut_title[i]);	
        GUISETLIST_SetCtrlState(shortcut_child_setlist_ctrl_id[i], GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    }            

    cur_table_type = GetCurShortcutTableType();
    if(MMISET_SHORTCUT_TABLE_SMART == cur_table_type)
    {
    #if defined(BAR_PHONE_WIDGET) 
        for(i = MMISET_SHORTCUT_ACTION_OK; i < MMISET_SHORTCUT_ACTION_FUNCKEY_END; i++)
    #else
        for(i = MMISET_SHORTCUT_ACTION_LEFT; i < MMISET_SHORTCUT_ACTION_DIRKEY_END; i++)
    #endif        
        {
            GUIFORM_SetChildDisplay(MMISET_SET_SHORTCUT_FORM_CTRL_ID,
            shortcut_child_form_ctrl_id[i],
            GUIFORM_CHILD_DISP_HIDE);
        }
    }
    else if(MMISET_SHORTCUT_TABLE_NORMAL == cur_table_type)
    {
        GUIFORM_SetChildDisplay(MMISET_SET_SHORTCUT_FORM_CTRL_ID,
        MMISET_SET_SHORTCUT_SL_FORM_CTRL_ID,
        GUIFORM_CHILD_DISP_HIDE);
#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
	    if (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
		{
		GUIFORM_SetChildDisplay(MMISET_SET_SHORTCUT_FORM_CTRL_ID,
		MMISET_SET_SHORTCUT_SR_FORM_CTRL_ID,
        GUIFORM_CHILD_DISP_HIDE); 
		}
#endif
        for(i = MMISET_SHORTCUT_ACTION_SMART_0; i< MMISET_SHORTCUT_ACTION_MAX; i++)
        {
            GUIFORM_SetChildDisplay(MMISET_SET_SHORTCUT_FORM_CTRL_ID,
            shortcut_child_form_ctrl_id[i],
            GUIFORM_CHILD_DISP_HIDE);
        }       
#ifdef KEYPAD_TYPE_COMPACT_KEYPAD//CR251531
        for(i = MMISET_SHORTCUT_ACTION_LEFT; i < MMISET_SHORTCUT_ACTION_DIRKEY_END; i++)
        {
            GUIFORM_SetChildDisplay(MMISET_SET_SHORTCUT_FORM_CTRL_ID,
            shortcut_child_form_ctrl_id[i],
            GUIFORM_CHILD_DISP_HIDE);
        }
#endif
    }
#ifdef KEYPAD_TYPE_COMPACT_KEYPAD//CR251531
    else if(MMISET_SHORTCUT_TABLE_WIDGET == cur_table_type)
    {
        for(i = MMISET_SHORTCUT_ACTION_LEFT; i < MMISET_SHORTCUT_ACTION_DIRKEY_END; i++)
        {
            GUIFORM_SetChildDisplay(MMISET_SET_SHORTCUT_FORM_CTRL_ID,
            shortcut_child_form_ctrl_id[i],
            GUIFORM_CHILD_DISP_HIDE);
        }
    }
#endif
}
/*****************************************************************************/
//  Description :设置所有setlist控件的索引值
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetAllShortcutindex(MMI_CTRL_ID_T ctrl_id, uint16 index)
{
    MMISET_SHORTCUT_TYPE_E  funckey_info[MMISET_FUNCKEY_MAX] = {0};
    MMISET_DIRECTIONKEY_SHORTCUT_T  direction_info = {0};
    MMISET_IDLE_SHORTCUT_T idle_shortcut_info = {0};
    uint32 cur_select_index    =     0;
    
    if(MMISET_SET_SHORTCUT_SL_CTRL_ID <= ctrl_id 
        && ctrl_id <= MMISET_SET_SHORTCUT_SR_CTRL_ID
        && index < MMISET_FUNCKEY_MAX)//功能键
    {
        GetEnabledFuncKeyShortcutInfo(funckey_info);
        cur_select_index = funckey_info[index];
    }
    else if(MMISET_SET_SHORTCUT_DIRKEY_UP_CTRL_ID <= ctrl_id 
        && ctrl_id <= MMISET_SET_SHORTCUT_DIRKEY_RIGHT_CTRL_ID)
    {
        GetDirectionKeyInfo(&direction_info);
        cur_select_index = direction_info.direction_shortcut[ctrl_id - MMISET_SET_SHORTCUT_DIRKEY_UP_CTRL_ID];
    }
    else if(MMISET_SET_IDLE_SHORTCUT1_CTRL_ID <= ctrl_id 
        && ctrl_id <= MMISET_SET_IDLE_SHORTCUT5_CTRL_ID)
    {
        GetIdleShortcutInfo(&idle_shortcut_info);
        cur_select_index = idle_shortcut_info.idle_shortcut[ctrl_id - MMISET_SET_IDLE_SHORTCUT1_CTRL_ID];
    }
    return cur_select_index;       
}
/*****************************************************************************/
//  Description :保存所有setlist的设定值
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SaveAllShortcutSettings(void)
{
    uint16   i = 0;
    uint16   cur_index = 0;
    MMISET_SHORTCUT_ACTION_E action_index = MMISET_SHORTCUT_ACTION_LEFT;
    MMISET_SHORTCUT_TABLE_TYPE_E cur_table_type = MMISET_SHORTCUT_TABLE_SMART;
    MMI_CTRL_ID_T shortcut_child_setlist_ctrl_id[]=
    {
        MMISET_SET_SHORTCUT_SL_CTRL_ID,
        MMISET_SET_SHORTCUT_SR_CTRL_ID,
        MMISET_SET_SHORTCUT_DIRKEY_UP_CTRL_ID,
        MMISET_SET_SHORTCUT_DIRKEY_DOWN_CTRL_ID,        
        MMISET_SET_SHORTCUT_DIRKEY_LEFT_CTRL_ID,
        MMISET_SET_SHORTCUT_DIRKEY_RIGHT_CTRL_ID,        
        MMISET_SET_IDLE_SHORTCUT1_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT2_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT3_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT4_CTRL_ID,
        MMISET_SET_IDLE_SHORTCUT5_CTRL_ID    
    };
    uint16  max_ctrl_num = ((uint16)MMISET_SET_IDLE_SHORTCUT5_CTRL_ID - (uint16)MMISET_SET_SHORTCUT_SL_CTRL_ID +1);
    for(i = 0; i < max_ctrl_num; i++)
    {
        cur_index = GUISETLIST_GetCurIndex(shortcut_child_setlist_ctrl_id[i]);
        cur_table_type = GetCurShortcutTableType();
        action_index = (MMISET_SHORTCUT_ACTION_E)i;
        SetShortcutInfo(action_index, cur_index);
        GUISETLIST_SetCurIndex(shortcut_child_setlist_ctrl_id[i], cur_index);
    }
}




/*Edit by script, ignore 1 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
