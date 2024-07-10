/****************************************************************************
** File Name:      watch_sms_chat.h                                   *
** Author:         qi.liu1                                             *
** Date:           20/2/2020                                                *
** Copyright:      2020 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 02/2020      qi.liu1                  Create
** 
****************************************************************************/
#ifndef _WATCH_SMS_CHAT_H_
#define _WATCH_SMS_CHAT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "sci_types.h"
#include "mmisms_export.h"

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : enter SMS chat  win
//  Parameter: [In] cur_order_id
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void WatchSMS_ChatWin_Enter(MMISMS_ORDER_ID_T cur_order_id);

#endif
