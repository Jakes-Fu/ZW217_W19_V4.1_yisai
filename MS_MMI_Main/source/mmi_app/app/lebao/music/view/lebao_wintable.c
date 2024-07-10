/**********************
 *      INCLUDES
 *********************/
#include "port_cfg.h"

#include "std_header.h"
#include "mmk_app.h"
#include "mmi_common.h"
#include "mmk_timer.h"
#include "mmi_module.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"

#ifdef WIFI_SUPPORT
  #if !defined(PLATFORM_UWS6121E)
  #define LEBAO_WIFI_SUPPORT_CRACK   1
  #else
  #define LEBAO_WIFI_SUPPORT_CRACK   0
  #endif
#else
  #define LEBAO_WIFI_SUPPORT_CRACK   0
#endif  

#if (LEBAO_WIFI_SUPPORT_CRACK != 0)
#include "mmiwifi_export.h"
#endif

#include "lebao_ctrl.h"
#include "lebao_misc.h"
#include "lebao_win_base.h"
#include "lebao_app.h"

#include "lebao_id.h"
#include "lebao_text.h"
#include "lebao_image.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lebao_show_no_network(void);
static MMI_RESULT_E lebao_null_win_func(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
static MMI_RESULT_E lebao_hook_win_func(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/**********************
 *  STATIC VARIABLES
 **********************/

 // splash page
#ifdef LEBAO_FEATURE_PHONE_STYLE
WINDOW_TABLE(MMI_LEBAO_SPLASH_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_BACKGROUND_ID(IMAGE_LEBAO_SPLASH),
	WIN_ID(MMI_LEBAO_WIN_ID_SPLASH),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_LEBAO_CTRL_ID_LABEL_WELCOME),
	CREATE_ANIM_CTRL(MMI_LEBAO_CTRL_ID_IMG_LOADING, MMI_LEBAO_WIN_ID_SPLASH),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_HIDE_STATUS,
	END_WIN
};
#else
WINDOW_TABLE(MMI_LEBAO_SPLASH_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_BACKGROUND_ID(IMAGE_LEBAO_SPLASH),
	WIN_ID(MMI_LEBAO_WIN_ID_SPLASH),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_MOVE_STYLE(MOVE_FORBIDDEN),
	WIN_HIDE_STATUS,
	END_WIN
};
#endif

// menu page
#if defined(LEBAO_FEATURE_PHONE_STYLE)
WINDOW_TABLE(MMI_LEBAO_MENU_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_MAINMENU),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,		MMI_LEBAO_CTRL_ID_LIST_MENU),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};
#else
WINDOW_TABLE(MMI_LEBAO_MENU_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_MAINMENU),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,		MMI_LEBAO_CTRL_ID_LIST_MENU),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_MOVE_STYLE(MOVE_FORBIDDEN),
	WIN_HIDE_STATUS,
	END_WIN
};
#endif

// about page
#if defined(LEBAO_FEATURE_PHONE_STYLE)
WINDOW_TABLE(MMI_LEBAO_ABOUT_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_ABOUT),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,		MMI_LEBAO_CTRL_ID_LIST_ABOUT),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};
#else
#define MMI_LEBAO_ABOUT_WIN_TAB NULL
#endif

// charts page
#if defined(LEBAO_FEATURE_PHONE_STYLE)
WINDOW_TABLE(MMI_LEBAO_CHARTS_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_CHARTS),
	//WIN_TITLE(TEXT_LEBAO_MAIN_TITLE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,		MMI_LEBAO_CTRL_ID_LIST_CHARTS),
	//WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_HIDE_STATUS,
	END_WIN
};
#else
WINDOW_TABLE(MMI_LEBAO_CHARTS_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_CHARTS),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,		MMI_LEBAO_CTRL_ID_LIST_CHARTS),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_MOVE_STYLE(MOVE_FORBIDDEN),
	WIN_HIDE_STATUS,
	END_WIN
};
#endif

// music list page
#if defined(LEBAO_FEATURE_PHONE_STYLE)
WINDOW_TABLE(MMI_LEBAO_MUSICLIST_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_MUSICLIST),
	WIN_SOFTKEY(TEXT_LEBAO_PLAY, TXT_NULL, STXT_RETURN),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMI_LEBAO_CTRL_ID_LIST_MUSICLIST),
	WIN_HIDE_STATUS,
	END_WIN
};
#else
WINDOW_TABLE(MMI_LEBAO_MUSICLIST_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_MUSICLIST),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMI_LEBAO_CTRL_ID_LIST_MUSICLIST),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_MOVE_STYLE(MOVE_FORBIDDEN),
	WIN_HIDE_STATUS,
	END_WIN
};
#endif

// option menu page
#if defined(LEBAO_FEATURE_PHONE_STYLE)
WINDOW_TABLE(MMI_LEBAO_OPTION_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_OPTION),
	WIN_STYLE(WS_HAS_TRANSPARENT),
	WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
	END_WIN
};
#else
#define MMI_LEBAO_OPTION_WIN_TAB NULL
#endif

// player page
#if defined(LEBAO_FEATURE_PHONE_STYLE)
WINDOW_TABLE(MMI_LEBAO_PLAYER_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_PLAYER),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_SONG_NAME),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_SINGER_NAME),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_PREVIOU,	MMI_LEBAO_CTRL_ID_BTN_PREV),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_PLAY,	MMI_LEBAO_CTRL_ID_BTN_PLAY),
//	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_PAUSE,	MMI_LEBAO_CTRL_ID_BTN_PAUSE),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_NEXT,	MMI_LEBAO_CTRL_ID_BTN_NEXT),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_RING,	MMI_LEBAO_CTRL_ID_BTN_RING),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_FAVORITE,	MMI_LEBAO_CTRL_ID_BTN_FAVORITE),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,		MMI_LEBAO_CTRL_ID_LABEL_CURRENT_TIME),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_RIGHT,		MMI_LEBAO_CTRL_ID_LABEL_TOTAL_TIME),
	WIN_SOFTKEY(STXT_OPTION, TEXT_LEBAO_PLAY, STXT_RETURN),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_HIDE_STATUS,
	END_WIN
};
#else
WINDOW_TABLE(MMI_LEBAO_PLAYER_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_PLAYER),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_SONG_NAME),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_SINGER_NAME),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_PREVIOU,	MMI_LEBAO_CTRL_ID_BTN_PREV),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_PLAY,	MMI_LEBAO_CTRL_ID_BTN_PLAY),
//	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_PAUSE,	MMI_LEBAO_CTRL_ID_BTN_PAUSE),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_NEXT,	MMI_LEBAO_CTRL_ID_BTN_NEXT),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_RING,	MMI_LEBAO_CTRL_ID_BTN_RING),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_FAVORITE,	MMI_LEBAO_CTRL_ID_BTN_FAVORITE),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_VOLUME,	MMI_LEBAO_CTRL_ID_BTN_VOLUME),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_MOVE_STYLE(MOVE_FORBIDDEN),
	WIN_HIDE_STATUS,
	END_WIN
};
#endif

// volume page
#if defined(LEBAO_FEATURE_PHONE_STYLE)
WINDOW_TABLE(MMI_LEBAO_VOLUME_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_VOLUME),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};
#else
WINDOW_TABLE(MMI_LEBAO_VOLUME_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_VOLUME),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_VOLUME),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_VOL_ADD,	MMI_LEBAO_CTRL_ID_BTN_VOL_ADD),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_VOL_SUB,	MMI_LEBAO_CTRL_ID_BTN_VOL_SUB),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_MOVE_STYLE(MOVE_FORBIDDEN),
	WIN_HIDE_STATUS,
	END_WIN
};
#endif

// My favorites page
#if  defined(LEBAO_FEATURE_PHONE_STYLE) 
WINDOW_TABLE(MMI_LEBAO_LOCAL_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_LOCAL),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	//CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_DELETE,	MMI_LEBAO_CTRL_ID_BTN_DEL),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,		MMI_LEBAO_CTRL_ID_LIST_LOCAL),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};
#else
WINDOW_TABLE(MMI_LEBAO_LOCAL_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_LOCAL),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_BTN_DELETE,	MMI_LEBAO_CTRL_ID_BTN_DEL),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,		MMI_LEBAO_CTRL_ID_LIST_LOCAL),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_MOVE_STYLE(MOVE_FORBIDDEN),
	WIN_HIDE_STATUS,
	END_WIN
};
#endif

// search page
#if  defined(LEBAO_FEATURE_PHONE_STYLE) 
WINDOW_TABLE(MMI_LEBAO_SEARCH_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_SEARCH),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TIMER),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TIPS),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_RECORD_GRAY,	MMI_LEBAO_CTRL_ID_BTN_RECORD),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};
#else
WINDOW_TABLE(MMI_LEBAO_SEARCH_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_SEARCH),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TIMER),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TIPS),
	CREATE_BUTTON_CTRL(IMAGE_LEBAO_RECORD_GRAY,	MMI_LEBAO_CTRL_ID_BTN_RECORD),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_MOVE_STYLE(MOVE_FORBIDDEN),
	WIN_HIDE_STATUS,
	END_WIN
};
#endif

// waiting page
#if defined(LEBAO_FEATURE_PHONE_STYLE)
WINDOW_TABLE(MMI_LEBAO_WAITING_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_WAITING),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_LEBAO_CTRL_ID_LABEL_TIPS),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_LEBAO_CTRL_ID_LABEL_TIMER),
	CREATE_ANIM_CTRL(MMI_LEBAO_CTRL_ID_IMG_LOADING, MMI_LEBAO_WIN_ID_WAITING),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
	WIN_HIDE_STATUS,
	END_WIN
};
#else
WINDOW_TABLE(MMI_LEBAO_WAITING_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_WAITING),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_LEBAO_CTRL_ID_LABEL_TIPS),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_LEBAO_CTRL_ID_LABEL_TIMER),
	CREATE_ANIM_CTRL(MMI_LEBAO_CTRL_ID_IMG_LOADING, MMI_LEBAO_WIN_ID_WAITING),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_MOVE_STYLE(MOVE_FORBIDDEN),
	WIN_HIDE_STATUS,
	END_WIN
};
#endif

// order page
#if defined(LEBAO_FEATURE_PHONE_STYLE)
WINDOW_TABLE(MMI_LEBAO_ORDER_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_ORDER),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_ANIM_CTRL(MMI_LEBAO_CTRL_ID_IMG_QRCODE, 0),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMI_LEBAO_CTRL_ID_LABEL_SCAN),
	CREATE_BUTTON_CTRL(IMAGE_NULL, MMI_LEBAO_CTRL_ID_BTN_CALL),
	WIN_SOFTKEY(STXT_OPTION, TXT_COMMON_OK, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};

WINDOW_TABLE(MMI_LEBAO_ORDER_OPT_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_OPTION),
	WIN_STYLE(WS_HAS_TRANSPARENT),
	WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
	END_WIN
};

WINDOW_TABLE(MMI_LEBAO_VIP_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_BACKGROUND_ID(IMAGE_LEBAO_SPLASH),
	WIN_ID(MMI_LEBAO_WIN_ID_VIP),
	CREATE_ANIM_CTRL(MMI_LEBAO_CTRL_ID_IMG_VIP, 0),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TIPS),
	WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
	WIN_HIDE_STATUS,
	END_WIN
};
#else
WINDOW_TABLE(MMI_LEBAO_ORDER_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_ORDER),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_ANIM_CTRL(MMI_LEBAO_CTRL_ID_IMG_QRCODE, 0),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_SCAN),
	WIN_STYLE(WS_NO_DEFAULT_STYLE),
	WIN_HIDE_STATUS,
	WIN_MOVE_STYLE(MOVE_FORBIDDEN),
	END_WIN
};
#define MMI_LEBAO_ORDER_OPT_TAB NULL
#define MMI_LEBAO_VIP_WIN_TAB NULL
#endif

// confirm page
#if defined(LEBAO_FEATURE_PHONE_STYLE)
WINDOW_TABLE(MMI_LEBAO_CONFIRM_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_CONFIRM),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
	CREATE_ANIM_CTRL(MMI_LEBAO_CTRL_ID_IMG_QRCODE, 0),
	CREATE_TEXT_CTRL(MMI_LEBAO_CTRL_ID_TEXT_CONFIRM),
	WIN_HIDE_STATUS,
	END_WIN
};
#else
#define MMI_LEBAO_CONFIRM_WIN_TAB NULL
#endif

// login page
#if defined(LEBAO_FEATURE_PHONE_STYLE)
WINDOW_TABLE(MMI_LEBAO_LOGIN_WIN_TAB) =
{
	WIN_FUNC((uint32)lebao_null_win_func),
	WIN_ID(MMI_LEBAO_WIN_ID_LOGIN),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,	MMI_LEBAO_CTRL_ID_LABEL_TITLE),
//	CREATE_EDIT_DIGITAL_CTRL(20, MMI_LEBAO_CTRL_ID_EDIT_MOBILE),
	CREATE_EDIT_TEXT_CTRL(6, MMI_LEBAO_CTRL_ID_EDIT_CAPTCHA),
	CREATE_TEXT_CTRL(MMI_LEBAO_CTRL_ID_TEXT_CONFIRM),
	WIN_HIDE_STATUS,
	END_WIN
};
#else
#define MMI_LEBAO_LOGIN_WIN_TAB NULL
#endif

static struct
{
	int pageId;
	uint32* tab;
	int winId;
} _resWinId[] = {
	{ LEBAO_PAGE_SRV,		NULL, 0 },
	{ LEBAO_PAGE_CTRL,		NULL, 0 },
	{ LEBAO_PAGE_SPLASH,	MMI_LEBAO_SPLASH_WIN_TAB,	MMI_LEBAO_WIN_ID_SPLASH },
	{ LEBAO_PAGE_MENU,		MMI_LEBAO_MENU_WIN_TAB,		MMI_LEBAO_WIN_ID_MAINMENU },
	{ LEBAO_PAGE_CHART,		MMI_LEBAO_CHARTS_WIN_TAB,	MMI_LEBAO_WIN_ID_CHARTS },
	{ LEBAO_PAGE_MUSICLIST, MMI_LEBAO_MUSICLIST_WIN_TAB, MMI_LEBAO_WIN_ID_MUSICLIST },
	{ LEBAO_PAGE_PLAYER,	MMI_LEBAO_PLAYER_WIN_TAB,	MMI_LEBAO_WIN_ID_PLAYER },
	{ LEBAO_PAGE_VOLUME,	MMI_LEBAO_VOLUME_WIN_TAB,	MMI_LEBAO_WIN_ID_VOLUME },
	{ LEBAO_PAGE_LOCAL,		MMI_LEBAO_LOCAL_WIN_TAB,	MMI_LEBAO_WIN_ID_LOCAL },
	{ LEBAO_PAGE_SEARCH,	MMI_LEBAO_SEARCH_WIN_TAB,	MMI_LEBAO_WIN_ID_SEARCH },
	{ LEBAO_PAGE_RECORD,	NULL, 0 },
	{ LEBAO_PAGE_WAITING,	MMI_LEBAO_WAITING_WIN_TAB,	MMI_LEBAO_WIN_ID_WAITING },
	{ LEBAO_PAGE_ORDER,		MMI_LEBAO_ORDER_WIN_TAB,	MMI_LEBAO_WIN_ID_ORDER },
	{ LEBAO_PAGE_CLEAR,		NULL, 0 },
	{ LEBAO_PAGE_STREAM,	NULL, 0 },
	{ LEBAO_PAGE_ABOUT,		MMI_LEBAO_ABOUT_WIN_TAB,	MMI_LEBAO_WIN_ID_ABOUT },
	{ LEBAO_PAGE_CONFIRM,	MMI_LEBAO_CONFIRM_WIN_TAB,	MMI_LEBAO_WIN_ID_CONFIRM },
	{ LEBAO_PAGE_OPTION,	MMI_LEBAO_OPTION_WIN_TAB,	MMI_LEBAO_WIN_ID_OPTION },
	{ LEBAO_PAGE_VIP,		MMI_LEBAO_VIP_WIN_TAB,		MMI_LEBAO_WIN_ID_VIP },
	{ LEBAO_PAGE_LOGIN,		MMI_LEBAO_LOGIN_WIN_TAB,	MMI_LEBAO_WIN_ID_LOGIN },
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
LOCAL uint32 _lebaoLastStartTime = 0;


LOCAL int lebao_abs(x)
{
    return (x >= 0) ? (x) : (-x);
}

void MMIAPIMENU_EnterLebao(void)
{
	int ret = 0;
	uint32 nowTime = timestamp_seconds_int();

	// to avoid clicking too fast, it's actually started
	if(lebao_abs(nowTime - _lebaoLastStartTime) <= 1) {
		helper_debug_printf("restart time is too short or abnormal exit, now=%d, last=%d", nowTime, _lebaoLastStartTime);
		return;
	}

	_lebaoLastStartTime = nowTime;

#ifdef _WIN32
	if(1) {
		StartLebaoApp();
		return;
	}
#endif

	if (MMIAPIUDISK_UdiskIsRun()) {
		MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
		return;
	}
	else if (MMIAPICC_IsInState(CC_IN_CALL_STATE)) {		
		return;
	}

#if (LEBAO_WIFI_SUPPORT_CRACK != 0)
	// check the wifi status
	if (lebao_active_network() == TRUE) {
		StartLebaoApp();
		return;
	}
	else
#endif
	{
		uint16 sim_sys = MN_DUAL_SYS_1;
		uint32 sim_num = 0;

#if defined(LEBAO_PLATFORM_T117) && !defined(_WIN32)
		if (MMISET_IsSSProcessing() || (FALSE == MMIAPISET_GetIsQueryCfu())) {// || MMIUSBSHARE_IsRunning() || MMIMMS_IsMMSChangedDataCard()) {
			MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
			return;
		}
#endif
		
		sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
		if (sim_num <= 0) {
			lebao_show_no_network();
			return;
		}

		#ifdef LEBAO_FEATURE_PHONE_STYLE
			if (MMIAPISET_GetFlyMode()) {
				MMIPUB_OpenAlertWarningWin(TEXT_LEBAO_CAN_NOT_FLY_MODE);
				return;
			}

			if (lebao_gprs_is_opened() == FALSE) {
				MMIPUB_OpenAlertWarningWin(TEXT_LEBAO_OPEN_GPRS);
				return;
			}

			if (lebao_active_network() == FALSE) {
				lebao_show_no_network();
				return;
			}
		#else
			if (MMIAPIPDP_PsIsActivedPdpLinkExist() == FALSE) {
				lebao_show_no_network();
				return;
			}
		#endif
	}

	// MMIAPIENVSET_CloseActModeKeyRingSet();
	StartLebaoApp();
}

void MMIAPIMENU_QuitLebao(void)
{
	StopLebaoApp();

#ifdef LEBAO_FEATURE_PHONE_STYLE
	lebao_deactive_network();
#endif

	// MMIAPIENVSET_ResetActModeKeyRingSet();
	
	_lebaoLastStartTime = 0;
}

MMI_HANDLE_T lebao_create_win(const int pageId) 
{
	MMI_HANDLE_T handle = NULL;

	if (pageId <= 0 || pageId > LEBAO_PAGE_LOGIN)
		return NULL;

	if (_resWinId[pageId].tab == NULL || _resWinId[pageId].winId == 0)
		return NULL;

	if (MMK_IsOpenWin(_resWinId[pageId].winId))
		return NULL;

	handle = MMK_CreateWin((uint32*)_resWinId[pageId].tab, PNULL);
	if (handle != NULL)
		MMK_SetWinHookFunc(handle, lebao_hook_win_func);

	helper_debug_printf("pageId=%d, handle=0x%x", pageId, handle);
	return handle;
}

void lebao_close_win(const int pageId)
{
	helper_debug_printf("pageId=%d", pageId);

	if (pageId <= 0 || pageId > LEBAO_PAGE_LOGIN)
		return;

	if (_resWinId[pageId].tab == NULL || _resWinId[pageId].winId < MMI_LEBAO_WIN_ID_SPLASH || _resWinId[pageId].winId > MMI_LEBAO_WIN_ID_LOGIN)
		return;
	
	if (MMK_IsOpenWin(_resWinId[pageId].winId)) {
		MMK_CloseWin(_resWinId[pageId].winId);
	}
}

BOOLEAN	lebao_is_win_opened(const int pageId)
{
	if (pageId <= 0 || pageId > LEBAO_PAGE_LOGIN)
		return FALSE;

	if (_resWinId[pageId].tab == NULL || _resWinId[pageId].winId == 0)
		return FALSE;

	if (MMK_IsOpenWin(_resWinId[pageId].winId))
		return TRUE;

	return FALSE;
}

int lebao_get_pageid(int win_id)
{
	int  i = 0, count = 0;
	if(win_id < MMI_LEBAO_WIN_ID_SPLASH || win_id > MMI_LEBAO_WIN_ID_LOGIN)
		return 0;

	for(i = LEBAO_PAGE_SPLASH; i <= LEBAO_PAGE_LOGIN; i++) {
		if(_resWinId[i].winId == win_id)
			return _resWinId[i].pageId;
	}

	return 0;
}

#ifndef WIN32
// #if defined(TRACE_INFO_SUPPORT)
void helper_debug_printf(const char *fmt, ...)
{
   char str[256] = {0};
   va_list args;
   
   va_start(args, fmt);
   os_vsnprintf(str, sizeof(str) - 1, fmt, args);
   va_end(args);

   SCI_TraceLow("{%s} %s", "lebao", str);
}
// #else
// void helper_debug_printf(const char *fmt, ...)
// {
// }
// #endif
#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void lebao_show_no_network(void)
{
	MMIPUB_OpenAlertWarningWin(TEXT_LEBAO_NO_SIM_CARD);
}

static MMI_RESULT_E lebao_null_win_func(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	(void)win_id;
	(void)msg_id;
	(void)param;

	return MMI_RESULT_FALSE;
}

static MMI_RESULT_E lebao_back_btn_click(void) 
{
	MMK_DispMsgToFocusWin(MSG_CTL_CANCEL, NULL);
	return MMI_RESULT_TRUE;
}

static BOOLEAN _keyRing = FALSE;
static BOOLEAN _tpRing = FALSE;

static MMI_RESULT_E lebao_hook_win_func(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	// displays the back button of the wide screen
	if(msg_id == MSG_OPEN_WINDOW) {
		#if !defined(LEBAO_FEATURE_PHONE_STYLE)
		if(win_id != MMI_LEBAO_WIN_ID_SPLASH && win_id != MMI_LEBAO_WIN_ID_MAINMENU) {
			// 320x170, 480x200
			if(LEBAO_SCREEN_WIDTH > 240 && LEBAO_SCREEN_WIDTH > LEBAO_SCREEN_HEIGHT) {
				GUI_RECT_T backRect = { 5, 0, 5 + 36, 0 + 36 }; // 36 x 36
				MMI_CTRL_ID_T backId = MMI_LEBAO_CTRL_ID_BTN_BACK;
				GUIBUTTON_INIT_DATA_T initData = {0};
				
				initData.both_rect.h_rect = initData.both_rect.v_rect = backRect;
				initData.bg.bg_type = GUI_BG_IMG;
				initData.bg.img_id  = IMAGE_LEBAO_BTN_BACK;
				
				if(GUIBUTTON_CreateDynamic(win_id, backId, &initData) != NULL) {
					GUIBUTTON_SetRect(backId, &backRect);
					GUIBUTTON_SetRunSheen(backId, FALSE);
					GUIBUTTON_SetCallBackFunc(backId, &lebao_back_btn_click);
				}
			}
		}
		#endif
	}

	// hide softkeybar
	if(msg_id == MSG_OPEN_WINDOW || msg_id == MSG_FULL_PAINT) {
		#if defined(LEBAO_HIDE_SOFTKEYBAR)
		GUIWIN_SetSoftkeyVisible(win_id, FALSE);
		#endif
	}

	// whether to turn off the key tone and turn off the screen
	#if defined(LEBAO_FEATURE_PHONE_STYLE)
	if(win_id == MMI_LEBAO_WIN_ID_PLAYER) {
		if(msg_id == MSG_OPEN_WINDOW) {
			_keyRing = MMIDEFAULT_IsEnableKeyRing();
			_tpRing  = MMIDEFAULT_IsEnableTpRing();
			MMIDEFAULT_EnableKeyTpRing("lebao", FALSE);

			if (lebao_player_is_backend() == 0) {
				MMIDEFAULT_TurnOnBackLight();
				MMIDEFAULT_AllowTurnOffBackLight(FALSE);
			}			
		}
		else if(msg_id == MSG_CLOSE_WINDOW) {
			MMIDEFAULT_EnableKeyRing("lebao", _keyRing);
			MMIDEFAULT_EnableTpRing("lebao", _tpRing);

			if (lebao_player_is_backend() == 0) {
				MMIDEFAULT_AllowTurnOffBackLight(TRUE);
			}		
		}
	}
	#endif

	// quit app
	if(msg_id == MSG_APP_RED) {
		#if 1
			MMIAPIMENU_QuitLebao();
			return MMI_RESULT_TRUE; // Must be true !!! otherwise, crash
		#else
			MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
			return MMI_RESULT_TRUE;
			// or
			// MMK_CloseWin(win_id);
			// or	
			// int pageId = lebao_get_pageid(win_id);
			// if(pageId != 0 && pageId != LEBAO_PAGE_SPLASH)
			//	 lebao_event_send(EVT_CTRL_LEBAO_CLOSE_PAGE, LEBAO_PAGE_CTRL, pageId, 0);
		#endif
	}
	
	return MMI_RESULT_FALSE;
}

