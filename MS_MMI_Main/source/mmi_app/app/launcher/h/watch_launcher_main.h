/*****************************************************************************
** File Name:      watch_launcher_main.h                                                                          *
** Author:          zhikun.lv                                                                                               *
** Date:             02/03/2020                                                                                          *
** Copyright:      2020 Spreadtrum, Incoporated. All Rights Reserved.                                   *
** Description:    Watch Launcher Win                                                                               *
******************************************************************************
**                         Important Edit History                                                                        *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                    *
** 02/03/2020  zhikun.lv        Create                                                                               *
******************************************************************************/
#ifndef WATCH_LAUNCHER_MAIN_H
#define WATCH_LAUNCHER_MAIN_H

#include "watch_launcher_common.h"

#if defined(LAUNCHER_FOUR_APP_IN_PAGE) || defined(LAUNCHER_ALL_APP_IN_PAGE)
#include "launcher_id.h"
#include "launcher_image.h"
#include "launcher_text.h"
#endif


/*****************************************************************************/
//  Discription: open watch launcher window
//  Global resource dependence:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
PUBLIC void WatchLAUNCHER_Enter(void);



/*****************************************************************************/
//  Discription: process the common action of launcher
//  Global resource dependence:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E WatchLAUNCHER_HandleCommonWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );


/*****************************************************************************/
//  Discription: display indicator images
//  Global resource dependence:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
PUBLIC void WatchLAUNCHER_DisplayIndicator(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Discription: set charge page visible
//  Global resource dependence:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
PUBLIC void WatchLAUNCHER_SetChargePageVisible(BOOLEAN bVisible);
PUBLIC void MMIWatchPanel_Open(void);

PUBLIC void WatchOpen_IdleWin(void);

PUBLIC void WatchOpen_Panel_SelectWin(void);

PUBLIC void WatchLAUNCHER_MainMenuSwitch(MMI_WIN_ID_T win_id);

#endif
