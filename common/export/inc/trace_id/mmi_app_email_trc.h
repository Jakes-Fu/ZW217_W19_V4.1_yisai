/******************************************************************************
 ** File Name:      mmi_app_email_trc.h                                         *
 ** DATE:           2012.03.18                                                *
 ** Copyright:      Copyright 2012 by Spreadtrum Communications,Inc.          *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **---------------------------------------------------------------------------*
 **  2012.03.18                         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//trace_id:172
#ifndef _MMI_APP_EMAIL_TRC_H_
#define _MMI_APP_EMAIL_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIEMAIL_EXPORT_341_112_2_18_2_16_13_0 "[MMIEMAIL]: HandleSIMPlugNotifyEmailFunc dual_sys=%d, notify_event=%d"
#define MMIEMAIL_EXPORT_576_112_2_18_2_16_14_1 "MMEMAIL+++++++++email is running"
#define MMIEMAIL_EXPORT_616_112_2_18_2_16_14_2 "[EMAIL]MMIAPIEMAIL_EditEmail"
#define MMIEMAIL_EXPORT_631_112_2_18_2_16_14_3 "[EMAIL]MMIAPIEMAIL_EditEmail, alloc memory fail!"
#define MMIEMAIL_UTILS_1215_112_2_18_2_16_57_4 "%s\n"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_EMAIL_TRC)
TRACE_MSG(MMIEMAIL_EXPORT_341_112_2_18_2_16_13_0,"[MMIEMAIL]: HandleSIMPlugNotifyEmailFunc dual_sys=%d, notify_event=%d")
TRACE_MSG(MMIEMAIL_EXPORT_576_112_2_18_2_16_14_1,"MMEMAIL+++++++++email is running")
TRACE_MSG(MMIEMAIL_EXPORT_616_112_2_18_2_16_14_2,"[EMAIL]MMIAPIEMAIL_EditEmail")
TRACE_MSG(MMIEMAIL_EXPORT_631_112_2_18_2_16_14_3,"[EMAIL]MMIAPIEMAIL_EditEmail, alloc memory fail!")
TRACE_MSG(MMIEMAIL_UTILS_1215_112_2_18_2_16_57_4,"%s\n")
END_TRACE_MAP(MMI_APP_EMAIL_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_EMAIL_TRC_H_

