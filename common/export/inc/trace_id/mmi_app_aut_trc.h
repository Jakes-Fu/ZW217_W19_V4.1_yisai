/******************************************************************************
 ** File Name:      mmi_app_aut_trc.h                                         *
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
//trace_id:153
#ifndef _MMI_APP_AUT_TRC_H_
#define _MMI_APP_AUT_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIAUT_EXPORT_61_112_2_18_2_5_21_0 "MMIAPIAUT_GetIsNeedUpdateTimeByNet:::s_update_time_by_netstatusind = %d"
#define MMIAUT_EXPORT_71_112_2_18_2_5_21_1 "[MMIAUT] s_is_manual_update_time_running= %d"
#define MMIAUT_EXPORT_81_112_2_18_2_5_21_2 "[MMIAUT] s_is_manual_update_time_running = s_running =%d"
#define MMIAUT_EXPORT_151_112_2_18_2_5_21_3 "[MMIAUT]::: MMIAPIAUT_GetTimeFromRecvData(), content_length = %d"
#define MMIAUT_NET_516_112_2_18_2_5_22_4 "[mmiaut]:MMIAUT_SetSysTime, Set time = %d-%d-%d %d:%d:%d"
#define MMIAUT_NET_574_112_2_18_2_5_22_5 "[mmiaut]:MMIAUT_SetSysTime, status = %d"
#define MMIAUT_NET_591_112_2_18_2_5_22_6 "[MMIAUT]::MMIAUT_GetSysTime, Cur time = %d-%d-%d %d:%d:%d"
#define MMIAUT_NET_668_112_2_18_2_5_23_7 "[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), Param is PNULL!"
#define MMIAUT_NET_677_112_2_18_2_5_23_8 "[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), Moved Temporarily,url has redirected!"
#define MMIAUT_NET_685_112_2_18_2_5_23_9 "[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), 304 Not Modified,time should not update!"
#define MMIAUT_NET_693_112_2_18_2_5_23_10 "[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf():%s, %s"
#define MMIAUT_NET_697_112_2_18_2_5_23_11 "[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), web entry from cache!"
#define MMIAUT_NET_711_112_2_18_2_5_23_12 "[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), no exist!"
#define MMIAUT_NET_718_112_2_18_2_5_23_13 "[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), error string!"
#define MMIAUT_NET_722_112_2_18_2_5_23_14 "[MMIAUT]:dave:%s, %s"
#define MMIAUT_NET_761_112_2_18_2_5_23_15 "[MMIAUT] ::is in summer:%d"
#define MMIAUT_NET_771_112_2_18_2_5_23_16 "[MMIAUT] ::dave:%2f"
#define MMIAUT_NET_834_112_2_18_2_5_23_17 "[MMIAUT] MMIAUT_HandleSNTPCnfMsg msg_id=%d"
#define MMIAUT_NET_838_112_2_18_2_5_23_18 "[MMIAUT] MMIAUT_HandleSNTPCnfMsg: invalid param!"
#define MMIAUT_NET_844_112_2_18_2_5_23_19 "[MMIAUT] MMIAUT_HandleSNTPCnfMsg: user canceled!"
#define MMIAUT_NET_858_112_2_18_2_5_23_20 "[MMIAUT]: MMIAUT_HandleSNTPCnfMsg error_code=%d, net_time=%d"
#define MMIAUT_NET_872_112_2_18_2_5_23_21 "[MMIAUT]: MMIAUT_HandleSNTPCnfMsg set systime set_value= %d,nettime_from_1980 =%d"
#define MMIAUT_NET_916_112_2_18_2_5_23_22 "[MMIAUT]::MMIAUT_GetTimezoneIndexForSNTP, mcc =%d,local_timezone=%d"
#define MMIAUT_NET_920_112_2_18_2_5_23_23 "[MMIAUT]:MMIAUT_GetTimezoneIndexForSNTP  error local_timezone"
#define MMIAUT_NET_934_112_2_18_2_5_23_24 "[MMIAUT]::MMIAUT_GetTimezoneIndexForSNTP dst_rule_index = %d"
#define MMIAUT_NET_946_112_2_18_2_5_23_25 "[MMIAUT]::MMIAUT_GetTimezoneIndexForSNTP timezone_index = %d"
#define MMIAUT_NET_960_112_2_18_2_5_23_26 "[MMIAUT]::MMIAUT_GetTimezoneIndexForSNTP timezone_index1 = %d"
#define MMIAUT_NET_978_112_2_18_2_5_23_27 "[MMIAUT]::MMIAUT_GetMccAndTimezone, mcc ||local_timezone is NULL"
#define MMIAUT_NET_982_112_2_18_2_5_23_28 "[MMIAUT]::MMIAUT_GetMccAndTimezone, mcc =%d,local_timezone=%d"
#define MMIAUT_NET_994_112_2_18_2_5_23_29 "[MMIAUT]::MMIAUT_GetMccAndTimezone2, mcc =%d,local_timezone=%d"
#define MMIAUT_NET_1021_112_2_18_2_5_23_30 "[MMIAUT]::MMIAUT_GetMccAndTimezone3, mcc =%d,local_timezone=%d"
#define MMIAUT_NET_1035_112_2_18_2_5_23_31 "[MMIAUT]::MMIAUT_GetSNTPTime sntp_server_addr_index=%d"
#define MMIAUT_NET_1039_112_2_18_2_5_23_32 "[MMIAUT]::MMIAUT_GetSNTPTime sntp_server_addr_index= %d"
#define MMIAUT_NET_1069_112_2_18_2_5_23_33 "[MMIAUT]::MMIAUT_GetSNTPTime GetNTPTime return_val = %d"
#define MMIAUT_NET_1084_112_2_18_2_5_23_34 "[MMIAUT]::MMIAUT_ReleaseAllPDPCallback detail_item.dual_sys=%d,detail_item.item_index =%d"
#define MMIAUT_NET_1109_112_2_18_2_5_24_35 "[MMIAUT]::MMIAUT_ActivePDPLink dual_sys=%d, item_index= %d"
#define MMIAUT_NET_1131_112_2_18_2_5_24_36 "[MMIAUT]::MMIAUT_ActivePDPLink use GPRS"
#define MMIAUT_NET_1143_112_2_18_2_5_24_37 "MMIAUT::MMIAUT_ActivePDPLink use WIFI!!!"
#define MMIAUT_NET_1147_112_2_18_2_5_24_38 "[MMIAUT]::MMIAUT_ActivePDPLink active pdp return_val = %d"
#define MMIAUT_NET_1161_112_2_18_2_5_24_39 "[MMIAUT]::HandleNetTimeServerPdpMsg msg_ptr->msg_id =%d, msg_ptr->result %d"
#define MMIAUT_NET_1197_112_2_18_2_5_24_40 "[MMIAUT]::HandleNetTimePdpMsg Active GPRS OK! "
#define MMIAUT_NET_1270_112_2_18_2_5_24_41 "[MMIAUT]::HandleNetTimeServerPdpMsg  MMIPDP_DEACTIVE_CNF"
#define MMIAUT_NET_1275_112_2_18_2_5_24_42 "[MMIAUT]::HandleNetTimeServerPdpMsg  MMIPDP_DEACTIVE_IND"
#define MMIAUT_NET_1293_112_2_18_2_5_24_43 "[MMIAUT]::MMIAUT_DeactivePDPLink  active pdp return_val= %d"
#define MMIAUT_NET_1340_112_2_18_2_5_24_44 "SetSntpLinkDetailItem dual_sys= %d, item_index=%d"
#define MMIAUT_WINTAB_370_112_2_18_2_5_25_45 "[MMIAUT]::HandleAutoUpdateTimeSelAccountWindow item_num=%d"
#define MMIAUT_WINTAB_397_112_2_18_2_5_25_46 "[MMIAUT]::HandleAutoUpdateTimeSelAccountWindow item_num=%d"
#define MMIAUT_WINTAB_613_112_2_18_2_5_25_47 "[MMIAUT]: OpenManualUpdateWaitWin deactive pdp s_is_need_auto_update_time= %d,MMIAPIAUT_GetIsNeedUpdateTimeByNet =%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_AUT_TRC)
TRACE_MSG(MMIAUT_EXPORT_61_112_2_18_2_5_21_0,"MMIAPIAUT_GetIsNeedUpdateTimeByNet:::s_update_time_by_netstatusind = %d")
TRACE_MSG(MMIAUT_EXPORT_71_112_2_18_2_5_21_1,"[MMIAUT] s_is_manual_update_time_running= %d")
TRACE_MSG(MMIAUT_EXPORT_81_112_2_18_2_5_21_2,"[MMIAUT] s_is_manual_update_time_running = s_running =%d")
TRACE_MSG(MMIAUT_EXPORT_151_112_2_18_2_5_21_3,"[MMIAUT]::: MMIAPIAUT_GetTimeFromRecvData(), content_length = %d")
TRACE_MSG(MMIAUT_NET_516_112_2_18_2_5_22_4,"[mmiaut]:MMIAUT_SetSysTime, Set time = %d-%d-%d %d:%d:%d")
TRACE_MSG(MMIAUT_NET_574_112_2_18_2_5_22_5,"[mmiaut]:MMIAUT_SetSysTime, status = %d")
TRACE_MSG(MMIAUT_NET_591_112_2_18_2_5_22_6,"[MMIAUT]::MMIAUT_GetSysTime, Cur time = %d-%d-%d %d:%d:%d")
TRACE_MSG(MMIAUT_NET_668_112_2_18_2_5_23_7,"[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), Param is PNULL!")
TRACE_MSG(MMIAUT_NET_677_112_2_18_2_5_23_8,"[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), Moved Temporarily,url has redirected!")
TRACE_MSG(MMIAUT_NET_685_112_2_18_2_5_23_9,"[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), 304 Not Modified,time should not update!")
TRACE_MSG(MMIAUT_NET_693_112_2_18_2_5_23_10,"[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf():%s, %s")
TRACE_MSG(MMIAUT_NET_697_112_2_18_2_5_23_11,"[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), web entry from cache!")
TRACE_MSG(MMIAUT_NET_711_112_2_18_2_5_23_12,"[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), no exist!")
TRACE_MSG(MMIAUT_NET_718_112_2_18_2_5_23_13,"[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), error string!")
TRACE_MSG(MMIAUT_NET_722_112_2_18_2_5_23_14,"[MMIAUT]:dave:%s, %s")
TRACE_MSG(MMIAUT_NET_761_112_2_18_2_5_23_15,"[MMIAUT] ::is in summer:%d")
TRACE_MSG(MMIAUT_NET_771_112_2_18_2_5_23_16,"[MMIAUT] ::dave:%2f")
TRACE_MSG(MMIAUT_NET_834_112_2_18_2_5_23_17,"[MMIAUT] MMIAUT_HandleSNTPCnfMsg msg_id=%d")
TRACE_MSG(MMIAUT_NET_838_112_2_18_2_5_23_18,"[MMIAUT] MMIAUT_HandleSNTPCnfMsg: invalid param!")
TRACE_MSG(MMIAUT_NET_844_112_2_18_2_5_23_19,"[MMIAUT] MMIAUT_HandleSNTPCnfMsg: user canceled!")
TRACE_MSG(MMIAUT_NET_858_112_2_18_2_5_23_20,"[MMIAUT]: MMIAUT_HandleSNTPCnfMsg error_code=%d, net_time=%d")
TRACE_MSG(MMIAUT_NET_872_112_2_18_2_5_23_21,"[MMIAUT]: MMIAUT_HandleSNTPCnfMsg set systime set_value= %d,nettime_from_1980 =%d")
TRACE_MSG(MMIAUT_NET_916_112_2_18_2_5_23_22,"[MMIAUT]::MMIAUT_GetTimezoneIndexForSNTP, mcc =%d,local_timezone=%d")
TRACE_MSG(MMIAUT_NET_920_112_2_18_2_5_23_23,"[MMIAUT]:MMIAUT_GetTimezoneIndexForSNTP  error local_timezone")
TRACE_MSG(MMIAUT_NET_934_112_2_18_2_5_23_24,"[MMIAUT]::MMIAUT_GetTimezoneIndexForSNTP dst_rule_index = %d")
TRACE_MSG(MMIAUT_NET_946_112_2_18_2_5_23_25,"[MMIAUT]::MMIAUT_GetTimezoneIndexForSNTP timezone_index = %d")
TRACE_MSG(MMIAUT_NET_960_112_2_18_2_5_23_26,"[MMIAUT]::MMIAUT_GetTimezoneIndexForSNTP timezone_index1 = %d")
TRACE_MSG(MMIAUT_NET_978_112_2_18_2_5_23_27,"[MMIAUT]::MMIAUT_GetMccAndTimezone, mcc ||local_timezone is NULL")
TRACE_MSG(MMIAUT_NET_982_112_2_18_2_5_23_28,"[MMIAUT]::MMIAUT_GetMccAndTimezone, mcc =%d,local_timezone=%d")
TRACE_MSG(MMIAUT_NET_994_112_2_18_2_5_23_29,"[MMIAUT]::MMIAUT_GetMccAndTimezone2, mcc =%d,local_timezone=%d")
TRACE_MSG(MMIAUT_NET_1021_112_2_18_2_5_23_30,"[MMIAUT]::MMIAUT_GetMccAndTimezone3, mcc =%d,local_timezone=%d")
TRACE_MSG(MMIAUT_NET_1035_112_2_18_2_5_23_31,"[MMIAUT]::MMIAUT_GetSNTPTime sntp_server_addr_index=%d")
TRACE_MSG(MMIAUT_NET_1039_112_2_18_2_5_23_32,"[MMIAUT]::MMIAUT_GetSNTPTime sntp_server_addr_index= %d")
TRACE_MSG(MMIAUT_NET_1069_112_2_18_2_5_23_33,"[MMIAUT]::MMIAUT_GetSNTPTime GetNTPTime return_val = %d")
TRACE_MSG(MMIAUT_NET_1084_112_2_18_2_5_23_34,"[MMIAUT]::MMIAUT_ReleaseAllPDPCallback detail_item.dual_sys=%d,detail_item.item_index =%d")
TRACE_MSG(MMIAUT_NET_1109_112_2_18_2_5_24_35,"[MMIAUT]::MMIAUT_ActivePDPLink dual_sys=%d, item_index= %d")
TRACE_MSG(MMIAUT_NET_1131_112_2_18_2_5_24_36,"[MMIAUT]::MMIAUT_ActivePDPLink use GPRS")
TRACE_MSG(MMIAUT_NET_1143_112_2_18_2_5_24_37,"MMIAUT::MMIAUT_ActivePDPLink use WIFI!!!")
TRACE_MSG(MMIAUT_NET_1147_112_2_18_2_5_24_38,"[MMIAUT]::MMIAUT_ActivePDPLink active pdp return_val = %d")
TRACE_MSG(MMIAUT_NET_1161_112_2_18_2_5_24_39,"[MMIAUT]::HandleNetTimeServerPdpMsg msg_ptr->msg_id =%d, msg_ptr->result %d")
TRACE_MSG(MMIAUT_NET_1197_112_2_18_2_5_24_40,"[MMIAUT]::HandleNetTimePdpMsg Active GPRS OK! ")
TRACE_MSG(MMIAUT_NET_1270_112_2_18_2_5_24_41,"[MMIAUT]::HandleNetTimeServerPdpMsg  MMIPDP_DEACTIVE_CNF")
TRACE_MSG(MMIAUT_NET_1275_112_2_18_2_5_24_42,"[MMIAUT]::HandleNetTimeServerPdpMsg  MMIPDP_DEACTIVE_IND")
TRACE_MSG(MMIAUT_NET_1293_112_2_18_2_5_24_43,"[MMIAUT]::MMIAUT_DeactivePDPLink  active pdp return_val= %s")
TRACE_MSG(MMIAUT_NET_1340_112_2_18_2_5_24_44,"SetSntpLinkDetailItem dual_sys= %d, item_index=%d")
TRACE_MSG(MMIAUT_WINTAB_370_112_2_18_2_5_25_45,"[MMIAUT]::HandleAutoUpdateTimeSelAccountWindow item_num=%d")
TRACE_MSG(MMIAUT_WINTAB_397_112_2_18_2_5_25_46,"[MMIAUT]::HandleAutoUpdateTimeSelAccountWindow item_num=%d")
TRACE_MSG(MMIAUT_WINTAB_613_112_2_18_2_5_25_47,"[MMIAUT]: OpenManualUpdateWaitWin deactive pdp s_is_need_auto_update_time= %d,MMIAPIAUT_GetIsNeedUpdateTimeByNet =%d")
END_TRACE_MAP(MMI_APP_AUT_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_AUT_TRC_H_

