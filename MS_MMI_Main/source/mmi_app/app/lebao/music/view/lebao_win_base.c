/**********************
 *      INCLUDES
 *********************/
#include "port_cfg.h"
#include "std_header.h"

#include "lebao_win_base.h"
#include "lebao_misc.h"

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
static int	_lebao_convert_to_wstring(const char * text, const int charsetType, wchar*  buffer, const int bufLen);
static void	_lebao_show_anim_image(MMI_CTRL_ID_T ctrl_id, const int imgId, const char* filename, GUI_RECT_T* img_rect, const BOOLEAN isZoom);
static void	_lebao_set_lable_font(MMI_CTRL_ID_T ctrl_id, GUI_FONT_T font, GUI_COLOR_T font_color, const char * text, const int charsetType);
static void _lebao_set_text_font(MMI_CTRL_ID_T ctrl_id, GUI_FONT_T font, GUI_COLOR_T font_color, const char * text, const int charsetType);
static void	_lebao_create_msgbox(const int textId, const char* title, const int ms, const int charsetType);
/**********************
 *  STATIC VARIABLES
 **********************/

// theme
#define	LEBAO_SCREEN_WIDTH_MAX			MMI_MAINSCREEN_WIDTH // MMI_MAINSCREEN_WIDTH
#define	LEBAO_SCREEN_HEIGHT_MAX			MMI_MAINSCREEN_HEIGHT // MMI_MAINSCREEN_HEIGHT

//MAINLCD_SIZE_240X320
//MAINLCD_SIZE_128X128
//MAINLCD_SIZE_240X240

#if (LEBAO_SCREEN_WIDTH_MAX == 128) && (LEBAO_SCREEN_HEIGHT_MAX == 128)
#define LEBAO_SIZE_128X128
#elif (LEBAO_SCREEN_WIDTH_MAX == 240) && (LEBAO_SCREEN_HEIGHT_MAX == 320)
#define LEBAO_SIZE_240X320
#else
#define LEBAO_SIZE_240X240
#endif

#ifdef MAINLCD_CIRCULAR_WATCH
	static int _isRoundWatch = 1;
	#if LEBAO_SCREEN_WIDTH_MAX > 360
		#define LEBAO_ROUND_PADDING_		(60)
	#elif LEBAO_SCREEN_WIDTH_MAX > 240
		#define LEBAO_ROUND_PADDING_		(40)
	#elif LEBAO_SCREEN_WIDTH_MAX <= 128
		#define LEBAO_ROUND_PADDING_		(8)
	#else
		#define LEBAO_ROUND_PADDING_		(20)
	#endif
#else
	static int _isRoundWatch = 0;
	#define LEBAO_ROUND_PADDING_			(0)
#endif

static int _labelCanRoll = 0;

#if defined(LEBAO_SIZE_240X320)
	#define  LEBAO_TITLE_HEIGHT_			(40)
	#include "lebao_theme_p_240X320.def"
#elif defined(LEBAO_SIZE_128X128)
	#define  LEBAO_TITLE_HEIGHT_			(20)
	#include "lebao_theme_w_128X128.def"
#else
	#define  LEBAO_TITLE_HEIGHT_			(36)

	#if defined(LEBAO_FEATURE_PHONE_STYLE)
	#include "lebao_theme_p_240X320.def"
	#else
	#include "lebao_theme_w_240X240.def"
	#endif
#endif

static struct
{
	int index;
	int resId;
} _resTextId[] = {
	{ 0, 0 }, // RES_TEXT_LEBAO_BEGIN
	{ RES_TEXT_LEBAO_WELCOME,		TEXT_LEBAO_WELCOME },
	{ RES_TEXT_LEBAO_MAIN_TITLE,	TEXT_LEBAO_MAIN_TITLE },
	{ RES_TEXT_LEBAO_MENU_CHARTS,	TEXT_LEBAO_MENU_CHARTS },
	{ RES_TEXT_LEBAO_MENU_SEARCH,	TEXT_LEBAO_MENU_SEARCH },
	{ RES_TEXT_LEBAO_MENU_LOCAL,	TEXT_LEBAO_MENU_LOCAL },
	{ RES_TEXT_LEBAO_MENU_CLEAR,	TEXT_LEBAO_MENU_CLEAR },
	{ RES_TEXT_LEBAO_MENU_ORDER,	TEXT_LEBAO_MENU_ORDER },
	{ RES_TEXT_LEBAO_MENU_SETTING,	TEXT_LEBAO_MENU_SETTING },
	{ RES_TEXT_LEBAO_MENU_ABOUT,	TEXT_LEBAO_MENU_ABOUT },
	{ RES_TEXT_LEBAO_MENU_SERVICE,	TEXT_LEBAO_MENU_SERVICE },
	{ RES_TEXT_LEBAO_MENU_PRIVACY,	TEXT_LEBAO_MENU_PRIVACY },
	{ RES_TEXT_LEBAO_MENU_COPYRIGHT,TEXT_LEBAO_MENU_COPYRIGHT },
	{ RES_TEXT_LEBAO_MENU_FAVORITES,TEXT_LEBAO_MENU_FAVORITES },
	{ RES_TEXT_LEBAO_OPT_OPEN,		TEXT_LEBAO_OPT_OPEN },
	{ RES_TEXT_LEBAO_OPT_LOGIN,		TEXT_LEBAO_OPT_LOGIN },
	{ RES_TEXT_LEBAO_OPT_INTEREST,	TEXT_LEBAO_OPT_INTEREST },
	{ RES_TEXT_LEBAO_OPT_LOOP,		TEXT_LEBAO_OPT_LOOP },
	{ RES_TEXT_LEBAO_OPT_CANCEL_LOOP,	TEXT_LEBAO_OPT_CANCEL_LOOP },
	{ RES_TEXT_LEBAO_OPT_REPEAT,		TEXT_LEBAO_OPT_REPEAT },
	{ RES_TEXT_LEBAO_OPT_SHUFFLE,		TEXT_LEBAO_OPT_SHUFFLE },
	{ RES_TEXT_LEBAO_OPT_FAVORITE,		TEXT_LEBAO_OPT_FAVORITE },
	{ RES_TEXT_LEBAO_OPT_RING,			TEXT_LEBAO_OPT_RING },
	{ RES_TEXT_LEBAO_OPT_FAVORITE_DEL,	TEXT_LEBAO_OPT_FAVORITE_DEL },
	{ RES_TEXT_LEBAO_OPT_RING_DEL,		TEXT_LEBAO_OPT_RING_DEL },
	{ RES_TEXT_LEBAO_OPT_12530,			TEXT_LEBAO_OPT_12530 },
	{ RES_TEXT_LEBAO_LIST,			TEXT_LEBAO_LIST },
	{ RES_TEXT_LEBAO_PLAY,			TEXT_LEBAO_PLAY },
	{ RES_TEXT_LEBAO_PAUSE,			TEXT_LEBAO_PAUSE },
	{ RES_TEXT_LEBAO_STOP,			TEXT_LEBAO_STOP },
	{ RES_TEXT_LEBAO_RING,			TEXT_LEBAO_RING },
	{ RES_TEXT_LEBAO_DEL,			TEXT_LEBAO_DEL },
	{ RES_TEXT_LEBAO_EMPTY,			TEXT_LEBAO_EMPTY },
	{ RES_TEXT_LEBAO_REMINDER,		TEXT_LEBAO_REMINDER },
	{ RES_TEXT_LEBAO_INTEREST,		TEXT_LEBAO_INTEREST },
	{ RES_TEXT_LEBAO_ONEKEY_SMS,	TEXT_LEBAO_ONEKEY_SMS },
	{ RES_TEXT_LEBAO_CONFIRM,		TEXT_LEBAO_CONFIRM },
	{ RES_TEXT_LEBAO_AGREE,			TEXT_LEBAO_AGREE },
	{ RES_TEXT_LEBAO_SUCCESSED,		TEXT_LEBAO_SUCCESSED },
	{ RES_TEXT_LEBAO_FAILED,		TEXT_LEBAO_FAILED },
	{ RES_TEXT_LEBAO_NEED_VIP,		TEXT_LEBAO_NEED_VIP },
	{ RES_TEXT_LEBAO_TIP_IVR,		TEXT_LEBAO_TIP_IVR },
	{ RES_TEXT_LEBAO_LOGIN,			TEXT_LEBAO_LOGIN },
	{ RES_TEXT_LEBAO_MOBILE,		TEXT_LEBAO_MOBILE },
	{ RES_TEXT_LEBAO_CAPTCHA,		TEXT_LEBAO_CAPTCHA },
	{ RES_TEXT_LEBAO_PASSWORD,		TEXT_LEBAO_PASSWORD },
	{ RES_TEXT_LEBAO_WAITING,		TEXT_LEBAO_WAITING },
	{ RES_TEXT_LEBAO_RENEW,			TEXT_LEBAO_RENEW },
	{ RES_TEXT_LEBAO_NO_MUSIC, 		TEXT_LEBAO_NO_MUSIC },
	{ RES_TEXT_LEBAO_PLAY_FAILED, 	TEXT_LEBAO_PLAY_FAILED },
	{ RES_TEXT_LEBAO_RING_FAILED, 	TEXT_LEBAO_RING_FAILED },
	{ RES_TEXT_LEBAO_SAY_NAME, 		TEXT_LEBAO_SAY_NAME },
	{ RES_TEXT_LEBAO_PUSH_TALK, 	TEXT_LEBAO_PUSH_TALK },
	{ RES_TEXT_LEBAO_OVER_TALK, 	TEXT_LEBAO_OVER_TALK },
	{ RES_TEXT_LEBAO_TIME_SHORT, 	TEXT_LEBAO_TIME_SHORT },
	{ RES_TEXT_LEBAO_SETTING_VOLUME,TEXT_LEBAO_SETTING_VOLUME },
	{ RES_TEXT_LEBAO_SEARCHING, 	TEXT_LEBAO_SEARCHING },
	{ RES_TEXT_LEBAO_FOUND_NOTHING,	TEXT_LEBAO_FOUND_NOTHING },
	{ RES_TEXT_LEBAO_SCAN_QR, 		TEXT_LEBAO_SCAN_QR },
	{ RES_TEXT_LEBAO_RING_OK, 		TEXT_LEBAO_RING_OK },
	{ RES_TEXT_LEBAO_MEMORY_FULL, 	TEXT_LEBAO_MEMORY_FULL },
	{ RES_TEXT_LEBAO_ERR_DOWNLOAD,	TEXT_LEBAO_ERR_DOWNLOAD },
	{ RES_TEXT_LEBAO_SEARCH_LIMIT,	TEXT_LEBAO_SEARCH_LIMIT },
	{ RES_TEXT_LEBAO_ADD_FAVORITE,	TEXT_LEBAO_ADD_FAVORITE },
	{ RES_TEXT_LEBAO_DEL_FAVORITE,	TEXT_LEBAO_DEL_FAVORITE },
	{ RES_TEXT_LEBAO_DOWNLOAD_OK, 	TEXT_LEBAO_DOWNLOAD_OK },
	{ RES_TEXT_LEBAO_NO_SIM_CARD, 	TEXT_LEBAO_NO_SIM_CARD },
	{ RES_TEXT_LEBAO_NO_MORE, 		TEXT_LEBAO_NO_MORE },
	{ RES_TEXT_LEBAO_MUSIC_LIST, 	TEXT_LEBAO_MUSIC_LIST },
	{ RES_TEXT_LEBAO_ERR_NETWORK,	TEXT_LEBAO_ERR_NETWORK },
	{ RES_TEXT_LEBAO_ERR_SERVICE,	TEXT_LEBAO_ERR_SERVICE },
	{ RES_TEXT_LEBAO_ERR_UPDATE,	TEXT_LEBAO_ERR_UPDATE },
	{ RES_TEXT_LEBAO_CAN_NOT_FLY_MODE,	TEXT_LEBAO_CAN_NOT_FLY_MODE },
	{ RES_TEXT_LEBAO_OPEN_GPRS,			TEXT_LEBAO_OPEN_GPRS },
	{ RES_TEXT_LEBAO_SERVICE_INFO,		TEXT_LEBAO_SERVICE_INFO },
	{ RES_TEXT_LEBAO_PRIVACY_INFO,		TEXT_LEBAO_PRIVACY_INFO },
	{ RES_TEXT_LEBAO_COPYRIGHT_INFO,	TEXT_LEBAO_COPYRIGHT_INFO },
	{ RES_TEXT_LEBAO_PLAYER_NOTIRY,		TEXT_LEBAO_PLAYER_NOTIRY },
	{ RES_TEXT_LEBAO_PUSH_TALK_PHONE,	TEXT_LEBAO_PUSH_TALK_PHONE },
	{ RES_TEXT_LEBAO_TIPS_LOADING,		TEXT_LEBAO_TIPS_LOADING },
	{ RES_TEXT_LEBAO_TIPS_VIP_ALREADY,	TEXT_LEBAO_TIPS_VIP_ALREADY },
	{ RES_TEXT_LEBAO_TIPS_CLEAN,		TEXT_LEBAO_TIPS_CLEAN },
	{ RES_TEXT_LEBAO_TIPS_BUSY,			TEXT_LEBAO_TIPS_BUSY },
	{ RES_TEXT_LEBAO_TIPS_VERIFY,		TEXT_LEBAO_TIPS_VERIFY },
	{ RES_TEXT_LEBAO_TIPS_NEED_CMCC,	TEXT_LEBAO_TIPS_NEED_CMCC },
	{ RES_TEXT_LEBAO_TIPS_QUERYING,		TEXT_LEBAO_TIPS_QUERYING },
	{ RES_TEXT_LEBAO_TIPS_OPENING,		TEXT_LEBAO_TIPS_OPENING },
	{ RES_TEXT_LEBAO_TIPS_SUBMIT,		TEXT_LEBAO_TIPS_SUBMIT },
	{ RES_TEXT_LEBAO_TIPS_FAILED,		TEXT_LEBAO_TIPS_FAILED },
	{ RES_TEXT_LEBAO_TIPS_TRY_IVR,		TEXT_LEBAO_TIPS_TRY_IVR },
	{ RES_TEXT_LEBAO_TIPS_LOGIN,		TEXT_LEBAO_TIPS_LOGIN },
	{ RES_TEXT_LEBAO_TIPS_LOGIN_OK,		TEXT_LEBAO_TIPS_LOGIN_OK },
	{ RES_TEXT_LEBAO_TIPS_LOGIN_FAILED,	TEXT_LEBAO_TIPS_LOGIN_FAILED },
	{ RES_TEXT_LEBAO_TIPS_TIMEOUT,		TEXT_LEBAO_TIPS_TIMEOUT },
	{ RES_TEXT_LEBAO_TIPS_TRY_QR,		TEXT_LEBAO_TIPS_TRY_QR },
	{ RES_TEXT_LEBAO_TIPS_ORDER_OK,		TEXT_LEBAO_TIPS_ORDER_OK },
	{ RES_TEXT_LEBAO_TIPS_VIP_TYPE,		TEXT_LEBAO_TIPS_VIP_TYPE },
	{ RES_TEXT_LEBAO_TIPS_THANK,		TEXT_LEBAO_TIPS_THANK },
	{ 0, 0 } // RES_TEXT_LEBAO_END
};

static struct
{
	int index;
	int resId;
} _resImgId[] = {
	{ 0, 0 }, // RES_IMAGE_LEBAO_BEGIN
	{ RES_IMAGE_LEBAO_LOGO,				IMAGE_LEBAO_LOGO },
	{ RES_IMAGE_LEBAO_SPLASH,			IMAGE_LEBAO_SPLASH },
	{ RES_IMAGE_LEBAO_MENU_CHATS,		IMAGE_LEBAO_MENU_CHATS },
	{ RES_IMAGE_LEBAO_MENU_SEARCH,		IMAGE_LEBAO_MENU_SEARCH },
	{ RES_IMAGE_LEBAO_MENU_LOCAL,		IMAGE_LEBAO_MENU_LOCAL },
	{ RES_IMAGE_LEBAO_MENU_CLEAR,		IMAGE_LEBAO_MENU_CLEAR },
	{ RES_IMAGE_LEBAO_MENU_ORDER,		IMAGE_LEBAO_MENU_ORDER },
	{ RES_IMAGE_LEBAO_MENU_SETTING,		IMAGE_LEBAO_MENU_SETTING },
	{ RES_IMAGE_LEBAO_BTN_BACK,			IMAGE_LEBAO_BTN_BACK },
	{ RES_IMAGE_LEBAO_BTN_CLOSE,		IMAGE_LEBAO_BTN_CLOSE },
	{ RES_IMAGE_LEBAO_BTN_VOLUME,		IMAGE_LEBAO_BTN_VOLUME },
	{ RES_IMAGE_LEBAO_BTN_DELETE,		IMAGE_LEBAO_BTN_DELETE },
	{ RES_IMAGE_LEBAO_BTN_FAVORITE,		IMAGE_LEBAO_BTN_FAVORITE },
	{ RES_IMAGE_LEBAO_BTN_FAVORITE_SEL,	IMAGE_LEBAO_BTN_FAVORITE_SEL },
	{ RES_IMAGE_LEBAO_BTN_NEXT,			IMAGE_LEBAO_BTN_NEXT },
	{ RES_IMAGE_LEBAO_BTN_PREVIOU,		IMAGE_LEBAO_BTN_PREVIOU },
	{ RES_IMAGE_LEBAO_BTN_RING,			IMAGE_LEBAO_BTN_RING },
	{ RES_IMAGE_LEBAO_BTN_RING_SEL,		IMAGE_LEBAO_BTN_RING_SEL },
	{ RES_IMAGE_LEBAO_BTN_PLAY,			IMAGE_LEBAO_BTN_PLAY },
	{ RES_IMAGE_LEBAO_BTN_PAUSE,		IMAGE_LEBAO_BTN_PAUSE },
	{ RES_IMAGE_LEBAO_BTN_VOL_ADD,		IMAGE_LEBAO_BTN_VOL_ADD },
	{ RES_IMAGE_LEBAO_BTN_VOL_SUB,		IMAGE_LEBAO_BTN_VOL_SUB },
	{ RES_IMAGE_LEBAO_ITEM_D1,			IMAGE_LEBAO_ITEM_D1 },
	{ RES_IMAGE_LEBAO_ITEM_D2,			IMAGE_LEBAO_ITEM_D2 },
	{ RES_IMAGE_LEBAO_ITEM_D3,			IMAGE_LEBAO_ITEM_D3 },
	{ RES_IMAGE_LEBAO_BTN_DEFAULT,		IMAGE_LEBAO_BTN_DEFAULT },
	{ RES_IMAGE_LEBAO_RECORD,			IMAGE_LEBAO_RECORD },
	{ RES_IMAGE_LEBAO_RECORD_GRAY,		IMAGE_LEBAO_RECORD_GRAY },
	{ RES_IMAGE_LEBAO_BTN_STAR,			IMAGE_LEBAO_BTN_STAR },
	{ RES_IMAGE_LEBAO_LINE,				IMAGE_LEBAO_LINE },
	{ RES_IMAGE_LEBAO_VIP,				IMAGE_LEBAO_VIP },
	{ RES_IMAGE_LEBAO_INDICATOR,		IMAGE_LEBAO_INDICATOR },
	{ RES_IMAGE_LEBAO_LOADING,			IMAGE_LEBAO_LOADING },
#if defined(LEBAO_FEATURE_PHONE_STYLE)
	{ RES_IMAGE_LEBAO_MENU_ABOUT,		IMAGE_LEBAO_MENU_ABOUT },
	{ RES_IMAGE_LEBAO_VOLUME_1,			IMAGE_LEBAO_VOLUME_1 },
	{ RES_IMAGE_LEBAO_VOLUME_2,			IMAGE_LEBAO_VOLUME_2 },
	{ RES_IMAGE_LEBAO_VOLUME_3,			IMAGE_LEBAO_VOLUME_3 },
	{ RES_IMAGE_LEBAO_VOLUME_4,			IMAGE_LEBAO_VOLUME_4 },
	{ RES_IMAGE_LEBAO_VOLUME_5,			IMAGE_LEBAO_VOLUME_5 },
#endif
	{ 0, 0 } // RES_IMAGE_LEBAO_END
};

static struct
{
	int index;
	int resId;
} _resCtrlId[] = {
	{ 0, 0 }, // RES_MMI_LEBAO_CTRL_ID_START

	// menu
	{ RES_MMI_LEBAO_CTRL_ID_MENU_OPTION,		MMI_LEBAO_CTRL_ID_MENU_OPTION },

	// list
	{ RES_MMI_LEBAO_CTRL_ID_LIST_MENU,			MMI_LEBAO_CTRL_ID_LIST_MENU },
	{ RES_MMI_LEBAO_CTRL_ID_LIST_CHARTS,		MMI_LEBAO_CTRL_ID_LIST_CHARTS },
	{ RES_MMI_LEBAO_CTRL_ID_LIST_MUSICLIST,		MMI_LEBAO_CTRL_ID_LIST_MUSICLIST },
	{ RES_MMI_LEBAO_CTRL_ID_LIST_PLAYER,		MMI_LEBAO_CTRL_ID_LIST_PLAYER },
	{ RES_MMI_LEBAO_CTRL_ID_LIST_LOCAL,			MMI_LEBAO_CTRL_ID_LIST_LOCAL },
	{ RES_MMI_LEBAO_CTRL_ID_LIST_ABOUT,			MMI_LEBAO_CTRL_ID_LIST_ABOUT },

	// progress
	{ RES_MMI_LEBAO_CTRL_ID_PGB_MUSIC,			MMI_LEBAO_CTRL_ID_PGB_MUSIC },

	// button
	{ RES_MMI_LEBAO_CTRL_ID_BTN_VOLUME,			MMI_LEBAO_CTRL_ID_BTN_VOLUME },
	{ RES_MMI_LEBAO_CTRL_ID_BTN_BACK,			MMI_LEBAO_CTRL_ID_BTN_BACK },
	{ RES_MMI_LEBAO_CTRL_ID_BTN_PLAY,			MMI_LEBAO_CTRL_ID_BTN_PLAY },
	{ RES_MMI_LEBAO_CTRL_ID_BTN_PAUSE,			MMI_LEBAO_CTRL_ID_BTN_PAUSE },
	{ RES_MMI_LEBAO_CTRL_ID_BTN_NEXT,			MMI_LEBAO_CTRL_ID_BTN_NEXT },
	{ RES_MMI_LEBAO_CTRL_ID_BTN_PREV,			MMI_LEBAO_CTRL_ID_BTN_PREV },
	{ RES_MMI_LEBAO_CTRL_ID_BTN_RING,			MMI_LEBAO_CTRL_ID_BTN_RING },
	{ RES_MMI_LEBAO_CTRL_ID_BTN_DEL,			MMI_LEBAO_CTRL_ID_BTN_DEL },
	{ RES_MMI_LEBAO_CTRL_ID_BTN_VOL_ADD,		MMI_LEBAO_CTRL_ID_BTN_VOL_ADD },
	{ RES_MMI_LEBAO_CTRL_ID_BTN_VOL_SUB,		MMI_LEBAO_CTRL_ID_BTN_VOL_SUB },
	{ RES_MMI_LEBAO_CTRL_ID_BTN_RECORD,			MMI_LEBAO_CTRL_ID_BTN_RECORD },
	{ RES_MMI_LEBAO_CTRL_ID_BTN_FAVORITE,		MMI_LEBAO_CTRL_ID_BTN_FAVORITE },
	{ RES_MMI_LEBAO_CTRL_ID_BTN_CALL,			MMI_LEBAO_CTRL_ID_BTN_CALL },

	// image
	{ RES_MMI_LEBAO_CTRL_ID_IMG_QRCODE,			MMI_LEBAO_CTRL_ID_IMG_QRCODE },
	{ RES_MMI_LEBAO_CTRL_ID_IMG_RECORD,			MMI_LEBAO_CTRL_ID_IMG_RECORD },
	{ RES_MMI_LEBAO_CTRL_ID_IMG_LOADING,		MMI_LEBAO_CTRL_ID_IMG_LOADING },
	{ RES_MMI_LEBAO_CTRL_ID_IMG_VIP,			MMI_LEBAO_CTRL_ID_IMG_VIP },

	// editbox
	{ RES_MMI_LEBAO_CTRL_ID_EDIT_MOBILE,		MMI_LEBAO_CTRL_ID_EDIT_MOBILE },
	{ RES_MMI_LEBAO_CTRL_ID_EDIT_CAPTCHA,		MMI_LEBAO_CTRL_ID_EDIT_CAPTCHA },
	{ RES_MMI_LEBAO_CTRL_ID_EDIT_USERNAME,		MMI_LEBAO_CTRL_ID_EDIT_USERNAME },
	{ RES_MMI_LEBAO_CTRL_ID_EDIT_PASSWORD,		MMI_LEBAO_CTRL_ID_EDIT_PASSWORD },
	{ RES_MMI_LEBAO_CTRL_ID_EDIT_CONTENT,		MMI_LEBAO_CTRL_ID_EDIT_CONTENT },

	// text
	{ RES_MMI_LEBAO_CTRL_ID_TEXT_CONFIRM,		MMI_LEBAO_CTRL_ID_TEXT_CONFIRM },

	// label
	{ RES_MMI_LEBAO_CTRL_ID_LABEL_WELCOME,		MMI_LEBAO_CTRL_ID_LABEL_WELCOME },
	{ RES_MMI_LEBAO_CTRL_ID_LABEL_TITLE,		MMI_LEBAO_CTRL_ID_LABEL_TITLE },
	{ RES_MMI_LEBAO_CTRL_ID_LABEL_SCAN,			MMI_LEBAO_CTRL_ID_LABEL_SCAN },
	{ RES_MMI_LEBAO_CTRL_ID_LABEL_VOLUME,		MMI_LEBAO_CTRL_ID_LABEL_VOLUME },
	{ RES_MMI_LEBAO_CTRL_ID_LABEL_TIMER,		MMI_LEBAO_CTRL_ID_LABEL_TIMER },
	{ RES_MMI_LEBAO_CTRL_ID_LABEL_SONG_NAME,	MMI_LEBAO_CTRL_ID_LABEL_SONG_NAME },
	{ RES_MMI_LEBAO_CTRL_ID_LABEL_SINGER_NAME,	MMI_LEBAO_CTRL_ID_LABEL_SINGER_NAME },
	{ RES_MMI_LEBAO_CTRL_ID_LABEL_TIPS,			MMI_LEBAO_CTRL_ID_LABEL_TIPS },
	{ RES_MMI_LEBAO_CTRL_ID_LABEL_SCAN_DESC,	MMI_LEBAO_CTRL_ID_LABEL_SCAN_DESC },
	{ RES_MMI_LEBAO_CTRL_ID_LABEL_VOLUME_TITLE,	MMI_LEBAO_CTRL_ID_LABEL_VOLUME_TITLE },
	{ RES_MMI_LEBAO_CTRL_ID_LABEL_CURRENT_TIME,	MMI_LEBAO_CTRL_ID_LABEL_CURRENT_TIME },
	{ RES_MMI_LEBAO_CTRL_ID_LABEL_TOTAL_TIME,	MMI_LEBAO_CTRL_ID_LABEL_TOTAL_TIME },
	{ RES_MMI_LEBAO_CTRL_ID_LABEL_MOBILE,		MMI_LEBAO_CTRL_ID_LABEL_MOBILE },
	{ RES_MMI_LEBAO_CTRL_ID_LABEL_CAPTCHA,		MMI_LEBAO_CTRL_ID_LABEL_CAPTCHA },

	{ 0, 0 } // RES_MMI_LEBAO_CTRL_ID_MAX
};

static struct
{
	int index;
	int resId;
} _resWinId[] = {
	{ 0, 0 }, // RES_MMI_LEBAO_WIN_ID_START
	{ RES_MMI_LEBAO_WIN_ID_SPLASH,		MMI_LEBAO_WIN_ID_SPLASH },
	{ RES_MMI_LEBAO_WIN_ID_MAINMENU,	MMI_LEBAO_WIN_ID_MAINMENU },
	{ RES_MMI_LEBAO_WIN_ID_CHARTS,		MMI_LEBAO_WIN_ID_CHARTS },
	{ RES_MMI_LEBAO_WIN_ID_MUSICLIST,	MMI_LEBAO_WIN_ID_MUSICLIST },
	{ RES_MMI_LEBAO_WIN_ID_PLAYER,		MMI_LEBAO_WIN_ID_PLAYER },
	{ RES_MMI_LEBAO_WIN_ID_VOLUME,		MMI_LEBAO_WIN_ID_VOLUME },
	{ RES_MMI_LEBAO_WIN_ID_SEARCH,		MMI_LEBAO_WIN_ID_SEARCH },
	{ RES_MMI_LEBAO_WIN_ID_ORDER,		MMI_LEBAO_WIN_ID_ORDER },
	{ RES_MMI_LEBAO_WIN_ID_LOCAL,		MMI_LEBAO_WIN_ID_LOCAL },
	{ RES_MMI_LEBAO_WIN_ID_WAITING,		MMI_LEBAO_WIN_ID_WAITING },
	{ RES_MMI_LEBAO_WIN_ID_ABOUT,		MMI_LEBAO_WIN_ID_ABOUT },
	{ RES_MMI_LEBAO_WIN_ID_CONFIRM,		MMI_LEBAO_WIN_ID_CONFIRM },
	{ RES_MMI_LEBAO_WIN_ID_OPTION,		MMI_LEBAO_WIN_ID_OPTION },
	{ RES_MMI_LEBAO_WIN_ID_VIP,			MMI_LEBAO_WIN_ID_VIP },
	{ RES_MMI_LEBAO_WIN_ID_LOGIN,		MMI_LEBAO_WIN_ID_LOGIN },
	{ 0, 0 } // RES_MMI_LEBAO_WIN_ID_MAX
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

// for test
void lebao_create_circle(void)
{
#if defined(_WIN32) && defined(MAINLCD_CIRCULAR_WATCH)
	GUI_RECT_T rect = { 0, 0, LEBAO_SCREEN_WIDTH_MAX - 1, LEBAO_SCREEN_HEIGHT_MAX - 1 };
	GUI_LCD_DEV_INFO lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };

	LCD_DrawCircle(&lcd_dev_info, &rect, LEBAO_SCREEN_WIDTH_MAX / 2, LEBAO_SCREEN_HEIGHT_MAX / 2, LEBAO_SCREEN_WIDTH_MAX / 2, GUI_RGB2RGB565(0x20, 0x20, 0x20));
#endif
}

void lebao_set_round_watch(void)
{
	_isRoundWatch = 1;
}

int	lebao_is_round_watch(void)
{
	return _isRoundWatch;
}

int lebao_is_feature_phone(void)
{
#if defined(LEBAO_FEATURE_PHONE_STYLE)
	return 1;
#else
	return 0;
#endif
}

void lebao_set_label_can_roll(void)
{
	_labelCanRoll = 1;
}

int lebao_get_label_can_roll(void)
{
	return _labelCanRoll;
}

int lebao_get_screen_width(void)
{
//	GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
//	return (rect.right - rect.left);
	return LEBAO_SCREEN_WIDTH_MAX;
}

int lebao_get_screen_height(void)
{
//	GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
//	return (rect.bottom - rect.top);
	return LEBAO_SCREEN_HEIGHT_MAX;
}

BOOL lebao_get_need_highlight_bar(void)
{
#if defined(LEBAO_FEATURE_PHONE_STYLE)
	return TRUE;
#else
	return FALSE;
#endif
}

int lebao_get_title_height(void)
{
	return LEBAO_TITLE_HEIGHT_;
}

int lebao_get_left_padding(void)
{
	return LEBAO_LEFT_PADDING_;
}

int lebao_get_top_padding(void)
{
	return LEBAO_TOP_PADDING_;
}

int lebao_get_right_padding(void)
{
	return LEBAO_RIGHT_PADDING_;
}

int lebao_get_bottom_padding(void)
{
	return LEBAO_BOTTOM_PADDING_;
}

int lebao_get_round_padding(void)
{
	return LEBAO_ROUND_PADDING_;
}

int lebao_get_bg_color(void)
{
#if defined(LEBAO_FEATURE_PHONE_STYLE)
	return MMI_WHITE_COLOR;
#else
	return MMI_BLACK_COLOR;
#endif
}

int lebao_get_font_color(void)
{
#if defined(LEBAO_FEATURE_PHONE_STYLE)
	return MMI_BLACK_COLOR;
#else
	return MMI_WHITE_COLOR;
#endif
}

int lebao_get_big_font(void)
{
#if defined(LEBAO_SIZE_128X128)
	return MMI_DEFAULT_NORMAL_FONT;
#else
	return MMI_DEFAULT_BIG_FONT;
#endif
}

int lebao_get_normal_font(void)
{
	return MMI_DEFAULT_NORMAL_FONT;
}

int lebao_get_small_font(void)
{
	return MMI_DEFAULT_SMALL_FONT;
}

void lebao_set_default_backgroud(const MMI_WIN_ID_T win_id, CAF_COLOR_T color)
{
	GUI_RECT_T bgRect = MMITHEME_GetFullScreenRect();
	GUI_BG_T bgPtr = { 0 };
	GUI_BG_DISPLAY_T bgDisplay = { 0 };

	bgPtr.bg_type = GUI_BG_COLOR;
	bgPtr.color = color;

	bgDisplay.win_handle = MMK_ConvertIdToHandle(win_id);
	bgDisplay.display_rect = bgRect;
	bgDisplay.rect = bgRect;

	GUI_DisplayBg(&bgPtr, &bgDisplay, MMITHEME_GetDefaultLcdDev());

	lebao_create_circle();
}

void lebao_set_title_bar(MMI_CTRL_ID_T labelId, const int textId, const char * utf8Text)
{
	GUI_RECT_T rect = { 0 };

	rect.left   = LEBAO_LEFT_PADDING + LEBAO_ROUND_PADDING * 2;
	rect.top    = LEBAO_TOP_PADDING;
	rect.right  = LEBAO_SCREEN_WIDTH_MAX - LEBAO_RIGHT_PADDING - LEBAO_ROUND_PADDING * 2;
	rect.bottom = LEBAO_TITLE_HEIGHT;

	GUILABEL_SetRect(labelId, &rect, TRUE);

	if (utf8Text != NULL) {
		lebao_set_lable_utf8_font(labelId, LEBAO_NORMAL_FONT, LEBAO_TITLE_COLOR, utf8Text);
        GUILABEL_SetIsAutoScroll(labelId, TRUE);
    }
	else {
		GUILABEL_SetFont(labelId, LEBAO_NORMAL_FONT, LEBAO_TITLE_COLOR);
		GUILABEL_SetTextById(labelId, textId, TRUE);
	}
}

void lebao_set_list_default_rect(MMI_CTRL_ID_T listId, int titleHeight)
{
	GUI_RECT_T rect = { 0 };

	rect.left   = 0;
	rect.top    = titleHeight;
	rect.right  = LEBAO_SCREEN_WIDTH_MAX - 1;
	rect.bottom = LEBAO_SCREEN_HEIGHT_MAX - LEBAO_SOFTKEY_BAR_HEIGHT - 1;

	GUILIST_SetRect(listId, &rect);
	GUILIST_SetListState(listId, GUILIST_STATE_CYC_SCROLL_ITEM, FALSE);
}

void lebao_set_text_default_rect(MMI_CTRL_ID_T textId, int titleHeight)
{
	GUI_RECT_T rect = { 0 };

	rect.left = 0;
	rect.top = titleHeight;
	rect.right = LEBAO_SCREEN_WIDTH_MAX - 1;
	rect.bottom = LEBAO_SCREEN_HEIGHT_MAX - LEBAO_SOFTKEY_BAR_HEIGHT - 1;

	GUITEXT_SetRect(textId, &rect);
}

void lebao_set_list_empty_info(MMI_CTRL_ID_T listId, const int textId)
{
	GUILIST_EMPTY_INFO_T empty_info = { 0 };
	empty_info.text_id = textId;
	GUILIST_SetEmptyInfo(listId, &empty_info);
	GUILIST_SetBgColor(listId, LEBAO_BACKGROUD_COLOR);
}

void lebao_add_list_customize_item(MMI_CTRL_ID_T listId, const int index)
{
	GUILIST_ITEM_T			item_t = { 0 };

	item_t.item_style = GUIITEM_STYLE_CUSTOMIZE;
	item_t.user_data = index;

	GUILIST_AppendItem(listId, &item_t);
}

void lebao_replace_list_customize_item(MMI_CTRL_ID_T listId, const int index)
{
	GUILIST_ITEM_T			item_t = { 0 };

	item_t.item_style = GUIITEM_STYLE_CUSTOMIZE;
	item_t.user_data = index;

	GUILIST_ReplaceItem(listId, &item_t, index);
}

void lebao_create_waitingbox(const int textId, const int ms) 
{
	// MMIPUB_WAITING_PARAM_T param = { 0 };
	// param.wait_id = textId;
	// param.win_id = MMIPUB_WAITING_WIN_ID;
	
	lebao_close_waitingbox();
	// MMIPUB_OpenWaitingWin(&param);
	_lebao_create_msgbox(textId, NULL, ms, 0);
}

void lebao_close_waitingbox(void)
{
	// MMIPUB_CloseWaitWin(MMIPUB_WAITING_WIN_ID);
	// MMIPUB_CloseAlertWin();
	lebao_close_msgbox();
}

void lebao_create_msgbox(const char* title, const int ms)
{
	lebao_close_msgbox();
	_lebao_create_msgbox(0, title, ms, 0);
}

void lebao_create_msgbox_id(const int textId, const int ms)
{
	lebao_close_msgbox();
	_lebao_create_msgbox(textId, NULL, ms, 0);
}

void lebao_create_msgbox_index(const int index, const int ms)
{
	int id = lebao_get_res_text_id(index);
	lebao_close_msgbox();
	if(id != 0)
		_lebao_create_msgbox(id, NULL, ms, 0);
}

void lebao_create_msgbox_utf8(const char* title, const int ms)
{
	lebao_close_msgbox();
	_lebao_create_msgbox(0, title, ms, 1);
}

void lebao_close_msgbox(void)
{
	lebao_waiting_close(NULL);
	// MMIPUB_CloseWaitWin(MMIPUB_WAITING_WIN_ID);
	// MMIPUB_CloseAlertWin();
	MMIPUB_ClearTips();
}

void lebao_add_menu_list_item(MMI_CTRL_ID_T ctrl_id, const int index, MMI_IMAGE_ID_T image_id, MMI_TEXT_ID_T text_id, const uint32 data)
{
	GUILIST_ITEM_T          item_t = { 0 };
	GUILIST_ITEM_DATA_T     item_data = { 0 };
	uint32                  item_index = 0;

	item_t.item_style = GUIITEM_STYLE_CUSTOMIZE;
	item_t.item_data_ptr = &item_data;

	item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
	item_data.item_content[0].item_data.text_id = image_id;

	item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
	item_data.item_content[1].item_data.text_id = text_id;

	item_t.user_data = data;

	GUILIST_AppendItem(ctrl_id, &item_t);
	lebao_set_menu_list_style(ctrl_id, index);
}

void lebao_set_menu_list_style(const MMI_CTRL_ID_T list_id, const int index)
{
	GUILIST_SetItemStyleEx(list_id, index, &s_item_style_1_icon_80x80_text_fixed_height_90);
}

void lebao_set_charts_list_style(const MMI_CTRL_ID_T list_id, const int index)
{
	GUILIST_SetItemStyleEx(list_id, index, &s_item_style_1_icon_80x80_text_fixed_height_90);
}

void lebao_set_charts_list_style_vip(const MMI_CTRL_ID_T list_id, const int index)
{
	GUILIST_SetItemStyleEx(list_id, index, &s_item_style_2_icon_text_fixed_height_90);
}

void lebao_set_musiclist_list_style(const MMI_CTRL_ID_T list_id, const int index)
{
	GUILIST_SetItemStyleEx(list_id, index, &s_item_style_1_icon_20x72_2_text_text_fixed_height_90);
}

void lebao_set_local_list_style(const MMI_CTRL_ID_T list_id, const int index)
{
	GUILIST_SetItemStyleEx(list_id, index, &s_item_style_1_icon_20x72_2_text_text_fixed_height_90);
}

#if defined(LEBAO_FEATURE_PHONE_STYLE)
void lebao_set_artist_lable_utf8(MMI_CTRL_ID_T ctrl_id, const char * text) 
{
	lebao_set_lable_utf8_font(ctrl_id, lebao_get_normal_font(), GUI_RGB2RGB565(0, 0, 0), text);
}

void lebao_set_song_name_lable_utf8(MMI_CTRL_ID_T ctrl_id, const char * text) 
{
//	CTRLLABEL_OBJ_T * songObj = (CTRLLABEL_OBJ_T *)MMK_GetCtrlPtr(ctrl_id);
//	if(songObj != NULL) songObj->align = GUILABEL_ALIGN_MIDDLE;
	lebao_set_lable_utf8_font(ctrl_id, lebao_get_big_font(), GUI_RGB2RGB565(250, 60, 110), text);
}
#else
void lebao_set_artist_lable_utf8(MMI_CTRL_ID_T ctrl_id, const char * text)
{
	lebao_set_lable_utf8_font(ctrl_id, lebao_get_big_font(), GUI_RGB2RGB565(126, 126, 126), text);
}

void lebao_set_song_name_lable_utf8(MMI_CTRL_ID_T ctrl_id, const char * text)
{
	lebao_set_lable_utf8_font(ctrl_id, lebao_get_big_font(), GUI_RGB2RGB565(0xff, 0xa0, 0x00), text);
}
#endif

void lebao_set_lable_font(MMI_CTRL_ID_T ctrl_id, GUI_FONT_T font, GUI_COLOR_T font_color, const char * text) 
{
	_lebao_set_lable_font(ctrl_id, font, font_color, text, 0);
}

void lebao_set_lable_utf8_font(MMI_CTRL_ID_T ctrl_id, GUI_FONT_T font, GUI_COLOR_T font_color, const char * text) 
{
	_lebao_set_lable_font(ctrl_id, font, font_color, text, 1);
}

void lebao_set_lable_font_id(MMI_CTRL_ID_T ctrl_id, GUI_FONT_T font, GUI_COLOR_T font_color, const int id)
{
	GUILABEL_SetFont(ctrl_id, font, font_color);
	GUILABEL_SetTextById(ctrl_id, id, TRUE);
}

void lebao_set_text_utf8_font(MMI_CTRL_ID_T ctrl_id, GUI_FONT_T font, GUI_COLOR_T font_color, const char * text)
{
	_lebao_set_text_font(ctrl_id, font, font_color, text, 0);
}

void lebao_set_text_font(MMI_CTRL_ID_T ctrl_id, GUI_FONT_T font, GUI_COLOR_T font_color, const char * text)
{
	_lebao_set_text_font(ctrl_id, font, font_color, text, 1);
}

void lebao_show_anim_image(MMI_CTRL_ID_T ctrl_id, const char* filename, GUI_RECT_T* img_rect)
{
	_lebao_show_anim_image(ctrl_id, 0, filename, img_rect, TRUE);
}

void lebao_show_anim_image_by_id(MMI_CTRL_ID_T ctrl_id, const int imgId, GUI_RECT_T* img_rect)
{
	_lebao_show_anim_image(ctrl_id, imgId, NULL, img_rect, FALSE);
}

void lebao_show_anim_image_buffer(MMI_CTRL_ID_T ctrl_id, const char* buffer, const int bufferSize, GUI_RECT_T* img_rect)
{
	GUIANIM_CTRL_INFO_T     ctrlInfo = { 0 };
	GUIANIM_DISPLAY_INFO_T  dispInfo = { 0 };

	ctrlInfo.is_ctrl_id = TRUE;
	ctrlInfo.ctrl_id = ctrl_id;

	dispInfo.align_style = GUIANIM_ALIGN_HVMIDDLE;
	dispInfo.is_zoom = TRUE;
	dispInfo.is_auto_zoom_in = FALSE;
	dispInfo.res_bg_color = MMI_WINDOW_BACKGROUND_COLOR; // MMI_WHITE_COLOR;
	dispInfo.specify_width = img_rect->right - img_rect->left;
	dispInfo.specify_height = img_rect->bottom - img_rect->top;
	dispInfo.is_disp_frame = FALSE;

	GUIANIM_SetCtrlRect(ctrl_id, img_rect);
	{
		GUIANIM_DATA_INFO_T		dataInfo = { 0 };

		os_memset(&dataInfo, 0, sizeof(dataInfo));
		dataInfo.data_ptr = buffer;
		dataInfo.data_size = bufferSize;
		
		GUIANIM_SetParam(&ctrlInfo, &dataInfo, NULL, &dispInfo);
	}
}

void lebao_set_res_text_id(const int begin, const int end)
{
	int i = RES_TEXT_LEBAO_WELCOME;
	int start = begin;
	helper_debug_printf("start=%d, end=%d\n", start, end);

	if (start <= 0 || end <= 0 || start > end)
		return;

	do {
		_resTextId[i].resId = start;

		++i;
		++start;
	} while (_resTextId[i].index != 0 && start <= end);
}

void lebao_set_res_image_id(const int begin, const int end)
{
	int i = RES_IMAGE_LEBAO_LOGO;
	int start = begin;
	helper_debug_printf("start=%d, end=%d\n", start, end);

	if (start <= 0 || end <= 0 || start > end)
		return;

	do {
		_resImgId[i].resId = start;

		++i;
		++start;
	} while (_resImgId[i].index != 0 && start <= end);
}

void lebao_set_res_ctrl_id(const int begin, const int end)
{
	int i = RES_MMI_LEBAO_CTRL_ID_MENU_OPTION;
	int start = begin;
	helper_debug_printf("start=%d, end=%d\n", start, end);

	if (start <= 0 || end <= 0 || start > end)
		return;

	do {
		_resCtrlId[i].resId = start;

		++i;
		++start;
	} while (_resCtrlId[i].index != 0 && start <= end);
}

void lebao_set_res_win_id(const int begin, const int end)
{
	int i = RES_MMI_LEBAO_WIN_ID_SPLASH;
	int start = begin;
	helper_debug_printf("start=%d, end=%d\n", start, end);

	if (start <= 0 || end <= 0 || start > end)
		return;

	do {
		_resWinId[i].resId = start;

		++i;
		++start;
	} while (_resWinId[i].index != 0 && start <= end);
}

int lebao_get_res_text_id(const int index)
{
	if (index > RES_TEXT_LEBAO_BEGIN && index < RES_TEXT_LEBAO_END)
		return _resTextId[index].resId;

	return 0;
}

int lebao_get_res_image_id(const int index)
{
	if (index > RES_IMAGE_LEBAO_BEGIN && index < RES_IMAGE_LEBAO_END)
		return _resImgId[index].resId;

	return 0;
}

int lebao_get_res_ctrl_id(const int index)
{
	if (index > RES_MMI_LEBAO_CTRL_ID_START && index < RES_MMI_LEBAO_CTRL_ID_MAX)
		return _resCtrlId[index].resId;

	return 0;
}

int lebao_get_res_win_id(const int index)
{
	if (index > RES_MMI_LEBAO_WIN_ID_START && index < RES_MMI_LEBAO_WIN_ID_MAX)
		return _resWinId[index].resId;

	return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void _lebao_show_anim_image(MMI_CTRL_ID_T ctrl_id, const int imgId, const char* filename, GUI_RECT_T* img_rect, const BOOLEAN isZoom)
{
	GUIANIM_CTRL_INFO_T     ctrlInfo = { 0 };
	GUIANIM_DISPLAY_INFO_T  dispInfo = { 0 };

	ctrlInfo.is_ctrl_id = TRUE;
	ctrlInfo.ctrl_id = ctrl_id;

	dispInfo.align_style = GUIANIM_ALIGN_HVMIDDLE;
	dispInfo.is_zoom = isZoom;
	dispInfo.is_auto_zoom_in = TRUE;
	dispInfo.res_bg_color = MMI_TRANSPARENCE_COLOR; // MMI_WINDOW_BACKGROUND_COLOR; // MMI_WHITE_COLOR;
	dispInfo.specify_width = img_rect->right - img_rect->left;
	dispInfo.specify_height = img_rect->bottom - img_rect->top;
	dispInfo.is_disp_frame = FALSE;
	
	dispInfo.is_bg_buf = TRUE;
	dispInfo.bg.bg_type = GUI_BG_COLOR;
	dispInfo.bg.color = LEBAO_BACKGROUD_COLOR; // MMI_WINDOW_BACKGROUND_COLOR;

	GUIANIM_SetCtrlRect(ctrl_id, img_rect);
	if (filename != NULL) {
		GUIANIM_FILE_INFO_T     fileInfo = { 0 };
		wchar					w_filename[255 + 1] = { 0 };

		fileInfo.full_path_wstr_len = GUI_UTF8ToWstr(w_filename, 255, (const uint8*)filename, os_strlen(filename));
		fileInfo.full_path_wstr_ptr = w_filename;
		GUIANIM_SetParam(&ctrlInfo, NULL, &fileInfo, &dispInfo);
	}
	else {
		GUIANIM_DATA_INFO_T		dataInfo = { 0 };

		os_memset(&dataInfo, 0, sizeof(dataInfo));
		dataInfo.img_id = imgId;
		GUIANIM_SetParam(&ctrlInfo, &dataInfo, NULL, &dispInfo);
	}
}

static int _lebao_convert_to_wstring(const char * text, const int charsetType, wchar*  buffer, const int bufLen)
{
	uint16			w_len = 0;
	uint16			len = os_strlen((text == NULL) ? "" : text);

	if (len <= 0 || buffer == NULL || bufLen <= 0)
		return 0;

	len = (len > bufLen) ? bufLen : len;

	// charsetType: 0 GBk, 1 utf8
	if (charsetType == 1)
		w_len = GUI_UTF8ToWstr(buffer, bufLen, (const uint8*)text, len);
	else
		w_len = GUI_GBToWstr(buffer, (const uint8*)text, len);

	return w_len;
}

static void _lebao_set_lable_font(MMI_CTRL_ID_T ctrl_id, GUI_FONT_T font, GUI_COLOR_T font_color, const char * text, const int charsetType)
{
	MMI_STRING_T	buffer = { 0 };
	wchar			w_text[255 + 1] = { 0 };

	buffer.wstr_ptr = w_text;
	buffer.wstr_len = _lebao_convert_to_wstring(text, charsetType, w_text, 255);

	GUILABEL_SetFont(ctrl_id, font, font_color);
	GUILABEL_SetText(
		ctrl_id, 
		&buffer,
		TRUE);
}

static void _lebao_set_text_font(MMI_CTRL_ID_T ctrl_id, GUI_FONT_T font, GUI_COLOR_T font_color, const char * text, const int charsetType)
{
	MMI_STRING_T	buffer = { 0 };
	wchar			w_text[1023 + 1] = { 0 };

	buffer.wstr_ptr = w_text;
	buffer.wstr_len = _lebao_convert_to_wstring(text, charsetType, w_text, 1023);

	GUITEXT_SetFont(ctrl_id, &font, &font_color);
	GUITEXT_SetString(
		ctrl_id,
		buffer.wstr_ptr,
		buffer.wstr_len,
		TRUE);
}

static void _lebao_create_msgbox(const int textId, const char* title, const int ms, const int charsetType)
{
	if (textId != 0) {
		// MMIPUB_OpenAlertWinByTextId(&ms, textId, NULL, IMAGE_PUBWIN_WAIT, NULL, NULL, MMIPUB_SOFTKEY_NONE, NULL);
		MMIPUB_DisplayTipsIDEx(NULL, textId, MMIPUB_TIPS_DIS_MIDDLE, &ms);
	}
	else {
		MMI_STRING_T	text_buffer = { 0 };
		wchar			w_text[255 + 1] = { 0 };

		text_buffer.wstr_ptr = w_text;
		text_buffer.wstr_len = _lebao_convert_to_wstring(title, charsetType, w_text, 255);
		// MMIPUB_OpenAlertWinByTextPtr(&ms, &text_buffer, NULL, IMAGE_PUBWIN_WAIT, NULL, NULL, MMIPUB_SOFTKEY_NONE, NULL);
		MMIPUB_DisplayTipsPtrEx(NULL, w_text, MMIPUB_TIPS_DIS_MIDDLE, &ms);
	}
}

extern int helper_save_qr_code_bmp(const char* uri, const char* filename);
extern int helper_get_qr_code_bmp(const char* uri, char** buffer, int* size);

void lebao_order_init_control(MMI_WIN_ID_T win_id)
{
	MMI_CTRL_ID_T titleId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_LABEL_TITLE);
	MMI_CTRL_ID_T tipId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_LABEL_SCAN);
	MMI_CTRL_ID_T qrId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_IMG_QRCODE);

	GUI_RECT_T titleRect = { 0 };
	GUI_RECT_T tipRect = { 0 };

	titleRect.left = LEBAO_LEFT_PADDING + LEBAO_ROUND_PADDING;
	titleRect.top = LEBAO_TOP_PADDING + LEBAO_ROUND_PADDING / 2;
	titleRect.right = LEBAO_SCREEN_WIDTH - LEBAO_LEFT_PADDING - LEBAO_RIGHT_PADDING - LEBAO_ROUND_PADDING;
	titleRect.bottom = titleRect.top + LEBAO_TITLE_HEIGHT;

	// title
	GUILABEL_SetRect(titleId, &titleRect, FALSE);
	GUILABEL_SetFont(titleId, LEBAO_NORMAL_FONT, GUI_RGB2RGB565(0xff, 0xa0, 0x00));
	GUILABEL_SetTextById(titleId, LEBAO_ID_TEXT(RES_TEXT_LEBAO_NEED_VIP), TRUE);
    if (LEBAO_SCREEN_WIDTH <= 128)
      GUILABEL_SetVisible(titleId, FALSE, TRUE);

	tipRect.left = LEBAO_LEFT_PADDING;
	tipRect.top = LEBAO_SCREEN_HEIGHT - LEBAO_TITLE_HEIGHT - LEBAO_ROUND_PADDING - 5;
	tipRect.right = LEBAO_SCREEN_WIDTH - LEBAO_LEFT_PADDING - LEBAO_RIGHT_PADDING;
	tipRect.bottom = tipRect.top + LEBAO_TITLE_HEIGHT;

	// tip
	GUILABEL_SetRect(tipId, &tipRect, FALSE);
	GUILABEL_SetFont(tipId, LEBAO_NORMAL_FONT, LEBAO_FONT_COLOR);
	GUILABEL_SetTextById(tipId, LEBAO_ID_TEXT(RES_TEXT_LEBAO_SCAN_QR), TRUE);
	MMK_SetAtvCtrl(win_id, tipId);
}

// supported in simulator
void lebao_order_show_qrcode(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, const int onlyBackground, char** qrCodeBmp, int* bufSize)
{
	MMI_CTRL_ID_T tipId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_LABEL_SCAN);
	GUI_LCD_DEV_INFO lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
	int qrWidth = 0; // qr image, 21x
	GUI_RECT_T imgRect = { 0 };
	GUI_RECT_T bgRect = { 0 };
	GUI_RECT_T tipRect = { 0 };
	char* qrBmp = NULL;
	int qrSize = 0;

	//qrCodeBmpSize == NULL
	if (*qrCodeBmp == NULL) {
		sds uri = lebao_srv_get_order_uri();
		if (uri != NULL) {
			if (helper_get_qr_code_bmp(uri, &qrBmp, &qrSize) != 0) {
				helper_debug_printf("get QR failed\n");
				sdsfree_val(uri);
				return;
			}
			sdsfree_val(uri);
		}

		if (qrBmp == NULL && qrSize <= 0)
			return;

		*qrCodeBmp = qrBmp;
		*bufSize = qrSize;
	}
	else {
		qrBmp = *qrCodeBmp;
		qrSize = *bufSize;
	}

	if (qrBmp != NULL && qrSize > 54) {
		qrWidth = (unsigned int)((qrBmp[0x12] & 0xFF) | ((qrBmp[0x13] << 8) & 0xFF00) | ((qrBmp[0x14] << 16) & 0xFF0000) | ((qrBmp[0x15] << 24) & 0xFF000000));
//		qrheight = (unsigned int)((qrBmp[0x16] & 0xFF) | ((qrBmp[0x17] << 8) & 0xFF00) | ((qrBmp[0x18] << 16) & 0xFF0000) | ((qrBmp[0x19] << 24) & 0xFF000000));
	}

	qrWidth = (qrWidth <= 0) ? ((LEBAO_SCREEN_WIDTH <= 128) ? 84 : 168) : qrWidth;
	qrWidth = qrWidth > 168 ? 168 : qrWidth;

	// 320x170, 480x200
	if (LEBAO_SCREEN_WIDTH > 240 && LEBAO_SCREEN_WIDTH > LEBAO_SCREEN_HEIGHT) {
		qrWidth = 84;
	}
	// 128x128
	else if (LEBAO_SCREEN_WIDTH <= 128)
		qrWidth = 84;

	imgRect.left = (LEBAO_SCREEN_WIDTH - qrWidth) / 2;
#if defined(LEBAO_FEATURE_PHONE_STYLE)
	imgRect.top = LEBAO_TOP_PADDING + LEBAO_TITLE_HEIGHT + 5;
#else
	imgRect.top = (LEBAO_SCREEN_HEIGHT - qrWidth) / 2;
#endif
		// 128x128
	if (LEBAO_SCREEN_WIDTH <= 128)
		imgRect.top = imgRect.top / 2;

	imgRect.right = imgRect.left + qrWidth;
	imgRect.bottom = imgRect.top + qrWidth;

	bgRect.left = imgRect.left - 2;
	bgRect.top = imgRect.top - 2;
	bgRect.right = imgRect.right + 2;
	bgRect.bottom = imgRect.bottom + 2;
	LCD_FillRect(&lcd_dev_info, bgRect, MMI_WHITE_COLOR);

	if (onlyBackground != 0)
		return;

	if (qrBmp != NULL && qrSize > 0) {
		lebao_show_anim_image_buffer(ctrl_id, qrBmp, qrSize, &imgRect);
		//	MMK_SetAtvCtrl(win_id, ctrl_id);
	}

	// adjust the tip position
	CTRLLABEL_GetRect(tipId, &tipRect);
	tipRect.top = bgRect.bottom + LEBAO_TOP_PADDING;
	tipRect.bottom = tipRect.top + LEBAO_TITLE_HEIGHT;
	GUILABEL_SetRect(tipId, &tipRect, FALSE);
}

static GUI_RECT_T _btnPlayRect = { 0 };

void lebao_player_init_control(MMI_WIN_ID_T win_id)
{
	MMI_CTRL_ID_T songNameId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_LABEL_SONG_NAME);
	MMI_CTRL_ID_T singerNameId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_LABEL_SINGER_NAME);
	MMI_CTRL_ID_T prevId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_BTN_PREV);
	MMI_CTRL_ID_T playId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_BTN_PLAY);
	MMI_CTRL_ID_T nextId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_BTN_NEXT);
	MMI_CTRL_ID_T ringId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_BTN_RING);
	MMI_CTRL_ID_T favId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_BTN_FAVORITE);
	MMI_CTRL_ID_T volId = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_BTN_VOLUME);

	// default: 240 x 240
	GUI_RECT_T songRect = { 
		5 + LEBAO_ROUND_PADDING_,
		20, 
		LEBAO_SCREEN_WIDTH_MAX - 5 - LEBAO_ROUND_PADDING_,
		20 + 28 };

	GUI_RECT_T singerRect = { 
		5 + LEBAO_ROUND_PADDING_,
		55, 
		LEBAO_SCREEN_WIDTH_MAX - 5 - LEBAO_ROUND_PADDING_,
		55 + 28 };

	// 36 x 36
	GUI_RECT_T prevRect = { 
		20, 
		LEBAO_SCREEN_HEIGHT_MAX / 2 - 18, 
		20 + 36, 
		LEBAO_SCREEN_HEIGHT_MAX / 2 + 18 };

	// 36 x 36
	GUI_RECT_T nextRect = { 
		LEBAO_SCREEN_WIDTH_MAX - 20 - 36,
		LEBAO_SCREEN_HEIGHT_MAX / 2 - 18, 
		LEBAO_SCREEN_WIDTH_MAX - 20,
		LEBAO_SCREEN_HEIGHT_MAX / 2 + 18 };	

	// 60 x 60
	GUI_RECT_T playRect = { 
		(LEBAO_SCREEN_WIDTH_MAX - 60) / 2, 
		LEBAO_SCREEN_HEIGHT_MAX / 2 - 12, 
		(LEBAO_SCREEN_WIDTH_MAX - 60) / 2 + 60, 
		LEBAO_SCREEN_HEIGHT_MAX / 2 + 48 };	
	
	// height = 3px
	GUI_RECT_T pgRect = { 
		5, 
		LEBAO_SCREEN_HEIGHT_MAX - 36 - 30,
		LEBAO_SCREEN_WIDTH_MAX - 10,
		LEBAO_SCREEN_HEIGHT_MAX - 36 - 30 + 3 };
	
	// 36 x 36
	GUI_RECT_T favRect = { 
		20, 
		LEBAO_SCREEN_HEIGHT_MAX - 36 - 20, 
		20 + 36, 
		LEBAO_SCREEN_HEIGHT_MAX - 36 - 20 + 36 };
	
	// 36 x 36
	GUI_RECT_T volumeRect = { 
		LEBAO_SCREEN_WIDTH_MAX - 20 - 36,
		LEBAO_SCREEN_HEIGHT_MAX - 36 - 20,
		LEBAO_SCREEN_WIDTH_MAX - 20,
		LEBAO_SCREEN_HEIGHT_MAX - 36 - 20 + 36 }; 

	GUI_RECT_T ringRect = favRect;

	// overwrite the rect
#if defined(LEBAO_SIZE_128X128)
	if (LEBAO_SCREEN_HEIGHT <= 128) {
		// 128 x 128
		GUI_RECT_T songRect_ = { 5, 5, 123, 5 + 15 };
		GUI_RECT_T singerRect_ = { 3, 25, 118, 40 };
		GUI_RECT_T prevRect_ = { 10, 65, 10 + 18, 65 + 18 };	// 18 x 18
		GUI_RECT_T nextRect_ = { 95, 65, 95 + 18, 65 + 18 };	// 18 x 18
		GUI_RECT_T playRect_ = { 48, 60, 48 + 30, 60 + 30 };	// 30 x 30
		GUI_RECT_T pgRect_ = { 3, 90, 125, 90 + 3 };

		GUI_RECT_T favRect_ = { 10, 102, 10 + 18, 102 + 18 };	// 18 x 18
		GUI_RECT_T volumeRect_ = { 95, 102, 95 + 18, 102 + 18 };// 18 x 18
		GUI_RECT_T ringRect_ = favRect_;

		songRect = songRect_;
		singerRect = singerRect_;
		prevRect = prevRect_;
		nextRect = nextRect_;
		playRect = playRect_;
		pgRect = pgRect_;
		favRect = favRect_;
		volumeRect = volumeRect_;
		ringRect = ringRect_;
	}
#elif defined(LEBAO_SIZE_240X320)
	if (LEBAO_SCREEN_WIDTH == 240 && LEBAO_SCREEN_HEIGHT == 320) {
		// 240 x 320
		GUI_RECT_T songRect_ = { 2, 1, 238, 40 };
		GUI_RECT_T singerRect_ = { 2, 41, 238, 81 };
		GUI_RECT_T prevRect_ = { 20, 150, 20 + 30, 150 + 30 };	 // 30 x 30
		GUI_RECT_T nextRect_ = { 190, 150, 190 + 30, 150 + 30 }; // 30 x 30
		GUI_RECT_T playRect_ = { 70,  115, 70 + 100, 115 + 100 };// 100 x 100
		GUI_RECT_T pgRect_ = { 10, 230, 230, 230 + 6 };

		GUI_RECT_T favRect_ = { 150 - 14, 242, 150 + 14, 242 + 28 };// 28 x 28
		GUI_RECT_T ringRect_ = { 90 - 14, 242, 90 + 14, 242 + 28 };	// 28 x 28

		songRect = songRect_;
		singerRect = singerRect_;
		prevRect = prevRect_;
		nextRect = nextRect_;
		playRect = playRect_;
		pgRect = pgRect_;
		favRect = favRect_;
		ringRect = ringRect_;
	}
#else
	// overwrite
	if (LEBAO_SCREEN_WIDTH_MAX > 240) {
		prevRect.left += (LEBAO_SCREEN_WIDTH_MAX / 240) * 20;
		prevRect.right = prevRect.left + 36;

		nextRect.left -= (LEBAO_SCREEN_WIDTH_MAX / 240) * 20;
		nextRect.right = nextRect.left + 36;

		favRect.left = prevRect.left;
		favRect.right = prevRect.right;

		volumeRect.left = nextRect.left;
		volumeRect.right = nextRect.right;
	}

	if (LEBAO_SCREEN_HEIGHT_MAX > 240) {
		favRect.top -= (LEBAO_SCREEN_HEIGHT_MAX / 240) * 20;
		favRect.bottom = favRect.top + 36;

		volumeRect.top = favRect.top;
		volumeRect.bottom = favRect.bottom;

		if (lebao_is_round_watch() == 0) {
			favRect.left += (LEBAO_SCREEN_WIDTH_MAX / 240) * 20;
			favRect.right = favRect.left + 36;

			volumeRect.left -= (LEBAO_SCREEN_WIDTH_MAX / 240) * 20;
			volumeRect.right = volumeRect.left + 36;
		}

		ringRect = favRect;
	}

	if (LEBAO_SCREEN_WIDTH_MAX > 240 || LEBAO_SCREEN_HEIGHT_MAX > 240) {
		int playWidth = 60, playHeight = 60;

		if (GUIRES_GetImgWidthHeight(&playWidth, &playHeight, LEBAO_ID_IMG(RES_IMAGE_LEBAO_BTN_PLAY), win_id)) {
			playRect.left = (LEBAO_SCREEN_WIDTH_MAX - playWidth) / 2;
			playRect.top = LEBAO_SCREEN_HEIGHT_MAX / 2 - playHeight / 2;
			playRect.right = (LEBAO_SCREEN_WIDTH_MAX - playWidth) / 2 + playWidth;
			playRect.bottom = LEBAO_SCREEN_HEIGHT_MAX / 2 + playHeight / 2;
		}	
	}

	// 360x360, 454x454, 456x456
	if (LEBAO_SCREEN_WIDTH_MAX >= 360 && LEBAO_SCREEN_HEIGHT_MAX >= 360) {
		songRect.top += 10;
		songRect.bottom += 20;
		singerRect.top += 15;
		singerRect.bottom += 25;
	}

	// 320x170, 480x200
	if(LEBAO_SCREEN_WIDTH_MAX > 240 && LEBAO_SCREEN_WIDTH_MAX > LEBAO_SCREEN_HEIGHT_MAX) {
		songRect.top -= 20;
		songRect.bottom -= 20;
		singerRect.top -= 20;
		singerRect.bottom -= 20;

		songRect.left += 41;
		singerRect.left += 41;
		songRect.right -= 41;
		singerRect.right -= 41;
					
		playRect.top += 25;
		playRect.bottom += 25;
	}

	_btnPlayRect = playRect;

	// 240x240
	#if defined(LEBAO_FEATURE_PHONE_STYLE)
		ringRect = favRect;
		favRect = volumeRect;

		ringRect.left += 50;
		ringRect.right += 50;

		favRect.left -= 50;
		favRect.right -= 50;

		if(LEBAO_SCREEN_HEIGHT_MAX <= 240) {
			playRect.top -= 20;
			playRect.bottom -= 20;
		}
	#endif
#endif

	_btnPlayRect = playRect;

	if (lebao_is_round_watch() != 0) {
		songRect.left += LEBAO_ROUND_PADDING;
		songRect.right -= LEBAO_ROUND_PADDING;

		favRect.left += (LEBAO_ROUND_PADDING * 2);
		favRect.right += (LEBAO_ROUND_PADDING * 2);

		volumeRect.left -= (LEBAO_ROUND_PADDING * 2);
		volumeRect.right -= (LEBAO_ROUND_PADDING * 2);

		// song
		GUILABEL_SetRect(songNameId, &singerRect, FALSE);
		// artist
		GUILABEL_SetRect(singerNameId, &songRect, FALSE);

		ringRect = favRect;
	}
	else {
		// song
		GUILABEL_SetRect(songNameId, &songRect, FALSE);
		// artist
		GUILABEL_SetRect(singerNameId, &singerRect, FALSE);
	}

	// prev
	GUIBUTTON_SetRect(prevId, &prevRect);
	GUIBUTTON_SetRunSheen(prevId, FALSE);
	//	GUIBUTTON_SetPressedBg(prevId, &prevRect_bg);

	// play / pause
	GUIBUTTON_SetRect(playId, &playRect);
	GUIBUTTON_SetRunSheen(playId, FALSE);
	//	GUIBUTTON_SetPressedBg(playId, &playRect_bg);

	// next
	GUIBUTTON_SetRect(nextId, &nextRect);
	GUIBUTTON_SetRunSheen(nextId, FALSE);
	//	GUIBUTTON_SetPressedBg(nextId, &nextRect_bg);

	// ring setting
	GUIBUTTON_SetRect(ringId, &ringRect);
	GUIBUTTON_SetRunSheen(ringId, FALSE);

	// favorite
	GUIBUTTON_SetRect(favId, &favRect);
	GUIBUTTON_SetRunSheen(favId, FALSE);
	

#if !defined(LEBAO_FEATURE_PHONE_STYLE)	
	// volume
	GUIBUTTON_SetRect(volId, &volumeRect);
	GUIBUTTON_SetRunSheen(volId, FALSE);
	//	GUIBUTTON_SetPressedBg(volId, &volume_press_bg);
#endif

	if(lebao_get_label_can_roll() != 0)
		MMK_SetAtvCtrlEx(win_id, songNameId, TRUE);
	else
		MMK_SetAtvCtrlEx(win_id, playId, TRUE);
}


#if defined(LEBAO_FEATURE_PHONE_STYLE)
// phone
static void lebao_player_draw_current_time(void)
{
	GUI_LCD_DEV_INFO lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
	// 240 x 320
	GUI_RECT_T elapsedRect = { 10, 242, 70, 270 };

	char str[64] = { 0 };
	uint32 c_time = lebao_get_elapsed_seconds();

	MMI_CTRL_ID_T curCtrl = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_LABEL_CURRENT_TIME);

	if (LEBAO_SCREEN_HEIGHT == 240) {
		// 240 x 240
		GUI_RECT_T elapsedRect_ = { 10, 185, 70, 221 };
		elapsedRect = elapsedRect_;
	}

	os_sprintf(str, "%d:%02d", c_time / 60, c_time % 60);

	// elapsed seconds
	LCD_FillRect(&lcd_dev_info, elapsedRect, LEBAO_BACKGROUD_COLOR);
	GUILABEL_SetRect(curCtrl, &elapsedRect, FALSE);
	lebao_set_lable_font(curCtrl, LEBAO_SMALL_FONT, LEBAO_FONT_COLOR, str);
}

static void lebao_player_draw_total_time(void)
{
	GUI_LCD_DEV_INFO lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
	// 320 x 240
	GUI_RECT_T durationRect = { 170, 242, 230, 270 };

	char str[64] = { 0 };
	uint32 t_time = lebao_get_duration();

	MMI_CTRL_ID_T tmCtrl = LEBAO_ID_CTRL(RES_MMI_LEBAO_CTRL_ID_LABEL_TOTAL_TIME);

	if (LEBAO_SCREEN_HEIGHT == 240) {
		// 240 x 240, 320 x 240
		GUI_RECT_T durationRect_ = { 170, 185, 230, 221 };
		durationRect_.right = LEBAO_SCREEN_WIDTH - 10;
		durationRect_.left = durationRect_.right - 60;
		durationRect = durationRect_;
	}
	
	os_sprintf(str, "%d:%02d", t_time / 60, t_time % 60);

	// duration
	LCD_FillRect(&lcd_dev_info, durationRect, LEBAO_BACKGROUD_COLOR);
	GUILABEL_SetRect(tmCtrl, &durationRect, FALSE);
	lebao_set_lable_font(tmCtrl, LEBAO_SMALL_FONT, LEBAO_FONT_COLOR, str);
}


void lebao_player_draw_progress(int pg)
{
	GUI_LCD_DEV_INFO lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
	int pos = 0;

	// 240 x 320
	GUI_RECT_T pgRect = { 10, 230, 230, 230 + 6 };

	if (LEBAO_SCREEN_HEIGHT == 240) {
		// 240 x 240, 320 x 240
		GUI_RECT_T pgRect_ = { 5, 180, 235, 180 + 2 };
		pgRect_.left = 5;
		pgRect_.right = LEBAO_SCREEN_WIDTH - 5;
		pgRect = pgRect_;
	}

	pos = (pg * (pgRect.right - pgRect.left)) / 100;

	LCD_FillRect(&lcd_dev_info, pgRect, GUI_RGB2RGB565(126, 126, 126));
	pgRect.right = pgRect.left + pos;
	LCD_FillRect(&lcd_dev_info, pgRect, GUI_RGB2RGB565(250, 60, 110));

	lebao_player_draw_current_time();
	lebao_player_draw_total_time();
}

#else

extern void _draw_play_progress(const GUI_LCD_DEV_INFO* dev_info_ptr, GUI_RECT_T* clip_rect_ptr, int16 x, int16 y, uint16 radius, uint16 pg, GUI_COLOR_T color);

void lebao_player_draw_progress(int pg)
{
	GUI_LCD_DEV_INFO lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };

	if (LEBAO_SCREEN_HEIGHT <= 128) {
		GUI_RECT_T rect = { 0, 0, 127, 127 };
		_draw_play_progress(&lcd_dev_info, &rect, 48 + 15, 60 + 15, 25, pg * 360 / 100, GUI_RGB2RGB565(0xff, 0xa0, 0x00));
	}
	else {
		GUI_RECT_T rect = { 0, 0, LEBAO_SCREEN_WIDTH_MAX - 1, LEBAO_SCREEN_HEIGHT_MAX - 1 };
		int playWidth = 60, playHeight = 60;
		GUI_RECT_T playRect = _btnPlayRect;
		int radius = playWidth / 2 + 15;

		if (LEBAO_SCREEN_WIDTH > 240 && LEBAO_SCREEN_HEIGHT > 240)
			radius += 5;

		_draw_play_progress(&lcd_dev_info, &rect, 
			playRect.left + playWidth / 2, playRect.top + playHeight / 2, radius,
			pg * 360 / 100, GUI_RGB2RGB565(0xff, 0xa0, 0x00));
	}
}

#endif
