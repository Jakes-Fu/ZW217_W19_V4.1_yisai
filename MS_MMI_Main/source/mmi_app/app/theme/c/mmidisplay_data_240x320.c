/*****************************************************************************
** File Name:      mmidisplay_data_240x320.c                                 *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#define _MMIDISPLAY_DATA_240x320_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmi_image.h"
#include "mmi_custom_define.h"
#include "mmi_signal_ext.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMI_IDLE_COMON_MAIN_LCD_X           10
#ifdef DCD_SUPPORT
    #define MMI_IDLE_COMON_MAIN_LCD_Y       45
#else
    #define MMI_IDLE_COMON_MAIN_LCD_Y       63 //40
#endif
#define MMI_IDLE_COMON_INTREVAL             5
#define MMI_IDLE_COMON_ITEM_FONT_H          16
#define MMI_IDLE_COMMO_ITEM_FONT            SONG_FONT_16
#define MMI_IDLE_TIME_X                     4
#define MMI_IDLE_TIME_Y                     27
#define MMI_IDLE_DATE_Y                     26
#define MMI_IDLE_WEEK_Y                     42
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//IDLE界面显示的文字的坐标，颜色，大小
LOCAL const MMI_IDLE_DISPLAY_T  mmi_idle_display[TOTAL_IDLE_DISPLAY_NUMBER] =
{   
	//待机MAIN LCD运营商文字 	
	{MMI_IDLE_COMON_MAIN_LCD_X, MMI_IDLE_COMON_MAIN_LCD_Y,IDLE_ALIGN_LEFT, MMI_IDLE_COMMO_ITEM_FONT},

#ifndef MMI_MULTI_SIM_SYS_SINGLE
	//待机MAIN LCD SIM2 Service Providor Name 
	{MMI_IDLE_COMON_MAIN_LCD_X,MMI_IDLE_COMON_MAIN_LCD_Y+MMI_IDLE_COMON_ITEM_FONT_H+MMI_IDLE_COMON_INTREVAL,
    IDLE_ALIGN_LEFT,MMI_IDLE_COMMO_ITEM_FONT},
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
	//待机MAIN LCD SIM3 Service Providor Name 
	{MMI_IDLE_COMON_MAIN_LCD_X,MMI_IDLE_COMON_MAIN_LCD_Y+(MMI_IDLE_COMON_ITEM_FONT_H+MMI_IDLE_COMON_INTREVAL)*2,
    IDLE_ALIGN_LEFT,MMI_IDLE_COMMO_ITEM_FONT},
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
	//待机MAIN LCD SIM4 Service Providor Name 
	{MMI_IDLE_COMON_MAIN_LCD_X,MMI_IDLE_COMON_MAIN_LCD_Y+(MMI_IDLE_COMON_ITEM_FONT_H+MMI_IDLE_COMON_INTREVAL)*3,
    IDLE_ALIGN_LEFT,MMI_IDLE_COMMO_ITEM_FONT},
#endif
#endif
    //待机MAIN LCD自定义词语 
	{MMI_IDLE_COMON_MAIN_LCD_X,MMI_IDLE_COMON_MAIN_LCD_Y+(MMI_IDLE_COMON_ITEM_FONT_H+MMI_IDLE_COMON_INTREVAL)*(MMI_DUAL_SYS_MAX),
    IDLE_ALIGN_LEFT,MMI_IDLE_COMMO_ITEM_FONT,},

	//待机MAIN LCD倒记时显示
	{MMI_IDLE_COMON_MAIN_LCD_X,MMI_IDLE_COMON_MAIN_LCD_Y+(MMI_IDLE_COMON_ITEM_FONT_H+MMI_IDLE_COMON_INTREVAL)*(MMI_DUAL_SYS_MAX +1)+2,
     IDLE_ALIGN_LEFT,MMI_IDLE_COMMO_ITEM_FONT,},

	//待机MAIN LCD日历显示
	{MMI_IDLE_COMON_MAIN_LCD_X,MMI_IDLE_COMON_MAIN_LCD_Y+(MMI_IDLE_COMON_ITEM_FONT_H+MMI_IDLE_COMON_INTREVAL)*(MMI_DUAL_SYS_MAX +2),
     IDLE_ALIGN_LEFT,MMI_IDLE_COMMO_ITEM_FONT,},
        
	//MIAN  LCD闹钟到了的时间文本显示
// 	{0, 87,IDLE_ALIGN_X_CENTER,
//     MMI_DEFAULT_TEXT_FONT},

//待机MAIN LCD MP3背景播放mp3 name
#ifdef MMI_MULTI_SIM_SYS_QUAD
	{MMI_IDLE_COMON_MAIN_LCD_X,MMI_IDLE_COMON_MAIN_LCD_Y+(MMI_IDLE_COMON_ITEM_FONT_H+MMI_IDLE_COMON_INTREVAL)*(MMI_DUAL_SYS_MAX + 3),
     IDLE_ALIGN_LEFT,MMI_IDLE_COMMO_ITEM_FONT,},
 	{MMI_IDLE_COMON_MAIN_LCD_X,MMI_IDLE_COMON_MAIN_LCD_Y+(MMI_IDLE_COMON_ITEM_FONT_H+MMI_IDLE_COMON_INTREVAL)*(MMI_DUAL_SYS_MAX + 4),
     IDLE_ALIGN_LEFT,MMI_IDLE_COMMO_ITEM_FONT,},
#else
	{MMI_IDLE_COMON_MAIN_LCD_X,MMI_IDLE_COMON_MAIN_LCD_Y+(MMI_IDLE_COMON_ITEM_FONT_H+MMI_IDLE_COMON_INTREVAL)*(MMI_DUAL_SYS_MAX + 3),
     IDLE_ALIGN_LEFT,MMI_IDLE_COMMO_ITEM_FONT,},

	//待机MAIN LCD FM背景播放FM name
	{MMI_IDLE_COMON_MAIN_LCD_X,MMI_IDLE_COMON_MAIN_LCD_Y+(MMI_IDLE_COMON_ITEM_FONT_H+MMI_IDLE_COMON_INTREVAL)*(MMI_DUAL_SYS_MAX + 4),
     IDLE_ALIGN_LEFT,MMI_IDLE_COMMO_ITEM_FONT,},
#endif
	//待机MAIN LCD时间
	{MMI_IDLE_TIME_X,MMI_IDLE_TIME_Y, IDLE_ALIGN_LEFT,MMI_IDLE_COMMO_ITEM_FONT}, 
	
	//待机MAIN LCD日期
	{MMI_MAINSCREEN_RIGHT_MAX_PIXEL- 6,MMI_IDLE_DATE_Y,IDLE_ALIGN_RIGHT,SONG_FONT_14},
	
    //待机MAIN LCD星期 
	{MMI_MAINSCREEN_RIGHT_MAX_PIXEL- 6,MMI_IDLE_WEEK_Y,IDLE_ALIGN_RIGHT,SONG_FONT_14},

    //待机SUB LCD时间
	{0,28,
	    IDLE_ALIGN_X_CENTER,SONG_FONT_29},

	//待机SUB LCD日期  
	{0,65,
        IDLE_ALIGN_X_CENTER, MMI_DEFAULT_TEXT_FONT},

	//待机SUB LCD星期
	{0,50,
	    IDLE_ALIGN_X_CENTER,
        MMI_DEFAULT_TEXT_FONT},

	//SUB LCD闹钟到了的时间文本显示
	{0,50, 
	    IDLE_ALIGN_X_CENTER, MMI_DEFAULT_TEXT_FONT},
    //SUB LCD倒记时显示,	
	{0,50, 
	    IDLE_ALIGN_X_CENTER,MMI_DEFAULT_NORMAL_FONT},
    //STK set up idle mode 显示的文本
	{MMI_MAINSCREEN_IDLE_STK_LEFT, 180,
	    IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT}
};

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/






















