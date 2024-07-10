/******************************************************************************
 ** File Name:      mmi_app_dropdownwin_trc.h                                         *
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
//trace_id:169
#ifndef _MMI_APP_DROPDOWNWIN_TRC_H_
#define _MMI_APP_DROPDOWNWIN_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIDROPDOWNWIN_MAIN_1070_112_2_18_2_14_50_0 "CheckNotifyList() i = %d, notify_type = %d, valid = %d"
#define MMIDROPDOWNWIN_MAIN_1656_112_2_18_2_14_51_1 "SetDropdownLayerPos() error input param!"
#define MMIDROPDOWNWIN_MAIN_2081_112_2_18_2_14_52_2 "== Add wre running apps: %d app info into dropdown win \n=="
#define MMIDROPDOWNWIN_MAIN_2101_112_2_18_2_14_52_3 "too much running wre apps!!!\n"
#define MMIDROPDOWNWIN_MAIN_2107_112_2_18_2_14_52_4 "too much running wre apps!!!\n"
#define MMIDROPDOWNWIN_MAIN_2346_112_2_18_2_14_53_5 "HandleDropDownWinMsg, MSG_CTL_PIANT_DONE"
#define MMIDROPDOWNWIN_MAIN_3988_112_2_18_2_14_56_6 "HandleDropDownTPMove"
#define MMIDROPDOWNWIN_MAIN_4066_112_2_18_2_14_56_7 "HandleDropDownTPUp"
#define MMIDROPDOWNWIN_MAIN_5029_112_2_18_2_14_58_8 "MMIASP this number has find a name "
#define MMIDROPDOWNWIN_MAIN_5080_112_2_18_2_14_58_9 "GetMissCallDetail call info alloc failed"
#define MMIDROPDOWNWIN_MAIN_5843_112_2_18_2_15_0_10 "GetNotifyNumber, num = %d"
#define MMIDROPDOWNWIN_MAIN_5861_112_2_18_2_15_0_11 "AddNotifyRecord, notify_type = %d"
#define MMIDROPDOWNWIN_MAIN_6010_112_2_18_2_15_0_12 "MMIDROPDOWNWIN_Create"
#define MMIDROPDOWNWIN_MAIN_6018_112_2_18_2_15_0_13 "McareV31_IsMcareRunning return TRUE"
#define MMIDROPDOWNWIN_MAIN_6021_112_2_18_2_15_0_14 "MAIN_IDLE_WIN_ID MMK_IsOpenWin TRUE"
#define MMIDROPDOWNWIN_MAIN_6158_112_2_18_2_15_0_15 "MMIDROPDOWNWIN_Close() is_show_anim = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_DROPDOWNWIN_TRC)
TRACE_MSG(MMIDROPDOWNWIN_MAIN_1070_112_2_18_2_14_50_0,"CheckNotifyList() i = %d, notify_type = %d, valid = %d")
TRACE_MSG(MMIDROPDOWNWIN_MAIN_1656_112_2_18_2_14_51_1,"SetDropdownLayerPos() error input param!")
TRACE_MSG(MMIDROPDOWNWIN_MAIN_2081_112_2_18_2_14_52_2,"== Add wre running apps: %d app info into dropdown win \n==")
TRACE_MSG(MMIDROPDOWNWIN_MAIN_2101_112_2_18_2_14_52_3,"too much running wre apps!!!\n")
TRACE_MSG(MMIDROPDOWNWIN_MAIN_2107_112_2_18_2_14_52_4,"too much running wre apps!!!\n")
TRACE_MSG(MMIDROPDOWNWIN_MAIN_2346_112_2_18_2_14_53_5,"HandleDropDownWinMsg, MSG_CTL_PIANT_DONE")
TRACE_MSG(MMIDROPDOWNWIN_MAIN_3988_112_2_18_2_14_56_6,"HandleDropDownTPMove")
TRACE_MSG(MMIDROPDOWNWIN_MAIN_4066_112_2_18_2_14_56_7,"HandleDropDownTPUp")
TRACE_MSG(MMIDROPDOWNWIN_MAIN_5029_112_2_18_2_14_58_8,"MMIASP this number has find a name ")
TRACE_MSG(MMIDROPDOWNWIN_MAIN_5080_112_2_18_2_14_58_9,"GetMissCallDetail call info alloc failed")
TRACE_MSG(MMIDROPDOWNWIN_MAIN_5843_112_2_18_2_15_0_10,"GetNotifyNumber, num = %d")
TRACE_MSG(MMIDROPDOWNWIN_MAIN_5861_112_2_18_2_15_0_11,"AddNotifyRecord, notify_type = %d")
TRACE_MSG(MMIDROPDOWNWIN_MAIN_6010_112_2_18_2_15_0_12,"MMIDROPDOWNWIN_Create")
TRACE_MSG(MMIDROPDOWNWIN_MAIN_6018_112_2_18_2_15_0_13,"McareV31_IsMcareRunning return TRUE")
TRACE_MSG(MMIDROPDOWNWIN_MAIN_6021_112_2_18_2_15_0_14,"MAIN_IDLE_WIN_ID MMK_IsOpenWin TRUE")
TRACE_MSG(MMIDROPDOWNWIN_MAIN_6158_112_2_18_2_15_0_15,"MMIDROPDOWNWIN_Close() is_show_anim = %d")
END_TRACE_MAP(MMI_APP_DROPDOWNWIN_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_DROPDOWNWIN_TRC_H_

