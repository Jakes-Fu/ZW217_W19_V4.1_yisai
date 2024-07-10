/******************************************************************************
 ** File Name:      mmi_app_taskmanager_trc.h                                         *
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
//trace_id:226
#ifndef _MMI_APP_TASKMANAGER_TRC_H_
#define _MMI_APP_TASKMANAGER_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMI_APPLET_MANAGER_595_112_2_18_2_56_22_0 "GetRunningTime: start_time =%d, tick_time.second =%d, "
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_TASKMANAGER_TRC)
TRACE_MSG(MMI_APPLET_MANAGER_595_112_2_18_2_56_22_0,"GetRunningTime: start_time =%d, tick_time.second =%d, ")
END_TRACE_MAP(MMI_APP_TASKMANAGER_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_TASKMANAGER_TRC_H_

