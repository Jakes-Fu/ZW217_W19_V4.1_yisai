/******************************************************************************
 ** File Name:      mmi_app_msd_trc.h                                         *
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
//trace_id:199
#ifndef _MMI_APP_MSD_TRC_H_
#define _MMI_APP_MSD_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIMSD_220_112_2_18_2_40_55_0 "MMIMSD_IsSimLegal s_sim_legal[%d]=%d"
#define MMIMSD_226_112_2_18_2_40_55_1 "MMIMSD_IsSimLegal s_sim_legal[%d]=%d"
#define MMIMSD_576_112_2_18_2_40_56_2 "mmimsd.c enter SendMsdMsg dual_sys=%d"
#define MMIMSD_619_112_2_18_2_40_56_3 "mmimsd.c MMIMSD_SendMsdMsg send fail"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_MSD_TRC)
TRACE_MSG(MMIMSD_220_112_2_18_2_40_55_0,"MMIMSD_IsSimLegal s_sim_legal[%d]=%d")
TRACE_MSG(MMIMSD_226_112_2_18_2_40_55_1,"MMIMSD_IsSimLegal s_sim_legal[%d]=%d")
TRACE_MSG(MMIMSD_576_112_2_18_2_40_56_2,"mmimsd.c enter SendMsdMsg dual_sys=%d")
TRACE_MSG(MMIMSD_619_112_2_18_2_40_56_3,"mmimsd.c MMIMSD_SendMsdMsg send fail")
END_TRACE_MAP(MMI_APP_MSD_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_MSD_TRC_H_

