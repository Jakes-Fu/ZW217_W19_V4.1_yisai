/*************************************************************************
 ** File Name:      mmiset_nv.h                                          *
 ** Author:         kelly.li                                        *
 ** Date:           2006/09/18                                          *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/18     kelly.li        Create.                              *
*************************************************************************/

#ifndef _MMISET_NV_H_
#define _MMISET_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
        MMI_CC_REDIAL_FLAG = MMI_MODULE_SET<< 16,
        MMINV_SET_MULTIM_VOLUME,
        MMINV_SET_MAIN_MENU_STYLE,
        MMINV_SET_CALL_REMINDER_TIME,
        MMINV_SET_CONNECT_PROMPT_STATUS,
        MMINV_SET_ANYKEY_REJECT_MODE,
        MMINV_SET_PERMIT_CALL_RING_INCALL,
        MMINV_SET_PERMIT_MSG_RING_INCALL,
        MMINV_SET_DSP_PLAY_RING,
        MMINV_SET_LANGUAGE_TYPE,
        MMINV_SET_KEYBOARD_LIGHT_INFO,
        MMINV_SET_WINDOW_MOVE_STYLE_INFO,
        MMINV_SET_MAINLCD_CONTRAST,
        MMINV_SET_SUBLCD_CONTRAST,
        MMINV_SET_AUTO_KEYLOCK_TIME,
        MMINV_SET_IS_RED_KEYLOCK,
        MMINV_SET_PRIVACY_PROTECT_INFO,
#ifdef FLIP_PHONE_SUPPORT
        MMINV_SET_IS_FLIPANSWER,
#endif
    //  MMINV_SET_IS_MENU_ANIMATION_OPEN,
        MMINV_SET_TV_OUT_MODE,
        MMINV_SET_CLIR_TYPE,    
        MMIENVSET_SETTING_CHOOSE,        // 记录使用的是什么环境设置
        MMIENVSET_SETTING_PRECHOOSE,     //  记录前一次使用的是什么环境设置
        MMIENVSET_SETTING_BEGIN,         //情景模式开始
        MMIENVSET_SETTING_STANDARD = MMIENVSET_SETTING_BEGIN, //常规模式
        MMIENVSET_SETTING_SILENT,      //静音模式
        MMIENVSET_SETTING_MEETING,      //振动模式
        MMIENVSET_SETTING_INSIDE,        //低音模式
        MMIENVSET_SETTING_NOISY,         //高音振动模式
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
        MMIENVSET_SETTING_EARPHONE,        //耳机模式
        MMIENVSET_SETTING_BLUETOOTH,        //蓝牙模式
#endif
        //CR244608
        MMIENVSET_SETTING_OWNER1,        //自定义情景模式1
        MMIENVSET_SETTING_OWNER2,        //自定义模式情景2
        MMIENVSET_SETTING_OWNER3,        //自定义情景模式3
        MMIENVSET_SETTING_OWNER4,        //自定义模式情景2
        MMIENVSET_SETTING_OWNER5,        //自定义情景模式3
        MMIENVSET_SETTING_END = MMIENVSET_SETTING_OWNER5, //情景模式结束
        MMIENVSET_SETTING_MUTE_STATE,    //set mute state,only valid for earphone and bluetooth mode
       // MMIENVSET_SETTING,               // 保存环境设置值
        MMINV_SET_IS_MINUTE_REMINDER,
        MMINV_SET_POWER_ON_OFF_ANIMATE_INFO,
        MMINV_SET_TIME_DISPLAY_TYPE,
        MMINV_SET_DATE_DISPLAY_TYPE,
        MMINV_SET_CLOCK_DISPLAY_STYLE,
#ifdef ADULT_WATCH_SUPPORT
        MMINV_SET_MTF_UP_TYPE,
        MMINV_SET_MTF_DOWN_TYPE,
        MMINV_SET_MTF_LEFT_TYPE,
        MMINV_SET_MTF_RIGHT_TYPE,

#endif
        MMINV_SET_IPNUMBER, 
        MMINV_SET_WALLPAPER_INFO,
        MMINV_SET_BACKLIGHT_INFO,
        MMINV_SET_POWER_ON_PWD_INFO,
        MMINV_SET_PRIVACY_PROTECT_PWD_INFO,
        MMINV_SET_IDLE_STYLE,
        MMINV_SET_IDLE_SUBSTYLE,    //substyle
#ifdef BAR_PHONE_WIDGET 
        MMINV_SET_IDLE_FORMER_STYLE,
#endif
       
        MMINV_SET_IDLEDISPLAY_INFO,
        MMINV_SET_LINE_TYPE,
        MMINV_SET_FLY_MODE_SWITCH,
#ifdef SCREENSAVER_SUPPORT
        MMINV_SCREENSAVER_SETTING_INFO,     //@xingdong.li
#endif
        MMINV_POWER_ON_ANIM_SETTING_INFO,   //@xingdong.li
        MMINV_POWER_OFF_ANIM_SETTING_INFO,  //@xingdong.li
        MMINV_SET_FUNCKEY_SHORTCUT_INFO,    //记录功能键快捷设置

#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
        MMINV_SET_SIM_NAME,
#ifdef MMI_MULTISIM_COLOR_SUPPORT                
        MMINV_SET_SIM_COLOR,        
#endif        
        MMINV_SET_MAJOR_SIM_SEL_FUNC,
        MMINV_SET_ANSWER_WITH_ORIGINAL_SIM_FUNC,
#endif
        MMINV_USER_SET_SIM_ONOFF_TYPE,
#ifdef MULTI_THEME_SUPPORT       
        MMINV_SET_CUR_STATIC_THEME,
        MMINV_SET_STATIC_THEME_NUM,
        MMINV_SET_THEME_TYPE,
        MMINV_SET_CUR_FILE_THEME,
        
#endif
#ifdef TTS_SUPPORT
        MMINV_SET_TTS_ON_OFF,
#endif
        MMINV_FLY_MODE_OPEN_PHONE,
#ifdef SET_3D_STYLE_SUPPORT
        MMINV_SET_3D_STYLE,
#endif
        MMINV_SET_IDLE_SHORTCUT,
        MMINV_SET_NEED_SET_DATETIME,
#if 0//def TOUCH_PANEL_SUPPORT
        MMINV_SET_TOUCH_VIBRATE_FLAG,
#endif
        MMINV_SET_TXT_CODE_TYPE,
#ifdef VT_SUPPORT
        MMINV_SET_VT_REPLACE_INFO,
        MMINV_SET_VT_RECORD_RV_DA_PATH,
        MMINV_SET_VT_RECORD_DA_PATH,
        MMINV_SET_VT_RECORD_RV_PATH,
        MMINV_SET_VT_FALLBACK_MODE,
        MMINV_SET_VT_ONE_TIME_RING_OPTION,   
        MMINV_SET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION,     
#endif
#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
        MMINV_SET_FONT,
#endif
#ifdef LCD_SLIDE_SUPPORT        
        MMINV_SET_SLIDE_OPEN_TYPE,
        MMINV_SET_SLIDE_CLOSE_TYPE,
        MMINV_SET_KEYLOCK_SWITCH_TYPE,
#endif
#ifdef MV_SUPPORT
    MMINV_SET_MAGIC_VOICE_STATUS,
    MMINV_SET_MAGIC_VOICE_SETTING_INFO,
#endif
        MMIENVSET_ENHANCE_RING,
#ifdef MOTION_SENSOR_TYPE
		MMINV_SET_SENSOR_OPEN_FLAG,
#endif
//#ifdef DPHONE_SUPPORT 
        MMINV_SET_SPEEDIAL_DELAY_TIME,
//#endif
#if defined(MMI_UPDATE_TIME_SUPPORT)
        MMINV_SET_AUTO_UPDATE_TIME_FLAG,
        MMINV_SET_UPDATETIME_INFO_FLAG,
#endif /*MMI_UPDATE_TIME_SUPPORT*/
#ifdef MMI_GPRS_DATA_SIM_SET
        MMINV_SET_GPRS_MODE_TYPE,
#endif
#if defined(MMI_BLACKLIST_SUPPORT)
        MMINV_SET_BLACKLIST_AUDIOCALL,
        MMINV_SET_BLACKLIST_VEDIOCALL,
        MMINV_SET_BLACKLIST_MESSAGE,
        MMINV_SET_BLACKLIST,
#endif
#ifdef MMI_SMARTDIVERT
        MMINV_SET_SMARTDIVERT_ACTION,
#endif
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
        MMINV_SET_KEY_LOCK_EFFECT,
        MMINV_SET_IS_ENABLE_KEYLOCK,
        MMINV_SET_AUTO_KEYLOCK_PASSWARD,
        MMINV_SET_IS_VISIBLE_KEYLOCK,        
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
        MMINV_SET_KL_PSW_ON,
        MMINV_SET_KL_PSW_CODE,
        MMINV_SET_KL_PSW_WATINT_TIME,
        MMINV_SET_KL_PSW_ERROR_TIMES,
#endif        
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        MMINV_SET_VIDEO_WALLPAPER,
#endif
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
        MMINV_SET_POWER_SAVING_SWITCH,//省电模式 开关 0 关，1 为开        
        MMINV_SET_POWER_SAVING_SETTING,//省电模式设置
        MMINV_SET_POWER_SAVING_OPRATE_ENV_MODE,//省电模式操作的情景模式(省电模式会去修改情景模式的设置项，这里记下被修改的情景模式id)
#endif
#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
        MMINV_SET_BATTERY_PERCENT_DIS_SWITCH,
#endif

#ifdef PDA_UI_SUPPORT_MANIMENU_GO
		MMINV_SET_GO_MENU_STYLE,
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        MMINV_SET_MAIN_MENU_SELECT_STYLE,
#endif
#endif

#ifdef PDA_UI_SUPPORT_U_IDLE
		MMINV_SET_UIDLE_STYLE,
#endif

#ifdef  PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER
        MMINV_SET_RENDER_EFFECT,
#endif
#ifdef MMI_AUTO_RECORD_SUPPORT
        MMINV_SET_AUTO_RECORD,
#endif

#if defined(TTS_SUPPORT_YOUNGTONE)
	MMINV_YTE_INFO_SETTING,
#endif

#ifdef MMI_INDIAN_SOS_SUPPORT
	MMINV_SET_SOS_SWITCH,
	MMINV_SET_SOS_MSG_SENT_SWITCH,
#ifdef MMIDC_F_FLASH
	MMINV_SET_SOS_LED_ALERT_SWITCH,
#endif
	MMINV_SET_SOS_RING_ALERT_SWITCH,
	MMINV_SET_SOS_MSG_INFO,
	MMINV_SET_SOS_CONTACT_LIST,
#endif
    MMINV_SET_DATA_SIM_SELECT,
    MMINV_SET_NETWORK_TYPE_MODE,
#ifdef ADULT_WATCH_SUPPORT
    MMINV_SET_WATCH_WEAR_TYPE,
    MMINV_SET_WATCH_RAISE_WRIST,
#endif
#ifdef MMI_GPS_SUPPORT
    MMINV_SET_GPS_ON_OFF,
#endif
    MMINV_SET_WATCH_PANEL_STYLE,
#ifdef MAINMENU_STYLE_SUPPORT
    MMINV_SET_WATCH_MENU_STYLE,
 #endif
    SET_MAX_NV_ITEM_NUM
}SET_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISET_NV_H_

