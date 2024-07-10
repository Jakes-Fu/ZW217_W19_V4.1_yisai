/******************************************************************************
 ** File Name:      mmi_app_tencentmcare_trc.h                                         *
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
//trace_id:227
#ifndef _MMI_APP_TENCENTMCARE_TRC_H_
#define _MMI_APP_TENCENTMCARE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define AMR_RECORD_87_112_2_18_2_56_24_0 "123=%d"
#define AMR_RECORD_91_112_2_18_2_56_24_1 "456=%d"
#define AMR_RECORD_111_112_2_18_2_56_24_2 "AMR_Record_Test: start reoced"
#define AMR_RECORD_139_112_2_18_2_56_24_3 "AMR_Record_Test: stop reoced"
#define AMR_RECORD_147_112_2_18_2_56_24_4 "AMR_Recode_Stop file_size = %d"
#define AMR_RECORD_174_112_2_18_2_56_24_5 "M_MMIAUDIO_StartRecord size = %d, aud_result = %d"
#define AMR_RECORD_196_112_2_18_2_56_24_6 "[MMIAUDIO]:MMIAUDIO_EndRecord:result = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_TENCENTMCARE_TRC)
TRACE_MSG(AMR_RECORD_87_112_2_18_2_56_24_0,"123=%d")
TRACE_MSG(AMR_RECORD_91_112_2_18_2_56_24_1,"456=%d")
TRACE_MSG(AMR_RECORD_111_112_2_18_2_56_24_2,"AMR_Record_Test: start reoced")
TRACE_MSG(AMR_RECORD_139_112_2_18_2_56_24_3,"AMR_Record_Test: stop reoced")
TRACE_MSG(AMR_RECORD_147_112_2_18_2_56_24_4,"AMR_Recode_Stop file_size = %d")
TRACE_MSG(AMR_RECORD_174_112_2_18_2_56_24_5,"M_MMIAUDIO_StartRecord size = %d, aud_result = %d")
TRACE_MSG(AMR_RECORD_196_112_2_18_2_56_24_6,"[MMIAUDIO]:MMIAUDIO_EndRecord:result = %d")
END_TRACE_MAP(MMI_APP_TENCENTMCARE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_TENCENTMCARE_TRC_H_

