/*****************************************************************************
** File Name:      watch_alarm_main.h      //alarm settings                  *
** Author:         qi.liu1                                                   *
** Date:           26/2/2021                                                 *
** Copyright:      All Rights Reserved.                                      *
** Description:  alarm list window                                           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2021      qi.liu1              Creat                                   *
******************************************************************************/

#ifndef _WATCH_ALARM_MAIN_H_
#define _WATCH_ALARM_MAIN_H_

#define WATCH_ALARM_DEFAULT_TITLE_X                   DP2PX_VALUE(12)
#define WATCH_ALARM_DEFAULT_TITLE_Y                   DP2PX_VALUE(12)
#define WATCH_ALARM_DEFAULT_TITLE_H                   DP2PX_VALUE(30)
#define WATCH_ALARM_DEFAULT_TITLE_W                   DP2PX_VALUE(240-12*2)

#define WATCH_ALARM_DEFAULT_ICON_X                    DP2PX_VALUE((240-112)/2)
#define WATCH_ALARM_DEFAULT_ICON_Y                    DP2PX_VALUE(12+24+8)
#define WATCH_ALARM_DEFAULT_ICON_H                    DP2PX_VALUE(112)
#define WATCH_ALARM_DEFAULT_ICON_W                    DP2PX_VALUE(112)

#ifdef ADULT_WATCH_SUPPORT
#define WATCH_ALARM_NO_ALRRM_ICON_X             DP2PX_VALUE(59)
#define WATCH_ALARM_NO_ALRRM_ICON_Y             DP2PX_VALUE(57)
#define WATCH_ALARM_NO_ALRRM_ICON_W             DP2PX_VALUE(120)
#define WATCH_ALARM_NO_ALRRM_ICON_H             DP2PX_VALUE(120)

#define WATCH_ALARM_LIST_ITEM_H                 DP2PX_VALUE(80)

#define WATCH_ALARM_LIST_X                      (0)
#define WATCH_ALARM_LIST_Y                      DP2PX_VALUE(48)
#define WATCH_ALARM_LIST_W                      DP2PX_VALUE(240)
#define WATCH_ALARM_LIST_H                      DP2PX_VALUE(142)//160 128

#define WATCH_ALARM_EDIT_X                      (0)
#define WATCH_ALARM_EDIT_Y                      DP2PX_VALUE(48)
#define WATCH_ALARM_EDIT_W                      DP2PX_VALUE(240)
#define WATCH_ALARM_EDIT_H                      DP2PX_VALUE(48*3)
#define WATCH_ALARM_ITEM_H                      DP2PX_VALUE(48)//52

#define WATCH_ALARM_BTN_RECT                    DP2PX_RECT(88,192,151,239)//179
#define WATCHCOM_LISTITEM_DRAW_2STR_1ICON_2LINE AdultWatchCOM_ListItem_Draw_2Str_1Icon_2line
#else
#define WATCH_ALARM_NO_ALRRM_ICON_X             DP2PX_VALUE(66)
#define WATCH_ALARM_NO_ALRRM_ICON_Y             DP2PX_VALUE(36)
#define WATCH_ALARM_NO_ALRRM_ICON_W             DP2PX_VALUE(108)
#define WATCH_ALARM_NO_ALRRM_ICON_H             DP2PX_VALUE(108)

#define WATCH_ALARM_LIST_ITEM_H                 DP2PX_VALUE(60)

#define WATCH_ALARM_LIST_X                      (0)
#define WATCH_ALARM_LIST_Y                      DP2PX_VALUE(30)
#define WATCH_ALARM_LIST_W                      DP2PX_VALUE(239)
#define WATCH_ALARM_LIST_H                      DP2PX_VALUE(135)

#define WATCH_ALARM_EDIT_X                      (0)
#define WATCH_ALARM_EDIT_Y                      DP2PX_VALUE(30)
#define WATCH_ALARM_EDIT_W                      DP2PX_VALUE(240)
#define WATCH_ALARM_EDIT_H                      DP2PX_VALUE(45*3)
#define WATCH_ALARM_ITEM_H                      DP2PX_VALUE(45)

//#define WATCH_ALARM_BTN_RECT                    DP2PX_RECT(95,167,143,215)
#define WATCH_ALARM_BTN_RECT                    DP2PX_RECT(0,167,239,215)

#define WATCHCOM_LISTITEM_DRAW_2STR_1ICON_2LINE WatchCOM_ListItem_Draw_2Str_1Icon
#endif
#define WATCH_ALARM_TEMP_STR_LEN                (32)
#define WATCH_ALARM_REPEAT_DATE_STR_LEN         (52)
/*****************************************************************************/
//  Description : WatchAlarm_MainWin_Enter
//  Parameter: [In] None
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void WatchAlarm_MainWin_Enter( void );

#endif


#ifdef ADULT_WATCH_SUPPORT
#define ALARM_CHECK_LIST_STYLE  GUIITEM_STYLE_2STR_1ICON_2LINE_ADULTWATCH
#define ALARM_IS_ADD_BUTTON_VISIBLE TRUE
#define ALARM_ADD_BUTTON_TYPE GUI_BG_IMG
#define ALARM_ADD_BUTTON_IMAGE res_aw_alarm_ic_none
#else
#define ALARM_CHECK_LIST_STYLE  GUIITEM_STYLE_2STR_1ICON_2LINE_LAYOUT1
#define ALARM_IS_ADD_BUTTON_VISIBLE FALSE
#define ALARM_ADD_BUTTON_TYPE GUI_BG_NONE
#define ALARM_ADD_BUTTON_IMAGE 0
#endif
