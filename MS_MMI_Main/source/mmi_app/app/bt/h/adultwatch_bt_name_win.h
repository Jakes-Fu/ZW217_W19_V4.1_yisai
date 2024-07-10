/*****************************************************************************
** File Name:      adultwatch_bt_name_win.h                                  *
** Author:         xiaotong.su                                               *
** Date:           04/16/2021                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2021      xiaotong.su            Creat                                 *
******************************************************************************/

#ifndef _ADULTWATCH_BT_NAME_WIN_H_
#define _ADULTWATCH_BT_NAME_WIN_H_

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define BT_LOCAL_NAME_LEN  18//BLEÖÐname len=40,classic name len =41
                             // Bug 1904532

/****************************************************************************/
//  Description : enter BT device name window
//  Global resource dependence :
//  Author:xiaotong.su
//  Note:
/****************************************************************************/
PUBLIC void Bt_NameWin_Enter(void);

#endif
