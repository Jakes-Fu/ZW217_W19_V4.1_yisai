#ifndef __LEBAO_ID_H
#define __LEBAO_ID_H 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
 
#define WIN_ID_DEF(win_id, win_id_name) win_id,
 
typedef enum
{
    MMI_LEBAO_WIN_ID_START = (MMI_MODULE_LEBAO << 16),

#include "lebao_id.def"

    MMI_LEBAO_WIN_ID_MAX
} MMI_LEBAO_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMI_LEBAO_CTRL_ID_START = MMI_LEBAO_WIN_ID_MAX,

	// menu
	MMI_LEBAO_CTRL_ID_MENU_OPTION,

	// list
	MMI_LEBAO_CTRL_ID_LIST_MENU,
    MMI_LEBAO_CTRL_ID_LIST_CHARTS,
	MMI_LEBAO_CTRL_ID_LIST_MUSICLIST,
	MMI_LEBAO_CTRL_ID_LIST_PLAYER,
	MMI_LEBAO_CTRL_ID_LIST_LOCAL,
	MMI_LEBAO_CTRL_ID_LIST_ABOUT,

	// progress
	MMI_LEBAO_CTRL_ID_PGB_MUSIC,

	// button
	MMI_LEBAO_CTRL_ID_BTN_VOLUME,
	MMI_LEBAO_CTRL_ID_BTN_BACK,
	MMI_LEBAO_CTRL_ID_BTN_PLAY,
	MMI_LEBAO_CTRL_ID_BTN_PAUSE,
	MMI_LEBAO_CTRL_ID_BTN_NEXT,
	MMI_LEBAO_CTRL_ID_BTN_PREV,
	MMI_LEBAO_CTRL_ID_BTN_RING,
	MMI_LEBAO_CTRL_ID_BTN_DEL,
	MMI_LEBAO_CTRL_ID_BTN_VOL_ADD,
	MMI_LEBAO_CTRL_ID_BTN_VOL_SUB,
	MMI_LEBAO_CTRL_ID_BTN_RECORD,
	MMI_LEBAO_CTRL_ID_BTN_FAVORITE,
	MMI_LEBAO_CTRL_ID_BTN_CALL,

	// image
	MMI_LEBAO_CTRL_ID_IMG_QRCODE,
	MMI_LEBAO_CTRL_ID_IMG_RECORD,
	MMI_LEBAO_CTRL_ID_IMG_LOADING,
	MMI_LEBAO_CTRL_ID_IMG_VIP,

	// text
	MMI_LEBAO_CTRL_ID_TEXT_CONFIRM,

	// editbox
	MMI_LEBAO_CTRL_ID_EDIT_MOBILE,
	MMI_LEBAO_CTRL_ID_EDIT_CAPTCHA,
	MMI_LEBAO_CTRL_ID_EDIT_USERNAME,
	MMI_LEBAO_CTRL_ID_EDIT_PASSWORD,
	MMI_LEBAO_CTRL_ID_EDIT_CONTENT,

	// label
	MMI_LEBAO_CTRL_ID_LABEL_WELCOME,
	MMI_LEBAO_CTRL_ID_LABEL_TITLE,
	MMI_LEBAO_CTRL_ID_LABEL_SCAN,
	MMI_LEBAO_CTRL_ID_LABEL_VOLUME,
	MMI_LEBAO_CTRL_ID_LABEL_TIMER,
	MMI_LEBAO_CTRL_ID_LABEL_SONG_NAME,
	MMI_LEBAO_CTRL_ID_LABEL_SINGER_NAME,
	MMI_LEBAO_CTRL_ID_LABEL_TIPS,
	MMI_LEBAO_CTRL_ID_LABEL_SCAN_DESC,
	MMI_LEBAO_CTRL_ID_LABEL_VOLUME_TITLE,
	MMI_LEBAO_CTRL_ID_LABEL_CURRENT_TIME,
	MMI_LEBAO_CTRL_ID_LABEL_TOTAL_TIME,
	MMI_LEBAO_CTRL_ID_LABEL_MOBILE,
	MMI_LEBAO_CTRL_ID_LABEL_CAPTCHA,

	MMI_LEBAO_CTRL_ID_MAX
} MMI_LEBAO_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //__LEBAO_ID_H
