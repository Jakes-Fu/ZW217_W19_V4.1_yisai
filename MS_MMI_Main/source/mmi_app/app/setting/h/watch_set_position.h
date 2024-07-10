/*****************************************************************************
** File Name:      watch_set_position.h                                      *
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

#ifndef _WATCH_SET_POSITION_H_
#define _WATCH_SET_POSITION_H_

/*****************************************************************************/
//  Discription: settings postion macro define                               *
//  Global resource dependence:                                              *
//  Author: bin.wang1                                                        *
//  Note:                                                                    *
/*****************************************************************************/

#ifdef MAINLCD_DEV_SIZE_128X160
#include "watch_set_position_128x160.h"
#elif (defined MAINLCD_DEV_SIZE_240X320) || (defined MAINLCD_DEV_SIZE_240X240)
#include "watch_set_position_240X320.h"
#else
#error
#endif

#endif
