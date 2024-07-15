/*****************************************************************************
** File Name:      mmitheme_list.c                                           *
** Author:         Great.Tian                                                *
** Date:           03/9/2011                                                 *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe the mmi theme               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2011        Xiaoqing.lu      Create                                    *
**                                                                           *
*****************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_theme_trc.h"
#include "std_header.h"
#include "mmitheme_list.h"
#include "mmi_theme.h"
#include "mmi_common.h"
#include "guilcd.h"
#include "mmi_image.h"
#include "mmi_anim.h"
#include "mmidisplay_data.h"
#include "guisoftkey.h"
#include "mmi_menutable.h"
#include "guilistbox.h"
#include "mmi_text.h"
#include "mmi_id.h"
#include "mmi_default.h"
#include "mmi_textfun.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmitv_out.h"
#ifndef WIN32
#include "arm_reg.h"
#endif
#include "guibutton.h"
#include "guiref_scale.h"
#include "guifont.h"
#include "mmiidle_export.h"
#include "mmk_app.h"
#include "mmiset_id.h"
#include "mmifmm_id.h"
#include "mmifmm_text.h"
#include "mmitheme_special_func.h"
#include "block_mem.h"
//#include "mmiset_display.h"
#include "guires.h"
#include "mmi_nv.h"
#ifdef UI_P3D_SUPPORT
#include "mmi3d_export.h"
#endif
#include "mmi_custom_define.h"
#include "mmi_position.h"
#include "guistring.h"
#include "ui_layer.h"
#include "mmi_color.h"
#include "mmi_resource.h"
#include "tp_id.h"
#include "mmi_modu_main.h"
#include "mmisms_image.h"
#include "mmicc_image.h"
#include "mmi_position_listitem.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
#define LIST_NUMBER_MAX_DIGIT    4     // 目前在ListBox的Index的显示当中，最大支持4位的
#define LIST_HIGHLIGHT_PERCENT_LUCENCY 128 // LIST高亮条的透明度
#define ASCII_CODE_0             0x30  // ascii code 0
#define ASCII_CODE_ZHE           0x2F  // ascii code /
#define LISTBOX_STEP_NUM_HOR     5     // 定义horizontal方向左右滑动时的次数,用于左右滑动时
#define LISTBOX_STEP_NUM_VERT    3     // 定义vertical方向上下滑动的次数, 用于item和text上下滑动

#define LISTBOX_KEY_TIME                        300   // 连续移动的按键Timer值

#define MMITHEME_LIST_STYLE_TEXT_TIME      500 //ms

#define MMITHEME_LIST_STYLE_MOVE_TIME       50//ms

// 所有的按像素滚动的像素数
#define MMI_SMALL_LIST_ITEM_HEIGHT    DP2PX_VALUE(22)

#define MMI_LIST_ITEM_SPLIT_BG_COLOR  MMI_DARK_GRAY_COLOR/*0x841*/

#define MMI_LIST_TIP_HIDE_TICK 2000
#define MMI_LIST_TIP_FONT_COLOR 0xef7d


#ifdef _SW_ZDT_PRODUCT_
#define WATCHLIST_ICON_TEXT_HALE_LEFT 120
#define WATCHLIST_ICON_TEXT_LEFT_SPACE 13
#define WATCHLIST_BG_ICON_TOP_SPACE 12
#define WATCHLIST_TEXT_WIDGH_MID 155
#define WATCHLIST_BG_ICON_BOTTOM_SPACE 64
#define WATCHLIST_ICON_TO_TEXT_SPACE 12
#define WATCHLIST_ICON_TEXT_TOP_SPACE 15
#define WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE 60//48
#define WATCHLIST_ICON_TEXT_RIGHT_SPACE 13
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
LOCAL   MMITHEME_LIST_ITEMSTYLE_NODE_PTR  s_item_customstyle_ptr = PNULL;
LOCAL   uint32  s_item_style_id_gen = 0;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#if 0
/*****************************************************************************/
//  Description : convert listnum to string, return convert digit
//  Global resource dependence :
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL uint8 ConvertListNumToStr(
    uint8 *str_ptr, //[OUT] string buffer pointer
    uint16 number   //[IN]  list number
);
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



///////////////////////BEGIN///////////////////////

//|---------------------------|
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_split =
{
    MMI_LIST_SPLIT_ITEM_HEIGHT, MMI_LIST_SPLIT_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE / 2,   0,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_SPLIT_ITEM_HEIGHT - 1 },
            { MMI_LIST_TEXT_LEFT_SPACE / 2,   0,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_SPLIT_ITEM_HEIGHT - 1 },
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|        text          @    |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_icon =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_ITEM_HEIGHT - 1 },
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_EXPAND_ITEM_HEIGHT - 1 },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_ITEM_HEIGHT - 2},
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2},
            0, 0,
            100, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};


#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|        text          @    |
//|---------------------------|
//GUIITEM_STYLE_HALF_LINE_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_icon_2 =
{
    MMI_LIST_ITEM_HEIGHT/2 , MMI_LIST_ITEM_HEIGHT/2,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE), (MMI_LIST_ITEM_HEIGHT - 1) / 2},
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE), (MMI_LIST_ITEM_HEIGHT - 1) / 2},
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1,  MMI_LIST_ITEM_RIGHT, (MMI_LIST_ITEM_HEIGHT - 2) / 2 },
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1,  MMI_LIST_ITEM_RIGHT, (MMI_LIST_ITEM_HEIGHT - 2) / 2 },
            0, 0,
            100, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

#endif

//|---------------------------|
//|     @   text          @   |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_icon =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2 },
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + 2*MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_ITEM_HEIGHT - 1 },
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + 2*MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_EXPAND_ITEM_HEIGHT - 1 },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE), 1,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_ITEM_HEIGHT - 1 },
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE), 1,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_EXPAND_ITEM_HEIGHT - 1 },
            0, 0,
            100, 100
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|-----------------------------------------------------------------------------|
//|                            @   text          @                              |
//|这种风格与GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON唯一不同就在于图片1不可以check|
//|-----------------------------------------------------------------------------|
// GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON_EXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_icon_ext =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2 },
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_ITEM_HEIGHT - 1 },
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_EXPAND_ITEM_HEIGHT - 1 },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_ITEM_HEIGHT - 2 },
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2 },
            0, 0,
            100, 100
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
#endif

/* 以下都为单行item, check为多选, radio为单选, number会在item前增加数字 */

//|---------------------------|
//|        text               |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_TEXT,
#if defined MAINLCD_SIZE_128X160 || defined MAINLCD_SIZE_176X220
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#else
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_EXPAND_ITEM_HEIGHT -1 },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif

#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|        text               |
//|---------------------------|//NEWMS00185675 非GUIF_LIST_PDASTYLE风格下高亮，字体大小不变
// GUIITEM_STYLE_ONE_LINE_TEXT_DEFAULT_FONT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_2 =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_EXPAND_ITEM_HEIGHT - 1},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif


//|---------------------------|
//|  @     text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2}, // normal
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE ,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2}, // focus
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE * 2, MMI_LIST_ITEM_HEIGHT - 1 },
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE * 2, MMI_LIST_EXPAND_ITEM_HEIGHT - 1 },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 85
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

#ifdef XYSDK_SUPPORT

#if 1
#define XYSDK_LIST_H			66
#define XYSDK_ALBUM_IMG_W		42
#define XYSDK_ALBUM_IMG_H		42
#define XYSDK_IMG_START_X		30
#define XYSDK_LIST_SPACE		4
#define XYSDK_LIST_SPACE_H		((XYSDK_LIST_H-XYSDK_ALBUM_IMG_H)/2)
#define XYSDK_TXT_START_X		(XYSDK_IMG_START_X+XYSDK_ALBUM_IMG_W+XYSDK_LIST_SPACE)
#define XYSDK_LIST_END			236
#define XYSDK_TAG_H				16
#define XYSDK_TAG_W				10
#define XYSDK_TAG_SPACE_H		((XYSDK_LIST_H-XYSDK_TAG_H)/2)
#define XYSDK_TAG_END_X			210
#define XYSDK_LIST_FONT			(SONG_FONT_18)
#define XYSDK_LIST_FONT_H		(30)
#define XYSDK_TXT_SPACE_H		((XYSDK_LIST_H-XYSDK_LIST_FONT_H)/2)
#define XYSDK_TXT_END_X			(XYSDK_TAG_END_X-XYSDK_TAG_W-XYSDK_LIST_SPACE*2)


#define XYSDK_ICON_START_X		22
#define XYSDK_ALBUM_ICON_W		18
#define XYSDK_ALBUM_ICON_H		18
#define XYSDK_TAG_L_H			26
#define XYSDK_TAG_L_W			26
#define XYSDK_TAG_L_END_X			222
#define XYSDK_LIST_SPACE_2_H		((XYSDK_LIST_H-XYSDK_ALBUM_ICON_H)/2) - 6
#define XYSDK_TXT_START_2_X		(XYSDK_ICON_START_X+XYSDK_ALBUM_ICON_W+XYSDK_LIST_SPACE)
#define XYSDK_TAG_L_SPACE_H		((XYSDK_LIST_H-XYSDK_TAG_L_H)/2)
#define XYSDK_TXT_L_END_X			(XYSDK_TAG_L_END_X-XYSDK_TAG_L_W-XYSDK_LIST_SPACE*2)


#define XYSDK_NUM_ICON_W			39
#define XYSDK_NUM_START_X			(XYSDK_ICON_START_X+XYSDK_NUM_ICON_W+XYSDK_LIST_SPACE)
#define XYSDK_LIST_NUM_FONT			(SONG_FONT_14)

#else

#define XYSDK_LIST_H			35
#define XYSDK_ALBUM_IMG_W		22
#define XYSDK_ALBUM_IMG_H		22
#define XYSDK_IMG_START_X		16
#define XYSDK_LIST_SPACE		2
#define XYSDK_LIST_SPACE_H		((XYSDK_LIST_H-XYSDK_ALBUM_IMG_H)/2)
#define XYSDK_TXT_START_X		(XYSDK_IMG_START_X+XYSDK_ALBUM_IMG_W+XYSDK_LIST_SPACE)
#define XYSDK_LIST_END			125
#define XYSDK_TAG_H				8
#define XYSDK_TAG_W				5
#define XYSDK_TAG_SPACE_H		((XYSDK_LIST_H-XYSDK_TAG_H)/2)
#define XYSDK_TAG_END_X			111
#define XYSDK_LIST_FONT			(SONG_FONT_14)
#define XYSDK_LIST_FONT_H		(14)
#define XYSDK_TXT_SPACE_H		((XYSDK_LIST_H-XYSDK_LIST_FONT_H)/2)
#define XYSDK_TXT_END_X			(XYSDK_TAG_END_X-XYSDK_TAG_W-XYSDK_LIST_SPACE*2)


#define XYSDK_ICON_START_X		12
#define XYSDK_ALBUM_ICON_W		10
#define XYSDK_ALBUM_ICON_H		10
#define XYSDK_TAG_L_H			14
#define XYSDK_TAG_L_W			14
#define XYSDK_TAG_L_END_X			118
#define XYSDK_LIST_SPACE_2_H		((XYSDK_LIST_H-XYSDK_ALBUM_ICON_H)/2) - 3
#define XYSDK_TXT_START_2_X		(XYSDK_ICON_START_X + XYSDK_ALBUM_ICON_W + XYSDK_LIST_SPACE)
#define XYSDK_TAG_L_SPACE_H		((XYSDK_LIST_H-XYSDK_TAG_L_H)/2)
#define XYSDK_TXT_L_END_X			(XYSDK_TAG_L_END_X-XYSDK_TAG_L_W-XYSDK_LIST_SPACE*2)


#define XYSDK_NUM_ICON_W			30
#define XYSDK_NUM_START_X			(XYSDK_ICON_START_X+XYSDK_NUM_ICON_W+XYSDK_LIST_SPACE)
#define XYSDK_LIST_NUM_FONT			(SONG_FONT_8)

#endif

//|---------------------------|
//|     @   text          @   |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON_XMLY,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_icon_xmly =
{
    XYSDK_LIST_H, XYSDK_LIST_H,

    {
    	{
            0,
            { 0, 0,  XYSDK_LIST_END,  XYSDK_LIST_H}, // normal
            { 0, 0,  XYSDK_LIST_END ,  XYSDK_LIST_H}, // focus
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { XYSDK_IMG_START_X+1, XYSDK_LIST_SPACE_H+1,  XYSDK_ALBUM_IMG_W + XYSDK_IMG_START_X,  XYSDK_LIST_H - XYSDK_LIST_SPACE_H}, // normal
            { XYSDK_IMG_START_X+1, XYSDK_LIST_SPACE_H+1,  XYSDK_ALBUM_IMG_W + XYSDK_IMG_START_X ,  XYSDK_LIST_H - XYSDK_LIST_SPACE_H}, // focus
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { XYSDK_TXT_START_X,  XYSDK_TXT_SPACE_H,  XYSDK_TXT_END_X, XYSDK_LIST_H - XYSDK_TXT_SPACE_H },
            { XYSDK_TXT_START_X,  XYSDK_TXT_SPACE_H,  XYSDK_TXT_END_X, XYSDK_LIST_H - XYSDK_TXT_SPACE_H},
            XYSDK_LIST_FONT, XYSDK_LIST_FONT,
            0, 100
        },
        {
            0,
            { XYSDK_TAG_END_X-XYSDK_TAG_W, XYSDK_TAG_SPACE_H,  XYSDK_TAG_END_X,  XYSDK_LIST_H - XYSDK_TAG_SPACE_H}, // normal
            { XYSDK_TAG_END_X-XYSDK_TAG_W, XYSDK_TAG_SPACE_H,  XYSDK_TAG_END_X ,  XYSDK_LIST_H - XYSDK_TAG_SPACE_H}, // focus
            0, 0,
            0, 0
        },
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};

LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_icon_xmly2 =
{
    XYSDK_LIST_H, XYSDK_LIST_H,

    {
    	{
            0,
            { 0, 0,  XYSDK_LIST_END,  XYSDK_LIST_H}, // normal
            { 0, 0,  XYSDK_LIST_END ,  XYSDK_LIST_H}, // focus
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_NONE,
            { XYSDK_IMG_START_X+1, XYSDK_LIST_SPACE_H+1,  XYSDK_ALBUM_IMG_W + XYSDK_IMG_START_X,  XYSDK_LIST_H - XYSDK_LIST_SPACE_H}, // normal
            { XYSDK_IMG_START_X+1, XYSDK_LIST_SPACE_H+1,  XYSDK_ALBUM_IMG_W + XYSDK_IMG_START_X ,  XYSDK_LIST_H - XYSDK_LIST_SPACE_H}, // focus
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_NONE,
            { XYSDK_TXT_START_X,  XYSDK_TXT_SPACE_H,  XYSDK_TXT_END_X, XYSDK_LIST_H - XYSDK_TXT_SPACE_H },
            { XYSDK_TXT_START_X,  XYSDK_TXT_SPACE_H,  XYSDK_TXT_END_X, XYSDK_LIST_H - XYSDK_TXT_SPACE_H},
            XYSDK_LIST_FONT, XYSDK_LIST_FONT,
            0, 100
        },
        {
            0,
            { XYSDK_TAG_END_X-XYSDK_TAG_W, XYSDK_TAG_SPACE_H,  XYSDK_TAG_END_X,  XYSDK_LIST_H - XYSDK_TAG_SPACE_H}, // normal
            { XYSDK_TAG_END_X-XYSDK_TAG_W, XYSDK_TAG_SPACE_H,  XYSDK_TAG_END_X ,  XYSDK_LIST_H - XYSDK_TAG_SPACE_H}, // focus
            0, 0,
            0, 0
        },
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|        text     text  @   |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_TEXT_ICON_XMLY,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_icon_xmly =
{
	XYSDK_LIST_H, XYSDK_LIST_H,

	{
		{
			0,
			{ 0, 0,  XYSDK_LIST_END,  XYSDK_LIST_H}, // normal
			{ 0, 0,  XYSDK_LIST_END ,  XYSDK_LIST_H}, // focus
			0, 0,
			0, 0
		},
		{
			0,
			{ XYSDK_ICON_START_X+1, XYSDK_LIST_SPACE_2_H,  XYSDK_NUM_ICON_W + XYSDK_ICON_START_X,  XYSDK_LIST_H - XYSDK_LIST_SPACE_2_H}, // normal
			{ XYSDK_ICON_START_X+1, XYSDK_LIST_SPACE_2_H,  XYSDK_NUM_ICON_W + XYSDK_ICON_START_X ,	XYSDK_LIST_H - XYSDK_LIST_SPACE_2_H}, // focus
			XYSDK_LIST_NUM_FONT, XYSDK_LIST_NUM_FONT,
			0, 0
		},
		{
			GUIITEM_CONTENT_STATE_EFFECT_STR,
			{ XYSDK_NUM_START_X,	XYSDK_TXT_SPACE_H,	XYSDK_TXT_L_END_X, XYSDK_LIST_H - XYSDK_TXT_SPACE_H },
			{ XYSDK_NUM_START_X,	XYSDK_TXT_SPACE_H,	XYSDK_TXT_L_END_X, XYSDK_LIST_H - XYSDK_TXT_SPACE_H},
			XYSDK_LIST_FONT, XYSDK_LIST_FONT,
			0, 100
		},
		{
			0,
			{ XYSDK_TAG_L_END_X-XYSDK_TAG_L_W, XYSDK_TAG_L_SPACE_H,  XYSDK_TAG_L_END_X,  XYSDK_LIST_H - XYSDK_TAG_L_SPACE_H}, // normal
			{ XYSDK_TAG_L_END_X-XYSDK_TAG_L_W, XYSDK_TAG_L_SPACE_H,  XYSDK_TAG_L_END_X ,  XYSDK_LIST_H - XYSDK_TAG_L_SPACE_H}, // focus
			0, 0,
			0, 0
		},
		{0},
		{0},
	},

	2, MMILIST_INVALID_ITEM_INDEX
};



//|---------------------------|
//|   @  @   text         @   |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_ICON_TEXT_ICON_XMLY,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_icon_text_icon_xmly =
{
    XYSDK_LIST_H, XYSDK_LIST_H,

    {
    	{
            0,
            { 0, 0,  XYSDK_LIST_END,  XYSDK_LIST_H}, // normal
            { 0, 0,  XYSDK_LIST_END ,  XYSDK_LIST_H}, // focus
            0, 0,
            0, 0
        },
        {
            0,
            { XYSDK_ICON_START_X+1, XYSDK_LIST_SPACE_2_H+1,  XYSDK_ALBUM_ICON_W + XYSDK_ICON_START_X,  XYSDK_LIST_H - XYSDK_LIST_SPACE_2_H}, // normal
            { XYSDK_ICON_START_X+1, XYSDK_LIST_SPACE_2_H+1,  XYSDK_ALBUM_ICON_W + XYSDK_ICON_START_X ,  XYSDK_LIST_H - XYSDK_LIST_SPACE_2_H}, // focus
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { XYSDK_TXT_START_2_X,  XYSDK_TXT_SPACE_H,  XYSDK_TXT_L_END_X, XYSDK_LIST_H - XYSDK_TXT_SPACE_H },
            { XYSDK_TXT_START_2_X,  XYSDK_TXT_SPACE_H,  XYSDK_TXT_L_END_X, XYSDK_LIST_H - XYSDK_TXT_SPACE_H},
            XYSDK_LIST_FONT, XYSDK_LIST_FONT,
            0, 100
        },
        {
            0,
            { XYSDK_TAG_L_END_X-XYSDK_TAG_L_W, XYSDK_TAG_L_SPACE_H,  XYSDK_TAG_L_END_X,  XYSDK_LIST_H - XYSDK_TAG_L_SPACE_H}, // normal
            { XYSDK_TAG_L_END_X-XYSDK_TAG_L_W, XYSDK_TAG_L_SPACE_H,  XYSDK_TAG_L_END_X ,  XYSDK_LIST_H - XYSDK_TAG_L_SPACE_H}, // focus
            0, 0,
            0, 0
        },
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
#endif

#ifdef _SW_ZDT_PRODUCT_
//GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_BIGICON
#define MMI_BIG_LIST_DOUBLE_LINE_ITEM_HEIGHT 80
#define MMI_BIG_LIST_TEXT_LEFT_SPACE 6
#define MMI_BIG_LIST_ITEM_TOP_MARGIN 10
#define MMI_BIG_LIST_ITEM_SELECT_ICON_WIDTH 64
#define MMI_BIG_LIST_ITEM_RIGHT 234
#define MMI_BIG_LIST_ICON_MARGIN_MS 16
#define MMI_BIG_LIST_RIGHT_ICON_LEFT_SPACE 6

LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_and_text_bigicon =
{
    MMI_BIG_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_BIG_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_BIG_LIST_TEXT_LEFT_SPACE,  MMI_BIG_LIST_ITEM_TOP_MARGIN,  MMI_BIG_LIST_ITEM_RIGHT - MMI_BIG_LIST_ITEM_SELECT_ICON_WIDTH - MMI_BIG_LIST_RIGHT_ICON_LEFT_SPACE, MMI_BIG_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     // 1 第一行文字
            { MMI_BIG_LIST_TEXT_LEFT_SPACE,  MMI_BIG_LIST_ITEM_TOP_MARGIN,  MMI_BIG_LIST_ITEM_RIGHT - MMI_BIG_LIST_ITEM_SELECT_ICON_WIDTH - MMI_BIG_LIST_RIGHT_ICON_LEFT_SPACE, MMI_BIG_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     // 1 第一行文字
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_BIG_LIST_ITEM_RIGHT - (MMI_BIG_LIST_ITEM_SELECT_ICON_WIDTH), 1, MMI_BIG_LIST_ITEM_RIGHT, MMI_BIG_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            { MMI_BIG_LIST_ITEM_RIGHT - (MMI_BIG_LIST_ITEM_SELECT_ICON_WIDTH), 1, MMI_BIG_LIST_ITEM_RIGHT, MMI_BIG_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            0, 0,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_BIG_LIST_TEXT_LEFT_SPACE, MMI_BIG_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_BIG_LIST_ITEM_RIGHT - MMI_BIG_LIST_ITEM_SELECT_ICON_WIDTH - MMI_BIG_LIST_RIGHT_ICON_LEFT_SPACE, MMI_BIG_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, // 3 第二行文字.
            { MMI_BIG_LIST_TEXT_LEFT_SPACE, MMI_BIG_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_BIG_LIST_ITEM_RIGHT - MMI_BIG_LIST_ITEM_SELECT_ICON_WIDTH - MMI_BIG_LIST_RIGHT_ICON_LEFT_SPACE, MMI_BIG_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, // 3 第二行文字.

            SONG_FONT_22, SONG_FONT_22,

            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, 2
};

//    GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_MS
#define MMI_PB_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS 80
#define MMI_PB_LIST_ICON_LEFT_SPACE_MS 6
#define MMI_PB_LIST_ITEM_TOP_MARGIN_MS 6
#define MMI_PB_LIST_BIG_ICON_WIDTH_MS 60
#define MMI_PB_LIST_ITEM_RIGHT_MS 234
#define MMI_PB_LIST_ICON_MARGIN_MS 10
#define MMI_PB_LIST_ITEM_BOTTOM_MARGIN_MS 6

LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_bigicon_text_and_text_ms =
{
    MMI_PB_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS, MMI_PB_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS,

    {   //icon
        {
            0,
            { MMI_PB_LIST_ICON_LEFT_SPACE_MS, MMI_PB_LIST_ITEM_TOP_MARGIN_MS*2, MMI_PB_LIST_BIG_ICON_WIDTH_MS + MMI_PB_LIST_ICON_LEFT_SPACE_MS, MMI_PB_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_PB_LIST_ITEM_TOP_MARGIN_MS*2}, /* 1 */
            { MMI_PB_LIST_ICON_LEFT_SPACE_MS, MMI_PB_LIST_ITEM_TOP_MARGIN_MS*2, MMI_PB_LIST_BIG_ICON_WIDTH_MS + MMI_PB_LIST_ICON_LEFT_SPACE_MS, MMI_PB_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_PB_LIST_ITEM_TOP_MARGIN_MS*2}, /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_PB_LIST_BIG_ICON_WIDTH_MS + MMI_PB_LIST_ICON_LEFT_SPACE_MS*2,  MMI_PB_LIST_ITEM_TOP_MARGIN_MS,  MMI_PB_LIST_ITEM_RIGHT_MS, MMI_PB_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            { MMI_PB_LIST_BIG_ICON_WIDTH_MS + MMI_PB_LIST_ICON_LEFT_SPACE_MS*2,  MMI_PB_LIST_ITEM_TOP_MARGIN_MS,  MMI_PB_LIST_ITEM_RIGHT_MS, MMI_PB_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_PB_LIST_BIG_ICON_WIDTH_MS + MMI_PB_LIST_ICON_LEFT_SPACE_MS*2, MMI_PB_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_PB_LIST_ITEM_RIGHT_MS, MMI_PB_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_PB_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            { MMI_PB_LIST_BIG_ICON_WIDTH_MS + MMI_PB_LIST_ICON_LEFT_SPACE_MS*2, MMI_PB_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS / 2 - 4, MMI_PB_LIST_ITEM_RIGHT_MS, MMI_PB_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - MMI_PB_LIST_ITEM_BOTTOM_MARGIN_MS},     /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        //add for name abbreviation text area
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { MMI_PB_LIST_ICON_LEFT_SPACE_MS + 3, MMI_PB_LIST_ICON_MARGIN_MS, MMI_PB_LIST_BIG_ICON_WIDTH_MS + MMI_PB_LIST_ICON_LEFT_SPACE_MS, MMI_PB_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - (MMI_PB_LIST_ICON_MARGIN_MS+1)}, /* 1 */
            { MMI_PB_LIST_ICON_LEFT_SPACE_MS + 3, MMI_PB_LIST_ICON_MARGIN_MS, MMI_PB_LIST_BIG_ICON_WIDTH_MS + MMI_PB_LIST_ICON_LEFT_SPACE_MS, MMI_PB_LIST_DOUBLE_LINE_ITEM2_HEIGHT_MS - (MMI_PB_LIST_ICON_MARGIN_MS+1)}, /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 0
        },
        {0},
        {0},
    },

    4, MMILIST_INVALID_ITEM_INDEX
};

// GUIITEM_STYLE_TWO_LINE_BIGICON_TWO_TEXT_AND_TEXT
#define MMI_SMS_LIST_DOUBLE_LINE_ITEM_HEIGHT 80
#define MMI_SMS_LIST_FIRST_HEIGHT 32
#define MMI_SMS_LIST_SECOND_HEIGHT 24
#define MMI_SMS_LIST_ICON_LEFT_SPACE 3
#define MMI_SMS_DEFAULT_FONT_ASCII_WIDTH 64
#define MMI_SMS_LIST_ITEM_BIG_ICON_WIDTH 60
#define MMI_SMS_LIST_ITEM_RIGHT 234

LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_bigicon_2text_and_text =
{
    MMI_SMS_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_SMS_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_SMS_LIST_ICON_LEFT_SPACE,   0,  MMI_SMS_LIST_ITEM_BIG_ICON_WIDTH + MMI_SMS_LIST_ICON_LEFT_SPACE,  MMI_SMS_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            { MMI_SMS_LIST_ICON_LEFT_SPACE,   0,  MMI_SMS_LIST_ITEM_BIG_ICON_WIDTH + MMI_SMS_LIST_ICON_LEFT_SPACE,  MMI_SMS_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_SMS_LIST_ITEM_BIG_ICON_WIDTH + MMI_SMS_LIST_ICON_LEFT_SPACE + 6,  0,  MMI_SMS_LIST_ITEM_RIGHT, MMI_SMS_LIST_FIRST_HEIGHT-1 }, /* 3 */
            { MMI_SMS_LIST_ITEM_BIG_ICON_WIDTH + MMI_SMS_LIST_ICON_LEFT_SPACE + 6,  0,  MMI_SMS_LIST_ITEM_RIGHT, MMI_SMS_LIST_FIRST_HEIGHT-1 }, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_SMS_LIST_ITEM_BIG_ICON_WIDTH + MMI_SMS_LIST_ICON_LEFT_SPACE + 6,  MMI_SMS_LIST_FIRST_HEIGHT,  MMI_SMS_LIST_ITEM_RIGHT, MMI_SMS_LIST_FIRST_HEIGHT+MMI_SMS_LIST_SECOND_HEIGHT-1 }, /* 3 */
            { MMI_SMS_LIST_ITEM_BIG_ICON_WIDTH + MMI_SMS_LIST_ICON_LEFT_SPACE + 6,  MMI_SMS_LIST_FIRST_HEIGHT,  MMI_SMS_LIST_ITEM_RIGHT, MMI_SMS_LIST_FIRST_HEIGHT+MMI_SMS_LIST_SECOND_HEIGHT-1 }, /* 3 */
            SONG_FONT_20, SONG_FONT_20,
            0, 100
        },
        {
            0,
            { MMI_SMS_LIST_ITEM_BIG_ICON_WIDTH + MMI_SMS_LIST_ICON_LEFT_SPACE + 6,  MMI_SMS_LIST_FIRST_HEIGHT+MMI_SMS_LIST_SECOND_HEIGHT,  MMI_SMS_LIST_ITEM_RIGHT, MMI_SMS_LIST_FIRST_HEIGHT+MMI_SMS_LIST_SECOND_HEIGHT*2-1 }, /* 3 */
            { MMI_SMS_LIST_ITEM_BIG_ICON_WIDTH + MMI_SMS_LIST_ICON_LEFT_SPACE + 6,  MMI_SMS_LIST_FIRST_HEIGHT+MMI_SMS_LIST_SECOND_HEIGHT,  MMI_SMS_LIST_ITEM_RIGHT, MMI_SMS_LIST_FIRST_HEIGHT+MMI_SMS_LIST_SECOND_HEIGHT*2-1  }, /* 3 */
            SONG_FONT_20, SONG_FONT_20,
            0, 100
        },
        {0},
        {0},
    },

    0, 3
};

//GUIITEM_STYLE_ONE_LINE_BGICON_TEXT_MS,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_bgicon_text_ms =
{
    40, 40,
	
    {
        //background
        {
            GUIITEM_CONTENT_STATE_NONE,
            { 40, 0,  145 + 40,  39}, /*1*/
            { 40, 0,  145 + 40,  39}, /*1*/
            0, 0,
            0, 0
        },
        //text content
        {
            GUIITEM_CONTENT_STATE_NONE,
            { 138,  2,  138 + 46 , 37},/*2*/
            { 138,  2,  138 + 46 , 37},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        //playing audio icon, icon height 40
        {
            GUIITEM_CONTENT_STATE_NONE,
            { 2, (40-38)/2,  38 + 2,  40-(40-38)/2}, /*1*/
            { 2, (40-38)/2,  38 + 2,  40-(40-38)/2}, /*1*/
            0, 0,
            0, 0
        },
        //read status icon 14
        {
            GUIITEM_CONTENT_STATE_NONE,
            { 188, (40-14)/2,  188 + 14,  40-(40-14)/2}, /*1*/
            { 188, (40-14)/2,  188 + 14,  40-(40-14)/2}, /*1*/
            0, 0,
            0, 0
        },
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

//GUIITEM_STYLE_ONE_LINE_BGICON_R_TEXT_MS
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_bgicon_text_ms =
{
    40, 40,
	
    {
        //background
        {
            GUIITEM_CONTENT_STATE_NONE,
            { 199-145, 0,  199,  39}, /*1*/
            { 199-145, 0,  199,  39}, /*1*/
            0, 0,
            0, 0
        },
        //text content
        {
            GUIITEM_CONTENT_STATE_NONE,
            { 199-145+2,  2,  199-145+48 , 37},/*2*/
            { 199-145+2,  2,  199-145+48 , 37},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        //playing audio icon, icon height 40
        {
            GUIITEM_CONTENT_STATE_NONE,
            { 200, (40-38)/2,  200+38,  40-(40-38)/2}, /*1*/
            { 200, (40-38)/2,  200+38,  40-(40-38)/2}, /*1*/
            0, 0,
            0, 0
        },
        //read status icon
        {
            GUIITEM_CONTENT_STATE_NONE,
            { 30, (40-14)/2,  30 + 14,  40-(40-14)/2}, /*1*/
            { 30, (40-14)/2,  30 + 14,  40-(40-14)/2}, /*1*/
            0, 0,
            0, 0
        },
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
#endif

//|---------------------------|
//|  @     text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_without_effect_str =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2}, // normal
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE ,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2}, // focus
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT - 1 },
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT - 1 },
            MMI_LIST_EXPAND_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|  @  @  text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_TWO_ICON_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_icon_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 4 + MMI_LIST_ICON_LEFT_SPACE * 2 ,  MMI_LIST_ITEM_HEIGHT - 1 }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2 }, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 4 + MMI_LIST_ICON_LEFT_SPACE * 2 ,  0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT }, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT - 1},   /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|  ⑴   text                |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_NUMBER_TEXT,
//s_item_style_1_text

#endif

//|---------------------------|
//|  @     text       text    |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH / 3 * 2 /*- MMI_LIST_ICON_LEFT_SPACE*/, MMI_LIST_ITEM_HEIGHT },   /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH / 3 * 2 /*- MMI_LIST_ICON_LEFT_SPACE*/, MMI_LIST_EXPAND_ITEM_HEIGHT  - 1},   /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 35
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + MMI_LIST_ICON_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_ITEM_HEIGHT },   /* 3 */
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + MMI_LIST_ICON_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_EXPAND_ITEM_HEIGHT - 1},   /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            35, 70/*65*/
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|   @   text           text |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT,  // item pos is not differnt with above one
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_text_2 =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_MAINSCREEN_WIDTH / 4 * 3 - MMI_LIST_ICON_LEFT_SPACE * 6, MMI_LIST_ITEM_HEIGHT - 1}, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_MAINSCREEN_WIDTH / 4 * 3 - MMI_LIST_ICON_LEFT_SPACE * 6, MMI_LIST_EXPAND_ITEM_HEIGHT - 1}, /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_MAINSCREEN_WIDTH / 4 * 3 - MMI_LIST_ICON_LEFT_SPACE - 8, 0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_ITEM_HEIGHT },   /* 3 */
            { MMI_MAINSCREEN_WIDTH / 4 * 3 - MMI_LIST_ICON_LEFT_SPACE - 8, 0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_EXPAND_ITEM_HEIGHT },   /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            100, 100
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|  @  @  text       text    |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_TWO_ICON_TWO_TEXT,
#ifdef GUIF_LIST_PDASTYLE
#ifdef MAINLCD_SIZE_240X320
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_icon_text_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE - 16,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE - 16,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2  - 16,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 16,  MMI_LIST_ITEM_HEIGHT  - 2}, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2  - 16,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 16,  MMI_LIST_EXPAND_ITEM_HEIGHT  - 2}, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 16,  0,  MMI_MAINSCREEN_WIDTH * 2 / 3, MMI_LIST_ITEM_HEIGHT -1}, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 16,  0,  MMI_MAINSCREEN_WIDTH * 2 / 3, MMI_LIST_EXPAND_ITEM_HEIGHT -1}, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH * 2 / 3 + 2, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT -1 },   /* 4 */
            { MMI_MAINSCREEN_WIDTH * 2 / 3 + 2, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT -1},   /* 4 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            100, 100
        },
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
#else
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_icon_text_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE - 20,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE - 20,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2  - 20,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 20,  MMI_LIST_ITEM_HEIGHT  - 2}, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2  - 20,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 20,  MMI_LIST_EXPAND_ITEM_HEIGHT  - 2}, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 20,  0,  MMI_MAINSCREEN_WIDTH * 2 / 3, MMI_LIST_ITEM_HEIGHT -1 }, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 - 20,  0,  MMI_MAINSCREEN_WIDTH * 2 / 3, MMI_LIST_EXPAND_ITEM_HEIGHT -1 }, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH * 2 / 3 + 2, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT -1},   /* 4 */
            { MMI_MAINSCREEN_WIDTH * 2 / 3 + 2, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT -1 },   /* 4 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            100, 100
        },
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
#endif
#else
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_icon_text_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_ITEM_HEIGHT }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2}, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_MAINSCREEN_WIDTH * 2 / 3, MMI_LIST_ITEM_HEIGHT }, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_MAINSCREEN_WIDTH * 2 / 3, MMI_LIST_EXPAND_ITEM_HEIGHT }, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH * 2 / 3 + 2, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT },   /* 4 */
            { MMI_MAINSCREEN_WIDTH * 2 / 3 + 2, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT },   /* 4 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            100, 100
        },
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};
#endif

//|---------------------------|
//|           | text |        |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_BUTTON_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_buttontext =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_EXPAND_ITEM_HEIGHT - 1},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|   | text |      |text|    |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_TWO_BUTTON_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_2buttontext =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_TEXT_LEFT_SPACE, 0, (MMI_MAINSCREEN_WIDTH) / 2 - MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_BUTTON_HEIGHT },      /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE, 0, (MMI_MAINSCREEN_WIDTH) / 2 - MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_BUTTON_HEIGHT },      /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            {(MMI_MAINSCREEN_WIDTH / 2 + MMI_LIST_TEXT_LEFT_SPACE) ,  0,  MMI_MAINSCREEN_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_BUTTON_HEIGHT },     /* 2 */
            {(MMI_MAINSCREEN_WIDTH / 2 + MMI_LIST_TEXT_LEFT_SPACE) ,  0,  MMI_MAINSCREEN_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_BUTTON_HEIGHT },     /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            50, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif

//|---------------------------|
//| text                text  |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT,
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_small_text_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_MAINSCREEN_WIDTH / 2,  MMI_LIST_ITEM_HEIGHT },     /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_MAINSCREEN_WIDTH / 2,  MMI_LIST_ITEM_HEIGHT },     /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            {MMI_MAINSCREEN_WIDTH / 2 + 3 * MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH, MMI_LIST_ITEM_HEIGHT },   /* 2 */
            {MMI_MAINSCREEN_WIDTH / 2 + 3 * MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH, MMI_LIST_ITEM_HEIGHT },   /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            50, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#else
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_small_text_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT / 2 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT },     /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT / 2 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT },     /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            GUIITEM_CONTENT_STATE_CHECK | GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            {MMI_LIST_ITEM_RIGHT / 2 + MMI_LIST_TEXT_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_ITEM_HEIGHT },     /* 2 */
            {MMI_LIST_ITEM_RIGHT / 2 + MMI_LIST_TEXT_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_ITEM_HEIGHT },     /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            50, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif
#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//| text             text  @  |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_small_text_text_icon =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,
    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH/3 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},  /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH/3 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},  /* 1 */
            0, 0,
            0, 50
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH / 2 + MMI_LIST_TEXT_LEFT_SPACE,  0, MMI_MAINSCREEN_WIDTH-MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},       /* 2 */
            { MMI_MAINSCREEN_WIDTH / 2 + MMI_LIST_TEXT_LEFT_SPACE,  0, MMI_MAINSCREEN_WIDTH-MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1},       /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE,  1,  MMI_MAINSCREEN_WIDTH, MMI_LIST_ITEM_HEIGHT - 2 },       /* 3 */
            { MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE,  1,  MMI_MAINSCREEN_WIDTH, MMI_LIST_ITEM_HEIGHT - 2 },       /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//| text   text      @     @  |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_TEXT_TWO_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_small_2text_2icon =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_HEIGHT,
    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH / 3 - MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT },     /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_MAINSCREEN_WIDTH / 3 - MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT },     /* 1 */
            0, 0,
            0, 50
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH / 3 - MMI_LIST_TEXT_LEFT_SPACE,  0, MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 - MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_ITEM_HEIGHT }, /* 2 */
            { MMI_MAINSCREEN_WIDTH / 3 - MMI_LIST_TEXT_LEFT_SPACE,  0, MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 - MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_ITEM_HEIGHT }, /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 - MMI_LIST_ICON_LEFT_SPACE * 2,  1,  MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_HEIGHT - 2}, /* 3 */
            { MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 - MMI_LIST_ICON_LEFT_SPACE * 2,  1,  MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_HEIGHT - 2}, /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_RIGHT, MMI_MAINSCREEN_WIDTH - 2},     /* 4 */
            { MMI_MAINSCREEN_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_RIGHT, MMI_MAINSCREEN_WIDTH - 2},     /* 4 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif

//|---------------------------|
//|      text              √ |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_CHECK,
// s_item_style_1_text

//|---------------------------|
//|     text              √  |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_CHECK_EXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_check_ext =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE * 2, MMI_LIST_ITEM_HEIGHT - 1},   /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE * 2, MMI_LIST_EXPAND_ITEM_HEIGHT - 1},   /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|      text             ⊙  |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_RADIO,
//s_item_style_1_text
//|---------------------------|
//|      text             ⊙  |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_RADIO_EXT,
//s_item_style_1_text_check_ext
//|---------------------------|
//|      text           @  √ |
//|---------------------------|
// GUIITEM_STYLE_ONE_LINE_CHECK_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_check_icon =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_EXPAND_ITEM_HEIGHT - 2 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT - 1 },       /* 2 */
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT - 1 },       /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 80
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|      text           @  ⊙ |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_RADIO_ICON,
//s_item_style_1_check_icon

/* 以下都为双行item, AND表示第二行 */

//|---------------------------|
//|        text               |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},    /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},     /* 2 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},     /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//| text            text      |
//| text            text      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_AND_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_2text_and_2text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT * 2 / 3 - 1, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT * 2 / 3 - 1, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT * 2 / 3 + 1,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, /* 2 */
            { MMI_LIST_ITEM_RIGHT * 2 / 3 + 1,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT * 2 / 3 - 1, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT * 2 / 3 - 1, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 50
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT * 2 / 3 + 1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            { MMI_LIST_ITEM_RIGHT * 2 / 3 + 1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|  @     text               |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_text_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},     /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},     /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};


#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|  @ @   text               |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_2icon_text_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},   /* 4 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},   /* 4 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|  ⑴    text               |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_NUMBER_TEXT_AND_TEXT,
//s_item_style_2_text_text

//|---------------------------|
//|  @     text               |
//|        text     text      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_text_and_2text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 }, /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 }, /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },   /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },   /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100,
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH * 3 / 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH * 3 / 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 50
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH * 3 / 5 + 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 4 */
            { MMI_MAINSCREEN_WIDTH * 3 / 5 + 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 4 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {0},
        {0},
    },

    1, 2
};


//|---------------------------|
//|  @     text               |
//|        text     text      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT_SMALL,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_text_and_2text_small =
{
    MMI_SMALL_LIST_ITEM_HEIGHT * 2 - 1, MMI_SMALL_LIST_ITEM_HEIGHT * 2 - 1,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_SMALL_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_SMALL_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_RIGHT, MMI_SMALL_LIST_ITEM_HEIGHT },     /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_RIGHT, MMI_SMALL_LIST_ITEM_HEIGHT },     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100,
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_SMALL_LIST_ITEM_HEIGHT, MMI_MAINSCREEN_WIDTH * 3 / 5, MMI_SMALL_LIST_ITEM_HEIGHT * 2 }, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_SMALL_LIST_ITEM_HEIGHT, MMI_MAINSCREEN_WIDTH * 3 / 5, MMI_SMALL_LIST_ITEM_HEIGHT * 2 }, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 50
        },
        {
            0,
            { 100/*MMI_MAINSCREEN_WIDTH*3/5 + 2*/, MMI_SMALL_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_RIGHT, MMI_SMALL_LIST_ITEM_HEIGHT * 2 },     /* 4 */
            { 100/*MMI_MAINSCREEN_WIDTH*3/5 + 2*/, MMI_SMALL_LIST_ITEM_HEIGHT, MMI_LIST_ITEM_RIGHT, MMI_SMALL_LIST_ITEM_HEIGHT * 2 },     /* 4 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            50, 100
        },
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};


#endif

//|---------------------------|
//|  @ @   text               |
//|        text     text      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_2icon_text_and_2text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },     /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },     /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 6 + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 6 + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },   /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },   /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100,
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 18,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8 , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT  }, /* 4 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 18,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8 , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT  }, /* 4 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 5 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 5 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            50, 100
        },
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|  @     text     text      |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_2text_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 },   /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 },   /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 9, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 9, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 9, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 9, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_TOP_MARGIN},     /* 4 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_TOP_MARGIN},     /* 4 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};



//|---------------------------|
//|  @     text     text      |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_SMALL,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_2text_and_text_small =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 },   /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 },   /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 50,
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_TOP_MARGIN},     /* 4 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_TOP_MARGIN},     /* 4 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|  @ @   text     text      |
//|        text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TWO_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_2icon_2text_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_MAINSCREEN_WIDTH / 3 * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_MAINSCREEN_WIDTH / 3 * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 50,
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 4 */
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 4 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },     /* 5 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },     /* 5 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
    },

    2, 4
};


//|---------------------------|
//|  ┏┓   text              |
//|  ┗┛            text     |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_text_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },     /* 2 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};



//|---------------------------|
//|  ┏┓   text              |
//|  ┗┛   text              |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_text_and_text_2 =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     /* 2 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },     /* 3 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },     /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};


#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|  ┏┓   text         @    |
//|  ┗┛   text              |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_text_icon_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },   /* 1 icon */
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },   /* 1 icon */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 text */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 text */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, 0, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, /* 3 icon */
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, 0, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, /* 3 icon */
            0, 0,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1}, /* 4 text */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1}, /* 4 text */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};



//|---------------------------|
//|  ┏┓   text       @ @ @  |
//|  ┗┛   text              |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_3ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_text_3icon_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },     /* 1 */
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },     /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + 3,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   /* 2 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + 3,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //3
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //3
            0, 0,
            100, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //4
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //4
            0, 0,
            100, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //5
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //5
            0, 0,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_HEIGHT + 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},
            { MMI_LIST_ITEM_HEIGHT + 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
    },

    5, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|      text              ⊙ |
//|      text                 |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_RADIO,
// s_item_style_2_text_text
#endif

//|---------------------------|
//|      text           @  √ |
//|      text                 |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_CHECK_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_icon_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE , 0, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            { MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE , 0, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            0, 0,
            100, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },  /* 2 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },  /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|  ┏┓   text         @    |
//|  ┗┛   text              |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_2TEXT_AND_ICON,
//s_item_style_2_anim_text_icon_and_text
#endif

//|---------------------------|
//|         text         ┏┓ |
//|         text         ┗┛ |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_and_text_anim =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     // 1 第一行文字
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     // 1 第一行文字
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            0, 0,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, // 3 第二行文字.
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN}, // 3 第二行文字.
#if defined(MAINLCD_SIZE_128X160)
            MMI_LIST_BIG_ITEM_FONT, MMI_LIST_BIG_ITEM_FONT,
#else
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
#endif
            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, 2
};


#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//| ┏┓    text         ┏┓ |
//| ┗┛    text         ┗┛ |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ANIM,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_text_and_text_anim =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 2 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 2 },       /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 0
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     // 1 第一行文字
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     // 1 第一行文字
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            0, 0,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第二行文字.
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第二行文字.
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
    },

    0, 2
};
#endif

//|---------------------------|
//|  text           ┏┓ ┏┓ |
//|  text           ┗┛ ┗┛ |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_TEXT_2ANIM_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_2anim_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_2ANIM_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   // 1 第一行文字
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_2ANIM_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   // 1 第一行文字
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH) - 2 * MMI_LIST_ICON_LEFT_SPACE, 1, MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, // 2 图片
            { MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH) - 2 * MMI_LIST_ICON_LEFT_SPACE, 1, MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, // 2 图片
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            0, 0,
            100, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第二行文字.
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第二行文字.
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
    },

    0, 3
};
/* 以下都为高亮时双行, 非高亮时单行的item */

//|---------------------------|
//|  @     text               |
//|        text               |
//|---------------------------|

//|---------------------------|
//|  @     text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_2_icon_text_and_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUIITEM_STATE_CONTENT_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE / 2, 1, MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 2 },        /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_ITEM_HEIGHT },       /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            {0},
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},     /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};



#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|  @ @   text               |
//|        text               |
//|---------------------------|

//|---------------------------|
//|  @ @   text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_2_2icon_text_and_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT - 1 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },    /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 4 + MMI_LIST_ICON_LEFT_SPACE * 2 ,  MMI_LIST_ITEM_HEIGHT - 1 }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 4 + MMI_LIST_ICON_LEFT_SPACE * 2 ,  0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT }, /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2 + MMI_LIST_ICON_LEFT_SPACE * 2 + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            {0},
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN},   /* 4 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|  ⑴    text               |
//|        text               |
//|---------------------------|

//|---------------------------|
//|  ⑴    text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_TWO_LINE_NUMBER_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_2_text_and_text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},    /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},    /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            {0},
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},     /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|  @     text               |
//|        text     text      |
//|---------------------------|

//|---------------------------|
//|  @     text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_2_icon_text_and_2text =
{
    MMI_LIST_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE / 2, 1, MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE / 2,  MMI_LIST_ITEM_HEIGHT - 2 },        /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 }, /* 1 */
            0, 0,
            0, 0
        },        
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + (MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE + 2) / 2,  0,  MMI_LIST_ITEM_RIGHT / 2, MMI_LIST_ITEM_HEIGHT },        /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },   /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },        
        {
            0,
            {0},
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE * 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH * 3 / 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            {0},
            { MMI_MAINSCREEN_WIDTH * 3 / 5 + 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 4 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            50, 100
        },
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|  @ @   text               |
//|        text     text      |
//|---------------------------|

//|---------------------------|
//|  @ @   text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_2_2icon_text_and_2text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },     /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },     /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 6 + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_DEFAULT_FONT_ASCII_WIDTH * 6 + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 2 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },   /* 3 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },   /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 18,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8 , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT  }, /* 4 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 18,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8 , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT  }, /* 4 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 50
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 5 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 8, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 5 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {0},
    },

    2, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|  ┏┓   text       @ @ @  |
//|  ┗┛   text              |
//|---------------------------|

//|---------------------------|
//|  ┏┓   text       @ @ @  |
//|---------------------------|
//GUIITEM_STYLE_ONE_TWO_ANIM_TEXT_AND_TEXT_3ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_2_anim_text_and_text_3icon =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { 1,   1,  MMI_LIST_ITEM_HEIGHT - 3,  MMI_LIST_ITEM_HEIGHT - 2 },       /* 1 */
            { 1,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },     /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_HEIGHT,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_HEIGHT * 3, MMI_LIST_ITEM_HEIGHT  - 1},     /* 2 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + 3,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 3, 1, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, MMI_LIST_ITEM_HEIGHT - 2}, //3
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //3
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, 1, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_ITEM_HEIGHT - 2}, //4
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //4
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT  - 2},//5
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, //5
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            {0},
            { MMI_LIST_ITEM_HEIGHT + 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_BOTTOM_MARGIN}, /* 6 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

#endif
/*以下都是三行*/

//|---------------------------|
//|        text               |
//|        text               |
//|        text               |
//|---------------------------|
// GUIITEM_STYLE_THREE_LINE_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_3_text =
{
    (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 - 1, (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 - 1,

    {
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { 2,   0,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     /* 1 */
            { 2,   0,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},     /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { 2,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1},     /* 1 */
            { 2,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1},     /* 1 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { 2,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_ITEM_RIGHT, (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 },    /* 1 */
            { 2,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_ITEM_RIGHT, (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 },    /* 1 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|        text               |
//|        text        @      |
//|        text               |
//|---------------------------|
// GUIITEM_STYLE_THREE_LINE_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_3_text_and_text_and_text_icon =
{
    (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 - 1, (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 - 1,

    {
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { 14,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},
            { 14,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { 14,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1},
            { 14,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1},
            MMI_LIST_BIG_ITEM_FONT, MMI_LIST_BIG_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { 14,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE, (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 - MMI_LIST_ITEM_BOTTOM_MARGIN },
            { 14,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE, (MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2) * 3 - MMI_LIST_ITEM_BOTTOM_MARGIN },
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1},
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1},
            0, 0,
            100, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif

//|---------------------------|
//|        text     @         |
//|        text     text      |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_TEXT_ICON_AND_TWO_TEXT_SMALL,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_icon_and_two_text_small =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, /* 2 */
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 2 * MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, /* 2 */
            0, 0,
            100, 100
        },
        {
            0,
#ifdef MAINLCD_SIZE_128X160
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 , MMI_MAINSCREEN_WIDTH / 3 + MMI_LIST_TEXT_LEFT_SPACE * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN }, /* 3 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 , MMI_MAINSCREEN_WIDTH / 3 + MMI_LIST_TEXT_LEFT_SPACE * 3, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN }, /* 3 */
#else
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },   /* 3 */
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN },   /* 3 */
#endif
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 50
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
#ifdef MAINLCD_SIZE_128X160
            { MMI_MAINSCREEN_WIDTH / 3 + MMI_LIST_TEXT_LEFT_SPACE * 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN }, /* 4 */
            { MMI_MAINSCREEN_WIDTH / 3 + MMI_LIST_TEXT_LEFT_SPACE * 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT + MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN }, /* 4 */
#else
            { MMI_MAINSCREEN_WIDTH / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN }, /* 4 */
            { MMI_MAINSCREEN_WIDTH / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN }, /* 4 */
#endif
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {0},
        {0},
    },

    1, 3
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//| ┏┓   text     text      |
//| ┗┛   @text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_2text_and_icon_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   1,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ICON_LEFT_SPACE  + 1,  0,  MMI_MAINSCREEN_WIDTH / 5 * 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1  }, /* 3 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ICON_LEFT_SPACE  + 1,  0,  MMI_MAINSCREEN_WIDTH / 5 * 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1  }, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 10, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },  /* 4  4/5*/
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 10, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },  /* 4  4/5*/
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },     /* 5 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },     /* 5 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
    },

    0, 3
};

//|---------------------------|
//| ┏┓   text     text      |
//| ┗┛   text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_2text_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },       /* 1 */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ICON_LEFT_SPACE + 1,  0,  MMI_MAINSCREEN_WIDTH / 5 * 4 - 6, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1  }, /* 3 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ICON_LEFT_SPACE + 1,  0,  MMI_MAINSCREEN_WIDTH / 5 * 4 - 6, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1  }, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 10 , 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 4 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 10 , 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 4 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + 3 ,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },     /* 5 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + 3 ,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },     /* 5 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
    },

    0, 3
};


//|---------------------------|
//| ┏┓   @text             |
//| ┗┛   @text     text          |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_ICON_TEXT_AND_ICON_2TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_icon_text_and_icon_2text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},       /* ANIMATE */
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},       /* ANIMATE */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 8 , MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 8 , MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 14, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},   /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 14, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},   /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            {MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH * 3 / 5 + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8 - 40 + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            {MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH * 3 / 5 + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8 - 40 + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 50
        },
        {
            0,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8 + MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_MAINSCREEN_WIDTH * 3 / 5 - 40, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8 + MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_MAINSCREEN_WIDTH * 3 / 5 - 40, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

#endif

//|---------------------------|
//| ┏┓   text               |
//| ┗┛                      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TEXT,//mary
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},      /* ANIMATE */
            { MMI_LIST_ICON_LEFT_SPACE,   MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},      /* ANIMATE */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, /* 3 */
            { MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT

//|---------------------------|
//| ┏┓   text               |
//| ┗┛                      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_CHECK_ANIM_TEXT,//mary
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_text_2 =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},      /* ANIMATE */
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},      /* ANIMATE */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 },
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//| ┏┓   text            ⊙ |
//| ┗┛                      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_RADIO_ANIM_TEXT,//mary
//s_item_style_2_anim_text_2

#endif
//|---------------------------|
//| ┏┓   text             √|
//| ┗┛                      |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_CHECK_ANIM_TEXT_AND_TEXT,//mary
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_check_anim_text_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},      /* ANIMATE */
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},      /* ANIMATE */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            MMI_LIST_EXPAND_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//| ┏┓   text             √|
//| ┗┛   text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_RADIO_ANIM_TEXT_AND_TEXT,//mary
//s_item_style_2_anim_text_and_text

#ifndef MMI_THEME_LIST_MINI_SUPPORT
//|---------------------------|
//|  text  text      畅?┏┓ |
//|  text           ┗┛ ┗┛ |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_2TEXT_2ANIM_AND_TEXT,//mary
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_2text_2anim_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_TEXT_LEFT_SPACE + MMI_MAINSCREEN_WIDTH * 2 / 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, // 1 第一行文字
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_TEXT_LEFT_SPACE + MMI_MAINSCREEN_WIDTH * 2 / 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, // 1 第一行文字
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE + MMI_MAINSCREEN_WIDTH * 2 / 5 + 5,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, // 1 第二行文字
            { MMI_LIST_TEXT_LEFT_SPACE + MMI_MAINSCREEN_WIDTH * 2 / 5 + 5,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1}, // 1 第二行文字
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH) - 2 * MMI_LIST_ICON_LEFT_SPACE, 1, MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, // 2 图片
            { MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH) - 2 * MMI_LIST_ICON_LEFT_SPACE, 1, MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, // 2 图片
            0, 0,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            0, 0,
            100, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第三行文字.
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第三行文字.
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//| ┏┓   @text  text           |
//| ┗┛   @text              |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_ICON_TWO_TEXT_AND_ICON_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_icon_two_text_and_icon_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},       /* ANIMATE */
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3},       /* ANIMATE */
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 8 , MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 8 , MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 },
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 14, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},   /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH - 14, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT + MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},   /* 1 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            0,
            {MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH * 3 / 5 + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8 - 30 + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            {MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_MAINSCREEN_WIDTH * 3 / 5 + MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 8 - 30 + MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 5, 15, MMI_LIST_ITEM_RIGHT - 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH - 5, 15, MMI_LIST_ITEM_RIGHT - 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
    },

    0, MMILIST_INVALID_ITEM_INDEX
};


#ifdef MMI_PDA_SUPPORT
//|---------------------------|
//|        text     text  @   |
//|        text           @   |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_2text_1icon_and_text_icon =
{
    MMI_ISTYLE_LIST_ITEM_HEIGHT, MMI_ISTYLE_LIST_ITEM_HEIGHT,

    {
        {
            GUILIST_STATE_TEXT_STYLE,
            { MMI_LIST_ICON_LEFT_SPACE, MMI_ISTYLE_LIST_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_BIG_ICON_WIDTH, MMI_ISTYLE_LIST_ITEM_HEIGHT},     /* 3 */
            { MMI_LIST_ICON_LEFT_SPACE, MMI_ISTYLE_LIST_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_BIG_ICON_WIDTH, MMI_ISTYLE_LIST_ITEM_HEIGHT},     /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 50
        },
        {
            GUILIST_STATE_TEXT_STYLE,
            { MMI_LIST_ICON_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_ISTYLE_LIST_ITEM_HEIGHT / 2 - 1 },     /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - 2 * MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_ISTYLE_LIST_ITEM_HEIGHT / 2 - 1 },     /* 1 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            50, 100
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH, MMI_ISTYLE_LIST_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT / 2, MMI_ISTYLE_LIST_ITEM_HEIGHT}, /* 2 */
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_BIG_ICON_WIDTH, MMI_ISTYLE_LIST_ITEM_HEIGHT / 2,  MMI_LIST_ITEM_RIGHT / 2, MMI_ISTYLE_LIST_ITEM_HEIGHT}, /* 2 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {0},
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_DEFAULT_FONT_ASCII_WIDTH * 10,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1 },   /* 4 */
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_DEFAULT_FONT_ASCII_WIDTH * 10,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1 },   /* 4 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH, 0, MMI_LIST_ITEM_RIGHT, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1},
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH, 0, MMI_LIST_ITEM_RIGHT, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            100, 100
        },
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|        text     text  @   |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_TEXT_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_text_icon =
{
    MMI_ISTYLE_LIST_ITEM_HEIGHT, MMI_ISTYLE_LIST_ITEM_HEIGHT,

    {
        {
            GUILIST_STATE_TEXT_STYLE,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT / 2,  MMI_ISTYLE_LIST_ITEM_HEIGHT - 1},     /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT / 2,  MMI_ISTYLE_LIST_ITEM_HEIGHT - 1},     /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 50
        },
        {
            GUILIST_STATE_TEXT_STYLE,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_RIGHT / 2 ,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1 },    /* 2 */
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_ITEM_RIGHT / 2 ,  0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1 },    /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, 0,  MMI_LIST_ITEM_RIGHT, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1 },       /* 3 */
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, 0,  MMI_LIST_ITEM_RIGHT, MMI_ISTYLE_LIST_ITEM_HEIGHT - 1 },       /* 3 */
            0, 0,
            100, 100
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|  @     text       text   @|
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_icon_text_text_icon =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            0,
            { 0, 0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_ITEM_HEIGHT },       /* 1 */
            { 0, 0,  MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_ITEM_HEIGHT },       /* 1 */
            0, 0,
            0, 0
        },
        {
            GUILIST_STATE_TEXT_STYLE,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH,  0,  MMI_MAINSCREEN_WIDTH / 3 * 2 - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_HEIGHT },   /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH,  0,  MMI_MAINSCREEN_WIDTH / 3 * 2 - MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_HEIGHT },   /* 2 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 50
        },
        {
            GUILIST_STATE_TEXT_STYLE,
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + MMI_LIST_ICON_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_ITEM_HEIGHT },   /* 3 */
            { MMI_MAINSCREEN_WIDTH / 3 * 2 + MMI_LIST_ICON_LEFT_SPACE, 0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, MMI_LIST_ITEM_HEIGHT },   /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT},       /* 4 */
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_SELECT_ICON_WIDTH, 0,  MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_HEIGHT},       /* 4 */
            0, 0,
            100, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

#endif

//|---------------------------|
//||_____| text               |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_LEFT_SPACE_TEXT,   //xiyuan edit can adjust left space of text
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_left_space_text =
{
    MMI_LIST_EXPAND_ITEM_HEIGHT, MMI_LIST_EXPAND_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE_PS,   0,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_EXPAND_ITEM_HEIGHT },       /* 1 */
            { MMI_LIST_TEXT_LEFT_SPACE_PS,   0,  MMI_LIST_ITEM_RIGHT,  MMI_LIST_EXPAND_ITEM_HEIGHT },       /* 1 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0}
    },

    0, MMILIST_INVALID_ITEM_INDEX
};



#endif

//GUIITEM_STYLE_TWO_LINE_TEXT_ANIM_AND_TEXT,//minghu for cr123283
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_anim_and_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,

    {
        {
            GUILIST_STATE_TEXT_STYLE,
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   // 1 第一行文字
            { MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_ICON_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1},   // 1 第一行文字
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            { MMI_LIST_ITEM_RIGHT - (MMI_LIST_ITEM_BIG_ICON_WIDTH), 1, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 },  // 2 图片
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第二行文字.
            { MMI_LIST_TEXT_LEFT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - 2 * (MMI_LIST_ITEM_BIG_ICON_WIDTH - MMI_LIST_TEXT_LEFT_SPACE), MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 - MMI_LIST_ITEM_TOP_MARGIN}, // 3 第二行文字.
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
    },

    0, 3
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT
#ifdef QBTHEME_SUPPORT
//|---------------------------|
//|        text               |
//|        text               |
//|        anim               |
//|---------------------------|
//GUIITEM_STYLE_THREE_LINE_TEXT_TEXT_ANIM,    //cheng.luo
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_3_text_text_anim =
{
    MMI_LIST_QBTHEME_ITEM_HEIGHT, MMI_LIST_QBTHEME_ITEM_HEIGHT,

    {
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, QB_FORM_MARGIN, MMI_LIST_ITEM_RIGHT, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE},
            { MMI_LIST_TEXT_LEFT_SPACE, QB_FORM_MARGIN, MMI_LIST_ITEM_RIGHT, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE},
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_TEXT_LEFT_SPACE, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE + MMI_LIST_HEIGHT_SPACE / 2 + MMI_DEFAULT_SMALL_FONT_SIZE},
            { MMI_LIST_TEXT_LEFT_SPACE, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE + MMI_LIST_HEIGHT_SPACE / 2 + MMI_DEFAULT_SMALL_FONT_SIZE},
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE + MMI_LIST_HEIGHT_SPACE + MMI_DEFAULT_SMALL_FONT_SIZE, MMI_LIST_ITEM_RIGHT, MMI_LIST_QBTHEME_ITEM_HEIGHT - QB_FORM_MARGIN },
            { MMI_LIST_ICON_LEFT_SPACE, QB_FORM_MARGIN + MMI_DEFAULT_NORMAL_FONT_SIZE + MMI_LIST_HEIGHT_SPACE + MMI_DEFAULT_SMALL_FONT_SIZE, MMI_LIST_ITEM_RIGHT, MMI_LIST_QBTHEME_ITEM_HEIGHT - QB_FORM_MARGIN },
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_MARK_WIDTH + 1, QB_FORM_MARGIN, MMI_LIST_ITEM_RIGHT, QB_FORM_MARGIN + MMI_LIST_QBTHEME_MARK_WIDTH - 1},
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_MARK_WIDTH + 1, QB_FORM_MARGIN, MMI_LIST_ITEM_RIGHT, QB_FORM_MARGIN + MMI_LIST_QBTHEME_MARK_WIDTH - 1},
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};


//|---------------------------|
//|                           |
//|   anim     text     icon  |
//|                           |
//|---------------------------|
//GUIITEM_STYLE_ONE_LINE_ANIM_TEXT_ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_anim_text_icon =
{
    MMI_LIST_QBTHEME_ITEM2_HEIGHT, MMI_LIST_QBTHEME_ITEM2_HEIGHT,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, (MMI_LIST_QBTHEME_ITEM2_HEIGHT - MMI_LIST_QBTHEME_ANIM_HEIGHT) / 2, MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_QBTHEME_ANIM_WIDTH - 1, (MMI_LIST_QBTHEME_ITEM2_HEIGHT + MMI_LIST_QBTHEME_ANIM_HEIGHT) / 2},
            { MMI_LIST_ICON_LEFT_SPACE, (MMI_LIST_QBTHEME_ITEM2_HEIGHT - MMI_LIST_QBTHEME_ANIM_HEIGHT) / 2, MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_QBTHEME_ANIM_WIDTH - 1, (MMI_LIST_QBTHEME_ITEM2_HEIGHT + MMI_LIST_QBTHEME_ANIM_HEIGHT) / 2},
            0, 0,
            0, 0
        },
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_QBTHEME_ANIM_WIDTH - 1 + MMI_LIST_QBTHEME_ITEM2_SPACE, MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_ICON_WIDTH - MMI_LIST_QBTHEME_ITEM2_SPACE * 2, MMI_LIST_QBTHEME_ITEM2_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            { MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_QBTHEME_ANIM_WIDTH - 1 + MMI_LIST_QBTHEME_ITEM2_SPACE, MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_ICON_WIDTH - MMI_LIST_QBTHEME_ITEM2_SPACE * 2, MMI_LIST_QBTHEME_ITEM2_HEIGHT - MMI_LIST_ITEM_BOTTOM_MARGIN},
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_ICON_WIDTH - MMI_LIST_QBTHEME_ITEM2_SPACE, (MMI_LIST_QBTHEME_ITEM2_HEIGHT - MMI_LIST_QBTHEME_ICON_WIDTH) / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_ITEM2_SPACE, (MMI_LIST_QBTHEME_ITEM2_HEIGHT + MMI_LIST_QBTHEME_ICON_WIDTH) / 2 },
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_ICON_WIDTH - MMI_LIST_QBTHEME_ITEM2_SPACE, (MMI_LIST_QBTHEME_ITEM2_HEIGHT - MMI_LIST_QBTHEME_ICON_WIDTH) / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_ITEM2_SPACE, (MMI_LIST_QBTHEME_ITEM2_HEIGHT + MMI_LIST_QBTHEME_ICON_WIDTH) / 2 },
            0, 0,
            100, 100
        },
        {
            0,
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_MARK_WIDTH + 1, MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_TOP_MARGIN + MMI_LIST_QBTHEME_MARK_WIDTH - 1},
            { MMI_LIST_ITEM_RIGHT - MMI_LIST_QBTHEME_MARK_WIDTH + 1, MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_ITEM_RIGHT, MMI_LIST_ITEM_TOP_MARGIN + MMI_LIST_QBTHEME_MARK_WIDTH - 1},
            0, 0,
            100, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

#endif
#endif
//|---------------------------|
//|   @     text            @ |
//|   @     text              |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_and_two_icon_text_and_icon_2 =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,
    {
        {
            0,
            {  0 , MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 3 / 4 },
            {  0 , MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 3 / 4 },
            0, 0,
            0, 0
        },
        {
            0,
            {  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            {  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ITEM_BIG_ICON_WIDTH, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            0,
            {  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT/3 + MMI_LIST_ITEM_SELECT_ICON_WIDTH ,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 1 */
            {  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT/3 + MMI_LIST_ITEM_SELECT_ICON_WIDTH ,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT }, /* 1 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
            0,
            {MMI_LIST_ITEM_RIGHT /3 + MMI_LIST_ITEM_SELECT_ICON_WIDTH + 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT/3 * 2 , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT },
            {MMI_LIST_ITEM_RIGHT /3 + MMI_LIST_ITEM_SELECT_ICON_WIDTH + 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT/3 * 2, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT },
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            {MMI_LIST_ITEM_RIGHT/3 * 2 + 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT },
            {MMI_LIST_ITEM_RIGHT/3 * 2 + 4, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE / 2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT },
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 6 , MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_ITEM_RIGHT , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE},
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 6 , MMI_LIST_ITEM_TOP_MARGIN, MMI_LIST_ITEM_RIGHT , MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 + MMI_LIST_HEIGHT_SPACE},
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            100, 100
        },
    },

    1, 3
};

//|---------------------------|
//|  @     text     text      |
//|        text               |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_NORMAL,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_icon_two_text_and_text_normal =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,
    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 },   /* 1 */
            { MMI_LIST_ICON_LEFT_SPACE, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT * 2 / 3 - 1 },   /* 1 */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 2 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 7, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            { MMI_LIST_ITEM_RIGHT - MMI_DEFAULT_FONT_ASCII_WIDTH * 7, MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2 - 1 }, /* 3 */
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            100, 100
        },
        {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_TOP_MARGIN},     /* 4 */
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + MMI_LIST_TEXT_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT / 2, MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - MMI_LIST_ITEM_TOP_MARGIN},     /* 4 */
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 100
        },
		{0},
        {0},
    },

    3, MMILIST_INVALID_ITEM_INDEX
};

#ifndef MMI_THEME_LIST_MINI_SUPPORT
#ifdef SEARCH_SUPPORT
//|---------------------------|
//|  @     text               |
//|---------------------------|
// GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_bigicon_text =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,
    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MENUICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2 ,  MENUICON_HEIGHT + 2 },  /* ANIMATE */
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MENUICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2 ,  MENUICON_HEIGHT + 2 },  /* ANIMATE */
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_CHECK,
            { MMI_LIST_ICON_LEFT_SPACE + MENUICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, /* 3 */
            { MMI_LIST_ICON_LEFT_SPACE + MENUICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_TOP_MARGIN,  MMI_LIST_ITEM_RIGHT - 10, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, /* 3 */
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif

//|---------------------------|
//| ┏┓   text     text      |
//| ┗┛   text               |
//|---------------------------|
//GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_2,
#ifdef MMI_PDA_SUPPORT
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_two_text_and_text_2 =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,
    {
        {
            0,
            { 1,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + 3,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, 
            { 1,   1,  MMI_LIST_ITEM_SELECT_ICON_WIDTH + 3,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, 
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + 6,  0,  MMI_MAINSCREEN_WIDTH/5*4 - 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1  }, 
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + 6,  0,  MMI_MAINSCREEN_WIDTH/5*4 - 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1  }, 
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH/5*4 - 6, 0,  MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1 }, 
            { MMI_MAINSCREEN_WIDTH/5*4 - 6, 0,  MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1 }, 
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + 6,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, 
            { MMI_LIST_ITEM_SELECT_ICON_WIDTH + 6,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, 
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#else
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_anim_two_text_and_text_2 =
{
    MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT,
    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, 
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 3 }, 
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2,  0,  MMI_MAINSCREEN_WIDTH/4*3 - 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1  }, 
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2,  0,  MMI_MAINSCREEN_WIDTH/4*3 - 7, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1  }, 
            MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT,
            0, 50
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH/4*3 - 6, 0,  MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1 }, 
            { MMI_MAINSCREEN_WIDTH/4*3 - 6, 0,  MMI_LIST_ITEM_RIGHT + 5, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2 - 1 }, 
            MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT,
            50, 100
        },
        {
            GUIITEM_CONTENT_STATE_EFFECT_STR,
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, 
            { MMI_LIST_ITEM_BIG_ICON_WIDTH + MMI_LIST_ICON_LEFT_SPACE + 2,  MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT/2, MMI_LIST_ITEM_RIGHT, MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT - 1 }, 
            MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            0, 100
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
#endif

#endif

//|---------------------------|
//|      text          icon   |
//|      text                |
//|---------------------------|
//GUIITEM_STYLE_TWO_TEXT_AND_ONE_ICON_MS
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2_text_and_1_icon_ms =
{
    78, 78,

    {
        	{
            		0,
            		{ WATCHLIST_ICON_TEXT_LEFT_SPACE+15, WATCHLIST_BG_ICON_TOP_SPACE - 2, WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID, WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            		{ WATCHLIST_ICON_TEXT_LEFT_SPACE+15, WATCHLIST_BG_ICON_TOP_SPACE - 2, WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID, WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            		MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
            		0, 0
        	},
        	{
			0,
			{ WATCHLIST_ICON_TEXT_LEFT_SPACE+15, WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2 - 6,  WATCHLIST_ICON_TEXT_LEFT_SPACE+15 +  WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
			{ WATCHLIST_ICON_TEXT_LEFT_SPACE+15, WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2 - 6,  WATCHLIST_ICON_TEXT_LEFT_SPACE+15 +  WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
			MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT,
			0, 0
		},
		{
			0,
			{ WATCHLIST_TEXT_WIDGH_MID, 8, WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MID + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
			{ WATCHLIST_TEXT_WIDGH_MID, 8, WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MID + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
			0, 0,
			0, 0
		},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//============================================================================//
//                                   For watch                                //
//============================================================================//
    //|---------------------------|
    //|   text       text   text  |
    //|---------------------------|
    //GUIITEM_STYLE_SMALL_3STR,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_small_3str =
{
    WATCHLIST_ITEM_HEIGHT_SMALL, WATCHLIST_ITEM_HEIGHT_SMALL,

    {
        {
            0,
            { WATCHLIST_SMALL_3TEXT_FIRST_LEFT_SPACE,   WATCHLIST_SMALL_3TEXT_TOP_SPACE,  WATCHLIST_SMALL_3TEXT_FIRST_LEFT_SPACE + WATCHLIST_SMALL_3TEXT_FIRST_WIDGH_SPACE , WATCHLIST_SMALL_3TEXT_BOTTOM_SPACE},
            { WATCHLIST_SMALL_3TEXT_FIRST_LEFT_SPACE,   WATCHLIST_SMALL_3TEXT_TOP_SPACE,  WATCHLIST_SMALL_3TEXT_FIRST_LEFT_SPACE + WATCHLIST_SMALL_3TEXT_FIRST_WIDGH_SPACE , WATCHLIST_SMALL_3TEXT_BOTTOM_SPACE},
            WATCHLIST_ITEM_NORMAL_FONT, WATCHLIST_ITEM_NORMAL_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { WATCHLIST_SMALL_3TEXT_SECOND_LEFT_SPACE,   WATCHLIST_SMALL_3TEXT_TOP_SPACE,  WATCHLIST_SMALL_3TEXT_SECOND_LEFT_SPACE + WATCHLIST_SMALL_3TEXT_SECOND_WIDGH_SPACE , WATCHLIST_SMALL_3TEXT_BOTTOM_SPACE},
            { WATCHLIST_SMALL_3TEXT_SECOND_LEFT_SPACE,   WATCHLIST_SMALL_3TEXT_TOP_SPACE,  WATCHLIST_SMALL_3TEXT_SECOND_LEFT_SPACE + WATCHLIST_SMALL_3TEXT_SECOND_WIDGH_SPACE , WATCHLIST_SMALL_3TEXT_BOTTOM_SPACE},
            WATCHLIST_ITEM_SMALL_FONT, WATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { WATCHLIST_SMALL_3TEXT_THIRD_LEFT_SPACE,   WATCHLIST_SMALL_3TEXT_TOP_SPACE,  WATCHLIST_SMALL_3TEXT_THIRD_LEFT_SPACE + WATCHLIST_SMALL_3TEXT_THIRD_WIDGH_SPACE , WATCHLIST_SMALL_3TEXT_BOTTOM_SPACE},
            { WATCHLIST_SMALL_3TEXT_THIRD_LEFT_SPACE,   WATCHLIST_SMALL_3TEXT_TOP_SPACE,  WATCHLIST_SMALL_3TEXT_THIRD_LEFT_SPACE + WATCHLIST_SMALL_3TEXT_THIRD_WIDGH_SPACE , WATCHLIST_SMALL_3TEXT_BOTTOM_SPACE},
            WATCHLIST_ITEM_SMALL_FONT, WATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|        text               |
    //|---------------------------|
    //GUIITEM_STYLE_1STR,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1str =
{
    WATCHLIST_ITEM_HEIGHT, WATCHLIST_ITEM_HEIGHT,

    {
        {
            0,
            { WATCHLIST_TEXT_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_BIG , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            { WATCHLIST_TEXT_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_BIG , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            WATCHLIST_ITEM_NORMAL_FONT, WATCHLIST_ITEM_NORMAL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|   @     text              |
    //|---------------------------|
    //GUIITEM_STYLE_1ICON_1STR,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1icon_1str =
{
    WATCHLIST_ITEM_HEIGHT, WATCHLIST_ITEM_HEIGHT,

    {
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            0, 0,
            0, 0
        },
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            WATCHLIST_ITEM_NORMAL_FONT, WATCHLIST_ITEM_NORMAL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
    //|---------------------------|
    //|      text             @   |
    //|---------------------------|
    //GUIITEM_STYLE_1STR_1ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1str_1icon =
{
    WATCHLIST_ITEM_HEIGHT, WATCHLIST_ITEM_HEIGHT,

    {
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE ,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE ,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            WATCHLIST_ITEM_NORMAL_FONT, WATCHLIST_ITEM_NORMAL_FONT,
            0, 0
        },
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MID + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MID + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MID + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MID + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            0, 0,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|   @     text       @text  |
    //|---------------------------|
    //GUIITEM_STYLE_1ICON_1STR_1ICON_1STR,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1icon_1str_1icon_1str =
{
    WATCHLIST_ITEM_HEIGHT, WATCHLIST_ITEM_HEIGHT,

    {
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            0, 0,
            0, 0
        },
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MIN , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MIN , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            WATCHLIST_ITEM_NORMAL_FONT, WATCHLIST_ITEM_NORMAL_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { WATCHLIST_SECOND_ICON_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE, WATCHLIST_BG_ICON_RIGHT_SPACE , WATCHLIST_BG_ICON_BOTTOM_SPACE },
            { WATCHLIST_SECOND_ICON_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE, WATCHLIST_BG_ICON_RIGHT_SPACE , WATCHLIST_BG_ICON_BOTTOM_SPACE },
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { WATCHLIST_SECOND_ICON_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE, WATCHLIST_BG_ICON_RIGHT_SPACE , WATCHLIST_BG_ICON_BOTTOM_SPACE },
            { WATCHLIST_SECOND_ICON_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE, WATCHLIST_BG_ICON_RIGHT_SPACE , WATCHLIST_BG_ICON_BOTTOM_SPACE },
            WATCHLIST_ITEM_SMALL_FONT, WATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX // Bug 1326755
};

    //|---------------------------|
    //|   @    text               |
    //|        text               |
    //|---------------------------|
    //GUIITEM_STYLE_1ICON_2STR,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1icon_2str =
{
    WATCHLIST_ITEM_HEIGHT, WATCHLIST_ITEM_HEIGHT,

    {
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            0, 0,
            0, 0
        },
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            WATCHLIST_ITEM_NORMAL_FONT, WATCHLIST_ITEM_NORMAL_FONT,
            0, 0
        },
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,    WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            WATCHLIST_ITEM_SMALL_FONT, WATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
    },

    1, 2 // Bug 1321373
};

    //|---------------------------|
    //|  text              @      |
    //|  text                     |
    //|---------------------------|
    //GUIITEM_STYLE_2STR_1ICON,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2str_1icon =
{
    WATCHLIST_ITEM_HEIGHT, WATCHLIST_ITEM_HEIGHT,

    {
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID, WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID, WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            WATCHLIST_ITEM_NORMAL_FONT, WATCHLIST_ITEM_NORMAL_FONT,
            0, 0
        },
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE,    WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2,  WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE,    WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2,  WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            WATCHLIST_ITEM_SMALL_FONT, WATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            0, 0,
            0, 0
        },
        {0},
        {0},
        {0},
    },

    0, 1
};

    //|---------------------------|
    //|  text                     |
    //|  text                     |
    //|---------------------------|
    //GUIITEM_STYLE_2STR,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2str =
{
    WATCHLIST_ITEM_HEIGHT, WATCHLIST_ITEM_HEIGHT,

    {
        {
            0,
            { WATCHLIST_TEXT_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_BIG , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            { WATCHLIST_TEXT_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_BIG , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            WATCHLIST_ITEM_NORMAL_FONT, WATCHLIST_ITEM_NORMAL_FONT,
            0, 0
        },
        {
            0,
            { WATCHLIST_TEXT_LEFT_SPACE,    WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_BIG , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            { WATCHLIST_TEXT_LEFT_SPACE,    WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_BIG , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            WATCHLIST_ITEM_SMALL_FONT, WATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, 1
};

    //|---------------------------|
    //|  text             text    |
    //|  text                     |
    //|---------------------------|
    //GUIITEM_STYLE_3STR,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_3str =
{
    WATCHLIST_ITEM_HEIGHT, WATCHLIST_ITEM_HEIGHT,

    {
        {
            0,
            { WATCHLIST_TEXT_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            { WATCHLIST_TEXT_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            WATCHLIST_ITEM_NORMAL_FONT, WATCHLIST_ITEM_NORMAL_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MID ,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_BG_ICON_RIGHT_SPACE ,  WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            { WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MID ,  WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_BG_ICON_RIGHT_SPACE,  WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            WATCHLIST_ITEM_SMALL_FONT, WATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
            0,
            { WATCHLIST_TEXT_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_BIG , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            { WATCHLIST_TEXT_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_BIG , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            WATCHLIST_ITEM_SMALL_FONT, WATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
    },

    0, 2
};

    //|---------------------------|
    //| @       text      text    |
    //|         text              |
    //|---------------------------|
    //GUIITEM_STYLE_1ICON_3STR,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1icon_3str =
{
    WATCHLIST_ITEM_HEIGHT, WATCHLIST_ITEM_HEIGHT,

    {
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            0, 0,
            0, 0
        },
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MIN , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MIN , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            WATCHLIST_ITEM_NORMAL_FONT, WATCHLIST_ITEM_NORMAL_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MIN ,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_BG_ICON_RIGHT_SPACE ,  WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MIN ,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_BG_ICON_RIGHT_SPACE ,  WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            WATCHLIST_ITEM_SMALL_FONT, WATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
            0,
            { WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_BIG , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            { WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_BIG , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            WATCHLIST_ITEM_SMALL_FONT, WATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
    },

    1, 3 // Bug 1326301
};

    
    //|---------------------------|
    //|  text   @text    text     |
    //|  text                     |
    //|---------------------------|
    //GUIITEM_STYLE_2STR_1ICON_2STR,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2str_1icon_2str =
{
    WATCHLIST_ITEM_HEIGHT, WATCHLIST_ITEM_HEIGHT,

    {
        {
            0,
            { WATCHLIST_TEXT_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MIN , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            { WATCHLIST_TEXT_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MIN , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            WATCHLIST_ITEM_NORMAL_FONT, WATCHLIST_ITEM_NORMAL_FONT,
            0, 0
        },
        {
            0,
            { WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MIN,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_SMALL , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            { WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MIN,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_SMALL , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            0, 0,
            0, 0
        },
        {
            0,
            { WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MIN + 5,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_SMALL , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            { WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_MIN + 5,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_SMALL , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            WATCHLIST_ITEM_SMALL_FONT, WATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_SMALL ,  WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_BG_ICON_RIGHT_SPACE ,  WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            { WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_SMALL ,  WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_BG_ICON_RIGHT_SPACE,  WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            WATCHLIST_ITEM_SMALL_FONT, WATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
            0,
            { WATCHLIST_TEXT_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_BIG , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            { WATCHLIST_TEXT_LEFT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2,  WATCHLIST_TEXT_LEFT_SPACE + WATCHLIST_TEXT_WIDGH_BIG , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            WATCHLIST_ITEM_SMALL_FONT, WATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|      text              √ |
    //|---------------------------|
    //GUIITEM_STYLE_1STR_CHECK,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1str_check =
{
    WATCHLIST_ITEM_HEIGHT, WATCHLIST_ITEM_HEIGHT,

    {
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE ,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE ,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            WATCHLIST_ITEM_NORMAL_FONT, WATCHLIST_ITEM_NORMAL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|      text            ⊙   |
    //|---------------------------|
    //GUIITEM_STYLE_1STR_RADIO,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1str_radio =
{
    WATCHLIST_ITEM_HEIGHT, WATCHLIST_ITEM_HEIGHT,

    {
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE ,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE ,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            WATCHLIST_ITEM_NORMAL_FONT, WATCHLIST_ITEM_NORMAL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|      text   |----->|      |
    //|---------------------------|
    //GUIITEM_STYLE_1STR_SWITCH,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1str_switch =
{
    WATCHLIST_ITEM_HEIGHT, WATCHLIST_ITEM_HEIGHT,

    {
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE ,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE ,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            WATCHLIST_ITEM_NORMAL_FONT, WATCHLIST_ITEM_NORMAL_FONT,
            0, 0
        },
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE +  WATCHLIST_TEXT_WIDGH_MID + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            0, 0,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
    //|---------------------------|
    //|   @     text              |
    //|---------------------------|
    //GUIITEM_STYLE_1ICON_1STR_DYNA,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1icon_1str_dyna =
{
    WATCHLIST_1STR_1LINE_HEIGHT, WATCHLIST_1STR_1LINE_HEIGHT,

    {
        {
            0,
            { WATCHLIST_LEFT_SPACE, (WATCHLIST_1STR_1LINE_HEIGHT - WATCHLIST_ICON_HEIGH)/2,  WATCHLIST_LEFT_SPACE + WATCHLIST_ICON_HEIGH,  (WATCHLIST_1STR_1LINE_HEIGHT - WATCHLIST_ICON_HEIGH)/2 + WATCHLIST_ICON_HEIGH},
            { WATCHLIST_LEFT_SPACE, (WATCHLIST_1STR_1LINE_HEIGHT - WATCHLIST_ICON_HEIGH)/2,  WATCHLIST_LEFT_SPACE + WATCHLIST_ICON_HEIGH,  (WATCHLIST_1STR_1LINE_HEIGHT - WATCHLIST_ICON_HEIGH)/2 + WATCHLIST_ICON_HEIGH},
            0, 0,
            0, 0
        },
        {
            0,
            { WATCHLIST_LEFT_SPACE + WATCHLIST_ICON_HEIGH + WATCHLIST_ICON_AND_TEXT_OFFESET, WATCHLIST_1LINE_TOP_SPACE,  WATCHLIST_RIGHT_SPACE, WATCHLIST_1STR_1LINE_HEIGHT - WATCHLIST_1LINE_BOTTOM_SPACE },
            { WATCHLIST_LEFT_SPACE + WATCHLIST_ICON_HEIGH + WATCHLIST_ICON_AND_TEXT_OFFESET, WATCHLIST_1LINE_TOP_SPACE,  WATCHLIST_RIGHT_SPACE, WATCHLIST_1STR_1LINE_HEIGHT - WATCHLIST_1LINE_BOTTOM_SPACE },
            MMI_DEFAULT_BIG_FONT, MMI_DEFAULT_BIG_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|    @   text               |
    //|        text               |
    //|          |@|     |@|      |
    //|---------------------------|
    //GUIITEM_STYLE_1ICON_2STR_2BUTTON_DYNA,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1icon_2str_2button_dyna =
 {
	WATCHLIST_ITEM_BIG_HEIGHT, WATCHLIST_ITEM_BIG_HEIGHT,
     {
         {
             0,
            { WATCHLIST_ICON_LEFT_SPACE, WATCHLIST_ICON_TOP_SPACE,  WATCHLIST_ICON_LEFT_SPACE + WATCHLIST_ICON_WIDGH_HEIGHT_SPACE,  WATCHLIST_ICON_TOP_SPACE + WATCHLIST_ICON_WIDGH_HEIGHT_SPACE},
            { WATCHLIST_ICON_LEFT_SPACE, WATCHLIST_ICON_TOP_SPACE,  WATCHLIST_ICON_LEFT_SPACE + WATCHLIST_ICON_WIDGH_HEIGHT_SPACE,  WATCHLIST_ICON_TOP_SPACE + WATCHLIST_ICON_WIDGH_HEIGHT_SPACE},
             0, 0,
             0, 0
         },
         {
             GUIITEM_CONTENT_STATE_ELLIPSIS,
            { WATCHLIST_ICON_LEFT_SPACE + WATCHLIST_ICON_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT, WATCHLIST_TEXT_TOP_SPACE, WATCHLIST_ICON_LEFT_SPACE + WATCHLIST_ICON_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT + WATCHLIST_TEXT_WIDGH_SPACE, WATCHLIST_TEXT_TOP_SPACE + WATCHLIST_1TEXT_HEIGHT_SPACE},
            { WATCHLIST_ICON_LEFT_SPACE + WATCHLIST_ICON_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT, WATCHLIST_TEXT_TOP_SPACE, WATCHLIST_ICON_LEFT_SPACE + WATCHLIST_ICON_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT + WATCHLIST_TEXT_WIDGH_SPACE, WATCHLIST_TEXT_TOP_SPACE + WATCHLIST_1TEXT_HEIGHT_SPACE },
            MMI_DEFAULT_BIG_FONT, MMI_DEFAULT_BIG_FONT,
             0, 100
         },
         {
            GUIITEM_CONTENT_STATE_ELLIPSIS,
            { WATCHLIST_ICON_LEFT_SPACE + WATCHLIST_ICON_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT, WATCHLIST_TEXT_TOP_SPACE + WATCHLIST_1TEXT_HEIGHT_SPACE + WATCHLIST_TEXT_TO_TEXT, WATCHLIST_ICON_LEFT_SPACE + WATCHLIST_ICON_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT + WATCHLIST_TEXT_WIDGH_SPACE, WATCHLIST_TEXT_TOP_SPACE + WATCHLIST_1TEXT_HEIGHT_SPACE + WATCHLIST_2TEXT_HEIGHT_SPACE},
            { WATCHLIST_ICON_LEFT_SPACE + WATCHLIST_ICON_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT, WATCHLIST_TEXT_TOP_SPACE + WATCHLIST_1TEXT_HEIGHT_SPACE + WATCHLIST_TEXT_TO_TEXT, WATCHLIST_ICON_LEFT_SPACE + WATCHLIST_ICON_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT + WATCHLIST_TEXT_WIDGH_SPACE, WATCHLIST_TEXT_TOP_SPACE + WATCHLIST_1TEXT_HEIGHT_SPACE + WATCHLIST_2TEXT_HEIGHT_SPACE},
            MMI_DEFAULT_NORMAL_FONT, MMI_DEFAULT_NORMAL_FONT,
             0, 100
         },
         {
             0,
            { WATCHLIST_BUTTON_LEFT_SPACE, WATCHLIST_BUTTON_TOP_SPACE, WATCHLIST_BUTTON_LEFT_SPACE + WATCHLIST_BUTTON_WIDGH_SPACE, WATCHLIST_BUTTON_TOP_SPACE + WATCHLIST_BUTTON_HEIGHT_SPACE },
            { WATCHLIST_BUTTON_LEFT_SPACE, WATCHLIST_BUTTON_TOP_SPACE, WATCHLIST_BUTTON_LEFT_SPACE + WATCHLIST_BUTTON_WIDGH_SPACE, WATCHLIST_BUTTON_TOP_SPACE + WATCHLIST_BUTTON_HEIGHT_SPACE },
            0, 0,
             0, 100
         },

         {
             0,
            {WATCHLIST_BUTTON_LEFT_SPACE + WATCHLIST_BUTTON_WIDGH_SPACE + WATCHLIST_BUTTON_TO_BUTTON, WATCHLIST_BUTTON_TOP_SPACE, WATCHLIST_BUTTON_LEFT_SPACE + WATCHLIST_BUTTON_WIDGH_SPACE + WATCHLIST_BUTTON_TO_BUTTON + WATCHLIST_BUTTON_WIDGH_SPACE, WATCHLIST_BUTTON_TOP_SPACE + WATCHLIST_BUTTON_HEIGHT_SPACE},
            {WATCHLIST_BUTTON_LEFT_SPACE + WATCHLIST_BUTTON_WIDGH_SPACE + WATCHLIST_BUTTON_TO_BUTTON, WATCHLIST_BUTTON_TOP_SPACE, WATCHLIST_BUTTON_LEFT_SPACE + WATCHLIST_BUTTON_WIDGH_SPACE + WATCHLIST_BUTTON_TO_BUTTON + WATCHLIST_BUTTON_WIDGH_SPACE, WATCHLIST_BUTTON_TOP_SPACE + WATCHLIST_BUTTON_HEIGHT_SPACE},
             0, 0,
             0, 100
         },

        {0},
     },
     0, MMILIST_INVALID_ITEM_INDEX
};

// watch commonlist start

    //|---------------------------|
    //|        text               |
    //|---------------------------|
    //GUIITEM_STYLE_1STR_1LINE_LAYOUT1,
LOCAL THEMELIST_ITEM_STYLE_T const s_1str_1line_layout1 =
{
    LIST_1STR_1LINE_ITEM_HEIGH, LIST_1STR_1LINE_ITEM_HEIGH,

    {
        {
            0,
            { LIST_1STR_1LINE_LEFT,  LIST_1STR_1LINE_TOP,  LIST_1STR_1LINE_RIGHT,  LIST_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1LINE_TOP},
            { LIST_1STR_1LINE_LEFT,  LIST_1STR_1LINE_TOP,  LIST_1STR_1LINE_RIGHT,  LIST_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1LINE_TOP},
            WATCH_LISTITEM_NORMAL_FONT, WATCH_LISTITEM_NORMAL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};


    //|---------------------------|
    //|   @     text              |
    //|---------------------------|
    //GUIITEM_STYLE_1ICON_1STR_1LINE_LAYOUT1,
LOCAL THEMELIST_ITEM_STYLE_T const s_1icon_1str_1line_layout1 =
{
    LIST_1ICON_1STR_1LINE_ITEM_HEIGH, LIST_1ICON_1STR_1LINE_ITEM_HEIGH,

    {
        {
            0,
            { LIST_1ICON_1STR_1LINE_LEFT,  (LIST_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1ICON_1STR_1LINE_ICON_HEIGH)/2,  LIST_1ICON_1STR_1LINE_LEFT + LIST_1ICON_1STR_1LINE_ICON_HEIGH,  (LIST_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1ICON_1STR_1LINE_ICON_HEIGH)/2 + LIST_1ICON_1STR_1LINE_ICON_HEIGH},
            { LIST_1ICON_1STR_1LINE_LEFT,  (LIST_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1ICON_1STR_1LINE_ICON_HEIGH)/2,  LIST_1ICON_1STR_1LINE_LEFT + LIST_1ICON_1STR_1LINE_ICON_HEIGH,  (LIST_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1ICON_1STR_1LINE_ICON_HEIGH)/2 + LIST_1ICON_1STR_1LINE_ICON_HEIGH},
            0, 0,
            0, 0
        },
        {
            0,
            { LIST_1ICON_1STR_1LINE_LEFT + LIST_1ICON_1STR_1LINE_ICON_HEIGH + LIST_1ICON_1STR_1LINE_ICON_AND_TEXT_OFFESET,  LIST_1ICON_1STR_1LINE_TOP,  LIST_1ICON_1STR_1LINE_RIGHT,  LIST_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1ICON_1STR_1LINE_TOP},
            { LIST_1ICON_1STR_1LINE_LEFT + LIST_1ICON_1STR_1LINE_ICON_HEIGH + LIST_1ICON_1STR_1LINE_ICON_AND_TEXT_OFFESET,  LIST_1ICON_1STR_1LINE_TOP,  LIST_1ICON_1STR_1LINE_RIGHT,  LIST_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1ICON_1STR_1LINE_TOP},
            WATCH_LISTITEM_NORMAL_FONT, WATCH_LISTITEM_NORMAL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
    //|---------------------------|
    //|      text             @   |
    //|---------------------------|
    //GUIITEM_STYLE_1STR_1ICON_1LINE_LAYOUT1,
LOCAL THEMELIST_ITEM_STYLE_T const s_1str_1icon_1line_layout1 =
{
    LIST_1STR_1ICON_1LINE_ITEM_HEIGH, LIST_1STR_1ICON_1LINE_ITEM_HEIGH,

    {
        {
            0,
            { LIST_1STR_1ICON_1LINE_LEFT,  LIST_1STR_1ICON_1LINE_TOP, LIST_1STR_1ICON_1LINE_LEFT + LIST_1STR_1ICON_1LINE_TEXT_WIDGH,  LIST_1STR_1ICON_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1LINE_TOP},
            { LIST_1STR_1ICON_1LINE_LEFT,  LIST_1STR_1ICON_1LINE_TOP, LIST_1STR_1ICON_1LINE_LEFT + LIST_1STR_1ICON_1LINE_TEXT_WIDGH,  LIST_1STR_1ICON_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1LINE_TOP},
            WATCH_LISTITEM_NORMAL_FONT, WATCH_LISTITEM_NORMAL_FONT,
            0, 0
        },
        {
            0,
            { LIST_1STR_1ICON_1LINE_LEFT + LIST_1STR_1ICON_1LINE_TEXT_WIDGH + LIST_1STR_1ICON_1LINE_TEXT_AND_ICON_OFFESET,  (LIST_1STR_1ICON_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1LINE_ICON_HEIGH)/2,  LIST_1STR_1ICON_1LINE_RIGHT,  (LIST_1STR_1ICON_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1LINE_ICON_HEIGH)/2 + LIST_1STR_1ICON_1LINE_ICON_HEIGH },
            { LIST_1STR_1ICON_1LINE_LEFT + LIST_1STR_1ICON_1LINE_TEXT_WIDGH + LIST_1STR_1ICON_1LINE_TEXT_AND_ICON_OFFESET,  (LIST_1STR_1ICON_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1LINE_ICON_HEIGH)/2,  LIST_1STR_1ICON_1LINE_RIGHT,  (LIST_1STR_1ICON_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1LINE_ICON_HEIGH)/2 + LIST_1STR_1ICON_1LINE_ICON_HEIGH },
            0, 0,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
    //|---------------------------|
    //|  text                     |
    //|  text                     |
    //|---------------------------|
    //GUIITEM_STYLE_2STR_2LINE_LAYOUT1,
LOCAL THEMELIST_ITEM_STYLE_T const s_2str_2line_layout1 =
{
    LIST_2STR_2LINE_ITEM_HEIGH, LIST_2STR_2LINE_ITEM_HEIGH,

    {
        {
            0,
            { LIST_2STR_2LINE_LEFT,  LIST_2STR_2LINE_TOP,  LIST_2STR_2LINE_RIGHT,  LIST_2STR_2LINE_TOP + LIST_2STR_2LINE_FIRST_LINE_HEIGH},
            { LIST_2STR_2LINE_LEFT,  LIST_2STR_2LINE_TOP,  LIST_2STR_2LINE_RIGHT,  LIST_2STR_2LINE_TOP + LIST_2STR_2LINE_FIRST_LINE_HEIGH},
            WATCH_LISTITEM_NORMAL_FONT, WATCH_LISTITEM_NORMAL_FONT,
            0, 0
        },
        {
            0,
            { LIST_2STR_2LINE_LEFT, LIST_2STR_2LINE_ITEM_HEIGH - LIST_2STR_2LINE_SECOND_LINE_HEIGH - LIST_2STR_2LINE_TOP,  LIST_2STR_2LINE_RIGHT,  LIST_2STR_2LINE_ITEM_HEIGH - LIST_2STR_2LINE_TOP},
            { LIST_2STR_2LINE_LEFT, LIST_2STR_2LINE_ITEM_HEIGH - LIST_2STR_2LINE_SECOND_LINE_HEIGH - LIST_2STR_2LINE_TOP,  LIST_2STR_2LINE_RIGHT,  LIST_2STR_2LINE_ITEM_HEIGH - LIST_2STR_2LINE_TOP},
            WATCH_LISTITEM_SMALL_FONT, WATCH_LISTITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|      text            ⊙   |
    //|---------------------------|
    //GUIITEM_STYLE_1STR_1LINE_RADIO_LAYOUT1,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1str_1line_radio_layout1 =
{
    LIST_1STR_1LINE_RADIO_ITEM_HEIGH, LIST_1STR_1LINE_RADIO_ITEM_HEIGH,

    {
        {
            0,
            { LIST_1STR_1LINE_RADIO_LEFT,  LIST_1STR_1LINE_RADIO_TOP,  LIST_1STR_1LINE_RADIO_LEFT + LIST_1STR_1LINE_RADIO_TEXT_WIDGH , LIST_1STR_1LINE_RADIO_ITEM_HEIGH - LIST_1STR_1LINE_RADIO_TOP},
            { LIST_1STR_1LINE_RADIO_LEFT,  LIST_1STR_1LINE_RADIO_TOP,  LIST_1STR_1LINE_RADIO_LEFT + LIST_1STR_1LINE_RADIO_TEXT_WIDGH , LIST_1STR_1LINE_RADIO_ITEM_HEIGH - LIST_1STR_1LINE_RADIO_TOP},
            WATCH_LISTITEM_NORMAL_FONT, WATCH_LISTITEM_NORMAL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|      text              √  |
    //|---------------------------|
    //GUIITEM_STYLE_1STR_1LINE_CHECK_LAYOUT1,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1str_1line_check_layout1 =
{
    LIST_1STR_1LINE_CHECK_ITEM_HEIGH, LIST_1STR_1LINE_CHECK_ITEM_HEIGH,

    {
        {
            0,
            { LIST_1STR_1LINE_CHECK_LEFT ,   LIST_1STR_1LINE_CHECK_TOP,  LIST_1STR_1LINE_CHECK_LEFT + LIST_1STR_1LINE_CHECK_TEXT_WIDGH  , LIST_1STR_1LINE_CHECK_ITEM_HEIGH - LIST_1STR_1LINE_CHECK_TOP},
            { LIST_1STR_1LINE_CHECK_LEFT ,   LIST_1STR_1LINE_CHECK_TOP,  LIST_1STR_1LINE_CHECK_LEFT + LIST_1STR_1LINE_CHECK_TEXT_WIDGH  , LIST_1STR_1LINE_CHECK_ITEM_HEIGH - LIST_1STR_1LINE_CHECK_TOP},
            WATCH_LISTITEM_NORMAL_FONT, WATCH_LISTITEM_NORMAL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|  text              @      |
    //|  text                     |
    //|---------------------------|
    //GUIITEM_STYLE_2STR_1ICON_2LINE_LAYOUT1,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2str_1icon_2line_layout1 =
{
    LIST_2STR_1ICON_2LINE_ITEM_HEIGH, LIST_2STR_1ICON_2LINE_ITEM_HEIGH,

    {
        {
            0,
            { LIST_2STR_1ICON_2LINE_LEFT,   LIST_2STR_1ICON_2LINE_TOP,  LIST_2STR_1ICON_2LINE_LEFT + LIST_2STR_1ICON_2LINE_TEXT_WIDGH, LIST_2STR_1ICON_2LINE_TOP + LIST_2STR_1ICON_2LINE_FIRST_LINE_HEIGH},
            { LIST_2STR_1ICON_2LINE_LEFT,   LIST_2STR_1ICON_2LINE_TOP,  LIST_2STR_1ICON_2LINE_LEFT + LIST_2STR_1ICON_2LINE_TEXT_WIDGH, LIST_2STR_1ICON_2LINE_TOP + LIST_2STR_1ICON_2LINE_FIRST_LINE_HEIGH},
            WATCH_LISTITEM_NORMAL_FONT, WATCH_LISTITEM_NORMAL_FONT,
            0, 0
        },
        {
            0,
            { LIST_2STR_1ICON_2LINE_LEFT,   LIST_2STR_1ICON_2LINE_TOP + LIST_2STR_1ICON_2LINE_FIRST_LINE_HEIGH + LIST_2STR_1ICON_2LINE_TEXT_OFFESET,  LIST_2STR_1ICON_2LINE_LEFT + LIST_2STR_1ICON_2LINE_TEXT_WIDGH , LIST_2STR_1ICON_2LINE_ITEM_HEIGH - LIST_2STR_1ICON_2LINE_TOP},
            { LIST_2STR_1ICON_2LINE_LEFT,   LIST_2STR_1ICON_2LINE_TOP + LIST_2STR_1ICON_2LINE_FIRST_LINE_HEIGH + LIST_2STR_1ICON_2LINE_TEXT_OFFESET,  LIST_2STR_1ICON_2LINE_LEFT + LIST_2STR_1ICON_2LINE_TEXT_WIDGH , LIST_2STR_1ICON_2LINE_ITEM_HEIGH - LIST_2STR_1ICON_2LINE_TOP},
            WATCH_LISTITEM_SMALL_FONT, WATCH_LISTITEM_SMALL_FONT,
            0, 0
        },
        {
            0,
            { LIST_2STR_1ICON_2LINE_RIGHT - LIST_2STR_1ICON_2LINE_ICON_HEIGH, (LIST_2STR_1ICON_2LINE_ITEM_HEIGH - LIST_2STR_1ICON_2LINE_ICON_HEIGH)/2, LIST_2STR_1ICON_2LINE_RIGHT,(LIST_2STR_1ICON_2LINE_ITEM_HEIGH - LIST_2STR_1ICON_2LINE_ICON_HEIGH)/2 + LIST_2STR_1ICON_2LINE_ICON_HEIGH},
            { LIST_2STR_1ICON_2LINE_RIGHT - LIST_2STR_1ICON_2LINE_ICON_HEIGH, (LIST_2STR_1ICON_2LINE_ITEM_HEIGH - LIST_2STR_1ICON_2LINE_ICON_HEIGH)/2, LIST_2STR_1ICON_2LINE_RIGHT,(LIST_2STR_1ICON_2LINE_ITEM_HEIGH - LIST_2STR_1ICON_2LINE_ICON_HEIGH)/2 + LIST_2STR_1ICON_2LINE_ICON_HEIGH},
            0, 0,
            0, 0
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|  text             text    |
    //|  text                     |
    //|---------------------------|
    //GUIITEM_STYLE_3STR_2LINE_LAYOUT1,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_3str_2line_layout1 =
{
    LIST_3STR_2LINE_ITEM_HEIGH, LIST_3STR_2LINE_ITEM_HEIGH,

    {
        {
            0,
            { LIST_3STR_2LINE_LEFT,  LIST_3STR_2LINE_TOP,  LIST_3STR_2LINE_LEFT + LIST_3STR_2LINE_LEFT_TEXT_WIDGH, LIST_3STR_2LINE_TOP + LIST_3STR_2LINE_FIRST_LINE_HEIGH},
            { LIST_3STR_2LINE_LEFT,  LIST_3STR_2LINE_TOP,  LIST_3STR_2LINE_LEFT + LIST_3STR_2LINE_LEFT_TEXT_WIDGH, LIST_3STR_2LINE_TOP + LIST_3STR_2LINE_FIRST_LINE_HEIGH},
            WATCH_LISTITEM_NORMAL_FONT, WATCH_LISTITEM_NORMAL_FONT,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_R_ALIGN,
            { LIST_3STR_2LINE_LEFT + LIST_3STR_2LINE_LEFT_TEXT_WIDGH,  LIST_3STR_2LINE_TOP + LIST_3STR_2LINE_TOP_OFFESET,  LIST_3STR_2LINE_RIGHT, LIST_3STR_2LINE_TOP + LIST_3STR_2LINE_FIRST_LINE_HEIGH - LIST_3STR_2LINE_TOP_OFFESET},
            { LIST_3STR_2LINE_LEFT + LIST_3STR_2LINE_LEFT_TEXT_WIDGH,  LIST_3STR_2LINE_TOP + LIST_3STR_2LINE_TOP_OFFESET,  LIST_3STR_2LINE_RIGHT, LIST_3STR_2LINE_TOP + LIST_3STR_2LINE_FIRST_LINE_HEIGH - LIST_3STR_2LINE_TOP_OFFESET},
            WATCH_LISTITEM_SMALL_FONT, WATCH_LISTITEM_SMALL_FONT,
            0, 0
        },
        {
            0,
            { LIST_3STR_2LINE_LEFT,  LIST_3STR_2LINE_TOP + LIST_3STR_2LINE_FIRST_LINE_HEIGH + LIST_3STR_2LINE_LEFT_TEXT_OFFESET,  LIST_3STR_2LINE_RIGHT, LIST_3STR_2LINE_ITEM_HEIGH - LIST_3STR_2LINE_TOP},
            { LIST_3STR_2LINE_LEFT,  LIST_3STR_2LINE_TOP + LIST_3STR_2LINE_FIRST_LINE_HEIGH + LIST_3STR_2LINE_LEFT_TEXT_OFFESET,  LIST_3STR_2LINE_RIGHT, LIST_3STR_2LINE_ITEM_HEIGH - LIST_3STR_2LINE_TOP},
            WATCH_LISTITEM_SMALL_FONT, WATCH_LISTITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|    text       @text       |
    //|---------------------------|
    //GUIITEM_STYLE_1STR_1ICON_1STR_1LINE_LAYOUT1,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1str_1icon_1str_1line_layout1 =
{
    LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH, LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH,

    {
        {
            0,
            { LIST_1STR_1ICON_1STR_1LINE_LEFT,   LIST_1STR_1ICON_1STR_1LINE_TOP, LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH, LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_TOP },
            { LIST_1STR_1ICON_1STR_1LINE_LEFT,   LIST_1STR_1ICON_1STR_1LINE_TOP, LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH, LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_TOP },
            WATCH_LISTITEM_NORMAL_FONT, WATCH_LISTITEM_NORMAL_FONT,
            0, 0
        },
        {
            0,
            { LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH, (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 ,  LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH + LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH , LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH + (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 },
            { LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH, (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 ,  LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH + LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH , LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH + (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 },
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH, (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 ,  LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH + LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH , LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH + (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 },
            { LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH, (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 ,  LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH + LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH , LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH + (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 },
            WATCH_LISTITEM_SMALL_FONT, WATCH_LISTITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
    },
    1, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|    text       @text       |
    //|---------------------------|
    //GUIITEM_STYLE_1STR_1ICON_1STR_1LINE_LAYOUT2,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1str_1icon_1str_1line_layout2 =
{
    LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH, LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH,

    {
        {
            0,
            { LIST_1STR_1ICON_1STR_1LINE_LEFT,   LIST_1STR_1ICON_1STR_1LINE_TOP, LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH_SMALL, LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_TOP },
            { LIST_1STR_1ICON_1STR_1LINE_LEFT,   LIST_1STR_1ICON_1STR_1LINE_TOP, LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH_SMALL, LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_TOP },
            WATCH_LISTITEM_NORMAL_FONT, WATCH_LISTITEM_NORMAL_FONT,
            0, 0
        },
        {
            0,
            { LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH_SMALL, (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 ,  LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH_SMALL + LIST_1STR_1ICON_1STR_1LINE_ICON_WIDGH , LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH + (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 },
            { LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH_SMALL, (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 ,  LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH_SMALL + LIST_1STR_1ICON_1STR_1LINE_ICON_WIDGH , LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH + (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 },
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH_SMALL, (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 ,  LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH_SMALL + LIST_1STR_1ICON_1STR_1LINE_ICON_WIDGH , LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH + (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 },
            { LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH_SMALL, (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 ,  LIST_1STR_1ICON_1STR_1LINE_LEFT + LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH_SMALL + LIST_1STR_1ICON_1STR_1LINE_ICON_WIDGH , LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH + (LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH)/2 },
            WATCH_LISTITEM_SMALL_FONT, WATCH_LISTITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
    },
    1, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|   @    text      @        |
    //|        text               |
    //|---------------------------|
    //GUIITEM_STYLE_1ICON_2STR,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1icon_2str_1icon =
{
    WATCHLIST_ITEM_HEIGHT, WATCHLIST_ITEM_HEIGHT,

    {
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            0, 0,
            0, 0
        },
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2},
            WATCHLIST_ITEM_NORMAL_FONT, WATCHLIST_ITEM_NORMAL_FONT,
            0, 0
        },
        {
            0,
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,    WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            { WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE,   WATCHLIST_BG_ICON_TOP_SPACE + (WATCHLIST_BG_ICON_BOTTOM_SPACE - WATCHLIST_BG_ICON_TOP_SPACE)/2,  WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE + WATCHLIST_TEXT_WIDGH_MID , WATCHLIST_BG_ICON_BOTTOM_SPACE},
            WATCHLIST_ITEM_SMALL_FONT, WATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
             0,
            { MMI_LIST_ITEM_RIGHT - WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE - MMI_LIST_RIGHT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            { MMI_LIST_ITEM_RIGHT - WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE - MMI_LIST_RIGHT_SPACE,   WATCHLIST_ICON_TEXT_TOP_SPACE,  MMI_LIST_ITEM_RIGHT - MMI_LIST_RIGHT_SPACE , WATCHLIST_ICON_TEXT_TOP_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE},
            0, 0,
             0, 0
         },
        {0},
        {0},
    },

    1, 2 // Bug 1321373
};

// watch commonlist end

#ifdef ZTE_WATCH
    //|---------------------------|
    //|   @     text              |
    //|---------------------------|
    //GUIITEM_STYLE_1ICON_1STR_1LINE_WITH_UNREAD_NUM,
LOCAL THEMELIST_ITEM_STYLE_T const s_1icon_1str_1line_with_unread_num =
{
    LIST_1ICON_1STR_1LINE_ITEM_HEIGH, LIST_1ICON_1STR_1LINE_ITEM_HEIGH,

    {
        {
            0,
            { LIST_1ICON_1STR_1LINE_LEFT,  (LIST_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1ICON_1STR_1LINE_ICON_HEIGH)/2,  LIST_1ICON_1STR_1LINE_LEFT + LIST_1ICON_1STR_1LINE_ICON_HEIGH,  (LIST_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1ICON_1STR_1LINE_ICON_HEIGH)/2 + LIST_1ICON_1STR_1LINE_ICON_HEIGH},
            { LIST_1ICON_1STR_1LINE_LEFT,  (LIST_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1ICON_1STR_1LINE_ICON_HEIGH)/2,  LIST_1ICON_1STR_1LINE_LEFT + LIST_1ICON_1STR_1LINE_ICON_HEIGH,  (LIST_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1ICON_1STR_1LINE_ICON_HEIGH)/2 + LIST_1ICON_1STR_1LINE_ICON_HEIGH},
            0, 0,
            0, 0
        },
        {
            0,
            { LIST_1ICON_1STR_1LINE_LEFT + LIST_1ICON_1STR_1LINE_ICON_HEIGH + LIST_1ICON_1STR_1LINE_ICON_AND_TEXT_OFFESET,  LIST_1ICON_1STR_1LINE_TOP,  LIST_1ICON_1STR_1LINE_RIGHT,  LIST_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1ICON_1STR_1LINE_TOP},
            { LIST_1ICON_1STR_1LINE_LEFT + LIST_1ICON_1STR_1LINE_ICON_HEIGH + LIST_1ICON_1STR_1LINE_ICON_AND_TEXT_OFFESET,  LIST_1ICON_1STR_1LINE_TOP,  LIST_1ICON_1STR_1LINE_RIGHT,  LIST_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1ICON_1STR_1LINE_TOP},
            WATCH_LISTITEM_NORMAL_FONT, WATCH_LISTITEM_NORMAL_FONT,
            0, 0
        },
        {
            0,
            { LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_LEFT, LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_TOP,  LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_LEFT + LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_WIDGH,LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_TOP+LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_HEIGH},
            { LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_LEFT, LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_TOP,  LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_LEFT + LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_WIDGH,LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_TOP+LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_HEIGH},
            0, 0,
            0, 0
        },
        {
            0,
            { LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_TEXT_LEFT, LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_TOP,  LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_TEXT_LEFT + LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_WIDGH,LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_TOP+LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_HEIGH},
            { LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_TEXT_LEFT, LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_TOP,  LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_TEXT_LEFT + LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_WIDGH,LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_TOP+LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_HEIGH},
            WATCH_LISTITEM_SMALL_FONT, WATCH_LISTITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
#endif

#ifdef ADULT_WATCH_SUPPORT
    //|---------------------------|
    //|  text                     |
    //|  text                     |
    //|  text                     |
    //|---------------------------|
    //GUIITEM_STYLE_3STR_3LINE_ADULTWATCH,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_3str_3line_adultwatch =
{
    ADULTWATCHLIST_3STR_3LINE_HEIGHT, ADULTWATCHLIST_3STR_3LINE_HEIGHT,

    {
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_TOP_SPACE, ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH},
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_TOP_SPACE, ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH},
            ADULTWATCHLIST_ITEM_BIG_FONT, ADULTWATCHLIST_ITEM_BIG_FONT,
            0, 0
        },
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET, ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET + ADULTWATCHLIST_SECOND_LINE_HEIGH},
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET, ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET + ADULTWATCHLIST_SECOND_LINE_HEIGH},
            ADULTWATCHLIST_ITEM_SMALL_FONT, ADULTWATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET*2 + ADULTWATCHLIST_SECOND_LINE_HEIGH, ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET*2 + ADULTWATCHLIST_SECOND_LINE_HEIGH*2},
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET*2 + ADULTWATCHLIST_SECOND_LINE_HEIGH, ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET*2 + ADULTWATCHLIST_SECOND_LINE_HEIGH*2},
            ADULTWATCHLIST_ITEM_SMALL_FONT, ADULTWATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
    //|---------------------------|
    //|  text                     |
    //|  text                     |
    //|---------------------------|
    //GUIITEM_STYLE_2STR_2LINE_ADULTWATCH,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2str_2line_adultwatch =
{
    ADULTWATCHLIST_2STR_2LINE_HEIGHT, ADULTWATCHLIST_2STR_2LINE_HEIGHT,

    {
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_2LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH},
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_2LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH},
            ADULTWATCHLIST_ITEM_BIG_FONT, ADULTWATCHLIST_ITEM_BIG_FONT,
            0, 0
        },
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET,  ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET + ADULTWATCHLIST_SECOND_LINE_HEIGH},
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET,  ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET + ADULTWATCHLIST_SECOND_LINE_HEIGH},
            ADULTWATCHLIST_ITEM_SMALL_FONT, ADULTWATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
    
    //|---------------------------|
    //|  text         text        |
    //|  text                     |
    //|  text                     |
    //|---------------------------|
    //GUIITEM_STYLE_4STR_3LINE_ADULTWATCH,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_4str_3line_adultwatch =
{
    ADULTWATCHLIST_3STR_3LINE_HEIGHT, ADULTWATCHLIST_3STR_3LINE_HEIGHT,

    {
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_TOP_SPACE, ADULTWATCHLIST_3LINE_SENCOND_WORD_LEFT , ADULTWATCHLIST_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH},
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_TOP_SPACE, ADULTWATCHLIST_3LINE_SENCOND_WORD_LEFT , ADULTWATCHLIST_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH},
            ADULTWATCHLIST_ITEM_BIG_FONT, ADULTWATCHLIST_ITEM_BIG_FONT,
            0, 0
        },
        {
            0,
            { ADULTWATCHLIST_3LINE_SENCOND_WORD_LEFT + ADULTWATCHLIST_TOP_SPACE,   ADULTWATCHLIST_TOP_SPACE, ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH},
            { ADULTWATCHLIST_3LINE_SENCOND_WORD_LEFT + ADULTWATCHLIST_TOP_SPACE,   ADULTWATCHLIST_TOP_SPACE, ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH},
            ADULTWATCHLIST_ITEM_BIG_FONT, ADULTWATCHLIST_ITEM_BIG_FONT,
            0, 0
        },
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET, ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET + ADULTWATCHLIST_SECOND_LINE_HEIGH},
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET, ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET + ADULTWATCHLIST_SECOND_LINE_HEIGH},
            ADULTWATCHLIST_ITEM_SMALL_FONT, ADULTWATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET*2 + ADULTWATCHLIST_SECOND_LINE_HEIGH, ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET*2 + ADULTWATCHLIST_SECOND_LINE_HEIGH*2},
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET*2 + ADULTWATCHLIST_SECOND_LINE_HEIGH, ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_TOP_SPACE +ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET*2 + ADULTWATCHLIST_SECOND_LINE_HEIGH*2},
            ADULTWATCHLIST_ITEM_SMALL_FONT, ADULTWATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
    //|---------------------------|
    //|   @    text               |
    //|        text               |
    //|---------------------------|
    //GUIITEM_STYLE_1ICON_2STR_2LINE_ADULTWATCH,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1icon_2str_2line_adultwatch =
{
    ADULTWATCHLIST_2STR_2LINE_HEIGHT, ADULTWATCHLIST_2STR_2LINE_HEIGHT,

    {
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE,   (ADULTWATCHLIST_2STR_2LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2,  ADULTWATCHLIST_LEFT_SPACE + ADULTWATCHLIST_ICON_HEIGH, (ADULTWATCHLIST_2STR_2LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2 + ADULTWATCHLIST_ICON_HEIGH},
            { ADULTWATCHLIST_LEFT_SPACE,   (ADULTWATCHLIST_2STR_2LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2,  ADULTWATCHLIST_LEFT_SPACE + ADULTWATCHLIST_ICON_HEIGH, (ADULTWATCHLIST_2STR_2LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2 + ADULTWATCHLIST_ICON_HEIGH},
            0, 0,
            0, 0
        },
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE + ADULTWATCHLIST_ICON_HEIGH + ADULTWATCHLIST_ICON_AND_TEXT_OFFESET,   ADULTWATCHLIST_2LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH},
            { ADULTWATCHLIST_LEFT_SPACE + ADULTWATCHLIST_ICON_HEIGH + ADULTWATCHLIST_ICON_AND_TEXT_OFFESET,   ADULTWATCHLIST_2LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH},
            ADULTWATCHLIST_ITEM_BIG_FONT, ADULTWATCHLIST_ITEM_BIG_FONT,
            0, 0
        },
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE + ADULTWATCHLIST_ICON_HEIGH + ADULTWATCHLIST_ICON_AND_TEXT_OFFESET,   ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET,  ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET + ADULTWATCHLIST_SECOND_LINE_HEIGH},
            { ADULTWATCHLIST_LEFT_SPACE + ADULTWATCHLIST_ICON_HEIGH + ADULTWATCHLIST_ICON_AND_TEXT_OFFESET,   ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET,  ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET + ADULTWATCHLIST_SECOND_LINE_HEIGH},
            ADULTWATCHLIST_ITEM_SMALL_FONT, ADULTWATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|  text              @      |
    //|  text                     |
    //|---------------------------|
    //GUIITEM_STYLE_2STR_1ICON_2LINE_ADULTWATCH,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2str_1icon_2line_adultwatch =
{
    ADULTWATCHLIST_2STR_2LINE_HEIGHT, ADULTWATCHLIST_2STR_2LINE_HEIGHT,

    {
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_2LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_ICON_HEIGH - ADULTWATCHLIST_ICON_AND_TEXT_OFFESET, ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH},
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_2LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_ICON_HEIGH - ADULTWATCHLIST_ICON_AND_TEXT_OFFESET, ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH},
            ADULTWATCHLIST_ITEM_BIG_FONT, ADULTWATCHLIST_ITEM_BIG_FONT,
            0, 0
        },
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET,  ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_ICON_HEIGH - ADULTWATCHLIST_ICON_AND_TEXT_OFFESET , ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET + ADULTWATCHLIST_SECOND_LINE_HEIGH},
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET,  ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_ICON_HEIGH - ADULTWATCHLIST_ICON_AND_TEXT_OFFESET , ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET + ADULTWATCHLIST_SECOND_LINE_HEIGH},
            ADULTWATCHLIST_ITEM_SMALL_FONT, ADULTWATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {
            0,
            { ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_ICON_HEIGH, (ADULTWATCHLIST_2STR_2LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2 , ADULTWATCHLIST_RIGHT_SPACE  ,(ADULTWATCHLIST_2STR_2LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2 + ADULTWATCHLIST_ICON_HEIGH},
            { ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_ICON_HEIGH, (ADULTWATCHLIST_2STR_2LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2 , ADULTWATCHLIST_RIGHT_SPACE  ,(ADULTWATCHLIST_2STR_2LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2 + ADULTWATCHLIST_ICON_HEIGH},
            0, 0,
            0, 0
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|     text                  |
//|---------------------------|
//GUIITEM_STYLE_1STR_1LINE_LEFTALIGN_ADULTWATCH,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1str_1line_leftalign_adultwatch =
{
    ADULTWATCHLIST_1STR_1LINE_HEIGHT, ADULTWATCHLIST_1STR_1LINE_HEIGHT,

    {
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE,  ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE,  ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            ADULTWATCHLIST_ITEM_BIG_FONT, ADULTWATCHLIST_ITEM_BIG_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|           text            |
//|---------------------------|
//GUIITEM_STYLE_1STR_1LINE_CENTERALIGN_ADULTWATCH,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1str_1line_centeralign_adultwatch =
{
    ADULTWATCHLIST_1STR_1LINE_HEIGHT, ADULTWATCHLIST_1STR_1LINE_HEIGHT,

    {
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE,  ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE,  ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            ADULTWATCHLIST_ITEM_BIG_FONT, ADULTWATCHLIST_ITEM_BIG_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|        text          @    |
//|---------------------------|
//GUIITEM_STYLE_1STR_1ICON_1LINE_ADULTWATCH,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1str_1icon_1line_adultwatch =
{
    ADULTWATCHLIST_1STR_1LINE_HEIGHT, ADULTWATCHLIST_1STR_1LINE_HEIGHT,

    {
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE, ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_ICON_HEIGH - ADULTWATCHLIST_ICON_AND_TEXT_OFFESET, ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            { ADULTWATCHLIST_LEFT_SPACE, ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_ICON_HEIGH - ADULTWATCHLIST_ICON_AND_TEXT_OFFESET, ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            ADULTWATCHLIST_ITEM_BIG_FONT, ADULTWATCHLIST_ITEM_BIG_FONT,
            0, 0
        },
        {
            0,
            { ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_ICON_HEIGH, (ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2,  ADULTWATCHLIST_RIGHT_SPACE,  (ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2 + ADULTWATCHLIST_ICON_HEIGH},
            { ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_ICON_HEIGH, (ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2,  ADULTWATCHLIST_RIGHT_SPACE,  (ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2 + ADULTWATCHLIST_ICON_HEIGH},
            0, 0,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|    @         text         |
//|---------------------------|
//GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1icon_1str_1line_adultwatch =
{
    ADULTWATCHLIST_1STR_1LINE_HEIGHT, ADULTWATCHLIST_1STR_1LINE_HEIGHT,

    {
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE, (ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2,  ADULTWATCHLIST_LEFT_SPACE + ADULTWATCHLIST_ICON_HEIGH,  (ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2 + ADULTWATCHLIST_ICON_HEIGH},
            { ADULTWATCHLIST_LEFT_SPACE, (ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2,  ADULTWATCHLIST_LEFT_SPACE + ADULTWATCHLIST_ICON_HEIGH,  (ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2 + ADULTWATCHLIST_ICON_HEIGH},
            0, 0,
            0, 0
        },
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE + ADULTWATCHLIST_ICON_HEIGH + ADULTWATCHLIST_ICON_AND_TEXT_OFFESET, ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE, ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            { ADULTWATCHLIST_LEFT_SPACE + ADULTWATCHLIST_ICON_HEIGH + ADULTWATCHLIST_ICON_AND_TEXT_OFFESET, ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE, ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            ADULTWATCHLIST_ITEM_BIG_FONT, ADULTWATCHLIST_ITEM_BIG_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|   ⊙         text         |
    //|---------------------------|
    //GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,
    //此Radio是相对布局，ADULTWATCHLIST_RADIO_LEFT_SPACE是字串与⊙的Space
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1str_1line_radio_adultwatch =
{
    ADULTWATCHLIST_1STR_1LINE_HEIGHT, ADULTWATCHLIST_1STR_1LINE_HEIGHT,

    {
        {
            0,
            {ADULTWATCHLIST_RADIO_LEFT_SPACE, ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE, ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            {ADULTWATCHLIST_RADIO_LEFT_SPACE, ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE, ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            ADULTWATCHLIST_ITEM_BIG_FONT, ADULTWATCHLIST_ITEM_BIG_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
    //|---------------------------|
    //|      text         ⊙      |
    //|---------------------------|
    //GUIITEM_STYLE_1STR_1LINE_RADIO_RIGHT_ADULTWATCH,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1str_1line_radio_right_adultwatch =
{
    ADULTWATCHLIST_1STR_1LINE_HEIGHT, ADULTWATCHLIST_1STR_1LINE_HEIGHT,

    {
        {
            0,
            {ADULTWATCHLIST_LEFT_SPACE, ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE, ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            {ADULTWATCHLIST_LEFT_SPACE, ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE, ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            ADULTWATCHLIST_ITEM_BIG_FONT, ADULTWATCHLIST_ITEM_BIG_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};
//|---------------------------|
//|             @             |
//|---------------------------|
//GUIITEM_STYLE_1ICON_1LINE_CENTERALIGN_ADULTWATCH,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1icon_1line_centeralign_adultwatch =
{
    ADULTWATCHLIST_1STR_1LINE_HEIGHT, ADULTWATCHLIST_1STR_1LINE_HEIGHT,

    {
        {
            0,
            { (ADULTWATCHLIST_WIDTH - ADULTWATCHLIST_ONLY_1ICON_WIDTH)/2,   0,  (ADULTWATCHLIST_WIDTH - ADULTWATCHLIST_ONLY_1ICON_WIDTH)/2 + ADULTWATCHLIST_ONLY_1ICON_WIDTH,  ADULTWATCHLIST_1STR_1LINE_HEIGHT },
            { (ADULTWATCHLIST_WIDTH - ADULTWATCHLIST_ONLY_1ICON_WIDTH)/2,   0,  (ADULTWATCHLIST_WIDTH - ADULTWATCHLIST_ONLY_1ICON_WIDTH)/2 + ADULTWATCHLIST_ONLY_1ICON_WIDTH,  ADULTWATCHLIST_1STR_1LINE_HEIGHT },
            0, 0,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

    //|---------------------------|
    //|   @     text       @      |
    //|---------------------------|
    //GUIITEM_STYLE_1ICON_1STR_1ICON_1LINE_ADULTWATCH,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1icon_1str_1icon_1line_adultwatch =
{
    ADULTWATCHLIST_1STR_1LINE_HEIGHT, ADULTWATCHLIST_1STR_1LINE_HEIGHT,

    {
       {
            0,
            { ADULTWATCHLIST_LEFT_SPACE, (ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2,  ADULTWATCHLIST_LEFT_SPACE + ADULTWATCHLIST_ICON_HEIGH,  (ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2 + ADULTWATCHLIST_ICON_HEIGH},
            { ADULTWATCHLIST_LEFT_SPACE, (ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2,  ADULTWATCHLIST_LEFT_SPACE + ADULTWATCHLIST_ICON_HEIGH,  (ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_ICON_HEIGH)/2 + ADULTWATCHLIST_ICON_HEIGH},
            0, 0,
            0, 0
        },
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE + ADULTWATCHLIST_ICON_HEIGH + ADULTWATCHLIST_ICON_AND_TEXT_OFFESET, ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_1ICON_1STR_1ICON_RIGHTICON_WIDTH, ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            { ADULTWATCHLIST_LEFT_SPACE + ADULTWATCHLIST_ICON_HEIGH + ADULTWATCHLIST_ICON_AND_TEXT_OFFESET, ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_1ICON_1STR_1ICON_RIGHTICON_WIDTH, ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            ADULTWATCHLIST_ITEM_BIG_FONT, ADULTWATCHLIST_ITEM_BIG_FONT,
            0, 0
        },
        {
            0,
            { ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_1ICON_1STR_1ICON_RIGHTICON_WIDTH,   (ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1ICON_1STR_1ICON_RIGHTICON_WIDTH)/2, ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_1STR_1LINE_HEIGHT/2 + ADULTWATCHLIST_1ICON_1STR_1ICON_RIGHTICON_WIDTH/2 },
            { ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_1ICON_1STR_1ICON_RIGHTICON_WIDTH,   (ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1ICON_1STR_1ICON_RIGHTICON_WIDTH)/2, ADULTWATCHLIST_RIGHT_SPACE , ADULTWATCHLIST_1STR_1LINE_HEIGHT/2 + ADULTWATCHLIST_1ICON_1STR_1ICON_RIGHTICON_WIDTH/2 },
            0, 0,
            0, 0
        },
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX 
};
    //|---------------------------|
    //|      text              √  |
    //|      text                 |
    //|---------------------------|
    //GUIITEM_STYLE_2STR_2LINE_CHECK_ADULTWATCH,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_2str_2line_check_adultwatch =
{
    ADULTWATCHLIST_2STR_2LINE_HEIGHT, ADULTWATCHLIST_2STR_2LINE_HEIGHT,

    {
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_2LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_ICON_HEIGH - ADULTWATCHLIST_ICON_AND_TEXT_OFFESET, ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH},
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_2LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_ICON_HEIGH - ADULTWATCHLIST_ICON_AND_TEXT_OFFESET, ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH},
            ADULTWATCHLIST_ITEM_BIG_FONT, ADULTWATCHLIST_ITEM_BIG_FONT,
            0, 0
        },
        {
            0,
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET,  ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_ICON_HEIGH - ADULTWATCHLIST_ICON_AND_TEXT_OFFESET , ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET + ADULTWATCHLIST_SECOND_LINE_HEIGH},
            { ADULTWATCHLIST_LEFT_SPACE,   ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET,  ADULTWATCHLIST_RIGHT_SPACE - ADULTWATCHLIST_ICON_HEIGH - ADULTWATCHLIST_ICON_AND_TEXT_OFFESET , ADULTWATCHLIST_2LINE_TOP_SPACE + ADULTWATCHLIST_FIRST_LINE_HEIGH + ADULTWATCHLIST_ITEM_OFFESET + ADULTWATCHLIST_SECOND_LINE_HEIGH},
            ADULTWATCHLIST_ITEM_SMALL_FONT, ADULTWATCHLIST_ITEM_SMALL_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//|---------------------------|
//|     √       text          |
//|---------------------------|
//GUIITEM_STYLE_1STR_1LINE_CHECK_LEFT_ADULTWATCH,
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1str_1line_check_left_adultwatch =
{
    ADULTWATCHLIST_1STR_1LINE_HEIGHT, ADULTWATCHLIST_1STR_1LINE_HEIGHT,

    {
        {
            0,
            { ADULTWATCHLIST_ICON_AND_TEXT_OFFESET, ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE, ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            { ADULTWATCHLIST_ICON_AND_TEXT_OFFESET, ADULTWATCHLIST_1LINE_TOP_SPACE,  ADULTWATCHLIST_RIGHT_SPACE, ADULTWATCHLIST_1STR_1LINE_HEIGHT - ADULTWATCHLIST_1LINE_BOTTOM_SPACE },
            ADULTWATCHLIST_ITEM_BIG_FONT, ADULTWATCHLIST_ITEM_BIG_FONT,
            0, 0
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    1, MMILIST_INVALID_ITEM_INDEX
};
#endif
//============================================================================//
//                                   For watch                                //
//============================================================================//

///////////////////////END///////////////////////

//============================================================================//
//                                   For engineer mode                        //
//============================================================================//

//|---------------------------|
//|        text               |
//|---------------------------|
// GUIITEM_STYLE_1STR_ENG/GUIITEM_STYLE_1CHECK_ENG/GUIITEM_STYLE_1RADIO_ENG
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_1_text_eng =
{
    MMI_LIST_ITEM_HEIGHT_ENG, MMI_LIST_ITEM_HEIGHT_ENG,

    {
        {
            0,
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT_ENG - 1},
            { MMI_LIST_ICON_LEFT_SPACE,   0,  MMI_LIST_ITEM_RIGHT - MMI_LIST_ICON_LEFT_SPACE,  MMI_LIST_ITEM_HEIGHT_ENG -1 },
            MMI_LIST_ITEM_FONT_ENG, MMI_LIST_ITEM_FONT_ENG,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
        {0},
    },

    0, MMILIST_INVALID_ITEM_INDEX
};

//zmt add start
#if defined(WORD_CARD_SUPPORT) || defined(HANZI_CARD_SUPPORT)
//GUIITEM_SYTLE_ZMT_PUBLISH_LIST_MS
LOCAL THEMELIST_ITEM_STYLE_T const zmt_publish_list_item =
{
    2*ZMT_LIST_LINE_HIGHT, 2*ZMT_LIST_LINE_HIGHT,
    {
        {
            0,
            { ZMT_LIST_ITEM_PANDDING, 0, ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            { ZMT_LIST_ITEM_PANDDING, 0, ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            {ZMT_LIST_ITEM_PANDDING+4,  0,  ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            {ZMT_LIST_ITEM_PANDDING+4,  0,  ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_20, SONG_FONT_20,
            0, 0
        },
        {
            0,
            {ZMT_LIST_LINE_WIDTH+2*ZMT_LIST_ITEM_PANDDING, 0,  MMI_MAINSCREEN_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            {ZMT_LIST_LINE_WIDTH+2*ZMT_LIST_ITEM_PANDDING, 0,  MMI_MAINSCREEN_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_22, SONG_FONT_22,
            0, 0
        },
        {0},
        {0},
        {0},
    },
    0, 1
};

//GUIITEM_SYTLE_ZMT_BOOK_LIST_MS,
LOCAL THEMELIST_ITEM_STYLE_T const zmt_book_list_item =
{
    2.5*ZMT_LIST_LINE_HIGHT, 2.5*ZMT_LIST_LINE_HIGHT,
    {
        {
            0,
            { ZMT_LIST_ITEM_PANDDING, 0.6*ZMT_LIST_LINE_HIGHT, ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            { ZMT_LIST_ITEM_PANDDING, 0.6*ZMT_LIST_LINE_HIGHT, ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            0, 0,
            0, 0
        },
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            {ZMT_LIST_ITEM_PANDDING+4,  0.6*ZMT_LIST_LINE_HIGHT,  ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            {ZMT_LIST_ITEM_PANDDING+4,  0.6*ZMT_LIST_LINE_HIGHT,  ZMT_LIST_LINE_WIDTH+ZMT_LIST_ITEM_PANDDING, 2*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_18, SONG_FONT_18,
            0, 0
        },
        {
            0,
            {ZMT_LIST_LINE_WIDTH+2*ZMT_LIST_ITEM_PANDDING, 10,  MMI_MAINSCREEN_WIDTH, ZMT_LIST_LINE_HIGHT+10},
            {ZMT_LIST_LINE_WIDTH+2*ZMT_LIST_ITEM_PANDDING, 10,  MMI_MAINSCREEN_WIDTH, ZMT_LIST_LINE_HIGHT+10},
            SONG_FONT_22, SONG_FONT_22,
            0, 0
        },
        {
            0,
            {ZMT_LIST_LINE_WIDTH+2*ZMT_LIST_ITEM_PANDDING, ZMT_LIST_LINE_HIGHT+11, MMI_MAINSCREEN_WIDTH, 2.3*ZMT_LIST_LINE_HIGHT},
            {ZMT_LIST_LINE_WIDTH+2*ZMT_LIST_ITEM_PANDDING, ZMT_LIST_LINE_HIGHT+11, MMI_MAINSCREEN_WIDTH, 2.3*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_22, SONG_FONT_22,
            0, 0
        },     
        {0},
        {0},
    },
    0, 1
};

//GUIITEM_SYTLE_ZMT_UNIT_LIST_MS,
LOCAL THEMELIST_ITEM_STYLE_T const zmt_unit_list_item =
{
    1.5*ZMT_LIST_LINE_HIGHT, 1.5*ZMT_LIST_LINE_HIGHT,
    {
        {
            0,
            { 5, 0,  MMI_MAINSCREEN_WIDTH-ZMT_LIST_LINE_WIDTH, 1.5*ZMT_LIST_LINE_HIGHT },       /* 2 */
            { 5, 0,  MMI_MAINSCREEN_WIDTH-ZMT_LIST_LINE_WIDTH, 1.5*ZMT_LIST_LINE_HIGHT },       /* 2 */
            SONG_FONT_20, SONG_FONT_20,
            0, 100
        },
        {
            0,
            { MMI_MAINSCREEN_WIDTH-ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH, 1.5*ZMT_LIST_LINE_HIGHT},     /* 2 */
            { MMI_MAINSCREEN_WIDTH-ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH, 1.5*ZMT_LIST_LINE_HIGHT},     /* 2 */
            0, 0,
            0, 100
        },
        {0},
        {0},
        {0},
        {0},
    },
};
#endif

#ifdef WORD_CARD_SUPPORT
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_poetry_grade_list_ms =
{
    2*ZMT_LIST_LINE_HIGHT, 2*ZMT_LIST_LINE_HIGHT,
    {
        {
            0,
            {0.4*ZMT_LIST_LINE_WIDTH, 0, 1.4*ZMT_LIST_LINE_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            {0.4*ZMT_LIST_LINE_WIDTH, 0, 1.4*ZMT_LIST_LINE_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            0, 0
        }, 
        {
            0,
            {1.4*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            {1.4*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_16, SONG_FONT_16,
            0, 0
        },    
        {0},
        {0},
        {0},
        {0},
    },
    0, MMILIST_INVALID_ITEM_INDEX
};
//GUIITEM_STYLE_POETRY_ITEM_LIST_MS
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_poetry_item_list_ms =
{
    2*ZMT_LIST_LINE_HIGHT, 2*ZMT_LIST_LINE_HIGHT,
    {
        //bg
        {
            0,
            {0, 0, MMI_MAINSCREEN_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            {0, 0, MMI_MAINSCREEN_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            0, 0,
            0, 0
        }, 
        //text
        {
            GUIITEM_CONTENT_STATE_TEXT_M_ALIGN,
            {0.3*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH - 0.3*ZMT_LIST_LINE_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            {0.3*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH - 0.3*ZMT_LIST_LINE_WIDTH, 2*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_20, SONG_FONT_20,
            0, 0
        },   
        {0},
        {0},
        {0},
        {0},
    },
    0, MMILIST_INVALID_ITEM_INDEX
};
#endif

#ifdef FORMULA_SUPPORT

//GUIITEM_STYLE_FORMULA_ITEM_LIST_MS
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_formula_item_list_ms =
{
    ZMT_LIST_ITEM_HIGHT, ZMT_LIST_ITEM_HIGHT,
    {
        //text
        {
            0,
            {10, 0, MMI_MAINSCREEN_WIDTH/2, ZMT_LIST_ITEM_HIGHT},
            {10, 0, MMI_MAINSCREEN_WIDTH/2, ZMT_LIST_ITEM_HIGHT},
            SONG_FONT_20, SONG_FONT_20,
            0, 0
        }, 
        {0},
        {0},
        {0},
        {0},
        {0},
    },
    0, MMILIST_INVALID_ITEM_INDEX
};
//GUIITEM_STYLE_CLASS_MAIN_LIST_MS
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_class_main_list_ms =
{
    2.5*ZMT_LIST_LINE_HIGHT, 2.5*ZMT_LIST_LINE_HIGHT,
    {
        //bg
        {
            0,
            {0, 0, MMI_MAINSCREEN_WIDTH, 2.5*ZMT_LIST_LINE_HIGHT},
            {0, 0, MMI_MAINSCREEN_WIDTH, 2.5*ZMT_LIST_LINE_HIGHT},
            0, 0,
            0, 0
        }, 
        //icon
        {
            0,
            {0.3*ZMT_LIST_LINE_WIDTH, 0, 2*ZMT_LIST_LINE_WIDTH, 2.5*ZMT_LIST_LINE_HIGHT},
            {0.3*ZMT_LIST_LINE_WIDTH, 0, 2*ZMT_LIST_LINE_WIDTH, 2.5*ZMT_LIST_LINE_HIGHT},
            0, 0,
            0, 0
        }, 
        //text
        {
            GUIITEM_CONTENT_STATE_MULTILINE,
            {2*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH-10, 2.5*ZMT_LIST_LINE_HIGHT},
            {2*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH-10, 2.5*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_18, SONG_FONT_18,
            0, 0
        },   
        {0},
        {0},
        {0},
    },
    2, 2
};
LOCAL THEMELIST_ITEM_STYLE_T const s_item_style_class_section_list_ms =
{
    1.5*ZMT_LIST_LINE_HIGHT, 1.5*ZMT_LIST_LINE_HIGHT,
    {
        {
            GUIITEM_CONTENT_STATE_MULTILINE,
            {0.3*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH-1.2*ZMT_LIST_LINE_WIDTH, 1.5*ZMT_LIST_LINE_HIGHT},
            {0.3*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH-1.2*ZMT_LIST_LINE_WIDTH, 1.5*ZMT_LIST_LINE_HIGHT},
            SONG_FONT_20, SONG_FONT_20,
            0, 0
        },
        {
            0,
            {MMI_MAINSCREEN_WIDTH-1.2*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH-5, 1.5*ZMT_LIST_LINE_HIGHT},
            {MMI_MAINSCREEN_WIDTH-1.2*ZMT_LIST_LINE_WIDTH, 0, MMI_MAINSCREEN_WIDTH-5, 1.5*ZMT_LIST_LINE_HIGHT},
            0, 0,
            0, 0
        }, 
        {0},
        {0},
        {0},
        {0},
    },
    0, 2
};
#endif
//zmt add end

//============================================================================//
//                                  End For engineer mode                     //
//============================================================================//

typedef struct
{
    GUIITEM_STYLE_E id;
    THEMELIST_ITEM_STYLE_T const *style;
} THEMELIST_STYLE_TABLE_T;


LOCAL THEMELIST_STYLE_TABLE_T const s_style_table[] =
{
    {GUIITEM_STYLE_DEFAULT, &s_item_style_1_text},
    {GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT, &s_item_style_split},
    {GUIITEM_STYLE_ONE_LINE_TEXT_ICON, &s_item_style_1_text_icon},
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_HALF_LINE_TEXT_ICON, &s_item_style_1_text_icon_2},
#endif
    {GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON, &s_item_style_1_icon_text_icon},
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON_EXT, &s_item_style_1_icon_text_icon_ext},
#endif
    {
        GUIITEM_STYLE_ONE_LINE_TEXT,
#if defined MAINLCD_SIZE_128X160 || defined MAINLCD_SIZE_176X220
        &s_item_style_1_text
#else
        &s_item_style_1_text
#endif
    },
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_ONE_LINE_TEXT_DEFAULT_FONT, &s_item_style_1_text_2},
#endif


    {GUIITEM_STYLE_ONE_LINE_ICON_TEXT, &s_item_style_1_icon_text},
    {GUIITEM_STYLE_ONE_LINE_ICON_TEXT_WITHOUT_EFFECT_STR, &s_item_style_1_icon_text_without_effect_str},

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_ONE_LINE_TWO_ICON_TEXT, &s_item_style_1_icon_icon_text },

    {GUIITEM_STYLE_ONE_LINE_NUMBER_TEXT, &s_item_style_1_text},
#endif
    {GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT, &s_item_style_1_icon_text_text},

    {GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT,  &s_item_style_1_icon_text_text_2 },

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {
        GUIITEM_STYLE_ONE_LINE_TWO_ICON_TWO_TEXT,
#ifdef GUIF_LIST_PDASTYLE
#ifdef MAINLCD_SIZE_240X320
        &s_item_style_1_icon_icon_text_text
#else
        &s_item_style_1_icon_icon_text_text
#endif // MAINLCD_SIZE_240X320
#else
        &s_item_style_1_icon_icon_text_text
#endif // GUIF_LIST_PDASTYLE
    },

    {GUIITEM_STYLE_ONE_LINE_BUTTON_TEXT, &s_item_style_1_buttontext },

    {GUIITEM_STYLE_ONE_LINE_TWO_BUTTON_TEXT, &s_item_style_1_2buttontext},
#endif // MMI_THEME_LIST_MINI_SUPPORT

    {
        GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT,
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
        &s_item_style_1_small_text_text
#else
        &s_item_style_1_small_text_text
#endif
    },
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_TEXT_ICON, &s_item_style_1_small_text_text_icon},
    {GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_TEXT_TWO_ICON, &s_item_style_1_small_2text_2icon },
#endif

    {GUIITEM_STYLE_ONE_LINE_CHECK, &s_item_style_1_text},

    {GUIITEM_STYLE_ONE_LINE_CHECK_EXT, &s_item_style_1_text_check_ext},

    {GUIITEM_STYLE_ONE_LINE_RADIO, &s_item_style_1_text},
    {GUIITEM_STYLE_ONE_LINE_RADIO_EXT, &s_item_style_1_text_check_ext},
    {GUIITEM_STYLE_ONE_LINE_CHECK_ICON, &s_item_style_1_check_icon },

    {GUIITEM_STYLE_ONE_LINE_RADIO_ICON, &s_item_style_1_check_icon},

    {GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT, &s_item_style_2_text_text },

    {GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_AND_TEXT_AND_TEXT, &s_item_style_2_2text_and_2text },

    {GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT, &s_item_style_2_icon_text_and_text },


#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT, &s_item_style_2_2icon_text_and_text },
    {GUIITEM_STYLE_TWO_LINE_NUMBER_TEXT_AND_TEXT, &s_item_style_2_text_text},
    {GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT, &s_item_style_2_icon_text_and_2text },
    {GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT_SMALL, &s_item_style_2_icon_text_and_2text_small },
#endif

    {GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT, &s_item_style_2_2icon_text_and_2text },
    {GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT, &s_item_style_2_icon_2text_and_text },
    {GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_SMALL, &s_item_style_2_icon_2text_and_text_small },

    {GUIITEM_STYLE_TWO_LINE_TWO_ICON_TWO_TEXT_AND_TEXT, &s_item_style_2_2icon_2text_and_text },

    {GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT, &s_item_style_2_anim_text_and_text },
    {GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2, &s_item_style_2_anim_text_and_text_2 },


#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ICON, &s_item_style_2_anim_text_icon_and_text },



    {GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_3ICON, &s_item_style_2_anim_text_3icon_and_text },

    {GUIITEM_STYLE_TWO_LINE_RADIO, &s_item_style_2_text_text},
#endif

    {GUIITEM_STYLE_TWO_LINE_CHECK_ICON, &s_item_style_2_text_icon_and_text},

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_ANIM_2TEXT_AND_ICON, &s_item_style_2_anim_text_icon_and_text},
#endif

    {GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM, &s_item_style_2_text_and_text_anim },
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ANIM, &s_item_style_2_anim_text_and_text_anim },
#endif

    {GUIITEM_STYLE_TWO_LINE_TEXT_2ANIM_AND_TEXT, &s_item_style_2_text_2anim_and_text },
    {GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TEXT, &s_item_style_1_2_icon_text_and_text },

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT, &s_item_style_1_2_2icon_text_and_text },

    {GUIITEM_STYLE_ONE_TWO_LINE_NUMBER_TEXT_AND_TEXT, &s_item_style_1_2_text_and_text },
    {GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT, &s_item_style_1_2_icon_text_and_2text},
    {GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT, &s_item_style_1_2_2icon_text_and_2text },

    {GUIITEM_STYLE_ONE_TWO_ANIM_TEXT_AND_TEXT_3ICON, &s_item_style_1_2_anim_text_and_text_3icon },

#endif
    {GUIITEM_STYLE_THREE_LINE_TEXT, &s_item_style_3_text },
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_THREE_LINE_TEXT_ICON, &s_item_style_3_text_and_text_and_text_icon },
#endif

    {GUIITEM_STYLE_TWO_LINE_TEXT_ICON_AND_TWO_TEXT_SMALL, &s_item_style_2_text_icon_and_two_text_small },
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_ICON, &s_item_style_2_anim_2text_and_icon_text },

    {GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT, &s_item_style_2_anim_2text_and_text },

    {GUIITEM_STYLE_TWO_LINE_ANIM_ICON_TEXT_AND_ICON_2TEXT, &s_item_style_2_anim_icon_text_and_icon_2text },
#endif

    {GUIITEM_STYLE_TWO_LINE_ANIM_TEXT, &s_item_style_2_anim_text },
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_CHECK_ANIM_TEXT, &s_item_style_2_anim_text_2 },

    {GUIITEM_STYLE_TWO_LINE_RADIO_ANIM_TEXT, &s_item_style_2_anim_text_2},
#endif
    {GUIITEM_STYLE_TWO_LINE_CHECK_ANIM_TEXT_AND_TEXT, &s_item_style_2_check_anim_text_and_text },

    {GUIITEM_STYLE_TWO_LINE_RADIO_ANIM_TEXT_AND_TEXT, &s_item_style_2_check_anim_text_and_text},

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_2TEXT_2ANIM_AND_TEXT, &s_item_style_2_2text_2anim_and_text },

    {GUIITEM_STYLE_TWO_LINE_ANIM_ICON_TWO_TEXT_AND_ICON_TEXT, &s_item_style_2_anim_icon_two_text_and_icon_text },

#ifdef MMI_PDA_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_ICON, &s_item_style_2_2text_1icon_and_text_icon },
    {GUIITEM_STYLE_ONE_LINE_TEXT_TEXT_ICON, &s_item_style_1_text_text_icon },

    {GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT_ICON, &s_item_style_1_icon_text_text_icon },

#endif

    {GUIITEM_STYLE_ONE_LINE_LEFT_SPACE_TEXT, &s_item_style_1_left_space_text },
#endif

    {GUIITEM_STYLE_TWO_LINE_TEXT_ANIM_AND_TEXT, &s_item_style_2_text_anim_and_text },

#ifndef MMI_THEME_LIST_MINI_SUPPORT
#ifdef QBTHEME_SUPPORT
    {GUIITEM_STYLE_THREE_LINE_TEXT_TEXT_ANIM, &s_item_style_3_text_text_anim },

    {GUIITEM_STYLE_ONE_LINE_ANIM_TEXT_ICON, &s_item_style_1_anim_text_icon },

#endif
#endif
    {GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2, &s_item_style_2_anim_and_two_icon_text_and_icon_2 },

    {GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_NORMAL, &s_item_style_2_icon_two_text_and_text_normal },
#ifndef MMI_THEME_LIST_MINI_SUPPORT
#ifdef SEARCH_SUPPORT
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT, &s_item_style_2_bigicon_text },
#endif

    {GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_2, &s_item_style_2_anim_two_text_and_text_2 },
#endif
#ifdef XYSDK_SUPPORT
    {GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON_XMLY, &s_item_style_1_icon_text_icon_xmly},
    {GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON_XMLY2, &s_item_style_1_icon_text_icon_xmly2},
    {GUIITEM_STYLE_ONE_LINE_TEXT_ICON_XMLY, &s_item_style_1_text_icon_xmly},
    {GUIITEM_STYLE_ONE_LINE_ICON_ICON_TEXT_ICON_XMLY, &s_item_style_1_icon_icon_text_icon_xmly},
#endif

	{GUIITEM_STYLE_TWO_TEXT_AND_ONE_ICON_MS, &s_item_style_2_text_and_1_icon_ms},
//============================================================================//
//                                   For watch                                //
//============================================================================//

    {GUIITEM_STYLE_SMALL_3STR, &s_item_style_small_3str },
    {GUIITEM_STYLE_1STR, &s_item_style_1str },
    {GUIITEM_STYLE_1ICON_1STR, &s_item_style_1icon_1str },
    {GUIITEM_STYLE_1STR_1ICON, &s_item_style_1str_1icon },
    {GUIITEM_STYLE_1ICON_1STR_1ICON_1STR, &s_item_style_1icon_1str_1icon_1str },
    {GUIITEM_STYLE_1ICON_2STR, &s_item_style_1icon_2str },
    {GUIITEM_STYLE_2STR_1ICON, &s_item_style_2str_1icon },
    {GUIITEM_STYLE_2STR, &s_item_style_2str },
    {GUIITEM_STYLE_3STR, &s_item_style_3str  },
    {GUIITEM_STYLE_1ICON_3STR, &s_item_style_1icon_3str  },
    {GUIITEM_STYLE_2STR_1ICON_2STR, &s_item_style_2str_1icon_2str  },
    {GUIITEM_STYLE_1STR_CHECK, &s_item_style_1str_check  },
    {GUIITEM_STYLE_1STR_RADIO, &s_item_style_1str_radio  },
    {GUIITEM_STYLE_1STR_SWITCH, &s_item_style_1str_switch  },
    {GUIITEM_STYLE_1ICON_1STR_DYNA, &s_item_style_1icon_1str_dyna},
    {GUIITEM_STYLE_1ICON_2STR_2BUTTON_DYNA, &s_item_style_1icon_2str_2button_dyna},
    {GUIITEM_STYLE_1STR_1LINE_LAYOUT1, &s_1str_1line_layout1},
    {GUIITEM_STYLE_1ICON_1STR_1LINE_LAYOUT1, &s_1icon_1str_1line_layout1},
    {GUIITEM_STYLE_1STR_1ICON_1LINE_LAYOUT1, &s_1str_1icon_1line_layout1},
    {GUIITEM_STYLE_2STR_2LINE_LAYOUT1, &s_2str_2line_layout1},
    {GUIITEM_STYLE_1STR_1LINE_RADIO_LAYOUT1, &s_item_style_1str_1line_radio_layout1},
    {GUIITEM_STYLE_1STR_1LINE_CHECK_LAYOUT1, &s_item_style_1str_1line_check_layout1},
    {GUIITEM_STYLE_2STR_1ICON_2LINE_LAYOUT1, &s_item_style_2str_1icon_2line_layout1},
    {GUIITEM_STYLE_3STR_2LINE_LAYOUT1, &s_item_style_3str_2line_layout1},
    {GUIITEM_STYLE_1STR_1ICON_1STR_1LINE_LAYOUT1, &s_item_style_1str_1icon_1str_1line_layout1},
    {GUIITEM_STYLE_1STR_1ICON_1STR_1LINE_LAYOUT2, &s_item_style_1str_1icon_1str_1line_layout2},
    {GUIITEM_STYLE_1ICON_2STR_1ICON, &s_item_style_1icon_2str_1icon},
#ifdef ADULT_WATCH_SUPPORT
    {GUIITEM_STYLE_3STR_3LINE_ADULTWATCH, &s_item_style_3str_3line_adultwatch  },
    {GUIITEM_STYLE_4STR_3LINE_ADULTWATCH, &s_item_style_4str_3line_adultwatch  },
    {GUIITEM_STYLE_2STR_2LINE_ADULTWATCH, &s_item_style_2str_2line_adultwatch  },
    {GUIITEM_STYLE_1ICON_2STR_2LINE_ADULTWATCH, &s_item_style_1icon_2str_2line_adultwatch  },
    {GUIITEM_STYLE_2STR_1ICON_2LINE_ADULTWATCH, &s_item_style_2str_1icon_2line_adultwatch  },
    {GUIITEM_STYLE_1STR_1LINE_LEFTALIGN_ADULTWATCH, &s_item_style_1str_1line_leftalign_adultwatch  },
    {GUIITEM_STYLE_1STR_1LINE_CENTERALIGN_ADULTWATCH, &s_item_style_1str_1line_centeralign_adultwatch   },
    {GUIITEM_STYLE_1STR_1ICON_1LINE_ADULTWATCH, &s_item_style_1str_1icon_1line_adultwatch   },
    {GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &s_item_style_1icon_1str_1line_adultwatch   },
    {GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH, &s_item_style_1str_1line_radio_adultwatch },
    {GUIITEM_STYLE_1STR_1LINE_RADIO_RIGHT_ADULTWATCH, &s_item_style_1str_1line_radio_right_adultwatch },
    {GUIITEM_STYLE_1ICON_1LINE_CENTERALIGN_ADULTWATCH, &s_item_style_1icon_1line_centeralign_adultwatch },
    {GUIITEM_STYLE_1ICON_1STR_1ICON_1LINE_ADULTWATCH, &s_item_style_1icon_1str_1icon_1line_adultwatch },
    {GUIITEM_STYLE_2STR_2LINE_CHECK_ADULTWATCH, &s_item_style_2str_2line_check_adultwatch },
    {GUIITEM_STYLE_1STR_1LINE_CHECK_LEFT_ADULTWATCH, &s_item_style_1str_1line_check_left_adultwatch },
#endif
    {GUIITEM_STYLE_1STR_ENG, &s_item_style_1_text_eng},//for egineer mode
    {GUIITEM_STYLE_1CHECK_ENG, &s_item_style_1_text_eng},//for egineer mode
    {GUIITEM_STYLE_1RADIO_ENG, &s_item_style_1_text_eng},//for egineer mode
#ifdef _SW_ZDT_PRODUCT_
    {GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_BIGICON, &s_item_style_2_text_and_text_bigicon },
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TWO_TEXT_AND_TEXT, &s_item_style_2_bigicon_2text_and_text },
    {GUIITEM_STYLE_ONE_LINE_BGICON_TEXT_MS, &s_item_style_1_bgicon_text_ms},  
    {GUIITEM_STYLE_ONE_LINE_BGICON_R_TEXT_MS, &s_item_style_2_bgicon_text_ms},
    {GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_MS, &s_item_style_2_bigicon_text_and_text_ms},
#endif
#ifdef ZTE_WATCH
    {GUIITEM_STYLE_1ICON_1STR_1LINE_WITH_UNREAD_NUM, &s_1icon_1str_1line_with_unread_num},
#endif
//zmt add start
#if defined(WORD_CARD_SUPPORT) || defined(HANZI_CARD_SUPPORT)
    {GUIITEM_SYTLE_ZMT_PUBLISH_LIST_MS, &zmt_publish_list_item},
    {GUIITEM_SYTLE_ZMT_BOOK_LIST_MS, &zmt_book_list_item},
    {GUIITEM_SYTLE_ZMT_UNIT_LIST_MS, &zmt_unit_list_item},
#endif
#ifdef WORD_CARD_SUPPORT
    {GUIITEM_STYLE_POETRY_GRADE_LIST_MS, &s_item_style_poetry_grade_list_ms},
    {GUIITEM_STYLE_POETRY_ITEM_LIST_MS, &s_item_style_poetry_item_list_ms},
#endif
#ifdef FORMULA_SUPPORT
    {GUIITEM_STYLE_FORMULA_ITEM_LIST_MS, &s_item_style_formula_item_list_ms},
    {GUIITEM_STYLE_CLASS_MAIN_LIST_MS, &s_item_style_class_main_list_ms},
    {GUIITEM_STYLE_CLASS_SECTION_LIST_MS, &s_item_style_class_section_list_ms},
#endif
//zmt add end

//============================================================================//
//                                   For watch                                //
//============================================================================//

};


//0 default, 1 focus
const uint8 s_item_theme_font_table[GUIITEM_THEME_MAX][2] =
{
    /* GUIITEM_THEME_DEFAULT */
    { MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_DEFAULT_ITEM_FONT },
    /* GUIITEM_THEME_NORMAL */
    { MMI_LIST_NORMAL_ITEM_FONT, MMI_LIST_NORMAL_ITEM_FONT },
    /* GUIITEM_THEME_EXPAND */
    { MMI_LIST_DEFAULT_ITEM_FONT, MMI_LIST_EXPAND_ITEM_FONT },
    /* GUIITEM_THEME_SMALL */
    { MMI_LIST_ITEM_SMALL_FONT, MMI_LIST_ITEM_SMALL_FONT },
    /* GUIITEM_THEME_BIG */
    { MMI_LIST_BIG_ITEM_FONT, MMI_LIST_BIG_ITEM_FONT },
};

// 存放特殊颜色值
const GUI_COLOR_T s_item_content_color[MMITHEME_COLOR_MAX] =
{
    0x0000,
    /*lint -e572*//*lint -e778*/
    GUI_RGB2RGB565(240, 21, 0),
    GUI_RGB2RGB565(255, 0, 151),
    GUI_RGB2RGB565(221, 107, 176),
    GUI_RGB2RGB565(51, 153, 51),
    GUI_RGB2RGB565(144, 64, 0),
    GUI_RGB2RGB565(162, 0, 255),
    GUI_RGB2RGB565(0, 119, 244),
    GUI_RGB2RGB565(1, 172, 170),
    GUI_RGB2RGB565(144, 144, 144),
    GUI_RGB2RGB565(40, 107, 197),
#ifdef ADULT_WATCH_SUPPORT
    AUDULT_GRAY_COLOR,
    AUDULT_BLUE_COLOR,
    AUDULT_RED_COLOR,
#endif
//zmt add start
	GUI_RGB2RGB565(255, 0, 0),
    GUI_RGB2RGB565(80, 162, 254),
//zmt add end
    /*lint -e572*//*lint -e778*/
};


/*****************************************************************************/
//  Description : get the item style state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMITHEME_GetItemContentColor(
    MMITHEME_COLOR_ID_E color_id
)
{
    SCI_ASSERT(color_id < MMITHEME_COLOR_MAX); /*assert verified*/

    return s_item_content_color[color_id];
}

/*****************************************************************************/
//  Description : get the item style
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC const GUIITEM_STYLE_T *MMITHEME_GetItemStyle(GUIITEM_STYLE_E style)
{
    return PNULL;
}

/*****************************************************************************/
//  Description : get content text color
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T THEMELIST_GetContentTextColor(
    GUIITEM_THEME_STATE_E state,
    const THEMELIST_ITEM_STYLE_T *item_style_ptr,
    uint16 content_index
)
{
    GUI_COLOR_T color = 0;

    //invalid
    if (GUIITEM_THEME_STATE_INVALID == state
        || GUIITEM_THEME_STATE_FOCUS_INVALID == state)
    {
        color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM2);
    }
    else if (GUIITEM_THEME_STATE_FOCUS == state)
    {
#ifdef MONOCHROME_LCD_SUPPORT
        color = MMI_WHITE_COLOR;//CR221441
#else
        color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM1);
#endif
    }
    else
    {
#ifdef MONOCHROME_LCD_SUPPORT
        color =  MMI_BLACK_COLOR;//CR221441
#else
        color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0);
#endif
    }

#ifdef GUIF_LIST_PDASTYLE // pda双行支持不同色彩

    // 这里暂时这么修改，取字体颜色太复杂！
    if (PNULL != item_style_ptr && MMI_LIST_ITEM_SMALL_FONT == item_style_ptr->content[content_index].font)
    {
        color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM_SMALL_FONT);
    }

#endif

    return color;
}


/*****************************************************************************/
//  Description : get the item layout data
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC const GUIITEM_LAYOUT_DATA_T *MMITHEME_GetItemLayoutData(GUIITEM_LAYOUT_ID_E layout_id, GUIITEM_THEME_STATE_E state)
{
    return PNULL;
}


/*****************************************************************************/
//  Description : get the list style, is called by the list control
//  Global resource dependence :
//  Author:Lin.Lin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetListStyle(
    MMITHEME_LIST_STYLE_T *list_style_ptr
)
{
    const MMI_IMAGE_ID_T num_icon_id[] =
    {
        IMAGE_NUMBER_1,
        IMAGE_NUMBER_2,
        IMAGE_NUMBER_3,
        IMAGE_NUMBER_4,
        IMAGE_NUMBER_5,
        IMAGE_NUMBER_6,
        IMAGE_NUMBER_7,
        IMAGE_NUMBER_8,
        IMAGE_NUMBER_9,
        IMAGE_NUMBER_10,
        IMAGE_NUMBER_11,
        IMAGE_NUMBER_12,
        IMAGE_NUMBER_13,
        IMAGE_NUMBER_14,
        IMAGE_NUMBER_15,
        IMAGE_NUMBER_16,
        IMAGE_NUMBER_17,
        IMAGE_NUMBER_18,
        IMAGE_NUMBER_19,
        IMAGE_NUMBER_20
    };

    SCI_PASSERT(PNULL != list_style_ptr, ("MMITHEME_GetListStyle: the param is PNULL!")); /*assert verified*/
    //SCI_MEMSET(list_style_ptr, 0, sizeof(MMITHEME_LIST_STYLE_T));

    list_style_ptr->back_img_disp_rect   = MMITHEME_GetFullScreenRect();
    list_style_ptr->bg_info.color           = MMI_WINDOW_BACKGROUND_COLOR;

    list_style_ptr->bg_info.bg_type         = GUI_BG_IMG;
#ifdef MONOCHROME_LCD_SUPPORT
    list_style_ptr->highlight_back_color = MMI_BLACK_COLOR;
    list_style_ptr->is_disp_hilight_image = FALSE;
    list_style_ptr->highlight_back_image = 0;
#else
#if defined MAINLCD_SIZE_240X320
    list_style_ptr->highlight_back_color = MMI_BLACK_COLOR;
#else
    list_style_ptr->highlight_back_color = MMI_GRAY_WHITE_COLOR;
#endif

#if defined (TOUCH_PANEL_SUPPORT)
    list_style_ptr->is_disp_hilight_image = FALSE;
#else
    list_style_ptr->is_disp_hilight_image = TRUE;
#endif
    list_style_ptr->highlight_back_image = IMAGE_COMMON_SELECTED_BAR; //Bug 1321334
#endif//syy modify 2010.11.30 -end

    list_style_ptr->a_percent_lucency    = LIST_HIGHLIGHT_PERCENT_LUCENCY;
    list_style_ptr->left_space           = 0;
    list_style_ptr->right_space          = 0;

    list_style_ptr->tag_width            = MMI_LIST_ITEM_SELECT_ICON_WIDTH + MMI_LIST_ICON_MARGIN*2;

    // for text scroll!
    list_style_ptr->text_time            = MMITHEME_LIST_STYLE_TEXT_TIME;

    list_style_ptr->marked_image_id      = IMAGE_MARKED_ICON;
#ifdef ADULT_WATCH_SUPPORT
    list_style_ptr->radio_selected_image_id   = IMAGE_AW_RADIO_SELECTED_ICON;   // 单选选中图标
    list_style_ptr->radio_unselected_image_id = IMAGE_AW_RADIO_UNSELECTED_ICON; // 单选未选中图标
#else
    list_style_ptr->radio_selected_image_id   = IMAGE_RADIO_SELECTED_ICON;   // 单选选中图标
    list_style_ptr->radio_unselected_image_id = IMAGE_RADIO_UNSELECTED_ICON; // 单选未选中图标
#endif
    list_style_ptr->check_selected_image_id   = IMAGE_CHECK_SELECTED_ICON;   // 多选选中图标
    list_style_ptr->check_unselected_image_id = IMAGE_CHECK_UNSELECTED_ICON; // 多选未选中图标

    list_style_ptr->long_key_period = LISTBOX_KEY_TIME;

    SCI_MEMCPY(list_style_ptr->num_icon_id, num_icon_id, sizeof(list_style_ptr->num_icon_id));

    list_style_ptr->text_ok_id     = TXT_COMMON_OK;
    list_style_ptr->text_return_id = STXT_RETURN;
    list_style_ptr->text_cancel_id = STXT_CANCEL;
    list_style_ptr->text_select_id = STXT_SELECT;
    list_style_ptr->text_mark_id   = STXT_MARK;
    list_style_ptr->text_null_id   = TXT_NULL;

#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    list_style_ptr->image_null_id   = IMAGE_NULL;
    list_style_ptr->image_ok_id     = IMAGE_COMMON_SOFTKEY_OK_ICON;
    list_style_ptr->image_return_id = IMAGE_COMMON_SOFTKEY_RETURN_ICON;
    list_style_ptr->image_cancel_id = IMAGE_COMMON_SOFTKEY_CANCEL_ICON;
    list_style_ptr->image_select_id = IMAGE_COMMON_SOFTKEY_SELECTED_ICON;
    list_style_ptr->image_mark_id   = IMAGE_COMMON_SOFTKEY_MARKED_ICON;
#endif
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    list_style_ptr->left_key_type = GUILIST_SK_TYPE_TEXT_ID;
    list_style_ptr->mid_key_type = GUILIST_SK_TYPE_IMAGE_ID;
    list_style_ptr->right_key_type = GUILIST_SK_TYPE_TEXT_ID;
#else
    list_style_ptr->left_key_type = GUILIST_SK_TYPE_TEXT_ID;
    list_style_ptr->mid_key_type = GUILIST_SK_TYPE_TEXT_ID;
    list_style_ptr->right_key_type = GUILIST_SK_TYPE_TEXT_ID;
#endif

    list_style_ptr->text_empty_id  = TXT_EMPTY_LIST;
    list_style_ptr->text_empty_font  = MMI_DEFAULT_TEXT_FONT;
#ifdef BAIDU_DRIVE_SUPPORT
    list_style_ptr->text_empty_color = MMI_DARK_WHITE_COLOR;
#else
    list_style_ptr->text_empty_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0);
#endif

#ifdef MAINLCD_SIZE_240X320 //@for light function modify
#ifdef ADULT_WATCH_SUPPORT
    list_style_ptr->title_bg_image_id = 0;
#else
    list_style_ptr->title_bg_image_id = IMAGE_TITLE_BAR;//IMAGE_COMMON_TITLE_BAR;//IMAGE_TITLE_BAR;
#endif
#else
    list_style_ptr->title_bg_image_id = MMITHEME_GetWinTitleBar();
#endif
    list_style_ptr->icon_default_id = IMAGE_COMMON_PIC_ICON;
    list_style_ptr->icon_error_id = IMAGE_COMMON_PIC_ERROR;
    list_style_ptr->mist_color = MMI_LIST_ITEM_MIST_COLOR;

    list_style_ptr->auto_switch_time = MMILIST_AUTO_SWITCH_TIME;
    list_style_ptr->slide_time = MMILIST_SLIDE_TIME;

#ifdef MMI_PDA_SUPPORT // 只有PDA的时候，高亮才消失
    list_style_ptr->is_hl_disappear = TRUE; // 高亮会消失
#else
    list_style_ptr->is_hl_disappear = FALSE; // 高亮不消失
#endif

#if defined GUIF_LIST_PDASTYLE
    list_style_ptr->split_item_bg_info.bg_type = GUI_BG_IMG;
    list_style_ptr->split_item_bg_info.img_id = IMAGE_CONTROL_LIST_SPLIT_ITEM_BG;
    list_style_ptr->expand_image_id = IMAGE_CONTROL_LIST_RIGHT_ARROW;
    list_style_ptr->add_item_image_id = IMAGE_CONTROL_LIST_ADD;
    list_style_ptr->delete_item_image_id = IMAGE_CONTROL_LIST_DELETE;
    list_style_ptr->delete_query_image_id = IMAGE_CONTROL_LIST_DELETE_QUERY;
#else
    list_style_ptr->split_item_bg_info.bg_type = GUI_BG_COLOR;
    list_style_ptr->split_item_bg_info.color = MMI_LIST_ITEM_SPLIT_BG_COLOR;
#endif
    list_style_ptr->split_line_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SPLIT_LINE);

    list_style_ptr->is_slide_used = TRUE;

    list_style_ptr->item_font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0);            //item color

    //tip
    list_style_ptr->tip_hide_tick = MMI_LIST_TIP_HIDE_TICK;
#ifdef GUIF_LIST_PDASTYLE
    list_style_ptr->tip_bg = IMAGE_CONTROL_LIST_TIP_BG;
#endif
    list_style_ptr->tip_font_color = MMI_LIST_TIP_FONT_COLOR;

    list_style_ptr->select_cancel_index = LEFT_BUTTON;

    return (TRUE);
}

#ifdef EFFECT_3D_LIST_ROLL_SUPPORT

/*****************************************************************************/
//  Description : is need 3d list
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsNeed3DList(void)
{
    return (BOOLEAN)((MMI_IsPermit3D((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMI_3D_SECOND_LIST))
                     && (MMIAPISET_GetList3DStyle()));
}

/*****************************************************************************/
//  Description : alloc 3d list buffer
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Alloc3DListBuf(
    uint32 buf_size,
    MMI_HANDLE_T    win_handle
)
{
    MMIAPI3D_Alloc3DMenuBuf((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMI_3D_SECOND_LIST, buf_size, win_handle);
}

/*****************************************************************************/
//  Description : save 3d list buffer
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Save3DListBuf(
    GUILIST_3D_BUF_E type,
    GUI_RECT_T rect
)
{
    if (type < GUILIST_BUF_MAX)
    {
        MMI3D_ICONMENU_BUF_E buf_type = PREV_BUF;
        SE_RECT_T se_rect = {0};

        switch (type)
        {
            case GUILIST_PREV_BUF:
                buf_type = PREV_BUF;
                break;

            case GUILIST_NEXT_BUF:
                buf_type = NEXT_BUF;
                break;

            case GUILIST_PREV_BG_BUF:
                buf_type = PREV_BG_BUF;
                break;

            case GUILIST_NEXT_BG_BUF:
                buf_type = NEXT_BG_BUF;
                break;

            default:
                break;
        }

        se_rect.left = rect.left;
        se_rect.right = rect.right;
        se_rect.top = rect.top;
        se_rect.bottom = rect.bottom;

        MMIAPI3D_SaveMenuIcon(buf_type, &se_rect);
    }
}

/*****************************************************************************/
//  Description : set 3d list direction
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Set3DListDirection(
    BOOLEAN is_next
)
{
    SE_DIRECTION_E  se_dir = SE_DIRECTION_DOWN;

    if (is_next)
    {
        se_dir = SE_DIRECTION_DOWN;
    }
    else
    {
        se_dir = SE_DIRECTION_UP;
    }

    //set icon move direction
    MMIAPI3D_SetIconMenuDirection(se_dir);
}

/*****************************************************************************/
//  Description : start 3d list anim
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Start3DListAnim(void)
{
    MMIAPI3D_StartItemAnim((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMI_3D_SECOND_LIST);
}

/*****************************************************************************/
//  Description : free 3d list buf
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Free3DListBuf(void)
{
    MMIAPI3D_Free3DMenuBuf((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMI_3D_SECOND_LIST);
}
#endif

/*****************************************************************************/
//  Description : get the list item backgroup image id
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMITHEME_GetListItemBGImage(GUIITEM_THEME_BGIMAGE_TYPE_E item_bg_type)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;

#ifdef GUIF_LIST_PDASTYLE

    switch (item_bg_type)
    {
        case GUIITEM_THEME_ALERT_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_ALERT_BG;
            break;

        case GUIITEM_THEME_ITEM_FULL_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_BG;
            break;

        case GUIITEM_THEME_ITEM_HEAD_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_HEAD_BG;
            break;

        case GUIITEM_THEME_ITEM_BODAY_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_BODAY_BG;
            break;

        case GUIITEM_THEME_ITEM_TAIL_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_TAIL_BG;
            break;

        case GUIITEM_THEME_HIGHLIGHT_ITEM_FULL_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_BG_PR;
            break;

        case GUIITEM_THEME_HIGHLIGHT_ITEM_HEAD_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_HEAD_BG_PR;//IMAGE_LIST_ITEM_HEAD_BG_PR;
            break;

        case GUIITEM_THEME_HIGHLIGHT_ITEM_BODAY_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_BG_PR;//IMAGE_CONTROL_LIST_ITEM_BODAY_BG_PR;
            break;

        case GUIITEM_THEME_HIGHLIGHT_ITEM_TAIL_BG:
            image_id = IMAGE_CONTROL_LIST_ITEM_TAIL_BG_PR;
            break;

        default:
            break;
    }

#endif
    return image_id;
}

/*****************************************************************************/
//  Description : get the list item disp data
//  Global resource dependence :
//  Author: xiaoqinglu
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetListItemDefData(
    MMITHEME_LIST_CUSTDATA_T *data_ptr
)
{
    //SCI_ASSERT(PNULL != data_ptr);
    if (PNULL == data_ptr)
    {
        return;
    }

    data_ptr->data_type = GUILIST_TYPE_TEXT_ID;
    data_ptr->str_style.font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0);            //item color
    data_ptr->str_style.font = MMI_DEFAULT_BIG_FONT;
    data_ptr->str_style.align = ALIGN_HVMIDDLE;
#ifndef MMI_RES_LOW_COST
    data_ptr->left_icon_id = IMAGE_CONTROL_LIST_LEFT_SLIDE;
#endif

    data_ptr->left_text_id = TXT_SMS;
#ifndef MMI_RES_LOW_COST
    data_ptr->right_icon_id = IMAGE_CONTROL_LIST_RIGHT_SLIDE;
#endif
    data_ptr->right_text_id = TXT_DIAL;

    return;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC THEMELIST_ITEM_STYLE_T *THEMELIST_GetStyle(GUIITEM_STYLE_E style_id)
{
    uint16 i = 0;
    uint16 style_count = 0;
    THEMELIST_ITEM_STYLE_T *style_ptr = PNULL;

    MMITHEME_LIST_ITEMSTYLE_NODE_PTR pNode = s_item_customstyle_ptr;
    if (style_id & GUIITEM_CUSTOM_STYLE_MASK)
    {
        style_id &= ~GUIITEM_CUSTOM_STYLE_MASK;
        while (pNode)
        {
            if (style_id == pNode->m_style_id)
            {
                return &(pNode->m_style);
            }
            pNode = pNode->m_next_ptr;
        }
        return style_ptr;
    }
    else
    {
        style_count = sizeof(s_style_table)/sizeof(THEMELIST_STYLE_TABLE_T);
    }
    for (i = 0; i < style_count; i++)
    {
        if (style_id == s_style_table[i].id)
        {
            style_ptr = s_style_table[i].style;
            break;
        }
    }

    return style_ptr;
}

/*****************************************************************************/
//  Description : get content text color
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T THEMELIST_GetSplitItemTextColor(void)
{
    return MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM_SMALL_FONT);
}


PUBLIC int16 THEMELIST_GetListBaseWidth(void)
{
    return MMI_LIST_ITEM_RIGHT;
}

//============================================================================//
//                                   For watch                                //
//============================================================================//

/*****************************************************************************/
// 	Description : 返回common List背景
//             [In] None
//             [Out] None
//             [Return] bg_info : listItem background info
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC GUI_BG_T WATCHTHEME_GetListItemBg(void)
{
    GUI_BG_T bg_info = {0};

    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id  = image_watch_list_bg;
    
    return bg_info;
}

/*****************************************************************************/
// 	Description : 返回common List hightlight item 背景
//             [In] None
//             [Out] None
//             [Return] bg_info : hightlight listItem background info
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC GUI_BG_T WATCHTHEME_GetListHightLightItemBg(void)
{
    GUI_BG_T bg_info = {0};

    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id  = image_watch_list_highlight_bg;
    
    return bg_info;
}
/*****************************************************************************/
//  Description : 注册新的list iem style类型
//  Global resource dependence : n/a
//  Author: dandan.cao
//  Note:
/*****************************************************************************/
PUBLIC GUIITEM_STYLE_E MMITHEME_CustomListItemStyleReg(MMI_HANDLE_T owner,                   //owner, 传入list ptr, 不能为空
                                                       THEMELIST_ITEM_STYLE_T* style_ptr,     //自定义style, 不能为空
                                                       uint32    style_state          //自定义style_state, 默认为0
                                                       )
{
    BOOLEAN     bFlag = FALSE;
    uint16      style_id = 0;

    MMI_HANDLE_T    owner_handle = MMK_ConvertIdToHandle(owner);
    MMITHEME_LIST_ITEMSTYLE_NODE_PTR    pNode = s_item_customstyle_ptr;
    MMITHEME_LIST_ITEMSTYLE_NODE_PTR    pPreNode = NULL;

    if (PNULL== style_ptr)
    {
        return style_id;
    }

    //先检查注册的style是否已经存在
    while(pNode)
    {
        if (pNode->m_hOwner == owner_handle)
        {
            //如果owner相同，检测记录的style是否重复
            if (0 == memcmp((void*)&(pNode->m_style), (void*)style_ptr, sizeof(THEMELIST_ITEM_STYLE_T)))
            {
                style_id = pNode->m_style_id;
                bFlag = TRUE;
                break;
            }
        }
        pPreNode = pNode;
        pNode = pNode->m_next_ptr;
    }
    pNode = pPreNode;
    //如果不曾注册，增加该类型
    if (!bFlag)
    {
        MMITHEME_LIST_ITEMSTYLE_NODE_PTR  pNew = SCI_ALLOC_APPZ(sizeof(MMITHEME_LIST_ITEMSTYLE_NODE_T));
        if( pNew == PNULL)
        {
            return 0;
        }
        pNew->m_hOwner = owner_handle;
        pNew->m_style_state = style_state;
        SCI_MEMCPY((void*)&(pNew->m_style), (void*)style_ptr, sizeof(THEMELIST_ITEM_STYLE_T));

        if (pNode)
        {
            pNode->m_next_ptr = pNew;
        }
        else
        {
            s_item_customstyle_ptr = pNew;
        }
        pNew->m_style_id = s_item_style_id_gen++;
        style_id = pNew->m_style_id;
        s_item_style_id_gen &= 0x7fff;
    }
    //返回值最高位作为flag，以区别内置style和custom style
    return (style_id | GUIITEM_CUSTOM_STYLE_MASK);
}

/*****************************************************************************/
//  Description: 注销指定List所有自定义item style类型
//  Global resource dependence :
//  Author: dandan.cao
//  Note: 由控件destruct时自动调用，app可忽略此函数
/*****************************************************************************/
PUBLIC void MMITHEME_CustomListItemStyleUnreg(uint32 owner)
{
    MMITHEME_LIST_ITEMSTYLE_NODE_PTR    pNode = s_item_customstyle_ptr;
    MMITHEME_LIST_ITEMSTYLE_NODE_PTR    pPreNode = s_item_customstyle_ptr;
    MMITHEME_LIST_ITEMSTYLE_NODE_PTR    pDelNode = NULL;

    while (pNode)
    {
        //如果style owner与要销毁的list相等，则销毁与之相关的custom data
        if (pNode->m_hOwner == owner)
        {
            pDelNode = pNode;
            if (pNode == s_item_customstyle_ptr)
            {
                s_item_customstyle_ptr = pNode->m_next_ptr;
            }
            else
            {
                pPreNode->m_next_ptr = pNode->m_next_ptr;
            }
            pNode = pNode->m_next_ptr;
            //释放
            SCI_FREE(pDelNode);
            pDelNode = PNULL;
        }
        else
        {
            pPreNode = pNode;
            pNode = pNode->m_next_ptr;
        }
    }
}

//============================================================================//
//                                   For watch                                //
//============================================================================//

