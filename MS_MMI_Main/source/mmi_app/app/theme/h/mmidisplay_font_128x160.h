/*****************************************************************************
** File Name:      mmidisplay_font_128x160.h                                 *
** Author:                                                                   *
** Date:           02/20/2020                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define font macro for 128x160        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2020      fangfang.yao       Create                                    *
******************************************************************************/

#ifndef _MMIDISPLAY_FONT_128X160_H_
#define _MMIDISPLAY_FONT_128X160_H_

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
#define MMI_TITLE_TEXT_FONT_PDA             SONG_FONT_16 //Title上文字的字体

#define MMI_STATUS_FONT_TYPE                SONG_FONT_18   
#define MMI_DCD_IDLE_TEXT_FONT              SONG_FONT_12
#define MMI_SET_TIME_FONT                   SONG_FONT_34
#define MMI_SET_DATE_FONT                   SONG_FONT_22

        
#define MMI_DEFAULT_TEXT_FONT               MMI_DEFAULT_NORMAL_FONT
#define MMI_FULLSCREEN_SOFTKEY_TEXT_FONT    MMI_DEFAULT_NORMAL_FONT //全屏下SOFTKEY上文字的字体
#define MMI_FULLSCREEN_SOFTKEY_TEXT_SMALL_FONT  MMI_DEFAULT_NORMAL_FONT //SOFTKEY外文时的字体

#if defined  MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT || defined TOUCHPANEL_TYPE
#define MMI_FULLSCREEN_SCROLLKEY_TEXT_FONT  SONG_FONT_24
#endif
#ifdef GUIF_TITLE_PDASTYLE
#define MMI_TITLE_TEXT_FONT                 MMI_TITLE_TEXT_FONT_PDA //Title上文字的字体
#else
#define MMI_TITLE_TEXT_FONT                 MMI_DEFAULT_BIG_FONT //Title上文字的字体
#endif

//pubwin font info
#define MMI_PUBWIN_BIG_FONT          SONG_FONT_14
#define MMI_PUBWIN_NORMAL_FONT       SONG_FONT_12
#define MMI_PUBWIN_SMALL_FONT        SONG_FONT_11        

/* list box item style*/
#define MMI_LIST_BIG_ITEM_FONT		 (MMI_DEFAULT_TEXT_FONT)
#define MMI_LIST_NORMAL_ITEM_FONT    (MMI_DEFAULT_TEXT_FONT)
#define MMI_LIST_DEFAULT_ITEM_FONT   (MMI_DEFAULT_TEXT_FONT)
#define MMI_LIST_EXPAND_ITEM_FONT    (MMI_DEFAULT_TEXT_FONT)
#define MMI_LIST_EXPAND_ITEM_HEIGHT  (MMI_LIST_ITEM_HEIGHT)
#define MMI_LIST_ITEM_SMALL_FONT     (MMI_DEFAULT_SMALL_FONT)


#define MMI_LIST_ITEM_FONT_ENG      (SONG_FONT_25)//list height of egineer mode

//==========================================================================//
//                            For watch                                     //
//==========================================================================//

//for listbox
#define WATCHLIST_ITEM_BIG_FONT     (WATCH_DEFAULT_BIG_FONT)
#define WATCHLIST_ITEM_NORMAL_FONT  (WATCH_DEFAULT_NORMAL_FONT)
#define WATCHLIST_ITEM_SMALL_FONT   (WATCH_DEFAULT_SMALL_FONT)

//for pubwin
#define WATCH_COMMONWIN_BIG_FONT    (WATCH_DEFAULT_BIG_FONT)
#define WATCH_COMMONWIN_NORMAL_FONT (WATCH_DEFAULT_NORMAL_FONT)
#define WATCH_COMMONWIN_SMALL_FONT  (WATCH_DEFAULT_SMALL_FONT)

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
