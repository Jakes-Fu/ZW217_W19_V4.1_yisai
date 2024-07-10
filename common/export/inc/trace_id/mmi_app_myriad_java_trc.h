/******************************************************************************
 ** File Name:      mmi_app_myriad_java_trc.h                                         *
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
//trace_id:201
#ifndef _MMI_APP_MYRIAD_JAVA_TRC_H_
#define _MMI_APP_MYRIAD_JAVA_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIJAVA_EXPORT_220_112_2_18_2_41_44_0 "========Myriad LOG=====: in MMIAPIJAVA_PauseBackgroundJVM()"
#define MMIJAVA_EXPORT_227_112_2_18_2_41_44_1 "========Myriad LOG=====: in MMIAPIJAVA_ResumeBackgroundJVM()"
#define MMIJAVA_WINTAB_SETTING_1221_112_2_18_2_42_2_2 " ====myriad log: MMIAPIFMM_DeleteFile result (%d)"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_MYRIAD_JAVA_TRC)
TRACE_MSG(MMIJAVA_EXPORT_220_112_2_18_2_41_44_0,"========Myriad LOG=====: in MMIAPIJAVA_PauseBackgroundJVM()")
TRACE_MSG(MMIJAVA_EXPORT_227_112_2_18_2_41_44_1,"========Myriad LOG=====: in MMIAPIJAVA_ResumeBackgroundJVM()")
TRACE_MSG(MMIJAVA_WINTAB_SETTING_1221_112_2_18_2_42_2_2," ====myriad log: MMIAPIFMM_DeleteFile result (%d)")
END_TRACE_MAP(MMI_APP_MYRIAD_JAVA_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_MYRIAD_JAVA_TRC_H_

