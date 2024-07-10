/*****************************************************************************
** File Name:      mmiset_other.c                                            *
** Author:                                                                   *
** Date:           03/06/2004                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004       Jassmine.Meng       Create
******************************************************************************/

#define _MMISET_FUC_C_
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmicc_export.h"
#if defined(MMI_MSD_SUPPORT)
#include "mmimsd_export.h"
#endif
#include "mmiset_func.h"
#include "mmi_textfun.h"
#include "mmimms_export.h"
#include "mmisms_export.h"
#include "mmicl_export.h"
#include "mmistk_export.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiset_display.h"
#include "mmiset_internal.h"
//#include "mmiset_call.h"
#include "mmiset_call_internal.h"
#include "mmiset_nv.h"
#include "mmiset_id.h"
#include "mmiset_text.h"
#include "mmiset_wintab.h"
#include "mmiset_set_ring.h"
#include "nvitem.h"
#include "mmipub.h"
//#include "mmiss_export.h"
#include "mmi_anim.h"
#include "mmiacc_event.h"
#include "mmipb_export.h"
#include "mmimms_export.h"
#include "mmi_theme.h"  //@yongwei.he for multi-langugae macro definition
#include "mmi_modu_main.h"
//#include "mmiset_multisim.h"
#include "mmiset_multisim_internal.h"
#include "mn_api.h"
#include "mmiset_image.h"//@zhaohui add for plmn icon
//#include "mmicountedtime_export.h"
#include "mmi_modu_main.h"
#include "mmidc_export.h"
#if 1 //def WIN32
#include "mmi_nv.h"
#endif
#include "nv_item_id.h"
#include "mmiphone_export.h"
#include "mmibt_export.h"
#include "mmifm_export.h"
#include "mmi_custom_define.h"
#include "mmi_appmsg.h"
#include "mmiconnection_export.h"
//#include "mmiebook.h"
#ifdef EBOOK_SUPPORT
#include "mmiebook_export.h"
#endif
//#ifdef CMMB_SUPPORT //bug 2099158
#include "mmimtv_export.h"
//#endif
//#ifdef ATV_SUPPORT
#include "mmiatv_export.h"
//#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#endif
#include "mmiim.h"
#ifdef DCD_SUPPORT
#include "mmidcd_export.h"
#endif
//#ifdef PIM_SUPPORT  //bug 2099158
#include "mmipim_export.h"
//#endif
#include "mmipicview_export.h"
#include "mn_api.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmisrveng.h"

#ifdef ASP_SUPPORT
#include "mmiasp_export.h"
#endif
#include "guidropdownlist.h"
#include "mmiim_im_switching.h"
//#include "mmiim_nv.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#ifdef MMI_MEMO_SUPPORT
#include "mmimemo_export.h"
#endif
#include "mmiim.h"
#include "mmimp3_export.h"
#include "mmifmm_export.h"
#ifdef PUSH_EMAIL_SUPPORT//sunguochen add
#include "mail_main.h"
#endif/*PUSH_EMAIL_SUPPORT*/
#include "mmiwclk_export.h"
#include "guisetlist.h"
#include "guilabel.h"
#include "mmi_resource.h"
#include "mmibt_export.h"
#if defined(MMI_UPDATE_TIME_SUPPORT)
#include "mmiaut_export.h"
#endif /*MMI_UPDATE_TIME_SUPPORT*/
#ifdef MMI_AZAN_SUPPORT
#include "mmiazan_export.h"
#endif
#include "mmicom_time.h"

#ifdef WEATHER_SUPPORT
#include "mmiweather_export.h"
#endif
#ifdef GAME_SUPPORT
#include "mmigame_export.h"
#endif
#ifdef MMI_KING_MOVIE_SUPPORT
#include "mmikm_export.h"
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
#include "mmiidle_statusbar.h"
#if defined MMI_GRID_IDLE_SUPPORT || (defined QBTHEME_SUPPORT)
#include "mmiwidget_export.h"
#endif
#if defined MMIUZONE_SUPPORT
#include "mmiuzone_export.h"
#endif
#if (defined QBTHEME_SUPPORT)
#include "mmiqbtheme_config.h"
#include "mmiqbtheme_item.h"
#include "mmiqbtheme_grid.h"
#include "mmiqbtheme_page.h"
#include "mmiqbtheme_virtualwin.h"
#include "mmiqbtheme_idleexport.h"
#include "mmiqbtheme_dockbar.h"
#endif
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
#include "mmi_mainmenu_export.h"
#endif
#include "ref_outport.h"

#ifdef CSC_XML_SUPPORT
#include "mmicus_export.h"
#endif

#ifdef MCARE_V31_SUPPORT
#include "Mcare_Interface.h"
#endif
#include "mmiset_lock_effect_export.h"

#include "mmk_gsensor.h"

#if defined IM_ENGINE_SOGOU     //非NV数据恢复
#include "mmiim_sogou.h"
#endif

#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#include "mmiplmn_export.h"
#endif
#ifdef MMI_EMAIL_SUPPORT
#include "mmiemail_export.h"
#endif

#ifdef VIDEO_PLAYER_SUPPORT
    #include "mmivp_export.h"
#endif

#ifdef JAVA_SUPPORT_MYRIAD
#include "mmijava_export.h"
#endif

#ifdef MMI_INDIAN_SOS_SUPPORT
#include "mmiset_sos.h"
#endif

#ifdef RESETFACTORY_SETTING_SIMPLE
#include "resetfactorysetting.h"
#endif
#ifdef BBM_ROLE_SUPPORT
#include "bbm_export.h"
#endif
#ifdef APP_STORE_SUPPORT
#include "appstore_control.h"
#endif
#ifdef OD_GAME_SUPPORT
extern void od_game_load_nv();
extern void od_game_save_nv();
#endif
#include "ual_tele_radio.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_appcontrol.h"
#endif
#include "mmiphone_internal.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//
#define     MMISET_KEYBOARD_LIGHT_TIME_20       20000
#define     MMISET_KEYBOARD_LIGHT_TIME_10       10000
#define     MMISET_KEYBOARD_LIGHT_TIME_5        5000
#define     MMISET_KEYBOARD_LIGHT_TIME_AUTO     (1000*60*30)  //夜间模式键盘灯可以亮30分钟
//
#define        MMISET_LANG_NAME_MAX_LEN         20

#define		MMI_MAX_SUPPORT_LANGUAGE_NUM		100
#define		MMI_MAX_RESERVED_NUM				12
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/


typedef struct
{
    wchar   lang_name[MMISET_LANG_NAME_MAX_LEN];
    BOOLEAN lang_status;
}MMISET_LANG_SET_T;

#ifdef MMI_SIM_LANGUAGE_SUPPORT
typedef struct
{
    MMISET_LANGUAGE_TYPE_E  mmi_type;
    uint8  sim_type;
}MMISET_LANG_MAP_SIM_BYTE_T;

typedef struct
{
    MMISET_LANGUAGE_TYPE_E  mmi_type;
    uint8    sim_type[2];
}MMISET_LANG_MAP_SIM_EXT_BYTE_T;
#endif

typedef struct
{
	uint8 flag[MMI_MAX_SUPPORT_LANGUAGE_NUM];
	uint8 reserved[MMI_MAX_RESERVED_NUM];
}MMI_MULCONFIG_INFO_T;

/*lint -save -e785 */
//各语言顺序必须与MMISET_LANGUAGE_TYPE_E的定义顺序相同
MMISET_LANG_SET_T s_lang_set[MMISET_MAX_LANGUAGE + 1] = 
{
#ifdef MMI_SIM_LANGUAGE_SUPPORT
    {{'A','u','t','o',0},TRUE},
#endif

    {{'E','n','g','l','i','s','h',0}, TRUE},
    
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
    {{0x7B80,0x4F53,0x4E2D,0x6587}, TRUE},
//#else
//    {{0x7B80,0x4F53,0x4E2D,0x6587}, FALSE},
#endif
    
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
    {{0x7E41,0x9AD4,0x4E2D,0x6587}, TRUE},
//#else
//    {{0x7E41,0x9AD4,0x4E2D,0x6587}, FALSE},
#endif
    
#ifdef IM_ARABIC_SUPPORT
    {{0x0627,0x0644,0x0639,0x0631,0x0628,0x064A,0x0629}, TRUE},
//#else
//    {{0x0627,0x0644,0x0639,0x0631,0x0628,0x064A,0x0629}, FALSE},
#endif
    
#ifdef IM_FRENCH_SUPPORT
    {{0x0046,0x0072,0x0061,0x006E,0x00E7,0x0061,0x0069,0x0073}, TRUE},
//#else
//    {{0x0046,0x0072,0x0061,0x006E,0x00E7,0x0061,0x0069,0x0073}, FALSE},
#endif
    
#ifdef IM_HINDI_SUPPORT
    {{0x0939,0x093F,0x0928,0x094D,0x0926,0x0940}, TRUE},
//#else
//    {{0x0939,0x093F,0x0928,0x094D,0x0926,0x0940}, FALSE},
#endif
    
#ifdef IM_HUNGARIAN_SUPPORT
    {{0x004D,0x0061,0x0067,0x0079,0x0061,0x0072}, TRUE},
//#else
//    {{0x004D,0x0061,0x0067,0x0079,0x0061,0x0072}, FALSE},
#endif
    
#ifdef IM_INDONESIAN_SUPPORT
    {{0x0042,0x0061,0x0068,0x0061,0x0073,0x0061,0x0020,0x0049,0x006E,0x0064,0x006F,0x006E,0x0065,0x0073,0x0069,0x0061}, TRUE},
//#else
//    {{0x0042,0x0061,0x0068,0x0061,0x0073,0x0061,0x0020,0x0049,0x006E,0x0064,0x006F,0x006E,0x0065,0x0073,0x0069,0x0061}, FALSE},
#endif
    
#ifdef IM_MALAY_SUPPORT
    {{0x0042,0x0061,0x0068,0x0061,0x0073,0x0061,0x0020,0x004D,0x0065,0x006C,0x0061,0x0079,0x0075}, TRUE},
//#else
//    {{0x0042,0x0061,0x0068,0x0061,0x0073,0x0061,0x0020,0x004D,0x0065,0x006C,0x0061,0x0079,0x0075}, FALSE},
#endif
    
#ifdef IM_PORTUGUESE_SUPPORT
    {{0x0050,0x006F,0x0072,0x0074,0x0075,0x0067,0x0075,0x00EA,0x0073}, TRUE},
//#else
//    {{0x0050,0x006F,0x0072,0x0074,0x0075,0x0067,0x0075,0x00EA,0x0073}, FALSE},
#endif
    
#ifdef IM_RUSSIAN_SUPPORT
    {{0x0420,0x0443,0x0441,0x0441,0x043A,0x0438,0x0439}, TRUE},
//#else
//    {{0x0420,0x0443,0x0441,0x0441,0x043A,0x0438,0x0439}, FALSE},
#endif
    
#ifdef IM_SPANISH_SUPPORT
    {{0x0045,0x0073,0x0070,0x0061,0x00F1,0x006F,0x006C}, TRUE},
//#else
//    {{0x0045,0x0073,0x0070,0x0061,0x00F1,0x006F,0x006C}, FALSE},
#endif
    
#ifdef IM_TAGALOG_SUPPORT
    {{0x0054,0x0061,0x0067,0x0061,0x006C,0x006F,0x0067}, TRUE},
//#else
//    {{0x0054,0x0061,0x0067,0x0061,0x006C,0x006F,0x0067}, FALSE},
#endif
    
#ifdef IM_THAI_SUPPORT
    {{0x0E20,0x0E32,0x0E29,0x0E32,0x0E44,0x0E17,0x0E22}, TRUE},
//#else
//    {{0x0E20,0x0E32,0x0E29,0x0E32,0x0E44,0x0E17,0x0E22}, FALSE},
#endif
    
#ifdef IM_VIETNAMESE_SUPPORT
    {{0x0054,0x0069,0x1EBF,0x006E,0x0067,0x0020,0x0056,0x0069,0x1EC7,0x0074}, TRUE},
//#else
//    {{0x0054,0x0069,0x1EBF,0x006E,0x0067,0x0020,0x0056,0x0069,0x1EC7,0x0074}, FALSE},
#endif

#ifdef IM_URDU_SUPPORT
    {{0x0627,0x0631,0x062f,0x0648}, TRUE},
//#else
//    {{0x0627,0x0631,0x062f,0x0648}, FALSE},
#endif

#ifdef IM_ITALIAN_SUPPORT
    {{0x0049,0x0074,0x0061,0x006c,0x0069,0x0061,0x006e,0x006f}, TRUE},
//#else
//    {{0x0049,0x0074,0x0061,0x006c,0x0069,0x0061,0x006e,0x006f}, FALSE},
#endif   
 
#ifdef IM_PERSIAN_SUPPORT
    {{0x067e,0x0627,0x0631,0x0633,0x06cc}, TRUE},
//#else
//    {{0x067e,0x0627,0x0631,0x0633,0x06cc}, FALSE},
#endif
 
#ifdef IM_TURKISH_SUPPORT
    {{0x0054,0x00fc,0x0072,0x006b,0x00e7,0x0065}, TRUE},
//#else
//    {{0x0054,0x00fc,0x0072,0x006b,0x00e7,0x0065}, FALSE},
#endif   

#ifdef IM_GERMAN_SUPPORT
    {{0x0044,0x0065,0x0075,0x0074,0x0073,0x0063,0x0068}, TRUE},
//#else
//    {{0x0044,0x0065,0x0075,0x0074,0x0073,0x0063,0x0068}, FALSE},
#endif
    
#ifdef IM_GREEK_SUPPORT
    {{0x0395,0x03bb,0x03bb,0x03b7,0x03bd,0x03b9,0x03ba,0x03ac}, TRUE},
//#else
//    {{0x0395,0x03bb,0x03bb,0x03b7,0x03bd,0x03b9,0x03ba,0x03ac}, FALSE},
#endif
    
#ifdef IM_HEBREW_SUPPORT  //希伯莱
    {{0x05e2,0x05d1,0x05e8,0x05d9,0x05ea}, TRUE},
//#else
//    {{0x05e2,0x05d1,0x05e8,0x05d9,0x05ea}, FALSE},
#endif

#ifdef IM_BENGALI_SUPPORT //孟加拉
    {{0x09ac,0x09be,0x0982,0x09b2,0x09be}, TRUE},
//#else
//    {{0x09ac,0x09be,0x0982,0x09b2,0x09be}, FALSE},
#endif

#ifdef IM_CZECH_SUPPORT  //捷克
    {{0x010c,0x0065,0x0161,0x0074,0x0069,0x006e,0x0061}, TRUE},
//#else
//    {{0x010c,0x0065,0x0161,0x0074,0x0069,0x006e,0x0061}, FALSE},
#endif

#ifdef IM_SLOVENIAN_SUPPORT  //斯诺文尼亚
    {{0x0053,0x006c,0x006f,0x0076,0x0065,0x006e,0x0161,0x010d,0x0069,0x006e,0x0061}, TRUE},
//#else
//    {{0x0053,0x006c,0x006f,0x0076,0x0065,0x006e,0x0161,0x010d,0x0069,0x006e,0x0061}, FALSE},
#endif

#ifdef IM_ROMANIAN_SUPPORT  //罗马尼亚
    {{0x0052,0x006f,0x006d,0x00e2,0x006e,0x0103}, TRUE},
//#else
//    {{0x0052,0x006f,0x006d,0x00e2,0x006e,0x0103}, FALSE},
 #endif

#ifdef IM_TELUGU_SUPPORT
    {{0xc24,0xc46,0xc32,0xc41,0xc17,0xc41}, TRUE},
//#else
//    {{0xc24,0xc46,0xc32,0xc41,0xc17,0xc41}, FALSE},
#endif

#ifdef IM_MARATHI_SUPPORT
    {{0x92e,0x930,0x93e,0x920,0x940}, TRUE},
//#else
//    {{0x92e,0x930,0x93e,0x920,0x940}, FALSE},
#endif

#ifdef IM_TAMIL_SUPPORT
    {{0xba4,0xbae,0xbbf,0xbb4,0xbcd}, TRUE},
//#else
//    {{0xba4,0xbae,0xbbf,0xbb4,0xbcd}, FALSE},
#endif

#ifdef IM_GUJARATI_SUPPORT
    {{0xa97,0xac1,0xa9c,0xab0,0xabe,0xaa4,0xac0}, TRUE},
//#else
//    {{0xa97,0xac1,0xa9c,0xab0,0xabe,0xaa4,0xac0}, FALSE},
#endif

#ifdef IM_KANNADA_SUPPORT
    {{0xc95,0xca8,0xccd,0xca8,0xca1}, TRUE},
//#else
//    {{0xc95,0xca8,0xccd,0xca8,0xca1}, FALSE},
#endif

#ifdef IM_MALAYALAM_SUPPORT
    {{0xd2e,0xd32,0xd2f,0xd3e,0xd33,0xd02}, TRUE},
//#else
//    {{0xd2e,0xd32,0xd2f,0xd3e,0xd33,0xd02}, FALSE},
#endif

#ifdef IM_ORIYA_SUPPORT
    {{0xb13,0xb21,0xb3c,0xb3f,0xb06}, TRUE},
//#else
//    {{0xb13,0xb21,0xb3c,0xb3f,0xb06}, FALSE},
#endif

#ifdef IM_PUNJABI_SUPPORT
    {{0xa2a,0xa70,0xa1c,0xa3e,0xa2c,0xa40}, TRUE},
//#else
//    {{0xa2a,0xa70,0xa1c,0xa3e,0xa2c,0xa40}, FALSE},
#endif

#ifdef IM_AFRIKAANS_SUPPORT
    {{0x41,0x66,0x72,0x69,0x6b,0x61,0x61,0x6e,0x73}, TRUE},
//#else
//    {{0x41,0x66,0x72,0x69,0x6b,0x61,0x61,0x6e,0x73}, FALSE},
#endif

#ifdef IM_ALBANIAN_SUPPORT
    {{0x53,0x68,0x71,0x69,0x70}, TRUE},
//#else
//    {{0x53,0x68,0x71,0x69,0x70}, FALSE},
#endif

#ifdef IM_ARMENIAN_SUPPORT
    {{0x540,0x561,0x575,0x565,0x580,0x567,0x576}, TRUE},
//#else
//    {{0x540,0x561,0x575,0x565,0x580,0x567,0x576}, FALSE},
#endif

#ifdef IM_AZERBAIJANI_SUPPORT
    {{0x41,0x7a,0x259,0x72,0x62,0x61,0x79,0x63,0x61,0x6e,0x20,0x64,0x69,0x6c,0x69}, TRUE},
//#else
//    {{0x41,0x7a,0x259,0x72,0x62,0x61,0x79,0x63,0x61,0x6e,0x20,0x64,0x69,0x6c,0x69}, FALSE},
#endif

#ifdef IM_BASQUE_SUPPORT
    {{0x45,0x75,0x73,0x6b,0x61,0x72,0x61}, TRUE},
//#else
//    {{0x45,0x75,0x73,0x6b,0x61,0x72,0x61}, FALSE},
#endif

#ifdef IM_BULGARIAN_SUPPORT
    {{0x411,0x44a,0x43b,0x433,0x430,0x440,0x441,0x43a,0x438,0x20,0x435,0x437,0x438,0x43a}, TRUE},
//#else
//    {{0x411,0x44a,0x43b,0x433,0x430,0x440,0x441,0x43a,0x438,0x20,0x435,0x437,0x438,0x43a}, FALSE},
#endif

#ifdef IM_CATALAN_SUPPORT
    {{0x43,0x61,0x74,0x61,0x6c,0xe0}, TRUE},
//#else
//    {{0x43,0x61,0x74,0x61,0x6c,0xe0}, FALSE},
#endif

#ifdef IM_CROATIAN_SUPPORT
    {{0x68,0x72,0x76,0x61,0x74,0x73,0x6b,0x69}, TRUE},
//#else
//    {{0x68,0x72,0x76,0x61,0x74,0x73,0x6b,0x69}, FALSE},
#endif

#ifdef IM_DANISH_SUPPORT
    {{0x64,0x61,0x6e,0x73,0x6b}, TRUE},
//#else
//    {{0x64,0x61,0x6e,0x73,0x6b}, FALSE},
#endif

#ifdef IM_DUTCH_SUPPORT
    {{0x4e,0x65,0x64,0x65,0x72,0x6c,0x61,0x6e,0x64,0x73}, TRUE},
//#else
//    {{0x4e,0x65,0x64,0x65,0x72,0x6c,0x61,0x6e,0x64,0x73}, FALSE},
#endif

#ifdef IM_ESTONIAN_SUPPORT
    {{0x65,0x65,0x73,0x74,0x69,0x20,0x6b,0x65,0x65,0x6c}, TRUE},
//#else
//    {{0x65,0x65,0x73,0x74,0x69,0x20,0x6b,0x65,0x65,0x6c}, FALSE},
#endif

#ifdef IM_FILIPINO_SUPPORT
    {{0x46,0x69,0x6c,0x69,0x70,0x69,0x6e,0x6f}, TRUE},
//#else
//    {{0x46,0x69,0x6c,0x69,0x70,0x69,0x6e,0x6f}, FALSE},
#endif

#ifdef IM_FINNISH_SUPPORT
    {{0x73,0x75,0x6f,0x6d,0x69}, TRUE},
//#else
//    {{0x73,0x75,0x6f,0x6d,0x69}, FALSE},
#endif

#ifdef IM_GALICIAN_SUPPORT
    {{0x47,0x61,0x6c,0x65,0x67,0x6f}, TRUE},
//#else
//    {{0x47,0x61,0x6c,0x65,0x67,0x6f}, FALSE},
#endif

#ifdef IM_GEORGIAN_SUPPORT
    {{0x10e5,0x10d0,0x10e0,0x10d7,0x10e3,0x10da,0x10d8,0x20,0x10d4,0x10dc,0x10d0}, TRUE},
//#else
//    {{0x10e5,0x10d0,0x10e0,0x10d7,0x10e3,0x10da,0x10d8,0x20,0x10d4,0x10dc,0x10d0}, FALSE},
#endif

#ifdef IM_HAUSA_SUPPORT
    {{0x647,0x64e,0x648,0x652,0x633,0x64e}, TRUE},
//#else
//    {{0x647,0x64e,0x648,0x652,0x633,0x64e}, FALSE},
#endif

#ifdef IM_ICELANDIC_SUPPORT
    {{0xcd,0x73,0x6c,0x65,0x6e,0x73,0x6b,0x61}, TRUE},
//#else
//    {{0xcd,0x73,0x6c,0x65,0x6e,0x73,0x6b,0x61}, FALSE},
#endif

#ifdef IM_IGBO_SUPPORT
    {{0x41,0x73,0x1ee5,0x73,0x1ee5,0x20,0x49,0x67,0x62,0x6f}, TRUE},
//#else
//    {{0x41,0x73,0x1ee5,0x73,0x1ee5,0x20,0x49,0x67,0x62,0x6f}, FALSE},
#endif

#ifdef IM_IRISH_SUPPORT
    {{0x47,0x61,0x65,0x69,0x6c,0x67,0x65}, TRUE},
//#else
//    {{0x47,0x61,0x65,0x69,0x6c,0x67,0x65}, FALSE},
#endif

#ifdef IM_KAZAKH_SUPPORT
    {{0x51,0x61,0x7a,0x61,0x71,0x20,0x74,0x69,0x6c,0x69}, TRUE},
//#else
//    {{0x51,0x61,0x7a,0x61,0x71,0x20,0x74,0x69,0x6c,0x69}, FALSE},
#endif

#ifdef IM_LATVIAN_SUPPORT
    {{0x6c,0x61,0x74,0x76,0x69,0x65,0x161,0x75,0x20,0x76,0x61,0x6c,0x6f,0x64,0x61}, TRUE},
//#else
//    {{0x6c,0x61,0x74,0x76,0x69,0x65,0x161,0x75,0x20,0x76,0x61,0x6c,0x6f,0x64,0x61}, FALSE},
#endif

#ifdef IM_LITHUANIAN_SUPPORT
    {{0x4c,0x69,0x65,0x74,0x75,0x76,0x69,0x173,0x20,0x6b,0x61,0x6c,0x62,0x61}, TRUE},
//#else
//    {{0x4c,0x69,0x65,0x74,0x75,0x76,0x69,0x173,0x20,0x6b,0x61,0x6c,0x62,0x61}, FALSE},
#endif

#ifdef IM_MACEDONIAN_SUPPORT
    {{0x41c,0x430,0x43a,0x435,0x434,0x43e,0x43d,0x441,0x43a,0x438,0x20,0x458,0x430,0x437,0x438,0x43a}, TRUE},
//#else
//    {{0x41c,0x430,0x43a,0x435,0x434,0x43e,0x43d,0x441,0x43a,0x438,0x20,0x458,0x430,0x437,0x438,0x43a}, FALSE},
#endif

#ifdef IM_MOLDOVAN_SUPPORT
    {{0x72,0x6f,0x6d,0xe2,0x6e,0x103}, TRUE},
//#else
//    {{0x72,0x6f,0x6d,0xe2,0x6e,0x103}, FALSE},
#endif

#ifdef IM_NORWEGIAN_SUPPORT
    {{0x6e,0x6f,0x72,0x73,0x6b}, TRUE},
//#else
//    {{0x6e,0x6f,0x72,0x73,0x6b}, FALSE},
#endif

#ifdef IM_POLISH_SUPPORT
    {{0x6a,0x119,0x7a,0x79,0x6b,0x20,0x70,0x6f,0x6c,0x73,0x6b,0x69}, TRUE},
//#else
//    {{0x6a,0x119,0x7a,0x79,0x6b,0x20,0x70,0x6f,0x6c,0x73,0x6b,0x69}, FALSE},
#endif

#ifdef IM_SERBIAN_SUPPORT
    {{0x73,0x72,0x70,0x73,0x6b,0x69}, TRUE},
//#else
//    {{0x73,0x72,0x70,0x73,0x6b,0x69}, FALSE},
#endif

#ifdef IM_SESOTHO_SUPPORT
    {{0x53,0x65,0x73,0x6f,0x74,0x68,0x6f}, TRUE},
//#else
//    {{0x53,0x65,0x73,0x6f,0x74,0x68,0x6f}, FALSE},
#endif

#ifdef IM_SLOVAK_SUPPORT
    {{0x73,0x6c,0x6f,0x76,0x65,0x6e,0x10d,0x69,0x6e,0x61}, TRUE},
//#else
//    {{0x73,0x6c,0x6f,0x76,0x65,0x6e,0x10d,0x69,0x6e,0x61}, FALSE},
#endif

#ifdef IM_SWEDISH_SUPPORT
    {{0x73,0x76,0x65,0x6e,0x73,0x6b,0x61}, TRUE},
//#else
//    {{0x73,0x76,0x65,0x6e,0x73,0x6b,0x61}, FALSE},
#endif

#ifdef IM_UKRAINIAN_SUPPORT
    {{0x443,0x43a,0x440,0x430,0x457,0x43d,0x441,0x44c,0x43a,0x430}, TRUE},
//#else
//    {{0x443,0x43a,0x440,0x430,0x457,0x43d,0x441,0x44c,0x43a,0x430}, FALSE},
#endif

#ifdef IM_YORUBA_SUPPORT
    {{0xe8,0x64,0xe8,0x20,0x59,0x6f,0x72,0xf9,0x62,0xe1}, TRUE},
//#else
//    {{0xe8,0x64,0xe8,0x20,0x59,0x6f,0x72,0xf9,0x62,0xe1}, FALSE},
#endif

//个人不知该需求应该是怎样的，暂且用英文替代	
//科萨语
#ifdef IM_XHOSA_SUPPORT
    {{'x', 'h', 'o', 's', 'a', 0}, TRUE},
//#else
//    {{'x', 'h', 'o', 's', 'a', 0}, FALSE},
#endif

//祖鲁语
#ifdef IM_ZULU_SUPPORT
    {{'z', 'u', 'l', 'u', 0}, TRUE},
//#else
//    {{'z', 'u', 'l', 'u', 0}, FALSE},
#endif

//(印度语支的)阿萨姆语
#ifdef IM_ASSAMESE_SUPPORT
    {{'a', 's', 's', 'a', 'm', 'e', 's', 'e', 0}, TRUE},
//#else
//    {{'a', 's', 's', 'a', 'm', 'e', 's', 'e', 0}, FALSE},
#endif

//斯瓦西里	
#ifdef IM_SWAHILI_SUPPORT
    {{'K', 'i', 's', 'w', 'a', 'h', 'i', 'l', 'i', 0}, TRUE},
//#else
//    {{'K', 'i', 's', 'w', 'a', 'h', 'i', 'l', 'i', 0}, FALSE},
#endif

#ifdef IM_MYANMAR_SUPPORT
    {{L"Myanmar"}, TRUE},
//#else
//    {{L"Myanmar"}, FALSE},
#endif  

#ifdef IM_AMHARIC_SUPPORT
    {{0x12A0, 0x121B, 0x122D, 0x129B}, TRUE},
//#else
//    {{0x12A0, 0x121B, 0x122D, 0x129B}, FALSE},
#endif    

#ifdef IM_KHMER_SUPPORT
    {{L"Khmer"}, TRUE},
//#else
//    {{L"Khmer"}, FALSE},
#endif

#ifdef IM_LAO_SUPPORT
    {{L"Lao"}, TRUE},
//#else
//    {{L"Lao"}, FALSE},
#endif

#ifdef IM_UYGHUR_SUPPORT
    {{L"Uyghur"}, TRUE},
//#else
//    {{L"Uyghur"}, FALSE},
#endif          

#ifdef IM_TIBETAN_SUPPORT   
	{{L"Tibetan"},TRUE},
//#else
//	{{L"Tibetan"},FALSE},
#endif

#ifdef IM_SINHALESE_SUPPORT
    {{L"Sinhalese"}, TRUE},
//#else
//    {{L"Sinhalese"}, FALSE},
#endif          

    {0 , 0} /*lint !e651*/
};
/*lint -save -e785 */

#ifdef MMI_SIM_LANGUAGE_SUPPORT
//单字节映射表
LOCAL const MMISET_LANG_MAP_SIM_BYTE_T s_lang_map_sim[ ]=
{
#ifdef IM_GERMAN_SUPPORT
    {MMISET_LANGUAGE_GERMAN, 0x00},
#endif
    {MMISET_LANGUAGE_ENGLISH, 0x01},
#ifdef IM_ITALIAN_SUPPORT
    {MMISET_LANGUAGE_ITALIAN,  0x02},    
#endif
#ifdef IM_FRENCH_SUPPORT
    {MMISET_LANGUAGE_FRENCH,  0x03},
#endif
#ifdef IM_SPANISH_SUPPORT
    {MMISET_LANGUAGE_SPANISH, 0x04},
#endif
#ifdef IM_DUTCH_SUPPORT
    {MMISET_LANGUAGE_DUTCH,  0x05},
#endif
#ifdef IM_SWEDISH_SUPPORT
    {MMISET_LANGUAGE_SWEDISH, 0x06},
#endif
#ifdef IM_DANISH_SUPPORT
    {MMISET_LANGUAGE_DANISH, 0x07},
#endif
#ifdef IM_PORTUGUESE_SUPPORT
    {MMISET_LANGUAGE_PORTUGUESE, 0x08},
#endif
#ifdef IM_FINNISH_SUPPORT
    {MMISET_LANGUAGE_FINNISH,  0x09},
#endif
#ifdef IM_NORWEGIAN_SUPPORT
    {MMISET_LANGUAGE_NORWEGIAN, 0x0a},
#endif
#ifdef IM_GREEK_SUPPORT
    {MMISET_LANGUAGE_GREEK,  0x0b},
#endif
#ifdef IM_TURKISH_SUPPORT
    {MMISET_LANGUAGE_TURKISH, 0x0c},
#endif
#ifdef IM_HUNGARIAN_SUPPORT
    {MMISET_LANGUAGE_HUNGARIAN, 0x0d},    
#endif
#ifdef IM_POLISH_SUPPORT
    {MMISET_LANGUAGE_POLISH,  0x0e},
#endif
#ifdef IM_CZECH_SUPPORT
    {MMISET_LANGUAGE_CZECH, 0x20 },
#endif
#ifdef IM_HEBREW_SUPPORT
    {MMISET_LANGUAGE_HEBREW, 0x21 },
#endif
#ifdef IM_ARABIC_SUPPORT
    {MMISET_LANGUAGE_ARABIC, 0x22 },
#endif
#ifdef IM_RUSSIAN_SUPPORT
    {MMISET_LANGUAGE_RUSSIAN, 0x23 },    
#endif
    //{MMISET_LANGUAGE_ICELANDIC, 0x24 },
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
    {MMISET_LANGUAGE_SIMP_CHINESE, 0x48 }, //没有做简体中文和繁体中文的区分，协议没有定??
#endif

};

//双字节映射表
LOCAL const MMISET_LANG_MAP_SIM_EXT_BYTE_T s_lang_map_sim_ext[ ] =
{
    {MMISET_LANGUAGE_ENGLISH, {'e', 'n'}},
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
    {MMISET_LANGUAGE_SIMP_CHINESE, {'z', 'h'}},
#endif
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
    {MMISET_LANGUAGE_TRAD_CHINESE, {'z', 'h'}},
#endif
//    {MMISET_LANGUAGE_ENGLISH, {'u', 's'}},
//    {MMISET_LANGUAGE_SIMP_CHINESE, {'c', 'n'}},
//    {MMISET_LANGUAGE_TRAD_CHINESE, {'t', 'w'}},
//    {MMISET_LANGUAGE_TRAD_CHINESE, {'h', 'k'}},
#ifdef IM_ARABIC_SUPPORT
    {MMISET_LANGUAGE_ARABIC, {'a', 'r'}},
#endif
#ifdef IM_FRENCH_SUPPORT
    {MMISET_LANGUAGE_FRENCH, {'f', 'r'}},
#endif
#ifdef IM_HINDI_SUPPORT
    {MMISET_LANGUAGE_HINDI, {'h', 'i'}},
#endif
#ifdef IM_HUNGARIAN_SUPPORT
    {MMISET_LANGUAGE_HUNGARIAN, {'h', 'u'}},
#endif
#ifdef IM_INDONESIAN_SUPPORT
    {MMISET_LANGUAGE_INDONESIAN, {'i', 'd'}},
#endif
#ifdef IM_MALAY_SUPPORT
    {MMISET_LANGUAGE_MALAY, {'m', 's'}},
#endif
#ifdef IM_PORTUGUESE_SUPPORT
    {MMISET_LANGUAGE_PORTUGUESE, {'p', 't'}},
#endif
#ifdef IM_RUSSIAN_SUPPORT
    {MMISET_LANGUAGE_RUSSIAN, {'r', 'u'}},
#endif
#ifdef IM_SPANISH_SUPPORT
    {MMISET_LANGUAGE_SPANISH, {'e', 's'}},
#endif
#ifdef IM_TAGALOG_SUPPORT
    {MMISET_LANGUAGE_TAGALOG, {'t', 'l'}},
#endif
#ifdef IM_THAI_SUPPORT
    {MMISET_LANGUAGE_THAI, {'t', 'h'}},
#endif
#ifdef IM_VIETNAMESE_SUPPORT
    {MMISET_LANGUAGE_VIETNAMESE, {'v', 'i'}},
#endif
#ifdef IM_URDU_SUPPORT
    {MMISET_LANGUAGE_URDU, {'u', 'r'}},
#endif
#ifdef IM_ITALIAN_SUPPORT
    {MMISET_LANGUAGE_ITALIAN, {'i', 't'}},
#endif
#ifdef IM_PERSIAN_SUPPORT
    {MMISET_LANGUAGE_PERSIAN, {'f', 'a'}},
#endif
#ifdef IM_TURKISH_SUPPORT
    {MMISET_LANGUAGE_TURKISH, {'t', 'r'}},
#endif
#ifdef IM_GERMAN_SUPPORT
    {MMISET_LANGUAGE_GERMAN, {'d', 'e'}},
#endif
#ifdef IM_GREEK_SUPPORT
    {MMISET_LANGUAGE_GREEK, {'e', 'l'}},
#endif
#ifdef IM_HEBREW_SUPPORT
    {MMISET_LANGUAGE_HEBREW, {'h', 'e'}},
#endif
#ifdef IM_BENGALI_SUPPORT
    {MMISET_LANGUAGE_BENGALI, {'b', 'e'}},
#endif
#ifdef IM_CZECH_SUPPORT
    {MMISET_LANGUAGE_CZECH, {'c', 's'}},
#endif
#ifdef IM_SLOVENIAN_SUPPORT
    {MMISET_LANGUAGE_SLOVENIAN, {'s', 'l'}},
#endif
#ifdef IM_ROMANIAN_SUPPORT
    {MMISET_LANGUAGE_ROMANIAN, {'r', 'o'}},
#endif
#ifdef IM_TELUGU_SUPPORT
    {MMISET_LANGUAGE_TELUGU, {'t', 'e'}},
#endif
#ifdef IM_MARATHI_SUPPORT
    {MMISET_LANGUAGE_MARATHI, {'m', 'r'}},
#endif
#ifdef IM_TAMIL_SUPPORT
    {MMISET_LANGUAGE_TAMIL, {'t', 'a'}},
#endif
#ifdef IM_GUJARATI_SUPPORT
    {MMISET_LANGUAGE_GUJARATI, {'g', 'u'}},
#endif
#ifdef IM_KANNADA_SUPPORT
    {MMISET_LANGUAGE_KANNADA, {'k', 'n'}},
#endif
#ifdef IM_MALAYALAM_SUPPORT
    {MMISET_LANGUAGE_MALAYALAM, {'m', 'l'}},
#endif
#ifdef IM_ORIYA_SUPPORT
    {MMISET_LANGUAGE_ORIYA, {'o', 'r'}},
#endif
#ifdef IM_PUNJABI_SUPPORT
    {MMISET_LANGUAGE_PUNJABI, {'p', 'a'}},
#endif
//    {MMISET_LANGUAGE_AFRIKAANS, {'a', 'f'}},
//    {MMISET_LANGUAGE_ALBANIAN, {'s', 'q'}},
#ifdef IM_ARMENIAN_SUPPORT
    {MMISET_LANGUAGE_ARMENIAN, {'h', 'y'}},
#endif
//    {MMISET_LANGUAGE_AZERBAIJANI, {'a', 'z'}},
//    {MMISET_LANGUAGE_BASQUE, {'e', 'u'}},
#ifdef IM_BULGARIAN_SUPPORT
    {MMISET_LANGUAGE_BULGARIAN, {'b', 'g'}},
#endif
//    {MMISET_LANGUAGE_CATALAN, {'c', 'a'}},
//    {MMISET_LANGUAGE_CROATIAN, {'h', 'r'}},
#ifdef IM_DANISH_SUPPORT
    {MMISET_LANGUAGE_DANISH, {'d', 'a'}},
#endif
#ifdef IM_DUTCH_SUPPORT
    {MMISET_LANGUAGE_DUTCH, {'n', 'l'}},
#endif
//    {MMISET_LANGUAGE_ESTONIAN, {'e', 't'}},
//    {MMISET_LANGUAGE_FILIPINO, {'f', 'p'}},//"fil-PH"
#ifdef IM_FINNISH_SUPPORT
    {MMISET_LANGUAGE_FINNISH, {'f', 'i'}},
#endif
//    {MMISET_LANGUAGE_GALICIAN, {'g', 'l'}},
//    {MMISET_LANGUAGE_GEORGIAN, {'k', 'a'}},
//    {MMISET_LANGUAGE_HAUSA, {'h', 'a'}},
//    {MMISET_LANGUAGE_ICELANDIC, {'i', 's'}},
//    {MMISET_LANGUAGE_IGBO, {'i', 'g'}},
//    {MMISET_LANGUAGE_IRISH, {'g', 'a'}},
//    {MMISET_LANGUAGE_KAZAKH, {'k', 'k'}},
//    {MMISET_LANGUAGE_LATVIAN, {'l', 'v'}},
#ifdef IM_LITHUANIAN_SUPPORT
    {MMISET_LANGUAGE_LITHUANIAN, {'l', 't'}},
#endif
//    {MMISET_LANGUAGE_MACEDONIAN, {'m', 'k'}},
//    {MMISET_LANGUAGE_MOLDOVAN, {'m', 'o'}},
#ifdef IM_NORWEGIAN_SUPPORT
    {MMISET_LANGUAGE_NORWEGIAN, {'n', 'b'}},//"no"
#endif
#ifdef IM_POLISH_SUPPORT
    {MMISET_LANGUAGE_POLISH, {'p', 'l'}},
#endif
//    {MMISET_LANGUAGE_SERBIAN, {'s', 'r'}},
//    {MMISET_LANGUAGE_SESOTHO, {'s', 't'}},
#ifdef IM_SLOVAK_SUPPORT
    {MMISET_LANGUAGE_SLOVAK, {'s', 'k'}},
#endif
#ifdef IM_SWEDISH_SUPPORT
    {MMISET_LANGUAGE_SWEDISH, {'s', 'v'}},
#endif
#ifdef IM_UKRAINIAN_SUPPORT
    {MMISET_LANGUAGE_UKRAINIAN, {'u', 'k'}},
#endif
//    {MMISET_LANGUAGE_YORUBA, {'y', 'o'}},
//    {MMISET_LANGUAGE_XHOSA, {'x', 'h'}},
//    {MMISET_LANGUAGE_ZULU, {'z', 'u'}},
//    {MMISET_LANGUAGE_ASSAMESE, {'a', 's'}},
#ifdef IM_SWAHILI_SUPPORT
    {MMISET_LANGUAGE_SWAHILI, {'s', 'w'}},
#endif
//    {MMISET_LANGUAGE_HINGLISH, {'e', 'n'}},
#ifdef IM_KHMER_SUPPORT
    {MMISET_LANGUAGE_KHMER, {'k', 'm'}},
#endif
#ifdef IM_MYANMAR_SUPPORT
    {MMISET_LANGUAGE_MYANMAR, {'m', 'y'}},
#endif
#ifdef IM_LAO_SUPPORT
    {MMISET_LANGUAGE_LAO, {'l', 'o'}},
#endif
#ifdef IM_AMHARIC_SUPPORT
    {MMISET_LANGUAGE_AMHARIC, {'a', 'm'}},
#endif
#ifdef IM_UYGHUR_SUPPORT
    {MMISET_LANGUAGE_UYGHUR, {'u', 'g'}},
#endif
#ifdef IM_HAUSA_SUPPORT
    {MMISET_LANGUAGE_HAUSA, {'h', 'a'}}
#endif
};
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern  uint8 g_ksp_Lang;     //当前的语言类型
#ifndef NANDBOOT_SUPPORT  //在nor的efs中没有这个接口。
extern NVITEM_ERROR_E NVITEM_FlushAll( void );
#endif
#ifdef MMI_SIM_LANGUAGE_SUPPORT
#ifdef SSL_SUPPORT
extern int32 SCI_MEMCMP(void * _BUF1, void * _BUF2, uint32 _SIZE);
#else
#define SCI_MEMCMP memcmp
#endif
#endif
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
MMI_GMMREG_RAT_E *g_rat_list_ptr = PNULL;//rat 列表//@$discuss,是否需要加宏？
MN_PHONE_PLMN_DETAIL_T  *g_plmn_detail_list_ptr = PNULL;//选择网络的列表
#ifdef MOTION_SENSOR_TYPE
LOCAL uint32 s_open_sensor_flag=0x0028;//cr227614 maryxiao
#endif
#if defined(MMI_BLACKLIST_SUPPORT)
MMISET_BLACKLIST_T *g_blacklist_ptr = PNULL;
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
static  MMISET_KEYBOARD_LIGHT_SET_T  s_keyboard_light_info = {0}; //按键背关灯信息
LOCAL BOOLEAN s_is_fly_mode_on = FALSE;
LOCAL MMISET_LANG_VALID_INFO_T   *s_lang_valid_info = PNULL;

#ifdef MMI_SIM_LANGUAGE_SUPPORT
//记录存储在NV Item 中的语言类型: 包含auto
//主要处理setting list item中auto的正确显示和配置。
//获取当前系统使用的语言:  MMIAPISET_GetLanguageType()
//获取setting项中，当前系统语言项设置的类型，使用MMIAPISET_GetNVLanguageType()
LOCAL uint8 s_nv_lang_setting = 0;  
#endif

LOCAL MMISET_PRIVACY_APP_ENTRY_CALLBACK_PFUNC g_app_entry_call_back = PNULL;//app传入的入口函数
LOCAL MMISET_PRIVACY_PROTECT_TYPE_E g_validate_privacy_type = MMISET_PROTECT_MAX;
LOCAL BOOLEAN g_is_need_privacy_validate = TRUE;//用户只需要进行一次成功的隐私保护密码验证，直至锁屏。privacy validate is needed only once before the phone is locked.

#ifdef LCD_SLIDE_SUPPORT
/*****************************************************************************/
// 	Description : reset slide setting 
//	Global resource dependence : none
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void ResetSlideSetting(void);
#endif

/*****************************************************************************/
//  Description : check the day of date if is valid
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//LOCAL BOOLEAN CheckDayOfDateIsValid(
//                                    MMISET_DATE_T            date    //the date
//                                    );


/*****************************************************************************/
//  Description : init other,include language setting,network select setting etc.
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void OtherInit(void);

/*****************************************************************************/
//  Description : reset display setting 
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL void ResetDisplaySetting(void);

/*****************************************************************************/
//  Description : init privacy protect password
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL void InitPrivacyProtectPwd(
                                 void
                                 );


/*****************************************************************************/
//  Description : 添加网络列表到text list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AppendPlmnListItem(
                              MMI_CTRL_ID_T             ctrl_id,
                              MN_PHONE_PLMN_INFO_LIST_T    *plmn_detail_list,
                              MN_DUAL_SYS_E dual_sys
                              );
/*****************************************************************************/
//  Description : Set global langguage reference settings
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetGlobalLanguageRef(MMISET_LANGUAGE_TYPE_E language_type);
/*****************************************************************************/
//  Description : reset pic wallpaper setting
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL void ResetWallpaperSetting(void);
#ifndef WIN32
/*****************************************************************************/
//  Description : get mul-language nv config info 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void  GetMulNVConfigInfo(MMI_MULCONFIG_INFO_T * config_info);
#endif 
/*****************************************************************************/
//  Description : restore data and time factory setting
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void RestoreDataTimeFactorySetting(void);

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : init blacklist parm
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void InitBlacklistParm(void);

/*****************************************************************************/
//  Description : find num in blacklist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindNumInBlacklist(MMIPB_BCD_NUMBER_T *orginal_addr_ptr);

/*****************************************************************************/
//  Description : restore blacklist setting
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void RestoreBlacklistSetting(BOOLEAN is_clean_blacklist_value);
#endif

/*****************************************************************************/
//  Description : alloc and init language
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void MMISET_LanguageInit(void);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init all setting 
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_AllInit(void)
{
    // environment init 
    MMISET_EnvSetInit();
#ifdef ZDT_PLAY_RING_CUSTOM
    SIO_CustRing_Data_Init();
#endif

   //  MMISET_RingInit();
    
    //init display set
    MMISET_DisplayInit();
    
    //init call setting
    MMISET_CallInit();
    
    //init other,include language setting,network select setting etc.
    OtherInit();
#if defined MMI_WALLPAPER_SUPPORT
	MMIAPIFMM_InitWallPaper();
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    MMIAPISETVWP_Init();
#endif
    
    MMISET_LanguageInit();

#ifdef MMI_INDIAN_SOS_SUPPORT
    MMISET_InitSOSContextInfo();
#endif
}

/*****************************************************************************/
//  Description : init privacy protect password
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL void InitPrivacyProtectPwd(
                                 void
                                 )
{
    uint8                           pwd_num[MMISET_PRIVACY_PROTECT_PWD_MAX_LEN] =   {0};
    MMISET_PRIVACY_PROTECT_PWD_T    code_info                                   =   {0};
    MN_RETURN_RESULT_E              return_value                                =   MN_RETURN_FAILURE;
    
    SCI_MEMSET(&code_info, 0 , sizeof(MMISET_PRIVACY_PROTECT_PWD_T));
    
    //get privacy protect password info
    MMINV_READ(MMINV_SET_PRIVACY_PROTECT_PWD_INFO,&code_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        sprintf((char*)pwd_num, "%s" , MMISET_PRIVACY_PROTECT_INIT_PWD);
        code_info.pwd_len = strlen((char*)pwd_num);
       // SCI_ASSERT( code_info.pwd_len < MMISET_PRIVACY_PROTECT_PWD_MAX_LEN);
	   if(code_info.pwd_len >= MMISET_PRIVACY_PROTECT_PWD_MAX_LEN)
	   {
	   	//SCI_TRACE_LOW:"mmisetting_assert InitPrivacyProtectPwd %d"
	   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_953_112_2_18_2_51_8_181,(uint8*)"d", code_info.pwd_len);
		return;
	   }
    
        SCI_MEMCPY(code_info.pwd, pwd_num, code_info.pwd_len);  
        MMINV_WRITE(MMINV_SET_PRIVACY_PROTECT_PWD_INFO,&code_info);
    }
}

/*****************************************************************************/
//  Description : init phone password
//  Global resource dependence : none
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC void InitPhonePwd(MMISET_POWER_ON_PWD_T* phone_code )
{
    MN_RETURN_RESULT_E              return_value                                =   MN_RETURN_FAILURE;
    uint8                           pwd_num[MMISET_PRIVACY_PROTECT_PWD_MAX_LEN] =   {0};
    
    SCI_MEMSET(phone_code, 0 , sizeof(MMISET_POWER_ON_PWD_T));
    MMINV_READ(MMINV_SET_POWER_ON_PWD_INFO,phone_code,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(phone_code,0,sizeof(MMISET_POWER_ON_PWD_T));
        sprintf((char*)pwd_num, "%s" , MMISET_PRIVACY_PROTECT_INIT_PWD);
        phone_code->pwd_len = strlen((char*)pwd_num);
       // SCI_ASSERT(phone_code->pwd_len < MMISET_PRIVACY_PROTECT_PWD_MAX_LEN);
	if(phone_code->pwd_len >= MMISET_PRIVACY_PROTECT_PWD_MAX_LEN)
	{
	    //SCI_TRACE_LOW:"mmisetting_assert InitPhonePwd %d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_981_112_2_18_2_51_8_182,(uint8*)"d", phone_code->pwd_len);
	    phone_code->pwd_len= MMISET_PRIVACY_PROTECT_PWD_MIN_LEN;
	}
        SCI_MEMCPY(phone_code->pwd, pwd_num, phone_code->pwd_len);  
        MMINV_WRITE(MMINV_SET_POWER_ON_PWD_INFO,phone_code);
    }
}

/*****************************************************************************/
//  Description : set nv by opencloselist current select
//  Global resource dependence : 
//  Author: jibin
//  Note: 通过funcSet设置nv当前开启关闭状态
/*****************************************************************************/
PUBLIC void MMIAPISET_SetByOpenCloseList(
                                         MMI_CTRL_ID_T ctrl_id,
                                         MN_DUAL_SYS_E dual_sys,
                                         void (*funcSet)(BOOLEAN)
                                         )
{
    uint16      cur_selection   =   0;
    BOOLEAN     is_open         =   FALSE;
    
    //get the selected item
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
    
   // SCI_ASSERT((MMISET_OPEN_CLOSE_ITEM > cur_selection));
   if(MMISET_OPEN_CLOSE_ITEM <= cur_selection)
   {
   	//SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_SetByOpenCloseList cur_selection %d"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_1008_112_2_18_2_51_8_183,(uint8*)"d", cur_selection);
	return;
   }
    
    
    switch (cur_selection)
    {
    case 0:
        is_open = TRUE;
        break;
        
    case 1:
        is_open = FALSE;
        break;
        
    default:
        break;
    }
    funcSet(is_open);
}


/*****************************************************************************/
//  Description : 添加开启关闭的单选list items
//  Global resource dependence : 
//  Author: jibin
//  Note: 通过funcGet从nv得到当前开启关闭状态
/*****************************************************************************/ 
PUBLIC void MMISET_AppendOpenCloseListItem(
                                           MMI_CTRL_ID_T ctrl_id,
                                           BOOLEAN (*funcGet)(void)
                                           )
{
    uint16      cur_selection   =   0;
    
    //add items to listbox
    MMIAPISET_AppendListItemByTextId( TXT_OPEN, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    if (funcGet())
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

/*****************************************************************************/
//  Description : init other,include language setting,network select setting etc.
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void OtherInit(void)
{
    uint16                          i                       =   0;
    MMISET_PRIVACY_PROTECT_T        privacy_protect_info    =   {0};
    MMISET_KEYBOARD_LIGHT_SET_T     keyboard_light_info     =   {0};
    MMISET_POWER_ON_PWD_T           phone_code              =   {0};
    //MMISET_LANGUAGE_TYPE_E          language_type           =   MMISET_EDEFAULT_LANGUAGE; /*lint !e64*/ //default type of language
    MN_RETURN_RESULT_E              return_value            =   MN_RETURN_FAILURE;
    MMISET_ANYKEY_REJECT_SET_T      anykey_reject_mode      =   {0};
    uint32                          move_style              =   MMISET_EDEFAULT_MOVE_STYLE;
    BOOLEAN                         is_fly_mode_set_power_on =  FALSE;
#ifdef MOTION_SENSOR_TYPE
    uint32 							open_sensor_flag=0;
#endif    
    
   #ifdef DPHONE_SUPPORT 
	uint32                          speedcall_delay         =   0;
    #endif	
    
    //init the type of language
    MMISET_GetCurrentLanguageType();
    //init picture, music, movie, ebook and vcard folde name
	//MMIAPIFMM_InitDefaultFoldeName(MMIFMM_FOLDER_TYPE_NORMAL, language_type);
	//MMIAPIFMM_InitDefaultFoldeName(MMIFMM_FOLDER_TYPE_PICTURE, language_type);
	//MMIAPIFMM_InitDefaultFoldeName(MMIFMM_FOLDER_TYPE_MUSIC, language_type);
	//MMIAPIFMM_InitDefaultFoldeName(MMIFMM_FOLDER_TYPE_MOVIE, language_type);
	//MMIAPIFMM_InitDefaultFoldeName(MMIFMM_FOLDER_TYPE_EBOOK, language_type);
	//MMIAPIFMM_InitDefaultFoldeName(MMIFMM_FOLDER_TYPE_VCARD, language_type); 
	
    //Set global language reference
    SetGlobalLanguageRef(g_ksp_Lang);/*lint !e64*/

#ifdef MMI_SIM_LANGUAGE_SUPPORT
    // load new language text resource
    if( MMISET_LANGUAGE_AUTO == s_nv_lang_setting)
    {
        MMI_LoadNewTextRes();
    }
#endif

	//init im
	MMIAPIIM_InitNv();
    
    MMIAPISTK_CnfSimLangSel();//@songbin.zeng cr53531
    
    //init the time of key board light
    MMINV_READ(MMINV_SET_KEYBOARD_LIGHT_INFO,&keyboard_light_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        keyboard_light_info.keyboard_type = MMISET_EDEFAULT_KEYBOARD_LIGHT_TYPE;/*lint !e64*/
        keyboard_light_info.keyboard_light_time = MMISET_EDEFAULT_KEYBOARD_LIGHT_TIME;
        MMINV_WRITE(MMINV_SET_KEYBOARD_LIGHT_INFO,&keyboard_light_info);
    }
    s_keyboard_light_info.keyboard_light_time = keyboard_light_info.keyboard_light_time;
    s_keyboard_light_info.keyboard_type = keyboard_light_info.keyboard_type;
    
    MMINV_READ(MMINV_SET_WINDOW_MOVE_STYLE_INFO, &move_style, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        move_style = 0;
        MMINV_WRITE(MMINV_SET_WINDOW_MOVE_STYLE_INFO, &move_style);
    }
    MMK_SetWinDefaultAnimMoveStyle(move_style);/*lint !e64*/
    //initial global variable
    if (PNULL != g_plmn_detail_list_ptr)
    {
        SCI_FREE(g_plmn_detail_list_ptr);
        g_plmn_detail_list_ptr = PNULL;
    }
    
    //init power on password
    InitPhonePwd(&phone_code);
    
    //init any key and reject mode
    MMINV_READ(MMINV_SET_ANYKEY_REJECT_MODE,&anykey_reject_mode,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        anykey_reject_mode.is_anykey_answer = FALSE;
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
        #ifdef CMCC_UI_STYLE
        anykey_reject_mode.is_sms_reply_open = TRUE;
        #else
        anykey_reject_mode.is_sms_reply_open = FALSE;
        #endif
#endif
        MMINV_WRITE(MMINV_SET_ANYKEY_REJECT_MODE,&anykey_reject_mode);
    }
    
    //init privacy protect mode
    MMINV_READ(MMINV_SET_PRIVACY_PROTECT_INFO,&privacy_protect_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        privacy_protect_info.protected_item_num = 0;
        for ( i = 0; i < MMISET_PROTECT_MAX; i++)
        {
            privacy_protect_info.protected_item[i] = FALSE;
        }       
        MMINV_WRITE(MMINV_SET_PRIVACY_PROTECT_INFO,&privacy_protect_info);
    }
    //init fly mode switch
    MMINV_READ(MMINV_SET_FLY_MODE_SWITCH,&s_is_fly_mode_on,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        s_is_fly_mode_on = FALSE;
        MMINV_WRITE(MMINV_SET_FLY_MODE_SWITCH,&s_is_fly_mode_on);
    }
    ual_tele_radio_set_fly_mode(s_is_fly_mode_on);
    MMINV_READ(MMINV_FLY_MODE_OPEN_PHONE,&is_fly_mode_set_power_on,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_fly_mode_set_power_on = FALSE;
        MMINV_WRITE(MMINV_FLY_MODE_OPEN_PHONE,&is_fly_mode_set_power_on);
    }
    //init privacy protect password
    InitPrivacyProtectPwd();
    
#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
    MMIAPISET_SetFont( PNULL, FALSE, FALSE );
#endif

#ifdef MOTION_SENSOR_TYPE
	MMINV_READ(MMINV_SET_SENSOR_OPEN_FLAG, &open_sensor_flag, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        open_sensor_flag = 0x0028;//maryxiao  cr227614
        MMINV_WRITE(MMINV_SET_SENSOR_OPEN_FLAG, &open_sensor_flag);
    }
	s_open_sensor_flag=open_sensor_flag;
#endif

#ifdef DPHONE_SUPPORT 
	MMINV_READ(MMINV_SET_SPEEDIAL_DELAY_TIME, &speedcall_delay, return_value);

	if (MN_RETURN_SUCCESS != return_value)
	{
		speedcall_delay = 4000;
		MMINV_WRITE(MMINV_SET_SPEEDIAL_DELAY_TIME,&speedcall_delay);
	}
	MMISET_SetSpeedDialDelay(speedcall_delay);
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
    InitBlacklistParm();
#endif
}

PUBLIC void MMIAPISET_SaveDataSimToNV(MN_DUAL_SYS_E dual_sys)
{
         MMINV_WRITE(MMINV_SET_DATA_SIM_SELECT, &dual_sys);
}
PUBLIC uint8 MMIAPISET_GetDataSimFromNV(void)
{
    MN_DUAL_SYS_E sim_num = MN_DUAL_SYS_1;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
   
    MMINV_READ(MMINV_SET_DATA_SIM_SELECT, &sim_num, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
                   sim_num = MN_DUAL_SYS_1;
    }
   
    return sim_num;
}

/*****************************************************************************/
//  Description : set fly mode
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetFlyMode(BOOLEAN is_fly_mode_on)
{  
#ifdef BLUETOOTH_SUPPORT
    static BOOLEAN need_restart_bt = FALSE;
#endif
#if 0 //bug 2197339
#ifdef ENG_SUPPORT    //return WHEN in monkey test mode
    if(MMIENG_IsMonkeyTestMode())
    {
        return;
    }
#endif 
#endif
    if(is_fly_mode_on)
    {
#ifndef BBM_ROLE_SUPPORT
#ifdef BLUETOOTH_SUPPORT
        if(MMIAPIBT_IsOpenFileWin())
        {
            MMIPUB_OpenAlertWarningWin(TXT_BT_CLOSE_WARNING);
            return;
        } 
        if(MMIAPIBT_IsBtOn())
        {
#ifdef BT_DIALER_SUPPORT
            if(MMIAPIBT_IsConnectDevice())// if devices is connected, close bt here will assert randomly 912234
            {
                MMIPUB_OpenAlertWarningWin(TXT_BT_DEVICE_CONNECT_ALERT);
                return;
            }
	   else
#endif
	   {
                MMIAPIBT_OpenBlueTooth(FALSE);
                need_restart_bt = TRUE;			
	   }
        }		
#endif
#else
    if(TRUE == Bbm_GetBTOn())
    {
        BbmCloseBt();
    }
#endif
#ifdef MMI_WIFI_SUPPORT
#ifndef ADULT_WATCH_SUPPORT
    if(FALSE == MMIAPIENG_GetIsFlightModeWifi())
#endif
    {
        if(MMIWIFI_STATUS_OFF != MMIAPIWIFI_GetStatus())
        {
            MMIAPIWIFI_SetInfo(FALSE);
            MMIAPIWIFI_Off();
        }
    }
#endif
        MMIAPIFM_Exit();

#ifdef ATV_SUPPORT
        MMIAPIATV_Exit();
#endif

#ifdef BROWSER_SUPPORT
        MMIAPIBROWSER_Close();
#endif

        {
            MMI_HANDLE_T status_win_id = MMIAPIPHONE_GetUpdateNetworkStatusWin();

            if (MMK_IsOpenWin(status_win_id)
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
                && MMISET_SET_MULTISYS_TYPE_WIN_ID != status_win_id
#endif                
                )
            {
                MMK_CloseWin(status_win_id);
            }
        }
    }
    s_is_fly_mode_on = is_fly_mode_on;
#ifdef  BT_DIALER_SUPPORT	
    if((!is_fly_mode_on) && need_restart_bt)//bug 911032 close fly mode, need restart bt
    {
        MMIAPIBT_OpenBlueToothInFlyMode(TRUE);
        need_restart_bt = FALSE;	
    }
#endif
    MMINV_WRITE(MMINV_SET_FLY_MODE_SWITCH,&is_fly_mode_on);

    MAIN_UpdateIdleSignalState();
}

/*****************************************************************************/
//  Description : set fly mode whole flow
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetFlyModeWholeFlow(BOOLEAN is_fly_mode_on)
{
#ifdef MCARE_V31_SUPPORT
	if(MCareV31_Exit_Confirm())
	{
		return;		
	}
#endif

    if(FALSE == is_fly_mode_on)
    {
        uint32 i = MN_DUAL_SYS_1;
        BOOLEAN is_ps_deactive = TRUE;

        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if(!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)i))
            {
                is_ps_deactive = FALSE;
                break;
            }
        }

        if (is_ps_deactive)
        {
            MMIAPISET_SetFlyMode(FALSE);
            MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);
            
            // 激活协议栈
            MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
            MMIAPIMMS_RestartAutodownMMS();
        }
        else
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SYS_WAITING_AND_TRY_LATER, TXT_NULL, 
                IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            return;
        }
    }
    else    // active flymode
    {
        uint32 dual_sys = MN_DUAL_SYS_1;
        BOOLEAN is_ps_ready = TRUE;
        MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
        
        for (dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
        {
            if ((e_dualsys_setting & (0x01 << dual_sys))
                && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
            {
                is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);
        
                if (!is_ps_ready)
                {
                    break;
                }
            }
        }        
        
        if (is_ps_ready)
        {
            if (MMISET_GetIsQueryOpenFlyMode())
            {
                MMIPUB_OpenQueryWinByTextId(TXT_FLY_MODE_PS_ALERT, IMAGE_PUBWIN_QUERY, PNULL, MMISET_HandleFlightmodeWinMsg);
                return;
            }
            else
            {
                uint32 i = 0;
                MMIAPISET_SetFlyMode(TRUE);
                MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);

                for( i = 0; i < MMI_DUAL_SYS_MAX; i++)
                {
                    MAIN_SetIdleRoamingState(i,FALSE);
                    MAIN_SetIdleForwardState(i,FALSE);
                }                
                
                MMIAPIPHONE_DeactivePsAccordingToSetting();
            }
        }
        else
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SYS_WAITING_AND_TRY_LATER, TXT_NULL, 
                IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            return;
        }
    }

#ifdef MMI_GUI_STYLE_TYPICAL
    //success 
    MMIPUB_OpenAlertSuccessWin( TXT_COMPLETE);
#endif
}

/*****************************************************************************/
//  Description : check the time if is valid
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPISET_CheckTimeIsValid(
//                                          MMISET_TIME_T       time        //the time
//                                          )
//{
//    BOOLEAN     result = TRUE;
//    
// 
//    //check the minute
//    result = MMIAPICOM_CheckTimeIsValid(time);
//    
//    return (result);
// }

///*****************************************************************************/
////    Description : check the date if is valid
////    Global resource dependence : none
////  Author: Jassmine.Meng
////    Note:
///*****************************************************************************/
//PUBLIC BOOLEAN MMIAPISET_CheckDateIsValid(
//                                          MMISET_DATE_T            date    //the date
//                                          )
//{
//    BOOLEAN     result = TRUE;
//    
//    //check year
//    if ((MMISET_DATE_MIN_YEAR > date.date_year) || (MMISET_DATE_MAX_YEAR < date.date_year))
//    {
//        result = FALSE;
//    }
//    else if ((MMISET_DATE_MAX_MONTH < date.date_month) || (0 == date.date_month))//check month
//    {
//        result = FALSE;
//    }
//    else if (0 == date.date_day)
//    {
//        result = FALSE;
//    }
//    else //check day
//    {
//        result = CheckDayOfDateIsValid(date);
//    }
//    
//    return (result);
//}

/*****************************************************************************/
//  Description : check the date if is valid for calendar
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
///*****************************************************************************/
//PUBLIC BOOLEAN MMIAPISET_CheckDateIsValidForCalendar(
//                                                     MMISET_DATE_T            date    //the date
//                                                     )
//{
//    BOOLEAN     result = TRUE;
//    
//    //check year
//    if ((MMISET_DATE_MIN_YEAR_CALENDAR > date.date_year) || (MMISET_DATE_MAX_YEAR_CALENDAR < date.date_year))
//    {
//        result = FALSE;
//    }
//  else
//  {
//      result = MMIAPICOM_CheckDateIsValid(date);
//  }
////    else if ((MMISET_DATE_MAX_MONTH < date.date_month) || (0 == date.date_month))//check month
////    {
////        result = FALSE;
////    }
////    else if (0 == date.date_day)
////    {
////        result = FALSE;
////    }
////    else //check day
////    {
////        result = CheckDayOfDateIsValid(date);
////     }
//    
//    return (result);
// }

///*****************************************************************************/
////    Description : check the day of date if is valid
////    Global resource dependence : none
////  Author: Jassmine.Meng
////    Note:
///*****************************************************************************/
//LOCAL BOOLEAN CheckDayOfDateIsValid(
//                                    MMISET_DATE_T            date    //the date
//                                    )
//{
//    BOOLEAN     result = TRUE;
//    
//    switch (date.date_month)
//    {
//    case MMISET_DATE_JAN:
//    case MMISET_DATE_MAR:
//    case MMISET_DATE_MAY:
//    case MMISET_DATE_JUL:
//    case MMISET_DATE_AUG:
//    case MMISET_DATE_OCT:
//    case MMISET_DATE_DEC:
//        //check big month
//        if ((MMISET_DATE_BIG_MONTH_DAY < date.date_day))
//        {
//            result = FALSE;
//        }
//        break;
//        
//    case MMISET_DATE_APR:
//    case MMISET_DATE_JUN:
//    case MMISET_DATE_SEP:
//    case MMISET_DATE_NOV:
//        //check small month
//        if ((MMISET_DATE_SMALL_MONTH_DAY < date.date_day))
//        {
//            result = FALSE;
//        }
//        break;
//        
//    case MMISET_DATE_FEB:
//        if ((!(date.date_year % 4) && (date.date_year % 100)) || !(date.date_year % 400))
//        {
//            //check leap month
//            if ((MMISET_DATE_LEAP_MONTH_DAY < date.date_day))
//            {
//                result = FALSE;
//            }
//        }
//        else
//        {
//            //check not leap month
//            if ((MMISET_DATE_NOT_LEAP_MONTH_DAY < date.date_day))
//            {
//                result = FALSE;
//            }
//        }
//        break;
//        
//    default:
//        SCI_TRACE_LOW("CheckDayOfDateIsValid:date.date_month = %d is error!",date.date_month);
//        break;
//    }
//    
//    return (result);
//}

/*****************************************************************************/
//  Description : get the type of language 
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetLanguageType(
                                      MMISET_LANGUAGE_TYPE_E  *language_type_ptr
                                      )
{
    //SCI_ASSERT(PNULL != language_type_ptr);
   if(PNULL == language_type_ptr)
   {
   	//SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_GetLanguageType"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_1498_112_2_18_2_51_9_184,(uint8*)"");
	return;
   }
    
    
    *language_type_ptr = (MMISET_LANGUAGE_TYPE_E)g_ksp_Lang;
}

/*****************************************************************************/
//  Description : set the type of language 
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetLanguageType(
                                      MMISET_LANGUAGE_TYPE_E   language_type//the type of language
                                      )
{
    if (!MMIAPISET_CheckLanguageStatus(language_type)//该语言状态是否有效
        || !MMIAPISET_IsSelectedLanguageValid(language_type)//该语言字库存在
        )
    {
        //SCI_TRACE_LOW:"MMIAPISET_SetLanguageType language_type=%d, not exist"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_1518_112_2_18_2_51_9_185,(uint8*)"d", language_type);
    
        return;
    }

    //int8 previous_Lang = g_ksp_Lang;
    //change the language of system
#ifdef MMI_SIM_LANGUAGE_SUPPORT
    //change the language of system : g_ksp_Lang
    g_ksp_Lang = MMIAPISET_ConvertNVLangToSysType(language_type);
#else
    g_ksp_Lang = (uint8)language_type;
#endif

    SetGlobalLanguageRef(g_ksp_Lang);/*lint !e64*/
    
    //write the type of language into NV
    MMINV_WRITE(MMINV_SET_LANGUAGE_TYPE,&language_type);
    
    // load new language text resource
    MMI_LoadNewTextRes();

    MMIAPISMS_SetDefaultPresetSms(FALSE);

    MMIAPICONNECTION_ChangeSettingListLanguage();

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMIAPISET_SetDefaultSimName(TRUE);
#endif

    MMIAPISTK_CnfSimLangSel();
}

/*****************************************************************************/
//  Description : Set global langguage reference settings
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:including inputmethod, diplay rtl
/*****************************************************************************/
LOCAL void SetGlobalLanguageRef(MMISET_LANGUAGE_TYPE_E language_type)
{
#ifdef VECTOR_FONT_SUPPORT_ETRUMP
    extern void SPMLAPI_SetCurrentLanguage(MMISET_LANGUAGE_TYPE_E lang_type);
	SPMLAPI_SetCurrentLanguage(language_type);
#endif

#if (defined IM_ARABIC_SUPPORT) ||  (defined IM_URDU_SUPPORT) || (defined IM_PERSIAN_SUPPORT) || (defined IM_HEBREW_SUPPORT)
    //set display direct
    if ((MMISET_LANGUAGE_ARABIC == language_type)
        ||(MMISET_LANGUAGE_URDU == language_type)
        ||(MMISET_LANGUAGE_PERSIAN == language_type)
        ||(MMISET_LANGUAGE_HEBREW ==language_type))
    {
        MMITHEME_SetDisDirection(MMI_DIRECTION_RIGHT);
    }
    else
#endif
    {
        MMITHEME_SetDisDirection(MMI_DIRECTION_LEFT);
    }

    // 所有滚动都是以字符串形式滚动的
    if ((MMISET_LANGUAGE_ENGLISH== language_type)   ||
        (MMISET_LANGUAGE_SIMP_CHINESE== language_type))
//      ||(MMISET_LANGUAGE_TRAD_CHINESE== language_type)
    {
        MMITHEME_SetIsScrollAsPixel(FALSE);//set win title,label,listbox,menu scroll as pixel or character
    }
    else
    {
        MMITHEME_SetIsScrollAsPixel(TRUE);
    }
}

/*****************************************************************************/
//  Description : get the time of key board light
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetKeyBoardLightTime(
                                           MMISET_KEYBOARD_LIGHT_SET_T   *keyboard_info
                                           )
{
   // SCI_ASSERT(PNULL != keyboard_info);
    if(PNULL == keyboard_info)
   {
   	//SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_GetKeyBoardLightTime"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_1598_112_2_18_2_51_9_186,(uint8*)"");
	return;
   }
       
    //get the info of key board light
    *keyboard_info = s_keyboard_light_info;
}

/********************************************************************************
NAME:           MMIAPISET_ParseCleanDataString
DESCRIPTION:    "####7777#": clean user data
PARAM IN:       
PARAM OUT:      
AUTHOR:     kelly.li
DATE:       
********************************************************************************/
PUBLIC BOOLEAN MMIAPISET_ParseCleanDataString(uint8* str_str, uint8 str_len)
{
    if (PNULL == str_str)
    {
        MMI_ASSERT_WARNING("str_str != PNULL");
        return  FALSE;
    }
       
    if (str_len != 9)
    {
        return  FALSE;
    }
    
    if (strncmp((char*)str_str, "####7777#", 9) == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************************************************/
//  Description : set the time of key board light
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetKeyBoardLightTime(
                                MMISET_KEYBOARD_LIGHT_SET_T  keyboard_light_info
                                )
{
    //set the time of key board light into NV
    MMINV_WRITE(MMINV_SET_KEYBOARD_LIGHT_INFO,&keyboard_light_info);
    
    //set global variable
    s_keyboard_light_info.keyboard_type = keyboard_light_info.keyboard_type;
    s_keyboard_light_info.keyboard_light_time = keyboard_light_info.keyboard_light_time;
}

/*****************************************************************************/
//  Description : clean user date
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_CleanUserData(void)
{    
    // NVITEM_ERROR_E                          result = NVERR_NONE;

    //format U disk
    
    //SCI_TRACE_LOW:"MMISET_CleanUserData: enter."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_1661_112_2_18_2_51_9_187,(uint8*)"");
	SCI_TRACE_LOW("MMISET_CleanUserData enter");
    
#ifdef OD_GAME_SUPPORT
	SCI_TRACE_LOW("od_game_load_nv is called");
    od_game_load_nv();
#endif

#ifndef PLATFORM_UWS6121E   //bug2092185
    SCI_TRACE_LOW("MMIAPIFMM_FormatDevice is called");
    MMIAPIFMM_FormatDevice((uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK), MMIFILE_FORMAT_AUTO);
#else
#ifdef MMI_RECORD_SUPPORT
    SCI_TRACE_LOW("MMIRECORD_FactoryReset is called");
    MMIRECORD_FactoryReset();
#endif
    SCI_TRACE_LOW("WATCH_DC_FactoryReset is called");
    WATCH_DC_FactoryReset();

    SCI_TRACE_LOW("MMIAPICL_ClearCalllogData is called");
    MMIAPICL_ClearCalllogData();

    SCI_TRACE_LOW("ual_audioplayer_restore_factory is called");
    ual_audioplayer_restore_factory();

#endif
#ifdef ZDT_ZFB_SUPPORT
    ZFB_DB_KV_ListDelAll();
#endif

#ifdef BAIDU_DRIVE_SUPPORT
	SCI_TRACE_LOW("MMISRV_BAIDUDRV_Logout is called");
	MMISRV_BAIDUDRV_Logout();
#endif

	SCI_TRACE_LOW("MMIAPIATV_ClearUserData is called");
	MMIAPIATV_ClearUserData();

	SCI_TRACE_LOW("MMIAPIMMS_DelUserInfo is called");
    MMIAPIMMS_DelUserInfo();

#ifdef MMI_MEMO_SUPPORT
	SCI_TRACE_LOW("MMIMEMO_DeleteUserDataInPhone is called");
    MMIMEMO_DeleteUserDataInPhone();
#endif

#ifdef MMI_WIFI_SUPPORT
	SCI_TRACE_LOW("MMIAPIWIFI_Reset is called");
    MMIAPIWIFI_Reset();
#endif
    
#ifdef BROWSER_SUPPORT
	SCI_TRACE_LOW("MMIAPIBROWSER_CleanUserInfo is called");
    MMIAPIBROWSER_CleanUserInfo();
#endif

#ifdef MMI_INDIAN_SOS_SUPPORT
	SCI_TRACE_LOW("MMISET_ResetSOSContextInfo is called");
	MMISET_ResetSOSContextInfo();
#endif
    
#if defined(MMI_BLACKLIST_SUPPORT)
	SCI_TRACE_LOW("RestoreBlacklistSetting is called");
    RestoreBlacklistSetting(TRUE);//clean blakclist user data
#endif

#ifndef WIN32
    //调用nv接口函数，在下次开机时对NV进行格式化处理
#ifdef NANDBOOT_SUPPORT  //在nor的efs中没有这个接口。
	SCI_TRACE_LOW("NVITEM_MarkReset is called");
    NVITEM_MarkReset();
#else
    {       
        NVITEM_ERROR_E                          result = NVERR_NONE;
		SCI_TRACE_LOW("NVITEM_FlushAll is called");
        
        //EFS_NvitemDelete(NV_SMS_MESSAGE_ITEM,NV_SMS_MESSAGE_ITEM_LAST_ENTRY); //delete sms in phone
        EFS_NvitemDelete(NV_PHONE_BOOK_MAIN,NV_PHONE_BOOK_LND_LAST_ENTRY); //delete phonebook information in phone
#ifndef PLATFORM_UWS6121E
        EFS_NvitemDelete(NV_USER_BASE,NV_USER_BASE + MAX_NV_USER_ITEM_NUM);              
        result = NVITEM_FlushAll();
        //SCI_TRACE_LOW:"MMISET_CleanUserData delete result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_1699_112_2_18_2_51_9_188,(uint8*)"d",result);
#endif
    }
#endif  
#endif
    
	SCI_TRACE_LOW("MMIAPISET_SetNeedResetDateTime is called");
    MMIAPISMS_NV_DelAllSmsItem(MMINV_MAX_SMS_RECORDS);
	SCI_TRACE_LOW("MMIAPISET_SetNeedResetDateTime is called");
    MMIAPISMS_SetDefaultPresetSms(TRUE);
	SCI_TRACE_LOW("MMIAPISET_SetNeedResetDateTime is called");
    MMIAPISET_SetNeedResetDateTime(TRUE);
#ifndef PLATFORM_UWS6121E
    SCI_TRACE_LOW("MMIAPICONNECTION_ResetFactorySetting is called");
    MMIAPICONNECTION_ResetFactorySetting(TRUE);
#endif

#ifdef OD_GAME_SUPPORT
	SCI_TRACE_LOW("od_game_save_nv is called");
    od_game_save_nv();
#endif
	SCI_TRACE_LOW("MMISET_CleanUserData exit");
    MMK_PostMsg(MMISET_RESET_FACTORY_WAITING_WIN_ID, MSG_SET_CLEAN_USER_DATA_OVER_IND, PNULL,0);
    
    //SCI_TRACE_LOW:"MMISET_CleanUserData: Success Exit."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_1707_112_2_18_2_51_10_189,(uint8*)"");
}

/*****************************************************************************/
//  Description : reset display setting 
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
LOCAL void ResetDisplaySetting(void)
{
    //MMISET_TIME_DISPLAY_TYPE_E              time_display_type       =   MMISET_DEFAULT_TIME_24HOURS;/*lint !e64*/
    //MMISET_DATE_DISPLAY_TYPE_E              date_display_type       =   MMISET_DEFAULT_DATE_YMD;/*lint !e64*/
    uint8                                   mainlcd_contrast        =   MMISET_EDEFAULT_CONTRAST;
    MMISET_KEYBOARD_LIGHT_SET_T             keyboard_time_info      =   {0};
#ifndef FLASH_SUPPORT
    MMISET_WALLPAPER_SET_T                  wallpaper_info          =   {0};
#ifdef SCREENSAVER_SUPPORT    
    MMISET_SCREENSAVER_INFO_T               screensaver_info        =   {0};
#endif
#endif
    MMISET_BACKLIGHT_SET_T                  backlight_info          =   {0};
    MMISET_TVOUT_TYPE_E                     tvout_mode              =   MMISET_PAL_LINE_BY_LINE;
    MMISET_POWER_ON_OFF_ANIM_INFO_T         power_on_off_anim_info  =   {0};
    uint32                                  move_style              =   MMISET_EDEFAULT_MOVE_STYLE;
#ifdef TTS_SUPPORT
    BOOLEAN                    	 			is_tts_on               =   TRUE; 
#endif
#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
    wchar font_name[MMIFMM_FILE_FULL_PATH_MAX_LEN + 1] = {0};
#endif
  
    //时间显示格式
    MMIAPISET_SetTimeDisplayType(MMISET_DEFAULT_TIME_24HOURS);
    //日期显示格式
    MMIAPISET_SetDateDisplayType(MMISET_DEFAULT_DATE_YMD);
    
#if defined(MMI_UPDATE_TIME_SUPPORT)
    MMIAPISET_ResetUpdateTimeDefault();
#endif /*MMI_UPDATE_TIME_SUPPORT*/

#ifdef MULTI_THEME_SUPPORT   
    MMISET_SetCurrentTheme(MMISET_DEFAULT_THEME_TYPE, PNULL);    
#endif
    
    //reset the time of key board light
    keyboard_time_info.keyboard_light_time = MMISET_EDEFAULT_KEYBOARD_LIGHT_TIME;
    keyboard_time_info.keyboard_type       = MMISET_EDEFAULT_KEYBOARD_LIGHT_TYPE;/*lint !e64*/
    MMINV_WRITE(MMINV_SET_KEYBOARD_LIGHT_INFO,&keyboard_time_info);
    s_keyboard_light_info.keyboard_type    = keyboard_time_info.keyboard_type;
    s_keyboard_light_info.keyboard_light_time = keyboard_time_info.keyboard_light_time;
    
    MMINV_WRITE(MMINV_SET_WINDOW_MOVE_STYLE_INFO, &move_style);
    MMK_SetWinDefaultAnimMoveStyle(move_style);/*lint !e64*/
  
#ifndef FLASH_SUPPORT
    //reset idle style
    MMISET_ResetIdleStyle();
    //reset idle display mode
    MMISET_ResetIdleDisplayMode();
    //reset the information about wallpaper set
    wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;
    wallpaper_info.wallpaper_id = MMIAPISET_GetDefaultWallpaperIDByStyle();/*lint !e64*/
    //MMINV_WRITE(MMINV_SET_WALLPAPER_INFO,&wallpaper_info);
    MMISET_SetWallpaperNVByStyle(&wallpaper_info);
#ifdef SCREENSAVER_SUPPORT    
    //reset the screensavers setting
    //@xingdong.li
    screensaver_info.is_open     = MMISET_EDEFAULT_SCREENSAVER_OPEN;
    screensaver_info.ssaver_type = MMISET_EDEFAULT_SCREENSAVER_TYPE;/*lint !e64*/
    MMINV_WRITE(MMINV_SCREENSAVER_SETTING_INFO, &screensaver_info);
#endif   
#endif

    //@xingdong.li
    power_on_off_anim_info.is_open      = MMISET_EDEFAULT_POWER_ONOFF_ANIM_OPEN;
    power_on_off_anim_info.anim_type    = MMISET_EDEFAULT_POWER_ONOFF_ANIM_TYPE;/*lint !e64*/
    MMINV_WRITE(MMINV_POWER_ON_ANIM_SETTING_INFO, &power_on_off_anim_info);
    MMINV_WRITE(MMINV_POWER_OFF_ANIM_SETTING_INFO, &power_on_off_anim_info);
    
    //reset the information about backlight
    backlight_info.backlight_type = MMISET_EDEFAULT_BACKLIGHT_OPEN;/*lint !e64*/
    backlight_info.backlight_time = MMISET_EDEFAULT_BACKLIGHT_TIME;
    MMIAPISET_SetBackLightInfo(backlight_info);
    
    //reset the contrast of mainlcd
    MMINV_WRITE(MMINV_SET_MAINLCD_CONTRAST,&mainlcd_contrast);
    MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,mainlcd_contrast);
    // @Great.Tian, CR25668
    MMIAPISET_SetCurrentContrast(mainlcd_contrast);
    
#ifdef TTS_SUPPORT
    MMINV_WRITE(MMINV_SET_TTS_ON_OFF, &is_tts_on);
#endif

    //reset main menu animation status
   // MMINV_WRITE(MMINV_SET_IS_MENU_ANIMATION_OPEN,&is_menu_animation_open);
    
    //reset tv out mode 
    MMINV_WRITE(MMINV_SET_TV_OUT_MODE,&tvout_mode);

#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
       MMIRES_UnloadFileFont();
       MMINV_WRITE( MMINV_SET_FONT, font_name );

#endif
}

/*****************************************************************************/
//  Description : reset display setting 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetDisplaySetting(void)
{
    ResetDisplaySetting();
}

#ifdef LCD_SLIDE_SUPPORT
/*****************************************************************************/
// 	Description : reset slide setting 
//	Global resource dependence : none
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void ResetSlideSetting(void)
{
    //reset lcd slide setting
    MMISET_SetSelectSlideOpenType(MMISET_RECEIVE_PHONE_TYPE);
    MMISET_SetSelectSlideCloseType(MMISET_STOP_OPERATION_TYPE);
    MMISET_SetSelectKeylockSwitchType(MMISET_KEYLOCK_SWITCH_ON_TYPE);
}
#endif

/*****************************************************************************/
//  Description : reset factory setting 
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_ResetFactorySetting(void)
{
#ifdef RESETFACTORY_SETTING_SIMPLE
#ifndef WIN32
	RFS_reboot();
#endif
#else
#ifdef FLIP_PHONE_SUPPORT
	BOOLEAN 							is_flip_answer				=	FALSE;
#endif
	MMISET_KEYLOCK_TYPE_E				auto_keylock_time			=	MMISET_DEFAULT_KEYLOCK_TIME;	
	BOOLEAN 							is_redial					=	DEFAULT_REDIAL;
	uint8								call_reminder_time			=	CC_CALL_REMINDER_TIME_DEFAULT;
#if defined(MMI_PDA_SUPPORT)
	BOOLEAN 							is_connect_prompt			=	TRUE;
#else
	BOOLEAN 							is_connect_prompt			=	FALSE;
#endif
	MMISET_ANYKEY_REJECT_SET_T			anykey_reject_mode			=	{0};  
	BOOLEAN 							is_fly_mode_on				=	FALSE;
#ifdef CMCC_UI_STYLE
	MMISET_SHORTCUT_TYPE_E	fk_info[MMISET_FUNCKEY_MAX] = {MMISET_SHORTCUT_MONERNET, MMISET_SHORTCUT_MAINMENU};
#else
	MMISET_SHORTCUT_TYPE_E	fk_info[MMISET_FUNCKEY_MAX] = {MMISET_SHORTCUT_MAINMENU,MMISET_SHORTCUT_PB};
#endif 
	BOOLEAN 							is_fly_mode_power_on		=	FALSE;
	BOOLEAN 							is_red_keylock				=	FALSE;
	MMISET_LANGUAGE_TYPE_E				language_type			=	MMISET_EDEFAULT_LANGUAGE;/*lint !e64*/	//MMISET_LANGUAGE_SIMP_CHINESE; //default type of language
	MMISET_TXT_CODE_TYPE_TYPE_E 		code_type = MMISET_TXT_CODE_TYPE_GB;
#ifdef MV_SUPPORT	 
	BOOLEAN								is_mv_on = FALSE;
	MMISET_MV_INFO_T					mv_info  = {0};
#endif
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
	MMIMAINMENU_TYPE_E					mainmenu_select_style = MMISET_DEFAULT_MENU_SELECT_STYLE_E;
#endif
#endif
#ifdef MMI_AUTO_RECORD_SUPPORT
	BOOLEAN 							reset_to_FALSE = FALSE;
#endif
	SCI_TRACE_LOW("MMISET_ResetFactorySetting enter");
#ifdef GAME_SUPPORT
	SCI_TRACE_LOW("MMIAPIGAME_RsetGameData is called");
	MMIAPIGAME_RsetGameData();
#endif
#ifdef OD_GAME_SUPPORT
	SCI_TRACE_LOW("od_game_load_nv is called");
	od_game_load_nv();
#endif

#ifdef MMI_TASK_MANAGER
#ifdef EBOOK_SUPPORT
		SCI_TRACE_LOW("MMIAPIEBOOK_StopApplet is called");
		MMIAPIEBOOK_StopApplet(FALSE);//NEWMS00134786
#endif
#endif
	SCI_TRACE_LOW("MMIAPISET_SetWaitFlag is called");
	MMIAPISET_SetWaitFlag();
	//reset main menu style
#ifndef FLASH_SUPPORT
	SCI_TRACE_LOW("MMIAPISET_SetMainmenuStyle is called");
	MMIAPISET_SetMainmenuStyle(MMISET_DEFAULT_MENU_STYLE_E);
#endif 
	//reset the type of language
	SCI_TRACE_LOW("MMIAPISET_SetLanguageType is called");
	MMIAPISET_SetLanguageType(language_type);
#ifndef MMI_KEY_LOCK_EFFECT_QQVGA
	#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
	SCI_TRACE_LOW("MMIAPISET_SetKeyLockEffect is called");
	MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_DEFAULT);
	#endif
#endif	  
	//reset txt code type	
	SCI_TRACE_LOW("MMINV_WRITE is called");
	MMINV_WRITE(MMINV_SET_TXT_CODE_TYPE,&code_type);
	
	#ifdef MMI_GRID_IDLE_SUPPORT
	SCI_TRACE_LOW("MMIAPIGRID_ResetFactorySetting is called");
	MMIAPIGRID_ResetFactorySetting();
#endif
  
 #ifdef QBTHEME_SUPPORT
	SCI_TRACE_LOW("MMIAPIQBIDLE_ResetFactorySetting is called");
	MMIAPIQBIDLE_ResetFactorySetting();
	SCI_TRACE_LOW("MMIQBTHEME_ResetDockbarPage2Data is called");
	MMIQBTHEME_ResetDockbarPage2Data();
#endif

	SCI_TRACE_LOW("ResetDisplaySetting is called");
	ResetDisplaySetting();
	
	//reset the function key shortcut settings	
	SCI_TRACE_LOW("MMINV_WRITE is called");
	MMINV_WRITE(MMINV_SET_FUNCKEY_SHORTCUT_INFO,fk_info);		

	//reset im
	SCI_TRACE_LOW("MMIAPIIM_ResetNv is called");
	MMIAPIIM_ResetNv();    
#if defined IM_ENGINE_SOGOU 	//非NV数据恢复
	SCI_TRACE_LOW("MMIAPIIM_SGResetDefault is called");
	MMIAPIIM_SGResetDefault ();
#endif
		
#ifdef LCD_SLIDE_SUPPORT
	//reset滑盖设置
	SCI_TRACE_LOW("ResetSlideSetting is called");
	ResetSlideSetting();
#endif
	
	//reset the mode of any key and reject
	SCI_TRACE_LOW("MMINV_WRITE ANYKEY_REJECT_MODE is called");
	anykey_reject_mode.is_anykey_answer = FALSE;
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
	#ifdef CMCC_UI_STYLE
	anykey_reject_mode.is_sms_reply_open = TRUE;
	#else
	anykey_reject_mode.is_sms_reply_open = FALSE;
	#endif
#endif
	MMINV_WRITE(MMINV_SET_ANYKEY_REJECT_MODE,&anykey_reject_mode);	
	
	//reset ip dial 
#ifdef MMI_IP_CALL_SUPPORT
	SCI_TRACE_LOW("MMISET_SetDefaultIPNumberInfoToNV is called");
	MMISET_SetDefaultIPNumberInfoToNV();
#endif
#ifdef FLIP_PHONE_SUPPORT
	//reset flip answer
	SCI_TRACE_LOW("MMINV_WRITE FLIPANSWER is called");
	is_flip_answer = FALSE;
	MMINV_WRITE(MMINV_SET_IS_FLIPANSWER,&is_flip_answer);
#endif
	
	//reset auto board key
	SCI_TRACE_LOW("MMINV_WRITE AUTO_KEYLOCK and RED_KEYLOCK is called");
	auto_keylock_time = MMISET_DEFAULT_KEYLOCK_TIME;
	MMINV_WRITE(MMINV_SET_AUTO_KEYLOCK_TIME,&auto_keylock_time);

	//reset red keylock
	is_red_keylock = FALSE;
	MMINV_WRITE(MMINV_SET_IS_RED_KEYLOCK,&is_red_keylock);

	//reset pim setting
	SCI_TRACE_LOW("MMIAPIPIM_ResetFactorySetting is called");
	MMIAPIPIM_ResetFactorySetting(); 

	//reset auto redail
	SCI_TRACE_LOW("MMIAPISET_SetIsAutoRedial is called");
	MMIAPISET_SetIsAutoRedial(is_redial);
	
	//reset minute reminder 
	SCI_TRACE_LOW("MMISET_SetMinuteReminderTime is called");
	MMISET_SetMinuteReminderTime(call_reminder_time);
	
	//reset connect prompt
	SCI_TRACE_LOW("MMINV_WRITE CONNECT_PROMPT_STATUS is called");
	MMINV_WRITE(MMINV_SET_CONNECT_PROMPT_STATUS,&is_connect_prompt);

#ifdef MV_SUPPORT
	//reset mv set
	MMINV_WRITE(MMINV_SET_MAGIC_VOICE_STATUS,&is_mv_on);
	//reset mv param sets
	MMINV_WRITE(MMINV_SET_MAGIC_VOICE_SETTING_INFO,&mv_info);
#endif

	//reset environment setting
	SCI_TRACE_LOW("MMIENVSET_ResetEnvSetSetting is called");
	MMIENVSET_ResetEnvSetSetting();

	//reset worldtime setting
	SCI_TRACE_LOW("MMIAPIWCLK_ResetFactorySetting is called");
	MMIAPIWCLK_ResetFactorySetting();

	//reset wifi
#ifdef MMI_WIFI_SUPPORT
	SCI_TRACE_LOW("MMIAPIWIFI_Reset is called");
	MMIAPIWIFI_Reset();
#endif

	SCI_TRACE_LOW("MMIAPICONNECTION_ResetFactorySetting is called");
	MMIAPICONNECTION_ResetFactorySetting(FALSE);
	
	//reset wap
#ifdef BROWSER_SUPPORT
	SCI_TRACE_LOW("MMIAPIBROWSER_ResetFactorySetting is called");
	MMIAPIBROWSER_ResetFactorySetting();
#endif

#ifdef DL_SUPPORT
	SCI_TRACE_LOW("MMIAPIDL_ResetFactorySetting is called");
	MMIAPIDL_ResetFactorySetting();
#endif
	
	//reset mms
	SCI_TRACE_LOW("MMIAPIMMS_ResetMMSFactorySetting is called");
	MMIAPIMMS_ResetMMSFactorySetting();
	
	// reset sms
	SCI_TRACE_LOW("MMIAPISMS_RestoreFactorySetting is called");
	MMIAPISMS_RestoreFactorySetting();
	
	//reset multim wallpaper
	//	  MMIAPIFMM_InitWallPaper();
	SCI_TRACE_LOW("ResetWallpaperSetting is called");
	ResetWallpaperSetting();
	SCI_TRACE_LOW("MMISET_ResetAllStylesWallpaperAtRestoreFactory is called");
	MMISET_ResetAllStylesWallpaperAtRestoreFactory();
	
	//reset direction key info
	SCI_TRACE_LOW("MMISET_ResetDirectionKeyInfo is called");
	MMISET_ResetDirectionKeyInfo();
	SCI_TRACE_LOW("MMISET_ResetIdleShortcutInfo is called");
	MMISET_ResetIdleShortcutInfo();
	
	// reset auto power on/off
	SCI_TRACE_LOW("MMIAPIALM_RestoreFactorySetting is called");
	MMIAPIALM_RestoreFactorySetting();
	
	//reset privacy protect info
	SCI_TRACE_LOW("MMISET_ResetPrivacyProtectInfo is called");
	MMISET_ResetPrivacyProtectInfo();

	//reset privacy protect password
	SCI_TRACE_LOW("MMISET_ResetPrivacyProtectPwd is called");
	MMISET_ResetPrivacyProtectPwd();//@fen.xie MS00179453

	//reset poweron pwd status
	SCI_TRACE_LOW("MMIAPISET_ResetPowerOnPwdStatus is called");
	MMIAPISET_ResetPowerOnPwdStatus();//@zhaohui,cr95392
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
	SCI_TRACE_LOW("MMIAPISET_ResetKLPsw is called");
	MMIAPISET_ResetKLPsw();
#endif
#endif

	SCI_TRACE_LOW("MMIAPISET_SetMultimVolume is called");
	MMIAPISET_SetMultimVolume(MMISET_VOL_DEFAULT_MULTIM);
#if defined(MMI_MSD_SUPPORT)
	//reset msd 
	SCI_TRACE_LOW("MMIAPIMSD_SetFactoryValue is called");
	MMIAPIMSD_SetFactoryValue();
#endif

	//reset picture viewer
	SCI_TRACE_LOW("MMIAPIPICVIEW_ResetFactorySettings is called");
	MMIAPIPICVIEW_ResetFactorySettings();

#ifdef MMIUZONE_SUPPORT
	SCI_TRACE_LOW("MMIAPIUZONE_ResetFactorySettings is called");
	MMIAPIUZONE_ResetFactorySettings();
#endif

#ifdef MMI_AUDIO_PLAYER_SUPPORT
	//reset audio player
	SCI_TRACE_LOW("MMIAPIMP3_ReSetFactory is called");
	MMIAPIMP3_ReSetFactory();
#endif
	//reset dc setting
	SCI_TRACE_LOW("MMIAPIDC_Setting_SetFactoryValue is called");
	MMIAPIDC_Setting_SetFactoryValue();
	
	//reset mobiletv
	SCI_TRACE_LOW("MMIAPIMTV_SettingInfoSetFactory is called");
	MMIAPIMTV_SettingInfoSetFactory();

	//reset atv
	SCI_TRACE_LOW("MMIAPIATV_RestoreFactorySetting is called");
	MMIAPIATV_RestoreFactorySetting();

#ifdef DCD_SUPPORT
	SCI_TRACE_LOW("MMIAPIDCD_RestoreFactorySetting is called");
	MMIAPIDCD_RestoreFactorySetting();
#endif
#ifdef ASP_SUPPORT
	SCI_TRACE_LOW("MMIAPIASP_ResetFactorySetting is called");
	MMIAPIASP_ResetFactorySetting();
#endif

	//飞行模式
	if (MMIAPISET_GetFlyMode())
	{
		is_fly_mode_on = FALSE;
		SCI_TRACE_LOW("MMIAPISET_SetFlyMode is called");
		MMIAPISET_SetFlyMode(is_fly_mode_on);

#ifdef MMI_DUALMODE_ENABLE 
		SCI_TRACE_LOW("MMISET_ResetNetWorkType is called");
		MMISET_ResetNetWorkType(TRUE);
#endif

		SCI_TRACE_LOW("MMIAPIPHONE_StartUpPsAccordingToSetting is called");
		MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
		SCI_TRACE_LOW("MMIAPIMMS_RestartAutodownMMS is called");
		MMIAPIMMS_RestartAutodownMMS();
	}
	else
	{
#ifdef MMI_DUALMODE_ENABLE 
		SCI_TRACE_LOW("MMISET_ResetNetWorkType is called");
		MMISET_ResetNetWorkType(FALSE);
#endif
	}
	is_fly_mode_power_on = FALSE;
	SCI_TRACE_LOW("MMINV_WRITE FLY_MODE_OPEN_PHONE is called");
	MMINV_WRITE(MMINV_FLY_MODE_OPEN_PHONE,&is_fly_mode_power_on);
#ifdef SET_3D_STYLE_SUPPORT
	SCI_TRACE_LOW("MMISET_Reset3DStyle is called");
	MMISET_Reset3DStyle();
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
	//MMIAPIPB_ResetPBGroupInfo();
#endif

	SCI_TRACE_LOW("MMIAPISET_FuncFinishedInd is called");
	MMIAPISET_FuncFinishedInd();
#ifdef BLUETOOTH_SUPPORT
	SCI_TRACE_LOW("MMIAPIBT_Reset is called");
	MMIAPIBT_Reset();
#endif
	// @baokun fix cr94235 end
#ifndef MMI_MULTI_SIM_SYS_SINGLE
	SCI_TRACE_LOW("MMIAPISET_SetDefaultDualSysSetting is called");
	MMIAPISET_SetDefaultDualSysSetting();
#endif
#ifdef EBOOK_SUPPORT
	SCI_TRACE_LOW("MMIAPIEBOOK_RestoreFactorySetting is called");
	MMIAPIEBOOK_RestoreFactorySetting();
#endif
	SCI_TRACE_LOW("RestoreDataTimeFactorySetting is called");
	RestoreDataTimeFactorySetting();
	
#ifdef MMI_UPDATE_TIME_SUPPORT
	SCI_TRACE_LOW("MMIAPISET_ResetUpdateTimeDefault is called");
	MMIAPISET_ResetUpdateTimeDefault();
#endif

#ifdef VT_SUPPORT
	SCI_TRACE_LOW("MMISET_ResetVTSetting is called");
	MMISET_ResetVTSetting();
#endif

#if 0//def TOUCH_PANEL_SUPPORT
	MMISET_ResetTouchVibrateSetting();
#endif

#ifdef PUSH_EMAIL_SUPPORT
	SCI_TRACE_LOW("mail_ResetFactorySetting is called");
	mail_ResetFactorySetting();
	mail_backToFactoryFinish();
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT
	SCI_TRACE_LOW("MMIAPISMS_DeleteAllMailMsg is called");
	MMIAPISMS_DeleteAllMailMsg();//清除信息服务中保存的邮件
#endif /* PUSH_EMAIL_3_IN_1_SUPPORT */
#endif/*PUSH_EMAIL_SUPPORT*/

#ifdef MMI_EMAIL_SUPPORT
	SCI_TRACE_LOW("MMIAPIEMAIL_ResetFactory is called");
	MMIAPIEMAIL_ResetFactory();
#endif

	SCI_TRACE_LOW("MMIAPIENG_RestoreFactorySetting is called");
	MMIAPIENG_RestoreFactorySetting();//@fen.xie MS00186848
#ifdef MAINLCD_SIZE_128X64
	MMISET_SetIsSensorOpenFlag(0xffff);
#else
	 MMISET_SetIsSensorOpenFlag(0x0028);//maryxiao cr230859
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
	SCI_TRACE_LOW("RestoreBlacklistSetting is called");
	RestoreBlacklistSetting(FALSE);//black list reset
#endif

#ifdef MMI_AZAN_SUPPORT
	SCI_TRACE_LOW("MMIAPIAZAN_ResetFactorySetting is called");
	MMIAPIAZAN_ResetFactorySetting();
#endif
#ifdef FM_SUPPORT
	SCI_TRACE_LOW("MMAPIIFM_Reset is called");
	MMAPIIFM_Reset();
#endif

#ifdef SOS_KEYLONGWEB
	//MMIAPIST_ResetSOS();
#endif

#ifdef ZDT_APP_SUPPORT

	MMIZDT_ResetFactory();
#endif
#ifdef HERO_ENGINE_SUPPORT
	//hero_tts_SettingsReset();
#endif
#ifdef WEATHER_SUPPORT
	SCI_TRACE_LOW("MMIAPIWEATHER_ResetFactory is called");
	MMIAPIWEATHER_ResetFactory();
#if defined MMI_WIDGET_WEATHER1
	SCI_TRACE_LOW("MMIAPIWEATHER_ResetDefaultSetting is called");
	MMIAPIWEATHER_ResetDefaultSetting(); 
#endif	 
#endif

#ifdef SCH_ASSISTANT_WIDGET
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
   SCI_TRACE_LOW("MMIAPIWIDGET_SCH_Reset is called");
   MMIAPIWIDGET_SCH_Reset();
#endif
#endif
	
#ifdef	MMI_KING_MOVIE_SUPPORT
	SCI_TRACE_LOW("MMIAPIKM_ResetFactory is called");
	MMIAPIKM_ResetFactory();
#endif	  
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
	SCI_TRACE_LOW("MMIAPISETVWP_Reset is called");
	MMIAPISETVWP_Reset();
#endif

#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
	SCI_TRACE_LOW("MMIAPIVP_ResetSettingInfo is called");
	MMIAPIVP_ResetSettingInfo();
#endif
#endif

#ifdef JAVA_SUPPORT_MYRIAD
	SCI_TRACE_LOW("MMIAPIJAVA_resetJbedFactorySetting is called");
	MMIAPIJAVA_resetJbedFactorySetting();
#endif

	SCI_TRACE_LOW("MMIAPIJAVA_resetJbedFactorySetting is called");
	MMIAPISET_FuncFinishedInd();

	
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
	SCI_TRACE_LOW("MMIAPISET_ResetPowerSavSetting is called");
	MMIAPISET_ResetPowerSavSetting();
#endif

#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
	SCI_TRACE_LOW("MMIAPISET_SetBatteryPercentDisSwitch is called");
	MMIAPISET_SetBatteryPercentDisSwitch(TRUE);
	GUIWIN_SetStbItemVisible(MMI_WIN_ICON_BATTERY_PERCENT, TRUE);
#endif

#ifdef PDA_UI_SUPPORT_MANIMENU_GO
	SCI_TRACE_LOW("MMISET_SetGOmenuStyle is called");
	MMISET_SetGOmenuStyle(GO_MENU_STYLE_CLASSIC);
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
	//reset main menu select style
	MMINV_WRITE(MMINV_SET_MAIN_MENU_SELECT_STYLE,&mainmenu_select_style);
#endif
#endif
#ifdef MMI_AUTO_RECORD_SUPPORT
	MMINV_WRITE(MMINV_SET_AUTO_RECORD,&reset_to_FALSE);
#endif

#ifdef MMI_GPRS_DATA_SIM_SET
	SCI_TRACE_LOW("MMIAPISET_RestoreGPRSModeSetting is called");
	MMIAPISET_RestoreGPRSModeSetting();
#endif

	// will.jiang: add for REF reset
	SCI_TRACE_LOW("REF_RestoreFactorySetting is called");
	REF_RestoreFactorySetting();	

	//放在Reset 最后
#ifdef CSC_XML_SUPPORT
	SCI_TRACE_LOW("MMIAPICUS_ResetSettings is called");
	MMIAPICUS_ResetSettings();
#endif

#ifdef OD_GAME_SUPPORT
	SCI_TRACE_LOW("od_game_save_nv is called");
	od_game_save_nv();
#endif
#endif

#ifdef APP_STORE_SUPPORT
    APPSTORE_FactoryReset();
#endif

    MMIAPIPHONE_VolteFactoryReset();

    MMIAPISET_SaveDataSimToNV(MN_DUAL_SYS_1);

	//MMK_PostMsg(MMISET_RESET_FACTORY_WAITING_WIN_ID, MSG_SET_RESET_FACTORY_OVER_IND, PNULL,0);
}

/*****************************************************************************/
//  Description : restore data and time factory setting
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void RestoreDataTimeFactorySetting(void)
{
#ifdef ZDT_ZFB_SUPPORT
    return;
#else
#ifndef WIN32
    SCI_TIME_T                  sys_time = {0};
    SCI_DATE_T                  sys_date = {0};
    sys_time.hour = 0;
    sys_time.min = 0;
    sys_time.sec = 0;
    MMIAPICOM_SetSysTime(sys_time);
    sys_date.mday = 1;
    sys_date.mon = 1;
    sys_date.wday = 6;
    sys_date.year = 2011;//bug-1641662
    MMIAPICOM_SetSysData(sys_date); 
    MAIN_SetStbDispTime();
#endif
#endif
}
/*****************************************************************************/
//  Description : need wait this function finished 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetWaitFlag(void)
{
	//SCI_TRACE_LOW:"[MMISET] MMIAPISET_SetWaitFlag"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2183_112_2_18_2_51_10_190,(uint8*)"");
	MMK_PostMsg(MMISET_RESET_FACTORY_WAITING_WIN_ID, MSG_SET_RESET_NEED_WAIT_IND, PNULL,0);
}

/*****************************************************************************/
//  Description : inform this function finished 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_FuncFinishedInd(void)
{
	//SCI_TRACE_LOW:"[MMISET] MMIAPISET_FuncFinishedInd"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2193_112_2_18_2_51_10_191,(uint8*)"");
	MMK_PostMsg(MMISET_RESET_FACTORY_WAITING_WIN_ID, MSG_SET_RESET_FACTORY_OVER_IND, PNULL,0);
}

/*****************************************************************************/
//  Description : get  valid lang index by lang index from s_lang_set
//  Global resource dependence : none
//  Author: yongwei.he
//  Note:
/*****************************************************************************/
LOCAL uint32 GetValidIdxByLangIdx(uint32 lang_index)
{
    uint32      i               =   0;
    
    if(lang_index >= MMISET_MAX_LANGUAGE)
    {
        //SCI_TRACE_LOW:"mmisetting_assert GetValidIdxByLangIdx %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2207_112_2_18_2_51_11_192,(uint8*)"d", lang_index);
        return 0;
    }
    if (PNULL!= s_lang_valid_info)
    {
        for (; i< s_lang_valid_info->lang_valid_num;i++)
        {
            if (lang_index == s_lang_valid_info->lang_valid[i])
            {
                return i;
            }
        }
    }
    return 0;
}

/*****************************************************************************/
//  Description : get  if input language_type is valid
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsSelectedLanguageValid( MMISET_LANGUAGE_TYPE_E language_type )
{
    uint32      i               =   0;
    // uint32      valid_index     =   0;
    BOOLEAN     result          =   FALSE;

    //SCI_ASSERT(language_type <= MMISET_MAX_LANGUAGE);
    if(language_type > MMISET_MAX_LANGUAGE)
    {
        //SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_IsSelectedLanguageValid language_type %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2236_112_2_18_2_51_11_193,(uint8*)"d", language_type);
        return FALSE;
    }


    if (language_type < MMISET_MAX_LANGUAGE)
    {
    	if(MMIAPISET_CheckLanguageStatus((MMISET_LANGUAGE_TYPE_E)i))
        //if (s_lang_set[language_type].lang_status)
        {
            result = TRUE;
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : get lang num by valid lang index from s_lang_set
//  Global resource dependence : none
//  Author: yongwei.he
//  Note:
/*****************************************************************************/
LOCAL uint32 GetLangIdxByValidIdx(uint32 valid_index)
{
    if (PNULL!=s_lang_valid_info)
    {
        return s_lang_valid_info->lang_valid[valid_index];
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : 添加语言选择的radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendLanguageListItem(
                                          MMI_CTRL_ID_T     ctrl_id
                                          )
{
    
    uint16                  cur_selection   =   0;
#ifdef MMI_SIM_LANGUAGE_SUPPORT
#else
    MMISET_LANGUAGE_TYPE_E  language_type   =   MMISET_MAX_LANGUAGE;
#endif    
    int32                   i               =   0;
    
    for(i = 0;i < MMISET_MAX_LANGUAGE;i++)
    {    
    	if(MMIAPISET_CheckLanguageStatus((MMISET_LANGUAGE_TYPE_E)i))  
        //if( s_lang_set[i].lang_status)
        {
            GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
            GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
            uint8 lang_name_len = 0;
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
            item_t.item_data_ptr = &item_data;
            
#ifdef MMI_PDA_SUPPORT
            item_data.softkey_id[0] = TXT_NULL;
            item_data.softkey_id[1] = TXT_NULL;
            item_data.softkey_id[2] = STXT_RETURN;
#else
            item_data.softkey_id[0] = TXT_COMMON_OK;
            item_data.softkey_id[1] = TXT_NULL;
            item_data.softkey_id[2] = STXT_RETURN;
#endif
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            
#ifdef MMI_SIM_LANGUAGE_SUPPORT
            if( MMISET_LANGUAGE_AUTO == (MMISET_LANGUAGE_TYPE_E)i)
            {
                MMI_STRING_T auto_lang_str = {0};
                
                MMI_GetLabelTextByLang(TXT_AUTO, &auto_lang_str);                
                item_data.item_content[0].item_data.text_buffer.wstr_ptr = auto_lang_str.wstr_ptr ;
                item_data.item_content[0].item_data.text_buffer.wstr_len = auto_lang_str.wstr_len;
            }
            else
#endif
            {
                lang_name_len = MMIAPICOM_Wstrlen(s_lang_set[i].lang_name);
                item_data.item_content[0].item_data.text_buffer.wstr_ptr = s_lang_set[i].lang_name;/*lint !e605*/
                item_data.item_content[0].item_data.text_buffer.wstr_len =  lang_name_len;
            }

            GUILIST_AppendItem(ctrl_id, &item_t);
        }
    }

#ifdef MMI_SIM_LANGUAGE_SUPPORT
    //MMIAPISET_GetLanguageType(&language_type);    
    if(s_nv_lang_setting >= MMISET_MAX_LANGUAGE)
    {
    	//SCI_TRACE_LOW:"mmisetting_assert MMISET_AppendLanguageListItem language_type %d"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2329_112_2_18_2_51_11_194,(uint8*)"d", s_nv_lang_setting);
        return;
    }

    cur_selection = GetValidIdxByLangIdx((uint32)s_nv_lang_setting);
#else
    MMIAPISET_GetLanguageType(&language_type);

    if (language_type >= MMISET_MAX_LANGUAGE)
    {
    	//SCI_TRACE_LOW:"mmisetting_assert MMISET_AppendLanguageListItem language_type %d"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2339_112_2_18_2_51_11_195,(uint8*)"d", language_type);
        return;
    }
    
    cur_selection = GetValidIdxByLangIdx((uint32)language_type);
#endif


    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
    
    //GUILIST_UpdateListBox(ctrl_id);
}

/*****************************************************************************/
//  Description : 设置语言选择的结果
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectLanguage(
                                     MMI_CTRL_ID_T      ctrl_id
                                     )
{
    uint16                  cur_selection   =   0;
    MMISET_LANGUAGE_TYPE_E  language_type   =   MMISET_MAX_LANGUAGE;
    
    //get the selected item
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
	//cur_selection=GUIDROPDOWNLIST_GetCurItemIndex(ctrl_id);

    language_type = GetLangIdxByValidIdx(cur_selection);/*lint !e64*/
    
    MMIAPISET_SetLanguageType(language_type);
}

/*****************************************************************************/
//  Description : 添加当前输入语言的所有项
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 添加完成后，设置当前焦点项为给定语言项
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendInputLanguageListItem(
											   MMI_CTRL_ID_T ctrl_id, 
											   MMISET_LANGUAGE_TYPE_E language_type
											   )
{
    uint16 cur_selection = 0;
    int32 i = 0;

    //SCI_ASSERT(language_type < MMISET_MAX_LANGUAGE);
    if(language_type >= MMISET_MAX_LANGUAGE)
    {
        //SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_AppendInputLanguageListItem language_type %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2391_112_2_18_2_51_11_196,(uint8*)"d", language_type);
        return;
    }
    
#ifdef MMI_SIM_LANGUAGE_SUPPORT
    i = 1;          //the first row is "auto", not used in input language
#else
    i = 0;
#endif

    for(; i < MMISET_MAX_LANGUAGE; i++)
    {    
        if(MMIAPISET_CheckLanguageStatus((MMISET_LANGUAGE_TYPE_E)i))
        {
            GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
            GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
            uint8 lang_name_len = 0;
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
            item_t.item_data_ptr = &item_data;

#ifdef MMI_PDA_SUPPORT
            item_data.softkey_id[0] = TXT_NULL;
            item_data.softkey_id[1] = TXT_NULL;
            item_data.softkey_id[2] = STXT_RETURN;
#else            
            item_data.softkey_id[0] = TXT_COMMON_OK;
            item_data.softkey_id[1] = TXT_NULL;
            item_data.softkey_id[2] = STXT_RETURN;
#endif

            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            
            lang_name_len = MMIAPICOM_Wstrlen(s_lang_set[i].lang_name);            
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = s_lang_set[i].lang_name; /*lint !e605*/           
            item_data.item_content[0].item_data.text_buffer.wstr_len =  lang_name_len;

            GUILIST_AppendItem(ctrl_id, &item_t);
        }
    }
    
    cur_selection = GetValidIdxByLangIdx((uint32)language_type);

#ifdef MMI_SIM_LANGUAGE_SUPPORT
    //the first row is "auto", not used in input language
    if (cur_selection > 0)
    {
        cur_selection -= 1;
    }
#endif
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}

/*****************************************************************************/
//  Description : 从对应的语言类型得到对应的语言字符串
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_GetInputLanguageString(
										  MMISET_LANGUAGE_TYPE_E language_type,
										  MMI_STRING_T *string_ptr
										  )
{
   // SCI_ASSERT(language_type < MMISET_MAX_LANGUAGE);
	   if(language_type >= MMISET_MAX_LANGUAGE)
	   {
	   	//SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_GetInputLanguageString language_type %d"
	   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2460_112_2_18_2_51_11_197,(uint8*)"d", language_type);
		return;
	   }
    
	//SCI_ASSERT(PNULL != string_ptr);
	   if(PNULL == string_ptr)
	   {
	   	//SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_GetInputLanguageString 2"
	   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2467_112_2_18_2_51_11_198,(uint8*)"");
		return;
	   }
    
    
	string_ptr->wstr_len = MMIAPICOM_Wstrlen(s_lang_set[language_type].lang_name);
	string_ptr->wstr_ptr = s_lang_set[language_type].lang_name;/*lint !e605*/
}

/*****************************************************************************/
//  Description : 添加键盘灯时间到radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendKeyboardLightListItem(
                                               MMI_CTRL_ID_T        ctrl_id
                                               )
{
    uint16      cur_selection   =   0;
    
    //add items to listbox
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_20S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_10S, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_5S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_NIGHT_MODE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    //get current keyboard light
    if (MMISET_KEYBOARD_LIGHT_OPEN == s_keyboard_light_info.keyboard_type
        && MMISET_KEYBOARD_LIGHT_TIME_20 == s_keyboard_light_info.keyboard_light_time)
    {
        cur_selection = 0;
    }
    else if (MMISET_KEYBOARD_LIGHT_OPEN == s_keyboard_light_info.keyboard_type
        && MMISET_KEYBOARD_LIGHT_TIME_10 == s_keyboard_light_info.keyboard_light_time)
    {
        cur_selection = 1;
    }
    else if (MMISET_KEYBOARD_LIGHT_OPEN == s_keyboard_light_info.keyboard_type
        && MMISET_KEYBOARD_LIGHT_TIME_5 == s_keyboard_light_info.keyboard_light_time)
    {
        cur_selection = 2;
    }
    else if (MMISET_KEYBOARD_LIGHT_AUTO == s_keyboard_light_info.keyboard_type)
    {
        cur_selection = 3;      
    }
    else
    {
        cur_selection = 4;
    }
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

/*****************************************************************************/
//  Description : 设置键盘灯选择的结果
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetKeyboardLight(
                                    MMI_CTRL_ID_T       ctrl_id
                                    )
{
    SCI_TIME_T                      sys_time                =   {0};
#ifdef DPHONE_SUPPORT 
    uint16                          cur_selection           =   0;
#else		
    uint32                          cur_selection           =   0;
#endif 
    MMISET_KEYBOARD_LIGHT_SET_T     keyboard_light_info     =   {0};
    
    //get the selected item
#ifdef DPHONE_SUPPORT 
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
#else    
    cur_selection = GUISETLIST_GetCurIndex(ctrl_id);
#endif 
    //SCI_ASSERT((MMISET_KEYBOARD_LIGHT_MAX_ITEM > cur_selection));
   if(MMISET_KEYBOARD_LIGHT_MAX_ITEM <= cur_selection)
   {
   	//SCI_TRACE_LOW:"mmisetting_assert MMISET_SetKeyboardLight cur_selection %d"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2549_112_2_18_2_51_11_199,(uint8*)"d", cur_selection);
	return;
   }
        
    switch (cur_selection)
    {
    case 0:
        keyboard_light_info.keyboard_light_time = MMISET_KEYBOARD_LIGHT_TIME_20;
        keyboard_light_info.keyboard_type       = MMISET_KEYBOARD_LIGHT_OPEN;
        MMISET_SetKeyBoardLightTime(keyboard_light_info);
        MMIDEFAULT_SetKeybadBackLight(TRUE);
        break;
        
    case 1:
        keyboard_light_info.keyboard_light_time = MMISET_KEYBOARD_LIGHT_TIME_10;
        keyboard_light_info.keyboard_type       = MMISET_KEYBOARD_LIGHT_OPEN;
        MMISET_SetKeyBoardLightTime(keyboard_light_info);
        MMIDEFAULT_SetKeybadBackLight(TRUE);
        break;
        
    case 2:
        keyboard_light_info.keyboard_light_time = MMISET_KEYBOARD_LIGHT_TIME_5;
        keyboard_light_info.keyboard_type       = MMISET_KEYBOARD_LIGHT_OPEN;
        MMISET_SetKeyBoardLightTime(keyboard_light_info);
        MMIDEFAULT_SetKeybadBackLight(TRUE);
        break;
        
    case 3:
        keyboard_light_info.keyboard_light_time = MMISET_KEYBOARD_LIGHT_TIME_AUTO;
        keyboard_light_info.keyboard_type       = MMISET_KEYBOARD_LIGHT_AUTO;
        MMISET_SetKeyBoardLightTime(keyboard_light_info);
        //get current system time
        TM_GetSysTime(&sys_time);
        if (sys_time.hour >= MMISET_OPEN_KEYBOARD_LIGHT_MAX_HOUR || sys_time.hour < MMISET_OPEN_KEYBOARD_LIGHT_MIN_HOUR)
        {
            MMIDEFAULT_SetKeybadBackLight(TRUE);
        }
        else
        {
            MMIDEFAULT_SetKeybadBackLight(FALSE);
        }        
        break;
        
    case 4:
        keyboard_light_info.keyboard_light_time = 0;
        keyboard_light_info.keyboard_type       = MMISET_KEYBOARD_LIGHT_CLOSE;
        MMISET_SetKeyBoardLightTime(keyboard_light_info);
        MMIDEFAULT_SetKeybadBackLight(FALSE);
        break;
        
    default:
        //SCI_TRACE_LOW:"MMISET_SetKeyboardLight: cur_selection %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2600_112_2_18_2_51_11_200,(uint8*)"d",cur_selection);
        break;
    }
}

/*****************************************************************************/
//  Description : 添加move style到radio list的item
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendMoveStyleListItem(
                                           MMI_CTRL_ID_T        ctrl_id
                                           )
{
    uint16      cur_selection   =   MMK_GetWinDefaultAnimMoveStyle();
    
    //add items to listbox
    MMIAPISET_AppendListItemByTextId(TXT_SET_FORBIDDEN_MOVE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_SET_HORIZONTAL_MOVE_STYLE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_SET_VERTICAL_MOVE_STYLE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}

/*****************************************************************************/
//  Description : 设置move style选择的结果
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetMoveStyle(
                                MMI_CTRL_ID_T       ctrl_id
                                )
{
    
    uint16                          cur_selection   =   0;
    uint32                          move_style      =   1;
    //get the selected item
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
    
   // SCI_ASSERT((MMISET_MOVE_STYLE_MAX_ITEM > cur_selection));
	if(MMISET_MOVE_STYLE_MAX_ITEM <= cur_selection)
	{
	    //SCI_TRACE_LOW:"mmisetting_assert MMISET_SetMoveStyle cur_selection %d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2645_112_2_18_2_51_11_201,(uint8*)"d", cur_selection);
	    return;
	}   
    switch (cur_selection)
    {
    case 0:
        MMK_SetWinDefaultAnimMoveStyle(MMI_WIN_ANIM_MOVE_SETTING_FORBID);
        move_style = MMI_WIN_ANIM_MOVE_SETTING_FORBID;
        break;
        
    case 1:
        MMK_SetWinDefaultAnimMoveStyle(MMI_WIN_ANIM_MOVE_SETTING_HORIZONTAL);
        move_style = MMI_WIN_ANIM_MOVE_SETTING_HORIZONTAL;
        break;
        
    case 2:
        MMK_SetWinDefaultAnimMoveStyle(MMI_WIN_ANIM_MOVE_SETTING_VERTICAL);
        move_style = MMI_WIN_ANIM_MOVE_SETTING_VERTICAL;
        break;
        
    default:
        //SCI_TRACE_LOW:"MMISET_SetMoveStyle: cur_selection %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2666_112_2_18_2_51_11_202,(uint8*)"d",cur_selection);
        break;
    }
    MMINV_WRITE(MMINV_SET_WINDOW_MOVE_STYLE_INFO, &move_style);
}

/*****************************************************************************/
//  Description : 添加网络选择方式到radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendSelectNetworkListItem(
                                               MN_DUAL_SYS_E dual_sys,
                                               MMI_CTRL_ID_T        ctrl_id
                                               )
{
    uint16                  cur_selection   =   0;
    uint16                  no_seltction    =   0;
    MN_PHONE_SELECT_PLMN_T  sel_net_mode    =   {MN_PHONE_PLMN_SELECT_MANUAL_MODE, 0};
    
    //add items to listbox
#ifdef MMI_DUALMODE_ENABLE    
    MMIAPISET_AppendListItemByTextId(TXT_AUTO,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_MANUAL,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#else
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_AUTO_SELECT,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_SET_MANUAL_SEL,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
    
    //get current select network mode 
    MNPHONE_GetPlmnSelectModeEx(dual_sys, &sel_net_mode);
    //SCI_TRACE_LOW:"MMISET_AppendSelectNetworkListItem:select network mode is %d!,dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2696_112_2_18_2_51_11_203,(uint8*)"dd",sel_net_mode.select_mode,dual_sys);
    
    switch (sel_net_mode.select_mode)
    {
    case MN_PHONE_PLMN_SELECT_AUTO_MODE:
        cur_selection = 0;
        no_seltction = 1;
        break;
        
    case MN_PHONE_PLMN_SELECT_MANUAL_MODE:
        cur_selection = 1;
        no_seltction = 0;
        break;
        
    default:     
        break;
    }
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, no_seltction, FALSE);
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}
#ifdef MMI_VOLTE_SUPPORT
PUBLIC void MMISET_AppendVoLTEOptionItem(
                                         MN_DUAL_SYS_E dual_sys,
                                         MMI_CTRL_ID_T        ctrl_id
                                         )
{
    uint16                  cur_selection   =   0;
    uint16                  no_seltction    =   0;
    uint8                   volte_info      =   0;
    
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_ON,TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_OFF,TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    
    volte_info = MMIAPIPHONE_GetVolteInfo();
    if (volte_info & (1 << dual_sys))
    {
        cur_selection = 0;
        no_seltction = 1;
    }
    else
    {
        cur_selection = 1;
        no_seltction = 0;
    }

    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, no_seltction, FALSE);
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}
#endif
/*****************************************************************************/
//  Description : 设置网络选择方式
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectNetworkMode(
                                        MN_DUAL_SYS_E dual_sys,
                                        MMI_CTRL_ID_T       ctrl_id
                                        )
{
#ifndef MMI_DUALMODE_ENABLE
    uint16                          cur_selection       =   0;
    MN_PLMN_T                       select_plmn         =   {0,0,2};
    MN_PHONE_PLMN_SELECT_MODE_E     sel_net_mode        =   MN_PHONE_PLMN_SELECT_AUTO_MODE;

    if (MMIAPIPHONE_IsNetworkSelecting(dual_sys))
    {
        MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);

        return;
    }
    
    //get the selected item
    cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
    
   // SCI_ASSERT((MMISET_NETWORK_SEL_MAX_ITEM > cur_selection));
    if (MMISET_NETWORK_SEL_MAX_ITEM <= cur_selection)
    {
        //SCI_TRACE_LOW:"mmisetting_assert MMISET_SetSelectNetworkMode cur_selection %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2749_112_2_18_2_51_12_204,(uint8*)"d", cur_selection);
        return;
    }    
    switch (cur_selection)
    {
    case 0://自动选网
        sel_net_mode = MN_PHONE_PLMN_SELECT_AUTO_MODE;

        MNPHONE_SelectPLMNEx(dual_sys, FALSE, select_plmn, MN_GMMREG_RAT_GPRS_AND_3G);
        
        //进入网络连接界面
        MMISET_OpenNetworkConnectWin(dual_sys,
                                                                MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                                                TXT_NETWORK_CONNECT,
                                                                MMIPUB_SOFTKEY_NONE,
                                                                MMISET_HandleSelNetworkConnectWindow);
        break;
        
    case 1://手动选网
        sel_net_mode = MN_PHONE_PLMN_SELECT_MANUAL_MODE;
        //the list of all plmn
        MNPHONE_ListPLMNEx(dual_sys, MN_GMMREG_RAT_GPRS_AND_3G);
        
        //进入网络连接界面
        MMISET_OpenNetworkConnectWin(dual_sys,
                                     MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                     TXT_NETWORK_CONNECT,
                                     MMIPUB_SOFTKEY_ONE,
                                     MMISET_HandleQuitSelNetworkConnectWindow);
        break;
        
    default:
        //SCI_TRACE_LOW:"MMISET_SetSelectNetworkMode: cur_selection %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2781_112_2_18_2_51_12_205,(uint8*)"d",cur_selection);
        break;
    }  
#endif
}

/*****************************************************************************/
//  Description : if app module open privacy protect
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsOpenPrivacyProtect( 
                                              MMISET_PRIVACY_PROTECT_TYPE_E protect_type
                                              )
{
    uint16                      i                       =   0;
    MMISET_PRIVACY_PROTECT_T    privacy_protect_info    =   {0};
    MN_RETURN_RESULT_E          return_value            =   MN_RETURN_FAILURE;
    
    //get if is flip answer
    //SCI_ASSERT(item_index < MMISET_PROTECT_MAX);
   if(protect_type >= MMISET_PROTECT_MAX)
   {
        //SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_IsOpenPrivacyProtect protect_type %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_2803_112_2_18_2_51_12_206,(uint8*)"d", protect_type);
        return FALSE;
    }     

    //如果已经成功输入了一次隐私保护密码，则视同已经验证了密码，直至锁屏
    if (!g_is_need_privacy_validate)
    {
        return FALSE;
    }
    
    MMINV_READ(MMINV_SET_PRIVACY_PROTECT_INFO,&privacy_protect_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        privacy_protect_info.protected_item_num = 0;
        for ( i = 0; i < MMISET_PROTECT_MAX; i++)
        {
            privacy_protect_info.protected_item[i] = FALSE;
        }       
        MMINV_WRITE(MMINV_SET_PRIVACY_PROTECT_INFO,&privacy_protect_info);
    }
    
    return (privacy_protect_info.protected_item[protect_type]);
}

/*****************************************************************************/
//  Description : validate privacy password for app module
//  Global resource dependence : none
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void Set_ValidatePrivacyAppEntry(BOOLEAN is_permitted, DPARAM param_ptr)
{
    if (is_permitted && PNULL != g_app_entry_call_back)
    {
        g_app_entry_call_back();
        
        g_app_entry_call_back = PNULL;
    }
}

/*****************************************************************************/
//  Description : validate privacy password for app module, if you want validate privacy before enter an app, This API is recommended.
//  (推荐使用，模块入口型隐私判断接口，不带参数，不处理失败情形)
//  Global resource dependence : none
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ValidatePrivacyAppEntry(
                                         MMISET_PRIVACY_PROTECT_TYPE_E  protect_type,
                                         MMISET_PRIVACY_APP_ENTRY_CALLBACK_PFUNC app_entry_call_back
                                         )
{
    g_app_entry_call_back = app_entry_call_back;
    MMIAPISET_ValidatePrivacy(protect_type, Set_ValidatePrivacyAppEntry, PNULL, 0);
}

/*****************************************************************************/
//  Description : validate privacy password for app window,  if you want a validate privacy in app window, or you need do some work in failure case of validate privacy, you should use this API.
//  (不推荐使用，只是为了兼容目前平台的一些做法，非入口型隐私判断接口，一般需要带参数，需要处理失败情形)
//  Global resource dependence : none
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ValidatePrivacy(
                                         MMISET_PRIVACY_PROTECT_TYPE_E protect_type,
                                         MMISET_PRIVACY_CALLBACK_PFUNC call_back,
                                         DPARAM param_ptr,
                                         uint32 param_size
                                         )
{
    MMISET_PRIVACY_CALLBACK_DATA* p_call_back_data = PNULL;

    if (PNULL == call_back)
    {
        return ;
    }
    
    if (MMIAPISET_IsOpenPrivacyProtect(protect_type))
    {
        p_call_back_data = SCI_ALLOC_APP(sizeof(MMISET_PRIVACY_CALLBACK_DATA));

        if (PNULL != p_call_back_data)
        {
            SCI_MEMSET((void*)p_call_back_data,0x00,sizeof(MMISET_PRIVACY_CALLBACK_DATA));
            
            p_call_back_data->call_back_func = call_back;
            
            if (PNULL != param_ptr && 0 != param_size)
            {
                p_call_back_data->param_ptr = SCI_ALLOC_APP(param_size + 2);

                if (PNULL != p_call_back_data->param_ptr)
                {
                    SCI_MEMSET((void*)p_call_back_data->param_ptr,0x00,param_size + 2);
                    SCI_MEMCPY((void*)p_call_back_data->param_ptr, (void*)param_ptr, param_size);
                }
            }

            MMISET_OpenVailidatePwdWin(p_call_back_data);
            g_validate_privacy_type = protect_type;
        }
    }
    else
    {
        (*call_back)(TRUE, param_ptr);
    }
}

/*****************************************************************************/
//  Description : close validate privacy password window
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_CloseValidatePrivacyWin(MMISET_PRIVACY_PROTECT_TYPE_E protect_type)
{
    if (MMK_IsOpenWin(MMISET_VALIDATE_PRIVACY_PWD_WIN_ID) && g_validate_privacy_type == protect_type)
    {
        MMK_CloseWin(MMISET_VALIDATE_PRIVACY_PWD_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : check if privacy password window is open
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsValidatePrivacyWinOpen(MMISET_PRIVACY_PROTECT_TYPE_E protect_type)
{
    if (MMK_IsOpenWin(MMISET_VALIDATE_PRIVACY_PWD_WIN_ID) && g_validate_privacy_type == protect_type)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : if privacy validate is needed to do again
//  Global resource dependence : none
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetIsNeedValidatePrivacy(BOOLEAN is_need)
{
    g_is_need_privacy_validate = is_need;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetValidatePrivacyType(void)
{
    g_validate_privacy_type = MMISET_PROTECT_MAX;
}

/*****************************************************************************/
//  Description : 网络设置中，自动搜索和手动搜索从LIST中选择网络的过程中，不允许取消！
//只有在手动搜索 --------------〉网络LIST,这中间才可以取消（包括red key, flip）
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_HandleQuitSelNetworkConnectWindow(
                                                             MMI_WIN_ID_T    win_id,    // 窗口的ID
                                                             MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                             DPARAM             param       // 相应消息的参数
                                                             )
{
    MMI_RESULT_E    result   =  MMI_RESULT_TRUE;
    uint32 i = 0;
    
    switch (msg_id)
    {
    case MSG_APP_CANCEL:       
        //关闭网络连接窗口
        MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
        break;
        
    case MSG_APP_RED:      
        //关闭网络连接窗口
        MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
        
        result = MMI_RESULT_FALSE;
        break;
        
    case MSG_APP_FLIP:
        //关闭网络连接窗口
        MMK_CloseWin(MMISET_SET_CONNECT_NETWORK_WIN_ID);
        
        result = MMI_RESULT_FALSE;
        break;

    case MSG_CLOSE_WINDOW:
        //no matter which card is searching,just both call abort
        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            MNPHONE_AbortPLMNSearchEx((MN_DUAL_SYS_E)i);
        }

        MMIAPISMS_CheckTimerMSG();

        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;

    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 网络设置中，自动搜索和手动搜索从LIST中选择网络的过程中，不允许取消！
//只有在手动搜索 --------------〉网络LIST,这中间才可以取消（包括red key, flip）
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_HandleSelNetworkConnectWindow(
                                                         MMI_WIN_ID_T    win_id,    // 窗口的ID
                                                         MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                         DPARAM             param       // 相应消息的参数
                                                         )
{
    MMI_RESULT_E    result  =   MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED:       
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif        

    case MSG_CLOSE_WINDOW:
        MMIAPISMS_CheckTimerMSG();
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : Open plmn list win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenPlmnListWin(MN_DUAL_SYS_E dual_sys, MN_PHONE_PLMN_INFO_LIST_T    *plmn_detail_list)
{
   // SCI_ASSERT(PNULL != plmn_detail_list);
   if(PNULL == plmn_detail_list)
   {
        //SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_OpenPlmnListWin"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_3052_112_2_18_2_51_12_207,(uint8*)"");
        return;
    } 
    MMK_CreateWin((uint32*)MMISET_SET_PLMN_LIST_WIN_TAB, (ADD_DATA)dual_sys);
    GUILIST_SetMaxItem(MMISET_SET_PLMN_LIST_CTRL_ID, MN_PHONE_MAX_PLMN_INFO_LIST_NUM, FALSE);//max item 16
    AppendPlmnListItem(MMISET_SET_PLMN_LIST_CTRL_ID, plmn_detail_list, dual_sys);  
}

/*****************************************************************************/
//  Description : 添加网络列表到text list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AppendPlmnListItem(
                              MMI_CTRL_ID_T             ctrl_id,
                              MN_PHONE_PLMN_INFO_LIST_T    *plmn_detail_list,
                              MN_DUAL_SYS_E dual_sys
                              )
{
    uint8 i = 0;
    MN_PLMN_T sim_hplmn = {0};
    // MMI_TEXT_ID_T network_name = TXT_NULL;
    
    // 全局变量分配空间
    if (PNULL != g_plmn_detail_list_ptr)
    {
        SCI_FREE(g_plmn_detail_list_ptr);
        g_plmn_detail_list_ptr = PNULL;
    }
    
    g_plmn_detail_list_ptr = SCI_ALLOC_APP(sizeof (MN_PHONE_PLMN_DETAIL_T) * plmn_detail_list->plmn_num);
    if(PNULL == g_plmn_detail_list_ptr)
    {
        //SCI_TRACE_LOW:"mmisetting_assert AppendPlmnListItem"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_3082_112_2_18_2_51_12_208,(uint8*)"");
        return;
    }
    
    if (PNULL != g_rat_list_ptr)
    {
        SCI_FREE(g_rat_list_ptr);
        g_rat_list_ptr = PNULL;
    }
    
    g_rat_list_ptr = SCI_ALLOC_APP(sizeof (MMI_GMMREG_RAT_E) * plmn_detail_list->plmn_num);
    // SCI_ASSERT(PNULL != g_rat_list_ptr);
    if(PNULL == g_rat_list_ptr)
    {
        //SCI_TRACE_LOW:"mmisetting_assert AppendPlmnListItem"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_3094_112_2_18_2_51_12_209,(uint8*)"");
        SCI_FREE(g_plmn_detail_list_ptr);
        g_plmn_detail_list_ptr = PNULL;
        return;
    }     
    
    for (i = 0; i < plmn_detail_list->plmn_num; i++)
    {
        wchar temp_name_str[GUILIST_STRING_MAX_NUM + 1] = {0};
        uint16 temp_name_str_len = GUILIST_STRING_MAX_NUM;
        wchar temp_num_str[GUILIST_STRING_MAX_NUM + 1] = {0};
        uint16 temp_num_str_len = GUILIST_STRING_MAX_NUM;
        GUILIST_ITEM_T item_t = {0};/*lint !e64*/
        GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
        MMI_STRING_T network_str = {0};
        BOOLEAN is_digit_output = FALSE;
        BOOLEAN isGmust = FALSE;
        BOOLEAN isLTEmust = FALSE;
        BOOLEAN isRaticonable = FALSE;
#ifdef DPHONE_SUPPORT //@CR230547 2011.03.04
        uint16 content_index = 0;
#endif
#if defined (PLATFORM_UWS6121E)
        char icon_4Gtype = 0;
#endif

		MMIAPIPHONE_GetSimHPLMN(&sim_hplmn, dual_sys);		
        if ((sim_hplmn.mcc == plmn_detail_list->plmn_details[i].plmn.mcc) 
            && (sim_hplmn.mnc == plmn_detail_list->plmn_details[i].plmn.mnc))
        {            
            MMI_STRING_T spn_string = {0};
            wchar spn[MN_SPN_ID_NUM_LEN + 1] = {0};
            MMISET_LANGUAGE_TYPE_E  language_type;  
            MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();
            
            MMIAPISET_GetLanguageType(&language_type);
            spn_string.wstr_ptr = (wchar *)spn;
            SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
            network_str.wstr_ptr = (wchar *)temp_name_str;
            if (network_name[dual_sys].is_spn_supported &&(0 < network_name[dual_sys].spn_len))
            {
                MMIAPICOM_ParseSimStr(network_name[dual_sys].spn, 
                                      network_name[dual_sys].spn_len,
                                      &spn_string,
                                      MN_SPN_ID_NUM_LEN);
                if (0 < spn_string.wstr_len)
                {
                    MMI_WSTRNCPY(network_str.wstr_ptr,
                                 spn_string.wstr_len,
                                 spn_string.wstr_ptr,
                                 spn_string.wstr_len,
                                 spn_string.wstr_len
                                 );
                    network_str.wstr_len = spn_string.wstr_len;
                }
            }
            
            if ((0 == network_str.wstr_len) 
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
                || (MMISET_LANGUAGE_TRAD_CHINESE == language_type) 
#endif
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
                || (MMISET_LANGUAGE_SIMP_CHINESE == language_type) 
#endif
                || ((460 == sim_hplmn.mcc) && (11 == sim_hplmn.mnc) && (MMISET_LANGUAGE_ENGLISH == language_type)))
            {
                is_digit_output = MMIAPIPHONE_GenPLMNDisplay(
                                                             MMI_GMMREG_RAT_GPRS,
                                                             &plmn_detail_list->plmn_details[i].plmn,
                                                             temp_name_str,
                                                             &temp_name_str_len,
                                                             FALSE);
            }
            else
            {
                MMI_WSTRNCPY(temp_name_str,
                             temp_name_str_len,
                             network_str.wstr_ptr,
                             temp_name_str_len,
                             network_str.wstr_len);
            }
        }
        else
        {
            is_digit_output = MMIAPIPHONE_GenPLMNDisplay(
                                                         MMI_GMMREG_RAT_GPRS,
                                                         &plmn_detail_list->plmn_details[i].plmn,
                                                         temp_name_str,
                                                         &temp_name_str_len,
                                                         FALSE);
        }
        
        SCI_TRACE_LOW("AppendPlmnListItem dual_sys:%d temp_name_str:%s, is_digit_output=%d, mcc=%d, mnc=%d", dual_sys, temp_name_str, is_digit_output, 
                      plmn_detail_list->plmn_details[i].plmn.mcc, plmn_detail_list->plmn_details[i].plmn.mnc);
                                                     
        isRaticonable = MMIAPICOM_IsExistInRatIconTable(&plmn_detail_list->plmn_details[i].plmn, &isGmust, &isLTEmust);
#ifdef DPHONE_SUPPORT //@CR230547 2011.03.04

        item_t.item_data_ptr = &item_data;
        //set softkey
        item_data.softkey_id[0] = TXT_COMMON_OK;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;

        if (!plmn_detail_list->plmn_details[i].plmn_is_forbidden)
        {
            if (MN_GMMREG_RAT_GPRS_AND_3G == plmn_detail_list->rat[i])
            {
                item_data.item_content[content_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST                
                item_data.item_content[content_index].item_data.image_id = IMAGE_SET_ALLOWABLE_2G;
#else
                item_data.item_content[content_index].item_data.image_id = IMAGE_NULL;
#endif
                content_index++;
                item_data.item_content[content_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST                   
                item_data.item_content[content_index].item_data.image_id = IMAGE_SET_ALLOWABLE_3G;
#else
                item_data.item_content[content_index].item_data.image_id = IMAGE_NULL;
#endif
                content_index++;
            }
            else if (MN_GMMREG_RAT_3G == plmn_detail_list->rat[i])
            {
                item_data.item_content[content_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST                
                item_data.item_content[content_index].item_data.image_id = IMAGE_SET_ALLOWABLE_3G;
#else
		  item_data.item_content[content_index].item_data.image_id = IMAGE_NULL;
#endif
                content_index++;
            }
            else if (MN_GMMREG_RAT_GPRS == plmn_detail_list->rat[i])
            {
                item_data.item_content[content_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST                 
                item_data.item_content[content_index].item_data.image_id = IMAGE_SET_ALLOWABLE_2G;
#else
                item_data.item_content[content_index].item_data.image_id = IMAGE_NULL;
#endif
                content_index++;
            }
        }
        else
        {
            if (MN_GMMREG_RAT_GPRS_AND_3G == plmn_detail_list->rat[i])
            {
                item_data.item_content[content_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
 #ifndef MMI_RES_LOW_COST               
                item_data.item_content[content_index].item_data.image_id = IMAGE_SET_FORBIDDEN_2G;
 #else
 		  item_data.item_content[content_index].item_data.image_id = IMAGE_NULL;
 #endif
                content_index++;
                item_data.item_content[content_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST                
                item_data.item_content[content_index].item_data.image_id = IMAGE_SET_FORBIDDEN_3G;
#else
		  item_data.item_content[content_index].item_data.image_id = IMAGE_NULL;
#endif
                content_index++;
            }
            else if (MN_GMMREG_RAT_3G == plmn_detail_list->rat[i])
            {
                item_data.item_content[content_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST                
                item_data.item_content[content_index].item_data.image_id = IMAGE_SET_FORBIDDEN_3G;
#else
		  item_data.item_content[content_index].item_data.image_id = IMAGE_NULL;
#endif
                content_index++;
            }
            else if (MN_GMMREG_RAT_GPRS == plmn_detail_list->rat[i])
            {
                item_data.item_content[content_index].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST                
                item_data.item_content[content_index].item_data.image_id = IMAGE_SET_FORBIDDEN_2G;
#else
		  item_data.item_content[content_index].item_data.image_id = IMAGE_NULL;
#endif
                content_index++;
            }
        }
        item_data.item_content[content_index].item_data_type  = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[content_index].item_data.text_buffer.wstr_len =  temp_name_str_len;
        item_data.item_content[content_index].item_data.text_buffer.wstr_ptr = (wchar *)temp_name_str;
        content_index++;
        if (!is_digit_output)
        {
            MMIAPIPHONE_GenPLMNDisplay(
                                                    MMI_GMMREG_RAT_GPRS,
                                                    &plmn_detail_list->plmn_details[i].plmn,
                                                    temp_num_str,
                                                    &temp_num_str_len,
                                                    TRUE);
            item_data.item_content[content_index].item_data_type  = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[content_index].item_data.text_buffer.wstr_len =  temp_num_str_len;
            item_data.item_content[content_index].item_data.text_buffer.wstr_ptr = (wchar *)temp_num_str;
            content_index++;
             if (MN_GMMREG_RAT_GPRS_AND_3G == plmn_detail_list->rat[i])
             {
                item_t.item_style = GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT;
             }
             else
             {
                item_t.item_style = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;
             }
        }
        else
        {
             if (MN_GMMREG_RAT_GPRS_AND_3G == plmn_detail_list->rat[i])
             {
                item_t.item_style = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;
             }
             else
             {
                item_t.item_style = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;
             }
        }        
#else
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_2ANIM_AND_TEXT;
        item_t.item_data_ptr = &item_data;

        //set softkey
        item_data.softkey_id[0] = TXT_COMMON_OK;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;
        
        item_data.item_content[0].item_data_type  = GUIITEM_DATA_TEXT_BUFFER;

        item_data.item_content[0].item_data.text_buffer.wstr_len =  temp_name_str_len;

        item_data.item_content[0].item_data.text_buffer.wstr_ptr = (wchar *)temp_name_str;

        if (!plmn_detail_list->plmn_details[i].plmn_is_forbidden)
        {
            if (MN_GMMREG_RAT_GPRS_AND_3G == plmn_detail_list->rat[i])
            {
                item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST                
                item_data.item_content[1].item_data.image_id = IMAGE_SET_ALLOWABLE_2G;
#else
                item_data.item_content[1].item_data.image_id = IMAGE_NULL;
#endif
                item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST 
                item_data.item_content[2].item_data.image_id = IMAGE_SET_ALLOWABLE_3G;
#else
                item_data.item_content[2].item_data.image_id = IMAGE_NULL;
#endif
            }
            else if (MN_GMMREG_RAT_3G == plmn_detail_list->rat[i])
            {
                item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST                 
                item_data.item_content[2].item_data.image_id = IMAGE_SET_ALLOWABLE_3G;
#else
                item_data.item_content[2].item_data.image_id = IMAGE_NULL;
#endif
            }
            else if (MN_GMMREG_RAT_GPRS == plmn_detail_list->rat[i])
            {
                item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST                 
                item_data.item_content[2].item_data.image_id = IMAGE_SET_ALLOWABLE_2G;
#else
                item_data.item_content[2].item_data.image_id = IMAGE_NULL;
#endif
            }
            
#if defined (MODEM_SUPPORT_SPRD)
            else if (MN_GMMREG_RAT_LTE == plmn_detail_list->rat[i])
            {
                item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST
                if (TRUE == isRaticonable)
                {
                    if (TRUE == isLTEmust)
                    {
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_ALLOWABLE_LTE;
                    }
                    else
                    {
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_ALLOWABLE_4G;
                    }
                }
                else
                {
                    if (MMIAPIPHONE_4g_icon_display_style() == 1)
                    {
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_ALLOWABLE_LTE;
                    }
                    else
                    {       
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_ALLOWABLE_4G;
                    }       
                }
#else
                item_data.item_content[2].item_data.image_id = IMAGE_NULL;
#endif
            }
#elif defined (MODEM_SUPPORT_RDA)
#ifdef MMI_2G_4G_SUPPORT
            else if (MN_GMMREG_RAT_4G == plmn_detail_list->rat[i])
            {
                item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST
                if (TRUE == isRaticonable)
                {
                    if (1 == icon_4Gtype)
                    {
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_ALLOWABLE_LTE;
                    }
                    else
                    {
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_ALLOWABLE_4G;
                    }
                }
                else
                {
                    if (MMIAPIPHONE_4g_icon_display_style() == 1)
                    {
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_ALLOWABLE_LTE;
                    }
                    else
                    {
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_ALLOWABLE_4G;
                    }
                }
#else
                item_data.item_content[2].item_data.image_id = IMAGE_NULL;
#endif
            }
#endif
#else
    #error
#endif
            else
            {
                /*
                  此处需要确认后续底层是否会上报MN_GMMREG_RAT_GPRS_3G_LTE类型的网络，
                  并且要根据需求确认是否更改控件GUIITEM_STYLE_TWO_LINE_TEXT_2ANIM_AND_TEXT为3个图标
                */
            }
        }
        else
        {
            if (MN_GMMREG_RAT_GPRS_AND_3G == plmn_detail_list->rat[i])
            {
                item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST                 
                item_data.item_content[1].item_data.image_id = IMAGE_SET_FORBIDDEN_2G;
#else
                item_data.item_content[1].item_data.image_id = IMAGE_NULL;
#endif

                item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST 
                item_data.item_content[2].item_data.image_id = IMAGE_SET_FORBIDDEN_3G;
#else
                item_data.item_content[2].item_data.image_id = IMAGE_NULL;
#endif
            }
            else if (MN_GMMREG_RAT_3G == plmn_detail_list->rat[i])
            {
                item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST 
                item_data.item_content[2].item_data.image_id = IMAGE_SET_FORBIDDEN_3G;
#else
                item_data.item_content[2].item_data.image_id = IMAGE_NULL;
#endif
            }
            else if (MN_GMMREG_RAT_GPRS == plmn_detail_list->rat[i])
            {
                item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST 
                item_data.item_content[2].item_data.image_id = IMAGE_SET_FORBIDDEN_2G;
#else
                item_data.item_content[2].item_data.image_id = IMAGE_NULL;
#endif
            }
#if defined (MODEM_SUPPORT_SPRD)
            else if (MN_GMMREG_RAT_LTE == plmn_detail_list->rat[i])
            {
                item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST
                if (TRUE == isRaticonable)
                {
                    if (TRUE == isLTEmust)
                    {
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_FORBIDDEN_LTE;
                    }
                    else
                    {
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_FORBIDDEN_4G;
                    }
                }
                else
                {
                    if (MMIAPIPHONE_4g_icon_display_style() == 1)
                    {
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_FORBIDDEN_LTE;
                    }
                    else
                    {
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_FORBIDDEN_4G;
                    }
                }
#else
                item_data.item_content[2].item_data.image_id = IMAGE_NULL;
#endif
            }
#elif defined (MODEM_SUPPORT_RDA)
#ifdef MMI_2G_4G_SUPPORT
            else if (MN_GMMREG_RAT_4G == plmn_detail_list->rat[i])
            {
                item_data.item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
#ifndef MMI_RES_LOW_COST
                if (TRUE == isRaticonable)
                {
                    if (1 == icon_4Gtype)
                    {
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_FORBIDDEN_LTE;
                    }
                    else
                    {
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_FORBIDDEN_4G;
                    }
                }
                else
                {
                    if (MMIAPIPHONE_4g_icon_display_style() == 1)
                    {
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_FORBIDDEN_LTE;
                    }
                    else
                    {
                        item_data.item_content[2].item_data.image_id = IMAGE_SET_FORBIDDEN_4G;
                    }
                }
#else
                item_data.item_content[2].item_data.image_id = IMAGE_NULL;
#endif
            }
#endif
#else
    #error
#endif
            else
            {
                /*
                  此处需要确认后续底层是否会上报MN_GMMREG_RAT_GPRS_3G_LTE类型的网络，
                  并且要根据需求确认是否更改控件GUIITEM_STYLE_TWO_LINE_TEXT_2ANIM_AND_TEXT为3个图标
                */
            }
        }
    
//        item_data.item_content[1].item_data_type  = GUIITEM_DATA_IMAGE_ID;
//        if (plmn_detail_list->plmn_details[i].plmn_is_registered)
//        {
//            item_data.item_content[1].item_data.image_id = IMAGE_CONTROL_LIST_CHECK_SELECTED_ICON;
//        }
//        else
//        {
//            item_data.item_content[1].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;
//        }

        if (!is_digit_output)
        {
            MMIAPIPHONE_GenPLMNDisplay(
                                       MMI_GMMREG_RAT_GPRS,
                                       &plmn_detail_list->plmn_details[i].plmn,
                                       temp_num_str,
                                       &temp_num_str_len,
                                       TRUE);

            item_data.item_content[3].item_data_type  = GUIITEM_DATA_TEXT_BUFFER;

            item_data.item_content[3].item_data.text_buffer.wstr_len =  temp_num_str_len;

            item_data.item_content[3].item_data.text_buffer.wstr_ptr = (wchar *)temp_num_str;
        }
#endif 
        GUILIST_AppendItem( ctrl_id, &item_t );

        //set gloable variable value
        g_plmn_detail_list_ptr[i] = plmn_detail_list->plmn_details[i];

        g_rat_list_ptr[i] = plmn_detail_list->rat[i];/*lint !e64*/
    }
}

/*****************************************************************************/
//  Description : Set the mode of anykey and reject
//  Global resource dependence : 
//  Author:kelly.li
//  Note:  
/*****************************************************************************/
PUBLIC void MMISET_SetAnykeyRejectMode(
                                       MMI_CTRL_ID_T ctrl_id
                                       )
{
    uint16                      i   = 0,selected_num = 0;
    uint16                      selected_item[MMISET_SET_ANYKEY_REJECT_MAX_ITEM]    =   {0};
    MMISET_ANYKEY_REJECT_SET_T  mode_info                                           =   {0};    
    
    //get current selected items
    selected_num = GUILIST_GetSelectedItemIndex(ctrl_id,selected_item,MMISET_SET_ANYKEY_REJECT_MAX_ITEM);
    
    for (i=0; i<selected_num; i++)
    {
        switch (selected_item[i])
        {
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
        case 0:
            mode_info.is_sms_reply_open  = TRUE;
            break;
            
        case 1:
            mode_info.is_anykey_answer = TRUE;
            break;
#else
        case 0:
            mode_info.is_anykey_answer = TRUE;
            break;
#endif

        default:
            //SCI_TRACE_LOW:"MMISET_SetAnykeyRejectMode: selected item is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_3336_112_2_18_2_51_13_210,(uint8*)"");
            break;
        }
    }
    MMINV_WRITE(MMINV_SET_ANYKEY_REJECT_MODE,&mode_info);
}

/*****************************************************************************/
//  Description : 选择指定的网络
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SelectNetwork(
                                 MN_DUAL_SYS_E dual_sys,
                                 MMI_CTRL_ID_T      ctrl_id
                                 )
{
    uint16 cur_selection = 0;
    
    //get the current item
    cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
    
    //SCI_ASSERT((MN_PHONE_MAX_PLMN_INFO_LIST_NUM > cur_selection));
    //SCI_ASSERT(PNULL != g_plmn_detail_list_ptr);
    if (MN_PHONE_MAX_PLMN_INFO_LIST_NUM <= cur_selection || PNULL == g_plmn_detail_list_ptr)
    {
        //SCI_TRACE_LOW:"mmisetting_assert MMISET_SelectNetwork cur_selection %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_3362_112_2_18_2_51_13_211,(uint8*)"d", cur_selection);
        return;
    }    
    
    //SCI_TRACE_LOW:"MMISET_SelectNetwork: mcc = %d, mnc = %d, mnc_digit_num = %d, select dual sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_3370_112_2_18_2_51_13_212,(uint8*)"dddd", g_plmn_detail_list_ptr[cur_selection].plmn.mcc,g_plmn_detail_list_ptr[cur_selection].plmn.mnc,g_plmn_detail_list_ptr[cur_selection].plmn.mnc_digit_num,dual_sys);
    
    //select network;
#ifdef MMI_DUALMODE_ENABLE
    if (ERR_MNPHONE_NO_ERR != MNPHONE_SelectPLMNEx(dual_sys, TRUE,g_plmn_detail_list_ptr[cur_selection].plmn, g_rat_list_ptr[cur_selection]))/*lint !e64*/
#else
    if (ERR_MNPHONE_NO_ERR != MNPHONE_SelectPLMNEx(dual_sys, TRUE,g_plmn_detail_list_ptr[cur_selection].plmn, MN_GMMREG_RAT_GPRS))
#endif
    {
        //SCI_TRACE_LOW:"MMISET_SelectNetwork: MNPHONE_SelectPLMN fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_3379_112_2_18_2_51_13_213,(uint8*)"");
        MMIPUB_OpenAlertSuccessWin(TXT_ERROR);
        return;
    }  
    
    //进入网络连接界面
    MMISET_OpenNetworkConnectWin(dual_sys,
                                 MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                 TXT_SET_NETWORK_REGISTERING,
                                 MMIPUB_SOFTKEY_NONE,
                                 MMISET_HandleSelNetworkConnectWindow);
                                 
    return;                                                         
}

/*****************************************************************************/
//  Description : 设置模块中是否有限制恢复MP3的窗口打开
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPTSET_ExitedWinForbidResumingMp3(void)
{
    return( MMK_IsFocusWin(MMISET_RING_SELECT_FIXED_WIN_ID) ||
        MMK_IsFocusWin(MMISET_RING_SELECT_DOWNLOAD_WIN_ID) ||
        MMK_IsFocusWin(MMISET_RING_SELECT_MP3_WIN_ID) ||
        MMK_IsFocusWin(MMISET_RING_SELECT_RECORD_WIN_ID) ||
        MMK_IsFocusWin(MMISET_RING_SELECT_MSG_WIN_ID)
        );
}

/*****************************************************************************/
//  Description : how to process sd card plug
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
//PUBLIC void MMIAPISET_ProcessSDPlug(void)
//{
//    MMK_CloseWin(MMISET_RING_SELECT_DOWNLOAD_WIN_ID);
//    MMK_CloseWin(MMISET_RING_SELECT_MP3_WIN_ID);
//    MMK_CloseWin(MMISET_RING_SELECT_RECORD_WIN_ID);
// }

/*****************************************************************************/
//  Description : 添加频段选择方式到list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendSelectBansListItem(
                                            MMI_CTRL_ID_T   ctrl_id
                                            )
{
    uint16                  cur_selection = 0,no_seltction = 0;
    MN_PHONE_BAND_TYPE_E    band_type       = MN_PHONE_BAND_GSM;
    
    //add items to listbox
    MMIAPISET_AppendListItemByTextId( TXT_SET_PCS_BAND, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_DUAL_BAND, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    //get current select band type
    band_type = MNPHONE_GetBandEx(MMIAPISET_GetActiveSim());    
    
    switch (band_type)
    {
    case MN_PHONE_BAND_PCS:
        cur_selection = 0;
        no_seltction = 1;
        break;
        
    case MN_PHONE_BAND_GSM_DCS:
        cur_selection = 1;
        no_seltction = 0;
        break;
        
    default:
        break;
    }
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

/*****************************************************************************/
//  Description : 设置频段选择方式
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetSelectBandType(
                                        MMI_CTRL_ID_T       ctrl_id
                                        )
{
    BOOLEAN                 result              =   FALSE;
    uint16                  cur_selection       =   0;
    MN_PHONE_BAND_TYPE_E    band_type           =   MN_PHONE_BAND_GSM;
    
    //get the selected item
    cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
    
    //SCI_ASSERT((MMISET_BAND_SEL_MAX_ITEM > cur_selection));
   if(MMISET_BAND_SEL_MAX_ITEM <= cur_selection)
   {
        //SCI_TRACE_LOW:"mmisetting_assert MMISET_SetSelectBandType cur_selection %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_3476_112_2_18_2_51_13_214,(uint8*)"d", cur_selection);
        return FALSE;
    }    
    switch (cur_selection)
    {
    case 0://PCS band
        band_type = MN_PHONE_BAND_PCS;
        break;
        
    case 1://Dual band
        band_type = MN_PHONE_BAND_GSM_DCS;
        break;
        
    default:
        //SCI_TRACE_LOW:"MMISET_SetSelectBandType: cur_selection %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_3490_112_2_18_2_51_13_215,(uint8*)"d",cur_selection);
        break;
    }
    
    if (ERR_MNPHONE_NO_ERR == MNPHONE_SelectBandEx(MMIAPISET_GetActiveSim(), band_type))
    {
        result = TRUE;
        //进入网络连接界面
        MMISET_OpenNetworkConnectWin(MMIAPISET_GetActiveSim(),
                                                                MMISET_SET_CONNECT_NETWORK_WIN_ID,
                                                                TXT_NETWORK_CONNECT,
                                                                MMIPUB_SOFTKEY_NONE,
                                                                MMISET_HandleSelNetworkConnectWindow);
    }
    
    return (result);
}



/*****************************************************************************/
//  Description : 设置开机密码开启或者关闭
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetPowerOnPwdStatus(
                                          BOOLEAN  is_open
                                          )
{
    MMISET_POWER_ON_PWD_T   phone_code      =   {0};
//    MN_RETURN_RESULT_E      return_value    =   MN_RETURN_FAILURE;
    
    InitPhonePwd(&phone_code);
    
    phone_code.is_enable = is_open;
    //write power on password into NV
    MMINV_WRITE(MMINV_SET_POWER_ON_PWD_INFO,&phone_code);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 获得开机密码长度，用来判断开机密码是否已经设置
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsSetPowerOnPwd(void)
{
    BOOLEAN                 result          =   FALSE;
    MMISET_POWER_ON_PWD_T   phone_code      =   {0};
//    MN_RETURN_RESULT_E      return_value    =   MN_RETURN_FAILURE;
    
    InitPhonePwd(&phone_code);
    
    if (0 < phone_code.pwd_len)
    {
        result = TRUE;
    }
    
    return (result);
}


/*****************************************************************************/
//  Description : get is auto boardkey
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC MMISET_KEYLOCK_TYPE_E MMIAPISET_GetAutoBoardKey(void)
{
#ifdef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
    MMISET_KEYLOCK_TYPE_E  auto_keylock_time = MMISET_DEFAULT_KEYLOCK_TIME;
    MMISET_BACKLIGHT_SET_T back_light = {0};

    MMIAPISET_GetBackLightInfo(&back_light);

    if (MMISET_BACKLIGHT_OPEN_FOREVER == back_light.backlight_type)
    {
        auto_keylock_time = MMISET_KEYLOCK_CLOSE;
    }
    else
    {
        switch (back_light.backlight_time)
        {
        case MMISET_TIME_15:
            auto_keylock_time = MMISET_KEYLOCK_15_SEC;
            break;

        case MMISET_TIME_30:
            auto_keylock_time = MMISET_KEYLOCK_30_SEC;
            break;

        case MMISET_TIME_60 :
            auto_keylock_time = MMISET_KEYLOCK_1_MIN;
            break;

        case MMISET_TIME_300:
            auto_keylock_time = MMISET_KEYLOCK_5_MIN;
            break;

        default:
            auto_keylock_time = MMISET_KEYLOCK_15_SEC;
            break;
        }
    }

    return (auto_keylock_time);
#else
    MMISET_KEYLOCK_TYPE_E   auto_keylock_time   =   MMISET_DEFAULT_KEYLOCK_TIME;
    MN_RETURN_RESULT_E      return_value        =    MN_RETURN_FAILURE;
    
    //get if is auto keyboard
    MMINV_READ(MMINV_SET_AUTO_KEYLOCK_TIME,&auto_keylock_time,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        auto_keylock_time = MMISET_DEFAULT_KEYLOCK_TIME;
        MMINV_WRITE(MMINV_SET_AUTO_KEYLOCK_TIME,&auto_keylock_time);
    }
    
    return (auto_keylock_time);
#endif
}

/*****************************************************************************/
//  Description : set is auto board key
//  Global resource dependence : none
//  Author: jibin
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_SetAutoBoardKey(MMISET_KEYLOCK_TYPE_E auto_boardkey_time) 
{
#ifdef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
    MMISET_BACKLIGHT_SET_T back_light = {0};

    back_light.backlight_type = MMISET_BACKLIGHT_OPEN;

    switch (auto_boardkey_time)
    {
    case MMISET_KEYLOCK_CLOSE:
        back_light.backlight_type = MMISET_BACKLIGHT_OPEN_FOREVER;
        break;

    case MMISET_KEYLOCK_15_SEC:
        back_light.backlight_time = MMISET_TIME_15;
        break;

    case MMISET_KEYLOCK_30_SEC:
        back_light.backlight_time = MMISET_TIME_30;
        break;

    case MMISET_KEYLOCK_1_MIN:
        back_light.backlight_time = MMISET_TIME_60;
        break;

    case MMISET_KEYLOCK_5_MIN:
        back_light.backlight_time = MMISET_TIME_300;
        break;

    default:
        back_light.backlight_time = MMISET_TIME_15;
        break;
    }

    MMIAPISET_SetBackLightInfo(back_light);
#else
    MMINV_WRITE(MMINV_SET_AUTO_KEYLOCK_TIME,&auto_boardkey_time);
#endif
}

/*****************************************************************************/
//   FUNCTION:      MMIAPISET_InitModule
//  Description :   
//  Global resource dependence : 
//   Author:        kelly.li
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPISET_InitModule(void)
{
    MMISET_RegMenuGroup();  
    MMISET_RegNv();  
    MMISET_RegWinIdNameArr(); 
}

/*****************************************************************************/
// Description : get fly mode
// Global resource dependence :
// return : FALSE: fly mode off; TRUE :fly mode on; 
// Author: baokun.yin
// Note:2007-8-21
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetFlyMode(void)
{
    return s_is_fly_mode_on;
}


/*****************************************************************************/
// Description : get fly mode Set Power On
// Global resource dependence :
// return :  
// Author: baokun.yin
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetFlyModeSetPowerOn(void)
{
    MN_RETURN_RESULT_E  return_value            =   MN_RETURN_FAILURE;
    BOOLEAN             is_fly_mode_power_on    =   FALSE;
    
    MMINV_READ(MMINV_FLY_MODE_OPEN_PHONE,&is_fly_mode_power_on,return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        is_fly_mode_power_on = FALSE;
        MMINV_WRITE(MMINV_FLY_MODE_OPEN_PHONE,&is_fly_mode_power_on);
    }

    //SCI_TRACE_LOW:"MMIAPISET_GetFlyModeSetPowerOn is_fly_mode_power_on=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_3702_112_2_18_2_51_13_216,(uint8*)"d", is_fly_mode_power_on);

    return is_fly_mode_power_on;
}

/*****************************************************************************/
//  Description : get default wallpaper id by style
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISET_GetDefaultWallpaperIDByStyle(void)
{
    uint16 wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID;

//#ifdef MMI_GRID_IDLE_SUPPORT
//    if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
//    {
//        wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID + 1; 
//    }
//    else
//#endif
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT 
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID + 1;
    }
//    else
 #endif  
//    {
//        wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID;
//     }

    return wallpaper_id;
}

/*****************************************************************************/
//  Description : reset pic wallpaper setting
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL void ResetWallpaperSetting(void)
{
    MMISET_WALLPAPER_SET_T  wallpaper_info  =   {0};
    
    SCI_MEMSET(&wallpaper_info,0,sizeof(MMISET_WALLPAPER_SET_T));    
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info); 

    wallpaper_info.wallpaper_id = MMIAPISET_GetDefaultWallpaperIDByStyle();       
    wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;

    MMIAPISET_SetWallpaperSetInfo(wallpaper_info);
}

/*****************************************************************************/
//  Description : get is red keylock
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetRedKeyLock(void)
{
    BOOLEAN                 is_red_keylock = FALSE;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    //get if is auto keyboard
    MMINV_READ(MMINV_SET_IS_RED_KEYLOCK,&is_red_keylock,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_red_keylock = FALSE;
        MMINV_WRITE(MMINV_SET_IS_RED_KEYLOCK,&is_red_keylock);
    }
    
    return (is_red_keylock);
}

/*****************************************************************************/
//  Description : set is red key lock
//  Global resource dependence : none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMIAPISET_SetRedKeyLock(BOOLEAN is_red_keylock) 
{
    MMINV_WRITE(MMINV_SET_IS_RED_KEYLOCK,&is_red_keylock);
}


/*****************************************************************************/
//  Description : 是否需要开机时设置时间日期
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsNeedResetDateTime(void)
{

    BOOLEAN                 is_need         =   FALSE;
    MN_RETURN_RESULT_E      return_value    =   MN_RETURN_FAILURE;   
    BOOLEAN                 is_rtcAbnormal  =   FALSE;

#ifndef WIN32
    is_rtcAbnormal = TM_IsRtcAbnormal();
    //SCI_TRACE_LOW:"MMIAPISET_IsNeedResetDateTime is_rtcAbnormal = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_3796_112_2_18_2_51_14_217,(uint8*)"d",is_rtcAbnormal);
#endif    //read power on password from
    MMINV_READ(MMINV_SET_NEED_SET_DATETIME,&is_need, return_value);
    if ((MN_RETURN_SUCCESS != return_value)||is_rtcAbnormal)
    {
        is_need = TRUE;
        MMINV_WRITE(MMINV_SET_NEED_SET_DATETIME,&is_need);
    }
    SCI_TRACE_LOW("MMIAPISET_IsNeedResetDateTime is_need = %d",is_need);
    return (is_need);
}
/*****************************************************************************/
//  Description : 设置是否需要开机时设置时间日期
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPISET_SetNeedResetDateTime(BOOLEAN  is_need)
{
   MMINV_WRITE(MMINV_SET_NEED_SET_DATETIME, &is_need);   
}
#ifndef WIN32
/*****************************************************************************/
//  Description : get mul-language nv config info 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void  GetMulNVConfigInfo(MMI_MULCONFIG_INFO_T * config_info)
{
    NVITEM_ERROR_E reslut = NVERR_NONE;
    
    reslut = EFS_NvitemRead(NV_MULTI_LANGUE_CFG,(uint16)sizeof(MMI_MULCONFIG_INFO_T),(uint8*)config_info);

    if (NVERR_NONE != reslut)
    {
        //SCI_TRACE_LOW:"GetMulNVConfigInfo fail! reslut = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_3827_112_2_18_2_51_14_218,(uint8*)"d",reslut);
    }
}
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jian.ma
//  Note: TRUE: language ready; FALSE: language not ready
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_CheckLanguageStatus(MMISET_LANGUAGE_TYPE_E language_type)
{
#ifndef WIN32
    MMI_MULCONFIG_INFO_T    config_info = {0};
#endif
    BOOLEAN                 result = FALSE;

	if(language_type >= MMISET_MAX_LANGUAGE)
	{
		return FALSE;
	}
	
    switch(language_type)
    {
#ifdef MMI_SIM_LANGUAGE_SUPPORT
        case MMISET_LANGUAGE_AUTO:
#endif
        case MMISET_LANGUAGE_ENGLISH:
#ifdef MMI_DISPLAY_SIMP_CHINESE_SUPPORT
        case MMISET_LANGUAGE_SIMP_CHINESE:
#endif
#ifdef MMI_DISPLAY_TRAD_CHINESE_SUPPORT
        case MMISET_LANGUAGE_TRAD_CHINESE: // Bug 1298880 - [TJ][MPT][T117][多国语言][PTR1Block]繁体中文未打开
#endif
            result = s_lang_set[language_type].lang_status;
            break;
        default:
            {       
#ifdef WIN32
				if(TRUE == s_lang_set[language_type].lang_status)
#else
                GetMulNVConfigInfo(&config_info);
                if((TRUE == s_lang_set[language_type].lang_status) && (config_info.flag[language_type]>>2)& 1)
#endif
                {
                    result = TRUE; 
                }
                break;
            }
    }
    //SCI_TRACE_LOW:"MMIAPISET_CheckLanguageStatus language_type = %d,result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_3871_112_2_18_2_51_14_219,(uint8*)"dd",language_type,result);

    return result;
}

/*****************************************************************************/
// Description : Get Is Allow Open Query Fly mode
// Global resource dependence :
// return :  
// Author: wancan.you
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_GetIsQueryOpenFlyMode(void)
{
    BOOLEAN is_query = FALSE;


    if (MMIAPIMMS_IsMMSWaitingDownload()
#ifdef DL_SUPPORT
        || MMIAPIDL_GetDownloadStatus()
#endif
        || MMIAPIDCD_IsDownloadingData()
        )
    {
        is_query = TRUE;
    }

    return is_query;
}

/*****************************************************************************/
// Description : Stop PS Application
// Global resource dependence :
// return :  
// Author: wancan.you
// Note:
/*****************************************************************************/
PUBLIC void MMISET_StopPSApplication(void)
{
#ifdef DL_SUPPORT
    if (MMIAPIDL_GetDownloadStatus())
    {
        MMIAPIDL_StopDownload(0, TRUE);

        SCI_Sleep(100);
    }
#endif
    MMIAPIMMS_StopAutodownMMS();
}
#if 0
/*****************************************************************************/
//  Description : 添加语言选择的dropdownlist的item
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendShowLanguageItem(
                                          MMI_CTRL_ID_T     ctrl_id
                                          )
{
    
    uint16                  cur_selection   =   0;
    MMISET_LANGUAGE_TYPE_E  language_type   =   MMISET_MAX_LANGUAGE;
    int32                   i               =   0;
	uint8 lang_name_len = 0; 
	MMI_STRING_T item[MMISET_MAX_LANGUAGE] = {0};
	uint16 itemTotalNum=0;
    
    for(i = 0;i < MMISET_MAX_LANGUAGE;i++)
    {    
    	if(MMIAPISET_CheckLanguageStatus((MMISET_LANGUAGE_TYPE_E)i))  
        {
            lang_name_len = MMIAPICOM_Wstrlen(s_lang_set[i].lang_name);
            
            item[itemTotalNum].wstr_ptr = s_lang_set[i].lang_name;  /*lint !e605*/
            item[itemTotalNum].wstr_len =  lang_name_len;
			itemTotalNum++;
		}
    }
	GUIDROPDOWNLIST_AppendItemArray(ctrl_id, item, itemTotalNum);

    MMIAPISET_GetLanguageType(&language_type);
    //SCI_ASSERT(language_type < MMISET_MAX_LANGUAGE);
   if(language_type >= MMISET_MAX_LANGUAGE)
   {
        //SCI_TRACE_LOW:"mmisetting_assert MMISET_AppendShowLanguageItem language_type %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_3950_112_2_18_2_51_14_220,(uint8*)"d", language_type);
        return;
    }     
    cur_selection = GetValidIdxByLangIdx((uint32)language_type);
    
    //set current item
    GUIDROPDOWNLIST_SetCurItemIndex(ctrl_id,cur_selection);
    
}

/*****************************************************************************/
//  Description : 添加语言输入的dropdownlist的item
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendInputLanguageItem(
											   MMI_CTRL_ID_T ctrl_id, 
											   MMISET_LANGUAGE_TYPE_E language_type
											   )
{
    uint16 cur_selection = 0;
    int32 i = 0;
	uint8 lang_name_len = 0; 
	MMI_STRING_T item[MMISET_MAX_LANGUAGE] = {0};
	uint16 itemTotalNum=0;

    //SCI_ASSERT(language_type < MMISET_MAX_LANGUAGE);
   if(language_type >= MMISET_MAX_LANGUAGE)
   {
        //SCI_TRACE_LOW:"mmisetting_assert MMISET_AppendInputLanguageItem language_type %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_3978_112_2_18_2_51_14_221,(uint8*)"d", language_type);
        return;
    }     
    for(i = 0; i < MMISET_MAX_LANGUAGE; i++)
    {    
    	if(MMIAPISET_CheckLanguageStatus((MMISET_LANGUAGE_TYPE_E)i))  
        {
            lang_name_len = MMIAPICOM_Wstrlen(s_lang_set[i].lang_name);
            
            item[itemTotalNum].wstr_ptr = s_lang_set[i].lang_name;  /*lint !e605*/
            item[itemTotalNum].wstr_len =  lang_name_len;
			itemTotalNum++;
		}
    }
	GUIDROPDOWNLIST_AppendItemArray(ctrl_id, item, itemTotalNum);
    
    cur_selection = GetValidIdxByLangIdx((uint32)language_type);
    
    //set current item
    GUIDROPDOWNLIST_SetCurItemIndex(ctrl_id, cur_selection);
}
#endif
/*****************************************************************************/
//  Description : 设置输入语言的结果
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetInputLanguage(MMI_CTRL_ID_T ctrl_id)
{
    uint16 cur_selection = 0;
    MMISET_LANGUAGE_TYPE_E language_type = MMISET_MAX_LANGUAGE;
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;
    uint32 i = 0;

    //get the selected item
    //cur_selection=GUIDROPDOWNLIST_GetCurItemIndex(ctrl_id);
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
    
    for(i=0; i < MMISET_MAX_LANGUAGE; i++)
    {
#ifdef MMI_SIM_LANGUAGE_SUPPORT
        if(cur_selection == GetValidIdxByLangIdx(i) - 1)//
#else
        if(cur_selection == GetValidIdxByLangIdx(i))//
#endif
        {
            language_type = i;
            break;
        }
    }

    //取得GUI语言序
    lang = MMIAPIIM_GetGUILangBySetLangIdx(language_type);

    //把当前的输入语言写NV
    MMIAPIIM_SetLanguage(lang);
}

/*****************************************************************************/
//  Description :是否相关sensor功能开启
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsSensorOpen(MMISET_SENSOR_TYPE_E type)
{
	BOOLEAN is_open=FALSE;
	
#ifdef MOTION_SENSOR_TYPE
	uint32 open_flag=0; /* lint_lai */

	switch(type)
	{
#ifdef MMI_RES_ORIENT_BOTH
		case MMISET_SENSOR_TYPE_LANDSCAPE_90:
			open_flag=MMISET_SENSOR_OPEN_FLAG_LANDSCAPE_90;
			break;
		case MMISET_SENSOR_TYPE_LANDSCAPE_180:
			open_flag=MMISET_SENSOR_OPEN_FLAG_LANDSCAPE_180;
			break;
		case MMISET_SENSOR_TYPE_LANDSCAPE_270:
			open_flag=MMISET_SENSOR_OPEN_FLAG_LANDSCAPE_270;
			break;
#endif
		case MMISET_SENSOR_TYPE_CALL_SILENCE:
			open_flag=MMISET_SENSOR_OPEN_FLAG_CALL_SILENCE;
			break;
		case MMISET_SENSOR_TYPE_REPLACE_WALLPAPER:
			open_flag=MMISET_SENSOR_OPEN_FLAG_REPLACE_WALLPAPER;
			break;
		case MMISET_SENSOR_TYPE_NEXT_SONG:
			open_flag=MMISET_SENSOR_OPEN_FLAG_NEXT_SONG;
			break;
		//case MMISET_SENSOR_TYPE_VIDEO_FULLSCREEN:
		//	open_flag=MMISET_SENSOR_OPEN_FLAG_VIDEO_FULLSCREEN;
		//	break;
		default:
			open_flag=0;
			break;
	}

	if(0!=(s_open_sensor_flag&open_flag))
	{
		is_open=TRUE;
	}
	else
	{
		is_open=FALSE;
	}
#endif
	return is_open;
}

/*****************************************************************************/
//  Description :设置sensor是否开启
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISET_SetIsSensorOpenFlag(uint32 open_flag)
{
#ifdef FLASH_SUPPORT
	uint32 flag = 0;
#else
	uint32 flag = open_flag;
#endif
#ifdef MOTION_SENSOR_TYPE
	s_open_sensor_flag=flag;
	MMINV_WRITE(MMINV_SET_SENSOR_OPEN_FLAG, &flag);

    MMISET_UpdateCommonGSensor();

#endif
	return flag;
}

/*****************************************************************************/
//  Description :更新common gsensor的状态
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_UpdateCommonGSensor()
{
    BOOLEAN is_turn_on = FALSE;

#ifdef MOTION_SENSOR_TYPE
    if ( s_open_sensor_flag & ( MMISET_SENSOR_OPEN_FLAG_LANDSCAPE_90 | MMISET_SENSOR_OPEN_FLAG_LANDSCAPE_180
        | MMISET_SENSOR_OPEN_FLAG_LANDSCAPE_270 | MMISET_SENSOR_OPEN_FLAG_REPLACE_WALLPAPER | MMISET_SENSOR_OPEN_FLAG_NEXT_SONG ) )
    {
        MMK_OpenGsensor( MAIN_IDLE_WIN_ID );
        is_turn_on = TRUE;
    }
    else
    {
        MMK_CloseGsensor( MAIN_IDLE_WIN_ID );
    }
#endif

    return is_turn_on;
}

/*****************************************************************************/
//  Description :获取sensor是否开启flag
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISET_GetIsSensorOpenFlag(void)
{
#ifdef MOTION_SENSOR_TYPE
	return s_open_sensor_flag;
#else
	return 0;
#endif
}
/*****************************************************************************/
//  Description :设置语言可active label的text
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetLanguageCtrlContent(void)
{
#ifdef MMI_SIM_LANGUAGE_SUPPORT
    uint16 index = s_nv_lang_setting;
    MMI_STRING_T content = {0};
    //MMISET_LANGUAGE_TYPE_E  language_type = MMISET_LANGUAGE_ENGLISH;
    //MMIAPISET_GetLanguageType(&language_type);
    
    if(MMISET_LANGUAGE_AUTO == (MMISET_LANGUAGE_TYPE_E)s_nv_lang_setting)
    {
        MMI_STRING_T auto_str = {0};
        MMI_GetLabelTextByLang(TXT_AUTO,&auto_str);
        
        content.wstr_ptr = (wchar *)auto_str.wstr_ptr ;
        content.wstr_len = auto_str.wstr_len;
    }
    else
    {
        content.wstr_ptr = (wchar *)s_lang_set[index].lang_name;
        content.wstr_len = MMIAPICOM_Wstrlen(s_lang_set[index].lang_name);
    }
#else
    uint16           cur_index = 0;
    MMI_STRING_T     content   = {0};
    MMISET_LANGUAGE_TYPE_E  language_type = MMISET_LANGUAGE_ENGLISH;

    MMIAPISET_GetLanguageType(&language_type);
    cur_index = (uint16)language_type;
    content.wstr_ptr = (wchar *)s_lang_set[language_type].lang_name;
    content.wstr_len = MMIAPICOM_Wstrlen(s_lang_set[language_type].lang_name);
#endif

    GUILABEL_SetText(MMISET_LANGUAGE_SHOW_LABEL_CONTENT_CTRL_ID, &content, FALSE);

}
/*****************************************************************************/
//  Description :设置输入法语言可active label的text
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetInputLanguageCtrlContent(void)
{
    GUIIM_LANGUAGE_T sel_im_index  =  0;
    MMISET_LANGUAGE_TYPE_E  cur_im_language = MMISET_LANGUAGE_ENGLISH;
    //uint16      cur_index   = 0;
    MMI_STRING_T     content   = {0};
    
    sel_im_index = MMIAPIIM_GetLanguage();
    cur_im_language = (MMISET_LANGUAGE_TYPE_E)MMIAPIIM_GetSetLangByGUILangIdx(sel_im_index);

#ifdef MMI_SIM_LANGUAGE_SUPPORT
    if (MMISET_LANGUAGE_AUTO == cur_im_language)
    {
        cur_im_language = MMISET_LANGUAGE_ENGLISH;
    }
#endif

    //cur_index = (uint16)cur_im_language;
    content.wstr_ptr = (wchar *)s_lang_set[cur_im_language].lang_name;
    content.wstr_len = MMIAPICOM_Wstrlen(s_lang_set[cur_im_language].lang_name);
    GUILABEL_SetText(MMISET_LANGUAGE_INPUT_LABEL_CONTENT_CTRL_ID, &content, FALSE);

}


#if defined MMI_IM_PDA_SUPPORT
/*****************************************************************************/
//  Description :获取当前输入法语言字符串，注意字符串保存
//                      str_content_ptr长度MMISET_LANG_NAME_MAX_LEN，小心越界
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIAPISET_GetInputLanguageContent(uint16 *str_len_ptr, wchar *str_content_ptr)
{
    GUIIM_LANGUAGE_T sel_im_index  =  0;
    MMISET_LANGUAGE_TYPE_E  cur_im_language = MMISET_LANGUAGE_ENGLISH;

    if(str_len_ptr == PNULL ||str_content_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"MMISET_GetInputLanguageContent"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_4226_112_2_18_2_51_15_222,(uint8*)"");
        return -1;
    }
    
    sel_im_index = MMIAPIIM_GetLanguage();
    cur_im_language = (MMISET_LANGUAGE_TYPE_E)MMIAPIIM_GetSetLangByGUILangIdx(sel_im_index);

#ifdef MMI_SIM_LANGUAGE_SUPPORT
    if (MMISET_LANGUAGE_AUTO == cur_im_language)
    {
        cur_im_language = MMISET_LANGUAGE_ENGLISH;
    }
#endif

    MMIAPICOM_Wstrncpy(str_content_ptr, (wchar *)s_lang_set[cur_im_language].lang_name, MMISET_LANG_NAME_MAX_LEN);
    *str_len_ptr = MMIAPICOM_Wstrlen(s_lang_set[cur_im_language].lang_name);
    return 1;
}

#endif

#if defined(MMI_UPDATE_TIME_SUPPORT)
/*****************************************************************************/
// 	Description : set if  is auto update time
//	Global resource dependence : none
//  Author: xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetIsAutoUpdateTime(
                                          BOOLEAN  is_auto_update_time
                                          )
{
    //write minute reminder info to nv 
    MMINV_WRITE(MMINV_SET_AUTO_UPDATE_TIME_FLAG, &is_auto_update_time);
    /*需同步该flag*/
    MMIAPIAUT_SetIsNeedUpdateTimeByNet(is_auto_update_time);
}

/*****************************************************************************/
// 	Description : set if  is auto update time
//	Global resource dependence : none
//  Author: dave.ruan
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetUpdateTimeDefault(void)
{
    BOOLEAN  is_auto_update_time = TRUE; 

    MMIAPISET_SetIsAutoUpdateTime(is_auto_update_time);
}

/*****************************************************************************/
// 	Description : get if  is auto update time
//	Global resource dependence : none
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsAutoUpdateTime(void)
{
    BOOLEAN is_auto_update_time = TRUE;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    //get if is is auto update time open
    MMINV_READ(MMINV_SET_AUTO_UPDATE_TIME_FLAG, &is_auto_update_time,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_AUTO_UPDATE_TIME_FLAG, &is_auto_update_time);
        /*需同步该flag*/
        MMIAPIAUT_SetIsNeedUpdateTimeByNet(is_auto_update_time);
    }
    
    return (is_auto_update_time);
}
/*****************************************************************************/
//  Description : API: restore data and time factory setting
//  Global resource dependence : none
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_RestoreDateTimeFactorySetting(void)
{
    //SCI_TRACE_LOW:"MMIAPISET_RestoreDataTimeFactorySetting"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_4307_112_2_18_2_51_15_223,(uint8*)"");
    
    RestoreDataTimeFactorySetting();
}
#endif /*MMI_UPDATE_TIME_SUPPORT*/

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : get is blacklist audio call
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsBlacklistAudioCall(void)
{
    BOOLEAN is_black_audio_call = FALSE;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_SET_BLACKLIST_AUDIOCALL,&is_black_audio_call,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_black_audio_call = FALSE;
        MMINV_WRITE(MMINV_SET_BLACKLIST_AUDIOCALL,&is_black_audio_call);
    }
    
    return (is_black_audio_call);
}

/*****************************************************************************/
//  Description : set is blacklist audio call
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_SetIsBlacklistAudioCall(BOOLEAN is_black_audio_call) 
{
    MMINV_WRITE(MMINV_SET_BLACKLIST_AUDIOCALL,&is_black_audio_call);
}

/*****************************************************************************/
//  Description : get is blacklist vedio call
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsBlacklistVedioCall(void)
{
    BOOLEAN                 is_black_vedio_call = FALSE;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_SET_BLACKLIST_VEDIOCALL,&is_black_vedio_call,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_black_vedio_call = FALSE;
        MMINV_WRITE(MMINV_SET_BLACKLIST_VEDIOCALL,&is_black_vedio_call);
    }
    
    return (is_black_vedio_call);
}

/*****************************************************************************/
//  Description : set is blacklist vedio call
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_SetIsBlacklistVedioCall(BOOLEAN is_black_vedio_call) 
{
    MMINV_WRITE(MMINV_SET_BLACKLIST_VEDIOCALL,&is_black_vedio_call);
}

/*****************************************************************************/
//  Description : get is blacklist message
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsBlacklistMessage(void)
{
    BOOLEAN                 is_black_mesage = FALSE;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_SET_BLACKLIST_MESSAGE,&is_black_mesage,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_black_mesage = FALSE;
        MMINV_WRITE(MMINV_SET_BLACKLIST_MESSAGE,&is_black_mesage);
    }
    
    return (is_black_mesage);
}

/*****************************************************************************/
//  Description : set is blacklist message
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_SetIsBlacklistMessage(BOOLEAN is_black_mesage) 
{
    MMINV_WRITE(MMINV_SET_BLACKLIST_MESSAGE,&is_black_mesage);
}

/*****************************************************************************/
//  Description : load blacklist
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_GetBlackList(void)
{
    MN_RETURN_RESULT_E return_val = MN_RETURN_FAILURE;

    if(PNULL == g_blacklist_ptr)
    {
        return FALSE;
    }

    MMINV_READ(MMINV_SET_BLACKLIST, g_blacklist_ptr, return_val);
    
    if (return_val != MN_RETURN_SUCCESS)
    {
        SCI_MEMSET(g_blacklist_ptr, 0x00, sizeof(MMISET_BLACKLIST_T));
        
        MMINV_WRITE(MMINV_SET_BLACKLIST, g_blacklist_ptr);
    }
   
    return (return_val);
}

/*****************************************************************************/
//  Description : init blacklist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC BOOLEAN MMISET_InitBlacklistParam(void) 
{
    BOOLEAN result = FALSE;

    if(PNULL == g_blacklist_ptr)
    {
        g_blacklist_ptr = (MMISET_BLACKLIST_T *)SCI_ALLOCA(sizeof(MMISET_BLACKLIST_T));
        if(PNULL != g_blacklist_ptr)
        {
            SCI_MEMSET(g_blacklist_ptr,0x00,sizeof(MMISET_BLACKLIST_T));
            result = TRUE;
        }
    }
    else
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : free blacklist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_FreeBlacklistParam(void) 
{
    if(PNULL != g_blacklist_ptr)
    {
        SCI_FREE(g_blacklist_ptr);
        g_blacklist_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : set blacklist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_SetBlackList(MMISET_BLACKLIST_T *blacklist) 
{
    MMINV_WRITE(MMINV_SET_BLACKLIST,blacklist);
}

/*****************************************************************************/
//  Description : init blacklist parm
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void InitBlacklistParm(void)
{
    BOOLEAN is_black_audio_call = FALSE;
    BOOLEAN is_black_vedio_call = FALSE;
    BOOLEAN is_black_mesage = FALSE;    
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_BLACKLIST_AUDIOCALL,&is_black_audio_call,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_black_audio_call = FALSE;
        MMINV_WRITE(MMINV_SET_BLACKLIST_AUDIOCALL,&is_black_audio_call);
    }

    MMINV_READ(MMINV_SET_BLACKLIST_VEDIOCALL,&is_black_vedio_call,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_black_vedio_call = FALSE;
        MMINV_WRITE(MMINV_SET_BLACKLIST_VEDIOCALL,&is_black_vedio_call);
    } 
    
    MMINV_READ(MMINV_SET_BLACKLIST_MESSAGE,&is_black_mesage,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_black_mesage = FALSE;
        MMINV_WRITE(MMINV_SET_BLACKLIST_MESSAGE,&is_black_mesage);
    }

    if(MMISET_InitBlacklistParam())
    {
        MMINV_READ(MMINV_SET_BLACKLIST, g_blacklist_ptr, return_value);
        
        if (return_value != MN_RETURN_SUCCESS)
        {
            SCI_MEMSET(g_blacklist_ptr, 0x00, sizeof(MMISET_BLACKLIST_T));
            
            MMINV_WRITE(MMINV_SET_BLACKLIST, g_blacklist_ptr);
        }  
        MMISET_FreeBlacklistParam();
    }

    return;
}

/*****************************************************************************/
//  Description : find num in blacklist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindNumInBlacklist(MMIPB_BCD_NUMBER_T *orginal_addr_ptr)
{
    int32 i = 0;
    BOOLEAN result = FALSE;
    MMIPB_BCD_NUMBER_T cmp_blacklist = {0};
    uint8 cmp_bcd_num[MMISET_PHONE_NUM_MAX_LEN + 1] = {0};
    MMI_PARTY_NUMBER_T party_number = {MN_NUM_TYPE_UNKNOW,0,0};
    MN_CALLED_NUMBER_T dest_num = {MN_NUM_TYPE_UNKNOW, MN_NUM_PLAN_UNKNOW, 0, 0};

    for(i=0;i<g_blacklist_ptr->num;i++)
    {
        SCI_MEMSET(&cmp_blacklist,0x00,sizeof(MMIPB_BCD_NUMBER_T));
        SCI_MEMSET(&cmp_bcd_num,0x00,sizeof(uint8) * MMISET_PHONE_NUM_MAX_LEN);

        //convert blacklist num to bcd num
        MMI_WSTRNTOSTR(
                       cmp_bcd_num,
                       MMISET_PHONE_NUM_MAX_LEN,
                       g_blacklist_ptr->blacklist[i].num,
                       MMISET_PHONE_NUM_MAX_LEN,
                       g_blacklist_ptr->blacklist[i].num_len
                      );
        
        MMIAPICOM_GenPartyNumber(cmp_bcd_num, g_blacklist_ptr->blacklist[i].num_len, &party_number);

        dest_num.number_type = party_number.number_type;                            
        dest_num.num_len      = MIN( party_number.num_len, MN_MAX_ADDR_BCD_LEN );
        SCI_MEMCPY(
                    (void*)dest_num.party_num,
                    (void*)party_number.bcd_num,
                    dest_num.num_len
                    );   

        cmp_blacklist.number_len = dest_num.num_len;
        cmp_blacklist.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(dest_num.number_type,MN_NUM_PLAN_UNKNOW);
        SCI_MEMCPY(
                    cmp_blacklist.number,
                    dest_num.party_num,
                    cmp_blacklist.number_len
                    );        

        //compare two bcd num
        if(0 == MMIAPIPB_BCDCmp(orginal_addr_ptr,&cmp_blacklist))
        {
            return TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : find num in blacklist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsCCBlacklist(
                                       MMIPB_BCD_NUMBER_T *orginal_addr_ptr,
                                       MN_CALL_TYPE_E call_type)
{
    BOOLEAN result = FALSE;
    BOOLEAN is_need_free = FALSE;

    if (PNULL == orginal_addr_ptr
	    || 0 == orginal_addr_ptr->number_len)
    {
        return FALSE;
    }

    //g_blacklist_ptr is not PNULL,set win is open,don't free
    if(PNULL == g_blacklist_ptr)
    {
        is_need_free = TRUE;
    }

    if(MMISET_InitBlacklistParam())
    {
        MMISET_GetBlackList();
            
        if(MN_CALL_TYPE_DATA == call_type)
        {
            if(MMIAPISET_GetIsBlacklistVedioCall())
            {
                result = FindNumInBlacklist(orginal_addr_ptr);
            }
            else
            {
                result = FALSE;
            }
        }
        else
        {
            if(MMIAPISET_GetIsBlacklistAudioCall())
            {
                result = FindNumInBlacklist(orginal_addr_ptr);
            }
            else
            {
                result = FALSE;
            }
        }
        if(is_need_free)
        {
            MMISET_FreeBlacklistParam();  
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : find num in blacklist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsSMSBlacklist(
                                       MMIPB_BCD_NUMBER_T *orginal_addr_ptr)
{
    BOOLEAN result = FALSE;
    BOOLEAN is_need_free = FALSE;

    if (PNULL == orginal_addr_ptr
	    || 0 == orginal_addr_ptr->number_len)
    {
        return FALSE;
    }

    //g_blacklist_ptr is not PNULL,set win is open,don't free
    if(PNULL == g_blacklist_ptr)
    {
        is_need_free = TRUE;
    }
    
    if(MMISET_InitBlacklistParam())
    {
        MMISET_GetBlackList();

        if(MMIAPISET_GetIsBlacklistMessage())
        {
            result = FindNumInBlacklist(orginal_addr_ptr);
        }
        else
        {
            result = FALSE;
        }

        if(is_need_free)
        {
            MMISET_FreeBlacklistParam();  
        }  
    }

    return result;
}

/*****************************************************************************/
//  Description : restore blacklist setting
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void RestoreBlacklistSetting(BOOLEAN is_clean_blacklist_value)
{
    BOOLEAN blacklist_default_value = FALSE;
    
    MMISET_SetIsBlacklistAudioCall(blacklist_default_value);
#if defined(VT_SUPPORT) 
    MMISET_SetIsBlacklistVedioCall(blacklist_default_value);
#endif
    MMISET_SetIsBlacklistMessage(blacklist_default_value);
    
    if(is_clean_blacklist_value)
    {
        if(MMISET_InitBlacklistParam())
        {
            MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
            
            MMINV_READ(MMINV_SET_BLACKLIST, g_blacklist_ptr, return_value);

            /*blacklist is not null, set g_blacklist_ptr null*/
            if (return_value != MN_RETURN_SUCCESS ||g_blacklist_ptr->num != 0)
            {
                SCI_MEMSET(g_blacklist_ptr, 0x00, sizeof(MMISET_BLACKLIST_T));
                
                MMINV_WRITE(MMINV_SET_BLACKLIST, g_blacklist_ptr);
            }  
            MMISET_FreeBlacklistParam();
        }
    }
}
/*****************************************************************************/
//  Description : set new blacklist num
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_AddNewBlacklistNum(
                                         MMIPB_BCD_NUMBER_T *orginal_addr_ptr
                                        )
{
    MMK_CloseWin(MMISET_SET_BLACKLIST_NEW_WIN_ID);
    MMK_CloseWin(MMISET_SET_BLACKLIST_EDIT_WIN_ID);

    //g_blacklist_ptr is not PNULL,set win is open,don't free
    if(MMISET_InitBlacklistParam())
    {
        MMISET_GetBlackList();

        if(g_blacklist_ptr->num == MMISET_BLACKLIST_MAX_NUM)
        {
            if(!MMK_IsOpenWin(MMISET_SET_BLACKLIST_LIST_WIN_ID))
            {
                MMISET_FreeBlacklistParam();
            }
            MMIPUB_OpenAlertWarningWin(TXT_SET_BLACKLIST_FULL);            
            return;
        }

        MMISET_OpenBlacklistEditWin(orginal_addr_ptr);
    }

    return;
}

/*****************************************************************************/
//  Description : set all blacklist setting open/close
//  Global resource dependence : none
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMIAPISET_SetAllBlacklistSetting(BOOLEAN is_open) 
{
    MMINV_WRITE(MMINV_SET_BLACKLIST_AUDIOCALL,&is_open);
    MMINV_WRITE(MMINV_SET_BLACKLIST_VEDIOCALL,&is_open);
    MMINV_WRITE(MMINV_SET_BLACKLIST_MESSAGE,&is_open);
}
#endif//#if defined(MMI_BLACKLIST_SUPPORT)

/*****************************************************************************/
//  Description : alloc and init language
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void MMISET_LanguageInit(void)
{
    int16 i =0;
    if (PNULL == s_lang_valid_info)
    {
        s_lang_valid_info = SCI_ALLOC_APP(sizeof(MMISET_LANG_VALID_INFO_T));
    }
    SCI_MEMSET(s_lang_valid_info,0x00,sizeof(MMISET_LANG_VALID_INFO_T));
    for(i = 0;i < MMISET_MAX_LANGUAGE;i++)
    {    
        if(MMIAPISET_CheckLanguageStatus((MMISET_LANGUAGE_TYPE_E)i))
        {
            s_lang_valid_info->lang_valid[s_lang_valid_info->lang_valid_num]=i;
            s_lang_valid_info->lang_valid_num++;
        }
    }
}
/*****************************************************************************/
//  Description : get valid language number
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISET_GetValidLanguageNum(void)
{
    //SCI_TRACE_LOW:"MMISET_AllocAndInitLanguageInfo valid language is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_4779_112_2_18_2_51_16_224,(uint8*)"d",s_lang_valid_info->lang_valid_num);
    return s_lang_valid_info->lang_valid_num;
}

#ifdef MMI_SIM_LOCK_SUPPORT
/********************************************************************************
NAME:           MMIAPISET_ParseSIMLockSwitchString
DESCRIPTION:    "####8888#": open sim lock switch menu
PARAM IN:       
PARAM OUT:      
AUTHOR:     dave.ruan
DATE:       
********************************************************************************/
PUBLIC BOOLEAN MMIAPISET_ParseSIMLockSwitchString(uint8* str_str, uint8 str_len)
{
    if (PNULL == str_str)
    {
        MMI_ASSERT_WARNING("str_str != PNULL");
        return  FALSE;
    }
       
    if (str_len != 9)
    {
        return  FALSE;
    }
    
    if (strncmp((char*)str_str, "####8888#", 9) == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

/*****************************************************************************/

/*****************************************************************************/
//  Description : Set Power On Off Animation information
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPowerOnOffAnimInfo(
                                            MMISET_POWER_ON_OFF_ANIM_INFO_T *power_on_off_anim_info_ptr,    //in/out
                                            BOOLEAN is_power_on_off     //TRUE; power on, FALSE: power off
                                            )
{
    if(is_power_on_off)
    {
        MMINV_WRITE(MMINV_POWER_ON_ANIM_SETTING_INFO, power_on_off_anim_info_ptr);
        SCI_TRACE_LOW(
            "MMIAPISET_SetPowerOnOffAnimInfo: file name length is %d",
            power_on_off_anim_info_ptr->more_anim_info.name_wstr_len
            );
    }
    else
    {
        MMINV_WRITE(MMINV_POWER_OFF_ANIM_SETTING_INFO, power_on_off_anim_info_ptr);
        SCI_TRACE_LOW(
            "MMIAPISET_SetPowerOnOffAnimInfo: file name length is %d",
            power_on_off_anim_info_ptr->more_anim_info.name_wstr_len
            );
    }
}


/*****************************************************************************/
//  Description : Reset Power On Off Animation information
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_RetPowerOnOffAnimInfo(
                                            BOOLEAN is_power_on_off     //TRUE; power on, FALSE: power off
                                            )
{
    MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_off_anim_info = {0};
    
    power_on_off_anim_info.is_open     = TRUE;
    power_on_off_anim_info.anim_type   = MMISET_POWER_ON_OFF_ANIM_DEFAULT;
    MMIAPISET_SetPowerOnOffAnimInfo(&power_on_off_anim_info, is_power_on_off);
}
/*****************************************************************************/
//  Description :获取当前系统语言
//  Global resource dependence : 
//  Author: yuantao.xu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISET_GetCurrentLanguageType(void)
{
    MMISET_LANGUAGE_TYPE_E    language_type =   MMISET_EDEFAULT_LANGUAGE; /*lint !e64*/ //default type of language
    MN_RETURN_RESULT_E        return_value  =   MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_SET_LANGUAGE_TYPE,(void*)(&language_type),return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_LANGUAGE_TYPE,(void*)(&language_type));
    }

#ifdef MMI_SIM_LANGUAGE_SUPPORT
    //g_ksp_Lang = (uint8)language_type;
    s_nv_lang_setting = (uint8)language_type;

    //初始化g_ksp_lang 的类型。
    g_ksp_Lang = MMIAPISET_ConvertNVLangToSysType((MMISET_LANGUAGE_TYPE_E)s_nv_lang_setting);
#else
    g_ksp_Lang = (uint8)language_type;
#endif

    return g_ksp_Lang;
}

#ifdef MMI_SIM_LANGUAGE_SUPPORT
/*****************************************************************************/
//  Description : MMIAPISET_ConvertNVLangToSysType
//  Global resource dependence : 
//  Author:michael.shi
//  Note: NV中存储的LANGUAGE Type分为 AUTO 和LANGUAGE 两种数据
//             通过此接口完成NV 的类型转换为实际的语言类型:
//   1. set g_ksp_Lang  :  这个是系统实际运行中显示的语言
//   2. return g_ksp_Lang :  返回当前的语言类型，供其它模块调用。   
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_ConvertNVLangToSysType(
                                   MMISET_LANGUAGE_TYPE_E   set_language   
                                   )
{
    MMISET_LANGUAGE_TYPE_E auto_lang = MMISET_LANGUAGE_AUTO;
    MN_DUAL_SYS_E dual_sys = MMIAPIPHONE_GetPreferLangSim();

    //SCI_TRACE_LOW:"MMIAPISET_ConvertNVLangToSysType(set_lang = %d) BEGIN:dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_4897_112_2_18_2_51_16_225,(uint8*)"dd",set_language,dual_sys);
    
    if(MMISET_LANGUAGE_AUTO == set_language)
    {
        //获取优选语言。
        if(dual_sys < MMI_DUAL_SYS_MAX)
        {
            MMIAPISET_GetPreferLanguageTypeFromSim(dual_sys,&auto_lang);
            //SCI_TRACE_LOW:"MMIAPISET_ConvertNVLangToSysType,get auto_lang = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_4905_112_2_18_2_51_16_226,(uint8*)"d",auto_lang);
        }
        
        //获取的语言种类有效值在"1~~(max-1) " 之间.
        if((MMISET_LANGUAGE_AUTO < auto_lang )
            && (MMISET_MAX_LANGUAGE > auto_lang))
        {
            g_ksp_Lang = (uint8)auto_lang;
        }
        else
        {        
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
            const MMIPLMN_LANGUAGE_T *local_language_list_ptr = PNULL;
            BOOLEAN is_use_local = FALSE;
#ifndef WIN32
            MMI_MULCONFIG_INFO_T config_info = {0};
#endif

            //根据当地语言配置
            if (dual_sys < MMI_DUAL_SYS_MAX)
            {
                local_language_list_ptr = MMIAPIPLMN_GetLocalLanguageSettings(dual_sys);

                if (PNULL != local_language_list_ptr && local_language_list_ptr->language_id < MMISET_MAX_LANGUAGE)
                {
#ifdef WIN32
                    if(TRUE == s_lang_set[local_language_list_ptr->language_id].lang_status)
#else
                    GetMulNVConfigInfo(&config_info);

                    if((TRUE == s_lang_set[local_language_list_ptr->language_id].lang_status) && (config_info.flag[local_language_list_ptr->language_id]>>2) & 1)
#endif
                    {
                        g_ksp_Lang = local_language_list_ptr->language_id;

                        is_use_local = TRUE;
                    }
                }
            }

            if (!is_use_local)
#endif
            {
                //如果自动配置的语言不合法，设置为英语
                g_ksp_Lang = MMISET_LANGUAGE_ENGLISH;           
            }
        }        
    }
    else
    {
        g_ksp_Lang = (uint8)set_language;
    }
    
    //需要同步修改s_nv_lang_setting 的数据。
    s_nv_lang_setting = (uint8)set_language;
    
    //SCI_TRACE_LOW:"MMIAPISET_ConvertNVLangToSysType END: cur_lang = %d,nv_lang=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_4928_112_2_18_2_51_16_227,(uint8*)"dd",g_ksp_Lang,s_nv_lang_setting);

    //返回当前系统使用的确切的语言类型。
    return g_ksp_Lang;   
}

/*****************************************************************************/
//  Description : MMIAPISET_GetPreferLanguageTypeFromSim
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 当系统设置为"AUTO"时，显示语言如何做配置:
//             通过MN 的接口读取存储在sim 卡中的优选语言列表
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetPreferLanguageTypeFromSim(
                                                                    MN_DUAL_SYS_E dual_sys,
                                                                    MMISET_LANGUAGE_TYPE_E *prefer_language
                                                                    )
{
    MN_SIM_LANGUAGE_LIST_T sim_language_list = {0};
    uint16 lang_num = sizeof(s_lang_map_sim)/sizeof(MMISET_LANG_MAP_SIM_BYTE_T) ;
    uint16 lang_num_ext = sizeof(s_lang_map_sim_ext)/sizeof(MMISET_LANG_MAP_SIM_EXT_BYTE_T);

    if((dual_sys < MMI_DUAL_SYS_MAX) && (PNULL != prefer_language) )
    {
        //调用MN 的接口去获取sim card prefer language list.
        MNSIM_GetLanguageEx(dual_sys, &sim_language_list);

        //SCI_TRACE_LOW:"MMIAPISET_GetPreferLanguageTypeFromSim Begin :sim=%d, [elp_lst_isvalid=%d and num=%d],[lp_isvalid=%d and num=%d],[elp_2nd_valid= %d and num=%d]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_4956_112_2_18_2_51_16_228,(uint8*)"ddddddd",dual_sys, sim_language_list.is_1st_valid,sim_language_list.elp_1st_valid_num,sim_language_list.is_lp_valid,sim_language_list.lp_1st_valid_num,sim_language_list.is_2nd_valid,sim_language_list.elp_2nd_valid_num);

        /* 解析sim language list 中的数据,遍历如下的三种数据顺序不能更改-- PS SPEC!*/
        
        //first elp list check:  双字节列表查询
        if( sim_language_list.is_1st_valid )
        {
            int32 i = 0, j = 0;
            int32 loop_num = MIN(sim_language_list.elp_1st_valid_num,MN_SIM_MAX_VALIDATE_LANGUAGE_NUM);

            for(i=0; i < loop_num; i++)
            {
                SCI_TRACE_LOW("GetPreferedLanguage code[0x%x,0x%x]", sim_language_list.elp_1st_lang_codes[i].code[0], sim_language_list.elp_1st_lang_codes[i].code[1]);

                for(j = 0; j < lang_num_ext; j++)
                {
                    //SCI_TRACE_LOW:"GetPreferedLanguage 1st_elp:i=%d,j=%d,mmi_type=%d,sim_code=[0x%x,0x%x]"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_4971_112_2_18_2_51_16_229,(uint8*)"ddddd",i,j,s_lang_map_sim_ext[j].mmi_type,s_lang_map_sim_ext[j].sim_type[0],s_lang_map_sim_ext[j].sim_type[1]);

                    //找到对应的语言并且该语言是当前系统支持的语言种类。
                    if( (0 == SCI_MEMCMP((uint8 *)sim_language_list.elp_1st_lang_codes[i].code, (uint8 *)s_lang_map_sim_ext[j].sim_type, 2)) 
                        && MMIAPISET_CheckLanguageStatus(s_lang_map_sim_ext[j].mmi_type))
                    {                        
                        *prefer_language = s_lang_map_sim_ext[j].mmi_type;
                         return TRUE;
                    }
                }
            }
        }
        
        //first lp check: 单字节列表查询
        if( sim_language_list.is_lp_valid ) 
        {
            int32 i = 0, j = 0;
            int32 loop_num = MIN(sim_language_list.lp_1st_valid_num,MN_SIM_MAX_VALIDATE_LANGUAGE_NUM);
            for(i=0; i < loop_num; i++)
            {
                SCI_TRACE_LOW("GetPreferedLanguage lp_1st_lang_codes[0x%x,0x%x]", sim_language_list.lp_1st_lang_codes[0], sim_language_list.lp_1st_lang_codes[1]);

                for(j = 0; j < lang_num; j++)
                {
                    //SCI_TRACE_LOW:"GetPreferedLanguage lp :i=%d,j=%d,mmi_type=%d,sim_code=0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_4994_112_2_18_2_51_16_230,(uint8*)"dddd",i,j,s_lang_map_sim[j].mmi_type,s_lang_map_sim[j].sim_type);

                    //找到对应的语言并且该语言是当前系统支持的语言种类。
                    if (( sim_language_list.lp_1st_lang_codes[i]  == s_lang_map_sim[j].sim_type)
                        && MMIAPISET_CheckLanguageStatus(s_lang_map_sim[j].mmi_type))
                    {                        
                        *prefer_language = s_lang_map_sim[j].mmi_type;
                        return  TRUE;
                    }
                }
            }
        }
        
        //second elp check: 双字节列表查询
        if( sim_language_list.is_2nd_valid ) 
        {
            int32 i =0, j = 0;
            int32 loop_num = MIN(sim_language_list.elp_2nd_valid_num,MN_SIM_MAX_VALIDATE_LANGUAGE_NUM);
            for(i=0; i < loop_num; i++)
            {
                SCI_TRACE_LOW("GetPreferedLanguage code[0x%x,0x%x]", sim_language_list.elp_2nd_lang_codes[i].code[0], sim_language_list.elp_2nd_lang_codes[i].code[1]);

                for(j = 0; j < lang_num_ext; j++)
                {
                    //SCI_TRACE_LOW:"GetPreferedLanguage 2nd_elp:i=%d,j=%d,mmi_type=%d,sim_code=[0x%x,0x%x]"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_FUNC_5017_112_2_18_2_51_16_231,(uint8*)"ddddd",i,j,s_lang_map_sim_ext[j].mmi_type,s_lang_map_sim_ext[j].sim_type[0],s_lang_map_sim_ext[j].sim_type[1]);

                    //找到对应的语言并且该语言是当前系统支持的语言种类。
                    if((0 == SCI_MEMCMP((uint8 *)sim_language_list.elp_2nd_lang_codes[i].code,(uint8 *)s_lang_map_sim_ext[j].sim_type,2) )
                        && MMIAPISET_CheckLanguageStatus(s_lang_map_sim_ext[j].mmi_type) )
                    {                                        
                        *prefer_language = s_lang_map_sim_ext[j].mmi_type;
                        return TRUE;
                    }
                }
            }
        }  
        
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : MMIAPISET_GetNVLanguageType
//  Global resource dependence : 
//  Author:michael.shi
//  Note: return s_nv_lang_setting to
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetNVLanguageType( void )
{
    return s_nv_lang_setting;
}
#endif

/*****************************************************************************/
//  Description : Select SIM API of SET app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_SelectSIMFunc(MMIAPIPHONE_SELECTSIM_FUNC MMISetSelectSIMWinCallback,
                                        MMISET_APPEND_SIM_TYPE_E append_type,
                                        ADD_DATA        add_data
                                        )
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    MMI_WIN_ID_T    selectSimWinID = MMISET_COM_SELECT_SIM_WIN_ID;
    MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};

    caller_data.append_type = (uint32)append_type;
    caller_data.callback_func =MMISetSelectSIMWinCallback ;
#ifdef BT_DIALER_SUPPORT
	caller_data.select_type = MMIPHONE_SELECTION_SIM_ONLY | MMIPHONE_SELECTION_SIM_NO_BT;
#else
    caller_data.select_type = MMIPHONE_SELECTION_SIM_ONLY;
#endif

    caller_data.publist_id = MMISET_COM_SELECT_SIM_CTRL_ID;
    caller_data.win_id_ptr = &selectSimWinID;
    caller_data.user_data  = add_data;
    caller_data.extra_data = PNULL;


    MMIAPIPHONE_OpenSelectSimWin(&caller_data);  
#endif

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : Select NETWORK API of SET app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_SelectNetworkFunc(MMIAPIPHONE_SELECTSIM_FUNC MMISetSelectSIMWinCallback,
                                        MMISET_APPEND_SIM_TYPE_E append_type,
                                        ADD_DATA        add_data
                                        )
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    MMI_WIN_ID_T    selectSimWinID = MMISET_COM_SELECT_SIM_WIN_ID;
    MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};

    caller_data.append_type = (uint32)append_type;
    caller_data.callback_func =MMISetSelectSIMWinCallback ;
    caller_data.select_type= MMIPHONE_SELECTION_SIM_WLAN;

    caller_data.publist_id = MMISET_COM_SELECT_SIM_CTRL_ID;
    caller_data.win_id_ptr = &selectSimWinID;
    caller_data.user_data  = add_data;
    caller_data.extra_data = PNULL;

    MMIAPIPHONE_OpenSelectSimWin(&caller_data);

#endif
    return MMI_RESULT_TRUE;
}

#ifdef MMI_GPRS_DATA_SIM_SET
/*****************************************************************************/
// 	Description : set GPRS Mode type 
//	Global resource dependence : none
//  Author: taiping.lai
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetGPRSModeType(
                                          MMISET_GPRS_MODE_TYPE_E  mode_type
                                          )
{
    SCI_TRACE_LOW("MMIAPISET_SetGPRSModeType mode_type=%d", mode_type);
    if (mode_type < DATA_CARD_MODE_MAX)
    {
        MMINV_WRITE(MMINV_SET_GPRS_MODE_TYPE, &mode_type);
    }
}

/*****************************************************************************/
// 	Description : get GPRS Mode type 
//	Global resource dependence : none
//  Author: taiping.lai
//	Note:
/*****************************************************************************/
PUBLIC MMISET_GPRS_MODE_TYPE_E MMIAPISET_GetGPRSModeType(void)
{
    MMISET_GPRS_MODE_TYPE_E       mode_type = DATA_CARD_MANUAL_SELECT;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_SET_GPRS_MODE_TYPE, &mode_type,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        mode_type = DATA_CARD_MANUAL_SELECT;
        MMIAPISET_SetGPRSModeType(mode_type);
    }
    SCI_TRACE_LOW("MMIAPISET_GetGPRSModeType mode_type=%d", mode_type);
    return (mode_type);
}

/*****************************************************************************/
// 	Description : restore GPRS mode
//	Global resource dependence : none
//  Author: taiping.lai
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_RestoreGPRSModeSetting(void)
{
    MMIAPISET_SetGPRSModeType(DATA_CARD_MANUAL_SELECT);
}
#endif
