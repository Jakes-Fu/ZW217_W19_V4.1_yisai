/*****************************************************************************
** File Name:      mmiebook_position.h                                         *
** Author:                                                                   *
** Date:           29/09/2007                                                *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe ebook macro about displaying position                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2007     yongwei.he              Create
******************************************************************************/
#ifndef _MMIEBOOK_POSITION_H_
#define _MMIEBOOK_POSITION_H_

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#ifdef EBOOK_SUPPORT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//是否需要整合字体颜色，背景颜色，字体大小到一个显示设置菜单
#define MMIEBOOK_OSD_DISPLAY_SETTING  

//文本控件使用
#define MMIEBOOK_TXT_SHOW_TEXTBOX_LEFT                  0
#define MMIEBOOK_TXT_SHOW_TEXTBOX_RIGHT                 ( MMI_MAINSCREEN_WIDTH -1 )
#define MMIEBOOK_TXT_SHOW_TEXTBOX_TOP                   0
#define MMIEBOOK_TXT_SHOW_TEXTBOX_BOTTOM                (MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - MMIEBOOK_COUNT_BAR_HEIGHT)


#define MMIEBOOK_TXT_SHOW_TEXTBOX_LINE_SPACE            MMITHEME_TEXT_LINE_SPACE


#if defined( MMI_GUI_STYLE_MINISCREEN)  
#define OSD_TIME_LABEL_LEN                              45
#define OSD_PERCENT_LABEL_LEN                           45
#else
#define OSD_TIME_LABEL_LEN                              60
#define OSD_PERCENT_LABEL_LEN                           60
#endif


#if defined (MAINLCD_SIZE_128X160)
	#define MMIEBOOK_COUNT_BAR_HEIGHT                       (25+1)
	#define MMIEBOOK_COUNT_BAR_BTN_WIDTH                    25
#elif defined (MAINLCD_SIZE_176X220)
	#define MMIEBOOK_COUNT_BAR_HEIGHT                       (25+1)
	#define MMIEBOOK_COUNT_BAR_BTN_WIDTH                    25
#elif defined MAINLCD_SIZE_240X320
	#define MMIEBOOK_COUNT_BAR_HEIGHT                       31
	#define MMIEBOOK_COUNT_BAR_BTN_WIDTH                    35
#elif defined MAINLCD_SIZE_128X64
	#define MMIEBOOK_COUNT_BAR_HEIGHT                       31
	#define MMIEBOOK_COUNT_BAR_BTN_WIDTH                    35
#elif defined MAINLCD_SIZE_240X400
	#define MMIEBOOK_COUNT_BAR_HEIGHT                       31
	#define MMIEBOOK_COUNT_BAR_BTN_WIDTH                    35
#elif defined MAINLCD_SIZE_320X480
	#define MMIEBOOK_COUNT_BAR_HEIGHT                       31
	#define MMIEBOOK_COUNT_BAR_BTN_WIDTH                    35
#else
    #error	
#endif

#ifdef MAINLCD_SIZE_240X400
#define MMIEBOOK_TITLE_BOTTOM  53
#define MMIEBOOK_START_X  7
#define MMIEBOOK_START_Y  70
#define MMIEBOOK_NAME_START_X  18
#define MMIEBOOK_NAME_START_Y  134
#define MMIEBOOK_NAME_HEIGHT  14
#define MMIEBOOK_X_INTERVAL  75
#define MMIEBOOK_Y_INTERVAL  112
#define MMIEBOOK_WIDTH  74
#define MMIEBOOK_HEIGHT  92
#define MMIEBOOK_TITLE_RECT_V  {77,26, 162,54}
#define MMIEBOOK_TITLE_RECT_H  {156,26, 244,54}
#define MMIEBOOK_TITTLE_FONT SONG_FONT_16
#define MMIEBOOK_NAME_FONT SONG_FONT_12
#define MMIEBOOK_TITLE_BAR_HEIGHT 50

#ifdef MMIEBOOK_OSD_DISPLAY_SETTING
#define MMIEBOOK_OSD_LABEL_NUM   5
#else
#define MMIEBOOK_OSD_LABEL_NUM   7
#endif
#define MMIEBOOK_OSD_LABEL_HEIGHT_UP   32
#define MMIEBOOK_OSD_LABEL_HEIGHT_DOWN   34
#define MMIEBOOK_OSD_RADIX_V   5
#define MMIEBOOK_OSD_RADIX_H   6
#define MMIEBOOK_OSD_FONT      SONG_FONT_14
#define MMIEBOOK_OSD_FONT_COLOR      1
#define MMIEBOOK_COLOR_HOR  25
#define MMIEBOOK_COLOR_VER  14
#define MMIEBOOK_COLOR_WIDTH 44
#define MMIEBOOK_COLOR_HEIGHT 44
#elif defined MAINLCD_SIZE_240X320
#define MMIEBOOK_TITLE_BOTTOM  55//
#define MMIEBOOK_START_X  17//
#define MMIEBOOK_START_Y  64//
#define MMIEBOOK_NAME_START_X  26//
#define MMIEBOOK_NAME_START_Y  110//
#define MMIEBOOK_NAME_HEIGHT  12//
#define MMIEBOOK_X_INTERVAL  71
#define MMIEBOOK_Y_INTERVAL  88
#define MMIEBOOK_WIDTH  62//
#define MMIEBOOK_HEIGHT  69//
#define MMIEBOOK_TITLE_RECT_V  {77,27, 162,51}//
#define MMIEBOOK_TITLE_RECT_H  {116,26, 204,54}
#define MMIEBOOK_TITTLE_FONT SONG_FONT_16
#define MMIEBOOK_TITLE_BAR_HEIGHT 42

#define MMIEBOOK_NAME_FONT SONG_FONT_11//

#ifdef MMIEBOOK_OSD_DISPLAY_SETTING
#define MMIEBOOK_OSD_LABEL_NUM   5
#else
#define MMIEBOOK_OSD_LABEL_NUM   7
#endif
#define MMIEBOOK_OSD_LABEL_HEIGHT_UP   32
#define MMIEBOOK_OSD_LABEL_HEIGHT_DOWN   34
#define MMIEBOOK_OSD_RADIX_V   5
#define MMIEBOOK_OSD_RADIX_H   6
#define MMIEBOOK_OSD_FONT      SONG_FONT_14
#define MMIEBOOK_OSD_FONT_COLOR      1
#define MMIEBOOK_COLOR_HOR  25
#define MMIEBOOK_COLOR_VER  14
#define MMIEBOOK_COLOR_WIDTH 44
#define MMIEBOOK_COLOR_HEIGHT 44
#else
#define MMIEBOOK_TITLE_BOTTOM  77
#define MMIEBOOK_START_X  19
#define MMIEBOOK_START_Y  94
#define MMIEBOOK_NAME_START_X  34
#define MMIEBOOK_NAME_START_Y  166
#define MMIEBOOK_NAME_HEIGHT  14
#define MMIEBOOK_X_INTERVAL  99
#define MMIEBOOK_Y_INTERVAL  134
#define MMIEBOOK_WIDTH  84
#define MMIEBOOK_HEIGHT  100
#define MMIEBOOK_TITLE_RECT_V  {118,37, 202,63}
#define MMIEBOOK_TITLE_RECT_H  {195,26, 285,54}
#define MMIEBOOK_TITTLE_FONT SONG_FONT_16
#define MMIEBOOK_NAME_FONT SONG_FONT_12
#define MMIEBOOK_TITLE_BAR_HEIGHT 56

#ifdef MMIEBOOK_OSD_DISPLAY_SETTING
#define MMIEBOOK_OSD_LABEL_NUM   5
#else
#define MMIEBOOK_OSD_LABEL_NUM   7
#endif
#if defined (MAINLCD_SIZE_128X160) 
#define MMIEBOOK_OSD_LABEL_HEIGHT_UP   20
#elif defined (MAINLCD_SIZE_176X220)  
#define MMIEBOOK_OSD_LABEL_HEIGHT_UP   24 
#else
#define MMIEBOOK_OSD_LABEL_HEIGHT_UP   32
#endif
#if defined (MAINLCD_SIZE_176X220)  
#define MMIEBOOK_OSD_LABEL_HEIGHT_DOWN   28
#else
#define MMIEBOOK_OSD_LABEL_HEIGHT_DOWN   38
#endif
#define MMIEBOOK_OSD_RADIX_V   5
#define MMIEBOOK_OSD_RADIX_H   6
#define MMIEBOOK_OSD_FONT      SONG_FONT_14
#define MMIEBOOK_OSD_FONT_COLOR      1
#if defined (MAINLCD_SIZE_176X220)  
#define MMIEBOOK_COLOR_HOR  15
#elif defined (MAINLCD_SIZE_128X160) 
#define MMIEBOOK_COLOR_HOR  15
#else
#define MMIEBOOK_COLOR_HOR  30
#endif
#define MMIEBOOK_COLOR_VER  14
#if defined (MAINLCD_SIZE_176X220)  
#define MMIEBOOK_COLOR_WIDTH 40
#define MMIEBOOK_COLOR_HEIGHT 40
#elif defined (MAINLCD_SIZE_128X160)  
#define MMIEBOOK_COLOR_WIDTH 30
#define MMIEBOOK_COLOR_HEIGHT 30
#else
#define MMIEBOOK_COLOR_WIDTH 54
#define MMIEBOOK_COLOR_HEIGHT 54
#endif

#endif
#endif//ENG_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
