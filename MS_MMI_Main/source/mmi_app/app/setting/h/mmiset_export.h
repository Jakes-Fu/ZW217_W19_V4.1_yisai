/*****************************************************************************
** File Name:      mmiset_export.h                                                  *
** Author:                                                                   *
** Date:           09/02/2012                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2004       Jassmine.Meng       Create
   02/2012       xiyuan.ma           edite
******************************************************************************/

#ifndef _MMISET_EXPORT_H_
#define _MMISET_EXPORT_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "tb_dal.h"
#include "mn_type.h"
#include "guilistbox.h"
#include "mmiparse_export.h"
#include "mmisrvaud_api.h"
#include "mmi_filemgr.h"
#include "mmicom_time.h"
#include "mmi_position.h"
#include "mmi_custom_define.h"
#include "guiownerdraw.h"

#include "mmiwallpaper_image.h"
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmilivewallpaper_export.h"
#endif

#include "guibutton.h"
#include "mmi_mainmenu_export.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
** Macro Declaration *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMISET_GetCallRingVolume             MMIAPISET_GetCallRingVolume
#define MMISET_SetMainmenuStyle              MMIAPISET_SetMainmenuStyle
#define MMISET_GetAnswerWithOriginalSimType  MMIAPISET_GetAnswerWithOriginalSimType
#define MMAPISET_OpenWizardCFNRCWin          MMIAPISET_OpenWizardCFNRCWin
#define MMISET_SetRedKeyLock                 MMIAPISET_SetRedKeyLock
#define MMISET_OpenDivertSettingsWindow      MMIAPISET_OpenDivertSettingsWindow
#define MMISET_GetSpeedDialDelay             MMIAPISET_GetSpeedDialDelay
#define MMISET_AppendFixedItems              MMIAPISET_AppendFixedItems
#define MMISET_GetRingName                   MMIAPISET_GetRingName
#define MMISET_GetMsgRingInfo                MMIAPISET_GetMsgRingInfo
#define MMISET_SetBackLightInfo              MMIAPISET_SetBackLightInfo
#define MMISET_SetAllBlacklistSetting        MMIAPISET_SetAllBlacklistSetting
#define MMISET_GetDateDisplayType            MMIAPISET_GetDateDisplayType
#define MMISET_SetTTSInfo                    MMIAPISET_SetTTSInfo
#define MMISET_OpenFuncKeyWin                MMIAPISET_OpenFuncKeyWin
#define MMISET_OpenDirectionKeyWin           MMIAPISET_OpenDirectionKeyWin
#define MMISET_OpenCallSettingsWindow        MMIAPISET_OpenCallSettingsWindow
#define MMISET_EnterDisplaySettingWin        MMIAPISET_EnterDisplaySettingWin
#define MMISET_EnterPhoneSettingWin          MMIAPISET_EnterPhoneSettingWin
#define MMISET_EnterSecuritySettingWin       MMIAPISET_EnterSecuritySettingWin
#define MMISET_OpenUssdSelectWin             MMIAPISET_OpenUssdSelectWin
#define MMISET_IsDisplayIdleWords            MMIAPISET_IsDisplayIdleWords
#define MMISET_CreateCustomWordEditWin       MMIAPISET_CreateCustomWordEditWin
#define MMMISET_SetDisplaytIdleMode          MMIAPISET_SetDisplaytIdleMode
#define MMISET_SetCurrentContrast            MMIAPISET_SetCurrentContrast
#define MMISET_SetMainLCDContrast            MMIAPISET_SetMainLCDContrast
#define MMISET_EnterMonternetWin             MMIAPISET_EnterMonternetWin
#define MMISET_EnterPocketHallWin            MMIAPISET_EnterPocketHallWin
#define MMISET_EnterMusicWin                 MMIAPISET_EnterMusicWin
#define MMISET_EnterFetionWin                MMIAPISET_EnterFetionWin
#define MMISET_EnterStockWin                 MMIAPISET_EnterStockWin
#define MMISET_EnterMapWin                   MMIAPISET_EnterMapWin
#define MMISET_IsKLPswOn                     MMIAPISET_IsKLPswOn
#define MMISET_SetKLPswOnOff                 MMIAPISET_SetKLPswOnOff
#define MMISET_GetKLPsw                      MMIAPISET_GetKLPsw
#define MMISET_SetKLPsw                      MMIAPISET_SetKLPsw
#define MMISET_OpenKLPswSettingWin           MMIAPISET_OpenKLPswSettingWin
#define MMISET_CreateMainmenuStyleWin        MMIAPISET_CreateMainmenuStyleWin
#define MMISET_SetOkButtonCallback           MMIAPISET_SetOkButtonCallback
#define MMISET_SetCancelButtonCallback       MMIAPISET_SetCancelButtonCallback
#define MMISET_LongpowerCreatmenu            MMIAPISET_LongpowerCreatmenu
#define MMISET_ResumePreLivewallpaper        MMIAPISET_ResumePreLivewallpaper
#define MMISET_ResumePDAPreLivewallpaper     MMIAPISET_ResumePDAPreLivewallpaper
#define MMISET_AppendInputLanguageListItem   MMIAPISET_AppendInputLanguageListItem
#define MMISET_GetInputLanguageString        MMIAPISET_GetInputLanguageString
#define MMISET_GetMainLCDContrast            MMIAPISET_GetMainLCDContrast

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#ifdef MAINLCD_SIZE_320X480
#define MMISET_SCREEN_LOCK_ICON_WIDTH 130
#define MMISET_SCREEN_LOCK_ICON_HEIGHT 200

#elif defined MAINLCD_SIZE_240X400
#define MMISET_SCREEN_LOCK_ICON_WIDTH 110
#define MMISET_SCREEN_LOCK_ICON_HEIGHT 190

#elif defined MAINLCD_SIZE_240X320
#define MMISET_SCREEN_LOCK_ICON_WIDTH 110
#define MMISET_SCREEN_LOCK_ICON_HEIGHT 150

#elif  defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define MMISET_SCREEN_LOCK_ICON_WIDTH  60
#define MMISET_SCREEN_LOCK_ICON_HEIGHT 100
#else
#error
#endif    
#ifndef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#ifdef MMI_SETTING_CUTDOWN_WALLPAPER
#define MMISET_WALLPAPER_USER_MAX_NUM      1
#else
#define MMISET_WALLPAPER_USER_MAX_NUM      3
#endif
#endif
    //环境名称的长度  @ add by maql@
#define MMIENVSET_ENVNAME_MAX_LEN     25// unicode num  "High ring and vibrate"21个字符，多国语言下字符串也很长
#define MMISET_OPEN_KEYBOARD_LIGHT_MIN_HOUR 6 //按键背光夜间模式关闭背关灯的开始时间
#define MMISET_OPEN_KEYBOARD_LIGHT_MAX_HOUR 18  //按键背光夜间模式关闭背关灯的结束时间

#define MMISET_RING_FIXED_DEFAUT_NUM        MMISET_RING_USER_MAX_NUM //固定铃声个数
#define MMISET_MSG_FIXED_RING_MAX_ITEM      MMISET_MSGRING_USER_MAX_NUM
#define MMISET_WALLPAPER_FIXED_PIC_MAX_NUM  (MMISET_WALLPAPER_USER_MAX_NUM + 1)

#define MMISET_MAX_RING_ID                  (MMISET_RING_FIXED_DEFAUT_NUM + 10)  //更多铃声id
#define MMISET_PHONE_NUM_MAX_LEN            (MMIPB_BCD_NUMBER_MAX_LEN << 1)

#define MMISET_VOL_ZERO                     0//no ring//对外
#define MMISET_VOL_ONE                      1//对外
#define MMISET_VOL_TWO                      2//对外
#define MMISET_VOL_THREE                    3//对外
#define MMISET_VOL_FOUR                     4//对外
#define MMISET_VOL_FIVE                     5//对外
#define MMISET_VOL_SIX                      6//对外
#define MMISET_VOL_SEVEN                    7
#define MMISET_VOL_EIGHT                    8
#define MMISET_VOL_NINE                     9
#define MMISET_VOL_MAX                      MMISRVAUD_VOLUME_LEVEL_MAX

#ifdef BLUETOOTH_SUPPORT
#define MMISET_BT_VOL_MAX                   (15)
#endif

#define MMISET_VOL_DEFAULT_MULTIM          9// 5 // wuxx multi media vol

#define MMISET_WALLPAPER_DEFAULT_ID         0//默认墙纸id
#define MMISET_DEFAULT_DSP_PLAY_RING_TIME   300//ms dsp铃声播放时间长度
#define MMISET_CALL_IN_CALL_PLAY_RING_TIME  300//ms 通话中播放来电铃声的时间长度
#define MMISET_CALL_IN_CALL_PLAY_RING_TIME_AUS  (200)//ms 通话中播放来电铃声的时间长度  Australia
#define MMISET_IPNUMBER_MAX_LEN             10 //@Lin.Lin, add for ip call, 2004-12-29/对外
#define MMISET_IPNUMBER_MAX_NUM             5 //ip号码最多5个/对外
#define MMISET_CALL_RING_NAME_MAX_LEN       MMIFILE_FULL_PATH_MAX_LEN //including deice name path name and file name/对外
//#define MMISET_IDLE_WORDS_UCS_MAX_LEN      (MMI_MAINSCREEN_WIDTH/MMI_DEFAULT_BIG_FONT_SIZE)//待机个性词语的最大长度
#define MMISET_IDLE_WORDS_UCS_MAX_LEN      10//待机个性词语的最大长
#define MMISET_MAX_SIM_NAME_LEN             20 //sim卡名字的最大程度
#ifdef SCREENSAVER_SUPPORT
#define MMISET_SCREENSAVER_NAME_MAX_LEN         MMIFILE_FULL_PATH_MAX_LEN //@xingdong.li 屏保名字的最大长度
#endif
#define MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN   MMIFILE_FULL_PATH_MAX_LEN //@xingdong.li //开关机动画的最大长度
#define     MMISET_TIME_PM_HOUR                 12 //12点
#define     MMISET_TIME_13_HOUR                 13 //13点
#define     MMISET_TIME_1_HOUR                  1 //1点
#define    MMISET_KEYLOCK_MAX_ITEM               6 //键盘锁开启时间最大list item
#ifndef MAINLCD_DEV_SIZE_128X160
#define MMISET_IDLE_SHORTCUT_MAX_NUM    5   //set idle shortcut max num
#else
#define MMISET_IDLE_SHORTCUT_MAX_NUM    4   //set idle shortcut max num
#endif
#define MMISET_TIME_STR_12HOURS_LEN	    8		//length of "%02d:%02d PM"
#define MMISET_DATE_STR_LEN	            11		//length of "%04d.%02d.%02d"

#if defined(MMI_BLACKLIST_SUPPORT)
#define MMISET_BLACKLIST_MAX_NUM        30
#endif

#define MMISET_SET_ANYKEY_REJECT_MAX_ITEM    	2				//anykey & reject mode max item
#define MMISET_KEYBOARD_LIGHT_MAX_ITEM      	5 //按键背关灯设置的最大项
#define MMISET_BACKLIGHT_MAX_ITEM           	6 //背光时间的最大项
#define MMISET_MOVE_STYLE_MAX_ITEM      		3
#define MMISET_OPEN_CLOSE_ITEM              	2

#define MMISET_NO_ANSWER_TIME_MAX_ITEM      	6
#define MMISET_NETWORK_SEL_MAX_ITEM         	2
#define MMISET_BAND_SEL_MAX_ITEM            	2
#define MMISET_BAR_PASSWORD_MAX_LEN            	4
#define MMISET_VOCIE_MAX_NUM                	40//20 //暂时定义，将和底层保持一致,baokun change to 40

#ifdef TTS_SUPPORT
#define MMISET_TTS_MAX_NUM                      2  //barry.peng for set tts
#endif

#define MMISET_AUTODIAL_MAX_ITEM                3   //自动重拨菜单的最大条目

// 当资源管理工具中的铃声作出修改后，这些宏均需要作出修改
// @Mark.hu, 集成资源管理工具, begin
// Midi Ring
#define MMI_MIDI_RING_START_ID R_CALL_1  //midi铃声的开始id

// SMS Ring
#define MMI_SMS_RING_START_ID  R_DL_1//短信铃声的开始id

// MMS Ring
#define MMI_MMS_RING_START_ID  R_MMS_1 //彩信铃声的开始id

// Power Ring
#ifdef CMCC_UI_STYLE
#define MMI_POWER_RING_START_ID R_CMCC_POWER_ON //开机铃声的开始id
#define MMI_POWER_RING_OFF_ID R_CMCC_POWER_OFF //抗鼗迳目糹d
#else
//#define MMI_POWER_RING_START_ID R_POWER_1 //开关机铃声的开始id // bug 2124817
#define MMI_POWER_RING_START_ID R_CALL_1 //开关机铃声的开始id
#endif


// Flip Ring
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
#define MMI_FLIP_RING_START_ID R_FLIP_1 //翻盖铃声的开始id
#endif

#ifdef LCD_SLIDE_SUPPORT
// Slide Ring
#define MMI_SLIDE_RING_START_ID R_SLIDE_1
#define MMI_SLIDE_RING_NUMBER   2
#endif
#ifndef MMI_LOW_MEMORY_RING
// Key Ring
#define MMI_KEY_RING_START_ID R_KEY_1  //按键音铃声的开始id

// Key Ring LOUND
#define MMI_KEY_RING_LOUND_START_ID R_KEY_LOUND_1  //按键音铃声的开始id
#else
// Key Ring
#define MMI_KEY_RING_START_ID R_KEY_2  //按键音铃声的开始id

// Key Ring LOUND
#define MMI_KEY_RING_LOUND_START_ID R_KEY_LOUND_2  //按键音铃声的开始id
#endif

// DC Ring
#define MMI_DC_RING_START_ID R_DC_1  //相机铃声的开始
#define MMI_DC_RING_COUNT_ID R_DC_COUNT
#define MMI_DC_RING_FINAL_COUNT_ID R_DC_FINAL_COUNT

#ifdef ASP_SUPPORT
// ASP Ring
#define MMI_ASP_RING_START_ID R_ASP_1
#define MMI_ASP_RING_NUMBER   8
#endif

#if defined(MMI_OCR_AUDIO_PLAY_TEST_LONG)// wuxx 20210805
#define MMI_OCR_AUDIO_TEST_START_ID R_OCR_AUDIO_TEST_MP3
#endif


// Bat Ring
#define MMI_BAT_RING_START_ID R_BAT_1 //低电量铃声开始id

// Charge Ring
#define MMI_CHARGE_RING_START_ID R_CHARGE_1 //充电提示音铃声开始id

//DL Ring
#define MMI_DL_RING_START_ID R_DL_1 //下载提示音开始id

// ETWS Ring
#ifdef MMI_ETWS_SUPPORT
#define MMI_ETWS_RING_START_ID R_ETWS
#define MMI_ETWS_PWS_RING_START_ID R_ETWS_PWS
#endif

//key lock ring
#ifdef MMI_KEY_LOCK_SUPPORT
#define MMI_KEY_LOCK_RING_START_ID R_TP_KEYLOCK //key lock ring start id // bug 2124817
#endif
#ifdef MMI_AZAN_SUPPORT
//Azan Rring
#define MMI_AZAN_RING_START_ID  R_AZAN_MP3_1
#ifdef MMI_AZAN_MATRIX_SUPPORT
#define MMI_AZAN_RING_NUMBER 1
#else
#define MMI_AZAN_RING_NUMBER 14
#endif /*MMI_AZAN_MATRIX_SUPPORT*/
#endif
#ifdef MMI_INDIAN_SOS_SUPPORT
#define MMI_SET_SOS_ALERT_RING_ID R_SOS_ALERT_RING
#endif

#define MMISET_ALARM_RING_DEFAULT_ID    (1)
#define MMISET_ALARM_RING_MAX_NUM       (3)
#define MMISET_RING_DEFAULT_ID          1  //默认铃声
#define MMISET_DC_DEFAULT_RING_ID       1 //相机默认铃声id
#define MMISET_DC_DEFAULT_VOL           MMISET_VOL_THREE        //相机默认铃声音量 
#define MMISET_BAT_DEFAULT_RING_ID      1  //低电量默认铃声id
#define MMISET_BAT_DEFAULT_VOL          MMISET_VOL_THREE         //低电量默认铃声音量
#define MMISET_CHARGE_DEFAULT_RING_ID   1                        //开始充电默认铃声id
#define MMISET_CHARGE_DEFAULT_VOL       MMISET_VOL_SIX           //开始充电默认铃声音量
#define MMISET_ALARM_DEFAULT_VOL        7   //闹钟默认铃声音量
#define MMISET_FLIP_RING_DEFAULT_VOL    MMISET_VOL_THREE //翻盖铃声默认音量
#define MMISET_MSG_RING_DEFAULT_ID      1 //信息铃声默认id
#define MMISET_PLAY_RING_DEFAULT_VOL    MMISET_VOL_THREE //信息铃声默认音量
#define MMISET_KEY_LOCK_RING_DEFAULT_VOL    MMISET_VOL_ONE //锁屏提示音默认音量

#define MMISET_SLIDE_RING1_ID             1
#define MMISET_SLIDE_RING2_ID             2 
#define MMISET_SLIDE_RING_DEFAULT_VOL    MMISET_VOL_FOUR

#define MMISET_DL_DEFAULT_RING_ID       1 //下载提示音默认铃声id
#define MMISET_DL_DEFAULT_VOL           MMISET_VOL_THREE        //下载提示音默认铃声音量
#define MMIPHONE_ADJUST_VOL_MAX_STEPS               MMISET_VOL_MAX //音量调节的最大级别 
#define MMISET_KEYLOCK_DEFAULT_RING_ID       1 //keylock提示音默认铃声id
#ifdef MMI_ETWS_SUPPORT
#define MMISET_ETWS_DEFAULT_ID              1
#define MMISET_ETWS_PWS_DEFAULT_ID              1
#endif

//由于flash大小限制,开机动画只支持2种
#define MMISET_FIXED_POWER_ON_OFF_ANIMATE_MAX_NUM      2 //固定开关机开关机动画的最大数目
#define MMISET_CONTRAST_ONE                 1   //待机屏幕背关亮度1级
#define MMISET_CONTRAST_TWO                 2   //待机屏幕背关亮度2级
#define MMISET_CONTRAST_THREE               3   //待机屏幕背关亮度3级
#define MMISET_CONTRAST_FOUR                4   //待机屏幕背关亮度4级
#define MMISET_CONTRAST_FIVE                5   //待机屏幕背关亮度5级
#define MMISET_CONTRAST_SIX                 6   //待机屏幕背关亮度6级
#define MMISET_CONTRAST_SEVEN               7   //待机屏幕背关亮度7级
#ifdef MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
#define MMISET_TIME_300                    300000//300s
#define MMISET_TIME_60                      60000//60s
#endif
#define MMISET_TIME_30                      30000 //30s
#define MMISET_TIME_25                      25000 //25s
#define MMISET_TIME_20                      20000 //20s
#define MMISET_TIME_15                      15000 //15s
#define MMISET_TIME_10                      10000 //10s
#define MMISET_TIME_5                       5000 //5s
#define MMISET_BACKLIGHT_DEFAULT_TIME       MMISET_EDEFAULT_BACKLIGHT_TIME//MMISET_TIME_15 //待机屏幕背关默认亮度

#ifdef MULTI_THEME_SUPPORT
#define MMISET_THEME_MAX_NUM      2   //主题的最大数目
#endif

#ifdef MMI_PREFER_PLMN_SUPPORT
//根据sim卡的实际情况，可能会有所不同,当前设置为orange的case：85。
#define MMISET_PREFER_PLMN_LIST_MAX             (MN_PHONE_PLMN_SEL_LIST_NUM)
#endif

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/


//the type of key ring and power on/off ring
typedef enum
{
    MMISET_OTHER_SILENT, //静音
    MMISET_OTHER_RING1, //铃声1
    MMISET_OTHER_RING2, //铃声2
    MMISET_MAX_OTHER_RING
} MMISET_OTHER_RING_TYPE_E;

typedef void(*MMISET_RING_CALLBACK_PFUNC)(MMISRVAUD_REPORT_RESULT_E result, DPARAM param);  //播放铃声后的回调函数
typedef void(*MMISET_PRIVACY_CALLBACK_PFUNC)(BOOLEAN is_permitted, DPARAM param_ptr);
typedef void(*MMISET_PRIVACY_APP_ENTRY_CALLBACK_PFUNC)(void);

typedef struct 
{
    MMISET_PRIVACY_CALLBACK_PFUNC call_back_func;
    DPARAM param_ptr;
}MMISET_PRIVACY_CALLBACK_DATA;

//shortcut option 
typedef enum
{
    MMISET_SHORTCUT_START,
    MMISET_SHORTCUT_MAINMENU = MMISET_SHORTCUT_START,
#ifdef TOUCH_PANEL_SUPPORT
    MMISET_SHORTCUT_DIAL,   //dialing
#endif
    MMISET_SHORTCUT_PB,                 //phonebook
    MMISET_SHORTCUT_CL,                  //call log
    MMISET_SHORTCUT_MESSAGE,            //message
    MMISET_SHORTCUT_SMS,                //new sms
#ifdef MMI_AUDIO_PLAYER_SUPPORT
    MMISET_SHORTCUT_MP3,
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
    MMISET_SHORTCUT_MEDIA_PLAY,
#endif
#endif

#ifdef FM_SUPPORT
    MMISET_SHORTCUT_FM_RADIO,           //fm
#endif
#ifdef  CAMERA_SUPPORT
    MMISET_SHORTCUT_CAMERA,             //camera
#endif    
   // MMISET_SHORTCUT_RING,               //choose ring
    MMISET_SHORTCUT_PROFILE,            //profile
    MMISET_SHORTCUT_ALARM,              //alarm 
    //MMISET_SHORTCUT_GAME,               //game
    MMISET_SHORTCUT_DOCUMENT,           //my document 
#ifdef PIC_VIEWER_SUPPORT
    MMISET_SHORTCUT_PIC,                //pic viewer
#endif
#ifdef BLUETOOTH_SUPPORT    
    MMISET_SHORTCUT_BT,                 //bluetooth
#endif  
    MMISET_SHORTCUT_CALENDAR,           //calendar
    MMISET_SHORTCUT_CALC,               //calculate
#ifdef BROWSER_SUPPORT//arvin zhang added for bugzilla 2532
    MMISET_SHORTCUT_WEB,                //web  
#endif

#ifdef EBOOK_SUPPORT
    MMISET_SHORTCUT_EBOOK,              //ebook
#endif
    MMISET_SHORTCUT_SMSINBOX,           //SMS inbox
#ifdef JAVA_SUPPORT 
    MMISET_SHORTCUT_JAVA,               //java
#endif
#ifdef MMI_RECORD_SUPPORT//arvin zhang added for bugzilla 2825
    MMISET_SHORTCUT_RECORDER,           //sound recorder
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE    
    MMISET_SHORTCUT_DUALSIM_SETTING,    //dual sim setting
#endif
#ifdef CMCC_UI_STYLE
    MMISET_SHORTCUT_FETION,          //飞信
    //MMISET_SHORTCUT_MC,             //音乐随身听 
    MMISET_SHORTCUT_MAP,            //地图
    MMISET_SHORTCUT_MONERNET,
#endif
#ifdef CMMB_SUPPORT     
    MMISET_SHORTCUT_CMMB_SETTING,     //cmmb setting
#endif

#ifdef MCARE_V31_SUPPORT
	MMISET_SHORTCUT_MCARE,
	MMISET_SHORTCUT_QBRW,
#endif

#ifdef SEARCH_SUPPORT     
    MMISET_SHORTCUT_SEARCH_SETTING,     //cmmb setting
#endif
#ifdef MMI_WIFI_SUPPORT     
    MMISET_SHORTCUT_WIFI,     //wifi setting
#endif
#ifdef ASP_SUPPORT     
    MMISET_SHORTCUT_ASP,
    MMISET_SHORTCUT_ASP_AUTOPLAY_MESSAGES,
#endif
#ifdef ATV_SUPPORT
    MMISET_SHORTCUT_ATV,//MARYXIAO
#endif
    MMISET_SHORTCUT_MAX
} MMISET_SHORTCUT_TYPE_E;

//方向键设置
typedef enum
{
    MMISET_DIRKEY_UP,           //上键
    MMISET_DIRKEY_DOWN,         //下键
    MMISET_DIRKEY_LEFT,         //左键
    MMISET_DIRKEY_RIGHT,        //右键
    MMISET_DIRKEY_MAX
}MMISET_DIRKEY_E;

//功能键设置
typedef enum
{
    MMISET_FUNCKEY_LEFT,         //左软键
    MMISET_FUNCKEY_RIGHT,        //右软键
    MMISET_FUNCKEY_MAX
}MMISET_FUNCKEY_E;

//---------------------------bin add action start-----------------------//
//动作分类
typedef enum
{
    MMISET_SHORTCUT_ACTION_TYPE_SMART,      //智能键
    MMISET_SHORTCUT_ACTION_TYPE_DIRKEY,     //方向键
    MMISET_SHORTCUT_ACTION_TYPE_FUNCKEY,    //功能键

    MMISET_SHORTCUT_ACTION_TYPE_MAX,
}MMISET_SHORTCUT_ACTION_TYPE_E;

//汇总各种动作，方便做统一处理
typedef enum
{
    MMISET_SHORTCUT_ACTION_OK ,
    MMISET_SHORTCUT_ACTION_CANCEL,
    MMISET_SHORTCUT_ACTION_FUNCKEY_END,

    MMISET_SHORTCUT_ACTION_LEFT = MMISET_SHORTCUT_ACTION_FUNCKEY_END,//MMISET_SHORTCUT_ACTION_DIRKEY_END,
    MMISET_SHORTCUT_ACTION_RIGHT,
    MMISET_SHORTCUT_ACTION_UP,
    MMISET_SHORTCUT_ACTION_DOWN,
    MMISET_SHORTCUT_ACTION_DIRKEY_END,

    MMISET_SHORTCUT_ACTION_SMART_0 = MMISET_SHORTCUT_ACTION_DIRKEY_END,
    MMISET_SHORTCUT_ACTION_SMART_1,
    MMISET_SHORTCUT_ACTION_SMART_2,
    MMISET_SHORTCUT_ACTION_SMART_3,
    MMISET_SHORTCUT_ACTION_SMART_4,

    MMISET_SHORTCUT_ACTION_MAX
}MMISET_SHORTCUT_ACTION_E;

//风格
typedef enum
{
    MMISET_SHORTCUT_TABLE_SMART,        //智能风格
    MMISET_SHORTCUT_TABLE_WIDGET,
    //MMISET_SHORTCUT_TABLE_WITHDCD,      //显示DCD的基本风格
    //MMISET_SHORTCUT_TABLE_NODCD,        //不显示DCD的基本风格
    MMISET_SHORTCUT_TABLE_NORMAL,
    
    MMISET_SHORTCUT_TABLE_MAX,
}MMISET_SHORTCUT_TABLE_TYPE_E;

//对应一种风格的需要自定义的快捷键表
typedef struct 
{
    MMISET_SHORTCUT_ACTION_E *table_arr;
    uint32 table_size;
}MMISET_SHORTCUT_TABLE_T;

//#define MMISET_SHORTCUT_KEY_LEFT        (MMISET_SHORTCUT_TYPE_DIRKEY|MMISET_DIRKEY_LEFT)
//#define MMISET_SHORTCUT_KEY_RIGHT       (MMISET_SHORTCUT_TYPE_DIRKEY|MMISET_DIRKEY_RIGHT)
//#define MMISET_SHORTCUT_KEY_UP          (MMISET_SHORTCUT_TYPE_DIRKEY|MMISET_DIRKEY_UP)
//#define MMISET_SHORTCUT_KEY_DOWN        (MMISET_SHORTCUT_TYPE_DIRKEY|MMISET_DIRKEY_DOWN)
//#define MMISET_SHORTCUT_KEY_OK          (MMISET_SHORTCUT_TYPE_FUNCKEY|MMISET_FUNCKEY_LEFT)
//#define MMISET_SHORTCUT_KEY_CANCEL      (MMISET_SHORTCUT_TYPE_FUNCKEY|MMISET_FUNCKEY_RIGHT)
//#define MMISET_SHORTCUT_SMART_0         (MMISET_SHORTCUT_TYPE_SMART|0)
//#define MMISET_SHORTCUT_SMART_1         (MMISET_SHORTCUT_TYPE_SMART|1)
//#define MMISET_SHORTCUT_SMART_2         (MMISET_SHORTCUT_TYPE_SMART|2)
//#define MMISET_SHORTCUT_SMART_3         (MMISET_SHORTCUT_TYPE_SMART|3)
//#define MMISET_SHORTCUT_SMART_4         (MMISET_SHORTCUT_TYPE_SMART|4)
//
//#define MMISET_SHORTCUT_ACTION_NUM      11

//---------------------------bin add action end-------------------------//
//隐私保护项
typedef enum
{
    MMISET_PROTECT_CL = 0, //通话记录
    MMISET_PROTECT_SMS,  //信息
#ifdef  CAMERA_SUPPORT
    MMISET_PROTECT_CAMER, //相机
#endif
    MMISET_PROTECT_PB, //电话本
    MMISET_PROTECT_MYDOC, //文件管理
#ifdef EBOOK_SUPPORT
    MMISET_PROTECT_EBOOK, //电子书
#endif
#ifdef CALENDAR_SUPPORT
    MMISET_PROTECT_CALENDAR, //日历表
#endif
    MMISET_PROTECT_USB, //u盘
#ifdef MMI_AUDIO_PLAYER_SUPPORT
    MMISET_PROTECT_AUDIO_PLAYER,    //audioplayer
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
    MMISET_PROTECT_VIDEO_PLAYER,    //videoplayer
#endif
#endif
#ifdef ASP_SUPPORT 
    MMISET_PROTECT_ASP, //asp    
#endif
    MMISET_PROTECT_VIDEO, //video
    MMISET_PROTECT_MAX
}MMISET_PRIVACY_PROTECT_TYPE_E;
 
//the type of all ring
typedef enum
{
    MMISET_RING_TYPE_CALL               = 0x00, //来单铃声
    MMISET_RING_TYPE_RECORDER_RECORD    = 0x01,
    MMISET_RING_TYPE_RECORDER_PLAY      = 0x02,
    MMISET_RING_TYPE_MSG                = 0x03, //信息铃声
    MMISET_RING_TYPE_KEY                = 0x04, //按键铃声
    MMISET_RING_TYPE_TKEY               = 0x05, //触屏音
    MMISET_RING_TYPE_POWER              = 0x06, //开关机
    MMISET_RING_TYPE_POWER_OFF          = 0x07, //#ifdef CMCC_UI_STYLE
    MMISET_RING_TYPE_FLIP               = 0x08, //翻盖
    MMISET_RING_TYPE_FLIP_DOWN          = 0x09, //盒盖
    MMISET_RING_TYPE_ALARM              = 0x0a, //闹钟
    MMISET_RING_TYPE_DC                 = 0x0b, //照相
    MMISET_RING_TYPE_ASP                = 0x0c, //#ifdef ASP_SUPPORT
    MMISET_RING_TYPE_DC_COUNT           = 0x0d,
    MMISET_RING_TYPE_DC_FINAL_COUNT     = 0x0e,
    MMISET_RING_TYPE_BAT                = 0x0f, //低电量
    MMISET_RING_TYPE_OTHER              = 0x10, //其他一些下载铃声(预览铃声)等
    MMISET_RING_TYPE_AUDIO_PLAYER       = 0x11, //音频播放器
    MMISET_RING_TYPE_KURO_PLAYER        = 0x12, //kuro播放器
    MMISET_RING_TYPE_FM_PLAYER          = 0x13, //收音机
    MMISET_RING_TYPE_CALL_IN_CALL       = 0x14, //通话中来电
    MMISET_RING_TYPE_MSG_IN_CALL        = 0x15, //通话中来信息
    MMISET_RING_TYPE_MINHINT_IN_CALL    = 0x16, //通话中提示音
    MMISET_RING_TYPE_MYDOCUMENT         = 0x17, //文件管理
    MMISET_RING_TYPE_JAVA               = 0x18, //java
    MMISET_RING_TYPE_SLIDE_ON           = 0x19, //开盖音
    MMISET_RING_TYPE_SLIDE_OFF          = 0x1a, //合盖音
    MMISET_RING_TYPE_DL                 = 0x1b, //download
#ifdef DPHONE_SUPPORT 
    MMISET_RING_TYPE_HOOK_HANDFREE_TONE = 0x1c,
    MMISET_RING_TYPE_HANGUP_REMIND = 0x1d,
    MMISET_RING_TYPE_AZAN				=0x1e,
    MMISET_RING_TYPE_NONE               = 0x1f,
#else
	MMISET_RING_TYPE_AZAN				=0x1c,
    MMISET_RING_TYPE_NONE               = 0x1d,
#endif
#ifdef MMI_KEY_LOCK_SUPPORT
    MMISET_RING_TYPE_KEY_LOCK,
#endif
#ifdef MMI_READMENU_ENABLE
    MMISET_RING_TYPE_READMENU,
#endif
#ifdef WRE_SUPPORT
    MMISET_RING_TYPE_WRE, //wre
#endif
#ifdef MMI_INDIAN_SOS_SUPPORT
    MMISET_RING_TYPE_SOS,
#endif
    MMISET_RING_TYPE_CHARGE,                    // 开始充电提示音

#ifdef MMI_ETWS_SUPPORT
    MMISET_RING_TYPE_ETWS,
    MMISET_RING_TYPE_ETWS_PWS,
#endif
#if defined(MMI_OCR_AUDIO_PLAY_TEST_LONG)// wuxx 20210805
        MMISET_RING_TYPE_OCR_AUDIO_TEST,
#endif

    MMISET_RING_TYPE_MAX
} MMISET_ALL_RING_TYPE_E;

//the type of time display
typedef enum
{
    MMISET_TXT_CODE_TYPE_GB,
    MMISET_TXT_CODE_TYPE_BIG5,
    MMISET_TXT_CODE_TYPE_MAX
 } MMISET_TXT_CODE_TYPE_TYPE_E;
 
//the class of call ring
typedef enum
{
    MMISET_CALL_RING_FIXED,     //固定铃声
    MMISET_CALL_RING_MORE_RING, //更多音乐
    MMISET_CALL_RING_MAX_CLASS
} MMISET_CALL_RING_CLASS_E;

//the class of msg ring
typedef enum
{
    MMISET_MSG_RING_FIXED,      //固定铃声
    MMISET_MSG_RING_MORE_RING,  //更多铃声
    MMISET_MSG_RING_MAX_CLASS
}MMISET_MSG_RING_CLASS_E;

//the type of call ring
typedef enum
{
    MMISET_CALL_RING, //铃声
    MMISET_CALL_VIBRA, //振动
    MMISET_CALL_SILENT, //静音
    MMISET_CALL_VIBRA_BEFORE_RING, //先振动后响铃
    MMISET_CALL_VIBRA_AND_RING, //响铃并振动
    MMISET_MAX_CALL_TYPE
} MMISET_CALL_RING_TYPE_E;

//the type of message ring
typedef enum
{
    MMISET_MSG_RING,
    MMISET_MSG_VIBRA,
    MMISET_MSG_SILENT,
    MMISET_MSG_VIBRA_AND_RING,
    MMISET_MAX_MSG_TYPE
} MMISET_MSG_RING_TYPE_E;

//the type of language, the sequence of language  in resource editor must be identical to it
// moved to mmi_base_common.h

//the type of time display
typedef enum
{
    MMISET_TIME_12HOURS=0,          //12小时制
    MMISET_TIME_24HOURS,            //24小时制
    MMISET_MAX_TIME_DISPLAY_TYPE
 } MMISET_TIME_DISPLAY_TYPE_E;

//the type of date display
typedef enum
{
    MMISET_DATE_YMD,            //年月日
    MMISET_DATE_MDY,            //月日年
    MMISET_DATE_DMY,            //日月年
    MMISET_DATE_DISPLAY_TYPE_MAX
 } MMISET_DATE_DISPLAY_TYPE_E;

//the style of clock display
typedef enum
{
    MMISET_CLOCK_STYLE_ANALOG,              //analog mode
    MMISET_CLOCK_STYLE_DIGIT,               //digit mode
    MMISET_CLOCK_STYLE_MULITIFUNCTION,               //digit mode
    MMISET_CLOCK_STYLE_DYNAMIC_1,
    MMISET_CLOCK_STYLE_DYNAMIC_2,
    MMISET_CLOCK_STYLE_DYNAMIC_3,
    MMISET_MAX_CLOCK_DISPLAY_STYLE
} MMISET_CLOCK_DISPLAY_STYLE_E;

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT    
// 动态墙纸特效类型的总数
#define MMISET_WALLPAPER_SPECIAL_ANIM_NUM   (MMI_LIVE_WALLPAPER_TYPE_MAX)

#else
// 动态墙纸特效类型的总数
#define MMISET_WALLPAPER_SPECIAL_ANIM_NUM   0
#endif

 //the type of wallpaper list
typedef enum
{
    MMISET_WALLPAPER_LIST_DEFAULT,     //默认
    MMISET_WALLPAPER_LIST_FIX_START, //固定墙纸开始
    MMISET_WALLPAPER_LIST_FIX_END = (MMISET_WALLPAPER_LIST_FIX_START + MMISET_WALLPAPER_USER_MAX_NUM-1), //固定墙纸结束
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT    
    MMISET_WALLPAPER_LIST_SPECIAL_START,           //特效墙纸开始
    MMISET_WALLPAPER_LIST_SPECIAL_END = (MMISET_WALLPAPER_LIST_SPECIAL_START+MMISET_WALLPAPER_SPECIAL_ANIM_NUM-1),  //特效墙纸结束
#endif    
#ifdef MMI_VIDEOWALLPAPER_SUPPORT    
    MMISET_WALLPAPER_LIST_VIDEO,           //视频墙纸
#endif    
    MMISET_WALLPAPER_LIST_MORE,           //更多墙纸

    MMISET_WALLPAPER_LIST_MAX
} MMISET_WALLPAPER_LIST_E;

//the type of wallpaper
typedef enum
{
    MMISET_WALLPAPER_FIXED_PIC,     //固定图片
    MMISET_WALLPAPER_FIXED_ANIM,    //固定动画
    MMISET_WALLPAPER_ALBUM,         //相册
    MMISET_WALLPAPER_DOWNLOAD,      //下载图片  
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    MMISET_WALLPAPER_LIVE,
#endif    
#ifdef MMI_VIDEOWALLPAPER_SUPPORT    
    MMISET_WALLPAPER_VIDEO,           //视频墙纸
#endif    
    #if 0//def SLIDESHOW_WALLPAPER_SUPPORT
    MMISET_WALLPAPER_SLIDESHOW,
    #endif
    MMISET_WALLPAPER_MAX
} MMISET_WALLPAPER_TYPE_E;

//#ifdef MMI_PDA_SUPPORT
//set wallpaper 加一个pre，表示是点击墙纸设置时进入的第一个界面(此时还不是墙纸设置界面)见pda_spec
typedef enum
{
    MMISET_WALLPAPER_TYPE_STATIC,//静态墙纸
    MMISET_WALLPAPER_TYPE_MORE_PIC,//更多图片
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    MMISET_WALLPAPER_TYPE_DYNAMIC,//动态墙纸
#endif    
#ifdef MMI_VIDEOWALLPAPER_SUPPORT   
    MMISET_WALLPAPER_TYPE_MORE_VIDEO,//更多视频
#endif
#ifdef SLIDESHOW_WALLPAPER_SUPPORT
    MMISET_WALLPAPER_TYPE_SLIDESHOW,
    #endif
    MMISET_WALLPAPER_TYPE_MAX
} MMISET_WALLPAPER_ALL_TYPE_E;
//#endif

//the type of operate power on password
typedef enum
{
    MMISET_PWD_SET,     //第一次设置密码
    MMISET_PWD_OPEN,    //开启
    MMISET_PWD_MODIFY,  //修改
    MMISET_PWD_CLOSE,   //关闭
    MMISET_PWD_NORMAL,  //正常开机时输入开机密码
    MMISET_PWD_OTHER    //关机充电或者闹钟闹过之后输入开机密码
} MMISET_PWD_OPERATE_TYPE_E;

//#ifdef MMI_MULTI_SIM_SYS_DUAL
typedef enum
{
    MMISET_CALL_FORWARD,
    MMISET_CALL_BAR,
    MMISET_CALL_WAITING,
    MMISET_CALL_HIDE_ID,
    MMISET_SET_NETWORK,
    MMISET_SET_NETWORK_TYPE,
    MMISET_SET_VOLTE_STATUS,
#ifdef MMI_PREFER_PLMN_SUPPORT
    MMISET_SET_PREFER_NETWORK,
#endif
    MMISET_IP_CALL,
    MMISET_FLIP_ANSWER,
#ifdef MMI_CSP_SUPPORT    
    MMISET_LINE_SELECT,
    MMISET_LINE_LOCK,
#endif    
    MMISET_SET_PIN,
    MMISET_SET_PIN2_MODIFY,
    MMISET_SET_FDN,
    MMISET_MP3_PLAYER_CALL_RING,    //在MP3播放界面设置来电铃声
    MMISET_MULTIM_MUSIC_CALL_RING,  //在我的文档音乐界面设置来电铃声
    MMISET_MULTI_SIM_NAME,  
#ifdef MMI_MULTISIM_COLOR_SUPPORT
    MMISET_MULTI_SIM_COLOR,  
#endif    
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
    MMISET_DUALSIM_CALL_DIVERT,  
#endif
    MMISET_SIM_SELECT_TYPE_MAX
}MMISET_SIM_SELECT_TYPE_E;
//#endif

//the type of backlight
typedef enum
{
    MMISET_BACKLIGHT_OPEN,          //打开
    MMISET_BACKLIGHT_OPEN_FOREVER,  //常亮
    MMISET_BACKLIGHT_CLOSE          //关闭
} MMISET_BACKLIGHT_TYPE_E;

//the type of keyboard light
typedef enum
{
    MMISET_KEYBOARD_LIGHT_OPEN,          //打开
    MMISET_KEYBOARD_LIGHT_AUTO,          //自动模式即夜间模式
    MMISET_KEYBOARD_LIGHT_CLOSE          //关闭
}MMISET_KEYBOARD_LIGHT_TYPE_E;

//the type of tv out mode
typedef enum
{
    MMISET_PAL_LINE_BY_LINE = 0,        //PAL line by line mode
    MMISET_NTSC_LINE_BY_LINE,           //NTSC line by line mode
    MMISET_PAL_INTERLEAVING_LINE,       //PAL interleaving line mode
    MMISET_NTSC_INTERLEAVING_LINE,      //NTSC interleaving line mode
    MMISET_TV_OUT_MODE_MAX_NUM
}MMISET_TVOUT_TYPE_E;

//the type of dual sim type
#if defined(MMI_MULTI_SIM_SYS_TRI)
typedef enum
{
    MMISET_DAULSYS_ONLY_SIM1_ON = 0x01,    //只有卡1待机
    MMISET_DAULSYS_ONLY_SIM2_ON = 0x02,    //只有卡2待机
    MMISET_DAULSYS_ONLY_SIM3_ON = 0x04,    //只有卡3待机
    MMISET_DAULSYS_ALL_SIM_ON = 0x07,      //双待机
    MMISET_DAULSYS_MAX  
}MMISET_DUALSYS_TYPE_E;
#elif defined(MMI_MULTI_SIM_SYS_QUAD)
typedef enum
{
    MMISET_DAULSYS_ONLY_SIM1_ON = 0x01,    //只有卡1待机
    MMISET_DAULSYS_ONLY_SIM2_ON = 0x02,    //只有卡2待机
    MMISET_DAULSYS_ONLY_SIM3_ON = 0x04,    //只有卡3待机
    MMISET_DAULSYS_ONLY_SIM4_ON = 0x08,   //只有卡4待机
    MMISET_DAULSYS_ALL_SIM_ON = 0x0F,      //双待机
    MMISET_DAULSYS_MAX  
}MMISET_DUALSYS_TYPE_E;
#else
typedef enum
{
    MMISET_DAULSYS_ONLY_SIM1_ON = 0x01,    //只有卡1待机
    MMISET_DAULSYS_ONLY_SIM2_ON = 0x02,    //只有卡2待机
    MMISET_DAULSYS_ALL_SIM_ON = 0x03,      //双待机
    MMISET_DAULSYS_MAX  
}MMISET_DUALSYS_TYPE_E;
#endif

//the type of major sim select
//Keep the enum data structrue same as MN_DUAL_SYS_TYPE_E !!!
#if defined MMI_MULTI_SIM_SYS_SINGLE
    typedef enum
    {
        MMISET_MAJOR_SIM_SEL_SIM1 = 0,          //sim1
        MMISET_MAJOR_SIM_SEL_MANUAL_SEL ,    //手动选择
        MMISET_MAJOR_SIM_SEL_TYPE_MAX
    }MMISET_MAJOR_SIM_SEL_TYPE_E;
#elif defined MMI_MULTI_SIM_SYS_DUAL
    typedef enum
    {
        MMISET_MAJOR_SIM_SEL_SIM1= 0,          //sim1
        MMISET_MAJOR_SIM_SEL_SIM2,          //sim2   
        MMISET_MAJOR_SIM_SEL_MANUAL_SEL,    //手动选择
        MMISET_MAJOR_SIM_SEL_TYPE_MAX
    }MMISET_MAJOR_SIM_SEL_TYPE_E;
#elif defined MMI_MULTI_SIM_SYS_TRI
    typedef enum
    {
        MMISET_MAJOR_SIM_SEL_SIM1= 0,          //sim1
        MMISET_MAJOR_SIM_SEL_SIM2,          //sim2 
        MMISET_MAJOR_SIM_SEL_SIM3,          //sim3
        MMISET_MAJOR_SIM_SEL_MANUAL_SEL,    //手动选择
        MMISET_MAJOR_SIM_SEL_TYPE_MAX
    }MMISET_MAJOR_SIM_SEL_TYPE_E;
#elif defined MMI_MULTI_SIM_SYS_QUAD
    typedef enum
    {
        MMISET_MAJOR_SIM_SEL_SIM1= 0,          //sim1
        MMISET_MAJOR_SIM_SEL_SIM2,          //sim2   
        MMISET_MAJOR_SIM_SEL_SIM3,          //sim3
        MMISET_MAJOR_SIM_SEL_SIM4,          //sim4
        MMISET_MAJOR_SIM_SEL_MANUAL_SEL,    //手动选择
        MMISET_MAJOR_SIM_SEL_TYPE_MAX
    }MMISET_MAJOR_SIM_SEL_TYPE_E;
#else
#error need MMISET_MAJOR_SIM_SEL_TYPE define
#endif

//the type of major sim sel function
typedef enum
{
    MMISET_MAJOR_SIM_SEL_FUNC_CALL,
    MMISET_MAJOR_SIM_SEL_FUNC_SMS,
    MMISET_MAJOR_SIM_SEL_FUNC_MMS,
    MMISET_MAJOR_SIM_SEL_FUNC_WAP,
    MMISET_MAJOR_SIM_SEL_FUNC_SPEED_DIAL,
    MMISET_MAJOR_SIM_SEL_FUNC_STREAMING,
    MMISET_MAJOR_SIM_SEL_FUNC_MAX
}MMISET_MAJOR_SIM_SEL_FUNC_TYPE_E;

//the type of answer with original sim
typedef enum
{
    MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_CALL,
    MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_SMS,
#ifndef MMIMMS_SMS_IN_1_SUPPORT
    MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MMS,
#endif    
#if 0
    MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_WAP,
 #endif
    MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX
}MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_TYPE_E;

//idle style
typedef enum
{
    MMISET_IDLE_STYLE_COMMON,   //common style
    MMISET_IDLE_STYLE_SMART,    //smart style
    MMISET_IDLE_STYLE_WIDGET,   //widget style
    MMISET_IDLE_STYLE_GRID,     //grid style
    MMISET_IDLE_ISTYLE,         //istyle
    MMISET_IDLE_QBTHEME,        //千变主题
    MMISET_IDLE_STYLE_MAX
} MMISET_IDLE_STYLE_E;
//the type of keylock
//idle substyle
typedef enum
{
    MMISET_IDLE_SUBSTYLE_DEFAULT,   //common style
#ifdef MMI_SAM_THEME
    MMISET_IDLE_SUBSTYLE_SAM,        //三星主题
#endif
    MMISET_IDLE_SUBSTYLE_MAX
} MMISET_IDLE_SUBSTYLE_E;

//the type of keylock
typedef enum
{
    MMISET_KEYLOCK_CLOSE,          //关闭
#if !defined(MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT)
    MMISET_KEYLOCK_5_SEC = 5000,   //5s
#endif    
    MMISET_KEYLOCK_15_SEC = 15000,   //15s
    MMISET_KEYLOCK_30_SEC = 30000,    //30s
    MMISET_KEYLOCK_1_MIN = 60000,   //1min
    MMISET_KEYLOCK_5_MIN = 300000,   //5min
    MMISET_KEYLOCK_MAX
} MMISET_KEYLOCK_TYPE_E;

//the selection index of keylock
typedef enum
{
    MMISET_KEYLOCK_SELECT_CLOSE,          //关闭
#if !defined(MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT)
    MMISET_KEYLOCK_SELECT_5_SEC ,   //5s
#endif    
    MMISET_KEYLOCK_SELECT_15_SEC,   // 15s
    MMISET_KEYLOCK_SELECT_30_SEC,  // 30s
    MMISET_KEYLOCK_SELECT_1_MIN,   // 1 min
    MMISET_KEYLOCK_SELECT_5_MIN,   //5min
    MMISET_KEYLOCK_SELECT_MAX
} MMISET_KEYLOCK_SELECT_TYPE_E;


//the type of slide open
typedef enum
{
    MMISET_RECEIVE_PHONE_TYPE,          //接听电话
    MMISET_REJECT_PHONE_TYPE,           //不接听电话
    MMISET_MAX_SLIDE_OPEN_TYPE
 } MMISET_SLIDE_OPEN_TYPE_E;

//the type of slide close
typedef enum
{
    MMISET_STOP_OPERATION_TYPE,          //停止操作
    MMISET_CONTINUE_OPERATION_TYPE,      //继续操作
    MMISET_MAX_SLIDE_CLOSE_TYPE
 } MMISET_SLIDE_CLOSE_TYPE_E;

//the type of keylock switch
typedef enum
{
    MMISET_KEYLOCK_SWITCH_ON_TYPE,          //启动键盘锁
    MMISET_KEYLOCK_SWITCH_OFF_TYPE,         //不启动键盘锁
    MMISET_MAX_KEYLOCK_SWITCH_TYPE
 } MMISET_KEYLOCK_SWITCH_TYPE_E;

#ifdef VT_SUPPORT
typedef enum
{
    MMISET_VT_FALLBACK_QUERY, //用户询问是否接受回落
    MMISET_VT_FALLBACK_QUIT, //自动放弃
    MMISET_VT_FALLBACK_ACCEPT, //自动接受回落
    MMISET_VT_FALLBACK_MAX
}MMISET_VT_FALLBACK_HANDLE_MODE_E;

typedef enum
{
    MMISET_VT_REPLACE_LOCAL_IMAGE,  //本端替代画面 图片
    MMISET_VT_REPLACE_REMOTE_IMAGE, //远端替代画面 图片
    MMISET_VT_REPLACE_MAX
}MMISET_VT_REPLACE_TYPE_E;

// 替代资源的类型
typedef enum
{
    MMISET_VT_REPLACE_MEDIA_TYPE_IMAGE,  //图片
    MMISET_VT_REPLACE_MEDIA_TYPE_VIDEO, //影像
    MMISET_VT_REPLACE_MEDIA_TYPE_MAX
}MMISET_VT_REPLACE_MEDIA_TYPE_E;

typedef enum
{
    MMISET_VT_REPLACE_REMOTE_IMG, //使用远端替代图像
    MMISET_VT_DISPLAY_REMOTE_SENDING_IMG, //显示对方传输图像
    MMISET_VT_REMOTE_SHUTDOWN_CAMERA_MAX
}MMISET_VT_REMOTE_SHUTDOWN_CAMERA_OPTION; //对端关闭摄像头的处理方式

typedef struct 
{
	BOOLEAN is_valid;							// 是否有效
	wchar	file_name[MMIFILE_FULL_PATH_MAX_LEN +1];  //注意结束符号
	uint32	file_name_len;						// 文件路径长度
												// 
	uint32	file_size;							// 文件大小
    MMISET_VT_REPLACE_MEDIA_TYPE_E media_type;  //
	uint32	photo_type;							// 图片的类型
    uint32  video_type;
}MMISET_VT_REPLACE_FILE_INFO_T;
#endif
typedef uint8 MMISET_IP_NUMBER_T[MMISET_IPNUMBER_MAX_LEN + 1]; //以0结尾

//add for dual sys
typedef struct
{
    BOOLEAN is_default;
    uint16  wstr_len;
    wchar   wstr_arr[MMISET_MAX_SIM_NAME_LEN + 1];
}MMISET_SIM_NAME_T;

#ifdef MMI_MULTISIM_COLOR_SUPPORT
//Sam@Jan_06    Add for SIM COLOR
typedef struct
{
    uint16      index;
    GUI_COLOR_T bg_color;
}MMISET_SIM_COLOR_T;
#endif

//墙纸的信息，包括墙纸类型，墙纸id
typedef struct
{
    uint16                      wallpaper_id;   //the id of wallpaper
    //uint16                      pre_wallpaper_id;   //the pre id of wallpaper
    MMISET_WALLPAPER_TYPE_E     wallpaper_type; //the type of wallpaper  
    //MMISET_WALLPAPER_TYPE_E     pre_wallpaper_type; //the type of wallpaper 
}MMISET_WALLPAPER_SET_T;

//来电铃声为更多铃声的信息
typedef struct  
{   
    wchar               name_wstr[MMISET_CALL_RING_NAME_MAX_LEN + 1];   //包括设备名，路径名和文件名等,16bit unicode
    uint16              name_wstr_len;                              //名字的长度
}MMISET_CALL_MORE_RING_T;
#ifdef SCREENSAVER_SUPPORT
//@xingdong.li
typedef struct  
{   
    wchar               name_wstr[MMISET_SCREENSAVER_NAME_MAX_LEN + 1];  //包括设备名，路径名和文件名等,16bit unicode
    uint16              name_wstr_len;                               //名字的长度
}MMISET_MORE_SCREENSAVER_T;

//@xingdong.li
typedef enum
{
    MMISET_SCREENSAVER_CLOSED, //关闭
    MMISET_SCREENSAVER_ANIM1, //屏保1
    //MMISET_SCREENSAVER_ANIM2,
    MMISET_SCREENSAVER_SEA_WORLD = MMISET_SCREENSAVER_ANIM1,//海洋世界屏保
    MMISET_SCREENSAVER_SPECIAL_BEGIN, //算法屏保
    MMISET_SCREENSAVER_STAR = MMISET_SCREENSAVER_SPECIAL_BEGIN,//星屏保
    MMISET_SCREENSAVER_SNAKE, //贪吃蛇屏保
    MMISET_SCREENSAVER_SEED,  //生命游戏屏保
    MMISET_SCREENSAVER_SPECIAL_END = MMISET_SCREENSAVER_SEED, //算法屏保结束
    MMISET_SCREENSAVER_MORE_PICTURE, //更多屏保
#ifdef VIDEO_PLAYER_SUPPORT
    MMISET_SCREENSAVER_MORE_ANIMATION, 
#endif    

    MMISET_SCREENSAVER_MAX_NUM
} MMISET_SCREENSAVER_TYPE_E;

//@xingdong.li
typedef struct
{
    BOOLEAN                   is_open;        //is open flag of screensaver.
    MMISET_SCREENSAVER_TYPE_E ssaver_type;    //screensaver type
    //MMISET_SCREENSAVER_MORE_SELECTION_E ssaver_more_select;
    MMISET_MORE_SCREENSAVER_T more_pic_saver_info;//info of more picture screensaver.
//#ifdef VIDEO_PLAYER_SUPPORT
    MMISET_MORE_SCREENSAVER_T more_ani_saver_info;//info of more animation screensaver.
//#endif

} MMISET_SCREENSAVER_INFO_T;
#endif
//@xingdong.li
typedef enum
{
    MMISET_POWER_ON_SET_TYPE, //开机
    MMISET_POWER_OFF_SET_TYPE, //关机设置
    MMISET_POWER_ON_OFF_SET_TYPE_MAX_NUM
}MMISET_POWER_ON_OFF_SET_TYPE_E;

//@xingdong.li
typedef struct  
{   
    wchar       name_wstr[MMISET_POWER_ON_OFF_ANIM_NAME_MAX_LEN+1];  //包括设备名，路径名和文件名等,16bit unicode
    uint16      name_wstr_len;                                       //名字的长度
}MMISET_MORE_POWER_ON_OFF_ANIM_T; //更多开关机动画文件名信息

//@xingdong.li
typedef enum
{
    //    MMISET_POWER_ON_OFF_ANIM_CLOSED,
    MMISET_POWER_ON_OFF_ANIM_DEFAULT,  //默认开关机动画
    MMISET_POWER_ON_OFF_ANIM_MORE, //更多开关机动画
    MMISET_POWER_ON_OFF_PIC_MORE,
    MMISET_POWER_ON_OFF_ANIM_MAX_NUM
} MMISET_POWER_ON_OFF_ANIM_TYPE_E;

//@xingdong.li
typedef struct
{
    BOOLEAN                         is_open;        //open flag of power on animation.
    MMISET_POWER_ON_OFF_ANIM_TYPE_E anim_type;      //power on animation type
    MMISET_MORE_POWER_ON_OFF_ANIM_T more_anim_info; //info of more animation.
} MMISET_POWER_ON_OFF_ANIM_INFO_T;  //开关机动画信息

//来电铃声的信息，包括铃声文件名，以及来电铃声类
typedef struct
{
  //  MN_DUAL_SYS_E               dual_sys;
    MMISET_CALL_RING_CLASS_E    call_ring_type; //铃声类型：固定铃声和更多铃声
    uint16                      fix_ring_index; //固定铃声的index
    MMISET_CALL_MORE_RING_T     more_ring_info; //更多铃声的信息
}MMISET_CALL_RING_T;

//信息铃声的相关信息
typedef struct
{
  //  MN_DUAL_SYS_E               dual_sys;
    MMISET_MSG_RING_CLASS_E     msg_ring_type; //信息铃声类型：固定铃声和更多铃声
    uint16                      fix_ring_index; //固定铃声的index
    MMISET_CALL_MORE_RING_T     more_ring_info; //更多铃声的信息
}MMISET_MSG_RING_T;

//来电铃声的信息，包括铃声文件名，以及来电铃声类
typedef struct
{
  //  MN_DUAL_SYS_E               dual_sys;
    MMISET_CALL_RING_CLASS_E    call_ring_type; //铃声类型：固定铃声和更多铃声
    uint16                      fix_ring_index; //固定铃声的index    
}MMISET_CALL_RING_INFO_T;

//信息铃声的相关信息
typedef struct
{
  //  MN_DUAL_SYS_E               dual_sys;
    MMISET_MSG_RING_CLASS_E     msg_ring_type; //信息铃声类型：固定铃声和更多铃声
    uint16                      fix_ring_index; //固定铃声的index   
}MMISET_MSG_RING_INFO_T;

typedef struct
{
    MMISET_MSG_RING_CLASS_E     alarm_ring_type; //闹钟铃声类型：固定铃声和更多铃声
    uint16                      fix_ring_index; //固定铃声的index
}MMISET_ALARM_RING_INFO_T;

//the type of line
typedef enum
{
    MMISET_LINE_1,                    //line 1
    MMISET_LINE_2,                    //line 2
    MMISET_MAX_LINE
} MMISET_LINE_TYPE_E; 


//the infomation about backlight
typedef struct
{
    uint32                  backlight_time; //the time of backlight
    MMISET_BACKLIGHT_TYPE_E backlight_type; //the type of backlight
}MMISET_BACKLIGHT_SET_T;

//the information about any key and reject mode
typedef struct  
{
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
    BOOLEAN             is_sms_reply_open;  //if replay sms after reject call
#endif
    BOOLEAN             is_anykey_answer;    //if anykey answer the call
}MMISET_ANYKEY_REJECT_SET_T;

//the information about keyboard time
typedef struct  
{
    uint32                          keyboard_light_time; // the time of keyboard light
    MMISET_KEYBOARD_LIGHT_TYPE_E    keyboard_type;       // the type of keyboard light
}MMISET_KEYBOARD_LIGHT_SET_T;

//the information about idle display
typedef struct  
{
    union
    {
        BOOLEAN    flag_array[16];
        struct {
            BOOLEAN    is_show_time_and_date;                   //if show time and date info on the idle win
            BOOLEAN    is_show_network_name;                    //if show network name info
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD ||  defined MMI_MULTI_SIM_SYS_DUAL                                                             
            BOOLEAN    is_show_network_name2;                    //if show network name info
#endif        
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD                                                           
            BOOLEAN    is_show_network_name3;                    //if show network name info
#endif        
#if defined MMI_MULTI_SIM_SYS_QUAD                                                           
            BOOLEAN    is_show_network_name4;                    //if show network name info
#endif        
            BOOLEAN    is_show_today_schedule;
#ifdef COUNTEDTIME_SUPPORT
            BOOLEAN    is_show_countdown;
#endif        
#ifdef MMI_AUDIO_PLAYER_SUPPORT
            BOOLEAN    is_show_mp3;
#endif
#ifdef FM_SUPPORT
            BOOLEAN    is_show_fm;
#endif
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
#if !(defined(MMI_DISPLAYSET_MINI_SUPPORT) && defined(MMI_ENABLE_DCD)) 
            BOOLEAN    is_show_idle_words;
#endif
#endif                   
#ifdef MMI_IDLE_STKTEXT_SUPPORT
            BOOLEAN    is_show_stk_idle_text;
#endif
#ifdef DCD_SUPPORT
            BOOLEAN    is_show_dcd;
#endif
#ifdef MMI_WIFI_SUPPORT
            BOOLEAN    is_show_wlan;
#endif
            
            BOOLEAN    is_show_shortcut;
        }item_flag;        
    }visible_flag;
    wchar      idle_words[MMISET_IDLE_WORDS_UCS_MAX_LEN + 1];   //idle win display words, ucs2 code
    uint16     wstr_len;                                     //the words length of idle display
}MMISET_IDLE_DISPLAY_T;

//idle shortcut info
typedef struct
{
    MMISET_SHORTCUT_TYPE_E  idle_shortcut[MMISET_IDLE_SHORTCUT_MAX_NUM]; //idle快捷键信息
} MMISET_IDLE_SHORTCUT_T;

//Fk shortcut info
typedef struct
{
    MMISET_SHORTCUT_TYPE_E  fk_shortcut[MMISET_FUNCKEY_MAX]; //待机功能快捷键信息
} MMISET_FK_SHORTCUT_T;

typedef struct
{
    MMISET_SHORTCUT_TYPE_E direction_shortcut[ MMISET_DIRKEY_MAX ]; //方向快捷键信息
}MMISET_DIRECTIONKEY_SHORTCUT_T;

#if defined(MMI_BLACKLIST_SUPPORT)
typedef struct
{
    uint16 name_len;
    wchar  name[MMIPB_NAME_MAX_LEN];
    uint16  num_len;
    wchar  num[MMISET_PHONE_NUM_MAX_LEN];
}MMISET_BLACKLIST_ITEM_T;

typedef struct
{
    uint16 num;
    MMISET_BLACKLIST_ITEM_T blacklist[MMISET_BLACKLIST_MAX_NUM]; 
}MMISET_BLACKLIST_T;
#endif

typedef enum
{
    MMISET_THEME_DEFAULT_TYPE,      // default资源
    MMISET_THEME_STATIC_TYPE,       // 和default资源打包在一起的存放在flash中的资源
    MMISET_THEME_FILE_TYPE,         // 放在文件系统中的资源
    MMISET_THEME_TYPE_MAX
}MMISET_THEME_TYPE_E;

//the time of call reminder time
typedef enum
{
    MMISET_CALL_REMINDER_TIME_OFF,          //0s
    MMISET_CALL_REMINDER_TIME_50S,          //50s
    MMISET_CALL_REMINDER_TIME_55S,          //55s
    MMISET_CALL_REMINDER_TIME_60S,          //60s
    MMISET_CALL_REMINDER_TIME_OTHER,        //other
    MMISET_CALL_REMINDER_TIME_MAX
}MMISET_CALL_REMINDER_TIME_TYPE_E;

//append sim type
typedef enum
{
    MMISET_APPEND_SYS_OK,//包括ps 
    MMISET_APPEND_SIM_OK,//不包括ps
    MMISET_APPEND_PS_READY,//仅ps
    MMISET_APPEND_PIN1_PERMIT,//pin1
    MMISET_APPEND_STK_CARD,//stk
    MMISET_APPEND_SIM_ALL,//all
#ifdef MMI_GPRS_DATA_SIM_SET
    MMISET_APPEND_GPRS_MODE, // 根据GPRS mode 选卡 
#endif
    MMISET_APPEND_SIM_TYPE_MAX
}MMISET_APPEND_SIM_TYPE_E;

#ifdef MMI_GPRS_DATA_SIM_SET
typedef enum
{
    DATA_CARD_MODE_1 = 0,             //数据卡为卡1
    DATA_CARD_MODE_2,                 //数据卡为卡2
    DATA_CARD_MODE_3,                 //数据卡为卡3
    DATA_CARD_MODE_4,                 //数据卡为卡4

    DATA_CARD_MANUAL_SELECT = 4,      //始终提示选择卡
    DATA_CARD_MODE_MAX
}MMISET_GPRS_MODE_TYPE_E;

//函数MNPHONE_GetProductForm()  返回值类型

#define  PRODUCT_FORM_TYPE_TG            0  //T+G 产品
#define  PRODUCT_FORM_TYPE_TTGG          1  //TTGG
#define  PRODUCT_FORM_TYPE_ORI           2  //原有产品
#define  PRODUCT_FORM_TYPE_FIX_SLOT      3   //固定卡槽
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
//the result of add blacklist
typedef enum
{
    MMISET_BLACKLIST_ERROR_NONE,
    MMISET_BLACKLIST_ERROR_BEYOND_MAX_NUM,
    MMISET_BLACKLIST_ERROR_NUM_EMPTY,
    MMISET_BLACKLIST_ERROR_SAME_NUM,
    MMISET_BLACKLIST_ERROR_PARAM_NULL,
    MMISET_BLACKLIST_ERROR_MAX
} MMISET_BLACKLIST_ERROR_TYPE_E;
#endif

//省电模式设置
typedef enum 
{
    MMISET_POWER_SAVING_SETTING_TWE,//20%剩余电量
    MMISET_POWER_SAVING_SETTING_FOURTY,//40%剩余电量
    MMISET_POWER_SAVING_SETTING_SIXTY,//60%剩余电量
    MMISET_POWER_SAVING_SETTING_MAX
}MMISET_POWER_SAVING_SETTING_E;

typedef struct//fb=forbidden
{
    BOOLEAN is_active;//标志位，判断省电模式是否真正开启
    BOOLEAN is_fb_bg_data;//背景数据
#ifdef MMI_WIFI_SUPPORT
    BOOLEAN is_fb_wlan;//WALN
#endif
#ifdef BLUETOOTH_SUPPORT
    BOOLEAN is_fb_bluetooth;//BLUETOOTH
#endif
    BOOLEAN is_fb_anim;
    BOOLEAN is_fb_ss;//屏幕保护
    BOOLEAN is_fb_keypad_light;
    BOOLEAN is_fb_anim_wallpaper;
    BOOLEAN is_fb_alert_ring;
    uint16  lcd_contrast;
    MMISET_POWER_SAVING_SETTING_E  schedule_setting;    
}MMISET_POWER_SETTING_ITEM_T;

#if defined(MMI_BLACKLIST_SUPPORT)
typedef struct
{
    MMIPB_BCD_NUMBER_T    pb_bcd;
    MMI_STRING_T          mmi_pb_name;
}MMISET_BLACKLIST_NAME_NUM_ITEM_T;
#endif
#ifdef PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER
typedef enum
{
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
    MMISET_EFFECT_RENDER_SETTING_GO_MENU,
#endif
    MMISET_EFFECT_RENDER_SETTING_MAX
}MMISET_EFFECT_RENDER_SETTING_E;
typedef struct
{
    BOOLEAN render_array[MMISET_EFFECT_RENDER_SETTING_MAX + 1];//是否支持光影效果
}MMISET_EFFECT_RENDER_SETTING;
#endif

#ifdef MMI_POWER_SAVING_MODE_SUPPORT
typedef enum
{
    MMISET_PS_SETTING_SCH,//省电模式时间表
    MMISET_PS_SETTING_BG,
#ifdef MMI_WIFI_SUPPORT
    MMISET_PS_SETTING_WLAN,
#endif
#ifdef BLUETOOTH_SUPPORT
    MMISET_PS_SETTING_BT,
#endif
    MMISET_PS_SETTING_LIST1_MAX
}MMISET_PS_SETTING_LIST1_E;

typedef enum
{
    MMISET_PS_SETTING_ANIM,//省电模式时间表
#ifdef SCREENSAVER_SUPPORT
    MMISET_PS_SETTING_SS,
#endif
    MMISET_PS_SETTING_KEYPAD,
    MMISET_PS_SETTING_WALLPAPER,
    MMISET_PS_SETTING_ALERT,
    MMISET_PS_SETTING_LIST2_MAX
}MMISET_PS_SETTING_LIST2_E;

#endif

#ifdef MMI_CSP_SUPPORT
typedef enum
{
    MMISET_LINE_IS_LOCK,  //lock
    MMISET_LINE_IS_UNLOCK, //unlock
    MMISET_LINE_LOCK_INVALID, //line lock is invalid
    MMISET_LINE_LOCK_MAX
}MMISET_LINE_LOCK_STATUS_E;
#endif

#ifdef MMI_CPHS_SUPPORT
typedef struct
{
    BOOLEAN is_cphs_valid;
    BOOLEAN is_cfu_saved; //saved in sim
#ifdef MMI_CSP_SUPPORT    
    BOOLEAN is_csp1_valid;
    BOOLEAN is_csp2_valid;
    MN_CSP_SERVICES_GROUP_T csp1_service_group;
    MN_CSP_LINE2_SERVICES_GROUP_T csp2_service_group;
#endif    
}MMISET_CPHS_INFO_T;
#endif

#ifdef MMI_INDIAN_SOS_SUPPORT
typedef void(*MMISET_SOS_CALLBACK_PFUNC)(DPARAM param);

/*The following is the entry style description between PDA and non-PDA devices */
#ifndef MMI_PDA_SUPPORT
#define MMISET_SOS_ENTRY_CODE MSG_KEYLONG_5
#else
#define MMISET_SOS_ENTRY_MAGIC_TICK 3
#define MMISET_SOS_ENTRY_TIME_LIMIT 2000
#endif
#define MMISET_SOS_CONTACTS_MAX_NUM 5 /*five sos contacts at most according to the Req*/
#define MMISET_SOS_MSG_CONTENT_MAX_LEN 75 //(MMISMS_DEFAULT_CHAR_LEN/2 - 1) /*temp value based on the relationship between wchar and uint8*/

typedef struct
{
	uint16 msg_len;/*actual msg content length*/
	wchar msg_content[MMISET_SOS_MSG_CONTENT_MAX_LEN] ;/*sos message content*/
}MMISET_SOS_MESSAGE_CONTENT_T;

typedef struct
{
	/*maybe need support contact name later*/
	//uint16 name_len;
	//wchar name[MMIPB_NAME_MAX_LEN];
	uint16 num_len;/*actual number length*/
	wchar num[MMISET_PHONE_NUM_MAX_LEN];/*contact's number*/
}MMISET_SOS_CONTACT_ITEM_T;
#endif//MMI_INDIAN_SOS_SUPPORT

#ifdef ADULT_WATCH_SUPPORT
//the infomation about watch wear
typedef enum
{
    MMISET_WATCH_WEAR_TYPE_LEFT,
    MMISET_WATCH_WEAR_TYPE_RIGHT,
    MMISET_WATCH_WEAR_TYPE_MAX
}MMISET_WATCH_WEAR_E;
#endif
/*****************************************************************************/
//  Description :??common gsensor???
//  Global resource dependence : 

#ifdef MMI_CPHS_SUPPORT
/*****************************************************************************/
// 	Description : deal with signal of APP_MN_CPHS_READY_IND
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISET_HandleCPHSReadyInd(
                                       DPARAM              param
                                       );

#endif

/*****************************************************************************/
// 	Description : is als valid
//	Global resource dependence : 
//  Author: samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsCfuStatusSavedInSim(
                                       MN_DUAL_SYS_E dual_sys
                                       ) ;

#ifdef MMI_INDIAN_SOS_SUPPORT
/*****************************************************************************/
// 	Description : start sos procedures 
//	Global resource dependence : 
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_StartSOSProcedures(void);

/*****************************************************************************/
// 	Description : enter sos setting window 
//	Global resource dependence : 
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterSOSSettingWin(void);

/*****************************************************************************/
// 	Description : get sos switch on/off 
//	Global resource dependence : s_sos_enabled
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetSOSSwitchState(void);

#ifdef MMIDC_F_FLASH
/*****************************************************************************/
// 	Description : get sos led alert switch on/off 
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetSOSLEDAlertSwitchState(void);
#endif

/*****************************************************************************/
// 	Description : get sos ring alert switch on/off 
//	Global resource dependence : none
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetSOSRingAlertSwitchState(void);

/*****************************************************************************/
// 	Description : get sos msg sent switch on/off 
//	Global resource dependence : s_sos_msg_sent_enabled
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetSOSMsgSentSwitchState(void);

/*****************************************************************************/
// 	Description : get sos msg info 
//	Global resource dependence : s_sos_msg_info
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC MMISET_SOS_MESSAGE_CONTENT_T* MMIAPISET_GetSOSMsgContent(void);

/*****************************************************************************/
// 	Description : get sos contact list  info 
//	Global resource dependence : s_sos_contact_list
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC MMISET_SOS_CONTACT_ITEM_T* MMIAPISET_GetSOSContactListInfo(void);

/*****************************************************************************/
// 	Description : get if sos is running or not
//	Global resource dependence : s_is_sos_running
//  Author: Arvin.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsSOSProcRunning(void);
#endif//MMI_INDIAN_SOS_SUPPORT

/*****************************************************************************/
//  Description :更新common gsensor的状态
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_UpdateCommonGSensor();
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : interface to choose wallpaper
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/

PUBLIC void MMIAPISET_SetWallpaper(void);


/*****************************************************************************/
//  Description : interface to choose game wallpaper
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/

PUBLIC void MMIAPISET_SetGameWallpaper(void);


/*****************************************************************************/
//  Description : interface to choose film wallpaper
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/

PUBLIC void MMIAPISET_SetFilmWallpaper(void);
/*****************************************************************************/
//  Description : get the volume of call ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
uint8 MMIAPISET_GetCallRingVolume(void);

/*****************************************************************************/
//  Description : init all setting 
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_AllInit(void);
        
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
                                         );

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
                                         );
                 
/*****************************************************************************/
//  Description : close validate privacy password window
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_CloseValidatePrivacyWin(MMISET_PRIVACY_PROTECT_TYPE_E protect_type);

/*****************************************************************************/
//  Description : check if privacy password window is open
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsValidatePrivacyWinOpen(MMISET_PRIVACY_PROTECT_TYPE_E protect_type);

/*****************************************************************************/
//  Description : if privacy validate is needed to do again
//  Global resource dependence : none
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetIsNeedValidatePrivacy(BOOLEAN is_need);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetValidatePrivacyType(void);
/*****************************************************************************/
//  Description : get line info from nv and update to mn
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetCallLineInfo(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :open voice message divert call number window
//  Global resource dependence : 
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenDivertCallWin(uint8   mail_len,
    uint8*  voice_mail_ptr
    );

/*****************************************************************************/
//  Description :get voice mail address record id
//  Global resource dependence : 
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetVMRecordId(void);

/*****************************************************************************/
//  Description : if app module open privacy protect
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsOpenPrivacyProtect( 
    MMISET_PRIVACY_PROTECT_TYPE_E   protect_type
    );
    
/*****************************************************************************/
//  Description : is phone in vibrate status, used in cc recently
//  Global resource dependence : none
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsVibratorNow(void);

/*****************************************************************************/
//  Description : set vibrator
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetVibrator(
    BOOLEAN                 is_open,
    MMISET_ALL_RING_TYPE_E  ring_type//in: the type of all vibrator
    );

/*****************************************************************************/
//  Description : set vibrator periodly
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetVibratorEx(
                                  BOOLEAN                 is_open,
                                  MMISET_ALL_RING_TYPE_E  ring_type,//in: the type of all vibrator
                                  BOOLEAN is_period, //是否间断不停的振动
                                  uint32 duration   //振动间隔
                                  );
//  Description : Stop all vibrator
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_StopAllVibrator(void);

/*****************************************************************************/
//  Description : open select fixed ring window for phone book
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenFixCallRingWin(
    MN_DUAL_SYS_E dual_sys,
    MMI_WIN_ID_T    win_id,
    uint16        selected_id
    );

/*****************************************************************************/
//  Description : get the backlight value
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPISET_GetRealBacklightValue(void);

/*****************************************************************************/
//  Description : play ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_PlayRing(
    MN_DUAL_SYS_E               dual_sys,
                BOOLEAN                    is_preview, 
                         uint8                      group_id,   //group_id or alarm_id,来电铃声才关心此参数!
                         uint32                     play_times, //the times of play ring
                         MMISET_ALL_RING_TYPE_E     ring_type,  //in: the type of all ring
                         MMISET_RING_CALLBACK_PFUNC callback_func
                         );

/*****************************************************************************/
//  Description : get clir type
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC MN_CALL_CLIR_E  MMIAPISET_GetClirType(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :设置是否已经查询了CFU的状态
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetIsQueryCfu( BOOLEAN is_query,MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :Reset dual sys query info
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetDualsysQueryInfo(void);

/*****************************************************************************/
//  Description :获得是否已经完成了CFU查询
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsQueryCfu(void);

/*****************************************************************************/
//  Description : get the type of tv out mode
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC MMISET_TVOUT_TYPE_E  MMIAPISET_GetTVOutMode( void );

/*****************************************************************************/
//  Description : InterrogateCfuStatusFromPs
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_InterrogateCfuStatusFromPs(                                                 
    MN_DUAL_SYS_E dual_sys,
    uint16      line_index
    );

/*****************************************************************************/
//  Description : 设置完更多来电铃声后，将信息存入nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
//PUBLIC void MMIAPISET_SetMoreCallRingInfo(
//    MN_DUAL_SYS_E dual_sys,
//    MMISET_CALL_MORE_RING_T ring_info
//     );

/*****************************************************************************/
//  Description : play ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_PlayCallRingById(
    uint16                      ring_id,
    uint32                     play_times, //the times of play ring
    MMISET_RING_CALLBACK_PFUNC callback_func
    );



/*****************************************************************************/
//  Description : get the mode of device
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetDeviceMode(
    AUDIO_DEVICE_MODE_TYPE_E    *mode_ptr//out
    );

/*****************************************************************************/
//  Description : API of get ring current type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC MMISET_ALL_RING_TYPE_E MMIAPISET_GetCurRingType(
                                    void
                                    );

/*****************************************************************************/
//  Description : API of set ring current type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetCurRingType(
                MMISET_ALL_RING_TYPE_E ring_type
                );

/*****************************************************************************/
//  Description : API of clear current ring  type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_ClearCurRingType(
                MMISET_ALL_RING_TYPE_E ring_type
                );
/*****************************************************************************/
//  Description : stop ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_StopRing(
                MMISET_ALL_RING_TYPE_E     ring_type//in: the type of all ring
                         );

/*****************************************************************************/
//  Description : stop all ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:除了setting和cc调用，其他模块不准调用!!!
/*****************************************************************************/
PUBLIC void MMIAPISET_StopAllRing(
                BOOLEAN     is_include_mp3  //是否包括mp3的背景播放
                            );
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
/*****************************************************************************/
// 	Description : get main menu select style
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMENU_SELECT_EFFECT_STYLE_E MMIAPISET_GetMainmenuSelectStyle(void);
/*****************************************************************************/
// 	Description : set main menu select style
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetMainmenuSelectStyle(
 MMIMENU_SELECT_EFFECT_STYLE_E  mainmenu_select_style
 );
#endif

/*****************************************************************************/
// 	Description : Stop app ring.
//	Global resource dependence : 
//   Author: Yintang.Ren
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_StopAppRing(void);

/*****************************************************************************/
//  Description : Get play time by ring type.
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_StopMsgRing(void);

/*****************************************************************************/
// 	Description : If app ring is in playing state.
//	Global resource dependence : 
//   Author: Yintang.Ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsPlayingAppRing(void);
/*****************************************************************************/
//  Description : get the info of main menu animation status
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMIAPISET_GetMenuAnimationInfo( void );

/*****************************************************************************/
//  Description : get main menu style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMIMAINMENU_TYPE_E MMIAPISET_GetMainmenuStyle(void);

/*****************************************************************************/
//  Description : set main menu style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetMainmenuStyle(
    MMIMAINMENU_TYPE_E  mainmenu_style
    );

/*****************************************************************************/
//  Description : play ring with ring id
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_PlayRingById(
    BOOLEAN                      is_earfree, //是否需要耳机下也支持speaker播放
    uint16                       ring_id,    //固定铃声的index,如果是更多音乐，ring_id设为MMISET_MAX_RING_ID
    uint32                       play_times, //the times of play ring
    MMISET_ALL_RING_TYPE_E       ring_type,  //in: the type of all ring
    MMISET_RING_CALLBACK_PFUNC   callback_func
    );

/*****************************************************************************/
//  Description : play ring with ring id
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_PlayRingByIdEx(
									BOOLEAN                      is_earfree, //是否需要耳机下也支持speaker播放
									uint16                       ring_id,    //固定铃声的index,如果是更多音乐，ring_id设为MMISET_MAX_RING_ID
									uint8   ring_vol,//音量
									uint32                       play_times, //the times of play ring
									MMISET_ALL_RING_TYPE_E       ring_type,  //in: the type of all ring
									MMISET_RING_CALLBACK_PFUNC   callback_func
                                   );

/*****************************************************************************/
//  Description : get information about wallpaper set
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC MMISET_IDLE_DISPLAY_T * MMIAPISET_GetIdleDisplayMode( void);

PUBLIC MMISET_BACKLIGHT_SET_T WATCHAPISET_GetBackLightinfo(void);

/*****************************************************************************/
//  Description : get the volume of call communication
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetCallVolume(void);

/*****************************************************************************/
//  Description : set the volume of call communication
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetCallVolume(
    uint8     call_volume//in
    );

/*****************************************************************************/
//  Description : get if open low voltage warning
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsOpenVoltageWarning(void);

/*****************************************************************************/
//  Description : check the time if is valid
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPISET_CheckTimeIsValid(
//    MMISET_TIME_T       time        //the time
//     );

///*****************************************************************************/
////    Description : check the date if is valid
////    Global resource dependence : none
////  Author: Jassmine.Meng
////    Note:
///*****************************************************************************/
//PUBLIC BOOLEAN MMIAPISET_CheckDateIsValid(
//    MMISET_DATE_T            date    //the date
//     );

/*****************************************************************************/
//  Description : check the date if is valid for calendar
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPISET_CheckDateIsValidForCalendar(
//    MMISET_DATE_T            date    //the date
//    );

/*****************************************************************************/
//  Description : get the type of language 
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetLanguageType(
    MMISET_LANGUAGE_TYPE_E  *language_type_ptr
    );

/*****************************************************************************/
//  Description : get the any key and reject mode from nv
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC MMISET_ANYKEY_REJECT_SET_T  MMIAPISET_GetAnykeyRejectMode(void);

/*****************************************************************************/
//  Description : set the type of language 
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetLanguageType(
    MMISET_LANGUAGE_TYPE_E   language_type//the type of language
    );

/*****************************************************************************/
//  Description : get the time of key board light
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetKeyBoardLightTime(
    MMISET_KEYBOARD_LIGHT_SET_T   *keyboard_info
    );

/*****************************************************************************/
//  Description : get information about wallpaper set
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetWallpaperSetInfo(
    MMISET_WALLPAPER_SET_T    *wallpaper_ptr
    );

/*****************************************************************************/
//  Description : set information about wallpaper set
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetWallpaperSetInfo(
    MMISET_WALLPAPER_SET_T  wallpaper_info//the info abput wallpaper
    );
/*****************************************************************************/
//  Description : reset default wallpaper
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetDefaultWallpaper(void);

/*****************************************************************************/
//  Description : get information about backlight set
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetBackLightInfo(
    MMISET_BACKLIGHT_SET_T     *backlight_ptr
    );


/*****************************************************************************/
//  Description : update contrast of LCD
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_UpdateLCDContrast(
    uint8  lcd_id,     //the id of lcd
    uint8  lcd_contrast//the contrast of lcd
    );

/*****************************************************************************/
//  Description : get current lcd contrast
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetCurrentContrast(void);

/*****************************************************************************/
//  Description : get current contrast value of sub LCD
//  Global resource dependence : none
//  Author: scv
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISET_GetSubLCDContrastValue(
    uint8  lcd_id     //the id of lcd
    );

/*****************************************************************************/
//  Description : 恢复屏幕的对比度
//  Global resource dependence : 
//  Author: scv
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetSubLcdContrast(
    uint8  lcd_id
    );

/*****************************************************************************/
//  Description : get is auto lock key
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC MMISET_KEYLOCK_TYPE_E MMIAPISET_GetAutoBoardKey(void);

/*****************************************************************************/
//  Description : to append one item to listbox by text id
//  Global resource dependence : 
//  Author:louis.wei
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendListItemByTextId(                                   
    MMI_TEXT_ID_T        text_id,
    MMI_TEXT_ID_T        left_softkey_id,
    MMI_CTRL_ID_T        ctrl_id,
    GUIITEM_STYLE_E      item_style
    );

//@zhaohui add 
/*****************************************************************************/
//  Description : to append one item to listbox by text id and set soft keys
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendListItemByTextIdExt(                                    
    MMI_TEXT_ID_T        text_id,
    MMI_TEXT_ID_T        left_softkey_id,
    MMI_TEXT_ID_T        middle_softkey_id,
    MMI_TEXT_ID_T        right_softkey_id,
    MMI_CTRL_ID_T          ctrl_id,
    GUIITEM_STYLE_E      item_style
    );

/*****************************************************************************/
//  Description : to append one item to listbox by text buffer and set soft keys
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendListItemByTextBuffer(                                    
    const wchar *        text_ptr,
    uint16               text_len,
    uint32               user_data,
    MMI_TEXT_ID_T        left_softkey_id,
    MMI_TEXT_ID_T        middle_softkey_id,
    MMI_TEXT_ID_T        right_softkey_id,
    MMI_CTRL_ID_T        ctrl_id,
    GUIITEM_STYLE_E      item_style
    );

/*****************************************************************************/
//  Description : 设置模块中是否有限制恢复MP3的窗口打开
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPTSET_ExitedWinForbidResumingMp3(void);

/*****************************************************************************/
//  Description :关闭网络连接的窗口（for phone module）
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_CloseConnectNetworkWin(void);

/*****************************************************************************/
//  Description :关闭网络列表的窗口（for phone module）
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ClosePlmnListWin(void);

/*****************************************************************************/
//  Description : 播放动画墙纸的第n帧
//  Global resource dependence : 
//  Author:Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void  MMIAPISET_DisplayAnimWallPaper(
//    BOOLEAN   is_first_page,
//    uint16    wallpaper_id,
//    MMI_WIN_ID_T win_id
//     );

/*****************************************************************************/
//  Description : 恢复屏幕的对比度
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetContrast(
    uint8  lcd_id
    );

/*****************************************************************************/
//  Description : play ring by ring data and data length
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_PlayRingByPtr(
                                    MMISET_ALL_RING_TYPE_E      ring_type,
                                    uint8                       ring_vol,   // the ring volume
                                    uint8                       *ring_data_ptr,
                                    uint32                      ring_data_len,
                                    uint32                      play_times,
                                    MMISRVAUD_RING_FMT_E audio_type,
                                    MMISET_RING_CALLBACK_PFUNC  callback_func
                                    );

/*****************************************************************************/
//  Description : Open plmn list win
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenPlmnListWin(MN_DUAL_SYS_E dual_sys, MN_PHONE_PLMN_INFO_LIST_T    *plmn_detail_list);

/*****************************************************************************/
//  Description : get call ring info including file name and ring type
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC MMISET_CALL_RING_T MMIAPISET_GetCallRingInfo(MN_DUAL_SYS_E dual_sys,uint8 mode_id);

/*****************************************************************************/
//  Description : if permit play ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsPermitPlayRing(
    MMISET_ALL_RING_TYPE_E  ring_type  //in: 即将播放铃声的类型
    );

/*****************************************************************************/
//  Description : play call ring with ring volume
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_PlayCallRingByVol(
    uint8                      ring_vol,
    uint8                        ring_id,
    uint32                     play_times, //the times of play ring
    MMISET_ALL_RING_TYPE_E     ring_type,   //in: the type of all ring
    MMISET_RING_CALLBACK_PFUNC callback_func
    );

/*****************************************************************************/
//  Description : if permit play ring(只给sms,cc,alarm用来判断是否能够播放振动调用的)
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsPermitPlayRingForVibrate(
    MMISET_ALL_RING_TYPE_E     ring_type  //in: 即将播放铃声的类型
    );

/*****************************************************************************/
//  Description : set if permit minute hint ring in call
//  Global resource dependence : none
//  Author: bin.ji
//  Note:2004.12.21
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPermitMinhintRingIncall(BOOLEAN is_permit, uint8 minutes);

/*****************************************************************************/
//  Description : set if permit minute hint ring in call
//  Global resource dependence : none
//  Author: bin.ji
//  Note:2004.12.21
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPermitCallRingIncall(BOOLEAN  is_permit);

/*****************************************************************************/
//  Description : set if permit minute hint ring in call
//  Global resource dependence : none
//  Author: bin.ji
//  Note:2004.12.21
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPermitMsgRingIncall(BOOLEAN  is_permit);

/*****************************************************************************/
//  Description : set if permit paly dsp tone,play generic tone
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetDspPlayRing(
    BOOLEAN  is_permit
    );

/*****************************************************************************/
//Description : set the type of call ring
//Global resource dependence : none
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ActiveModeSetCallRingTypeIcon(uint8 mode_id);

/*****************************************************************************/
//  Description : get time in 12 hour fomat
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: time_str_ptr is allocated outer, and must be longer than 8 byte
/*****************************************************************************/
PUBLIC void MMIAPISET_GetTimeStr12(uint8* time_str_ptr);

/*****************************************************************************/
//  Description : get time in 24 hour format
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: time_str_ptr is allocated outer, and must be longer than 8 byte
/*****************************************************************************/
PUBLIC void MMIAPISET_GetTimeStr24(uint8* time_str_ptr);
#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
//  Description : Get IP number
//  Global resource dependence : none
//  Author: lin.lin
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetActiveIPNumber(
    MN_DUAL_SYS_E dual_sys,
    uint8* ipnumber_str_ptr, 
    uint8 len
    );
#endif

/*****************************************************************************/
//  Description : 开机密码是否已经开启
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetPowerOnPwdStatus(void);

/*****************************************************************************/
//  Description : 开机密码出厂设置
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetPowerOnPwdStatus(void);//@zhaohui,cr95392

/*****************************************************************************/
//  Description : parse ss code which is input in idle mode.
//  Global resource dependence : none
//  Author:Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_ParsSsStringProc(
    MN_DUAL_SYS_E dual_sys,
    parsStrInfoS   *parsStrInfo
    );

/*****************************************************************************/
//  Description : open msg box
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenMsgBox(uint32 win_tab, BOOLEAN is_special_func);

/*****************************************************************************/
//  Description : open input power on password win
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenPowerOnPwdWin(void);


/*****************************************************************************/
//  Description : alert ring play result
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_AlertRingPlayResult(MMISRVAUD_REPORT_RESULT_E result, DPARAM param);

/*****************************************************************************/
//   FUNCTION:      MMIAPISET_InitModule
//  Description :   
//  Global resource dependence : 
//   Author:        kelly.li
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPISET_InitModule(void);

/*****************************************************************************/
//  Description : get if set security password win open
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsSetSecurityWinOpen(void);

/*****************************************************************************/
//  Description : how to process sd card plug
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
// PUBLIC void MMIAPISET_ProcessSDPlug(void);

/*****************************************************************************/
//  Description : get is minute reminder open
//  Global resource dependence : none
//  Author: scv
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsAutoRedail(void);
#ifdef FLIP_PHONE_SUPPORT
/*****************************************************************************/
//  Description : get is flipanswer
//  Global resource dependence : none
//  Author: jibin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsFlipAnswer(MN_DUAL_SYS_E dual_sys);
#endif
/*****************************************************************************/
//  Description : whether the key ring is silent
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMIAPISET_IsKeyRingSilent(void);
/*****************************************************************************/
//  Description : whether is net work connecting now
//  Global resource dependence : 
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsNetworkConnecting(void);

/*****************************************************************************/
//  Description : get is connect prompt open FOR CC
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsConnectPrompt(void);

/*****************************************************************************/
//  Description : deal with signal of APP_MN_DISPLAY_PLMN_IND
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISET_HandleDisplayPlmnInd(MN_DUAL_SYS_E dual_sys);

/********************************************************************************
NAME:           MMIAPISET_ParseCleanDataString
DESCRIPTION:    "####7777#": clean user data
PARAM IN:       
PARAM OUT:      
AUTHOR:     kelly.li
DATE:       
********************************************************************************/
PUBLIC BOOLEAN MMIAPISET_ParseCleanDataString(uint8* str_str, uint8 str_len);

/*****************************************************************************/
//  Description :open input reset factory password window
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenInputResetPwdWin(BOOLEAN      is_clean_all_data);
#ifdef SCREENSAVER_SUPPORT
/*****************************************************************************/
//  Description : get screen saver info
//  Global resource dependence : 
//  Author:Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_GetScreensaverInfo(
    MMISET_SCREENSAVER_INFO_T  *ssaver_info_ptr    //in/out:
    );

/*****************************************************************************/
//  Description : Set ScreenSaver information
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetScreenSaverInfo(
    MMISET_SCREENSAVER_INFO_T *ssaver_info_ptr
    );

/*****************************************************************************/
//  Description : 保存当前选中的屏保
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SaveCurScreenSaver(MMISET_SCREENSAVER_TYPE_E ssaver_type);
/*****************************************************************************/
//  Description : 打开屏保预览窗口
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenScreenSaverWin(uint16 select_item_id);
#endif
/*****************************************************************************/
//  Description : Set Power On Off Animation information
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPowerOnOffAnimInfo(
    MMISET_POWER_ON_OFF_ANIM_INFO_T *power_on_off_anim_info_ptr,    //in/out
    BOOLEAN is_power_on_off                 //TRUE:power on, FALSE:power off
    );

/*****************************************************************************/
//  Description : Reset Power On Off Animation information
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_RetPowerOnOffAnimInfo(
                        BOOLEAN is_power_on_off     //TRUE; power on, FALSE: power off
                        );

/*****************************************************************************/
//  Description : Get Power On Off animation set info
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_GetPowerOnOffAnimInfo(
    MMISET_POWER_ON_OFF_ANIM_INFO_T *power_on_off_anim_info_ptr,    //in/out
    BOOLEAN is_power_on_off                 //TRUE:power on, FALSE:power off
    );

/*****************************************************************************/
//  Description : set audio mode is earfree
//  Global resource dependence : none
//  Author: jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetAudioMode(
    BOOLEAN  is_earfree  //是否需要耳机下也支持speaker播放
    );  



/*****************************************************************************/
//Description : 关闭震动计时器,该计时器表示每次震动
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
//    PUBLIC void MMIENVSET_StopOnetimeVibTimer(void);

/*****************************************************************************/
//Description : 判断是否震动计时器时间到 
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
//   PUBLIC BOOLEAN MMIENVSET_IsOnetimeVibTimerOut(DPARAM param);

/*****************************************************************************/
//Description : 判断震动计时器是否开启 
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
//   PUBLIC BOOLEAN MMIENVSET_IsOnetimeVibTimerActive(void);

/*****************************************************************************/
//Description : 判断震动计时器是否开启 
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMIENVSET_GetVibStatus(void);

/*****************************************************************************/
//Description : 判断震动计时器是否开启 
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
// PUBLIC void MMIENVSET_SetVibStatus(BOOLEAN is_vibra_start);
/*****************************************************************************/
// Description : 设置线路
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-6
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPISET_SetLineInfo(void);

/*****************************************************************************/
// Description : 设置飞行模式
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-8
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPISET_OpenSetFlyMode(void);

/*****************************************************************************/
// Description : open set idle display win
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-21
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenSetIdleDisWin(void);

/*****************************************************************************/
// Description : open select network win
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-21
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenSelNetWorkWin(MN_DUAL_SYS_E dual_sys);
// @baokun fix cr94854 begin
/*****************************************************************************/
// Description : get fly mode
// Global resource dependence :
// return : FALSE: fly mode off; TRUE :fly mode on; 
// Author: baokun.yin
// Note:2007-8-21
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetFlyMode(void);
// @baokun fix cr94854 end

/*****************************************************************************/
//  Description : get first day of week
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetFirstDayOfWeek( void );

 /*****************************************************************************/
 // 	Description : get week day by offset
 //	Global resource dependence : 
 //  Author:James.Zhang
 //	Note: 根据第几格获得周几
 /*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetWeekDayByOffset( uint8 offset );

/*****************************************************************************/
 // 	Description : get offset by week day
 //	Global resource dependence : 
 //  Author:James.Zhang
 //	Note: 根据周几获得第几格
 /*****************************************************************************/
 PUBLIC uint8 MMIAPISET_GetOffsetByWeekDay( uint8 week_day );
 
/*****************************************************************************/
//  Description : 获得mmi 双sim卡系统设置情况
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC MMISET_DUALSYS_TYPE_E MMIAPISET_GetMultiSysSetting(void);

/*****************************************************************************/
//  Description : 获得mmi multi sim 系统设置情况
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPISET_GetMultiSysSettingNum(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
//  Description : 获得当前主sim卡的id 
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISET_GetActiveSim(void);

/*****************************************************************************/
//  Description : 获得当前主sim卡的选项，包括ps是否启动
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC MMISET_MAJOR_SIM_SEL_TYPE_E MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_TYPE_E e_func_type);

/*****************************************************************************/
//  Description : 获得当前主sim卡的选项，不包括ps
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC MMISET_MAJOR_SIM_SEL_TYPE_E MMIAPISET_GetMajorSimCard(MMISET_MAJOR_SIM_SEL_FUNC_TYPE_E e_func_type);

/*****************************************************************************/
//  Description : Get Send SS repeat timer id
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetSendSSRepeatTimerId(void);

/*****************************************************************************/
//  Description : Stop Send SS repeat timer
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_StopSendSSRepeatTimer(void);

/*****************************************************************************/
//  Description : Interrogate Status on background 
//  Global resource dependence : 
//  Author:shengjie.liu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_BgInterrogateStatus(void);

/*****************************************************************************/
//  Description : set param for Interrogate Status
//  Global resource dependence : 
//  Author:shengjie.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_BgSetIgStatusParam(MN_DUAL_SYS_E  dual_sys);

/*****************************************************************************/
// 	Description : get plmn service status string to display select sim
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetSimServiceString(MN_DUAL_SYS_E dual_sys, MMI_STRING_T *network_str_ptr);

/*****************************************************************************/
//  Description : get sim name
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC MMISET_SIM_NAME_T MMIAPISET_GetSimName(MN_DUAL_SYS_E dual_sys);

#ifdef MMI_MULTISIM_COLOR_SUPPORT
/*****************************************************************************/
//  Description : get sim color
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMIAPISET_GetSimColor(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : get sim color
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISET_GetSimColorID(MN_DUAL_SYS_E dual_sys);
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE

/*****************************************************************************/
//  Description : get sim icon 
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISET_GetSimIcon(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
// 	Description : to open multi sys call settings standby menu window
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenMultiSimStandbySettingsWindow(void);

/*****************************************************************************/
//  Description : to open multi sys call settings main menu window
//  Global resource dependence : none
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenMultiSimSettingsWindow(void);

/*****************************************************************************/
//  Description : 设置呼叫转移的dual sys 类型
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void   MMIAPISET_SetSysForCf(MN_DUAL_SYS_E    dual_sys);

/*****************************************************************************/
//  Description : get another sim card
//  Global resource dependence : 
//  Author:shengjie.liu
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISET_GetAnotherSim(MN_DUAL_SYS_E  dual_sys);

/*****************************************************************************/
//  Description : 获得呼叫转移的dual sys 类型
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E   MMIAPISET_GetSysForCf(void);

//#endif

/*****************************************************************************/
// 	Description : to append sys ok item to listbox by sim name
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendMultiSimName(
    MMI_CTRL_ID_T ctrl_id,
    MMI_TEXT_ID_T left_softkey_id,
    uint32 max_num,
    MMISET_APPEND_SIM_TYPE_E append_type    
    );

/*****************************************************************************/
// 	Description : to append sys ok item to listbox by sim name
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendMultiSimNameExt(
    MMI_CTRL_ID_T ctrl_id,
    MMI_TEXT_ID_T left_softkey_id,
    MMI_TEXT_ID_T middle_softkey_id,
    MMI_TEXT_ID_T right_softkey_id,
    uint32 max_num,
    MMISET_APPEND_SIM_TYPE_E append_type    
    );

/*****************************************************************************/
//  Description :获得本卡回复的nv设置
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetAnswerWithOriginalSimType(
                                                      MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_TYPE_E e_func_type
                                                      );
                                                 
/*****************************************************************************/
//  Description :关闭网络搜索的窗口
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_CloseSearchNetworkWin(void);

/*****************************************************************************/
//  Description :判断窗口MMISET_SET_WAIT_SEARCHNETWORK_WIN_ID是否打开
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SearchNetworkWinisOpen(void);
//@zhaohui,cr96828


/*****************************************************************************/
//  Description : MMIAPISET_SetTime
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetTime(void);
/*****************************************************************************/
//  Description : MMIAPISET_SetDate
//  Global resource dependence : 
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetDate(void);
/*****************************************************************************/ 
//  Description : set if is auto redial
//  Global resource dependence : none
//  Author: scv
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMIAPISET_SetIsAutoRedial( 
    BOOLEAN  is_auto_redial
    );

/*****************************************************************************/
//  Description : set the mode of TV out
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetTVOutMmode(
                MMI_CTRL_ID_T       ctrl_id
                            );
/*****************************************************************************/
//  Description : add TVout mode list item
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendTVOutModeListItem(
    MMI_CTRL_ID_T       ctrl_id
    );

/*****************************************************************************/
//  Description : 获得mmi 双sim卡系统设置情况
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetDualSysSetting(MMISET_DUALSYS_TYPE_E   e_dual_sys);

/*****************************************************************************/
//  Description :  获得mmi 用户设置双sim卡系统设置情况，适用于开机判断情况。
//  Global resource dependence : 
//  Author: bownzhang
//  Note:
/*****************************************************************************/
//PUBLIC MMISET_DUALSYS_TYPE_E MMIAPISET_GetUserDualSysSetting(void);

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
    );

/*****************************************************************************/
//  Description : get message ring id
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
// PUBLIC uint8 MMIAPISET_GetFixedMsgRingId(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : set the type of call ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetCallRingType(
    MMISET_CALL_RING_TYPE_E  call_ring_type,//in
    MN_DUAL_SYS_E            dual_sys
    );

/*****************************************************************************/
//  Description :肯据所给总级别连续画出对应个数的背景block
//  Global resource dependence : 
//  Author: murphy xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_DisplayVolumeBlockBgPicture(
										   uint8	   blocknum,
                                           MMI_WIN_ID_T win_id,MMI_IMAGE_ID_T img_id);
#ifdef TOUCH_PANEL_SUPPORT 
/*****************************************************************************/
//  Description :根据触笔的坐标调整音量
//  Global resource dependence : 
//  Author: baokun
//  Note:
/*****************************************************************************/
PUBLIC uint8  MMIAPISET_TPAdjustVolume(GUI_POINT_T tp_point, uint8 cur_vol, uint8 max_vol, MMI_WIN_ID_T win_id);
#endif
/*****************************************************************************/
//  Description : 根据音量的大小显示音量图片
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_DisplayVolumePicture(
    uint8   volume,
    MMI_WIN_ID_T win_id
    );
/*****************************************************************************/
//Description : 预览铃声 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_PreviewRing(
    MN_DUAL_SYS_E dual_sys,
    uint16                       ring_id,    // the ring id
    uint8                       ring_vol,   // the ring volume 
    uint32                      play_times, // play times
    MMISET_ALL_RING_TYPE_E      ring_type,  // in: the type of all ring
    MMISET_RING_CALLBACK_PFUNC  callback_func
    );


/*****************************************************************************/
//Description : 开启震动 
//Global resource dependence : 
//Author: maqianlin
//Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_StartVibrator(MMISET_ALL_RING_TYPE_E  ring_type);

/*****************************************************************************/
//Description : 开启震动计时器 
//Global resource dependence : 
//Author: maqianlin
//Note:在开启计时器
/*****************************************************************************/
PUBLIC void MMIAPISET_StartVibratTimer(void);

/*****************************************************************************/
//Description : 关闭震动 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_StopVibrator(
                MMISET_ALL_RING_TYPE_E  ring_type
                            );

/*****************************************************************************/
//Description : 关闭震动计时器 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_StopVibratTimer(void);

/*****************************************************************************/
//Description : 判断是否震动计时器时间到 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsVibratTimerOut(DPARAM param);


/*****************************************************************************/
//Description : 振动
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetAutoStopVibrator(
                                      BOOLEAN                 is_open,
                                      MMISET_ALL_RING_TYPE_E  ring_type,//in: the type of all vibrator
                                      uint32 duration   //振动间隔
                                       );

/*****************************************************************************/
//  Description : to STOP the vibrator
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_RingCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param);

/*****************************************************************************/
//Description : 判断震动计时器是否开启 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsVibratTimerActive(void);

/*****************************************************************************/
//  Description : to open ussd select  window
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
// PUBLIC void MMIAPISET_OpenUssdSelectWin(MMISET_SIM_SELECT_TYPE_E select_type);
/*****************************************************************************/
//  Description : 是否是设置墙纸
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
// PUBLIC void MMIAPISET_SetIsWallPaper(BOOLEAN is_wallpaper);

/*****************************************************************************/
//  Description : 是否是设置墙纸
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMIAPISET_GetIsWallPaper(void);
#ifdef MULTI_THEME_SUPPORT
/*****************************************************************************/
//  Description : ini current theme
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_IniCurrentTheme(void);
#endif
/*****************************************************************************/
//  Description : get current theme
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISET_GetCurrentStaticThemeID(void);

/*****************************************************************************/
//  Description : get current theme type
//  Global resource dependence : 
//  Author: 
//  Note: 这个函数不能被resource直接调用判断，
//  因为在某些时候（udisk使用中），虽说记录的是文件theme,但实际上只能使用default资源。
/*****************************************************************************/
PUBLIC MMISET_THEME_TYPE_E MMIAPISET_GetCurrentThemeType(void);

 /*****************************************************************************/
// Description : 开机设置飞行模式
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-8
/*****************************************************************************/
PUBLIC void MMIAPISET_SetFlyModeOpenPhone(void);
//@zhaohui,cr108097
/*****************************************************************************/
//  Description :显示搜网结果 
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SearchNetworkResultDisp(BOOLEAN result);

/*****************************************************************************/
//  Description : set fly mode
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetFlyMode(BOOLEAN is_fly_mode_on);

/*****************************************************************************/
//  Description : set fly mode whole flow
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetFlyModeWholeFlow(BOOLEAN is_fly_mode_on);

/*****************************************************************************/
// Description : get fly mode Set Power On
// Global resource dependence :
// return :  
// Author: baokun.yin
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetFlyModeSetPowerOn(void);

/*****************************************************************************/
//  Description : 提示飞行模式
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_AlertFlyMode(void);

/*****************************************************************************/
//  Description : to get text id of left shortcut main win
//  Global resource dependence : none
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetFSMainTextId(MMI_TEXT_ID_T * left_key_id, MMI_TEXT_ID_T * right_key_id);
#ifdef MMI_COMMON_IDLE_IMG_SOFTKEY_SUPPORT
PUBLIC void MMIAPISET_GetFSMainIMGId(MMI_IMAGE_ID_T * left_key_id, MMI_IMAGE_ID_T * right_key_id);
#endif
/*****************************************************************************/
//  Description :get cfu info
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetCfuInfo(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : open wizard cfnrc win
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_OpenWizardCFNRCWin(void);

/*****************************************************************************/
//  Description :Init audio play rate before play, to MMIAUDIO_PLAY_RATE_NORMAL.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_InitPlayRate(
                void
                );

/*****************************************************************************/
//  Description :Init audio device before play.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_InitAudioDevice(
                uint8  volume
                );

/*****************************************************************************/
//  Description :Init audio device before play.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_InitBtAudioDevice(
                BOOLEAN is_bt_play,
                uint8   volume
                );
                
/*****************************************************************************/
//  Description :Init audio device before play.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_InitAudioDeviceExt(
                    MMISET_ALL_RING_TYPE_E  ring_type,
                    uint8  volume
                    );

/*****************************************************************************/
//  Description : Is the current ring playing through receiver 
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsThroughReceiver(
                    void
                    );

/*****************************************************************************/
//  Description : Whether the the ring type is the playing ring.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsPlayingRingType(
                    MMISET_ALL_RING_TYPE_E ring_type
                    );

/*****************************************************************************/
//  Description :Init audio EQ mode before play, to regular mode.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_InitEqMode(
                void
                );

/*****************************************************************************/
//  Description : reset display setting 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetDisplaySetting(void);

/*****************************************************************************/
//  Description : 更新闹钟/日程/开关机的设置
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_UpdateAlarmSet(void);

/*****************************************************************************/
//  Description : MMIAPISET_OpenImInputLangSettingWin
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
#if defined MMI_IM_PDA_SUPPORT
PUBLIC void MMIAPISET_OpenImInputLangSettingWin(void);
#endif

/*****************************************************************************/
//  Description : get idle display style
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMISET_IDLE_STYLE_E MMIAPISET_GetIdleStyle(void);

/*****************************************************************************/
//  Description : get idle display substyle
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMISET_IDLE_SUBSTYLE_E MMIAPISET_GetIdleSubStyle(void);

/*****************************************************************************/
//  Description : get idle shortcut
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMISET_IDLE_SHORTCUT_T MMIAPISET_GetIdleShortcut(void);

/*****************************************************************************/
//  Description : open shortcut win
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenShortcutWin(
                                      uint16        shortcut_index, //from 0
                                      MMI_WIN_ID_T  win_id
                                      );

/*****************************************************************************/
//  Description : to open selected shortcut win
//  Global resource dependence : 
//  Author:cheng.luo
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenSelectedShortcutWin(
                                              MMI_WIN_ID_T             win_id,
                                              MMISET_SHORTCUT_TYPE_E   selected    
                                              );
#ifndef MMI_PHONESET_MINI_SUPPORT
/*****************************************************************************/
 //     Description : 获得编码方式
 // Global resource dependence : 
 //  Author: 
 // Note:
 /*****************************************************************************/
 PUBLIC MMISET_TXT_CODE_TYPE_TYPE_E MMIAPISET_GetCodeType( void );

#endif
/*****************************************************************************/
//  Description : get  if input language_type is valid
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsSelectedLanguageValid( MMISET_LANGUAGE_TYPE_E language_type );

/*****************************************************************************/
//  Description : get default wallpaper id by style
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISET_GetDefaultWallpaperIDByStyle(void);

/*****************************************************************************/
//  Description : set is red key lock
//  Global resource dependence : none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMIAPISET_SetRedKeyLock(BOOLEAN is_red_keylock);
/*****************************************************************************/
//  Description : get is red keylock
//  Global resource dependence : none
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetRedKeyLock(void);

/*****************************************************************************/
//  Description : 是否需要开机时设置时间日期
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsNeedResetDateTime(void);
/*****************************************************************************/
//  Description : 设置是否需要开机时设置时间日期
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPISET_SetNeedResetDateTime(BOOLEAN  is_need);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: jian.ma
//	Note: TRUE: language ready; FALSE: language not ready
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_CheckLanguageStatus(MMISET_LANGUAGE_TYPE_E language_type);

/*****************************************************************************/
// 	Description : MMIAPISET_HandlePreferRatRsp
//	Global resource dependence : 
//  Author: zhaohui
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISET_HandlePreferRatRspResult(
                                                 BOOLEAN rsp_result
                                                );

/*****************************************************************************/
// 	Description : to open Divert settings main menu window
//	Global resource dependence : none
//  Author: xiangjie
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenDivertSettingsWindow(void);


/*****************************************************************************/
//  Description : need wait this function finished 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetWaitFlag(void);

/*****************************************************************************/
//  Description : inform this function finished 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_FuncFinishedInd(void);



/*****************************************************************************/
//  Description : format date string by date style
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_FormatDateStrByDateStyle(
                                            uint16 year, //in
                                            uint8 month, //in
                                            uint8 date, //in
                                            char symbol, //in
                                            uint8* date_str,//in/out
                                            uint16 date_str_max_len//in
                                            );

/*****************************************************************************/
//  Description : get date string by date style
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_FormatDateStrByDateEx(
                                            uint16 year, //in
                                            uint8 month, //in
                                            uint8 date, //in
                                            char symbol, //in
                                            uint8* date_str,//in/out
                                            uint16 date_str_max_len,//in
                                            BOOLEAN is_display_year
                                            );

/*****************************************************************************/
// 	Description : format time string by time
//	Global resource dependence : 
//  Author:    jian.ma
//	Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_FormatTimeStrByTime(
					  uint8 hour,//in
					  uint8 min,//in
					  uint8* timestr,//in/out
					  uint16 time_str_max_len//in
					  );

/*****************************************************************************/
// 	Description : get current data display string
//	Global resource dependence : none
//  Author: SCV
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetCurrentDateStr(
                                        uint8* datestr,//in/out
                                        char symbol, //in
                                        uint16 datestr_len //in
                                        );


#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : get is minute reminder open
//	Global resource dependence : none
//   Author: kun.yu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsTouchVibrate(void);
#endif

/*****************************************************************************/
//  Description : update wallpaper
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_UpdateWallpaper(void);

#ifdef VT_SUPPORT

/*****************************************************************************/
// 	Description : append fallback item
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/
PUBLIC MMISET_VT_FALLBACK_HANDLE_MODE_E MMIAPISET_GetFallbackHandleMode(void);  

/*****************************************************************************/
// 	Description : append fallback item
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetReplaceInfoFromNv(MMISET_VT_REPLACE_FILE_INFO_T* out_replace_file_info_ptr);

/*****************************************************************************/
// 	Description : MMIAPISET_OpenVTSeting
//	Global resource dependence : 
//  Author: samboo
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenVTSeting(void);

/*****************************************************************************/
// 	Description : MMIAPISET_GetVTRecordRVDAPath
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:获得VT录制远端视频和混音的位置 从nv
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPISET_GetVTRecordRVDAPath(void);

/*****************************************************************************/
// 	Description : MMIAPISET_GetVTRecordDAPath
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:获得VT录制混音的位置 从nv
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPISET_GetVTRecordDAPath(void);

/*****************************************************************************/
// 	Description : MMIAPISET_GetVTRecordRVPath
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:获得VT录制远端视频的位置 从nv
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPISET_GetVTRecordRVPath(void);
#endif
#ifdef MMI_DUALMODE_ENABLE
/*****************************************************************************/
//  Description : MMIAPISET_SetNetworkType
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetNetworkType(MN_GMMREG_RAT_E rat);

/*****************************************************************************/
// Description : get switch rat state
// Global resource dependence : 
// Author:kun.yu
// Note: 2010-02-04
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetSwitchRatState(void);

/*****************************************************************************/
//  Description : 判断是否满足提示"切换双模后再试"搜网的情况
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
BOOLEAN MMIAPISET_IsPromptRetry(MN_DUAL_SYS_E  dual_sys);

/*****************************************************************************/
//  Description : open NetWorkType win
//  Global resource dependence : 
//  Author: panvs.zhong
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenNetWorkTypeWin(void);

PUBLIC void MMIAPISET_SetTestModeEx(MN_DUAL_SYS_E dual_sys, uint32 network_type);
#endif

#ifdef DPHONE_SUPPORT 
/************************************************************************/
// 	Description : MMIAPISET_GetSpeedDialDelay
//	Global resource dependence : 
//  Author:Will.Shao
/************************************************************************/
PUBLIC uint32 MMIAPISET_GetSpeedDialDelay(void);
#endif

#ifdef LCD_SLIDE_SUPPORT
/*****************************************************************************/
//  Description : get the type of slide open type
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC MMISET_SLIDE_OPEN_TYPE_E MMIAPISET_GetSlideOpenType(void);

/*****************************************************************************/
//  Description : get the type of slide close type
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC MMISET_SLIDE_CLOSE_TYPE_E MMIAPISET_GetSlideCloseType(void);

/*****************************************************************************/
//  Description : get the type of keylock switch type
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC MMISET_KEYLOCK_SWITCH_TYPE_E MMIAPISET_GetKeylockSwitchType(void);

#endif

#ifdef MV_SUPPORT
/*****************************************************************************/
// 	Description : open magic voice main window
//	Global resource dependence : 
//  Author: fen.xie
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenMagicVoiceWin(void);
#endif

/*****************************************************************************/
//  Description : to append a group of items to listbox by prefix text id and num.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendFixedItems(
                                                                        MMI_TEXT_ID_T        prefix_txt_id,
                                                                        uint32                      item_num,      
                                                                        MMI_TEXT_ID_T        left_softkey_id,
                                                                        MMI_TEXT_ID_T        middle_softkey_id,
                                                                        MMI_TEXT_ID_T        right_softkey_id,
                                                                        MMI_CTRL_ID_T         ctrl_id,
                                                                        GUIITEM_STYLE_E      item_style
                                                                        );

/*****************************************************************************/
//  Description : 获取铃声的名字
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISET_GetRingName(uint16 idx, wchar *ring_name, uint32 len);

/*****************************************************************************/
// 	Description : get password edit rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC GUI_BOTH_RECT_T MMIAPISET_GetPwEditRect(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 设置输入密码窗口的背景色，image 和title
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPwBg(
                              MMI_WIN_ID_T      win_id, //窗口的ID
                              MMI_IMAGE_ID_T    image_id
                              );

#ifdef MULTI_SIM_GSM_CTA_SUPPORT
/*****************************************************************************/
// 	Description : Open CTA MULTI SIM Standby Win
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenCTAMultiSysStandbyWin(BOOLEAN is_power_select);

/*****************************************************************************/
// 	Description : Implement CTA Standby Mode
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_ImplementCTAStandbyModeAsync(MN_DUAL_SYS_E dual_sys);
#endif
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/07
/*****************************************************************************/
//  Description : 设置密码输入框的TITLE 区域
//  Global resource dependence : none
//  Author: Taiping Lai
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPwTitleRect(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description : 获取Dphone 密码输入区域
//  Global resource dependence : none
//  Author: Taiping Lai
//  Note: 
/*****************************************************************************/
PUBLIC GUI_BOTH_RECT_T MMIAPISET_GetPwEditRectForDphone(void);
#endif

/*****************************************************************************/
//  Description : 打开时间/日期设置窗口
//                     判断"自动更新时间"功能是否开启
//  Global resource dependence : 
//  Author:xiaoming.ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenTimeOrDateWindow(uint32* win_table_ptr, BOOLEAN is_enter_from_setting);

/*****************************************************************************/
// Description : open set network win
// Global resource dependence : 
// Author: mary.xiao
// Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenSetNetWorkWin(void);

#if defined(MMI_UPDATE_TIME_SUPPORT)
/*****************************************************************************/
// 	Description : set if  is auto update time
//	Global resource dependence : none
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetIsAutoUpdateTime(BOOLEAN  is_auto_update_time);

/*****************************************************************************/
// 	Description : set if  is auto update time
//	Global resource dependence : none
//  Author: dave.ruan
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetUpdateTimeDefault(void);

/*****************************************************************************/
// 	Description : get if  is auto update time
//	Global resource dependence : none
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsAutoUpdateTime(void);

/*****************************************************************************/
//  Description : API: restore data and time factory setting
//  Global resource dependence : none
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_RestoreDateTimeFactorySetting(void);

/*****************************************************************************/
// 	Description : 更新时间结束后提示用户
//	Global resource dependence : 
//    Author: dave.ruan
//	Note: 手动更新成功会提示，自动更新成功不提示
/*****************************************************************************/
PUBLIC void MMIAPISET_ShowUpdateTimeMsgWin(BOOLEAN is_success, BOOLEAN is_manual_update);

/*****************************************************************************/
// 	Description : 获取关闭自动更新窗口的id
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPISET_GetCloseAutoUpdateQueryWinID(void);

/*****************************************************************************/
// 	Description : refresh time date edit ctrl id after time date update 
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_RefreshTimeDateEditCtrl(void);

#endif /*MMI_UPDATE_TIME_SUPPORT*/

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : find num in blacklist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsCCBlacklist(
                                       MMIPB_BCD_NUMBER_T *orginal_addr_ptr,
                                       MN_CALL_TYPE_E call_type);

/*****************************************************************************/
//  Description : find num in blacklist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsSMSBlacklist(
                                        MMIPB_BCD_NUMBER_T *orginal_addr_ptr
                                       );

/*****************************************************************************/
//  Description : set new blacklist num
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_AddNewBlacklistNum(
                                         MMIPB_BCD_NUMBER_T *orginal_addr_ptr
                                        );

/*****************************************************************************/
//  Description : set all blacklist setting open/close
//  Global resource dependence : none
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMIAPISET_SetAllBlacklistSetting(BOOLEAN is_open);

/*****************************************************************************/
//  Description : add blacklist from external,such as PB,CL  
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
PUBLIC MMISET_BLACKLIST_ERROR_TYPE_E MMIAPISET_AddBlacklistFromExt(
                                                                            MMI_STRING_T * blacklist_name_ptr,
                                                                            MMIPB_BCD_NUMBER_T *pb_num_ptr      
                                                                            );

/*****************************************************************************/
//  Description : add blacklist from PB,CL,sms,mms
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_AddBlacklist(
                                MMI_STRING_T mmi_pb_name,
                                MMIPB_BCD_NUMBER_T pb_bcd      
                                );

/*****************************************************************************/
//  Description : delete blacklist,such as from CL,sms,mms
//  Global resource dependence : 
//  Author:sam.hua
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_DeleteBlacklist(MMIPB_BCD_NUMBER_T pb_bcd);
                                
#endif

/*****************************************************************************/
//  Description : get msg ring info including ring class and index and ring colum
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC MMISET_MSG_RING_T MMIAPISET_GetMsgRingInfo(MN_DUAL_SYS_E dual_sys,uint8 mode_id);


/*****************************************************************************/
//  Description : set information about backlight set
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetBackLightInfo(
                                    MMISET_BACKLIGHT_SET_T     backlight_info//the info about backlight
                                    );

/*****************************************************************************/
// 	Description : 是否包含回車
//	Global resource dependence : 
//  Author:	bruce.chi
//	Return: TRUE: contain invalid char. FALSE: do not contain invalid char
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsIncludeReturnChar(
                                             const wchar* name_ptr, 
                                             uint16 wstr_len
                                             );

/*****************************************************************************/
// 	Description : get the type of date display type
//	Global resource dependence : none
//  Author: xiangjie
//	Note:
/*****************************************************************************/
PUBLIC MMISET_DATE_DISPLAY_TYPE_E MMIAPISET_GetDateDisplayType(void);

#ifdef TTS_SUPPORT
/*****************************************************************************/
// 	Description : get the info of TTS setting status
//	Global resource dependence : 
//  Author: barry.peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetTTSInfo( void );
/*****************************************************************************/
// 	Description : set the info of TTS setting status
//	Global resource dependence : 
//  Author: barry.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetTTSInfo(
                              MMI_CTRL_ID_T	    ctrl_id
                              );
#endif

/*****************************************************************************/
// 	Description : to open function key win
//	Global resource dependence : 
//  Author:xiangjie
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenFuncKeyWin(
                                  MMI_WIN_ID_T     win_id,
                                  MMISET_FUNCKEY_E index
                                  );

/*****************************************************************************/
// 	Description : to open direction key win
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenDirectionKeyWin(
                                       MMI_WIN_ID_T    win_id,
                                       MMISET_DIRKEY_E index    //方向键
                                       );

#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
/*****************************************************************************/
// 	Description : load font
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetFont( 
                                 const uint16* new_name,  //字体路径
                                 BOOLEAN       is_unload, //是否卸载
                                 BOOLEAN       is_prompt  //是否需要提示
                                 );
#endif


/*****************************************************************************/
// 	Description : to open call settings main menu window
//	Global resource dependence : none
//  Author: xiangjie
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenCallSettingsWindow(void);


/*****************************************************************************/
// 	Description : open display setting window
//	Global resource dependence : 
//  Author: xiangjie
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterDisplaySettingWin(void);

/*****************************************************************************/
// 	Description : open phone setting window
//	Global resource dependence : 
//  Author: xiangjie
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterPhoneSettingWin(void);

/*****************************************************************************/
// 	Description : open security setting window
//	Global resource dependence : 
//  Author: xiangjie
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterSecuritySettingWin(void);


/*****************************************************************************/
//  Description : 得到3D其他特效的开启/关闭情况
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetList3DStyle(void);
                                     
/*****************************************************************************/
// 	Description : 得到g-sensor切换特效
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPISET_GetGsensorSwitchStyle(void);

/*****************************************************************************/
// 	Description : to open ussd select  window
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenUssdSelectWin(MMISET_SIM_SELECT_TYPE_E select_type);

/*****************************************************************************/
//  Description : Get wallpaper image id by index.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISET_GetWallpaperByIndex(uint16 index);


/*****************************************************************************/
//  Description : MMIAPISET_IsWallpaperCall
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsWallpaperCall(void);

 /*****************************************************************************/
 //     Description : is display idle words
 // Global resource dependence : 
 //  Author:apple.zhang
 // Note: 
 /*****************************************************************************/
 PUBLIC BOOLEAN MMIAPISET_IsDisplayIdleWords(void);

 
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
PUBLIC void MMIAPISET_CreateCustomWordEditWin(void);
#endif
#endif
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
 /*****************************************************************************/
 //     Description : set idle win display mode
 // Global resource dependence : 
 //  Author: kelly.li
 // Note:
 /*****************************************************************************/
PUBLIC void MMIAPISET_SetDisplaytIdleMode(
                                        void
                                        );

#endif
#endif
/*****************************************************************************/
//  Description : set current lcd contrast
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetCurrentContrast(uint8 lcd_contrast);

// this API not related UI only change data
PUBLIC void MMIAPISET_SetCurrentContrastEx( uint8 lcd_contrast );

/*****************************************************************************/
//  Description : set contrast of main LCD
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetMainLCDContrast(
                                      void
                                      );

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : to enter web win
//  Global resource dependence : 
//  Author:hongying wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterMonternetWin( void);

/*****************************************************************************/
//  Description : to enter web win
//  Global resource dependence : 
//  Author:hongying wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterPocketHallWin( void);

/*****************************************************************************/
//  Description : to enter Fetion win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISET_EnterMusicWin( void);

/*****************************************************************************/
//  Description : to enter Fetion win
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISET_EnterFetionWin( void);

/*****************************************************************************/
//  Description : to enter stock win
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterStockWin( void);

/*****************************************************************************/
//  Description : to enter Map win
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_EnterMapWin( void);
#endif

/*****************************************************************************/
//Description : Get SS SIM(Not In SS return MMI_DUAL_SYS_MAX)
//  Global resource dependence : none
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISET_GetSSSIM(void);

#ifdef MMI_SIM_LOCK_SUPPORT
/*****************************************************************************/
//  Description :open reset sim lock switch menu
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenSIMLockSwitchWin(void);

/********************************************************************************
NAME:           MMIAPISET_ParseSIMLockSwitchString
DESCRIPTION:    "####8888#": open sim lock switch menu
PARAM IN:       
PARAM OUT:      
AUTHOR:     dave.ruan
DATE:       
********************************************************************************/
PUBLIC BOOLEAN MMIAPISET_ParseSIMLockSwitchString(uint8* str_str, uint8 str_len);
#endif

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT

#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT

/*****************************************************************************/
// 	Description : make a judgement that if key lock password is on.
//	Global resource dependence : 
//  Author:Yintang.Ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsKLPswOn(void);

/*****************************************************************************/
// 	Description : Set key lock password on or off.
//	Global resource dependence : 
//  Author:Yintang.Ren
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetKLPswOnOff(BOOLEAN is_on);

/*****************************************************************************/
// 	Description : Get key lock password from NV.
//	Global resource dependence : 
//  Author:Yintang.Ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetKLPsw(uint8 *psw);

/*****************************************************************************/
// 	Description : Save key lock password to NV.
//	Global resource dependence : 
//  Author:Yintang.Ren
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetKLPsw(uint8 *psw);

/*****************************************************************************/
// 	Description : Open key lock password setting windows.
//	Global resource dependence : 
//  Author:Yintang.Ren
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenKLPswSettingWin(void);
// 	Description : 锁屏密码恢复出厂设置
//	Global resource dependence : 
//  Author: zhaohui
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetKLPsw(void);
#endif

#endif

/*****************************************************************************/
//  Description : get multim volume
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetMultimVolume(void);

/*****************************************************************************/
//  Description : set multim volume
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetMultimVolume(uint8 vol);

/*****************************************************************************/
//  Description : 静音与非静音切换,返回切换后的声音
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_MultimMuteSwitch(void);

/*****************************************************************************/
//  Description : get last multim volume(当前音量是0 时才有效)
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetLastMultimVolume(void);
/*****************************************************************************/
//  Description : main menu type set win
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreateMainmenuStyleWin(void);

#ifdef PDA_UI_SUPPORT_U_IDLE
/*****************************************************************************/
//  Description : uidle effect set win
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreateUIdleStyleWin(void);
#endif

/*****************************************************************************/
//  Description :open power saving setting
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISET_OpenPowerSavingSetting(void);

/*****************************************************************************/
//  Description :MMIAPISET_SetPowerSavingSwitch
//  Global resource dependence : 
//  Author:juan.wu
//  Note:设置省电模式
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPowerSavingSwitch(BOOLEAN is_switch);


/*****************************************************************************/
//  Description :MMIAPISET_GetPowerSavingSwitch
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 当前省电模式是否开启
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetPowerSavingSwitch(void);

/*****************************************************************************/
//  Description :MMIAPISET_GetPowerSavingSwitch
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 当前省电模式设置
/*****************************************************************************/
PUBLIC MMISET_POWER_SETTING_ITEM_T MMIAPISET_GetPowerSavingSetting(void);

/*****************************************************************************/
//  Description :MMIAPISET_GetPowerSavingSwitch
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 设置省电模式参数
/*****************************************************************************/
PUBLIC void MMIAPISET_SetPowerSavingSetting(MMISET_POWER_SETTING_ITEM_T power_sav_set);
                                
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
/*****************************************************************************/
//  Description :MMIAPISET_IsProgramRunning
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 检查各个后台程序是否开启
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsProgramRunning(void);

/*****************************************************************************/
//  Description :MMIAPISET_OpenPowerSettingMode
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 开启省电模式
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenPowerSettingMode(void);

/*****************************************************************************/
//  Description :MMIAPISET_ResetPowerSavSetting
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 恢复出厂设置
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetPowerSavSetting(void);
#endif

#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
/*****************************************************************************/
//  Description :MMIAPISET_SetBatteryDisSwitch
//  Global resource dependence : 
//  Author:juan.wu
//  Note:设置电池电量在status bar上显示
/*****************************************************************************/
PUBLIC void MMIAPISET_SetBatteryPercentDisSwitch(BOOLEAN is_switch);

/*****************************************************************************/
//  Description :MMIAPISET_GetPowerSavingSwitch
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 获得是否要在status bar上显示电池电量百分比
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetBatteryPercentDisSwitch(void);

#endif


/*****************************************************************************/
//  Description : get the type of time display type
//  Global resource dependence : none
//  Author: SCV
//  Note:
/*****************************************************************************/
PUBLIC MMISET_TIME_DISPLAY_TYPE_E MMIAPISET_GetTimeDisplayType(void);

/*****************************************************************************/
// 	Description : set time display type 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetTimeDisplayType(MMISET_TIME_DISPLAY_TYPE_E time_display_type);

/*****************************************************************************/
//  Description : set date display type 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetDateDisplayType(MMISET_DATE_DISPLAY_TYPE_E date_display_type);

/*****************************************************************************/
//  Description : get the type of clock display type
//  Global resource dependence : none
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC MMISET_CLOCK_DISPLAY_STYLE_E MMIAPISET_GetClockDisplayStyle( void );

/*****************************************************************************/
//  Description : set clock display type
//  Global resource dependence : none
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetClockDisplayStyle( MMISET_CLOCK_DISPLAY_STYLE_E clock_display_style );

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
// 	Description : Set the any key and reject mode to nv
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetAnykeyRejectMode(MMISET_ANYKEY_REJECT_SET_T *anykey_setting_ptr);
#endif

/*****************************************************************************/
//  Description : MMIAPISET_IsWallpaperCall
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsWallpaperCall(void);
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
/*****************************************************************************/
//  Description : MMIAPISET_IsPoweronCall
//  Global resource dependence : 
//  Author:liyan.zhu
//  Note: copy MMIAPISET_IsWallpaperCall
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsPoweronCall(void);

/*****************************************************************************/
//  Description : MMIAPISET_IsPoweroffCall
//  Global resource dependence : 
//  Author:liyan.zhu
//  Note: copy MMIAPISET_IsWallpaperCall
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsPoweroffCall(void);
#endif
#ifdef SCREENSAVER_SUPPORT
/*****************************************************************************/
//  Description : MMIAPISET_IsScreensaverCall
//  Global resource dependence : 
//  Author:liyan.zhu
//  Note: copy MMIAPISET_IsWallpaperCall
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsScreensaverCall(void);
#endif
#ifdef MMI_SIM_LANGUAGE_SUPPORT
/*****************************************************************************/
//  Description : MMIAPISET_GetPreferLanguageTypeFromSim
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetPreferLanguageTypeFromSim(
                                   MN_DUAL_SYS_E dual_sys,
                                   MMISET_LANGUAGE_TYPE_E *prefer_language
                                   );

/*****************************************************************************/
//  Description : MMIAPISET_ConvertNVLangToSysType
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 设置g_ksp_lang 
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_ConvertNVLangToSysType(
                                   MMISET_LANGUAGE_TYPE_E   set_language   
                                   );

/*****************************************************************************/
//  Description : MMIAPISET_GetNVLanguageType
//  Global resource dependence : 
//  Author:michael.shi
//  Note: return s_nv_lang_setting to
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetNVLanguageType( void );
#endif

/*****************************************************************************/
//  Description : MMISET_LoadPreferNetworkList.
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 读取sim中的优选网络列表，sim的读操作是同步进行的。
/*****************************************************************************/
PUBLIC MN_PHONE_PLMN_SEL_LIST_T MMIAPISET_LoadPreferNetworkList(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : MMIAPISET_SavePreferNetworkList
//  Global resource dependence : 
//  Author:michael.shi
//  Note: 将更新后的优选网络列表写入sim卡。
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SavePreferNetworkList(
                                   MN_DUAL_SYS_E   dual_sys,
                                   MN_PHONE_PLMN_SEL_LIST_T   *prefer_network   
                                   );

/*****************************************************************************/
//  Description : MMIAPISET_HandlePreferNetworkListCnf
//  Global resource dependence : 
//  Author:michael.shi
//  Note:  处理sim卡更新优选网络列表的confirm，sim的写操作是异步完成的。
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_HandlePreferNetworkListCnf(APP_MN_PREFER_PLMN_CHANGE_CNF_T* prefer_cnf);

#ifdef MMISRV_AUDIO_SUPPORT
/*****************************************************************************/
//  Description : play a music file.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_PlayMusicFile(
                        MMISET_ALL_RING_TYPE_E         ring_type,
                        BOOLEAN                        is_a2dp,
                        BOOLEAN                        is_consume,
                        const wchar                    *full_path_ptr,
                        uint16                         full_path_len,
                        uint32                         play_times,
                        uint32                         play_offset,
                        uint32                         vol,
                        MMISET_RING_CALLBACK_PFUNC    call_back
                        );
#endif
#ifdef FDN_SUPPORT
/*****************************************************************************/
// 	Description : open fdn window
//	Global resource dependence : 
//  Author:wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenFDNWindow(void);
#endif

/*****************************************************************************/
// 	Description :MMIAPISET_IsWizardCfnrc
//	Global resource dependence : 
//  Author:kelly.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsWizardCfnrc(void);

#ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:juan.wu
//	Note: 
/*****************************************************************************/
PUBLIC  MMI_RESULT_E MMIAPISET_SetOkButtonCallback(void);

/*****************************************************************************/
//  Description : SetCancelButtonCallback
//  Global resource dependence : none
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISET_SetCancelButtonCallback(void);
 
/*****************************************************************************/
//  Description : MMIAPISET_CreatButtonLayer供静态墙纸和屏保或其它模块使用 
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreatSetButton(
                                     MMI_WIN_ID_T win_id,
                                     MMI_CTRL_ID_T ok_ctrl_id,
                                     GUIBUTTON_CALLBACK_FUNC ok_call_back,
                                     GUIBUTTON_CALLBACK_FUNC cancel_call_back
                                     );

/*****************************************************************************/
//  Description : 创建button显示的层，并设置控件到层上,与
//                    MMIAPISET_DestroySetButtonLayer成对调用
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreateSetButtonLayer(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ok_ctrl_id);

/*****************************************************************************/
//  Description : 当创建了这两个button的控件失去焦点时调用
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_DestroySetButtonLayer(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ok_ctrl_id);
#endif

/*****************************************************************************/
//  Description : MMIAPISET_CreatButtonLayer供静态墙纸和屏保或其它模块使用 
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_CreatButtonLayer(
                                       MMI_WIN_ID_T win_id,
                                       MMI_CTRL_ID_T ok_ctrl_id,
                                       GUIBUTTON_CALLBACK_FUNC ok_call_back,
                                       GUIBUTTON_CALLBACK_FUNC cancel_call_back
                                       );

#if defined MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : MMIAPISET_OpenImInputLangSettingWin 
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenImInputLangSettingWin(void);
#endif

/*****************************************************************************/
//  Description : open longpower menu  
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_LongpowerCreatmenu(void);

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
//  Description : MMIAPISET_ResumePreLivewallpaper
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_ResumePreLivewallpaper(void);

/*****************************************************************************/
//  Description : MMISET_ResumePreLivewallpaper
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_ResumePDAPreLivewallpaper(void);
#endif


/*****************************************************************************/
//  Description : 添加当前输入语言的所有项
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 添加完成后，设置当前焦点项为给定语言项
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendInputLanguageListItem(
                                                  MMI_CTRL_ID_T ctrl_id, 
                                                  MMISET_LANGUAGE_TYPE_E language_type
                                                  );

/*****************************************************************************/
//  Description : 从对应的语言类型得到对应的语言字符串
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_GetInputLanguageString(
                                             MMISET_LANGUAGE_TYPE_E language_type,
                                             MMI_STRING_T *string
                                             );

/*****************************************************************************/
//  Description : 供外部使用的设置墙纸label值的接口
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC  void MMIAPISET_SetWallPaperName(void);

/*****************************************************************************/
// 	Description : get contrast of main LCD
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetMainLCDContrast(void);


/*****************************************************************************/
// 	Description : get is minute reminder open
//	Global resource dependence : none
//  Author: scv
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetMinuteReminderTime(void);

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : get is blacklist audio call
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsBlacklistAudioCall(void);

/*****************************************************************************/
//  Description : get is blacklist vedio call
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsBlacklistVedioCall(void);

/*****************************************************************************/
//  Description : get is blacklist message
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetIsBlacklistMessage(void);
#endif
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
/*****************************************************************************/
// 	Description : get GO main menu style
//	Global resource dependence : 
//  Author: KYLE.JIN
//	Note:
/*****************************************************************************/
PUBLIC GO_MENU_STYLE_E MMIAPISET_GetGOmenuStyle(void);

 
/*****************************************************************************/
// 	Description : set main menu style
//	Global resource dependence : 
//  Author: kyle.jin
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetGOmenuStyle(
                                    GO_MENU_STYLE_E     go_menu_style
                                    );
#endif

#ifdef PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER
/*****************************************************************************/
// 	Description : get the render effect 
//	Global resource dependence : 
//  Author: Arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC MMISET_EFFECT_RENDER_SETTING* MMISET_GetLightShadowRenderSetting(void);

/*****************************************************************************/
// 	Description : set the render effect 
//	Global resource dependence : 
//  Author: Arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetLightShadowRenderSetting(MMISET_EFFECT_RENDER_SETTING  *render_setting_ptr);
#endif

#ifdef PDA_UI_SUPPORT_U_IDLE
/*****************************************************************************/
// 	Description : get u idle effect style
//	Global resource dependence : 
//  Author: hongbo.lan
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISET_GetUIdleStyle(void);
#endif

#ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT
/*****************************************************************************/
//  Description : MMISET_AppendButtonLayer
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 这个button layer在full paint的时候要append一下
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendSetButtonLayer(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO* MMIAPISET_GetButtonLayer();
/*****************************************************************************/
//  Description : MMISET_AppendButtonLayer
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 这个button layer在button update时，clear一下层，变成透明
/*****************************************************************************/
PUBLIC void MMIAPISET_ClearButtonLayer(GUI_RECT_T update_rect);

#endif

#if defined MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :获取当前输入法语言字符串，注意字符串保存
//                      str_content_ptr长度，小心越界
//  Global resource dependence : 
//  Author: yaoguang.chen
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIAPISET_GetInputLanguageContent(uint16 *str_len_ptr, wchar *str_content_ptr);
#endif


#ifdef MMI_AUTO_RECORD_SUPPORT

/*****************************************************************************/
// 	Description : Get auto record flag
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetAutoRecordSetting(void);

/*****************************************************************************/
// 	Description : Set auto record flag
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetAutoRecordSetting(BOOLEAN is_autorecord);

#endif

#ifdef MMI_GPRS_DATA_SIM_SET
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenGPRSModeSetting(void); 
#endif
#if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
/*****************************************************************************/
// 	Description : Set DualSIM CFNRC
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetDualSIMCFNRC(MMI_HANDLE_T win_id);
#endif

#ifdef MMI_GPRS_DATA_SIM_SET
/*****************************************************************************/
// 	Description : set GPRS Mode type 
//	Global resource dependence : none
//  Author: taiping.lai
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetGPRSModeType(MMISET_GPRS_MODE_TYPE_E mode_type);

/*****************************************************************************/
// 	Description : get GPRS Mode type 
//	Global resource dependence : none
//  Author: taiping.lai
//	Note:
/*****************************************************************************/
PUBLIC MMISET_GPRS_MODE_TYPE_E MMIAPISET_GetGPRSModeType(void);

/*****************************************************************************/
// 	Description : TG mode restore factory setting
//	Global resource dependence : none
//  Author: taiping.lai
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_RestoreGPRSModeSetting(void);
#endif
#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : Reset Select Line
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetSelectLine(MN_DUAL_SYS_E dual_sys);

#endif //#ifdef MMI_CSP_SUPPORT
#ifdef MMI_MMS_ADJUST_VOLUME_SUPPORT
/*****************************************************************************/
// 	Description : Set multi and real ausrv sound by app  by multi ptr buf
//	Global resource dependence : 
//    Author: CBK
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetMultimRealVolume(uint8 vol);
#endif

#ifdef VIRTUAL_VIBRATE_FUNC
PUBLIC  void MMISRVAUD_StopVibrate( );
PUBLIC MMISRV_HANDLE_T MMISRVAUD_StartVibrate(uint16 pri, uint16 duation, uint16 interval, uint16 times, MMISRVAUD_VIBRATE_CB callback);
#endif


#ifdef MMI_MMS_ADJUST_VOLUME_SUPPORT
/*****************************************************************************/
// 	Description : Set multi and real ausrv sound by app  by multi ptr buf
//	Global resource dependence : 
//    Author: CBK
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetMultimRealVolume(uint8 vol);
#endif

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_HandleSOSVibrateTimer(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_HandleSOSTorchTimer(uint8 timer_id, uint32 param);


PUBLIC void MMIAPISET_SetSOSVibrate(MN_DUAL_SYS_E dual_sys, BOOLEAN is_personal, BOOLEAN is_need_torch);

/*****************************************************************************/
//  Description : stop SOS vibrate
//  Global resource dependence : none
//  Author: candice chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_StopSOSVibrate(void);

/*****************************************************************************/
//  Description : check SOS vibrate
//  Global resource dependence : none
//  Author: candice chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsSOSVibrate(void);
#endif
/*****************************************************************************/
// Description : open idle->settings window
// Global resource dependence :
// Author: bin.wang1
// Note:S
/*****************************************************************************/
PUBLIC void WatchSET_MainWin_Enter( void );

PUBLIC uint8 MMISET_GetWatchMenuStyle(void);

/*****************************************************************************/
//  Description : Is User Update Time
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:TRUE or FALSE
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsUserUpdateTime(void);

#ifdef ADULT_WATCH_SUPPORT
PUBLIC MMISET_WATCH_WEAR_E WATCHAPISET_GetWearType(void);
PUBLIC void WATCHAPISET_SetWearType(MMISET_WATCH_WEAR_E wear_type);
PUBLIC BOOLEAN WATCHAPISET_GetRaiseWrist(void);
PUBLIC void WATCHAPISET_SetRaiseWrist(BOOLEAN is_raise_wrist_on);
#endif
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
