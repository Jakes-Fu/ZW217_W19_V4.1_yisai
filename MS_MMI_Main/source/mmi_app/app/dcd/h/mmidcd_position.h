/*****************************************************************************
** File Name:      mmidcd_position.h                                *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      byte.guo              Creat
******************************************************************************/

#ifndef _MMIDCD_POSITION_H_
#define _MMIDCD_POSITION_H_ 

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
#define     MMI_DCD_IDLEWIN_LISTBOX_HEIGHT          37
#define     MMI_DCD_IDLEWIN_ANIM_WIDTH              30
#define     MMIDCD_TAB_DEFAULT_HEIGHT               19

#define     MMIDCD_TAB_IMG_W                        16
#define     MMIDCD_TAB_IMG_H                        16

#elif defined(MAINLCD_SIZE_240X320)        
/*************************************MAINLCD_SIZE_240X320 BEGINS*******************************/
#define     MMI_DCD_IDLEWIN_LISTBOX_HEIGHT          51
#define     MMI_DCD_IDLEWIN_ANIM_WIDTH              50
#define     MMIDCD_TAB_DEFAULT_HEIGHT               33
#define     MMIDCD_TAB_IMG_W                        20
#define     MMIDCD_TAB_IMG_H                        20
    
#elif defined(MAINLCD_SIZE_240X400)
/*************************************MAINLCD_SIZE_240X400 BEGINS*******************************/
#define     MMI_DCD_IDLEWIN_LISTBOX_HEIGHT          51
#define     MMI_DCD_IDLEWIN_ANIM_WIDTH              50
#define     MMIDCD_TAB_DEFAULT_HEIGHT               35

#define     MMIDCD_TAB_IMG_W                        20
#define     MMIDCD_TAB_IMG_H                        20

#elif defined(MAINLCD_SIZE_320X480)        
/*************************************MAINLCD_SIZE_320X480 BEGINS*******************************/
#define     MMI_DCD_IDLEWIN_LISTBOX_HEIGHT          61
#define     MMI_DCD_IDLEWIN_ANIM_WIDTH              60
#define     MMIDCD_TAB_DEFAULT_HEIGHT               33

#define     MMIDCD_TAB_IMG_W                        20
#define     MMIDCD_TAB_IMG_H                        20

#else
#error
#endif

#define     MMIDCD_ITEM_PREVIEW_LINE_SPACE          5
#define     MMI_DCD_IDLEWIN_LISTBOX_TOP             (MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-MMI_DCD_IDLEWIN_LISTBOX_HEIGHT)
#define     MMI_DCD_IDLEWIN_LISTBOX_BOTTOM          (MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)

#define     MMI_DCD_IDLEWIN_ANIM_TOP                (MMI_DCD_IDLEWIN_LISTBOX_TOP+2)
#define     MMI_DCD_IDLEWIN_ANIM_BOTTOM             (MMI_DCD_IDLEWIN_LISTBOX_BOTTOM-2)
#define     MMI_DCD_IDLEWIN_ANIM_LEFT               2
#define     MMI_DCD_IDLEWIN_ANIM_RIGHT              (MMI_DCD_IDLEWIN_ANIM_LEFT+MMI_DCD_IDLEWIN_ANIM_WIDTH)

#define     MMI_DCD_IDLEWIN_TEXT_TOP                (MMI_DCD_IDLEWIN_LISTBOX_TOP+1)
#define     MMI_DCD_IDLEWIN_TEXT_BOTTOM             (MMI_DCD_IDLEWIN_LISTBOX_BOTTOM-1)
#define     MMI_DCD_IDLEWIN_TEXT_LEFT               (MMI_DCD_IDLEWIN_ANIM_RIGHT+2)
#define     MMI_DCD_IDLEWIN_TEXT_RIGHT              MMI_MAINSCREEN_RIGHT_MAX_PIXEL

#if defined(MAINLCD_SIZE_240X320)     
#define     MMI_DCD_IDLEWIN_TAB_TOP                 (MMI_DCD_IDLEWIN_LISTBOX_TOP-MMIDCD_TAB_DEFAULT_HEIGHT+5)
#elif   defined(MAINLCD_SIZE_240X400)||defined(MAINLCD_SIZE_320X480)
#define     MMI_DCD_IDLEWIN_TAB_TOP                 (MMI_DCD_IDLEWIN_LISTBOX_TOP-MMIDCD_TAB_DEFAULT_HEIGHT+7)
#else
#define     MMI_DCD_IDLEWIN_TAB_TOP                 (MMI_DCD_IDLEWIN_LISTBOX_TOP-MMIDCD_TAB_DEFAULT_HEIGHT-1)
#endif

#define     MMI_DCD_IDLEWIN_TAB_BOTTOM              MMI_DCD_IDLEWIN_LISTBOX_TOP


#define		MMIDCD_MAX_PROXY_LEN			        DCD_PROXY_MAX_LEN
#define		MMIDCD_MAX_USERNAME_LEN			        DCD_USERNAME_MAX_LEN
#define		MMIDCD_MAX_PASSWORD_LEN			        DCD_PASSWORD_MAX_LEN
#define		MMIDCD_MAX_APN_LEN				        DCD_APN_MAX_LEN
#define		MMIDCD_MAX_PORT_LEN				        4
#define		MMIDCD_MAX_UA_LEN				        255
#define     DCD_IMAGE_MAX_SIZE                      30000
#define     DCD_ITEM_MAX_FILE_NUM                   5

#define     MMIDCD_TITLE_FONT                       MMI_DEFAULT_SMALL_FONT
#define     MMIDCD_TITLE_FONT_WID                   MMI_DEFAULT_FONT_ASCII_WIDTH

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

#endif   //_MMIDCD_POSITION_H_