/*****************************************************************************
** File Name:      mmieng_uitestwin.c                                        *
** Author:                                                                   *
** Date:           09/2004                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 9/2004         Allen

******************************************************************************/

#define _MMIENG_UITESTWIN_C_

/**--------------------------------------------------------------------------*
Include Files
**--------------------------------------------------------------------------*/
#include "mmi_app_eng_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"

#include "mmk_timer.h"
#include "mmieng_uitestwin.h"
#include "mmieng_export.h"
#include "window_parse.h"
#include "mmi_image.h"
#include "mmi_theme.h"
#include "mmi_text.h"
#include "mmiidle_export.h"
#include "mmiacc.h"
#include "guilcd.h"
#include "guicommon.h"
#include "guirichtext.h"
#include "guitext.h"
#include "guilabel.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_default.h"
#include "tb_dal.h"
#ifdef _MINITEST
#include "mmk_timer.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "mmi_osbridge.h"
#include "guifont.h"
#endif
#include "mmi_textfun.h"
#include "mmiidle_subwintab.h"
#include "gpio_prod_api.h"
#include "mmi_ring.h"
#include "mmisd_export.h"
//#include "guiref_scale.h"
//#ifdef SC6800_DVB
//  #include "scm_api.h"
//  #include "sc6800_reg.h"
//  #include "mmi_appmsg.h"
//#endif
#include "mmitv_out.h"
#include "mmi_appmsg.h"
#include "mmieng_text.h"
#include "mmieng_image.h"
#include "mmieng_id.h"
#include "mmieng_menutable.h"  /*lint !e766*/
#include "mmifm_export.h"
#include "mmifm_text.h"
#include "mmieng_nv.h"
#include "mmibt_export.h" // baokun add
#ifdef BLUETOOTH_SUPPORT
//#include "mmi_btinterface.h"
//#include "mmibt_text.h"
#include "mmibt_id.h"
#endif
//#include "mmieng.h"
#include "mmieng_internal.h"
#include "mmieng_position.h"

#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmifm_export.h"
#include "guiref_scale.h"
//#include "Isp_service.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmidc_export.h"
#include "guistring.h"
#include "guires.h"
/* BEGIN: Added by George.Liu 01526, 2010/7/13   PN:WiFi_UITest */
#ifdef MMI_WIFI_SUPPORT
#include "mmiwlan_text.h"
#include "mmiwifi_export.h"
#endif
#ifdef UAL_WIFI_SUPPORT
#include "ual_wifi.h"
#ifdef UAL_RF_AM_SUPPORT
#include "ual_rf_am.h"
#endif
#endif
/* END:   Added by George.Liu 01526, 2010/7/13  */
#ifdef DSP_AUDIO_III
#include "admm_III.h"
#else
//#include "admm_II.h"
#endif

#include "ref_param.h"
#include "msensor_drv.h"

#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#include "java_mmi_text.h"
#endif
#ifdef QQ_SUPPORT
#include "qq2009.h"
#include "qq_text.h"
#endif
#include "mmipub.h"
#include "mmisrvaud_api.h"
#include "guiform.h"    /*lint !e766*/
#include "guilistbox.h"
#include "sensor_atv.h"
#include "dal_recorder.h"
#include "prod_param.h"


#ifdef MEDIA_DEMO_SUPPORT

#include "aac_stream_demo.h"
#include "amr_stream_demo.h"
#include "amr_record_demo.h"
#include "wav_stream_demo.h"
#include "mp3_stream_demo.h"

#ifdef MIC_SAMPLE_SUPPORT
#include "mic_sample_demo.h"
#endif

#include "audio_express_demo.h"

#include "dc_video_demo.h"
#include "display_yuv_demo.h"

#ifdef CAT_MVOICE_SUPPORT
#include "cat_mvoice.h"
#include "cat_mvoice_demo.h"
#endif

#endif

//camera
#ifdef  CAMERA_SUPPORT
#include "dal_dcamera.h"
#include "sensor_drv.h"
#endif
#include "production_test.h"
#include "mmieng_win.h"
//bug 746951
#include "mmicc_id.h"
#include "mmicc_internal.h"
#include "mmicc_export.h"
//bug 746951
#ifdef GPS_SUPPORT
#include "gps_drv.h"
//#include "mmigps_api.h"
#endif
#include "watch_common.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "mmk_timer.h"
#include "watch_qrcode_export.h"
#ifndef WIN32
#ifdef GOODIX_SUPPORT_GR5515
#include "production_line.h"
#endif
#endif
#ifdef ADULT_WATCH_SUPPORT
#ifdef BLUETOOTH_SUPPORT
#include "ual_bt_dev.h"
#endif
#endif
#ifdef UAL_GNSS_SUPPORT
#include "ual_gnss.h"
#endif

#include "watch_common_list.h"

#include "deep_sleep.h"
#include "nvitem.h"

/**--------------------------------------------------------------------------*
MACRO DEFINITION
**--------------------------------------------------------------------------*/
#ifdef MMI_ENG_WIFI_SUPPORT
#define UAL_WIFI_AP_NODE_NUMBER 30
#define UAL_WIFI_AP_NODE_BSSID_STR_LEN 25
#define UAL_WIFI_AP_NODE_RSSI_STR_LEN 15
#endif

#ifdef _MINITEST

#define LCD_TIMER_INDEX         0
#define BACKLIGHT_TIMER_INDEX   1
#define CAMERA_TIMER_INDEX      2

#define MINITEST_MAINLCD_TIMES              3
#define MINITEST_MAINLCD_TIMER_EXPIRE       700
#define MINITEST_BACKLIGHT_TIMES            2
#define MINITEST_BACKLIGHT_TIMER_EXPIRE     1000
#define MINITEST_CAMERA_TIMER_EXPIRE        3000

#define MINITEST_LCD_TIMER_NAME         "Mi0"
#define MINITEST_BACKLIGHT_TIMER_NAME   "Mi1"
#define MINITEST_CAMERA_TIMER_NAME      "Mi2"


#endif

//#ifndef MMI_ENG_GREENKEYTONEXT_ENABLE

#define MMIENG_TEST_FONT_16                                    MMI_DEFAULT_NORMAL_FONT
#define MMIENG_TEST_FONT_16_HEIGHT                      MMI_DEFAULT_NORMAL_FONT_SIZE
#define MMIENG_TEST_FONT_16_ASCII_WIDTH             (MMI_DEFAULT_NORMAL_FONT_SIZE/2)

#define MMIENG_PROGRESSBAR_HEIGHT   40

#define MMIENG_TEXT_FONT_SIZE       (SONG_FONT_22)
#define MMIENG_TEXT_FONT_COLOR      (MMI_BLACK_COLOR)

#define MMIENG_BATTERY_MAX_VOL 4100
#define MMIENG_ICHG_MAX 200
#define MMIENG_ICHG_MIN 100

#if defined (PLATFORM_SC6600L) || defined (PLATFORM_SC8800H)
#define MMIENG_TEST_KEY_TEXT_MAX_LEN             7
#else
#define MMIENG_TEST_KEY_TEXT_MAX_LEN             9
#endif

// video test
#include "dal_dcamera.h"
#include "sensor_drv.h"
#include "mmi_common.h"
#include "mmidisplay_color.h"
#include "guibutton.h"

#ifdef _MSG_VER <= 1600
#include "h264_enc_vt.h"
#include "h264_dec_vt.h"
#endif
#include "dc_cfg.h"

typedef enum
{
    MMIENG_TEST_CAMERA_WIN_MIN = 0,
        MMIENG_TEST_CAMERA_WIN_SIZE,
        MMIENG_TEST_CAMERA_WIN_NUM,
        MMIENG_TEST_CAMERA_WIN_POS,
        MMIENG_TEST_CAMERA_WIN_AWB,
        MMIENG_TEST_CAMERA_WIN_MAX
} MMIENG_TEST_CAMERA_WIN_FOCUS_E;


#ifdef MEDIA_DEMO_SUPPORT

typedef enum
{
    AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS = 0,//对于AMR录制来说，PLAY＝录音PAUSE＝录音 结束
    AUDIO_STREAM_PAUSE_OR_STOP_RECORD_OR_LEFT_TRACK,// 对于AMR录制来说，RESUME=回放  STOP=回放结束
    AUDIO_STREAM_RESUME_OR_PLAYBACK_OR_RIGHT_TRACK,
    AUDIO_STREAM_LEFT_AND_RIGHT_TRACK,
    AUDIO_STREAM_NULL_ONE,
    AUDIO_STREAM_NULL_TWO,
    AUDIO_STREAM_STOP = 6,
    AUDIO_STREAM_MAX
} MMIENG_MEDIA_AUDIO_DEMO_E;

typedef enum
{
    VIDEO_DEMO_START = 0,
    VIDEO_DEMO_NEXT,
    VIDEO_DEMO_STOP = 2,
    VIDEO_DEMO_MAX
} MMIENG_MEDIA_VIDEO_DEMO_E;

#endif

typedef struct
{
    MMIENG_TEST_CAMERA_WIN_FOCUS_E      focus_item;
    int                                                         photo_num;
    MMIFILE_DEVICE_E                                         photo_pos;
    BOOLEAN                                                is_AWB;//白平衡
} MMIENG_TEST_CAMERA_WIN_PARAM_T;
typedef struct
{
    uint8 idx;
    uint8 *test_win_name_ptr;
    int8  test_state;  //2 fail,0 not tesed,1 pass
}WATCH_ENG_TEST_RESULT;
#define WATCH_TEST_RESULT_NOTEST " not test"
#define WATCH_TEST_RESULT_PASS " test pass"
#define WATCH_TEST_RESULT_FAIL " test fail"
#define WATCH_ITEM_MAX_VALUE  (24)
uint8 g_item_test_value = 0;

LOCAL WATCH_ENG_TEST_RESULT g_test_result[WATCH_ITEM_MAX_VALUE] = {0};

#ifdef MEDIA_DEMO_SUPPORT
uint8 s_audio_cur_state = AUDIO_STREAM_MAX;
uint8 s_video_cur_state = VIDEO_DEMO_MAX;
#endif
uint32 s_loopback_volume_value_cur = 9;
#ifdef ADULT_WATCH_SUPPORT
#ifdef BLUETOOTH_SUPPORT
uint32 s_bt_dev_handle = 0;//接收bt dev 模块的消息handle
#endif
#endif
#ifdef GPS_SUPPORT
typedef enum
{
    GNSS_SATELLITE_TYPE_GPS,
    GNSS_SATELLITE_TYPE_BEIDOU,
    GNSS_SATELLITE_TYPE_GALIEO,
    GNSS_SATELLITE_TYPE_GLONASS,
    GNSS_SATELLITE_TYPE_MAX
}ENG_GNSS_SATELLITE_TYPE_E;
#endif

/**--------------------------------------------------------------------------*
STATIC DEFINITION
**--------------------------------------------------------------------------*/
static uint32       s_mmieng_rtc_counter = 0;
static uint8        s_mmieng_testrtc_timer_id = 0;
static uint8        s_mmieng_testbacklight_timer_id = 0;
static BOOLEAN  s_mmieng_test_backlight_on = TRUE;
static uint8        s_mmieng_key_ind_buf[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
static BOOLEAN  s_mmieng_headset_is_in = FALSE;
static uint8        s_mmieng_melody_volume = 0;
static BOOLEAN      s_is_in_eng_mode = FALSE;

#ifdef _MINITEST
static MMI_UITEST_STEP  s_mmieng_minitest_step = MMI_UI_MAIN_LCD;
static uint8        s_mmieng_testlcd_counter = 0;
static uint8        s_mmieng_testlcd_timer_id = 0;
static uint8        s_mmieng_testbacklight_counter = 0;
static uint8        s_mmieng_testdc_timer_id = 0;
#endif

LOCAL SP09_PHASE_CHECK_T  s_mmieng_product = {0}; //get phase check value

LOCAL MMI_STRING_T s_mmieng_str_up_to_next = {0};
LOCAL MMIENG_TEST_CAMERA_WIN_PARAM_T s_mmieng_test_camera_param = {
    MMIENG_TEST_CAMERA_WIN_MIN,
        0,
        MMI_DEVICE_SYSTEM,
        FALSE};



    LOCAL const wchar s_key_to_next_wstr[] =
        //#ifndef MMI_ENG_GREENKEYTONEXT_ENABLE
    { 'P', 'a', 's', 's', '/', 'F', 'a', 'i', 'l', ' ', 'k', 'e', 'y', ' ', 't', 'o', ' ', 'n', 'e', 'x', 't', 0 };
    //#else
    //{ 'G', 'r', 'e', 'e', 'n' ,' ', 'k', 'e', 'y', ' ', 't', 'o', ' ', 'n', 'e', 'x', 't', 0 };
    //#endif

    /**--------------------------------------------------------------------------*
    TYPE AND CONSTANT
    **--------------------------------------------------------------------------*/

const MMI_UITEST_DISPLAY_T mmi_uitest_display =
{
#ifdef MMI_PDA_SUPPORT
	MMI_DEFAULT_TEXT_FONT,              // text_font
	MMI_BLACK_COLOR,					// text_color
	MMI_WHITE_COLOR,					// bkgd_color
	0                                   // char_spac
#else
	MMI_DEFAULT_TEXT_FONT,              // text_font
	MMI_DEFAULT_TEXT_COLOR,				// text_color
	MMI_WINDOW_BACKGROUND_COLOR,		// bkgd_color
	0                                   // char_space
#endif
};

    //key test global data structure
    //added by ying.xu for qwerty keypad test
    typedef struct
    {
        int32 msg_id;
        struct KEY_POSITON_T  //key test display positon ,  7X10 for qwerty keypad
        {
            int32 x;
            int32 y;
        }key_position;
        int32 width ;           //default 1,may be oher value
        int32 pressed_flag;     //default 0,means not pressed, 1 pressed
    }KEY_MAP_T;

    typedef struct
    {
        int32 key_num ;
        int32 row;
        int32 collion;
        int32 key_height ;
        int32 key_width;
        KEY_MAP_T *key_map_ptr;
    } KEY_TEST_T;

#if defined PLATFORM_UWS6121E
static const char* s_key_string[] = {"SHORTCUT", "POWER"};
static const KEY_MAP_T s_key_map[] =
{
    //ROW 0
    {MSG_KEYDOWN_SHORTCUT,      {0,0}, 1, 0},
    {MSG_KEYDOWN_RED,        	{0,1}, 1, 0},
};
#endif

#if defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE
static const char* s_key_string[] = {"MENU", "HOME", "RETURN", "POWER","VOL+","VOL-", "CAMERA"};
static const KEY_MAP_T s_key_map[] =
{
    //ROW 0
    {MSG_KEYDOWN_MENU,       {0,0}, 1, 0},
    {MSG_KEYDOWN_RED,        {0,1}, 1, 0},
    {MSG_KEYDOWN_CANCEL,     {0,2}, 1, 0},
    {MSG_KEYDOWN_POWER,      {1,0}, 1, 0},
    {MSG_KEYDOWN_UPSIDE,     {1,1}, 1, 0},
    {MSG_KEYDOWN_DOWNSIDE,   {1,2}, 1, 0},
    {MSG_KEYDOWN_CAMERA,     {2,0}, 1, 0},
};
#endif

#if defined KEYPAD_TYPE_QWERTY_KEYPAD
    // 7 X 10
#ifndef MAINLCD_DEV_SIZE_160X128
    static const char* s_key_string[] = {
        "SL",   "SR",   "UP",   "DOWN", "LEFT",     "RIGHT","OK",   "HANG", "RED",
        "GREEN1",       "GREEN2",       "V+",       "V-",
        "HOMEPAGE",     "CALENDER",     "NOTES",    "SMS",  "CAMERA",       "TV",  "MP3",
        "Q",    "W",    "E",    "R 1",  "T 2",      "Y 3",  "U *",  "I",    "O",    "P",      
        "A",    "S",    "D",    "F 4",  "G 5",      "H 6",  "J #",  "K",    "L",    "DEL",      
        "Z",    "X",    "C",    "V 7",  "B 8",      "N 9",  "M 0",  ",",    ".",    "ENTER",      
        "ALT",  "SHIFT",        "/",    "SPACE",                    "'",    "!",    "CTRL"       
    };

    //MUST be matched with s_key_string
    static const KEY_MAP_T s_key_map[] =
    {
        //ROW 0
        {MSG_KEYDOWN_OK,        {0,0}, 1, 0},
        {MSG_KEYDOWN_CANCEL,    {0,1}, 1, 0},
        {MSG_KEYDOWN_UP,        {0,2}, 1, 0},
        {MSG_KEYDOWN_DOWN,      {0,3}, 1, 0},
        {MSG_KEYDOWN_LEFT,      {0,4}, 1, 0},
        {MSG_KEYDOWN_RIGHT,     {0,5}, 1, 0},
        {MSG_KEYDOWN_WEB,       {0,6}, 1, 0},
        {MSG_KEYDOWN_HANG,       {0,7}, 1, 0},
        {MSG_KEYDOWN_RED,       {0,8}, 1, 0},
        //ROW 1
        {MSG_KEYDOWN_CALL1,     {1,0}, 2, 0},
        {MSG_KEYDOWN_CALL2,     {1,2}, 2, 0},
        {MSG_KEYDOWN_UPSIDE,      {1,4}, 1, 0},
        {MSG_KEYDOWN_DOWNSIDE,    {1,5}, 1, 0},
        //ROW 2
        {MSG_KEYDOWN_IE,        {2,0}, 2, 0},
        {MSG_KEYDOWN_CALENDER,  {2,2}, 2, 0},
        {MSG_KEYDOWN_NOTES,     {2,4}, 1, 0},
        {MSG_KEYDOWN_SMS,       {2,5}, 1, 0},
        {MSG_KEYDOWN_CAMERA,    {2,6}, 2, 0},
        {MSG_KEYDOWN_TV,        {2,8}, 1, 0},
        {MSG_KEYDOWN_MP3,       {2,9}, 1, 0},
        //ROW 3
        {MSG_KEYDOWN_Q,         {3,0}, 1, 0},
        {MSG_KEYDOWN_W,         {3,1}, 1, 0},
        {MSG_KEYDOWN_E,         {3,2}, 1, 0},
        {MSG_KEYDOWN_1,         {3,3}, 1, 0},   //MSG_KEYDOWN_R
        {MSG_KEYDOWN_2,         {3,4}, 1, 0},   //MSG_KEYDOWN_T
        {MSG_KEYDOWN_3,         {3,5}, 1, 0},   //MSG_KEYDOWN_Y
        {MSG_KEYDOWN_STAR,      {3,6}, 1, 0},   //MSG_KEYDOWN_U
        {MSG_KEYDOWN_I,         {3,7}, 1, 0},
        {MSG_KEYDOWN_O,         {3,8}, 1, 0},
        {MSG_KEYDOWN_P,         {3,9}, 1, 0},
        //ROW 4
        {MSG_KEYDOWN_A,         {4,0}, 1, 0},
        {MSG_KEYDOWN_S,         {4,1}, 1, 0},
        {MSG_KEYDOWN_D,         {4,2}, 1, 0},
        {MSG_KEYDOWN_4,         {4,3}, 1, 0},   //MSG_KEYDOWN_F
        {MSG_KEYDOWN_5,         {4,4}, 1, 0},   //MSG_KEYDOWN_G
        {MSG_KEYDOWN_6,         {4,5}, 1, 0},   //MSG_KEYDOWN_H
        {MSG_KEYDOWN_HASH,      {4,6}, 1, 0},   //MSG_KEYDOWN_J
        {MSG_KEYDOWN_K,         {4,7}, 1, 0},
        {MSG_KEYDOWN_L,         {4,8}, 1, 0},
        {MSG_KEYDOWN_DEL,       {4,9}, 1, 0},
        //ROW 5
        {MSG_KEYDOWN_Z,         {5,0}, 1, 0},
        {MSG_KEYDOWN_X,         {5,1}, 1, 0},
        {MSG_KEYDOWN_C,         {5,2}, 1, 0},
        {MSG_KEYDOWN_7,         {5,3}, 1, 0},   //MSG_KEYDOWN_V
        {MSG_KEYDOWN_8,         {5,4}, 1, 0},   //MSG_KEYDOWN_B
        {MSG_KEYDOWN_9,         {5,5}, 1, 0},   //MSG_KEYDOWN_N
        {MSG_KEYDOWN_0,         {5,6}, 1, 0},   //MSG_KEYDOWN_M
        {MSG_KEYDOWN_COMMA,     {5,7}, 1, 0},
        {MSG_KEYDOWN_PERIOD,    {5,8}, 1, 0},
        {MSG_KEYDOWN_ENTER,     {5,9}, 1, 0},
        // ROW 6
        {MSG_KEYDOWN_FN,        {6,0}, 1, 0},
        {MSG_KEYDOWN_SHIFT,     {6,1}, 2, 0},
        {MSG_KEYDOWN_AT_QWERTY, {6,3}, 1, 0},
        {MSG_KEYDOWN_SPACE,     {6,4}, 3, 0},
        {MSG_KEYDOWN_AND,       {6,7}, 1, 0},
        {MSG_KEYDOWN_QUESTION,  {6,8}, 1, 0},
        {MSG_KEYDOWN_CTRL,      {6,9}, 1, 0},
    };
#else  //for 160X128 miniscreen 
    static const char* s_key_string[] = {
        "SL",   "SR",   "UP",   "DOWN", "LEFT",     "RIGHT",
        "GREEN1", "GREEN2",       "V+",       "V-",  "HANG", 
        "OK",   "RED",  "NOTE",  "SMS",  "TV",  "MP3", 
        "Q",    "W",    "E",    "R",  "T",      "Y",  "U",  "I",    "O",    "P",     
        "A",    "S",    "D",    "F",  "G",      "H",  "J",  "K",    "L",    "DEL",       
        "Z",    "X",    "C",    "V",  "B",      "N",  "M",  ",",    ".",    "ENTER",     
        "ALT",  "SHIFT",        "/",    "SPACE",            "'",    "!",    "CTRL",
		"HOMEPAGE",     "CALENDER",    "CAM", 
    };

	//MUST be matched with s_key_string
    static const KEY_MAP_T s_key_map[] =
    {
        //ROW 0
        {MSG_KEYDOWN_OK,        {0,0}, 3, 0},
        {MSG_KEYDOWN_CANCEL,    {0,3}, 3, 0},
        {MSG_KEYDOWN_UP,        {0,6}, 3, 0},
        {MSG_KEYDOWN_DOWN,      {0,9}, 5, 0},
        {MSG_KEYDOWN_LEFT,      {0,14}, 4, 0},
        {MSG_KEYDOWN_RIGHT,     {0,18}, 5, 0},

        //ROW 1
        {MSG_KEYDOWN_CALL1,     {1,0}, 6, 0},
        {MSG_KEYDOWN_CALL2,     {1,6}, 6, 0},
        {MSG_KEYDOWN_UPSIDE,    {1,12}, 3, 0},
        {MSG_KEYDOWN_DOWNSIDE,  {1,15}, 3, 0},
		{MSG_KEYDOWN_HANG,      {1,18}, 5, 0},
        //ROW 2
		{MSG_KEYDOWN_WEB,       {2,0}, 3, 0},
        {MSG_KEYDOWN_RED,       {2,3}, 4, 0},
        {MSG_KEYDOWN_NOTES,     {2,7}, 5, 0},
		{MSG_KEYDOWN_SMS,       {2,12}, 4, 0},
		{MSG_KEYDOWN_TV,        {2,16}, 3, 0},
        {MSG_KEYDOWN_MP3,       {2,19}, 4, 0},
        
		//ROW 3
        {MSG_KEYDOWN_Q,         {3,0}, 2, 0},
        {MSG_KEYDOWN_W,         {3,2}, 2, 0},
        {MSG_KEYDOWN_E,         {3,4}, 2, 0},
        {MSG_KEYDOWN_1,         {3,6}, 2, 0},   //MSG_KEYDOWN_R
        {MSG_KEYDOWN_2,         {3,8}, 2, 0},   //MSG_KEYDOWN_T
        {MSG_KEYDOWN_3,         {3,10}, 2, 0},   //MSG_KEYDOWN_Y
        {MSG_KEYDOWN_STAR,      {3,12}, 2, 0},   //MSG_KEYDOWN_U
        {MSG_KEYDOWN_I,         {3,14}, 2, 0},
        {MSG_KEYDOWN_O,         {3,16}, 2, 0},
        {MSG_KEYDOWN_P,         {3,18}, 2, 0},

        //ROW 4
        {MSG_KEYDOWN_A,         {4,0}, 2, 0},
        {MSG_KEYDOWN_S,         {4,2}, 2, 0},
        {MSG_KEYDOWN_D,         {4,4}, 2, 0},
        {MSG_KEYDOWN_4,         {4,6}, 2, 0},   //MSG_KEYDOWN_F
        {MSG_KEYDOWN_5,         {4,8}, 2, 0},   //MSG_KEYDOWN_G
        {MSG_KEYDOWN_6,         {4,10}, 2, 0},   //MSG_KEYDOWN_H
        {MSG_KEYDOWN_HASH,      {4,12}, 2, 0},   //MSG_KEYDOWN_J
        {MSG_KEYDOWN_K,         {4,14}, 2, 0},
        {MSG_KEYDOWN_L,         {4,16}, 2, 0},
        {MSG_KEYDOWN_DEL,       {4,18}, 5, 0},


        //ROW 5
        {MSG_KEYDOWN_Z,         {5,0}, 2, 0},
        {MSG_KEYDOWN_X,         {5,2}, 2, 0},
        {MSG_KEYDOWN_C,         {5,4}, 2, 0},
        {MSG_KEYDOWN_7,         {5,6}, 2, 0},   //MSG_KEYDOWN_V
        {MSG_KEYDOWN_8,         {5,8}, 2, 0},   //MSG_KEYDOWN_B
        {MSG_KEYDOWN_9,         {5,10}, 2, 0},   //MSG_KEYDOWN_N
        {MSG_KEYDOWN_0,         {5,12}, 2, 0},   //MSG_KEYDOWN_M
        {MSG_KEYDOWN_COMMA,     {5,14}, 2, 0},
        {MSG_KEYDOWN_PERIOD,    {5,16}, 2, 0},
        {MSG_KEYDOWN_ENTER,     {5,18}, 5, 0},

        // ROW 6
        {MSG_KEYDOWN_FN,        {6,0}, 3, 0},
        {MSG_KEYDOWN_SHIFT,     {6,3}, 5, 0},
        {MSG_KEYDOWN_AT_QWERTY, {6,8}, 2, 0},
        {MSG_KEYDOWN_SPACE,     {6,10}, 5, 0},
        {MSG_KEYDOWN_AND,       {6,15}, 2, 0},
        {MSG_KEYDOWN_QUESTION,  {6,17}, 2, 0},
        {MSG_KEYDOWN_CTRL,      {6,19}, 4, 0},
        
		//ROW7
		{MSG_KEYDOWN_IE,        {7,0}, 9, 0},
		{MSG_KEYDOWN_CALENDER,  {7,9}, 9, 0},
		{MSG_KEYDOWN_CAMERA,    {7,18}, 5, 0},
    };
#endif
#endif

#ifdef DC_TESTPATTERN_SUPPORT
    #define TEST_PATTERN_MODE TRUE
#else
    #define TEST_PATTERN_MODE FALSE
#endif

    /**--------------------------------------------------------------------------*
    LOCAL FUNCTION DECLARE
    **--------------------------------------------------------------------------*/

#ifdef _MINITEST
    LOCAL void RunMiniTestNextStep(void);

    LOCAL void MiniTimerCallBack(uint32 input);

    LOCAL void MiniTestLCDCallback(void);

    LOCAL void MiniTestMainLCD();

    LOCAL void MiniTestBacklightCallback(void);

    LOCAL void MiniTestBacklight();

    LOCAL void MiniTestCameraCallback(void);

    LOCAL void MiniTestCamera();
#endif
#if defined KEYPAD_TYPE_TYPICAL_KEYPAD
    LOCAL BOOLEAN TestKeyboard(MMI_MESSAGE_ID_E key_msg_id);
#endif

    LOCAL void TestMainLCD(MMI_MESSAGE_ID_E msg_id);

    //@Lin.Lin, 2005-2-2, start
    //LOCAL void TestSubLCD(MMI_MESSAGE_ID_E msg_id);

    LOCAL void SetBacklight(BOOLEAN is_open);

    //LOCAL void DrawCameraParamWin(void);

    //LOCAL void FocusNextCameraParamItem(void);
#if 0
    LOCAL MMIENG_TEST_CAMERA_WIN_PARAM_T GetCameraParam(void);
#endif
    //LOCAL void SetCameraParam(MMIENG_TEST_CAMERA_WIN_PARAM_T *param);

    //LOCAL void CameraParamInit(void);

    LOCAL void MMIENG_RunUITestInit(void);
#if defined KEYPAD_TYPE_TYPICAL_KEYPAD
    LOCAL void DrawTestKeyBG( void );
#endif
    LOCAL void EditWinDrawFocusRect(GUI_RECT_T rect,GUI_COLOR_T color);

//    LOCAL void Pause3DMMI( void );
//    LOCAL void Resume3DMMI( void );

    LOCAL int32 SetTestResult(uint32 win_id, uint32 result);
    LOCAL int32 SaveTestResult(uint32 win_id, uint32 result) ;
    LOCAL int32 ReadUITestResultNV( uint32* is_tested, uint32* result  );
    LOCAL int32 WriteUITestResultNV( uint32* is_tested, uint32* result  );
    LOCAL int32 SetMMITestResultBit(uint32* is_tested, uint32* result);
    LOCAL uint32  SetADC7MMITestBit(uint32 result);
    LOCAL int32 GetUITestResultInfo(char* buffer, int32 buffer_length);

#ifdef ENG_SUPPORT
    LOCAL MMI_RESULT_E ENGUITestMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

#ifdef MEDIA_DEMO_SUPPORT
    LOCAL MMI_RESULT_E ENGAudioDemoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
    LOCAL MMI_RESULT_E ENGVideoDemoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
    LOCAL  MMI_RESULT_E ENGUITestResultWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
    
//    LOCAL uint16 UITestLoopbackSetUlPA(AUDIO_DEVICE_MODE_TYPE_E mode_type, uint16 new_value);

#ifdef MOTION_SENSOR_TYPE
    LOCAL  MMI_RESULT_E ENGMotionSensorTestWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
#ifdef ATV_SUPPORT
    LOCAL BOOLEAN ENGUITestATVChannelWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
    LOCAL BOOLEAN ENGUITestATVChannelEditWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
    LOCAL BOOLEAN ENGUITestATVSignalWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
#if defined KEYPAD_TYPE_QWERTY_KEYPAD || defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE || defined (PLATFORM_UWS6121E)
    LOCAL int32 InitKeyTest(KEY_TEST_T *key_test_ptr);
    LOCAL int32 DrawKeyBackground(KEY_TEST_T *key_test_ptr);
    LOCAL int32 DrawKeyPressed(KEY_TEST_T *key_test_ptr);
    LOCAL int32 DrawKeyPressedByIndex(KEY_TEST_T *key_test_ptr, int32 index);
    LOCAL int32 DrawKeyAll(KEY_TEST_T *key_test_ptr);
    LOCAL int32 HandleKeyPressed(KEY_TEST_T *key_test_ptr, int32 msg_id);
    LOCAL int32 FinishKeyTest(KEY_TEST_T *key_test_ptr);
    LOCAL int32 GetKeyRect(GUI_RECT_T *rect,KEY_TEST_T *key_test_ptr, int32 index);
#endif
    LOCAL  MMI_RESULT_E ENGUITestPhoneInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
    LOCAL int32 GetPhoneInfo(char* buffer, int32 buffer_length);

    LOCAL void GetItemTestResult(void);
    LOCAL void DisplayItemTestResult(MMI_CTRL_ID_T ctrl_id);


/*****************************************************************************/
//  Description : Display loopback volume images.
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsVolumePanelOpen(
								void
								);

/*****************************************************************************/
// Description : set vol
// Global resource dependence :
// Author: 
// Note:
/*****************************************************************************/
LOCAL void SetVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr);

#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
/*****************************************************************************/
// 	Description : convert qwertykey to typicalkey
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_MESSAGE_ID_E ConvertKeyFromQW(MMI_MESSAGE_ID_E msg_id);
#endif

LOCAL BOOLEAN ENGUITestChargingWinHandleMsg(MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

LOCAL BOOLEAN UITestSimWinHandleMsg(MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );
#ifdef SENSORHUB_SUPPORT
LOCAL BOOLEAN UITestSensorHubWinHandleMsg(MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );
#endif
/*****************************************************************************/

LOCAL BOOLEAN ENGUITestShowVersionWinHandleMsg(MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );


//bug 746951
LOCAL BOOLEAN ENGUITestEmccWinHandleMsg(MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

//bug 746951

LOCAL BOOLEAN ENGUITestNTCWinHandleMsg(MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );



// 	Description : SetLabelTextByCharArray
//	Global resource dependence :
//  Author:
//	Note:  subject to: buffer_size < 50
/*****************************************************************************/
LOCAL void SetLabelTextByCharArray(MMI_CTRL_ID_T ctrl_id,
                                   char    *buffer,        //char array
                                   uint32   buffer_size,   //must <50
                                   BOOLEAN  is_update);

LOCAL BOOLEAN ENGUITestTouchScreenWinHandleMsg(MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );
PUBLIC MMI_RESULT_E UITestGPSWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

    /**--------------------------------------------------------------------------*
    GLOBAL DEFINITION
    **--------------------------------------------------------------------------*/
    
#ifdef _MINITEST
    MMI_TIMER_T g_minitest_timer[3] = 
    {
        {   
            PNULL, 
                0, 
                LCD_TIMER_INDEX, 
                PNULL
        },
        {   
                PNULL, 
                    0, 
                    BACKLIGHT_TIMER_INDEX, 
                    PNULL
            },
            {
                    PNULL, 
                        0, 
                        CAMERA_TIMER_INDEX, 
                        PNULL
                }
    };
#endif
#ifdef ENG_SUPPORT
    WINDOW_TABLE(MMIENG_UITEST_MENU_WIN_TAB) =
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)ENGUITestMenuWinHandleMsg),
        WIN_ID(MMIENG_UITEST_MENU_WIN_ID),
        WIN_TITLE(TXT_ENG_PHONETEST),
        CREATE_MENU_CTRL( MENU_PHONE_TEST, MMIENG_UITEST_MENU_CTRL_ID),
        WIN_SOFTKEY(TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN),
        END_WIN
    };
#endif

#ifdef MEDIA_DEMO_SUPPORT

    WINDOW_TABLE(MMIENG_MEIDA_AUDIO_DEMO_TAB) = 
    {
            WIN_FUNC((uint32)ENGAudioDemoWinHandleMsg),
            WIN_ID(MMIENG_MEDIA_AUDIO_DEMO_MENU_WIN_ID),
#if defined MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
            WIN_TITLE(TXT_ENG_MEDIA_AUDIO_DEMO),
            WIN_SOFTKEY(TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN),
            END_WIN
    };

    WINDOW_TABLE(MMIENG_MEIDA_VIDEO_DEMO_TAB) = 
    {
            WIN_FUNC((uint32)ENGVideoDemoWinHandleMsg),
            WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_INIT),
            WIN_ID(MMIENG_MEDIA_VIDEO_DEMO_MENU_WIN_ID),
#if defined MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
            WIN_TITLE(TXT_ENG_MEDIA_VIDEO_DEMO),
            WIN_SOFTKEY(TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN),
            END_WIN
    };
    
#endif
    
    WINDOW_TABLE(MMIENG_UITEST_RESULT_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)ENGUITestResultWinHandleMsg),
        WIN_ID(MMIENG_UITEST_RESULT_WIN_ID),
        WIN_TITLE(TXT_ENG_PHONETEST),
        CREATE_RICHTEXT_CTRL( MMIENG_UITEST_TEXT_CTRL_ID),
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_ENG_RETURN),
        END_WIN
    };
    WINDOW_TABLE(MMIENG_UITEST_PHONEINFO_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)ENGUITestPhoneInfoWinHandleMsg),
            WIN_ID(MMIENG_UITEST_PHONEINFO_WIN_ID),
            WIN_TITLE(TXT_ENG_PHONE_INFO),
            CREATE_TEXT_CTRL( MMIENG_UITEST_TEXT_CTRL_ID),
            WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_ENG_RETURN),
            END_WIN
    };
    
    WINDOW_TABLE(MMIENG_UIREADY_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UIReadyWinHandleMsg),
            WIN_ID(MMIENG_UIREADY_WIN_ID),
#if defined MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
            WIN_TITLE(TXT_ENG_UITEST_TITLE),
            CREATE_TEXT_CTRL( MMIENG_UITEST_TEXT_CTRL_ID),
            WIN_SOFTKEY(TXT_ENG_OK, TXT_NULL, TXT_NULL),
            END_WIN
    };
    
    WINDOW_TABLE(MMIENG_UITESTKEY_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestKeyWinHandleMsg),    
            WIN_ID(MMIENG_UITESTKEY_WIN_ID),
            WIN_HIDE_STATUS,
#if defined MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
#if defined KEYPAD_TYPE_QWERTY_KEYPAD               
            WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_90|WIN_SUPPORT_ANGLE_270),            
#else            
            WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT),
#endif
#if defined MMI_PDA_SUPPORT || defined(PLATFORM_UWS6121E)
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),    
#endif
            END_WIN
    };
    
    WINDOW_TABLE(MMIENG_UITESTMAINLCD_WIN_TAB) = 
    {
        //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32)UITestMainLCDWinHandleMsg),
            WIN_ID(MMIENG_UITESTMAINLCD_WIN_ID),
            WIN_HIDE_STATUS,
            WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
            END_WIN
    };
    
    //add by linlin, 2005-2-2
    //WINDOW_TABLE(MMIENG_UITESTSUBLCD_WIN_TAB) = 
    //{
    //  //CLEAR_LCD,
    //  //WIN_PRIO( WIN_ONE_LEVEL ),
    //  WIN_FUNC((uint32)UITestSubLCDWinHandleMsg),
    //  WIN_ID(MMIENG_UITESTSUBLCD_WIN_ID),
    //  WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    //    END_WIN
    // };
    
    WINDOW_TABLE(MMIENG_UITESTRTC_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestRtcWinHandleMsg),
            WIN_ID(MMIENG_UITESTRTC_WIN_ID),
#ifdef MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
            WIN_TITLE(TXT_ENG_UIRTC_TITLE),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),    
            END_WIN
    };
    
    WINDOW_TABLE(MMIENG_UITESTVIBRATE_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestVibrateWinHandleMsg),
            WIN_ID(MMIENG_UITESTVIBRATE_WIN_ID),
#if defined MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif   
            WIN_TITLE(TXT_ENG_UIVIBRATE_TITLE),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
            END_WIN
    };
    
    WINDOW_TABLE(MMIENG_UITESTBACKLIGHT_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestBacklightWinHandleMsg),
            WIN_ID(MMIENG_UITESTBACKLIGHT_WIN_ID),
#if defined MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif   
            WIN_TITLE(TXT_ENG_UIBACKLIGHT_TITLE),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
            END_WIN
    };
    
#ifdef  CAMERA_SUPPORT
    WINDOW_TABLE(MMIENG_UITESTCAMERA_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestCameraWinHandleMsg),
            WIN_STYLE( WS_DISABLE_COMMON_BG),
            WIN_ID(MMIENG_UITESTCAMERA_WIN_ID),
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),// Bug 1337717
            WIN_HIDE_STATUS,
            END_WIN
    };
    WINDOW_TABLE(MMIENG_UITESTVIDEO_WIN_TAB) =
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestVideoWinHandleMsg),
        WIN_STYLE( WS_DISABLE_COMMON_BG),
        WIN_ID(MMIENG_UITESTVIDEO_WIN_ID),
        WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
        WIN_HIDE_STATUS,
        END_WIN
    };
#endif

    WINDOW_TABLE(MMIENG_UITESTHEADSET_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestHeadsetWinHandleMsg),
            WIN_ID(MMIENG_UITESTHEADSET_WIN_ID),
#if defined MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
            WIN_TITLE(TXT_COMM_HEADSET),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
            END_WIN
    };
    
    WINDOW_TABLE(MMIENG_UITESTLOOPBACK_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestLoopbackWinHandleMsg),
            WIN_ID(MMIENG_UITESTLOOPBACK_WIN_ID),
#if defined MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY | WS_DISPATCH_TO_CHILDWIN),
#else
			WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
#endif
            WIN_TITLE(TXT_ENG_UILOOPBACK_TITLE),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
            END_WIN
    };
    
#ifdef BLUETOOTH_SUPPORT
    WINDOW_TABLE(MMIENG_UITESTBLUETOOTH_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestBluetoothWinHandleMsg),
            WIN_ID(MMIENG_UITESTBLUETOOTH_WIN_ID),
#if defined MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
            WIN_TITLE(TXT_BLUETOOTH),
			CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_TEXTBOX_CTRL_ID),
            //CREATE_TEXT_CTRL(MMIENG_UITEST_TEXTBOX_CTRL_ID),
            WIN_SOFTKEY(TXT_ENG_PASS, TXT_NULL, TXT_ENG_FAIL ),
            END_WIN
    };
#endif

    WINDOW_TABLE(MMIENG_UITESTMELODY_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestMelodyWinHandleMsg),
            WIN_ID(MMIENG_UITESTMELODY_WIN_ID),
#if defined MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
            WIN_TITLE(TXT_ENG_UIMELODY_TITLE),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
            WIN_SOFTKEY(TXT_ENG_PASS, TXT_NULL, TXT_ENG_FAIL ),
            END_WIN
    };
    
    WINDOW_TABLE(MMIENG_UITESTSD_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestSDWinHandleMsg),
            WIN_ID(MMIENG_UITESTSD_WIN_ID),
#if defined MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
            WIN_TITLE(TXT_ENG_UISD_TITLE),  
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
            END_WIN
    };
  
    //WINDOW_TABLE(MMIENG_UITESTTVOUT_WIN_TAB) =
    //{
    //  //CLEAR_LCD,
    //  //WIN_PRIO( WIN_ONE_LEVEL ),
    //  WIN_FUNC((uint32)UITestTVOutWinHandleMsg),
    //  WIN_ID(MMIENG_UITESTTVOUT_WIN_ID),
    //  CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
    //  WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    //    END_WIN
    // };
    
#ifdef APP_ENG_TEST_TFLASH_SUPPORT
    WINDOW_TABLE(MMIENG_UITESTTFLASH_WIN_TAB) =
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestTFlashWinHandleMsg),
            WIN_ID(MMIENG_UITESTTFLASH_WIN_ID),
            WIN_TITLE(TXT_NULL),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
            END_WIN
    };
#endif
    
#ifdef APP_ENG_COMTEST_SUPPORT
    WINDOW_TABLE(MMIENG_UITESTCOM_WIN_TAB) =
    {
        //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32)UITestCOMWinHandleMsg),
            WIN_ID(MMIENG_UITESTCOM_WIN_ID),
            WIN_TITLE(TXT_NULL),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL3_CTRL_ID),
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
            END_WIN
    };
    
#endif
    
#ifdef FM_SUPPORT
    WINDOW_TABLE(MMIENG_UITESTFM_WIN_TAB) = 
    {
        //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32)UITestFmWinHandleMsg),
            WIN_ID(MMIENG_UITESTFM_WIN_ID),
#if defined MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
            WIN_TITLE(TXT_FM),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
            END_WIN
    };
#endif
        
    /* BEGIN: Added by George.Liu 01526, 2010/7/12   PN:WiFi_UITest */
#ifdef MMI_ENG_WIFI_SUPPORT
    WINDOW_TABLE(MMIENG_UITESTWIFI_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestWiFiWinHandleMsg),
        WIN_ID(MMIENG_UITESTWIFI_WIN_ID),
#if defined MMI_PDA_SUPPORT
        WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
        //WIN_TITLE(TXT_WIFI),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
#ifdef MMI_ENG_WIFI_SUPPORT
        CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMIENG_UITEST_LISTBOX_CTRL_ID ),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIENG_UITEST_LABEL1_CTRL_ID),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL2_CTRL_ID),
        CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT, MMIENG_UITEST_LABEL3_CTRL_ID),
#endif
        WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
        END_WIN
    };
#endif

#ifdef GPS_SUPPORT
    WINDOW_TABLE(MMIENG_UITEST_GPS_WIN_TAB) =
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestGPSWinHandleMsg),
        WIN_ID(MMIENG_UITEST_GPS_WIN_ID),
        WIN_TITLE(TXT_GPSTEST),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIENG_LISTBOX_CTRL_ID),
        WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),

        END_WIN
    };

#endif
    /* END:   Added by George.Liu 01526, 2010/7/12  */

#ifdef MOTION_SENSOR_TYPE 
WINDOW_TABLE(MMIENG_MOTIONSENSOR_WIN_TAB) = 
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)ENGMotionSensorTestWinHandleMsg),
    WIN_ID(MMIENG_MOTIONSENSOR_WIN_ID),
#if defined MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
    WIN_TITLE(TXT_ENG_MOTIONSENSOR_TITLE),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIENG_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_LABEL1_CTRL_ID, MMIENG_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_LABEL2_CTRL_ID, MMIENG_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_LABEL3_CTRL_ID, MMIENG_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_LABEL4_CTRL_ID, MMIENG_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_LABEL5_CTRL_ID, MMIENG_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_LABEL6_CTRL_ID, MMIENG_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_LABEL7_CTRL_ID, MMIENG_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
    END_WIN
};
#endif

#ifdef ATV_SUPPORT
    WINDOW_TABLE(MMIENG_UITESTATV_CHANNEL_WIN_TAB) =
    {
        WIN_FUNC((uint32)ENGUITestATVChannelWinHandleMsg),
        WIN_ID(MMIENG_UITESTATV_CHANNEL_WIN_ID),
//#if defined MMI_PDA_SUPPORT
//        WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
//#endif
        WIN_TITLE(TXT_ATV_CHANNEL),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIENG_UITEST_LISTBOX_CTRL_ID),
        WIN_SOFTKEY(STXT_SELECT , TXT_NULL, TXT_EDIT),
        END_WIN
    };

    WINDOW_TABLE(MMIENG_UITESTATV_CHANNEL_EDIT_WIN_TAB) =
    {
        WIN_FUNC((uint32)ENGUITestATVChannelEditWinHandleMsg),
        WIN_ID(MMIENG_UITESTATV_CHANNEL_EDIT_WIN_ID),
#if defined MMI_PDA_SUPPORT
        WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
        WIN_TITLE(TXT_ATV_CHANNEL_EDIT),
        WIN_SOFTKEY(TXT_COMMON_OK , TXT_NULL, STXT_RETURN),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_UITEST_FORM1_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_UITEST_FORM2_CTRL_ID,MMIENG_UITEST_FORM1_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE,MMIENG_UITEST_LABEL1_CTRL_ID,MMIENG_UITEST_FORM2_CTRL_ID),
            CHILD_EDIT_DIGITAL_CTRL(TRUE, 2, MMIENG_UITEST_EDITBOX_CTRL_ID, MMIENG_UITEST_FORM2_CTRL_ID),
        END_WIN
    };

    WINDOW_TABLE(MMIENG_UITESTATV_SIGNAL_WIN_TAB) =
    {
        WIN_FUNC((uint32)ENGUITestATVSignalWinHandleMsg),
        WIN_ID(MMIENG_UITESTATV_SIGNAL_WIN_ID),
#if defined MMI_PDA_SUPPORT
        WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
        WIN_TITLE(TXT_ATV_SIGNAL),                
        WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_UITEST_FORM2_CTRL_ID),   
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE,MMIENG_UITEST_LABEL1_CTRL_ID,MMIENG_UITEST_FORM2_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE,MMIENG_UITEST_LABEL2_CTRL_ID,MMIENG_UITEST_FORM2_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE,MMIENG_UITEST_LABEL3_CTRL_ID,MMIENG_UITEST_FORM2_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE,MMIENG_UITEST_LABEL4_CTRL_ID,MMIENG_UITEST_FORM2_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE,MMIENG_UITEST_LABEL5_CTRL_ID,MMIENG_UITEST_FORM2_CTRL_ID),
        END_WIN
    };
#endif

    WINDOW_TABLE(MMIENG_UITEST_CHARGING_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)ENGUITestChargingWinHandleMsg),
            WIN_ID(MMIENG_UITEST_CHARGING_WIN_ID),
#if defined MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
            WIN_TITLE(TXT_ENG_TEST_CHARGING),                
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
            CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),            
            END_WIN
    };

#ifdef ENG_SUPPORT
WINDOW_TABLE(MMIENG_UITESTSIM_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestSimWinHandleMsg),
        WIN_ID(MMIENG_UITESTSIM_WIN_ID),
        WIN_TITLE(TXT_ENG_TEST_SIM),                
        WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
        CREATE_RICHTEXT_CTRL(MMIENG_TESTSIM_CTRL_ID),
            END_WIN
    };
#endif
#ifdef SENSORHUB_SUPPORT
WINDOW_TABLE(MMIENG_UITEST_SENSORHUB_WIN_TAB) = 
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)UITestSensorHubWinHandleMsg),
        WIN_ID(MMIENG_UITEST_SENSORHUB_WIN_ID),
        WIN_TITLE(TXT_ENG_TEST_SENSORHUB),
        WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
            END_WIN
    };
#endif
//bug 746951
    WINDOW_TABLE(MMIENG_UITEST_EMCC_WIN_TAB) = 
    {
            WIN_HIDE_STATUS,
            WIN_FUNC((uint32)ENGUITestEmccWinHandleMsg),
            WIN_ID(MMIENG_UITEST_EMCC_WIN_ID),
#if defined MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMIENG_UITEST_LABEL_CTRL_ID),
            WIN_TITLE(TXT_ENG_TEST_EMCC),                
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
            //CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),            
            END_WIN
    };
//bug 746951


    WINDOW_TABLE(MMIENG_UITEST_NTC_WIN_TAB) = 
    {
            WIN_HIDE_STATUS,
            WIN_FUNC((uint32)ENGUITestNTCWinHandleMsg),
            WIN_ID(MMIENG_UITEST_NTC_WIN_ID),
#if defined MMI_PDA_SUPPORT
            WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_UITEST_LABEL_CTRL_ID),
            CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIENG_LABEL1_CTRL_ID),
            //WIN_TITLE(TXT_ENG_NTC_TEST),                
            WIN_SOFTKEY(TXT_ENG_PASS , TXT_NULL, TXT_ENG_FAIL),
            //CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),            
            END_WIN
    };

#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
//adjust touch screen
    WINDOW_TABLE(MMIENG_TOUCHSCREEN_MENU_WIN_TAB) =
    {
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)ENGUITestTouchScreenWinHandleMsg),
        WIN_ID(MMIENG_TOUCHSCREEN_WIN_ID),
        WIN_SOFTKEY(TXT_ENG_PASS, TXT_NULL, TXT_ENG_FAIL),
        END_WIN
    };
#endif
    //s_mmieng_test_win_ptr_tab must match with s_mmieng_test_win_id    
   //s_mmieng_test_win_ptr_tab must match with s_mmieng_test_win_id     
    const  uint32*  s_mmieng_test_win_ptr_tab[] =
    {
            MMIENG_UIREADY_WIN_TAB,
#if !defined  TOUCH_PANEL_SUPPORT || defined(PLATFORM_UWS6121E)
            MMIENG_UITESTKEY_WIN_TAB,
#endif
            MMIENG_UITESTMAINLCD_WIN_TAB,
            MMIENG_UITESTRTC_WIN_TAB,
            MMIENG_UITESTVIBRATE_WIN_TAB,
            MMIENG_UITESTBACKLIGHT_WIN_TAB,
            MMIENG_UITESTLOOPBACK_WIN_TAB,
            
#ifdef  CAMERA_SUPPORT    
#ifndef _WIN32
            MMIENG_UITESTCAMERA_WIN_TAB,
#ifndef PLATFORM_UWS6121E
            MMIENG_UITESTVIDEO_WIN_TAB,
#endif
#endif
#endif

#ifdef BLUETOOTH_SUPPORT
            //BT should before camera
            MMIENG_UITESTBLUETOOTH_WIN_TAB,
#endif
#if !defined (PLATFORM_ANTISW3) && !defined(PLATFORM_UWS6121E)
            MMIENG_UITESTHEADSET_WIN_TAB,
#endif
            MMIENG_UITESTMELODY_WIN_TAB,   
#if defined MSDC_CARD_SUPPORT && !defined(PLATFORM_UWS6121E)
#ifndef ADULT_WATCH_SUPPORT
            MMIENG_UITESTSD_WIN_TAB,
#endif
#endif

#ifdef APP_ENG_TEST_TFLASH_SUPPORT
            MMIENG_UITESTTFLASH_WIN_TAB,
#endif
#ifdef APP_ENG_COMTEST_SUPPORT
            MMIENG_UITESTCOM_WIN_TAB,
#endif 
#ifdef FM_SUPPORT
            MMIENG_UITESTFM_WIN_TAB,
#endif
            
            /* BEGIN: Added by George.Liu 01526, 2010/7/13   PN:WiFi_UITest */
#ifdef MMI_ENG_WIFI_SUPPORT
            MMIENG_UITESTWIFI_WIN_TAB,
#endif
            /* END:   Added by George.Liu 01526, 2010/7/13  */
            
#ifdef MOTION_SENSOR_TYPE               
            MMIENG_MOTIONSENSOR_WIN_TAB,
#endif
            
#ifdef ATV_SUPPORT
            MMIENG_UITESTATV_CHANNEL_WIN_TAB,
#endif
            MMIENG_UITEST_CHARGING_WIN_TAB,

           MMIENG_SHOWVERSION_WIN_TAB,
#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
           MMIENG_TOUCHSCREEN_MENU_WIN_TAB,
#endif
          MMIENG_UITESTSIM_WIN_TAB,

#ifdef SENSORHUB_SUPPORT
#ifndef UAL_FACTORY_TEST_SUPPORT
           MMIENG_UITEST_SENSORHUB_WIN_TAB,
#endif
#endif
        MMIENG_UITEST_EMCC_WIN_TAB,

	MMIENG_UITEST_NTC_WIN_TAB,
#ifdef GPS_SUPPORT
        MMIENG_UITEST_GPS_WIN_TAB,
#endif
            PNULL
    };
//s_mmieng_test_win_id must match with s_mmieng_test_win_ptr_tab
    const  uint32 s_mmieng_test_win_id[] =
    {
	     MMIENG_UIREADY_WIN_ID,
#if !defined  TOUCH_PANEL_SUPPORT || defined(PLATFORM_UWS6121E)
            MMIENG_UITESTKEY_WIN_ID,
#endif
            MMIENG_UITESTMAINLCD_WIN_ID,
            MMIENG_UITESTRTC_WIN_ID,
            MMIENG_UITESTVIBRATE_WIN_ID,
            MMIENG_UITESTBACKLIGHT_WIN_ID,
            MMIENG_UITESTLOOPBACK_WIN_ID,

#ifdef  CAMERA_SUPPORT
#ifndef _WIN32
            MMIENG_UITESTCAMERA_WIN_ID,
#ifndef PLATFORM_UWS6121E
            MMIENG_UITESTVIDEO_WIN_ID,
#endif
#endif
#endif

#ifdef BLUETOOTH_SUPPORT
            //BT should before camer
            MMIENG_UITESTBLUETOOTH_WIN_ID,
#endif
#if !defined (PLATFORM_ANTISW3) && !defined(PLATFORM_UWS6121E)
            MMIENG_UITESTHEADSET_WIN_ID,
#endif
            MMIENG_UITESTMELODY_WIN_ID,

#if defined MSDC_CARD_SUPPORT && !defined(PLATFORM_UWS6121E)
#ifndef ADULT_WATCH_SUPPORT
            MMIENG_UITESTSD_WIN_ID    ,
#endif
#endif

#ifdef APP_ENG_TEST_TFLASH_SUPPORT
            MMIENG_UITESTTFLASH_WIN_ID,
#endif
#ifdef APP_ENG_COMTEST_SUPPORT
            MMIENG_UITESTCOM_WIN_ID,
#endif
#ifdef FM_SUPPORT
            MMIENG_UITESTFM_WIN_ID,
#endif


#ifdef MMI_ENG_WIFI_SUPPORT
            MMIENG_UITESTWIFI_WIN_ID,
#endif


#ifdef MOTION_SENSOR_TYPE
    MMIENG_MOTIONSENSOR_WIN_ID,
#endif

#ifdef ATV_SUPPORT
            MMIENG_UITESTATV_SIGNAL_WIN_ID,
#endif
            MMIENG_UITEST_CHARGING_WIN_ID,

            MMIENG_SHOWVERSION_WIN_ID,           
#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
            MMIENG_TOUCHSCREEN_WIN_ID,
#endif
            MMIENG_UITESTSIM_WIN_ID,

#ifdef SENSORHUB_SUPPORT
#ifndef UAL_FACTORY_TEST_SUPPORT
            MMIENG_UITEST_SENSORHUB_WIN_ID,
#endif
#endif
            MMIENG_UITEST_EMCC_WIN_ID,

            MMIENG_UITEST_NTC_WIN_ID,
#ifdef GPS_SUPPORT
            MMIENG_UITEST_GPS_WIN_ID,
#endif
    };
    /*
    const char*  s_mmieng_test_name[] =
    {
    "KEY",
    "MAINLCD",
    "RTC",
    "VIBRATE",
    "BACKLIGHT",
    "LOOPBACK",
    #ifdef  CAMERA_SUPPORT 
    "CAMERA",
    #endif
    #ifdef BLUETOOTH_SUPPORT
    //BT should before camera
    "BLUETOOTH",
    #endif   
    "HEADSET",    
    "MELODY",   
    #ifdef MSDC_CARD_SUPPORT
    "SD",
    #endif
    #ifdef APP_ENG_TEST_TFLASH_SUPPORT
    "TFLASH",
    #endif
    #ifdef APP_ENG_COMTEST_SUPPORT
    "TCOM",
    #endif 
    #ifdef FM_SUPPORT
    "FM",
    #endif
    #ifdef GPS_SUPPORT
    "GPS",
    #endif

        #ifdef WIFI_SUPPORT
        "WIFI",
        #endif
        
          };
    */
    
const uint32 s_all_test_win_id[]  = 
    {
#if !defined  TOUCH_PANEL_SUPPORT || defined(PLATFORM_UWS6121E)
        MMIENG_UITESTKEY_WIN_ID,
#endif
        MMIENG_UITESTMAINLCD_WIN_ID,
        MMIENG_UITESTRTC_WIN_ID,
        MMIENG_UITESTVIBRATE_WIN_ID,
        MMIENG_UITESTBACKLIGHT_WIN_ID,
        MMIENG_UITESTLOOPBACK_WIN_ID,

#ifdef  CAMERA_SUPPORT
#ifndef _WIN32
       MMIENG_UITESTCAMERA_WIN_ID,
#ifndef PLATFORM_UWS6121E
       MMIENG_UITESTVIDEO_WIN_ID,
#endif
#else
       0,
       0,
#endif
#else
       0,
       0,
#endif //#ifdef  CAMERA_SUPPORT

#ifdef BLUETOOTH_SUPPORT
       //BT should before camer
       MMIENG_UITESTBLUETOOTH_WIN_ID,
#else
       0,
#endif
#if !defined (PLATFORM_ANTISW3) && !defined(PLATFORM_UWS6121E)
       MMIENG_UITESTHEADSET_WIN_ID,
#else
       0,
#endif
       MMIENG_UITESTMELODY_WIN_ID,

#if defined MSDC_CARD_SUPPORT && !defined(PLATFORM_UWS6121E)
#ifndef ADULT_WATCH_SUPPORT
       MMIENG_UITESTSD_WIN_ID  ,
#endif
#else
       0,
#endif

#ifdef APP_ENG_TEST_TFLASH_SUPPORT
       MMIENG_UITESTTFLASH_WIN_ID,
#else
       0,
#endif

#ifdef APP_ENG_COMTEST_SUPPORT
       MMIENG_UITESTCOM_WIN_ID,
#else
       0,
#endif

#ifdef FM_SUPPORT
       MMIENG_UITESTFM_WIN_ID,
#else
       0,
#endif

       0 ,

#ifdef MMI_TVOUT_ENABLE
       0,
#else
       0,
#endif

#ifdef MMI_ENG_WIFI_SUPPORT
       MMIENG_UITESTWIFI_WIN_ID,
#else
       0,
#endif

#ifdef MOTION_SENSOR_TYPE   
        MMIENG_MOTIONSENSOR_WIN_ID,
#else
    0,
#endif

#ifdef ATV_SUPPORT
        MMIENG_UITESTATV_SIGNAL_WIN_ID,
#else
            0,   
#endif

        MMIENG_UITEST_CHARGING_WIN_ID,
        MMIENG_SHOWVERSION_WIN_ID,
#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
        MMIENG_TOUCHSCREEN_WIN_ID,
#endif
        MMIENG_UITESTSIM_WIN_ID,
#ifdef SENSORHUB_SUPPORT
#ifndef UAL_FACTORY_TEST_SUPPORT
        MMIENG_UITEST_SENSORHUB_WIN_ID,
#endif
#endif
        MMIENG_UITEST_EMCC_WIN_ID,
#ifdef GPS_SUPPORT
        MMIENG_UITEST_GPS_WIN_ID,
#else
       0 ,
 #endif
        MMIENG_UITEST_NTC_WIN_ID,
    };

  //all test name must match with all test win id
    const char*  s_all_test_name[] =
    {
#if !defined  TOUCH_PANEL_SUPPORT || defined(PLATFORM_UWS6121E)
        "KEY",
#endif
            "MAINLCD",
            "RTC",
            "VIBRATE",
            "BACKLIGHT",
            "LOOPBACK",    
            
#ifdef  CAMERA_SUPPORT   
#ifndef _WIN32   
            "CAMERA",
#ifndef PLATFORM_UWS6121E
            "VIDEO",//bug1910649
#endif
#else
            "/0",
            "/0",//bug1910649
#endif
#else
            "/0",
            "/0",//bug1910649
#endif  //#ifdef  CAMERA_SUPPORT

#ifdef  BLUETOOTH_SUPPORT   
            "BLUETOOTH",
#else
            "/0",
#endif  //#ifdef  CAMERA_SUPPORT
#if !defined (PLATFORM_ANTISW3) && !defined(PLATFORM_UWS6121E)
            "HEADSET",
#else
            "/0",
#endif
            "MELODY",      
            
#if defined MSDC_CARD_SUPPORT && !defined(PLATFORM_UWS6121E)
#ifndef ADULT_WATCH_SUPPORT
            "SD"  ,
#endif
#else
            "/0",          
#endif
            
#ifdef APP_ENG_TEST_TFLASH_SUPPORT
            "TFLASH",
#else
            "/0",
#endif
            
#ifdef APP_ENG_COMTEST_SUPPORT
            "TCOM", 
#else
            "/0",
#endif 
            
#ifdef FM_SUPPORT
            "FM",
#else
            "/0" ,
#endif
            "/0" ,
            
#ifdef MMI_TVOUT_ENABLE
            " TVOUT", 
#else
            "/0" ,
#endif
            
#ifdef MMI_ENG_WIFI_SUPPORT
            "WIFI",
#else
            "/0" ,
#endif

#ifdef MOTION_SENSOR_TYPE
        "Motion Sensor",
#else
        "/0",
#endif

#ifdef ATV_SUPPORT
            "ATV",
#else
            "/0" ,
#endif
        "CHARGING",
        "VERSION",

#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
        "TP",
#endif

        "SIM",
#ifdef SENSORHUB_SUPPORT
#ifndef UAL_FACTORY_TEST_SUPPORT
        "SensorHub",
#endif
#endif
        //bug 746951
        "EMCC",
        //bug 746951
 #if defined(GPS_SUPPORT)
        "GPS",
 #else
        "/0",
 #endif
	"NTC"
    };

    LOCAL  uint32 s_is_fulltest = 1;    //default as full test mode
    LOCAL  uint32 s_is_tested = 0;      //0:not tested; 1,tested
    LOCAL  uint32 s_test_result = 0;    //test rulat in memory
    LOCAL  int    s_mmieng_test_win_tab_index = 0;
    LOCAL  BOOLEAN s_is_pause_3DMMI = FALSE;
    /**--------------------------------------------------------------------------*
    EXTERNAL DECLARE
    **--------------------------------------------------------------------------*/
    
    extern BOOLEAN g_mmieng_showversion_from_ui;
    
    //extern PUBLIC void GPIO_SetLcdBackLight(BOOLEAN b_on);
    
    //extern PUBLIC BOOLEAN GPIO_CheckHeadsetStatus( void );
    
    /*===============================================================================
    IMPLEMENTATION
    ===============================================================================*/
    
    
#ifdef _MINITEST
    /********************************************************************************
    NAME:          MMIENG_StartMiniTest
    DESCRIPTION:   
    PARAM IN:      None
    PARAM OUT:     None
    AUTHOR:        allen
    DATE:          2004.10.23
    ********************************************************************************/
    void MMIENG_StartMiniTest(void)
    {
        LCD_EnterSleep(GUI_MAIN_LCD_ID, FALSE); // quit LCD sleep
#ifndef WIN32
        GPIO_SetLcdBackLight(TRUE);         // open LCD backlight
        GPIO_SetKeyPadBackLight(TRUE);
#endif
        
        MiniTestMainLCD();
        s_mmieng_minitest_step = MMI_UI_MAIN_LCD;
    }
    
    /********************************************************************************
    NAME:          RunMiniTestNextStep
    DESCRIPTION:   
    PARAM IN:      None
    PARAM OUT:     None
    AUTHOR:        allen
    DATE:          2004.10.23
    ********************************************************************************/
    LOCAL void RunMiniTestNextStep(void)
    {
        GUI_RECT_T text_rect = {5, 60, 127, 80};
        GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
        GUISTR_STYLE_T      text_style      = {0};
        GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK; 
        MMI_STRING_T        disp_data = {0};
        
        lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
        lcd_dev_info.block_id = GUI_BLOCK_MAIN;
        
        
        switch (s_mmieng_minitest_step)
        {
        case MMI_UI_MAIN_LCD:
            s_mmieng_minitest_step = MMI_UI_BACKLIGHT;
            MiniTestBacklight();
            break;
            
        case MMI_UI_BACKLIGHT:
            s_mmieng_minitest_step = MMI_UI_CAMERA;
            MiniTestCamera();
            break;
            
        case MMI_UI_CAMERA:
            s_mmieng_minitest_step = MMI_UI_END;
            TestMainLCD(MSG_KEYDOWN_2);     // white color clear LCD
            
            // style
            text_style.font = mmi_uitest_display.text_font;
            text_style.font_color = mmi_uitest_display.text_color;
            text_style.align = ALIGN_LEFT;
            text_style.char_space = mmi_uitest_display.char_space;
            text_style.line_space = 0;
            
            disp_data.wstr_ptr = {"Mini test is over"};
            disp_data.wstr_len = 17;
            // display
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&text_rect,       //the fixed display area
                (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&disp_data,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                ); 
            break;
            
        case MMI_UI_END:
            break;
            
        default:
            break;
        }
    }
    
    /********************************************************************************
    NAME:          MiniTimerCallBack
    DESCRIPTION:   
    PARAM IN:      param -- 
    PARAM OUT:     None
    AUTHOR:        allen
    DATE:          2004.10.23
    ********************************************************************************/
    LOCAL void MiniTimerCallBack(uint32 input)
    {
        MMI_TIMER_T* timer = NULL;
        MmiTimerExpiryS* sendSignal = NULL;
        
        //check the pointer
        if(PNULL == input)
        {
             //SCI_TRACE_LOW:"mmieng_uitestwin.c MiniTimerCallBack input == null"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_1404_112_2_18_2_17_26_58,(uint8*)"");
             return ;
        }
        
        timer = (MMI_TIMER_T*)input;
        sendSignal = PNULL;
        // create the signal to send requesting task
        MmiCreateSignal(APP_MINITEST_EXPIRY, sizeof(MmiTimerExpiryS), (MmiSignalS**)&sendSignal);
        sendSignal->timerId = timer->timerId;
        sendSignal->ref_num = timer->ref_num;
        sendSignal->Sender = P_APP;
        MmiSendSignal(TASK_FL_ID, (MmiSignalS*)sendSignal);
    }
    
    /********************************************************************************
    NAME:          MMIENG_MiniTestTimerCallback
    DESCRIPTION:   
    PARAM IN:      signal_ptr -- 
    PARAM OUT:     None
    AUTHOR:        allen
    DATE:          2004.10.23
    ********************************************************************************/
    void MMIENG_MiniTestTimerCallback(Signal* signal_ptr)
    {
        MmiTimerExpiryS* timer = PNULL;
        
        if(PNULL == signal_ptr)
        {
            //SCI_TRACE_LOW:"mmieng_uitestwin.c MMIENG_MiniTestTimerCallback signal_ptr == null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_1432_112_2_18_2_17_26_59,(uint8*)"");
            return ;
        }
        timer = (MmiTimerExpiryS*)signal_ptr;
        
        if (0 == timer->ref_num)
        {
            MiniTestLCDCallback();
        }
        else if (1 == timer->ref_num)
        {
            MiniTestBacklightCallback();
        }
        else if (2 == timer->ref_num)
        {
            MiniTestCameraCallback();
        }
    }
    
    /********************************************************************************
    NAME:          MiniTestLCDCallback
    DESCRIPTION:   
    PARAM IN:      None 
    PARAM OUT:     None
    AUTHOR:        allen
    DATE:          2004.10.23
    ********************************************************************************/
    LOCAL void MiniTestLCDCallback(void)
    {
        s_mmieng_testlcd_counter--;
        if (0 >= s_mmieng_testlcd_counter)
        {
            MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
            
            
            SCI_DeleteTimer(g_minitest_timer[LCD_TIMER_INDEX].timer_ptr);
            g_minitest_timer[LCD_TIMER_INDEX].timer_ptr = PNULL;
            
            RunMiniTestNextStep();
        }
        else if ((MINITEST_MAINLCD_TIMES - 1) == s_mmieng_testlcd_counter)
        {
            TestMainLCD(MSG_KEYDOWN_8); // green
        }
        else if ((MINITEST_MAINLCD_TIMES - 2) == s_mmieng_testlcd_counter)
        {
            TestMainLCD(MSG_KEYDOWN_9); // blue
        }
    }
    
    /********************************************************************************
    NAME:          MiniTestMainLCD
    DESCRIPTION:   
    PARAM IN:      None
    PARAM OUT:     None
    AUTHOR:        allen
    DATE:          2004.10.23
    ********************************************************************************/
    LOCAL void MiniTestMainLCD()
    {
        s_mmieng_testlcd_counter = MINITEST_MAINLCD_TIMES;
        TestMainLCD(MSG_KEYDOWN_7); // red
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
        
        // creat timer
        g_minitest_timer[LCD_TIMER_INDEX].timer_ptr = 
            SCI_CreatePeriodTimer(  MINITEST_LCD_TIMER_NAME,    // timer name
            &MiniTimerCallBack, 
            (uint32)(&g_minitest_timer[LCD_TIMER_INDEX]),   // timer_id
            MINITEST_MAINLCD_TIMER_EXPIRE,  // time out
            TRUE
            );
    }
    
    /********************************************************************************
    NAME:          MiniTestBacklightCallback
    DESCRIPTION:   
    PARAM IN:      None 
    PARAM OUT:     None
    AUTHOR:        allen
    DATE:          2004.10.23
    ********************************************************************************/
    LOCAL void MiniTestBacklightCallback(void)
    {
        if (0 >= s_mmieng_testbacklight_counter)
        {
            // stop melody
            MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
            
            SCI_DeleteTimer(g_minitest_timer[BACKLIGHT_TIMER_INDEX].timer_ptr);
            g_minitest_timer[BACKLIGHT_TIMER_INDEX].timer_ptr = PNULL;
            
            SetBacklight(TRUE);
            RunMiniTestNextStep();
        }
        else
        {
            if (s_mmieng_test_backlight_on)
                s_mmieng_test_backlight_on = FALSE;
            else
                s_mmieng_test_backlight_on = TRUE;
            SetBacklight(s_mmieng_test_backlight_on);
            
            if (s_mmieng_test_backlight_on)
                s_mmieng_testbacklight_counter--;
        }
    }
    
    /********************************************************************************
    NAME:          MiniTestBacklight
    DESCRIPTION:   
    PARAM IN:      None
    PARAM OUT:     None
    AUTHOR:        allen
    DATE:          2004.10.23
    ********************************************************************************/
    LOCAL void MiniTestBacklight()
    {
        MMIAUD_RING_DATA_INFO_T     *ring_info_ptr = PNULL;
        uint16 ring_id = 0;
        
        SetBacklight(TRUE);
        TestMainLCD(MSG_KEYDOWN_7); // red
        
        s_mmieng_test_backlight_on = TRUE;
        s_mmieng_testbacklight_counter = MINITEST_BACKLIGHT_TIMES;
        s_mmieng_melody_volume = MMIAPISET_GetCallRingVolume();
        
        //MMIAUDIO_SetVolume(AUDIO_DEVICE_MODE_HANDFREE, AUD_MAX_SPEAKER_VOLUME);
        
        ring_id = 2;
        
        MMI_GetRingInfo(
            (ring_id + MMI_DEL_RING_START_ID - 1),
            ring_info_ptr);
        
        
        MMISET_PlayRingByPtr(MMISET_RING_TYPE_OTHER,
            MMISET_VOL_MAX,
            ring_info_ptr->data_ptr,
            ring_info_ptr->data_len,
            AUD_PLAY_FOREVER,
            ring_info_ptr->type,
            PNULL);
        
        // creat timer
        g_minitest_timer[BACKLIGHT_TIMER_INDEX].timer_ptr = 
            SCI_CreatePeriodTimer(  MINITEST_BACKLIGHT_TIMER_NAME,  // timer name
            &MiniTimerCallBack, 
            (uint32)(&g_minitest_timer[BACKLIGHT_TIMER_INDEX]), // timer_id
            MINITEST_BACKLIGHT_TIMER_EXPIRE,    // time out
            TRUE
            );
    }
    
    /********************************************************************************
    NAME:          MiniTestCameraCallback
    DESCRIPTION:   
    PARAM IN:      None 
    PARAM OUT:     None
    AUTHOR:        allen
    DATE:          2004.10.23
    ********************************************************************************/
    LOCAL void MiniTestCameraCallback(void)
    {
        DC_Close();                         //退出CAMERA模式
        
        SCI_DeleteTimer(g_minitest_timer[CAMERA_TIMER_INDEX].timer_ptr);
        g_minitest_timer[CAMERA_TIMER_INDEX].timer_ptr = PNULL;
        
        RunMiniTestNextStep();
    }
    
    /********************************************************************************
    NAME:          MiniTestCamera
    DESCRIPTION:   
    PARAM IN:      None
    PARAM OUT:     None
    AUTHOR:        allen
    DATE:          2004.10.23
    ********************************************************************************/
    LOCAL void MiniTestCamera()
    {   
        
        // creat timer
        g_minitest_timer[CAMERA_TIMER_INDEX].timer_ptr = 
            SCI_CreateTimer(    MINITEST_CAMERA_TIMER_NAME,     // timer name
            &MiniTimerCallBack, 
            (uint32)(&g_minitest_timer[CAMERA_TIMER_INDEX]),    // timer_id
            MINITEST_CAMERA_TIMER_EXPIRE,   // time out
            SCI_AUTO_ACTIVATE
            );
    }
#endif

/*****************************************************************************/
//  Description : Request a virtual audio service to prevent other app to play sound 
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMISRV_HANDLE_T MMIENG_RequestVirtualAudService(void)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};

    req.pri = MMISRVAUD_PRI_HIGH;
    audio_srv.info.type = MMISRVAUD_TYPE_VIRTUAL;
    return MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
}

/********************************************************************************/
// NAME:          MiniTestCamera
// DESCRIPTION:
// PARAM IN:      None
// PARAM OUT:     None
// AUTHOR:        songbin.zeng
// DATE:          2006.12.01
/********************************************************************************/
LOCAL void MMIENG_RunUITestInit(void)
{
    //    s_mmieng_str_up_to_next.is_ucs2 = FALSE;
    s_mmieng_str_up_to_next.wstr_ptr = s_key_to_next_wstr;   /*lint !e605*/
    s_mmieng_str_up_to_next.wstr_len = MMIAPICOM_Wstrlen(s_key_to_next_wstr);

    s_mmieng_test_win_tab_index = 0;
}

/********************************************************************************/
// NAME:          MMIAPIENG_StartUITest
// DESCRIPTION:
// PARAM IN:      None
// PARAM OUT:     None
// AUTHOR:        allen
// DATE:          2004.09.23
/********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_StartUITest(void)
{
#ifdef MMI_AUDIO_PLAYER_SUPPORT
    MMIAPIMP3_ResetAllMp3();
#endif
#ifdef FM_SUPPORT
    MMIAPIFM_Exit();
#endif
#ifdef KURO_SUPPORT
    MMIAPIKUR_ResetAllKur();
#endif
    MMIENG_RunUITestInit();

    MMIAPIENG_RunUITestNextStep();

    return TRUE;
}

/********************************************************************************/
//     NAME:          MMIAPIENG_VibrateWinGetFocus
//     DESCRIPTION:
//     PARAM IN:
//     PARAM OUT:
//     AUTHOR:        songbin.zeng
//     DATE:          2006.10.11
/********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_VibrateWinGetFocus(void)
{
    if (MMK_IsFocusWin(MMIENG_UITESTVIBRATE_WIN_ID))
    {
        MMK_SendMsg(MMIENG_UITESTVIBRATE_WIN_ID, MSG_GET_FOCUS, PNULL);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/********************************************************************************/
//     NAME:          MMIAPIENG_IsFocusInLoopBack
//     DESCRIPTION:
//     PARAM IN:
//     PARAM OUT:
//     AUTHOR:
//     DATE:
/********************************************************************************/
/*
    PUBLIC BOOLEAN MMIAPIENG_IsFocusInLoopBack(void)
    {
        if (MMK_IsFocusWin(MMIENG_UITESTLOOPBACK_WIN_ID))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
 */
    /********************************************************************************
    NAME:          MMIAPIENG_RunUITestNextStep
    DESCRIPTION:   
    PARAM IN:      None
    PARAM OUT:     None
    AUTHOR:        songbin.zeng
    DATE:          2006.12.01
    ********************************************************************************/
    PUBLIC BOOLEAN MMIAPIENG_RunUITestNextStep(void)
    {    
        if(0 != s_is_fulltest)
        {
            if (MMIENG_SHOWVERSION_WIN_TAB == s_mmieng_test_win_ptr_tab[s_mmieng_test_win_tab_index])
            {
                g_mmieng_showversion_from_ui = TRUE;    //enter from uitest
            }
            else
            {
                g_mmieng_showversion_from_ui = FALSE;   // enter from uitest
            }
            
            if (PNULL != s_mmieng_test_win_ptr_tab[s_mmieng_test_win_tab_index])
            { 
#ifdef UI_P3D_SUPPORT
#ifdef  CAMERA_SUPPORT    
                if (MMIENG_UITESTCAMERA_WIN_TAB == s_mmieng_test_win_ptr_tab[s_mmieng_test_win_tab_index])
                {
                    MMI_Disable3DEffect(MMI_3D_EFFECT_CAMERA);
                }
#endif                
#endif         
                if (MMIENG_UITEST_EMCC_WIN_TAB == s_mmieng_test_win_ptr_tab[s_mmieng_test_win_tab_index])
                {
                    uint8 *temp_str = "10086";
                    MMK_CreateWin((uint32 *)s_mmieng_test_win_ptr_tab[s_mmieng_test_win_tab_index ++], PNULL);
                    WatchAPICC_MakeCall(
                        MN_DUAL_SYS_MAX,
                        temp_str,
                        5,
                        PNULL,
                        PNULL,
                        CC_CALL_SIM_MAX,
                        CC_CALL_NORMAL_CALL,
                        PNULL
                        );
                }
                else
                {
                    MMK_CreateWin((uint32 *)s_mmieng_test_win_ptr_tab[s_mmieng_test_win_tab_index ++], PNULL);
                }

            }
            else
            {
                s_mmieng_test_win_tab_index = 0;        
                
                WriteUITestResultNV(&s_is_tested, &s_test_result);  
                SetMMITestResultBit(&s_is_tested, &s_test_result);
                MMK_CreateWin((uint32*)MMIENG_UITEST_RESULT_WIN_TAB,  PNULL);
            }
        }

        return TRUE;
    }

    /********************************************************************************
    NAME:          Pause3DMMI when in uitestcamera win
    DESCRIPTION:
    PARAM IN:
    PARAM OUT:
    AUTHOR:        zhaohui
    DATE:          2009.03.25
    ********************************************************************************/
//     LOCAL void Pause3DMMI( void )
//     {
//         if (MMI_Is3DMMIEnable())
//         {
//             if (!s_is_pause_3DMMI)
//             {
//                 //SCI_TRACE_LOW:"zhaohui Pause3DMMI"
//                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_1781_112_2_18_2_17_27_60,(uint8*)"");
// #ifdef UI_P3D_SUPPORT
//                 MMI_Disable3DEffect(MMI_3D_EFFECT_CAMERA);
// #endif
//                 //MMI_Enable3DMMI(FALSE);
//                 s_is_pause_3DMMI = TRUE;
//             }
//         }
//     }

    /********************************************************************************
    NAME:          Resume3DMMI when after uitestcamera win
    DESCRIPTION:
    PARAM IN:
    PARAM OUT:
    AUTHOR:        zhaohui
    DATE:          2009.03.25
    ********************************************************************************/
//     LOCAL void Resume3DMMI( void )
//     {
//         if (s_is_pause_3DMMI)
//         {
//             //SCI_TRACE_LOW:"zhaohui Resume3DMMI"
//             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_1803_112_2_18_2_17_27_61,(uint8*)"");
// #ifdef UI_P3D_SUPPORT
//             MMI_Enable3DEffect(MMI_3D_EFFECT_CAMERA);
// #endif
//             //MMI_Enable3DMMI(TRUE);
//             s_is_pause_3DMMI = FALSE;
//         }
//     }
//
    /********************************************************************************
    NAME:          UIReadyWinHandleMsg
    DESCRIPTION:   
    PARAM IN:      
    PARAM OUT:     
    AUTHOR:        allen
    DATE:          2004.09.08
    ********************************************************************************/
    PUBLIC MMI_RESULT_E UIReadyWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
    {
        MMI_RESULT_E result = MMI_RESULT_TRUE;

        switch (msg_id)
        {
        case MSG_OPEN_WINDOW:
            {
#if defined MMI_PDA_SUPPORT
                const char *str = {"SoftKey/Menu&Return Key to the next step.\nPlease press left softkey/menu key when successed.\nPress right key/return key when failed."} ;
#else
                const char *str = {"SoftKey to next.\nPlease press left key when successed.\nPress right key when failed."} ;
#endif
                uint32  length = 0;
                wchar   wstr[256] = {0} ;
                length = SCI_STRLEN(str);
                MMI_STRNTOWSTR(wstr, 255, (uint8*)str, length, length);
                GUITEXT_SetString(MMIENG_UITEST_TEXT_CTRL_ID,  wstr,  length,  FALSE);
            }
            break;

        case MSG_FULL_PAINT:
            break;

#if defined MMI_PDA_SUPPORT
        case MSG_APP_MENU:
#endif
        case MSG_KEYDOWN_OK:
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
        case MSG_CTL_OK:
            MMIAPIENG_RunUITestNextStep();
            MMK_CloseWin(win_id);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#ifdef MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif   
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_KEYDOWN_RED:
            break;

        case MSG_CLOSE_WINDOW:
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
        }
        return (result);
    }
    
    /********************************************************************************
    NAME:          TestKeyboard
    DESCRIPTION:   
    PARAM IN:      key_msg_id -- 
    PARAM OUT:     
    AUTHOR:        allen
    DATE:          2004.09.08
    ********************************************************************************/
#if defined KEYPAD_TYPE_TYPICAL_KEYPAD
    LOCAL BOOLEAN TestKeyboard(MMI_MESSAGE_ID_E key_msg_id)
    {
        //uint8               all_tested      = 0x00;
        int16               test_key_x      = 0;
        int16               test_key_y      = 0;
        int16               key_on_map_idx  = 0;
        int16               byte_idx        = 0;
        int16               bit_idx         = 0;
        uint8               key_buf_byte    = 0x00;
        GUI_RECT_T          draw_rect       = {0};
        GUI_RECT_T          update_rect     = {0};
        GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        
        
        //SCI_TRACE_LOW:"TestKeyboard key_msg_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_1908_112_2_18_2_17_27_62,(uint8*)"d",key_msg_id);
        
        
        switch (key_msg_id)
        {
        case MSG_KEYDOWN_1:
            test_key_y = 2;
            test_key_x = 0;
            break;
            
        case MSG_KEYDOWN_2:
            test_key_y = 2;
            test_key_x = 1;
            break;
            
        case MSG_KEYDOWN_3:
            test_key_y = 2;
            test_key_x = 2;
            break;
            
        case MSG_KEYDOWN_4:
            test_key_y = 2;
            test_key_x = 3;
            break;
            
        case MSG_KEYDOWN_5:
            test_key_y = 3;
            test_key_x = 0;
            break;
            
        case MSG_KEYDOWN_6:
            test_key_y = 3;
            test_key_x = 1;
            break;
            
        case MSG_KEYDOWN_7:
            test_key_y = 3;
            test_key_x = 2;
            break;
            
        case MSG_KEYDOWN_8:
            test_key_y = 3;
            test_key_x = 3;
            break;
            
        case MSG_KEYDOWN_9:
            test_key_y = 4;
            test_key_x = 0;
            break;
            
        case MSG_KEYDOWN_0:
            test_key_y = 4;
            test_key_x = 1;
            break;
            
        case MSG_KEYDOWN_STAR:
            test_key_y = 4;
            test_key_x = 2;
            break;
            
        case MSG_KEYDOWN_HASH:
            test_key_y = 4;
            test_key_x = 3;
            break;
            
        case MSG_KEYDOWN_UP:
            test_key_y = 0;
            test_key_x = 2;
            break;
            
        case MSG_KEYDOWN_DOWN:
            test_key_y = 0;
            test_key_x = 3;
            break;
            
        case MSG_KEYDOWN_LEFT:
            test_key_y = 1;
            test_key_x = 2;
            break;
            
        case MSG_KEYDOWN_RIGHT:
            test_key_y = 1;
            test_key_x = 3;
            break;
            
        case MSG_KEYDOWN_GREEN:
            test_key_y = 0;
            test_key_x = 1;
            break;
            
        case MSG_KEYDOWN_RED:
            test_key_y = 1;
            test_key_x = 1;
            break;
#if defined MMI_PDA_SUPPORT
        case MSG_APP_MENU:            
#endif 
        case MSG_KEYDOWN_OK:
            test_key_y = 1;
            test_key_x = 0;
            break;
            
        case MSG_KEYDOWN_CANCEL:
            test_key_y = 0;
            test_key_x = 0;
            break;
            
        case MSG_KEYDOWN_WEB:
#if defined (KEYPAD_TYPE_COMPACT_KEYPAD )
            test_key_y = 2;
            test_key_x = 0;
#else
            test_key_y = 5;
            test_key_x = 0;
#endif
            break;
        case MSG_KEYDOWN_UPSIDE:
            test_key_y = 5;
            test_key_x = 1;
            break;
        case MSG_KEYDOWN_DOWNSIDE:
            test_key_y = 5;
            test_key_x = 2;
            break;
        case MSG_KEYDOWN_CAMERA:
            test_key_y=5;
            test_key_x=1;
            break;
        case MSG_KEYDOWN_CALL2:   //green2
            test_key_y=5;
            test_key_x=1;
            break;
        case MSG_KEYDOWN_TV:   //ATV
            test_key_y=5;
            test_key_x=2;
            break;
            
        default:
            break;
    }
    key_on_map_idx = test_key_y * 4 + test_key_x;
    key_on_map_idx %= UI_TEST_KEY_NUM;
    byte_idx = key_on_map_idx / 8;
    bit_idx = key_on_map_idx % 8;
    key_buf_byte = s_mmieng_key_ind_buf[byte_idx];
    s_mmieng_key_ind_buf[byte_idx] &= (unsigned char)(~(1<<bit_idx));/*lint !e502*/  
    // show key passed
    if ((key_buf_byte^s_mmieng_key_ind_buf[byte_idx]) != 0)
    {        
        {        
            GUI_RECT_T rect = MMITHEME_GetClientRect();
            int32 TESTPAD_KEY_WIDTH = (rect.right- rect.left - MMIENG_TESTPAD_MARGIN_X*2)>>2 ;
            
            draw_rect.left = MMIENG_TESTPAD_MARGIN_X + test_key_x * TESTPAD_KEY_WIDTH;
            draw_rect.top = MMIENG_TESTPAD_MARGIN_Y + test_key_y * MMIENG_TESTPAD_KEY_HIGHT;
            draw_rect.right = draw_rect.left + TESTPAD_KEY_WIDTH - 1;
            draw_rect.bottom = draw_rect.top + MMIENG_TESTPAD_KEY_HIGHT-1;
            LCD_FillRect(&lcd_dev_info, draw_rect, UITEST_COLOR_BLACK);
            
            update_rect.right = rect.right;
            update_rect.bottom = rect.bottom;
            GUILCD_InvalidateRect(GUI_MAIN_LCD_ID,update_rect, GUIREF_GetUpdateBlockSet(GUI_BLOCK_MAIN));
        }
    }
    return (0);
}
#endif

/********************************************************************************
NAME:          UITestKeyWinHandleMsg
DESCRIPTION:   
PARAM IN:      
PARAM OUT:     
AUTHOR:        allen
DATE:          2004.09.08
********************************************************************************/
PUBLIC MMI_RESULT_E UITestKeyWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    static int32 s_msg_app_ok = 0;    
    static int32 s_msg_app_cancel = 0;
    static SCI_TICK_TIME_T last_tick_time_info = {0};
    SCI_TICK_TIME_T cur_tick_time_info = {0};

#if defined KEYPAD_TYPE_QWERTY_KEYPAD || defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE || defined (PLATFORM_UWS6121E)
    static KEY_TEST_T key_test = {0};
#endif
    //SCI_TRACE_LOW:"UITestKeyWinHandleMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_2091_112_2_18_2_17_27_63,(uint8*)"d", msg_id);
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        s_msg_app_ok = 0;
        s_msg_app_cancel = 0;
#if defined KEYPAD_TYPE_QWERTY_KEYPAD || defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE || defined (PLATFORM_UWS6121E)
        InitKeyTest(&key_test);
#endif
        break;
        
    case MSG_FULL_PAINT:
#if defined KEYPAD_TYPE_QWERTY_KEYPAD || defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE || defined (PLATFORM_UWS6121E)
        DrawKeyAll(&key_test);
#elif defined KEYPAD_TYPE_TYPICAL_KEYPAD
        s_mmieng_key_ind_buf[0] = 0xFF;
        s_mmieng_key_ind_buf[1] = 0xFF;
        s_mmieng_key_ind_buf[2] = 0xFF;
        s_mmieng_key_ind_buf[3] = 0x01;//#ifdef SC6800_DVB s_mmieng_key_ind_buf[3] = 0x1f;
        s_mmieng_key_ind_buf[4] = 0x00;
        DrawTestKeyBG();
#endif
        break;
#if !defined (PLATFORM_UWS6121E)        
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_CALL2:    
    case MSG_KEYDOWN_CALL3:
    case MSG_KEYDOWN_0:
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
    case MSG_KEYDOWN_STAR:
    case MSG_KEYDOWN_HASH:
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYDOWN_RED:
    case MSG_KEYDOWN_UP:                
    case MSG_KEYDOWN_WEB:
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYDOWN_CAMERA:
    case MSG_KEYDOWN_TV:    
#endif		
	case MSG_KEYDOWN_SHORTCUT:
#if defined KEYPAD_TYPE_QWERTY_KEYPAD || defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE || defined (PLATFORM_UWS6121E)
        HandleKeyPressed(&key_test,msg_id);
        
#elif defined KEYPAD_TYPE_TYPICAL_KEYPAD
        TestKeyboard(msg_id);
#endif
        break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_KEYDOWN_OK:
#if !defined (PLATFORM_UWS6121E)
        if (0 == s_msg_app_ok)
        {
            SCI_GetTickTime(&last_tick_time_info);
            SCI_TRACE_LOW("[ENG_UITEST] MSG_KEYDOWN_OK -0- last_tick_time_info.second = %d", last_tick_time_info.second);
            s_msg_app_ok ++;
        }
        else
        {
            SCI_GetTickTime(&cur_tick_time_info);
            SCI_TRACE_LOW("[ENG_UITEST] MSG_KEYDOWN_OK %d cur_tick_time_info.second = %d", s_msg_app_ok, cur_tick_time_info.second);
            if (cur_tick_time_info.second - last_tick_time_info.second > 3)
            {
                s_msg_app_ok ++;
            }
            else
            {
                last_tick_time_info.second = cur_tick_time_info.second;
                SCI_TRACE_LOW("[ENG_UITEST] MSG_KEYDOWN_OK ast_tick_time_info.second = %d", last_tick_time_info.second);
            }
        }
        if (s_msg_app_ok > 1)

        {
            s_msg_app_ok = 0 ;      
            SaveTestResult(win_id, 1) ;
            MMIAPIENG_RunUITestNextStep();
            MMK_CloseWin(win_id) ;
        }
        else
        {
#if defined KEYPAD_TYPE_TYPICAL_KEYPAD
            TestKeyboard(msg_id);
#elif defined KEYPAD_TYPE_QWERTY_KEYPAD || defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE || defined (PLATFORM_UWS6121E)
            HandleKeyPressed(&key_test,msg_id);
#endif
        }
#else
            s_msg_app_ok = 0 ;      
            SaveTestResult(win_id, 1) ;
            MMIAPIENG_RunUITestNextStep();
            MMK_CloseWin(win_id) ;
#endif
        break;
        
    case MSG_KEYDOWN_CANCEL:
#if !defined (PLATFORM_UWS6121E)
        if (0 == s_msg_app_cancel)
        {
            SCI_GetTickTime(&last_tick_time_info);
            SCI_TRACE_LOW("[ENG_UITEST] MSG_KEYDOWN_CANCEL -0- last_tick_time_info.second = %d", last_tick_time_info.second);
            s_msg_app_cancel ++;
        }
        else
        {
            SCI_GetTickTime(&cur_tick_time_info);
            SCI_TRACE_LOW("[ENG_UITEST] MSG_KEYDOWN_CANCEL %d cur_tick_time_info.second = %d", s_msg_app_cancel, cur_tick_time_info.second);
            if (cur_tick_time_info.second - last_tick_time_info.second > 3)
            {
                s_msg_app_cancel ++;
            }
            else
            {
                last_tick_time_info.second = cur_tick_time_info.second;
                SCI_TRACE_LOW("[ENG_UITEST] MSG_KEYDOWN_CANCEL last_tick_time_info.second = %d",last_tick_time_info.second);
            }
        }
        if (s_msg_app_cancel > 1)
        {
            s_msg_app_cancel = 0;       
            SaveTestResult(win_id, 0);
            MMIAPIENG_RunUITestNextStep();
            MMK_CloseWin(win_id) ;
        }        
        else
        {
#if defined KEYPAD_TYPE_TYPICAL_KEYPAD
            TestKeyboard(msg_id);
#elif defined KEYPAD_TYPE_QWERTY_KEYPAD || defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE || defined (PLATFORM_UWS6121E)
            HandleKeyPressed(&key_test,msg_id);
#endif
        }
#else
		s_msg_app_cancel = 0;       
		SaveTestResult(win_id, 0);
		MMIAPIENG_RunUITestNextStep();
		MMK_CloseWin(win_id) ;
#endif
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#ifdef MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                s_msg_app_ok ++ ;
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                s_msg_app_cancel ++;
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif   
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CLOSE_WINDOW:
        s_msg_app_ok = 0;
        s_msg_app_cancel = 0;
#if defined KEYPAD_TYPE_QWERTY_KEYPAD || defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE || defined (PLATFORM_UWS6121E)
        FinishKeyTest(&key_test);
#endif
        break;
        
    default:
#if defined KEYPAD_TYPE_QWERTY_KEYPAD || defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE || defined (PLATFORM_UWS6121E)
        //check key down msg id 
        if(MSG_KEYDOWN_UP <= msg_id && msg_id <= MSG_KEYDOWN_MENU)
        {
//@CR NEWMS00181634
#if defined KEYPAD_TYPE_QWERTY_KEYPAD
            uint8 keycode= (uint8)(msg_id & 0x000000ff);
            keycode = MMIDEFAULT_ConvertQwertyKeyToTypicalKey(keycode);
            msg_id = (msg_id & 0xffffff00) | (keycode & 0x000000ff);
#endif
//end
            HandleKeyPressed(&key_test,msg_id);
        }
        //拦截所有的keyup消息，返回MMI_RESULT_TRUE
        else if(MSG_KEYUP_UP <= msg_id && msg_id <= MSG_KEYUP_MENU)
        {}
        else
        {
            result = MMI_RESULT_FALSE;
        }       
#endif
        break;
    }
    return (result);
}

/********************************************************************************
NAME:          DrawTestKeyBG
DESCRIPTION:   
PARAM IN:      msg_id -- 
PARAM OUT:     None
AUTHOR:        zhaohui
DATE:          2009.03.24
********************************************************************************/
#ifdef KEYPAD_TYPE_TYPICAL_KEYPAD
LOCAL void DrawTestKeyBG( void )
{
    uint16      x = 0;
    uint16      y = 0;
    wchar       disp_text[9] = {0};
    GUI_RECT_T  draw_rect = MMITHEME_GetFullScreenRect();
    int16       TESTPAD_KEY_WIDTH = (draw_rect.right - draw_rect.left - MMIENG_TESTPAD_MARGIN_X*2)>>2 ;
    const GUI_LCD_DEV_INFO  *lcd_dev_info = MMITHEME_GetDefaultLcdDev() ;
    MMI_STRING_T        string = {0};
    GUISTR_STYLE_T      text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;


#if defined (PLATFORM_SC8800H) || defined (PLATFORM_SC8800G)
#if defined (KEYPAD_TYPE_COMPACT_KEYPAD )
    char s_test_key_text[24][MMIENG_TEST_KEY_TEXT_MAX_LEN] =
    {
        {"SR"},     {"Green"},  {"Up"},     {"Down"},
        {"SL"},     {"Red"},    {"Left"},   {"Right"},
        {"Web"},    {""},      {" "},      {" "},
        {" "},{" "},{" "},{" "},
        {" "},{" "},{" "},{" "},
        {" "},{" "},{" "},{" "}
    };
    
    
#else
    char s_test_key_text[24][MMIENG_TEST_KEY_TEXT_MAX_LEN] =
    {
        {"SR"},     {"Green"},  {"Up"},     {"Down"},
        {"SL"},     {"Red"},    {"Left"},   {"Right"},
        {"1"},      {"2"},      {"3"},      {"4"},
        {"5"},      {"6"},      {"7"},      {"8"},
        {"9"},      {"0"},      {"*"},      {"#"},
        {"Web"},    {"Camera"},      {" "},      {" "}
    };
#endif

#elif defined (PLATFORM_SC6800H)
    const char s_test_key_text[24][MMIENG_TEST_KEY_TEXT_MAX_LEN] =
    {
        {"SR"},     {"Green1"},     {"Up"},     {"Down"},
        {"SL"},     {"Red"},        {"Left"},   {"Right"},
        {"1"},      {"2"},          {"3"},      {"4"},
        {"5"},      {"6"},          {"7"},      {"8"},
        {"9"},      {"0"},          {"*"},      {"#"},
        {"Web"},    {"Green2"},     {"TV"},     {" "}
    };

#elif defined (PLATFORM_SC6600L)
    char s_test_key_text[24][MMIENG_TEST_KEY_TEXT_MAX_LEN] =
    {
        {"SR"},     {"Green"},  {"Up"},     {"Down"},
        {"SL"},     {"Red"},    {"Left"},   {"Right"},
        {"1"},      {"2"},      {"3"},      {"4"},
        {"5"},      {"6"},      {"7"},      {"8"},
        {"9"},      {"0"},      {"*"},      {"#"},
        {"Web"},    {" "},      {" "},      {" "}
    };
#elif defined (PLATFORM_SC7702)
    char s_test_key_text[24][MMIENG_TEST_KEY_TEXT_MAX_LEN] =
    {
        {"SR"},     {"Green"},  {"Up"},     {"Down"},
        {"SL"},     {"Red"},    {"Left"},   {"Right"},
        {"1"},      {"2"},      {"3"},      {"4"},
        {"5"},      {"6"},      {"7"},      {"8"},
        {"9"},      {"0"},      {"*"},      {"#"},
        {"Web"},    {" "}, {" "}, {" "}
    };
#elif defined (PLATFORM_ANTISW3)
    char s_test_key_text[24][MMIENG_TEST_KEY_TEXT_MAX_LEN] =
    {
        {"SR"},     {"Green"},  {"Up"},     {"Down"},
        {"SL"},     {"Red"},    {"Left"},   {"Right"},
        {"1"},      {"2"},      {"3"},      {"4"},
        {"5"},      {"6"},      {"7"},      {"8"},
        {"9"},      {"0"},      {"*"},      {"#"},
        {"Web"},    {" "}, {" "}, {" "}
    };
#else
    char s_test_key_text[24][MMIENG_TEST_KEY_TEXT_MAX_LEN] =
    {
        {"SR"},     {"Green"},  {"Up"},     {"Down"},
        {"SL"},     {"Red"},    {"Left"},   {"Right"},
        {"1"},      {"2"},      {"3"},      {"4"},
        {"5"},      {"6"},      {"7"},      {"8"},
        {"9"},      {"0"},      {"*"},      {"#"},
        {"Web"},    {"Upside"}, {"Downside"}, {" "}
    };
#endif
    
    draw_rect.top = 0;
    LCD_FillRect(lcd_dev_info, draw_rect, UITEST_COLOR_WHITE);
    
    for (y=0;y<6;y++)
    {
        for (x=0;x<4;x++)
        {
            draw_rect.left = (uint16)MMIENG_TESTPAD_MARGIN_X + (uint16)(x * TESTPAD_KEY_WIDTH);
            draw_rect.top = MMIENG_TESTPAD_MARGIN_Y + y * MMIENG_TESTPAD_KEY_HIGHT;
            draw_rect.right = draw_rect.left + TESTPAD_KEY_WIDTH-1;
            draw_rect.bottom = draw_rect.top + MMIENG_TESTPAD_KEY_HIGHT-1;
            EditWinDrawFocusRect(draw_rect, UITEST_COLOR_BLACK);
            
            string.wstr_len = strlen(s_test_key_text[y*4+x]);
            MMI_STRNTOWSTR(disp_text,MMIENG_TEST_KEY_TEXT_MAX_LEN, (uint8*)s_test_key_text[y*4+x],string.wstr_len,string.wstr_len);
            string.wstr_ptr = disp_text;
            
            // display
            text_style.align = ALIGN_HVMIDDLE;
            text_style.font = MMI_DEFAULT_TEXT_FONT;
            text_style.font_color = MMI_BLACK_COLOR;
            //  for(;GUI_GetStringWidth(text_style.font, string.wstr_ptr,string.wstr_len)>(TESTPAD_KEY_WIDTH-1)&&(text_style.font>2);text_style.font-=2);
            while(GUI_GetStringWidth(text_style.font, string.wstr_ptr,string.wstr_len)>(TESTPAD_KEY_WIDTH-1)&&(text_style.font>2))
            {
                text_style.font-=2    ;         
            }
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)lcd_dev_info,
                (const GUI_RECT_T      *)&draw_rect,       //the fixed display area
                (const GUI_RECT_T      *)&draw_rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&string,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                ); 
        }
    }
    
}
#endif

/*****************************************************************************/
//  Description : EditWinDrawFocusRect 
//  Global resource dependence : 
//  Author: zhaohui
//  Note:   
/*****************************************************************************/
LOCAL void EditWinDrawFocusRect(
                                GUI_RECT_T rect,
                                GUI_COLOR_T color
                                )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};    
    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.top, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.left, rect.top, rect.bottom, color);
    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.bottom, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.right, rect.top, rect.bottom, color);
}

#if defined KEYPAD_TYPE_QWERTY_KEYPAD || defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE || defined PLATFORM_UWS6121E
 /*****************************************************************************/
 //  Description : InitKeyTest 
 //  Global resource dependence : 
 //  Author: ying.xu
 //  Note:   
 /*****************************************************************************/
 LOCAL int32 InitKeyTest(KEY_TEST_T *key_test_ptr)
 {    
    KEY_MAP_T *map_ptr = PNULL;
    GUI_RECT_T fullscreen = {0};
#ifdef MMI_PDA_SUPPORT
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(MMIENG_UITESTKEY_WIN_ID);
    fullscreen = both_rect.v_rect;
#else
    fullscreen = MMITHEME_GetClientRectEx((MMK_ConvertIdToHandle(MMIENG_UITESTKEY_WIN_ID)));
#endif    
    SCI_ASSERT(ARR_SIZE(s_key_string) == ARR_SIZE(s_key_map));  /*assert verified*/
    key_test_ptr->key_num = ARR_SIZE(s_key_map);
    map_ptr = (KEY_MAP_T*)SCI_ALLOC_APP(sizeof(s_key_map));
    if(PNULL == map_ptr)
    {
        return 1;
    }
    SCI_MEMCPY(map_ptr, &s_key_map, sizeof(s_key_map));
    key_test_ptr->key_map_ptr = map_ptr;  
#if defined KEYPAD_TYPE_QWERTY_KEYPAD 
#ifndef MAINLCD_DEV_SIZE_160X128
    key_test_ptr->row = 7;
    key_test_ptr->collion = 10;
#else
	key_test_ptr->row = 8;
	key_test_ptr->collion = 21; //Max 23 chars per line
#endif
#elif defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE
#haha
    key_test_ptr->row = 4;
    key_test_ptr->collion = 4;
#elif defined (PLATFORM_UWS6121E)
    key_test_ptr->row = 1;
    key_test_ptr->collion = 2;	
#endif
    key_test_ptr->key_height = (fullscreen.bottom - fullscreen.top + 1 - MMIENG_TESTPAD_MARGIN_Y*2) / key_test_ptr->row;
    key_test_ptr->key_width = (fullscreen.right - fullscreen.left  + 1 - MMIENG_TESTPAD_MARGIN_X*2) / key_test_ptr->collion;
    return 0;
 }
 /*****************************************************************************/
 //  Description : DrawKeyBackground 
 //  Global resource dependence : 
 //  Author: ying.xu
 //  Note:   
 /*****************************************************************************/
 LOCAL int32 DrawKeyBackground(KEY_TEST_T *key_test_ptr)
 {
     uint32 i = 0;
     GUI_RECT_T          draw_rect       = MMITHEME_GetClientRectEx(MMK_ConvertIdToHandle(MMIENG_UITESTKEY_WIN_ID));
     const GUI_LCD_DEV_INFO    *lcd_dev_info    = MMITHEME_GetDefaultLcdDev() ;
     MMI_STRING_T        string = {0}; 
     wchar disp_text[16] = {0};
     GUISTR_STYLE_T      text_style      = {0};
     GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
     
     LCD_FillRect(lcd_dev_info, draw_rect, UITEST_COLOR_WHITE);
     
     for (i = 0; i < key_test_ptr->key_num; i++)
     {         
         GetKeyRect(&draw_rect, key_test_ptr, i);
         EditWinDrawFocusRect(draw_rect, UITEST_COLOR_BLACK);
         
         string.wstr_len = strlen(s_key_string[i]);
         MMI_STRNTOWSTR(disp_text,MMIENG_TEST_KEY_TEXT_MAX_LEN, (uint8*)s_key_string[i],string.wstr_len,string.wstr_len);
         string.wstr_ptr = disp_text;
         
         // display
         text_style.align = ALIGN_HVMIDDLE;
         text_style.font = MMI_DEFAULT_TEXT_FONT;
         text_style.font_color = MMI_BLACK_COLOR;
         // shorten string length
         while(GUI_GetStringWidth(text_style.font, string.wstr_ptr,string.wstr_len)
             >(draw_rect.right - draw_rect.left) && (text_style.font > 2))
         {
             text_style.font -= 2    ;         
         }
         GUISTR_DrawTextToLCDInRect( 
             (const GUI_LCD_DEV_INFO *)lcd_dev_info,
             (const GUI_RECT_T      *)&draw_rect,       //the fixed display area
             (const GUI_RECT_T      *)&draw_rect,       //用户要剪切的实际区域
             (const MMI_STRING_T    *)&string,
             &text_style,
             state,
             GUISTR_TEXT_DIR_AUTO
             ); 
     }    
     return 0;
 } 
 /*****************************************************************************/
 //  Description : DrawKeyPressed 
 //  Global resource dependence : 
 //  Author: ying.xu
 //  Note:   
 /*****************************************************************************/
 LOCAL int32 DrawKeyPressed(KEY_TEST_T *key_test_ptr)
 { 
     int32 i = 0;
     for(i = 0; i< key_test_ptr->key_num; i++)
     {
        if(0 != key_test_ptr->key_map_ptr[i].pressed_flag)
        {
            DrawKeyPressedByIndex(key_test_ptr, i);
        }
     }
     return i;
 }
  /*****************************************************************************/
 //  Description : DrawKeyPressedByIndex 
 //  Global resource dependence : 
 //  Author: ying.xu
 //  Note:   
 /*****************************************************************************/
 LOCAL int32 DrawKeyPressedByIndex(KEY_TEST_T *key_test_ptr, int32 index)
 {    
    GUI_RECT_T draw_rect = {0};
    GetKeyRect(&draw_rect, key_test_ptr, index);
    LCD_FillRect(MMITHEME_GetDefaultLcdDev(), draw_rect, UITEST_COLOR_BLACK);  
    return index;
    //GUILCD_InvalidateRect(GUI_MAIN_LCD_ID,draw_rect, GUIREF_GetUpdateBlockSet(GUI_BLOCK_MAIN));
 } 
 /*****************************************************************************/
 //  Description : DrawAll ,background and pressed key
 //  Global resource dependence : 
 //  Author: ying.xu
 //  Note:   
 /*****************************************************************************/
 LOCAL int32 DrawKeyAll(KEY_TEST_T *key_test_ptr)
 {
    DrawKeyBackground(key_test_ptr);
    DrawKeyPressed(key_test_ptr);
    return 0;
 } 
 
 /*****************************************************************************/
 //  Description : HandleKeyPressed 
 //  Global resource dependence : 
 //  Author: ying.xu
 //  Note:   
 /*****************************************************************************/
 LOCAL int32 HandleKeyPressed(KEY_TEST_T *key_test_ptr, int32 msg_id)
 {
    int32 i = 0;
    for(i = 0; i< key_test_ptr->key_num; i++)
    {
        if(msg_id == key_test_ptr->key_map_ptr[i].msg_id)
        {
            key_test_ptr->key_map_ptr[i].pressed_flag = 1 ;
            DrawKeyPressedByIndex(key_test_ptr, i);
            return i ;
        }
    }
    return i;
 } 
 /*****************************************************************************/
 //  Description : FinishKeyTest 
 //  Global resource dependence : 
 //  Author: ying.xu
 //  Note:   
 /*****************************************************************************/
 LOCAL int32 FinishKeyTest(KEY_TEST_T *key_test_ptr)
  {
    SCI_FREE(key_test_ptr->key_map_ptr);
    return 0;
  } 
  
 /*****************************************************************************/
 //  Description : GetKeyRect 
 //  Global resource dependence : 
 //  Author: ying.xu
 //  Note:   
 /*****************************************************************************/
 LOCAL int32 GetKeyRect(GUI_RECT_T *rect,KEY_TEST_T *key_test_ptr, int32 index)
  {  
  GUI_RECT_T draw_rect = {0};
  GUI_RECT_T fullscreen = MMITHEME_GetClientRectEx(MMIENG_UITESTKEY_WIN_ID);
  int32 key_width = key_test_ptr->key_width ;
  int32 key_height = key_test_ptr->key_height;
  int32 key_x = key_test_ptr->key_map_ptr[index].key_position.x ;
  int32 key_y = key_test_ptr->key_map_ptr[index].key_position.y ;
  draw_rect.left = MMIENG_TESTPAD_MARGIN_X + key_y * key_width + fullscreen.left;
  draw_rect.top = MMIENG_TESTPAD_MARGIN_Y + key_x * key_height + fullscreen.top;    
  draw_rect.right = draw_rect.left +  key_test_ptr->key_map_ptr[index].width * key_width- 1;
  draw_rect.bottom = draw_rect.top + key_height - 1;
  SCI_MEMCPY(rect,&draw_rect, sizeof(GUI_RECT_T));
  return 0;
   }   
#endif

/********************************************************************************
NAME:          TestMainLCD
DESCRIPTION:   
PARAM IN:      msg_id -- 
PARAM OUT:     None
AUTHOR:        allen
DATE:          2004.09.08
********************************************************************************/
LOCAL void TestMainLCD(MMI_MESSAGE_ID_E msg_id)
{
    GUI_RECT_T draw_rect = MMITHEME_GetFullScreenRect();
    GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();

    //RGB565
    GUI_COLOR_T draw_color[3] = {0x0000, 0x0000, 0x0000};
    int i = 0, j = 0, color_idx = 0;
    BOOLEAN fill_flg = FALSE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};   

//#if defined  KEYPAD_TYPE_COMPACT_KEYPAD
    draw_rect.bottom = (int16)(draw_rect.bottom - MMITHEME_GetSoftkeyHeight());/*lint !e737*/
    full_rect.bottom = (int16)(full_rect.bottom - MMITHEME_GetSoftkeyHeight());/*lint !e737*/
//#endif
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    msg_id = ConvertKeyFromQW(msg_id);
#endif
    switch (msg_id)
    {
    case MSG_KEYDOWN_0: // draw three block with different color 
        for (i = 0; i < (full_rect.right + 1) ; i++)
        {
            draw_color[0] = i * 0x1f / (full_rect.right );
            draw_color[0] <<= 11;
            LCD_DrawVLine(
                &lcd_dev_info, 
                (uint16)i, 
                //MMIENG_TEST_MAIN_LCD_R_Y_START,
                full_rect.top,
                //MMIENG_TEST_MAIN_LCD_R_Y_END, 
                (full_rect.bottom + 1)/3,
                draw_color[0]
                );
            
            draw_color[0] = i * 0x3f / (draw_rect.right );
            draw_color[0] <<= 5;
            LCD_DrawVLine(
                &lcd_dev_info, 
                (uint16)i, 
                //MMIENG_TEST_MAIN_LCD_G_Y_START, 
                (full_rect.bottom + 1)/3 + 1,
                //MMIENG_TEST_MAIN_LCD_G_Y_END, 
                (full_rect.bottom + 1)*2/3,
                draw_color[0]
                );
            
            draw_color[0] = i * 0x1f / (draw_rect.right );
            LCD_DrawVLine(
                &lcd_dev_info, 
                (uint16)i, 
                //MMIENG_TEST_MAIN_LCD_B_Y_START, 
                (full_rect.bottom + 1)*2/3 + 1,
                //MMIENG_TEST_MAIN_LCD_B_Y_END, 
                draw_rect.bottom,
                draw_color[0]
                );
        }
        break;
        
    case MSG_KEYDOWN_1:
        draw_color[0] = UITEST_COLOR_BLACK;
        LCD_FillRect(&lcd_dev_info, full_rect, draw_color[0]);
        break;
        
    case MSG_KEYDOWN_2:
        draw_color[0] = UITEST_COLOR_WHITE;
        LCD_FillRect(&lcd_dev_info, full_rect, draw_color[0]);
        break;
        
    case MSG_KEYDOWN_3:     // draw box
        draw_color[0] = UITEST_COLOR_WHITE;
        LCD_FillRect(&lcd_dev_info, full_rect, draw_color[0]);
        draw_color[0] = UITEST_COLOR_BLACK;
        do
        {
            LCD_DrawRect(&lcd_dev_info, draw_rect, draw_color[0]);
            draw_rect.left += 2;
            draw_rect.top += 2;
            draw_rect.right -= 2;
            draw_rect.bottom -= 2;
        } while ((draw_rect.left < draw_rect.right) && (draw_rect.top < draw_rect.bottom));
        break;
        
    case MSG_KEYDOWN_4:     // fill box
        draw_color[0] = UITEST_COLOR_WHITE;
        LCD_FillRect(&lcd_dev_info, full_rect, draw_color[0]);
        draw_color[0] = UITEST_COLOR_BLACK;
        for (i = 0; i < (full_rect.bottom + 1)/8; i++)
        {
            fill_flg = !fill_flg;
            for (j = 0; j < (full_rect.right + 1)/8; j++)
            {
                if ((fill_flg && (j % 2 == 0)) || (!fill_flg && (j % 2 == 1)))
                {
                    draw_rect.left = j * 8;
                    draw_rect.top = i * 8;
                    draw_rect.right = j * 8 + 7;
                    draw_rect.bottom = i * 8 + 7;
                    LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[0]);
                }
            }   
        }
        break;
        
    case MSG_KEYDOWN_5: // draw color cavas
        draw_color[0] = UITEST_COLOR_WHITE;
        LCD_FillRect(&lcd_dev_info, full_rect, draw_color[0]);
        draw_color[0] = UITEST_COLOR_RED;
        draw_color[1] = UITEST_COLOR_GREEN;
        draw_color[2] = UITEST_COLOR_BLUE;
        for (i = 0; i < (full_rect.right + 1)/8; i++)
        {
            if (i % 2 == 0)
            {
                draw_rect.left = i * 8;
                draw_rect.top = 0;
                draw_rect.right = i * 8 + 7;
                draw_rect.bottom = full_rect.bottom ;
                LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[color_idx]);
                color_idx++;
                color_idx %= 3;
            }
        }
        color_idx = 0;
        for (j = 0; j < (full_rect.bottom + 1)/8; j++)
        {
            if (j % 2 == 1)
            {
                draw_rect.left = 0;
                draw_rect.top = j * 8;
                draw_rect.right = full_rect.right ;
                draw_rect.bottom = j * 8 + 7;
                LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[color_idx]);
                color_idx++;
                color_idx %= 3;
            }
        }
        break;
        
    case MSG_KEYDOWN_6:
        LCD_FillRect(&lcd_dev_info, full_rect, UITEST_COLOR_WHITE);
        for (i = 0; i < (draw_rect.right + 1); i++)
        {
            if (i % 2 == 0)
            {
                LCD_DrawVLine(&lcd_dev_info, (uint16)i, 0, full_rect.bottom , UITEST_COLOR_BLACK);
            }
        }
        break;
        
    case MSG_KEYDOWN_7:
        draw_color[0] = UITEST_COLOR_RED;
        LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[0]);
        break;
        
    case MSG_KEYDOWN_8:
        draw_color[0] = UITEST_COLOR_GREEN;
        LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[0]);
        break;
        
    case MSG_KEYDOWN_9:
        draw_color[0] = UITEST_COLOR_BLUE;
        LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[0]);
        break;
        
    default:
        break;
    }
    
    //update_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
    //update_rect.bottom = MMI_CLIENT_RECT_BOTTOM;
    //GUILCD_InvalidateRect(GUI_MAIN_LCD_ID,full_rect, GUIREF_GetUpdateBlockSet(GUI_BLOCK_MAIN));
}

/********************************************************************************
NAME:          TestSubLCD
DESCRIPTION:   
PARAM IN:      msg_id -- 
PARAM OUT:     None
AUTHOR:        linlin
DATE:          2005-2-2
********************************************************************************/
//LOCAL void TestSubLCD(MMI_MESSAGE_ID_E msg_id)
//{
//  GUI_RECT_T draw_rect = {0, 0, MMI_SUBSCREEN_WIDTH - 1, MMI_SUBSCREEN_HEIGHT - 1};
//  GUI_COLOR_T draw_color[3] = {0x0000, 0x0000, 0x0000};
//  uint16 i = 0, j = 0, color_idx = 0;
//  BOOLEAN fill_flg = FALSE;
//  GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
//    
//  lcd_dev_info.lcd_id = GUI_SUB_LCD_ID;
//  lcd_dev_info.block_id = GUI_BLOCK_MAIN;
//
//  
//  switch (msg_id)
//  {
//      case MSG_KEYDOWN_0: // draw three block with different color 
//          for (i = 0; i < MMI_SUBSCREEN_WIDTH; i++)
//          {         
//    
//              draw_color[0] = i / 3;
//              draw_color[0] <<= 11;
//
//              LCD_DrawVLine(&lcd_dev_info, i, 0, 20, draw_color[0]);
//              
//              draw_color[0] = (MMI_SUBSCREEN_WIDTH - i) * 2 / 3;
//              draw_color[0] <<= 5;
//              LCD_DrawVLine(&lcd_dev_info, i, 21, 40, draw_color[0]);
//              
//              if (i > 48)
//                  draw_color[0] = (96 - i) / 3;
//              else
//                  draw_color[0] = i / 3;
//              LCD_DrawVLine(&lcd_dev_info, i, 41, MMI_SUBSCREEN_HEIGHT - 1, draw_color[0]);
//          }
//          break;
//          
//      case MSG_KEYDOWN_1:
//          draw_color[0] = UITEST_COLOR_BLACK;
//          LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[0]);
//          break;
//          
//      case MSG_KEYDOWN_2:
//          draw_color[0] = UITEST_COLOR_WHITE;
//          LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[0]);
//          break;
//          
//      case MSG_KEYDOWN_3:     // draw box
//          draw_color[0] = UITEST_COLOR_WHITE;
//          LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[0]);
//          draw_color[0] = UITEST_COLOR_BLACK;
//          do
//          {
//              LCD_DrawRect(&lcd_dev_info, draw_rect, draw_color[0]);
//              draw_rect.left += 2;
//              draw_rect.top += 2;
//              draw_rect.right -= 2;
//              draw_rect.bottom -= 2;
//          } while ((draw_rect.left < draw_rect.right) && (draw_rect.top < draw_rect.bottom));
//          break;
//          
//      case MSG_KEYDOWN_4:     // fill box
//          draw_color[0] = UITEST_COLOR_WHITE;
//          LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[0]);
//          draw_color[0] = UITEST_COLOR_BLACK;
//          for (i = 0; i < 20; i++)
//          {
//              fill_flg = !fill_flg;
//              for (j = 0; j < 16; j++)
//              {
//                  if ((fill_flg && (j % 2 == 0)) || (!fill_flg && (j % 2 == 1)))
//                  {
//                      draw_rect.left = j * 8;
//                      draw_rect.top = i * 8;
//                      draw_rect.right = j * 8 + 7;
//                      draw_rect.bottom = i * 8 + 7;
//                      LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[0]);
//                  }
//              }   
//          }
//          break;
//          
//      case MSG_KEYDOWN_5: // draw color cavas
//          draw_color[0] = UITEST_COLOR_WHITE;
//          LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[0]);
//          draw_color[0] = UITEST_COLOR_RED;
//          draw_color[1] = UITEST_COLOR_GREEN;
//          draw_color[2] = UITEST_COLOR_BLUE;
//          for (i = 0; i < 16; i++)
//          {
//              if (i % 2 == 0)
//              {
//                  draw_rect.left = i * 8;
//                  draw_rect.top = 0;
//                  draw_rect.right = i * 8 + 7;
//                  draw_rect.bottom = MMI_SUBSCREEN_HEIGHT - 1;
//                  LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[color_idx]);
//                  color_idx++;
//                  color_idx %= 3;
//              }
//          }
//          color_idx = 0;
//          for (j = 0; j < 19; j++)
//          {
//              if (j % 2 == 1)
//              {
//                  draw_rect.left = 0;
//                  draw_rect.top = j * 8;
//                  draw_rect.right = MMI_SUBSCREEN_WIDTH - 1;
//                  draw_rect.bottom = j * 8 + 7;
//                  LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[color_idx]);
//                  color_idx++;
//                  color_idx %= 3;
//              }
//          }
//          break;
//          
//      case MSG_KEYDOWN_6:
//          LCD_FillRect(&lcd_dev_info, draw_rect, UITEST_COLOR_WHITE);
//          for (i = 0; i < MMI_SUBSCREEN_WIDTH; i++)
//          {
//              if (i % 2 == 0)
//              {
//                  LCD_DrawVLine(&lcd_dev_info, i, 0, MMI_SUBSCREEN_HEIGHT - 1, UITEST_COLOR_BLACK);
//              }
//          }
//          break;
//          
//      case MSG_KEYDOWN_7:
//          draw_color[0] = UITEST_COLOR_RED;
//          LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[0]);
//          break;
//          
//      case MSG_KEYDOWN_8:
//          draw_color[0] = UITEST_COLOR_GREEN;
//          LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[0]);
//          break;
//          
//      case MSG_KEYDOWN_9:
//          draw_color[0] = UITEST_COLOR_BLUE;
//          LCD_FillRect(&lcd_dev_info, draw_rect, draw_color[0]);
//          break;
//          
//      default:
//          break;
//  }
//  GUILCD_Invalidate(GUI_MAIN_LCD_ID, GUIREF_GetUpdateBlockSet(GUI_BLOCK_MAIN));
//}

/********************************************************************************
NAME:          UITestMainLCDWinHandleMsg
DESCRIPTION:   
PARAM IN:      
PARAM OUT:     
AUTHOR:        allen
DATE:          2004.09.08
********************************************************************************/
PUBLIC MMI_RESULT_E UITestMainLCDWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
#ifdef TOUCH_PANEL_SUPPORT
    static int32 tp_up_count = 0;
#endif
    static MMI_MESSAGE_ID_E last_draw_msg_id = 0;
#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
    msg_id = ConvertKeyFromQW(msg_id);
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef TOUCH_PANEL_SUPPORT
        tp_up_count = 0;
#endif
        TestMainLCD(MSG_KEYDOWN_0);
        last_draw_msg_id= MSG_KEYDOWN_0;
        break;
        
    case MSG_FULL_PAINT:
        break;
    case MSG_GET_FOCUS:
        TestMainLCD(last_draw_msg_id);
        break;
        
    case MSG_KEYDOWN_0:
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
        TestMainLCD(msg_id);
        last_draw_msg_id = msg_id;
        break;
        

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:            
        tp_up_count++;
        if(10 == tp_up_count)
        {
            tp_up_count = 0;
            MMK_SendMsg(win_id, MSG_KEYDOWN_0 , 0);
        }
        else
        {
            MMK_SendMsg(win_id, MSG_KEYDOWN_1 + tp_up_count - 1, 0);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_KEYDOWN_OK:            
        SaveTestResult(win_id, 1);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin( win_id );  
        break;
        
    case MSG_KEYDOWN_CANCEL:
        SaveTestResult(win_id, 0);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin( win_id ) ;            
        break;

#ifdef FLIP_PHONE_SUPPORT        
    case MSG_KEYDOWN_FLIP:      // close flip
#endif    
    case MSG_KEYDOWN_RED:
        break;
        
    default:            
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
NAME:          UITestSubLCDWinHandleMsg
DESCRIPTION:   
PARAM IN:      
PARAM OUT:     
AUTHOR:        linlin
DATE:          2005-2-2
********************************************************************************/
//PUBLIC MMI_RESULT_E UITestSubLCDWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
//{
//  MMI_RESULT_E result = MMI_RESULT_TRUE;
//
//  switch (msg_id) 
//  {
//      case MSG_OPEN_WINDOW:
//            break;
//
//        case MSG_FULL_PAINT:
//            //小屏是否允许刷新
//            MMISUB_IsPermitUpdate(FALSE);
//          TestSubLCD(MSG_KEYDOWN_0);
//          break;
//  
//      case MSG_KEYDOWN_0:
//      case MSG_KEYDOWN_1:
//      case MSG_KEYDOWN_2:
//      case MSG_KEYDOWN_3:
//      case MSG_KEYDOWN_4:
//      case MSG_KEYDOWN_5:
//      case MSG_KEYDOWN_6:
//      case MSG_KEYDOWN_7:
//      case MSG_KEYDOWN_8:
//      case MSG_KEYDOWN_9:
//          TestSubLCD(msg_id);
//          break;
//          
//      case MSG_KEYDOWN_UP:
//#ifdef SC6800_DVB
//        case MSG_KEYDOWN_GREEN:
//#endif
//           
//          MMIAPIENG_RunUITestNextStep();
//          break;
//          
//      case MSG_KEYDOWN_OK:
//          break;
//          
//      case MSG_KEYDOWN_CANCEL:
//          break;
//          
//      case MSG_KEYDOWN_FLIP:      // close flip
//      case MSG_KEYDOWN_RED:
//          break;
//      
// 
//      case MSG_CTL_CANCEL:
//          break;
//      
//
//      case MSG_CTL_OK:
//          break;
//      
//      case MSG_LOSE_FOCUS:
//            //小屏是否允许刷新
//            MMISUB_IsPermitUpdate(TRUE);
//          break;
//          
//      case MSG_GET_FOCUS:
//          break;
//          
//      case MSG_CLOSE_WINDOW:
//            //小屏是否允许刷新
//            MMISUB_IsPermitUpdate(TRUE);
//          break;
//      
//      default:
//          result = MMI_RESULT_FALSE;
//          break;
//  }
//  return (result);
//}

/********************************************************************************
NAME:          UITestRtcWinHandleMsg
DESCRIPTION:   
PARAM IN:      
PARAM OUT:     
AUTHOR:        allen
DATE:          2004.09.08
********************************************************************************/
PUBLIC MMI_RESULT_E UITestRtcWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_STRING_T timer_text = {0};
    uint8 timer_buf[5] = {0};
//    wchar timer_wstr[5] = {0};
    
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
    GUI_RECT_T		lab_rect = {0};
    
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:            
        lab_rect = client_rect;
        lab_rect.bottom =   (client_rect.bottom + client_rect.top)/2;
        GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);
        
        // lab_rect.bottom = client_rect.top + (client_rect.bottom - client_rect.top)*4/5;
        lab_rect.top =  lab_rect.bottom + 1;
        lab_rect.bottom = client_rect.bottom ;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);  
        
        s_mmieng_rtc_counter = 0;
        if (s_mmieng_testrtc_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_testrtc_timer_id);
            s_mmieng_testrtc_timer_id = 0;
        }
        s_mmieng_testrtc_timer_id = MMK_CreateWinTimer(MMIENG_UITESTRTC_WIN_ID, 1000, TRUE);
        timer_text = s_mmieng_str_up_to_next;
        
        //GUILABEL_SetBackgroundColor(MMIENG_LABEL1_CTRL_ID, mmi_uitest_display.bkgd_color);
        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &timer_text, FALSE);
        GUILABEL_SetFont(MMIENG_LABEL1_CTRL_ID,SONG_FONT_22,MMI_BLACK_COLOR);
        sprintf((char*)timer_buf, "%04d", s_mmieng_rtc_counter);
        //          timer_text.is_ucs2 = FALSE;
        SetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, (char*)timer_buf, 4, FALSE);
        GUILABEL_SetFont(MMIENG_UITEST_LABEL_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
//        timer_text.wstr_len = 4;
//        timer_text.wstr_ptr = timer_wstr;
//        MMI_STRNTOWSTR( timer_wstr, timer_text.wstr_len, timer_buf, timer_text.wstr_len, timer_text.wstr_len );
//        GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &timer_text, FALSE);
        break;
#ifndef MMI_RES_LOW_COST
    case MSG_LCD_SWITCH:
        lab_rect = client_rect;
        lab_rect.bottom =   (client_rect.bottom + client_rect.top)/2;
        GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);
        
        lab_rect.top =  lab_rect.bottom + 1;
        lab_rect.bottom = client_rect.bottom ;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);      
        break;
#endif
    case MSG_FULL_PAINT:
        //LCD_FillRect(&lcd_dev_info,client_rect,mmi_uitest_display.bkgd_color);      
        GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  IMAGE_COMMON_BG, &lcd_dev_info);
        break;
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif   
    case MSG_CTL_OK:
    case MSG_KEYDOWN_OK:  
        if (s_mmieng_testrtc_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_testrtc_timer_id);
            s_mmieng_testrtc_timer_id = 0;
        }
        SaveTestResult(win_id, 1);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(win_id);           
        break;          
        
    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        if (s_mmieng_testrtc_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_testrtc_timer_id);
            s_mmieng_testrtc_timer_id = 0;
        }
        SaveTestResult(win_id, 0);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT        
    case MSG_KEYDOWN_FLIP:      // close flip
#endif    
    case MSG_KEYDOWN_RED:
        break;  
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif   
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_TIMER:
        if (*(uint8*)param == s_mmieng_testrtc_timer_id) 
        {
            s_mmieng_rtc_counter++;
            if (s_mmieng_rtc_counter > 3)
            	{
                s_mmieng_rtc_counter = 0;
                MMK_StopTimer(s_mmieng_testrtc_timer_id);
                s_mmieng_testrtc_timer_id = 0;
            	}
            sprintf((char*)timer_buf, "%04d", s_mmieng_rtc_counter);
            SetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, (char*)timer_buf, 4, TRUE);
//            timer_text.wstr_len = 4;
//            timer_text.wstr_ptr = timer_wstr;
//            MMI_STRNTOWSTR( timer_wstr, timer_text.wstr_len, timer_buf, timer_text.wstr_len, timer_text.wstr_len );
//            GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &timer_text, TRUE);
        }
        else
            result = MMI_RESULT_FALSE;      // need additional process
        break;  
        
        
    case MSG_CLOSE_WINDOW:
        if (s_mmieng_testrtc_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_testrtc_timer_id);
            s_mmieng_testrtc_timer_id = 0;
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
NAME:          UITestVibrateWinHandleMsg
DESCRIPTION:   
PARAM IN:      
PARAM OUT:     
AUTHOR:        allen
DATE:          2004.09.08
********************************************************************************/
PUBLIC MMI_RESULT_E UITestVibrateWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:             
        
        MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_OTHER);
        break; 
        
    case MSG_FULL_PAINT:
        //LCD_FillRect(&lcd_dev_info,client_rect,mmi_uitest_display.bkgd_color);
        //GUILABEL_SetBackgroundColor(MMIENG_UITEST_LABEL_CTRL_ID, mmi_uitest_display.bkgd_color);
        {
            GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
            GUI_RECT_T          client_rect = MMITHEME_GetClientRectEx(win_id);
            GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  IMAGE_COMMON_BG, &lcd_dev_info);
            text = s_mmieng_str_up_to_next;
            GUILABEL_SetFont(MMIENG_UITEST_LABEL_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
            GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, FALSE);
        }
        break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_KEYDOWN_OK:            
    case MSG_CTL_OK:
        MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_OTHER);
        SaveTestResult(win_id, 1);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(win_id);
        break;

    case MSG_LOSE_FOCUS:
        MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_OTHER);
        break;

    case MSG_GET_FOCUS:
        MMIAPISET_SetVibrator(TRUE, MMISET_RING_TYPE_OTHER);
        break;

    case MSG_APP_CANCEL :
    case MSG_CTL_CANCEL:
        MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_OTHER);
        SaveTestResult(win_id, 0);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin( win_id ) ;
        break;
        
    case MSG_KEYDOWN_RED:
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif   
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CLOSE_WINDOW:
        MMIAPISET_SetVibrator(FALSE, MMISET_RING_TYPE_OTHER);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
NAME:          SetBacklight
DESCRIPTION:   
PARAM IN:      
PARAM OUT:     
AUTHOR:        allen
DATE:          2004.10.25
********************************************************************************/
LOCAL void SetBacklight(BOOLEAN is_open)
{    
    if (is_open)
    {
        MMIDEFAULT_TurnOnBackLight();
        
    }
    else
    {
        MMIDEFAULT_TurnOffBackLight();
        
#ifndef WIN32   
        MMIDEFAULT_StopKeybadBackLightTimer();
        MMIDEFAULT_SetKeybadBackLight(FALSE);
#endif
        
    }
}

/********************************************************************************
NAME:          UITestBacklightWinHandleMsg
DESCRIPTION:   
PARAM IN:      
PARAM OUT:     
AUTHOR:        allen
DATE:          2004.09.08
********************************************************************************/
PUBLIC MMI_RESULT_E UITestBacklightWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        
        text = s_mmieng_str_up_to_next;
        GUILABEL_SetFont(MMIENG_UITEST_LABEL_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
        GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, FALSE);
#ifndef WIN32
        SCI_MMI_EnableDeepSleep(FALSE);//bug1954557
#endif
        s_mmieng_test_backlight_on = TRUE;
        SetBacklight(TRUE);
        
        if (!MMIAPITVOUT_GetStartingStatus())
        {
            if (s_mmieng_testbacklight_timer_id != 0)
            {
                MMK_StopTimer(s_mmieng_testbacklight_timer_id);
                s_mmieng_testbacklight_timer_id = 0;
            }
            s_mmieng_testbacklight_timer_id = MMK_CreateWinTimer(MMIENG_UITESTBACKLIGHT_WIN_ID, 
                2000,
                TRUE
                );
        }
        break;            
        
    case MSG_FULL_PAINT:
        {
            GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
            GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  
                IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
        }
        break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_KEYDOWN_OK:    
    case MSG_CTL_OK:
        if (s_mmieng_testbacklight_timer_id > 0)
        {
            MMK_StopTimer(s_mmieng_testbacklight_timer_id);
            s_mmieng_testbacklight_timer_id = 0;
        }
        if (!s_mmieng_test_backlight_on)
        {
            s_mmieng_test_backlight_on = TRUE;
            SetBacklight(TRUE);
        }
        SaveTestResult(win_id, 1);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(win_id);
        break;          
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:
        if (s_mmieng_testbacklight_timer_id > 0)
        {
            MMK_StopTimer(s_mmieng_testbacklight_timer_id);
            s_mmieng_testbacklight_timer_id = 0;
        }
        
        if (!s_mmieng_test_backlight_on)
        {
            s_mmieng_test_backlight_on = TRUE;
            SetBacklight(TRUE);
        }
        
        SaveTestResult(win_id, 0);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(win_id);
        break;          
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
        {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
        }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
        {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
        }
#endif   
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_TIMER:
        if (*(uint8*)param == s_mmieng_testbacklight_timer_id) 
        {
            s_mmieng_test_backlight_on = !s_mmieng_test_backlight_on;
            SetBacklight(s_mmieng_test_backlight_on);
        }
        else
            result = MMI_RESULT_FALSE;      // need additional process
        break;
        
    case MSG_LOSE_FOCUS:            
        if (s_mmieng_testbacklight_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_testbacklight_timer_id);
            s_mmieng_testbacklight_timer_id = 0;
        } 
        SetBacklight(TRUE);
        
        break;
        
    case MSG_GET_FOCUS:           
        if (!MMIAPITVOUT_GetStartingStatus())
        {
            if (s_mmieng_testbacklight_timer_id != 0)
            {
                MMK_StopTimer(s_mmieng_testbacklight_timer_id);
                s_mmieng_testbacklight_timer_id = 0;
            }                
            s_mmieng_testbacklight_timer_id = MMK_CreateWinTimer(MMIENG_UITESTBACKLIGHT_WIN_ID, 
                1000,
                TRUE
                );
        }           
        SetBacklight(s_mmieng_test_backlight_on);
        break;
        
        
        
    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_CLOSE_WINDOW:
        if (s_mmieng_testbacklight_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_testbacklight_timer_id);
            s_mmieng_testbacklight_timer_id = 0;
        } 
        if (!s_mmieng_test_backlight_on)
        {
            s_mmieng_test_backlight_on = TRUE;
            SetBacklight(TRUE);
        }
#ifndef WIN32
        SCI_MMI_EnableDeepSleep(TRUE);//bug1954557
#endif
        break;          
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

#ifdef  CAMERA_SUPPORT
/********************************************************************************
NAME:          get sensor id
DESCRIPTION:   
PARAM IN:      
PARAM OUT:     
AUTHOR:        robert.wang
********************************************************************************/
LOCAL int8 GetSensorID(void)
{
    SENSOR_ID_E sensor_id = 0;
    uint32 sensor_param = 0;

    DCAMERA_GetInfo(DCAMERA_INFO_CUR_IMG_SENSOR, &sensor_param);
    sensor_id = sensor_param;

    //SCI_TRACE_LOW:"MMIENG DC GetSensorID sensor_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_3484_112_2_18_2_17_30_64,(uint8*)"d", sensor_id);
    return sensor_id;
}

/********************************************************************************/
// NAME:          get sensor number
// DESCRIPTION:
// PARAM IN:
// PARAM OUT:
// AUTHOR:        robert.wang
/********************************************************************************/
LOCAL int16 GetSensorNumber(void)
{
    int16 sensor_number = 0;
    uint32 sensor_param = 0;

    DCAMERA_GetInfo(DCAMERA_INFO_IMG_SENSOR_NUM, &sensor_param);
    sensor_number = sensor_param;

    //SCI_TRACE_LOW:"MMIENG DC GetSensorNumber sensor_number=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_3503_112_2_18_2_17_30_65,(uint8*)"d",sensor_number);

    return sensor_number;
}

/********************************************************************************/
// NAME:          UITestCameraWinHandleMsg
// DESCRIPTION:
// PARAM IN:
// PARAM OUT:
// AUTHOR:        allen
// DATE:          2004.09.08
/********************************************************************************/
PUBLIC MMI_RESULT_E UITestCameraWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    
     MMI_RESULT_E result = MMI_RESULT_TRUE;
//       MMIENG_TEST_CAMERA_WIN_PARAM_T camera_param = {MMIENG_TEST_CAMERA_WIN_MIN, 
//        0,
//        FS_HS,
//        FALSE};
    
#ifndef _WIN32 
    switch(msg_id)
    {
        //added by fhp for test 2006.11.23
    case MSG_APP_WEB:
        break;
        //end of added by fhp for test 2006.11.23
    case MSG_KEYDOWN_RED:
        break;
    default:
        result = MMIENG_ForEngWinMsg(win_id, msg_id, param);
        break;         
    }
#endif
    
    return result;
}

/*****************************************************************************/
//  Description : handle the message of dc window
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_IsDCEngMode(void)
{
    SCI_TRACE_LOW("[MMIDC] MMIENG_IsDCEngMode s_is_in_eng_mode=%d", s_is_in_eng_mode);
    return s_is_in_eng_mode;
}

/*****************************************************************************/
//  Description : handle the message of dc window
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E ENG_DC_Preview(int16 sensor_id)
{
    DCAMERA_RETURN_VALUE_E  ret = DCAMERA_OP_SUCCESS;
    DCAMERA_PREVIEW_PARAM_T preview_param = {0};/*lint !e64*/
    LCD_INFO_T lcd_info = {0};
    GUI_LCD_DEV_INFO dev_info_ptr = {0};
    GUI_RECT_T       rect = {0};    
    MMI_STRING_T     text_str = {0};

    GUISTR_STYLE_T   text_style = {0};/*lint !e64*/
    GUISTR_STATE_T   state = GUISTR_STATE_BOLD;

    GUI_COLOR_T      color = MMI_BLACK_COLOR;

    uint32 sensor_number = 0;
    GUI_RECT_T  str_rect = {0};
    wchar  sensor_wchar[] = {'S', '1', '/', '1',0};
    int16 cur_sensor_id = 0;

    // text style
    text_style.char_space = 1;
    text_style.line_space = 0;
    text_style.effect = FONT_EFFECT_NONE;
    text_style.font = SONG_FONT_16;
    text_style.font_color = MMI_GRAY_WHITE_COLOR;

    GUILCD_GetInfo(0, &lcd_info); /*lint !e64*/       

    
    sensor_number = GetSensorNumber();
    if (2 <= sensor_number)
    {
        cur_sensor_id = sensor_id;
    }
    else
    {
       cur_sensor_id = GetSensorID();
    }
    
    sensor_wchar[1] = sensor_wchar[1] + cur_sensor_id;
    sensor_wchar[3] = sensor_wchar[3] + sensor_number - 1;
    
    DCAMERA_StopPreview();

    //填充透明色
    rect = MMITHEME_GetFullScreenRect();
    GUI_FillRect(&dev_info_ptr, rect, color);
    MMITHEME_UpdateRect();
    
    str_rect.left = 10;
    str_rect.right = str_rect.left + 50 - 1;
    str_rect.top = 5;
    str_rect.bottom = str_rect.top + 20 - 1;
    
    text_str.wstr_ptr = sensor_wchar;
    text_str.wstr_len = MMIAPICOM_Wstrlen(sensor_wchar);

    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&dev_info_ptr,
        (const GUI_RECT_T      *)&str_rect,       //the fixed display area
        (const GUI_RECT_T      *)&str_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&text_str,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    MMITHEME_UpdateRect();

    if (2 <= sensor_number)
    {
        ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_IMG_SENSOR_MODE, sensor_id, PNULL);
        //SCI_TRACE_LOW:"[MMIDC] ENG_DC_Preview SENSOR ret=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_3628_112_2_18_2_17_31_66,(uint8*)"d", ret);
    }

    //added, @robert.wang, 09-9-24, cr153905
    //需要设置默认参数，否则会画面暗
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_BRIGHTNESS, DCAMERA_EV_03,PNULL);
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_CONTRAST, DCAMERA_EV_03,PNULL);
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_EFFECT, DCAMERA_EFFECT_NORMAL,PNULL);
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_EXPOSURE_COMPENSATION, DCAMERA_EV_03,PNULL); 
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_FLICKER_COMPENSATION, DCAMERA_EV_00,PNULL);
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_PREVIEW_MODE, DCAMERA_ENVIRONMENT_NORMAL,PNULL);
    if(TEST_PATTERN_MODE)
    {
        ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_TESTPATTERN_MODE, DCAMERA_ENVIRONMENT_TEST_PATTERN, PNULL);
    }
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_WB_MODE, DCAMERA_WB_MODE_AUTO,PNULL);
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_FLASH, DCAMERA_FLASH_ON,PNULL);//bug746948, 工程模式测试时打开闪光灯常亮
    //end cr153905
    
    preview_param.lcd_id        = GUI_MAIN_LCD_ID;     /*lint !e64*/   
    preview_param.disp_rect.w   = lcd_info.lcd_width;
    preview_param.disp_rect.h   = lcd_info.lcd_height;
    preview_param.disp_mode = DCAMERA_DISP_FULLSCREEN;
    preview_param.rotation_mode = DCAMERA_ROTATION_0;
    
    preview_param.disp_rect.x   = 0;
    preview_param.disp_rect.y   = 0;
    preview_param.lcd_block_id  = GUI_BLOCK_0;
    preview_param.target_rect = preview_param.disp_rect;

    ret = DCAMERA_StartPreview(&preview_param);
    
    //SCI_TRACE_LOW:"[MMIDC] ENG_DC_Preview StartPreview ret=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_3661_112_2_18_2_17_31_67,(uint8*)"d", ret);
    if(DCAMERA_OP_SUCCESS != ret)
    {
        
        DCAMERA_Close();
        rect = MMITHEME_GetFullScreenRect();
        GUI_FillRect(&dev_info_ptr, rect, MMI_WINDOW_BACKGROUND_COLOR);
        MMI_GetLabelTextByLang(TXT_DC_OPEN_FAIL , &text_str);
        rect.top = rect.bottom / 4;
        rect.left = (rect.right - GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, SONG_FONT_16, 1)) / 2;
        
      
        // display
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&dev_info_ptr,
            (const GUI_RECT_T      *)&rect,       //the fixed display area
            (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&text_str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
        
        MMITHEME_UpdateRect();
    }  

    return ret;
}

/*****************************************************************************/
//  Description : handle the message of dc window
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIENG_ForEngWinMsg(
                                        MMI_WIN_ID_T      win_id, 
                                        MMI_MESSAGE_ID_E  msg_id, 
                                        DPARAM                param   
                                        )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    DCAMERA_RETURN_VALUE_E  ret = DCAMERA_OP_SUCCESS;
//     DCAMERA_PREVIEW_PARAM_T preview_param = {0};/*lint !e64*/
    GUI_LCD_DEV_INFO dev_info_ptr = {0};
    GUI_RECT_T       rect = {0};    
    MMI_STRING_T     text_str = {0};
    GUISTR_STYLE_T   text_style = {0};/*lint !e64*/
    GUISTR_STATE_T   state = GUISTR_STATE_BOLD;
    static MMISRV_HANDLE_T audio_handle = 0;
    GUI_COLOR_T      color = MMI_BLACK_COLOR;

    uint32 sensor_number = 0;
#ifdef TOUCH_PANEL_SUPPORT    
    static GUI_POINT_T down_tp = {0};
#endif    
    //SCI_TRACE_LOW:"[MMIDC]: MMIAPIDC_ForEngWinMsg, msg_id = 0x%.2x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_3714_112_2_18_2_17_31_68,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:    
    case MSG_GET_FOCUS:   
        
        if (MSG_GET_FOCUS == msg_id
            && !MMIENG_IsDCEngMode())  // add for cr246017
        {
            SaveTestResult( win_id, 1);
            MMIAPIENG_RunUITestNextStep();
            MMK_CloseWin( win_id ) ;
            break;
        }
        //QQ运行后，内存不足，进入DC要提示QQ退出
#ifdef QQ_SUPPORT
        if (QQ_IsRunning())
        {
            MMIPUB_OpenAlertWarningWin( TXT_EXIT_QQ);
          //  MMK_CloseWin(win_id);// masked for cr246017
            break;
        }
#endif
        
        //JAVA 后台运行,内存不足，提示退出
#ifdef JAVA_SUPPORT
        if (MMIAPIJAVA_IsJavaRuning())
        {
            MMIPUB_OpenAlertWarningWin( TXT_EXIT_BACKGROUND_JAVA);
        //    MMK_CloseWin(win_id); // masked for cr246017
            break;
        }
#endif
        
        
        //MMI_Enable3DMMI(FALSE);
        s_is_in_eng_mode = TRUE;
        // MMITHEME_InitMainLcdStortUpdateRect();
        MMIDEFAULT_TurnOnBackLight();  
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);     
        
        
#ifdef WIN32
        GUIBLOCK_SetType(0, color, GUIBLOCK_TYPE_OSD, &dev_info_ptr);
#else
        GUIBLOCK_SetType(255, color, GUIBLOCK_TYPE_COMMON, &dev_info_ptr);
#endif

        MMITHEME_SetUpdateDelayCount(0);

        // text style
        text_style.char_space = 1;
        text_style.line_space = 0;
        text_style.effect = FONT_EFFECT_NONE;
        text_style.font = SONG_FONT_16;
        text_style.font_color = MMI_GRAY_WHITE_COLOR;
        audio_handle = MMIENG_RequestVirtualAudService();
        ret = DCAMERA_Open(DCAMERA_OP_NORMAL);
        if(DCAMERA_OP_SUCCESS != ret)
        {
            rect = MMITHEME_GetFullScreenRect();
            GUI_FillRect(&dev_info_ptr, rect, MMI_WINDOW_BACKGROUND_COLOR);
            MMI_GetLabelTextByLang(TXT_DC_OPEN_FAIL , &text_str);
            rect.top = rect.bottom / 4;
            rect.left = (rect.right - GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, SONG_FONT_16, 1)) / 2;
            
            // display
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&dev_info_ptr,
                (const GUI_RECT_T      *)&rect,       //the fixed display area
                (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&text_str,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
            
            MMITHEME_UpdateRect();
            break;
        }

        ENG_DC_Preview(SENSOR_MAIN);
        break;
    case MSG_APP_LEFT:
    case MSG_APP_UP:
        if (SENSOR_MAIN != GetSensorID())
        {
            ENG_DC_Preview(SENSOR_MAIN);
        }
        break;
    case MSG_APP_RIGHT:
    case MSG_APP_DOWN:
         sensor_number = GetSensorNumber();
         if (2 <= sensor_number)
         {
            if (SENSOR_SUB != GetSensorID())
            {
                ENG_DC_Preview(SENSOR_SUB);
            }
         }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        down_tp.x = MMK_GET_TP_X(param);
        down_tp.y = MMK_GET_TP_Y(param);
        break;

    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T up_tp = {0};

            sensor_number = GetSensorNumber();
            up_tp.x = MMK_GET_TP_X(param);
            up_tp.y = MMK_GET_TP_Y(param);

            if (2 <= sensor_number)
            {
                if (up_tp.x > down_tp.x)
                {
                    MMK_PostMsg(win_id,MSG_APP_RIGHT, PNULL, 0);
                }
                else if (up_tp.x < down_tp.x)
                {
                   MMK_PostMsg(win_id,MSG_APP_LEFT, PNULL, 0);
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:
        s_is_in_eng_mode = FALSE;
        DCAMERA_StopPreview();
        DCAMERA_Close();
        
        MMISRVMGR_Free(audio_handle);
        audio_handle = 0;
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        //Resume3DMMI();
        //MMI_Enable3DMMI(TRUE);
#ifdef UI_P3D_SUPPORT
        if (MSG_CLOSE_WINDOW == msg_id)
        {
            MMI_Enable3DEffect(MMI_3D_EFFECT_CAMERA);
        }
#endif 
        
        rect = MMITHEME_GetFullScreenRect();
        GUIRES_DisplayImg(PNULL,
            &rect,
            &rect,
            win_id,
            IMAGE_COMMON_BG,
            &dev_info_ptr);
        break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:  // add for cr246017
        s_is_in_eng_mode = FALSE;
        
        DCAMERA_StopPreview();
        DCAMERA_Close();
        
        MMISRVMGR_Free(audio_handle);
        audio_handle = 0;
        
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        SaveTestResult( win_id, 1);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin( win_id ) ;
        break;
        
    case MSG_KEYDOWN_CANCEL :    
    case MSG_CTL_CANCEL:    // add for cr246017
        s_is_in_eng_mode = FALSE;
        DCAMERA_StopPreview();
        DCAMERA_Close();
        
        MMISRVMGR_Free(audio_handle);
        audio_handle = 0;
        
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        SaveTestResult( win_id, 0);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin( win_id ) ;
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

#endif
/********************************************************************************/
// NAME:          UITestVideoWinHandleMsg
// DESCRIPTION:   video dec/enc test
// PARAM IN:
// PARAM OUT:
// AUTHOR:        zhichao.zhang
// DATE:          2021.10.12
/********************************************************************************/
PUBLIC MMI_RESULT_E UITestVideoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

#ifndef _WIN32
    switch(msg_id)
    {
    case MSG_APP_WEB:
        break;
    case MSG_KEYDOWN_RED:
        break;
    default:
        result = MMIENG_VideoForEngWinMsg(win_id, msg_id, param);
        break;
    }
#endif

    return result;
}
extern PUBLIC DCAMERA_RETURN_VALUE_E ENG_DC_Preview(int16 sensor_id);
extern PUBLIC MMISRV_HANDLE_T MMIENG_RequestVirtualAudService(void);

LOCAL uint32 frm_len_per_sec = 0;
LOCAL uint32 debug_time_start = 0;
LOCAL SFS_HANDLE yuv_file_handle = 0;
LOCAL SFS_HANDLE test_strm_file_handle = 0;
LOCAL uint8* s_p_test_strm_buf = PNULL;
LOCAL uint32 s_i_test_strm_size = 0;
LOCAL BOOLEAN bH264DecRefreseLCD = SCI_FALSE;
LOCAL BOOLEAN bImgShowing = SCI_FALSE;

LOCAL void enc_callback(uint8* p_buffer, uint32 len, uint32 type, uint32 timestamp) {
    // calculate bit rate
    frm_len_per_sec += len;
    if (SCI_GetTickCount() - debug_time_start >= 1000) {
        uint32 bps = 0;
        debug_time_start = SCI_GetTickCount();
        bps = (frm_len_per_sec *8);
        frm_len_per_sec = 0;
    }
#if _MSC_VER <= 1600
    VT_Send_H264_Decoder(p_buffer, len, SCI_GetTickCount());
#endif
}

#if defined (PLATFORM_ANTISW3)
LOCAL void dec_callback(VT_YUV_FRAME_T* p_yuv_frm, uint32 timestamp) {
	    VT_DISP_PARAM disp_param = {0};
    if (yuv_file_handle != NULL) {
        uint32 bytes_written = 0;
        SFS_WriteFile(yuv_file_handle,
        p_yuv_frm->yaddr,
        p_yuv_frm->width*p_yuv_frm->height*3/2,
        &bytes_written,PNULL);
    }


    disp_param.crop_flag = SCI_FALSE;
    disp_param.crop_param.x = 40;
    disp_param.crop_param.y = 40;
    disp_param.crop_param.width = 240;
    disp_param.crop_param.height = 240;
    
    if (bH264DecRefreseLCD) {
#if _MSC_VER <= 1600
        VT_Display_YUV(p_yuv_frm, PNULL, SCI_TRUE);
#endif
    } else {
#if _MSC_VER <= 1600
    VT_Display_YUV(p_yuv_frm, PNULL, SCI_FALSE);
#endif
    }
}
#elif defined (PLATFORM_UWS6121E)
LOCAL void dec_callback(void* p_yuv_frm, uint32 timestamp) {

}
#endif

LOCAL void start_encoder() {
#if defined (PLATFORM_ANTISW3)

    VT_ENC_PARAM_T vt_enc_param = {0};
    vt_enc_param.vt_preview_param.disp_rect.x = 0;
    vt_enc_param.vt_preview_param.disp_rect.y = 0;
    vt_enc_param.vt_preview_param.disp_rect.dx = 60;
    vt_enc_param.vt_preview_param.disp_rect.dy = 60;
    vt_enc_param.vt_preview_param.target_rect.x = 0;
    vt_enc_param.vt_preview_param.target_rect.y = 0;
    vt_enc_param.vt_preview_param.target_rect.dx = 48;
    vt_enc_param.vt_preview_param.target_rect.dy = 48;
    vt_enc_param.vt_preview_param.b_preview_displayed = SCI_TRUE;
    vt_enc_param.vt_preview_param.cam_sensor_id = DCAMERA_IMG_SENSOR_MAIN;

    vt_enc_param.vt_h264_enc_param.i_width = 320;
    vt_enc_param.vt_h264_enc_param.i_height= 320;
    vt_enc_param.vt_h264_enc_param.i_frame_rate = 15;
    vt_enc_param.vt_h264_enc_param.i_frame_rate_base = 1;
    vt_enc_param.vt_h264_enc_param.v_quality_level = 1;
    vt_enc_param.vt_h264_enc_param.i_key_frame_rate = 30;
    vt_enc_param.vt_h264_enc_param.i_task_priority = 67;
    vt_enc_param.vt_h264_enc_param.i_target_bitrate = 300*1000;
    vt_enc_param.vt_h264_enc_param.i_task_stack = 100*1024;
    vt_enc_param.vt_h264_enc_param.callback = enc_callback;
    vt_enc_param.vt_h264_enc_param.b_dump_yuv = SCI_FALSE;
    vt_enc_param.vt_h264_enc_param.b_dump_strm = SCI_FALSE;
#if _MSC_VER <= 1600
    VT_Init_H264_Encoder(&vt_enc_param);
	VT_Start_H264_Encoder();
#endif    
#endif
}
#define  AUDIO_CHANNEL                        1
#define  AUDIO_SAMPLE_RATE                    8000

LOCAL void start_decoder() {
#if defined (PLATFORM_ANTISW3)

    VT_H264_DEC_PARAM_T dec_param;
    dec_param.b_dump_strm = SCI_FALSE;
    dec_param.b_dump_yuv = SCI_FALSE;
    dec_param.i_task_priority = 69;
    dec_param.callback = dec_callback;
#if _MSC_VER <= 1600
    VT_Init_H264_Decoder(&dec_param);
#endif
#endif

}
PUBLIC MMI_RESULT_E MMIENG_VideoForEngWinMsg(
                                        MMI_WIN_ID_T      win_id, 
                                        MMI_MESSAGE_ID_E  msg_id, 
                                        DPARAM                param   
                                        )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUI_COLOR_T      color = MMI_BLACK_COLOR;
    GUI_LCD_DEV_INFO dev_info = {0};
    static MMISRV_HANDLE_T audio_handle = 0;
    GUI_RECT_T       rect = {0,0,239,239};
    SCI_TRACE_LOW("[MMISAMPLE] HandleAppSampleMainWinMsg msg_id:%d", msg_id);
#if defined (PLATFORM_ANTISW3) 

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        case MSG_GET_FOCUS:
            MMIDEFAULT_TurnOnBackLight();
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
            LCD_FillRect(&dev_info, rect, MMI_BLACK_COLOR);
            audio_handle = MMIENG_RequestVirtualAudService();
            GUIBLOCK_Enable(&dev_info);
            MMITHEME_UpdateRect();

#ifdef WIN32
    GUIBLOCK_SetType(0, color, GUIBLOCK_TYPE_OSD, &dev_info);
#else
    GUIBLOCK_SetType(255, color, GUIBLOCK_TYPE_COMMON, &dev_info);
#endif
            frm_len_per_sec = 0;
            debug_time_start = 0;
            SCI_TRACE_LOW("%s, SCI_TRUE = %d, SCI_FALSE = %d", __FUNCTION__, SCI_TRUE, SCI_FALSE);
            LCD_EnableBlock(0, 1);
            start_decoder();
            start_encoder();
            break;

        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
            SaveTestResult( win_id, 1);
            MMK_CloseWin( win_id ) ;
            break;

        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            SaveTestResult( win_id, 0);
            MMK_CloseWin( win_id ) ;
            break;

        case MSG_KEYDOWN_1:
            if (!bImgShowing) {
                LCD_DisableBlock(0, 0);
#if _MSC_VER <= 1600
                VT_SetPreviewDisplayed(SCI_FALSE);
#endif
                bH264DecRefreseLCD = SCI_TRUE;
            } else {
                LCD_EnableBlock(0, 0);
#if _MSC_VER <= 1600
                VT_SetPreviewDisplayed(SCI_TRUE);
#endif
                bH264DecRefreseLCD = SCI_FALSE;
            }
            break;
        case MSG_KEYDOWN_2:
#if _MSC_VER <= 1600
            VT_SetPreviewDisplayed(SCI_FALSE);
#endif
            bH264DecRefreseLCD = SCI_TRUE;
            break;
        case MSG_KEYDOWN_3:
            SCI_TRACE_LOW("%s@%d, case MSG_KEYDOWN_3: switch camera sensor");
#if _MSC_VER <= 1600
            VT_Stop_H264_Encoder();
            VT_Release_H264_Encoder();
#endif
            start_encoder();
            LCD_EnableBlock(MAIN_LCD_ID, 2);
            break;

        case MSG_LOSE_FOCUS:
        case MSG_CLOSE_WINDOW:
#if _MSC_VER <= 1600
            VT_Stop_H264_Encoder();

            VT_Release_H264_Encoder();

            VT_Release_H264_Decoder();
#endif
            LCD_DisableBlock(0, 1);
            frm_len_per_sec = 0;
            debug_time_start = 0;

            MMISRVMGR_Free(audio_handle);
            audio_handle = 0;
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            MMIAPIENG_RunUITestNextStep();
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
#endif
        return recode;
}
/********************************************************************************
NAME:          CameraParamInit
DESCRIPTION:   
PARAM IN:      
PARAM OUT:     
AUTHOR:        songbin.zeng
DATE:          2006.11.30
********************************************************************************/
//LOCAL void CameraParamInit(void)
//{
//    s_mmieng_test_camera_param.focus_item = MMIENG_TEST_CAMERA_WIN_SIZE;
//    s_mmieng_test_camera_param.photo_num = 1;
//    s_mmieng_test_camera_param.photo_pos = FS_UDISK;
//    s_mmieng_test_camera_param.is_AWB = FALSE;//是否白平衡
// }

/********************************************************************************
NAME:          FocusNextCameraParamItem
DESCRIPTION:   
PARAM IN:      
PARAM OUT:     
AUTHOR:        songbin.zeng
DATE:          2006.11.30
********************************************************************************/
//LOCAL void FocusNextCameraParamItem(void)
//{
//    MMIENG_TEST_CAMERA_WIN_PARAM_T param = {MMIENG_TEST_CAMERA_WIN_MIN, 
//                                            0,
//                                            FS_HS,
//                                            FALSE};
//    param = GetCameraParam();
//    param.focus_item ++;
//    if (param.focus_item >= MMIENG_TEST_CAMERA_WIN_MAX)
//    {
//        param.focus_item = MMIENG_TEST_CAMERA_WIN_SIZE;
//    }
//    else
//    {
//    }
//
//    SetCameraParam(&param);
// }
#if 0
/********************************************************************************
NAME:          AdjustCameraParam
DESCRIPTION:   
PARAM IN:      
PARAM OUT:     
AUTHOR:        songbin.zeng
DATE:          2006.11.30
********************************************************************************/
LOCAL void AdjustCameraParam(void)
{
    MMIENG_TEST_CAMERA_WIN_PARAM_T param = {MMIENG_TEST_CAMERA_WIN_MIN, 
        0,
        FS_HS,
        FALSE};
    
    param = GetCameraParam();
    switch(param.focus_item)
    {
    case MMIENG_TEST_CAMERA_WIN_SIZE:
        switch (param.photo_size)
        {
        case MMIDC_PHOTO_SIZE_176X224:
            param.photo_size = MMIDC_PHOTO_SIZE_320X240;
            break;
            
        case MMIDC_PHOTO_SIZE_320X240:
            param.photo_size = MMIDC_PHOTO_SIZE_640X480;
            break;
        case MMIDC_PHOTO_SIZE_640X480:
            param.photo_size = MMIDC_PHOTO_SIZE_1280X1024;
            break;
            
        case MMIDC_PHOTO_SIZE_1280X1024:
            param.photo_size = MMIDC_PHOTO_SIZE_1600X1200;
            break;
            
        case MMIDC_PHOTO_SIZE_1600X1200:
            param.photo_size = MMIDC_PHOTO_SIZE_176X224;
            break;
            
        default:
            SCI_PASSERT(FALSE, ("AdjustCameraParam: param.photo_size is %d",param.photo_size)); 
            break;
        }
        break;
        
    case MMIENG_TEST_CAMERA_WIN_NUM:
        switch (param.photo_num)
        {
        case 1:
            param.photo_num = 10;
            break;
            
        case 10:
            param.photo_num = 20;
            break;
            
        case 20:
            param.photo_num = 50;
            break;
            
        case 50:
            param.photo_num = 100;
            break;
            
        case 100:
            param.photo_num = 200;
            break;
            
        case 200:
            param.photo_num = 300;
            break;
            
        case 300:
            param.photo_num = 1;
            break;
            
        default:
            SCI_PASSERT(FALSE, ("AdjustCameraParam: param.photo_num is %d",param.photo_num));   
            break;
        }
        break;
            
        case MMIENG_TEST_CAMERA_WIN_POS:
            switch (param.photo_pos)
            {
            case FS_UDISK:
                if (MMIAPIFMM_GetDeviceTypeStatus())
                {
                    param.photo_pos = FS_MMC;
                }
                else
                {}
                break;
                
            case FS_MMC:
                param.photo_pos = FS_UDISK;
                break;
                
            default:
                SCI_PASSERT(FALSE, ("AdjustCameraParam: param.photo_pos is %d",param.photo_pos));   
                break;
            }
            break;
                
        case MMIENG_TEST_CAMERA_WIN_AWB:
            switch (param.is_AWB)
            {
            case FALSE:
                if(MMIENG_SENSOR_MODE_RGB == MMIAPIENG_GetSensorMode())
                {
                    param.is_AWB = TRUE;
                }
                else
                {}
                break;
                
            case TRUE:
                param.is_AWB = FALSE;
                break;
                
            default:
                SCI_PASSERT(FALSE, ("AdjustCameraParam: param.is_AWB is %d",param.is_AWB)); 
                break;
            }
            break;
            
            default:
                SCI_PASSERT(FALSE, ("AdjustCameraParam: param.focus_item is %d",param.focus_item)); 
                break;
    }   
    SetCameraParam(&param);
}

/********************************************************************************/
// NAME:          GetCameraParam
// DESCRIPTION:
// PARAM IN:
// PARAM OUT:
// AUTHOR:        songbin.zeng
// DATE:          2006.11.30
/********************************************************************************/
LOCAL MMIENG_TEST_CAMERA_WIN_PARAM_T GetCameraParam(void)
{
    return s_mmieng_test_camera_param;
}
#endif
/********************************************************************************/
// NAME:          UITestHeadsetWinHandleMsg
// DESCRIPTION:
// PARAM IN:
// PARAM OUT:
// AUTHOR:        allen
// DATE:          2004.09.08
/********************************************************************************/
PUBLIC MMI_RESULT_E UITestHeadsetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    #define LB_VOLUME   9
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    wchar headset_is_in_wstr[] = { 'H', 'e', 'a', 'd', 's', 'e', 't', ' ', 'i', 's', ' ', 'i', 'n', ' ', 0 };
    wchar headset_is_out_wstr[] = { 'H', 'e', 'a', 'd', 's', 'e', 't', ' ', 'i', 's', ' ', 'o', 'u', 't', 0 };
//     wchar button_is_pressed_wstr[] = { 'b', 'u', 't', 't', 'o', 'n', ' ', 'i', 's', ' ', 'p', 'r', 'e', 's', 's', 'e', 'd', 0 };
    GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
    GUI_RECT_T lab_rect = client_rect;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:        
        lab_rect = client_rect;
        lab_rect.bottom =   (client_rect.bottom + client_rect.top)/2;
        GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);
        
        lab_rect.top =  lab_rect.bottom + 1;
        lab_rect.bottom = client_rect.bottom ;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
        
        //MMIDEFAULT_EnableKeyRing(FALSE);
        //MMIDEFAULT_EnableTpRing(FALSE);
        
        break;
#ifndef MMI_RES_LOW_COST
    case MSG_LCD_SWITCH:
        lab_rect = client_rect;
        lab_rect.bottom =   (client_rect.bottom + client_rect.top)/2;
        GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);
        
        lab_rect.top =  lab_rect.bottom + 1;
        lab_rect.bottom = client_rect.bottom ;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);      
        break;
#endif
    case MSG_FULL_PAINT:
        text = s_mmieng_str_up_to_next;
        GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  
                IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
        GUILABEL_SetFont(MMIENG_UITEST_LABEL_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
        GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, FALSE);
#ifndef _WIN32
        s_mmieng_headset_is_in = GPIO_CheckHeadsetStatus();
#endif
        if (s_mmieng_headset_is_in)
        {
            text.wstr_ptr = headset_is_in_wstr;//(uint8*)"Headset is in ";
#ifndef _WIN32
            MMISRVAUD_EnableVoiceLB(TRUE, LB_VOLUME);
#endif
        }
        else
        {
            text.wstr_ptr = headset_is_out_wstr;//(uint8*)"Headset is out";
#ifndef _WIN32
            MMISRVAUD_EnableVoiceLB(FALSE, 0);
#endif
        }
        text.wstr_len = (uint16)MMIAPICOM_Wstrlen( text.wstr_ptr);
        GUILABEL_SetFont(MMIENG_LABEL1_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
        
        break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
        SaveTestResult(win_id, 1);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(win_id);
        break;
        
    case MSG_KEYDOWN_CANCEL:         
    case MSG_CTL_CANCEL: 
        SaveTestResult( win_id, 0) ;
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(  win_id) ;            
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif   
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_KEYUP_HEADSET_DETECT:
#if 0 
        text = s_mmieng_str_up_to_next;
        GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, TRUE);
        
        text.wstr_ptr = headset_is_out_wstr;//(uint8*)"Headset is out";
        text.wstr_len = MMIAPICOM_Wstrlen(text.wstr_ptr);
        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
        s_mmieng_headset_is_in = FALSE;
#endif
#ifndef _WIN32
        MMISRVAUD_EnableVoiceLB(FALSE, 0);
#endif
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
        result = MMI_RESULT_FALSE;
        break;
        
    case MSG_KEYDOWN_HEADSET_DETECT:
#if 0
        text.wstr_ptr = button_is_pressed_wstr;//(uint8*)"button is pressed";
        text.wstr_len = MMIAPICOM_Wstrlen( text.wstr_ptr);
        GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, TRUE);
        
        text.wstr_ptr = headset_is_in_wstr;//(uint8*)"Headset is in ";
        text.wstr_len = MMIAPICOM_Wstrlen( text.wstr_ptr);
        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
        s_mmieng_headset_is_in = TRUE;
#endif
#ifndef _WIN32
        MMISRVAUD_EnableVoiceLB(TRUE, LB_VOLUME);
#endif
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);
        result = MMI_RESULT_FALSE;
        break;
        
        
        
    case MSG_LOSE_FOCUS:            
    case MSG_CLOSE_WINDOW:
#ifndef _WIN32
        MMISRVAUD_EnableVoiceLB(FALSE, LB_VOLUME);
#endif
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#ifdef BLUETOOTH_SUPPORT

PUBLIC BOOLEAN MMIENG_UITestBTIsOpenWin(void)
{
    return MMK_IsOpenWin(MMIENG_UITESTBLUETOOTH_WIN_ID);
}
PUBLIC void MMIENG_UITestBT_ON_CNF(BT_MSG_T *msg_body_ptr,uint16 msg_id)
{
    switch(msg_id)
    {
        case MMI_BT_ME_ON_CNF://BT on/off
            MMK_PostMsg(MMIENG_UITESTBLUETOOTH_WIN_ID, MMI_BT_ME_ON_CNF, (DPARAM)msg_body_ptr,sizeof(BT_MSG_T));
            break;
        case MMI_BT_INQ_DEV_NAME://get BT name
            MMK_PostMsg(MMIENG_UITESTBLUETOOTH_WIN_ID, MMI_BT_INQ_DEV_NAME, (DPARAM)msg_body_ptr,sizeof(BT_MSG_T));
            break;
        case MMI_BT_INQ_COMP_CNF://搜索设备结束
            MMK_PostMsg(MMIENG_UITESTBLUETOOTH_WIN_ID, MMI_BT_INQ_COMP_CNF, PNULL,0);
            break;
    }
}
#ifdef ADULT_WATCH_SUPPORT
LOCAL BOOLEAN MMIENG_UITestBT_CNF(ual_cms_msg_t param)
{
    MMI_WIN_ID_T win_id = MMIENG_UITESTBLUETOOTH_WIN_ID;
    uint8           *str_ptr     = PNULL;
    MMI_STRING_T    text         = {0};
    wchar           wstr[100]    = {0};
    uint16          str_len      = 0;
    ual_bt_status_e  ret_bt_val = UAL_BT_STATUS_SUCCESS;

    SCI_TRACE_LOW("[MMIENG]MMIENG_UITestBT_CNF msg_id:%x\n",param.msg_id);
    switch(param.msg_id)
    {
        case UAL_APP_BT_DEV_CLOSED://关闭成功
            SCI_TRACE_LOW("[MMIENG]MMIENG_UITestBT_CNF UAL_APP_BT_DEV_CLOSED");
            break;

        case UAL_APP_BT_DEV_CLOSE_ERR://关闭失败
            SCI_TRACE_LOW("[MMIENG]MMIENG_UITestBT_CNF UAL_APP_BT_DEV_CLOSE_ERR");
            break;

        case UAL_APP_BT_DEV_OPENED://打开bt phone成功
        {
            SCI_TRACE_LOW("[MMIENG]MMIENG_UITestBT_CNF MMI_BT_ME_ON_CNF");
            str_ptr = (uint8*)"Bluetooth is open!";
            str_len = SCI_STRLEN((char*)str_ptr);
            MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
            text.wstr_ptr = wstr;
            text.wstr_len = str_len;
            GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);
            ret_bt_val = ual_bt_dev_cancel_search();
            if(UAL_BT_STATUS_PENDING != ret_bt_val)//already canceled, search directly
            {
                ret_bt_val = ual_bt_dev_start_search(UAL_BT_SERVICE_ALL);

                if((UAL_BT_STATUS_PENDING == ret_bt_val)||(UAL_BT_STATUS_SUCCESS == ret_bt_val))
                {
                    str_ptr = (uint8*)"Bluetooth search ...";
                }
                else
                {
                    str_ptr = (uint8*)"Bluetooth search fail";
                }
            }
            MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
            text.wstr_ptr = wstr;
            text.wstr_len = str_len;
            GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);
            break;
        }

        case UAL_APP_BT_DEV_SEARCH_ONE:
        {
            uint8 searched_dev_num = 0;
            ual_bt_dev_info_t *p_searched_dev_info = PNULL;
            ual_bt_dev_info_t device_info = {0};

            SCI_TRACE_LOW("[MMIENG]MMIENG_UITestBT_CNF UAL_APP_BT_DEV_SEARCH_ONE enter!");

            searched_dev_num = ual_bt_dev_get_searched_num();
            if(0 == searched_dev_num)
            {
                SCI_TRACE_LOW("[MMIENG]MMIENG_UITestBT_CNF searched_dev_num=0!");
                break;
            }
            p_searched_dev_info = ual_bt_dev_get_searched_list();
            if (p_searched_dev_info == PNULL)
            {
                SCI_TRACE_LOW("[MMIENG]MMIENG_UITestBT_CNF searched list not valid.");
                break;
            }

            //current info 只取一个
            device_info = p_searched_dev_info[0];

            //parse name
            text.wstr_ptr = device_info.name;
            text.wstr_len = MMIAPICOM_Wstrlen(text.wstr_ptr);
            GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);

            GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);

            ret_bt_val = ual_bt_dev_cancel_search();
            SCI_TRACE_LOW("[MMIENG]MMIENG_UITestBT_CNF cancel ret_bt_val=%d",ret_bt_val);
            break;
        }

        case UAL_APP_BT_DEV_SEARCH_COMPLETE:
        {
            str_ptr = (uint8*)"Bluetooth search timeout";
            str_len = SCI_STRLEN((char*)str_ptr);
            MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
            text.wstr_ptr = wstr;
            text.wstr_len = str_len;
            GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);
            GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
            {
                MMI_STRING_T kstring = {0};
                wchar test_string[] = L"Reset";

                kstring.wstr_ptr = test_string;
                kstring.wstr_len = MMIAPICOM_Wstrlen(test_string);
                GUIWIN_SetSoftkeyButtonTextPtr( win_id, kstring, 1, TRUE);
            }
            SCI_TRACE_LOW("[MMIENG]MMIENG_UITestBT_CNF UAL_APP_BT_DEV_SEARCH_COMPLETE");
            break;
        }

        case UAL_APP_BT_DEV_OPEN_ERR://打开bt phone 失败
        {
            str_ptr = (uint8*)"Bluetooth open fail";
            str_len = SCI_STRLEN((char*)str_ptr);
            MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
            text.wstr_ptr = wstr;
            text.wstr_len = str_len;
            GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);
            SCI_TRACE_LOW("[MMIENG]MMIENG_UITestBT_CNF UAL_APP_BT_DEV_OPEN_ERR");
            break;
        }
        default:
            SCI_TRACE_LOW("msg_id:%d should not process\n",param.msg_id);
            break;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : UITestBluetoothWinHandleMsg 
//  Parameter: [In] win_id
//             [In] msg_id
//             [In] param
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E UITestBluetoothWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    ual_bt_status_e  ret_bt_val = UAL_BT_STATUS_SUCCESS;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    uint8           *str_ptr     = PNULL;
    MMI_STRING_T    text         = {0};
    wchar           wstr[100]    = {0};
    uint16          str_len      = 0;
    MMI_TEXT_ID_T   s_mmieng_softkey_id[GUISOFTKEY_BUTTON_NUM] = {0};
    boolean  bt_dev_ret = FALSE;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T    text      = {0};
            wchar           wstr[100] = {0};
            uint8           *str_ptr  = PNULL;
            uint16          str_len   = 0;
            GUI_RECT_T      lab_rect  = {0};
            GUI_RECT_T  client_rect   = MMITHEME_GetClientRectEx(win_id);

            //注册bt dev接口
            bt_dev_ret = ual_bt_dev_register(MMIENG_UITestBT_CNF,&s_bt_dev_handle);

            GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,
                IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
            lab_rect = client_rect;
            lab_rect.bottom = (client_rect.bottom + client_rect.top)/2;
            GUILABEL_SetRect(MMIENG_UITEST_TEXTBOX_CTRL_ID, &lab_rect, FALSE);

            lab_rect.top =  lab_rect.bottom + 1;
            lab_rect.bottom = client_rect.bottom ;
            GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);

            GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);


            MMIDEFAULT_SetBackLight(TRUE);
            MMIDEFAULT_SetLcdBackLight(TRUE);
            if(MMIAPISET_GetFlyMode())
            {
                //in fly mode ,can not open BT
                str_ptr = (uint8*)"Fly Mode On, Fail key to next step!";
            }
            else if(TRUE == ual_bt_dev_get_is_on())
            {
                str_ptr = (uint8*)"Bluetooth has open!";

                ret_bt_val = ual_bt_dev_cancel_search();
                if(UAL_BT_STATUS_PENDING != ret_bt_val)//already canceled, search directly
                {
                    ret_bt_val = ual_bt_dev_start_search(UAL_BT_SERVICE_ALL);
                    if((UAL_BT_STATUS_PENDING == ret_bt_val)||(UAL_BT_STATUS_SUCCESS == ret_bt_val))
                    {
                        str_ptr = (uint8*)"Bluetooth search ...";
                    }
                    else
                    {
                        str_ptr = (uint8*)"Bluetooth search fail";
                    }
                }
            }
            else
            {
                if(!ual_bt_dev_get_is_on())
                {
                    ret_bt_val = ual_bt_dev_open();
                    if(UAL_BT_STATUS_PENDING == ret_bt_val)
                    {
                        str_ptr = (uint8*)"Open bluetooth, waiting ...";
                    }
                    else
                    {
                        str_ptr = (uint8*)"Open bluetooth fail";
                    }
                }
                else
                {
                    str_ptr = (uint8*)"Open bluetooth fail";
                }
            }
            str_len = SCI_STRLEN((char*)str_ptr);
            MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
            text.wstr_ptr = wstr;
            text.wstr_len = str_len;
            GUILABEL_SetFont(MMIENG_UITEST_TEXTBOX_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
            GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, FALSE);
            MMK_SetAtvCtrl(win_id,MMIENG_UITEST_TEXTBOX_CTRL_ID);
            SCI_TRACE_LOW("[MMIENG]UITestBluetoothWinHandleMsg: MSG_OPEN_WINDOW bt_dev_ret=%d,handle=%d",bt_dev_ret,s_bt_dev_handle);
        }
            break;
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
            GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,
                IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
        }
            break;

#if defined MMI_PDA_SUPPORT
        case MSG_APP_MENU:
#endif
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
            GUIWIN_GetSoftkeyTextId (win_id, &s_mmieng_softkey_id[0], &s_mmieng_softkey_id[1], &s_mmieng_softkey_id[2]);
            if(!MMIAPISET_GetFlyMode() && (MMI_TEXT_ID_T)TXT_NULL != s_mmieng_softkey_id[0])
            {
                ret_bt_val = ual_bt_dev_cancel_search();

                if(ual_bt_dev_get_is_on())
                {
                    ret_bt_val = ual_bt_dev_close();
                }

                SaveTestResult(win_id, 1);
                MMIAPIENG_RunUITestNextStep();
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            ret_bt_val = ual_bt_dev_cancel_search();

            if(ual_bt_dev_get_is_on())
            {
                ret_bt_val = ual_bt_dev_close();
            }

            SaveTestResult(win_id, 0);
            MMIAPIENG_RunUITestNextStep();
            MMK_CloseWin(win_id);
            break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif
        }
        break;
#endif
        case MSG_KEYDOWN_WEB:
            GUIWIN_GetSoftkeyTextId (win_id, &s_mmieng_softkey_id[0], &s_mmieng_softkey_id[1], &s_mmieng_softkey_id[2]);
            if(!MMIAPISET_GetFlyMode() && (MMI_TEXT_ID_T)TXT_NULL != s_mmieng_softkey_id[1])
            {
                ret_bt_val = ual_bt_dev_cancel_search();
                if(UAL_BT_STATUS_PENDING != ret_bt_val)
                {
                    ret_bt_val = ual_bt_dev_start_search(UAL_BT_SERVICE_ALL);
                    if(UAL_BT_STATUS_PENDING == ret_bt_val)
                    {
                        str_ptr = (uint8*)"Bluetooth search ...";
                    }
                    else
                    {
                        str_ptr = (uint8*)"Bluetooth search fail";
                    }
                }

                str_len = SCI_STRLEN((char*)str_ptr);
                MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
                text.wstr_ptr = wstr;
                text.wstr_len = str_len;
                GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);
                GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
            }
            break;
        case MSG_KEYDOWN_RED:
            break;

        case MSG_CLOSE_WINDOW:
            ret_bt_val = ual_bt_dev_cancel_search();

            if(ual_bt_dev_get_is_on())
            {
                ret_bt_val = ual_bt_dev_close();
            }

            //去注册bt dev接口
            bt_dev_ret = ual_bt_dev_unregister(s_bt_dev_handle);
            SCI_TRACE_LOW("[MMIENG]UITestBluetoothWinHandleMsg: MSG_CLOSE_WINDOW bt_dev_ret=%d,handle=%d",bt_dev_ret,s_bt_dev_handle);
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}
#else
/********************************************************************************/
// NAME:          UITestBluetoothWinHandleMsg
// DESCRIPTION:
// PARAM IN:
// PARAM OUT:
// AUTHOR:        kelly.li
// DATE:          2006.08.19
/********************************************************************************/
PUBLIC MMI_RESULT_E UITestBluetoothWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{

    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    BOOLEAN         bluetooth_on = FALSE;
    uint8           *str_ptr     = PNULL;
    MMI_STRING_T    text         = {0};
    wchar           wstr[100]    = {0};
    uint16          str_len      = 0;
    BT_STATUS       ret_val      = BT_SUCCESS;
    MMI_TEXT_ID_T   s_mmieng_softkey_id[GUISOFTKEY_BUTTON_NUM] = {0};


    bluetooth_on = MMIAPIBT_IsBtOn();
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T    text      = {0};
            wchar           wstr[100] = {0};
            uint8           *str_ptr  = PNULL;
            uint16          str_len   = 0;
            GUI_RECT_T      lab_rect  = {0};
            GUI_RECT_T  client_rect   = MMITHEME_GetClientRectEx(win_id);
            GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,
                IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
            lab_rect = client_rect;
            lab_rect.bottom = (client_rect.bottom + client_rect.top)/2;
            GUILABEL_SetRect(MMIENG_UITEST_TEXTBOX_CTRL_ID, &lab_rect, FALSE);

            lab_rect.top =  lab_rect.bottom + 1;
            lab_rect.bottom = client_rect.bottom ;
            GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);

            GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);


            MMIDEFAULT_SetBackLight(TRUE);
            MMIDEFAULT_SetLcdBackLight(TRUE);
            if(MMIAPISET_GetFlyMode())
            {
                //in fly mode ,can not open BT
                str_ptr = (uint8*)"Fly Mode On, Fail key to next step!";
            }else if(TRUE == bluetooth_on)
            {
                str_ptr = (uint8*)"Bluetooth has open!";
                ret_val = BT_CancelSearch();
                if(BT_PENDING != ret_val)//already canceled, search directly
                {
                    ret_val = BT_SearchDevice(BT_SERVICE_ALL);
                    if(BT_PENDING == ret_val)
                    {
                        str_ptr = (uint8*)"Bluetooth search ...";
                    }
                    else
                    {
                        str_ptr = (uint8*)"Bluetooth search fail";
                    }
                }
            }
            else
            {
                if(!BT_GetState())
                {
                    ret_val = BT_Start();
                    if(BT_PENDING == ret_val)
                    {
                        str_ptr = (uint8*)"Open bluetooth, waiting ...";
                    }
                    else
                    {
                        str_ptr = (uint8*)"Open bluetooth fail";
                    }
                }else
                {
                    str_ptr = (uint8*)"Open bluetooth fail";
                }
            }
            str_len = SCI_STRLEN((char*)str_ptr);
            MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
            text.wstr_ptr = wstr;
            text.wstr_len = str_len;
            GUILABEL_SetFont(MMIENG_UITEST_TEXTBOX_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
            GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, FALSE);
            MMK_SetAtvCtrl(win_id,MMIENG_UITEST_TEXTBOX_CTRL_ID);
        }
            break;
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
            GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,
                IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
        }
            break;
        case MMI_BT_ME_ON_CNF:
        {
            BT_MSG_T     *msg_body = (BT_MSG_T*)param;
            SCI_TRACE_LOW("[MMIENG]TestBTWinHandleMsg MMI_BT_ME_ON_CNF");
            if(BT_SUCCESS == msg_body->status)
            {
                str_ptr = (uint8*)"Bluetooth is open!";
                str_len = SCI_STRLEN((char*)str_ptr);
                MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
                text.wstr_ptr = wstr;
                text.wstr_len = str_len;
                GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);
                ret_val = BT_CancelSearch();
                if(BT_PENDING != ret_val)//already canceled, search directly
                {
                    ret_val = BT_SearchDevice(BT_SERVICE_ALL);

                    if(BT_PENDING == ret_val)
                    {
                        str_ptr = (uint8*)"Bluetooth search ...";
                    }
                    else
                    {
                        str_ptr = (uint8*)"Bluetooth search fail";
                    }
                }
            }
            else
            {
                str_ptr = (uint8*)"Bluetooth open fail";
            }
                str_len = SCI_STRLEN((char*)str_ptr);
                MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
                text.wstr_ptr = wstr;
                text.wstr_len = str_len;
                GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);
            }
            break;
        case MMI_BT_INQ_DEV_NAME:
        {
            BT_MSG_T     *msg_body = (BT_MSG_T*)param;
            uint8 addr_str[BT_ADDRESS_SIZE*4] = {0};
            wchar addr_wstr[BT_ADDRESS_SIZE*4] = {0};
            uint8 i = 0;
            uint16 addr_str_len = 0; 

            if(BT_SUCCESS == msg_body->status)
            {
                BT_DEVICE_INFO  dev_info   = *((BT_DEVICE_INFO*)(msg_body->body_ptr));
                MMI_STRING_T    text2 = {0};

                sprintf((char*)addr_str, "%x", dev_info.addr.addr[0]);
                for(i = 1; i < BT_ADDRESS_SIZE; i++)
                {
                    addr_str_len = strlen((char*)addr_str);
                    sprintf((char*)&addr_str[addr_str_len], ":%x", dev_info.addr.addr[i]);
                }
                addr_str_len = strlen((char*)addr_str);
                MMI_STRNTOWSTR( addr_wstr, BT_ADDRESS_SIZE*4, addr_str, addr_str_len, addr_str_len);

                text.wstr_ptr = dev_info.name;
                text.wstr_len = MMIAPICOM_Wstrlen(dev_info.name);
                GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);

                text2.wstr_ptr = addr_wstr;
                text2.wstr_len = MMIAPICOM_Wstrlen(addr_wstr);
                GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text2, TRUE);

                GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
            }
            else
            {
                str_ptr = (uint8*)"Bluetooth search fail";
                str_len = SCI_STRLEN((char*)str_ptr);
                MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
                text.wstr_ptr = wstr;
                text.wstr_len = str_len;
                GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);
                GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
            }
                ret_val = BT_CancelSearch();
        }
            break;
        case MMI_BT_INQ_COMP_CNF:
            str_ptr = (uint8*)"Bluetooth search timeout";
            str_len = SCI_STRLEN((char*)str_ptr);
            MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
            text.wstr_ptr = wstr;
            text.wstr_len = str_len;
            GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);
            GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
            {
                MMI_STRING_T kstring = {0};
                wchar test_string[] = L"Reset";

                kstring.wstr_ptr = test_string;
                kstring.wstr_len = MMIAPICOM_Wstrlen(test_string);
                GUIWIN_SetSoftkeyButtonTextPtr( win_id, kstring, 1, TRUE);
            }      
            break;
#if defined MMI_PDA_SUPPORT
        case MSG_APP_MENU:
#endif
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
            GUIWIN_GetSoftkeyTextId (win_id, &s_mmieng_softkey_id[0], &s_mmieng_softkey_id[1], &s_mmieng_softkey_id[2]);
            if(!MMIAPISET_GetFlyMode() && (MMI_TEXT_ID_T)TXT_NULL != s_mmieng_softkey_id[0])
            {
                ret_val = BT_CancelSearch();
                if(FALSE == bluetooth_on)
                {
                    if(BT_GetState())
                    {
                        ret_val = BT_Stop();
                    }
                }
                SaveTestResult(win_id, 1);
                MMIAPIENG_RunUITestNextStep();
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            ret_val = BT_CancelSearch();
            if(FALSE == bluetooth_on)
            {
                if(BT_GetState())
                {
                    ret_val = BT_Stop();
                }
            }
            SaveTestResult(win_id, 0);
            MMIAPIENG_RunUITestNextStep();
            MMK_CloseWin(win_id);
            break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif   
        }
        break;
#endif
        case MSG_KEYDOWN_WEB:
            GUIWIN_GetSoftkeyTextId (win_id, &s_mmieng_softkey_id[0], &s_mmieng_softkey_id[1], &s_mmieng_softkey_id[2]);
            if(!MMIAPISET_GetFlyMode() && (MMI_TEXT_ID_T)TXT_NULL != s_mmieng_softkey_id[1])
            {
                ret_val = BT_CancelSearch();
                if(BT_PENDING != ret_val)
                {
                    ret_val = BT_SearchDevice(BT_SERVICE_ALL);
                    if(BT_PENDING == ret_val)
                    {
                        str_ptr = (uint8*)"Bluetooth search ...";
                    }
                    else
                    {
                        str_ptr = (uint8*)"Bluetooth search fail";
                    }
                }

                str_len = SCI_STRLEN((char*)str_ptr);
                MMI_STRNTOWSTR( wstr, 100, str_ptr, str_len, str_len);
                text.wstr_ptr = wstr;
                text.wstr_len = str_len;
                GUILABEL_SetText(MMIENG_UITEST_TEXTBOX_CTRL_ID, &text, TRUE);
                GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
            }
            break;
        case MSG_KEYDOWN_RED:
            break;

        case MSG_CLOSE_WINDOW:
            ret_val = BT_CancelSearch();
            if(FALSE == bluetooth_on)
            {
                if(BT_GetState())
                {
                    ret_val = BT_Stop();
                }
            }
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}
#endif  //ADULT_WATCH_SUPPORT else end
#endif

/*****************************************************************************/
//  Description : Start receiver voice loop.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIENG_EnableReceiverVoiceLB(BOOLEAN is_enable, uint32 volume) //add for bug746955
{
    static MMISRV_HANDLE_T lb_handle = 0;
    BOOLEAN res = FALSE;

    //MMISRV_TRACE_LOW:"[MMISRV]: MMISRVAUD_EnableVoiceLB() entry, handle=0x%x, is_enable=%d,volume=%d"
    SCI_TraceLow("MMIENG_EnableReceiverVoiceLB  lb_handle:%d is_enable:%d volume:%d", lb_handle, is_enable, volume);
    
    if(lb_handle != 0)
    {
        MMISRVMGR_Free(lb_handle);
        lb_handle = 0;
    }
    
    if(is_enable)
    {
        MMISRVMGR_SERVICE_REQ_T req = {0};
        MMISRVAUD_TYPE_T audio_srv = {0};
        
        req.pri = MMISRVAUD_PRI_HIGH;

        audio_srv.duation = 0;
        audio_srv.eq_mode = 0;
        audio_srv.is_mixing_enable = FALSE;
        audio_srv.play_times = 1;
        audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER;
        audio_srv.info.voice.type = MMISRVAUD_TYPE_VOICE_LOOPBACK;
        audio_srv.volume = volume;

        lb_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, (void *)&audio_srv);
        if(lb_handle > 0)
        {
            res = MMISRVAUD_Play(lb_handle, 0);
        }
    }
    else
    {
        res = TRUE;
    }

    //MMISRV_TRACE_LOW:"[MMISRV]: MMISRVAUD_EnableVoiceLB() exit, handle=0x%x, res=%d"
    SCI_TraceLow("MMIENG_EnableReceiverVoiceLB lb_handle:%d res:%d", lb_handle, res);
    
    return res;
}


/*******************************************************************************/
// NAME:          UITestLoopbackWinHandleMsg
// DESCRIPTION:
// PARAM IN:
// PARAM OUT:
// AUTHOR:        ying.xu
// DATE:          2010.08.16
/*******************************************************************************/
PUBLIC MMI_RESULT_E UITestLoopbackWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
//    static  uint16  original_call_volume = 0;
    const  uint16  test_call_volume = 9;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        text = s_mmieng_str_up_to_next;
        GUILABEL_SetFont(MMIENG_UITEST_LABEL_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
        GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, FALSE);
//        original_call_volume  = MMIAPISET_GetCallVolume();
//         original_mode =   MMIAUDIO_GetCurDevMode();
//         AUD_EnableVoiceLoopback(hVoiceRing, TRUE, 100);
//         MMIAUDIO_SetVolumeDeviceModeExt(MMIAUDIO_GetCurDevMode(), test_call_volume);

        MMIENG_EnableReceiverVoiceLB(TRUE, test_call_volume); //fix for bug746955
        //SCI_TRACE_LOW:"MMIENG_UITESTWIN.C  UITestLoopbackWinHandleMsg   MSG_OPEN_WINDOW "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_4407_112_2_18_2_17_32_69,(uint8*)"");
        
        break;
        
  /*      
    case MSG_KEYUP_HEADSET_DETECT:
        {
            MMIAUDIO_SetVolume(AUDIO_DEVICE_MODE_HANDHOLD, test_call_volume);
            break;
        }
    case MSG_KEYDOWN_HEADSET_DETECT:
        {
            MMIAUDIO_SetVolume(AUDIO_DEVICE_MODE_EARPHONE, test_call_volume);
            break;
        }
 */       
    case MSG_FULL_PAINT:
        {
            GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
            GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  
                IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
        }
        break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_KEYDOWN_OK:  
    case MSG_CTL_OK: 
        SaveTestResult(win_id, 1);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(win_id);        
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL: 
        SaveTestResult(win_id, 0);
        MMIAPIENG_RunUITestNextStep();   
        MMK_CloseWin(  win_id) ;      
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif   
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_GET_FOCUS:
        //需要先停止键盘音等
        MMIENG_EnableReceiverVoiceLB(TRUE, test_call_volume);
       //SCI_TRACE_LOW:"MMIENG_UITESTWIN.C  UITestLoopbackWinHandleMsg   MSG_GET_FOCUS "
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_4469_112_2_18_2_17_32_70,(uint8*)"");
        //AUD_EnableVoiceLoopback(hVoiceRing, TRUE, 100);
        //MMIAUDIO_SetVolumeDeviceModeExt(original_mode, test_call_volume);
        break;
        
    case MSG_LOSE_FOCUS:   
    case MSG_CLOSE_WINDOW:
 //       UITestLoopbackSetUlPA(AUDIO_DEVICE_MODE_HANDHOLD, original_handset_volume);
 //       UITestLoopbackSetUlPA(AUDIO_DEVICE_MODE_EARPHONE, original_headset_volume);
  //      AUD_EnableVoiceLoopback(hVoiceRing, FALSE, 100);
 //       MMIAUDIO_SetVolumeDeviceModeExt(original_mode, original_call_volume);
        MMIENG_EnableReceiverVoiceLB(FALSE, test_call_volume);
        //SCI_TRACE_LOW:"MMIENG_UITESTWIN.C  UITestLoopbackWinHandleMsg   MSG_LOSE_FOCUS "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_4481_112_2_18_2_17_32_71,(uint8*)"");
        break;

	case MSG_APP_UPSIDE:        
        if(!IsVolumePanelOpen())
        {
            MMILOOPBACK_TurnVol(MMICOM_PANEL_OPEN_INC);
        }
        break;
		
    case MSG_APP_DOWNSIDE:
        
        if(!IsVolumePanelOpen())
        {
            MMILOOPBACK_TurnVol(MMICOM_PANEL_OPEN_DEC);
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


/*****************************************************************************/
// Description : set vol
// Global resource dependence :
// Author: 
// Note:
/*****************************************************************************/
LOCAL void SetVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr)
{
    if(PNULL != para_ptr)
    {
		s_loopback_volume_value_cur = para_ptr->cur_value;
		MMISRVAUD_SetVolumeNoHandle(para_ptr->cur_value);
    } 
}
/*****************************************************************************/
//  Description : 调节音量窗口
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMILOOPBACK_TurnVol(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type)
{
    MMICOM_PANEL_INFO_T panel_info = {0};
	
    panel_info.x = -1;
    panel_info.y = -1;
    panel_info.cur_value = s_loopback_volume_value_cur;
    panel_info.last_value = s_loopback_volume_value_cur;
    panel_info.min_value = MMISET_VOL_ZERO;   
    panel_info.max_value = MMISET_VOL_MAX;
    panel_info.panel_type = MMICOM_PANEL_VOLUME;
    panel_info.open_type = turn_vol_type;
    panel_info.set_callback = SetVolumeCallback; 
#ifndef UI_MULTILAYER_SUPPORT
	panel_info.createwin_style = MMICOM_PLAYER_PANEL_CREATE_WIN;
#endif
    MMIAPICOM_OpenPanelChildWin(MMK_GetFocusWinHandle(),&panel_info); 
}

/*****************************************************************************/
//  Description : Display loopback volume images.
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsVolumePanelOpen(
								void
								)
{
    return MMIAPICOM_IsPanelOpened();
}

/********************************************************************************/
// NAME:          UITestMelodyWinHandleMsg
// DESCRIPTION:
// PARAM IN:
// PARAM OUT:
// AUTHOR:        allen
// DATE:          2004.09.08
/********************************************************************************/
PUBLIC MMI_RESULT_E UITestMelodyWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    LOCAL   uint8  ring_id = 1;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        text = s_mmieng_str_up_to_next;
        GUILABEL_SetFont(MMIENG_UITEST_LABEL_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
        GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, FALSE);
        // start melody playing
        s_mmieng_melody_volume = MMIAPISET_GetCallRingVolume();
        if (0 == s_mmieng_melody_volume)
        {
            s_mmieng_melody_volume = 3;
        }
        MMIAPISET_PlayCallRingByVol(s_mmieng_melody_volume, ring_id,AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);
        break;

    case MSG_FULL_PAINT:  
        {
            GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
            GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  
                IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
        }
        break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        SaveTestResult(win_id, 1);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(win_id);
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:  
        MMIAPISET_StopAppRing();
        SaveTestResult(win_id, 0);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif   
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_UPSIDE:
        // adjust melody volume
        if (s_mmieng_melody_volume < MMISET_VOL_MAX)
        {
            s_mmieng_melody_volume++;
            MMIAPISET_PlayCallRingByVol(s_mmieng_melody_volume,ring_id, AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);
        }
        break;
        
    case MSG_KEYDOWN_DOWNSIDE:
        // adjust melody volume
        if (s_mmieng_melody_volume > MMISET_VOL_ONE)
        {
            s_mmieng_melody_volume--;
            MMIAPISET_PlayCallRingByVol(s_mmieng_melody_volume,ring_id, AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);
        }
        break;
        
    case MSG_KEYDOWN_LEFT:
        if(ring_id>1) ring_id--;
        else ring_id = MMISET_RING_FIXED_DEFAUT_NUM;
        MMIAPISET_PlayCallRingByVol(s_mmieng_melody_volume,ring_id, AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);
        
        break;
        
    case MSG_KEYDOWN_RIGHT:
        if(ring_id<MMISET_RING_FIXED_DEFAUT_NUM) ring_id++;
        else ring_id = 1;
        MMIAPISET_PlayCallRingByVol(s_mmieng_melody_volume,ring_id, AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);     
        
        break;  
#if defined MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {        
            GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(!GUI_PointIsInRect(point,client_rect))
                break;
            if(ring_id<MMISET_RING_FIXED_DEFAUT_NUM) 
                ring_id++;
            else
                ring_id = 1;
            MMIAPISET_PlayCallRingByVol(s_mmieng_melody_volume,ring_id, AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);             
        }
        break;  
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

#ifdef FLIP_PHONE_SUPPORT
    case MSG_KEYDOWN_FLIP:      // close flip
#endif    
    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_LOSE_FOCUS:
        // stop melody
        MMIAPISET_StopAppRing();
        break;
        
    case MSG_GET_FOCUS:
        // start melody playing     
        MMIAPISET_PlayCallRingByVol(s_mmieng_melody_volume,ring_id, AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);
        break;
        
    case MSG_CLOSE_WINDOW:
        // stop melody
        MMIAPISET_StopAppRing();
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
/********************************************************************************
NAME:          UITestTVOutWinHandleMsg
DESCRIPTION:     handle message of TV out test window 
AUTHOR:              yuehz
DATE:          2005.12.25
********************************************************************************/
//PUBLIC MMI_RESULT_E UITestTVOutWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
//{
//    MMI_RESULT_E result = MMI_RESULT_TRUE;
//    MMI_STRING_T text = {0};
//
//    switch (msg_id)
//    {
//        case MSG_OPEN_WINDOW:
//            text = s_mmieng_str_up_to_next;
//            GUILABEL_SetBackgroundColor(MMIENG_UITEST_LABEL_CTRL_ID, mmi_uitest_display.bkgd_color);
//            GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, FALSE);
//            
//         text.wstr_ptr = (uint8*)"TV Out";
//         text.wstr_len  = 6;
//            MMK_SetWinTitle(win_id, (uint8 *)text.wstr_ptr, text.wstr_len, FALSE, TRUE);
//          
//           MMK_PostMsg(
//                              win_id,
//                              MSG_TVOUT_STARTING,
//                              (DPARAM)PNULL,
//                              0
//                              );
//      break;
//
//        case MSG_TVOUT_STARTING:
//            MMIAPITVOUT_StartTvOut();
//            break;
//
//        case MSG_FULL_PAINT:       
//            break;
//         
//        case MSG_KEYDOWN_RED:
//            break;
//        case MSG_KEYDOWN_UP:
//        #ifdef SC6800_DVB
//        case MSG_KEYDOWN_GREEN:
//        #endif
//            #ifndef MMI_PRJ8800
//            MMIAPITVOUT_Close();
//            #endif
//          
//            MMIAPIENG_RunUITestNextStep();
//            break;
//            
//        case MSG_CLOSE_WINDOW:
//            MMIAPITVOUT_Close();
//            break;
//
//        default:
//            result = MMI_RESULT_FALSE;
//            break;
//    }
//
//    return (result);
//}

#ifdef APP_ENG_TEST_TFLASH_SUPPORT
/********************************************************************************
NAME:          UITestTFlashWinHandleMsg
DESCRIPTION:
AUTHOR:        guopeng.zhang
DATE:          2005.12.25
********************************************************************************/
PUBLIC MMI_RESULT_E UITestTFlashWinHandleMsg(
                                             MMI_WIN_ID_T  win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM           param
                                             )
{
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_RECT_T client_rect = MMITHEME_GetClientRect();
    GUI_RECT_T lab_rect = client_rect ;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        lab_rect.top =  client_rect.top + (client_rect.bottom - client_rect.top)*1/5;
        lab_rect.bottom =  client_rect.top + (client_rect.bottom - client_rect.top)*2/5;  
        GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);                
        lab_rect.top =  client_rect.top + (client_rect.bottom - client_rect.top)*3/5;
        lab_rect.bottom = client_rect.top + (client_rect.bottom - client_rect.top)*4/5;
        
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
        
        text.wstr_ptr = (uint8*)"TFlash Card";
        text.wstr_len  = 11;
        GUIWIN_SetTitleText(win_id, (uint8 *)text.wstr_ptr, text.wstr_len, FALSE);
        
        GUILABEL_SetBackgroundColor(MMIENG_UITEST_LABEL_CTRL_ID, 
            mmi_uitest_display.bkgd_color);
        text.wstr_ptr = (uint8*)"Green key to quit";
        text.wstr_len  = 17;
        GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, FALSE);
        
#ifndef _WIN32      //@$discuss rocky仍然用的PROD_GetSDCardVector函数
        if(SCM_STATUS_NORMAL == SCM_GetSlotStatus(SCM_SLOT_0))
#else
            if (FALSE)
#endif
            {
                text.wstr_ptr = (uint8*)"TFLASH detected";
                text.wstr_len  = 14;
            }
            else
            {
                text.wstr_ptr = (uint8*)"TFLASH not detected";
                text.wstr_len  = 19;
            }
            GUILABEL_SetFont(MMIENG_LABEL1_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, FALSE);
            
            break;
#ifndef MMI_RES_LOW_COST
    case MSG_LCD_SWITCH:
        lab_rect.top =  client_rect.top + (client_rect.bottom - client_rect.top)*1/5;
        lab_rect.bottom =  client_rect.top + (client_rect.bottom - client_rect.top)*2/5;
        GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);
        
        lab_rect.top =  client_rect.top + (client_rect.bottom - client_rect.top)*3/5;
        lab_rect.bottom = client_rect.top + (client_rect.bottom - client_rect.top)*4/5;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);      
        break;
#endif
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
        SaveTestResult( win_id, 1) ;
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(  win_id) ;
        
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        SaveTestResult( win_id, 0) ;
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(  win_id) ;        
        break;
        
    case MSG_KEYDOWN_RED:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
#endif

#ifdef APP_ENG_COMTEST_SUPPORT
/********************************************************************************
NAME:          UITestCOMWinHandleMsg
DESCRIPTION:     handle message of com  test window 
AUTHOR:              yuehz
DATE:          2006.04.18
********************************************************************************/
PUBLIC MMI_RESULT_E UITestCOMWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    static BOOLEAN is_restore_armlog;
    uint8           debug_phy_port = 0;
    uint8           debug_phy_port_bak = 0;
    SIO_CONTROL_S   m_dcb = {0};
    uint32         save_val = 0;
    uint32         save_val_0 = 0;
    uint32         save_val_1 = 0;
    uint32         temp = 0;
    int  i = 0;
    void *ret_ptr = NULL;
    GUI_RECT_T client_rect = MMITHEME_GetClientRect();
    GUI_RECT_T lab_rect = client_rect; 
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMI_STRING_T text = {0};
            lab_rect.top =  client_rect.top + (client_rect.bottom - client_rect.top)*1/5;
            lab_rect.bottom =  client_rect.top + (client_rect.bottom - client_rect.top)*2/5;
            GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);

            lab_rect.top =  client_rect.top + (client_rect.bottom - client_rect.top)*3/5;
            lab_rect.bottom = client_rect.top + (client_rect.bottom - client_rect.top)*4/5;
            GUILABEL_SetRect(MMIENG_LABEL3_CTRL_ID, &lab_rect, FALSE);
        }

        text.wstr_ptr = (uint8*)"COM Test";
        text.wstr_len  = 8;
        GUIWIN_SetTitleText(win_id, (uint8 *)text.wstr_ptr, text.wstr_len, FALSE);

#ifndef WIN32
        if (REFPARAM_GetEnableArmLogFlag())
        {
            SCI_SetArmLogFlag(FALSE);
            is_restore_armlog=TRUE;
        }
        SIO_SetUARTConfigForUITest();
#endif

    break;

    case MSG_LCD_SWITCH:
        lab_rect.top =  client_rect.top + (client_rect.bottom - client_rect.top)*1/5;
        lab_rect.bottom =  client_rect.top + (client_rect.bottom - client_rect.top)*2/5;
        GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);
        
        lab_rect.top =  client_rect.top + (client_rect.bottom - client_rect.top)*3/5;
        lab_rect.bottom = client_rect.top + (client_rect.bottom - client_rect.top)*4/5;
        GUILABEL_SetRect(MMIENG_LABEL3_CTRL_ID, &lab_rect, FALSE);      
        
        break;
        
    case MSG_FULL_PAINT:
        {
            MMI_STRING_T text = {0};
            
            GUILABEL_SetBackgroundColor(MMIENG_UITEST_LABEL_CTRL_ID, 
                mmi_uitest_display.bkgd_color);
            
            text.wstr_ptr = (uint8*)"Green key to quit";
            text.wstr_len  = 17;
            GUILABEL_SetFont(MMIENG_UITEST_LABEL_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
            GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, FALSE);
            
#if 0
            GUILABEL_SetBackgroundColor(MMIENG_LABEL1_CTRL_ID, 
                mmi_uitest_display.bkgd_color);
            text.wstr_ptr = (uint8*)"COM0 Receives:";
            text.wstr_len  = 14;
            GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, FALSE);
            
            GUILABEL_SetBackgroundColor(MMIENG_LABEL2_CTRL_ID, 
                mmi_uitest_display.bkgd_color);
            text.wstr_ptr = (uint8*)"COM1 Receives:";
            text.wstr_len  = 14;
            GUILABEL_SetText(MMIENG_LABEL2_CTRL_ID, &text, FALSE);
#endif
            GUILABEL_SetBackgroundColor(MMIENG_LABEL3_CTRL_ID, 
                mmi_uitest_display.bkgd_color);
            text.wstr_ptr = (uint8*)"Three com ports will receive data from PC and send them back to PC.";
            text.wstr_len  = 66;
            GUILABEL_SetText(MMIENG_LABEL3_CTRL_ID, &text, FALSE);
            
        }
        
        break;
        
    case MSG_KEYDOWN_RED:
        {
            char ch=NULL;
#ifndef WIN32
            
            KPD_SetGreenKeyPressed(FALSE);  //clear the green key pressed sign.
            
            m_dcb.baud_rate      = SIO_GetBaudRate(COM_DEBUG);
            
            debug_phy_port = REFPARAM_GetDebugPortPhyNo();
            debug_phy_port_bak = debug_phy_port;
            if(debug_phy_port != 0xff)
            {
                SIO_Close(COM_DEBUG);
            }      
            
            //test COM0
            debug_phy_port = 0;
            ret_ptr = SIO_Create(COM_DEBUG, debug_phy_port, &m_dcb);   
            
            if(PNULL == ret_ptr)
            {
                //SCI_TRACE_LOW:"mmieng_uitestwin.c UITestCOMWinHandleMsg COM0 ret_ptr == null"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_4972_112_2_18_2_17_33_73,(uint8*)"");
                return MMI_RESULT_TRUE;
            }
            while(TRUE)
            {  
                ch=SIO_GetAChar(0);      
                while(ch)
                {                   
                    SIO_PutAChar(0/*COM0*/,ch);              
                    ch=SIO_GetAChar(0/*COM0*/);                  
                }
                if(KPD_GetGreenKeyPressed())
                {
                    break;
                }
            }   
            
            SIO_Close(COM_DEBUG);
            KPD_SetGreenKeyPressed(FALSE);  //clear the green key pressed sign.
            
            
            //test COM1
            debug_phy_port = 1;
            ret_ptr = SIO_Create(COM_DEBUG, debug_phy_port, &m_dcb);   
            
            if(PNULL == ret_ptr)
            {
                //SCI_TRACE_LOW:"mmieng_uitestwin.c UITestCOMWinHandleMsg COM1 ret_ptr == null"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_4999_112_2_18_2_17_33_74,(uint8*)"");
                return MMI_RESULT_TRUE;
            }
            
            while(TRUE)
            {  
                ch=SIO_GetAChar(1);      
                
                while(ch)
                {                   
                    SIO_PutAChar(1,ch);              
                    ch=SIO_GetAChar(1);                  
                }
                if(KPD_GetGreenKeyPressed())
                {
                    break;
                }
            }   
            
            SIO_Close(COM_DEBUG);
            KPD_SetGreenKeyPressed(FALSE);  //clear the green key pressed sign.
            
            //test COM2            
            debug_phy_port = 2;
            ret_ptr = SIO_Create(COM_DEBUG, debug_phy_port, &m_dcb);   
            
            if(PNULL == ret_ptr)
            {
                //SCI_TRACE_LOW:"mmieng_uitestwin.c UITestCOMWinHandleMsg COM2 ret_ptr == null"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_5027_112_2_18_2_17_33_75,(uint8*)"");
                return MMI_RESULT_TRUE;
            }

            while(TRUE)
            {  
                ch=SIO_GetAChar(2);      
                while(ch)
                {                   
                    SIO_PutAChar(2,ch);              
                    ch=SIO_GetAChar(2);                  
                }
                if(KPD_GetGreenKeyPressed())
                {
                    break;
                }
            }   
            
            SIO_Close(COM_DEBUG);  
            
            debug_phy_port = debug_phy_port_bak;         
            SIO_Create(COM_DEBUG, debug_phy_port, &m_dcb);   
#endif
            
        }    
        
        break;
        
    case MSG_KEYDOWN_OK:        
    case MSG_CTL_OK:
        SaveTestResult( win_id, 1) ;
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(  win_id) ;            
        break;
        
        
    case MSG_KEYDOWN_CANCEL:   
    case MSG_CTL_CANCEL:
        SaveTestResult( win_id, 0) ;
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(  win_id) ;            
        break;
        
    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_CLOSE_WINDOW:
#ifndef WIN32
        if(is_restore_armlog)
        {
            SCI_SetArmLogFlag(TRUE);
            is_restore_armlog=FALSE;
        }
        SIO_RestoreUARTConfigForUITest();          
#endif
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

#endif
/********************************************************************************/
// NAME:          UITestSDWinHandleMsg
// DESCRIPTION:
// PARAM IN:
// PARAM OUT:
// AUTHOR:        allen
// DATE:          2004.09.08
/********************************************************************************/
PUBLIC MMI_RESULT_E UITestSDWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    wchar have_sdcard_wstr[] = { 'H', 'a', 'v', 'e', ' ', 'S', 'D', 'C', 'a', 'r', 'd', 0 };
    wchar no_sdcard_wstr[] = { 'N', 'o', ' ', 'S', 'D', 'C', 'a', 'r', 'd', 0 };
    
    MMIFILE_DEVICE_E device = MMI_DEVICE_SDCARD;
    BOOLEAN sd_card_ok = FALSE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        break;
#ifndef MMI_RES_LOW_COST
    case MSG_LCD_SWITCH :
        break;
#endif
    case MSG_FULL_PAINT:
        {
            GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
            GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  
                IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
        }
        do
        {
            if(MMIAPIFMM_GetDeviceTypeStatus(device))
            {
                sd_card_ok = TRUE;
                break;
            }
        }while(++device < MMI_DEVICE_NUM);
        
        if(sd_card_ok)
        {
            text.wstr_ptr = have_sdcard_wstr;//(uint8*)"Have SDCard";
        }
        else
        {
            text.wstr_ptr = no_sdcard_wstr;//(uint8*)"No SDCard";
        }
        text.wstr_len = MMIAPICOM_Wstrlen( text.wstr_ptr);
        GUILABEL_SetFont(MMIENG_UITEST_LABEL_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
        GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, FALSE);
        break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
        SaveTestResult(win_id, 1);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(win_id);
        break;
        
    case MSG_KEYDOWN_CANCEL:    
    case MSG_CTL_CANCEL:
        SaveTestResult(win_id, 0);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(  win_id) ;            
        break;  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif   
        }
        break;  
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_KEYDOWN_RED:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

#ifdef FM_SUPPORT
/********************************************************************************
NAME:          UITestFmWinHandleMsg
DESCRIPTION:   
AUTHOR:        liqing.peng
DATE:          2006.10.9
********************************************************************************/
PUBLIC MMI_RESULT_E UITestFmWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
 //   MMI_STRING_T    text = {0};
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    uint8           *str_ptr = PNULL;
//    uint16          str_len = 0;
//    wchar           wstr[35] = {0};
    GUI_RECT_T      client_rect = MMITHEME_GetClientRectEx(win_id);
    GUI_RECT_T      lab_rect = {0};
    uint16 cur_freq = 875;
    char   freq_number[21] = {0};
    MMIENGFM_CUR_INFO_T *cur_info_ptr = NULL;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        lab_rect = client_rect;
        lab_rect.bottom = (client_rect.bottom + client_rect.top)/2;
        GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);
        
        lab_rect.top =  lab_rect.bottom + 1;
        lab_rect.bottom = client_rect.bottom ;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
        
#ifndef _WIN32
        s_mmieng_headset_is_in = GPIO_CheckHeadsetStatus();
#endif

        if (s_mmieng_headset_is_in && (!MMIAPISET_GetFlyMode()))
        {
            MMIAPIFM_EngTestPlay();
        }
//         MMI_GetLabelTextByLang(TXT_FM, &text);
//         GUIWIN_SetTitleText(win_id, text.wstr_ptr , text.wstr_len, FALSE);
        break;
#ifndef MMI_RES_LOW_COST
    case MSG_LCD_SWITCH:
        lab_rect = client_rect;
        lab_rect.bottom = (client_rect.bottom + client_rect.top)/2;
        GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);

        lab_rect.top =  lab_rect.bottom + 1;
        lab_rect.bottom = client_rect.bottom ;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE);
        break;
#endif
    case MSG_FULL_PAINT:
        GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,
            IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
        str_ptr = (uint8*)"87.5MHz";
        GUILABEL_SetFont(MMIENG_UITEST_LABEL_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
        SetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, (char*)str_ptr, 35, FALSE);
//         str_len = strlen((char*)str_ptr);
//         MMI_STRNTOWSTR( wstr, 35, (char*)str_ptr, str_len, str_len);   /*lint !e64*/
//         text.wstr_ptr = wstr;
//         text.wstr_len = MMIAPICOM_Wstrlen(text.wstr_ptr);
//         GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, FALSE);
        if(MMIAPISET_GetFlyMode())
        {
            //in fly mode ,can not open FM
            str_ptr = (uint8*)"Fly Mode On !";
        }
#ifndef _WIN32
        else if (GPIO_CheckHeadsetStatus())
        {
            str_ptr = (uint8*)"Headset is in";
        }
#endif
        else
        {
            str_ptr = (uint8*)"Insert Headset!";
        }
        GUILABEL_SetFont(MMIENG_LABEL1_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
        SetLabelTextByCharArray(MMIENG_LABEL1_CTRL_ID, (char*)str_ptr, 35, FALSE);
//         str_len = strlen((char*)str_ptr);
//         MMI_STRNTOWSTR( wstr, 35, (char*)str_ptr, str_len, str_len);        /*lint !e64*/
//         text.wstr_ptr = wstr;
//         text.wstr_len = MMIAPICOM_Wstrlen(text.wstr_ptr);
//         GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, FALSE);

        break;

#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:
#endif
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
        MMIAPIFM_EngTestStop();
        SaveTestResult(win_id, 1);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(  win_id) ;          
        break;
        
    case MSG_KEYUP_HEADSET_BUTTON:          
        break;
        
    case MSG_KEYDOWN_HEADSET_BUTTON:            
        break;

    case MSG_KEYUP_HEADSET_DETECT:
        s_mmieng_headset_is_in = FALSE;
        str_ptr = (uint8*)"Insert Headset!";
        SetLabelTextByCharArray(MMIENG_LABEL1_CTRL_ID, (char*)str_ptr, 35, TRUE);
//        str_len = SCI_STRLEN((char*)str_ptr);
//        MMI_STRNTOWSTR( wstr, 35, (char*)str_ptr, str_len, str_len);            /*lint !e64*/
//        text.wstr_ptr = wstr;
//        text.wstr_len = MMIAPICOM_Wstrlen(text.wstr_ptr);
//        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
        MMIAPIFM_EngTestStop();
        break;

    case MSG_KEYDOWN_HEADSET_DETECT:
        s_mmieng_headset_is_in = TRUE;
        str_ptr = (uint8*)"Headset is in";
        SetLabelTextByCharArray(MMIENG_LABEL1_CTRL_ID, (char*)str_ptr, 35, TRUE);
//         str_len = SCI_STRLEN((char*)str_ptr);
//         MMI_STRNTOWSTR( wstr, 35,(char*)str_ptr, str_len, str_len);         /*lint !e64*/
//         text.wstr_ptr = wstr;
//         text.wstr_len = MMIAPICOM_Wstrlen(text.wstr_ptr);
//         GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, TRUE);
        MMIAPIFM_EngTestPlay();
        result = MMI_RESULT_FALSE;
        break;
        
        
    case MSG_KEYDOWN_CANCEL:          
    case MSG_CTL_CANCEL:            
        MMIAPIFM_EngTestStop();           
        SaveTestResult(win_id, 0);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(  win_id) ; 
        break;      
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif   
        }
        break;      
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:        
        MMIAPIFM_EngTestStop();  
        break;

    case MSG_APP_LEFT:
	    if(s_mmieng_headset_is_in)
		{
            if(MMIAPIFM_SearchValidFreq(-1))
            {
                MMIAPIFM_AdjustChannel(-1);
            }
			
            cur_info_ptr = MMIAPIFM_GetFMCurrentInfo();
            if(NULL != cur_info_ptr) 
            {
                cur_freq = cur_info_ptr->cur_freq;
                SCI_TRACE_LOW("cur_freq = %d", cur_freq);
            }
            sprintf(freq_number,"%d.%d %s",cur_freq/10, cur_freq%10, "MHz");
            SCI_TRACE_LOW("freq_number = %s", freq_number);
            GUILABEL_SetFont(MMIENG_UITEST_LABEL_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
            SetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, freq_number, 35, TRUE);
		}
        break;

    case MSG_APP_RIGHT:
	    if(s_mmieng_headset_is_in)
		{
            if(MMIAPIFM_SearchValidFreq(1))
           {
                MMIAPIFM_AdjustChannel(1);
            }
            cur_info_ptr = MMIAPIFM_GetFMCurrentInfo();
            if(NULL != cur_info_ptr) 
            {
                cur_freq = cur_info_ptr->cur_freq;
                SCI_TRACE_LOW("cur_freq = %d", cur_freq);
            }

            sprintf(freq_number,"%d.%d %s",cur_freq/10, cur_freq%10, "MHz");
            SCI_TRACE_LOW("freq_number = %s", freq_number);
            GUILABEL_SetFont(MMIENG_UITEST_LABEL_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
            SetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, freq_number, 35, TRUE);
		}
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif
/* BEGIN: Added by George.Liu 01526, 2010/7/12   PN:WiFi_UITest */
#ifdef MMI_ENG_WIFI_SUPPORT
/*****************************************************************************
Prototype    : MMITOOL_SetLabelText
Description  : 设置Lable文本
Input        : MMI_CTRL_ID_T  ctrl_id  
const uint8 *char_ptr   
BOOLEAN is_fresh        
Output       : None
Return Value : PUBLIC
Calls        : 
Called By    : 

  History        :
  1.Date         : 2010/7/14
  Author       : George.Liu 01526
  Modification : Created function
  
*****************************************************************************/
PUBLIC BOOLEAN MMITOOL_SetLabelText(MMI_CTRL_ID_T  ctrl_id, 
                                    const uint8 *char_ptr, BOOLEAN is_fresh)
{
    MMI_STRING_T  text = {0};
    uint32        char_len = 0;
    BOOLEAN       ret = FALSE;
    
    char_len  = SCI_STRLEN((char *)char_ptr);
    text.wstr_ptr = (wchar *)SCI_ALLOC_APP((char_len + 2) << 1);
    text.wstr_len = char_len;
    MMIAPICOM_StrToWstr(char_ptr, text.wstr_ptr);
    GUILABEL_SetFont(ctrl_id,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);
    ret = GUILABEL_SetText(ctrl_id, &text, is_fresh);

    SCI_FREE(text.wstr_ptr);
    return ret;
}

#ifdef UAL_WIFI_SUPPORT
LOCAL uint32 g_ual_wifi_test_cms_handle = NULL;
LOCAL BOOLEAN s_is_reopen_wifi_scan = FALSE;
/*****************************************************************************
Prototype    : MMITOOL_SetListText
Description  : 设置Lable文本
Input        : MMI_CTRL_ID_T  ctrl_id  
const uint8 *char_ptr   
BOOLEAN clear_appended_list        
Output       : None
Return Value : PUBLIC
Calls        : 
Called By    : 

  History        :
  1.Date         : 2022/10/15
  Author       : miao.xi
  Modification : Created function
  
*****************************************************************************/
PUBLIC void UiTestWifi_AddListText(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T  ctrl_id, 
                                    const uint8 *char_ptr1, const uint8 *char_ptr2,BOOLEAN clear_appended_list)
{
    MMI_STRING_T  text={0};
    MMI_STRING_T  text2={0};
    uint32        char1_len=0;
    uint32        char2_len=0;

    if(clear_appended_list)
    {
        GUILIST_RemoveAllItems( ctrl_id );
    }
    //text1
    char1_len  = SCI_STRLEN((char *)char_ptr1);
    text.wstr_ptr = (wchar *)SCI_ALLOC_APP((char1_len + 2) << 1);
    text.wstr_len = char1_len;
    MMIAPICOM_StrToWstr(char_ptr1, text.wstr_ptr);
    //text2
    char2_len  = SCI_STRLEN((char *)char_ptr2);
    text2.wstr_ptr = (wchar *)SCI_ALLOC_APP((char2_len + 2) << 1);
    text2.wstr_len = char2_len;
    MMIAPICOM_StrToWstr(char_ptr2, text2.wstr_ptr);
    //add
    WatchCOM_ListItem_Draw_2Str(ctrl_id,text,text2);
    //refresh
    MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
    SCI_FREE(text.wstr_ptr);
    SCI_FREE(text2.wstr_ptr);
    return;
}

/*****************************************************************************/
//  Description :  ui test start scan
//  Global resource dependence : none
//  Author: miao.xi
//  Note:
/*****************************************************************************/
LOCAL void UiTestWifiStartScan(void)
{
    uint8 i = 0;
    ual_wifi_scan_req_param_t scan_req_param = {0};

    SCI_TRACE_LOW("[ENG_UITEST] ENTER UiTestWifiStartScan");
    scan_req_param.scan_mode = UAL_WIFI_SCAN_BY_CHANNEL;
    scan_req_param.ual_wifi_scan_param_u.ual_wifi_scan_by_channle_t.nChNum = UAL_WIFI_CHANNEL_NUMBER;
    scan_req_param.ual_wifi_scan_param_u.ual_wifi_scan_by_channle_t.nMaxNodeNum = UAL_WIFI_AP_NODE_NUMBER;
    for(i = 0;i < UAL_WIFI_CHANNEL_NUMBER;i++)
    {
        scan_req_param.ual_wifi_scan_param_u.ual_wifi_scan_by_channle_t.nch[i] = i+1;
    }
    ual_wifi_start_scan(&scan_req_param);
    return;
}

/*****************************************************************************/
//  Description :  handle ual wifi msg
//  Global resource dependence : none
//  Author: miao.xi
//  Note:
/*****************************************************************************/
LOCAL boolean UiTestHandleUalWifimsg(ual_cms_msg_t param)
{
    MMI_WIN_ID_T winId = MMIENG_UITESTWIFI_WIN_ID;
    MMI_CTRL_ID_T listCtrlId =  MMIENG_UITEST_LISTBOX_CTRL_ID;
    ual_wifi_msg_e ual_wifi_msg = (ual_wifi_msg_e)param.msg_id;
    ual_wifi_scan_req_param_t scan_req_param = {0};


    if(PNULL == param.p_body)
    {
        SCI_TRACE_LOW("[ENG_UITEST] UiTestHandleUalWifimsg param.pbody is PNULL");
    }
    SCI_TRACE_LOW("[ENG_UITEST] msg id = %d", param.msg_id);
    switch(ual_wifi_msg)
    {
        case MSG_UAL_WIFI_OPEN_CNF :
        {
            ual_wifi_open_cnf_t ret = *(ual_wifi_open_cnf_t*)param.p_body;
            if(TRUE == ret.is_ok)
            {
                SCI_TRACE_LOW("[ENG_UITEST] UiTestHandleUalWifimsg open ok ,start scan");
                UiTestWifiStartScan();
            }
            else
            {
                UiTestWifi_AddListText(winId,listCtrlId,(uint8*)"[ENG_UITEST] WIFI Open fail","",TRUE);
            }
            break;
        }
        case MSG_UAL_WIFI_CLOSE_CNF :
        {
            if(TRUE == s_is_reopen_wifi_scan)
            {
                s_is_reopen_wifi_scan = FALSE;
#ifdef UAL_RF_AM_SUPPORT
                if(RF_AM_STATE_IDLE == ual_rf_am_get_current_status() || RF_AM_STATE_WIFISCAN_WORK == ual_rf_am_get_current_status())
                {
                    ual_wifi_open();
                }
                else
                {
                    GUILIST_RemoveAllItems( listCtrlId );
                    UiTestWifi_AddListText(winId,listCtrlId,(uint8*)"   rf status error","can't scan",TRUE);
                }
#else
                ual_wifi_open();
#endif
                SCI_TRACE_LOW("[ENG_UITEST] UiTestHandleUalWifimsg close wifi cnf,reopen wifi");
            }
            else
            {
                SCI_TRACE_LOW("[ENG_UITEST] UiTestHandleUalWifimsg close wifi cnf,close wifi");
            }
            break;
        }
        case MSG_UAL_WIFI_START_SCAN_CNF :
        {
            ual_wifi_start_scan_cnf_t ret = *(ual_wifi_start_scan_cnf_t*)param.p_body;
            if(TRUE == ret.is_ok)
            {
                GUILIST_RemoveAllItems( listCtrlId );//remove list,wait for ap_lsit node
                SCI_TRACE_LOW("[ENG_UITEST] UiTestHandleUalWifimsg start scan ok,wait for ap_list");
            }
            else
            {
                UiTestWifi_AddListText(winId,listCtrlId,(uint8*)"WIFI Start Scan fail","",TRUE);
            }
            break;
        }
        case MSG_UAL_WIFI_STOP_SCAN_CNF :
        {
            SCI_TRACE_LOW("[ENG_UITEST] UiTestHandleUalWifimsg stop scan cnf");
            break;
        }
        case MSG_UAL_WIFI_SCAN_AP_INFO_IND :
        {
            SCI_TRACE_LOW("[ENG_UITEST] UiTestHandleUalWifimsg scan ap_node");
            break;
        }
        case MSG_UAL_WIFI_SCAN_AP_INFO_CNF :
        {
            uint32 node_index = 0;
            ual_wifi_scan_ap_info_t show_ap_list[UAL_WIFI_AP_NODE_NUMBER] = {0};

            if(UAL_WIFI_RES_SUCCESS != ual_wifi_get_scaned_ap_list(show_ap_list,UAL_WIFI_AP_NODE_NUMBER))
            {
                SCI_TRACE_LOW("[ENG_UITEST]get ap_list wrong");
                break;
            }
            for(node_index = 0;node_index < UAL_WIFI_AP_NODE_NUMBER; node_index++)//cid607264
            {
                char bssid_char[UAL_WIFI_AP_NODE_BSSID_STR_LEN] = { 0 };
                char rssi_char[UAL_WIFI_AP_NODE_RSSI_STR_LEN] = { 0 };

                SCI_TRACE_LOW("[ENG_UITEST] UiTestHandleUalWifimsg scan ok,show ap_list node");
                if(0 == show_ap_list[node_index].rssival.rssival)
                {
                    continue;
                }
                sprintf( bssid_char, "MAC:%02x:%02x:%02x:%02x:%02x:%02x", show_ap_list[node_index].bssid_info.bssid[5], show_ap_list[node_index].bssid_info.bssid[4],
                        show_ap_list[node_index].bssid_info.bssid[3], show_ap_list[node_index].bssid_info.bssid[2],show_ap_list[node_index].bssid_info.bssid[1], show_ap_list[node_index].bssid_info.bssid[0] );
                sprintf( rssi_char, "rssi:%02d db",show_ap_list[node_index].rssival.rssival);

                UiTestWifi_AddListText(winId,listCtrlId,bssid_char,rssi_char,FALSE);
            }
            ual_wifi_stop_scan();
            SCI_TRACE_LOW("[ENG_UITEST] UiTestHandleUalWifimsg has scaned all ap_list node");
            break;
        }
        default:
        {
            SCI_TRACE_LOW("[ENG_UITEST] UiTestHandleUalWifimsg msg is wrong");
            return FALSE;
        }
    }
    return TRUE;
}
/*****************************************************************************/
//  Description :  ui test win open
//  Global resource dependence : none
//  Author: miao.xi
//  Note:
/*****************************************************************************/
LOCAL void UiTestWifiWinOpen(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T listCtrlId)
{
    GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
    GUI_RECT_T  lab_rect = {0};
    GUI_RECT_T  list_rect = {0};
    uint8 is_need_button = 0;

    if(PNULL != MMK_GetWinAddDataPtr(win_id))
    {
        is_need_button = *((uint8*)MMK_GetWinAddDataPtr(win_id));
        SCI_TRACE_LOW("[ENG_UITEST] MMK_GetWinAddDataPtr(win_id)=%x,is_need_button = %d",MMK_GetWinAddDataPtr(win_id),is_need_button);
    }
    if(PNULL == MMK_GetWinAddDataPtr(win_id) || TRUE == is_need_button)//不给参数或者参数为1，都创建button
    {
        MMITOOL_SetLabelText(MMIENG_UITEST_LABEL1_CTRL_ID,"PASS",FALSE);
        MMITOOL_SetLabelText(MMIENG_UITEST_LABEL2_CTRL_ID,"REOPEN",FALSE);
        MMITOOL_SetLabelText(MMIENG_UITEST_LABEL3_CTRL_ID,"FAIL",FALSE);

        lab_rect = client_rect;
        lab_rect.top = (client_rect.bottom + client_rect.top)/5*3;
        lab_rect.bottom= (client_rect.bottom + client_rect.top)/5*4;
        lab_rect.right = (client_rect.left+ client_rect.right)/3;
        lab_rect.left=(client_rect.left+ client_rect.right)/10;
        GUILABEL_SetRect(MMIENG_UITEST_LABEL1_CTRL_ID, &lab_rect, FALSE);
        lab_rect.left+=(client_rect.left+ client_rect.right)/4;
        lab_rect.right +=(client_rect.left+ client_rect.right)/3;
        GUILABEL_SetRect(MMIENG_UITEST_LABEL2_CTRL_ID, &lab_rect, FALSE);
        lab_rect.left+=(client_rect.left+ client_rect.right)/4;
        lab_rect.right +=(client_rect.left+ client_rect.right)/3;
        GUILABEL_SetRect(MMIENG_UITEST_LABEL3_CTRL_ID, &lab_rect, FALSE);

        GUILABEL_SetFont(MMIENG_UITEST_LABEL1_CTRL_ID,SONG_FONT_20,MMI_WHITE_COLOR);
        GUILABEL_SetFont(MMIENG_UITEST_LABEL2_CTRL_ID,SONG_FONT_20,MMI_WHITE_COLOR);
        GUILABEL_SetFont(MMIENG_UITEST_LABEL3_CTRL_ID,SONG_FONT_20,MMI_WHITE_COLOR);
    }
    else
    {
        MMK_DestroyControl(MMIENG_UITEST_LABEL1_CTRL_ID);
        MMK_DestroyControl(MMIENG_UITEST_LABEL2_CTRL_ID);
        MMK_DestroyControl(MMIENG_UITEST_LABEL3_CTRL_ID);
    }
    MMK_DestroyControl(MMIENG_UITEST_LABEL_CTRL_ID);
    list_rect = client_rect;
    if(PNULL == MMK_GetWinAddDataPtr(win_id) || TRUE == is_need_button)
    {
        list_rect.bottom = list_rect.top+(client_rect.bottom + client_rect.top)/2;
    }
    CTRLLIST_SetRect(MMIENG_UITEST_LISTBOX_CTRL_ID,&list_rect);

    GUILIST_RemoveAllItems( listCtrlId );
    GUILIST_SetMaxItem( listCtrlId, UAL_WIFI_AP_NODE_NUMBER, FALSE );
    GUILIST_SetListState( listCtrlId, GUILIST_STATE_TEXTSCROLL_ENABLE | GUILIST_STATE_AUTO_SCROLL, TRUE );//增加list自动滚动属性

    if(NULL == g_ual_wifi_test_cms_handle)
    {
        ual_wifi_register(UiTestHandleUalWifimsg,&g_ual_wifi_test_cms_handle);
        SCI_TRACE_LOW("[ENG_UITEST]creat wifi cms handle = %d",g_ual_wifi_test_cms_handle);
    }
    else
    {
        SCI_TRACE_LOW("[ENG_UITEST]Error:wifi cms handle is %d,not NULL",g_ual_wifi_test_cms_handle);
    }

    UiTestWifi_AddListText(win_id,listCtrlId,(uint8*)"WIFI searching...","",TRUE);
#ifdef UAL_RF_AM_SUPPORT
    if(RF_AM_STATE_IDLE == ual_rf_am_get_current_status() || RF_AM_STATE_WIFISCAN_WORK == ual_rf_am_get_current_status())
    {
        ual_wifi_open();
    }
    else
    {
        GUILIST_RemoveAllItems( listCtrlId );
        UiTestWifi_AddListText(win_id,listCtrlId,(uint8*)"   rf status error","can't scan",TRUE);
    }
#else
    ual_wifi_open();
#endif
    MMK_SetAtvCtrl(win_id,listCtrlId);
    return;
}

/*****************************************************************************
Prototype    : UITestWiFiWinHandleMsg
Description  : WiFi测试主窗口消息处理函数
Input        : MMI_WIN_ID_T win_id
MMI_MESSAGE_ID_E msg_id
DPARAM param
Output       : None
Return Value :
Calls        :
Called By    :

  History        :
  1.Date         : 2010/7/12
  Author       : George.Liu 01526
  Modification : Created function

*****************************************************************************/
PUBLIC MMI_RESULT_E UITestWiFiWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           listCtrlId =  MMIENG_UITEST_LISTBOX_CTRL_ID;
    ual_wifi_state_e        ual_wifi_state = 0;

    SCI_TRACE_LOW("[ENG_UITEST] UITestWiFiWinHandleMsg msg_id is 0x%x", msg_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            UiTestWifiWinOpen(win_id,listCtrlId);
            break;
        }

        case MSG_FULL_PAINT:
        {
            GUI_LCD_DEV_INFO     lcd_dev_info   = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
            GUI_RECT_T           BackGroundRect = MMITHEME_GetClientRectEx(win_id);
            LCD_FillRect(&lcd_dev_info, BackGroundRect, MMI_BLACK_COLOR);
            break;
        }
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        {
            ual_wifi_get_current_state(&ual_wifi_state);
            if(UAL_WIFI_STATE_IDLE != ual_wifi_state)
            {
                ual_wifi_close();
                SCI_TRACE_LOW("[ENG_UITEST] UITestWiFiWinHandleMsg pass");
            }
            SaveTestResult(win_id, 1);
            MMIAPIENG_RunUITestNextStep();
            MMK_CloseWin(win_id) ;
            break;
        }
        case MSG_NOTIFY_MIDSK:
        case MSG_KEYDOWN_WEB:
        {
            ual_wifi_get_current_state(&ual_wifi_state);
            if(UAL_WIFI_STATE_IDLE != ual_wifi_state)
            {
                ual_wifi_close();
                SCI_TRACE_LOW("[ENG_UITEST] UITestWiFiWinHandleMsg close wifi,then open");
            }
            s_is_reopen_wifi_scan = TRUE;
            SCI_TRACE_LOW("[ENG_UITEST] UITestWiFiWinHandleMsg start reopen wifi");
            break;
        }

        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            ual_wifi_get_current_state(&ual_wifi_state);
            if(UAL_WIFI_STATE_IDLE != ual_wifi_state)
            {
                ual_wifi_close();
            }
            SaveTestResult(win_id, 0);
            MMIAPIENG_RunUITestNextStep();
            MMK_CloseWin(win_id) ;
            break;

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
            {
                MMI_CTRL_ID_T labelctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

                SCI_TRACE_LOW("[ENG_UITEST] UITestWiFiWinHandleMsg labelctrl_id is %d", ((MMI_NOTIFY_T*) param)->src_id);
                SCI_TRACE_LOW("[ENG_UITEST] UITestWiFiWinHandleMsg MMIENG_UITEST_LABEL1_CTRL_ID is %d", MMIENG_UITEST_LABEL1_CTRL_ID);
                if( MMIENG_UITEST_LABEL1_CTRL_ID == labelctrl_id)
                {
                    MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                }
                else if( MMIENG_UITEST_LABEL3_CTRL_ID == labelctrl_id)
                {
                    MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                }
                else if( MMIENG_UITEST_LABEL2_CTRL_ID == labelctrl_id)
                {
                    MMK_SendMsg(win_id, MSG_APP_WEB, PNULL);
                }
            }
            break;
#endif

        case MSG_KEYDOWN_RED:
            break;
        case MSG_CLOSE_WINDOW:
                ual_wifi_get_current_state(&ual_wifi_state);
                if(UAL_WIFI_STATE_IDLE != ual_wifi_state)
                {
                    ual_wifi_close();
                }
                if(NULL != g_ual_wifi_test_cms_handle)
                {
                    SCI_TRACE_LOW("[ENG_UITEST]unregister wifi cms handle = %d",g_ual_wifi_test_cms_handle);
                    ual_wifi_unregister(g_ual_wifi_test_cms_handle);
                    g_ual_wifi_test_cms_handle = NULL;
                }

            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}

/*****************************************************************************/
//  Description : [IN]:is_not_need_button 不给参数或者参数为0，都创建button
//  Global resource dependence :
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenWifiScanWin(uint8 is_need_button)
{
    MMK_CreateWin((uint32*)MMIENG_UITESTWIFI_WIN_TAB, &is_need_button);
}

#else



/*****************************************************************************/
//  Description :  Wifi msg callback
//  Global resource dependence : none
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
LOCAL void UITestWifiCallback( MMIWIFI_NOTIFY_EVENT_E event_id, MMIWIFI_NOTIFY_PARAM_T* param_ptr )
{
    uint32          wifiNum = 0;
    BOOLEAN         result = FALSE;
    MMI_WIN_ID_T    winId = MMIENG_UITESTWIFI_WIN_ID;
    WIFISUPP_SSID_T wifiSsid = { 0 };

    if( PNULL == param_ptr )
    {
        SCI_TRACE_LOW("[ENG_UITEST] UITestWiFiWinHandleMsg param_ptr is PNULL!!");
        return;
    }

    if( TRUE != MMK_IsOpenWin(MMIENG_UITESTWIFI_WIN_ID) )
    {
        SCI_TRACE_LOW("[ENG_UITEST] UITestWiFiWinHandleMsg WIFI eng window not open!!");
        return;
    }

    SCI_TRACE_LOW("[ENG_UITEST] UITestWiFiWinHandleMsg event_id %d param_ptr->result %d", event_id, param_ptr->result);

    switch( event_id )
    {
        case MMIWIFI_NOTIFY_EVENT_ON:
        {
            if( param_ptr->result != WIFISUPP_RESULT_SUCC )
            {
                MMITOOL_SetLabelText( MMIENG_UITEST_LABEL_CTRL_ID, (uint8*)"WIFI On fail", TRUE );
                GUIWIN_SetSoftkeyTextId(winId, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RESET, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
            }
            break;
        }

        case MMIWIFI_NOTIFY_EVENT_SCAN_CNF:
        {
            if( param_ptr->result == WIFISUPP_RESULT_SUCC )
            {
                wifiNum = MMIAPIWIFI_GetSearchedAPNum();
                if( wifiNum > 0 )
                {
                    result = MMIAPIWIFI_GetSearchedSsidByIndex( 0, &wifiSsid );
                }
            }

            if( TRUE == result )
            {
                MMITOOL_SetLabelText(MMIENG_UITEST_LABEL_CTRL_ID, wifiSsid.ssid, TRUE);
                GUIWIN_SetSoftkeyTextId(winId, (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
            }
            else
            {
                MMITOOL_SetLabelText(MMIENG_UITEST_LABEL_CTRL_ID, (uint8*)"WIFI Search fail", TRUE);
                GUIWIN_SetSoftkeyTextId(winId, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RESET, (MMI_TEXT_ID_T)TXT_ENG_FAIL, TRUE);
            }

            break;
        }

        default:
            break;
    }
}

/*****************************************************************************/
//  Description :  Wifi test start
//  Global resource dependence : none
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
LOCAL void UITestWifiStart( MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id )
{
    BOOLEAN                 reCode = FALSE;
    MMIWIFI_NOTIFY_INFO_T   notifyInfo = { 0 };

    SCI_TRACE_LOW("[ENG_UITEST] UITestWifiStart");

    if( TRUE == MMIAPISET_GetFlyMode() )
    {
        MMITOOL_SetLabelText(ctrl_id, (uint8*)"Fly Mode On, Fail key to next step!", FALSE);
    }
    else
    {
        MMITOOL_SetLabelText(ctrl_id, (uint8*)"WIFI searching...", FALSE);
        GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);

        if( MMIAPIWIFI_GetStatus() != MMIWIFI_STATUS_OFF )
        {
            MMIAPIWIFI_Reset();
        }
        notifyInfo.module_id = MMI_MODULE_ENG;
        notifyInfo.notify_func = UITestWifiCallback;

        MMIAPIWIFI_RegNotifyInfo(&notifyInfo);
        reCode = MMIAPIWIFI_On( FALSE );
        if( FALSE == reCode )
        {
            SCI_TRACE_LOW("[ENG_UITEST] UITestWifiStart MMIAPIWIFI_On fail");
            MMIAPIWIFI_DeRegNotifyInfo( MMI_MODULE_ENG );
            MMITOOL_SetLabelText(ctrl_id, (uint8*)"WIFI open fail", FALSE);
            GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RESET, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);
        }
    }

    MMK_SetAtvCtrl(win_id,ctrl_id);
}

/*****************************************************************************
Prototype    : UITestWiFiWinHandleMsg
Description  : WiFi测试主窗口消息处理函数
Input        : MMI_WIN_ID_T win_id
MMI_MESSAGE_ID_E msg_id
DPARAM param
Output       : None
Return Value :
Calls        :
Called By    :

  History        :
  1.Date         : 2010/7/12
  Author       : George.Liu 01526
  Modification : Created function

*****************************************************************************/
PUBLIC MMI_RESULT_E UITestWiFiWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T           softkeyId[GUISOFTKEY_BUTTON_NUM] = {0};
    MMIWIFI_NOTIFY_INFO_T   notifyInfo = { 0 };

    SCI_TRACE_LOW("[ENG_UITEST] UITestWiFiWinHandleMsg msg_id is 0x%x", msg_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            UITestWifiStart( win_id, MMIENG_UITEST_LABEL_CTRL_ID );
            break;
        }

        case MSG_FULL_PAINT:
        {
            GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
            GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,
                IMAGE_COMMON_BG, MMITHEME_GetDefaultLcdDev());
            break;
        }

#if defined MMI_PDA_SUPPORT
        case MSG_APP_MENU:
#endif
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        {
            GUIWIN_GetSoftkeyTextId(win_id, &softkeyId[0], &softkeyId[1], &softkeyId[2]);

            if( (MMI_TEXT_ID_T)TXT_NULL == softkeyId[0] )
            {
                SCI_TRACE_LOW("[ENG_UITEST] UITestWiFiWinHandleMsg not ok status!!");
                break;
            }

            if (TRUE == MMIAPISET_GetFlyMode())
            {
                MMITOOL_SetLabelText(MMIENG_UITEST_LABEL_CTRL_ID, (uint8*)"Fly Mode On, Fail key to next step!", FALSE);
            }
            else
            {
                if (MMIAPIWIFI_GetStatus() != MMIWIFI_STATUS_OFF)
                {
                    MMIAPIWIFI_Reset();
                }
                MMIAPIWIFI_DeRegNotifyInfo( MMI_MODULE_ENG );
                SaveTestResult( win_id, 1);
                MMIAPIENG_RunUITestNextStep();
                MMK_CloseWin(  win_id) ;
            }

            break;
        }
        case MSG_NOTIFY_MIDSK:
        case MSG_KEYDOWN_WEB:
        {
            GUIWIN_GetSoftkeyTextId(win_id, &softkeyId[0], &softkeyId[1], &softkeyId[2]);

            if( (MMI_TEXT_ID_T)TXT_NULL != softkeyId[1] )
            {
                UITestWifiStart( win_id, MMIENG_UITEST_LABEL_CTRL_ID );
            }
            else
            {
                SCI_TRACE_LOW("[ENG_UITEST] UITestWiFiWinHandleMsg not reset status!!");
            }

            break;
        }

        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            if (MMIAPIWIFI_GetStatus() != MMIWIFI_STATUS_OFF)
            {
                MMIAPIWIFI_Reset();
            }
            MMIAPIWIFI_DeRegNotifyInfo( MMI_MODULE_ENG );
            SaveTestResult(win_id, 0);
            MMIAPIENG_RunUITestNextStep();
            MMK_CloseWin(win_id) ;
            break;

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
            {
#if defined MMI_PDA_SUPPORT
                MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
                if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
                {
                    MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                }
                else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
                {
                    MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                }
                else if( MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ctrl_id)
                {
                    MMK_SendMsg(win_id, MSG_APP_WEB, PNULL);
                }
#endif
            }
            break;
#endif

        case MSG_KEYDOWN_RED:
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}

#endif
#endif

/* END:   Added by George.Liu 01526, 2010/7/12   PN:WiFi_UITest */

#ifdef GPS_SUPPORT
typedef enum
{
    GPS_STARTING,
    GPS_START_FAIL,
    GPS_STAT_SUC,
    GPS_START_STATE_MAX
}GPS_START_STATE_E;
LOCAL uint8  g_gps_readInfo_timer_id   = 0;
LOCAL BOOLEAN g_isResponsePassKey = FALSE;
#ifdef GPS_SUPPORT
LOCAL uint32 g_gnss_handle = 0;
#endif
/*****************************************************************************/
//  Description : fill one line content to listbox
//  param:IN :ctrl_id
//  param:IN : gps_state, 1.starting gps, 2. start gps fail
//  Author: xiuyun.wang
//  RETRUN:
//  Note:
/*****************************************************************************/
LOCAL void UITestGPS_AppendONELINEItemContent(MMI_CTRL_ID_T current_ctrl_id, GPS_START_STATE_E gps_state)
{
    GUILIST_ITEM_T          item_t    = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};
    wchar temp[30]  = {'G', 'P', 'S', ' ','s','t','a','r','t',' ','a','n','d',' ','s','e','a','r','c','h','i','n','g',' ', '.','.','.',0};
    wchar start_fail[20]  = {'G', 'P', 'S', ' ','s','t','a','r','t',' ','f','a','i','l','e','d','!',0};

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    SCI_TRACE_LOW("[ENG_UITEST] [GPS]enter UITestGPS_AppendONELINEItemContent,gps_state:%d",gps_state);

    switch(gps_state)
    {
        case GPS_STARTING:
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(temp);   //satellite zhi
            SCI_TRACE_LOW("[ENG_UITEST][GPS] %ls\n",temp);
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = temp;
            break;
        }
        case GPS_START_FAIL:
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(start_fail);   //satellite zhi
            SCI_TRACE_LOW("[ENG_UITEST][GPS] %ls\n",start_fail);
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = start_fail;
            break;
        }
        case GPS_STAT_SUC:
            break;
        default:
            break;
    }
    GUILIST_AppendItem(current_ctrl_id,&item_t);
    GUIWIN_SetSoftkeyTextId(MMIENG_UITEST_GPS_WIN_ID, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);
    g_isResponsePassKey = FALSE;
}

#define GNSS_TEST_GPS_SATELLITE_PRN_MIN   1
#define GNSS_TEST_GPS_SATELLITE_PRN_MAX   32
#define GNSS_TEST_BEIDOU_SATELLITE_PRN_MIN   201
#define GNSS_TEST_BEIDOU_SATELLITE_PRN_MAX   264
#define GNSS_TEST_GALIEO_SATELLITE_PRN_MIN   95
#define GNSS_TEST_GALIEO_SATELLITE_PRN_MAX   130
#define GNSS_TEST_GLONASS_SATELLITE_PRN_MIN   65
#define GNSS_TEST_GLONASS_SATELLITE_PRN_MAX   92

/*****************************************************************************/
//  Description : fill content to listbox
//  param:IN ctrl_id,satellite_info
//  param: IN gpsSatelliteInfo:the gps information for displaying in window
//  Author: xiuyun.wang
//  RETRUN:
//  Note:
/*****************************************************************************/
#ifdef UAL_GNSS_SUPPORT
LOCAL void UITestGPS_AppendMutilLINEItemContent(MMI_CTRL_ID_T ctrl_id,ual_gnss_read_info_cnf_t*gpsSatelliteInfo)
{
    GUILIST_ITEM_T item_info = {0};
    GUILIST_ITEM_DATA_T     item_data= {0};

    uint8 tmp_item[GUILIST_STRING_MAX_NUM] = {0};
    wchar tmp_wstr_1[GUILIST_STRING_MAX_NUM] = {0};
    uint8 i = 0;
    uint8 GPS_Num = 0;
    uint8 BD_Num = 0;
    uint8 GALIEO_Num = 0;
    uint8 GLONASS_Num = 0;
    wchar temp_gps[4] = {'G','P','S',0};
    wchar temp_bd[7] = {'B','e','i','d','o','u',0};
    wchar temp_galieo[7] = {'G','a','l','i','e','o',0};
    wchar temp_glonass[8] = {'G','l','o','n','a','s','s',0};
    ENG_GNSS_SATELLITE_TYPE_E  satellite_type = GNSS_SATELLITE_TYPE_MAX;
    wchar wstr_gps_num[50] = {0};
    wchar wstr_bd_num[50] = {0};
    wchar wstr_galieo_num[50] = {0};
    wchar wstr_glonass_num[50] = {0};
    MMI_STRING_T sat_num_str = {0};
    MMI_STRING_T position_ok = {0};
    MMI_STRING_T position_fail = {0};
    uint8 str_position_ok[GUILIST_STRING_MAX_NUM] = {0};
    uint8 str_position_fail[GUILIST_STRING_MAX_NUM] = {0};
    uint8 str_gps_num[GUILIST_STRING_MAX_NUM] = {0};
    uint8 str_bd_num[GUILIST_STRING_MAX_NUM] = {0};
    uint8 str_galieo_num[GUILIST_STRING_MAX_NUM] = {0};
    uint8 str_glonass_num[GUILIST_STRING_MAX_NUM] = {0};
    wchar wstr_position_result[50] = {0};

    if(PNULL == gpsSatelliteInfo)
    {
        SCI_TRACE_LOW("[ENG_UITEST] [GPS]: AppendSatelliteListItem, gpsSatelliteInfo is null!!!\n");
        return;
    }
    SCI_TRACE_LOW("[ENG_UITEST] [GPS]: AppendSatelliteListItem\n");
    GUILIST_RemoveAllItems(MMIENG_LISTBOX_CTRL_ID);
    GUILIST_SetListState(MMIENG_LISTBOX_CTRL_ID,GUILIST_STATE_FOCUS,FALSE);
    SCI_TRACE_LOW("[ENG_UITEST] [GPS]: AppendSatelliteListItem longitude %f \n",gpsSatelliteInfo->gps_info.longitude);
    SCI_TRACE_LOW("[ENG_UITEST] [GPS]: AppendSatelliteListItem satellite num %d\n",gpsSatelliteInfo->satellite_list_info.satellite_num);

    item_info.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_info.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    //未定位/已经定位
    if(0 == gpsSatelliteInfo->gps_info.ttff_time)
    {
        SCI_TRACE_LOW("[ENG_UITEST]no position");
        MMI_GetLabelTextByLang(TXT_ENG_NO_POSITION,&position_fail);
        sprintf(tmp_item, "  %s:%d", "TTFF", gpsSatelliteInfo->gps_info.ttff_time);
        MMI_STRNTOWSTR(tmp_wstr_1,
            strlen((char*)tmp_item),
            tmp_item,
            strlen((char*)tmp_item),
            strlen((char*)tmp_item));
        MMIAPICOM_Wstrncat(wstr_position_result,position_fail.wstr_ptr,position_fail.wstr_len);
        MMIAPICOM_Wstrncat(wstr_position_result,tmp_wstr_1,strlen(tmp_item));
        item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_position_result);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_position_result;
    }
    else
    {
        SCI_TRACE_LOW("[ENG_UITEST]have positioned");
        MMI_GetLabelTextByLang(TXT_ENG_HAVE_POSITIONED,&position_ok);
        sprintf(tmp_item, "  %s:%d", "TTFF", gpsSatelliteInfo->gps_info.ttff_time);
        MMI_STRNTOWSTR(tmp_wstr_1,
            strlen((char*)tmp_item),
            tmp_item,
            strlen((char*)tmp_item),
            strlen((char*)tmp_item));
        MMIAPICOM_Wstrncat(wstr_position_result,position_ok.wstr_ptr,position_ok.wstr_len);
        MMIAPICOM_Wstrncat(wstr_position_result,tmp_wstr_1,strlen(tmp_item));
        item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_position_result);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_position_result;
    }
    GUILIST_AppendItem(ctrl_id,&item_info);

    //纬度 经度
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    sprintf(tmp_item, "%s:%f  %s:%f", "N", gpsSatelliteInfo->gps_info.latitude,"E", gpsSatelliteInfo->gps_info.longitude);
    MMI_STRNTOWSTR( tmp_wstr_1,
        strlen((char*)tmp_item),
        tmp_item,
        strlen((char*)tmp_item),
        strlen((char*)tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = strlen(tmp_item);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_wstr_1;
    GUILIST_AppendItem(ctrl_id,&item_info);

    //append satellite info
    for(i=0;i<gpsSatelliteInfo->satellite_list_info.satellite_num;i++)
    {
        //GPS星座
        if(gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier >= GNSS_TEST_GPS_SATELLITE_PRN_MIN &&
            gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier <= GNSS_TEST_GPS_SATELLITE_PRN_MAX)
        {
            satellite_type = GNSS_SATELLITE_TYPE_GPS;
            GPS_Num += 1;
        }
        //北斗星座
        else if(gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier >= GNSS_TEST_BEIDOU_SATELLITE_PRN_MIN &&
            gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier <= GNSS_TEST_BEIDOU_SATELLITE_PRN_MAX)
        {
            satellite_type = GNSS_SATELLITE_TYPE_BEIDOU;
            BD_Num += 1;
        }
        //Galieo星座
        else if(gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier >= GNSS_TEST_GALIEO_SATELLITE_PRN_MIN &&
            gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier <= GNSS_TEST_GALIEO_SATELLITE_PRN_MAX)
        {
            satellite_type = GNSS_SATELLITE_TYPE_GALIEO;
            GALIEO_Num += 1;
        }
        //GLONASS星座
        else if(gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier >= GNSS_TEST_GLONASS_SATELLITE_PRN_MIN &&
            gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier <= GNSS_TEST_GLONASS_SATELLITE_PRN_MAX)
        {
            satellite_type = GNSS_SATELLITE_TYPE_GLONASS;
            GLONASS_Num += 1;
        }
        // Satellite Identifier  and CN0
        SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
        SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
        switch(satellite_type)
        {
            case GNSS_SATELLITE_TYPE_GPS:
                sprintf(tmp_item, "%s [%d]:%d", "GPS", gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier,gpsSatelliteInfo->satellite_list_info.satellite_info[i].cn0);
                break;
            case GNSS_SATELLITE_TYPE_BEIDOU:
                sprintf(tmp_item, "%s [%d]:%d", "Beidou", gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier,gpsSatelliteInfo->satellite_list_info.satellite_info[i].cn0);
                break;
            case GNSS_SATELLITE_TYPE_GALIEO:
                sprintf(tmp_item, "%s [%d]:%d", "Galieo", gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier,gpsSatelliteInfo->satellite_list_info.satellite_info[i].cn0);
                break;
            case GNSS_SATELLITE_TYPE_GLONASS:
                sprintf(tmp_item, "%s [%d]:%d", "Glonass", gpsSatelliteInfo->satellite_list_info.satellite_info[i].satellite_identifier,gpsSatelliteInfo->satellite_list_info.satellite_info[i].cn0);
                break;
            default:
                SCI_TRACE_LOW("[ENG_UITEST] [GPS]:other satellite type");
                break;
        }
        MMI_STRNTOWSTR(tmp_wstr_1,strlen((char*)tmp_item),tmp_item,strlen((char*)tmp_item),strlen((char*)tmp_item));
        item_data.item_content[0].item_data.text_buffer.wstr_len = strlen(tmp_item);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_wstr_1;
        GUILIST_InsertItem(ctrl_id, &item_info,i+6);
        SCI_TRACE_LOW("[ENG_UITEST] [GPS]: AppendSatelliteListItem %s\n",tmp_item);
    }

    //GPS卫星数量
    SCI_TRACE_LOW("[ENG_UITEST] [GPS]: GPS satellite num:%d\n",GPS_Num);
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    MMIAPICOM_Wstrncat(wstr_gps_num,temp_gps,MMIAPICOM_Wstrlen(temp_gps));
    MMI_GetLabelTextByLang(TXT_ENG_SATELLITE_NUM,&sat_num_str);
    MMIAPICOM_Wstrncat(wstr_gps_num,sat_num_str.wstr_ptr,sat_num_str.wstr_len);
    sprintf(tmp_item, ":%d", GPS_Num);
    MMI_STRNTOWSTR(tmp_wstr_1,strlen((char*)tmp_item),tmp_item,strlen((char*)tmp_item),strlen((char*)tmp_item));
    MMIAPICOM_Wstrncat(wstr_gps_num,tmp_wstr_1,strlen(tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_gps_num);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_gps_num;
    GUILIST_InsertItem(ctrl_id, &item_info,2);

    //北斗卫星数量
    SCI_TRACE_LOW("[ENG_UITEST] [GPS]: Beidou satellite num:%d\n",BD_Num);
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    MMIAPICOM_Wstrncat(wstr_bd_num,temp_bd,MMIAPICOM_Wstrlen(temp_bd));
    MMIAPICOM_Wstrncat(wstr_bd_num,sat_num_str.wstr_ptr,sat_num_str.wstr_len);
    sprintf(tmp_item, ":%d", BD_Num);
    MMI_STRNTOWSTR(tmp_wstr_1,strlen((char*)tmp_item),tmp_item,strlen((char*)tmp_item),strlen((char*)tmp_item));
    MMIAPICOM_Wstrncat(wstr_bd_num,tmp_wstr_1,strlen(tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_bd_num);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_bd_num;
    GUILIST_InsertItem(ctrl_id, &item_info,3);

    //Galieo卫星数量
    SCI_TRACE_LOW("[ENG_UITEST] [GPS]: Galieo satellite num:%d\n",GALIEO_Num);
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    MMIAPICOM_Wstrncat(wstr_galieo_num,temp_galieo,MMIAPICOM_Wstrlen(temp_galieo));
    MMIAPICOM_Wstrncat(wstr_galieo_num,sat_num_str.wstr_ptr,sat_num_str.wstr_len);
    sprintf(tmp_item, ":%d", GALIEO_Num);
    MMI_STRNTOWSTR(tmp_wstr_1,strlen((char*)tmp_item),tmp_item,strlen((char*)tmp_item),strlen((char*)tmp_item));
    MMIAPICOM_Wstrncat(wstr_galieo_num,tmp_wstr_1,strlen(tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_galieo_num);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_galieo_num;
    GUILIST_InsertItem(ctrl_id, &item_info,4);

    //GLONASS卫星数量
    SCI_TRACE_LOW("[ENG_UITEST] [GPS]: Glonass satellite num:%d\n",GLONASS_Num);
    SCI_MEMSET(tmp_wstr_1,0,sizeof(tmp_wstr_1));
    SCI_MEMSET(tmp_item,0,sizeof(tmp_item));
    MMIAPICOM_Wstrncat(wstr_glonass_num,temp_glonass,MMIAPICOM_Wstrlen(temp_glonass));
    MMIAPICOM_Wstrncat(wstr_glonass_num,sat_num_str.wstr_ptr,sat_num_str.wstr_len);
    sprintf(tmp_item, ":%d", GLONASS_Num);
    MMI_STRNTOWSTR(tmp_wstr_1,strlen((char*)tmp_item),tmp_item,strlen((char*)tmp_item),strlen((char*)tmp_item));
    MMIAPICOM_Wstrncat(wstr_glonass_num,tmp_wstr_1,strlen(tmp_item));
    item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(wstr_glonass_num);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = wstr_glonass_num;
    GUILIST_InsertItem(ctrl_id, &item_info,5);


    GUIWIN_SetSoftkeyTextId(MMIENG_UITEST_GPS_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_PASS, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_FAIL, FALSE);
    g_isResponsePassKey = TRUE;
    MMK_PostMsg(MMIENG_UITEST_GPS_WIN_ID,MSG_FULL_PAINT,PNULL,0);
}
#endif
LOCAL void Gps_HandleReadInfoReqTimer(uint8 timer_id,uint32 param)
{
    //MNGPS_ReadInfoReq(0);
#ifdef UAL_GNSS_SUPPORT
    ual_gnss_read_info();
#endif
    SCI_TRACE_LOW("[ENG_UITEST] [GPS]:readInfoReq in timer\n");
}

LOCAL void Gps_StartReadInfoReqTimer(void)
{
    if (0 != g_gps_readInfo_timer_id)
    {
        MMK_StopTimer(g_gps_readInfo_timer_id);
        g_gps_readInfo_timer_id = 0;
    }
    SCI_TRACE_LOW("[ENG_UITEST] [GPS]:Gps_StartReadInfoReqTimer\n");
    g_gps_readInfo_timer_id = MMK_CreateTimerCallback(1000, Gps_HandleReadInfoReqTimer, 0,TRUE);
}
#if 0
LOCAL BOOLEAN UITestGPSCallback(APP_MN_GPS_SIGNAL_E gps_sig_id,void* gps_info)
{
    BOOLEAN result = TRUE;
    APP_MN_GPS_READ_INFO_CNF_T read_gpsinfo = {0};
    int i =0;
    char test_str[256] = {0};
    SCI_TRACE_LOW("[ENG_UITEST] [GPS]UITestGPSCallback gps_sig_id:%d\n",gps_sig_id);
    switch ( gps_sig_id )
    {
        case APP_MN_GPS_INIT_CNF:
        {
            APP_MN_GPS_INIT_CNF_T *pInitcnf = (APP_MN_GPS_INIT_CNF_T *)gps_info;
            MMIGPS_START_GPS_PARAM_T start_param = {0};
            if(PNULL == pInitcnf)
            {
                SCI_TRACE_LOW("[ENG_UITEST] [GPS]:APP_MN_GPS_INIT_CNF param is null\n");
                break;
            }
            if(TRUE == pInitcnf->is_ok)
            {
                SCI_TRACE_LOW("[ENG_UITEST] [GPS]:gps init succ");
                //start gps
                start_param.request_type = MMIGPS_REQUEST_TYPE_ENG;
                start_param.start_type = MMIGPS_START_TYPE_NONE;
                start_param.start_mode = 0;
                MMIGPS_Start(&start_param,UITestGPSCallback);
            }
            break;
        }
        case APP_MN_GPS_START_CNF:
        {
            //CALL report req
            APP_MN_GPS_START_CNF_T *pStartInfo = (APP_MN_GPS_START_CNF_T *)gps_info;
            if (NULL == pStartInfo)
            {
                SCI_TRACE_LOW("[ENG_UITEST] [GPS]UITestGPSCallback APP_MN_GPS_START_CNF,pStartInfo is null \n");
                return FALSE;
            }
            if(TRUE == pStartInfo->is_ok)
            {
                MNGPS_ReadInfoReq(0);   //request gps and location info
                Gps_StartReadInfoReqTimer();
                SCI_TRACE_LOW("[ENG_UITEST] [GPS]UITestGPSCallback APP_MN_GPS_START_CNF is OK\n");

            }else
            {
                SCI_TRACE_LOW("[ENG_UITEST] [GPS]UITestGPSCallback APP_MN_GPS_START_CNF is NOT OK\n");
                UITestGPS_AppendONELINEItemContent(MMIENG_LISTBOX_CTRL_ID,GPS_START_FAIL);
            }
            break;
        }
        case APP_MN_GPS_READ_INFO_CNF:
        {
            APP_MN_GPS_READ_INFO_CNF_T *param_ptr = (APP_MN_GPS_READ_INFO_CNF_T *)gps_info;
            SCI_TRACE_LOW("[ENG_UITEST][GPS]UITestGPSCallback APP_MN_GPS_READ_INFO_CNF\n");
            if(PNULL == param_ptr)
            {
                SCI_TRACE_LOW("[ENG_UITEST] [GPS]UITestGPSCallback APP_MN_GPS_READ_INFO_CNF  gps_info is null\n");
                return FALSE;
            }
            read_gpsinfo.gps_info.longitude = param_ptr->gps_info.longitude;
            read_gpsinfo.gps_info.latitude  = param_ptr->gps_info.latitude;
            read_gpsinfo.gps_info.altitude = param_ptr->gps_info.altitude;
            read_gpsinfo.gps_info.ttff_time = param_ptr->gps_info.ttff_time;
            read_gpsinfo.gps_info.satellite_num = param_ptr->gps_info.satellite_num;
            SCI_TRACE_LOW("[ENG_UITEST] [GPS]read_gpsinfo.gps_ReadInfo.gps_info.longitude = %d ",read_gpsinfo.gps_info.longitude);
            SCI_TRACE_LOW("[ENG_UITEST] [GPS]read_gpsinfo.gps_ReadInfo.gps_info.latitude = %d ",read_gpsinfo.gps_info.latitude);
            SCI_TRACE_LOW("[ENG_UITEST] [GPS]read_gpsinfo.gps_ReadInfo.gps_info.altitude = %d ",read_gpsinfo.gps_info.altitude);
            SCI_TRACE_LOW("[ENG_UITEST] [GPS]read_gpsinfo.gps_ReadInfo.gps_info.ttff_time = %d ",read_gpsinfo.gps_info.ttff_time);
            SCI_TRACE_LOW("[ENG_UITEST] [GPS]read_gpsinfo.gps_ReadInfo.gps_info.satellite_num = %d ",read_gpsinfo.gps_info.satellite_num);

            read_gpsinfo.satellite_list_info.satellite_num = param_ptr->satellite_list_info.satellite_num;
            SCI_MEMCPY(read_gpsinfo.satellite_list_info.satellite_info, param_ptr->satellite_list_info.satellite_info, sizeof(param_ptr->satellite_list_info.satellite_info));
            SCI_TRACE_LOW("[ENG_UITEST] [GPS]read_gpsinfo.satellite_list_info.satellite_num = %d ",read_gpsinfo.satellite_list_info.satellite_num);
            for(i=0; i<read_gpsinfo.satellite_list_info.satellite_num; i++)
            {
                sprintf(test_str, "%s: APP_MN_GPS_READ_INFO_CNF satellite info %d,%d,%d,%d,%d", "read_gpsinfo satellite info:",
                        read_gpsinfo.satellite_list_info.satellite_info[i].m_nSatelliteIdentifier,
                        read_gpsinfo.satellite_list_info.satellite_info[i].m_cn0,
                        read_gpsinfo.satellite_list_info.satellite_info[i].m_nElevation,  param_ptr->satellite_list_info.satellite_info[i].m_nAzimuth,
                        read_gpsinfo.satellite_list_info.satellite_info[i].m_IsUsed);
                SCI_TRACE_LOW("[ENG_UITEST] [GPS]%s",test_str);
            }
            UITestGPS_AppendMutilLINEItemContent(MMIENG_LISTBOX_CTRL_ID, &read_gpsinfo);
             break;
        }
        case APP_MN_GPS_STOP_CNF:
        {
            APP_MN_GPS_STOP_CNF_T *pStopcnf = (APP_MN_GPS_STOP_CNF_T *)gps_info;
            if(PNULL == pStopcnf)
            {
                SCI_TRACE_LOW("[ENG_UITEST] [GPS]: APP_MN_GPS_SATELLITE_INFO_IND  gps_info is null\n");
                return FALSE;
            }
            if (0 != g_gps_readInfo_timer_id)
            {
                MMK_StopTimer(g_gps_readInfo_timer_id);
                g_gps_readInfo_timer_id = 0;
            }

            //power off gps
            MMIGPS_Close();

            SCI_TRACE_LOW("[ENG_UITEST] [GPS]:APP_MN_GPS_STOP_CNF is OK\n");
            break;
        }

        default:
        {
            result = FALSE;
            SCI_TRACE_LOW("[ENG_UITEST] [GPS]: Unknow msg id!\n");
            break;
        }
    }
    return result;
}
#endif
#ifdef UAL_GNSS_SUPPORT
LOCAL boolean gnss_callback(ual_cms_msg_t param)
{
    boolean  ret    = TRUE;
    uint32   msg_id = 0;
    SCI_TRACE_LOW("[ENG GPS]:Enter\n");
    msg_id = param.msg_id;
    SCI_TRACE_LOW("[ENG GPS]:msg_id:%d\n",msg_id);
    switch(msg_id)
    {
        case MSG_UAL_GNSS_START_CNF:
        {
            //CALL report req
            ual_gnss_start_cnf_t *pStartInfo = (ual_gnss_start_cnf_t *)(param.p_body);
            if (NULL == pStartInfo)
            {
                SCI_TRACE_LOW("[ENG_UITEST] [GPS]UITestGPSCallback APP_MN_GPS_START_CNF,pStartInfo is null \n");
                return FALSE;
            }
            if(TRUE == pStartInfo->is_ok)
            {
                ual_gnss_read_info();   //request gps and location info
                Gps_StartReadInfoReqTimer();
                SCI_TRACE_LOW("[ENG_UITEST] [GPS]UITestGPSCallback APP_MN_GPS_START_CNF is OK\n");

            }
            else
            {
                SCI_TRACE_LOW("[ENG_UITEST] [GPS]UITestGPSCallback APP_MN_GPS_START_CNF is NOT OK\n");
                UITestGPS_AppendONELINEItemContent(MMIENG_LISTBOX_CTRL_ID,GPS_START_FAIL);
            }
            break;
        }
        case MSG_UAL_GNSS_READ_INFO_CNF:
        {
            ual_gnss_read_info_cnf_t*  param_ptr = (ual_gnss_read_info_cnf_t*)(param.p_body);
            ual_gnss_read_info_cnf_t   read_gpsinfo = {0};
            int                        i=0;
            char                       test_str[256] = {0};
            SCI_TRACE_LOW("[ENG_UITEST][GPS]:gnss_callback MSG_UAL_GNSS_READ_INFO_CNF\n");
            if(PNULL == param_ptr)
            {
                SCI_TRACE_LOW("[ENG_UITEST][GPS]: MSG_UAL_GNSS_READ_INFO_CNF  gps_info is null\n");
                ret = FALSE;
                break;
            }
            read_gpsinfo.gps_info.ttff_time = param_ptr->gps_info.ttff_time;
            read_gpsinfo.gps_info.longitude = param_ptr->gps_info.longitude;
            read_gpsinfo.gps_info.latitude = param_ptr->gps_info.latitude;
            read_gpsinfo.gps_info.altitude = param_ptr->gps_info.altitude;
            SCI_TRACE_LOW("[ENG_UITEST][GPS]ttff:%d,longtitude:%d,latitude:%d,altitude:%d",
                read_gpsinfo.gps_info.ttff_time,(int)(read_gpsinfo.gps_info.longitude),
                (int)(read_gpsinfo.gps_info.latitude),(int)(read_gpsinfo.gps_info.altitude));
            read_gpsinfo.satellite_list_info.satellite_num = param_ptr->satellite_list_info.satellite_num;
            SCI_MEMCPY(read_gpsinfo.satellite_list_info.satellite_info,
                param_ptr->satellite_list_info.satellite_info,
                sizeof(param_ptr->satellite_list_info.satellite_info));
            SCI_TRACE_LOW("[ENG_UITEST][GPS]read_gpsinfo.satellite_list_info.satellite_num = %d ",read_gpsinfo.satellite_list_info.satellite_num);
            for(i=0; i<read_gpsinfo.satellite_list_info.satellite_num; i++)
            {
                SCI_TRACE_LOW("[ENG_UITEST] [GPS]MSG_UAL_GNSS_READ_INFO_CNF satellite info %d,%d,%d,%d,%d", "read_gpsinfo satellite info:",
                        read_gpsinfo.satellite_list_info.satellite_info[i].satellite_identifier,
                        read_gpsinfo.satellite_list_info.satellite_info[i].cn0,
                        read_gpsinfo.satellite_list_info.satellite_info[i].elevation,  param_ptr->satellite_list_info.satellite_info[i].azimuth,
                        read_gpsinfo.satellite_list_info.satellite_info[i].is_used);
            }
            UITestGPS_AppendMutilLINEItemContent(MMIENG_LISTBOX_CTRL_ID, &read_gpsinfo);
            break;
        }
        case MSG_UAL_GNSS_STOP_CNF:
        {
            ual_gnss_stop_cnf_t* pStopcnf = (ual_gnss_stop_cnf_t*)(param.p_body);
            if(PNULL == pStopcnf)
            {
                SCI_TRACE_LOW("[ENG_UITEST][GPS]: MSG_UAL_GNSS_STOP_CNF  gps_info is null\n");
                ret = FALSE;
                break;
            }
            SCI_TRACE_LOW("[ENG_UITEST][GPS]:MSG_UAL_GNSS_STOP_CNF \n");
            if (0 != g_gps_readInfo_timer_id)
            {
                MMK_StopTimer(g_gps_readInfo_timer_id);
                g_gps_readInfo_timer_id = 0;
            }

            //power off gps
            //MMIGPS_Close();
            //ual_gnss_poweroff();
            ual_gnss_unregister(g_gnss_handle);
            break;
        }
        case MSG_UAL_GNSS_FIX_IND:
        {
            ual_gnss_fix_ind_t* pFixInd = (ual_gnss_fix_ind_t*)(param.p_body);
            if(PNULL == pFixInd)
            {
                SCI_TRACE_LOW("[ENG_UITEST][GPS]: MSG_UAL_GNSS_FIX_IND info is null\n");
                ret = FALSE;
                break;
            }
            //UITestGPS_AppendONELINEItemContent(MMIENG_LISTBOX_CTRL_ID,GPS_STAT_SUC,pFixInd);
            break;
        }
        default:
            SCI_TRACE_LOW("[ENG_UITEST] [GPS]: Unknow msg id!\n");
            ret = FALSE;
            break;
    }
    SCI_TRACE_LOW("[ENG_UITEST][GPS]:Exit,ret:%d\n",ret);
    return ret;
}
#endif

/*****************************************************************************/
//  Description :  GPS test start
//  Global resource dependence : none
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
LOCAL void UITestGPSStart( void )
{
    //MMIGPS_RES_E gps_res = MMIGPS_RES_STATE_WRONG;
#ifdef UAL_GNSS_SUPPORT
    ual_gnss_start_param_t   start_param = {0};
#endif
    SCI_TRACE_LOW("[ENG_UITEST] [GPS]UITestGPSStart");
    //init gps first
    //MMIGPS_RegisterCallBack(UITestGPSCallback);
    //gps_res = MMIGPS_Open();
#ifdef UAL_GNSS_SUPPORT
    ual_gnss_register(gnss_callback,&g_gnss_handle);
    start_param.start_mode = 0;
    ual_gnss_start(&start_param);
#endif
    //SCI_TRACE_LOW("[ENG_UITEST] gps init res:%d", gps_res);
}
/*****************************************************************************
Prototype    : UITestGPSWinHandleMsg
Description  : GPS测试主窗口消息处理函数
Input        : MMI_WIN_ID_T win_id
MMI_MESSAGE_ID_E msg_id
DPARAM param
Output       : None
Return Value :
Calls        :
Called By    :

  History        :
  1.Date         : 2020/5/29
  Author       : xiuyun.wang
  Modification : Created function

*****************************************************************************/
PUBLIC MMI_RESULT_E UITestGPSWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;

    SCI_TRACE_LOW("[ENG_UITEST] UITestGPSWinHandleMsg msg_id is 0x%x", msg_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            SCI_TRACE_LOW("[ENG_UITEST] UITestGPSWinHandleMsg MSG_OPEN_WINDOW");
            //set display window
            GUILIST_SetMaxItem(MMIENG_LISTBOX_CTRL_ID, 140, FALSE );
            GUILIST_SetListState(MMIENG_LISTBOX_CTRL_ID,GUILIST_STATE_FOCUS,FALSE);
            UITestGPS_AppendONELINEItemContent(MMIENG_LISTBOX_CTRL_ID,GPS_STARTING);
            MMK_SetAtvCtrl(win_id,MMIENG_LISTBOX_CTRL_ID);

            //start gps
            UITestGPSStart();
            break;
        }

#if defined MMI_PDA_SUPPORT
        case MSG_APP_MENU:
#endif
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        {
            if(TRUE == g_isResponsePassKey)
            {
                //MMIGPS_Stop();
#ifdef UAL_GNSS_SUPPORT
                ual_gnss_stop();
#endif
                SaveTestResult( win_id, 1);
                MMIAPIENG_RunUITestNextStep();
                MMK_CloseWin(  win_id) ;
            }
            break;
        }
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            //MMIGPS_Stop();
#ifdef UAL_GNSS_SUPPORT
            ual_gnss_stop();
#endif
            SaveTestResult(win_id, 0);
            MMIAPIENG_RunUITestNextStep();
            MMK_CloseWin(win_id) ;
            break;

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
            {
                MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
                if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
                {
                    MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                }
                else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
                {
                    MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                }
                else if( MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ctrl_id)
                {
                    MMK_SendMsg(win_id, MSG_APP_WEB, PNULL);
                }
            }
            break;
#endif

        case MSG_KEYDOWN_RED:
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}


#endif

#ifdef ATV_SUPPORT
/********************************************************************************
NAME:          ENGUITestATVChannelWinHandleMsg
DESCRIPTION:   atc channel selecting win
AUTHOR:        ying.xu
DATE:          2010.12.21
********************************************************************************/
LOCAL BOOLEAN ENGUITestATVChannelWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_STRING_T text = {0};
    uint32      buffer_size = 31 ;
    uint32      str_length = 0;
    char        buffer[32] = {0};
    wchar       wstr[32] = {0} ;
    uint32      i = 0;

    const int32 channel_num = 9;
    //   to de changed by custom
    const int32 channel[9] = {11,21,31,41,51,61,71,81,91};       //channel id <100
    MMI_CTRL_ID_T list_id = MMIENG_UITEST_LISTBOX_CTRL_ID;
    int32 list_index = 0;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(list_id, channel_num, FALSE );  
        GUILIST_SetOwnSofterKey(list_id, FALSE);
        //set selected item
        GUILIST_SetSelectedItem(list_id, 0, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(list_id,0);
        
        for(i=0; i < channel_num; i++ )
        {
            GUILIST_ITEM_T      item_t    = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
            item_t.item_data_ptr = &item_data;    
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            SCI_MEMSET(buffer, 0,  buffer_size);
            str_length  = sprintf(buffer, "CH%d", channel[i]);   
            SCI_MEMSET(wstr, 0,  buffer_size*sizeof(wchar));
            MMI_STRNTOWSTR(wstr, buffer_size, (uint8*)buffer, buffer_size, str_length);
            text.wstr_ptr = wstr ;
            text.wstr_len = str_length ;
            item_data.item_content[0].item_data.text_buffer =  text;        
            GUILIST_AppendItem( list_id, &item_t );
        }                        
        MMK_SetAtvCtrl(win_id, list_id);    
        
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:          
        list_index = GUILIST_GetCurItemIndex(list_id);
        MMK_CreateWin( (uint32*)MMIENG_UITESTATV_CHANNEL_EDIT_WIN_TAB, (ADD_DATA)channel[list_index]);
        break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB: 
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:    
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        list_index = GUILIST_GetCurItemIndex(list_id);      
        MMK_CreateWin( (uint32*)MMIENG_UITESTATV_SIGNAL_WIN_TAB,  (ADD_DATA)channel[list_index]);
        MMK_CloseWin(win_id);
        break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:     
        MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
        break;
#endif

    case MSG_KEYDOWN_RED:           
        break;   

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
NAME:          UITESTWIN_OpenPhoneTestWin
DESCRIPTION:   atc channel selecting win
AUTHOR:        ying.xu
DATE:          2010.12.21
********************************************************************************/
LOCAL BOOLEAN ENGUITestATVChannelEditWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_STRING_T    text = {0};
    int32           edit_id = MMIENG_UITEST_EDITBOX_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIFORM_SetStyle(MMIENG_UITEST_FORM2_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetType(MMIENG_UITEST_FORM2_CTRL_ID, GUIFORM_TYPE_TP);
        {
            char    str[] = "Please input frequency";
            char    buffer[32] = {0};
            uint32  str_length = 0;
            uint32  buffer_size = 31;
            wchar   wstr[32] = {0} ;
            uint32  frequency = (uint32)MMK_GetWinAddDataPtr(win_id);
            str_length = SCI_STRLEN(str);
            SetLabelTextByCharArray(MMIENG_UITEST_LABEL1_CTRL_ID, str, str_length, FALSE);
//             SCI_MEMSET(wstr, 0,  buffer_size*sizeof(wchar));
//             text.wstr_ptr = wstr ;
//             text.wstr_len = str_length ;
//             MMI_STRNTOWSTR(wstr, buffer_size, (uint8*)str, str_length, str_length) ;
//             GUILABEL_SetText( MMIENG_UITEST_LABEL1_CTRL_ID, &text, FALSE);
            sprintf(buffer, "%d", frequency);
            str_length  = SCI_STRLEN(buffer);
            MMI_STRNTOWSTR(wstr, buffer_size, (uint8*)buffer, buffer_size, str_length);
            GUIEDIT_SetString(edit_id , wstr,  (uint16)str_length);
        }
        MMK_SetAtvCtrl(win_id, edit_id);
        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:           
        MMK_CloseWin(win_id);
        break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:         
        {
            int32   buffer_size = 31 ;
            char    buffer[32] = {0};
            int32   freq = 0;
            GUIEDIT_GetString( edit_id,  &text);
            MMI_WSTRNTOSTR((uint8*)buffer, buffer_size, text.wstr_ptr, text.wstr_len,   text.wstr_len);
            freq = atoi(buffer);
            MMK_CreateWin( (uint32*)MMIENG_UITESTATV_SIGNAL_WIN_TAB,  (ADD_DATA)freq);  /*lint !e64*/         
            MMK_CloseWin(MMIENG_UITESTATV_CHANNEL_WIN_ID);
            MMK_CloseWin(win_id);
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif   
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEYDOWN_RED:           
        break;    
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
NAME:          UITESTWIN_OpenPhoneTestWin
DESCRIPTION:   atc channel selecting win
AUTHOR:        ying.xu
DATE:          2010.12.21
********************************************************************************/
LOCAL BOOLEAN ENGUITestATVSignalWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32       frequency = (uint32)MMK_GetWinAddDataPtr(win_id);
    static uint8 timer_id = 0;
#ifndef WIN32
    static int32 atv_result = 0;
#endif
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint32  buffer_size = 31 ;
            char    str[3][32] = {"MAX RSSI","5","Measured RSSI"};
//            char    *wait_str = "Opening ATV, Please wait...";
            char    buffer[32] = {0} ;
//            uint32  str_length = 0;
//            wchar   wstr[32] = {0} ;
            uint32  i = 0;
//            MMI_STRING_T text = {0};
            sprintf(buffer,"CH%d", frequency);
            SetLabelTextByCharArray(MMIENG_UITEST_LABEL1_CTRL_ID, buffer, buffer_size, FALSE);
//             str_length  = SCI_STRLEN(buffer);
//             SCI_MEMSET(wstr, 0,  buffer_size*sizeof(wchar));
//             MMI_STRNTOWSTR(wstr, buffer_size, (uint8*)buffer, buffer_size, str_length);
//             text.wstr_ptr = wstr ;
//             text.wstr_len = str_length ;
//             GUILABEL_SetText( MMIENG_UITEST_LABEL1_CTRL_ID, &text, FALSE);

            for(i=0; i < 3; i++)
            {
                SetLabelTextByCharArray(MMIENG_UITEST_LABEL2_CTRL_ID+i, str[i], buffer_size, FALSE);
//                 str_length  = SCI_STRLEN(str[i]);
//                 SCI_MEMSET(wstr, 0,  buffer_size*sizeof(wchar));
//                 MMI_STRNTOWSTR(wstr, buffer_size, (uint8*)str[i], buffer_size, str_length);
//                 text.wstr_ptr = wstr ;
//                 text.wstr_len = str_length ;
//                 GUILABEL_SetText( MMIENG_UITEST_LABEL2_CTRL_ID + i, &text, FALSE);
            }

//             text.wstr_len = strlen(wait_str);
//             text.wstr_ptr = wstr;
//             MMI_STRNTOWSTR( wstr, 31, (uint8*)wait_str, text.wstr_len, text.wstr_len );
            //MMIPUB_OpenWaitWin(1,&text,PNULL,PNULL,MMIENG_WAITING_WIN_ID,IMAGE_NULL,
             //  ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,MMIPUB_HandleWaitWinMsg);

#if !defined(WIN32) && defined (CAMERA_SUPPORT)
            //call interface of ATV, set the frequency
            {
          
                atv_result = DRECORDER_SelectSrcDevice(DRECORDER_SRC_DEV_ATV_FACTORY);
                 //DRECORDER_Open时默认为camera工作方式，这里需要设置为ATV模式
                //SCI_TRACE_LOW:"mmieng_uitestwin DRECORDER_SelectSrcDevice ATV result = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_5969_112_2_18_2_17_35_78,(uint8*)"d", atv_result);
                if(0 != atv_result)
                {
                    break;
                }                 
                //SCI_TRACE_LOW:"mmieng_uitestwin DRECORDER_Open before, result = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_5974_112_2_18_2_17_35_79,(uint8*)"d", atv_result);
                 atv_result = DRECORDER_Open(); 
                 //SCI_TRACE_LOW:"mmieng_uitestwin DRECORDER_Open result = %d"
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_5976_112_2_18_2_17_35_80,(uint8*)"d", atv_result);
                if(0 != atv_result)   
                {
                break;
                }                

                 //set region
                atv_result = DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_ATV_REGION,  ATV_REGION_CHINA, 0);                 
                //SCI_TRACE_LOW:"mmieng_uitestwin DCAMERA_ParamCtrl ATV_REGION_CHINA = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_5984_112_2_18_2_17_35_81,(uint8*)"d", atv_result);
                if(0 != atv_result)
                {
                    break;
                }
                atv_result = DRECORDER_ParamCtrl(DRECORDER_PARAM_CTRL_ATV_CHN,
                    //ref需要根据高位来做对应的处理
                    ((DCAMERA_ATV_CHN_NOLY_SET<<0x10) & 0xffff0000)|(frequency & 0xffff),  0);
                //SCI_TRACE_LOW:"mmieng_uitestwin DCAMERA_ParamCtrl result = %d,ch = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_5992_112_2_18_2_17_35_82,(uint8*)"dd", atv_result, frequency);
                if(0 != atv_result)
                {
                    break;
                }
            }
#endif
            //MMK_CloseWin( MMIENG_WAITING_WIN_ID);
            if(timer_id != 0)
            {
                MMK_StopTimer(timer_id);
                timer_id = 0;
            }
            timer_id = MMK_CreateTimer(1000 , FALSE);
            MMK_SetAtvCtrl(win_id, MMIENG_UITEST_FORM2_CTRL_ID);
        }
            break;

    case MSG_TIMER:
        {
            uint32  rssi = 0;
            uint32  buffer_size = 31 ;
//            uint32  str_length = 0;
            char    buffer[32] = {0};
//           wchar   wstr[32] = {0} ;
//            MMI_STRING_T text = {0};
#ifndef WIN32
#ifdef  CAMERA_SUPPORT
            //call interface of ATV, set the frequency
            atv_result = DCAMERA_GetInfo(DCAMERA_INFO_ATV_RSSI, &rssi);
            //SCI_TRACE_LOW:"mmieng_uitestwin DCAMERA_GetInfo, result = %d,ch = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_6021_112_2_18_2_17_35_83,(uint8*)"dd", atv_result, frequency);
#endif			
#else
            rssi = 5 ; //for simulator test  1~5
#endif
            sprintf(buffer, "%d", rssi);
            SetLabelTextByCharArray(MMIENG_UITEST_LABEL5_CTRL_ID, buffer, buffer_size, TRUE);
//             str_length  = SCI_STRLEN(buffer);
//             SCI_MEMSET(wstr, 0,  buffer_size*sizeof(wchar));
//             MMI_STRNTOWSTR(wstr, buffer_size, (uint8*)buffer, buffer_size, str_length);
//             text.wstr_ptr = wstr ;
//             text.wstr_len = str_length ;
//             GUILABEL_SetText( MMIENG_UITEST_LABEL5_CTRL_ID, &text, TRUE);
        }
        if(timer_id != 0)
        {
            MMK_StopTimer(timer_id);
            timer_id = 0;
        }
        timer_id = MMK_CreateTimer(1000 , FALSE);  
        break;
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:       
        SaveTestResult(win_id, 0);
        MMIAPIENG_RunUITestNextStep();       
        MMK_CloseWin(win_id);           
        break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
        SaveTestResult(win_id, 1);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(win_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif   
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEYDOWN_RED:
        break;

    case MSG_CLOSE_WINDOW:
        if(timer_id != 0)
        {
            MMK_StopTimer(timer_id);
            timer_id = 0;
        }
  #ifndef WIN32
  #ifdef  CAMERA_SUPPORT  
        atv_result =  DRECORDER_Close();
  #endif         
  #endif
  
        break;   
        
    default:
        result = MMI_RESULT_FALSE;
        break;
        }
        return (result);   
}
#endif
#if defined (PLATFORM_ANTISW3)
LOCAL int32 GetCharingInfo(char p_str[], int32 size)
{
    char str[256] = {0};
    int32 offset = 0;
    int   chg_temperature = 0;
    wchar  udicode_deg = 0xB0;
    char   str_deg[2] = {0};
    CHGMNG_STATE_INFO_T charging_info = {0};
    char *charging_state[] = {"idle", "starting", "high current charging",
                            "low current charging","re-charing", "pulse charing", "stopping"};
    char *charger_type[] = {"unknow", "standard", "non-standard","USB"};
    char *charger_result[] = {"Fail", "Pass", "Please Change Battery,fail"};
    uint8 result_index = 0;

    charging_info = *CHGMNG_GetModuleState();
    SCI_TRACE_LOW("watcheng: GetCharingInfo, ichg = %d",charging_info.ichg);
    chg_temperature = charging_info.charging_temperature - 1000; //需要减去1000

    offset += sprintf(str+offset, "Charging State :\n %s\n",charging_state[charging_info.chgmng_state]);

    offset += sprintf(str+offset, "Adaptor Type :\n %s\n",charger_type[charging_info.adp_type]);

    offset += sprintf(str+offset, "Charging Current :\n %d ma\n",charging_info.charging_current);

    offset += sprintf(str+offset, "Battery Voltage :\n %d mv\n",charging_info.bat_cur_vol);

    MMI_WSTRNTOSTR(str_deg,2,&udicode_deg,2,2);//convert unicode to char
    if(charging_info.bat_cur_vol>=MMIENG_BATTERY_MAX_VOL)
    {
        if(charging_info.ichg<=MMIENG_ICHG_MIN)
        {
            result_index = 2;//CHANGE
        }
        else
        {
            result_index = 1;//PASS
        }
    }
    else
    {
        if(charging_info.ichg>=MMIENG_ICHG_MAX)
        {
            result_index = 1;//PASS
        }
        else
        {
            result_index = 0;//FAIL
        }
    }

    offset += sprintf(str+offset, "Charging Result :\n %s\n",charger_result[result_index]);


    MMI_MEMCPY(p_str, size, str, offset, offset);

    return offset;
}
#elif defined (PLATFORM_UWS6121E)
LOCAL int32 GetCharingInfo(char p_str[], int32 size)
{
    char str[256] = {0};
    int32 offset = 0;
    CHGMNG_STATE_INFO_T charging_info = {0};
    char *charging_state[] = {"idle", "starting", "high current charging",
                            "low current charging","re-charing", "pulse charing", "stopping"};   
    char *charger_type[] = {"unknow", "standard", "non-standard","USB"};
    

    charging_info = *CHGMNG_GetModuleState();    
    
    offset += sprintf(str+offset, "Charging State :\n %s\n",charging_state[charging_info.chgmng_state]);

    offset += sprintf(str+offset, "Adaptor Type :\n %s\n",charger_type[charging_info.adp_type]);
    
    offset += sprintf(str+offset, "Charging Current :\n %d ma\n",charging_info.charging_current);

    offset += sprintf(str+offset, "Battery Voltage :\n %d mv\n",charging_info.bat_cur_vol);

    MMI_MEMCPY(p_str, size, str, offset, offset);

    return offset;
}
#endif
LOCAL BOOLEAN ENGUITestChargingWinHandleMsg(MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    static uint8 timer_id = 0;
    char str[256] = {0};
    wchar wstr[256] = {0};
    int32 length = 0;
    GUI_FONT_T  text_font_size  = MMIENG_TEXT_FONT_SIZE;
    GUI_COLOR_T text_font_color = MMIENG_TEXT_FONT_COLOR;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        length = GetCharingInfo(str, 255);
        MMI_STRNTOWSTR(wstr, 255, (uint8*)str, length, length);
        GUITEXT_SetFont(MMIENG_TEXTBOX1_CTRL_ID,&text_font_size,&text_font_color);
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, wstr, length, FALSE);
        if(timer_id != 0)
        {
            MMK_StopTimer(timer_id);
            timer_id = 0;
        }
        timer_id = MMK_CreateTimer(1000 , TRUE);            
        
        MMK_SetAtvCtrl(win_id, MMIENG_TEXTBOX1_CTRL_ID);     
        break;
        
    case MSG_FULL_PAINT:
        break;   
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:       
        SaveTestResult(win_id, 0);
        MMIAPIENG_RunUITestNextStep();       
        MMK_CloseWin(win_id);           
        break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
        SaveTestResult(win_id, 1);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(win_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif   
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_KEYDOWN_RED:           
        break;   
        
    case MSG_TIMER:    
        length = GetCharingInfo(str, 255);
        MMI_STRNTOWSTR(wstr, 255, (uint8*)str, length, length);
        GUITEXT_SetFont(MMIENG_TEXTBOX1_CTRL_ID,&text_font_size,&text_font_color);
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, wstr, length, TRUE);
        break; 
        
    case MSG_CLOSE_WINDOW:           
        if(timer_id != 0)
        {
            MMK_StopTimer(timer_id);
            timer_id = 0;
        }
        
        break;   
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}




//bug 746951
LOCAL BOOLEAN ENGUITestEmccWinHandleMsg(MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E result         = MMI_RESULT_TRUE;
    wchar wstr[256]             = {0};
    char str[256]               = { 'P', 'a', 's', 's', '/', 'F', 'a', 'i', 'l', ' ', 'k', 'e', 'y', ' ', 't', 'o', ' ', 'n', 'e', 'x', 't', 0 };
    int length = 64;
    MMI_STRING_T text           = {0};
    GUI_RECT_T      client_rect = MMITHEME_GetClientRectEx(win_id);

    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
        {
            MMI_STRNTOWSTR(wstr, 255, (uint8*)str, length, length);
            GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID,&client_rect, FALSE);
            text.wstr_ptr = wstr;
            text.wstr_len = length;
            GUILABEL_SetText(MMIENG_UITEST_LABEL_CTRL_ID, &text, FALSE);
        }
        break;

        case MSG_FULL_PAINT:
        {
            GUI_RECT_T          disp_rect   = MMITHEME_GetFullScreenRect();
            GUI_LCD_DEV_INFO    dev_info    = {0};
            GUI_LCD_DEV_INFO  lcd_dev_info  = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

            LCD_FillRect(&lcd_dev_info, disp_rect, MMI_WHITE_COLOR);
            break;
        }

        #if defined MMI_PDA_SUPPORT
        case MSG_APP_MENU:            
        #endif
        case MSG_KEYDOWN_OK:            
        SaveTestResult(win_id, 1);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin( win_id );  
        break;
        
    case MSG_KEYDOWN_CANCEL:
        SaveTestResult(win_id, 0);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin( win_id ) ;            
        break;
        default:
            result = MMI_RESULT_FALSE;
            break;
     }
    
    return (result);
}


LOCAL BOOLEAN ENGUITestNTCWinHandleMsg(MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_STRING_T temp_text = {0};
    uint8 timer_buf[5] = {0};
    wchar wstr[256]             = {0};
    char str[256]               = { 'T', 'e', 'm', 'p', ' ', 'V', 'a', 'l', 'u', 'e', 0 };    
    int length = 64;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T  client_rect = MMITHEME_GetClientRectEx(win_id);
    GUI_RECT_T		lab_rect = {0};
#ifndef WIN32
		int32 temp_val = CHGMNG_GetNTC_TempVolValue();
#else
		int32 temp_val = 256;
#endif    
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:            
        lab_rect = client_rect;
        lab_rect.bottom =   (lab_rect.top + lab_rect.bottom) /2 ;
        GUILABEL_SetRect(MMIENG_LABEL1_CTRL_ID, &lab_rect, FALSE); 
                
        lab_rect.top =  lab_rect.bottom + 1;
        lab_rect.bottom = lab_rect.top + MMIENG_TEXT_FONT_SIZE + 1;
 
        GUILABEL_SetRect(MMIENG_UITEST_LABEL_CTRL_ID, &lab_rect, FALSE);        
        if (s_mmieng_testrtc_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_testrtc_timer_id);
            s_mmieng_testrtc_timer_id = 0;
        }
        s_mmieng_testrtc_timer_id = MMK_CreateWinTimer(MMIENG_UITEST_NTC_WIN_ID, 1000, TRUE);              
        
		MMI_STRNTOWSTR(wstr, 255, (uint8*)str, length, length);
		
		temp_text.wstr_ptr = wstr;
		temp_text.wstr_len = length;

		GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &temp_text, FALSE);
		GUILABEL_SetFont(MMIENG_LABEL1_CTRL_ID,SONG_FONT_22,MMI_BLACK_COLOR);
        
		sprintf((char*)timer_buf, "%04d", temp_val);
        SetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, (char*)timer_buf, 4, FALSE);
        GUILABEL_SetFont(MMIENG_UITEST_LABEL_CTRL_ID,MMIENG_TEXT_FONT_SIZE,MMIENG_TEXT_FONT_COLOR);

        break;

    case MSG_FULL_PAINT:
        //LCD_FillRect(&lcd_dev_info,client_rect,mmi_uitest_display.bkgd_color);      
        GUIRES_DisplayImg(PNULL, &client_rect ,  &client_rect,  win_id,  IMAGE_COMMON_BG, &lcd_dev_info);
        break;
#if defined MMI_PDA_SUPPORT
    case MSG_APP_MENU:            
#endif   
    case MSG_CTL_OK:
    case MSG_KEYDOWN_OK:  
        if (s_mmieng_testrtc_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_testrtc_timer_id);
            s_mmieng_testrtc_timer_id = 0;
        }
		SaveTestResult(win_id, 1);
		MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(win_id);           
        break;          
        
    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        if (s_mmieng_testrtc_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_testrtc_timer_id);
            s_mmieng_testrtc_timer_id = 0;
        }

		SaveTestResult(win_id, 0);
		MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT        
    case MSG_KEYDOWN_FLIP:      // close flip
#endif    
    case MSG_KEYDOWN_RED:
        break;  
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif   
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_TIMER:
        if (*(uint8*)param == s_mmieng_testrtc_timer_id) 
        {

#ifndef WIN32
		temp_val = CHGMNG_GetNTC_TempVolValue();
#else
		temp_val = 256;
#endif
            sprintf((char*)timer_buf, "%04d", temp_val);
            SetLabelTextByCharArray(MMIENG_UITEST_LABEL_CTRL_ID, (char*)timer_buf, 4, TRUE);
        }
        else
            result = MMI_RESULT_FALSE;      // need additional process
        break;  
        
        
    case MSG_CLOSE_WINDOW:
        if (s_mmieng_testrtc_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_testrtc_timer_id);
            s_mmieng_testrtc_timer_id = 0;
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
//Adjust Touch Screen
#define WATCH_ENG_TP_SCREEN_BOX_ARR_NUM          (24)
#define WATCH_ENG_TP_SCREEN_BOX_WIDTH            (59)
#define WATCH_ENG_TP_SCREEN_BOX_HEIGHT           (39)
#define WATCH_ENG_TP_SCREEN_BOX_COLOR_INI        GUI_RGB2RGB565(0xE0, 0x42, 0x15)
#define WATCH_ENG_TP_SCREEN_BOX_COLOR_PRESSED    MMI_BLACK_COLOR

typedef struct
{
    BOOLEAN is_pressed;
    GUI_RECT_T rect;
}WATCH_ENG_TP_SCREEN_BOX_INFO;

LOCAL uint8 s_cur_pressed_totalnum = 0;
LOCAL WATCH_ENG_TP_SCREEN_BOX_INFO s_eng_tp_screen_box_arr[] =
{
    { FALSE ,{0}},  { FALSE ,{0}},  { FALSE ,{0}},  { FALSE ,{0}},
    { FALSE ,{0}},  { FALSE ,{0}},  { FALSE ,{0}},  { FALSE ,{0}},
    { FALSE ,{0}},  { FALSE ,{0}},  { FALSE ,{0}},  { FALSE ,{0}},
    { FALSE ,{0}},  { FALSE ,{0}},  { FALSE ,{0}},  { FALSE ,{0}},
    { FALSE ,{0}},  { FALSE ,{0}},  { FALSE ,{0}},  { FALSE ,{0}},
    { FALSE ,{0}},  { FALSE ,{0}},  { FALSE ,{0}},  { FALSE ,{0}},
};

LOCAL void TP_SetBoxRect( void )
{
    uint8       curIndex    = 0;
    uint8       labelWidth  = WATCH_ENG_TP_SCREEN_BOX_WIDTH;
    uint8       labelHeight = WATCH_ENG_TP_SCREEN_BOX_HEIGHT;
    GUI_RECT_T  rect        = {0};

    for(; curIndex < WATCH_ENG_TP_SCREEN_BOX_ARR_NUM; curIndex++)
    {
        if( curIndex % 4 == 0)
        {
            rect.left = 1;
            rect.right = rect.left + labelWidth - 1;
            rect.top = ( labelHeight + 1 ) * ( curIndex / 4 ) + 1;
            rect.bottom= rect.top + labelHeight - 1;
        }
        else
        {
            rect.left = rect.right + 2;
            rect.right = rect.left + labelWidth - 1;
        }
        SCI_MEMCPY( &s_eng_tp_screen_box_arr[curIndex].rect, &rect,sizeof(GUI_RECT_T) );
    }
}

LOCAL void TP_DisplayAllBoxWithColor( void )
{
    uint8   curIndex    = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info  = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    for(; curIndex < WATCH_ENG_TP_SCREEN_BOX_ARR_NUM; curIndex++)
    {
        if( s_eng_tp_screen_box_arr[curIndex].is_pressed == FALSE )
        {
            LCD_FillRect(&lcd_dev_info, s_eng_tp_screen_box_arr[curIndex].rect, WATCH_ENG_TP_SCREEN_BOX_COLOR_INI);
        }
        else
        {
            LCD_FillRect(&lcd_dev_info, s_eng_tp_screen_box_arr[curIndex].rect, WATCH_ENG_TP_SCREEN_BOX_COLOR_PRESSED);
        }
    }
}

LOCAL BOOLEAN ENGUITestTouchScreenWinHandleMsg(MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_RESULT_E        result          = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_BG_T            btnBg           = {0};

    btnBg.bg_type = GUI_BG_COLOR;
    btnBg.color   = WATCH_ENG_TP_SCREEN_BOX_COLOR_PRESSED;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            TP_SetBoxRect();
        }
        break;

        #if defined MMI_PDA_SUPPORT
        case MSG_APP_MENU:
        #endif
        case MSG_KEYDOWN_OK:
        {
            if(s_cur_pressed_totalnum == WATCH_ENG_TP_SCREEN_BOX_ARR_NUM)
            {
                SaveTestResult(win_id, 1);
                MMIAPIENG_RunUITestNextStep();
                MMK_CloseWin( win_id );
            }
        }
        break;
        case MSG_KEYDOWN_CANCEL:
        {
            //if(s_cur_pressed_totalnum == WATCH_ENG_TP_SCREEN_BOX_ARR_NUM) //支持中途失败退出
            {
                SaveTestResult(win_id, 0);
                MMIAPIENG_RunUITestNextStep();
                MMK_CloseWin( win_id );
            }
        }
        break;
        case MSG_TP_PRESS_DOWN:
        case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T tp_point        = {0};
            uint8       checkedIndex    = 0;
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);
            for(; checkedIndex < WATCH_ENG_TP_SCREEN_BOX_ARR_NUM; checkedIndex++)
            {
                if( (s_eng_tp_screen_box_arr[checkedIndex].rect.left <= tp_point.x)  &&
                    (s_eng_tp_screen_box_arr[checkedIndex].rect.right >= tp_point.x) &&
                    (s_eng_tp_screen_box_arr[checkedIndex].rect.top <= tp_point.y)   &&
                    (s_eng_tp_screen_box_arr[checkedIndex].rect.bottom >= tp_point.y)
                )
                {
                    if( s_eng_tp_screen_box_arr[checkedIndex].is_pressed == FALSE )
                    {
                        LCD_FillRect(&lcd_dev_info, s_eng_tp_screen_box_arr[checkedIndex].rect, WATCH_ENG_TP_SCREEN_BOX_COLOR_PRESSED);
                        s_eng_tp_screen_box_arr[checkedIndex].is_pressed = TRUE;
                        s_cur_pressed_totalnum ++;
                    }
                    break;
                }
            }
        }
        break;
        case MSG_FULL_PAINT:
        {
            WATCHCOM_DisplayBackground(win_id);
            TP_DisplayAllBoxWithColor();
        }
        break;
        case MSG_CLOSE_WINDOW:
        {
            uint8   curIndex    = 0;
            for(; curIndex < WATCH_ENG_TP_SCREEN_BOX_ARR_NUM; curIndex++)
            {
                s_eng_tp_screen_box_arr[curIndex].is_pressed = FALSE;
                SCI_MEMSET( &s_eng_tp_screen_box_arr[curIndex].rect, 0,sizeof(GUI_RECT_T) );
            }
            s_cur_pressed_totalnum = 0;
        }
        break;
        default:
        break;
     }

    return (result);
}
#endif

#ifdef ENG_SUPPORT
/********************************************************************************
NAME:          EngTestSimDetailWindow
DESCRIPTION:   create the detial window of test sim
AUTHOR:        zhenxing.chen
DATE:         2016.10.25
********************************************************************************/
LOCAL void EngTestSimDetailWindow(MMI_CTRL_ID_T  ctrl_id)
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    uint16              index = 0;
    BOOLEAN             sim1_reslut = FALSE;
    BOOLEAN             sim2_reslut = FALSE;

 //   GUIRICHTEXT_SetItemSpace(ctrl_id,0);
    GUIRICHTEXT_SetBgColor(ctrl_id,MMI_BLACK_COLOR);
//the result:
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_set_font_color = TRUE;
    item_data.setting_font_color= MMI_WHITE_COLOR;
    item_data.text_set_font = TRUE;
    item_data.setting_font = SONG_FONT_22;
    item_data.text_data.res.id = TXT_ENG_TEST_SIM_RESULT;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

//test sim1 result:   
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_set_font_color = TRUE;
    item_data.setting_font_color = MMI_WHITE_COLOR;
    item_data.text_set_font = TRUE;
    item_data.setting_font = SONG_FONT_22;
    item_data.text_data.res.id = TXT_ENG_TEST_SIM1;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

//sim1  result  
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_set_font = TRUE;
    item_data.setting_font = SONG_FONT_22;
    item_data.text_set_font_color= TRUE;
    item_data.text_set_align = TRUE;
    item_data.setting_align =ALIGN_HVMIDDLE;
    if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
    {       
        item_data.setting_font_color= MMI_GREEN_COLOR;
        item_data.text_set_font = TRUE;
        item_data.setting_font = SONG_FONT_22;
        item_data.text_data.res.id = TXT_ENG_TEST_SIM_RESULT1;
        sim1_reslut = TRUE;
    }
    else
    {
        item_data.setting_font_color= MMI_RED_COLOR;
        item_data.text_set_font = TRUE;
        item_data.setting_font = SONG_FONT_22;
        item_data.text_data.res.id = TXT_ENG_TEST_SIM_RESULT2;
    }
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
	
#ifdef MMI_MULTI_SIM_SYS_DUAL
   //test sim2 result:   
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_set_font_color = TRUE;
    item_data.setting_font_color = MMI_WHITE_COLOR;
    item_data.text_set_font = TRUE;
    item_data.setting_font = SONG_FONT_22;
    item_data.text_data.res.id = TXT_ENG_TEST_SIM2;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

//sim2  result  
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_set_font = TRUE;
    item_data.setting_font = SONG_FONT_22;
    item_data.text_set_font_color= TRUE;
    item_data.text_set_align = TRUE;
    item_data.setting_align =ALIGN_HVMIDDLE;
    if(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        item_data.setting_font_color= MMI_GREEN_COLOR;
        item_data.text_set_font = TRUE;
        item_data.setting_font = SONG_FONT_22;
        item_data.text_data.res.id = TXT_ENG_TEST_SIM_RESULT1;
        sim2_reslut = TRUE;
    }
    else
    {
        item_data.setting_font_color= MMI_RED_COLOR;
        item_data.text_set_font = TRUE;
        item_data.setting_font = SONG_FONT_22;
        item_data.text_data.res.id = TXT_ENG_TEST_SIM_RESULT2;
    }
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
#endif

//finial  result  
    SCI_MEMSET(&item_data, 0x00, sizeof(GUIRICHTEXT_ITEM_T));
    item_data.text_type = GUIRICHTEXT_TEXT_RES;
    item_data.text_set_font = TRUE;
    item_data.setting_font = SONG_FONT_22;
    item_data.text_set_font_color= TRUE;
    if(sim1_reslut
#ifdef MMI_MULTI_SIM_SYS_DUAL
       && sim2_reslut
#endif
       )
    {
        item_data.setting_font_color= MMI_GREEN_COLOR;	
        item_data.text_set_font = TRUE;
        item_data.setting_font = SONG_FONT_22;
        item_data.text_data.res.id = TXT_ENG_TEST_SIM_RESULT1;
    }
    else
    {
        item_data.setting_font_color= MMI_RED_COLOR;
        item_data.text_set_font = TRUE;
        item_data.setting_font = SONG_FONT_22;
        item_data.text_data.res.id = TXT_ENG_TEST_SIM_RESULT2;
    }
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
   
}

/********************************************************************************
NAME:          UITestSimWinHandleMsg
DESCRIPTION:   test of sim
AUTHOR:        zhenxing.chen
DATE:         2016.10.25
********************************************************************************/
LOCAL BOOLEAN UITestSimWinHandleMsg(MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
     switch(msg_id)
     {
      case MSG_OPEN_WINDOW:        
        EngTestSimDetailWindow(MMIENG_TESTSIM_CTRL_ID);
        MMK_SetAtvCtrl(win_id,MMIENG_TESTSIM_CTRL_ID);
	    break;
      case MSG_FULL_PAINT:
        break;   
        
     case MSG_KEYDOWN_CANCEL:
     case MSG_CTL_CANCEL: 
         SaveTestResult(win_id, 0);
         MMIAPIENG_RunUITestNextStep();
	     MMK_CloseWin(win_id);
        break;
        
#if defined MMI_PDA_SUPPORT
     case MSG_APP_MENU:            
#endif
     case MSG_KEYDOWN_OK:
     case MSG_CTL_OK:
         SaveTestResult(win_id, 1);
         MMIAPIENG_RunUITestNextStep();
         MMK_CloseWin(win_id);
        break;  
                
    case MSG_CLOSE_WINDOW:
    case MSG_APP_UP:
    case MSG_APP_DOWN:
        break;   
    default:
        result = MMI_RESULT_FALSE;
        break;
     }
     return (result);
}
#endif
#ifdef GOODIX_SUPPORT_GR5515
LOCAL void UITestSensorHubCB(uint16 opcode, uint16 status)
{
    SENSORHUB_ENGTEST_CB_T                  *sig_ptr = PNULL;
    SCI_TRACE_LOW("enter UITestSensorHubCB");
    MmiCreateSignal (MSG_ENG_SENSORHUB_TEST, sizeof (SENSORHUB_ENGTEST_CB_T), (MmiSignalS**)&sig_ptr);
    sig_ptr->SignalSize = sizeof (SENSORHUB_ENGTEST_CB_T);
    sig_ptr->Sender = P_APP;
    sig_ptr->opcode = opcode;
    sig_ptr->status = status;
    sig_ptr->win_id = MMIENG_UITEST_SENSORHUB_WIN_ID;
    sig_ptr->msg_id = MSG_FULL_PAINT;
    MMI_SEND_SIGNAL (sig_ptr, P_APP);
}
#endif
#ifndef WIN32
#ifdef GOODIX_SUPPORT_GR5515
LOCAL plt_func_cfg_t sensorTest_cb =
{
    .plt_op_status_handler = UITestSensorHubCB,
};
#endif
#endif
#ifdef SENSORHUB_SUPPORT
/********************************************************************************
NAME:          UITestSensorHubWinHandleMsg
DESCRIPTION:   test of SensorHub
AUTHOR:        longwei.qiao
DATE:          2021.03.11
********************************************************************************/
LOCAL BOOLEAN UITestSensorHubWinHandleMsg(MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
     switch(msg_id)
     {
          case MSG_OPEN_WINDOW:
               MMIDEFAULT_AllowTurnOffBackLight(FALSE);
#ifndef WIN32
#ifdef GOODIX_SUPPORT_GR5515
               register_plt_cb(&sensorTest_cb);
               plt_start();
#endif
#endif
          break;
          case MSG_FULL_PAINT:
          {
            GUI_RECT_T clientRect = MMITHEME_GetClientRectEx(win_id);
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            GUI_FillRect(&lcd_dev_info, clientRect, MMI_BLACK_COLOR);
            if(PNULL != param)
            {
                GUI_LCD_DEV_INFO    lcd_dev_info   = {0};
                GUISTR_STYLE_T      text_style     = {0};
                GUISTR_STATE_T      text_state     = GUISTR_STATE_ALIGN;
                //set font param
                text_style.align      = ALIGN_HMIDDLE;
                text_style.font       = DP_FONT_24;
                text_style.font_color = MMI_WHITE_COLOR;

                //转换窗口坐标到显示坐标
                GUISTR_DrawTextToLCDInRect(
                                  (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                  &clientRect,
                                  &clientRect,
                                  (MMI_STRING_T*)param,
                                  &text_style,
                                  text_state,
                                  GUISTR_TEXT_DIR_AUTO
                                  );
            }
          }
          break;
         case MSG_KEYDOWN_CANCEL:
         case MSG_CTL_CANCEL:
             SaveTestResult(win_id, 0);
             MMIAPIENG_RunUITestNextStep();
             MMK_CloseWin(win_id);
            break;
         case MSG_KEYDOWN_OK:
         case MSG_CTL_OK:
             SaveTestResult(win_id, 1);
             MMIAPIENG_RunUITestNextStep();
             MMK_CloseWin(win_id);
            break;
            case MSG_CLOSE_WINDOW:
             MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
         }
     return (result);
}
#endif
/********************************************************************************
NAME:          UITESTWIN_OpenPhoneTestWin
DESCRIPTION:   open uitest main menu
AUTHOR:        ying.xu
DATE:          2010.8.10
********************************************************************************/
#ifdef ENG_SUPPORT
PUBLIC BOOLEAN UITESTWIN_OpenPhoneTestWin(void)
{
    MMK_CreateWin((uint32 *)(MMIENG_UITEST_MENU_WIN_TAB), PNULL);
    return TRUE;
}
#endif
#ifdef MEDIA_DEMO_SUPPORT

/********************************************************************************
NAME:          UITESTWIN_OpenVideoDemoWin
DESCRIPTION:   open audio string test main menu
AUTHOR:         
DATE:          2011.3.20
********************************************************************************/
PUBLIC BOOLEAN UITESTWIN_OpenVideoDemoWin(void)
{
    MMI_WIN_ID_T    win_id = MMIENG_MEDIA_VIDEO_DEMO_MENU_WIN_ID;
    MMI_CTRL_ID_T   ctrl_id = MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID;
    GUI_BOTH_RECT_T both_rect = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    MMI_TEXT_ID_T            text_id = 0;
    MMI_STRING_T    kstring = {0};
    uint32          node_id = 0;
    uint16           i = 0;

    MMK_CreateWin((uint32 *)(MMIENG_MEIDA_VIDEO_DEMO_TAB), PNULL);

    both_rect = MMITHEME_GetWinClientBothRect(win_id);//set both rect    //creat dynamic menu
    GUIMENU_CreatDynamic(&both_rect,win_id, ctrl_id, GUIMENU_STYLE_THIRD);

    MMI_GetLabelTextByLang(TXT_ENG_MEDIA_VIDEO_DEMO, &kstring);
    GUIMENU_SetMenuTitle(&kstring,ctrl_id);
    
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_NULL, TXT_ENG_MEDIA_DEMO_START, TXT_ENG_RETURN);

    text_id = TXT_ENG_MEDIA_VIDEO_CHAT_DEMO;
    node_id = ID_ENG_VIDEO_DC_DEMO;
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(i++, node_id, 0, &node_item, ctrl_id);

    text_id = TXT_ENG_MEDIA_VIDEO_YUV_DEMO;
    node_id = ID_ENG_VIDEO_YUV_DEMO;
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(i++, node_id, 0, &node_item, ctrl_id);

    return TRUE;
}

/********************************************************************************
NAME:          UITESTWIN_OpenAudioDemoWin
DESCRIPTION:   open audio string test main menu
AUTHOR:         
DATE:          2011.3.20
********************************************************************************/
PUBLIC BOOLEAN UITESTWIN_OpenAudioDemoWin(void)
{
    MMI_WIN_ID_T    win_id = MMIENG_MEDIA_AUDIO_DEMO_MENU_WIN_ID;
    MMI_CTRL_ID_T   ctrl_id = MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID;
    GUI_BOTH_RECT_T both_rect = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    MMI_TEXT_ID_T            text_id = 0;
    MMI_STRING_T    kstring = {0};
    uint32          node_id = 0;
    uint16           i = 0;

    MMK_CreateWin((uint32 *)(MMIENG_MEIDA_AUDIO_DEMO_TAB), PNULL);

    both_rect = MMITHEME_GetWinClientBothRect(win_id);//set both rect    //creat dynamic menu
    GUIMENU_CreatDynamic(&both_rect,win_id, ctrl_id, GUIMENU_STYLE_THIRD);

    MMI_GetLabelTextByLang(TXT_ENG_MEDIA_AUDIO_DEMO, &kstring);
    GUIMENU_SetMenuTitle(&kstring,ctrl_id);
    
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);

    text_id = TXT_ENG_MEDIA_AUDIO_AAC_STREAM_DEMO;
    node_id = ID_ENG_AUDIO_AAC_STRING_DEMO;
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(i++, node_id, 0, &node_item, ctrl_id);

    text_id = TXT_ENG_MEDIA_AUDIO_AMR_STREAM_DEMO;
    node_id = ID_ENG_AUDIO_AMR_STRING_PLAY_DEMO;
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(i++, node_id, 0, &node_item, ctrl_id);

    text_id = TXT_ENG_MEDIA_AUDIO_MP3_STREAM_DEMO;
    node_id = ID_ENG_AUDIO_MP3_STRING_DEMO;
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(i++, node_id, 0, &node_item, ctrl_id);

    text_id = TXT_ENG_MEDIA_AUDIO_WAV_STREAM_DEMO;
    node_id = ID_ENG_AUDIO_WAV_STRING_DEMO;
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(i++, node_id, 0, &node_item, ctrl_id);

    text_id = TXT_ENG_MEDIA_AUDIO_AMR_RECORD_DEMO;
    node_id = ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO;
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(i++, node_id, 0, &node_item, ctrl_id);

    text_id = TXT_ENG_MEDIA_AUDIO_EXPRESS_DEMO;
    node_id = ID_ENG_AUDIO_EXPRESS_DEMO;
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(i++, node_id, 0, &node_item, ctrl_id);
    

    text_id = TXT_ENG_MEDIA_AUDIO_CAT_MVOICE_DEMO;
    node_id = ID_ENG_AUDIO_CAT_MVOICE_DEMO;
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(i++, node_id, 0, &node_item, ctrl_id);

    text_id = TXT_ENG_MEDIA_AUDIO_MIC_SAMPLE_DEMO;
    node_id = ID_ENG_AUDIO_MIC_SAMPLE_DEMO;
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(i++, node_id, 0, &node_item, ctrl_id);


    return TRUE;
}

#endif
#ifdef CAMERA_SUPPORT
PUBLIC void UITestCameraEnter(void)
{
    s_is_fulltest = 0;
    MMK_CreateWin((uint32*)MMIENG_UITESTCAMERA_WIN_TAB,  PNULL);
}

PUBLIC void UITestVideoEnter(void)
{
    s_is_fulltest = 0;
    MMK_CreateWin((uint32*)MMIENG_UITESTVIDEO_WIN_TAB,  PNULL);
}
#endif
/********************************************************************************
NAME:          SetItemFontColor
DESCRIPTION:   set menu item font color
PARAM IN:
PARAM OUT:
AUTHOR:        longwei.qiao
DATE:          2020.07.13
********************************************************************************/
LOCAL void SetItemFontColor(      MMI_CTRL_ID_T        ctrl_id,    // [in]
                                      MMI_MENU_GROUP_ID_T  group_id   // [in]
                               )
{

    uint32 i=0 ;
    uint32 j=0 ;

    uint32 is_tested   = 0;
    uint32 test_result = 0;
    uint32 item_result = 0;

    ReadUITestResultNV(&is_tested, &test_result);//get result from nv

    for(i = 0, j = 0; i < ARR_SIZE(s_all_test_win_id); i++)
    {
        if(0 == s_all_test_win_id[i])
        {
            continue;
        }

        //if tested
        item_result = (is_tested >>i) & 0x00000001 ;

        if(1 == item_result)
        {
            //result
            item_result = (test_result >>i) & 0x00000001 ;
            if(0 == item_result)
            {
                CTRLMENU_SetItemFontColor(ctrl_id, group_id, j, MMIENG_TEST_FAIL_COLOR);
                SCI_TRACE_LOW("test result is fail item = %d", j);
                SCI_TRACE_LOW("s_all_test_win_id index = %d", i);
            }else if(1 == item_result)
            {
                CTRLMENU_SetItemFontColor(ctrl_id, group_id, j, MMIENG_TEST_PASS_COLOR);
                SCI_TRACE_LOW("test result is pass item = %d", j);
                SCI_TRACE_LOW("s_all_test_win_id index = %d", i);
            }
        }else
        {
            CTRLMENU_SetItemFontColor(ctrl_id, group_id, j, MMIENG_NOT_TEST_COLOR);
            SCI_TRACE_LOW("test result is not test item = %d", j);
            SCI_TRACE_LOW("s_all_test_win_id index = %d", i);
        }
       j++;
     }
}

#ifdef ENG_SUPPORT  //@leon.wang add#, the function is not used when ENG_SUPPORT=FALSE;
/********************************************************************************
NAME:          ENGUITestMenuWinHandleMsg
DESCRIPTION:   uitest menu option
PARAM IN:
PARAM OUT:
AUTHOR:        ying.xu
DATE:          2010.08.05
********************************************************************************/
LOCAL  MMI_RESULT_E ENGUITestMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 group_id = 0;
    uint32 menu_id = 0;
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            SetItemFontColor(MMIENG_UITEST_MENU_CTRL_ID,MENU_PHONE_ITEM_TEST);
            CTRLMENU_SetSecMenuFontSize(MMIENG_UITEST_MENU_CTRL_ID,MMIENG_MENU_ITEM_FONT,MMIENG_MENU_ITEM_FONT);
            CTRLMENU_SetSecMenuItemHeight(MMIENG_UITEST_MENU_CTRL_ID,MMIENG_MENU_ITEM_HEIGHT,MMIENG_MENU_ITEM_HEIGHT);
            CTRLMENU_SetFontColor(MMIENG_UITEST_MENU_CTRL_ID,MMI_WHITE_COLOR);
            MMK_SetAtvCtrl(win_id, MMIENG_UITEST_MENU_CTRL_ID);
            break;

        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_GET_FOCUS:
            SetItemFontColor(MMIENG_UITEST_MENU_CTRL_ID,MENU_PHONE_ITEM_TEST);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB: 
            MMIENG_RunUITestInit();
            GUIMENU_GetId(MMIENG_UITEST_MENU_CTRL_ID, &group_id, &menu_id);
            switch (menu_id)
            {
            case ID_ENG_TEST_FULLTEST:
                s_is_fulltest = 1;
                MMIAPIENG_StartUITest();
                break;

            case ID_ENG_TEST_VIEWRESULT:
                MMK_CreateWin((uint32*)MMIENG_UITEST_RESULT_WIN_TAB,  PNULL);
                break;

            case ID_ENG_TEST_ADC_CALIBRATE_INFO:
                MMK_CreateWin((uint32*)MMIENG_SHOW_ADC_CALIBRATE_WIN_TAB,  PNULL);
                break;

            case ID_ENG_TEST_PHONEINFO:
                MMK_CreateWin((uint32*)MMIENG_UITEST_PHONEINFO_WIN_TAB,  PNULL);
                break;
#if !defined  TOUCH_PANEL_SUPPORT || defined(PLATFORM_UWS6121E)
        case ID_ENG_TEST_KEY :
            s_is_fulltest = 0 ;
            MMK_CreateWin((uint32*)MMIENG_UITESTKEY_WIN_TAB,  PNULL);
            break;
#endif
        case ID_ENG_TEST_FACTORYRESET:
            MMIAPISET_OpenInputResetPwdWin(FALSE); 
            break;
        case ID_ENG_TEST_RESET:
            MMIAPIPHONE_PowerReset();
            break;
        case ID_ENG_TEST_MAINLCD :
            s_is_fulltest = 0 ;
            MMK_CreateWin((uint32*)MMIENG_UITESTMAINLCD_WIN_TAB,  PNULL);
            break;

            case ID_ENG_TEST_RTC :
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITESTRTC_WIN_TAB,  PNULL);
                break;

            case ID_ENG_TEST_VIBRATER :
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITESTVIBRATE_WIN_TAB,  PNULL);
                break;

            case ID_ENG_TEST_BACKLIGHT :
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITESTBACKLIGHT_WIN_TAB,  PNULL);
                break;

            case ID_ENG_TEST_LOOPBACK :
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITESTLOOPBACK_WIN_TAB,  PNULL);
                break;

#ifdef  CAMERA_SUPPORT
#ifndef WIN32
            case ID_ENG_TEST_CAMERA :

                #ifdef UI_P3D_SUPPORT
                    MMI_Disable3DEffect(MMI_3D_EFFECT_CAMERA);
                #endif

                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITESTCAMERA_WIN_TAB,  PNULL);
                break;
            case ID_ENG_TEST_VIDEO :
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITESTVIDEO_WIN_TAB,  PNULL);
                break;
#endif
#endif

#ifdef BLUETOOTH_SUPPORT
            case ID_ENG_TEST_BLUETOOTH :
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITESTBLUETOOTH_WIN_TAB,  PNULL);
                break;
#endif
#ifndef PLATFORM_ANTISW3
            case ID_ENG_TEST_HEADSET :
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITESTHEADSET_WIN_TAB,  PNULL);
                break;
#endif
            case ID_ENG_TEST_MELODY:
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITESTMELODY_WIN_TAB,  PNULL);
                break;

#if defined MSDC_CARD_SUPPORT && !defined(PLATFORM_UWS6121E)
#ifndef ADULT_WATCH_SUPPORT //bug1613981
            case ID_ENG_TEST_SDCARD:
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITESTSD_WIN_TAB,  PNULL);
                break;
#endif
#endif

#ifdef APP_ENG_COMTEST_SUPPORT
            case ID_ENG_TEST_COM:
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITESTCOM_WIN_TAB,  PNULL);
                break;
#endif

#ifdef FM_SUPPORT
            case ID_ENG_TEST_FM:
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITESTFM_WIN_TAB,  PNULL);
                break;
#endif

#ifdef MMI_ENG_WIFI_SUPPORT
            case ID_ENG_TEST_WIFI:
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITESTWIFI_WIN_TAB,  PNULL);
                break;
#endif
#ifdef GPS_SUPPORT
            case ID_ENG_TEST_GPS:
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITEST_GPS_WIN_TAB,  PNULL);
                break;
#endif
#ifdef MOTION_SENSOR_TYPE
            case ID_ENG_TEST_MOTION_SENSOR:
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_MOTIONSENSOR_WIN_TAB,  PNULL);
                break;
#endif

#ifdef ATV_SUPPORT
             case ID_ENG_TEST_ATV:
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITESTATV_CHANNEL_WIN_TAB,  PNULL);
                break;
#endif

             case ID_ENG_TEST_CHARGING:
    			s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITEST_CHARGING_WIN_TAB,  PNULL);
                break;
              case ID_ENG_TEST_VERSION:
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_SHOWVERSION_WIN_TAB,  PNULL);
                break;
#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
              case ID_ENG_TEST_TPTEST:
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_TOUCHSCREEN_MENU_WIN_TAB,  PNULL);
                break;
#endif
#ifdef ENG_SUPPORT
               case ID_ENG_TEST_SIM :
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITESTSIM_WIN_TAB,  PNULL);
                break;
#endif
#ifdef SENSORHUB_SUPPORT
#ifndef UAL_FACTORY_TEST_SUPPORT
            case ID_ENG_SENSORHUB_TEST:
                s_is_fulltest = 0 ;
                MMK_CreateWin((uint32*)MMIENG_UITEST_SENSORHUB_WIN_TAB,  PNULL);  
                break;
#endif
#endif
            case ID_ENG_EMCC_TEST:
                    {
                        uint8 *temp_str = "10086";
                        s_is_fulltest = 0 ;
                        MMK_CreateWin((uint32*)MMIENG_UITEST_EMCC_WIN_TAB,  PNULL);

                        WatchAPICC_MakeCall(
                            MN_DUAL_SYS_MAX,
                            temp_str,
                            5,
                            PNULL,
                            PNULL,
                            CC_CALL_SIM_MAX,
                            CC_CALL_NORMAL_CALL,
                            PNULL
                            );
                     }


                break;
#ifdef FT_WIFI_SUPPORT_STANDARD
            case ID_ENG_TEST_SETMODE:
#ifndef WIN32
                MMIENGFactorySetMode_Enter();
#endif
                break;
            case ID_ENG_TEST_SETWIFIPARAM:
#ifndef WIN32
                MMIENGFactorySetParam_Enter();
#endif
                break;
            case ID_ENG_TEST_SOCKETCONN:
#ifndef WIN32
                MMIENGFactorySocketConn_Enter();
#endif
                break;
#endif

		case ID_ENG_NTC_TEST :
			s_is_fulltest = 0 ;
			MMK_CreateWin((uint32*)MMIENG_UITEST_NTC_WIN_TAB,  PNULL); 
		break;

            default:
                break;
        }
                break;

            default:
                result = MMI_RESULT_FALSE;
                break;
        }
    return (result);
}
#endif

#ifdef MEDIA_DEMO_SUPPORT

//typedef unsigned char   BOOLEAN;
LOCAL uint8 s_yuv_demo_type=0;

LOCAL uint8 lose_focus_flag=0;

uint8 get_video_state(void)
{
    return s_video_cur_state;
}

uint8 get_audio_state(void)
{
    return s_audio_cur_state;
}

BOOLEAN check_sdcard(void)
{

    return MMIAPISD_IsSDPlugIn(MMI_DEVICE_SDCARD);
}



LOCAL void GUIMENU_SetNodeGrayed_DO(BOOLEAN is_grayed,uint16 pstart, uint16 pend)
{   
    uint16 i=0;
   
    for(i=pstart;i<=pend;i++)
    {
        GUIMENU_SetNodeGrayed(is_grayed, i, NULL, MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);
    }

    #ifndef CAT_MVOICE_SUPPORT
           is_grayed=1;
           i=6;
           GUIMENU_SetNodeGrayed(is_grayed, i, NULL, MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);

    #endif
      
    #ifndef MIC_SAMPLE_SUPPORT
            is_grayed=1;
            i=7;
            GUIMENU_SetNodeGrayed(is_grayed, i, NULL, MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);
    #endif
    
}

#ifndef WIN32
/********************************************************************************
NAME:          ENGVideoDemoWinHandleMsg
DESCRIPTION:   audio string test 
PARAM IN:      
PARAM OUT:     
AUTHOR:        merlin.yang
DATE:          2011.03.20
********************************************************************************/
LOCAL  MMI_RESULT_E ENGVideoDemoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 menu_id = 0;
    
    MMI_TEXT_ID_T txt_id[3]={TXT_ENG_MEDIA_VIDEO_YUV422_2P,TXT_ENG_MEDIA_VIDEO_YUV420_2P,TXT_ENG_MEDIA_VIDEO_YUV420_3P};

    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            s_yuv_demo_type=0; 
            s_video_cur_state = VIDEO_DEMO_STOP;
            //SCI_TRACE_LOW:"ENGVideoDemoWinHandleMsg MSG_OPEN_WINDOW s_video_cur_state = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_6463_112_2_18_2_17_36_84,(uint8*)"d", s_video_cur_state);
            MMK_SetAtvCtrl(win_id, MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        break;
        case  MSG_GET_FOCUS:

            menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);

            //SCI_TRACE_LOW:"ENGVideoDemoWinHandleMsg MSG_GET_FOCUS s_video_cur_state = %d,menu_id=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_6471_112_2_18_2_17_36_85,(uint8*)"dd", s_video_cur_state,menu_id);

            if(s_video_cur_state == VIDEO_DEMO_STOP && lose_focus_flag==1)
            {
                lose_focus_flag=0;
                switch (menu_id)
                {
                    case ID_ENG_VIDEO_DC_DEMO:
                        GUIMENU_SetNodeGrayed(TRUE, 1, NULL, MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, TXT_NULL, TXT_NULL, TXT_ENG_MEDIA_DEMO_STOP);
                        MMK_SendMsg(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID,MSG_CTL_PAINT,0); 
                        DCVIDEO_Demo_Start();  
                        s_video_cur_state = VIDEO_DEMO_START;

                    break;

                    default:
                    break; 
                }
                
                //SCI_TRACE_LOW:"ENGVideoDemoWinHandleMsg MSG_GET_FOCUS s_video_cur_state = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_6491_112_2_18_2_17_36_86,(uint8*)"d", s_video_cur_state);

            }
            break;
            
        case MSG_LOSE_FOCUS:
            //s_yuv_demo_type=0; 
            
            if(s_video_cur_state != VIDEO_DEMO_STOP)
            {
                lose_focus_flag=1; 
                menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
                switch (menu_id)
                {
                    case ID_ENG_VIDEO_DC_DEMO:
                        
                        DCVIDEO_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_START, TXT_ENG_RETURN);
                        s_video_cur_state = VIDEO_DEMO_STOP;
                        GUIMENU_SetNodeGrayed(FALSE, 1, NULL, MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
                    break;

                    case ID_ENG_VIDEO_YUV_DEMO:                    
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_START, TXT_ENG_RETURN);
                        s_video_cur_state = VIDEO_DEMO_STOP;
                        GUIMENU_SetNodeGrayed(FALSE, 0, NULL, MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
                    break;  

                    default:
                    break;
                }
                MMK_PostMsg(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
            }
        break;
        
        case MSG_CLOSE_WINDOW:
            s_yuv_demo_type=0;
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            //SCI_TRACE_LOW:"ENGVideoDemoWinHandleMsg MSG_OPEN_WINDOW s_video_cur_state = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_6529_112_2_18_2_17_36_87,(uint8*)"d", s_video_cur_state);

            if(s_video_cur_state != VIDEO_DEMO_STOP)
            {
                menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
                switch (menu_id)
                {
                    case ID_ENG_VIDEO_DC_DEMO:
                        DCVIDEO_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_START, TXT_ENG_RETURN);
                        s_video_cur_state = VIDEO_DEMO_STOP;
                        GUIMENU_SetNodeGrayed(FALSE, 1, NULL, MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
                    break;

                    case ID_ENG_VIDEO_YUV_DEMO:                    
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_START, TXT_ENG_RETURN);
                        s_video_cur_state = VIDEO_DEMO_STOP;
                        GUIMENU_SetNodeGrayed(FALSE, 0, NULL, MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
                    break;  

                    default:
                    break;
                }
                MMK_PostMsg(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
            }
            break;
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            s_yuv_demo_type=0;
            //SCI_TRACE_LOW:"ENGVideoDemoWinHandleMsg MSG_OPEN_WINDOW s_video_cur_state = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_6558_112_2_18_2_17_37_88,(uint8*)"d", s_video_cur_state);

            if(s_video_cur_state == VIDEO_DEMO_STOP)
            {
                MMK_CloseWin(win_id);    
            }
            else
            {
                menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
                switch (menu_id)
                {
                    case ID_ENG_VIDEO_DC_DEMO:
                        DCVIDEO_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_START, TXT_ENG_RETURN);
                        s_video_cur_state = VIDEO_DEMO_STOP;
                        GUIMENU_SetNodeGrayed(FALSE, 1, NULL, MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
                    break;

                    case ID_ENG_VIDEO_YUV_DEMO:                    
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_START, TXT_ENG_RETURN);
                        s_video_cur_state = VIDEO_DEMO_STOP;
                        GUIMENU_SetNodeGrayed(FALSE, 0, NULL, MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
                    break;  

                    default:
                    break;
                }
                MMK_PostMsg(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
            }
        break;

        case MSG_APP_RED:
            s_yuv_demo_type=0;
            
            if(s_video_cur_state != VIDEO_DEMO_STOP)
            {
                menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
                switch (menu_id)
                {
                    case ID_ENG_VIDEO_DC_DEMO:
                        DCVIDEO_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_START, TXT_ENG_RETURN);
                    break;

                    case ID_ENG_VIDEO_YUV_DEMO:
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_START, TXT_ENG_RETURN);
                    break;   
                    
                    default:
                    break; 
                
                }
                s_video_cur_state = VIDEO_DEMO_STOP;
            }
            MMK_CloseWin(win_id);    
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
            if(s_video_cur_state == VIDEO_DEMO_STOP)
            {
                switch (menu_id)
                {
                    case ID_ENG_VIDEO_DC_DEMO:
                        GUIMENU_SetNodeGrayed(TRUE, 1, NULL, MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, TXT_NULL, TXT_NULL, TXT_ENG_MEDIA_DEMO_STOP);
                        MMK_SendMsg(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID,MSG_CTL_PAINT,0); 
                        DCVIDEO_Demo_Start();  
                    break;
                 
                    case ID_ENG_VIDEO_YUV_DEMO: 
                        GUIMENU_SetNodeGrayed(TRUE, 0, NULL, MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
                        GUIWIN_SetSoftkeyTextId(win_id,txt_id[s_yuv_demo_type],TXT_ENG_MEDIA_DEMO_NEXT,TXT_ENG_MEDIA_DEMO_STOP,TRUE);
                        // GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, txt_id[s_yuv_demo_type], TXT_ENG_MEDIA_DEMO_NEXT, TXT_ENG_MEDIA_DEMO_STOP);
                        // MMK_SendMsg(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID,MSG_CTL_PAINT,0);
                        // SCI_Sleep(100);
                        Video_Demo_yuv_Start(s_yuv_demo_type);
                    break;

                    default:
                    break; 
                }
                s_video_cur_state = VIDEO_DEMO_START;
            }
            else if(s_video_cur_state == VIDEO_DEMO_START)
            {
                switch (menu_id)
                {
                    case ID_ENG_VIDEO_YUV_DEMO:
                        s_yuv_demo_type++;
                        if(s_yuv_demo_type>2)
                        {
                            s_yuv_demo_type=0;
                        }
                        // GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, txt_id[s_yuv_demo_type], TXT_ENG_MEDIA_DEMO_NEXT, TXT_ENG_MEDIA_DEMO_STOP);
                        GUIWIN_SetSoftkeyTextId(win_id,txt_id[s_yuv_demo_type],TXT_ENG_MEDIA_DEMO_NEXT,TXT_ENG_MEDIA_DEMO_STOP,TRUE);
                        Video_Demo_yuv_Start(s_yuv_demo_type);
                    break;

                    default:
                    break;   
                }
            }
            //MMK_PostMsg(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
        break; 
        
        case MSG_MEDIA_DEMO_APP_UP:
            //SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_APP_UP s_video_cur_state = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_6669_112_2_18_2_17_37_89,(uint8*)"d", s_video_cur_state);
            MMK_PostMsg(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);

        break;

        case MSG_MEDIA_DEMO_APP_DOWN:
            //SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_APP_DOWN s_video_cur_state = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_6675_112_2_18_2_17_37_90,(uint8*)"d", s_video_cur_state);
            MMK_PostMsg(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);

        break;

        default:
            result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


/********************************************************************************
NAME:          ENGAudioDemoWinHandleMsg
DESCRIPTION:   audio string test 
PARAM IN:      
PARAM OUT:     
AUTHOR:        merlin.yang
DATE:          2011.03.20
********************************************************************************/
LOCAL  MMI_RESULT_E ENGAudioDemoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 menu_id = 0;
    uint8   title_str[40] = {0};
    wchar   title_wstr[40] = {0};
    MMI_STRING_T    str_info = {0};
    static MMISRV_HANDLE_T virtual_handle = 0;
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            s_audio_cur_state = AUDIO_STREAM_STOP;
            //SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_OPEN_WINDOW s_audio_cur_state = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_6707_112_2_18_2_17_37_91,(uint8*)"d", s_audio_cur_state);
            MMK_SetAtvCtrl(win_id, MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);
            virtual_handle = MMIENG_RequestVirtualAudService();
            break;

        case MSG_CLOSE_WINDOW:
            //SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_OPEN_WINDOW s_audio_cur_state = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_6713_112_2_18_2_17_37_92,(uint8*)"d", s_audio_cur_state);
            if(s_audio_cur_state != AUDIO_STREAM_STOP)
            {

                MMISRVMGR_Free(virtual_handle);
                virtual_handle = 0;
                
                menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);
                switch (menu_id)
                {
                    case ID_ENG_AUDIO_AAC_STRING_DEMO:
                        AAC_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                        s_audio_cur_state = AUDIO_STREAM_STOP;
                        GUIMENU_SetNodeGrayed_DO(FALSE,1,7);
                    break;

                    case ID_ENG_AUDIO_AMR_STRING_PLAY_DEMO:                    
                        AMR_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                        s_audio_cur_state = AUDIO_STREAM_STOP;
                        GUIMENU_SetNodeGrayed_DO(FALSE,0,0);
                        GUIMENU_SetNodeGrayed_DO(FALSE,2,7);
                    break;  

                    case ID_ENG_AUDIO_MP3_STRING_DEMO :
                        MP3_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                        s_audio_cur_state = AUDIO_STREAM_STOP;
                        GUIMENU_SetNodeGrayed_DO(FALSE,0,1);
                        GUIMENU_SetNodeGrayed_DO(FALSE,3,7);
                    break;

                    case ID_ENG_AUDIO_WAV_STRING_DEMO :
                        WAV_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                        s_audio_cur_state = AUDIO_STREAM_STOP;
                        GUIMENU_SetNodeGrayed_DO(FALSE,0,2);
                        GUIMENU_SetNodeGrayed_DO(FALSE,4,7);
                    break;

                    case ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO :
                        if(s_audio_cur_state == AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS)//录音
                        {
                            AMR_Recode_Demo_Stop();
                            GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_NULL);
                            s_audio_cur_state = AUDIO_STREAM_PAUSE_OR_STOP_RECORD_OR_LEFT_TRACK;//录音停止
                            }
                        else if(s_audio_cur_state == AUDIO_STREAM_RESUME_OR_PLAYBACK_OR_RIGHT_TRACK)//回放
                        {
                            AMR_Recode_Demo_PlayBack_Stop();//停止回放的函数接口，需要写一个函数。
                            GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RECORD, TXT_ENG_RETURN);
                            s_audio_cur_state = AUDIO_STREAM_STOP;//录音停止
                            GUIMENU_SetNodeGrayed_DO(FALSE,0,3);
                            GUIMENU_SetNodeGrayed_DO(FALSE,5,7);
                        }
                    break;

                    case ID_ENG_AUDIO_EXPRESS_DEMO:
                        MP3_Express_Demo_Stop();
                        AUDIO_Express_Demo_Stop();
                        s_audio_cur_state = AUDIO_STREAM_STOP;
                        GUIMENU_SetNodeGrayed_DO(FALSE,0,4);
                        GUIMENU_SetNodeGrayed_DO(FALSE,6,7);
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                    break; 
                    
                    case ID_ENG_AUDIO_CAT_MVOICE_DEMO:
                        #ifdef CAT_MVOICE_SUPPORT 
                        CatMv_Demo_Stop();
                        #endif
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                        s_audio_cur_state = AUDIO_STREAM_STOP;
                        GUIMENU_SetNodeGrayed_DO(FALSE,0,5);
                        GUIMENU_SetNodeGrayed_DO(FALSE,7,7);
                    break;

                    case ID_ENG_AUDIO_MIC_SAMPLE_DEMO:
                        #ifdef MIC_SAMPLE_SUPPORT
                        MIC_Sample_Demo_Stop();
                        #endif
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                        s_audio_cur_state = AUDIO_STREAM_STOP;	    
                        GUIMENU_SetNodeGrayed_DO(FALSE,0,6);
                    break;     

                    default:
                    break;
                }
                MMK_PostMsg(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
            }
        break;
            
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            //SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_OPEN_WINDOW s_audio_cur_state = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_6808_112_2_18_2_17_37_93,(uint8*)"d", s_audio_cur_state);
            if(s_audio_cur_state == AUDIO_STREAM_STOP)
            {
                MMISRVMGR_Free(virtual_handle);
                virtual_handle = 0;
            }
            else
            {
                menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);
                switch (menu_id)
                {
                    case ID_ENG_AUDIO_AAC_STRING_DEMO:
                        AAC_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                        s_audio_cur_state = AUDIO_STREAM_STOP;
                        GUIMENU_SetNodeGrayed_DO(FALSE,1,7);
                    break;

                    case ID_ENG_AUDIO_AMR_STRING_PLAY_DEMO:                    
                        AMR_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                        s_audio_cur_state = AUDIO_STREAM_STOP;
                        GUIMENU_SetNodeGrayed_DO(FALSE,0,0);
                        GUIMENU_SetNodeGrayed_DO(FALSE,2,7);
                    break;  

                    case ID_ENG_AUDIO_MP3_STRING_DEMO :
                        MP3_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                        s_audio_cur_state = AUDIO_STREAM_STOP;
                        GUIMENU_SetNodeGrayed_DO(FALSE,0,1);
                        GUIMENU_SetNodeGrayed_DO(FALSE,3,7);
                    break;

                    case ID_ENG_AUDIO_WAV_STRING_DEMO :
                        WAV_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                        s_audio_cur_state = AUDIO_STREAM_STOP;
                        GUIMENU_SetNodeGrayed_DO(FALSE,0,2);
                        GUIMENU_SetNodeGrayed_DO(FALSE,4,7);
                    break;

                    case ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO :
                        if(s_audio_cur_state == AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS)//录音
                        {
                            AMR_Recode_Demo_Stop();
                            GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_NULL);
                            s_audio_cur_state = AUDIO_STREAM_PAUSE_OR_STOP_RECORD_OR_LEFT_TRACK;//录音停止
                            }
                        else if(s_audio_cur_state == AUDIO_STREAM_RESUME_OR_PLAYBACK_OR_RIGHT_TRACK)//回放
                        {
                            AMR_Recode_Demo_PlayBack_Stop();//停止回放的函数接口，需要写一个函数。
                            GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RECORD, TXT_ENG_RETURN);
                            s_audio_cur_state = AUDIO_STREAM_STOP;//录音停止
                            GUIMENU_SetNodeGrayed_DO(FALSE,0,3);
                            GUIMENU_SetNodeGrayed_DO(FALSE,5,7);
                        }
                    break;

                    case ID_ENG_AUDIO_EXPRESS_DEMO:
                        MP3_Express_Demo_Stop();
                        AUDIO_Express_Demo_Stop();
                        s_audio_cur_state = AUDIO_STREAM_STOP;
                        GUIMENU_SetNodeGrayed_DO(FALSE,0,4);
                        GUIMENU_SetNodeGrayed_DO(FALSE,6,7);
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                    break; 
                    
                    case ID_ENG_AUDIO_CAT_MVOICE_DEMO:
                        #ifdef CAT_MVOICE_SUPPORT 
                        CatMv_Demo_Stop();
                        #endif
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                        s_audio_cur_state = AUDIO_STREAM_STOP;
                        GUIMENU_SetNodeGrayed_DO(FALSE,0,5);
                        GUIMENU_SetNodeGrayed_DO(FALSE,7,7);
                    break;

                    case ID_ENG_AUDIO_MIC_SAMPLE_DEMO:
                        #ifdef MIC_SAMPLE_SUPPORT
                        MIC_Sample_Demo_Stop();
                        #endif
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                        s_audio_cur_state = AUDIO_STREAM_STOP;	    
                        GUIMENU_SetNodeGrayed_DO(FALSE,0,6);
                    break;     

                    default:
                    break;
                }
                MMK_PostMsg(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
            }
        break;
        case MSG_APP_RED:
            if(s_audio_cur_state != AUDIO_STREAM_STOP)
            {
                menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);
                switch (menu_id)
                {
                    case ID_ENG_AUDIO_AAC_STRING_DEMO:
                        AAC_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                    break;

                    case ID_ENG_AUDIO_AMR_STRING_PLAY_DEMO:                    
                        AMR_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                    break;  

                    case  ID_ENG_AUDIO_MP3_STRING_DEMO :
                        MP3_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                    break;

                    case  ID_ENG_AUDIO_WAV_STRING_DEMO :
                        WAV_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                    break;

                    case  ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO :
                        if(s_audio_cur_state == AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS)
                        {
                            AMR_Recode_Demo_Stop();
                        }
                        else if(s_audio_cur_state == AUDIO_STREAM_RESUME_OR_PLAYBACK_OR_RIGHT_TRACK)
                        {
                            AMR_Recode_Demo_PlayBack_Stop();
                        }
                        //GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RECORD, TXT_NULL);
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RECORD, TXT_ENG_RETURN);
                    break;

                    case ID_ENG_AUDIO_EXPRESS_DEMO:
                        MP3_Express_Demo_Stop();
                        AUDIO_Express_Demo_Stop();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                    break;
                    
                    case ID_ENG_AUDIO_CAT_MVOICE_DEMO:
                        #ifdef CAT_MVOICE_SUPPORT 
                        CatMv_Demo_Stop();
                        #endif
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                    break;
                    
                    case ID_ENG_AUDIO_MIC_SAMPLE_DEMO:
                        #ifdef MIC_SAMPLE_SUPPORT
                        MIC_Sample_Demo_Stop();
                        #endif
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                    break;

                    default:
                    break;   
                }
                s_audio_cur_state = AUDIO_STREAM_STOP;
            }
            MMISRVMGR_Free(virtual_handle);
            virtual_handle = 0;
            break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);
            if(s_audio_cur_state == AUDIO_STREAM_STOP)
            {
                switch (menu_id)
                {
                    case ID_ENG_AUDIO_AAC_STRING_DEMO:
                        AAC_Demo_Start();
                        GUIMENU_SetNodeGrayed_DO(TRUE,1,7);
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE,TXT_ENG_MEDIA_DEMO_STOP);
                        s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
                    break;

                    case ID_ENG_AUDIO_AMR_STRING_PLAY_DEMO:                    
                        AMR_Demo_Start();
                        GUIMENU_SetNodeGrayed_DO(TRUE,0,0);
                        GUIMENU_SetNodeGrayed_DO(TRUE,2,7);
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE, TXT_ENG_MEDIA_DEMO_STOP);
                        s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
                    break;

                    case ID_ENG_AUDIO_MP3_STRING_DEMO:                    
                        MP3_Demo_Start();
                        GUIMENU_SetNodeGrayed_DO(TRUE,0,1);
                        GUIMENU_SetNodeGrayed_DO(TRUE,3,7);

                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE, TXT_ENG_MEDIA_DEMO_STOP);
                        s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
                    break;

                    case ID_ENG_AUDIO_WAV_STRING_DEMO:
                        WAV_Demo_Start();

                        GUIMENU_SetNodeGrayed_DO(TRUE,0,2);
                        GUIMENU_SetNodeGrayed_DO(TRUE,4,7);

                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE,  TXT_ENG_MEDIA_DEMO_STOP);
                        s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
                    break;

                    case ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO:
                        AMR_Recode_Demo_Start();

                        GUIMENU_SetNodeGrayed_DO(TRUE,0,3);
                        GUIMENU_SetNodeGrayed_DO(TRUE,5,7);

                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_NULL, TXT_ENG_MEDIA_DEMO_STOP);
                        s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
                    break;

                    case ID_ENG_AUDIO_EXPRESS_DEMO: 
                        AUDIO_Express_Demo_Start();
                        AUDIO_Express_Demo_SetPar(3);//play right +left track
                        if(MP3_Express_Demo_Start())
                        {
                            GUIMENU_SetNodeGrayed_DO(TRUE,0,4);
                            GUIMENU_SetNodeGrayed_DO(TRUE,6,7);
                            GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_AUDIO_STREAM_LEFT_TRACK, TXT_ENG_MEDIA_DEMO_STOP);
                            s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
                        }
                        else
                        {
                            MMIPUB_OpenAlertFailWin(TXT_ENG_MEDIA_AUDIO_EXPRESSION_CARD);
                        }
                    break;
                    
                    case ID_ENG_AUDIO_CAT_MVOICE_DEMO: 

                        if(check_sdcard())
                        {
                        #ifdef CAT_MVOICE_SUPPORT 
                            CatMv_Demo_Start();
                        #endif
                            GUIMENU_SetNodeGrayed_DO(TRUE,0,5);
                            GUIMENU_SetNodeGrayed_DO(TRUE,7,7);
                            GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_AUDIO_VCOIE, TXT_ENG_MEDIA_DEMO_STOP);
                            s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
                        }
                        else
                        {
                            MMIPUB_OpenAlertFailWin(TXT_ENG_MEDIA_AUDIO_CAT_MVOICE_CARD);
                        }
                    break;
                    
                    case ID_ENG_AUDIO_MIC_SAMPLE_DEMO: 
                        #ifdef MIC_SAMPLE_SUPPORT
                        MIC_Sample_Demo_Start();
                        #endif
                        GUIMENU_SetNodeGrayed_DO(TRUE,0,6);

                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_NULL, TXT_ENG_MEDIA_DEMO_STOP);
                        s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
                    break;
                    
                    default:
                    break; 
                }
            }
            else if(s_audio_cur_state == AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS)
            {
                switch (menu_id)
                {
                    case ID_ENG_AUDIO_AAC_STRING_DEMO:
                        AAC_Demo_Pause();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RESUME, TXT_ENG_MEDIA_DEMO_STOP);
                    break;

                    case ID_ENG_AUDIO_AMR_STRING_PLAY_DEMO:                    
                        AMR_Demo_Pause();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RESUME, TXT_ENG_MEDIA_DEMO_STOP);
                    break;

                    case ID_ENG_AUDIO_MP3_STRING_DEMO:                    
                        MP3_Demo_Pause();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RESUME, TXT_ENG_MEDIA_DEMO_STOP);
                    break;

                    case ID_ENG_AUDIO_WAV_STRING_DEMO:
                        WAV_Demo_Pause();
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RESUME, TXT_ENG_MEDIA_DEMO_STOP);
                    break;

                    case ID_ENG_AUDIO_EXPRESS_DEMO: 
                        AUDIO_Express_Demo_SetPar(1); //play left track
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, 
                        TXT_ENG_MEDIA_AUDIO_STREAM_RIGHT_TRACK, TXT_ENG_MEDIA_DEMO_STOP);
                    break;

                    case ID_ENG_AUDIO_CAT_MVOICE_DEMO: 
                        #ifdef CAT_MVOICE_SUPPORT 
                         CatMv_RegExpOnOff(SCI_FALSE);
                        #endif
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_AUDIO_MVCOIE, TXT_ENG_MEDIA_DEMO_STOP);
                    break;    

                        
                    default:
                    break;   
                }
                if(menu_id!=ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO)	
                {
                    s_audio_cur_state = AUDIO_STREAM_PAUSE_OR_STOP_RECORD_OR_LEFT_TRACK;
                }
            }
            else if(s_audio_cur_state == AUDIO_STREAM_PAUSE_OR_STOP_RECORD_OR_LEFT_TRACK)
            {
                switch (menu_id)
                {
                    case ID_ENG_AUDIO_AAC_STRING_DEMO:
                        AAC_Demo_Resume();
                        s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE, TXT_ENG_MEDIA_DEMO_STOP);
                    break;

                    case ID_ENG_AUDIO_AMR_STRING_PLAY_DEMO:                    
                        AMR_Demo_Resume();
                        s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE, TXT_ENG_MEDIA_DEMO_STOP);
                    break;

                    case ID_ENG_AUDIO_MP3_STRING_DEMO:                    
                        MP3_Demo_Resume();
                        s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE, TXT_ENG_MEDIA_DEMO_STOP);
                    break;

                    case ID_ENG_AUDIO_WAV_STRING_DEMO:
                        WAV_Demo_Resume();
                        s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE, TXT_ENG_MEDIA_DEMO_STOP);
                    break;

                    case ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO:
                        AMR_Recode_Demo_PlayBack();//回放的接口函数
                        s_audio_cur_state = AUDIO_STREAM_RESUME_OR_PLAYBACK_OR_RIGHT_TRACK;
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_NULL, TXT_ENG_MEDIA_DEMO_STOP);
                    break;

                    case ID_ENG_AUDIO_EXPRESS_DEMO:   
                        AUDIO_Express_Demo_SetPar(2);//play right track
                        s_audio_cur_state = AUDIO_STREAM_RESUME_OR_PLAYBACK_OR_RIGHT_TRACK;
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_AUDIO_STREAM_LEFT_AND_RIGHT, TXT_ENG_MEDIA_DEMO_STOP);
                    break;
                    
                    case ID_ENG_AUDIO_CAT_MVOICE_DEMO: 
                        #ifdef CAT_MVOICE_SUPPORT 
                        CatMv_RegExpOnOff(1);
                        #endif
                        s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_AUDIO_VCOIE, TXT_ENG_MEDIA_DEMO_STOP);
                    break;  

                    default:
                    break;
                }
            }
            else if(s_audio_cur_state == AUDIO_STREAM_RESUME_OR_PLAYBACK_OR_RIGHT_TRACK)
            {
                switch (menu_id)
                { 
                    case ID_ENG_AUDIO_EXPRESS_DEMO:
                        AUDIO_Express_Demo_SetPar(3);
                        s_audio_cur_state = AUDIO_STREAM_LEFT_AND_RIGHT_TRACK;
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_AUDIO_STREAM_LEFT_TRACK, TXT_ENG_MEDIA_DEMO_STOP);
                    break;
                    
                    default:
                    break;    
                }
            }
            else if(s_audio_cur_state == AUDIO_STREAM_LEFT_AND_RIGHT_TRACK)
            {
                switch (menu_id)
                { 
                    case ID_ENG_AUDIO_EXPRESS_DEMO:
                        AUDIO_Express_Demo_SetPar(1);
                        s_audio_cur_state = AUDIO_STREAM_PAUSE_OR_STOP_RECORD_OR_LEFT_TRACK;
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_AUDIO_STREAM_RIGHT_TRACK, TXT_ENG_MEDIA_DEMO_STOP);
                    break;
                    
                    default:
                    break; 
                }
            }
            MMK_PostMsg(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
        break; 
        
        case MSG_MEDIA_DEMO_APP_UP:

            //SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_APP_UP s_audio_cur_state = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7201_112_2_18_2_17_38_94,(uint8*)"d", s_audio_cur_state);
            if(s_audio_cur_state == AUDIO_STREAM_STOP)
            {
                menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);
                //SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_APP_UP menu_id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7205_112_2_18_2_17_38_95,(uint8*)"d", menu_id);
                switch (menu_id)
                {
                    case ID_ENG_AUDIO_EXPRESS_DEMO:
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                    break;
                    
                    case ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO:
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RECORD, TXT_ENG_RETURN);
                    break;
                    
                    case ID_ENG_AUDIO_WAV_STRING_DEMO:
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                    break;
                    
                    default:
                    break; 
                }
                MMK_PostMsg(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
            }
        break;

        case MSG_MEDIA_DEMO_APP_DOWN:

            //SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_APP_DOWN s_audio_cur_state = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7229_112_2_18_2_17_38_96,(uint8*)"d", s_audio_cur_state);

            if(s_audio_cur_state == AUDIO_STREAM_STOP)
            {
                menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);

                //SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_APP_DOWN menu_id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7235_112_2_18_2_17_38_97,(uint8*)"d", menu_id);
                switch (menu_id)
                {
                    case ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO:
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RECORD, TXT_ENG_RETURN);
                    break;
                    
                    case ID_ENG_AUDIO_EXPRESS_DEMO:
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                    break;
                    
                    case ID_ENG_AUDIO_AAC_STRING_DEMO:
                        GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
                    break;
                    
                    default:
                    break; 
                    }
                MMK_PostMsg(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
            }
        break;
        
        case MSG_MEDIA_DEMO_SHOW_DATE:
            if(s_audio_cur_state != AUDIO_STREAM_STOP)
            {
                menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);

                //SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_MEDIA_DEMO_SHOW_DATE MSG_APP_DOWN menu_id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7262_112_2_18_2_17_38_98,(uint8*)"d", menu_id);
                switch (menu_id)
                {
                    case ID_ENG_AUDIO_MIC_SAMPLE_DEMO:
#if 1
                        sprintf((char*)title_str, "MIC dB: %d",  (*(int32*)param));
                        MMIAPICOM_StrToWstr( (const uint8*)title_str, title_wstr );
                        str_info.wstr_len = MMIAPICOM_Wstrlen(title_wstr);
                        str_info.wstr_ptr = title_wstr;
                        GUIWIN_SetTitleText(win_id,str_info.wstr_ptr,str_info.wstr_len,TRUE);
#endif
                    break;

                    default:
                    break;
                }
                //MK_PostMsg(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
            }
        break;

        default:
            result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

#else

/********************************************************************************
NAME:          ENGVideoDemoWinHandleMsg
DESCRIPTION:   audio string test 
PARAM IN:      
PARAM OUT:     
AUTHOR:        merlin.yang
DATE:          2011.03.20
********************************************************************************/
LOCAL  MMI_RESULT_E ENGVideoDemoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 menu_id = 0;
    
    MMI_TEXT_ID_T txt_id[3]={TXT_ENG_MEDIA_VIDEO_YUV422_2P,TXT_ENG_MEDIA_VIDEO_YUV420_2P,TXT_ENG_MEDIA_VIDEO_YUV420_3P};

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
              s_yuv_demo_type=0; 
		s_video_cur_state = VIDEO_DEMO_STOP;
		//SCI_TRACE_LOW:"ENGVideoDemoWinHandleMsg MSG_OPEN_WINDOW s_video_cur_state = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7310_112_2_18_2_17_38_99,(uint8*)"d", s_video_cur_state);
              MMK_SetAtvCtrl(win_id, MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
        break;
    case MSG_CLOSE_WINDOW:	
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        s_yuv_demo_type=0;
		//SCI_TRACE_LOW:"ENGVideoDemoWinHandleMsg MSG_OPEN_WINDOW s_video_cur_state = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7317_112_2_18_2_17_38_100,(uint8*)"d", s_video_cur_state);

		if(s_video_cur_state == VIDEO_DEMO_STOP)
		{
                  MMK_CloseWin(win_id);    
		}
		else
		{
		    menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
            switch (menu_id)
            {
            case ID_ENG_VIDEO_DC_DEMO:
			
				//DCVIDEO_Demo_Stop();
			
				GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_START, TXT_ENG_RETURN);
				s_video_cur_state = VIDEO_DEMO_STOP;
                GUIMENU_SetNodeGrayed(FALSE, 1, NULL, MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);

                break;
                
            case ID_ENG_VIDEO_YUV_DEMO:                    
    			
				GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_START, TXT_ENG_RETURN);
				s_video_cur_state = VIDEO_DEMO_STOP;
                           GUIMENU_SetNodeGrayed(FALSE, 0, NULL, MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);

                break;  

            default:
                break;
            }
			MMK_PostMsg(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
		}
        break;

	case MSG_APP_RED:
	    s_yuv_demo_type=0;
	    if(s_video_cur_state != VIDEO_DEMO_STOP)
	    {
	        menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
            switch (menu_id)
            {
                case ID_ENG_VIDEO_DC_DEMO:
    				//DCVIDEO_Demo_Stop();
    				GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_START, TXT_ENG_RETURN);
                    break;
      
                case ID_ENG_VIDEO_YUV_DEMO:
    				GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_START, TXT_ENG_RETURN);
                    break;                
                default:
                    break;   
	        }
			s_video_cur_state = VIDEO_DEMO_STOP;
	    }
        MMK_CloseWin(win_id);    
		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
		 menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
		 if(s_video_cur_state == VIDEO_DEMO_STOP)
		 {
             switch (menu_id)
             {
                case ID_ENG_VIDEO_DC_DEMO:
                	 GUIMENU_SetNodeGrayed(TRUE, 1, NULL, MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, TXT_NULL, TXT_NULL, TXT_ENG_MEDIA_DEMO_STOP);
			         MMK_SendMsg(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID,MSG_CTL_PAINT,0); 
                     //DCVIDEO_Demo_Start();  
					 break;
					 					 
                case ID_ENG_VIDEO_YUV_DEMO: 

                    GUIMENU_SetNodeGrayed(TRUE, 0, NULL, MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID);
                    GUIWIN_SetSoftkeyTextId(win_id,txt_id[s_yuv_demo_type],TXT_ENG_MEDIA_DEMO_NEXT,TXT_ENG_MEDIA_DEMO_STOP,TRUE);
				   // GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, txt_id[s_yuv_demo_type], TXT_ENG_MEDIA_DEMO_NEXT, TXT_ENG_MEDIA_DEMO_STOP);
                   // MMK_SendMsg(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID,MSG_CTL_PAINT,0);
                   // SCI_Sleep(100);
                    //Video_Demo_yuv_Start(s_yuv_demo_type);
					break;
					 
				 default:
                   break;	 
             	}
			 s_video_cur_state = VIDEO_DEMO_START;
		}
		else if(s_video_cur_state == VIDEO_DEMO_START)
		{
             switch (menu_id)
             {
                case ID_ENG_VIDEO_YUV_DEMO:
                
                     s_yuv_demo_type++;
                     if(s_yuv_demo_type>2)
                     {
                       s_yuv_demo_type=0;
                     }
                     
                    // GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID, txt_id[s_yuv_demo_type], TXT_ENG_MEDIA_DEMO_NEXT, TXT_ENG_MEDIA_DEMO_STOP);
                     GUIWIN_SetSoftkeyTextId(win_id,txt_id[s_yuv_demo_type],TXT_ENG_MEDIA_DEMO_NEXT,TXT_ENG_MEDIA_DEMO_STOP,TRUE);
                   //  Video_Demo_yuv_Start(s_yuv_demo_type);
                     break;
					 
                default:
                  break;   
             }
		}
		
		//MMK_PostMsg(MMIENG_MEDIA_VIDEO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
        break; 
    case MSG_MEDIA_DEMO_APP_UP:	
	
		//SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_APP_UP s_video_cur_state = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7435_112_2_18_2_17_38_101,(uint8*)"d", s_video_cur_state);
       break;
	
	case MSG_MEDIA_DEMO_APP_DOWN:

		//SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_APP_DOWN s_video_cur_state = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7440_112_2_18_2_17_38_102,(uint8*)"d", s_video_cur_state);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
NAME:          ENGAudioDemoWinHandleMsg
DESCRIPTION:   audio string test 
PARAM IN:      
PARAM OUT:     
AUTHOR:        merlin.yang
DATE:          2011.03.20
********************************************************************************/
LOCAL  MMI_RESULT_E ENGAudioDemoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 menu_id = 0;
    uint8   title_str[40] = {0};
    wchar   title_wstr[40] = {0};
    MMI_STRING_T    str_info = {0};

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
		s_audio_cur_state = AUDIO_STREAM_STOP;
		//SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_OPEN_WINDOW s_audio_cur_state = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7469_112_2_18_2_17_38_103,(uint8*)"d", s_audio_cur_state);
              MMK_SetAtvCtrl(win_id, MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);
		MMIAPISET_InitAudioDevice(AUD_MAX_SPEAKER_VOLUME-2);
        break;

    case MSG_CLOSE_WINDOW:
	
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
		//SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_OPEN_WINDOW s_audio_cur_state = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7478_112_2_18_2_17_38_104,(uint8*)"d", s_audio_cur_state);
		if(s_audio_cur_state == AUDIO_STREAM_STOP)
		{
                  MMK_CloseWin(win_id);    
		}
		else
		{
		    menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);
                   switch (menu_id)
	            {
		            case ID_ENG_AUDIO_AAC_STRING_DEMO:
						//AAC_Demo_Stop();
						GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
						s_audio_cur_state = AUDIO_STREAM_STOP;
		                           GUIMENU_SetNodeGrayed_DO(FALSE,1,7);
		                break;
		                
		            case ID_ENG_AUDIO_AMR_STRING_PLAY_DEMO:                    
		                            //AMR_Demo_Stop();
						
						GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
						s_audio_cur_state = AUDIO_STREAM_STOP;
		                            GUIMENU_SetNodeGrayed_DO(FALSE,0,0);
						GUIMENU_SetNodeGrayed_DO(FALSE,2,7);
		                break;  
		                   
		            case ID_ENG_AUDIO_MP3_STRING_DEMO :
		                          //  MP3_Demo_Stop();					
						GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
						s_audio_cur_state = AUDIO_STREAM_STOP;
						GUIMENU_SetNodeGrayed_DO(FALSE,0,1);
						GUIMENU_SetNodeGrayed_DO(FALSE,3,7);
			            break;
		                
		            case ID_ENG_AUDIO_WAV_STRING_DEMO :
		                           //WAV_Demo_Stop();				
						GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
						s_audio_cur_state = AUDIO_STREAM_STOP;
		                            GUIMENU_SetNodeGrayed_DO(FALSE,0,2);
						GUIMENU_SetNodeGrayed_DO(FALSE,4,7);
					    break;

					case ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO :
						if(s_audio_cur_state == AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS)//录音
						{
			                         //   AMR_Recode_Demo_Stop();
							GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_NULL);
							s_audio_cur_state = AUDIO_STREAM_PAUSE_OR_STOP_RECORD_OR_LEFT_TRACK;//录音停止
						}
						else if(s_audio_cur_state == AUDIO_STREAM_RESUME_OR_PLAYBACK_OR_RIGHT_TRACK)//回放
						{
						      // AMR_Recode_Demo_PlayBack_Stop();//停止回放的函数接口，需要写一个函数。					
							GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RECORD, TXT_ENG_RETURN);
							s_audio_cur_state = AUDIO_STREAM_STOP;//录音停止
							GUIMENU_SetNodeGrayed_DO(FALSE,0,3);
						       GUIMENU_SetNodeGrayed_DO(FALSE,5,7);
						}
		                break;
		                
		            case ID_ENG_AUDIO_EXPRESS_DEMO:
						//MP3_Express_Demo_Stop();
		                           // AUDIO_Express_Demo_Stop();
						s_audio_cur_state = AUDIO_STREAM_STOP;
						GUIMENU_SetNodeGrayed_DO(FALSE,0,4);
						GUIMENU_SetNodeGrayed_DO(FALSE,6,7);
						GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
		                break; 
		            case ID_ENG_AUDIO_CAT_MVOICE_DEMO:

		                       //  CatMv_Demo_Stop();
		                     	GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
		    				s_audio_cur_state = AUDIO_STREAM_STOP;
		    			       GUIMENU_SetNodeGrayed_DO(FALSE,0,5);
		    				GUIMENU_SetNodeGrayed_DO(FALSE,7,7);
		                break;
		                
		            case ID_ENG_AUDIO_MIC_SAMPLE_DEMO:

		                           //MIC_Sample_Demo_Stop();
  		                           GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
						s_audio_cur_state = AUDIO_STREAM_STOP;	    
						GUIMENU_SetNodeGrayed_DO(FALSE,0,6);
		                break;     
		                
		            default:
		                break;
	            }
			MMK_PostMsg(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
		}
        break;
	case MSG_APP_RED:
	    if(s_audio_cur_state != AUDIO_STREAM_STOP)
	    {
	            menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);
                   switch (menu_id)
	            {
		            case ID_ENG_AUDIO_AAC_STRING_DEMO:
					//	AAC_Demo_Stop();
						GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
		                break;
		                
		            case ID_ENG_AUDIO_AMR_STRING_PLAY_DEMO:                    
		              ///  AMR_Demo_Stop();
						GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
		                break;  
		                
		            case  ID_ENG_AUDIO_MP3_STRING_DEMO :
		            //    MP3_Demo_Stop();
						GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
		                break;
		                
		            case  ID_ENG_AUDIO_WAV_STRING_DEMO :
		             //   WAV_Demo_Stop();
						GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
		                break;

					case  ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO :
						if(s_audio_cur_state == AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS)
						{
			              //  AMR_Recode_Demo_Stop();
						}
						else if(s_audio_cur_state == AUDIO_STREAM_RESUME_OR_PLAYBACK_OR_RIGHT_TRACK)
						{
						//    AMR_Recode_Demo_PlayBack_Stop();
						}
						//GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RECORD, TXT_NULL);
						GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RECORD, TXT_ENG_RETURN);

		                break;
		                
		            case ID_ENG_AUDIO_EXPRESS_DEMO:
					//	MP3_Express_Demo_Stop();
		                //AUDIO_Express_Demo_Stop();
						
						GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
		                break;
		            case ID_ENG_AUDIO_CAT_MVOICE_DEMO:
		               // CatMv_Demo_Stop();
						
						GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
		                break;
		            case ID_ENG_AUDIO_MIC_SAMPLE_DEMO:

		                //MIC_Sample_Demo_Stop();
						
						GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
		                break;
		                
		            default:
		                break;   
		        }
			s_audio_cur_state = AUDIO_STREAM_STOP;
	    }
		
              MMK_CloseWin(win_id);    
		break;

		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
		 menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);
		 if(s_audio_cur_state == AUDIO_STREAM_STOP)
		 {
                           switch (menu_id)
		             {
		                case ID_ENG_AUDIO_AAC_STRING_DEMO:
		                     //AAC_Demo_Start();
						     
					GUIMENU_SetNodeGrayed_DO(TRUE,1,7);
					GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE,TXT_ENG_MEDIA_DEMO_STOP);
		                     s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
							 break;
							 
		                case ID_ENG_AUDIO_AMR_STRING_PLAY_DEMO:                    
		                     //AMR_Demo_Start();
		                     GUIMENU_SetNodeGrayed_DO(TRUE,0,0);
		                     GUIMENU_SetNodeGrayed_DO(TRUE,2,7);
		                     
							 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE, TXT_ENG_MEDIA_DEMO_STOP);
		                     s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
							 break;
							 
		                case ID_ENG_AUDIO_MP3_STRING_DEMO:                    
		                     //MP3_Demo_Start();
		                     GUIMENU_SetNodeGrayed_DO(TRUE,0,1);
		                     GUIMENU_SetNodeGrayed_DO(TRUE,3,7);
		                     
							 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE, TXT_ENG_MEDIA_DEMO_STOP);
		                     s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
							 break;
							 
					    case ID_ENG_AUDIO_WAV_STRING_DEMO:
		                                  //WAV_Demo_Start();

						     GUIMENU_SetNodeGrayed_DO(TRUE,0,2);
		                     GUIMENU_SetNodeGrayed_DO(TRUE,4,7);

		             		 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE,  TXT_ENG_MEDIA_DEMO_STOP);
		                     s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
		                     break;

						case ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO:
		                   //  AMR_Recode_Demo_Start();

		                     GUIMENU_SetNodeGrayed_DO(TRUE,0,3);
		                     GUIMENU_SetNodeGrayed_DO(TRUE,5,7);

						     GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_NULL, TXT_ENG_MEDIA_DEMO_STOP);
		                     s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
							 break;
							 
		                case ID_ENG_AUDIO_EXPRESS_DEMO: 
						//	 AUDIO_Express_Demo_Start();
						//	 AUDIO_Express_Demo_SetPar(3);//play right +left track
							 if(MP3_Express_Demo_Start())
							 {
		    					 GUIMENU_SetNodeGrayed_DO(TRUE,0,4);
		                         GUIMENU_SetNodeGrayed_DO(TRUE,6,7);
		    				     GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_AUDIO_STREAM_LEFT_TRACK, TXT_ENG_MEDIA_DEMO_STOP);
		                         s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
		                     }
		                     else
		                     {
		                          MMIPUB_OpenAlertFailWin(TXT_ENG_MEDIA_AUDIO_EXPRESSION_CARD);
		                     }
							 break;
						case ID_ENG_AUDIO_CAT_MVOICE_DEMO: 

						    if(check_sdcard())
		                    {
		    				//	 CatMv_Demo_Start();
		    					 
		    					 GUIMENU_SetNodeGrayed_DO(TRUE,0,5);
		                         GUIMENU_SetNodeGrayed_DO(TRUE,7,7);
		    					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_AUDIO_VCOIE, TXT_ENG_MEDIA_DEMO_STOP);
					             s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;

		                     }
		                     else
		                     {
		                        MMIPUB_OpenAlertFailWin(TXT_ENG_MEDIA_AUDIO_CAT_MVOICE_CARD);

		                     }
							 break;	 
						case ID_ENG_AUDIO_MIC_SAMPLE_DEMO: 
		                      
						//	 MIC_Sample_Demo_Start();
							 GUIMENU_SetNodeGrayed_DO(TRUE,0,6);
							 
		                	 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_NULL, TXT_ENG_MEDIA_DEMO_STOP);
		                     s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
							 break;	 		 
						 default:
		                   break;	 
		             	}
		}
		else if(s_audio_cur_state == AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS)
		{
             switch (menu_id)
             {
                case ID_ENG_AUDIO_AAC_STRING_DEMO:
                  //   AAC_Demo_Pause();
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RESUME, TXT_ENG_MEDIA_DEMO_STOP);
                     break;
					 
                case ID_ENG_AUDIO_AMR_STRING_PLAY_DEMO:                    
                  //   AMR_Demo_Pause();
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RESUME, TXT_ENG_MEDIA_DEMO_STOP);
					 break;
					 
                case ID_ENG_AUDIO_MP3_STRING_DEMO:                    
                 //    MP3_Demo_Pause();
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RESUME, TXT_ENG_MEDIA_DEMO_STOP);
                     break;
					 
			    case ID_ENG_AUDIO_WAV_STRING_DEMO:
                 //    WAV_Demo_Pause();
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RESUME, TXT_ENG_MEDIA_DEMO_STOP);
                     break;
					 
                case ID_ENG_AUDIO_EXPRESS_DEMO: 
			//		 AUDIO_Express_Demo_SetPar(1); //play left track
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, 
					 TXT_ENG_MEDIA_AUDIO_STREAM_RIGHT_TRACK, TXT_ENG_MEDIA_DEMO_STOP);
                     break;

                case ID_ENG_AUDIO_CAT_MVOICE_DEMO: 
			//		 CatMv_RegExpOnOff(0);
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_AUDIO_MVCOIE, TXT_ENG_MEDIA_DEMO_STOP);
                     break;    
                default:
                  break;   
             	}
             if(menu_id!=ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO)	
             {
			   s_audio_cur_state = AUDIO_STREAM_PAUSE_OR_STOP_RECORD_OR_LEFT_TRACK;
			 }
		}
		else if(s_audio_cur_state == AUDIO_STREAM_PAUSE_OR_STOP_RECORD_OR_LEFT_TRACK)
		{
             switch (menu_id)
             {
                case ID_ENG_AUDIO_AAC_STRING_DEMO:
                 //    AAC_Demo_Resume();
				     s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE, TXT_ENG_MEDIA_DEMO_STOP);
                     break;
					 
                case ID_ENG_AUDIO_AMR_STRING_PLAY_DEMO:                    
                    // AMR_Demo_Resume();
				     s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE, TXT_ENG_MEDIA_DEMO_STOP);
                     break;
					 
                case ID_ENG_AUDIO_MP3_STRING_DEMO:                    
                   //  MP3_Demo_Resume();
				     s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE, TXT_ENG_MEDIA_DEMO_STOP);
                     break;
					 
			    case ID_ENG_AUDIO_WAV_STRING_DEMO:
                   //  WAV_Demo_Resume();
				     s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PAUSE, TXT_ENG_MEDIA_DEMO_STOP);
                     break;

				case ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO:
                   //  AMR_Recode_Demo_PlayBack();//回放的接口函数
				     s_audio_cur_state = AUDIO_STREAM_RESUME_OR_PLAYBACK_OR_RIGHT_TRACK;
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_NULL, TXT_ENG_MEDIA_DEMO_STOP);
                     break;
					 
                case ID_ENG_AUDIO_EXPRESS_DEMO:   
				//	 AUDIO_Express_Demo_SetPar(2);//play right track
				     s_audio_cur_state = AUDIO_STREAM_RESUME_OR_PLAYBACK_OR_RIGHT_TRACK;
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_AUDIO_STREAM_LEFT_AND_RIGHT, TXT_ENG_MEDIA_DEMO_STOP);
                     break;
                case ID_ENG_AUDIO_CAT_MVOICE_DEMO: 
				//	 CatMv_RegExpOnOff(1);
					 s_audio_cur_state = AUDIO_STREAM_PLAY_OR_RECORD_OR_EXPRESS;
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_AUDIO_VCOIE, TXT_ENG_MEDIA_DEMO_STOP);
                     break;  
                     
                default:
                
                     break;
             	}
		}
		else if(s_audio_cur_state == AUDIO_STREAM_RESUME_OR_PLAYBACK_OR_RIGHT_TRACK)
		{
             switch (menu_id)
             {	 
                case ID_ENG_AUDIO_EXPRESS_DEMO:
				//	 AUDIO_Express_Demo_SetPar(3);
				     s_audio_cur_state = AUDIO_STREAM_LEFT_AND_RIGHT_TRACK;
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_AUDIO_STREAM_LEFT_TRACK, TXT_ENG_MEDIA_DEMO_STOP);
                     break;
                 default:
                
                     break;    
             }
		}
		else if(s_audio_cur_state == AUDIO_STREAM_LEFT_AND_RIGHT_TRACK)
		{
             switch (menu_id)
             {	 
                case ID_ENG_AUDIO_EXPRESS_DEMO:
				//	 AUDIO_Express_Demo_SetPar(1);
				     s_audio_cur_state = AUDIO_STREAM_PAUSE_OR_STOP_RECORD_OR_LEFT_TRACK;
					 GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_AUDIO_STREAM_RIGHT_TRACK, TXT_ENG_MEDIA_DEMO_STOP);
                     break;
                default:
                
                     break; 
             }
		}
		MMK_PostMsg(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
        break; 
        
	case MSG_MEDIA_DEMO_APP_UP:	
	
		//SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_APP_UP s_audio_cur_state = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7862_112_2_18_2_17_39_105,(uint8*)"d", s_audio_cur_state);

		if(s_audio_cur_state == AUDIO_STREAM_STOP)
	    {
			menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);
			
			//SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_APP_UP menu_id = %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7868_112_2_18_2_17_39_106,(uint8*)"d", menu_id);
	        switch (menu_id)
	        {
	            case ID_ENG_AUDIO_EXPRESS_DEMO:
					GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
					break;
				case ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO:
					GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RECORD, TXT_ENG_RETURN);
					break;
	            case ID_ENG_AUDIO_WAV_STRING_DEMO:
					GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
					break;
			    default:
                
                   break; 		
	        }
			MMK_PostMsg(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
		}
        break;
	
	case MSG_MEDIA_DEMO_APP_DOWN:

		//SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_APP_DOWN s_audio_cur_state = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7890_112_2_18_2_17_39_107,(uint8*)"d", s_audio_cur_state);

		if(s_audio_cur_state == AUDIO_STREAM_STOP)
	    {
			menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);
			
			//SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_APP_DOWN menu_id = %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7896_112_2_18_2_17_39_108,(uint8*)"d", menu_id);
	        switch (menu_id)
	        {
	            case ID_ENG_AUDIO_AMR_STRING_RECORD_DEMO:
					GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_RECORD, TXT_ENG_RETURN);
					break;
				case ID_ENG_AUDIO_EXPRESS_DEMO:
					GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
					break;
	            case ID_ENG_AUDIO_AAC_STRING_DEMO:
					GUIMENU_SetDynamicMenuSoftkey(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID, TXT_NULL, TXT_ENG_MEDIA_DEMO_PLAY, TXT_ENG_RETURN);
					break;
				default:
                
                   break; 	
	        }
			MMK_PostMsg(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
		}
        break;
        
    case MSG_MEDIA_DEMO_SHOW_DATE:
        if(s_audio_cur_state != AUDIO_STREAM_STOP)
	    {
			menu_id = GUIMENU_GetCurNodeId(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID);
			
			//SCI_TRACE_LOW:"ENGAudioDemoWinHandleMsg MSG_MEDIA_DEMO_SHOW_DATE MSG_APP_DOWN menu_id = %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_7921_112_2_18_2_17_39_109,(uint8*)"d", menu_id);
	        switch (menu_id)
	        {
	            case ID_ENG_AUDIO_MIC_SAMPLE_DEMO:
#if 1
                    sprintf((char*)title_str, "MIC dB: %d",  (*(int32*)param));

                    MMIAPICOM_StrToWstr( (const uint8*)title_str, title_wstr );
                    str_info.wstr_len = MMIAPICOM_Wstrlen(title_wstr);
                    str_info.wstr_ptr = title_wstr;
                    GUIWIN_SetTitleText(win_id,str_info.wstr_ptr,str_info.wstr_len,TRUE);
#endif
            
					break;
			    default:
        
                    break;		
	        }
		//	MMK_PostMsg(MMIENG_MEDIA_AUDIO_DEMO_CTRL_ID,MSG_CTL_PAINT,0,0);
		}
    break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

#endif

#endif

#define MMI_PHONE_INFO_BUFFER_LEN       2048

/********************************************************************************
NAME:          ENGPhoneInfoWinHandleMsg
DESCRIPTION:   Phone Info display window
PARAM IN:
PARAM OUT:
AUTHOR:        chunjuanliang.rda
DATE:          2017.09.19
********************************************************************************/
LOCAL  MMI_RESULT_E ENGUITestPhoneInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIENG_UITEST_TEXT_CTRL_ID);
        {
            uint32  strLength = 0;
            uint8*  pStrBuffer = PNULL;
            wchar*  pWStrBuffer = PNULL;

            pStrBuffer = (char*)SCI_ALLOC_APPZ( MMI_PHONE_INFO_BUFFER_LEN+1 );
            if( PNULL == pStrBuffer )
            {
                SCI_TRACE_LOW("ENGUITestPhoneInfoWinHandleMsg, malloc pStrBuffer fail");
                break;
            }
            SCI_MEMSET(pStrBuffer, '\0', MMI_PHONE_INFO_BUFFER_LEN + 1);//cid603543

            pWStrBuffer = (wchar*)SCI_ALLOC_APPZ( sizeof(wchar)*(MMI_PHONE_INFO_BUFFER_LEN+1) );
            if( PNULL == pWStrBuffer )
            {
                if( PNULL != pStrBuffer )
                {
                    SCI_FREE( pStrBuffer );
                    pStrBuffer = PNULL;
                }
                SCI_TRACE_LOW("ENGUITestPhoneInfoWinHandleMsg, malloc pWStrBuffer fail");
                break;
            }

            strLength = GetPhoneInfo( pStrBuffer, MMI_PHONE_INFO_BUFFER_LEN ) ;
            MMI_STRNTOWSTR( pWStrBuffer, MMI_PHONE_INFO_BUFFER_LEN, (uint8*)pStrBuffer, MMI_PHONE_INFO_BUFFER_LEN, strLength ) ;
            GUITEXT_SetString( MMIENG_UITEST_TEXT_CTRL_ID, pWStrBuffer, (uint16)strLength, FALSE );

            if( PNULL != pStrBuffer )
            {
                SCI_FREE( pStrBuffer );
                pStrBuffer = PNULL;
            }
            if( PNULL != pWStrBuffer )
            {
                SCI_FREE( pWStrBuffer );
                pWStrBuffer = PNULL;
            }
        }
        break;
  
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:          
#ifdef TOUCH_PANEL_SUPPORT 
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT 
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB: 
        MMK_CloseWin(win_id);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}       

/********************************************************************************
NAME:          ENGUITestResultWinHandleMsg
DESCRIPTION:   uitest result display window
PARAM IN:      
PARAM OUT:     
AUTHOR:        ying.xu
DATE:          2010.08.10
********************************************************************************/
LOCAL  MMI_RESULT_E ENGUITestResultWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIENG_UITEST_TEXT_CTRL_ID);
        GetItemTestResult();
        DisplayItemTestResult(MMIENG_UITEST_TEXT_CTRL_ID);

        break;
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB: 
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}       

/********************************************************************************
NAME:          GetPhaseCheckInfo
DESCRIPTION:   get phasecheckinfo
PARAM IN:
PARAM OUT:
AUTHOR:        chunjuanliang.rda
DATE:          2017.09.19
*******************************************************************************/
LOCAL uint32 GetPhaseCheckInfo(char* buffer,int32  buffer_len)
{
    int32 i = 0;
    int32 len1 = 0;
    uint32 offset = 0;
    char num[8] = {0};    
    uint16 item_num = 0;
    int32 station_num = 0;

    char *isTested[2] = {"Tested!", "Not Tested!"};
    char *isPassed[2] ={"Passed!", "Failed!"} ;

    uint8 temp_buffer[200] = {0};
    int32 temp_buffer_len = 200 ;

    int32 test_length[2] ={0};	
    int32 pass_length[2] = {0};
#if 0
#ifndef WIN32
    SP09_PHASE_CHECK_T  s_mmieng_product = {0};    
#else
    SP09_PHASE_CHECK_T  s_mmieng_product  = {SP09_SPPH_MAGIC_NUMBER,
        "12345678901234567890123",
        "12345678901234567890123",
        15,
    {"123456789","234567891","345678912","456789123","567891234",
    "678912345","789123456","891234567","912345678","123456789",
    "234567891","345678912","456789123","567891234","678912345"},
    {0}, 0x01, {0},	0xf00f,0x0ff0 };
#endif
#endif
    test_length[0] = SCI_STRLEN(isTested[0]) ;
    test_length[1] = SCI_STRLEN(isTested[1]) ;
    pass_length[0] = SCI_STRLEN(isPassed[0]) ;
    pass_length[1] = SCI_STRLEN(isPassed[1]) ;

    if(PNULL == buffer)
    {
        SCI_TRACE_LOW("mmieng_uitestwin.c GetPhaseCheckInfo buffer == null");
        return -1;
    }

    SCI_TRACE_LOW("mmieng_uitestwin.c GetPhaseCheckInfo buffer_len is %d",buffer_len);
    if(2 > buffer_len)
    {
        return -2;
    }

    //read data from flash
#if 0 //ndef WIN32  //get data from s_mmieng_product 
    if(0 == EFS_ReadPhaseCheck(temp_buffer,temp_buffer_len))
    {
        return 0;
    } 
    SCI_MEMCPY(&s_mmieng_product, temp_buffer, SP09_MAX_PHASE_BUFF_SIZE);   
#endif

#if defined(__BIG_ENDIAN) || defined(__BigEndian)	
    if(SP09_SPPH_MAGIC_NUMBER != CHANGE_ENDIAN_SEQUENCE_32(s_mmieng_product.Magic))
#else   
    if(SP09_SPPH_MAGIC_NUMBER != s_mmieng_product.Magic) 
#endif    
    {
        SCI_TRACE_LOW("mmieng_uitestwin.c GetPhaseCheckInfo magic num = %x", s_mmieng_product.Magic); 

        len1 = sprintf((char*)temp_buffer,"error! magic num = %x\n",s_mmieng_product.Magic);
        offset += len1;
        SCI_MEMCPY(buffer, temp_buffer, len1);
    }

        //station test result//
        SCI_MEMCPY(buffer + offset, "<Station Result>",16);
        offset += 16;

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
        station_num = CHANGE_ENDIAN_SEQUENCE_32(s_mmieng_product.StationNum);
#else
        station_num = s_mmieng_product.StationNum;
#endif       
        for (i = 0; i <  station_num; i++)
        {
            buffer[offset] = CR_CHAR;
            offset += 1;

            //num	        
            SCI_MEMSET(num, 0 ,8);
            sprintf(num,"<%d> ",i+1);
            len1  = SCI_STRLEN(num);
            if((buffer_len - offset) < len1)
            {
                return offset;
            }
            SCI_MEMCPY(buffer + offset, num, len1);	
            offset += len1;	

            //name   
            len1 = MIN(SP09_MAX_STATION_NAME_LEN, SCI_STRLEN(s_mmieng_product.StationName[i]));/*lint !e666*/
            if((buffer_len - offset) < len1)
            {
                return offset;
            }
            SCI_MEMCPY(buffer + offset, s_mmieng_product.StationName[i],len1);
            offset += len1;

            if((buffer_len - offset) < 2)
            {
                return offset;
            }
            SCI_MEMCPY(buffer + offset, ": ", 2);
            offset += 2;

            // if tested
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
            item_num = (CHANGE_ENDIAN_SEQUENCE_16(s_mmieng_product.iTestSign) >> i) & (uint16)0x0001;
#else    
            item_num = (s_mmieng_product.iTestSign >> i) & (uint16)0x0001; 
#endif   
              if(s_mmieng_product.SignFlag == 0x00)
            {
                item_num = (~item_num)&(uint16)0x0001;
            }

            if((buffer_len - offset) < test_length[item_num])
            {
                return offset;
            }
            SCI_MEMCPY(buffer + offset,isTested[item_num],test_length[item_num]);

            offset += test_length[item_num];

            //if passed
            if(item_num == 0)
            {
                SCI_MEMCPY(buffer + offset, ": ",2);
                offset += 2;
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
                item_num = (CHANGE_ENDIAN_SEQUENCE_16(s_mmieng_product.iItem) >> i) & (uint16)0x0001;
#else     
                item_num = (s_mmieng_product.iItem >> i)&(uint16)0x0001;
#endif               
                if(s_mmieng_product.SignFlag == 0x00)
                {
                    item_num = (~item_num)&(uint16)0x0001;
                }

                if((buffer_len - offset) < pass_length[item_num])
                {
                    return offset;
                }
                SCI_MEMCPY(buffer + offset,isPassed[item_num],pass_length[item_num]);
                offset += pass_length[item_num];
            }

        }
        SCI_TRACE_LOW("mmieng_uitestwin.c GetPhaseCheckInfo offset = %d",offset);
        return offset;
}

/********************************************************************************
NAME:          GetPhoneInfo
DESCRIPTION:   get string of Phoneinfo
PARAM IN:
PARAM OUT:
AUTHOR:        chunjuanliang.rda
DATE:          2017.09.19
********************************************************************************/
LOCAL int32 GetPhoneInfo(char* buffer, int32 buffer_length)
{

    MN_IMEISVN_T imei = {0};
    char tmp_buf1[20] = {0};
    char tmp_buf2[20] = {0};
    uint32 offset  = 0;
    uint16 len = 0;
    uint8 i  = 0;
    uint8 j = 0;
    //PRODUCTION_TEST_INFO_T s_mmieng_product = {0};
    BT_ADDRESS    device_addr = {0};
    uint8   addr_str[20]      = {0};

#ifdef MMI_ENG_WIFI_SUPPORT
    uint16  temLen = 0;
    char    wifi_addr[6]      = {0};  //ETH_ALEN
#endif

    if(PNULL == buffer)
    {
        SCI_TRACE_LOW("mmieng_uitestwin.c GetPhoneInfo buffer == null");
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_WIN_4039_112_2_18_2_17_49_131,(uint8*)"");
        return -1;
    }

    SCI_TRACE_LOW("mmieng_uitestwin.c GetPhoneInfo buffer_length is %d",buffer_length);
    if(2 > buffer_length)
    {
        return -2;
    }
#if 0
    //S/N //
    SCI_MEMCPY(buffer + offset,  "<S/N>", 5);
#ifndef _WIN32
    SCI_GetProductionTestInfo(&s_mmieng_product);
#endif
    offset += 5;
    buffer[offset] = CR_CHAR;
    offset += 1;
    len = SCI_STRLEN(s_mmieng_product.serial_num);
    SCI_MEMCPY(buffer + offset,s_mmieng_product.serial_num,len);
    offset += len;
    buffer[offset] = CR_CHAR;
    offset += 1;
#endif
    {
            SCI_TRACE_LOW("mmieng_uitestwin.c GetPhoneInfo SP09_MAX_PHASE_BUFF_SIZE is %d",SP09_MAX_PHASE_BUFF_SIZE);

            if(0 == EFS_ReadPhaseCheck(&s_mmieng_product,SP09_MAX_PHASE_BUFF_SIZE))
            {
                return 0;
            }

            SCI_MEMCPY(buffer + offset,  "<S/N1>", 6);
            offset += 6;
            buffer[offset] = CR_CHAR;
            offset += 1;

            len = SCI_STRLEN(s_mmieng_product.SN1);
            SCI_MEMCPY(buffer + offset,s_mmieng_product.SN1,len);
            offset += len;
            buffer[offset] = CR_CHAR;
            offset += 1;

            SCI_MEMCPY(buffer + offset,  "<S/N2>", 6);
            offset += 6;
            buffer[offset] = CR_CHAR;
            offset += 1;

            len = SCI_STRLEN(s_mmieng_product.SN2);
            SCI_MEMCPY(buffer + offset,s_mmieng_product.SN2,len);
            offset += len;
            buffer[offset] = CR_CHAR;
            offset += 1;
    }

    // IMEI //
    SCI_MEMCPY(buffer + offset, "<IMEI>",6);
    offset += 6;
    buffer[offset] = CR_CHAR;
    offset += 1;
    for(j=0;j < MMI_DUAL_SYS_MAX; j++)
    {
         if(!MNNV_GetIMEISVNEx(MN_DUAL_SYS_1 + j, &imei))//cid602184
         {
            SCI_MEMSET(&imei, 0, sizeof(MN_IMEISVN_T));
         }

        SCI_MEMSET(tmp_buf1, 0, 20);
        SCI_MEMSET(tmp_buf2, 0, 20);
        for (i = 0; i < 8; i++) 
        {
            tmp_buf1[2 * i] = (imei.imei[i]) & 0x0F;
            tmp_buf1[2 * i + 1] = imei.imei[i] >> 4;
        }
        sprintf( tmp_buf2,
                "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
                tmp_buf1[1], tmp_buf1[2], tmp_buf1[3],
                tmp_buf1[4], tmp_buf1[5], tmp_buf1[6], tmp_buf1[7],
                tmp_buf1[8], tmp_buf1[9], tmp_buf1[10], tmp_buf1[11],
                tmp_buf1[12], tmp_buf1[13], tmp_buf1[14], tmp_buf1[15]
            );
        SCI_MEMCPY(buffer + offset, tmp_buf2,15);
        offset += 15;
        buffer[offset] = CR_CHAR;
        offset += 1;
    }

    // BT address //
    SCI_MEMCPY(buffer + offset,  "<BT ADDRESS>", 12);
    offset += 12;
    buffer[offset] = CR_CHAR;
    offset += 1;
#ifdef BLUETOOTH_SUPPORT
    MMIAPIBT_GetAddress(&device_addr);
    sprintf((char *)addr_str, "%02x:%02x:%02x:%02x:%02x:%02x",device_addr.addr[5],device_addr.addr[4],device_addr.addr[3],
    device_addr.addr[2],device_addr.addr[1],device_addr.addr[0]);
    SCI_MEMCPY(buffer + offset, addr_str,17);
    offset += 17;
    buffer[offset] = CR_CHAR;
    offset += 1;
#endif

#ifdef MMI_WIFI_SUPPORT
    temLen = strlen("<WIFI ADDRESS>");
    SCI_TRACE_LOW("GetPhoneInfo temLen %d", temLen);
    SCI_MEMCPY( buffer+offset, "<WIFI ADDRESS>", temLen );
    offset += temLen;
    buffer[offset] = CR_CHAR;
    offset += 1;
#ifndef WIN32
    wifi_get_mac( wifi_addr );
#endif
    SCI_MEMSET( addr_str, 0, 20 );
    sprintf( (char*)addr_str, "%02x:%02x:%02x:%02x:%02x:%02x", wifi_addr[0], wifi_addr[1], wifi_addr[2], wifi_addr[3], wifi_addr[4], wifi_addr[5]);
    temLen = strlen(addr_str);
    SCI_MEMCPY( buffer+offset, addr_str, temLen );
    SCI_TRACE_LOW("WIFI mac %02x %02x %02x %02x %02x %02x", wifi_addr[0], wifi_addr[1], wifi_addr[2], wifi_addr[3], wifi_addr[4], wifi_addr[5]);
    offset += temLen;
    buffer[offset] = CR_CHAR;
    offset += 1;
#endif


    SCI_TRACE_LOW("GetPhoneInfo offset %d", offset);
    //phase check//
    len = GetPhaseCheckInfo(buffer + offset,buffer_length-offset);
    offset +=len;
    SCI_TRACE_LOW("GetPhoneInfo len= %d,offset=%d", len,offset);

    return (offset);
}


/********************************************************************************
NAME:          GetItemTestResult
DESCRIPTION:   get result of uitest
PARAM IN:
PARAM OUT:
AUTHOR:        chunjuan.liang
DATE:          2020.05.22
********************************************************************************/
LOCAL void GetItemTestResult(void)
{
    int    item_str_len = 0;

    uint32 i=0 ;
    uint32 j=0 ;

    uint32 is_tested   = 0;
    uint32 test_result = 0;
    uint32 item_result = 0;

    ReadUITestResultNV(&is_tested, &test_result);//get result from nv

    for(i = 0,j =0; i < ARR_SIZE(s_all_test_win_id); i++)
    {
        if(0 == s_all_test_win_id[i])
        {
            continue;
        }

        //serial num
        g_test_result[j].idx = j + 1;

        //item 
        g_test_result[j].test_win_name_ptr= s_all_test_name[i];

        //if tested
        item_result = (is_tested >>i) & 0x00000001 ;

        if(1 == item_result)
        {
            //result
            item_result = (test_result >>i) & 0x00000001 ;
            if(0 == item_result)
            {
                g_test_result[j].test_state = 2; //fail
                SCI_TRACE_LOW("test result is %d,%d",j,g_test_result[j].test_state);
            }else if(1 == item_result)
            {
                g_test_result[j].test_state = 1; //pass
                SCI_TRACE_LOW("test result is %d,%d",j,g_test_result[j].test_state);
            }
        }else
        {
            g_test_result[j].test_state = 0;//not tested
            SCI_TRACE_LOW("test result is %d,%d",j,g_test_result[j].test_state);
        }
        j++;
     }
    g_item_test_value = j;
}
/********************************************************************************
NAME:          DisplayItemTestResult
DESCRIPTION:   display result of uitest 
PARAM IN:
PARAM OUT:
AUTHOR:        chunjuan.liang
DATE:          2020.05.22
********************************************************************************/
LOCAL void DisplayItemTestResult(MMI_CTRL_ID_T ctrl_id)
{
    int i =0;
    char index_str[16] = {0};
    uint16 index_str_len = 0;
    wchar wstr_item_str[50] = {0};
    uint16 wstr_item_str_len =0;

    GUIRICHTEXT_ITEM_T  item_data = {0};
    GUI_FONT_T test_result_color = MMI_BLACK_COLOR;
    uint16 index = 0;

    GUIRICHTEXT_SetBgColor(ctrl_id,MMI_WHITE_COLOR); //set bg color

    index_str_len = SCI_STRLEN("<Test Result>\n");
   //append <Test Resut>
    SCI_MEMCPY(index_str,  "<Test Result>\n",  index_str_len);
    MMI_STRNTOWSTR(wstr_item_str,50,index_str,50,index_str_len);
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.text_set_font_color = TRUE;
    item_data.setting_font_color= test_result_color;
    item_data.text_set_font = TRUE;
    item_data.setting_font = SONG_FONT_22;
    item_data.text_data.buf.str_ptr = wstr_item_str;
    item_data.text_data.buf.len = MMIAPICOM_Wstrlen(wstr_item_str);
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    for(i = 0 ;i < g_item_test_value ;i++)
    {
        SCI_MEMSET(index_str,0,16);
        SCI_MEMSET(wstr_item_str,0x00,sizeof(wchar)*50);
        wstr_item_str_len = 0;
        sprintf(index_str,"<%d> ", g_test_result[i].idx);
        MMIAPICOM_StrcatFromStrToUCS2(wstr_item_str,&wstr_item_str_len,index_str,strlen(index_str));
        MMIAPICOM_StrcatFromStrToUCS2(wstr_item_str,&wstr_item_str_len,g_test_result[i].test_win_name_ptr,strlen(g_test_result[i].test_win_name_ptr));
        if(0 == g_test_result[i].test_state)
        {
            MMIAPICOM_StrcatFromStrToUCS2(wstr_item_str,&wstr_item_str_len,WATCH_TEST_RESULT_NOTEST,strlen(WATCH_TEST_RESULT_NOTEST));
            test_result_color = MMI_BLACK_COLOR;
        }else if(2 == g_test_result[i].test_state)
        {
            MMIAPICOM_StrcatFromStrToUCS2(wstr_item_str,&wstr_item_str_len,WATCH_TEST_RESULT_FAIL,strlen(WATCH_TEST_RESULT_FAIL));
            test_result_color = MMI_DARK_RED_COLOR;
        }else if(1 == g_test_result[i].test_state)
        {
            MMIAPICOM_StrcatFromStrToUCS2(wstr_item_str,&wstr_item_str_len,WATCH_TEST_RESULT_PASS,strlen(WATCH_TEST_RESULT_PASS));
            test_result_color = MMI_GREEN_COLOR;
        }

        SCI_MEMSET(&item_data,0,sizeof(GUIRICHTEXT_ITEM_T));
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        item_data.text_set_font_color = TRUE;
        item_data.setting_font_color= test_result_color;
        item_data.text_set_font = TRUE;
        item_data.setting_font = SONG_FONT_16;
        item_data.text_data.buf.str_ptr = wstr_item_str;
        item_data.text_data.buf.len = MMIAPICOM_Wstrlen(wstr_item_str);
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    }

}

#if 0
/********************************************************************************
NAME:          GetUITestResultInfo
DESCRIPTION:   get string of uitest 
PARAM IN:      
PARAM OUT:     
AUTHOR:        ying.xu
DATE:          2010.08.10
********************************************************************************/
LOCAL int32 GetUITestResultInfo(char* buffer, int32 buffer_length)
{
    int32 offset = 0;
    uint32 is_tested = 0 ;
    uint32 test_result = 0;     //full test result
    uint32 item_result = 0;     //item test result
    int32 str_length = 0 ;
    char temp_str[16] = {0};
    uint32 i=0 ;
    uint32 j=0 ;
    
    const char *if_tested[] = {"not tested", "tested"};
    const char *if_passed[] = {"failed", "passed"};
    
    int32 tested_length[]  = {0,0};
    int32 passed_length[]  = {0,0};
    
    tested_length[0] = SCI_STRLEN(if_tested[0]);
    tested_length[1] = SCI_STRLEN(if_tested[1]);
    passed_length[0] = SCI_STRLEN(if_passed[0]);
    passed_length[1] = SCI_STRLEN(if_passed[1]);    
    
    ReadUITestResultNV(&is_tested, &test_result);  
    
    str_length = SCI_STRLEN("<Test Result>\n");
    if ( offset + str_length >= buffer_length)
    {
        return offset ;
    }    
    SCI_MEMCPY(buffer + offset,  "<Test Result>\n",  str_length);
    offset += str_length;  
    
    
    for(i = 0,j =1; i < ARR_SIZE(s_all_test_win_id); i++)
    {
        if(0 == s_all_test_win_id[i])
        {
            continue;
        }   
        
        //serial num               
        sprintf(temp_str,"<%d> ", j);
        str_length = SCI_STRLEN(temp_str) ;
        if ( offset + str_length >= buffer_length )
        {
            return offset ;
        }
        SCI_MEMCPY(buffer + offset, temp_str, str_length);      
        offset += str_length;   
        
        //item 
        str_length =  SCI_STRLEN(s_all_test_name[i]) ; 
        if ( offset + str_length >= buffer_length )
        {
            return offset ;
        }      
        SCI_MEMCPY(buffer + offset, s_all_test_name[i], str_length) ;
        offset += str_length;
        
        //space key
        if ( offset + 1 >= buffer_length)
        {
            return offset ;
        }   
        buffer[offset] = BL_CHAR;
        offset += 1;
        
        //if tested
        item_result = (is_tested >>i) & 0x00000001 ;            
        str_length =  tested_length[item_result] ;
        if ( offset + str_length >= buffer_length ) 
        {
            return offset ;
        }
        SCI_MEMCPY(buffer + offset, if_tested[item_result], str_length) ;
        offset += str_length;    
        
        if(1 == item_result)
        {     //space key
            if ( offset + 1 >= buffer_length)
            {
                return offset ;
            }   
            buffer[offset] = BL_CHAR;
            offset += 1;
            //result
            item_result = (test_result >>i) & 0x00000001 ;            
            str_length =   passed_length[item_result] ;
            if ( offset + str_length >= buffer_length ) 
            {
                return offset ;
            }
            SCI_MEMCPY(buffer + offset, if_passed[item_result], str_length) ;
            offset += str_length;              
        } 
        //enter key
        if ( offset + 1 >= buffer_length)
        {
            return offset ;
        }   
        buffer[offset] = CR_CHAR;
        offset += 1;
        
        j++;
    }      
    
    return offset ;
}
#endif
/********************************************************************************
NAME:          ReadUITestResultNV
DESCRIPTION:   
PARAM IN:      
PARAM OUT:     
AUTHOR:        ying.xu
DATE:          2010.08.10
********************************************************************************/
LOCAL int32 ReadUITestResultNV( uint32* is_tested, uint32* result  )
{
    uint8 nvitem[64] = {0}; 
    uint32 nv_length = 64;
    uint32 return_code = 0 ;
#ifndef WIN32    
    return_code = PROD_ReadEngModeTestResult((void*)nvitem, nv_length);
#endif    
    //SCI_TRACE_LOW:"MMIENG_UITESTWIN ReadUITestResultNV  return_code = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8134_112_2_18_2_17_40_110,(uint8*)"d", return_code);
    if(return_code == 0)   
    {        
        *is_tested = *(uint32*)&nvitem[0] ;
        *result = *(uint32*)(&nvitem[0] + 8) ;        
        //SCI_TRACE_LOW:"MMIENG_UITESTWIN ReadUITestResultNV : is_tested =  %x,  result = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8139_112_2_18_2_17_40_111,(uint8*)"dd", *is_tested, *result);
    }
    else
    {
        SCI_MEMSET(nvitem, 0, nv_length);
#ifndef WIN32    
        return_code = PROD_WriteEngModeTestResult((void*)nvitem, nv_length);
#endif    
    }    
    return return_code;
}

/********************************************************************************
NAME:          WriteUITestResultNV
DESCRIPTION:   
PARAM IN:      
PARAM OUT:     
AUTHOR:        ying.xu
DATE:          2010.08.10
********************************************************************************/
LOCAL int32 WriteUITestResultNV(uint32*  is_tested ,  uint32* result )
{       
    uint8 nvitem[64] ={ 0}; 
    uint32 nv_length = 64;
    uint32 return_code = 0 ;    
    uint32 temp_is_tested = 0;
    uint32 temp_result = 0;
    
#ifndef WIN32    
    return_code = ReadUITestResultNV(&temp_is_tested, &temp_result);   
#endif    
    if((temp_is_tested != *is_tested) || (temp_result != *result))
    {        
        *(uint32*)&nvitem[0]  = *is_tested ;
        *(uint32*)(&nvitem[0] + 8) = *result ;
#ifndef WIN32    
        return_code = PROD_WriteEngModeTestResult((void*)nvitem, nv_length);  
#endif
        //SCI_TRACE_LOW:"MMIENG_UITESTWIN WriteUITestResultNV return_code =  %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8176_112_2_18_2_17_40_112,(uint8*)"d", return_code);
        if(SCI_SUCCESS !=  return_code )
        {
            SCI_MEMSET(nvitem, 0, nv_length);
#ifndef WIN32    
            return_code = PROD_WriteEngModeTestResult((void*)nvitem, nv_length);
#endif 
        }
        else
        {
            //SCI_TRACE_LOW:"MMIENG_UITESTWIN WriteUITestResultNV success: is_tested = %x, result = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8186_112_2_18_2_17_40_113,(uint8*)"dd", *is_tested, *result);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIENG_UITESTWIN WriteUITestResultNV do nothing: is_tested = %x, result = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8191_112_2_18_2_17_40_114,(uint8*)"dd", *is_tested, *result);
    }
    return return_code ; 
}

/********************************************************************************
NAME:          SetTestResult
DESCRIPTION:   utlized when full test
PARAM IN:      
PARAM OUT:     
AUTHOR:        ying.xu
DATE:          2010.08.10
********************************************************************************/
LOCAL int32 SetTestResult(uint32 win_id, uint32 result)
{
    uint32 i =0 ;
    
    for( i=0; i < (uint32)( ARR_SIZE(s_all_test_win_id)); i++ )
    {
        if(*(s_all_test_win_id + i) == win_id)
        {
            s_is_tested |=  (0x00000001 << i);  //if tested
            
            result &=  0x000001 ;           
            s_test_result &=  ~(0x00000001 << i);   /*lint !e502*/ //clear the bit 
            s_test_result |= (result << i);         //set the bit
            break;
        }
        
    }   
    //SCI_TRACE_LOW:" SetTestResult : i = %d,  result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8220_112_2_18_2_17_40_115,(uint8*)"dd", i, result);
    //SCI_TRACE_LOW:" SetTestResult :s_is_tested = %x,  s_test_result = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8221_112_2_18_2_17_40_116,(uint8*)"dd", s_is_tested, s_test_result);
    
    return i;
}

/********************************************************************************
NAME:          SaveTestResult
DESCRIPTION:   utlized when test one item
PARAM IN:      
PARAM OUT:     
AUTHOR:        ying.xu
DATE:          2010.08.10
********************************************************************************/
LOCAL int32 SaveTestResult(uint32 win_id, uint32 result)
{
    int32 return_error = 0;
    //SCI_TRACE_LOW:"MMIENG_UITESTWIN SaveTestResult : fulltest = %d, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8236_112_2_18_2_17_40_117,(uint8*)"dd", s_is_fulltest, result);
    if( 0 != s_is_fulltest )    //full test
    {
        return SetTestResult(win_id, result);       
    }
    else
    {
        //get test result
        return_error = ReadUITestResultNV(&s_is_tested, &s_test_result);
   
        //modify the cuurent test result
        SetTestResult(win_id, result) ;        
        
        //save to nv
        WriteUITestResultNV(&s_is_tested, &s_test_result);
        
        //set adc7_reserved MMI SET BIT
        SetMMITestResultBit(&s_is_tested, &s_test_result);
        return return_error;
    }
}
/********************************************************************************
NAME:          MMIAPIENG_SaveTestResult
DESCRIPTION:   api of settestresult
AUTHOR:        ying.xu
DATE:          2010.8.10
********************************************************************************/
PUBLIC uint32 MMIAPIENG_SaveTestResult(uint32 win_id, uint32 result)
{
    return SaveTestResult( win_id, result);
}

/********************************************************************************
NAME:          MMIAPIENG_SaveTestResult
DESCRIPTION:   api of settestresult
AUTHOR:        ying.xu
DATE:          2010.8.10
********************************************************************************/
LOCAL int32 SetMMITestResultBit(uint32* is_tested, uint32* result)
{
    uint32 i= 0;
    uint32 j= 0;
    
    for(i = 0; i < ARR_SIZE(s_mmieng_test_win_id) ; i++)
    {
        for( j=0; j < ARR_SIZE(s_all_test_win_id); j++)
        {
            if( *(s_mmieng_test_win_id + i) == *(s_all_test_win_id + j))
                //check
            {
                if( ((*is_tested >> j) & 0x00000001 ) == 1)
                {
                    if( ((*result >> j) & 0x00000001 ) == 1)
                    {
                        continue;
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"SetMMITestResultBit :  j = %d, result = 0"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8292_112_2_18_2_17_40_118,(uint8*)"d", j);
                        return SetADC7MMITestBit(0);        
                        
                    }
                }
                else
                {
//SCI_TRACE_LOW:"SetMMITestResultBit :  j = %d, result = 0"
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8299_112_2_18_2_17_40_119,(uint8*)"d", j);
                return SetADC7MMITestBit(0);
                }
            }
        }
    }
    //SCI_TRACE_LOW:"SetMMITestResultBit :  j = %d, result = 1"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8305_112_2_18_2_17_40_120,(uint8*)"d", j);
    return SetADC7MMITestBit(1) ;
}

/********************************************************************************
NAME:          SetADC7MMITestBit
DESCRIPTION:   
PARAM IN:      
PARAM OUT:     None
AUTHOR:        ying.xu
DATE:          2010.08.17
********************************************************************************/
LOCAL uint32  SetADC7MMITestBit(uint32 result)
{
    
    uint32 adc_reserved_info = 0;
    uint32 return_code = 0 ;
    //从NV中读取adc_reserved[7]的信息
#ifdef WIN32    
    adc_reserved_info = 0xc3fe0155;/*lint !e64*/
#else
    adc_reserved_info = REFPARAM_GetCaliMarkerValue();
    //SCI_TRACE_LOW:"REFPARAM_GetCaliMarkerValue %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8326_112_2_18_2_17_40_121,(uint8*)"d",adc_reserved_info);
#endif

    if ((result & 0x01) == ((adc_reserved_info >> 27) & 0x01))
    {
        return 1;
    }
    
    adc_reserved_info &= ~ (0x00000001 << 27) ;
    adc_reserved_info |=  (result&0x01) << 27;
    return_code =  REFPARAM_SetCaliMarkerValue(adc_reserved_info) ; 
    //SCI_TRACE_LOW:"MMIENG_UITESTWIN SetADC7MMITestBit : result = %d,  return_code = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8332_112_2_18_2_17_40_122,(uint8*)"dd", result, return_code);
    return return_code;
}   


/********************************************************************************
NAME:          ENGMotionSensorTestWinHandleMsg
DESCRIPTION:   
PARAM IN:      
PARAM OUT:     
AUTHOR:        ying.xu
DATE:          2010.12.05
********************************************************************************/
#ifdef MOTION_SENSOR_TYPE
typedef enum
{
    UP_DIRECTION,
    DOWN_DIRECTION,
    LEFT_DIRECTION,
    RIGHT_DIRECTION,
    MAX_DIRECTION
}MOTION_SENSOR_TEST_DIRECTION_E;
#define MOTION_SENSOR_DISP_MAX_LEN   40
static BOOLEAN pass_flag[MAX_DIRECTION] = {0};

LOCAL void DisplayTestDirectionResult(MSENSOR_MOTION_VECTOR_T coordinate)
{
    char *str_direction[MAX_DIRECTION] = {"UP-direction:", "DOWN-direction:", "LEFT-direction:", "RIGHT-direction:"};
    char str_pass[10] = {"Pass"};
    wchar wstr_pass[10] = {0};
    int32 i = 0;
    MMI_STRING_T text = {0};
    wchar wstr[MOTION_SENSOR_DISP_MAX_LEN+1] = {0};
    MSENSOR_MOTION_VECTOR_T temp_coordinae ={0};

    SCI_TRACE_LOW("[UITest]enter DisplayTestDirectionResult, x:%d , y:%d ",coordinate.x,coordinate.y);

    SCI_TRACE_LOW("[UITest]enter DisplayTestDirectionResult, abs(x) %d , abs(y) %d ",abs(coordinate.x),abs(coordinate.y) );	
	
    if(abs(coordinate.x)<2)
    {
        temp_coordinae.x= 0;
    }
    else
    {
        temp_coordinae.x = coordinate.x;
    }
    if(abs(coordinate.y)<2)
    {
        temp_coordinae.y= 0;
    }
    else
    {
        temp_coordinae.y = coordinate.y;
    }
    if(abs(temp_coordinae.x) <= abs(temp_coordinae.y))
    {
        if(temp_coordinae.y < 0)
        {
            //UP direction
            SCI_TRACE_LOW("[UITest]enter DisplayTestDirectionResult, UP direction");
            pass_flag[UP_DIRECTION]=TRUE;
        }
        else if(temp_coordinae.y > 0)
        {
            //DOWN direction
            SCI_TRACE_LOW("[UITest]enter DisplayTestDirectionResult, DOWN direction");
            pass_flag[DOWN_DIRECTION]=TRUE;
        }
        else if(temp_coordinae.y == 0)
        {
            SCI_TRACE_LOW("[UITest]enter DisplayTestDirectionResult, coordinate.y is zero, do nothing");
        }
    }
    else
    {
        if(temp_coordinae.x > 0)
        {
            //LEFT direction
            SCI_TRACE_LOW("[UITest]enter DisplayTestDirectionResult, Left direction");
            pass_flag[LEFT_DIRECTION]=TRUE;
        }
        else if(temp_coordinae.x < 0)
        {
            //RIGHT direction
            SCI_TRACE_LOW("[UITest]enter DisplayTestDirectionResult, Right direction");
            pass_flag[RIGHT_DIRECTION]=TRUE;
        }
        else if(temp_coordinae.x == 0)
        {
            SCI_TRACE_LOW("[UITest]enter DisplayTestDirectionResult, coordinate.x is zero, do nothing");
        }
    }

    for(i=0; i < MAX_DIRECTION; i++ )
    {
        text.wstr_len = SCI_STRLEN(str_direction[i]);
        text.wstr_ptr = wstr;
        SCI_MEMSET(wstr, 0 , (MOTION_SENSOR_DISP_MAX_LEN + 1)*sizeof(wchar));
        SCI_MEMSET(wstr_pass, 0 , 10*sizeof(wchar));
        MMI_STRNTOWSTR(wstr, MOTION_SENSOR_DISP_MAX_LEN , (uint8*)str_direction[i], MOTION_SENSOR_DISP_MAX_LEN ,  text.wstr_len);
        if(pass_flag[i] == TRUE)
        {
            text.wstr_len += SCI_STRLEN(str_pass);
            MMI_STRNTOWSTR(wstr_pass, SCI_STRLEN(str_pass) , str_pass, SCI_STRLEN(str_pass) ,  SCI_STRLEN(str_pass));
            MMIAPICOM_Wstrncat(text.wstr_ptr,wstr_pass,MMIAPICOM_Wstrlen(wstr_pass));
        }
        GUILABEL_SetText( MMIENG_LABEL4_CTRL_ID + i ,  &text,  TRUE);
    }
}
LOCAL  MMI_RESULT_E ENGMotionSensorTestWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    float acceleration[3] = {0};
    char str[MOTION_SENSOR_DISP_MAX_LEN+1] = {0} ;
    wchar wstr[MOTION_SENSOR_DISP_MAX_LEN+1] = {0};
    wchar wstr_data[MOTION_SENSOR_DISP_MAX_LEN+1] = {0};
    static int32 s_mmieng_motionsensor_timer_id = 0;
    const int32 timer_interval = 50 ;
    const char *str_coordinate[3] = {"X-direction: ", "Y-direction: ", "Z-direction: "};
    const char *str_direction[MAX_DIRECTION] = {"UP-direction:", "DOWN-direction:", "LEFT-direction:", "RIGHT-direction:"};
    int32 i = 0;
    MMI_STRING_T text = {0};
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef WIN32
        //no need to initmodule,because it was inited when boot the phone
        //MSensor_InitModule();
        MSensor_Open(MSENSOR_TYPE_ACCELEROMETER);
#endif
        //X, Y, Z coordinate tips
        for(i=0; i < 3; i++ )
        {
            text.wstr_len = SCI_STRLEN(str_coordinate[i]);
            text.wstr_ptr = wstr;
            SCI_MEMSET(wstr, 0 , (MOTION_SENSOR_DISP_MAX_LEN + 1)*sizeof(wchar));
            MMI_STRNTOWSTR(wstr, MOTION_SENSOR_DISP_MAX_LEN , (uint8*)str_coordinate[i], MOTION_SENSOR_DISP_MAX_LEN ,  text.wstr_len);
            GUILABEL_SetText( MMIENG_LABEL1_CTRL_ID + i ,  &text,  FALSE);
        }
        //direction tips
        for(i=0; i < MAX_DIRECTION; i++ )
        {
            text.wstr_len = SCI_STRLEN(str_direction[i]);
            text.wstr_ptr = wstr;
            SCI_MEMSET(wstr, 0 , (MOTION_SENSOR_DISP_MAX_LEN + 1)*sizeof(wchar));
            MMI_STRNTOWSTR(wstr, MOTION_SENSOR_DISP_MAX_LEN , (uint8*)str_direction[i], MOTION_SENSOR_DISP_MAX_LEN ,  text.wstr_len);
            GUILABEL_SetText( MMIENG_LABEL4_CTRL_ID + i ,  &text,  FALSE);
        }
        for(i=0;i<MAX_DIRECTION; i++)
        {
            pass_flag[i] = 0;
        }
        MMK_SetAtvCtrl( win_id,  MMIENG_FORM_CTRL_ID);
        if (s_mmieng_motionsensor_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_motionsensor_timer_id);
            s_mmieng_motionsensor_timer_id = 0;
        }
        s_mmieng_motionsensor_timer_id  =  MMK_CreateWinTimer(win_id, timer_interval, FALSE);      
        break;
        
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:     
        SaveTestResult( win_id, 1);
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(  win_id) ;    
        break;

    case MSG_KEYDOWN_CANCEL:  
    case MSG_CTL_CANCEL:    
        SaveTestResult( win_id, 0 );
        MMIAPIENG_RunUITestNextStep();
        MMK_CloseWin(  win_id) ;
        break;  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#if defined MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
#endif   
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CLOSE_WINDOW:
        if (s_mmieng_motionsensor_timer_id != 0)
        {
            MMK_StopTimer(s_mmieng_motionsensor_timer_id);
            s_mmieng_motionsensor_timer_id = 0;
        }    
#ifndef WIN32 
        //no need close motion sensor
        MSensor_Close(MSENSOR_TYPE_ACCELEROMETER);
#endif
        break;

    case MSG_TIMER:
        if (*(uint8*)param == s_mmieng_motionsensor_timer_id)
        {
            MSENSOR_MOTION_DATA_T sensor_data = {0};
            uint32               sensor_data_ex = 0;
            uint32 error_code = 0;  /*lint !e529 */
#ifdef WIN32
            acceleration[0] = 50;
            acceleration[1] = 100;
            acceleration[2] = 150;
            for(i=0; i < 3; i++ )
            {
                text.wstr_len = SCI_STRLEN(str_coordinate[i]);
                text.wstr_ptr = wstr;
                SCI_MEMSET(wstr, 0 , (MOTION_SENSOR_DISP_MAX_LEN + 1)*sizeof(wchar));
                MMI_STRNTOWSTR(wstr, MOTION_SENSOR_DISP_MAX_LEN , (uint8*)str_coordinate[i], MOTION_SENSOR_DISP_MAX_LEN ,  text.wstr_len);

                SCI_MEMSET(str, 0 , (MOTION_SENSOR_DISP_MAX_LEN + 1)*sizeof(char));
                SCI_MEMSET(wstr_data, 0 , (MOTION_SENSOR_DISP_MAX_LEN + 1)*sizeof(wchar));
                sprintf(str, "%.2f", acceleration[i]);
                text.wstr_len += SCI_STRLEN(str);
                MMI_STRNTOWSTR(wstr_data, MOTION_SENSOR_DISP_MAX_LEN , (uint8*)str, MOTION_SENSOR_DISP_MAX_LEN ,  SCI_STRLEN(str));
                MMIAPICOM_Wstrncat(text.wstr_ptr,wstr_data,MMIAPICOM_Wstrlen(wstr_data));
                GUILABEL_SetText( MMIENG_LABEL1_CTRL_ID + i ,  &text,  TRUE);
            }
#else
            //MSensor_GetVector(&sensor_data)  ;  //6800h a
            MSensor_ioctl(MSENSOR_TYPE_ACCELEROMETER, MSENSOR_IOCTL_GET_CAP, &sensor_data_ex);
            //SCI_TRACE_LOW:"MSensor_ioctl sensor_data_ex =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8448_112_2_18_2_17_40_123,(uint8*)"d",sensor_data_ex);

            if(sensor_data_ex & MSENSOR_MOTION_CAP_DATA)
            {
                error_code = MSensor_ioctl(MSENSOR_TYPE_ACCELEROMETER ,  MSENSOR_IOCTL_GET_DATA, &sensor_data);
                if(sensor_data_ex & MSENSOR_MOTION_CAP_XY_AXIS)
                {
                    acceleration[0] = sensor_data.u.vector.x ;
                    acceleration[1] = sensor_data.u.vector.y ;
                    //SCI_TRACE_LOW:"MSensor_ioctl X = %d, Y = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8457_112_2_18_2_17_40_124,(uint8*)"dd",sensor_data.u.vector.x, sensor_data.u.vector.y);
                }
                if(sensor_data_ex & MSENSOR_MOTION_CAP_Z_AXIS)
                {
                    acceleration[2] = sensor_data.u.vector.z ;
                    //SCI_TRACE_LOW:"MSensor_ioctl Z = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_8462_112_2_18_2_17_40_125,(uint8*)"d",sensor_data.u.vector.z);
                }

                //display X,Y,Z coordinate value
                for(i=0; i < 3; i++ )
                {
                    text.wstr_len = SCI_STRLEN(str_coordinate[i]);
                    text.wstr_ptr = wstr;
                    SCI_MEMSET(wstr, 0 , (MOTION_SENSOR_DISP_MAX_LEN + 1)*sizeof(wchar));
                    MMI_STRNTOWSTR(wstr, MOTION_SENSOR_DISP_MAX_LEN , (uint8*)str_coordinate[i], MOTION_SENSOR_DISP_MAX_LEN ,  text.wstr_len);

                    SCI_MEMSET(str, 0 , (MOTION_SENSOR_DISP_MAX_LEN + 1)*sizeof(char));
                    SCI_MEMSET(wstr_data, 0 , (MOTION_SENSOR_DISP_MAX_LEN + 1)*sizeof(wchar));
                    sprintf(str, "%.2f", acceleration[i]);
                    MMI_STRNTOWSTR(wstr_data, MOTION_SENSOR_DISP_MAX_LEN , (uint8*)str, MOTION_SENSOR_DISP_MAX_LEN ,  SCI_STRLEN(str));
                    text.wstr_len += MMIAPICOM_Wstrlen(wstr_data);
                    MMIAPICOM_Wstrncat(text.wstr_ptr,wstr_data,MMIAPICOM_Wstrlen(wstr_data));
                    SCI_TRACE_LOW("[UITest] enter ENGMotionSensorTestWinHandleMsg, coordinate string:%ls",text.wstr_ptr);
                    GUILABEL_SetText( MMIENG_LABEL1_CTRL_ID + i ,  &text,  TRUE);
                }

                //display direction result
                DisplayTestDirectionResult(sensor_data.u.vector);
                s_mmieng_motionsensor_timer_id = MMK_CreateWinTimer(win_id, timer_interval, FALSE);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_ENG_NOT_SUPPORT_MOTIONSENSOR);
                if (s_mmieng_motionsensor_timer_id != 0)
                {
                    MMK_StopTimer(s_mmieng_motionsensor_timer_id);
                }
                result = MMI_RESULT_FALSE;               
            }
#endif
        }
        else
            result = MMI_RESULT_FALSE;     
        break;
        
    case MSG_KEYDOWN_RED:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}  
#endif

#ifdef MMI_ENG_WIFI_SUPPORT   
/********************************************************************************
NAME:          MMIAPIENG_IsWifiUITest
DESCRIPTION:   is wifi ui test
AUTHOR:        jian.ma
DATE:    
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsWifiUITest(void)
{
    return MMK_IsOpenWin(MMIENG_UITESTWIFI_WIN_ID);
}
#endif

#ifdef KEYPAD_TYPE_QWERTY_KEYPAD
/*****************************************************************************/
// 	Description : convert qwertykey to typicalkey
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_MESSAGE_ID_E ConvertKeyFromQW(MMI_MESSAGE_ID_E msg_id)
{
    if (MMK_IsKeyMsg( msg_id))
    {
        uint8 key_code = msg_id & 0xff;

        key_code = MMIDEFAULT_ConvertQwertyKeyToTypicalKey(key_code);

        msg_id = (msg_id & 0xff00) | key_code;
    }

    return msg_id;
}
#endif

/*****************************************************************************/
// 	Description : SetLabelTextByCharArray
//	Global resource dependence :
//  Author:
//	Note:  subject to: buffer_size < 50
/*****************************************************************************/
LOCAL void SetLabelTextByCharArray(MMI_CTRL_ID_T ctrl_id,
                                   char    *buffer,
                                   uint32   buffer_size,   //must <50
                                   BOOLEAN  is_update)
{
    MMI_STRING_T    text ={0};
    uint32          str_length = 0;
    wchar           wstr[50] = {0};

    str_length  = SCI_STRLEN(buffer);
    SCI_MEMSET(wstr, 0,  buffer_size*sizeof(wchar));
    MMI_STRNTOWSTR(wstr, buffer_size, (uint8*)buffer, buffer_size, MIN(str_length,49));
    text.wstr_ptr = wstr ;
    text.wstr_len = MMIAPICOM_Wstrlen(text.wstr_ptr);
    GUILABEL_SetText( ctrl_id, &text, is_update);
}
PUBLIC void autotest_keypad(void)    // for autotest_device.c  bugID780430
{
   	s_is_fulltest = 0 ;
       SCI_TRACE_LOW("eng test autotest_keypad");
       MMK_CreateWin((uint32*)MMIENG_UITESTKEY_WIN_TAB,  PNULL);
}
/*Edit by script, ignore 4 case. Thu Apr 26 19:00:53 2012*/ //IGNORE9527


/*Edit by script, ignore 22 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
