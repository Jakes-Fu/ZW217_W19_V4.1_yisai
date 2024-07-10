/*****************************************************************************
** File Name:      watch_launcher_editclockwin.h                                                                *
** Author:                                                                                                                     *
** Date:             02/06/2020                                                                                          *
** Copyright:      2020 Spreadtrum, Incoporated. All Rights Reserved.                                   *
** Description:    This file is edit clock type window
*****************************************************************************
**                         Important Edit History                                                                        *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                    *
** 02/06/2020  zhikun.lv        Create
******************************************************************************/

#ifndef WATCH_LAUNCHER_EDITCLOCKWIN_H
#define WATCH_LAUNCHER_EDITCLOCKWIN_H

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#if (!defined ADULT_WATCH_SUPPORT) && (defined SCREEN_SHAPE_CIRCULAR)
#define LAUNCHER_EDITCLOCKWIN_BG_RECT DP2PX_RECT(0, 0, 240, 240)
#define LAUNCHER_EDITCLOCKWIN_RECT DP2PX_RECT(30, 30, 210, 210)
#else
#define LAUNCHER_EDITCLOCKWIN_RECT DP2PX_RECT(48, 48, 192, 192)
#endif

#define LAUNCHER_EDITCLOCKWIN_NUM 4
#define LAUNCHER_EDITCLOCKWIN_LEFTBTNRECT DP2PX_RECT(0, 108, 24, 132)
#define LAUNCHER_EDITCLOCKWIN_RIGHTBTNRECT DP2PX_RECT(216, 108, 240, 132)

/*****************************************************************************
//  Description : open selectclocktype window
//  Author: zhikun.lv
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchLAUNCHER_EditClockWin_Enter();
#endif
