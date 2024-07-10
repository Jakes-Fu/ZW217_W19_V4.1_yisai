/******************************************************************************
 ** File Name:      mmi_app_parse_trc.h                                         *
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
//trace_id:205
#ifndef _MMI_APP_PARSE_TRC_H_
#define _MMI_APP_PARSE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIPARSE_DIAL_651_112_2_18_2_42_56_0 "MMIAPIPARSE_ParseTeleNum: input num is 08"
#define MMIPARSE_DIAL_676_112_2_18_2_42_56_1 "MMIAPIPARSE_ParseTeleNum error: is_video_call=1 but VT_SUPPORT is not defined"
#define MMIPARSE_DIAL_696_112_2_18_2_42_57_2 "MMIAPIPARSE_ProcessImeiStr str_ptr is NULL"
#define MMIPARSE_DIAL_701_112_2_18_2_42_57_3 "MMIAPIPARSE_ProcessImeiStr str_len is not 5"
#define MMIPARSE_DIAL_722_112_2_18_2_42_57_4 "ParseRedialString str_ptr is NULL"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_PARSE_TRC)
TRACE_MSG(MMIPARSE_DIAL_651_112_2_18_2_42_56_0,"MMIAPIPARSE_ParseTeleNum: input num is 08")
TRACE_MSG(MMIPARSE_DIAL_676_112_2_18_2_42_56_1,"MMIAPIPARSE_ParseTeleNum error: is_video_call=1 but VT_SUPPORT is not defined")
TRACE_MSG(MMIPARSE_DIAL_696_112_2_18_2_42_57_2,"MMIAPIPARSE_ProcessImeiStr str_ptr is NULL")
TRACE_MSG(MMIPARSE_DIAL_701_112_2_18_2_42_57_3,"MMIAPIPARSE_ProcessImeiStr str_len is not 5")
TRACE_MSG(MMIPARSE_DIAL_722_112_2_18_2_42_57_4,"ParseRedialString str_ptr is NULL")
END_TRACE_MAP(MMI_APP_PARSE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_PARSE_TRC_H_

