/*****************************************************************************
/*****************************************************************************
** Copyright 2022 <Name of Copyright Owner: chaonan.hao>                     *
** Licensed under the Unisoc General Software License,                       *
** version 1.0 (the License);                                                *
** you may not use this file except in compliance with the License.          *
** You may obtain a copy of the License at                                   *
** https://www.unisoc.com/en_us/license/UNISOC_GENERAL_LICENSE_V1.0-EN_US    *
** Software distributed under the License is distributed on an "AS IS" BASIS,*
** WITHOUT WARRANTIES OF ANY KIND, either express or implied.                *
** See the Unisoc General Software License, version 1.0 for more details.    *
******************************************************************************/
/*****************************************************************************
** File Name:      mmi_position_listitem.h                                   *
** Author:                                                                   *
** Date:           11/1/2022                                                 *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME                   DESCRIPTION                         *
** 11/1/2022      chaonan.hao             Creat
******************************************************************************/

#ifndef _MMI_POSITION_LISTITEM_H_
#define _MMI_POSITION_LISTITEM_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/


//============================================================================//
//                                   For watch                                //
//============================================================================//

//common list item style
#define WATCHLIST_ITEM_HEIGH                          DP2PX_VALUE(60)
#define WATCHLIST_LEFT_SPACE                         DP2PX_VALUE(15) //DP2PX_VALUE(10)
#define WATCHLIST_LEFT_SPACE_BIG                      DP2PX_VALUE(56)
#define WATCHLIST_RIGHT_SPACE                         DP2PX_VALUE(230)//screen_width-WATCHLIST_LEFT_SPACE
#define WATCHLIST_1LINE_TOP_SPACE                     DP2PX_VALUE(19)
#define WATCHLIST_2LINE_TOP_SPACE                     DP2PX_VALUE(9)
#define WATCHLIST_FIRST_LINE_HEIGH                    DP2PX_VALUE(22)
#define WATCHLIST_SECOND_LINE_HEIGH                   DP2PX_VALUE(16)
#define WATCHLIST_TEXT_OFFESET                        DP2PX_VALUE(4)
#define WATCHLIST_ICON_HEIGH_SMALL                    DP2PX_VALUE(32)
#define WATCHLIST_ICON_HEIGH                          DP2PX_VALUE(46)
#define WATCHLIST_ICON_WIDGH                          DP2PX_VALUE(48)
#define WATCHLIST_ICON_AND_TEXT_OFFESET               DP2PX_VALUE(8)
#define WATCHLIST_TEXT_WIDGH_SPACE_SMALL              DP2PX_VALUE(80)
#define WATCHLIST_TEXT_WIDGH_SPACE_MIDDLE             DP2PX_VALUE(96)
#define WATCHLIST_TEXT_WIDGH_SPACE                    DP2PX_VALUE(124)
#define WATCHLIST_TEXT_AND_ICON_OFFESET               DP2PX_VALUE(8)
#define WATCHLIST_TOP_OFFESET                         DP2PX_VALUE(3)

//LIST_1STR_1LINE
#define LIST_1STR_1LINE_ITEM_HEIGH                    WATCHLIST_ITEM_HEIGH
#define LIST_1STR_1LINE_TOP                           WATCHLIST_1LINE_TOP_SPACE
#define LIST_1STR_1LINE_LEFT                          (WATCHLIST_LEFT_SPACE+8)
#define LIST_1STR_1LINE_RIGHT                         WATCHLIST_RIGHT_SPACE

//LIST_1ICON_1STR_1LINE
#define LIST_1ICON_1STR_1LINE_ITEM_HEIGH              WATCHLIST_ITEM_HEIGH
#define LIST_1ICON_1STR_1LINE_LEFT                    WATCHLIST_LEFT_SPACE
#define LIST_1ICON_1STR_1LINE_ICON_HEIGH              WATCHLIST_ICON_HEIGH
#define LIST_1ICON_1STR_1LINE_ICON_AND_TEXT_OFFESET   WATCHLIST_ICON_AND_TEXT_OFFESET
#define LIST_1ICON_1STR_1LINE_TOP                     WATCHLIST_1LINE_TOP_SPACE
#define LIST_1ICON_1STR_1LINE_RIGHT                   WATCHLIST_RIGHT_SPACE

//LIST_1STR_1ICON_1LINE
#define LIST_1STR_1ICON_1LINE_ITEM_HEIGH              WATCHLIST_ITEM_HEIGH
#define LIST_1STR_1ICON_1LINE_LEFT                    WATCHLIST_LEFT_SPACE
#define LIST_1STR_1ICON_1LINE_TOP                     WATCHLIST_1LINE_TOP_SPACE
#define LIST_1STR_1ICON_1LINE_TEXT_WIDGH              WATCHLIST_TEXT_WIDGH_SPACE
#define LIST_1STR_1ICON_1LINE_TEXT_AND_ICON_OFFESET   WATCHLIST_TEXT_AND_ICON_OFFESET
#define LIST_1STR_1ICON_1LINE_ICON_HEIGH              WATCHLIST_ICON_HEIGH
#define LIST_1STR_1ICON_1LINE_RIGHT                   WATCHLIST_RIGHT_SPACE

//LIST_2STR_2LINE
#define LIST_2STR_2LINE_ITEM_HEIGH                    WATCHLIST_ITEM_HEIGH
#define LIST_2STR_2LINE_LEFT                          WATCHLIST_LEFT_SPACE
#define LIST_2STR_2LINE_TOP                           WATCHLIST_2LINE_TOP_SPACE
#define LIST_2STR_2LINE_RIGHT                         WATCHLIST_RIGHT_SPACE
#define LIST_2STR_2LINE_FIRST_LINE_HEIGH              WATCHLIST_FIRST_LINE_HEIGH
#define LIST_2STR_2LINE_SECOND_LINE_HEIGH             WATCHLIST_SECOND_LINE_HEIGH

//LIST_1STR_1LINE_RADIO
#define LIST_1STR_1LINE_RADIO_ITEM_HEIGH              WATCHLIST_ITEM_HEIGH
#define LIST_1STR_1LINE_RADIO_LEFT                    (WATCHLIST_LEFT_SPACE+8)
#define LIST_1STR_1LINE_RADIO_TOP                     WATCHLIST_1LINE_TOP_SPACE
#define LIST_1STR_1LINE_RADIO_RIGHT                   WATCHLIST_RIGHT_SPACE
#define LIST_1STR_1LINE_RADIO_TEXT_WIDGH              WATCHLIST_TEXT_WIDGH_SPACE

//LIST_1STR_1LINE_CHECK
#define LIST_1STR_1LINE_CHECK_ITEM_HEIGH              WATCHLIST_ITEM_HEIGH
#define LIST_1STR_1LINE_CHECK_LEFT                    WATCHLIST_LEFT_SPACE
#define LIST_1STR_1LINE_CHECK_TOP                     WATCHLIST_1LINE_TOP_SPACE
#define LIST_1STR_1LINE_CHECK_TEXT_WIDGH              WATCHLIST_TEXT_WIDGH_SPACE

//LIST_2STR_1ICON_2LINE
#define LIST_2STR_1ICON_2LINE_ITEM_HEIGH              WATCHLIST_ITEM_HEIGH
#define LIST_2STR_1ICON_2LINE_FIRST_LINE_HEIGH        WATCHLIST_FIRST_LINE_HEIGH
#define LIST_2STR_1ICON_2LINE_LEFT                    WATCHLIST_LEFT_SPACE
#define LIST_2STR_1ICON_2LINE_TOP                     WATCHLIST_2LINE_TOP_SPACE
#define LIST_2STR_1ICON_2LINE_RIGHT                   WATCHLIST_RIGHT_SPACE
#define LIST_2STR_1ICON_2LINE_TEXT_WIDGH              WATCHLIST_TEXT_WIDGH_SPACE
#define LIST_2STR_1ICON_2LINE_TEXT_OFFESET            WATCHLIST_TEXT_OFFESET
#define LIST_2STR_1ICON_2LINE_ICON_HEIGH              WATCHLIST_ICON_HEIGH

//LIST_3STR_2LINE
#define LIST_3STR_2LINE_ITEM_HEIGH                    WATCHLIST_ITEM_HEIGH
#define LIST_3STR_2LINE_FIRST_LINE_HEIGH              WATCHLIST_FIRST_LINE_HEIGH
#define LIST_3STR_2LINE_LEFT                          WATCHLIST_LEFT_SPACE
#define LIST_3STR_2LINE_TOP                           WATCHLIST_2LINE_TOP_SPACE
#define LIST_3STR_2LINE_RIGHT                         WATCHLIST_RIGHT_SPACE
#define LIST_3STR_2LINE_LEFT_TEXT_WIDGH               WATCHLIST_TEXT_WIDGH_SPACE
#define LIST_3STR_2LINE_LEFT_TEXT_OFFESET             WATCHLIST_TEXT_OFFESET
#define LIST_3STR_2LINE_TOP_OFFESET                   WATCHLIST_TOP_OFFESET

//LIST_1STR_1ICON_1STR_1LINE
#define LIST_1STR_1ICON_1STR_1LINE_ITEM_HEIGH         WATCHLIST_ITEM_HEIGH
#define LIST_1STR_1ICON_1STR_1LINE_LEFT               WATCHLIST_LEFT_SPACE_BIG
#define LIST_1STR_1ICON_1STR_1LINE_TOP                WATCHLIST_1LINE_TOP_SPACE
#define LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH         WATCHLIST_TEXT_WIDGH_SPACE_MIDDLE
#define LIST_1STR_1ICON_1STR_1LINE_ICON_HEIGH         WATCHLIST_ICON_HEIGH_SMALL
#define LIST_1STR_1ICON_1STR_1LINE_TEXT_WIDGH_SMALL   WATCHLIST_TEXT_WIDGH_SPACE_SMALL
#define LIST_1STR_1ICON_1STR_1LINE_ICON_WIDGH         WATCHLIST_ICON_WIDGH

//GUIITEM_STYLE_1ICON_1STR_1LINE_WITH_UNREAD_NUM
#define LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_TOP      (LIST_1ICON_1STR_1LINE_ITEM_HEIGH - LIST_1ICON_1STR_1LINE_ICON_HEIGH)/2+3
#define LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_LEFT     LIST_1ICON_1STR_1LINE_LEFT + LIST_1ICON_1STR_1LINE_ICON_HEIGH-14
#define LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_HEIGH    14
#define LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_WIDGH    14
#define LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_TEXT_LEFT LIST_1ICON_1STR_1LINE_ITEM_UNREAD_NUM_LEFT+3

//zmt add start
#define ZMT_LIST_LINE_WIDTH MMI_MAINSCREEN_WIDTH/6
#define ZMT_LIST_LINE_HIGHT MMI_MAINSCREEN_HEIGHT/10
#define ZMT_LIST_ITEM_HIGHT MMI_MAINSCREEN_HEIGHT/10
#define ZMT_LIST_ITEM_PANDDING 10
//zmt add end

//============================================================================//
//                                   For watch                                //
//============================================================================//



#endif
