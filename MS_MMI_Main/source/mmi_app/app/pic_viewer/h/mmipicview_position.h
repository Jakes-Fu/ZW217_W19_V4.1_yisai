/*****************************************************************************
** File Name:      mmipicview_position.h                                     *
** Author:                                                                   *
** Date:           24/03/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmimp3_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008       Jassmine           Create
******************************************************************************/
#ifdef PIC_VIEWER_SUPPORT
#ifndef _MMIPICVIEW_POSITION_H_
#define _MMIPICVIEW_POSITION_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"

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
#define MMIPICVIEW_TITLE_GAP    3

//time drop down list
#define MMIPICVIEW_SLIDE_TIME_NUM           6

#if defined MAINLCD_SIZE_320X480
#define MMIPICPREVIEW_TITLE_HEIGHT             50
#define MMIPICPREVIEW_BOTTOM_MENU_HEIGHT       50
#define MMIPICPREVIEW_TITLE_MARGIN             ((GUILCD_IsLandscape(0))?18:14) 
#define MMIPICPREVIEW_LIST_ICON_SIZE           70
#define MMIPICPREVIEW_LIST_ICON_MARGIN         6
#define MMIPICPREVIEW_TITLE_FONT               SONG_FONT_28
#elif defined MAINLCD_SIZE_128X160
#define MMIPICPREVIEW_TITLE_HEIGHT             0
#define MMIPICPREVIEW_BOTTOM_MENU_HEIGHT       40
#define MMIPICPREVIEW_TITLE_MARGIN             ((GUILCD_IsLandscape(0))?16:8) 
#define MMIPICPREVIEW_LIST_ICON_SIZE           38
#define MMIPICPREVIEW_LIST_ICON_MARGIN         2
#define MMIPICPREVIEW_TIME_BG_V_TOP            20
#define MMIPICPREVIEW_TIME_BG__RIGHT           128
#define MMIPICPREVIEW_TIME_HEIGHT              16
#define MMIPICPREVIEW_TITLE_FONT               MMI_DEFAULT_BIG_FONT
#define MMIPICVIEW_DELIMETER_MARGIN          2
#define MMIPICVIEW_TOTAL_NUM_TOP               4
#define MMIPICVIEW_TOTAL_NUM_HEIGHT            12
#define MMIPICVIEW_TOTAL_NUM_V_LEFT              100
#define MMIPICVIEW_TOTAL_NUM_V_RIGHT             127
#define MMIPICVIEW_TOTAL_NUM_H_LEFT               130
#define MMIPICVIEW_TOTAL_NUM_H_RIGHT             159
#elif defined MAINLCD_SIZE_176X220
#define MMIPICPREVIEW_TITLE_HEIGHT             0
#define MMIPICPREVIEW_BOTTOM_MENU_HEIGHT       40
#define MMIPICPREVIEW_TITLE_MARGIN             ((GUILCD_IsLandscape(0))?16:8) 
#define MMIPICPREVIEW_LIST_ICON_SIZE           54
#define MMIPICPREVIEW_LIST_ICON_MARGIN         2
#define MMIPICPREVIEW_TIME_BG_V_TOP            24
#define MMIPICPREVIEW_TIME_BG__RIGHT           176
#define MMIPICPREVIEW_TIME_HEIGHT              18
#define MMIPICPREVIEW_TITLE_FONT               MMI_DEFAULT_BIG_FONT
#define MMIPICVIEW_DELIMETER_MARGIN          2
#define MMIPICVIEW_TOTAL_NUM_TOP               6
#define MMIPICVIEW_TOTAL_NUM_HEIGHT            14
#define MMIPICVIEW_TOTAL_NUM_V_LEFT              140
#define MMIPICVIEW_TOTAL_NUM_V_RIGHT             175
#define MMIPICVIEW_TOTAL_NUM_H_LEFT              184
#define MMIPICVIEW_TOTAL_NUM_H_RIGHT             219
#else
#define MMIPICPREVIEW_TITLE_HEIGHT             40
#define MMIPICPREVIEW_BOTTOM_MENU_HEIGHT       40
#define MMIPICPREVIEW_TITLE_MARGIN             ((GUILCD_IsLandscape(0))?16:8) 
#ifdef BAIDU_DRIVE_SUPPORT
#if defined (ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
#define MMIPICPREVIEW_LIST_ICON_SIZE           90//170
#define MMIPICPREVIEW_LIST_ICON_MARGIN         5//6 
#else
#define MMIPICPREVIEW_LIST_ICON_SIZE           110
#define MMIPICPREVIEW_LIST_ICON_MARGIN         6 
#endif
#else
#define MMIPICPREVIEW_LIST_ICON_SIZE           54
#define MMIPICPREVIEW_LIST_ICON_MARGIN         4
#endif
#define MMIPICPREVIEW_TITLE_FONT               MMI_DEFAULT_BIG_FONT
#endif//MAINLCD_SIZE_320X480

#ifdef MAINLCD_SIZE_240X320
 //vertical
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_LEFT                          100
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_RIGHT                        220
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_TOP                            441
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_BOTTOM                     479

//horizontal
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_LEFT                          440
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_RIGHT                        479
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_TOP                            124
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_BOTTOM                     244

#elif defined MAINLCD_SIZE_240X400
 //vertical
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_LEFT                          60
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_RIGHT                        180
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_TOP                            361
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_BOTTOM                     399

//horizontal
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_LEFT                          361
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_RIGHT                        399
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_TOP                            81
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_BOTTOM                     201
#elif defined MAINLCD_SIZE_320X480
 //vertical
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_LEFT                          100
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_RIGHT                        220
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_TOP                            441
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_BOTTOM                     479

//horizontal
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_LEFT                          440
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_RIGHT                        479
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_TOP                            124
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_BOTTOM                     244

#else
 //vertical
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_LEFT                          100
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_RIGHT                        220
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_TOP                            441
#define PICVIEW_FOLDER_CAMERA_BUTTON_V_BOTTOM                     479

//horizontal
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_LEFT                          440
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_RIGHT                        479
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_TOP                            124
#define PICVIEW_FOLDER_CAMERA_BUTTON_H_BOTTOM                     244
#endif
//#endif //PIC_PREVIEW_U200_SUPPORT

#ifdef MAINLCD_SIZE_128X160
#define MMI_PIC_PREVIEW_WIDTH 128
#define MMI_PIC_PREVIEW_HEIGHT 160
#define MMI_PIC_PREVIEW_TOP_MARGIN 5
#define MMI_PIC_BTN_TWO_LEFT_RECT       DP2PX_RECT(15,60-12-6,15+12,60-6)
#define MMI_PIC_BTN_TWO_RIGHT_RECT      DP2PX_RECT(15+12+6,60-12-6,60-15,60-6)
#elif defined (MAINLCD_SIZE_240X320) || defined(MAINLCD_SIZE_240X240)
#define MMI_PIC_PREVIEW_WIDTH 200
#define MMI_PIC_PREVIEW_HEIGHT 200
#define MMI_PIC_PREVIEW_TOP_MARGIN 5
#define MMI_PIC_BTN_TWO_LEFT_RECT       DP2PX_RECT(15,60-12-6,15+12,60-6)
#define MMI_PIC_BTN_TWO_RIGHT_RECT      DP2PX_RECT(15+12+6,60-12-6,60-15,60-6)
#elif defined (MAINLCD_SIZE_360X360)
#define MMI_PIC_PREVIEW_WIDTH 360
#define MMI_PIC_PREVIEW_HEIGHT 360
#define MMI_PIC_PREVIEW_TOP_MARGIN 15
#define MMI_PIC_BTN_TWO_LEFT_RECT       DP2PX_RECT(60,240-48-24,60+48,240-24)
#define MMI_PIC_BTN_TWO_RIGHT_RECT      DP2PX_RECT(60+48+24,240-48-24,240-60,240-24)
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
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

#endif
#endif //#ifdef PIC_VIEWER_SUPPORT
