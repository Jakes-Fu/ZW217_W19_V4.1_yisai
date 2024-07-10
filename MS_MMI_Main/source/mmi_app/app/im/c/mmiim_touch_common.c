/*==============================================================================
File Name: mmiim_touch_common.c
Author: Lianxiang.Zhou
Date: 2011/12/13
Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2011/12/13   Lianxiang.Zhou  Create.
==============================================================================*/
#include "mmi_app_im_trc.h"
#ifdef WIN32
#pragma warning (disable:4244)
#pragma warning (disable:4127)
#pragma warning (disable:4761)
#endif




/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
#include "mmiim_touch_common.h"


#if defined (MMI_IM_PDA_SUPPORT)

#include "guires.h"


#include "mmi_text.h"

#include "mmiim.h"
#include "mmiim_image.h"
#include "mmiim_text.h"
#include "mmiim_id.h"
#include "mmi_image.h"
//先借用之前的切换
#include "mmiim_im_switching.h"
#include "mmiim_touch_set.h"
#include "mmiim_touch_ime_common.h"

#include "mmiim_touch_ime_hw.h"

#if defined IM_ENGINE_CSTAR
#include "mmiim_touch_ime_cstar.h"
#elif defined IM_ENGINE_SOGOU
#include "mmiim_touch_ime_sogou.h"
#endif
#ifdef SXH_APPLE_SUPPORT
#include "OEMOS.h"
#endif
#define TIP_SELECT_LETTER_SPACE_X (8)      //按键与两侧偏移量
#define TIP_SELECT_LETTER_INTERAL_X (6)      //水平方向按键间距

#define TIP_LETTER_NORMAL_BG_COLOR (0xffcccccc)
#define TIP_LETTER_HIGHLIGHT_BG_TOP_COLOR (0xff3fb4ff)
#define TIP_LETTER_HIGHLIGHT_BG_MID_COLOR (0xff0082d5)
#define TIP_LETTER_HIGHLIGHT_BG_BOTTOM_COLOR (0xff46abeb)

#define TIP_LETTER_NORMAL_TEXT_COLOR (0xff000000)
#define TIP_LETTER_HIGHLIGHT_TEXT_COLOR (0xffffffff)

#if defined MAINLCD_SIZE_240X320
#define SETTING_IM_RECT_WIDTH 150
#define SETTING_IM_RECT_HEIGHT 198
#define SETTING_IM_ITEM_HEIGHT 36
#define SETTING_IM_RECT_WIDTH_H 150
#define SETTING_IM_RECT_HEIGHT_H 176
#define SETTING_IM_ITEM_HEIGHT_H 32
#define SETTING_IM_FONT SONG_FONT_20
#elif defined MAINLCD_SIZE_240X400
#define SETTING_IM_RECT_WIDTH 150
#define SETTING_IM_RECT_HEIGHT 198
#define SETTING_IM_ITEM_HEIGHT 36
#define SETTING_IM_RECT_WIDTH_H 150
#define SETTING_IM_RECT_HEIGHT_H 176
#define SETTING_IM_ITEM_HEIGHT_H 32
#define SETTING_IM_FONT SONG_FONT_20
#elif defined MAINLCD_SIZE_320X480
#define SETTING_IM_RECT_WIDTH 150
#define SETTING_IM_RECT_HEIGHT 220
#define SETTING_IM_ITEM_HEIGHT 40 
#define SETTING_IM_RECT_WIDTH_H 150
#define SETTING_IM_RECT_HEIGHT_H 220
#define SETTING_IM_ITEM_HEIGHT_H 40
#define SETTING_IM_FONT SONG_FONT_22
#else 
#define SETTING_IM_RECT_WIDTH 150
#define SETTING_IM_RECT_HEIGHT 220
#define SETTING_IM_ITEM_HEIGHT 40
#define SETTING_IM_RECT_WIDTH_H 150
#define SETTING_IM_RECT_HEIGHT_H 220
#define SETTING_IM_ITEM_HEIGHT_H 40
#define SETTING_IM_FONT SONG_FONT_22
#endif

#define SETTINGIM_DARK_COLOR 0xFF3C3B3C
#define SETTINGIM_BRIGHT_COLOR 0xFF272627
#define SETTINGIM_ITEM_COLOR 0xFF313031

#define SETTINGIM_DOWN_HALF FALSE //下面多半个item显示
#define SETTINGIM_UP_HALF TRUE	//上面多半个item显示

/*lint -e785 -e651*/

#if defined MAINLCD_SIZE_240X400
#define THUMB_CHAR_SUBTEXT_SIZE 24
#define THUMB_CHAR_TEXT_SIZE 20
#else   //默认以320x480计算
#define THUMB_CHAR_SUBTEXT_SIZE 36
#define THUMB_CHAR_TEXT_SIZE 32
#endif

#define THUMB_SUBTEXT_WIDTH_SIZE_RATIO (20)     //subtext宽度与按键显示区域宽度比/100
#define THUMB_SUBTEXT_HEIGHT_SIZE_RATIO (60)     //subtext高度与按键显示区域高度比/100
#define THUMB_MIDDLE_SUBTEXT_HEIGHT_SIZE_RATIO (50)     //subtext显示在中间时，高度与按键显示区域高度比/100
#define THUMB_HEIGHT_SIZE_CROSS_RATIO (25)     //subtext与Text字段交错的高度百分比

/* 输入法的键盘的base size是1000*1000， 
 *代码会根据不同的分辨率做自适应，因此
 *不需要使用自适应的公式做转换
 */
#define SYS_PANEL_TOP 0
#define SYS_PANEL_LEFT 166
#define SYS_PANEL_KEY_WIDTH 167
#define SYS_PANEL_KEY_HIGHT 174

//添加符号theme，当前的符号theme复用9键or26键，因9键26键不一致的原因，导致symbol在
//不同的输入法下面，会出现不同的显示方式
//如果修改了当前输入法的theme，再进入symbol，也会对symbol有一定的影响
THEME_T const g_theme_sys_default =
{
    //general define
    0xff,
    7,
    48,

    //panel
    SHARP_RECTANGLE,
    45,      //key_space
    0xff000000,
    {RELIEF_FLAT, 0, 0, 0, 0},

    //key define
    15,     //key_hor_space
    20,     //key_ver_space
    15,     //cand_hor_space
    20,     //cand_ver_space
    5,      //key_padding
    //key char
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 1, 0, 0x00000000, 0x00000000}, //sharp, border
            FILL_PURE, {0xff000000}, //fill, fill data
            0xffffffff, 0xffffffff, THUMB_CHAR_TEXT_SIZE, THUMB_CHAR_SUBTEXT_SIZE}, //text color, font  //active
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_PURE, {0xffff9800}, //fill, fill data
         0xff2e2e2e, 0xff2e2e2e, THUMB_CHAR_TEXT_SIZE, THUMB_CHAR_SUBTEXT_SIZE}, //text color, font        //press
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0x00000000, 0, 0}, //sharp, border
         FILL_PURE, {0xff000000}, //fill, fill data
         0xffffffff, 0xffffffff, THUMB_CHAR_TEXT_SIZE, THUMB_CHAR_SUBTEXT_SIZE}, //text color, font        //inactive
    },

    //key ctrl
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 1, 0, 0x00000000, 0x00000000}, //sharp, border
            FILL_PURE, {0xff000000}, //fill, fill data
            0xffeeeeee, 0, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_PURE, {0xffff9800}, //fill, fill data
         0xffffffff, 0xffffffff, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 1, 0x00000000, 0, 0}, //sharp, border
         FILL_PURE, {0xff000000}, //fill, fill data
         0xffffffff, 0, 20, 0}, //text color, font
    },

    //key custom
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 1, 0x00000000, 0x00000000, 0x00000000}, //sharp, border
            FILL_PURE, {0xff000000}, //fill, fill data
            0xffffffff, 0, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_PURE, {0xffff9800}, //fill, fill data
         0xffffffff, 0xffffffff, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xff000000}, //fill, fill data
         0xffffffff, 0, 20, 0}, //text color, font
    },

    //key candidate
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xffff6400, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xff000000}, //fill, fill data
            0xff101010, 0x0, 24, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SUNKEN, 1, 0, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffff9800}, //fill, fill data
         0xffffffff, 0xffffffff, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xff008000, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xff000000}, //fill, fill data
         0xff101010, 0x0, 24, 0}, //text color, font
    },

    //key system
    {
        {SHARP_RECTANGLE, {RELIEF_FLAT, 1, 0x00000000, 0x00000000, 0x00000000}, //sharp, border
            FILL_PURE, {0xff000000}, //fill, fill data
            0xffffffff, 0x0, 24, 0}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_PURE, {0xffff9800}, //fill, fill data
         0xffffffff, 0xffffffff, 24, 0}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_FLAT, 1, 0x00000000, 0x00000000, 0x00000000}, //sharp, border
         FILL_PURE, {0xff000000}, //fill, fill data
         0xff2d2d2d, 0x0, 24, 0}, //text color, font
    },

    //key art - 对于key art，我们特别的定义
    {
        {SHARP_RECTANGLE, {RELIEF_RIDGE, 3, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xff000000}, //fill, fill data
            0xff2d2d2d, 0x0, 24, 16}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffff9800}, //fill, fill data
         0xff8c3700, 0xff111111, 24, 16}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xff000000}, //fill, fill data
         0xffa8a8a8, 0x0, 24, 16}, //text color, font
    },

    //tip define
    {SHARP_ROUND_RECTANGLE, {RELIEF_INSET_BRIGHT, 2, 0, 0xffffffff, 0xffff6400}, //sharp, border
     FILL_PURE, {0xffff6400}, //fill, fill data
     0xffffffff, 0xffffffff, 48, 56}, //text color, font
    9,
    0xf0,
    300,
    5000,   //cue_delay

    //pre-edit define
    0xff000000,
    0xffffffff,
    0xff000000,
    18,
    2
};

THEME_T const g_theme_symbol_default =
{
    //general define
    0xff,
    7,
    48,

    //panel
    SHARP_RECTANGLE,
    6,      //key_space
    0xff4d4d4d,
    {RELIEF_FLAT, 0, 0, 0, 0},

    //key define
    15,     //key_hor_space
    20,     //key_ver_space
    15,     //cand_hor_space
    20,     //cand_ver_space
    5,      //key_padding
    //key char
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_GRADIENT, {0xfff8f8f8, 0xffdfdfdf, 1}, //fill, fill data
            0xff2e2e2e, 0xff858585, THUMB_CHAR_TEXT_SIZE, THUMB_CHAR_SUBTEXT_SIZE}, //text color, font  //active
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_GRADIENT_PURE_BLOCK, {0xffffbd6f, 0xffff6400, 0,  0xffffbd6f, 0xffff6400, 0, 0xffff6400, 0xffffbd6f, 0}, //fill, fill data
         0xffffffff, 0xffffffff, THUMB_CHAR_TEXT_SIZE, THUMB_CHAR_SUBTEXT_SIZE}, //text color, font        //press
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0, 0}, //sharp, border
         FILL_PURE, {0xffc0c0c0, 0, 0}, //fill, fill data
         0xff7d7d7d, 0xff9d9d9d, THUMB_CHAR_TEXT_SIZE, THUMB_CHAR_SUBTEXT_SIZE}, //text color, font        //inactive
    },

    //key ctrl
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0, 0xff808080, 0xff202020}, //sharp, border
            FILL_GRADIENT_BLOCK, {0xffc1c1c1, 0xffa6a6a6, 0,  0xffa6a6a6, 0xff797979, 0, 0xff797979, 0xff797979, 0}, //fill, fill data
            0xffeeeeee, 0, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_GRADIENT_PURE_BLOCK, {0xffffbd6f, 0xffff6400, 0,  0xffffbd6f, 0xffff6400, 0, 0xffff6400, 0xffffbd6f, 0}, //fill, fill data
         0xffffffff, 0xffffffff, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0, 0}, //sharp, border
         FILL_GRADIENT, {0xff606060, 0xff404040, 0}, //fill, fill data
         0xffeeeeee, 0, 20, 0}, //text color, font
    },

    //key custom
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xffd0d0d0}, //fill, fill data
            0xff2e2e2e, 0, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_GRADIENT_PURE_BLOCK, {0xffffbd6f, 0xffff6400, 0,  0xffffbd6f, 0xffff6400, 0, 0xffff6400, 0xffffbd6f, 0}, //fill, fill data
         0xffffffff, 0xffffffff, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffe0e0e0}, //fill, fill data
         0xff2e2e2e, 0, 20, 0}, //text color, font
    },

    //key candidate
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xffff6400, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xffffffff}, //fill, fill data
            0xff101010, 0x0, 24, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SUNKEN, 1, 0, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffff6400}, //fill, fill data
         0xffffffff, 0xffffffff, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xff008000, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xfff0f0f0}, //fill, fill data
         0xff101010, 0x0, 24, 0}, //text color, font
    },

    //key system
    {
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xffffffff}, //fill, fill data
            0xff2d2d2d, 0x0, 24, 0}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_PURE, {0xffff6400}, //fill, fill data
         0xffffffff, 0xffffffff, 24, 0}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffeeeeee}, //fill, fill data
         0xff2d2d2d, 0x0, 24, 0}, //text color, font
    },

    //key art - 对于key art，我们特别的定义
    {
        {SHARP_RECTANGLE, {RELIEF_RIDGE, 3, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xff2d2d2d}, //fill, fill data
            0xff2d2d2d, 0x0, 24, 16}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffffffff}, //fill, fill data
         0xff8c3700, 0xff111111, 24, 16}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffffffff}, //fill, fill data
         0xffa8a8a8, 0x0, 24, 16}, //text color, font
    },

    //tip define
    {SHARP_ROUND_RECTANGLE, {RELIEF_INSET_BRIGHT, 2, 0, 0xffffffff, 0xffff6400}, //sharp, border
     FILL_PURE, {0xffff6400}, //fill, fill data
     0xffffffff, 0xffffffff, 48, 56}, //text color, font
    9,
    0xf0,
    300,
    5000,   //cue_delay

    //pre-edit define
    0xff000000,
    0xffffffff,
    0xff000000,
    18,
    2
};


THEME_T const g_theme_thumb_default =
{
    //general define
    0xff,
    7,
    48,

    //panel
    SHARP_RECTANGLE,
    6,      //key_space
    0xff4d4d4d,
    {RELIEF_FLAT, 0, 0, 0, 0},

    //key define
    15,     //key_hor_space
    20,     //key_ver_space
    15,     //cand_hor_space
    20,     //cand_ver_space
    5,      //key_padding
    //key char
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_GRADIENT, {0xfff8f8f8, 0xffdfdfdf, 1}, //fill, fill data
            0xff2e2e2e, 0xff858585, THUMB_CHAR_TEXT_SIZE, THUMB_CHAR_SUBTEXT_SIZE}, //text color, font  //active
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_GRADIENT_PURE_BLOCK, {0xffffbd6f, 0xffff6400, 0,  0xffffbd6f, 0xffff6400, 0, 0xffff6400, 0xffffbd6f, 0}, //fill, fill data
         0xffffffff, 0xffffffff, THUMB_CHAR_TEXT_SIZE, THUMB_CHAR_SUBTEXT_SIZE}, //text color, font        //press
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0, 0}, //sharp, border
         FILL_PURE, {0xffc0c0c0, 0, 0}, //fill, fill data
         0xff7d7d7d, 0xff9d9d9d, THUMB_CHAR_TEXT_SIZE, THUMB_CHAR_SUBTEXT_SIZE}, //text color, font        //inactive
    },

    //key ctrl
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0, 0xff808080, 0xff202020}, //sharp, border
            FILL_GRADIENT_BLOCK, {0xffc1c1c1, 0xffa6a6a6, 0,  0xffa6a6a6, 0xff797979, 0, 0xff797979, 0xff797979, 0}, //fill, fill data
            0xffeeeeee, 0, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_GRADIENT_PURE_BLOCK, {0xffffbd6f, 0xffff6400, 0,  0xffffbd6f, 0xffff6400, 0, 0xffff6400, 0xffffbd6f, 0}, //fill, fill data
         0xffffffff, 0xffffffff, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0, 0}, //sharp, border
         FILL_GRADIENT, {0xff606060, 0xff404040, 0}, //fill, fill data
         0xffeeeeee, 0, 20, 0}, //text color, font
    },

    //key custom
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xffd0d0d0}, //fill, fill data
            0xff2e2e2e, 0, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_GRADIENT_PURE_BLOCK, {0xffffbd6f, 0xffff6400, 0,  0xffffbd6f, 0xffff6400, 0, 0xffff6400, 0xffffbd6f, 0}, //fill, fill data
         0xffffffff, 0xffffffff, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffe0e0e0}, //fill, fill data
         0xff2e2e2e, 0, 20, 0}, //text color, font
    },

    //key candidate
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xffff6400, 0xffc0c0c0, 0xff202020}, //sharp, border
#if defined SOGOU_USE_PDA_SKIN      //sogou存在背景图片，正常状态不需要绘制背景
            FILL_NONE, {0xffffffff}, //fill, fill data
#else
            FILL_PURE, {0xffffffff}, //fill, fill data
#endif
        0xff101010, 0x0, 24, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SUNKEN, 1, 0, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffff6400}, //fill, fill data
         0xffffffff, 0xffffffff, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xff008000, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xfff0f0f0}, //fill, fill data
         0xff101010, 0x0, 24, 0}, //text color, font
    },

    //key system
    {
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xffffffff}, //fill, fill data
            0xff2d2d2d, 0x0, 24, 0}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_PURE, {0xffff6400}, //fill, fill data
         0xffffffff, 0xffffffff, 24, 0}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffeeeeee}, //fill, fill data
         0xff2d2d2d, 0x0, 24, 0}, //text color, font
    },

    //key art - 对于key art，我们特别的定义
    {
        {SHARP_RECTANGLE, {RELIEF_RIDGE, 3, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xff2d2d2d}, //fill, fill data
            0xff2d2d2d, 0x0, 24, 16}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffffffff}, //fill, fill data
         0xff8c3700, 0xff111111, 24, 16}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffffffff}, //fill, fill data
         0xffa8a8a8, 0x0, 24, 16}, //text color, font
    },

    //tip define
    {SHARP_ROUND_RECTANGLE, {RELIEF_INSET_BRIGHT, 2, 0, 0xffffffff, 0xffff6400}, //sharp, border
     FILL_PURE, {0xffff6400}, //fill, fill data
     0xffffffff, 0xffffffff, 48, 56}, //text color, font
    9,
    0xf0,
    300,
    5000,   //cue_delay
    
    //pre-edit define
    0xff000000,
    0xffffffff,
    0xff000000,
    18,
    2
};


THEME_T const g_theme_qwert_default =
{
    //general define
    0xff,
    7,
    48,

    //panel
    SHARP_RECTANGLE,
    6,
    0xff4d4d4d,
    {RELIEF_FLAT, 0, 0, 0, 0},

    //key define
    20,      //key_hor_space
    50,   //key_ver_space
    20,      //cand_hor_space
    50,   //cand_ver_space
    2,      //key_padding
    //key char
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_GRADIENT, {0xfff8f8f8, 0xffdfdfdf, 1}, //fill, fill data
            0xff2e2e2e, 0xff858585, 24, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_GRADIENT_PURE_BLOCK, {0xffffbd6f, 0xffff6400, 0,  0xffffbd6f, 0xffff6400, 0, 0xffff6400, 0xffffbd6f, 0}, //fill, fill data
         0xffffffff, 0xffffffff, 24, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0, 0}, //sharp, border
         FILL_PURE, {0xffc0c0c0, 0, 0}, //fill, fill data
         0xff7d7d7d, 0xff9d9d9d, 24, 0}, //text color, font
    },

    //key ctrl
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0, 0xff808080, 0xff202020}, //sharp, border
            FILL_GRADIENT_BLOCK, {0xffc1c1c1, 0xffa6a6a6, 0,  0xffa6a6a6, 0xff797979, 0, 0xff797979, 0xff797979, 0}, //fill, fill data
            0xffeeeeee, 0, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_GRADIENT_PURE_BLOCK, {0xffffbd6f, 0xffff6400, 0,  0xffffbd6f, 0xffff6400, 0, 0xffff6400, 0xffffbd6f, 0}, //fill, fill data
         0xffffffff, 0xffffffff, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0, 0}, //sharp, border
         FILL_GRADIENT, {0xff606060, 0xff404040, 0}, //fill, fill data
         0xffeeeeee, 0, 20, 0}, //text color, font
    },

    //key custom
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xffd0d0d0}, //fill, fill data
            0xff2e2e2e, 0, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_GRADIENT_PURE_BLOCK, {0xffffbd6f, 0xffff6400, 0,  0xffffbd6f, 0xffff6400, 0, 0xffff6400, 0xffffbd6f, 0}, //fill, fill data
         0xffffffff, 0xffffffff, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffe0e0e0}, //fill, fill data
         0xff2e2e2e, 0, 20, 0}, //text color, font
    },

    //key candidate
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xffff6400, 0xffc0c0c0, 0xff202020}, //sharp, border
#if defined SOGOU_USE_PDA_SKIN      //sogou存在背景图片，正常状态不需要绘制背景
            FILL_NONE, {0xffffffff}, //fill, fill data
#else
            FILL_PURE, {0xffffffff}, //fill, fill data
#endif
            0xff101010, 0x0, 24, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SUNKEN, 1, 0, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffff6400}, //fill, fill data
         0xffffffff, 0xffffffff, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xff008000, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xfff0f0f0}, //fill, fill data
         0xff101010, 0x0, 24, 0}, //text color, font
    },

    //key system
    {
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xffffffff}, //fill, fill data
            0xff2d2d2d, 0x0, 24, 0}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_PURE, {0xffff6400}, //fill, fill data
         0xffffffff, 0xffffffff, 24, 0}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffeeeeee}, //fill, fill data
         0xff2d2d2d, 0x0, 24, 0}, //text color, font
    },

    //key art - 对于key art，我们特别的定义
    {
        {SHARP_RECTANGLE, {RELIEF_RIDGE, 3, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xff2d2d2d}, //fill, fill data
            0xff2d2d2d, 0x0, 24, 0}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffffffff}, //fill, fill data
         0xff8c3700, 0xff111111, 24, 0}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffffffff}, //fill, fill data
         0xffa8a8a8, 0x0, 24, 0}, //text color, font
    },

    //tip define
    {SHARP_ROUND_RECTANGLE, {RELIEF_INSET_BRIGHT, 2, 0, 0xffffffff, 0xffff6400}, //sharp, border
     FILL_PURE, {0xffff6400}, //fill, fill data
     0xffffffff, 0xffffffff, 56, 0}, //text color, font
    9,
    0xf0,
    300,
    5000,   //cue_delay
    //pre-edit define
    0xff000000,
    0xffffffff,
    0xff000000,
    18,
    2
};

#if defined MMIIM_HW_FULLSCREEN_SUPPORT
THEME_T const g_hw_fullscreen_theme_default =
{
    //general define
    0xff,
    7,
    48,

    //panel
    SHARP_RECTANGLE,
    6,
    0xff4d4d4d,     
    {RELIEF_FLAT, 0, 0, 0, 0},

    //key define
    10,     //key_hor_space
    40,   //key_ver_space,这里的值不是相对按键显示区域的间距，而是相对整个屏幕的显示间距
    10,     //cand_hor_space
    50,   //cand_ver_space,这里的值不是相对按键显示区域的间距，而是相对整个屏幕的显示间距
    5,      //key_padding
    //key char
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_GRADIENT, {0xfff8f8f8, 0xffdfdfdf, 1}, //fill, fill data
            0xff2e2e2e, 0xff858585, 20, 32}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_GRADIENT_PURE_BLOCK, {0xffffbd6f, 0xffff6400, 0,  0xffffbd6f, 0xffff6400, 0, 0xffffbd6f, 0xffffbd6f, 0}, //fill, fill data
         0xffffffff, 0xffffffff, 20, 32}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0, 0}, //sharp, border
         FILL_PURE, {0xffc0c0c0, 0, 0}, //fill, fill data
         0xff7d7d7d, 0xff9d9d9d, 20, 32}, //text color, font
    },

    //key ctrl
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0, 0xff808080, 0xff202020}, //sharp, border
            FILL_GRADIENT_BLOCK, {0xffc1c1c1, 0xffa6a6a6, 0,  0xffa6a6a6, 0xff797979, 0, 0xff797979, 0xff797979, 0}, //fill, fill data
            0xffeeeeee, 0, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_GRADIENT_PURE_BLOCK, {0xffffbd6f, 0xffff6400, 0,  0xffffbd6f, 0xffff6400, 0, 0xffffbd6f, 0xffffbd6f, 0}, //fill, fill data
         0xffffffff, 0xffffffff, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0, 0}, //sharp, border
         FILL_GRADIENT, {0xff606060, 0xff404040, 0}, //fill, fill data
         0xffeeeeee, 0, 20, 0}, //text color, font
    },

    //key custom
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xffd0d0d0}, //fill, fill data
            0xff2e2e2e, 0, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_GRADIENT_PURE_BLOCK, {0xffffbd6f, 0xffff6400, 0,  0xffffbd6f, 0xffff6400, 0, 0xffffbd6f, 0xffffbd6f, 0}, //fill, fill data
         0xff8c3700, 0xff111111, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_RAISED, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffe0e0e0}, //fill, fill data
         0xff2e2e2e, 0, 20, 0}, //text color, font
    },

    //key candidate
    {
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xffff6400, 0xffc0c0c0, 0xff202020}, //sharp, border
#if defined SOGOU_USE_PDA_SKIN      //sogou存在背景图片，正常状态不需要绘制背景
            FILL_NONE, {0xffffffff}, //fill, fill data
#else
            FILL_PURE, {0xffffffff}, //fill, fill data
#endif
            0xff101010, 0x0, 24, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SUNKEN, 1, 0, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffff6400}, //fill, fill data
         0xffffffff, 0xffffffff, 20, 0}, //text color, font
        {SHARP_ROUND_RECTANGLE, {RELIEF_SOLID, 1, 0xff008000, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xfff0f0f0}, //fill, fill data
         0xff101010, 0x0, 24, 0}, //text color, font
    },

    //key system
    {
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xffffffff}, //fill, fill data
            0xff2d2d2d, 0x0, 24, 0}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_FLAT, 0, 0, 0, 0}, //sharp, border
         FILL_PURE, {0xffff6400}, //fill, fill data
         0xffffffff, 0xffffffff, 24, 0}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffeeeeee}, //fill, fill data
         0xff2d2d2d, 0x0, 24, 0}, //text color, font
    },

    //key art - 对于key art，我们特别的定义
    {
        {SHARP_RECTANGLE, {RELIEF_RIDGE, 3, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
            FILL_PURE, {0xff2d2d2d}, //fill, fill data
            0x0, 0x0, 0, 0}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffffffff}, //fill, fill data
         0x0, 0x0, 0, 0}, //text color, font
        {SHARP_RECTANGLE, {RELIEF_SOLID, 1, 0xff808080, 0xffc0c0c0, 0xff202020}, //sharp, border
         FILL_PURE, {0xffffffff}, //fill, fill data
         0x0, 0x0, 0, 0}, //text color, font
    },

    //tip define
    {SHARP_ROUND_RECTANGLE, {RELIEF_INSET_BRIGHT, 2, 0, 0xffffffff, 0xffff6400}, //sharp, border
     FILL_PURE, {0xffff6400}, //fill, fill data
     0xffffffff, 0xffffffff, 48, 56}, //text color, font
    9,
    0xf0,
    300,
    5000,   //cue_delay
    
    //pre-edit define
    0xff000000,
    0xffffffff,
    0xff000000,
    18,
    2
};
#endif

/*lint +e785 +e651*/



/*
定义键盘数据，这个或者应该在单独的文件来做，但做为一个定制的应用，放在一起也行
*/
/*
定制9键键盘的排布，一般拇指键盘都会采用这样的排布
分为5行
第一行是空白行，最右边是隐藏键，输入的过程中，是扩展键，属于系统键
第二行到第五行，左边是大致是5*4的
最左一列4行分成5行，用来放置定制按键部分，属于定制键
右下的4*4是传统的键盘布局
1234567890和一个机动键，是字符键
删除、回车、数字、符号和切换，是控制键
*/
GUI_RECT_T const g_thumb_panel_position[] =
{
    {  0,   0, 839, 199}, //0 - cand panel(art)
    {840,   0, 999, 199}, //1 - hide & expand(sys)
    {  0, 200, 199, 359}, //2 -custom * 5(custom)
    {  0, 360, 199, 519}, //3
    {  0, 520, 199, 679}, //4
    {  0, 680, 199, 839}, //5
    {  0, 840, 199, 999}, //6
    //4*4 char & ctrl, char * 10, ctrl * (5 + 1)...
    //char 10
    {200, 200, 399, 399}, // 7 - 1(char)
    {400, 200, 599, 399}, // 8 - 2
    {600, 200, 799, 399}, // 9 - 3
    {200, 400, 399, 599}, //10 - 4
    {400, 400, 599, 599}, //11 - 5
    {600, 400, 799, 599}, //12 - 6
    {200, 600, 399, 799}, //13 - 7
    {400, 600, 599, 799}, //14 - 8
    {600, 600, 799, 799}, //15 - 9
    {400, 800, 599, 999}, //16 - 0
    //??4?,0????,?????,???????
    {800, 200, 999, 399}, //17 - (3, 0)(ctrl)
    {800, 400, 999, 599}, //18 - (3, 1)
    {800, 600, 999, 799}, //19 - (3, 2)
    {200, 800, 399, 999}, //20 - (0, 3)
    {600, 800, 799, 999}, //21 - (2, 3)
    {800, 800, 999, 999}, //22 - (3, 3)
    {840, 200, 999, 499}, //23 - expand - page up
    {840, 500, 999, 799}, //24 - expand - page down
    {160, 800, 839, 999}, //25 - expand - pycand panel(art)
    {  0, 800, 159, 999}, //26 - expand - page up(sys)
    {840, 800, 999, 999}, //27 - expand - page down
};
uint32 const g_thumb_panel_position_size = ARR_SIZE(g_thumb_panel_position);

#if defined MMIIM_HW_FULLSCREEN_SUPPORT

//面板显示3行，每行高度占用屏幕高度的8%
GUI_RECT_T const g_hw_fullscreen_position[] =
{
    //others...
    {0, 0, 0, 0}, //0 - blank panel(art)        //这个按键被特殊配置，切记
    {0, 0, 0, 0}, // 1 - blank panel(art)        //有些时候，手写面板处于窗口的中间部分而不是一定在底部    
    
    {0, 0, 999, 999}, // 2 - back panel(art)
    {0, 0, 833, 333}, // 3- expand - cand panel(art)
    
    //第二行
    {  0,   333, 167, 666},     // 4  char '?' 
    {167,   333, 334, 666},     // 5 char '.'
    {334,   333, 500, 666},     //  6 char ','
    {500,   333, 666, 666},     //  7 char '!'
    {666,   333, 833, 666},     //  7 char '...'
    {833,   333, 999, 666},     //  8 char '、'
    {500,   666, 666, 999},     //  9 char space
    
    //ctrl
    {666,   666, 833, 999},     //  10 char backspace
    {833,   666, 999, 999},     //  11 char enter
    {  0,   666, 167, 999},     //  12 123symb 
    {167,   666, 333, 999},     //  13 ime switch 
    {334,   666, 500, 999},     //14  ime lang switch 
    {833,   333, 999, 666}, // 15 - expand - page up
    {833,   666, 999, 999}, //  16- expand - page down

    //SYS
    {833,   0, 999, 333},     //17  sys hide, expand
    {833,   0, 999, 333},     //18 sys hide, expand ===>与上一个坐标相同
};
#endif

// thumb键盘，翻页键大小调整
GUI_RECT_T const g_thumb_panel_position_adjust_page_icon[] =
{
    {  0,   0, 839, 199}, //0 - cand panel(art)
    {840,   0, 999, 199}, //1 - hide & expand(sys)
    {  0, 200, 199, 359}, //2 -custom * 5(custom)
    {  0, 360, 199, 519}, //3
    {  0, 520, 199, 679}, //4
    {  0, 680, 199, 839}, //5
    {  0, 840, 199, 999}, //6
    //4*4 char & ctrl, char * 10, ctrl * (5 + 1)...
    //char 10
    {200, 200, 399, 399}, // 7 - 1(char)
    {400, 200, 599, 399}, // 8 - 2
    {600, 200, 799, 399}, // 9 - 3
    {200, 400, 399, 599}, //10 - 4
    {400, 400, 599, 599}, //11 - 5
    {600, 400, 799, 599}, //12 - 6
    {200, 600, 399, 799}, //13 - 7
    {400, 600, 599, 799}, //14 - 8
    {600, 600, 799, 799}, //15 - 9
    {400, 800, 599, 999}, //16 - 0
    //右边4个，0旁边两个，按从上到下，从左到右的顺序
    {800, 200, 999, 399}, //17 - (3, 0)(ctrl)
    {800, 400, 999, 599}, //18 - (3, 1)
    {800, 600, 999, 799}, //19 - (3, 2)
    {200, 800, 399, 999}, //20 - (0, 3)
    {600, 800, 799, 999}, //21 - (2, 3)
    {800, 800, 999, 999}, //22 - (3, 3)
    {840, 200, 999, 599}, //23 - expand - page up
    {840, 600, 999, 999}, //24 - expand - page down
    {160, 800, 839, 999}, //25 - expand - pycand panel(art)
    {  0, 800, 159, 999}, //26 - expand - page up(sys)
    {840, 800, 999, 999}, //27 - expand - page down
};

uint32 const g_thumb_panel_position_adjust_page_icon_size = ARR_SIZE(g_thumb_panel_position_adjust_page_icon);

uint8 const g_thumb_panel_expand_page_cnt = 2;
uint8 const g_thumb_panel_expand_py_cnt = 3;

#if defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT
wchar const * const g_thumb_cust_cn_symb = L"\xff0c\x3002\xff1f\xff01\x2026";
#endif
wchar const * const g_thumb_cust_symb = L"\x2c\x2e\x3f\xff01\x2026";

#if defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT
wchar const * const g_hw_fullscreen_cust_cn_symb = L"\xff1f\x3002\xff0c\xff01\x2026\x3001\x3000";     //?。，!... 、space
#endif
wchar const * const g_hw_fullscreen_cust_symb = L"\x3f\x2e\x2c\x21\x40\x5c\x20";    //?。，!... 、space

wchar const *g_thumb_key_disp[] =
{
    L"?,.", L"abc", L"def", L"ghi", L"jkl", L"mno", L"pqrs", L"tuv", L"wxyz", L""
};
uint8 const g_thumb_key_disp_len[] =
{
    4, 3, 3, 3, 3, 3, 4, 3, 4, 0
};


char const *g_thumb_data_lower[] =
{
    "\0011",
    "\004abc2",
    "\004def3",
    "\004ghi4",
    "\004jkl5",
    "\004mno6",
    "\005pqrs7",
    "\004tuv8",
    "\005wxyz9",
    "\002 0",
};

char const *g_thumb_data_upper[] =
{
    "\0011",
    "\004ABC2",
    "\004DEF3",
    "\004GHI4",
    "\004JKL5",
    "\004MNO6",
    "\005PQRS7",
    "\004TUV8",
    "\005WXYZ9",
    "\002 0",
};


/*
定制笔画键盘的排布，采用简化按键模式
是将传统的九健键盘的3*3变形为3*2
删除、回车、数字、符号和切换，是控制键
*/
GUI_RECT_T const g_stroke_panel_position[] =
{
    {  0,   0, 839, 199}, //0 - cand panel(art)
    {840,   0, 999, 199}, //1 - hide & expand(sys)
    {  0, 200, 199, 359}, //2 -custom * 5(custom)
    {  0, 360, 199, 519}, //3
    {  0, 520, 199, 679}, //4
    {  0, 680, 199, 839}, //5
    {  0, 840, 199, 999}, //6
    //4*4 char & ctrl, char * 10, ctrl * (5 + 1)...
    //char 10
    {200, 200, 399, 499}, // 7 - (0, 0) 横
    {400, 200, 599, 499}, // 8 - (1, 0) 竖
    {600, 200, 799, 499}, // 9 - (2, 0) 撇
    {200, 500, 399, 799}, //10 - (0, 2) 点
    {400, 500, 599, 799}, //11 - (1, 2) 弯钩
    {600, 500, 799, 799}, //12 - (2, 2) 任意键
    {400, 800, 599, 999}, //13 - (2, 3) 0
    //右边4个，0旁边两个，按从上到下，从左到右的顺序
    {800, 200, 999, 499}, //14 - (0, 3)(ctrl)
    {800, 500, 999, 799}, //16 - (2, 3)
    {200, 800, 399, 999}, //17 - (0, 3)
    {600, 800, 799, 999}, //18 - (2, 3)
    {800, 800, 999, 999}, //19 - (3, 3)
    {840, 200, 999, 499}, //20 - expand - page up
    {840, 500, 999, 799}, //21 - expand - page down
};

uint32 const g_stroke_panel_position_size = ARR_SIZE(g_stroke_panel_position);

wchar const * const g_stroke_symb = L"\x4e00\x4e28\x4e3f\x4e36\x4e5b\xff1f\x3000";


/*
qwert键盘的布局，统一布局
分为5行
第一行是空白行，最右边是隐藏键，输入的过程中，是扩展键，属于系统键
下面四行，等高，字母排布按照常用布局
qwertyuiop - 10个键，均分
asdfghjkl - 9个键，宽度同上，左右留半个空
caps - zxcvbnm - del - 7个键 + 2个控制键，字符键同上一行，控制键占字符键1.5
123sym - ?, - space - set - enter - 2个custom + 4 个控制键
1.5+1+1+3+1.5+2
*/
GUI_RECT_T const g_qwert_panel_position[] =
{
    {  0,   0, 839, 199}, //cand panel
    {840,   0, 999, 199}, //hide & expand
    {  0, 200,  99, 399}, //q - char
    {100, 200, 199, 399}, //w
    {200, 200, 299, 399}, //e
    {300, 200, 399, 399}, //r
    {400, 200, 499, 399}, //t
    {500, 200, 599, 399}, //y
    {600, 200, 699, 399}, //u
    {700, 200, 799, 399}, //i
    {800, 200, 899, 399}, //o
    {900, 200, 999, 399}, //p
    { 50, 400, 149, 599}, //a
    {150, 400, 249, 599}, //s
    {250, 400, 349, 599}, //d
    {350, 400, 449, 599}, //f
    {450, 400, 549, 599}, //g
    {550, 400, 649, 599}, //h
    {650, 400, 749, 599}, //j
    {750, 400, 849, 599}, //k
    {850, 400, 949, 599}, //l
    {150, 600, 249, 799}, //z
    {250, 600, 349, 799}, //x
    {350, 600, 449, 799}, //c
    {450, 600, 549, 799}, //v
    {550, 600, 649, 799}, //b
    {650, 600, 749, 799}, //n
    {750, 600, 849, 799}, //m
    {150, 800, 249, 999}, //? - custom
    {450, 800, 549, 999}, //,
    {  0, 600, 149, 799}, //caps - ctrl
    {850, 600, 999, 799}, //del
    {  0, 800, 149, 999}, //123?!#
    {250, 800, 449, 999}, //space       
    {550, 800, 699, 999}, //ime type set
    {700, 800, 849, 999}, //ime lang set
    {850, 800, 999, 999}, //enter
    {840, 200, 999, 599}, //expand - page up
    {840, 600, 999, 999}, //expand - page down
};
uint32 const g_qwert_panel_position_size = ARR_SIZE(g_qwert_panel_position);

uint8 const g_qwert_panel_expand_page_cnt = 2;

/*
qwert键盘的布局，统一布局
分为5行
第一行是空白行，最右边是隐藏键，输入的过程中，是扩展键，属于系统键
下面四行，等高，字母排布按照常用布局
vkey_irregular_1 ~ 10 - 10个键，均分
vkey_irregular_2 ~ 20 - 10个键，宽度同上
caps - vkey_irregular_20 ~ 28 del - 8个键 + 2个控制键，字符键同上一行，宽度相同
123sym - ?, - space - set - enter - 2个custom + 4 个控制键
1.5+1+1+3+1.5+2
*/

GUI_RECT_T const g_28char_panel_position[] =
{
    {  0,   0, 839, 199}, //cand panel
    {840,   0, 999, 199}, //hide & expand

    {  0, 200,  99, 399}, //vkey_irregular_1 - char
    {100, 200, 199, 399}, //VKEY_SPECIAL_2
    {200, 200, 299, 399}, //VKEY_SPECIAL_3
    {300, 200, 399, 399}, //VKEY_SPECIAL_4
    {400, 200, 499, 399}, //VKEY_SPECIAL_5
    {500, 200, 599, 399}, //VKEY_SPECIAL_6
    {600, 200, 699, 399}, //VKEY_SPECIAL_7
    {700, 200, 799, 399}, //VKEY_SPECIAL_8
    {800, 200, 899, 399}, //VKEY_SPECIAL_9
    {900, 200, 999, 399}, //VKEY_SPECIAL_10
    
    { 0, 400, 99, 599}, //vkey_speacial_11
    {100, 400, 199, 599}, //VKEY_SPECIAL_12
    {200, 400, 299, 599}, //
    {300, 400, 399, 599}, //
    {400, 400, 499, 599}, //
    {500, 400, 599, 599}, //
    {600, 400, 699, 599}, //
    {700, 400, 799, 599}, //
    {800, 400, 899, 599}, //
    {900, 400, 999, 599}, //VKEY_SPECIAL_20
    
    {100, 600, 199, 799}, //VKEY_SPECIAL_21
    {200, 600, 299, 799}, //
    {300, 600, 399, 799}, //
    {400, 600, 499, 799}, //
    {500, 600, 599, 799}, //
    {600, 600, 699, 799}, //
    {700, 600, 799, 799}, //
    {800, 600, 899, 799}, //VKEY_SPECIAL_28
    
    {150, 800, 249, 999}, //? - custom
    {450, 800, 549, 999}, //,
    {  0, 600, 99, 799}, //caps - ctrl
    {900, 600, 999, 799}, //del
    {  0, 800, 149, 999}, //123?!#
    {250, 800, 449, 999}, //space
    {550, 800, 699, 999}, //ime type set
    {700, 800, 849, 999}, //ime lang set
    {850, 800, 999, 999}, //enter
    {840, 200, 999, 599}, //expand - page up
    {840, 600, 999, 999}, //expand - page down
};
uint32 const g_28char_panel_position_size = ARR_SIZE(g_28char_panel_position);

//40 char key, for trad_chinese,some other IME may use it.
GUI_RECT_T const g_40char_panel_position[] =
{
    {  0,   0, 839, 166}, //cand panel
    {840,   0, 999, 166}, //hide & expand

	//first line
    {  0, 167,  99, 332}, //vkey_irregular_1 - char
    {100, 167, 199, 332}, //VKEY_SPECIAL_2
    {200, 167, 299, 332}, //VKEY_SPECIAL_3
    {300, 167, 399, 332}, //VKEY_SPECIAL_4
    {400, 167, 499, 332}, //VKEY_SPECIAL_5
    {500, 167, 599, 332}, //VKEY_SPECIAL_6
    {600, 167, 699, 332}, //VKEY_SPECIAL_7
    {700, 167, 799, 332}, //VKEY_SPECIAL_8
    {800, 167, 899, 332}, //VKEY_SPECIAL_9
    {900, 167, 999, 332}, //VKEY_SPECIAL_10
    
    { 0,  333,  99, 499}, //vkey_speacial_11
    {100, 333, 199, 499}, //VKEY_SPECIAL_12
    {200, 333, 299, 499}, //
    {300, 333, 399, 499}, //
    {400, 333, 499, 499}, //
    {500, 333, 599, 499}, //
    {600, 333, 699, 499}, //
    {700, 333, 799, 499}, //
    {800, 333, 899, 499}, //
    {900, 333, 999, 499}, //VKEY_SPECIAL_20
    
    {  0, 500,  99, 666}, //VKEY_SPECIAL_21
    {100, 500, 199, 666}, //
    {200, 500, 299, 666}, //
    {300, 500, 399, 666}, //
    {400, 500, 499, 666}, //
    {500, 500, 599, 666}, //
    {600, 500, 699, 666}, //
    {700, 500, 799, 666}, //VKEY_SPECIAL_28
    {800, 500, 899, 666}, //VKEY_SPECIAL_29
    {900, 500, 999, 666}, //VKEY_SPECIAL_30

    {  0, 667,  99, 833}, //vkey_irregular_1 - char
    {100, 667, 199, 833}, //VKEY_SPECIAL_2
    {200, 667, 299, 833}, //VKEY_SPECIAL_3
    {300, 667, 399, 833}, //VKEY_SPECIAL_4
    {400, 667, 499, 833}, //VKEY_SPECIAL_5
    {500, 667, 599, 833}, //VKEY_SPECIAL_6
    {600, 667, 699, 833}, //VKEY_SPECIAL_7
    {700, 667, 799, 833}, //VKEY_SPECIAL_8
    {800, 667, 899, 833}, //VKEY_SPECIAL_9
    {900, 667, 999, 833}, //VKEY_SPECIAL_10
    
    {100, 834, 199, 999}, //? - custom
    {400, 834, 499, 999}, //,
    {500, 834, 599, 999}, //caps - ctrl
    {800, 834, 899, 999}, //del
    {  0, 834,  99, 999}, //123?!#
    {200, 834, 399, 999}, //space
    {600, 834, 699, 999}, //ime type set
    {700, 834, 799, 999}, //ime lang set
    {900, 834, 999, 999}, //enter
    {840, 200, 999, 599}, //expand - page up
    {840, 600, 999, 999}, //expand - page down
};
uint32 const g_40char_panel_position_size = ARR_SIZE(g_40char_panel_position);

/* 数字键盘的定义，4*4，最右一列放两个大的按钮，删除和回车*/
GUI_RECT_T const g_digit_panel_position[] =
{
    {  0,   0, 839, 159}, //0 - cand panel(art)
    {840,   0, 999, 159}, //1 - hide & expand(sys)
    {  0, 160, 249, 369}, //2 - 1(char)
    {250, 160, 499, 369}, //3 - 2
    {500, 160, 749, 369}, //4 - 3
    {  0, 370, 249, 579}, //5 - 4
    {250, 370, 499, 579}, //6 - 5
    {500, 370, 749, 579}, //7 - 6
    {  0, 580, 249, 789}, //8 - 7
    {250, 580, 499, 789}, //9 - 8
    {500, 580, 749, 789}, //10 - 9
    {  0, 790, 249, 999}, //11 - sym
    {250, 790, 499, 999}, //12 - 0
    {500, 790, 749, 999}, //13 - dot
    {750, 160, 999, 579}, //14 - del(ctrl)
    {750, 580, 999, 999}, //14 - enter(ctrl)
};

uint32 const g_digit_panel_position_size = ARR_SIZE(g_digit_panel_position);

/* 数字键盘的定义，4*4，最右一列放两个大的按钮，删除和回车*/
GUI_RECT_T const g_sys_panel_position[] =
{
    {SYS_PANEL_LEFT,
     SYS_PANEL_TOP,
     SYS_PANEL_LEFT + SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + SYS_PANEL_KEY_HIGHT},
    {SYS_PANEL_LEFT + SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP,
     SYS_PANEL_LEFT + 2*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + SYS_PANEL_KEY_HIGHT},
    {SYS_PANEL_LEFT + 2*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP,
     SYS_PANEL_LEFT + 3*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + SYS_PANEL_KEY_HIGHT},

    {SYS_PANEL_LEFT,
     SYS_PANEL_TOP  + SYS_PANEL_KEY_HIGHT,
     SYS_PANEL_LEFT + SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 2*SYS_PANEL_KEY_HIGHT},
    {SYS_PANEL_LEFT + SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + SYS_PANEL_KEY_HIGHT,
     SYS_PANEL_LEFT + 2*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 2*SYS_PANEL_KEY_HIGHT},
    {SYS_PANEL_LEFT + 2*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + SYS_PANEL_KEY_HIGHT,
     SYS_PANEL_LEFT + 3*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 2*SYS_PANEL_KEY_HIGHT},

    {SYS_PANEL_LEFT,
     SYS_PANEL_TOP  + 2*SYS_PANEL_KEY_HIGHT,
     SYS_PANEL_LEFT + SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 3*SYS_PANEL_KEY_HIGHT},
    {SYS_PANEL_LEFT + SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 2*SYS_PANEL_KEY_HIGHT,
     SYS_PANEL_LEFT + 2*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 3*SYS_PANEL_KEY_HIGHT},
    {SYS_PANEL_LEFT + 2*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 2*SYS_PANEL_KEY_HIGHT,
     SYS_PANEL_LEFT + 3*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 3*SYS_PANEL_KEY_HIGHT},

    {SYS_PANEL_LEFT,
     SYS_PANEL_TOP  + 3*SYS_PANEL_KEY_HIGHT,
     SYS_PANEL_LEFT + SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 4*SYS_PANEL_KEY_HIGHT},
    {SYS_PANEL_LEFT + SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 3*SYS_PANEL_KEY_HIGHT,
     SYS_PANEL_LEFT + 2*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 4*SYS_PANEL_KEY_HIGHT},
    {SYS_PANEL_LEFT + 2*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 3*SYS_PANEL_KEY_HIGHT,
     SYS_PANEL_LEFT + 3*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 4*SYS_PANEL_KEY_HIGHT},

    {SYS_PANEL_LEFT + 3*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP,
     SYS_PANEL_LEFT + 4*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + SYS_PANEL_KEY_HIGHT}, //2 //del
    {SYS_PANEL_LEFT + 3*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + SYS_PANEL_KEY_HIGHT,
     SYS_PANEL_LEFT + 4*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 3*SYS_PANEL_KEY_HIGHT}, //shift
    {SYS_PANEL_LEFT + 3*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 3*SYS_PANEL_KEY_HIGHT,
     SYS_PANEL_LEFT + 4*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 4*SYS_PANEL_KEY_HIGHT}, //hide

    {SYS_PANEL_LEFT,
     SYS_PANEL_TOP  + 4*SYS_PANEL_KEY_HIGHT,
     SYS_PANEL_LEFT + SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 5*SYS_PANEL_KEY_HIGHT}, //page up
    {SYS_PANEL_LEFT + SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 4*SYS_PANEL_KEY_HIGHT,
     SYS_PANEL_LEFT + 3*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 5*SYS_PANEL_KEY_HIGHT}, //page
    {SYS_PANEL_LEFT + 3*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 4*SYS_PANEL_KEY_HIGHT,
     SYS_PANEL_LEFT + 4*SYS_PANEL_KEY_WIDTH,
     SYS_PANEL_TOP  + 5*SYS_PANEL_KEY_HIGHT}, //page down
};
uint32 const g_sys_panel_position_size = ARR_SIZE(g_sys_panel_position);

GUIIM_TYPE_T g_default_ch_type = GUIIM_TYPE_MULTITAP;
GUIIM_TYPE_T g_default_en_type = GUIIM_TYPE_SMART;
GUIIM_INPUT_TYPE_T g_default_input_type = GUIIM_INPUT_TYPE_TP_QWERT;
//进入手写前，记录非手写输入方式，防止手写时，切换语言，导致输入方式g_default_input_type记录错误
GUIIM_INPUT_TYPE_T g_entry_hw_input_type = GUIIM_INPUT_TYPE_TP_QWERT;
LOCAL CAPITTAL_MODE_T g_last_care_caps = CAPITTAL_MODE_INITIAL;

#if defined MMIIM_TOUCH_SMART_CAPS
LOCAL CAPS_SWITCH_TYPE_E s_caps_switch_type = CAPS_SWITCH_TYPE_NONE_E;
#endif

uint16 const g_kb_keymap[] =
{
    VKEY_Q, VKEY_W, VKEY_E, VKEY_R, VKEY_T, VKEY_Y, VKEY_U, VKEY_I, VKEY_O, VKEY_P,
    VKEY_A, VKEY_S, VKEY_D, VKEY_F, VKEY_G, VKEY_H, VKEY_J, VKEY_K, VKEY_L,
    VKEY_Z, VKEY_X, VKEY_C, VKEY_V, VKEY_B, VKEY_N, VKEY_M,
};
uint32 const g_kb_keymap_size = ARR_SIZE(g_kb_keymap);

uint16 const g_28char_kb_keymap[] =
{
    VKEY_IRREGULAR_1, VKEY_IRREGULAR_2, VKEY_IRREGULAR_3, VKEY_IRREGULAR_4, VKEY_IRREGULAR_5,
    VKEY_IRREGULAR_6, VKEY_IRREGULAR_7, VKEY_IRREGULAR_8, VKEY_IRREGULAR_9, VKEY_IRREGULAR_10,

    VKEY_IRREGULAR_11, VKEY_IRREGULAR_12, VKEY_IRREGULAR_13, VKEY_IRREGULAR_14, VKEY_IRREGULAR_15,
    VKEY_IRREGULAR_16, VKEY_IRREGULAR_17, VKEY_IRREGULAR_18, VKEY_IRREGULAR_19, VKEY_IRREGULAR_20,

    VKEY_IRREGULAR_21, VKEY_IRREGULAR_22, VKEY_IRREGULAR_23, VKEY_IRREGULAR_24, VKEY_IRREGULAR_25,
    VKEY_IRREGULAR_26, VKEY_IRREGULAR_27, VKEY_IRREGULAR_28,
};

uint16 const g_40char_kb_keymap[] =
{
    VKEY_IRREGULAR_1, VKEY_IRREGULAR_2, VKEY_IRREGULAR_3, VKEY_IRREGULAR_4, VKEY_IRREGULAR_5,
    VKEY_IRREGULAR_6, VKEY_IRREGULAR_7, VKEY_IRREGULAR_8, VKEY_IRREGULAR_9, VKEY_IRREGULAR_10,

    VKEY_IRREGULAR_11, VKEY_IRREGULAR_12, VKEY_IRREGULAR_13, VKEY_IRREGULAR_14, VKEY_IRREGULAR_15,
    VKEY_IRREGULAR_16, VKEY_IRREGULAR_17, VKEY_IRREGULAR_18, VKEY_IRREGULAR_19, VKEY_IRREGULAR_20,

    VKEY_IRREGULAR_21, VKEY_IRREGULAR_22, VKEY_IRREGULAR_23, VKEY_IRREGULAR_24, VKEY_IRREGULAR_25,
    VKEY_IRREGULAR_26, VKEY_IRREGULAR_27, VKEY_IRREGULAR_28, VKEY_IRREGULAR_29, VKEY_IRREGULAR_30,

    VKEY_IRREGULAR_31, VKEY_IRREGULAR_32, VKEY_IRREGULAR_33, VKEY_IRREGULAR_34, VKEY_IRREGULAR_35,
    VKEY_IRREGULAR_36, VKEY_IRREGULAR_37, VKEY_IRREGULAR_38, VKEY_IRREGULAR_39, VKEY_IRREGULAR_40,
};


	
int32 g_select_letter_index;     //记录当前选择的letter index
LOCAL int32 s_default_menu_index;     //记录默认的menu index

const wchar normal_mul_sym[TEXT_LEN_LIMIT] = {'.'};  /*lint !e785*/
const wchar normal_9key_mul_sym[TEXT_LEN_LIMIT] = {':'};  /*lint !e785*/    //9键已经有'.'

#if defined IM_ARABIC_SUPPORT
const wchar arabic_mul_sym[TEXT_LEN_LIMIT] = {0x002e, 0x060c, 0x061b, 0x061f, 0x0021};    /*lint !e785*/
#endif

#if defined IM_THAI_SUPPORT
const wchar thai_mul_sym[TEXT_LEN_LIMIT] = {0x0E3F, 0x0020, 0x002C};    /*lint !e785*/
#endif

LOCAL BOOLEAN VkeySwitchMethod (IM_DATA_T *data_ptr);
LOCAL int16 MMIIM_TOUCH_GetKbKeyHeight (IM_DATA_T *data_ptr);
    

/*==============================================================================
Description: 按键指定区域及绘制方案情况下，获取按键字符text字段最大字符尺寸 
                    大小,当前仅仅对character属性的按键做了该项功能，其他类型按键没有
                    添加字符按键也仅仅增加了部分现在中英文需要的属性，如果外文
                    需要类似功能，后期再做添加
Global resource dependence: DrawKeyRaw
Author: yaoguang.chen   2012-04-12
Note:DrawKeyRaw，与该处的绘制方式一致，否则获取的字符大小没有意义
==============================================================================*/
LOCAL GUI_FONT_T ComputKeyFontMaxSize (
    IM_DATA_T const *data_ptr,
    KEY_THEME_T const *theme_ptr,
    KEY_INFO_T *key_ptr
);


/*==============================================================================
Description: 画按键背景，将该部分代码从DrawKeyRaw中抽出来，
                注意，key_rect是去掉了边框后的rect，边框是从内往外画的
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL void DrawKeyBg (
    GUILCD_DRAW_DC_T const *dc_data_ptr, 
    KEY_THEME_T const *theme_ptr,
    GUI_RECT_T key_rect 
    );


/*----------------------------------------------------------------------------------------------------------------------
Name:   HasConfigSelectInputType

Func:   某种语言，是否配置或者存在某种输入方式

Input:
            lang:  输入法对应的语言

Return:
            TRUE:配置了该种输入方式
Thoughtway:

Create: yaoguangchen@spread. 2011-10-11

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN HasConfigSelectInputType(IM_DATA_T *data_ptr, GUIIM_LANGUAGE_T guiim_lang, GUIIM_TYPE_T type)
{
    uint32 iloop = 0;

    for (iloop = 0; iloop < data_ptr->setting_menu_size; iloop ++)
    {
        if (guiim_lang == data_ptr->setting_menu_data[iloop].lang && data_ptr->setting_menu_data[iloop].type == type)
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*----------------------------------------------------------------------------------------------------------------------
Name:   HasConfigSelectInput

Func:   某种语言，是否配置或者存在某种输入方式

Input:
            lang:  输入法对应的语言

Return:
            TRUE:配置了该种输入方式
Thoughtway:

Create: yaoguangchen@spread. 2011-10-11

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN HasConfigSelectInput(IM_DATA_T *data_ptr, GUIIM_LANGUAGE_T guiim_lang, GUIIM_INPUT_TYPE_T input)
{
    uint32 iloop = 0;

    for (iloop = 0; iloop < data_ptr->setting_menu_size; iloop ++)
    {
        if (guiim_lang == data_ptr->setting_menu_data[iloop].lang && data_ptr->setting_menu_data[iloop].input == input)
        {
            return TRUE;
        }
    }

    return FALSE;
}



/*==============================================================================
Description: 查找触笔下的按键
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC KEY_INFO_T *FindKey(
    IM_DATA_T *data_ptr,
    GUI_POINT_T *point_ptr
)
{
    KEY_INFO_T *key_ptr = PNULL;
    GUI_POINT_T point = {0};
    int32 key_idx = 0;
    uint8 key_hor_space = 0, key_ver_space = 0; 

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != point_ptr); /*assert verified*/
    if(PNULL == point_ptr)
    {
        return  PNULL;
    }
    
    point = *point_ptr;

    if (!GUI_PointIsInRect(point, data_ptr->rect))
    {
        return PNULL;
    }

    point.x -= data_ptr->rect.left;
    point.y -= data_ptr->rect.top;

    // 1/2间距向四周扩散
#if defined MMIIM_HW_FULLSCREEN_SUPPORT     
{
    if (data_ptr->init_param.method_param_ptr
            && data_ptr->init_param.method_param_ptr->im_def_ptr->method == GUIIM_M_HANDWRITING
            && !data_ptr->is_symbol_panel
            )   //这个间隔是按整个屏幕计算的，不是面板大小
    {  
        GUI_RECT_T panel_rect = {0};

        HwFullScreenGetDisplayPanelRect (data_ptr, &panel_rect);
        key_hor_space = (data_ptr->theme.key_hor_space * (data_ptr->rect.right - data_ptr->rect.left)) / (2 * 1000);  
        key_ver_space = (data_ptr->theme.key_ver_space * (panel_rect.bottom - panel_rect.top)) /(2 * 1000);
    } 
    else    
    {
        key_hor_space = (data_ptr->theme.key_hor_space * (data_ptr->rect.right - data_ptr->rect.left)) / (2 * 1000);  
        key_ver_space = (data_ptr->theme.key_ver_space * (data_ptr->rect.bottom - data_ptr->rect.top)) /(2 * 1000);
    }
}
#else
    key_hor_space = (data_ptr->theme.key_hor_space * (data_ptr->rect.right - data_ptr->rect.left)) / (2 * 1000);  
    key_ver_space = (data_ptr->theme.key_ver_space * (data_ptr->rect.bottom - data_ptr->rect.top)) /(2 * 1000);
#endif    
    for (key_idx = 0; key_idx < data_ptr->vkb.key_cnt; key_idx++)
    {
        GUI_RECT_T rect = data_ptr->vkb.keys[key_idx].rect;

        rect.left = rect.left > key_hor_space ? rect.left - key_hor_space : 0;
        rect.top = rect.top > key_ver_space ? rect.top - key_ver_space : 0;
        rect.right += key_hor_space;
        rect.bottom += key_ver_space;
        
        if (data_ptr->vkb.keys[key_idx].is_inactive) continue;

        if (data_ptr->vkb.keys[key_idx].is_hide) continue;

        //if (GUI_PointIsInRect(point, data_ptr->vkb.keys[key_idx].rect))
        if (GUI_PointIsInRect(point, rect))
        {
            key_ptr = data_ptr->vkb.keys + key_idx;
            break;
        }
    }

    return key_ptr;
}


/*==============================================================================
Description: 查找触笔下的按键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC KEY_INFO_T *FindKeyWithCode(
    IM_DATA_T *data_ptr,
    uint16 code
)
{
    KEY_INFO_T *key_ptr = PNULL;
    int32 key_idx = 0;

    CHECK_DATA_EX(data_ptr);

    for (key_idx = 0; key_idx < data_ptr->vkb.key_cnt; key_idx++)
    {
        if (code == data_ptr->vkb.keys[key_idx].code)
        {
            key_ptr = data_ptr->vkb.keys + key_idx;
            break;
        }
    }

    return key_ptr;
}

/*==============================================================================
Description: SlidingPanel
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN SlidingPanel(IM_DATA_T *data_ptr, int32 dir)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    CHECK_DATA_EX(data_ptr);

    if (0 == dir) return FALSE;

    if (data_ptr->f_is_inputting && data_ptr->f_is_inputting(data_ptr)) return FALSE;

	//force to modify the type of im_set so as to enable the following set_input effective
	//support to switch the im_def between digital and multitap(character)
	if(GUIIM_TAG_NUM_WITH_SWITCHING == data_ptr->init_param.method_param_ptr->tag)
	{
		if (GUIIM_TYPE_DIGITAL == data_ptr->init_param.method_param_ptr->im_def_ptr->type)
		{
			data_ptr->init_param.method_param_ptr->im_set.type = GUIIM_TYPE_MULTITAP;
		}
		else if (GUIIM_TYPE_MULTITAP == data_ptr->init_param.method_param_ptr->im_def_ptr->type)
		{
			data_ptr->init_param.method_param_ptr->im_set.type = GUIIM_TYPE_DIGITAL;
		}
	}

    if (GUIIM_INPUT_TYPE_TOUCHPEN == data_ptr->init_param.method_param_ptr->im_def_ptr->input)
    {
        notify_pack.data.set_input = GUIIM_INPUT_TYPE_TP_QWERT;
    }
    else if (GUIIM_INPUT_TYPE_TP_QWERT == data_ptr->init_param.method_param_ptr->im_def_ptr->input)
    {
        notify_pack.data.set_input = GUIIM_INPUT_TYPE_TOUCHPEN;
    }
    else
    {
        return FALSE;
    }

    data_ptr->is_out_effect_enabled = FALSE;

    notify_pack.notify = GUIIM_NOTIFY_SET_INPUT;

    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    return TRUE;
}


/*==============================================================================
Description: 输入法设置窗口
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_HANDLE_T CreateSettingWin(IM_DATA_T *data_ptr)
{
    MMI_WINDOW_CREATE_T create_info = {0};
    MMI_HANDLE_T win_handle = 0;

    create_info.applet_handle = MMK_GetFirstAppletHandle();
    create_info.win_id = MMIIM_TOUCH_SETTING_WIN_ID;
    create_info.func = HandleSettingWinMsg;
    create_info.add_data_ptr = (ADD_DATA)data_ptr;
    create_info.window_style = WS_HAS_TRANSPARENT;
    create_info.window_move_style = MOVE_FORBIDDEN;
    create_info.win_priority = WIN_ONE_LEVEL;

    win_handle = MMK_CreateWindow(&create_info);

    if (win_handle != 0) GUIIM_AppendWin(data_ptr->init_param.im_handle, win_handle);

    return win_handle;
}


/*==============================================================================
Description: HandleSettingWinMsg
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E HandleSettingWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    IM_DATA_T *data_ptr = PNULL;
    MMI_RESULT_E handle_res = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T menu_id = MMIIM_TOUCH_SETTING_MENU_ID;

    data_ptr = (IM_DATA_T *)MMK_GetWinAddDataPtr(win_id);

    //如下情况不再处理消息, 并且任意消息关闭窗口
    //1 数据校验信息被破坏
    //2 控件已经被销毁
    if (sizeof(IM_DATA_T) != data_ptr->check_info
            || PNULL == MMK_GetCtrlPtr(data_ptr->init_param.im_handle))
    {
        MMK_CloseWin(win_id);

        //SCI_TRACE_LOW:"[mmi_touch.c]HandleSettingWinMsg(), data lost..."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_COMMON_1302_112_2_18_2_27_8_86,(uint8*)"");

        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T both_rect = {0};
            GUIMENU_BUTTON_INFO_T   menu_button_info = {0};
            uint32 item_idx = 0;
            MMI_STRING_T title_str = {0};

            MMIIM_SetTpPopMenuRect(data_ptr->init_param.win_handle, &both_rect);

            GUIMENU_CreatDynamic(&both_rect, win_id, menu_id, GUIMENU_STYLE_POPUP_AUTO);

            menu_button_info.is_static = FALSE;
            menu_button_info.node_id = 0;
            menu_button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;

            GUIMENU_SetButtonStyle(menu_id, &menu_button_info);

            MMITHEME_GetResText(STXT_MAIN_SETTING, data_ptr->init_param.win_handle, &title_str);
            GUIMENU_SetMenuTitle(&title_str, menu_id);

            for (item_idx = 0; item_idx < data_ptr->setting_menu_size; item_idx++)
            {
                GUIMENU_DYNA_ITEM_T node_item = {0};
                MMI_STRING_T disp_str = {0};

                //首次download版本
                if (MMIAPIIM_GetPrevLanguage() == GUIIM_LANG_NONE)
                {
                    GUIIM_LANGUAGE_T guiim_disp_lang = GUIIM_LANG_NONE;
                    MMISET_LANGUAGE_TYPE_E language_type = MMISET_LANGUAGE_ENGLISH;

                    MMIAPISET_GetLanguageType(&language_type);
                    guiim_disp_lang = MMIAPIIM_GetGUILangBySetLangIdx(language_type);

                    //可能当前显示语言也为英文
                    if (guiim_disp_lang == GUIIM_LANG_NONE || guiim_disp_lang == GUIIM_LANG_ENGLISH)
                    {
                        continue;
                    }
                    else
                    {
                        MMIAPIIM_SetPrevLanguage(guiim_disp_lang);
                    }
                }
                else if (MMIAPIIM_GetPrevLanguage() != data_ptr->setting_menu_data[item_idx].lang    //非前一次使用的非英语语言
                         && GUIIM_LANG_NONE != data_ptr->setting_menu_data[item_idx].lang        //其他类型菜单，没有设置语言
                         && GUIIM_LANG_ENGLISH != data_ptr->setting_menu_data[item_idx].lang      //英文为标配
                        )    //
                {
                    continue;
                }

                //英文手写和其他语言手写只匹配其中一个
                if (data_ptr->setting_menu_data[item_idx].lang == GUIIM_LANG_ENGLISH
                        && data_ptr->setting_menu_data[item_idx].input == GUIIM_INPUT_TYPE_HANDWRITING)		//英文手写
                {
                    if (MMIAPIIM_GetPrevLanguage() == GUIIM_LANG_NONE
                            && MMIAPIIM_GetLanguage() == GUIIM_LANG_ENGLISH)	//有且仅有英语输入时
                    {

                    }
                    else
                    {
                        continue;
                    }
                }

                MMITHEME_GetResText(data_ptr->setting_menu_data[item_idx].text_id,
                                    data_ptr->init_param.win_handle, &disp_str);

                if ((GUIIM_LIMIT_LOCAL & data_ptr->init_param.method_param_ptr->limit)
                        && item_idx < 3)
                {
                    node_item.is_grayed = TRUE;
                }
                else
                {
                    node_item.is_grayed = FALSE;
                }

                node_item.select_icon_id = 0;
                node_item.item_text_ptr = &disp_str;

                GUIMENU_InsertNode(item_idx, item_idx + 1, 0, &node_item, menu_id);
            }

            MMK_SetAtvCtrl(win_id, menu_id);
        }

        break;

    case MSG_CLOSE_WINDOW:
        GUIIM_RemoveWin(data_ptr->init_param.im_handle, MMK_ConvertIdToHandle(win_id));
        break;

    case MSG_LOSE_FOCUS:
    case MSG_PRE_LCD_SWITCH:
        MMK_CloseWin(win_id);
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_FULL_PAINT:
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_NOTIFY_PENOK:
        {
            MMI_MENU_ID_T menu_select = 0;

            menu_select = GUIMENU_GetCurNodeId(MMIIM_TOUCH_SETTING_MENU_ID);
            menu_select -= 1;

            if (menu_select < data_ptr->setting_menu_size - 1)
            {
                SettingMethod(data_ptr,
                              data_ptr->setting_menu_data[menu_select].lang,
                              data_ptr->setting_menu_data[menu_select].type,
                              data_ptr->setting_menu_data[menu_select].input);

                MMK_CloseWin(win_id);
            }
            else if (menu_select == data_ptr->setting_menu_size - 1)
            {
                MMI_HANDLE_T win_handle = 0;

                MMK_CloseWin(win_id);

                win_handle = MMIAPIIM_OpenSetWin();

                //如下情况不再处理消息
                //1 数据校验信息被破坏
                //2 控件已经被销毁
                if (sizeof(IM_DATA_T) != data_ptr->check_info) return MMI_RESULT_TRUE;
                if (PNULL == MMK_GetCtrlPtr(data_ptr->init_param.im_handle)) return MMI_RESULT_TRUE;

                //enter more setting...
                data_ptr->more_setting_win_handle = win_handle;

                if (win_handle != 0)
                {
                    GUIIM_AppendWin(data_ptr->init_param.im_handle, win_handle);
                }
            }
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_APP_CANCEL:
    case MSG_NOTIFY_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        handle_res = MMI_RESULT_FALSE;
        break;
    }

    //param; //use it for warning

    return handle_res;
}






/*==============================================================================
Description: SettingMethod
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN SettingMethod(
    IM_DATA_T *data_ptr,
    GUIIM_LANGUAGE_T lang,
    GUIIM_TYPE_T type,
    GUIIM_INPUT_TYPE_T input
)
{
    GUIIM_METHOD_CFG_T *im_def_ptr = PNULL;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    CHECK_DATA_EX(data_ptr);

    if (GUIIM_LANG_NONE == lang)
    {
        lang = data_ptr->init_param.method_param_ptr->im_set.lang;
    }

    if (GUIIM_TYPE_NONE == type)
    {
        type = data_ptr->init_param.method_param_ptr->im_set.type;
    }

    if (GUIIM_INPUT_TYPE_NONE == input)
    {
        if (GUIIM_INPUT_TYPE_HANDWRITING == data_ptr->init_param.method_param_ptr->im_set.input)        //当前为手写
        {
            //手写切换有误
            if (g_default_input_type == GUIIM_INPUT_TYPE_HANDWRITING)
            {
                g_default_input_type = g_entry_hw_input_type;
            }

            input = g_default_input_type;
        }
        else
        {
            input = data_ptr->init_param.method_param_ptr->im_set.input;
        }
    }

    im_def_ptr = MMITHEME_IMGetDefBySet(lang, input, type, data_ptr->init_param.method_param_ptr->im_set.capital);

    if (PNULL == im_def_ptr)
    {
        return FALSE;
    }

    data_ptr->is_out_effect_enabled = FALSE;

    notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
    notify_pack.data.set_method = im_def_ptr->method;

    GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

    return TRUE;
}



/*==============================================================================
Description: 回到之前的输入法
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ResetMethod(IM_DATA_T *data_ptr)
{
    BOOLEAN is_method_set = FALSE;

    CHECK_DATA(data_ptr);

    if (0 != data_ptr->tip_hide_timer)
    {
        MMK_StopTimer(data_ptr->tip_hide_timer);
        data_ptr->tip_hide_timer = 0;
    }

    if (0 != data_ptr->cue_hide_timer_id)
    {
        MMK_StopTimer(data_ptr->cue_hide_timer_id);
        data_ptr->cue_hide_timer_id = 0;
    }

    UILAYER_RemoveBltLayer(&data_ptr->tip_layer);
    data_ptr->is_tip_display = FALSE;
    UILAYER_RemoveBltLayer(&data_ptr->edit_layer);

    is_method_set = data_ptr->f_set_method(data_ptr, data_ptr->init_param.method_param_ptr->im_def_ptr->method);

    //SCI_ASSERT(is_method_set); /*assert verified*/

    data_ptr->f_draw_all(data_ptr);

    return;
}


/*==============================================================================
Description: 根据当前语言输入法，设置新输入语言
                    当前语言存在的输入法可能新的输入语言不支持或没有开启，
                    开启方式见mmithem_im.c/s_im_def 和mmiim_touch_app_sogou.c/s_setting_menu_data
Global resource dependence: NONE
Author: yaoguang.chen
Note:   
==============================================================================*/
PUBLIC GUIIM_METHOD_T GetMethodBySwitchLang(IM_DATA_T *data_ptr, GUIIM_LANGUAGE_T lang)
{
    uint32 iloop = 0;
    GUIIM_METHOD_T new_method = GUIIM_M_NONE;
    GUIIM_METHOD_CFG_T *cfg_ptr = PNULL;
    GUIIM_TYPE_T type = data_ptr->init_param.method_param_ptr->im_def_ptr->type;
    GUIIM_INPUT_TYPE_T input = data_ptr->init_param.method_param_ptr->im_def_ptr->input;
    CAPITTAL_MODE_T cap = g_last_care_caps;//data_ptr->vkb.cap;
    GUIIM_CAPITAL_MODE_T im_cap = GUIIM_MODE_MAX;

    //本地语不支持
    if (MMIIM_IsLimitSwitchLang (data_ptr->init_param.method_param_ptr))
    {
        return GUIIM_M_NONE;
    }

    //NEWMS00180829 切换语言建议保留切换前的状态
    //开机后，该值有可能与某些语言输入法对应的类型不对
    if (lang == GUIIM_LANG_ENGLISH)
    {
        type = g_default_en_type;
    }
    else
    {
        type = g_default_ch_type;
    }
    
    //手写，切换到没有手写的输入语言
    if (data_ptr->init_param.method_param_ptr->im_def_ptr->method == GUIIM_M_HANDWRITING)
    {
        //可能某些语言存在手写，而新的语言没有手写
        if (!HasConfigSelectInput(data_ptr, lang, GUIIM_INPUT_TYPE_HANDWRITING))
        {
            input = g_entry_hw_input_type;
            type = GUIIM_TYPE_MULTITAP;
        }
    }
    //可能某些语言存在智能输入，而新的语言没有智能输入
    else if ((type == GUIIM_TYPE_SMART && !HasConfigSelectInputType(data_ptr, lang, GUIIM_TYPE_SMART))
            || (type == GUIIM_TYPE_STROKE && !HasConfigSelectInputType(data_ptr, lang, GUIIM_TYPE_STROKE))
       ) 
    {
        type = GUIIM_TYPE_MULTITAP;
    }
    //可能某些语言存在multi-tap输入，而新的语言没有multi-tap输入，比如中文特例
    else if (type == GUIIM_TYPE_MULTITAP && !HasConfigSelectInputType(data_ptr, lang, GUIIM_TYPE_MULTITAP))
    {
        //if (type != GUIIM_TYPE_STROKE) //always be true
        {
            type = GUIIM_TYPE_SMART;
        }
    }

    //可能从有大小写的语言切换到没有大小写的语言
    if (!MMIAPIIM_CapsExist(lang))
    {
        //cap = GUIIM_MODE_MAX;
        im_cap = GUIIM_MODE_MAX;
    }
    else
    {
        im_cap = MMIIM_GetIMCaps(cap);
    }

    for (iloop = 0; iloop < data_ptr->setting_menu_size; iloop++)
    {
        //查找新的语言对应的输入法
        if (lang == data_ptr->setting_menu_data[iloop].lang)
        {
            //找相同输入方式的输入法
            cfg_ptr = MMITHEME_IMGetDefBySet(data_ptr->setting_menu_data[iloop].lang, input, type, im_cap);
            if (cfg_ptr != PNULL)
            {
                new_method = cfg_ptr->method;
                break;
            }
        }
    }

    return new_method;
}



/*==============================================================================
Description: Because the speed is too slow when use etrump font. 
    			IME can't use so many draw keys when press down or up.
		So if we need to draw one key, we need to set the key to paint status    			
Global resource dependence: NONE
Author: yaoguang.chen
Note:   DrawVkb
==============================================================================*/
PUBLIC void MMIIM_SetVKeyPaintStatus (KEY_INFO_T *key_ptr, BOOLEAN is_paint)
{
	if (key_ptr)
	{
		key_ptr->is_paint = is_paint;
	}

	return;
}



/*==============================================================================
Description: 
	1.The speed is too slow when use etrump font. 
    	IME can't use so many draw keys when press down or up.
		So if we need to draw one key, we need to set the key paint status  
		All keys need to paint, but when is hide status, it won't paint.
	2.When vkb is_paint, will paint the vkb background, because if paint bg 
		the all other keys need to paint it again
Global resource dependence: NONE
Author: yaoguang.chen
Note:   DrawVkb
==============================================================================*/
PUBLIC void MMIIM_SetVkbPaintStatus (IM_DATA_T *data_ptr, BOOLEAN is_paint)
{
	if (data_ptr && data_ptr->vkb.keys)
	{
		int32 iloop = 0;
		
		for (iloop = 0; iloop < data_ptr->vkb.key_cnt; iloop ++)
		{
			data_ptr->vkb.keys[iloop].is_paint = is_paint;
		}
	}

	if (data_ptr)
	{
		data_ptr->vkb.is_paint = is_paint;
	}
	
	return;
}



/*==============================================================================
Description: DrawVkb
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN DrawVkb(IM_DATA_T *data_ptr)
{
    BOOLEAN has_disp_layer = FALSE;
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUILCD_DRAW_DC_T dc_data = {0};
    BOOLEAN is_dev_ok = FALSE;
    GUI_RECT_T panel_rect = {0};
    int32 key_idx = 0;
	
    CHECK_DATA_EX(data_ptr);

    has_disp_layer = CreateUILayer(data_ptr);

    //SCI_ASSERT(has_disp_layer); /*assert verified*/
    if(!has_disp_layer)
    {
        return  FALSE;
    }

	if (data_ptr->vkb.is_paint)		//vkb need to paint, but the keys paint status dependence itself.
    {
    	UILAYER_Clear(&data_ptr->ui_layer);
	}

    append_layer.lcd_dev_info = data_ptr->ui_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->ui_layer, &dc_data);

    if (!is_dev_ok)
    {
        return FALSE;
    }

    panel_rect.right = data_ptr->rect.right - data_ptr->rect.left;
    panel_rect.bottom = data_ptr->rect.bottom - data_ptr->rect.top;
    
    //panel
    if (data_ptr->vkb.is_paint)		//background, dependence vkb paint status
    {
	    if (SHARP_ROUND_RECTANGLE == data_ptr->theme.panel_sharp)
	    {
	        _FillRoundRect(&dc_data, &panel_rect, data_ptr->theme.panel_color, FALSE);
	    }
	    else
	    {
	        _FillRect(&dc_data, &panel_rect, data_ptr->theme.panel_color);
	    }
	}

    //key
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    if(PNULL == data_ptr->vkb.keys)
    {
        return  FALSE;
    }
    
//sogou背景皮肤
#ifdef SOGOU_USE_PDA_SKIN
	if (data_ptr->vkb.is_paint || (data_ptr->vkb.vessel_key && data_ptr->vkb.vessel_key->is_paint))
    {
    	Sogou_DrawVkbSkin (data_ptr, &dc_data);
	}
#endif

    for (key_idx = 0; key_idx < data_ptr->vkb.key_cnt; key_idx++)
    {
    	//the etrump font GUI_GetStringWidth waste too much time, one key paint used 18 ticks,
    	//so we have to minus paint times.
    	if (((KEY_INFO_T *)(data_ptr->vkb.keys + key_idx))->is_paint)		//need to paint
        {
	        //输入法细节改进，这里使用潜规则，默认按键绘制函数均不设置，如果需要设置，另行处理
	        if (((KEY_INFO_T *)(data_ptr->vkb.keys + key_idx))->f_draw == PNULL)     
	        {
	            DrawKey(data_ptr, &dc_data, data_ptr->vkb.keys + key_idx);		
	        }
	        else
	        {
	            ((KEY_INFO_T *)(data_ptr->vkb.keys + key_idx))->f_draw(data_ptr, &dc_data, data_ptr->vkb.keys + key_idx);
	        }
		}
    }

	MMIIM_SetVkbPaintStatus (data_ptr, FALSE);		//clear all paint status

    //draw over...
    if (data_ptr->theme.alpha < 0xff)
    {
        UILAYER_WeakLayerAlpha(&data_ptr->ui_layer, data_ptr->theme.alpha);
    }

    if (data_ptr->is_in_effect_enabled)
    {
        int16 height = data_ptr->rect.bottom - data_ptr->rect.top;
        int16 top = 0;
        int16 remain = VKB_EFFECT_FRAME_CNT;
        int16 total_frame = VKB_EFFECT_FRAME_CNT;

//NEWMS00190811 [6530][128x64_320x480PDA2]弹出的输入框界面中，退出输入键盘，界面背景全黑
#if 0
#if defined MMIIM_HW_FULLSCREEN_SUPPORT
        if (!data_ptr->is_symbol_panel
            && data_ptr->init_param.method_param_ptr->im_def_ptr->method == GUIIM_M_HANDWRITING   //全屏手写状态
            ) //从手写切换到sym后，上述的输入法值依然为hw
        {
            height = panel_rect.bottom - panel_rect.top;
            HwFullScreenGetDisplayPanelRect (data_ptr, &panel_rect);
            GUI_FillRect(data_ptr->init_param.lcd_info_ptr, panel_rect, RGB888TO565(data_ptr->theme.panel_color));

            total_frame = VKB_EFFECT_FRAME_CNT * 3 / 5;      //行数为3行，而VKB_EFFECT_FRAME_CNT是针对5行设计的
        }
        else
        {
            GUI_FillRect(data_ptr->init_param.lcd_info_ptr, data_ptr->rect, RGB888TO565(data_ptr->theme.panel_color));
        }
#else
        GUI_FillRect(data_ptr->init_param.lcd_info_ptr, data_ptr->rect, RGB888TO565(data_ptr->theme.panel_color));
#endif
#endif

        do
        {
            remain--;
            top = data_ptr->rect.top + remain * height / total_frame;
            UILAYER_SetLayerPosition(&data_ptr->ui_layer, data_ptr->rect.left, top);
            MMITHEME_UpdateRect();
        }
        while (remain > 0);

        data_ptr->is_in_effect_enabled = FALSE;
    }
    else
    {
        MMITHEME_StoreUpdateRect(&data_ptr->ui_layer, panel_rect);
    }

    return TRUE;
}


/*==============================================================================
Description: MMIIM_DrawAll
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN MMIIM_DrawAll(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    if (PNULL == data_ptr->vkb.keys)
    {
        return FALSE;
    }

    if (data_ptr->is_symbol_panel)
    {
        SymDraw(data_ptr);
        //return TRUE;      //不能退出，后面还有失去焦点的情况
    }
    else
    {
        switch (data_ptr->init_param.method_param_ptr->im_def_ptr->method)
        {
#if (defined IM_HANDWRITING_HANWANG || defined IM_HANDWRITING_SOGOU)
        case GUIIM_M_HANDWRITING:
            HwDraw(data_ptr);
            break;
#endif

        case GUIIM_M_TP_123:
        case GUIIM_M_TPQ_123:
            DigitDraw(data_ptr);
            break;
        case GUIIM_M_TP_SYS:
            SysDraw(data_ptr);
            break;
        case GUIIM_M_TP_ENGLISH_ABC:
        case GUIIM_M_TP_ENGLISH_ABC_LOWER:
        case GUIIM_M_TP_ENGLISH_ABC_LEAD_UPPER:
        case GUIIM_M_TP_ENGLISH_ABC_UPPER:
#if defined IM_ENGINE_SOGOU
            ThumbMultiTapDraw (data_ptr);
#else
            ThumbAbcDraw(data_ptr);
#endif
            break;

        case GUIIM_M_TP_ENGLISH:
        case GUIIM_M_TP_ENGLISH_LOWER:
        case GUIIM_M_TP_ENGLISH_LEAD_UPPER:
        case GUIIM_M_TP_ENGLISH_UPPER:
#if defined IM_ENGINE_SOGOU
            ThumbSmartDraw(data_ptr);
#elif defined IM_ENGINE_CSTAR
            ThumbLatinDraw(data_ptr);
#endif        
            break;

#if defined (IM_SIMP_CHINESE_SUPPORT)
        case GUIIM_M_TP_PINYIN:
#if defined (IM_ENGINE_SOGOU) || defined (IM_ENGINE_CSTAR)
            ThumbPinyinDraw(data_ptr);
#endif
            break;
#endif

#if defined (IM_SIMP_CHINESE_SUPPORT)
        case GUIIM_M_TP_STROKE:
        case GUIIM_M_TPQ_STROKE:
#if defined (IM_ENGINE_SOGOU) || defined (IM_ENGINE_CSTAR)
            ThumbStrokeDraw(data_ptr);
#endif
            break;
#endif

#if defined (IM_TRAD_CHINESE_SUPPORT)
        case GUIIM_M_TP_ZHUYIN:
            ThumbZhuyinDraw(data_ptr);
            break;

        case GUIIM_M_TPQ_ZHUYIN:
            QwertZhuyinDraw (data_ptr);
            break;

        case GUIIM_M_TP_STROKE_CHT:
        case GUIIM_M_TPQ_STROKE_CHT:
            ThumbStrokeDraw(data_ptr);
            break;
#endif

        case GUIIM_M_TPQ_ENGLISH_ABC:
        case GUIIM_M_TPQ_ENGLISH_ABC_LOWER:
        case GUIIM_M_TPQ_ENGLISH_ABC_LEAD_UPPER:
        case GUIIM_M_TPQ_ENGLISH_ABC_UPPER:
#if defined IM_ENGINE_SOGOU
            QwertMultiTapDraw (data_ptr);
#else
            QwertAbcDraw(data_ptr);
#endif
            break;

        case GUIIM_M_TPQ_ENGLISH:
        case GUIIM_M_TPQ_ENGLISH_LOWER:
        case GUIIM_M_TPQ_ENGLISH_LEAD_UPPER:
        case GUIIM_M_TPQ_ENGLISH_UPPER:
#if defined IM_ENGINE_SOGOU
            QwertSmartDraw(data_ptr);
#elif defined IM_ENGINE_CSTAR
            QwertLatinDraw(data_ptr);
#endif
            break;

#if defined (IM_SIMP_CHINESE_SUPPORT)
        case GUIIM_M_TPQ_PINYIN:
#if defined (IM_ENGINE_SOGOU) || defined (IM_ENGINE_CSTAR)

            QwertPinyinDraw(data_ptr);
#endif
            break;
#endif

    //RUSSIAN 俄语
#if defined (IM_RUSSIAN_SUPPORT)
#if (defined MMIIM_ENGINE_EXIST)
        case GUIIM_M_TP_RUSSIAN:
        case GUIIM_M_TP_RUSSIAN_LEAD_UPPER:
        case GUIIM_M_TP_RUSSIAN_LOWER:
        case GUIIM_M_TP_RUSSIAN_UPPER:
            ThumbSmartDraw(data_ptr);
            break;

        case GUIIM_M_TPQ_RUSSIAN:
        case GUIIM_M_TPQ_RUSSIAN_LEAD_UPPER:
        case GUIIM_M_TPQ_RUSSIAN_LOWER:
        case GUIIM_M_TPQ_RUSSIAN_UPPER:
            QwertSmartDraw(data_ptr);
            break;
#endif            
#endif

    //french 法语
#if defined (IM_FRENCH_SUPPORT)
#if (defined MMIIM_ENGINE_EXIST)
        case GUIIM_M_TP_FRENCH:
        case GUIIM_M_TP_FRENCH_LEAD_UPPER:
        case GUIIM_M_TP_FRENCH_LOWER:
        case GUIIM_M_TP_FRENCH_UPPER:
            ThumbSmartDraw(data_ptr);
            break;

        case GUIIM_M_TPQ_FRENCH:
        case GUIIM_M_TPQ_FRENCH_LEAD_UPPER:
        case GUIIM_M_TPQ_FRENCH_LOWER:
        case GUIIM_M_TPQ_FRENCH_UPPER:
            QwertSmartDraw(data_ptr);
            break;
#endif            
#endif

    //HUNGARIAN 匈牙利
#if defined (IM_HUNGARIAN_SUPPORT)
#if (defined MMIIM_ENGINE_EXIST)
        case GUIIM_M_TP_HUNGARIAN:
        case GUIIM_M_TP_HUNGARIAN_LEAD_UPPER:
        case GUIIM_M_TP_HUNGARIAN_LOWER:
        case GUIIM_M_TP_HUNGARIAN_UPPER:
            ThumbSmartDraw(data_ptr);
            break;

        case GUIIM_M_TPQ_HUNGARIAN:
        case GUIIM_M_TPQ_HUNGARIAN_LEAD_UPPER:
        case GUIIM_M_TPQ_HUNGARIAN_LOWER:
        case GUIIM_M_TPQ_HUNGARIAN_UPPER:
            QwertSmartDraw(data_ptr);
            break;
#endif            
#endif

    //PORTUGUESE 葡萄牙
#if defined (IM_PORTUGUESE_SUPPORT)
#if (defined MMIIM_ENGINE_EXIST)
        case GUIIM_M_TP_PORTUGUESE:
        case GUIIM_M_TP_PORTUGUESE_LEAD_UPPER:
        case GUIIM_M_TP_PORTUGUESE_LOWER:
        case GUIIM_M_TP_PORTUGUESE_UPPER:
            ThumbSmartDraw(data_ptr);
            break;

        case GUIIM_M_TPQ_PORTUGUESE:
        case GUIIM_M_TPQ_PORTUGUESE_LEAD_UPPER:
        case GUIIM_M_TPQ_PORTUGUESE_LOWER:
        case GUIIM_M_TPQ_PORTUGUESE_UPPER:
            QwertSmartDraw(data_ptr);
            break;
#endif            
#endif

    //SPANISH 西班牙
#if defined (IM_SPANISH_SUPPORT)
#if (defined MMIIM_ENGINE_EXIST)
        case GUIIM_M_TP_SPANISH:
        case GUIIM_M_TP_SPANISH_LEAD_UPPER:
        case GUIIM_M_TP_SPANISH_LOWER:
        case GUIIM_M_TP_SPANISH_UPPER:
            ThumbSmartDraw(data_ptr);
            break;

        case GUIIM_M_TPQ_SPANISH:
        case GUIIM_M_TPQ_SPANISH_LEAD_UPPER:
        case GUIIM_M_TPQ_SPANISH_LOWER:
        case GUIIM_M_TPQ_SPANISH_UPPER:
            QwertSmartDraw(data_ptr);
            break;
#endif            
#endif

    //ITALIAN 意大利
#if defined (IM_ITALIAN_SUPPORT)
#if (defined MMIIM_ENGINE_EXIST)
        case GUIIM_M_TP_ITALIAN:
        case GUIIM_M_TP_ITALIAN_LEAD_UPPER:
        case GUIIM_M_TP_ITALIAN_LOWER:
        case GUIIM_M_TP_ITALIAN_UPPER:
            ThumbSmartDraw(data_ptr);
            break;

        case GUIIM_M_TPQ_ITALIAN:
        case GUIIM_M_TPQ_ITALIAN_LEAD_UPPER:
        case GUIIM_M_TPQ_ITALIAN_LOWER:
        case GUIIM_M_TPQ_ITALIAN_UPPER:
            QwertSmartDraw(data_ptr);
            break;
#endif            
#endif

    //ARABIC 阿拉伯
#if defined (IM_ARABIC_SUPPORT)
#if (defined MMIIM_ENGINE_EXIST)
        case GUIIM_M_TP_ARABIC:
            ThumbSmartDraw(data_ptr);
            break;

        case GUIIM_M_TPQ_ARABIC:
            QwertSmartDraw(data_ptr);
            break;
#endif            
#endif

    //Amharic 阿姆哈拉不是multi-tap
#if defined IM_ENGINE_CSTAR && defined (IM_AMHARIC_SUPPORT)
        case GUIIM_M_TP_AMHARIC_ABC:
            ThumbAmharicDraw(data_ptr);
            break;
#endif

#if defined (IM_UYGHUR_SUPPORT)
#if (defined MMIIM_ENGINE_EXIST)
        case GUIIM_M_TP_UYGHUR:
            ThumbSmartDraw(data_ptr);
            break;

        case GUIIM_M_TPQ_UYGHUR:
            QwertSmartDraw(data_ptr);
            break;
#endif
#endif

#if defined (IM_TIBETAN_SUPPORT)    //tibetan 2012 0529
#if 0
        case GUIIM_M_TP_TIBETAN:
            ThumbSmartDraw(data_ptr);
            break;
        case GUIIM_M_TPQ_TIBETAN:
            QwertSmartDraw(data_ptr);
            break;
#endif
		case GUIIM_M_TP_TIBETAN_ABC:
            ThumbMultiTapDraw (data_ptr);
            break;

        case GUIIM_M_TPQ_TIBETAN_ABC:  // Tibetan with multitap
            QwertMultiTapDraw (data_ptr);
            break;
#endif

    //TURKISH 土耳其
#if defined (IM_TURKISH_SUPPORT)
#if (defined MMIIM_ENGINE_EXIST)
        case GUIIM_M_TP_TURKISH:
        case GUIIM_M_TP_TURKISH_LEAD_UPPER:
        case GUIIM_M_TP_TURKISH_LOWER:
        case GUIIM_M_TP_TURKISH_UPPER:
            ThumbSmartDraw(data_ptr);
            break;

        case GUIIM_M_TPQ_TURKISH:
        case GUIIM_M_TPQ_TURKISH_LEAD_UPPER:
        case GUIIM_M_TPQ_TURKISH_LOWER:
        case GUIIM_M_TPQ_TURKISH_UPPER:
            QwertSmartDraw(data_ptr);
            break;
#endif            
#endif

	//SLOVAK 斯洛伐克
#if defined (IM_SLOVAK_SUPPORT)
#if (defined MMIIM_ENGINE_EXIST)
        case GUIIM_M_TP_SLOVAK:
        case GUIIM_M_TP_SLOVAK_LEAD_UPPER:
        case GUIIM_M_TP_SLOVAK_LOWER:
        case GUIIM_M_TP_SLOVAK_UPPER:
            ThumbSmartDraw(data_ptr);
            break;

        case GUIIM_M_TPQ_SLOVAK:
        case GUIIM_M_TPQ_SLOVAK_LEAD_UPPER:
        case GUIIM_M_TPQ_SLOVAK_LOWER:
        case GUIIM_M_TPQ_SLOVAK_UPPER:
            QwertSmartDraw(data_ptr);
            break;
#endif            
#endif

        default:
        	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
            DrawVkb(data_ptr);
            break;
        }
    }

    //如果非焦点状态
    if (!MMK_IsFocusWin(data_ptr->init_param.win_handle))
    {
        GUILCD_DRAW_DC_T dst_dc_data = {0};
        BOOLEAN is_layer_valid = LCD_PrepareDrawParam(data_ptr->init_param.lcd_info_ptr, &dst_dc_data);

#if defined MMIIM_HW_FULLSCREEN_SUPPORT
        if (!data_ptr->is_symbol_panel
            && data_ptr->init_param.method_param_ptr->im_def_ptr->method == GUIIM_M_HANDWRITING   //全屏手写状态
            ) //从手写切换到sym后，上述的输入法值依然为hw
        {
            GUI_RECT_T display_rect = {0, 0, 0, 0};

            HwFullScreenGetDisplayPanelRect (data_ptr, &display_rect);
            
            // 1 将显示信息blt到基层
            if (is_layer_valid && PNULL != dst_dc_data.lcd_buffer_ptr)
            {
                UILAYER_BltLayerToLayer(data_ptr->init_param.lcd_info_ptr,
                                        &data_ptr->ui_layer,
                                        &display_rect,
                                        FALSE);
            }
            else
            {
                GUI_LCD_DEV_INFO const main_lcd_dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

                UILAYER_BltLayerToLayer(&main_lcd_dev,
                                        &data_ptr->ui_layer,
                                        &display_rect,
                                        FALSE);
            }
        }
        else
        {
            // 1 将显示信息blt到基层
            if (is_layer_valid && PNULL != dst_dc_data.lcd_buffer_ptr)
            {
                UILAYER_BltLayerToLayer(data_ptr->init_param.lcd_info_ptr,
                                        &data_ptr->ui_layer,
                                        &data_ptr->rect,
                                        FALSE);
            }
            else
            {
                GUI_LCD_DEV_INFO const main_lcd_dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

                UILAYER_BltLayerToLayer(&main_lcd_dev,
                                        &data_ptr->ui_layer,
                                        &data_ptr->rect,
                                        FALSE);
            }
        }
#else
        // 1 将显示信息blt到基层
        if (is_layer_valid && PNULL != dst_dc_data.lcd_buffer_ptr)
        {
            UILAYER_BltLayerToLayer(data_ptr->init_param.lcd_info_ptr,
                                    &data_ptr->ui_layer,
                                    &data_ptr->rect,
                                    FALSE);
        }
        else
        {
            GUI_LCD_DEV_INFO const main_lcd_dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

            UILAYER_BltLayerToLayer(&main_lcd_dev,
                                    &data_ptr->ui_layer,
                                    &data_ptr->rect,
                                    FALSE);
        }
#endif

        //2 释放显示层
        ReleaseUILayer(data_ptr);
        ReleaseTipLayer(data_ptr);
        ReleaseEditLayer(data_ptr);
    }
    else if (data_ptr->is_tip_display)
    {
        UILAYER_APPEND_BLT_T append_layer = {0};

        UILAYER_RemoveBltLayer(&data_ptr->tip_layer);
        append_layer.lcd_dev_info = data_ptr->tip_layer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }

    return TRUE;
}


/*==============================================================================
Description: GetThemeIndex
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC int32 GetThemeIndex(KEY_INFO_T *key_ptr)
{
    uint32 index = 0;

    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return  index;
    }
    
    if (key_ptr->is_inactive)
    {
        index = KEY_STATE_INACTIVE;
    }
    else if (key_ptr->is_press)
    {
        index = KEY_STATE_PRESS;
    }
    else
    {
        index = KEY_STATE_ACTIVE;
    }

    return index;
}

/*==============================================================================
Description: 绘制按键
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void DrawKey(
    IM_DATA_T const *data_ptr,
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    KEY_INFO_T *key_ptr
)
{
    KEY_THEME_T const *theme_ptr = PNULL;
//#if defined MMIIM_HW_FULLSCREEN_SUPPORT
    KEY_THEME_T theme_special = {0};
//#endif
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != dc_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr || PNULL == dc_data_ptr)
    {
        return;
    }
    
    if (key_ptr->is_hide)
    {
        return;
    }

    //theme
    switch (key_ptr->style)
    {
    case KEY_STYLE_CONTROL:
        theme_ptr = data_ptr->theme.key_ctrl + GetThemeIndex(key_ptr);
        break;

    case KEY_STYLE_CUSTOM:
        theme_ptr = data_ptr->theme.key_cust + GetThemeIndex(key_ptr);
        break;

    case KEY_STYLE_CANDIDATE:
        theme_ptr = data_ptr->theme.key_cand + GetThemeIndex(key_ptr);
        break;

    case KEY_STYLE_SYSTEM:
        theme_ptr = data_ptr->theme.key_sys + GetThemeIndex(key_ptr);
        break;

    case KEY_STYLE_ART:
        theme_ptr = data_ptr->theme.key_art + GetThemeIndex(key_ptr);
//#if defined MMIIM_HW_FULLSCREEN_SUPPORT
        if (key_ptr->add_style.art_disp == KASA_TRANSPARENT_E)
        {
            SCI_MEMCPY (&theme_special, theme_ptr, sizeof (KEY_THEME_T));
            theme_special.border.relief = RELIEF_FLAT;
            theme_special.fill_data.pure = MMIIM_UILAYER_TRANSPARENT_COLOR;     //如果使用565层，在函数内部会转换为565
            theme_ptr = &theme_special;
        }
        else if (key_ptr->add_style.art_disp == KASA_NOT_DRAW_E)
        {
            SCI_MEMCPY (&theme_special, theme_ptr, sizeof (KEY_THEME_T));
            theme_special.border.relief = RELIEF_FLAT;
            theme_special.fill_data.pure = data_ptr->theme.panel_color;
            theme_ptr = &theme_special;
        }
        else if (key_ptr->add_style.art_disp == KASA_NOT_BG_E)
        {
            SCI_MEMCPY (&theme_special, theme_ptr, sizeof (KEY_THEME_T));
            theme_special.border.relief = RELIEF_FLAT;
            theme_special.fill = FILL_NONE;
            theme_ptr = &theme_special;
        }
//#endif        
        break;

        //case KEY_STYLE_CHARACTER:
    default:
        theme_ptr = data_ptr->theme.key_char + GetThemeIndex(key_ptr);
        break;
    }

    DrawKeyRaw(data_ptr, dc_data_ptr, theme_ptr, &key_ptr->rect, key_ptr);

    return ;
}


/*==============================================================================
Description: 特殊按键绘制
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN DrawLangSwitchKey(
    IM_DATA_T const *data_ptr,
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    KEY_INFO_T *key_ptr
)
{
#define COLOR_CURRENT_LANG (0xffffffff)
#define COLOR_PREV_LANG (0xffd9d9d9)

    KEY_THEME_T const *theme_ptr = PNULL;
    MMI_STRING_T str_info = {0};
    GUISTR_STYLE_T str_style = {0};
    int16 text_width = 0;
    GUISTR_BUF_T des_str_buf = {0};
    GUI_RECT_T key_rect = {0};
    int16 disp_height = 0;    
    BOOLEAN is_en = FALSE;
    const wchar *local_lang_str = PNULL, *en_lang_str = L"en";
    int32 normal_font = 0;     //修正字符大小    
    GUIIM_LANGUAGE_T prev_lang = GUIIM_LANG_NONE;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != dc_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr || PNULL == dc_data_ptr)
    {
        return FALSE;
    }
        
    DC_CHECK(dc_data_ptr, (&key_ptr->rect));
    
    if (key_ptr->is_hide)
    {
        //SCI_TraceLow:"DrawLangSwitchKey"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_COMMON_2267_112_2_18_2_27_10_87,(uint8*)"");
        return FALSE;
    }

    key_rect = key_ptr->rect;
    //theme
    theme_ptr = data_ptr->theme.key_ctrl + GetThemeIndex(key_ptr);
    if (theme_ptr->border.relief != RELIEF_FLAT)   //存在边框
    {
        key_rect.left += theme_ptr->border.width;
        key_rect.top += theme_ptr->border.width;
        key_rect.right -= theme_ptr->border.width;
        key_rect.bottom -= theme_ptr->border.width;
    }

    DrawKeyBg (dc_data_ptr, theme_ptr, key_rect);
        
    //text
    DcToDstStr(dc_data_ptr, &des_str_buf);

    prev_lang= MMIAPIIM_GetPrevLanguage();
    if (prev_lang != GUIIM_LANG_NONE)
    {
        if (!MMIIM_IsLimitSwitchLang (data_ptr->init_param.method_param_ptr))
        {
            local_lang_str = MMIAPIIM_GetLangCodeString_ptr(prev_lang);
        }
    }

    if (local_lang_str == PNULL)    //无本地语，仅仅显示英文字符串
    {
        if (data_ptr->init_param.method_param_ptr
            && data_ptr->init_param.method_param_ptr->im_def_ptr
            && data_ptr->init_param.method_param_ptr->im_def_ptr->method == GUIIM_M_HANDWRITING)   //手写，显示En，从Abc切换到手写时，值g_default_en_type不改变
        {
            en_lang_str = L"en";//
        }
        else if (g_default_en_type == GUIIM_TYPE_MULTITAP || g_default_en_type == GUIIM_TYPE_ABC)  //multi-tap,显示Abc
        {
            en_lang_str = L"abc";//Abc
        }
    }
    else    //有本地语，显示本地语和英文部分L"/En" or L"/Abc"
    {
        if (data_ptr->init_param.method_param_ptr
            && data_ptr->init_param.method_param_ptr->im_def_ptr
            && data_ptr->init_param.method_param_ptr->im_def_ptr->method == GUIIM_M_HANDWRITING)   //手写，显示En，从Abc切换到手写时，值g_default_en_type不改变
        {
            en_lang_str = L"/en";//En
        }
        else if (g_default_en_type == GUIIM_TYPE_MULTITAP || g_default_en_type == GUIIM_TYPE_ABC)  //multi-tap,显示Abc
        {
            en_lang_str = L"/abc";//Abc
        }
        else    //需要分隔符，不能与手写合并在一起，手写要优先判断
        {
            en_lang_str = L"/en";//En
        }
    }
    
    if (local_lang_str == PNULL)            //显示语言和输入语言均为英文，没有设置本地语，显示英文标志码字符串
    {
        local_lang_str = str_info.wstr_ptr = en_lang_str;       /*lint !e605*/
        key_ptr->print_text_len = str_info.wstr_len = MMIAPICOM_Wstrlen (en_lang_str);
        MMIAPICOM_Wstrncpy (key_ptr->print_text, local_lang_str, TEXT_LEN_LIMIT - 1);
        
        str_style.font_color = RGB888TO565(COLOR_CURRENT_LANG);
        str_style.align = ALIGN_HVMIDDLE;

        //自动调整字体的大小
        disp_height = key_rect.bottom - key_rect.top - 2;
        str_style.font = theme_ptr->text_font + key_ptr->font_size;
        if (str_style.font > disp_height) str_style.font = (GUI_FONT_T)disp_height;

        text_width = key_rect.right - key_rect.left - 2;
        while (GUI_GetStringWidth(str_style.font, key_ptr->print_text, key_ptr->print_text_len) > text_width)
        {
            if (str_style.font <= data_ptr->theme.font_min) break;

            str_style.font -= 1;
        }
        
        GUISTR_DrawTextInRect(
            &des_str_buf,
            &key_rect,
            &key_rect,
            &str_info,
            &str_style,
            GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR,
            GUISTR_TEXT_DIR_AUTO);      
        
        return TRUE;
    }

    //有设置本地输入语言，该本地输入语言为非英文
    disp_height = key_rect.bottom - key_rect.top - 2;

    //自动调整字体的大小
    str_style.font = theme_ptr->text_font + key_ptr->font_size;
    if (str_style.font > disp_height) str_style.font = (GUI_FONT_T)disp_height;

    text_width = key_rect.right - key_rect.left - 2;
    
    MMIAPICOM_Wstrncpy (key_ptr->print_text, local_lang_str, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrcat (key_ptr->print_text, en_lang_str);
    key_ptr->print_text_len = MMIAPICOM_Wstrlen (key_ptr->print_text);
    while (GUI_GetStringWidth(str_style.font, key_ptr->print_text, key_ptr->print_text_len) > text_width)
    {
        if (str_style.font <= data_ptr->theme.font_min) break;

        str_style.font -= 1;
    }
    normal_font = str_style.font;

    if (MMIAPIIM_GetLanguage() == GUIIM_LANG_ENGLISH)
    {
        is_en = TRUE;
    }
    
    //前半部分，显示本地语言标志码字符串
    str_info.wstr_ptr = local_lang_str;     /*lint !e605*/
    str_info.wstr_len = MMIAPICOM_Wstrlen (str_info.wstr_ptr);

    if (is_en)   //英文为当前语言
    {
        str_style.font =  normal_font - 4;
        str_style.font_color = RGB888TO565(COLOR_PREV_LANG);
    }
    else
    {
        str_style.font =  normal_font;
        str_style.font_color = RGB888TO565(COLOR_CURRENT_LANG);
    }
    str_style.align = ALIGN_HVMIDDLE;

    key_rect.left += data_ptr->theme.key_padding;
    key_rect.right = key_rect.left + GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, str_info.wstr_len);
    GUISTR_DrawTextInRect(
        &des_str_buf,
        &key_rect,
        &key_rect,
        &str_info,
        &str_style,
        GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR,
        GUISTR_TEXT_DIR_AUTO);

    //后半部分，显示英文标志码字符串
    str_info.wstr_ptr = en_lang_str;    /*lint !e605*/
    str_info.wstr_len = MMIAPICOM_Wstrlen (en_lang_str);

    if (is_en)   //英文为当前语言
    {
        str_style.font =  normal_font;
        str_style.font_color = RGB888TO565(COLOR_CURRENT_LANG);
    }
    else
    {
        str_style.font =  normal_font - 4;
        str_style.font_color = RGB888TO565(COLOR_PREV_LANG);
    }
    str_style.align = ALIGN_HVMIDDLE;

    key_rect.left = key_rect.right;
    key_rect.right += GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, str_info.wstr_len);
    
    GUISTR_DrawTextInRect(
        &des_str_buf,
        &key_rect,
        &key_rect,
        &str_info,
        &str_style,
        GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR,
        GUISTR_TEXT_DIR_AUTO);

    return TRUE;
}



/*==============================================================================
Description: 画按键背景，将该部分代码从DrawKeyRaw中抽出来，
                注意，key_rect是去掉了边框后的rect，边框是从内往外画的
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL void DrawKeyBg (
    GUILCD_DRAW_DC_T const *dc_data_ptr, 
    KEY_THEME_T const *theme_ptr,
    GUI_RECT_T key_rect 
    )
{
    if (SHARP_ROUND_RECTANGLE == theme_ptr->sharp)
    {
        //fill
        switch (theme_ptr->fill)
        {
        case FILL_PURE:
            _FillRoundRect(dc_data_ptr, &key_rect, theme_ptr->fill_data.pure, TRUE);
            break;

        case FILL_GRADIENT:
            _FillGradientRoundRect(dc_data_ptr, &key_rect,
                                   theme_ptr->fill_data.gradient.c0,
                                   theme_ptr->fill_data.gradient.c1,
                                   theme_ptr->fill_data.gradient.a, TRUE);
            break;

        case FILL_GRADIENT_BLOCK:
            _FillGradientBlockRoundRect(dc_data_ptr, &key_rect,
                                        theme_ptr->fill_data.gradient_block.d0.c0,
                                        theme_ptr->fill_data.gradient_block.d0.c1,
                                        theme_ptr->fill_data.gradient_block.d0.a,
                                        theme_ptr->fill_data.gradient_block.d1.c0,
                                        theme_ptr->fill_data.gradient_block.d1.c1,
                                        theme_ptr->fill_data.gradient_block.d1.a,
                                        theme_ptr->fill_data.gradient_block.d2.c0,
                                        theme_ptr->fill_data.gradient_block.d2.c1,
                                        theme_ptr->fill_data.gradient_block.d2.a,
                                        TRUE);
            break;
            
        case FILL_GRADIENT_PURE_BLOCK:
            _FillGradientPureBlockRoundRect (dc_data_ptr, &key_rect,
                                        theme_ptr->fill_data.gradient.c0,
                                        theme_ptr->fill_data.gradient.c1,
                                        theme_ptr->fill_data.gradient.a,
                                        TRUE);
            break;
            
        default:
            break;
        }

        //border
        switch (theme_ptr->border.relief)
        {
        case RELIEF_RAISED:
        case RELIEF_SUNKEN:
        case RELIEF_SOLID:
            _DrawRoundBorderUneven(dc_data_ptr, &key_rect,
                                   theme_ptr->border.width, theme_ptr->border.relief,
                                   theme_ptr->border.color,
                                   theme_ptr->border.color_bright,
                                   theme_ptr->border.color_dark);
            break;

        case RELIEF_RIDGE:
        case RELIEF_GROOVE:
            _DrawRoundBorderSharp(dc_data_ptr, &key_rect,
                                  theme_ptr->border.width, theme_ptr->border.relief,
                                  theme_ptr->border.color,
                                  theme_ptr->border.color_bright,
                                  theme_ptr->border.color_dark);
            break;

        case RELIEF_INSET_BRIGHT:
        case RELIEF_INSET_DARK:
            _DrawRoundBorderInset(dc_data_ptr, &key_rect,
                                  theme_ptr->border.width, theme_ptr->border.relief,
                                  theme_ptr->border.color_bright,
                                  theme_ptr->border.color_dark);
            break;

        default:
            break;
        }
    }
    else
    {
        //fill
        switch (theme_ptr->fill)
        {
        case FILL_PURE:
            _FillRect(dc_data_ptr, &key_rect, theme_ptr->fill_data.pure);
            break;

        case FILL_GRADIENT:
            _FillGradientRect(dc_data_ptr, &key_rect,
                              theme_ptr->fill_data.gradient.c0,
                              theme_ptr->fill_data.gradient.c1,
                              theme_ptr->fill_data.gradient.a);
            break;

        case FILL_GRADIENT_BLOCK:
            break;

        default:
            break;
        }

        //border
        switch (theme_ptr->border.relief)
        {
        case RELIEF_RAISED:
        case RELIEF_SUNKEN:
        case RELIEF_SOLID:
            _DrawBorderUneven(dc_data_ptr, &key_rect,
                              theme_ptr->border.width, theme_ptr->border.relief,
                              theme_ptr->border.color,
                              theme_ptr->border.color_bright,
                              theme_ptr->border.color_dark);
            break;

        case RELIEF_RIDGE:
        case RELIEF_GROOVE:
            _DrawBorderSharp(dc_data_ptr, &key_rect,
                             theme_ptr->border.width, theme_ptr->border.relief,
                             theme_ptr->border.color,
                             theme_ptr->border.color_bright,
                             theme_ptr->border.color_dark);
            break;

        default:
            break;
        }
    }

}

/*==============================================================================
Description: 绘制按键 - 已知theme
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void DrawKeyRaw(
    IM_DATA_T const *data_ptr,
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    KEY_THEME_T const *theme_ptr,
    GUI_RECT_T const *rect_ptr,
    KEY_INFO_T *key_ptr
)
{
    GUISTR_BUF_T des_str_buf = {0};
    GUI_RECT_T key_rect = {0};
    int16 disp_height = 0;
    const uint8 key_padding = 2;    //字符显示与边缘的间距
    GUI_RECT_T str_rect = {0};
    int16 subtext_width = 0;
    GUISTR_STATE_T str_state = GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR | GUISTR_STATE_BOLD;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != dc_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != theme_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != rect_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == dc_data_ptr || PNULL == theme_ptr||PNULL == rect_ptr||PNULL == key_ptr)
    {
        return;
    }
    
    DC_CHECK(dc_data_ptr, rect_ptr);

    key_rect = *rect_ptr;
    if (theme_ptr->border.relief != RELIEF_FLAT)   //存在边框
    {
        key_rect.left += theme_ptr->border.width;
        key_rect.top += theme_ptr->border.width;
        key_rect.right -= theme_ptr->border.width;
        key_rect.bottom -= theme_ptr->border.width;
    }

    //画背景
    DrawKeyBg (dc_data_ptr, theme_ptr, key_rect);
    
    //icon & text
    //icon first
    if (key_ptr->icon)
    {
        BOOLEAN has_get_img = FALSE;
        uint16 img_width = 0, img_height = 0;
        int16 offset_x = 0, offset_y = 0;
        GUI_RECT_T disp_rect = {0};
        GUI_RECT_T img_rect = {0};

        has_get_img = GUIRES_GetImgWidthHeight(&img_width,
                                               &img_height,
                                               key_ptr->icon,
                                               data_ptr->init_param.win_id);

        if (has_get_img)
        {
            offset_x = (key_rect.right - key_rect.left - 2 - img_width) / 2;
            offset_y = (key_rect.bottom - key_rect.top - 2 - img_height) / 2;

            disp_rect = key_rect;

            if (offset_x > 0)
            {
                disp_rect.left += offset_x;
                img_rect.left = 0;
            }
            else
            {
                img_rect.left = (-1) * offset_x;
            }

            if (offset_y > 0)
            {
                disp_rect.top += offset_y;
                img_rect.top = 0;
            }
            else
            {
                img_rect.top = (-1) * offset_y;
            }

            img_rect.right = img_width - 1;
            img_rect.bottom = img_height - 1;

            GUIRES_DisplayImgDst(PNULL,
                                 &disp_rect,
                                 &img_rect,
                                 data_ptr->init_param.win_id,
                                 key_ptr->icon,
                                 dc_data_ptr);
        }
    }

    //then text
    DcToDstStr(dc_data_ptr, &des_str_buf);

    disp_height = key_rect.bottom - key_rect.top;

    //subtext
    if (key_ptr->subtext_len > 0)
    {
        BOOLEAN cross = FALSE;  //sub text 与一般字符之间是否需要交错显示，不交错，则上下居中显示
        
        if (key_ptr->style == KEY_STYLE_CHARACTER)      //不同的字符数，显示方式不同
        {
            switch (key_ptr->add_style.text_disp & KASC_TEXT_ATTR)
            {
                case KASC_FIRST:        //显示第一个字符
                case KASC_ALL_TEXT:     //单行显示所有字符
                case KASC_EXCLUDE_DIGIT:    //单行显示,排除最后一位数字
                {
                    cross = TRUE;
                    break;
                }
                    
                case KASC_2X4_EXCLUDE_DIGIT:    //两行显示，最后如果为数字，不显示，每行最多显示4个字符
                case KASC_2LINE:         //两行显示，最后如果为数字，不显示，每行最多显示4个字符
                default:
                    cross = TRUE;       //UI设计要求如此
                    break;
            }

			if ((key_ptr->add_style.text_disp & KASC_SUBTEXT_TEXT_ATTR) == KASC_SUBTEXT_MIDDLE)		//居中显示时，不需要交错距离
    		{
				cross = FALSE;
			}
        }  
        
        if (cross)      //subtext 和text交错显示
        {
            MMI_STRING_T str_info = {0};
            GUISTR_STYLE_T str_style = {0};

            str_style.font = theme_ptr->subtext_font;
            str_info.wstr_ptr = key_ptr->subtext;
            str_info.wstr_len = key_ptr->subtext_len;

            str_style.font_color = RGB888TO565(theme_ptr->subtext_color);
            str_style.align = ALIGN_LVMIDDLE;    //ALIGN_LVMIDDLE;

            if (str_style.font > disp_height)
                str_style.font = (GUI_FONT_T)disp_height;

            //在有subtext的时候，需要留空
            str_rect = key_rect;
            
            str_rect.left += key_padding;
	    	if ((key_ptr->add_style.text_disp & KASC_SUBTEXT_TEXT_ATTR) == KASC_SUBTEXT_MIDDLE)		//居中显示时，不需要交错距离
    		{
				;
			}
			else
			{
	            str_rect.right = str_rect.left + (key_rect.right - key_rect.left) * THUMB_SUBTEXT_WIDTH_SIZE_RATIO / 100;
			}
            str_rect.bottom = str_rect.top + (key_rect.bottom - key_rect.top) * THUMB_SUBTEXT_HEIGHT_SIZE_RATIO / 100;

            while (GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, str_info.wstr_len) > str_rect.right - str_rect.left)    //
            {
                if (str_style.font <= data_ptr->theme.font_min)
                    break;

                str_style.font -= 1;
            }
            str_style.font = str_style.font < (key_rect.bottom - key_rect.top) * THUMB_SUBTEXT_HEIGHT_SIZE_RATIO / 100 ? 
                str_style.font : (key_rect.bottom - key_rect.top) * THUMB_SUBTEXT_HEIGHT_SIZE_RATIO / 100;

            GUISTR_DrawTextInRect(
                &des_str_buf,
                &str_rect,
                &str_rect,
                &str_info,
                &str_style,
                GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN_BY_DIR,
                GUISTR_TEXT_DIR_LTR);       //GUISTR_TEXT_DIR_AUTO);暂时强制改为左侧显示，原因是某些右到左显示语言和text字符坐标会重合

            //不再直接做偏移量，如果字符有两行，则需要偏移量，否则不需要
            subtext_width = GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, str_info.wstr_len);          
        }
        else
        {
            MMI_STRING_T str_info = {0};
            GUISTR_STYLE_T str_style = {0};

            str_style.font = theme_ptr->subtext_font;
            str_info.wstr_ptr = key_ptr->subtext;
            str_info.wstr_len = key_ptr->subtext_len;

            str_style.font_color = RGB888TO565(theme_ptr->subtext_color);
            str_style.align = ALIGN_HVMIDDLE;    //ALIGN_LVMIDDLE;

            if (str_style.font > disp_height)
                str_style.font = (GUI_FONT_T)disp_height;

            str_rect = key_rect;
			str_rect.bottom = str_rect.top + (key_rect.bottom - key_rect.top) * THUMB_MIDDLE_SUBTEXT_HEIGHT_SIZE_RATIO / 100;
            while (GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, str_info.wstr_len) > str_rect.right - str_rect.left)    //
            {
                if (str_style.font <= data_ptr->theme.font_min)
                    break;

                str_style.font -= 1;
            }
            str_style.font = str_style.font < (str_rect.bottom - str_rect.top) ? 
                str_style.font : (str_rect.bottom - str_rect.top);

            GUISTR_DrawTextInRect(
                &des_str_buf,
                &str_rect,
                &str_rect,
                &str_info,
                &str_style,
                GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN_BY_DIR,
                GUISTR_TEXT_DIR_LTR);       //GUISTR_TEXT_DIR_AUTO);暂时强制改为左侧显示，原因是某些右到左显示语言和text字符坐标会重合
        }
    }

    //字符显示方式
    if (key_ptr->print_text_len > 0)      //存在字符
    {
        //字符按键，丝印从数据表读取的数据，需要根据情况来显示
        if (key_ptr->style == KEY_STYLE_CHARACTER)
        {
            switch (key_ptr->add_style.text_disp & KASC_TEXT_ATTR)
            {
                //单行显示所有字符
            case KASC_ALL_TEXT:
                {
                    MMI_STRING_T str_info = {0};
                    GUISTR_STYLE_T str_style = {0};

                    //往左下方偏移一些像素，防止字符显示不下
                    if (key_ptr->subtext_len > 0)   //存在subtext, 2/5 高度区域显示subtext
                    {
                        //key_rect.left -= key_padding;
                        //key_rect.top += (rect_ptr->bottom - rect_ptr->top) * THUMB_SUBTEXT_WIDTH_SIZE_RATIO / 100;
                    }
                    
                    str_info.wstr_ptr = key_ptr->print_text;
                    str_info.wstr_len = key_ptr->print_text_len;

                    str_style.font = (GUI_FONT_T)key_ptr->font_size;
                    str_style.font_color = RGB888TO565(theme_ptr->text_color);
                    str_style.align = ALIGN_HVMIDDLE;
                    str_style.char_space = 1;

                    GUISTR_DrawTextInRect(
                        &des_str_buf,
                        &key_rect,
                        &key_rect,
                        &str_info,
                        &str_style,
                        str_state,
                        GUISTR_TEXT_DIR_AUTO);

                    break;
                }

                //显示第一个字符
            case KASC_FIRST:
                {
                    MMI_STRING_T str_info = {0};
                    GUISTR_STYLE_T str_style = {0};
                    
                    str_info.wstr_ptr = key_ptr->print_text;
                    str_info.wstr_len = 1;

                    str_style.font = (GUI_FONT_T)key_ptr->font_size;
                    str_style.font_color = RGB888TO565(theme_ptr->text_color);
                    str_style.align = ALIGN_HVMIDDLE;

                    GUISTR_DrawTextInRect(
                        &des_str_buf,
                        &key_rect,
                        &key_rect,
                        &str_info,
                        &str_style,
                        str_state,
                        GUISTR_TEXT_DIR_AUTO);

                    break;
                }

                //两行显示，最后如果为数字，不显示，每行最多显示4个字符
            case KASC_2X4_EXCLUDE_DIGIT:
                {
                    MMI_STRING_T str_info = {0};
                    GUISTR_STYLE_T str_style = {0};
                    int16 ver_offset = 0, ver_mod = 0;
                    int16 first_line_cnt = 0, second_line_cnt = 0;
                    GUI_RECT_T rect_str = {0};
                    wchar tmp_text[TEXT_LEN_LIMIT] = {0};   //key_text中的数字可能不是最后一位
                    int16 iloop = 0;
                    
                    //去除subtext所占用的宽度
                    if (key_ptr->subtext_len > 0)   //存在subtext
                    {
				    	if ((key_ptr->add_style.text_disp & KASC_SUBTEXT_TEXT_ATTR) == KASC_SUBTEXT_MIDDLE)		//居中显示时，不需要交错距离
			    		{
							;
						}
						else
						{
	                        key_rect.left += (key_rect.right - key_rect.left) * THUMB_SUBTEXT_WIDTH_SIZE_RATIO / 100;

						}
                	}
                    rect_str = key_rect;
                    
                    str_state = GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR;     //不加粗
                    str_style.font = (GUI_FONT_T)key_ptr->font_size;
                    str_style.font_color = RGB888TO565(theme_ptr->text_color);
                    str_style.align = ALIGN_HVMIDDLE;

                    SCI_MEMSET (tmp_text, 0, sizeof (tmp_text));
                    str_info.wstr_ptr = tmp_text;
                    str_info.wstr_len = 0;

                    //数字字符均不需要显示
                    for (iloop = 0; iloop < key_ptr->print_text_len; iloop ++)        //此处已经约定key_ptr->print_text_len < TEXT_LEN_LIMIT
                    {
                        if (key_ptr->print_text[iloop] > '9' || key_ptr->print_text[iloop] < '0')   //非数字字符
                        {
                            tmp_text[str_info.wstr_len ++] = key_ptr->print_text[iloop];
                        }
                    }

                    if (str_info.wstr_len > 4)      //超过4个，两行显示
                    {
                        first_line_cnt = str_info.wstr_len / 2;       //每行显示字符个数
                        first_line_cnt = first_line_cnt < 4 ? first_line_cnt : 4;
                        second_line_cnt = str_info.wstr_len - first_line_cnt;
                        second_line_cnt = second_line_cnt < 4 ? second_line_cnt : 4;

                        //两行显示时，高度需要做一个调整，将空白部分5等分，顶部2，中间间隔1，底部2
                        ver_offset = (key_rect.bottom - key_rect.top - 2 * str_style.font) / 5;
                        ver_mod = (key_rect.bottom - key_rect.top - 2 * str_style.font - ver_offset * 5 - 2) / 2;

                        //第一行
                        rect_str.top = key_rect.top + 2 * ver_offset + ver_mod;
                        rect_str.bottom = rect_str.top + str_style.font;

                        str_info.wstr_len = first_line_cnt;
                        GUISTR_DrawTextInRect(
                            &des_str_buf,
                            &rect_str,
                            &rect_str,
                            &str_info,
                            &str_style,
                            str_state,
                            GUISTR_TEXT_DIR_AUTO);

                        //第二行
                        rect_str.top = key_rect.top + 3 * ver_offset + str_style.font + ver_mod;
                        rect_str.bottom = rect_str.top + str_style.font;

                        str_info.wstr_ptr += first_line_cnt;
                        str_info.wstr_len = second_line_cnt;
                        GUISTR_DrawTextInRect(
                            &des_str_buf,
                            &rect_str,
                            &rect_str,
                            &str_info,
                            &str_style,
                            str_state,
                            GUISTR_TEXT_DIR_AUTO);
                    }
                    //显示全部字符，排除数字
                    else
                    {
                        str_style.char_space = 1;
                        
                        GUISTR_DrawTextInRect(
                            &des_str_buf,
                            &key_rect,
                            &key_rect,
                            &str_info,
                            &str_style,
                            str_state,
                            GUISTR_TEXT_DIR_AUTO);
                    }

                    break;
                }

                //两行显示，最后如果为数字，不显示，每行最多显示4个字符
            case KASC_2LINE:
                {
                    MMI_STRING_T str_info = {0};
                    GUISTR_STYLE_T str_style = {0};
                    int16 text_width = 0, ver_offset = 0, ver_mod = 0;
                    int16 first_line_cnt = 0, second_line_cnt = 0;
                    GUI_RECT_T rect_str = key_rect;

                    str_style.font = (GUI_FONT_T) key_ptr->font_size;
                    str_style.font_color = RGB888TO565(theme_ptr->text_color);
                    str_style.align = ALIGN_HVMIDDLE;
                    
                    text_width = key_rect.right - key_rect.left - 2;

                    str_info.wstr_ptr = key_ptr->print_text;
                    str_info.wstr_len = key_ptr->print_text_len;

                    if (str_info.wstr_len > 4)      //超过4个，两行显示
                    {
                        first_line_cnt = str_info.wstr_len / 2;       //每行显示字符个数
                        second_line_cnt = str_info.wstr_len - first_line_cnt;

                        //自动调整字体的大小
                        if (str_style.font > (disp_height >> 1)) str_style.font = (GUI_FONT_T)(disp_height >> 1);//如果是两行显示字体必须除2 modified by arvin

                        while (GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, first_line_cnt) > text_width)    //
                        {
                            if (str_style.font <= data_ptr->theme.font_min)
                                break;

                            str_style.font -= 1;
                        }

                        //两行字符稍微缩小点，不然看起来与一行显示大小一样，有些拥挤
                        if (str_style.font  != data_ptr->theme.font_min && str_style.font  > data_ptr->theme.font_min + 2)
                            str_style.font -= 2;

                        //两行显示时，高度需要做一个调整，将空白部分5等分，顶部2，中间间隔1，底部2
                        ver_offset = (key_rect.bottom - key_rect.top - 2 * str_style.font) / 5;
                        ver_mod = (key_rect.bottom - key_rect.top - 2 * str_style.font - ver_offset * 5 - 2) / 2;

                        //第一行
                        rect_str.top = key_rect.top + 2 * ver_offset + ver_mod;
                        rect_str.bottom = rect_str.top + str_style.font;

                        str_info.wstr_len = first_line_cnt;
                        GUISTR_DrawTextInRect(
                            &des_str_buf,
                            &rect_str,
                            &rect_str,
                            &str_info,
                            &str_style,
                            GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR,
                            GUISTR_TEXT_DIR_AUTO);

                        //第二行
                        rect_str.top = key_rect.top + 3 * ver_offset + str_style.font + ver_mod;
                        rect_str.bottom = rect_str.top + str_style.font;

                        str_info.wstr_ptr += first_line_cnt;
                        str_info.wstr_len = second_line_cnt;
                        GUISTR_DrawTextInRect(
                            &des_str_buf,
                            &rect_str,
                            &rect_str,
                            &str_info,
                            &str_style,
                            GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR,
                            GUISTR_TEXT_DIR_AUTO);
                    }
                    //显示全部字符，排除数字
                    else
                    {
                        str_style.char_space = 1;
                        text_width = key_rect.right - key_rect.left - 2 - str_style.char_space * str_info.wstr_len - key_padding;
                        
                        //自动调整字体的大小
                        if (str_style.font > disp_height)
                        {
                            str_style.font = (GUI_FONT_T)(disp_height);
                        }

                        while (GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, str_info.wstr_len) > text_width)
                        {
                            if (str_style.font <= data_ptr->theme.font_min)
                                break;

                            str_style.font -= 1;
                        }

                        GUISTR_DrawTextInRect(
                            &des_str_buf,
                            &key_rect,
                            &key_rect,
                            &str_info,
                            &str_style,
                            str_state,
                            GUISTR_TEXT_DIR_AUTO);
                    }

                    break;
                }
            case KASC_EXCLUDE_DIGIT:
            default:
                {
                    MMI_STRING_T str_info = {0};
                    GUISTR_STYLE_T str_style = {0};
                    wchar tmp_text[TEXT_LEN_LIMIT] = {0};   //key_text中的数字可能不是最后一位
                    int16 iloop = 0;
                    
                    SCI_MEMSET (tmp_text, 0, sizeof (tmp_text));
                    str_info.wstr_ptr = tmp_text;
                    str_info.wstr_len = 0;

                    //数字字符均不需要显示
                    for (iloop = 0; iloop < key_ptr->print_text_len; iloop ++)        //此处已经约定key_ptr->print_text_len < TEXT_LEN_LIMIT
                    {
                        if (key_ptr->print_text[iloop] > '9' || key_ptr->print_text[iloop] < '0')   //非数字字符
                        {
                            tmp_text[str_info.wstr_len ++] = key_ptr->print_text[iloop];
                        }
                    }

                    str_style.font = (GUI_FONT_T) key_ptr->font_size;
                    str_style.font_color = RGB888TO565(theme_ptr->text_color);
                    str_style.align = ALIGN_HVMIDDLE;
                    str_style.char_space = 1;

                    //往左下方偏移一些像素，防止字符显示不下
                    if (key_ptr->subtext_len > 0)   
                    {
                        //key_rect.left -= key_padding;
            	    	if ((key_ptr->add_style.text_disp & KASC_SUBTEXT_TEXT_ATTR) == KASC_SUBTEXT_MIDDLE)		//居中显示时，不需要交错距离
			    		{
							key_rect.top += (key_rect.bottom - key_rect.top) * (THUMB_MIDDLE_SUBTEXT_HEIGHT_SIZE_RATIO - THUMB_HEIGHT_SIZE_CROSS_RATIO) / 100;
							key_rect.top += 2;	//str_style.align无法底部对齐且剧中显示组合，只能将顶部往下移动2个像素
						}
						else
						{
	                        key_rect.left += (key_rect.right - key_rect.left) * THUMB_SUBTEXT_WIDTH_SIZE_RATIO / 100;
							key_rect.top += (key_rect.bottom - key_rect.top) * (100 - THUMB_SUBTEXT_HEIGHT_SIZE_RATIO) / (100 * 2);
						}
                    }
                    
                    GUISTR_DrawTextInRect(
                        &des_str_buf,
                        &key_rect,
                        &key_rect,
                        &str_info,
                        &str_style,
                        str_state,
                        GUISTR_TEXT_DIR_AUTO);

                    break;
                }
            }
        }
        //非字符按键
        else
        {
            MMI_STRING_T str_info = {0};
            GUISTR_STYLE_T str_style = {0};
            int16 text_width = 0;

            str_info.wstr_ptr = key_ptr->print_text;
            str_info.wstr_len = key_ptr->print_text_len;

            str_style.font = theme_ptr->text_font + key_ptr->font_size;
            str_style.font_color = RGB888TO565(theme_ptr->text_color);
            str_style.align = ALIGN_HVMIDDLE;

            text_width = key_rect.right - key_rect.left - 2;    //边框2个像素
 
            //自动调整字体的大小
            if (str_style.font > disp_height) str_style.font = (GUI_FONT_T)disp_height;

            while (GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, str_info.wstr_len) > text_width)
            {
                if (str_style.font <= data_ptr->theme.font_min) break;

                str_style.font -= 1;
            }

            GUISTR_DrawTextInRect(
                &des_str_buf,
                &key_rect,
                &key_rect,
                &str_info,
                &str_style,
                GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR,
                GUISTR_TEXT_DIR_AUTO);
        }
    }

    return ;
}



/*==============================================================================
Description: 绘制tips按键 - 其实这里有一个简单的办法，直接键按键绘制数据缩放
                    后，再在层中绘制即可，但是当前还不知道怎么缩放
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void DrawTipKey(
    IM_DATA_T const *data_ptr,
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    KEY_THEME_T const *theme_ptr,
    GUI_RECT_T const *rect_ptr,
    KEY_INFO_T *key_ptr
)
{
    GUISTR_BUF_T des_str_buf = {0};
    GUI_RECT_T key_rect = {0};
    int16 disp_height = 0;
    const uint8 key_padding = 2;    //字符显示与边缘的间距
    GUI_RECT_T str_rect = {0};
    int16 subtext_width = 0;
    GUISTR_STATE_T str_state = GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR | GUISTR_STATE_BOLD;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != dc_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != theme_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != rect_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == dc_data_ptr || PNULL == theme_ptr||PNULL == rect_ptr||PNULL == key_ptr)
    {
        return;
    }
    
    DC_CHECK(dc_data_ptr, rect_ptr);

    key_rect = *rect_ptr;
    if (theme_ptr->border.relief != RELIEF_FLAT)   //存在边框
    {
        key_rect.left += theme_ptr->border.width;
        key_rect.top += theme_ptr->border.width;
        key_rect.right -= theme_ptr->border.width;
        key_rect.bottom -= theme_ptr->border.width;
    }

    //画背景
    DrawKeyBg (dc_data_ptr, theme_ptr, key_rect);
    
    //icon & text
    //icon first
    if (key_ptr->icon)
    {
        BOOLEAN has_get_img = FALSE;
        uint16 img_width = 0, img_height = 0;
        int16 offset_x = 0, offset_y = 0;
        GUI_RECT_T disp_rect = {0};
        GUI_RECT_T img_rect = {0};

        has_get_img = GUIRES_GetImgWidthHeight(&img_width,
                                               &img_height,
                                               key_ptr->icon,
                                               data_ptr->init_param.win_id);

        if (has_get_img)
        {
            offset_x = (key_rect.right - key_rect.left - 2 - img_width) / 2;
            offset_y = (key_rect.bottom - key_rect.top - 2 - img_height) / 2;

            disp_rect = key_rect;

            if (offset_x > 0)
            {
                disp_rect.left += offset_x;
                img_rect.left = 0;
            }
            else
            {
                img_rect.left = (-1) * offset_x;
            }

            if (offset_y > 0)
            {
                disp_rect.top += offset_y;
                img_rect.top = 0;
            }
            else
            {
                img_rect.top = (-1) * offset_y;
            }

            img_rect.right = img_width - 1;
            img_rect.bottom = img_height - 1;

            GUIRES_DisplayImgDst(PNULL,
                                 &disp_rect,
                                 &img_rect,
                                 data_ptr->init_param.win_id,
                                 key_ptr->icon,
                                 dc_data_ptr);
        }
    }

    //then text
    DcToDstStr(dc_data_ptr, &des_str_buf);

    disp_height = key_rect.bottom - key_rect.top;

    //subtext
    if (key_ptr->subtext_len > 0)
    {
        BOOLEAN cross = FALSE;  //sub text 与一般字符之间是否需要交错显示，不交错，则上下居中显示
        
        if (key_ptr->style == KEY_STYLE_CHARACTER)      //不同的字符数，显示方式不同
        {
            switch (key_ptr->add_style.text_disp & KASC_TEXT_ATTR)
            {
                case KASC_FIRST:        //显示第一个字符
                case KASC_ALL_TEXT:     //单行显示所有字符
                case KASC_EXCLUDE_DIGIT:    //单行显示,排除最后一位数字
                {
                    cross = TRUE;
                    break;
                }
                    
                case KASC_2X4_EXCLUDE_DIGIT:    //两行显示，最后如果为数字，不显示，每行最多显示4个字符
                case KASC_2LINE:         //两行显示，最后如果为数字，不显示，每行最多显示4个字符
                default:
                    cross = TRUE;       //UI设计要求如此
                    break;
            }

	    	if ((key_ptr->add_style.text_disp & KASC_SUBTEXT_TEXT_ATTR) == KASC_SUBTEXT_MIDDLE)		//居中显示时，不需要交错距离
    		{
				cross = FALSE;
			}
        }  
        
        if (cross)      //subtext 和text交错显示
        {
            MMI_STRING_T str_info = {0};
            GUISTR_STYLE_T str_style = {0};

            str_style.font = theme_ptr->subtext_font;
            str_info.wstr_ptr = key_ptr->subtext;
            str_info.wstr_len = key_ptr->subtext_len;

            str_style.font_color = RGB888TO565(theme_ptr->subtext_color);
            str_style.align = ALIGN_LVMIDDLE;    //ALIGN_LVMIDDLE;

            //在有subtext的时候，需要留空
            str_rect = key_rect;
            
            str_rect.left += key_padding;
            str_rect.right = str_rect.left + (key_rect.right - key_rect.left) * THUMB_SUBTEXT_WIDTH_SIZE_RATIO / 100;
            str_rect.bottom = str_rect.top + (key_rect.bottom - key_rect.top) * THUMB_SUBTEXT_HEIGHT_SIZE_RATIO / 100;

            if (str_style.font > disp_height || str_style.font > str_rect.bottom - str_rect.top)
            {
                str_style.font = (GUI_FONT_T)disp_height < (GUI_FONT_T) (str_rect.bottom - str_rect.top) ? 
                    (GUI_FONT_T)disp_height : (GUI_FONT_T)(str_rect.bottom - str_rect.top);
            }

            while (GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, str_info.wstr_len) > str_rect.right - str_rect.left)    //
            {
                if (str_style.font <= data_ptr->theme.font_min)
                    break;

                str_style.font -= 1;
            }

            GUISTR_DrawTextInRect(
                &des_str_buf,
                &str_rect,
                &str_rect,
                &str_info,
                &str_style,
                GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN_BY_DIR,
                GUISTR_TEXT_DIR_LTR);       //GUISTR_TEXT_DIR_AUTO);暂时强制改为左侧显示，原因是某些右到左显示语言和text字符坐标会重合

            //不再直接做偏移量，如果字符有两行，则需要偏移量，否则不需要
            subtext_width = GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, str_info.wstr_len);          
        }
        else
        {
            MMI_STRING_T str_info = {0};
            GUISTR_STYLE_T str_style = {0};

            str_style.font = theme_ptr->subtext_font;
            str_info.wstr_ptr = key_ptr->subtext;
            str_info.wstr_len = key_ptr->subtext_len;

            str_style.font_color = RGB888TO565(theme_ptr->subtext_color);
            str_style.align = ALIGN_LVMIDDLE;    //ALIGN_LVMIDDLE;

            str_rect = key_rect;
	    	if ((key_ptr->add_style.text_disp & KASC_SUBTEXT_TEXT_ATTR) == KASC_SUBTEXT_MIDDLE)		//居中显示时，不需要交错距离
    		{
    			disp_height = (key_rect.bottom - key_rect.top) * THUMB_MIDDLE_SUBTEXT_HEIGHT_SIZE_RATIO / 100;
				str_rect.bottom = key_rect.top + disp_height;
				str_style.align = ALIGN_HVMIDDLE;
			}
            else
			{
				str_rect.right = str_rect.left + (key_rect.right - key_rect.left) * THUMB_SUBTEXT_WIDTH_SIZE_RATIO / 100;
        	}
			
            if (str_style.font > disp_height)
                str_style.font = (GUI_FONT_T)disp_height;

			while (GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, str_info.wstr_len) > str_rect.right - str_rect.left)    //
            {
                if (str_style.font <= data_ptr->theme.font_min)
                    break;

                str_style.font -= 1;
            }

            GUISTR_DrawTextInRect(
                &des_str_buf,
                &str_rect,
                &str_rect,
                &str_info,
                &str_style,
                GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN_BY_DIR,
                GUISTR_TEXT_DIR_LTR);       //GUISTR_TEXT_DIR_AUTO);暂时强制改为左侧显示，原因是某些右到左显示语言和text字符坐标会重合

            //不再直接做偏移量，如果字符有两行，则需要偏移量，否则不需要
            subtext_width = GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, str_info.wstr_len);          
        }
    }

    //字符显示方式
    if (key_ptr->print_text_len > 0)      //存在字符
    {
        //字符按键，丝印从数据表读取的数据，需要根据情况来显示
        if (key_ptr->style == KEY_STYLE_CHARACTER)
        {
			KEY_ADD_STYLE_CHARACTER_E text_disp_style = (KEY_ADD_STYLE_CHARACTER_E)(key_ptr->add_style.text_disp & KASC_TEXT_ATTR);

			//display all characters for cur key pressed as an actual tip for user under QWERTY vkb
			if (KASC_FIRST == text_disp_style
				&& GUIIM_TYPE_MULTITAP == data_ptr->init_param.method_param_ptr->im_set.type
				&& GUIIM_INPUT_TYPE_TP_QWERT == data_ptr->init_param.method_param_ptr->im_set.input)
			{
				text_disp_style = KASC_ALL_TEXT;
			}

            switch (text_disp_style)
            {
                //单行显示所有字符
            case KASC_ALL_TEXT:
                {
                    MMI_STRING_T str_info = {0};
                    GUISTR_STYLE_T str_style = {0};

                    //往左下方偏移一些像素，防止字符显示不下
                    if (key_ptr->subtext_len > 0)   //存在subtext, 2/5 高度区域显示subtext
                    {
                        //key_rect.left -= key_padding;
                        //key_rect.top += (rect_ptr->bottom - rect_ptr->top) * THUMB_SUBTEXT_WIDTH_SIZE_RATIO / 100;
                    }
                    
                    str_info.wstr_ptr = key_ptr->print_text;
                    str_info.wstr_len = key_ptr->print_text_len;

                    str_style.font = (GUI_FONT_T) key_ptr->font_size * TIP_EXPAND_MULTIPLE;
                    str_style.font_color = RGB888TO565(theme_ptr->text_color);
                    str_style.align = ALIGN_HVMIDDLE;
                    str_style.char_space = 1;

                    while (GUISTR_GetStringWidth (&str_style, &str_info, str_state) > key_rect.right - key_rect.left)
                    {
                        if (str_style.font <= data_ptr->theme.font_min) 
                        {
                            break;
                        }

                        str_style.font -= 1;
                    }

                    GUISTR_DrawTextInRect(
                        &des_str_buf,
                        &key_rect,
                        &key_rect,
                        &str_info,
                        &str_style,
                        str_state,
                        GUISTR_TEXT_DIR_AUTO);

                    break;
                }

                //显示第一个字符
            case KASC_FIRST:
                {
                    MMI_STRING_T str_info = {0};
                    GUISTR_STYLE_T str_style = {0};

                    str_info.wstr_ptr = key_ptr->print_text;
                    str_info.wstr_len = 1;

                    str_style.font = (GUI_FONT_T) key_ptr->font_size * TIP_EXPAND_MULTIPLE;
                    str_style.font_color = RGB888TO565(theme_ptr->text_color);
                    str_style.align = ALIGN_HVMIDDLE;

                    while (GUISTR_GetStringWidth (&str_style, &str_info, str_state) > key_rect.right - key_rect.left)
                    {
                        if (str_style.font <= data_ptr->theme.font_min) 
                        {
                            break;
                        }

                        str_style.font -= 1;
                    }

                    GUISTR_DrawTextInRect(
                        &des_str_buf,
                        &key_rect,
                        &key_rect,
                        &str_info,
                        &str_style,
                        str_state,
                        GUISTR_TEXT_DIR_AUTO);

                    break;
                }

                //两行显示，最后如果为数字，不显示，每行最多显示4个字符
            case KASC_2X4_EXCLUDE_DIGIT:
                {
                    MMI_STRING_T str_info = {0};
                    GUISTR_STYLE_T str_style = {0};
                    int16 ver_offset = 0, ver_mod = 0;
                    int16 first_line_cnt = 0, second_line_cnt = 0;
                    GUI_RECT_T rect_str = {0};
                    wchar tmp_text[TEXT_LEN_LIMIT] = {0};   //key_text中的数字可能不是最后一位
                    int16 iloop = 0;
                    
                    //去除subtext所占用的宽度
                    if (key_ptr->subtext_len > 0)   //存在subtext
                    {
                        key_rect.left += (key_rect.right - key_rect.left) * THUMB_SUBTEXT_WIDTH_SIZE_RATIO / 100;
                    }

                    rect_str = key_rect;
                    
                    str_state = GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR;     //不加粗
                    str_style.font = (GUI_FONT_T) key_ptr->font_size * TIP_EXPAND_MULTIPLE;
                    str_style.font_color = RGB888TO565(theme_ptr->text_color);
                    str_style.align = ALIGN_HVMIDDLE;

                    SCI_MEMSET (tmp_text, 0, sizeof (tmp_text));
                    str_info.wstr_ptr = tmp_text;
                    str_info.wstr_len = 0;

                    //数字字符均不需要显示
                    for (iloop = 0; iloop < key_ptr->print_text_len; iloop ++)        //此处已经约定key_ptr->print_text_len < TEXT_LEN_LIMIT
                    {
                        if (key_ptr->print_text[iloop] > '9' || key_ptr->print_text[iloop] < '0')   //非数字字符
                        {
                            tmp_text[str_info.wstr_len ++] = key_ptr->print_text[iloop];
                        }
                    }

                    if (str_info.wstr_len > 4)      //超过4个，两行显示
                    {
                        first_line_cnt = str_info.wstr_len / 2;       //每行显示字符个数
                        first_line_cnt = first_line_cnt < 4 ? first_line_cnt : 4;
                        second_line_cnt = str_info.wstr_len - first_line_cnt;
                        second_line_cnt = second_line_cnt < 4 ? second_line_cnt : 4;

                        //两行显示时，高度需要做一个调整，将空白部分5等分，顶部2，中间间隔1，底部2
                        ver_offset = (key_rect.bottom - key_rect.top - 2 * str_style.font) / 5;
                        ver_mod = (key_rect.bottom - key_rect.top - 2 * str_style.font - ver_offset * 5 - 2) / 2;

                        //第一行
                        rect_str.top = key_rect.top + 2 * ver_offset + ver_mod;
                        rect_str.bottom = rect_str.top + str_style.font;

                        str_info.wstr_len = first_line_cnt;
                        while (GUISTR_GetStringWidth (&str_style, &str_info, str_state) > key_rect.right - key_rect.left)
                        {
                            if (str_style.font <= data_ptr->theme.font_min) 
                            {
                                break;
                            }

                            str_style.font -= 1;
                        }
                        
                        GUISTR_DrawTextInRect(
                            &des_str_buf,
                            &rect_str,
                            &rect_str,
                            &str_info,
                            &str_style,
                            str_state,
                            GUISTR_TEXT_DIR_AUTO);

                        //第二行
                        rect_str.top = key_rect.top + 3 * ver_offset + str_style.font + ver_mod;
                        rect_str.bottom = rect_str.top + str_style.font;

                        str_info.wstr_ptr += first_line_cnt;
                        str_info.wstr_len = second_line_cnt;

                        while (GUISTR_GetStringWidth (&str_style, &str_info, str_state) > key_rect.right - key_rect.left)
                        {
                            if (str_style.font <= data_ptr->theme.font_min) 
                            {
                                break;
                            }

                            str_style.font -= 1;
                        }
                        
                        GUISTR_DrawTextInRect(
                            &des_str_buf,
                            &rect_str,
                            &rect_str,
                            &str_info,
                            &str_style,
                            str_state,
                            GUISTR_TEXT_DIR_AUTO);
                    }
                    //显示全部字符，排除数字
                    else
                    {
                        str_style.char_space = 1;

                        while (GUISTR_GetStringWidth (&str_style, &str_info, str_state) > key_rect.right - key_rect.left)
                        {
                            if (str_style.font <= data_ptr->theme.font_min) 
                            {
                                break;
                            }

                            str_style.font -= 1;
                        }
                        
                        GUISTR_DrawTextInRect(
                            &des_str_buf,
                            &key_rect,
                            &key_rect,
                            &str_info,
                            &str_style,
                            str_state,
                            GUISTR_TEXT_DIR_AUTO);
                    }

                    break;
                }

                //两行显示，最后如果为数字，不显示，每行最多显示4个字符
            case KASC_2LINE:
                {
                    MMI_STRING_T str_info = {0};
                    GUISTR_STYLE_T str_style = {0};
                    int16 text_width = 0, ver_offset = 0, ver_mod = 0;
                    int16 first_line_cnt = 0, second_line_cnt = 0;
                    GUI_RECT_T rect_str = key_rect;

                    str_style.font = key_ptr->font_size * TIP_EXPAND_MULTIPLE;
                    str_style.font_color = RGB888TO565(theme_ptr->text_color);
                    str_style.align = ALIGN_HVMIDDLE;
                    
                    text_width = key_rect.right - key_rect.left - 2;

                    str_info.wstr_ptr = key_ptr->print_text;
                    str_info.wstr_len = key_ptr->print_text_len;

                    if (str_info.wstr_len > 4)      //超过4个，两行显示
                    {
                        first_line_cnt = str_info.wstr_len / 2;       //每行显示字符个数
                        second_line_cnt = str_info.wstr_len - first_line_cnt;

                        //自动调整字体的大小
                        if (str_style.font > (disp_height >> 1)) str_style.font = (GUI_FONT_T)(disp_height >> 1);//如果是两行显示字体必须除2 modified by arvin

                        while (GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, first_line_cnt) > text_width)    //
                        {
                            if (str_style.font <= data_ptr->theme.font_min)
                                break;

                            str_style.font -= 1;
                        }

                        //两行字符稍微缩小点，不然看起来与一行显示大小一样，有些拥挤
                        if (str_style.font  != data_ptr->theme.font_min && str_style.font  > data_ptr->theme.font_min + 2)
                            str_style.font -= 2;

                        //两行显示时，高度需要做一个调整，将空白部分5等分，顶部2，中间间隔1，底部2
                        ver_offset = (key_rect.bottom - key_rect.top - 2 * str_style.font) / 5;
                        ver_mod = (key_rect.bottom - key_rect.top - 2 * str_style.font - ver_offset * 5 - 2) / 2;

                        //第一行
                        rect_str.top = key_rect.top + 2 * ver_offset + ver_mod;
                        rect_str.bottom = rect_str.top + str_style.font;

                        str_info.wstr_len = first_line_cnt;
                        GUISTR_DrawTextInRect(
                            &des_str_buf,
                            &rect_str,
                            &rect_str,
                            &str_info,
                            &str_style,
                            GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR,
                            GUISTR_TEXT_DIR_AUTO);

                        //第二行
                        rect_str.top = key_rect.top + 3 * ver_offset + str_style.font + ver_mod;
                        rect_str.bottom = rect_str.top + str_style.font;

                        str_info.wstr_ptr += first_line_cnt;
                        str_info.wstr_len = second_line_cnt;
                        GUISTR_DrawTextInRect(
                            &des_str_buf,
                            &rect_str,
                            &rect_str,
                            &str_info,
                            &str_style,
                            GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR,
                            GUISTR_TEXT_DIR_AUTO);
                    }
                    //显示全部字符，排除数字
                    else
                    {
                        str_style.char_space = 1;
                        text_width = key_rect.right - key_rect.left - 2 - str_style.char_space * str_info.wstr_len - key_padding;
                        
                        //自动调整字体的大小
                        if (str_style.font > disp_height)
                        {
                            str_style.font = (GUI_FONT_T)(disp_height);
                        }

                        while (GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, str_info.wstr_len) > text_width)
                        {
                            if (str_style.font <= data_ptr->theme.font_min)
                                break;

                            str_style.font -= 1;
                        }

                        GUISTR_DrawTextInRect(
                            &des_str_buf,
                            &key_rect,
                            &key_rect,
                            &str_info,
                            &str_style,
                            str_state,
                            GUISTR_TEXT_DIR_AUTO);
                    }

                    break;
                }
            case KASC_EXCLUDE_DIGIT:
            default:
                {
                    MMI_STRING_T str_info = {0};
                    GUISTR_STYLE_T str_style = {0};
                    wchar tmp_text[TEXT_LEN_LIMIT] = {0};   //key_text中的数字可能不是最后一位
                    int16 iloop = 0;
                    
                    SCI_MEMSET (tmp_text, 0, sizeof (tmp_text));
                    str_info.wstr_ptr = tmp_text;
                    str_info.wstr_len = 0;

                    //数字字符均不需要显示
                    for (iloop = 0; iloop < key_ptr->print_text_len; iloop ++)        //此处已经约定key_ptr->print_text_len < TEXT_LEN_LIMIT
                    {
                        if (key_ptr->print_text[iloop] > '9' || key_ptr->print_text[iloop] < '0')   //非数字字符
                        {
                            tmp_text[str_info.wstr_len ++] = key_ptr->print_text[iloop];
                        }
                    }

                    str_style.font = (GUI_FONT_T) key_ptr->font_size * TIP_EXPAND_MULTIPLE;
                    str_style.font_color = RGB888TO565(theme_ptr->text_color);
                    str_style.align = ALIGN_HVMIDDLE;
                    str_style.char_space = 1;

                    //往左下方偏移一些像素，防止字符显示不下
                    if (key_ptr->subtext_len > 0)   //存在subtext, 2/5 高度区域显示subtext
                    {
                        //key_rect.left -= key_padding;
				    	if ((key_ptr->add_style.text_disp & KASC_SUBTEXT_TEXT_ATTR) == KASC_SUBTEXT_MIDDLE)		//居中显示时，不需要交错距离
			    		{
							key_rect.top += (key_rect.bottom - key_rect.top) * (THUMB_MIDDLE_SUBTEXT_HEIGHT_SIZE_RATIO - THUMB_HEIGHT_SIZE_CROSS_RATIO) / 100;
						}
						else
						{
	                        key_rect.left += (key_rect.right - key_rect.left) * THUMB_SUBTEXT_WIDTH_SIZE_RATIO / 100;
							key_rect.top += (key_rect.bottom - key_rect.top) * (100 - THUMB_SUBTEXT_HEIGHT_SIZE_RATIO) / (100 * 2);
						}
                	}
					
                    while (GUISTR_GetStringWidth (&str_style, &str_info, str_state) > key_rect.right - key_rect.left)
                    {
                        if (str_style.font <= data_ptr->theme.font_min) 
                        {
                            break;
                        }

                        str_style.font -= 1;
                    }
                    
                    GUISTR_DrawTextInRect(
                        &des_str_buf,
                        &key_rect,
                        &key_rect,
                        &str_info,
                        &str_style,
                        str_state,
                        GUISTR_TEXT_DIR_AUTO);

                    break;
                }
            }
        }
        //非字符按键
        else
        {
            MMI_STRING_T str_info = {0};
            GUISTR_STYLE_T str_style = {0};
            int16 text_width = 0;

            str_info.wstr_ptr = key_ptr->print_text;
            str_info.wstr_len = key_ptr->print_text_len;

            str_style.font = theme_ptr->text_font + key_ptr->font_size;
            str_style.font_color = RGB888TO565(theme_ptr->text_color);
            str_style.align = ALIGN_HVMIDDLE;

            text_width = key_rect.right - key_rect.left - 2;    //边框2个像素
 
            //自动调整字体的大小
            if (str_style.font > disp_height) str_style.font = (GUI_FONT_T)disp_height;

            while (GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, str_info.wstr_len) > text_width)
            {
                if (str_style.font <= data_ptr->theme.font_min) break;

                str_style.font -= 1;
            }

            GUISTR_DrawTextInRect(
                &des_str_buf,
                &key_rect,
                &key_rect,
                &str_info,
                &str_style,
                GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR,
                GUISTR_TEXT_DIR_AUTO);
        }
    }

    return ;
}


/*==============================================================================
Description: 按键指定区域及绘制方案情况下，获取按键字符text字段最大字符尺寸 
                    大小,当前仅仅对character属性的按键做了该项功能，其他类型按键没有
                    添加字符按键也仅仅增加了部分现在中英文需要的属性，如果外文
                    需要类似功能，后期再做添加
Global resource dependence: DrawKeyRaw
Author: yaoguang.chen   2012-04-12
Note:DrawKeyRaw，与该处的绘制方式一致，否则获取的字符大小没有意义
==============================================================================*/
LOCAL GUI_FONT_T ComputKeyFontMaxSize (
    IM_DATA_T const *data_ptr,
    KEY_THEME_T const *theme_ptr,
    KEY_INFO_T *key_ptr
)
{
    GUI_RECT_T key_rect = {0};
    //GUI_RECT_T text_rect = {0};
    int16 disp_height = 0;
    GUI_FONT_T cur_font = SONG_FONT_40;
    
    CHECK_DATA_EX(data_ptr);
    if(PNULL == theme_ptr || PNULL == key_ptr)
    {
        return cur_font;
    }

    key_rect = key_ptr->rect;
    if (theme_ptr->border.relief != RELIEF_FLAT)   //存在边框
    {
        key_rect.left += theme_ptr->border.width;
        key_rect.top += theme_ptr->border.width;
        key_rect.right -= theme_ptr->border.width;
        key_rect.bottom -= theme_ptr->border.width;
    }

    //字符显示方式
    if (key_ptr->print_text_len > 0)       //存在字符
    {
        //字符按键，丝印从数据表读取的数据，需要根据情况来显示
        //小心部分按键被重定义为KEY_STYLE_CHARACTER类型，如函数MultiTapCharKeyInit
        if (key_ptr->style == KEY_STYLE_CHARACTER)
        {
            GUISTR_STATE_T str_state = GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR | GUISTR_STATE_BOLD;
            switch (key_ptr->add_style.text_disp & KASC_TEXT_ATTR)
            {
                //单行显示所有字符
                case KASC_ALL_TEXT:
                {
                    MMI_STRING_T str_info = {0};
                    GUISTR_STYLE_T str_style = {0};
                    int16 text_width = 0;

                    disp_height = key_rect.bottom - key_rect.top;
                    //往左下方偏移一些像素，防止字符显示不下
                    if (key_ptr->subtext_len > 0)   //存在subtext, 2/5 高度区域显示subtext
                    {
                        //key_rect.left -= key_padding;
                        //key_rect.left -= key_padding;
				    	if ((key_ptr->add_style.text_disp & KASC_SUBTEXT_TEXT_ATTR) == KASC_SUBTEXT_MIDDLE)		//居中显示时，不需要交错距离
			    		{
							;
						}
						else                        
					    {
                        	key_rect.top += (key_rect.bottom - key_rect.top) * THUMB_SUBTEXT_WIDTH_SIZE_RATIO / 100;
						}
						
                        disp_height = key_rect.bottom - key_rect.top;
                    }
                    
                    str_info.wstr_ptr = key_ptr->print_text;
                    str_info.wstr_len = key_ptr->print_text_len;

                    str_style.char_space = 1;
                    str_style.font = theme_ptr->text_font;
                    
                    //自动调整字体的大小
                    if (str_style.font > disp_height) 
                    {
                        str_style.font = (GUI_FONT_T)disp_height;
                    }

                    text_width = key_rect.right - key_rect.left;
                    while (GUISTR_GetStringWidth (&str_style, &str_info, str_state) > text_width)
                    {
                        if (str_style.font <= data_ptr->theme.font_min) 
                        {
                            break;
                        }

                        str_style.font -= 1;
                    }

                    cur_font = str_style.font;
                    break;
                }

                //显示第一个字符，假设不存在subtext
                case KASC_FIRST:
                {
                    MMI_STRING_T str_info = {0};
                    GUISTR_STYLE_T str_style = {0};
                    int16 text_width = 0;
                    uint8 key_padding = 2;    //字符显示与边缘的间距
                    
                    str_info.wstr_ptr = key_ptr->print_text;
                    str_info.wstr_len = 1;

                    str_style.font = theme_ptr->text_font;
                    str_style.align = ALIGN_HVMIDDLE;

                    //自动调整字体的大小
                    disp_height = key_rect.bottom - key_rect.top - 2 * key_padding;
                    if (str_style.font > disp_height)
                    {
                        str_style.font = (GUI_FONT_T)disp_height;
                    }

                    text_width = key_rect.right - key_rect.left;
                    while (GUISTR_GetStringWidth (&str_style, &str_info, str_state) > text_width)
                    {
                        if (str_style.font <= data_ptr->theme.font_min)
                            break;

                        str_style.font -= 1;
                    }

                    cur_font = str_style.font;

                    break;
                }
                
                 //两行显示，最后如果为数字，不显示，每行最多显示4个字符
                case KASC_2X4_EXCLUDE_DIGIT:
                {
                    MMI_STRING_T str_info = {0};
                    GUISTR_STYLE_T str_style = {0};
                    int16 text_width = 0;
                    int16 first_line_cnt = 0, second_line_cnt = 0;
                    GUI_FONT_T min_font = SONG_FONT_40;
                    wchar tmp_text[TEXT_LEN_LIMIT] = {0};   //key_text中的数字可能不是最后一位
                    int16 iloop = 0;

                    str_state = GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR;     //不加粗
                    
                    SCI_MEMSET (tmp_text, 0, sizeof (tmp_text));
                    str_info.wstr_ptr = tmp_text;
                    str_info.wstr_len = 0;

                    //数字字符均不需要显示
                    for (iloop = 0; iloop < key_ptr->print_text_len; iloop ++)        //此处已经约定key_ptr->print_text_len < TEXT_LEN_LIMIT
                    {
                        if (key_ptr->print_text[iloop] > '9' || key_ptr->print_text[iloop] < '0')   //非数字字符
                        {
                            tmp_text[str_info.wstr_len ++] = key_ptr->print_text[iloop];
                        }
                    }
                    
                    str_style.font = theme_ptr->text_font;
                    str_style.font_color = RGB888TO565(theme_ptr->text_color);
                    str_style.align = ALIGN_HVMIDDLE;

                    //去除subtext所占用的宽度
                    if (key_ptr->subtext_len > 0)   //存在subtext
                    {
                    	if ((key_ptr->add_style.text_disp & KASC_SUBTEXT_TEXT_ATTR) == KASC_SUBTEXT_MIDDLE)		//居中显示时，不需要交错距离
                		{
							;
						}
						else
                        {
                        	key_rect.left += (key_rect.right - key_rect.left) * THUMB_SUBTEXT_WIDTH_SIZE_RATIO / 100;
						}
                    }
                
                    if (str_info.wstr_len > 4)      //超过4个，两行显示
                    {
                        first_line_cnt = str_info.wstr_len / 2;       //每行显示字符个数
                        first_line_cnt = first_line_cnt < 4 ? first_line_cnt : 4;
                        second_line_cnt = str_info.wstr_len - first_line_cnt;
                        second_line_cnt = second_line_cnt < 4 ? second_line_cnt : 4;

                        disp_height = key_rect.bottom - key_rect.top;
                        
                        if (str_style.font > (disp_height >> 1)) str_style.font = (GUI_FONT_T)(disp_height >> 1);//如果是两行显示字体必须除2 modified by arvin

                        //自动调整字体的大小
                        str_info.wstr_len = first_line_cnt;
                        text_width = key_rect.right - key_rect.left;
                        while (GUISTR_GetStringWidth (&str_style, &str_info, str_state) > text_width)    //
                        {
                            if (str_style.font <= data_ptr->theme.font_min)
                                break;

                            str_style.font -= 1;
                        }
                        min_font = str_style.font;      //第一行

                        //第二行
                        str_info.wstr_ptr += (str_info.wstr_len - second_line_cnt);
                        str_info.wstr_len = second_line_cnt;
                        text_width = key_rect.right - key_rect.left;
                        while (GUISTR_GetStringWidth (&str_style, &str_info, str_state) > text_width)    //
                        {
                            if (str_style.font <= data_ptr->theme.font_min)
                                break;

                            str_style.font -= 1;
                        }

                        str_style.font = min_font < str_style.font ? min_font : str_style.font;
                    }
                    //显示全部字符，排除数字
                    else
                    {
                        disp_height = key_rect.bottom - key_rect.top;
                        str_style.char_space = 1;
                        
                        //自动调整字体的大小
                        if (str_style.font > disp_height) 
                        {
                            str_style.font = (GUI_FONT_T)(disp_height);
                        }

                        text_width = key_rect.right - key_rect.left;
                        while (GUISTR_GetStringWidth (&str_style, &str_info, str_state) > text_width)
                        {
                            if (str_style.font <= data_ptr->theme.font_min)
                                break;

                            str_style.font -= 1;
                        }

                        str_style.font = str_style.font < disp_height ? str_style.font : disp_height;/*lint !e64*/ 
                    }

                    cur_font = str_style.font;
                    break;
                }

                case KASC_EXCLUDE_DIGIT:    
                default:
                {
                    MMI_STRING_T str_info = {0};
                    GUISTR_STYLE_T str_style = {0};
                    int16 text_width = 0;
                    wchar tmp_text[TEXT_LEN_LIMIT] = {0};   //key_text中的数字可能不是最后一位
                    int16 iloop = 0;
                    
                    SCI_MEMSET (tmp_text, 0, sizeof (tmp_text));
                    str_info.wstr_ptr = tmp_text;
                    str_info.wstr_len = 0;

                    //数字字符均不需要显示
                    for (iloop = 0; iloop < key_ptr->print_text_len; iloop ++)        //此处已经约定key_ptr->print_text_len < TEXT_LEN_LIMIT
                    {
                        if (key_ptr->print_text[iloop] > '9' || key_ptr->print_text[iloop] < '0')   //非数字字符
                        {
                            tmp_text[str_info.wstr_len ++] = key_ptr->print_text[iloop];
                        }
                    }

                    //往左下方偏移一些像素，防止字符显示不下
                    if (key_ptr->subtext_len > 0)   //存在subtext, 2/5 高度区域显示subtext
                    {
                    	if ((key_ptr->add_style.text_disp & KASC_SUBTEXT_TEXT_ATTR) == KASC_SUBTEXT_MIDDLE)		//居中显示时，不需要交错距离
                		{
							key_rect.top += (key_rect.bottom - key_rect.top) * (THUMB_MIDDLE_SUBTEXT_HEIGHT_SIZE_RATIO - THUMB_HEIGHT_SIZE_CROSS_RATIO) / 100;
						}
						else
                        {
	                        key_rect.left += (key_rect.right - key_rect.left) * THUMB_SUBTEXT_WIDTH_SIZE_RATIO / 100;
	                        disp_height = key_rect.bottom - key_rect.top;
                		}
                    }
                    
                    str_style.char_space = 1;
                    str_style.font = theme_ptr->text_font;
                    
                    //自动调整字体的大小
                    disp_height = key_rect.bottom - key_rect.top;
                    if (str_style.font > disp_height) 
                    {
                        str_style.font = (GUI_FONT_T)disp_height;
                    }

                    text_width = key_rect.right - key_rect.left;
                    while (GUISTR_GetStringWidth (&str_style, &str_info, str_state) > text_width)
                    {
                        if (str_style.font <= data_ptr->theme.font_min) 
                        {
                            break;
                        }

                        str_style.font -= 1;
                    }
                    str_style.font = str_style.font < disp_height ? str_style.font : disp_height; /*lint !e64*/      //横屏时，还是会出现极端情况，字符比显示高度大
                
                    cur_font = str_style.font;
                    break;
                }
            }
        }
    }

    return cur_font;
}


/*==============================================================================
Description: 绘制按键 ，此处假设按键中的字符有选择状态，且默认为按键字符
                串长度大于1
Global resource dependence: g_select_letter_index
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void DrawKeySelectLetter(
    IM_DATA_T const *data_ptr,
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    KEY_THEME_T const *theme_ptr,
    GUI_RECT_T const *rect_ptr,     //tip_rect
    KEY_INFO_T *key_ptr
)
{
    GUISTR_BUF_T des_str_buf = {0};
    GUI_RECT_T key_rect = {0};
    int16 disp_height = 0;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != dc_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != rect_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == dc_data_ptr || PNULL == rect_ptr||PNULL == key_ptr)
    {
        return;
    }
    
    DC_CHECK(dc_data_ptr, rect_ptr);

    key_rect = *rect_ptr;
    key_rect.left += theme_ptr->border.width;
    key_rect.top += theme_ptr->border.width;
    key_rect.right -= theme_ptr->border.width;
    key_rect.bottom -= theme_ptr->border.width;

    if (SHARP_ROUND_RECTANGLE == theme_ptr->sharp)
    {
        //fill
        switch (theme_ptr->fill)
        {
        case FILL_PURE:
            _FillRoundRect(dc_data_ptr, &key_rect, theme_ptr->fill_data.pure, TRUE);
            break;

        case FILL_GRADIENT:
            _FillGradientRoundRect(dc_data_ptr, &key_rect,
                                   theme_ptr->fill_data.gradient.c0,
                                   theme_ptr->fill_data.gradient.c1,
                                   theme_ptr->fill_data.gradient.a, TRUE);
            break;

        case FILL_GRADIENT_BLOCK:
            _FillGradientBlockRoundRect(dc_data_ptr, &key_rect,
                                        theme_ptr->fill_data.gradient_block.d0.c0,
                                        theme_ptr->fill_data.gradient_block.d0.c1,
                                        theme_ptr->fill_data.gradient_block.d0.a,
                                        theme_ptr->fill_data.gradient_block.d1.c0,
                                        theme_ptr->fill_data.gradient_block.d1.c1,
                                        theme_ptr->fill_data.gradient_block.d1.a,
                                        theme_ptr->fill_data.gradient_block.d2.c0,
                                        theme_ptr->fill_data.gradient_block.d2.c1,
                                        theme_ptr->fill_data.gradient_block.d2.a,
                                        TRUE);
            break;

        case FILL_GRADIENT_PURE_BLOCK:
            _FillGradientPureBlockRoundRect (dc_data_ptr, &key_rect,
                                        theme_ptr->fill_data.gradient.c0,
                                        theme_ptr->fill_data.gradient.c1,
                                        theme_ptr->fill_data.gradient.a,
                                        TRUE);
            break;
            
        default:
            break;
        }

        //border
        switch (theme_ptr->border.relief)
        {
        case RELIEF_RAISED:
        case RELIEF_SUNKEN:
        case RELIEF_SOLID:
            _DrawRoundBorderUneven(dc_data_ptr, &key_rect,
                                   theme_ptr->border.width, theme_ptr->border.relief,
                                   theme_ptr->border.color,
                                   theme_ptr->border.color_bright,
                                   theme_ptr->border.color_dark);
            break;

        case RELIEF_RIDGE:
        case RELIEF_GROOVE:
            _DrawRoundBorderSharp(dc_data_ptr, &key_rect,
                                  theme_ptr->border.width, theme_ptr->border.relief,
                                  theme_ptr->border.color,
                                  theme_ptr->border.color_bright,
                                  theme_ptr->border.color_dark);
            break;

        case RELIEF_INSET_BRIGHT:
        case RELIEF_INSET_DARK:
            _DrawRoundBorderInset(dc_data_ptr, &key_rect,
                                  theme_ptr->border.width, theme_ptr->border.relief,
                                  theme_ptr->border.color_bright,
                                  theme_ptr->border.color_dark);
            break;

        default:
            break;
        }
    }
    else
    {
        //fill
        switch (theme_ptr->fill)
        {
        case FILL_PURE:
            _FillRect(dc_data_ptr, &key_rect, theme_ptr->fill_data.pure);
            break;

        case FILL_GRADIENT:
            _FillGradientRect(dc_data_ptr, &key_rect,
                              theme_ptr->fill_data.gradient.c0,
                              theme_ptr->fill_data.gradient.c1,
                              theme_ptr->fill_data.gradient.a);
            break;

        case FILL_GRADIENT_BLOCK:
            break;

        default:
            break;
        }

        //border
        switch (theme_ptr->border.relief)
        {
        case RELIEF_RAISED:
        case RELIEF_SUNKEN:
        case RELIEF_SOLID:
            _DrawBorderUneven(dc_data_ptr, &key_rect,
                              theme_ptr->border.width, theme_ptr->border.relief,
                              theme_ptr->border.color,
                              theme_ptr->border.color_bright,
                              theme_ptr->border.color_dark);
            break;

        case RELIEF_RIDGE:
        case RELIEF_GROOVE:
            _DrawBorderSharp(dc_data_ptr, &key_rect,
                             theme_ptr->border.width, theme_ptr->border.relief,
                             theme_ptr->border.color,
                             theme_ptr->border.color_bright,
                             theme_ptr->border.color_dark);
            break;

        default:
            break;
        }
    }

    //icon & text
    //icon first
    if (key_ptr->icon)
    {
        BOOLEAN has_get_img = FALSE;
        uint16 img_width = 0, img_height = 0;
        int16 offset_x = 0, offset_y = 0;
        GUI_RECT_T disp_rect = {0};
        GUI_RECT_T img_rect = {0};

        has_get_img = GUIRES_GetImgWidthHeight(&img_width,
                                               &img_height,
                                               key_ptr->icon,
                                               data_ptr->init_param.win_id);

        if (has_get_img)
        {
            offset_x = (key_rect.right - key_rect.left - 2 - img_width) / 2;
            offset_y = (key_rect.bottom - key_rect.top - 2 - img_height) / 2;

            disp_rect = key_rect;

            if (offset_x > 0)
            {
                disp_rect.left += offset_x;
                img_rect.left = 0;
            }
            else
            {
                img_rect.left = (-1) * offset_x;
            }

            if (offset_y > 0)
            {
                disp_rect.top += offset_y;
                img_rect.top = 0;
            }
            else
            {
                img_rect.top = (-1) * offset_y;
            }

            img_rect.right = img_width - 1;
            img_rect.bottom = img_height - 1;

            GUIRES_DisplayImgDst(PNULL,
                                 &disp_rect,
                                 &img_rect,
                                 data_ptr->init_param.win_id,
                                 key_ptr->icon,
                                 dc_data_ptr);
        }
    }

    //then text
    DcToDstStr(dc_data_ptr, &des_str_buf);

    disp_height = key_rect.bottom - key_rect.top - 2;

    if (key_ptr->subtext_len > 0)
    {
        MMI_STRING_T str_info = {0};
        GUISTR_STYLE_T str_style = {0};

        str_info.wstr_ptr = key_ptr->subtext;
        str_info.wstr_len = key_ptr->subtext_len;

        str_style.font = theme_ptr->subtext_font + key_ptr->font_size;
        str_style.font_color = RGB888TO565(theme_ptr->subtext_color);
        str_style.align = ALIGN_LVMIDDLE;

        if (str_style.font > disp_height) str_style.font = (GUI_FONT_T)disp_height;

        //在有subtext的时候，需要留空
        key_rect.left += data_ptr->theme.key_padding;

        GUISTR_DrawTextInRect(
            &des_str_buf,
            &key_rect,
            &key_rect,
            &str_info,
            &str_style,
            GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN_BY_DIR,
            GUISTR_TEXT_DIR_AUTO);

        key_rect.left += GUI_GetStringWidth(str_style.font, str_info.wstr_ptr, str_info.wstr_len);
    }

    //字符显示
    if (key_ptr->text_len > 1)      //存在多个字符
    {
        int32 iloop = 0;

        //字符按键，丝印从数据表读取的数据，需要根据情况来显示
        MMI_STRING_T str_info = {0};
        GUISTR_STYLE_T str_style = {0};
        int16 text_width = 0;

        str_info.wstr_ptr = key_ptr->text;
        str_info.wstr_len = key_ptr->text_len;

        str_style.font = theme_ptr->text_font + key_ptr->font_size;
        str_style.font_color = RGB888TO565(theme_ptr->text_color);
        str_style.align = ALIGN_HVMIDDLE;

        //key_rect = *rect_ptr;
        text_width = ((rect_ptr->right - rect_ptr->left - 2 * TIP_SELECT_LETTER_SPACE_X + TIP_SELECT_LETTER_INTERAL_X)
                      / key_ptr->text_len) - TIP_SELECT_LETTER_INTERAL_X; //key_ptr->rect.right - key_ptr->rect.left;

        key_rect.top = rect_ptr->top + ((rect_ptr->bottom - rect_ptr->top) - (key_ptr->rect.bottom - key_ptr->rect.top)) / 2;
        key_rect.bottom = rect_ptr->bottom - ((rect_ptr->bottom - rect_ptr->top) - (key_ptr->rect.bottom - key_ptr->rect.top)) / 2;

        disp_height = key_rect.bottom - key_rect.top - 2;

        //自动调整字体的大小
        if (str_style.font > disp_height)
        {
            str_style.font = (GUI_FONT_T)disp_height;
        }

        if (str_style.font > text_width * 5 / 6)
        {
            str_style.font = (GUI_FONT_T)text_width * 5 / 6;
        }

        for (iloop = 0; iloop < key_ptr->text_len; iloop ++)
        {
            key_rect.left = TIP_SELECT_LETTER_SPACE_X + iloop * (text_width + TIP_SELECT_LETTER_INTERAL_X);
            key_rect.right = key_rect.left + text_width;

            str_info.wstr_ptr = key_ptr->text + iloop;
            str_info.wstr_len = 1;

            //是否高亮
            if (GetLetterSelectIndex() == iloop)    /*lint !e746*/
            {
                str_style.font_color = RGB888TO565(TIP_LETTER_HIGHLIGHT_TEXT_COLOR);

                //绘制背景
                _FillGradientBlockRoundRect(
                    dc_data_ptr,
                    &key_rect,
                    TIP_LETTER_HIGHLIGHT_BG_TOP_COLOR,
                    TIP_LETTER_HIGHLIGHT_BG_MID_COLOR,
                    theme_ptr->fill_data.gradient.a,
                    TIP_LETTER_HIGHLIGHT_BG_MID_COLOR,
                    TIP_LETTER_HIGHLIGHT_BG_BOTTOM_COLOR,
                    theme_ptr->fill_data.gradient.a,
                    TIP_LETTER_HIGHLIGHT_BG_BOTTOM_COLOR,
                    TIP_LETTER_HIGHLIGHT_BG_BOTTOM_COLOR,
                    theme_ptr->fill_data.gradient.a,
                    TRUE
                );
            }
            else
            {
                str_style.font_color = RGB888TO565(TIP_LETTER_NORMAL_TEXT_COLOR);

                //绘制背景
                _FillRoundRect(dc_data_ptr, &key_rect, TIP_LETTER_NORMAL_BG_COLOR, TRUE);;
            }

            //绘制
            GUISTR_DrawTextInRect(
                &des_str_buf,
                &key_rect,
                &key_rect,
                &str_info,
                &str_style,
                GUISTR_STATE_ALIGN | GUISTR_STATE_ALIGN_BY_DIR,
                GUISTR_TEXT_DIR_AUTO);
        }
    }

    return ;
}



/*==============================================================================
Description: ShowTip
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ShowTip(IM_DATA_T *data_ptr,  KEY_INFO_T *key_ptr)
{
    uint8 const tip_distance = 18;
    BOOLEAN has_tip_layer = FALSE;
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUILCD_DRAW_DC_T dc_data = {0};
    BOOLEAN is_dev_ok = FALSE;
    uint16 key_width = 0;
    uint16 tip_width = 0;
    GUI_RECT_T tip_rect = {0};
    uint16 mix_alpha = 0;
    int16 tip_x = 0, tip_y = 0;
    int16 panel_width = 0;

    CHECK_DATA_EX(data_ptr);

    if (0 != data_ptr->tip_hide_timer)
    {
        MMK_StopTimer(data_ptr->tip_hide_timer);
        data_ptr->tip_hide_timer = 0;
    }

    if (!key_ptr->has_tip)
    {
        UILAYER_RemoveBltLayer(&data_ptr->tip_layer);
        data_ptr->is_tip_display = FALSE;

        return;
    }

    has_tip_layer = CreateTipLayer(data_ptr);

    //SCI_ASSERT(has_tip_layer); /*assert verified*/
    if(!has_tip_layer)
    {
        return;
    }
    
    UILAYER_Clear(&data_ptr->tip_layer);

    append_layer.lcd_dev_info = data_ptr->tip_layer;
    append_layer.layer_level = UILAYER_LEVEL_DEBUG;
    UILAYER_AppendBltLayer(&append_layer);

    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->tip_layer, &dc_data);

    if (!is_dev_ok)
    {
        return;
    }

    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;

    tip_rect = data_ptr->tip_rect;
    key_width = key_ptr->rect.right - key_ptr->rect.left + 1;
    tip_width = key_width * TIP_EXPAND_MULTIPLE; /*lint !e524*/ 
    if (tip_width > panel_width)
    {
        tip_width = panel_width;
    }
    tip_rect.right = tip_width - 1;

    if (data_ptr->vkb.state & KEYBOARD_CHOOSE_LETTER)
    {
        //9键时，宽度太大，不美观，暂时不做修改
        tip_width = key_width * (key_ptr->text_len) + TIP_SELECT_LETTER_SPACE_X * 2 + TIP_SELECT_LETTER_INTERAL_X * (key_ptr->text_len - 1);
        if (tip_width >= panel_width)
        {
            tip_width = panel_width;
        }
        tip_rect.right = tip_width - 1;
        DrawKeySelectLetter(data_ptr, &dc_data, &(data_ptr->theme.tip_theme), &tip_rect, key_ptr);
    }
    else
    {
        DrawTipKey (data_ptr, &dc_data, &(data_ptr->theme.tip_theme), &tip_rect, key_ptr);
    }

    //draw over...
    mix_alpha = data_ptr->theme.alpha;
    mix_alpha *= data_ptr->theme.tip_alpha;
    mix_alpha /= 0xff;

    if (mix_alpha < 0xff)
    {
        UILAYER_WeakLayerAlpha(&data_ptr->tip_layer, mix_alpha);
    }

    MMITHEME_StoreUpdateRect(&data_ptr->tip_layer, tip_rect);

    tip_x = data_ptr->rect.left + key_ptr->rect.left - (tip_width - key_width) / 2;

    if (tip_x < 0)
    {
        tip_x = 0;
    }
    else if (tip_x + tip_width - 1 > data_ptr->rect.right)
    {
        tip_x = data_ptr->rect.right - tip_width + 1;
    }

    tip_y = data_ptr->rect.top + key_ptr->rect.top;
    tip_y -= tip_rect.bottom;
    tip_y -= tip_distance; //间隔5个像素，增加距离感

    data_ptr->rect_disp_tips.left = tip_x;    //将区域保存，下次move时，需要使用
    data_ptr->rect_disp_tips.right = tip_x + tip_width;    //将区域保存，下次move时，需要使用

    UILAYER_SetLayerPosition(&data_ptr->tip_layer, tip_x, tip_y);

    data_ptr->is_tip_display = TRUE;

    return;
}


/*==============================================================================
Description: HideTip
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void HideTip(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (!data_ptr->is_tip_display) return;

    if (0 != data_ptr->tip_hide_timer)
    {
        MMK_StopTimer(data_ptr->tip_hide_timer);
        data_ptr->tip_hide_timer = 0;   //还是要恢复该值，防止下次创建timer失败
    }

    data_ptr->tip_hide_timer = MMK_CreateTimerCallback(
                                   data_ptr->theme.tip_delay,
                                   HideTipCB,
                                   (uint32)data_ptr,
                                   FALSE);

    return;
}


/*==============================================================================
Description: HideTipCB
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void HideTipCB(uint8 timer_id, uint32 param)
{
    IM_DATA_T *data_ptr = (IM_DATA_T *)param;

    if (PNULL == data_ptr
            || (sizeof(IM_DATA_T) != data_ptr->check_info)
            || data_ptr->tip_hide_timer != timer_id)
    {
        return;
    }

    data_ptr->tip_hide_timer = 0;

    UILAYER_RemoveBltLayer(&data_ptr->tip_layer);
    data_ptr->is_tip_display = FALSE;

    return;
}


/*==============================================================================
Description: CreateUILayer
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN CreateUILayer(IM_DATA_T *data_ptr)
{
    UILAYER_CREATE_T create_info = {0};
    UILAYER_RESULT_E create_result = UILAYER_RESULT_SUCCESS;
    GUI_RECT_T prev_ui_layer_rect = {0, 0, 0, 0};

    CHECK_DATA_EX(data_ptr);

    if (UILAYER_NULL_HANDLE != data_ptr->ui_layer.block_id)
    {
        prev_ui_layer_rect = UILAYER_GetLayerRect (&data_ptr->ui_layer);
        if (prev_ui_layer_rect.top == data_ptr->rect.top
            && prev_ui_layer_rect.left == data_ptr->rect.left
            && prev_ui_layer_rect.right == data_ptr->rect.right
            && prev_ui_layer_rect.bottom == data_ptr->rect.bottom)      //检查层大小是否一致 
        {
            return TRUE;
        }
        else
        {
            ReleaseUILayer(data_ptr);
        }
    }

    create_info.lcd_id = data_ptr->init_param.lcd_info_ptr->lcd_id;
    create_info.owner_handle = data_ptr->init_param.im_handle;
    create_info.offset_x = data_ptr->rect.left;
    create_info.offset_y = data_ptr->rect.top;
    create_info.width = data_ptr->rect.right - data_ptr->rect.left + 1;
    create_info.height = data_ptr->rect.bottom - data_ptr->rect.top + 1;
#ifdef MMIIM_LAYER_565    
    create_info.is_bg_layer = TRUE;
#else
    create_info.is_bg_layer = FALSE;
#endif
    create_info.is_static_layer = TRUE;
    create_info.format = UILAYER_MEM_IMMUTABLE;

    create_result = UILAYER_CreateLayer(&create_info, &data_ptr->ui_layer);

    if (UILAYER_RESULT_SUCCESS != create_result)
    {
        return FALSE;
    }

#ifdef MMIIM_LAYER_565 
    //SCI_ASSERT(DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(&data_ptr->ui_layer)); /*assert verified*/
    if(DATA_TYPE_RGB565 != UILAYER_GetLayerColorType(&data_ptr->ui_layer))
    {
        return  FALSE;
    }
#else
    //SCI_ASSERT(DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(&data_ptr->ui_layer)); /*assert verified*/
    if(DATA_TYPE_ARGB888 != UILAYER_GetLayerColorType(&data_ptr->ui_layer))
    {
        return  FALSE;
    }
#endif

    UILAYER_SetLayerColorKey(&data_ptr->ui_layer, TRUE, UILAYER_TRANSPARENT_COLOR);


    return TRUE;
}


/*==============================================================================
Description: ReleaseUILayer
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ReleaseUILayer(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    UILAYER_RELEASELAYER(&data_ptr->ui_layer);   /*lint !e506 !e774*/
//    UILAYER_ReleaseLayer(&data_ptr->ui_layer);
//    data_ptr->ui_layer.block_id = UILAYER_NULL_HANDLE;

    return;
}


/*==============================================================================
Description: CreateTipLayer
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN CreateTipLayer(IM_DATA_T *data_ptr)
{
    UILAYER_CREATE_T create_info = {0};
    UILAYER_RESULT_E create_result = UILAYER_RESULT_SUCCESS;

    CHECK_DATA_EX(data_ptr);

    if (UILAYER_NULL_HANDLE != data_ptr->tip_layer.block_id)
    {
        return TRUE;
    }

    create_info.lcd_id = data_ptr->init_param.lcd_info_ptr->lcd_id;
    create_info.owner_handle = data_ptr->init_param.im_handle;
    create_info.offset_x = data_ptr->tip_rect.left;
    create_info.offset_y = data_ptr->tip_rect.top;
    create_info.width = data_ptr->tip_rect.right - data_ptr->tip_rect.left + 1;
    create_info.height = data_ptr->tip_rect.bottom - data_ptr->tip_rect.top + 1;
#ifdef MMIIM_LAYER_565    
    create_info.is_bg_layer = TRUE;
#else
    create_info.is_bg_layer = FALSE;
#endif
    create_info.is_static_layer = TRUE;
    create_info.format = UILAYER_MEM_IMMUTABLE;

    create_result = UILAYER_CreateLayer(&create_info, &data_ptr->tip_layer);

    if (UILAYER_RESULT_SUCCESS != create_result)
    {
        return FALSE;
    }
#ifdef MMIIM_LAYER_565 
    //SCI_ASSERT(DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(&data_ptr->tip_layer)); /*assert verified*/
    if(DATA_TYPE_RGB565 != UILAYER_GetLayerColorType(&data_ptr->tip_layer))
    {
        return  FALSE;
    }
#else
    //SCI_ASSERT(DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(&data_ptr->tip_layer)); /*assert verified*/
    if(DATA_TYPE_ARGB888 != UILAYER_GetLayerColorType(&data_ptr->tip_layer))
    {
        return  FALSE;
    }
#endif
    
    UILAYER_SetLayerColorKey(&data_ptr->tip_layer, TRUE, UILAYER_TRANSPARENT_COLOR);

    return TRUE;
}


/*==============================================================================
Description: ReleaseTipLayer
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ReleaseTipLayer(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    if (0 != data_ptr->tip_hide_timer)
    {
        MMK_StopTimer(data_ptr->tip_hide_timer);
        data_ptr->tip_hide_timer = 0;
    }

    UILAYER_RELEASELAYER(&data_ptr->tip_layer);   /*lint !e506 !e774*/
//    UILAYER_ReleaseLayer(&data_ptr->tip_layer);
//    data_ptr->tip_layer.block_id = UILAYER_NULL_HANDLE;

    return;
}


/*==============================================================================
Description: CreateEditLayer
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN CreateEditLayer(IM_DATA_T *data_ptr)
{
    UILAYER_CREATE_T create_info = {0};
    UILAYER_RESULT_E create_result = UILAYER_RESULT_SUCCESS;

    CHECK_DATA_EX(data_ptr);

    if (UILAYER_NULL_HANDLE != data_ptr->edit_layer.block_id)
    {
        return TRUE;
    }

    //edit rect, 高度为字体的高度
    data_ptr->edit_rect = data_ptr->rect;
    data_ptr->edit_rect.bottom = data_ptr->rect.top - 1;
    data_ptr->edit_rect.top = data_ptr->edit_rect.top - data_ptr->theme.edit_text_font * 2;

    create_info.lcd_id = data_ptr->init_param.lcd_info_ptr->lcd_id;
    create_info.owner_handle = data_ptr->init_param.im_handle;
    create_info.offset_x = data_ptr->edit_rect.left;
    create_info.offset_y = data_ptr->edit_rect.top;
    create_info.width = data_ptr->edit_rect.right - data_ptr->edit_rect.left + 1;
    create_info.height = data_ptr->edit_rect.bottom - data_ptr->edit_rect.top + 1;
#ifdef MMIIM_LAYER_565    
    create_info.is_bg_layer = TRUE;
#else
    create_info.is_bg_layer = FALSE;
#endif
    create_info.is_static_layer = TRUE;
    create_info.format = UILAYER_MEM_DOUBLE_COPY;

    create_result = UILAYER_CreateLayer(&create_info, &data_ptr->edit_layer);

    if (UILAYER_RESULT_SUCCESS != create_result)
    {
        return FALSE;
    }
#ifdef MMIIM_LAYER_565 
    //SCI_ASSERT(DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(&data_ptr->edit_layer)); /*assert verified*/
    if(DATA_TYPE_RGB565 != UILAYER_GetLayerColorType(&data_ptr->edit_layer))
    {
        return  FALSE;
    }
#else
    //SCI_ASSERT(DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(&data_ptr->edit_layer)); /*assert verified*/
    if(DATA_TYPE_ARGB888 != UILAYER_GetLayerColorType(&data_ptr->edit_layer))
    {
        return  FALSE;
    }
#endif
    
    UILAYER_SetLayerColorKey(&data_ptr->edit_layer, TRUE, UILAYER_TRANSPARENT_COLOR);

    return TRUE;
}


/*==============================================================================
Description: ReleaseEditLayer
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ReleaseEditLayer(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    UILAYER_RELEASELAYER(&data_ptr->edit_layer);   /*lint !e506 !e774*/
//    UILAYER_ReleaseLayer(&data_ptr->edit_layer);
//    data_ptr->edit_layer.block_id = UILAYER_NULL_HANDLE;

    return;
}


/*==============================================================================
Description: InitVkbCaps
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void InitVkbCaps(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    switch (data_ptr->init_param.method_param_ptr->im_def_ptr->capital)
    {
    case GUIIM_MODE_LEAD_UPPER:
        data_ptr->vkb.cap = CAPITTAL_MODE_INITIAL;
        break;

    case GUIIM_MODE_UPPER:
        data_ptr->vkb.cap = CAPITTAL_MODE_UPPER;
        break;

        //case GUIIM_MODE_LEAD_UPPER:
    default:
        data_ptr->vkb.cap = CAPITTAL_MODE_LOWER;
        break;
    }

    return;
}


/*==============================================================================
Description: 设置
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void SetVkbNextCaps(IM_DATA_T *data_ptr)
{
    CAPITTAL_MODE_T next_cap;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.caps_key); /*assert verified*/
    if(PNULL == data_ptr->vkb.caps_key)
    {
        return ;
    }
    
    switch (data_ptr->vkb.cap)
    {
    case CAPITTAL_MODE_INITIAL:
        next_cap = CAPITTAL_MODE_UPPER;
        break;

    case CAPITTAL_MODE_LOWER:
        next_cap = CAPITTAL_MODE_INITIAL;
        break;

        //case CAPITTAL_MODE_UPPER:
    default:
        next_cap = CAPITTAL_MODE_LOWER;
        break;
    }

    SetVkbCaps(data_ptr, next_cap);

#if defined MMIIM_TOUCH_SMART_CAPS
    SetCapsSwitchType (CAPS_SWITCH_TYPE_USER_E);
#endif

    return;
}


/*==============================================================================
Description: SetVkbCaps
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void SetVkbCaps(IM_DATA_T *data_ptr, CAPITTAL_MODE_T cap)
{
    char(*tr_f)(char) = PNULL;
    GUIIM_CAPITAL_MODE_T im_cap = GUIIM_MODE_LEAD_UPPER;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.caps_key); /*assert verified*/

    //某些语言，不存在大小写输入
    if (data_ptr->init_param.method_param_ptr
            && data_ptr->init_param.method_param_ptr->im_def_ptr
            && !MMIAPIIM_CapsExist(data_ptr->init_param.method_param_ptr->im_def_ptr->lang))
    {
        return;
    }

    if (data_ptr->vkb.cap == cap)
    {
        return;
    }

    if (data_ptr->vkb.caps_key == PNULL)
    {
        return;
    }
        
    switch (cap)
    {
    case CAPITTAL_MODE_INITIAL:
        im_cap = GUIIM_MODE_LEAD_UPPER;
        data_ptr->vkb.caps_key->icon = IMG_IMT_CAP_LU;

        if (CAPITTAL_MODE_LOWER == data_ptr->vkb.cap) tr_f = UpperCase;

        break;

    case CAPITTAL_MODE_LOWER:
        im_cap = GUIIM_MODE_LOWER;
        data_ptr->vkb.caps_key->icon = IMG_IMT_CAP_L;

        if (CAPITTAL_MODE_LOWER != data_ptr->vkb.cap) tr_f = LowerCase;

        break;

    case CAPITTAL_MODE_UPPER:
        im_cap = GUIIM_MODE_UPPER;
        data_ptr->vkb.caps_key->icon = IMG_IMT_CAP_U;

        if (CAPITTAL_MODE_LOWER == data_ptr->vkb.cap) tr_f = UpperCase;

        break;

    default:
        break;
    }

    //set kb
    if (PNULL != tr_f)
    {
        int32 key_idx = 0;
        GUIIM_METHOD_T new_method = 0;

        new_method = MMITHEME_IMGetMethodByCapital(data_ptr->init_param.method_param_ptr->im_def_ptr->method, im_cap);
        if (data_ptr->init_param.method_param_ptr->im_def_ptr->input == GUIIM_INPUT_TYPE_TOUCHPEN)
        {
            MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL;
            wchar *cur_char_ptr = PNULL;

            key_table_ptr = MMIIM_SP_ML9KEY_GetKeyTable(new_method);
            if (key_table_ptr == PNULL)
            { 
                key_table_ptr = MMIIM_SP_Smart9Key_GetKeyTable(new_method);
            }

            for (key_idx = 0; key_idx < data_ptr->vkb.key_cnt; key_idx++)
            {
                if (KEY_STYLE_CHARACTER == data_ptr->vkb.keys[key_idx].style
                        && data_ptr->vkb.keys[key_idx].code <= VKEY_0
                        && data_ptr->vkb.keys[key_idx].code >= VKEY_1)      //数字键
                {
                    cur_char_ptr = (wchar *)key_table_ptr + ((data_ptr->vkb.keys[key_idx].code - VKEY_1) * (MMIIM_SP_9KEY_CHAR_MAX + 1));       // + 1第一个字节表示字符个数

                    data_ptr->vkb.keys[key_idx].text_len = MIN (cur_char_ptr[0], TEXT_LEN_LIMIT - 1);      //字符个数
                    SCI_MEMCPY(data_ptr->vkb.keys[key_idx].text,
                               &cur_char_ptr[1],
                               data_ptr->vkb.keys[key_idx].text_len*sizeof(wchar));
                }
            }
        }
        else if (data_ptr->init_param.method_param_ptr->im_def_ptr->input == GUIIM_INPUT_TYPE_TP_QWERT)
        {
            MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr = PNULL;
            MMIIM_SP_26KEY_KEYCHAR_T *cur_char_ptr = PNULL;

            key_table_ptr = MMIIM_SP_ML26KEY_GetKeyTable(new_method);
            if (key_table_ptr == PNULL)
            {
                key_table_ptr = MMIIM_SP_SMART26KEY_GetKeyTable(new_method);
            }

            for (key_idx = 0; key_idx < data_ptr->vkb.key_cnt; key_idx++)
            {
                if (KEY_STYLE_CHARACTER == data_ptr->vkb.keys[key_idx].style
                        && data_ptr->vkb.keys[key_idx].code <= VKEY_Z
                        && data_ptr->vkb.keys[key_idx].code >= VKEY_A)
                {
                    cur_char_ptr = key_table_ptr + (g_kb_keymap[key_idx] - VKEY_A);
                    data_ptr->vkb.keys[key_idx].text_len = cur_char_ptr->key_chars[0];
                    SCI_MEMCPY(data_ptr->vkb.keys[key_idx].text,
                               &cur_char_ptr->key_chars[1],
                               data_ptr->vkb.keys[key_idx].text_len*sizeof(wchar));
                }
            }
        }

		//大小写切换后，将text字段数据同步到display_text字段，以备显示使用
        if (data_ptr->init_param.method_param_ptr->im_def_ptr->input == GUIIM_INPUT_TYPE_TOUCHPEN)
        {
            MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL;
            wchar *cur_char_ptr = PNULL;

            key_table_ptr = MMIIM_SP_ML9KEY_GetPrintKeyTable(new_method);
            if (key_table_ptr == PNULL)
            { 
                key_table_ptr = MMIIM_SP_Smart9KEY_GetPrintKeyTable (new_method);
            }

            for (key_idx = 0; key_idx < data_ptr->vkb.key_cnt; key_idx++)
            {
                if (KEY_STYLE_CHARACTER == data_ptr->vkb.keys[key_idx].style
                        && data_ptr->vkb.keys[key_idx].code <= VKEY_0
                        && data_ptr->vkb.keys[key_idx].code >= VKEY_1)      //数字键
                {
                    cur_char_ptr = (wchar *)key_table_ptr + ((data_ptr->vkb.keys[key_idx].code - VKEY_1) * (MMIIM_SP_9KEY_CHAR_MAX + 1));       // + 1第一个字节表示字符个数

                    data_ptr->vkb.keys[key_idx].print_text_len = MIN (cur_char_ptr[0], PRINT_TEXT_LEN_LIMIT - 1);      //字符个数
                    SCI_MEMCPY(data_ptr->vkb.keys[key_idx].print_text,
                               &cur_char_ptr[1],
                               data_ptr->vkb.keys[key_idx].print_text_len*sizeof(wchar));
                }
            }
        }
        else if (data_ptr->init_param.method_param_ptr->im_def_ptr->input == GUIIM_INPUT_TYPE_TP_QWERT)
        {
            MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr = PNULL;
            MMIIM_SP_26KEY_KEYCHAR_T *cur_char_ptr = PNULL;

            key_table_ptr = MMIIM_SP_ML26KEY_GetPrintKeyTable(new_method);
            if (key_table_ptr == PNULL)
            {
                key_table_ptr = MMIIM_SP_SMART26KEY_GetPrintKeyTable(new_method);
            }

            for (key_idx = 0; key_idx < data_ptr->vkb.key_cnt; key_idx++)
            {
                if (KEY_STYLE_CHARACTER == data_ptr->vkb.keys[key_idx].style
                        && data_ptr->vkb.keys[key_idx].code <= VKEY_Z
                        && data_ptr->vkb.keys[key_idx].code >= VKEY_A)
                {
                    cur_char_ptr = key_table_ptr + (g_kb_keymap[key_idx] - VKEY_A);
                    data_ptr->vkb.keys[key_idx].print_text_len = cur_char_ptr->key_chars[0];
                    SCI_MEMCPY(data_ptr->vkb.keys[key_idx].print_text,
                               &cur_char_ptr->key_chars[1],
                               data_ptr->vkb.keys[key_idx].print_text_len*sizeof(wchar));
                }
            }
        }

		//大小写切换后，字符大小需要再次调整
        MMIIM_TOUCH_KbUnifiedCharKeysFontSize (data_ptr); 
    }

    data_ptr->vkb.cap = cap;
    g_last_care_caps = cap;

    notify_pack.notify = GUIIM_NOTIFY_SWITCH_CAPITAL;
    notify_pack.data.set_capital = im_cap;

    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    return;
}


/*==============================================================================
Description: SetHideKey
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void SetHideKey(IM_DATA_T * data_ptr, uint8 cand_cnt)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.hide_key); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->vkb.expand_key); /*assert verified*/
    if(PNULL == data_ptr->vkb.hide_key || PNULL == data_ptr->vkb.expand_key)
    {
        return ;
    }
    
    if (cand_cnt > 0)
    {
        data_ptr->vkb.hide_key->is_hide = 1;
        data_ptr->vkb.expand_key->is_hide = 0;
    }
    else
    {
        data_ptr->vkb.hide_key->is_hide = 0;
        data_ptr->vkb.expand_key->is_hide = 1;
    }

	MMIIM_SetVKeyPaintStatus (data_ptr->vkb.hide_key, TRUE);
	MMIIM_SetVKeyPaintStatus (data_ptr->vkb.expand_key, TRUE);

    return;
}


/*==============================================================================
Description: SwitchExpandKey
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void SwitchExpandKey(IM_DATA_T *data_ptr)
{
    int32 key_idx = 0;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.expand_key); /*assert verified*/
    if(PNULL == data_ptr->vkb.expand_key)
    {
        return ;
    }

	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
	
    if (0 == data_ptr->vkb.is_expand)
    {
        data_ptr->vkb.is_expand = 1;
        data_ptr->vkb.expand_key->icon = IMG_IMT_CONTRACT;
        
        //候选字状态设置
        if (data_ptr->vkb.state & KEYBOARD_CANDIDATE)
        {
            data_ptr->vkb.state |= KEYBOARD_CANDIDATE_EXPAND;
            data_ptr->vkb.state &= (~KEYBOARD_CANDIDATE);
        }
        else if (data_ptr->vkb.state & KEYBOARD_ASSOCIATE)
        {
            data_ptr->vkb.state |= KEYBOARD_ASSOCIATE_EXPAND;
            data_ptr->vkb.state &= (~KEYBOARD_ASSOCIATE);
        }

        //hide all other keys, expect key expand
        for (key_idx = 0; key_idx < data_ptr->vkb.key_cnt; key_idx++)
        {
            if ((data_ptr->vkb.keys[key_idx].tag & KEY_TAG_EXPAND) > 0)
            {
                data_ptr->vkb.keys[key_idx].is_hide = 0;
            }
            else
            {
                data_ptr->vkb.keys[key_idx].is_hide = 1;
            }
        }
    }
    else
    {
        data_ptr->vkb.is_expand = 0;
        data_ptr->vkb.expand_key->icon = IMG_IMT_EXPAND;
        
        //候选字状态设置
        if (data_ptr->vkb.state & KEYBOARD_CANDIDATE_EXPAND)
        {
            data_ptr->vkb.state |= KEYBOARD_CANDIDATE;
            data_ptr->vkb.state &= (~KEYBOARD_CANDIDATE_EXPAND);
        }
        else if (data_ptr->vkb.state & KEYBOARD_ASSOCIATE_EXPAND)
        {
            data_ptr->vkb.state |= KEYBOARD_ASSOCIATE;
            data_ptr->vkb.state &= (~KEYBOARD_ASSOCIATE_EXPAND);
        }

        //hide all other keys, expect key expand
        for (key_idx = 0; key_idx < data_ptr->vkb.key_cnt; key_idx++)
        {
            if ((data_ptr->vkb.keys[key_idx].tag & KEY_TAG_NORMAL) > 0)
            {
                data_ptr->vkb.keys[key_idx].is_hide = 0;
            }
            else
            {
                data_ptr->vkb.keys[key_idx].is_hide = 1;
            }
        }
    }

    return;
}


/*==============================================================================
Description: ReleaseVkb
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ReleaseVkb(VKEYBOARD_T *vkb_ptr)
{
    //SCI_ASSERT(PNULL != vkb_ptr); /*assert verified*/
    if(PNULL == vkb_ptr)
    {
        return ;
    }
    
    SCI_FREEIF(vkb_ptr->keys);
    SCI_MEMSET(vkb_ptr, 0, sizeof(VKEYBOARD_T));

    return;
}


/*==============================================================================
Description: SetPanel
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 放在client rect ex中，使用client rect 的大小，保证大小的稳定性
==============================================================================*/
PUBLIC void SetPanel(IM_DATA_T *data_ptr)
{
    int16 panel_percent = MMIIM_PANEL_PERCENT_NORMAL;
    int16 panel_width = 0;
    int16 panel_height = 0;
    GUI_RECT_T full_rect = MMITHEME_GetFullScreenRectEx(data_ptr->init_param.win_handle);
    GUI_RECT_T client_rect_ex = MMITHEME_GetClientRectEx(data_ptr->init_param.win_handle);

    CHECK_DATA(data_ptr);

    data_ptr->rect = client_rect_ex;

    switch (data_ptr->kb_size)
    {
    case MMIIM_KEYBOARD_SIZE_BIG:
        panel_percent = MMIIM_PANEL_PERCENT_BIG;
        break;

    case MMIIM_KEYBOARD_SIZE_NORMAL:
        panel_percent = MMIIM_PANEL_PERCENT_NORMAL;
        break;
    case MMIIM_KEYBOARD_SIZE_SMALL:
        panel_percent = MMIIM_PANEL_PERCENT_SMALL;
        break;

        //case MMIIM_KEYBOARD_SIZE_NORMAL:
    default:
        break;
    }

    panel_width = full_rect.right - full_rect.left + 1;
    panel_height = full_rect.bottom - full_rect.top + 1;

    panel_height *= panel_percent;
    panel_height /= 100;

    data_ptr->rect.top = data_ptr->rect.bottom - panel_height;

    //tip rect高度不能固定，否则字符宽度缩放后，高度却没有缩放导致显示不下
    data_ptr->tip_rect.left = 0;
    data_ptr->tip_rect.top = 0;
    data_ptr->tip_rect.right = panel_width - 1;
    //data_ptr->tip_rect.bottom = panel_height / 3;
    data_ptr->tip_rect.bottom = panel_height * TIP_EXPAND_MULTIPLE / GetPanelKeyRowNum (data_ptr);/*lint !e524*/
    return;
}



/*==============================================================================
Description: GetPanelKeyRowNum
Global resource dependence: NONE
Author: yaoguang.chen 2012-05-26
Note: tips高度应该与按键高度成比例，而不应该与面板高度成比例
==============================================================================*/
PUBLIC uint16 GetPanelKeyRowNum (IM_DATA_T *data_ptr)
{
    uint16 num = 5;
    
    if (data_ptr == PNULL)
    {
        SCI_TRACE_LOW ("[MMIIM] GetPanelKeyLineNum");
        return num;  
    }

#if defined MMIIM_HW_FULLSCREEN_SUPPORT
    if (data_ptr->init_param.method_param_ptr
        && data_ptr->init_param.method_param_ptr->im_def_ptr
        && data_ptr->init_param.method_param_ptr->im_def_ptr->method == GUIIM_M_HANDWRITING)
    {
        num = 3;
    }
#endif

    if (data_ptr->is_symbol_panel)
    {
        num = 5;
    }

    return num;
}

/*==============================================================================
Description: 通常的 - 键 - 按下
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 在down的最后调用
==============================================================================*/
PUBLIC BOOLEAN VkbDown(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    if (PNULL == key_ptr)
    {
        return TRUE;
    }

    CHECK_DATA_EX(data_ptr);

    if(data_ptr->vkb.last_key != key_ptr)
    {
        if ((PNULL != data_ptr->vkb.last_key) && data_ptr->vkb.last_key->is_press)
        {
            data_ptr->vkb.last_key->is_press = 0;
        }
        
        //multitap输入法定时器需要立即关闭并commit last
        data_ptr->vkb.last_key = key_ptr;
    }
    
    key_ptr->is_press = 1;

    MMIIM_SetVKeyPaintStatus (key_ptr, TRUE);
	DrawVkb (data_ptr);
	
    ShowTip(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: 通常的 - 键 - 触笔移动
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 在move的最后调用
==============================================================================*/
PUBLIC BOOLEAN VkbMove(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    if (PNULL == key_ptr)
    {
        return TRUE;
    }

    CHECK_DATA_EX(data_ptr);

    if (key_ptr == data_ptr->vkb.last_key)
    {
        return TRUE;
    }

#if defined SOGOU_USE_PDA_SKIN     //sogou存在背景皮肤，按键背景无法单独绘制

	MMIIM_BmpBgKeyCausedRepaint (data_ptr, key_ptr);

    if (data_ptr->vkb.last_key)
    {
        data_ptr->vkb.last_key->is_press = 0;
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.last_key, TRUE);
    }
	
    data_ptr->vkb.last_key = key_ptr;
    key_ptr->is_press = 1;
	MMIIM_SetVKeyPaintStatus (key_ptr, TRUE);
	
    DrawVkb (data_ptr);
    ShowTip(data_ptr, key_ptr);
#else
    if (data_ptr->vkb.last_key)
    {
        data_ptr->vkb.last_key->is_press = 0;
	    MMIIM_SetVKeyPaintStatus (data_ptr->vkb.last_key, TRUE);
    }
    
    data_ptr->vkb.last_key = key_ptr;
    key_ptr->is_press = 1;
    MMIIM_SetVKeyPaintStatus (key_ptr, TRUE);
	
	DrawVkb (data_ptr);
    ShowTip(data_ptr, key_ptr);
#endif

    return TRUE;
}


/*==============================================================================
Description: 通常的 - 键 - 长按
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 在long的最后调用
==============================================================================*/
PUBLIC BOOLEAN VkbLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    if (PNULL == key_ptr)
    {
        return TRUE;
    }

    CHECK_DATA_EX(data_ptr);

    if (key_ptr->is_press)
    {
        key_ptr->is_press = 0;
		MMIIM_SetVKeyPaintStatus (key_ptr, TRUE);
    }

    if (data_ptr->vkb.last_key && data_ptr->vkb.last_key->is_press)
    {
        data_ptr->vkb.last_key->is_press = 0;
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.last_key, TRUE);
    }
    
    data_ptr->vkb.last_key = PNULL;

    HideTip(data_ptr);
    DrawVkb(data_ptr);

    return TRUE;
}


/*==============================================================================
Description: 通常的 - 键 - 抬起
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 在up的最后调用
==============================================================================*/
PUBLIC BOOLEAN VkbUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    if (PNULL == key_ptr)
    {
        return TRUE;
    }

    CHECK_DATA_EX(data_ptr);

    if (key_ptr->is_press)
    {
        key_ptr->is_press = 0;
		MMIIM_SetVKeyPaintStatus (key_ptr, TRUE);
    }

    //这里理论上是可以不处理的，
    //但是某些烂屏会出现down, 没有move，但是up点和down点不一致的情况，整死哥了
    if (data_ptr->vkb.last_key && data_ptr->vkb.last_key->is_press)
    {
        //SCI_TRACE_LOW:"VkbUp cao touchpanel"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_COMMON_4439_112_2_18_2_27_14_88,(uint8*)"");
        data_ptr->vkb.last_key->is_press = 0;
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.last_key, TRUE);
    }

    HideTip(data_ptr);
    DrawVkb(data_ptr);      

    return TRUE;
}


/*==============================================================================
Description: 通常的提笔响应 - 提交按键显示字符串
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN VkeyUpDeal(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    if (PNULL == key_ptr)
    {
        return TRUE;
    }

    CHECK_DATA_EX(data_ptr);

    if (key_ptr->text_len > 0)
    {
        CapsCommitStr (data_ptr, key_ptr->text, key_ptr->text_len);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: 隐藏键盘，并发送消息至控件
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void HidePanel(IM_DATA_T *data_ptr)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_out_effect_enabled)
    {
        int16 height = data_ptr->rect.bottom - data_ptr->rect.top;
        int16 top = 0;
        int16 remain = 0;

    //先把背景图黑
//NEWMS00190811 [6530][128x64_320x480PDA2]弹出的输入框界面中，退出输入键盘，界面背景全黑
#if 0    
#if defined MMIIM_HW_FULLSCREEN_SUPPORT
        if (!data_ptr->is_symbol_panel
            && data_ptr->init_param.method_param_ptr->im_def_ptr->method == GUIIM_M_HANDWRITING   //全屏手写状态
            ) //从手写切换到sym后，上述的输入法值依然为hw
        {
            GUI_RECT_T display_panel_rect = {0};

            HwFullScreenGetDisplayPanelRect (data_ptr, &display_panel_rect);
            GUI_FillRect(data_ptr->init_param.lcd_info_ptr, display_panel_rect, RGB888TO565(data_ptr->theme.panel_color));
        }
        else
        {
            GUI_FillRect(data_ptr->init_param.lcd_info_ptr, data_ptr->rect, RGB888TO565(data_ptr->theme.panel_color));
        }
#else
        GUI_FillRect(data_ptr->init_param.lcd_info_ptr, data_ptr->rect, RGB888TO565(data_ptr->theme.panel_color));
#endif
#endif

        //键盘逐渐隐藏
        do
        {
            remain++;
            top = data_ptr->rect.top + remain * height / VKB_EFFECT_FRAME_CNT;
            UILAYER_SetLayerPosition(&data_ptr->ui_layer, data_ptr->rect.left, top);
            MMITHEME_UpdateRect();
        }
        while (remain < VKB_EFFECT_FRAME_CNT);
    }

    notify_pack.notify = GUIIM_NOTIFY_HIDE;

    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    return;
}


/*==============================================================================
Description: 通常的提笔响应 - hide键被点击
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN VkeyUpHide(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    if (PNULL == key_ptr)
    {
        return TRUE;
    }

    VkbUp(data_ptr, key_ptr);       //调换顺序，防止隐藏后重复绘制键盘
    HidePanel(data_ptr);

    return TRUE;
}


/*==============================================================================
Description: 通常的长按设置，setting
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN VkeyLongSet(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    if (PNULL == key_ptr)
    {
        return TRUE;
    }

    CHECK_DATA_EX(data_ptr);
#if defined(MMI_ISTYLE_SUPPORT)	
    if (MMITHEME_IsIstyle())
    {
        MMIIM_CreateSettingIMLayer(data_ptr,key_ptr);

        MMIIM_DrawSettingIm(data_ptr,key_ptr,FALSE,0);
    }
    else
#endif		
    {
        CreateSettingWin(data_ptr);
    }

    if (MMK_IsFocusWin(data_ptr->init_param.win_handle))
    {
        VkbLong(data_ptr, key_ptr);
    }   

    return TRUE;
}


/*==============================================================================
Description: 通常的长按设置，setting
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN VkeyLongDel(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    if (PNULL == key_ptr)
    {
        return TRUE;
    }

    CHECK_DATA_EX(data_ptr);

    notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;
    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


LOCAL BOOLEAN VkeySwitchMethod (IM_DATA_T *data_ptr)
{
    BOOLEAN is_new_method_set = FALSE;

    CHECK_DATA_EX(data_ptr);
    
    //首次down完版本后，显示为英文，没有设置相应的非英文本地语言
    if(MMIAPIIM_GetPrevLanguage() == GUIIM_LANG_NONE)
    {
        GUIIM_LANGUAGE_T guiim_new_lang = GUIIM_LANG_NONE;
        MMISET_LANGUAGE_TYPE_E language_type = MMISET_LANGUAGE_ENGLISH;
		
        MMIAPISET_GetLanguageType(&language_type);
        guiim_new_lang = MMIAPIIM_GetGUILangBySetLangIdx(language_type);
        MMIAPIIM_SetPrevLanguage(guiim_new_lang);

        //可能当前显示语言也为英文
	 if(MMIAPIIM_GetPrevLanguage() == GUIIM_LANG_NONE)
 	 {
            //switch to next ime
            if(g_default_en_type == GUIIM_TYPE_MULTITAP)
            {
                  g_default_en_type = GUIIM_TYPE_SMART;
	     }
            else if(g_default_en_type == GUIIM_TYPE_SMART)
            {
		    g_default_en_type = GUIIM_TYPE_MULTITAP;
	     }
	 }
    }

    //当前为手写
    if (GUIIM_M_HANDWRITING == data_ptr->init_param.method_param_ptr->im_def_ptr->method)   
    {
        //手写时，切换输入语言，返回时，s_default_input_type为手写状态
        if(g_default_input_type == GUIIM_INPUT_TYPE_HANDWRITING)
            g_default_input_type = g_entry_hw_input_type;
        
        if(MMIAPIIM_GetLanguage() == GUIIM_LANG_ENGLISH)       //手写前英文
            is_new_method_set = SettingMethod(data_ptr, GUIIM_LANG_ENGLISH, g_default_en_type, g_default_input_type);
        else
            is_new_method_set = SettingMethod(data_ptr, MMIAPIIM_GetPrevLanguage(), g_default_ch_type, g_default_input_type);
    }
    //本地语言输入法
    else if (MMIAPIIM_GetPrevLanguage() == data_ptr->init_param.method_param_ptr->im_def_ptr->lang
        && data_ptr->init_param.method_param_ptr->im_def_ptr->lang != GUIIM_LANG_NONE)     
    {
        g_default_en_type = g_default_ch_type;
        //如果 中文 
#if defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT    
        if(g_default_ch_type == GUIIM_TYPE_STROKE)
        {
            g_default_en_type = GUIIM_TYPE_SMART;
        }
#endif
        
        is_new_method_set = SettingMethod(data_ptr, GUIIM_LANG_ENGLISH, g_default_en_type, g_default_input_type);
    }
    //当前为英文
    else if (GUIIM_LANG_ENGLISH == data_ptr->init_param.method_param_ptr->im_def_ptr->lang
             && !(GUIIM_LIMIT_LOCAL & data_ptr->init_param.method_param_ptr->limit))
    {
        if (MMIAPIIM_GetPrevLanguage() == GUIIM_LANG_NONE)      //有且仅有英文输入时
        {
        	is_new_method_set = SettingMethod(data_ptr, GUIIM_LANG_ENGLISH, g_default_en_type, g_default_input_type);
        }
        else if (GUIIM_TYPE_NONE == g_default_ch_type)
        {
            is_new_method_set = SettingMethod(data_ptr, MMIAPIIM_GetPrevLanguage(), g_default_ch_type, GUIIM_INPUT_TYPE_HANDWRITING);
        }
        else
        {
            //可能英文有智能输入法，而其他对应的语言没有智能输入法，导致异常，智能之间切换有问题
            if((! HasConfigSelectInputType(data_ptr, MMIAPIIM_GetPrevLanguage(), GUIIM_TYPE_SMART))
                && g_default_en_type == GUIIM_TYPE_SMART)
            {
                g_default_ch_type = GUIIM_TYPE_MULTITAP;
            }
            //可能英文为abc，而其他语言为中文，即没有multitap输入法
            else if((! HasConfigSelectInputType(data_ptr, MMIAPIIM_GetPrevLanguage(), GUIIM_TYPE_MULTITAP))
                && g_default_en_type == GUIIM_TYPE_MULTITAP)
            {
                g_default_ch_type = GUIIM_TYPE_SMART;      //切换到与英文输入方式相同的输入法
            }
            //相同方式切换
            else
            {
                g_default_ch_type = g_default_en_type;
            }

            is_new_method_set = SettingMethod(data_ptr, MMIAPIIM_GetPrevLanguage(), g_default_ch_type, GUIIM_INPUT_TYPE_NONE);
        }
    }

    return is_new_method_set;
}


PUBLIC void SetMethodSelectMenuDefaultIndex (int32 index)
{
    s_default_menu_index = index;	//begin from 0
}

PUBLIC int32 GetMethodSelectMenuDefaultIndex ( )
{
    return s_default_menu_index;
}


/*==============================================================================
Description: 输入法设置按键down
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN VkeySetDown (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    GUI_POINT_T point = {0, 0};
    GUI_RECT_T kb_rect = {0, 0, 0, 0};
    
    if (PNULL == key_ptr)
    {
        //SCI_TraceLow:"VkeySetDown"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_COMMON_4729_112_2_18_2_27_15_89,(uint8*)"");
        return TRUE;
    }

    CHECK_DATA_EX(data_ptr);

    //绘制按键
    if(data_ptr->vkb.last_key != key_ptr)
    {
        if (data_ptr->vkb.last_key)
        {
            data_ptr->vkb.last_key->is_press = 0;
        }
        
        data_ptr->vkb.last_key = key_ptr;
    }

    //进入选择菜单状态
    data_ptr->vkb.state |= KEYBOARD_CHOOSE_MENU;
    data_ptr->vkb.is_key_block = 1; //锁定键盘

    if (data_ptr->ui_layer.block_id != UILAYER_NULL_HANDLE)     //使用剪贴板时，会失去焦点
    {
        kb_rect = UILAYER_GetLayerRect(&data_ptr->ui_layer);
    }
    else
    {
        kb_rect = data_ptr->rect;
    }
    
    point.x = key_ptr->rect.left + kb_rect.left; 
    point.y = key_ptr->rect.top + kb_rect.top;
    MMIIM_OpenMethodSettingMenu (data_ptr, point);

    return TRUE;
}


/*==============================================================================
Description: 通常的输入法切换设置
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN VkeyUpSet(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    BOOLEAN is_new_method_set = FALSE;
    
    if (PNULL == key_ptr)
    {
        return TRUE;
    }

    CHECK_DATA_EX(data_ptr);

    is_new_method_set = VkeySwitchMethod (data_ptr);    
    if (!is_new_method_set)
    {
        if (key_ptr->is_press)
        {
            key_ptr->is_press = 0;
			MMIIM_SetVKeyPaintStatus (key_ptr, TRUE);
        }

        DrawVkb(data_ptr);
    }

    return TRUE;
}


/*==============================================================================
Description: 语言切换键
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN VkeyUpLangSet (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    GUIIM_METHOD_T new_method = GUIIM_M_NONE;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_ENGLISH;
    
    if (PNULL == key_ptr)
    {
        return TRUE;
    }

    CHECK_DATA_EX(data_ptr);

    if (data_ptr->init_param.method_param_ptr
        && data_ptr->init_param.method_param_ptr->im_def_ptr
        && data_ptr->init_param.method_param_ptr->im_def_ptr->lang == GUIIM_LANG_ENGLISH)
    {
        lang = MMIAPIIM_GetPrevLanguage();
    }
    //当前为手写英文，输入法控件认为手写与语言无关，但是当前的应用需要区分语言
    else if (data_ptr->init_param.method_param_ptr
        && data_ptr->init_param.method_param_ptr->im_def_ptr
        && data_ptr->init_param.method_param_ptr->im_def_ptr->method == GUIIM_M_HANDWRITING        
        && MMIAPIIM_GetLanguage() == GUIIM_LANG_ENGLISH)        
    {
        lang = MMIAPIIM_GetPrevLanguage();
    }

    MMIAPIIM_SetLanguage(lang);     //语言切换
    
    new_method = GetMethodBySwitchLang(data_ptr, lang);

    if (new_method != GUIIM_M_NONE)
    {
        notify_pack.notify = GUIIM_NOTIFY_SET_METHOD;
        notify_pack.data.set_method = new_method;
        GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
    }
    
    return TRUE;
}


/*==============================================================================
Description: 键盘滑动切换输入法提示关闭启动
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN PanelSlideCueCloseByTimer (IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);
     
    if (data_ptr->cue_hide_timer_id != 0)
    {
        MMK_StopTimer (data_ptr->cue_hide_timer_id);
        data_ptr->cue_hide_timer_id = 0;
    }

    data_ptr->cue_hide_timer_id = MMK_CreateTimerCallback(
                                   data_ptr->theme.cue_delay,
                                   PanelSlideCueCloseCB,
                                   (uint32)data_ptr,
                                   FALSE);
    
    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   PanelSlideCueCloseCB

Func:   定时器关闭输入法滑动提示信息

Thoughtway:
            如果一直显示提示信息，有时候挺烦的
            
Create: yaoguangchen@spread. 2011-10-11

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void PanelSlideCueCloseCB (uint8 timer_id, uint32 param)
{
    IM_DATA_T *data_ptr = (IM_DATA_T *)param;

    if (PNULL == data_ptr
            || (sizeof(IM_DATA_T) != data_ptr->check_info)
            || data_ptr->cue_hide_timer_id != timer_id)
    {
        return;
    }

    //如果当前对应的窗口非active状态，不需要刷新键盘
    //虽然在losefocus将定时器关闭了，但是某些应用会在非焦点窗口打开新输入法
    if (!MMK_IsFocusWin(data_ptr->init_param.win_handle))
    {
        PanelSlideCueClose (data_ptr, FALSE);
    }
    else
    {
        PanelSlideCueClose (data_ptr, TRUE);
    }
    
    return; 
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   PanelSlideCueClose

Func:   关闭提示信息

Thoughtway:
        是否立即刷新，防止有时候被重复刷新虚拟键盘
        
Create: yaoguangchen@spread. 2011-10-11

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN PanelSlideCueClose (IM_DATA_T *data_ptr, BOOLEAN immediately_paint)
{
    CHECK_DATA_EX (data_ptr);

    if (data_ptr->cue_hide_timer_id == 0)
    {
        //SCI_TraceLow:"PanelSlideCueClose"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_COMMON_4918_112_2_18_2_27_15_90,(uint8*)"");
        return FALSE;
    }
    
    if (data_ptr->vkb.vessel_key)
    {
        if (data_ptr->cue_hide_timer_id != 0)
        {
            MMK_StopTimer (data_ptr->cue_hide_timer_id);
            data_ptr->cue_hide_timer_id = 0;
        }

        if (data_ptr->vkb.vessel_key->print_text_len== 0)
        {
            //SCI_TraceLow:"PanelSlideCueClose"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_COMMON_4932_112_2_18_2_27_15_91,(uint8*)"");
            return TRUE;
        }

        SCI_MEMSET (data_ptr->vkb.vessel_key->print_text, 0, sizeof(data_ptr->vkb.vessel_key->print_text)); 
        data_ptr->vkb.vessel_key->print_text_len = 0;

        if (immediately_paint)
        {
    		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);
            DrawVkb(data_ptr);
        }
    }
    
    return TRUE;
}


/*==============================================================================
Description: 提交一个字符
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
PUBLIC void CommitChar(IM_DATA_T *data_ptr, wchar c)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    CHECK_DATA_EX(data_ptr);

    notify_pack.notify = GUIIM_NOTIFY_COMMIT;
    notify_pack.data.commit_data.commit[0] = c;
    notify_pack.data.commit_data.commit_len = 1;

    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    return;
}


/*==============================================================================
Description: CommitStr
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void CommitStr(IM_DATA_T *data_ptr, wchar *str, uint32 len)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != str); /*assert verified*/
    if(PNULL == str)
    {
    	SCI_TRACE_LOW ("MMIIM:CommitStr ERR str == PNULL");
        return ;
    }

	if (len > GUIIM_COMMIT_BUFFER_MAX)
	{
		len = GUIIM_COMMIT_BUFFER_MAX;
		SCI_TRACE_LOW ("MMIIM:CommitStr ERR LEN == %d", len);
	}
	
    notify_pack.notify = GUIIM_NOTIFY_COMMIT;
    SCI_MEMCPY(notify_pack.data.commit_data.commit, str, len*sizeof(wchar));
    notify_pack.data.commit_data.commit_len = len;

    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);

    return;
}


/*==============================================================================
Description: 提交一个字符 - Caps
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 凡是有大小写的提交字符，都会要求在leader upper的时候切换至lower
返回是否修改了caps
==============================================================================*/
PUBLIC BOOLEAN CapsCommitChar(IM_DATA_T *data_ptr, wchar c)
{
    BOOLEAN is_cap_set = FALSE;

    CHECK_DATA_EX(data_ptr);

    CommitChar(data_ptr, c);

    //不存在大小写
    if (data_ptr->init_param.method_param_ptr == PNULL
        || data_ptr->init_param.method_param_ptr->im_def_ptr == PNULL
        || (!MMIAPIIM_CapsExist (data_ptr->init_param.method_param_ptr->im_def_ptr->lang)))
    {
        SCI_TRACE_LOW ("[MMIIM]CapsCommitStr has no caps");
        return FALSE;
    }

    //URL不需要自动大小写切换
    if (data_ptr->init_param.method_param_ptr->tag == GUIIM_TAG_EMAIL
        || data_ptr->init_param.method_param_ptr->tag == GUIIM_TAG_URL)
    {
        SCI_TRACE_LOW ("[MMIIM]CapsCommitStr needn't caps");
        return FALSE;
    }
    
    if (CAPITTAL_MODE_INITIAL == data_ptr->vkb.cap)
    {
#if defined MMIIM_TOUCH_SMART_CAPS        
        if (DialogSeperateChar (c) )     //如果为一句话结束，不需要切换
        {
            return FALSE;
        }
#endif
        SetVkbCaps(data_ptr, CAPITTAL_MODE_LOWER);
        is_cap_set = TRUE;
#if defined MMIIM_TOUCH_SMART_CAPS
        SetCapsSwitchType (CAPS_SWITCH_TYPE_COMMIT_E);
#endif
    }
#if defined MMIIM_TOUCH_SMART_CAPS
    else if (CAPITTAL_MODE_LOWER == data_ptr->vkb.cap)  //有可能需要从小写切换到大写，如一句话结束等
    {
        if (GetCapsSwitchType () != CAPS_SWITCH_TYPE_USER_E && DialogSeperateChar (c) )     /*lint !e746*/
        {
            SetVkbCaps(data_ptr, CAPITTAL_MODE_INITIAL);
            is_cap_set = TRUE;
            SetCapsSwitchType (CAPS_SWITCH_TYPE_COMMIT_E);
        }
    }
#endif

#if defined MMIIM_TOUCH_SMART_CAPS
#if defined IM_ENGINE_CSTAR
        Cstar_SetCapsLock (data_ptr);
#elif defined IM_ENGINE_SOGOU
        Sogou_SetCapsLock (data_ptr);
#endif
#endif

    return is_cap_set;
}


/*==============================================================================
Description: CapsCommitStr
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN CapsCommitStr(IM_DATA_T *data_ptr, wchar *str, uint32 len)
{
    BOOLEAN is_cap_set = FALSE;

    CHECK_DATA_EX(data_ptr);

    CommitStr(data_ptr, str, len);

    //不存在大小写
    if (data_ptr->init_param.method_param_ptr == PNULL
        || data_ptr->init_param.method_param_ptr->im_def_ptr == PNULL
        || (!MMIAPIIM_CapsExist (data_ptr->init_param.method_param_ptr->im_def_ptr->lang)))
    {
        SCI_TRACE_LOW ("[MMIIM] CapsCommitStr");
        return FALSE;
    }

    //URL不需要自动大小写切换
    if (data_ptr->init_param.method_param_ptr->tag == GUIIM_TAG_EMAIL
        || data_ptr->init_param.method_param_ptr->tag == GUIIM_TAG_URL)
    {
        SCI_TRACE_LOW ("[MMIIM] CapsCommitStr needn't caps");
        return FALSE;
    }
    
    if (CAPITTAL_MODE_INITIAL == data_ptr->vkb.cap)
    {
#if defined MMIIM_TOUCH_SMART_CAPS        
        if (DialogSeperateChar (str[len - 1]) )     //如果为一句话结束，不需要切换
        {
            return FALSE;
        }
#endif

        SetVkbCaps(data_ptr, CAPITTAL_MODE_LOWER);
        is_cap_set = TRUE;
#if defined MMIIM_TOUCH_SMART_CAPS
        SetCapsSwitchType (CAPS_SWITCH_TYPE_COMMIT_E);
#endif
    }
#if defined MMIIM_TOUCH_SMART_CAPS
    else if (CAPITTAL_MODE_LOWER == data_ptr->vkb.cap)  //有可能需要从小写切换到大写，如一句话结束等
    {
        if (GetCapsSwitchType () != CAPS_SWITCH_TYPE_USER_E
            && DialogSeperateChar (str[len - 1]) )
        {
            SetVkbCaps(data_ptr, CAPITTAL_MODE_INITIAL);
            is_cap_set = TRUE;
            SetCapsSwitchType (CAPS_SWITCH_TYPE_COMMIT_E);
        }
    }
#endif

#if defined MMIIM_TOUCH_SMART_CAPS
#if defined IM_ENGINE_CSTAR
        Cstar_SetCapsLock (data_ptr);
#elif defined IM_ENGINE_SOGOU
        Sogou_SetCapsLock (data_ptr);
#endif
#endif

    return is_cap_set;
}


#if defined MMIIM_TOUCH_SMART_CAPS
/*----------------------------------------------------------------------------------------------------------------------
Name:   SetCapsSwitchType

Func:   记录当前大小写切换引起的原因

Thoughtway:
            智能大小写，如果程序自动帮用户切换，但用户又不想切换
        
Create: yaoguangchen@spread. 2012-02-01

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void SetCapsSwitchType (CAPS_SWITCH_TYPE_E switch_type)
{
    s_caps_switch_type = switch_type;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   CAPS_SWITCH_TYPE_E

Func:   根据当前大小写切换引起的原因，判断是否需要自动切换大小写

Thoughtway:
            智能大小写，如果程序自动帮用户切换，但用户又不想切换
        
Create: yaoguangchen@spread. 2012-02-01

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC CAPS_SWITCH_TYPE_E GetCapsSwitchType ()
{
    return s_caps_switch_type;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   DialogSeperateChar

Func:   判断是否为一句话的结束标志符号

Create: yaoguangchen@spread. 2012-02-01

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN DialogSeperateChar (wchar c)
{
    if (c == '.' || c == '?' || c == '!' )
        return TRUE;

    return FALSE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   WordSeperateChar

Func:   判断是否为一个单词的分割符号

Create: yaoguangchen@spread. 2012-02-01

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN WordSeperateChar (wchar c)
{
    if (c == ' ' || c == '\n' || c == 0x3000)  
        return TRUE;

    return DialogSeperateChar (c);
}

#endif


/*==============================================================================
Description: 设置候选字母的索引
Global resource dependence: g_select_letter_index
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void SetLetterSelectIndex(int32 index)
{
    g_select_letter_index = index;
}

/*==============================================================================
Description: 获取候选字母的索引
Global resource dependence: g_select_letter_index
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC int32 GetLetterSelectIndex()
{
    return g_select_letter_index;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   RefreshTipLetterSelectIndex

Func:   根据当前touch上的点坐标，更新当前选择的字母，供touch移动来选择字母

Input:
            data_ptr:  输入法结构体指针
            point_ptr: 当前touch上的坐标位置
            repaint: 是否理解绘制

Output:
            g_select_letter_index: 如果移动到一个新的letter下，则需要更新该值

Return:
            err: < 0  succ: == 0

Thoughtway:
        1.每个字母所在区域的宽度，假设与键盘上的按键宽度相同
        2.tips绘制方式采用边距TIP_SELECT_LETTER_SPACE_X 及按键间距TIP_SELECT_LETTER_INTERAL_X
        3.函数存在一个缺陷，即不应该在每次刷新时直接调用ShowTip，因函数ShowTip
          涉及到层的消耗等操作，而当前的操作仅仅需要刷新层中的内容即可，
          此处暂时将功能完成，后期有时间再优化

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 RefreshTipLetterSelectIndex(IM_DATA_T *data_ptr, GUI_POINT_T *point_ptr, BOOLEAN repaint)
{
    int32 text_width = 0;
    int32 prev_index = g_select_letter_index;

    CHECK_DATA(data_ptr);
    if (point_ptr == PNULL || data_ptr->vkb.last_key == PNULL)
    {
        //SCI_TRACE_LOW:"RefreshTipLetterSelectIndex"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_COMMON_5231_112_2_18_2_27_16_92,(uint8*)"");
        return -1;
    }

    //计算坐标
    if (point_ptr->x < data_ptr->rect_disp_tips.left + TIP_SELECT_LETTER_SPACE_X
            || point_ptr->x > data_ptr->rect_disp_tips.right - TIP_SELECT_LETTER_SPACE_X)   //在按键以外的坐标，则去掉高亮选择
    {
        g_select_letter_index = -1;
    }
    else
    {
        text_width = (data_ptr->rect_disp_tips.right - data_ptr->rect_disp_tips.left - 2 * TIP_SELECT_LETTER_SPACE_X + TIP_SELECT_LETTER_INTERAL_X)
                     / (data_ptr->vkb.last_key->text_len) - TIP_SELECT_LETTER_INTERAL_X;
        g_select_letter_index = (point_ptr->x - data_ptr->rect_disp_tips.left - TIP_SELECT_LETTER_SPACE_X) / (TIP_SELECT_LETTER_INTERAL_X + text_width);
    }

    if (repaint && g_select_letter_index != prev_index)
    {
        ShowTip(data_ptr, data_ptr->vkb.last_key);
    }

    return 0;
}


PUBLIC BOOLEAN ChooseLetter(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    GUI_POINT_T point = {0, 0};

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return FALSE;
    }
    //切换到选择字母状态
    if (MMIAPICOM_Wstrlen(key_ptr->text) > 1)
    {
        point.x = key_ptr->rect.left + 1;
        data_ptr->vkb.state |= KEYBOARD_CHOOSE_LETTER;
        data_ptr->vkb.is_key_block = 1; //锁定键盘
        RefreshTipLetterSelectIndex(data_ptr, &point, FALSE);

        ShowTip(data_ptr, key_ptr);
        DrawVkb(data_ptr);
    }

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   GetMultiSymCharPtr

Func:   如果某些语言不支持大小写切换，则将大小写切换键配置为多个符号输入按键，
            此函数获取按键内的符号数组指针

Input:
            lang:  输入法对应的语言

Output:
            s_select_letter_index: 如果移动到一个新的letter下，则需要更新该值

Return:
            err: < 0  succ: == 0

Thoughtway:

Create: yaoguangchen@spread. 2011-10-11

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC const wchar * GetMultiSymCharPtr(GUIIM_LANGUAGE_T lang)
{
    const wchar * psym = PNULL;
    switch (lang)
    {
#if defined IM_ARABIC_SUPPORT
    case GUIIM_LANG_ARABIC:
        {
            psym = arabic_mul_sym;
            break;
        }
#endif

#if defined IM_THAI_SUPPORT
    case GUIIM_LANG_THAI:
        {
            psym = thai_mul_sym;
            break;
        }
#endif

    case GUIIM_LANG_ENGLISH:
        {
            psym = normal_mul_sym;
            break;
        }
    default:
        {
            psym = normal_mul_sym;
            break;
        }
    }
    
    return psym;
}



/*----------------------------------------------------------------------------------------------------------------------
Name:   MultiCharLong

Func:   multi-type key长按输入，弹出选择字符状态

Input:
            data_ptr:    输入法指针
            key_ptr:    对应的字符按键

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN MultiCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    GUI_POINT_T point = {0, 0};

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return FALSE;
    }
    //切换到选择字母状态
    if (MMIAPICOM_Wstrlen(key_ptr->text) > 1)
    {
		MMIIM_SetVKeyPaintStatus (key_ptr, TRUE);

        point.x = key_ptr->rect.left + 1;
        data_ptr->vkb.state |= KEYBOARD_CHOOSE_LETTER;
        data_ptr->vkb.is_key_block = 1; //锁定键盘
        RefreshTipLetterSelectIndex(data_ptr, &point, FALSE);

        ShowTip(data_ptr, key_ptr);
        DrawVkb(data_ptr);
    }

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:  QwertKbLayoutType

Func:    根据输入法，获取键盘布局类型
Input:

Create: yaoguangchen@spread. 2011-01-08

Modify:	某些语言存在多张键盘布局，而每张键盘的布局字符按键个数是不同的，添加键盘索引

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC QWERT_KEYBOARD_LAYOUT_TYPE_E QwertKbLayoutType (GUIIM_METHOD_T method, uint8 cur_page)
{
    QWERT_KEYBOARD_LAYOUT_TYPE_E layout = KEYBOARD_LAYOUT_26CHAR;       //默认字符按键个数

    switch (method)
    {
        case GUIIM_M_Q_AMHARIC: 
        case GUIIM_M_TPQ_AMHARIC: 
        case GUIIM_M_Q_AMHARIC_ABC:
        case GUIIM_M_TPQ_AMHARIC_ABC:
            
            layout = KEYBOARD_LAYOUT_28CHAR;
            break;

		case GUIIM_M_Q_TIBETAN:
		case GUIIM_M_TPQ_TIBETAN:
		case GUIIM_M_Q_TIBETAN_ABC:
		case GUIIM_M_TPQ_TIBETAN_ABC:
		{
			layout = KEYBOARD_LAYOUT_26CHAR;
			break;
		}

		case GUIIM_M_TPQ_ZHUYIN:
            layout = KEYBOARD_LAYOUT_40CHAR;
            break;
		
        default:
            break;
    }
    
    return layout;
}


#if 0   //暂时没有区分键盘主题
PUBLIC THEME_T QwertKbGetTheme (QWERT_KEYBOARD_LAYOUT_TYPE_E layout)
{
    THEME_T theme = g_theme_qwert_default;
    switch (layout)
    {
        case KEYBOARD_LAYOUT_26CHAR: 
            break;
            
        case KEYBOARD_LAYOUT_28CHAR:
            break;
            
        default:
            break;
    }
    
    return theme;
}
#endif


/*----------------------------------------------------------------------------------------------------------------------
Name:  QwertKbGetPosition_ptr

Func:    根据键盘布局,获取键盘按键坐标
Input:

Create: yaoguangchen@spread. 2011-01-08

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC GUI_RECT_T const* QwertKbGetPosition_ptr (QWERT_KEYBOARD_LAYOUT_TYPE_E layout)
{
    GUI_RECT_T const *pos = g_qwert_panel_position;
    switch (layout)
    {
        case KEYBOARD_LAYOUT_28CHAR:
            pos = g_28char_panel_position;
            break;

        case KEYBOARD_LAYOUT_40CHAR:
            pos = g_40char_panel_position;
            break;

        case KEYBOARD_LAYOUT_26CHAR:                     
        default:
            break;
    }
    
    return pos;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:  QwertKbGetPosition_ptr

Func:    根据键盘布局,获取键盘按键个数
Input:

Create: yaoguangchen@spread. 2011-01-08

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint32 QwertKbGetKeyCnt (QWERT_KEYBOARD_LAYOUT_TYPE_E layout)
{
    uint32 cnt = g_qwert_panel_position_size;
    switch (layout)
    {
        case KEYBOARD_LAYOUT_28CHAR:
            cnt = g_28char_panel_position_size;
            break;

		case KEYBOARD_LAYOUT_40CHAR:
			cnt = g_40char_panel_position_size;
			break;
			
        case KEYBOARD_LAYOUT_26CHAR:             
        default:
            break;
    }
    
    return cnt;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:  QwertKbGetCharKeyCnt

Func:    根据键盘布局,获取键盘输入法引擎或字符按键个数
Input:

Create: yaoguangchen@spread. 2011-01-08

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint8 QwertKbGetCharKeyCnt (QWERT_KEYBOARD_LAYOUT_TYPE_E layout)
{
    uint8 cnt = 26;
    switch (layout)
    {
        case KEYBOARD_LAYOUT_28CHAR:
            cnt = 28;
            break;

		case KEYBOARD_LAYOUT_40CHAR:
			cnt = 40;
			
        case KEYBOARD_LAYOUT_26CHAR: 
        default:
            break;
    }
    
    return cnt;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:  QwertKbGetKeyMap_ptr

Func:    根据键盘布局,获取键盘字符按键与引擎按键之间的对应关系
Input:

Create: yaoguangchen@spread. 2011-01-08

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint16 const *QwertKbGetKeyMap_ptr (QWERT_KEYBOARD_LAYOUT_TYPE_E layout)
{
    uint16 const *keymap_ptr = g_kb_keymap;
    switch (layout)
    {
        case KEYBOARD_LAYOUT_28CHAR:
            keymap_ptr = g_28char_kb_keymap;
            break;

		case KEYBOARD_LAYOUT_40CHAR:
            keymap_ptr = g_40char_kb_keymap;
            break;
			
        case KEYBOARD_LAYOUT_26CHAR: 
        default:
            break;
    }
    
    return keymap_ptr;
}


/*==============================================================================
Description: 是否禁止本地语输入
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN MMIIM_IsLimitSwitchLang (GUIIM_METHOD_PARAM_T *method_param_ptr)
{
    if (method_param_ptr == PNULL)
        return FALSE;

    if (GUIIM_TAG_EMAIL == method_param_ptr->tag
        || GUIIM_TAG_URL == method_param_ptr->tag
        || (GUIIM_LIMIT_LOCAL & method_param_ptr->limit)
        || MMIAPIIM_GetPrevLanguage() == GUIIM_LANG_NONE)       //有且仅有英语输入，本地语没有设置
    {
        return TRUE;
    }

    return FALSE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:  MMIIM_ConfigSettingMenu

Func:    配置输入法选择菜单，当前语言支持的输入法和更多菜单

Input:

Create: yaoguangchen@spread. 2012-02-01

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 MMIIM_ConfigSettingMenu (
                                               IM_DATA_T *data_ptr
                                               )
{
    int16 index =0 ;//indicate index of setting_menu_data
    uint32 item_idx = 0;
    GUIIM_LANGUAGE_T guiim_cur_lang = GUIIM_LANG_NONE;
    //GUIIM_LANGUAGE_T guiim_local_lang = MMIAPIIM_GetPrevLanguage ();
    GUIIM_METHOD_CFG_T *cur_method_cfg_ptr = PNULL;

    CHECK_DATA_EX (data_ptr);

    if (data_ptr->init_param.method_param_ptr == PNULL 
        || data_ptr->init_param.method_param_ptr->im_def_ptr == PNULL)
    {
        //SCI_TraceLow:"MMIIM_OpenMethodSettingMenu  data_ptr->init_param.method_param_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_COMMON_5571_112_2_18_2_27_16_93,(uint8*)"");
        return -1;
    }
    
    data_ptr->settingim_menu_index = (int16 *)SCI_ALLOCA( data_ptr->setting_menu_size * sizeof(int16));
    
    cur_method_cfg_ptr = data_ptr->init_param.method_param_ptr->im_def_ptr;
    guiim_cur_lang = cur_method_cfg_ptr->lang;
    if (guiim_cur_lang == GUIIM_LANG_NONE)  //手写时，该值为none
    {
        guiim_cur_lang = MMIAPIIM_GetLanguage();
    }

    for (item_idx = 0; item_idx < data_ptr->setting_menu_size; item_idx++)
    {
        if (data_ptr->setting_menu_data[item_idx].lang == guiim_cur_lang)   //根据语言配置的菜单
        {
//即使本地语没有手写，英文手写还是应该保留的，
//如果进入了英文手写，再考虑手写语言切换功能            
#if 0            
            //如果本地语没有手写时，不需要配置手写菜单
            if (guiim_cur_lang == GUIIM_LANG_ENGLISH 
                && data_ptr->setting_menu_data[item_idx].input == GUIIM_INPUT_TYPE_HANDWRITING)     //英文手写
            {
                if (MMIIM_IsLimitSwitchLang (data_ptr->init_param.method_param_ptr))    //没有设置本地语，则手写不需要配置
                    continue;
                else if (!(HasConfigSelectInput (data_ptr, guiim_local_lang, GUIIM_INPUT_TYPE_HANDWRITING)))    //本地语不支持手写
                    continue;
            }
#endif

            //NEWMS00202927 [6530][128X64_320X480PDA2]在输入帐号密码的界面,选择手写输入，写出的字还没来及选择就变成密码的“*”了
            if (data_ptr->setting_menu_data[item_idx].input == GUIIM_INPUT_TYPE_HANDWRITING)
            {
                MMI_HANDLE_T edit_handle = MMK_GetParentCtrlHandle (data_ptr->init_param.im_handle);
                MMI_CTRL_ID_T edit_id = MMK_GetCtrlId (edit_handle);

                if (GUIEDIT_GetType (edit_id) == GUIEDIT_TYPE_PASSWORD)
                {
                    continue;
                }
            }
 
            //记住当前默认的index，在move到菜单外时，高亮显示
            if ((data_ptr->setting_menu_data[item_idx].type == cur_method_cfg_ptr->type) 
                && (data_ptr->setting_menu_data[item_idx].type != GUIIM_TYPE_NONE))         //smart multitap
            {
                SetMethodSelectMenuDefaultIndex (index);        
            }
            else if ((data_ptr->setting_menu_data[item_idx].input == cur_method_cfg_ptr->input) && 
                (data_ptr->setting_menu_data[item_idx].input != GUIIM_INPUT_TYPE_NONE))     //handwriting
            {
                SetMethodSelectMenuDefaultIndex (index);        
            }

            data_ptr->settingim_menu_index[index] = item_idx;  
            index ++;
        }
        else if (data_ptr->setting_menu_data[item_idx].lang == 0)      //没有配置语言，即表示非输入法菜单，如更多等
        {
            //NEWMS00195760 [6530][128X64_320X480PDA2]互联网界面切换到笔画后再切到且他语言还是笔画输入
            //不允许设置输入语言时，将更多屏蔽掉，依赖于s_setting_menu_data
            //理论上应该在输入法法设置里面单独设置输入语言是否可用的，但是
            //输入语言设置可以从设置菜单中进入，这样无法区分到底是谁引起的不可设置语言
            //修改输入语言切换功能时，引起的新bug
            if ((GUIIM_LIMIT_LOCAL & data_ptr->init_param.method_param_ptr->limit) == 0)
            {
                data_ptr->settingim_menu_index[index] = item_idx;  
                index ++;
            }
        }
    }

    data_ptr->settingim_item_count  = index ; //settingim_item_count
    
    if(PNULL == data_ptr->settingim_menu_index)
    {
        //SCI_TRACE_LOW:"MMIIM_ConfigSettingMenu"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_COMMON_5627_112_2_18_2_27_16_94,(uint8*)"");
        return -2;
    }        

    return 0;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:  MMIIM_CreateSettingMenuLayer

Func:    创建输入法选择菜单层

Input:

Create: yaoguangchen@spread. 2012-02-01

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 MMIIM_CreateSettingMenuLayer (IM_DATA_T *data_ptr, GUI_POINT_T pos)
{
    
    int16                   setting_im_rect_width = SETTING_IM_RECT_WIDTH;
    int16                   max_height = SETTING_IM_RECT_HEIGHT;    //layer max height
    UILAYER_CREATE_T        create_info = {0};
    UILAYER_APPEND_BLT_T    append_layer = {0};
    int16 setting_im_item_height = MMIIM_TOUCH_GetKbKeyHeight (data_ptr);   /*lint !e746*/
    
    CHECK_DATA_EX(data_ptr);
    
    if(data_ptr->is_setting_display)
    {
        //SCI_TRACE_LOW:"MMIIM_CreateSettingMenuLayer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_COMMON_5657_112_2_18_2_27_17_95,(uint8*)"");
        return -1;
    }
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        setting_im_rect_width = SETTING_IM_RECT_WIDTH_H;
        max_height = SETTING_IM_RECT_HEIGHT_H;
    }

    //create layer
    create_info.lcd_id = data_ptr->init_param.lcd_info_ptr->lcd_id;
    create_info.owner_handle = data_ptr->init_param.im_handle;
    create_info.width = setting_im_rect_width;
    create_info.height = (data_ptr->settingim_item_count * setting_im_item_height < max_height)? 
        (data_ptr->settingim_item_count * setting_im_item_height) : max_height;
    create_info.offset_x = (MMITHEME_GetClientRectWidth () - create_info.width) / 2;    //居中显示
    create_info.offset_y = MAX (pos.y - create_info.height, data_ptr->rect.top);	//可能修改了设置菜单按键坐标，导致setting菜单高于虚拟键盘而无法操作
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    
    UILAYER_CreateLayer(&create_info, &data_ptr->setting_im_layer);
    UILAYER_RemoveBltLayer(&data_ptr->setting_im_layer);
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    append_layer.lcd_dev_info = data_ptr->setting_im_layer;
    UILAYER_AppendBltLayer(&append_layer);
    
    //initialize global params
    data_ptr->settingim_halfposition = SETTINGIM_DOWN_HALF;
    data_ptr->settingim_top_item = 0;
    data_ptr->settingim_last_highlight = GetMethodSelectMenuDefaultIndex();

    data_ptr->is_setting_display = TRUE;
    
    return 0;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:  MMIIM_DrawSettingMenu

Func:    绘制输入法选择菜单，当前选择的菜单项高亮显示

Input:

Create: yaoguangchen@spread. 2012-02-01

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void MMIIM_DrawSettingMenu (
                                IM_DATA_T *    data_ptr,
                                int16          highlight_index //index of item will highlight
                                )
{
    int16 item_index = 0;
    GUI_RECT_T list_rect={0};
    GUI_RECT_T item_rect={0};
    GUI_RECT_T str_rect = {0};
    GUI_RECT_T highlight_rect = {0};
    GUISTR_STYLE_T str_style = {0};
    int16 setting_im_item_height = MMIIM_TOUCH_GetKbKeyHeight (data_ptr);   /*lint !e746*/
    
    CHECK_DATA_EX(data_ptr);
    
    if (!UILAYER_IsLayerActive(&data_ptr->setting_im_layer) || FALSE == data_ptr->is_setting_display)
    {
        return;
    }  

    list_rect = UILAYER_GetLayerRect(&data_ptr->setting_im_layer);
    UILAYER_ClearRect(&data_ptr->setting_im_layer,list_rect);
    
    str_style.font = SETTING_IM_FONT;
    str_style.font_color = MMI_WHITE_COLOR;
    str_style.align = ALIGN_LVMIDDLE; 
    
    item_rect = list_rect;
    item_rect.bottom = item_rect.top + setting_im_item_height-1;
    
    //move up half item
    if (SETTINGIM_UP_HALF== data_ptr->settingim_halfposition)
    {
        item_rect.top -= setting_im_item_height/2;
        item_rect.bottom -= setting_im_item_height/2;
    }
    
#ifdef SXH_APPLE_SUPPORT
if(AppleIsRun())
{
	data_ptr->settingim_item_count-=1;//去掉更多选项
}
#endif

	
    for (item_index = data_ptr->settingim_top_item; item_index < data_ptr->settingim_item_count; item_index++)
    {
        MMI_STRING_T disp_str = {0}; 
        GUI_RECT_T border_rect = {0};
        
        //display up border
        border_rect = item_rect;
        border_rect.bottom = border_rect.top;
        
        LCD_FillRectU32(&data_ptr->setting_im_layer,border_rect,SETTINGIM_DARK_COLOR);
        
        //display item
        item_rect.top +=1;
        item_rect.bottom -=1;
        
        if( (item_index == highlight_index + data_ptr->settingim_top_item))
        {
            //display highlight image
            highlight_rect = item_rect;
            
            if (0 == highlight_index)
            {
                highlight_rect.top -= 1; //not not display border, display heighligth img
            }
            
            GUIRES_DisplayImg(PNULL,&highlight_rect,&highlight_rect,data_ptr->init_param.win_handle,IMAGE_COMMON_SELECTED_BAR,&data_ptr->setting_im_layer);
            str_style.font_color = MMI_BLACK_COLOR;
        }
        else
        {
            LCD_FillRectU32(&data_ptr->setting_im_layer,item_rect,SETTINGIM_ITEM_COLOR);
        }
        
        //display text
        str_rect = item_rect;
        str_rect.left += 18;
        
        MMITHEME_GetResText(data_ptr->setting_menu_data[data_ptr->settingim_menu_index[item_index]].text_id,
            data_ptr->init_param.win_handle, &disp_str);
        
        GUISTR_DrawTextToLCDInRect(&data_ptr->setting_im_layer,&str_rect,&str_rect,&disp_str,&str_style,GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS,GUISTR_TEXT_DIR_RTL);
        str_style.font_color = MMI_WHITE_COLOR; //reset font color
        
        item_rect.top -= 1;
        item_rect.bottom += 1;
        
        //display bottom border
        border_rect = item_rect;
        border_rect.top = border_rect.bottom;
        
        LCD_FillRectU32(&data_ptr->setting_im_layer,border_rect,SETTINGIM_BRIGHT_COLOR);
        
        //change item_rect to next item
        item_rect.top += setting_im_item_height;
        item_rect.bottom += setting_im_item_height;
        
        if (item_rect.top >= list_rect.bottom)
        {
            break;
        }
    }
    
    return;    
}


/*----------------------------------------------------------------------------------------------------------------------
Name:  MMIIM_OpenMethodSettingMenu

Func:    打开输入法选择菜单

Input:

Create: yaoguangchen@spread. 2012-02-01

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 MMIIM_OpenMethodSettingMenu (
                                IM_DATA_T *    data_ptr, 
                                GUI_POINT_T pos
                                )
{
    CHECK_DATA_EX (data_ptr);

    data_ptr->setting_isst_e = EISST_NONE;        //滑动操作菜单方式标志

    MMIIM_ConfigSettingMenu (data_ptr);
    MMIIM_CreateSettingMenuLayer (data_ptr, pos);
    MMIIM_DrawSettingMenu (data_ptr, GetMethodSelectMenuDefaultIndex ());

    return 0;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:  MMIIM_CloseMethodSettingMenu

Func:   关闭输入法选择菜单，释放创建层

Input:

Create: yaoguangchen@spread. 2012-02-01

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 MMIIM_CloseMethodSettingMenu (
                                IM_DATA_T *    data_ptr
                                )
{
    CHECK_DATA_EX (data_ptr);

    if (data_ptr->is_setting_display)
    {
        data_ptr->is_setting_display = FALSE;
        UILAYER_ReleaseLayer (&data_ptr->setting_im_layer);
        SCI_FREEIF (data_ptr->settingim_menu_index);
    }

    data_ptr->vkb.state &= (~KEYBOARD_CHOOSE_MENU);
    data_ptr->vkb.is_key_block = 0;
    
    return 0;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:  MMIIM_MethodSettingMenuDown

Func:   输入法选择菜单弹出时的点击操作

Input:

Create: yaoguangchen@spread. 2012-02-10

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 MMIIM_MethodSettingMenuDown (
                                IM_DATA_T *    data_ptr, 
                                GUI_POINT_T point, 
                                BOOLEAN repaint
                                )
{
    int16 cur_index = GetMethodSelectMenuDefaultIndex ();
    GUI_RECT_T layer_rect = {0, 0, 0, 0};
    int16 setting_im_item_height = MMIIM_TOUCH_GetKbKeyHeight (data_ptr);   /*lint !e746*/

    CHECK_DATA_EX (data_ptr);

    layer_rect = UILAYER_GetLayerRect (&data_ptr->setting_im_layer);
    if (GUI_PointIsInRect(point, layer_rect))
    {
        cur_index = (point.y - layer_rect.top) / setting_im_item_height;
    }
    else
    {
        if (data_ptr->vkb.last_key)
        {
            data_ptr->vkb.last_key->is_press = 0;
            data_ptr->vkb.last_key = PNULL; //UP消息丢掉
        }
        MMIIM_CloseMethodSettingMenu (data_ptr);
        return 0;
    }
    
    if (data_ptr->settingim_last_highlight != cur_index)
    {
        data_ptr->settingim_last_highlight = cur_index;
        if (repaint)
        {
            MMIIM_DrawSettingMenu (data_ptr, cur_index);
        }
    }
    
    return 0;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:  MMIIM_MethodSettingMenuRefresh

Func:   用户操作输入法选择菜单时，及时刷新输入法菜单高亮条

Input:

Create: yaoguangchen@spread. 2012-02-01

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 MMIIM_MethodSettingMenuRefresh (
                                IM_DATA_T *    data_ptr, 
                                GUI_POINT_T point, 
                                BOOLEAN repaint
                                )
{
    int16 cur_index = GetMethodSelectMenuDefaultIndex ();
    GUI_RECT_T layer_rect = {0, 0, 0, 0};
    int16 setting_im_item_height = MMIIM_TOUCH_GetKbKeyHeight (data_ptr);   /*lint !e746*/
    CHECK_DATA_EX (data_ptr);

    data_ptr->setting_isst_e = EISST_MOVE_SELECT;
    layer_rect = UILAYER_GetLayerRect (&data_ptr->setting_im_layer);
    if (GUI_PointIsInRect(point, layer_rect))
    {
        cur_index = (point.y - layer_rect.top) / setting_im_item_height;
    }
    
    if (data_ptr->settingim_last_highlight != cur_index)
    {
        data_ptr->settingim_last_highlight = cur_index;
        if (repaint)
        {
            MMIIM_DrawSettingMenu (data_ptr, cur_index);
        }
    }
    
    return 0;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:  MMIIM_MethodSettingMenuUp

Func:   用户点击输入法选择菜单的操作响应，选择输入法or进入更多菜单

Input:

Create: yaoguangchen@spread. 2012-02-01

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC MMI_RESULT_E MMIIM_MethodSettingMenuUp(
                                            IM_DATA_T *data_ptr
                                            )
{
    int16           index = 0;//indicate which item was slided
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    //KEY_INFO_T *key_set = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    if ((data_ptr->vkb.state & KEYBOARD_CHOOSE_MENU) == 0)
    {
        //SCI_TraceLow:"MMIIM_MethodSettingMenuUp"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_COMMON_5973_112_2_18_2_27_17_96,(uint8*)"");
        return result;
    }
    
    if (FALSE == data_ptr->is_setting_display)
    {
        return result;
    }

    //点击在set按键上，直接up，继续保留菜单
    if (data_ptr->setting_isst_e == EISST_NONE)
    {
        data_ptr->setting_isst_e = EISST_CLICK_SELECT;
        return MMI_RESULT_TRUE;
    }
    
    //先将vkey高亮释放
    //key_set = FindKeyWithCode (data_ptr, VKEY_SET);
    //VkbUp(data_ptr, key_set);
    
    index = data_ptr->settingim_last_highlight;    
    //更多，某些情况下，程序异常导致没有可用的输入法
    if (data_ptr->settingim_menu_index[index] == data_ptr->setting_menu_size - 1)  /*lint !e574 !e737*/
    {
        MMI_HANDLE_T win_handle = 0;
        
        win_handle = MMIAPIIM_OpenSetWin();
        
        //如下情况不再处理消息
        //1 数据校验信息被破坏
        //2 控件已经被销毁
        if (sizeof(IM_DATA_T) != data_ptr->check_info) return MMI_RESULT_TRUE;
        if (PNULL == MMK_GetCtrlPtr(data_ptr->init_param.im_handle)) return MMI_RESULT_TRUE;
        
        //enter more setting...
        data_ptr->more_setting_win_handle = win_handle;
        
        if (win_handle != 0)
        {
            GUIIM_AppendWin(data_ptr->init_param.im_handle, win_handle);
        }
    }  
    else if ( index != GetMethodSelectMenuDefaultIndex ())   //输入法选择菜单发生改变
    {
        //输入法选择
        if ( data_ptr->settingim_menu_index[index] < data_ptr->setting_menu_size - 1)   /*lint !e574 !e737*/
        {
            //SettingMethod will destroy im data,RECODE im index before SettingMethod
            int16 cur_im_index = data_ptr->settingim_menu_index[index];

            MMIIM_DestroySettingIMData(data_ptr);
            SettingMethod(data_ptr,
                data_ptr->setting_menu_data[cur_im_index].lang,
                data_ptr->setting_menu_data[cur_im_index].type,
                data_ptr->setting_menu_data[cur_im_index].input);

            return result;
        }
    }
    
    //reset params
    data_ptr->settingim_last_highlight = 0;
    data_ptr->settingim_top_item = 0;
    data_ptr->settingim_halfposition = SETTINGIM_DOWN_HALF;
    data_ptr->settingim_item_count = 0;
    
    MMIIM_DestroySettingIMData(data_ptr);
    
    result = MMI_RESULT_TRUE;

    data_ptr->vkb.state &= (~KEYBOARD_CHOOSE_MENU);
    data_ptr->vkb.is_key_block = 0;
    
    return result;
}


/*==============================================================================
Description: 输入法选择菜单键，仅仅在第一次按下时有效，一旦菜单弹出，则
                该按键的消息被菜单拦截掉了
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN MMIIM_VkeyUpSet(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    GUI_POINT_T point = {0, 0};
    GUI_RECT_T kb_rect = {0};
    
    if (PNULL == key_ptr)
    {
        //SCI_TraceLow:"MMIIM_VkeyUpSet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_COMMON_6063_112_2_18_2_27_17_97,(uint8*)"");
        return FALSE;
    }

    CHECK_DATA_EX(data_ptr);

    if (key_ptr->is_press)
    {
        key_ptr->is_press = 0;
    }

	MMIIM_SetVKeyPaintStatus (key_ptr, TRUE);
    DrawVkb(data_ptr);

    if (data_ptr->vkb.state & KEYBOARD_CHOOSE_MENU)
    {
        MMIIM_CloseMethodSettingMenu (data_ptr);
        return TRUE;
    }
    
    //进入选择菜单状态
    data_ptr->vkb.state |= KEYBOARD_CHOOSE_MENU;
    data_ptr->vkb.is_key_block = 1; //锁定键盘

    kb_rect = UILAYER_GetLayerRect(&data_ptr->ui_layer);
    point.x = key_ptr->rect.left + kb_rect.left; 
    point.y = key_ptr->rect.top + kb_rect.top;
    MMIIM_OpenMethodSettingMenu (data_ptr, point);
    
    return TRUE;
}

/*==============================================================================
Description: MMIIM_GetIMCaps
Global resource dependence: NONE
Author: haiwu.chen
Note: 把输入法内部的大小写属性改成输入法需要的属性
==============================================================================*/
PUBLIC GUIIM_CAPITAL_MODE_T MMIIM_GetIMCaps(CAPITTAL_MODE_T cap)
{
    GUIIM_CAPITAL_MODE_T im_cap = GUIIM_MODE_MAX;

    switch (cap)
    {
    case CAPITTAL_MODE_LOWER:
        im_cap = GUIIM_MODE_LOWER;
        break;
    case CAPITTAL_MODE_UPPER:
        im_cap = GUIIM_MODE_UPPER;
        break;
    case CAPITTAL_MODE_INITIAL:
        im_cap = GUIIM_MODE_LEAD_UPPER;
        break;
    default:
        break;
    }

    return im_cap;
}

/*==============================================================================
Description: 当前输入法面板每行按键最小高度
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL int16 MMIIM_TOUCH_GetKbKeyHeight (IM_DATA_T *data_ptr)
{
    GUI_RECT_T full_rect = MMITHEME_GetFullScreenRectEx(data_ptr->init_param.win_handle);
    int16 height = (full_rect.bottom - full_rect.top) * MMIIM_PANEL_PERCENT_SMALL / (LATIN_CANDIDATE_ROW * 100);

    return height;
}


/*==============================================================================
Description: 统一字符按键text字段字符大小，因量产原因，先将输入法
                作为内部使用参数，将可能引起问题的范围缩小到指定输入法中
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_KbUnifiedCharKeysFontSize (IM_DATA_T *data_ptr)
{
    BOOLEAN need_unified = FALSE;
    GUIIM_METHOD_T method = GUIIM_M_NONE;
    GUI_FONT_T font_size = 0;

    CHECK_DATA (data_ptr);

    method = data_ptr->init_param.method_param_ptr->im_def_ptr->method;
    switch (method)
    {
        //9键，26键以abc显示的键盘状态
        case GUIIM_M_TP_ENGLISH:
        case GUIIM_M_TP_ENGLISH_LEAD_UPPER:
        case GUIIM_M_TP_ENGLISH_LOWER:
        case GUIIM_M_TP_ENGLISH_UPPER:    
        case GUIIM_M_TPQ_ENGLISH:
        case GUIIM_M_TPQ_ENGLISH_LEAD_UPPER:
        case GUIIM_M_TPQ_ENGLISH_LOWER:
        case GUIIM_M_TPQ_ENGLISH_UPPER:    
            
        case GUIIM_M_TP_ENGLISH_ABC:
        case GUIIM_M_TP_ENGLISH_ABC_LEAD_UPPER:
        case GUIIM_M_TP_ENGLISH_ABC_LOWER:
        case GUIIM_M_TP_ENGLISH_ABC_UPPER:    
        case GUIIM_M_TPQ_ENGLISH_ABC:
        case GUIIM_M_TPQ_ENGLISH_ABC_LEAD_UPPER:
        case GUIIM_M_TPQ_ENGLISH_ABC_LOWER:
        case GUIIM_M_TPQ_ENGLISH_ABC_UPPER:    

        case GUIIM_M_TP_PINYIN:
        case GUIIM_M_TPQ_PINYIN:    
            need_unified = TRUE;
            break;
            
        default:
            need_unified = TRUE;        //JUST TEST
            break;
    }

    if (need_unified)   /*lint !e774*/       //这里暂时将所有的语言均按统一字号处理，后期如果某些语言存在异常，再做添加修改
    {
        font_size = (GUI_FONT_T) MMIIM_TOUCH_ComputKbUnifiedFontSize (data_ptr, KEY_STYLE_CHARACTER);
		MMIIM_TOUCH_SetKeyFontSize (data_ptr, KEY_STYLE_CHARACTER, font_size);
	
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    }

    return 0;
}

/*==============================================================================
Description: 获取面板统一字符大小(data, 按键类型, 按键附加属性)
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC GUI_FONT_T MMIIM_TOUCH_ComputKbUnifiedFontSize (IM_DATA_T *data_ptr, KEY_STYLE_T key_style)
{
    int32 iloop = 0;
    GUI_FONT_T cur_font = SONG_FONT_10, ret_font = SONG_FONT_40;
    
    CHECK_DATA_EX (data_ptr);

    switch (key_style)
    {
        case KEY_STYLE_CONTROL:
            break;
            
        case KEY_STYLE_CHARACTER:
        default:
        //循环查找符合属性的按键
        for (iloop = 0; iloop < data_ptr->vkb.key_cnt; iloop ++)
        {
            if (data_ptr->vkb.keys[iloop].style == key_style)
            {
                //获取每个按键字符大小
                cur_font = ComputKeyFontMaxSize (data_ptr, 
                    &data_ptr->theme.key_char[0], 
                    &data_ptr->vkb.keys[iloop]);

                //保存最小的字符
                if (ret_font > cur_font)
                    ret_font = cur_font;
            }
        }
    }

    return ret_font;    //返回最小字符即统一字符
}


/*==============================================================================
Description: 设置统一字符大小 (data, 按键类型, 按键附加属性)
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_SetKeyFontSize (
    IM_DATA_T *data_ptr, 
    KEY_STYLE_T key_style, 
    GUI_FONT_T font_size
)
{
    int32 iloop = 0;
    
    CHECK_DATA_EX (data_ptr);

    switch (key_style)
    {
        case KEY_STYLE_CONTROL:
            break;
            
        case KEY_STYLE_CHARACTER:
        default:
        //循环查找符合属性的按键
        for (iloop = 0; iloop < data_ptr->vkb.key_cnt; iloop ++)
        {
            if (data_ptr->vkb.keys[iloop].style == key_style)
            {
                //设置字符大小
                data_ptr->vkb.keys[iloop].font_size = font_size;
            }
        }
    }
		 
    return 0;    
}


/*==============================================================================
Description: 初始化输入法对应的虚拟键盘页相关属性，前提是这个输入法存在多张键盘
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_InitVkbMultiPageParam (
    GUIIM_METHOD_T method, 
    VKB_MULTI_PAGE_T *vkb_multi_page_ptr
)
{
	if (vkb_multi_page_ptr == PNULL)
	{
		SCI_TRACE_LOW ("[MMIIM]MMIIM_TOUCH_InitVkbMultiPageParam method == %d", method);
		return -1;
	}

	SCI_MEMSET(vkb_multi_page_ptr, 0, sizeof (VKB_MULTI_PAGE_T));
	switch (method)
	{
		case GUIIM_M_TPQ_TIBETAN:
		case GUIIM_M_TPQ_TIBETAN_ABC:
		{
			vkb_multi_page_ptr->cur_page = 0;
			vkb_multi_page_ptr->total_page = 2;
			
			break;
		}
		
		default:
			break;
	}
		
	return 0;
}

/*==============================================================================
Description: 设置虚拟键盘的当前页，某些输入法存在多张键盘的情况发生
	此处约定按键字符个数相等
	后期再添加字符个数不等的键盘，如繁体中文的键盘字符个数就不同
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_ResetQwertVkbChars (
    IM_DATA_T *data_ptr, 
    uint8 set_page
)
{
	uint32 iloop = 0;
	uint32 character_cnt = 0;
	uint8 cur_page = 0;
	QWERT_KEYBOARD_LAYOUT_TYPE_E layout = KEYBOARD_LAYOUT_26CHAR;
    uint16 const *keymap_ptr = g_kb_keymap;
    MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr = PNULL;
    MMIIM_SP_26KEY_KEYCHAR_T *cur_char_ptr = PNULL;
	
	//参数校验
	if (data_ptr == PNULL || data_ptr->vkb.keys == PNULL || set_page >= VKB_PAGE_MAX || set_page >= data_ptr->vkb_multi_page.total_page)
	{
		SCI_TRACE_LOW ("[MMIIM]MMIIM_TOUCH_ResetVkbChars");
		return -1;
	}

	if (data_ptr->vkb_multi_page.cur_page >= VKB_PAGE_MAX || set_page >= VKB_PAGE_MAX)
	{
		SCI_TRACE_LOW ("[MMIIM]MMIIM_TOUCH_ResetVkbChars Err cur page == %d set_page == %d", cur_page, set_page);
		data_ptr->vkb_multi_page.cur_page = 0;
	}

	if (data_ptr->vkb_multi_page.cur_page == set_page)
	{
		SCI_TRACE_LOW ("[MMIIM]MMIIM_TOUCH_ResetVkbChars == %d", set_page);
		return -2;
	}

	//更新参数
	cur_page = data_ptr->vkb_multi_page.cur_page;
	data_ptr->vkb_multi_page.cur_page = set_page;

	//清空当前按键字符数据	
	layout = QwertKbLayoutType (data_ptr->init_param.method_param_ptr->im_def_ptr->method, cur_page);
	character_cnt = QwertKbGetCharKeyCnt (layout);
	for (iloop = 0; iloop < character_cnt; iloop ++)
	{
		SCI_MEMSET (&data_ptr->vkb.keys[data_ptr->vkb.charactor_first + iloop].text, 0, sizeof (data_ptr->vkb.keys[data_ptr->vkb.charactor_first + iloop].text));
		data_ptr->vkb.keys[data_ptr->vkb.charactor_first + iloop].text_len = 0;
		data_ptr->vkb.keys[data_ptr->vkb.charactor_first + iloop].is_hide = 1;		//先将不需要的按键全部隐藏
	}
	
	//更新按键对应的字符数据
	layout = QwertKbLayoutType (data_ptr->init_param.method_param_ptr->im_def_ptr->method, set_page);
    keymap_ptr = QwertKbGetKeyMap_ptr (layout);
	switch (set_page)
	{
		case 0:
		{
			key_table_ptr = MMIIM_SP_ML26KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);    //此表的顺序是按a~z排列
		    if (key_table_ptr == PNULL)
		    {
		        key_table_ptr = MMIIM_SP_SMART26KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
		    }

			break;
		}

		case 1:
		{
			key_table_ptr = MMIIM_SP_ML26KEY_GetShiftKeyTable (data_ptr->init_param.method_param_ptr->im_def_ptr->method);
		    if (key_table_ptr == PNULL)
		    {
		        key_table_ptr = MMIIM_SP_SMART26KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
		    }

			break;
		}
		
		default:
		{
			key_table_ptr = MMIIM_SP_ML26KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);    //此表的顺序是按a~z排列
		    if (key_table_ptr == PNULL)
		    {
		        key_table_ptr = MMIIM_SP_SMART26KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
		    }

			break;
		}
	}

	if (key_table_ptr == PNULL)
	{
		SCI_TRACE_LOW ("[MMIIM]MMIIM_TOUCH_ResetVkbChars key_table_ptr");
		return -3;
	}
	
    for (iloop = 0; iloop < character_cnt; iloop++)
    {
        cur_char_ptr = key_table_ptr + (keymap_ptr[iloop] - VKEY_A);
        data_ptr->vkb.keys[data_ptr->vkb.charactor_first + iloop].text_len = MIN (cur_char_ptr->key_chars[0], TEXT_LEN_LIMIT - 1);
        SCI_MEMCPY(data_ptr->vkb.keys[data_ptr->vkb.charactor_first + iloop].text,
                   &cur_char_ptr->key_chars[1],
                   data_ptr->vkb.keys[data_ptr->vkb.charactor_first + iloop].text_len*sizeof(wchar));
		data_ptr->vkb.keys[data_ptr->vkb.charactor_first + iloop].is_hide = 0;		//先将不需要的按键全部隐藏
    }

	//将text字段数据同步到display_text字段，以备显示使用，免得再每个地方修改
	MMIIM_TOUCH_SynchronizeDisplayText (data_ptr, KEY_STYLE_CHARACTER);
	
	//重新绘制键盘
	if (data_ptr->f_draw_all == PNULL)
	{
		SCI_TRACE_LOW ("[MMIIM]MMIIM_TOUCH_ResetVkbChars data_ptr->f_draw_all");
		return -4;
	}
	
	data_ptr->f_draw_all (data_ptr);
	
    return 0;    
}



/*==============================================================================
Description: 因当前程序无法满足显示字符设置的情况，导致需要将按键码字符串和
			按键显示字符串分离，此处为了保持前期程序一致性，将按键码字符串
			同步到显示字符串，如果某些地方需要修改显示字符串，在其他函数中处理
Global resource dependence: NONE
Author: yaoguang.chen
Note: 1.后期程序维护时，可能将display_text字段空间变小，所以到时候的同步长度
		就会仅仅保留display_text能够处理的长度，防止内存越界
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_SynchronizeDisplayText (IM_DATA_T *data_ptr, KEY_STYLE_T key_style)
{
	uint32 iloop = 0;
	
	if (data_ptr == PNULL || data_ptr->vkb.keys == PNULL)
	{
		SCI_TRACE_LOW ("[MMIIM]MMIIM_TOUCH_SynchronizeDisplayText err param");
		return -1;
	}

	for (iloop = 0; iloop < data_ptr->vkb.key_cnt; iloop ++)
	{
		if (key_style == data_ptr->vkb.keys[iloop].style)
		{
			data_ptr->vkb.keys[iloop].print_text_len = MIN (data_ptr->vkb.keys[iloop].text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (data_ptr->vkb.keys[iloop].print_text, data_ptr->vkb.keys[iloop].text, data_ptr->vkb.keys[iloop].print_text_len * sizeof (wchar));
		}
	}
	
	return 0;
}



/*==============================================================================
Description: 因当前程序无法满足显示字符设置的情况，导致需要将按键码字符串和
			按键显示字符串分离，此处为了保持前期程序一致性，将按键码字符串
			同步到显示字符串，如果某些地方需要修改显示字符串，在其他函数中处理
Global resource dependence: NONE
Author: yaoguang.chen
Note: 1.后期程序维护时，可能将display_text字段空间变小，所以到时候的同步长度
		就会仅仅保留display_text能够处理的长度，防止内存越界
	  2.不能随便使用这个函数，否则会将所有的字符串同步了，而某些
	    不需要同步的会出问题	
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_SynchronizeAllDisplayText (IM_DATA_T *data_ptr)
{
	uint32 iloop = 0;
	
	if (data_ptr == PNULL || data_ptr->vkb.keys == PNULL)
	{
		SCI_TRACE_LOW ("[MMIIM]MMIIM_TOUCH_SynchronizeAllDisplayText err param");
		return -1;
	}

	for (iloop = 0; iloop < data_ptr->vkb.key_cnt; iloop ++)
	{
		data_ptr->vkb.keys[iloop].print_text_len = MIN (data_ptr->vkb.keys[iloop].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[iloop].print_text, data_ptr->vkb.keys[iloop].text, data_ptr->vkb.keys[iloop].print_text_len * sizeof (wchar));
	}
	
	return 0;
}



/*==============================================================================
Description: 某些语言，显示字符串和按键码不同，需要自定义显示的字符串，
			仅仅处理0~9等字符按键
Global resource dependence: NONE
Author: yaoguang.chen
Note: 
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_ThumbKbRedefinePrintText (IM_DATA_T *data_ptr)
{
	uint8 key_idx = 0, char_begin = 0;
	MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL;
	VKEYBOARD_T *vkb_ptr = PNULL;
	wchar *cur_char_ptr = PNULL;
	
	if (data_ptr == PNULL || data_ptr->vkb.keys == PNULL 
		|| data_ptr->init_param.method_param_ptr == PNULL
		|| data_ptr->init_param.method_param_ptr->im_def_ptr == PNULL)
	{
		SCI_TRACE_LOW ("[MMIIM]MMIIM_TOUCH_ThumbKbRedefinePrintText err param");
		return -1;
	}

	//该函数以及定义为9键，所有只能是 GUIIM_TYPE_SMART / GUIIM_TYPE_MULTITAP中的一种
	if (data_ptr->init_param.method_param_ptr->im_def_ptr->type == GUIIM_TYPE_SMART)
    {
	    key_table_ptr = MMIIM_SP_Smart9KEY_GetPrintKeyTable (data_ptr->init_param.method_param_ptr->im_def_ptr->method);
	    if (key_table_ptr == PNULL)
	    {
	        key_table_ptr = MMIIM_SP_ML9KEY_GetPrintKeyTable (data_ptr->init_param.method_param_ptr->im_def_ptr->method);
	    }
	}
	else if (data_ptr->init_param.method_param_ptr->im_def_ptr->type == GUIIM_TYPE_MULTITAP)
	{
	    key_table_ptr = MMIIM_SP_ML9KEY_GetPrintKeyTable (data_ptr->init_param.method_param_ptr->im_def_ptr->method);
	    if (key_table_ptr == PNULL)
	    {
	        key_table_ptr = MMIIM_SP_Smart9KEY_GetPrintKeyTable (data_ptr->init_param.method_param_ptr->im_def_ptr->method);
	    }
	}
	else
	{
		SCI_TRACE_LOW ("MMIIM_TOUCH_ThumbKbRedefinePrintText Big Err");
		return -1;
	}

	key_idx = char_begin = data_ptr->vkb.charactor_first;
	vkb_ptr = &data_ptr->vkb;
    for (; key_idx < char_begin + 10; key_idx++)		//1~9, 0
    {
        cur_char_ptr = (wchar *)key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].print_text_len = MIN (cur_char_ptr[0], PRINT_TEXT_LEN_LIMIT - 1);      //字符个数
        SCI_MEMCPY(vkb_ptr->keys[key_idx].print_text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].print_text_len * sizeof(wchar));
    }
	
	return 0;
}

#endif


/*Edit by script, ignore 1 case. Fri Apr 27 09:37:17 2012*/ //IGNORE9527
