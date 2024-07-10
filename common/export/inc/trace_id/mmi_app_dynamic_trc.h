/******************************************************************************
 ** File Name:      mmi_app_dynamic_trc.h                                         *
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
//trace_id:170
#ifndef _MMI_APP_DYNAMIC_TRC_H_
#define _MMI_APP_DYNAMIC_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIDYNA_WIN_473_112_2_18_2_15_3_0 "MMIFDYNA: HandleDynaMyAppListOpt, win_id =%d, msg_id = %d."
#define MMIDYNA_WIN_1145_112_2_18_2_15_5_1 "[MOD MNG]: error! module code size is 0x%x"
#define MMIDYNA_WIN_1445_112_2_18_2_15_5_2 "[Szip]:file_name_len = %d, file_size = %d, zip_size = %d, reserved = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_DYNAMIC_TRC)
TRACE_MSG(MMIDYNA_WIN_473_112_2_18_2_15_3_0,"MMIFDYNA: HandleDynaMyAppListOpt, win_id =%d, msg_id = %d.")
TRACE_MSG(MMIDYNA_WIN_1145_112_2_18_2_15_5_1,"[MOD MNG]: error! module code size is 0x%x")
TRACE_MSG(MMIDYNA_WIN_1445_112_2_18_2_15_5_2,"[Szip]:file_name_len = %d, file_size = %d, zip_size = %d, reserved = %d")
END_TRACE_MAP(MMI_APP_DYNAMIC_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_DYNAMIC_TRC_H_

