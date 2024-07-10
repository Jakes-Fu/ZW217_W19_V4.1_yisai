/****************************************************************************
** File Name:      watch_sms_preset.h                                    *
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
#ifndef _WATCH_SMS_PRESETLIST_H_
#define _WATCH_SMS_PRESETLIST_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "sci_types.h"
#include "mmismsapp_interface.h"

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : to open the preset list window message
//  Parameter: Null
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void WatchSMS_PresetListWin_Enter(SmsSrvCallbackFunc send_call_back);


#endif
