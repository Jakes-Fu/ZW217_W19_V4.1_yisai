/*****************************************************************************
** File Name:      mmiset_menutable.h                                        *
** Author:         kelly.li                                             *
** Date:           2006/09/18                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to set menu                          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18     kelly.li     Create                                    *
******************************************************************************/

#ifndef _MMISET_MENUTABLE_H_
#define _MMISET_MENUTABLE_H_

#include "mmi_module.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/


//Menu ID
typedef enum
{
    SET_ID_MENU_START = (MMI_MODULE_SET << 16),
    //Env Set,Start @ add by maql
    ID_ENVSET_ACTIVE,				//启动
    ID_ENVSET_SETTING_OPT,			//个性化设置
    ID_ENVSET_CHANGE_NAME,			//改名称   
    #ifdef PLATFORM_SC6600L
    ID_ENVSET_ENHANCE_RING,         //铃声增强
    #endif
    ID_ENVSET_RING_VOL,				//铃声音量调节
    ID_ENVSET_RING_TYPE,            //响铃方式
    ID_ENVSET_RING_OTHER,           //其他提示音
#ifndef MMI_ENVSET_MINI_SUPPORT
    ID_ENVSET_CALL_RING_VOL,
    ID_ENVSET_MSG_RING_VOL,
    ID_ENVSET_KEY_RING_VOL,
    ID_ENVSET_TKEY_RING_VOL,
    ID_ENVSET_COMM_VOL,
    ID_ENVSET_ALARM_RING_VOL,
    ID_ENVSET_POWER_RING_VOL,
#ifdef MMI_READMENU_ENABLE
    ID_ENVSET_MENUREAD_VOL,
#endif
#endif
#ifndef MMI_ENVSET_MINI_SUPPORT
    ID_ENVSET_CALL_RING_TYPE,		//铃声类型
    ID_ENVSET_MSG_RING_TYPE,
    ID_ENVSET_ALARM_RING_TYPE,
    ID_ENVSET_KEY_RING_TYPE,
	ID_ENVSET_TKEY_RING_TYPE,
    ID_ENVSET_SLIDE_RING_TYPE,
    ID_ENVSET_POWER_RING_TYPE,
    ID_ENVSET_FLIP_RING_TYPE,
    ID_ENVSET_DOWNLOAD_COMPLETE_NOTIFY_SWITCH,//下载提示音
    ID_ENVSET_LOW_VOLTAGE_WARN_SWITCH,//低电压报警
	ID_ENVSET_POWER_RING_SWITCH,	  //开关机声音的开关
    ID_ENVSET_SLIDE_RING_ON,
    ID_ENVSET_SLIDE_RING_OFF,
    //Env Set,End @ add by maql 
#endif
    
	ID_SET_CALL_DIVERT_OPEN,
	ID_SET_CALL_DIVERT_CLOSE,
	ID_SET_CALL_DIVERT_STATUS,
	
	ID_SET_CALL_WAITING_OPEN,
	ID_SET_CALL_WAITING_CLOSE,
	ID_SET_CALL_WAITING_STATUS,
	
	ID_SET_CALL_BAR_OPEN,
	ID_SET_CALL_BAR_CLOSE,
	ID_SET_CALL_BAR_STATUS,

	ID_SET_RING,
    ID_SET_RING_SELECT_CALL,
    ID_SET_RING_SELECT_MSG,
    ID_SET_RING_VOLUME_CALL,
    ID_SET_RING_VOLUME_MSG,
    ID_SET_RING_VOLUME_COMMUNICATION,
    ID_SET_RING_VOLUME_KEY_TONE,
    ID_SET_RING_VOLUME_POWER,
    ID_SET_PROFILES,
    ID_SET_TIME_DATE,
    ID_SET_LANGUAGE,

//开放这个ID定义，不影响大局，只是在使用的时候增加宏的控制
//sogou不支持模糊音，非触摸屏不支持手写，如果两者同时成立时，则导致没有输入法设置项
//#if (defined TOUCH_PANEL_SUPPORT || ((!defined IM_ENGINE_SOGOU) && ((defined IM_SIMP_CHINESE_SUPPORT) || (defined IM_TRAD_CHINESE_SUPPORT))))
    ID_SET_INPUT_MODE,
//#endif

    ID_SET_SHORTCUT,
    ID_SET_TXT_CODE_TYPE,
    ID_SET_FUNCKEY_SHORTCUT,
    ID_SET_IDLE_SHORTCUT,
    ID_SET_COORDINATE,
    ID_SET_AUTO_POWERONOFF,
    ID_SET_SLIDE_OPERATION,   
    ID_SET_SAVE_ELECTRICITY,//省电模式
    ID_SET_POWER_MANAGEMENT,
    ID_SET_DM_SERVICE,
    ID_SET_KEY_BOARD_LIGHT,
    ID_SET_WINDOW_MOVE_STYLE,
	ID_SET_AUTO_BOARDKEY,
    ID_SET_DISPLAY,
    ID_SET_CALL,
    ID_SET_DIRECTIONKEY,
    ID_SET_NETWORK,
#ifdef MMI_PREFER_PLMN_SUPPORT
    ID_SET_NETWORK_SETTING_SEL_MENU, // 网络选择menu id
    ID_SET_NETWORK_SETTING_PREFER_MENU, //偏好网络menu id
    ID_SET_ADD_PREFER_NETWORK_FROM_LIST, //偏好网络: add from list
    ID_SET_CREATE_PREFER_NETWORK, //偏好网络: New
    ID_SET_CHANGE_PREFER_NETWORK_PRIORITY,//偏好网络:Change priority
    ID_SET_DELETE_PREFER_NETWORK, //偏好网络: Delete
#endif
    ID_SET_BAND,
    ID_SET_SECURITY,
    ID_SET_RESET_FACTORY,
#ifdef CMCC_UI_STYLE
    ID_SET_PHONE_VERSION,
#endif
    ID_SET_TIME,
    ID_SET_DATE,
	ID_SET_TIME_DISPLAY_TYPE,   
	ID_SET_DATE_DISPLAY_TYPE,
    ID_SET_WALLPAPER,
	ID_SET_PIC_WALLPAPER,
	//ID_SET_PRIVACY_PROTECT_SET,
	//ID_SET_PRIVACY_PROTECT_MODIFY,
    ID_SET_ANIMATION_WALLPAPER,
//	ID_SET_MENU_ANIMATION,
    ID_SET_MAINMENU_STYLE,
    ID_SET_IDLE_STYLE,
    ID_SET_3D_STYLE,
    ID_SET_3D_STYLE_WINDOW,
    ID_SET_3D_STYLE_MAINMENU,
    ID_SET_3D_OTHER,    
	ID_SET_IDLE_DISPLAY,
#ifdef SCREENSAVER_SUPPORT 
	ID_SET_SCREENSAVER,
#endif
	ID_SET_TV_OUT,
    ID_SET_POWER_ON_OFF_ANIMATION,
    ID_SET_POWER_ON_SET,
    ID_SET_POWER_OFF_SET,
    ID_SET_BACK_LIGHT,
    ID_SET_CONTRAST,
	ID_SET_MAIN_LCD_CONTRAST,
    ID_SET_SUB_LCD_CONTRAST,
    ID_SET_IP_CALL, 
    ID_SET_IPNUMBER_ACTIVE,
    ID_SET_IPNUMBER_DEACTIVE,
    ID_SET_IPNUMBER_MODIFY,
    ID_SET_IPNUMBER_ADD,
    ID_SET_IPNUMBER_DEL,
    ID_SET_FLIP_ANSWER,
    ID_SET_MINUTE_REMINDER,
	ID_SET_CONNECT_PROMPT,
	ID_SET_ANYKEY_REJECT_MODE,
	ID_SET_HIDE_ID,
    ID_SET_AUTO_REDIAL,
    ID_SET_CALL_DIVERTS,
    ID_SET_CALL_WAITING,
    ID_SET_CALL_BAR,
    ID_SET_DIVERTS_ALL_CALLS,
#ifdef MMI_CSP_SUPPORT
    ID_SET_DIVERTS_ALL_CONDITIONAL, //contains cfb,cfnry,cfnrc
#endif  
    ID_SET_DIVERTS_WHEN_BUSY,
    ID_SET_DIVERTS_NO_ANSWER,
    ID_SET_DIVERTS_NOT_REACHABLE,
    ID_SET_DIVERTS_CANCEL_ALL,
    ID_SET_SS_OPEN,
    ID_SET_SS_CLOSE,
    ID_SET_SS_QUERY_STATUS,
    ID_SET_SS_DIVERT_VOICE,
    ID_SET_SS_DIVERT_NUMBER,
    ID_SET_BAR_ALL_MO_CALLS,
    ID_SET_BAR_ALL_MT_CALLS,
    ID_SET_BAR_MT_CALLS_ROAM,
    ID_SET_BAR_MO_CALLS_INTL,
    ID_SET_BAR_MO_CALLS_INTL_ROAM,
    ID_SET_BAR_CANCEL_ALL,
    ID_SET_BAR_CHANGE_PW,
    ID_SET_PIN1,
    ID_SET_PIN2,
    ID_SET_PIN_OPEN,
    ID_SET_PIN_MODIFY,
    ID_SET_PIN_CLOSE,
    ID_SET_MSD,
	/*FDN start*/
	ID_FDN_STATE,//@zhaohui add for fdn
	ID_FDN_NUM,
	/*FDN end*/
#if defined(MMI_BLACKLIST_SUPPORT)
    ID_SET_BLACKLIST_LIST,
    ID_SET_BLACKLIST_SETTING,
    ID_SET_BLACKLIST_NEW,
    ID_SET_BLACKLIST_EDIT,
    ID_SET_BLACKLIST_DEL,
    ID_SET_BLACKLIST_CONTACT,
    ID_SET_BLACKLIST_DELALL,
#endif
    ID_SET_PWD_OPEN,
    ID_SET_PWD_MODIFY,
    ID_SET_PWD_CLOSE,	
	ID_SET_NETWORK_SETTING,
	ID_SET_NETWOEK_LOCKING,
	ID_SET_NETWORK_OPEN,
	ID_SET_NETWORK_CLOSE,
	ID_SET_NETWORK_STATUS,
//#ifdef MMI_CSP_SUPPORT	
	ID_SET_LINE_SELECT,	
	ID_SET_LINE_LOCK,
	ID_SET_LINE_SELECT_LINE1,
	ID_SET_LINE_SELECT_LINE2,
	ID_SET_SS_LINE1_VMN,
	ID_SET_SS_LINE2_VMN,
//#endif
    ID_SET_SS_LINE_VMN,
    ID_SET_SS_FAX_VMN,
	ID_SET_SS_DATA_VMN,
    ID_SET_NETSETTING,       //@maggie add for cr79064
    ID_SET_NETWORK_TYPE,     //@maggie add for cr79064
    ID_SET_NETWORK_METHOD,   //@maggie add for cr79064 
    ID_SET_NETWORK_SEL_AUTO,	//@yongwei.he add begin
    ID_SET_NETWORK_SEL_MANU,
    ID_SET_NETWORK_TYPE_GSM,
    ID_SET_NETWORK_TYPE_TD,
    ID_SET_NETWORK_TYPE_DM,
    ID_SET_NETWORK_SEL_MANU_GSM,
    ID_SET_NETWORK_SEL_MANU_TD,
    ID_SET_NETWORK_SEL_MANU_DM,
    ID_SET_NETWORK_TYPE_GSM_PREFER,
    ID_SET_NETWORK_TYPE_TD_PREFER,
    ID_SET_NETWORK_SEL_MANU_GSM_PREFER,
    ID_SET_NETWORK_SEL_MANU_TD_PREFER,	//@yongwei.he add end
    ID_SET_DIVERTS_QUERY_ALL_CALLS,//@zhaohui add,查询全部转移
    ID_SET_DIVERTS_AUDIO,//@zhaohui add,呼叫转移中的"语音电话"
    ID_SET_DIVERTS_VIDEO,//@zhaohui add,呼叫转移中的"可视电话"
    ID_SET_DIVERTS_AUDIOANDVIDEO,//@zhaohui add,呼叫转移中的"语音电话与可视电话"
#ifdef VT_SUPPORT
    ID_SET_VIDEOPHONE,//vt call setting
    ID_SET_VT_FALLBACK_HANDLE, //vt fallback handle setting
    ID_SET_VT_SPCIFY_SAVE_PATH, //vt record save path
    ID_SET_VT_REPLACE_IMAGE, //select replacing image
    ID_SET_VT_LOCAL_VIDEO, //local video setting
    ID_SET_VT_ONE_TIME_RING, //one time ring when voice accepting vt incoming call
    ID_SET_VT_CUSTOM_PHOTO_MODIFY, //modify replace image
    ID_SET_VT_CUSTOM_PHOTO_DEL, //del replace image, change to default
#endif
#ifdef TOUCH_PANEL_SUPPORT
    ID_SET_DIAL_PANEL_TOUCH_VIBRATE,
#endif
//add for dual sys
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    ID_SET_RING_SIM1_SELECT_CALL,
    ID_SET_RING_SIM2_SELECT_CALL,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    ID_SET_RING_SIM3_SELECT_CALL,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    ID_SET_RING_SIM4_SELECT_CALL,
#endif
#endif
    ID_SET_RING_SIM_MAX_SELECT_CALL,   //dummy.
#endif

#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    ID_SET_RING_SIM1_SELECT_MSG,
    ID_SET_RING_SIM2_SELECT_MSG,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    ID_SET_RING_SIM3_SELECT_MSG,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    ID_SET_RING_SIM4_SELECT_MSG,
#endif
#endif
    ID_SET_RING_SIM_MAX_SELECT_MSG,  //dummy.
#endif

#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    ID_SET_MULTI_SIM_SETTING,
#endif
    ID_SET_MULTISYS_SIM_ONOFF_TYPE,
	ID_SET_MAJORSIM_SELECT,

	ID_SET_DUALSYS_SIM1_ON,
	ID_SET_DUALSYS_SIM2_ON,
	ID_SET_DUALSYS_ALL_SIM_ON,
	ID_SET_SIM1_ACTIVE,
	ID_SET_SIM2_ACTIVE,
	ID_SET_SIM_NAME,
	ID_SET_SIM_COLOR,
	ID_SET_DUALSIM_CALL_DIVERT,
	ID_SET_DUALSIM_CALL_DIVERT_HELP,
	ID_SET_SIM1_NAME,
	ID_SET_SIM2_NAME,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
	ID_SET_SIM3_NAME,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
	ID_SET_SIM4_NAME, 
#endif
#endif
	ID_SET_DUALSIM_WIZARD,
    ID_SET_MULTISIM_HELP,
    ID_SET_DUAL_SYS_CFNRC,

    ID_SET_CALL_SIM_SEL,
	ID_SET_SMS_SIM_SEL,
	ID_SET_MMS_SIM_SEL,
	ID_SET_BROWSER_SIM_SEL,
	ID_SET_SPEED_DIAL,
    ID_SET_STREAMING_SIM_SEL,
    ID_SET_ANSWER_USING_ORIGINAL_SIM,

    ID_SET_FLY_MODE,
    ID_SET_OTHER,
#ifdef MULTI_THEME_SUPPORT
    ID_SET_THEME,
#endif
#ifdef TTS_SUPPORT
    ID_SET_TTS, //barry.peng tts
#endif
    ID_SET_ANIMATION,

#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
    ID_SET_FONT,
#endif
    ID_SET_SLIDE_OPEN,
    ID_SET_SLIDE_CLOSE,
    ID_SET_KEYLOCK_SWITCH,    
	
#ifdef MV_SUPPORT
    ID_SET_MAGIC_VOICE,
    ID_SET_MAGIC_VOICE_OPEN_CLOSE,
    ID_SET_MAGIC_VOICE_PARAM_SET,
#endif

#ifdef MOTION_SENSOR_TYPE
	ID_SET_SENSOR,
	ID_SET_SENSOR_AUTO_ROTATE,
#endif
#if defined(MMI_UPDATE_TIME_SUPPORT)
    ID_SET_UPDATE_TIME_SET,
    ID_SET_AUTO_UPDATE_TIME,
    ID_SET_UPDATE_SYSTEM_TIME,
    ID_SET_UPDATE_TIME_HELP,
#endif/*(MMI_UPDATE_TIME_SUPPORT)*/
#if defined(MMI_SIM_LOCK_SUPPORT)
    ID_SET_SECURITY_SIM_LOCK_NETWORK,
    ID_SET_SECURITY_SIM_LOCK_NETWORK_SUBSET,
    ID_SET_SECURITY_SIM_LOCK_SP,
    ID_SET_SECURITY_SIM_LOCK_CORPORATE,
    ID_SET_SECURITY_SIM_LOCK_USER,
#endif
#ifdef MMI_PDA_SUPPORT
	ID_ENVSET_SILENTON,
	ID_ENVSET_FLYON,
	ID_POWEROFF,
	ID_ENVSET_SILENTOFF,
	ID_ENVSET_FLYOFF,
#endif
    ID_ENVSET_VIDEO_ADD,
    ID_ENVSET_VIDEO_DELETE,
    ID_ENVSET_VIDEO_RESET,
    #ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
	#ifdef MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT  
    ID_SET_KEYLOCK_LONG_TP,
    #endif
    #ifdef MMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT  
    ID_SET_KEYLOCK_SLIDEDOWN,
    #endif
    #ifdef MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT  
    ID_SET_KEYLOCK_SLIDE_SCROLLKEY,
    #endif
    #ifdef MMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT  
    ID_SET_KEYLOCK_SMART_SLIDEDOWN,
    #endif
    #ifdef MMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT  
    ID_SET_KEYLOCK_SLIDE_IPHONE,
    #endif
    #ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT  
    ID_SET_KEYLOCK_GRAPHIC,
    #endif
    #ifdef MMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT  
    ID_SET_KEYLOCK_ICON_RING,
    #endif
    #ifdef MMI_KEY_LOCK_FRUIT_SPLIT_SUPPORT  
    ID_SET_KEYLOCK_FRUIT_SPLIT,
    #endif
    //michael wang kl
    #ifdef MMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
    ID_SET_KEYLOCK_DOUBLE_CIRCLE,
    #endif
    //michael wang
    //spec
    #ifdef MMI_KEY_LOCK_SPEC_DOUBLE_CIRCLE_SUPPORT
    ID_SET_KEYLOCK_DOUBLE_CIRCLE_SPEC,
    #endif
    #ifdef MMI_KEY_LOCK_EFFECT_QQVGA
    ID_SET_KEYLOCK_LONG_TP_QQVGA,
    #endif
#endif
    
#ifdef BROWSER_SUPPORT
	ID_SET_SELECT_BROWSER_TYPE,
#endif
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
    ID_SET_MAINMENU_SELECT_STYLE,
#endif
#ifdef MMI_INDIAN_SOS_SUPPORT
    ID_SET_SOS_NUMBER_LIST,
    ID_SET_SOS_MESSAGE_SETTINGS,
    ID_SET_SOS_TYPE_SETTINGS,
    ID_SET_SOS_INFO,
    ID_SET_SOS_NUMBER_EDIT,
    ID_SET_SOS_NUMBER_CLEAR,
    ID_SET_SOS_NUMBER_CLEAR_ALL,
    ID_SET_SOS_NUMBER_CONTACT,
#endif
    SET_ID_MENU_MAX
}SET_ID_MENU_E;



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    SET_MENU_LABEL_START = (MMI_MODULE_SET << 16),

    #include "mmiset_menutable.def"

    SET_MENUTABLE_MAX
} SET_MENU_LABEL_E;

#undef MENU_DEF


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //_SET_MENUTABLE_H_

