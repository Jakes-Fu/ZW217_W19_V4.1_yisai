/******************************************************************************
 ** File Name:      mmi_app_apple_trc.h                                         *
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
//trace_id:246
#ifndef _MMI_APP_APPLE_TRC_H_
#define _MMI_APP_APPLE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define APPLE_EXPORT_131_112_2_18_12_21_51_0 "[WRE]RegisterDevice : RAM disk success"
#define APPLE_EXPORT_135_112_2_18_12_21_51_1 "[WRE]RegisterDevice: RAM disk fail, the format device"
#define APPLE_EXPORT_2412_112_2_18_12_21_56_2 "APPLE_OS-%d - %s"
#define APPLE_EXPORT_2459_112_2_18_12_21_56_3 "APPLE_APP(0x%x)-%d - %s"
#define APPLE_EXPORT_2480_112_2_18_12_21_56_4 "APPLE_LOG - %s"
#define APPLE_WINTAB_867_112_2_18_12_22_11_5 "[MMIDC] MMIAPIDC_OpenPhotoWin java running =%d"
#define APPLE_WINTAB_952_112_2_18_12_22_11_6 "[MMIDC] MMIAPIDC_OpenPhotoWin java running =%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_APPLE_TRC)
TRACE_MSG(APPLE_EXPORT_131_112_2_18_12_21_51_0,"[WRE]RegisterDevice : RAM disk success")
TRACE_MSG(APPLE_EXPORT_135_112_2_18_12_21_51_1,"[WRE]RegisterDevice: RAM disk fail, the format device")
TRACE_MSG(APPLE_EXPORT_2412_112_2_18_12_21_56_2,"APPLE_OS-%d - %s")
TRACE_MSG(APPLE_EXPORT_2459_112_2_18_12_21_56_3,"APPLE_APP(0x%x)-%d - %s")
TRACE_MSG(APPLE_EXPORT_2480_112_2_18_12_21_56_4,"APPLE_LOG - %s")
TRACE_MSG(APPLE_WINTAB_867_112_2_18_12_22_11_5,"[MMIDC] MMIAPIDC_OpenPhotoWin java running =%d")
TRACE_MSG(APPLE_WINTAB_952_112_2_18_12_22_11_6,"[MMIDC] MMIAPIDC_OpenPhotoWin java running =%d")
END_TRACE_MAP(MMI_APP_APPLE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_APPLE_TRC_H_

