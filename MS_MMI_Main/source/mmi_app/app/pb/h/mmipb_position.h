/*****************************************************************************
** File Name:      mmipb_displaysize.h                                                   *
** Author:                                                                   *
** Date:           13/04/2007                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe phonebook                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**04/2007       maggie.ren         Create
******************************************************************************/

#ifndef _MMIPB_POSITION_H
#define _MMIPB_POSITION_H

#ifdef MAINLCD_SIZE_128X160
//    #define MMIPB_BUTTON_HEIGHT             20
//    #define MMIPB_BUTTON_WIDTH              18
//	#define MMIPB_HASH_BUTTON_WIDTH         28
//    #define MMIPB_BUTTON_TOP                175
//    #define MMIPB_BUTTON_BOTTOM             196
//    #define MMIPB_BUTTON0_LEFT              0
//    #define MMIPB_BUTTON1_LEFT              17
//    #define MMIPB_BUTTON2_LEFT              34
//    #define MMIPB_BUTTON3_LEFT              51
//    #define MMIPB_BUTTON4_LEFT              68
//    #define MMIPB_BUTTON5_LEFT              86
//    #define MMIPB_BUTTON6_LEFT              104
//    #define MMIPB_BUTTON7_LEFT              122
//    #define MMIPB_BUTTON8_LEFT              140
//    #define MMIPB_BUTTON9_LEFT              159
//     #define MMIPB_TAB_SPACE                 3
    #define MMIPB_TITLE_FONT                SONG_FONT_12
//     #define MMIPB_TITLE_FONT_WID            6
    #define MMIPB_PHOTO_WIDTH               68
    #define MMIPB_PHOTO_HEIGHT              68
#elif MAINLCD_SIZE_176X220
//    #define MMIPB_BUTTON_HEIGHT             20
//    #define MMIPB_BUTTON_WIDTH              18
//	#define MMIPB_HASH_BUTTON_WIDTH         28
//    #define MMIPB_BUTTON_TOP                175
//    #define MMIPB_BUTTON_BOTTOM             196
//    #define MMIPB_BUTTON0_LEFT              0
//    #define MMIPB_BUTTON1_LEFT              17
//    #define MMIPB_BUTTON2_LEFT              34
//    #define MMIPB_BUTTON3_LEFT              51
//    #define MMIPB_BUTTON4_LEFT              68
//    #define MMIPB_BUTTON5_LEFT              86
//    #define MMIPB_BUTTON6_LEFT              104
//    #define MMIPB_BUTTON7_LEFT              122
//    #define MMIPB_BUTTON8_LEFT              140
//    #define MMIPB_BUTTON9_LEFT              159
//     #define MMIPB_TAB_SPACE                 3
    #define MMIPB_TITLE_FONT                SONG_FONT_12
//     #define MMIPB_TITLE_FONT_WID            6
    #define MMIPB_PHOTO_WIDTH               68
    #define MMIPB_PHOTO_HEIGHT              68
#elif MAINLCD_SIZE_240X320
/*
    #define MMIPB_BUTTON_HEIGHT             26
    #define MMIPB_BUTTON_WIDTH              24 
    #define MMIPB_BUTTON_TOP                262
    #define MMIPB_BUTTON_BOTTOM             287
	#define MMIPB_HASH_BUTTON_WIDTH			29
    #define MMIPB_BUTTON0_LEFT              0
    #define MMIPB_BUTTON1_LEFT              (MMIPB_BUTTON0_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON2_LEFT              (MMIPB_BUTTON1_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON3_LEFT              (MMIPB_BUTTON2_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON4_LEFT              (MMIPB_BUTTON3_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON5_LEFT              (MMIPB_BUTTON4_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON6_LEFT              (MMIPB_BUTTON5_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON7_LEFT              (MMIPB_BUTTON6_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON8_LEFT              (MMIPB_BUTTON7_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON9_LEFT              (MMIPB_BUTTON8_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_TAB_SPACE                 5
	*/
    #define MMIPB_TITLE_FONT                MMI_DEFAULT_NORMAL_FONT
    //#define MMIPB_TITLE_FONT_WID            (MMI_DEFAULT_FONT_ASCII_WIDTH +2)
    #define MMIPB_PHOTO_WIDTH               90
    #define MMIPB_PHOTO_HEIGHT              90
#elif MAINLCD_SIZE_128X64
/*
    #define MMIPB_BUTTON_HEIGHT             26
    #define MMIPB_BUTTON_WIDTH              24 
    #define MMIPB_BUTTON_TOP                262
    #define MMIPB_BUTTON_BOTTOM             287
	#define MMIPB_HASH_BUTTON_WIDTH			29
    #define MMIPB_BUTTON0_LEFT              0
    #define MMIPB_BUTTON1_LEFT              (MMIPB_BUTTON0_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON2_LEFT              (MMIPB_BUTTON1_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON3_LEFT              (MMIPB_BUTTON2_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON4_LEFT              (MMIPB_BUTTON3_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON5_LEFT              (MMIPB_BUTTON4_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON6_LEFT              (MMIPB_BUTTON5_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON7_LEFT              (MMIPB_BUTTON6_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON8_LEFT              (MMIPB_BUTTON7_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_BUTTON9_LEFT              (MMIPB_BUTTON8_LEFT + MMIPB_BUTTON_WIDTH)
    #define MMIPB_TAB_SPACE                 5
	*/
    #define MMIPB_TITLE_FONT                MMI_DEFAULT_NORMAL_FONT
    //#define MMIPB_TITLE_FONT_WID            (MMI_DEFAULT_FONT_ASCII_WIDTH +2)
    #define MMIPB_PHOTO_WIDTH               20//90
    #define MMIPB_PHOTO_HEIGHT              20//90

#elif MAINLCD_SIZE_240X400
//    #define MMIPB_BUTTON_HEIGHT             26
//    #define MMIPB_BUTTON_WIDTH              24  
//    #define MMIPB_BUTTON_TOP                341
//    #define MMIPB_BUTTON_BOTTOM             367
//	#define MMIPB_HASH_BUTTON_WIDTH			29
//    #define MMIPB_BUTTON0_LEFT              0
//    #define MMIPB_BUTTON1_LEFT              (MMIPB_BUTTON0_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON2_LEFT              (MMIPB_BUTTON1_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON3_LEFT              (MMIPB_BUTTON2_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON4_LEFT              (MMIPB_BUTTON3_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON5_LEFT              (MMIPB_BUTTON4_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON6_LEFT              (MMIPB_BUTTON5_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON7_LEFT              (MMIPB_BUTTON6_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON8_LEFT              (MMIPB_BUTTON7_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON9_LEFT              (MMIPB_BUTTON8_LEFT + MMIPB_BUTTON_WIDTH)
 //   #define MMIPB_TAB_SPACE                 5
    #define MMIPB_TITLE_FONT                MMI_DEFAULT_NORMAL_FONT
    //#define MMIPB_TITLE_FONT_WID            (MMI_DEFAULT_FONT_ASCII_WIDTH +2)
    #define MMIPB_PHOTO_WIDTH               90
    #define MMIPB_PHOTO_HEIGHT              90
#else
//    #define MMIPB_BUTTON_HEIGHT             26
//    #define MMIPB_BUTTON_WIDTH              24  
//    #define MMIPB_BUTTON_TOP                262
//    #define MMIPB_BUTTON_BOTTOM             287
//	#define MMIPB_HASH_BUTTON_WIDTH			28
//    #define MMIPB_BUTTON0_LEFT              0
//    #define MMIPB_BUTTON1_LEFT              (MMIPB_BUTTON0_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON2_LEFT              (MMIPB_BUTTON1_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON3_LEFT              (MMIPB_BUTTON2_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON4_LEFT              (MMIPB_BUTTON3_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON5_LEFT              (MMIPB_BUTTON4_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON6_LEFT              (MMIPB_BUTTON5_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON7_LEFT              (MMIPB_BUTTON6_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON8_LEFT              (MMIPB_BUTTON7_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_BUTTON9_LEFT              (MMIPB_BUTTON8_LEFT + MMIPB_BUTTON_WIDTH)
//    #define MMIPB_TAB_SPACE                 5
    #define MMIPB_TITLE_FONT                MMI_DEFAULT_NORMAL_FONT
    //#define MMIPB_TITLE_FONT_WID            (MMI_DEFAULT_FONT_ASCII_WIDTH +2)
    #define MMIPB_PHOTO_WIDTH               90
    #define MMIPB_PHOTO_HEIGHT              90
#endif
//#define MMIPB_TAB_TOP                 28
//#define MMIPB_TAB_WIDTH               40		
//#define MMIPB_TAB_HEIGH               21
/*
#ifdef TOUCH_PANEL_SUPPORT
       #define MMIPB_BUTTONHASH_TOP   (MMIPB_BUTTON_TOP-MMIPB_BUTTON_HEIGHT-1)


	#define MMIPB_BUTTONHASH_BOTTOM (MMIPB_BUTTON_TOP-1)
	#define MMIPB_SEARCH_EDIT_RIGHT	   (MMI_MAINSCREEN_WIDTH-(MMIPB_BUTTON_WIDTH+MMIPB_HASH_BUTTON_WIDTH))
#else
	#define MMIPB_BUTTONHASH_TOP   (MMIPB_BUTTON_TOP)
	#define MMIPB_BUTTONHASH_BOTTOM (MMIPB_BUTTON_BOTTOM)
	#define MMIPB_SEARCH_EDIT_RIGHT	   (MMI_MAINSCREEN_WIDTH)
#endif
	#define MMIPB_BUTTONHASH_LEFT  MMIPB_BUTTON0_LEFT
	#define MMIPB_BUTTONHASH_RIGHT (MMIPB_BUTTONHASH_LEFT+MMIPB_HASH_BUTTON_WIDTH)
	#define MMIPB_SEARCH_EDIT_TOP      MMIPB_BUTTONHASH_TOP
	#define MMIPB_SEARCH_EDIT_BOTTOM MMIPB_BUTTONHASH_BOTTOM

    #define MMIPB_SEARCH_EDIT_LEFT     MMIPB_BUTTONHASH_RIGHT
    #define MMIPB_BUTTONDEL_TOP   (MMIPB_BUTTONHASH_TOP)
    #define MMIPB_BUTTONDEL_LEFT MMIPB_SEARCH_EDIT_RIGHT
    #define MMIPB_BUTTONDEL_RIGHT (MMI_MAINSCREEN_RIGHT_MAX_PIXEL)
    #define MMIPB_BUTTONDEL_BOTTOM MMIPB_BUTTONHASH_BOTTOM	
*/
#endif //_MMIPB_POSITION_H


