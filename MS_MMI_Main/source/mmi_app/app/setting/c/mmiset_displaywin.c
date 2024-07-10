/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004      maryxiao       Create
******************************************************************************/

#define _MMISET_DISPLAYWIN_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilistbox.h"
#include "guiform.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmiset_id.h"
#include "mmiset_menutable.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmipub.h"
#include "mmiset_text.h"
#include "mmi_appmsg.h"
#include "mmiset_display.h"
#include "guires.h"
#include "mmiset_nv.h"
#include "mmiset_wintab.h"
#include "mmiudisk_export.h"
#include "guiownerdraw.h"
#include "mmiset_image.h"
#include "mmiset_text.h"
#include "guisetlist.h"
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmi_livewallpaper_text.h"
#endif
#include "mmk_tp.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmiset_func.h"
#include "mmi_resource.h"
#include "mmiset_export.h"
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_text.h"
#include "mmiqbtheme_export.h"
#endif
#ifdef UI_P3D_SUPPORT   
#include "mmi_winse.h"
#endif
#include "mmiset_internal.h"
#include "mmiset_lock_effect_internal.h"
#include "version.h"

#include "mmi_text.h"
#include "guilabel.h"
#include "guitext.h"




//#ifdef PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM
#include "mmiset_mainmenu_export.h"
//#endif

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#include "mmiset_lock_effect_export.h"
#endif

#include "watch_common.h"
#include "watch_common_list.h"

#include "mmicom_trace.h"

int16 s_setting_brightness_value = 0;
static int16 s_setting_call_volume_value = 0;
static int16 s_setting_ring_volume_value = 0;
uint8 cur_bright_img_id=0;
static uint8 cur_call_volume_img_id=0;
uint8 cur_ring_volume_img_id=0;
uint8 progress_down_flag = 0;
int16 current_percent_brightness = 0;

/**--------------------------------------------------------------------------*
**                function declaration                                       *
**--------------------------------------------------------------------------*/
#define     MMISET_KEYBOARD_LIGHT_TIME_20       20000
#define     MMISET_KEYBOARD_LIGHT_TIME_10       10000
#define     MMISET_KEYBOARD_LIGHT_TIME_5        5000
#if defined MMISET_ANIM_EFFECT_SUPPORT
BOOLEAN     s_fmm_poweron_call = FALSE;
BOOLEAN     s_fmm_poweroff_call = FALSE;

#define MMISET_WATCH_TEXT_LEN	300	

#define SCALE 1  //yangyu add
#ifndef ROUND
#define ROUND(f) (int)((f)>0.0? (f+0.5):(f-0.5))
#endif

#ifdef ZTE_SUPPORT_240X284
#ifdef ZTE_WATCH
#define SET_PROGRESSBAR_RECT DP2PX_RECT(12,112, 228, 133)
#define WATCH_SETOK_BTN_RECT     DP2PX_RECT(12,196,228,236)
#define SET_BTN_TWO_LEFT_RECT     DP2PX_RECT(5,204,110,254)
#define SET_BTN_TWO_RIGHT_RECT    DP2PX_RECT(130,204,240,254)

#define SET_PROGRESSBAR_TOTAL_LENGTH    216
#define SET_PROGRESSBAR_PERLEVEL_LENGTH 36
#define PROGRESSBAR_BRIGTH_LEVEL_MAX 6
#define BRIGHT_ICON_B_X	90
#define BRIGHT_ICON_B_Y	30
#define BRIGHT_ICON_VOL_X	102
#define BRIGHT_ICON_VOL_Y	57
#else
#define SET_BTN_TWO_LEFT_RECT     DP2PX_RECT(5,234,110,284)
#define SET_BTN_TWO_RIGHT_RECT    DP2PX_RECT(130,234,240,284)

#define SET_BTN_MINUS_RECT     DP2PX_RECT(22,195,80,250)
#define SET_BTN_PLUS_RECT    DP2PX_RECT(168,195,220,250)

#define BRIGHT_IMAGE_X	66
#define BRIGHT_IMAGE_Y	80

#define BRIGHT_ICON_X	104
#define BRIGHT_ICON_Y	205
#endif

#else

#ifdef ZTE_WATCH
#define SETOK_BTN_HEIGHT 40
#define SET_PROGRESSBAR_RECT DP2PX_RECT(12,112, 228, 133)
#define WATCH_SETOK_BTN_RECT     DP2PX_RECT(12,MMI_MAINSCREEN_HEIGHT - SETOK_BTN_HEIGHT-20,228,MMI_MAINSCREEN_HEIGHT -20)

#define SET_PROGRESSBAR_TOTAL_LENGTH    216
#define SET_PROGRESSBAR_PERLEVEL_LENGTH 36
#define PROGRESSBAR_BRIGTH_LEVEL_MAX 6
#define BRIGHT_ICON_B_X	90
#define BRIGHT_ICON_B_Y	20
#define BRIGHT_ICON_VOL_X	102
#define BRIGHT_ICON_VOL_Y	37
#define SET_BTN_TWO_LEFT_RECT     DP2PX_RECT(5,190,110,240)
#define SET_BTN_TWO_RIGHT_RECT    DP2PX_RECT(130,190,240,240)

#else
#define SET_BTN_TWO_LEFT_RECT     DP2PX_RECT(5,190,110,240)
#define SET_BTN_TWO_RIGHT_RECT    DP2PX_RECT(130,190,240,240)

#define SET_BTN_MINUS_RECT     DP2PX_RECT(22,155,80,210)
#define SET_BTN_PLUS_RECT    DP2PX_RECT(168,155,220,210)

#define BRIGHT_IMAGE_X	66
#define BRIGHT_IMAGE_Y	54

#define BRIGHT_ICON_X	104
#define BRIGHT_ICON_Y	165
#endif

#endif
/*****************************************************************************/
//  Description : Set Anim effect menu name 
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetAnimLabelParam(void);
#endif
#if defined MMISET_WALLPAPER_SUPPORT
/*****************************************************************************/
//  Description : Set Wallpaper menu name 
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetWallPaperParam(void);
#endif
#if defined MMISET_THEME_SUPPORT
/*****************************************************************************/
//  Description : set theme content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetThemeParam(void);
#endif
#if defined MMISET_IDLE_STYLE_SUPPORT
/*****************************************************************************/
//  Description : set idle style content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetIdleStyleParam(void);
#endif
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
/*****************************************************************************/
//  Description : set idle style content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetIdleDisplaySettingParam(void);
#endif
#endif
/*****************************************************************************/
//  Description : set contrast lable name
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetContrastParam(void);
#if defined MMISET_BACKLIGHT_TIME_SUPPORT
/*****************************************************************************/
//  Description : set backlight param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetBacklightParam(void);
#endif
#if defined MMISET_KEYBACKLIGHT_SUPPORT
/*****************************************************************************/
//  Description : set Keyboard light param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetKeyboardLightParam(void);
#endif
#if defined MMISET_FONT_SUPPORT /* lint_lai */
/*****************************************************************************/
//  Description : set font content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetFontParam(void);
#endif
#if !defined FLASH_SUPPORT && !defined MMI_PDA_SUPPORT
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : set mainmenu effect param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetMainmenu_EffectParam(void);
#endif
#endif
#if !defined MMI_PDA_SUPPORT && defined(EFFECT_3D_LIST_ROLL_SUPPORT)
/*****************************************************************************/
//  Description : set list effect param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetListEffectParam(void);
#endif
#ifdef EFFECT_LCD_ROTATE_SUPPORT
/*****************************************************************************/
//  Description : set g-sensor effect param
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void SetGsensor_EffectParam(void);
#endif
#ifdef MMISET_ANIM_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : set power on param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetPowerOnEffectParam(void);


/*****************************************************************************/
//  Description : set power off param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetPowerOffEffectParam(void);
#endif
#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER)
/*****************************************************************************/
//  Description : set go effect param
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetRenderEffectParam(void);
#endif
#ifdef MMISET_ANIM_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : save all anim set feature
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SaveAllAnimSet(void);



/*****************************************************************************/
//  Description : Handle the window of power on animation setting.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetPowerOnAnimWindow(
                                               MMI_WIN_ID_T       win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );

/*****************************************************************************/
//  Description : Handle the window of power off animation setting.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetPowerOffAnimWindow(
                                                MMI_WIN_ID_T       win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param
                                                );
 
/*****************************************************************************/
//  Description : Append the power on off setting items.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void AppendPowerOnOffAnimItem(
                                    MMI_CTRL_ID_T   ctrl_id,
                                    BOOLEAN         is_power_on_off
                                    );
#endif
                                    
#ifdef SET_3D_STYLE_SUPPORT 
/*****************************************************************************/
//  Description : set 3d interface param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void Set3D_EffectParam(void);
#endif
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
/*****************************************************************************/
//  Description : to handle edit idle words win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditIdleWordsWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              );   
                                              
#endif
/*****************************************************************************/
//  Description : save all setting
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SaveAllFeatureState(void);
#if defined DPHONE_SUPPORT
/*****************************************************************************/
//  Description : SetContraseFormParam
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void SetContrastFormParam(void);
/*****************************************************************************/
//  Description : to handle set backlight
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetBacklightWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );

/*****************************************************************************/
//  Description : to handle set constrast
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetConstrastWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );      
                                             

#endif
#if defined MMISET_KEY_BOARD_LIGHT_SUPPORT 
/*****************************************************************************/
//  Description : to handle select keyborad light
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetKeyboardLightWindow(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 );
#endif
#if defined MMISET_FONT_SUPPORT
/*****************************************************************************/
//  Description : get font info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFontInfo( 
                          wchar * font_name_ptr
                          );

/*****************************************************************************/
//  Description : set font info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetFontInfo(
                          const wchar* font_name_ptr
                          );

/*****************************************************************************/
//  Description : set font
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetFontWindow(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );
#endif 
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
/*****************************************************************************/
//  Description : set main menu style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetMainmenuStyle(
                            MMI_CTRL_ID_T       ctrl_id
                            );
#endif                            
/*****************************************************************************/
//  Description : set idle style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetIdleStyle(
                           MMI_CTRL_ID_T  ctrl_id
                           );                                                                                                                                                                                                                                                 
#ifdef MMISET_ANIM_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : to handle display setting animation window message
//  Global resource dependence : 
//  Author:applezhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetDisplayAnimationWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    );         
#endif

#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER)
/*****************************************************************************/
//  Description : to handle display setting go animation window message
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetLightShadowAnimationWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    );
#endif
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT                                
/*****************************************************************************/
//  Description : to handle select idle display mode
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetIdleDisplayWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );   
#endif  
#endif                                                 
/*****************************************************************************/
//  Description : to handle display setting window message
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetDisplayWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );
#if defined MMISET_MAINMENU_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : SetMainmenuEffectParam
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetMainmenuEffectParam(void);
#endif

#if defined MMISET_UIDLE_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : SetMainmenuEffectParam
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void SetUIdleEffectParam(void);
#endif
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
/*****************************************************************************/
//  Description : SetMainmenuEffectParam
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetMainmenuSelectEffectParam(void);

#endif


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:James li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerOffSelectWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:James li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBrightnessWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );
LOCAL MMI_RESULT_E HandleVolumeWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

LOCAL MMI_RESULT_E HandleCallVolumeSetWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

LOCAL MMI_RESULT_E HandleRingVolumeSetWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:James li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerRestartSelectWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:James li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWatchInfoSelectWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetBacklightCrWindow(
                                               MMI_WIN_ID_T       win_id,
                                               MMI_MESSAGE_ID_E   msg_id,
                                               DPARAM             param
                                               );

//2015-7-9 tangting modify settings-display ui
/*****************************************************************************/
//  Description : to handle blacklight window message
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetDisplayBlacklightWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );


/*****************************************************************************/
//  Description : to handle blacklight time out window message
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetDisplayBlacklightTimeOutWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

PUBLIC void MMIAPISET_CallVolumeWin(void);
PUBLIC void MMIAPISET_RingToneVolumeWin(void);

//#define ZTE_WATCH

#ifdef  ZTE_WATCH
//#if 1

extern PUBLIC void Settings_CallRingSelectWin_Enter( void );

LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_ST   list_item_callvolume    = {text_call_volume_setting };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_ST   list_item_media_volume    = {TXT_ZDT_SET_MM_VOLUME };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_ST   list_item_callring    = {TXT_SET_RING_SET };

LOCAL WATCHCOM_LIST_ITEM__ST s_settings_volume_text_list_data[] =
{
	{ WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR, &list_item_media_volume,  MMIAPISET_RingToneVolumeWin},
	{ WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR, &list_item_callvolume, MMIAPISET_CallVolumeWin},
	{ WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR, &list_item_callring, Settings_CallRingSelectWin_Enter},
   
};
#else

LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_callvolume    = { IMAGE_CALL_VOLUME_ICON,     text_call_volume_setting };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_ringtonevolume    = { IMAGE_RINGTONE_SET_ICON,     text_ringtone_volume_setting };

LOCAL WATCHCOM_LIST_ITEM__ST s_settings_volume_text_list_data[] =
{
	{ WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_callvolume, MMIAPISET_CallVolumeWin/*Settings_BrightnessWin_Enter*/ },
	{ WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_ringtonevolume,  MMIAPISET_RingToneVolumeWin},
   
};
#endif

/*****************************************************************************/  
#if defined MMISET_FONT_SUPPORT
// the window for select font
WINDOW_TABLE( MMISET_SET_FONT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetFontWindow ),    
        WIN_ID( MMISET_SET_FONT_WIN_ID ),
        WIN_TITLE( TXT_SET_FONT ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_FONT_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};
#endif

#if defined MMISET_KEY_BOARD_LIGHT_SUPPORT 
// the window for select keyboard light time
WINDOW_TABLE( MMISET_SET_KEYBOARD_LIGHT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetKeyboardLightWindow ),    
        WIN_ID( MMISET_SET_KEYBOARD_LIGHT_WIN_ID ),
        WIN_TITLE( TXT_SET_KEY_BOARD_LIGHT ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_KEYBOARD_LIGHT_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
#endif
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
//the window for set idle win display 
WINDOW_TABLE( MMISET_SET_IDLE_DISPLAY_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetIdleDisplayWindow ),    
        WIN_ID( MMISET_SET_IDLE_DISPLAY_WIN_ID ),
        WIN_TITLE( TXT_SET_IDLE_DISPLAY ),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_SET_IDLE_DISPLAY_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
#ifdef 	GUI_FULLSCREEN_POPMENU_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
#endif
        END_WIN
};
#endif
#endif
//@xingdong.li
#ifdef MMISET_ANIM_EFFECT_SUPPORT//yongsheng.wang add
// the window for select power on animation
WINDOW_TABLE( MMISET_SET_POWER_ON_ANIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetPowerOnAnimWindow ),    
        WIN_ID( MMISET_SET_POWER_ON_ANIM_WIN_ID ),
        WIN_TITLE( TXT_COMMON_POWER_ON_ANIMATION ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_POWER_ON_ANIM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};

//@xingdong.li
// the window for select power off animation
WINDOW_TABLE( MMISET_SET_POWER_OFF_ANIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetPowerOffAnimWindow ),    
        WIN_ID( MMISET_SET_POWER_OFF_ANIM_WIN_ID ),
        WIN_TITLE( TXT_COMMON_POWER_OFF_ANIMATION ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_POWER_OFF_ANIM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        END_WIN
};
#endif
#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER)
WINDOW_TABLE( MMISET_SET_RENDER_EFFECT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_SET_LIGHT_SHADOW_RENDER_ANIMATION ),
        WIN_FUNC( (uint32)HandleSetLightShadowAnimationWindow ),    
        WIN_ID( MMISET_SET_DISPLAY_RENDER_ANIMATION_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
        CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMISET_DISPLAY_ANIM_RENDER_CHECKLIST_CTRL_ID),
        END_WIN
};

#endif
#ifdef MMISET_ANIM_EFFECT_SUPPORT
//九宫格模式下的动画效果
WINDOW_TABLE( MMISET_SET_DISPLAY_ICON_ANIMATION_WIN_TAB ) = 
{
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetDisplayAnimationWindow ),    
        WIN_ID( MMISET_SET_DISPLAY_ICON_ANIMATION_WIN_ID ),
        WIN_TITLE(TXT_SET_ANIMATION),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
#ifdef SET_3D_STYLE_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_3D_EFFECT_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_3D_EFFECT_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_3D_EFFECT_FORM_CTRL_ID),//界面特效
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID,MMISET_DISPLAY_ANIM_3D_EFFECT_FORM_CTRL_ID),
#endif
#if !defined FLASH_SUPPORT && !defined MMI_PDA_SUPPORT
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID),//主菜单特效
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_SETLIST_CTRL_ID,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID),
#endif
#endif
#if !defined MMI_PDA_SUPPORT
#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_LIST_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_LIST_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_LIST_FORM_CTRL_ID),//列表特效
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ANIM_LIST_SETLIST_CTRL_ID,MMISET_DISPLAY_ANIM_LIST_FORM_CTRL_ID),
#endif
#endif    
          

#ifdef EFFECT_LCD_ROTATE_SUPPORT // 说明支持横竖屏切换
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_G_SENSOR_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_G_SENSOR_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_G_SENSOR_FORM_CTRL_ID),//g-sensor切换
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ANIM_G_SENSOR_SETLIST_CTRL_ID,MMISET_DISPLAY_ANIM_G_SENSOR_FORM_CTRL_ID),
#endif

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_POWER_ON_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_POWER_ON_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_ON_FORM_CTRL_ID),//开关机
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_ANIM_POWER_ON_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_ON_FORM_CTRL_ID),//开关机

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_POWER_OFF_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_OFF_FORM_CTRL_ID),//开关机
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_OFF_FORM_CTRL_ID),//开关机
#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_RENDER_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_RENDER_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_RENDER_FORM_CTRL_ID),//开关机
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_ANIM_RENDER_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_ANIM_RENDER_FORM_CTRL_ID),//开关机
#endif
        END_WIN
};                                         
#endif
#if 0//def QBTHEME_SUPPORT
//九宫格模式下的动画效果
WINDOW_TABLE( MMISET_SET_QB_DISPLAY_ICON_ANIMATION_WIN_TAB ) = 
{
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetDisplayAnimationWindow ),    
        WIN_ID( MMISET_SET_DISPLAY_ICON_ANIMATION_WIN_ID ),
        WIN_TITLE(TXT_SET_ANIMATION),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),

#if !defined FLASH_SUPPORT && !defined MMI_PDA_SUPPORT
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID),//主菜单特效
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_SETLIST_CTRL_ID,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID),
#endif
#endif
#if !defined MMI_PDA_SUPPORT
#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_LIST_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_LIST_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_LIST_FORM_CTRL_ID),//列表特效
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ANIM_LIST_SETLIST_CTRL_ID,MMISET_DISPLAY_ANIM_LIST_FORM_CTRL_ID),
#endif
#endif    
          

#ifdef EFFECT_LCD_ROTATE_SUPPORT // 说明支持横竖屏切换
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_G_SENSOR_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_G_SENSOR_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_G_SENSOR_FORM_CTRL_ID),//g-sensor切换
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_ANIM_G_SENSOR_SETLIST_CTRL_ID,MMISET_DISPLAY_ANIM_G_SENSOR_FORM_CTRL_ID),
#endif

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_POWER_ON_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_POWER_ON_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_ON_FORM_CTRL_ID),//开关机
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_ANIM_POWER_ON_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_ON_FORM_CTRL_ID),//开关机

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_POWER_OFF_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_OFF_FORM_CTRL_ID),//开关机
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_ANIM_POWER_OFF_FORM_CTRL_ID),//开关机
        
        END_WIN
};      
#endif
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
//the window of edit idle display words
WINDOW_TABLE( MMISET_EDIT_IDLE_WOEDS_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEditIdleWordsWindow ),    
        WIN_ID( MMISET_EDIT_IDLE_WOEDS_WIN_ID ),
        WIN_TITLE( TXT_SET_CUSTOM_WORD ),

#if !defined MMI_GUI_STYLE_TYPICAL
		CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISET_IDLE_WORDS_FORM_CTRL_ID),
		CHILD_EDIT_TEXT_CTRL(TRUE, MMIENVSET_ENVNAME_MAX_LEN, MMISET_IDLE_WORDS_EDITCTRL_ID, MMISET_IDLE_WORDS_FORM_CTRL_ID),
#else
        CREATE_EDIT_TEXT_CTRL(0,MMISET_IDLE_WORDS_EDITCTRL_ID),
#endif
#ifdef MMI_PDA_SUPPORT
		CHILD_SOFTKEY_CTRL(STXT_SAVE, TXT_NULL, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,MMISET_IDLE_WORDS_FORM_CTRL_ID),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        END_WIN
};
#endif
#endif
#if defined DPHONE_SUPPORT
// the window for set constrast
WINDOW_TABLE( MMISET_SET_CONSTRAST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetConstrastWindow ),    
    WIN_ID( MMISET_SET_CONSTRAST_WIN_ID ),
    WIN_TITLE( TXT_SET_CONTRAST ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //create down button ctrl
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS,MMISET_CONTRAST_FROM_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_SET_CONTRAST_DOWN_RELEASED,MMISET_CONTRAST_DOWN_BUTTON_CTRL_ID,MMISET_CONTRAST_FROM_ID),
        CHILD_ANIM_CTRL(FALSE,FALSE,MMISET_CONTRAST_DOWN_BAR_ID,MMISET_CONTRAST_FROM_ID),
    //create up button ctrl
        CHILD_BUTTON_CTRL(TRUE,IMAGE_SET_CONTRAST_UP_RELEASED,MMISET_CONTRAST_UP_BUTTON_CTRL_ID,MMISET_CONTRAST_FROM_ID),
    END_WIN
};

// the window for set backlight
WINDOW_TABLE( MMISET_SET_BACKLIGHT_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetBacklightWindow ),    
        WIN_ID( MMISET_SET_BACKLIGHT_WIN_ID ),
        WIN_TITLE( TXT_SET_BACK_LIGHT ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_BACKLIGHT_CTRL_ID),
       // WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),                     
        END_WIN
};


// the window for display setting 
WINDOW_TABLE( MMISET_SET_DISPLAY_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetDisplayWindow ),    
    WIN_ID( MMISET_SET_DISPLAY_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    //WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_SET_DISPLAY_OPT, MMISET_SET_DISPLAY_CTRL_ID),
    END_WIN
};

#else
WINDOW_TABLE( MMISET_SET_DISPLAY_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetDisplayWindow ),    
        WIN_ID( MMISET_SET_DISPLAY_WIN_ID ),
        WIN_TITLE(TXT_NULL),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_FORM_CTRL_ID),
#if defined MMISET_ANIM_EFFECT_SUPPORT        
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_ANIM_SET_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_DISPLAY_ANIM_LABEL_CTRL_ID,MMISET_DISPLAY_ANIM_SET_FORM_CTRL_ID),//动画
#endif
#if defined MMISET_WALLPAPER_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_WALLPAPER_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_WALLPAPER_LABEL_CTRL_ID,MMISET_DISPLAY_WALLPAPER_FORM_CTRL_ID),//墙纸
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_WALLPAPER_FORM_CTRL_ID),
#endif
#if defined MMISET_THEME_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_THEME_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_THEME_LABEL_CTRL_ID,MMISET_DISPLAY_THEME_FORM_CTRL_ID),//主题
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_THEME_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_THEME_FORM_CTRL_ID),
#endif    
#if defined MMISET_MAINMENU_EFFECT_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_MAINMENU_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_MAINMENU_LABEL_CTRL_ID,MMISET_DISPLAY_MAINMENU_FORM_CTRL_ID),//主菜单风格
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_MAINMENU_CONTENT_CTRL_ID,MMISET_DISPLAY_MAINMENU_FORM_CTRL_ID), 
#endif 
#if defined MMISET_UIDLE_EFFECT_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_UIDLE_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_UIDLE_LABEL_CTRL_ID,MMISET_DISPLAY_UIDLE_FORM_CTRL_ID),//uidle
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_UIDLE_CONTENT_CTRL_ID,MMISET_DISPLAY_UIDLE_FORM_CTRL_ID), 
#endif 
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_MAINMENU_SELECT_STYLE_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_MAINMENU_SELECT_STYLE_LABEL_CTRL_ID,MMISET_MAINMENU_SELECT_STYLE_FORM_CTRL_ID),//主菜单高亮风格
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_MAINMENU_SELECT_STYLE_CONTENT_CTRL_ID,MMISET_MAINMENU_SELECT_STYLE_FORM_CTRL_ID), 
#endif

#if defined MMISET_IDLE_STYLE_SUPPORT
            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_IDLE_STYLE_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_IDLE_STYLE_LABEL_CTRL_ID,MMISET_DISPLAY_IDLE_STYLE_FORM_CTRL_ID),//待机风格
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID,MMISET_DISPLAY_IDLE_STYLE_FORM_CTRL_ID),
#endif
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_IDLE_DISPLAY_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISET_DISPLAY_IDLE_DISPLAY_LABEL_CTRL_ID,MMISET_DISPLAY_IDLE_DISPLAY_FORM_CTRL_ID),//待机设置
#endif
#endif   
#if defined MMISET_KEYLOCK_SUPPORT
            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_KEY_LOCK_EFFECT_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_KEY_LOCK_EFFECT_LABEL_CTRL_ID,MMISET_DISPLAY_KEY_LOCK_EFFECT_FORM_CTRL_ID),//锁屏
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_KEY_LOCK_EFFECT_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_KEY_LOCK_EFFECT_FORM_CTRL_ID),  
#endif
#if defined MMISET_SCREENSAVER_SUPPORT 
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_SCREEN_SAVERS_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_SCREEN_SAVERS_LABEL_CTRL_ID,MMISET_DISPLAY_SCREEN_SAVERS_FORM_CTRL_ID),//屏保
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_SCREEN_SAVERS_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_SCREEN_SAVERS_FORM_CTRL_ID),
#endif            

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_CONTRAST_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_CONTRAST_CTRL_ID,MMISET_DISPLAY_CONTRAST_FORM_CTRL_ID),//屏幕背光亮度
            CHILD_OWNDRAW_CTRL(TRUE,MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID,MMISET_DISPLAY_CONTRAST_FORM_CTRL_ID,(uint32)MMISET_ContrastBarCtrlCallbackFun),

#if defined MMISET_BACKLIGHT_TIME_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_BACKLIGH_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_BACKLIGHT_CTRL_ID,MMISET_DISPLAY_BACKLIGH_FORM_CTRL_ID),//背光时间
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID,MMISET_DISPLAY_BACKLIGH_FORM_CTRL_ID),
#endif  
#if defined MMISET_KEYBACKLIGHT_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_KEYPAD_BACKLIGH_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_KEYPAD_BACKLIGHT_CTRL_ID,MMISET_DISPLAY_KEYPAD_BACKLIGH_FORM_CTRL_ID),//按键背光时间
            CHILD_SETLIST_CTRL(TRUE,MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID,MMISET_DISPLAY_KEYPAD_BACKLIGH_FORM_CTRL_ID),
#endif
#if defined MMISET_FONT_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_DISPLAY_FONT_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_DISPLAY_FONT_SETTING_CTRL_ID,MMISET_DISPLAY_FONT_FORM_CTRL_ID),//字体设置
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT,TRUE,MMISET_DISPLAY_FONT_SETTING_LABEL_EDIT_CTRL_ID,MMISET_DISPLAY_FONT_FORM_CTRL_ID),
#endif
#ifdef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISET_SECURITY_SET_KEYPAD_LOCK_FORM_CTRL_ID,MMISET_DISPLAY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISET_SECURITY_AUTO_KEYPADLOCK_LABEL_CTRL_ID,MMISET_SECURITY_SET_KEYPAD_LOCK_FORM_CTRL_ID),//AUTO KEYPAD LOCK
            CHILD_SETLIST_CTRL(TRUE,MMISET_SECURITY_AUTO_KEYPADLOCK_SETLIST_CTRL_ID,MMISET_SECURITY_SET_KEYPAD_LOCK_FORM_CTRL_ID),//AUTO KEYPAD LOCK
#endif
        END_WIN
};
#endif     

//pclint

//blacklight timeout
WINDOW_TABLE(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_WIN_TAB) =
{
    WIN_FUNC( (uint32)HandleSetDisplayBlacklightTimeOutWindow ),    
    WIN_ID( MMISET_DISPLAY_BACKLIGHT_TIMEOUT_WIN_ID ),
    //nhl mmodify back button
    //WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMISET_SET_POWEROFF_WIN_TAB) =
{
    //WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandlePowerOffSelectWindow),
    WIN_ID(MMISET_SET_POWEROFF_WIN_ID),
    WIN_HIDE_STATUS,
    //WIN_SOFTKEY(NULL, TXT_COMMON_OK, NULL),//yangyu delete
    //WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    END_WIN
};

WINDOW_TABLE(MMISET_SET_BRIGHTNESS_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleBrightnessWindow),
    WIN_ID(MMISET_SET_BRIGHTNESS_WIN_ID),
#ifdef ZTE_WATCH
    CREATE_BUTTON_CTRL( IMAGE_COMMON_ONEBTN_BG_IMAG, MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID),
#else
    CREATE_BUTTON_CTRL( IMAGE_BRIGHT_MINUS_ICON, MMISET_COMMON_L_BTN_CTRL_ID ),
    CREATE_BUTTON_CTRL( IMAGE_BRIGHT_PLUS_ICON, MMISET_COMMON_R_BTN_CTRL_ID ),
#endif
    WIN_HIDE_STATUS,
    END_WIN
};

WINDOW_TABLE(MMISET_SET_VOLUME_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_STYLE(WS_NO_DEFAULT_STYLE),
    WIN_FUNC((uint32)HandleVolumeWindow),
    WIN_ID(MMISET_SET_VOLUME_WIN_ID),
    //CREATE_MENU_CTRL(MENU_VOLUME_SETTINGS_ENTRY, MMISET_SET_VOLUME_CTRL_ID),
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMISET_SET_VOLUME_CTRL_ID ),
    END_WIN
};

WINDOW_TABLE(MMISET_CALL_VOLUME_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_STYLE(WS_NO_DEFAULT_STYLE),
    WIN_FUNC((uint32)HandleCallVolumeSetWindow),
    WIN_ID(MMISET_SET_CALL_VOLUME_WIN_ID),
#ifdef ZTE_WATCH
    CREATE_BUTTON_CTRL( IMAGE_COMMON_ONEBTN_BG_IMAG, MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID),
#else
    CREATE_BUTTON_CTRL( IMAGE_VOLUME_MINUS_ICON, MMISET_COMMON_L_BTN_CTRL_ID ),
    CREATE_BUTTON_CTRL( IMAGE_VOLUME_PLUS_ICON, MMISET_COMMON_R_BTN_CTRL_ID ),
#endif
    END_WIN
};

WINDOW_TABLE(MMISET_RINGTONE_VOLUME_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_STYLE(WS_NO_DEFAULT_STYLE),
    WIN_FUNC((uint32)HandleRingVolumeSetWindow),
    WIN_ID(MMISET_SET_RINGTONE_VOLUME_WIN_ID),
 #ifdef ZTE_WATCH
    CREATE_BUTTON_CTRL( IMAGE_COMMON_ONEBTN_BG_IMAG, MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID),
#else
    CREATE_BUTTON_CTRL( IMAGE_BRIGHT_MINUS_ICON, MMISET_COMMON_L_BTN_CTRL_ID ),
    CREATE_BUTTON_CTRL( IMAGE_BRIGHT_PLUS_ICON, MMISET_COMMON_R_BTN_CTRL_ID ),
#endif
    END_WIN
};
WINDOW_TABLE(MMISET_SET_POWER_RESTART_WIN_TAB) =
{
    //WIN_STATUSBAR,
     WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandlePowerRestartSelectWindow),
    WIN_ID(MMISET_SET_POWER_RESTART_WIN_ID),
    //WIN_SOFTKEY(NULL, TXT_COMMON_OK, NULL),
    //WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    #ifdef ZTE_WATCH
    CREATE_BUTTON_CTRL( res_common_set_confirm, MMISET_BRIGHTNESS_L_BTN_CTRL_ID ),
    CREATE_BUTTON_CTRL( res_common_set_cancel, MMISET_BRIGHTNESS_R_BTN_CTRL_ID ),
    #else
    CREATE_BUTTON_CTRL( NULL, MMISET_BRIGHTNESS_L_BTN_CTRL_ID ),
    CREATE_BUTTON_CTRL( NULL, MMISET_BRIGHTNESS_R_BTN_CTRL_ID ),
    #endif
    CREATE_TEXT_CTRL(MMISET_SETTIPS_TEXT_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMISET_SET_WATCH_INFO_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleWatchInfoSelectWindow),
    WIN_ID(MMISET_SET_WATCH_INFO_WIN_ID),
    CREATE_TEXT_CTRL(MMIZDT_WATCHINFO_TEXT_ID),
    //WIN_SOFTKEY(NULL, TXT_COMMON_OK, NULL),

    END_WIN
};

#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
/*****************************************************************************/
// 	Description : set idle win display mode
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetDisplaytIdleMode(void)
{
    //write information about idle win display set into NV
    MMINV_WRITE(MMINV_SET_IDLEDISPLAY_INFO,MMIAPISET_GetIdleDisplayMode());
}
#endif 
#endif
/*****************************************************************************/
//  Description : to handle display setting window message
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
#if defined DPHONE_SUPPORT 
LOCAL MMI_RESULT_E  HandleSetDisplayWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_MENU_GROUP_ID_T     group_id    =   0;
    MMI_MENU_ID_T           menu_id     =   0;
    MMI_RESULT_E            recode      =   MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMISET_SET_DISPLAY_CTRL_ID); 
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(MMISET_SET_DISPLAY_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
        case ID_SET_IDLE_DISPLAY:
            MMK_CreatePubListWin((uint32*)MMISET_SET_IDLE_DISPLAY_WIN_TAB,PNULL);
            break;
#endif
#endif
        case ID_SET_BACK_LIGHT:
            MMK_CreateWin((uint32*)MMISET_SET_BACKLIGHT_WIN_TAB,PNULL);
            break;
            
        case ID_SET_CONTRAST:
            MMK_CreateWin((uint32*)MMISET_SET_CONSTRAST_WIN_TAB,PNULL);
            break;       
        case ID_SET_KEY_BOARD_LIGHT:
            MMK_CreateWin((uint32*)MMISET_SET_KEYBOARD_LIGHT_WIN_TAB,PNULL);
            break; 
        default:
            //SCI_PASSERT(SCI_FALSE, ("HandleSetDisplayWindow! MSG_CTL_OK menu_id = %d", menu_id));
            break;
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
    
    return recode;
}
#else
LOCAL MMI_RESULT_E  HandleSetDisplayWindow(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_DISPLAY_ANIM_LABEL_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_PDA_SUPPORT
		GUIFORM_SetCircularHandleUpDown(MMISET_DISPLAY_FORM_CTRL_ID,TRUE);
#endif
#if !defined(MMI_GUI_STYLE_TYPICAL) 
        GUIFORM_SetType(MMISET_DISPLAY_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif
#ifndef MMI_DISPLAYSET_MINI_SUPPORT 
        GUIFORM_SetMargin(MMISET_DISPLAY_FORM_CTRL_ID, 0);
#if defined MMISET_ANIM_EFFECT_SUPPORT
        SetAnimLabelParam();
#endif
#endif
#if defined MMISET_WALLPAPER_SUPPORT
        SetWallPaperParam();
#endif
#if defined MMISET_THEME_SUPPORT
        SetThemeParam();
#endif   
#if defined MMISET_MAINMENU_EFFECT_SUPPORT
        if(1 < MMISET_PDA_MAINMENU_MAX)/*lint !e506 !e774*///可能存在恒为FALSE或恒为TRUE的情况
        {
            SetMainmenuEffectParam();
        }
        else
        {
            GUIFORM_SetChildDisplay(MMISET_DISPLAY_FORM_CTRL_ID, 
                                    MMISET_DISPLAY_MAINMENU_FORM_CTRL_ID, 
                                    GUIFORM_CHILD_DISP_HIDE);
        }
#endif
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        SetMainmenuSelectEffectParam();
#endif
#if defined MMISET_UIDLE_EFFECT_SUPPORT
        SetUIdleEffectParam();
#endif
#if defined MMISET_IDLE_STYLE_SUPPORT
        SetIdleStyleParam();
#endif
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
        SetIdleDisplaySettingParam();
#endif
#endif
#if defined MMISET_FONT_SUPPORT
        SetFontParam();
#endif    
#if defined MMISET_SCREENSAVER_SUPPORT 
        SetScreenSaverParam();
#endif
        SetContrastParam();
#if defined MMISET_BACKLIGHT_TIME_SUPPORT
        SetBacklightParam();
#endif
#if defined MMISET_KEYBACKLIGHT_SUPPORT
        SetKeyboardLightParam();
#endif
#if defined MMISET_KEYLOCK_SUPPORT
        if( 1 < MMISET_KEY_LOCK_EFFECT_MAX )/*lint !e506 !e774*///可能存在恒为FALSE或恒为TRUE的情况
        {
            MMISET_SetKeyLockParam();
        }
        else
        {
            GUIFORM_SetChildDisplay(MMISET_DISPLAY_FORM_CTRL_ID, 
                                    MMISET_DISPLAY_KEY_LOCK_EFFECT_FORM_CTRL_ID, 
                                    GUIFORM_CHILD_DISP_HIDE);
        }
#endif
#ifdef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
        MMISET_SetAutoLockKeyPadList();
#endif

#ifndef MMI_DISPLAYSET_MINI_SUPPORT 
        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_FORM_CTRL_ID);
#else
        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_WALLPAPER_FORM_CTRL_ID);
#endif    
        GUIWIN_SetTitleTextId(win_id,TXT_SETTINGS_DISPLAY,FALSE);
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_FINISH, FALSE);

        break;
        
    case MSG_GET_FOCUS:
#if defined MMISET_WALLPAPER_SUPPORT
        SetWallPaperParam();
#endif
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        SetMainmenuSelectEffectParam();
#endif
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if (MSG_CTL_PENOK == msg_id)
        {
            ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
        }
        else
        {
            ctrl_id= MMK_GetActiveCtrlId(win_id);
        }
        
        switch (ctrl_id)
        {
#ifndef MMI_DISPLAYSET_MINI_SUPPORT  
#if defined MMISET_ANIM_EFFECT_SUPPORT
        case MMISET_DISPLAY_ANIM_LABEL_CTRL_ID:
#if 0//def QBTHEME_SUPPORT
            {
                MMISET_IDLE_STYLE_E cur_idle_style = MMIAPISET_GetIdleStyle();

                if (MMISET_IDLE_QBTHEME == cur_idle_style)
                {
                    MMK_CreateWin((uint32 *)MMISET_SET_QB_DISPLAY_ICON_ANIMATION_WIN_TAB, PNULL);
                    break;
                }
            }
#endif
            MMK_CreateWin((uint32 *)MMISET_SET_DISPLAY_ICON_ANIMATION_WIN_TAB, PNULL);

#endif
            break;  
#endif
#if defined MMISET_WALLPAPER_SUPPORT
        case MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID:
           MMIAPISET_SetWallpaper();
            break;
#endif
//#ifdef MMI_MULTI_MAINMENU_SUPPORT
#if defined MMISET_MAINMENU_EFFECT_SUPPORT
        case MMISET_DISPLAY_MAINMENU_CONTENT_CTRL_ID:
            MMIAPISET_CreateMainmenuStyleWin();
            break;
#endif
//#endif
#if defined( PDA_UI_SUPPORT_U_IDLE)
        case MMISET_DISPLAY_UIDLE_CONTENT_CTRL_ID:
            MMIAPISET_CreateUIdleStyleWin();
            break;
#endif
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
        case MMISET_MAINMENU_SELECT_STYLE_CONTENT_CTRL_ID:
            MMIAPISET_CreateMainmenuSelectStyleWin();
            break;
#endif 
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
        case MMISET_DISPLAY_IDLE_DISPLAY_LABEL_CTRL_ID:
            MMK_CreatePubListWin((uint32*)MMISET_SET_IDLE_DISPLAY_WIN_TAB,PNULL);
            break;
#endif
#endif
#if defined MMISET_KEYLOCK_SUPPORT
        case MMISET_DISPLAY_KEY_LOCK_EFFECT_LABEL_EDIT_CTRL_ID:
            MMIAPISET_CreateLockStyleWin();//NEWMS00204601
            //MMISET_CreateKeyLockEffectWin();
            break;
#endif
#if defined MMISET_THEME_SUPPORT
        case MMISET_DISPLAY_THEME_LABEL_EDIT_CTRL_ID:
            MMISET_CreateThemePubWin();
            break;
#endif
#ifdef TOUCH_PANEL_SUPPORT
        case MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID:
            if (MSG_CTL_PENOK == msg_id)
            {
                GUI_POINT_T     tp_point = {0};
                MMI_TP_STATUS_E tp_status = MMI_TP_NONE;

                MMK_GetMultiTPMsg(&tp_status,&tp_point);

                MMISET_HandleContrastTPDown(&tp_point, 
                                            win_id,
                                            MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID,
                                            FALSE);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            break;
#endif
#if defined MMISET_SCREENSAVER_SUPPORT 
        case MMISET_DISPLAY_SCREEN_SAVERS_LABEL_EDIT_CTRL_ID:
            MMISETAPI_CreateScreensaverWin();
            break;
#endif
#if defined MMISET_FONT_SUPPORT        
        case MMISET_DISPLAY_FONT_SETTING_LABEL_EDIT_CTRL_ID:
            MMK_CreatePubListWin((uint32*)MMISET_SET_FONT_WIN_TAB, PNULL);
            break;
  #endif
            
        default:
            //SCI_TRACE_LOW:"HandleSetDisplayWindow ctrl_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_862_112_2_18_2_51_0_170,(uint8*)"d",ctrl_id);
            break;
        }

        break;

    case MSG_NOTIFY_SETLIST_SWITCH:
        {
            MMI_CTRL_ID_T  act_ctrl_id = MMK_GetActiveCtrlId(win_id);

            switch(act_ctrl_id)
            {
#ifdef MMISET_KEYBACKLIGHT_SUPPORT
            case MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID:
                MMISET_SetKeyboardLight(act_ctrl_id);
                break;
#endif

#ifdef MMISET_BACKLIGHT_TIME_SUPPORT    
            case MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID:
                MMISET_SetBackLight(act_ctrl_id);
                break;
#endif

#ifdef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
            case MMISET_SECURITY_AUTO_KEYPADLOCK_SETLIST_CTRL_ID:
                MMISET_SaveAutoKeyPadList();
				MMIDEFAULT_StartLcdBackLightTimer();//CR140422
                break;
#endif
            default:
                break;
            }

        }
        break;
        
    case MSG_KEYUP_DOWN:
    case MSG_KEYUP_UP:
    case MSG_KEYLONG_UP:
    case MSG_KEYLONG_DOWN:
        if(MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_FINISH, TRUE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_FINISH, TRUE);
        }
        break;
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
        if(MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            MMISET_TuneUpConstrast(msg_id);
            //display contrast picutres
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        }
        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_KEYDOWN_RED:
        break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        SaveAllFeatureState();
        break;
            
    default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}
#endif 


//yangyu add
LOCAL uint16 sr_tp_down_x = 0;
#define MT_ABS(x) ((x) < 0 ? ((x) * (-1)) : (x))


typedef enum
{
	REBOOT_STYLE_PWROFF_RESTART_ONE_PAGE,		
	REBOOT_STYLE_PWROFF_ONLY,		
	REBOOT_STYLE_RESTART_ONLY,
	REBOOT_STYLE_RESTORE_ONLY,	
	REBOOT_STYLE__MAX
}REBOOT_SYLE;

REBOOT_SYLE g_reboot_screen_show_style=REBOOT_STYLE_PWROFF_RESTART_ONE_PAGE;

static void mmi_set_reboot_screen_show_style(REBOOT_SYLE style)
{
    g_reboot_screen_show_style=style;
}

static REBOOT_SYLE mmi_get_reboot_screen_show_style()
{
    return g_reboot_screen_show_style;
}

//yangyu end
/*****************************************************************************/
//  Description :HandleSetSystemSettingWindow
//  Global resource dependence :
//  Author:James li
//  Note:
/*****************************************************************************/ 
LOCAL MMI_RESULT_E HandlePowerOffSelectWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    GUI_RECT_T          rect         = MMITHEME_GetClientRect();
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};    
    
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};
    GUI_POINT_T   point = {0};
    rect.top =  0;
	
    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:
	    break;
	case MSG_FULL_PAINT:	
        rect.bottom = 240;
#if 0
	if(mmi_get_reboot_screen_show_style()==REBOOT_STYLE_PWROFF_ONLY)
	{
		GUIRES_DisplayImg(PNULL,
		    &rect,
		    PNULL,
		    win_id,
		    IMAGE_REBOOT_SHUTDOWN_PWROFF_ONLY_BG,
		    &lcd_dev_info);    
	}
	else if(mmi_get_reboot_screen_show_style()==REBOOT_STYLE_RESTART_ONLY)
	{
		GUIRES_DisplayImg(PNULL,
		    &rect,
		    PNULL,
		    win_id,
		    IMAGE_REBOOT_SHUTDOWN_RESTART_ONLY_BG,
		    &lcd_dev_info);    
	}
	else
	{
		GUIRES_DisplayImg(PNULL,
		    &rect,
		    PNULL,
		    win_id,
		    IMAGE_REBOOT_SHUTDOWN_BG,
		    &lcd_dev_info);    
	}	
#endif
	    break;
		
	case MSG_CTL_MIDSK:
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
	case MSG_APP_WEB:
#if defined( TOUCH_PANEL_SUPPORT)||defined(ZDT_TOUCHPANEL_TYPE_MULTITP) //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	   // MMIAPIPHONE_PowerOff();
	    break;
    case MSG_TP_PRESS_DOWN:
        sr_tp_down_x = MMK_GET_TP_X(param);

	    break;
	case MSG_TP_PRESS_UP:
	    	{
		        
		        point.x = MMK_GET_TP_X(param);
		        point.y = MMK_GET_TP_Y(param);
		        //if((point.x > 80&& point.x <160)&&(point.y > 200 &&  point.y < 240 ))
		        #if 1
				if(mmi_get_reboot_screen_show_style()==REBOOT_STYLE_PWROFF_ONLY)
				{
					if(MT_ABS(point.x - sr_tp_down_x) < 40 && (point.x >= 60 && point.x <= 180)&&(point.y >= 60 &&	point.y <= 180 ))
					{
						MMIAPIPHONE_PowerOff();
					}
				}
				else if(mmi_get_reboot_screen_show_style()==REBOOT_STYLE_RESTART_ONLY)
				{
					if(MT_ABS(point.x - sr_tp_down_x) < 40 && (point.x >= 60 && point.x <= 180)&&(point.y >= 60 &&	point.y <= 180 ))
					{
						MMIAPIPHONE_PowerReset();
					}
				}
				else
				{
					if(MT_ABS(point.x - sr_tp_down_x) < 40*SCALE && (point.x >= 32*SCALE && point.x <= 110*SCALE)&&(point.y >= 60*SCALE &&	point.y <= 136*SCALE ))
					{
						MMIAPIPHONE_PowerOff();
					}
					else if(MT_ABS(point.x - sr_tp_down_x) < 40*SCALE && (point.x >= 132*SCALE && point.x <= 210*SCALE)&&(point.y >= 60*SCALE &&  point.y <= 136*SCALE ))
					{
						MMIAPIPHONE_PowerReset();
					}
				}				
			 #else 	
	     		        if(MT_ABS(point.x - sr_tp_down_x) < 40*SCALE && (point.x >= 32*SCALE && point.x <= 110*SCALE)&&(point.y >= 60*SCALE &&  point.y <= 136*SCALE ))
	     		        {
	     		    		MMIAPIPHONE_PowerOff();
	     		        }
	     		        else if(MT_ABS(point.x - sr_tp_down_x) < 40*SCALE && (point.x >= 132*SCALE && point.x <= 210*SCALE)&&(point.y >= 60*SCALE &&  point.y <= 136*SCALE ))
	     		        {
	     		    		MMIAPIPHONE_PowerReset();
	     		        }
			 #endif 		 
	    	}
	    break;		


		 
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
	    MMK_CloseWin(win_id);
	    break;
    case MSG_KEYDOWN_RED:
        break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;    
	default:
	    recode = MMI_RESULT_FALSE;
	    break;
    }
	return recode;
}

#ifdef ZDT_ZFB_SUPPORT
LOCAL uint32 s_win_tp_count =0;
LOCAL uint32 s_win_tp_ms =0;
#endif

void Brightness_Full_Paint( MMI_WIN_ID_T    win_id)
{
	MMI_RESULT_E            recode = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
	GUI_POINT_T         display_point = {0};
	GUI_RECT_T img_rect = {0};
	GUI_BG_T bg_ptr = {0};

#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)	
	GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
	GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
	MMI_STRING_T        string = {0};	
	GUI_RECT_T          text_rect={42,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 
#endif
	GUI_RECT_T          rect         = MMITHEME_GetFullScreenRectEx(win_id);
#ifdef ZTE_WATCH
	display_point.x = BRIGHT_ICON_B_X; // //43*BIG_SCALE;//81*SCALE;//client_rect.v_rect.left;
	display_point.y = BRIGHT_ICON_B_Y; // // 14*BIG_SCALE;//25*SCALE;///client_rect.v_rect.top;	

	GUIRES_DisplayImg(&display_point,
		PNULL,
		PNULL,
		win_id,
		IMAGE_BRIGHT_ICON_ZTE,
		&lcd_dev_info);
#else

	 MMI_IMAGE_ID_T Bright_Image_Id[6]=
    {
        IMAGE_BRIGHT_LEVEL0,
        IMAGE_BRIGHT_LEVEL1,
        IMAGE_BRIGHT_LEVEL2,
        IMAGE_BRIGHT_LEVEL3,
        IMAGE_BRIGHT_LEVEL4,
        IMAGE_BRIGHT_LEVEL5
    };

	LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)		
	//draw bg		
	GUIRES_DisplayImg(PNULL,
		&rect,
		PNULL,
		win_id,
		IMAGE_THEME_BLACK_BG_SPEC, //xiongkai IMAGE_THEME_BG,
		&lcd_dev_info); 

	///////draw title
	text_style.align = ALIGN_LVMIDDLE;
	text_style.font = SONG_FONT_28;//MMI_DEFAULT_EXTRA_BIG_FONT;
	text_style.font_color = MMI_WHITE_COLOR;
	text_style.char_space = 0;

	MMI_GetLabelTextByLang(TXT_COMMON_COMMON_BRIGHTNESS, &string);
	GUISTR_DrawTextToLCDInRect( 
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		(const GUI_RECT_T      *)&text_rect,       //the fixed display area
		(const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
		(const MMI_STRING_T    *)&string,
		&text_style,
		state,
		GUISTR_TEXT_DIR_AUTO
		);
#endif

	display_point.x = BRIGHT_IMAGE_X; //35*BIG_SCALE;//35*SCALE;//client_rect.v_rect.left;
	display_point.y =  BRIGHT_IMAGE_Y; //   74*BIG_SCALE;//103*SCALE;///client_rect.v_rect.top;		

	GUIRES_DisplayImg(&display_point,
		PNULL,
		PNULL,
		win_id,
		Bright_Image_Id[cur_bright_img_id],
		&lcd_dev_info);	

	display_point.x = BRIGHT_ICON_X; // //43*BIG_SCALE;//81*SCALE;//client_rect.v_rect.left;
	display_point.y = BRIGHT_ICON_Y; // // 14*BIG_SCALE;//25*SCALE;///client_rect.v_rect.top;	

	GUIRES_DisplayImg(&display_point,
		PNULL,
		PNULL,
		win_id,
		IMAGE_BRIGHT_ICON,
		&lcd_dev_info);
#endif
}
/*****************************************************************************/
//  Description :HandleSetSystemSettingWindow
//  Global resource dependence :
//  Author:James li
//  Note:
/*****************************************************************************/ 
LOCAL uint8 Get_ProgressBar_CurrentIndex(uint16 current_percent,uint16 progress_length,uint16 per_length)
{
	uint8 level_index = (current_percent * progress_length)/100/per_length;
	if(level_index >5)
		level_index =5;
	return level_index;
}
LOCAL MMI_RESULT_E HandleBrightnessWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_POINT_T         display_point = {0};
    GUI_RECT_T img_rect = {0};
    GUI_BG_T bg_ptr = {0};

#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)	
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};	
    GUI_RECT_T          text_rect={42,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 
#endif
    GUI_RECT_T          rect         = MMITHEME_GetFullScreenRectEx(win_id);	
#ifdef ZDT_ZFB_SUPPORT
    uint32 time_ms = 0;
#endif

#ifdef ZTE_WATCH
MMI_IMAGE_ID_T         res_progress_bg = IMAGE_ZTE_CONTROL_PROGRESS_BG;
	MMI_IMAGE_ID_T         res_progress_fg= IMAGE_ZTE_CONTROL_PROGRESS;
	MMI_IMAGE_ID_T         res_progress_hand = IMAGE_ZTE_PROGRESS_HAND;
	GUI_RECT_T progressrect = SET_PROGRESSBAR_RECT;
	GUI_RECT_T setok_btn_rect =  WATCH_SETOK_BTN_RECT;
	uint32      cur_item_index = 0;
#else
	GUI_RECT_T leftBtnRect = SET_BTN_MINUS_RECT;
	GUI_RECT_T rightBtnRect = SET_BTN_PLUS_RECT;
#endif
	

    MMI_IMAGE_ID_T Bright_Image_Id[6]=
    {
        IMAGE_BRIGHT_LEVEL0,
        IMAGE_BRIGHT_LEVEL1,
        IMAGE_BRIGHT_LEVEL2,
        IMAGE_BRIGHT_LEVEL3,
        IMAGE_BRIGHT_LEVEL4,
        IMAGE_BRIGHT_LEVEL5
    };
	
    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:	
#ifdef ZTE_WATCH
		 cur_bright_img_id = MMIAPISET_GetCurrentContrast();
		 progress_down_flag = 0;
		if (MMISET_CONTRAST_SEVEN < cur_bright_img_id)
			cur_bright_img_id = MMISET_CONTRAST_SEVEN;
		WATCHCOM_ProgressBarByIndex(win_id, progressrect, cur_bright_img_id,MMISET_CONTRAST_SEVEN,res_progress_bg, res_progress_fg, res_progress_hand);

		GUIBUTTON_SetRect(MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID, &setok_btn_rect);
#else
		s_setting_brightness_value = MMIAPISET_GetCurrentContrast();
		if(1==s_setting_brightness_value)
		{
			cur_bright_img_id = 0;			
		}
		else if((2==s_setting_brightness_value)||(3==s_setting_brightness_value))
		{
			cur_bright_img_id = 1;			
		}
		else if(4==s_setting_brightness_value)
		{
			cur_bright_img_id = 2;	
		}
		else if(5==s_setting_brightness_value)
		{
			cur_bright_img_id = 3;	
		}
		else if(6==s_setting_brightness_value)
		{
			cur_bright_img_id = 4;	
		}
		else
		{
			cur_bright_img_id = 5;	
		}		
		GUIBUTTON_SetRect(MMISET_COMMON_L_BTN_CTRL_ID, &leftBtnRect);
		GUIBUTTON_SetRect(MMISET_COMMON_R_BTN_CTRL_ID, &rightBtnRect);
#endif
	    break;
		
	case MSG_FULL_PAINT:
		
#ifdef ZTE_WATCH
		 LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
		 //WATCHCOM_ProgressBar(win_id, progressrect,current_percent_brightness,res_progress_bg,res_progress_fg,res_progress_hand);
		 WATCHCOM_ProgressBarByIndex(win_id, progressrect, cur_bright_img_id,MMISET_CONTRAST_SEVEN,res_progress_bg, res_progress_fg, res_progress_hand);
#endif
		Brightness_Full_Paint(win_id);

	    break;
		
	case MSG_CTL_MIDSK:
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
	case MSG_APP_WEB:
#if defined( TOUCH_PANEL_SUPPORT)||defined(ZDT_TOUCHPANEL_TYPE_MULTITP) //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE95

#ifdef ZTE_WATCH
		if(MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//reset button被选中
		{
			MMIAPISET_SetMainLCDContrast();
			MMK_CloseWin(win_id);
		}

#else
		if(MMISET_COMMON_L_BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//reset button被选中
		{
			if(cur_bright_img_id>0)
			{
				if(cur_bright_img_id<3)
				{
					//s_setting_brightness_value = cur_bright_img_id+2;
					s_setting_brightness_value = cur_bright_img_id;
				}
				else
				{
					//s_setting_brightness_value = cur_bright_img_id+3;
					s_setting_brightness_value = cur_bright_img_id+1;
				}
				cur_bright_img_id--;
			}
			else
			{
				s_setting_brightness_value = 1;
				cur_bright_img_id = 0;
			}				

			MMIAPISET_SetCurrentContrast(s_setting_brightness_value);      
			MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, s_setting_brightness_value);
			MMIAPISET_SetMainLCDContrast();
			
				display_point.x = BRIGHT_IMAGE_X; // 35*BIG_SCALE;//35*SCALE;//client_rect.v_rect.left;
				display_point.y =  BRIGHT_IMAGE_Y; // 74*BIG_SCALE;//103*SCALE;///client_rect.v_rect.top;		

				bg_ptr.bg_type = GUI_BG_COLOR;
				bg_ptr.color = MMI_BLACK_COLOR;
			GUI_DisplayBg(&bg_ptr,&display_point,MMITHEME_GetDefaultLcdDev());
			
			GUIRES_DisplayImg(&display_point,
			    PNULL,
			    PNULL,
			    win_id,
			    Bright_Image_Id[cur_bright_img_id],
			    &lcd_dev_info);

			MMK_UpdateScreen();

		}
		else if(MMISET_COMMON_R_BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//play button被选中
		{
			if(cur_bright_img_id<5)
			{
				//if(cur_bright_img_id<2)
				if(cur_bright_img_id<1)
				{
					s_setting_brightness_value = cur_bright_img_id+2;
				}
				else
				{
					s_setting_brightness_value = cur_bright_img_id+3;
				}
				cur_bright_img_id++;
			}
			else
			{
				s_setting_brightness_value = 7;
				cur_bright_img_id = 5;
			}				

			MMIAPISET_SetCurrentContrast(s_setting_brightness_value);      
			MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, s_setting_brightness_value);
			MMIAPISET_SetMainLCDContrast();
			
				display_point.x =BRIGHT_IMAGE_X; ///  35*BIG_SCALE;//35*SCALE;//client_rect.v_rect.left;
				display_point.y =BRIGHT_IMAGE_Y; //   74*BIG_SCALE;//103*SCALE;///client_rect.v_rect.top;		

				bg_ptr.bg_type = GUI_BG_COLOR;
				bg_ptr.color = MMI_BLACK_COLOR;
			GUI_DisplayBg(&bg_ptr,&display_point,MMITHEME_GetDefaultLcdDev());
			
			GUIRES_DisplayImg(&display_point,
			    PNULL,
			    PNULL,
			    win_id,
			    Bright_Image_Id[cur_bright_img_id],
			    &lcd_dev_info);

			MMK_UpdateScreen();
		}
#endif
	    break;


#ifdef ZTE_WATCH
	case MSG_TP_PRESS_DOWN:
	{
		        GUI_POINT_T   point = {0};
		        point.x = MMK_GET_TP_X(param);
		        point.y = MMK_GET_TP_Y(param);
			 if (GUI_PointIsInRect(point, progressrect))
			{
				//uint8 percent = (point.x - rect.left)*100/(rect.right-rect.left);
				progress_down_flag = 1;
				cur_item_index =  ROUND((float)(MMISET_CONTRAST_SEVEN)
				                  *(point.x - progressrect.left)
				                  /(progressrect.right-progressrect.left)); //0 is not in total num, eg total=100, progressbar display 0~100
				if(cur_item_index != cur_bright_img_id)
				{
					cur_bright_img_id = cur_item_index;
					WATCHCOM_ProgressBarByIndex(win_id, progressrect, cur_bright_img_id,MMISET_CONTRAST_SEVEN,res_progress_bg, res_progress_fg, res_progress_hand);
					MMIAPISET_SetCurrentContrast(cur_bright_img_id);      
					MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, cur_bright_img_id);
					MMK_UpdateScreen();
				}
				/*current_percent_brightness = (point.x - progressrect.left)*100/(progressrect.right-progressrect.left);
				WATCHCOM_ProgressBar(win_id, progressrect, current_percent_brightness,res_progress_bg, res_progress_fg, res_progress_hand);
				cur_bright_img_id = Get_ProgressBar_CurrentIndex(current_percent_brightness,SET_PROGRESSBAR_TOTAL_LENGTH,SET_PROGRESSBAR_PERLEVEL_LENGTH);
				if(cur_bright_img_id<5)
				{
					if(cur_bright_img_id<2)
					{
						s_setting_brightness_value = cur_bright_img_id+1;
					}
					else
					{
						s_setting_brightness_value = cur_bright_img_id+2;
					}
					//cur_bright_img_id++;
				}
				else
				{
					s_setting_brightness_value = 7;
					cur_bright_img_id = 5;
				}
				MMIAPISET_SetCurrentContrast(s_setting_brightness_value);      
				MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, s_setting_brightness_value);
				MMK_UpdateScreen();*/
				
			} 
			
	    	}	
	      break;	

	  case MSG_TP_PRESS_MOVE:
				{
			
			GUI_POINT_T   point = {0};              
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if (progress_down_flag /*&& GUI_PointIsInRect(point, progressrect)*/)
			{
				//uint8 percent = (point.x - rect.left)*100/(rect.right-rect.left);
				/*current_percent_brightness= (point.x - progressrect.left)*100/(progressrect.right-progressrect.left);
				WATCHCOM_ProgressBar(win_id, progressrect, current_percent_brightness,res_progress_bg, res_progress_fg, res_progress_hand);
				cur_bright_img_id = Get_ProgressBar_CurrentIndex(current_percent_brightness,SET_PROGRESSBAR_TOTAL_LENGTH,SET_PROGRESSBAR_PERLEVEL_LENGTH);
				if(cur_bright_img_id<5)
				{
					if(cur_bright_img_id<2)
					{
						s_setting_brightness_value = cur_bright_img_id+1;
					}
					else
					{
						s_setting_brightness_value = cur_bright_img_id+2;
					}
					//cur_bright_img_id++;
				}
				else
				{
					s_setting_brightness_value = 7;
					cur_bright_img_id = 5;
				}
				MMIAPISET_SetCurrentContrast(s_setting_brightness_value);      
				MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, s_setting_brightness_value);*/
				cur_item_index =  ROUND((float)(MMISET_CONTRAST_SEVEN)
				                  *(point.x - progressrect.left)
				                  /(progressrect.right-progressrect.left)); //0 is not in total num, eg total=100, progressbar display 0~100
				if(cur_item_index != cur_bright_img_id)
				{
					cur_bright_img_id = cur_item_index;
					WATCHCOM_ProgressBarByIndex(win_id, progressrect, cur_bright_img_id,MMISET_CONTRAST_SEVEN,res_progress_bg, res_progress_fg, res_progress_hand);
					MMIAPISET_SetCurrentContrast(cur_bright_img_id);      
					MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, cur_bright_img_id);
					MMK_UpdateScreen();
				}
			}
			else
				progress_down_flag = 0;
			}
			break;
  #endif
	case MSG_TP_PRESS_UP:
	    	{
#ifdef ZTE_WATCH
			progress_down_flag = 0;
#else
		        GUI_POINT_T   point = {0};
		        point.x = MMK_GET_TP_X(param);
		        point.y = MMK_GET_TP_Y(param);
			
				
#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)	
                     if((point.x > 0&& point.x <42)&&(point.y > 0 &&  point.y < MMI_SPECIAL_TITLE_HEIGHT))
                     {
			     MMK_CloseWin(win_id);
			     return;	 
                     }
#endif
#ifdef ZDT_ZFB_SUPPORT
		    if((point.x > 0&& point.x <42)&&(point.y > MMI_SPECIAL_TITLE_HEIGHT &&  point.y < 155))
                {
                   time_ms = MMIAPICOM_GetFfsTime();
                   if (s_win_tp_ms == 0)
                   {
                       s_win_tp_ms = time_ms;
                       s_win_tp_count = 1;
                   }
                   else if (time_ms - s_win_tp_ms <1000)
                   {
                       s_win_tp_count++;
                       s_win_tp_ms = time_ms;
                   }
                   else
                   {
                       s_win_tp_ms = time_ms;
                       s_win_tp_count = 1;
                   }
                   if (s_win_tp_count >= 10)
                   {
                      s_win_tp_count=0;
                      s_win_tp_ms=0;
                      ZFB_DB_INIT_OK_Reset_All();
                   }
                }
#endif
#endif
	    	}
         	break;
#ifndef ZTE_WATCH		
		case MSG_KEYUP_UP:	
		{
			if(cur_bright_img_id<5)
			{
				if(cur_bright_img_id<2)
				{
					s_setting_brightness_value = cur_bright_img_id+2;
				}
				else
				{
					s_setting_brightness_value = cur_bright_img_id+3;
				}
				cur_bright_img_id++;
			}
			else
			{
				s_setting_brightness_value = 7;
				cur_bright_img_id = 5;
			}				

			MMIAPISET_SetCurrentContrast(s_setting_brightness_value);      
			MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, s_setting_brightness_value);
			MMIAPISET_SetMainLCDContrast();
			
				display_point.x = 35;//35*SCALE;//client_rect.v_rect.left;
				display_point.y =  74;//103*SCALE;///client_rect.v_rect.top;		

				bg_ptr.bg_type = GUI_BG_COLOR;
				bg_ptr.color = MMI_BLACK_COLOR;
			GUI_DisplayBg(&bg_ptr,&display_point,MMITHEME_GetDefaultLcdDev());
			
			GUIRES_DisplayImg(&display_point,
			    PNULL,
			    PNULL,
			    win_id,
			    Bright_Image_Id[cur_bright_img_id],
			    &lcd_dev_info);

			MMK_UpdateScreen();
		    }
		 break;
	case MSG_KEYUP_DOWN:
		{
			if(cur_bright_img_id>0)
			{
				if(cur_bright_img_id<3)
				{
					//s_setting_brightness_value = cur_bright_img_id+2;
					s_setting_brightness_value = cur_bright_img_id;
				}
				else
				{
					//s_setting_brightness_value = cur_bright_img_id+3;
					s_setting_brightness_value = cur_bright_img_id+1;
				}
				cur_bright_img_id--;
			}
			else
			{
				s_setting_brightness_value = 1;
				cur_bright_img_id = 0;
			}				

			MMIAPISET_SetCurrentContrast(s_setting_brightness_value);      
			MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, s_setting_brightness_value);
			MMIAPISET_SetMainLCDContrast();
			
				display_point.x = 35;//35*SCALE;//client_rect.v_rect.left;
				display_point.y =  74;//103*SCALE;///client_rect.v_rect.top;		

				bg_ptr.bg_type = GUI_BG_COLOR;
				bg_ptr.color = MMI_BLACK_COLOR;
			GUI_DisplayBg(&bg_ptr,&display_point,MMITHEME_GetDefaultLcdDev());
			
			GUIRES_DisplayImg(&display_point,
			    PNULL,
			    PNULL,
			    win_id,
			    Bright_Image_Id[cur_bright_img_id],
			    &lcd_dev_info);

			MMK_UpdateScreen();
		        }
		 break;
#endif
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
	    MMK_CloseWin(win_id);
	    break;
    case MSG_KEYDOWN_RED:
        break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;    
	default:
	    recode = MMI_RESULT_FALSE;
	    break;
    }
	return recode;
}

/*****************************************************************************/
//  Description :HandleSetSystemSettingWindow
//  Global resource dependence :
//  Author:James li
//  Note:
/*****************************************************************************/ 
LOCAL MMI_RESULT_E HandleVolumeWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)		
    GUI_RECT_T          content_rect={0,MMI_SPECIAL_TITLE_HEIGHT,240,240}; ////xiongkai ADD_TITLE_IN_ALL_SETTING_MENU	
    GUI_RECT_T          title_rect={0,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 	
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_RECT_T          rect         = MMITHEME_GetFullScreenRectEx(win_id);	
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};	
    GUI_RECT_T          text_rect={42,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 
#endif
	uint32 listNum = 0;

    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:	
#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)		
		//draw TITLE bg		
			GUIRES_DisplayImg(PNULL,
			    &rect,
			    PNULL,
			    win_id,
			    IMAGE_THEME_BLACK_BG_SPEC, 
			    &lcd_dev_info); 

		///////draw title
		text_style.align = ALIGN_LVMIDDLE;
		text_style.font = SONG_FONT_28;
		text_style.font_color = MMI_WHITE_COLOR;
		text_style.char_space = 0;
	
    		MMI_GetLabelTextByLang(text_volume_settings, &string);
		GUISTR_DrawTextToLCDInRect( 
		    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		    (const GUI_RECT_T      *)&text_rect,       //the fixed display area
		    (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
		    (const MMI_STRING_T    *)&string,
		    &text_style,
		    state,
		    GUISTR_TEXT_DIR_AUTO
		    );
#endif		
	     //GUIMENU_SetRect(MMISET_SET_VOLUME_CTRL_ID,&content_rect);		 
	    // GUIMENU_GetId(MMISET_SET_VOLUME_CTRL_ID,&group_id,&menu_id);
         //MMK_SetAtvCtrl(win_id,MMISET_SET_VOLUME_CTRL_ID);	
		// CTRLMENU_SetSecMenuItemHeight(MMISET_SET_VOLUME_CTRL_ID,48,48);
		// CTRLMENU_SetFontColor(MMISET_SET_VOLUME_CTRL_ID,MMI_WHITE_COLOR);
		  

		listNum = sizeof(s_settings_volume_text_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
		GUILIST_SetRect(MMISET_SET_VOLUME_CTRL_ID,&content_rect);
		WatchCOM_TextList_Create( s_settings_volume_text_list_data, listNum, MMISET_SET_VOLUME_CTRL_ID );
		MMK_SetAtvCtrl( MMISET_SET_VOLUME_WIN_ID, MMISET_SET_VOLUME_CTRL_ID );
	    break;

#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)				
	case MSG_FULL_PAINT:	//从子菜单退出来后title变成子菜单的了。
		LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
	
		//draw TITLE bg		
			GUIRES_DisplayImg(PNULL,
			    &rect,
			    PNULL,
			    win_id,
			    IMAGE_THEME_BLACK_BG_SPEC,
			    &lcd_dev_info);  
		
		///////draw title
		text_style.align = ALIGN_LVMIDDLE;
		text_style.font = SONG_FONT_28;
		text_style.font_color = MMI_WHITE_COLOR;
		text_style.char_space = 0;
	
    		MMI_GetLabelTextByLang(text_volume_settings, &string);
		GUISTR_DrawTextToLCDInRect( 
		    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		    (const GUI_RECT_T      *)&text_rect,       //the fixed display area
		    (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
		    (const MMI_STRING_T    *)&string,
		    &text_style,
		    state,
		    GUISTR_TEXT_DIR_AUTO
		    );	
		//CTRLMENU_SetFontColor(MMISET_SET_VOLUME_CTRL_ID,MMI_WHITE_COLOR);
	    break;	
#endif

#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)	
	case MSG_TP_PRESS_UP:
	    	{
		        GUI_POINT_T   point = {0};
			 GUI_RECT_T   back_rect=WINDOW_BACK_RECT; 
		        point.x = MMK_GET_TP_X(param);
		        point.y = MMK_GET_TP_Y(param);

                     if (GUI_PointIsInRect(point, back_rect))
                     {
			     MMK_CloseWin(win_id);
			     return;	 
                     }
		}
	      break;
#endif
		  
#if defined( TOUCH_PANEL_SUPPORT)||defined(ZDT_TOUCHPANEL_TYPE_MULTITP)  //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
	case MSG_KEYUP_OK: 
		  WatchCOM_List_Item_CallBack( MMISET_SET_VOLUME_CTRL_ID );
	    #if 0
		GUIMENU_GetId(MMISET_SET_VOLUME_CTRL_ID,&group_id,&menu_id);
	    switch (menu_id)
	    {
		case ID_SET_MENU_CALL_VOLUME:
			MMK_CreateWin((uint32 *) MMISET_CALL_VOLUME_WIN_TAB, PNULL);
                    break;
					
		case ID_SET_MENU_RINGTONE_VOLUME:
			MMK_CreateWin((uint32 *) MMISET_RINGTONE_VOLUME_WIN_TAB, PNULL);
                    break;

                default:
                    break;
           }
		#endif
	    break;
		 
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
	    MMK_CloseWin(win_id);
	    break;
     case MSG_KEYDOWN_RED:
            break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;   
	default:
	    recode = MMI_RESULT_FALSE;
	    break;
    }
	return recode;
}

typedef struct
{
    MMISET_ALL_RING_TYPE_E type;
    uint16 id;
    uint8 volume;
    BOOLEAN isVibrate;
}MMI_SETTINGS_VOLUME_PREVIEW_PARAM;

#define SETTINGS_VOLUME_PRIVIEW_DEFAULT_TIME              (1000)
#define SETTINGS_VOLUME_PRIVIEW_REPEAT_TIMES              (1)
#define SETTINGS_V_VIBRATE_PRIVIEW_REPEAT_TIMES           (20)

LOCAL uint8 s_settings_volume_preview_timer_id = 0;

LOCAL BOOLEAN s_settings_volume_ring_preview_is_runing = FALSE;

LOCAL BOOLEAN s_settings_volume_vibrate_preview_is_runing = FALSE;
LOCAL uint8 s_settings_volume_vibrate_preview_timer_id = 0;
LOCAL uint8 s_settings_volume_vibrate_preview_count = 0;
LOCAL MMISET_ALL_RING_TYPE_E s_settings_volume_vibrate_type = MMISET_RING_TYPE_CALL;

LOCAL MMI_SETTINGS_VOLUME_PREVIEW_PARAM s_settings_preview_volume_param = { 0 };


LOCAL void Settings_Volume_StopVibratePreviewTimer( void )
{
    TRACE_APP_SETTINGS(" enter");

    if ( s_settings_volume_vibrate_preview_timer_id != 0 )
    {
        MMK_StopTimer( s_settings_volume_vibrate_preview_timer_id );
        s_settings_volume_vibrate_preview_timer_id = 0;
    }
}

LOCAL void _Callback_Settings_Volume_RingPreview( MMISRVAUD_REPORT_RESULT_E result, DPARAM param )
{
    TRACE_APP_SETTINGS(" enter");

    s_settings_volume_ring_preview_is_runing = FALSE;

    if( TRUE == s_settings_volume_vibrate_preview_is_runing )
    {
        Settings_Volume_StopVibratePreviewTimer();

        MMIAPISET_StopVibrator( s_settings_volume_vibrate_type );
        s_settings_volume_vibrate_preview_is_runing = FALSE;
    }
}

LOCAL void _Callback_Volume_Settings_VibrateTimer( uint8 timer_id, uint32 param )
{
    TRACE_APP_SETTINGS("enter");

    s_settings_volume_vibrate_preview_count++;

    if( TRUE == s_settings_volume_vibrate_preview_is_runing )
    {
        TRACE_APP_SETTINGS("stop vibrate");
        MMIAPISET_StopVibrator( (MMISET_ALL_RING_TYPE_E)param );
        s_settings_volume_vibrate_preview_is_runing = FALSE;

        s_settings_volume_vibrate_preview_timer_id = MMK_CreateTimerCallback( SETTINGS_VOLUME_PRIVIEW_DEFAULT_TIME, _Callback_Volume_Settings_VibrateTimer, (uint32)s_settings_volume_vibrate_type, FALSE );
    }
    else
    {
        if( s_settings_volume_vibrate_preview_count < SETTINGS_V_VIBRATE_PRIVIEW_REPEAT_TIMES )
        {
            TRACE_APP_SETTINGS("start vibrate");
            MMIAPIENVSET_StartVibrator( (MMISET_ALL_RING_TYPE_E)param );
            s_settings_volume_vibrate_preview_is_runing = TRUE;

            s_settings_volume_vibrate_preview_timer_id = MMK_CreateTimerCallback( SETTINGS_VOLUME_PRIVIEW_DEFAULT_TIME, _Callback_Volume_Settings_VibrateTimer, (uint32)s_settings_volume_vibrate_type, FALSE );
        }
    }
}

LOCAL void _Callback_Settings_Volume_PreviewTimer( uint8 timer_id, uint32 param )
{
    MN_DUAL_SYS_E                       dualSys = MMIAPISET_GetActiveSim();
    MMI_SETTINGS_VOLUME_PREVIEW_PARAM*    pPreviewParam = (MMI_SETTINGS_VOLUME_PREVIEW_PARAM*)param;

    TRACE_APP_SETTINGS(" enter");

    if( s_settings_volume_preview_timer_id != timer_id )
    {
        TRACE_APP_SETTINGS("timer_id error!");
        return;
    }
    s_settings_volume_preview_timer_id = 0;

    if( PNULL == pPreviewParam )
    {
        TRACE_APP_SETTINGS("pPreviewParam is null!");
        return;
    }

    if( pPreviewParam->type != MMISET_RING_TYPE_MAX )
    {
        MMIAPISET_PreviewRing( dualSys, pPreviewParam->id, pPreviewParam->volume, SETTINGS_VOLUME_PRIVIEW_REPEAT_TIMES, pPreviewParam->type, _Callback_Settings_Volume_RingPreview );
        s_settings_volume_ring_preview_is_runing = TRUE;
    }

    if( TRUE == pPreviewParam->isVibrate )
    {
        s_settings_volume_vibrate_preview_count = 0;
        s_settings_volume_vibrate_type = pPreviewParam->type;
        s_settings_volume_vibrate_preview_timer_id = MMK_CreateTimerCallback( SETTINGS_VOLUME_PRIVIEW_DEFAULT_TIME, _Callback_Volume_Settings_VibrateTimer, (uint32)s_settings_volume_vibrate_type, FALSE );

        MMIAPIENVSET_StartVibrator( s_settings_volume_vibrate_type );
        s_settings_volume_vibrate_preview_is_runing  = TRUE;
    }
}

LOCAL void Settings_Volume_StopPreviewTimer( void )
{
    TRACE_APP_SETTINGS(" enter");

    if ( s_settings_volume_preview_timer_id != 0 )
    {
        MMK_StopTimer( s_settings_volume_preview_timer_id );
        s_settings_volume_preview_timer_id = 0;
    }
}

PUBLIC void Settings_Volume_StopPreviewRing( void )
{
    TRACE_APP_SETTINGS(" enter");

    Settings_Volume_StopPreviewTimer();
    Settings_Volume_StopVibratePreviewTimer();

    if( TRUE == s_settings_volume_ring_preview_is_runing )
    {
        MMIAPISET_StopAppRing();
        s_settings_volume_ring_preview_is_runing = FALSE;
    }
    if( TRUE == s_settings_volume_vibrate_preview_is_runing )
    {
        MMIAPISET_StopVibrator( s_settings_volume_vibrate_type );
        s_settings_volume_vibrate_preview_is_runing = FALSE;
    }
}

PUBLIC void SettingsVolume_PreviewRing( uint16 ring_id, MMISET_ALL_RING_TYPE_E ring_type, uint8 ring_volume, BOOLEAN is_vibrate )
{
    TRACE_APP_SETTINGS(" enter");

    Settings_Volume_StopPreviewRing();

    s_settings_preview_volume_param.id = ring_id;
    s_settings_preview_volume_param.type = ring_type;
    s_settings_preview_volume_param.volume = ring_volume;
    s_settings_preview_volume_param.isVibrate = is_vibrate;

    s_settings_volume_preview_timer_id = MMK_CreateTimerCallback( SETTINGS_VOLUME_PRIVIEW_DEFAULT_TIME, _Callback_Settings_Volume_PreviewTimer, (uint32)(&s_settings_preview_volume_param), FALSE );
}

PUBLIC void SettingsVolume_PreviewRingEx( uint16 ring_id, MMISET_ALL_RING_TYPE_E ring_type, uint8 ring_volume, BOOLEAN is_vibrate )
{
    TRACE_APP_SETTINGS(" enter");

    Settings_Volume_StopPreviewRing();

    s_settings_preview_volume_param.id = ring_id;
    s_settings_preview_volume_param.type = ring_type;
    s_settings_preview_volume_param.volume = ring_volume;
    s_settings_preview_volume_param.isVibrate = is_vibrate;

    s_settings_volume_preview_timer_id = MMK_CreateTimerCallback( 50, _Callback_Settings_Volume_PreviewTimer, (uint32)(&s_settings_preview_volume_param), FALSE );
}

LOCAL void Settings_Volume_SoundUpdate( uint8 volume )
{
    // preview use call now, waiting more detail requirement
    uint16 ringId = MMIAPIENVSET_GetFixCallRingId( MN_DUAL_SYS_1, MMIAPIENVSET_GetActiveModeId() );

    if( volume > MMISET_VOL_MAX )
    {
        TRACE_APP_SETTINGS("MMVolume error %u change to default value", volume);
        volume = MMISET_VOL_DEFAULT_MULTIM;
    }

    //volume = volume / SETTINGS_MMSOUND_VOLUME_INTERVAL * SETTINGS_MMSOUND_VOLUME_INTERVAL;
    //MMIAPISET_SetMultimVolume( volume );
	
	//MMIENVSET_SetActiveModeOptValue( CALL_RING_VOL, volume, MN_DUAL_SYS_1 );
    //MMIENVSET_SetActiveModeOptValue( MSG_RING_VOL, volume, MN_DUAL_SYS_1 );
    //MMIENVSET_SetActiveModeOptValue( ALARM_RING_VOL, volume, MN_DUAL_SYS_1 );

    SettingsVolume_PreviewRingEx( ringId, MMISET_RING_TYPE_CALL, volume, FALSE );

    //MMK_SendMsg( MMISET_MM_SOUND_WIN_ID, MSG_FULL_PAINT, PNULL );
}

LOCAL MMI_RESULT_E HandleCallVolumeSetWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_POINT_T         display_point = {0};
    GUI_RECT_T img_rect = {0};
    GUI_BG_T bg_ptr = {0};

#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)	
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};	
    GUI_RECT_T          text_rect={42,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 
#endif

    GUI_RECT_T          rect         = MMITHEME_GetFullScreenRectEx(win_id);	

#ifdef ZTE_WATCH
	MMI_IMAGE_ID_T         res_progress_bg = IMAGE_ZTE_CONTROL_PROGRESS_BG;
	MMI_IMAGE_ID_T         res_progress_fg= IMAGE_ZTE_CONTROL_PROGRESS;
	MMI_IMAGE_ID_T         res_progress_hand = IMAGE_ZTE_PROGRESS_HAND;
	GUI_RECT_T progressrect = SET_PROGRESSBAR_RECT;
	GUI_RECT_T setok_btn_rect =  WATCH_SETOK_BTN_RECT;
	uint32      cur_item_index = 0;
	uint32  volume_item_total = MMISET_VOL_MAX+1;
#else
	GUI_RECT_T leftBtnRect = SET_BTN_MINUS_RECT;
	GUI_RECT_T rightBtnRect = SET_BTN_PLUS_RECT;


    MMI_IMAGE_ID_T Volume_Image_Id[6]=
    {
        IMAGE_VOLUME_LEVEL0,
        IMAGE_VOLUME_LEVEL1,
        IMAGE_VOLUME_LEVEL2,
        IMAGE_VOLUME_LEVEL3,
        IMAGE_VOLUME_LEVEL4,
        IMAGE_VOLUME_LEVEL5
    };

    uint32 Volume_Level[6]=
    {
        0,
        1,
        3,
        5,
        7,
        9
    };
#endif
    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:	
        MMISRVAUD_ReqVirtualHandle("FIX CALL SELECT RING", MMISRVAUD_PRI_NORMAL);  
             //LCD_FillRect(&lcd_dev_info, rect, MMI_RED_COLOR);
#ifdef ZTE_WATCH
		 progress_down_flag = 0;
		 /* if(cur_call_volume_img_id ==0)
			 current_percent_brightness = 0;
		 else
			 current_percent_brightness = (cur_call_volume_img_id+1 )*SET_PROGRESSBAR_PERLEVEL_LENGTH*100/SET_PROGRESSBAR_TOTAL_LENGTH;
		WATCHCOM_ProgressBar(win_id, progressrect,current_percent_brightness,res_progress_bg,res_progress_fg,res_progress_hand);*/
		cur_call_volume_img_id = MMIAPISET_GetCallVolume();
		 if(MMISET_VOL_MAX < cur_call_volume_img_id)
			cur_call_volume_img_id = MMISET_VOL_MAX;
		WATCHCOM_ProgressBarByIndex(win_id, progressrect, cur_call_volume_img_id,volume_item_total,res_progress_bg, res_progress_fg, res_progress_hand);

		GUIBUTTON_SetRect(MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID, &setok_btn_rect);
#else
		
        s_setting_call_volume_value = MMIAPISET_GetCallVolume();

        if(0==s_setting_call_volume_value)
        {
            cur_call_volume_img_id = 0;         
        }
        else if(1==s_setting_call_volume_value)
        {
            cur_call_volume_img_id = 1;         
        }
        else if((2==s_setting_call_volume_value)||(3==s_setting_call_volume_value))
        {
            cur_call_volume_img_id = 2; 
        }
        else if((4==s_setting_call_volume_value)||(5==s_setting_call_volume_value))
        {
            cur_call_volume_img_id = 3; 
        }
        else if((6==s_setting_call_volume_value)||(7==s_setting_call_volume_value))
        {
            cur_call_volume_img_id = 4; 
        }
        else
        {
            cur_call_volume_img_id = 5; 
        }       

		GUIBUTTON_SetRect(MMISET_COMMON_L_BTN_CTRL_ID, &leftBtnRect);
		GUIBUTTON_SetRect(MMISET_COMMON_R_BTN_CTRL_ID, &rightBtnRect);
#endif
	    break;

	
	case MSG_FULL_PAINT:
		LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
#ifdef ZTE_WATCH
		display_point.x = BRIGHT_ICON_VOL_X; // 43*BIG_SCALE;//81*SCALE;//client_rect.v_rect.left;
		display_point.y = BRIGHT_ICON_VOL_Y; //  14*BIG_SCALE;//25*SCALE;///client_rect.v_rect.top;	

		GUIRES_DisplayImg(&display_point,
		    PNULL,
		    PNULL,
		    win_id,
		    IMAGE_VOLUME_ICON,
		    &lcd_dev_info);
		//WATCHCOM_ProgressBar(win_id, progressrect,current_percent_brightness,res_progress_bg,res_progress_fg,res_progress_hand);
		WATCHCOM_ProgressBarByIndex(win_id, progressrect, cur_call_volume_img_id,volume_item_total,res_progress_bg, res_progress_fg, res_progress_hand);
#else
#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)				
		//draw bg		
			GUIRES_DisplayImg(PNULL,
			    &rect,
			    PNULL,
			    win_id,
			    IMAGE_THEME_BLACK_BG_SPEC, //xiongkai IMAGE_THEME_BG,
			    &lcd_dev_info); 

		///////draw title
		text_style.align = ALIGN_LVMIDDLE;
		text_style.font = SONG_FONT_28;
		text_style.font_color = MMI_WHITE_COLOR;
		text_style.char_space = 0;
	
    		MMI_GetLabelTextByLang(text_call_volume_setting, &string);
		GUISTR_DrawTextToLCDInRect( 
		    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		    (const GUI_RECT_T      *)&text_rect,       //the fixed display area
		    (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
		    (const MMI_STRING_T    *)&string,
		    &text_style,
		    state,
		    GUISTR_TEXT_DIR_AUTO
		    );

#endif
			display_point.x = BRIGHT_ICON_X; // 43*BIG_SCALE;//81*SCALE;//client_rect.v_rect.left;
			display_point.y = BRIGHT_ICON_Y; //  14*BIG_SCALE;//25*SCALE;///client_rect.v_rect.top;	
			
			GUIRES_DisplayImg(&display_point,
			    PNULL,
			    PNULL,
			    win_id,
			    IMAGE_VOLUME_ICON,
			    &lcd_dev_info);

			display_point.x = BRIGHT_IMAGE_X; // 35*BIG_SCALE;//35*SCALE;//client_rect.v_rect.left;
			display_point.y = BRIGHT_IMAGE_Y; // 74*BIG_SCALE;//103*SCALE;///client_rect.v_rect.top;		
			
			GUIRES_DisplayImg(&display_point,
			    PNULL,
			    PNULL,
			    win_id,
			    Volume_Image_Id[cur_call_volume_img_id],
			    &lcd_dev_info);	
#endif
	    break;
		
	case MSG_CTL_MIDSK:
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
	case MSG_APP_WEB:
#if defined( TOUCH_PANEL_SUPPORT)||defined(ZDT_TOUCHPANEL_TYPE_MULTITP) //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef ZTE_WATCH
		if(MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//reset button被选中
		{
			/*if(cur_call_volume_img_id>1)
			{
				//s_setting_call_volume_value = 2*cur_call_volume_img_id+1;
				s_setting_call_volume_value = 2*cur_call_volume_img_id-2;
				//cur_call_volume_img_id--;
			}
			else
			{
				s_setting_call_volume_value = cur_call_volume_img_id;//0;
			}				

			MMIAPISET_SetCallVolume(s_setting_call_volume_value);     */
			MMIAPISET_SetCallVolume(cur_call_volume_img_id);  
			//MMIAPISET_UiPlayRingByVolume(0, FALSE,0, 1, MMISET_RING_TYPE_KEY, PNULL, s_setting_call_volume_value);
			//Settings_Volume_SoundUpdate(s_setting_call_volume_value);
			MMK_CloseWin(win_id);
		}

#else
		if(MMISET_COMMON_L_BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//reset button被选中
		{
			if(cur_call_volume_img_id>1)
			{
				//s_setting_call_volume_value = 2*cur_call_volume_img_id+1;
				s_setting_call_volume_value = 2*cur_call_volume_img_id-3;
				cur_call_volume_img_id--;
			}
			else
			{
				s_setting_call_volume_value = 0;
				cur_call_volume_img_id = 0;
			}				

			MMIAPISET_SetCallVolume(s_setting_call_volume_value);  
			ual_tele_call_set_call_volume(s_setting_call_volume_value);
			//MMIAPISET_UiPlayRingByVolume(0, FALSE,0, 1, MMISET_RING_TYPE_KEY, PNULL, s_setting_call_volume_value);
			Settings_Volume_SoundUpdate(s_setting_call_volume_value);
			//PlayRing(0, ring_id,ring_vol, play_times,ring_type,callback_func,MMIENVSET_GetCurModeId());
			//PlayRing(dual_sys, ring_id,ring_vol, play_times,ring_type,callback_func,MMIENVSET_GetCurModeId());
			display_point.x = BRIGHT_IMAGE_X; // 35*BIG_SCALE;//35*SCALE;//client_rect.v_rect.left;
			display_point.y = BRIGHT_IMAGE_Y; // 74*BIG_SCALE;//103*SCALE;///client_rect.v_rect.top;	

				bg_ptr.bg_type = GUI_BG_COLOR;
				bg_ptr.color = MMI_BLACK_COLOR;
			GUI_DisplayBg(&bg_ptr,&display_point,MMITHEME_GetDefaultLcdDev());
			
			GUIRES_DisplayImg(&display_point,
			    PNULL,
			    PNULL,
			    win_id,
			    Volume_Image_Id[cur_call_volume_img_id],
			    &lcd_dev_info);

			MMK_UpdateScreen();

		}
		else if(MMISET_COMMON_R_BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//play button被选中
		{
			if(cur_call_volume_img_id<5)
			{
				s_setting_call_volume_value = 2*cur_call_volume_img_id+1;
				cur_call_volume_img_id++;
			}
			else
			{
				s_setting_call_volume_value = 9;//0;
				cur_call_volume_img_id = 5;//0;
			}				

			MMIAPISET_SetCallVolume(s_setting_call_volume_value);     
			ual_tele_call_set_call_volume(s_setting_call_volume_value);
			//MMIAPISET_UiPlayRingByVolume(0, FALSE,0, 1, MMISET_RING_TYPE_KEY, PNULL, s_setting_call_volume_value);
			Settings_Volume_SoundUpdate(s_setting_call_volume_value);

			display_point.x = BRIGHT_IMAGE_X; // 35*BIG_SCALE;//35*SCALE;//client_rect.v_rect.left;
			display_point.y = BRIGHT_IMAGE_Y; //  74*BIG_SCALE;//103*SCALE;///client_rect.v_rect.top;	

				bg_ptr.bg_type = GUI_BG_COLOR;
				bg_ptr.color = MMI_BLACK_COLOR;
			GUI_DisplayBg(&bg_ptr,&display_point,MMITHEME_GetDefaultLcdDev());

			GUIRES_DisplayImg(&display_point,
			    PNULL,
			    PNULL,
			    win_id,
			    Volume_Image_Id[cur_call_volume_img_id],
			    &lcd_dev_info);

			MMK_UpdateScreen();
		}
#endif

	    break;
#ifdef ZTE_WATCH
	case MSG_TP_PRESS_DOWN:
	{
		        GUI_POINT_T   point = {0};
		        point.x = MMK_GET_TP_X(param);
		        point.y = MMK_GET_TP_Y(param);
			 if (GUI_PointIsInRect(point, progressrect))
			{
				//uint8 percent = (point.x - rect.left)*100/(rect.right-rect.left);
				progress_down_flag = 1;
				cur_item_index =  ROUND((float)(volume_item_total)
			                      *(point.x - progressrect.left)
			                      /(progressrect.right-progressrect.left)); //0 is not in total num, eg total=100, progressbar display 0~100
				if(cur_item_index != cur_call_volume_img_id)
				{
					cur_call_volume_img_id = cur_item_index;
					WATCHCOM_ProgressBarByIndex(win_id, progressrect, cur_call_volume_img_id,volume_item_total,res_progress_bg, res_progress_fg, res_progress_hand);
					MMK_UpdateScreen();
				}
				/*current_percent_brightness = (point.x - rect.left)*100/(rect.right-rect.left);
				WATCHCOM_ProgressBar(win_id, progressrect, current_percent_brightness,res_progress_bg, res_progress_fg, res_progress_hand);
				cur_call_volume_img_id = Get_ProgressBar_CurrentIndex(current_percent_brightness,SET_PROGRESSBAR_TOTAL_LENGTH,SET_PROGRESSBAR_PERLEVEL_LENGTH);
				if(cur_call_volume_img_id>1)
				{
					//s_setting_call_volume_value = 2*cur_call_volume_img_id+1;
					s_setting_call_volume_value = 2*cur_call_volume_img_id-2;
					//cur_call_volume_img_id--;
				}
				else
				{
					s_setting_call_volume_value = cur_call_volume_img_id;//0;
					//cur_call_volume_img_id = 1;//0;
				}	
				MMK_UpdateScreen();*/
				
			} 
			
	    	}	
	      break;	

	  case MSG_TP_PRESS_MOVE:
			{
			
			GUI_POINT_T   point = {0};              
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if (progress_down_flag /*&& GUI_PointIsInRect(point, progressrect)*/)
			{
				//uint8 percent = (point.x - rect.left)*100/(rect.right-rect.left);
				/*current_percent_brightness= (point.x - rect.left)*100/(rect.right-rect.left);
				WATCHCOM_ProgressBar(win_id, progressrect, current_percent_brightness,res_progress_bg, res_progress_fg, res_progress_hand);
				cur_call_volume_img_id = Get_ProgressBar_CurrentIndex(current_percent_brightness,SET_PROGRESSBAR_TOTAL_LENGTH,SET_PROGRESSBAR_PERLEVEL_LENGTH);
				cur_call_volume_img_id = Get_ProgressBar_CurrentIndex(current_percent_brightness,SET_PROGRESSBAR_TOTAL_LENGTH,SET_PROGRESSBAR_PERLEVEL_LENGTH);
				if(cur_call_volume_img_id>1)
				{
					//s_setting_call_volume_value = 2*cur_call_volume_img_id+1;
					s_setting_call_volume_value = 2*cur_call_volume_img_id-2;
					//cur_call_volume_img_id--;
				}
				else
				{
					s_setting_call_volume_value = cur_call_volume_img_id;//0;
					//cur_call_volume_img_id = 1;//0;
				}
				MMK_UpdateScreen();*/
				cur_item_index =  ROUND((float)(volume_item_total)
			                      *(point.x - progressrect.left)
			                      /(progressrect.right-progressrect.left)); //0 is not in total num, eg total=100, progressbar display 0~100
				if(cur_item_index != cur_call_volume_img_id)
				{
					cur_call_volume_img_id = cur_item_index;
					WATCHCOM_ProgressBarByIndex(win_id, progressrect, cur_call_volume_img_id,volume_item_total,res_progress_bg, res_progress_fg, res_progress_hand);
					MMK_UpdateScreen();
				}
			}
			else
				progress_down_flag = 0;
			}
			break;

	case MSG_TP_PRESS_UP:
		if(progress_down_flag)
		{
			//MMIAPISET_SetCallVolume(s_setting_call_volume_value);     
			//MMIAPISET_UiPlayRingByVolume(0, FALSE,0, 1, MMISET_RING_TYPE_KEY, PNULL, s_setting_call_volume_value);
			Settings_Volume_SoundUpdate(cur_call_volume_img_id);
		}
		progress_down_flag = 0;
         	break;
  #else
  	   case MSG_TP_PRESS_UP:
		{
			GUI_RECT_T   back_rect=WINDOW_BACK_RECT; 
			GUI_POINT_T   point = {0};

			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if (GUI_PointIsInRect(point, back_rect))
			{
				MMK_CloseWin( win_id);
				return;
			}
		}
         	break;
	   case MSG_KEYUP_UP:
		 {
    				if(cur_call_volume_img_id<5)
    				{
    					s_setting_call_volume_value = 2*cur_call_volume_img_id+1;
    					cur_call_volume_img_id++;
    				}
    				else
    				{
    					s_setting_call_volume_value = 9;//0;
    					cur_call_volume_img_id = 5;//0;
    				}				

    				MMIAPISET_SetCallVolume(s_setting_call_volume_value);     
    				
                    //MMIAPISET_UiPlayRingByVolume(0, FALSE,0, 1, MMISET_RING_TYPE_KEY, PNULL, s_setting_call_volume_value);
                    Settings_Volume_SoundUpdate(s_setting_call_volume_value);
    				
    				display_point.x = 66; // 35*BIG_SCALE;//35*SCALE;//client_rect.v_rect.left;
    				display_point.y = 54; //  74*BIG_SCALE;//103*SCALE;///client_rect.v_rect.top;	

     				bg_ptr.bg_type = GUI_BG_COLOR;
        				bg_ptr.color = MMI_BLACK_COLOR;
    				GUI_DisplayBg(&bg_ptr,&display_point,MMITHEME_GetDefaultLcdDev());
    				
    				GUIRES_DisplayImg(&display_point,
    				    PNULL,
    				    PNULL,
    				    win_id,
    				    Volume_Image_Id[cur_call_volume_img_id],
    				    &lcd_dev_info);

    				MMK_UpdateScreen();
		        }
		 break;
	case MSG_KEYUP_DOWN:
		{
    				if(cur_call_volume_img_id>1)
    				{
    					//s_setting_call_volume_value = 2*cur_call_volume_img_id+1;
    					s_setting_call_volume_value = 2*cur_call_volume_img_id-3;
    					cur_call_volume_img_id--;
    				}
    				else
    				{
    					s_setting_call_volume_value = 0;
    					cur_call_volume_img_id = 0;
    				}				

    				MMIAPISET_SetCallVolume(s_setting_call_volume_value);     
    				//MMIAPISET_UiPlayRingByVolume(0, FALSE,0, 1, MMISET_RING_TYPE_KEY, PNULL, s_setting_call_volume_value);
    				Settings_Volume_SoundUpdate(s_setting_call_volume_value);
    				display_point.x = 66; // 35*BIG_SCALE;//35*SCALE;//client_rect.v_rect.left;
    				display_point.y =  54; // 74*BIG_SCALE;//103*SCALE;///client_rect.v_rect.top;	

     				bg_ptr.bg_type = GUI_BG_COLOR;
        				bg_ptr.color = MMI_BLACK_COLOR;
    				GUI_DisplayBg(&bg_ptr,&display_point,MMITHEME_GetDefaultLcdDev());
    				
    				GUIRES_DisplayImg(&display_point,
    				    PNULL,
    				    PNULL,
    				    win_id,
    				    Volume_Image_Id[cur_call_volume_img_id],
    				    &lcd_dev_info);

    				MMK_UpdateScreen();
		 }
		 break;
#endif
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
	    MMK_CloseWin(win_id);
	    break;
    case MSG_KEYDOWN_RED:
        break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;    
    case MSG_CLOSE_WINDOW:
        //stop ring
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        /* resume background sound */ 
        MMISRVAUD_FreeVirtualHandle("FIX CALL SELECT RING");

        
        break;
	default:
	    recode = MMI_RESULT_FALSE;
	    break;
    }
	return recode;
}

LOCAL MMI_RESULT_E HandleRingVolumeSetWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_POINT_T         display_point = {0};
    GUI_RECT_T img_rect = {0};
    GUI_BG_T bg_ptr = {0};

#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)	
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};	
    GUI_RECT_T          text_rect={42,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 
#endif
    GUI_RECT_T          rect         = MMITHEME_GetFullScreenRectEx(win_id);	

#ifdef ZTE_WATCH
	MMI_IMAGE_ID_T         res_progress_bg = IMAGE_ZTE_CONTROL_PROGRESS_BG;
	MMI_IMAGE_ID_T         res_progress_fg= IMAGE_ZTE_CONTROL_PROGRESS;
	MMI_IMAGE_ID_T         res_progress_hand = IMAGE_ZTE_PROGRESS_HAND;
	GUI_RECT_T progress_volume_rect = SET_PROGRESSBAR_RECT;
	GUI_RECT_T setok_btn_rect =  WATCH_SETOK_BTN_RECT;
	uint32      cur_item_index = 0;
	uint32  volume_item_total = MMISET_VOL_MAX+1;
#else
	GUI_RECT_T leftBtnRect = SET_BTN_MINUS_RECT;
	GUI_RECT_T rightBtnRect = SET_BTN_PLUS_RECT;

    MMI_IMAGE_ID_T Volume_Image_Id[6]=
    {
        IMAGE_VOLUME_LEVEL0_CALL_RING,
        IMAGE_VOLUME_LEVEL1_CALL_RING,
        IMAGE_VOLUME_LEVEL2_CALL_RING,
        IMAGE_VOLUME_LEVEL3_CALL_RING,
        IMAGE_VOLUME_LEVEL4_CALL_RING,
        IMAGE_VOLUME_LEVEL5_CALL_RING
    };

    uint32 Volume_Level[6]=
    {
        0,
        1,
        3,
        5,
        7,
        9
    };
#endif
    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:	
         MMISRVAUD_ReqVirtualHandle("FIX CALL SELECT RING", MMISRVAUD_PRI_NORMAL);  
#ifdef ZTE_WATCH
		 progress_down_flag = 0;
		cur_ring_volume_img_id = MMIAPISET_GetCallRingVolume();
		 /* if(cur_ring_volume_img_id ==0)
			 current_percent_brightness = 0;
		 else
			 current_percent_brightness = (cur_ring_volume_img_id+1 )*SET_PROGRESSBAR_PERLEVEL_LENGTH*100/SET_PROGRESSBAR_TOTAL_LENGTH;
	
		WATCHCOM_ProgressBar(win_id, progressrect,current_percent_brightness,res_progress_bg,res_progress_fg,res_progress_hand);*/
		if(MMISET_VOL_MAX < cur_ring_volume_img_id)
			cur_ring_volume_img_id = MMISET_VOL_MAX;
		WATCHCOM_ProgressBarByIndex(win_id, progress_volume_rect, cur_ring_volume_img_id,volume_item_total,res_progress_bg, res_progress_fg, res_progress_hand);

		GUIBUTTON_SetRect(MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID, &setok_btn_rect);
#else
		
    		s_setting_ring_volume_value = MMIAPISET_GetCallRingVolume();
			
		if(0==s_setting_ring_volume_value)
		{
			cur_ring_volume_img_id = 0;			
		}
		else if((1 <= s_setting_ring_volume_value) && (s_setting_ring_volume_value <= 2))
		{
			cur_ring_volume_img_id = 1;			
		}
		else if((3 <= s_setting_ring_volume_value) && (s_setting_ring_volume_value <= 4))
		{
			cur_ring_volume_img_id = 2;	
		}
		else if((5<=s_setting_ring_volume_value)&&(s_setting_ring_volume_value<=6))
		{
			cur_ring_volume_img_id = 3;	
		}
		else if((7<=s_setting_ring_volume_value)&&(s_setting_ring_volume_value <= 8))
		{
			cur_ring_volume_img_id = 4;	
		}
		else
		{
			cur_ring_volume_img_id = 5;	
		}

		GUIBUTTON_SetRect(MMISET_COMMON_L_BTN_CTRL_ID, &leftBtnRect);
		GUIBUTTON_SetRect(MMISET_COMMON_R_BTN_CTRL_ID, &rightBtnRect);
#endif
	    break;
	case MSG_FULL_PAINT:	
		LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
#ifdef ZTE_WATCH
		display_point.x = BRIGHT_ICON_VOL_X; // 43*BIG_SCALE;//81*SCALE;//client_rect.v_rect.left;
		display_point.y = BRIGHT_ICON_VOL_Y; //  14*BIG_SCALE;//25*SCALE;///client_rect.v_rect.top;	

		GUIRES_DisplayImg(&display_point,
		    PNULL,
		    PNULL,
		    win_id,
		    IMAGE_VOLUME_ICON,
		    &lcd_dev_info);
		WATCHCOM_ProgressBarByIndex(win_id, progress_volume_rect, cur_ring_volume_img_id,volume_item_total,res_progress_bg, res_progress_fg, res_progress_hand);
		//WATCHCOM_ProgressBar(win_id, progressrect,current_percent_brightness,res_progress_bg,res_progress_fg,res_progress_hand);
#else
#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)			
		//draw bg		
			GUIRES_DisplayImg(PNULL,
			    &rect,
			    PNULL,
			    win_id,
			    IMAGE_THEME_BLACK_BG_SPEC, //xiongkai IMAGE_THEME_BG,
			    &lcd_dev_info); 

		///////draw title
		text_style.align = ALIGN_LVMIDDLE;
		text_style.font = SONG_FONT_28;
		text_style.font_color = MMI_WHITE_COLOR;
		text_style.char_space = 0;
	
    		MMI_GetLabelTextByLang(text_ringtone_volume_setting, &string);
		GUISTR_DrawTextToLCDInRect( 
		    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		    (const GUI_RECT_T      *)&text_rect,       //the fixed display area
		    (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
		    (const MMI_STRING_T    *)&string,
		    &text_style,
		    state,
		    GUISTR_TEXT_DIR_AUTO
		    );
#endif		
			display_point.x = BRIGHT_ICON_X; // 43*BIG_SCALE;//81*SCALE;//client_rect.v_rect.left;
			display_point.y =  BRIGHT_ICON_Y; // 14*BIG_SCALE;//25*SCALE;///client_rect.v_rect.top;	
			
			GUIRES_DisplayImg(&display_point,
			    PNULL,
			    PNULL,
			    win_id,
			    IMAGE_VOLUME_ICON_CALL_RING,
			    &lcd_dev_info);

			display_point.x =BRIGHT_IMAGE_X; // 35*BIG_SCALE;//35*SCALE;//client_rect.v_rect.left;
			display_point.y =BRIGHT_IMAGE_Y; //  74*BIG_SCALE;//103*SCALE;///client_rect.v_rect.top;		
			
			GUIRES_DisplayImg(&display_point,
			    PNULL,
			    PNULL,
			    win_id,
			    Volume_Image_Id[cur_ring_volume_img_id],
			    &lcd_dev_info);	

#endif
	    break;
		
	case MSG_CTL_MIDSK:
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
	case MSG_APP_WEB:
#if defined( TOUCH_PANEL_SUPPORT)||defined(ZDT_TOUCHPANEL_TYPE_MULTITP) //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef ZTE_WATCH
		if(MMISET_ZTE_WATCH_SETOK_BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//reset button被选中
		{
			/*if(cur_ring_volume_img_id<5)
			{
				s_setting_ring_volume_value = 2*cur_ring_volume_img_id+1;
			}
			else
			{
				s_setting_ring_volume_value = 9;//0;
				cur_ring_volume_img_id = 5;//0;
			}	*/				

			MMIAPISET_SetCallRingVolume(cur_ring_volume_img_id);     
			//MMIAPISET_UiPlayRingByVolume(0, FALSE,0, 1, MMISET_RING_TYPE_KEY, PNULL, s_setting_call_volume_value);
			//Settings_Volume_SoundUpdate(s_setting_call_volume_value);
			MMK_CloseWin(win_id);
		}

#else
		if(MMISET_COMMON_L_BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//reset button被选中
		{
			if(cur_ring_volume_img_id>1)
			{
				//s_setting_ring_volume_value = 2*cur_call_volume_img_id+1;
				s_setting_ring_volume_value = 2*cur_ring_volume_img_id-3;
				cur_ring_volume_img_id--;
			}
			else
			{
				s_setting_ring_volume_value = 0;
				cur_ring_volume_img_id = 0;
			}				

			MMIAPISET_SetCallRingVolume(s_setting_ring_volume_value); 
			//MMIAPISET_UiPlayRingByVolume(0, FALSE,0, 1, MMISET_RING_TYPE_KEY/*MMISET_RING_TYPE_TKEY*/, PNULL, s_setting_ring_volume_value);
			Settings_Volume_SoundUpdate(s_setting_ring_volume_value);
			
			display_point.x = BRIGHT_IMAGE_X; // 35*BIG_SCALE;//35*SCALE;//client_rect.v_rect.left;
			display_point.y = BRIGHT_IMAGE_Y; // 74*BIG_SCALE;//103*SCALE;///client_rect.v_rect.top;	

			bg_ptr.bg_type = GUI_BG_COLOR;
			bg_ptr.color = MMI_BLACK_COLOR;
			GUI_DisplayBg(&bg_ptr,&display_point,MMITHEME_GetDefaultLcdDev());
			
			GUIRES_DisplayImg(&display_point,
			    PNULL,
			    PNULL,
			    win_id,
			    Volume_Image_Id[cur_ring_volume_img_id],
			    &lcd_dev_info);

			MMK_UpdateScreen();
		}
		else if(MMISET_COMMON_R_BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//play button被选中
		{
			if(cur_ring_volume_img_id<5)
			{
				s_setting_ring_volume_value = 2*cur_ring_volume_img_id+1;
				cur_ring_volume_img_id++;
			}
			else
			{
				s_setting_ring_volume_value = 9;//0;
				cur_ring_volume_img_id = 5;//0;
			}				

			MMIAPISET_SetCallRingVolume(s_setting_ring_volume_value); 
			//MMIAPISET_UiPlayRingByVolume(0, FALSE,0, 1, MMISET_RING_TYPE_KEY/*MMISET_RING_TYPE_TKEY*/, PNULL, s_setting_ring_volume_value);
			Settings_Volume_SoundUpdate(s_setting_ring_volume_value);
			
			display_point.x =  BRIGHT_IMAGE_X; // 35*BIG_SCALE;//35*SCALE;//client_rect.v_rect.left;
			display_point.y =  BRIGHT_IMAGE_Y; // 74*BIG_SCALE;//103*SCALE;///client_rect.v_rect.top;	

			bg_ptr.bg_type = GUI_BG_COLOR;
			bg_ptr.color = MMI_BLACK_COLOR;
			GUI_DisplayBg(&bg_ptr,&display_point,MMITHEME_GetDefaultLcdDev());
			
			GUIRES_DisplayImg(&display_point,
			    PNULL,
			    PNULL,
			    win_id,
			    Volume_Image_Id[cur_ring_volume_img_id],
			    &lcd_dev_info);

			MMK_UpdateScreen();
		}
	    break;
#endif

#ifdef ZTE_WATCH
	case MSG_TP_PRESS_DOWN:
	{
		        GUI_POINT_T   point = {0};
		        point.x = MMK_GET_TP_X(param);
		        point.y = MMK_GET_TP_Y(param);
			 if (GUI_PointIsInRect(point, progress_volume_rect))
			{
				//uint8 percent = (point.x - rect.left)*100/(rect.right-rect.left);
				progress_down_flag = 1;	
				cur_item_index =  ROUND((float)(volume_item_total)
			                      *(point.x - progress_volume_rect.left)
			                      /(progress_volume_rect.right-progress_volume_rect.left)); //0 is not in total num, eg total=100, progressbar display 0~100
				if(cur_item_index != cur_ring_volume_img_id)
				{
					cur_ring_volume_img_id = cur_item_index;
					WATCHCOM_ProgressBarByIndex(win_id, progress_volume_rect, cur_ring_volume_img_id,volume_item_total,res_progress_bg, res_progress_fg, res_progress_hand);
					MMK_UpdateScreen();
				}
				/*current_percent_brightness = (point.x - rect.left)*100/(rect.right-rect.left);
				WATCHCOM_ProgressBar(win_id, progressrect, current_percent_brightness,res_progress_bg, res_progress_fg, res_progress_hand);
				cur_ring_volume_img_id = Get_ProgressBar_CurrentIndex(current_percent_brightness,SET_PROGRESSBAR_TOTAL_LENGTH,SET_PROGRESSBAR_PERLEVEL_LENGTH);
				if(cur_ring_volume_img_id>1)
    				{
    					s_setting_ring_volume_value = 2*cur_ring_volume_img_id-2;
    				}
    				else
    				{
    					s_setting_ring_volume_value = cur_ring_volume_img_id;//0;
    					//cur_ring_volume_img_id = 5;//0;
    				}
				MMK_UpdateScreen();*/
				
			} 			
	    	}	
	      break;	

	  case MSG_TP_PRESS_MOVE:
			{
			
			GUI_POINT_T   point = {0};              
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if (progress_down_flag /*&& GUI_PointIsInRect(point, progressrect)*/)
			{
				//uint8 percent = (point.x - rect.left)*100/(rect.right-rect.left);
				/*current_percent_brightness= (point.x - rect.left)*100/(rect.right-rect.left);
				current_percent_brightness = (point.x - rect.left)*100/(rect.right-rect.left);
				WATCHCOM_ProgressBar(win_id, progressrect, current_percent_brightness,res_progress_bg, res_progress_fg, res_progress_hand);
				cur_ring_volume_img_id = Get_ProgressBar_CurrentIndex(current_percent_brightness,SET_PROGRESSBAR_TOTAL_LENGTH,SET_PROGRESSBAR_PERLEVEL_LENGTH);
				if(cur_ring_volume_img_id>1)
    				{
    					s_setting_ring_volume_value = 2*cur_ring_volume_img_id-2;
    				}
    				else
    				{
    					s_setting_ring_volume_value = cur_ring_volume_img_id;//0;
    					//cur_ring_volume_img_id = 5;//0;
    				}
				MMK_UpdateScreen();*/
				cur_item_index =  ROUND((float)(volume_item_total)
			                      *(point.x - progress_volume_rect.left)
			                      /(progress_volume_rect.right-progress_volume_rect.left)); //0 is not in total num, eg total=100, progressbar display 0~100
				if(cur_item_index != cur_ring_volume_img_id)
				{
					cur_ring_volume_img_id = cur_item_index;
					WATCHCOM_ProgressBarByIndex(win_id, progress_volume_rect, cur_ring_volume_img_id,volume_item_total,res_progress_bg, res_progress_fg, res_progress_hand);
					MMK_UpdateScreen();
				}
			}
			else
				progress_down_flag = 0;
			}
			break;

	case MSG_TP_PRESS_UP:
		if(progress_down_flag)
		{
			/*if(cur_ring_volume_img_id>1)
			{
				s_setting_ring_volume_value = 2*cur_ring_volume_img_id-2;
			}
			else
			{
				s_setting_ring_volume_value = cur_ring_volume_img_id;//0;
				//cur_ring_volume_img_id = 5;//0;
			}*/	

			//MMIAPISET_SetCallVolume(s_setting_ring_volume_value);     
			//MMIAPISET_UiPlayRingByVolume(0, FALSE,0, 1, MMISET_RING_TYPE_KEY, PNULL, s_setting_call_volume_value);
			Settings_Volume_SoundUpdate(cur_ring_volume_img_id);
		}
		progress_down_flag = 0;
         	break;
  #else
  		case MSG_TP_PRESS_UP:
		{
			GUI_RECT_T   back_rect=WINDOW_BACK_RECT; 
			GUI_POINT_T   point = {0};

			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if (GUI_PointIsInRect(point, back_rect))
			{
				MMK_CloseWin( win_id);
				return;
			}
		}
         	break;
		case MSG_KEYUP_UP:
		{
    				if(cur_ring_volume_img_id<5)
    				{
    					s_setting_ring_volume_value = 2*cur_ring_volume_img_id+1;
    					cur_ring_volume_img_id++;
    				}
    				else
    				{
    					s_setting_ring_volume_value = 9;//0;
    					cur_ring_volume_img_id = 5;//0;
    				}				

    				MMIAPISET_SetCallRingVolume(s_setting_ring_volume_value); 
    				//MMIAPISET_UiPlayRingByVolume(0, FALSE,0, 1, MMISET_RING_TYPE_KEY/*MMISET_RING_TYPE_TKEY*/, PNULL, s_setting_ring_volume_value);
    				Settings_Volume_SoundUpdate(s_setting_ring_volume_value);
    				
    				display_point.x =  BRIGHT_IMAGE_X; // 35*BIG_SCALE;//35*SCALE;//client_rect.v_rect.left;
    				display_point.y =  BRIGHT_IMAGE_Y; // 74*BIG_SCALE;//103*SCALE;///client_rect.v_rect.top;	

     				bg_ptr.bg_type = GUI_BG_COLOR;
        				bg_ptr.color = MMI_BLACK_COLOR;
    				GUI_DisplayBg(&bg_ptr,&display_point,MMITHEME_GetDefaultLcdDev());
    				
    				GUIRES_DisplayImg(&display_point,
    				    PNULL,
    				    PNULL,
    				    win_id,
    				    Volume_Image_Id[cur_ring_volume_img_id],
    				    &lcd_dev_info);

    				MMK_UpdateScreen();
		        }
		 break;
	case MSG_KEYUP_DOWN:
		{
    				if(cur_ring_volume_img_id>1)
    				{
    					//s_setting_ring_volume_value = 2*cur_call_volume_img_id+1;
    					s_setting_ring_volume_value = 2*cur_ring_volume_img_id-3;
    					cur_ring_volume_img_id--;
    				}
    				else
    				{
    					s_setting_ring_volume_value = 0;
    					cur_ring_volume_img_id = 0;
    				}				

    				MMIAPISET_SetCallRingVolume(s_setting_ring_volume_value); 
    				//MMIAPISET_UiPlayRingByVolume(0, FALSE,0, 1, MMISET_RING_TYPE_KEY/*MMISET_RING_TYPE_TKEY*/, PNULL, s_setting_ring_volume_value);
    				Settings_Volume_SoundUpdate(s_setting_ring_volume_value);
    				
    				display_point.x = BRIGHT_IMAGE_X; // 35*BIG_SCALE;//35*SCALE;//client_rect.v_rect.left;
    				display_point.y = BRIGHT_IMAGE_Y; // 74*BIG_SCALE;//103*SCALE;///client_rect.v_rect.top;	

     				bg_ptr.bg_type = GUI_BG_COLOR;
        			bg_ptr.color = MMI_BLACK_COLOR;
    				GUI_DisplayBg(&bg_ptr,&display_point,MMITHEME_GetDefaultLcdDev());
    				
    				GUIRES_DisplayImg(&display_point,
    				    PNULL,
    				    PNULL,
    				    win_id,
    				    Volume_Image_Id[cur_ring_volume_img_id],
    				    &lcd_dev_info);

    				MMK_UpdateScreen();
		        }
		break;
#endif
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
	    MMK_CloseWin(win_id);
	    break;
    case MSG_KEYDOWN_RED:
            break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;    
    case MSG_CLOSE_WINDOW:
        //stop ring
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        /* resume background sound */ 
        MMISRVAUD_FreeVirtualHandle("FIX CALL SELECT RING");
        break;
	default:
	    recode = MMI_RESULT_FALSE;
	    break;
    }
	return recode;
}


/*****************************************************************************/
//  Description :HandleSetSystemSettingWindow
//  Global resource dependence :
//  Author:James li
//  Note:
/*****************************************************************************/ 

#ifndef FLASH_SUPPORT
/*****************************************************************************/
//  Description : handle prompt win of theme
//  Global resource dependence :
//  Author: lu.lifei
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePromptCompleteThemeWinW217(
                                                MMI_WIN_ID_T    win_id, // 窗口的ID
                                                MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                DPARAM             param   // 相应消息的参数
                                                )
{
    MMI_RESULT_E    result  =   MMI_RESULT_FALSE;

    switch(msg_id)
    {
    case MSG_CLOSE_WINDOW:
        if(MMK_IsOpenWin(MMISET_INPUT_RESET_FACTORY_PWD_WIN_ID))
        {
            MMK_CloseWin(MMISET_INPUT_RESET_FACTORY_PWD_WIN_ID);
        }
#ifdef MULTI_THEME_SUPPORT
        else if(MMK_IsOpenWin(MMISET_SET_THEME_PREVIEW_WIN_ID))
        {
            MMK_CloseWin(MMISET_SET_THEME_PREVIEW_WIN_ID);
        }
        //        else if(MMK_IsOpenWin(MMISET_SET_THEME_WIN_ID))
        //        {
        //            MMK_CloseWin(MMISET_SET_THEME_WIN_ID);
        //         }
#endif
        break;

    default:
        break;
    }

    result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
    return (result);
}
#endif
/*****************************************************************************/
//  Description : 处理恢复出厂设置和清楚用户数据的等待窗口
//  Global resource dependence :
//  Author: lu.lifei
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleResetOrCleanDataWaitWinW217(
                                                 MMI_WIN_ID_T    win_id, // 窗口的ID
                                                 MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                 DPARAM             param   // 相应消息的参数
                                                 )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    static uint32   s_waitting_num = 0;
    static BOOLEAN is_cleandata =FALSE;
    SCI_TRACE_LOW("mmiset_displaywin.c HandleResetOrCleanDataWaitWin: msg_id=0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_waitting_num = 0;
       GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL,RIGHT_BUTTON,TRUE);
       GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
       GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, TRUE);
        //recode  =   MMI_RESULT_FALSE;
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_KEYDOWN_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
//yangyu add begin
    case MSG_FULL_PAINT:
    {
        GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        GUI_RECT_T rect = {0,0, MMI_MAINSCREEN_WIDTH - 1, MMI_MAINSCREEN_HEIGHT- 1};
        
        GUI_RECT_T	text_rect={0,MMI_MAINSCREEN_HEIGHT/2-15, MMI_MAINSCREEN_WIDTH - 1 , MMI_MAINSCREEN_HEIGHT/2+15};
    	MMI_STRING_T tmp_string = {0};
    	MMI_STRING_T        content_str = {0};
    	wchar               content_text[100 + 1] = {0};
    	uint16 uint16_str_len = 0;
    	GUISTR_STYLE_T      text_style = {0};
    	GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    	text_style.align = ALIGN_HVMIDDLE; // 这里设置为Left,因为Label的位置自有自己调整区域
    	text_style.font_color = MMI_GREEN_COLOR;
    	text_style.angle = ANGLE_0;
    	text_style.char_space = 0;
    	text_style.effect = FONT_EFFECT_CUSTOM;
    	text_style.edge_color = 0;
#if defined(ZDT_MMI_USE_SMALL_FONT)
    	text_style.font = SONG_FONT_22;//SONG_FONT_36*SCALE;//SONG_FONT_26;
#else
    	text_style.font = SONG_FONT_28;//SONG_FONT_36*SCALE;//SONG_FONT_26;
#endif    	
    	text_style.line_space = 0;
    	text_style.region_num = 0;
    	text_style.region_ptr = PNULL;	
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_WATCH_RESETING,&tmp_string);
    	text_style.align = ALIGN_HVMIDDLE;
    	text_style.font_color = MMI_WHITE_COLOR;
	#if defined(ZDT_MMI_USE_SMALL_FONT)
    	text_style.font = SONG_FONT_22;//SONG_FONT_26*SCALE;	
	#else
    	text_style.font = SONG_FONT_28;//SONG_FONT_26*SCALE;
    	#endif
    	
    	GUI_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
    	GUISTR_DrawTextToLCDInRect( 
    		        MMITHEME_GetDefaultLcdDev(),
    		        (const GUI_RECT_T      *)&text_rect,       //the fixed display area
    		        (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
    		        (const MMI_STRING_T    *)&tmp_string,
    		        &text_style,
    		        state,
    		        GUISTR_TEXT_DIR_AUTO
    		        );	
    }
        break;
//yangyu end
    case MSG_KEYDOWN_RED:
        break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;
    case MSG_SET_RESET_NEED_WAIT_IND:
        //add one wait action
        s_waitting_num++;
        SCI_TRACE_LOW("mmiset_displaywin.c HandleResetOrCleanDataWaitWin: add s_waitting_num=%d", s_waitting_num);
        break;
    case MSG_SET_RESET_FACTORY_OVER_IND:
        if(s_waitting_num > 0)
        {
            //finished one, wait number -1
            s_waitting_num --;
        }
        SCI_TRACE_LOW("mmiset_displaywin.c HandleResetOrCleanDataWaitWin: minus s_waitting_num=%d, is_cleandata=%d", s_waitting_num, is_cleandata);
        //no one to wait, finished
        if(s_waitting_num == 0)
        {
            if(is_cleandata)
            {
                //MMK_CloseWin(win_id);
                //MMIAPIPHONE_PowerReset();//yangyu modify
               MMIAPIPHONE_PowerResetByFactoryReset();
               //MMIAPIPHONE_PowerReset(); //重启
                
            }
            else
            {
                MMK_CloseWin(win_id);
                //MMK_ReturnIdleWin(); //crMS00187558 @murphy.xie
                //success!
#ifndef FLASH_SUPPORT
                //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePromptCompleteThemeWinW217);
#endif
            }
        }
        break;

    case MSG_SET_CLEAN_USER_DATA_OVER_IND:
        is_cleandata = TRUE;
        break;

    case MSG_CLOSE_WINDOW:
        //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_RESET_FACT, TRUE);
#ifdef BLUETOOTH_SUPPORT
        MMIAPIBT_ClearAllDevice();
#endif
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return recode;
}
LOCAL MMI_RESULT_E HandlePowerRestartSelectWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    GUI_RECT_T          rect         = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};    
    
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};
	
    GUI_RECT_T leftBtnRect = SET_BTN_TWO_LEFT_RECT;
    GUI_RECT_T rightBtnRect = SET_BTN_TWO_RIGHT_RECT;

    MMI_TEXT_ID_T    text_id = 0;
    GUI_RECT_T	content_rect= MMITHEME_GetFullScreenRect();//{0,MMI_SPECIAL_TITLE_HEIGHT,240,240}; 
    rect.top =  0;
	content_rect.bottom = leftBtnRect.top;
	
    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:
		{
		GUI_FONT_T font = SONG_FONT_24;//SONG_FONT_16;
		GUI_COLOR_T color = MMI_WHITE_COLOR;	
		GUI_BG_T text_bg;
		wchar*          		pDisplayWStr = PNULL;
		uint16          		displayWStrLen = 0;
		wchar           		newLineCode = CR_CHAR;
		MMI_STRING_T    	newLineStr = { 0 };
		
		pDisplayWStr = (wchar*)SCI_ALLOC_APPZ( sizeof(wchar)*(255+1) );
		if( PNULL == pDisplayWStr )
		{
		TRACE_APP_SETTINGS("pDisplayWStr is PNULL!!");
		return;
		}
		newLineStr.wstr_ptr = &newLineCode;
		newLineStr.wstr_len = 1;
		LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR); 
		
#ifndef ZTE_WATCH		
		GUIBUTTON_SetRect(MMISET_BRIGHTNESS_L_BTN_CTRL_ID, &leftBtnRect);
    		GUIBUTTON_SetRect(MMISET_BRIGHTNESS_R_BTN_CTRL_ID, &rightBtnRect);
		CTRLBUTTON_SetTextId(MMISET_BRIGHTNESS_L_BTN_CTRL_ID,TXT_COMMON_OK );
		CTRLBUTTON_SetTextId(MMISET_BRIGHTNESS_R_BTN_CTRL_ID,STXT_RETURN );
		CTRLBUTTON_SetFontSize(MMISET_BRIGHTNESS_L_BTN_CTRL_ID,SONG_FONT_24 );
		CTRLBUTTON_SetFontSize(MMISET_BRIGHTNESS_R_BTN_CTRL_ID,SONG_FONT_24);
		CTRLBUTTON_SetFontColor(MMISET_BRIGHTNESS_L_BTN_CTRL_ID,MMI_WHITE_COLOR );
		CTRLBUTTON_SetFontColor(MMISET_BRIGHTNESS_R_BTN_CTRL_ID,MMI_WHITE_COLOR);
#else  
		GUIBUTTON_SetRect(MMISET_BRIGHTNESS_L_BTN_CTRL_ID, &rightBtnRect); //ZTE确定取消需要对调
    		GUIBUTTON_SetRect(MMISET_BRIGHTNESS_R_BTN_CTRL_ID, &leftBtnRect);
#endif

		if(g_reboot_screen_show_style == REBOOT_STYLE_PWROFF_ONLY)
			text_id = STR_QUERY_POWEROFF_NOTIFY;
		else if(g_reboot_screen_show_style == REBOOT_STYLE_RESTART_ONLY)
			text_id= STR_QUERY_RESTART_NOTIFY;
		else if(g_reboot_screen_show_style == REBOOT_STYLE_RESTORE_ONLY)
			text_id= STR_QUERY_RESET_SETTINGS_EXT01;//TXT_COMMON_RESTORE_FACTORY_SETTINGS;//STR_QUERY_RESET_SETTINGS_EXT01;

		MMIAPICOM_StrcatFromTextIDToUCS2(pDisplayWStr,&displayWStrLen,text_id);

		CTRLTEXT_SetAlign(MMISET_SETTIPS_TEXT_CTRL_ID,ALIGN_HVMIDDLE);
		GUITEXT_IsDisplayPrg(FALSE,MMISET_SETTIPS_TEXT_CTRL_ID);
		GUITEXT_SetRect(MMISET_SETTIPS_TEXT_CTRL_ID,&content_rect);
		GUITEXT_SetFont(MMISET_SETTIPS_TEXT_CTRL_ID, &font, &color);
		text_bg.bg_type = GUI_BG_COLOR;
		text_bg.color = MMI_BLACK_COLOR;
		GUITEXT_SetBg(MMISET_SETTIPS_TEXT_CTRL_ID, &text_bg);
		GUITEXT_SetClipboardEnabled (MMISET_SETTIPS_TEXT_CTRL_ID,FALSE);//bug 2087169
		GUITEXT_IsSlide(MMISET_SETTIPS_TEXT_CTRL_ID,FALSE);
        CTRLTEXT_SetHandleTpMsg(FALSE,MMISET_SETTIPS_TEXT_CTRL_ID);
		GUITEXT_SetString( MMISET_SETTIPS_TEXT_CTRL_ID, pDisplayWStr, displayWStrLen, FALSE );

		SCI_FREE( pDisplayWStr );
		}
	    break;
	case MSG_FULL_PAINT:
		 LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR); 
		
		/*text_style.align = ALIGN_HVMIDDLE;
		text_style.font = SONG_FONT_24;
		text_style.font_color = MMI_WHITE_COLOR;
		text_style.char_space = 0;

		if(g_reboot_screen_show_style == REBOOT_STYLE_PWROFF_ONLY)
			text_id = STR_QUERY_POWEROFF_NOTIFY;
		else if(g_reboot_screen_show_style == REBOOT_STYLE_RESTART_ONLY)
			text_id= STR_QUERY_RESTART_NOTIFY;
		else if(g_reboot_screen_show_style == REBOOT_STYLE_RESTORE_ONLY)
			text_id= STR_QUERY_RESET_SETTINGS_EXT01;//TXT_COMMON_RESTORE_FACTORY_SETTINGS;//STR_QUERY_RESET_SETTINGS_EXT01;
	
    	MMI_GetLabelTextByLang(text_id, &string);
		// display
		GUISTR_DrawTextToLCDInRect( 
		    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		    (const GUI_RECT_T      *)&rect,       //the fixed display area
		    (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
		    (const MMI_STRING_T    *)&string,
		    &text_style,
		    state,
		    GUISTR_TEXT_DIR_AUTO
		    ); */
                //MMIPUB_OpenAlertWarningWin(text_comfirm_poweroff_selection);
	    break;
	case MSG_SET_CLEAN_DATE_IND:
		{
			//清除所有数据
			SCI_TRACE_LOW("mmiset_about_handleConfirmationDialog clearalldata reset");
			MMISET_CleanUserData();
			MMISET_ResetFactorySetting();
			MMK_CloseWin(win_id);        
        }
        break;	
	case MSG_CTL_MIDSK:
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
	case MSG_APP_WEB:
#if defined( TOUCH_PANEL_SUPPORT)||defined(ZDT_TOUCHPANEL_TYPE_MULTITP) //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	    //MMIAPIPHONE_PowerReset(); //开机
	    if(MMISET_BRIGHTNESS_L_BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//reset button被选中
        {
			if(g_reboot_screen_show_style == REBOOT_STYLE_PWROFF_ONLY)
				//MMIAPIPHONE_PowerOff(); //关机
				MMIPHONE_PowerOff();
			else if(g_reboot_screen_show_style == REBOOT_STYLE_RESTART_ONLY)
				MMIAPIPHONE_PowerReset(); //重启
			else if(g_reboot_screen_show_style == REBOOT_STYLE_RESTORE_ONLY)
			{
				//MMISET_CleanUserData();
                //MMISET_ResetFactorySetting();
                //MMK_CloseWin(win_id);
#if 1
				MMI_STRING_T    wait_text = {0};
				MMI_GetLabelTextByLang (STR_NOTE_WAITING, &wait_text);
				MMIPUB_OpenWaitWin (1, &wait_text, PNULL, PNULL, MMISET_RESET_FACTORY_WAITING_WIN_ID,
				    COMMON_IMAGE_NULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL,
				    MMIPUB_SOFTKEY_NONE, HandleResetOrCleanDataWaitWinW217);

				MMK_PostMsg(win_id/*MMISET_SET_POWER_RESTART_WIN_ID*/, MSG_SET_CLEAN_DATE_IND, PNULL,PNULL);
#endif				
				//ZdtWatch_Factory_RESTORE_Start(MMIZDT_WATCH_FACTORY_WIN_ID);
				//ZdtWatch_Factory_RESTORE_Start(win_id);
			}
            
        }
        else if(MMISET_BRIGHTNESS_R_BTN_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//play button被选中
        {
           MMK_CloseWin(win_id); //取消
        }
	    break;
	case MSG_TP_PRESS_UP:
	    	/*{
		        GUI_POINT_T   point = {0};
		        point.x = MMK_GET_TP_X(param);
		        point.y = MMK_GET_TP_Y(param);
		        if((point.x > 5&& point.x <110)&&(point.y > 190 &&  point.y < 240 ))
		        {
		    		MMIAPIPHONE_PowerReset(); //开机
		        }
				else if((point.x > 130 && point.x <240)&&(point.y > 190 &&  point.y < 240 ))
		        {
		    		MMK_CloseWin(win_id); //取消
		        }
	    	}*/
         	break;
		 
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
	    MMK_CloseWin(win_id);
	    break;
    case MSG_KEYDOWN_RED:
        break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;    
	default:
	    recode = MMI_RESULT_FALSE;
	    break;
    }
	return recode;
}

#define WATCH_MODE_MANUF        ZDT_SFR_MANUF
#define WATCH_MODE_NAME        ZDT_SFR_TYPE
#define WATCH_HARDWARE_VERSION  ZDT_SFR_HW_VER
#define WATCH_SOFTWARE_VERSION  ZDT_SFR_SW_VER 

/*****************************************************************************/
//  Description :HandleSetSystemSettingWindow
//  Global resource dependence :
//  Author:James li
//  Note:
/*****************************************************************************/ 
LOCAL MMI_RESULT_E HandleWatchInfoSelectWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    GUI_RECT_T          rect         = MMITHEME_GetFullScreenRect();
    //GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};    
   // GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
   // GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_BOLD;//|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        content_str = {0};
    wchar               content_text[MMISET_WATCH_TEXT_LEN + 1] = {0};
    MN_IMEISVN_T imeisvn = {0};/*lint !e64*/
    uint8 imei_str[(MN_MAX_IMEI_LENGTH<<1) + 1] = {0};
    char*           temp_ptr = WATCH_SOFTWARE_VERSION;//PNULL;
    uint16          temp_len = 0;
    uint16 imei_len = 0;
    uint16 uint16_str_len = 0;
    uint8 i;
    MMI_STRING_T    title_str = {0};
    MMI_STRING_T    cr_s = {0};
    wchar           unicode_cr = 0x0d;
	GUI_BG_T text_bg;
	wchar*           pDisplayWStr = PNULL;
	 uint16           displayWStrLen = 0;

#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)	
		GUI_RECT_T			content_rect={0,MMI_SPECIAL_TITLE_HEIGHT,240,240}; 
		GUI_RECT_T			title_rect={0,0,240,MMI_SPECIAL_TITLE_HEIGHT};	
		GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
		GUISTR_STYLE_T		text_style		= {0};/*lint !e64*/
		GUISTR_STATE_T		state = 		GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
		MMI_STRING_T		string = {0};	
		GUI_RECT_T			text_rect={42,0,240,MMI_SPECIAL_TITLE_HEIGHT};	
#endif

    cr_s.wstr_ptr = &unicode_cr;
    cr_s.wstr_len = 1;
    rect.top =  0;
	
    switch (msg_id)
    {
	case MSG_OPEN_WINDOW: 



	{
	#if defined(ZDT_MMI_USE_SMALL_FONT)
	    GUI_FONT_T font = SONG_FONT_20;//SONG_FONT_16;	
	#else
	    GUI_FONT_T font = SONG_FONT_24;//SONG_FONT_16;
	#endif	    
	    GUI_COLOR_T color = MMI_WHITE_COLOR;

	//pDisplayWStr = (wchar*)SCI_ALLOC_APPZ( sizeof(wchar)*(255+1) );

#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)		
	GUITEXT_SetRect(MMIZDT_WATCHINFO_TEXT_ID,&content_rect);

	//draw TITLE bg 	
	GUIRES_DisplayImg(PNULL,
						&rect,
						PNULL,
						win_id,
						IMAGE_THEME_BLACK_BG_SPEC, 
						&lcd_dev_info); 
		
	///////draw title
	text_style.align = ALIGN_LVMIDDLE;
	text_style.font = SONG_FONT_28;
	text_style.font_color = MMI_WHITE_COLOR;
	text_style.char_space = 0;
			
	MMI_GetLabelTextByLang(TEXT_WATCH_INFO, &string);
	GUISTR_DrawTextToLCDInRect( 
					(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
					(const GUI_RECT_T	   *)&text_rect,	   //the fixed display area
					(const GUI_RECT_T	   *)&text_rect,	   //用户要剪切的实际区域
					(const MMI_STRING_T    *)&string,
					&text_style,
					state,
					GUISTR_TEXT_DIR_AUTO
					);
#else		
	    GUITEXT_SetRect(MMIZDT_WATCHINFO_TEXT_ID, &rect);
#endif
	    GUITEXT_SetFont(MMIZDT_WATCHINFO_TEXT_ID, &font, &color);
		text_bg.bg_type = GUI_BG_COLOR;
		text_bg.color = MMI_BLACK_COLOR;
		GUITEXT_SetBg(MMIZDT_WATCHINFO_TEXT_ID, &text_bg);

		/*MMIAPICOM_StrcatFromTextIDToUCS2(pDisplayWStr,&uint16_str_len,TXT_WATCH_MODEL_NAME);
		content_str.wstr_ptr = content_text;
		content_str.wstr_len = MMIAPICOM_Wstrlen(content_text);
		GUITEXT_SetString(MMIZDT_WATCHINFO_TEXT_ID, content_str.wstr_ptr,content_str.wstr_len , FALSE);
		//GUITEXT_SetString(MMIZDT_WATCHINFO_TEXT_ID, pDisplayWStr,uint16_str_len , FALSE);

		MMIAPICOM_StrcatFromTextIDToUCS2(content_text,&uint16_str_len,TXT_WATCH_MODEL_NAME);
		MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
		//GUITEXT_SetString(MMIZDT_WATCHINFO_TEXT_ID, content_text,uint16_str_len , FALSE);
		

		content_str.wstr_ptr = content_text;
		content_str.wstr_len = MMIAPICOM_Wstrlen(content_text);
		GUITEXT_SetString(MMIZDT_WATCHINFO_TEXT_ID, content_str.wstr_ptr,content_str.wstr_len , FALSE);*/

		#if 1
		//MMI_GetLabelTextByLang(TXT_WATCH_MODEL_NAME, &title_str);
		//MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &title_str);
		MMIAPICOM_StrcatFromTextIDToUCS2(content_text,&uint16_str_len,TXT_WATCH_MODEL_NAME);
#if 1
		MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
		MMIAPICOM_StrcatFromStrToUCS2(content_text, &uint16_str_len, WATCH_MODE_NAME, strlen(WATCH_MODE_NAME));
	    MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行

		MMI_GetLabelTextByLang(TXT_WATCH_IMEI, &title_str);
		MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &title_str);
		MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行

#if 0
		for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
		   {
		    if (MNNV_GetIMEISVNEx(i, &imeisvn))
		       {
		         
				 MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)imeisvn.imei, MN_MAX_IMEI_LENGTH<<1, (char*)imei_str);
				 
				 imei_len = (uint16)strlen((char *)imei_str) - 1;
				 MMIAPICOM_StrcatFromStrToUCS2(content_text, &uint16_str_len, (uint8 *)&imei_str[1], imei_len);
				 MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
		       }
		   }
		#endif

		//MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
		
		//temp_ptr = (char*)"Software Version";
		//temp_len = SCI_STRLEN((char*)temp_ptr);
		//MMIAPICOM_StrcatFromStrToUCS2(content_text, &uint16_str_len, (uint8*)temp_ptr, temp_len);
		//MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行

		
		#ifndef WIN32
		//temp_ptr = VERSION_GetInfo(PROJECT_VERSION);
		#endif
		//temp_len = SCI_STRLEN((char*)temp_ptr);
		//MMIAPICOM_StrcatFromStrToUCS2(content_text, &uint16_str_len, (uint8*)temp_ptr, temp_len);
		//MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
		
		// SwVersionInfo(content_text);
		//yangyu add
		MMI_GetLabelTextByLang(TXT_WATCH_SOFTWARE_VERSION, &title_str);
        MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &title_str);
        MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
        //temp_ptr = ZDT_GetVersionInfo();
        temp_len = SCI_STRLEN((char*)temp_ptr);
        MMIAPICOM_StrcatFromStrToUCS2(content_text, &uint16_str_len, (uint8*)temp_ptr, temp_len);
		MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
		//yangyu end

		
        MMI_GetLabelTextByLang(TXT_WATCH_HARDWARE_VERSION, &title_str);
        MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &title_str);
        MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
        MMIAPICOM_StrcatFromStrToUCS2(content_text, &uint16_str_len, WATCH_HARDWARE_VERSION, strlen(WATCH_HARDWARE_VERSION));
        //MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
	
		content_str.wstr_ptr = content_text;
		content_str.wstr_len = MMIAPICOM_Wstrlen(content_text);

        #if 0
		// display
		GUISTR_DrawTextToLCDInRect( 
		    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		    (const GUI_RECT_T      *)&rect,       //the fixed display area
		    (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
		    (const MMI_STRING_T    *)&content_str,
		    &text_style,
		    state,
		    GUISTR_TEXT_DIR_AUTO
		    );
                //MMIPUB_OpenAlertWarningWin(text_comfirm_poweroff_selection);
        #endif
		
        GUITEXT_SetString(MMIZDT_WATCHINFO_TEXT_ID, content_str.wstr_ptr,content_str.wstr_len , FALSE);
		#endif
		#endif

		//SCI_FREE( pDisplayWStr );
    }
	    break;

	case MSG_FULL_PAINT:	
		
#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)			
	//draw TITLE bg 	
	GUIRES_DisplayImg(PNULL,
						&rect,
						PNULL,
						win_id,
						IMAGE_THEME_BLACK_BG_SPEC, 
						&lcd_dev_info); 
		
	///////draw title
	text_style.align = ALIGN_LVMIDDLE;
	text_style.font = SONG_FONT_28;
	text_style.font_color = MMI_WHITE_COLOR;
	text_style.char_space = 0;
			
	MMI_GetLabelTextByLang(TEXT_WATCH_INFO, &string);
	GUISTR_DrawTextToLCDInRect( 
					(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
					(const GUI_RECT_T	   *)&text_rect,	   //the fixed display area
					(const GUI_RECT_T	   *)&text_rect,	   //用户要剪切的实际区域
					(const MMI_STRING_T    *)&string,
					&text_style,
					state,
					GUISTR_TEXT_DIR_AUTO
					);
#endif

#if 0
		//MMI_GetLabelTextByLang(TXT_WATCH_MODEL_NAME, &title_str);
		//MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &title_str);
		MMIAPICOM_StrcatFromTextIDToUCS2(content_text,&uint16_str_len,TXT_WATCH_MODEL_NAME);
#if 1
		MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
		MMIAPICOM_StrcatFromStrToUCS2(content_text, &uint16_str_len, WATCH_MODE_NAME, strlen(WATCH_MODE_NAME));
	    MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行

		MMI_GetLabelTextByLang(TXT_WATCH_IMEI, &title_str);
		MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &title_str);
		MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行

#if 0
		for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
		   {
		    if (MNNV_GetIMEISVNEx(i, &imeisvn))
		       {
		         
				 MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)imeisvn.imei, MN_MAX_IMEI_LENGTH<<1, (char*)imei_str);
				 
				 imei_len = (uint16)strlen((char *)imei_str) - 1;
				 MMIAPICOM_StrcatFromStrToUCS2(content_text, &uint16_str_len, (uint8 *)&imei_str[1], imei_len);
				 MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
		       }
		   }
		#endif

		//MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
		
		//temp_ptr = (char*)"Software Version";
		//temp_len = SCI_STRLEN((char*)temp_ptr);
		//MMIAPICOM_StrcatFromStrToUCS2(content_text, &uint16_str_len, (uint8*)temp_ptr, temp_len);
		//MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行

		
		#ifndef WIN32
		//temp_ptr = VERSION_GetInfo(PROJECT_VERSION);
		#endif
		//temp_len = SCI_STRLEN((char*)temp_ptr);
		//MMIAPICOM_StrcatFromStrToUCS2(content_text, &uint16_str_len, (uint8*)temp_ptr, temp_len);
		//MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
		
		// SwVersionInfo(content_text);
		//yangyu add
		MMI_GetLabelTextByLang(TXT_WATCH_SOFTWARE_VERSION, &title_str);
        MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &title_str);
        MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
        //temp_ptr = ZDT_GetVersionInfo();
        temp_len = SCI_STRLEN((char*)temp_ptr);
        MMIAPICOM_StrcatFromStrToUCS2(content_text, &uint16_str_len, (uint8*)temp_ptr, temp_len);
		MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
		//yangyu end

		
        MMI_GetLabelTextByLang(TXT_WATCH_HARDWARE_VERSION, &title_str);
        MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &title_str);
        MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
        MMIAPICOM_StrcatFromStrToUCS2(content_text, &uint16_str_len, WATCH_HARDWARE_VERSION, strlen(WATCH_HARDWARE_VERSION));
        //MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &cr_s);//换行
	
		content_str.wstr_ptr = content_text;
		content_str.wstr_len = MMIAPICOM_Wstrlen(content_text);

        #if 0
		// display
		GUISTR_DrawTextToLCDInRect( 
		    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		    (const GUI_RECT_T      *)&rect,       //the fixed display area
		    (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
		    (const MMI_STRING_T    *)&content_str,
		    &text_style,
		    state,
		    GUISTR_TEXT_DIR_AUTO
		    );
                //MMIPUB_OpenAlertWarningWin(text_comfirm_poweroff_selection);
        #endif
		
        GUITEXT_SetString(MMIZDT_WATCHINFO_TEXT_ID, content_str.wstr_ptr,content_str.wstr_len , FALSE);
		#endif
		#endif
		
	    break;
		
	case MSG_CTL_MIDSK:
	case MSG_KEYDOWN_OK:
	case MSG_CTL_OK:
	case MSG_APP_WEB:
#if defined( TOUCH_PANEL_SUPPORT)||defined(ZDT_TOUCHPANEL_TYPE_MULTITP) //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	    //MMK_CloseWin(win_id);
	    break;

#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)	
		case MSG_TP_PRESS_UP:
		{
		    GUI_POINT_T   point = {0};
		    point.x = MMK_GET_TP_X(param);
		    point.y = MMK_GET_TP_Y(param);
	
		    if((point.x > 0&& point.x <42)&&(point.y > 0 &&  point.y < MMI_SPECIAL_TITLE_HEIGHT))
		    {
		        MMK_CloseWin(win_id);
		        return;	 
		    }
	
		}
		break;
#endif

		 
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
	    MMK_CloseWin(win_id);
	    break;
    case MSG_KEYDOWN_RED:
            break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;    
	default:
	    recode = MMI_RESULT_FALSE;
	    break;
    }
	return recode;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL void CreatTimeOutListWin(MMI_TEXT_ID_T text_id)
{
    uint32  i = 0;
    GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
    //GUILIST_SetMaxItem(MMISET_DISPLAY_CTRL_ID, 3, FALSE);
    item_t.item_style    = GUIITEM_STYLE_1STR;//GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_t.item_data_ptr = &item_data;
        //item_data.item_content[0].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        //item_data.item_content[0].item_data.image_id = IMAGE_NULL;

        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = text_id;
                
        item_data.softkey_id[0] = TXT_NULL;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;
        
        GUILIST_AppendItem( MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID, &item_t );      
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetDisplayBlacklightTimeOutWindow(
	                                    MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    uint32 loop = 0;
    MMI_STRING_T backlight_name = {0};
    MMI_STRING_T string_to = {0};
    MMISET_BACKLIGHT_SET_T backlight = {0};
    uint16 index = 0;
    uint16 cur_select = 0;
    GUI_RECT_T list_rect = {0, 0, MMI_MAINSCREEN_WIDTH - 1 , MMI_MAINSCREEN_HEIGHT - 1}; //  //xiongkai  背光时间长度调节界面，下方显示不良。
#if 1	//xiongkai ADD_TITLE_IN_ALL_SETTING_MENU	
    GUI_RECT_T          content_rect={0,MMI_SPECIAL_TITLE_HEIGHT,240,240}; 
    GUI_RECT_T          title_rect={0,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 	
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_RECT_T          rect         = MMITHEME_GetFullScreenRectEx(win_id);	
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};	
    GUI_RECT_T          text_rect={42,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 	
#endif	
    uint32 text_id[]=
    {
        TXT_SET_TIME_15S,
        STXT_BACKLIGHT_TIME_T20,
        STXT_BACKLIGHT_TIME_T40,
        STXT_BACKLIGHT_TIME_T60,
        //STXT_BACKLIGHT_TIME_T120 //yangyu delete
#ifdef HW_LCD_TEST
        ,TXT_SET_TIME_OPEN_FOREVER
#endif
    };
    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:
		
// 	    MMI_GetLabelTextByLang(STR_BACKLIGHT_TIMEOUT, &string_to );		
// 	    if(MMIAPICOM_StatusAreaSetViewHeaderinfo(string_to.wstr_ptr, string_to.wstr_len))
// 	    {
// 	        GUIWIN_UpdateStb();
// 	    }
	    GUILIST_SetMaxItem(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID, 6, FALSE);
        //CTRLLIST_SetTextFont(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID, SONG_FONT_28, MMI_WHITE_COLOR);//yangyu delete

	    for(loop=0;loop<ARR_SIZE(text_id);loop++)
	    {
	        CreatTimeOutListWin(text_id[loop]);
	    }
	    MMIAPISET_GetBackLightInfo(&backlight);
	    switch (backlight.backlight_type)
	    {
	    case MMISET_BACKLIGHT_OPEN:
	        switch (backlight.backlight_time)
	        {
	        case MMISET_TIME_15:
	            index = 0;
	            break;

	        case MMISET_TIME_20:
	            index = 1;
	            break;

	        case MMISET_TIME_40:
	            index = 2;
	            break;

	        case MMISET_TIME_60:
	            index = 3;
	            break;

	        case MMISET_TIME_120:
	            index = 4;
	            break;

	        default:
	            //SCI_TRACE_LOW:"MMISET_AppendBacklightListItem:backlight time is %d error!"
	            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1486_112_2_18_2_51_1_173,(uint8*)"d",backlight.backlight_time);
	            break;
	        }
	        break;

	    case MMISET_BACKLIGHT_OPEN_FOREVER:
	        index = 0;
	        break;

	    case MMISET_BACKLIGHT_CLOSE:
	        //SCI_TRACE_LOW("MMISET_AppendBacklightListItem:backlight type is close!");
	        break;

	    default:
	        //SCI_TRACE_LOW:"MMISET_AppendBacklightListItem:backlight type is %d error!"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1500_112_2_18_2_51_1_174,(uint8*)"d",backlight.backlight_type);
	        break;
	    }
		
	    #if defined(ADD_TITLE_IN_ALL_SETTING_MENU)
	          GUILIST_SetRect(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID,&content_rect);
				
	          //draw TITLE bg 	
	          GUIRES_DisplayImg(PNULL,
	                      &rect,
	                      PNULL,
	                      win_id,
	                      IMAGE_THEME_BLACK_BG_SPEC, 
	                      &lcd_dev_info); 
						
	          ///////draw title
	          text_style.align = ALIGN_LVMIDDLE;
	          text_style.font = SONG_FONT_28;
	          text_style.font_color = MMI_WHITE_COLOR;
	          text_style.char_space = 0;
							
	          MMI_GetLabelTextByLang(zdt_backlight_timeout, &string);
	          GUISTR_DrawTextToLCDInRect( 
	                      (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
	                      (const GUI_RECT_T	   *)&text_rect,	   //the fixed display area
	                      (const GUI_RECT_T	   *)&text_rect,	   //用户要剪切的实际区域
	                      (const MMI_STRING_T    *)&string,
	                      &text_style,
	                      state,
	                      GUISTR_TEXT_DIR_AUTO
	                      );
	    #else	
	          GUILIST_SetRect(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID, &list_rect);	  //xiongkai  背光时间长度调节界面，下方显示不良。
	    #endif

        GUILIST_SetCurItemIndex(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID, index);
        GUILIST_SetSelectedItem(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID, index, TRUE);
        GUILIST_SetListState(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID, GUILIST_STATE_SPLIT_LINE, FALSE); //需要下划线可以打开
        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID);
	break;

#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)	
	case MSG_FULL_PAINT:
	          //draw TITLE bg 	
	          GUIRES_DisplayImg(PNULL,
	                      &rect,
	                      PNULL,
	                      win_id,
	                      IMAGE_THEME_BLACK_BG_SPEC, 
	                      &lcd_dev_info); 
						
	          ///////draw title
	          text_style.align = ALIGN_LVMIDDLE;
	          text_style.font = SONG_FONT_28;
	          text_style.font_color = MMI_WHITE_COLOR;
	          text_style.char_space = 0;
							
	          MMI_GetLabelTextByLang(zdt_backlight_timeout, &string);
	          GUISTR_DrawTextToLCDInRect( 
	                      (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
	                      (const GUI_RECT_T	   *)&text_rect,	   //the fixed display area
	                      (const GUI_RECT_T	   *)&text_rect,	   //用户要剪切的实际区域
	                      (const MMI_STRING_T    *)&string,
	                      &text_style,
	                      state,
	                      GUISTR_TEXT_DIR_AUTO
	                      );
	break;		  
#endif			  

#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)	
		case MSG_TP_PRESS_UP:
			{
				GUI_POINT_T   point = {0};
				point.x = MMK_GET_TP_X(param);
				point.y = MMK_GET_TP_Y(param);
	
				if((point.x > 0&& point.x <42)&&(point.y > 0 &&  point.y < MMI_SPECIAL_TITLE_HEIGHT))
				{
				    MMK_CloseWin(win_id);
				    return;	 
				}
	
			}
			break;
#endif

	
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
#if defined( TOUCH_PANEL_SUPPORT)||defined(ZDT_TOUCHPANEL_TYPE_MULTITP) //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	    #if defined (MMISET_BACKLIGHT_TIME_SUPPORT)
	    MMISET_SetBackLight(MMISET_DISPLAY_BACKLIGHT_TIMEOUT_CTRL_ID);
	    MMIDEFAULT_TurnOnLCDBackLight();
	    #endif
	   // MMK_CloseWin(win_id);
	    break;
	
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_KEYDOWN_RED:
            break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;    
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
	return recode;
}

#if defined MMISET_ANIM_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : Set Anim effect menu name 
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetAnimLabelParam(void)
{
    MMI_STRING_T anim_displayname = {0};
    
    GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_SET_ANIMATION, &anim_displayname);
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_LABEL_CTRL_ID, &anim_displayname, FALSE);
}
#endif
#if defined MMISET_WALLPAPER_SUPPORT
/*****************************************************************************/
//  Description : Set Wallpaper menu name 
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetWallPaperParam(void)
{
    MMI_STRING_T title = {0};
    MMI_STRING_T wallpaper_label_name = {0};
    MMISET_WALLPAPER_SET_T wallpaper_info = {0};
    uint32 text_id[MMISET_WALLPAPER_LIST_MAX]=
    {
        TXT_SET_WALLPAPER_1,
        TXT_SET_WALLPAPER_2, 
#ifndef MMI_SETTING_CUTDOWN_WALLPAPER 
        TXT_SET_WALLPAPER_3,
        TXT_SET_WALLPAPER_4,
#endif
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT            
#ifdef LIVE_WALLPAPER_SUPPORT_LINEAR
        TXT_SET_WALLPAPER_LINEAR,         //线性光幕动态墙纸
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_POP
        TXT_SET_WALLPAPER_POP,         //POP动态墙纸
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_BEE
        TXT_SET_WALLPAPER_BEE,         //BEE动态墙纸
#endif

#ifdef LIVE_WALLPAPER_SUPPORT_GALAXY
        TXT_SET_WALLPAPER_GALAXY,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SAKURA
        TXT_SET_WALLPAPER_SAKURA,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SEAWORLD
        TXT_SET_WALLPAPER_SEAWORLD,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_LIGHTGRID
        TXT_SET_WALLPAPER_LIGHTGRID,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_RIPPLE
        TXT_SET_WALLPAPER_RIPPLE,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION
        TXT_SET_WALLPAPER_COLLISION,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_LITTLECAT
        TXT_SET_WALLPAPER_LITTLECAT,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SWEETTREE
        TXT_SET_WALLPAPER_SWEETTREE,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION_BALLOON
        TXT_SET_WALLPAPER_COLLISION_BALLOON,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION_MENU
        TXT_SET_WALLPAPER_COLLISION_MENU,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_WINDMILL
        TXT_SET_WALLPAPER_WINDMILL,                           //吹风车
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_RAINBOWCITY
        TXT_SET_WALLPAPER_RAINBOWCITY,                   //彩虹城市
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_PAPERTOWN
        TXT_SET_WALLPAPER_PAPERTOWN,                     //卡通小镇
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_AQUARIUM
        TXT_SET_WALLPAPER_AQUARIUM,                     //水族馆
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_GIRLSWING
	TXT_SET_WALLPAPER_GIRLSWING,                //童话世界
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SPRING
        TXT_SET_WALLPAPER_SPRING,         //
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_NEST
        TXT_SET_WALLPAPER_NEST,         //
#endif

#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT            
        TXT_SET_WALLPAPER_VIDEO,
#endif 
        TXT_SET_WALLPAPER_MORE
    };

    GUIFORM_SetStyle(MMISET_DISPLAY_WALLPAPER_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_COMMON_SET_WALLPAPER, &title);
    GUILABEL_SetText(MMISET_DISPLAY_WALLPAPER_LABEL_CTRL_ID, &title, FALSE);
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
    if (MMISET_WALLPAPER_FIXED_PIC == wallpaper_info.wallpaper_type)
    {
        if (MMISET_WALLPAPER_DEFAULT_ID <=wallpaper_info.wallpaper_id && wallpaper_info.wallpaper_id <= MMISET_WALLPAPER_FIXED_PIC_MAX_NUM)/*lint !e685 !e568*/
        {
            MMI_TEXT_ID_T txt_id = text_id[wallpaper_info.wallpaper_id];
            
#ifdef QBTHEME_SUPPORT
            if (MMISET_WALLPAPER_DEFAULT_ID != wallpaper_info.wallpaper_id
                && STYLE_IS_SUPPORT_QBTHEME)
            {
                txt_id = TXT_QB_THEME_WALLPAPER;
            }
#endif
            MMI_GetLabelTextByLang(txt_id, &wallpaper_label_name);
        }
    }
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT        	
    else if (MMISET_WALLPAPER_LIVE == wallpaper_info.wallpaper_type)
    {
        #ifdef SLIDESHOW_WALLPAPER_SUPPORT
        if(MMI_LIVE_WALLPAPER_TYPE_SLIDESHOW == wallpaper_info.wallpaper_id)
        {
            MMI_GetLabelTextByLang(TXT_SET_WALLPAPER_SLIDESHOW, &wallpaper_label_name);
        }
        else
		#endif
        {
            MMI_GetLabelTextByLang(text_id[MMISET_WALLPAPER_FIXED_PIC_MAX_NUM + wallpaper_info.wallpaper_id], &wallpaper_label_name);
        }
        
    }
#endif  
#ifdef MMI_VIDEOWALLPAPER_SUPPORT    
    else if (MMISET_WALLPAPER_VIDEO == wallpaper_info.wallpaper_type)
    {
        MMI_GetLabelTextByLang(TXT_SET_WALLPAPER_VIDEO, &wallpaper_label_name);
    }
#endif 
    else if(MMISET_WALLPAPER_DOWNLOAD == wallpaper_info.wallpaper_type)
    {
        
//        MMIAPIFMM_GetWallPaperInfo(wallpaper_file_ptr);

//        wallpaper_label_name.wstr_ptr = wallpaper_file_ptr->full_path_name;
//        wallpaper_label_name.wstr_len = wallpaper_file_ptr->full_path_len;
		MMI_GetLabelTextByLang(TXT_MORE_PICTURE, &wallpaper_label_name);
    }
    
    GUILABEL_SetText(MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID, &wallpaper_label_name, FALSE);
}
#endif

/*****************************************************************************/
//  Description : 供外部使用的设置墙纸label值的接口
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC  void MMIAPISET_SetWallPaperName(void)
{
#if defined MMISET_WALLPAPER_SUPPORT
    SetWallPaperParam();
#endif
}  
#if defined MMISET_THEME_SUPPORT
/*****************************************************************************/
//  Description : set theme content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetThemeParam(void)
{
    MMI_STRING_T theme_title_name = {0};
    MMI_STRING_T itemStr={0};
    uint16 index =0;
    uint32 text_id[]=
    {
        TXT_SET_THEME_1,
        TXT_SET_THEME_2
    };

    GUIFORM_SetStyle(MMISET_DISPLAY_THEME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    
    MMI_GetLabelTextByLang(TXT_SET_THEME, &theme_title_name);
    GUILABEL_SetText(MMISET_DISPLAY_THEME_LABEL_CTRL_ID, &theme_title_name, FALSE);
    
    index = MMIAPISET_GetCurrentStaticThemeID();
    
    MMI_GetLabelTextByLang(text_id[index],&itemStr);
    GUILABEL_SetText(MMISET_DISPLAY_THEME_LABEL_EDIT_CTRL_ID, &itemStr, FALSE);
}
#endif
#if defined MMISET_IDLE_STYLE_SUPPORT
/*****************************************************************************/
//  Description : set idle style content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetIdleStyleParam(void)
{
    MMI_STRING_T idle_label_name = {0};
    MMISET_IDLE_STYLE_E     idle_style  =   MMISET_IDLE_STYLE_COMMON;
    uint16 index = 0;
    uint32 loop = 0;
    uint32 text_id[MMISET_IDLE_STYLE_MAX]=
    {
#ifndef MMI_PDA_SUPPORT    
        TXT_SET_IDLE_COMMON,
        TXT_SET_IDLE_SMART,
#ifndef BAR_PHONE_WIDGET 
#ifdef MMIWIDGET_SUPPORT    
        TXT_SET_IDLE_WIDGET,
#endif
#ifdef MMI_GRID_IDLE_SUPPORT
        TXT_SET_IDLE_WIDGET,
#endif
#endif
#else    
    #ifdef MMI_GRID_IDLE_SUPPORT
        TXT_SET_IDLE_WIDGET,
    #endif

        TXT_SET_MAINMENU_SLIDE,
#endif

    };

    GUIFORM_SetStyle(MMISET_DISPLAY_IDLE_STYLE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
#if defined (MMI_GUI_STYLE_MINISCREEN)  
    MMI_GetLabelTextByLang(TXT_IDLE_STYLE_SWITH, &idle_label_name);    
#else
    MMI_GetLabelTextByLang(TXT_IDLE_SWITH, &idle_label_name);
#endif
    GUILABEL_SetText(MMISET_DISPLAY_IDLE_STYLE_LABEL_CTRL_ID, &idle_label_name, FALSE);
    
    for(loop=0;loop<MMISET_IDLE_STYLE_MAX;loop++)
    {
        GUISETLIST_AddItemById(MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID, text_id[loop]);
    }
    idle_style = MMIAPISET_GetIdleStyle();
    switch (idle_style)
    {
#ifndef MMI_PDA_SUPPORT    
    case MMISET_IDLE_STYLE_COMMON:
        index = 0;
        break;
        
    case MMISET_IDLE_STYLE_SMART:
        index = 1;
        break; 
        
    #ifdef MMIWIDGET_SUPPORT
    case MMISET_IDLE_STYLE_WIDGET:
        index = 2;
        break;
    #endif
        
    #ifdef MMI_GRID_IDLE_SUPPORT
    case MMISET_IDLE_STYLE_GRID:
        #ifdef BAR_PHONE_WIDGET 
        {
            MMISET_IDLE_STYLE_E pre_idle_style = MMIAPISET_GetPreIdleStyle();
            if(pre_idle_style == MMISET_IDLE_STYLE_COMMON)
            {
                index = 0;
            }
            else
            {
                index = 1;
            }
            
        }
        #else
        index = 2;
        #endif        
        break;
    #endif 
#else
    #ifdef MMI_GRID_IDLE_SUPPORT
    case MMISET_IDLE_STYLE_GRID:
        index = 0;
        break;
    #endif
#if defined MMI_ISTYLE_SUPPORT 
    case MMISET_IDLE_ISTYLE:
        index = 1;
        break;
#endif		
#endif
        
    default:
        //SCI_TRACE_LOW:"HandleIdleMsg:idle_style %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1366_112_2_18_2_51_1_172,(uint8*)"d",idle_style);
        break;
    }
    GUISETLIST_SetCtrlState(MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID, index );
#if defined (MMI_DISPLAYSET_MINI_SUPPORT)   
    GUISETLIST_SetTitleTextId(MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID,TXT_IDLE_STYLE_SWITH);   
#else
    GUISETLIST_SetTitleTextId(MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID,TXT_IDLE_SWITH);    
#endif
}
#endif
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
/*****************************************************************************/
//  Description : set idle style content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetIdleDisplaySettingParam(void)
{
    MMI_STRING_T idle_display_setting_name = {0};

    //istyle下隐藏
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
    if ( MMITHEME_IsIstyle() )
    {
        GUIFORM_SetChildDisplay(MMISET_DISPLAY_FORM_CTRL_ID, MMISET_DISPLAY_IDLE_DISPLAY_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        return;
    }
#endif
    
    GUIFORM_SetStyle(MMISET_DISPLAY_IDLE_DISPLAY_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_SET_IDLE_DISPLAY, &idle_display_setting_name);
    GUILABEL_SetText(MMISET_DISPLAY_IDLE_DISPLAY_LABEL_CTRL_ID, &idle_display_setting_name, FALSE);
}
#endif
#endif
/*****************************************************************************/
//  Description : set contrast lable name
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetContrastParam(void)
{
    MMI_STRING_T contrast_name = {0};
#ifndef MMI_GUI_STYLE_MINISCREEN //pclint
    uint16    bar_width = 0;
#endif
    uint16    bar_height = 0;
#if 0
#ifdef MAINLCD_SIZE_240X320
    GUIFORM_CHILD_HEIGHT_T form_height = {0};

    form_height.add_data = 45;
    form_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    GUIFORM_SetChildHeight(MMISET_DISPLAY_FORM_CTRL_ID,MMISET_DISPLAY_CONTRAST_FORM_CTRL_ID,&form_height);
#endif
#endif
    GUIFORM_SetStyle(MMISET_DISPLAY_CONTRAST_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_SET_CONTRAST, &contrast_name);
    GUILABEL_SetText(MMISET_DISPLAY_CONTRAST_CTRL_ID, &contrast_name, FALSE); 
#ifndef MMI_GUI_STYLE_MINISCREEN
    GUIRES_GetImgWidthHeight(&bar_width,
        &bar_height,
        IMAGE_SET_CONTRAST_BAR_FG,
        MMISET_SET_DISPLAY_WIN_ID);
    GUIOWNDRAW_SetHeight(MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID, bar_height + 20);
#else
    bar_height = MMISET_DISPLAY_CONTRAST_BAR_HIGHT;
    GUIOWNDRAW_SetHeight(MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID, (bar_height + 10));
#endif

}
#if defined MMISET_BACKLIGHT_TIME_SUPPORT
/*****************************************************************************/
//  Description : set backlight param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetBacklightParam(void)
{
    MMI_STRING_T backlight_name = {0};
    MMISET_BACKLIGHT_SET_T backlight = {0};
    uint16 index = 0;
    GUI_BG_T  bg_id = {0};
    uint32 loop = 0;
    uint32 text_id[]=
    {
        TXT_SET_TIME_OPEN_FOREVER,
        TXT_SET_TIME_30S,
        TXT_SET_TIME_20S,
        TXT_SET_TIME_15S,
        TXT_COMM_10SECONDS,
        TXT_SET_TIME_5S
    };
    GUIFORM_SetStyle(MMISET_DISPLAY_BACKLIGH_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

#ifdef MMI_PDA_SUPPORT
    GUISETLIST_SetFontAlign(MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif

    bg_id.img_id = IMAGE_COMMON_BG;
    MMI_GetLabelTextByLang(TXT_SET_BACK_LIGHT, &backlight_name);
    GUILABEL_SetText(MMISET_DISPLAY_BACKLIGHT_CTRL_ID, &backlight_name, FALSE);
    
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID, text_id[loop]);
    }

    MMIAPISET_GetBackLightInfo(&backlight);
    switch (backlight.backlight_type)
    {
    case MMISET_BACKLIGHT_OPEN:
        switch (backlight.backlight_time)
        {
        case MMISET_TIME_30:
            index = 1;
            break;

        case MMISET_TIME_20:
            index = 2;
            break;

        case MMISET_TIME_15:
            index = 3;
            break;

        case MMISET_TIME_10:
            index = 4;
            break;

        case MMISET_TIME_5:
            index = 5;
            break;

        default:
            //SCI_TRACE_LOW:"MMISET_AppendBacklightListItem:backlight time is %d error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1486_112_2_18_2_51_1_173,(uint8*)"d",backlight.backlight_time);
            break;
        }
        break;

    case MMISET_BACKLIGHT_OPEN_FOREVER:
        index = 0;
        break;

    case MMISET_BACKLIGHT_CLOSE:
        //SCI_TRACE_LOW("MMISET_AppendBacklightListItem:backlight type is close!");
        break;

    default:
        //SCI_TRACE_LOW:"MMISET_AppendBacklightListItem:backlight type is %d error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1500_112_2_18_2_51_1_174,(uint8*)"d",backlight.backlight_type);
        break;
    }
    GUISETLIST_SetCtrlState(MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);  
    GUISETLIST_SetBackGround(MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID, &bg_id);
    GUISETLIST_SetCurIndex(MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID, index );
    GUISETLIST_SetTitleTextId(MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID,TXT_SET_BACK_LIGHT); 
}
#endif

/*****************************************************************************/
//  Description : set font content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
#if defined MMISET_FONT_SUPPORT
LOCAL void SetFontParam(void)
{
    MMI_STRING_T title_name = {0};
    MMI_STRING_T font_name ={0};
    wchar font[MMIFMM_FILE_FULL_PATH_MAX_LEN + 1] = {0};

    GUIFORM_SetStyle(MMISET_DISPLAY_FONT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_SET_FONT, &title_name);
    GUILABEL_SetText(MMISET_DISPLAY_FONT_SETTING_CTRL_ID, &title_name, FALSE);
    GetFontInfo(font);
    if( 0 == *font)
    {
        MMI_GetLabelTextByLang(TXT_SET_DEFAULT_FONT, &font_name);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_SET_FONT_MORE, &font_name);
    }
    GUILABEL_SetText(MMISET_DISPLAY_FONT_SETTING_LABEL_EDIT_CTRL_ID, &font_name, FALSE);
}
#endif


/*****************************************************************************/
//  Description : set ScreenSaver content
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
#if defined MMISET_SCREENSAVER_SUPPORT 
PUBLIC void SetScreenSaverParam(void)
{
    MMI_STRING_T screensaver_title= {0};
    MMI_STRING_T screensaver_name= {0};   
    MMISET_SCREENSAVER_INFO_T  ssaver_info = {0};
    uint32 text_id[MMISET_SCREENSAVER_MAX_NUM]=
    {
        TXT_CLOSE,
        TXT_SET_SCREENSAVER_SEA,
        TXT_SET_SCREENSAVER_STAR,
        TXT_SET_SCREENSAVER_SNAKE,
        TXT_SET_SCREENSAVER_SEED,
        TXT_SET_SCREENSAVER_PICTURE,
#ifdef VIDEO_PLAYER_SUPPORT 
        TXT_SET_SCREENSAVER_ANIMATION
#endif
    };

    GUIFORM_SetStyle(MMISET_DISPLAY_SCREEN_SAVERS_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    
    MMI_GetLabelTextByLang(TXT_SET_SCREENSAVER, &screensaver_title);
    GUILABEL_SetText(MMISET_DISPLAY_SCREEN_SAVERS_LABEL_CTRL_ID, &screensaver_title, FALSE);
    MMIAPISET_GetScreensaverInfo(&ssaver_info);
    if(ssaver_info.ssaver_type <=MMISET_SCREENSAVER_SEED)
    {
        MMI_GetLabelTextByLang(text_id[ssaver_info.ssaver_type], &screensaver_name);
    }
    else
    {

        if(ssaver_info.ssaver_type == MMISET_SCREENSAVER_MORE_PICTURE)
		{
            if (0 == ssaver_info.more_pic_saver_info.name_wstr_len)
            {
                MMI_GetLabelTextByLang(TXT_SET_SCREENSAVER_PICTURE, &screensaver_name);
            }
            else
            {
                screensaver_name.wstr_ptr = ssaver_info.more_pic_saver_info.name_wstr;
                screensaver_name.wstr_len = ssaver_info.more_pic_saver_info.name_wstr_len;
            }
		}
#ifdef VIDEO_PLAYER_SUPPORT
        else if(ssaver_info.ssaver_type == MMISET_SCREENSAVER_MORE_ANIMATION)
		{
            if (0 == ssaver_info.more_ani_saver_info.name_wstr_len)
            {
                MMI_GetLabelTextByLang(TXT_SET_SCREENSAVER_ANIMATION, &screensaver_name);
            }
            else
            {
                screensaver_name.wstr_ptr = ssaver_info.more_ani_saver_info.name_wstr;
                screensaver_name.wstr_len = ssaver_info.more_ani_saver_info.name_wstr_len;
            }
        }
#endif
    }
    GUILABEL_SetText(MMISET_DISPLAY_SCREEN_SAVERS_LABEL_EDIT_CTRL_ID, &screensaver_name, FALSE);       
}
#endif

#if defined MMISET_KEYBACKLIGHT_SUPPORT
/*****************************************************************************/
//  Description : set Keyboard light param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetKeyboardLightParam(void)
{
    MMI_STRING_T keyboardlight_name = {0};
    MMISET_KEYBOARD_LIGHT_SET_T   keyboard_info = {0};
    uint16 index = 0;
    uint32 loop = 0;
    uint32 text_id[]=
    {
        TXT_SET_TIME_20S,
        TXT_SET_TIME_10S,
        TXT_SET_TIME_5S,
        TXT_NIGHT_MODE,
        TXT_CLOSE
    };
    GUIFORM_SetStyle(MMISET_DISPLAY_KEYPAD_BACKLIGH_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif

    MMI_GetLabelTextByLang(TXT_SET_KEY_BOARD_LIGHT, &keyboardlight_name);
    GUILABEL_SetText(MMISET_DISPLAY_KEYPAD_BACKLIGHT_CTRL_ID, &keyboardlight_name, FALSE);

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID, text_id[loop]);
    }
    
    MMIAPISET_GetKeyBoardLightTime(&keyboard_info);
    //get current keyboard light
    if (MMISET_KEYBOARD_LIGHT_OPEN == keyboard_info.keyboard_type
        && MMISET_KEYBOARD_LIGHT_TIME_20 == keyboard_info.keyboard_light_time)
    {
        index = 0;
    }
    else if (MMISET_KEYBOARD_LIGHT_OPEN == keyboard_info.keyboard_type
        && MMISET_KEYBOARD_LIGHT_TIME_10 == keyboard_info.keyboard_light_time)
    {
        index = 1;
    }
    else if (MMISET_KEYBOARD_LIGHT_OPEN == keyboard_info.keyboard_type
        && MMISET_KEYBOARD_LIGHT_TIME_5 == keyboard_info.keyboard_light_time)
    {
        index = 2;
    }
    else if (MMISET_KEYBOARD_LIGHT_AUTO == keyboard_info.keyboard_type)
    {
        index = 3;      
    }
    else
    {
        index = 4;
    }
    GUISETLIST_SetCtrlState(MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID, index );
    GUISETLIST_SetTitleTextId(MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID,TXT_SET_KEY_BOARD_LIGHT); 
}
#endif
#ifdef MMISET_ANIM_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : to handle display setting animation window message
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetDisplayAnimationWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    )
{    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIFORM_SetType(MMISET_DISPLAY_ANIM_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif
        GUIFORM_SetMargin(MMISET_DISPLAY_ANIM_FORM_CTRL_ID, 0);
//#ifndef MMI_PDA_SUPPORT
//        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_ANIM_POWER_ON_LABEL_EDIT_CTRL_ID); 
//#else
//        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_ANIM_FORM_CTRL_ID); 
//#endif
#ifdef SET_3D_STYLE_SUPPORT 
        Set3D_EffectParam();
#endif
#if !defined FLASH_SUPPORT && !defined MMI_PDA_SUPPORT
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
		SetMainmenu_EffectParam();
#endif
#endif
#if !defined MMI_PDA_SUPPORT && defined(EFFECT_3D_LIST_ROLL_SUPPORT)
        SetListEffectParam();
#endif
#ifdef EFFECT_LCD_ROTATE_SUPPORT
        SetGsensor_EffectParam();
#endif
        SetPowerOnEffectParam();
        SetPowerOffEffectParam();
#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER) 
        SetRenderEffectParam();
#endif
        MMK_SetAtvCtrl(win_id, MMISET_DISPLAY_ANIM_FORM_CTRL_ID); 
		GUIFORM_SetCircularHandleUpDown(MMISET_DISPLAY_ANIM_FORM_CTRL_ID, TRUE);
		GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_FINISH, FALSE);
        break;

    case MSG_GET_FOCUS:
        SaveAllAnimSet();
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        if (MSG_CTL_MIDSK == msg_id)/*lint !e774 */
        {
            ctrl_id= MMK_GetActiveCtrlId(win_id);
        }
        else
        {
            ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
        }
        switch (ctrl_id)
        {
        case MMISET_DISPLAY_ANIM_POWER_ON_LABEL_EDIT_CTRL_ID:
            MMK_CreatePubListWin((uint32*)MMISET_SET_POWER_ON_ANIM_WIN_TAB,PNULL);           
            break;
            
        case MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_EDIT_CTRL_ID:
            MMK_CreatePubListWin((uint32*)MMISET_SET_POWER_OFF_ANIM_WIN_TAB,PNULL);
            break;

#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER)
        case MMISET_DISPLAY_ANIM_RENDER_LABEL_EDIT_CTRL_ID:
            MMK_CreatePubListWin((uint32*)MMISET_SET_RENDER_EFFECT_WIN_TAB,PNULL);
            break;
#endif
        default:
            //SCI_TRACE_LOW:"HandleSetDisplayAnimationWindow ctrl_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_1744_112_2_18_2_51_2_175,(uint8*)"d",ctrl_id);
            break;
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        MMK_SendMsg(MMISET_SET_DISPLAY_ICON_ANIMATION_WIN_ID, MSG_APP_WEB, PNULL);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        SaveAllAnimSet();
        MMK_CloseWin(win_id);
        break;
        
    case MSG_KEYDOWN_RED:
            break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        SaveAllAnimSet();
        recode = MMI_RESULT_FALSE;
        break;
            
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER)
/*****************************************************************************/
//  Description : to handle display setting go animation window message
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetLightShadowAnimationWindow(
                                                    MMI_WIN_ID_T    win_id, 
                                                    MMI_MESSAGE_ID_E   msg_id, 
                                                    DPARAM             param
                                                    )
{
    MMI_RESULT_E                      recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   check_list_id = MMISET_DISPLAY_ANIM_RENDER_CHECKLIST_CTRL_ID;
    uint32 i = 0;
    
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMISET_EFFECT_RENDER_SETTING* render_setting_ptr = PNULL;
            //init list string
            GUILIST_SetMaxItem(check_list_id, MMISET_EFFECT_RENDER_SETTING_MAX, FALSE );//max item 4
            GUILIST_SetMaxSelectedItem(check_list_id, MMISET_EFFECT_RENDER_SETTING_MAX);

            /*第一个ITEM*/
            MMIAPISET_AppendListItemByTextId(TXT_SET_GO_MENU_ANIMATION, STXT_SELECT, check_list_id, GUIITEM_STYLE_ONE_LINE_CHECK);
            render_setting_ptr = MMISET_GetLightShadowRenderSetting();

            for(i = 0;i < MMISET_EFFECT_RENDER_SETTING_MAX;i++) /*lint !e685 !e586 !e681 !e568*/
            {
                GUILIST_SetSelectedItem(check_list_id, (uint16)i,render_setting_ptr->render_array[i]);
            }
            MMK_SetAtvCtrl(win_id, check_list_id);
        }
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_CLOSE_WINDOW:
        {
            uint16 select_item[MMISET_EFFECT_RENDER_SETTING_MAX + 1] = {0};
            MMISET_EFFECT_RENDER_SETTING render_setting = {0};
            if(0 < GUILIST_GetSelectedItemIndex(check_list_id, select_item, MMISET_EFFECT_RENDER_SETTING_MAX))
            {
                for(i = 0;i < MMISET_EFFECT_RENDER_SETTING_MAX;i++) /*lint !e685 !e586 !e681 !e568*/
                {
                    if(i == select_item[i])
                    {
                        render_setting.render_array[i] = TRUE;
                    }
                    else
                    {
                        render_setting.render_array[i] = FALSE;
                    }
                }
            }
            MMISET_SetLightShadowRenderSetting(&render_setting);
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;  
}
#endif     
#ifdef SET_3D_STYLE_SUPPORT 
/*****************************************************************************/
//  Description : set 3d interface param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void Set3D_EffectParam(void)
{
    MMI_STRING_T title = {0};
    uint16 index = 0;
    uint32 loop = 0;

    uint32 text_id[]=
    {
        TXT_CLOSE,
        TXT_SET_3D_STYLE_SCREEN_DEFAULT,
        TXT_AUTO, 
#ifdef EFFECT_WIN_SLIDE_SUPPORT        
        TXT_SET_3D_STYLE_SCREEN_SLIDE_MOVE,
        TXT_SET_3D_STYLE_SCREEN_SLIDE, 
#endif        
#ifdef EFFECT_ZOOM_SUPPORT        
        TXT_SET_3D_STYLE_SCREEN_ZOOM,
#endif        
#if 0//def EFFECT_WIN_SLIDE_SUPPORT
        TXT_COMM_FADE_INOUT,
#endif        
#ifdef EFFECT_3D_ROTATE_SUPPORT		        
        TXT_SET_3D_STYLE_SCREEN_ROLL
#endif        
    };

#if 0//def QBTHEME_SUPPORT
    MMISET_IDLE_STYLE_E cur_idle_style = MMIAPISET_GetIdleStyle();

    if (MMISET_IDLE_QBTHEME == cur_idle_style)
    {
        return;
    }
#endif

    
    GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_3D_EFFECT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_SET_3D_STYLE_SCREEN, &title);
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_3D_EFFECT_LABEL_CTRL_ID, &title, FALSE);
#if !defined MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID, text_id[loop]);
    }
#ifdef UI_P3D_SUPPORT   
    // 后续特效请在框架中添加
    for(loop=loop ;loop < MMISET_3D_STYLE_SCREEN_MAX;loop++)
    {
        uint32 winse_text_id = 0;
        
        winse_text_id = MMIWINSE_GetNameIdBySetingStyle(loop);
        if(0 != winse_text_id)
        {
            GUISETLIST_AddItemById(MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID, winse_text_id);
        }
    }
    
#endif
    index = MMISET_Get3DStyleScreen();    
    GUISETLIST_SetCurIndex(MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID, index);
    GUISETLIST_SetTitleTextId(MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID,TXT_SET_3D_STYLE_SCREEN);   
}
#endif
#if !defined FLASH_SUPPORT && !defined MMI_PDA_SUPPORT
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : set mainmenu effect param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetMainmenu_EffectParam(void)
{
    MMI_STRING_T title = {0};
    uint16 index = 0;
    uint32 loop = 0;
    uint32 text_id[MMISET_3D_STYLE_MAINMENU_MAX]=
    {
        TXT_COMMON_COMMON_NORMAL, 
        TXT_MMISET_3D_STYLE_MAINMENU_MAGICMOVE,
        TXT_MMISET_3D_STYLE_MAINMENU_ROTATE3D,
        TXT_MMISET_3D_STYLE_MAINMENU_SLIDEMOVE, 
        TXT_MMISET_3D_STYLE_MAINMENU_ROTATEMOVE,
        TXT_MMISET_3D_STYLE_MAINMENU_LUCKYBOARD, 
        TXT_MMISET_3D_STYLE_MAINMENU_WRAPBAND, 
        TXT_MMISET_3D_STYLE_MAINMENU_DISTORTMOVE    
    };
	//GO不支持3D动画
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
	if(MMIMAINMENU_GO_SLIDE_E == MMIAPISET_GetMainmenuStyle())
	{
		GUIFORM_SetChildDisplay(MMISET_DISPLAY_ANIM_FORM_CTRL_ID,MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
	}
	else
#endif
	{
            GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#ifndef MMI_GUI_STYLE_TYPICAL
	     GUISETLIST_SetFontAlign(MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
	     MMI_GetLabelTextByLang(TXT_SET_3D_STYLE_MAINMENU, &title);
	     GUILABEL_SetText(MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_LABEL_CTRL_ID, &title, FALSE);
		    
	     for(loop=0;loop<MMISET_3D_STYLE_MAINMENU_MAX;loop++)
		    {
		        GUISETLIST_AddItemById(MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_SETLIST_CTRL_ID, text_id[loop]);
		    }
		    
	     index = MMISET_Get3DStyleMainmenu();  
            GUISETLIST_SetCurIndex(MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_SETLIST_CTRL_ID, index);
	     GUISETLIST_SetTitleTextId(MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_SETLIST_CTRL_ID,TXT_SET_3D_STYLE_MAINMENU);   
	}
}
#endif
#endif
#if !defined MMI_PDA_SUPPORT && defined(EFFECT_3D_LIST_ROLL_SUPPORT)
/*****************************************************************************/
//  Description : set list effect param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetListEffectParam(void)
{
    MMI_STRING_T title = {0};
    BOOLEAN is_list_effect_on = 0;
    uint32 loop = 0;
    uint32 text_id[]=
    {
        TXT_CLOSE,
        TXT_COMMON_ENABLE_ON
    };

    GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_LIST_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_MMISET_3D_STYLE_LIST_ROLL,&title);
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_LIST_LABEL_CTRL_ID, &title, FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_DISPLAY_ANIM_LIST_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMISET_DISPLAY_ANIM_LIST_SETLIST_CTRL_ID, text_id[loop]);
    }
    is_list_effect_on = MMIAPISET_GetList3DStyle();
    GUISETLIST_SetCurIndex(MMISET_DISPLAY_ANIM_LIST_SETLIST_CTRL_ID, is_list_effect_on);
    GUISETLIST_SetTitleTextId(MMISET_DISPLAY_ANIM_LIST_SETLIST_CTRL_ID,TXT_MMISET_3D_STYLE_LIST_ROLL);  
}
#endif
#ifdef EFFECT_LCD_ROTATE_SUPPORT
/*****************************************************************************/
//  Description : set g-sensor effect param
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void SetGsensor_EffectParam(void)
{
#ifdef MMI_RES_ORIENT_BOTH // 说明支持横竖屏切换
    MMI_STRING_T title = {0};
    uint16 index = 0;
    uint32 loop = 0;
    uint32 text_id[MMISET_G_SENSOR_SWITCH_MAX]=
    {
        TXT_CLOSE, 
        TXT_SET_3D_STYLE_SCREEN_DEFAULT,
        TXT_COMM_FADE_INOUT 
    };
    
    GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_G_SENSOR_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_MMISET_3D_STYLE_GSENSOR_ROLL, &title);
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_G_SENSOR_LABEL_CTRL_ID, &title, FALSE);

#if !defined MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMISET_DISPLAY_ANIM_G_SENSOR_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<MMISET_G_SENSOR_SWITCH_MAX;loop++)
    {
        GUISETLIST_AddItemById(MMISET_DISPLAY_ANIM_G_SENSOR_SETLIST_CTRL_ID, text_id[loop]);
    }
    
    index = MMIAPISET_GetGsensorSwitchStyle();  
    
    if (index >= MMISET_G_SENSOR_SWITCH_MAX)
    {
        index = MMISET_G_SENSOR_SWITCH_DEFAULT;
    }

    GUISETLIST_SetCurIndex(MMISET_DISPLAY_ANIM_G_SENSOR_SETLIST_CTRL_ID, index);
    GUISETLIST_SetTitleTextId(MMISET_DISPLAY_ANIM_G_SENSOR_SETLIST_CTRL_ID,TXT_MMISET_3D_STYLE_GSENSOR_ROLL);   
#endif
}
#endif


/*****************************************************************************/
//  Description : set list effect param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetPowerOnEffectParam(void)
{
    MMI_STRING_T title = {0};

    MMI_STRING_T power_on_name ={0};
    MMISET_POWER_ON_OFF_ANIM_INFO_T     power_on_off_info  =   {0};
    MMISET_POWER_ON_OFF_ANIM_TYPE_E     index =MMISET_POWER_ON_OFF_ANIM_DEFAULT;
    
    GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_POWER_ON_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_COMMON_POWER_ON_ANIMATION, &title);
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_ON_LABEL_CTRL_ID, &title, FALSE);
    
    MMIAPISET_GetPowerOnOffAnimInfo(&power_on_off_info,TRUE);
    index = power_on_off_info.anim_type;
    if(MMISET_POWER_ON_OFF_ANIM_DEFAULT == index)
    {
        MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &power_on_name);
    }
    else if(MMISET_POWER_ON_OFF_PIC_MORE == index
        || MMISET_POWER_ON_OFF_ANIM_MORE == index)
    {
        //MMI_GetLabelTextByLang(TXT_MORE_PICTURE, &power_on_name);
        power_on_name.wstr_ptr = power_on_off_info.more_anim_info.name_wstr;
        power_on_name.wstr_len = power_on_off_info.more_anim_info.name_wstr_len;
    }
//    else
//    {
//        MMI_GetLabelTextByLang(TXT_SET_POWER_ON_OFF_ANIM_MORE, &power_on_name);
//    }
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_ON_LABEL_EDIT_CTRL_ID, &power_on_name, FALSE); //MODIFY BY MARY
}


/*****************************************************************************/
//  Description : set power off param
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetPowerOffEffectParam(void)
{
    MMI_STRING_T title = {0};
    MMI_STRING_T power_off_name ={0};
    MMISET_POWER_ON_OFF_ANIM_INFO_T     power_on_off_info  =   {0};
    MMISET_POWER_ON_OFF_ANIM_TYPE_E     index =MMISET_POWER_ON_OFF_ANIM_DEFAULT;

    GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_POWER_OFF_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang(TXT_COMMON_POWER_OFF_ANIMATION, &title);
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_CTRL_ID, &title, FALSE);

    MMIAPISET_GetPowerOnOffAnimInfo(&power_on_off_info,FALSE);
    index = power_on_off_info.anim_type;
    if(MMISET_POWER_ON_OFF_ANIM_DEFAULT == index)
    {
        MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &power_off_name);
    }
    else if(MMISET_POWER_ON_OFF_PIC_MORE == index
        || MMISET_POWER_ON_OFF_ANIM_MORE == index)
    {
        //MMI_GetLabelTextByLang(TXT_MORE_PICTURE, &power_off_name);
        power_off_name.wstr_ptr = power_on_off_info.more_anim_info.name_wstr;
        power_off_name.wstr_len = power_on_off_info.more_anim_info.name_wstr_len;
    }
//    else
//    {
//        MMI_GetLabelTextByLang(TXT_SET_POWER_ON_OFF_ANIM_MORE, &power_off_name);
//    }
    GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_EDIT_CTRL_ID, &power_off_name, FALSE);// MODIFY BY MARY

}   
#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER) 
/*****************************************************************************/
//  Description : set go effect param
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetRenderEffectParam(void)
{
    MMI_STRING_T title = {0};

	//在iphone下不显示设置光亮特效等
    if(MMISET_EFFECT_RENDER_SETTING_MAX > 0
	   && (MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle())
	   )/*lint !e506 !e774*/
    {
        GUIFORM_SetStyle(MMISET_DISPLAY_ANIM_RENDER_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

        MMI_GetLabelTextByLang(TXT_SET_LIGHT_SHADOW_RENDER_ANIMATION, &title);
        
        GUILABEL_SetText(MMISET_DISPLAY_ANIM_RENDER_LABEL_CTRL_ID, &title, FALSE);
        MMI_GetLabelTextByLang(TXT_SET_GO_MENU_ANIMATION, &title);
        GUILABEL_SetText(MMISET_DISPLAY_ANIM_RENDER_LABEL_EDIT_CTRL_ID, &title, FALSE);// 
    }
    else
    {
        //GUIFORM_SetVisible(MMISET_DISPLAY_ANIM_RENDER_FORM_CTRL_ID,FALSE,FALSE);
        GUIFORM_SetChildDisplay(MMISET_DISPLAY_ANIM_FORM_CTRL_ID, MMISET_DISPLAY_ANIM_RENDER_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    }
    
} 
#endif
#ifdef MMISET_ANIM_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : save all anim set feature
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SaveAllAnimSet(void)
{
    // @xiaoqing.lu(注) 这是一段效率很低的代码，读取4次NV，保存4次NV 
#ifdef SET_3D_STYLE_SUPPORT
    MMISET_Set3DStyleScreen(MMISET_DISPLAY_ANIM_3D_EFFECT_SETLIST_CTRL_ID);
#endif
#ifndef FLASH_SUPPORT
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
    MMISET_Set3DStyleMainmenu(MMISET_DISPLAY_ANIM_MAINMENU_EFFECT_SETLIST_CTRL_ID);
#endif
#endif
#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
    MMISET_Set3DStyleList3DStyle(MMISET_DISPLAY_ANIM_LIST_SETLIST_CTRL_ID);
#endif
#ifdef MMI_RES_ORIENT_BOTH // 说明支持横竖屏切换
    MMISET_SetGsensorSwitchStyle(GUISETLIST_GetCurIndex(MMISET_DISPLAY_ANIM_G_SENSOR_SETLIST_CTRL_ID));
#endif
}
#endif

#ifdef MMISET_ANIM_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : Handle the window of power on animation setting.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetPowerOnAnimWindow(
                                               MMI_WIN_ID_T       win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               )
{
    MMI_RESULT_E                    recode              =   MMI_RESULT_TRUE;
    MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_anim_info  =   {0};
    MULTIM_SELECT_RETURN_T          *win_param          =   NULL;
    uint16                          cur_item            = 0;
    MMI_STRING_T                    power_on_name ={0};
    static   MMISET_POWER_ON_OFF_ANIM_TYPE_E   power_off_type = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
#ifdef MMI_PIC_CROP_SUPPORT
    static   BOOLEAN             is_power_on_crop_in    = TRUE;//crop只在开始进入，之后不在进入
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(
            MMISET_SET_POWER_ON_ANIM_CTRL_ID, 
            MMISET_POWER_ON_OFF_ANIM_MAX_NUM, FALSE 
            );//max item 2
        AppendPowerOnOffAnimItem(MMISET_SET_POWER_ON_ANIM_CTRL_ID, TRUE);        
        MMK_SetAtvCtrl(win_id,MMISET_SET_POWER_ON_ANIM_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        cur_item = GUILIST_GetCurItemIndex(MMISET_SET_POWER_ON_ANIM_CTRL_ID);
        MMIAPISET_GetPowerOnOffAnimInfo(&power_on_anim_info,TRUE);
        GUILIST_SetSelectedItem(MMISET_SET_POWER_ON_ANIM_CTRL_ID, power_on_anim_info.anim_type, TRUE);    
        GUILIST_SetCurItemIndex(MMISET_SET_POWER_ON_ANIM_CTRL_ID,cur_item);
        break;    
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        switch (GUILIST_GetCurItemIndex(MMISET_SET_POWER_ON_ANIM_CTRL_ID))
        {
            
        case MMISET_POWER_ON_OFF_ANIM_DEFAULT:
            {
                power_on_anim_info.is_open     = TRUE;
                power_on_anim_info.anim_type   = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
                power_off_type = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
                MMIAPISET_SetPowerOnOffAnimInfo(&power_on_anim_info, TRUE);
                
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &power_on_name);
                GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_ON_LABEL_EDIT_CTRL_ID, &power_on_name, FALSE); //MODIFY BY MARY
                MMK_CloseWin( win_id );
            }
            break;
            
            
        case MMISET_POWER_ON_OFF_ANIM_MORE:
            {
                if (MMIAPIUDISK_UdiskIsRun())
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                }
                else
                {
                    MMIFMM_SELECT_INFO_T    select_info = {{MMIFMM_MOVIE_ALL,0},0,0,PNULL,0,-1};
                    MMIAPISET_GetPowerOnOffAnimInfo(&power_on_anim_info, TRUE);
                    power_off_type = MMISET_POWER_ON_OFF_ANIM_MORE;
#ifdef DRM_SUPPORT
            		select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif                   
                    if(MMISET_POWER_ON_OFF_ANIM_MORE == power_on_anim_info.anim_type)
                    {
                        select_info.ticked_file_wstr = power_on_anim_info.more_anim_info.name_wstr;
                        select_info.ticked_file_len = power_on_anim_info.more_anim_info.name_wstr_len;
                        select_info.win_id = win_id;
                        MMIAPIFMM_OpenSelectMovieWin(&select_info);
                    }
                    else
                    {
                        select_info.ticked_file_wstr = PNULL;
                        select_info.ticked_file_len = 0;
                        select_info.win_id = win_id;
                        MMIAPIFMM_OpenSelectMovieWin(&select_info);
                    }
#ifdef DRM_SUPPORT
                    {
                        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                        limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT;
            			limit_value.is_rights_valid = 1;
                        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
                    }
#endif
                }
            }
            break;
 
        case MMISET_POWER_ON_OFF_PIC_MORE:
        {
        
            MMIFMM_SELECT_INFO_T    select_info = {0};

            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_PIC_ALL;
            select_info.ring_vol = -1;
            MMIAPISET_GetPowerOnOffAnimInfo(&power_on_anim_info, TRUE);
            power_off_type = MMISET_POWER_ON_OFF_PIC_MORE;
#ifdef DRM_SUPPORT
    		select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif                   
			s_fmm_poweron_call = TRUE;
            if(MMISET_POWER_ON_OFF_PIC_MORE == power_on_anim_info.anim_type)
            {
                select_info.win_id = win_id;
                select_info.ticked_file_wstr = power_on_anim_info.more_anim_info.name_wstr;
                select_info.ticked_file_len = power_on_anim_info.more_anim_info.name_wstr_len;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
            }
            else
            {
                select_info.ticked_file_wstr = PNULL;
                select_info.ticked_file_len = 0;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
            }
#ifdef DRM_SUPPORT
            {
                MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT;
    			limit_value.is_rights_valid = 1;
                MMIAPIFMM_SetSelDrmLimitValue(limit_value);
            }
#endif

        }
            break;
 
        default:
            //SCI_PASSERT(0,("HandleSetPowerOnAnimWindow msg_id = %d", msg_id));
            break;
        }
        break;
        
        
        case MSG_MULTIM_SELECTED_RETURN:
            {          
                MMI_TEXT_ID_T   txt_id = 0;
                 
                win_param  = (MULTIM_SELECT_RETURN_T *)param;
                power_on_anim_info.more_anim_info.name_wstr_len = (uint16)MIN(
                    win_param->file_name_len,
                    MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN
                    );
                
                MMI_WSTRNCPY(
                    power_on_anim_info.more_anim_info.name_wstr, 
                    MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN,
                    win_param->file_name_ptr, 
                    win_param->file_name_len,
                    power_on_anim_info.more_anim_info.name_wstr_len
                    );
                
                power_on_anim_info.is_open      = TRUE;
                power_on_anim_info.anim_type    = power_off_type;
#ifdef MMI_PIC_CROP_SUPPORT
				if (MMISET_POWER_ON_OFF_PIC_MORE == power_off_type&&is_power_on_crop_in )
				{
                    is_power_on_crop_in = FALSE;
					MMIAPIFMM_SetPictureToPowerAndSaverEx(win_param->file_name_ptr, win_param->file_name_len);
				}
                else
				{
#endif
                MMIAPISET_SetPowerOnOffAnimInfo(&power_on_anim_info, TRUE);
                AppendPowerOnOffAnimItem(MMISET_SET_POWER_ON_ANIM_CTRL_ID, TRUE);

                /* Modify by michael on 2/23/2012 for NEWMS00171595 : 去掉"成功"提示框*/
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                /************************Modify over*************************/

                if(MMISET_POWER_ON_OFF_ANIM_MORE == power_on_anim_info.anim_type
                    || MMISET_POWER_ON_OFF_PIC_MORE == power_on_anim_info.anim_type)
                {
                    //txt_id = TXT_SET_POWER_ON_OFF_ANIM_MORE;
                    power_on_name.wstr_ptr = power_on_anim_info.more_anim_info.name_wstr;
                    power_on_name.wstr_len = power_on_anim_info.more_anim_info.name_wstr_len;
                }
                else if(MMISET_POWER_ON_OFF_PIC_MORE == power_on_anim_info.anim_type)
                {
                    txt_id = TXT_MORE_PICTURE;
                }
                MMI_GetLabelTextByLang(txt_id, &power_on_name);
                GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_ON_LABEL_EDIT_CTRL_ID, &power_on_name, FALSE); //MODIFY BY MARY
#if !defined MMI_GUI_STYLE_TYPICAL
                MMK_CloseWin( win_id );
#endif
#ifdef MMI_PIC_CROP_SUPPORT
				}
#endif
            }
            break;
            
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );
            break;
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;    
        case MSG_CLOSE_WINDOW:
			s_fmm_poweron_call = FALSE;
#ifdef MMI_PIC_CROP_SUPPORT
			is_power_on_crop_in = TRUE;
#endif
            power_off_type = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : Handle the window of power off animation setting.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetPowerOffAnimWindow(
                                                MMI_WIN_ID_T       win_id, 
                                                MMI_MESSAGE_ID_E   msg_id, 
                                                DPARAM             param
                                                )
{
    MMI_RESULT_E                        recode                =     MMI_RESULT_TRUE;
    MMISET_POWER_ON_OFF_ANIM_INFO_T     power_off_anim_info   =     {0};
    MULTIM_SELECT_RETURN_T              *win_param            =     NULL;
    uint16                              cur_item              = 0;
    MMI_STRING_T power_off_name ={0};
    static   MMISET_POWER_ON_OFF_ANIM_TYPE_E   power_off_type = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
#ifdef MMI_PIC_CROP_SUPPORT
    static   BOOLEAN is_power_off_crop_in    = TRUE;//crop_support return
#endif
	switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(
            MMISET_SET_POWER_OFF_ANIM_CTRL_ID, 
            MMISET_POWER_ON_OFF_ANIM_MAX_NUM, FALSE 
            );//max item 2
        AppendPowerOnOffAnimItem(MMISET_SET_POWER_OFF_ANIM_CTRL_ID, FALSE);
        MMK_SetAtvCtrl(win_id,MMISET_SET_POWER_OFF_ANIM_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        cur_item = GUILIST_GetCurItemIndex(MMISET_SET_POWER_OFF_ANIM_CTRL_ID);
        MMIAPISET_GetPowerOnOffAnimInfo(&power_off_anim_info,FALSE);
        GUILIST_SetSelectedItem(MMISET_SET_POWER_OFF_ANIM_CTRL_ID, power_off_anim_info.anim_type, TRUE);    
        GUILIST_SetCurItemIndex(MMISET_SET_POWER_OFF_ANIM_CTRL_ID,cur_item);
        break;  
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        switch (GUILIST_GetCurItemIndex(MMISET_SET_POWER_OFF_ANIM_CTRL_ID))
        {
            
        case MMISET_POWER_ON_OFF_ANIM_DEFAULT:
            {
                power_off_anim_info.is_open     = TRUE;
                power_off_anim_info.anim_type   = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
                power_off_type = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
                MMIAPISET_SetPowerOnOffAnimInfo(&power_off_anim_info, FALSE);
                
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);

            MMI_GetLabelTextByLang(TXT_EXPORT_LOC_DEFAULT, &power_off_name);
            GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_EDIT_CTRL_ID, &power_off_name, FALSE);// MODIFY BY MARY
                MMK_CloseWin( win_id );
            }
            break;
            
            
        case MMISET_POWER_ON_OFF_ANIM_MORE:
            {
                if (MMIAPIUDISK_UdiskIsRun())
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                }
                else
                {
                    MMIFMM_SELECT_INFO_T    select_info = {0};
                    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_MOVIE_ALL;
                    select_info.ring_vol = -1;
                    MMIAPISET_GetPowerOnOffAnimInfo(&power_off_anim_info, FALSE);
                    power_off_type = MMISET_POWER_ON_OFF_ANIM_MORE;
#ifdef DRM_SUPPORT
            		select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif                   
                    if(MMISET_POWER_ON_OFF_ANIM_MORE == power_off_anim_info.anim_type)
                    {
                        select_info.win_id = win_id;
                        select_info.ticked_file_wstr = power_off_anim_info.more_anim_info.name_wstr;
                        select_info.ticked_file_len = power_off_anim_info.more_anim_info.name_wstr_len;
                        MMIAPIFMM_OpenSelectMovieWin(&select_info);
                    }
                    else
                    {
                        select_info.ticked_file_wstr = PNULL;
                        select_info.ticked_file_len = 0;
                        select_info.win_id = win_id;
                        MMIAPIFMM_OpenSelectMovieWin(&select_info);
                    }
#ifdef DRM_SUPPORT
                    {
                        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                        limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT;
            			limit_value.is_rights_valid = 1;
                        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
                    }
#endif
                }
            }
            break;
     
        case MMISET_POWER_ON_OFF_PIC_MORE:
        {
        
            MMIFMM_SELECT_INFO_T    select_info = {0};
            select_info.ring_vol = -1;
            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_PIC_ALL;

            MMIAPISET_GetPowerOnOffAnimInfo(&power_off_anim_info, FALSE);
            power_off_type = MMISET_POWER_ON_OFF_PIC_MORE;
#ifdef DRM_SUPPORT
    		select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif            
			s_fmm_poweroff_call = TRUE;
            if(MMISET_POWER_ON_OFF_PIC_MORE == power_off_anim_info.anim_type)
            {
                select_info.win_id = win_id;
                select_info.ticked_file_wstr = power_off_anim_info.more_anim_info.name_wstr;
                select_info.ticked_file_len = power_off_anim_info.more_anim_info.name_wstr_len;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
            }
            else
            {
                select_info.ticked_file_wstr = PNULL;
                select_info.ticked_file_len = 0;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
            }
#ifdef DRM_SUPPORT
            {
                MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT;
    			limit_value.is_rights_valid = 1;
                MMIAPIFMM_SetSelDrmLimitValue(limit_value);
            }
#endif

        }
            break;
 
        default:
            //SCI_PASSERT(0,("HandleSetPowerOffAnimWindow msg_id = %d", msg_id));
            break;
        }
        break;
        
        
        case MSG_MULTIM_SELECTED_RETURN:
            {          
                MMI_TEXT_ID_T   txt_id = 0;
                
                win_param  = (MULTIM_SELECT_RETURN_T *)param;                
                power_off_anim_info.more_anim_info.name_wstr_len = (uint16)MIN(
                    win_param->file_name_len,
                    MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN
                    );
                
                MMI_WSTRNCPY(
                    power_off_anim_info.more_anim_info.name_wstr, 
                    MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN,
                    win_param->file_name_ptr, 
                    win_param->file_name_len,
                    power_off_anim_info.more_anim_info.name_wstr_len
                    );
                
                power_off_anim_info.is_open      = TRUE;
                power_off_anim_info.anim_type    = power_off_type;
#ifdef MMI_PIC_CROP_SUPPORT
				if (MMISET_POWER_ON_OFF_PIC_MORE == power_off_type &&is_power_off_crop_in)
				{
                    is_power_off_crop_in = FALSE;
					MMIAPIFMM_SetPictureToPowerAndSaverEx(win_param->file_name_ptr, win_param->file_name_len);
				}
				else
				{  
#endif  
                MMIAPISET_SetPowerOnOffAnimInfo(&power_off_anim_info, FALSE);
                AppendPowerOnOffAnimItem(MMISET_SET_POWER_OFF_ANIM_CTRL_ID, FALSE);
                
                /* Modify by michael on 2/23/2012 for NEWMS00171595 : 去掉"成功"提示框*/
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                /************************Modify over*************************/
                
                if(MMISET_POWER_ON_OFF_ANIM_MORE == power_off_anim_info.anim_type
                    || MMISET_POWER_ON_OFF_PIC_MORE == power_off_anim_info.anim_type)
                {
                    //txt_id = TXT_SET_POWER_ON_OFF_ANIM_MORE;
                    power_off_name.wstr_ptr = power_off_anim_info.more_anim_info.name_wstr;
                    power_off_name.wstr_len = power_off_anim_info.more_anim_info.name_wstr_len;
                }
                else if(MMISET_POWER_ON_OFF_PIC_MORE == power_off_anim_info.anim_type)
                {
                    txt_id = TXT_MORE_PICTURE;
                }
                MMI_GetLabelTextByLang(txt_id, &power_off_name);
                
                GUILABEL_SetText(MMISET_DISPLAY_ANIM_POWER_OFF_LABEL_EDIT_CTRL_ID, &power_off_name, FALSE);// MODIFY BY MARY
#if !defined MMI_GUI_STYLE_TYPICAL
                MMK_CloseWin( win_id );
#endif
#ifdef MMI_PIC_CROP_SUPPORT
				}
#endif
            }
            break;
            
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );
            break;
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;    
        case MSG_CLOSE_WINDOW:
            s_fmm_poweroff_call = FALSE;
#ifdef MMI_PIC_CROP_SUPPORT
            is_power_off_crop_in = TRUE;
#endif
			
            power_off_type = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}


/*****************************************************************************/
//  Description : Append the power on off setting items.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void AppendPowerOnOffAnimItem(
                                    MMI_CTRL_ID_T   ctrl_id,
                                    BOOLEAN         is_power_on_off
                                    )
{
    
    uint16                              cur_selection           =   0;
    MMISET_POWER_ON_OFF_ANIM_INFO_T     power_on_off_anim_info  =   {0};
    
    MMIAPISET_AppendListItemByTextId( TXT_EXPORT_LOC_DEFAULT, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#ifdef VIDEO_PLAYER_SUPPORT
    MMIAPISET_AppendListItemByTextId( TXT_SET_POWER_ON_OFF_ANIM_MORE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#endif
    MMIAPISET_AppendListItemByTextId( TXT_MORE_PICTURE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_GetPowerOnOffAnimInfo(
        &power_on_off_anim_info,
        is_power_on_off
        );
    cur_selection = (uint16)power_on_off_anim_info.anim_type;
    
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

#endif
/*****************************************************************************/
//  Description : get power on off animation set info
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_GetPowerOnOffAnimInfo(
                                            MMISET_POWER_ON_OFF_ANIM_INFO_T *power_on_off_anim_info_ptr,    //in/out:
                                            BOOLEAN is_power_on_off     //TRUE:power on, FALSE:power off
                                            )
{
    MN_RETURN_RESULT_E   return_value   =   MN_RETURN_FAILURE;
    BOOLEAN              is_need_reset  =   FALSE;
    BOOLEAN              is_file_exist  =   TRUE;
    
    //SCI_ASSERT(PNULL != power_on_off_anim_info_ptr);
   if(PNULL == power_on_off_anim_info_ptr)
   {
        //SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_GetPowerOnOffAnimInfo"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_2609_112_2_18_2_51_4_176,(uint8*)"");
        return;
    }     
    if(is_power_on_off)
    {
        MMINV_READ(
            MMINV_POWER_ON_ANIM_SETTING_INFO, 
            power_on_off_anim_info_ptr, 
            return_value
            );
    }
    else
    {
        MMINV_READ(
            MMINV_POWER_OFF_ANIM_SETTING_INFO, 
            power_on_off_anim_info_ptr, 
            return_value
            );
    }
    
    if((MMISET_POWER_ON_OFF_ANIM_MORE == power_on_off_anim_info_ptr->anim_type)
        || (MMISET_POWER_ON_OFF_PIC_MORE == power_on_off_anim_info_ptr->anim_type))
    {
        is_file_exist = MMIAPIFMM_IsFileExist(power_on_off_anim_info_ptr->more_anim_info.name_wstr, power_on_off_anim_info_ptr->more_anim_info.name_wstr_len);
    }
    if ((MN_RETURN_SUCCESS != return_value) || (!is_file_exist))
    {
        is_need_reset = TRUE;
    }
#ifdef DRM_SUPPORT
        if (!is_need_reset
            && MMIAPIDRM_IsDRMFile(SFS_INVALID_HANDLE, power_on_off_anim_info_ptr->more_anim_info.name_wstr)
            && !MMIAPIDRM_IsRightsValid(SFS_INVALID_HANDLE, power_on_off_anim_info_ptr->more_anim_info.name_wstr, TRUE, DRM_PERMISSION_PLAY))
        {
            is_need_reset = TRUE;
        }
#endif	
    if(is_need_reset)
    {    
        power_on_off_anim_info_ptr->is_open     = TRUE;
        power_on_off_anim_info_ptr->anim_type   = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
        SCI_MEMSET(
            &power_on_off_anim_info_ptr->more_anim_info, 
            0, 
            sizeof(MMISET_MORE_POWER_ON_OFF_ANIM_T)
            );
        if(!MMIAPIUDISK_UdiskIsRun())
        {        
            if(is_power_on_off)
            {
                MMINV_WRITE(
                    MMINV_POWER_ON_ANIM_SETTING_INFO, 
                    power_on_off_anim_info_ptr
                    );
            }
            else
            {
                MMINV_WRITE(
                    MMINV_POWER_OFF_ANIM_SETTING_INFO, 
                    power_on_off_anim_info_ptr
                    );
            }
        }
    }
}
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
/*****************************************************************************/
//  Description : to handle select idle display mode
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetIdleDisplayWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;    
    uint16          cur_index   =   0;
	uint16			top_index	=	0;
    
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:  
        MMK_SetAtvCtrl(win_id,MMISET_SET_IDLE_DISPLAY_CTRL_ID);
        break;
    case MSG_FULL_PAINT:
		top_index = GUILIST_GetTopItemIndex(MMISET_SET_IDLE_DISPLAY_CTRL_ID);
		cur_index = GUILIST_GetCurItemIndex(MMISET_SET_IDLE_DISPLAY_CTRL_ID);
        MMISET_AppendIdleDisplayListItem(MMISET_SET_IDLE_DISPLAY_CTRL_ID); 
		GUILIST_SetTopItemIndex(MMISET_SET_IDLE_DISPLAY_CTRL_ID, top_index);
        GUILIST_SetCurItemIndex(MMISET_SET_IDLE_DISPLAY_CTRL_ID, cur_index);
        break;
    case MSG_CTL_OK:
#if !defined MMI_GUI_STYLE_TYPICAL
#ifndef MMI_GUI_STYLE_MINISCREEN //小屏项目不需要执行该语句add by yongsheng.wang
        MMK_SendMsg(win_id,MSG_APP_CANCEL, PNULL);
        break;
#endif
#endif
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:

        //get current selected items
        // selected_num = GUILIST_GetSelectedItemIndex(MMISET_SET_IDLE_DISPLAY_CTRL_ID,selected_item,MMISET_IDLE_DISPLAY_MAX_ITEM);
        cur_index = GUILIST_GetCurItemIndex(MMISET_SET_IDLE_DISPLAY_CTRL_ID); 
		
        switch(cur_index) 
        {
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
        case MMISET_IDLE_CUSTOM_WORD:
            if (MMIAPISET_IsDisplayIdleWords())
            {
                MMISET_SetIdleDisplayMode(cur_index);   
                MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
            }
            else
            {
                MMK_CreateWin((uint32*)MMISET_EDIT_IDLE_WOEDS_WIN_TAB,PNULL);
            } 
            break;
#endif
#endif
        default:
            MMISET_SetIdleDisplayMode(cur_index);   
            MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);            
            break;
        }
        break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            //MMIAPISET_SetDisplaytIdleMode();
            MMK_CloseWin( win_id );      
            break;
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;    
        case MSG_CLOSE_WINDOW:
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
            MMIAPISET_SetDisplaytIdleMode();        /*lint !e718 !e18 */
#endif
#endif
            break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.            
        case MSG_APP_FLIP:
            MMIDEFAULT_HandleFlipKey(FALSE);
            MMIAPISET_SetDisplaytIdleMode();
            recode = MMI_RESULT_FALSE;
            break;
#endif            
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }   
    
    
    return recode;
}

#endif
#endif
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
/*****************************************************************************/
//  Description : to handle edit idle words win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditIdleWordsWindow(
                                              MMI_WIN_ID_T    win_id, 
                                              MMI_MESSAGE_ID_E   msg_id, 
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E            recode          =   MMI_RESULT_TRUE;
#ifdef DPHONE_SUPPORT 
#else       
    MMI_CTRL_ID_T           list_ctrl_id    =   MMISET_SET_IDLE_DISPLAY_CTRL_ID;
#endif 
    MMI_STRING_T            edit_string     =   {0};
    MMISET_IDLE_DISPLAY_T   *display_info   =   PNULL;
    wchar                               forbid_char_value[]= {0x0d, 0x0a};
    display_info = MMIAPISET_GetIdleDisplayMode();
    
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        break;
        
    case MSG_OPEN_WINDOW:
        GUIEDIT_SetTextMaxLen(MMISET_IDLE_WORDS_EDITCTRL_ID,MMISET_IDLE_WORDS_UCS_MAX_LEN,MMISET_IDLE_WORDS_UCS_MAX_LEN);
        GUIEDIT_SetString(MMISET_IDLE_WORDS_EDITCTRL_ID, display_info->idle_words,(uint16)display_info->wstr_len);         

     //set softkey //个性词语可以为空
        //GUIEDIT_SetSoftkey(MMISET_IDLE_WORDS_EDITCTRL_ID,0,1,TXT_NULL,STXT_OK,PNULL);
         GUIEDIT_ConfigDisableChar(MMISET_IDLE_WORDS_EDITCTRL_ID, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
        
        MMK_SetAtvCtrl(win_id, MMISET_IDLE_WORDS_EDITCTRL_ID);
        break;
#ifdef TOUCH_PANEL_SUPPORT
    case  MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T     softkey_ctrl_id = 0;
            softkey_ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
            if(MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == softkey_ctrl_id)
            {
				MMK_PostMsg(win_id, MSG_APP_OK, PNULL, 0);
            }
            else if(MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == softkey_ctrl_id)
            {
                MMK_PostMsg(win_id, MSG_APP_CANCEL, PNULL, 0);
            }
        }
        break;
#endif

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
        // to get the content which the user input  
        SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
        GUIEDIT_GetString( MMISET_IDLE_WORDS_EDITCTRL_ID, &edit_string);

    //个性词语可以为空
    // if (0 == edit_string.wstr_len)
       // {
       //       break;
       // }
        if (MMIAPISET_IsIncludeReturnChar( edit_string.wstr_ptr,edit_string.wstr_len ))
        {
            MMIPUB_OpenAlertWarningWin(TXT_SET_INVALID_CUSTOM_WORD);
            break;
        }
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/04
#else               
#if !(defined(MMI_DISPLAYSET_MINI_SUPPORT) && defined(MMI_ENABLE_DCD))       
        // to save the information
        if (MMISET_SetIdleWordsContent(&edit_string))
        {
            if (MMK_IsOpenWin(MMISET_SET_IDLE_DISPLAY_WIN_ID))
            {
            #if defined MMISET_IDLE_DISPLAY_SUPPORT
                MMISET_SetIdleDisplayMode(MMISET_IDLE_CUSTOM_WORD);  
            #endif
                GUILIST_SetSelectedItem(list_ctrl_id,MMISET_IDLE_CUSTOM_WORD,FALSE);
            }
            MMK_CloseWin( win_id ); 
        }
#endif  
#endif 
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        // close the edit window
        MMK_CloseWin( win_id );        
        break;        
    case MSG_KEYDOWN_RED:
        break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;    
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
/*****************************************************************************/
//  Description : HandleContrastTPDown
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: for user touch bar support
/*****************************************************************************/
PUBLIC void MMISET_HandleContrastTPDown(
                                GUI_POINT_T     *point_ptr,
                                MMI_WIN_ID_T    win_id,
                                MMI_CTRL_ID_T   ctrl_id,
                                BOOLEAN         is_called_by_ps//是否由省电模式呼叫
                                )
{
    int16           cur_contrast = 0;
    int16           step_len = 0;
    int16           display_x = 0;
    int16           display_y = 0;
    uint16          bar_width = 0;
    uint16          bar_height = 0;
    GUI_RECT_T      display_rect = {0};
    GUI_RECT_T      ctrl_display_rect = {0};
    GUI_RECT_T      img_rect = {0};
#ifdef MMI_POWER_SAVING_MODE_SUPPORT    
    MMISET_POWER_SETTING_ITEM_T power_sav_setting = MMIAPISET_GetPowerSavingSetting();
#endif

#ifdef MMI_GUI_STYLE_MINISCREEN
    bar_width = MMISET_DISPLAY_CONTRAST_BAR_LONG;
    bar_height = MMISET_DISPLAY_CONTRAST_BAR_HIGHT;
#else
    GUIRES_GetImgWidthHeight(&bar_width,
        &bar_height,
        IMAGE_SET_CONTRAST_BAR_FG,
        win_id);
#endif

    GUIOWNDRAW_GetRect(ctrl_id,&display_rect);
    display_x = display_rect.left;
    display_y = display_rect.top;
    GUIANIM_AdjustDisplayPosition(&display_x,
        &display_y,
        bar_width,
        bar_height,
        display_rect,
        GUIANIM_ALIGN_HVMIDDLE);

    //set display rect
    display_rect.left   = display_x;
    display_rect.top    = display_y;
    display_rect.right  = (int16)(display_rect.left + bar_width - 1);
    display_rect.bottom = (int16)(display_rect.top + bar_height - 1);

    GUIOWNDRAW_GetDisplayRect(ctrl_id,&ctrl_display_rect);

    if (GUI_IntersectRect(&display_rect,display_rect,ctrl_display_rect))
    {
        //set image rect
        img_rect.left   = (int16)(display_rect.left - display_x);
        img_rect.top    = (int16)(display_rect.top - display_y);
        img_rect.right  = (int16)(display_rect.right - display_x);
        img_rect.bottom = (int16)(display_rect.bottom - display_y);

        //display bg
        GUIRES_DisplayImg(PNULL,
            &display_rect,
            &img_rect,
            win_id,
#ifdef MMI_GUI_STYLE_MINISCREEN
            IMAGE_SET_CONTRAST_BAR_BG_NEW,
#else
            IMAGE_SET_CONTRAST_BAR_BG,
#endif
            MMITHEME_GetDefaultLcdDev());

        //get current contrast
        if (point_ptr->x <= display_rect.left)
        {
            cur_contrast = 0;
        }
        else if (point_ptr->x >= display_rect.right)
        {
            cur_contrast = MMISET_CONTRAST_SEVEN;
        }
        else
        {
            step_len     = bar_width/MMISET_CONTRAST_SEVEN;
            cur_contrast = (point_ptr->x - display_x + step_len)/step_len;
        }
        //set image rect
        img_rect.right = img_rect.left + (cur_contrast*bar_width/MMISET_CONTRAST_SEVEN) - 1;

        //display fg
        GUIRES_DisplayImg(PNULL,
            &display_rect,
            &img_rect,
            win_id,
#ifdef MMI_GUI_STYLE_MINISCREEN
            IMAGE_SET_CONTRAST_BAR_BG_NEW,
#else
            IMAGE_SET_CONTRAST_BAR_FG,
#endif
            MMITHEME_GetDefaultLcdDev());

#ifdef MMI_POWER_SAVING_MODE_SUPPORT    
        if(is_called_by_ps)//NEWMS00160657是由省电模式呼叫，且当前未开启省电模式
        {
            power_sav_setting.lcd_contrast = cur_contrast;
            MMIAPISET_SetPowerSavingSetting(power_sav_setting);
            MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,cur_contrast);
            
            if(power_sav_setting.is_active)
            {
                MMIAPISET_SetCurrentContrast(cur_contrast);
            }
        }
        else
#endif
        {
            MMIAPISET_SetCurrentContrast(cur_contrast);
            MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,cur_contrast);
        }
    }
} 




/*****************************************************************************/
//  Description : save all setting
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SaveAllFeatureState(void)
{
    //设置背光亮度
    MMIAPISET_SetMainLCDContrast();
    //背光时间
//#if defined MMISET_BACKLIGHT_TIME_SUPPORT
//    MMISET_SetBackLight(MMISET_DISPLAY_BACKLIGHT_SETLIST_CTRL_ID);
//#endif
    MMIDEFAULT_TurnOnLCDBackLight();        
    //按键背光时间
//#if defined MMISET_KEYBACKLIGHT_SUPPORT
//    MMISET_SetKeyboardLight(MMISET_DISPLAY_KEYPAD_BACKLIGHT_SETLIST_CTRL_ID); 
//#endif
#if defined MMISET_IDLE_STYLE_SUPPORT
    //待机界面风格
    if ( SetIdleStyle(MMISET_DISPLAY_IDLE_STYLE_SETLIST_CTRL_ID) )/*lint !e774*/
    {
        MMK_ReturnIdleWin();
    }
#endif
}  

#if defined DPHONE_SUPPORT
/*****************************************************************************/
//  Description : to handle set backlight
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetBacklightWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_BACKLIGHT_CTRL_ID,MMISET_BACKLIGHT_MAX_ITEM, FALSE );//max item 6
        MMISET_AppendBacklightListItem(MMISET_SET_BACKLIGHT_CTRL_ID);
        
        MMK_SetAtvCtrl(win_id,MMISET_SET_BACKLIGHT_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        MMISET_SetBackLight(MMISET_SET_BACKLIGHT_CTRL_ID);
        MMIDEFAULT_TurnOnLCDBackLight();        
        //success!
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}


/*****************************************************************************/
//  Description : to handle set constrast
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetConstrastWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{

    MMI_RESULT_E    recode          =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id         =   MMISET_CONTRAST_DOWN_BUTTON_CTRL_ID;
    static uint8    cur_constrast   =   0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMISET_DisplayInitContrast(win_id);
        SetContrastFormParam();
        break;
    case MSG_FULL_PAINT:
        //MMIMAIN_DisplayDefaultWallpaper();
        break;
        
        
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
        MMISET_TuneUpConstrast(msg_id);
        //display contrast picutres
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
        break;
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            //HandleContrastTPDown(point,win_id);
        }
        break;

     case MSG_CTL_PENOK:
        ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
        
        if (MMISET_CONTRAST_DOWN_BUTTON_CTRL_ID == ctrl_id)
        {
            MMK_PostMsg(win_id, MSG_APP_LEFT, PNULL, 0);
        }
        else if (MMISET_CONTRAST_UP_BUTTON_CTRL_ID == ctrl_id)
        {
            MMK_PostMsg(win_id, MSG_APP_RIGHT, PNULL, 0);
        }       
        break;      
#endif        

    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
        MMIAPISET_SetMainLCDContrast();
        
        //success!
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin( win_id );
        break;
        
    case MSG_LOSE_FOCUS:
        cur_constrast = MMIAPISET_GetCurrentContrast();
        //SCI_TRACE_LOW:"HandleSetConstrastWindow cur_constrast = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_3150_112_2_18_2_51_5_177,(uint8*)"d",cur_constrast);
        //MMIAPISET_ResetContrast(GUI_MAIN_LCD_ID);
        break;
        
    case MSG_GET_FOCUS:
        //SCI_TRACE_LOW:"HandleSetConstrastWindow cur_constrast = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_3155_112_2_18_2_51_5_178,(uint8*)"d",cur_constrast);
        MMIAPISET_SetCurrentContrast(cur_constrast);      
        MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,cur_constrast);       
        
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMIAPISET_ResetContrast(GUI_MAIN_LCD_ID);
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_APP_RED:
        //SCI_TRACE_LOW("HandleSetConstrastWindow:received MSG_APP_RED and return idle win!");
        MMIAPISET_ResetContrast(GUI_MAIN_LCD_ID);
        
        recode = MMI_RESULT_FALSE;
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        //SCI_TRACE_LOW("HandleSetConstrastWindow:received MSG_APP_FLIP and return idle win!");
        MMIAPISET_ResetContrast(GUI_MAIN_LCD_ID);
        recode = MMI_RESULT_FALSE;
        break;
#endif
        
    case MSG_CLOSE_WINDOW:
        cur_constrast = 0;
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : SetContraseFormParam
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void SetContrastFormParam(void)
{
    GUIANIM_CTRL_INFO_T         control_info = {0};
    GUIANIM_DATA_INFO_T         data_info =    {0};
    GUIANIM_DISPLAY_INFO_T      display_info = {0};
   
    data_info.img_id = IMAGE_SET_CONTRAST_BAR_BG;
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID;
    control_info.ctrl_ptr = PNULL;
    
    display_info.is_syn_decode = TRUE;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
}
#endif
/*****************************************************************************/
//  Description : ContrastBarCtrlCallbackFun
//  Global resource dependence : 
//  Author:murphy.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_ContrastBarCtrlCallbackFun (
                                       GUIOWNDRAW_INFO_T    *draw_ptr
                                       )
{
    uint8           cur_contrast = 0;
    int16           display_x = 0;
    int16           display_y = 0;
    uint16          bar_width = 0;
    uint16          bar_height = 0;
#if (defined  MMI_GUI_STYLE_MINISCREEN ) && (!defined TOUCH_PANEL_SUPPORT)
    uint16          triangle_width = 0;
    uint16          triangle_height = 0;
    GUI_RECT_T      triangle_display_rect = {0};
#endif    
    GUI_RECT_T      display_rect = {0};
#ifndef MMI_GUI_STYLE_MINISCREEN //pclint
    GUI_RECT_T      img_rect = {0};
#endif
    MMI_WIN_ID_T    win_id = 0;
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    MMISET_POWER_SETTING_ITEM_T power_sav_setting = MMIAPISET_GetPowerSavingSetting();
#endif

    if(MMK_IsOpenWin(MMISET_SET_DISPLAY_WIN_ID))
    {
        win_id = MMISET_SET_DISPLAY_WIN_ID;
        //get current contrast
        cur_contrast = MMIAPISET_GetCurrentContrast();
    }
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    else if(MMK_IsOpenWin(MMISET_POWER_SAVE_SETTING_WIN_ID))
    {
        win_id = MMISET_POWER_SAVE_SETTING_WIN_ID;
        //get powersaving contrast
        cur_contrast = power_sav_setting.lcd_contrast;
    }
#endif

#ifdef MMI_GUI_STYLE_MINISCREEN
#ifndef TOUCH_PANEL_SUPPORT  //Touch情况下不需要三角提示
    GUIRES_GetImgWidthHeight(&triangle_width,
                                                    &triangle_height,
                                                    IMAGE_SET_CONTRAST_TRIANGLE_LEFT,
                                                    win_id);
#endif
    bar_width = MMISET_DISPLAY_CONTRAST_BAR_LONG;
    bar_height = MMISET_DISPLAY_CONTRAST_BAR_HIGHT;
    
    display_x = draw_ptr->ctrl_rect.left;
    display_y = draw_ptr->ctrl_rect.top;
    
    GUIANIM_AdjustDisplayPosition(&display_x,
                                                        &display_y,
                                                        bar_width,
                                                        bar_height,
                                                        draw_ptr->ctrl_rect,
                                                        GUIANIM_ALIGN_HVMIDDLE);

    //set display rect
    display_rect.left   = display_x;
    display_rect.top    = display_y;
    display_rect.right  = (int16)(display_rect.left + bar_width - 1);
    display_rect.bottom = (int16)(display_rect.top + bar_height - 1);

    if (GUI_IntersectRect(&display_rect,display_rect,draw_ptr->display_rect))
    {
#ifndef TOUCH_PANEL_SUPPORT  //Touch情况下不需要三角提示
        if(MMK_IsActiveCtrl(MMISET_DISPLAY_CONTRAST_OWNERDRAW_CTRL_ID)
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
           || MMK_IsActiveCtrl(MMISET_POWER_SAVE_SETTING_BACKLIGHT_ANIM_CTRL_ID)
#endif
          )
        {
            //display left triangle 
            triangle_display_rect.left = display_rect.left - MMISET_DISPLAY_CONTRAST_TRIANGLE_LEFT_OFFSET;
            triangle_display_rect.top = display_rect.top + MMISET_DISPLAY_CONTRAST_TRIANGLE_TOP_OFFSET;
            triangle_display_rect.right =  triangle_display_rect.left + triangle_width;
            triangle_display_rect.bottom = triangle_display_rect.top + triangle_height;

            GUIRES_DisplayImg(PNULL,
                                            &triangle_display_rect,
                                            PNULL,
                                            win_id,
                                            IMAGE_SET_CONTRAST_TRIANGLE_LEFT,
                                            MMITHEME_GetDefaultLcdDev());

            //display right triangle 
            triangle_display_rect.left = display_rect.right + MMISET_DISPLAY_CONTRAST_TRIANGLE_RIGHT_OFFSET;
            triangle_display_rect.top = display_rect.top + MMISET_DISPLAY_CONTRAST_TRIANGLE_TOP_OFFSET;
            triangle_display_rect.right =  triangle_display_rect.left + triangle_width;
            triangle_display_rect.bottom = triangle_display_rect.top + triangle_height;
        
            GUIRES_DisplayImg(PNULL,
                                            &triangle_display_rect,
                                            PNULL,
                                            win_id,
                                            IMAGE_SET_CONTRAST_TRIANGLE_RIGHT,
                                            MMITHEME_GetDefaultLcdDev());
        }
#endif
        
        //display bg
        GUIRES_DisplayImg(PNULL,
                                        &display_rect,
                                        PNULL,
                                        win_id,
                                        IMAGE_SET_CONTRAST_BAR_BG_NEW,
                                        MMITHEME_GetDefaultLcdDev());

        //set image rect
        display_rect.right = (int16)(display_rect.left + (cur_contrast*bar_width/MMISET_CONTRAST_SEVEN) - 1) - MMISET_DISPLAY_CONTRAST_HIGHLIGHT_RIGHT_OFFSET;        
        display_rect.top = display_rect.top + MMISET_DISPLAY_CONTRAST_HIGHLIGHT_TOP_OFFSET;
        display_rect.left = display_rect.left + MMISET_DISPLAY_CONTRAST_HIGHLIGHT_LEFT_OFFSET;
        //display fg
        GUIRES_DisplayImg(PNULL,
            &display_rect,
            PNULL,
            win_id,
            IMAGE_SET_CONTRAST_BAR_FG_NEW,
            MMITHEME_GetDefaultLcdDev());
    }   
#else
    GUIRES_GetImgWidthHeight(&bar_width,
        &bar_height,
        IMAGE_SET_CONTRAST_BAR_FG,
        win_id);

    display_x = draw_ptr->ctrl_rect.left;
    display_y = draw_ptr->ctrl_rect.top;
    GUIANIM_AdjustDisplayPosition(&display_x,
        &display_y,
        bar_width,
        bar_height,
        draw_ptr->ctrl_rect,
        GUIANIM_ALIGN_HVMIDDLE);

    //set display rect
    display_rect.left   = display_x;
    display_rect.top    = display_y;
    display_rect.right  = (int16)(display_rect.left + bar_width - 1);
    display_rect.bottom = (int16)(display_rect.top + bar_height - 1);

    if (GUI_IntersectRect(&display_rect,display_rect,draw_ptr->display_rect))
    {
        //set image rect
        img_rect.left   = (int16)(display_rect.left - display_x);
        img_rect.top    = (int16)(display_rect.top - display_y);
        img_rect.right  = (int16)(display_rect.right - display_x);
        img_rect.bottom = (int16)(display_rect.bottom - display_y);

        //display bg
        GUIRES_DisplayImg(PNULL,
            &display_rect,
            &img_rect,
            win_id,
            IMAGE_SET_CONTRAST_BAR_BG,
            MMITHEME_GetDefaultLcdDev());


        //set image rect
        img_rect.right = (int16)(img_rect.left + (cur_contrast*bar_width/MMISET_CONTRAST_SEVEN) - 1);

        //display fg
        GUIRES_DisplayImg(PNULL,
            &display_rect,
            &img_rect,
            win_id,
            IMAGE_SET_CONTRAST_BAR_FG,
            MMITHEME_GetDefaultLcdDev());
    }
#endif    
}


#if defined MMISET_KEY_BOARD_LIGHT_SUPPORT 
/*****************************************************************************/
//  Description : to handle select keyborad light
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetKeyboardLightWindow(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_KEYBOARD_LIGHT_CTRL_ID,MMISET_KEYBOARD_LIGHT_MAX_ITEM, FALSE );//max item 5
        MMISET_AppendKeyboardLightListItem(MMISET_SET_KEYBOARD_LIGHT_CTRL_ID);
        MMK_SetAtvCtrl(win_id,MMISET_SET_KEYBOARD_LIGHT_CTRL_ID);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        MMISET_SetKeyboardLight(MMISET_SET_KEYBOARD_LIGHT_CTRL_ID);
        
        //success!
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
    case MSG_KEYDOWN_RED:
        break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;    
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

#if defined MMISET_FONT_SUPPORT
/*****************************************************************************/
//  Description : load font
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetFont( 
                                 const uint16* new_name,  //字体路径
                                 BOOLEAN       is_unload, //是否卸载
                                 BOOLEAN       is_prompt  //是否需要提示
                                 )
{
    BOOLEAN result = FALSE;
    wchar font_name[MMIFMM_FILE_FULL_PATH_MAX_LEN + 1] = {0};
    
    if ( is_unload )
    {
        result = MMIRES_UnloadFileFont();
        
        //用户主动卸载时才需要清除nv, u盘时不需要.
        if ( is_prompt )
        {
            SetFontInfo( font_name );
        }
    }
    else
    {
        //get default font
        if ( PNULL == new_name )
        {
            GetFontInfo( font_name );
            if ( 0 != *font_name )
            {
                if ( MMIRES_LoadFileFont( font_name ) )
                {
                    result = TRUE;
                }
                else
                {
                    font_name[0] = 0;
                    SetFontInfo( font_name );
                }
            }
        }
        else
        {
            if ( MMIRES_LoadFileFont( new_name ) )
            {
                SetFontInfo( new_name );
                
                result = TRUE;
            }
        }
    }
    
    //是否需要提示用户
    if ( is_prompt )
    {
        if ( result )
        {
            //MMIPUB_OpenAlertSuccessWin(TXT_SET_FONT_SUCCEED);
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_SET_FONT_FAILED);
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : get font info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFontInfo( 
                          wchar * font_name_ptr
                          )
{
    BOOLEAN result = FALSE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_SUCCESS;
    
    if ( PNULL != font_name_ptr )
    {
        MMINV_READ(MMINV_SET_FONT, font_name_ptr, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {           
            *font_name_ptr = 0;
            MMINV_WRITE( MMINV_SET_FONT, font_name_ptr );
        }
        else
        {
            result = TRUE;
        }
    }
    
    return result;
}
/*****************************************************************************/
//  Description : set font info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetFontInfo(
                          const wchar* font_name_ptr
                          )
{
    BOOLEAN result = FALSE;
    
    if ( PNULL != font_name_ptr )
    {
        MMINV_WRITE( MMINV_SET_FONT, font_name_ptr );/*lint !e605*/
        
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : set font
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetFontWindow(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_STRING_T    font_str = {0};
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_FONT_CTRL_ID;
    wchar font_name[MMIFMM_FILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 cur_index = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, 2, FALSE );
        MMIAPISET_AppendListItemByTextIdExt(TXT_SET_DEFAULT_FONT,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO  );
        MMIAPISET_AppendListItemByTextIdExt(TXT_SET_FONT_MORE,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO  );
        MMK_SetAtvCtrl(win_id,ctrl_id);   
        break;
        
    case MSG_FULL_PAINT:
        GetFontInfo( font_name );
        if ( 0 == *font_name )
        {
            cur_index = 0;
        }
        else
        {
            cur_index = 1;
        }
        GUILIST_SetSelectedItem( ctrl_id, cur_index, TRUE );      
        GUILIST_SetCurItemIndex(ctrl_id,cur_index);
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);     
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if (0 == GUILIST_GetCurItemIndex(ctrl_id)) 
        {
            MMIAPISET_SetFont( PNULL, TRUE, TRUE );

            MMI_GetLabelTextByLang(TXT_SET_DEFAULT_FONT, &font_str);
            GUILABEL_SetText(MMISET_DISPLAY_FONT_SETTING_LABEL_EDIT_CTRL_ID, &font_str, FALSE);

            MMK_CloseWin(win_id);
        }
        else
        {
            MMIFMM_SELECT_INFO_T select_info = {{MMIFMM_FONT,0},0,0,PNULL,0,-1};
            
            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_FONT;
            select_info.ticked_file_wstr = PNULL;
            select_info.ticked_file_len = 0;
            select_info.win_id = win_id;
            MMIAPIFMM_OpenSelectFontWin(&select_info);

        }
        break;
        
    case MSG_MULTIM_SELECTED_RETURN:
        { 
            MULTIM_SELECT_RETURN_T *selected_info = (MULTIM_SELECT_RETURN_T *)param;
            
            MMIAPISET_SetFont( selected_info->file_name_ptr, FALSE, TRUE );

            MMI_GetLabelTextByLang(TXT_SET_FONT_MORE, &font_str);
            GUILABEL_SetText(MMISET_DISPLAY_FONT_SETTING_LABEL_EDIT_CTRL_ID, &font_str, FALSE);

            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
    case MSG_KEYDOWN_RED:
        break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;   
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

/*****************************************************************************/
//  Description : open display setting window
//  Global resource dependence : 
//  Author: xiangjie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterDisplaySettingWin(void)
{  
    MMK_CreateWin((uint32 *)MMISET_SET_DISPLAY_WIN_TAB, PNULL);
}

#ifdef _SW_ZDT_PRODUCT_

PUBLIC void MMIAPISET_ZdtDisplayTimeSettingWin(void)
{

	MMK_CreateWin((uint32*)MMISET_DISPLAY_BACKLIGHT_TIMEOUT_WIN_TAB,PNULL);
}


LOCAL MMI_RESULT_E handlePowerOffConfirmationDialog(
                                                MMI_WIN_ID_T    win_id, // 窗口的ID
                                                MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                DPARAM             param   // 相应消息的参数
                                                )
{
    MMI_RESULT_E    result  =   MMI_RESULT_FALSE;
    MMI_WIN_ID_T                wait_win_id             =   0;
    MMIPUB_INFO_T *add_data_ptr = PNULL;
    uint32                      is_delete_all_data      =   FALSE;
    uint32                      time_out                =   0;

    add_data_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if(PNULL != add_data_ptr)
    {
        is_delete_all_data= (uint32)add_data_ptr->user_data;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_OK, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_CANCEL, RIGHT_BUTTON, TRUE);
        break;
		
    case MSG_KEYUP_GREEN:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        MMIAPIPHONE_PowerOff();
        break;

    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;
    case MSG_KEYDOWN_RED:
        break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;

    default:
        break;
    }

   result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
    return (result);
}

PUBLIC void MMIAPISET_ZdtPowerOffWin(void)
{
	MMI_HANDLE_T win_handle;
	GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
#if 0
    MMI_WIN_ID_T	dialog_win_id = MMIPUB_ALERT_WIN_ID;
    MMIPUB_OpenConfirmationDialogWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,STR_QUERY_POWEROFF_NOTIFY,PNULL,PNULL,&dialog_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,handlePowerOffConfirmationDialog,0);
#else
       //mmi_set_reboot_screen_show_style(REBOOT_STYLE_PWROFF_ONLY);
	//MMK_CreateWin((uint32*)MMISET_SET_POWEROFF_WIN_TAB,PNULL);
	mmi_set_reboot_screen_show_style(REBOOT_STYLE_PWROFF_ONLY);
	//MMK_CreateWin((uint32*)MMISET_SET_POWEROFF_WIN_TAB,PNULL);//MMIAPISET_ZdtPowerOffWin();
	win_handle = MMK_CreateWin((uint32*)MMISET_SET_POWER_RESTART_WIN_TAB,PNULL);
	
	if(win_handle != NULL)
		MMK_SetWinRect(win_handle, &rect);
#endif	
}

PUBLIC void MMIAPISET_ZdtBrightnessWin(void)
{
	MMI_HANDLE_T win_handle = MMK_CreateWin((uint32*)MMISET_SET_BRIGHTNESS_WIN_TAB,PNULL);
	GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
	if(win_handle != NULL)
		MMK_SetWinRect(win_handle, &rect);
}
			
PUBLIC void MMIAPISET_CallVolumeWin(void)
{
	MMI_HANDLE_T win_handle = MMK_CreateWin((uint32 *) MMISET_CALL_VOLUME_WIN_TAB, PNULL);
	GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
	if(win_handle != NULL)
		MMK_SetWinRect(win_handle, &rect);
}
extern PUBLIC void Settings_MMSoundWin_Enter( void );
PUBLIC void MMIAPISET_RingToneVolumeWin(void)
{
	MMI_HANDLE_T win_handle = MMK_CreateWin((uint32 *) MMISET_RINGTONE_VOLUME_WIN_TAB, PNULL);
	GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
	if(win_handle != NULL)
		MMK_SetWinRect(win_handle, &rect);
	//Settings_MMSoundWin_Enter();
}
PUBLIC void MMIAPISET_ZdtVolumeWin(void)
{
	MMI_HANDLE_T win_handle = MMK_CreateWin((uint32*)MMISET_SET_VOLUME_WIN_TAB,PNULL);
	GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
	if(win_handle != NULL)
		MMK_SetWinRect(win_handle, &rect);
}

LOCAL MMI_RESULT_E handleRestartConfirmationDialog(
                                                MMI_WIN_ID_T    win_id, // 窗口的ID
                                                MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                DPARAM             param   // 相应消息的参数
                                                )
{
    MMI_RESULT_E    result  =   MMI_RESULT_FALSE;
    MMI_WIN_ID_T                wait_win_id             =   0;
    MMIPUB_INFO_T *add_data_ptr = PNULL;
    uint32                      is_delete_all_data      =   FALSE;
    uint32                      time_out                =   0;

    add_data_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if(PNULL != add_data_ptr)
    {
        is_delete_all_data= (uint32)add_data_ptr->user_data;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_OK, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_CANCEL, RIGHT_BUTTON, TRUE);
        break;

    case MSG_APP_OK:
    case MSG_APP_WEB:
        MMIAPIPHONE_PowerReset();
        break;

    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;
    case MSG_KEYDOWN_RED:
        break;
    case MSG_KEYUP_RED:
        MMK_CloseWin(win_id);
        break;
    default:
        break;
    }

   result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
    return (result);
}

PUBLIC void MMIAPISET_ZdtPowerRestartWin(void)
{
	MMI_HANDLE_T win_handle;
	GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
#if 0
	MMI_WIN_ID_T	dialog_win_id = MMIPUB_ALERT_WIN_ID;
	MMIPUB_OpenConfirmationDialogWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,STR_QUERY_RESTART_NOTIFY,PNULL,PNULL,&dialog_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,handleRestartConfirmationDialog,0);
#else
	mmi_set_reboot_screen_show_style(REBOOT_STYLE_RESTART_ONLY);
	//MMK_CreateWin((uint32*)MMISET_SET_POWEROFF_WIN_TAB,PNULL);//MMIAPISET_ZdtPowerOffWin();
	win_handle = MMK_CreateWin((uint32*)MMISET_SET_POWER_RESTART_WIN_TAB,PNULL);
	if(win_handle != NULL)
		MMK_SetWinRect(win_handle, &rect);
#endif	
}

PUBLIC void MMIAPISET_ZdtPowerRestoreWin(void)
{
 	MMI_HANDLE_T win_handle;
	GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
	
#if 0
	MMI_WIN_ID_T	dialog_win_id = MMIPUB_ALERT_WIN_ID;
	MMIPUB_OpenConfirmationDialogWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,STR_QUERY_RESTART_NOTIFY,PNULL,PNULL,&dialog_win_id,PNULL,MMIPUB_SOFTKEY_DIALOG_OK_CLOSE,handleRestartConfirmationDialog,0);
#else
	mmi_set_reboot_screen_show_style(REBOOT_STYLE_RESTORE_ONLY);
	//MMK_CreateWin((uint32*)MMISET_SET_POWEROFF_WIN_TAB,PNULL);//MMIAPISET_ZdtPowerOffWin();
	win_handle = MMK_CreateWin((uint32*)MMISET_SET_POWER_RESTART_WIN_TAB,PNULL);
	if(win_handle != NULL)
		MMK_SetWinRect(win_handle, &rect);
#endif	
}

PUBLIC void MMIAPISET_ZdtWatchInfoWin(void)
{
	MMI_HANDLE_T win_handle = MMK_CreateWin((uint32*)MMISET_SET_WATCH_INFO_WIN_TAB,PNULL);
		GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
	if(win_handle != NULL)
		MMK_SetWinRect(win_handle, &rect);
}

#endif

/*****************************************************************************/
// Description : open set idle display win
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-21
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenSetIdleDisWin(void)
{
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
    MMK_CreatePubListWin((uint32*)MMISET_SET_IDLE_DISPLAY_WIN_TAB,PNULL);
#endif
#endif
}

#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
PUBLIC void MMIAPISET_CreateCustomWordEditWin(void)
{
    MMK_CreateWin((uint32*)MMISET_EDIT_IDLE_WOEDS_WIN_TAB,PNULL);
}
#endif   
#endif

#ifndef MMI_DISPLAYSET_MINI_SUPPORT
/*****************************************************************************/
//  Description : set main menu style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetMainmenuStyle(
                            MMI_CTRL_ID_T       ctrl_id
                            )
{
    uint16              cur_selection   =   0;
    MMIMAINMENU_TYPE_E     mainmenu_style  =   MMIMAINMENU_MATRIX_E;
    BOOLEAN             result = FALSE;
    //get the selected item
    //GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
    cur_selection = GUISETLIST_GetCurIndex(ctrl_id);

#ifndef MMI_DISPLAYSET_MINI_SUPPORT   
    mainmenu_style = MMIMAINMENU_MATRIX_TITLE_E;
#else
    switch (cur_selection)
    {
    case GUIMENU_STYLE_INDEX_ICON:
        mainmenu_style = MMIMAINMENU_MATRIX_E;
        break;
        
    case GUIMENU_STYLE_INDEX_TITLE_ICON:
        mainmenu_style = MMIMAINMENU_MATRIX_TITLE_E;
        break;

#ifdef PDA_UI_SUPPORT_MANIMENU_GO
    case GUIMENU_STYLE_INDEX_GO:
        mainmenu_style = MMIMAINMENU_GO_SLIDE_E;
        break;
#endif
#ifdef MENU_SWEETS_SUPPORT
    case GUIMENU_STYLE_INDEX_SWEETS:
        mainmenu_style = MMIMAINMENU_SWEETS_E;
        break;
#endif    
    default:
        //SCI_TRACE_LOW:"SetMainmenuStyle:cur_selection %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_3679_112_2_18_2_51_6_179,(uint8*)"d",cur_selection);
        break;
    }
#endif
    
    if (mainmenu_style !=MMIAPISET_GetMainmenuStyle())
    {
        //set main menu style
        MMIAPISET_SetMainmenuStyle(mainmenu_style);
        result = TRUE;
    }
    return result;
} 
#endif


/*****************************************************************************/
//  Description : set idle style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetIdleStyle(
                           MMI_CTRL_ID_T  ctrl_id
                           )
{
    BOOLEAN                 result      =   FALSE;
    uint16                  cur_index   =   0;
    MMISET_IDLE_STYLE_E     idle_style  =   MMISET_IDLE_STYLE_COMMON;
    
    //get the selected item
    //GUILIST_GetSelectedItemIndex(ctrl_id,&cur_index,1);
    cur_index = GUISETLIST_GetCurIndex(ctrl_id);
    switch (cur_index)
    {
#ifndef MMI_PDA_SUPPORT    
    case 0:
        idle_style = MMISET_IDLE_STYLE_COMMON;
        break;
        
    case 1:
        idle_style = MMISET_IDLE_STYLE_SMART;
        break;
        
#ifdef MMIWIDGET_SUPPORT
    case 2:
        idle_style = MMISET_IDLE_STYLE_WIDGET;
        break;
#endif

#ifdef MMI_GRID_IDLE_SUPPORT
    case 2:
        idle_style = MMISET_IDLE_STYLE_GRID;
        break;
#endif

#else //MMI_PDA_SUPPORT

#ifdef MMI_GRID_IDLE_SUPPORT
    case 0:
        idle_style = MMISET_IDLE_STYLE_GRID;
        break;
#endif
#if defined MMI_ISTYLE_SUPPORT 
    case 1:
        idle_style = MMISET_IDLE_ISTYLE;
        break;
#endif		
#endif //MMI_PDA_SUPPORT

    default:
        //SCI_TRACE_LOW:"SetIdleStyle:cur_index %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAYWIN_3747_112_2_18_2_51_6_180,(uint8*)"d",cur_index);
        break;
    }
#ifdef BAR_PHONE_WIDGET
    if(MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
    {
        if ( idle_style != MMIAPISET_GetPreIdleStyle() )
        {
            //set idle style
            MMINV_WRITE(MMINV_SET_IDLE_FORMER_STYLE,&idle_style);
            result = FALSE;            
        }  
    }
    else
#endif
    if ( idle_style != MMIAPISET_GetIdleStyle() )
    {
        //set idle style
        MMISET_SetIdleStyle(idle_style);

        result = TRUE;
    }

    return result;
}  
#if defined MMISET_MAINMENU_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : SetMainmenuEffectParam
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetMainmenuEffectParam(void)
{
    MMI_TEXT_ID_T    mainmenu_effect_txt = {0};
    MMI_CTRL_ID_T   title_ctrl_id = MMISET_DISPLAY_MAINMENU_LABEL_CTRL_ID;
    MMI_CTRL_ID_T   content_ctrl_id = MMISET_DISPLAY_MAINMENU_CONTENT_CTRL_ID;

	//只在andriod下显示设置主菜单的选项
	if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()||MMISET_IDLE_STYLE_SMART == MMIAPISET_GetIdleStyle())
	{
		GUIFORM_SetStyle(MMISET_DISPLAY_MAINMENU_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
		GUILABEL_SetTextById(title_ctrl_id, TXT_SET_MAINMENU_STYLE, FALSE);
		mainmenu_effect_txt = MMISET_GetMenuStyleTxt();
		GUILABEL_SetTextById(content_ctrl_id, mainmenu_effect_txt, FALSE);
	}
	else
	{
        GUIFORM_SetChildDisplay(MMISET_DISPLAY_FORM_CTRL_ID, MMISET_DISPLAY_MAINMENU_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
	}
}
#endif
#if defined MMISET_UIDLE_EFFECT_SUPPORT
/*****************************************************************************/
//  Description : SetMainmenuEffectParam
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL void SetUIdleEffectParam(void)
{
    MMI_TEXT_ID_T   uidle_effect_txt = {0};
    MMI_CTRL_ID_T   title_ctrl_id   = MMISET_DISPLAY_UIDLE_LABEL_CTRL_ID;
    MMI_CTRL_ID_T   content_ctrl_id = MMISET_DISPLAY_UIDLE_CONTENT_CTRL_ID;
#if defined( PDA_UI_SUPPORT_U_IDLE)
	//只在andriod下显示设置主菜单的选项
	if (   MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()
	    || MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
	{
		GUIFORM_SetStyle(MMISET_DISPLAY_UIDLE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
		GUILABEL_SetTextById(title_ctrl_id, TXT_SET_UIDLE_STYLE, FALSE);
		
		uidle_effect_txt = MMISET_GetUIdleStyleTxt();
		GUILABEL_SetTextById(content_ctrl_id, uidle_effect_txt, FALSE);
	}
	else
#endif     
	{
        GUIFORM_SetChildDisplay(MMISET_DISPLAY_FORM_CTRL_ID, MMISET_DISPLAY_UIDLE_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
	}
}
#endif
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
/*****************************************************************************/
//  Description : SetMainmenuEffectParam
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetMainmenuSelectEffectParam(void)
{
    MMI_TEXT_ID_T    select_effect_txt = {0};
    MMI_CTRL_ID_T   title_ctrl_id = MMISET_MAINMENU_SELECT_STYLE_LABEL_CTRL_ID;
    MMI_CTRL_ID_T   content_ctrl_id = MMISET_MAINMENU_SELECT_STYLE_CONTENT_CTRL_ID;

	//只在andriod下显示设置选项
	//if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
	{
		GUIFORM_SetStyle(MMISET_MAINMENU_SELECT_STYLE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
		GUILABEL_SetTextById(title_ctrl_id, TXT_SET_MAINMENU_SELECT_STYLE, FALSE);
		select_effect_txt = MMISET_GetMenuSelectStyleTxt();
		GUILABEL_SetTextById(content_ctrl_id, select_effect_txt, FALSE);   
	}
	//else
	{
    //    GUIFORM_SetChildDisplay(MMISET_DISPLAY_FORM_CTRL_ID, MMISET_MAINMENU_SELECT_STYLE_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
	}
}

#endif
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
/*****************************************************************************/
//  Description : MMIAPISET_IsPoweronCall
//  Global resource dependence : 
//  Author:liyan.zhu
//  Note: copy MMIAPISET_IsWallpaperCall
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsPoweronCall(void)
{
    return s_fmm_poweron_call;
}
/*****************************************************************************/
//  Description : MMIAPISET_IsPoweroffCall
//  Global resource dependence : 
//  Author:liyan.zhu
//  Note: copy MMIAPISET_IsWallpaperCall
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsPoweroffCall(void)
{
    return s_fmm_poweroff_call;
}
#endif



/*Edit by script, ignore 1 case. Thu Apr 26 19:01:05 2012*/ //IGNORE9527


/*Edit by script, ignore 12 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
