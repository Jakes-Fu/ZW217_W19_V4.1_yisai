/******************************************************************************
 ** File Name:      mmi_app_fdn_trc.h                                         *
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
//trace_id:176
#ifndef _MMI_APP_FDN_TRC_H_
#define _MMI_APP_FDN_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIFDN_144_112_2_18_2_19_14_0 "HandleFDNPsMsg:receive APP_MN_FDN_SERVICE_IND"
#define MMIFDN_173_112_2_18_2_19_14_1 "HandleFDNServiceCnf  fdn_service_cnf is NULL"
#define MMIFDN_179_112_2_18_2_19_14_2 "HandleFDNServiceCnf:is_fdn_enable=%d, dual_sys=%d, oper_type=%d"
#define MMIFDN_183_112_2_18_2_19_14_3 "HandleFDNServiceCnf:error dual_sys=%d"
#define MMIFDN_279_112_2_18_2_19_15_4 "HandleFDNUpdateCnf  fdn_update_cnf_ptr is NULL"
#define MMIFDN_287_112_2_18_2_19_15_5 "HandleFDNUpdateCnf operate_type=%d, dual_sys=%d, is_ok=%d"
#define MMIFDN_291_112_2_18_2_19_15_6 "HandleFDNUpdateCnf:error dual_sys=%d"
#define MMIFDN_392_112_2_18_2_19_15_7 "MMIAPIFDN_HandleCallReadyInd  call_ready_param_ptr is NULL"
#define MMIFDN_404_112_2_18_2_19_15_8 "MMIAPIFDN_HandleCallReadyInd dual_sys=%d,is_fdn_enable=%d,is_adn_enable=%d,is_usim=%d"
#define MMIFDN_408_112_2_18_2_19_15_9 "MMIAPIFDN_HandleCallReadyInd:error dual_sys=%d"
#define MMIFDN_444_112_2_18_2_19_15_10 "MMIAPIFDN_InitFDNInfo  dual_sys is error"
#define MMIFDN_451_112_2_18_2_19_15_11 "MMIAPIFDN_InitFDNInfo is_valid=%d,max=%d, used=%d"
#define MMIFDN_465_112_2_18_2_19_15_12 "MMIAPIFDN_InitFDNInfo entry state alloc failed"
#define MMIFDN_473_112_2_18_2_19_15_13 "MMIAPIFDN_InitFDNInfo MMIPB_CONTACT_T alloc failed"
#define MMIFDN_507_112_2_18_2_19_15_14 "MMIAPIFDN_GetFDNContext  dual_sys is error"
#define MMIFDN_612_112_2_18_2_19_15_15 "MMIAPIFDN_GetFDNContext  dual_sys is error"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_FDN_TRC)
TRACE_MSG(MMIFDN_144_112_2_18_2_19_14_0,"HandleFDNPsMsg:receive APP_MN_FDN_SERVICE_IND")
TRACE_MSG(MMIFDN_173_112_2_18_2_19_14_1,"HandleFDNServiceCnf  fdn_service_cnf is NULL")
TRACE_MSG(MMIFDN_179_112_2_18_2_19_14_2,"HandleFDNServiceCnf:is_fdn_enable=%d, dual_sys=%d, oper_type=%d")
TRACE_MSG(MMIFDN_183_112_2_18_2_19_14_3,"HandleFDNServiceCnf:error dual_sys=%d")
TRACE_MSG(MMIFDN_279_112_2_18_2_19_15_4,"HandleFDNUpdateCnf  fdn_update_cnf_ptr is NULL")
TRACE_MSG(MMIFDN_287_112_2_18_2_19_15_5,"HandleFDNUpdateCnf operate_type=%d, dual_sys=%d, is_ok=%d")
TRACE_MSG(MMIFDN_291_112_2_18_2_19_15_6,"HandleFDNUpdateCnf:error dual_sys=%d")
TRACE_MSG(MMIFDN_392_112_2_18_2_19_15_7,"MMIAPIFDN_HandleCallReadyInd  call_ready_param_ptr is NULL")
TRACE_MSG(MMIFDN_404_112_2_18_2_19_15_8,"MMIAPIFDN_HandleCallReadyInd dual_sys=%d,is_fdn_enable=%d,is_adn_enable=%d,is_usim=%d")
TRACE_MSG(MMIFDN_408_112_2_18_2_19_15_9,"MMIAPIFDN_HandleCallReadyInd:error dual_sys=%d")
TRACE_MSG(MMIFDN_444_112_2_18_2_19_15_10,"MMIAPIFDN_InitFDNInfo  dual_sys is error")
TRACE_MSG(MMIFDN_451_112_2_18_2_19_15_11,"MMIAPIFDN_InitFDNInfo is_valid=%d,max=%d, used=%d")
TRACE_MSG(MMIFDN_465_112_2_18_2_19_15_12,"MMIAPIFDN_InitFDNInfo entry state alloc failed")
TRACE_MSG(MMIFDN_473_112_2_18_2_19_15_13,"MMIAPIFDN_InitFDNInfo MMIPB_CONTACT_T alloc failed")
TRACE_MSG(MMIFDN_507_112_2_18_2_19_15_14,"MMIAPIFDN_GetFDNContext  dual_sys is error")
TRACE_MSG(MMIFDN_612_112_2_18_2_19_15_15,"MMIAPIFDN_GetFDNContext  dual_sys is error")
END_TRACE_MAP(MMI_APP_FDN_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_FDN_TRC_H_

