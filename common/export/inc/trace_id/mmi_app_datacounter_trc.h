/******************************************************************************
 ** File Name:      mmi_app_datacounter_trc.h                                         *
 ** DATE:           2015.09.15                                                *
 **                 All Rights Reserved.                                      *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **---------------------------------------------------------------------------*
 **  2015.09.15                         Create.                               *
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
#ifndef _MMI_APP_DATACOUNTER_TRC_H_
#define _MMI_APP_DATACOUNTER_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIDATACOUNTER_DATA_MSG_TRC_01 "MMIAPIDataCounter_dataAlertNotifaction dataType = %d"
#define MMIDATACOUNTER_DATA_MSG_TRC_02 "MMIAPIDataCounter_dataAlertNotifaction CheckSameSimCard %d"
#define MMIDATACOUNTER_DATA_MSG_TRC_03 "MMIAPIDataCounter_dataAlertNotifaction read AlertValue %d"
#define MMIDATACOUNTER_DATA_MSG_TRC_04 "MMIAPIDataCounter_dataAlertNotifaction bAlertStatus = %d"
#define MMIDATACOUNTER_DATA_MSG_TRC_05 "MMIAPIDataCounter_dataAlertNotifaction showJustone.dataType = %d"
#define MMIDATACOUNTER_DATA_MSG_TRC_06 "MMIAPIDataCounter_dataAlertNotifaction showJustone.showJustOne = %d"
#define MMIDATACOUNTER_DATA_MSG_TRC_07 "MMIAPIDataCounter_dataAlertNotifaction bAlertNotifaction 111 = %d"
#define MMIDATACOUNTER_DATA_MSG_TRC_08 "MMIAPIDataCounter_dataAlertNotifaction dataActualSize = %d"
#define MMIDATACOUNTER_DATA_MSG_TRC_09 "MMIAPIDataCounter_dataAlertNotifaction dataAlertSize = %d"
#define MMIDATACOUNTER_DATA_MSG_TRC_10 "MMIAPIDataCounter_dataAlertNotifaction bAlertNotifaction 2222 = %d"

#define MMIDATACOUNTER_CALL_MSG_TRC_01 "MMIAPIDataCounter_callAlertNotifaction dataType = %d"
#define MMIDATACOUNTER_CALL_MSG_TRC_02 "MMIAPIDataCounter_callAlertNotifaction CheckSameSimCard %d"
#define MMIDATACOUNTER_CALL_MSG_TRC_03 "MMIAPIDataCounter_callAlertNotifaction read AlertValue %d"
#define MMIDATACOUNTER_CALL_MSG_TRC_04 "MMIAPIDataCounter_callAlertNotifaction bAlertStatus = %d"
#define MMIDATACOUNTER_CALL_MSG_TRC_05 "MMIAPIDataCounter_callAlertNotifaction showJustone.dataType = %d"
#define MMIDATACOUNTER_CALL_MSG_TRC_06 "MMIAPIDataCounter_callAlertNotifaction showJustone.showJustOne = %d"
#define MMIDATACOUNTER_CALL_MSG_TRC_07 "MMIAPIDataCounter_callAlertNotifaction bAlertNotifaction 111 = %d"
#define MMIDATACOUNTER_CALL_MSG_TRC_08 "MMIAPIDataCounter_callAlertNotifaction dataActualSize = %d"
#define MMIDATACOUNTER_CALL_MSG_TRC_09 "MMIAPIDataCounter_callAlertNotifaction dataAlertSize = %d"
#define MMIDATACOUNTER_CALL_MSG_TRC_10 "MMIAPIDataCounter_callAlertNotifaction bAlertNotifaction 2222 = %d"

#define MMIDATACOUNTER_SMS_MSG_TRC_01 "MMIAPIDataCounter_smsAlertNotifaction dataType = %d"
#define MMIDATACOUNTER_SMS_MSG_TRC_02 "MMIAPIDataCounter_smsAlertNotifaction CheckSameSimCard %d"
#define MMIDATACOUNTER_SMS_MSG_TRC_03 "MMIAPIDataCounter_smsAlertNotifaction read AlertValue %d"
#define MMIDATACOUNTER_SMS_MSG_TRC_04 "MMIAPIDataCounter_smsAlertNotifaction bAlertStatus = %d"
#define MMIDATACOUNTER_SMS_MSG_TRC_05 "MMIAPIDataCounter_smsAlertNotifaction showJustone.dataType = %d"
#define MMIDATACOUNTER_SMS_MSG_TRC_06 "MMIAPIDataCounter_smsAlertNotifaction showJustone.showJustOne = %d"
#define MMIDATACOUNTER_SMS_MSG_TRC_07 "MMIAPIDataCounter_smsAlertNotifaction bAlertNotifaction 111 = %d"
#define MMIDATACOUNTER_SMS_MSG_TRC_08 "MMIAPIDataCounter_smsAlertNotifaction dataActualSize = %d"
#define MMIDATACOUNTER_SMS_MSG_TRC_09 "MMIAPIDataCounter_smsAlertNotifaction dataAlertSize = %d"
#define MMIDATACOUNTER_SMS_MSG_TRC_10 "MMIAPIDataCounter_smsAlertNotifaction bAlertNotifaction 2222 = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_DATACOUNTER_TRC)
TRACE_MSG(MMIDATACOUNTER_DATA_MSG_TRC_01,"MMIAPIDataCounter_dataAlertNotifaction dataType = %d")
TRACE_MSG(MMIDATACOUNTER_DATA_MSG_TRC_02,"MMIAPIDataCounter_dataAlertNotifaction CheckSameSimCard %d")
TRACE_MSG(MMIDATACOUNTER_DATA_MSG_TRC_03,"MMIAPIDataCounter_dataAlertNotifaction read AlertValue %d")
TRACE_MSG(MMIDATACOUNTER_DATA_MSG_TRC_04,"MMIAPIDataCounter_dataAlertNotifaction bAlertStatus = %d")
TRACE_MSG(MMIDATACOUNTER_DATA_MSG_TRC_05,"MMIAPIDataCounter_dataAlertNotifaction showJustone.dataType = %d")
TRACE_MSG(MMIDATACOUNTER_DATA_MSG_TRC_06,"MMIAPIDataCounter_dataAlertNotifaction showJustone.showJustOne = %d")
TRACE_MSG(MMIDATACOUNTER_DATA_MSG_TRC_07,"MMIAPIDataCounter_dataAlertNotifaction bAlertNotifaction 111  = %d")
TRACE_MSG(MMIDATACOUNTER_DATA_MSG_TRC_08,"MMIAPIDataCounter_dataAlertNotifaction dataActualSize = %d")
TRACE_MSG(MMIDATACOUNTER_DATA_MSG_TRC_09,"MMIAPIDataCounter_dataAlertNotifaction dataAlertSize = %d")
TRACE_MSG(MMIDATACOUNTER_DATA_MSG_TRC_10,"MMIAPIDataCounter_dataAlertNotifaction bAlertNotifaction 2222 = %d")

TRACE_MSG(MMIDATACOUNTER_CALL_MSG_TRC_01,"MMIAPIDataCounter_callAlertNotifaction dataType = %d")
TRACE_MSG(MMIDATACOUNTER_CALL_MSG_TRC_02,"MMIAPIDataCounter_callAlertNotifaction CheckSameSimCard %d")
TRACE_MSG(MMIDATACOUNTER_CALL_MSG_TRC_03,"MMIAPIDataCounter_callAlertNotifaction read AlertValue %d")
TRACE_MSG(MMIDATACOUNTER_CALL_MSG_TRC_04,"MMIAPIDataCounter_callAlertNotifaction bAlertStatus = %d")
TRACE_MSG(MMIDATACOUNTER_CALL_MSG_TRC_05,"MMIAPIDataCounter_callAlertNotifaction showJustone.dataType = %d")
TRACE_MSG(MMIDATACOUNTER_CALL_MSG_TRC_06,"MMIAPIDataCounter_callAlertNotifaction showJustone.showJustOne = %d")
TRACE_MSG(MMIDATACOUNTER_CALL_MSG_TRC_07,"MMIAPIDataCounter_callAlertNotifaction bAlertNotifaction 111  = %d")
TRACE_MSG(MMIDATACOUNTER_CALL_MSG_TRC_08,"MMIAPIDataCounter_callAlertNotifaction dataActualSize = %d")
TRACE_MSG(MMIDATACOUNTER_CALL_MSG_TRC_09,"MMIAPIDataCounter_callAlertNotifaction dataAlertSize = %d")
TRACE_MSG(MMIDATACOUNTER_CALL_MSG_TRC_10,"MMIAPIDataCounter_callAlertNotifaction bAlertNotifaction 2222 = %d")

TRACE_MSG(MMIDATACOUNTER_SMS_MSG_TRC_01,"MMIAPIDataCounter_smsAlertNotifaction dataType = %d")
TRACE_MSG(MMIDATACOUNTER_SMS_MSG_TRC_02,"MMIAPIDataCounter_smsAlertNotifaction CheckSameSimCard %d")
TRACE_MSG(MMIDATACOUNTER_SMS_MSG_TRC_03,"MMIAPIDataCounter_smsAlertNotifaction read AlertValue %d")
TRACE_MSG(MMIDATACOUNTER_SMS_MSG_TRC_04,"MMIAPIDataCounter_smsAlertNotifaction bAlertStatus = %d")
TRACE_MSG(MMIDATACOUNTER_SMS_MSG_TRC_05,"MMIAPIDataCounter_smsAlertNotifaction showJustone.dataType = %d")
TRACE_MSG(MMIDATACOUNTER_SMS_MSG_TRC_06,"MMIAPIDataCounter_smsAlertNotifaction showJustone.showJustOne = %d")
TRACE_MSG(MMIDATACOUNTER_SMS_MSG_TRC_07,"MMIAPIDataCounter_smsAlertNotifaction bAlertNotifaction 111  = %d")
TRACE_MSG(MMIDATACOUNTER_SMS_MSG_TRC_08,"MMIAPIDataCounter_smsAlertNotifaction dataActualSize = %d")
TRACE_MSG(MMIDATACOUNTER_SMS_MSG_TRC_09,"MMIAPIDataCounter_smsAlertNotifaction dataAlertSize = %d")
TRACE_MSG(MMIDATACOUNTER_SMS_MSG_TRC_10,"MMIAPIDataCounter_smsAlertNotifaction bAlertNotifaction 2222 = %d")

END_TRACE_MAP(MMI_APP_DATACOUNTER_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_DATACOUNTER_TRC_H_
