/*****************************************************************************
** File Name:      mmidisplay_color.h                                        *
** Author:                                                                   *
** Date:           02/20/2020                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define color macro                   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2020      fangfang.yao       Create                                    *
******************************************************************************/

#ifndef _MMIDISPLAY_COLOR_H_
#define _MMIDISPLAY_COLOR_H_
#include "guicommon.h"
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

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
#define MMI_WHITE_COLOR		        0xffff
#define MMI_BLACK_COLOR		        0x0000
#define MMI_GRAY_WHITE_COLOR        0xef9d
#define MMI_DEFAULT_TEXT_COLOR      MMI_GRAY_WHITE_COLOR     //内屏幕中所有的字体颜色,黑色
#define MMI_TRANSPARENCE_COLOR      MMI_BLACK_COLOR     //所有界面中透明色定义为0x0 
#ifdef MMI_PDA_SUPPORT
#define MMI_WINDOW_BACKGROUND_COLOR	MMI_WHITE_COLOR              //窗口的背景色@phoebe
#else
#define MMI_WINDOW_BACKGROUND_COLOR	0x0080              //窗口的背景色@phoebe
#endif
#define MMI_BACKGROUND_COLOR        0x0040              //应用通用的背景色，接近黑色
#define WAIT_TEXT_BACKGROUND_COLOR  0x955d  //特殊界面中文本背景的颜色

#define MMI_DARK_BLUE_COLOR		    0x001f
#define MMI_BLUE_COLOR			    0x07ff
#define MMI_DARK_GREEN_COLOR		0x03e0
#define MMI_DARK_CYAN_COLOR		    0xf81f
#define MMI_GREEN_COLOR		        0x07e0
#define MMI_CYAN_COLOR			    0xfb81	      
#define MMI_DARK_RED_COLOR		    0xf800
#define MMI_DARK_MAGENTA_COLOR		0x0081
#define MMI_DARK_YELLOW_COLOR		0x0090
#define MMI_DARK_GRAY_COLOR		    0x8410
#define MMI_GRAY_COLOR			    0xCD7C
#define MMI_RED_COLOR			    0x8000
#define MMI_MAGENTA_COLOR		    0x00e3
#define MMI_YELLOW_COLOR		    0xfff0
#define MMI_PURPLE_COLOR		    0x81fb
#define MMI_WEAK_DARK_COLOR		    0x0800
#define MMI_EDITBOX_COLOR           0x1258
#define MMI_TEXTBOX_RECT_COLOR      0x1258
#define MMI_PROMPT_SK_BG_COLOR      0xa63f
#define MMI_IS_SELECTED_COLOR       0x6E7E
#define MMI_OSD_TRANSPARENT_COLOR	0Xf81f
#define MMI_OSD_DEFAULT_ALPHA       0x00ff              //覆盖下层图片
#define MMI_DARK_WHITE_COLOR        0xb5b6
#define MMI_DIAL_GRAY_COLOR         0xcccc
#define MMI_DEFAULT_TITLE_BG_COLOR	0x1258              //default title color
#define	MMI_SPELL_BACK_COLOR		0x9e7f
#define	MMI_T9_FONT_COLOR			0x193
#define MMI_TAB_DEFAULT_FOCUS_COLOR 0x397
#define MMI_LIGHT_BLUE_COLOR		0x7e3d
#define MMI_HIGH_LIGHT_COLOR		0xff0f
#define MMI_DARK_GRAY2_COLOR		GUI_RGB2RGB565(0x24,0x24,0x24)

#define MMI_MASK_ALPHA              165                 //蒙层的透明度，65%透明;

#ifdef ADULT_WATCH_SUPPORT

#define AUDULT_BLUE_COLOR GUI_RGB8882RGB565(0X2196F3)
#define AUDULT_BLUE1_COLOR GUI_RGB8882RGB565(0XBCD4)
#define AUDULT_RED_COLOR GUI_RGB8882RGB565(0XF44336)
#define AUDULT_RED1_COLOR GUI_RGB8882RGB565(0XE91E63)
#define AUDULT_GREEN_COLOR GUI_RGB8882RGB565(0X4CAF50)
#define AUDULT_GREEN1_COLOR GUI_RGB8882RGB565(0X9688)
#define AUDULT_YELLOW_COLOR GUI_RGB8882RGB565(0XFFC107)
#define AUDULT_ORANGE_COLOR GUI_RGB8882RGB565(0XFF9800)
#define AUDULT_VLOTE_COLOR GUI_RGB8882RGB565(0X3F51B5)
#define AUDULT_VLOTE1_COLOR GUI_RGB8882RGB565(0X9C27B0)
#define AUDULT_GREEN_COLOR GUI_RGB8882RGB565(0X8BC34A)
#define AUDULT_BROWN_COLOR GUI_RGB8882RGB565(0X795548)
#define AUDULT_BLACK_COLOR GUI_RGB8882RGB565(0X000000)
#define AUDULT_BLACK1_COLOR GUI_RGB8882RGB565(0X282828)
#define AUDULT_GRAY_COLOR GUI_RGB8882RGB565(0X505050)
#define AUDULT_GRAY1_COLOR GUI_RGB8882RGB565(0X282828)
#define AUDULT_GRAY2_COLOR GUI_RGB8882RGB565(0X999999)

#endif
// 列表控件Item的薄雾效果颜色
#define MMI_LIST_ITEM_MIST_COLOR    0x6e7e    //被提示的图片的alpha blending颜色


//==========================================================================//
//                            For watch                                     //
//==========================================================================//
#define WATCH_COMMONWIN_FONT_COLOR  MMI_WHITE_COLOR
//==========================================================================//
//                            For watch                                     //
//==========================================================================//

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
