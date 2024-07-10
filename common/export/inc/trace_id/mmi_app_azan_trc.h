/******************************************************************************
 ** File Name:      mmi_app_azan_trc.h                                         *
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
//trace_id:154
#ifndef _MMI_APP_AZAN_TRC_H_
#define _MMI_APP_AZAN_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIAZAN_WINTAB_1801_112_2_18_2_5_31_0 "HandleAzanChooseOptWinMsg current_index=%d"
#define MMIAZAN_WINTAB_2865_112_2_18_2_5_33_1 "---MMIAPIAZAN_OpenAlarmExpiredWin()---"
#define MMIAZAN_WINTAB_3022_112_2_18_2_5_33_2 "---iCountryMaxLen:%d---"
#define MMIAZAN_WINTAB_3111_112_2_18_2_5_33_3 "---iCountryMaxLen:%d---"
#define MMIAZAN_WINTAB_3371_112_2_18_2_5_34_4 "MMIAZAN_CreateCalMethodListCtl Failed"
#define MMIAZAN_WINTAB_3991_112_2_18_2_5_35_5 "MMIAZAN_SetNextAlarmTimeByCurrent(): event_ptr->fre_mode is %d"
#define MMIAZAN_WINTAB_3998_112_2_18_2_5_35_6 "MMIAZAN_SetNextAlarmTimeByCurrent(), success, year = %d, month = %d, day = %d, hour = %d, minute = %d"
#define MMIAZAN_WINTAB_4001_112_2_18_2_5_35_7 "MMIAZAN_SetNextAlarmTimeByCurrent(), fail"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_AZAN_TRC)
TRACE_MSG(MMIAZAN_WINTAB_1801_112_2_18_2_5_31_0,"HandleAzanChooseOptWinMsg current_index=%d")
TRACE_MSG(MMIAZAN_WINTAB_2865_112_2_18_2_5_33_1,"---MMIAPIAZAN_OpenAlarmExpiredWin()---")
TRACE_MSG(MMIAZAN_WINTAB_3022_112_2_18_2_5_33_2,"---iCountryMaxLen:%d---")
TRACE_MSG(MMIAZAN_WINTAB_3111_112_2_18_2_5_33_3,"---iCountryMaxLen:%d---")
TRACE_MSG(MMIAZAN_WINTAB_3371_112_2_18_2_5_34_4,"MMIAZAN_CreateCalMethodListCtl Failed")
TRACE_MSG(MMIAZAN_WINTAB_3991_112_2_18_2_5_35_5,"MMIAZAN_SetNextAlarmTimeByCurrent(): event_ptr->fre_mode is %d")
TRACE_MSG(MMIAZAN_WINTAB_3998_112_2_18_2_5_35_6,"MMIAZAN_SetNextAlarmTimeByCurrent(), success, year = %d, month = %d, day = %d, hour = %d, minute = %d")
TRACE_MSG(MMIAZAN_WINTAB_4001_112_2_18_2_5_35_7,"MMIAZAN_SetNextAlarmTimeByCurrent(), fail")
END_TRACE_MAP(MMI_APP_AZAN_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_AZAN_TRC_H_

