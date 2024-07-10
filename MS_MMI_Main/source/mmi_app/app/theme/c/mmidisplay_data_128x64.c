/*****************************************************************************
** File Name:      mmidisplay_data_64x128.c                                 *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#define _MMIDISPLAY_DATA_64x128_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmi_image.h"
#include "mmi_custom_define.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifndef IDLE_WEEK_TOP  //@ alex modified
#define IDLE_WEEK_TOP 251
#endif
#ifndef IDLE_DATE_TOP
#define IDLE_DATE_TOP 254
#endif
//


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//IDLE界面显示的文字的坐标，颜色，大小
LOCAL const MMI_IDLE_DISPLAY_T  mmi_idle_display[TOTAL_IDLE_DISPLAY_NUMBER] =
{               
	//@zhaohui,cr102440,调整了idle下显示的位置
	//待机MAIN LCD运营商文字 	
 //added by lxf for dphone

	{0, 16, IDLE_ALIGN_X_CENTER, MMI_DEFAULT_TEXT_FONT},
#ifndef MMI_MULTI_SIM_SYS_SINGLE
	{0, 16, IDLE_ALIGN_X_CENTER, MMI_DEFAULT_TEXT_FONT},
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
	{0, 16, IDLE_ALIGN_X_CENTER, MMI_DEFAULT_TEXT_FONT},
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
	{0, 16, IDLE_ALIGN_X_CENTER, MMI_DEFAULT_TEXT_FONT},
#endif
#endif

	//待机MAIN LCD Service Providor Name 	
	{82,55,
    IDLE_ALIGN_X_CENTER, 
    MMI_DEFAULT_TEXT_FONT},
   
	//待机MAIN LCD MP3背景播放mp3 name
	{0,	144, IDLE_ALIGN_X_CENTER, 
    MMI_DEFAULT_TEXT_FONT},
	
	//待机MAIN LCD FM背景播放FM name
	{0,	144, IDLE_ALIGN_X_CENTER, 
    MMI_DEFAULT_TEXT_FONT},

	//待机MAIN LCD时间
	{94,MMI_MAIN_TIME_Y, IDLE_ALIGN_CUSTOM,
    MMI_DEFAULT_TEXT_FONT}, 
	

	{0, 32, IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},
	{0,MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT,IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},


    //待机MAIN LCD纪念日
	{0,MMI_IDLE_ICON_AREA_HEIGH + (MMI_DEFAULT_FONT_HEIGHT+2) + 1,
        IDLE_ALIGN_X_CENTER,
        MMI_DEFAULT_TEXT_FONT},
	{0, 32, IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},
//	{0,MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT,IDLE_ALIGN_X_CENTER,MMI_DEFAULT_TEXT_FONT},


    //待机MAIN LCD自定义词语 
	//{0,95,IDLE_ALIGN_X_CENTER,
   // MMI_DEFAULT_TEXT_FONT,},

	//待机MAIN LCD倒记时显示
	{0,95,//MMI_IDLE_ICON_AREA_HEIGH + 2*(MMI_DEFAULT_FONT_HEIGHT+2) + 1,
        IDLE_ALIGN_X_CENTER,
        MMI_DEFAULT_TEXT_FONT,},

	//待机MAIN LCD日历显示
	{0,120,
        IDLE_ALIGN_X_CENTER,
        MMI_DEFAULT_TEXT_FONT,},
        
	//MIAN  LCD闹钟到了的时间文本显示
	{0, 87,IDLE_ALIGN_X_CENTER,
    MMI_DEFAULT_TEXT_FONT},
	
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

    //STK set up idle mode 显示的文本
	{0,MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - 3 * MMI_DEFAULT_FONT_HEIGHT - 5,
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






















