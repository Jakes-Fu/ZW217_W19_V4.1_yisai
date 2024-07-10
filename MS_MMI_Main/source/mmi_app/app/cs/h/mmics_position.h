/*****************************************************************************
** File Name:      mmidcd_position.h                                *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      byte.guo              Creat
******************************************************************************/

#ifndef _MMICS_POSITION_H_
#define _MMICS_POSITION_H_ 

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

#ifdef MAINLCD_SIZE_176X220
/*************************************MMI_LCD_176_220 BEGINS*******************************/
/* 需要重新调整坐标 */

#define            MMIDCD_ITEM_PREVIEW_LINE_SPACE          5

#define		MMI_DCD_CHILD_WINDOW_TOP				40

#define            MMI_DCD_IDLEWIN_LISTBOX_TOP             (MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-37)
#define            MMI_DCD_IDLEWIN_LISTBOX_BOTTOM          (MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)

#define            MMI_DCD_IDLEWIN_ANIM_TOP             (MMI_DCD_IDLEWIN_LISTBOX_TOP+4)//239
#define            MMI_DCD_IDLEWIN_ANIM_BOTTOM       (MMI_DCD_IDLEWIN_LISTBOX_BOTTOM-4)//286
#define            MMI_DCD_IDLEWIN_ANIM_LEFT             3
#define            MMI_DCD_IDLEWIN_ANIM_RIGHT          32

#ifdef TOUCH_PANEL_SUPPORT
#define            MMI_DCD_IDLEWIN_TEXT_TOP             (MMI_DCD_IDLEWIN_LISTBOX_TOP+1)//238
#define            MMI_DCD_IDLEWIN_TEXT_BOTTOM         (MMI_DCD_IDLEWIN_LISTBOX_BOTTOM-2)//286
#define            MMI_DCD_IDLEWIN_TEXT_LEFT             35
#define            MMI_DCD_IDLEWIN_TEXT_RIGHT          175
#else
#define            MMI_DCD_IDLEWIN_TEXT_TOP             (MMI_DCD_IDLEWIN_LISTBOX_TOP+1)//238
#define            MMI_DCD_IDLEWIN_TEXT_BOTTOM          (MMI_DCD_IDLEWIN_LISTBOX_BOTTOM-2)//286
#define            MMI_DCD_IDLEWIN_TEXT_LEFT             35
#define            MMI_DCD_IDLEWIN_TEXT_RIGHT          175
#endif
#define            MMI_DCD_IDLEWIN_TAB_TOP                 (MMI_DCD_IDLEWIN_LISTBOX_TOP-20)//211//(MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-52-21)
#define            MMI_DCD_IDLEWIN_TAB_BOTTOM              MMI_DCD_IDLEWIN_LISTBOX_TOP//237//(MMI_DCD_IDLEWIN_TAB_TOP + 21)


#define		MMIDCD_MAX_URL_LEN				        DCD_URL_MAX_LEN
#define		MMIDCD_MAX_PROXY_LEN			        DCD_PROXY_MAX_LEN
#define		MMIDCD_MAX_USERNAME_LEN			        DCD_USERNAME_MAX_LEN
#define		MMIDCD_MAX_PASSWORD_LEN			        DCD_PASSWORD_MAX_LEN
#define		MMIDCD_MAX_APN_LEN				        DCD_APN_MAX_LEN
#define		MMIDCD_MAX_PORT_LEN				        4
#define		MMIDCD_MAX_UA_LEN				        255

#define            DCD_IMAGE_MAX_SIZE                      30000
#define            DCD_ITEM_MAX_FILE_NUM                   5

#define          MMIDCD_TAB_DEFAULT_HEIGHT           19

#define          MMIDCD_TAB_IMG_W                             16
#define          MMIDCD_TAB_IMG_H                             16

#define            MMIDCD_TITLE_FONT                      MMI_DEFAULT_TEXT_FONT
#define            MMIDCD_TITLE_FONT_WID                  MMI_DEFAULT_FONT_ASCII_WIDTH

#elif MAINLCD_SIZE_240X320        
/*************************************MAINLCD_SIZE_240X300 BEGINS*******************************/

#define            MMICS_ITEM_PREVIEW_LINE_SPACE          5

//name
#define MMICS_NAME_TITLE_LEFT          14
#define MMICS_NAME_TITLE_TOP           (MMI_TITLE_HEIGHT + 10)
#define MMICS_NAME_TITLE_RIGHT         (MMI_MAINSCREEN_RIGHT_MAX_PIXEL - 13)
#define MMICS_NAME_TITLE_BOTTOM        (MMICS_NAME_TITLE_TOP + 18)//(MMISCH_NAME_TITLE_TOP + 22)

#define MMICS_NAME_LEFT                (MMICS_NAME_TITLE_LEFT )
#define MMICS_NAME_TOP                 MMICS_NAME_TITLE_BOTTOM+2
#define MMICS_NAME_RIGHT               MMICS_NAME_TITLE_RIGHT
#define MMICS_NAME_BOTTOM              MMICS_NAME_TOP+18
#define MMICS_NAME_RECT				{MMICS_NAME_LEFT, MMICS_NAME_TOP, MMICS_NAME_RIGHT, MMICS_NAME_BOTTOM}

//num
#define MMICS_NUM_TITLE_LEFT          MMICS_NAME_TITLE_LEFT
#define MMICS_NUM_TITLE_TOP           (MMICS_NAME_BOTTOM + 10)
#define MMICS_NUM_TITLE_RIGHT         MMICS_NAME_TITLE_RIGHT
#define MMICS_NUM_TITLE_BOTTOM        (MMICS_NUM_TITLE_TOP + 18)//(MMISCH_DATE_TITLE_TOP + 22)

#define MMICS_NUM_LEFT                MMICS_NAME_TITLE_LEFT
#define MMICS_NUM_TOP                 (MMICS_NUM_TITLE_BOTTOM+2)
#define MMICS_NUM_RIGHT               MMICS_NAME_TITLE_RIGHT
#define MMICS_NUM_BOTTOM              MMICS_NUM_TOP+18
#define MMICS_NUM_RECT				{MMICS_NUM_LEFT, MMICS_NUM_TOP, MMICS_NUM_RIGHT, MMICS_NUM_BOTTOM}

#elif MAINLCD_SIZE_128X64        
/*************************************MAINLCD_SIZE_240X300 BEGINS*******************************/

#define            MMICS_ITEM_PREVIEW_LINE_SPACE          5

//name
#define MMICS_NAME_TITLE_LEFT          14
#define MMICS_NAME_TITLE_TOP           (MMI_TITLE_HEIGHT + 10)
#define MMICS_NAME_TITLE_RIGHT         (MMI_MAINSCREEN_RIGHT_MAX_PIXEL - 13)
#define MMICS_NAME_TITLE_BOTTOM        (MMICS_NAME_TITLE_TOP + 18)//(MMISCH_NAME_TITLE_TOP + 22)

#define MMICS_NAME_LEFT                (MMICS_NAME_TITLE_LEFT )
#define MMICS_NAME_TOP                 MMICS_NAME_TITLE_BOTTOM+2
#define MMICS_NAME_RIGHT               MMICS_NAME_TITLE_RIGHT
#define MMICS_NAME_BOTTOM              MMICS_NAME_TOP+18
#define MMICS_NAME_RECT				{MMICS_NAME_LEFT, MMICS_NAME_TOP, MMICS_NAME_RIGHT, MMICS_NAME_BOTTOM}

//num
#define MMICS_NUM_TITLE_LEFT          MMICS_NAME_TITLE_LEFT
#define MMICS_NUM_TITLE_TOP           (MMICS_NAME_BOTTOM + 10)
#define MMICS_NUM_TITLE_RIGHT         MMICS_NAME_TITLE_RIGHT
#define MMICS_NUM_TITLE_BOTTOM        (MMICS_NUM_TITLE_TOP + 18)//(MMISCH_DATE_TITLE_TOP + 22)

#define MMICS_NUM_LEFT                MMICS_NAME_TITLE_LEFT
#define MMICS_NUM_TOP                 (MMICS_NUM_TITLE_BOTTOM+2)
#define MMICS_NUM_RIGHT               MMICS_NAME_TITLE_RIGHT
#define MMICS_NUM_BOTTOM              MMICS_NUM_TOP+18
#define MMICS_NUM_RECT				{MMICS_NUM_LEFT, MMICS_NUM_TOP, MMICS_NUM_RIGHT, MMICS_NUM_BOTTOM}


#elif MAINLCD_SIZE_240X400
/*************************************MAINLCD_SIZE_240X400 BEGINS*******************************/
/* 需要重新调整坐标 */

#define            MMIDCD_ITEM_PREVIEW_LINE_SPACE          5
#define		MMI_DCD_CHILD_WINDOW_TOP				53

#define            MMI_DCD_IDLEWIN_LISTBOX_TOP             (MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-51)
#define            MMI_DCD_IDLEWIN_LISTBOX_BOTTOM          (MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)

#define            MMI_DCD_IDLEWIN_ANIM_TOP             (MMI_DCD_IDLEWIN_LISTBOX_TOP+2)//239
#define            MMI_DCD_IDLEWIN_ANIM_BOTTOM       (MMI_DCD_IDLEWIN_LISTBOX_BOTTOM-2)//286
#define            MMI_DCD_IDLEWIN_ANIM_LEFT             2
#define            MMI_DCD_IDLEWIN_ANIM_RIGHT          50

#ifdef TOUCH_PANEL_SUPPORT
#define            MMI_DCD_IDLEWIN_TEXT_TOP             (MMI_DCD_IDLEWIN_LISTBOX_TOP+1)//238
#define            MMI_DCD_IDLEWIN_TEXT_BOTTOM         (MMI_DCD_IDLEWIN_LISTBOX_BOTTOM-2)//286
#define            MMI_DCD_IDLEWIN_TEXT_LEFT             52
#define            MMI_DCD_IDLEWIN_TEXT_RIGHT          239//195
#else
#define            MMI_DCD_IDLEWIN_TEXT_TOP             (MMI_DCD_IDLEWIN_LISTBOX_TOP+1)//238
#define            MMI_DCD_IDLEWIN_TEXT_BOTTOM          (MMI_DCD_IDLEWIN_LISTBOX_BOTTOM-2)//286
#define            MMI_DCD_IDLEWIN_TEXT_LEFT             52
#define            MMI_DCD_IDLEWIN_TEXT_RIGHT          239//195
#endif
#define            MMI_DCD_IDLEWIN_TAB_TOP                 (MMI_DCD_IDLEWIN_LISTBOX_TOP-26)//211//(MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-52-21)
#define            MMI_DCD_IDLEWIN_TAB_BOTTOM              MMI_DCD_IDLEWIN_LISTBOX_TOP//237//(MMI_DCD_IDLEWIN_TAB_TOP + 21)


#define		MMIDCD_MAX_URL_LEN				        DCD_URL_MAX_LEN
#define		MMIDCD_MAX_PROXY_LEN			        DCD_PROXY_MAX_LEN
#define		MMIDCD_MAX_USERNAME_LEN			        DCD_USERNAME_MAX_LEN
#define		MMIDCD_MAX_PASSWORD_LEN			        DCD_PASSWORD_MAX_LEN
#define		MMIDCD_MAX_APN_LEN				        DCD_APN_MAX_LEN
#define		MMIDCD_MAX_PORT_LEN				        4
#define		MMIDCD_MAX_UA_LEN				        255

#define            DCD_IMAGE_MAX_SIZE                      30000
#define            DCD_ITEM_MAX_FILE_NUM                   5

#define          MMIDCD_TAB_DEFAULT_HEIGHT            25

#define          MMIDCD_TAB_IMG_W                             20
#define          MMIDCD_TAB_IMG_H                             20

#define            MMIDCD_TITLE_FONT                      MMI_DEFAULT_TEXT_FONT
#define            MMIDCD_TITLE_FONT_WID                  MMI_DEFAULT_FONT_ASCII_WIDTH

#else
/*************************************MAINLCD_SIZE_240X400 BEGINS*******************************/
/* 需要重新调整坐标 */

/*************************************MAINLCD_SIZE_240X300 BEGINS*******************************/

#define            MMICS_ITEM_PREVIEW_LINE_SPACE          5

//name
#define MMICS_NAME_TITLE_LEFT          14
#define MMICS_NAME_TITLE_TOP           (MMI_TITLE_HEIGHT + 10)
#define MMICS_NAME_TITLE_RIGHT         (MMI_MAINSCREEN_RIGHT_MAX_PIXEL - 13)
#define MMICS_NAME_TITLE_BOTTOM        (MMICS_NAME_TITLE_TOP + 18)//(MMISCH_NAME_TITLE_TOP + 22)

#define MMICS_NAME_LEFT                (MMICS_NAME_TITLE_LEFT )
#define MMICS_NAME_TOP                 MMICS_NAME_TITLE_BOTTOM+2
#define MMICS_NAME_RIGHT               MMICS_NAME_TITLE_RIGHT
#define MMICS_NAME_BOTTOM              MMICS_NAME_TOP+18
#define MMICS_NAME_RECT				{MMICS_NAME_LEFT, MMICS_NAME_TOP, MMICS_NAME_RIGHT, MMICS_NAME_BOTTOM}

//num
#define MMICS_NUM_TITLE_LEFT          MMICS_NAME_TITLE_LEFT
#define MMICS_NUM_TITLE_TOP           (MMICS_NAME_BOTTOM + 10)
#define MMICS_NUM_TITLE_RIGHT         MMICS_NAME_TITLE_RIGHT
#define MMICS_NUM_TITLE_BOTTOM        (MMICS_NUM_TITLE_TOP + 18)//(MMISCH_DATE_TITLE_TOP + 22)

#define MMICS_NUM_LEFT                MMICS_NAME_TITLE_LEFT
#define MMICS_NUM_TOP                 (MMICS_NUM_TITLE_BOTTOM+2)
#define MMICS_NUM_RIGHT               MMICS_NAME_TITLE_RIGHT
#define MMICS_NUM_BOTTOM              MMICS_NUM_TOP+18
#define MMICS_NUM_RECT				{MMICS_NUM_LEFT, MMICS_NUM_TOP, MMICS_NUM_RIGHT, MMICS_NUM_BOTTOM}
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif   //_MMICS_POSITION_H_