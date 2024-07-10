/******************************************************************************
 ** File Name:      dm_2g_ps_trc.h                                         *
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
//trace_id:10
#ifndef _DM_2G_PS_TRC_H_
#define _DM_2G_PS_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define CCMSG_1855_112_2_18_0_3_13_0 "CC: The message is valid CC message. \n"
#define CCMSG_1859_112_2_18_0_3_13_1 "CC: The message is invalid CC message. \n"
#define CCMSG_2213_112_2_18_0_3_13_2 "CC: the IE is not valid IE of Setup message. \n"
#define CCMSG_2220_112_2_18_0_3_13_3 "CC: Decode Setup message and find serious ie error.\n"
#define CCMSG_2549_112_2_18_0_3_14_4 "CC: the IE is not valid IE of Setup message. \n"
#define CCMSG_2595_112_2_18_0_3_14_5 "CC: Progress message is too short to contain mandatory IE.\n"
#define CCMSG_2615_112_2_18_0_3_15_6 "CC: Progress message mandatory IE error.\n"
#define CCMSG_2628_112_2_18_0_3_15_7 "CC: Progress Message Ie sequence error.\n"
#define CCMSG_2681_112_2_18_0_3_15_8 "CC: Facility message is too short to contain mandatory IE.\n"
#define CCMSG_2727_112_2_18_0_3_15_9 "CC: Notify message is too short to contain mandatory IE.\n"
#define CCMSG_2892_112_2_18_0_3_15_10 "CC: Error IE in Call Proceeding Message. \n"
#define CCMSG_2992_112_2_18_0_3_15_11 "CC: Error IE in Alerting Message. \n"
#define CCMSG_3113_112_2_18_0_3_15_12 "CC: Error IE in Connect Message. \n"
#define CCMSG_3241_112_2_18_0_3_16_13 "CC: Error IE in Disconnect Message. \n"
#define CCMSG_3364_112_2_18_0_3_16_14 "CC: Error IE in Modify Message. \n"
#define CCMSG_3481_112_2_18_0_3_16_15 "CC: Error IE in Modify Complete Message. \n"
#define CCMSG_3601_112_2_18_0_3_16_16 "CC: Error IE in Modify Reject Message. \n"
#define CCMSG_3650_112_2_18_0_3_17_17 "CC: Message is too short.\n"
#define CCMSG_3691_112_2_18_0_3_17_18 "CC: Message is too short.\n"
#define CCMSG_4013_112_2_18_0_3_17_19 "CC: Error IE in Release Message. \n"
#define CCMSG_4115_112_2_18_0_3_17_20 "CC: Error IE in Release Complete Message. \n"
#define CCMSG_4301_112_2_18_0_3_18_21 "CC: Cc-Establishment message is too short to contain mandatory IE.\n"
#define CCMSG_4352_112_2_18_0_3_18_22 "CC: Recall message is too short to contain mandatory IE.\n"
#define CCMSG_5230_112_2_18_0_3_20_23 "CC: bc ie is type error"
#define CCMSG_5277_112_2_18_0_3_20_24 "CC: facility ie is type error"
#define CCMSG_5337_112_2_18_0_3_20_25 "CC: progress ie is type error"
#define CCMSG_5878_112_2_18_0_3_21_26 "CC: progress ie's type error"
#define CCMSG_5947_112_2_18_0_3_21_27 "CC: progress ie's type error"
#define CCMSG_6047_112_2_18_0_3_22_28 "CC: backup bc ie is type error"
#define CCMSG_6103_112_2_18_0_3_22_29 "CC: allowed action ie is type error"
#define L3FUNC_58_112_2_18_0_3_26_30 "L3FUNC: Assert Failure, PLMN list length %d is more than MAX %d."
#define L3FUNC_95_112_2_18_0_3_27_31 "L3FUNC: Assert Failure, FPLMN list length %d is more than MAX %d."
#define L3FUNC_189_112_2_18_0_3_27_32 "L3FUNC: Assert Failure, LongFPLMN list length %d is more than MAX %d."
#define L3FUNC_406_112_2_18_0_3_27_33 "L3FUNC: Assert Failure, FLai list length %d is more than MAX %d."
#define L3FUNC_445_112_2_18_0_3_27_34 "L3FUNC: Assert Failure, FPLMN list length %d is more than MAX %d."
#define L3FUNC_479_112_2_18_0_3_27_35 "L3FUNC: Assert Failure, LongFPLMN list length %d is more than MAX %d."
#define L3FUNC_512_112_2_18_0_3_27_36 "L3FUNC: Assert Failure, PLMN list length %d is more than MAX %d."
#define L3FUNC_542_112_2_18_0_3_28_37 "L3FUNC: Assert Failure, FLai list length %d is more than MAX %d."
#define L3FUNC_580_112_2_18_0_3_28_38 "L3FUNC: Assert Failure, EPLMN list length %d is more than MAX %d."
#define L3FUNC_625_112_2_18_0_3_28_39 "L3FUNC: Assert Failure, EPLMN list length %d is more than MAX %d."
#define L3FUNC_717_112_2_18_0_3_28_40 "L3FUNC: Assert Failure, Select PLMN list length %d is more than MAX %d."
#define L3FUNC_747_112_2_18_0_3_28_41 "L3FUNC: Assert Failure, Available PLMN list length %d is more than MAX %d."
#define MMGMM_COMMON_IE_133_112_2_18_0_3_29_42 "MMGMM: Assert Failure, ie_type %d error, can't decode EC number list."
#define MMGMM_COMMON_IE_210_112_2_18_0_3_29_43 "MMGMM: DecodeEmgNum data_len=%d, data_ptr[0]=%d"
#define MMGMM_COMMON_IE_266_112_2_18_0_3_29_44 "MMGMM:The length of the network name is too long"
#define MMGMM_COMMON_IE_293_112_2_18_0_3_29_45 "MMGMM:The length of the network name is too long"
#define MMGMM_COMMON_IE_328_112_2_18_0_3_29_46 "MMGMM: Assert Failure, date_len %d error, can't decode TimeZone."
#define MMGMM_COMMON_IE_353_112_2_18_0_3_29_47 "MMGMM: Assert Failure, date_len %d error, can't decode TimeZone and Time."
#define MMGMM_COMMON_IE_458_112_2_18_0_3_29_48 "MMGMM: decode MM/GMM information msg, MANDATORY_IE_CONTENT_ERROR"
#define MMGMM_COMMON_IE_468_112_2_18_0_3_29_49 "MMGMM: decode MM/GMM information msg, DECODE_SUCCESS, No Optional IE"
#define MMGMM_COMMON_IE_578_112_2_18_0_3_29_50 "MMGMM: Assert Failure, ie_type %d error, can't decode MobileIdentify."
#define PS_MAIN_499_112_2_18_0_3_32_51 "SCI_GetRRTaskIdBySysId:ASSERT!multi_sys >= MULTI_SYS_NUM"
#define PS_MAIN_636_112_2_18_0_3_32_52 "SCI_GetSysIdByNSWTHTask:nswth sys id=%d"
#define SCI_AIR_INTERFACE_232_112_2_18_0_3_35_53 "SCIAIR:SCIAI_BSAIRMsgToPSMsg Receive unknown SignalCode"
#define SCI_AIR_INTERFACE_307_112_2_18_0_3_35_54 "SCIAIR:SCIAI_BSPSMsgToAIRMsg Receive unknown SignalCode"
#define STATE_MACHINE_33_112_2_18_0_4_8_55 "SDL:save signal %x\n"
#define STATE_MACHINE_46_112_2_18_0_4_8_56 "SDL:discard signal %x\n"
#define DL_BUFFER_771_112_2_18_0_4_14_57 "DL: NR error, ns=%d ,new=%d ,old=%d n"
#define DL_BUFFER_1437_112_2_18_0_4_15_58 "L2:GetIFrame State is unexpected"
#define DL_BUFFER_1513_112_2_18_0_4_16_59 "L2:PackageSFrame stype %d, is unexpected"
#define DL_BUFFER_1659_112_2_18_0_4_16_60 "DL: send repeated SACCH"
#define DL_BUFFER_1982_112_2_18_0_4_16_61 "L2:PackageUFrame unexpected utype %d"
#define DL_BUFFER_2200_112_2_18_0_4_17_62 "DL:contension data fail,length wrong\n"
#define DL_BUFFER_2207_112_2_18_0_4_17_63 "DL:contension data fail,data wrong\n"
#define DL_BUFFER_2287_112_2_18_0_4_17_64 "DL: DL_GetUnsentSAPI3MsgHeader, MSG_PTR IS NOT NULL"
#define DL_BUFFER_2293_112_2_18_0_4_17_65 "DL: MSG header is %d,%d"
#define DL_BUFFER_2295_112_2_18_0_4_17_66 "DL:There is a message to be retrans!"
#define DL_BUFFER_2327_112_2_18_0_4_17_67 "DL: receive SABM in established state, but not init dlc according to special condition"
#define DL_BUFFER_2429_112_2_18_0_4_17_68 "DL: req_sro is true, DL_sro=%d, DL_repeat_uplink_sacch_pos=%d"
#define DL_BUFFER_2544_112_2_18_0_4_18_69 "DL: is repeated FACCH"
#define DL_BUFFER_2563_112_2_18_0_4_18_70 "DL: is repeated FACCH"
#define GMM_CLASSMARK_476_112_2_18_0_4_19_71 "GMM:ASSERT Failure,GMMCLASSMARK_SetMsBand, ms_band=%d"
#define GMM_CLASSMARK_491_112_2_18_0_4_19_72 "GMM: classmark InitMsBand, ms_band=%d"
#define GMM_ENCODE_DECODE_189_112_2_18_0_4_19_73 "GMM:Assert Failure,Decode time failure"
#define GMM_ENCODE_DECODE_284_112_2_18_0_4_20_74 "GMM:mobile_identity->type_identity =%d"
#define GMM_ENCODE_DECODE_584_112_2_18_0_4_20_75 "GMM:detach type %d is interpreted as re-attach not required"
#define GMM_ENCODE_DECODE_615_112_2_18_0_4_20_76 "GMM:message too short,len = %d!"
#define GMM_ENCODE_DECODE_843_112_2_18_0_4_21_77 "GMM:Attach Accept msg, unknown field!"
#define GMM_ENCODE_DECODE_849_112_2_18_0_4_21_78 "GMM:Memory Overflow!Check!"
#define GMM_ENCODE_DECODE_875_112_2_18_0_4_21_79 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_916_112_2_18_0_4_21_80 "GMM:Attach Reject, unknownd field!"
#define GMM_ENCODE_DECODE_922_112_2_18_0_4_21_81 "GMM:Memory Overflow!Check!"
#define GMM_ENCODE_DECODE_946_112_2_18_0_4_21_82 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_991_112_2_18_0_4_21_83 "GMM:Detach Request,unknown field!"
#define GMM_ENCODE_DECODE_997_112_2_18_0_4_21_84 "GMM:Memory Overflow!Check!"
#define GMM_ENCODE_DECODE_1022_112_2_18_0_4_21_85 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_1048_112_2_18_0_4_21_86 "GMM:Detach Accept msg, unknown field!"
#define GMM_ENCODE_DECODE_1054_112_2_18_0_4_21_87 "GMM:Memory Overflow!Check!"
#define GMM_ENCODE_DECODE_1080_112_2_18_0_4_21_88 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_1139_112_2_18_0_4_21_89 "GMM:PTMSI allocation msg,unknown field!"
#define GMM_ENCODE_DECODE_1145_112_2_18_0_4_21_90 "GMM:Memory Overflow!Check!"
#define GMM_ENCODE_DECODE_1170_112_2_18_0_4_21_91 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_1233_112_2_18_0_4_22_92 "GMM:Authentication Msg error,rand-cksn should as both!"
#define GMM_ENCODE_DECODE_1244_112_2_18_0_4_22_93 "GMM:Authentication&Ciper msg, unknown field!"
#define GMM_ENCODE_DECODE_1250_112_2_18_0_4_22_94 "GMM:Memory Overflow!Check!"
#define GMM_ENCODE_DECODE_1273_112_2_18_0_4_22_95 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_1301_112_2_18_0_4_22_96 "GMM:Identify Request,unknown field!"
#define GMM_ENCODE_DECODE_1307_112_2_18_0_4_22_97 "GMM:Memory Overflow!Check!"
#define GMM_ENCODE_DECODE_1336_112_2_18_0_4_22_98 "GMM:Nsapi number error,npdu_list->number=%d"
#define GMM_ENCODE_DECODE_1377_112_2_18_0_4_22_99 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_1701_112_2_18_0_4_23_100 "GMM:Routing Area Accept msg,unknown field!"
#define GMM_ENCODE_DECODE_1707_112_2_18_0_4_23_101 "GMM:Memory Overflow!Check!"
#define GMM_ENCODE_DECODE_1762_112_2_18_0_4_23_102 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_1806_112_2_18_0_4_23_103 "GMM:RAU Reject msg, unknown field!"
#define GMM_ENCODE_DECODE_1812_112_2_18_0_4_23_104 "GMM:Memory Overflow!Check!"
#define GMM_ENCODE_DECODE_1835_112_2_18_0_4_23_105 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_1859_112_2_18_0_4_23_106 "GMM:GMM Status msg, unknown field!"
#define GMM_ENCODE_DECODE_1865_112_2_18_0_4_23_107 "GMM:Memory Overflow!Check!"
#define GMM_ENCODE_DECODE_1969_112_2_18_0_4_23_108 "GMM: rl_ind = %d"
#define GMM_ENCODE_DECODE_2169_112_2_18_0_4_24_109 "GMM:Assert Failure,invalid type_identity,"
#define GMM_ENCODE_DECODE_2856_112_2_18_0_4_25_110 "GMM: TLLI_FOREIGN ptmsi.valid_flag is %d"
#define GMM_ENCODE_DECODE_2862_112_2_18_0_4_25_111 "GMM: TLLI_LOCAL ptmsi.valid_flag is %d"
#define GMM_ENCODE_DECODE_2882_112_2_18_0_4_25_112 "GMM:TLLI error"
#define GMM_ENCODE_DECODE_2893_112_2_18_0_4_25_113 "GMM: Type = %d, new TLLI allocated = %x"
#define GMM_SDL_185_112_2_18_0_4_26_114 "GMM:Assert Failure,GMM_SendUnitdataReq msg_type = %d"
#define GMM_SDL_520_112_2_18_0_4_27_115 "GMM:T3312 is deactivated"
#define GMM_SDL_784_112_2_18_0_4_27_116 "GMM:info decode error"
#define LLC_ANALYSE_FRAME_244_112_2_18_0_4_36_117 "LLC:add oc_unack_downlink,512 for missing N(u)"
#define LLC_ANALYSE_FRAME_249_112_2_18_0_4_36_118 "LLC:the received UI frame is enciphered but no cipher algorithm!"
#define LLC_ANALYSE_FRAME_275_112_2_18_0_4_36_119 "LLC:before_crc 512+oc_unack_downlink=%d"
#define LLC_ANALYSE_FRAME_385_112_2_18_0_4_36_120 "LLC: I frame need to be deciphered!"
#define LLC_ANALYSE_FRAME_405_112_2_18_0_4_36_121 "LLC: I frame length invalid,discard"
#define LLC_ANALYSE_FRAME_426_112_2_18_0_4_36_122 "LLC: I frame FCS invalid,discard"
#define LLC_ANALYSE_FRAME_510_112_2_18_0_4_36_123 "LLE: Received a duplicate frame"
#define LLC_ANALYSE_FRAME_757_112_2_18_0_4_37_124 "LLC: I frame N(S) invalid,discard"
#define LLC_ANALYSE_FRAME_784_112_2_18_0_4_37_125 "LLC:Assert Failure,DeleteSHeadNode empty 0!"
#define LLC_ANALYSE_FRAME_921_112_2_18_0_4_37_126 "LLC:Assert Failure,DeleteSHeadNode empty!"
#define LLC_ANALYSE_FRAME_973_112_2_18_0_4_37_127 "LLC:Assert Failure,frame size of dummy UI is wrong!"
#define LLC_COMMON_CODE_337_112_2_18_0_4_38_128 "LLC:Assert Failure,wrong sapi value!"
#define LLC_COMMON_CODE_426_112_2_18_0_4_38_129 "LLC:Assert Failure,SetFRMRErrMsg wrong error type!"
#define LLC_COMMON_CODE_831_112_2_18_0_4_39_130 "LLC:Assert Failure,Polling next U node wrong value!"
#define LLC_COMMON_CODE_875_112_2_18_0_4_39_131 "LLC:Assert Failure,InvalidXID next U node wrong value!"
#define LLC_COMMON_CODE_1011_112_2_18_0_4_39_132 "LLC:KC changed and reset IOV_UI/I"
#define LLC_COMMON_CODE_1267_112_2_18_0_4_40_133 "LLC:Cipher ASSERT !! gea_ciphering failed!"
#define LLC_COMMON_CODE_1329_112_2_18_0_4_40_134 "LLC:Cipher ASSERT !! gea_ciphering failed!"
#define LLC_COMMON_CODE_1383_112_2_18_0_4_40_135 "LLC: Xid negotiation N201-U=%d"
#define LLC_COMMON_CODE_1423_112_2_18_0_4_40_136 "LLC:gmm need cipher misstate!"
#define LLC_COMMON_CODE_1947_112_2_18_0_4_41_137 "LLCCC_SendSignalToLlentity: signal to be sent is 0x%x "
#define LLC_COMMON_CODE_1977_112_2_18_0_4_41_138 "LLCCC_SendSignalToLlentity: assert error, unprocessed signal 0x%x"
#define LLC_COMPOSE_FRAME_163_112_2_18_0_4_42_139 "LLC:UPLINK XID PARAMETES-->T200:%d"
#define LLC_COMPOSE_FRAME_173_112_2_18_0_4_42_140 "LLC:UPLINK XID PARAMETES-->N200:%d"
#define LLC_COMPOSE_FRAME_185_112_2_18_0_4_42_141 "LLC:UPLINK XID PARAMETES-->N201 U:%d"
#define LLC_COMPOSE_FRAME_202_112_2_18_0_4_42_142 "LLC:UPLINK XID PARAMETES-->N201 I:%d"
#define LLC_COMPOSE_FRAME_214_112_2_18_0_4_42_143 "LLC:UPLINK XID PARAMETES-->mD:%d"
#define LLC_COMPOSE_FRAME_226_112_2_18_0_4_42_144 "LLC:UPLINK XID PARAMETES-->mU:%d"
#define LLC_COMPOSE_FRAME_236_112_2_18_0_4_42_145 "LLC:UPLINK XID PARAMETES-->kD:%d"
#define LLC_COMPOSE_FRAME_246_112_2_18_0_4_42_146 "LLC:UPLINK XID PARAMETES-->kU:%d"
#define LLC_COMPOSE_FRAME_280_112_2_18_0_4_42_147 "LLC:UPLINK XID PARAMETES-->zero-length Layer 3 Parameters"
#define LLC_COMPOSE_FRAME_287_112_2_18_0_4_42_148 "LLC:UPLINK XID PARAMETES-->zero-length Layer 3 Parameters"
#define LLC_COMPOSE_FRAME_664_112_2_18_0_4_43_149 "LLC:Assert Failure,ComposeIFrame S empty!"
#define LLC_COMPOSE_FRAME_1082_112_2_18_0_4_43_150 "LLC: in set frame status\n"
#define LLC_COMPOSE_FRAME_1123_112_2_18_0_4_44_151 "LLC: in set frame before n(r) acked\n"
#define LLC_COMPOSE_FRAME_1296_112_2_18_0_4_44_152 "LLC:decode I frame SACK frame error!"
#define LLC_COMPOSE_FRAME_1385_112_2_18_0_4_44_153 "LLC: SACK error bitmap"
#define LLC_COMPOSE_FRAME_1559_112_2_18_0_4_45_154 "LLC:Assert Failure,SACK DeleteSNode S rnr invalid or empty"
#define LLC_COMPOSE_FRAME_1575_112_2_18_0_4_45_155 "LLC:Assert Failure,SACK DeleteSNode S invalid or empty"
#define LLC_COMPOSE_FRAME_1686_112_2_18_0_4_45_156 "LLC:Assert Failure,DeleteSNode S rnr invalid or empty"
#define LLC_COMPOSE_FRAME_1702_112_2_18_0_4_45_157 "LLC:Assert Failure,DeleteSNode S invalid or empty"
#define LLC_COMPOSE_FRAME_1782_112_2_18_0_4_45_158 "LLC:Assert Failure,DeleteUINode empty or invalid"
#define LLC_COMPOSE_FRAME_1814_112_2_18_0_4_45_159 "LLC:Assert Failure,ComposeNewFrame  UI empty or invalid"
#define LLC_QUEUE_406_112_2_18_0_4_47_160 "LLC:Add U frame pos = %d"
#define LLC_QUEUE_463_112_2_18_0_4_47_161 "LLC:Add UI frame pos = %d"
#define LLC_QUEUE_564_112_2_18_0_4_47_162 "LLC:DeleteSHeadNode S queue empty!"
#define LLC_QUEUE_724_112_2_18_0_4_48_163 "LLC: DeleteSNode, line = %d"
#define LLC_QUEUE_784_112_2_18_0_4_48_164 "LLC: LLCLQ_DeleteUNode index = %d invalid"
#define LLC_QUEUE_842_112_2_18_0_4_48_165 "LLC: LLCLQ_DeleteUINode index = %d invalid"
#define LLC_QUEUE_959_112_2_18_0_4_48_166 "LLC: DeleteINode, line = %d"
#define LLC_QUEUE_1072_112_2_18_0_4_49_167 "LLC: DeleteReceiveNode, line = %d"
#define LLC_QUEUE_1603_112_2_18_0_4_50_168 "LLC:Assert Failure,LLC_GlobalInit,sapi invalid!"
#define LLC_QUEUE_1750_112_2_18_0_4_50_169 "LLC: in ResetRecvQueueWithSize, line = %d"
#define LLC_SDL_145_112_2_18_0_4_51_170 "LLentity:free,reliability error!"
#define LLC_SDL_314_112_2_18_0_4_51_171 "LLC:invalid lle_reliability = %d"
#define LLC_SDL_474_112_2_18_0_4_51_172 "LLC:LLC_SendUIQueue i = %d"
#define LLC_SDL_604_112_2_18_0_4_51_173 "LLentity: discard l3 pdu, abnormal"
#define LLC_TEST_47_112_2_18_0_4_52_174 "LLC_TEST:lle_ResetT201 expire!"
#define LLC_TEST_52_112_2_18_0_4_52_175 "LLC_TEST:lle_SetT200 expire!"
#define LLC_TEST_57_112_2_18_0_4_52_176 "LLC_TEST:lle_SetT201 expire!"
#define LLC_XID_CODE_972_112_2_18_0_4_54_177 "LLC:Log XID frame parameter:"
#define LLC_XID_CODE_975_112_2_18_0_4_54_178 "LLC:xid Reset"
#define LLC_XID_CODE_979_112_2_18_0_4_54_179 "LLC:Version = %d"
#define LLC_XID_CODE_983_112_2_18_0_4_54_180 "LLC:T200 = %d"
#define LLC_XID_CODE_987_112_2_18_0_4_54_181 "LLC:N200 = %d"
#define LLC_XID_CODE_991_112_2_18_0_4_54_182 "LLC:N201_U = %d"
#define LLC_XID_CODE_995_112_2_18_0_4_54_183 "LLC:N201_I = %d"
#define LLC_XID_CODE_999_112_2_18_0_4_54_184 "LLC:mD = %d"
#define LLC_XID_CODE_1003_112_2_18_0_4_54_185 "LLC:mU = %d"
#define LLC_XID_CODE_1007_112_2_18_0_4_54_186 "LLC:kD = %d"
#define LLC_XID_CODE_1011_112_2_18_0_4_54_187 "LLC:kU = %d"
#define LLC_XID_CODE_1015_112_2_18_0_4_54_188 "LLC:IOV_UI = 0x%08x"
#define LLC_XID_CODE_1019_112_2_18_0_4_54_189 "LLC:IOV_I = 0x%08x"
#define LLC_XID_CODE_1023_112_2_18_0_4_54_190 "LLC:SNDCP xid Parameters included"
#define LLC_XID_CODE_1064_112_2_18_0_4_55_191 "LLC:the received U frame does not include xid parameter!"
#define MAC_CSN_MSG_1171_112_2_18_0_4_58_192 "MAC:egprs message!"
#define MAC_CSN_MSG_1612_112_2_18_0_4_59_193 "MAC:DL PWR CTRL %d, MODE %d, PR MODE %d"
#define MAC_CSN_MSG_2940_112_2_18_0_5_2_194 "MAC:ul ack alloc fix alloction,ms not support!"
#define MAC_CSN_MSG_3021_112_2_18_0_5_2_195 "MAC:ul ack alloc fix alloction,ms not support!"
#define MAC_CSN_MSG_3130_112_2_18_0_5_2_196 "MAC:DL PWR CTRL %d, MODE %d, PR MODE %d"
#define MAC_CSN_MSG_3144_112_2_18_0_5_2_197 "MAC:uplink ass allo fix allocation,ms not support!"
#define MAC_CSN_MSG_3235_112_2_18_0_5_2_198 "MAC:uplink ass allo fix allocation,ms not support!"
#define MAC_CSN_MSG_3313_112_2_18_0_5_2_199 "MAC:uplink ass allo fix allocation,ms not support!"
#define MAC_CSN_MSG_3393_112_2_18_0_5_3_200 "MAC:uplink ass allo fix allocation,ms not support!"
#define MAC_CSN_MSG_3482_112_2_18_0_5_3_201 "MAC:Packet Cell Change Failure have FDD_ind!! ERR!!!"
#define MAC_CSN_MSG_3487_112_2_18_0_5_3_202 "MAC:Packet Cell Change Failure have TDD_ind!! ERR!!!"
#define MAC_CSN_MSG_3673_112_2_18_0_5_3_203 "MAC:Unknown access type, error!!!"
#define MAC_CSN_MSG_3966_112_2_18_0_5_4_204 "MAC:Packet Measurement Report have 3g_meas_report!ERR!!!"
#define MAC_CSN_MSG_4075_112_2_18_0_5_4_205 "MAC:L_length =%d is not enough when encode crbb_len!!\n"
#define MAC_CSN_MSG_4086_112_2_18_0_5_4_206 "MAC:L_length =%d is not enough when encode comp_bitmap_start_color_code!!\n"
#define MAC_CSN_MSG_4131_112_2_18_0_5_4_207 "MAC:L_length =%d is not enough when encode crbb!!\n"
#define MAC_CSN_MSG_4180_112_2_18_0_5_4_208 "MAC:L_length =%d is not enough when encode urbb!!\n"
#define MAC_CSN_MSG_4425_112_2_18_0_5_5_209 "PRR-encode: %d"
#define MAC_CSN_MSG_4430_112_2_18_0_5_5_210 "PRR-encode: %d"
#define MAC_CSN_MSG_4437_112_2_18_0_5_5_211 "PRR-encode: %d"
#define MAC_CSN_MSG_4443_112_2_18_0_5_5_212 "PRR-encode: %d"
#define MAC_CSN_MSG_4451_112_2_18_0_5_5_213 "PRR-encode: %d"
#define MAC_CSN_MSG_4456_112_2_18_0_5_5_214 "PRR-encode: %d"
#define MAC_CSN_MSG_4467_112_2_18_0_5_5_215 "PRR-encode: %d"
#define MAC_CSN_MSG_4472_112_2_18_0_5_5_216 "PRR-encode: %d"
#define MAC_CSN_MSG_4480_112_2_18_0_5_5_217 "PRR-encode: %d"
#define MAC_CSN_MSG_4486_112_2_18_0_5_5_218 "PRR-encode: %d"
#define MAC_CSN_MSG_4494_112_2_18_0_5_5_219 "PRR-encode: %d"
#define MAC_CSN_MSG_4499_112_2_18_0_5_5_220 "PRR-encode: %d"
#define MAC_CSN_MSG_4507_112_2_18_0_5_5_221 "PRR-encode: %d"
#define MAC_CSN_MSG_4514_112_2_18_0_5_5_222 "PRR-encode: %d"
#define MAC_CSN_MSG_4521_112_2_18_0_5_5_223 "PRR-encode: %d"
#define MAC_CSN_MSG_4529_112_2_18_0_5_5_224 "PRR-encode: %d"
#define MAC_CSN_MSG_4536_112_2_18_0_5_5_225 "PRR-encode: %d"
#define MAC_CSN_MSG_4542_112_2_18_0_5_5_226 "PRR-encode: %d"
#define MAC_CSN_MSG_4548_112_2_18_0_5_5_227 "PRR-encode: %d"
#define MAC_CSN_MSG_4558_112_2_18_0_5_5_228 "PRR-encode: %d"
#define MAC_CSN_MSG_4564_112_2_18_0_5_5_229 "PRR-encode: %d"
#define MAC_CSN_MSG_4770_112_2_18_0_5_5_230 "MACCSN_DecodeNcFreqList:ASSERT,add_freq_str.freq_diff_len is 0,return FALSE"
#define MAC_CSN_MSG_6503_112_2_18_0_5_9_231 "MAC:Error,the Access Technology Type num beyond 7!"
#define MAC_CSN_MSG_6588_112_2_18_0_5_9_232 "MAC:DL PWR CTRL %d, PR MODE %d"
#define MAC_CSN_MSG_6675_112_2_18_0_5_9_233 "PRR-encode: %d"
#define MAC_CSN_MSG_6700_112_2_18_0_5_9_234 "PRR-encode: %d"
#define MAC_CSN_MSG_6710_112_2_18_0_5_9_235 "PRR-encode: %d"
#define MAC_CSN_MSG_6744_112_2_18_0_5_9_236 "PRR-encode: %d"
#define MAC_CSN_MSG_6749_112_2_18_0_5_9_237 "PRR-encode: %d"
#define MAC_CSN_MSG_6767_112_2_18_0_5_9_238 "PRR-encode: %d"
#define MAC_CSN_MSG_6775_112_2_18_0_5_9_239 "PRR-encode: %d"
#define MAC_CSN_MSG_6780_112_2_18_0_5_9_240 "PRR-encode: %d"
#define MAC_CSN_MSG_6786_112_2_18_0_5_9_241 "PRR-encode: %d"
#define MAC_CSN_MSG_6791_112_2_18_0_5_9_242 "PRR-encode: %d"
#define MAC_CSN_MSG_6798_112_2_18_0_5_9_243 "PRR-encode: %d"
#define MAC_CSN_MSG_6803_112_2_18_0_5_9_244 "PRR-encode: %d"
#define MAC_CSN_MSG_6812_112_2_18_0_5_9_245 "PRR-encode: %d"
#define MAC_CSN_MSG_6817_112_2_18_0_5_9_246 "PRR-encode: %d"
#define MAC_CSN_MSG_6823_112_2_18_0_5_9_247 "PRR-encode: %d"
#define MAC_CSN_MSG_6828_112_2_18_0_5_9_248 "PRR-encode: %d"
#define MAC_CSN_MSG_6835_112_2_18_0_5_9_249 "PRR-encode: %d"
#define MAC_CSN_MSG_6840_112_2_18_0_5_9_250 "PRR-encode: %d"
#define MAC_CSN_MSG_6846_112_2_18_0_5_10_251 "PRR-encode: %d"
#define MAC_CSN_MSG_6851_112_2_18_0_5_10_252 "PRR-encode: %d"
#define MAC_CSN_MSG_6863_112_2_18_0_5_10_253 "PRR-encode: %d"
#define MAC_CSN_MSG_6868_112_2_18_0_5_10_254 "PRR-encode: %d"
#define MAC_CSN_MSG_6885_112_2_18_0_5_10_255 "PRR-encode: %d"
#define MAC_CSN_MSG_6931_112_2_18_0_5_10_256 "MAC: Current cell band = %d"
#define MAC_CSN_MSG_7659_112_2_18_0_5_12_257 "MAC:  RrPacketUlAssi fix alloc"
#define MAC_CSN_MSG_7664_112_2_18_0_5_12_258 "MAC: EGPRS msg Ext alloc"
#define MAC_CSN_MSG_7772_112_2_18_0_5_12_259 "MAC:DL PWR CTRL %d, MODE %d, PR MODE %d"
#define MAC_DECODE_DATA_IND_322_112_2_18_0_5_14_260 "MAC: tfi ie not present"
#define MAC_DECODE_DATA_IND_336_112_2_18_0_5_14_261 "MAC: msg rx on slot%d's global tfi matched the ul tfi, but not on the assigned ul slot!, "
#define MAC_DECODE_DATA_IND_355_112_2_18_0_5_14_262 "MAC: msg rx on slot%d's global tfi matched the dl tfi, but not on the assigned dl slot!, "
#define MAC_DECODE_DATA_IND_366_112_2_18_0_5_14_263 "MAC:Assert Failure, tfi type invalid"
#define MAC_DECODE_DATA_IND_471_112_2_18_0_5_14_264 "MAC: ignore msg due to tlli mismatch"
#define MAC_DECODE_DATA_IND_474_112_2_18_0_5_14_265 "MAC: match the old tlli"
#define MAC_DECODE_DATA_IND_478_112_2_18_0_5_14_266 "MAC: match the new tlli"
#define MAC_DECODE_DATA_IND_489_112_2_18_0_5_14_267 "MAC: Assert Failure,Invalid addr"
#define MAC_DECODE_DATA_IND_576_112_2_18_0_5_14_268 "MAC: ignore msg due to tlli mismatch"
#define MAC_DECODE_DATA_IND_579_112_2_18_0_5_14_269 "MAC: match the old tlli"
#define MAC_DECODE_DATA_IND_583_112_2_18_0_5_14_270 "MAC: match the new tlli"
#define MAC_DECODE_DATA_IND_590_112_2_18_0_5_14_271 "MAC: Assert Failure,measur order msg error"
#define MAC_DECODE_DATA_IND_617_112_2_18_0_5_14_272 "MAC: ignore msg due to tlli mismatch"
#define MAC_DECODE_DATA_IND_620_112_2_18_0_5_14_273 "MAC: match the old tlli"
#define MAC_DECODE_DATA_IND_624_112_2_18_0_5_14_274 "MAC: match the new tlli"
#define MAC_DECODE_DATA_IND_631_112_2_18_0_5_14_275 "MAC: Assert Failure,polling req msg error"
#define MAC_DECODE_DATA_IND_643_112_2_18_0_5_14_276 "MAC:The packet queueing nitify message has not request reference!"
#define MAC_DECODE_DATA_IND_698_112_2_18_0_5_14_277 "MAC: PAN on slot%d's tfi matched the ul tfi, but not on the assigned ul slot!, "
#define MAC_DECODE_DATA_IND_739_112_2_18_0_5_14_278 "MAC: Assert Failure,unknown msg"
#define MAC_DECODE_DATA_IND_745_112_2_18_0_5_14_279 "MAC: MSG decode error!"
#define MAC_DECODE_DATA_IND_749_112_2_18_0_5_14_280 "MAC: MSG decode error!"
#define MAC_DECODE_DATA_IND_755_112_2_18_0_5_14_281 "MAC: MSG decode error!"
#define MAC_DECODE_DATA_IND_865_112_2_18_0_5_15_282 "MAC: Assert Failure,unknow decode msg error"
#define MAC_DECODE_DATA_IND_970_112_2_18_0_5_15_283 "MAC:CTRL REASSEMBLE:T3200 EXPIRE TS %d"
#define MAC_DECODE_DATA_IND_999_112_2_18_0_5_15_284 "MAC: msg rx on slot%d's header tfi is dl tfi, but not on the assigned dl slot!, "
#define MAC_DECODE_DATA_IND_1011_112_2_18_0_5_15_285 "MAC: msg rx on slot%d's header tfi is ul tfi, but not on the assigned ul slot!, "
#define MAC_DECODE_DATA_IND_1052_112_2_18_0_5_15_286 "MAC:CTRL REASSEMBLE PDCH TS %d"
#define MAC_DECODE_DATA_IND_1130_112_2_18_0_5_15_287 "MAC: Assert Failure,assemb control eror"
#define MAC_DECODE_DATA_IND_1139_112_2_18_0_5_15_288 "MAC:CTRL REASSEMBLE SAVE BLOCK TS %d"
#define MAC_DECODE_DATA_IND_1209_112_2_18_0_5_15_289 "MAC: invalid rrbp value,ignore it!!"
#define MAC_DECODE_DATA_IND_1260_112_2_18_0_5_15_290 "MAC: recv data block on slot%d tfi match but not on the assigned dl slots"
#define MAC_DECODE_DATA_IND_1351_112_2_18_0_5_16_291 "MAC: recv egprs data block on slot%d tfi match but not on the assigned dl slots"
#define MAC_DECODE_DATA_IND_1507_112_2_18_0_5_16_292 "MAC:Receive MCSERROR data!"
#define MAC_DECODE_DATA_IND_1512_112_2_18_0_5_16_293 "MAC:Receive unexpected send_type data!"
#define MAC_DECODE_DATA_IND_1749_112_2_18_0_5_16_294 "MAC: discard the second ul ack"
#define MAC_DECODE_DATA_IND_1756_112_2_18_0_5_16_295 "MAC: ingore msg %x"
#define MAC_DECODE_DATA_IND_1767_112_2_18_0_5_16_296 "MAC: in wrong state receive packet uplink ack/unack"
#define MAC_DECODE_DATA_IND_1834_112_2_18_0_5_17_297 "MAC: presdo tbf starting time, pkt dl assignment!"
#define MAC_DECODE_DATA_IND_1843_112_2_18_0_5_17_298 "MAC: presdo tbf starting time, pkt ul assignment!"
#define MAC_DECODE_DATA_IND_1852_112_2_18_0_5_17_299 "MAC: presdo tbf starting time, pkt ts reconfig!"
#define MAC_DECODE_DATA_IND_1871_112_2_18_0_5_17_300 "MAC: unknown msg type %d"
#define MAC_DECODE_DATA_IND_1913_112_2_18_0_5_17_301 "MAC: Ingore the polling flag!"
#define MAC_DECODE_DATA_IND_1933_112_2_18_0_5_17_302 "MAC: Assert Failure,Invalid control ack type"
#define MAC_DECODE_DATA_IND_1963_112_2_18_0_5_17_303 "MAC: Assert failure1,Invalid control ack type"
#define MAC_DECODE_DATA_IND_2047_112_2_18_0_5_17_304 "MAC:wrong MCS/CS, discard invalid data"
#define MAC_DECODE_DATA_IND_2117_112_2_18_0_5_17_305 "MAC: error payload type %d"
#define MAC_DECODE_DATA_IND_2145_112_2_18_0_5_17_306 "SCT loop 8psk data fail"
#define MAC_DECODE_DATA_IND_2217_112_2_18_0_5_18_307 "MAC: error payload type %d"
#define MAC_DECODE_DATA_IND_2301_112_2_18_0_5_18_308 "MAC: error,pdch assi invalid pd type %d"
#define MAC_DECODE_DATA_IND_2358_112_2_18_0_5_18_309 "MAC:error mutislot control message!"
#define MAC_DECODE_DATA_IND_2419_112_2_18_0_5_18_310 "MAC: tlli mismatch in PAR msg!Select next!"
#define MAC_DECODE_MA_440_112_2_18_0_5_19_311 "MAC: GPRS class > 12 %d"
#define MAC_DECODE_MA_453_112_2_18_0_5_19_312 "MAC: Multislot mismatch, UL timeslot assi num: %d"
#define MAC_DECODE_MA_461_112_2_18_0_5_19_313 "MAC: Multislot mismatch, DL timeslot assi num: %d"
#define MAC_DECODE_MA_470_112_2_18_0_5_19_314 "MAC: Multislot mismatch, UL timeslot assi num: %d DL timeslot assi num: %d"
#define MAC_DECODE_MA_498_112_2_18_0_5_19_315 "MAC: UL slot %x mismatch DL slot %x"
#define MAC_DECODE_MA_553_112_2_18_0_5_19_316 "MAC: polling slot before first rx slot, discard!"
#define MAC_DECODE_MA_560_112_2_18_0_5_19_317 "MAC: old polling occupy new assignment slot after changed, discard!"
#define MAC_DECODE_MA_567_112_2_18_0_5_19_318 "MAC: check polling validate, polling slot and the rx slot collision"
#define MAC_DECODE_MA_606_112_2_18_0_5_20_319 "MAC: GPRS class > 12 %d"
#define MAC_DECODE_MA_611_112_2_18_0_5_20_320 "MAC: check polling validate, violates MS capability"
#define MAC_DECODE_MA_833_112_2_18_0_5_20_321 "MAC: Decode ma error, ref arfcn num=0!"
#define MAC_DECODE_MA_881_112_2_18_0_5_20_322 "MAC: Decode ma error, arfcn num!=bitmap len %d %d!"
#define MAC_DECODE_MA_944_112_2_18_0_5_20_323 "MAC: Decode ma error, arfcn num=0!"
#define MAC_DECODE_MA_951_112_2_18_0_5_20_324 "MAC: Decode ma error, arfcn not in band!"
#define MAC_DECODE_MA_1007_112_2_18_0_5_20_325 "MAC: Freq para update error in PSI2, invalid ma number!"
#define MAC_DECODE_MA_1019_112_2_18_0_5_20_326 "MAC: Freq para update error in PSI2, decode ma error!"
#define MAC_DECODE_MA_1045_112_2_18_0_5_20_327 "MAC: Freq para update error in SI13, decode ma error!"
#define MAC_DECODE_MA_1066_112_2_18_0_5_20_328 "MAC: Invalid freq param type"
#define MAC_DECODE_MA_1100_112_2_18_0_5_21_329 "MAC: Decode freq para error, decode ma error,freq no in band!"
#define MAC_DECODE_MA_1115_112_2_18_0_5_21_330 "MAC: Decode freq para error, invalid ma number %d!"
#define MAC_DECODE_MA_1124_112_2_18_0_5_21_331 "MAC: Decode freq para error, ma number not referenced %d!"
#define MAC_DECODE_MA_1135_112_2_18_0_5_21_332 "MAC: Decode freq para error, ma number=15 but change mark present!"
#define MAC_DECODE_MA_1159_112_2_18_0_5_21_333 "MAC: Decode freq para error, change mark not match!"
#define MAC_DECODE_MA_1167_112_2_18_0_5_21_334 "MAC: Decode freq para error, change mark not match!"
#define MAC_DECODE_MA_1188_112_2_18_0_5_21_335 "MAC: Decode freq para error, decode ma error!"
#define MAC_DECODE_MA_1206_112_2_18_0_5_21_336 "MAC: Decode freq bitmap error!"
#define MAC_DECODE_MA_1213_112_2_18_0_5_21_337 "MAC: Decode freq para error, decode ma error,freq no in band!"
#define MAC_DECODE_MA_1226_112_2_18_0_5_21_338 "MAC: Assert Failure,Invalid freq pars selector %d"
#define MAC_DECODE_MA_1476_112_2_18_0_5_21_339 "MAC: alloc extended dynamic allocation, but not support"
#define MAC_DECODE_MA_1481_112_2_18_0_5_21_340 "MAC: network alloc EXTENDED_DYNAMIC!"
#define MAC_DECODE_MA_1534_112_2_18_0_5_21_341 "MAC: dynamic allocation, allocation no ul slot"
#define MAC_DECODE_MA_1541_112_2_18_0_5_21_342 "MAC: violate ms multislot capability"
#define MAC_DECODE_MA_1562_112_2_18_0_5_21_343 "MAC: receive pkt uplink ass with no tfi when establish uplink TBF"
#define MAC_DECODE_MA_1661_112_2_18_0_5_22_344 "MAC: single tbf get slot mask, not on starting time, st = %d, fn = %d"
#define MAC_DECODE_MA_1789_112_2_18_0_5_22_345 "absolute frame number"
#define MAC_DECODE_MA_1798_112_2_18_0_5_22_346 "MAC:MACMA_DecodeUlMultiBlockAllocation:%d"
#define MAC_DECODE_MA_1860_112_2_18_0_5_22_347 "MAC: Decode freq para error, decode ma error,freq no in band!"
#define MAC_DECODE_MA_1884_112_2_18_0_5_22_348 "MAC: Decode packet channel descri freq error(invalid ma number)!\n"
#define MAC_DECODE_MA_1900_112_2_18_0_5_22_349 "MAC: Decode packet channel descri freq error(change mark not match)\n"
#define MAC_DECODE_MA_1918_112_2_18_0_5_22_350 "MAC: Decode packet channel descri freq error(num=0)\n"
#define MAC_DECODE_MA_1950_112_2_18_0_5_22_351 "MAC: invalid polling frame number=%d"
#define MAC_DECODE_MA_1977_112_2_18_0_5_22_352 "MAC:Invalid control ack type for response imm ass!"
#define MAC_DECODE_MA_2171_112_2_18_0_5_23_353 "MAC: network alloc fix allocation,ms not support!"
#define MAC_DECODE_MA_2305_112_2_18_0_5_23_354 "MAC: network alloc fix allocation,ms not support!"
#define MAC_DECODE_MA_2353_112_2_18_0_5_23_355 "MAC: tlli mismatch in DL assi! MS: %x  Assi: %x"
#define MAC_DECODE_MA_2356_112_2_18_0_5_23_356 "MAC: match the old tlli"
#define MAC_DECODE_MA_2360_112_2_18_0_5_23_357 "MAC: match the new tlli"
#define MAC_DECODE_MA_2369_112_2_18_0_5_23_358 "MAC:imm ass without egprs content whereascurrent TBF mode is EGPRS, ignore message!"
#define MAC_DECODE_MA_2551_112_2_18_0_5_23_359 "MAC: No ta_value in global_timing_advance!"
#define MAC_DECODE_MA_2606_112_2_18_0_5_24_360 "MAC: No valid tfi to encode in pccn msg"
#define MAC_DECODE_MA_2620_112_2_18_0_5_24_361 "MAC: rlc add %d control msg"
#define MAC_DECODE_MA_2949_112_2_18_0_5_24_362 "MAC: prach persist_level is wrong!"
#define MAC_LOGEL_MSG_1747_112_2_18_0_5_29_363 "MAC:ERROR!L in EGPRS Ack/Nack Description IE = %d,beyond range!!\n"
#define MAC_LOGEL_MSG_1772_112_2_18_0_5_29_364 "MAC:L_length =%d is not enough when encode crbb_len!!\n"
#define MAC_LOGEL_MSG_1783_112_2_18_0_5_29_365 "MAC:L_length =%d is not enough when encode comp_bitmap_start_color_code!!\n"
#define MAC_LOGEL_MSG_1828_112_2_18_0_5_29_366 "MAC:L_length =%d is not enough when encode crbb!!\n"
#define MAC_LOGEL_MSG_1876_112_2_18_0_5_29_367 "MAC:L_length =%d is not enough when encode urbb!!\n"
#define MAC_LOGEL_MSG_4924_112_2_18_0_5_35_368 "MAC: EGPRS msg BEP"
#define MAC_LOGEL_MSG_4929_112_2_18_0_5_35_369 "MAC: EGPRS msg ETimeslot"
#define MAC_PH_371_112_2_18_0_5_38_370 "MAC Start t3206!"
#define MAC_PH_399_112_2_18_0_5_38_371 "MAC:rr add msg =%d have invalid tfi,add fail!"
#define MAC_PH_484_112_2_18_0_5_38_372 "MAC:MACPH_DelElementInCtrlQ find no element in Ctrl!"
#define MAC_PH_499_112_2_18_0_5_38_373 "MAC:del num > amount in control queue,check!"
#define MAC_PH_629_112_2_18_0_5_38_374 "MAC: add the second timeslot polling data!"
#define MAC_PH_653_112_2_18_0_5_38_375 "MAC: replace the more than 2 timeslot polling data!"
#define MAC_PH_658_112_2_18_0_5_38_376 "MAC: discard the more than 2 timeslot polling data!"
#define MAC_PH_784_112_2_18_0_5_39_377 "pkt-resource-req: psi2 change mark"
#define MAC_PH_791_112_2_18_0_5_39_378 "pkt-resource-req: psi13 change mark"
#define MAC_PH_797_112_2_18_0_5_39_379 "pkt-resource-req: psi change mark not present"
#define MAC_PH_917_112_2_18_0_5_39_380 "MAC:net_command_bitmap = %d,common_bitmap=%d"
#define MAC_PH_1060_112_2_18_0_5_39_381 "MAC: get actual band = %d"
#define MAC_PH_1158_112_2_18_0_5_39_382 "MAC: network access_tech_type is GSM_P,but we don't support in nv!"
#define MAC_PH_1172_112_2_18_0_5_39_383 "MAC: network access_tech_type is GSM_E,but we don't support in nv!"
#define MAC_PH_1186_112_2_18_0_5_39_384 "MAC: network access_tech_type is GSM_1800,but we don't support in nv!"
#define MAC_PH_1200_112_2_18_0_5_40_385 "MAC: network access_tech_type is GSM_1900,but we don't support in nv!"
#define MAC_PH_1214_112_2_18_0_5_40_386 "MAC: network access_tech_type is GSM_850,but we don't support in nv!"
#define MAC_PH_1221_112_2_18_0_5_40_387 "MAC:requested access type %d not supported!"
#define MAC_PH_1240_112_2_18_0_5_40_388 "MAC:can only encode %d access tech type,the next should be encoded is %d"
#define MAC_PH_1257_112_2_18_0_5_40_389 "MAC:ms_ra_capability_ptr->num_of_elems=%d,common_band_num= %d"
#define MAC_PH_1260_112_2_18_0_5_40_390 "MAC :access_tech_req_num =%d"
#define MAC_PH_1309_112_2_18_0_5_40_391 "pkt-resource-req: sign var not present, is two phase %d"
#define MAC_PH_1316_112_2_18_0_5_40_392 "pkt-resource-req: var present"
#define MAC_PH_1330_112_2_18_0_5_40_393 "pkt-resource-req: 2 phase"
#define MAC_PH_1358_112_2_18_0_5_40_394 "pkt-resource-req: racap 2"
#define MAC_PH_1373_112_2_18_0_5_40_395 "pkt-resource-req: bcch"
#define MAC_PH_1505_112_2_18_0_5_40_396 "MAC:no EGPRS Timeslot Link Quality Measurements Information! bits= %d"
#define MAC_PH_1631_112_2_18_0_5_40_397 "MAC:Encode ARAC, but no access technology in ul assign!!!"
#define MAC_PH_1827_112_2_18_0_5_41_398 "MAC: get block %d to send in poll"
#define MAC_PH_1911_112_2_18_0_5_41_399 "MAC:send packet resource request, don't start 3168"
#define MAC_PH_1986_112_2_18_0_5_41_400 "MAC: GMSK BEP: mean = %d, cv = %d"
#define MAC_PH_2002_112_2_18_0_5_41_401 "MAC: 8PSK BEP: mean = %d, cv = %d"
#define MAC_PH_2240_112_2_18_0_5_42_402 "MAC:send PCCN instead of packet downlink ack!"
#define MAC_PH_2300_112_2_18_0_5_42_403 "MAC:Mac is already start t3192 when send dl ack!"
#define MAC_PH_2373_112_2_18_0_5_42_404 "MAC:send PCCN instead of egprs packet downlink ack!"
#define MAC_PH_2409_112_2_18_0_5_42_405 "MAC:Mac is already start t3192 when send egprs dl ack!"
#define MAC_PH_2486_112_2_18_0_5_42_406 "MAC: Invalid type of polling block"
#define MAC_PH_2581_112_2_18_0_5_42_407 "MAC: send pccn from CTRL QUEUE!"
#define MAC_PH_2663_112_2_18_0_5_42_408 "MAC: delte ctrl queue find pccn"
#define MAC_PH_2935_112_2_18_0_5_43_409 "MAC: polling message type error"
#define MAC_PH_2966_112_2_18_0_5_43_410 "MAC: send pccn from poll queue!"
#define MAC_PH_2991_112_2_18_0_5_43_411 "MAC: No valid tfi to encode in pccn msg"
#define MAC_PH_3005_112_2_18_0_5_43_412 "mac:uplink tfi in pccn =%d"
#define MAC_PH_3013_112_2_18_0_5_43_413 "mac:downlink tfi in pccn =%d"
#define MM_C_CODE_289_112_2_18_0_5_55_414 "MM: decode Msg Header,MESSAGE_TOO_SHORT"
#define MM_C_CODE_317_112_2_18_0_5_55_415 "MM: decode Msg Header,SKIP_INDICATOR_NOT_ZERO"
#define MM_C_CODE_351_112_2_18_0_5_55_416 "MM: decode Msg Header,INVALID_MESSAGE_TYPE"
#define MM_C_CODE_363_112_2_18_0_5_55_417 "MM: decode Msg Header,INVALID_PROTOCOL_DISCRIMINATOR"
#define MM_C_CODE_430_112_2_18_0_5_55_418 "MM: decode Location updating accept msg,min len error."
#define MM_C_CODE_441_112_2_18_0_5_55_419 "MM: decode Location updating accept msg,LAI invalid."
#define MM_C_CODE_449_112_2_18_0_5_55_420 "MM:Location updating accept's LAI != current cell LAI"
#define MM_C_CODE_471_112_2_18_0_5_55_421 "MM: decode Location updating accept msg,%d"
#define MM_C_CODE_495_112_2_18_0_5_55_422 "MM: decode MM_MOBILE_IDENTITY_IEI failure"
#define MM_C_CODE_541_112_2_18_0_5_55_423 "MM: decode MM_EMERGENCY_NUM_LIST_IEI failure"
#define MM_C_CODE_546_112_2_18_0_5_55_424 "MM: receive wrong msg %d"
#define MM_C_CODE_572_112_2_18_0_5_55_425 "MM: decode Location updating reject msg,MANDATORY_IE_CONTENT_ERROR"
#define MM_C_CODE_599_112_2_18_0_5_55_426 "MM CODEC: Assert Failure, EncodeLocUpdReq: CKSN %d error, change to invalid"
#define MM_C_CODE_664_112_2_18_0_5_55_427 "MM: decode Authentication request msg,MANDATORY_IE_CONTENT_ERROR"
#define MM_C_CODE_727_112_2_18_0_5_55_428 "MM: decode Identity request msg,MANDATORY_IE_CONTENT_ERROR"
#define MM_C_CODE_741_112_2_18_0_5_56_429 "MM: decode Identity request msg,MANDATORY_IE_CONTENT_ERROR"
#define MM_C_CODE_814_112_2_18_0_5_56_430 "MM: decode TMSI reallocation command msg,min len error"
#define MM_C_CODE_822_112_2_18_0_5_56_431 "MM: decode TMSI reallocation command msg,LAI failure"
#define MM_C_CODE_830_112_2_18_0_5_56_432 "MM:TMSI reallocation command's LAI != current cell LAI"
#define MM_C_CODE_841_112_2_18_0_5_56_433 "MM: decode TMSI reallocation command msg,Mobile Identity failure"
#define MM_C_CODE_887_112_2_18_0_5_56_434 "MM: decode CM service reject msg,MANDATORY_IE_CONTENT_ERROR"
#define MM_C_CODE_912_112_2_18_0_5_56_435 "MM: decode CM service reject msg,MANDATORY_IE_CONTENT_ERROR"
#define MM_C_CODE_979_112_2_18_0_5_56_436 "MM CODEC: Assert Failure, EncodeCMServiceReq: CKSN %d error, change to invalid"
#define MM_C_CODE_1035_112_2_18_0_5_56_437 "MM CODEC: Assert Failure, EncodeCMReestReq: CKSN %d error, change to invalid"
#define MM_C_CODE_1098_112_2_18_0_5_56_438 "MM: decode Abort msg,MANDATORY_IE_CONTENT_ERROR"
#define MM_C_CODE_1123_112_2_18_0_5_56_439 "MM CODEC: Assert Failure, EncodeMMStatus: Reject cause %d error, set to Protocol error, unspecified"
#define MM_C_CODE_1243_112_2_18_0_5_57_440 "MM CODEC: Assert Failure, DecodeMobileIdentity: offset %d error, there is no MobileIdentity to decode"
#define MM_C_CODE_1734_112_2_18_0_5_57_441 "MM: IsIeiValid temp_byte=%d, offset = %d, l3_msg_len = %d"
#define MM_C_CODE_1766_112_2_18_0_5_58_442 "MM_LaiIsInValidPlmnList result = %d"
#define RLC_DOWNLINK_645_112_2_18_0_6_4_443 "RLC: too many option octet in data block: %d\n"
#define RLC_DOWNLINK_651_112_2_18_0_6_4_444 "RLC: total data len error in data block: %d\n"
#define RLC_DOWNLINK_742_112_2_18_0_6_4_445 "RLC: too many option octet in data block: %d\n"
#define RLC_DOWNLINK_748_112_2_18_0_6_4_446 "RLC: total data len error in data block: %d\n"
#define RLC_DOWNLINK_904_112_2_18_0_6_4_447 "RLC: Reserved M+E: ME = '00'B\n"
#define RLC_DOWNLINK_1005_112_2_18_0_6_5_448 "RLC: Error option octet: LI = %d, E = %d\n"
#define RLC_MS_PARAM_49_112_2_18_0_6_8_449 "rlcul: MS support extended uplink TBF: %d"
#define RLC_NET_PARAM_252_112_2_18_0_6_9_450 "rlcul: network support extended uplink TBF: %d"
#define RLC_SCT_799_112_2_18_0_6_10_451 "rlcul: in loopback mode no data recved yet"
#define RLC_TX_PDU_POOL_73_112_2_18_0_6_13_452 "rlcul: alloc pdu, pid = %d"
#define RLC_TX_PDU_POOL_96_112_2_18_0_6_13_453 "rlcul: free pdu, pid = %d"
#define RLC_UPLINK_821_112_2_18_0_6_15_454 "rlcul: release error!"
#define RLC_UPLINK_2588_112_2_18_0_6_18_455 "rlcul: ext utbf, sending new block, reset t3182"
#define RLC_UPLINK_2660_112_2_18_0_6_18_456 "rlcul: all data block have been send, still get data block\n"
#define RLC_UPLINK_3972_112_2_18_0_6_21_457 "rlcul: gprs tx blk cs%d bsn = %d, va = %d, vs = %d"
#define RLC_UPLINK_3981_112_2_18_0_6_21_458 "rlcul: gprs retx %s blk cs%d bsn = %d, va = %d, vs = %d"
#define RLC_UPLINK_4095_112_2_18_0_6_21_459 "rlcul: egprs tx blk mcs%d bsn = %d, va = %d, vs = %d"
#define RLC_UPLINK_4104_112_2_18_0_6_21_460 "rlcul: egprs retx %s blk mcs%d bsn = %d, va = %d, vs = %d"
#define RLC_UPLINK_4126_112_2_18_0_6_21_461 "rlcul: egprs retx %s blk mcs%d bsn = %d, va = %d, vs = %d"
#define RLC_UPLINK_4132_112_2_18_0_6_21_462 "rlcul: egprs tx blk mcs%d bsn = %d, va = %d, vs = %d"
#define RLC_UPLINK_4351_112_2_18_0_6_22_463 "rlcul: in RLCUP_ProcessAckUnackMsg sendnumcnf"
#define RLC_UPLINK_4547_112_2_18_0_6_22_464 "rlcul: decode crbb error!\n"
#define RLC_UPLINK_4678_112_2_18_0_6_23_465 "rlcul: in RLCUP_EGPRS_ProcessAckNackMsg sendnumcnf"
#define RLC_UPLINK_5329_112_2_18_0_6_24_466 "rlcul: in change tlli sendnumcnf"
#define RLC_UPLINK_5544_112_2_18_0_6_24_467 "rlcul: in change tlli sendnumcnf"
#define RLC_UPLINK_6023_112_2_18_0_6_25_468 "rlcul:Error!unkown tlli_present!"
#define RLC_UPLINK_6042_112_2_18_0_6_25_469 "rlcul: in change tlli sendnumcnf"
#define RLC_UPLINK_6077_112_2_18_0_6_25_470 "rlcul: update timeslot mismatch! Error"
#define RLC_UPLINK_6261_112_2_18_0_6_26_471 "rlcul: set flow ctrl, llc num = %d, spare num = %d, line = %d"
#define RLC_UPLINK_6274_112_2_18_0_6_26_472 "rlcul: set flow ctrl, octet num = %d, line = %d"
#define RLC_UPLINK_6300_112_2_18_0_6_26_473 "rlcul: reset flow ctrl, line = %d"
#define RR2RRC_PRIM_148_112_2_18_0_6_57_474 "DM: handle ho cmd , message error"
#define RR2RRC_PRIM_154_112_2_18_0_6_57_475 "DM: handle ho cmd , PD or msg-type ERROR"
#define RR2RRC_PRIM_160_112_2_18_0_6_57_476 "DM: call decode function to decode ho cmd"
#define RR2RRC_PRIM_194_112_2_18_0_6_57_477 "TRR has sent msg %s to GSM. multi_sys=%d."
#define RR2RRC_PRIM_233_112_2_18_0_6_58_478 "Invalid rr context pointer."
#define RR2RRC_PRIM_241_112_2_18_0_6_58_479 "RRDM_Handle_Ccofu_callback: ASSERT! gsm msg num is too large = %d"
#define RR2RRC_PRIM_246_112_2_18_0_6_58_480 "CALLBACK Rev RAT CHG MSG , type = %d"
#define RR2RRC_PRIM_313_112_2_18_0_6_58_481 "Invalid rr context pointer."
#define RR2RRC_PRIM_322_112_2_18_0_6_58_482 "Trr_handle_rej_to_2G_callback: ho_type is error, ho_type = %d"
#define RR2RRC_PRIM_376_112_2_18_0_6_58_483 "Trr_handle_ho_to_gsm_callback:Invalid rr context pointer."
#define RR2RRC_PRIM_391_112_2_18_0_6_58_484 "RRDM: TD Handover To GSM start."
#define RR2RRC_PRIM_488_112_2_18_0_6_58_485 "inter RAT HO from UTRAN failed for msg error, msg_error =%d"
#define RR2RRC_PRIM_520_112_2_18_0_6_58_486 "Trr_arfcn_in_gsmr_band:Invalid rr context pointer."
#define RR2RRC_PRIM_528_112_2_18_0_6_58_487 "Trr_arfcn_in_gsmr_band: this arfcn is GSM-R band,multi_sys=%d,arfcn=%d"
#define RR_CSN_MSG_4514_112_2_18_0_7_7_488 "RRC:received System Information 9, not supported now!"
#define RR_CSN_MSG_4538_112_2_18_0_7_7_489 "RRC:received System Information 16, not supported now!"
#define RR_CSN_MSG_4548_112_2_18_0_7_7_490 "RRC:received System Information 17, not supported now!"
#define RR_CSN_MSG_4558_112_2_18_0_7_7_491 "RRC:received System Information 18, not supported now!"
#define RR_CSN_MSG_4568_112_2_18_0_7_7_492 "RRC:received System Information 19, not supported now!"
#define RR_CSN_MSG_4579_112_2_18_0_7_7_493 "RRC:received System Information 20, not supported now!"
#define RR_CSN_MSG_6754_112_2_18_0_7_12_494 "RRC: 2 quater,  index %d, count %d"
#define RR_CSN_MSG_6865_112_2_18_0_7_12_495 "rrc: no Optional Selection Parameters in Si4RestOctets, line = %d"
#define RR_CSN_MSG_6928_112_2_18_0_7_12_496 "RRC: si2quater available"
#define RR_CSN_MSG_6953_112_2_18_0_7_12_497 "rrc: no Optional Selection Parameters in Si4RestOctets, line = %d"
#define RR_CSN_MSG_6971_112_2_18_0_7_12_498 "RRC:receive LSA connent in System Information 8,not support!line=%d"
#define RR_CSN_MSG_7091_112_2_18_0_7_12_499 "RRC: SGSNR %d"
#define RR_CSN_MSG_7101_112_2_18_0_7_12_500 "RRC: si_status_ind %d"
#define RR_ENG_98_112_2_18_0_7_13_501 "RRENG_Init: g_dm_rr_context[%d] has been already allocated."
#define RR_ENG_168_112_2_18_0_7_13_502 "RRENG_SetStackCfgParam: multi_sys=%d,params_ptr is NULL."
#define RR_ENG_208_112_2_18_0_7_14_503 "RRENG_GetStackCfgParam: multi_sys=%d,gsm_cfg_param_ptr is NULL."
#define RR_ENG_242_112_2_18_0_7_14_504 "RRENG_GetStackCfgParam:multi_sys=%d, %d, %d, %d ,%d, %d, %d,%d,%d,%d,%d,%d,%d,%d"
#define RR_ENG_281_112_2_18_0_7_14_505 "RRENG_ResetGSMStackCfgParam:multi_sys=%d, %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"
#define RR_ENG_296_112_2_18_0_7_14_506 "RRENG_SetDMParam:multi_sys=%d,index=%d,value=%d"
#define RR_ENG_343_112_2_18_0_7_14_507 "RRENG_SetDMParam: pinp guard is stopped."
#define RR_ENG_350_112_2_18_0_7_14_508 "RRENG_SetDMParam: recovery as default value:offset_step_rate=2,init_pinp_offset=3."
#define RR_ENG_414_112_2_18_0_7_14_509 "RRENG_SetDMParam: not support yet"
#define RR_ENG_433_112_2_18_0_7_14_510 "RRENG_GetDMParam,multi_sys=%d, valu_ptr is NULL."
#define RR_ENG_477_112_2_18_0_7_14_511 "RRENG_GetDMParam: not support yet. multi_sys=%d"
#define RR_ENG_481_112_2_18_0_7_14_512 "RRENG_GetDMParam:multi_sys=%d,index=%d,value=%d"
#define RR_ENG_525_112_2_18_0_7_14_513 "RRENG_SetTDMeasSwitch: utran_meas_switch = %d"
#define RR_ENG_550_112_2_18_0_7_14_514 "RRENG_GetTDMeasSwitch: utran_meas_switch = %d"
#define RR_ENG_629_112_2_18_0_7_14_515 "RRENG_ReadTdNcellFromNV:multi_sys=%d, td_ncell_list_ptr->arfcn_num = %d"
#define RR_ENG_660_112_2_18_0_7_14_516 "RRENG_SetDummyTdNcell:multi_sys=%d"
#define RR_ENG_663_112_2_18_0_7_14_517 "RRENG_SetDummyTdNcell:ERROR->index value is wrong,maximum value is 9!"
#define RR_ENG_668_112_2_18_0_7_14_518 "RRENG_SetDummyTdNcell:ERROR->index value should be set in succession!"
#define RR_ENG_673_112_2_18_0_7_14_519 "RRENG_SetDummyTdNcell:ERROR->cell_param_num value is wrong,maximum value is 4!"
#define RR_ENG_678_112_2_18_0_7_14_520 "RRENG_SetDummyTdNcell:ERROR->uarfcn value is wrong,minimum value is 10054!"
#define RR_ENG_685_112_2_18_0_7_14_521 "RRENG_SetDummyTdNcell:INVALID UARFCN = %d from dummy"
#define RR_ENG_701_112_2_18_0_7_15_522 "RRENG_SetDummyTdNcell:added UARFCN, index=%d"
#define RR_ENG_703_112_2_18_0_7_15_523 "RRENG_SetDummyTdNcell:cell_id_num=%d,cell_id1=%d,cell_id2=%d,cell_id3=%d,cell_id4=%d"
#define RR_ENG_707_112_2_18_0_7_15_524 "RRENG_SetDummyTdNcell:updated UARFCN, index=%d"
#define RR_ENG_709_112_2_18_0_7_15_525 "RRENG_SetDummyTdNcell:cell_id_num=%d,cell_id1=%d,cell_id2=%d,cell_id3=%d,cell_id4=%d"
#define RR_ENG_738_112_2_18_0_7_15_526 "RRENG_ResetDummyTdNcell: multi_sys=%d,DUMMY TDD NCELL is reset."
#define RR_ENG_777_112_2_18_0_7_15_527 "RRENG_SetG2URevertStatus: multi_sys=%d,invalid type:%d"
#define RR_ENG_818_112_2_18_0_7_15_528 "RRENG_GetGsm2UtranRevertStatus: multi_sys=%d,invalid type:%d"
#define RR_ENG_908_112_2_18_0_7_15_529 "RRENG_SetDummyPCCOTddCell: uarfcn=%d,cpi=%d"
#define RR_ENG_1093_112_2_18_0_7_15_530 "RRENG_Set_PowerSweep_IsUsed_Ba:usedBa=%d power_sweep_used_ba = %d,power_sweep_unused_ba=%d"
#define RR_ENG_1123_112_2_18_0_7_15_531 "RRENG_Set_PowerSweep_UseBa_IsCamp:power_sweep_used_ba_camp_suc_amount=%d,sweep_used_ba_camp_fail_amount=%d"
#define RR_ENG_1141_112_2_18_0_7_15_532 "RRENG_Set_T2G_Resel_Fail_No_Revert: T2G_resel_fail_no_revert_amount = %d"
#define RR_ENG_1160_112_2_18_0_7_15_533 "RRENG_Set_T2G_Resel_No_Revert: T2G_resel_no_revert_total_amount = %d"
#define RR_ENG_1201_112_2_18_0_7_16_534 "weixia:G2G_resel_fail_amount=%d,G2G_resel_total_amount=%d,G2G_resel_succ_amount=%d"
#define RR_ENG_1316_112_2_18_0_7_16_535 "RRENG_Add_TdNcell_ENG_PARAMS: eng td ncell list is full."
#define RR_ENG_1349_112_2_18_0_7_16_536 "RRENG: nv_item_ptr is null!!"
#define RR_ENG_1391_112_2_18_0_7_16_537 "RRENG: si_2quarter_test[012] = %x%x%x"
#define RR_ENG_1421_112_2_18_0_7_16_538 "RRENG_GetSiUpdateTimeDur: active_card_num = %d, extra_dur_one_card = %d,time_dur_ptr =%d"
#define RRCELL_1756_112_2_18_0_7_25_539 "RRCELL_ReadSelParamInfo: multi_sys=%d invalid"
#define RRCELL_1772_112_2_18_0_7_26_540 "RRCELL_ReadEplmnInfo: multi_sys=%d invalid"
#define RRCELL_1787_112_2_18_0_7_26_541 "RRCELL_ReadClassMark: multi_sys=%d invalid"
#define RRCELL_2146_112_2_18_0_7_26_542 "RRC: ref_sen %d, num %d"
#define RRCELL_2151_112_2_18_0_7_26_543 "RRC: ref_sen %d, num %d"
#define RRCELL_2156_112_2_18_0_7_26_544 "RRC: ref_sen is 0, num %d"
#define RRCELL_2250_112_2_18_0_7_26_545 "RRC: clean rxlev count %d"
#define RRCELL_2458_112_2_18_0_7_27_546 "RRC: set rxlev[%d] to 0"
#define RRCELL_2529_112_2_18_0_7_27_547 "RRC: In RRCELL_SortRssi, freq_count=%d"
#define RRCELL_2535_112_2_18_0_7_27_548 "RRC:ARFCN = 0x%x, band %d, rssi = %d\n"
#define RRCELL_2581_112_2_18_0_7_27_549 "RRC: %d set arfcn %d to 0"
#define RRCELL_2605_112_2_18_0_7_27_550 "RRC: %d set arfcn %d, index %d to 0"
#define RRCELL_2628_112_2_18_0_7_27_551 "RRC: %d set arfcn %d, index %d to 0"
#define RRCELL_2653_112_2_18_0_7_27_552 "RRC: %d set arfcn %d, index %d to 0"
#define RRCELL_2677_112_2_18_0_7_28_553 "RRC: %d set arfcn %d, index %d to 0"
#define RRCELL_2754_112_2_18_0_7_28_554 "RRC: %d clearC p %d a 0x%x"
#define RRCELL_2775_112_2_18_0_7_28_555 "RRC: %d clearRc p %d a 0x%x"
#define RRCELL_2897_112_2_18_0_7_28_556 "RRCELL_AdjustC1ByNV:Error, c1_ptr is NULL"
#define RRCELL_2905_112_2_18_0_7_28_557 "RRCELL_AdjustC1ByNV:Error, rr_dm_context_ptr is NULL"
#define RRCELL_3003_112_2_18_0_7_28_558 "RRC:In RRCELL_IsSuitableCell\n"
#define RRCELL_3004_112_2_18_0_7_28_559 "RRC: %d ARFCN = %d, c1 = %d\n"
#define RRCELL_3394_112_2_18_0_7_29_560 "RRC: %d In  RRCELL_IsAcceptableCell\n"
#define RRCELL_3552_112_2_18_0_7_29_561 "RRC: %d In UpdateNCellRssi"
#define RRCELL_3633_112_2_18_0_7_29_562 "RRC:RR NCELL: cell from store to temp, arfcn = %d, bsic = %d"
#define RRCELL_3720_112_2_18_0_7_30_563 "RRC:RR NCELL: cell from backup to store, arfcn = %d, bsic = %d"
#define RRCELL_3755_112_2_18_0_7_30_564 "RRC: NCELL cell from temp to backup, arfcn = %d, bsic = %d"
#define RRCELL_3796_112_2_18_0_7_30_565 "RRC: RRCELL_UpdateNcellSi has NULL pointer param!!"
#define RRCELL_4077_112_2_18_0_7_30_566 "RRC: arfcn %d, si7_not_present %d, "
#define RRCELL_4085_112_2_18_0_7_30_567 "RRC: arfcn %d, si8_not_present %d, "
#define RRCELL_4581_112_2_18_0_7_31_568 "RR_FindSuitableCellInSelectedPlmn:c2 > INVALID_C2 return TRUE in dual mode"
#define RRCELL_4610_112_2_18_0_7_31_569 "RR_FindSuitableCellInSelectedPlmn:c2 <= INVALID_C2 return TRUE in dual mode"
#define RRCELL_4675_112_2_18_0_7_31_570 "RR_FindSuitableCellInSelectedPlmn_C1_Low: return TRUE in dual mode."
#define RRCELL_5260_112_2_18_0_7_33_571 "RRC: assert unknown si %d"
#define RRCELL_5300_112_2_18_0_7_33_572 "RRC: %d checkncellSi, cur %d"
#define RRCELL_5360_112_2_18_0_7_33_573 "RRCELL_CheckNcellSysInfo: RRC: %d checkncellSi, another_rr_status = %d, si_update_dur = %d, NCELL_SI_UPDATE_PERIOD = %d"
#define RRCELL_5538_112_2_18_0_7_33_574 "RRCELL_CheckNcellSysInfo: num_of_ncell_present = %d"
#define RRCELL_5542_112_2_18_0_7_33_575 "RRC: cell %d, arfcn 0x%x, tc 0x%x new_tc 0x%x"
#define RRCELL_5705_112_2_18_0_7_34_576 "RRC: GetReselPriority cell_ptr->cell_prio = %d "
#define RRCELL_5719_112_2_18_0_7_34_577 "RRC: %d reselprior %d arfcn 0x%x c1 %d"
#define RRCELL_5723_112_2_18_0_7_34_578 "RRC: %d cell prio NO_CELL arfcn = %d, c1 = %d\n"
#define RRCELL_5832_112_2_18_0_7_34_579 "RRC:In RRCELL_UpdateBcchSweepResult\n"
#define RRCELL_5833_112_2_18_0_7_34_580 "RRC:ARFCN = %d, rssi = %d \n"
#define RRCELL_5973_112_2_18_0_7_34_581 "RRCELL: (SCI_GetTickCount()-assert_tick)>300000,ERROR"
#define RRCELL_6042_112_2_18_0_7_34_582 "RRC:%d In ReselTimer\n"
#define RRCELL_6043_112_2_18_0_7_34_583 "RRC:A = 0x%x, rssi = %d, c1 = %d, c2 = %d\n"
#define RRCELL_6047_112_2_18_0_7_34_584 "RRC:A = 0x%x, rssi = %d, c1 = %d, c2 = %d\n"
#define RRCELL_6060_112_2_18_0_7_34_585 "RRC resel: card %d, scell: a=0x%x,c1=%d,c2=%d; max rssi ncell: a=0x%x,c1=%d,c2=%d; ct=%d"
#define RRCELL_6091_112_2_18_0_7_34_586 "RRCELL: RR_IsAdditions, current bit is not 1."
#define RRCELL_6095_112_2_18_0_7_34_587 "RRCELL: RR_IsAdditions, offset_octets=%d > msg_length=%d."
#define RRCELL_6411_112_2_18_0_7_35_588 "RRCELL:RR_DecodeGPRSRepPriorityDescSt, ignored the last rep priority."
#define RRCELL_6576_112_2_18_0_7_35_589 "RR_DecodeGPRS3GMeasParamsDesc:search_p=%d,earch_prio_3g=%d,multirat_rep_present:%d,multirat_rep:%d,rep_off_and_thres_present:%d,rep_offset:%d,rep_threshold:%d"
#define RRCELL_6657_112_2_18_0_7_35_590 "RR_DecodeGPRS3GMeasParamsBisDesc:search_p=%d,earch_prio_3g=%d,multirat_rep_present:%d,multirat_rep:%d,n rep_off_and_thres_present:%d,rep_offset:%d,rep_threshold:%d"
#define RRCELL_6704_112_2_18_0_7_36_591 "RR_DecodeRemoved3GCellDesc, not save, and continue to decode the next IE."
#define RRCELL_6707_112_2_18_0_7_36_592 "RR_DecodeRemoved3GCellDesc:removed_cell_index=%d,remv_cell_diff_value=%d."
#define RRCELL_6830_112_2_18_0_7_36_593 "RR_DecodePcco3GNeighbourCell: Index_Start_3G included"
#define RRCELL_6841_112_2_18_0_7_36_594 "RR_DecodePcco3GNeighbourCell: Index_Start_3G included"
#define RRCELL_6852_112_2_18_0_7_36_595 "RR_DecodePcco3GNeighbourCell: FDD neighbor cell is assigned  in pcco!\n"
#define RRCELL_6860_112_2_18_0_7_36_596 "RR_DecodePcco3GNeighbourCell: UTRAN TDD Description included"
#define RRCELL_6909_112_2_18_0_7_36_597 "RR_DecodePMO3GNeighbourCell: Index_Start_3G included.offset_octets=%d,offset_bits=%d"
#define RRCELL_6920_112_2_18_0_7_36_598 "RR_DecodePMO3GNeighbourCell: Index_Start_3G included.offset_octets=%d,offset_bits=%d"
#define RRCELL_6931_112_2_18_0_7_36_599 "RR_DecodePMO3GNeighbourCell: FDD neighbor cell is assigned  in pcco!offset_octets=%d,offset_bits=%d"
#define RRCELL_6939_112_2_18_0_7_36_600 "RR_DecodePMO3GNeighbourCell: UTRAN TDD Description included.offset_octets=%d,offset_bits=%d"
#define RRCELL_6947_112_2_18_0_7_36_601 "RR_DecodePMO3GNeighbourCell: CDMA2000 Description included.offset_octets=%d,offset_bits=%d"
#define RRCELL_6955_112_2_18_0_7_36_602 "RR_DecodePMO3GNeighbourCell: REMOVED 3GCELL Description included.offset_octets=%d,offset_bits=%d"
#define RRCELL_7010_112_2_18_0_7_36_603 "RR_DecodeENHMeasParam, ENH Measurement parameters included. offset_octets=%d,offset_bits=%d"
#define RRCELL_7046_112_2_18_0_7_36_604 "RR_DecodeENHMeasParam, pcco 3G Neighbour Cell Description included.offset_octets=%d,offset_bits=%d"
#define RRCELL_7051_112_2_18_0_7_36_605 "RR_DecodeENHMeasParam,pmo 3G Neighbour Cell Description included.offset_octets=%d,offset_bits=%d"
#define RRCELL_7079_112_2_18_0_7_36_606 "RR_DecodeENHMeasParam, GPRS 3G MEASUREMENT Parameters  included in PCCO. offset_octets=%d,offset_bits=%d"
#define RRCELL_7089_112_2_18_0_7_36_607 "RR_DecodeENHMeasParam, GPRS 3G MEASUREMENT Parameters BIS desc  included in PMO.offset_octets=%d,offset_bits=%d"
#define RRCELL_8599_112_2_18_0_7_39_608 "Extension Information LENGTH =  %d"
#define RRCELL_8602_112_2_18_0_7_39_609 "RR_DecodeGprsCellOption:left length = %d"
#define RRCELL_8605_112_2_18_0_7_39_610 "EGPRS Information is included  in GPRS_CELL_OPTION IE"
#define RRCELL_8626_112_2_18_0_7_39_611 "EGPRS Information is not enough."
#define RRCELL_8653_112_2_18_0_7_39_612 "CCN_ACTIVE is %d  in GPRS_CELL_OPTION IE"
#define RRCELL_10560_112_2_18_0_7_43_613 "RR_DecodePmoMsgBlock: start..."
#define RRCELL_10593_112_2_18_0_7_43_614 "RR_DecodePmoMsgBlock: PCCO NC_MODE = %d"
#define RRCELL_10607_112_2_18_0_7_43_615 "RR_DecodePmoMsgBlock :EXT Measurement Parameters  included"
#define RRCELL_10784_112_2_18_0_7_44_616 "RRC:TLLI in PacketMeasureOrder is wrong!new_tiil=%d, old_tlli=%d, rec_tlli=%d"
#define RRCELL_10820_112_2_18_0_7_44_617 "RR_DecodePacketMeasureOrder:count:%d "
#define RRCELL_11446_112_2_18_0_7_45_618 "RR_DecodePacketCellChangeOrder start."
#define RRCELL_11470_112_2_18_0_7_45_619 "RRC:TLLI in PacketCellChangeOrder is wrong!new_tiil=%d, old_tlli=%d, rec_tlli=%d"
#define RRCELL_11497_112_2_18_0_7_45_620 "RR_DecodePacketCellChangeOrder: PCCO NC_MODE = %d"
#define RRCELL_11521_112_2_18_0_7_45_621 "RR_DecodePacketCellChangeOrder: 3g target cell included."
#define RRCELL_11531_112_2_18_0_7_45_622 "RR_DecodePacketCellChangeOrder: msg_length:%d,offset_octets:%d,offset_bits:%d"
#define RRCELL_11591_112_2_18_0_7_46_623 "RR_DecodePacketCellChangeOrder failure. msg_error=%d"
#define RRCELL_12257_112_2_18_0_7_47_624 "RRC: scell is barred cba2 = 1\n"
#define RRCELL_12266_112_2_18_0_7_47_625 "RRC: scell c1 <= 0\n"
#define RRCELL_12285_112_2_18_0_7_47_626 "RRC:In  RRCELL_ProcessReselTimerGprs\n"
#define RRCELL_12286_112_2_18_0_7_47_627 "RRC:ARFCN = %d, rssi = %d, c1 = %d, c31 = %d, c32 = %d\n"
#define RRCELL_12291_112_2_18_0_7_47_628 "RRC:ARFCN = %d, rssi = %d, c1 = %d, c31 = %d, c32 = %d\n"
#define RRCELL_12574_112_2_18_0_7_48_629 "RRCELL_GetNCMeasRepList: NC MEAS list is full."
#define RRCELL_12617_112_2_18_0_7_48_630 "RRCELL_Get3GMeasRepList: PCCO switch is disabled!!"
#define RRCELL_12632_112_2_18_0_7_48_631 "RRCELL_Get3GMeasRepList:cell_num=%d,TDD multirat_rep_present=%d,multirat_rep=%d."
#define RRCELL_12668_112_2_18_0_7_48_632 "RRCELL_Get3GMeasRepList: 3g target cell has been barred."
#define RRCELL_12684_112_2_18_0_7_48_633 "RRCELL_Get3GMeasRepList: cell_3g_list[%d]={index=%d,quantity=%d}."
#define RRCELL_12690_112_2_18_0_7_48_634 "RRCELL_Get3GMeasRepList: n_3g=%d."
#define RRCELL_12760_112_2_18_0_7_48_635 "RRCELL_ConfigPMRMessage: failure, nc order is not NC1/NC2"
#define RRCELL_12780_112_2_18_0_7_48_636 "RRCELL_ConfigPMRMessage:sgsnr=%d,revision_level=%d."
#define RRCELL_12855_112_2_18_0_7_48_637 "RRCELL_CalcNrOfNcMeasInPMR: offset_octets=%d,offset_bits=%d"
#define RRCELL_12891_112_2_18_0_7_48_638 "RRCELL_CalcNrOfNcMeasInPMR: new offset_octets=%d,offset_bits=%d"
#define RRCELL_13014_112_2_18_0_7_49_639 "RRCELL_EncodeNcMeasReport: INTERFERENCE_SERVING_CELL is included."
#define RRCELL_13025_112_2_18_0_7_49_640 "RRCELL_EncodeNcMeasReport: nr_nc_meas = %d."
#define RRCELL_13047_112_2_18_0_7_49_641 "RRCELL_EncodeNcMeasReport: EXT Measurement Report not support yet."
#define RRCELL_13100_112_2_18_0_7_49_642 "RRCELL_EncodeNcMeasReport: meas_3g_rep_present = %d."
#define RRCELL_13137_112_2_18_0_7_49_643 "RRCELL_EncodeNcMeasReport: encode failure."
#define RRCELL_13586_112_2_18_0_7_50_644 "RRC: cell reselection is controled by network\n"
#define RRCELL_13793_112_2_18_0_7_50_645 "RR_DecodePacketMeasureOrder failure. msg_error=%d"
#define RRCELL_14005_112_2_18_0_7_51_646 "RRC: Assert Failure, unknown extend report type."
#define RRCELL_14085_112_2_18_0_7_51_647 "isNcMeasReport=%d,dm_td_cells_info.cell_num=%d"
#define RRCELL_15273_112_2_18_0_7_53_648 "RRC: %d scell ba_ext %d, 2bis %d, tc5bad %d"
#define RRCELL_15274_112_2_18_0_7_53_649 "RRC: 2ter_ind %d, 2ter_p %d, tc4bad %d"
#define RRCELL_15377_112_2_18_0_7_53_650 "RRC: cell specified is not present!!"
#define RRCELL_15382_112_2_18_0_7_53_651 "RRC: param plmn_la_list is NULL!!"
#define RRCELL_15387_112_2_18_0_7_53_652 "RRC: si3 and si4 of cell specified are not received!!"
#define RRCELL_15392_112_2_18_0_7_53_653 "RRC: cell specified is barred!!"
#define RRCELL_15599_112_2_18_0_7_54_654 "RRC: one of pointer params is NULL."
#define RRCELL_15929_112_2_18_0_7_54_655 "RRC: NCELL cell to store, arfcn = %d, bsic = %d"
#define RRCELL_15945_112_2_18_0_7_54_656 "RRC: NCELL cell clear, arfcn = %d, bsic = %d"
#define RRCELL_16199_112_2_18_0_7_55_657 "RRC: %d FillBsic start %d, num %d, max %d"
#define RRCELL_16215_112_2_18_0_7_55_658 "RRC: %d bsic %d, a 0x%x, b %d"
#define RRCELL_16224_112_2_18_0_7_55_659 "RRC: %d ASSERT ERROR, no space when search"
#define RRCELL_16290_112_2_18_0_7_55_660 "RRC: %d research num %d"
#define RRCELL_16326_112_2_18_0_7_55_661 "RRC: %d g_ind %d, 13 %d, ba-ext %d, 2bis %d, 2t_ind %d, 2ter %d"
#define RRCELL_16378_112_2_18_0_7_55_662 "RRC: %d EssSiBad %d %d %d %d"
#define RRCELL_16556_112_2_18_0_7_56_663 "RRCELL_CleanUseless_Tc: not use tc5:%d, %d, %d"
#define RRCELL_16590_112_2_18_0_7_56_664 "RRCELL_CleanUseless_Tc: not use tc4:%d, %d, %d"
#define RRCELL_16604_112_2_18_0_7_56_665 "RRCELL_CleanUseless_Tc: si2_present=%d,si3_present=%d,si4_present=%d,si_2ter_ind=%d,gprs_ind_present=%d,si2quater_ind=%d,si2ter_not_use_tc4=%d,si13_not_use_tc4=%d"
#define RRCELL_16678_112_2_18_0_7_56_666 "RRC: assert error, exceptional exp"
#define RRCELL_16689_112_2_18_0_7_56_667 "RRC: assert error, exceptional exp"
#define RRCELL_16751_112_2_18_0_7_56_668 "RRC: %d FillBcch p %d, a 0x%x, tc 0x%x"
#define RRCELL_16797_112_2_18_0_7_56_669 "RRCELL_FillBcchReqInfo_ForCamp: si3_present=%d,si_2ter_ind=%d,si4_acs=%d,si2quater_ind=%d"
#define RRCELL_16822_112_2_18_0_7_56_670 "RRC: %d FillBcch old_tc 0x%x, new 0x%x"
#define RRCELL_16914_112_2_18_0_7_56_671 "RRCELL_FillBcchReqInfo_Si2quater:si2quater_position=%d,ba_2_ext_ind=%d,si_2ter_ind=%d"
#define RRCELL_16931_112_2_18_0_7_56_672 "RRCELL_FillBcchReqInfo_Si2quater: old tc=0x%x,2quater tc=0x%x"
#define RRCELL_17030_112_2_18_0_7_57_673 "RRC: %d UpdateBadSi tc 0x%x, pos %d"
#define RRCELL_17049_112_2_18_0_7_57_674 "RRC: %d bad si tomax, cell a 0x%x, fn 0x%x"
#define RRCELL_17124_112_2_18_0_7_57_675 "RRC: %d BadSiTO MAX tc 0x%x, pos %d"
#define RRCELL_17240_112_2_18_0_7_57_676 "RRC: assert error, illogical si, tc %d, pos %d, fn %d"
#define RRCELL_17276_112_2_18_0_7_57_677 "RRC: %d gprs_ind %d, si13 %d, ba-ext %d, si2bis %d, 2ter_ind %d, 2ter %d"
#define RRCELL_17323_112_2_18_0_7_57_678 "RRC:%d ALL SI OK"
#define RRCELL_17328_112_2_18_0_7_57_679 "RRC:%d cell exp, set allneedsi to true"
#define RRCELL_17349_112_2_18_0_7_57_680 "RRC: %d IsEssSiOk T, a 0x%x"
#define RRCELL_17354_112_2_18_0_7_57_681 "RRC: %d, a 0x%x, p%d,%d %d %d %d"
#define RRCELL_17376_112_2_18_0_7_57_682 "RRC: %d MaxRssi, %d not max"
#define RRCELL_17380_112_2_18_0_7_57_683 "RRC: %d MaxRssi %d is max"
#define RRCELL_17407_112_2_18_0_7_57_684 "RRC: %d %d not bestcell"
#define RRCELL_17412_112_2_18_0_7_57_685 "RRC: %d %d is bestcell"
#define RRCELL_17441_112_2_18_0_7_57_686 "RRC: %d %d not bestncell"
#define RRCELL_17446_112_2_18_0_7_57_687 "RRC: %d %d is bestncell"
#define RRCELL_17469_112_2_18_0_7_57_688 "RRC: %d PlmnSearch, pos %d is suit to camp"
#define RRCELL_17479_112_2_18_0_7_57_689 "RRC: cell[%d] p %d"
#define RRCELL_17494_112_2_18_0_7_57_690 "RRC: %d PlmnEnd? s %d, num %d, end %d"
#define RRCELL_17513_112_2_18_0_7_57_691 "RRC: Plmn search end s %d,num %d,before camp."
#define RRCELL_17525_112_2_18_0_7_57_692 "RRC: Plmn search end, i %d, bsic_exist %d,si3_p %d,si4_p %d"
#define RRCELL_17592_112_2_18_0_7_58_693 "RRC: %d plmn research a=%d,b=%d"
#define RRCELL_17596_112_2_18_0_7_58_694 "RRC: %d ASSERT ERROR, no space when plmn research"
#define RRCELL_17654_112_2_18_0_7_58_695 "RRC: %d SelBestCell"
#define RRCELL_17658_112_2_18_0_7_58_696 "RRC: cell[%d] p %d, rssi %d, pri %d"
#define RRCELL_17697_112_2_18_0_7_58_697 "RRC: SelBestHCell sel %d"
#define RRCELL_17704_112_2_18_0_7_58_698 "RRC: SelBestRCell sel %d"
#define RRCELL_17711_112_2_18_0_7_58_699 "RRC: SelBestCell sel %d"
#define RRCELL_17762_112_2_18_0_7_58_700 "RRC: SelBestHCell sel %d"
#define RRCELL_17769_112_2_18_0_7_58_701 "RRC: SelBestRCell sel %d"
#define RRCELL_17776_112_2_18_0_7_58_702 "RRC: SelBestCell sel %d"
#define RRCELL_17799_112_2_18_0_7_58_703 "RRC: SelBestCell sel %d"
#define RRCELL_17840_112_2_18_0_7_58_704 "RRC: keep cell %d, a 0x %x"
#define RRCELL_17880_112_2_18_0_7_58_705 "%s"
#define RRCELL_17885_112_2_18_0_7_58_706 "%s"
#define RRCELL_17979_112_2_18_0_7_58_707 "RRC: %d CleanExp"
#define RRCELL_17984_112_2_18_0_7_58_708 "C[%d] p %d, a 0x%x, tc 0x%x, du %d, si %d, %d, %d, %d"
#define RRCELL_17990_112_2_18_0_7_58_709 "RRC: assert, bsic not ret dur 10s"
#define RRCELL_18119_112_2_18_0_7_59_710 "RRC: %d SET a 0x%x to bad %d"
#define RRCELL_18137_112_2_18_0_7_59_711 "RRC: %d, a 0x%x is bad"
#define RRCELL_18163_112_2_18_0_7_59_712 "RRC: %d get free rcell, %d,%d,%d,%d,%d,%d"
#define RRCELL_18241_112_2_18_0_7_59_713 "RRC: %d ASSERT ERROR, no space when search"
#define RRCELL_18441_112_2_18_0_7_59_714 "RRC: %d tc_mask_2 0x%x, rcell tc 0x%x"
#define RRCELL_18628_112_2_18_0_8_0_715 "RRC: %d plmnscan end s %d, num %d"
#define RRCELL_18650_112_2_18_0_8_0_716 "RRC: %d plmnend %d"
#define RRCELL_18881_112_2_18_0_8_0_717 "RRC: assert error bsic>search num"
#define RRCELL_18927_112_2_18_0_8_0_718 "RRC: %d, a 0x%x, p %d"
#define RRCELL_19006_112_2_18_0_8_0_719 "RRC: assert error bcch>search num"
#define RRCELL_19023_112_2_18_0_8_0_720 "RRC: search_num=%d,decode_num=%d"
#define RRCELL_19082_112_2_18_0_8_1_721 "RRC: assert error bcch>search num"
#define RRCELL_19117_112_2_18_0_8_1_722 "RRC: %d FillBcch fast p %d, a 0x%x, tc 0x%x"
#define RRCELL_19147_112_2_18_0_8_1_723 "RRC: %d FillBcch old_tc 0x%x, new 0x%x"
#define RRCELL_19344_112_2_18_0_8_1_724 "RRC: %d fast sel %d a 0x%x"
#define RRCELL_19347_112_2_18_0_8_1_725 "RRC: %d fast nocell"
#define RRCELL_19376_112_2_18_0_8_1_726 "RRC: %d C1 All Low %d"
#define RRCELL_19808_112_2_18_0_8_2_727 "NACC: SCELL C1<0"
#define RRCELL_19814_112_2_18_0_8_2_728 "NACC: VALID PMO %d"
#define RRCELL_19817_112_2_18_0_8_2_729 "NACC: IN NC2"
#define RRCELL_19823_112_2_18_0_8_2_730 "NACC: PMO ccn_active %d"
#define RRCELL_19834_112_2_18_0_8_2_731 "NACC: PMO TARGET CCN DEFAULT SUPPORT"
#define RRCELL_19837_112_2_18_0_8_2_732 "NACC: PMO target ccn support %d"
#define RRCELL_19849_112_2_18_0_8_2_733 "NACC: PMO TARGET CCN DEFAULT SUPPORT"
#define RRCELL_19865_112_2_18_0_8_2_734 "NACC: CCN ACTIVE: %d"
#define RRCELL_19878_112_2_18_0_8_2_735 "NACC: TARGET CCN DEFAULT SUPPORT"
#define RRCELL_19881_112_2_18_0_8_2_736 "NACC: TARGET CCN SUPPORT: %d"
#define RRCELL_19893_112_2_18_0_8_2_737 "NACC: TARGET CCN DEFAULT SUPPORT"
#define RRCELL_19958_112_2_18_0_8_2_738 "NACC: PNCD container_id %d, idx %d"
#define RRCELL_19965_112_2_18_0_8_2_739 "NACC: PNCD arfcn 0x%x, bsic 0x%x"
#define RRCELL_20019_112_2_18_0_8_3_740 "NACC: buff_idx %d, saved si: 0x%x, 0x%x"
#define RRCELL_20069_112_2_18_0_8_3_741 "NACC: PCCC arfcn 0x%x bsic 0x%x container_id %d"
#define RRCELL_20134_112_2_18_0_8_3_742 "NACC: PCCO container_id %d"
#define RRCELL_20145_112_2_18_0_8_3_743 "NACC: PCCO arfcn 0x%x, bsic 0x%x"
#define RRCELL_20198_112_2_18_0_8_3_744 "NACC: saved si num %d"
#define RRCELL_20209_112_2_18_0_8_3_745 "NACC: cd_len %d"
#define RRCELL_20213_112_2_18_0_8_3_746 "NACC: last_pncd_data.container_contiue %d"
#define RRCELL_20218_112_2_18_0_8_3_747 "NACC: reassemble si error, discard!"
#define RRCELL_20225_112_2_18_0_8_3_748 "NACC: REASSEMBEL MSG %x, %x; %x, %x"
#define RRCELL_20235_112_2_18_0_8_3_749 "NACC: DECODE PNCD si %d"
#define RRCELL_20242_112_2_18_0_8_3_750 "NACC: DECODE PNCD psi %d"
#define RRCELL_20272_112_2_18_0_8_3_751 "NACC: DECODE PNCD si %d"
#define RRCELL_20279_112_2_18_0_8_3_752 "NACC: DECODE PNCD psi %d"
#define RRCELL_20295_112_2_18_0_8_3_753 "NACC: buffer_len %d, offset_byte %d"
#define RRCELL_20318_112_2_18_0_8_3_754 "NACC: TARGET CELL PSI1 %d PSI2 %d"
#define RRCELL_20323_112_2_18_0_8_3_755 "NACC: TARGET CELL SI1_EXIST %d SI1 %d SI3 %d SI13 %d"
#define RRCELL_20343_112_2_18_0_8_3_756 "NACC: pbcch %d, psi status support %d si status support %d"
#define RRCELL_20719_112_2_18_0_8_4_757 "NACC: reassemble si error, discard!"
#define RRCELL_20747_112_2_18_0_8_4_758 "NACC: DECODE PSCD si error %d, type %d"
#define RRCELL_20752_112_2_18_0_8_4_759 "NACC: DECODE PSCD psi error %d, type %d"
#define RRCELL_20780_112_2_18_0_8_4_760 "NACC: DECODE PSCD si %d"
#define RRCELL_20785_112_2_18_0_8_4_761 "NACC: DECODE PSCD psi %d"
#define RRCELL_21662_112_2_18_0_8_6_762 "RRCELL_IsNC2Valid:nc = %d in PMO is used,gmm_state=%d."
#define RRCELL_21682_112_2_18_0_8_6_763 "RRCELL_IsNC2Valid:nc = %d in SI is used,gmm_state=%d."
#define RRCELL_21700_112_2_18_0_8_6_764 "RRCELL_IsNC2Valid:cell_present=%d,pbcch_present=%d"
#define RRCELL_21848_112_2_18_0_8_6_765 "RRCELL_FillBsicSearchList_FastPlmn:multisys=%d,scan_num:%d"
#define RRCELL_21889_112_2_18_0_8_6_766 "RRCELL_SetGprsSupportStatus: rr_service_state:%d scell lai:(%d %d %d %d),old lai:(%d %d %d %d)"
#define RRCELL_21906_112_2_18_0_8_6_767 "RRCELL_SetGprsSupportStatus: mm_gprs_supported:%d"
#define RRCELL_21926_112_2_18_0_8_6_768 "RRSIG: Warning, Invalid input number=%d of bsic!!"
#define RRCELL_21965_112_2_18_0_8_6_769 "RRSIG: Warning, Invalid input number=%d of arfcn!!"
#define RRCELL_22041_112_2_18_0_8_7_770 "RRCELL: DecodeACellBcch_ForCamp, the cell is not present!"
#define RRCELL_22066_112_2_18_0_8_7_771 "RRCELL: DecodeACellBsic, the cell is not present!"
#define RRCELL_22094_112_2_18_0_8_7_772 "RRCELL: ClearACellTc, not happened, %d,%d"
#define RRDM_171_112_2_18_0_8_7_773 "RRDM_IsUtranNcellFreq:no arfcn exist in TD CELL LIST"
#define RRDM_185_112_2_18_0_8_7_774 "RRDM_IsUtranNcellFreq:tdd cell arfcn=%d NOT exist in TD CELL LIST"
#define RRDM_214_112_2_18_0_8_7_775 "RRDM: RRDM_GetBestTdCell, immediate_resel, cell_num= %d"
#define RRDM_390_112_2_18_0_8_8_776 "RRDM: reset the dm_td_cells_barred_info"
#define RRDM_408_112_2_18_0_8_8_777 "RRDM: add the new barred cell: ARFCN = %d,cell_param_id = %d, cell_barred_time = %d, rx_cell_barred_time = %d, td_bar_num = %d"
#define RRDM_518_112_2_18_0_8_8_778 "RRCELL:RRCELL_AddReselCrt, resel crt array is full."
#define RRDM_529_112_2_18_0_8_8_779 "RRCELL:RRCELL_AddReselCrt, invalid 3g ncell index."
#define RRDM_535_112_2_18_0_8_8_780 "RRCELL:RRCELL_AddReselCrt, INVALID UARFCN = %d"
#define RRDM_577_112_2_18_0_8_8_781 "RRCELL_RemoveReselCrt, it has been removed."
#define RRDM_598_112_2_18_0_8_8_782 "RRCELL_RemoveReselCrt, invalid array index=%d >= cell_num=%d"
#define RRDM_622_112_2_18_0_8_8_783 "RRCELL: RRCELL_ModifyReselCrt,warning,this item is not present."
#define RRDM_679_112_2_18_0_8_8_784 "RRDM_FindReselCrtByIndex, error, the item is not present."
#define RRDM_716_112_2_18_0_8_9_785 "RRDM_FindReselCrtByArfcnAndCpi, error, the item is not present."
#define RRDM_739_112_2_18_0_8_9_786 "RRCELL_IsNeedToReplaceReselCrt, invalid index value=%d."
#define RRDM_753_112_2_18_0_8_9_787 "RRCELL_IsNeedToReplaceReselCrt, find, but index,arfn,cell_param_id are the same."
#define RRDM_777_112_2_18_0_8_9_788 "RRCELL_IsNeedToReplaceReselCrt, find, but index is not the highest."
#define RRDM_806_112_2_18_0_8_9_789 "RRDM_AddNcell3g, 3g ncells array is full."
#define RRDM_817_112_2_18_0_8_9_790 "RRDM_AddNcell3g, invalid 3g ncell index=%d."
#define RRDM_823_112_2_18_0_8_9_791 "RRDM_AddNcell3g, INVALID UARFCN = %d"
#define RRDM_904_112_2_18_0_8_9_792 "RRDM_RemoveNcell3g, invalid array index=%d >= ncells_3g_num=%d"
#define RRDM_1013_112_2_18_0_8_9_793 "RRDM_IsNeedToReplaceNcell3gArr, invalid index value=%d."
#define RRDM_1027_112_2_18_0_8_9_794 "RRDM_IsNeedToReplaceNcell3gArr, find, but index,arfn,cell_param_id are the same."
#define RRDM_1051_112_2_18_0_8_9_795 "RRDM_IsNeedToReplaceNcell3gArr, find, but index is not the highest."
#define RRDM_1115_112_2_18_0_8_9_796 "RRDM_FillEplmn: p_as_common_info or g_rrc_eplmn_list is NULL."
#define RRDM_1123_112_2_18_0_8_9_797 "RRDM_FillEplmn: warning, bigger plmns number:%d"
#define RRDM_1159_112_2_18_0_8_9_798 "RRCELL_UpdateReselCrtArrByMeasRep:sync_type=%d, nt td_meas_type=%d, nt meas_rep->cell_num=%d,nt meas_rep->arfcn_num=%d nt resel_crts->cell_num=%d"
#define RRDM_1258_112_2_18_0_8_10_799 "RRCELL_UpdateMeasRepToReselCrtArr:TD_INFO_L1_TEST_MODE in td_meas_report message, do nothing!!!"
#define RRDM_1262_112_2_18_0_8_10_800 "RRCELL_UpdateReselCrtArrByMeasRep:TD_MEAS_TYPE ERROR in td_meas_report message!!!"
#define RRDM_1267_112_2_18_0_8_10_801 "RRCELL_UpdateReselCrtArrByMeasRep, resel criteria cell num=%d"
#define RRDM_1285_112_2_18_0_8_10_802 "RRDM_FreeTrrMsg: invalid msg pointer!"
#define RRDM_1382_112_2_18_0_8_10_803 "RRDM_FillDummyUtranMeasParams: NULL eng params pointer!!"
#define RRDM_1391_112_2_18_0_8_10_804 "RRDM_FillDummyUtranMeasParams, qsearch_x is set as 7 by NV."
#define RRDM_1402_112_2_18_0_8_10_805 "RRDM_FillDummyUtranMeasParams:TDDO is running,tdd_offset is adjusted by tddo_tdd_offset of NV. tddo_tdd_offset=%d"
#define RRDM_1417_112_2_18_0_8_10_806 "RRDM_FillDummyUtranMeasParams:ASSERT!WRONG g_gsm_stack_cfg_params.q_offset_table_switch = %d"
#define RRDM_1427_112_2_18_0_8_10_807 "RRDM_FillDummyUtranMeasParams: qsearch_x is set as %d by AT CMD."
#define RRDM_1446_112_2_18_0_8_10_808 "RRDM_FillDummyUtranMeasParams: tdd offset=%d is set by AT cmd."
#define RRDM_1518_112_2_18_0_8_10_809 "RRDM_FillTddNcellsToReselCrtArr: find_result=%d,index=%d,arfcn=%d,cpi=%d,tdd_cell_info=%d"
#define RRDM_1532_112_2_18_0_8_10_810 "RRDM_FillTddNcellsToReselCrtArr, INVALID UARFCN = %d"
#define RRDM_1586_112_2_18_0_8_10_811 "RRDM_RemoveTddNcellsBy3gIndex: removed 3g ncell(meas reported) related to 3g index=%d"
#define RRDM_1638_112_2_18_0_8_10_812 "RRDM_CheckQsearch: dummy td meas without 2quater condition."
#define RRDM_1643_112_2_18_0_8_10_813 "RRDM_CheckQsearch:multi_sys=%d,si2qter_ind=%d,meas_param_desc_3g_present=%d"
#define RRDM_1662_112_2_18_0_8_10_814 "RRDM_CheckQsearch:multi_sys=%d,scell_rssi=%d,qsearch_x=%d,s_q_search_i_table[qsearch_x]=%d"
#define RRDM_1688_112_2_18_0_8_10_815 "RRDM_CompUtranMeasCriteria:para1.3g_present=%d,para2.3g_present=%d"
#define RRDM_1695_112_2_18_0_8_10_816 "RRDM_CompUtranMeasCriteria:para1.t_offset=%d,para2.t_offset=%d"
#define RRDM_1702_112_2_18_0_8_11_817 "RRDM_CompUtranMeasCriteria:return FALSE,para1->td_meas_type=%d,para2->td_meas_type."
#define RRDM_1723_112_2_18_0_8_11_818 "RRDM_CompUtranMeasParam:DGY_COMP_1"
#define RRDM_1736_112_2_18_0_8_11_819 "RRDM_CompUtranMeasParam:DGY_COMP_2"
#define RRDM_1745_112_2_18_0_8_11_820 "RRDM_CompUtranMeasParam:DGY_COMP_3"
#define RRDM_1761_112_2_18_0_8_11_821 "RRDM_CompUtranMeasParam:DGY_COMP_4,equal_num=%d,para1->arfcn_num=%d"
#define RRDM_1787_112_2_18_0_8_11_822 "RRDM_CompL1UtranMeasConf:return FALSE,TD NCELLs are not consistent."
#define RRDM_1842_112_2_18_0_8_11_823 "RRDM_FillDummyTddNcells:no dummy cell set by AT!"
#define RRDM_1928_112_2_18_0_8_11_824 "RRDM_UpdateUtranMeasNcells: UARFCN = %d already exists,index = %d in TD NCELL LIST."
#define RRDM_1944_112_2_18_0_8_11_825 "RRDM_UpdateUtranMeasNcells: td meas arfcn array is full."
#define RRDM_1960_112_2_18_0_8_11_826 "RRDM_UpdateUtranMeasNcells: td meas cell param id array is full, relate to arfcn=%d."
#define RRDM_1991_112_2_18_0_8_11_827 "RRDM_UpdateUtranMeasNcells: TDD MEAS TYPE = TD_INFO_WITH_ARFCN_AND_CELL"
#define RRDM_2043_112_2_18_0_8_11_828 "RRDM_UpdateUtranCellReselCrts: multi_sys=%d,UTRAN NCELLs are dummyed!!!"
#define RRDM_2049_112_2_18_0_8_11_829 "RRDM_UpdateUtranCellReselCrts:multi_sys=%d, 3G NCELLS in 2quater included."
#define RRDM_2060_112_2_18_0_8_11_830 "RRDM_UpdateUtranCellReselCrts:multi_sys=%d, 3G NCELLS in pmo included."
#define RRDM_2128_112_2_18_0_8_12_831 "RRDM_UpdateUtranCellReselCrts: new_resel_ctr_ptr resel criteria array is full."
#define RRDM_2173_112_2_18_0_8_12_832 "RRDM_FillUtranMeasParams: 3G meas param is not included in 2quater."
#define RRDM_2226_112_2_18_0_8_12_833 "RRDM_FillUtranDescInfoToNcell3gArry: repeated_tdd_ncell_ptr->tdd_arfcn_valid_ind=%d"
#define RRDM_2238_112_2_18_0_8_12_834 "RRDM_FillUtranDescInfoToNcell3gArry: real nw procedure."
#define RRDM_2250_112_2_18_0_8_12_835 "RRDM_FillUtranDescInfoToNcell3gArry: find_result=%d, index=%d, arfcn=%d, cpi=0xff"
#define RRDM_2264_112_2_18_0_8_12_836 "RRDM_FillTddNcellsToNcell3gArry, INVALID UARFCN = %d"
#define RRDM_2296_112_2_18_0_8_12_837 "RRDM_FillUtranDescInfoToNcell3gArry: real nw procedure."
#define RRDM_2310_112_2_18_0_8_12_838 "RRDM_FillUtranDescInfoToNcell3gArry: find_result=%d, index=%d, arfcn=%d, cpi=%d"
#define RRDM_2325_112_2_18_0_8_12_839 "RRDM_FillTddNcellsToNcell3gArry, INVALID UARFCN = %d"
#define RRDM_2378_112_2_18_0_8_12_840 "RRDM_CopySI2Quater3GNcells: si2quater count=%d,arfcn=%d,bsic=%d,ba_ind_3g=%d."
#define RRDM_2381_112_2_18_0_8_12_841 "RRDM_CopySI2Quater3GNcells:si2qter_3g_ncells is_present=%d,arfcn=%d,bsic=%d,ba_ind_3g=%d"
#define RRDM_2394_112_2_18_0_8_12_842 "RRDM_CopySI2Quater3GNcells:3g ncell info in SI2Quater are not changed."
#define RRDM_2423_112_2_18_0_8_12_843 "RRDM_CopySI2Quater3GNcells si2quater_inst[%d].msg_present = false"
#define RRDM_2439_112_2_18_0_8_12_844 "RRDM: MS is in real network configuration!!!"
#define RRDM_2449_112_2_18_0_8_12_845 "RRDM_CopySI2Quater3GNcells si2quater_inst[%d].msg_present = false"
#define RRDM_2475_112_2_18_0_8_12_846 "RRDM_AddBarredTdCellInG2TFailure: pointer cnf_ptr is NULL. "
#define RRDM_2479_112_2_18_0_8_12_847 "RRDM_AddBarredTdCellInG2TFailure: failure cause=%d"
#define RRDM_2553_112_2_18_0_8_13_848 "RRDM_GetTDCellList: utran_tdd_desc_present = 0."
#define RRDM_2559_112_2_18_0_8_13_849 "RRDM_GetTDCellList: bandwidth_tdd = 0."
#define RRDM_2567_112_2_18_0_8_13_850 "RRDM_GetTDCellList: tdd_cells_num > 10."
#define RRDM_2571_112_2_18_0_8_13_851 "RRDM_GetTDCellList:index %d, UARFCN = %d"
#define RRDM_2575_112_2_18_0_8_13_852 "RRDM_GetTDCellList:index %d, tdd_arfcn_valid_ind = 0."
#define RRDM_2581_112_2_18_0_8_13_853 "RRDM_GetTDCellList:INVALID UARFCN = %d"
#define RRDM_2590_112_2_18_0_8_13_854 "RRDM_GetTDCellList:index %d, no cell_param_id"
#define RRDM_2598_112_2_18_0_8_13_855 "RRDM_GetTDCellList:index %d, tdd_cells_num exceed 10,not store tdd cell any more"
#define RRDM_2604_112_2_18_0_8_13_856 "RRDM_GetTDCellList:index %d, cell_param_id[%d] = %d"
#define RRDM_2633_112_2_18_0_8_13_857 "RRDM_FillFastRetTDCellList: tdd_desc_present = 0."
#define RRDM_2686_112_2_18_0_8_13_858 "RRC: Assert error, invalid arfcn %d"
#define RRDM_2740_112_2_18_0_8_13_859 "RRDM_PrintDmTdNcellsInfo: multi_sys=%d,i=%d,arfcn=%d,cell_param_id=%d,index=%d,rscp=%d,q_offset=%d"
#define RRDM_2776_112_2_18_0_8_13_860 "RRDM_Init: multi_sys=%d"
#define RRDM_2781_112_2_18_0_8_13_861 "RRDM_Init: g_dm_rr_context[%d] has been already allocated."
#define RRDM_2934_112_2_18_0_8_13_862 "RRDM: use band %d"
#define RRDM_2953_112_2_18_0_8_13_863 "RRDM_UpdateMaxTimeUpdateInd invalid cell_num = %d"
#define RRDM_2986_112_2_18_0_8_13_864 "RRDM_UpdateTdCellsBarredInfo td_bar_num = %d"
#define RRDM_3016_112_2_18_0_8_14_865 "RRDM: RRDM_IsBarredTdCell: ARFCN = %d,cell_param_id = %d is barred."
#define RRDM_3071_112_2_18_0_8_14_866 "RRDM: update multi_sys=%d td ncell meas info"
#define RRDM_3112_112_2_18_0_8_14_867 "RRDM_IsInPinpGuard: multi_sys=%d,inter_rat_pinp_offset = %d"
#define RRDM_3151_112_2_18_0_8_14_868 "RRDM_InitPinpoffset: inter_rat_init_pinp_offset = %d"
#define RRDM_3176_112_2_18_0_8_14_869 "RRDM_GainPinpOffset: multi_sys=%d, gain step = %d"
#define RRDM_3185_112_2_18_0_8_14_870 "RRDM_GainPinpOffset: multi_sys=%d,inter_rat_pinp_offset = %d"
#define RRDM_3202_112_2_18_0_8_14_871 "RRDM_ResetPinpOffset:multi_sys=%d, old inter_rat_pinp_offset = %d"
#define RRDM_3237_112_2_18_0_8_14_872 "RRDM_GetPinpParams: multi_sys=%d,gsm_tmr = %d half-mins, td_tmr = %d half-mins"
#define RRDM_3244_112_2_18_0_8_14_873 "RRDM_GetPinpParams: gsm_tmr is initiated to %d s"
#define RRDM_3250_112_2_18_0_8_14_874 "RRDM_GetPinpParams: td_tmr is initiated to %d s"
#define RRDM_3269_112_2_18_0_8_14_875 "RRDM_IsAllISi2QuaterInstRcv:multi_sys=%d,si2quater_count = %d"
#define RRDM_3274_112_2_18_0_8_14_876 "RRDM_IsAllISi2QuaterInstRcv:multi_sys=%d,no si2quater is received!"
#define RRDM_3287_112_2_18_0_8_14_877 "RRDM_IsAllISi2QuaterInstRcv:multi_sys=%d,SI2QUATER %d segment not received!"
#define RRDM_3295_112_2_18_0_8_14_878 "RRDM_IsAllISi2QuaterInstRcv:multi_sys=%d,SI2QUATER all segment received!"
#define RRDM_3346_112_2_18_0_8_14_879 "RRDM:reset multi_sys=%d dm_td_cells_barred_info"
#define RRDM_3372_112_2_18_0_8_14_880 "RRDM_IsReptTDCellInfo:reset TD cell info because no TD cell is reported from L1."
#define RRDM_3409_112_2_18_0_8_14_881 "RRDM_IsReptTDCellInfo:td_cell_info-> meas < 3, and not report."
#define RRDM_3425_112_2_18_0_8_14_882 "RRDM_IsReptTDCellInfo:not report."
#define RRDM_3489_112_2_18_0_8_14_883 "RRDM_ResetTDCellInfo:reset TD cell info."
#define RRDM_3510_112_2_18_0_8_14_884 "Is eplmn"
#define RRDM_3530_112_2_18_0_8_15_885 "RRDM_IsDmReselAnyPLMN: g_rr_dm_select_any_plmn_flag = %d return TRUE"
#define RRDM_3534_112_2_18_0_8_15_886 "RRDM_IsDmReselAnyPLMN: g_rr_dm_select_any_plmn_flag = %d return FALSE"
#define RRDM_3568_112_2_18_0_8_15_887 "RRDM_UpdateMaxTimeUpdateInd invalid cell_num = %d"
#define RRDM_3607_112_2_18_0_8_15_888 "RRDM_UpdateMaxTimeUpdateInd cell_num = %d"
#define RRDM_3621_112_2_18_0_8_15_889 "RRDM_HandleMaxTimeUpdateInd: handle the timer of RR_MAX_TIME_UPDATE_IND"
#define RRDM_3646_112_2_18_0_8_15_890 "RRDM_FillUtranMeas: multi_sys=%d,SCELL ARFCN = %d"
#define RRDM_3652_112_2_18_0_8_15_891 "RRDM_FillUtranMeas: SI2Q receiving is not completed, so TD MEAS param is not filled."
#define RRDM_3660_112_2_18_0_8_15_892 "RRDM_FillUtranMeas: SI2Q is not present, so TD MEAS param is not filled."
#define RRDM_3685_112_2_18_0_8_15_893 "RRDM_FillUtranMeas: multi_sys=%d,SI2QUATER 3g ncell[%d]: arfcn=%d,cpi=%d,index=%d"
#define RRDM_3692_112_2_18_0_8_15_894 "RRDM_FillUtranMeas: td_meas_type=%d,meas_param_desc_3g_present=%d,qsearch_x=%d,t_offset=%d"
#define RRDM_3718_112_2_18_0_8_15_895 "RRDM_StartUtranMeas:multi_sys=%d, stack_cfg_ptr is NULL."
#define RRDM_3731_112_2_18_0_8_15_896 "RRDM_StartUtranMeas: multi_sys=%d,MS is not Dule-Mode,does not need tO start TD measurement"
#define RRDM_3758_112_2_18_0_8_15_897 "RRDM_StartUtranMeas: td_meas_list:i = %d,arfcn = %d,cell_param_id_num = %d"
#define RRDM_3763_112_2_18_0_8_15_898 "RRDM_StartUtranMeas:multi_sys=%d,NO valid 3G neibourgh cells!!!!,return"
#define RRDM_3774_112_2_18_0_8_15_899 "RRDM_StartUtranMeas:TD should be stopped for UE in dark,return FALSE"
#define RRDM_3798_112_2_18_0_8_15_900 "RRDM_StartUtranMeas:multi_sys=%d,UPDATE_L1_TD_MEAS"
#define RRDM_3802_112_2_18_0_8_15_901 "RRDM_StartUtranMeas:multi_sys=%d,KEEP_L1_TD_MEAS"
#define RRDM_3811_112_2_18_0_8_15_902 "RRDM_StartUtranMeas:multi_sys=%d, START_L1_TD_MEAS"
#define RRDM_3820_112_2_18_0_8_15_903 "RRDM_StartUtranMeas:ASSERT!WRONG q_offset_table_switch = %d"
#define RRDM_3824_112_2_18_0_8_15_904 "RRDM_StartUtranMeas:multi_sys=%d,last return STOP_L1_TD_MEAS"
#define RRDM_3900_112_2_18_0_8_15_905 "RRDM_TdReselTrig: multi_sys=%d,eng_offset=%d,penny_offset=%d"
#define RRDM_3923_112_2_18_0_8_15_906 "RRDM_TdReselTrig: Q_offset =0,resel to TDD immediate"
#define RRDM_3934_112_2_18_0_8_15_907 "RRDM_TdReselTrig:multi_sys=%d,rssi = %d,rscp = %d ,offset = %d"
#define RRDM_3940_112_2_18_0_8_15_908 "temp_level < temp_offset,NOT reselect"
#define RRDM_3966_112_2_18_0_8_15_909 "RRDM_TdReselTrig: TRIGGER TD RESEL , TD RSCP LESS THAN ENG-OFFSET, NOT RESEL"
#define RRDM_3993_112_2_18_0_8_15_910 "RRDM_TdReselTrig: cmcc spec multi_sys=%d, cell_num=%d, q_offset=%d,penny_offset=%d,eng_offset=%d,inter_rat_pinp_offset=%d"
#define RRDM_4006_112_2_18_0_8_15_911 "RRDM_TdReselTrig: multi_sys=%d, q_meas=%d, resel_threshold =%d"
#define RRDM_4091_112_2_18_0_8_16_912 "RRDM: RRDM_IsReselToUtranAllowed, not dual mode."
#define RRDM_4098_112_2_18_0_8_16_913 "DM:call ph_gsm_to_td_is_allowed, and return false."
#define RRDM_4101_112_2_18_0_8_16_914 "DM:PDP status is %d, PDP timer status is %d"
#define RRDM_4117_112_2_18_0_8_16_915 "RRDM_IsReselToTD:return FALSE,not reselect to TD tcell_index=%d"
#define RRDM_4125_112_2_18_0_8_16_916 "RRDM: RRDM_IsReselToUtranAllowed, cell_num= %d, tcell_index=%d"
#define RRDM_4193_112_2_18_0_8_16_917 "RRDM_ProcessTDReselTimer:multi_sys=%d"
#define RRDM_4217_112_2_18_0_8_16_918 "RRDM_ProcessTDReselTimer:multi_sys=%d,rssi=%d,rscp=%d,offset=%d"
#define RRDM_4222_112_2_18_0_8_16_919 "RRDM_ProcessTDReselTimer:clear candidate_times!"
#define RRDM_4241_112_2_18_0_8_16_920 "RRDM, PROCESS 2->3 RESEL TIMER , TD RSCP LESS THAN ENG-OFFSET, NOT RESEL"
#define RRDM_4261_112_2_18_0_8_16_921 "RRDM_ProcessTDReselTimer: cmcc spec multi_sys=%d,q_offset = %d,penny_offset = %d,eng_offset = %d,inter_rat_pinp_offset = %d"
#define RRDM_4280_112_2_18_0_8_16_922 "RRDM_ProcessTDReselTimer:clear candidate_times!"
#define RRDM_4294_112_2_18_0_8_16_923 "RRDM_ProcessTDReselTimer:multi_sys=%d,TDD index = %d,rscp = %d ,q_offset = %d,candidate_times = %d"
#define RRDM_4300_112_2_18_0_8_16_924 "RRDM_ProcessTDReselTimer:ASSERT!WRONG g_gsm_stack_cfg_params.q_offset_table_switch = %d"
#define RRDM_4304_112_2_18_0_8_16_925 "RRDM_ProcessTDReselTimer:multi_sys=%d,TDD q_offset_table_switch = %d"
#define RRDM_4305_112_2_18_0_8_16_926 "RRDM_ProcessTDReselTimer:multi_sys=%d,q_offset = %d"
#define RRDM_4312_112_2_18_0_8_16_927 "DM:call ph_gsm_to_td_is_allowed, and return false."
#define RRDM_4315_112_2_18_0_8_16_928 "DM:PDP status is %d, PDP timer status is %d"
#define RRDM_4330_112_2_18_0_8_16_929 "GSM->TDD reselect start:multi_sys=%d,TDD cell arfcn = %d,cell_id = %d"
#define RRDM_4387_112_2_18_0_8_16_930 "RRDM_StartReselToTD: multi_sys=%d,target td ARFCN=%d,CELL PARAM ID=%d,RSCP=%d"
#define RRDM_4393_112_2_18_0_8_16_931 "TD_INFO_WITH_ARFCN_BUT_RSSI:parameter id = %d"
#define RRDM_4437_112_2_18_0_8_16_932 "RRDM: multi_sys=%d receive TRR_RR_DM_RAT_CHG_TO_UMTS_START_RESP."
#define RRDM_4573_112_2_18_0_8_17_933 "RRDM: card sync target cell: %d %d."
#define RRDM_4646_112_2_18_0_8_17_934 "RRDM_IsFastRet2UtranAllowed: all FR switches are disabled!!"
#define RRDM_4653_112_2_18_0_8_17_935 "DM:call ph_gsm_to_td_is_allowed, and return false."
#define RRDM_4656_112_2_18_0_8_17_936 "DM:PDP status is %d, PDP timer status is %d"
#define RRDM_4662_112_2_18_0_8_17_937 "RRDM_IsFastRet2UtranAllowed: MS is not Dule-Mode,NOT return to TD!"
#define RRDM_4699_112_2_18_0_8_17_938 "RRDM_IsFastRet2UtranAllowed: tdd_cells_num = 0,not start FR2U."
#define RRDM_4724_112_2_18_0_8_17_939 "RRDM: RRDM_StartFastRet2Utran, multi_sys=%d."
#define RRDM_4788_112_2_18_0_8_17_940 "RRDM_IsPcco2UtranAllowed: PCCO switch is disabled!!"
#define RRDM_4795_112_2_18_0_8_17_941 "RRDM_IsPcco2UtranAllowed: 3G target cell is not present."
#define RRDM_4804_112_2_18_0_8_17_942 "RRDM_IsPcco2UtranAllowed: 3G target cell is dummied."
#define RRDM_4856_112_2_18_0_8_17_943 "RRDM: RRDM_StartPCCO2UTRAN, multi_sys=%d."
#define RRDM_4918_112_2_18_0_8_17_944 "RRDM: multi_sys=%d, 2->3 PCCO failure."
#define RRDM_4934_112_2_18_0_8_17_945 "RRDM: multi_sys=%d, 2->3 PCCO success."
#define RRDM_4940_112_2_18_0_8_17_946 "RRDM: card sync target cell: %d %d."
#define RRDM_4967_112_2_18_0_8_17_947 "RRDM_HandleGsm2TdPCCOResp:multi_sys=%d,ho_status=%d"
#define RRDM_4973_112_2_18_0_8_17_948 "RRDM:multi_sys=%d,2->3 PCCO failure."
#define RRDM_5040_112_2_18_0_8_18_949 "RRDM_HandleGsm2UtranAbortCnf:multi_sys=%d,ho_type=%d"
#define RRDM_5072_112_2_18_0_8_18_950 "RRDM_IsUseGPRS3GParam:gmm_state=%d,pmo_valid=%d,n enh_meas_param_present=%d,neighbour_cell_desc_3g_present=%d,gprs_3g_meas_para_desc_present=%d"
#define RRDM_5208_112_2_18_0_8_18_951 "RRDM_UpdateGPRSTDNcelllist:multi_sys=%d, si2quater arfcn=%d,bsic=%d;n PMO pmo_ind=%d."
#define RRDM_5213_112_2_18_0_8_18_952 "RRDM_UpdateGPRSTDNcelllist:pmo_3g_ncells is_present=%d,scell arfcn=%d, scell bsic=%d,pmo_ind=%d."
#define RRDM_5219_112_2_18_0_8_18_953 "RRDM_UpdateGPRSTDNcelllist: ncell 3g info in PMO are not changed."
#define RRDM_5261_112_2_18_0_8_18_954 "RRDM_UpdateGPRSTDNcelllist: PMO 3G NCELL[%d]: arfcn=%d,cpi=%d,index=%d"
#define RRDM_5305_112_2_18_0_8_18_955 "RRDM_FillGPRSTDMeasParam from the params of PMO."
#define RRDM_5315_112_2_18_0_8_18_956 "RRDM_FillGPRSTDMeasParam from the params of 2Quater."
#define RRDM_5338_112_2_18_0_8_18_957 "RRDM_GetT3174Duration: multi_sys=%d,dummy millisec_val=%d"
#define RRDM_5493_112_2_18_0_8_18_958 "RRDM_HandleRatChg2GsmStartReqMsg: multi_sys=%d,target gsm={arfcn=%d,band=%d,bsic=%d}; n   tdd source cell={arfcn=%d,cpi=%d};n   revert_flag=%d,is_card_sync=%d,sync_cause=%d,cmd_type=%d"
#define RRDM_5561_112_2_18_0_8_19_959 "RRDM_GetPLMNUsedBand: band=%d"
#define RRDM_5687_112_2_18_0_8_19_960 "RRDM: multi_sys=%d, Set rr_utran_2_gsm_sub_state=%d"
#define RRDM_5699_112_2_18_0_8_19_961 "RRDM: multi_sys=%d, get rr_utran_2_gsm_sub_state=%d"
#define RRDM_5748_112_2_18_0_8_19_962 "RRDM_SendUtranRej2GsmResult, multi_sys=%d, result=%d"
#define RRDM_5771_112_2_18_0_8_19_963 "RRDM_SendUtranRej2GsmResult: camp_cell.arfcn=%d,bsic=%d,freq_band=%d"
#define RRDM_5842_112_2_18_0_8_19_964 "RRDM_SendInterRatHOResult: multi_sys=%d, HO result arfcn %d, band %d"
#define RRDM_5858_112_2_18_0_8_19_965 "RRDM_SendInterRatHOResult: multi_sys=%d,camp_cell.arfcn=%d,bsic=%d,freq_band=%d"
#define RRDM_5976_112_2_18_0_8_19_966 "DM: RRDM_SendUtran2GsmStartResp ,multi_sys=%d, result = %d"
#define RRDM_6048_112_2_18_0_8_20_967 "RRDM_GetReselGsmArfcnAndBand: arfcn=%d band %d"
#define RRDM_6092_112_2_18_0_8_20_968 "RRDM_IsNACCInfoPresent: NACC info is not present."
#define RRDM_6098_112_2_18_0_8_20_969 "RRDM_IsNACCInfoPresent: PBCCH is present."
#define RRDM_6103_112_2_18_0_8_20_970 "RRDM_IsNACCInfoPresent: no NACC info!psi1_present = %d,psi2_present=%d"
#define RRDM_6114_112_2_18_0_8_20_971 "RRDM_IsNACCInfoPresent: no NACC info!si3_present = %d,si13_present=%d"
#define RRDM_6121_112_2_18_0_8_20_972 "RRDM_IsNACCInfoPresent: should get NACC CCOFU!"
#define RRDM_6136_112_2_18_0_8_20_973 "RRDM_GetNACCCellInfo: no NACC info, so NOT copy cell info."
#define RRDM_6140_112_2_18_0_8_20_974 "RRDM_GetNACCCellInfo: start copy CCOFU NACC cell info!"
#define RRDM_6203_112_2_18_0_8_20_975 "RRDM_GetNACCinfo: SI index %d, msg type = %d is error."
#define RRDM_6207_112_2_18_0_8_20_976 "RRDM_GetNACCinfo: SI index %d, msg type = %d"
#define RRDM_6218_112_2_18_0_8_20_977 "RRDM_GetNACCinfo: PSI index %d, msg type = %d is error."
#define RRDM_6222_112_2_18_0_8_20_978 "RRDM_GetNACCinfo: PSI index %d, msg type = %d"
#define RRDM_6226_112_2_18_0_8_20_979 "RRDM_GetNACCinfo: index %d, NOT SI or PSI, msg kind = %d"
#define RRDM_6244_112_2_18_0_8_20_980 "RRDM_IsStartNACC: NACC info is not present."
#define RRDM_6250_112_2_18_0_8_20_981 "RRDM_IsStartNACC: PBCCH is present."
#define RRDM_6255_112_2_18_0_8_20_982 "RRDM_IsStartNACC: no NACC info!psi1_present = %d,psi2_present=%d"
#define RRDM_6264_112_2_18_0_8_20_983 "RRDM_IsStartNACC: no NACC info!si3_present = %d,si13_present=%d"
#define RRDM_6269_112_2_18_0_8_20_984 "RRDM_IsStartNACC: start NACC CCOFU!"
#define RRDM_6322_112_2_18_0_8_20_985 "RRDM_ControlInterRat:multi_sys=%d,rr_cur_rat=%d,rat_chg_allowed=%d,inter_rat_chg_allowed=%d"
#define RRDM_6328_112_2_18_0_8_20_986 "RRDM_ControlInterRat:ASSERT!WRONG q_offset_table_switch = %d"
#define RRDM_6379_112_2_18_0_8_20_987 "RRDM_SendUMTS2GSMProcResultToNAS , result = %d"
#define RRDM_6410_112_2_18_0_8_20_988 "RRDM_SendUMTS2GSMProcResultToTRR, result = %d"
#define RRDM_6491_112_2_18_0_8_20_989 "RRDM_SendReselResultToNAS , result = %d"
#define RRDM_6519_112_2_18_0_8_20_990 "DM: RRDM_SendReselResultToTRR , result = %d"
#define RRDM_6551_112_2_18_0_8_21_991 "RRDM_SendReselResultToTRR: camp_cell.arfcn=%d,bsic=%d,freq_band=%d"
#define RRDM_6606_112_2_18_0_8_21_992 "DM: , RESEL , BSIC FAIL"
#define RRDM_6638_112_2_18_0_8_21_993 "DM: check arfcn , %d %d %d %d "
#define RRDM_6645_112_2_18_0_8_21_994 "DM: RRDM_CheckArfcnBsic , BSIC not matched"
#define RRDM_6664_112_2_18_0_8_21_995 "DM: get rssi from td cmd , rssi = %d "
#define RRDM_6732_112_2_18_0_8_21_996 "RRDM_WaitDspSync: wait dsp sync 10ms"
#define RRDM_6777_112_2_18_0_8_21_997 "RRDM_GetMphChangeMode: g_rr_dm_rat_chg_type = %d,ho_type=%d,change_type=%d."
#define RRDM_6782_112_2_18_0_8_21_998 "RRDM_GetMphChangeMode:Error,ho_type=%d,g_rr_dm_rat_chg_type=%d,not equal."
#define RRDM_6985_112_2_18_0_8_21_999 "RRDM_AddBaFromInterRAT: ba_list has been full,return FALSE."
#define RRDM_7021_112_2_18_0_8_21_1000 "RRDM_AddBaFromInterRAT: not fill ba list,return FALSE."
#define RRMSG_2369_112_2_18_0_8_26_1001 "CASE207:get si2bis ba"
#define RRMSG_2375_112_2_18_0_8_26_1002 "CASE207:invalid si2bis"
#define RRMSG_2640_112_2_18_0_8_27_1003 "RR:si 1, pcs ind is TRUE "
#define RRMSG_3020_112_2_18_0_8_28_1004 "SI2Quater: bandwidth_tdd=%d"
#define RRMSG_3023_112_2_18_0_8_28_1005 "RR_DecodeUTRANTDD_Desc:@@@@@384 Mcps band is received,which is not supported in this version!\n"
#define RRMSG_3034_112_2_18_0_8_28_1006 "RR_DecodeUTRANTDD_Desc:@@@@@384 Mcps band is received,which is not supported in this version!\n"
#define RRMSG_3042_112_2_18_0_8_28_1007 "SI2Quater: nr_repeated_utran_tdd_neighbour_cells = %d"
#define RRMSG_3085_112_2_18_0_8_28_1008 "RR_DecodeUTRANTDD_Desc:after tdd_indic0=%d bit is 1,TDD_ARFCN_INDEX ignore"
#define RRMSG_3099_112_2_18_0_8_28_1009 "SI2Quater: L= %d,arfcn = %d,nr_of_tdd_cells = %d"
#define RRMSG_3125_112_2_18_0_8_28_1010 "RR_DecodeUTRANTDD_Desc:offset_octets = %d, offset_bits = %d"
#define RRMSG_3127_112_2_18_0_8_28_1011 "RR_DecodeUTRANTDD_Desc:w[%d] = %d, wn[%d] = %d"
#define RRMSG_3185_112_2_18_0_8_28_1012 "SI2Quater: L = %d,cell_param_id = %d"
#define RRMSG_3231_112_2_18_0_8_28_1013 "Bandwidth_FDD included"
#define RRMSG_3236_112_2_18_0_8_28_1014 "Repeated UMTS FDD Neighbour Cells included"
#define RRMSG_3296_112_2_18_0_8_28_1015 "SI2Quater: Index_Start_3G=%d"
#define RRMSG_3307_112_2_18_0_8_28_1016 "SI2Quater: absolute_index_start_emr=%d"
#define RRMSG_3314_112_2_18_0_8_28_1017 "SI2Quater: total_nr_utran_fdd_ncells=%d"
#define RRMSG_3350_112_2_18_0_8_28_1018 "SI2Quater: QSearch_i = %d"
#define RRMSG_3357_112_2_18_0_8_28_1019 "SI2Quater: fdd measurement information is included,need skip all bits in now version"
#define RRMSG_3372_112_2_18_0_8_28_1020 "SI2Quater: tdd_qoffset=%d, tdd_multirat_reporting = %d"
#define RRMSG_3390_112_2_18_0_8_28_1021 "SIQuater:skip following bits!\n"
#define RRMSG_3394_112_2_18_0_8_28_1022 "SIQuater:RTD6 exist!\n"
#define RRMSG_3423_112_2_18_0_8_29_1023 "SIQuater:RTD12 exist!\n"
#define RRMSG_3507_112_2_18_0_8_29_1024 "SIQuater:GPRS_MEASUREMENT_Parameters Description is included!\n"
#define RRMSG_3615_112_2_18_0_8_29_1025 "SI2Quater: nc_order=%d,nc_period_present=%d,nc_non_drx_period=%d,nc_reporting_period_i=%d,nc_reporting_period_t=%d"
#define RRMSG_3658_112_2_18_0_8_29_1026 "Si2Q EXT: ccn_support %d, num %d"
#define RRMSG_3754_112_2_18_0_8_29_1027 "RRC: si2quater available indicated by si3"
#define RRMSG_3762_112_2_18_0_8_29_1028 "RRC: si2quater position = %d"
#define RRMSG_3768_112_2_18_0_8_29_1029 "RRC: arfcn = %d, ra_colour = %d\n "
#define RRMSG_3878_112_2_18_0_8_29_1030 "RRC: pcs ind %d"
#define RRMSG_3881_112_2_18_0_8_29_1031 "RRC: SI6 rest len wrong"
#define RRMSG_3915_112_2_18_0_8_30_1032 "RRC: network assign half rate channel!"
#define RRMSG_4855_112_2_18_0_8_31_1033 "    si2quater_present=%d"
#define RRMSG_4887_112_2_18_0_8_31_1034 "    temp_ba_ind = %d,\n    temp_3g_ba_ind = %d,\n    temp_mp_change_mark = %d,\n    temp_si2quater_index = %d,\n    temp_si2quater_count = %d"
#define RRMSG_4892_112_2_18_0_8_31_1035 "instance %d hans been received,return"
#define RRMSG_4967_112_2_18_0_8_32_1036 "SIQuater:Measurement_Parameters Description is included!\n"
#define RRMSG_4985_112_2_18_0_8_32_1037 "SIQuater:GPRS RTD description is not supported by this version!\n"
#define RRMSG_4991_112_2_18_0_8_32_1038 "SIQuater:GPRS_BSIC Descriptionis included!\n"
#define RRMSG_5001_112_2_18_0_8_32_1039 "SIQuater:GPRS_REPORT PRIORITY Description is included!\n"
#define RRMSG_5010_112_2_18_0_8_32_1040 "SIQuater:GPRS_MEASUREMENT_Parameters Description is included!\n"
#define RRMSG_5020_112_2_18_0_8_32_1041 "SIQuater :GPRS_3G_MEASUREMENT Parameters Description  included"
#define RRMSG_5053_112_2_18_0_8_32_1042 "SI2Q REST: ccn_support %d, num %d"
#define RRMSG_5060_112_2_18_0_8_32_1043 "SIQuater :extension length included"
#define RRMSG_5098_112_2_18_0_8_32_1044 "SIQuater :GPRS_3G_MEASUREMENT Parameters Description  included"
#define RRMSG_5137_112_2_18_0_8_32_1045 "RR_DecodeSi2Quater:RRC_MANDATORY_IE_ERROR"
#define RRMSG_5161_112_2_18_0_8_32_1046 "RR_DecodeSi2Quater:SI2QUATER instance %d not present,need wait it!"
#define RRMSG_5169_112_2_18_0_8_32_1047 "RR_DecodeSi2Quater:all sengment is received!"
#define RRMSG_5189_112_2_18_0_8_32_1048 "RRC:received System Information 9, not supported now!"
#define RRMSG_5207_112_2_18_0_8_32_1049 "RRC:received System Information 16, not supported now!"
#define RRMSG_5225_112_2_18_0_8_32_1050 "RRC:received System Information 17, not supported now!"
#define RRMSG_5243_112_2_18_0_8_33_1051 "RRC:received System Information 18, not supported now!"
#define RRMSG_5261_112_2_18_0_8_33_1052 "RRC:received System Information 19, not supported now!"
#define RRMSG_5279_112_2_18_0_8_33_1053 "RRC:received System Information 20, not supported now!"
#define RRMSG_5308_112_2_18_0_8_33_1054 "RRC: si2bis is not present, ba_0 is %d"
#define RRMSG_5319_112_2_18_0_8_33_1055 "RRC: ba_0 is %d"
#define RRMSG_5423_112_2_18_0_8_33_1056 "RRMSG_HandleSysInfo: rr_dm_context_ptr is NULL."
#define RRMSG_5438_112_2_18_0_8_33_1057 "RR: Decode dummy si2quarter"
#define RRMSG_5465_112_2_18_0_8_33_1058 "RRC: use bcch ba when traffic."
#define RRMSG_5572_112_2_18_0_8_33_1059 "RRC: RR_DecodeSi2quater"
#define RRMSG_5603_112_2_18_0_8_33_1060 "RRC: unknown msg type %d"
#define RRMSG_5697_112_2_18_0_8_33_1061 "RRC: cell bar"
#define RRMSG_5780_112_2_18_0_8_34_1062 "RRC: cell bar"
#define RRMSG_6326_112_2_18_0_8_35_1063 "RRC: egprs packet uplink extend_RA %d"
#define RRMSG_6431_112_2_18_0_8_35_1064 "RRC: gprs packet uplink extend_RA %d"
#define RRMSG_6469_112_2_18_0_8_35_1065 "RRC:Timing Advance is out of range!"
#define RRMSG_6565_112_2_18_0_8_35_1066 "RRC: immed_assign->request_ref->random_ab %x"
#define RRMSG_6851_112_2_18_0_8_36_1067 "RRC: recv imm reject"
#define RRMSG_8117_112_2_18_0_8_38_1068 "RR:MR config icmi %d start_mode %d acs 0x%x"
#define RRMSG_8201_112_2_18_0_8_38_1069 "RRC: VGCS???"
#define RRMSG_8212_112_2_18_0_8_39_1070 "RR:Channel modify msg error"
#define RRMSG_8437_112_2_18_0_8_39_1071 "RR: MULTI SLOT ERROR!"
#define RRMSG_8940_112_2_18_0_8_40_1072 "RRC: VGCS???"
#define RRMSG_8950_112_2_18_0_8_40_1073 "RR:Channel assign msg error"
#define RRMSG_8970_112_2_18_0_8_40_1074 "RRC: assert error, assign_cmd start cipher but no cipher cmd recv before"
#define RRMSG_9711_112_2_18_0_8_42_1075 "RRC: VGCS???"
#define RRMSG_9721_112_2_18_0_8_42_1076 "RR:Channel assign msg error"
#define RRMSG_9741_112_2_18_0_8_42_1077 "RRC: assert error, handover start cipher but no cipher cmd recv before"
#define RRMSG_11598_112_2_18_0_8_45_1078 "RRC: ba_5_ind and si5bis not present"
#define RRMSG_11614_112_2_18_0_8_45_1079 "RRC: ba_2_ind and si2bis not present"
#define RRMSG_11941_112_2_18_0_8_46_1080 "RRMSG_DecodeCellChangeOrder:optional 3G targe cell IE is error,so ignore it."
#define RRMSG_12388_112_2_18_0_8_47_1081 "RRC: SGSNR %d"
#define RRMSG_12397_112_2_18_0_8_47_1082 "NACC: si_status_ind %d"
#define RRMSG_12537_112_2_18_0_8_47_1083 "RRMSG_EncodePacketCellChangeFailure: cause=%d"
#define RRMSG_13022_112_2_18_0_8_48_1084 "RRC:received Extended Mearsurement Order!"
#define RRMSG_13179_112_2_18_0_8_49_1085 "RRC: receive all mi, emr support is %d"
#define RRMSG_13897_112_2_18_0_8_50_1086 "RRMSG_Record3GTargetCellContent: tdd_cell_valid:%d,fdd_cell_valid:%d"
#define RRMSG_13960_112_2_18_0_8_50_1087 "Bandwidth_FDD included"
#define RRMSG_13972_112_2_18_0_8_50_1088 "FDD Neighbour Cells no = %d"
#define RRMSG_14034_112_2_18_0_8_50_1089 "Bandwidth_TDD included"
#define RRMSG_14038_112_2_18_0_8_50_1090 "@@@@@384 Mcps band is received,which is not supported in this version!\n"
#define RRMSG_14045_112_2_18_0_8_51_1091 "@@@@@384 Mcps band is received,which is not supported in this version!\n"
#define RRMSG_14074_112_2_18_0_8_51_1092 "L= %d,arfcn = %d"
#define RRMSG_14081_112_2_18_0_8_51_1093 "L= %d,nr_of_tdd_cells = %d"
#define RRMSG_14218_112_2_18_0_8_51_1094 "RR_RecordCellSelIndicator: simple FR and innovation FR are disabled!!"
#define RRMSG_14245_112_2_18_0_8_51_1095 "RR_RecordCellSelIndicator:WRONG description is included in channel release!"
#define RRPROC_376_112_2_18_0_8_52_1096 "RRC: nc_reporting_period_i = %d\n"
#define RRPROC_377_112_2_18_0_8_52_1097 "RRC: drx_period = %d\n"
#define RRPROC_378_112_2_18_0_8_52_1098 "RRC: t3158_value = %d\n"
#define RRPROC_412_112_2_18_0_8_52_1099 "RRC: transfer nc_reporting_period_t = %d\n"
#define RRPROC_478_112_2_18_0_8_52_1100 "RRPROC_CheckAccessPermission:rr_service_state = %d,permission = %d"
#define RRPROC_479_112_2_18_0_8_52_1101 "RRPROC_CheckAccessPermission:rr_est_cause = %d"
#define RRPROC_574_112_2_18_0_8_52_1102 "RRPROC:Default case!"
#define RRPROC_1158_112_2_18_0_8_53_1103 "RRPROC_FindStrongestCell:all gsm cells are not suitable for camping in dual mode."
#define RRPROC_1213_112_2_18_0_8_53_1104 "RRC:RR NCELL: cell clear all"
#define RRPROC_1289_112_2_18_0_8_54_1105 "RR: Assert Failure, unknown page mode."
#define RRPROC_1931_112_2_18_0_8_55_1106 "RRC: use band %d"
#define RRPROC_1956_112_2_18_0_8_55_1107 "RRC: Assert error, invalid arfcn %d"
#define RRPROC_2146_112_2_18_0_8_55_1108 "RRC: traffic ba is invalid, not clear ncell"
#define RRPROC_2161_112_2_18_0_8_55_1109 "RRC: clear cell %d, arfcn %d"
#define RRPROC_2293_112_2_18_0_8_56_1110 "RRC_ForceCamping:error! arfcn_count = %d,exceed the allowed max count %d \n"
#define RRPROC_2299_112_2_18_0_8_56_1111 "RRC_ForceCamping:warning! arfcn_count = %d \n"
#define RRPROC_2320_112_2_18_0_8_56_1112 "RRC_ForceCamping: index = %d, arfcn = %d \n"
#define RRPROC_2346_112_2_18_0_8_56_1113 "RRC: RRC_CancelForceCamping \n"
#define RRPROC_2768_112_2_18_0_8_57_1114 "RRC: RECV si BAD,compare1{%d,%d},compare2{%d,%d},ph_decode_indicator=%d,msg_error=%d"
#define RRPROC_2815_112_2_18_0_8_57_1115 "RRC: FTA RRMSG_GetPacketMsgType is TRUE"
#define RRPROC_2818_112_2_18_0_8_57_1116 "RRC: FTA msg type is paging"
#define RRPROC_2822_112_2_18_0_8_57_1117 "RRC: FTA paging msg decode is ok"
#define RRPROC_2825_112_2_18_0_8_57_1118 "RRC: FTA paging address is ok, save it"
#define RRPROC_2915_112_2_18_0_8_57_1119 "%s"
#define RRPROC_2923_112_2_18_0_8_57_1120 "%s"
#define RRPROC_2962_112_2_18_0_8_57_1121 "RRC: %d findRcell, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d"
#define RRPROC_2967_112_2_18_0_8_57_1122 "RRC: tc 0x%x, tc 0x%x, tc 0x%x, tc 0x%x, tc 0x%x, tc 0x%x"
#define RRPROC_3070_112_2_18_0_8_57_1123 "RRC: FAB CAMP"
#define RRPROC_3406_112_2_18_0_8_58_1124 "RRC_Get_ForceCamping_Gcell: error! g_forced_arfcn_count = %d"
#define RRPROC_3428_112_2_18_0_8_58_1125 "RRC: get rach action with %d, %d, %d"
#define RR_ADAPTER_973_112_2_18_0_9_37_1126 "RRA: enter RRA_Init"
#define RR_ADAPTER_1108_112_2_18_0_9_38_1127 "RRA: rr1 -- %d "
#define RR_ADAPTER_1109_112_2_18_0_9_38_1128 "RRA: rrca_status1 -- %d, sig_code 0x%x"
#define RR_ADAPTER_1112_112_2_18_0_9_38_1129 "RRA: rr2 -- %d "
#define RR_ADAPTER_1113_112_2_18_0_9_38_1130 "RRA: rrca_status2 -- %d sig_code 0x%x"
#define RR_ADAPTER_1241_112_2_18_0_9_38_1131 "RRA: s_rra_gprs_id 0x%x is invalid, sig_code = 0x%x."
#define RR_ADAPTER_1277_112_2_18_0_9_38_1132 "RRA: s_rra_gprs_id 0x%x is invalid, sig_code = 0x%x."
#define RR_ADAPTER_1446_112_2_18_0_9_38_1133 "RRA: assert error, unprocessed signal 0x%x"
#define RR_ADAPTER_1480_112_2_18_0_9_38_1134 "RRA: MPH_ACTIVE_REQ discarded in rat change, rr %d"
#define RR_ADAPTER_1589_112_2_18_0_9_39_1135 "RRA: rr %d, rr_task_id = 0x%x"
#define RR_ADAPTER_1664_112_2_18_0_9_39_1136 "RRA: powersweep req %d state0 = %d, state1 = %d, state2 = %d, state3 = %d"
#define RR_ADAPTER_1670_112_2_18_0_9_39_1137 "RRA: powersweep req %d state0 = %d, state1 = %d, state2 = %d"
#define RR_ADAPTER_1675_112_2_18_0_9_39_1138 "RRA: powersweep req %d state0 = %d, state1 = %d"
#define RR_ADAPTER_1679_112_2_18_0_9_39_1139 "RRA: powersweep req %d state0 = %d"
#define RR_ADAPTER_1695_112_2_18_0_9_39_1140 "RRA: band change from %d to %d, must do power sweep"
#define RR_ADAPTER_1709_112_2_18_0_9_39_1141 "RRA: discard powersweep req %d, wait layer1 powersweep cnf."
#define RR_ADAPTER_1761_112_2_18_0_9_39_1142 "RRA: dummy powersweep cnf state0 = %d, state1 = %d, state2 = %d, state3 = %d, last traffic id = %d"
#define RR_ADAPTER_1767_112_2_18_0_9_39_1143 "RRA: dummy powersweep cnf state0 = %d, state1 = %d, state2 = %d, last traffic id = %d"
#define RR_ADAPTER_1772_112_2_18_0_9_39_1144 "RRA: dummy powersweep cnf state0 = %d, state1 = %d, last traffic id = %d"
#define RR_ADAPTER_1776_112_2_18_0_9_39_1145 "RRA: dummy powersweep cnf state0 = %d, last traffic id = %d"
#define RR_ADAPTER_1791_112_2_18_0_9_39_1146 "RRA: return dummy cnf."
#define RR_ADAPTER_1813_112_2_18_0_9_39_1147 "RRA: rr %d in find plmn flag = %d, state = %d"
#define RR_ADAPTER_1837_112_2_18_0_9_39_1148 "RRA: rr %d enter RRA_RR_SEARCH."
#define RR_ADAPTER_1852_112_2_18_0_9_39_1149 "powersweepreq:master id %d"
#define RR_ADAPTER_1906_112_2_18_0_9_39_1150 "RRA: po task id = %d, %d, %d, d, find plmn flag = %d, %d, %d, %d , state = %d, %d, %d, %d, master = %d"
#define RR_ADAPTER_1919_112_2_18_0_9_39_1151 "RRA: po task id = %d, %d, %d, find plmn flag = %d, %d, %d , state = %d, %d, %d, master = %d"
#define RR_ADAPTER_1929_112_2_18_0_9_39_1152 "RRA: po task id = %d, %d, find plmn flag = %d, %d, state = %d, %d,, master = %d"
#define RR_ADAPTER_1936_112_2_18_0_9_39_1153 "RRA: po task id = %d, find plmn flag = %d, state = %d,master = %d"
#define RR_ADAPTER_1959_112_2_18_0_9_39_1154 "RRA: layer1 return dummy powersweep cnf."
#define RR_ADAPTER_2281_112_2_18_0_9_40_1155 "RRA: discard bcch decode req when rat change, i = %d"
#define RR_ADAPTER_2310_112_2_18_0_9_40_1156 "RRA: recv pbcch decode req, req_rr = %d"
#define RR_ADAPTER_2343_112_2_18_0_9_40_1157 "RRA: no BcchInfoInd because of busy state of RR (%d)!"
#define RR_ADAPTER_2406_112_2_18_0_9_40_1158 "RRA: recv pbcch info ind, only send to rr %d"
#define RR_ADAPTER_2494_112_2_18_0_9_40_1159 "RRA: can't do bcch camp, because there is pbcch camp"
#define RR_ADAPTER_2520_112_2_18_0_9_41_1160 "RRA: can't do bcch camp, because there is card conflict"
#define RR_ADAPTER_2606_112_2_18_0_9_41_1161 "RRA: clear mac rlc flag %d"
#define RR_ADAPTER_2619_112_2_18_0_9_41_1162 "RRA: camp param of is same as before, discard it, req_rr = %d"
#define RR_ADAPTER_2639_112_2_18_0_9_41_1163 "RRA: camp arfcn = 0x%x, req_rr = %d"
#define RR_ADAPTER_2854_112_2_18_0_9_41_1164 "RRA: exist active rr, rr %d donot send deactive to layer1."
#define RR_ADAPTER_2870_112_2_18_0_9_41_1165 "RRA: deactive req_rr = %d, s_rra_master_id = %d"
#define RR_ADAPTER_2919_112_2_18_0_9_41_1166 "RRA: send main campon to layer1, master rr = %d"
#define RR_ADAPTER_2934_112_2_18_0_9_41_1167 "RRA: layer1 enter deactive."
#define RR_ADAPTER_2948_112_2_18_0_9_41_1168 "RRA: MPH_DEACTIVATE_REQ discarded in rat change, rr %d"
#define RR_ADAPTER_2977_112_2_18_0_9_41_1169 "RRA: rr %d enter RRA_RR_NULL, master id = %d"
#define RR_ADAPTER_3000_112_2_18_0_9_42_1170 "RRA: HandleMphDataInd Param4(0x%x)\n"
#define RR_ADAPTER_3003_112_2_18_0_9_42_1171 "RRA: HandleMphDataInd cardid(0x%x),frameno(0x%x)\n"
#define RR_ADAPTER_3024_112_2_18_0_9_42_1172 "RRA: HandleMphDataInd i(0x%x)\n"
#define RR_ADAPTER_3101_112_2_18_0_9_42_1173 "RRA: dis card rrc %d page mode %d."
#define RR_ADAPTER_3123_112_2_18_0_9_42_1174 "RRA: send MPH_BCCH_CAMP_REQ, rr = %d"
#define RR_ADAPTER_3219_112_2_18_0_9_42_1175 "HandleRandomAccessReq:there exist ManualPLMNSelect or ModeChange or NasRatChange or SynchronizeModeChange"
#define RR_ADAPTER_3247_112_2_18_0_9_42_1176 "HandleRandomAccessReq:rr %d RA cause is %d"
#define RR_ADAPTER_3262_112_2_18_0_9_42_1177 "HandleRandomAccessReq:rr %d last RA cause is %d"
#define RR_ADAPTER_3317_112_2_18_0_9_42_1178 "RRA: rr %d rach delay 1s in order to waiting another card terminate its ps service."
#define RR_ADAPTER_3329_112_2_18_0_9_42_1179 "RRA: rr %d rach delay 1s while in rf scan protect."
#define RR_ADAPTER_3390_112_2_18_0_9_42_1180 "RRA: send 4 MPH_BCCH_CAMP_REQ, rr = %d"
#define RR_ADAPTER_3422_112_2_18_0_9_42_1181 "RRA: send RRA_MPH_RANDOM_ACCESS_REQ, rr = %d"
#define RR_ADAPTER_3431_112_2_18_0_9_42_1182 "RRA: xxrr %d enter RRA_RR_TRAFFIC."
#define RR_ADAPTER_3511_112_2_18_0_9_43_1183 "RRA: rr %d enter RRA_RR_IDLE."
#define RR_ADAPTER_3738_112_2_18_0_9_43_1184 "RRA: do not send %d meas report while in mode change, manual plmn sel etc."
#define RR_ADAPTER_3749_112_2_18_0_9_43_1185 "RRA: do not send %d meas report while in rf scan protect."
#define RR_ADAPTER_3899_112_2_18_0_9_43_1186 "RRA: ba combinate exceed, discard some info."
#define RR_ADAPTER_4138_112_2_18_0_9_44_1187 "RRA: act gprs = %d"
#define RR_ADAPTER_4262_112_2_18_0_9_44_1188 "RRA: DeliverToRRCSig rr id = %d"
#define RR_ADAPTER_4407_112_2_18_0_9_44_1189 "RRA: receive PBCCH CAMP REQ but can't do pbcch camp, because another exist"
#define RR_ADAPTER_4422_112_2_18_0_9_44_1190 "RRA: enter PBCCH CAMP"
#define RR_ADAPTER_4432_112_2_18_0_9_44_1191 "RRA: clear mac rlc flag %d"
#define RR_ADAPTER_4438_112_2_18_0_9_44_1192 "RRA: pbcch camp arfcn = 0x%x, req_rr = %d"
#define RR_ADAPTER_4549_112_2_18_0_9_45_1193 "RRA: handoverReq req_rr %d, arfcn_before_handover %d, camp_arfcn %d"
#define RR_ADAPTER_4616_112_2_18_0_9_45_1194 "RRA: assert case, exp ind sig = 0x%x."
#define RR_ADAPTER_4655_112_2_18_0_9_45_1195 "RRA: rach delay exp, master id = %d"
#define RR_ADAPTER_4709_112_2_18_0_9_45_1196 "RRA: rr to mac, s_rra_gprs_id = 0x%x"
#define RR_ADAPTER_4811_112_2_18_0_9_45_1197 "RRA: discard immed assign!"
#define RR_ADAPTER_4835_112_2_18_0_9_45_1198 "RRA: discard mac rach rsp. rr = %d, s_rra_gprs_id = %d"
#define RR_ADAPTER_4881_112_2_18_0_9_45_1199 "RRA: rr to rlc, s_rra_gprs_id = 0x%x"
#define RR_ADAPTER_4929_112_2_18_0_9_46_1200 "RRA: handle deact req, req_rr = %d, s_rra_master_id = %d, s_rra_gprs_id = %d"
#define RR_ADAPTER_4934_112_2_18_0_9_46_1201 "RRA: rr %d state = %d"
#define RR_ADAPTER_4955_112_2_18_0_9_46_1202 "RRA: invalid s_rra_gprs_id."
#define RR_ADAPTER_4967_112_2_18_0_9_46_1203 "RRA: use rr %d param to act rlc."
#define RR_ADAPTER_4996_112_2_18_0_9_46_1204 "RRA: SetGprsParaReq,req_rr = %d, gprs id = %d"
#define RR_ADAPTER_5017_112_2_18_0_9_46_1205 "RRA: rlc act flag %d"
#define RR_ADAPTER_5127_112_2_18_0_9_46_1206 "RRA: latest traffic id %d"
#define RR_ADAPTER_5199_112_2_18_0_9_46_1207 "RRA: rf scan ind: rr %d in state %d"
#define RR_ADAPTER_5280_112_2_18_0_9_46_1208 "RRA: copy rf scan num = %d"
#define RR_ADAPTER_5313_112_2_18_0_9_47_1209 "RRA: layer1 rach expired, master id = %d"
#define RR_ADAPTER_5344_112_2_18_0_9_47_1210 "RRA: start rf scan protect timer."
#define RR_ADAPTER_5495_112_2_18_0_9_47_1211 "RRA: mac freq %d, type = %d"
#define RR_ADAPTER_5519_112_2_18_0_9_47_1212 "RRA: unknow mac freq type %d"
#define RR_ADAPTER_5791_112_2_18_0_9_48_1213 "RRA: bcch camp req_rr = %d, before camp, s_rra_master_id = %d"
#define RR_ADAPTER_6170_112_2_18_0_9_48_1214 "RRA: recv unexpected tc = 0."
#define RR_ADAPTER_6245_112_2_18_0_9_49_1215 "RRA: bcch list decode arfcn = 0x%x, band = %d, tc in = 0x%x, out = 0x%x"
#define RR_ADAPTER_6444_112_2_18_0_9_49_1216 "RRA: unknow signal 0x%x,free."
#define RR_ADAPTER_6499_112_2_18_0_9_49_1217 "RRA: invalid arfcn index, BAND %d, arfcn %d"
#define RR_ADAPTER_6534_112_2_18_0_9_49_1218 "RRA: HandleMphChangeModeReq rr %d, rr_task_id = 0x%x"
#define RR_ADAPTER_6535_112_2_18_0_9_49_1219 "RRA: HandleMphChangeModeReq rr_state %d"
#define RR_ADAPTER_6538_112_2_18_0_9_49_1220 "RRA: HandleMphChangeModeReq rr %d, rr_task_id = 0x%x"
#define RR_ADAPTER_6628_112_2_18_0_9_49_1221 "RRA: xxrr %d enter pseudo RRA_RR_TRAFFIC."
#define RR_ADAPTER_6680_112_2_18_0_9_49_1222 "RRA: process dummy rat change to TD!"
#define RR_ADAPTER_6773_112_2_18_0_9_50_1223 "RRA: HandleMphChangeModeReq to send grr(%d) all zero measure report!"
#define RR_ADAPTER_6798_112_2_18_0_9_50_1224 "RRA: nas_rat_change_flag = TRUE when no one knows it begins rat change!"
#define RR_ADAPTER_6825_112_2_18_0_9_50_1225 "RRA: s_rra_curr_work_mode = %d, when change mode!!!"
#define RR_ADAPTER_6872_112_2_18_0_9_50_1226 "RRA: rr %d rach delay 1s while in rf scan protect."
#define RR_ADAPTER_7039_112_2_18_0_9_50_1227 "RRA: xxrr %d enter RRA_RR_TRAFFIC."
#define RR_ADAPTER_7166_112_2_18_0_9_51_1228 "RRA: HandleMphStartTdMeasReq rr %d, rr_task_id = 0x%x"
#define RR_ADAPTER_7167_112_2_18_0_9_51_1229 "RRA: HandleMphStartTdMeasReq rr_state %d"
#define RR_ADAPTER_7243_112_2_18_0_9_51_1230 "RRA: HandleMphUpdateTdMeasReq rr %d, rr_task_id = 0x%x"
#define RR_ADAPTER_7244_112_2_18_0_9_51_1231 "RRA: HandleMphUpdateTdMeasReq rr_state %d"
#define RR_ADAPTER_7247_112_2_18_0_9_51_1232 "RRA: HandleMphUpdateTdMeasReq td_meas_start_flag is false!"
#define RR_ADAPTER_7317_112_2_18_0_9_51_1233 "RRA: HandleMphStopTdMeasReq rr %d, rr_task_id = 0x%x"
#define RR_ADAPTER_7318_112_2_18_0_9_51_1234 "RRA: HandleMphStopTdMeasReq rr_state %d"
#define RR_ADAPTER_7321_112_2_18_0_9_51_1235 "RRA: HandleMphStopTdMeasReq td_meas_start_flag is false!"
#define RR_ADAPTER_7395_112_2_18_0_9_51_1236 "RRA: rra_combine_td_measurement A+B combine OK : req_rr %d;  arfcn %d "
#define RR_ADAPTER_7416_112_2_18_0_9_51_1237 "RRA: rra_combine_td_measurement combine OK : req_rr %d;  arfcn %d, cell_param = 0x%x"
#define RR_ADAPTER_7428_112_2_18_0_9_51_1238 "RRA: rra_combine_td_measurement combine failed, reach MAX_CELL_PARAM_ID_PER_ARFCN : req_rr %d;  arfcn %d, cell_param = 0x%x"
#define RR_ADAPTER_7439_112_2_18_0_9_51_1239 "RRA: rra_combine_td_measurement combine arfcn OK : req_rr %d;  arfcn %d"
#define RR_ADAPTER_7450_112_2_18_0_9_51_1240 "RRA: rra_combine_td_measurement combine arfcn failed, i_arfcn_combine reach MAX_CELL_PARAM_ID_PER_ARFCN : req_rr %d;"
#define RR_ADAPTER_7546_112_2_18_0_9_51_1241 "RRA: HandleTdMeasReportInd to s_rra_master_id %d;  cell_num %d; arfcn_num  %d"
#define RR_ADAPTER_7568_112_2_18_0_9_51_1242 "RRA: do not send %d TD meas report while in rat change or manual plmn select/search."
#define RR_ADAPTER_7581_112_2_18_0_9_51_1243 "RRA: do not send %d td meas report while in rf scan protect."
#define RR_ADAPTER_7648_112_2_18_0_9_52_1244 "RRA: HandleTdMeasReportInd to i_card %d;  cell_num %d; arfcn_num  %d"
#define RR_ADAPTER_7724_112_2_18_0_9_52_1245 "RRA:rat change to 2G,but 2G is not activated."
#define RR_ADAPTER_7786_112_2_18_0_9_52_1246 "RRA: rr handshake type is wrong, discarded the handshake request!"
#define RR_ADAPTER_7819_112_2_18_0_9_52_1247 "RRA: HandleRrRraGsm2TdResultReq -- failed, change mode rejected by RRA, rr = %d!"
#define RR_ADAPTER_7829_112_2_18_0_9_52_1248 "RRA: HandleRrRraGsm2TdResultReq -- enter pseudo traffic state, rr = %d!"
#define RR_ADAPTER_7907_112_2_18_0_9_52_1249 "RRA: no plmn happened in error condition to RR (%d)!"
#define RR_ADAPTER_7934_112_2_18_0_9_52_1250 "RRA: RR_RRA_NOTIFY_REQ error type param = %d!"
#define RR_ADAPTER_7957_112_2_18_0_9_52_1251 "RrcaRraStateNotify: rrca_notify_ptr, rrca_status = %d, camp_flag = %d, card_id = %d!"
#define RR_ADAPTER_7960_112_2_18_0_9_52_1252 "RrcaRraStateNotify: TRUE == rrca_notify_ptr->Param2!   TRUE = %d "
#define RR_ADAPTER_7971_112_2_18_0_9_52_1253 "RrcaRraStateNotify: before s_rra_rr_info[req_rr].rrca_status = %d!"
#define RR_ADAPTER_7980_112_2_18_0_9_52_1254 "RRA: nas rat change 2G->3G Failed, no plmn in 3G!"
#define RR_ADAPTER_8007_112_2_18_0_9_52_1255 "RrcaRraStateNotify: rrca_notify_ptr->Param1 != RRA_RRCA_STATUS_NO_CELL &&rrca_notify_ptr->Param2 "
#define RR_ADAPTER_8234_112_2_18_0_9_53_1256 "RRA: can't do bcch camp, because there is Mode Change"
#define RR_ADAPTER_8245_112_2_18_0_9_53_1257 "RRA: can't do bcch camp, because there is Mode Change"
#define RR_ADAPTER_8469_112_2_18_0_9_53_1258 "RRA_GetPsDisturbStatus: s_is_ps_disturb_by_other_cs %d"
#define SCT_PROC_87_112_2_18_0_9_54_1259 "SCT: Close tch loop type I"
#define SCT_PROC_92_112_2_18_0_9_54_1260 "SCT: cmd error C %d, B%d, A%d"
#define SCT_PROC_102_112_2_18_0_9_54_1261 "SCT: Close tch loop type F"
#define SCT_PROC_107_112_2_18_0_9_54_1262 "SCT loop type E"
#define SCT_PROC_113_112_2_18_0_9_54_1263 "SCT: Close tch loop type D"
#define SCT_PROC_120_112_2_18_0_9_54_1264 "SCT: Close tch loop type C"
#define SCT_PROC_127_112_2_18_0_9_54_1265 "SCT: Close tch loop type B"
#define SCT_PROC_132_112_2_18_0_9_54_1266 "SCT: Close tch loop type A"
#define SCT_PROC_141_112_2_18_0_9_54_1267 "SCT: Open loop command"
#define SCT_PROC_147_112_2_18_0_9_54_1268 "SCT: Test interfaec %d "
#define SCT_PROC_205_112_2_18_0_9_54_1269 "SCT: GPRS loop command"
#define SCT_PROC_216_112_2_18_0_9_54_1270 "SCT:EGPRS SRB loopback command"
#define SCT_PROC_250_112_2_18_0_9_54_1271 "SCT: Close tch loop ack"
#define SCT_PROC_263_112_2_18_0_9_54_1272 "SCT: Open tch loop ack"
#define CCENTITY_5433_112_2_18_0_10_18_1273 "CC: send start dtmf at %d staten"
#define CCENTITY_5478_112_2_18_0_10_18_1274 "CC: send stop dtmf at %d state n"
#define CMROUTER_337_112_2_18_0_10_22_1275 "cmrouter: Rel_PId, ti is %x\n"
#define CMROUTER_1746_112_2_18_0_10_24_1276 "cmrouter: deal MNSS_END_REQ, ti is %x, entity not exist\n"
#define CMROUTER_2192_112_2_18_0_10_25_1277 "cmrouter: deal MNSS_END_REQ, ti is %x, entity not exist\n"
#define CMROUTER_2610_112_2_18_0_10_26_1278 "cmrouter: deal MNSS_END_REQ, ti is %x, entity not exist\n"
#define DLR_272_112_2_18_0_10_36_1279 "DL: release req 0,but no entity"
#define DLR_296_112_2_18_0_10_36_1280 "DL: release req 3,but no entity"
#define DLR_458_112_2_18_0_10_37_1281 "L2: unexpected sapi\n"
#define DLR_776_112_2_18_0_10_37_1282 "PH_READY_TO_SEND_IND recved, but task released\n"
#define GMM_2185_112_2_18_0_10_42_1283 "GMM: deregistered gmm_state =%d"
#define GMM_2381_112_2_18_0_10_42_1284 "GMM:SDL cell_status = %d"
#define GMM_2509_112_2_18_0_10_42_1285 "GMM:SDL return_state =%d"
#define GMM_2586_112_2_18_0_10_43_1286 "GMM:Ignore msg_type = %d"
#define GMM_2845_112_2_18_0_10_43_1287 "GMM:SDL Error gmm_state =%d"
#define GMM_2991_112_2_18_0_10_44_1288 "APP_GMM_DETACH_REQ:g_detach_type=%d"
#define GMM_3002_112_2_18_0_10_44_1289 "GMM error:g_detach_type=%d"
#define GMM_3124_112_2_18_0_10_44_1290 "GMM: error g_gmm_state is %d"
#define GMM_3261_112_2_18_0_10_44_1291 "GMM: error cell_status is %d"
#define GMM_3408_112_2_18_0_10_45_1292 "GMM:wrong state =%d"
#define GMM_3515_112_2_18_0_10_45_1293 "GMM:free sm unitdata request!"
#define GMM_3535_112_2_18_0_10_45_1294 "GMM:free sm unitdata request in states!"
#define GMM_3815_112_2_18_0_10_46_1295 "GMM:T3312 is deactivated and the MS shall not perform periodic routing area updating!"
#define GMM_3860_112_2_18_0_10_46_1296 "GMM:T3312 is deactivated and the MS shall not perform periodic routing area updating!"
#define GMM_3979_112_2_18_0_10_46_1297 "GMM:Igore msg_type = %d"
#define GMM_4111_112_2_18_0_10_46_1298 "impossible g_detach_type"
#define GMM_4659_112_2_18_0_10_47_1299 "Don't received the signal"
#define GMM_4764_112_2_18_0_10_47_1300 "GMM:Igore msg_type = %d"
#define GMM_4942_112_2_18_0_10_48_1301 "GMM:Assert Failure,gmm_state =%d"
#define GMM_5109_112_2_18_0_10_48_1302 "GMM:Assert Error,g_detach_type=%d"
#define GMM_5158_112_2_18_0_10_48_1303 "GMM:T3312 is deactivated and the MS shall not perform periodic routing area updating!"
#define GMM_5295_112_2_18_0_10_49_1304 "GMM:Ignore msg_type =%d"
#define GMM_5385_112_2_18_0_10_49_1305 "GMM:Assert Error,T3321 g_detach_type =%d"
#define GMM_5391_112_2_18_0_10_49_1306 "GMM: NOT recieve DETACH ACC,  T3321expire 4t imes"
#define GMM_5515_112_2_18_0_10_49_1307 "GMM:Assert,Error gmm_state =%d"
#define GMM_5679_112_2_18_0_10_50_1308 "GMM: resume GMM, stop Detach"
#define GMM_5917_112_2_18_0_10_50_1309 "GMM:Ack APP Detach for poweroff"
#define GMM_5941_112_2_18_0_10_50_1310 "GMM:MN Update plmn forbid list"
#define GMM_5982_112_2_18_0_10_50_1311 "GMM:handle_rau_reject_sdl attach fail"
#define GMM_6039_112_2_18_0_10_50_1312 "GMM:detach_reattach fail"
#define GMM_6132_112_2_18_0_10_50_1313 "GMM: error g_gmm_state is %d"
#define GMM_6303_112_2_18_0_10_51_1314 "GMM: SDL 1 return_state =%d"
#define GMM_6424_112_2_18_0_10_51_1315 "GMM:SDL Assert,g_detach_type =%d"
#define GMM_6430_112_2_18_0_10_51_1316 "DECODE DETACH_ACCEPT FAIL"
#define GMM_6615_112_2_18_0_10_51_1317 "GMM:Assert Error,return_state=%d"
#define GMM_6700_112_2_18_0_10_52_1318 "GMM: recieve DETACH ACC, stop T3321"
#define GMM_6743_112_2_18_0_10_52_1319 "GMM:Assert Error,g_detach_type = %d"
#define GMM_6798_112_2_18_0_10_52_1320 "GMM:decode detach accept fail."
#define GMM_6838_112_2_18_0_10_52_1321 "GMM:Assert Error,any state g_gmm_state =%d"
#define GMM_7012_112_2_18_0_10_52_1322 "GMM:Enter is_message_allowed_to_send_sdl"
#define GMM_7016_112_2_18_0_10_52_1323 "GMM:access is barred"
#define GMM_7029_112_2_18_0_10_52_1324 "GMM:gmm is suspended"
#define GMM_7044_112_2_18_0_10_52_1325 "GMM:g_reg_substate= REG_UPDATE_NEEDED"
#define GMM_7080_112_2_18_0_10_52_1326 "GMM:is message,ATTACH_NEEDED"
#define GMM_7140_112_2_18_0_10_52_1327 "GMM:substate isnot allowed to attach"
#define GMM_7271_112_2_18_0_10_53_1328 "GMM: Ready to Attach,Attach Type = %d! "
#define GMM_7385_112_2_18_0_10_53_1329 "GMM:not allowed to attach "
#define GMM_7421_112_2_18_0_10_53_1330 "GMM: ptmsi_flag_pro =%d ,tmsi_is= %d"
#define GMM_7649_112_2_18_0_10_53_1331 "GMM:Decode rau reject err"
#define GMM_8313_112_2_18_0_10_55_1332 "GMM:call handle_rau_req_sdl failure"
#define GMM_8503_112_2_18_0_10_55_1333 "GMM:plmn mismatch!"
#define GMM_8698_112_2_18_0_10_55_1334 "GMM: Assert Failure, wrong g_gmm_state = %d"
#define GMM_9053_112_2_18_0_10_57_1335 "Gmm: rauattemp count > 5"
#define GMM_9662_112_2_18_0_10_58_1336 "GMM:rau accept rai mismatch!"
#define GMM_9870_112_2_18_0_10_59_1337 "GMM:error rau_accept!update_result"
#define GMM_9882_112_2_18_0_10_59_1338 "GMM:RAU type mismatch!"
#define GMM_9919_112_2_18_0_10_59_1339 "GMM:decode error"
#define GMM_10057_112_2_18_0_10_59_1340 "GMM:send_detach_req_sdl return false"
#define GMM_10112_112_2_18_0_11_0_1341 "GMM:Receive Ready Timer Run Forever!"
#define GMM_10218_112_2_18_0_11_0_1342 "GMM:T3312 is deactivated and the MS shall not perform periodic routing area updating !"
#define GMM_10558_112_2_18_0_11_1_1343 "GMM:Attach type mismatch!"
#define GMM_10562_112_2_18_0_11_1_1344 "GMM:Attach result value is reserved!"
#define GMM_10604_112_2_18_0_11_1_1345 "GMM:attach accept rai mismatch!"
#define GMM_10750_112_2_18_0_11_1_1346 "GMM error:g_detach_type=%d"
#define GMM_10882_112_2_18_0_11_2_1347 "GMM:Attach result value is reserved!"
#define GMM_10891_112_2_18_0_11_2_1348 "GMM:Decode failure"
#define GMM_10988_112_2_18_0_11_2_1349 "GMM:Decode attach reject message is err"
#define GMM_11370_112_2_18_0_11_3_1350 "GMM:enter send_detach_req_sdl"
#define GMM_11486_112_2_18_0_11_3_1351 "GMM:don't send message"
#define GMM_11629_112_2_18_0_11_3_1352 "GMM:detach_req!detach_type =%d"
#define GMM_12021_112_2_18_0_11_4_1353 "GMM:invalid detach cause %d,yPrdVarP->z0000040R3YE_detach_req.gmm_cause"
#define GMM_12239_112_2_18_0_11_4_1354 "GMM: DecodeAuthCiphRequest error = %d"
#define GMM_12445_112_2_18_0_11_5_1355 "GMM:in FPLMN list"
#define GMM_12474_112_2_18_0_11_5_1356 "GMM:LAI in Forbidded list"
#define GMM_12528_112_2_18_0_11_5_1357 "GMM:lai in Forbidded list"
#define GMM_12649_112_2_18_0_11_5_1358 "GMM: Can't handle this PD, discard it."
#define LAYER1_629_112_2_18_0_11_47_1359 "deactive req.\n"
#define LAYER1_702_112_2_18_0_11_47_1360 "rxpower_sweep_req asked and confirm replied.\n"
#define LAYER1_720_112_2_18_0_11_47_1361 "rxpower from BS come.\n"
#define LAYER1_738_112_2_18_0_11_47_1362 "bcch req asked.\n"
#define LAYER1_749_112_2_18_0_11_47_1363 "the bcch req is single %d.\n"
#define LAYER1_771_112_2_18_0_11_47_1364 "bcch req stop.\n"
#define LAYER1_788_112_2_18_0_11_47_1365 "bcch timer expired.\n"
#define LAYER1_823_112_2_18_0_11_47_1366 "bsic req asked.\n"
#define LAYER1_899_112_2_18_0_11_47_1367 "\n*****Layer1 Rx BCCH:*****\n"
#define LAYER1_900_112_2_18_0_11_47_1368 "BCCH_ARFCN:       %d \n\n"
#define LAYER1_941_112_2_18_0_11_47_1369 "\n*****L1Sim Rx BCCH:*****\n"
#define LAYER1_942_112_2_18_0_11_47_1370 "BCCH_ARFCN:%d \n\n"
#define LAYER1_985_112_2_18_0_11_47_1371 "bcch ind true\n"
#define LAYER1_1146_112_2_18_0_11_48_1372 "campon req&confirmed.\n"
#define LAYER1_1244_112_2_18_0_11_48_1373 "TBF Downlink Assignment Recv\n"
#define LAYER1_1317_112_2_18_0_11_48_1374 "Send the first RA\n"
#define LAYER1_1391_112_2_18_0_11_48_1375 "L1 Send Random Acess Burst\n"
#define LAYER1_1445_112_2_18_0_11_48_1376 "AGCH recv\n"
#define LAYER1_1717_112_2_18_0_11_49_1377 "PLMN search asked and  rxpower confirm replied.\n"
#define LAYER1_2273_112_2_18_0_11_50_1378 "chan_assign:%d\n"
#define LAYER1_2315_112_2_18_0_11_50_1379 "chan_assign_fail:%d\n"
#define LAYER1_2398_112_2_18_0_11_50_1380 "handover_fail:%d\n"
#define LAYER1_2512_112_2_18_0_11_50_1381 "Received Data block from new DCCH in non-syn handover\n"
#define LAYER1_2521_112_2_18_0_11_50_1382 "handover :%d\n"
#define LAYER1_2596_112_2_18_0_11_50_1383 "handover_fail:%d\n"
#define LAYER1_2645_112_2_18_0_11_51_1384 "L1:sacch data recv\n"
#define LAYER1_2667_112_2_18_0_11_51_1385 "l1 recv data:%d\n"
#define LAYER1_2747_112_2_18_0_11_51_1386 "L1:data send at chan %d\n"
#define LAYER1_2769_112_2_18_0_11_51_1387 "L1:sacch data send\n"
#define LAYER1_2963_112_2_18_0_11_51_1388 "This bcch block is unexpected\n"
#define LAYER1_3001_112_2_18_0_11_51_1389 "bcch ind,true\n"
#define LAYER1_3008_112_2_18_0_11_51_1390 "bcch ind,not requested\n"
#define LAYER1_3121_112_2_18_0_11_51_1391 "SI_ID: 1"
#define LAYER1_3125_112_2_18_0_11_51_1392 "SI_ID: 2"
#define LAYER1_3129_112_2_18_0_11_51_1393 "SI_ID: 2Bis"
#define LAYER1_3133_112_2_18_0_11_51_1394 "SI_ID: 2Ter"
#define LAYER1_3137_112_2_18_0_11_51_1395 "SI_ID: 3"
#define LAYER1_3141_112_2_18_0_11_51_1396 "SI_ID: 4"
#define LAYER1_3145_112_2_18_0_11_51_1397 "SI_ID: 13"
#define LAYER1_3149_112_2_18_0_11_52_1398 "SI_ID: 5BIS"
#define LAYER1_3153_112_2_18_0_11_52_1399 "SI_ID: 5BIS"
#define LAYER1_3157_112_2_18_0_11_52_1400 "SI_ID: 6"
#define LAYER1_3161_112_2_18_0_11_52_1401 "SI_ID: 7"
#define LAYER1_3165_112_2_18_0_11_52_1402 "PSI_ID: 13"
#define LAYER1_3169_112_2_18_0_11_52_1403 "SI_ID: 4"
#define LAYER1_3173_112_2_18_0_11_52_1404 "PSI_ID: 3Bis"
#define LAYER1_3177_112_2_18_0_11_52_1405 "SI_ID: 8"
#define LAYER1_3181_112_2_18_0_11_52_1406 "SI_ID: 9"
#define LAYER1_3185_112_2_18_0_11_52_1407 "SI_ID: 13"
#define LAYER1_3189_112_2_18_0_11_52_1408 "PSI_ID: 1"
#define LAYER1_3193_112_2_18_0_11_52_1409 "PSI_ID: 2"
#define LAYER1_3197_112_2_18_0_11_52_1410 "PSI_ID: 3 "
#define LLENTITY_876_112_2_18_0_11_54_1411 "LLentity:impossible U frame type\n"
#define LLENTITY_1077_112_2_18_0_11_54_1412 "LLentity:impossible I frame type\n"
#define LLENTITY_1186_112_2_18_0_11_54_1413 "LLentity:impossible S frame type\n"
#define LLENTITY_1293_112_2_18_0_11_54_1414 "LLentity:recvd SNDCP ACK pdu in unack mode!\n"
#define LLENTITY_2499_112_2_18_0_11_57_1415 "LLentity:unexpected value\n"
#define LLENTITY_3047_112_2_18_0_11_58_1416 "LLE:received invalid command on ADM state"
#define LLMAIN_676_112_2_18_0_12_0_1417 "LLMAIN: discard l3 pdu, tlli mismatch"
#define LLMAIN_918_112_2_18_0_12_1_1418 "LLc Frame Error %d"
#define LLMAIN_1303_112_2_18_0_12_2_1419 "LLc2 Frame Error %d"
#define LLMAIN_1367_112_2_18_0_12_2_1420 "LLMAIN:discard l3 pdu, tlli mismatch,sapi %d\n"
#define LLMAIN_1775_112_2_18_0_12_3_1421 "LLMAIN: discard pdu, wrong state"
#define LLMAIN_1818_112_2_18_0_12_3_1422 "LLMAIN: discard pdu, wrong state"
#define LLMAIN_1842_112_2_18_0_12_3_1423 "LLMAIN: discard l3 pdu,wrong state"
#define LLMAIN_1864_112_2_18_0_12_3_1424 "LLMAIN: discard l3 pdu,wrong state"
#define MAC_500_112_2_18_0_12_5_1425 "MAC:try to start t3192 0 ms"
#define MAC_960_112_2_18_0_12_5_1426 "MAC: Decode grr freq req error!\n"
#define MAC_997_112_2_18_0_12_5_1427 "MAC: DL imm assi,freq Acquare Sys Info error"
#define MAC_1166_112_2_18_0_12_6_1428 "MAC: DL imm assi,Decode ia rest octets error!"
#define MAC_1187_112_2_18_0_12_6_1429 "MAC: DL imm assi,Decode chan descri error!"
#define MAC_1245_112_2_18_0_12_6_1430 "MAC: recv pkt dl ass from rrc alloc no dl slot! Ignore pdch dl req!\n"
#define MAC_1262_112_2_18_0_12_6_1431 "MAC: Decode freq para error! Acquare Sys Info"
#define MAC_1531_112_2_18_0_12_7_1432 "MAC: Multislot capability not match! Ignore pdch dl req!"
#define MAC_1539_112_2_18_0_12_7_1433 "MAC: TBF_MODE in packet downlink ass is different from current!wrong! \n"
#define MAC_1545_112_2_18_0_12_7_1434 "MAC: No tfi assign,ignore the packet DL assi from ppch!"
#define MAC_1552_112_2_18_0_12_7_1435 "MAC: Decode packet DL error in idle, ignore the msg!"
#define MAC_1737_112_2_18_0_12_7_1436 "MAC: Decode rach response error! Acquare Sys Info"
#define MAC_1756_112_2_18_0_12_7_1437 "MAC: One phase received Dl single assi!\n"
#define MAC_2092_112_2_18_0_12_8_1438 "MAC: Single block without TBF!"
#define MAC_2219_112_2_18_0_12_8_1439 "MAC: One phase received Dl assi!"
#define MAC_2226_112_2_18_0_12_8_1440 "MAC: One phase received Dl EGPRS assi!"
#define MAC_2244_112_2_18_0_12_8_1441 "MAC: Decode rach response error! Rach retry!"
#define MAC_2285_112_2_18_0_12_8_1442 "MAC: wrong condition receive rach rsp"
#define MAC_2321_112_2_18_0_12_8_1443 "MAC: Rach response reject, Rach retry!"
#define MAC_2374_112_2_18_0_12_8_1444 "MAC: Rach response reject, ul tbf failure!\n"
#define MAC_2485_112_2_18_0_12_9_1445 "MAC: not support GRR_MAC_PDCH_ASSIGN_REQ"
#define MAC_2637_112_2_18_0_12_9_1446 "MAC: access abort invalid"
#define MAC_2690_112_2_18_0_12_9_1447 "MAC: access abort invalid"
#define MAC_2990_112_2_18_0_12_10_1448 "MAC: tbf mode invalid in connect cnf %d"
#define MAC_3055_112_2_18_0_12_10_1449 "MAC: Ul release, send ctrl ack request new TBF!"
#define MAC_3066_112_2_18_0_12_10_1450 "MAC: Ul release, send res req, request new TBF!"
#define MAC_3071_112_2_18_0_12_10_1451 "MAC: Ul release, unexpected ul_exp_action!!"
#define MAC_3159_112_2_18_0_12_10_1452 "MAC: receive ul_rel_req but ul not active"
#define MAC_3226_112_2_18_0_12_10_1453 "MAC:Assert Failure,tbf mode invalid in rlc_mac_rel_req"
#define MAC_3332_112_2_18_0_12_10_1454 "MAC:Assert Failure,release both link tbf but not in rel state"
#define MAC_3360_112_2_18_0_12_10_1455 "MAC:tbf mode invalid,uplink tbf starting time?"
#define MAC_3485_112_2_18_0_12_11_1456 "MAC:rlc_mac_tbf_req in ul_active!!"
#define MAC_3494_112_2_18_0_12_11_1457 "MAC:ignore rlc tbf req when prach have been send but not receive ass!"
#define MAC_3600_112_2_18_0_12_11_1458 "MAC:rlc realloc req but ul not active"
#define MAC_3706_112_2_18_0_12_11_1459 "MAC: wrong state 3186 expire"
#define MAC_3717_112_2_18_0_12_11_1460 "MAC: T3170 expiry! T3170 Dur: %d \n"
#define MAC_3814_112_2_18_0_12_11_1461 "MAC: wrong state t3170 expire"
#define MAC_3880_112_2_18_0_12_11_1462 "MAC: T3168 expiry! T3168 Dur: %d\n"
#define MAC_3996_112_2_18_0_12_12_1463 "MAC: wrong state rev t3168 expire"
#define MAC_4113_112_2_18_0_12_12_1464 "MAC: wrong state rev t3164 expire"
#define MAC_4191_112_2_18_0_12_12_1465 "MAC: wrong state rev t3162 expire"
#define MAC_4208_112_2_18_0_12_12_1466 "MAC: wrong state rev t3180 expire"
#define MAC_4287_112_2_18_0_12_12_1467 "MAC: T3192 expiry! T3192 Dur: %d\n"
#define MAC_4578_112_2_18_0_12_13_1468 "MAC: Prach access not permitted!"
#define MAC_4617_112_2_18_0_12_13_1469 "MAC: IA allocation uplink 8psk, but MS not support"
#define MAC_4772_112_2_18_0_12_13_1470 "MAC: IA assignment egprs, but MS not support"
#define MAC_4884_112_2_18_0_12_13_1471 "MAC: Single block without TBF in EGPRS!\n"
#define MAC_5263_112_2_18_0_12_14_1472 "MAC: Mac mode mismatch! Ignore packet ul assi!"
#define MAC_5288_112_2_18_0_12_14_1473 "MAC: rev ul assi in ul idle or other invalid state"
#define MAC_5301_112_2_18_0_12_14_1474 "MAC: Mac mode mismatch! Ignore packet ul assi!\n"
#define MAC_5382_112_2_18_0_12_14_1475 "MAC: one_phase_access_error!!"
#define MAC_5400_112_2_18_0_12_14_1476 "MAC: 1ph pkt ul ass allocation uplink 8psk, but MS not support"
#define MAC_5462_112_2_18_0_12_14_1477 "MAC: Decode multiblock allocation error! Rach retry!\n"
#define MAC_5712_112_2_18_0_12_15_1478 "MAC: Contention failure!\n"
#define MAC_5839_112_2_18_0_12_15_1479 "MAC: EGPRS invalid one phase state when rev ul assi"
#define MAC_5845_112_2_18_0_12_15_1480 "MAC: Decode Dynamic allocation error! Rach retry!\n"
#define MAC_5852_112_2_18_0_12_15_1481 "MAC: fixed allocation error! Rach retry!\n"
#define MAC_5861_112_2_18_0_12_15_1482 "MAC: 1ph pkt ul ass assignment egprs, but MS not support"
#define MAC_5886_112_2_18_0_12_15_1483 "MAC: Frequency not in band! Rach retry!\n"
#define MAC_5971_112_2_18_0_12_15_1484 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_5976_112_2_18_0_12_15_1485 "MAC: decode freq error return invalid value"
#define MAC_6001_112_2_18_0_12_16_1486 "MAC: 2ph allocation uplink 8psk, but MS not support"
#define MAC_6021_112_2_18_0_12_16_1487 "MAC: wrong assignment in second phase access"
#define MAC_6246_112_2_18_0_12_16_1488 "MAC: Decode allocation error! Rach retry!\n"
#define MAC_6253_112_2_18_0_12_16_1489 "MAC: Ingore the egprs ul assi in egprs_ul_assi_two_phase"
#define MAC_6261_112_2_18_0_12_16_1490 "MAC: 2ph assignment egprs, but MS not support"
#define MAC_6291_112_2_18_0_12_16_1491 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_6355_112_2_18_0_12_16_1492 "MAC: Frequency not in band! Rach retry!\n"
#define MAC_6361_112_2_18_0_12_16_1493 "MAC: decode freq error return invalid value"
#define MAC_6371_112_2_18_0_12_16_1494 "MAC: false tbf mode"
#define MAC_6385_112_2_18_0_12_16_1495 "MAC: ul active allocation uplink 8psk, but MS not support"
#define MAC_6394_112_2_18_0_12_16_1496 "MAC: false ul assignment in LABEL_UL_ASSI_UL_ACTIVE"
#define MAC_6556_112_2_18_0_12_17_1497 "MAC: Decode allocation error in LABEL_UL_ASSI_UL_ACTIVE! Rach retry!\n"
#define MAC_6565_112_2_18_0_12_17_1498 "MAC: Ingore the ul assi msg beacause lose dynamic allo in LABEL_UL_ASSI_UL_ACTIVE!"
#define MAC_6573_112_2_18_0_12_17_1499 "MAC: ul active assignment egprs, but MS not support"
#define MAC_6583_112_2_18_0_12_17_1500 "MAC: rev single block assi when ul active"
#define MAC_6638_112_2_18_0_12_17_1501 "MAC: DL est when waiting L1 release cnf"
#define MAC_6661_112_2_18_0_12_17_1502 "MAC:Ignore packet dl assi when two phase or contention phase!"
#define MAC_6671_112_2_18_0_12_17_1503 "MAC: rev dl assi when ul access two phase or other state"
#define MAC_6696_112_2_18_0_12_17_1504 "MAC: in ul access recv pkt dl ass alloc no dl slot! Ignore the msg!\n"
#define MAC_6709_112_2_18_0_12_17_1505 "MAC: wrong state rev dl assi,ignore!"
#define MAC_6720_112_2_18_0_12_17_1506 "MAC:  dl assign DL_TBF is different with UL_TBF in LABEL_DL_ASSI_UL_ACTIVE! Ignore the msg!\n"
#define MAC_6735_112_2_18_0_12_17_1507 "MAC: Ignore without dl_tfi dl assign msg while T3192 run!\n"
#define MAC_6747_112_2_18_0_12_17_1508 "MAC: in dl active recv pkt dl ass alloc no dl slot! Ignore the msg!\n"
#define MAC_6761_112_2_18_0_12_17_1509 "MAC: currnet dl tbf is EGPRS,receive gprs downlink ass,ignore!\n"
#define MAC_6768_112_2_18_0_12_17_1510 "MAC: Rlc mode mismatch! Ignore dl ass msg in dl_active!"
#define MAC_6829_112_2_18_0_12_17_1511 "MAC:contention phase, ignore TS RECONFIG"
#define MAC_6847_112_2_18_0_12_17_1512 "MAC: error state to receive ts reconfig"
#define MAC_6865_112_2_18_0_12_17_1513 "MAC: Timeslot reconfig assigned  tbf mode confilit with current ul tbf mode!ignor it!\n"
#define MAC_6877_112_2_18_0_12_17_1514 "MAC: e_ch_coding_command in ts recofig_ul_active err, ignore it!"
#define MAC_6893_112_2_18_0_12_17_1515 "MAC: in ul active recv pkt ts reconfig allocation no dl slot! Rach retry!\n"
#define MAC_6944_112_2_18_0_12_17_1516 "MAC: Decode error! Acquare Sys Info\n"
#define MAC_6949_112_2_18_0_12_17_1517 "MAC: Decode freq error! UL release and Rach retry!\n"
#define MAC_6969_112_2_18_0_12_17_1518 "MAC: Decode allocation error! UL release and Rach retry!\n"
#define MAC_7205_112_2_18_0_12_18_1519 "MAC: Timeslot reconfig not include dl tfi! Ul release and Rach retry!\n"
#define MAC_7240_112_2_18_0_12_18_1520 "MAC: in dl active recv pkt ts reconfig allocation no dl slot! Rach retry!\n"
#define MAC_7335_112_2_18_0_12_18_1521 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_7340_112_2_18_0_12_18_1522 "MAC: wrong state when rev ts realloc assi"
#define MAC_7346_112_2_18_0_12_18_1523 "MAC: Decode freq error! Rach retry!\n"
#define MAC_7358_112_2_18_0_12_18_1524 "MAC: Timeslot reconfig assigned  tbf mode confilit with current dl tbf mode!ignor it!\n"
#define MAC_7370_112_2_18_0_12_18_1525 "MAC: e_ch_coding_command in ts recofig_dl_active err, ignore it!"
#define MAC_7395_112_2_18_0_12_18_1526 "MAC: Decode allocation error! Rach retry!"
#define MAC_7729_112_2_18_0_12_19_1527 "MAC: Timeslot reconfig not include dl tfi! Rach retry!\n"
#define MAC_7739_112_2_18_0_12_19_1528 "MAC: Rlc mode mismatch! Ignore ts reconfig msg in dl_active!"
#define MAC_7789_112_2_18_0_12_19_1529 "MAC: Timeslot reconfig assigned  tbf mode confilit with current dl_ul tbf mode!ignor it!\n"
#define MAC_7801_112_2_18_0_12_19_1530 "MAC: e_ch_coding_command in ts recofig_ul_dl_active err, ignore it!"
#define MAC_7820_112_2_18_0_12_19_1531 "MAC: in ul dl active recv pkt ts reconfig allocation no dl slot! Rach retry!\n"
#define MAC_7869_112_2_18_0_12_19_1532 "MAC: Decode error! Acquare Sys Info\n"
#define MAC_7874_112_2_18_0_12_19_1533 "MAC: Decode freq error! UL release and Rach retry!\n"
#define MAC_7894_112_2_18_0_12_19_1534 "MAC: Decode allocation error! Rach retry!"
#define MAC_8161_112_2_18_0_12_20_1535 "MAC: Rlc mode mismatch! Ignore ts reconfig msg in ul_dl_active!"
#define MAC_8171_112_2_18_0_12_20_1536 "MAC: EGPRS UL ACK_NACK assigned tbf mode conflict with currnt ul tbf mode, ignore it"
#define MAC_8184_112_2_18_0_12_20_1537 "MAC: e_ch_coding_command in EGPRS UL ACK_NACK err, ignore it!"
#define MAC_8427_112_2_18_0_12_21_1538 "MAC: wrong state rev ul ack"
#define MAC_8453_112_2_18_0_12_21_1539 "MAC: Ignore additional PACKET ACCESS REJECT messages!"
#define MAC_8758_112_2_18_0_12_22_1540 "MAC: Receive reject, UL release!\n"
#define MAC_8812_112_2_18_0_12_22_1541 "MAC: ignore PACKET_QUEUING_NOTIFICATION while waiting TBF Starting Time of ul ass!"
#define MAC_9081_112_2_18_0_12_23_1542 "MAC: wrong ul state recv Packet tbf realse msg!"
#define MAC_9250_112_2_18_0_12_23_1543 "MAC: recv PPR no affect"
#define MAC_9278_112_2_18_0_12_23_1544 "MAC: wrong PPR"
#define MAC_9678_112_2_18_0_12_24_1545 "MAC: wrong state rev t3166 expire"
#define MAC_9684_112_2_18_0_12_24_1546 "MAC: One phase received egprs ul assi£¬but we don't support EDGE!"
#define MAC_9928_112_2_18_0_12_25_1547 "MAC: Abnormal release with retry!"
#define MAC_9952_112_2_18_0_12_25_1548 "MAC: !!Impossible ul_release_state!!"
#define MAC_9995_112_2_18_0_12_25_1549 "MAC: Abnormal release with rach retry!"
#define MAC_10007_112_2_18_0_12_25_1550 "MAC: release_cnf for ul but ul not active,maybe hold for connect cnf"
#define MAC_10066_112_2_18_0_12_25_1551 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_10078_112_2_18_0_12_25_1552 "MAC: Frequency not in band! Rach retry!\n"
#define MAC_10084_112_2_18_0_12_25_1553 "MAC: decode freq error,invalid return value"
#define MAC_10107_112_2_18_0_12_25_1554 "MAC: wrong state when rev single block assi"
#define MAC_10279_112_2_18_0_12_26_1555 "MAC: Decode allocation error! Rach retry!"
#define MAC_10286_112_2_18_0_12_26_1556 "MAC: Ingore the ul assi in UL_ASSI_TWO_PHASE_ACCESS"
#define MAC_10365_112_2_18_0_12_26_1557 "MAC: Decode freq error! Acquare Sys Info"
#define MAC_10376_112_2_18_0_12_26_1558 "MAC: wrong state when rev dl assi"
#define MAC_10382_112_2_18_0_12_26_1559 "MAC: Decode freq error! Rach retry!"
#define MAC_10626_112_2_18_0_12_26_1560 "MAC: No tfi assign in dl assi! Rach retry!\n"
#define MAC_10633_112_2_18_0_12_26_1561 "MAC: Multislot capability not match! Rach retry!"
#define MAC_10644_112_2_18_0_12_26_1562 "MAC: in ul active recv pkt dl ass alloc no dl slot! Ignore the msg!\n"
#define MAC_10697_112_2_18_0_12_26_1563 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_10702_112_2_18_0_12_26_1564 "MAC: Decode freq error! Ignore the msg!"
#define MAC_10901_112_2_18_0_12_27_1565 "MAC: Dl assi not include tfi!"
#define MAC_10907_112_2_18_0_12_27_1566 "MAC: Multislot capability not match! Ignore the msg!\n"
#define MAC_10980_112_2_18_0_12_27_1567 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_11269_112_2_18_0_12_27_1568 "MAC: Decode error! acquare si!\n"
#define MAC_11281_112_2_18_0_12_27_1569 "MAC: Multislot capability not match! Ignore the msg!"
#define MAC_11289_112_2_18_0_12_28_1570 "MAC: Contention failure!"
#define MAC_11514_112_2_18_0_12_28_1571 "MAC: Single block without TBF!\n"
#define MAC_11643_112_2_18_0_12_28_1572 "MAC: invalid one phase state when rev single block assi"
#define MAC_11857_112_2_18_0_12_29_1573 "MAC: invalid one phase state when rev ul assi"
#define MAC_11863_112_2_18_0_12_29_1574 "MAC: Decode allocation error! Rach retry!\n"
#define MAC_11870_112_2_18_0_12_29_1575 "MAC: fixed allocation, rach retry"
#define PROCESSMM_1945_112_2_18_0_12_33_1576 "MM: read sim data error"
#define PROCESSMM_2204_112_2_18_0_12_33_1577 "MM:begin saved call"
#define PROCESSMM_2320_112_2_18_0_12_33_1578 "MM:begin saved call"
#define PROCESSMM_2814_112_2_18_0_12_34_1579 "MM: GSM service detach\n"
#define PROCESSMM_2834_112_2_18_0_12_34_1580 "MM_CALI: MN_MM_NREG_REQ in WAIT FOR MN REG\n"
#define PROCESSMM_3122_112_2_18_0_12_35_1581 "MM:begin saved call"
#define PROCESSMM_4819_112_2_18_0_12_38_1582 "MM:receive MM_PD MSG at WAIT_FOR_NETWORK_COMMAND\n"
#define PROCESSMM_7123_112_2_18_0_12_43_1583 "MM:begin saved call"
#define PROCESSMM_7752_112_2_18_0_12_44_1584 "MM:begin saved call"
#define PROCESSMM_9609_112_2_18_0_12_48_1585 "MM:begin saved call"
#define PROCESSMM_11333_112_2_18_0_12_51_1586 "MM_CALI: MN_MM_NREG_REQ in WAIT_FOR_RR_CONNECTION_MM\n"
#define PROCESSMM_11668_112_2_18_0_12_52_1587 "MM_CALI: MN_MM_NREG_REQ in other status, with mm connect\n"
#define PROCESSMM_12185_112_2_18_0_12_53_1588 "MM_CALI: MN_MM_NREG_REQ in MM_NO_CELL"
#define PROCESSMM_12251_112_2_18_0_12_53_1589 "MM_CALI: MN_MM_NREG_REQ in MM_NO_SIM"
#define PROCESSMM_13009_112_2_18_0_12_55_1590 "MM:begin saved call"
#define PROCESSMM_13068_112_2_18_0_12_55_1591 "MM: sim authentication error"
#define PROCESSMM_13139_112_2_18_0_12_55_1592 "MM:begin saved call"
#define PROCESSMM_13659_112_2_18_0_12_56_1593 "MM:begin saved call"
#define PROCESSMM_13961_112_2_18_0_12_57_1594 "MM:begin saved call"
#define PROCESSMM_14075_112_2_18_0_12_57_1595 "MM:begin saved call"
#define PROCESSMM_14094_112_2_18_0_12_57_1596 "MM:begin saved call"
#define PROCESSMM_14213_112_2_18_0_12_57_1597 "MM:begin saved call"
#define PROCESSMM_14832_112_2_18_0_12_58_1598 "MM:discard MM_RR_DATA_IND"
#define PROCESSMM_15159_112_2_18_0_12_59_1599 "MM_CALI: MN_MM_NREG_REQ in other status, with mm connect\n"
#define PROCESSMM_15260_112_2_18_0_12_59_1600 "MM_CALI: MN_MM_NREG_REQ in other status, without mm connect\n"
#define PROCESSMM_15377_112_2_18_0_12_59_1601 "MM:begin saved call"
#define PROCESSMM_15843_112_2_18_0_13_0_1602 "MM: current T3212 duration is %d"
#define PROCESSMM_16739_112_2_18_0_13_2_1603 "MM: There is a delay CM service\n"
#define RLC_733_112_2_18_0_13_6_1604 "rlc:remain llc tbf request!"
#define RLC_750_112_2_18_0_13_6_1605 "rlc:cell reselect tbf!"
#define RLC_969_112_2_18_0_13_6_1606 "rlc:Up Single Block Access!\n"
#define RLC_1108_112_2_18_0_13_7_1607 "rlc:Up TBF Access for EGPRS SCT!\n"
#define RLC_1115_112_2_18_0_13_7_1608 "RLC: SCT est fail, downlink tbf is not acitved!"
#define RLC_1955_112_2_18_0_13_8_1609 "RLC: recv data error in loop back mode"
#define RLC_1990_112_2_18_0_13_8_1610 "rlc:error data_ind when dl is not actived!"
#define RLC_2089_112_2_18_0_13_9_1611 "rlc:SCT end!\n"
#define RLC_2276_112_2_18_0_13_9_1612 "rlc:should not recv up release"
#define RLC_2506_112_2_18_0_13_10_1613 "rlc:Access Fail with CS Page!\n"
#define RLC_2908_112_2_18_0_13_11_1614 "rlc:Up tbf fail with dl tbf is esting!\n"
#define RLC_2912_112_2_18_0_13_11_1615 "RLC: est sigle block without tbf interrupt by dl tbf ass"
#define RLC_2955_112_2_18_0_13_11_1616 "rlc:Up tbf ok for single block!\n"
#define RLC_2982_112_2_18_0_13_11_1617 "rlc:IS Rejected Access!\n"
#define RLC_3004_112_2_18_0_13_11_1618 "RLC:SCT Test Mode B Ested!"
#define RLC_3011_112_2_18_0_13_11_1619 "RLC:SCT Test Mode A Ested!"
#define RLC_3033_112_2_18_0_13_11_1620 "RLC:EGPRS SCT Test  Ested!"
#define RLC_3228_112_2_18_0_13_11_1621 "rlc:mac retry,but no need!\n"
#define RLC_3262_112_2_18_0_13_12_1622 "rlc:Up tbf fail with t3172 in mac ul_active!\n"
#define RLC_3340_112_2_18_0_13_12_1623 "rlc:error normal release"
#define RLC_3529_112_2_18_0_13_12_1624 "rlc:Up fail and perform cell reselect!\n"
#define RLC_3580_112_2_18_0_13_12_1625 "rlc:Up tbf fail with t3172 in mac ul_active!\n"
#define RLC_3664_112_2_18_0_13_12_1626 "rlc:Up tbf fail with timer t3172!\n"
#define RLC_3730_112_2_18_0_13_12_1627 "rlc:Receive Second mac_rlc_tbf_cnf msg!"
#define RLC_3745_112_2_18_0_13_12_1628 "rlc:Acess state is not one or two phase!"
#define RRC_3853_112_2_18_0_13_29_1629 "RRC: unknown bsic cnf"
#define RRC_3920_112_2_18_0_13_29_1630 "RRC: ASSERT, research>6"
#define RRC_3939_112_2_18_0_13_29_1631 "RRC: unknown bsic fail"
#define RRC_6831_112_2_18_0_13_35_1632 "RRC: discard IA,because mm est saved"
#define RRC_6839_112_2_18_0_13_35_1633 "RRC: discard IA,because plmn list req"
#define RRC_9493_112_2_18_0_13_40_1634 "RR:receive_psi_resel Get Packet paging!!\n"
#define RRC_9883_112_2_18_0_13_41_1635 "RRC: assert error, not fastplmn"
#define RRC_10022_112_2_18_0_13_41_1636 "RRC: assert error, not fastplmn"
#define RRC_10491_112_2_18_0_13_42_1637 "RRC: unknown bsic cnf"
#define RRC_10570_112_2_18_0_13_43_1638 "RRC: unknown bsic fail"
#define RRC_11106_112_2_18_0_13_44_1639 "RR: discard immed_assig"
#define RRC_11139_112_2_18_0_13_44_1640 "RR: rx immediate rej"
#define RRC_11942_112_2_18_0_13_45_1641 "RRC wrong sapi %d"
#define RRC_12766_112_2_18_0_13_47_1642 "RRC: wait assign dcch est error in dl chan type"
#define RRC_13526_112_2_18_0_13_48_1643 "RRC wrong sapi %d"
#define RRC_16598_112_2_18_0_13_54_1644 "RRC: Receive data block on PPCH, discard it!"
#define RRC_17409_112_2_18_0_13_56_1645 "RRC: Decode PacketMeasureOrder err!"
#define RRC_17587_112_2_18_0_13_56_1646 "RRC: Packet cell change failure msg is sent, failure cause=%d"
#define RRC_17777_112_2_18_0_13_57_1647 "RRC: Decode PacketCellChangeOrder err!"
#define RRC_18099_112_2_18_0_13_57_1648 "RRC: Decode Pmo err!"
#define RRC_18955_112_2_18_0_13_59_1649 "RRC: ASSERT ERROR recv prach when bad"
#define RRC_21843_112_2_18_0_14_5_1650 "RRC: discard IA,because plmn list req"
#define RRC_22060_112_2_18_0_14_6_1651 "RRC: Receive data block on PPCH, discard it!"
#define RRC_22222_112_2_18_0_14_6_1652 "RRC: Decode pmo err!"
#define RRC_22300_112_2_18_0_14_6_1653 "RRC: find ncell fail"
#define RRC_22312_112_2_18_0_14_6_1654 "RRC: gmm state not ready"
#define RRC_22320_112_2_18_0_14_6_1655 "RRC: decode pcco fail"
#define RRC_23993_112_2_18_0_14_10_1656 "RRC: process saved ppch"
#define RRC_24082_112_2_18_0_14_10_1657 "RRC: proc saved pch "
#define RRC_24763_112_2_18_0_14_11_1658 "RRC: resel scell arfcn 0x%x"
#define RRC_24996_112_2_18_0_14_12_1659 "RRC: cell resel arfcn 0x%x"
#define RRC_25249_112_2_18_0_14_12_1660 " RRC: switch to another cell, clear PSI Msg Buffer \n"
#define RRC_25688_112_2_18_0_14_13_1661 "RRC: unknown bsic cnf"
#define RRC_26466_112_2_18_0_14_15_1662 "RR temp = %d"
#define RRC_28152_112_2_18_0_14_18_1663 "RRC: Decode Pmo err!"
#define RRC_28222_112_2_18_0_14_18_1664 "RRC: Receive data block on PPCH, discard it!"
#define SMENTITY_208_112_2_18_0_14_24_1665 "SM_GetSysId dual_sys = %d"
#define SMENTITY_1577_112_2_18_0_14_27_1666 "SMentity:sapi not match."
#define SMENTITY_1654_112_2_18_0_14_27_1667 "SMentity:wrong release_type:%d."
#define SMENTITY_1750_112_2_18_0_14_27_1668 "SMentity:Decode SUCCESS!\n"
#define SMENTITY_1891_112_2_18_0_14_28_1669 "SMentity:Decode SUCCESS!\n"
#define SMENTITY_1924_112_2_18_0_14_28_1670 "SMentity:wrong message!"
#define SMENTITY_1929_112_2_18_0_14_28_1671 "SMentity:Decode Mtmodifypdpreq/Reqpdpact error!\n"
#define SMENTITY_2091_112_2_18_0_14_28_1672 "SMentity:wrong release_type:%d."
#define SMENTITY_2178_112_2_18_0_14_28_1673 "SMentity:sapi not match."
#define SMENTITY_2369_112_2_18_0_14_29_1674 "SMentity:sapi not match."
#define SMENTITY_2564_112_2_18_0_14_29_1675 "SMentity:wrong state mesage!!"
#define SMENTITY_3003_112_2_18_0_14_30_1676 "SMentity:sapi not match."
#define SMMAIN_556_112_2_18_0_14_33_1677 "SM: same sys, discard init."
#define SMMAIN_773_112_2_18_0_14_33_1678 "SMmain:discard sm msg"
#define SMMAIN_840_112_2_18_0_14_34_1679 "SMmain:Decode msg reqpdpact error!\n"
#define SMMAIN_909_112_2_18_0_14_34_1680 "SMmain:Decode msg header error!\n"
#define SMMAIN_1565_112_2_18_0_14_35_1681 "SMmain:discard sm msg"
#define SMMAIN_2345_112_2_18_0_14_37_1682 "SMmain:SM do not send SNSM_SEQUENCE_IND to sndcp "
#define SMMAIN_2581_112_2_18_0_14_37_1683 "smmain: received unexpect me status."
#define SMMAIN_2926_112_2_18_0_14_38_1684 "smmain: received unexpect me status."
#define SMMAIN_3753_112_2_18_0_14_40_1685 "SMmain:Decode msg reqpdpact error!\n"
#define SNDCPENTITY_843_112_2_18_0_14_49_1686 "SNDCP: received release_ind with RELEASE_CAUSE_NORMAL_RELEASE on esting state"
#define SNDCPENTITY_872_112_2_18_0_14_49_1687 "SNDCP:received not defined cause.\n"
#define SNDCPENTITY_1329_112_2_18_0_14_50_1688 "SNDCP:Discard NPDU on invalid state!"
#define SNDCPENTITY_1351_112_2_18_0_14_50_1689 "SNDCP:Discard NPDU when reaching on the max buffer_size!"
#define SNDCPENTITY_1426_112_2_18_0_14_50_1690 "SNDCP:Discard NPDU on invalid state!"
#define SNDCPMAIN_281_112_2_18_0_14_55_1691 "SNDCP:NSAPI already existed."
#define SNDCPMAIN_372_112_2_18_0_14_55_1692 "SNDCP:NSAPI doesnt exist."
#define SNDCPMAIN_632_112_2_18_0_14_55_1693 "SNDCP:SN_UNITDATA_REQ - Null pointer"
#define SNDCPMAIN_691_112_2_18_0_14_55_1694 "SNDCP:SN_DATA_REQ - Null pointer"
#define SNDCPMAIN_1311_112_2_18_0_14_57_1695 "SNDCP:LL_SNDC_UNITDATA_IND - wrong nsapi or sapi"
#define SNDCPMAIN_1316_112_2_18_0_14_57_1696 "SNDCP:LL_SNDC_UNITDATA_IND - Null pointer"
#define SNDCPMAIN_1379_112_2_18_0_14_57_1697 "SNDCP:LL_SNDC_DATA_IND - wrong nsapi"
#define SNDCPMAIN_1384_112_2_18_0_14_57_1698 "SNDCP:LL_SNDC_DATA_IND - Null pointer"
#define DLR_296_112_2_18_0_15_14_1699 "DL: release req 0,but no entity"
#define DLR_320_112_2_18_0_15_14_1700 "DL: release req 3,but no entity"
#define DLR_480_112_2_18_0_15_15_1701 "L2: unexpected sapi\n"
#define DLR_798_112_2_18_0_15_15_1702 "PH_READY_TO_SEND_IND recved, but task released\n"
#define LAYER1_659_112_2_18_0_16_1_1703 "deactive req.\n"
#define LAYER1_732_112_2_18_0_16_1_1704 "rxpower_sweep_req asked and confirm replied.\n"
#define LAYER1_750_112_2_18_0_16_1_1705 "rxpower from BS come.\n"
#define LAYER1_768_112_2_18_0_16_1_1706 "bcch req asked.\n"
#define LAYER1_779_112_2_18_0_16_1_1707 "the bcch req is single %d.\n"
#define LAYER1_801_112_2_18_0_16_1_1708 "bcch req stop.\n"
#define LAYER1_818_112_2_18_0_16_1_1709 "bcch timer expired.\n"
#define LAYER1_853_112_2_18_0_16_1_1710 "bsic req asked.\n"
#define LAYER1_929_112_2_18_0_16_1_1711 "\n*****Layer1 Rx BCCH:*****\n"
#define LAYER1_930_112_2_18_0_16_1_1712 "BCCH_ARFCN:       %d \n\n"
#define LAYER1_971_112_2_18_0_16_1_1713 "\n*****L1Sim Rx BCCH:*****\n"
#define LAYER1_972_112_2_18_0_16_1_1714 "BCCH_ARFCN:%d \n\n"
#define LAYER1_1015_112_2_18_0_16_1_1715 "bcch ind true\n"
#define LAYER1_1176_112_2_18_0_16_2_1716 "campon req&confirmed.\n"
#define LAYER1_1274_112_2_18_0_16_2_1717 "TBF Downlink Assignment Recv\n"
#define LAYER1_1347_112_2_18_0_16_2_1718 "Send the first RA\n"
#define LAYER1_1421_112_2_18_0_16_2_1719 "L1 Send Random Acess Burst\n"
#define LAYER1_1475_112_2_18_0_16_2_1720 "AGCH recv\n"
#define LAYER1_1747_112_2_18_0_16_3_1721 "PLMN search asked and  rxpower confirm replied.\n"
#define LAYER1_2303_112_2_18_0_16_4_1722 "chan_assign:%d\n"
#define LAYER1_2345_112_2_18_0_16_4_1723 "chan_assign_fail:%d\n"
#define LAYER1_2428_112_2_18_0_16_4_1724 "handover_fail:%d\n"
#define LAYER1_2542_112_2_18_0_16_4_1725 "Received Data block from new DCCH in non-syn handover\n"
#define LAYER1_2551_112_2_18_0_16_5_1726 "handover :%d\n"
#define LAYER1_2626_112_2_18_0_16_5_1727 "handover_fail:%d\n"
#define LAYER1_2675_112_2_18_0_16_5_1728 "L1:sacch data recv\n"
#define LAYER1_2697_112_2_18_0_16_5_1729 "l1 recv data:%d\n"
#define LAYER1_2777_112_2_18_0_16_5_1730 "L1:data send at chan %d\n"
#define LAYER1_2799_112_2_18_0_16_5_1731 "L1:sacch data send\n"
#define LAYER1_2993_112_2_18_0_16_5_1732 "This bcch block is unexpected\n"
#define LAYER1_3031_112_2_18_0_16_5_1733 "bcch ind,true\n"
#define LAYER1_3038_112_2_18_0_16_5_1734 "bcch ind,not requested\n"
#define LAYER1_3151_112_2_18_0_16_6_1735 "SI_ID: 1"
#define LAYER1_3155_112_2_18_0_16_6_1736 "SI_ID: 2"
#define LAYER1_3159_112_2_18_0_16_6_1737 "SI_ID: 2Bis"
#define LAYER1_3163_112_2_18_0_16_6_1738 "SI_ID: 2Ter"
#define LAYER1_3167_112_2_18_0_16_6_1739 "SI_ID: 3"
#define LAYER1_3171_112_2_18_0_16_6_1740 "SI_ID: 4"
#define LAYER1_3175_112_2_18_0_16_6_1741 "SI_ID: 13"
#define LAYER1_3179_112_2_18_0_16_6_1742 "SI_ID: 5BIS"
#define LAYER1_3183_112_2_18_0_16_6_1743 "SI_ID: 5BIS"
#define LAYER1_3187_112_2_18_0_16_6_1744 "SI_ID: 6"
#define LAYER1_3191_112_2_18_0_16_6_1745 "SI_ID: 7"
#define LAYER1_3195_112_2_18_0_16_6_1746 "PSI_ID: 13"
#define LAYER1_3199_112_2_18_0_16_6_1747 "SI_ID: 4"
#define LAYER1_3203_112_2_18_0_16_6_1748 "PSI_ID: 3Bis"
#define LAYER1_3207_112_2_18_0_16_6_1749 "SI_ID: 8"
#define LAYER1_3211_112_2_18_0_16_6_1750 "SI_ID: 9"
#define LAYER1_3215_112_2_18_0_16_6_1751 "SI_ID: 13"
#define LAYER1_3219_112_2_18_0_16_6_1752 "PSI_ID: 1"
#define LAYER1_3223_112_2_18_0_16_6_1753 "PSI_ID: 2"
#define LAYER1_3227_112_2_18_0_16_6_1754 "PSI_ID: 3 "
#define LLENTITY_900_112_2_18_0_16_8_1755 "LLentity:impossible U frame type\n"
#define LLENTITY_1101_112_2_18_0_16_8_1756 "LLentity:impossible I frame type\n"
#define LLENTITY_1210_112_2_18_0_16_8_1757 "LLentity:impossible S frame type\n"
#define LLENTITY_1317_112_2_18_0_16_9_1758 "LLentity:recvd SNDCP ACK pdu in unack mode!\n"
#define LLENTITY_2523_112_2_18_0_16_11_1759 "LLentity:unexpected value\n"
#define LLENTITY_3071_112_2_18_0_16_12_1760 "LLE:received invalid command on ADM state"
#define LLMAIN_638_112_2_18_0_16_14_1761 "LLMAIN: discard l3 pdu, tlli mismatch"
#define LLMAIN_880_112_2_18_0_16_15_1762 "LLc Frame Error %d"
#define LLMAIN_1225_112_2_18_0_16_16_1763 "LLc2 Frame Error %d"
#define LLMAIN_1289_112_2_18_0_16_16_1764 "LLMAIN:discard l3 pdu, tlli mismatch,sapi %d\n"
#define LLMAIN_1607_112_2_18_0_16_16_1765 "LLMAIN: discard pdu, wrong state"
#define LLMAIN_1650_112_2_18_0_16_17_1766 "LLMAIN: discard pdu, wrong state"
#define LLMAIN_1674_112_2_18_0_16_17_1767 "LLMAIN: discard l3 pdu,wrong state"
#define LLMAIN_1696_112_2_18_0_16_17_1768 "LLMAIN: discard l3 pdu,wrong state"
#define MAC_523_112_2_18_0_16_18_1769 "MAC:try to start t3192 0 ms"
#define MAC_983_112_2_18_0_16_19_1770 "MAC: Decode grr freq req error!\n"
#define MAC_1020_112_2_18_0_16_19_1771 "MAC: DL imm assi,freq Acquare Sys Info error"
#define MAC_1189_112_2_18_0_16_20_1772 "MAC: DL imm assi,Decode ia rest octets error!"
#define MAC_1210_112_2_18_0_16_20_1773 "MAC: DL imm assi,Decode chan descri error!"
#define MAC_1268_112_2_18_0_16_20_1774 "MAC: recv pkt dl ass from rrc alloc no dl slot! Ignore pdch dl req!\n"
#define MAC_1285_112_2_18_0_16_20_1775 "MAC: Decode freq para error! Acquare Sys Info"
#define MAC_1554_112_2_18_0_16_20_1776 "MAC: Multislot capability not match! Ignore pdch dl req!"
#define MAC_1562_112_2_18_0_16_20_1777 "MAC: TBF_MODE in packet downlink ass is different from current!wrong! \n"
#define MAC_1568_112_2_18_0_16_20_1778 "MAC: No tfi assign,ignore the packet DL assi from ppch!"
#define MAC_1575_112_2_18_0_16_20_1779 "MAC: Decode packet DL error in idle, ignore the msg!"
#define MAC_1760_112_2_18_0_16_21_1780 "MAC: Decode rach response error! Acquare Sys Info"
#define MAC_1779_112_2_18_0_16_21_1781 "MAC: One phase received Dl single assi!\n"
#define MAC_2115_112_2_18_0_16_22_1782 "MAC: Single block without TBF!"
#define MAC_2242_112_2_18_0_16_22_1783 "MAC: One phase received Dl assi!"
#define MAC_2249_112_2_18_0_16_22_1784 "MAC: One phase received Dl EGPRS assi!"
#define MAC_2267_112_2_18_0_16_22_1785 "MAC: Decode rach response error! Rach retry!"
#define MAC_2308_112_2_18_0_16_22_1786 "MAC: wrong condition receive rach rsp"
#define MAC_2344_112_2_18_0_16_22_1787 "MAC: Rach response reject, Rach retry!"
#define MAC_2397_112_2_18_0_16_22_1788 "MAC: Rach response reject, ul tbf failure!\n"
#define MAC_2508_112_2_18_0_16_22_1789 "MAC: not support GRR_MAC_PDCH_ASSIGN_REQ"
#define MAC_2660_112_2_18_0_16_23_1790 "MAC: access abort invalid"
#define MAC_2713_112_2_18_0_16_23_1791 "MAC: access abort invalid"
#define MAC_3013_112_2_18_0_16_23_1792 "MAC: tbf mode invalid in connect cnf %d"
#define MAC_3078_112_2_18_0_16_23_1793 "MAC: Ul release, send ctrl ack request new TBF!"
#define MAC_3089_112_2_18_0_16_24_1794 "MAC: Ul release, send res req, request new TBF!"
#define MAC_3094_112_2_18_0_16_24_1795 "MAC: Ul release, unexpected ul_exp_action!!"
#define MAC_3182_112_2_18_0_16_24_1796 "MAC: receive ul_rel_req but ul not active"
#define MAC_3249_112_2_18_0_16_24_1797 "MAC:Assert Failure,tbf mode invalid in rlc_mac_rel_req"
#define MAC_3355_112_2_18_0_16_24_1798 "MAC:Assert Failure,release both link tbf but not in rel state"
#define MAC_3383_112_2_18_0_16_24_1799 "MAC:tbf mode invalid,uplink tbf starting time?"
#define MAC_3508_112_2_18_0_16_24_1800 "MAC:rlc_mac_tbf_req in ul_active!!"
#define MAC_3517_112_2_18_0_16_24_1801 "MAC:ignore rlc tbf req when prach have been send but not receive ass!"
#define MAC_3623_112_2_18_0_16_25_1802 "MAC:rlc realloc req but ul not active"
#define MAC_3729_112_2_18_0_16_25_1803 "MAC: wrong state 3186 expire"
#define MAC_3740_112_2_18_0_16_25_1804 "MAC: T3170 expiry! T3170 Dur: %d \n"
#define MAC_3837_112_2_18_0_16_25_1805 "MAC: wrong state t3170 expire"
#define MAC_3903_112_2_18_0_16_25_1806 "MAC: T3168 expiry! T3168 Dur: %d\n"
#define MAC_4019_112_2_18_0_16_25_1807 "MAC: wrong state rev t3168 expire"
#define MAC_4136_112_2_18_0_16_26_1808 "MAC: wrong state rev t3164 expire"
#define MAC_4214_112_2_18_0_16_26_1809 "MAC: wrong state rev t3162 expire"
#define MAC_4231_112_2_18_0_16_26_1810 "MAC: wrong state rev t3180 expire"
#define MAC_4310_112_2_18_0_16_26_1811 "MAC: T3192 expiry! T3192 Dur: %d\n"
#define MAC_4601_112_2_18_0_16_27_1812 "MAC: Prach access not permitted!"
#define MAC_4640_112_2_18_0_16_27_1813 "MAC: IA allocation uplink 8psk, but MS not support"
#define MAC_4795_112_2_18_0_16_27_1814 "MAC: IA assignment egprs, but MS not support"
#define MAC_4907_112_2_18_0_16_27_1815 "MAC: Single block without TBF in EGPRS!\n"
#define MAC_5286_112_2_18_0_16_28_1816 "MAC: Mac mode mismatch! Ignore packet ul assi!"
#define MAC_5311_112_2_18_0_16_28_1817 "MAC: rev ul assi in ul idle or other invalid state"
#define MAC_5324_112_2_18_0_16_28_1818 "MAC: Mac mode mismatch! Ignore packet ul assi!\n"
#define MAC_5405_112_2_18_0_16_28_1819 "MAC: one_phase_access_error!!"
#define MAC_5423_112_2_18_0_16_28_1820 "MAC: 1ph pkt ul ass allocation uplink 8psk, but MS not support"
#define MAC_5485_112_2_18_0_16_28_1821 "MAC: Decode multiblock allocation error! Rach retry!\n"
#define MAC_5735_112_2_18_0_16_29_1822 "MAC: Contention failure!\n"
#define MAC_5862_112_2_18_0_16_29_1823 "MAC: EGPRS invalid one phase state when rev ul assi"
#define MAC_5868_112_2_18_0_16_29_1824 "MAC: Decode Dynamic allocation error! Rach retry!\n"
#define MAC_5875_112_2_18_0_16_29_1825 "MAC: fixed allocation error! Rach retry!\n"
#define MAC_5884_112_2_18_0_16_29_1826 "MAC: 1ph pkt ul ass assignment egprs, but MS not support"
#define MAC_5909_112_2_18_0_16_29_1827 "MAC: Frequency not in band! Rach retry!\n"
#define MAC_5994_112_2_18_0_16_29_1828 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_5999_112_2_18_0_16_29_1829 "MAC: decode freq error return invalid value"
#define MAC_6024_112_2_18_0_16_29_1830 "MAC: 2ph allocation uplink 8psk, but MS not support"
#define MAC_6044_112_2_18_0_16_29_1831 "MAC: wrong assignment in second phase access"
#define MAC_6269_112_2_18_0_16_30_1832 "MAC: Decode allocation error! Rach retry!\n"
#define MAC_6276_112_2_18_0_16_30_1833 "MAC: Ingore the egprs ul assi in egprs_ul_assi_two_phase"
#define MAC_6284_112_2_18_0_16_30_1834 "MAC: 2ph assignment egprs, but MS not support"
#define MAC_6314_112_2_18_0_16_30_1835 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_6378_112_2_18_0_16_30_1836 "MAC: Frequency not in band! Rach retry!\n"
#define MAC_6384_112_2_18_0_16_30_1837 "MAC: decode freq error return invalid value"
#define MAC_6394_112_2_18_0_16_30_1838 "MAC: false tbf mode"
#define MAC_6408_112_2_18_0_16_30_1839 "MAC: ul active allocation uplink 8psk, but MS not support"
#define MAC_6417_112_2_18_0_16_30_1840 "MAC: false ul assignment in LABEL_UL_ASSI_UL_ACTIVE"
#define MAC_6579_112_2_18_0_16_30_1841 "MAC: Decode allocation error in LABEL_UL_ASSI_UL_ACTIVE! Rach retry!\n"
#define MAC_6588_112_2_18_0_16_30_1842 "MAC: Ingore the ul assi msg beacause lose dynamic allo in LABEL_UL_ASSI_UL_ACTIVE!"
#define MAC_6596_112_2_18_0_16_31_1843 "MAC: ul active assignment egprs, but MS not support"
#define MAC_6606_112_2_18_0_16_31_1844 "MAC: rev single block assi when ul active"
#define MAC_6661_112_2_18_0_16_31_1845 "MAC: DL est when waiting L1 release cnf"
#define MAC_6684_112_2_18_0_16_31_1846 "MAC:Ignore packet dl assi when two phase or contention phase!"
#define MAC_6694_112_2_18_0_16_31_1847 "MAC: rev dl assi when ul access two phase or other state"
#define MAC_6719_112_2_18_0_16_31_1848 "MAC: in ul access recv pkt dl ass alloc no dl slot! Ignore the msg!\n"
#define MAC_6732_112_2_18_0_16_31_1849 "MAC: wrong state rev dl assi,ignore!"
#define MAC_6743_112_2_18_0_16_31_1850 "MAC:  dl assign DL_TBF is different with UL_TBF in LABEL_DL_ASSI_UL_ACTIVE! Ignore the msg!\n"
#define MAC_6758_112_2_18_0_16_31_1851 "MAC: Ignore without dl_tfi dl assign msg while T3192 run!\n"
#define MAC_6770_112_2_18_0_16_31_1852 "MAC: in dl active recv pkt dl ass alloc no dl slot! Ignore the msg!\n"
#define MAC_6784_112_2_18_0_16_31_1853 "MAC: currnet dl tbf is EGPRS,receive gprs downlink ass,ignore!\n"
#define MAC_6791_112_2_18_0_16_31_1854 "MAC: Rlc mode mismatch! Ignore dl ass msg in dl_active!"
#define MAC_6852_112_2_18_0_16_31_1855 "MAC:contention phase, ignore TS RECONFIG"
#define MAC_6870_112_2_18_0_16_31_1856 "MAC: error state to receive ts reconfig"
#define MAC_6888_112_2_18_0_16_31_1857 "MAC: Timeslot reconfig assigned  tbf mode confilit with current ul tbf mode!ignor it!\n"
#define MAC_6900_112_2_18_0_16_31_1858 "MAC: e_ch_coding_command in ts recofig_ul_active err, ignore it!"
#define MAC_6916_112_2_18_0_16_31_1859 "MAC: in ul active recv pkt ts reconfig allocation no dl slot! Rach retry!\n"
#define MAC_6967_112_2_18_0_16_31_1860 "MAC: Decode error! Acquare Sys Info\n"
#define MAC_6972_112_2_18_0_16_31_1861 "MAC: Decode freq error! UL release and Rach retry!\n"
#define MAC_6992_112_2_18_0_16_31_1862 "MAC: Decode allocation error! UL release and Rach retry!\n"
#define MAC_7228_112_2_18_0_16_32_1863 "MAC: Timeslot reconfig not include dl tfi! Ul release and Rach retry!\n"
#define MAC_7263_112_2_18_0_16_32_1864 "MAC: in dl active recv pkt ts reconfig allocation no dl slot! Rach retry!\n"
#define MAC_7358_112_2_18_0_16_32_1865 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_7363_112_2_18_0_16_32_1866 "MAC: wrong state when rev ts realloc assi"
#define MAC_7369_112_2_18_0_16_32_1867 "MAC: Decode freq error! Rach retry!\n"
#define MAC_7381_112_2_18_0_16_32_1868 "MAC: Timeslot reconfig assigned  tbf mode confilit with current dl tbf mode!ignor it!\n"
#define MAC_7393_112_2_18_0_16_32_1869 "MAC: e_ch_coding_command in ts recofig_dl_active err, ignore it!"
#define MAC_7418_112_2_18_0_16_32_1870 "MAC: Decode allocation error! Rach retry!"
#define MAC_7752_112_2_18_0_16_33_1871 "MAC: Timeslot reconfig not include dl tfi! Rach retry!\n"
#define MAC_7762_112_2_18_0_16_33_1872 "MAC: Rlc mode mismatch! Ignore ts reconfig msg in dl_active!"
#define MAC_7812_112_2_18_0_16_33_1873 "MAC: Timeslot reconfig assigned  tbf mode confilit with current dl_ul tbf mode!ignor it!\n"
#define MAC_7824_112_2_18_0_16_33_1874 "MAC: e_ch_coding_command in ts recofig_ul_dl_active err, ignore it!"
#define MAC_7843_112_2_18_0_16_33_1875 "MAC: in ul dl active recv pkt ts reconfig allocation no dl slot! Rach retry!\n"
#define MAC_7892_112_2_18_0_16_33_1876 "MAC: Decode error! Acquare Sys Info\n"
#define MAC_7897_112_2_18_0_16_33_1877 "MAC: Decode freq error! UL release and Rach retry!\n"
#define MAC_7917_112_2_18_0_16_33_1878 "MAC: Decode allocation error! Rach retry!"
#define MAC_8184_112_2_18_0_16_34_1879 "MAC: Rlc mode mismatch! Ignore ts reconfig msg in ul_dl_active!"
#define MAC_8194_112_2_18_0_16_34_1880 "MAC: EGPRS UL ACK_NACK assigned tbf mode conflict with currnt ul tbf mode, ignore it"
#define MAC_8207_112_2_18_0_16_34_1881 "MAC: e_ch_coding_command in EGPRS UL ACK_NACK err, ignore it!"
#define MAC_8450_112_2_18_0_16_35_1882 "MAC: wrong state rev ul ack"
#define MAC_8476_112_2_18_0_16_35_1883 "MAC: Ignore additional PACKET ACCESS REJECT messages!"
#define MAC_8781_112_2_18_0_16_36_1884 "MAC: Receive reject, UL release!\n"
#define MAC_8835_112_2_18_0_16_36_1885 "MAC: ignore PACKET_QUEUING_NOTIFICATION while waiting TBF Starting Time of ul ass!"
#define MAC_9104_112_2_18_0_16_37_1886 "MAC: wrong ul state recv Packet tbf realse msg!"
#define MAC_9273_112_2_18_0_16_37_1887 "MAC: recv PPR no affect"
#define MAC_9301_112_2_18_0_16_37_1888 "MAC: wrong PPR"
#define MAC_9701_112_2_18_0_16_38_1889 "MAC: wrong state rev t3166 expire"
#define MAC_9707_112_2_18_0_16_38_1890 "MAC: One phase received egprs ul assi£¬but we don't support EDGE!"
#define MAC_9951_112_2_18_0_16_39_1891 "MAC: Abnormal release with retry!"
#define MAC_9975_112_2_18_0_16_39_1892 "MAC: !!Impossible ul_release_state!!"
#define MAC_10018_112_2_18_0_16_39_1893 "MAC: Abnormal release with rach retry!"
#define MAC_10030_112_2_18_0_16_39_1894 "MAC: release_cnf for ul but ul not active,maybe hold for connect cnf"
#define MAC_10089_112_2_18_0_16_39_1895 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_10101_112_2_18_0_16_39_1896 "MAC: Frequency not in band! Rach retry!\n"
#define MAC_10107_112_2_18_0_16_39_1897 "MAC: decode freq error,invalid return value"
#define MAC_10130_112_2_18_0_16_39_1898 "MAC: wrong state when rev single block assi"
#define MAC_10302_112_2_18_0_16_39_1899 "MAC: Decode allocation error! Rach retry!"
#define MAC_10309_112_2_18_0_16_39_1900 "MAC: Ingore the ul assi in UL_ASSI_TWO_PHASE_ACCESS"
#define MAC_10388_112_2_18_0_16_40_1901 "MAC: Decode freq error! Acquare Sys Info"
#define MAC_10399_112_2_18_0_16_40_1902 "MAC: wrong state when rev dl assi"
#define MAC_10405_112_2_18_0_16_40_1903 "MAC: Decode freq error! Rach retry!"
#define MAC_10649_112_2_18_0_16_40_1904 "MAC: No tfi assign in dl assi! Rach retry!\n"
#define MAC_10656_112_2_18_0_16_40_1905 "MAC: Multislot capability not match! Rach retry!"
#define MAC_10667_112_2_18_0_16_40_1906 "MAC: in ul active recv pkt dl ass alloc no dl slot! Ignore the msg!\n"
#define MAC_10720_112_2_18_0_16_40_1907 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_10725_112_2_18_0_16_40_1908 "MAC: Decode freq error! Ignore the msg!"
#define MAC_10924_112_2_18_0_16_41_1909 "MAC: Dl assi not include tfi!"
#define MAC_10930_112_2_18_0_16_41_1910 "MAC: Multislot capability not match! Ignore the msg!\n"
#define MAC_11003_112_2_18_0_16_41_1911 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_11292_112_2_18_0_16_41_1912 "MAC: Decode error! acquare si!\n"
#define MAC_11304_112_2_18_0_16_41_1913 "MAC: Multislot capability not match! Ignore the msg!"
#define MAC_11312_112_2_18_0_16_41_1914 "MAC: Contention failure!"
#define MAC_11537_112_2_18_0_16_42_1915 "MAC: Single block without TBF!\n"
#define MAC_11666_112_2_18_0_16_42_1916 "MAC: invalid one phase state when rev single block assi"
#define MAC_11880_112_2_18_0_16_43_1917 "MAC: invalid one phase state when rev ul assi"
#define MAC_11886_112_2_18_0_16_43_1918 "MAC: Decode allocation error! Rach retry!\n"
#define MAC_11893_112_2_18_0_16_43_1919 "MAC: fixed allocation, rach retry"
#define RLC_756_112_2_18_0_16_44_1920 "rlc:remain llc tbf request!"
#define RLC_773_112_2_18_0_16_44_1921 "rlc:cell reselect tbf!"
#define RLC_992_112_2_18_0_16_45_1922 "rlc:Up Single Block Access!\n"
#define RLC_1131_112_2_18_0_16_45_1923 "rlc:Up TBF Access for EGPRS SCT!\n"
#define RLC_1138_112_2_18_0_16_45_1924 "RLC: SCT est fail, downlink tbf is not acitved!"
#define RLC_1978_112_2_18_0_16_47_1925 "RLC: recv data error in loop back mode"
#define RLC_2013_112_2_18_0_16_47_1926 "rlc:error data_ind when dl is not actived!"
#define RLC_2112_112_2_18_0_16_47_1927 "rlc:SCT end!\n"
#define RLC_2299_112_2_18_0_16_47_1928 "rlc:should not recv up release"
#define RLC_2529_112_2_18_0_16_48_1929 "rlc:Access Fail with CS Page!\n"
#define RLC_2931_112_2_18_0_16_49_1930 "rlc:Up tbf fail with dl tbf is esting!\n"
#define RLC_2935_112_2_18_0_16_49_1931 "RLC: est sigle block without tbf interrupt by dl tbf ass"
#define RLC_2978_112_2_18_0_16_49_1932 "rlc:Up tbf ok for single block!\n"
#define RLC_3005_112_2_18_0_16_49_1933 "rlc:IS Rejected Access!\n"
#define RLC_3027_112_2_18_0_16_49_1934 "RLC:SCT Test Mode B Ested!"
#define RLC_3034_112_2_18_0_16_49_1935 "RLC:SCT Test Mode A Ested!"
#define RLC_3056_112_2_18_0_16_49_1936 "RLC:EGPRS SCT Test  Ested!"
#define RLC_3251_112_2_18_0_16_49_1937 "rlc:mac retry,but no need!\n"
#define RLC_3285_112_2_18_0_16_49_1938 "rlc:Up tbf fail with t3172 in mac ul_active!\n"
#define RLC_3363_112_2_18_0_16_49_1939 "rlc:error normal release"
#define RLC_3552_112_2_18_0_16_50_1940 "rlc:Up fail and perform cell reselect!\n"
#define RLC_3603_112_2_18_0_16_50_1941 "rlc:Up tbf fail with t3172 in mac ul_active!\n"
#define RLC_3687_112_2_18_0_16_50_1942 "rlc:Up tbf fail with timer t3172!\n"
#define RLC_3753_112_2_18_0_16_50_1943 "rlc:Receive Second mac_rlc_tbf_cnf msg!"
#define RLC_3768_112_2_18_0_16_50_1944 "rlc:Acess state is not one or two phase!"
#define RRC_1441_112_2_18_0_16_59_1945 "RRC_PutYvarpPtr: multi_sys=%d"
#define RRC_5211_112_2_18_0_17_6_1946 "DM: receive MPH_CAMP_FAIL_IND"
#define RRC_5527_112_2_18_0_17_7_1947 "RRC: ASSERT, invalid value. "
#define RRC_5836_112_2_18_0_17_8_1948 "RRC: unknown bsic cnf"
#define RRC_5923_112_2_18_0_17_8_1949 "RRC: ASSERT, research>6"
#define RRC_5942_112_2_18_0_17_8_1950 "RRC: unknown bsic fail"
#define RRC_6114_112_2_18_0_17_8_1951 "ASSERT: RRA should't set service_type RRA_RR_INVALID"
#define RRC_6296_112_2_18_0_17_9_1952 "RRC: ASSERT, it should not be happened."
#define RRC_6545_112_2_18_0_17_9_1953 "CCOFU Target cell is only GSM service !\n"
#define RRC_6555_112_2_18_0_17_9_1954 "CCOFU camp PBCCH!"
#define RRC_6926_112_2_18_0_17_10_1955 "RRC: rr_dm_request_type:%dn"
#define RRC_8030_112_2_18_0_17_12_1956 "MDL error with cause value:%dn"
#define RRC_8168_112_2_18_0_17_12_1957 "DM PS HO CCOFU SUCCESS "
#define RRC_8689_112_2_18_0_17_14_1958 "RRC: discard IA,because mm est saved"
#define RRC_8697_112_2_18_0_17_14_1959 "RRC: discard IA,because plmn list req"
#define RRC_9285_112_2_18_0_17_15_1960 "RRC: ASSERT, Invalid result string returned by NormIdleModeHandleBcchInd_sdl"
#define RRC_10678_112_2_18_0_17_18_1961 "RRC: arfcn =%d of camp cnf is different with arfcn=%d of camp req"
#define RRC_11291_112_2_18_0_17_19_1962 "RR:receive_psi_resel Get Packet paging!!\n"
#define RRC_11448_112_2_18_0_17_19_1963 "ASSERT: HandleFabRach_sdl return incorrectly"
#define RRC_11525_112_2_18_0_17_19_1964 "ASSERT: RRA set service_type(%d) is incorrect"
#define RRC_11540_112_2_18_0_17_19_1965 "ASSERT: RRA shouldn't send MPH_FAB_RACH_IND(%d)"
#define RRC_11702_112_2_18_0_17_20_1966 "RRC: assert error, not fastplmn"
#define RRC_11841_112_2_18_0_17_20_1967 "RRC: assert error, not fastplmn"
#define RRC_12051_112_2_18_0_17_20_1968 "RRC: ASSERT, invalid value. "
#define RRC_12273_112_2_18_0_17_21_1969 "RRC: unknown bsic cnf"
#define RRC_12352_112_2_18_0_17_21_1970 "RRC: unknown bsic fail"
#define RRC_12884_112_2_18_0_17_22_1971 "CCOFU immediate Ass Ext is received \n"
#define RRC_12983_112_2_18_0_17_22_1972 " CCOFU immediate Ass is received \n"
#define RRC_13000_112_2_18_0_17_22_1973 "RR: discard immed_assig"
#define RRC_13033_112_2_18_0_17_22_1974 "RR: rx immediate rej"
#define RRC_13482_112_2_18_0_17_23_1975 "CCOFU should not received MM_RR_ACT_REQ with non-RR_ACT_NO_SERVICE"
#define RRC_13961_112_2_18_0_17_24_1976 "RRC wrong sapi %d"
#define RRC_14033_112_2_18_0_17_24_1977 "RRC: set meas_valid to false"
#define RRC_14608_112_2_18_0_17_25_1978 "RRC: Invalid dl_sapi_num %d"
#define RRC_14874_112_2_18_0_17_26_1979 "RRC: wait assign dcch est error in dl chan type"
#define RRC_15751_112_2_18_0_17_28_1980 "RRC wrong sapi %d"
#define RRC_16951_112_2_18_0_17_30_1981 "CCOTU has been removed. 2010.11.19"
#define RRC_17285_112_2_18_0_17_31_1982 "DECODE_SI13_IN_NC_CELL_CHANGE receive msg RR_DELAY_EXP_IND"
#define RRC_18105_112_2_18_0_17_32_1983 "CCOFU MAC access success!"
#define RRC_18120_112_2_18_0_17_32_1984 "CCOFU wait MAC access response!"
#define RRC_18993_112_2_18_0_17_34_1985 "RRC: Receive data block on PPCH, discard it!"
#define RRC_19804_112_2_18_0_17_36_1986 "RRC: Decode PacketMeasureOrder err!"
#define RRC_20142_112_2_18_0_17_37_1987 "RRC: Packet cell change failure msg is sent, failure cause=%d"
#define RRC_20184_112_2_18_0_17_37_1988 "RRC: Decode PacketCellChangeOrder err!"
#define RRC_20992_112_2_18_0_17_38_1989 "SI2Quater is received in TBF mode "
#define RRC_21404_112_2_18_0_17_39_1990 "ignore any data from MPH during DM CCOFU"
#define RRC_21480_112_2_18_0_17_39_1991 "RRC: ASSERT ERROR recv prach when bad"
#define RRC_23625_112_2_18_0_17_43_1992 "RR ASSERT: The rat change action is wrong"
#define RRC_24245_112_2_18_0_17_45_1993 "RRDM: ERROR, handshake timer is timeout."
#define RRC_24289_112_2_18_0_17_45_1994 "TIMER_RESEL_TYPE"
#define RRC_24313_112_2_18_0_17_45_1995 "IMMEDIATE_RESEL_TYPE"
#define RRC_24338_112_2_18_0_17_45_1996 "NOT_RESEL_TYPE"
#define RRC_24359_112_2_18_0_17_45_1997 "TDD cell out of sync,clear Candidate_time to 0"
#define RRC_24463_112_2_18_0_17_45_1998 "NOT_RESEL_TYPE"
#define RRC_24478_112_2_18_0_17_45_1999 "IMMEDIATE_RESEL_TYPE"
#define RRC_24503_112_2_18_0_17_45_2000 "TIMER_RESEL_TYPE"
#define RRC_24519_112_2_18_0_17_45_2001 "TDD cell out of sync,clear Candidate_time to 0"
#define RRC_24566_112_2_18_0_17_45_2002 "RR_TDD_CELL_RESEL_EXP_IND:RESEL"
#define RRC_24594_112_2_18_0_17_45_2003 "RESTART RR_TDD_CELL_RESEL_EXP_IND:"
#define RRC_25040_112_2_18_0_17_46_2004 "cell-prio=%d"
#define RRC_25356_112_2_18_0_17_47_2005 "RRDM: BSIC decode fail, wait for the next one."
#define RRC_25726_112_2_18_0_17_47_2006 "TDD cell_num and arfcn_num are 0 ,clear Candidate_time to 0"
#define RRC_26439_112_2_18_0_17_49_2007 "DM TD TO GSM PS HO, FAIL , BACK TO TD"
#define RRC_26474_112_2_18_0_17_49_2008 "DM TD TO GSM  ABORT , BACK TO TD"
#define RRC_26510_112_2_18_0_17_49_2009 "DM TD TO GSM REJ, FAIL , BACK TO TD"
#define RRC_26515_112_2_18_0_17_49_2010 "DM TD TO GSM RESEL, FAIL , BACK TO TD"
#define RRC_26754_112_2_18_0_17_49_2011 "RR ASSERT: The rat change action is wrong"
#define RRC_26768_112_2_18_0_17_49_2012 "RRC: Recv MPH_RF_SCAN_IND in state %d"
#define RRC_26854_112_2_18_0_17_50_2013 "DM: goto label GO_TO_NO_CELL"
#define RRC_27392_112_2_18_0_17_51_2014 "DM: REJ to 2G is fail, revert"
#define RRC_28028_112_2_18_0_17_52_2015 "RRC: discard IA,because plmn list req"
#define RRC_28245_112_2_18_0_17_52_2016 "RRC: Receive data block on PPCH, discard it!"
#define RRC_28427_112_2_18_0_17_53_2017 "RRC: Decode pmo err!"
#define RRC_28465_112_2_18_0_17_53_2018 "RRC: Packet cell change failure msg is sent, failure cause=%d"
#define RRC_28508_112_2_18_0_17_53_2019 "RRC: find ncell fail"
#define RRC_28521_112_2_18_0_17_53_2020 "RRC: gmm state not ready"
#define RRC_28536_112_2_18_0_17_53_2021 "RRC: decode pcco fail"
#define RRC_29660_112_2_18_0_17_55_2022 "RRC: process saved ppch"
#define RRC_29749_112_2_18_0_17_55_2023 "RRC: proc saved pch "
#define RRC_29858_112_2_18_0_17_56_2024 "RRC: Packet cell change failure msg is sent, failure cause=%d"
#define RRC_29896_112_2_18_0_17_56_2025 "RRC: %d enter_idle %d,%d,%d,%d"
#define RRC_29996_112_2_18_0_17_56_2026 "RRC: Error, invalid ENTER_IDLE proc!!! %d "
#define RRC_30032_112_2_18_0_17_56_2027 "RRC: Plmn is scaning, skip limited search"
#define RRC_30396_112_2_18_0_17_57_2028 "RRC: resel scell arfcn 0x%x"
#define RRC_30628_112_2_18_0_17_57_2029 "RRC: cell resel arfcn 0x%x"
#define RRC_30890_112_2_18_0_17_58_2030 " RRC: switch to another cell, clear PSI Msg Buffer \n"
#define RRC_31328_112_2_18_0_17_58_2031 "RRC: unknown bsic cnf"
#define RRC_31497_112_2_18_0_17_59_2032 "RRC: warning, AssignPlmnScan_sdl fail %d,%d"
#define RRC_32138_112_2_18_0_18_0_2033 "RR temp = %d"
#define RRC_33405_112_2_18_0_18_3_2034 "RRC: Decode PacketMeasureOrder err!"
#define RRC_33492_112_2_18_0_18_3_2035 "RRC: Packet cell change failure msg is sent, failure cause=%d"
#define RRC_34038_112_2_18_0_18_4_2036 "RRC: Decode Pmo err!"
#define RRC_34094_112_2_18_0_18_4_2037 "RRC: Receive data block on PPCH, discard it!"
#define RRC_34175_112_2_18_0_18_4_2038 "DM PS HO CCOFU SUCCESS "
#define RRC_34970_112_2_18_0_18_6_2039 "RRC ASSERT: current state=%d"
#define RRC_35001_112_2_18_0_18_6_2040 "Packet transfer reselect to UTRAN; start"
#define RRC_35016_112_2_18_0_18_6_2041 "PCCN TO UTRAN; start"
#define RRC_35062_112_2_18_0_18_6_2042 "RRC:bsic_req_num=%d,arfcn_num=%d,search_num=%d"
#define RRC_35158_112_2_18_0_18_6_2043 "DM: goto lable DM-RESEL-TO-TD"
#define RRC_35194_112_2_18_0_18_6_2044 "DM: goto label DM_RESEL_TO_TD_CARD_SYNC"
#define RRC_35355_112_2_18_0_18_6_2045 "DM: goto lable DM_FAST_RETURN_TO_TD"
#define RRC_35405_112_2_18_0_18_7_2046 "Immediate to start PCCOTU"
#define RRC_35491_112_2_18_0_18_7_2047 "CCOFU arfcn = %d, band %d"
#define RRC_35538_112_2_18_0_18_7_2048 "DM: goto lable DM_CCOFU_BSIC_DECODE"
#define RRC_35625_112_2_18_0_18_7_2049 "si13 has been received together with si1^4 when ccofu"
#define RRC_35665_112_2_18_0_18_7_2050 "DM RR_MM_ACT_IND is send! "
#define RRC_35721_112_2_18_0_18_7_2051 "DM GRR_RLC_ACT_REQ is send! "
#define RRC_35732_112_2_18_0_18_7_2052 "DM GRR_RLC_SUSPEND is send!"
#define RRC_35744_112_2_18_0_18_7_2053 "GRR_RLC_NC_CELL_CHANGE_REQ is send!"
#define RRC_35835_112_2_18_0_18_7_2054 "DM TD TO GSM  ABORT  REQ Error"
#define RRC_35850_112_2_18_0_18_7_2055 "DM TD TO GSM  ABORT , BACK TO TD,L1 not active "
#define RRC_35932_112_2_18_0_18_8_2056 "DM: goto lable DM_TD_RESEL_TO_GSM"
#define RRC_35934_112_2_18_0_18_8_2057 "RRC:rr_user_request_band=%d,ms_band=%d"
#define RRC_36023_112_2_18_0_18_8_2058 "DM: goto lable DM_RESEL_BACK_TO_TD"
#define RRC_36047_112_2_18_0_18_8_2059 "DM: goto label  DM_RESEL_FAIL_NO_REVERT"
#define RRC_36099_112_2_18_0_18_8_2060 "DM: goto lable DM_RESEL_HANDLE_BCCH_INFO"
#define RRC_36140_112_2_18_0_18_8_2061 "PLMN is  in FPLMN list"
#define RRC_36146_112_2_18_0_18_8_2062 "PLMN is not Selected PLMN"
#define RRC_36188_112_2_18_0_18_8_2063 "cell-prio=%d"
#define RRC_36228_112_2_18_0_18_8_2064 "PLMN is in FLAI list"
#define RRC_36256_112_2_18_0_18_8_2065 "Arfcn not matcn"
#define RRC_36263_112_2_18_0_18_8_2066 "cell-prio error"
#define RRC_36270_112_2_18_0_18_8_2067 "CELL is barred when 3-2 reselect"
#define RRC_36288_112_2_18_0_18_8_2068 "DM: continue to handle BCCH info"
#define RRC_36299_112_2_18_0_18_8_2069 "DM: goto lable DM_TD_HO_TO_GSM"
#define RRC_36363_112_2_18_0_18_8_2070 "DM: goto lable DM_HO_CNF"
#define RRC_36435_112_2_18_0_18_9_2071 "DM: goto lable DM-HO-WAIT-PHYINFO"
#define RRC_36520_112_2_18_0_18_9_2072 "DM: goto lable DM_HO_HANDLE_EST_CNF"
#define RRC_36525_112_2_18_0_18_9_2073 "suspend gprs task after handover successfully."
#define RRC_36720_112_2_18_0_18_9_2074 "DM , goto lable DM-HO-HANDLE-REL-IND"
#define RRC_36722_112_2_18_0_18_9_2075 "DM   rx DL_RELEASE_IND in WAIT_HANDOVER_DCCH_EST\n"
#define RRC_36744_112_2_18_0_18_9_2076 "goto lable DM_TD_REJ_TO_GSM"
#define RRC_36810_112_2_18_0_18_9_2077 "DM: goto lable DM-RESEL-HO-BSIC-CNF"
#define RRC_36873_112_2_18_0_18_9_2078 "DM:, goto lable DM_RESEL_HO_BSIC_FAIL"
#define RRC_38845_112_2_18_0_18_13_2079 "rr_cur_manual_type=%d"
#define RRC_40617_112_2_18_0_18_17_2080 "RRC:  multi_sys=%d,RR State=%d,  plmn_list_req_state=%d,oper=%d,search end=%d"
#define RRC_40883_112_2_18_0_18_17_2081 "ASSERT: RRA set service_type(%d) is incorrect"
#define RRC_40957_112_2_18_0_18_18_2082 "warning: unexpect rr status appears in the procedure"
#define RRC_41211_112_2_18_0_18_18_2083 "RRC: Discard SI13 when scell is limited camp"
#define SNDCPENTITY_875_112_2_18_0_18_21_2084 "SNDCP: received release_ind with RELEASE_CAUSE_NORMAL_RELEASE on esting state"
#define SNDCPENTITY_904_112_2_18_0_18_21_2085 "SNDCP:received not defined cause.\n"
#define SNDCPENTITY_1367_112_2_18_0_18_22_2086 "SNDCP:Discard NPDU on invalid state!"
#define SNDCPENTITY_1389_112_2_18_0_18_22_2087 "SNDCP:Discard NPDU when reaching on the max buffer_size!"
#define SNDCPENTITY_1464_112_2_18_0_18_22_2088 "SNDCP:Discard NPDU on invalid state!"
#define SNDCPMAIN_318_112_2_18_0_18_27_2089 "SNDCP:NSAPI already existed."
#define SNDCPMAIN_414_112_2_18_0_18_27_2090 "SNDCP:NSAPI doesnt exist."
#define SNDCPMAIN_676_112_2_18_0_18_28_2091 "SNDCP:SN_UNITDATA_REQ - Null pointer"
#define SNDCPMAIN_735_112_2_18_0_18_28_2092 "SNDCP:SN_DATA_REQ - Null pointer"
#define SNDCPMAIN_1355_112_2_18_0_18_29_2093 "SNDCP:LL_SNDC_UNITDATA_IND - wrong nsapi or sapi"
#define SNDCPMAIN_1360_112_2_18_0_18_29_2094 "SNDCP:LL_SNDC_UNITDATA_IND - Null pointer"
#define SNDCPMAIN_1423_112_2_18_0_18_29_2095 "SNDCP:LL_SNDC_DATA_IND - wrong nsapi"
#define SNDCPMAIN_1428_112_2_18_0_18_29_2096 "SNDCP:LL_SNDC_DATA_IND - Null pointer"
#define CCENTITY_5433_112_2_18_0_18_46_2097 "CC: send start dtmf at %d staten"
#define CCENTITY_5478_112_2_18_0_18_46_2098 "CC: send stop dtmf at %d state n"
#define CMROUTER_337_112_2_18_0_18_50_2099 "cmrouter: Rel_PId, ti is %x\n"
#define CMROUTER_1746_112_2_18_0_18_53_2100 "cmrouter: deal MNSS_END_REQ, ti is %x, entity not exist\n"
#define CMROUTER_2192_112_2_18_0_18_54_2101 "cmrouter: deal MNSS_END_REQ, ti is %x, entity not exist\n"
#define CMROUTER_2610_112_2_18_0_18_55_2102 "cmrouter: deal MNSS_END_REQ, ti is %x, entity not exist\n"
#define DLR_272_112_2_18_0_19_5_2103 "DL: release req 0,but no entity"
#define DLR_296_112_2_18_0_19_5_2104 "DL: release req 3,but no entity"
#define DLR_458_112_2_18_0_19_5_2105 "L2: unexpected sapi\n"
#define DLR_776_112_2_18_0_19_6_2106 "PH_READY_TO_SEND_IND recved, but task released\n"
#define LAYER1_594_112_2_18_0_19_47_2107 "deactive req.\n"
#define LAYER1_667_112_2_18_0_19_47_2108 "rxpower_sweep_req asked and confirm replied.\n"
#define LAYER1_685_112_2_18_0_19_47_2109 "rxpower from BS come.\n"
#define LAYER1_703_112_2_18_0_19_47_2110 "bcch req asked.\n"
#define LAYER1_714_112_2_18_0_19_48_2111 "the bcch req is single %d.\n"
#define LAYER1_736_112_2_18_0_19_48_2112 "bcch req stop.\n"
#define LAYER1_753_112_2_18_0_19_48_2113 "bcch timer expired.\n"
#define LAYER1_788_112_2_18_0_19_48_2114 "bsic req asked.\n"
#define LAYER1_864_112_2_18_0_19_48_2115 "\n*****Layer1 Rx BCCH:*****\n"
#define LAYER1_865_112_2_18_0_19_48_2116 "BCCH_ARFCN:       %d \n\n"
#define LAYER1_906_112_2_18_0_19_48_2117 "\n*****L1Sim Rx BCCH:*****\n"
#define LAYER1_907_112_2_18_0_19_48_2118 "BCCH_ARFCN:%d \n\n"
#define LAYER1_950_112_2_18_0_19_48_2119 "bcch ind true\n"
#define LAYER1_1111_112_2_18_0_19_48_2120 "campon req&confirmed.\n"
#define LAYER1_1209_112_2_18_0_19_49_2121 "TBF Downlink Assignment Recv\n"
#define LAYER1_1282_112_2_18_0_19_49_2122 "Send the first RA\n"
#define LAYER1_1356_112_2_18_0_19_49_2123 "L1 Send Random Acess Burst\n"
#define LAYER1_1410_112_2_18_0_19_49_2124 "AGCH recv\n"
#define LAYER1_1682_112_2_18_0_19_49_2125 "PLMN search asked and  rxpower confirm replied.\n"
#define LAYER1_2012_112_2_18_0_19_50_2126 "chan_assign:%d\n"
#define LAYER1_2054_112_2_18_0_19_50_2127 "chan_assign_fail:%d\n"
#define LAYER1_2137_112_2_18_0_19_50_2128 "handover_fail:%d\n"
#define LAYER1_2251_112_2_18_0_19_51_2129 "Received Data block from new DCCH in non-syn handover\n"
#define LAYER1_2260_112_2_18_0_19_51_2130 "handover :%d\n"
#define LAYER1_2335_112_2_18_0_19_51_2131 "handover_fail:%d\n"
#define LAYER1_2384_112_2_18_0_19_51_2132 "L1:sacch data recv\n"
#define LAYER1_2406_112_2_18_0_19_51_2133 "l1 recv data:%d\n"
#define LAYER1_2486_112_2_18_0_19_51_2134 "L1:data send at chan %d\n"
#define LAYER1_2508_112_2_18_0_19_51_2135 "L1:sacch data send\n"
#define LAYER1_2702_112_2_18_0_19_51_2136 "This bcch block is unexpected\n"
#define LAYER1_2740_112_2_18_0_19_52_2137 "bcch ind,true\n"
#define LAYER1_2747_112_2_18_0_19_52_2138 "bcch ind,not requested\n"
#define LAYER1_2860_112_2_18_0_19_52_2139 "SI_ID: 1"
#define LAYER1_2864_112_2_18_0_19_52_2140 "SI_ID: 2"
#define LAYER1_2868_112_2_18_0_19_52_2141 "SI_ID: 2Bis"
#define LAYER1_2872_112_2_18_0_19_52_2142 "SI_ID: 2Ter"
#define LAYER1_2876_112_2_18_0_19_52_2143 "SI_ID: 3"
#define LAYER1_2880_112_2_18_0_19_52_2144 "SI_ID: 4"
#define LAYER1_2884_112_2_18_0_19_52_2145 "SI_ID: 13"
#define LAYER1_2888_112_2_18_0_19_52_2146 "SI_ID: 5BIS"
#define LAYER1_2892_112_2_18_0_19_52_2147 "SI_ID: 5BIS"
#define LAYER1_2896_112_2_18_0_19_52_2148 "SI_ID: 6"
#define LAYER1_2900_112_2_18_0_19_52_2149 "SI_ID: 7"
#define LAYER1_2904_112_2_18_0_19_52_2150 "PSI_ID: 13"
#define LAYER1_2908_112_2_18_0_19_52_2151 "SI_ID: 4"
#define LAYER1_2912_112_2_18_0_19_52_2152 "PSI_ID: 3Bis"
#define LAYER1_2916_112_2_18_0_19_52_2153 "SI_ID: 8"
#define LAYER1_2920_112_2_18_0_19_52_2154 "SI_ID: 9"
#define LAYER1_2924_112_2_18_0_19_52_2155 "SI_ID: 13"
#define LAYER1_2928_112_2_18_0_19_52_2156 "PSI_ID: 1"
#define LAYER1_2932_112_2_18_0_19_52_2157 "PSI_ID: 2"
#define LAYER1_2936_112_2_18_0_19_52_2158 "PSI_ID: 3 "
#define PROCESSMM_1932_112_2_18_0_19_56_2159 "MM: read sim data error"
#define PROCESSMM_2123_112_2_18_0_19_56_2160 "MM:begin saved call"
#define PROCESSMM_2222_112_2_18_0_19_57_2161 "MM:begin saved call"
#define PROCESSMM_2663_112_2_18_0_19_58_2162 "MM: GSM service detach\n"
#define PROCESSMM_2683_112_2_18_0_19_58_2163 "MM: MS_DEACT_TO_CALI in WAIT FOR MN REG"
#define PROCESSMM_4173_112_2_18_0_20_1_2164 "MM:receive MM_PD MSG at WAIT_FOR_NETWORK_COMMAND\n"
#define PROCESSMM_6308_112_2_18_0_20_5_2165 "MM:begin saved call"
#define PROCESSMM_6921_112_2_18_0_20_6_2166 "MM:begin saved call"
#define PROCESSMM_8568_112_2_18_0_20_9_2167 "MM:begin saved call"
#define PROCESSMM_10298_112_2_18_0_20_13_2168 "MM_CALI: MN_MM_NREG_REQ in WAIT_FOR_RR_CONNECTION_MM\n"
#define PROCESSMM_10633_112_2_18_0_20_13_2169 "MM_CALI: MN_MM_NREG_REQ in other status, with mm connect\n"
#define PROCESSMM_11138_112_2_18_0_20_14_2170 "MM_CALI: MN_MM_NREG_REQ in  MM_NO_CELL"
#define PROCESSMM_11204_112_2_18_0_20_15_2171 "MM_CALI: MN_MM_NREG_REQ in MM_NO_SIM"
#define PROCESSMM_11406_112_2_18_0_20_15_2172 "MM:begin saved call"
#define PROCESSMM_11465_112_2_18_0_20_15_2173 "MM: sim authentication error"
#define PROCESSMM_11536_112_2_18_0_20_15_2174 "MM:begin saved call"
#define PROCESSMM_12046_112_2_18_0_20_16_2175 "MM:begin saved call"
#define PROCESSMM_12338_112_2_18_0_20_17_2176 "MM:begin saved call"
#define PROCESSMM_12442_112_2_18_0_20_17_2177 "MM:begin saved call"
#define PROCESSMM_12461_112_2_18_0_20_17_2178 "MM:begin saved call"
#define PROCESSMM_12564_112_2_18_0_20_17_2179 "MM:begin saved call"
#define PROCESSMM_13164_112_2_18_0_20_18_2180 "MM:discard MM_RR_DATA_IND"
#define PROCESSMM_13491_112_2_18_0_20_19_2181 "MM_CALI: MN_MM_NREG_REQ in other status, with mm connect\n"
#define PROCESSMM_13592_112_2_18_0_20_19_2182 "MM_CALI: MN_MM_NREG_REQ in other status, without mm connect\n"
#define PROCESSMM_13709_112_2_18_0_20_19_2183 "MM:begin saved call"
#define PROCESSMM_14170_112_2_18_0_20_20_2184 "MM: current T3212 duration is %d"
#define PROCESSMM_15017_112_2_18_0_20_22_2185 "MM: There is a delay CM service\n"
#define RRC_2769_112_2_18_0_20_38_2186 "RRC: unknown bsic cnf"
#define RRC_2833_112_2_18_0_20_38_2187 "RRC: ASSERT, research>6"
#define RRC_2852_112_2_18_0_20_38_2188 "RRC: unknown bsic fail"
#define RRC_4366_112_2_18_0_20_42_2189 "RRC: discard IA,because mm est saved"
#define RRC_4374_112_2_18_0_20_42_2190 "RRC: discard IA,because plmn list req"
#define RRC_5798_112_2_18_0_20_44_2191 "RRC: assert error, not fastplmn"
#define RRC_5937_112_2_18_0_20_45_2192 "RRC: assert error, not fastplmn"
#define RRC_6397_112_2_18_0_20_46_2193 "RRC: unknown bsic cnf"
#define RRC_6476_112_2_18_0_20_46_2194 "RRC: unknown bsic fail"
#define RRC_6867_112_2_18_0_20_47_2195 "RR: discard immed_assig"
#define RRC_6900_112_2_18_0_20_47_2196 "RR: rx immediate rej"
#define RRC_7613_112_2_18_0_20_48_2197 "RRC wrong sapi %d"
#define RRC_8400_112_2_18_0_20_50_2198 "RRC: wait assign dcch est error in dl chan type"
#define RRC_9095_112_2_18_0_20_51_2199 "RRC wrong sapi %d"
#define RRC_11746_112_2_18_0_20_57_2200 "RRC: discard IA,because plmn list req"
#define RRC_13294_112_2_18_0_21_0_2201 "RRC: proc saved pch "
#define RRC_13878_112_2_18_0_21_1_2202 "RRC: resel scell arfcn 0x%x"
#define RRC_13998_112_2_18_0_21_1_2203 "RRC: cell resel arfcn 0x%x"
#define RRC_14394_112_2_18_0_21_2_2204 "RRC: unknown bsic cnf"
#define RRC_15165_112_2_18_0_21_4_2205 "RR temp = %d"
#define SM_COMMON_249_112_2_18_0_21_36_2206 "SMCOMMON:delete the activation pdp context request(nsapi %d)"
#define SM_COMMON_277_112_2_18_0_21_37_2207 "SMCOMMON: handle nsapi %d"
#define SM_DECODE_106_112_2_18_0_21_37_2208 "SM:decode Transaction identifier, TIE reserved!"
#define SM_DECODE_133_112_2_18_0_21_37_2209 "SM:decode Transaction identifier, EXT is '0'!"
#define SM_DECODE_261_112_2_18_0_21_37_2210 "SM:PDP address length too long!"
#define SM_DECODE_284_112_2_18_0_21_37_2211 "SM:decode pdp address,PPP,PDP type reserved!"
#define SM_DECODE_312_112_2_18_0_21_37_2212 "SM:decode pdp address,PDP org type reserved!"
#define SM_DECODE_394_112_2_18_0_21_38_2213 "SM: LLC SAPI Spare Field non-0!"
#define SM_DECODE_402_112_2_18_0_21_38_2214 "SM: LLC SAPI not assigned!"
#define SM_DECODE_414_112_2_18_0_21_38_2215 "SM: LLC SAPI value reserved,invalid!"
#define SM_DECODE_558_112_2_18_0_21_38_2216 "SM: is_r99 = %d"
#define SM_DECODE_589_112_2_18_0_21_38_2217 "SM:decode Qos Field invalid!"
#define SM_DECODE_689_112_2_18_0_21_38_2218 "SM:decode Qos Field invalid!"
#define SM_DECODE_696_112_2_18_0_21_38_2219 "SM:ignore extented qos fields!"
#define SM_DECODE_738_112_2_18_0_21_39_2220 "SM: Radio Priority Spare Field non-0!"
#define SM_DECODE_743_112_2_18_0_21_39_2221 "SM: Radio Priority reserved,level 4!"
#define SM_DECODE_784_112_2_18_0_21_39_2222 "SM:Receive APN Length > APN buffer!"
#define SM_DECODE_908_112_2_18_0_21_39_2223 "SM: PDP context accept,not support iei = %d"
#define SM_DECODE_1022_112_2_18_0_21_39_2224 "SM: PDP context accept,not support iei = %d"
#define SM_DECODE_1083_112_2_18_0_21_39_2225 "SM: CTIVATE PDP CONTEXT REJECT,not support iei = %d"
#define SM_DECODE_1149_112_2_18_0_21_39_2226 "SM: REQUEST PDP CONTEXT ACTIVATION, not support iei = %d"
#define SM_DECODE_1207_112_2_18_0_21_39_2227 "SM: DEACTIVATE PDP CONTEXT REQUEST,not support iei = %d"
#define SM_DECODE_1303_112_2_18_0_21_40_2228 "SM: Modify PDP context request(NT2MS),not support iei = %d"
#define SM_DECODE_1396_112_2_18_0_21_40_2229 "SM:Modify PDP context accept(NT2MS),not support iei = %d"
#define SM_DECODE_1511_112_2_18_0_21_40_2230 "SM: PDP 2th context accept,not support iei = %d"
#define SM_DECODE_1536_112_2_18_0_21_40_2231 "SM:PDP context accept Qos < MS Min_Qos Setting!!"
#define SM_DECODE_1541_112_2_18_0_21_40_2232 "SM:PDP context accept Qos > MS Min_Qos Setting!!\n"
#define SM_DECODE_1572_112_2_18_0_21_40_2233 "APN:invalid char appear!"
#define SM_ENCODE_289_112_2_18_0_21_41_2234 "SM:SM_EncodePdpaddr pdp_addr_length too long!"
#define SM_ENCODE_321_112_2_18_0_21_41_2235 "SM:SM_EncodePdpaddr pdp_addr_length too long!"
#define SM_ENCODE_336_112_2_18_0_21_41_2236 "SM:Assert Failure,SM_EncodePdpaddr"
#define SM_ENCODE_369_112_2_18_0_21_41_2237 "SM:SM_EncodeApn apn too long!"
#define SMS_RL_195_112_2_18_0_21_49_2238 "SMSRL_DecodeRpError: the cause is %d"
#define RFC2507C_105_112_2_18_0_21_52_2239 "\n"
#define RFC2507C_106_112_2_18_0_21_52_2240 "v6s_full_c \t %d \n"
#define RFC2507C_107_112_2_18_0_21_52_2241 "v6s_comp_c \t %d \n"
#define RFC2507C_108_112_2_18_0_21_52_2242 "v6s_unmod_c \t %d \n"
#define RFC2507C_109_112_2_18_0_21_52_2243 "v6s_bbefore_c \t %d \n"
#define RFC2507C_110_112_2_18_0_21_52_2244 "v6s_bafter_c \t %d \n"
#define RFC2507C_111_112_2_18_0_21_52_2245 "v6s_changes_c \t %d \n"
#define RFC2507C_112_112_2_18_0_21_52_2246 "v6s_unknown_c \t %d \n"
#define RFC2507C_113_112_2_18_0_21_52_2247 "v6s_full_d \t %d \n"
#define RFC2507C_114_112_2_18_0_21_52_2248 "v6s_comp_d \t %d \n"
#define RFC2507C_115_112_2_18_0_21_52_2249 "v6s_unmod_d \t %d \n"
#define RFC2507C_116_112_2_18_0_21_52_2250 "v6s_dropped_d \t %d \n"
#define RFC2507C_117_112_2_18_0_21_52_2251 "v6s_badsum_d \t %d \n"
#define RFC2507C_118_112_2_18_0_21_52_2252 "v6s_head_req_revc \t %d \n"
#define RFC2507C_119_112_2_18_0_21_52_2253 "\n"
#define RFC2507C_172_112_2_18_0_21_52_2254 "free_all: start\n"
#define RFC2507C_208_112_2_18_0_21_52_2255 "free_all: done\n"
#define RFC2507C_414_112_2_18_0_21_53_2256 "iphc_init: start sizeof(*csList) == %u\n"
#define RFC2507C_416_112_2_18_0_21_53_2257 "iphc_init: sizeof(*comp) == %u sizeof(*decomp) == %un"
#define RFC2507C_419_112_2_18_0_21_53_2258 "iphc_init: csList == NULL\n"
#define RFC2507C_428_112_2_18_0_21_53_2259 "iphc_init: cfg == NULL\n"
#define RFC2507C_433_112_2_18_0_21_53_2260 "iphc_init: len_send_t %d \n"
#define RFC2507C_434_112_2_18_0_21_53_2261 "iphc_init: len_recv_t %d \n"
#define RFC2507C_435_112_2_18_0_21_53_2262 "iphc_init: len_send_u %d \n"
#define RFC2507C_436_112_2_18_0_21_53_2263 "iphc_init: len_recv_u %d \n"
#define RFC2507C_437_112_2_18_0_21_53_2264 "iphc_init: max_header %d \n"
#define RFC2507C_438_112_2_18_0_21_53_2265 "iphc_init: max_period %d \n"
#define RFC2507C_439_112_2_18_0_21_53_2266 "iphc_init: max_time   %d \n"
#define RFC2507C_440_112_2_18_0_21_53_2267 "iphc_init: min_wrap   %d \n"
#define RFC2507C_441_112_2_18_0_21_53_2268 "iphc_init: reordering %d \n"
#define RFC2507C_442_112_2_18_0_21_53_2269 "iphc_init: nodelta    %d \n"
#define RFC2507C_443_112_2_18_0_21_53_2270 "iphc_init: udp_prime  %d \n"
#define RFC2507C_444_112_2_18_0_21_53_2271 "iphc_init: tcp_prime  %d \n"
#define RFC2507C_459_112_2_18_0_21_53_2272 "iphc_init: bad init values\n"
#define RFC2507C_464_112_2_18_0_21_53_2273 "iphc_init: bad value for tcp_prime\n"
#define RFC2507C_468_112_2_18_0_21_53_2274 "iphc_init: bad value for udp_prime\n"
#define RFC2507C_539_112_2_18_0_21_53_2275 "iphc_init: init done.\n"
#define RFC2507C_673_112_2_18_0_21_53_2276 "hash_keys: *** UNKNOWN IP version %d ***\n"
#define RFC2507C_678_112_2_18_0_21_53_2277 "iphc_hashkeys: IPv4 hdr corrupt\n"
#define RFC2507C_681_112_2_18_0_21_53_2278 "hash_keys: IP4\n"
#define RFC2507C_688_112_2_18_0_21_53_2279 "hash_keys: length fields for IP4 do not match! Bailing out!\n"
#define RFC2507C_690_112_2_18_0_21_53_2280 "hash_keys: length fields are: pkt %u len %u ip %un"
#define RFC2507C_691_112_2_18_0_21_53_2281 "hash_keys: %s\n"
#define RFC2507C_704_112_2_18_0_21_53_2282 "hash_keys: ip4 fragment\n"
#define RFC2507C_716_112_2_18_0_21_53_2283 "hash_keys: ip4 options len=%u  \n"
#define RFC2507C_739_112_2_18_0_21_53_2284 "hash_keys: mine hdr corrupt\n"
#define RFC2507C_745_112_2_18_0_21_53_2285 "hash_keys: IPPROTO_MIN_CAP  length %u\n"
#define RFC2507C_751_112_2_18_0_21_53_2286 "hash_keys: IPPROTO_MIN_CAP  next %x \n"
#define RFC2507C_757_112_2_18_0_21_53_2287 "hash_keys: auth hdr corrupt\n"
#define RFC2507C_762_112_2_18_0_21_53_2288 "hash_keys: IPPROTO_AH4  length %u \n"
#define RFC2507C_767_112_2_18_0_21_53_2289 "hash_keys: IPPROTO_AH4  next %x \n"
#define RFC2507C_773_112_2_18_0_21_53_2290 "hash_keys: unknown proto %d in ip4\n"
#define RFC2507C_785_112_2_18_0_21_54_2291 "hash_keys: IP6\n"
#define RFC2507C_793_112_2_18_0_21_54_2292 "hash_keys: length fields for IP6 does not match! Bailing out!\n"
#define RFC2507C_816_112_2_18_0_21_54_2293 "hash_keys: extention hdr corrupt\n"
#define RFC2507C_829_112_2_18_0_21_54_2294 "hash_keys: ROUTING\n"
#define RFC2507C_831_112_2_18_0_21_54_2295 "hash_keys: extension hdr corrupt\n"
#define RFC2507C_846_112_2_18_0_21_54_2296 "hash_keys: IPPROTO_AH6  length %u \n"
#define RFC2507C_848_112_2_18_0_21_54_2297 "hash_keys: auth hdr corrupt\n"
#define RFC2507C_857_112_2_18_0_21_54_2298 "hash_keys: IPPROTO_AH6  next %x \n"
#define RFC2507C_874_112_2_18_0_21_54_2299 "hash_keys: FRAGMENT\n"
#define RFC2507C_882_112_2_18_0_21_54_2300 "hash_keys: IPPROTO_NONE\n"
#define RFC2507C_887_112_2_18_0_21_54_2301 "hash_keys: unknown proto %d in ip6_hdr\n"
#define RFC2507C_899_112_2_18_0_21_54_2302 "hash_keys: ESP\n"
#define RFC2507C_908_112_2_18_0_21_54_2303 "hash_keys: UDP\n"
#define RFC2507C_916_112_2_18_0_21_54_2304 "hash_keys: length fields for UDP does not match! Bailing out!\n"
#define RFC2507C_922_112_2_18_0_21_54_2305 "hash_keys: udp ports s %d, d %dn"
#define RFC2507C_928_112_2_18_0_21_54_2306 "hash_keys: TCP\n"
#define RFC2507C_930_112_2_18_0_21_54_2307 "iphc_hashkeys: TCP hdr corrupt\n"
#define RFC2507C_939_112_2_18_0_21_54_2308 "hash_keys: tcp ports s %d, d %dn"
#define RFC2507C_959_112_2_18_0_21_54_2309 "equal_flows: ipv = %d\n"
#define RFC2507C_968_112_2_18_0_21_54_2310 "equal_flows: ip4\n"
#define RFC2507C_978_112_2_18_0_21_54_2311 "equal_flows: ip4 fragment\n"
#define RFC2507C_985_112_2_18_0_21_54_2312 "equal_flows: ip4 options len=%u\n"
#define RFC2507C_994_112_2_18_0_21_54_2313 "equal_flows: ip4h\n"
#define RFC2507C_1033_112_2_18_0_21_54_2314 "equal_flows: unknown proto %d in ip4\n"
#define RFC2507C_1039_112_2_18_0_21_54_2315 "equal_flows: ip6\n"
#define RFC2507C_1058_112_2_18_0_21_54_2316 "equal_flows: ip6 options i=%x t=%x\n"
#define RFC2507C_1111_112_2_18_0_21_54_2317 "equal_flows: frament\n"
#define RFC2507C_1118_112_2_18_0_21_54_2318 "equal_flows: none\n"
#define RFC2507C_1122_112_2_18_0_21_54_2319 "equal_flows: unknown proto %d in ip6_hdr\n"
#define RFC2507C_1128_112_2_18_0_21_54_2320 "equal_flows: esp\n"
#define RFC2507C_1135_112_2_18_0_21_54_2321 "equal_flows: udp\n"
#define RFC2507C_1139_112_2_18_0_21_54_2322 "equal_flows: udp ports s %d, d %dn"
#define RFC2507C_1145_112_2_18_0_21_54_2323 "equal_flows: tcp\n"
#define RFC2507C_1150_112_2_18_0_21_54_2324 "equal_flows: tcp ports s %d, d %dn"
#define RFC2507C_1227_112_2_18_0_21_55_2325 "compress: LARGE_CID %u \n"
#define RFC2507C_1250_112_2_18_0_21_55_2326 "compress: ip4:\n"
#define RFC2507C_1258_112_2_18_0_21_55_2327 "compress: ip4 fragment\n"
#define RFC2507C_1270_112_2_18_0_21_55_2328 "compress: ip4 options len=%u\n"
#define RFC2507C_1299_112_2_18_0_21_55_2329 "compress: ip4h:\n"
#define RFC2507C_1333_112_2_18_0_21_55_2330 "compress: unknown proto %x in ipv4\n"
#define RFC2507C_1339_112_2_18_0_21_55_2331 "compress: ip6:\n"
#define RFC2507C_1468_112_2_18_0_21_55_2332 "compress: proto NONE %x in ip6_hdr\n"
#define RFC2507C_1472_112_2_18_0_21_55_2333 "compress: unknown proto %x in ip6_hdr\n"
#define RFC2507C_1482_112_2_18_0_21_55_2334 "compress: tcp\n"
#define RFC2507C_1487_112_2_18_0_21_55_2335 "compress: tcp seq %u  ack %un"
#define RFC2507C_1491_112_2_18_0_21_55_2336 "compress: doing NODELTA\n"
#define RFC2507C_1510_112_2_18_0_21_55_2337 "compress: header request for cid %u ch %u\n"
#define RFC2507C_1530_112_2_18_0_21_55_2338 "compress: ipv4 r_byte is SET to %x  ip_ecn_save %xn"
#define RFC2507C_1541_112_2_18_0_21_55_2339 "compress: ipv6 r_byte is SET to %x  ip_ecn_save %xn"
#define RFC2507C_1556_112_2_18_0_21_55_2340 "compress: deltaS = %u iseq %u tseq %un"
#define RFC2507C_1614_112_2_18_0_21_55_2341 "compress: NOTHING\n"
#define RFC2507C_1622_112_2_18_0_21_55_2342 "compress: SPECIAL -> full\n"
#define RFC2507C_1626_112_2_18_0_21_55_2343 "compress: NEW_S | NEW_A\n"
#define RFC2507C_1631_112_2_18_0_21_55_2344 "compress: SPECIAL_I\n"
#define RFC2507C_1636_112_2_18_0_21_55_2345 "compress: NEW_S\n"
#define RFC2507C_1641_112_2_18_0_21_55_2346 "compress: SPECIAL_D deltaS = %u\n"
#define RFC2507C_1658_112_2_18_0_21_55_2347 "compress: tcp options %u bytes\n"
#define RFC2507C_1666_112_2_18_0_21_55_2348 "compress: TcpHc= %s\n"
#define RFC2507C_1675_112_2_18_0_21_55_2349 "compress: r_byte  restore ipv4 %x  newvalue=%xn"
#define RFC2507C_1679_112_2_18_0_21_55_2350 "compress: r_byte  restore ipv6 %x  newvalue=%xn"
#define RFC2507C_1689_112_2_18_0_21_56_2351 "compress: done_tcp cinfo=%x \" rdp %x chdr %xn"
#define RFC2507C_1738_112_2_18_0_21_56_2352 "compress: done NODELTA\n"
#define RFC2507C_1746_112_2_18_0_21_56_2353 "compress: CH %s\n"
#define RFC2507C_1748_112_2_18_0_21_56_2354 "compress: comp %d - %d = %dn"
#define RFC2507C_1758_112_2_18_0_21_56_2355 "compress: error cinfo"
#define RFC2507C_1764_112_2_18_0_21_56_2356 "compress: udp\n"
#define RFC2507C_1785_112_2_18_0_21_56_2357 "compress: max_time now - last == xxx %u - %u == %u  max %un"
#define RFC2507C_1804_112_2_18_0_21_56_2358 "compress: bailing out on min wrap violation (cid %u)n"
#define RFC2507C_1812_112_2_18_0_21_56_2359 "compress: min wrap not violated (cid %u)\n"
#define RFC2507C_1851_112_2_18_0_21_56_2360 "compress: max_time expired  sending full\n"
#define RFC2507C_1860_112_2_18_0_21_56_2361 "compress: done_udp %d \" rdp %x chdr %xn"
#define RFC2507C_1956_112_2_18_0_21_56_2362 "compress: CH %s\n"
#define RFC2507C_1958_112_2_18_0_21_56_2363 "compress: comp %d - %d = %dn"
#define RFC2507C_1965_112_2_18_0_21_56_2364 "compress: error cinfo"
#define RFC2507C_1972_112_2_18_0_21_56_2365 "compress: bailing out\n"
#define RFC2507C_2001_112_2_18_0_21_56_2366 "iphc_comp: length of incoming packet < IPv4 hdr\n"
#define RFC2507C_2009_112_2_18_0_21_56_2367 "iphc_comp: ---------- ipv4 iphdr_len %d,  real len %dn"
#define RFC2507C_2014_112_2_18_0_21_56_2368 "iphc_comp: ---------- ipv6 iphdr_len %d,  real len %dn"
#define RFC2507C_2022_112_2_18_0_21_56_2369 "iphc_comp: H  %s\n"
#define RFC2507C_2023_112_2_18_0_21_56_2370 "iphc_comp: H  %s\n"
#define RFC2507C_2048_112_2_18_0_21_56_2371 "iphc_comp: compress => ci_regu\n"
#define RFC2507C_2054_112_2_18_0_21_56_2372 "compress => unknown\n"
#define RFC2507C_2079_112_2_18_0_21_56_2373 "iphc_comp: compress => ci_full\n"
#define RFC2507C_2088_112_2_18_0_21_56_2374 "iphc_comp: compress => ci_comp\n"
#define RFC2507C_2095_112_2_18_0_21_56_2375 "iphc_comp: compress => ci_regu\n"
#define RFC2507C_2101_112_2_18_0_21_56_2376 "compress => unknown\n"
#define RFC2507D_209_112_2_18_0_21_57_2377 "iphc_decompf: ----------  len=%d\n"
#define RFC2507D_210_112_2_18_0_21_57_2378 "iphc_decompf: FH %s\n"
#define RFC2507D_211_112_2_18_0_21_57_2379 "iphc_decompf: FH %s\n"
#define RFC2507D_214_112_2_18_0_21_57_2380 "iphc_decompf: Too large packet, don't try to decompress\n"
#define RFC2507D_222_112_2_18_0_21_57_2381 "iphc_decompf: length of incoming packet smaller < IPv4 hdrn"
#define RFC2507D_230_112_2_18_0_21_57_2382 "iphc_decompf: ipv4\n"
#define RFC2507D_244_112_2_18_0_21_57_2383 "iphc_decompf: ip6_hdr\n"
#define RFC2507D_259_112_2_18_0_21_57_2384 "iphc_decompf: ip4\n"
#define RFC2507D_262_112_2_18_0_21_57_2385 "iphc_decompf: IPv4 hdr corrupt\n"
#define RFC2507D_267_112_2_18_0_21_57_2386 "iphc_decompf: IPv4 hdr length field too small\n"
#define RFC2507D_275_112_2_18_0_21_57_2387 "iphc_decompf: ************ IP4\n"
#define RFC2507D_284_112_2_18_0_21_57_2388 "iphc_decompf: ip4 fragment\n"
#define RFC2507D_306_112_2_18_0_21_57_2389 "hash_keys: mine hdr corrupt\n"
#define RFC2507D_318_112_2_18_0_21_57_2390 "iphc_decompf: auth in ip4_hdr\n"
#define RFC2507D_320_112_2_18_0_21_57_2391 "hash_keys: auth hdr corrupt\n"
#define RFC2507D_333_112_2_18_0_21_57_2392 "iphc_decompf: unknown proto %x in ip4_hdr\n"
#define RFC2507D_339_112_2_18_0_21_57_2393 "iphc_decompf: ip6\n"
#define RFC2507D_344_112_2_18_0_21_57_2394 "iphc_decompf: ************ IP6\n"
#define RFC2507D_362_112_2_18_0_21_57_2395 "hash_keys: extention hdr corrupt\n"
#define RFC2507D_374_112_2_18_0_21_58_2396 "hash_keys: auth hdr corrupt\n"
#define RFC2507D_403_112_2_18_0_21_58_2397 "iphc_decompf: proto NONE %x in ip6_hdr\n"
#define RFC2507D_407_112_2_18_0_21_58_2398 "iphc_decompf: unknown proto %x in ip6_hdr\n"
#define RFC2507D_416_112_2_18_0_21_58_2399 "iphc_decompf: esp\n"
#define RFC2507D_420_112_2_18_0_21_58_2400 "iphc_decompf: udp\n"
#define RFC2507D_430_112_2_18_0_21_58_2401 "iphc_decompf: ************ UDP %u  n"
#define RFC2507D_437_112_2_18_0_21_58_2402 "iphc_decompf: TCP header too small %d\n"
#define RFC2507D_442_112_2_18_0_21_58_2403 "iphc_decompf: tcp\n"
#define RFC2507D_447_112_2_18_0_21_58_2404 "iphc_decompf: tcp header is %d bytes longn"
#define RFC2507D_450_112_2_18_0_21_58_2405 "iphc_decompf: too large tcp cid %d\n"
#define RFC2507D_469_112_2_18_0_21_58_2406 "iphc_decompf: saved r_byte [TTttttII] %x\n"
#define RFC2507D_475_112_2_18_0_21_58_2407 "iphc_decompf: bailing out\n"
#define RFC2507D_481_112_2_18_0_21_58_2408 "decompf: LARGE CID \n"
#define RFC2507D_485_112_2_18_0_21_58_2409 "decompf: LARGE CID %u\n"
#define RFC2507D_490_112_2_18_0_21_58_2410 "iphc_decompf: too large tcp cid %d\n"
#define RFC2507D_497_112_2_18_0_21_58_2411 "iphc_decompf: too large udp cid %d\n"
#define RFC2507D_506_112_2_18_0_21_58_2412 "iphc_decompf: donefull cid %d gen %d len %d \n"
#define RFC2507D_507_112_2_18_0_21_58_2413 "iphc_decompf: FH %s\n"
#define RFC2507D_508_112_2_18_0_21_58_2414 "iphc_decompf: FH %s\n"
#define RFC2507D_526_112_2_18_0_21_58_2415 "iphc_decompf: removed cid %u from queue\n"
#define RFC2507D_571_112_2_18_0_21_58_2416 "iphc_decomp: CH %s\n"
#define RFC2507D_574_112_2_18_0_21_58_2417 "iphc_decomp: Too large packet, don't try to decompress\n"
#define RFC2507D_590_112_2_18_0_21_58_2418 "iphc_decomp: type TCP\n"
#define RFC2507D_605_112_2_18_0_21_58_2419 "iphc_decomp: too large tcp cid %d\n"
#define RFC2507D_609_112_2_18_0_21_58_2420 "iphc_decomp: tcp cid not in use %d\n"
#define RFC2507D_629_112_2_18_0_21_58_2421 "iphc_decomp: type UDP\n"
#define RFC2507D_633_112_2_18_0_21_58_2422 "iphc_decomp: too large udp cid %d\n"
#define RFC2507D_640_112_2_18_0_21_58_2423 "iphc_decomp: udp cid not in use %d\n"
#define RFC2507D_644_112_2_18_0_21_58_2424 "iphc_decomp: bad generation %u %u\n"
#define RFC2507D_662_112_2_18_0_21_58_2425 "iphc_decomp: hmm... ip version %u\n"
#define RFC2507D_669_112_2_18_0_21_58_2426 "iphc_decomp: IPv4 hdr length field too small\n"
#define RFC2507D_672_112_2_18_0_21_58_2427 "iphc_decomp: IP4\n"
#define RFC2507D_677_112_2_18_0_21_58_2428 "decomp: ip4 fragment\n"
#define RFC2507D_690_112_2_18_0_21_58_2429 "iphc_decomp: IPv4 hdr length field too small\n"
#define RFC2507D_713_112_2_18_0_21_58_2430 "decomp: ip4 options size changed \n"
#define RFC2507D_791_112_2_18_0_21_58_2431 "iphc_decomp: unknown proto %x in ip4_hdr\n"
#define RFC2507D_797_112_2_18_0_21_58_2432 "iphc_decomp: IP6\n"
#define RFC2507D_862_112_2_18_0_21_59_2433 "iphc_decomp: IPPROTO_ROUTING segleft=%u n"
#define RFC2507D_863_112_2_18_0_21_59_2434 "iphc_decomp:  %s\n"
#define RFC2507D_867_112_2_18_0_21_59_2435 "iphc_decomp: IPPROTO_ROUTING OPTLEN=%u i=%un"
#define RFC2507D_869_112_2_18_0_21_59_2436 "iphc_decomp: Bad optlen for routing option\n"
#define RFC2507D_909_112_2_18_0_21_59_2437 "iphc_decomp: proto NONE  %x in ip6_hdr\n"
#define RFC2507D_913_112_2_18_0_21_59_2438 "iphc_decomp: unknown proto %x in ip6_hdr\n"
#define RFC2507D_923_112_2_18_0_21_59_2439 "iphc_decomp: TCP  left = %u\n"
#define RFC2507D_953_112_2_18_0_21_59_2440 "iphc_decomp: r_byte *SET* to %6x\n"
#define RFC2507D_956_112_2_18_0_21_59_2441 "iphc_decomp: r_byte default  %6x\n"
#define RFC2507D_974_112_2_18_0_21_59_2442 "iphc_decomp: hmm... ip version %un"
#define RFC2507D_987_112_2_18_0_21_59_2443 "iphc_decomp: SPECIAL_I\n"
#define RFC2507D_994_112_2_18_0_21_59_2444 "iphc_decomp: SPECIAL_DATA seq = %un"
#define RFC2507D_1031_112_2_18_0_21_59_2445 "iphc_decomp: NEW_O but no options in table\n"
#define RFC2507D_1045_112_2_18_0_21_59_2446 "iphc_decomp: UDP space left = %u\n"
#define RFC2507D_1060_112_2_18_0_21_59_2447 "iphc_decomp: bailing out\n"
#define RFC2507D_1132_112_2_18_0_21_59_2448 "iphc_decomp: twice0_cksum %x   recv_cksum %xn"
#define RFC2507D_1141_112_2_18_0_21_59_2449 "iphc_decomp: try twice once with delta = %xn"
#define RFC2507D_1150_112_2_18_0_21_59_2450 "iphc_decomp: twice1_cksum %x   recv_cksum %xn"
#define RFC2507D_1154_112_2_18_0_21_59_2451 "iphc_decomp: success with twice.\n"
#define RFC2507D_1161_112_2_18_0_21_59_2452 "iphc_decomp: failed twice \n"
#define RFC2507D_1175_112_2_18_0_21_59_2453 "iphc_decomp: Checksum mismatch, Twice not supported. \n"
#define RFC2507D_1195_112_2_18_0_21_59_2454 "iphc_decomp: header request queued cid = %u\n"
#define RFC2507D_1210_112_2_18_0_21_59_2455 "iphc_decomp: hlen %d   cs->len %d\n"
#define RFC2507D_1214_112_2_18_0_21_59_2456 "iphc_decomp: cid %d  gen %d  hlen %d  comp_head_len %dn"
#define RFC2507D_1215_112_2_18_0_21_59_2457 "iphc_decomp: DH %s\n"
#define RFC2507D_1216_112_2_18_0_21_59_2458 "iphc_decomp: DH %s\n"
#define SNDCP_PC_2629_112_2_18_0_22_6_2459 "SNDCP:pc_decompress result = %d, s_sndcp_pc0.nsapis = %d, nsapi = %d"
#define SNDCP_SDL_446_112_2_18_0_22_9_2460 "SNDCP_SegNpduUnack seg_count = %d"
#define SNDCP_SDL_762_112_2_18_0_22_10_2461 "SNDCP_HandleUnsequentUnackBuffer reset_flag = %d, seg_count = %d"
#define SNDCP_SDL_825_112_2_18_0_22_10_2462 "SNDCP: first segment"
#define SNDCP_SDL_949_112_2_18_0_22_10_2463 "SNDCP: SNDCP_HandleTllSndcUnitdataInd receiving_state = %d"
#define SNDCP_SDL_1005_112_2_18_0_22_10_2464 "SNDCP: DATA-SNPDU F field set in receive subsequent state.\n"
#define SNDCP_SNPDU_447_112_2_18_0_22_11_2465 "SNDCP:invalid nsapi %d"
#define SNDCP_SNPDU_464_112_2_18_0_22_11_2466 "SNDCP:invalid sapi %d"
#define XID_SDL_427_112_2_18_0_22_15_2467 "SNDCP: RenegotiateReq result = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(DM_2G_PS_TRC)
TRACE_MSG(CCMSG_1855_112_2_18_0_3_13_0,"CC: The message is valid CC message. \n")
TRACE_MSG(CCMSG_1859_112_2_18_0_3_13_1,"CC: The message is invalid CC message. \n")
TRACE_MSG(CCMSG_2213_112_2_18_0_3_13_2,"CC: the IE is not valid IE of Setup message. \n")
TRACE_MSG(CCMSG_2220_112_2_18_0_3_13_3,"CC: Decode Setup message and find serious ie error.\n")
TRACE_MSG(CCMSG_2549_112_2_18_0_3_14_4,"CC: the IE is not valid IE of Setup message. \n")
TRACE_MSG(CCMSG_2595_112_2_18_0_3_14_5,"CC: Progress message is too short to contain mandatory IE.\n")
TRACE_MSG(CCMSG_2615_112_2_18_0_3_15_6,"CC: Progress message mandatory IE error.\n")
TRACE_MSG(CCMSG_2628_112_2_18_0_3_15_7,"CC: Progress Message Ie sequence error.\n")
TRACE_MSG(CCMSG_2681_112_2_18_0_3_15_8,"CC: Facility message is too short to contain mandatory IE.\n")
TRACE_MSG(CCMSG_2727_112_2_18_0_3_15_9,"CC: Notify message is too short to contain mandatory IE.\n")
TRACE_MSG(CCMSG_2892_112_2_18_0_3_15_10,"CC: Error IE in Call Proceeding Message. \n")
TRACE_MSG(CCMSG_2992_112_2_18_0_3_15_11,"CC: Error IE in Alerting Message. \n")
TRACE_MSG(CCMSG_3113_112_2_18_0_3_15_12,"CC: Error IE in Connect Message. \n")
TRACE_MSG(CCMSG_3241_112_2_18_0_3_16_13,"CC: Error IE in Disconnect Message. \n")
TRACE_MSG(CCMSG_3364_112_2_18_0_3_16_14,"CC: Error IE in Modify Message. \n")
TRACE_MSG(CCMSG_3481_112_2_18_0_3_16_15,"CC: Error IE in Modify Complete Message. \n")
TRACE_MSG(CCMSG_3601_112_2_18_0_3_16_16,"CC: Error IE in Modify Reject Message. \n")
TRACE_MSG(CCMSG_3650_112_2_18_0_3_17_17,"CC: Message is too short.\n")
TRACE_MSG(CCMSG_3691_112_2_18_0_3_17_18,"CC: Message is too short.\n")
TRACE_MSG(CCMSG_4013_112_2_18_0_3_17_19,"CC: Error IE in Release Message. \n")
TRACE_MSG(CCMSG_4115_112_2_18_0_3_17_20,"CC: Error IE in Release Complete Message. \n")
TRACE_MSG(CCMSG_4301_112_2_18_0_3_18_21,"CC: Cc-Establishment message is too short to contain mandatory IE.\n")
TRACE_MSG(CCMSG_4352_112_2_18_0_3_18_22,"CC: Recall message is too short to contain mandatory IE.\n")
TRACE_MSG(CCMSG_5230_112_2_18_0_3_20_23,"CC: bc ie is type error")
TRACE_MSG(CCMSG_5277_112_2_18_0_3_20_24,"CC: facility ie is type error")
TRACE_MSG(CCMSG_5337_112_2_18_0_3_20_25,"CC: progress ie is type error")
TRACE_MSG(CCMSG_5878_112_2_18_0_3_21_26,"CC: progress ie's type error")
TRACE_MSG(CCMSG_5947_112_2_18_0_3_21_27,"CC: progress ie's type error")
TRACE_MSG(CCMSG_6047_112_2_18_0_3_22_28,"CC: backup bc ie is type error")
TRACE_MSG(CCMSG_6103_112_2_18_0_3_22_29,"CC: allowed action ie is type error")
TRACE_MSG(L3FUNC_58_112_2_18_0_3_26_30,"L3FUNC: Assert Failure, PLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_95_112_2_18_0_3_27_31,"L3FUNC: Assert Failure, FPLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_189_112_2_18_0_3_27_32,"L3FUNC: Assert Failure, LongFPLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_406_112_2_18_0_3_27_33,"L3FUNC: Assert Failure, FLai list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_445_112_2_18_0_3_27_34,"L3FUNC: Assert Failure, FPLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_479_112_2_18_0_3_27_35,"L3FUNC: Assert Failure, LongFPLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_512_112_2_18_0_3_27_36,"L3FUNC: Assert Failure, PLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_542_112_2_18_0_3_28_37,"L3FUNC: Assert Failure, FLai list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_580_112_2_18_0_3_28_38,"L3FUNC: Assert Failure, EPLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_625_112_2_18_0_3_28_39,"L3FUNC: Assert Failure, EPLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_717_112_2_18_0_3_28_40,"L3FUNC: Assert Failure, Select PLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_747_112_2_18_0_3_28_41,"L3FUNC: Assert Failure, Available PLMN list length %d is more than MAX %d.")
TRACE_MSG(MMGMM_COMMON_IE_133_112_2_18_0_3_29_42,"MMGMM: Assert Failure, ie_type %d error, can't decode EC number list.")
TRACE_MSG(MMGMM_COMMON_IE_210_112_2_18_0_3_29_43,"MMGMM: DecodeEmgNum data_len=%d, data_ptr[0]=%d")
TRACE_MSG(MMGMM_COMMON_IE_266_112_2_18_0_3_29_44,"MMGMM:The length of the network name is too long")
TRACE_MSG(MMGMM_COMMON_IE_293_112_2_18_0_3_29_45,"MMGMM:The length of the network name is too long")
TRACE_MSG(MMGMM_COMMON_IE_328_112_2_18_0_3_29_46,"MMGMM: Assert Failure, date_len %d error, can't decode TimeZone.")
TRACE_MSG(MMGMM_COMMON_IE_353_112_2_18_0_3_29_47,"MMGMM: Assert Failure, date_len %d error, can't decode TimeZone and Time.")
TRACE_MSG(MMGMM_COMMON_IE_458_112_2_18_0_3_29_48,"MMGMM: decode MM/GMM information msg, MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MMGMM_COMMON_IE_468_112_2_18_0_3_29_49,"MMGMM: decode MM/GMM information msg, DECODE_SUCCESS, No Optional IE")
TRACE_MSG(MMGMM_COMMON_IE_578_112_2_18_0_3_29_50,"MMGMM: Assert Failure, ie_type %d error, can't decode MobileIdentify.")
TRACE_MSG(PS_MAIN_499_112_2_18_0_3_32_51,"SCI_GetRRTaskIdBySysId:ASSERT!multi_sys >= MULTI_SYS_NUM")
TRACE_MSG(PS_MAIN_636_112_2_18_0_3_32_52,"SCI_GetSysIdByNSWTHTask:nswth sys id=%d")
TRACE_MSG(SCI_AIR_INTERFACE_232_112_2_18_0_3_35_53,"SCIAIR:SCIAI_BSAIRMsgToPSMsg Receive unknown SignalCode")
TRACE_MSG(SCI_AIR_INTERFACE_307_112_2_18_0_3_35_54,"SCIAIR:SCIAI_BSPSMsgToAIRMsg Receive unknown SignalCode")
TRACE_MSG(STATE_MACHINE_33_112_2_18_0_4_8_55,"SDL:save signal %x\n")
TRACE_MSG(STATE_MACHINE_46_112_2_18_0_4_8_56,"SDL:discard signal %x\n")
TRACE_MSG(DL_BUFFER_771_112_2_18_0_4_14_57,"DL: NR error, ns=%d ,new=%d ,old=%d n")
TRACE_MSG(DL_BUFFER_1437_112_2_18_0_4_15_58,"L2:GetIFrame State is unexpected")
TRACE_MSG(DL_BUFFER_1513_112_2_18_0_4_16_59,"L2:PackageSFrame stype %d, is unexpected")
TRACE_MSG(DL_BUFFER_1659_112_2_18_0_4_16_60,"DL: send repeated SACCH")
TRACE_MSG(DL_BUFFER_1982_112_2_18_0_4_16_61,"L2:PackageUFrame unexpected utype %d")
TRACE_MSG(DL_BUFFER_2200_112_2_18_0_4_17_62,"DL:contension data fail,length wrong\n")
TRACE_MSG(DL_BUFFER_2207_112_2_18_0_4_17_63,"DL:contension data fail,data wrong\n")
TRACE_MSG(DL_BUFFER_2287_112_2_18_0_4_17_64,"DL: DL_GetUnsentSAPI3MsgHeader, MSG_PTR IS NOT NULL")
TRACE_MSG(DL_BUFFER_2293_112_2_18_0_4_17_65,"DL: MSG header is %d,%d")
TRACE_MSG(DL_BUFFER_2295_112_2_18_0_4_17_66,"DL:There is a message to be retrans!")
TRACE_MSG(DL_BUFFER_2327_112_2_18_0_4_17_67,"DL: receive SABM in established state, but not init dlc according to special condition")
TRACE_MSG(DL_BUFFER_2429_112_2_18_0_4_17_68,"DL: req_sro is true, DL_sro=%d, DL_repeat_uplink_sacch_pos=%d")
TRACE_MSG(DL_BUFFER_2544_112_2_18_0_4_18_69,"DL: is repeated FACCH")
TRACE_MSG(DL_BUFFER_2563_112_2_18_0_4_18_70,"DL: is repeated FACCH")
TRACE_MSG(GMM_CLASSMARK_476_112_2_18_0_4_19_71,"GMM:ASSERT Failure,GMMCLASSMARK_SetMsBand, ms_band=%d")
TRACE_MSG(GMM_CLASSMARK_491_112_2_18_0_4_19_72,"GMM: classmark InitMsBand, ms_band=%d")
TRACE_MSG(GMM_ENCODE_DECODE_189_112_2_18_0_4_19_73,"GMM:Assert Failure,Decode time failure")
TRACE_MSG(GMM_ENCODE_DECODE_284_112_2_18_0_4_20_74,"GMM:mobile_identity->type_identity =%d")
TRACE_MSG(GMM_ENCODE_DECODE_584_112_2_18_0_4_20_75,"GMM:detach type %d is interpreted as re-attach not required")
TRACE_MSG(GMM_ENCODE_DECODE_615_112_2_18_0_4_20_76,"GMM:message too short,len = %d!")
TRACE_MSG(GMM_ENCODE_DECODE_843_112_2_18_0_4_21_77,"GMM:Attach Accept msg, unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_849_112_2_18_0_4_21_78,"GMM:Memory Overflow!Check!")
TRACE_MSG(GMM_ENCODE_DECODE_875_112_2_18_0_4_21_79,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_916_112_2_18_0_4_21_80,"GMM:Attach Reject, unknownd field!")
TRACE_MSG(GMM_ENCODE_DECODE_922_112_2_18_0_4_21_81,"GMM:Memory Overflow!Check!")
TRACE_MSG(GMM_ENCODE_DECODE_946_112_2_18_0_4_21_82,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_991_112_2_18_0_4_21_83,"GMM:Detach Request,unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_997_112_2_18_0_4_21_84,"GMM:Memory Overflow!Check!")
TRACE_MSG(GMM_ENCODE_DECODE_1022_112_2_18_0_4_21_85,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_1048_112_2_18_0_4_21_86,"GMM:Detach Accept msg, unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_1054_112_2_18_0_4_21_87,"GMM:Memory Overflow!Check!")
TRACE_MSG(GMM_ENCODE_DECODE_1080_112_2_18_0_4_21_88,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_1139_112_2_18_0_4_21_89,"GMM:PTMSI allocation msg,unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_1145_112_2_18_0_4_21_90,"GMM:Memory Overflow!Check!")
TRACE_MSG(GMM_ENCODE_DECODE_1170_112_2_18_0_4_21_91,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_1233_112_2_18_0_4_22_92,"GMM:Authentication Msg error,rand-cksn should as both!")
TRACE_MSG(GMM_ENCODE_DECODE_1244_112_2_18_0_4_22_93,"GMM:Authentication&Ciper msg, unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_1250_112_2_18_0_4_22_94,"GMM:Memory Overflow!Check!")
TRACE_MSG(GMM_ENCODE_DECODE_1273_112_2_18_0_4_22_95,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_1301_112_2_18_0_4_22_96,"GMM:Identify Request,unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_1307_112_2_18_0_4_22_97,"GMM:Memory Overflow!Check!")
TRACE_MSG(GMM_ENCODE_DECODE_1336_112_2_18_0_4_22_98,"GMM:Nsapi number error,npdu_list->number=%d")
TRACE_MSG(GMM_ENCODE_DECODE_1377_112_2_18_0_4_22_99,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_1701_112_2_18_0_4_23_100,"GMM:Routing Area Accept msg,unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_1707_112_2_18_0_4_23_101,"GMM:Memory Overflow!Check!")
TRACE_MSG(GMM_ENCODE_DECODE_1762_112_2_18_0_4_23_102,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_1806_112_2_18_0_4_23_103,"GMM:RAU Reject msg, unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_1812_112_2_18_0_4_23_104,"GMM:Memory Overflow!Check!")
TRACE_MSG(GMM_ENCODE_DECODE_1835_112_2_18_0_4_23_105,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_1859_112_2_18_0_4_23_106,"GMM:GMM Status msg, unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_1865_112_2_18_0_4_23_107,"GMM:Memory Overflow!Check!")
TRACE_MSG(GMM_ENCODE_DECODE_1969_112_2_18_0_4_23_108,"GMM: rl_ind = %d")
TRACE_MSG(GMM_ENCODE_DECODE_2169_112_2_18_0_4_24_109,"GMM:Assert Failure,invalid type_identity,")
TRACE_MSG(GMM_ENCODE_DECODE_2856_112_2_18_0_4_25_110,"GMM: TLLI_FOREIGN ptmsi.valid_flag is %d")
TRACE_MSG(GMM_ENCODE_DECODE_2862_112_2_18_0_4_25_111,"GMM: TLLI_LOCAL ptmsi.valid_flag is %d")
TRACE_MSG(GMM_ENCODE_DECODE_2882_112_2_18_0_4_25_112,"GMM:TLLI error")
TRACE_MSG(GMM_ENCODE_DECODE_2893_112_2_18_0_4_25_113,"GMM: Type = %d, new TLLI allocated = %x")
TRACE_MSG(GMM_SDL_185_112_2_18_0_4_26_114,"GMM:Assert Failure,GMM_SendUnitdataReq msg_type = %d")
TRACE_MSG(GMM_SDL_520_112_2_18_0_4_27_115,"GMM:T3312 is deactivated")
TRACE_MSG(GMM_SDL_784_112_2_18_0_4_27_116,"GMM:info decode error")
TRACE_MSG(LLC_ANALYSE_FRAME_244_112_2_18_0_4_36_117,"LLC:add oc_unack_downlink,512 for missing N(u)")
TRACE_MSG(LLC_ANALYSE_FRAME_249_112_2_18_0_4_36_118,"LLC:the received UI frame is enciphered but no cipher algorithm!")
TRACE_MSG(LLC_ANALYSE_FRAME_275_112_2_18_0_4_36_119,"LLC:before_crc 512+oc_unack_downlink=%d")
TRACE_MSG(LLC_ANALYSE_FRAME_385_112_2_18_0_4_36_120,"LLC: I frame need to be deciphered!")
TRACE_MSG(LLC_ANALYSE_FRAME_405_112_2_18_0_4_36_121,"LLC: I frame length invalid,discard")
TRACE_MSG(LLC_ANALYSE_FRAME_426_112_2_18_0_4_36_122,"LLC: I frame FCS invalid,discard")
TRACE_MSG(LLC_ANALYSE_FRAME_510_112_2_18_0_4_36_123,"LLE: Received a duplicate frame")
TRACE_MSG(LLC_ANALYSE_FRAME_757_112_2_18_0_4_37_124,"LLC: I frame N(S) invalid,discard")
TRACE_MSG(LLC_ANALYSE_FRAME_784_112_2_18_0_4_37_125,"LLC:Assert Failure,DeleteSHeadNode empty 0!")
TRACE_MSG(LLC_ANALYSE_FRAME_921_112_2_18_0_4_37_126,"LLC:Assert Failure,DeleteSHeadNode empty!")
TRACE_MSG(LLC_ANALYSE_FRAME_973_112_2_18_0_4_37_127,"LLC:Assert Failure,frame size of dummy UI is wrong!")
TRACE_MSG(LLC_COMMON_CODE_337_112_2_18_0_4_38_128,"LLC:Assert Failure,wrong sapi value!")
TRACE_MSG(LLC_COMMON_CODE_426_112_2_18_0_4_38_129,"LLC:Assert Failure,SetFRMRErrMsg wrong error type!")
TRACE_MSG(LLC_COMMON_CODE_831_112_2_18_0_4_39_130,"LLC:Assert Failure,Polling next U node wrong value!")
TRACE_MSG(LLC_COMMON_CODE_875_112_2_18_0_4_39_131,"LLC:Assert Failure,InvalidXID next U node wrong value!")
TRACE_MSG(LLC_COMMON_CODE_1011_112_2_18_0_4_39_132,"LLC:KC changed and reset IOV_UI/I")
TRACE_MSG(LLC_COMMON_CODE_1267_112_2_18_0_4_40_133,"LLC:Cipher ASSERT !! gea_ciphering failed!")
TRACE_MSG(LLC_COMMON_CODE_1329_112_2_18_0_4_40_134,"LLC:Cipher ASSERT !! gea_ciphering failed!")
TRACE_MSG(LLC_COMMON_CODE_1383_112_2_18_0_4_40_135,"LLC: Xid negotiation N201-U=%d")
TRACE_MSG(LLC_COMMON_CODE_1423_112_2_18_0_4_40_136,"LLC:gmm need cipher misstate!")
TRACE_MSG(LLC_COMMON_CODE_1947_112_2_18_0_4_41_137,"LLCCC_SendSignalToLlentity: signal to be sent is 0x%x ")
TRACE_MSG(LLC_COMMON_CODE_1977_112_2_18_0_4_41_138,"LLCCC_SendSignalToLlentity: assert error, unprocessed signal 0x%x")
TRACE_MSG(LLC_COMPOSE_FRAME_163_112_2_18_0_4_42_139,"LLC:UPLINK XID PARAMETES-->T200:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_173_112_2_18_0_4_42_140,"LLC:UPLINK XID PARAMETES-->N200:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_185_112_2_18_0_4_42_141,"LLC:UPLINK XID PARAMETES-->N201 U:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_202_112_2_18_0_4_42_142,"LLC:UPLINK XID PARAMETES-->N201 I:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_214_112_2_18_0_4_42_143,"LLC:UPLINK XID PARAMETES-->mD:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_226_112_2_18_0_4_42_144,"LLC:UPLINK XID PARAMETES-->mU:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_236_112_2_18_0_4_42_145,"LLC:UPLINK XID PARAMETES-->kD:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_246_112_2_18_0_4_42_146,"LLC:UPLINK XID PARAMETES-->kU:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_280_112_2_18_0_4_42_147,"LLC:UPLINK XID PARAMETES-->zero-length Layer 3 Parameters")
TRACE_MSG(LLC_COMPOSE_FRAME_287_112_2_18_0_4_42_148,"LLC:UPLINK XID PARAMETES-->zero-length Layer 3 Parameters")
TRACE_MSG(LLC_COMPOSE_FRAME_664_112_2_18_0_4_43_149,"LLC:Assert Failure,ComposeIFrame S empty!")
TRACE_MSG(LLC_COMPOSE_FRAME_1082_112_2_18_0_4_43_150,"LLC: in set frame status\n")
TRACE_MSG(LLC_COMPOSE_FRAME_1123_112_2_18_0_4_44_151,"LLC: in set frame before n(r) acked\n")
TRACE_MSG(LLC_COMPOSE_FRAME_1296_112_2_18_0_4_44_152,"LLC:decode I frame SACK frame error!")
TRACE_MSG(LLC_COMPOSE_FRAME_1385_112_2_18_0_4_44_153,"LLC: SACK error bitmap")
TRACE_MSG(LLC_COMPOSE_FRAME_1559_112_2_18_0_4_45_154,"LLC:Assert Failure,SACK DeleteSNode S rnr invalid or empty")
TRACE_MSG(LLC_COMPOSE_FRAME_1575_112_2_18_0_4_45_155,"LLC:Assert Failure,SACK DeleteSNode S invalid or empty")
TRACE_MSG(LLC_COMPOSE_FRAME_1686_112_2_18_0_4_45_156,"LLC:Assert Failure,DeleteSNode S rnr invalid or empty")
TRACE_MSG(LLC_COMPOSE_FRAME_1702_112_2_18_0_4_45_157,"LLC:Assert Failure,DeleteSNode S invalid or empty")
TRACE_MSG(LLC_COMPOSE_FRAME_1782_112_2_18_0_4_45_158,"LLC:Assert Failure,DeleteUINode empty or invalid")
TRACE_MSG(LLC_COMPOSE_FRAME_1814_112_2_18_0_4_45_159,"LLC:Assert Failure,ComposeNewFrame  UI empty or invalid")
TRACE_MSG(LLC_QUEUE_406_112_2_18_0_4_47_160,"LLC:Add U frame pos = %d")
TRACE_MSG(LLC_QUEUE_463_112_2_18_0_4_47_161,"LLC:Add UI frame pos = %d")
TRACE_MSG(LLC_QUEUE_564_112_2_18_0_4_47_162,"LLC:DeleteSHeadNode S queue empty!")
TRACE_MSG(LLC_QUEUE_724_112_2_18_0_4_48_163,"LLC: DeleteSNode, line = %d")
TRACE_MSG(LLC_QUEUE_784_112_2_18_0_4_48_164,"LLC: LLCLQ_DeleteUNode index = %d invalid")
TRACE_MSG(LLC_QUEUE_842_112_2_18_0_4_48_165,"LLC: LLCLQ_DeleteUINode index = %d invalid")
TRACE_MSG(LLC_QUEUE_959_112_2_18_0_4_48_166,"LLC: DeleteINode, line = %d")
TRACE_MSG(LLC_QUEUE_1072_112_2_18_0_4_49_167,"LLC: DeleteReceiveNode, line = %d")
TRACE_MSG(LLC_QUEUE_1603_112_2_18_0_4_50_168,"LLC:Assert Failure,LLC_GlobalInit,sapi invalid!")
TRACE_MSG(LLC_QUEUE_1750_112_2_18_0_4_50_169,"LLC: in ResetRecvQueueWithSize, line = %d")
TRACE_MSG(LLC_SDL_145_112_2_18_0_4_51_170,"LLentity:free,reliability error!")
TRACE_MSG(LLC_SDL_314_112_2_18_0_4_51_171,"LLC:invalid lle_reliability = %d")
TRACE_MSG(LLC_SDL_474_112_2_18_0_4_51_172,"LLC:LLC_SendUIQueue i = %d")
TRACE_MSG(LLC_SDL_604_112_2_18_0_4_51_173,"LLentity: discard l3 pdu, abnormal")
TRACE_MSG(LLC_TEST_47_112_2_18_0_4_52_174,"LLC_TEST:lle_ResetT201 expire!")
TRACE_MSG(LLC_TEST_52_112_2_18_0_4_52_175,"LLC_TEST:lle_SetT200 expire!")
TRACE_MSG(LLC_TEST_57_112_2_18_0_4_52_176,"LLC_TEST:lle_SetT201 expire!")
TRACE_MSG(LLC_XID_CODE_972_112_2_18_0_4_54_177,"LLC:Log XID frame parameter:")
TRACE_MSG(LLC_XID_CODE_975_112_2_18_0_4_54_178,"LLC:xid Reset")
TRACE_MSG(LLC_XID_CODE_979_112_2_18_0_4_54_179,"LLC:Version = %d")
TRACE_MSG(LLC_XID_CODE_983_112_2_18_0_4_54_180,"LLC:T200 = %d")
TRACE_MSG(LLC_XID_CODE_987_112_2_18_0_4_54_181,"LLC:N200 = %d")
TRACE_MSG(LLC_XID_CODE_991_112_2_18_0_4_54_182,"LLC:N201_U = %d")
TRACE_MSG(LLC_XID_CODE_995_112_2_18_0_4_54_183,"LLC:N201_I = %d")
TRACE_MSG(LLC_XID_CODE_999_112_2_18_0_4_54_184,"LLC:mD = %d")
TRACE_MSG(LLC_XID_CODE_1003_112_2_18_0_4_54_185,"LLC:mU = %d")
TRACE_MSG(LLC_XID_CODE_1007_112_2_18_0_4_54_186,"LLC:kD = %d")
TRACE_MSG(LLC_XID_CODE_1011_112_2_18_0_4_54_187,"LLC:kU = %d")
TRACE_MSG(LLC_XID_CODE_1015_112_2_18_0_4_54_188,"LLC:IOV_UI = 0x%08x")
TRACE_MSG(LLC_XID_CODE_1019_112_2_18_0_4_54_189,"LLC:IOV_I = 0x%08x")
TRACE_MSG(LLC_XID_CODE_1023_112_2_18_0_4_54_190,"LLC:SNDCP xid Parameters included")
TRACE_MSG(LLC_XID_CODE_1064_112_2_18_0_4_55_191,"LLC:the received U frame does not include xid parameter!")
TRACE_MSG(MAC_CSN_MSG_1171_112_2_18_0_4_58_192,"MAC:egprs message!")
TRACE_MSG(MAC_CSN_MSG_1612_112_2_18_0_4_59_193,"MAC:DL PWR CTRL %d, MODE %d, PR MODE %d")
TRACE_MSG(MAC_CSN_MSG_2940_112_2_18_0_5_2_194,"MAC:ul ack alloc fix alloction,ms not support!")
TRACE_MSG(MAC_CSN_MSG_3021_112_2_18_0_5_2_195,"MAC:ul ack alloc fix alloction,ms not support!")
TRACE_MSG(MAC_CSN_MSG_3130_112_2_18_0_5_2_196,"MAC:DL PWR CTRL %d, MODE %d, PR MODE %d")
TRACE_MSG(MAC_CSN_MSG_3144_112_2_18_0_5_2_197,"MAC:uplink ass allo fix allocation,ms not support!")
TRACE_MSG(MAC_CSN_MSG_3235_112_2_18_0_5_2_198,"MAC:uplink ass allo fix allocation,ms not support!")
TRACE_MSG(MAC_CSN_MSG_3313_112_2_18_0_5_2_199,"MAC:uplink ass allo fix allocation,ms not support!")
TRACE_MSG(MAC_CSN_MSG_3393_112_2_18_0_5_3_200,"MAC:uplink ass allo fix allocation,ms not support!")
TRACE_MSG(MAC_CSN_MSG_3482_112_2_18_0_5_3_201,"MAC:Packet Cell Change Failure have FDD_ind!! ERR!!!")
TRACE_MSG(MAC_CSN_MSG_3487_112_2_18_0_5_3_202,"MAC:Packet Cell Change Failure have TDD_ind!! ERR!!!")
TRACE_MSG(MAC_CSN_MSG_3673_112_2_18_0_5_3_203,"MAC:Unknown access type, error!!!")
TRACE_MSG(MAC_CSN_MSG_3966_112_2_18_0_5_4_204,"MAC:Packet Measurement Report have 3g_meas_report!ERR!!!")
TRACE_MSG(MAC_CSN_MSG_4075_112_2_18_0_5_4_205,"MAC:L_length =%d is not enough when encode crbb_len!!\n")
TRACE_MSG(MAC_CSN_MSG_4086_112_2_18_0_5_4_206,"MAC:L_length =%d is not enough when encode comp_bitmap_start_color_code!!\n")
TRACE_MSG(MAC_CSN_MSG_4131_112_2_18_0_5_4_207,"MAC:L_length =%d is not enough when encode crbb!!\n")
TRACE_MSG(MAC_CSN_MSG_4180_112_2_18_0_5_4_208,"MAC:L_length =%d is not enough when encode urbb!!\n")
TRACE_MSG(MAC_CSN_MSG_4425_112_2_18_0_5_5_209,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4430_112_2_18_0_5_5_210,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4437_112_2_18_0_5_5_211,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4443_112_2_18_0_5_5_212,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4451_112_2_18_0_5_5_213,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4456_112_2_18_0_5_5_214,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4467_112_2_18_0_5_5_215,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4472_112_2_18_0_5_5_216,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4480_112_2_18_0_5_5_217,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4486_112_2_18_0_5_5_218,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4494_112_2_18_0_5_5_219,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4499_112_2_18_0_5_5_220,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4507_112_2_18_0_5_5_221,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4514_112_2_18_0_5_5_222,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4521_112_2_18_0_5_5_223,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4529_112_2_18_0_5_5_224,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4536_112_2_18_0_5_5_225,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4542_112_2_18_0_5_5_226,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4548_112_2_18_0_5_5_227,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4558_112_2_18_0_5_5_228,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4564_112_2_18_0_5_5_229,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_4770_112_2_18_0_5_5_230,"MACCSN_DecodeNcFreqList:ASSERT,add_freq_str.freq_diff_len is 0,return FALSE")
TRACE_MSG(MAC_CSN_MSG_6503_112_2_18_0_5_9_231,"MAC:Error,the Access Technology Type num beyond 7!")
TRACE_MSG(MAC_CSN_MSG_6588_112_2_18_0_5_9_232,"MAC:DL PWR CTRL %d, PR MODE %d")
TRACE_MSG(MAC_CSN_MSG_6675_112_2_18_0_5_9_233,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6700_112_2_18_0_5_9_234,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6710_112_2_18_0_5_9_235,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6744_112_2_18_0_5_9_236,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6749_112_2_18_0_5_9_237,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6767_112_2_18_0_5_9_238,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6775_112_2_18_0_5_9_239,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6780_112_2_18_0_5_9_240,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6786_112_2_18_0_5_9_241,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6791_112_2_18_0_5_9_242,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6798_112_2_18_0_5_9_243,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6803_112_2_18_0_5_9_244,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6812_112_2_18_0_5_9_245,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6817_112_2_18_0_5_9_246,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6823_112_2_18_0_5_9_247,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6828_112_2_18_0_5_9_248,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6835_112_2_18_0_5_9_249,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6840_112_2_18_0_5_9_250,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6846_112_2_18_0_5_10_251,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6851_112_2_18_0_5_10_252,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6863_112_2_18_0_5_10_253,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6868_112_2_18_0_5_10_254,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6885_112_2_18_0_5_10_255,"PRR-encode: %d")
TRACE_MSG(MAC_CSN_MSG_6931_112_2_18_0_5_10_256,"MAC: Current cell band = %d")
TRACE_MSG(MAC_CSN_MSG_7659_112_2_18_0_5_12_257,"MAC:  RrPacketUlAssi fix alloc")
TRACE_MSG(MAC_CSN_MSG_7664_112_2_18_0_5_12_258,"MAC: EGPRS msg Ext alloc")
TRACE_MSG(MAC_CSN_MSG_7772_112_2_18_0_5_12_259,"MAC:DL PWR CTRL %d, MODE %d, PR MODE %d")
TRACE_MSG(MAC_DECODE_DATA_IND_322_112_2_18_0_5_14_260,"MAC: tfi ie not present")
TRACE_MSG(MAC_DECODE_DATA_IND_336_112_2_18_0_5_14_261,"MAC: msg rx on slot%d's global tfi matched the ul tfi, but not on the assigned ul slot!, ")
TRACE_MSG(MAC_DECODE_DATA_IND_355_112_2_18_0_5_14_262,"MAC: msg rx on slot%d's global tfi matched the dl tfi, but not on the assigned dl slot!, ")
TRACE_MSG(MAC_DECODE_DATA_IND_366_112_2_18_0_5_14_263,"MAC:Assert Failure, tfi type invalid")
TRACE_MSG(MAC_DECODE_DATA_IND_471_112_2_18_0_5_14_264,"MAC: ignore msg due to tlli mismatch")
TRACE_MSG(MAC_DECODE_DATA_IND_474_112_2_18_0_5_14_265,"MAC: match the old tlli")
TRACE_MSG(MAC_DECODE_DATA_IND_478_112_2_18_0_5_14_266,"MAC: match the new tlli")
TRACE_MSG(MAC_DECODE_DATA_IND_489_112_2_18_0_5_14_267,"MAC: Assert Failure,Invalid addr")
TRACE_MSG(MAC_DECODE_DATA_IND_576_112_2_18_0_5_14_268,"MAC: ignore msg due to tlli mismatch")
TRACE_MSG(MAC_DECODE_DATA_IND_579_112_2_18_0_5_14_269,"MAC: match the old tlli")
TRACE_MSG(MAC_DECODE_DATA_IND_583_112_2_18_0_5_14_270,"MAC: match the new tlli")
TRACE_MSG(MAC_DECODE_DATA_IND_590_112_2_18_0_5_14_271,"MAC: Assert Failure,measur order msg error")
TRACE_MSG(MAC_DECODE_DATA_IND_617_112_2_18_0_5_14_272,"MAC: ignore msg due to tlli mismatch")
TRACE_MSG(MAC_DECODE_DATA_IND_620_112_2_18_0_5_14_273,"MAC: match the old tlli")
TRACE_MSG(MAC_DECODE_DATA_IND_624_112_2_18_0_5_14_274,"MAC: match the new tlli")
TRACE_MSG(MAC_DECODE_DATA_IND_631_112_2_18_0_5_14_275,"MAC: Assert Failure,polling req msg error")
TRACE_MSG(MAC_DECODE_DATA_IND_643_112_2_18_0_5_14_276,"MAC:The packet queueing nitify message has not request reference!")
TRACE_MSG(MAC_DECODE_DATA_IND_698_112_2_18_0_5_14_277,"MAC: PAN on slot%d's tfi matched the ul tfi, but not on the assigned ul slot!, ")
TRACE_MSG(MAC_DECODE_DATA_IND_739_112_2_18_0_5_14_278,"MAC: Assert Failure,unknown msg")
TRACE_MSG(MAC_DECODE_DATA_IND_745_112_2_18_0_5_14_279,"MAC: MSG decode error!")
TRACE_MSG(MAC_DECODE_DATA_IND_749_112_2_18_0_5_14_280,"MAC: MSG decode error!")
TRACE_MSG(MAC_DECODE_DATA_IND_755_112_2_18_0_5_14_281,"MAC: MSG decode error!")
TRACE_MSG(MAC_DECODE_DATA_IND_865_112_2_18_0_5_15_282,"MAC: Assert Failure,unknow decode msg error")
TRACE_MSG(MAC_DECODE_DATA_IND_970_112_2_18_0_5_15_283,"MAC:CTRL REASSEMBLE:T3200 EXPIRE TS %d")
TRACE_MSG(MAC_DECODE_DATA_IND_999_112_2_18_0_5_15_284,"MAC: msg rx on slot%d's header tfi is dl tfi, but not on the assigned dl slot!, ")
TRACE_MSG(MAC_DECODE_DATA_IND_1011_112_2_18_0_5_15_285,"MAC: msg rx on slot%d's header tfi is ul tfi, but not on the assigned ul slot!, ")
TRACE_MSG(MAC_DECODE_DATA_IND_1052_112_2_18_0_5_15_286,"MAC:CTRL REASSEMBLE PDCH TS %d")
TRACE_MSG(MAC_DECODE_DATA_IND_1130_112_2_18_0_5_15_287,"MAC: Assert Failure,assemb control eror")
TRACE_MSG(MAC_DECODE_DATA_IND_1139_112_2_18_0_5_15_288,"MAC:CTRL REASSEMBLE SAVE BLOCK TS %d")
TRACE_MSG(MAC_DECODE_DATA_IND_1209_112_2_18_0_5_15_289,"MAC: invalid rrbp value,ignore it!!")
TRACE_MSG(MAC_DECODE_DATA_IND_1260_112_2_18_0_5_15_290,"MAC: recv data block on slot%d tfi match but not on the assigned dl slots")
TRACE_MSG(MAC_DECODE_DATA_IND_1351_112_2_18_0_5_16_291,"MAC: recv egprs data block on slot%d tfi match but not on the assigned dl slots")
TRACE_MSG(MAC_DECODE_DATA_IND_1507_112_2_18_0_5_16_292,"MAC:Receive MCSERROR data!")
TRACE_MSG(MAC_DECODE_DATA_IND_1512_112_2_18_0_5_16_293,"MAC:Receive unexpected send_type data!")
TRACE_MSG(MAC_DECODE_DATA_IND_1749_112_2_18_0_5_16_294,"MAC: discard the second ul ack")
TRACE_MSG(MAC_DECODE_DATA_IND_1756_112_2_18_0_5_16_295,"MAC: ingore msg %x")
TRACE_MSG(MAC_DECODE_DATA_IND_1767_112_2_18_0_5_16_296,"MAC: in wrong state receive packet uplink ack/unack")
TRACE_MSG(MAC_DECODE_DATA_IND_1834_112_2_18_0_5_17_297,"MAC: presdo tbf starting time, pkt dl assignment!")
TRACE_MSG(MAC_DECODE_DATA_IND_1843_112_2_18_0_5_17_298,"MAC: presdo tbf starting time, pkt ul assignment!")
TRACE_MSG(MAC_DECODE_DATA_IND_1852_112_2_18_0_5_17_299,"MAC: presdo tbf starting time, pkt ts reconfig!")
TRACE_MSG(MAC_DECODE_DATA_IND_1871_112_2_18_0_5_17_300,"MAC: unknown msg type %d")
TRACE_MSG(MAC_DECODE_DATA_IND_1913_112_2_18_0_5_17_301,"MAC: Ingore the polling flag!")
TRACE_MSG(MAC_DECODE_DATA_IND_1933_112_2_18_0_5_17_302,"MAC: Assert Failure,Invalid control ack type")
TRACE_MSG(MAC_DECODE_DATA_IND_1963_112_2_18_0_5_17_303,"MAC: Assert failure1,Invalid control ack type")
TRACE_MSG(MAC_DECODE_DATA_IND_2047_112_2_18_0_5_17_304,"MAC:wrong MCS/CS, discard invalid data")
TRACE_MSG(MAC_DECODE_DATA_IND_2117_112_2_18_0_5_17_305,"MAC: error payload type %d")
TRACE_MSG(MAC_DECODE_DATA_IND_2145_112_2_18_0_5_17_306,"SCT loop 8psk data fail")
TRACE_MSG(MAC_DECODE_DATA_IND_2217_112_2_18_0_5_18_307,"MAC: error payload type %d")
TRACE_MSG(MAC_DECODE_DATA_IND_2301_112_2_18_0_5_18_308,"MAC: error,pdch assi invalid pd type %d")
TRACE_MSG(MAC_DECODE_DATA_IND_2358_112_2_18_0_5_18_309,"MAC:error mutislot control message!")
TRACE_MSG(MAC_DECODE_DATA_IND_2419_112_2_18_0_5_18_310,"MAC: tlli mismatch in PAR msg!Select next!")
TRACE_MSG(MAC_DECODE_MA_440_112_2_18_0_5_19_311,"MAC: GPRS class > 12 %d")
TRACE_MSG(MAC_DECODE_MA_453_112_2_18_0_5_19_312,"MAC: Multislot mismatch, UL timeslot assi num: %d")
TRACE_MSG(MAC_DECODE_MA_461_112_2_18_0_5_19_313,"MAC: Multislot mismatch, DL timeslot assi num: %d")
TRACE_MSG(MAC_DECODE_MA_470_112_2_18_0_5_19_314,"MAC: Multislot mismatch, UL timeslot assi num: %d DL timeslot assi num: %d")
TRACE_MSG(MAC_DECODE_MA_498_112_2_18_0_5_19_315,"MAC: UL slot %x mismatch DL slot %x")
TRACE_MSG(MAC_DECODE_MA_553_112_2_18_0_5_19_316,"MAC: polling slot before first rx slot, discard!")
TRACE_MSG(MAC_DECODE_MA_560_112_2_18_0_5_19_317,"MAC: old polling occupy new assignment slot after changed, discard!")
TRACE_MSG(MAC_DECODE_MA_567_112_2_18_0_5_19_318,"MAC: check polling validate, polling slot and the rx slot collision")
TRACE_MSG(MAC_DECODE_MA_606_112_2_18_0_5_20_319,"MAC: GPRS class > 12 %d")
TRACE_MSG(MAC_DECODE_MA_611_112_2_18_0_5_20_320,"MAC: check polling validate, violates MS capability")
TRACE_MSG(MAC_DECODE_MA_833_112_2_18_0_5_20_321,"MAC: Decode ma error, ref arfcn num=0!")
TRACE_MSG(MAC_DECODE_MA_881_112_2_18_0_5_20_322,"MAC: Decode ma error, arfcn num!=bitmap len %d %d!")
TRACE_MSG(MAC_DECODE_MA_944_112_2_18_0_5_20_323,"MAC: Decode ma error, arfcn num=0!")
TRACE_MSG(MAC_DECODE_MA_951_112_2_18_0_5_20_324,"MAC: Decode ma error, arfcn not in band!")
TRACE_MSG(MAC_DECODE_MA_1007_112_2_18_0_5_20_325,"MAC: Freq para update error in PSI2, invalid ma number!")
TRACE_MSG(MAC_DECODE_MA_1019_112_2_18_0_5_20_326,"MAC: Freq para update error in PSI2, decode ma error!")
TRACE_MSG(MAC_DECODE_MA_1045_112_2_18_0_5_20_327,"MAC: Freq para update error in SI13, decode ma error!")
TRACE_MSG(MAC_DECODE_MA_1066_112_2_18_0_5_20_328,"MAC: Invalid freq param type")
TRACE_MSG(MAC_DECODE_MA_1100_112_2_18_0_5_21_329,"MAC: Decode freq para error, decode ma error,freq no in band!")
TRACE_MSG(MAC_DECODE_MA_1115_112_2_18_0_5_21_330,"MAC: Decode freq para error, invalid ma number %d!")
TRACE_MSG(MAC_DECODE_MA_1124_112_2_18_0_5_21_331,"MAC: Decode freq para error, ma number not referenced %d!")
TRACE_MSG(MAC_DECODE_MA_1135_112_2_18_0_5_21_332,"MAC: Decode freq para error, ma number=15 but change mark present!")
TRACE_MSG(MAC_DECODE_MA_1159_112_2_18_0_5_21_333,"MAC: Decode freq para error, change mark not match!")
TRACE_MSG(MAC_DECODE_MA_1167_112_2_18_0_5_21_334,"MAC: Decode freq para error, change mark not match!")
TRACE_MSG(MAC_DECODE_MA_1188_112_2_18_0_5_21_335,"MAC: Decode freq para error, decode ma error!")
TRACE_MSG(MAC_DECODE_MA_1206_112_2_18_0_5_21_336,"MAC: Decode freq bitmap error!")
TRACE_MSG(MAC_DECODE_MA_1213_112_2_18_0_5_21_337,"MAC: Decode freq para error, decode ma error,freq no in band!")
TRACE_MSG(MAC_DECODE_MA_1226_112_2_18_0_5_21_338,"MAC: Assert Failure,Invalid freq pars selector %d")
TRACE_MSG(MAC_DECODE_MA_1476_112_2_18_0_5_21_339,"MAC: alloc extended dynamic allocation, but not support")
TRACE_MSG(MAC_DECODE_MA_1481_112_2_18_0_5_21_340,"MAC: network alloc EXTENDED_DYNAMIC!")
TRACE_MSG(MAC_DECODE_MA_1534_112_2_18_0_5_21_341,"MAC: dynamic allocation, allocation no ul slot")
TRACE_MSG(MAC_DECODE_MA_1541_112_2_18_0_5_21_342,"MAC: violate ms multislot capability")
TRACE_MSG(MAC_DECODE_MA_1562_112_2_18_0_5_21_343,"MAC: receive pkt uplink ass with no tfi when establish uplink TBF")
TRACE_MSG(MAC_DECODE_MA_1661_112_2_18_0_5_22_344,"MAC: single tbf get slot mask, not on starting time, st = %d, fn = %d")
TRACE_MSG(MAC_DECODE_MA_1789_112_2_18_0_5_22_345,"absolute frame number")
TRACE_MSG(MAC_DECODE_MA_1798_112_2_18_0_5_22_346,"MAC:MACMA_DecodeUlMultiBlockAllocation:%d")
TRACE_MSG(MAC_DECODE_MA_1860_112_2_18_0_5_22_347,"MAC: Decode freq para error, decode ma error,freq no in band!")
TRACE_MSG(MAC_DECODE_MA_1884_112_2_18_0_5_22_348,"MAC: Decode packet channel descri freq error(invalid ma number)!\n")
TRACE_MSG(MAC_DECODE_MA_1900_112_2_18_0_5_22_349,"MAC: Decode packet channel descri freq error(change mark not match)\n")
TRACE_MSG(MAC_DECODE_MA_1918_112_2_18_0_5_22_350,"MAC: Decode packet channel descri freq error(num=0)\n")
TRACE_MSG(MAC_DECODE_MA_1950_112_2_18_0_5_22_351,"MAC: invalid polling frame number=%d")
TRACE_MSG(MAC_DECODE_MA_1977_112_2_18_0_5_22_352,"MAC:Invalid control ack type for response imm ass!")
TRACE_MSG(MAC_DECODE_MA_2171_112_2_18_0_5_23_353,"MAC: network alloc fix allocation,ms not support!")
TRACE_MSG(MAC_DECODE_MA_2305_112_2_18_0_5_23_354,"MAC: network alloc fix allocation,ms not support!")
TRACE_MSG(MAC_DECODE_MA_2353_112_2_18_0_5_23_355,"MAC: tlli mismatch in DL assi! MS: %x  Assi: %x")
TRACE_MSG(MAC_DECODE_MA_2356_112_2_18_0_5_23_356,"MAC: match the old tlli")
TRACE_MSG(MAC_DECODE_MA_2360_112_2_18_0_5_23_357,"MAC: match the new tlli")
TRACE_MSG(MAC_DECODE_MA_2369_112_2_18_0_5_23_358,"MAC:imm ass without egprs content whereascurrent TBF mode is EGPRS, ignore message!")
TRACE_MSG(MAC_DECODE_MA_2551_112_2_18_0_5_23_359,"MAC: No ta_value in global_timing_advance!")
TRACE_MSG(MAC_DECODE_MA_2606_112_2_18_0_5_24_360,"MAC: No valid tfi to encode in pccn msg")
TRACE_MSG(MAC_DECODE_MA_2620_112_2_18_0_5_24_361,"MAC: rlc add %d control msg")
TRACE_MSG(MAC_DECODE_MA_2949_112_2_18_0_5_24_362,"MAC: prach persist_level is wrong!")
TRACE_MSG(MAC_LOGEL_MSG_1747_112_2_18_0_5_29_363,"MAC:ERROR!L in EGPRS Ack/Nack Description IE = %d,beyond range!!\n")
TRACE_MSG(MAC_LOGEL_MSG_1772_112_2_18_0_5_29_364,"MAC:L_length =%d is not enough when encode crbb_len!!\n")
TRACE_MSG(MAC_LOGEL_MSG_1783_112_2_18_0_5_29_365,"MAC:L_length =%d is not enough when encode comp_bitmap_start_color_code!!\n")
TRACE_MSG(MAC_LOGEL_MSG_1828_112_2_18_0_5_29_366,"MAC:L_length =%d is not enough when encode crbb!!\n")
TRACE_MSG(MAC_LOGEL_MSG_1876_112_2_18_0_5_29_367,"MAC:L_length =%d is not enough when encode urbb!!\n")
TRACE_MSG(MAC_LOGEL_MSG_4924_112_2_18_0_5_35_368,"MAC: EGPRS msg BEP")
TRACE_MSG(MAC_LOGEL_MSG_4929_112_2_18_0_5_35_369,"MAC: EGPRS msg ETimeslot")
TRACE_MSG(MAC_PH_371_112_2_18_0_5_38_370,"MAC Start t3206!")
TRACE_MSG(MAC_PH_399_112_2_18_0_5_38_371,"MAC:rr add msg =%d have invalid tfi,add fail!")
TRACE_MSG(MAC_PH_484_112_2_18_0_5_38_372,"MAC:MACPH_DelElementInCtrlQ find no element in Ctrl!")
TRACE_MSG(MAC_PH_499_112_2_18_0_5_38_373,"MAC:del num > amount in control queue,check!")
TRACE_MSG(MAC_PH_629_112_2_18_0_5_38_374,"MAC: add the second timeslot polling data!")
TRACE_MSG(MAC_PH_653_112_2_18_0_5_38_375,"MAC: replace the more than 2 timeslot polling data!")
TRACE_MSG(MAC_PH_658_112_2_18_0_5_38_376,"MAC: discard the more than 2 timeslot polling data!")
TRACE_MSG(MAC_PH_784_112_2_18_0_5_39_377,"pkt-resource-req: psi2 change mark")
TRACE_MSG(MAC_PH_791_112_2_18_0_5_39_378,"pkt-resource-req: psi13 change mark")
TRACE_MSG(MAC_PH_797_112_2_18_0_5_39_379,"pkt-resource-req: psi change mark not present")
TRACE_MSG(MAC_PH_917_112_2_18_0_5_39_380,"MAC:net_command_bitmap = %d,common_bitmap=%d")
TRACE_MSG(MAC_PH_1060_112_2_18_0_5_39_381,"MAC: get actual band = %d")
TRACE_MSG(MAC_PH_1158_112_2_18_0_5_39_382,"MAC: network access_tech_type is GSM_P,but we don't support in nv!")
TRACE_MSG(MAC_PH_1172_112_2_18_0_5_39_383,"MAC: network access_tech_type is GSM_E,but we don't support in nv!")
TRACE_MSG(MAC_PH_1186_112_2_18_0_5_39_384,"MAC: network access_tech_type is GSM_1800,but we don't support in nv!")
TRACE_MSG(MAC_PH_1200_112_2_18_0_5_40_385,"MAC: network access_tech_type is GSM_1900,but we don't support in nv!")
TRACE_MSG(MAC_PH_1214_112_2_18_0_5_40_386,"MAC: network access_tech_type is GSM_850,but we don't support in nv!")
TRACE_MSG(MAC_PH_1221_112_2_18_0_5_40_387,"MAC:requested access type %d not supported!")
TRACE_MSG(MAC_PH_1240_112_2_18_0_5_40_388,"MAC:can only encode %d access tech type,the next should be encoded is %d")
TRACE_MSG(MAC_PH_1257_112_2_18_0_5_40_389,"MAC:ms_ra_capability_ptr->num_of_elems=%d,common_band_num= %d")
TRACE_MSG(MAC_PH_1260_112_2_18_0_5_40_390,"MAC :access_tech_req_num =%d")
TRACE_MSG(MAC_PH_1309_112_2_18_0_5_40_391,"pkt-resource-req: sign var not present, is two phase %d")
TRACE_MSG(MAC_PH_1316_112_2_18_0_5_40_392,"pkt-resource-req: var present")
TRACE_MSG(MAC_PH_1330_112_2_18_0_5_40_393,"pkt-resource-req: 2 phase")
TRACE_MSG(MAC_PH_1358_112_2_18_0_5_40_394,"pkt-resource-req: racap 2")
TRACE_MSG(MAC_PH_1373_112_2_18_0_5_40_395,"pkt-resource-req: bcch")
TRACE_MSG(MAC_PH_1505_112_2_18_0_5_40_396,"MAC:no EGPRS Timeslot Link Quality Measurements Information! bits= %d")
TRACE_MSG(MAC_PH_1631_112_2_18_0_5_40_397,"MAC:Encode ARAC, but no access technology in ul assign!!!")
TRACE_MSG(MAC_PH_1827_112_2_18_0_5_41_398,"MAC: get block %d to send in poll")
TRACE_MSG(MAC_PH_1911_112_2_18_0_5_41_399,"MAC:send packet resource request, don't start 3168")
TRACE_MSG(MAC_PH_1986_112_2_18_0_5_41_400,"MAC: GMSK BEP: mean = %d, cv = %d")
TRACE_MSG(MAC_PH_2002_112_2_18_0_5_41_401,"MAC: 8PSK BEP: mean = %d, cv = %d")
TRACE_MSG(MAC_PH_2240_112_2_18_0_5_42_402,"MAC:send PCCN instead of packet downlink ack!")
TRACE_MSG(MAC_PH_2300_112_2_18_0_5_42_403,"MAC:Mac is already start t3192 when send dl ack!")
TRACE_MSG(MAC_PH_2373_112_2_18_0_5_42_404,"MAC:send PCCN instead of egprs packet downlink ack!")
TRACE_MSG(MAC_PH_2409_112_2_18_0_5_42_405,"MAC:Mac is already start t3192 when send egprs dl ack!")
TRACE_MSG(MAC_PH_2486_112_2_18_0_5_42_406,"MAC: Invalid type of polling block")
TRACE_MSG(MAC_PH_2581_112_2_18_0_5_42_407,"MAC: send pccn from CTRL QUEUE!")
TRACE_MSG(MAC_PH_2663_112_2_18_0_5_42_408,"MAC: delte ctrl queue find pccn")
TRACE_MSG(MAC_PH_2935_112_2_18_0_5_43_409,"MAC: polling message type error")
TRACE_MSG(MAC_PH_2966_112_2_18_0_5_43_410,"MAC: send pccn from poll queue!")
TRACE_MSG(MAC_PH_2991_112_2_18_0_5_43_411,"MAC: No valid tfi to encode in pccn msg")
TRACE_MSG(MAC_PH_3005_112_2_18_0_5_43_412,"mac:uplink tfi in pccn =%d")
TRACE_MSG(MAC_PH_3013_112_2_18_0_5_43_413,"mac:downlink tfi in pccn =%d")
TRACE_MSG(MM_C_CODE_289_112_2_18_0_5_55_414,"MM: decode Msg Header,MESSAGE_TOO_SHORT")
TRACE_MSG(MM_C_CODE_317_112_2_18_0_5_55_415,"MM: decode Msg Header,SKIP_INDICATOR_NOT_ZERO")
TRACE_MSG(MM_C_CODE_351_112_2_18_0_5_55_416,"MM: decode Msg Header,INVALID_MESSAGE_TYPE")
TRACE_MSG(MM_C_CODE_363_112_2_18_0_5_55_417,"MM: decode Msg Header,INVALID_PROTOCOL_DISCRIMINATOR")
TRACE_MSG(MM_C_CODE_430_112_2_18_0_5_55_418,"MM: decode Location updating accept msg,min len error.")
TRACE_MSG(MM_C_CODE_441_112_2_18_0_5_55_419,"MM: decode Location updating accept msg,LAI invalid.")
TRACE_MSG(MM_C_CODE_449_112_2_18_0_5_55_420,"MM:Location updating accept's LAI != current cell LAI")
TRACE_MSG(MM_C_CODE_471_112_2_18_0_5_55_421,"MM: decode Location updating accept msg,%d")
TRACE_MSG(MM_C_CODE_495_112_2_18_0_5_55_422,"MM: decode MM_MOBILE_IDENTITY_IEI failure")
TRACE_MSG(MM_C_CODE_541_112_2_18_0_5_55_423,"MM: decode MM_EMERGENCY_NUM_LIST_IEI failure")
TRACE_MSG(MM_C_CODE_546_112_2_18_0_5_55_424,"MM: receive wrong msg %d")
TRACE_MSG(MM_C_CODE_572_112_2_18_0_5_55_425,"MM: decode Location updating reject msg,MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MM_C_CODE_599_112_2_18_0_5_55_426,"MM CODEC: Assert Failure, EncodeLocUpdReq: CKSN %d error, change to invalid")
TRACE_MSG(MM_C_CODE_664_112_2_18_0_5_55_427,"MM: decode Authentication request msg,MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MM_C_CODE_727_112_2_18_0_5_55_428,"MM: decode Identity request msg,MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MM_C_CODE_741_112_2_18_0_5_56_429,"MM: decode Identity request msg,MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MM_C_CODE_814_112_2_18_0_5_56_430,"MM: decode TMSI reallocation command msg,min len error")
TRACE_MSG(MM_C_CODE_822_112_2_18_0_5_56_431,"MM: decode TMSI reallocation command msg,LAI failure")
TRACE_MSG(MM_C_CODE_830_112_2_18_0_5_56_432,"MM:TMSI reallocation command's LAI != current cell LAI")
TRACE_MSG(MM_C_CODE_841_112_2_18_0_5_56_433,"MM: decode TMSI reallocation command msg,Mobile Identity failure")
TRACE_MSG(MM_C_CODE_887_112_2_18_0_5_56_434,"MM: decode CM service reject msg,MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MM_C_CODE_912_112_2_18_0_5_56_435,"MM: decode CM service reject msg,MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MM_C_CODE_979_112_2_18_0_5_56_436,"MM CODEC: Assert Failure, EncodeCMServiceReq: CKSN %d error, change to invalid")
TRACE_MSG(MM_C_CODE_1035_112_2_18_0_5_56_437,"MM CODEC: Assert Failure, EncodeCMReestReq: CKSN %d error, change to invalid")
TRACE_MSG(MM_C_CODE_1098_112_2_18_0_5_56_438,"MM: decode Abort msg,MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MM_C_CODE_1123_112_2_18_0_5_56_439,"MM CODEC: Assert Failure, EncodeMMStatus: Reject cause %d error, set to Protocol error, unspecified")
TRACE_MSG(MM_C_CODE_1243_112_2_18_0_5_57_440,"MM CODEC: Assert Failure, DecodeMobileIdentity: offset %d error, there is no MobileIdentity to decode")
TRACE_MSG(MM_C_CODE_1734_112_2_18_0_5_57_441,"MM: IsIeiValid temp_byte=%d, offset = %d, l3_msg_len = %d")
TRACE_MSG(MM_C_CODE_1766_112_2_18_0_5_58_442,"MM_LaiIsInValidPlmnList result = %d")
TRACE_MSG(RLC_DOWNLINK_645_112_2_18_0_6_4_443,"RLC: too many option octet in data block: %d\n")
TRACE_MSG(RLC_DOWNLINK_651_112_2_18_0_6_4_444,"RLC: total data len error in data block: %d\n")
TRACE_MSG(RLC_DOWNLINK_742_112_2_18_0_6_4_445,"RLC: too many option octet in data block: %d\n")
TRACE_MSG(RLC_DOWNLINK_748_112_2_18_0_6_4_446,"RLC: total data len error in data block: %d\n")
TRACE_MSG(RLC_DOWNLINK_904_112_2_18_0_6_4_447,"RLC: Reserved M+E: ME = '00'B\n")
TRACE_MSG(RLC_DOWNLINK_1005_112_2_18_0_6_5_448,"RLC: Error option octet: LI = %d, E = %d\n")
TRACE_MSG(RLC_MS_PARAM_49_112_2_18_0_6_8_449,"rlcul: MS support extended uplink TBF: %d")
TRACE_MSG(RLC_NET_PARAM_252_112_2_18_0_6_9_450,"rlcul: network support extended uplink TBF: %d")
TRACE_MSG(RLC_SCT_799_112_2_18_0_6_10_451,"rlcul: in loopback mode no data recved yet")
TRACE_MSG(RLC_TX_PDU_POOL_73_112_2_18_0_6_13_452,"rlcul: alloc pdu, pid = %d")
TRACE_MSG(RLC_TX_PDU_POOL_96_112_2_18_0_6_13_453,"rlcul: free pdu, pid = %d")
TRACE_MSG(RLC_UPLINK_821_112_2_18_0_6_15_454,"rlcul: release error!")
TRACE_MSG(RLC_UPLINK_2588_112_2_18_0_6_18_455,"rlcul: ext utbf, sending new block, reset t3182")
TRACE_MSG(RLC_UPLINK_2660_112_2_18_0_6_18_456,"rlcul: all data block have been send, still get data block\n")
TRACE_MSG(RLC_UPLINK_3972_112_2_18_0_6_21_457,"rlcul: gprs tx blk cs%d bsn = %d, va = %d, vs = %d")
TRACE_MSG(RLC_UPLINK_3981_112_2_18_0_6_21_458,"rlcul: gprs retx %s blk cs%d bsn = %d, va = %d, vs = %d")
TRACE_MSG(RLC_UPLINK_4095_112_2_18_0_6_21_459,"rlcul: egprs tx blk mcs%d bsn = %d, va = %d, vs = %d")
TRACE_MSG(RLC_UPLINK_4104_112_2_18_0_6_21_460,"rlcul: egprs retx %s blk mcs%d bsn = %d, va = %d, vs = %d")
TRACE_MSG(RLC_UPLINK_4126_112_2_18_0_6_21_461,"rlcul: egprs retx %s blk mcs%d bsn = %d, va = %d, vs = %d")
TRACE_MSG(RLC_UPLINK_4132_112_2_18_0_6_21_462,"rlcul: egprs tx blk mcs%d bsn = %d, va = %d, vs = %d")
TRACE_MSG(RLC_UPLINK_4351_112_2_18_0_6_22_463,"rlcul: in RLCUP_ProcessAckUnackMsg sendnumcnf")
TRACE_MSG(RLC_UPLINK_4547_112_2_18_0_6_22_464,"rlcul: decode crbb error!\n")
TRACE_MSG(RLC_UPLINK_4678_112_2_18_0_6_23_465,"rlcul: in RLCUP_EGPRS_ProcessAckNackMsg sendnumcnf")
TRACE_MSG(RLC_UPLINK_5329_112_2_18_0_6_24_466,"rlcul: in change tlli sendnumcnf")
TRACE_MSG(RLC_UPLINK_5544_112_2_18_0_6_24_467,"rlcul: in change tlli sendnumcnf")
TRACE_MSG(RLC_UPLINK_6023_112_2_18_0_6_25_468,"rlcul:Error!unkown tlli_present!")
TRACE_MSG(RLC_UPLINK_6042_112_2_18_0_6_25_469,"rlcul: in change tlli sendnumcnf")
TRACE_MSG(RLC_UPLINK_6077_112_2_18_0_6_25_470,"rlcul: update timeslot mismatch! Error")
TRACE_MSG(RLC_UPLINK_6261_112_2_18_0_6_26_471,"rlcul: set flow ctrl, llc num = %d, spare num = %d, line = %d")
TRACE_MSG(RLC_UPLINK_6274_112_2_18_0_6_26_472,"rlcul: set flow ctrl, octet num = %d, line = %d")
TRACE_MSG(RLC_UPLINK_6300_112_2_18_0_6_26_473,"rlcul: reset flow ctrl, line = %d")
TRACE_MSG(RR2RRC_PRIM_148_112_2_18_0_6_57_474,"DM: handle ho cmd , message error")
TRACE_MSG(RR2RRC_PRIM_154_112_2_18_0_6_57_475,"DM: handle ho cmd , PD or msg-type ERROR")
TRACE_MSG(RR2RRC_PRIM_160_112_2_18_0_6_57_476,"DM: call decode function to decode ho cmd")
TRACE_MSG(RR2RRC_PRIM_194_112_2_18_0_6_57_477,"TRR has sent msg %s to GSM. multi_sys=%d.")
TRACE_MSG(RR2RRC_PRIM_233_112_2_18_0_6_58_478,"Invalid rr context pointer.")
TRACE_MSG(RR2RRC_PRIM_241_112_2_18_0_6_58_479,"RRDM_Handle_Ccofu_callback: ASSERT! gsm msg num is too large = %d")
TRACE_MSG(RR2RRC_PRIM_246_112_2_18_0_6_58_480,"CALLBACK Rev RAT CHG MSG , type = %d")
TRACE_MSG(RR2RRC_PRIM_313_112_2_18_0_6_58_481,"Invalid rr context pointer.")
TRACE_MSG(RR2RRC_PRIM_322_112_2_18_0_6_58_482,"Trr_handle_rej_to_2G_callback: ho_type is error, ho_type = %d")
TRACE_MSG(RR2RRC_PRIM_376_112_2_18_0_6_58_483,"Trr_handle_ho_to_gsm_callback:Invalid rr context pointer.")
TRACE_MSG(RR2RRC_PRIM_391_112_2_18_0_6_58_484,"RRDM: TD Handover To GSM start.")
TRACE_MSG(RR2RRC_PRIM_488_112_2_18_0_6_58_485,"inter RAT HO from UTRAN failed for msg error, msg_error =%d")
TRACE_MSG(RR2RRC_PRIM_520_112_2_18_0_6_58_486,"Trr_arfcn_in_gsmr_band:Invalid rr context pointer.")
TRACE_MSG(RR2RRC_PRIM_528_112_2_18_0_6_58_487,"Trr_arfcn_in_gsmr_band: this arfcn is GSM-R band,multi_sys=%d,arfcn=%d")
TRACE_MSG(RR_CSN_MSG_4514_112_2_18_0_7_7_488,"RRC:received System Information 9, not supported now!")
TRACE_MSG(RR_CSN_MSG_4538_112_2_18_0_7_7_489,"RRC:received System Information 16, not supported now!")
TRACE_MSG(RR_CSN_MSG_4548_112_2_18_0_7_7_490,"RRC:received System Information 17, not supported now!")
TRACE_MSG(RR_CSN_MSG_4558_112_2_18_0_7_7_491,"RRC:received System Information 18, not supported now!")
TRACE_MSG(RR_CSN_MSG_4568_112_2_18_0_7_7_492,"RRC:received System Information 19, not supported now!")
TRACE_MSG(RR_CSN_MSG_4579_112_2_18_0_7_7_493,"RRC:received System Information 20, not supported now!")
TRACE_MSG(RR_CSN_MSG_6754_112_2_18_0_7_12_494,"RRC: 2 quater,  index %d, count %d")
TRACE_MSG(RR_CSN_MSG_6865_112_2_18_0_7_12_495,"rrc: no Optional Selection Parameters in Si4RestOctets, line = %d")
TRACE_MSG(RR_CSN_MSG_6928_112_2_18_0_7_12_496,"RRC: si2quater available")
TRACE_MSG(RR_CSN_MSG_6953_112_2_18_0_7_12_497,"rrc: no Optional Selection Parameters in Si4RestOctets, line = %d")
TRACE_MSG(RR_CSN_MSG_6971_112_2_18_0_7_12_498,"RRC:receive LSA connent in System Information 8,not support!line=%d")
TRACE_MSG(RR_CSN_MSG_7091_112_2_18_0_7_12_499,"RRC: SGSNR %d")
TRACE_MSG(RR_CSN_MSG_7101_112_2_18_0_7_12_500,"RRC: si_status_ind %d")
TRACE_MSG(RR_ENG_98_112_2_18_0_7_13_501,"RRENG_Init: g_dm_rr_context[%d] has been already allocated.")
TRACE_MSG(RR_ENG_168_112_2_18_0_7_13_502,"RRENG_SetStackCfgParam: multi_sys=%d,params_ptr is NULL.")
TRACE_MSG(RR_ENG_208_112_2_18_0_7_14_503,"RRENG_GetStackCfgParam: multi_sys=%d,gsm_cfg_param_ptr is NULL.")
TRACE_MSG(RR_ENG_242_112_2_18_0_7_14_504,"RRENG_GetStackCfgParam:multi_sys=%d, %d, %d, %d ,%d, %d, %d,%d,%d,%d,%d,%d,%d,%d")
TRACE_MSG(RR_ENG_281_112_2_18_0_7_14_505,"RRENG_ResetGSMStackCfgParam:multi_sys=%d, %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d")
TRACE_MSG(RR_ENG_296_112_2_18_0_7_14_506,"RRENG_SetDMParam:multi_sys=%d,index=%d,value=%d")
TRACE_MSG(RR_ENG_343_112_2_18_0_7_14_507,"RRENG_SetDMParam: pinp guard is stopped.")
TRACE_MSG(RR_ENG_350_112_2_18_0_7_14_508,"RRENG_SetDMParam: recovery as default value:offset_step_rate=2,init_pinp_offset=3.")
TRACE_MSG(RR_ENG_414_112_2_18_0_7_14_509,"RRENG_SetDMParam: not support yet")
TRACE_MSG(RR_ENG_433_112_2_18_0_7_14_510,"RRENG_GetDMParam,multi_sys=%d, valu_ptr is NULL.")
TRACE_MSG(RR_ENG_477_112_2_18_0_7_14_511,"RRENG_GetDMParam: not support yet. multi_sys=%d")
TRACE_MSG(RR_ENG_481_112_2_18_0_7_14_512,"RRENG_GetDMParam:multi_sys=%d,index=%d,value=%d")
TRACE_MSG(RR_ENG_525_112_2_18_0_7_14_513,"RRENG_SetTDMeasSwitch: utran_meas_switch = %d")
TRACE_MSG(RR_ENG_550_112_2_18_0_7_14_514,"RRENG_GetTDMeasSwitch: utran_meas_switch = %d")
TRACE_MSG(RR_ENG_629_112_2_18_0_7_14_515,"RRENG_ReadTdNcellFromNV:multi_sys=%d, td_ncell_list_ptr->arfcn_num = %d")
TRACE_MSG(RR_ENG_660_112_2_18_0_7_14_516,"RRENG_SetDummyTdNcell:multi_sys=%d")
TRACE_MSG(RR_ENG_663_112_2_18_0_7_14_517,"RRENG_SetDummyTdNcell:ERROR->index value is wrong,maximum value is 9!")
TRACE_MSG(RR_ENG_668_112_2_18_0_7_14_518,"RRENG_SetDummyTdNcell:ERROR->index value should be set in succession!")
TRACE_MSG(RR_ENG_673_112_2_18_0_7_14_519,"RRENG_SetDummyTdNcell:ERROR->cell_param_num value is wrong,maximum value is 4!")
TRACE_MSG(RR_ENG_678_112_2_18_0_7_14_520,"RRENG_SetDummyTdNcell:ERROR->uarfcn value is wrong,minimum value is 10054!")
TRACE_MSG(RR_ENG_685_112_2_18_0_7_14_521,"RRENG_SetDummyTdNcell:INVALID UARFCN = %d from dummy")
TRACE_MSG(RR_ENG_701_112_2_18_0_7_15_522,"RRENG_SetDummyTdNcell:added UARFCN, index=%d")
TRACE_MSG(RR_ENG_703_112_2_18_0_7_15_523,"RRENG_SetDummyTdNcell:cell_id_num=%d,cell_id1=%d,cell_id2=%d,cell_id3=%d,cell_id4=%d")
TRACE_MSG(RR_ENG_707_112_2_18_0_7_15_524,"RRENG_SetDummyTdNcell:updated UARFCN, index=%d")
TRACE_MSG(RR_ENG_709_112_2_18_0_7_15_525,"RRENG_SetDummyTdNcell:cell_id_num=%d,cell_id1=%d,cell_id2=%d,cell_id3=%d,cell_id4=%d")
TRACE_MSG(RR_ENG_738_112_2_18_0_7_15_526,"RRENG_ResetDummyTdNcell: multi_sys=%d,DUMMY TDD NCELL is reset.")
TRACE_MSG(RR_ENG_777_112_2_18_0_7_15_527,"RRENG_SetG2URevertStatus: multi_sys=%d,invalid type:%d")
TRACE_MSG(RR_ENG_818_112_2_18_0_7_15_528,"RRENG_GetGsm2UtranRevertStatus: multi_sys=%d,invalid type:%d")
TRACE_MSG(RR_ENG_908_112_2_18_0_7_15_529,"RRENG_SetDummyPCCOTddCell: uarfcn=%d,cpi=%d")
TRACE_MSG(RR_ENG_1093_112_2_18_0_7_15_530,"RRENG_Set_PowerSweep_IsUsed_Ba:usedBa=%d power_sweep_used_ba = %d,power_sweep_unused_ba=%d")
TRACE_MSG(RR_ENG_1123_112_2_18_0_7_15_531,"RRENG_Set_PowerSweep_UseBa_IsCamp:power_sweep_used_ba_camp_suc_amount=%d,sweep_used_ba_camp_fail_amount=%d")
TRACE_MSG(RR_ENG_1141_112_2_18_0_7_15_532,"RRENG_Set_T2G_Resel_Fail_No_Revert: T2G_resel_fail_no_revert_amount = %d")
TRACE_MSG(RR_ENG_1160_112_2_18_0_7_15_533,"RRENG_Set_T2G_Resel_No_Revert: T2G_resel_no_revert_total_amount = %d")
TRACE_MSG(RR_ENG_1201_112_2_18_0_7_16_534,"weixia:G2G_resel_fail_amount=%d,G2G_resel_total_amount=%d,G2G_resel_succ_amount=%d")
TRACE_MSG(RR_ENG_1316_112_2_18_0_7_16_535,"RRENG_Add_TdNcell_ENG_PARAMS: eng td ncell list is full.")
TRACE_MSG(RR_ENG_1349_112_2_18_0_7_16_536,"RRENG: nv_item_ptr is null!!")
TRACE_MSG(RR_ENG_1391_112_2_18_0_7_16_537,"RRENG: si_2quarter_test[012] = %x%x%x")
TRACE_MSG(RR_ENG_1421_112_2_18_0_7_16_538,"RRENG_GetSiUpdateTimeDur: active_card_num = %d, extra_dur_one_card = %d,time_dur_ptr =%d")
TRACE_MSG(RRCELL_1756_112_2_18_0_7_25_539,"RRCELL_ReadSelParamInfo: multi_sys=%d invalid")
TRACE_MSG(RRCELL_1772_112_2_18_0_7_26_540,"RRCELL_ReadEplmnInfo: multi_sys=%d invalid")
TRACE_MSG(RRCELL_1787_112_2_18_0_7_26_541,"RRCELL_ReadClassMark: multi_sys=%d invalid")
TRACE_MSG(RRCELL_2146_112_2_18_0_7_26_542,"RRC: ref_sen %d, num %d")
TRACE_MSG(RRCELL_2151_112_2_18_0_7_26_543,"RRC: ref_sen %d, num %d")
TRACE_MSG(RRCELL_2156_112_2_18_0_7_26_544,"RRC: ref_sen is 0, num %d")
TRACE_MSG(RRCELL_2250_112_2_18_0_7_26_545,"RRC: clean rxlev count %d")
TRACE_MSG(RRCELL_2458_112_2_18_0_7_27_546,"RRC: set rxlev[%d] to 0")
TRACE_MSG(RRCELL_2529_112_2_18_0_7_27_547,"RRC: In RRCELL_SortRssi, freq_count=%d")
TRACE_MSG(RRCELL_2535_112_2_18_0_7_27_548,"RRC:ARFCN = 0x%x, band %d, rssi = %d\n")
TRACE_MSG(RRCELL_2581_112_2_18_0_7_27_549,"RRC: %d set arfcn %d to 0")
TRACE_MSG(RRCELL_2605_112_2_18_0_7_27_550,"RRC: %d set arfcn %d, index %d to 0")
TRACE_MSG(RRCELL_2628_112_2_18_0_7_27_551,"RRC: %d set arfcn %d, index %d to 0")
TRACE_MSG(RRCELL_2653_112_2_18_0_7_27_552,"RRC: %d set arfcn %d, index %d to 0")
TRACE_MSG(RRCELL_2677_112_2_18_0_7_28_553,"RRC: %d set arfcn %d, index %d to 0")
TRACE_MSG(RRCELL_2754_112_2_18_0_7_28_554,"RRC: %d clearC p %d a 0x%x")
TRACE_MSG(RRCELL_2775_112_2_18_0_7_28_555,"RRC: %d clearRc p %d a 0x%x")
TRACE_MSG(RRCELL_2897_112_2_18_0_7_28_556,"RRCELL_AdjustC1ByNV:Error, c1_ptr is NULL")
TRACE_MSG(RRCELL_2905_112_2_18_0_7_28_557,"RRCELL_AdjustC1ByNV:Error, rr_dm_context_ptr is NULL")
TRACE_MSG(RRCELL_3003_112_2_18_0_7_28_558,"RRC:In RRCELL_IsSuitableCell\n")
TRACE_MSG(RRCELL_3004_112_2_18_0_7_28_559,"RRC: %d ARFCN = %d, c1 = %d\n")
TRACE_MSG(RRCELL_3394_112_2_18_0_7_29_560,"RRC: %d In  RRCELL_IsAcceptableCell\n")
TRACE_MSG(RRCELL_3552_112_2_18_0_7_29_561,"RRC: %d In UpdateNCellRssi")
TRACE_MSG(RRCELL_3633_112_2_18_0_7_29_562,"RRC:RR NCELL: cell from store to temp, arfcn = %d, bsic = %d")
TRACE_MSG(RRCELL_3720_112_2_18_0_7_30_563,"RRC:RR NCELL: cell from backup to store, arfcn = %d, bsic = %d")
TRACE_MSG(RRCELL_3755_112_2_18_0_7_30_564,"RRC: NCELL cell from temp to backup, arfcn = %d, bsic = %d")
TRACE_MSG(RRCELL_3796_112_2_18_0_7_30_565,"RRC: RRCELL_UpdateNcellSi has NULL pointer param!!")
TRACE_MSG(RRCELL_4077_112_2_18_0_7_30_566,"RRC: arfcn %d, si7_not_present %d, ")
TRACE_MSG(RRCELL_4085_112_2_18_0_7_30_567,"RRC: arfcn %d, si8_not_present %d, ")
TRACE_MSG(RRCELL_4581_112_2_18_0_7_31_568,"RR_FindSuitableCellInSelectedPlmn:c2 > INVALID_C2 return TRUE in dual mode")
TRACE_MSG(RRCELL_4610_112_2_18_0_7_31_569,"RR_FindSuitableCellInSelectedPlmn:c2 <= INVALID_C2 return TRUE in dual mode")
TRACE_MSG(RRCELL_4675_112_2_18_0_7_31_570,"RR_FindSuitableCellInSelectedPlmn_C1_Low: return TRUE in dual mode.")
TRACE_MSG(RRCELL_5260_112_2_18_0_7_33_571,"RRC: assert unknown si %d")
TRACE_MSG(RRCELL_5300_112_2_18_0_7_33_572,"RRC: %d checkncellSi, cur %d")
TRACE_MSG(RRCELL_5360_112_2_18_0_7_33_573,"RRCELL_CheckNcellSysInfo: RRC: %d checkncellSi, another_rr_status = %d, si_update_dur = %d, NCELL_SI_UPDATE_PERIOD = %d")
TRACE_MSG(RRCELL_5538_112_2_18_0_7_33_574,"RRCELL_CheckNcellSysInfo: num_of_ncell_present = %d")
TRACE_MSG(RRCELL_5542_112_2_18_0_7_33_575,"RRC: cell %d, arfcn 0x%x, tc 0x%x new_tc 0x%x")
TRACE_MSG(RRCELL_5705_112_2_18_0_7_34_576,"RRC: GetReselPriority cell_ptr->cell_prio = %d ")
TRACE_MSG(RRCELL_5719_112_2_18_0_7_34_577,"RRC: %d reselprior %d arfcn 0x%x c1 %d")
TRACE_MSG(RRCELL_5723_112_2_18_0_7_34_578,"RRC: %d cell prio NO_CELL arfcn = %d, c1 = %d\n")
TRACE_MSG(RRCELL_5832_112_2_18_0_7_34_579,"RRC:In RRCELL_UpdateBcchSweepResult\n")
TRACE_MSG(RRCELL_5833_112_2_18_0_7_34_580,"RRC:ARFCN = %d, rssi = %d \n")
TRACE_MSG(RRCELL_5973_112_2_18_0_7_34_581,"RRCELL: (SCI_GetTickCount()-assert_tick)>300000,ERROR")
TRACE_MSG(RRCELL_6042_112_2_18_0_7_34_582,"RRC:%d In ReselTimer\n")
TRACE_MSG(RRCELL_6043_112_2_18_0_7_34_583,"RRC:A = 0x%x, rssi = %d, c1 = %d, c2 = %d\n")
TRACE_MSG(RRCELL_6047_112_2_18_0_7_34_584,"RRC:A = 0x%x, rssi = %d, c1 = %d, c2 = %d\n")
TRACE_MSG(RRCELL_6060_112_2_18_0_7_34_585,"RRC resel: card %d, scell: a=0x%x,c1=%d,c2=%d; max rssi ncell: a=0x%x,c1=%d,c2=%d; ct=%d")
TRACE_MSG(RRCELL_6091_112_2_18_0_7_34_586,"RRCELL: RR_IsAdditions, current bit is not 1.")
TRACE_MSG(RRCELL_6095_112_2_18_0_7_34_587,"RRCELL: RR_IsAdditions, offset_octets=%d > msg_length=%d.")
TRACE_MSG(RRCELL_6411_112_2_18_0_7_35_588,"RRCELL:RR_DecodeGPRSRepPriorityDescSt, ignored the last rep priority.")
TRACE_MSG(RRCELL_6576_112_2_18_0_7_35_589,"RR_DecodeGPRS3GMeasParamsDesc:search_p=%d,earch_prio_3g=%d,multirat_rep_present:%d,multirat_rep:%d,rep_off_and_thres_present:%d,rep_offset:%d,rep_threshold:%d")
TRACE_MSG(RRCELL_6657_112_2_18_0_7_35_590,"RR_DecodeGPRS3GMeasParamsBisDesc:search_p=%d,earch_prio_3g=%d,multirat_rep_present:%d,multirat_rep:%d,n rep_off_and_thres_present:%d,rep_offset:%d,rep_threshold:%d")
TRACE_MSG(RRCELL_6704_112_2_18_0_7_36_591,"RR_DecodeRemoved3GCellDesc, not save, and continue to decode the next IE.")
TRACE_MSG(RRCELL_6707_112_2_18_0_7_36_592,"RR_DecodeRemoved3GCellDesc:removed_cell_index=%d,remv_cell_diff_value=%d.")
TRACE_MSG(RRCELL_6830_112_2_18_0_7_36_593,"RR_DecodePcco3GNeighbourCell: Index_Start_3G included")
TRACE_MSG(RRCELL_6841_112_2_18_0_7_36_594,"RR_DecodePcco3GNeighbourCell: Index_Start_3G included")
TRACE_MSG(RRCELL_6852_112_2_18_0_7_36_595,"RR_DecodePcco3GNeighbourCell: FDD neighbor cell is assigned  in pcco!\n")
TRACE_MSG(RRCELL_6860_112_2_18_0_7_36_596,"RR_DecodePcco3GNeighbourCell: UTRAN TDD Description included")
TRACE_MSG(RRCELL_6909_112_2_18_0_7_36_597,"RR_DecodePMO3GNeighbourCell: Index_Start_3G included.offset_octets=%d,offset_bits=%d")
TRACE_MSG(RRCELL_6920_112_2_18_0_7_36_598,"RR_DecodePMO3GNeighbourCell: Index_Start_3G included.offset_octets=%d,offset_bits=%d")
TRACE_MSG(RRCELL_6931_112_2_18_0_7_36_599,"RR_DecodePMO3GNeighbourCell: FDD neighbor cell is assigned  in pcco!offset_octets=%d,offset_bits=%d")
TRACE_MSG(RRCELL_6939_112_2_18_0_7_36_600,"RR_DecodePMO3GNeighbourCell: UTRAN TDD Description included.offset_octets=%d,offset_bits=%d")
TRACE_MSG(RRCELL_6947_112_2_18_0_7_36_601,"RR_DecodePMO3GNeighbourCell: CDMA2000 Description included.offset_octets=%d,offset_bits=%d")
TRACE_MSG(RRCELL_6955_112_2_18_0_7_36_602,"RR_DecodePMO3GNeighbourCell: REMOVED 3GCELL Description included.offset_octets=%d,offset_bits=%d")
TRACE_MSG(RRCELL_7010_112_2_18_0_7_36_603,"RR_DecodeENHMeasParam, ENH Measurement parameters included. offset_octets=%d,offset_bits=%d")
TRACE_MSG(RRCELL_7046_112_2_18_0_7_36_604,"RR_DecodeENHMeasParam, pcco 3G Neighbour Cell Description included.offset_octets=%d,offset_bits=%d")
TRACE_MSG(RRCELL_7051_112_2_18_0_7_36_605,"RR_DecodeENHMeasParam,pmo 3G Neighbour Cell Description included.offset_octets=%d,offset_bits=%d")
TRACE_MSG(RRCELL_7079_112_2_18_0_7_36_606,"RR_DecodeENHMeasParam, GPRS 3G MEASUREMENT Parameters  included in PCCO. offset_octets=%d,offset_bits=%d")
TRACE_MSG(RRCELL_7089_112_2_18_0_7_36_607,"RR_DecodeENHMeasParam, GPRS 3G MEASUREMENT Parameters BIS desc  included in PMO.offset_octets=%d,offset_bits=%d")
TRACE_MSG(RRCELL_8599_112_2_18_0_7_39_608,"Extension Information LENGTH =  %d")
TRACE_MSG(RRCELL_8602_112_2_18_0_7_39_609,"RR_DecodeGprsCellOption:left length = %d")
TRACE_MSG(RRCELL_8605_112_2_18_0_7_39_610,"EGPRS Information is included  in GPRS_CELL_OPTION IE")
TRACE_MSG(RRCELL_8626_112_2_18_0_7_39_611,"EGPRS Information is not enough.")
TRACE_MSG(RRCELL_8653_112_2_18_0_7_39_612,"CCN_ACTIVE is %d  in GPRS_CELL_OPTION IE")
TRACE_MSG(RRCELL_10560_112_2_18_0_7_43_613,"RR_DecodePmoMsgBlock: start...")
TRACE_MSG(RRCELL_10593_112_2_18_0_7_43_614,"RR_DecodePmoMsgBlock: PCCO NC_MODE = %d")
TRACE_MSG(RRCELL_10607_112_2_18_0_7_43_615,"RR_DecodePmoMsgBlock :EXT Measurement Parameters  included")
TRACE_MSG(RRCELL_10784_112_2_18_0_7_44_616,"RRC:TLLI in PacketMeasureOrder is wrong!new_tiil=%d, old_tlli=%d, rec_tlli=%d")
TRACE_MSG(RRCELL_10820_112_2_18_0_7_44_617,"RR_DecodePacketMeasureOrder:count:%d ")
TRACE_MSG(RRCELL_11446_112_2_18_0_7_45_618,"RR_DecodePacketCellChangeOrder start.")
TRACE_MSG(RRCELL_11470_112_2_18_0_7_45_619,"RRC:TLLI in PacketCellChangeOrder is wrong!new_tiil=%d, old_tlli=%d, rec_tlli=%d")
TRACE_MSG(RRCELL_11497_112_2_18_0_7_45_620,"RR_DecodePacketCellChangeOrder: PCCO NC_MODE = %d")
TRACE_MSG(RRCELL_11521_112_2_18_0_7_45_621,"RR_DecodePacketCellChangeOrder: 3g target cell included.")
TRACE_MSG(RRCELL_11531_112_2_18_0_7_45_622,"RR_DecodePacketCellChangeOrder: msg_length:%d,offset_octets:%d,offset_bits:%d")
TRACE_MSG(RRCELL_11591_112_2_18_0_7_46_623,"RR_DecodePacketCellChangeOrder failure. msg_error=%d")
TRACE_MSG(RRCELL_12257_112_2_18_0_7_47_624,"RRC: scell is barred cba2 = 1\n")
TRACE_MSG(RRCELL_12266_112_2_18_0_7_47_625,"RRC: scell c1 <= 0\n")
TRACE_MSG(RRCELL_12285_112_2_18_0_7_47_626,"RRC:In  RRCELL_ProcessReselTimerGprs\n")
TRACE_MSG(RRCELL_12286_112_2_18_0_7_47_627,"RRC:ARFCN = %d, rssi = %d, c1 = %d, c31 = %d, c32 = %d\n")
TRACE_MSG(RRCELL_12291_112_2_18_0_7_47_628,"RRC:ARFCN = %d, rssi = %d, c1 = %d, c31 = %d, c32 = %d\n")
TRACE_MSG(RRCELL_12574_112_2_18_0_7_48_629,"RRCELL_GetNCMeasRepList: NC MEAS list is full.")
TRACE_MSG(RRCELL_12617_112_2_18_0_7_48_630,"RRCELL_Get3GMeasRepList: PCCO switch is disabled!!")
TRACE_MSG(RRCELL_12632_112_2_18_0_7_48_631,"RRCELL_Get3GMeasRepList:cell_num=%d,TDD multirat_rep_present=%d,multirat_rep=%d.")
TRACE_MSG(RRCELL_12668_112_2_18_0_7_48_632,"RRCELL_Get3GMeasRepList: 3g target cell has been barred.")
TRACE_MSG(RRCELL_12684_112_2_18_0_7_48_633,"RRCELL_Get3GMeasRepList: cell_3g_list[%d]={index=%d,quantity=%d}.")
TRACE_MSG(RRCELL_12690_112_2_18_0_7_48_634,"RRCELL_Get3GMeasRepList: n_3g=%d.")
TRACE_MSG(RRCELL_12760_112_2_18_0_7_48_635,"RRCELL_ConfigPMRMessage: failure, nc order is not NC1/NC2")
TRACE_MSG(RRCELL_12780_112_2_18_0_7_48_636,"RRCELL_ConfigPMRMessage:sgsnr=%d,revision_level=%d.")
TRACE_MSG(RRCELL_12855_112_2_18_0_7_48_637,"RRCELL_CalcNrOfNcMeasInPMR: offset_octets=%d,offset_bits=%d")
TRACE_MSG(RRCELL_12891_112_2_18_0_7_48_638,"RRCELL_CalcNrOfNcMeasInPMR: new offset_octets=%d,offset_bits=%d")
TRACE_MSG(RRCELL_13014_112_2_18_0_7_49_639,"RRCELL_EncodeNcMeasReport: INTERFERENCE_SERVING_CELL is included.")
TRACE_MSG(RRCELL_13025_112_2_18_0_7_49_640,"RRCELL_EncodeNcMeasReport: nr_nc_meas = %d.")
TRACE_MSG(RRCELL_13047_112_2_18_0_7_49_641,"RRCELL_EncodeNcMeasReport: EXT Measurement Report not support yet.")
TRACE_MSG(RRCELL_13100_112_2_18_0_7_49_642,"RRCELL_EncodeNcMeasReport: meas_3g_rep_present = %d.")
TRACE_MSG(RRCELL_13137_112_2_18_0_7_49_643,"RRCELL_EncodeNcMeasReport: encode failure.")
TRACE_MSG(RRCELL_13586_112_2_18_0_7_50_644,"RRC: cell reselection is controled by network\n")
TRACE_MSG(RRCELL_13793_112_2_18_0_7_50_645,"RR_DecodePacketMeasureOrder failure. msg_error=%d")
TRACE_MSG(RRCELL_14005_112_2_18_0_7_51_646,"RRC: Assert Failure, unknown extend report type.")
TRACE_MSG(RRCELL_14085_112_2_18_0_7_51_647,"isNcMeasReport=%d,dm_td_cells_info.cell_num=%d")
TRACE_MSG(RRCELL_15273_112_2_18_0_7_53_648,"RRC: %d scell ba_ext %d, 2bis %d, tc5bad %d")
TRACE_MSG(RRCELL_15274_112_2_18_0_7_53_649,"RRC: 2ter_ind %d, 2ter_p %d, tc4bad %d")
TRACE_MSG(RRCELL_15377_112_2_18_0_7_53_650,"RRC: cell specified is not present!!")
TRACE_MSG(RRCELL_15382_112_2_18_0_7_53_651,"RRC: param plmn_la_list is NULL!!")
TRACE_MSG(RRCELL_15387_112_2_18_0_7_53_652,"RRC: si3 and si4 of cell specified are not received!!")
TRACE_MSG(RRCELL_15392_112_2_18_0_7_53_653,"RRC: cell specified is barred!!")
TRACE_MSG(RRCELL_15599_112_2_18_0_7_54_654,"RRC: one of pointer params is NULL.")
TRACE_MSG(RRCELL_15929_112_2_18_0_7_54_655,"RRC: NCELL cell to store, arfcn = %d, bsic = %d")
TRACE_MSG(RRCELL_15945_112_2_18_0_7_54_656,"RRC: NCELL cell clear, arfcn = %d, bsic = %d")
TRACE_MSG(RRCELL_16199_112_2_18_0_7_55_657,"RRC: %d FillBsic start %d, num %d, max %d")
TRACE_MSG(RRCELL_16215_112_2_18_0_7_55_658,"RRC: %d bsic %d, a 0x%x, b %d")
TRACE_MSG(RRCELL_16224_112_2_18_0_7_55_659,"RRC: %d ASSERT ERROR, no space when search")
TRACE_MSG(RRCELL_16290_112_2_18_0_7_55_660,"RRC: %d research num %d")
TRACE_MSG(RRCELL_16326_112_2_18_0_7_55_661,"RRC: %d g_ind %d, 13 %d, ba-ext %d, 2bis %d, 2t_ind %d, 2ter %d")
TRACE_MSG(RRCELL_16378_112_2_18_0_7_55_662,"RRC: %d EssSiBad %d %d %d %d")
TRACE_MSG(RRCELL_16556_112_2_18_0_7_56_663,"RRCELL_CleanUseless_Tc: not use tc5:%d, %d, %d")
TRACE_MSG(RRCELL_16590_112_2_18_0_7_56_664,"RRCELL_CleanUseless_Tc: not use tc4:%d, %d, %d")
TRACE_MSG(RRCELL_16604_112_2_18_0_7_56_665,"RRCELL_CleanUseless_Tc: si2_present=%d,si3_present=%d,si4_present=%d,si_2ter_ind=%d,gprs_ind_present=%d,si2quater_ind=%d,si2ter_not_use_tc4=%d,si13_not_use_tc4=%d")
TRACE_MSG(RRCELL_16678_112_2_18_0_7_56_666,"RRC: assert error, exceptional exp")
TRACE_MSG(RRCELL_16689_112_2_18_0_7_56_667,"RRC: assert error, exceptional exp")
TRACE_MSG(RRCELL_16751_112_2_18_0_7_56_668,"RRC: %d FillBcch p %d, a 0x%x, tc 0x%x")
TRACE_MSG(RRCELL_16797_112_2_18_0_7_56_669,"RRCELL_FillBcchReqInfo_ForCamp: si3_present=%d,si_2ter_ind=%d,si4_acs=%d,si2quater_ind=%d")
TRACE_MSG(RRCELL_16822_112_2_18_0_7_56_670,"RRC: %d FillBcch old_tc 0x%x, new 0x%x")
TRACE_MSG(RRCELL_16914_112_2_18_0_7_56_671,"RRCELL_FillBcchReqInfo_Si2quater:si2quater_position=%d,ba_2_ext_ind=%d,si_2ter_ind=%d")
TRACE_MSG(RRCELL_16931_112_2_18_0_7_56_672,"RRCELL_FillBcchReqInfo_Si2quater: old tc=0x%x,2quater tc=0x%x")
TRACE_MSG(RRCELL_17030_112_2_18_0_7_57_673,"RRC: %d UpdateBadSi tc 0x%x, pos %d")
TRACE_MSG(RRCELL_17049_112_2_18_0_7_57_674,"RRC: %d bad si tomax, cell a 0x%x, fn 0x%x")
TRACE_MSG(RRCELL_17124_112_2_18_0_7_57_675,"RRC: %d BadSiTO MAX tc 0x%x, pos %d")
TRACE_MSG(RRCELL_17240_112_2_18_0_7_57_676,"RRC: assert error, illogical si, tc %d, pos %d, fn %d")
TRACE_MSG(RRCELL_17276_112_2_18_0_7_57_677,"RRC: %d gprs_ind %d, si13 %d, ba-ext %d, si2bis %d, 2ter_ind %d, 2ter %d")
TRACE_MSG(RRCELL_17323_112_2_18_0_7_57_678,"RRC:%d ALL SI OK")
TRACE_MSG(RRCELL_17328_112_2_18_0_7_57_679,"RRC:%d cell exp, set allneedsi to true")
TRACE_MSG(RRCELL_17349_112_2_18_0_7_57_680,"RRC: %d IsEssSiOk T, a 0x%x")
TRACE_MSG(RRCELL_17354_112_2_18_0_7_57_681,"RRC: %d, a 0x%x, p%d,%d %d %d %d")
TRACE_MSG(RRCELL_17376_112_2_18_0_7_57_682,"RRC: %d MaxRssi, %d not max")
TRACE_MSG(RRCELL_17380_112_2_18_0_7_57_683,"RRC: %d MaxRssi %d is max")
TRACE_MSG(RRCELL_17407_112_2_18_0_7_57_684,"RRC: %d %d not bestcell")
TRACE_MSG(RRCELL_17412_112_2_18_0_7_57_685,"RRC: %d %d is bestcell")
TRACE_MSG(RRCELL_17441_112_2_18_0_7_57_686,"RRC: %d %d not bestncell")
TRACE_MSG(RRCELL_17446_112_2_18_0_7_57_687,"RRC: %d %d is bestncell")
TRACE_MSG(RRCELL_17469_112_2_18_0_7_57_688,"RRC: %d PlmnSearch, pos %d is suit to camp")
TRACE_MSG(RRCELL_17479_112_2_18_0_7_57_689,"RRC: cell[%d] p %d")
TRACE_MSG(RRCELL_17494_112_2_18_0_7_57_690,"RRC: %d PlmnEnd? s %d, num %d, end %d")
TRACE_MSG(RRCELL_17513_112_2_18_0_7_57_691,"RRC: Plmn search end s %d,num %d,before camp.")
TRACE_MSG(RRCELL_17525_112_2_18_0_7_57_692,"RRC: Plmn search end, i %d, bsic_exist %d,si3_p %d,si4_p %d")
TRACE_MSG(RRCELL_17592_112_2_18_0_7_58_693,"RRC: %d plmn research a=%d,b=%d")
TRACE_MSG(RRCELL_17596_112_2_18_0_7_58_694,"RRC: %d ASSERT ERROR, no space when plmn research")
TRACE_MSG(RRCELL_17654_112_2_18_0_7_58_695,"RRC: %d SelBestCell")
TRACE_MSG(RRCELL_17658_112_2_18_0_7_58_696,"RRC: cell[%d] p %d, rssi %d, pri %d")
TRACE_MSG(RRCELL_17697_112_2_18_0_7_58_697,"RRC: SelBestHCell sel %d")
TRACE_MSG(RRCELL_17704_112_2_18_0_7_58_698,"RRC: SelBestRCell sel %d")
TRACE_MSG(RRCELL_17711_112_2_18_0_7_58_699,"RRC: SelBestCell sel %d")
TRACE_MSG(RRCELL_17762_112_2_18_0_7_58_700,"RRC: SelBestHCell sel %d")
TRACE_MSG(RRCELL_17769_112_2_18_0_7_58_701,"RRC: SelBestRCell sel %d")
TRACE_MSG(RRCELL_17776_112_2_18_0_7_58_702,"RRC: SelBestCell sel %d")
TRACE_MSG(RRCELL_17799_112_2_18_0_7_58_703,"RRC: SelBestCell sel %d")
TRACE_MSG(RRCELL_17840_112_2_18_0_7_58_704,"RRC: keep cell %d, a 0x %x")
TRACE_MSG(RRCELL_17880_112_2_18_0_7_58_705,"%s")
TRACE_MSG(RRCELL_17885_112_2_18_0_7_58_706,"%s")
TRACE_MSG(RRCELL_17979_112_2_18_0_7_58_707,"RRC: %d CleanExp")
TRACE_MSG(RRCELL_17984_112_2_18_0_7_58_708,"C[%d] p %d, a 0x%x, tc 0x%x, du %d, si %d, %d, %d, %d")
TRACE_MSG(RRCELL_17990_112_2_18_0_7_58_709,"RRC: assert, bsic not ret dur 10s")
TRACE_MSG(RRCELL_18119_112_2_18_0_7_59_710,"RRC: %d SET a 0x%x to bad %d")
TRACE_MSG(RRCELL_18137_112_2_18_0_7_59_711,"RRC: %d, a 0x%x is bad")
TRACE_MSG(RRCELL_18163_112_2_18_0_7_59_712,"RRC: %d get free rcell, %d,%d,%d,%d,%d,%d")
TRACE_MSG(RRCELL_18241_112_2_18_0_7_59_713,"RRC: %d ASSERT ERROR, no space when search")
TRACE_MSG(RRCELL_18441_112_2_18_0_7_59_714,"RRC: %d tc_mask_2 0x%x, rcell tc 0x%x")
TRACE_MSG(RRCELL_18628_112_2_18_0_8_0_715,"RRC: %d plmnscan end s %d, num %d")
TRACE_MSG(RRCELL_18650_112_2_18_0_8_0_716,"RRC: %d plmnend %d")
TRACE_MSG(RRCELL_18881_112_2_18_0_8_0_717,"RRC: assert error bsic>search num")
TRACE_MSG(RRCELL_18927_112_2_18_0_8_0_718,"RRC: %d, a 0x%x, p %d")
TRACE_MSG(RRCELL_19006_112_2_18_0_8_0_719,"RRC: assert error bcch>search num")
TRACE_MSG(RRCELL_19023_112_2_18_0_8_0_720,"RRC: search_num=%d,decode_num=%d")
TRACE_MSG(RRCELL_19082_112_2_18_0_8_1_721,"RRC: assert error bcch>search num")
TRACE_MSG(RRCELL_19117_112_2_18_0_8_1_722,"RRC: %d FillBcch fast p %d, a 0x%x, tc 0x%x")
TRACE_MSG(RRCELL_19147_112_2_18_0_8_1_723,"RRC: %d FillBcch old_tc 0x%x, new 0x%x")
TRACE_MSG(RRCELL_19344_112_2_18_0_8_1_724,"RRC: %d fast sel %d a 0x%x")
TRACE_MSG(RRCELL_19347_112_2_18_0_8_1_725,"RRC: %d fast nocell")
TRACE_MSG(RRCELL_19376_112_2_18_0_8_1_726,"RRC: %d C1 All Low %d")
TRACE_MSG(RRCELL_19808_112_2_18_0_8_2_727,"NACC: SCELL C1<0")
TRACE_MSG(RRCELL_19814_112_2_18_0_8_2_728,"NACC: VALID PMO %d")
TRACE_MSG(RRCELL_19817_112_2_18_0_8_2_729,"NACC: IN NC2")
TRACE_MSG(RRCELL_19823_112_2_18_0_8_2_730,"NACC: PMO ccn_active %d")
TRACE_MSG(RRCELL_19834_112_2_18_0_8_2_731,"NACC: PMO TARGET CCN DEFAULT SUPPORT")
TRACE_MSG(RRCELL_19837_112_2_18_0_8_2_732,"NACC: PMO target ccn support %d")
TRACE_MSG(RRCELL_19849_112_2_18_0_8_2_733,"NACC: PMO TARGET CCN DEFAULT SUPPORT")
TRACE_MSG(RRCELL_19865_112_2_18_0_8_2_734,"NACC: CCN ACTIVE: %d")
TRACE_MSG(RRCELL_19878_112_2_18_0_8_2_735,"NACC: TARGET CCN DEFAULT SUPPORT")
TRACE_MSG(RRCELL_19881_112_2_18_0_8_2_736,"NACC: TARGET CCN SUPPORT: %d")
TRACE_MSG(RRCELL_19893_112_2_18_0_8_2_737,"NACC: TARGET CCN DEFAULT SUPPORT")
TRACE_MSG(RRCELL_19958_112_2_18_0_8_2_738,"NACC: PNCD container_id %d, idx %d")
TRACE_MSG(RRCELL_19965_112_2_18_0_8_2_739,"NACC: PNCD arfcn 0x%x, bsic 0x%x")
TRACE_MSG(RRCELL_20019_112_2_18_0_8_3_740,"NACC: buff_idx %d, saved si: 0x%x, 0x%x")
TRACE_MSG(RRCELL_20069_112_2_18_0_8_3_741,"NACC: PCCC arfcn 0x%x bsic 0x%x container_id %d")
TRACE_MSG(RRCELL_20134_112_2_18_0_8_3_742,"NACC: PCCO container_id %d")
TRACE_MSG(RRCELL_20145_112_2_18_0_8_3_743,"NACC: PCCO arfcn 0x%x, bsic 0x%x")
TRACE_MSG(RRCELL_20198_112_2_18_0_8_3_744,"NACC: saved si num %d")
TRACE_MSG(RRCELL_20209_112_2_18_0_8_3_745,"NACC: cd_len %d")
TRACE_MSG(RRCELL_20213_112_2_18_0_8_3_746,"NACC: last_pncd_data.container_contiue %d")
TRACE_MSG(RRCELL_20218_112_2_18_0_8_3_747,"NACC: reassemble si error, discard!")
TRACE_MSG(RRCELL_20225_112_2_18_0_8_3_748,"NACC: REASSEMBEL MSG %x, %x; %x, %x")
TRACE_MSG(RRCELL_20235_112_2_18_0_8_3_749,"NACC: DECODE PNCD si %d")
TRACE_MSG(RRCELL_20242_112_2_18_0_8_3_750,"NACC: DECODE PNCD psi %d")
TRACE_MSG(RRCELL_20272_112_2_18_0_8_3_751,"NACC: DECODE PNCD si %d")
TRACE_MSG(RRCELL_20279_112_2_18_0_8_3_752,"NACC: DECODE PNCD psi %d")
TRACE_MSG(RRCELL_20295_112_2_18_0_8_3_753,"NACC: buffer_len %d, offset_byte %d")
TRACE_MSG(RRCELL_20318_112_2_18_0_8_3_754,"NACC: TARGET CELL PSI1 %d PSI2 %d")
TRACE_MSG(RRCELL_20323_112_2_18_0_8_3_755,"NACC: TARGET CELL SI1_EXIST %d SI1 %d SI3 %d SI13 %d")
TRACE_MSG(RRCELL_20343_112_2_18_0_8_3_756,"NACC: pbcch %d, psi status support %d si status support %d")
TRACE_MSG(RRCELL_20719_112_2_18_0_8_4_757,"NACC: reassemble si error, discard!")
TRACE_MSG(RRCELL_20747_112_2_18_0_8_4_758,"NACC: DECODE PSCD si error %d, type %d")
TRACE_MSG(RRCELL_20752_112_2_18_0_8_4_759,"NACC: DECODE PSCD psi error %d, type %d")
TRACE_MSG(RRCELL_20780_112_2_18_0_8_4_760,"NACC: DECODE PSCD si %d")
TRACE_MSG(RRCELL_20785_112_2_18_0_8_4_761,"NACC: DECODE PSCD psi %d")
TRACE_MSG(RRCELL_21662_112_2_18_0_8_6_762,"RRCELL_IsNC2Valid:nc = %d in PMO is used,gmm_state=%d.")
TRACE_MSG(RRCELL_21682_112_2_18_0_8_6_763,"RRCELL_IsNC2Valid:nc = %d in SI is used,gmm_state=%d.")
TRACE_MSG(RRCELL_21700_112_2_18_0_8_6_764,"RRCELL_IsNC2Valid:cell_present=%d,pbcch_present=%d")
TRACE_MSG(RRCELL_21848_112_2_18_0_8_6_765,"RRCELL_FillBsicSearchList_FastPlmn:multisys=%d,scan_num:%d")
TRACE_MSG(RRCELL_21889_112_2_18_0_8_6_766,"RRCELL_SetGprsSupportStatus: rr_service_state:%d scell lai:(%d %d %d %d),old lai:(%d %d %d %d)")
TRACE_MSG(RRCELL_21906_112_2_18_0_8_6_767,"RRCELL_SetGprsSupportStatus: mm_gprs_supported:%d")
TRACE_MSG(RRCELL_21926_112_2_18_0_8_6_768,"RRSIG: Warning, Invalid input number=%d of bsic!!")
TRACE_MSG(RRCELL_21965_112_2_18_0_8_6_769,"RRSIG: Warning, Invalid input number=%d of arfcn!!")
TRACE_MSG(RRCELL_22041_112_2_18_0_8_7_770,"RRCELL: DecodeACellBcch_ForCamp, the cell is not present!")
TRACE_MSG(RRCELL_22066_112_2_18_0_8_7_771,"RRCELL: DecodeACellBsic, the cell is not present!")
TRACE_MSG(RRCELL_22094_112_2_18_0_8_7_772,"RRCELL: ClearACellTc, not happened, %d,%d")
TRACE_MSG(RRDM_171_112_2_18_0_8_7_773,"RRDM_IsUtranNcellFreq:no arfcn exist in TD CELL LIST")
TRACE_MSG(RRDM_185_112_2_18_0_8_7_774,"RRDM_IsUtranNcellFreq:tdd cell arfcn=%d NOT exist in TD CELL LIST")
TRACE_MSG(RRDM_214_112_2_18_0_8_7_775,"RRDM: RRDM_GetBestTdCell, immediate_resel, cell_num= %d")
TRACE_MSG(RRDM_390_112_2_18_0_8_8_776,"RRDM: reset the dm_td_cells_barred_info")
TRACE_MSG(RRDM_408_112_2_18_0_8_8_777,"RRDM: add the new barred cell: ARFCN = %d,cell_param_id = %d, cell_barred_time = %d, rx_cell_barred_time = %d, td_bar_num = %d")
TRACE_MSG(RRDM_518_112_2_18_0_8_8_778,"RRCELL:RRCELL_AddReselCrt, resel crt array is full.")
TRACE_MSG(RRDM_529_112_2_18_0_8_8_779,"RRCELL:RRCELL_AddReselCrt, invalid 3g ncell index.")
TRACE_MSG(RRDM_535_112_2_18_0_8_8_780,"RRCELL:RRCELL_AddReselCrt, INVALID UARFCN = %d")
TRACE_MSG(RRDM_577_112_2_18_0_8_8_781,"RRCELL_RemoveReselCrt, it has been removed.")
TRACE_MSG(RRDM_598_112_2_18_0_8_8_782,"RRCELL_RemoveReselCrt, invalid array index=%d >= cell_num=%d")
TRACE_MSG(RRDM_622_112_2_18_0_8_8_783,"RRCELL: RRCELL_ModifyReselCrt,warning,this item is not present.")
TRACE_MSG(RRDM_679_112_2_18_0_8_8_784,"RRDM_FindReselCrtByIndex, error, the item is not present.")
TRACE_MSG(RRDM_716_112_2_18_0_8_9_785,"RRDM_FindReselCrtByArfcnAndCpi, error, the item is not present.")
TRACE_MSG(RRDM_739_112_2_18_0_8_9_786,"RRCELL_IsNeedToReplaceReselCrt, invalid index value=%d.")
TRACE_MSG(RRDM_753_112_2_18_0_8_9_787,"RRCELL_IsNeedToReplaceReselCrt, find, but index,arfn,cell_param_id are the same.")
TRACE_MSG(RRDM_777_112_2_18_0_8_9_788,"RRCELL_IsNeedToReplaceReselCrt, find, but index is not the highest.")
TRACE_MSG(RRDM_806_112_2_18_0_8_9_789,"RRDM_AddNcell3g, 3g ncells array is full.")
TRACE_MSG(RRDM_817_112_2_18_0_8_9_790,"RRDM_AddNcell3g, invalid 3g ncell index=%d.")
TRACE_MSG(RRDM_823_112_2_18_0_8_9_791,"RRDM_AddNcell3g, INVALID UARFCN = %d")
TRACE_MSG(RRDM_904_112_2_18_0_8_9_792,"RRDM_RemoveNcell3g, invalid array index=%d >= ncells_3g_num=%d")
TRACE_MSG(RRDM_1013_112_2_18_0_8_9_793,"RRDM_IsNeedToReplaceNcell3gArr, invalid index value=%d.")
TRACE_MSG(RRDM_1027_112_2_18_0_8_9_794,"RRDM_IsNeedToReplaceNcell3gArr, find, but index,arfn,cell_param_id are the same.")
TRACE_MSG(RRDM_1051_112_2_18_0_8_9_795,"RRDM_IsNeedToReplaceNcell3gArr, find, but index is not the highest.")
TRACE_MSG(RRDM_1115_112_2_18_0_8_9_796,"RRDM_FillEplmn: p_as_common_info or g_rrc_eplmn_list is NULL.")
TRACE_MSG(RRDM_1123_112_2_18_0_8_9_797,"RRDM_FillEplmn: warning, bigger plmns number:%d")
TRACE_MSG(RRDM_1159_112_2_18_0_8_9_798,"RRCELL_UpdateReselCrtArrByMeasRep:sync_type=%d, nt td_meas_type=%d, nt meas_rep->cell_num=%d,nt meas_rep->arfcn_num=%d nt resel_crts->cell_num=%d")
TRACE_MSG(RRDM_1258_112_2_18_0_8_10_799,"RRCELL_UpdateMeasRepToReselCrtArr:TD_INFO_L1_TEST_MODE in td_meas_report message, do nothing!!!")
TRACE_MSG(RRDM_1262_112_2_18_0_8_10_800,"RRCELL_UpdateReselCrtArrByMeasRep:TD_MEAS_TYPE ERROR in td_meas_report message!!!")
TRACE_MSG(RRDM_1267_112_2_18_0_8_10_801,"RRCELL_UpdateReselCrtArrByMeasRep, resel criteria cell num=%d")
TRACE_MSG(RRDM_1285_112_2_18_0_8_10_802,"RRDM_FreeTrrMsg: invalid msg pointer!")
TRACE_MSG(RRDM_1382_112_2_18_0_8_10_803,"RRDM_FillDummyUtranMeasParams: NULL eng params pointer!!")
TRACE_MSG(RRDM_1391_112_2_18_0_8_10_804,"RRDM_FillDummyUtranMeasParams, qsearch_x is set as 7 by NV.")
TRACE_MSG(RRDM_1402_112_2_18_0_8_10_805,"RRDM_FillDummyUtranMeasParams:TDDO is running,tdd_offset is adjusted by tddo_tdd_offset of NV. tddo_tdd_offset=%d")
TRACE_MSG(RRDM_1417_112_2_18_0_8_10_806,"RRDM_FillDummyUtranMeasParams:ASSERT!WRONG g_gsm_stack_cfg_params.q_offset_table_switch = %d")
TRACE_MSG(RRDM_1427_112_2_18_0_8_10_807,"RRDM_FillDummyUtranMeasParams: qsearch_x is set as %d by AT CMD.")
TRACE_MSG(RRDM_1446_112_2_18_0_8_10_808,"RRDM_FillDummyUtranMeasParams: tdd offset=%d is set by AT cmd.")
TRACE_MSG(RRDM_1518_112_2_18_0_8_10_809,"RRDM_FillTddNcellsToReselCrtArr: find_result=%d,index=%d,arfcn=%d,cpi=%d,tdd_cell_info=%d")
TRACE_MSG(RRDM_1532_112_2_18_0_8_10_810,"RRDM_FillTddNcellsToReselCrtArr, INVALID UARFCN = %d")
TRACE_MSG(RRDM_1586_112_2_18_0_8_10_811,"RRDM_RemoveTddNcellsBy3gIndex: removed 3g ncell(meas reported) related to 3g index=%d")
TRACE_MSG(RRDM_1638_112_2_18_0_8_10_812,"RRDM_CheckQsearch: dummy td meas without 2quater condition.")
TRACE_MSG(RRDM_1643_112_2_18_0_8_10_813,"RRDM_CheckQsearch:multi_sys=%d,si2qter_ind=%d,meas_param_desc_3g_present=%d")
TRACE_MSG(RRDM_1662_112_2_18_0_8_10_814,"RRDM_CheckQsearch:multi_sys=%d,scell_rssi=%d,qsearch_x=%d,s_q_search_i_table[qsearch_x]=%d")
TRACE_MSG(RRDM_1688_112_2_18_0_8_10_815,"RRDM_CompUtranMeasCriteria:para1.3g_present=%d,para2.3g_present=%d")
TRACE_MSG(RRDM_1695_112_2_18_0_8_10_816,"RRDM_CompUtranMeasCriteria:para1.t_offset=%d,para2.t_offset=%d")
TRACE_MSG(RRDM_1702_112_2_18_0_8_11_817,"RRDM_CompUtranMeasCriteria:return FALSE,para1->td_meas_type=%d,para2->td_meas_type.")
TRACE_MSG(RRDM_1723_112_2_18_0_8_11_818,"RRDM_CompUtranMeasParam:DGY_COMP_1")
TRACE_MSG(RRDM_1736_112_2_18_0_8_11_819,"RRDM_CompUtranMeasParam:DGY_COMP_2")
TRACE_MSG(RRDM_1745_112_2_18_0_8_11_820,"RRDM_CompUtranMeasParam:DGY_COMP_3")
TRACE_MSG(RRDM_1761_112_2_18_0_8_11_821,"RRDM_CompUtranMeasParam:DGY_COMP_4,equal_num=%d,para1->arfcn_num=%d")
TRACE_MSG(RRDM_1787_112_2_18_0_8_11_822,"RRDM_CompL1UtranMeasConf:return FALSE,TD NCELLs are not consistent.")
TRACE_MSG(RRDM_1842_112_2_18_0_8_11_823,"RRDM_FillDummyTddNcells:no dummy cell set by AT!")
TRACE_MSG(RRDM_1928_112_2_18_0_8_11_824,"RRDM_UpdateUtranMeasNcells: UARFCN = %d already exists,index = %d in TD NCELL LIST.")
TRACE_MSG(RRDM_1944_112_2_18_0_8_11_825,"RRDM_UpdateUtranMeasNcells: td meas arfcn array is full.")
TRACE_MSG(RRDM_1960_112_2_18_0_8_11_826,"RRDM_UpdateUtranMeasNcells: td meas cell param id array is full, relate to arfcn=%d.")
TRACE_MSG(RRDM_1991_112_2_18_0_8_11_827,"RRDM_UpdateUtranMeasNcells: TDD MEAS TYPE = TD_INFO_WITH_ARFCN_AND_CELL")
TRACE_MSG(RRDM_2043_112_2_18_0_8_11_828,"RRDM_UpdateUtranCellReselCrts: multi_sys=%d,UTRAN NCELLs are dummyed!!!")
TRACE_MSG(RRDM_2049_112_2_18_0_8_11_829,"RRDM_UpdateUtranCellReselCrts:multi_sys=%d, 3G NCELLS in 2quater included.")
TRACE_MSG(RRDM_2060_112_2_18_0_8_11_830,"RRDM_UpdateUtranCellReselCrts:multi_sys=%d, 3G NCELLS in pmo included.")
TRACE_MSG(RRDM_2128_112_2_18_0_8_12_831,"RRDM_UpdateUtranCellReselCrts: new_resel_ctr_ptr resel criteria array is full.")
TRACE_MSG(RRDM_2173_112_2_18_0_8_12_832,"RRDM_FillUtranMeasParams: 3G meas param is not included in 2quater.")
TRACE_MSG(RRDM_2226_112_2_18_0_8_12_833,"RRDM_FillUtranDescInfoToNcell3gArry: repeated_tdd_ncell_ptr->tdd_arfcn_valid_ind=%d")
TRACE_MSG(RRDM_2238_112_2_18_0_8_12_834,"RRDM_FillUtranDescInfoToNcell3gArry: real nw procedure.")
TRACE_MSG(RRDM_2250_112_2_18_0_8_12_835,"RRDM_FillUtranDescInfoToNcell3gArry: find_result=%d, index=%d, arfcn=%d, cpi=0xff")
TRACE_MSG(RRDM_2264_112_2_18_0_8_12_836,"RRDM_FillTddNcellsToNcell3gArry, INVALID UARFCN = %d")
TRACE_MSG(RRDM_2296_112_2_18_0_8_12_837,"RRDM_FillUtranDescInfoToNcell3gArry: real nw procedure.")
TRACE_MSG(RRDM_2310_112_2_18_0_8_12_838,"RRDM_FillUtranDescInfoToNcell3gArry: find_result=%d, index=%d, arfcn=%d, cpi=%d")
TRACE_MSG(RRDM_2325_112_2_18_0_8_12_839,"RRDM_FillTddNcellsToNcell3gArry, INVALID UARFCN = %d")
TRACE_MSG(RRDM_2378_112_2_18_0_8_12_840,"RRDM_CopySI2Quater3GNcells: si2quater count=%d,arfcn=%d,bsic=%d,ba_ind_3g=%d.")
TRACE_MSG(RRDM_2381_112_2_18_0_8_12_841,"RRDM_CopySI2Quater3GNcells:si2qter_3g_ncells is_present=%d,arfcn=%d,bsic=%d,ba_ind_3g=%d")
TRACE_MSG(RRDM_2394_112_2_18_0_8_12_842,"RRDM_CopySI2Quater3GNcells:3g ncell info in SI2Quater are not changed.")
TRACE_MSG(RRDM_2423_112_2_18_0_8_12_843,"RRDM_CopySI2Quater3GNcells si2quater_inst[%d].msg_present = false")
TRACE_MSG(RRDM_2439_112_2_18_0_8_12_844,"RRDM: MS is in real network configuration!!!")
TRACE_MSG(RRDM_2449_112_2_18_0_8_12_845,"RRDM_CopySI2Quater3GNcells si2quater_inst[%d].msg_present = false")
TRACE_MSG(RRDM_2475_112_2_18_0_8_12_846,"RRDM_AddBarredTdCellInG2TFailure: pointer cnf_ptr is NULL. ")
TRACE_MSG(RRDM_2479_112_2_18_0_8_12_847,"RRDM_AddBarredTdCellInG2TFailure: failure cause=%d")
TRACE_MSG(RRDM_2553_112_2_18_0_8_13_848,"RRDM_GetTDCellList: utran_tdd_desc_present = 0.")
TRACE_MSG(RRDM_2559_112_2_18_0_8_13_849,"RRDM_GetTDCellList: bandwidth_tdd = 0.")
TRACE_MSG(RRDM_2567_112_2_18_0_8_13_850,"RRDM_GetTDCellList: tdd_cells_num > 10.")
TRACE_MSG(RRDM_2571_112_2_18_0_8_13_851,"RRDM_GetTDCellList:index %d, UARFCN = %d")
TRACE_MSG(RRDM_2575_112_2_18_0_8_13_852,"RRDM_GetTDCellList:index %d, tdd_arfcn_valid_ind = 0.")
TRACE_MSG(RRDM_2581_112_2_18_0_8_13_853,"RRDM_GetTDCellList:INVALID UARFCN = %d")
TRACE_MSG(RRDM_2590_112_2_18_0_8_13_854,"RRDM_GetTDCellList:index %d, no cell_param_id")
TRACE_MSG(RRDM_2598_112_2_18_0_8_13_855,"RRDM_GetTDCellList:index %d, tdd_cells_num exceed 10,not store tdd cell any more")
TRACE_MSG(RRDM_2604_112_2_18_0_8_13_856,"RRDM_GetTDCellList:index %d, cell_param_id[%d] = %d")
TRACE_MSG(RRDM_2633_112_2_18_0_8_13_857,"RRDM_FillFastRetTDCellList: tdd_desc_present = 0.")
TRACE_MSG(RRDM_2686_112_2_18_0_8_13_858,"RRC: Assert error, invalid arfcn %d")
TRACE_MSG(RRDM_2740_112_2_18_0_8_13_859,"RRDM_PrintDmTdNcellsInfo: multi_sys=%d,i=%d,arfcn=%d,cell_param_id=%d,index=%d,rscp=%d,q_offset=%d")
TRACE_MSG(RRDM_2776_112_2_18_0_8_13_860,"RRDM_Init: multi_sys=%d")
TRACE_MSG(RRDM_2781_112_2_18_0_8_13_861,"RRDM_Init: g_dm_rr_context[%d] has been already allocated.")
TRACE_MSG(RRDM_2934_112_2_18_0_8_13_862,"RRDM: use band %d")
TRACE_MSG(RRDM_2953_112_2_18_0_8_13_863,"RRDM_UpdateMaxTimeUpdateInd invalid cell_num = %d")
TRACE_MSG(RRDM_2986_112_2_18_0_8_13_864,"RRDM_UpdateTdCellsBarredInfo td_bar_num = %d")
TRACE_MSG(RRDM_3016_112_2_18_0_8_14_865,"RRDM: RRDM_IsBarredTdCell: ARFCN = %d,cell_param_id = %d is barred.")
TRACE_MSG(RRDM_3071_112_2_18_0_8_14_866,"RRDM: update multi_sys=%d td ncell meas info")
TRACE_MSG(RRDM_3112_112_2_18_0_8_14_867,"RRDM_IsInPinpGuard: multi_sys=%d,inter_rat_pinp_offset = %d")
TRACE_MSG(RRDM_3151_112_2_18_0_8_14_868,"RRDM_InitPinpoffset: inter_rat_init_pinp_offset = %d")
TRACE_MSG(RRDM_3176_112_2_18_0_8_14_869,"RRDM_GainPinpOffset: multi_sys=%d, gain step = %d")
TRACE_MSG(RRDM_3185_112_2_18_0_8_14_870,"RRDM_GainPinpOffset: multi_sys=%d,inter_rat_pinp_offset = %d")
TRACE_MSG(RRDM_3202_112_2_18_0_8_14_871,"RRDM_ResetPinpOffset:multi_sys=%d, old inter_rat_pinp_offset = %d")
TRACE_MSG(RRDM_3237_112_2_18_0_8_14_872,"RRDM_GetPinpParams: multi_sys=%d,gsm_tmr = %d half-mins, td_tmr = %d half-mins")
TRACE_MSG(RRDM_3244_112_2_18_0_8_14_873,"RRDM_GetPinpParams: gsm_tmr is initiated to %d s")
TRACE_MSG(RRDM_3250_112_2_18_0_8_14_874,"RRDM_GetPinpParams: td_tmr is initiated to %d s")
TRACE_MSG(RRDM_3269_112_2_18_0_8_14_875,"RRDM_IsAllISi2QuaterInstRcv:multi_sys=%d,si2quater_count = %d")
TRACE_MSG(RRDM_3274_112_2_18_0_8_14_876,"RRDM_IsAllISi2QuaterInstRcv:multi_sys=%d,no si2quater is received!")
TRACE_MSG(RRDM_3287_112_2_18_0_8_14_877,"RRDM_IsAllISi2QuaterInstRcv:multi_sys=%d,SI2QUATER %d segment not received!")
TRACE_MSG(RRDM_3295_112_2_18_0_8_14_878,"RRDM_IsAllISi2QuaterInstRcv:multi_sys=%d,SI2QUATER all segment received!")
TRACE_MSG(RRDM_3346_112_2_18_0_8_14_879,"RRDM:reset multi_sys=%d dm_td_cells_barred_info")
TRACE_MSG(RRDM_3372_112_2_18_0_8_14_880,"RRDM_IsReptTDCellInfo:reset TD cell info because no TD cell is reported from L1.")
TRACE_MSG(RRDM_3409_112_2_18_0_8_14_881,"RRDM_IsReptTDCellInfo:td_cell_info-> meas < 3, and not report.")
TRACE_MSG(RRDM_3425_112_2_18_0_8_14_882,"RRDM_IsReptTDCellInfo:not report.")
TRACE_MSG(RRDM_3489_112_2_18_0_8_14_883,"RRDM_ResetTDCellInfo:reset TD cell info.")
TRACE_MSG(RRDM_3510_112_2_18_0_8_14_884,"Is eplmn")
TRACE_MSG(RRDM_3530_112_2_18_0_8_15_885,"RRDM_IsDmReselAnyPLMN: g_rr_dm_select_any_plmn_flag = %d return TRUE")
TRACE_MSG(RRDM_3534_112_2_18_0_8_15_886,"RRDM_IsDmReselAnyPLMN: g_rr_dm_select_any_plmn_flag = %d return FALSE")
TRACE_MSG(RRDM_3568_112_2_18_0_8_15_887,"RRDM_UpdateMaxTimeUpdateInd invalid cell_num = %d")
TRACE_MSG(RRDM_3607_112_2_18_0_8_15_888,"RRDM_UpdateMaxTimeUpdateInd cell_num = %d")
TRACE_MSG(RRDM_3621_112_2_18_0_8_15_889,"RRDM_HandleMaxTimeUpdateInd: handle the timer of RR_MAX_TIME_UPDATE_IND")
TRACE_MSG(RRDM_3646_112_2_18_0_8_15_890,"RRDM_FillUtranMeas: multi_sys=%d,SCELL ARFCN = %d")
TRACE_MSG(RRDM_3652_112_2_18_0_8_15_891,"RRDM_FillUtranMeas: SI2Q receiving is not completed, so TD MEAS param is not filled.")
TRACE_MSG(RRDM_3660_112_2_18_0_8_15_892,"RRDM_FillUtranMeas: SI2Q is not present, so TD MEAS param is not filled.")
TRACE_MSG(RRDM_3685_112_2_18_0_8_15_893,"RRDM_FillUtranMeas: multi_sys=%d,SI2QUATER 3g ncell[%d]: arfcn=%d,cpi=%d,index=%d")
TRACE_MSG(RRDM_3692_112_2_18_0_8_15_894,"RRDM_FillUtranMeas: td_meas_type=%d,meas_param_desc_3g_present=%d,qsearch_x=%d,t_offset=%d")
TRACE_MSG(RRDM_3718_112_2_18_0_8_15_895,"RRDM_StartUtranMeas:multi_sys=%d, stack_cfg_ptr is NULL.")
TRACE_MSG(RRDM_3731_112_2_18_0_8_15_896,"RRDM_StartUtranMeas: multi_sys=%d,MS is not Dule-Mode,does not need tO start TD measurement")
TRACE_MSG(RRDM_3758_112_2_18_0_8_15_897,"RRDM_StartUtranMeas: td_meas_list:i = %d,arfcn = %d,cell_param_id_num = %d")
TRACE_MSG(RRDM_3763_112_2_18_0_8_15_898,"RRDM_StartUtranMeas:multi_sys=%d,NO valid 3G neibourgh cells!!!!,return")
TRACE_MSG(RRDM_3774_112_2_18_0_8_15_899,"RRDM_StartUtranMeas:TD should be stopped for UE in dark,return FALSE")
TRACE_MSG(RRDM_3798_112_2_18_0_8_15_900,"RRDM_StartUtranMeas:multi_sys=%d,UPDATE_L1_TD_MEAS")
TRACE_MSG(RRDM_3802_112_2_18_0_8_15_901,"RRDM_StartUtranMeas:multi_sys=%d,KEEP_L1_TD_MEAS")
TRACE_MSG(RRDM_3811_112_2_18_0_8_15_902,"RRDM_StartUtranMeas:multi_sys=%d, START_L1_TD_MEAS")
TRACE_MSG(RRDM_3820_112_2_18_0_8_15_903,"RRDM_StartUtranMeas:ASSERT!WRONG q_offset_table_switch = %d")
TRACE_MSG(RRDM_3824_112_2_18_0_8_15_904,"RRDM_StartUtranMeas:multi_sys=%d,last return STOP_L1_TD_MEAS")
TRACE_MSG(RRDM_3900_112_2_18_0_8_15_905,"RRDM_TdReselTrig: multi_sys=%d,eng_offset=%d,penny_offset=%d")
TRACE_MSG(RRDM_3923_112_2_18_0_8_15_906,"RRDM_TdReselTrig: Q_offset =0,resel to TDD immediate")
TRACE_MSG(RRDM_3934_112_2_18_0_8_15_907,"RRDM_TdReselTrig:multi_sys=%d,rssi = %d,rscp = %d ,offset = %d")
TRACE_MSG(RRDM_3940_112_2_18_0_8_15_908,"temp_level < temp_offset,NOT reselect")
TRACE_MSG(RRDM_3966_112_2_18_0_8_15_909,"RRDM_TdReselTrig: TRIGGER TD RESEL , TD RSCP LESS THAN ENG-OFFSET, NOT RESEL")
TRACE_MSG(RRDM_3993_112_2_18_0_8_15_910,"RRDM_TdReselTrig: cmcc spec multi_sys=%d, cell_num=%d, q_offset=%d,penny_offset=%d,eng_offset=%d,inter_rat_pinp_offset=%d")
TRACE_MSG(RRDM_4006_112_2_18_0_8_15_911,"RRDM_TdReselTrig: multi_sys=%d, q_meas=%d, resel_threshold =%d")
TRACE_MSG(RRDM_4091_112_2_18_0_8_16_912,"RRDM: RRDM_IsReselToUtranAllowed, not dual mode.")
TRACE_MSG(RRDM_4098_112_2_18_0_8_16_913,"DM:call ph_gsm_to_td_is_allowed, and return false.")
TRACE_MSG(RRDM_4101_112_2_18_0_8_16_914,"DM:PDP status is %d, PDP timer status is %d")
TRACE_MSG(RRDM_4117_112_2_18_0_8_16_915,"RRDM_IsReselToTD:return FALSE,not reselect to TD tcell_index=%d")
TRACE_MSG(RRDM_4125_112_2_18_0_8_16_916,"RRDM: RRDM_IsReselToUtranAllowed, cell_num= %d, tcell_index=%d")
TRACE_MSG(RRDM_4193_112_2_18_0_8_16_917,"RRDM_ProcessTDReselTimer:multi_sys=%d")
TRACE_MSG(RRDM_4217_112_2_18_0_8_16_918,"RRDM_ProcessTDReselTimer:multi_sys=%d,rssi=%d,rscp=%d,offset=%d")
TRACE_MSG(RRDM_4222_112_2_18_0_8_16_919,"RRDM_ProcessTDReselTimer:clear candidate_times!")
TRACE_MSG(RRDM_4241_112_2_18_0_8_16_920,"RRDM, PROCESS 2->3 RESEL TIMER , TD RSCP LESS THAN ENG-OFFSET, NOT RESEL")
TRACE_MSG(RRDM_4261_112_2_18_0_8_16_921,"RRDM_ProcessTDReselTimer: cmcc spec multi_sys=%d,q_offset = %d,penny_offset = %d,eng_offset = %d,inter_rat_pinp_offset = %d")
TRACE_MSG(RRDM_4280_112_2_18_0_8_16_922,"RRDM_ProcessTDReselTimer:clear candidate_times!")
TRACE_MSG(RRDM_4294_112_2_18_0_8_16_923,"RRDM_ProcessTDReselTimer:multi_sys=%d,TDD index = %d,rscp = %d ,q_offset = %d,candidate_times = %d")
TRACE_MSG(RRDM_4300_112_2_18_0_8_16_924,"RRDM_ProcessTDReselTimer:ASSERT!WRONG g_gsm_stack_cfg_params.q_offset_table_switch = %d")
TRACE_MSG(RRDM_4304_112_2_18_0_8_16_925,"RRDM_ProcessTDReselTimer:multi_sys=%d,TDD q_offset_table_switch = %d")
TRACE_MSG(RRDM_4305_112_2_18_0_8_16_926,"RRDM_ProcessTDReselTimer:multi_sys=%d,q_offset = %d")
TRACE_MSG(RRDM_4312_112_2_18_0_8_16_927,"DM:call ph_gsm_to_td_is_allowed, and return false.")
TRACE_MSG(RRDM_4315_112_2_18_0_8_16_928,"DM:PDP status is %d, PDP timer status is %d")
TRACE_MSG(RRDM_4330_112_2_18_0_8_16_929,"GSM->TDD reselect start:multi_sys=%d,TDD cell arfcn = %d,cell_id = %d")
TRACE_MSG(RRDM_4387_112_2_18_0_8_16_930,"RRDM_StartReselToTD: multi_sys=%d,target td ARFCN=%d,CELL PARAM ID=%d,RSCP=%d")
TRACE_MSG(RRDM_4393_112_2_18_0_8_16_931,"TD_INFO_WITH_ARFCN_BUT_RSSI:parameter id = %d")
TRACE_MSG(RRDM_4437_112_2_18_0_8_16_932,"RRDM: multi_sys=%d receive TRR_RR_DM_RAT_CHG_TO_UMTS_START_RESP.")
TRACE_MSG(RRDM_4573_112_2_18_0_8_17_933,"RRDM: card sync target cell: %d %d.")
TRACE_MSG(RRDM_4646_112_2_18_0_8_17_934,"RRDM_IsFastRet2UtranAllowed: all FR switches are disabled!!")
TRACE_MSG(RRDM_4653_112_2_18_0_8_17_935,"DM:call ph_gsm_to_td_is_allowed, and return false.")
TRACE_MSG(RRDM_4656_112_2_18_0_8_17_936,"DM:PDP status is %d, PDP timer status is %d")
TRACE_MSG(RRDM_4662_112_2_18_0_8_17_937,"RRDM_IsFastRet2UtranAllowed: MS is not Dule-Mode,NOT return to TD!")
TRACE_MSG(RRDM_4699_112_2_18_0_8_17_938,"RRDM_IsFastRet2UtranAllowed: tdd_cells_num = 0,not start FR2U.")
TRACE_MSG(RRDM_4724_112_2_18_0_8_17_939,"RRDM: RRDM_StartFastRet2Utran, multi_sys=%d.")
TRACE_MSG(RRDM_4788_112_2_18_0_8_17_940,"RRDM_IsPcco2UtranAllowed: PCCO switch is disabled!!")
TRACE_MSG(RRDM_4795_112_2_18_0_8_17_941,"RRDM_IsPcco2UtranAllowed: 3G target cell is not present.")
TRACE_MSG(RRDM_4804_112_2_18_0_8_17_942,"RRDM_IsPcco2UtranAllowed: 3G target cell is dummied.")
TRACE_MSG(RRDM_4856_112_2_18_0_8_17_943,"RRDM: RRDM_StartPCCO2UTRAN, multi_sys=%d.")
TRACE_MSG(RRDM_4918_112_2_18_0_8_17_944,"RRDM: multi_sys=%d, 2->3 PCCO failure.")
TRACE_MSG(RRDM_4934_112_2_18_0_8_17_945,"RRDM: multi_sys=%d, 2->3 PCCO success.")
TRACE_MSG(RRDM_4940_112_2_18_0_8_17_946,"RRDM: card sync target cell: %d %d.")
TRACE_MSG(RRDM_4967_112_2_18_0_8_17_947,"RRDM_HandleGsm2TdPCCOResp:multi_sys=%d,ho_status=%d")
TRACE_MSG(RRDM_4973_112_2_18_0_8_17_948,"RRDM:multi_sys=%d,2->3 PCCO failure.")
TRACE_MSG(RRDM_5040_112_2_18_0_8_18_949,"RRDM_HandleGsm2UtranAbortCnf:multi_sys=%d,ho_type=%d")
TRACE_MSG(RRDM_5072_112_2_18_0_8_18_950,"RRDM_IsUseGPRS3GParam:gmm_state=%d,pmo_valid=%d,n enh_meas_param_present=%d,neighbour_cell_desc_3g_present=%d,gprs_3g_meas_para_desc_present=%d")
TRACE_MSG(RRDM_5208_112_2_18_0_8_18_951,"RRDM_UpdateGPRSTDNcelllist:multi_sys=%d, si2quater arfcn=%d,bsic=%d;n PMO pmo_ind=%d.")
TRACE_MSG(RRDM_5213_112_2_18_0_8_18_952,"RRDM_UpdateGPRSTDNcelllist:pmo_3g_ncells is_present=%d,scell arfcn=%d, scell bsic=%d,pmo_ind=%d.")
TRACE_MSG(RRDM_5219_112_2_18_0_8_18_953,"RRDM_UpdateGPRSTDNcelllist: ncell 3g info in PMO are not changed.")
TRACE_MSG(RRDM_5261_112_2_18_0_8_18_954,"RRDM_UpdateGPRSTDNcelllist: PMO 3G NCELL[%d]: arfcn=%d,cpi=%d,index=%d")
TRACE_MSG(RRDM_5305_112_2_18_0_8_18_955,"RRDM_FillGPRSTDMeasParam from the params of PMO.")
TRACE_MSG(RRDM_5315_112_2_18_0_8_18_956,"RRDM_FillGPRSTDMeasParam from the params of 2Quater.")
TRACE_MSG(RRDM_5338_112_2_18_0_8_18_957,"RRDM_GetT3174Duration: multi_sys=%d,dummy millisec_val=%d")
TRACE_MSG(RRDM_5493_112_2_18_0_8_18_958,"RRDM_HandleRatChg2GsmStartReqMsg: multi_sys=%d,target gsm={arfcn=%d,band=%d,bsic=%d}; n   tdd source cell={arfcn=%d,cpi=%d};n   revert_flag=%d,is_card_sync=%d,sync_cause=%d,cmd_type=%d")
TRACE_MSG(RRDM_5561_112_2_18_0_8_19_959,"RRDM_GetPLMNUsedBand: band=%d")
TRACE_MSG(RRDM_5687_112_2_18_0_8_19_960,"RRDM: multi_sys=%d, Set rr_utran_2_gsm_sub_state=%d")
TRACE_MSG(RRDM_5699_112_2_18_0_8_19_961,"RRDM: multi_sys=%d, get rr_utran_2_gsm_sub_state=%d")
TRACE_MSG(RRDM_5748_112_2_18_0_8_19_962,"RRDM_SendUtranRej2GsmResult, multi_sys=%d, result=%d")
TRACE_MSG(RRDM_5771_112_2_18_0_8_19_963,"RRDM_SendUtranRej2GsmResult: camp_cell.arfcn=%d,bsic=%d,freq_band=%d")
TRACE_MSG(RRDM_5842_112_2_18_0_8_19_964,"RRDM_SendInterRatHOResult: multi_sys=%d, HO result arfcn %d, band %d")
TRACE_MSG(RRDM_5858_112_2_18_0_8_19_965,"RRDM_SendInterRatHOResult: multi_sys=%d,camp_cell.arfcn=%d,bsic=%d,freq_band=%d")
TRACE_MSG(RRDM_5976_112_2_18_0_8_19_966,"DM: RRDM_SendUtran2GsmStartResp ,multi_sys=%d, result = %d")
TRACE_MSG(RRDM_6048_112_2_18_0_8_20_967,"RRDM_GetReselGsmArfcnAndBand: arfcn=%d band %d")
TRACE_MSG(RRDM_6092_112_2_18_0_8_20_968,"RRDM_IsNACCInfoPresent: NACC info is not present.")
TRACE_MSG(RRDM_6098_112_2_18_0_8_20_969,"RRDM_IsNACCInfoPresent: PBCCH is present.")
TRACE_MSG(RRDM_6103_112_2_18_0_8_20_970,"RRDM_IsNACCInfoPresent: no NACC info!psi1_present = %d,psi2_present=%d")
TRACE_MSG(RRDM_6114_112_2_18_0_8_20_971,"RRDM_IsNACCInfoPresent: no NACC info!si3_present = %d,si13_present=%d")
TRACE_MSG(RRDM_6121_112_2_18_0_8_20_972,"RRDM_IsNACCInfoPresent: should get NACC CCOFU!")
TRACE_MSG(RRDM_6136_112_2_18_0_8_20_973,"RRDM_GetNACCCellInfo: no NACC info, so NOT copy cell info.")
TRACE_MSG(RRDM_6140_112_2_18_0_8_20_974,"RRDM_GetNACCCellInfo: start copy CCOFU NACC cell info!")
TRACE_MSG(RRDM_6203_112_2_18_0_8_20_975,"RRDM_GetNACCinfo: SI index %d, msg type = %d is error.")
TRACE_MSG(RRDM_6207_112_2_18_0_8_20_976,"RRDM_GetNACCinfo: SI index %d, msg type = %d")
TRACE_MSG(RRDM_6218_112_2_18_0_8_20_977,"RRDM_GetNACCinfo: PSI index %d, msg type = %d is error.")
TRACE_MSG(RRDM_6222_112_2_18_0_8_20_978,"RRDM_GetNACCinfo: PSI index %d, msg type = %d")
TRACE_MSG(RRDM_6226_112_2_18_0_8_20_979,"RRDM_GetNACCinfo: index %d, NOT SI or PSI, msg kind = %d")
TRACE_MSG(RRDM_6244_112_2_18_0_8_20_980,"RRDM_IsStartNACC: NACC info is not present.")
TRACE_MSG(RRDM_6250_112_2_18_0_8_20_981,"RRDM_IsStartNACC: PBCCH is present.")
TRACE_MSG(RRDM_6255_112_2_18_0_8_20_982,"RRDM_IsStartNACC: no NACC info!psi1_present = %d,psi2_present=%d")
TRACE_MSG(RRDM_6264_112_2_18_0_8_20_983,"RRDM_IsStartNACC: no NACC info!si3_present = %d,si13_present=%d")
TRACE_MSG(RRDM_6269_112_2_18_0_8_20_984,"RRDM_IsStartNACC: start NACC CCOFU!")
TRACE_MSG(RRDM_6322_112_2_18_0_8_20_985,"RRDM_ControlInterRat:multi_sys=%d,rr_cur_rat=%d,rat_chg_allowed=%d,inter_rat_chg_allowed=%d")
TRACE_MSG(RRDM_6328_112_2_18_0_8_20_986,"RRDM_ControlInterRat:ASSERT!WRONG q_offset_table_switch = %d")
TRACE_MSG(RRDM_6379_112_2_18_0_8_20_987,"RRDM_SendUMTS2GSMProcResultToNAS , result = %d")
TRACE_MSG(RRDM_6410_112_2_18_0_8_20_988,"RRDM_SendUMTS2GSMProcResultToTRR, result = %d")
TRACE_MSG(RRDM_6491_112_2_18_0_8_20_989,"RRDM_SendReselResultToNAS , result = %d")
TRACE_MSG(RRDM_6519_112_2_18_0_8_20_990,"DM: RRDM_SendReselResultToTRR , result = %d")
TRACE_MSG(RRDM_6551_112_2_18_0_8_21_991,"RRDM_SendReselResultToTRR: camp_cell.arfcn=%d,bsic=%d,freq_band=%d")
TRACE_MSG(RRDM_6606_112_2_18_0_8_21_992,"DM: , RESEL , BSIC FAIL")
TRACE_MSG(RRDM_6638_112_2_18_0_8_21_993,"DM: check arfcn , %d %d %d %d ")
TRACE_MSG(RRDM_6645_112_2_18_0_8_21_994,"DM: RRDM_CheckArfcnBsic , BSIC not matched")
TRACE_MSG(RRDM_6664_112_2_18_0_8_21_995,"DM: get rssi from td cmd , rssi = %d ")
TRACE_MSG(RRDM_6732_112_2_18_0_8_21_996,"RRDM_WaitDspSync: wait dsp sync 10ms")
TRACE_MSG(RRDM_6777_112_2_18_0_8_21_997,"RRDM_GetMphChangeMode: g_rr_dm_rat_chg_type = %d,ho_type=%d,change_type=%d.")
TRACE_MSG(RRDM_6782_112_2_18_0_8_21_998,"RRDM_GetMphChangeMode:Error,ho_type=%d,g_rr_dm_rat_chg_type=%d,not equal.")
TRACE_MSG(RRDM_6985_112_2_18_0_8_21_999,"RRDM_AddBaFromInterRAT: ba_list has been full,return FALSE.")
TRACE_MSG(RRDM_7021_112_2_18_0_8_21_1000,"RRDM_AddBaFromInterRAT: not fill ba list,return FALSE.")
TRACE_MSG(RRMSG_2369_112_2_18_0_8_26_1001,"CASE207:get si2bis ba")
TRACE_MSG(RRMSG_2375_112_2_18_0_8_26_1002,"CASE207:invalid si2bis")
TRACE_MSG(RRMSG_2640_112_2_18_0_8_27_1003,"RR:si 1, pcs ind is TRUE ")
TRACE_MSG(RRMSG_3020_112_2_18_0_8_28_1004,"SI2Quater: bandwidth_tdd=%d")
TRACE_MSG(RRMSG_3023_112_2_18_0_8_28_1005,"RR_DecodeUTRANTDD_Desc:@@@@@384 Mcps band is received,which is not supported in this version!\n")
TRACE_MSG(RRMSG_3034_112_2_18_0_8_28_1006,"RR_DecodeUTRANTDD_Desc:@@@@@384 Mcps band is received,which is not supported in this version!\n")
TRACE_MSG(RRMSG_3042_112_2_18_0_8_28_1007,"SI2Quater: nr_repeated_utran_tdd_neighbour_cells = %d")
TRACE_MSG(RRMSG_3085_112_2_18_0_8_28_1008,"RR_DecodeUTRANTDD_Desc:after tdd_indic0=%d bit is 1,TDD_ARFCN_INDEX ignore")
TRACE_MSG(RRMSG_3099_112_2_18_0_8_28_1009,"SI2Quater: L= %d,arfcn = %d,nr_of_tdd_cells = %d")
TRACE_MSG(RRMSG_3125_112_2_18_0_8_28_1010,"RR_DecodeUTRANTDD_Desc:offset_octets = %d, offset_bits = %d")
TRACE_MSG(RRMSG_3127_112_2_18_0_8_28_1011,"RR_DecodeUTRANTDD_Desc:w[%d] = %d, wn[%d] = %d")
TRACE_MSG(RRMSG_3185_112_2_18_0_8_28_1012,"SI2Quater: L = %d,cell_param_id = %d")
TRACE_MSG(RRMSG_3231_112_2_18_0_8_28_1013,"Bandwidth_FDD included")
TRACE_MSG(RRMSG_3236_112_2_18_0_8_28_1014,"Repeated UMTS FDD Neighbour Cells included")
TRACE_MSG(RRMSG_3296_112_2_18_0_8_28_1015,"SI2Quater: Index_Start_3G=%d")
TRACE_MSG(RRMSG_3307_112_2_18_0_8_28_1016,"SI2Quater: absolute_index_start_emr=%d")
TRACE_MSG(RRMSG_3314_112_2_18_0_8_28_1017,"SI2Quater: total_nr_utran_fdd_ncells=%d")
TRACE_MSG(RRMSG_3350_112_2_18_0_8_28_1018,"SI2Quater: QSearch_i = %d")
TRACE_MSG(RRMSG_3357_112_2_18_0_8_28_1019,"SI2Quater: fdd measurement information is included,need skip all bits in now version")
TRACE_MSG(RRMSG_3372_112_2_18_0_8_28_1020,"SI2Quater: tdd_qoffset=%d, tdd_multirat_reporting = %d")
TRACE_MSG(RRMSG_3390_112_2_18_0_8_28_1021,"SIQuater:skip following bits!\n")
TRACE_MSG(RRMSG_3394_112_2_18_0_8_28_1022,"SIQuater:RTD6 exist!\n")
TRACE_MSG(RRMSG_3423_112_2_18_0_8_29_1023,"SIQuater:RTD12 exist!\n")
TRACE_MSG(RRMSG_3507_112_2_18_0_8_29_1024,"SIQuater:GPRS_MEASUREMENT_Parameters Description is included!\n")
TRACE_MSG(RRMSG_3615_112_2_18_0_8_29_1025,"SI2Quater: nc_order=%d,nc_period_present=%d,nc_non_drx_period=%d,nc_reporting_period_i=%d,nc_reporting_period_t=%d")
TRACE_MSG(RRMSG_3658_112_2_18_0_8_29_1026,"Si2Q EXT: ccn_support %d, num %d")
TRACE_MSG(RRMSG_3754_112_2_18_0_8_29_1027,"RRC: si2quater available indicated by si3")
TRACE_MSG(RRMSG_3762_112_2_18_0_8_29_1028,"RRC: si2quater position = %d")
TRACE_MSG(RRMSG_3768_112_2_18_0_8_29_1029,"RRC: arfcn = %d, ra_colour = %d\n ")
TRACE_MSG(RRMSG_3878_112_2_18_0_8_29_1030,"RRC: pcs ind %d")
TRACE_MSG(RRMSG_3881_112_2_18_0_8_29_1031,"RRC: SI6 rest len wrong")
TRACE_MSG(RRMSG_3915_112_2_18_0_8_30_1032,"RRC: network assign half rate channel!")
TRACE_MSG(RRMSG_4855_112_2_18_0_8_31_1033,"    si2quater_present=%d")
TRACE_MSG(RRMSG_4887_112_2_18_0_8_31_1034,"    temp_ba_ind = %d,\n    temp_3g_ba_ind = %d,\n    temp_mp_change_mark = %d,\n    temp_si2quater_index = %d,\n    temp_si2quater_count = %d")
TRACE_MSG(RRMSG_4892_112_2_18_0_8_31_1035,"instance %d hans been received,return")
TRACE_MSG(RRMSG_4967_112_2_18_0_8_32_1036,"SIQuater:Measurement_Parameters Description is included!\n")
TRACE_MSG(RRMSG_4985_112_2_18_0_8_32_1037,"SIQuater:GPRS RTD description is not supported by this version!\n")
TRACE_MSG(RRMSG_4991_112_2_18_0_8_32_1038,"SIQuater:GPRS_BSIC Descriptionis included!\n")
TRACE_MSG(RRMSG_5001_112_2_18_0_8_32_1039,"SIQuater:GPRS_REPORT PRIORITY Description is included!\n")
TRACE_MSG(RRMSG_5010_112_2_18_0_8_32_1040,"SIQuater:GPRS_MEASUREMENT_Parameters Description is included!\n")
TRACE_MSG(RRMSG_5020_112_2_18_0_8_32_1041,"SIQuater :GPRS_3G_MEASUREMENT Parameters Description  included")
TRACE_MSG(RRMSG_5053_112_2_18_0_8_32_1042,"SI2Q REST: ccn_support %d, num %d")
TRACE_MSG(RRMSG_5060_112_2_18_0_8_32_1043,"SIQuater :extension length included")
TRACE_MSG(RRMSG_5098_112_2_18_0_8_32_1044,"SIQuater :GPRS_3G_MEASUREMENT Parameters Description  included")
TRACE_MSG(RRMSG_5137_112_2_18_0_8_32_1045,"RR_DecodeSi2Quater:RRC_MANDATORY_IE_ERROR")
TRACE_MSG(RRMSG_5161_112_2_18_0_8_32_1046,"RR_DecodeSi2Quater:SI2QUATER instance %d not present,need wait it!")
TRACE_MSG(RRMSG_5169_112_2_18_0_8_32_1047,"RR_DecodeSi2Quater:all sengment is received!")
TRACE_MSG(RRMSG_5189_112_2_18_0_8_32_1048,"RRC:received System Information 9, not supported now!")
TRACE_MSG(RRMSG_5207_112_2_18_0_8_32_1049,"RRC:received System Information 16, not supported now!")
TRACE_MSG(RRMSG_5225_112_2_18_0_8_32_1050,"RRC:received System Information 17, not supported now!")
TRACE_MSG(RRMSG_5243_112_2_18_0_8_33_1051,"RRC:received System Information 18, not supported now!")
TRACE_MSG(RRMSG_5261_112_2_18_0_8_33_1052,"RRC:received System Information 19, not supported now!")
TRACE_MSG(RRMSG_5279_112_2_18_0_8_33_1053,"RRC:received System Information 20, not supported now!")
TRACE_MSG(RRMSG_5308_112_2_18_0_8_33_1054,"RRC: si2bis is not present, ba_0 is %d")
TRACE_MSG(RRMSG_5319_112_2_18_0_8_33_1055,"RRC: ba_0 is %d")
TRACE_MSG(RRMSG_5423_112_2_18_0_8_33_1056,"RRMSG_HandleSysInfo: rr_dm_context_ptr is NULL.")
TRACE_MSG(RRMSG_5438_112_2_18_0_8_33_1057,"RR: Decode dummy si2quarter")
TRACE_MSG(RRMSG_5465_112_2_18_0_8_33_1058,"RRC: use bcch ba when traffic.")
TRACE_MSG(RRMSG_5572_112_2_18_0_8_33_1059,"RRC: RR_DecodeSi2quater")
TRACE_MSG(RRMSG_5603_112_2_18_0_8_33_1060,"RRC: unknown msg type %d")
TRACE_MSG(RRMSG_5697_112_2_18_0_8_33_1061,"RRC: cell bar")
TRACE_MSG(RRMSG_5780_112_2_18_0_8_34_1062,"RRC: cell bar")
TRACE_MSG(RRMSG_6326_112_2_18_0_8_35_1063,"RRC: egprs packet uplink extend_RA %d")
TRACE_MSG(RRMSG_6431_112_2_18_0_8_35_1064,"RRC: gprs packet uplink extend_RA %d")
TRACE_MSG(RRMSG_6469_112_2_18_0_8_35_1065,"RRC:Timing Advance is out of range!")
TRACE_MSG(RRMSG_6565_112_2_18_0_8_35_1066,"RRC: immed_assign->request_ref->random_ab %x")
TRACE_MSG(RRMSG_6851_112_2_18_0_8_36_1067,"RRC: recv imm reject")
TRACE_MSG(RRMSG_8117_112_2_18_0_8_38_1068,"RR:MR config icmi %d start_mode %d acs 0x%x")
TRACE_MSG(RRMSG_8201_112_2_18_0_8_38_1069,"RRC: VGCS???")
TRACE_MSG(RRMSG_8212_112_2_18_0_8_39_1070,"RR:Channel modify msg error")
TRACE_MSG(RRMSG_8437_112_2_18_0_8_39_1071,"RR: MULTI SLOT ERROR!")
TRACE_MSG(RRMSG_8940_112_2_18_0_8_40_1072,"RRC: VGCS???")
TRACE_MSG(RRMSG_8950_112_2_18_0_8_40_1073,"RR:Channel assign msg error")
TRACE_MSG(RRMSG_8970_112_2_18_0_8_40_1074,"RRC: assert error, assign_cmd start cipher but no cipher cmd recv before")
TRACE_MSG(RRMSG_9711_112_2_18_0_8_42_1075,"RRC: VGCS???")
TRACE_MSG(RRMSG_9721_112_2_18_0_8_42_1076,"RR:Channel assign msg error")
TRACE_MSG(RRMSG_9741_112_2_18_0_8_42_1077,"RRC: assert error, handover start cipher but no cipher cmd recv before")
TRACE_MSG(RRMSG_11598_112_2_18_0_8_45_1078,"RRC: ba_5_ind and si5bis not present")
TRACE_MSG(RRMSG_11614_112_2_18_0_8_45_1079,"RRC: ba_2_ind and si2bis not present")
TRACE_MSG(RRMSG_11941_112_2_18_0_8_46_1080,"RRMSG_DecodeCellChangeOrder:optional 3G targe cell IE is error,so ignore it.")
TRACE_MSG(RRMSG_12388_112_2_18_0_8_47_1081,"RRC: SGSNR %d")
TRACE_MSG(RRMSG_12397_112_2_18_0_8_47_1082,"NACC: si_status_ind %d")
TRACE_MSG(RRMSG_12537_112_2_18_0_8_47_1083,"RRMSG_EncodePacketCellChangeFailure: cause=%d")
TRACE_MSG(RRMSG_13022_112_2_18_0_8_48_1084,"RRC:received Extended Mearsurement Order!")
TRACE_MSG(RRMSG_13179_112_2_18_0_8_49_1085,"RRC: receive all mi, emr support is %d")
TRACE_MSG(RRMSG_13897_112_2_18_0_8_50_1086,"RRMSG_Record3GTargetCellContent: tdd_cell_valid:%d,fdd_cell_valid:%d")
TRACE_MSG(RRMSG_13960_112_2_18_0_8_50_1087,"Bandwidth_FDD included")
TRACE_MSG(RRMSG_13972_112_2_18_0_8_50_1088,"FDD Neighbour Cells no = %d")
TRACE_MSG(RRMSG_14034_112_2_18_0_8_50_1089,"Bandwidth_TDD included")
TRACE_MSG(RRMSG_14038_112_2_18_0_8_50_1090,"@@@@@384 Mcps band is received,which is not supported in this version!\n")
TRACE_MSG(RRMSG_14045_112_2_18_0_8_51_1091,"@@@@@384 Mcps band is received,which is not supported in this version!\n")
TRACE_MSG(RRMSG_14074_112_2_18_0_8_51_1092,"L= %d,arfcn = %d")
TRACE_MSG(RRMSG_14081_112_2_18_0_8_51_1093,"L= %d,nr_of_tdd_cells = %d")
TRACE_MSG(RRMSG_14218_112_2_18_0_8_51_1094,"RR_RecordCellSelIndicator: simple FR and innovation FR are disabled!!")
TRACE_MSG(RRMSG_14245_112_2_18_0_8_51_1095,"RR_RecordCellSelIndicator:WRONG description is included in channel release!")
TRACE_MSG(RRPROC_376_112_2_18_0_8_52_1096,"RRC: nc_reporting_period_i = %d\n")
TRACE_MSG(RRPROC_377_112_2_18_0_8_52_1097,"RRC: drx_period = %d\n")
TRACE_MSG(RRPROC_378_112_2_18_0_8_52_1098,"RRC: t3158_value = %d\n")
TRACE_MSG(RRPROC_412_112_2_18_0_8_52_1099,"RRC: transfer nc_reporting_period_t = %d\n")
TRACE_MSG(RRPROC_478_112_2_18_0_8_52_1100,"RRPROC_CheckAccessPermission:rr_service_state = %d,permission = %d")
TRACE_MSG(RRPROC_479_112_2_18_0_8_52_1101,"RRPROC_CheckAccessPermission:rr_est_cause = %d")
TRACE_MSG(RRPROC_574_112_2_18_0_8_52_1102,"RRPROC:Default case!")
TRACE_MSG(RRPROC_1158_112_2_18_0_8_53_1103,"RRPROC_FindStrongestCell:all gsm cells are not suitable for camping in dual mode.")
TRACE_MSG(RRPROC_1213_112_2_18_0_8_53_1104,"RRC:RR NCELL: cell clear all")
TRACE_MSG(RRPROC_1289_112_2_18_0_8_54_1105,"RR: Assert Failure, unknown page mode.")
TRACE_MSG(RRPROC_1931_112_2_18_0_8_55_1106,"RRC: use band %d")
TRACE_MSG(RRPROC_1956_112_2_18_0_8_55_1107,"RRC: Assert error, invalid arfcn %d")
TRACE_MSG(RRPROC_2146_112_2_18_0_8_55_1108,"RRC: traffic ba is invalid, not clear ncell")
TRACE_MSG(RRPROC_2161_112_2_18_0_8_55_1109,"RRC: clear cell %d, arfcn %d")
TRACE_MSG(RRPROC_2293_112_2_18_0_8_56_1110,"RRC_ForceCamping:error! arfcn_count = %d,exceed the allowed max count %d \n")
TRACE_MSG(RRPROC_2299_112_2_18_0_8_56_1111,"RRC_ForceCamping:warning! arfcn_count = %d \n")
TRACE_MSG(RRPROC_2320_112_2_18_0_8_56_1112,"RRC_ForceCamping: index = %d, arfcn = %d \n")
TRACE_MSG(RRPROC_2346_112_2_18_0_8_56_1113,"RRC: RRC_CancelForceCamping \n")
TRACE_MSG(RRPROC_2768_112_2_18_0_8_57_1114,"RRC: RECV si BAD,compare1{%d,%d},compare2{%d,%d},ph_decode_indicator=%d,msg_error=%d")
TRACE_MSG(RRPROC_2815_112_2_18_0_8_57_1115,"RRC: FTA RRMSG_GetPacketMsgType is TRUE")
TRACE_MSG(RRPROC_2818_112_2_18_0_8_57_1116,"RRC: FTA msg type is paging")
TRACE_MSG(RRPROC_2822_112_2_18_0_8_57_1117,"RRC: FTA paging msg decode is ok")
TRACE_MSG(RRPROC_2825_112_2_18_0_8_57_1118,"RRC: FTA paging address is ok, save it")
TRACE_MSG(RRPROC_2915_112_2_18_0_8_57_1119,"%s")
TRACE_MSG(RRPROC_2923_112_2_18_0_8_57_1120,"%s")
TRACE_MSG(RRPROC_2962_112_2_18_0_8_57_1121,"RRC: %d findRcell, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d")
TRACE_MSG(RRPROC_2967_112_2_18_0_8_57_1122,"RRC: tc 0x%x, tc 0x%x, tc 0x%x, tc 0x%x, tc 0x%x, tc 0x%x")
TRACE_MSG(RRPROC_3070_112_2_18_0_8_57_1123,"RRC: FAB CAMP")
TRACE_MSG(RRPROC_3406_112_2_18_0_8_58_1124,"RRC_Get_ForceCamping_Gcell: error! g_forced_arfcn_count = %d")
TRACE_MSG(RRPROC_3428_112_2_18_0_8_58_1125,"RRC: get rach action with %d, %d, %d")
TRACE_MSG(RR_ADAPTER_973_112_2_18_0_9_37_1126,"RRA: enter RRA_Init")
TRACE_MSG(RR_ADAPTER_1108_112_2_18_0_9_38_1127,"RRA: rr1 -- %d ")
TRACE_MSG(RR_ADAPTER_1109_112_2_18_0_9_38_1128,"RRA: rrca_status1 -- %d, sig_code 0x%x")
TRACE_MSG(RR_ADAPTER_1112_112_2_18_0_9_38_1129,"RRA: rr2 -- %d ")
TRACE_MSG(RR_ADAPTER_1113_112_2_18_0_9_38_1130,"RRA: rrca_status2 -- %d sig_code 0x%x")
TRACE_MSG(RR_ADAPTER_1241_112_2_18_0_9_38_1131,"RRA: s_rra_gprs_id 0x%x is invalid, sig_code = 0x%x.")
TRACE_MSG(RR_ADAPTER_1277_112_2_18_0_9_38_1132,"RRA: s_rra_gprs_id 0x%x is invalid, sig_code = 0x%x.")
TRACE_MSG(RR_ADAPTER_1446_112_2_18_0_9_38_1133,"RRA: assert error, unprocessed signal 0x%x")
TRACE_MSG(RR_ADAPTER_1480_112_2_18_0_9_38_1134,"RRA: MPH_ACTIVE_REQ discarded in rat change, rr %d")
TRACE_MSG(RR_ADAPTER_1589_112_2_18_0_9_39_1135,"RRA: rr %d, rr_task_id = 0x%x")
TRACE_MSG(RR_ADAPTER_1664_112_2_18_0_9_39_1136,"RRA: powersweep req %d state0 = %d, state1 = %d, state2 = %d, state3 = %d")
TRACE_MSG(RR_ADAPTER_1670_112_2_18_0_9_39_1137,"RRA: powersweep req %d state0 = %d, state1 = %d, state2 = %d")
TRACE_MSG(RR_ADAPTER_1675_112_2_18_0_9_39_1138,"RRA: powersweep req %d state0 = %d, state1 = %d")
TRACE_MSG(RR_ADAPTER_1679_112_2_18_0_9_39_1139,"RRA: powersweep req %d state0 = %d")
TRACE_MSG(RR_ADAPTER_1695_112_2_18_0_9_39_1140,"RRA: band change from %d to %d, must do power sweep")
TRACE_MSG(RR_ADAPTER_1709_112_2_18_0_9_39_1141,"RRA: discard powersweep req %d, wait layer1 powersweep cnf.")
TRACE_MSG(RR_ADAPTER_1761_112_2_18_0_9_39_1142,"RRA: dummy powersweep cnf state0 = %d, state1 = %d, state2 = %d, state3 = %d, last traffic id = %d")
TRACE_MSG(RR_ADAPTER_1767_112_2_18_0_9_39_1143,"RRA: dummy powersweep cnf state0 = %d, state1 = %d, state2 = %d, last traffic id = %d")
TRACE_MSG(RR_ADAPTER_1772_112_2_18_0_9_39_1144,"RRA: dummy powersweep cnf state0 = %d, state1 = %d, last traffic id = %d")
TRACE_MSG(RR_ADAPTER_1776_112_2_18_0_9_39_1145,"RRA: dummy powersweep cnf state0 = %d, last traffic id = %d")
TRACE_MSG(RR_ADAPTER_1791_112_2_18_0_9_39_1146,"RRA: return dummy cnf.")
TRACE_MSG(RR_ADAPTER_1813_112_2_18_0_9_39_1147,"RRA: rr %d in find plmn flag = %d, state = %d")
TRACE_MSG(RR_ADAPTER_1837_112_2_18_0_9_39_1148,"RRA: rr %d enter RRA_RR_SEARCH.")
TRACE_MSG(RR_ADAPTER_1852_112_2_18_0_9_39_1149,"powersweepreq:master id %d")
TRACE_MSG(RR_ADAPTER_1906_112_2_18_0_9_39_1150,"RRA: po task id = %d, %d, %d, d, find plmn flag = %d, %d, %d, %d , state = %d, %d, %d, %d, master = %d")
TRACE_MSG(RR_ADAPTER_1919_112_2_18_0_9_39_1151,"RRA: po task id = %d, %d, %d, find plmn flag = %d, %d, %d , state = %d, %d, %d, master = %d")
TRACE_MSG(RR_ADAPTER_1929_112_2_18_0_9_39_1152,"RRA: po task id = %d, %d, find plmn flag = %d, %d, state = %d, %d,, master = %d")
TRACE_MSG(RR_ADAPTER_1936_112_2_18_0_9_39_1153,"RRA: po task id = %d, find plmn flag = %d, state = %d,master = %d")
TRACE_MSG(RR_ADAPTER_1959_112_2_18_0_9_39_1154,"RRA: layer1 return dummy powersweep cnf.")
TRACE_MSG(RR_ADAPTER_2281_112_2_18_0_9_40_1155,"RRA: discard bcch decode req when rat change, i = %d")
TRACE_MSG(RR_ADAPTER_2310_112_2_18_0_9_40_1156,"RRA: recv pbcch decode req, req_rr = %d")
TRACE_MSG(RR_ADAPTER_2343_112_2_18_0_9_40_1157,"RRA: no BcchInfoInd because of busy state of RR (%d)!")
TRACE_MSG(RR_ADAPTER_2406_112_2_18_0_9_40_1158,"RRA: recv pbcch info ind, only send to rr %d")
TRACE_MSG(RR_ADAPTER_2494_112_2_18_0_9_40_1159,"RRA: can't do bcch camp, because there is pbcch camp")
TRACE_MSG(RR_ADAPTER_2520_112_2_18_0_9_41_1160,"RRA: can't do bcch camp, because there is card conflict")
TRACE_MSG(RR_ADAPTER_2606_112_2_18_0_9_41_1161,"RRA: clear mac rlc flag %d")
TRACE_MSG(RR_ADAPTER_2619_112_2_18_0_9_41_1162,"RRA: camp param of is same as before, discard it, req_rr = %d")
TRACE_MSG(RR_ADAPTER_2639_112_2_18_0_9_41_1163,"RRA: camp arfcn = 0x%x, req_rr = %d")
TRACE_MSG(RR_ADAPTER_2854_112_2_18_0_9_41_1164,"RRA: exist active rr, rr %d donot send deactive to layer1.")
TRACE_MSG(RR_ADAPTER_2870_112_2_18_0_9_41_1165,"RRA: deactive req_rr = %d, s_rra_master_id = %d")
TRACE_MSG(RR_ADAPTER_2919_112_2_18_0_9_41_1166,"RRA: send main campon to layer1, master rr = %d")
TRACE_MSG(RR_ADAPTER_2934_112_2_18_0_9_41_1167,"RRA: layer1 enter deactive.")
TRACE_MSG(RR_ADAPTER_2948_112_2_18_0_9_41_1168,"RRA: MPH_DEACTIVATE_REQ discarded in rat change, rr %d")
TRACE_MSG(RR_ADAPTER_2977_112_2_18_0_9_41_1169,"RRA: rr %d enter RRA_RR_NULL, master id = %d")
TRACE_MSG(RR_ADAPTER_3000_112_2_18_0_9_42_1170,"RRA: HandleMphDataInd Param4(0x%x)\n")
TRACE_MSG(RR_ADAPTER_3003_112_2_18_0_9_42_1171,"RRA: HandleMphDataInd cardid(0x%x),frameno(0x%x)\n")
TRACE_MSG(RR_ADAPTER_3024_112_2_18_0_9_42_1172,"RRA: HandleMphDataInd i(0x%x)\n")
TRACE_MSG(RR_ADAPTER_3101_112_2_18_0_9_42_1173,"RRA: dis card rrc %d page mode %d.")
TRACE_MSG(RR_ADAPTER_3123_112_2_18_0_9_42_1174,"RRA: send MPH_BCCH_CAMP_REQ, rr = %d")
TRACE_MSG(RR_ADAPTER_3219_112_2_18_0_9_42_1175,"HandleRandomAccessReq:there exist ManualPLMNSelect or ModeChange or NasRatChange or SynchronizeModeChange")
TRACE_MSG(RR_ADAPTER_3247_112_2_18_0_9_42_1176,"HandleRandomAccessReq:rr %d RA cause is %d")
TRACE_MSG(RR_ADAPTER_3262_112_2_18_0_9_42_1177,"HandleRandomAccessReq:rr %d last RA cause is %d")
TRACE_MSG(RR_ADAPTER_3317_112_2_18_0_9_42_1178,"RRA: rr %d rach delay 1s in order to waiting another card terminate its ps service.")
TRACE_MSG(RR_ADAPTER_3329_112_2_18_0_9_42_1179,"RRA: rr %d rach delay 1s while in rf scan protect.")
TRACE_MSG(RR_ADAPTER_3390_112_2_18_0_9_42_1180,"RRA: send 4 MPH_BCCH_CAMP_REQ, rr = %d")
TRACE_MSG(RR_ADAPTER_3422_112_2_18_0_9_42_1181,"RRA: send RRA_MPH_RANDOM_ACCESS_REQ, rr = %d")
TRACE_MSG(RR_ADAPTER_3431_112_2_18_0_9_42_1182,"RRA: xxrr %d enter RRA_RR_TRAFFIC.")
TRACE_MSG(RR_ADAPTER_3511_112_2_18_0_9_43_1183,"RRA: rr %d enter RRA_RR_IDLE.")
TRACE_MSG(RR_ADAPTER_3738_112_2_18_0_9_43_1184,"RRA: do not send %d meas report while in mode change, manual plmn sel etc.")
TRACE_MSG(RR_ADAPTER_3749_112_2_18_0_9_43_1185,"RRA: do not send %d meas report while in rf scan protect.")
TRACE_MSG(RR_ADAPTER_3899_112_2_18_0_9_43_1186,"RRA: ba combinate exceed, discard some info.")
TRACE_MSG(RR_ADAPTER_4138_112_2_18_0_9_44_1187,"RRA: act gprs = %d")
TRACE_MSG(RR_ADAPTER_4262_112_2_18_0_9_44_1188,"RRA: DeliverToRRCSig rr id = %d")
TRACE_MSG(RR_ADAPTER_4407_112_2_18_0_9_44_1189,"RRA: receive PBCCH CAMP REQ but can't do pbcch camp, because another exist")
TRACE_MSG(RR_ADAPTER_4422_112_2_18_0_9_44_1190,"RRA: enter PBCCH CAMP")
TRACE_MSG(RR_ADAPTER_4432_112_2_18_0_9_44_1191,"RRA: clear mac rlc flag %d")
TRACE_MSG(RR_ADAPTER_4438_112_2_18_0_9_44_1192,"RRA: pbcch camp arfcn = 0x%x, req_rr = %d")
TRACE_MSG(RR_ADAPTER_4549_112_2_18_0_9_45_1193,"RRA: handoverReq req_rr %d, arfcn_before_handover %d, camp_arfcn %d")
TRACE_MSG(RR_ADAPTER_4616_112_2_18_0_9_45_1194,"RRA: assert case, exp ind sig = 0x%x.")
TRACE_MSG(RR_ADAPTER_4655_112_2_18_0_9_45_1195,"RRA: rach delay exp, master id = %d")
TRACE_MSG(RR_ADAPTER_4709_112_2_18_0_9_45_1196,"RRA: rr to mac, s_rra_gprs_id = 0x%x")
TRACE_MSG(RR_ADAPTER_4811_112_2_18_0_9_45_1197,"RRA: discard immed assign!")
TRACE_MSG(RR_ADAPTER_4835_112_2_18_0_9_45_1198,"RRA: discard mac rach rsp. rr = %d, s_rra_gprs_id = %d")
TRACE_MSG(RR_ADAPTER_4881_112_2_18_0_9_45_1199,"RRA: rr to rlc, s_rra_gprs_id = 0x%x")
TRACE_MSG(RR_ADAPTER_4929_112_2_18_0_9_46_1200,"RRA: handle deact req, req_rr = %d, s_rra_master_id = %d, s_rra_gprs_id = %d")
TRACE_MSG(RR_ADAPTER_4934_112_2_18_0_9_46_1201,"RRA: rr %d state = %d")
TRACE_MSG(RR_ADAPTER_4955_112_2_18_0_9_46_1202,"RRA: invalid s_rra_gprs_id.")
TRACE_MSG(RR_ADAPTER_4967_112_2_18_0_9_46_1203,"RRA: use rr %d param to act rlc.")
TRACE_MSG(RR_ADAPTER_4996_112_2_18_0_9_46_1204,"RRA: SetGprsParaReq,req_rr = %d, gprs id = %d")
TRACE_MSG(RR_ADAPTER_5017_112_2_18_0_9_46_1205,"RRA: rlc act flag %d")
TRACE_MSG(RR_ADAPTER_5127_112_2_18_0_9_46_1206,"RRA: latest traffic id %d")
TRACE_MSG(RR_ADAPTER_5199_112_2_18_0_9_46_1207,"RRA: rf scan ind: rr %d in state %d")
TRACE_MSG(RR_ADAPTER_5280_112_2_18_0_9_46_1208,"RRA: copy rf scan num = %d")
TRACE_MSG(RR_ADAPTER_5313_112_2_18_0_9_47_1209,"RRA: layer1 rach expired, master id = %d")
TRACE_MSG(RR_ADAPTER_5344_112_2_18_0_9_47_1210,"RRA: start rf scan protect timer.")
TRACE_MSG(RR_ADAPTER_5495_112_2_18_0_9_47_1211,"RRA: mac freq %d, type = %d")
TRACE_MSG(RR_ADAPTER_5519_112_2_18_0_9_47_1212,"RRA: unknow mac freq type %d")
TRACE_MSG(RR_ADAPTER_5791_112_2_18_0_9_48_1213,"RRA: bcch camp req_rr = %d, before camp, s_rra_master_id = %d")
TRACE_MSG(RR_ADAPTER_6170_112_2_18_0_9_48_1214,"RRA: recv unexpected tc = 0.")
TRACE_MSG(RR_ADAPTER_6245_112_2_18_0_9_49_1215,"RRA: bcch list decode arfcn = 0x%x, band = %d, tc in = 0x%x, out = 0x%x")
TRACE_MSG(RR_ADAPTER_6444_112_2_18_0_9_49_1216,"RRA: unknow signal 0x%x,free.")
TRACE_MSG(RR_ADAPTER_6499_112_2_18_0_9_49_1217,"RRA: invalid arfcn index, BAND %d, arfcn %d")
TRACE_MSG(RR_ADAPTER_6534_112_2_18_0_9_49_1218,"RRA: HandleMphChangeModeReq rr %d, rr_task_id = 0x%x")
TRACE_MSG(RR_ADAPTER_6535_112_2_18_0_9_49_1219,"RRA: HandleMphChangeModeReq rr_state %d")
TRACE_MSG(RR_ADAPTER_6538_112_2_18_0_9_49_1220,"RRA: HandleMphChangeModeReq rr %d, rr_task_id = 0x%x")
TRACE_MSG(RR_ADAPTER_6628_112_2_18_0_9_49_1221,"RRA: xxrr %d enter pseudo RRA_RR_TRAFFIC.")
TRACE_MSG(RR_ADAPTER_6680_112_2_18_0_9_49_1222,"RRA: process dummy rat change to TD!")
TRACE_MSG(RR_ADAPTER_6773_112_2_18_0_9_50_1223,"RRA: HandleMphChangeModeReq to send grr(%d) all zero measure report!")
TRACE_MSG(RR_ADAPTER_6798_112_2_18_0_9_50_1224,"RRA: nas_rat_change_flag = TRUE when no one knows it begins rat change!")
TRACE_MSG(RR_ADAPTER_6825_112_2_18_0_9_50_1225,"RRA: s_rra_curr_work_mode = %d, when change mode!!!")
TRACE_MSG(RR_ADAPTER_6872_112_2_18_0_9_50_1226,"RRA: rr %d rach delay 1s while in rf scan protect.")
TRACE_MSG(RR_ADAPTER_7039_112_2_18_0_9_50_1227,"RRA: xxrr %d enter RRA_RR_TRAFFIC.")
TRACE_MSG(RR_ADAPTER_7166_112_2_18_0_9_51_1228,"RRA: HandleMphStartTdMeasReq rr %d, rr_task_id = 0x%x")
TRACE_MSG(RR_ADAPTER_7167_112_2_18_0_9_51_1229,"RRA: HandleMphStartTdMeasReq rr_state %d")
TRACE_MSG(RR_ADAPTER_7243_112_2_18_0_9_51_1230,"RRA: HandleMphUpdateTdMeasReq rr %d, rr_task_id = 0x%x")
TRACE_MSG(RR_ADAPTER_7244_112_2_18_0_9_51_1231,"RRA: HandleMphUpdateTdMeasReq rr_state %d")
TRACE_MSG(RR_ADAPTER_7247_112_2_18_0_9_51_1232,"RRA: HandleMphUpdateTdMeasReq td_meas_start_flag is false!")
TRACE_MSG(RR_ADAPTER_7317_112_2_18_0_9_51_1233,"RRA: HandleMphStopTdMeasReq rr %d, rr_task_id = 0x%x")
TRACE_MSG(RR_ADAPTER_7318_112_2_18_0_9_51_1234,"RRA: HandleMphStopTdMeasReq rr_state %d")
TRACE_MSG(RR_ADAPTER_7321_112_2_18_0_9_51_1235,"RRA: HandleMphStopTdMeasReq td_meas_start_flag is false!")
TRACE_MSG(RR_ADAPTER_7395_112_2_18_0_9_51_1236,"RRA: rra_combine_td_measurement A+B combine OK : req_rr %d;  arfcn %d ")
TRACE_MSG(RR_ADAPTER_7416_112_2_18_0_9_51_1237,"RRA: rra_combine_td_measurement combine OK : req_rr %d;  arfcn %d, cell_param = 0x%x")
TRACE_MSG(RR_ADAPTER_7428_112_2_18_0_9_51_1238,"RRA: rra_combine_td_measurement combine failed, reach MAX_CELL_PARAM_ID_PER_ARFCN : req_rr %d;  arfcn %d, cell_param = 0x%x")
TRACE_MSG(RR_ADAPTER_7439_112_2_18_0_9_51_1239,"RRA: rra_combine_td_measurement combine arfcn OK : req_rr %d;  arfcn %d")
TRACE_MSG(RR_ADAPTER_7450_112_2_18_0_9_51_1240,"RRA: rra_combine_td_measurement combine arfcn failed, i_arfcn_combine reach MAX_CELL_PARAM_ID_PER_ARFCN : req_rr %d;")
TRACE_MSG(RR_ADAPTER_7546_112_2_18_0_9_51_1241,"RRA: HandleTdMeasReportInd to s_rra_master_id %d;  cell_num %d; arfcn_num  %d")
TRACE_MSG(RR_ADAPTER_7568_112_2_18_0_9_51_1242,"RRA: do not send %d TD meas report while in rat change or manual plmn select/search.")
TRACE_MSG(RR_ADAPTER_7581_112_2_18_0_9_51_1243,"RRA: do not send %d td meas report while in rf scan protect.")
TRACE_MSG(RR_ADAPTER_7648_112_2_18_0_9_52_1244,"RRA: HandleTdMeasReportInd to i_card %d;  cell_num %d; arfcn_num  %d")
TRACE_MSG(RR_ADAPTER_7724_112_2_18_0_9_52_1245,"RRA:rat change to 2G,but 2G is not activated.")
TRACE_MSG(RR_ADAPTER_7786_112_2_18_0_9_52_1246,"RRA: rr handshake type is wrong, discarded the handshake request!")
TRACE_MSG(RR_ADAPTER_7819_112_2_18_0_9_52_1247,"RRA: HandleRrRraGsm2TdResultReq -- failed, change mode rejected by RRA, rr = %d!")
TRACE_MSG(RR_ADAPTER_7829_112_2_18_0_9_52_1248,"RRA: HandleRrRraGsm2TdResultReq -- enter pseudo traffic state, rr = %d!")
TRACE_MSG(RR_ADAPTER_7907_112_2_18_0_9_52_1249,"RRA: no plmn happened in error condition to RR (%d)!")
TRACE_MSG(RR_ADAPTER_7934_112_2_18_0_9_52_1250,"RRA: RR_RRA_NOTIFY_REQ error type param = %d!")
TRACE_MSG(RR_ADAPTER_7957_112_2_18_0_9_52_1251,"RrcaRraStateNotify: rrca_notify_ptr, rrca_status = %d, camp_flag = %d, card_id = %d!")
TRACE_MSG(RR_ADAPTER_7960_112_2_18_0_9_52_1252,"RrcaRraStateNotify: TRUE == rrca_notify_ptr->Param2!   TRUE = %d ")
TRACE_MSG(RR_ADAPTER_7971_112_2_18_0_9_52_1253,"RrcaRraStateNotify: before s_rra_rr_info[req_rr].rrca_status = %d!")
TRACE_MSG(RR_ADAPTER_7980_112_2_18_0_9_52_1254,"RRA: nas rat change 2G->3G Failed, no plmn in 3G!")
TRACE_MSG(RR_ADAPTER_8007_112_2_18_0_9_52_1255,"RrcaRraStateNotify: rrca_notify_ptr->Param1 != RRA_RRCA_STATUS_NO_CELL &&rrca_notify_ptr->Param2 ")
TRACE_MSG(RR_ADAPTER_8234_112_2_18_0_9_53_1256,"RRA: can't do bcch camp, because there is Mode Change")
TRACE_MSG(RR_ADAPTER_8245_112_2_18_0_9_53_1257,"RRA: can't do bcch camp, because there is Mode Change")
TRACE_MSG(RR_ADAPTER_8469_112_2_18_0_9_53_1258,"RRA_GetPsDisturbStatus: s_is_ps_disturb_by_other_cs %d")
TRACE_MSG(SCT_PROC_87_112_2_18_0_9_54_1259,"SCT: Close tch loop type I")
TRACE_MSG(SCT_PROC_92_112_2_18_0_9_54_1260,"SCT: cmd error C %d, B%d, A%d")
TRACE_MSG(SCT_PROC_102_112_2_18_0_9_54_1261,"SCT: Close tch loop type F")
TRACE_MSG(SCT_PROC_107_112_2_18_0_9_54_1262,"SCT loop type E")
TRACE_MSG(SCT_PROC_113_112_2_18_0_9_54_1263,"SCT: Close tch loop type D")
TRACE_MSG(SCT_PROC_120_112_2_18_0_9_54_1264,"SCT: Close tch loop type C")
TRACE_MSG(SCT_PROC_127_112_2_18_0_9_54_1265,"SCT: Close tch loop type B")
TRACE_MSG(SCT_PROC_132_112_2_18_0_9_54_1266,"SCT: Close tch loop type A")
TRACE_MSG(SCT_PROC_141_112_2_18_0_9_54_1267,"SCT: Open loop command")
TRACE_MSG(SCT_PROC_147_112_2_18_0_9_54_1268,"SCT: Test interfaec %d ")
TRACE_MSG(SCT_PROC_205_112_2_18_0_9_54_1269,"SCT: GPRS loop command")
TRACE_MSG(SCT_PROC_216_112_2_18_0_9_54_1270,"SCT:EGPRS SRB loopback command")
TRACE_MSG(SCT_PROC_250_112_2_18_0_9_54_1271,"SCT: Close tch loop ack")
TRACE_MSG(SCT_PROC_263_112_2_18_0_9_54_1272,"SCT: Open tch loop ack")
TRACE_MSG(CCENTITY_5433_112_2_18_0_10_18_1273,"CC: send start dtmf at %d staten")
TRACE_MSG(CCENTITY_5478_112_2_18_0_10_18_1274,"CC: send stop dtmf at %d state n")
TRACE_MSG(CMROUTER_337_112_2_18_0_10_22_1275,"cmrouter: Rel_PId, ti is %x\n")
TRACE_MSG(CMROUTER_1746_112_2_18_0_10_24_1276,"cmrouter: deal MNSS_END_REQ, ti is %x, entity not exist\n")
TRACE_MSG(CMROUTER_2192_112_2_18_0_10_25_1277,"cmrouter: deal MNSS_END_REQ, ti is %x, entity not exist\n")
TRACE_MSG(CMROUTER_2610_112_2_18_0_10_26_1278,"cmrouter: deal MNSS_END_REQ, ti is %x, entity not exist\n")
TRACE_MSG(DLR_272_112_2_18_0_10_36_1279,"DL: release req 0,but no entity")
TRACE_MSG(DLR_296_112_2_18_0_10_36_1280,"DL: release req 3,but no entity")
TRACE_MSG(DLR_458_112_2_18_0_10_37_1281,"L2: unexpected sapi\n")
TRACE_MSG(DLR_776_112_2_18_0_10_37_1282,"PH_READY_TO_SEND_IND recved, but task released\n")
TRACE_MSG(GMM_2185_112_2_18_0_10_42_1283,"GMM: deregistered gmm_state =%d")
TRACE_MSG(GMM_2381_112_2_18_0_10_42_1284,"GMM:SDL cell_status = %d")
TRACE_MSG(GMM_2509_112_2_18_0_10_42_1285,"GMM:SDL return_state =%d")
TRACE_MSG(GMM_2586_112_2_18_0_10_43_1286,"GMM:Ignore msg_type = %d")
TRACE_MSG(GMM_2845_112_2_18_0_10_43_1287,"GMM:SDL Error gmm_state =%d")
TRACE_MSG(GMM_2991_112_2_18_0_10_44_1288,"APP_GMM_DETACH_REQ:g_detach_type=%d")
TRACE_MSG(GMM_3002_112_2_18_0_10_44_1289,"GMM error:g_detach_type=%d")
TRACE_MSG(GMM_3124_112_2_18_0_10_44_1290,"GMM: error g_gmm_state is %d")
TRACE_MSG(GMM_3261_112_2_18_0_10_44_1291,"GMM: error cell_status is %d")
TRACE_MSG(GMM_3408_112_2_18_0_10_45_1292,"GMM:wrong state =%d")
TRACE_MSG(GMM_3515_112_2_18_0_10_45_1293,"GMM:free sm unitdata request!")
TRACE_MSG(GMM_3535_112_2_18_0_10_45_1294,"GMM:free sm unitdata request in states!")
TRACE_MSG(GMM_3815_112_2_18_0_10_46_1295,"GMM:T3312 is deactivated and the MS shall not perform periodic routing area updating!")
TRACE_MSG(GMM_3860_112_2_18_0_10_46_1296,"GMM:T3312 is deactivated and the MS shall not perform periodic routing area updating!")
TRACE_MSG(GMM_3979_112_2_18_0_10_46_1297,"GMM:Igore msg_type = %d")
TRACE_MSG(GMM_4111_112_2_18_0_10_46_1298,"impossible g_detach_type")
TRACE_MSG(GMM_4659_112_2_18_0_10_47_1299,"Don't received the signal")
TRACE_MSG(GMM_4764_112_2_18_0_10_47_1300,"GMM:Igore msg_type = %d")
TRACE_MSG(GMM_4942_112_2_18_0_10_48_1301,"GMM:Assert Failure,gmm_state =%d")
TRACE_MSG(GMM_5109_112_2_18_0_10_48_1302,"GMM:Assert Error,g_detach_type=%d")
TRACE_MSG(GMM_5158_112_2_18_0_10_48_1303,"GMM:T3312 is deactivated and the MS shall not perform periodic routing area updating!")
TRACE_MSG(GMM_5295_112_2_18_0_10_49_1304,"GMM:Ignore msg_type =%d")
TRACE_MSG(GMM_5385_112_2_18_0_10_49_1305,"GMM:Assert Error,T3321 g_detach_type =%d")
TRACE_MSG(GMM_5391_112_2_18_0_10_49_1306,"GMM: NOT recieve DETACH ACC,  T3321expire 4t imes")
TRACE_MSG(GMM_5515_112_2_18_0_10_49_1307,"GMM:Assert,Error gmm_state =%d")
TRACE_MSG(GMM_5679_112_2_18_0_10_50_1308,"GMM: resume GMM, stop Detach")
TRACE_MSG(GMM_5917_112_2_18_0_10_50_1309,"GMM:Ack APP Detach for poweroff")
TRACE_MSG(GMM_5941_112_2_18_0_10_50_1310,"GMM:MN Update plmn forbid list")
TRACE_MSG(GMM_5982_112_2_18_0_10_50_1311,"GMM:handle_rau_reject_sdl attach fail")
TRACE_MSG(GMM_6039_112_2_18_0_10_50_1312,"GMM:detach_reattach fail")
TRACE_MSG(GMM_6132_112_2_18_0_10_50_1313,"GMM: error g_gmm_state is %d")
TRACE_MSG(GMM_6303_112_2_18_0_10_51_1314,"GMM: SDL 1 return_state =%d")
TRACE_MSG(GMM_6424_112_2_18_0_10_51_1315,"GMM:SDL Assert,g_detach_type =%d")
TRACE_MSG(GMM_6430_112_2_18_0_10_51_1316,"DECODE DETACH_ACCEPT FAIL")
TRACE_MSG(GMM_6615_112_2_18_0_10_51_1317,"GMM:Assert Error,return_state=%d")
TRACE_MSG(GMM_6700_112_2_18_0_10_52_1318,"GMM: recieve DETACH ACC, stop T3321")
TRACE_MSG(GMM_6743_112_2_18_0_10_52_1319,"GMM:Assert Error,g_detach_type = %d")
TRACE_MSG(GMM_6798_112_2_18_0_10_52_1320,"GMM:decode detach accept fail.")
TRACE_MSG(GMM_6838_112_2_18_0_10_52_1321,"GMM:Assert Error,any state g_gmm_state =%d")
TRACE_MSG(GMM_7012_112_2_18_0_10_52_1322,"GMM:Enter is_message_allowed_to_send_sdl")
TRACE_MSG(GMM_7016_112_2_18_0_10_52_1323,"GMM:access is barred")
TRACE_MSG(GMM_7029_112_2_18_0_10_52_1324,"GMM:gmm is suspended")
TRACE_MSG(GMM_7044_112_2_18_0_10_52_1325,"GMM:g_reg_substate= REG_UPDATE_NEEDED")
TRACE_MSG(GMM_7080_112_2_18_0_10_52_1326,"GMM:is message,ATTACH_NEEDED")
TRACE_MSG(GMM_7140_112_2_18_0_10_52_1327,"GMM:substate isnot allowed to attach")
TRACE_MSG(GMM_7271_112_2_18_0_10_53_1328,"GMM: Ready to Attach,Attach Type = %d! ")
TRACE_MSG(GMM_7385_112_2_18_0_10_53_1329,"GMM:not allowed to attach ")
TRACE_MSG(GMM_7421_112_2_18_0_10_53_1330,"GMM: ptmsi_flag_pro =%d ,tmsi_is= %d")
TRACE_MSG(GMM_7649_112_2_18_0_10_53_1331,"GMM:Decode rau reject err")
TRACE_MSG(GMM_8313_112_2_18_0_10_55_1332,"GMM:call handle_rau_req_sdl failure")
TRACE_MSG(GMM_8503_112_2_18_0_10_55_1333,"GMM:plmn mismatch!")
TRACE_MSG(GMM_8698_112_2_18_0_10_55_1334,"GMM: Assert Failure, wrong g_gmm_state = %d")
TRACE_MSG(GMM_9053_112_2_18_0_10_57_1335,"Gmm: rauattemp count > 5")
TRACE_MSG(GMM_9662_112_2_18_0_10_58_1336,"GMM:rau accept rai mismatch!")
TRACE_MSG(GMM_9870_112_2_18_0_10_59_1337,"GMM:error rau_accept!update_result")
TRACE_MSG(GMM_9882_112_2_18_0_10_59_1338,"GMM:RAU type mismatch!")
TRACE_MSG(GMM_9919_112_2_18_0_10_59_1339,"GMM:decode error")
TRACE_MSG(GMM_10057_112_2_18_0_10_59_1340,"GMM:send_detach_req_sdl return false")
TRACE_MSG(GMM_10112_112_2_18_0_11_0_1341,"GMM:Receive Ready Timer Run Forever!")
TRACE_MSG(GMM_10218_112_2_18_0_11_0_1342,"GMM:T3312 is deactivated and the MS shall not perform periodic routing area updating !")
TRACE_MSG(GMM_10558_112_2_18_0_11_1_1343,"GMM:Attach type mismatch!")
TRACE_MSG(GMM_10562_112_2_18_0_11_1_1344,"GMM:Attach result value is reserved!")
TRACE_MSG(GMM_10604_112_2_18_0_11_1_1345,"GMM:attach accept rai mismatch!")
TRACE_MSG(GMM_10750_112_2_18_0_11_1_1346,"GMM error:g_detach_type=%d")
TRACE_MSG(GMM_10882_112_2_18_0_11_2_1347,"GMM:Attach result value is reserved!")
TRACE_MSG(GMM_10891_112_2_18_0_11_2_1348,"GMM:Decode failure")
TRACE_MSG(GMM_10988_112_2_18_0_11_2_1349,"GMM:Decode attach reject message is err")
TRACE_MSG(GMM_11370_112_2_18_0_11_3_1350,"GMM:enter send_detach_req_sdl")
TRACE_MSG(GMM_11486_112_2_18_0_11_3_1351,"GMM:don't send message")
TRACE_MSG(GMM_11629_112_2_18_0_11_3_1352,"GMM:detach_req!detach_type =%d")
TRACE_MSG(GMM_12021_112_2_18_0_11_4_1353,"GMM:invalid detach cause %d,yPrdVarP->z0000040R3YE_detach_req.gmm_cause")
TRACE_MSG(GMM_12239_112_2_18_0_11_4_1354,"GMM: DecodeAuthCiphRequest error = %d")
TRACE_MSG(GMM_12445_112_2_18_0_11_5_1355,"GMM:in FPLMN list")
TRACE_MSG(GMM_12474_112_2_18_0_11_5_1356,"GMM:LAI in Forbidded list")
TRACE_MSG(GMM_12528_112_2_18_0_11_5_1357,"GMM:lai in Forbidded list")
TRACE_MSG(GMM_12649_112_2_18_0_11_5_1358,"GMM: Can't handle this PD, discard it.")
TRACE_MSG(LAYER1_629_112_2_18_0_11_47_1359,"deactive req.\n")
TRACE_MSG(LAYER1_702_112_2_18_0_11_47_1360,"rxpower_sweep_req asked and confirm replied.\n")
TRACE_MSG(LAYER1_720_112_2_18_0_11_47_1361,"rxpower from BS come.\n")
TRACE_MSG(LAYER1_738_112_2_18_0_11_47_1362,"bcch req asked.\n")
TRACE_MSG(LAYER1_749_112_2_18_0_11_47_1363,"the bcch req is single %d.\n")
TRACE_MSG(LAYER1_771_112_2_18_0_11_47_1364,"bcch req stop.\n")
TRACE_MSG(LAYER1_788_112_2_18_0_11_47_1365,"bcch timer expired.\n")
TRACE_MSG(LAYER1_823_112_2_18_0_11_47_1366,"bsic req asked.\n")
TRACE_MSG(LAYER1_899_112_2_18_0_11_47_1367,"\n*****Layer1 Rx BCCH:*****\n")
TRACE_MSG(LAYER1_900_112_2_18_0_11_47_1368,"BCCH_ARFCN:       %d \n\n")
TRACE_MSG(LAYER1_941_112_2_18_0_11_47_1369,"\n*****L1Sim Rx BCCH:*****\n")
TRACE_MSG(LAYER1_942_112_2_18_0_11_47_1370,"BCCH_ARFCN:%d \n\n")
TRACE_MSG(LAYER1_985_112_2_18_0_11_47_1371,"bcch ind true\n")
TRACE_MSG(LAYER1_1146_112_2_18_0_11_48_1372,"campon req&confirmed.\n")
TRACE_MSG(LAYER1_1244_112_2_18_0_11_48_1373,"TBF Downlink Assignment Recv\n")
TRACE_MSG(LAYER1_1317_112_2_18_0_11_48_1374,"Send the first RA\n")
TRACE_MSG(LAYER1_1391_112_2_18_0_11_48_1375,"L1 Send Random Acess Burst\n")
TRACE_MSG(LAYER1_1445_112_2_18_0_11_48_1376,"AGCH recv\n")
TRACE_MSG(LAYER1_1717_112_2_18_0_11_49_1377,"PLMN search asked and  rxpower confirm replied.\n")
TRACE_MSG(LAYER1_2273_112_2_18_0_11_50_1378,"chan_assign:%d\n")
TRACE_MSG(LAYER1_2315_112_2_18_0_11_50_1379,"chan_assign_fail:%d\n")
TRACE_MSG(LAYER1_2398_112_2_18_0_11_50_1380,"handover_fail:%d\n")
TRACE_MSG(LAYER1_2512_112_2_18_0_11_50_1381,"Received Data block from new DCCH in non-syn handover\n")
TRACE_MSG(LAYER1_2521_112_2_18_0_11_50_1382,"handover :%d\n")
TRACE_MSG(LAYER1_2596_112_2_18_0_11_50_1383,"handover_fail:%d\n")
TRACE_MSG(LAYER1_2645_112_2_18_0_11_51_1384,"L1:sacch data recv\n")
TRACE_MSG(LAYER1_2667_112_2_18_0_11_51_1385,"l1 recv data:%d\n")
TRACE_MSG(LAYER1_2747_112_2_18_0_11_51_1386,"L1:data send at chan %d\n")
TRACE_MSG(LAYER1_2769_112_2_18_0_11_51_1387,"L1:sacch data send\n")
TRACE_MSG(LAYER1_2963_112_2_18_0_11_51_1388,"This bcch block is unexpected\n")
TRACE_MSG(LAYER1_3001_112_2_18_0_11_51_1389,"bcch ind,true\n")
TRACE_MSG(LAYER1_3008_112_2_18_0_11_51_1390,"bcch ind,not requested\n")
TRACE_MSG(LAYER1_3121_112_2_18_0_11_51_1391,"SI_ID: 1")
TRACE_MSG(LAYER1_3125_112_2_18_0_11_51_1392,"SI_ID: 2")
TRACE_MSG(LAYER1_3129_112_2_18_0_11_51_1393,"SI_ID: 2Bis")
TRACE_MSG(LAYER1_3133_112_2_18_0_11_51_1394,"SI_ID: 2Ter")
TRACE_MSG(LAYER1_3137_112_2_18_0_11_51_1395,"SI_ID: 3")
TRACE_MSG(LAYER1_3141_112_2_18_0_11_51_1396,"SI_ID: 4")
TRACE_MSG(LAYER1_3145_112_2_18_0_11_51_1397,"SI_ID: 13")
TRACE_MSG(LAYER1_3149_112_2_18_0_11_52_1398,"SI_ID: 5BIS")
TRACE_MSG(LAYER1_3153_112_2_18_0_11_52_1399,"SI_ID: 5BIS")
TRACE_MSG(LAYER1_3157_112_2_18_0_11_52_1400,"SI_ID: 6")
TRACE_MSG(LAYER1_3161_112_2_18_0_11_52_1401,"SI_ID: 7")
TRACE_MSG(LAYER1_3165_112_2_18_0_11_52_1402,"PSI_ID: 13")
TRACE_MSG(LAYER1_3169_112_2_18_0_11_52_1403,"SI_ID: 4")
TRACE_MSG(LAYER1_3173_112_2_18_0_11_52_1404,"PSI_ID: 3Bis")
TRACE_MSG(LAYER1_3177_112_2_18_0_11_52_1405,"SI_ID: 8")
TRACE_MSG(LAYER1_3181_112_2_18_0_11_52_1406,"SI_ID: 9")
TRACE_MSG(LAYER1_3185_112_2_18_0_11_52_1407,"SI_ID: 13")
TRACE_MSG(LAYER1_3189_112_2_18_0_11_52_1408,"PSI_ID: 1")
TRACE_MSG(LAYER1_3193_112_2_18_0_11_52_1409,"PSI_ID: 2")
TRACE_MSG(LAYER1_3197_112_2_18_0_11_52_1410,"PSI_ID: 3 ")
TRACE_MSG(LLENTITY_876_112_2_18_0_11_54_1411,"LLentity:impossible U frame type\n")
TRACE_MSG(LLENTITY_1077_112_2_18_0_11_54_1412,"LLentity:impossible I frame type\n")
TRACE_MSG(LLENTITY_1186_112_2_18_0_11_54_1413,"LLentity:impossible S frame type\n")
TRACE_MSG(LLENTITY_1293_112_2_18_0_11_54_1414,"LLentity:recvd SNDCP ACK pdu in unack mode!\n")
TRACE_MSG(LLENTITY_2499_112_2_18_0_11_57_1415,"LLentity:unexpected value\n")
TRACE_MSG(LLENTITY_3047_112_2_18_0_11_58_1416,"LLE:received invalid command on ADM state")
TRACE_MSG(LLMAIN_676_112_2_18_0_12_0_1417,"LLMAIN: discard l3 pdu, tlli mismatch")
TRACE_MSG(LLMAIN_918_112_2_18_0_12_1_1418,"LLc Frame Error %d")
TRACE_MSG(LLMAIN_1303_112_2_18_0_12_2_1419,"LLc2 Frame Error %d")
TRACE_MSG(LLMAIN_1367_112_2_18_0_12_2_1420,"LLMAIN:discard l3 pdu, tlli mismatch,sapi %d\n")
TRACE_MSG(LLMAIN_1775_112_2_18_0_12_3_1421,"LLMAIN: discard pdu, wrong state")
TRACE_MSG(LLMAIN_1818_112_2_18_0_12_3_1422,"LLMAIN: discard pdu, wrong state")
TRACE_MSG(LLMAIN_1842_112_2_18_0_12_3_1423,"LLMAIN: discard l3 pdu,wrong state")
TRACE_MSG(LLMAIN_1864_112_2_18_0_12_3_1424,"LLMAIN: discard l3 pdu,wrong state")
TRACE_MSG(MAC_500_112_2_18_0_12_5_1425,"MAC:try to start t3192 0 ms")
TRACE_MSG(MAC_960_112_2_18_0_12_5_1426,"MAC: Decode grr freq req error!\n")
TRACE_MSG(MAC_997_112_2_18_0_12_5_1427,"MAC: DL imm assi,freq Acquare Sys Info error")
TRACE_MSG(MAC_1166_112_2_18_0_12_6_1428,"MAC: DL imm assi,Decode ia rest octets error!")
TRACE_MSG(MAC_1187_112_2_18_0_12_6_1429,"MAC: DL imm assi,Decode chan descri error!")
TRACE_MSG(MAC_1245_112_2_18_0_12_6_1430,"MAC: recv pkt dl ass from rrc alloc no dl slot! Ignore pdch dl req!\n")
TRACE_MSG(MAC_1262_112_2_18_0_12_6_1431,"MAC: Decode freq para error! Acquare Sys Info")
TRACE_MSG(MAC_1531_112_2_18_0_12_7_1432,"MAC: Multislot capability not match! Ignore pdch dl req!")
TRACE_MSG(MAC_1539_112_2_18_0_12_7_1433,"MAC: TBF_MODE in packet downlink ass is different from current!wrong! \n")
TRACE_MSG(MAC_1545_112_2_18_0_12_7_1434,"MAC: No tfi assign,ignore the packet DL assi from ppch!")
TRACE_MSG(MAC_1552_112_2_18_0_12_7_1435,"MAC: Decode packet DL error in idle, ignore the msg!")
TRACE_MSG(MAC_1737_112_2_18_0_12_7_1436,"MAC: Decode rach response error! Acquare Sys Info")
TRACE_MSG(MAC_1756_112_2_18_0_12_7_1437,"MAC: One phase received Dl single assi!\n")
TRACE_MSG(MAC_2092_112_2_18_0_12_8_1438,"MAC: Single block without TBF!")
TRACE_MSG(MAC_2219_112_2_18_0_12_8_1439,"MAC: One phase received Dl assi!")
TRACE_MSG(MAC_2226_112_2_18_0_12_8_1440,"MAC: One phase received Dl EGPRS assi!")
TRACE_MSG(MAC_2244_112_2_18_0_12_8_1441,"MAC: Decode rach response error! Rach retry!")
TRACE_MSG(MAC_2285_112_2_18_0_12_8_1442,"MAC: wrong condition receive rach rsp")
TRACE_MSG(MAC_2321_112_2_18_0_12_8_1443,"MAC: Rach response reject, Rach retry!")
TRACE_MSG(MAC_2374_112_2_18_0_12_8_1444,"MAC: Rach response reject, ul tbf failure!\n")
TRACE_MSG(MAC_2485_112_2_18_0_12_9_1445,"MAC: not support GRR_MAC_PDCH_ASSIGN_REQ")
TRACE_MSG(MAC_2637_112_2_18_0_12_9_1446,"MAC: access abort invalid")
TRACE_MSG(MAC_2690_112_2_18_0_12_9_1447,"MAC: access abort invalid")
TRACE_MSG(MAC_2990_112_2_18_0_12_10_1448,"MAC: tbf mode invalid in connect cnf %d")
TRACE_MSG(MAC_3055_112_2_18_0_12_10_1449,"MAC: Ul release, send ctrl ack request new TBF!")
TRACE_MSG(MAC_3066_112_2_18_0_12_10_1450,"MAC: Ul release, send res req, request new TBF!")
TRACE_MSG(MAC_3071_112_2_18_0_12_10_1451,"MAC: Ul release, unexpected ul_exp_action!!")
TRACE_MSG(MAC_3159_112_2_18_0_12_10_1452,"MAC: receive ul_rel_req but ul not active")
TRACE_MSG(MAC_3226_112_2_18_0_12_10_1453,"MAC:Assert Failure,tbf mode invalid in rlc_mac_rel_req")
TRACE_MSG(MAC_3332_112_2_18_0_12_10_1454,"MAC:Assert Failure,release both link tbf but not in rel state")
TRACE_MSG(MAC_3360_112_2_18_0_12_10_1455,"MAC:tbf mode invalid,uplink tbf starting time?")
TRACE_MSG(MAC_3485_112_2_18_0_12_11_1456,"MAC:rlc_mac_tbf_req in ul_active!!")
TRACE_MSG(MAC_3494_112_2_18_0_12_11_1457,"MAC:ignore rlc tbf req when prach have been send but not receive ass!")
TRACE_MSG(MAC_3600_112_2_18_0_12_11_1458,"MAC:rlc realloc req but ul not active")
TRACE_MSG(MAC_3706_112_2_18_0_12_11_1459,"MAC: wrong state 3186 expire")
TRACE_MSG(MAC_3717_112_2_18_0_12_11_1460,"MAC: T3170 expiry! T3170 Dur: %d \n")
TRACE_MSG(MAC_3814_112_2_18_0_12_11_1461,"MAC: wrong state t3170 expire")
TRACE_MSG(MAC_3880_112_2_18_0_12_11_1462,"MAC: T3168 expiry! T3168 Dur: %d\n")
TRACE_MSG(MAC_3996_112_2_18_0_12_12_1463,"MAC: wrong state rev t3168 expire")
TRACE_MSG(MAC_4113_112_2_18_0_12_12_1464,"MAC: wrong state rev t3164 expire")
TRACE_MSG(MAC_4191_112_2_18_0_12_12_1465,"MAC: wrong state rev t3162 expire")
TRACE_MSG(MAC_4208_112_2_18_0_12_12_1466,"MAC: wrong state rev t3180 expire")
TRACE_MSG(MAC_4287_112_2_18_0_12_12_1467,"MAC: T3192 expiry! T3192 Dur: %d\n")
TRACE_MSG(MAC_4578_112_2_18_0_12_13_1468,"MAC: Prach access not permitted!")
TRACE_MSG(MAC_4617_112_2_18_0_12_13_1469,"MAC: IA allocation uplink 8psk, but MS not support")
TRACE_MSG(MAC_4772_112_2_18_0_12_13_1470,"MAC: IA assignment egprs, but MS not support")
TRACE_MSG(MAC_4884_112_2_18_0_12_13_1471,"MAC: Single block without TBF in EGPRS!\n")
TRACE_MSG(MAC_5263_112_2_18_0_12_14_1472,"MAC: Mac mode mismatch! Ignore packet ul assi!")
TRACE_MSG(MAC_5288_112_2_18_0_12_14_1473,"MAC: rev ul assi in ul idle or other invalid state")
TRACE_MSG(MAC_5301_112_2_18_0_12_14_1474,"MAC: Mac mode mismatch! Ignore packet ul assi!\n")
TRACE_MSG(MAC_5382_112_2_18_0_12_14_1475,"MAC: one_phase_access_error!!")
TRACE_MSG(MAC_5400_112_2_18_0_12_14_1476,"MAC: 1ph pkt ul ass allocation uplink 8psk, but MS not support")
TRACE_MSG(MAC_5462_112_2_18_0_12_14_1477,"MAC: Decode multiblock allocation error! Rach retry!\n")
TRACE_MSG(MAC_5712_112_2_18_0_12_15_1478,"MAC: Contention failure!\n")
TRACE_MSG(MAC_5839_112_2_18_0_12_15_1479,"MAC: EGPRS invalid one phase state when rev ul assi")
TRACE_MSG(MAC_5845_112_2_18_0_12_15_1480,"MAC: Decode Dynamic allocation error! Rach retry!\n")
TRACE_MSG(MAC_5852_112_2_18_0_12_15_1481,"MAC: fixed allocation error! Rach retry!\n")
TRACE_MSG(MAC_5861_112_2_18_0_12_15_1482,"MAC: 1ph pkt ul ass assignment egprs, but MS not support")
TRACE_MSG(MAC_5886_112_2_18_0_12_15_1483,"MAC: Frequency not in band! Rach retry!\n")
TRACE_MSG(MAC_5971_112_2_18_0_12_15_1484,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_5976_112_2_18_0_12_15_1485,"MAC: decode freq error return invalid value")
TRACE_MSG(MAC_6001_112_2_18_0_12_16_1486,"MAC: 2ph allocation uplink 8psk, but MS not support")
TRACE_MSG(MAC_6021_112_2_18_0_12_16_1487,"MAC: wrong assignment in second phase access")
TRACE_MSG(MAC_6246_112_2_18_0_12_16_1488,"MAC: Decode allocation error! Rach retry!\n")
TRACE_MSG(MAC_6253_112_2_18_0_12_16_1489,"MAC: Ingore the egprs ul assi in egprs_ul_assi_two_phase")
TRACE_MSG(MAC_6261_112_2_18_0_12_16_1490,"MAC: 2ph assignment egprs, but MS not support")
TRACE_MSG(MAC_6291_112_2_18_0_12_16_1491,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_6355_112_2_18_0_12_16_1492,"MAC: Frequency not in band! Rach retry!\n")
TRACE_MSG(MAC_6361_112_2_18_0_12_16_1493,"MAC: decode freq error return invalid value")
TRACE_MSG(MAC_6371_112_2_18_0_12_16_1494,"MAC: false tbf mode")
TRACE_MSG(MAC_6385_112_2_18_0_12_16_1495,"MAC: ul active allocation uplink 8psk, but MS not support")
TRACE_MSG(MAC_6394_112_2_18_0_12_16_1496,"MAC: false ul assignment in LABEL_UL_ASSI_UL_ACTIVE")
TRACE_MSG(MAC_6556_112_2_18_0_12_17_1497,"MAC: Decode allocation error in LABEL_UL_ASSI_UL_ACTIVE! Rach retry!\n")
TRACE_MSG(MAC_6565_112_2_18_0_12_17_1498,"MAC: Ingore the ul assi msg beacause lose dynamic allo in LABEL_UL_ASSI_UL_ACTIVE!")
TRACE_MSG(MAC_6573_112_2_18_0_12_17_1499,"MAC: ul active assignment egprs, but MS not support")
TRACE_MSG(MAC_6583_112_2_18_0_12_17_1500,"MAC: rev single block assi when ul active")
TRACE_MSG(MAC_6638_112_2_18_0_12_17_1501,"MAC: DL est when waiting L1 release cnf")
TRACE_MSG(MAC_6661_112_2_18_0_12_17_1502,"MAC:Ignore packet dl assi when two phase or contention phase!")
TRACE_MSG(MAC_6671_112_2_18_0_12_17_1503,"MAC: rev dl assi when ul access two phase or other state")
TRACE_MSG(MAC_6696_112_2_18_0_12_17_1504,"MAC: in ul access recv pkt dl ass alloc no dl slot! Ignore the msg!\n")
TRACE_MSG(MAC_6709_112_2_18_0_12_17_1505,"MAC: wrong state rev dl assi,ignore!")
TRACE_MSG(MAC_6720_112_2_18_0_12_17_1506,"MAC:  dl assign DL_TBF is different with UL_TBF in LABEL_DL_ASSI_UL_ACTIVE! Ignore the msg!\n")
TRACE_MSG(MAC_6735_112_2_18_0_12_17_1507,"MAC: Ignore without dl_tfi dl assign msg while T3192 run!\n")
TRACE_MSG(MAC_6747_112_2_18_0_12_17_1508,"MAC: in dl active recv pkt dl ass alloc no dl slot! Ignore the msg!\n")
TRACE_MSG(MAC_6761_112_2_18_0_12_17_1509,"MAC: currnet dl tbf is EGPRS,receive gprs downlink ass,ignore!\n")
TRACE_MSG(MAC_6768_112_2_18_0_12_17_1510,"MAC: Rlc mode mismatch! Ignore dl ass msg in dl_active!")
TRACE_MSG(MAC_6829_112_2_18_0_12_17_1511,"MAC:contention phase, ignore TS RECONFIG")
TRACE_MSG(MAC_6847_112_2_18_0_12_17_1512,"MAC: error state to receive ts reconfig")
TRACE_MSG(MAC_6865_112_2_18_0_12_17_1513,"MAC: Timeslot reconfig assigned  tbf mode confilit with current ul tbf mode!ignor it!\n")
TRACE_MSG(MAC_6877_112_2_18_0_12_17_1514,"MAC: e_ch_coding_command in ts recofig_ul_active err, ignore it!")
TRACE_MSG(MAC_6893_112_2_18_0_12_17_1515,"MAC: in ul active recv pkt ts reconfig allocation no dl slot! Rach retry!\n")
TRACE_MSG(MAC_6944_112_2_18_0_12_17_1516,"MAC: Decode error! Acquare Sys Info\n")
TRACE_MSG(MAC_6949_112_2_18_0_12_17_1517,"MAC: Decode freq error! UL release and Rach retry!\n")
TRACE_MSG(MAC_6969_112_2_18_0_12_17_1518,"MAC: Decode allocation error! UL release and Rach retry!\n")
TRACE_MSG(MAC_7205_112_2_18_0_12_18_1519,"MAC: Timeslot reconfig not include dl tfi! Ul release and Rach retry!\n")
TRACE_MSG(MAC_7240_112_2_18_0_12_18_1520,"MAC: in dl active recv pkt ts reconfig allocation no dl slot! Rach retry!\n")
TRACE_MSG(MAC_7335_112_2_18_0_12_18_1521,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_7340_112_2_18_0_12_18_1522,"MAC: wrong state when rev ts realloc assi")
TRACE_MSG(MAC_7346_112_2_18_0_12_18_1523,"MAC: Decode freq error! Rach retry!\n")
TRACE_MSG(MAC_7358_112_2_18_0_12_18_1524,"MAC: Timeslot reconfig assigned  tbf mode confilit with current dl tbf mode!ignor it!\n")
TRACE_MSG(MAC_7370_112_2_18_0_12_18_1525,"MAC: e_ch_coding_command in ts recofig_dl_active err, ignore it!")
TRACE_MSG(MAC_7395_112_2_18_0_12_18_1526,"MAC: Decode allocation error! Rach retry!")
TRACE_MSG(MAC_7729_112_2_18_0_12_19_1527,"MAC: Timeslot reconfig not include dl tfi! Rach retry!\n")
TRACE_MSG(MAC_7739_112_2_18_0_12_19_1528,"MAC: Rlc mode mismatch! Ignore ts reconfig msg in dl_active!")
TRACE_MSG(MAC_7789_112_2_18_0_12_19_1529,"MAC: Timeslot reconfig assigned  tbf mode confilit with current dl_ul tbf mode!ignor it!\n")
TRACE_MSG(MAC_7801_112_2_18_0_12_19_1530,"MAC: e_ch_coding_command in ts recofig_ul_dl_active err, ignore it!")
TRACE_MSG(MAC_7820_112_2_18_0_12_19_1531,"MAC: in ul dl active recv pkt ts reconfig allocation no dl slot! Rach retry!\n")
TRACE_MSG(MAC_7869_112_2_18_0_12_19_1532,"MAC: Decode error! Acquare Sys Info\n")
TRACE_MSG(MAC_7874_112_2_18_0_12_19_1533,"MAC: Decode freq error! UL release and Rach retry!\n")
TRACE_MSG(MAC_7894_112_2_18_0_12_19_1534,"MAC: Decode allocation error! Rach retry!")
TRACE_MSG(MAC_8161_112_2_18_0_12_20_1535,"MAC: Rlc mode mismatch! Ignore ts reconfig msg in ul_dl_active!")
TRACE_MSG(MAC_8171_112_2_18_0_12_20_1536,"MAC: EGPRS UL ACK_NACK assigned tbf mode conflict with currnt ul tbf mode, ignore it")
TRACE_MSG(MAC_8184_112_2_18_0_12_20_1537,"MAC: e_ch_coding_command in EGPRS UL ACK_NACK err, ignore it!")
TRACE_MSG(MAC_8427_112_2_18_0_12_21_1538,"MAC: wrong state rev ul ack")
TRACE_MSG(MAC_8453_112_2_18_0_12_21_1539,"MAC: Ignore additional PACKET ACCESS REJECT messages!")
TRACE_MSG(MAC_8758_112_2_18_0_12_22_1540,"MAC: Receive reject, UL release!\n")
TRACE_MSG(MAC_8812_112_2_18_0_12_22_1541,"MAC: ignore PACKET_QUEUING_NOTIFICATION while waiting TBF Starting Time of ul ass!")
TRACE_MSG(MAC_9081_112_2_18_0_12_23_1542,"MAC: wrong ul state recv Packet tbf realse msg!")
TRACE_MSG(MAC_9250_112_2_18_0_12_23_1543,"MAC: recv PPR no affect")
TRACE_MSG(MAC_9278_112_2_18_0_12_23_1544,"MAC: wrong PPR")
TRACE_MSG(MAC_9678_112_2_18_0_12_24_1545,"MAC: wrong state rev t3166 expire")
TRACE_MSG(MAC_9684_112_2_18_0_12_24_1546,"MAC: One phase received egprs ul assi£¬but we don't support EDGE!")
TRACE_MSG(MAC_9928_112_2_18_0_12_25_1547,"MAC: Abnormal release with retry!")
TRACE_MSG(MAC_9952_112_2_18_0_12_25_1548,"MAC: !!Impossible ul_release_state!!")
TRACE_MSG(MAC_9995_112_2_18_0_12_25_1549,"MAC: Abnormal release with rach retry!")
TRACE_MSG(MAC_10007_112_2_18_0_12_25_1550,"MAC: release_cnf for ul but ul not active,maybe hold for connect cnf")
TRACE_MSG(MAC_10066_112_2_18_0_12_25_1551,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_10078_112_2_18_0_12_25_1552,"MAC: Frequency not in band! Rach retry!\n")
TRACE_MSG(MAC_10084_112_2_18_0_12_25_1553,"MAC: decode freq error,invalid return value")
TRACE_MSG(MAC_10107_112_2_18_0_12_25_1554,"MAC: wrong state when rev single block assi")
TRACE_MSG(MAC_10279_112_2_18_0_12_26_1555,"MAC: Decode allocation error! Rach retry!")
TRACE_MSG(MAC_10286_112_2_18_0_12_26_1556,"MAC: Ingore the ul assi in UL_ASSI_TWO_PHASE_ACCESS")
TRACE_MSG(MAC_10365_112_2_18_0_12_26_1557,"MAC: Decode freq error! Acquare Sys Info")
TRACE_MSG(MAC_10376_112_2_18_0_12_26_1558,"MAC: wrong state when rev dl assi")
TRACE_MSG(MAC_10382_112_2_18_0_12_26_1559,"MAC: Decode freq error! Rach retry!")
TRACE_MSG(MAC_10626_112_2_18_0_12_26_1560,"MAC: No tfi assign in dl assi! Rach retry!\n")
TRACE_MSG(MAC_10633_112_2_18_0_12_26_1561,"MAC: Multislot capability not match! Rach retry!")
TRACE_MSG(MAC_10644_112_2_18_0_12_26_1562,"MAC: in ul active recv pkt dl ass alloc no dl slot! Ignore the msg!\n")
TRACE_MSG(MAC_10697_112_2_18_0_12_26_1563,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_10702_112_2_18_0_12_26_1564,"MAC: Decode freq error! Ignore the msg!")
TRACE_MSG(MAC_10901_112_2_18_0_12_27_1565,"MAC: Dl assi not include tfi!")
TRACE_MSG(MAC_10907_112_2_18_0_12_27_1566,"MAC: Multislot capability not match! Ignore the msg!\n")
TRACE_MSG(MAC_10980_112_2_18_0_12_27_1567,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_11269_112_2_18_0_12_27_1568,"MAC: Decode error! acquare si!\n")
TRACE_MSG(MAC_11281_112_2_18_0_12_27_1569,"MAC: Multislot capability not match! Ignore the msg!")
TRACE_MSG(MAC_11289_112_2_18_0_12_28_1570,"MAC: Contention failure!")
TRACE_MSG(MAC_11514_112_2_18_0_12_28_1571,"MAC: Single block without TBF!\n")
TRACE_MSG(MAC_11643_112_2_18_0_12_28_1572,"MAC: invalid one phase state when rev single block assi")
TRACE_MSG(MAC_11857_112_2_18_0_12_29_1573,"MAC: invalid one phase state when rev ul assi")
TRACE_MSG(MAC_11863_112_2_18_0_12_29_1574,"MAC: Decode allocation error! Rach retry!\n")
TRACE_MSG(MAC_11870_112_2_18_0_12_29_1575,"MAC: fixed allocation, rach retry")
TRACE_MSG(PROCESSMM_1945_112_2_18_0_12_33_1576,"MM: read sim data error")
TRACE_MSG(PROCESSMM_2204_112_2_18_0_12_33_1577,"MM:begin saved call")
TRACE_MSG(PROCESSMM_2320_112_2_18_0_12_33_1578,"MM:begin saved call")
TRACE_MSG(PROCESSMM_2814_112_2_18_0_12_34_1579,"MM: GSM service detach\n")
TRACE_MSG(PROCESSMM_2834_112_2_18_0_12_34_1580,"MM_CALI: MN_MM_NREG_REQ in WAIT FOR MN REG\n")
TRACE_MSG(PROCESSMM_3122_112_2_18_0_12_35_1581,"MM:begin saved call")
TRACE_MSG(PROCESSMM_4819_112_2_18_0_12_38_1582,"MM:receive MM_PD MSG at WAIT_FOR_NETWORK_COMMAND\n")
TRACE_MSG(PROCESSMM_7123_112_2_18_0_12_43_1583,"MM:begin saved call")
TRACE_MSG(PROCESSMM_7752_112_2_18_0_12_44_1584,"MM:begin saved call")
TRACE_MSG(PROCESSMM_9609_112_2_18_0_12_48_1585,"MM:begin saved call")
TRACE_MSG(PROCESSMM_11333_112_2_18_0_12_51_1586,"MM_CALI: MN_MM_NREG_REQ in WAIT_FOR_RR_CONNECTION_MM\n")
TRACE_MSG(PROCESSMM_11668_112_2_18_0_12_52_1587,"MM_CALI: MN_MM_NREG_REQ in other status, with mm connect\n")
TRACE_MSG(PROCESSMM_12185_112_2_18_0_12_53_1588,"MM_CALI: MN_MM_NREG_REQ in MM_NO_CELL")
TRACE_MSG(PROCESSMM_12251_112_2_18_0_12_53_1589,"MM_CALI: MN_MM_NREG_REQ in MM_NO_SIM")
TRACE_MSG(PROCESSMM_13009_112_2_18_0_12_55_1590,"MM:begin saved call")
TRACE_MSG(PROCESSMM_13068_112_2_18_0_12_55_1591,"MM: sim authentication error")
TRACE_MSG(PROCESSMM_13139_112_2_18_0_12_55_1592,"MM:begin saved call")
TRACE_MSG(PROCESSMM_13659_112_2_18_0_12_56_1593,"MM:begin saved call")
TRACE_MSG(PROCESSMM_13961_112_2_18_0_12_57_1594,"MM:begin saved call")
TRACE_MSG(PROCESSMM_14075_112_2_18_0_12_57_1595,"MM:begin saved call")
TRACE_MSG(PROCESSMM_14094_112_2_18_0_12_57_1596,"MM:begin saved call")
TRACE_MSG(PROCESSMM_14213_112_2_18_0_12_57_1597,"MM:begin saved call")
TRACE_MSG(PROCESSMM_14832_112_2_18_0_12_58_1598,"MM:discard MM_RR_DATA_IND")
TRACE_MSG(PROCESSMM_15159_112_2_18_0_12_59_1599,"MM_CALI: MN_MM_NREG_REQ in other status, with mm connect\n")
TRACE_MSG(PROCESSMM_15260_112_2_18_0_12_59_1600,"MM_CALI: MN_MM_NREG_REQ in other status, without mm connect\n")
TRACE_MSG(PROCESSMM_15377_112_2_18_0_12_59_1601,"MM:begin saved call")
TRACE_MSG(PROCESSMM_15843_112_2_18_0_13_0_1602,"MM: current T3212 duration is %d")
TRACE_MSG(PROCESSMM_16739_112_2_18_0_13_2_1603,"MM: There is a delay CM service\n")
TRACE_MSG(RLC_733_112_2_18_0_13_6_1604,"rlc:remain llc tbf request!")
TRACE_MSG(RLC_750_112_2_18_0_13_6_1605,"rlc:cell reselect tbf!")
TRACE_MSG(RLC_969_112_2_18_0_13_6_1606,"rlc:Up Single Block Access!\n")
TRACE_MSG(RLC_1108_112_2_18_0_13_7_1607,"rlc:Up TBF Access for EGPRS SCT!\n")
TRACE_MSG(RLC_1115_112_2_18_0_13_7_1608,"RLC: SCT est fail, downlink tbf is not acitved!")
TRACE_MSG(RLC_1955_112_2_18_0_13_8_1609,"RLC: recv data error in loop back mode")
TRACE_MSG(RLC_1990_112_2_18_0_13_8_1610,"rlc:error data_ind when dl is not actived!")
TRACE_MSG(RLC_2089_112_2_18_0_13_9_1611,"rlc:SCT end!\n")
TRACE_MSG(RLC_2276_112_2_18_0_13_9_1612,"rlc:should not recv up release")
TRACE_MSG(RLC_2506_112_2_18_0_13_10_1613,"rlc:Access Fail with CS Page!\n")
TRACE_MSG(RLC_2908_112_2_18_0_13_11_1614,"rlc:Up tbf fail with dl tbf is esting!\n")
TRACE_MSG(RLC_2912_112_2_18_0_13_11_1615,"RLC: est sigle block without tbf interrupt by dl tbf ass")
TRACE_MSG(RLC_2955_112_2_18_0_13_11_1616,"rlc:Up tbf ok for single block!\n")
TRACE_MSG(RLC_2982_112_2_18_0_13_11_1617,"rlc:IS Rejected Access!\n")
TRACE_MSG(RLC_3004_112_2_18_0_13_11_1618,"RLC:SCT Test Mode B Ested!")
TRACE_MSG(RLC_3011_112_2_18_0_13_11_1619,"RLC:SCT Test Mode A Ested!")
TRACE_MSG(RLC_3033_112_2_18_0_13_11_1620,"RLC:EGPRS SCT Test  Ested!")
TRACE_MSG(RLC_3228_112_2_18_0_13_11_1621,"rlc:mac retry,but no need!\n")
TRACE_MSG(RLC_3262_112_2_18_0_13_12_1622,"rlc:Up tbf fail with t3172 in mac ul_active!\n")
TRACE_MSG(RLC_3340_112_2_18_0_13_12_1623,"rlc:error normal release")
TRACE_MSG(RLC_3529_112_2_18_0_13_12_1624,"rlc:Up fail and perform cell reselect!\n")
TRACE_MSG(RLC_3580_112_2_18_0_13_12_1625,"rlc:Up tbf fail with t3172 in mac ul_active!\n")
TRACE_MSG(RLC_3664_112_2_18_0_13_12_1626,"rlc:Up tbf fail with timer t3172!\n")
TRACE_MSG(RLC_3730_112_2_18_0_13_12_1627,"rlc:Receive Second mac_rlc_tbf_cnf msg!")
TRACE_MSG(RLC_3745_112_2_18_0_13_12_1628,"rlc:Acess state is not one or two phase!")
TRACE_MSG(RRC_3853_112_2_18_0_13_29_1629,"RRC: unknown bsic cnf")
TRACE_MSG(RRC_3920_112_2_18_0_13_29_1630,"RRC: ASSERT, research>6")
TRACE_MSG(RRC_3939_112_2_18_0_13_29_1631,"RRC: unknown bsic fail")
TRACE_MSG(RRC_6831_112_2_18_0_13_35_1632,"RRC: discard IA,because mm est saved")
TRACE_MSG(RRC_6839_112_2_18_0_13_35_1633,"RRC: discard IA,because plmn list req")
TRACE_MSG(RRC_9493_112_2_18_0_13_40_1634,"RR:receive_psi_resel Get Packet paging!!\n")
TRACE_MSG(RRC_9883_112_2_18_0_13_41_1635,"RRC: assert error, not fastplmn")
TRACE_MSG(RRC_10022_112_2_18_0_13_41_1636,"RRC: assert error, not fastplmn")
TRACE_MSG(RRC_10491_112_2_18_0_13_42_1637,"RRC: unknown bsic cnf")
TRACE_MSG(RRC_10570_112_2_18_0_13_43_1638,"RRC: unknown bsic fail")
TRACE_MSG(RRC_11106_112_2_18_0_13_44_1639,"RR: discard immed_assig")
TRACE_MSG(RRC_11139_112_2_18_0_13_44_1640,"RR: rx immediate rej")
TRACE_MSG(RRC_11942_112_2_18_0_13_45_1641,"RRC wrong sapi %d")
TRACE_MSG(RRC_12766_112_2_18_0_13_47_1642,"RRC: wait assign dcch est error in dl chan type")
TRACE_MSG(RRC_13526_112_2_18_0_13_48_1643,"RRC wrong sapi %d")
TRACE_MSG(RRC_16598_112_2_18_0_13_54_1644,"RRC: Receive data block on PPCH, discard it!")
TRACE_MSG(RRC_17409_112_2_18_0_13_56_1645,"RRC: Decode PacketMeasureOrder err!")
TRACE_MSG(RRC_17587_112_2_18_0_13_56_1646,"RRC: Packet cell change failure msg is sent, failure cause=%d")
TRACE_MSG(RRC_17777_112_2_18_0_13_57_1647,"RRC: Decode PacketCellChangeOrder err!")
TRACE_MSG(RRC_18099_112_2_18_0_13_57_1648,"RRC: Decode Pmo err!")
TRACE_MSG(RRC_18955_112_2_18_0_13_59_1649,"RRC: ASSERT ERROR recv prach when bad")
TRACE_MSG(RRC_21843_112_2_18_0_14_5_1650,"RRC: discard IA,because plmn list req")
TRACE_MSG(RRC_22060_112_2_18_0_14_6_1651,"RRC: Receive data block on PPCH, discard it!")
TRACE_MSG(RRC_22222_112_2_18_0_14_6_1652,"RRC: Decode pmo err!")
TRACE_MSG(RRC_22300_112_2_18_0_14_6_1653,"RRC: find ncell fail")
TRACE_MSG(RRC_22312_112_2_18_0_14_6_1654,"RRC: gmm state not ready")
TRACE_MSG(RRC_22320_112_2_18_0_14_6_1655,"RRC: decode pcco fail")
TRACE_MSG(RRC_23993_112_2_18_0_14_10_1656,"RRC: process saved ppch")
TRACE_MSG(RRC_24082_112_2_18_0_14_10_1657,"RRC: proc saved pch ")
TRACE_MSG(RRC_24763_112_2_18_0_14_11_1658,"RRC: resel scell arfcn 0x%x")
TRACE_MSG(RRC_24996_112_2_18_0_14_12_1659,"RRC: cell resel arfcn 0x%x")
TRACE_MSG(RRC_25249_112_2_18_0_14_12_1660," RRC: switch to another cell, clear PSI Msg Buffer \n")
TRACE_MSG(RRC_25688_112_2_18_0_14_13_1661,"RRC: unknown bsic cnf")
TRACE_MSG(RRC_26466_112_2_18_0_14_15_1662,"RR temp = %d")
TRACE_MSG(RRC_28152_112_2_18_0_14_18_1663,"RRC: Decode Pmo err!")
TRACE_MSG(RRC_28222_112_2_18_0_14_18_1664,"RRC: Receive data block on PPCH, discard it!")
TRACE_MSG(SMENTITY_208_112_2_18_0_14_24_1665,"SM_GetSysId dual_sys = %d")
TRACE_MSG(SMENTITY_1577_112_2_18_0_14_27_1666,"SMentity:sapi not match.")
TRACE_MSG(SMENTITY_1654_112_2_18_0_14_27_1667,"SMentity:wrong release_type:%d.")
TRACE_MSG(SMENTITY_1750_112_2_18_0_14_27_1668,"SMentity:Decode SUCCESS!\n")
TRACE_MSG(SMENTITY_1891_112_2_18_0_14_28_1669,"SMentity:Decode SUCCESS!\n")
TRACE_MSG(SMENTITY_1924_112_2_18_0_14_28_1670,"SMentity:wrong message!")
TRACE_MSG(SMENTITY_1929_112_2_18_0_14_28_1671,"SMentity:Decode Mtmodifypdpreq/Reqpdpact error!\n")
TRACE_MSG(SMENTITY_2091_112_2_18_0_14_28_1672,"SMentity:wrong release_type:%d.")
TRACE_MSG(SMENTITY_2178_112_2_18_0_14_28_1673,"SMentity:sapi not match.")
TRACE_MSG(SMENTITY_2369_112_2_18_0_14_29_1674,"SMentity:sapi not match.")
TRACE_MSG(SMENTITY_2564_112_2_18_0_14_29_1675,"SMentity:wrong state mesage!!")
TRACE_MSG(SMENTITY_3003_112_2_18_0_14_30_1676,"SMentity:sapi not match.")
TRACE_MSG(SMMAIN_556_112_2_18_0_14_33_1677,"SM: same sys, discard init.")
TRACE_MSG(SMMAIN_773_112_2_18_0_14_33_1678,"SMmain:discard sm msg")
TRACE_MSG(SMMAIN_840_112_2_18_0_14_34_1679,"SMmain:Decode msg reqpdpact error!\n")
TRACE_MSG(SMMAIN_909_112_2_18_0_14_34_1680,"SMmain:Decode msg header error!\n")
TRACE_MSG(SMMAIN_1565_112_2_18_0_14_35_1681,"SMmain:discard sm msg")
TRACE_MSG(SMMAIN_2345_112_2_18_0_14_37_1682,"SMmain:SM do not send SNSM_SEQUENCE_IND to sndcp ")
TRACE_MSG(SMMAIN_2581_112_2_18_0_14_37_1683,"smmain: received unexpect me status.")
TRACE_MSG(SMMAIN_2926_112_2_18_0_14_38_1684,"smmain: received unexpect me status.")
TRACE_MSG(SMMAIN_3753_112_2_18_0_14_40_1685,"SMmain:Decode msg reqpdpact error!\n")
TRACE_MSG(SNDCPENTITY_843_112_2_18_0_14_49_1686,"SNDCP: received release_ind with RELEASE_CAUSE_NORMAL_RELEASE on esting state")
TRACE_MSG(SNDCPENTITY_872_112_2_18_0_14_49_1687,"SNDCP:received not defined cause.\n")
TRACE_MSG(SNDCPENTITY_1329_112_2_18_0_14_50_1688,"SNDCP:Discard NPDU on invalid state!")
TRACE_MSG(SNDCPENTITY_1351_112_2_18_0_14_50_1689,"SNDCP:Discard NPDU when reaching on the max buffer_size!")
TRACE_MSG(SNDCPENTITY_1426_112_2_18_0_14_50_1690,"SNDCP:Discard NPDU on invalid state!")
TRACE_MSG(SNDCPMAIN_281_112_2_18_0_14_55_1691,"SNDCP:NSAPI already existed.")
TRACE_MSG(SNDCPMAIN_372_112_2_18_0_14_55_1692,"SNDCP:NSAPI doesnt exist.")
TRACE_MSG(SNDCPMAIN_632_112_2_18_0_14_55_1693,"SNDCP:SN_UNITDATA_REQ - Null pointer")
TRACE_MSG(SNDCPMAIN_691_112_2_18_0_14_55_1694,"SNDCP:SN_DATA_REQ - Null pointer")
TRACE_MSG(SNDCPMAIN_1311_112_2_18_0_14_57_1695,"SNDCP:LL_SNDC_UNITDATA_IND - wrong nsapi or sapi")
TRACE_MSG(SNDCPMAIN_1316_112_2_18_0_14_57_1696,"SNDCP:LL_SNDC_UNITDATA_IND - Null pointer")
TRACE_MSG(SNDCPMAIN_1379_112_2_18_0_14_57_1697,"SNDCP:LL_SNDC_DATA_IND - wrong nsapi")
TRACE_MSG(SNDCPMAIN_1384_112_2_18_0_14_57_1698,"SNDCP:LL_SNDC_DATA_IND - Null pointer")
TRACE_MSG(DLR_296_112_2_18_0_15_14_1699,"DL: release req 0,but no entity")
TRACE_MSG(DLR_320_112_2_18_0_15_14_1700,"DL: release req 3,but no entity")
TRACE_MSG(DLR_480_112_2_18_0_15_15_1701,"L2: unexpected sapi\n")
TRACE_MSG(DLR_798_112_2_18_0_15_15_1702,"PH_READY_TO_SEND_IND recved, but task released\n")
TRACE_MSG(LAYER1_659_112_2_18_0_16_1_1703,"deactive req.\n")
TRACE_MSG(LAYER1_732_112_2_18_0_16_1_1704,"rxpower_sweep_req asked and confirm replied.\n")
TRACE_MSG(LAYER1_750_112_2_18_0_16_1_1705,"rxpower from BS come.\n")
TRACE_MSG(LAYER1_768_112_2_18_0_16_1_1706,"bcch req asked.\n")
TRACE_MSG(LAYER1_779_112_2_18_0_16_1_1707,"the bcch req is single %d.\n")
TRACE_MSG(LAYER1_801_112_2_18_0_16_1_1708,"bcch req stop.\n")
TRACE_MSG(LAYER1_818_112_2_18_0_16_1_1709,"bcch timer expired.\n")
TRACE_MSG(LAYER1_853_112_2_18_0_16_1_1710,"bsic req asked.\n")
TRACE_MSG(LAYER1_929_112_2_18_0_16_1_1711,"\n*****Layer1 Rx BCCH:*****\n")
TRACE_MSG(LAYER1_930_112_2_18_0_16_1_1712,"BCCH_ARFCN:       %d \n\n")
TRACE_MSG(LAYER1_971_112_2_18_0_16_1_1713,"\n*****L1Sim Rx BCCH:*****\n")
TRACE_MSG(LAYER1_972_112_2_18_0_16_1_1714,"BCCH_ARFCN:%d \n\n")
TRACE_MSG(LAYER1_1015_112_2_18_0_16_1_1715,"bcch ind true\n")
TRACE_MSG(LAYER1_1176_112_2_18_0_16_2_1716,"campon req&confirmed.\n")
TRACE_MSG(LAYER1_1274_112_2_18_0_16_2_1717,"TBF Downlink Assignment Recv\n")
TRACE_MSG(LAYER1_1347_112_2_18_0_16_2_1718,"Send the first RA\n")
TRACE_MSG(LAYER1_1421_112_2_18_0_16_2_1719,"L1 Send Random Acess Burst\n")
TRACE_MSG(LAYER1_1475_112_2_18_0_16_2_1720,"AGCH recv\n")
TRACE_MSG(LAYER1_1747_112_2_18_0_16_3_1721,"PLMN search asked and  rxpower confirm replied.\n")
TRACE_MSG(LAYER1_2303_112_2_18_0_16_4_1722,"chan_assign:%d\n")
TRACE_MSG(LAYER1_2345_112_2_18_0_16_4_1723,"chan_assign_fail:%d\n")
TRACE_MSG(LAYER1_2428_112_2_18_0_16_4_1724,"handover_fail:%d\n")
TRACE_MSG(LAYER1_2542_112_2_18_0_16_4_1725,"Received Data block from new DCCH in non-syn handover\n")
TRACE_MSG(LAYER1_2551_112_2_18_0_16_5_1726,"handover :%d\n")
TRACE_MSG(LAYER1_2626_112_2_18_0_16_5_1727,"handover_fail:%d\n")
TRACE_MSG(LAYER1_2675_112_2_18_0_16_5_1728,"L1:sacch data recv\n")
TRACE_MSG(LAYER1_2697_112_2_18_0_16_5_1729,"l1 recv data:%d\n")
TRACE_MSG(LAYER1_2777_112_2_18_0_16_5_1730,"L1:data send at chan %d\n")
TRACE_MSG(LAYER1_2799_112_2_18_0_16_5_1731,"L1:sacch data send\n")
TRACE_MSG(LAYER1_2993_112_2_18_0_16_5_1732,"This bcch block is unexpected\n")
TRACE_MSG(LAYER1_3031_112_2_18_0_16_5_1733,"bcch ind,true\n")
TRACE_MSG(LAYER1_3038_112_2_18_0_16_5_1734,"bcch ind,not requested\n")
TRACE_MSG(LAYER1_3151_112_2_18_0_16_6_1735,"SI_ID: 1")
TRACE_MSG(LAYER1_3155_112_2_18_0_16_6_1736,"SI_ID: 2")
TRACE_MSG(LAYER1_3159_112_2_18_0_16_6_1737,"SI_ID: 2Bis")
TRACE_MSG(LAYER1_3163_112_2_18_0_16_6_1738,"SI_ID: 2Ter")
TRACE_MSG(LAYER1_3167_112_2_18_0_16_6_1739,"SI_ID: 3")
TRACE_MSG(LAYER1_3171_112_2_18_0_16_6_1740,"SI_ID: 4")
TRACE_MSG(LAYER1_3175_112_2_18_0_16_6_1741,"SI_ID: 13")
TRACE_MSG(LAYER1_3179_112_2_18_0_16_6_1742,"SI_ID: 5BIS")
TRACE_MSG(LAYER1_3183_112_2_18_0_16_6_1743,"SI_ID: 5BIS")
TRACE_MSG(LAYER1_3187_112_2_18_0_16_6_1744,"SI_ID: 6")
TRACE_MSG(LAYER1_3191_112_2_18_0_16_6_1745,"SI_ID: 7")
TRACE_MSG(LAYER1_3195_112_2_18_0_16_6_1746,"PSI_ID: 13")
TRACE_MSG(LAYER1_3199_112_2_18_0_16_6_1747,"SI_ID: 4")
TRACE_MSG(LAYER1_3203_112_2_18_0_16_6_1748,"PSI_ID: 3Bis")
TRACE_MSG(LAYER1_3207_112_2_18_0_16_6_1749,"SI_ID: 8")
TRACE_MSG(LAYER1_3211_112_2_18_0_16_6_1750,"SI_ID: 9")
TRACE_MSG(LAYER1_3215_112_2_18_0_16_6_1751,"SI_ID: 13")
TRACE_MSG(LAYER1_3219_112_2_18_0_16_6_1752,"PSI_ID: 1")
TRACE_MSG(LAYER1_3223_112_2_18_0_16_6_1753,"PSI_ID: 2")
TRACE_MSG(LAYER1_3227_112_2_18_0_16_6_1754,"PSI_ID: 3 ")
TRACE_MSG(LLENTITY_900_112_2_18_0_16_8_1755,"LLentity:impossible U frame type\n")
TRACE_MSG(LLENTITY_1101_112_2_18_0_16_8_1756,"LLentity:impossible I frame type\n")
TRACE_MSG(LLENTITY_1210_112_2_18_0_16_8_1757,"LLentity:impossible S frame type\n")
TRACE_MSG(LLENTITY_1317_112_2_18_0_16_9_1758,"LLentity:recvd SNDCP ACK pdu in unack mode!\n")
TRACE_MSG(LLENTITY_2523_112_2_18_0_16_11_1759,"LLentity:unexpected value\n")
TRACE_MSG(LLENTITY_3071_112_2_18_0_16_12_1760,"LLE:received invalid command on ADM state")
TRACE_MSG(LLMAIN_638_112_2_18_0_16_14_1761,"LLMAIN: discard l3 pdu, tlli mismatch")
TRACE_MSG(LLMAIN_880_112_2_18_0_16_15_1762,"LLc Frame Error %d")
TRACE_MSG(LLMAIN_1225_112_2_18_0_16_16_1763,"LLc2 Frame Error %d")
TRACE_MSG(LLMAIN_1289_112_2_18_0_16_16_1764,"LLMAIN:discard l3 pdu, tlli mismatch,sapi %d\n")
TRACE_MSG(LLMAIN_1607_112_2_18_0_16_16_1765,"LLMAIN: discard pdu, wrong state")
TRACE_MSG(LLMAIN_1650_112_2_18_0_16_17_1766,"LLMAIN: discard pdu, wrong state")
TRACE_MSG(LLMAIN_1674_112_2_18_0_16_17_1767,"LLMAIN: discard l3 pdu,wrong state")
TRACE_MSG(LLMAIN_1696_112_2_18_0_16_17_1768,"LLMAIN: discard l3 pdu,wrong state")
TRACE_MSG(MAC_523_112_2_18_0_16_18_1769,"MAC:try to start t3192 0 ms")
TRACE_MSG(MAC_983_112_2_18_0_16_19_1770,"MAC: Decode grr freq req error!\n")
TRACE_MSG(MAC_1020_112_2_18_0_16_19_1771,"MAC: DL imm assi,freq Acquare Sys Info error")
TRACE_MSG(MAC_1189_112_2_18_0_16_20_1772,"MAC: DL imm assi,Decode ia rest octets error!")
TRACE_MSG(MAC_1210_112_2_18_0_16_20_1773,"MAC: DL imm assi,Decode chan descri error!")
TRACE_MSG(MAC_1268_112_2_18_0_16_20_1774,"MAC: recv pkt dl ass from rrc alloc no dl slot! Ignore pdch dl req!\n")
TRACE_MSG(MAC_1285_112_2_18_0_16_20_1775,"MAC: Decode freq para error! Acquare Sys Info")
TRACE_MSG(MAC_1554_112_2_18_0_16_20_1776,"MAC: Multislot capability not match! Ignore pdch dl req!")
TRACE_MSG(MAC_1562_112_2_18_0_16_20_1777,"MAC: TBF_MODE in packet downlink ass is different from current!wrong! \n")
TRACE_MSG(MAC_1568_112_2_18_0_16_20_1778,"MAC: No tfi assign,ignore the packet DL assi from ppch!")
TRACE_MSG(MAC_1575_112_2_18_0_16_20_1779,"MAC: Decode packet DL error in idle, ignore the msg!")
TRACE_MSG(MAC_1760_112_2_18_0_16_21_1780,"MAC: Decode rach response error! Acquare Sys Info")
TRACE_MSG(MAC_1779_112_2_18_0_16_21_1781,"MAC: One phase received Dl single assi!\n")
TRACE_MSG(MAC_2115_112_2_18_0_16_22_1782,"MAC: Single block without TBF!")
TRACE_MSG(MAC_2242_112_2_18_0_16_22_1783,"MAC: One phase received Dl assi!")
TRACE_MSG(MAC_2249_112_2_18_0_16_22_1784,"MAC: One phase received Dl EGPRS assi!")
TRACE_MSG(MAC_2267_112_2_18_0_16_22_1785,"MAC: Decode rach response error! Rach retry!")
TRACE_MSG(MAC_2308_112_2_18_0_16_22_1786,"MAC: wrong condition receive rach rsp")
TRACE_MSG(MAC_2344_112_2_18_0_16_22_1787,"MAC: Rach response reject, Rach retry!")
TRACE_MSG(MAC_2397_112_2_18_0_16_22_1788,"MAC: Rach response reject, ul tbf failure!\n")
TRACE_MSG(MAC_2508_112_2_18_0_16_22_1789,"MAC: not support GRR_MAC_PDCH_ASSIGN_REQ")
TRACE_MSG(MAC_2660_112_2_18_0_16_23_1790,"MAC: access abort invalid")
TRACE_MSG(MAC_2713_112_2_18_0_16_23_1791,"MAC: access abort invalid")
TRACE_MSG(MAC_3013_112_2_18_0_16_23_1792,"MAC: tbf mode invalid in connect cnf %d")
TRACE_MSG(MAC_3078_112_2_18_0_16_23_1793,"MAC: Ul release, send ctrl ack request new TBF!")
TRACE_MSG(MAC_3089_112_2_18_0_16_24_1794,"MAC: Ul release, send res req, request new TBF!")
TRACE_MSG(MAC_3094_112_2_18_0_16_24_1795,"MAC: Ul release, unexpected ul_exp_action!!")
TRACE_MSG(MAC_3182_112_2_18_0_16_24_1796,"MAC: receive ul_rel_req but ul not active")
TRACE_MSG(MAC_3249_112_2_18_0_16_24_1797,"MAC:Assert Failure,tbf mode invalid in rlc_mac_rel_req")
TRACE_MSG(MAC_3355_112_2_18_0_16_24_1798,"MAC:Assert Failure,release both link tbf but not in rel state")
TRACE_MSG(MAC_3383_112_2_18_0_16_24_1799,"MAC:tbf mode invalid,uplink tbf starting time?")
TRACE_MSG(MAC_3508_112_2_18_0_16_24_1800,"MAC:rlc_mac_tbf_req in ul_active!!")
TRACE_MSG(MAC_3517_112_2_18_0_16_24_1801,"MAC:ignore rlc tbf req when prach have been send but not receive ass!")
TRACE_MSG(MAC_3623_112_2_18_0_16_25_1802,"MAC:rlc realloc req but ul not active")
TRACE_MSG(MAC_3729_112_2_18_0_16_25_1803,"MAC: wrong state 3186 expire")
TRACE_MSG(MAC_3740_112_2_18_0_16_25_1804,"MAC: T3170 expiry! T3170 Dur: %d \n")
TRACE_MSG(MAC_3837_112_2_18_0_16_25_1805,"MAC: wrong state t3170 expire")
TRACE_MSG(MAC_3903_112_2_18_0_16_25_1806,"MAC: T3168 expiry! T3168 Dur: %d\n")
TRACE_MSG(MAC_4019_112_2_18_0_16_25_1807,"MAC: wrong state rev t3168 expire")
TRACE_MSG(MAC_4136_112_2_18_0_16_26_1808,"MAC: wrong state rev t3164 expire")
TRACE_MSG(MAC_4214_112_2_18_0_16_26_1809,"MAC: wrong state rev t3162 expire")
TRACE_MSG(MAC_4231_112_2_18_0_16_26_1810,"MAC: wrong state rev t3180 expire")
TRACE_MSG(MAC_4310_112_2_18_0_16_26_1811,"MAC: T3192 expiry! T3192 Dur: %d\n")
TRACE_MSG(MAC_4601_112_2_18_0_16_27_1812,"MAC: Prach access not permitted!")
TRACE_MSG(MAC_4640_112_2_18_0_16_27_1813,"MAC: IA allocation uplink 8psk, but MS not support")
TRACE_MSG(MAC_4795_112_2_18_0_16_27_1814,"MAC: IA assignment egprs, but MS not support")
TRACE_MSG(MAC_4907_112_2_18_0_16_27_1815,"MAC: Single block without TBF in EGPRS!\n")
TRACE_MSG(MAC_5286_112_2_18_0_16_28_1816,"MAC: Mac mode mismatch! Ignore packet ul assi!")
TRACE_MSG(MAC_5311_112_2_18_0_16_28_1817,"MAC: rev ul assi in ul idle or other invalid state")
TRACE_MSG(MAC_5324_112_2_18_0_16_28_1818,"MAC: Mac mode mismatch! Ignore packet ul assi!\n")
TRACE_MSG(MAC_5405_112_2_18_0_16_28_1819,"MAC: one_phase_access_error!!")
TRACE_MSG(MAC_5423_112_2_18_0_16_28_1820,"MAC: 1ph pkt ul ass allocation uplink 8psk, but MS not support")
TRACE_MSG(MAC_5485_112_2_18_0_16_28_1821,"MAC: Decode multiblock allocation error! Rach retry!\n")
TRACE_MSG(MAC_5735_112_2_18_0_16_29_1822,"MAC: Contention failure!\n")
TRACE_MSG(MAC_5862_112_2_18_0_16_29_1823,"MAC: EGPRS invalid one phase state when rev ul assi")
TRACE_MSG(MAC_5868_112_2_18_0_16_29_1824,"MAC: Decode Dynamic allocation error! Rach retry!\n")
TRACE_MSG(MAC_5875_112_2_18_0_16_29_1825,"MAC: fixed allocation error! Rach retry!\n")
TRACE_MSG(MAC_5884_112_2_18_0_16_29_1826,"MAC: 1ph pkt ul ass assignment egprs, but MS not support")
TRACE_MSG(MAC_5909_112_2_18_0_16_29_1827,"MAC: Frequency not in band! Rach retry!\n")
TRACE_MSG(MAC_5994_112_2_18_0_16_29_1828,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_5999_112_2_18_0_16_29_1829,"MAC: decode freq error return invalid value")
TRACE_MSG(MAC_6024_112_2_18_0_16_29_1830,"MAC: 2ph allocation uplink 8psk, but MS not support")
TRACE_MSG(MAC_6044_112_2_18_0_16_29_1831,"MAC: wrong assignment in second phase access")
TRACE_MSG(MAC_6269_112_2_18_0_16_30_1832,"MAC: Decode allocation error! Rach retry!\n")
TRACE_MSG(MAC_6276_112_2_18_0_16_30_1833,"MAC: Ingore the egprs ul assi in egprs_ul_assi_two_phase")
TRACE_MSG(MAC_6284_112_2_18_0_16_30_1834,"MAC: 2ph assignment egprs, but MS not support")
TRACE_MSG(MAC_6314_112_2_18_0_16_30_1835,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_6378_112_2_18_0_16_30_1836,"MAC: Frequency not in band! Rach retry!\n")
TRACE_MSG(MAC_6384_112_2_18_0_16_30_1837,"MAC: decode freq error return invalid value")
TRACE_MSG(MAC_6394_112_2_18_0_16_30_1838,"MAC: false tbf mode")
TRACE_MSG(MAC_6408_112_2_18_0_16_30_1839,"MAC: ul active allocation uplink 8psk, but MS not support")
TRACE_MSG(MAC_6417_112_2_18_0_16_30_1840,"MAC: false ul assignment in LABEL_UL_ASSI_UL_ACTIVE")
TRACE_MSG(MAC_6579_112_2_18_0_16_30_1841,"MAC: Decode allocation error in LABEL_UL_ASSI_UL_ACTIVE! Rach retry!\n")
TRACE_MSG(MAC_6588_112_2_18_0_16_30_1842,"MAC: Ingore the ul assi msg beacause lose dynamic allo in LABEL_UL_ASSI_UL_ACTIVE!")
TRACE_MSG(MAC_6596_112_2_18_0_16_31_1843,"MAC: ul active assignment egprs, but MS not support")
TRACE_MSG(MAC_6606_112_2_18_0_16_31_1844,"MAC: rev single block assi when ul active")
TRACE_MSG(MAC_6661_112_2_18_0_16_31_1845,"MAC: DL est when waiting L1 release cnf")
TRACE_MSG(MAC_6684_112_2_18_0_16_31_1846,"MAC:Ignore packet dl assi when two phase or contention phase!")
TRACE_MSG(MAC_6694_112_2_18_0_16_31_1847,"MAC: rev dl assi when ul access two phase or other state")
TRACE_MSG(MAC_6719_112_2_18_0_16_31_1848,"MAC: in ul access recv pkt dl ass alloc no dl slot! Ignore the msg!\n")
TRACE_MSG(MAC_6732_112_2_18_0_16_31_1849,"MAC: wrong state rev dl assi,ignore!")
TRACE_MSG(MAC_6743_112_2_18_0_16_31_1850,"MAC:  dl assign DL_TBF is different with UL_TBF in LABEL_DL_ASSI_UL_ACTIVE! Ignore the msg!\n")
TRACE_MSG(MAC_6758_112_2_18_0_16_31_1851,"MAC: Ignore without dl_tfi dl assign msg while T3192 run!\n")
TRACE_MSG(MAC_6770_112_2_18_0_16_31_1852,"MAC: in dl active recv pkt dl ass alloc no dl slot! Ignore the msg!\n")
TRACE_MSG(MAC_6784_112_2_18_0_16_31_1853,"MAC: currnet dl tbf is EGPRS,receive gprs downlink ass,ignore!\n")
TRACE_MSG(MAC_6791_112_2_18_0_16_31_1854,"MAC: Rlc mode mismatch! Ignore dl ass msg in dl_active!")
TRACE_MSG(MAC_6852_112_2_18_0_16_31_1855,"MAC:contention phase, ignore TS RECONFIG")
TRACE_MSG(MAC_6870_112_2_18_0_16_31_1856,"MAC: error state to receive ts reconfig")
TRACE_MSG(MAC_6888_112_2_18_0_16_31_1857,"MAC: Timeslot reconfig assigned  tbf mode confilit with current ul tbf mode!ignor it!\n")
TRACE_MSG(MAC_6900_112_2_18_0_16_31_1858,"MAC: e_ch_coding_command in ts recofig_ul_active err, ignore it!")
TRACE_MSG(MAC_6916_112_2_18_0_16_31_1859,"MAC: in ul active recv pkt ts reconfig allocation no dl slot! Rach retry!\n")
TRACE_MSG(MAC_6967_112_2_18_0_16_31_1860,"MAC: Decode error! Acquare Sys Info\n")
TRACE_MSG(MAC_6972_112_2_18_0_16_31_1861,"MAC: Decode freq error! UL release and Rach retry!\n")
TRACE_MSG(MAC_6992_112_2_18_0_16_31_1862,"MAC: Decode allocation error! UL release and Rach retry!\n")
TRACE_MSG(MAC_7228_112_2_18_0_16_32_1863,"MAC: Timeslot reconfig not include dl tfi! Ul release and Rach retry!\n")
TRACE_MSG(MAC_7263_112_2_18_0_16_32_1864,"MAC: in dl active recv pkt ts reconfig allocation no dl slot! Rach retry!\n")
TRACE_MSG(MAC_7358_112_2_18_0_16_32_1865,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_7363_112_2_18_0_16_32_1866,"MAC: wrong state when rev ts realloc assi")
TRACE_MSG(MAC_7369_112_2_18_0_16_32_1867,"MAC: Decode freq error! Rach retry!\n")
TRACE_MSG(MAC_7381_112_2_18_0_16_32_1868,"MAC: Timeslot reconfig assigned  tbf mode confilit with current dl tbf mode!ignor it!\n")
TRACE_MSG(MAC_7393_112_2_18_0_16_32_1869,"MAC: e_ch_coding_command in ts recofig_dl_active err, ignore it!")
TRACE_MSG(MAC_7418_112_2_18_0_16_32_1870,"MAC: Decode allocation error! Rach retry!")
TRACE_MSG(MAC_7752_112_2_18_0_16_33_1871,"MAC: Timeslot reconfig not include dl tfi! Rach retry!\n")
TRACE_MSG(MAC_7762_112_2_18_0_16_33_1872,"MAC: Rlc mode mismatch! Ignore ts reconfig msg in dl_active!")
TRACE_MSG(MAC_7812_112_2_18_0_16_33_1873,"MAC: Timeslot reconfig assigned  tbf mode confilit with current dl_ul tbf mode!ignor it!\n")
TRACE_MSG(MAC_7824_112_2_18_0_16_33_1874,"MAC: e_ch_coding_command in ts recofig_ul_dl_active err, ignore it!")
TRACE_MSG(MAC_7843_112_2_18_0_16_33_1875,"MAC: in ul dl active recv pkt ts reconfig allocation no dl slot! Rach retry!\n")
TRACE_MSG(MAC_7892_112_2_18_0_16_33_1876,"MAC: Decode error! Acquare Sys Info\n")
TRACE_MSG(MAC_7897_112_2_18_0_16_33_1877,"MAC: Decode freq error! UL release and Rach retry!\n")
TRACE_MSG(MAC_7917_112_2_18_0_16_33_1878,"MAC: Decode allocation error! Rach retry!")
TRACE_MSG(MAC_8184_112_2_18_0_16_34_1879,"MAC: Rlc mode mismatch! Ignore ts reconfig msg in ul_dl_active!")
TRACE_MSG(MAC_8194_112_2_18_0_16_34_1880,"MAC: EGPRS UL ACK_NACK assigned tbf mode conflict with currnt ul tbf mode, ignore it")
TRACE_MSG(MAC_8207_112_2_18_0_16_34_1881,"MAC: e_ch_coding_command in EGPRS UL ACK_NACK err, ignore it!")
TRACE_MSG(MAC_8450_112_2_18_0_16_35_1882,"MAC: wrong state rev ul ack")
TRACE_MSG(MAC_8476_112_2_18_0_16_35_1883,"MAC: Ignore additional PACKET ACCESS REJECT messages!")
TRACE_MSG(MAC_8781_112_2_18_0_16_36_1884,"MAC: Receive reject, UL release!\n")
TRACE_MSG(MAC_8835_112_2_18_0_16_36_1885,"MAC: ignore PACKET_QUEUING_NOTIFICATION while waiting TBF Starting Time of ul ass!")
TRACE_MSG(MAC_9104_112_2_18_0_16_37_1886,"MAC: wrong ul state recv Packet tbf realse msg!")
TRACE_MSG(MAC_9273_112_2_18_0_16_37_1887,"MAC: recv PPR no affect")
TRACE_MSG(MAC_9301_112_2_18_0_16_37_1888,"MAC: wrong PPR")
TRACE_MSG(MAC_9701_112_2_18_0_16_38_1889,"MAC: wrong state rev t3166 expire")
TRACE_MSG(MAC_9707_112_2_18_0_16_38_1890,"MAC: One phase received egprs ul assi£¬but we don't support EDGE!")
TRACE_MSG(MAC_9951_112_2_18_0_16_39_1891,"MAC: Abnormal release with retry!")
TRACE_MSG(MAC_9975_112_2_18_0_16_39_1892,"MAC: !!Impossible ul_release_state!!")
TRACE_MSG(MAC_10018_112_2_18_0_16_39_1893,"MAC: Abnormal release with rach retry!")
TRACE_MSG(MAC_10030_112_2_18_0_16_39_1894,"MAC: release_cnf for ul but ul not active,maybe hold for connect cnf")
TRACE_MSG(MAC_10089_112_2_18_0_16_39_1895,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_10101_112_2_18_0_16_39_1896,"MAC: Frequency not in band! Rach retry!\n")
TRACE_MSG(MAC_10107_112_2_18_0_16_39_1897,"MAC: decode freq error,invalid return value")
TRACE_MSG(MAC_10130_112_2_18_0_16_39_1898,"MAC: wrong state when rev single block assi")
TRACE_MSG(MAC_10302_112_2_18_0_16_39_1899,"MAC: Decode allocation error! Rach retry!")
TRACE_MSG(MAC_10309_112_2_18_0_16_39_1900,"MAC: Ingore the ul assi in UL_ASSI_TWO_PHASE_ACCESS")
TRACE_MSG(MAC_10388_112_2_18_0_16_40_1901,"MAC: Decode freq error! Acquare Sys Info")
TRACE_MSG(MAC_10399_112_2_18_0_16_40_1902,"MAC: wrong state when rev dl assi")
TRACE_MSG(MAC_10405_112_2_18_0_16_40_1903,"MAC: Decode freq error! Rach retry!")
TRACE_MSG(MAC_10649_112_2_18_0_16_40_1904,"MAC: No tfi assign in dl assi! Rach retry!\n")
TRACE_MSG(MAC_10656_112_2_18_0_16_40_1905,"MAC: Multislot capability not match! Rach retry!")
TRACE_MSG(MAC_10667_112_2_18_0_16_40_1906,"MAC: in ul active recv pkt dl ass alloc no dl slot! Ignore the msg!\n")
TRACE_MSG(MAC_10720_112_2_18_0_16_40_1907,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_10725_112_2_18_0_16_40_1908,"MAC: Decode freq error! Ignore the msg!")
TRACE_MSG(MAC_10924_112_2_18_0_16_41_1909,"MAC: Dl assi not include tfi!")
TRACE_MSG(MAC_10930_112_2_18_0_16_41_1910,"MAC: Multislot capability not match! Ignore the msg!\n")
TRACE_MSG(MAC_11003_112_2_18_0_16_41_1911,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_11292_112_2_18_0_16_41_1912,"MAC: Decode error! acquare si!\n")
TRACE_MSG(MAC_11304_112_2_18_0_16_41_1913,"MAC: Multislot capability not match! Ignore the msg!")
TRACE_MSG(MAC_11312_112_2_18_0_16_41_1914,"MAC: Contention failure!")
TRACE_MSG(MAC_11537_112_2_18_0_16_42_1915,"MAC: Single block without TBF!\n")
TRACE_MSG(MAC_11666_112_2_18_0_16_42_1916,"MAC: invalid one phase state when rev single block assi")
TRACE_MSG(MAC_11880_112_2_18_0_16_43_1917,"MAC: invalid one phase state when rev ul assi")
TRACE_MSG(MAC_11886_112_2_18_0_16_43_1918,"MAC: Decode allocation error! Rach retry!\n")
TRACE_MSG(MAC_11893_112_2_18_0_16_43_1919,"MAC: fixed allocation, rach retry")
TRACE_MSG(RLC_756_112_2_18_0_16_44_1920,"rlc:remain llc tbf request!")
TRACE_MSG(RLC_773_112_2_18_0_16_44_1921,"rlc:cell reselect tbf!")
TRACE_MSG(RLC_992_112_2_18_0_16_45_1922,"rlc:Up Single Block Access!\n")
TRACE_MSG(RLC_1131_112_2_18_0_16_45_1923,"rlc:Up TBF Access for EGPRS SCT!\n")
TRACE_MSG(RLC_1138_112_2_18_0_16_45_1924,"RLC: SCT est fail, downlink tbf is not acitved!")
TRACE_MSG(RLC_1978_112_2_18_0_16_47_1925,"RLC: recv data error in loop back mode")
TRACE_MSG(RLC_2013_112_2_18_0_16_47_1926,"rlc:error data_ind when dl is not actived!")
TRACE_MSG(RLC_2112_112_2_18_0_16_47_1927,"rlc:SCT end!\n")
TRACE_MSG(RLC_2299_112_2_18_0_16_47_1928,"rlc:should not recv up release")
TRACE_MSG(RLC_2529_112_2_18_0_16_48_1929,"rlc:Access Fail with CS Page!\n")
TRACE_MSG(RLC_2931_112_2_18_0_16_49_1930,"rlc:Up tbf fail with dl tbf is esting!\n")
TRACE_MSG(RLC_2935_112_2_18_0_16_49_1931,"RLC: est sigle block without tbf interrupt by dl tbf ass")
TRACE_MSG(RLC_2978_112_2_18_0_16_49_1932,"rlc:Up tbf ok for single block!\n")
TRACE_MSG(RLC_3005_112_2_18_0_16_49_1933,"rlc:IS Rejected Access!\n")
TRACE_MSG(RLC_3027_112_2_18_0_16_49_1934,"RLC:SCT Test Mode B Ested!")
TRACE_MSG(RLC_3034_112_2_18_0_16_49_1935,"RLC:SCT Test Mode A Ested!")
TRACE_MSG(RLC_3056_112_2_18_0_16_49_1936,"RLC:EGPRS SCT Test  Ested!")
TRACE_MSG(RLC_3251_112_2_18_0_16_49_1937,"rlc:mac retry,but no need!\n")
TRACE_MSG(RLC_3285_112_2_18_0_16_49_1938,"rlc:Up tbf fail with t3172 in mac ul_active!\n")
TRACE_MSG(RLC_3363_112_2_18_0_16_49_1939,"rlc:error normal release")
TRACE_MSG(RLC_3552_112_2_18_0_16_50_1940,"rlc:Up fail and perform cell reselect!\n")
TRACE_MSG(RLC_3603_112_2_18_0_16_50_1941,"rlc:Up tbf fail with t3172 in mac ul_active!\n")
TRACE_MSG(RLC_3687_112_2_18_0_16_50_1942,"rlc:Up tbf fail with timer t3172!\n")
TRACE_MSG(RLC_3753_112_2_18_0_16_50_1943,"rlc:Receive Second mac_rlc_tbf_cnf msg!")
TRACE_MSG(RLC_3768_112_2_18_0_16_50_1944,"rlc:Acess state is not one or two phase!")
TRACE_MSG(RRC_1441_112_2_18_0_16_59_1945,"RRC_PutYvarpPtr: multi_sys=%d")
TRACE_MSG(RRC_5211_112_2_18_0_17_6_1946,"DM: receive MPH_CAMP_FAIL_IND")
TRACE_MSG(RRC_5527_112_2_18_0_17_7_1947,"RRC: ASSERT, invalid value. ")
TRACE_MSG(RRC_5836_112_2_18_0_17_8_1948,"RRC: unknown bsic cnf")
TRACE_MSG(RRC_5923_112_2_18_0_17_8_1949,"RRC: ASSERT, research>6")
TRACE_MSG(RRC_5942_112_2_18_0_17_8_1950,"RRC: unknown bsic fail")
TRACE_MSG(RRC_6114_112_2_18_0_17_8_1951,"ASSERT: RRA should't set service_type RRA_RR_INVALID")
TRACE_MSG(RRC_6296_112_2_18_0_17_9_1952,"RRC: ASSERT, it should not be happened.")
TRACE_MSG(RRC_6545_112_2_18_0_17_9_1953,"CCOFU Target cell is only GSM service !\n")
TRACE_MSG(RRC_6555_112_2_18_0_17_9_1954,"CCOFU camp PBCCH!")
TRACE_MSG(RRC_6926_112_2_18_0_17_10_1955,"RRC: rr_dm_request_type:%dn")
TRACE_MSG(RRC_8030_112_2_18_0_17_12_1956,"MDL error with cause value:%dn")
TRACE_MSG(RRC_8168_112_2_18_0_17_12_1957,"DM PS HO CCOFU SUCCESS ")
TRACE_MSG(RRC_8689_112_2_18_0_17_14_1958,"RRC: discard IA,because mm est saved")
TRACE_MSG(RRC_8697_112_2_18_0_17_14_1959,"RRC: discard IA,because plmn list req")
TRACE_MSG(RRC_9285_112_2_18_0_17_15_1960,"RRC: ASSERT, Invalid result string returned by NormIdleModeHandleBcchInd_sdl")
TRACE_MSG(RRC_10678_112_2_18_0_17_18_1961,"RRC: arfcn =%d of camp cnf is different with arfcn=%d of camp req")
TRACE_MSG(RRC_11291_112_2_18_0_17_19_1962,"RR:receive_psi_resel Get Packet paging!!\n")
TRACE_MSG(RRC_11448_112_2_18_0_17_19_1963,"ASSERT: HandleFabRach_sdl return incorrectly")
TRACE_MSG(RRC_11525_112_2_18_0_17_19_1964,"ASSERT: RRA set service_type(%d) is incorrect")
TRACE_MSG(RRC_11540_112_2_18_0_17_19_1965,"ASSERT: RRA shouldn't send MPH_FAB_RACH_IND(%d)")
TRACE_MSG(RRC_11702_112_2_18_0_17_20_1966,"RRC: assert error, not fastplmn")
TRACE_MSG(RRC_11841_112_2_18_0_17_20_1967,"RRC: assert error, not fastplmn")
TRACE_MSG(RRC_12051_112_2_18_0_17_20_1968,"RRC: ASSERT, invalid value. ")
TRACE_MSG(RRC_12273_112_2_18_0_17_21_1969,"RRC: unknown bsic cnf")
TRACE_MSG(RRC_12352_112_2_18_0_17_21_1970,"RRC: unknown bsic fail")
TRACE_MSG(RRC_12884_112_2_18_0_17_22_1971,"CCOFU immediate Ass Ext is received \n")
TRACE_MSG(RRC_12983_112_2_18_0_17_22_1972," CCOFU immediate Ass is received \n")
TRACE_MSG(RRC_13000_112_2_18_0_17_22_1973,"RR: discard immed_assig")
TRACE_MSG(RRC_13033_112_2_18_0_17_22_1974,"RR: rx immediate rej")
TRACE_MSG(RRC_13482_112_2_18_0_17_23_1975,"CCOFU should not received MM_RR_ACT_REQ with non-RR_ACT_NO_SERVICE")
TRACE_MSG(RRC_13961_112_2_18_0_17_24_1976,"RRC wrong sapi %d")
TRACE_MSG(RRC_14033_112_2_18_0_17_24_1977,"RRC: set meas_valid to false")
TRACE_MSG(RRC_14608_112_2_18_0_17_25_1978,"RRC: Invalid dl_sapi_num %d")
TRACE_MSG(RRC_14874_112_2_18_0_17_26_1979,"RRC: wait assign dcch est error in dl chan type")
TRACE_MSG(RRC_15751_112_2_18_0_17_28_1980,"RRC wrong sapi %d")
TRACE_MSG(RRC_16951_112_2_18_0_17_30_1981,"CCOTU has been removed. 2010.11.19")
TRACE_MSG(RRC_17285_112_2_18_0_17_31_1982,"DECODE_SI13_IN_NC_CELL_CHANGE receive msg RR_DELAY_EXP_IND")
TRACE_MSG(RRC_18105_112_2_18_0_17_32_1983,"CCOFU MAC access success!")
TRACE_MSG(RRC_18120_112_2_18_0_17_32_1984,"CCOFU wait MAC access response!")
TRACE_MSG(RRC_18993_112_2_18_0_17_34_1985,"RRC: Receive data block on PPCH, discard it!")
TRACE_MSG(RRC_19804_112_2_18_0_17_36_1986,"RRC: Decode PacketMeasureOrder err!")
TRACE_MSG(RRC_20142_112_2_18_0_17_37_1987,"RRC: Packet cell change failure msg is sent, failure cause=%d")
TRACE_MSG(RRC_20184_112_2_18_0_17_37_1988,"RRC: Decode PacketCellChangeOrder err!")
TRACE_MSG(RRC_20992_112_2_18_0_17_38_1989,"SI2Quater is received in TBF mode ")
TRACE_MSG(RRC_21404_112_2_18_0_17_39_1990,"ignore any data from MPH during DM CCOFU")
TRACE_MSG(RRC_21480_112_2_18_0_17_39_1991,"RRC: ASSERT ERROR recv prach when bad")
TRACE_MSG(RRC_23625_112_2_18_0_17_43_1992,"RR ASSERT: The rat change action is wrong")
TRACE_MSG(RRC_24245_112_2_18_0_17_45_1993,"RRDM: ERROR, handshake timer is timeout.")
TRACE_MSG(RRC_24289_112_2_18_0_17_45_1994,"TIMER_RESEL_TYPE")
TRACE_MSG(RRC_24313_112_2_18_0_17_45_1995,"IMMEDIATE_RESEL_TYPE")
TRACE_MSG(RRC_24338_112_2_18_0_17_45_1996,"NOT_RESEL_TYPE")
TRACE_MSG(RRC_24359_112_2_18_0_17_45_1997,"TDD cell out of sync,clear Candidate_time to 0")
TRACE_MSG(RRC_24463_112_2_18_0_17_45_1998,"NOT_RESEL_TYPE")
TRACE_MSG(RRC_24478_112_2_18_0_17_45_1999,"IMMEDIATE_RESEL_TYPE")
TRACE_MSG(RRC_24503_112_2_18_0_17_45_2000,"TIMER_RESEL_TYPE")
TRACE_MSG(RRC_24519_112_2_18_0_17_45_2001,"TDD cell out of sync,clear Candidate_time to 0")
TRACE_MSG(RRC_24566_112_2_18_0_17_45_2002,"RR_TDD_CELL_RESEL_EXP_IND:RESEL")
TRACE_MSG(RRC_24594_112_2_18_0_17_45_2003,"RESTART RR_TDD_CELL_RESEL_EXP_IND:")
TRACE_MSG(RRC_25040_112_2_18_0_17_46_2004,"cell-prio=%d")
TRACE_MSG(RRC_25356_112_2_18_0_17_47_2005,"RRDM: BSIC decode fail, wait for the next one.")
TRACE_MSG(RRC_25726_112_2_18_0_17_47_2006,"TDD cell_num and arfcn_num are 0 ,clear Candidate_time to 0")
TRACE_MSG(RRC_26439_112_2_18_0_17_49_2007,"DM TD TO GSM PS HO, FAIL , BACK TO TD")
TRACE_MSG(RRC_26474_112_2_18_0_17_49_2008,"DM TD TO GSM  ABORT , BACK TO TD")
TRACE_MSG(RRC_26510_112_2_18_0_17_49_2009,"DM TD TO GSM REJ, FAIL , BACK TO TD")
TRACE_MSG(RRC_26515_112_2_18_0_17_49_2010,"DM TD TO GSM RESEL, FAIL , BACK TO TD")
TRACE_MSG(RRC_26754_112_2_18_0_17_49_2011,"RR ASSERT: The rat change action is wrong")
TRACE_MSG(RRC_26768_112_2_18_0_17_49_2012,"RRC: Recv MPH_RF_SCAN_IND in state %d")
TRACE_MSG(RRC_26854_112_2_18_0_17_50_2013,"DM: goto label GO_TO_NO_CELL")
TRACE_MSG(RRC_27392_112_2_18_0_17_51_2014,"DM: REJ to 2G is fail, revert")
TRACE_MSG(RRC_28028_112_2_18_0_17_52_2015,"RRC: discard IA,because plmn list req")
TRACE_MSG(RRC_28245_112_2_18_0_17_52_2016,"RRC: Receive data block on PPCH, discard it!")
TRACE_MSG(RRC_28427_112_2_18_0_17_53_2017,"RRC: Decode pmo err!")
TRACE_MSG(RRC_28465_112_2_18_0_17_53_2018,"RRC: Packet cell change failure msg is sent, failure cause=%d")
TRACE_MSG(RRC_28508_112_2_18_0_17_53_2019,"RRC: find ncell fail")
TRACE_MSG(RRC_28521_112_2_18_0_17_53_2020,"RRC: gmm state not ready")
TRACE_MSG(RRC_28536_112_2_18_0_17_53_2021,"RRC: decode pcco fail")
TRACE_MSG(RRC_29660_112_2_18_0_17_55_2022,"RRC: process saved ppch")
TRACE_MSG(RRC_29749_112_2_18_0_17_55_2023,"RRC: proc saved pch ")
TRACE_MSG(RRC_29858_112_2_18_0_17_56_2024,"RRC: Packet cell change failure msg is sent, failure cause=%d")
TRACE_MSG(RRC_29896_112_2_18_0_17_56_2025,"RRC: %d enter_idle %d,%d,%d,%d")
TRACE_MSG(RRC_29996_112_2_18_0_17_56_2026,"RRC: Error, invalid ENTER_IDLE proc!!! %d ")
TRACE_MSG(RRC_30032_112_2_18_0_17_56_2027,"RRC: Plmn is scaning, skip limited search")
TRACE_MSG(RRC_30396_112_2_18_0_17_57_2028,"RRC: resel scell arfcn 0x%x")
TRACE_MSG(RRC_30628_112_2_18_0_17_57_2029,"RRC: cell resel arfcn 0x%x")
TRACE_MSG(RRC_30890_112_2_18_0_17_58_2030," RRC: switch to another cell, clear PSI Msg Buffer \n")
TRACE_MSG(RRC_31328_112_2_18_0_17_58_2031,"RRC: unknown bsic cnf")
TRACE_MSG(RRC_31497_112_2_18_0_17_59_2032,"RRC: warning, AssignPlmnScan_sdl fail %d,%d")
TRACE_MSG(RRC_32138_112_2_18_0_18_0_2033,"RR temp = %d")
TRACE_MSG(RRC_33405_112_2_18_0_18_3_2034,"RRC: Decode PacketMeasureOrder err!")
TRACE_MSG(RRC_33492_112_2_18_0_18_3_2035,"RRC: Packet cell change failure msg is sent, failure cause=%d")
TRACE_MSG(RRC_34038_112_2_18_0_18_4_2036,"RRC: Decode Pmo err!")
TRACE_MSG(RRC_34094_112_2_18_0_18_4_2037,"RRC: Receive data block on PPCH, discard it!")
TRACE_MSG(RRC_34175_112_2_18_0_18_4_2038,"DM PS HO CCOFU SUCCESS ")
TRACE_MSG(RRC_34970_112_2_18_0_18_6_2039,"RRC ASSERT: current state=%d")
TRACE_MSG(RRC_35001_112_2_18_0_18_6_2040,"Packet transfer reselect to UTRAN; start")
TRACE_MSG(RRC_35016_112_2_18_0_18_6_2041,"PCCN TO UTRAN; start")
TRACE_MSG(RRC_35062_112_2_18_0_18_6_2042,"RRC:bsic_req_num=%d,arfcn_num=%d,search_num=%d")
TRACE_MSG(RRC_35158_112_2_18_0_18_6_2043,"DM: goto lable DM-RESEL-TO-TD")
TRACE_MSG(RRC_35194_112_2_18_0_18_6_2044,"DM: goto label DM_RESEL_TO_TD_CARD_SYNC")
TRACE_MSG(RRC_35355_112_2_18_0_18_6_2045,"DM: goto lable DM_FAST_RETURN_TO_TD")
TRACE_MSG(RRC_35405_112_2_18_0_18_7_2046,"Immediate to start PCCOTU")
TRACE_MSG(RRC_35491_112_2_18_0_18_7_2047,"CCOFU arfcn = %d, band %d")
TRACE_MSG(RRC_35538_112_2_18_0_18_7_2048,"DM: goto lable DM_CCOFU_BSIC_DECODE")
TRACE_MSG(RRC_35625_112_2_18_0_18_7_2049,"si13 has been received together with si1^4 when ccofu")
TRACE_MSG(RRC_35665_112_2_18_0_18_7_2050,"DM RR_MM_ACT_IND is send! ")
TRACE_MSG(RRC_35721_112_2_18_0_18_7_2051,"DM GRR_RLC_ACT_REQ is send! ")
TRACE_MSG(RRC_35732_112_2_18_0_18_7_2052,"DM GRR_RLC_SUSPEND is send!")
TRACE_MSG(RRC_35744_112_2_18_0_18_7_2053,"GRR_RLC_NC_CELL_CHANGE_REQ is send!")
TRACE_MSG(RRC_35835_112_2_18_0_18_7_2054,"DM TD TO GSM  ABORT  REQ Error")
TRACE_MSG(RRC_35850_112_2_18_0_18_7_2055,"DM TD TO GSM  ABORT , BACK TO TD,L1 not active ")
TRACE_MSG(RRC_35932_112_2_18_0_18_8_2056,"DM: goto lable DM_TD_RESEL_TO_GSM")
TRACE_MSG(RRC_35934_112_2_18_0_18_8_2057,"RRC:rr_user_request_band=%d,ms_band=%d")
TRACE_MSG(RRC_36023_112_2_18_0_18_8_2058,"DM: goto lable DM_RESEL_BACK_TO_TD")
TRACE_MSG(RRC_36047_112_2_18_0_18_8_2059,"DM: goto label  DM_RESEL_FAIL_NO_REVERT")
TRACE_MSG(RRC_36099_112_2_18_0_18_8_2060,"DM: goto lable DM_RESEL_HANDLE_BCCH_INFO")
TRACE_MSG(RRC_36140_112_2_18_0_18_8_2061,"PLMN is  in FPLMN list")
TRACE_MSG(RRC_36146_112_2_18_0_18_8_2062,"PLMN is not Selected PLMN")
TRACE_MSG(RRC_36188_112_2_18_0_18_8_2063,"cell-prio=%d")
TRACE_MSG(RRC_36228_112_2_18_0_18_8_2064,"PLMN is in FLAI list")
TRACE_MSG(RRC_36256_112_2_18_0_18_8_2065,"Arfcn not matcn")
TRACE_MSG(RRC_36263_112_2_18_0_18_8_2066,"cell-prio error")
TRACE_MSG(RRC_36270_112_2_18_0_18_8_2067,"CELL is barred when 3-2 reselect")
TRACE_MSG(RRC_36288_112_2_18_0_18_8_2068,"DM: continue to handle BCCH info")
TRACE_MSG(RRC_36299_112_2_18_0_18_8_2069,"DM: goto lable DM_TD_HO_TO_GSM")
TRACE_MSG(RRC_36363_112_2_18_0_18_8_2070,"DM: goto lable DM_HO_CNF")
TRACE_MSG(RRC_36435_112_2_18_0_18_9_2071,"DM: goto lable DM-HO-WAIT-PHYINFO")
TRACE_MSG(RRC_36520_112_2_18_0_18_9_2072,"DM: goto lable DM_HO_HANDLE_EST_CNF")
TRACE_MSG(RRC_36525_112_2_18_0_18_9_2073,"suspend gprs task after handover successfully.")
TRACE_MSG(RRC_36720_112_2_18_0_18_9_2074,"DM , goto lable DM-HO-HANDLE-REL-IND")
TRACE_MSG(RRC_36722_112_2_18_0_18_9_2075,"DM   rx DL_RELEASE_IND in WAIT_HANDOVER_DCCH_EST\n")
TRACE_MSG(RRC_36744_112_2_18_0_18_9_2076,"goto lable DM_TD_REJ_TO_GSM")
TRACE_MSG(RRC_36810_112_2_18_0_18_9_2077,"DM: goto lable DM-RESEL-HO-BSIC-CNF")
TRACE_MSG(RRC_36873_112_2_18_0_18_9_2078,"DM:, goto lable DM_RESEL_HO_BSIC_FAIL")
TRACE_MSG(RRC_38845_112_2_18_0_18_13_2079,"rr_cur_manual_type=%d")
TRACE_MSG(RRC_40617_112_2_18_0_18_17_2080,"RRC:  multi_sys=%d,RR State=%d,  plmn_list_req_state=%d,oper=%d,search end=%d")
TRACE_MSG(RRC_40883_112_2_18_0_18_17_2081,"ASSERT: RRA set service_type(%d) is incorrect")
TRACE_MSG(RRC_40957_112_2_18_0_18_18_2082,"warning: unexpect rr status appears in the procedure")
TRACE_MSG(RRC_41211_112_2_18_0_18_18_2083,"RRC: Discard SI13 when scell is limited camp")
TRACE_MSG(SNDCPENTITY_875_112_2_18_0_18_21_2084,"SNDCP: received release_ind with RELEASE_CAUSE_NORMAL_RELEASE on esting state")
TRACE_MSG(SNDCPENTITY_904_112_2_18_0_18_21_2085,"SNDCP:received not defined cause.\n")
TRACE_MSG(SNDCPENTITY_1367_112_2_18_0_18_22_2086,"SNDCP:Discard NPDU on invalid state!")
TRACE_MSG(SNDCPENTITY_1389_112_2_18_0_18_22_2087,"SNDCP:Discard NPDU when reaching on the max buffer_size!")
TRACE_MSG(SNDCPENTITY_1464_112_2_18_0_18_22_2088,"SNDCP:Discard NPDU on invalid state!")
TRACE_MSG(SNDCPMAIN_318_112_2_18_0_18_27_2089,"SNDCP:NSAPI already existed.")
TRACE_MSG(SNDCPMAIN_414_112_2_18_0_18_27_2090,"SNDCP:NSAPI doesnt exist.")
TRACE_MSG(SNDCPMAIN_676_112_2_18_0_18_28_2091,"SNDCP:SN_UNITDATA_REQ - Null pointer")
TRACE_MSG(SNDCPMAIN_735_112_2_18_0_18_28_2092,"SNDCP:SN_DATA_REQ - Null pointer")
TRACE_MSG(SNDCPMAIN_1355_112_2_18_0_18_29_2093,"SNDCP:LL_SNDC_UNITDATA_IND - wrong nsapi or sapi")
TRACE_MSG(SNDCPMAIN_1360_112_2_18_0_18_29_2094,"SNDCP:LL_SNDC_UNITDATA_IND - Null pointer")
TRACE_MSG(SNDCPMAIN_1423_112_2_18_0_18_29_2095,"SNDCP:LL_SNDC_DATA_IND - wrong nsapi")
TRACE_MSG(SNDCPMAIN_1428_112_2_18_0_18_29_2096,"SNDCP:LL_SNDC_DATA_IND - Null pointer")
TRACE_MSG(CCENTITY_5433_112_2_18_0_18_46_2097,"CC: send start dtmf at %d staten")
TRACE_MSG(CCENTITY_5478_112_2_18_0_18_46_2098,"CC: send stop dtmf at %d state n")
TRACE_MSG(CMROUTER_337_112_2_18_0_18_50_2099,"cmrouter: Rel_PId, ti is %x\n")
TRACE_MSG(CMROUTER_1746_112_2_18_0_18_53_2100,"cmrouter: deal MNSS_END_REQ, ti is %x, entity not exist\n")
TRACE_MSG(CMROUTER_2192_112_2_18_0_18_54_2101,"cmrouter: deal MNSS_END_REQ, ti is %x, entity not exist\n")
TRACE_MSG(CMROUTER_2610_112_2_18_0_18_55_2102,"cmrouter: deal MNSS_END_REQ, ti is %x, entity not exist\n")
TRACE_MSG(DLR_272_112_2_18_0_19_5_2103,"DL: release req 0,but no entity")
TRACE_MSG(DLR_296_112_2_18_0_19_5_2104,"DL: release req 3,but no entity")
TRACE_MSG(DLR_458_112_2_18_0_19_5_2105,"L2: unexpected sapi\n")
TRACE_MSG(DLR_776_112_2_18_0_19_6_2106,"PH_READY_TO_SEND_IND recved, but task released\n")
TRACE_MSG(LAYER1_594_112_2_18_0_19_47_2107,"deactive req.\n")
TRACE_MSG(LAYER1_667_112_2_18_0_19_47_2108,"rxpower_sweep_req asked and confirm replied.\n")
TRACE_MSG(LAYER1_685_112_2_18_0_19_47_2109,"rxpower from BS come.\n")
TRACE_MSG(LAYER1_703_112_2_18_0_19_47_2110,"bcch req asked.\n")
TRACE_MSG(LAYER1_714_112_2_18_0_19_48_2111,"the bcch req is single %d.\n")
TRACE_MSG(LAYER1_736_112_2_18_0_19_48_2112,"bcch req stop.\n")
TRACE_MSG(LAYER1_753_112_2_18_0_19_48_2113,"bcch timer expired.\n")
TRACE_MSG(LAYER1_788_112_2_18_0_19_48_2114,"bsic req asked.\n")
TRACE_MSG(LAYER1_864_112_2_18_0_19_48_2115,"\n*****Layer1 Rx BCCH:*****\n")
TRACE_MSG(LAYER1_865_112_2_18_0_19_48_2116,"BCCH_ARFCN:       %d \n\n")
TRACE_MSG(LAYER1_906_112_2_18_0_19_48_2117,"\n*****L1Sim Rx BCCH:*****\n")
TRACE_MSG(LAYER1_907_112_2_18_0_19_48_2118,"BCCH_ARFCN:%d \n\n")
TRACE_MSG(LAYER1_950_112_2_18_0_19_48_2119,"bcch ind true\n")
TRACE_MSG(LAYER1_1111_112_2_18_0_19_48_2120,"campon req&confirmed.\n")
TRACE_MSG(LAYER1_1209_112_2_18_0_19_49_2121,"TBF Downlink Assignment Recv\n")
TRACE_MSG(LAYER1_1282_112_2_18_0_19_49_2122,"Send the first RA\n")
TRACE_MSG(LAYER1_1356_112_2_18_0_19_49_2123,"L1 Send Random Acess Burst\n")
TRACE_MSG(LAYER1_1410_112_2_18_0_19_49_2124,"AGCH recv\n")
TRACE_MSG(LAYER1_1682_112_2_18_0_19_49_2125,"PLMN search asked and  rxpower confirm replied.\n")
TRACE_MSG(LAYER1_2012_112_2_18_0_19_50_2126,"chan_assign:%d\n")
TRACE_MSG(LAYER1_2054_112_2_18_0_19_50_2127,"chan_assign_fail:%d\n")
TRACE_MSG(LAYER1_2137_112_2_18_0_19_50_2128,"handover_fail:%d\n")
TRACE_MSG(LAYER1_2251_112_2_18_0_19_51_2129,"Received Data block from new DCCH in non-syn handover\n")
TRACE_MSG(LAYER1_2260_112_2_18_0_19_51_2130,"handover :%d\n")
TRACE_MSG(LAYER1_2335_112_2_18_0_19_51_2131,"handover_fail:%d\n")
TRACE_MSG(LAYER1_2384_112_2_18_0_19_51_2132,"L1:sacch data recv\n")
TRACE_MSG(LAYER1_2406_112_2_18_0_19_51_2133,"l1 recv data:%d\n")
TRACE_MSG(LAYER1_2486_112_2_18_0_19_51_2134,"L1:data send at chan %d\n")
TRACE_MSG(LAYER1_2508_112_2_18_0_19_51_2135,"L1:sacch data send\n")
TRACE_MSG(LAYER1_2702_112_2_18_0_19_51_2136,"This bcch block is unexpected\n")
TRACE_MSG(LAYER1_2740_112_2_18_0_19_52_2137,"bcch ind,true\n")
TRACE_MSG(LAYER1_2747_112_2_18_0_19_52_2138,"bcch ind,not requested\n")
TRACE_MSG(LAYER1_2860_112_2_18_0_19_52_2139,"SI_ID: 1")
TRACE_MSG(LAYER1_2864_112_2_18_0_19_52_2140,"SI_ID: 2")
TRACE_MSG(LAYER1_2868_112_2_18_0_19_52_2141,"SI_ID: 2Bis")
TRACE_MSG(LAYER1_2872_112_2_18_0_19_52_2142,"SI_ID: 2Ter")
TRACE_MSG(LAYER1_2876_112_2_18_0_19_52_2143,"SI_ID: 3")
TRACE_MSG(LAYER1_2880_112_2_18_0_19_52_2144,"SI_ID: 4")
TRACE_MSG(LAYER1_2884_112_2_18_0_19_52_2145,"SI_ID: 13")
TRACE_MSG(LAYER1_2888_112_2_18_0_19_52_2146,"SI_ID: 5BIS")
TRACE_MSG(LAYER1_2892_112_2_18_0_19_52_2147,"SI_ID: 5BIS")
TRACE_MSG(LAYER1_2896_112_2_18_0_19_52_2148,"SI_ID: 6")
TRACE_MSG(LAYER1_2900_112_2_18_0_19_52_2149,"SI_ID: 7")
TRACE_MSG(LAYER1_2904_112_2_18_0_19_52_2150,"PSI_ID: 13")
TRACE_MSG(LAYER1_2908_112_2_18_0_19_52_2151,"SI_ID: 4")
TRACE_MSG(LAYER1_2912_112_2_18_0_19_52_2152,"PSI_ID: 3Bis")
TRACE_MSG(LAYER1_2916_112_2_18_0_19_52_2153,"SI_ID: 8")
TRACE_MSG(LAYER1_2920_112_2_18_0_19_52_2154,"SI_ID: 9")
TRACE_MSG(LAYER1_2924_112_2_18_0_19_52_2155,"SI_ID: 13")
TRACE_MSG(LAYER1_2928_112_2_18_0_19_52_2156,"PSI_ID: 1")
TRACE_MSG(LAYER1_2932_112_2_18_0_19_52_2157,"PSI_ID: 2")
TRACE_MSG(LAYER1_2936_112_2_18_0_19_52_2158,"PSI_ID: 3 ")
TRACE_MSG(PROCESSMM_1932_112_2_18_0_19_56_2159,"MM: read sim data error")
TRACE_MSG(PROCESSMM_2123_112_2_18_0_19_56_2160,"MM:begin saved call")
TRACE_MSG(PROCESSMM_2222_112_2_18_0_19_57_2161,"MM:begin saved call")
TRACE_MSG(PROCESSMM_2663_112_2_18_0_19_58_2162,"MM: GSM service detach\n")
TRACE_MSG(PROCESSMM_2683_112_2_18_0_19_58_2163,"MM: MS_DEACT_TO_CALI in WAIT FOR MN REG")
TRACE_MSG(PROCESSMM_4173_112_2_18_0_20_1_2164,"MM:receive MM_PD MSG at WAIT_FOR_NETWORK_COMMAND\n")
TRACE_MSG(PROCESSMM_6308_112_2_18_0_20_5_2165,"MM:begin saved call")
TRACE_MSG(PROCESSMM_6921_112_2_18_0_20_6_2166,"MM:begin saved call")
TRACE_MSG(PROCESSMM_8568_112_2_18_0_20_9_2167,"MM:begin saved call")
TRACE_MSG(PROCESSMM_10298_112_2_18_0_20_13_2168,"MM_CALI: MN_MM_NREG_REQ in WAIT_FOR_RR_CONNECTION_MM\n")
TRACE_MSG(PROCESSMM_10633_112_2_18_0_20_13_2169,"MM_CALI: MN_MM_NREG_REQ in other status, with mm connect\n")
TRACE_MSG(PROCESSMM_11138_112_2_18_0_20_14_2170,"MM_CALI: MN_MM_NREG_REQ in  MM_NO_CELL")
TRACE_MSG(PROCESSMM_11204_112_2_18_0_20_15_2171,"MM_CALI: MN_MM_NREG_REQ in MM_NO_SIM")
TRACE_MSG(PROCESSMM_11406_112_2_18_0_20_15_2172,"MM:begin saved call")
TRACE_MSG(PROCESSMM_11465_112_2_18_0_20_15_2173,"MM: sim authentication error")
TRACE_MSG(PROCESSMM_11536_112_2_18_0_20_15_2174,"MM:begin saved call")
TRACE_MSG(PROCESSMM_12046_112_2_18_0_20_16_2175,"MM:begin saved call")
TRACE_MSG(PROCESSMM_12338_112_2_18_0_20_17_2176,"MM:begin saved call")
TRACE_MSG(PROCESSMM_12442_112_2_18_0_20_17_2177,"MM:begin saved call")
TRACE_MSG(PROCESSMM_12461_112_2_18_0_20_17_2178,"MM:begin saved call")
TRACE_MSG(PROCESSMM_12564_112_2_18_0_20_17_2179,"MM:begin saved call")
TRACE_MSG(PROCESSMM_13164_112_2_18_0_20_18_2180,"MM:discard MM_RR_DATA_IND")
TRACE_MSG(PROCESSMM_13491_112_2_18_0_20_19_2181,"MM_CALI: MN_MM_NREG_REQ in other status, with mm connect\n")
TRACE_MSG(PROCESSMM_13592_112_2_18_0_20_19_2182,"MM_CALI: MN_MM_NREG_REQ in other status, without mm connect\n")
TRACE_MSG(PROCESSMM_13709_112_2_18_0_20_19_2183,"MM:begin saved call")
TRACE_MSG(PROCESSMM_14170_112_2_18_0_20_20_2184,"MM: current T3212 duration is %d")
TRACE_MSG(PROCESSMM_15017_112_2_18_0_20_22_2185,"MM: There is a delay CM service\n")
TRACE_MSG(RRC_2769_112_2_18_0_20_38_2186,"RRC: unknown bsic cnf")
TRACE_MSG(RRC_2833_112_2_18_0_20_38_2187,"RRC: ASSERT, research>6")
TRACE_MSG(RRC_2852_112_2_18_0_20_38_2188,"RRC: unknown bsic fail")
TRACE_MSG(RRC_4366_112_2_18_0_20_42_2189,"RRC: discard IA,because mm est saved")
TRACE_MSG(RRC_4374_112_2_18_0_20_42_2190,"RRC: discard IA,because plmn list req")
TRACE_MSG(RRC_5798_112_2_18_0_20_44_2191,"RRC: assert error, not fastplmn")
TRACE_MSG(RRC_5937_112_2_18_0_20_45_2192,"RRC: assert error, not fastplmn")
TRACE_MSG(RRC_6397_112_2_18_0_20_46_2193,"RRC: unknown bsic cnf")
TRACE_MSG(RRC_6476_112_2_18_0_20_46_2194,"RRC: unknown bsic fail")
TRACE_MSG(RRC_6867_112_2_18_0_20_47_2195,"RR: discard immed_assig")
TRACE_MSG(RRC_6900_112_2_18_0_20_47_2196,"RR: rx immediate rej")
TRACE_MSG(RRC_7613_112_2_18_0_20_48_2197,"RRC wrong sapi %d")
TRACE_MSG(RRC_8400_112_2_18_0_20_50_2198,"RRC: wait assign dcch est error in dl chan type")
TRACE_MSG(RRC_9095_112_2_18_0_20_51_2199,"RRC wrong sapi %d")
TRACE_MSG(RRC_11746_112_2_18_0_20_57_2200,"RRC: discard IA,because plmn list req")
TRACE_MSG(RRC_13294_112_2_18_0_21_0_2201,"RRC: proc saved pch ")
TRACE_MSG(RRC_13878_112_2_18_0_21_1_2202,"RRC: resel scell arfcn 0x%x")
TRACE_MSG(RRC_13998_112_2_18_0_21_1_2203,"RRC: cell resel arfcn 0x%x")
TRACE_MSG(RRC_14394_112_2_18_0_21_2_2204,"RRC: unknown bsic cnf")
TRACE_MSG(RRC_15165_112_2_18_0_21_4_2205,"RR temp = %d")
TRACE_MSG(SM_COMMON_249_112_2_18_0_21_36_2206,"SMCOMMON:delete the activation pdp context request(nsapi %d)")
TRACE_MSG(SM_COMMON_277_112_2_18_0_21_37_2207,"SMCOMMON: handle nsapi %d")
TRACE_MSG(SM_DECODE_106_112_2_18_0_21_37_2208,"SM:decode Transaction identifier, TIE reserved!")
TRACE_MSG(SM_DECODE_133_112_2_18_0_21_37_2209,"SM:decode Transaction identifier, EXT is '0'!")
TRACE_MSG(SM_DECODE_261_112_2_18_0_21_37_2210,"SM:PDP address length too long!")
TRACE_MSG(SM_DECODE_284_112_2_18_0_21_37_2211,"SM:decode pdp address,PPP,PDP type reserved!")
TRACE_MSG(SM_DECODE_312_112_2_18_0_21_37_2212,"SM:decode pdp address,PDP org type reserved!")
TRACE_MSG(SM_DECODE_394_112_2_18_0_21_38_2213,"SM: LLC SAPI Spare Field non-0!")
TRACE_MSG(SM_DECODE_402_112_2_18_0_21_38_2214,"SM: LLC SAPI not assigned!")
TRACE_MSG(SM_DECODE_414_112_2_18_0_21_38_2215,"SM: LLC SAPI value reserved,invalid!")
TRACE_MSG(SM_DECODE_558_112_2_18_0_21_38_2216,"SM: is_r99 = %d")
TRACE_MSG(SM_DECODE_589_112_2_18_0_21_38_2217,"SM:decode Qos Field invalid!")
TRACE_MSG(SM_DECODE_689_112_2_18_0_21_38_2218,"SM:decode Qos Field invalid!")
TRACE_MSG(SM_DECODE_696_112_2_18_0_21_38_2219,"SM:ignore extented qos fields!")
TRACE_MSG(SM_DECODE_738_112_2_18_0_21_39_2220,"SM: Radio Priority Spare Field non-0!")
TRACE_MSG(SM_DECODE_743_112_2_18_0_21_39_2221,"SM: Radio Priority reserved,level 4!")
TRACE_MSG(SM_DECODE_784_112_2_18_0_21_39_2222,"SM:Receive APN Length > APN buffer!")
TRACE_MSG(SM_DECODE_908_112_2_18_0_21_39_2223,"SM: PDP context accept,not support iei = %d")
TRACE_MSG(SM_DECODE_1022_112_2_18_0_21_39_2224,"SM: PDP context accept,not support iei = %d")
TRACE_MSG(SM_DECODE_1083_112_2_18_0_21_39_2225,"SM: CTIVATE PDP CONTEXT REJECT,not support iei = %d")
TRACE_MSG(SM_DECODE_1149_112_2_18_0_21_39_2226,"SM: REQUEST PDP CONTEXT ACTIVATION, not support iei = %d")
TRACE_MSG(SM_DECODE_1207_112_2_18_0_21_39_2227,"SM: DEACTIVATE PDP CONTEXT REQUEST,not support iei = %d")
TRACE_MSG(SM_DECODE_1303_112_2_18_0_21_40_2228,"SM: Modify PDP context request(NT2MS),not support iei = %d")
TRACE_MSG(SM_DECODE_1396_112_2_18_0_21_40_2229,"SM:Modify PDP context accept(NT2MS),not support iei = %d")
TRACE_MSG(SM_DECODE_1511_112_2_18_0_21_40_2230,"SM: PDP 2th context accept,not support iei = %d")
TRACE_MSG(SM_DECODE_1536_112_2_18_0_21_40_2231,"SM:PDP context accept Qos < MS Min_Qos Setting!!")
TRACE_MSG(SM_DECODE_1541_112_2_18_0_21_40_2232,"SM:PDP context accept Qos > MS Min_Qos Setting!!\n")
TRACE_MSG(SM_DECODE_1572_112_2_18_0_21_40_2233,"APN:invalid char appear!")
TRACE_MSG(SM_ENCODE_289_112_2_18_0_21_41_2234,"SM:SM_EncodePdpaddr pdp_addr_length too long!")
TRACE_MSG(SM_ENCODE_321_112_2_18_0_21_41_2235,"SM:SM_EncodePdpaddr pdp_addr_length too long!")
TRACE_MSG(SM_ENCODE_336_112_2_18_0_21_41_2236,"SM:Assert Failure,SM_EncodePdpaddr")
TRACE_MSG(SM_ENCODE_369_112_2_18_0_21_41_2237,"SM:SM_EncodeApn apn too long!")
TRACE_MSG(SMS_RL_195_112_2_18_0_21_49_2238,"SMSRL_DecodeRpError: the cause is %d")
TRACE_MSG(RFC2507C_105_112_2_18_0_21_52_2239,"\n")
TRACE_MSG(RFC2507C_106_112_2_18_0_21_52_2240,"v6s_full_c \t %d \n")
TRACE_MSG(RFC2507C_107_112_2_18_0_21_52_2241,"v6s_comp_c \t %d \n")
TRACE_MSG(RFC2507C_108_112_2_18_0_21_52_2242,"v6s_unmod_c \t %d \n")
TRACE_MSG(RFC2507C_109_112_2_18_0_21_52_2243,"v6s_bbefore_c \t %d \n")
TRACE_MSG(RFC2507C_110_112_2_18_0_21_52_2244,"v6s_bafter_c \t %d \n")
TRACE_MSG(RFC2507C_111_112_2_18_0_21_52_2245,"v6s_changes_c \t %d \n")
TRACE_MSG(RFC2507C_112_112_2_18_0_21_52_2246,"v6s_unknown_c \t %d \n")
TRACE_MSG(RFC2507C_113_112_2_18_0_21_52_2247,"v6s_full_d \t %d \n")
TRACE_MSG(RFC2507C_114_112_2_18_0_21_52_2248,"v6s_comp_d \t %d \n")
TRACE_MSG(RFC2507C_115_112_2_18_0_21_52_2249,"v6s_unmod_d \t %d \n")
TRACE_MSG(RFC2507C_116_112_2_18_0_21_52_2250,"v6s_dropped_d \t %d \n")
TRACE_MSG(RFC2507C_117_112_2_18_0_21_52_2251,"v6s_badsum_d \t %d \n")
TRACE_MSG(RFC2507C_118_112_2_18_0_21_52_2252,"v6s_head_req_revc \t %d \n")
TRACE_MSG(RFC2507C_119_112_2_18_0_21_52_2253,"\n")
TRACE_MSG(RFC2507C_172_112_2_18_0_21_52_2254,"free_all: start\n")
TRACE_MSG(RFC2507C_208_112_2_18_0_21_52_2255,"free_all: done\n")
TRACE_MSG(RFC2507C_414_112_2_18_0_21_53_2256,"iphc_init: start sizeof(*csList) == %u\n")
TRACE_MSG(RFC2507C_416_112_2_18_0_21_53_2257,"iphc_init: sizeof(*comp) == %u sizeof(*decomp) == %un")
TRACE_MSG(RFC2507C_419_112_2_18_0_21_53_2258,"iphc_init: csList == NULL\n")
TRACE_MSG(RFC2507C_428_112_2_18_0_21_53_2259,"iphc_init: cfg == NULL\n")
TRACE_MSG(RFC2507C_433_112_2_18_0_21_53_2260,"iphc_init: len_send_t %d \n")
TRACE_MSG(RFC2507C_434_112_2_18_0_21_53_2261,"iphc_init: len_recv_t %d \n")
TRACE_MSG(RFC2507C_435_112_2_18_0_21_53_2262,"iphc_init: len_send_u %d \n")
TRACE_MSG(RFC2507C_436_112_2_18_0_21_53_2263,"iphc_init: len_recv_u %d \n")
TRACE_MSG(RFC2507C_437_112_2_18_0_21_53_2264,"iphc_init: max_header %d \n")
TRACE_MSG(RFC2507C_438_112_2_18_0_21_53_2265,"iphc_init: max_period %d \n")
TRACE_MSG(RFC2507C_439_112_2_18_0_21_53_2266,"iphc_init: max_time   %d \n")
TRACE_MSG(RFC2507C_440_112_2_18_0_21_53_2267,"iphc_init: min_wrap   %d \n")
TRACE_MSG(RFC2507C_441_112_2_18_0_21_53_2268,"iphc_init: reordering %d \n")
TRACE_MSG(RFC2507C_442_112_2_18_0_21_53_2269,"iphc_init: nodelta    %d \n")
TRACE_MSG(RFC2507C_443_112_2_18_0_21_53_2270,"iphc_init: udp_prime  %d \n")
TRACE_MSG(RFC2507C_444_112_2_18_0_21_53_2271,"iphc_init: tcp_prime  %d \n")
TRACE_MSG(RFC2507C_459_112_2_18_0_21_53_2272,"iphc_init: bad init values\n")
TRACE_MSG(RFC2507C_464_112_2_18_0_21_53_2273,"iphc_init: bad value for tcp_prime\n")
TRACE_MSG(RFC2507C_468_112_2_18_0_21_53_2274,"iphc_init: bad value for udp_prime\n")
TRACE_MSG(RFC2507C_539_112_2_18_0_21_53_2275,"iphc_init: init done.\n")
TRACE_MSG(RFC2507C_673_112_2_18_0_21_53_2276,"hash_keys: *** UNKNOWN IP version %d ***\n")
TRACE_MSG(RFC2507C_678_112_2_18_0_21_53_2277,"iphc_hashkeys: IPv4 hdr corrupt\n")
TRACE_MSG(RFC2507C_681_112_2_18_0_21_53_2278,"hash_keys: IP4\n")
TRACE_MSG(RFC2507C_688_112_2_18_0_21_53_2279,"hash_keys: length fields for IP4 do not match! Bailing out!\n")
TRACE_MSG(RFC2507C_690_112_2_18_0_21_53_2280,"hash_keys: length fields are: pkt %u len %u ip %un")
TRACE_MSG(RFC2507C_691_112_2_18_0_21_53_2281,"hash_keys: %s\n")
TRACE_MSG(RFC2507C_704_112_2_18_0_21_53_2282,"hash_keys: ip4 fragment\n")
TRACE_MSG(RFC2507C_716_112_2_18_0_21_53_2283,"hash_keys: ip4 options len=%u  \n")
TRACE_MSG(RFC2507C_739_112_2_18_0_21_53_2284,"hash_keys: mine hdr corrupt\n")
TRACE_MSG(RFC2507C_745_112_2_18_0_21_53_2285,"hash_keys: IPPROTO_MIN_CAP  length %u\n")
TRACE_MSG(RFC2507C_751_112_2_18_0_21_53_2286,"hash_keys: IPPROTO_MIN_CAP  next %x \n")
TRACE_MSG(RFC2507C_757_112_2_18_0_21_53_2287,"hash_keys: auth hdr corrupt\n")
TRACE_MSG(RFC2507C_762_112_2_18_0_21_53_2288,"hash_keys: IPPROTO_AH4  length %u \n")
TRACE_MSG(RFC2507C_767_112_2_18_0_21_53_2289,"hash_keys: IPPROTO_AH4  next %x \n")
TRACE_MSG(RFC2507C_773_112_2_18_0_21_53_2290,"hash_keys: unknown proto %d in ip4\n")
TRACE_MSG(RFC2507C_785_112_2_18_0_21_54_2291,"hash_keys: IP6\n")
TRACE_MSG(RFC2507C_793_112_2_18_0_21_54_2292,"hash_keys: length fields for IP6 does not match! Bailing out!\n")
TRACE_MSG(RFC2507C_816_112_2_18_0_21_54_2293,"hash_keys: extention hdr corrupt\n")
TRACE_MSG(RFC2507C_829_112_2_18_0_21_54_2294,"hash_keys: ROUTING\n")
TRACE_MSG(RFC2507C_831_112_2_18_0_21_54_2295,"hash_keys: extension hdr corrupt\n")
TRACE_MSG(RFC2507C_846_112_2_18_0_21_54_2296,"hash_keys: IPPROTO_AH6  length %u \n")
TRACE_MSG(RFC2507C_848_112_2_18_0_21_54_2297,"hash_keys: auth hdr corrupt\n")
TRACE_MSG(RFC2507C_857_112_2_18_0_21_54_2298,"hash_keys: IPPROTO_AH6  next %x \n")
TRACE_MSG(RFC2507C_874_112_2_18_0_21_54_2299,"hash_keys: FRAGMENT\n")
TRACE_MSG(RFC2507C_882_112_2_18_0_21_54_2300,"hash_keys: IPPROTO_NONE\n")
TRACE_MSG(RFC2507C_887_112_2_18_0_21_54_2301,"hash_keys: unknown proto %d in ip6_hdr\n")
TRACE_MSG(RFC2507C_899_112_2_18_0_21_54_2302,"hash_keys: ESP\n")
TRACE_MSG(RFC2507C_908_112_2_18_0_21_54_2303,"hash_keys: UDP\n")
TRACE_MSG(RFC2507C_916_112_2_18_0_21_54_2304,"hash_keys: length fields for UDP does not match! Bailing out!\n")
TRACE_MSG(RFC2507C_922_112_2_18_0_21_54_2305,"hash_keys: udp ports s %d, d %dn")
TRACE_MSG(RFC2507C_928_112_2_18_0_21_54_2306,"hash_keys: TCP\n")
TRACE_MSG(RFC2507C_930_112_2_18_0_21_54_2307,"iphc_hashkeys: TCP hdr corrupt\n")
TRACE_MSG(RFC2507C_939_112_2_18_0_21_54_2308,"hash_keys: tcp ports s %d, d %dn")
TRACE_MSG(RFC2507C_959_112_2_18_0_21_54_2309,"equal_flows: ipv = %d\n")
TRACE_MSG(RFC2507C_968_112_2_18_0_21_54_2310,"equal_flows: ip4\n")
TRACE_MSG(RFC2507C_978_112_2_18_0_21_54_2311,"equal_flows: ip4 fragment\n")
TRACE_MSG(RFC2507C_985_112_2_18_0_21_54_2312,"equal_flows: ip4 options len=%u\n")
TRACE_MSG(RFC2507C_994_112_2_18_0_21_54_2313,"equal_flows: ip4h\n")
TRACE_MSG(RFC2507C_1033_112_2_18_0_21_54_2314,"equal_flows: unknown proto %d in ip4\n")
TRACE_MSG(RFC2507C_1039_112_2_18_0_21_54_2315,"equal_flows: ip6\n")
TRACE_MSG(RFC2507C_1058_112_2_18_0_21_54_2316,"equal_flows: ip6 options i=%x t=%x\n")
TRACE_MSG(RFC2507C_1111_112_2_18_0_21_54_2317,"equal_flows: frament\n")
TRACE_MSG(RFC2507C_1118_112_2_18_0_21_54_2318,"equal_flows: none\n")
TRACE_MSG(RFC2507C_1122_112_2_18_0_21_54_2319,"equal_flows: unknown proto %d in ip6_hdr\n")
TRACE_MSG(RFC2507C_1128_112_2_18_0_21_54_2320,"equal_flows: esp\n")
TRACE_MSG(RFC2507C_1135_112_2_18_0_21_54_2321,"equal_flows: udp\n")
TRACE_MSG(RFC2507C_1139_112_2_18_0_21_54_2322,"equal_flows: udp ports s %d, d %dn")
TRACE_MSG(RFC2507C_1145_112_2_18_0_21_54_2323,"equal_flows: tcp\n")
TRACE_MSG(RFC2507C_1150_112_2_18_0_21_54_2324,"equal_flows: tcp ports s %d, d %dn")
TRACE_MSG(RFC2507C_1227_112_2_18_0_21_55_2325,"compress: LARGE_CID %u \n")
TRACE_MSG(RFC2507C_1250_112_2_18_0_21_55_2326,"compress: ip4:\n")
TRACE_MSG(RFC2507C_1258_112_2_18_0_21_55_2327,"compress: ip4 fragment\n")
TRACE_MSG(RFC2507C_1270_112_2_18_0_21_55_2328,"compress: ip4 options len=%u\n")
TRACE_MSG(RFC2507C_1299_112_2_18_0_21_55_2329,"compress: ip4h:\n")
TRACE_MSG(RFC2507C_1333_112_2_18_0_21_55_2330,"compress: unknown proto %x in ipv4\n")
TRACE_MSG(RFC2507C_1339_112_2_18_0_21_55_2331,"compress: ip6:\n")
TRACE_MSG(RFC2507C_1468_112_2_18_0_21_55_2332,"compress: proto NONE %x in ip6_hdr\n")
TRACE_MSG(RFC2507C_1472_112_2_18_0_21_55_2333,"compress: unknown proto %x in ip6_hdr\n")
TRACE_MSG(RFC2507C_1482_112_2_18_0_21_55_2334,"compress: tcp\n")
TRACE_MSG(RFC2507C_1487_112_2_18_0_21_55_2335,"compress: tcp seq %u  ack %un")
TRACE_MSG(RFC2507C_1491_112_2_18_0_21_55_2336,"compress: doing NODELTA\n")
TRACE_MSG(RFC2507C_1510_112_2_18_0_21_55_2337,"compress: header request for cid %u ch %u\n")
TRACE_MSG(RFC2507C_1530_112_2_18_0_21_55_2338,"compress: ipv4 r_byte is SET to %x  ip_ecn_save %xn")
TRACE_MSG(RFC2507C_1541_112_2_18_0_21_55_2339,"compress: ipv6 r_byte is SET to %x  ip_ecn_save %xn")
TRACE_MSG(RFC2507C_1556_112_2_18_0_21_55_2340,"compress: deltaS = %u iseq %u tseq %un")
TRACE_MSG(RFC2507C_1614_112_2_18_0_21_55_2341,"compress: NOTHING\n")
TRACE_MSG(RFC2507C_1622_112_2_18_0_21_55_2342,"compress: SPECIAL -> full\n")
TRACE_MSG(RFC2507C_1626_112_2_18_0_21_55_2343,"compress: NEW_S | NEW_A\n")
TRACE_MSG(RFC2507C_1631_112_2_18_0_21_55_2344,"compress: SPECIAL_I\n")
TRACE_MSG(RFC2507C_1636_112_2_18_0_21_55_2345,"compress: NEW_S\n")
TRACE_MSG(RFC2507C_1641_112_2_18_0_21_55_2346,"compress: SPECIAL_D deltaS = %u\n")
TRACE_MSG(RFC2507C_1658_112_2_18_0_21_55_2347,"compress: tcp options %u bytes\n")
TRACE_MSG(RFC2507C_1666_112_2_18_0_21_55_2348,"compress: TcpHc= %s\n")
TRACE_MSG(RFC2507C_1675_112_2_18_0_21_55_2349,"compress: r_byte  restore ipv4 %x  newvalue=%xn")
TRACE_MSG(RFC2507C_1679_112_2_18_0_21_55_2350,"compress: r_byte  restore ipv6 %x  newvalue=%xn")
TRACE_MSG(RFC2507C_1689_112_2_18_0_21_56_2351,"compress: done_tcp cinfo=%x \" rdp %x chdr %xn")
TRACE_MSG(RFC2507C_1738_112_2_18_0_21_56_2352,"compress: done NODELTA\n")
TRACE_MSG(RFC2507C_1746_112_2_18_0_21_56_2353,"compress: CH %s\n")
TRACE_MSG(RFC2507C_1748_112_2_18_0_21_56_2354,"compress: comp %d - %d = %dn")
TRACE_MSG(RFC2507C_1758_112_2_18_0_21_56_2355,"compress: error cinfo")
TRACE_MSG(RFC2507C_1764_112_2_18_0_21_56_2356,"compress: udp\n")
TRACE_MSG(RFC2507C_1785_112_2_18_0_21_56_2357,"compress: max_time now - last == xxx %u - %u == %u  max %un")
TRACE_MSG(RFC2507C_1804_112_2_18_0_21_56_2358,"compress: bailing out on min wrap violation (cid %u)n")
TRACE_MSG(RFC2507C_1812_112_2_18_0_21_56_2359,"compress: min wrap not violated (cid %u)\n")
TRACE_MSG(RFC2507C_1851_112_2_18_0_21_56_2360,"compress: max_time expired  sending full\n")
TRACE_MSG(RFC2507C_1860_112_2_18_0_21_56_2361,"compress: done_udp %d \" rdp %x chdr %xn")
TRACE_MSG(RFC2507C_1956_112_2_18_0_21_56_2362,"compress: CH %s\n")
TRACE_MSG(RFC2507C_1958_112_2_18_0_21_56_2363,"compress: comp %d - %d = %dn")
TRACE_MSG(RFC2507C_1965_112_2_18_0_21_56_2364,"compress: error cinfo")
TRACE_MSG(RFC2507C_1972_112_2_18_0_21_56_2365,"compress: bailing out\n")
TRACE_MSG(RFC2507C_2001_112_2_18_0_21_56_2366,"iphc_comp: length of incoming packet < IPv4 hdr\n")
TRACE_MSG(RFC2507C_2009_112_2_18_0_21_56_2367,"iphc_comp: ---------- ipv4 iphdr_len %d,  real len %dn")
TRACE_MSG(RFC2507C_2014_112_2_18_0_21_56_2368,"iphc_comp: ---------- ipv6 iphdr_len %d,  real len %dn")
TRACE_MSG(RFC2507C_2022_112_2_18_0_21_56_2369,"iphc_comp: H  %s\n")
TRACE_MSG(RFC2507C_2023_112_2_18_0_21_56_2370,"iphc_comp: H  %s\n")
TRACE_MSG(RFC2507C_2048_112_2_18_0_21_56_2371,"iphc_comp: compress => ci_regu\n")
TRACE_MSG(RFC2507C_2054_112_2_18_0_21_56_2372,"compress => unknown\n")
TRACE_MSG(RFC2507C_2079_112_2_18_0_21_56_2373,"iphc_comp: compress => ci_full\n")
TRACE_MSG(RFC2507C_2088_112_2_18_0_21_56_2374,"iphc_comp: compress => ci_comp\n")
TRACE_MSG(RFC2507C_2095_112_2_18_0_21_56_2375,"iphc_comp: compress => ci_regu\n")
TRACE_MSG(RFC2507C_2101_112_2_18_0_21_56_2376,"compress => unknown\n")
TRACE_MSG(RFC2507D_209_112_2_18_0_21_57_2377,"iphc_decompf: ----------  len=%d\n")
TRACE_MSG(RFC2507D_210_112_2_18_0_21_57_2378,"iphc_decompf: FH %s\n")
TRACE_MSG(RFC2507D_211_112_2_18_0_21_57_2379,"iphc_decompf: FH %s\n")
TRACE_MSG(RFC2507D_214_112_2_18_0_21_57_2380,"iphc_decompf: Too large packet, don't try to decompress\n")
TRACE_MSG(RFC2507D_222_112_2_18_0_21_57_2381,"iphc_decompf: length of incoming packet smaller < IPv4 hdrn")
TRACE_MSG(RFC2507D_230_112_2_18_0_21_57_2382,"iphc_decompf: ipv4\n")
TRACE_MSG(RFC2507D_244_112_2_18_0_21_57_2383,"iphc_decompf: ip6_hdr\n")
TRACE_MSG(RFC2507D_259_112_2_18_0_21_57_2384,"iphc_decompf: ip4\n")
TRACE_MSG(RFC2507D_262_112_2_18_0_21_57_2385,"iphc_decompf: IPv4 hdr corrupt\n")
TRACE_MSG(RFC2507D_267_112_2_18_0_21_57_2386,"iphc_decompf: IPv4 hdr length field too small\n")
TRACE_MSG(RFC2507D_275_112_2_18_0_21_57_2387,"iphc_decompf: ************ IP4\n")
TRACE_MSG(RFC2507D_284_112_2_18_0_21_57_2388,"iphc_decompf: ip4 fragment\n")
TRACE_MSG(RFC2507D_306_112_2_18_0_21_57_2389,"hash_keys: mine hdr corrupt\n")
TRACE_MSG(RFC2507D_318_112_2_18_0_21_57_2390,"iphc_decompf: auth in ip4_hdr\n")
TRACE_MSG(RFC2507D_320_112_2_18_0_21_57_2391,"hash_keys: auth hdr corrupt\n")
TRACE_MSG(RFC2507D_333_112_2_18_0_21_57_2392,"iphc_decompf: unknown proto %x in ip4_hdr\n")
TRACE_MSG(RFC2507D_339_112_2_18_0_21_57_2393,"iphc_decompf: ip6\n")
TRACE_MSG(RFC2507D_344_112_2_18_0_21_57_2394,"iphc_decompf: ************ IP6\n")
TRACE_MSG(RFC2507D_362_112_2_18_0_21_57_2395,"hash_keys: extention hdr corrupt\n")
TRACE_MSG(RFC2507D_374_112_2_18_0_21_58_2396,"hash_keys: auth hdr corrupt\n")
TRACE_MSG(RFC2507D_403_112_2_18_0_21_58_2397,"iphc_decompf: proto NONE %x in ip6_hdr\n")
TRACE_MSG(RFC2507D_407_112_2_18_0_21_58_2398,"iphc_decompf: unknown proto %x in ip6_hdr\n")
TRACE_MSG(RFC2507D_416_112_2_18_0_21_58_2399,"iphc_decompf: esp\n")
TRACE_MSG(RFC2507D_420_112_2_18_0_21_58_2400,"iphc_decompf: udp\n")
TRACE_MSG(RFC2507D_430_112_2_18_0_21_58_2401,"iphc_decompf: ************ UDP %u  n")
TRACE_MSG(RFC2507D_437_112_2_18_0_21_58_2402,"iphc_decompf: TCP header too small %d\n")
TRACE_MSG(RFC2507D_442_112_2_18_0_21_58_2403,"iphc_decompf: tcp\n")
TRACE_MSG(RFC2507D_447_112_2_18_0_21_58_2404,"iphc_decompf: tcp header is %d bytes longn")
TRACE_MSG(RFC2507D_450_112_2_18_0_21_58_2405,"iphc_decompf: too large tcp cid %d\n")
TRACE_MSG(RFC2507D_469_112_2_18_0_21_58_2406,"iphc_decompf: saved r_byte [TTttttII] %x\n")
TRACE_MSG(RFC2507D_475_112_2_18_0_21_58_2407,"iphc_decompf: bailing out\n")
TRACE_MSG(RFC2507D_481_112_2_18_0_21_58_2408,"decompf: LARGE CID \n")
TRACE_MSG(RFC2507D_485_112_2_18_0_21_58_2409,"decompf: LARGE CID %u\n")
TRACE_MSG(RFC2507D_490_112_2_18_0_21_58_2410,"iphc_decompf: too large tcp cid %d\n")
TRACE_MSG(RFC2507D_497_112_2_18_0_21_58_2411,"iphc_decompf: too large udp cid %d\n")
TRACE_MSG(RFC2507D_506_112_2_18_0_21_58_2412,"iphc_decompf: donefull cid %d gen %d len %d \n")
TRACE_MSG(RFC2507D_507_112_2_18_0_21_58_2413,"iphc_decompf: FH %s\n")
TRACE_MSG(RFC2507D_508_112_2_18_0_21_58_2414,"iphc_decompf: FH %s\n")
TRACE_MSG(RFC2507D_526_112_2_18_0_21_58_2415,"iphc_decompf: removed cid %u from queue\n")
TRACE_MSG(RFC2507D_571_112_2_18_0_21_58_2416,"iphc_decomp: CH %s\n")
TRACE_MSG(RFC2507D_574_112_2_18_0_21_58_2417,"iphc_decomp: Too large packet, don't try to decompress\n")
TRACE_MSG(RFC2507D_590_112_2_18_0_21_58_2418,"iphc_decomp: type TCP\n")
TRACE_MSG(RFC2507D_605_112_2_18_0_21_58_2419,"iphc_decomp: too large tcp cid %d\n")
TRACE_MSG(RFC2507D_609_112_2_18_0_21_58_2420,"iphc_decomp: tcp cid not in use %d\n")
TRACE_MSG(RFC2507D_629_112_2_18_0_21_58_2421,"iphc_decomp: type UDP\n")
TRACE_MSG(RFC2507D_633_112_2_18_0_21_58_2422,"iphc_decomp: too large udp cid %d\n")
TRACE_MSG(RFC2507D_640_112_2_18_0_21_58_2423,"iphc_decomp: udp cid not in use %d\n")
TRACE_MSG(RFC2507D_644_112_2_18_0_21_58_2424,"iphc_decomp: bad generation %u %u\n")
TRACE_MSG(RFC2507D_662_112_2_18_0_21_58_2425,"iphc_decomp: hmm... ip version %u\n")
TRACE_MSG(RFC2507D_669_112_2_18_0_21_58_2426,"iphc_decomp: IPv4 hdr length field too small\n")
TRACE_MSG(RFC2507D_672_112_2_18_0_21_58_2427,"iphc_decomp: IP4\n")
TRACE_MSG(RFC2507D_677_112_2_18_0_21_58_2428,"decomp: ip4 fragment\n")
TRACE_MSG(RFC2507D_690_112_2_18_0_21_58_2429,"iphc_decomp: IPv4 hdr length field too small\n")
TRACE_MSG(RFC2507D_713_112_2_18_0_21_58_2430,"decomp: ip4 options size changed \n")
TRACE_MSG(RFC2507D_791_112_2_18_0_21_58_2431,"iphc_decomp: unknown proto %x in ip4_hdr\n")
TRACE_MSG(RFC2507D_797_112_2_18_0_21_58_2432,"iphc_decomp: IP6\n")
TRACE_MSG(RFC2507D_862_112_2_18_0_21_59_2433,"iphc_decomp: IPPROTO_ROUTING segleft=%u n")
TRACE_MSG(RFC2507D_863_112_2_18_0_21_59_2434,"iphc_decomp:  %s\n")
TRACE_MSG(RFC2507D_867_112_2_18_0_21_59_2435,"iphc_decomp: IPPROTO_ROUTING OPTLEN=%u i=%un")
TRACE_MSG(RFC2507D_869_112_2_18_0_21_59_2436,"iphc_decomp: Bad optlen for routing option\n")
TRACE_MSG(RFC2507D_909_112_2_18_0_21_59_2437,"iphc_decomp: proto NONE  %x in ip6_hdr\n")
TRACE_MSG(RFC2507D_913_112_2_18_0_21_59_2438,"iphc_decomp: unknown proto %x in ip6_hdr\n")
TRACE_MSG(RFC2507D_923_112_2_18_0_21_59_2439,"iphc_decomp: TCP  left = %u\n")
TRACE_MSG(RFC2507D_953_112_2_18_0_21_59_2440,"iphc_decomp: r_byte *SET* to %6x\n")
TRACE_MSG(RFC2507D_956_112_2_18_0_21_59_2441,"iphc_decomp: r_byte default  %6x\n")
TRACE_MSG(RFC2507D_974_112_2_18_0_21_59_2442,"iphc_decomp: hmm... ip version %un")
TRACE_MSG(RFC2507D_987_112_2_18_0_21_59_2443,"iphc_decomp: SPECIAL_I\n")
TRACE_MSG(RFC2507D_994_112_2_18_0_21_59_2444,"iphc_decomp: SPECIAL_DATA seq = %un")
TRACE_MSG(RFC2507D_1031_112_2_18_0_21_59_2445,"iphc_decomp: NEW_O but no options in table\n")
TRACE_MSG(RFC2507D_1045_112_2_18_0_21_59_2446,"iphc_decomp: UDP space left = %u\n")
TRACE_MSG(RFC2507D_1060_112_2_18_0_21_59_2447,"iphc_decomp: bailing out\n")
TRACE_MSG(RFC2507D_1132_112_2_18_0_21_59_2448,"iphc_decomp: twice0_cksum %x   recv_cksum %xn")
TRACE_MSG(RFC2507D_1141_112_2_18_0_21_59_2449,"iphc_decomp: try twice once with delta = %xn")
TRACE_MSG(RFC2507D_1150_112_2_18_0_21_59_2450,"iphc_decomp: twice1_cksum %x   recv_cksum %xn")
TRACE_MSG(RFC2507D_1154_112_2_18_0_21_59_2451,"iphc_decomp: success with twice.\n")
TRACE_MSG(RFC2507D_1161_112_2_18_0_21_59_2452,"iphc_decomp: failed twice \n")
TRACE_MSG(RFC2507D_1175_112_2_18_0_21_59_2453,"iphc_decomp: Checksum mismatch, Twice not supported. \n")
TRACE_MSG(RFC2507D_1195_112_2_18_0_21_59_2454,"iphc_decomp: header request queued cid = %u\n")
TRACE_MSG(RFC2507D_1210_112_2_18_0_21_59_2455,"iphc_decomp: hlen %d   cs->len %d\n")
TRACE_MSG(RFC2507D_1214_112_2_18_0_21_59_2456,"iphc_decomp: cid %d  gen %d  hlen %d  comp_head_len %dn")
TRACE_MSG(RFC2507D_1215_112_2_18_0_21_59_2457,"iphc_decomp: DH %s\n")
TRACE_MSG(RFC2507D_1216_112_2_18_0_21_59_2458,"iphc_decomp: DH %s\n")
TRACE_MSG(SNDCP_PC_2629_112_2_18_0_22_6_2459,"SNDCP:pc_decompress result = %d, s_sndcp_pc0.nsapis = %d, nsapi = %d")
TRACE_MSG(SNDCP_SDL_446_112_2_18_0_22_9_2460,"SNDCP_SegNpduUnack seg_count = %d")
TRACE_MSG(SNDCP_SDL_762_112_2_18_0_22_10_2461,"SNDCP_HandleUnsequentUnackBuffer reset_flag = %d, seg_count = %d")
TRACE_MSG(SNDCP_SDL_825_112_2_18_0_22_10_2462,"SNDCP: first segment")
TRACE_MSG(SNDCP_SDL_949_112_2_18_0_22_10_2463,"SNDCP: SNDCP_HandleTllSndcUnitdataInd receiving_state = %d")
TRACE_MSG(SNDCP_SDL_1005_112_2_18_0_22_10_2464,"SNDCP: DATA-SNPDU F field set in receive subsequent state.\n")
TRACE_MSG(SNDCP_SNPDU_447_112_2_18_0_22_11_2465,"SNDCP:invalid nsapi %d")
TRACE_MSG(SNDCP_SNPDU_464_112_2_18_0_22_11_2466,"SNDCP:invalid sapi %d")
TRACE_MSG(XID_SDL_427_112_2_18_0_22_15_2467,"SNDCP: RenegotiateReq result = %d")
END_TRACE_MAP(DM_2G_PS_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _DM_2G_PS_TRC_H_

