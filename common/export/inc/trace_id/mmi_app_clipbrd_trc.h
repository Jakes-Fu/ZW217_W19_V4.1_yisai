/******************************************************************************
 ** File Name:      mmi_app_clipbrd_trc.h                                         *
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
//trace_id:161
#ifndef _MMI_APP_CLIPBRD_TRC_H_
#define _MMI_APP_CLIPBRD_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMICLIPBRD_MAGNIFY_478_112_2_18_2_11_59_0 "== BlendManginfy == buf is null, 0x%x, 0x%x, 0x%x"
#define MMICLIPBRD_TOOLBAR_604_112_2_18_2_12_2_1 "== SendMenuMsgToCtrl == msg id %d is error"
#define MMICLIPBRD_WINTAB_314_112_2_18_2_12_4_2 "== SendMenuMsgToCtrl == msg id %d is error"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_CLIPBRD_TRC)
TRACE_MSG(MMICLIPBRD_MAGNIFY_478_112_2_18_2_11_59_0,"== BlendManginfy == buf is null, 0x%x, 0x%x, 0x%x")
TRACE_MSG(MMICLIPBRD_TOOLBAR_604_112_2_18_2_12_2_1,"== SendMenuMsgToCtrl == msg id %d is error")
TRACE_MSG(MMICLIPBRD_WINTAB_314_112_2_18_2_12_4_2,"== SendMenuMsgToCtrl == msg id %d is error")
END_TRACE_MAP(MMI_APP_CLIPBRD_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_CLIPBRD_TRC_H_

