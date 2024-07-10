/*************************************************************************
** File Name:      mmiset_nv.c                                          *
** Author:         kelly.li                                         *
** Date:           2006/09/18                                           *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.    *
** Description:     This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 2006/09/18     kelly.li     Create.                              *
*************************************************************************/


#define _MMISET_NV_C_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mmiset_nv.h"
#include "mmi_modu_main.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
//#include "mmiset.h"
#include "mmiset_internal.h"
#include "mmiset_display.h"
//#include "mmiset_call.h"
#include "mmiset_call_internal.h"
#include "mmiset_set_ring.h"
//#include "mmiset_multisim.h"
#include "mmiset_multisim_internal.h"
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#include "mmikl_export.h"
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif

#if defined(TTS_SUPPORT_YOUNGTONE) 
#include "yte_type.h"
#endif

/*the length of set nv*/
const uint16 set_nv_len[] =
{
        sizeof(BOOLEAN),      				// MMI_CC_REDIAL_FLAG,
        sizeof(uint8),                      // MMINV_SET_MULTIM_VOLUME
        sizeof(MMIMAINMENU_TYPE_E),         // MMINV_SET_MAIN_MENU_STYLE
        1,                                  // MMINV_SET_PERMIT_MINHINT_RING_INCALL
        1,									// MMINV_SET_CONNECT_PROMPT_STATUS
        sizeof(MMISET_ANYKEY_REJECT_SET_T),	// MMINV_SET_ANYKEY_REJECT_MODE
        1,                                  // MMINV_SET_PERMIT_CALL_RING_INCALL
        1,                                  // MMINV_SET_PERMIT_MSG_RING_INCALL
        1,                                  // MMINV_SET_DSP_PLAY_RING
        
        4,                                  // MMINV_SET_LANGUAGE_TYPE
        sizeof(MMISET_KEYBOARD_LIGHT_SET_T),// MMINV_SET_KEYBOARD_LIGHT_INFO
        4,									// MMINV_SET_WINDOW_MOVE_STYLE_INFO
        1,                                  // MMINV_SET_MAINLCD_CONTRAST
        1,                                  // MMINV_SET_SUBLCD_CONTRAST  
        sizeof(MMISET_KEYLOCK_TYPE_E),                                  // MMINV_SET_AUTO_KEYLOCK_TIME
        1,                                      //MMINV_SET_IS_RED_KEYLOCK
        sizeof(MMISET_PRIVACY_PROTECT_T),	// MMINV_SET_PRIVACY_PROTECT_INFO
#ifdef FLIP_PHONE_SUPPORT
        sizeof(MMISET_FLIP_ANSWER_TYPE_T),  // MMINV_SET_IS_FLIPANSWER
#endif
      //  1,                                  // MMINV_SET_IS_MENU_ANIMATION_OPEN
        sizeof(MMISET_TVOUT_TYPE_E),        // MMINV_SET_TV_OUT_MODE
        (MMI_DUAL_SYS_MAX * sizeof(MN_CALL_CLIR_E)),				// MMINV_SET_CLIR_TYPE    
        sizeof(uint8),                                 // WHICH  ENV SETTING
        sizeof(uint8),                  //PRE ENV SETTING
        sizeof(MMIENVSET_SETTING_T),   //MMIENVSET_SETTING_STANDARD
        sizeof(MMIENVSET_SETTING_T),   //MMIENVSET_SETTING_SILENT
        sizeof(MMIENVSET_SETTING_T),   //MMIENVSET_SETTING_MEETING
        sizeof(MMIENVSET_SETTING_T),   //MMIENVSET_SETTING_INSIDE
        sizeof(MMIENVSET_SETTING_T),   //MMIENVSET_SETTING_NOISY
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
        sizeof(MMIENVSET_SETTING_T),   //MMIENVSET_SETTING_EARPHONE
        sizeof(MMIENVSET_SETTING_T),   //MMIENVSET_SETTING_BLUETOOTH
#endif
        //CR244608
        sizeof(MMIENVSET_SETTING_T),   //MMIENVSET_SETTING_OWNER1
        sizeof(MMIENVSET_SETTING_T),   //MMIENVSET_SETTING_OWNER2
        sizeof(MMIENVSET_SETTING_T),   //MMIENVSET_SETTING_OWNER3
        sizeof(MMIENVSET_SETTING_T),   //MMIENVSET_SETTING_OWNER4
        sizeof(MMIENVSET_SETTING_T),   //MMIENVSET_SETTING_OWNER5
        sizeof(BOOLEAN),               //MMIENVSET_SETTING_MUTE_STATE
        1,									//MMINV_SET_IS_MINUTE_REMINDER
        sizeof(MMISET_ANIMATE_SET_T),       //MMINV_SET_POWER_ON_OFF_ANIMATE_INFO
        4, 									//MMINV_SET_TIME_DISPLAY_TYPE
        4,
        4,                                  //MMINV_SET_CLOCK_DISPLAY_STYLE
#ifdef ADULT_WATCH_SUPPORT
        4,                                  //MMINV_SET_MTF_UP_TYPE,
        4,                                  //MMINV_SET_MTF_DOWN_TYPE,
        4,                                  //MMINV_SET_MTF_LEFT_TYPE,
        4,                                  //MMINV_SET_MTF_RIGHT_TYPE,

#endif
        (sizeof(MMISET_IP_NUMBER_INFO_T)*MMI_DUAL_SYS_MAX),    // MMINV_SET_IPNUMBER
        (sizeof(MMISET_WALLPAPER_SET_T)*(MMISET_IDLE_STYLE_MAX+1)),     //MMINV_SET_WALLPAPER_INFO
        sizeof(MMISET_BACKLIGHT_SET_T),     // MMINV_SET_BACKLIGHT_INFO
        sizeof(MMISET_POWER_ON_PWD_T),      // MMINV_SET_POWER_ON_PWD_INFO
        sizeof(MMISET_PRIVACY_PROTECT_PWD_T),//MMINV_SET_PRIVACY_PROTECT_PWD_INFO
        sizeof(MMISET_IDLE_STYLE_E),        //MMINV_SET_IDLE_STYLE
        sizeof(MMISET_IDLE_SUBSTYLE_E),        //MMINV_SET_IDLE_SUBSTYLE        
#ifdef BAR_PHONE_WIDGET 
        sizeof(MMISET_IDLE_STYLE_E),        //MMINV_SET_IDLE_FORMER_STYLE
#endif        
        sizeof(MMISET_IDLE_DISPLAY_T),      // MMINV_SET_IDLEDISPLAY_INFO
        (MMI_DUAL_SYS_MAX * sizeof(uint8)),   // MMINV_SET_LINE_TYPE
        sizeof(BOOLEAN),                     // MMINV_SET_FLY_MODE_SWITCH
#ifdef SCREENSAVER_SUPPORT        
        sizeof(MMISET_SCREENSAVER_INFO_T),          //@xingdong.li
#endif
        sizeof(MMISET_POWER_ON_OFF_ANIM_INFO_T),    //@xingdong.li
        sizeof(MMISET_POWER_ON_OFF_ANIM_INFO_T),    //@xingdong.li
        
        (sizeof(MMISET_SHORTCUT_TYPE_E)*MMISET_FUNCKEY_MAX),	// MMINV_SET_FUNCKEY_SHORTCUT_INFO
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
        (sizeof(MMISET_SIM_NAME_T)*MMI_DUAL_SYS_MAX),  //MMINV_SET_SIM_NAME
#ifdef MMI_MULTISIM_COLOR_SUPPORT        
        (sizeof(MMISET_SIM_COLOR_T)*MMI_DUAL_SYS_MAX),
#endif        
        MMISET_MAJOR_SIM_SEL_FUNC_MAX*sizeof(MMISET_MAJOR_SIM_SEL_TYPE_E),//MMINV_SET_MAJOR_SIM_SEL_FUNC
        sizeof(BOOLEAN) * MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX,
#endif
        sizeof(MMISET_DUALSYS_TYPE_E),		//MMINV_USER_SET_SIM_ONOFF_TYPE
#ifdef MULTI_THEME_SUPPORT       
        sizeof(uint16),
        sizeof(uint8),
        sizeof(MMISET_THEME_TYPE_E), 
        sizeof(wchar)*(MMIFMM_FILE_FULL_PATH_MAX_LEN+1),
        
#endif
#ifdef TTS_SUPPORT
        sizeof(BOOLEAN),
#endif
        sizeof(BOOLEAN),
#ifdef SET_3D_STYLE_SUPPORT
        sizeof(MMISET_3D_STYLE_T),      //MMINV_SET_3D_STYLE
#endif
        sizeof(MMISET_IDLE_SHORTCUT_T), //MMINV_SET_IDLE_SHORTCUT
        1,//MMINV_SET_NEED_SET_DATETIME
#if 0//def TOUCH_PANEL_SUPPORT
        sizeof(BOOLEAN),
#endif
        sizeof(MMISET_TXT_CODE_TYPE_TYPE_E),
#ifdef VT_SUPPORT
        MMISET_VT_REPLACE_MAX*sizeof(MMISET_VT_REPLACE_FILE_INFO_T), //MMINV_SET_VT_REPLACE_INFO
        sizeof(MMIFILE_DEVICE_E), //MMINV_SET_VT_RECORD_RV_DA_PATH
        sizeof(MMIFILE_DEVICE_E), //MMINV_SET_VT_RECORD_DA_PATH
        sizeof(MMIFILE_DEVICE_E), //MMINV_SET_VT_RECORD_RV_PATH
        sizeof(MMISET_VT_FALLBACK_HANDLE_MODE_E), //MMINV_SET_VT_FALLBACK_MODE
        sizeof(BOOLEAN), //MMINV_SET_VT_ONE_TIME_RING_OPTION
        sizeof(MMISET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION), //MMINV_SET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION
#endif
#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
        sizeof(wchar)*(MMIFMM_FILE_FULL_PATH_MAX_LEN+1),
#endif
#ifdef LCD_SLIDE_SUPPORT        
        sizeof(MMINV_SET_SLIDE_OPEN_TYPE), 									//MMINV_SET_SLIDE_OPEN_TYPE
        sizeof(MMINV_SET_SLIDE_CLOSE_TYPE), 									//MMINV_SET_SLIDE_CLOSE_TYPE
        sizeof(MMINV_SET_KEYLOCK_SWITCH_TYPE), 									//MMINV_SET_KEYLOCK_SWITCH_TYPE
#endif    
#ifdef MV_SUPPORT
        sizeof(BOOLEAN),					// MMINV_SET_MAGIC_VOICE_STATUS
        sizeof(MMISET_MV_INFO_T),           //
#endif
        
        sizeof(BOOLEAN),    //agc enhance flag
#ifdef MOTION_SENSOR_TYPE
		sizeof(uint32),//MMINV_SET_SENSOR_OPEN_FLAG
#endif

//#ifdef DPHONE_SUPPORT
        sizeof(uint32),					//MMINV_SET_SPEEDIAL_DELAY_TIME
//#endif
#if defined(MMI_UPDATE_TIME_SUPPORT)
        sizeof(BOOLEAN),        //MMINV_SET_AUTO_UPDATE_TIME_FLAG
        sizeof(BOOLEAN),        //MMINV_SET_UPDATETIME_INFO_FLAG
#endif /*MMI_UPDATE_TIME_SUPPORT*/
#ifdef MMI_GPRS_DATA_SIM_SET
        sizeof(MMISET_GPRS_MODE_TYPE_E),//MMINV_SET_GPRS_MODE_TYPE
#endif
#if defined(MMI_BLACKLIST_SUPPORT)
        sizeof(BOOLEAN),//blacklist audio call
        sizeof(BOOLEAN),//blacklist vedio call
        sizeof(BOOLEAN),//blacklist message
        sizeof(MMISET_BLACKLIST_T),
#endif
#ifdef MMI_SMARTDIVERT
    sizeof(BOOLEAN),//MMINV_SET_SMARTDIVERT_ACTION
#endif	

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
        sizeof(int32),
        1,                                                                                  //MMINV_SET_IS_ENABLE_KEYLOCK
        (MMI_KL_PASSWARD_POINT_MAX * sizeof(int8)),             // MMINV_SET_AUTO_KEYLOCK_PASSWARD
        1,                                                                                  //MMINV_SET_IS_VISIBLE_KEYLOCK        
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
        sizeof(BOOLEAN),                                                //MMINV_SET_KL_PSW_ON,
        (MMI_KL_PSW_LEN * sizeof(int8)),    //MMINV_SET_KL_PSW_CODE,
        1,                                  //MMINV_SET_KL_PSW_WATINT_TIME,
        1,                                  //MMINV_SET_KL_PSW_ERROR_TIMES,
#endif
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
        sizeof(MMISET_VIDEO_WALLPAPER_NV_T),    //MMINV_SET_VIDEO_WALLPAPER
#endif

#ifdef MMI_POWER_SAVING_MODE_SUPPORT
        sizeof(BOOLEAN),
        sizeof(MMISET_POWER_SETTING_ITEM_T),
        sizeof(uint8),
#endif    
#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
        sizeof(BOOLEAN),    
#endif

#ifdef PDA_UI_SUPPORT_MANIMENU_GO
		sizeof(MMINV_SET_GO_MENU_STYLE),
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        sizeof(MMINV_SET_MAIN_MENU_SELECT_STYLE),
#endif
#endif
#ifdef PDA_UI_SUPPORT_U_IDLE
		sizeof(MMINV_SET_UIDLE_STYLE),
#endif
#ifdef PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER
    	sizeof(MMISET_EFFECT_RENDER_SETTING),
#endif
#ifdef MMI_AUTO_RECORD_SUPPORT
    	sizeof(BOOLEAN),
#endif
#if defined(TTS_SUPPORT_YOUNGTONE)
		sizeof(YTE_INFO_STRUCT),
#endif
#ifdef MMI_INDIAN_SOS_SUPPORT
	sizeof(BOOLEAN),//MMINV_SET_SOS_SWITCH
	sizeof(BOOLEAN),//MMINV_SET_SOS_MSG_SENT_SWITCH	
#ifdef MMIDC_F_FLASH
	sizeof(BOOLEAN),//MMINV_SET_SOS_LED_ALERT_SWITCH
#endif
	sizeof(BOOLEAN),//MMINV_SET_SOS_RING_ALERT_SWITCH
	sizeof(MMISET_SOS_MESSAGE_CONTENT_T),//MMINV_SET_SOS_MSG_INFO
	(MMISET_SOS_CONTACTS_MAX_NUM * sizeof(MMISET_SOS_CONTACT_ITEM_T)),	//MMINV_SET_SOS_CONTACT_LIST
#endif
	sizeof(uint8),
	(sizeof(uint8) * 4),
#ifdef ADULT_WATCH_SUPPORT
    sizeof(MMINV_SET_WATCH_WEAR_TYPE),
    sizeof(BOOLEAN),
#endif
#ifdef MMI_GPS_SUPPORT
    sizeof(BOOLEAN),
#endif
    sizeof(WATCH_PANEL_TYPE_T), //yangyu add for watch panel face
#ifdef MAINMENU_STYLE_SUPPORT
    sizeof(uint8),
#endif
};

/*****************************************************************************/
// 	Description : register set module nv len and max item
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_SET, set_nv_len, sizeof(set_nv_len) / sizeof(uint16));
}



