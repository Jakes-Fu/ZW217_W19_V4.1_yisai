/******************************************************************************
 ** File Name:      mmi_app_ussd_trc.h                                         *
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
//trace_id:232
#ifndef _MMI_APP_USSD_TRC_H_
#define _MMI_APP_USSD_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIUSSD_389_112_2_18_3_4_14_0 "ReleaseUssdService has received service cnf"
#define MMIUSSD_398_112_2_18_3_4_14_1 "ReleaseUssdService s_ussd_status[%d]=%d"
#define MMIUSSD_405_112_2_18_3_4_14_2 "ReleaseUssdService ss_code=%d"
#define MMIUSSD_714_112_2_18_3_4_15_3 "mmiussd.c ParseUssdData: 0 == ussd_data_ptr->str_len"
#define MMIUSSD_731_112_2_18_3_4_15_4 "ParseUssdData data_code_scheme=0x%02x, str_len=%d"
#define MMIUSSD_754_112_2_18_3_4_15_5 "ParseUssdData ret_val=%d, len=%d"
#define MMIUSSD_903_112_2_18_3_4_15_6 "not user request! ussd cnf error%d"
#define MMIUSSD_921_112_2_18_3_4_15_7 "ussd cnf error%d"
#define MMIUSSD_1703_112_2_18_3_4_17_8 "MMIUSSD: MMIAPIUSSD_ParseUssdString ss_code=%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_USSD_TRC)
TRACE_MSG(MMIUSSD_389_112_2_18_3_4_14_0,"ReleaseUssdService has received service cnf")
TRACE_MSG(MMIUSSD_398_112_2_18_3_4_14_1,"ReleaseUssdService s_ussd_status[%d]=%d")
TRACE_MSG(MMIUSSD_405_112_2_18_3_4_14_2,"ReleaseUssdService ss_code=%d")
TRACE_MSG(MMIUSSD_714_112_2_18_3_4_15_3,"mmiussd.c ParseUssdData: 0 == ussd_data_ptr->str_len")
TRACE_MSG(MMIUSSD_731_112_2_18_3_4_15_4,"ParseUssdData data_code_scheme=0x%02x, str_len=%d")
TRACE_MSG(MMIUSSD_754_112_2_18_3_4_15_5,"ParseUssdData ret_val=%d, len=%d")
TRACE_MSG(MMIUSSD_903_112_2_18_3_4_15_6,"not user request! ussd cnf error%d")
TRACE_MSG(MMIUSSD_921_112_2_18_3_4_15_7,"ussd cnf error%d")
TRACE_MSG(MMIUSSD_1703_112_2_18_3_4_17_8,"MMIUSSD: MMIAPIUSSD_ParseUssdString ss_code=%d")
END_TRACE_MAP(MMI_APP_USSD_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_USSD_TRC_H_

