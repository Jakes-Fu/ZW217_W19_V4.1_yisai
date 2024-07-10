/****************************************************************************
** File Name:      adultwatch_bt_win.h                                      *
** Author:         chunjuan.liang                                           *
** Date:           01/26/2021                                               *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the bluetooth display menu *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 01/26/2020    chunjuan.liang     Create                                  *
**                                                                          *
****************************************************************************/

#ifndef _ADULTWATCH_BT_WIN_H_
#define  _ADULTWATCH_BT_WIN_H_

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "ual_bt.h"

/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Discription: Open the blue tooth main menu win
//  Global resource dependence: none 
//  Author:chunjuan.liang
//  Note:
//
/*****************************************************************************/
PUBLIC void AdultBT_MainWin_Enter(void);
#ifdef ADULT_WATCH_SUPPORT
/****************************************************************************/
//  Description : BT_HandleDevMsg
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:处理ual返回给UI的消息
/****************************************************************************/
PUBLIC BOOLEAN BT_HandleDevMsg(ual_cms_msg_t param);
#endif
/****************************************************************************/
//  Description : get bt addr of headset
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:for deleting headset
/****************************************************************************/
PUBLIC void AdultWatch_GetRemoveHeadsetAddr(uint8 *p_headset_addr);

/****************************************************************************/
//  Description : process ps msg
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN AdultWatchBt_ProcessBtPsMsg(uint16 msg_id, const BT_MSG_T *msg_body_ptr);

#endif

