/******************************************************************************
 ** File Name:      mmi_app_controlpanel_trc.h                                         *
 ** DATE:                                                       *
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
 **                    *
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
#ifndef _MMI_APP_CONTROLPANEL_TRC_H_
#define _MMI_APP_CONTROLPANEL_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMICP_APPHANDLEMSG_0 "[MMICP] AppHandleMsg msg_id=%d"
#define MMICP_HANDLEMAINWINMSG_0 "[MMICP] HandleMainWinMsg msg_id=%d"
#define MMICP_GETSIMSTATE_0 "[MMICP] GetSIMState sim_status=%d"
#define MMICP_GETBTSTATE_0 "[MMICP] GetBTState bt_state=%d"
#define MMICP_GETCURRENTFOUCS_0 "[MMICP] SetCurrentFoucs CURRENT_FOCUS=%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_CONTROLPANEL_TRC)
TRACE_MSG(MMICP_APPHANDLEMSG_0,"[MMICP] AppHandleMsg msg_id=%d")
TRACE_MSG(MMICP_HANDLEMAINWINMSG_0,"[MMICP] HandleMainWinMsg msg_id=%d")
TRACE_MSG(MMICP_GETSIMSTATE_0,"[MMICP] GetSIMState sim_status=%d")
TRACE_MSG(MMICP_GETBTSTATE_0,"[MMICP] GetBTState bt_state=%d")
TRACE_MSG(MMICP_GETCURRENTFOUCS_0,"[MMICP] SetCurrentFoucs CURRENT_FOCUS=%d")
END_TRACE_MAP(MMI_APP_CONTROLPANEL_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_CONTROLPANEL_TRC_H_

