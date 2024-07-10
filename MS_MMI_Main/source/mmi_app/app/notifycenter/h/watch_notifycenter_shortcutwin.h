/*****************************************************************************
** File Name:      watch_notifycenter_shortcutwin.h                          *
** Author:         longwei.qiao                                              *
** Date:           16/02/2020                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe gallery export interface    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2/2020       longwei.qiao         Create                                  *
******************************************************************************/
#ifndef WATCH_NOTIFYCENTER_SHORTCUTWIN_H
#define WATCH_NOTIFYCENTER_SHORTCUTWIN_H
/*****************************************************************************/
//  Description : enter Shortcut win
//  Paramter:
//          [In]:none
//          [Out]:none
//          [Return]:none
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void NotifyCenter_EnterShortcutWin(void);

/*****************************************************************************/
//  Description : close Shortcut win
//  Paramter:
//          [In]:none
//          [Out]:none
//          [Return]:none
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void NotifyCenter_CloseShortcutWin(void);
/*****************************************************************************/
//  Description : Send Msg To Shortcut Win
//  Paramter:
//          [In]:none
//          [Out]:none
//          [Return]:none
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN NotifyCenter_SendMsgToShortcutWin(MMI_MESSAGE_ID_E msg_id ,DPARAM param_ptr);
/*****************************************************************************/
//  Description : Register BT Service
//  Paramter:
//          [In]:none
//          [Out]:none
//          [Return]:none
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void NotifyCenter_RegisterBTService(void);
/*****************************************************************************/
//  Description : Register wifi Service
//  Paramter:
//          [In]:none
//          [Out]:none
//          [Return]:none
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void NotifyCenter_RegisterWIFIService(void);
/*****************************************************************************/
//  Description : Open or close fly mode
//  Paramter:
//          [In]:none
//          [Out]:none
//          [Return]:none
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN NotifyCenter_OpenOrCloseFlyMode(void);
/*****************************************************************************/
//  Description : Clear wait state for ps
//  Paramter:
//          [In]:none
//          [Out]:none
//          [Return]:none
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void NotifyCenter_ClearWaitStateForPs(void);
#endif
