/*****************************************************************************
** File Name:      mmiim_tp_cstar_position.h                                 *
** Author:                                                                   *
** Date:           2009/09/16                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used define tp cstar position                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/9/16     rui.xu        Create                                        *
******************************************************************************/


#ifndef _MMIIM_TP_CSTAR_POSITION_H_
#define _MMIIM_TP_CSTAR_POSITION_H_

#include "mmidisplay_data.h"

#if defined (MAINLCD_SIZE_320X480)
#define MMIIM_TP_CSTAR_CAND_MORGEN              4


//BAR 相关
#define MMIIM_TP_CSTAR_TEXT_BAR_FONT_TEXT       (SONG_FONT_20)
#define MMIIM_TP_CSTAR_TEXT_BAR_FONT_LABEL      (SONG_FONT_10)
#define MMIIM_TP_CSTAR_TEXT_BAR_COLOR_TEXT      (MMI_BACKGROUND_COLOR)
#define MMIIM_TP_CSTAR_TEXT_BAR_COLOR_LABEL     (MMI_BACKGROUND_COLOR)
#define MMIIM_TP_CSTAR_TEXT_BAR_COLOR_HOT       (0x7EFF)

#if defined (MMI_IM_PDA_SUPPORT)
#define MMIIM_TP_CSTAR_TEXT_TIPS_COLOR_HOT      (0x7EFF)
#else
#define MMIIM_TP_CSTAR_TEXT_TIPS_COLOR_HOT      (0x0000)
#endif

#define MMIIM_TP_CSTAR_TEXT_BAR_SPACE           (5)

#define MMIIM_TP_CSTAR_CHOICE_BAR_FONT_TEXT     (SONG_FONT_16)
#define MMIIM_TP_CSTAR_CHOICE_BAR_SPACE_HEIGHT  12
#define MMIIM_TP_CSTAR_CHOICE_BAR_COLOR_TEXT    (0xFFFF)
#define MMIIM_TP_CSTAR_CHOICE_BAR_COLOR_HOT     (0x7EFF)
#define MMIIM_TP_CSTAR_CHOICE_UP_LEFT_X_V       1           //表示choice bar上箭头，离原图顶点的左边偏移
#define MMIIM_TP_CSTAR_CHOICE_UP_TOP_X_V        3
#define MMIIM_TP_CSTAR_CHOICE_DOWN_BOTTOM_X_V   1

#define MMIIM_TP_CSTAR_CAND_UP_LEFT_X_V         231
#define MMIIM_TP_CSTAR_CAND_DOWN_LEFT_X_V       275
#define MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_V       3
#define MMIIM_TP_CSTAR_CAND_IDC_TOP_Y_V         3

#define MMIIM_TP_CSTAR_CAND_UP_LEFT_X_H         391
#define MMIIM_TP_CSTAR_CAND_DOWN_LEFT_X_H       435
#define MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_H       3
#define MMIIM_TP_CSTAR_CAND_IDC_TOP_Y_H         1

#define KEY_HOR_COL_V                           2           //两个按键之间的水平间隔
#define KEY_VER_COL_V                           1

#define KEY_HOR_COL_H                           2           //两个按键之间的水平间隔
#define KEY_VER_COL_H                           1

#define TOP_SPACE_WIDTH_V                       1           //左上角与第一个键之间的空白区域，用于求每个按键的宽，高
#define TOP_SPACE_HEIGHT_V                      3
#define BOTTOM_SPACE_WIDTH_V                    1           //右下角与最后一个键的空白区域
#define BOTTOM_SPACE_HEIGHT_V                   0

#define TOP_SPACE_WIDTH_H                       1           //左上角与第一个键之间的空白区域，用于求每个按键的宽，高
#define TOP_SPACE_HEIGHT_H                      3
#define BOTTOM_SPACE_WIDTH_H                    1           //右下角与最后一个键的空白区域
#define BOTTOM_SPACE_HEIGHT_H                   0

#define MMIIM_TP_KEY_DISPLAY_FONT_V             SONG_FONT_24
#define MMIIM_TP_KEY_DISPLAY_FONT_H             SONG_FONT_18
#define MMIIM_TP_KEY_DISPLAY_FONT_COLOR_V       (0xFFFF)
#define MMIIM_TP_KEY_DISPLAY_FONT_COLOR_H       (0xFFFF)

#elif defined (MAINLCD_SIZE_176X220)
#define MMIIM_TP_CSTAR_CAND_MORGEN              4

//BAR 相关
#define MMIIM_TP_CSTAR_TEXT_BAR_FONT_TEXT       (SONG_FONT_16)
#define MMIIM_TP_CSTAR_TEXT_BAR_FONT_LABEL      (SONG_FONT_10)
#define MMIIM_TP_CSTAR_TEXT_BAR_COLOR_TEXT      (MMI_BACKGROUND_COLOR)
#define MMIIM_TP_CSTAR_TEXT_BAR_COLOR_LABEL     (MMI_BACKGROUND_COLOR)
#define MMIIM_TP_CSTAR_TEXT_BAR_COLOR_HOT       (0x7EFF)
#define MMIIM_TP_CSTAR_TEXT_TIPS_COLOR_HOT      (0xFFFF)
#define MMIIM_TP_CSTAR_TEXT_BAR_SPACE           (5)

#define MMIIM_TP_CSTAR_CHOICE_BAR_FONT_TEXT     (SONG_FONT_14)
#define MMIIM_TP_CSTAR_CHOICE_BAR_SPACE_HEIGHT  2
#define MMIIM_TP_CSTAR_CHOICE_BAR_COLOR_TEXT    (MMI_BACKGROUND_COLOR)
#define MMIIM_TP_CSTAR_CHOICE_BAR_COLOR_HOT     (0x7EFF)
#define MMIIM_TP_CSTAR_CHOICE_UP_LEFT_X_V       2           //表示choice bar上箭头，离原图顶点的左边偏移
#define MMIIM_TP_CSTAR_CHOICE_UP_TOP_X_V        2
#define MMIIM_TP_CSTAR_CHOICE_DOWN_BOTTOM_X_V   1

#define MMIIM_TP_CSTAR_CAND_UP_LEFT_X_V         141
#define MMIIM_TP_CSTAR_CAND_DOWN_LEFT_X_V       158
#define MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_V       1
#define MMIIM_TP_CSTAR_CAND_IDC_TOP_Y_V         2

#define MMIIM_TP_CSTAR_CAND_UP_LEFT_X_H         185
#define MMIIM_TP_CSTAR_CAND_DOWN_LEFT_X_H       202
#define MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_H       1
#define MMIIM_TP_CSTAR_CAND_IDC_TOP_Y_H         2

#define KEY_HOR_COL_V                           1           //两个按键之间的水平间隔
#define KEY_VER_COL_V                           1

#define KEY_HOR_COL_H                           1           //两个按键之间的水平间隔
#define KEY_VER_COL_H                           1

#define TOP_SPACE_WIDTH_V                       4           //左上角与第一个键之间的空白区域，用于求每个按键的宽，高
#define TOP_SPACE_HEIGHT_V                      3
#define BOTTOM_SPACE_WIDTH_V                    3           //右下角与最后一个键的空白区域
#define BOTTOM_SPACE_HEIGHT_V                   1

#define TOP_SPACE_WIDTH_H                       6           //左上角与第一个键之间的空白区域，用于求每个按键的宽，高
#define TOP_SPACE_HEIGHT_H                      3
#define BOTTOM_SPACE_WIDTH_H                    5           //右下角与最后一个键的空白区域
#define BOTTOM_SPACE_HEIGHT_H                   1

#define MMIIM_TP_KEY_DISPLAY_FONT_V             SONG_FONT_12
#define MMIIM_TP_KEY_DISPLAY_FONT_H             SONG_FONT_12
#define MMIIM_TP_KEY_DISPLAY_FONT_COLOR_V       (0x1169)
#define MMIIM_TP_KEY_DISPLAY_FONT_COLOR_H       (0x1169)

#elif defined (MAINLCD_SIZE_240X400)                        //注意保持240X400与240X320的竖屏数据是一致的

#define MMIIM_TP_CSTAR_CAND_MORGEN              4

//BAR 相关
#define MMIIM_TP_CSTAR_TEXT_BAR_FONT_TEXT       (SONG_FONT_20)
#define MMIIM_TP_CSTAR_TEXT_BAR_FONT_LABEL      (SONG_FONT_10)
#define MMIIM_TP_CSTAR_TEXT_BAR_COLOR_TEXT      (MMI_BACKGROUND_COLOR)
#define MMIIM_TP_CSTAR_TEXT_BAR_COLOR_LABEL     (MMI_BACKGROUND_COLOR)
#define MMIIM_TP_CSTAR_TEXT_BAR_COLOR_HOT       (0x7EFF)
#define MMIIM_TP_CSTAR_TEXT_TIPS_COLOR_HOT      (0xFFFF)
#define MMIIM_TP_CSTAR_TEXT_BAR_SPACE           (5)

#define MMIIM_TP_CSTAR_CHOICE_BAR_FONT_TEXT     (SONG_FONT_14)
#define MMIIM_TP_CSTAR_CHOICE_BAR_SPACE_HEIGHT  2
#define MMIIM_TP_CSTAR_CHOICE_BAR_COLOR_TEXT    (0xFFFF)
#define MMIIM_TP_CSTAR_CHOICE_BAR_COLOR_HOT     (0x7EFF)
#define MMIIM_TP_CSTAR_CHOICE_UP_LEFT_X_V       1           //表示choice bar上箭头，离原图顶点的左边偏移
#define MMIIM_TP_CSTAR_CHOICE_UP_TOP_X_V        4
#define MMIIM_TP_CSTAR_CHOICE_DOWN_BOTTOM_X_V   3

#define MMIIM_TP_CSTAR_CAND_UP_LEFT_X_V         191
#define MMIIM_TP_CSTAR_CAND_DOWN_LEFT_X_V       215
#define MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_V       2
#define MMIIM_TP_CSTAR_CAND_IDC_TOP_Y_V         1

#define MMIIM_TP_CSTAR_CAND_UP_LEFT_X_H         347
#define MMIIM_TP_CSTAR_CAND_DOWN_LEFT_X_H       372
#define MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_H       2
#define MMIIM_TP_CSTAR_CAND_IDC_TOP_Y_H         1

#define KEY_HOR_COL_V                           2           //两个按键之间的水平间隔
#define KEY_VER_COL_V                           2

#define KEY_HOR_COL_H                           4           //两个按键之间的水平间隔
#define KEY_VER_COL_H                           1

#define TOP_SPACE_WIDTH_V                       6           //左上角与第一个键之间的空白区域，用于求每个按键的宽，高
#define TOP_SPACE_HEIGHT_V                      3
#define BOTTOM_SPACE_WIDTH_V                    6           //右下角与最后一个键的空白区域
#define BOTTOM_SPACE_HEIGHT_V                   2

#define TOP_SPACE_WIDTH_H                       22           //左上角与第一个键之间的空白区域，用于求每个按键的宽，高
#define TOP_SPACE_HEIGHT_H                      3
#define BOTTOM_SPACE_WIDTH_H                    22           //右下角与最后一个键的空白区域
#define BOTTOM_SPACE_HEIGHT_H                   1

#define MMIIM_TP_KEY_DISPLAY_FONT_V             SONG_FONT_16
#define MMIIM_TP_KEY_DISPLAY_FONT_H             SONG_FONT_14
#define MMIIM_TP_KEY_DISPLAY_FONT_COLOR_V       (0x4AAE)
#define MMIIM_TP_KEY_DISPLAY_FONT_COLOR_H       (0x4AAE)

#else //240*320 and other unknown

#define MMIIM_TP_CSTAR_CAND_MORGEN              4

//BAR 相关
#define MMIIM_TP_CSTAR_TEXT_BAR_FONT_TEXT       (SONG_FONT_20)
#define MMIIM_TP_CSTAR_TEXT_BAR_FONT_LABEL      (SONG_FONT_10)
#define MMIIM_TP_CSTAR_TEXT_BAR_COLOR_TEXT      (MMI_BACKGROUND_COLOR)
#define MMIIM_TP_CSTAR_TEXT_BAR_COLOR_LABEL     (MMI_BACKGROUND_COLOR)
#define MMIIM_TP_CSTAR_TEXT_BAR_COLOR_HOT       (0x7EFF)
#define MMIIM_TP_CSTAR_TEXT_TIPS_COLOR_HOT      (0xFFFF)
#define MMIIM_TP_CSTAR_TEXT_BAR_SPACE           (5)

#define MMIIM_TP_CSTAR_CHOICE_BAR_FONT_TEXT     (SONG_FONT_14)
#define MMIIM_TP_CSTAR_CHOICE_BAR_SPACE_HEIGHT  2
#define MMIIM_TP_CSTAR_CHOICE_BAR_COLOR_TEXT    (0xFFFF)
#define MMIIM_TP_CSTAR_CHOICE_BAR_COLOR_HOT     (0x7EFF)
#define MMIIM_TP_CSTAR_CHOICE_UP_LEFT_X_V       1           //表示choice bar上箭头，离原图顶点的左边偏移
#define MMIIM_TP_CSTAR_CHOICE_UP_TOP_X_V        3
#define MMIIM_TP_CSTAR_CHOICE_DOWN_BOTTOM_X_V   1

#define MMIIM_TP_CSTAR_CAND_UP_LEFT_X_V         174
#define MMIIM_TP_CSTAR_CAND_DOWN_LEFT_X_V       206
#define MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_V       3
#define MMIIM_TP_CSTAR_CAND_IDC_TOP_Y_V         3

#define MMIIM_TP_CSTAR_CAND_UP_LEFT_X_H         254
#define MMIIM_TP_CSTAR_CAND_DOWN_LEFT_X_H       286
#define MMIIM_TP_CSTAR_CAND_IDC_SPACE_X_H       3
#define MMIIM_TP_CSTAR_CAND_IDC_TOP_Y_H         2

#define KEY_HOR_COL_V	                        2           //两个按键之间的水平间隔
#define KEY_VER_COL_V                           2

#define KEY_HOR_COL_H                           3           //两个按键之间的水平间隔
#define KEY_VER_COL_H                           2

#define TOP_SPACE_WIDTH_V                       1           //左上角与第一个键之间的空白区域，用于求每个按键的宽，高
#define TOP_SPACE_HEIGHT_V                      3
#define BOTTOM_SPACE_WIDTH_V                    1           //右下角与最后一个键的空白区域
#define BOTTOM_SPACE_HEIGHT_V                   0

#define TOP_SPACE_WIDTH_H                       7           //左上角与第一个键之间的空白区域，用于求每个按键的宽，高
#define TOP_SPACE_HEIGHT_H                      3
#define BOTTOM_SPACE_WIDTH_H                    6           //右下角与最后一个键的空白区域
#define BOTTOM_SPACE_HEIGHT_H                   1

#define MMIIM_TP_KEY_DISPLAY_FONT_V             SONG_FONT_18
#define MMIIM_TP_KEY_DISPLAY_FONT_H             SONG_FONT_14
#define MMIIM_TP_KEY_DISPLAY_FONT_COLOR_V       (MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT))
#define MMIIM_TP_KEY_DISPLAY_FONT_COLOR_H       (MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT))


#endif


#define MMIIM_TP_CSTAR_DIG_VNUM                 3
#define MMIIM_TP_CSTAR_DIG_HNUM                 5

#define MMIIM_TP_CSTAR_DIGSYM_VNUM              3
#define MMIIM_TP_CSTAR_DIGSYM_HNUM              5

#define MMIIM_TP_CSTAR_ABC_VNUM                 4
#define MMIIM_TP_CSTAR_ABC_HNUM                 10

#define MMIIM_TP_CSTAR_SYM_VNUM                 5
#define MMIIM_TP_CSTAR_SYM_HNUM                 11

#define MMIIM_TP_CSTAR_PY_VNUM                  4
#define MMIIM_TP_CSTAR_PY_HNUM                  10

#define MMIIM_TP_CSTAR_STROKE_VNUM              3
#define MMIIM_TP_CSTAR_STROKE_HNUM              4

#define MMIIM_TP_CSTAR_9KEY_ABC_VNUM            4
#define MMIIM_TP_CSTAR_9KEY_ABC_HNUM            5

//虚拟键盘采用字符显示方式，因键盘背景图不同，需要配置字体颜色
#if defined (MAINLCD_SIZE_320X480)
#define IMUI_KB_DEFAULT_NORMAL_TEXT_COLOR   (0xffff)        //正常显示颜色
#define IMUI_KB_DEFAULT_HIGHLIGHT_TEXT_COLOR    (0xffff)    //点击高亮显示颜色
#elif defined (MAINLCD_SIZE_176X220)
#define IMUI_KB_DEFAULT_NORMAL_TEXT_COLOR   (0x1169)
#define IMUI_KB_DEFAULT_HIGHLIGHT_TEXT_COLOR    (0x1169)
#elif defined (MAINLCD_SIZE_240X400)                        //注意保持240X400与240X320的竖屏数据是一致的
#define IMUI_KB_DEFAULT_NORMAL_TEXT_COLOR   (0x4AAE)
#define IMUI_KB_DEFAULT_HIGHLIGHT_TEXT_COLOR    (0x4AAE)
#else //240*320 and other unknown
#define IMUI_KB_DEFAULT_NORMAL_TEXT_COLOR   (0xffff)
#define IMUI_KB_DEFAULT_HIGHLIGHT_TEXT_COLOR    (0xffff)
#endif



#endif//_MMIIM_TP_CSTAR_POSITION_H_
