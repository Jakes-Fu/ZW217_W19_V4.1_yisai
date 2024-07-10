/******************************************************************************
 ** File Name:      mmi_app_browser_manager_trc.h                                         *
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
//trace_id:156
#ifndef _MMI_APP_BROWSER_MANAGER_TRC_H_
#define _MMI_APP_BROWSER_MANAGER_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIBROWSER_MANAGER_API_165_112_2_18_2_7_36_0 "MMIAPIBROWSER_Entry MMIBROWSER_TYPE_MAX = %d"
#define MMIBROWSER_MANAGER_API_549_112_2_18_2_7_36_1 "[MMIBM]MMIAPIBROWSER_IsActive:active_browser_type=%d"
#define MMIBROWSER_MANAGER_API_573_112_2_18_2_7_36_2 "[MMIBM]MMIAPIBROWSER_IsRunning:running_browser_type=%d"
#define MMIBROWSER_MANAGER_API_599_112_2_18_2_7_36_3 "[MMIBM]BMIsOtherRunning:cur_browser_type=%d,running_browser_type=%d"
#define MMIBROWSER_MANAGER_FUNC_444_112_2_18_2_7_38_4 "[BM]W BMHandleEmail:email_subject_ptr alloc fail"
#define MMIBROWSER_MANAGER_FUNC_453_112_2_18_2_7_38_5 "[BM]W BMHandleEmail:email_body_ptr alloc fail"
#define MMIBROWSER_MANAGER_FUNC_489_112_2_18_2_7_38_6 "[BM]BMHandleEmail:subject_len = %d"
#define MMIBROWSER_MANAGER_FUNC_508_112_2_18_2_7_38_7 "[BM]BMHandleEmail:body_len = %d"
#define MMIBROWSER_MANAGER_FUNC_534_112_2_18_2_7_38_8 "[BM]BMHandleEmail:temp_ptr ALLOC Fail"
#define MMIBROWSER_MANAGER_FUNC_559_112_2_18_2_7_38_9 "MMIAPIBROWSER_HandleExScheme: in_url =%s"
#define MMIBROWSER_MANAGER_FUNC_569_112_2_18_2_7_38_10 "MMIAPIBROWSER_HandleExScheme: in_url empty"
#define MMIBROWSER_MANAGER_FUNC_576_112_2_18_2_7_38_11 "MMIAPIBROWSER_HandleExScheme: Err No memory "
#define MMIBROWSER_MANAGER_FUNC_655_112_2_18_2_7_38_12 "[BM]MMIBM_HandleAppMsg,msg_id=%d"
#define MMIBROWSER_MANAGER_FUNC_689_112_2_18_2_7_38_13 "[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x"
#define MMIBROWSER_MANAGER_FUNC_708_112_2_18_2_7_38_14 "[BM]W MMIBM_HandleAppMsg: Err No memory"
#define MMIBROWSER_MANAGER_FUNC_734_112_2_18_2_7_38_15 "[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x"
#define MMIBROWSER_MANAGER_FUNC_759_112_2_18_2_7_38_16 "[BM]W MMIBM_HandleAppMsg: Err No memory"
#define MMIBROWSER_MANAGER_FUNC_791_112_2_18_2_7_38_17 "[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x"
#define MMIBROWSER_MANAGER_FUNC_817_112_2_18_2_7_39_18 "[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x"
#define MMIBROWSER_MANAGER_FUNC_848_112_2_18_2_7_39_19 "MMIAPIBROWSER_HandleExScheme: mms alloc fail!"
#define MMIBROWSER_MANAGER_FUNC_863_112_2_18_2_7_39_20 "[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x"
#define MMIBROWSER_MANAGER_FUNC_897_112_2_18_2_7_39_21 "MMIAPIBROWSER_HandleExScheme: mms alloc fail!"
#define MMIBROWSER_MANAGER_FUNC_912_112_2_18_2_7_39_22 "[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x"
#define MMIBROWSER_MANAGER_WINTABLE_484_112_2_18_2_7_40_23 "[BM]MMIBM_OpenCCSelWin error1"
#define MMIBROWSER_MANAGER_WINTABLE_490_112_2_18_2_7_40_24 "[BM]MMIBM_OpenCCSelWin invalid number"
#define MMIBROWSER_MANAGER_WINTABLE_515_112_2_18_2_7_40_25 "[BM]mmi cc enter selectwin  %s"
#define MMIBROWSER_MANAGER_WINTABLE_813_112_2_18_2_7_41_26 "[BM]HandleSelectNetworkWinMsg sys_state[%d] = MMI_DUAL_SYS_MAX"
#define MMIBROWSER_MANAGER_WINTABLE_857_112_2_18_2_7_41_27 "HandleOpenBrowserWaitingWin, msg_id = 0x%x"
#define MMIBROWSER_MANAGER_WINTABLE_887_112_2_18_2_7_41_28 "HandleOpenBrowserWaitingWin, msg_id = 0x%x end"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_BROWSER_MANAGER_TRC)
TRACE_MSG(MMIBROWSER_MANAGER_API_165_112_2_18_2_7_36_0,"MMIAPIBROWSER_Entry MMIBROWSER_TYPE_MAX = %d")
TRACE_MSG(MMIBROWSER_MANAGER_API_549_112_2_18_2_7_36_1,"[MMIBM]MMIAPIBROWSER_IsActive:active_browser_type=%d")
TRACE_MSG(MMIBROWSER_MANAGER_API_573_112_2_18_2_7_36_2,"[MMIBM]MMIAPIBROWSER_IsRunning:running_browser_type=%d")
TRACE_MSG(MMIBROWSER_MANAGER_API_599_112_2_18_2_7_36_3,"[MMIBM]BMIsOtherRunning:cur_browser_type=%d,running_browser_type=%d")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_444_112_2_18_2_7_38_4,"[BM]W BMHandleEmail:email_subject_ptr alloc fail")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_453_112_2_18_2_7_38_5,"[BM]W BMHandleEmail:email_body_ptr alloc fail")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_489_112_2_18_2_7_38_6,"[BM]BMHandleEmail:subject_len = %d")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_508_112_2_18_2_7_38_7,"[BM]BMHandleEmail:body_len = %d")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_534_112_2_18_2_7_38_8,"[BM]BMHandleEmail:temp_ptr ALLOC Fail")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_559_112_2_18_2_7_38_9,"MMIAPIBROWSER_HandleExScheme: in_url =%s")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_569_112_2_18_2_7_38_10,"MMIAPIBROWSER_HandleExScheme: in_url empty")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_576_112_2_18_2_7_38_11,"MMIAPIBROWSER_HandleExScheme: Err No memory ")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_655_112_2_18_2_7_38_12,"[BM]MMIBM_HandleAppMsg,msg_id=%d")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_689_112_2_18_2_7_38_13,"[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_708_112_2_18_2_7_38_14,"[BM]W MMIBM_HandleAppMsg: Err No memory")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_734_112_2_18_2_7_38_15,"[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_759_112_2_18_2_7_38_16,"[BM]W MMIBM_HandleAppMsg: Err No memory")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_791_112_2_18_2_7_38_17,"[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_817_112_2_18_2_7_39_18,"[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_848_112_2_18_2_7_39_19,"MMIAPIBROWSER_HandleExScheme: mms alloc fail!")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_863_112_2_18_2_7_39_20,"[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_897_112_2_18_2_7_39_21,"MMIAPIBROWSER_HandleExScheme: mms alloc fail!")
TRACE_MSG(MMIBROWSER_MANAGER_FUNC_912_112_2_18_2_7_39_22,"[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x")
TRACE_MSG(MMIBROWSER_MANAGER_WINTABLE_484_112_2_18_2_7_40_23,"[BM]MMIBM_OpenCCSelWin error1")
TRACE_MSG(MMIBROWSER_MANAGER_WINTABLE_490_112_2_18_2_7_40_24,"[BM]MMIBM_OpenCCSelWin invalid number")
TRACE_MSG(MMIBROWSER_MANAGER_WINTABLE_515_112_2_18_2_7_40_25,"[BM]mmi cc enter selectwin  %s")
TRACE_MSG(MMIBROWSER_MANAGER_WINTABLE_813_112_2_18_2_7_41_26,"[BM]HandleSelectNetworkWinMsg sys_state[%d] = MMI_DUAL_SYS_MAX")
TRACE_MSG(MMIBROWSER_MANAGER_WINTABLE_857_112_2_18_2_7_41_27,"HandleOpenBrowserWaitingWin, msg_id = 0x%x")
TRACE_MSG(MMIBROWSER_MANAGER_WINTABLE_887_112_2_18_2_7_41_28,"HandleOpenBrowserWaitingWin, msg_id = 0x%x end")
END_TRACE_MAP(MMI_APP_BROWSER_MANAGER_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_BROWSER_MANAGER_TRC_H_

