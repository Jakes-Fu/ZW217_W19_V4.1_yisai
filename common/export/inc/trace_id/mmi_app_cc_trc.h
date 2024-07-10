/******************************************************************************
 ** File Name:      mmi_app_cc_trc.h                                         *
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
//trace_id:159
#ifndef _MMI_APP_CC_TRC_H_
#define _MMI_APP_CC_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMICC_APP_966_112_2_18_2_10_8_0 "ccapp.c:enter function CC_HandlePsMsg(), msg_id = %d"
#define MMICC_APP_970_112_2_18_2_10_8_1 "ccapp.c: CC_HandlePsMsg() app_ptr == PNULL"
#define MMICC_APP_1022_112_2_18_2_10_8_2 "ccapp.c:enter function CC_HandlePsMsg(), APP_MN_CALL_DISCONNECTED_IND"
#define MMICC_APP_1075_112_2_18_2_10_8_3 "ccapp.c:enter function CC_HandlePsMsg(), APP_MN_CALL_ERR_IND, dis_cause = %d"
#define MMICC_APP_1081_112_2_18_2_10_8_4 "ccapp: CC_HandlePsMsg->APP_MN_SETUP_IND"
#define MMICC_APP_1088_112_2_18_2_10_8_5 "ccapp disc sim %d mo call"
#define MMICC_APP_1127_112_2_18_2_10_9_6 "ccapp.c CC_HandlePsMsg, dual_sys = %d, s_call_ready = %d"
#define MMICC_APP_1131_112_2_18_2_10_9_7 "ccapp error call_ready_param_ptr.dual_sys = %d"
#define MMICC_APP_1178_112_2_18_2_10_9_8 "ccapp.c: CC_HandlePsMsg() CC isn't process, msg_id = %d"
#define MMICC_APP_1229_112_2_18_2_10_9_9 "ccapp.c:enter function HandleMTFWDisconnected() disconnected cause = %d, call_id = %d"
#define MMICC_APP_1247_112_2_18_2_10_9_10 "[MMICC]: HandleSIMPlugNotifySTKFunc dual_sys=%d, notify_event=%d"
#define MMICC_APP_1345_112_2_18_2_10_9_11 "ccapp.c: ProcessPhoneNumExt() tele_num == PNULL"
#define MMICC_APP_1356_112_2_18_2_10_9_12 "ccapp: dial_method error %d"
#define MMICC_APP_1382_112_2_18_2_10_9_13 "ccapp.c: ProcessPhoneNumExt() req is reject by vt"
#define MMICC_APP_1389_112_2_18_2_10_9_14 "ccapp.c:ProcessPhoneNumExt() tele_len = 0"
#define MMICC_APP_1399_112_2_18_2_10_9_15 "mmicc: ProcessPhoneNumExt, dual_sys = %d, tele_num = %s, tele_len =%d, call_type = %d"
#define MMICC_APP_1404_112_2_18_2_10_9_16 "ccapp.c:ProcessPhoneNumExt() call MMIAPICC_MakeCallReq error"
#define MMICC_APP_1465_112_2_18_2_10_9_17 "ccapp.c: MMIAPICC_MakeCallReq() tele_num == PNULL"
#define MMICC_APP_1481_112_2_18_2_10_9_18 "ccapp error dualsys = %d, is_call_video_call = %d"
#define MMICC_APP_1498_112_2_18_2_10_9_19 "ccapp.c:MMIAPICC_MakeCallReq() call_number= %d"
#define MMICC_APP_1503_112_2_18_2_10_9_20 "ccapp.c:MMIAPICC_MakeCallReq() tele_len= %d"
#define MMICC_APP_1509_112_2_18_2_10_9_21 "ccapp.c:MMIAPICC_MakeCallReq() tele_len = 0"
#define MMICC_APP_1517_112_2_18_2_10_9_22 "ccapp.c:MakeCallReqExt() CC_GenPartyNumber return failure"
#define MMICC_APP_1539_112_2_18_2_10_9_23 "ccapp.c:MMIAPICC_MakeCallReq() CC_ProcessExtensionNum error"
#define MMICC_APP_1548_112_2_18_2_10_9_24 "ccapp.c:MMIAPICC_MakeCallReq() call_type = MN_CALL_TYPE_EMERGENCY"
#define MMICC_APP_1559_112_2_18_2_10_9_25 "ccapp.c:MMIAPICC_MakeCallReq() setup_ind msg is being"
#define MMICC_APP_1569_112_2_18_2_10_9_26 "ccapp.c:MakeCallReqExt() CC_GenPartyNumber return failure"
#define MMICC_APP_1590_112_2_18_2_10_10_27 "mmiccapp: s_call_context.want_mo_call.address.num_len = %d, tele_num = %s, tele_len = %d"
#define MMICC_APP_1727_112_2_18_2_10_10_28 "ccapp.c:MakeCallReqExt() mmi_clir_type=%d"
#define MMICC_APP_1731_112_2_18_2_10_10_29 "ERR: ccapp dial method %d"
#define MMICC_APP_1754_112_2_18_2_10_10_30 "ccapp.c:MMIAPICC_MakeCallReq() MNCALL_StartCall Fail by FDN"
#define MMICC_APP_1759_112_2_18_2_10_10_31 "ccapp.c:MMIAPICC_MakeCallReq() MNCALL_StartCall return failure"
#define MMICC_APP_1833_112_2_18_2_10_10_32 "MMIAPICC_RemoveIpNumber:%s,%d"
#define MMICC_APP_1847_112_2_18_2_10_10_33 "ccapp error origin_number_len = %d, tele_len = %d, ip_number_len = %d"
#define MMICC_APP_1875_112_2_18_2_10_10_34 "ccapp.c: GetNameFromPb() name_ptr  ||entry_id_ptr || group_id_ptr is null"
#define MMICC_APP_1915_112_2_18_2_10_10_35 "ccapp.c, GetNameFromPb search_result=%d"
#define MMICC_APP_1935_112_2_18_2_10_10_36 "MMIAPICC_GetExitedCallCount: The active call number is %d"
#define MMICC_APP_1961_112_2_18_2_10_10_37 "MMIAPPICC_BtAcceptCall: hold and answer call!"
#define MMICC_APP_1969_112_2_18_2_10_10_38 "MMIAPPICC_BtAcceptCall: the call status is %d"
#define MMICC_APP_1984_112_2_18_2_10_10_39 "MMIAPICC_BtCancelCall: the call status is %d"
#define MMICC_APP_2028_112_2_18_2_10_10_40 "ccapp.c:enter function CC_CallStartInd(), call_id = %d"
#define MMICC_APP_2031_112_2_18_2_10_10_41 "ccapp.c: CC_CallStartInd() pSig == PNULL"
#define MMICC_APP_2037_112_2_18_2_10_10_42 "ccapp.c: CC_CallStartInd() , call_id = %d"
#define MMICC_APP_2097_112_2_18_2_10_11_43 "---WD:CC_CallStartInd deal with hook"
#define MMICC_APP_2116_112_2_18_2_10_11_44 "ccapp.c:enter function CC_SyncInd(), sync_cause = %d"
#define MMICC_APP_2119_112_2_18_2_10_11_45 "ccapp.c: CC_SyncInd() pSig == PNULL"
#define MMICC_APP_2128_112_2_18_2_10_11_46 "ccapp SyncInd current call index:%d"
#define MMICC_APP_2133_112_2_18_2_10_11_47 "ccapp SyncInd param mtfw index:%d"
#define MMICC_APP_2171_112_2_18_2_10_11_48 "ccapp.c:enter function CC_CallAlertingInd(), call_id = %d, alerting_type = %d"
#define MMICC_APP_2174_112_2_18_2_10_11_49 "ccapp.c: CC_CallAlertingInd() pSig == PNULL"
#define MMICC_APP_2179_112_2_18_2_10_11_50 "ccapp.c: CC_CallAlertingInd() , call_id = %d"
#define MMICC_APP_2217_112_2_18_2_10_11_51 "ccapp.c:CC_CallAlertingInd() alerting_type = %d"
#define MMICC_APP_2232_112_2_18_2_10_11_52 "ccapp.c:enter function CC_CallProgressInd(), descrip = %d"
#define MMICC_APP_2235_112_2_18_2_10_11_53 "ccapp.c: CC_CallProgressInd() pSig == PNULL"
#define MMICC_APP_2269_112_2_18_2_10_11_54 "ccapp.c:enter function CC_SetupCompleteCnf(), pSig = NULL ||pSig->call_id >= 7"
#define MMICC_APP_2272_112_2_18_2_10_11_55 "ccapp.c:enter function CC_SetupCompleteCnf(), call_id = %d"
#define MMICC_APP_2282_112_2_18_2_10_11_56 "ccapp.c:CC_SetupCompleteCnf() current_call invalid %d"
#define MMICC_APP_2313_112_2_18_2_10_11_57 "---WD:CC_SetupCompleteCnf deal with hook"
#define MMICC_APP_2355_112_2_18_2_10_11_58 "ccapp.c:enter function CC_ReleaseCallReq(), dual_sys = %d, call_id = %d, cause = %d"
#define MMICC_APP_2360_112_2_18_2_10_11_59 "ccapp.c: CC_ReleaseCallReq , call_id = %d"
#define MMICC_APP_2368_112_2_18_2_10_11_60 "ccapp.c:CC_ReleaseCallReq() current_call invalid %d"
#define MMICC_APP_2375_112_2_18_2_10_11_61 "ccapp.c:CC_ReleaseCallReq() MNCALL_DiscCall return failure"
#define MMICC_APP_2441_112_2_18_2_10_11_62 "ccapp.c:enter function HandleCallForwardInd(), pSig = NULL ||pSig->call_id >= 7"
#define MMICC_APP_2444_112_2_18_2_10_11_63 "ccapp.c:enter function HandleCallForwardInd(), call_id = %d"
#define MMICC_APP_2449_112_2_18_2_10_11_64 "ccapp.c:HandleCallForwardInd() mtfw"
#define MMICC_APP_2491_112_2_18_2_10_11_65 "ccapp.c:enter function HandleForwardStatusInd(), pSig = NULL ||pSig->call_id >= MMI_DUAL_SYS_MAX"
#define MMICC_APP_2494_112_2_18_2_10_11_66 "ccapp: HandleForwardStatusInd oper: %d stat: %d"
#define MMICC_APP_2499_112_2_18_2_10_11_67 "ccapp.c:HandleCallForwardInd() mtfw"
#define MMICC_APP_2605_112_2_18_2_10_12_68 "ccapp.c:enter function CC_DisconnectedCall()"
#define MMICC_APP_2608_112_2_18_2_10_12_69 "ccapp.c: CC_DisconnectedCall() param == PNULL"
#define MMICC_APP_2622_112_2_18_2_10_12_70 "ccapp.c  disconnected call_id = %d, cause = %d"
#define MMICC_APP_2630_112_2_18_2_10_12_71 "APP_MN_CALL_ERR_IND:call_id = %d, cause = %d"
#define MMICC_APP_2641_112_2_18_2_10_12_72 "ccapp.c: CC_DisconnectedCall() , call_id = %d"
#define MMICC_APP_2672_112_2_18_2_10_12_73 "ccapp.c ERROR:CC_DisconnectedCall() current_call invalid %d"
#define MMICC_APP_2731_112_2_18_2_10_12_74 "ccapp.c: CC_DisconnectedCallByIndex() invalid index %d"
#define MMICC_APP_2780_112_2_18_2_10_12_75 "ccapp.c:CC_DisconnectedCallByIndex() call_state = %d"
#define MMICC_APP_2800_112_2_18_2_10_12_76 "CC_DisconnectedCallByIndex :Call the function: BT_StopAlert"
#define MMICC_APP_2812_112_2_18_2_10_12_77 "CC_DisconnectedCallByIndex :Call the function: BT_StopRing"
#define MMICC_APP_2818_112_2_18_2_10_12_78 "ccapp.c:CC_DisconnectedCallByIndex() call_state = %d"
#define MMICC_APP_2825_112_2_18_2_10_12_79 "CC_DisconnectedCallByIndex :Call the function: BT_CallDisconnected"
#define MMICC_APP_3045_112_2_18_2_10_13_80 "mmiccapp:CC_DisconnectedCallByIndex,operated_by_user = %d, call_state = %d"
#define MMICC_APP_3214_112_2_18_2_10_13_81 "ccapp.c:enter function CC_CallSetupInd(), s_call_context.dual_sys = %d, s_call_context.call_number = %d"
#define MMICC_APP_3217_112_2_18_2_10_13_82 "ccapp.c: CC_CallSetupInd() pSig == PNULL"
#define MMICC_APP_3224_112_2_18_2_10_13_83 "ccapp.c:CC_CallSetupInd() call_number= %d"
#define MMICC_APP_3229_112_2_18_2_10_13_84 "ccapp.c: CC_CallSetupInd() , call_id = %d"
#define MMICC_APP_3315_112_2_18_2_10_13_85 "ccapp.c, CC_CallSetupInd search_result=%d, name.wstr_len=%d"
#define MMICC_APP_3340_112_2_18_2_10_13_86 "%d,%d,%d"
#define MMICC_APP_3399_112_2_18_2_10_13_87 "ccapp.c:enter function CC_ConnectCallReq()"
#define MMICC_APP_3410_112_2_18_2_10_13_88 "ccapp.c: CC_ConnectCallReq(), call MNCALL_ConnectCall failure"
#define MMICC_APP_3442_112_2_18_2_10_13_89 "ccapp.c:enter function CC_SetupCompleteInd(), call_id = %d"
#define MMICC_APP_3445_112_2_18_2_10_13_90 "ccapp.c: CC_SetupCompleteInd() sig_ptr == PNULL"
#define MMICC_APP_3450_112_2_18_2_10_13_91 "ccapp.c: CC_SetupCompleteInd() , call_id = %d"
#define MMICC_APP_3526_112_2_18_2_10_13_92 "CC_SetupCompleteInd(), call_info.call_type= %d"
#define MMICC_APP_3559_112_2_18_2_10_14_93 "ccapp.c: CC_ProcessExtensionNum() tele_num == PNULL"
#define MMICC_APP_3775_112_2_18_2_10_14_94 "ccapp.c: CC_GetIndexOfCallId , call_id = %d"
#define MMICC_APP_3799_112_2_18_2_10_14_95 "ccapp.c:enter function CC_DeleteCallInfo(), call_index = %d"
#define MMICC_APP_3802_112_2_18_2_10_14_96 "ccapp.c: CC_DeleteCallInfo , call_index = %d"
#define MMICC_APP_3901_112_2_18_2_10_14_97 "ccapp MMIAPICC_IsEmergencyNum error dual_sys = %d"
#define MMICC_APP_3907_112_2_18_2_10_14_98 "ccapp.c: MMIAPICC_IsEmergencyNum tele_num == PNULL"
#define MMICC_APP_3913_112_2_18_2_10_14_99 "ccapp.c: MMIAPICC_IsEmergencyNum tele_len == 0"
#define MMICC_APP_3923_112_2_18_2_10_14_100 "ccapp.c: MMIAPICC_IsEmergencyNum temp_tele_len == 0"
#define MMICC_APP_3951_112_2_18_2_10_14_101 "ccapp.c: MMIAPICC_IsEmergencyNum is emergency call"
#define MMICC_APP_3959_112_2_18_2_10_14_102 "MMIAPICC_IsEmergencyCall: emc num=%d"
#define MMICC_APP_3978_112_2_18_2_10_14_103 "MMIAPICC_IsEmergencyNum: ecc_code=%s"
#define MMICC_APP_3983_112_2_18_2_10_14_104 "ccapp.c: MMIAPICC_IsEmergencyNum() is sim emc"
#define MMICC_APP_4007_112_2_18_2_10_14_105 "ccapp.c: MMIAPICC_IsEmergencyNum is s_emergency_call_no_sim call %d"
#define MMICC_APP_4036_112_2_18_2_10_14_106 "ccapp CC_IsSimOk error dual_sys = %d"
#define MMICC_APP_4054_112_2_18_2_10_15_107 "ccapp: CC_OpenAudioTunnel codec_flag %d"
#define MMICC_APP_4091_112_2_18_2_10_15_108 "ccapp.c:enter function CC_ReleaseActiveCall()"
#define MMICC_APP_4101_112_2_18_2_10_15_109 "CC_ReleaseActiveCall , call_id = %d"
#define MMICC_APP_4146_112_2_18_2_10_15_110 "ccapp.c:enter function CC_ReleaseHoldCall()"
#define MMICC_APP_4156_112_2_18_2_10_15_111 "CC_ReleaseHoldCall , call_id = %d"
#define MMICC_APP_4165_112_2_18_2_10_15_112 "CC_ReleaseHoldCall MNCALL_DiscCall return failure"
#define MMICC_APP_4221_112_2_18_2_10_15_113 "ccapp.c:enter function MMIAPICC_ReleaseAllCallExt()"
#define MMICC_APP_4226_112_2_18_2_10_15_114 "ccapp: ReleaseAllCall fail for at cmd"
#define MMICC_APP_4234_112_2_18_2_10_15_115 "ccapp ReleaseAllCallExt error call_id = %d"
#define MMICC_APP_4273_112_2_18_2_10_15_116 "ccapp.c:enter function CC_ReleaseAllCall()"
#define MMICC_APP_4282_112_2_18_2_10_15_117 "CC_ReleaseHoldCall , call_id = %d"
#define MMICC_APP_4302_112_2_18_2_10_15_118 "CC_ReleaseActiveCall MNCALL_ReleaseActiveCalls return failure"
#define MMICC_APP_4331_112_2_18_2_10_15_119 "ccapp.c:enter function CC_HoldAndAnswerCW()"
#define MMICC_APP_4346_112_2_18_2_10_15_120 "ccapp.c: CC_HoldAndAnswerCW() index = %d"
#define MMICC_APP_4355_112_2_18_2_10_15_121 "CC_HoldAndAnswerCW: the result of MNCALL_HoldCall is error"
#define MMICC_APP_4417_112_2_18_2_10_15_122 "ccapp.c:enter function CC_ReleaseAndAnswerCW()"
#define MMICC_APP_4471_112_2_18_2_10_15_123 "ccapp.c: CC_HandleRetrieveCnf( param == NULL"
#define MMICC_APP_4474_112_2_18_2_10_15_124 "ccapp.c:enter function CC_HandleRetrieveCnf(), call_id = %d, req_is_accepted = %d"
#define MMICC_APP_4517_112_2_18_2_10_15_125 "ccapp HandleRetrieveCnf error call_id = %d"
#define MMICC_APP_4552_112_2_18_2_10_16_126 "ccapp HandleRetrieveCnf call index %d"
#define MMICC_APP_4584_112_2_18_2_10_16_127 "ccapp.c: CC_HandleHoldCnf( param == NULL"
#define MMICC_APP_4587_112_2_18_2_10_16_128 "ccapp.c:enter function CC_HandleHoldCnf(), call_id = %d, req_is_accepted = %d"
#define MMICC_APP_4608_112_2_18_2_10_16_129 "ccapp HandleHoldCnf1 call index: %d"
#define MMICC_APP_4740_112_2_18_2_10_16_130 "ccapp HandleHoldCnf call index: %d"
#define MMICC_APP_4771_112_2_18_2_10_16_131 "ccapp.c:enter function CC_HandleDisconnectedInMPTY(), call_index = %d, cause = %d"
#define MMICC_APP_4780_112_2_18_2_10_16_132 "ccapp: mpty active: %d, hold: %d, outgoing: %d"
#define MMICC_APP_4800_112_2_18_2_10_16_133 "ccapp DisconnectedInMPTY call_number: %d"
#define MMICC_APP_4817_112_2_18_2_10_16_134 "the index of active call is %d"
#define MMICC_APP_4850_112_2_18_2_10_16_135 "the index of active call is %d"
#define MMICC_APP_4881_112_2_18_2_10_16_136 "the index of active call is %d"
#define MMICC_APP_4984_112_2_18_2_10_16_137 "ccapp DisconnectedInMPTY current call:%d"
#define MMICC_APP_5008_112_2_18_2_10_16_138 "mmiccapp: FindCallInModule , index = %d"
#define MMICC_APP_5019_112_2_18_2_10_16_139 "mmiccapp: FindCallInModule , index = %d"
#define MMICC_APP_5030_112_2_18_2_10_16_140 "mmiccapp: FindCallInModule , index = %d"
#define MMICC_APP_5055_112_2_18_2_10_17_141 "ccapp.c:enter function CC_HoldCall()"
#define MMICC_APP_5067_112_2_18_2_10_17_142 "ccapp HoldCall: invalid active idx: %d"
#define MMICC_APP_5072_112_2_18_2_10_17_143 "CC_HoldCall: the active call is %s"
#define MMICC_APP_5078_112_2_18_2_10_17_144 "ccapp HoldCall: invalid hold idx: %d"
#define MMICC_APP_5082_112_2_18_2_10_17_145 "CC_HoldCall: the hold call is %s"
#define MMICC_APP_5100_112_2_18_2_10_17_146 "ccapp HoldCall: invalid hold idx: %d"
#define MMICC_APP_5118_112_2_18_2_10_17_147 "ccapp HoldCall: invalid hold idx: %d"
#define MMICC_APP_5167_112_2_18_2_10_17_148 "ccapp.c:enter function CC_BuildMPTYCall()"
#define MMICC_APP_5178_112_2_18_2_10_17_149 "ccapp error active_index = %d"
#define MMICC_APP_5187_112_2_18_2_10_17_150 "ccapp error hold_index = %d"
#define MMICC_APP_5231_112_2_18_2_10_17_151 "ccapp.c:enter function CC_BuildMPTYAndAnswerCW()"
#define MMICC_APP_5283_112_2_18_2_10_17_152 "ccapp.c: CC_HandleBuildMPTYCnf( param == NULL"
#define MMICC_APP_5286_112_2_18_2_10_17_153 "ccapp.c:enter function CC_HandleBuildMPTYCnf(), call_id = %d, req_is_accepted = %d"
#define MMICC_APP_5291_112_2_18_2_10_17_154 "ccapp.c: CC_HandleBuildMPTYCnf() , call_id = %d"
#define MMICC_APP_5299_112_2_18_2_10_17_155 "CC_HandleBuildMPTYCnf() invald call index %d"
#define MMICC_APP_5334_112_2_18_2_10_17_156 "CC_HandleBuildMPTYCnf: MNCALL_HoldCall is fault"
#define MMICC_APP_5492_112_2_18_2_10_17_157 "ccapp.c:enter function CC_SplitSpecCall(), call_id = %d"
#define MMICC_APP_5497_112_2_18_2_10_17_158 "ccapp CC_SplitSpecCall error call_id:%d"
#define MMICC_APP_5504_112_2_18_2_10_17_159 "ccapp CC_SplitSpecCall error active:%d, hold:%d"
#define MMICC_APP_5511_112_2_18_2_10_17_160 "CC_SplitSpecCall: MNCALL_SplitMPTY return failure"
#define MMICC_APP_5538_112_2_18_2_10_18_161 "CC_HandleSpecCallCnf( param == NULL"
#define MMICC_APP_5544_112_2_18_2_10_18_162 "ccapp.c:enter function CC_HandleSplitCallCnf(), call_id = %d, req_is_accepted = %d"
#define MMICC_APP_5547_112_2_18_2_10_18_163 "ccapp error sig_ptr->call_id = %d, call_index = %d"
#define MMICC_APP_5587_112_2_18_2_10_18_164 "ccapp HandleSplitCallCnf call_index: %d"
#define MMICC_APP_5667_112_2_18_2_10_18_165 "ccapp.c:enter function CC_MTCallFireWall()"
#define MMICC_APP_5671_112_2_18_2_10_18_166 "ccapp.c: CC_MTCallFireWall() call_id=%d"
#define MMICC_APP_5750_112_2_18_2_10_18_167 "CC_AutoMoCall: call_index = %d, cause = %d"
#define MMICC_APP_5751_112_2_18_2_10_18_168 "CC_AutoMoCall: call_state = %d "
#define MMICC_APP_5789_112_2_18_2_10_18_169 "CC_AutoMoCall: Need auto dial = %d"
#define MMICC_APP_5841_112_2_18_2_10_18_170 "ccapp.c:MMIAPICC_SendDtmfByChar() current_call invalid %d"
#define MMICC_APP_5877_112_2_18_2_10_18_171 "ccapp.c:MMIAPICC_SendDtmfByChar() start dtmf failure"
#define MMICC_APP_5884_112_2_18_2_10_18_172 "ccapp.c:MMIAPICC_SendDtmfByChar() stop dtmf failure"
#define MMICC_APP_5921_112_2_18_2_10_18_173 "mmiapp.c: MMIAPICC_IsInState call is rejected by black list"
#define MMICC_APP_5925_112_2_18_2_10_18_174 "mmiapp.c: MMIAPICC_IsInState, call_state = %d"
#define MMICC_APP_6041_112_2_18_2_10_19_175 "ccapp.c:enter function GetCurrentCallNumber(), bcd_number_ptr = NULL "
#define MMICC_APP_6155_112_2_18_2_10_19_176 "CC_HandleMTCallDisc: the call is disconnected"
#define MMICC_APP_6293_112_2_18_2_10_19_177 "ccapp ShowCallDiscCause:cause type = %d"
#define MMICC_APP_6387_112_2_18_2_10_19_178 "ccapp.c:MMIAPICC_ReleaseAspCallReq() MNCALL_DiscCall return failure"
#define MMICC_APP_6433_112_2_18_2_10_19_179 "ccapp: auto dial normal call..."
#define MMICC_APP_6459_112_2_18_2_10_19_180 "mmicc_app: CC_IsValideDest, disc_cause = %d"
#define MMICC_APP_6487_112_2_18_2_10_20_181 "mmicc_app: CC_IsUserBusy, disc_cause = %d"
#define MMICC_APP_6526_112_2_18_2_10_20_182 "ccapp.c:enter function MMIAPICC_GetSimCallReady(), dual_sys >= MMI_DUAL_SYS_MAX "
#define MMICC_APP_6530_112_2_18_2_10_20_183 "ccapp: MMIAPICC_GetSimCallReady, s_call_ready[dual_sys] = %d, dual_sys = %d"
#define MMICC_APP_6544_112_2_18_2_10_20_184 "ccapp: MMIAPICC_SetSimCallReady, is_ready = %d, dual_sys = %d"
#define MMICC_APP_6551_112_2_18_2_10_20_185 "ccapp: error dual_sys = %d"
#define MMICC_APP_6579_112_2_18_2_10_20_186 "ccapp.c:enter function CC_GetIPCallNum(), tele_num is NULL "
#define MMICC_APP_6948_112_2_18_2_10_21_187 "mmiccapp: CC_SendDTMFStr, dtmf_str = %s, dtmf_len = %d"
#define MMICC_APP_7146_112_2_18_2_10_21_188 "ccapp:CC_IsWaitCallBeing %d"
#define MMICC_APP_7159_112_2_18_2_10_21_189 "ccapp.c:enter function CC_ReleaseWaitCall()"
#define MMICC_APP_7169_112_2_18_2_10_21_190 "CC_ReleaseWaitCall , call_id = %d"
#define MMICC_APP_7176_112_2_18_2_10_21_191 "CC_ReleaseWaitCall MNCALL_DiscCall return failure"
#define MMICC_APP_7209_112_2_18_2_10_21_192 "ccapp.c:enter function CC_CRSSReleaseAndAnswer()"
#define MMICC_APP_7264_112_2_18_2_10_21_193 "ccapp.c:enter function CC_CRSSHoldAndAnswer()"
#define MMICC_APP_7290_112_2_18_2_10_21_194 "ccapp CC_CRSSHoldAndAnswer error index %d"
#define MMICC_APP_7295_112_2_18_2_10_21_195 "CC_CRSSHoldAndAnswer: the result of MNCALL_HoldCall is error"
#define MMICC_APP_7349_112_2_18_2_10_21_196 "ccapp:CC_ReleaseActiveCallByIndex()"
#define MMICC_APP_7371_112_2_18_2_10_21_197 "ccapp:MMICC_ParseStringOfCRSS()"
#define MMICC_APP_7556_112_2_18_2_10_22_198 "ccapp.c:enter function MMIAPICC_MakeCall(), tele_num is NULL "
#define MMICC_APP_7565_112_2_18_2_10_22_199 "ccapp.c: MMIAPICC_MakeCall Parsed in ParseString"
#define MMICC_APP_7574_112_2_18_2_10_22_200 "ccapp.c: MakeCall req is process by CRSS"
#define MMICC_APP_7593_112_2_18_2_10_22_201 "ccapp.c: MakeCall req is reject when mo or mt"
#define MMICC_APP_7599_112_2_18_2_10_22_202 "ccapp.c: MakeCall req is reject when vt"
#define MMICC_APP_7604_112_2_18_2_10_22_203 "MMIAPICC_MakeCall dual_sys=%d,call_sim_type=%d,call_type=%d"
#define MMICC_APP_7626_112_2_18_2_10_22_204 "MMIAPICC_MakeCall call_dual_sys=%d"
#define MMICC_APP_7659_112_2_18_2_10_22_205 "ccapp.c:enter function MMICC_ProcessMakeCall(), make_call_info_ptr is NULL "
#define MMICC_APP_7665_112_2_18_2_10_22_206 "ProcessMakeCall Telephone number is null!"
#define MMICC_APP_7679_112_2_18_2_10_22_207 "ProcessMakeCall dual_sys=%d"
#define MMICC_APP_7684_112_2_18_2_10_22_208 "***ProcessMakeCall MO call is barring: tele_num = %s***"
#define MMICC_APP_7719_112_2_18_2_10_22_209 "ccapp.c:MMIAPICC_MakeCallReq() network not support"
#define MMICC_APP_7736_112_2_18_2_10_22_210 "ccapp.c: CC_ProcessExtensionNum() 'P' == tele_num[0]"
#define MMICC_APP_7796_112_2_18_2_10_22_211 "ProcessMakeCall parse_result=%d"
#define MMICC_APP_7808_112_2_18_2_10_22_212 "ccapp.c:MMIAPICC_MakeCallReq() SIM isn't ok"
#define MMICC_APP_7828_112_2_18_2_10_22_213 "ccapp: sim corrupt!"
#define MMICC_APP_7921_112_2_18_2_10_23_214 "s_call_context.call_number = %d"
#define MMICC_APP_7930_112_2_18_2_10_23_215 "ProcessMakeCall: the result of MNCALL_HoldCall is error"
#define MMICC_APP_7962_112_2_18_2_10_23_216 "ProcessMakeCall: has handled in parse function"
#define MMICC_APP_8075_112_2_18_2_10_23_217 "ProcessMakeCall: Alloc memory Fail"
#define MMICC_APP_8191_112_2_18_2_10_23_218 "MMIAPICC_GetCallSimType dual_sys=%d,call_sim_type=%d"
#define MMICC_APP_8205_112_2_18_2_10_23_219 "ccapp: StartLocalNetRing"
#define MMICC_APP_8240_112_2_18_2_10_23_220 "ccapp:StopLocalNetRing"
#define MMICC_APP_8270_112_2_18_2_10_23_221 "ccapp:CC_IsMptySetupAvailable %d"
#define MMICC_APP_8287_112_2_18_2_10_23_222 "ccapp:CC_IsMptySplitAvailable %d"
#define MMICC_APP_8316_112_2_18_2_10_23_223 "ccapp:MMIAPICC_ReleaseCallByRedkey()"
#define MMICC_APP_8438_112_2_18_2_10_24_224 "ccapp: dial method error %d"
#define MMICC_APP_8641_112_2_18_2_10_24_225 "ccapp IncomingCallName_To_Speech error"
#define MMICC_APP_8760_112_2_18_2_10_24_226 "mmivt null param"
#define MMICC_APP_8840_112_2_18_2_10_25_227 "ccapp HandleMoMuteAndLocalRingState %d,msg_id= %d"
#define MMICC_APP_8857_112_2_18_2_10_25_228 "CC_HandleBtHfuHoldInd: the hf hold action is %d"
#define MMICC_APP_8871_112_2_18_2_10_25_229 "CC_HandleBtHfuHoldInd: release incoming call"
#define MMICC_APP_8878_112_2_18_2_10_25_230 "CC_HandleBtHfuHoldInd: CC_ReleaseAndAnswerCW, and answer incoming"
#define MMICC_APP_8883_112_2_18_2_10_25_231 "CC_HandleBtHfuHoldInd: CC_ReleaseAllCall"
#define MMICC_APP_8892_112_2_18_2_10_25_232 "CC_HandleBtHfuHoldInd: CC_HoldAndAnswerCW"
#define MMICC_APP_8899_112_2_18_2_10_25_233 "CC_HandleBtHfuHoldInd: CC_HoldCall"
#define MMICC_APP_8903_112_2_18_2_10_25_234 "CC_HandleBtHfuHoldInd: the hold action is not support!"
#define MMICC_APP_8908_112_2_18_2_10_25_235 "CC_HandleBtHfuHoldInd: the hold action is not support!"
#define MMICC_APP_8928_112_2_18_2_10_25_236 "***IsIncludeBarringNumbers()***"
#define MMICC_APP_9002_112_2_18_2_10_25_237 "MMIASP MMIAPICC_GetCallNumber number str len mayber is not enough !"
#define MMICC_APP_9084_112_2_18_2_10_25_238 "ccapp.c: CC_HandleCcWinMsg() call_state = %d"
#define MMICC_APP_9114_112_2_18_2_10_25_239 "AnswerASPCall fail "
#define MMICC_APP_9133_112_2_18_2_10_25_240 "ccapp.c: MMIAPICC_ResponseCallCtl dual_sys=%d, is_need_modified=%d, result=%d"
#define MMICC_APP_9163_112_2_18_2_10_25_241 "MMICC: MMIAPICC_ResponseCallCtl No Memory"
#define MMICC_APP_9447_112_2_18_2_10_26_242 "<CC_SetDevModeAndVolume>CALIBRATION_POST_MODE == iRet, Return mmi audio action to match the layer1_audio modify!"
#define MMICC_APP_9467_112_2_18_2_10_26_243 "ccapp GetHoldCallCount: exist_hold_call_num is %d"
#define MMICC_APP_9477_112_2_18_2_10_26_244 "ccapp GetActiveHoldCallCount: exist_hold_call_num is %d"
#define MMICC_APP_9547_112_2_18_2_10_27_245 "[MMIAPICC_IsCallingAndInfoBt]"
#define MMICC_APP_10098_112_2_18_2_10_28_246 "ccapp.c: MMICC_ECTIsPermit, ret = %d "
#define MMICC_APP_10130_112_2_18_2_10_28_247 "ccapp.c:enter function MMICC_ECTTransfer()"
#define MMICC_APP_10140_112_2_18_2_10_28_248 "ccapp MMICC_ECTTransfer: invalid active idx: %d"
#define MMICC_APP_10150_112_2_18_2_10_28_249 "CC_HoldCall: MNCALL_ECTInvokeEx is fault"
#define MMICC_APP_10188_112_2_18_2_10_28_250 "ccapp.c HandleECTInd"
#define MMICC_APP_10245_112_2_18_2_10_28_251 "ccapp.c party_num_present %d call id %d num_len %d"
#define MMICC_APP_10257_112_2_18_2_10_28_252 "ccapp.c HandleECTCnf"
#define MMICC_APP_10285_112_2_18_2_10_28_253 "ccapp.c ReplaceCallNumberByCallId call_id %d dual_sys %d"
#define MMICC_APP_10292_112_2_18_2_10_28_254 "ccapp.c ReplaceCallNumberByCallId find it "
#define MMICC_APP_10360_112_2_18_2_10_29_255 "ccapp IsCurrentCallMute: %d"
#define MMICC_APP_10371_112_2_18_2_10_29_256 "ccapp SetCurrentCallMute: %d"
#define MMICC_AUDIO_221_112_2_18_2_10_30_257 "ccapp_a SwitchAudioDevice err oper:%d, dev:%d"
#define MMICC_AUDIO_225_112_2_18_2_10_30_258 "ccapp_a SwitchAudioDevice oper:%d, dev:%d, status:%d, %d, %d"
#define MMICC_AUDIO_226_112_2_18_2_10_30_259 "ccapp_a SwitchAudioDevice oper:%d, dev:%d"
#define MMICC_AUDIO_231_112_2_18_2_10_30_260 "ccapp_a SwitchAudioDevice: no call is being"
#define MMICC_AUDIO_238_112_2_18_2_10_30_261 "ccapp_a SwitchAudioDevice bt is invalid, mv is running"
#define MMICC_AUDIO_263_112_2_18_2_10_30_262 "ccapp_a AudioOperation: invalid oper %d"
#define MMICC_AUDIO_269_112_2_18_2_10_30_263 "ccapp_a SwitchAudioDevice not need switch"
#define MMICC_AUDIO_275_112_2_18_2_10_30_264 "ccapp_a target dev: %d"
#define MMICC_AUDIO_280_112_2_18_2_10_30_265 "ccapp_a SwitchAudioDevice end, status:%d, %d, %d"
#define MMICC_AUDIO_360_112_2_18_2_10_30_266 "ccapp_a ChangeStatusToHH result:%d"
#define MMICC_AUDIO_433_112_2_18_2_10_30_267 "ccapp_a: ChangeStatusToEP result:%d"
#define MMICC_AUDIO_465_112_2_18_2_10_30_268 "ccapp_a: ChangeStatusToHF result:%d"
#define MMICC_AUDIO_497_112_2_18_2_10_30_269 "ccapp_a: ChangeStatusToBT result:%d"
#define MMICC_AUDIO_509_112_2_18_2_10_30_270 "ccapp_a SwitchMode dev:%d"
#define MMICC_AUDIO_576_112_2_18_2_10_31_271 "ccapp_a ConstructAudioHandle error"
#define MMICC_AUDIO_579_112_2_18_2_10_31_272 "ccapp_a ConstructAudioHandle"
#define MMICC_AUDIO_608_112_2_18_2_10_31_273 "ccapp_a DestroyAudioHandle handle not being"
#define MMICC_AUDIO_611_112_2_18_2_10_31_274 "ccapp_a DestroyAudioHandle"
#define MMICC_AUDIO_633_112_2_18_2_10_31_275 "ccapp_a ConstructVoiceCodec codec has been enable"
#define MMICC_AUDIO_636_112_2_18_2_10_31_276 "ccapp_a ConstructVoiceCodec"
#define MMICC_AUDIO_692_112_2_18_2_10_31_277 "ccapp_a DestroyVoiceCodec handle not being"
#define MMICC_AUDIO_695_112_2_18_2_10_31_278 "ccapp_a DestroyVoiceCodec"
#define MMICC_AUDIO_711_112_2_18_2_10_31_279 "ccapp_a VoiceCodec is same state"
#define MMICC_AUDIO_714_112_2_18_2_10_31_280 "ccapp_a VoiceCodec enable: %d"
#define MMICC_AUDIO_747_112_2_18_2_10_31_281 "ccapp_a VoiceCodec status: %d"
#define MMICC_AUDIO_768_112_2_18_2_10_31_282 "ccapp_a: MMICC_EnableMute is_mute:%d"
#define MMICC_AUDIO_771_112_2_18_2_10_31_283 "ccapp_a mute is same state"
#define MMICC_AUDIO_799_112_2_18_2_10_31_284 "ccapp_a: MMICC_EnableRemoteMute is_enable:%d"
#define MMICC_AUDIO_802_112_2_18_2_10_31_285 "ccapp_a remote mute is same state"
#define MMICC_AUDIO_851_112_2_18_2_10_31_286 "ccapp: StartRingOrVibrate type: %d"
#define MMICC_AUDIO_988_112_2_18_2_10_31_287 "HandleVibrateTimer: call_ring_type is %d"
#define MMICC_AUDIO_1219_112_2_18_2_10_32_288 "ccapp PlayCustomRing valid: %d, type: %d"
#define MMICC_AUDIO_1289_112_2_18_2_10_32_289 "ccapp PlayCustomRing default"
#define MMICC_MAIN_123_112_2_18_2_10_33_290 "ccapp Applet_HandleEvent msg_id = 0x%04x"
#define MMICC_MAIN_227_112_2_18_2_10_33_291 "ccapp CC_Initialize"
#define MMICC_MAIN_263_112_2_18_2_10_33_292 "ccapp CC_Finalize"
#define MMICC_MAIN_289_112_2_18_2_10_33_293 "ccapp StartCCApplet"
#define MMICC_MAIN_305_112_2_18_2_10_33_294 "ccapp Stop cc applet"
#define MMICC_MAIN_324_112_2_18_2_10_33_295 "MMIAPICC_SendMsgToApplet msg:%d"
#define MMICC_MAIN_358_112_2_18_2_10_33_296 "ccapp ActiveCC result:%d"
#define MMICC_MAIN_384_112_2_18_2_10_33_297 "ccapp HideCC result:%d"
#define MMICC_SPEEDDIAL_567_112_2_18_2_10_36_298 "mmi_speeddial.c: CC_HandleSpeedDialListWinMsg( %d, %d)"
#define MMICC_SPEEDDIAL_920_112_2_18_2_10_37_299 "mmi_speeddial.c: CC_HandleSpeedDialMenuWinMsg( %d, %d)"
#define MMICC_SPEEDDIAL_1451_112_2_18_2_10_38_300 "GetSelectedItem PNULL == number_ptr || PNULL == number_len_ptr"
#define MMICC_SPEEDDIAL_1481_112_2_18_2_10_38_301 "SaveSpeedDialItem PNULL == number_ptr"
#define MMICC_SPEEDDIAL_1648_112_2_18_2_10_39_302 "GenSpeedStr speedkey_entry_ptrm, name_str_ptr,speedkey_entry_ptr->number_len maybe null"
#define MMICC_SPEEDDIAL_1660_112_2_18_2_10_39_303 "ccapp GenSpeedStr mem alloc failed"
#define MMICC_SPEEDDIAL_1769_112_2_18_2_10_39_304 "ccapp SpeedDial error key_num = %d"
#define MMICC_SPEEDDIAL_1778_112_2_18_2_10_39_305 "[SPEEDDIAL] MMIAPICC_SpeedDial ret_val %d"
#define MMICC_SPEEDDIAL_1831_112_2_18_2_10_39_306 "ccapp SpeedDial error key_num = %d"
#define MMICC_SPEEDDIAL_1840_112_2_18_2_10_39_307 "[SPEEDDIAL] MMIAPICC_SpeedDial ret_val %d"
#define MMICC_SPEEDDIAL_1883_112_2_18_2_10_39_308 "AppendItemToListBox string_ptr"
#define MMICC_WINTAB_2276_112_2_18_2_10_44_309 "ccapp.c:enter function CC_HandleCcLockMsg() msg_id = %x"
#define MMICC_WINTAB_2594_112_2_18_2_10_45_310 "ccapp.c:enter function CC_HandleCcLockMsg() WRONG param! enter default branch!!!"
#define MMICC_WINTAB_2651_112_2_18_2_10_45_311 "mmiccapp:OpenCCHoldMenu, cur dev = %d"
#define MMICC_WINTAB_3240_112_2_18_2_10_46_312 "ccapp HandleCallSIMSelectWinMsg,item_data[%d] >= MMI_DUAL_SYS_MAX,ERROR!"
#define MMICC_WINTAB_3247_112_2_18_2_10_46_313 "ccapp HandleCallSIMSelectWinMsg make info ptr is null"
#define MMICC_WINTAB_3318_112_2_18_2_10_47_314 "mmicc: CC_CallStateDisplaying, s_call_context.dual_sys = %d, sim_name.str_arr = %s"
#define MMICC_WINTAB_3326_112_2_18_2_10_47_315 "ccapp str is too long"
#define MMICC_WINTAB_3391_112_2_18_2_10_47_316 "CC_SetCallWinSoftkey:win_id %d is not open!"
#define MMICC_WINTAB_3718_112_2_18_2_10_47_317 "ccapp.c:MMICC_HandleInsertDTMFWinMsg() send dtmf failure"
#define MMICC_WINTAB_5067_112_2_18_2_10_50_318 "ccapp err DisplayCallInfoForSingle, null ptr"
#define MMICC_WINTAB_5428_112_2_18_2_10_51_319 "ccapp err DisplayCallInfoForSingle, null ptr"
#define MMICC_WINTAB_5472_112_2_18_2_10_51_320 "ccapp err DisplayCallInfoForSingle, null ptr"
#define MMICC_WINTAB_5735_112_2_18_2_10_52_321 "ccapp.c:enter function PdaDisplayDisconnectWin():call_index=%d, cause=%d"
#define MMICC_WINTAB_5739_112_2_18_2_10_52_322 "ccapp disc win not open as active call"
#define MMICC_WINTAB_5931_112_2_18_2_10_52_323 "ccapp err DisplayCallInfoForSingle, null ptr"
#define MMICC_WINTAB_6510_112_2_18_2_10_53_324 "GetCallStateText state_ptr is NULL"
#define MMICC_WINTAB_6606_112_2_18_2_10_54_325 "ccapp.c:enter function CC_HandleCcAnimWinMsg() msg_id = %x"
#define MMICC_WINTAB_6678_112_2_18_2_10_54_326 "ccapp.c CC_HandleCcAnimWinMsg() headset is plugout!"
#define MMICC_WINTAB_6846_112_2_18_2_10_54_327 "MMIASP WAIT TIME OUT, asp answer fail"
#define MMICC_WINTAB_6992_112_2_18_2_10_54_328 "MMINV_SET_IS_FLIP_ANSWER = %d"
#define MMICC_WINTAB_7102_112_2_18_2_10_55_329 "ccapp.c: CC_HandleCcWinMsg() call_state = %d"
#define MMICC_WINTAB_7267_112_2_18_2_10_55_330 "ccapp.c: CC_HandleCcWinMsg() msg_id = %d"
#define MMICC_WINTAB_7751_112_2_18_2_10_56_331 "ccapp CreatePhotoAnim null ptr"
#define MMICC_WINTAB_8015_112_2_18_2_10_57_332 "ccapp.c:enter function CC_OpenMtCallWin()"
#define MMICC_WINTAB_8111_112_2_18_2_10_57_333 "ccapp.c:enter function CC_OpenCallProcessingWin()"
#define MMICC_WINTAB_8146_112_2_18_2_10_57_334 "ccapp.c:enter function CC_DisplayDisconnectWin():call_index=%d, cause=%d"
#define MMICC_WINTAB_8179_112_2_18_2_10_57_335 "CC_DisplayDisconnectWin call_state = %d, call_time = %d"
#define MMICC_WINTAB_8301_112_2_18_2_10_58_336 "ccapp.c:enter function CC_OpenMissedCallWin()"
#define MMICC_WINTAB_8310_112_2_18_2_10_58_337 "ccapp error char_num_len = %d, missed_call_str = %d"
#define MMICC_WINTAB_8354_112_2_18_2_10_58_338 "ccapp.c:enter function CC_OpenMissedCallWin() mem alloc FAILED"
#define MMICC_WINTAB_8709_112_2_18_2_10_59_339 "ccapp.c:enter function MMICC_HandleAutoDialWinMsg() msg_id = %d"
#define MMICC_WINTAB_8916_112_2_18_2_10_59_340 "ccapp.c: MMICC_HandleAutoDialWinMsg() msg_id = %d"
#define MMICC_WINTAB_8935_112_2_18_2_11_0_341 "GetRedialedTimes redialed_times_ptr is NULL"
#define MMICC_WINTAB_8979_112_2_18_2_11_0_342 "ccapp.c:enter function CC_OpenAutoDialWindow()"
#define MMICC_WINTAB_9015_112_2_18_2_11_0_343 "ccapp.c:enter function CC_OpenAutoDialWindow()"
#define MMICC_WINTAB_9191_112_2_18_2_11_0_344 "---mmiccapp:MMICC_HandleCCMenuWinMsg"
#define MMICC_WINTAB_9362_112_2_18_2_11_1_345 "mmiccapp:MMICC_HandleCCMenuWinMsg, ID_CC_RELEASE_ALL"
#define MMICC_WINTAB_9373_112_2_18_2_11_1_346 "mmiccapp:MMICC_HandleCCMenuWinMsg, ID_CC_BUILDMPTY"
#define MMICC_WINTAB_9384_112_2_18_2_11_1_347 "mmiccapp:MMICC_HandleCCMenuWinMsg, ID_CC_HANDHOLD"
#define MMICC_WINTAB_9418_112_2_18_2_11_1_348 "mmiccapp:MMICC_HandleCCMenuWinMsg, ID_HANDFREE_SWITCH"
#define MMICC_WINTAB_9465_112_2_18_2_11_1_349 "MMICC_HandleCCMenuWinMsg: the menu id is error"
#define MMICC_WINTAB_9492_112_2_18_2_11_1_350 "MMICC_HandleCCMenuWinMsg() msg_id = %d"
#define MMICC_WINTAB_9504_112_2_18_2_11_1_351 "ccapp.c:enter function OpenCCMenu()"
#define MMICC_WINTAB_9823_112_2_18_2_11_2_352 "ccapp err index: %d, state : %d"
#define MMICC_WINTAB_9958_112_2_18_2_11_2_353 "ccapp.c:enter function CC_OpenCallList()"
#define MMICC_WINTAB_10007_112_2_18_2_11_3_354 "ccapp.c:enter function CC_CallList()"
#define MMICC_WINTAB_10079_112_2_18_2_11_3_355 "ccapp.c:enter function OpenAdjustVolumeWin()"
#define MMICC_WINTAB_10293_112_2_18_2_11_3_356 "MMIAPICC_GetBtSpeakerVol=%d"
#define MMICC_WINTAB_10536_112_2_18_2_11_4_357 "mmiccapp: CC_HOLDMENU_NODE_HANDHOLD"
#define MMICC_WINTAB_10558_112_2_18_2_11_4_358 "mmiccapp: CC_HOLDMENU_NODE_HANDFREE"
#define MMICC_WINTAB_10672_112_2_18_2_11_4_359 "ccapp.c:enter function CC_HandleCcProcessingWinMsg() msg_id = %d"
#define MMICC_WINTAB_11057_112_2_18_2_11_5_360 "ccapp.c:enter function CC_CreateNewHotButtonWithoutForm() PNULL == s_current_win_button_order_ptr!"
#define MMICC_WINTAB_11193_112_2_18_2_11_5_361 "ccapp.c:enter function MMICC_CloseDtmfPanel() else WARRNING!"
#define MMICC_WINTAB_11213_112_2_18_2_11_5_362 "ccapp.c:enter function MMICC_CloseDtmfPanel() else WARRNING!"
#define MMICC_WINTAB_11322_112_2_18_2_11_5_363 "ccapp.c:enter function CC_UpdateCcDisplayType() default case WARRNING!"
#define MMICC_WINTAB_11703_112_2_18_2_11_6_364 "ccapp.c:enter function CC_InitHotButtonState() default WARRNING!"
#define MMICC_WINTAB_11733_112_2_18_2_11_6_365 "ccapp HandleCCWinWebMsg button id: %d"
#define MMICC_WINTAB_11815_112_2_18_2_11_6_366 "ccapp HandleCCWinWebMsg !MMIAPIASP_WorkOneTime()"
#define MMICC_WINTAB_11819_112_2_18_2_11_6_367 "ccapp HandleCCWinWebMsg !AnswerASPCall()"
#define MMICC_WINTAB_11850_112_2_18_2_11_6_368 "ccapp.c:enter function CC_UpdateCCWinHotButton(), PNULL == s_current_win_button_order_ptr ERROR!"
#define MMICC_WINTAB_11874_112_2_18_2_11_6_369 "ccapp CC_FindNextHotButton didn't find the current focus button!"
#define MMICC_WINTAB_11931_112_2_18_2_11_6_370 "ccapp CC_FindNextHotButton didn't find the next focus button!"
#define MMICC_WINTAB_11985_112_2_18_2_11_6_371 "ccapp CC_FindNextHotButton didn't find the next focus button!"
#define MMICC_WINTAB_12033_112_2_18_2_11_7_372 "ccapp CC_FindNextHotButton didn't find the next focus button!"
#define MMICC_WINTAB_12081_112_2_18_2_11_7_373 "ccapp CC_FindNextHotButton didn't find the next focus button!"
#define MMICC_WINTAB_12215_112_2_18_2_11_7_374 "ccapp.c:enter function CC_UpdateCCWinHotButton(), PNULL == s_current_win_button_order_ptr ERROR!"
#define MMICC_WINTAB_12422_112_2_18_2_11_7_375 "ccapp HandleCCWinWebMsg !MMIAPIASP_WorkOneTime()"
#define MMICC_WINTAB_12426_112_2_18_2_11_7_376 "ccapp HandleCCWinWebMsg !AnswerASPCall()"
#define MMICC_WINTAB_12533_112_2_18_2_11_8_377 "mmicc_app.c HandleCCWinPenOkMsg edit_ctrl_id is not active ctrl, can't handle the message!!!"
#define MMICC_WINTAB_12594_112_2_18_2_11_8_378 "ccapp.c:enter function CC_HandleCcWinMsg() msg_id = %x"
#define MMICC_WINTAB_12652_112_2_18_2_11_8_379 "ccapp.c CC_HandleCcWinMsg() headset is plugout!"
#define MMICC_WINTAB_12817_112_2_18_2_11_8_380 "ccapp.c:CC_SetupCompleteCnf() send dtmf failure"
#define MMICC_WINTAB_12941_112_2_18_2_11_8_381 "ccapp.c: CC_HandleCcWinMsg() call_state = %d"
#define MMICC_WINTAB_13025_112_2_18_2_11_8_382 "ccapp.c: CC_HandleCcWinMsg() call_state = %d"
#define MMICC_WINTAB_13135_112_2_18_2_11_9_383 "ccapp.c: CC_HandleCcWinMsg() msg_id = %d"
#define MMICC_WINTAB_13157_112_2_18_2_11_9_384 "ccapp.c:enter function CC_HandleDisconnectWinMsg(),msg_id=%d"
#define MMICC_WINTAB_13216_112_2_18_2_11_9_385 "ccapp.c:enter function CC_HandleDisconnectWinMsg(), MSG_TIMER"
#define MMICC_WINTAB_13223_112_2_18_2_11_9_386 "ccapp.c: CC_HandleDisconnectWinMsg, tele_num = %s, tele_len = %d"
#define MMICC_WINTAB_13228_112_2_18_2_11_9_387 "ccapp.c: s_call_repeat_info.disc_cause = %d, s_autodial_num = %d"
#define MMICC_WINTAB_13396_112_2_18_2_11_9_388 "ccapp.c: CC_HandleDisconnectWinMsg() msg_id = %d"
#define MMICC_WINTAB_13411_112_2_18_2_11_9_389 "ccapp:PlayCallEndHint"
#define MMICC_WINTAB_13506_112_2_18_2_11_9_390 "ccapp.c: MMICC_HandleMissedcallWinMsg() msg_id = %d"
#define MMICC_WINTAB_13676_112_2_18_2_11_10_391 "CC_SendSMSAfterRejectMTCall name_ptr is NULL"
#define MMICC_WINTAB_13824_112_2_18_2_11_10_392 "mmiccapp: HandleEmergencyCallWinMsg, msg_id = %d"
#define MMICC_WINTAB_13906_112_2_18_2_11_10_393 "ccapp.c: MMICC_SetRepeatInfoCause, s_call_repeat_info.disc_cause = %d"
#define MMICC_WINTAB_14206_112_2_18_2_11_11_394 "ccapp:AutoAnswerIncomingCall"
#define MMICC_WINTAB_14223_112_2_18_2_11_11_395 "cc app MMICC_UpdateCallStatusDisplay error param: %d"
#define MMICC_WINTAB_14532_112_2_18_2_11_11_396 "ccapp OpenMagicVoice mv cannot open"
#define MMICC_WINTAB_14538_112_2_18_2_11_11_397 "ccapp OpenMagicVoice mv is working"
#define MMICC_WINTAB_14591_112_2_18_2_11_12_398 "ccapp SetMagicVoiceParam mv is not working"
#define MMICC_WINTAB_14623_112_2_18_2_11_12_399 "CopyMagicVoiceData dst_data_ptr OR src_data_ptr is NULL"
#define MMICC_WINTAB_14641_112_2_18_2_11_12_400 "ccapp CloseMagicVoice mv is not working"
#define MMICC_WINTAB_14828_112_2_18_2_11_12_401 "CopyMagicVoiceData item_t_ptr OR item_data_ptr is NULL"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_CC_TRC)
TRACE_MSG(MMICC_APP_966_112_2_18_2_10_8_0,"ccapp.c:enter function CC_HandlePsMsg(), msg_id = %d")
TRACE_MSG(MMICC_APP_970_112_2_18_2_10_8_1,"ccapp.c: CC_HandlePsMsg() app_ptr == PNULL")
TRACE_MSG(MMICC_APP_1022_112_2_18_2_10_8_2,"ccapp.c:enter function CC_HandlePsMsg(), APP_MN_CALL_DISCONNECTED_IND")
TRACE_MSG(MMICC_APP_1075_112_2_18_2_10_8_3,"ccapp.c:enter function CC_HandlePsMsg(), APP_MN_CALL_ERR_IND, dis_cause = %d")
TRACE_MSG(MMICC_APP_1081_112_2_18_2_10_8_4,"ccapp: CC_HandlePsMsg->APP_MN_SETUP_IND")
TRACE_MSG(MMICC_APP_1088_112_2_18_2_10_8_5,"ccapp disc sim %d mo call")
TRACE_MSG(MMICC_APP_1127_112_2_18_2_10_9_6,"ccapp.c CC_HandlePsMsg, dual_sys = %d, s_call_ready = %d")
TRACE_MSG(MMICC_APP_1131_112_2_18_2_10_9_7,"ccapp error call_ready_param_ptr.dual_sys = %d")
TRACE_MSG(MMICC_APP_1178_112_2_18_2_10_9_8,"ccapp.c: CC_HandlePsMsg() CC isn't process, msg_id = %d")
TRACE_MSG(MMICC_APP_1229_112_2_18_2_10_9_9,"ccapp.c:enter function HandleMTFWDisconnected() disconnected cause = %d, call_id = %d")
TRACE_MSG(MMICC_APP_1247_112_2_18_2_10_9_10,"[MMICC]: HandleSIMPlugNotifySTKFunc dual_sys=%d, notify_event=%d")
TRACE_MSG(MMICC_APP_1345_112_2_18_2_10_9_11,"ccapp.c: ProcessPhoneNumExt() tele_num == PNULL")
TRACE_MSG(MMICC_APP_1356_112_2_18_2_10_9_12,"ccapp: dial_method error %d")
TRACE_MSG(MMICC_APP_1382_112_2_18_2_10_9_13,"ccapp.c: ProcessPhoneNumExt() req is reject by vt")
TRACE_MSG(MMICC_APP_1389_112_2_18_2_10_9_14,"ccapp.c:ProcessPhoneNumExt() tele_len = 0")
TRACE_MSG(MMICC_APP_1399_112_2_18_2_10_9_15,"mmicc: ProcessPhoneNumExt, dual_sys = %d, tele_num = %s, tele_len =%d, call_type = %d")
TRACE_MSG(MMICC_APP_1404_112_2_18_2_10_9_16,"ccapp.c:ProcessPhoneNumExt() call MMIAPICC_MakeCallReq error")
TRACE_MSG(MMICC_APP_1465_112_2_18_2_10_9_17,"ccapp.c: MMIAPICC_MakeCallReq() tele_num == PNULL")
TRACE_MSG(MMICC_APP_1481_112_2_18_2_10_9_18,"ccapp error dualsys = %d, is_call_video_call = %d")
TRACE_MSG(MMICC_APP_1498_112_2_18_2_10_9_19,"ccapp.c:MMIAPICC_MakeCallReq() call_number= %d")
TRACE_MSG(MMICC_APP_1503_112_2_18_2_10_9_20,"ccapp.c:MMIAPICC_MakeCallReq() tele_len= %d")
TRACE_MSG(MMICC_APP_1509_112_2_18_2_10_9_21,"ccapp.c:MMIAPICC_MakeCallReq() tele_len = 0")
TRACE_MSG(MMICC_APP_1517_112_2_18_2_10_9_22,"ccapp.c:MakeCallReqExt() CC_GenPartyNumber return failure")
TRACE_MSG(MMICC_APP_1539_112_2_18_2_10_9_23,"ccapp.c:MMIAPICC_MakeCallReq() CC_ProcessExtensionNum error")
TRACE_MSG(MMICC_APP_1548_112_2_18_2_10_9_24,"ccapp.c:MMIAPICC_MakeCallReq() call_type = MN_CALL_TYPE_EMERGENCY")
TRACE_MSG(MMICC_APP_1559_112_2_18_2_10_9_25,"ccapp.c:MMIAPICC_MakeCallReq() setup_ind msg is being")
TRACE_MSG(MMICC_APP_1569_112_2_18_2_10_9_26,"ccapp.c:MakeCallReqExt() CC_GenPartyNumber return failure")
TRACE_MSG(MMICC_APP_1590_112_2_18_2_10_10_27,"mmiccapp: s_call_context.want_mo_call.address.num_len = %d, tele_num = %s, tele_len = %d")
TRACE_MSG(MMICC_APP_1727_112_2_18_2_10_10_28,"ccapp.c:MakeCallReqExt() mmi_clir_type=%d")
TRACE_MSG(MMICC_APP_1731_112_2_18_2_10_10_29,"ERR: ccapp dial method %d")
TRACE_MSG(MMICC_APP_1754_112_2_18_2_10_10_30,"ccapp.c:MMIAPICC_MakeCallReq() MNCALL_StartCall Fail by FDN")
TRACE_MSG(MMICC_APP_1759_112_2_18_2_10_10_31,"ccapp.c:MMIAPICC_MakeCallReq() MNCALL_StartCall return failure")
TRACE_MSG(MMICC_APP_1833_112_2_18_2_10_10_32,"MMIAPICC_RemoveIpNumber:%s,%d")
TRACE_MSG(MMICC_APP_1847_112_2_18_2_10_10_33,"ccapp error origin_number_len = %d, tele_len = %d, ip_number_len = %d")
TRACE_MSG(MMICC_APP_1875_112_2_18_2_10_10_34,"ccapp.c: GetNameFromPb() name_ptr  ||entry_id_ptr || group_id_ptr is null")
TRACE_MSG(MMICC_APP_1915_112_2_18_2_10_10_35,"ccapp.c, GetNameFromPb search_result=%d")
TRACE_MSG(MMICC_APP_1935_112_2_18_2_10_10_36,"MMIAPICC_GetExitedCallCount: The active call number is %d")
TRACE_MSG(MMICC_APP_1961_112_2_18_2_10_10_37,"MMIAPPICC_BtAcceptCall: hold and answer call!")
TRACE_MSG(MMICC_APP_1969_112_2_18_2_10_10_38,"MMIAPPICC_BtAcceptCall: the call status is %d")
TRACE_MSG(MMICC_APP_1984_112_2_18_2_10_10_39,"MMIAPICC_BtCancelCall: the call status is %d")
TRACE_MSG(MMICC_APP_2028_112_2_18_2_10_10_40,"ccapp.c:enter function CC_CallStartInd(), call_id = %d")
TRACE_MSG(MMICC_APP_2031_112_2_18_2_10_10_41,"ccapp.c: CC_CallStartInd() pSig == PNULL")
TRACE_MSG(MMICC_APP_2037_112_2_18_2_10_10_42,"ccapp.c: CC_CallStartInd() , call_id = %d")
TRACE_MSG(MMICC_APP_2097_112_2_18_2_10_11_43,"---WD:CC_CallStartInd deal with hook")
TRACE_MSG(MMICC_APP_2116_112_2_18_2_10_11_44,"ccapp.c:enter function CC_SyncInd(), sync_cause = %d")
TRACE_MSG(MMICC_APP_2119_112_2_18_2_10_11_45,"ccapp.c: CC_SyncInd() pSig == PNULL")
TRACE_MSG(MMICC_APP_2128_112_2_18_2_10_11_46,"ccapp SyncInd current call index:%d")
TRACE_MSG(MMICC_APP_2133_112_2_18_2_10_11_47,"ccapp SyncInd param mtfw index:%d")
TRACE_MSG(MMICC_APP_2171_112_2_18_2_10_11_48,"ccapp.c:enter function CC_CallAlertingInd(), call_id = %d, alerting_type = %d")
TRACE_MSG(MMICC_APP_2174_112_2_18_2_10_11_49,"ccapp.c: CC_CallAlertingInd() pSig == PNULL")
TRACE_MSG(MMICC_APP_2179_112_2_18_2_10_11_50,"ccapp.c: CC_CallAlertingInd() , call_id = %d")
TRACE_MSG(MMICC_APP_2217_112_2_18_2_10_11_51,"ccapp.c:CC_CallAlertingInd() alerting_type = %d")
TRACE_MSG(MMICC_APP_2232_112_2_18_2_10_11_52,"ccapp.c:enter function CC_CallProgressInd(), descrip = %d")
TRACE_MSG(MMICC_APP_2235_112_2_18_2_10_11_53,"ccapp.c: CC_CallProgressInd() pSig == PNULL")
TRACE_MSG(MMICC_APP_2269_112_2_18_2_10_11_54,"ccapp.c:enter function CC_SetupCompleteCnf(), pSig = NULL ||pSig->call_id >= 7")
TRACE_MSG(MMICC_APP_2272_112_2_18_2_10_11_55,"ccapp.c:enter function CC_SetupCompleteCnf(), call_id = %d")
TRACE_MSG(MMICC_APP_2282_112_2_18_2_10_11_56,"ccapp.c:CC_SetupCompleteCnf() current_call invalid %d")
TRACE_MSG(MMICC_APP_2313_112_2_18_2_10_11_57,"---WD:CC_SetupCompleteCnf deal with hook")
TRACE_MSG(MMICC_APP_2355_112_2_18_2_10_11_58,"ccapp.c:enter function CC_ReleaseCallReq(), dual_sys = %d, call_id = %d, cause = %d")
TRACE_MSG(MMICC_APP_2360_112_2_18_2_10_11_59,"ccapp.c: CC_ReleaseCallReq , call_id = %d")
TRACE_MSG(MMICC_APP_2368_112_2_18_2_10_11_60,"ccapp.c:CC_ReleaseCallReq() current_call invalid %d")
TRACE_MSG(MMICC_APP_2375_112_2_18_2_10_11_61,"ccapp.c:CC_ReleaseCallReq() MNCALL_DiscCall return failure")
TRACE_MSG(MMICC_APP_2441_112_2_18_2_10_11_62,"ccapp.c:enter function HandleCallForwardInd(), pSig = NULL ||pSig->call_id >= 7")
TRACE_MSG(MMICC_APP_2444_112_2_18_2_10_11_63,"ccapp.c:enter function HandleCallForwardInd(), call_id = %d")
TRACE_MSG(MMICC_APP_2449_112_2_18_2_10_11_64,"ccapp.c:HandleCallForwardInd() mtfw")
TRACE_MSG(MMICC_APP_2491_112_2_18_2_10_11_65,"ccapp.c:enter function HandleForwardStatusInd(), pSig = NULL ||pSig->call_id >= MMI_DUAL_SYS_MAX")
TRACE_MSG(MMICC_APP_2494_112_2_18_2_10_11_66,"ccapp: HandleForwardStatusInd oper: %d stat: %d")
TRACE_MSG(MMICC_APP_2499_112_2_18_2_10_11_67,"ccapp.c:HandleCallForwardInd() mtfw")
TRACE_MSG(MMICC_APP_2605_112_2_18_2_10_12_68,"ccapp.c:enter function CC_DisconnectedCall()")
TRACE_MSG(MMICC_APP_2608_112_2_18_2_10_12_69,"ccapp.c: CC_DisconnectedCall() param == PNULL")
TRACE_MSG(MMICC_APP_2622_112_2_18_2_10_12_70,"ccapp.c  disconnected call_id = %d, cause = %d")
TRACE_MSG(MMICC_APP_2630_112_2_18_2_10_12_71,"APP_MN_CALL_ERR_IND:call_id = %d, cause = %d")
TRACE_MSG(MMICC_APP_2641_112_2_18_2_10_12_72,"ccapp.c: CC_DisconnectedCall() , call_id = %d")
TRACE_MSG(MMICC_APP_2672_112_2_18_2_10_12_73,"ccapp.c ERROR:CC_DisconnectedCall() current_call invalid %d")
TRACE_MSG(MMICC_APP_2731_112_2_18_2_10_12_74,"ccapp.c: CC_DisconnectedCallByIndex() invalid index %d")
TRACE_MSG(MMICC_APP_2780_112_2_18_2_10_12_75,"ccapp.c:CC_DisconnectedCallByIndex() call_state = %d")
TRACE_MSG(MMICC_APP_2800_112_2_18_2_10_12_76,"CC_DisconnectedCallByIndex :Call the function: BT_StopAlert")
TRACE_MSG(MMICC_APP_2812_112_2_18_2_10_12_77,"CC_DisconnectedCallByIndex :Call the function: BT_StopRing")
TRACE_MSG(MMICC_APP_2818_112_2_18_2_10_12_78,"ccapp.c:CC_DisconnectedCallByIndex() call_state = %d")
TRACE_MSG(MMICC_APP_2825_112_2_18_2_10_12_79,"CC_DisconnectedCallByIndex :Call the function: BT_CallDisconnected")
TRACE_MSG(MMICC_APP_3045_112_2_18_2_10_13_80,"mmiccapp:CC_DisconnectedCallByIndex,operated_by_user = %d, call_state = %d")
TRACE_MSG(MMICC_APP_3214_112_2_18_2_10_13_81,"ccapp.c:enter function CC_CallSetupInd(), s_call_context.dual_sys = %d, s_call_context.call_number = %d")
TRACE_MSG(MMICC_APP_3217_112_2_18_2_10_13_82,"ccapp.c: CC_CallSetupInd() pSig == PNULL")
TRACE_MSG(MMICC_APP_3224_112_2_18_2_10_13_83,"ccapp.c:CC_CallSetupInd() call_number= %d")
TRACE_MSG(MMICC_APP_3229_112_2_18_2_10_13_84,"ccapp.c: CC_CallSetupInd() , call_id = %d")
TRACE_MSG(MMICC_APP_3315_112_2_18_2_10_13_85,"ccapp.c, CC_CallSetupInd search_result=%d, name.wstr_len=%d")
TRACE_MSG(MMICC_APP_3340_112_2_18_2_10_13_86,"%d,%d,%d")
TRACE_MSG(MMICC_APP_3399_112_2_18_2_10_13_87,"ccapp.c:enter function CC_ConnectCallReq()")
TRACE_MSG(MMICC_APP_3410_112_2_18_2_10_13_88,"ccapp.c: CC_ConnectCallReq(), call MNCALL_ConnectCall failure")
TRACE_MSG(MMICC_APP_3442_112_2_18_2_10_13_89,"ccapp.c:enter function CC_SetupCompleteInd(), call_id = %d")
TRACE_MSG(MMICC_APP_3445_112_2_18_2_10_13_90,"ccapp.c: CC_SetupCompleteInd() sig_ptr == PNULL")
TRACE_MSG(MMICC_APP_3450_112_2_18_2_10_13_91,"ccapp.c: CC_SetupCompleteInd() , call_id = %d")
TRACE_MSG(MMICC_APP_3526_112_2_18_2_10_13_92,"CC_SetupCompleteInd(), call_info.call_type= %d")
TRACE_MSG(MMICC_APP_3559_112_2_18_2_10_14_93,"ccapp.c: CC_ProcessExtensionNum() tele_num == PNULL")
TRACE_MSG(MMICC_APP_3775_112_2_18_2_10_14_94,"ccapp.c: CC_GetIndexOfCallId , call_id = %d")
TRACE_MSG(MMICC_APP_3799_112_2_18_2_10_14_95,"ccapp.c:enter function CC_DeleteCallInfo(), call_index = %d")
TRACE_MSG(MMICC_APP_3802_112_2_18_2_10_14_96,"ccapp.c: CC_DeleteCallInfo , call_index = %d")
TRACE_MSG(MMICC_APP_3901_112_2_18_2_10_14_97,"ccapp MMIAPICC_IsEmergencyNum error dual_sys = %d")
TRACE_MSG(MMICC_APP_3907_112_2_18_2_10_14_98,"ccapp.c: MMIAPICC_IsEmergencyNum tele_num == PNULL")
TRACE_MSG(MMICC_APP_3913_112_2_18_2_10_14_99,"ccapp.c: MMIAPICC_IsEmergencyNum tele_len == 0")
TRACE_MSG(MMICC_APP_3923_112_2_18_2_10_14_100,"ccapp.c: MMIAPICC_IsEmergencyNum temp_tele_len == 0")
TRACE_MSG(MMICC_APP_3951_112_2_18_2_10_14_101,"ccapp.c: MMIAPICC_IsEmergencyNum is emergency call")
TRACE_MSG(MMICC_APP_3959_112_2_18_2_10_14_102,"MMIAPICC_IsEmergencyCall: emc num=%d")
TRACE_MSG(MMICC_APP_3978_112_2_18_2_10_14_103,"MMIAPICC_IsEmergencyNum: ecc_code=%s")
TRACE_MSG(MMICC_APP_3983_112_2_18_2_10_14_104,"ccapp.c: MMIAPICC_IsEmergencyNum() is sim emc")
TRACE_MSG(MMICC_APP_4007_112_2_18_2_10_14_105,"ccapp.c: MMIAPICC_IsEmergencyNum is s_emergency_call_no_sim call %d")
TRACE_MSG(MMICC_APP_4036_112_2_18_2_10_14_106,"ccapp CC_IsSimOk error dual_sys = %d")
TRACE_MSG(MMICC_APP_4054_112_2_18_2_10_15_107,"ccapp: CC_OpenAudioTunnel codec_flag %d")
TRACE_MSG(MMICC_APP_4091_112_2_18_2_10_15_108,"ccapp.c:enter function CC_ReleaseActiveCall()")
TRACE_MSG(MMICC_APP_4101_112_2_18_2_10_15_109,"CC_ReleaseActiveCall , call_id = %d")
TRACE_MSG(MMICC_APP_4146_112_2_18_2_10_15_110,"ccapp.c:enter function CC_ReleaseHoldCall()")
TRACE_MSG(MMICC_APP_4156_112_2_18_2_10_15_111,"CC_ReleaseHoldCall , call_id = %d")
TRACE_MSG(MMICC_APP_4165_112_2_18_2_10_15_112,"CC_ReleaseHoldCall MNCALL_DiscCall return failure")
TRACE_MSG(MMICC_APP_4221_112_2_18_2_10_15_113,"ccapp.c:enter function MMIAPICC_ReleaseAllCallExt()")
TRACE_MSG(MMICC_APP_4226_112_2_18_2_10_15_114,"ccapp: ReleaseAllCall fail for at cmd")
TRACE_MSG(MMICC_APP_4234_112_2_18_2_10_15_115,"ccapp ReleaseAllCallExt error call_id = %d")
TRACE_MSG(MMICC_APP_4273_112_2_18_2_10_15_116,"ccapp.c:enter function CC_ReleaseAllCall()")
TRACE_MSG(MMICC_APP_4282_112_2_18_2_10_15_117,"CC_ReleaseHoldCall , call_id = %d")
TRACE_MSG(MMICC_APP_4302_112_2_18_2_10_15_118,"CC_ReleaseActiveCall MNCALL_ReleaseActiveCalls return failure")
TRACE_MSG(MMICC_APP_4331_112_2_18_2_10_15_119,"ccapp.c:enter function CC_HoldAndAnswerCW()")
TRACE_MSG(MMICC_APP_4346_112_2_18_2_10_15_120,"ccapp.c: CC_HoldAndAnswerCW() index = %d")
TRACE_MSG(MMICC_APP_4355_112_2_18_2_10_15_121,"CC_HoldAndAnswerCW: the result of MNCALL_HoldCall is error")
TRACE_MSG(MMICC_APP_4417_112_2_18_2_10_15_122,"ccapp.c:enter function CC_ReleaseAndAnswerCW()")
TRACE_MSG(MMICC_APP_4471_112_2_18_2_10_15_123,"ccapp.c: CC_HandleRetrieveCnf( param == NULL")
TRACE_MSG(MMICC_APP_4474_112_2_18_2_10_15_124,"ccapp.c:enter function CC_HandleRetrieveCnf(), call_id = %d, req_is_accepted = %d")
TRACE_MSG(MMICC_APP_4517_112_2_18_2_10_15_125,"ccapp HandleRetrieveCnf error call_id = %d")
TRACE_MSG(MMICC_APP_4552_112_2_18_2_10_16_126,"ccapp HandleRetrieveCnf call index %d")
TRACE_MSG(MMICC_APP_4584_112_2_18_2_10_16_127,"ccapp.c: CC_HandleHoldCnf( param == NULL")
TRACE_MSG(MMICC_APP_4587_112_2_18_2_10_16_128,"ccapp.c:enter function CC_HandleHoldCnf(), call_id = %d, req_is_accepted = %d")
TRACE_MSG(MMICC_APP_4608_112_2_18_2_10_16_129,"ccapp HandleHoldCnf1 call index: %d")
TRACE_MSG(MMICC_APP_4740_112_2_18_2_10_16_130,"ccapp HandleHoldCnf call index: %d")
TRACE_MSG(MMICC_APP_4771_112_2_18_2_10_16_131,"ccapp.c:enter function CC_HandleDisconnectedInMPTY(), call_index = %d, cause = %d")
TRACE_MSG(MMICC_APP_4780_112_2_18_2_10_16_132,"ccapp: mpty active: %d, hold: %d, outgoing: %d")
TRACE_MSG(MMICC_APP_4800_112_2_18_2_10_16_133,"ccapp DisconnectedInMPTY call_number: %d")
TRACE_MSG(MMICC_APP_4817_112_2_18_2_10_16_134,"the index of active call is %d")
TRACE_MSG(MMICC_APP_4850_112_2_18_2_10_16_135,"the index of active call is %d")
TRACE_MSG(MMICC_APP_4881_112_2_18_2_10_16_136,"the index of active call is %d")
TRACE_MSG(MMICC_APP_4984_112_2_18_2_10_16_137,"ccapp DisconnectedInMPTY current call:%d")
TRACE_MSG(MMICC_APP_5008_112_2_18_2_10_16_138,"mmiccapp: FindCallInModule , index = %d")
TRACE_MSG(MMICC_APP_5019_112_2_18_2_10_16_139,"mmiccapp: FindCallInModule , index = %d")
TRACE_MSG(MMICC_APP_5030_112_2_18_2_10_16_140,"mmiccapp: FindCallInModule , index = %d")
TRACE_MSG(MMICC_APP_5055_112_2_18_2_10_17_141,"ccapp.c:enter function CC_HoldCall()")
TRACE_MSG(MMICC_APP_5067_112_2_18_2_10_17_142,"ccapp HoldCall: invalid active idx: %d")
TRACE_MSG(MMICC_APP_5072_112_2_18_2_10_17_143,"CC_HoldCall: the active call is %s")
TRACE_MSG(MMICC_APP_5078_112_2_18_2_10_17_144,"ccapp HoldCall: invalid hold idx: %d")
TRACE_MSG(MMICC_APP_5082_112_2_18_2_10_17_145,"CC_HoldCall: the hold call is %s")
TRACE_MSG(MMICC_APP_5100_112_2_18_2_10_17_146,"ccapp HoldCall: invalid hold idx: %d")
TRACE_MSG(MMICC_APP_5118_112_2_18_2_10_17_147,"ccapp HoldCall: invalid hold idx: %d")
TRACE_MSG(MMICC_APP_5167_112_2_18_2_10_17_148,"ccapp.c:enter function CC_BuildMPTYCall()")
TRACE_MSG(MMICC_APP_5178_112_2_18_2_10_17_149,"ccapp error active_index = %d")
TRACE_MSG(MMICC_APP_5187_112_2_18_2_10_17_150,"ccapp error hold_index = %d")
TRACE_MSG(MMICC_APP_5231_112_2_18_2_10_17_151,"ccapp.c:enter function CC_BuildMPTYAndAnswerCW()")
TRACE_MSG(MMICC_APP_5283_112_2_18_2_10_17_152,"ccapp.c: CC_HandleBuildMPTYCnf( param == NULL")
TRACE_MSG(MMICC_APP_5286_112_2_18_2_10_17_153,"ccapp.c:enter function CC_HandleBuildMPTYCnf(), call_id = %d, req_is_accepted = %d")
TRACE_MSG(MMICC_APP_5291_112_2_18_2_10_17_154,"ccapp.c: CC_HandleBuildMPTYCnf() , call_id = %d")
TRACE_MSG(MMICC_APP_5299_112_2_18_2_10_17_155,"CC_HandleBuildMPTYCnf() invald call index %d")
TRACE_MSG(MMICC_APP_5334_112_2_18_2_10_17_156,"CC_HandleBuildMPTYCnf: MNCALL_HoldCall is fault")
TRACE_MSG(MMICC_APP_5492_112_2_18_2_10_17_157,"ccapp.c:enter function CC_SplitSpecCall(), call_id = %d")
TRACE_MSG(MMICC_APP_5497_112_2_18_2_10_17_158,"ccapp CC_SplitSpecCall error call_id:%d")
TRACE_MSG(MMICC_APP_5504_112_2_18_2_10_17_159,"ccapp CC_SplitSpecCall error active:%d, hold:%d")
TRACE_MSG(MMICC_APP_5511_112_2_18_2_10_17_160,"CC_SplitSpecCall: MNCALL_SplitMPTY return failure")
TRACE_MSG(MMICC_APP_5538_112_2_18_2_10_18_161,"CC_HandleSpecCallCnf( param == NULL")
TRACE_MSG(MMICC_APP_5544_112_2_18_2_10_18_162,"ccapp.c:enter function CC_HandleSplitCallCnf(), call_id = %d, req_is_accepted = %d")
TRACE_MSG(MMICC_APP_5547_112_2_18_2_10_18_163,"ccapp error sig_ptr->call_id = %d, call_index = %d")
TRACE_MSG(MMICC_APP_5587_112_2_18_2_10_18_164,"ccapp HandleSplitCallCnf call_index: %d")
TRACE_MSG(MMICC_APP_5667_112_2_18_2_10_18_165,"ccapp.c:enter function CC_MTCallFireWall()")
TRACE_MSG(MMICC_APP_5671_112_2_18_2_10_18_166,"ccapp.c: CC_MTCallFireWall() call_id=%d")
TRACE_MSG(MMICC_APP_5750_112_2_18_2_10_18_167,"CC_AutoMoCall: call_index = %d, cause = %d")
TRACE_MSG(MMICC_APP_5751_112_2_18_2_10_18_168,"CC_AutoMoCall: call_state = %d ")
TRACE_MSG(MMICC_APP_5789_112_2_18_2_10_18_169,"CC_AutoMoCall: Need auto dial = %d")
TRACE_MSG(MMICC_APP_5841_112_2_18_2_10_18_170,"ccapp.c:MMIAPICC_SendDtmfByChar() current_call invalid %d")
TRACE_MSG(MMICC_APP_5877_112_2_18_2_10_18_171,"ccapp.c:MMIAPICC_SendDtmfByChar() start dtmf failure")
TRACE_MSG(MMICC_APP_5884_112_2_18_2_10_18_172,"ccapp.c:MMIAPICC_SendDtmfByChar() stop dtmf failure")
TRACE_MSG(MMICC_APP_5921_112_2_18_2_10_18_173,"mmiapp.c: MMIAPICC_IsInState call is rejected by black list")
TRACE_MSG(MMICC_APP_5925_112_2_18_2_10_18_174,"mmiapp.c: MMIAPICC_IsInState, call_state = %d")
TRACE_MSG(MMICC_APP_6041_112_2_18_2_10_19_175,"ccapp.c:enter function GetCurrentCallNumber(), bcd_number_ptr = NULL ")
TRACE_MSG(MMICC_APP_6155_112_2_18_2_10_19_176,"CC_HandleMTCallDisc: the call is disconnected")
TRACE_MSG(MMICC_APP_6293_112_2_18_2_10_19_177,"ccapp ShowCallDiscCause:cause type = %d")
TRACE_MSG(MMICC_APP_6387_112_2_18_2_10_19_178,"ccapp.c:MMIAPICC_ReleaseAspCallReq() MNCALL_DiscCall return failure")
TRACE_MSG(MMICC_APP_6433_112_2_18_2_10_19_179,"ccapp: auto dial normal call...")
TRACE_MSG(MMICC_APP_6459_112_2_18_2_10_19_180,"mmicc_app: CC_IsValideDest, disc_cause = %d")
TRACE_MSG(MMICC_APP_6487_112_2_18_2_10_20_181,"mmicc_app: CC_IsUserBusy, disc_cause = %d")
TRACE_MSG(MMICC_APP_6526_112_2_18_2_10_20_182,"ccapp.c:enter function MMIAPICC_GetSimCallReady(), dual_sys >= MMI_DUAL_SYS_MAX ")
TRACE_MSG(MMICC_APP_6530_112_2_18_2_10_20_183,"ccapp: MMIAPICC_GetSimCallReady, s_call_ready[dual_sys] = %d, dual_sys = %d")
TRACE_MSG(MMICC_APP_6544_112_2_18_2_10_20_184,"ccapp: MMIAPICC_SetSimCallReady, is_ready = %d, dual_sys = %d")
TRACE_MSG(MMICC_APP_6551_112_2_18_2_10_20_185,"ccapp: error dual_sys = %d")
TRACE_MSG(MMICC_APP_6579_112_2_18_2_10_20_186,"ccapp.c:enter function CC_GetIPCallNum(), tele_num is NULL ")
TRACE_MSG(MMICC_APP_6948_112_2_18_2_10_21_187,"mmiccapp: CC_SendDTMFStr, dtmf_str = %s, dtmf_len = %d")
TRACE_MSG(MMICC_APP_7146_112_2_18_2_10_21_188,"ccapp:CC_IsWaitCallBeing %d")
TRACE_MSG(MMICC_APP_7159_112_2_18_2_10_21_189,"ccapp.c:enter function CC_ReleaseWaitCall()")
TRACE_MSG(MMICC_APP_7169_112_2_18_2_10_21_190,"CC_ReleaseWaitCall , call_id = %d")
TRACE_MSG(MMICC_APP_7176_112_2_18_2_10_21_191,"CC_ReleaseWaitCall MNCALL_DiscCall return failure")
TRACE_MSG(MMICC_APP_7209_112_2_18_2_10_21_192,"ccapp.c:enter function CC_CRSSReleaseAndAnswer()")
TRACE_MSG(MMICC_APP_7264_112_2_18_2_10_21_193,"ccapp.c:enter function CC_CRSSHoldAndAnswer()")
TRACE_MSG(MMICC_APP_7290_112_2_18_2_10_21_194,"ccapp CC_CRSSHoldAndAnswer error index %d")
TRACE_MSG(MMICC_APP_7295_112_2_18_2_10_21_195,"CC_CRSSHoldAndAnswer: the result of MNCALL_HoldCall is error")
TRACE_MSG(MMICC_APP_7349_112_2_18_2_10_21_196,"ccapp:CC_ReleaseActiveCallByIndex()")
TRACE_MSG(MMICC_APP_7371_112_2_18_2_10_21_197,"ccapp:MMICC_ParseStringOfCRSS()")
TRACE_MSG(MMICC_APP_7556_112_2_18_2_10_22_198,"ccapp.c:enter function MMIAPICC_MakeCall(), tele_num is NULL ")
TRACE_MSG(MMICC_APP_7565_112_2_18_2_10_22_199,"ccapp.c: MMIAPICC_MakeCall Parsed in ParseString")
TRACE_MSG(MMICC_APP_7574_112_2_18_2_10_22_200,"ccapp.c: MakeCall req is process by CRSS")
TRACE_MSG(MMICC_APP_7593_112_2_18_2_10_22_201,"ccapp.c: MakeCall req is reject when mo or mt")
TRACE_MSG(MMICC_APP_7599_112_2_18_2_10_22_202,"ccapp.c: MakeCall req is reject when vt")
TRACE_MSG(MMICC_APP_7604_112_2_18_2_10_22_203,"MMIAPICC_MakeCall dual_sys=%d,call_sim_type=%d,call_type=%d")
TRACE_MSG(MMICC_APP_7626_112_2_18_2_10_22_204,"MMIAPICC_MakeCall call_dual_sys=%d")
TRACE_MSG(MMICC_APP_7659_112_2_18_2_10_22_205,"ccapp.c:enter function MMICC_ProcessMakeCall(), make_call_info_ptr is NULL ")
TRACE_MSG(MMICC_APP_7665_112_2_18_2_10_22_206,"ProcessMakeCall Telephone number is null!")
TRACE_MSG(MMICC_APP_7679_112_2_18_2_10_22_207,"ProcessMakeCall dual_sys=%d")
TRACE_MSG(MMICC_APP_7684_112_2_18_2_10_22_208,"***ProcessMakeCall MO call is barring: tele_num = %s***")
TRACE_MSG(MMICC_APP_7719_112_2_18_2_10_22_209,"ccapp.c:MMIAPICC_MakeCallReq() network not support")
TRACE_MSG(MMICC_APP_7736_112_2_18_2_10_22_210,"ccapp.c: CC_ProcessExtensionNum() 'P' == tele_num[0]")
TRACE_MSG(MMICC_APP_7796_112_2_18_2_10_22_211,"ProcessMakeCall parse_result=%d")
TRACE_MSG(MMICC_APP_7808_112_2_18_2_10_22_212,"ccapp.c:MMIAPICC_MakeCallReq() SIM isn't ok")
TRACE_MSG(MMICC_APP_7828_112_2_18_2_10_22_213,"ccapp: sim corrupt!")
TRACE_MSG(MMICC_APP_7921_112_2_18_2_10_23_214,"s_call_context.call_number = %d")
TRACE_MSG(MMICC_APP_7930_112_2_18_2_10_23_215,"ProcessMakeCall: the result of MNCALL_HoldCall is error")
TRACE_MSG(MMICC_APP_7962_112_2_18_2_10_23_216,"ProcessMakeCall: has handled in parse function")
TRACE_MSG(MMICC_APP_8075_112_2_18_2_10_23_217,"ProcessMakeCall: Alloc memory Fail")
TRACE_MSG(MMICC_APP_8191_112_2_18_2_10_23_218,"MMIAPICC_GetCallSimType dual_sys=%d,call_sim_type=%d")
TRACE_MSG(MMICC_APP_8205_112_2_18_2_10_23_219,"ccapp: StartLocalNetRing")
TRACE_MSG(MMICC_APP_8240_112_2_18_2_10_23_220,"ccapp:StopLocalNetRing")
TRACE_MSG(MMICC_APP_8270_112_2_18_2_10_23_221,"ccapp:CC_IsMptySetupAvailable %d")
TRACE_MSG(MMICC_APP_8287_112_2_18_2_10_23_222,"ccapp:CC_IsMptySplitAvailable %d")
TRACE_MSG(MMICC_APP_8316_112_2_18_2_10_23_223,"ccapp:MMIAPICC_ReleaseCallByRedkey()")
TRACE_MSG(MMICC_APP_8438_112_2_18_2_10_24_224,"ccapp: dial method error %d")
TRACE_MSG(MMICC_APP_8641_112_2_18_2_10_24_225,"ccapp IncomingCallName_To_Speech error")
TRACE_MSG(MMICC_APP_8760_112_2_18_2_10_24_226,"mmivt null param")
TRACE_MSG(MMICC_APP_8840_112_2_18_2_10_25_227,"ccapp HandleMoMuteAndLocalRingState %d,msg_id= %d")
TRACE_MSG(MMICC_APP_8857_112_2_18_2_10_25_228,"CC_HandleBtHfuHoldInd: the hf hold action is %d")
TRACE_MSG(MMICC_APP_8871_112_2_18_2_10_25_229,"CC_HandleBtHfuHoldInd: release incoming call")
TRACE_MSG(MMICC_APP_8878_112_2_18_2_10_25_230,"CC_HandleBtHfuHoldInd: CC_ReleaseAndAnswerCW, and answer incoming")
TRACE_MSG(MMICC_APP_8883_112_2_18_2_10_25_231,"CC_HandleBtHfuHoldInd: CC_ReleaseAllCall")
TRACE_MSG(MMICC_APP_8892_112_2_18_2_10_25_232,"CC_HandleBtHfuHoldInd: CC_HoldAndAnswerCW")
TRACE_MSG(MMICC_APP_8899_112_2_18_2_10_25_233,"CC_HandleBtHfuHoldInd: CC_HoldCall")
TRACE_MSG(MMICC_APP_8903_112_2_18_2_10_25_234,"CC_HandleBtHfuHoldInd: the hold action is not support!")
TRACE_MSG(MMICC_APP_8908_112_2_18_2_10_25_235,"CC_HandleBtHfuHoldInd: the hold action is not support!")
TRACE_MSG(MMICC_APP_8928_112_2_18_2_10_25_236,"***IsIncludeBarringNumbers()***")
TRACE_MSG(MMICC_APP_9002_112_2_18_2_10_25_237,"MMIASP MMIAPICC_GetCallNumber number str len mayber is not enough !")
TRACE_MSG(MMICC_APP_9084_112_2_18_2_10_25_238,"ccapp.c: CC_HandleCcWinMsg() call_state = %d")
TRACE_MSG(MMICC_APP_9114_112_2_18_2_10_25_239,"AnswerASPCall fail ")
TRACE_MSG(MMICC_APP_9133_112_2_18_2_10_25_240,"ccapp.c: MMIAPICC_ResponseCallCtl dual_sys=%d, is_need_modified=%d, result=%d")
TRACE_MSG(MMICC_APP_9163_112_2_18_2_10_25_241,"MMICC: MMIAPICC_ResponseCallCtl No Memory")
TRACE_MSG(MMICC_APP_9447_112_2_18_2_10_26_242,"<CC_SetDevModeAndVolume>CALIBRATION_POST_MODE == iRet, Return mmi audio action to match the layer1_audio modify!")
TRACE_MSG(MMICC_APP_9467_112_2_18_2_10_26_243,"ccapp GetHoldCallCount: exist_hold_call_num is %d")
TRACE_MSG(MMICC_APP_9477_112_2_18_2_10_26_244,"ccapp GetActiveHoldCallCount: exist_hold_call_num is %d")
TRACE_MSG(MMICC_APP_9547_112_2_18_2_10_27_245,"[MMIAPICC_IsCallingAndInfoBt]")
TRACE_MSG(MMICC_APP_10098_112_2_18_2_10_28_246,"ccapp.c: MMICC_ECTIsPermit, ret = %d ")
TRACE_MSG(MMICC_APP_10130_112_2_18_2_10_28_247,"ccapp.c:enter function MMICC_ECTTransfer()")
TRACE_MSG(MMICC_APP_10140_112_2_18_2_10_28_248,"ccapp MMICC_ECTTransfer: invalid active idx: %d")
TRACE_MSG(MMICC_APP_10150_112_2_18_2_10_28_249,"CC_HoldCall: MNCALL_ECTInvokeEx is fault")
TRACE_MSG(MMICC_APP_10188_112_2_18_2_10_28_250,"ccapp.c HandleECTInd")
TRACE_MSG(MMICC_APP_10245_112_2_18_2_10_28_251,"ccapp.c party_num_present %d call id %d num_len %d")
TRACE_MSG(MMICC_APP_10257_112_2_18_2_10_28_252,"ccapp.c HandleECTCnf")
TRACE_MSG(MMICC_APP_10285_112_2_18_2_10_28_253,"ccapp.c ReplaceCallNumberByCallId call_id %d dual_sys %d")
TRACE_MSG(MMICC_APP_10292_112_2_18_2_10_28_254,"ccapp.c ReplaceCallNumberByCallId find it ")
TRACE_MSG(MMICC_APP_10360_112_2_18_2_10_29_255,"ccapp IsCurrentCallMute: %d")
TRACE_MSG(MMICC_APP_10371_112_2_18_2_10_29_256,"ccapp SetCurrentCallMute: %d")
TRACE_MSG(MMICC_AUDIO_221_112_2_18_2_10_30_257,"ccapp_a SwitchAudioDevice err oper:%d, dev:%d")
TRACE_MSG(MMICC_AUDIO_225_112_2_18_2_10_30_258,"ccapp_a SwitchAudioDevice oper:%d, dev:%d, status:%d, %d, %d")
TRACE_MSG(MMICC_AUDIO_226_112_2_18_2_10_30_259,"ccapp_a SwitchAudioDevice oper:%d, dev:%d")
TRACE_MSG(MMICC_AUDIO_231_112_2_18_2_10_30_260,"ccapp_a SwitchAudioDevice: no call is being")
TRACE_MSG(MMICC_AUDIO_238_112_2_18_2_10_30_261,"ccapp_a SwitchAudioDevice bt is invalid, mv is running")
TRACE_MSG(MMICC_AUDIO_263_112_2_18_2_10_30_262,"ccapp_a AudioOperation: invalid oper %d")
TRACE_MSG(MMICC_AUDIO_269_112_2_18_2_10_30_263,"ccapp_a SwitchAudioDevice not need switch")
TRACE_MSG(MMICC_AUDIO_275_112_2_18_2_10_30_264,"ccapp_a target dev: %d")
TRACE_MSG(MMICC_AUDIO_280_112_2_18_2_10_30_265,"ccapp_a SwitchAudioDevice end, status:%d, %d, %d")
TRACE_MSG(MMICC_AUDIO_360_112_2_18_2_10_30_266,"ccapp_a ChangeStatusToHH result:%d")
TRACE_MSG(MMICC_AUDIO_433_112_2_18_2_10_30_267,"ccapp_a: ChangeStatusToEP result:%d")
TRACE_MSG(MMICC_AUDIO_465_112_2_18_2_10_30_268,"ccapp_a: ChangeStatusToHF result:%d")
TRACE_MSG(MMICC_AUDIO_497_112_2_18_2_10_30_269,"ccapp_a: ChangeStatusToBT result:%d")
TRACE_MSG(MMICC_AUDIO_509_112_2_18_2_10_30_270,"ccapp_a SwitchMode dev:%d")
TRACE_MSG(MMICC_AUDIO_576_112_2_18_2_10_31_271,"ccapp_a ConstructAudioHandle error")
TRACE_MSG(MMICC_AUDIO_579_112_2_18_2_10_31_272,"ccapp_a ConstructAudioHandle")
TRACE_MSG(MMICC_AUDIO_608_112_2_18_2_10_31_273,"ccapp_a DestroyAudioHandle handle not being")
TRACE_MSG(MMICC_AUDIO_611_112_2_18_2_10_31_274,"ccapp_a DestroyAudioHandle")
TRACE_MSG(MMICC_AUDIO_633_112_2_18_2_10_31_275,"ccapp_a ConstructVoiceCodec codec has been enable")
TRACE_MSG(MMICC_AUDIO_636_112_2_18_2_10_31_276,"ccapp_a ConstructVoiceCodec")
TRACE_MSG(MMICC_AUDIO_692_112_2_18_2_10_31_277,"ccapp_a DestroyVoiceCodec handle not being")
TRACE_MSG(MMICC_AUDIO_695_112_2_18_2_10_31_278,"ccapp_a DestroyVoiceCodec")
TRACE_MSG(MMICC_AUDIO_711_112_2_18_2_10_31_279,"ccapp_a VoiceCodec is same state")
TRACE_MSG(MMICC_AUDIO_714_112_2_18_2_10_31_280,"ccapp_a VoiceCodec enable: %d")
TRACE_MSG(MMICC_AUDIO_747_112_2_18_2_10_31_281,"ccapp_a VoiceCodec status: %d")
TRACE_MSG(MMICC_AUDIO_768_112_2_18_2_10_31_282,"ccapp_a: MMICC_EnableMute is_mute:%d")
TRACE_MSG(MMICC_AUDIO_771_112_2_18_2_10_31_283,"ccapp_a mute is same state")
TRACE_MSG(MMICC_AUDIO_799_112_2_18_2_10_31_284,"ccapp_a: MMICC_EnableRemoteMute is_enable:%d")
TRACE_MSG(MMICC_AUDIO_802_112_2_18_2_10_31_285,"ccapp_a remote mute is same state")
TRACE_MSG(MMICC_AUDIO_851_112_2_18_2_10_31_286,"ccapp: StartRingOrVibrate type: %d")
TRACE_MSG(MMICC_AUDIO_988_112_2_18_2_10_31_287,"HandleVibrateTimer: call_ring_type is %d")
TRACE_MSG(MMICC_AUDIO_1219_112_2_18_2_10_32_288,"ccapp PlayCustomRing valid: %d, type: %d")
TRACE_MSG(MMICC_AUDIO_1289_112_2_18_2_10_32_289,"ccapp PlayCustomRing default")
TRACE_MSG(MMICC_MAIN_123_112_2_18_2_10_33_290,"ccapp Applet_HandleEvent msg_id = 0x%04x")
TRACE_MSG(MMICC_MAIN_227_112_2_18_2_10_33_291,"ccapp CC_Initialize")
TRACE_MSG(MMICC_MAIN_263_112_2_18_2_10_33_292,"ccapp CC_Finalize")
TRACE_MSG(MMICC_MAIN_289_112_2_18_2_10_33_293,"ccapp StartCCApplet")
TRACE_MSG(MMICC_MAIN_305_112_2_18_2_10_33_294,"ccapp Stop cc applet")
TRACE_MSG(MMICC_MAIN_324_112_2_18_2_10_33_295,"MMIAPICC_SendMsgToApplet msg:%d")
TRACE_MSG(MMICC_MAIN_358_112_2_18_2_10_33_296,"ccapp ActiveCC result:%d")
TRACE_MSG(MMICC_MAIN_384_112_2_18_2_10_33_297,"ccapp HideCC result:%d")
TRACE_MSG(MMICC_SPEEDDIAL_567_112_2_18_2_10_36_298,"mmi_speeddial.c: CC_HandleSpeedDialListWinMsg( %d, %d)")
TRACE_MSG(MMICC_SPEEDDIAL_920_112_2_18_2_10_37_299,"mmi_speeddial.c: CC_HandleSpeedDialMenuWinMsg( %d, %d)")
TRACE_MSG(MMICC_SPEEDDIAL_1451_112_2_18_2_10_38_300,"GetSelectedItem PNULL == number_ptr || PNULL == number_len_ptr")
TRACE_MSG(MMICC_SPEEDDIAL_1481_112_2_18_2_10_38_301,"SaveSpeedDialItem PNULL == number_ptr")
TRACE_MSG(MMICC_SPEEDDIAL_1648_112_2_18_2_10_39_302,"GenSpeedStr speedkey_entry_ptrm, name_str_ptr,speedkey_entry_ptr->number_len maybe null")
TRACE_MSG(MMICC_SPEEDDIAL_1660_112_2_18_2_10_39_303,"ccapp GenSpeedStr mem alloc failed")
TRACE_MSG(MMICC_SPEEDDIAL_1769_112_2_18_2_10_39_304,"ccapp SpeedDial error key_num = %d")
TRACE_MSG(MMICC_SPEEDDIAL_1778_112_2_18_2_10_39_305,"[SPEEDDIAL] MMIAPICC_SpeedDial ret_val %d")
TRACE_MSG(MMICC_SPEEDDIAL_1831_112_2_18_2_10_39_306,"ccapp SpeedDial error key_num = %d")
TRACE_MSG(MMICC_SPEEDDIAL_1840_112_2_18_2_10_39_307,"[SPEEDDIAL] MMIAPICC_SpeedDial ret_val %d")
TRACE_MSG(MMICC_SPEEDDIAL_1883_112_2_18_2_10_39_308,"AppendItemToListBox string_ptr")
TRACE_MSG(MMICC_WINTAB_2276_112_2_18_2_10_44_309,"ccapp.c:enter function CC_HandleCcLockMsg() msg_id = %x")
TRACE_MSG(MMICC_WINTAB_2594_112_2_18_2_10_45_310,"ccapp.c:enter function CC_HandleCcLockMsg() WRONG param! enter default branch!!!")
TRACE_MSG(MMICC_WINTAB_2651_112_2_18_2_10_45_311,"mmiccapp:OpenCCHoldMenu, cur dev = %d")
TRACE_MSG(MMICC_WINTAB_3240_112_2_18_2_10_46_312,"ccapp HandleCallSIMSelectWinMsg,item_data[%d] >= MMI_DUAL_SYS_MAX,ERROR!")
TRACE_MSG(MMICC_WINTAB_3247_112_2_18_2_10_46_313,"ccapp HandleCallSIMSelectWinMsg make info ptr is null")
TRACE_MSG(MMICC_WINTAB_3318_112_2_18_2_10_47_314,"mmicc: CC_CallStateDisplaying, s_call_context.dual_sys = %d, sim_name.str_arr = %s")
TRACE_MSG(MMICC_WINTAB_3326_112_2_18_2_10_47_315,"ccapp str is too long")
TRACE_MSG(MMICC_WINTAB_3391_112_2_18_2_10_47_316,"CC_SetCallWinSoftkey:win_id %d is not open!")
TRACE_MSG(MMICC_WINTAB_3718_112_2_18_2_10_47_317,"ccapp.c:MMICC_HandleInsertDTMFWinMsg() send dtmf failure")
TRACE_MSG(MMICC_WINTAB_5067_112_2_18_2_10_50_318,"ccapp err DisplayCallInfoForSingle, null ptr")
TRACE_MSG(MMICC_WINTAB_5428_112_2_18_2_10_51_319,"ccapp err DisplayCallInfoForSingle, null ptr")
TRACE_MSG(MMICC_WINTAB_5472_112_2_18_2_10_51_320,"ccapp err DisplayCallInfoForSingle, null ptr")
TRACE_MSG(MMICC_WINTAB_5735_112_2_18_2_10_52_321,"ccapp.c:enter function PdaDisplayDisconnectWin():call_index=%d, cause=%d")
TRACE_MSG(MMICC_WINTAB_5739_112_2_18_2_10_52_322,"ccapp disc win not open as active call")
TRACE_MSG(MMICC_WINTAB_5931_112_2_18_2_10_52_323,"ccapp err DisplayCallInfoForSingle, null ptr")
TRACE_MSG(MMICC_WINTAB_6510_112_2_18_2_10_53_324,"GetCallStateText state_ptr is NULL")
TRACE_MSG(MMICC_WINTAB_6606_112_2_18_2_10_54_325,"ccapp.c:enter function CC_HandleCcAnimWinMsg() msg_id = %x")
TRACE_MSG(MMICC_WINTAB_6678_112_2_18_2_10_54_326,"ccapp.c CC_HandleCcAnimWinMsg() headset is plugout!")
TRACE_MSG(MMICC_WINTAB_6846_112_2_18_2_10_54_327,"MMIASP WAIT TIME OUT, asp answer fail")
TRACE_MSG(MMICC_WINTAB_6992_112_2_18_2_10_54_328,"MMINV_SET_IS_FLIP_ANSWER = %d")
TRACE_MSG(MMICC_WINTAB_7102_112_2_18_2_10_55_329,"ccapp.c: CC_HandleCcWinMsg() call_state = %d")
TRACE_MSG(MMICC_WINTAB_7267_112_2_18_2_10_55_330,"ccapp.c: CC_HandleCcWinMsg() msg_id = %d")
TRACE_MSG(MMICC_WINTAB_7751_112_2_18_2_10_56_331,"ccapp CreatePhotoAnim null ptr")
TRACE_MSG(MMICC_WINTAB_8015_112_2_18_2_10_57_332,"ccapp.c:enter function CC_OpenMtCallWin()")
TRACE_MSG(MMICC_WINTAB_8111_112_2_18_2_10_57_333,"ccapp.c:enter function CC_OpenCallProcessingWin()")
TRACE_MSG(MMICC_WINTAB_8146_112_2_18_2_10_57_334,"ccapp.c:enter function CC_DisplayDisconnectWin():call_index=%d, cause=%d")
TRACE_MSG(MMICC_WINTAB_8179_112_2_18_2_10_57_335,"CC_DisplayDisconnectWin call_state = %d, call_time = %d")
TRACE_MSG(MMICC_WINTAB_8301_112_2_18_2_10_58_336,"ccapp.c:enter function CC_OpenMissedCallWin()")
TRACE_MSG(MMICC_WINTAB_8310_112_2_18_2_10_58_337,"ccapp error char_num_len = %d, missed_call_str = %d")
TRACE_MSG(MMICC_WINTAB_8354_112_2_18_2_10_58_338,"ccapp.c:enter function CC_OpenMissedCallWin() mem alloc FAILED")
TRACE_MSG(MMICC_WINTAB_8709_112_2_18_2_10_59_339,"ccapp.c:enter function MMICC_HandleAutoDialWinMsg() msg_id = %d")
TRACE_MSG(MMICC_WINTAB_8916_112_2_18_2_10_59_340,"ccapp.c: MMICC_HandleAutoDialWinMsg() msg_id = %d")
TRACE_MSG(MMICC_WINTAB_8935_112_2_18_2_11_0_341,"GetRedialedTimes redialed_times_ptr is NULL")
TRACE_MSG(MMICC_WINTAB_8979_112_2_18_2_11_0_342,"ccapp.c:enter function CC_OpenAutoDialWindow()")
TRACE_MSG(MMICC_WINTAB_9015_112_2_18_2_11_0_343,"ccapp.c:enter function CC_OpenAutoDialWindow()")
TRACE_MSG(MMICC_WINTAB_9191_112_2_18_2_11_0_344,"---mmiccapp:MMICC_HandleCCMenuWinMsg")
TRACE_MSG(MMICC_WINTAB_9362_112_2_18_2_11_1_345,"mmiccapp:MMICC_HandleCCMenuWinMsg, ID_CC_RELEASE_ALL")
TRACE_MSG(MMICC_WINTAB_9373_112_2_18_2_11_1_346,"mmiccapp:MMICC_HandleCCMenuWinMsg, ID_CC_BUILDMPTY")
TRACE_MSG(MMICC_WINTAB_9384_112_2_18_2_11_1_347,"mmiccapp:MMICC_HandleCCMenuWinMsg, ID_CC_HANDHOLD")
TRACE_MSG(MMICC_WINTAB_9418_112_2_18_2_11_1_348,"mmiccapp:MMICC_HandleCCMenuWinMsg, ID_HANDFREE_SWITCH")
TRACE_MSG(MMICC_WINTAB_9465_112_2_18_2_11_1_349,"MMICC_HandleCCMenuWinMsg: the menu id is error")
TRACE_MSG(MMICC_WINTAB_9492_112_2_18_2_11_1_350,"MMICC_HandleCCMenuWinMsg() msg_id = %d")
TRACE_MSG(MMICC_WINTAB_9504_112_2_18_2_11_1_351,"ccapp.c:enter function OpenCCMenu()")
TRACE_MSG(MMICC_WINTAB_9823_112_2_18_2_11_2_352,"ccapp err index: %d, state : %d")
TRACE_MSG(MMICC_WINTAB_9958_112_2_18_2_11_2_353,"ccapp.c:enter function CC_OpenCallList()")
TRACE_MSG(MMICC_WINTAB_10007_112_2_18_2_11_3_354,"ccapp.c:enter function CC_CallList()")
TRACE_MSG(MMICC_WINTAB_10079_112_2_18_2_11_3_355,"ccapp.c:enter function OpenAdjustVolumeWin()")
TRACE_MSG(MMICC_WINTAB_10293_112_2_18_2_11_3_356,"MMIAPICC_GetBtSpeakerVol=%d")
TRACE_MSG(MMICC_WINTAB_10536_112_2_18_2_11_4_357,"mmiccapp: CC_HOLDMENU_NODE_HANDHOLD")
TRACE_MSG(MMICC_WINTAB_10558_112_2_18_2_11_4_358,"mmiccapp: CC_HOLDMENU_NODE_HANDFREE")
TRACE_MSG(MMICC_WINTAB_10672_112_2_18_2_11_4_359,"ccapp.c:enter function CC_HandleCcProcessingWinMsg() msg_id = %d")
TRACE_MSG(MMICC_WINTAB_11057_112_2_18_2_11_5_360,"ccapp.c:enter function CC_CreateNewHotButtonWithoutForm() PNULL == s_current_win_button_order_ptr!")
TRACE_MSG(MMICC_WINTAB_11193_112_2_18_2_11_5_361,"ccapp.c:enter function MMICC_CloseDtmfPanel() else WARRNING!")
TRACE_MSG(MMICC_WINTAB_11213_112_2_18_2_11_5_362,"ccapp.c:enter function MMICC_CloseDtmfPanel() else WARRNING!")
TRACE_MSG(MMICC_WINTAB_11322_112_2_18_2_11_5_363,"ccapp.c:enter function CC_UpdateCcDisplayType() default case WARRNING!")
TRACE_MSG(MMICC_WINTAB_11703_112_2_18_2_11_6_364,"ccapp.c:enter function CC_InitHotButtonState() default WARRNING!")
TRACE_MSG(MMICC_WINTAB_11733_112_2_18_2_11_6_365,"ccapp HandleCCWinWebMsg button id: %d")
TRACE_MSG(MMICC_WINTAB_11815_112_2_18_2_11_6_366,"ccapp HandleCCWinWebMsg !MMIAPIASP_WorkOneTime()")
TRACE_MSG(MMICC_WINTAB_11819_112_2_18_2_11_6_367,"ccapp HandleCCWinWebMsg !AnswerASPCall()")
TRACE_MSG(MMICC_WINTAB_11850_112_2_18_2_11_6_368,"ccapp.c:enter function CC_UpdateCCWinHotButton(), PNULL == s_current_win_button_order_ptr ERROR!")
TRACE_MSG(MMICC_WINTAB_11874_112_2_18_2_11_6_369,"ccapp CC_FindNextHotButton didn't find the current focus button!")
TRACE_MSG(MMICC_WINTAB_11931_112_2_18_2_11_6_370,"ccapp CC_FindNextHotButton didn't find the next focus button!")
TRACE_MSG(MMICC_WINTAB_11985_112_2_18_2_11_6_371,"ccapp CC_FindNextHotButton didn't find the next focus button!")
TRACE_MSG(MMICC_WINTAB_12033_112_2_18_2_11_7_372,"ccapp CC_FindNextHotButton didn't find the next focus button!")
TRACE_MSG(MMICC_WINTAB_12081_112_2_18_2_11_7_373,"ccapp CC_FindNextHotButton didn't find the next focus button!")
TRACE_MSG(MMICC_WINTAB_12215_112_2_18_2_11_7_374,"ccapp.c:enter function CC_UpdateCCWinHotButton(), PNULL == s_current_win_button_order_ptr ERROR!")
TRACE_MSG(MMICC_WINTAB_12422_112_2_18_2_11_7_375,"ccapp HandleCCWinWebMsg !MMIAPIASP_WorkOneTime()")
TRACE_MSG(MMICC_WINTAB_12426_112_2_18_2_11_7_376,"ccapp HandleCCWinWebMsg !AnswerASPCall()")
TRACE_MSG(MMICC_WINTAB_12533_112_2_18_2_11_8_377,"mmicc_app.c HandleCCWinPenOkMsg edit_ctrl_id is not active ctrl, can't handle the message!!!")
TRACE_MSG(MMICC_WINTAB_12594_112_2_18_2_11_8_378,"ccapp.c:enter function CC_HandleCcWinMsg() msg_id = %x")
TRACE_MSG(MMICC_WINTAB_12652_112_2_18_2_11_8_379,"ccapp.c CC_HandleCcWinMsg() headset is plugout!")
TRACE_MSG(MMICC_WINTAB_12817_112_2_18_2_11_8_380,"ccapp.c:CC_SetupCompleteCnf() send dtmf failure")
TRACE_MSG(MMICC_WINTAB_12941_112_2_18_2_11_8_381,"ccapp.c: CC_HandleCcWinMsg() call_state = %d")
TRACE_MSG(MMICC_WINTAB_13025_112_2_18_2_11_8_382,"ccapp.c: CC_HandleCcWinMsg() call_state = %d")
TRACE_MSG(MMICC_WINTAB_13135_112_2_18_2_11_9_383,"ccapp.c: CC_HandleCcWinMsg() msg_id = %d")
TRACE_MSG(MMICC_WINTAB_13157_112_2_18_2_11_9_384,"ccapp.c:enter function CC_HandleDisconnectWinMsg(),msg_id=%d")
TRACE_MSG(MMICC_WINTAB_13216_112_2_18_2_11_9_385,"ccapp.c:enter function CC_HandleDisconnectWinMsg(), MSG_TIMER")
TRACE_MSG(MMICC_WINTAB_13223_112_2_18_2_11_9_386,"ccapp.c: CC_HandleDisconnectWinMsg, tele_num = %s, tele_len = %d")
TRACE_MSG(MMICC_WINTAB_13228_112_2_18_2_11_9_387,"ccapp.c: s_call_repeat_info.disc_cause = %d, s_autodial_num = %d")
TRACE_MSG(MMICC_WINTAB_13396_112_2_18_2_11_9_388,"ccapp.c: CC_HandleDisconnectWinMsg() msg_id = %d")
TRACE_MSG(MMICC_WINTAB_13411_112_2_18_2_11_9_389,"ccapp:PlayCallEndHint")
TRACE_MSG(MMICC_WINTAB_13506_112_2_18_2_11_9_390,"ccapp.c: MMICC_HandleMissedcallWinMsg() msg_id = %d")
TRACE_MSG(MMICC_WINTAB_13676_112_2_18_2_11_10_391,"CC_SendSMSAfterRejectMTCall name_ptr is NULL")
TRACE_MSG(MMICC_WINTAB_13824_112_2_18_2_11_10_392,"mmiccapp: HandleEmergencyCallWinMsg, msg_id = %d")
TRACE_MSG(MMICC_WINTAB_13906_112_2_18_2_11_10_393,"ccapp.c: MMICC_SetRepeatInfoCause, s_call_repeat_info.disc_cause = %d")
TRACE_MSG(MMICC_WINTAB_14206_112_2_18_2_11_11_394,"ccapp:AutoAnswerIncomingCall")
TRACE_MSG(MMICC_WINTAB_14223_112_2_18_2_11_11_395,"cc app MMICC_UpdateCallStatusDisplay error param: %d")
TRACE_MSG(MMICC_WINTAB_14532_112_2_18_2_11_11_396,"ccapp OpenMagicVoice mv cannot open")
TRACE_MSG(MMICC_WINTAB_14538_112_2_18_2_11_11_397,"ccapp OpenMagicVoice mv is working")
TRACE_MSG(MMICC_WINTAB_14591_112_2_18_2_11_12_398,"ccapp SetMagicVoiceParam mv is not working")
TRACE_MSG(MMICC_WINTAB_14623_112_2_18_2_11_12_399,"CopyMagicVoiceData dst_data_ptr OR src_data_ptr is NULL")
TRACE_MSG(MMICC_WINTAB_14641_112_2_18_2_11_12_400,"ccapp CloseMagicVoice mv is not working")
TRACE_MSG(MMICC_WINTAB_14828_112_2_18_2_11_12_401,"CopyMagicVoiceData item_t_ptr OR item_data_ptr is NULL")
END_TRACE_MAP(MMI_APP_CC_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_CC_TRC_H_

