#ifndef LEBAO_WIN_BASE_H
#define LEBAO_WIN_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "std_header.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmk_app.h"
#include "mmicc_export.h"
#include "guitext.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "guiedit.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "guibutton.h"
#include "mmi_default.h"
#include "mmi_common.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmi_appmsg.h"
#include "mmipub.h"

#include "lebao_win_waiting.h"

/*********************
 *      DEFINES	
 *********************/
 // Download the list pictures of chart from the Server, 0 : hide, 1 : show
#define  LEBAO_SHOW_CHART_IMAGE		(1)

// Download the list pictures of music from the Server, 0 : hide, 1 : show
#define  LEBAO_SHOW_SONG_IMAGE		(0)

#if defined(LEBAO_FEATURE_PHONE_STYLE)

	#if defined(LEBAO_HIDE_SOFTKEYBAR)
	#define LEBAO_SOFTKEY_BAR_HEIGHT	(0)
	#else
	#define LEBAO_SOFTKEY_BAR_HEIGHT	(40)
	#endif
	
	#define LEBAO_TITLE_COLOR			GUI_RGB2RGB565(250, 60, 110)
#else
	#define LEBAO_SOFTKEY_BAR_HEIGHT	(0)
	#define LEBAO_TITLE_COLOR			MMI_GRAY_WHITE_COLOR
#endif

#define	LEBAO_SCREEN_WIDTH		lebao_get_screen_width()
#define	LEBAO_SCREEN_HEIGHT		lebao_get_screen_height()

#define LEBAO_NEED_HIGHLIGHT_BAR  lebao_get_need_highlight_bar()


#define LEBAO_TITLE_HEIGHT		lebao_get_title_height()
#define LEBAO_LEFT_PADDING		lebao_get_left_padding()
#define LEBAO_TOP_PADDING		lebao_get_top_padding()
#define LEBAO_RIGHT_PADDING		lebao_get_right_padding()
#define LEBAO_BOTTOM_PADDING	lebao_get_bottom_padding()
#define LEBAO_ROUND_PADDING		lebao_get_round_padding()

#define LEBAO_BACKGROUD_COLOR	lebao_get_bg_color()
#define LEBAO_FONT_COLOR		lebao_get_font_color()
#define LEBAO_BIG_FONT			lebao_get_big_font()
#define LEBAO_NORMAL_FONT		lebao_get_normal_font()
#define LEBAO_SMALL_FONT		lebao_get_small_font()


/**********************
 *      TYPEDEFS
 **********************/
typedef MMI_RESULT_E(*LEBAO_BTN_CALLBACK_FUNC)();

typedef enum {
	SOUND_LEBAO_SEARCH_LIMIT = 0,
	SOUND_LEBAO_MEMBER_OPENED = 1,
	SOUND_LEBAO_RING_CLICK = 5,
	SOUND_LEBAO_SCAN_QR = 6,
	SOUND_LEBAO_PUSH_TALK = 7,
	SOUND_LEBAO_RING_OK = 9,
	SOUND_LEBAO_DEL_FAVORITE = 11,
	SOUND_LEBAO_WELCOME = 13,
	SOUND_LEBAO_WELCOME2 = 14,
	SOUND_LEBAO_WELCOME3 = 15,
} lebao_sound_id_t;

typedef enum
{
	RES_TEXT_LEBAO_BEGIN = 0,
	RES_TEXT_LEBAO_WELCOME,
	RES_TEXT_LEBAO_MAIN_TITLE,
	RES_TEXT_LEBAO_MENU_CHARTS,
	RES_TEXT_LEBAO_MENU_SEARCH,
	RES_TEXT_LEBAO_MENU_LOCAL,
	RES_TEXT_LEBAO_MENU_CLEAR,
	RES_TEXT_LEBAO_MENU_ORDER,
	RES_TEXT_LEBAO_MENU_SETTING,
	RES_TEXT_LEBAO_MENU_ABOUT,
	RES_TEXT_LEBAO_MENU_SERVICE,
	RES_TEXT_LEBAO_MENU_PRIVACY,
	RES_TEXT_LEBAO_MENU_COPYRIGHT,
	RES_TEXT_LEBAO_MENU_FAVORITES,
	RES_TEXT_LEBAO_OPT_OPEN,
	RES_TEXT_LEBAO_OPT_LOGIN,
	RES_TEXT_LEBAO_OPT_INTEREST,
	RES_TEXT_LEBAO_OPT_LOOP,
	RES_TEXT_LEBAO_OPT_CANCEL_LOOP,
	RES_TEXT_LEBAO_OPT_REPEAT,
	RES_TEXT_LEBAO_OPT_SHUFFLE,
	RES_TEXT_LEBAO_OPT_FAVORITE,
	RES_TEXT_LEBAO_OPT_RING,
	RES_TEXT_LEBAO_OPT_FAVORITE_DEL,
	RES_TEXT_LEBAO_OPT_RING_DEL,
	RES_TEXT_LEBAO_OPT_12530,
	RES_TEXT_LEBAO_LIST,
	RES_TEXT_LEBAO_PLAY,
	RES_TEXT_LEBAO_PAUSE,
	RES_TEXT_LEBAO_STOP,
	RES_TEXT_LEBAO_RING,
	RES_TEXT_LEBAO_DEL,
	RES_TEXT_LEBAO_EMPTY,
	RES_TEXT_LEBAO_REMINDER,
	RES_TEXT_LEBAO_INTEREST,
	RES_TEXT_LEBAO_ONEKEY_SMS,
	RES_TEXT_LEBAO_CONFIRM,
	RES_TEXT_LEBAO_AGREE,
	RES_TEXT_LEBAO_SUCCESSED,
	RES_TEXT_LEBAO_FAILED,
	RES_TEXT_LEBAO_NEED_VIP,
	RES_TEXT_LEBAO_TIP_IVR,
	RES_TEXT_LEBAO_LOGIN,
	RES_TEXT_LEBAO_MOBILE,
	RES_TEXT_LEBAO_CAPTCHA,
	RES_TEXT_LEBAO_PASSWORD,
	RES_TEXT_LEBAO_WAITING,
	RES_TEXT_LEBAO_RENEW,
	RES_TEXT_LEBAO_NO_MUSIC,
	RES_TEXT_LEBAO_PLAY_FAILED,
	RES_TEXT_LEBAO_RING_FAILED,
	RES_TEXT_LEBAO_SAY_NAME,
	RES_TEXT_LEBAO_PUSH_TALK,
	RES_TEXT_LEBAO_OVER_TALK,
	RES_TEXT_LEBAO_TIME_SHORT,
	RES_TEXT_LEBAO_SETTING_VOLUME,
	RES_TEXT_LEBAO_SEARCHING,
	RES_TEXT_LEBAO_FOUND_NOTHING,
	RES_TEXT_LEBAO_SCAN_QR,
	RES_TEXT_LEBAO_RING_OK,
	RES_TEXT_LEBAO_MEMORY_FULL,
	RES_TEXT_LEBAO_ERR_DOWNLOAD,
	RES_TEXT_LEBAO_SEARCH_LIMIT,
	RES_TEXT_LEBAO_ADD_FAVORITE,
	RES_TEXT_LEBAO_DEL_FAVORITE,
	RES_TEXT_LEBAO_DOWNLOAD_OK,
	RES_TEXT_LEBAO_NO_SIM_CARD,
	RES_TEXT_LEBAO_NO_MORE,
	RES_TEXT_LEBAO_MUSIC_LIST,
	RES_TEXT_LEBAO_ERR_NETWORK,
	RES_TEXT_LEBAO_ERR_SERVICE,
	RES_TEXT_LEBAO_ERR_UPDATE,
	RES_TEXT_LEBAO_CAN_NOT_FLY_MODE,
	RES_TEXT_LEBAO_OPEN_GPRS,
	RES_TEXT_LEBAO_SERVICE_INFO,
	RES_TEXT_LEBAO_PRIVACY_INFO,
	RES_TEXT_LEBAO_COPYRIGHT_INFO,
	RES_TEXT_LEBAO_PLAYER_NOTIRY,
	RES_TEXT_LEBAO_PUSH_TALK_PHONE,
	RES_TEXT_LEBAO_TIPS_LOADING,
	RES_TEXT_LEBAO_TIPS_VIP_ALREADY,
	RES_TEXT_LEBAO_TIPS_CLEAN,
	RES_TEXT_LEBAO_TIPS_BUSY,
	RES_TEXT_LEBAO_TIPS_VERIFY,
	RES_TEXT_LEBAO_TIPS_NEED_CMCC,
	RES_TEXT_LEBAO_TIPS_QUERYING,
	RES_TEXT_LEBAO_TIPS_OPENING,
	RES_TEXT_LEBAO_TIPS_SUBMIT,
	RES_TEXT_LEBAO_TIPS_FAILED,
	RES_TEXT_LEBAO_TIPS_TRY_IVR,
	RES_TEXT_LEBAO_TIPS_LOGIN,
	RES_TEXT_LEBAO_TIPS_LOGIN_OK,
	RES_TEXT_LEBAO_TIPS_LOGIN_FAILED,
	RES_TEXT_LEBAO_TIPS_TIMEOUT,
	RES_TEXT_LEBAO_TIPS_TRY_QR,
	RES_TEXT_LEBAO_TIPS_ORDER_OK,
	RES_TEXT_LEBAO_TIPS_VIP_TYPE,
	RES_TEXT_LEBAO_TIPS_THANK,
	RES_TEXT_LEBAO_END
} lebao_text_id_t;

typedef enum
{
	RES_IMAGE_LEBAO_BEGIN = 0,
	RES_IMAGE_LEBAO_LOGO,
	RES_IMAGE_LEBAO_SPLASH,
	RES_IMAGE_LEBAO_MENU_CHATS,
	RES_IMAGE_LEBAO_MENU_SEARCH,
	RES_IMAGE_LEBAO_MENU_LOCAL,
	RES_IMAGE_LEBAO_MENU_CLEAR,
	RES_IMAGE_LEBAO_MENU_ORDER,
	RES_IMAGE_LEBAO_MENU_SETTING,
	RES_IMAGE_LEBAO_BTN_BACK,
	RES_IMAGE_LEBAO_BTN_CLOSE,
	RES_IMAGE_LEBAO_BTN_VOLUME,
	RES_IMAGE_LEBAO_BTN_DELETE,
	RES_IMAGE_LEBAO_BTN_FAVORITE,
	RES_IMAGE_LEBAO_BTN_FAVORITE_SEL,
	RES_IMAGE_LEBAO_BTN_NEXT,
	RES_IMAGE_LEBAO_BTN_PREVIOU,
	RES_IMAGE_LEBAO_BTN_RING,
	RES_IMAGE_LEBAO_BTN_RING_SEL,
	RES_IMAGE_LEBAO_BTN_PLAY,
	RES_IMAGE_LEBAO_BTN_PAUSE,
	RES_IMAGE_LEBAO_BTN_VOL_ADD,
	RES_IMAGE_LEBAO_BTN_VOL_SUB,
	RES_IMAGE_LEBAO_ITEM_D1,
	RES_IMAGE_LEBAO_ITEM_D2,
	RES_IMAGE_LEBAO_ITEM_D3,
	RES_IMAGE_LEBAO_BTN_DEFAULT,
	RES_IMAGE_LEBAO_RECORD,
	RES_IMAGE_LEBAO_RECORD_GRAY,
	RES_IMAGE_LEBAO_BTN_STAR,
	RES_IMAGE_LEBAO_LINE,
	RES_IMAGE_LEBAO_VIP,
	RES_IMAGE_LEBAO_INDICATOR,
	RES_IMAGE_LEBAO_LOADING,
#if defined(LEBAO_FEATURE_PHONE_STYLE)
	RES_IMAGE_LEBAO_MENU_ABOUT,
	RES_IMAGE_LEBAO_VOLUME_1,
	RES_IMAGE_LEBAO_VOLUME_2,
	RES_IMAGE_LEBAO_VOLUME_3,
	RES_IMAGE_LEBAO_VOLUME_4,
	RES_IMAGE_LEBAO_VOLUME_5,
#endif
	RES_IMAGE_LEBAO_END
} lebao_image_id_t;

typedef enum
{
	RES_MMI_LEBAO_CTRL_ID_START = 0,

	// menu
	RES_MMI_LEBAO_CTRL_ID_MENU_OPTION,

	// list
	RES_MMI_LEBAO_CTRL_ID_LIST_MENU,
	RES_MMI_LEBAO_CTRL_ID_LIST_CHARTS,
	RES_MMI_LEBAO_CTRL_ID_LIST_MUSICLIST,
	RES_MMI_LEBAO_CTRL_ID_LIST_PLAYER,
	RES_MMI_LEBAO_CTRL_ID_LIST_LOCAL,
	RES_MMI_LEBAO_CTRL_ID_LIST_ABOUT,

	// progress
	RES_MMI_LEBAO_CTRL_ID_PGB_MUSIC,

	// button
	RES_MMI_LEBAO_CTRL_ID_BTN_VOLUME,
	RES_MMI_LEBAO_CTRL_ID_BTN_BACK,
	RES_MMI_LEBAO_CTRL_ID_BTN_PLAY,
	RES_MMI_LEBAO_CTRL_ID_BTN_PAUSE,
	RES_MMI_LEBAO_CTRL_ID_BTN_NEXT,
	RES_MMI_LEBAO_CTRL_ID_BTN_PREV,
	RES_MMI_LEBAO_CTRL_ID_BTN_RING,
	RES_MMI_LEBAO_CTRL_ID_BTN_DEL,
	RES_MMI_LEBAO_CTRL_ID_BTN_VOL_ADD,
	RES_MMI_LEBAO_CTRL_ID_BTN_VOL_SUB,
	RES_MMI_LEBAO_CTRL_ID_BTN_RECORD,
	RES_MMI_LEBAO_CTRL_ID_BTN_FAVORITE,
	RES_MMI_LEBAO_CTRL_ID_BTN_CALL,

	// image
	RES_MMI_LEBAO_CTRL_ID_IMG_QRCODE,
	RES_MMI_LEBAO_CTRL_ID_IMG_RECORD,
	RES_MMI_LEBAO_CTRL_ID_IMG_LOADING,
	RES_MMI_LEBAO_CTRL_ID_IMG_VIP,

	// text
	RES_MMI_LEBAO_CTRL_ID_TEXT_CONFIRM,

	// editbox
	RES_MMI_LEBAO_CTRL_ID_EDIT_MOBILE,
	RES_MMI_LEBAO_CTRL_ID_EDIT_CAPTCHA,
	RES_MMI_LEBAO_CTRL_ID_EDIT_USERNAME,
	RES_MMI_LEBAO_CTRL_ID_EDIT_PASSWORD,
	RES_MMI_LEBAO_CTRL_ID_EDIT_CONTENT,

	// label
	RES_MMI_LEBAO_CTRL_ID_LABEL_WELCOME,
	RES_MMI_LEBAO_CTRL_ID_LABEL_TITLE,
	RES_MMI_LEBAO_CTRL_ID_LABEL_SCAN,
	RES_MMI_LEBAO_CTRL_ID_LABEL_VOLUME,
	RES_MMI_LEBAO_CTRL_ID_LABEL_TIMER,
	RES_MMI_LEBAO_CTRL_ID_LABEL_SONG_NAME,
	RES_MMI_LEBAO_CTRL_ID_LABEL_SINGER_NAME,
	RES_MMI_LEBAO_CTRL_ID_LABEL_TIPS,
	RES_MMI_LEBAO_CTRL_ID_LABEL_SCAN_DESC,
	RES_MMI_LEBAO_CTRL_ID_LABEL_VOLUME_TITLE,
	RES_MMI_LEBAO_CTRL_ID_LABEL_CURRENT_TIME,
	RES_MMI_LEBAO_CTRL_ID_LABEL_TOTAL_TIME,
	RES_MMI_LEBAO_CTRL_ID_LABEL_MOBILE,
	RES_MMI_LEBAO_CTRL_ID_LABEL_CAPTCHA,

	RES_MMI_LEBAO_CTRL_ID_MAX
} lebao_ctrl_id_t;

typedef enum
{
	RES_MMI_LEBAO_WIN_ID_START = 0,

	RES_MMI_LEBAO_WIN_ID_SPLASH,
	RES_MMI_LEBAO_WIN_ID_MAINMENU,
	RES_MMI_LEBAO_WIN_ID_CHARTS,
	RES_MMI_LEBAO_WIN_ID_MUSICLIST,
	RES_MMI_LEBAO_WIN_ID_PLAYER,
	RES_MMI_LEBAO_WIN_ID_VOLUME,
	RES_MMI_LEBAO_WIN_ID_SEARCH,
	RES_MMI_LEBAO_WIN_ID_ORDER,
	RES_MMI_LEBAO_WIN_ID_LOCAL,
	RES_MMI_LEBAO_WIN_ID_WAITING,
	RES_MMI_LEBAO_WIN_ID_ABOUT,
	RES_MMI_LEBAO_WIN_ID_CONFIRM,
	RES_MMI_LEBAO_WIN_ID_OPTION,
	RES_MMI_LEBAO_WIN_ID_VIP,
	RES_MMI_LEBAO_WIN_ID_LOGIN,

	RES_MMI_LEBAO_WIN_ID_MAX
} lebao_win_id_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
// test
void lebao_create_circle(void);

// config
void lebao_set_round_watch(void);
int	 lebao_is_round_watch(void);
int	 lebao_is_feature_phone(void);
void lebao_set_label_can_roll(void);
int  lebao_get_label_can_roll(void);

// theme
int	 lebao_get_screen_width(void);
int	 lebao_get_screen_height(void);

int	 lebao_get_title_height(void);
int	 lebao_get_left_padding(void);
int	 lebao_get_top_padding(void);
int	 lebao_get_right_padding(void);
int	 lebao_get_bottom_padding(void);
int	 lebao_get_round_padding(void);

int	 lebao_get_bg_color(void);
int  lebao_get_font_color(void);
int  lebao_get_big_font(void);
int  lebao_get_normal_font(void);
int  lebao_get_small_font(void);

// external relations
void lebao_set_res_text_id(const int begin, const int end);
void lebao_set_res_image_id(const int begin, const int end);
void lebao_set_res_ctrl_id(const int begin, const int end);
void lebao_set_res_win_id(const int begin, const int end);
int  lebao_get_res_text_id(const int index);
int  lebao_get_res_image_id(const int index);
int  lebao_get_res_ctrl_id(const int index);
int  lebao_get_res_win_id(const int index);

void lebao_create_waitingbox(const int textId, const int ms);
void lebao_close_waitingbox(void);

void lebao_create_msgbox(const char* title, const int ms);
void lebao_create_msgbox_id(const int textId, const int ms);
void lebao_create_msgbox_index(const int index, const int ms);
void lebao_create_msgbox_utf8(const char* title, const int ms);
void lebao_close_msgbox();

void lebao_set_default_backgroud(const MMI_WIN_ID_T win_id, CAF_COLOR_T color);
void lebao_set_title_bar(MMI_CTRL_ID_T labelId, const int textId, const char * utf8Text);

void lebao_set_list_default_rect(MMI_CTRL_ID_T listId, int titleHeight);
void lebao_set_text_default_rect(MMI_CTRL_ID_T textId, int titleHeight);
void lebao_set_list_empty_info(MMI_CTRL_ID_T listId, const int textId);
void lebao_add_list_customize_item(MMI_CTRL_ID_T listId, const int index);
void lebao_replace_list_customize_item(MMI_CTRL_ID_T listId, const int index);

void lebao_add_menu_list_item(MMI_CTRL_ID_T ctrl_id, const int index, MMI_IMAGE_ID_T image_id, MMI_TEXT_ID_T text_id, const uint32 data);
void lebao_set_menu_list_style(const MMI_CTRL_ID_T list_id, const int index);
void lebao_set_charts_list_style(const MMI_CTRL_ID_T list_id, const int index);
void lebao_set_charts_list_style_vip(const MMI_CTRL_ID_T list_id, const int index);

void lebao_set_local_list_style(const MMI_CTRL_ID_T list_id, const int index);

void lebao_set_lable_utf8_font(MMI_CTRL_ID_T ctrl_id, GUI_FONT_T font, GUI_COLOR_T font_color, const char * text);
void lebao_set_lable_font(MMI_CTRL_ID_T ctrl_id, GUI_FONT_T font, GUI_COLOR_T font_color, const char * text);
void lebao_set_lable_font_id(MMI_CTRL_ID_T ctrl_id, GUI_FONT_T font, GUI_COLOR_T font_color, const int id);

void lebao_set_text_utf8_font(MMI_CTRL_ID_T ctrl_id, GUI_FONT_T font, GUI_COLOR_T font_color, const char * text);
void lebao_set_text_font(MMI_CTRL_ID_T ctrl_id, GUI_FONT_T font, GUI_COLOR_T font_color, const char * text);

void lebao_set_artist_lable_utf8(MMI_CTRL_ID_T ctrl_id, const char * text);
void lebao_set_song_name_lable_utf8(MMI_CTRL_ID_T ctrl_id, const char * text);

void lebao_show_anim_image(MMI_CTRL_ID_T ctrl_id, const char* filename, GUI_RECT_T* img_rect);
void lebao_show_anim_image_by_id(MMI_CTRL_ID_T ctrl_id, const int index, GUI_RECT_T* img_rect);
void lebao_show_anim_image_buffer(MMI_CTRL_ID_T ctrl_id, const char* buffer, const int bufferSize, GUI_RECT_T* img_rect);

void lebao_order_init_control(MMI_WIN_ID_T win_id);
void lebao_order_show_qrcode(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, const int onlyBackground, char** qrCodeBmp, int* bufSize);
void lebao_player_init_control(MMI_WIN_ID_T win_id);
void lebao_player_draw_progress(int pg);

/**********************
 *      MACROS
 **********************/

#define LEBAO_ID_TEXT(index)	lebao_get_res_text_id(index)
#define LEBAO_ID_IMG(index)		lebao_get_res_image_id(index)
#define LEBAO_ID_CTRL(index)	lebao_get_res_ctrl_id(index)
#define LEBAO_ID_WIN(index)		lebao_get_res_win_id(index)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LEBAO_WIN_BASE_H*/
