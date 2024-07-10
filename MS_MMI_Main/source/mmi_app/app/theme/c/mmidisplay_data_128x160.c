/*****************************************************************************
** File Name:      mmidisplay_data_176x220.c                                 *
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

#define _MMIDISPLAY_DATA_128x160_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmi_image.h"
#include "mmi_signal_ext.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
#define MMI_IDLE_COMON_INTREVAL             5 //juan.wu cr255113
//IDLE界面显示的文字的坐标，颜色，大小
LOCAL const MMI_IDLE_DISPLAY_T  mmi_idle_display[TOTAL_IDLE_DISPLAY_NUMBER] =
{                  
	//待机MAIN LCD运营商文字 	
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DEFAULT_FONT_HEIGHT + MMI_IDLE_COMON_INTREVAL,IDLE_ALIGN_X_CENTER, MMI_DEFAULT_TEXT_FONT},

#ifndef MMI_MULTI_SIM_SYS_SINGLE
	//待机MAIN LCD SIM2 Service Providor Name 
	{0,MMI_IDLE_ICON_AREA_HEIGH + 2*MMI_DEFAULT_FONT_HEIGHT + MMI_IDLE_COMON_INTREVAL + 2,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
	//待机MAIN LCD SIM3 Service Providor Name 
	{0,MMI_IDLE_ICON_AREA_HEIGH + 3*MMI_DEFAULT_FONT_HEIGHT + MMI_IDLE_COMON_INTREVAL + 4,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
	//待机MAIN LCD SIM4 Service Providor Name 
	{0,MMI_IDLE_ICON_AREA_HEIGH + 4*MMI_DEFAULT_FONT_HEIGHT + MMI_IDLE_COMON_INTREVAL + 6,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},
#endif
#endif

#ifdef MMI_MULTI_SIM_SYS_SINGLE
    //待机MAIN LCD自定义词语 
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + (MMI_DEFAULT_NORMAL_FONT_SIZE+4) + MMI_IDLE_COMON_INTREVAL,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//待机MAIN LCD倒记时显示
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 2*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//待机MAIN LCD日历显示
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 3*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//待机MAIN LCD MP3背景播放mp3 name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 4*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL,
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},
	
	//待机MAIN LCD FM背景播放FM name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT+5*(MMI_DEFAULT_NORMAL_FONT_SIZE+4) + MMI_IDLE_COMON_INTREVAL, 
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},

#elif defined MMI_MULTI_SIM_SYS_DUAL
    //待机MAIN LCD自定义词语 
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + (MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 2,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//待机MAIN LCD倒记时显示
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 2*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 2,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//待机MAIN LCD日历显示
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 3*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 2,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//待机MAIN LCD MP3背景播放mp3 name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 4*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 2,
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},
	
	//待机MAIN LCD FM背景播放FM name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 5*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 2, 
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},

#elif defined MMI_MULTI_SIM_SYS_TRI
    //待机MAIN LCD自定义词语 
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + (MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//待机MAIN LCD倒记时显示
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 2*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//待机MAIN LCD日历显示
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 3*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//待机MAIN LCD MP3背景播放mp3 name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 4*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},
	
	//待机MAIN LCD FM背景播放FM name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 5*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6, 
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},

#elif defined MMI_MULTI_SIM_SYS_QUAD
    //待机MAIN LCD自定义词语 
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + (MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//待机MAIN LCD倒记时显示
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 2*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//待机MAIN LCD日历显示
	{0, MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 3*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
        IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},

	//待机MAIN LCD MP3背景播放mp3 name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 4*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6,
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},
	
	//待机MAIN LCD FM背景播放FM name
	{0,	MMI_IDLE_ICON_AREA_HEIGH + MMI_DUAL_SYS_MAX *MMI_DEFAULT_FONT_HEIGHT + 5*(MMI_DEFAULT_NORMAL_FONT_SIZE+4)  + MMI_IDLE_COMON_INTREVAL + 6, 
	    IDLE_ALIGN_X_CENTER,  MMI_DEFAULT_TEXT_FONT},

#endif

#ifndef DCD_SUPPORT
	//待机MAIN LCD时间
	{0,22, IDLE_ALIGN_CUSTOM,SONG_FONT_14}, 
#ifdef MMI_RES_ORIENT_PORTRAIT	
	//待机MAIN LCD日期
    {0, 21+14+5,
	    IDLE_ALIGN_X_CENTER,SONG_FONT_13},//SONG_FONT_12

	//待机MAIN LCD星期 
	{MMI_MAINSCREEN_RIGHT_MAX_PIXEL - 4,21+14+5,IDLE_ALIGN_RIGHT,SONG_FONT_13},
#else
	//待机MAIN LCD日期
    {0, 21+14+3,
	    IDLE_ALIGN_X_CENTER,SONG_FONT_13},//SONG_FONT_12

	//待机MAIN LCD星期 
	{MMI_MAINSCREEN_RIGHT_MAX_PIXEL - 4,21+14+3,IDLE_ALIGN_RIGHT,SONG_FONT_13},
#endif
   
#else
	{0,27, IDLE_ALIGN_CUSTOM,MMI_DEFAULT_TEXT_FONT}, 
	//待机MAIN LCD日期
	{60, 27,
	    IDLE_ALIGN_CUSTOM,MMI_DEFAULT_NORMAL_FONT},
	
    //待机MAIN LCD星期 
	{90,27,IDLE_ALIGN_CUSTOM,MMI_DEFAULT_NORMAL_FONT},
#endif
    //待机SUB LCD时间
	{0,28,
	    IDLE_ALIGN_X_CENTER, SONG_FONT_29},

	//待机SUB LCD日期  
	{0,65,
        IDLE_ALIGN_X_CENTER, MMI_DEFAULT_NORMAL_FONT},

	//待机SUB LCD星期
	{0,50,
	    IDLE_ALIGN_X_CENTER,MMI_DEFAULT_NORMAL_FONT},

	//SUB LCD闹钟到了的时间文本显示
	{0,50, 
	    IDLE_ALIGN_X_CENTER,MMI_DEFAULT_NORMAL_FONT},

	//SUB LCD倒记时显示,
	{0,50, 
	    IDLE_ALIGN_X_CENTER,MMI_DEFAULT_NORMAL_FONT},
    //STK set up idle mode 显示的文本
	{MMI_MAINSCREEN_IDLE_STK_LEFT, MMI_COMMOM_IDLE_STK_TEXT_Y,
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






















