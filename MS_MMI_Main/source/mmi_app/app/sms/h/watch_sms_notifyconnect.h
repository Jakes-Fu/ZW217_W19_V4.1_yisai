/*************************************************************************
 ** File Name:      mmisms_receive.h                                      *
 ** Author:         liming.deng                                          *
 ** Date:           2007/8/23                                            *
 ** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about receiving sms    *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2012/07/12      liming.deng      Create.                              *
*************************************************************************/
#ifndef _WATCH_SMS_NOTIFYCONNECT_H_
#define _WATCH_SMS_NOTIFYCONNECT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_default.h"
#include "mmismsapp_interface.h"

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : WatchSMS_Notification_Init
//  Parameter: [In] None
//             [In] None
//             [Out] None
//             [Return] handle
//  Author: qi.liu1
//  Note:初始化,监听新短信消息
/*****************************************************************************/
PUBLIC void WatchSMS_Notification_Init(void);

/*****************************************************************************/
//  Description : MotifySMSNotifyHandle
//  Parameter: [In] new_order_id
//             [Out] finded_order_id
//             [Return]
//  Author: qi.liu1
//  Note:  通过cur_order_id获取notify_handle
/*****************************************************************************/
PUBLIC uint32 WatchSMS_GetNotifyHandleByOrderID(MMISMS_ORDER_ID_T  new_order_id);

#endif
