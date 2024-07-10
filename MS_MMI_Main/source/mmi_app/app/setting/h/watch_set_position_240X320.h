/*****************************************************************************
** File Name:      watch_set_position_240X320.h                              *
** Author:         bin.wang1                                                 *
** Date:           02/26/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:    Position marco                                            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2020      bin.wang1              Creat                                 *
******************************************************************************/

#ifndef _WATCH_SET_POSITION_240X320_H_
#define _WATCH_SET_POSITION_240X320_H_

/*****************************************************************************/
//  Discription: settings postion macro define                               *
//  Global resource dependence:                                              *
//  Author: bin.wang1                                                        *
//  Note:                                                                    *
/*****************************************************************************/


// brightness, multi-media sound, note sound
#ifdef SCREEN_SHAPE_CIRCULAR
#define WATCH_SET_DEFAULT_TITLE_X                   (35)
#define WATCH_SET_DEFAULT_TITLE_Y                   (0)
#define WATCH_SET_DEFAULT_TITLE_H                   (60)
#define WATCH_SET_DEFAULT_TITLE_W                   (168)

#define WATCH_SET_DEFAULT_ICON_X                    (66)//(70)
#define WATCH_SET_DEFAULT_ICON_Y                    (54)//(60)
#define WATCH_SET_DEFAULT_ICON_H                    (72)//(98)
#define WATCH_SET_DEFAULT_ICON_W                    (108)//(98)

#define WATCH_SET_BRIGHT_ICON_X                    (104)//(70)
#define WATCH_SET_BRIGHT_ICON_Y                    (165)//(60)
//#define WATCH_SET_BRIGHT_ICON_H                    (72)//(98)
//#define WATCH_SET_BRIGHT_ICON_W                    (108)//(98)
#else
#define WATCH_SET_DEFAULT_TITLE_X                   (12)
#define WATCH_SET_DEFAULT_TITLE_Y                   (12)
#define WATCH_SET_DEFAULT_TITLE_H                   (30)
#define WATCH_SET_DEFAULT_TITLE_W                   (240-12*2)

#define WATCH_SET_DEFAULT_ICON_X                    ((240-112)/2)
#define WATCH_SET_DEFAULT_ICON_Y                    (12+24+8)
#define WATCH_SET_DEFAULT_ICON_H                    (112)
#define WATCH_SET_DEFAULT_ICON_W                    (112)
#endif
#define WATCH_SET_ALARM_NO_ALRRM_ICON_X             ((240-76)/2)
#define WATCH_SET_ALARM_NO_ALRRM_ICON_Y             (56)
#define WATCH_SET_ALARM_NO_ALRRM_ICON_W             (76)
#define WATCH_SET_ALARM_NO_ALRRM_ICON_H             (76)

#define WATCH_SET_ALARM_NO_ALRRM_LABEL_X            (0)
#define WATCH_SET_ALARM_NO_ALRRM_LABEL_Y            (56+76+8)
#define WATCH_SET_ALARM_NO_ALRRM_LABEL_W            (240)
#define WATCH_SET_ALARM_NO_ALRRM_LABEL_H            (16)

#define WATCH_SET_ALARM_LIST_ITEM_H                 (80)

#define WATCH_SET_ALARM_LIST_X                      (0)
#define WATCH_SET_ALARM_LIST_Y                      (0)
#define WATCH_SET_ALARM_LIST_W                      (240)
#define WATCH_SET_ALARM_LIST_H                      (160)

#define WATCH_SET_ALARM_EDIT_X                      (0)
#define WATCH_SET_ALARM_EDIT_Y                      (0)
#define WATCH_SET_ALARM_EDIT_W                      (240)
#define WATCH_SET_ALARM_EDIT_H                      (52*3)
#define WATCH_SET_ALARM_ITEM_H                      (52)
#endif
