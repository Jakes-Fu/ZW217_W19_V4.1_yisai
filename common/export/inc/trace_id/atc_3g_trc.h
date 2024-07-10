/******************************************************************************
 ** File Name:      atc_3g_trc.h                                              *
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
//trace_id:1
#ifndef _ATC_3G_TRC_H_
#define _ATC_3G_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define ATC_959_112_2_17_22_12_3_0 "ATC: write nv itemid: %d"
#define ATC_966_112_2_17_22_12_3_1 "ATC: write NV_ATC_CONFIG_ID failed."
#define ATC_974_112_2_17_22_12_3_2 "ATC:  write NV_ATC_MIDI_SIZE_ID failed."
#define ATC_1018_112_2_17_22_12_3_3 "ATC: read atc_config_nv failed"
#define ATC_1031_112_2_17_22_12_3_4 "ATC: read midi_size_arr failed"
#define ATC_1051_112_2_17_22_12_3_5 "ATC: ATC_Mux_Setup_Link, WARNING! link_id:%d"
#define ATC_1057_112_2_17_22_12_3_6 "ATC: ATC_Mux_Setup_Link, WARNING! link_id:%d is already setup"
#define ATC_1070_112_2_17_22_12_3_7 "ATC: ATC_Mux_Setup_Link, ERROR! no mem to setup link_id:%d"
#define ATC_1078_112_2_17_22_12_3_8 "ATC: ATC_Mux_Setup_Link, link_id: %d"
#define ATC_1092_112_2_17_22_12_3_9 "ATC: ATC_Mux_Release_Link, link_id: %d"
#define ATC_1155_112_2_17_22_12_4_10 "ATC: ATC_Mux_Release_Link ERROR!!!!"
#define ATC_1186_112_2_17_22_12_4_11 "ATC:ATC_Add_New_Pdp_Link ERROR!!!!"
#define ATC_1578_112_2_17_22_12_4_12 "ATC: ATC_Receive_Expected_Event, id:%d,event:%s"
#define ATC_1582_112_2_17_22_12_4_13 "ATC: ATC_Receive_Expected_Event, id:%d,event:%d"
#define ATC_1591_112_2_17_22_12_4_14 "ATC: ATC_Receive_Expected_Event, WARNING! NOT FOUND, set link id: %d"
#define ATC_1596_112_2_17_22_12_4_15 "ATC: ATC_Receive_Expected_Event, WARNING! NOT FOUND, set urc link id: %d"
#define ATC_1611_112_2_17_22_12_5_16 "ATC: ATC_Receive_Expected_Event, FOUND, ERROR! match_link_id:%d != call_link_id:%d"
#define ATC_1616_112_2_17_22_12_5_17 "ATC: ATC_Receive_Expected_Event, FOUND, WARNING! match_link_id:%d, no call_link_id, NOT ADD EARLIER?"
#define ATC_1624_112_2_17_22_12_5_18 "ATC: ATC_Receive_Expected_Event, FOUND, match_link_id:%d, call_link_id:%d"
#define ATC_1629_112_2_17_22_12_5_19 "ATC: ATC_Receive_Expected_Event, FOUND, match_link_id:%d"
#define ATC_1640_112_2_17_22_12_5_20 "ATC: ATC_Receive_Expected_Event over as follow"
#define ATC_1656_112_2_17_22_12_5_21 "ATC: ATC_Add_Expected_Event, link_id:%d, event:%s"
#define ATC_1660_112_2_17_22_12_5_22 "ATC: ATC_Add_Expected_Event, link_id:%d, event:%d"
#define ATC_1698_112_2_17_22_12_5_23 "ATC: ATC_Add_Expected_Event, unknown domain_id: %d"
#define ATC_1724_112_2_17_22_12_5_24 "ATC: ATC_Add_Expected_Event, Warning :The atc expected event buffer is full!Remove the first one for adding!"
#define ATC_2057_112_2_17_22_12_6_25 "ATC: ATC_ReadCmdLine Warning: only read %d < %d."
#define ATC_2061_112_2_17_22_12_6_26 "ATC: ATC_ReadCmdLine Warning: garbage data ???"
#define ATC_2135_112_2_17_22_12_6_27 "ATC: ATC_SendNewATInd, WARNING,s_atc_global_info_ptr is NULL."
#define ATC_2142_112_2_17_22_12_6_28 "ATC: ATC_SendNewATInd,link_id:%d,data_length:%d ,%x, %x, close:%d,count:%d,discard:%d,atc_end_ch1:%d,atc_end_ch2:%d"
#define ATC_2170_112_2_17_22_12_6_29 "ATC: ATC_SendNewATInd, is_close_atc_queue is TRUE, return"
#define ATC_2176_112_2_17_22_12_6_30 "ATC: ATC_SendNewATInd, link is not setup!!"
#define ATC_2182_112_2_17_22_12_6_31 "ATC: ATC_SendNewATInd, atc_is_echo is TRUE, send copy back"
#define ATC_2201_112_2_17_22_12_6_32 "ATC:ERROR AT BUFFER FULL  LINK %d !!!! "
#define ATC_2261_112_2_17_22_12_6_33 "ATC: ATC_Debug_SendATInd, len:%d"
#define ATC_2265_112_2_17_22_12_6_34 "ATC: ATC_Debug_SendATInd, WARNING,s_atc_global_info_ptr is NULL."
#define ATC_2292_112_2_17_22_12_6_35 "ATC: ATC_Debug_SendATInd, is_close_atc_queue is TRUE, return"
#define ATC_2311_112_2_17_22_12_6_36 "ATC:ERROR AT BUFFER FULL  LINK ATC_DEBUG_LINK!!!! "
#define ATC_2362_112_2_17_22_12_6_37 "ATC: ATC_Get_Link_Id, get call link id: %d"
#define ATC_2397_112_2_17_22_12_6_38 "ATC: ATC_Clear_All_Exp_Result id:%d,domain_id:%d"
#define ATC_2416_112_2_17_22_12_6_39 "ATC: ATC_Clear_All_Exp_Result, cid:%d, exp event: %s"
#define ATC_2421_112_2_17_22_12_6_40 "ATC: ATC_Clear_All_Exp_Result, cid:%d, exp event: %d"
#define ATC_2461_112_2_17_22_12_6_41 "ATC: ATC_Clear_All_Exp_Result, call_id:%d, exp event: %d"
#define ATC_2519_112_2_17_22_12_7_42 "|---|-------|-----|-----|-------|------------------------------"
#define ATC_2520_112_2_17_22_12_7_43 "|Idx|Link-ID| SIM | CID |Call-ID| Event"
#define ATC_2521_112_2_17_22_12_7_44 "|---|-------|-----|-----|-------|------------------------------"
#define ATC_2534_112_2_17_22_12_7_45 "|%-3d|  %d    |   %d   | %-3d |  %-3d  | %s"
#define ATC_2543_112_2_17_22_12_7_46 "|%-3d|  %d    |   %d   | %-3d |  %-3d  | %d"
#define ATC_2554_112_2_17_22_12_7_47 "|---|-------|-----|-------|------------------------------"
#define ATC_2557_112_2_17_22_12_7_48 "Total:%d"
#define ATC_2570_112_2_17_22_12_7_49 "ATC: SendNewATInd, len:%d"
#define ATC_2574_112_2_17_22_12_7_50 "ATC: SendNewATInd, WARNING,s_atc_global_info_ptr is NULL."
#define ATC_2600_112_2_17_22_12_7_51 "ATC: SendNewATInd, is_close_atc_queue is TRUE"
#define ATC_2639_112_2_17_22_12_7_52 "ATC: ATC_Create_PSD_Call,PSD_CALL,cid:%d,link_id:%d,%d,data_mode:%d,state:%d,init_mod_state:%d"
#define ATC_2652_112_2_17_22_12_7_53 "ATC: ATC_Create_PSD_Call,PSD_CALL,ERROR,cid:%d,(!)link_id:%d-->%d,data_mode:%d,(!)state:%d"
#define ATC_2662_112_2_17_22_12_7_54 "ATC: ATC_Create_PSD_Call,PSD_CALL,ERROR,cid:%d,link_id:%d,(!)data_mode:%d-->%d,(!)state:%d"
#define ATC_2671_112_2_17_22_12_7_55 "ATC: ATC_Create_PSD_Call,PSD_CALL,WARNING,cid:%d->%d,link_id:%d,%d->%d,%d,data_mode:%d->%d,(!)state:%d"
#define ATC_2684_112_2_17_22_12_7_56 "ATC: ATC_Create_PSD_Call,PSD_CALL,cid:%d is not in range"
#define ATC_2707_112_2_17_22_12_7_57 "ATC: ATC_Set_PSD_Call_State,PSD_CALL,op-cid:%d,cid:%d,link_id:%d,%d,data_mode:%d,state:%d->%d"
#define ATC_2713_112_2_17_22_12_7_58 "ATC: ATC_Set_PSD_Call_State,PSD_CALL,cid:%d is not in range"
#define ATC_2736_112_2_17_22_12_7_59 "ATC: ATC_Clear_PSD_Call_Info,PSD_CALL,op-cid:%d,cid:%d,link_id:%d,%d,data_mode:%d,state:%d,init_mod_state:%d"
#define ATC_2752_112_2_17_22_12_7_60 "ATC: ATC_Clear_PSD_Call_Info,PSD_CALL,cid:%d is not in range"
#define ATC_2775_112_2_17_22_12_7_61 "ATC: ATC_List_PSD_Call_Info,PSD_CALL,op-cid:%d,cid:%d,link_id:%d,%d,data_mode:%d,state:%d,init_mod_state:%d"
#define ATC_2859_112_2_17_22_12_7_62 "ATC: CMD COUNT ERROR %d, SigCode=%d"
#define ATC_2874_112_2_17_22_12_7_63 "ATC: Error atc_info_ptr = %x"
#define ATC_2897_112_2_17_22_12_7_64 "ATC: ATC_Task, Signal Code: ATC_MUX_LINK_SETUP_REQ, link_id:%d"
#define ATC_2902_112_2_17_22_12_7_65 "ATC: ATC_Task, Signal Code: ATC_MUX_LINK_RELEASE_REQ, link_id:%d"
#define ATC_2910_112_2_17_22_12_8_66 "ATC: ATC_Task, Signal Code: ATC_MUX_CLOSE_REQ"
#define ATC_2917_112_2_17_22_12_8_67 "ATC: ATC_Task, Signal Code: ATC_MUX_RECV_NEW_AT, link_id:%d,len:%d,close:%d,count:%d"
#define ATC_2925_112_2_17_22_12_8_68 "ATC: buffered_at_cmd_count is zero in close  "
#define ATC_2945_112_2_17_22_12_8_69 "ATC: buffered_at_cmd_count is zero in NOT close "
#define ATC_3001_112_2_17_22_12_8_70 "ATC: ATC_Task, Signal Code: SIO_RECV_NEW_AT"
#define ATC_3109_112_2_17_22_12_8_71 "ATC: ATC_Task, Signal Code: APP_MN_ACTIVATE_PDP_CONTEXT_CNF"
#define ATC_3126_112_2_17_22_12_8_72 "ATC: ATC_Task, Signal Code: APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, result:%d,pdp_num:%d,cid:%d, set atc_err_protected_flag"
#define ATC_3203_112_2_17_22_12_8_73 "ATC: APP_MN_GPRS_DETACH_RESULT, set atc_err_protected_flag"
#define ATC_3290_112_2_17_22_12_8_74 "ATC: ATC_Task, auto answer call, id:%d,type:%d"
#define ATC_3402_112_2_17_22_12_9_75 "ATC: ATC_Task, Signal Code: APP_MN_CALL_DISCONNECTED_IND, set atc_err_protected_flag"
#define ATC_3415_112_2_17_22_12_9_76 "ATC: ATC_Task, disconnect VT call, destroy mux link, call_id:%d"
#define ATC_3420_112_2_17_22_12_9_77 "ATC: ATC_Task, APP_MN_CALL_DISCONNECTED_IND, disconnect VT mux link, call_id:%d, cause:%d"
#define ATC_3449_112_2_17_22_12_9_78 "ATC: ATC_Task, APP_MN_CALL_ERR_IND, disconnect VT mux link, call_id:%d, cause:%d"
#define ATC_3462_112_2_17_22_12_9_79 "ATC: ATC_Task, Signal Code: APP_MN_CALL_DISCONNECTED_IND, set atc_err_protected_flag"
#define ATC_3497_112_2_17_22_12_9_80 "ATC:call ATC_ProcessReadDSPCnf"
#define ATC_3505_112_2_17_22_12_9_81 "ATC: APP_MN_DEACTIVE_PS_CNF, set atc_err_protected_flag"
#define ATC_3636_112_2_17_22_12_9_82 "ATC, WARNING, APP_MN_READ_SMS_CNF is not for the request of ATC"
#define ATC_3673_112_2_17_22_12_9_83 "ATC, WARNING, APP_MN_SEND_SMS_CNF is not for the request of ATC"
#define ATC_3703_112_2_17_22_12_9_84 "ATC, WARNING, APP_MN_WRITE_SMS_CNF is not for the request of ATC"
#define ATC_3719_112_2_17_22_12_9_85 "ATC, WARNING, APP_MN_UPDATE_SMS_STATE_CNF is not for the request of ATC"
#define ATC_3735_112_2_17_22_12_9_86 "ATC, WARNING, APP_MN_DELETE_ALL_SMS_CNF is not for the request of ATC"
#define ATC_3780_112_2_17_22_12_9_87 "ATC:APP_MN_DEACTIVATE_SS_CNF"
#define ATC_3802_112_2_17_22_12_10_88 "ATC:APP_MN_ACTIVATE_SS_CNF"
#define ATC_3874_112_2_17_22_12_10_89 "ATC:call ATC_ProcessCusdInd"
#define ATC_3882_112_2_17_22_12_10_90 "ATC:call ATC_ProcessCusdInd"
#define ATC_3890_112_2_17_22_12_10_91 "ATC:call ATC_ProcessCusdInd"
#define ATC_3919_112_2_17_22_12_10_92 "ATC:APP_MN_GET_PASSWORD_IND"
#define ATC_3926_112_2_17_22_12_10_93 "ATC:APP_MN_REGISTER_PASSWORD_CNF"
#define ATC_3958_112_2_17_22_12_10_94 "Eddie.Wang ******** SIM is ready, not busy, start power on PS if needed"
#define ATC_3962_112_2_17_22_12_10_95 "Eddie.Wang ******** sim post power on after sim ready!!"
#define ATC_4034_112_2_17_22_12_10_96 "ATC: arrive at ATC_TASK APP_MN_CALL_READY_IND"
#define ATC_4038_112_2_17_22_12_10_97 "ATC: arrive at ATC_TASK APP_MN_CALL_READY_IND is_fdn_enable=%d"
#define ATC_4041_112_2_17_22_12_10_98 "ATC: arrive at ATC_TASK APP_MN_CALL_READY_IND is_fdn_enable=%d"
#define ATC_4051_112_2_17_22_12_10_99 "ATC: arrive at ATC_TASK APP_MN_FDN_SERVICE_IND"
#define ATC_4053_112_2_17_22_12_10_100 "ATC: arrive at ATC_TASK APP_MN_FDN_SERVICE_IND is_fdn_enable=%d"
#define ATC_4060_112_2_17_22_12_10_101 "ATC: arrive at ATC_TASK APP_MN_FDN_SERVICE_IND is_fdn_enable=%d"
#define ATC_4127_112_2_17_22_12_10_102 "ATC:call ATC_ProcessCusdInd"
#define ATC_4142_112_2_17_22_12_10_103 "ATC:ATC_STSF_EXP_IND"
#define ATC_4232_112_2_17_22_12_10_104 "ATC:call ATC_ProcessSetBandCnf"
#define ATC_4245_112_2_17_22_12_10_105 "ATC: APP_MN_PS_POWER_OFF_CNF: reset_flag=%d."
#define ATC_4297_112_2_17_22_12_10_106 "ATC: cfun_flag:%d,APP_MN_PS_POWER_ON_CNF,URC:+SIND: 8"
#define ATC_4370_112_2_17_22_12_11_107 "Eddie.Wang ******** sim is powered on, not busy!!!sim_flag[g_current_card_id]=%d, need ps power on?%d"
#define ATC_4413_112_2_17_22_12_11_108 "ATC: cfun_flag:%d,APP_MN_SIM_POWER_OFF_CNF,URC:+SIND: 8"
#define ATC_4518_112_2_17_22_12_11_109 "ATC: rcv atcmd rsp,dlci:%d,info:%s"
#define ATC_4543_112_2_17_22_12_11_110 "ATC: rcv atcmd result,dlci:%d,info:%s"
#define ATC_4567_112_2_17_22_12_11_111 "ATC: ATC_Task: Unknown Signal Code (%d)!"
#define ATC_4589_112_2_17_22_12_11_112 "ATC: ATC_Initialize failed in ATCM_CreateHeap."
#define ATC_4600_112_2_17_22_12_11_113 "ATC: ATC_Initialize failed in SCI_ALLOC for s_atc_global_info_ptr."
#define ATC_4773_112_2_17_22_12_11_114 "ATC: ATC_RecNewLineSig,link_id:%d,len:%d"
#define ATC_4833_112_2_17_22_12_12_115 "ATC: backspace? len: %d->%d"
#define ATC_4850_112_2_17_22_12_12_116 "ATC: ATC_RecNewLineSig, len:%d,hex:%s,line:%s"
#define ATC_4855_112_2_17_22_12_12_117 "ATC: ATC_RecNewLineSig, len:%d"
#define ATC_5028_112_2_17_22_12_12_118 "ATC: ATC_RecNewLineSig, ignore ip data, len:%d->%d, new atcmd:%s"
#define ATC_5055_112_2_17_22_12_12_119 "ATC: ATC_RecNewLineSig, Assert Failure sharp len too long."
#define ATC_5062_112_2_17_22_12_12_120 "ATC: ATC_RecNewLineSig, Call the external function"
#define ATC_5083_112_2_17_22_12_12_121 "ATC: ATC_RecNewLineSig, Parse AT cmd line success."
#define ATC_5107_112_2_17_22_12_12_122 "ATC: ATC_RecNewLineSig, Parse SMS PDU successful"
#define ATC_5111_112_2_17_22_12_12_123 "ATC: ATC_RecNewLineSig, Parse AT command line failed."
#define ATC_5293_112_2_17_22_12_12_124 "ATC: ATC_RecNewLineSig, len:%d,hex:%s,line:%s"
#define ATC_5298_112_2_17_22_12_12_125 "ATC: ATC_RecNewLineSig, len:%d"
#define ATC_5465_112_2_17_22_12_13_126 "ATC: ATC_RecNewLineSig, ignore ip data, len:%d->%d, new atcmd:%s"
#define ATC_5491_112_2_17_22_12_13_127 "ATC: ATC_RecNewLineSig, Assert Failure sharp len too long."
#define ATC_5498_112_2_17_22_12_13_128 "ATC: ATC_RecNewLineSig, Call the external function"
#define ATC_5519_112_2_17_22_12_13_129 "ATC: ATC_RecNewLineSig, Parse AT cmd line success."
#define ATC_5543_112_2_17_22_12_13_130 "ATC: ATC_RecNewLineSig, Parse SMS PDU successful"
#define ATC_5547_112_2_17_22_12_13_131 "ATC: ATC_RecNewLineSig, Parse AT command line failed."
#define ATC_5639_112_2_17_22_12_13_132 "ATC: ATC_DespatchCmd, cmd_type:%d(1-exe,2-set,4-read,8-test)"
#define ATC_5646_112_2_17_22_12_13_133 "ATC: ATC_DespatchCmd, sim_flag[g_current_card_id]:%d, cpin:%d"
#define ATC_5752_112_2_17_22_12_13_134 "ATC: ATC_DespatchCmd, %s Test Command: Default Response"
#define ATC_5915_112_2_17_22_12_14_135 "ATC: ATC_CheckCmd,Check AT Command valid."
#define ATC_5919_112_2_17_22_12_14_136 "ATC: ATC_CheckCmd,Check AT Command invalid."
#define ATC_5954_112_2_17_22_12_14_137 "ATC: filter state, not report unsolicited code."
#define ATC_5967_112_2_17_22_12_14_138 "ATC: ATC_BuildResultCodeRsp, err text string too long"
#define ATC_6010_112_2_17_22_12_14_139 "ATC: ATC_BuildResultCodeRsp, q_value is TRUE"
#define ATC_6070_112_2_17_22_12_14_140 "ATC: ATC_BuildResultCodeRsp,The sent string is %s"
#define ATC_6138_112_2_17_22_12_14_141 "ATC: ATC_BuildResultErrCodeRsp, q_value is TRUE"
#define ATC_6157_112_2_17_22_12_14_142 "ATC: ATC_BuildResultErrCodeRsp,The sent string is %s"
#define ATC_6249_112_2_17_22_12_14_143 "ATC: ATC_BuildSmsCodeRsp,The sent string is %s"
#define ATC_6311_112_2_17_22_12_15_144 "ATC: filter state, not report unsolicited code: %s."
#define ATC_6326_112_2_17_22_12_15_145 "ATC: ATC_BuildInfoRsp, q_value is TRUE"
#define ATC_6332_112_2_17_22_12_15_146 "ATC: ATC_BuildInfoRsp, rsp_str_ptr's length is zero"
#define ATC_6386_112_2_17_22_12_15_147 "ATC: ATC_BuildInfoRsp,The sent string is %s"
#define ATC_6491_112_2_17_22_12_15_148 "ATC: ATC_BuildTestCmdResponse, q_value is TRUE"
#define ATC_6575_112_2_17_22_12_15_149 "ATC:ATC_BuildTestCmdResponse,The sent string is %s"
#define ATC_6915_112_2_17_22_12_16_150 "ATC: ATC_InitConfig cfun_flag %d "
#define ATC_7130_112_2_17_22_12_16_151 "ATC:SCI_RegisterMsg,STK"
#define ATC_7203_112_2_17_22_12_16_152 "ATC: ATC_DisconnectModem "
#define ATC_7212_112_2_17_22_12_16_153 "ATC: ATC_DisconnectModem,ERROR,s_atc_global_info_ptr is NULL."
#define ATC_7218_112_2_17_22_12_16_154 "ATC: ATC_DisconnectModem, illegal cid:%d, not in range"
#define ATC_7227_112_2_17_22_12_16_155 "ATC: ATC_DisconnectModem be invoked for more times, cid:%d is already inactive!!!"
#define ATC_7248_112_2_17_22_12_16_156 "ATC: ATC_DisconnectModem, no call(cid:%d) link id! PSD is not existed!"
#define ATC_7254_112_2_17_22_12_16_157 "ATC: ATC_DisconnectModem, failure in MNGPRS_DeactivatePdpContext"
#define ATC_7275_112_2_17_22_12_16_158 "ATC: ATC_DisconnectModem, no call(cid:%d) link id! PSD is not existed!"
#define ATC_7281_112_2_17_22_12_16_159 "ATC: ATC_DisconnectModem, failure in MNGPRS_DeactivatePdpContext"
#define ATC_7316_112_2_17_22_12_17_160 "ATC: ATC_DisconnectModem,ERROR,s_atc_global_info_ptr is NULL."
#define ATC_7392_112_2_17_22_12_17_161 "ATC: Assert Failure unknow chr signal."
#define ATC_7497_112_2_17_22_12_17_162 "ATC: ATHEventHandle,ev:%d,ERROR!!! link_id:%d not in range!"
#define ATC_7518_112_2_17_22_12_17_163 "ATC: ATHEventHandle,NEW_PDP(cid:%d),ERROR:link_id:%d,cid:%d,ath_stat:(!)LISTENING"
#define ATC_7524_112_2_17_22_12_17_164 "ATC: ATHEventHandle,NEW_PDP(cid:%d),Reset,link_id:%d,cid:%d->INVALID,ath_stat:WAITING->NULL"
#define ATC_7535_112_2_17_22_12_17_165 "ATC: ATHEventHandle,NEW_PDP(cid:%d),Reset,link_id:%d,cid:%d->INVALID,ath_stat:AVOID_REPETITION->NULL"
#define ATC_7548_112_2_17_22_12_17_166 "ATC: ATHEventHandle,NEW_PPP(cid:%d),link_id:%d,cid:%d->%d,ath_stat:NULL->LISTENING"
#define ATC_7563_112_2_17_22_12_17_167 "ATC: ATHEventHandle,NEW_CS,ERROR:link_id:%d,cid:%d,ath_stat:%d==LISTENING"
#define ATC_7569_112_2_17_22_12_17_168 "ATC: ATHEventHandle,NEW_CS,link_id:%d,cid:%d,ath_stat:%d"
#define ATC_7582_112_2_17_22_12_17_169 "ATC: ATHEventHandle,NEW_CS,link_id:%d,cid:%d,ath_stat:%d"
#define ATC_7598_112_2_17_22_12_17_170 "ATC: ATHEventHandle,RECV_ATH,link_id:%d,cid:%d,ath_stat:LISTENING->AVOID_REPETITION"
#define ATC_7636_112_2_17_22_12_17_171 "ATC: ATHEventHandle,RECV_ATH,link_id:%d,change [%d] expected event: ACT->DEACT"
#define ATC_7649_112_2_17_22_12_17_172 "ATC: ATHEventHandle,RECV_ATH,add deact expected event. cid:%d"
#define ATC_7667_112_2_17_22_12_17_173 "ATC: ATHEventHandle,RECV_ATH,link_id:%d,cid:%d,ath_stat:WAITING->AVOID_REPETITION"
#define ATC_7683_112_2_17_22_12_17_174 "ATC: ATHEventHandle,RECV_ATH,link_id:%d,cid:%d,ath_stat:AVOID_REPETITION"
#define ATC_7698_112_2_17_22_12_17_175 "ATC: ATHEventHandle,ACT_REJ,link_id:%d,cid:%d,ath_stat:LISTENING->WAITING"
#define ATC_7713_112_2_17_22_12_17_176 "ATC: ATHEventHandle,ACT_REJ,ERROR:link_id:%d,cid:%d,%d,ath_stat:%d"
#define ATC_7734_112_2_17_22_12_17_177 "ATC: ATHEventHandle,DEACT,link_id:%d,cid:%d,ath_stat:LISTENING->WAITING"
#define ATC_7757_112_2_17_22_12_17_178 "ATC: ATHEventHandle,DEACT,ERROR:link_id:%d,cid:%d,%d,ath_stat:%d"
#define ATC_7768_112_2_17_22_12_17_179 "ATC: ATHEventHandle,WAIT_TIMEOUT,link_id:%d,cid:%d,ath_stat:WAITING->NULL"
#define ATC_7779_112_2_17_22_12_17_180 "ATC: ATHEventHandle,AVOID_REP_TIMEOUT,link_id:%d,cid:%d,ath_stat:AVOID_REPETITION->NULL"
#define ATC_7787_112_2_17_22_12_17_181 "ATC: ATHEventHandle,unknown event:%d,link_id:%d"
#define ATC_7804_112_2_17_22_12_18_182 "ATC: ATC_Link_Property invalid link id: %d."
#define ATC_7825_112_2_17_22_12_18_183 "ATC: ATC_Link_Is_Existed_PSD invalid link id: %d."
#define ATC_7899_112_2_17_22_12_18_184 "ATC: get call state error"
#define ATC_7914_112_2_17_22_12_18_185 "ATC: ATC_SynchronizeService, dual_sys= %d"
#define ATC_7922_112_2_17_22_12_18_186 "ATC: SynchronizeService, ERROR!!! status: %d, MN_SS_CFU"
#define ATC_7934_112_2_17_22_12_18_187 "ATC: SynchronizeService, ERROR!!! status: %d, MN_SS_CFB"
#define ATC_7946_112_2_17_22_12_18_188 "ATC: SynchronizeService, ERROR!!! status: %d, MN_SS_CFNRY"
#define ATC_7958_112_2_17_22_12_18_189 "ATC: SynchronizeService, ERROR!!! status: %d, MN_SS_CFNRC"
#define ATC_7976_112_2_17_22_12_18_190 "ATC: SynchronizeService, status: %d->%d"
#define ATC_7988_112_2_17_22_12_18_191 "ATC: SynchronizeService, finished! cfall:%d,cfu:%d,cfcond:%d,cfb:%d,cfnrc:%d,cfnry:%d"
#define ATC_8013_112_2_17_22_12_18_192 "ATC: ATC_ProcessEsqoptTimeout,+ECSQ rxlev=%d,rxqual=%d"
#define ATC_8030_112_2_17_22_12_18_193 "ATC: +ECSQ deactivate timer first while time out"
#define ATC_8061_112_2_17_22_12_18_194 "ATC: ATC_Get_Data_Link_Id: invalid cid for MUX max"
#define ATC_8415_112_2_17_22_12_19_195 "ATC: ATC_Find_Expected_Event find event: %s, number: %d"
#define ATC_8419_112_2_17_22_12_19_196 "ATC: ATC_Find_Expected_Event find event: %d, number: %d"
#define ATC_8448_112_2_17_22_12_19_197 "ATC: ATC_UpDSCIInfo  dsci_flag: %d, expected_event: %s , cardid: %d"
#define ATC_8453_112_2_17_22_12_19_198 "ATC: ATC_UpDSCIInfo  dsci_flag: %d, expected_event: %d, cardid: %d"
#define ATC_8513_112_2_17_22_12_19_199 "ATC: ATC_UpSINDInfo, IndLevel: %d, atc_wind_bit_flag: %d, upValue: %d, UpFlag:%d"
#define ATC_8772_112_2_17_22_12_19_200 "ATC: UP +CREG, mode:%d,unsolicited flag:%d,oper_status:%d"
#define ATC_8798_112_2_17_22_12_20_201 "ATC: ATC_UpCREGInfo, unknow creg enum:%d."
#define ATC_8832_112_2_17_22_12_20_202 "ATC: ATC_UpCREGInfo, unknow creg enum:%d."
#define ATC_8857_112_2_17_22_12_20_203 "ATC: UP +CGREG, mode:%d,unsolicited flag:%d,gprs_oper_status:%d"
#define ATC_8886_112_2_17_22_12_20_204 "ATC: ATC_UpCGREGInfo, unknow cgreg enum:%d."
#define ATC_8921_112_2_17_22_12_20_205 "ATC: ATC_UpCGREGInfo, unknow cgreg enum:%d."
#define ATC_8960_112_2_17_22_12_20_206 "ATC: ATC_UpCENDInfo, invalid signal code:%d."
#define ATC_9364_112_2_17_22_12_21_207 "ATC_ATC_MsgIsRelatedToSim: event not checked return FALSE"
#define ATC_9502_112_2_17_22_12_21_208 "ATC: filter state, not report unsolicited code: %s."
#define ATC_9520_112_2_17_22_12_21_209 "ATC: ATC_BuildUnsolicitedInfo, q_value is TRUE"
#define ATC_9526_112_2_17_22_12_21_210 "ATC: ATC_BuildUnsolicitedInfo, rsp_str_ptr's length is zero"
#define ATC_9579_112_2_17_22_12_21_211 "ATC: ATC_BuildUnsolicitedInfo,1"
#define ATC_9586_112_2_17_22_12_21_212 "ATC: ATC_BuildUnsolicitedInfo,2"
#define ATC_9595_112_2_17_22_12_21_213 "ATC: ATC_BuildUnsolicitedInfo,3"
#define ATC_9603_112_2_17_22_12_21_214 "ATC: ATC_BuildUnsolicitedInfo,4"
#define ATC_BASIC_CMD_477_112_2_17_22_12_25_215 "ATC ATC_ProcessV the param num is %d, invalid!"
#define ATC_BASIC_CMD_487_112_2_17_22_12_25_216 "ATC: ATC_ProcessV EXE: cur_param_num:%d"
#define ATC_BASIC_CMD_493_112_2_17_22_12_25_217 "ATC: ATC_ProcessV EXE: PARAM1_FLAG, %d"
#define ATC_BASIC_CMD_496_112_2_17_22_12_25_218 "ATC: ATC_ProcessV the param num is %d, invalid!"
#define ATC_BASIC_CMD_737_112_2_17_22_12_25_219 "ATC: ATC_ProcessCREG, plmn_status:%d,rat:%d,cell_cap:%d%d%d%d"
#define ATC_BASIC_CMD_847_112_2_17_22_12_26_220 "ATC: ATC_ProcessCSQ,+CSQ rxlev=%d,rxqual=%d"
#define ATC_BASIC_CMD_884_112_2_17_22_12_26_221 "ATC: +ESQOPT  deactivate timer while active"
#define ATC_BASIC_CMD_894_112_2_17_22_12_26_222 "ATC: +ESQOPT , use defaul value 10!"
#define ATC_BASIC_CMD_905_112_2_17_22_12_26_223 "ATC: +ESQOPT  ATC_ESQOPT_TIMER_INTERVAL!"
#define ATC_BASIC_CMD_911_112_2_17_22_12_26_224 "ATC: +ESQOPT  ATC_ESQOPT_RSSI_THRESHOLD!"
#define ATC_BASIC_CMD_918_112_2_17_22_12_26_225 "ATC: +ESQOPT deactivate timer first while active"
#define ATC_BASIC_CMD_931_112_2_17_22_12_26_226 "ATC: +ESQOPT activate timer"
#define ATC_BASIC_CMD_939_112_2_17_22_12_26_227 "ATC: +ESQOPT option=%d,timer_value=%d,thres_value=%d"
#define ATC_BASIC_CMD_947_112_2_17_22_12_26_228 "ATC: ATC_ProcessESQOPT, +ECSQ rxlev=%d,rxqual=%d"
#define ATC_BASIC_CMD_1401_112_2_17_22_12_27_229 "ATC: +CGMR: %s"
#define ATC_BASIC_CMD_1544_112_2_17_22_12_27_230 "ATC: Assert Failure pin len error."
#define ATC_BASIC_CMD_1568_112_2_17_22_12_27_231 "ATC: Assert Failure pin length error."
#define ATC_BASIC_CMD_1591_112_2_17_22_12_27_232 "ATC:sim_flag=%d,cpin=%d,sim_ready[g_current_card_id]=%d|%d"
#define ATC_BASIC_CMD_1610_112_2_17_22_12_27_233 "ATC:ATC_ProcessCPIN,  APP_MN_SIM_READY_IND"
#define ATC_BASIC_CMD_1616_112_2_17_22_12_27_234 "ATC:ATC_ProcessCPIN,  APP_MN_SIM_PIN_FUNC_CNF"
#define ATC_BASIC_CMD_1624_112_2_17_22_12_27_235 "ATC: ATC_ProcessCPIN, PIN1,failure "
#define ATC_BASIC_CMD_1652_112_2_17_22_12_27_236 "ATC: ATC_ProcessCPIN, PIN2, failure "
#define ATC_BASIC_CMD_1701_112_2_17_22_12_28_237 "ATC: ATC_ProcessCPIN, sim_flag:TRUE, PUK1, failure "
#define ATC_BASIC_CMD_1722_112_2_17_22_12_28_238 "ATC: ATC_ProcessCPIN, sim_flag:TRUE,PUK1, failure in MNPHONE_ResponsePinEx"
#define ATC_BASIC_CMD_1769_112_2_17_22_12_28_239 "ATC: ATC_ProcessCPIN, PUK2, failure in MNPHONE_ResponsePinEx"
#define ATC_BASIC_CMD_1807_112_2_17_22_12_28_240 "ATC: Assert Failure unknow cpin state %d"
#define ATC_BASIC_CMD_1817_112_2_17_22_12_28_241 "ATC: Assert Failure cpin state not allowed."
#define ATC_BASIC_CMD_1876_112_2_17_22_12_28_242 "ATC: ecpin2 the param is error"
#define ATC_BASIC_CMD_1890_112_2_17_22_12_28_243 "ATC: Assert Failure pin len error."
#define ATC_BASIC_CMD_1918_112_2_17_22_12_28_244 "ATC: Assert Failure pin length error."
#define ATC_BASIC_CMD_1952_112_2_17_22_12_28_245 "ATC: ATC_ProcessCPIN, PIN2, failure in MNPHONE_OperatePinEx"
#define ATC_BASIC_CMD_1995_112_2_17_22_12_28_246 "ATC: ATC_ProcessCPIN, PIN2, failure in MNPHONE_ResponsePinEx"
#define ATC_BASIC_CMD_2050_112_2_17_22_12_29_247 "ATC: Assert Failure unknow ecpin2 state %d"
#define ATC_BASIC_CMD_2097_112_2_17_22_12_29_248 "ATC: unknow pin type %d."
#define ATC_BASIC_CMD_2141_112_2_17_22_12_29_249 "ATC:ATC_ProcessCIMI,imsi_len=%d"
#define ATC_BASIC_CMD_2388_112_2_17_22_12_29_250 "ATC: receive the APP_MN_SYNC_IND signal when no call in outgoing state"
#define ATC_BASIC_CMD_2397_112_2_17_22_12_29_251 "ATC: receive the APP_MN_SYNC_IND signal"
#define ATC_BASIC_CMD_2437_112_2_17_22_12_29_252 "ATC: ATC_ProcessMnSyncInd is_emergency_call:%d, call_amount: %d"
#define ATC_BASIC_CMD_2537_112_2_17_22_12_30_253 "ATC: ATC_ProcessNetworkStatusInd, plmn_status:%d,rat:%d,cell_cap:%d%d%d%d"
#define ATC_BASIC_CMD_2925_112_2_17_22_12_31_254 "ATC: ATC_ProcessCPBW, param1 is not string!"
#define ATC_BASIC_CMD_2930_112_2_17_22_12_31_255 "ATC: $$$ the input  is %s"
#define ATC_BASIC_CMD_2935_112_2_17_22_12_31_256 "ATC: $$ set timer success"
#define ATC_BASIC_CMD_2940_112_2_17_22_12_31_257 "ATC:  set timer fail"
#define ATC_BASIC_CMD_2947_112_2_17_22_12_31_258 "ATC:  the parameter check fail"
#define ATC_BASIC_CMD_2973_112_2_17_22_12_31_259 "ATC: $$$ the status is %d"
#define ATC_BASIC_CMD_3208_112_2_17_22_12_31_260 "ATC:param_len = %d"
#define ATC_BASIC_CMD_3223_112_2_17_22_12_31_261 "ATC:AT+SSEA,audio_data.UL_equalizer_coeff[%d]=%x"
#define ATC_BASIC_CMD_3229_112_2_17_22_12_31_262 "ATC:AT+SSEA,audio_data.DL_equalizer_coeff[%d]=%x"
#define ATC_BASIC_CMD_3233_112_2_17_22_12_31_263 "ATC:audio_data.DL_equalizer_coeff=%x,%x,%x"
#define ATC_BASIC_CMD_3235_112_2_17_22_12_31_264 "ATC:audio_data.UL_PGA_gain=%x"
#define ATC_BASIC_CMD_3238_112_2_17_22_12_31_265 "ATC:audio_data.DL_PGA_gain=%x"
#define ATC_BASIC_CMD_3241_112_2_17_22_12_31_266 "ATC:audio_data.UL_digital_gain=%x"
#define ATC_BASIC_CMD_3244_112_2_17_22_12_31_267 "ATC:audio_data.UL_digital_gain=%x"
#define ATC_BASIC_CMD_3247_112_2_17_22_12_31_268 "ATC:audio_data.UL_digital_scale=%x"
#define ATC_BASIC_CMD_3250_112_2_17_22_12_31_269 "ATC:audio_data.DL_digital_scale=%x"
#define ATC_BASIC_CMD_3253_112_2_17_22_12_31_270 "ATC:audio_data.midi_PGA_gain_base=%x"
#define ATC_BASIC_CMD_3256_112_2_17_22_12_31_271 "ATC:audio_data.digital_sidetone_gain=%x"
#define ATC_BASIC_CMD_3261_112_2_17_22_12_31_272 "ATC:audio_data.DL_IIR1_coeff[%d]=%x"
#define ATC_BASIC_CMD_3267_112_2_17_22_12_31_273 "ATC:audio_data.DL_IIR2_coeff[%d]=%x"
#define ATC_BASIC_CMD_3279_112_2_17_22_12_31_274 "ATC:flag = %d"
#define ATC_BASIC_CMD_3386_112_2_17_22_12_32_275 "ATC: bat capacity %d."
#define ATC_BASIC_CMD_3504_112_2_17_22_12_32_276 "ATC: call MNPHONE_GetMeasReportEx"
#define ATC_BASIC_CMD_3510_112_2_17_22_12_32_277 "ATC: get ncell info succ"
#define ATC_BASIC_CMD_3528_112_2_17_22_12_32_278 "ATC: the index: %d"
#define ATC_BASIC_CMD_3553_112_2_17_22_12_32_279 "ATC: the string length is %d"
#define ATC_BASIC_CMD_3561_112_2_17_22_12_32_280 "ATC: Set csq false"
#define ATC_BASIC_CMD_3644_112_2_17_22_12_32_281 "ATC: Enter the function of ATC_ProcessCCED()"
#define ATC_BASIC_CMD_3666_112_2_17_22_12_32_282 "ATC: Set csq true"
#define ATC_BASIC_CMD_4078_112_2_17_22_12_33_283 "ATC: The remain size is %d"
#define ATC_BASIC_CMD_4107_112_2_17_22_12_33_284 "ATC: create a new file"
#define ATC_BASIC_CMD_4150_112_2_17_22_12_33_285 "ATC: add to the present file"
#define ATC_BASIC_CMD_4187_112_2_17_22_12_33_286 "ATC: The parameters is error for the midi files"
#define ATC_BASIC_CMD_4210_112_2_17_22_12_33_287 "ATC: The parameters is error for the midi files"
#define ATC_BASIC_CMD_4263_112_2_17_22_12_33_288 "ATC: SIO string lenght=%d,s_pre_block_num=%d,Data1=%x,Data2=%x,Data3=%x,Data4=%x,Data5=%x"
#define ATC_BASIC_CMD_4283_112_2_17_22_12_33_289 "ATC: write file error"
#define ATC_BASIC_CMD_4285_112_2_17_22_12_33_290 "ATC: the added file:Name=%s, size=%d"
#define ATC_BASIC_CMD_4291_112_2_17_22_12_33_291 "ATC: open file error"
#define ATC_BASIC_CMD_4301_112_2_17_22_12_33_292 "ATC: write file error"
#define ATC_BASIC_CMD_4303_112_2_17_22_12_33_293 "ATC: the added file size is %d"
#define ATC_BASIC_CMD_4309_112_2_17_22_12_33_294 "ATC: open file error"
#define ATC_BASIC_CMD_4317_112_2_17_22_12_33_295 "ATC: cancel the given midi files"
#define ATC_BASIC_CMD_4322_112_2_17_22_12_33_296 "ATC: the midi file format is error"
#define ATC_BASIC_CMD_4346_112_2_17_22_12_33_297 "ATC: smgv SIO string lenght=%d,s_pre_block_chk_num=%d,Data1=%x,Data2=%x,Data3=%x,Data4=%x,Data5=%x, terminator = 0x%x"
#define ATC_BASIC_CMD_4355_112_2_17_22_12_34_298 "ATC: smgv receive a frame head."
#define ATC_BASIC_CMD_4364_112_2_17_22_12_34_299 "ATC: smgv receive a frame stop, switch to AT command mode."
#define ATC_BASIC_CMD_4365_112_2_17_22_12_34_300 "ATC: smgv ctrlz_end set to false."
#define ATC_BASIC_CMD_4379_112_2_17_22_12_34_301 "ATC: the midi file format is error"
#define ATC_BASIC_CMD_4578_112_2_17_22_12_34_302 "ATC: smgv out_len = %d, s_file_chk_len = %d, s_file_received_size = %d"
#define ATC_BASIC_CMD_4579_112_2_17_22_12_34_303 "ATC: smgv ctrlz_end set to false."
#define ATC_BASIC_CMD_4603_112_2_17_22_12_34_304 "ATC: smgv write file error"
#define ATC_BASIC_CMD_4605_112_2_17_22_12_34_305 "ATC: the added file:Name=%s, size=%d"
#define ATC_BASIC_CMD_4612_112_2_17_22_12_34_306 "ATC: smgv open file error"
#define ATC_BASIC_CMD_4623_112_2_17_22_12_34_307 "ATC: smgv write file error"
#define ATC_BASIC_CMD_4625_112_2_17_22_12_34_308 "ATC: the added file size is %d"
#define ATC_BASIC_CMD_4632_112_2_17_22_12_34_309 "ATC: smgv open file error"
#define ATC_BASIC_CMD_4644_112_2_17_22_12_34_310 "ATC: smgv send success frame num! num = %d"
#define ATC_BASIC_CMD_4654_112_2_17_22_12_34_311 "ATC: smgv send error frame num! num = %d"
#define ATC_BASIC_CMD_4702_112_2_17_22_12_34_312 "ATC: file length = %d,block_num = %d,block_id ="
#define ATC_BASIC_CMD_4731_112_2_17_22_12_34_313 "ATC: open file error"
#define ATC_BASIC_CMD_4758_112_2_17_22_12_34_314 "ATC: open file error"
#define ATC_BASIC_CMD_4783_112_2_17_22_12_34_315 "ATC: create a new file,file_name=%s,file_name_len=%d"
#define ATC_BASIC_CMD_4798_112_2_17_22_12_34_316 "ATC: error in block_num,s_file_name"
#define ATC_BASIC_CMD_4854_112_2_17_22_12_35_317 "ATC: create a new file,file_name=%s,file_name_len=%d"
#define ATC_BASIC_CMD_4881_112_2_17_22_12_35_318 "ATC: smgv ctrlz_end set to true."
#define ATC_BASIC_CMD_4932_112_2_17_22_12_35_319 "ATC: ATC_ProcessSMGF, param2 is not string!"
#define ATC_BASIC_CMD_4944_112_2_17_22_12_35_320 "ATC: handle file:Name=%s"
#define ATC_BASIC_CMD_4955_112_2_17_22_12_35_321 "ATC: the parameters is error"
#define ATC_BASIC_CMD_4985_112_2_17_22_12_35_322 "ATC: the parameters is error"
#define ATC_BASIC_CMD_4998_112_2_17_22_12_35_323 "ATC: ATC_ProcessSMGF, param6 is not string!"
#define ATC_BASIC_CMD_5005_112_2_17_22_12_35_324 "ATC: handle file:NewName=%s"
#define ATC_BASIC_CMD_5023_112_2_17_22_12_35_325 "ATC: the parameters is error"
#define ATC_BASIC_CMD_5076_112_2_17_22_12_35_326 "ATC: ATC_ProcessSMGV, param2 is not string!"
#define ATC_BASIC_CMD_5088_112_2_17_22_12_35_327 "ATC: handle file:Name=%s"
#define ATC_BASIC_CMD_5099_112_2_17_22_12_35_328 "ATC: the parameters is error"
#define ATC_BASIC_CMD_5130_112_2_17_22_12_35_329 "ATC: ATC_ProcessSMGV, param4 is not num!"
#define ATC_BASIC_CMD_5139_112_2_17_22_12_35_330 "ATC: the parameters is error"
#define ATC_BASIC_CMD_5151_112_2_17_22_12_35_331 "ATC: ATC_ProcessSMGV, param6 is not string!"
#define ATC_BASIC_CMD_5158_112_2_17_22_12_35_332 "ATC: handle file:NewName=%s"
#define ATC_BASIC_CMD_5175_112_2_17_22_12_35_333 "ATC: the parameters is error"
#define ATC_BASIC_CMD_5220_112_2_17_22_12_35_334 "SMGD:error in para1"
#define ATC_BASIC_CMD_5226_112_2_17_22_12_35_335 "SMGD:error in para2"
#define ATC_BASIC_CMD_5232_112_2_17_22_12_35_336 "ATC: SMGD, param2 is not string!"
#define ATC_BASIC_CMD_5239_112_2_17_22_12_35_337 "SMGD:error in length of para2"
#define ATC_BASIC_CMD_5245_112_2_17_22_12_35_338 "ATC: handle file:Name=%s"
#define ATC_BASIC_CMD_5250_112_2_17_22_12_35_339 "SMGD:%s doesnt exist"
#define ATC_BASIC_CMD_5268_112_2_17_22_12_35_340 "ATC: run in the loop"
#define ATC_BASIC_CMD_5272_112_2_17_22_12_35_341 "ATC: run the if"
#define ATC_BASIC_CMD_5279_112_2_17_22_12_35_342 "ATC: run the else"
#define ATC_BASIC_CMD_5327_112_2_17_22_12_36_343 "ATC: ATC_ProcessSMGD, param3 is not string!"
#define ATC_BASIC_CMD_5343_112_2_17_22_12_36_344 "ATC: the parameters is error"
#define ATC_BASIC_CMD_5381_112_2_17_22_12_36_345 "ATC: receive the midi file from SIO string lenght: %d"
#define ATC_BASIC_CMD_5395_112_2_17_22_12_36_346 "ATC: write file error"
#define ATC_BASIC_CMD_5397_112_2_17_22_12_36_347 "ATC: the added file size is %d"
#define ATC_BASIC_CMD_5408_112_2_17_22_12_36_348 "ATC: open file error"
#define ATC_BASIC_CMD_5418_112_2_17_22_12_36_349 "ATC: write file error"
#define ATC_BASIC_CMD_5421_112_2_17_22_12_36_350 "ATC: the added file size is %d"
#define ATC_BASIC_CMD_5429_112_2_17_22_12_36_351 "ATC: open file error"
#define ATC_BASIC_CMD_5436_112_2_17_22_12_36_352 "ATC: cancel the given midi files"
#define ATC_BASIC_CMD_5441_112_2_17_22_12_36_353 "ATC: the midi file format is error"
#define ATC_BASIC_CMD_5505_112_2_17_22_12_36_354 "ATC: ProcessCFUN: not get the pending event"
#define ATC_BASIC_CMD_5519_112_2_17_22_12_36_355 "ATC: ATC_ProcessCFUN: set, PARA1:%d"
#define ATC_BASIC_CMD_5542_112_2_17_22_12_36_356 "ATC: ATC_ProcessCFUN: AT+CFUN=4 return S_ATC_SUCCESS when open macro _REF_SC8800H_MODEM_USB_ or TIANLONG_MOCOR"
#define ATC_BASIC_CMD_5954_112_2_17_22_12_37_357 "ATC: pga_gain = %d, digital_gain = %d"
#define ATC_BASIC_CMD_6096_112_2_17_22_12_37_358 "ATC: sadm mode_index = %d, dev_number = %d"
#define ATC_BASIC_CMD_6134_112_2_17_22_12_37_359 "ATC: sadm mode_index = %d, dev_number = %d"
#define ATC_BASIC_CMD_6233_112_2_17_22_12_37_360 "ATC: set dev mode, can't get valid mode type, mode name:%s."
#define ATC_BASIC_CMD_6241_112_2_17_22_12_37_361 "ATC: set dev mode result = %d"
#define ATC_BASIC_CMD_6255_112_2_17_22_12_37_362 "ATC: set dev mode result = %d"
#define ATC_BASIC_CMD_6269_112_2_17_22_12_37_363 "ATC: set aud dev param error."
#define ATC_BASIC_CMD_6332_112_2_17_22_12_38_364 "ATC: sadm mode_index = %d, dev_number = %d"
#define ATC_BASIC_CMD_6357_112_2_17_22_12_38_365 "ATC: PROD_ReadAudioParamFromFlash failed."
#define ATC_BASIC_CMD_6387_112_2_17_22_12_38_366 "ATC: sadm mode_index = %d, dev_number = %d"
#define ATC_BASIC_CMD_6415_112_2_17_22_12_38_367 "ATC: sadm mode_index = %d, dev_number = %d, %d"
#define ATC_BASIC_CMD_6416_112_2_17_22_12_38_368 "ATC:atc_config_ptr->q_value=%d, 0x%x "
#define ATC_BASIC_CMD_6425_112_2_17_22_12_38_369 "AGC_Get_Param failed to get mode name, mode:%d."
#define ATC_BASIC_CMD_6430_112_2_17_22_12_38_370 "ATC:mode_name_ptr: %s, 0x%x, 0x%x"
#define ATC_BASIC_CMD_6433_112_2_17_22_12_38_371 "AGC_Get_Param failed to get mode para."
#define ATC_BASIC_CMD_6439_112_2_17_22_12_38_372 "ATC:atc_config_ptr->q_value=%d, 0x%x, 0x%x, 0x%x"
#define ATC_BASIC_CMD_6442_112_2_17_22_12_38_373 "ATC:atc_config_ptr->q_value=%d, 0x%x, 0x%x, 0x%x"
#define ATC_BASIC_CMD_6452_112_2_17_22_12_38_374 "ATC:atc_config_ptr->q_value=%d, "
#define ATC_BASIC_CMD_6522_112_2_17_22_12_38_375 "ATC: sadm mode_index = %d, dev_number = %d"
#define ATC_BASIC_CMD_6530_112_2_17_22_12_38_376 "AGC_Get_Param failed to get mode name, mode:%d."
#define ATC_BASIC_CMD_6533_112_2_17_22_12_38_377 "AGC_Get_Param to get mode name, mode:%d, %s."
#define ATC_BASIC_CMD_6536_112_2_17_22_12_38_378 "ATC: AUDIONVARM_ReadModeParamFromFlash failed."
#define ATC_BASIC_CMD_6639_112_2_17_22_12_38_379 "ATC: siosadm param_len = %d"
#define ATC_BASIC_CMD_6672_112_2_17_22_12_38_380 "ATC: siosadm setmode param result = %d"
#define ATC_BASIC_CMD_6679_112_2_17_22_12_38_381 "ATC: siosadm input data length error."
#define ATC_BASIC_CMD_6684_112_2_17_22_12_38_382 "Lost the escape character is %s is line %d"
#define ATC_BASIC_CMD_6738_112_2_17_22_12_39_383 "ATC: siosadm for arm, param_len = %d"
#define ATC_BASIC_CMD_6761_112_2_17_22_12_39_384 "ATC:ATC_ProcessSioARMSADMData %s, %s, %d."
#define ATC_BASIC_CMD_6767_112_2_17_22_12_39_385 "ATC:ATC_ProcessSioARMSADMData 2 %s, %s, %d."
#define ATC_BASIC_CMD_6774_112_2_17_22_12_39_386 "ATC: siosadm for arm, setmode param result = %d"
#define ATC_BASIC_CMD_6785_112_2_17_22_12_39_387 "ATC: siosadm for arm , input data index error."
#define ATC_BASIC_CMD_6790_112_2_17_22_12_39_388 "ATC: siosadm for arm , input data length error."
#define ATC_BASIC_CMD_6795_112_2_17_22_12_39_389 "Lost the escape character is %s is line %d"
#define ATC_BASIC_CMD_6851_112_2_17_22_12_39_390 "ATC:ATC_ProcessSioARMSADMNVData:length:%d,terminater_char:%d,mode index::%d"
#define ATC_BASIC_CMD_6858_112_2_17_22_12_39_391 "ATC:cancel the snvm audio data!"
#define ATC_BASIC_CMD_6875_112_2_17_22_12_39_392 "ATC:AUDIONVARM_WriteModeParamToFlash %s, %s, %d."
#define ATC_BASIC_CMD_6881_112_2_17_22_12_39_393 "ATC:AUDIONVARM_WriteModeParamToFlash 2 %s, %s, %d."
#define ATC_BASIC_CMD_6885_112_2_17_22_12_39_394 "ATC:ConvertHexToBin failed."
#define ATC_BASIC_CMD_6893_112_2_17_22_12_39_395 "ATC:AUDIONVARM_WriteModeParamToFlash failed. result:%d"
#define ATC_BASIC_CMD_6900_112_2_17_22_12_39_396 "ATC: siosadm for arm , input data index error."
#define ATC_BASIC_CMD_6906_112_2_17_22_12_39_397 "ATC: siosadmnv for arm, input audio data length error.length:%d != %d"
#define ATC_BASIC_CMD_6911_112_2_17_22_12_39_398 "ATC: snvm audio data format is error."
#define ATC_BASIC_CMD_6964_112_2_17_22_12_39_399 "ATC:ATC_ProcessSioSADMNVData:length:%d,terminater_char:%d,mode index::%d"
#define ATC_BASIC_CMD_6971_112_2_17_22_12_39_400 "ATC:cancel the snvm audio data!"
#define ATC_BASIC_CMD_6995_112_2_17_22_12_39_401 "ATC:ConvertHexToBin failed."
#define ATC_BASIC_CMD_7003_112_2_17_22_12_39_402 "ATC:PROD_WriteAudioParamToFlash failed. result:%d"
#define ATC_BASIC_CMD_7011_112_2_17_22_12_39_403 "ATC: siosadmnv input audio data length error.length:%d != %d"
#define ATC_BASIC_CMD_7016_112_2_17_22_12_39_404 "ATC: snvm audio data format is error."
#define ATC_BASIC_CMD_7158_112_2_17_22_12_39_405 "ATC_ProcessPinInd MNPHONE_ActivePs!~"
#define ATC_BASIC_CMD_7171_112_2_17_22_12_39_406 "ATC_ProcessPinInd MNPHONE_ActivePs!~"
#define ATC_BASIC_CMD_7250_112_2_17_22_12_40_407 "ATC:No sim reason = %d"
#define ATC_BASIC_CMD_7447_112_2_17_22_12_40_408 "ATC: ATC_ProcessRssiInd,+CSQ rxlev=%d,rxqual=%d,csq_flag:%d"
#define ATC_BASIC_CMD_7516_112_2_17_22_12_40_409 "ATC: ATC_ProcessRssiIndEx, +ECSQ old_rxlev=%d,rxlev=%d,rxqual=%d,esqopt thres:%d"
#define ATC_BASIC_CMD_7643_112_2_17_22_12_40_410 "ATC: battery starts charge"
#define ATC_BASIC_CMD_7669_112_2_17_22_12_40_411 "ATC: the battery is out of charge"
#define ATC_BASIC_CMD_7964_112_2_17_22_12_41_412 "ATC:error MNPHONE_SelectBandEx"
#define ATC_BASIC_CMD_8030_112_2_17_22_12_41_413 "ATC: unknow band %d"
#define ATC_BASIC_CMD_8155_112_2_17_22_12_41_414 "ATC: nv item length = %d"
#define ATC_BASIC_CMD_8168_112_2_17_22_12_41_415 "ATC: SNVM read nv item id = %d fail, err = %d"
#define ATC_BASIC_CMD_8230_112_2_17_22_12_42_416 "ATC: nv_item_id_end < nv_item_id"
#define ATC_BASIC_CMD_8242_112_2_17_22_12_42_417 "ATC: SNVM read nv item id = %d fail, err = %d"
#define ATC_BASIC_CMD_8294_112_2_17_22_12_42_418 "ATC: write snvm data fail, err = %d."
#define ATC_BASIC_CMD_8299_112_2_17_22_12_42_419 "ATC: cancel the snvm data."
#define ATC_BASIC_CMD_8303_112_2_17_22_12_42_420 "ATC: snvm data format is error."
#define ATC_BASIC_CMD_8354_112_2_17_22_12_42_421 "ATC: AT+SPSNVM input nv item data length error.length:%d != %d"
#define ATC_BASIC_CMD_8367_112_2_17_22_12_42_422 "ATC: cancel the snvm data."
#define ATC_BASIC_CMD_8371_112_2_17_22_12_42_423 "ATC: snvm data format is error."
#define ATC_BASIC_CMD_8418_112_2_17_22_12_42_424 "ATC:dest_Addr=%x,ch=%x"
#define ATC_BASIC_CMD_8435_112_2_17_22_12_42_425 "ATC:enter ATC_ProcessReadDSPCnf"
#define ATC_BASIC_CMD_8492_112_2_17_22_12_42_426 "ATC:AT+SRWD,ConvertHexToBin=FALSE"
#define ATC_BASIC_CMD_8495_112_2_17_22_12_42_427 "ATC:AT+SRWD,value_or_source_address=%d"
#define ATC_BASIC_CMD_8509_112_2_17_22_12_42_428 "ATC:AT+SRWD,value_or_source_address=%x,%x,%x"
#define ATC_BASIC_CMD_8519_112_2_17_22_12_42_429 "ATC:flag = %d"
#define ATC_BASIC_CMD_8546_112_2_17_22_12_42_430 "ATC:AT+SRWD,error in parameters"
#define ATC_BASIC_CMD_8560_112_2_17_22_12_42_431 "ATC: ATC SRWD, param4 is not string!"
#define ATC_BASIC_CMD_8567_112_2_17_22_12_42_432 "ATC:Length of string is error!"
#define ATC_BASIC_CMD_8576_112_2_17_22_12_42_433 "ATC:AT+SRWD,ConvertInt16ToBin=FALSE"
#define ATC_BASIC_CMD_8808_112_2_17_22_12_43_434 "ATC: need gpio direction parameter!"
#define ATC_BASIC_CMD_8818_112_2_17_22_12_43_435 "ATC: set direction gpio id = %d, type = %d, value = %d"
#define ATC_BASIC_CMD_8832_112_2_17_22_12_43_436 "ATC: Assert Failure receive gpio type = %d"
#define ATC_BASIC_CMD_8891_112_2_17_22_12_43_437 "ATC: need gpio set parameter!"
#define ATC_BASIC_CMD_8905_112_2_17_22_12_43_438 "ATC: gpio:%d not enabled!"
#define ATC_BASIC_CMD_8911_112_2_17_22_12_43_439 "ATC: set value gpio id = %d type = %d, value = %d"
#define ATC_BASIC_CMD_8919_112_2_17_22_12_43_440 "ATC: gpio %d set to input directiong, set value not permit."
#define ATC_BASIC_CMD_8929_112_2_17_22_12_43_441 "ATC: gpio %d set to input directiong, get value not permit."
#define ATC_BASIC_CMD_8938_112_2_17_22_12_43_442 "ATC: Assert Failure receive gpio type = %d"
#define ATC_BASIC_CMD_9074_112_2_17_22_12_43_443 "ATC: $$$ the timer string length is error %d"
#define ATC_BASIC_CMD_9085_112_2_17_22_12_43_444 "ATC: $$$ The timer string format error"
#define ATC_BASIC_CMD_9094_112_2_17_22_12_43_445 "ATC: $$$ the value of year is error"
#define ATC_BASIC_CMD_9105_112_2_17_22_12_43_446 "ATC: $$$ the month digital is error"
#define ATC_BASIC_CMD_9111_112_2_17_22_12_43_447 "ATC: $$$ the month value is error"
#define ATC_BASIC_CMD_9119_112_2_17_22_12_43_448 "ATC: $$$ the day value is error"
#define ATC_BASIC_CMD_9127_112_2_17_22_12_43_449 "ATC: $$$ the hour value is error"
#define ATC_BASIC_CMD_9135_112_2_17_22_12_43_450 "ATC: $$$ the minute value is error"
#define ATC_BASIC_CMD_9144_112_2_17_22_12_43_451 "ATC: $$$ the second value is error"
#define ATC_BASIC_CMD_9148_112_2_17_22_12_43_452 "ATC: $$$ the timer parameter is right"
#define ATC_BASIC_CMD_9367_112_2_17_22_12_44_453 "ATC: Assert Failure unknow clck_code."
#define ATC_BASIC_CMD_9628_112_2_17_22_12_44_454 "ATC: ATC_ProcessERGA, busy"
#define ATC_BASIC_CMD_9638_112_2_17_22_12_44_455 "ATC: ATC_ProcessERGA, result: %d"
#define ATC_BASIC_CMD_9654_112_2_17_22_12_44_456 "ATC: ATC_ProcessERGA, param1 is invalid!"
#define ATC_BASIC_CMD_9684_112_2_17_22_12_45_457 "ATC: ATC_ProcessERGA, busy"
#define ATC_BASIC_CMD_9692_112_2_17_22_12_45_458 "ATC: ATC_ProcessERTCA, param1 or param2 is not string"
#define ATC_BASIC_CMD_9705_112_2_17_22_12_45_459 "ATC: ATC_ProcessERTCA, result: %d"
#define ATC_BASIC_CMD_9722_112_2_17_22_12_45_460 "ATC: ATC_ProcessERTCA, param1 or param2 is invalid"
#define ATC_BASIC_CMD_9757_112_2_17_22_12_45_461 "ATC: ATC_Processxor_MBAU, param1 or param2 is not string"
#define ATC_BASIC_CMD_9769_112_2_17_22_12_45_462 "ATC: ATC_Processxor_MBAU, result: %d"
#define ATC_BASIC_CMD_9793_112_2_17_22_12_45_463 "ATC: ATC_Processxor_MBAU, result: %d"
#define ATC_BASIC_CMD_9811_112_2_17_22_12_45_464 "ATC: ATC_Processxor_MBAU, param1 or param2 is invalid"
#define ATC_BASIC_CMD_9876_112_2_17_22_12_45_465 "ATC: $$$ the status is %d"
#define ATC_BASIC_CMD_9892_112_2_17_22_12_45_466 "ATC: ATC_ProcessSIMmbbmsAlgoCnf_Mbau, mbbms context:%d, Rsp:%d "
#define ATC_BASIC_CMD_9978_112_2_17_22_12_45_467 "ATC: ATC_ProcessSIMmbbmsAlgoCnf, mbbms context:%d, Rsp:%d "
#define ATC_BASIC_CMD_10100_112_2_17_22_12_45_468 "ATC: ATC_ProcessM."
#define ATC_BASIC_CMD_10168_112_2_17_22_12_46_469 "ATC: ATC_ProcessNVCOOPERATE, param1 or param2 is not numeric"
#define ATC_BASIC_CMD_10328_112_2_17_22_12_46_470 "No any time info!"
#define ATC_BASIC_CMD_10451_112_2_17_22_12_46_471 "ATC:ATC_ProcessCrsmCnf"
#define ATC_BASIC_CMD_10454_112_2_17_22_12_46_472 "ATC:ATC_ProcessCrsmCnf,true"
#define ATC_BASIC_CMD_10469_112_2_17_22_12_46_473 "ATC:ATC_ProcessCrsmCnf,FAIL"
#define ATC_BASIC_CMD_10562_112_2_17_22_12_47_474 "ATC: get the CRSM parameters %d"
#define ATC_BASIC_CMD_10592_112_2_17_22_12_47_475 "ATC:ATC_ProcessCRSM"
#define ATC_BASIC_CMD_10619_112_2_17_22_12_47_476 "temp %s  %s"
#define ATC_BASIC_CMD_10657_112_2_17_22_12_47_477 "ATC:ATC_ProcessCRSM  data len is too long"
#define ATC_BASIC_CMD_10712_112_2_17_22_12_47_478 "binli add sim_result %d "
#define ATC_BASIC_CMD_10746_112_2_17_22_12_47_479 "status %d"
#define ATC_BASIC_CMD_10852_112_2_17_22_12_47_480 "ATC: spenha eq mode_index = %d, dev_number = %d, %d"
#define ATC_BASIC_CMD_10864_112_2_17_22_12_47_481 "AGC_Get_Param failed to get mode_index, mode_index :%d."
#define ATC_BASIC_CMD_10884_112_2_17_22_12_47_482 "ATC: spenha tun_eq mode_index = %d, dev_number = %d, %d"
#define ATC_BASIC_CMD_10896_112_2_17_22_12_47_483 "AGC_Get_Param failed to get mode_index, mode_index :%d."
#define ATC_BASIC_CMD_10998_112_2_17_22_12_48_484 "ATC: spenha eq mode_index = %d, dev_number = %d, %d"
#define ATC_BASIC_CMD_11009_112_2_17_22_12_48_485 "AGC_Get_Param failed to get mode_index, mode_index:%d."
#define ATC_BASIC_CMD_11027_112_2_17_22_12_48_486 "ATC: spenha tun_eq mode_index = %d, dev_number = %d, %d"
#define ATC_BASIC_CMD_11038_112_2_17_22_12_48_487 "AGC_Get_Param failed to get mode_index, mode_index:%d."
#define ATC_BASIC_CMD_11158_112_2_17_22_12_48_488 "ATC: siospenha for enha, param_len = %d"
#define ATC_BASIC_CMD_11181_112_2_17_22_12_48_489 "ATC: siospenha for enha, set result = %d"
#define ATC_BASIC_CMD_11192_112_2_17_22_12_48_490 "ATC: siospenha for enha, input data index error."
#define ATC_BASIC_CMD_11197_112_2_17_22_12_48_491 "ATC: siospenha for enha, input data length error."
#define ATC_BASIC_CMD_11202_112_2_17_22_12_48_492 "Lost the escape character is %s is line %d"
#define ATC_BASIC_CMD_11255_112_2_17_22_12_48_493 "ATC: siospenha for enha tun, param_len = %d"
#define ATC_BASIC_CMD_11278_112_2_17_22_12_48_494 "ATC: siospenha for enha tun, set result = %d"
#define ATC_BASIC_CMD_11289_112_2_17_22_12_48_495 "ATC: siospenha for enha tun, input data index error."
#define ATC_BASIC_CMD_11294_112_2_17_22_12_48_496 "ATC: siospenha for enha tun, input data length error."
#define ATC_BASIC_CMD_11299_112_2_17_22_12_48_497 "Lost the escape character is %s is line %d"
#define ATC_BASIC_CMD_11336_112_2_17_22_12_48_498 "ATC:ATC_ProcessSNVM"
#define ATC_BASIC_CMD_11371_112_2_17_22_12_48_499 "ATC: nv item length = %d"
#define ATC_BASIC_CMD_11422_112_2_17_22_12_49_500 "ATC: ERR_INVALID_INDEX mode = %d"
#define ATC_BASIC_CMD_11470_112_2_17_22_12_49_501 "ATC: siospenha for enha, param_len = %d"
#define ATC_BASIC_CMD_11494_112_2_17_22_12_49_502 "ATC: siospenha for enha, set result = %d"
#define ATC_BASIC_CMD_11505_112_2_17_22_12_49_503 "ATC: siospenha for enha, input data index error."
#define ATC_BASIC_CMD_11510_112_2_17_22_12_49_504 "ATC: siospenha for enha, input data length error."
#define ATC_BASIC_CMD_11515_112_2_17_22_12_49_505 "Lost the escape character is %s is line %d"
#define ATC_BASIC_CMD_11569_112_2_17_22_12_49_506 "ATC: siospenha for enha tun, param_len = %d"
#define ATC_BASIC_CMD_11593_112_2_17_22_12_49_507 "ATC: siospenha for enha tun, set result = %d"
#define ATC_BASIC_CMD_11604_112_2_17_22_12_49_508 "ATC: siospenha for enha tun, input data index error."
#define ATC_BASIC_CMD_11609_112_2_17_22_12_49_509 "ATC: siospenha for enha tun, input data length error."
#define ATC_BASIC_CMD_11614_112_2_17_22_12_49_510 "Lost the escape character is %s is line %d"
#define ATC_BASIC_CMD_11718_112_2_17_22_12_49_511 "ATC: call amount = %d, call_flag = %d"
#define ATC_BASIC_CMD_11754_112_2_17_22_12_49_512 "record MUX Channel is %d"
#define ATC_BASIC_CMD_11765_112_2_17_22_12_49_513 "s_record_mux_chl not build up"
#define ATC_BASIC_CMD_11784_112_2_17_22_12_49_514 "[ATAUDIO] ATC_ProcessSPAUDRECORD"
#define ATC_BASIC_CMD_11813_112_2_17_22_12_49_515 "[ATAUDIO] +AUDR :error"
#define ATC_BASIC_CMD_11818_112_2_17_22_12_50_516 "[ATAUDIO] +AUDR :STOP UPLOAD DATA"
#define ATC_BASIC_CMD_11832_112_2_17_22_12_50_517 "[ATAUDIO] +AUDR :CREATE HANDLE"
#define ATC_BASIC_CMD_11839_112_2_17_22_12_50_518 "[ATAUDIO] +AUDR :CREATE HANDLE SUCCESS"
#define ATC_BASIC_CMD_11854_112_2_17_22_12_50_519 "[ATAUDIO] +AUDR :error"
#define ATC_BASIC_CMD_11885_112_2_17_22_12_50_520 "ATC:ATC_ProcessCTZR"
#define ATC_COMMON_275_112_2_17_22_12_50_521 "The state is %d"
#define ATC_COMMON_338_112_2_17_22_12_51_522 "Log ATC infomation:\n"
#define ATC_COMMON_342_112_2_17_22_12_51_523 "AT Command Index : %d\n"
#define ATC_COMMON_343_112_2_17_22_12_51_524 "AT Command Type :"
#define ATC_COMMON_347_112_2_17_22_12_51_525 " BASIC"
#define ATC_COMMON_352_112_2_17_22_12_51_526 " EXTEND"
#define ATC_COMMON_357_112_2_17_22_12_51_527 " EXECUTE"
#define ATC_COMMON_362_112_2_17_22_12_51_528 " SET"
#define ATC_COMMON_367_112_2_17_22_12_51_529 " READ"
#define ATC_COMMON_372_112_2_17_22_12_51_530 " TEST"
#define ATC_COMMON_374_112_2_17_22_12_51_531 "\n"
#define ATC_COMMON_376_112_2_17_22_12_51_532 "AT Command Parameter Number : %d\n"
#define ATC_COMMON_388_112_2_17_22_12_51_533 "AT Command Parameter %d : %dn"
#define ATC_COMMON_394_112_2_17_22_12_51_534 "AT Command Parameter %d : %sn"
#define ATC_COMMON_398_112_2_17_22_12_51_535 "AT Command Parameter %d : Default\n"
#define ATC_COMMON_421_112_2_17_22_12_51_536 "%s\n"
#define ATC_COMMON_456_112_2_17_22_12_51_537 "ATC: Assert Failure bcd len."
#define ATC_COMMON_458_112_2_17_22_12_51_538 "ATC: the BCD CODE length is %d"
#define ATC_COMMON_736_112_2_17_22_12_51_539 " pin1_status: %d pin2_status: %d, phone_status:%d"
#define ATC_COMMON_861_112_2_17_22_12_52_540 "ATC: get the password parameters %d"
#define ATC_COMMON_899_112_2_17_22_12_52_541 "ATC:ATC_GetCommandParams,parsing,<data:> %s,<len:> %d"
#define ATC_COMMON_943_112_2_17_22_12_52_542 "ATC: invalid string param, overstep param max length:%d"
#define ATC_COMMON_958_112_2_17_22_12_52_543 "ATC: invalid string param"
#define ATC_COMMON_982_112_2_17_22_12_52_544 "ATC: param overflow, max count of params is %d"
#define ATC_COMMON_992_112_2_17_22_12_52_545 "ATC: invalid param, quotation mark wrong position, return position:%d"
#define ATC_COMMON_999_112_2_17_22_12_52_546 "ATC: invalid param, overstep param max length , return position:%d"
#define ATC_COMMON_1027_112_2_17_22_12_52_547 "ATC: ATC_GetCommandParams, success, offset:%d,other_cmd_flag:%d"
#define ATC_COMMON_1035_112_2_17_22_12_52_548 "PARAM[%d]: | %d | %d | %s |"
#define ATC_COMMON_1040_112_2_17_22_12_52_549 "ATC: ATC_GetCommandParams, failed!"
#define ATC_COMMON_1066_112_2_17_22_12_52_550 "ATC: invalid param, numeric value overflow, return position:%d"
#define ATC_COMMON_1073_112_2_17_22_12_52_551 "ATC: invalid param, not numeric, return position:%d"
#define ATC_COMMON_1078_112_2_17_22_12_52_552 "ATC: get the number: %d"
#define ATC_COMMON_1109_112_2_17_22_12_52_553 "ATC: Get the facility type %d"
#define ATC_COMMON_1141_112_2_17_22_12_52_554 "ATC: Get the CLCK facility type %d"
#define ATC_COMMON_1190_112_2_17_22_12_52_555 "ATC: Parse dtmf string %d"
#define ATC_COMMON_1217_112_2_17_22_12_52_556 "ATC: check pin string %d"
#define ATC_COMMON_1234_112_2_17_22_12_52_557 "ATC: the called number len: %d"
#define ATC_COMMON_1284_112_2_17_22_12_53_558 "ATC: ATC_ConvertCalledNumToStr,Convert called number={%d,%d,%d}...."
#define ATC_COMMON_1313_112_2_17_22_12_53_559 "ATC: ATC_ConvertCalledNumToStr,Get telephone type:%d"
#define ATC_COMMON_1334_112_2_17_22_12_53_560 "ATC: ATC_ConvertCalledNumToStr,Get telephone num:%s"
#define ATC_COMMON_1340_112_2_17_22_12_53_561 "ATC: ATC_ConvertCalledNumToStr,Get telephone num:%s,the length:%d"
#define ATC_COMMON_1378_112_2_17_22_12_53_562 "ATC:High temp=%x"
#define ATC_COMMON_1402_112_2_17_22_12_53_563 "ATC:low temp=%x"
#define ATC_COMMON_1952_112_2_17_22_12_54_564 "ATC: the midi file length: %d"
#define ATC_COMMON_1973_112_2_17_22_12_54_565 "ATC: allocate memory failed"
#define ATC_COMMON_1982_112_2_17_22_12_54_566 "ATC: open file error"
#define ATC_COMMON_1987_112_2_17_22_12_54_567 "ATC: The specific midi index isn't present"
#define ATC_COMMON_2022_112_2_17_22_12_54_568 "ATC: Assert Failure rsp len."
#define ATC_COMMON_2196_112_2_17_22_12_54_569 "ATC: smgv check frame num error."
#define ATC_COMMON_2202_112_2_17_22_12_54_570 "ATC: smgv check frame data len error."
#define ATC_COMMON_2208_112_2_17_22_12_54_571 "ATC: smgv check crc error."
#define ATC_COMMON_2245_112_2_17_22_12_54_572 "ATC: GetIndexOfCallContext out of range, dual_sys = %d ,call_id = %d"
#define ATC_COMMON_2448_112_2_17_22_12_55_573 "ATC: not GPRS neither TD rat."
#define ATC_COMMON_2475_112_2_17_22_12_55_574 "ATC: not GPRS neither TD rat."
#define ATC_COMMON_2769_112_2_17_22_12_56_575 "ATC:transform chset IRA,WARNING,invalid string length. src_str_length:%d,dest_str_max_len:%d"
#define ATC_COMMON_2780_112_2_17_22_12_56_576 "ATC:transform chset IRA=>IRA,WARNING,dest string buffer overflow."
#define ATC_COMMON_2784_112_2_17_22_12_56_577 "ATC:originator chset and destination chset are both IRA."
#define ATC_COMMON_2808_112_2_17_22_12_56_578 "ATC:transform chset IRA,GSM string length:%d,sevent bit offset:%d."
#define ATC_COMMON_2815_112_2_17_22_12_56_579 "ATC:transform chset IRA=>GSM,WARNING,dest string buffer overflow."
#define ATC_COMMON_2828_112_2_17_22_12_56_580 "ATC:transform chset IRA=>HEX,WARNING,dest string buffer overflow."
#define ATC_COMMON_2841_112_2_17_22_12_56_581 "ATC:transform chset IRA=>UCS2,WARNING,dest string buffer overflow."
#define ATC_COMMON_2871_112_2_17_22_12_56_582 "ATC:transform chset IRA => %s success. string length %d=>%d"
#define ATC_COMMON_2875_112_2_17_22_12_56_583 "ATC:transform chset IRA => %s failed."
#define ATC_COMMON_2912_112_2_17_22_12_56_584 "ATC:transform chset GSM,WARNING,invalid string length.src_str_length:%d,dest_str_max_len:%d"
#define ATC_COMMON_2922_112_2_17_22_12_56_585 "ATC:transform chset GSM=>GSM,WARNING,dest string buffer overflow."
#define ATC_COMMON_2926_112_2_17_22_12_56_586 "ATC:originator chset and destination chset are both GSM."
#define ATC_COMMON_2943_112_2_17_22_12_56_587 "ATC:transform chset first step GSM(hex=>bin) success, gsm string len:%d."
#define ATC_COMMON_2958_112_2_17_22_12_56_588 "ATC:transform chset second step GSM(bin)=>IRA success, IRA string length:%d."
#define ATC_COMMON_2993_112_2_17_22_12_56_589 "ATC:transform chset GSM => %s success. string length %d=>%d"
#define ATC_COMMON_2997_112_2_17_22_12_56_590 "ATC:transform chset GSM => %s failed."
#define ATC_COMMON_3030_112_2_17_22_12_56_591 "ATC:transform chset HEX,WARNING,invalid string length.src_str_length:%d,dest_str_max_len:%d"
#define ATC_COMMON_3040_112_2_17_22_12_56_592 "ATC:transform chset HEX=>HEX,WARNING,dest string buffer overflow."
#define ATC_COMMON_3044_112_2_17_22_12_56_593 "ATC:originator chset and destination chset are both HEX."
#define ATC_COMMON_3065_112_2_17_22_12_57_594 "ATC:transform chset first step HEX=>IRA success, ira string len:%d."
#define ATC_COMMON_3087_112_2_17_22_12_57_595 "ATC:transform chset HEX=>IRA,WARNING,dest string buffer overflow."
#define ATC_COMMON_3098_112_2_17_22_12_57_596 "ATC: ATC_ConvertHEXToUCS2 failed."
#define ATC_COMMON_3120_112_2_17_22_12_57_597 "ATC:transform chset HEX => %s success. string length %d=>%d"
#define ATC_COMMON_3124_112_2_17_22_12_57_598 "ATC:transform chset HEX => %s failed."
#define ATC_COMMON_3156_112_2_17_22_12_57_599 "ATC:transform chset UCS2,WARNING,invalid string length."
#define ATC_COMMON_3166_112_2_17_22_12_57_600 "ATC:transform chset UCS2=>UCS2,WARNING,dest string buffer overflow."
#define ATC_COMMON_3170_112_2_17_22_12_57_601 "ATC:originator chset and destination chset are both UCS2."
#define ATC_COMMON_3178_112_2_17_22_12_57_602 "ATC:ATC_ConvertUCS2ToIRA failed."
#define ATC_COMMON_3192_112_2_17_22_12_57_603 "ATC:ATC_ConvertUCS2ToIRA failed."
#define ATC_COMMON_3196_112_2_17_22_12_57_604 "ATC:transform chset first step UCS2=>IRA success, ira string len:%d."
#define ATC_COMMON_3214_112_2_17_22_12_57_605 "ATC:ATC_ConvertUCS2ToHEX failed."
#define ATC_COMMON_3236_112_2_17_22_12_57_606 "ATC:transform chset UCS2 => %s success. string length %d=>%d"
#define ATC_COMMON_3240_112_2_17_22_12_57_607 "ATC:transform chset UCS2 => %s failed."
#define ATC_COMMON_3280_112_2_17_22_12_57_608 "ATC:transform chset %s => %s not support yet."
#define ATC_ENG_340_112_2_17_22_12_58_609 "ATC: service_type = %d, ul_rate = %d,dl_rate"
#define ATC_ENG_446_112_2_17_22_12_58_610 "WARNING: set_real_nw_opt_enable_flag is unsupported!"
#define ATC_ENG_814_112_2_17_22_12_59_611 "aucPlmn[0]=%x,aucPlmn[1]=%x,aucPlmn[2]=%x"
#define ATC_ENG_821_112_2_17_22_12_59_612 "atc write uplmn wact file error\n"
#define ATC_ENG_847_112_2_17_22_12_59_613 "at uplmn num=%d\n"
#define ATC_ENG_879_112_2_17_22_12_59_614 "the uplmn id =%s\n"
#define ATC_ENG_981_112_2_17_22_12_59_615 " the %d freq have %d cells\n"
#define ATC_ENG_984_112_2_17_22_12_59_616 " total freq point =%d\n"
#define ATC_ENG_1046_112_2_17_22_12_59_617 "ATENG:ATC_Processxor_SYSINFO\n"
#define ATC_ENG_1073_112_2_17_22_12_59_618 "ATENG:ATC_ProcessSPDSP\n"
#define ATC_ENG_1201_112_2_17_22_13_0_619 "ATC: ^HVER: %s"
#define ATC_ENG_1395_112_2_17_22_13_0_620 "ATC:Set rat:%d, dm_prefer_rat:%d"
#define ATC_ENG_1533_112_2_17_22_13_0_621 "ATC:Set rat:%d, dm_prefer_rat:%d"
#define ATC_ENG_1948_112_2_17_22_13_1_622 "ATC: get the CRSM parameters %d"
#define ATC_ENG_1969_112_2_17_22_13_1_623 "ATC:ATC_ProcessCRSM"
#define ATC_ENG_1999_112_2_17_22_13_1_624 "ATC:ATC_ProcessCRSM  data len is too long"
#define ATC_ENG_2105_112_2_17_22_13_1_625 "ATC:ATC_ProcessCrsmCnf"
#define ATC_ENG_2108_112_2_17_22_13_1_626 "ATC:ATC_ProcessCrsmCnf,true"
#define ATC_ENG_2123_112_2_17_22_13_1_627 "ATC:ATC_ProcessCrsmCnf,FAIL"
#define ATC_ENG_2198_112_2_17_22_13_2_628 "ATC: ECMMB, NO param1 !"
#define ATC_ENG_2203_112_2_17_22_13_2_629 "ATC:ECMMB, set command, oper=%d"
#define ATC_ENG_2815_112_2_17_22_13_3_630 "ATC: Invalid param1, dummy gsm cells' num is %d"
#define ATC_ENG_2837_112_2_17_22_13_3_631 "ATC: Invalid param2"
#define ATC_ENG_2849_112_2_17_22_13_3_632 "ATC: Invalid param3"
#define ATC_ENG_2860_112_2_17_22_13_3_633 "ATC: Invalid param4"
#define ATC_ENG_2873_112_2_17_22_13_3_634 "ATC: wrong dummy gsm cell num, %d == %d, Max:%d"
#define ATC_ENG_3187_112_2_17_22_13_3_635 "ATC: ATC_ProcessAutoTestInfoFromMMI,ERROR,wrong string length:%d"
#define ATC_ENG_3230_112_2_17_22_13_4_636 "ATC: SIMULATOR KPD: generate down key, key code is 0x%x"
#define ATC_ENG_3238_112_2_17_22_13_4_637 "ATC: SIMULATOR KPD: generate up key, key code is 0x%x"
#define ATC_ENG_3252_112_2_17_22_13_4_638 "ATC: SIMULATOR KPD: generate down and up key, key code is 0x%x"
#define ATC_ENG_3486_112_2_17_22_13_4_639 "ATC: SPEQPARA: <n> is invalid."
#define ATC_ENG_3501_112_2_17_22_13_4_640 "ATC: SPEQPARA: <type> or <dest> is invalid."
#define ATC_ENG_3519_112_2_17_22_13_4_641 "ATC: SPEQPARA: Get EQ PARA failed."
#define ATC_ENG_3539_112_2_17_22_13_4_642 "ATC: SPEQPARA: <type> or <dest> or <data> is invalid."
#define ATC_ENG_3546_112_2_17_22_13_4_643 "ATC: SPEQPARA: <data> type is not string."
#define ATC_ENG_3552_112_2_17_22_13_4_644 "ATC: SPEQPARA: <data> length:%d != %d*2."
#define ATC_ENG_3561_112_2_17_22_13_4_645 "ATC: SPEQPARA: ConvertHexToBin failed."
#define ATC_ENG_3582_112_2_17_22_13_4_646 "ATC: SPEQPARA: Set EQ PARA failed."
#define ATC_ENG_4081_112_2_17_22_13_5_647 "test spinterrat!"
#define ATC_ENG_4175_112_2_17_22_13_5_648 "GetAdcCalibrateInfoStr %x"
#define ATC_ENG_4178_112_2_17_22_13_5_649 "ATC: SPCALIBRATE."
#define ATC_ENG_4312_112_2_17_22_13_6_650 "test : %s %s "
#define ATC_ENG_4418_112_2_17_22_13_6_651 "test : %s %s "
#define ATC_ENG_4504_112_2_17_22_13_6_652 "STEVE nvm_band_info.no_range = %d"
#define ATC_ENG_4719_112_2_17_22_13_7_653 "ATC: ATC_ProcessSPAUTOPOWER."
#define ATC_ENG_4727_112_2_17_22_13_7_654 "ATC: auto_poweron_flag = %d"
#define ATC_ENG_4756_112_2_17_22_13_7_655 "ATC:SPENGMD params1-3 less"
#define ATC_ENG_4766_112_2_17_22_13_7_656 "ATC:SPENGMD param1-3 is error"
#define ATC_ENG_4805_112_2_17_22_13_7_657 "ATC: SPENGMD: mainType=%d, subType=%d"
#define ATC_ENG_4808_112_2_17_22_13_7_658 "ATC:SPENGMD param check fail"
#define ATC_ENG_4835_112_2_17_22_13_7_659 "ATC: SPENGMD   item_count = %d"
#define ATC_ENG_4842_112_2_17_22_13_7_660 "ATC: SPENGMD   item.arr_count = %d"
#define ATC_ENG_4879_112_2_17_22_13_7_661 "ATC: SPENGMD set param check index fail"
#define ATC_ENG_4886_112_2_17_22_13_7_662 "ATC: SPENGMD set_fptr == null"
#define ATC_ENG_4894_112_2_17_22_13_7_663 "ATC: SPENGMD set l1 info param"
#define ATC_ENG_4935_112_2_17_22_13_7_664 "ATC: SPENGMD set l1 info param %d %d %d %d"
#define ATC_ENG_4953_112_2_17_22_13_7_665 "ATC: SPENGMD set mem deinit assert param"
#define ATC_ENG_4966_112_2_17_22_13_7_666 "ATC: SPENGMD set save smebuffer udisc"
#define ATC_ENG_5001_112_2_17_22_13_7_667 "ATC:SPENGMD check paramters %d ,mainType=%d, subType=%d"
#define ATC_GPRS_MODEM_197_112_2_17_22_13_8_668 "ATC: ATC_ProcessCPBW, param3 is not string!"
#define ATC_GPRS_MODEM_283_112_2_17_22_13_8_669 "ATGPRS: ATC_ProcessSDATASTART\n"
#define ATC_GPRS_MODEM_435_112_2_17_22_13_9_670 "ATGPRS: ATC_ProcessSDATASEND\n"
#define ATC_GPRS_MODEM_464_112_2_17_22_13_9_671 "ATC: ATC_ProcessSDATASEND, param2 is not num!"
#define ATC_GPRS_MODEM_501_112_2_17_22_13_9_672 "ATGPRS:set atc Z gprs end char"
#define ATC_GPRS_MODEM_545_112_2_17_22_13_9_673 "ATC: ATC_ProcessSSTRSEND, param1, param2 is need!"
#define ATC_GPRS_MODEM_558_112_2_17_22_13_9_674 "ATC: ATC_ProcessSSTRSEND, param2 is not string!"
#define ATC_GPRS_MODEM_820_112_2_17_22_13_9_675 "ATGPRS:Send PDU data:"
#define ATC_GPRS_MODEM_823_112_2_17_22_13_9_676 " %02x"
#define ATC_GPRS_MODEM_869_112_2_17_22_13_9_677 "ATC: Assert Failure data_len."
#define ATC_GPRS_MODEM_893_112_2_17_22_13_10_678 "AT MODEM:update udp fport as %04x"
#define ATC_GPRS_MODEM_1120_112_2_17_22_13_10_679 "ATC: close tcp upcall."
#define ATC_GPRS_MODEM_1218_112_2_17_22_13_10_680 "ATGPRS:+tcp Send Failed!"
#define ATC_GPRS_MODEM_1236_112_2_17_22_13_10_681 "ATGPRS:+udp Send Failed!"
#define ATC_GPRS_MODEM_1315_112_2_17_22_13_10_682 "modem:read tcp len =%d"
#define ATC_GPRS_MODEM_1339_112_2_17_22_13_10_683 "ATC: close tcp upcall."
#define ATC_GPRS_MODEM_1370_112_2_17_22_13_11_684 "ATC: enter tcp rcv callback!"
#define ATC_GPRS_MODEM_1380_112_2_17_22_13_11_685 "ATC: send stcpd notification!"
#define ATC_GPRS_MODEM_1391_112_2_17_22_13_11_686 "ATC: socket closed notification!"
#define ATC_GPRS_MODEM_1405_112_2_17_22_13_11_687 "ATC: so error index, so =%d"
#define ATC_GSM_991_112_2_17_22_13_13_688 "ATC: ATC_ProcessA: there is an active call, not allowed another!"
#define ATC_GSM_997_112_2_17_22_13_13_689 "ATC: ATC_ProcessA, no waiting call!"
#define ATC_GSM_1008_112_2_17_22_13_13_690 "ATC: ATC_ProcessA, invalid call index!"
#define ATC_GSM_1028_112_2_17_22_13_13_691 "ATC: ATC_ProcessA, MNCALL_ConnectCall failure!"
#define ATC_GSM_1484_112_2_17_22_13_14_692 "ATC: this is a emergency call!"
#define ATC_GSM_1539_112_2_17_22_13_14_693 "ATC: this is a voice call!"
#define ATC_GSM_1544_112_2_17_22_13_14_694 "ATC: this is a data call!"
#define ATC_GSM_1659_112_2_17_22_13_14_695 "ATC:ATC_ProcessD atc_config_ptr->call_link_id = %d "
#define ATC_GSM_1720_112_2_17_22_13_14_696 "ATC: Assert Failure call_amount error %d."
#define ATC_GSM_1729_112_2_17_22_13_14_697 "ATC: ATC_ProcessCallStartInd call_type:%d, call_amount: %d"
#define ATC_GSM_1961_112_2_17_22_13_15_698 "ATC: ProcessGprs99Dial, the cid:%d is not allowed."
#define ATC_GSM_1969_112_2_17_22_13_15_699 "ATC: set dial apn ..."
#define ATC_GSM_1986_112_2_17_22_13_15_700 "ATC: ProcessGprs99Dial, this pdp id is being deactivated."
#define ATC_GSM_2081_112_2_17_22_13_15_701 "ATC: Gprs98Dial, the cid:%d is not allowed."
#define ATC_GSM_2097_112_2_17_22_13_15_702 "ATC: Gprs98Dial, this pdp id is being deactivated."
#define ATC_GSM_2141_112_2_17_22_13_15_703 "ATC: ATC_ProcessGprsDial, 99 modem, cid=%d"
#define ATC_GSM_2164_112_2_17_22_13_15_704 "ATC: ATC_ProcessGprsDial, 99 failure"
#define ATC_GSM_2180_112_2_17_22_13_15_705 "ATC: ATC_ProcessGprsDial, 98 modem, cid=%d"
#define ATC_GSM_2203_112_2_17_22_13_15_706 "ATC: ATC_ProcessGprsDial, 98 failure"
#define ATC_GSM_2271_112_2_17_22_13_16_707 "ATC: StartDTMFKey, call_id >= MN_CALL_MAX_CALL_NUM"
#define ATC_GSM_2278_112_2_17_22_13_16_708 "ATC: MNCALL_StartDTMF, call id:%d dtmf key:%c"
#define ATC_GSM_2285_112_2_17_22_13_16_709 "ATC: MNCALL_StartDTMF failed."
#define ATC_GSM_2302_112_2_17_22_13_16_710 "ATC: StopDTMFKey, call_id >= MN_CALL_MAX_CALL_NUM"
#define ATC_GSM_2309_112_2_17_22_13_16_711 "ATC: MNCALL_StopDTMF, call id:%d."
#define ATC_GSM_2316_112_2_17_22_13_16_712 "ATC: MNCALL_StopDTMF failed."
#define ATC_GSM_2338_112_2_17_22_13_16_713 "ATC: GetDTMFKey, <dtmf> is not character or not in range."
#define ATC_GSM_2345_112_2_17_22_13_16_714 "ATC: GetDTMFKey, %d."
#define ATC_GSM_2378_112_2_17_22_13_16_715 "ATC: <DTMF> string format wrong."
#define ATC_GSM_2395_112_2_17_22_13_16_716 "ATC: GetDTMFString, %s."
#define ATC_GSM_2476_112_2_17_22_13_16_717 "ATC: enter the DTMF timer expire"
#define ATC_GSM_2497_112_2_17_22_13_16_718 "ATC: ATC_ProcessDTMFTimerExpInd"
#define ATC_GSM_2500_112_2_17_22_13_16_719 "ATC: no dtmf key to stop."
#define ATC_GSM_2530_112_2_17_22_13_16_720 "ATC:ATC_ProcessStartDTMFCnf,callid:%d == %d,DTMF key:%c, AT cmd type:%d"
#define ATC_GSM_2578_112_2_17_22_13_16_721 "ATC: DTMF SIG WRONG."
#define ATC_GSM_2584_112_2_17_22_13_16_722 "ATC: DTMF TIMER WRONG."
#define ATC_GSM_2624_112_2_17_22_13_16_723 "ATC:ATC_ProcessStopDTMFCnf,callid:%d == %d,AT cmd type:%d"
#define ATC_GSM_2710_112_2_17_22_13_16_724 "ATC: VTS: no active call"
#define ATC_GSM_2716_112_2_17_22_13_16_725 "ATC: VTS: no active call found"
#define ATC_GSM_2720_112_2_17_22_13_16_726 "ATC: VTS: call-id:%d"
#define ATC_GSM_2726_112_2_17_22_13_16_727 "ATC: VTS: the mode of the call is not voice"
#define ATC_GSM_2733_112_2_17_22_13_16_728 "ATC: VTS: the previous dtmf keys have not done."
#define ATC_GSM_2764_112_2_17_22_13_17_729 "ATC: VTS: <duration> param error. not numeric."
#define ATC_GSM_2775_112_2_17_22_13_17_730 "ATC: VTS: <duration> not in range (0-255)."
#define ATC_GSM_2792_112_2_17_22_13_17_731 "ATC: ATC_ProcessVTS: too long dtmf string."
#define ATC_GSM_2820_112_2_17_22_13_17_732 "ATC: VTS: <DTMF> param set error."
#define ATC_GSM_2826_112_2_17_22_13_17_733 "ATC: VTS: params not supported."
#define ATC_GSM_2838_112_2_17_22_13_17_734 "ATC: send dtmf:%s,duration:%d*100ms"
#define ATC_GSM_2848_112_2_17_22_13_17_735 "ATC: VTS, MNCALL_StartDTMF failed."
#define ATC_GSM_2944_112_2_17_22_13_17_736 "ATC: EVTS: no active call"
#define ATC_GSM_2951_112_2_17_22_13_17_737 "ATC: EVTS: no active call found"
#define ATC_GSM_2959_112_2_17_22_13_17_738 "ATC: EVTS: the call:%d is not voice mode"
#define ATC_GSM_2964_112_2_17_22_13_17_739 "ATC:EVTS: call-id:%d"
#define ATC_GSM_2968_112_2_17_22_13_17_740 "ATC: EVTS: AT+VTS has not done."
#define ATC_GSM_2987_112_2_17_22_13_17_741 "ATC: EVTS: <mode> param error. not numeric."
#define ATC_GSM_2993_112_2_17_22_13_17_742 "ATC: EVTS: <mode> set error"
#define ATC_GSM_3025_112_2_17_22_13_17_743 "ATC: StopDTMF = %c"
#define ATC_GSM_3029_112_2_17_22_13_17_744 "ATC: ATC_ProcessEVTS: MNCALL_StopDTMF failed."
#define ATC_GSM_3035_112_2_17_22_13_17_745 "ATC: WARN:%d,%c"
#define ATC_GSM_3060_112_2_17_22_13_17_746 "ATC: ATC_ProcessEVTS: DTMF is already started, pls stop firstly."
#define ATC_GSM_3066_112_2_17_22_13_17_747 "ATC: ATC_ProcessEVTS: <mode> set error"
#define ATC_GSM_3141_112_2_17_22_13_17_748 "ATC: ATC_ProcessCOPS, not allowed when CALL is existed."
#define ATC_GSM_3154_112_2_17_22_13_17_749 "ATC: ATC_ProcessCOPS, sim is not ready or cpin code is not ready."
#define ATC_GSM_3169_112_2_17_22_13_17_750 "ATC: ATC_ProcessCOPS, APP_MN_PLMN_SELECT_CNF is being handled."
#define ATC_GSM_3191_112_2_17_22_13_17_751 "ATC: MNPHONE_SelectPLMN failure"
#define ATC_GSM_3350_112_2_17_22_13_18_752 "ATC: RAT value is wrong"
#define ATC_GSM_3422_112_2_17_22_13_18_753 "ATC: Assert Failure plmn_status."
#define ATC_GSM_3432_112_2_17_22_13_18_754 "ATC: search the plmn list"
#define ATC_GSM_3451_112_2_17_22_13_18_755 "ATC: ATC_ProcessCOPS, failure in MNPHONE_ListPLMN."
#define ATC_GSM_3499_112_2_17_22_13_18_756 "ATC: plmn list type is invaild"
#define ATC_GSM_3571_112_2_17_22_13_18_757 "ATC: MNPHONE_WritePLMNtoSim failure."
#define ATC_GSM_3605_112_2_17_22_13_18_758 "ATC:CPOL FULL_NAME not found!"
#define ATC_GSM_3617_112_2_17_22_13_18_759 "ATC:CPOL SHORT_NAME not found!"
#define ATC_GSM_3633_112_2_17_22_13_18_760 "ATC:CPOL not support this plmn"
#define ATC_GSM_3638_112_2_17_22_13_18_761 "ATC:cpol plmn = %d"
#define ATC_GSM_3654_112_2_17_22_13_18_762 "ATC: cpol not support this plmn"
#define ATC_GSM_3670_112_2_17_22_13_18_763 "ATC: cpol not support to set gsm_compact_act "
#define ATC_GSM_3686_112_2_17_22_13_18_764 "ATC: MNPHONE_WritePLMNtoSim failure"
#define ATC_GSM_3696_112_2_17_22_13_18_765 "ATC: reading the plmn list"
#define ATC_GSM_3706_112_2_17_22_13_18_766 "ATC: failure in MNPHONE_ReadPLMNfromSim"
#define ATC_GSM_3730_112_2_17_22_13_18_767 "ATC: failure in MNPHONE_ReadPLMNfromSim"
#define ATC_GSM_3878_112_2_17_22_13_19_768 "ATC: enter ATC_ProcessSSMP"
#define ATC_GSM_3911_112_2_17_22_13_19_769 "ATC: read plmn_num = %d,is_valid_result = %d"
#define ATC_GSM_3953_112_2_17_22_13_19_770 "atc: is_valid_plmn[%d]=%d, is_gsm_act[%d]=%dn"
#define ATC_GSM_3955_112_2_17_22_13_19_771 "atc: is_gsm_compact_act[%d]=%d,is_td_act[%d]=%dn"
#define ATC_GSM_3957_112_2_17_22_13_19_772 "atc: plmn_list=%s\n"
#define ATC_GSM_4026_112_2_17_22_13_19_773 "ATC: write plmn confirm "
#define ATC_GSM_4060_112_2_17_22_13_19_774 "ATC: received the plmn list confirm "
#define ATC_GSM_4061_112_2_17_22_13_19_775 "ATC: %d,%d"
#define ATC_GSM_4096_112_2_17_22_13_19_776 "ACT: RAT value is wrong"
#define ATC_GSM_4217_112_2_17_22_13_19_777 "ATC: Assert Failure unknow plmn_status."
#define ATC_GSM_4226_112_2_17_22_13_19_778 "ATC: PlmnSelCnf sig_ptr->cause: 0x%x"
#define ATC_GSM_4243_112_2_17_22_13_19_779 "ATC: ATC_ProcessPlmnSelCnf, is_plmn_success:FALSE, MANUAL_MATO_MODE"
#define ATC_GSM_4306_112_2_17_22_13_20_780 "ATC: ManualSelectPlmn, FULL_NAME, not found!"
#define ATC_GSM_4318_112_2_17_22_13_20_781 "ATC: ManualSelectPlmn, SHORT_NAME, not found!"
#define ATC_GSM_4329_112_2_17_22_13_20_782 "ATC: ManualSelectPlmn, NUMBERIC, cops not support this plmn"
#define ATC_GSM_4339_112_2_17_22_13_20_783 "ATC: ManualSelectPlmn, plmn = %d"
#define ATC_GSM_4355_112_2_17_22_13_20_784 "ATC: ManualSelectPlmn, cops not support this plmn"
#define ATC_GSM_4362_112_2_17_22_13_20_785 "ATC: ManualSelectPlmn, failure in MN"
#define ATC_GSM_4403_112_2_17_22_13_20_786 "ATC: link_id:%d,g_calls_state.call_amout= %d"
#define ATC_GSM_4418_112_2_17_22_13_20_787 "ATC: ATC_ProcessH, cause_value=%d"
#define ATC_GSM_4462_112_2_17_22_13_20_788 "ATC: Assert Failure call_amount %d."
#define ATC_GSM_4515_112_2_17_22_13_20_789 "ATC: the call number len: %d"
#define ATC_GSM_4619_112_2_17_22_13_20_790 "ATC: the call number len: %d"
#define ATC_GSM_4627_112_2_17_22_13_20_791 "ATC: no call number present!"
#define ATC_GSM_4649_112_2_17_22_13_20_792 "ATC: ATC_ProcessAlertingInd, invalid call index!"
#define ATC_GSM_4803_112_2_17_22_13_21_793 "ATC: Response the CONNECT result code"
#define ATC_GSM_5069_112_2_17_22_13_21_794 "ATC:ATC_ProcessSTAR_SPNTRG"
#define ATC_GSM_5094_112_2_17_22_13_21_795 "ATC: ATC_ProcessSTAR_SPNTRG, plmn_status:%d,rat:%d,cell_cap:%d%d%d%d%d"
#define ATC_GSM_5204_112_2_17_22_13_21_796 "ATC: Assert Failure unknow disc code."
#define ATC_GSM_5236_112_2_17_22_13_21_797 "ss state %x,%d"
#define ATC_GSM_5274_112_2_17_22_13_22_798 "ATC: ATC_ProcessCallDiscInd, call_amount:%d,g_atc_call_flag:%d,is_voice_call:%d"
#define ATC_GSM_5357_112_2_17_22_13_22_799 "ss state %x"
#define ATC_GSM_5437_112_2_17_22_13_22_800 "connect call id = %d"
#define ATC_GSM_5448_112_2_17_22_13_22_801 "ATC: retrieve call id = %d"
#define ATC_GSM_5456_112_2_17_22_13_22_802 "ATC: chld disconnect call success"
#define ATC_GSM_5556_112_2_17_22_13_22_803 "ATC: enter ATC_ProcessAcmInd, dual_sys =%d"
#define ATC_GSM_5580_112_2_17_22_13_22_804 "ATC: enter the timer expire dual_sys =%d"
#define ATC_GSM_5584_112_2_17_22_13_22_805 "ATC: unexpected timer expire"
#define ATC_GSM_5590_112_2_17_22_13_22_806 "ATC: ATC_ProcessCallExpInd, CheckPresentWaitCall failure!"
#define ATC_GSM_5598_112_2_17_22_13_22_807 "ATC: ATC_ProcessCallExpInd, invalid call index!"
#define ATC_GSM_5658_112_2_17_22_13_22_808 "ATC: ATC_ProcessCallExpInd, MNCALL_ConnectCall failure!"
#define ATC_GSM_5708_112_2_17_22_13_22_809 "ATC: ATC_ProcessCBST"
#define ATC_GSM_5757_112_2_17_22_13_22_810 "ATC: ATC_ProcessCMOD"
#define ATC_GSM_5806_112_2_17_22_13_23_811 "ATC: ATC_ProcessDL"
#define ATC_GSM_5820_112_2_17_22_13_23_812 "ATC:ATC_ProcessDL atc_config_ptr->call_link_id = %d "
#define ATC_GSM_5828_112_2_17_22_13_23_813 "ATC: failure in MNCALL_StartCallEx"
#define ATC_GSM_5834_112_2_17_22_13_23_814 "ATC: no call number recently."
#define ATC_GSM_5855_112_2_17_22_13_23_815 "ATC: ATC_ProcessCHUP\n"
#define ATC_GSM_5885_112_2_17_22_13_23_816 "ATC: g_calls_state.call_amout= %d"
#define ATC_GSM_6102_112_2_17_22_13_23_817 "ATC: ATC_ProcessCHUPVT"
#define ATC_GSM_6106_112_2_17_22_13_23_818 "ATC: ATC_ProcessCHUPVT, NOT set command!"
#define ATC_GSM_6112_112_2_17_22_13_23_819 "ATC: ATC_ProcessCHUPVT, no param1"
#define ATC_GSM_6127_112_2_17_22_13_23_820 "ATC: g_calls_state.call_amout= %d"
#define ATC_GSM_6174_112_2_17_22_13_23_821 "ATC: the dtmf value is %c"
#define ATC_GSM_6249_112_2_17_22_13_23_822 "ATC:invalid plmn num %c"
#define ATC_GSM_6265_112_2_17_22_13_23_823 "ATC: ATC_ProcessTRACE"
#define ATC_GSM_6275_112_2_17_22_13_24_824 "ATC: ATC_ProcessTRACE set:%d->%d"
#define ATC_GSM_6312_112_2_17_22_13_24_825 "ATC: ATC_ProcessOFF"
#define ATC_GSM_6343_112_2_17_22_13_24_826 "ATC: ATC_ProcessRESET"
#define ATC_GSM_6351_112_2_17_22_13_24_827 "ATC: no parameter 1"
#define ATC_GSM_6356_112_2_17_22_13_24_828 "ATC: no parameter 1"
#define ATC_GSM_6370_112_2_17_22_13_24_829 "ATC: Previous Reset is in progress."
#define ATC_GSM_6413_112_2_17_22_13_24_830 "ATC: ATC_ProcessSPAUTO, invalid parameter or not execute command"
#define ATC_GSM_6427_112_2_17_22_13_24_831 "ATC: ATC_ProcessSPAUTO, invalid parameter"
#define ATC_GSM_6438_112_2_17_22_13_24_832 "ATC: ATC_ProcessSPAUTO, invalid parameter"
#define ATC_GSM_6466_112_2_17_22_13_24_833 "ATC: ATC_ProcessSPAUTO, invalide parameter(not 0 & 1)"
#define ATC_GSM_6506_112_2_17_22_13_24_834 "ATC: ATC_GetPlmnDetailedInfo, Invalid parameters"
#define ATC_GSM_6517_112_2_17_22_13_24_835 "ATC: ATC_GetPlmnDetailedInfo, FULL_NAME is too long (%d)"
#define ATC_GSM_6538_112_2_17_22_13_24_836 "ATC: ATC_GetPlmnDetailedInfo, SHORT_NAME is too long (%d)"
#define ATC_GSM_6766_112_2_17_22_13_24_837 "ATC:CUFP not support this plmn"
#define ATC_GSM_6771_112_2_17_22_13_24_838 "ATC:CUFP plmn = %d"
#define ATC_GSM_6787_112_2_17_22_13_25_839 "ATC: CUFP not support this plmn"
#define ATC_GSM_6793_112_2_17_22_13_25_840 "ATC: MNPHONE_WritePLMNtoSim failure"
#define ATC_GSM_SMS_636_112_2_17_22_13_26_841 "ATC: Get sc failure"
#define ATC_GSM_SMS_643_112_2_17_22_13_26_842 "ATC: Read s_sms_nv_config failure"
#define ATC_GSM_SMS_658_112_2_17_22_13_26_843 "ATC: CSAS, sms is not ready!!!"
#define ATC_GSM_SMS_669_112_2_17_22_13_26_844 "ATC: write NV_ATC_CONFIG_ID fail"
#define ATC_GSM_SMS_704_112_2_17_22_13_26_845 "ATC: ATC_ProcessCPMS: sms is not ready"
#define ATC_GSM_SMS_734_112_2_17_22_13_26_846 "ATC:AT+CPMS,error in para1"
#define ATC_GSM_SMS_757_112_2_17_22_13_26_847 "ATC:AT+CPMS,error in para2"
#define ATC_GSM_SMS_782_112_2_17_22_13_26_848 "ATC:AT+CPMS,error in para2"
#define ATC_GSM_SMS_796_112_2_17_22_13_26_849 "ATC: CPMS: sms is not ready"
#define ATC_GSM_SMS_841_112_2_17_22_13_26_850 "ATC: CPMS: sms is not ready"
#define ATC_GSM_SMS_1112_112_2_17_22_13_27_851 "ATC: CSCA, sms is not ready!!!"
#define ATC_GSM_SMS_1151_112_2_17_22_13_27_852 "ATC: Get sc failure"
#define ATC_GSM_SMS_1205_112_2_17_22_13_27_853 "ATC: CNMI, only support <bm>=0,1 others as 1 (%d->1)"
#define ATC_GSM_SMS_1230_112_2_17_22_13_27_854 "ATC: CNMI, bfr:%d"
#define ATC_GSM_SMS_1454_112_2_17_22_13_28_855 "ATC:CMGS,Deactive CMMS timer"
#define ATC_GSM_SMS_1468_112_2_17_22_13_28_856 "ATC:s_sms_record[g_current_card_id].length=%d"
#define ATC_GSM_SMS_1604_112_2_17_22_13_28_857 "ATC: CMGW, no PARAM1"
#define ATC_GSM_SMS_1617_112_2_17_22_13_28_858 "ATC: CMGW, text mode, invalid parameters"
#define ATC_GSM_SMS_1639_112_2_17_22_13_28_859 "ATC: ATC_ProcessCMGW, text mode, ATC_ConvertTeleStrToStruct"
#define ATC_GSM_SMS_1650_112_2_17_22_13_28_860 "ATC: ATC_ProcessCMGW, text mode, invalid <tooa> parameter(%d)."
#define ATC_GSM_SMS_1673_112_2_17_22_13_28_861 "ATC: CMGW, text mode, stat need less than ATC_SMS_ALL(%d)"
#define ATC_GSM_SMS_1688_112_2_17_22_13_28_862 "ATC: CMGW, text mode, oa=%s,tooa=%d,stat=%d"
#define ATC_GSM_SMS_1700_112_2_17_22_13_28_863 "ATC: CMGW, PDU mode, invalid parameters"
#define ATC_GSM_SMS_1711_112_2_17_22_13_28_864 "ATC: CMGW, PDU mode, length: NOT (%d-%d)"
#define ATC_GSM_SMS_1719_112_2_17_22_13_28_865 "ATC: CMGW, PDU mode, stat need less than ATC_SMS_ALL(%d)"
#define ATC_GSM_SMS_1728_112_2_17_22_13_28_866 "ATC: CMGW, PDU mode, length=%d,stat=%d"
#define ATC_GSM_SMS_2002_112_2_17_22_13_29_867 "ATC: CMSS,ERROR, param2 is not string!"
#define ATC_GSM_SMS_2032_112_2_17_22_13_29_868 "ATC: ATC_ProcessCMSS, ERROR, invalid PDU mode paramete"
#define ATC_GSM_SMS_2065_112_2_17_22_13_29_869 "ATC: +CMSS toda=%d"
#define ATC_GSM_SMS_2159_112_2_17_22_13_29_870 "ATC: Read msg and language from NV fail"
#define ATC_GSM_SMS_2195_112_2_17_22_13_29_871 "ATC: CSCB param2 is not string type."
#define ATC_GSM_SMS_2202_112_2_17_22_13_29_872 "ATC: CSCB param2 is not correct values."
#define ATC_GSM_SMS_2218_112_2_17_22_13_29_873 "ATC:  CSCB param3 is not string type."
#define ATC_GSM_SMS_2224_112_2_17_22_13_29_874 "ATC: CSCB param2 is not correct values."
#define ATC_GSM_SMS_2329_112_2_17_22_13_30_875 "ATC:enter SMSC"
#define ATC_GSM_SMS_2333_112_2_17_22_13_30_876 "ATC: SMSC: sms is not ready"
#define ATC_GSM_SMS_2377_112_2_17_22_13_30_877 "ATC: SMSC, status_flag = TRUE"
#define ATC_GSM_SMS_2386_112_2_17_22_13_30_878 "ATC:SMSC, status_flag = FALSE"
#define ATC_GSM_SMS_2392_112_2_17_22_13_30_879 "ATC: SMSC, ChangeSmsStatus is FALSE"
#define ATC_GSM_SMS_2396_112_2_17_22_13_30_880 "ATC:cmgf:0,status_ptr=%d, sms_status=%d,at_sms_status=%d,status_flag=%d"
#define ATC_GSM_SMS_2427_112_2_17_22_13_30_881 "ATC:cmgf:1,status_ptr=%d, sms_status=%d,at_sms_status=%d,status_flag=%d"
#define ATC_GSM_SMS_2554_112_2_17_22_13_30_882 "ATC: CSMS = 0 ,CNMA not support"
#define ATC_GSM_SMS_2637_112_2_17_22_13_30_883 "ATC: CSMS = 0 ,CNMA not support"
#define ATC_GSM_SMS_2796_112_2_17_22_13_31_884 "ATC: The record length is %d,pdu_len:%d"
#define ATC_GSM_SMS_2821_112_2_17_22_13_31_885 "ATC:Call HandlePduModeSms()"
#define ATC_GSM_SMS_2833_112_2_17_22_13_31_886 "ATC:flag = %d"
#define ATC_GSM_SMS_2875_112_2_17_22_13_31_887 "ATC: The cnma input length is error %d != %d"
#define ATC_GSM_SMS_3015_112_2_17_22_13_31_888 "ATC: write NV_ATC_CONFIG_ID fail"
#define ATC_GSM_SMS_3051_112_2_17_22_13_31_889 "ATC: ATC_RestoreSmsConfig, Get sc failure"
#define ATC_GSM_SMS_3056_112_2_17_22_13_31_890 "ATC: ATC_RestoreSmsConfig, SIM not ready, cannot get sc"
#define ATC_GSM_SMS_3131_112_2_17_22_13_31_891 "ATC:error ConvertHexToBin"
#define ATC_GSM_SMS_3140_112_2_17_22_13_31_892 "ATC:no SC address"
#define ATC_GSM_SMS_3155_112_2_17_22_13_32_893 "ATC: HandlePduModeSms sc_addr.length = %d"
#define ATC_GSM_SMS_3170_112_2_17_22_13_32_894 "ATC:have SC address"
#define ATC_GSM_SMS_3235_112_2_17_22_13_32_895 "ATC: failure in MNSMS_AtcWriteSmsEx"
#define ATC_GSM_SMS_3313_112_2_17_22_13_32_896 "ATC_ProcessWriteSmsCnf: max_num=%d"
#define ATC_GSM_SMS_3314_112_2_17_22_13_32_897 "ATC_ProcessWriteSmsCnf: used_num=%d"
#define ATC_GSM_SMS_3323_112_2_17_22_13_32_898 "ATC: failure in MNSMS_AtcWriteSmsEx"
#define ATC_GSM_SMS_3355_112_2_17_22_13_32_899 "ATC: full in HandlePduSms_SpsmsFl"
#define ATC_GSM_SMS_3577_112_2_17_22_13_33_900 "ATC:ATC_ProcessSendSmsCnf, Active CMMS timer"
#define ATC_GSM_SMS_3584_112_2_17_22_13_33_901 "ATC: Receive the error status: %d"
#define ATC_GSM_SMS_3644_112_2_17_22_13_33_902 "ATC: Received APP_MN_UPDATE_SMS_STATE_CNF, sleep 30ms"
#define ATC_GSM_SMS_3754_112_2_17_22_13_33_903 "ATC: GetStorageInfo: WARNING, SmsNum is 0"
#define ATC_GSM_SMS_3764_112_2_17_22_13_33_904 "ATC: GetStorageInfo: no mem, SmsNum is %d"
#define ATC_GSM_SMS_3795_112_2_17_22_13_33_905 "ATC: total amout: %d, used amount:%d,unread flag:%d"
#define ATC_GSM_SMS_3911_112_2_17_22_13_33_906 "ATC: the current cmgl index: %d"
#define ATC_GSM_SMS_3938_112_2_17_22_13_33_907 "ATC:WRITE error, Stopping Filling SMS Storage!"
#define ATC_GSM_SMS_3957_112_2_17_22_13_33_908 "ATC:WRITE error, Stopping Filling SMS Storage!"
#define ATC_GSM_SMS_3985_112_2_17_22_13_33_909 "ATC:WRITE error!"
#define ATC_GSM_SMS_4003_112_2_17_22_13_33_910 "ATC:WRITE error!"
#define ATC_GSM_SMS_4021_112_2_17_22_13_33_911 "ATC:ATC_ProcessSmsFullInd, mem_status = %d"
#define ATC_GSM_SMS_4075_112_2_17_22_13_33_912 "ATC: Get sc failure"
#define ATC_GSM_SMS_4104_112_2_17_22_13_34_913 "ATC: ATC_ProcessSmsStatusReportInd  sig->dual_sys: %d"
#define ATC_GSM_SMS_4172_112_2_17_22_13_34_914 "ATC: disable the status report"
#define ATC_GSM_SMS_4254_112_2_17_22_13_34_915 "ATC: invalid <text> length:%d."
#define ATC_GSM_SMS_4276_112_2_17_22_13_34_916 "ATC: text sms length:%d,dcs:%d."
#define ATC_GSM_SMS_4343_112_2_17_22_13_34_917 "ATC: MNSMS_EncodeUserData failed."
#define ATC_GSM_SMS_4389_112_2_17_22_13_34_918 "ATC: MNSMS_EncodeUserData failed."
#define ATC_GSM_SMS_4398_112_2_17_22_13_34_919 "ATC:status_report_is_request=%d"
#define ATC_GSM_SMS_4446_112_2_17_22_13_34_920 "ATC: call MNSMS_WriteMtSmsEx"
#define ATC_GSM_SMS_4450_112_2_17_22_13_34_921 "ATC: failure in MNSMS_WriteMtSmsEx"
#define ATC_GSM_SMS_4465_112_2_17_22_13_34_922 "ATC: failure in MNSMS_WriteMoSmsEx"
#define ATC_GSM_SMS_4546_112_2_17_22_13_34_923 "ATC_ProcessWriteSmsCnf: max_num=%d"
#define ATC_GSM_SMS_4547_112_2_17_22_13_34_924 "ATC_ProcessWriteSmsCnf: used_num=%d"
#define ATC_GSM_SMS_4553_112_2_17_22_13_34_925 "ATC: call MNSMS_WriteMtSmsEx"
#define ATC_GSM_SMS_4557_112_2_17_22_13_34_926 "ATC: failure in MNSMS_WriteMtSmsEx"
#define ATC_GSM_SMS_4586_112_2_17_22_13_35_927 "ATC: failure in MNSMS_WriteMoSmsEx"
#define ATC_GSM_SMS_4615_112_2_17_22_13_35_928 "ATC: full in HandleTextModeSms_SpsmsFL"
#define ATC_GSM_SMS_4705_112_2_17_22_13_35_929 "ATC: HandleTextModeSmsReadCnf, dual_sys: %d sms_status:%d,record_id:%d"
#define ATC_GSM_SMS_4713_112_2_17_22_13_35_930 "ATC: SMS FREE SPACE!"
#define ATC_GSM_SMS_5014_112_2_17_22_13_35_931 "ATC: Received  APP_MN_READ_SMS_CNF, text mode, sleep 30ms"
#define ATC_GSM_SMS_5181_112_2_17_22_13_36_932 "ATC: HandlePduModeSmsReadCnf,dual_sys:%d, sms_status:%d,record_id:%d"
#define ATC_GSM_SMS_5189_112_2_17_22_13_36_933 "ATC: Read a free SMS space."
#define ATC_GSM_SMS_5243_112_2_17_22_13_36_934 "atc: george for status report"
#define ATC_GSM_SMS_5321_112_2_17_22_13_36_935 "ATC: Received  APP_MN_READ_SMS_CNF, pdu mode, sleep 30ms"
#define ATC_GSM_SMS_5518_112_2_17_22_13_36_936 "ATC: The temp_len is %d"
#define ATC_GSM_SMS_5548_112_2_17_22_13_36_937 "ATC: ReadNextSMS, dual_sys = %d"
#define ATC_GSM_SMS_5552_112_2_17_22_13_36_938 "ATC: ReadNextSMS, sleep 30ms"
#define ATC_GSM_SMS_5561_112_2_17_22_13_36_939 "ATC: ReadNextSMS, finish"
#define ATC_GSM_SMS_5675_112_2_17_22_13_37_940 "ATC: NO MEM!!!in DeleteSpecSms "
#define ATC_GSM_SMS_5750_112_2_17_22_13_37_941 "ATC:the current cmgl index: %d"
#define ATC_GSM_SMS_5951_112_2_17_22_13_37_942 "ATC: Write NV_ATC_CB_MSG_ID failed"
#define ATC_GSM_SMS_5956_112_2_17_22_13_37_943 "ATC: Write NV_ATC_CB_LANG_ID failed"
#define ATC_GSM_SMS_6015_112_2_17_22_13_37_944 "ATC: The temp_len is %d"
#define ATC_GSM_SMS_6055_112_2_17_22_13_37_945 "ATC: ATC_ProcessCMMS, no PARAM1"
#define ATC_GSM_SMS_6119_112_2_17_22_13_38_946 "ATC: CMMSTimeout: WARNING! DISABLE == cmms_value"
#define ATC_GSM_SMS_6124_112_2_17_22_13_38_947 "ATC: CMMSTimeout: cmms_value: KEEP_ENABLED->DISABLE"
#define ATC_GSM_SMS_6130_112_2_17_22_13_38_948 "ATC: CMMSTimeout: cmms_value: ENABLED"
#define ATC_GSM_SMS_6135_112_2_17_22_13_38_949 "ATC: CMMSTimeout: WARNING! unknown cmms_value:%d"
#define ATC_GSM_SMS_6149_112_2_17_22_13_38_950 "ATC: ATC_ActiveCMMSTimer, dual_sys = %d"
#define ATC_GSM_SMS_6160_112_2_17_22_13_38_951 "ATC: ATC_ActiveCMMSTimer, active timer(cmms_value=%d,cmms_flag=%d)"
#define ATC_GSM_SMS_6168_112_2_17_22_13_38_952 "ATC: ATC_ActiveCMMSTimer, deactive timer(cmms_value=%d,cmms_flag=%d)"
#define ATC_GSM_SMS_6287_112_2_17_22_13_38_953 "ATC: GetValidityPeriod, warning, invalid absolute period"
#define ATC_GSM_SMS_6360_112_2_17_22_13_38_954 "ATC: TransfromStrToBcd failed."
#define ATC_GSM_SMS_6366_112_2_17_22_13_38_955 "ATC: invalid bcd length:%d"
#define ATC_GSM_SMS_6440_112_2_17_22_13_38_956 "ATC: ATC_EncodeSMSDataToTE, invalid parameters."
#define ATC_GSM_SMS_6480_112_2_17_22_13_38_957 "enter into Atc_FillMtMoSmsEx"
#define ATC_GSM_SMS_6526_112_2_17_22_13_38_958 "enter into Atc_FreeMemAppliedForSfsmsm"
#define ATC_GSM_SS_268_112_2_17_22_13_39_959 "ATC_EnableVoiceCodec: is_enable = %d"
#define ATC_GSM_SS_330_112_2_17_22_13_39_960 "ATC: ATC_ProcessCCFC, too many params(more than 2 is allowed if REGISTRATION)"
#define ATC_GSM_SS_337_112_2_17_22_13_39_961 "ATC: CCFC,  param 4 or 7 wrong values!"
#define ATC_GSM_SS_363_112_2_17_22_13_39_962 "ATC: CCFC, Class not allowed"
#define ATC_GSM_SS_380_112_2_17_22_13_39_963 "ATC: CCFC, MNSS_DeactivateSS, failure:%d"
#define ATC_GSM_SS_388_112_2_17_22_13_39_964 "ATC: CCFC, Class not allowed"
#define ATC_GSM_SS_405_112_2_17_22_13_39_965 "ATC: CCFC, MNSS_ActivateSS, failure:%d"
#define ATC_GSM_SS_413_112_2_17_22_13_39_966 "ATC: CCFC, Class not allowed"
#define ATC_GSM_SS_435_112_2_17_22_13_39_967 "ATC: CCFC, MNSS_InterrogateService, failure:%d"
#define ATC_GSM_SS_447_112_2_17_22_13_39_968 "ATC: reg fail1"
#define ATC_GSM_SS_454_112_2_17_22_13_39_969 "ATC: CCFC, param3 is not string!"
#define ATC_GSM_SS_469_112_2_17_22_13_40_970 "ATC: reg fail2"
#define ATC_GSM_SS_480_112_2_17_22_13_40_971 "ATC: reg fail3"
#define ATC_GSM_SS_488_112_2_17_22_13_40_972 "ATC: Subaddr is not string"
#define ATC_GSM_SS_496_112_2_17_22_13_40_973 "ATC: reg fail4"
#define ATC_GSM_SS_516_112_2_17_22_13_40_974 "ATC: CCFC, Class not allowed"
#define ATC_GSM_SS_532_112_2_17_22_13_40_975 "ATC: CCFC, MNSS_RegisterSSInfo, failure:%d"
#define ATC_GSM_SS_544_112_2_17_22_13_40_976 "ATC: CCFC, Class not allowed"
#define ATC_GSM_SS_561_112_2_17_22_13_40_977 "ATC: CCFC, MNSS_EraseSSInfo, failure:%d"
#define ATC_GSM_SS_570_112_2_17_22_13_40_978 "ATC: re_valuse=%x"
#define ATC_GSM_SS_792_112_2_17_22_13_40_979 "ATC: APP_MN_SETUP_COMPLETE_CNF"
#define ATC_GSM_SS_799_112_2_17_22_13_40_980 "ATC: APP_MN_SETUP_COMPLETE_IND"
#define ATC_GSM_SS_813_112_2_17_22_13_40_981 "ATC: ATC_ProcessSetupCompleteSignal, invalid call index."
#define ATC_GSM_SS_827_112_2_17_22_13_40_982 "ATC: setup complete expire timer check."
#define ATC_GSM_SS_1154_112_2_17_22_13_41_983 "ATC: user set the AOC mode"
#define ATC_GSM_SS_1215_112_2_17_22_13_41_984 "ATC: CACM 1"
#define ATC_GSM_SS_1222_112_2_17_22_13_41_985 "ATC:CACM 1-1"
#define ATC_GSM_SS_1227_112_2_17_22_13_41_986 "ATC: CACM 1-2"
#define ATC_GSM_SS_1250_112_2_17_22_13_41_987 "ATC: CACM 2(1)"
#define ATC_GSM_SS_1252_112_2_17_22_13_41_988 "ATC: CACM 2(2)"
#define ATC_GSM_SS_1255_112_2_17_22_13_41_989 "ATC:CACM 2-1"
#define ATC_GSM_SS_1260_112_2_17_22_13_41_990 "ATC: CACM 2-2"
#define ATC_GSM_SS_1315_112_2_17_22_13_41_991 "ATC: CAMM, param1 is not string!"
#define ATC_GSM_SS_1320_112_2_17_22_13_41_992 "ATC: CAMM, param1 length is 0!"
#define ATC_GSM_SS_1325_112_2_17_22_13_41_993 "ATC: CAMM, param1 length is too long >6 !"
#define ATC_GSM_SS_1332_112_2_17_22_13_41_994 "ATC: CAMM, param1 format error!"
#define ATC_GSM_SS_1350_112_2_17_22_13_41_995 "ATC: CAMM, param2 is not string!"
#define ATC_GSM_SS_1414_112_2_17_22_13_41_996 "ATC: ATC_DecStrToE, no valid string data!"
#define ATC_GSM_SS_1426_112_2_17_22_13_41_997 "ATC: ATC_DecStrToE, more '.'"
#define ATC_GSM_SS_1435_112_2_17_22_13_41_998 "ATC: ATC_DecStrToE, NOT '0' ~ '9'!"
#define ATC_GSM_SS_1444_112_2_17_22_13_41_999 "ATC: ATC_DecStrToE, too many digits!"
#define ATC_GSM_SS_1449_112_2_17_22_13_41_1000 "ATC: ATC_DecStrToE, too big value!"
#define ATC_GSM_SS_1515_112_2_17_22_13_42_1001 "ATC: CPUC, param1 is not string!"
#define ATC_GSM_SS_1540_112_2_17_22_13_42_1002 "ATC: CPUC, param2 is not string!"
#define ATC_GSM_SS_1545_112_2_17_22_13_42_1003 "ATC: CPUC, param2 len is 0"
#define ATC_GSM_SS_1552_112_2_17_22_13_42_1004 "ATC: CPUC, ATC_DecStrToE error!"
#define ATC_GSM_SS_1570_112_2_17_22_13_42_1005 "ATC: CPUC, param3 is not string!"
#define ATC_GSM_SS_1639_112_2_17_22_13_42_1006 "ATC: ATC_PROCESSCHVCNF, chv_status=%d, pin_function_type=%d"
#define ATC_GSM_SS_1658_112_2_17_22_13_42_1007 "ATC: ATC_PROCESSCHVCNF, ATC_PIN_RESET_ACM_UNIT"
#define ATC_GSM_SS_1670_112_2_17_22_13_42_1008 "ATC: ATC_PROCESSCHVCNF, ATC_PIN_ENABLE_FDN"
#define ATC_GSM_SS_1675_112_2_17_22_13_42_1009 "ATC: ATC_PROCESSCHVCNF, ATC_PIN_DISABLE_FDN"
#define ATC_GSM_SS_1760_112_2_17_22_13_42_1010 "ATC: ATC_ProcessPuctCnf, Read: eppu:%d,ex_sign:%d,ex:%d,ex_value:%d"
#define ATC_GSM_SS_2164_112_2_17_22_13_43_1011 "ATC: MNSS_UserOriginateUSSD"
#define ATC_GSM_SS_2183_112_2_17_22_13_43_1012 "ATC: MNSS_ResponseUSSD"
#define ATC_GSM_SS_2249_112_2_17_22_13_43_1013 "ATC: CUSD, param3 is not num!"
#define ATC_GSM_SS_2274_112_2_17_22_13_43_1014 "ATC: CUSD, param2 is not string!"
#define ATC_GSM_SS_2281_112_2_17_22_13_43_1015 "ATC: CUSD, the string is too long"
#define ATC_GSM_SS_2307_112_2_17_22_13_43_1016 "ATC: <dcs> is %d."
#define ATC_GSM_SS_2346_112_2_17_22_13_43_1017 "ATC: ATC_ProcessUssdInd, dual_sys: %d, s_cusd_presentation:%d,s_cusd_isresponse:%d"
#define ATC_GSM_SS_2376_112_2_17_22_13_43_1018 "ATC: ATC_ProcessUssdNotifyInd, dual_sys: %d, s_cusd_isresponse:%d,s_cusd_presentation:%d"
#define ATC_GSM_SS_2401_112_2_17_22_13_43_1019 "ATC: ATC_ProcessUssdServiceCnf, dual_sys: %d, s_cusd_isresponse:%d,s_cusd_presentation:%d"
#define ATC_GSM_SS_2406_112_2_17_22_13_43_1020 "ATC: MN_SS_OPERATION_SUCC"
#define ATC_GSM_SS_2411_112_2_17_22_13_43_1021 "ATC: MN_SS_OPERATION_ERROR,error code:%d"
#define ATC_GSM_SS_2416_112_2_17_22_13_43_1022 "ATC: MN_SS_OPERATION_REJECT,error code:%d"
#define ATC_GSM_SS_2702_112_2_17_22_13_44_1023 "ATC: ATC_ProcessBarredInd, unknown barred_operation:%d"
#define ATC_GSM_SS_3024_112_2_17_22_13_45_1024 "ATC: ATC_ProcessInterCnf,cfall=%d,cfu=%d,cfcond=%d,cfb=%d,cfnry=%d,cfnrc=%d"
#define ATC_GSM_SS_3042_112_2_17_22_13_45_1025 "ATC: ATC_ProcessInterCnf, unknown s_ss_context.cmd_id: %d"
#define ATC_GSM_SS_3152_112_2_17_22_13_45_1026 "ATC: ATC_GetParams=FALSE"
#define ATC_GSM_SS_3186_112_2_17_22_13_45_1027 "ATC: WARNING, the length of passward is wrong!"
#define ATC_GSM_SS_3230_112_2_17_22_13_45_1028 "ATC: is_sim_busy=%d"
#define ATC_GSM_SS_3234_112_2_17_22_13_45_1029 "ATC: SIM is busy"
#define ATC_GSM_SS_3407_112_2_17_22_13_45_1030 "ATC: CLCK ATC_CLCK_QUERY_STATUS is_fdn_enable=%d"
#define ATC_GSM_SS_3426_112_2_17_22_13_45_1031 "ATC: CPIN pin1 enable flag: %d"
#define ATC_GSM_SS_3479_112_2_17_22_13_45_1032 "ATC: basic_service.service_code=%d"
#define ATC_GSM_SS_3534_112_2_17_22_13_46_1033 "ATC: cpwd parameter is not correct"
#define ATC_GSM_SS_3545_112_2_17_22_13_46_1034 "ATC: cpwd command type is set"
#define ATC_GSM_SS_3568_112_2_17_22_13_46_1035 "ATC: WARNING, the length of passward is wrong!"
#define ATC_GSM_SS_3587_112_2_17_22_13_46_1036 "ATC: WARNING, the length of passward is wrong!"
#define ATC_GSM_SS_3609_112_2_17_22_13_46_1037 "ATC: cpwd sim is not ready"
#define ATC_GSM_SS_3631_112_2_17_22_13_46_1038 "ATC: cpwd sim is not ready"
#define ATC_GSM_SS_3660_112_2_17_22_13_46_1039 "ATC: cpwd password length is large"
#define ATC_GSM_SS_3680_112_2_17_22_13_46_1040 "ATC: cpwd command type is read"
#define ATC_GSM_SS_3684_112_2_17_22_13_46_1041 "ATC: cpwd command type is test"
#define ATC_GSM_SS_3713_112_2_17_22_13_46_1042 "ATC:ATC_ProcessGetPswInd"
#define ATC_GSM_SS_3717_112_2_17_22_13_46_1043 "ATC:AT_CMD_CPWD"
#define ATC_GSM_SS_3721_112_2_17_22_13_46_1044 "ATC:s_old_password"
#define ATC_GSM_SS_3727_112_2_17_22_13_46_1045 "ATC:s_new_password"
#define ATC_GSM_SS_3732_112_2_17_22_13_46_1046 "ATC:s_new_password again"
#define ATC_GSM_SS_3737_112_2_17_22_13_46_1047 "ATC: received error singal in get password indicate!!!"
#define ATC_GSM_SS_3741_112_2_17_22_13_46_1048 "ATC:s_clck_psw"
#define ATC_GSM_SS_3785_112_2_17_22_13_46_1049 "ATC: APP_MN_RELEASE_SS_IND"
#define ATC_GSM_SS_3941_112_2_17_22_13_46_1050 "ATC: ATC_ProcessSSCnf result=%d,list_num=%d,ss_status_present=%d,ss_status=%d,cmd_id=%d,ss_code=%d"
#define ATC_GSM_SS_4035_112_2_17_22_13_47_1051 "ATC: ATC_ProcessSSCnf,cfall=%d,cfu=%d,cfcond=%d,cfb=%d,cfnry=%d,cfnrc=%d"
#define ATC_GSM_SS_4069_112_2_17_22_13_47_1052 "ATC: ATC_CheckInterrogateCnf result=%d,list_num=%d,ss_status_present=%d,ss_status=%d,service_type=%d,service_code=%d"
#define ATC_GSM_SS_4392_112_2_17_22_13_47_1053 "ATC: build MPTY call successfully"
#define ATC_GSM_SS_4398_112_2_17_22_13_47_1054 "ATC: build MPTY call failed"
#define ATC_GSM_SS_4418_112_2_17_22_13_47_1055 "ATC: receive the APP_MN_CALL_RETRIEVED_CNF signal"
#define ATC_GSM_SS_4528_112_2_17_22_13_48_1056 "ATC: receive the APP_MN_CALL_HELD_CNF signal"
#define ATC_GSM_SS_4599_112_2_17_22_13_48_1057 "ATC:is_swap"
#define ATC_GSM_SS_4642_112_2_17_22_13_48_1058 "ATC: receive the APP_MN_SPLIT_MPTY_CNF signal"
#define ATC_GSM_SS_4760_112_2_17_22_13_48_1059 "ATC: enter ATC_CheckSSStr, dual_sys = %d"
#define ATC_GSM_SS_4761_112_2_17_22_13_48_1060 "ATC: length = %d, str_ptr = %s"
#define ATC_GSM_SS_4767_112_2_17_22_13_48_1061 "ATC: MNPHONE_ParseBStringEx operation = %d!"
#define ATC_GSM_SS_4786_112_2_17_22_13_48_1062 "ATC: begain MNSS_UserOriginateUSSDEx"
#define ATC_GSM_SS_4792_112_2_17_22_13_48_1063 "ATC: MNSS_UserOriginateUSSDEx result = %d"
#define ATC_GSM_SS_4804_112_2_17_22_13_48_1064 "ATC: begain MNSS_ResponseUSSDEx"
#define ATC_GSM_SS_4808_112_2_17_22_13_48_1065 "ATC: MNSS_ResponseUSSDEx result = %d"
#define ATC_GSM_SS_4891_112_2_17_22_13_48_1066 "ATC: unknow ss code = %d"
#define ATC_GSM_SS_4918_112_2_17_22_13_48_1067 "ATC: enter ATC_SendReqToSs dual_sys=%d"
#define ATC_GSM_SS_4935_112_2_17_22_13_48_1068 "ATC: mn register ss error code = %d"
#define ATC_GSM_SS_4953_112_2_17_22_13_48_1069 "ATC: mn erase ss info error code = %d"
#define ATC_GSM_SS_4975_112_2_17_22_13_48_1070 "ATC: mn active ss error code %d"
#define ATC_GSM_SS_4995_112_2_17_22_13_48_1071 "ATC: mn deactive ss error code = %d"
#define ATC_GSM_SS_5015_112_2_17_22_13_49_1072 "ATC: mn interrogate service error code = %d"
#define ATC_GSM_SS_5034_112_2_17_22_13_49_1073 "ATC: registerpassword ss_code = %d"
#define ATC_GSM_SS_5038_112_2_17_22_13_49_1074 "ATC: mn register ss password error code = %d"
#define ATC_GSM_SS_5052_112_2_17_22_13_49_1075 "ATC: SendReqToSs in default!"
#define ATC_GSM_SS_5140_112_2_17_22_13_49_1076 "ATC_getclass_from_interrogate_cnf:service_class=%d"
#define ATC_GSM_SS_5265_112_2_17_22_13_49_1077 "ATC: ATC_InterrogateCFService, NOT Call Forward Code (%d)!"
#define ATC_GSM_SS_5273_112_2_17_22_13_49_1078 "ATC: ATC_InterrogateCFService, failure in MN (ss_code:%d)"
#define ATC_GSM_SS_5280_112_2_17_22_13_49_1079 "ATC: ATC_InterrogateCFService, WARNING! s_ss_context[dual_sys](%d,%d) is already existed."
#define ATC_GSM_SS_5301_112_2_17_22_13_49_1080 "ATC: CCWE, no param1"
#define ATC_GSM_STK_220_112_2_17_22_13_50_1081 "ATC:ATC_InitCnfSetMenu, dual_sys:%d"
#define ATC_GSM_STK_241_112_2_17_22_13_50_1082 "ATC:enter SetupMenu, dual_sys=%d"
#define ATC_GSM_STK_245_112_2_17_22_13_50_1083 "ATC: SetupMenu,ERROR,s_stk_mainmenu_ptr[%d] is NULL."
#define ATC_GSM_STK_440_112_2_17_22_13_50_1084 "ATC:DisplayText:STK_DCS_8_BIT_ALPHABET or STK_DCS_UNPACKED_ALPHABET "
#define ATC_GSM_STK_455_112_2_17_22_13_50_1085 "ATC:DisplayText:STK_DCS_DEFAULT_ALPHABET"
#define ATC_GSM_STK_466_112_2_17_22_13_50_1086 "ATC: error length in simat."
#define ATC_GSM_STK_484_112_2_17_22_13_50_1087 "ATC:DisplayText:STK_DCS_UCS2_ALPHABET"
#define ATC_GSM_STK_514_112_2_17_22_13_50_1088 "ATC:DisplayText,%d,%d"
#define ATC_GSM_STK_551_112_2_17_22_13_50_1089 "ATC:GetInkey:STK_DCS_8_BIT_ALPHABET or STK_DCS_UNPACKED_ALPHABET"
#define ATC_GSM_STK_574_112_2_17_22_13_50_1090 "ATC: error data length in simat"
#define ATC_GSM_STK_591_112_2_17_22_13_50_1091 "ATC:GetInkey:STK_DCS_UCS2_ALPHABET"
#define ATC_GSM_STK_622_112_2_17_22_13_51_1092 "ATC:GetInkey,%d,%d"
#define ATC_GSM_STK_651_112_2_17_22_13_51_1093 "ATC: process get input dcs = %d, str_len = %d"
#define ATC_GSM_STK_697_112_2_17_22_13_51_1094 "ATC:GetInput:STK_DCS_8_BIT_ALPHABET or STK_DCS_UNPACKED_ALPHABET"
#define ATC_GSM_STK_722_112_2_17_22_13_51_1095 "ATC: error length in simat."
#define ATC_GSM_STK_744_112_2_17_22_13_51_1096 "ATC:GetInput:STK_DCS_UCS2_ALPHABET"
#define ATC_GSM_STK_794_112_2_17_22_13_51_1097 "ATC: enter ATC_ProcessSetupCall"
#define ATC_GSM_STK_937_112_2_17_22_13_51_1098 "ATC:select_item.is_alpha_id==TRUE"
#define ATC_GSM_STK_952_112_2_17_22_13_51_1099 "ATC:select_item.is_alpha_id.id_ptr[0]==ATC_UCS2_CODE"
#define ATC_GSM_STK_963_112_2_17_22_13_51_1100 "ATC:select_item.is_alpha_id.id_ptr[0]!=ATC_UCS2_CODE"
#define ATC_GSM_STK_1011_112_2_17_22_13_52_1101 "s_stk_signal.menu_item_id[%d]:%d"
#define ATC_GSM_STK_1149_112_2_17_22_13_52_1102 "ATC: SendSMS SC is exist."
#define ATC_GSM_STK_1161_112_2_17_22_13_52_1103 "ATC: SendSMS sc_addr.length = %d"
#define ATC_GSM_STK_1173_112_2_17_22_13_52_1104 "ATC: SendSMS sc_addr.length = %d"
#define ATC_GSM_STK_1178_112_2_17_22_13_52_1105 "ATC: address is not exist."
#define ATC_GSM_STK_1247_112_2_17_22_13_52_1106 "ATC: SMS is packed."
#define ATC_GSM_STK_1254_112_2_17_22_13_52_1107 "ATC: send sms pdu err code = %d."
#define ATC_GSM_STK_1266_112_2_17_22_13_52_1108 "ATC: SMS is unpacked."
#define ATC_GSM_STK_1274_112_2_17_22_13_52_1109 "ATC: send sms pdu err code = %d."
#define ATC_GSM_STK_1301_112_2_17_22_13_52_1110 "ATC:enter ATC_ProcessPlayTone"
#define ATC_GSM_STK_1580_112_2_17_22_13_53_1111 "ATC: user originate ussd fail, err code = %d."
#define ATC_GSM_STK_1625_112_2_17_22_13_53_1112 "ATC:SetupEventList,num of events:%d"
#define ATC_GSM_STK_1673_112_2_17_22_13_53_1113 "ATC:SetupIdleModeText:STK_DEFAULT_ALPHABET"
#define ATC_GSM_STK_1685_112_2_17_22_13_53_1114 "ATC:SetupIdleModeText:STK_DCS_DEFAULT_ALPHABET"
#define ATC_GSM_STK_1690_112_2_17_22_13_53_1115 "ATC: error data length in simat"
#define ATC_GSM_STK_1704_112_2_17_22_13_53_1116 "ATC:SetupIdleModeText:STK_DCS_UCS2_ALPHABET"
#define ATC_GSM_STK_1729_112_2_17_22_13_53_1117 "ATC:SetupIdleModeText,dcs:%d,text_len:%d"
#define ATC_GSM_STK_1805_112_2_17_22_13_53_1118 "ATC:SendDtmf"
#define ATC_GSM_STK_1880_112_2_17_22_13_53_1119 "%s"
#define ATC_GSM_STK_1882_112_2_17_22_13_53_1120 "ATC:Refresh"
#define ATC_GSM_STK_2004_112_2_17_22_13_54_1121 "ATC:LauchBrowser"
#define ATC_GSM_STK_2248_112_2_17_22_13_54_1122 "ATC:OpenChannel"
#define ATC_GSM_STK_2326_112_2_17_22_13_54_1123 "ATC:ATC_ProcessCloseChannel"
#define ATC_GSM_STK_2409_112_2_17_22_13_55_1124 "ATC:ReceiveData"
#define ATC_GSM_STK_2500_112_2_17_22_13_55_1125 "ATC:LauchBrowser"
#define ATC_GSM_STK_2523_112_2_17_22_13_55_1126 "ATC:LauchBrowser"
#define ATC_GSM_STK_2543_112_2_17_22_13_55_1127 "ATC: RefreshCnf, is_ok=%d,is_file_changed=%d"
#define ATC_GSM_STK_2586_112_2_17_22_13_55_1128 "ATC:ATC_STK SmsCnf,MN_SMS_OPERATE_SUCCESS"
#define ATC_GSM_STK_2594_112_2_17_22_13_55_1129 "ATC:ATC_STK SendSmsCnf,MN_SMS_FAIL"
#define ATC_GSM_STK_2625_112_2_17_22_13_55_1130 "ATC:ATC_STK SendUssdCnf,MN_SMS_OPERATE_SUCCESS"
#define ATC_GSM_STK_2635_112_2_17_22_13_55_1131 "ATC:ATC_STK SendUssdCnf,MN_USSD_FAIL"
#define ATC_GSM_STK_2662_112_2_17_22_13_55_1132 "ATC:ATC_STK SendSsCnf,MN_SMS_OPERATE_SUCCESS"
#define ATC_GSM_STK_2673_112_2_17_22_13_55_1133 "ATC:ATC_STK SendSsCnf,MN_SS_FAIL"
#define ATC_GSM_STK_2700_112_2_17_22_13_55_1134 "ATC:ATC_STKProcessSendSsRegPwdCnf,MN_SMS_OPERATE_SUCCESS"
#define ATC_GSM_STK_2709_112_2_17_22_13_55_1135 "ATC:ATC_STKProcessSendSsRegPwdCnf,MN_SS_FAIL"
#define ATC_GSM_STK_2735_112_2_17_22_13_55_1136 "[ATC_MainMenuCnf]::n_select= %d"
#define ATC_GSM_STK_2740_112_2_17_22_13_55_1137 "ATC: ATC_MainMenuCnf,ERROR,s_stk_mainmenu_ptr is NULL."
#define ATC_GSM_STK_2775_112_2_17_22_13_55_1138 "ATC:ATC_MainMenuCnf=%d"
#define ATC_GSM_STK_2866_112_2_17_22_13_55_1139 "ATC:ATC_CheckAndHandleGetInkey,SIMAT_DIGIT_ONLY,%d"
#define ATC_GSM_STK_2879_112_2_17_22_13_55_1140 "ATC:ATC_CheckAndHandleGetInkey,SIMAT_SMS_ALPHABET,%d"
#define ATC_GSM_STK_2897_112_2_17_22_13_55_1141 "ATC:ATC_CheckAndHandleGetInkey,SIMAT_UCS2_ALPHABET,%d"
#define ATC_GSM_STK_2923_112_2_17_22_13_56_1142 "ATC:ATC_CheckAndHandleGetInkey,SIMAT_UCS2_ALPHABET,%d"
#define ATC_GSM_STK_2935_112_2_17_22_13_56_1143 "ATC: re_value=%d"
#define ATC_GSM_STK_2984_112_2_17_22_13_56_1144 "[ATC_GetInkeyCnf]::ATC_CheckAndHandleGetInkey==FALSE"
#define ATC_GSM_STK_3001_112_2_17_22_13_56_1145 "[ATC_GetInkeyCnf]::re_value=%d"
#define ATC_GSM_STK_3053_112_2_17_22_13_56_1146 "ATC: getinput type = %d, str_len = %d, min_len = %d max_len = %d"
#define ATC_GSM_STK_3067_112_2_17_22_13_56_1147 "ATC:error in length"
#define ATC_GSM_STK_3082_112_2_17_22_13_56_1148 "ATC:ATC_CheckAndHandleGetInput,SIMAT_DIGIT_ONLY,%d"
#define ATC_GSM_STK_3089_112_2_17_22_13_56_1149 "ATC:error in length"
#define ATC_GSM_STK_3099_112_2_17_22_13_56_1150 "ATC:ATC_CheckAndHandleGetInput,SIMAT_INPUT_SMS_ALPHABET,%d"
#define ATC_GSM_STK_3107_112_2_17_22_13_56_1151 "ATC:error in length"
#define ATC_GSM_STK_3121_112_2_17_22_13_56_1152 "ATC:SIMAT_INPUT_UCS2_ALPHABET,%d"
#define ATC_GSM_STK_3128_112_2_17_22_13_56_1153 "ATC:error in length"
#define ATC_GSM_STK_3141_112_2_17_22_13_56_1154 "ATC:SIMAT_INPUT_DUAL_DIGIT,%d"
#define ATC_GSM_STK_3186_112_2_17_22_13_56_1155 "[ATC_GetInputCnf]::enter ATC_GetInputCnf,%d,%d"
#define ATC_GSM_STK_3215_112_2_17_22_13_56_1156 "[ATC_GetInputCnf]::ATC_CheckAndHandleGetInput==FALSE"
#define ATC_GSM_STK_3373_112_2_17_22_13_56_1157 "ATC:ATC_StkProcessCallSignals, dual_sys=%d"
#define ATC_GSM_STK_3378_112_2_17_22_13_56_1158 "ATC:IsCallSucc == TRUE"
#define ATC_GSM_STK_3385_112_2_17_22_13_56_1159 "ATC:IsCallSucc == FALSE"
#define ATC_GSM_STK_3479_112_2_17_22_13_57_1160 "ATC:ATC_HandleSetupCall atc_config_ptr->call_link_id = %d "
#define ATC_GSM_STK_3501_112_2_17_22_13_57_1161 "ATC:ATC_HandleSetupCall atc_config_ptr->call_link_id = %d "
#define ATC_GSM_STK_3538_112_2_17_22_13_57_1162 "[ATC_SetupCallCnf]::ATC_SetupCallCnf , dual_sys:%d"
#define ATC_GSM_STK_3565_112_2_17_22_13_57_1163 "[ATC_SetupCallCnf]::ATC_SetupCallCnf,ATC_SETUP_CALL"
#define ATC_GSM_STK_3606_112_2_17_22_13_57_1164 "ATC_SelectItemHandle n_select %d"
#define ATC_GSM_STK_3612_112_2_17_22_13_57_1165 "ATC_SelectItemHandle select_item.item_id %d"
#define ATC_GSM_STK_3748_112_2_17_22_13_57_1166 "bin.li add sim_terminal_profile!!!!!!"
#define ATC_GSM_STK_3772_112_2_17_22_13_57_1167 "ATC:: ATC_StringToByte pointer is NULL!"
#define ATC_GSM_STK_3823_112_2_17_22_13_57_1168 "ATC:: invalid number:%x"
#define ATC_GSM_STK_3859_112_2_17_22_13_57_1169 "ATC:before SIMAT_GetMeProfile"
#define ATC_GSM_STK_3863_112_2_17_22_13_57_1170 "result_terminal_profile[%d]:%x "
#define ATC_GSM_STK_3866_112_2_17_22_13_57_1171 "add length:%d"
#define ATC_GSM_STK_3887_112_2_17_22_13_57_1172 " ATC:ATC_STK_DEACTIVATE"
#define ATC_GSM_STK_3890_112_2_17_22_13_57_1173 "ATC:TRUE == s_stk_signal[g_current_card_id].is_support_stk"
#define ATC_GSM_STK_3918_112_2_17_22_13_57_1174 "ATC: stk deactivate fail."
#define ATC_GSM_STK_3924_112_2_17_22_13_58_1175 "ATC:return_value = ERR_OPERATION_NOT_ALLOWED"
#define ATC_GSM_STK_3930_112_2_17_22_13_58_1176 " ATC_STK_ACTIVATE !s_stk_signal.is_support_stk:%d"
#define ATC_GSM_STK_3966_112_2_17_22_13_58_1177 "ATC:FALSE == s_stk_signal[g_current_card_id].is_support_stk"
#define ATC_GSM_STK_3973_112_2_17_22_13_58_1178 " ATC_STK_CONFIGURE!"
#define ATC_GSM_STK_3981_112_2_17_22_13_58_1179 "ATC:ERR_INVALID_INDEX"
#define ATC_GSM_STK_3988_112_2_17_22_13_58_1180 "ATC: ATC_ProcessSTSF, param2 is not string!"
#define ATC_GSM_STK_3993_112_2_17_22_13_58_1181 "PARA2.str_ptr->str_len: %d"
#define ATC_GSM_STK_3997_112_2_17_22_13_58_1182 "ATC:ATC_CheckAndHandleConfig is false"
#define ATC_GSM_STK_4116_112_2_17_22_13_58_1183 "ATC: sio stk data_len = %d"
#define ATC_GSM_STK_4134_112_2_17_22_13_58_1184 "ATC:flag = %d"
#define ATC_GSM_STK_4200_112_2_17_22_13_58_1185 "ATC_CheckCondAndPara1 %d,%d"
#define ATC_GSM_STK_4209_112_2_17_22_13_58_1186 "para1_flag %d"
#define ATC_GSM_STK_4220_112_2_17_22_13_58_1187 "ERR_INVALID_INDEX"
#define ATC_GSM_STK_4251_112_2_17_22_13_58_1188 "ATC:ATC_MainMenuCnf=FALSE"
#define ATC_GSM_STK_5499_112_2_17_22_14_1_1189 "ATC:s_stk_signal.cur_status=%d"
#define ATC_GSM_STK_5502_112_2_17_22_14_1_1190 "ATC_GetCmdNumByString %d "
#define ATC_GSM_STK_5510_112_2_17_22_14_1_1191 "ATC_GetCmdNumByString %d "
#define ATC_GSM_STK_5515_112_2_17_22_14_1_1192 "ATC_GetCmdNumByString %d "
#define ATC_GSM_STK_5579_112_2_17_22_14_1_1193 "ATC:s_stk_signal.cur_status=%d"
#define ATC_GSM_STK_5600_112_2_17_22_14_1_1194 "ATC:s_stk_signal.cur_status=%d"
#define ATC_GSM_STK_5694_112_2_17_22_14_1_1195 "PARAM4_FLAG %d para4_num %d"
#define ATC_GSM_STK_5995_112_2_17_22_14_2_1196 "ATC_ProcessSTED stk_actflag %d,sim_flag %d,is_support_stk %d"
#define ATC_GSM_STK_6156_112_2_17_22_14_2_1197 "ATC:ATC_HandleProvidLocalInfo,info_type = %d"
#define ATC_GSM_STK_6187_112_2_17_22_14_2_1198 "ATC: get imei failed."
#define ATC_GSM_STK_6246_112_2_17_22_14_2_1199 "ATC_ProcessUpSTIN dual_sys:%d"
#define ATC_GSM_STK_6254_112_2_17_22_14_2_1200 "ATC_ProcessUpSTIN SCI_SET_TIMER 1s"
#define ATC_GSM_STK_6342_112_2_17_22_14_2_1201 "ATC_ProcessUpSTIN invalid status %d"
#define ATC_GSM_STK_6360_112_2_17_22_14_2_1202 "ATC:ATC_ProcessSTKSignal , dual_sys:%d"
#define ATC_GSM_STK_6374_112_2_17_22_14_2_1203 "ATC: ATC_ProcessSTKSignal,ERROR,s_stk_mainmenu_ptr is NULL."
#define ATC_GSM_STK_6403_112_2_17_22_14_2_1204 "ATC:STK_SETUP_MENU_REQ,%d"
#define ATC_GSM_STK_6421_112_2_17_22_14_2_1205 "ATC:STK_DISPLAY_TEXT_REQ"
#define ATC_GSM_STK_6440_112_2_17_22_14_3_1206 "ATC:STK_GET_INKEY_REQ"
#define ATC_GSM_STK_6458_112_2_17_22_14_3_1207 "ATC:STK_GET_INPUT_REQ"
#define ATC_GSM_STK_6475_112_2_17_22_14_3_1208 "ATC:STK_SETUP_CALL_REQ"
#define ATC_GSM_STK_6493_112_2_17_22_14_3_1209 "ATC:STK_SELECT_ITEM_REQ"
#define ATC_GSM_STK_6511_112_2_17_22_14_3_1210 "ATC:STK_SEND_SMS_REQ"
#define ATC_GSM_STK_6523_112_2_17_22_14_3_1211 "ATC:SIMAT_SEND_SS_REQ"
#define ATC_GSM_STK_6533_112_2_17_22_14_3_1212 "ATC:SIMAT_SEND_USSD_REQ"
#define ATC_GSM_STK_6543_112_2_17_22_14_3_1213 "ATC:SIMAT_PLAY_TONE_REQ"
#define ATC_GSM_STK_6553_112_2_17_22_14_3_1214 "ATC:SIMAT_PROVIDE_LOCAL_INFO"
#define ATC_GSM_STK_6558_112_2_17_22_14_3_1215 "ATC:STK_SESSION_TERMINATE_REQ"
#define ATC_GSM_STK_6589_112_2_17_22_14_3_1216 "ATC:SIMAT_SETUP_EVENT_LIST_REQ"
#define ATC_GSM_STK_6599_112_2_17_22_14_3_1217 "ATC:EV_MN_APP_SIMAT_SETUP_IDLE_TEXT_IND_F"
#define ATC_GSM_STK_6617_112_2_17_22_14_3_1218 "ATC:EV_MN_APP_SIMAT_SEND_DTMF_IND_F"
#define ATC_GSM_STK_6636_112_2_17_22_14_3_1219 "ATC:APP_MN_SIMAT_LANG_NOTIFY_IND"
#define ATC_GSM_STK_6641_112_2_17_22_14_3_1220 "ATC:SIMAT_EVENT_REFRESH"
#define ATC_GSM_STK_6645_112_2_17_22_14_3_1221 "ATC:SIMAT_EVENT_CC_RSP"
#define ATC_GSM_STK_6649_112_2_17_22_14_3_1222 "ATC:SIMAT_EVENT_SS_CON_RSP"
#define ATC_GSM_STK_6653_112_2_17_22_14_3_1223 "ATC:SIMAT_EVENT_USSD_CON_RSP"
#define ATC_GSM_STK_6657_112_2_17_22_14_3_1224 "ATC:APP_MN_SIMAT_SMS_CTL_TO_MMI_IND"
#define ATC_GSM_STK_6661_112_2_17_22_14_3_1225 "ATC:SIMAT_EVENT_REFRESH"
#define ATC_GSM_STK_6665_112_2_17_22_14_3_1226 "ATC:SIMAT_EVENT_SMS_PP_RSP"
#define ATC_GSM_STK_6668_112_2_17_22_14_3_1227 "ATC:SIMAT_REFRSH_IND"
#define ATC_GSM_STK_6680_112_2_17_22_14_3_1228 "ATC:APP_MN_SIMAT_LAUNCH_BROWSER_IND"
#define ATC_GSM_STK_6691_112_2_17_22_14_3_1229 "ATC:APP_MN_SIMAT_SEND_DATA_IND"
#define ATC_GSM_STK_6704_112_2_17_22_14_3_1230 "ATC:APP_MN_SIMAT_RECEIVE_DATA_IND"
#define ATC_GSM_STK_6714_112_2_17_22_14_3_1231 "ATC:APP_SIMAT_OPEN_CHANNEL_IND"
#define ATC_GSM_STK_6728_112_2_17_22_14_3_1232 "ATC:APP_SIMAT_CLOSE_CHANNEL_IND"
#define ATC_GSM_STK_6739_112_2_17_22_14_3_1233 "ATC:APP_SIMAT_GET_CHANNEL_STATUS_IND"
#define ATC_GSM_STK_6765_112_2_17_22_14_3_1234 "ATC_ProcessTimerEXp s_stk_signal.cur_status %d "
#define ATC_GSM_STK_6779_112_2_17_22_14_3_1235 "ATC_ProcessTimerEXp SCI_SET_TIMER 1s"
#define ATC_GSM_STK_6842_112_2_17_22_14_3_1236 "ATC_STK_HandleSocketGetData_Timer times = %d"
#define ATC_GSM_STK_6877_112_2_17_22_14_3_1237 " ATCSTK_GetSocketData = %ld, channel id= %d"
#define ATC_GSM_STK_6922_112_2_17_22_14_3_1238 "ATCSTK_GetSocketData Create timer"
#define ATC_GSM_STK_6940_112_2_17_22_14_4_1239 "ata_dest_address.addr_len = %d, type = %d, dest_address = %s"
#define ATC_GSM_STK_6946_112_2_17_22_14_4_1240 "port = %d, trans_level_type = %d"
#define ATC_GSM_STK_6975_112_2_17_22_14_4_1241 "ATC_STK_HandleConnectSocket_Timer ConnectSocket success!"
#define ATC_GSM_STK_6984_112_2_17_22_14_4_1242 "ATC_STK_HandleConnectSocket_Timer ConnectSocket error!"
#define ATC_GSM_STK_7011_112_2_17_22_14_4_1243 "ATCSTK_ConnectSocket  is established, sock=%ld"
#define ATC_GSM_STK_7016_112_2_17_22_14_4_1244 "ATCSTK_ConnectSocket sci_sock_connect, err = %d"
#define ATC_GSM_STK_7033_112_2_17_22_14_4_1245 "ATCSTK_ConnectSocket, sock=%d, return=%d"
#define ATC_GSM_STK_7067_112_2_17_22_14_4_1246 "ATC_Status_OpenChannel sci_sock_socket error = %d"
#define ATC_GSM_STK_7078_112_2_17_22_14_4_1247 "ATC_Status_OpenChannel sci_sock_setsockopt, result=%d"
#define ATC_GSM_STK_7127_112_2_17_22_14_4_1248 "tp_pdp_writedata error= %d"
#define ATC_GSM_STK_7131_112_2_17_22_14_4_1249 "tp_pdp_writedata, s=%d, return %d"
#define ATC_GSM_STK_OMS_436_112_2_17_22_14_5_1250 "ATC:ATC_OmsInitCnfSetMenu, dual_sys:%d"
#define ATC_GSM_STK_OMS_701_112_2_17_22_14_5_1251 "ATC: ATC_ProcessSendSMS SC is exist."
#define ATC_GSM_STK_OMS_712_112_2_17_22_14_5_1252 "ATC: ATC_ProcessSendSMS sc_addr.length = %d"
#define ATC_GSM_STK_OMS_719_112_2_17_22_14_5_1253 "ATC: ATC_ProcessSendSMS sc_addr.length = %d"
#define ATC_GSM_STK_OMS_724_112_2_17_22_14_5_1254 "ATC: address is not exist."
#define ATC_GSM_STK_OMS_732_112_2_17_22_14_5_1255 "ATC: send sms pdu len error !."
#define ATC_GSM_STK_OMS_750_112_2_17_22_14_6_1256 "ATC: SMS is packed."
#define ATC_GSM_STK_OMS_754_112_2_17_22_14_6_1257 "ATC: send sms pdu err code = %d."
#define ATC_GSM_STK_OMS_766_112_2_17_22_14_6_1258 "ATC: SMS is unpacked."
#define ATC_GSM_STK_OMS_771_112_2_17_22_14_6_1259 "ATC: send sms pdu err code = %d."
#define ATC_GSM_STK_OMS_958_112_2_17_22_14_6_1260 "ATC: user originate ussd fail, err code = %d."
#define ATC_GSM_STK_OMS_1016_112_2_17_22_14_6_1261 "ATC:EV_MN_APP_SIMAT_SETUP_IDLE_TEXT_IND_F"
#define ATC_GSM_STK_OMS_1100_112_2_17_22_14_6_1262 "ATC:ATC_OmsSTKProcessSendSmsCnf,MN_SMS_OPERATE_SUCCESS"
#define ATC_GSM_STK_OMS_1109_112_2_17_22_14_6_1263 "ATC:ATC_OmsSTKProcessSendSmsCnf,MN_SMS_FAIL"
#define ATC_GSM_STK_OMS_1140_112_2_17_22_14_6_1264 "ATC:ATC_STKProcessSendUssdCnf,MN_SMS_OPERATE_SUCCESS"
#define ATC_GSM_STK_OMS_1150_112_2_17_22_14_6_1265 "ATC:ATC_STKProcessSendUssdCnf,MN_USSD_FAIL"
#define ATC_GSM_STK_OMS_1177_112_2_17_22_14_7_1266 "ATC:ATC_OmsSTKProcessSendSsCnf,MN_SMS_OPERATE_SUCCESS"
#define ATC_GSM_STK_OMS_1188_112_2_17_22_14_7_1267 "ATC:ATC_OmsSTKProcessSendSsCnf,MN_SS_FAIL"
#define ATC_GSM_STK_OMS_1215_112_2_17_22_14_7_1268 "ATC:ATC_STKProcessSendSsRegPwdCnf,MN_SMS_OPERATE_SUCCESS"
#define ATC_GSM_STK_OMS_1224_112_2_17_22_14_7_1269 "ATC:ATC_STKProcessSendSsRegPwdCnf,MN_SS_FAIL"
#define ATC_GSM_STK_OMS_1318_112_2_17_22_14_7_1270 "ATC: re_value=%d"
#define ATC_GSM_STK_OMS_1386_112_2_17_22_14_7_1271 "ATC:IsCallSucc == TRUE"
#define ATC_GSM_STK_OMS_1394_112_2_17_22_14_7_1272 "ATC:IsCallSucc == FALSE"
#define ATC_GSM_STK_OMS_1889_112_2_17_22_14_8_1273 "ATC: ATC_ProcessRefreshInd, refresh_type = %d,is_files_exist=%d"
#define ATC_GSM_STK_OMS_1923_112_2_17_22_14_8_1274 "ATC: ATC_ProcessRefreshCnf, is_ok=%d,is_file_changed=%d"
#define ATC_GSM_STK_OMS_1960_112_2_17_22_14_8_1275 "ATC: ATC_ProcessSendData, channel id:%d"
#define ATC_GSM_STK_OMS_1992_112_2_17_22_14_8_1276 "ATC: ATC_ProcessReceiveData, channel id:%d"
#define ATC_GSM_STK_OMS_2079_112_2_17_22_14_8_1277 "ATC: ATC_ProcessCloseChannel, channel_id = %d"
#define ATC_GSM_STK_OMS_2134_112_2_17_22_14_8_1278 "ATC:: ATC_OmsStringToByte pointer is NULL!"
#define ATC_GSM_STK_OMS_2185_112_2_17_22_14_9_1279 "ATC:: invalid number:%x"
#define ATC_GSM_STK_OMS_2226_112_2_17_22_14_9_1280 "Param1  invalid\n"
#define ATC_GSM_STK_OMS_2310_112_2_17_22_14_9_1281 "ATC:ATC_ProcessSPUSATCALLSETUP atc_config_ptr->call_link_id = %d "
#define ATC_GSM_STK_OMS_2335_112_2_17_22_14_9_1282 "ATC:ATC_ProcessSPUSATCALLSETUP atc_config_ptr->call_link_id = %d "
#define ATC_GSM_STK_OMS_2379_112_2_17_22_14_9_1283 "ATC:Param1  invalid\n"
#define ATC_GSM_STK_OMS_2382_112_2_17_22_14_9_1284 "ATC:+SPUSATPROFILE: Param1 length=%d\n"
#define ATC_GSM_STK_OMS_2386_112_2_17_22_14_9_1285 "ATC:+SPUSATPROFILE: Param1 is not string or string length is too long\n"
#define ATC_GSM_STK_OMS_2392_112_2_17_22_14_9_1286 "ATC:+SPUSATPROFILE,s_ap_is_active is already TRUE."
#define ATC_GSM_STK_OMS_2403_112_2_17_22_14_9_1287 "ATC:+SPUSATPROFILE, String to Byte  error\n"
#define ATC_GSM_STK_OMS_2420_112_2_17_22_14_9_1288 "ATC:+SPUSATPROFILE,Profile download :s_stk_signal.is_support_stk:%d"
#define ATC_GSM_STK_OMS_2445_112_2_17_22_14_9_1289 "ATC: SPUSATCAPREQ command is read command"
#define ATC_GSM_STK_OMS_2489_112_2_17_22_14_9_1290 "ATC:+SPUSATENVECMD,s_ap_is_active:%d==FALSE or s_stk_is_active:%d==FALSE or s_stk_is_pending==TRUE"
#define ATC_GSM_STK_OMS_2514_112_2_17_22_14_9_1291 "ATC: SPUSATENVECMD set command "
#define ATC_GSM_STK_OMS_2522_112_2_17_22_14_9_1292 "SPUSATENVECMD event_tag =%x "
#define ATC_GSM_STK_OMS_2530_112_2_17_22_14_9_1293 "ATC  SPUSATENVECMD Menu Selection\n"
#define ATC_GSM_STK_OMS_2538_112_2_17_22_14_9_1294 "SPUSATENVECMD set cmd:stk current status is not setupmenu"
#define ATC_GSM_STK_OMS_2545_112_2_17_22_14_9_1295 "SPUSATENVECMD set cmd: tag is invalid"
#define ATC_GSM_STK_OMS_2599_112_2_17_22_14_9_1296 "ATC:+SPUSATTERMINAL,s_ap_is_active:%d==FALSE or s_stk_is_active:%d==FALSE"
#define ATC_GSM_STK_OMS_2635_112_2_17_22_14_9_1297 "ATC: terminal set command "
#define ATC_GSM_STK_OMS_2790_112_2_17_22_14_10_1298 "ATC:ATC_HandleProvidLocalInfo,info_type = %d"
#define ATC_GSM_STK_OMS_2818_112_2_17_22_14_10_1299 "ATC: get imei failed."
#define ATC_GSM_STK_OMS_2882_112_2_17_22_14_10_1300 "ATC:ATC_OmsProcessSTKSignal,s_ap_is_active:%d,s_stk_is_pending:%d"
#define ATC_GSM_STK_OMS_2932_112_2_17_22_14_10_1301 "ATC:SIMAT_PROVIDE_LOCAL_INFO"
#define ATC_GSM_STK_OMS_2937_112_2_17_22_14_10_1302 "ATC:STK_SESSION_TERMINATE_REQ"
#define ATC_GSM_STK_OMS_2962_112_2_17_22_14_10_1303 "ATC:APP_MN_SIMAT_LANG_NOTIFY_IND"
#define ATC_GSM_STK_OMS_2993_112_2_17_22_14_10_1304 "ATC:SIMAT_EVENT_CC_RSP"
#define ATC_GSM_STK_OMS_2997_112_2_17_22_14_10_1305 "ATC:SIMAT_EVENT_SS_CON_RSP"
#define ATC_GSM_STK_OMS_3001_112_2_17_22_14_10_1306 "ATC:SIMAT_EVENT_USSD_CON_RSP"
#define ATC_GSM_STK_OMS_3005_112_2_17_22_14_10_1307 "ATC:SIMAT_EVENT_SMS_CON_RSP"
#define ATC_GSM_STK_OMS_3009_112_2_17_22_14_10_1308 "ATC:SIMAT_EVENT_REFRESH"
#define ATC_GSM_STK_OMS_3013_112_2_17_22_14_10_1309 "ATC:SIMAT_EVENT_SMS_PP_RSP"
#define ATC_GSM_STK_OMS_3037_112_2_17_22_14_10_1310 "ATC_OmsSignalCacheFreeSize:size=%d\n"
#define ATC_GSM_STK_OMS_3051_112_2_17_22_14_10_1311 "ATC_OmsSignalCacheSignalNum:size=%d\n"
#define ATC_GSM_STK_OMS_3114_112_2_17_22_14_10_1312 "ATC:Status error"
#define ATC_GSM_STK_OMS_3119_112_2_17_22_14_10_1313 "ATC :Sig_code=0x%04x"
#define ATC_GSM_STK_OMS_3224_112_2_17_22_14_11_1314 "ATC:SIMAT_PROVIDE_LOCAL_INFO"
#define ATC_GSM_STK_OMS_3264_112_2_17_22_14_11_1315 "ATC:EV_MN_APP_SIMAT_LANG_NOTIFY_IND_F"
#define ATC_GSM_STK_OMS_3360_112_2_17_22_14_11_1316 "ATC:Status error or NO Pending Event"
#define ATC_GSM_STK_OMS_3366_112_2_17_22_14_11_1317 "ATC:No pending Event\n"
#define ATC_GSM_STK_OMS_3374_112_2_17_22_14_11_1318 "ATC:Sig_Ptr error"
#define ATC_GSM_STK_OMS_3404_112_2_17_22_14_11_1319 "ATC: signal code:0x%04x, do nothing."
#define ATC_GSM_STK_OMS_3631_112_2_17_22_14_11_1320 "bin.li add sim_terminal_profile!!!!!!"
#define ATC_GSM_STK_OMS_3641_112_2_17_22_14_11_1321 "ATC:ATC_CheckAndHandleConfig,re_value=%d"
#define ATC_GSM_STK_OMS_3660_112_2_17_22_14_11_1322 "ATC:ATC_ProcessSPSTPARAM, param1 and param2 are not both present."
#define ATC_GSM_STK_OMS_3665_112_2_17_22_14_11_1323 "ATC:ATC_ProcessSPSTPARAM, param1=%d,param2=%d"
#define ATC_GSM_STK_OMS_3688_112_2_17_22_14_12_1324 "ATC:ATC_ProcessSPSTPARAM, param1 is not support."
#define ATC_GSM_STK_OMS_3718_112_2_17_22_14_12_1325 "ATC: ATC_ProcessRefreshCmd, refresh_type = %d "
#define ATC_GSM_STK_OMS_3727_112_2_17_22_14_12_1326 "ATC: ATC_ProcessRefreshCmd, refresh cmd execute failure!"
#define ATC_GSM_STK_OMS_3737_112_2_17_22_14_12_1327 "ATC: ATC_ProcessRefreshCmd, refresh cmd execute failure!"
#define ATC_GSM_STK_OMS_3742_112_2_17_22_14_12_1328 "ATC: ATC_ProcessRefreshCmd, current refresh type is not support yet!"
#define ATC_GSM_STK_OMS_3794_112_2_17_22_14_12_1329 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_ACTIVE_PS, param2=%d,param3=%d, one of them is invalid."
#define ATC_GSM_STK_OMS_3800_112_2_17_22_14_12_1330 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_ACTIVE_PS, param2 and param3 are not both valid."
#define ATC_GSM_STK_OMS_3834_112_2_17_22_14_12_1331 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_SET_EXT_PROFILE, param2,param3 are not both valid."
#define ATC_GSM_STK_OMS_3854_112_2_17_22_14_12_1332 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_STARTUP_PS, param2 are not valid."
#define ATC_GSM_STK_OMS_3860_112_2_17_22_14_12_1333 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_STARTUP_PS, param2 are not inputed."
#define ATC_GSM_STK_OMS_3893_112_2_17_22_14_12_1334 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_STARTUP_PS, param2 are not valid."
#define ATC_GSM_STK_OMS_3911_112_2_17_22_14_12_1335 "ATC: PHONEBOOK_Get3GPhonebookStatus fail, code: %d"
#define ATC_GSM_STK_OMS_3968_112_2_17_22_14_12_1336 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_MALLOC_MEMORY, SUCCESS."
#define ATC_GSM_STK_OMS_3972_112_2_17_22_14_12_1337 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_MALLOC_MEMORY, param2 are not valid."
#define ATC_GSM_STK_OMS_3978_112_2_17_22_14_12_1338 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_MALLOC_MEMORY, param2 are not inputed."
#define ATC_GSM_STK_OMS_4005_112_2_17_22_14_12_1339 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_GETPLMNNAME, param3 or param4 are invalid."
#define ATC_GSM_STK_OMS_4017_112_2_17_22_14_12_1340 "ATC_ProcessSPSTACTION,MNPHONE_SelectGSMBandEx,fail."
#define ATC_GSM_STK_OMS_4021_112_2_17_22_14_12_1341 "ATC_ProcessSPSTACTION,MNPHONE_SelectGSMBandEx,success."
#define ATC_GSM_STK_OMS_4026_112_2_17_22_14_12_1342 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_SELGSMBAND, param3 are invalid."
#define ATC_GSM_STK_OMS_4038_112_2_17_22_14_12_1343 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_SETGCFCALIWITHSIM, param3 are invalid."
#define ATC_GSM_STK_OMS_4070_112_2_17_22_14_12_1344 "ATC_ProcessSPSTACTION,MNSIM_GetEFInfo,fail."
#define ATC_GSM_STK_OMS_4074_112_2_17_22_14_12_1345 "ATC_ProcessSPSTACTION,MNSIM_GetEFInfo,success."
#define ATC_GSM_STK_OMS_4079_112_2_17_22_14_12_1346 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_GETEFINFO, param3 or param4 are invalid."
#define ATC_GSM_STK_OMS_4098_112_2_17_22_14_12_1347 "ATC_ProcessSPSTACTION,MNSIM_ReadSimEF,fail."
#define ATC_GSM_STK_OMS_4102_112_2_17_22_14_12_1348 "ATC_ProcessSPSTACTION,MNSIM_ReadSimEF,success."
#define ATC_GSM_STK_OMS_4107_112_2_17_22_14_12_1349 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_READSIMEF, param3 or param4 are invalid."
#define ATC_GSM_STK_OMS_4131_112_2_17_22_14_12_1350 "ATC_ProcessSPSTACTION,MNSIM_WriteSimEF,fail."
#define ATC_GSM_STK_OMS_4135_112_2_17_22_14_12_1351 "ATC_ProcessSPSTACTION,MNSIM_WriteSimEF,success."
#define ATC_GSM_STK_OMS_4140_112_2_17_22_14_12_1352 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_GETSIMFILEINFO, param3 are invalid."
#define ATC_GSM_STK_OMS_4176_112_2_17_22_14_13_1353 "ATC_ProcessSPSTACTION,MNSIM_ReadSimRecordFile,fail."
#define ATC_GSM_STK_OMS_4180_112_2_17_22_14_13_1354 "ATC_ProcessSPSTACTION,MNSIM_ReadSimRecordFile,success."
#define ATC_GSM_STK_OMS_4185_112_2_17_22_14_13_1355 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_READSIMRECORDFILE, params are invalid."
#define ATC_GSM_STK_OMS_4246_112_2_17_22_14_13_1356 "ATC_ProcessSPSTACTION,MNSIM_UpdateSimRecordFile,fail."
#define ATC_GSM_STK_OMS_4250_112_2_17_22_14_13_1357 "ATC_ProcessSPSTACTION,MNSIM_UpdateSimRecordFile,success."
#define ATC_GSM_STK_OMS_4255_112_2_17_22_14_13_1358 "ATC:ATC_ProcessSPSTACTION,MNSIM_UpdateSimRecordFile, param3 are invalid."
#define ATC_GSM_STK_OMS_4296_112_2_17_22_14_13_1359 "ATC_ProcessSPSTACTION,MNSIM_SearchSimRecord,fail."
#define ATC_GSM_STK_OMS_4300_112_2_17_22_14_13_1360 "ATC_ProcessSPSTACTION,MNSIM_SearchSimRecord,success."
#define ATC_GSM_STK_OMS_4305_112_2_17_22_14_13_1361 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_SEARCHSIMRECORD, params are invalid."
#define ATC_GSM_STK_OMS_4321_112_2_17_22_14_13_1362 "ATC_ProcessSPSTACTION,MNSIM_SetSimEFService,fail."
#define ATC_GSM_STK_OMS_4325_112_2_17_22_14_13_1363 "ATC_ProcessSPSTACTION,MNSIM_SetSimEFService,success."
#define ATC_GSM_STK_OMS_4330_112_2_17_22_14_13_1364 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_SETSIMEFSERVICE, param3 or param4 are invalid."
#define ATC_GSM_STK_OMS_4348_112_2_17_22_14_13_1365 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_GETSIMEFSERVICE, param3 are invalid."
#define ATC_GSM_STK_OMS_4366_112_2_17_22_14_13_1366 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_GETSIMFILEINFO, param3 are invalid."
#define ATC_GSM_STK_OMS_4393_112_2_17_22_14_13_1367 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_GETSIMATRRAWDATA, param3 are invalid."
#define ATC_GSM_STK_OMS_4409_112_2_17_22_14_13_1368 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_SETSAVESMS, param3 is invalid."
#define ATC_GSM_STK_OMS_4467_112_2_17_22_14_13_1369 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_INDMMIMOCC, param3 are invalid."
#define ATC_GSM_STK_OMS_4505_112_2_17_22_14_13_1370 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_STARTCALLNOCC, param3 are invalid."
#define ATC_GSM_STK_OMS_4576_112_2_17_22_14_13_1371 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_INDMMISMSC, param3 are invalid."
#define ATC_GSM_STK_OMS_4636_112_2_17_22_14_13_1372 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_APPSENDSMSNOCC, param3 are invalid."
#define ATC_GSM_STK_OMS_4647_112_2_17_22_14_13_1373 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_SAMSUNG_INTERFACE_TEST, param2 are not valid."
#define ATC_GSM_STK_OMS_4684_112_2_17_22_14_14_1374 "ueit nvm: read nvm_read_of_gsm suc"
#define ATC_GSM_STK_OMS_4697_112_2_17_22_14_14_1375 "ueit nvm: read nvm_read_of_gsm fail."
#define ATC_GSM_STK_OMS_4705_112_2_17_22_14_14_1376 "ueit nvm: read nvm_read_of_gsm begin"
#define ATC_GSM_STK_OMS_4733_112_2_17_22_14_14_1377 "ueit nvm: invalid parameter"
#define ATC_GSM_STK_OMS_4739_112_2_17_22_14_14_1378 "ATC:ATC_ProcessSPSTACTION,ST_ACTION_UEIT_NV, one of them is invalid."
#define ATC_GSM_STK_OMS_4794_112_2_17_22_14_14_1379 "ATC: AT+SPUEITNVM input nv item data length error.length:%d != %d"
#define ATC_GSM_STK_OMS_4807_112_2_17_22_14_14_1380 "ATC: cancel the SPUEITNVM data."
#define ATC_GSM_STK_OMS_4811_112_2_17_22_14_14_1381 "ATC: snvm data format is error."
#define ATC_MALLOC_254_112_2_17_22_14_36_1382 "Add heap success, heap size = %d.\n"
#define ATC_PHONEBOOK_182_112_2_17_22_14_44_1383 "ATC: CPBS, param1 is not string!"
#define ATC_PHONEBOOK_324_112_2_17_22_14_44_1384 "ATC: SIM/USIM is not ready."
#define ATC_PHONEBOOK_334_112_2_17_22_14_44_1385 "ATC: +CPBR, no phonebook entity."
#define ATC_PHONEBOOK_351_112_2_17_22_14_44_1386 "ATC: CPBR: index:%d is out of range(1-%d)"
#define ATC_PHONEBOOK_365_112_2_17_22_14_44_1387 "ATC: no phonebook record."
#define ATC_PHONEBOOK_490_112_2_17_22_14_44_1388 "ATC: ATC_ProcessCPBF, param1 is not string!"
#define ATC_PHONEBOOK_513_112_2_17_22_14_44_1389 "ATC:ConvertAscUcs2IntoBin IS TRUE"
#define ATC_PHONEBOOK_518_112_2_17_22_14_44_1390 "ATC:ConvertAscUcs2IntoBin is FALSE"
#define ATC_PHONEBOOK_559_112_2_17_22_14_44_1391 "ATC:ConvertAscUcs2IntoBin"
#define ATC_PHONEBOOK_609_112_2_17_22_14_45_1392 "ATC: SIM/USIM is not ready."
#define ATC_PHONEBOOK_618_112_2_17_22_14_45_1393 "ATC: +CPBW, no phonebook entity."
#define ATC_PHONEBOOK_632_112_2_17_22_14_45_1394 "ATC: CPBW: index:%d is out of range(1-%d)"
#define ATC_PHONEBOOK_649_112_2_17_22_14_45_1395 "ATC: CPBW: no index"
#define ATC_PHONEBOOK_663_112_2_17_22_14_45_1396 "ATC:CPBW: memory full."
#define ATC_PHONEBOOK_673_112_2_17_22_14_45_1397 "ATC: CPBW, <number> is not string!"
#define ATC_PHONEBOOK_728_112_2_17_22_14_45_1398 "ATC: CPBW, param4 is not string!"
#define ATC_PHONEBOOK_744_112_2_17_22_14_45_1399 "ATC:cpbw,tel_alpha=,%x,%x,%x,str_len,%x"
#define ATC_PHONEBOOK_750_112_2_17_22_14_45_1400 "ATC:CPBW,ATC_UCS2_CODE == *entry.alpha"
#define ATC_PHONEBOOK_782_112_2_17_22_14_45_1401 "ATC:cpbw,PARA4.str_ptr=%x,%x,%x"
#define ATC_PHONEBOOK_784_112_2_17_22_14_45_1402 "ATC:cpbw,entry.alpha=%x,%x,%x"
#define ATC_PHONEBOOK_834_112_2_17_22_14_45_1403 "ATC:ATC_ProcessDelItemCnf, delete entry(id=%d) by ATC."
#define ATC_PHONEBOOK_850_112_2_17_22_14_45_1404 "ATC:ATC_ProcessDelItemCnf, delete entry(id=%d) by MMI."
#define ATC_PHONEBOOK_890_112_2_17_22_14_45_1405 "ATC: CPBP, param1 is not string!"
#define ATC_PHONEBOOK_1196_112_2_17_22_14_46_1406 "ATC: ATC_CMD_TYPE_TEST"
#define ATC_PHONEBOOK_1276_112_2_17_22_14_46_1407 "ATC:PHONEBOOK_ReadVoiceMailAddr IS OK"
#define ATC_PHONEBOOK_1288_112_2_17_22_14_46_1408 "ATC: PHONEBOOK_ReadVoiceMailAddr IS ERROR!"
#define ATC_PHONEBOOK_1301_112_2_17_22_14_46_1409 "ATC:PHONEBOOK_EnableVoiceMailAddr failed"
#define ATC_PHONEBOOK_1317_112_2_17_22_14_46_1410 "ATC: CSVM, param2 is not string!"
#define ATC_PHONEBOOK_1326_112_2_17_22_14_46_1411 "ATC:AT+CSVM,error in length of input"
#define ATC_PHONEBOOK_1345_112_2_17_22_14_46_1412 "ATC:ERR_OPERATION_NOT_SUPPORTED"
#define ATC_PHONEBOOK_1360_112_2_17_22_14_46_1413 "ATC:PHONEBOOK_UpdateVoiceMailAddr failed"
#define ATC_PHONEBOOK_1367_112_2_17_22_14_46_1414 "ATC:PHONEBOOK_EnableVoiceMailAddr failed"
#define ATC_PHONEBOOK_1412_112_2_17_22_14_46_1415 "ATC: ATC_CMD_TYPE_READ"
#define ATC_PHONEBOOK_1419_112_2_17_22_14_46_1416 "ATC: ERR_INVALID_INDEX"
#define ATC_PHONEBOOK_1432_112_2_17_22_14_46_1417 "ATC: para_ptr=atc_storage_str"
#define ATC_PHONEBOOK_1498_112_2_17_22_14_47_1418 "ATC: : phonebook used records number:%d"
#define ATC_PHONEBOOK_1530_112_2_17_22_14_47_1419 "ATC_AddNationChToDIAL:%d"
#define ATC_PHONEBOOK_1572_112_2_17_22_14_47_1420 "ATC:HandlePhonebookRead,entry_id:%d,is_usim:%d"
#define ATC_PHONEBOOK_1924_112_2_17_22_14_47_1421 "ATC: Read phonebook additional info failed.entry_id=%d,cause:%d"
#define ATC_PHONEBOOK_1970_112_2_17_22_14_47_1422 "ATC:PHONEBOOK_Delete3GPhonebook failed,cause:%u"
#define ATC_PHONEBOOK_1995_112_2_17_22_14_47_1423 "ATC:PHONEBOOK_DeletePhonebookEntry failed,cause:%u"
#define ATC_PHONEBOOK_2050_112_2_17_22_14_48_1424 "ATC:PHONEBOOK_Update3GPhonebook failed,cause:%d"
#define ATC_PHONEBOOK_2081_112_2_17_22_14_48_1425 "ATC:PHONEBOOK_UpdatePhonebookEntry failed,cause:%lu"
#define ATC_PHONEBOOK_2243_112_2_17_22_14_48_1426 "ATC: ATC_ProcessUSIMReadAddCnf: read entry(id=%d) by ATC"
#define ATC_PHONEBOOK_2285_112_2_17_22_14_48_1427 "ATC: read ADN entry(id:%d) failed"
#define ATC_PHONEBOOK_2344_112_2_17_22_14_48_1428 "ATC: ATC_ProcessUSIMReadAddCnf: read entry(id=%d) by MMI"
#define ATC_PHONEBOOK_2368_112_2_17_22_14_48_1429 "ATC: ATC_ProcessDelUserInfoCnf: delete entry(id=%d) by ATC."
#define ATC_PHONEBOOK_2381_112_2_17_22_14_48_1430 "ATC: ATC_ProcessDelUserInfoCnf: delete entry(id=%d) by MMI."
#define ATC_PHONEBOOK_2404_112_2_17_22_14_48_1431 "ATC: ATC_ProcessUpdateUserInfoCnf: update entry(id=%d) by ATC."
#define ATC_PHONEBOOK_2434_112_2_17_22_14_48_1432 "ATC: ATC_ProcessUpdateUserInfoCnf: update entry(id=%d) by MMI."
#define ATC_PHONEBOOK_2479_112_2_17_22_14_49_1433 "ATC: SIM/USIM is not ready."
#define ATC_PHONEBOOK_2517_112_2_17_22_14_49_1434 "ATC: ^SCPBW, no phonebook entity."
#define ATC_PHONEBOOK_2536_112_2_17_22_14_49_1435 "AT^SCPBW: MMI is initializing phonebook(current id:%d,total:%d)."
#define ATC_PHONEBOOK_2548_112_2_17_22_14_49_1436 "ATC: SCPBW, <index> is not numeric!"
#define ATC_PHONEBOOK_2555_112_2_17_22_14_49_1437 "ATC: SCPBW: index:%d is out of range(1-%d)"
#define ATC_PHONEBOOK_2572_112_2_17_22_14_49_1438 "ATC: SCPBW: <index1> use default value"
#define ATC_PHONEBOOK_2583_112_2_17_22_14_49_1439 "ATC: SCPBW: no index 1 and 2"
#define ATC_PHONEBOOK_2590_112_2_17_22_14_49_1440 "ATC: SCPBW: memory full."
#define ATC_PHONEBOOK_2601_112_2_17_22_14_49_1441 "ATC: SCPBW, <num1> is not string!"
#define ATC_PHONEBOOK_2613_112_2_17_22_14_49_1442 "ATC: SCPBW: index2 length is zero"
#define ATC_PHONEBOOK_2669_112_2_17_22_14_49_1443 "ATC: SCPBW, <num2> is not string!"
#define ATC_PHONEBOOK_2677_112_2_17_22_14_49_1444 "ATC: SCPBW: index4 length is zero"
#define ATC_PHONEBOOK_2733_112_2_17_22_14_49_1445 "ATC: SCPBW, <num3> is not string!"
#define ATC_PHONEBOOK_2741_112_2_17_22_14_49_1446 "ATC: SCPBW: index6 length is zero"
#define ATC_PHONEBOOK_2796_112_2_17_22_14_49_1447 "ATC: SCPBW, <num4> is not string!"
#define ATC_PHONEBOOK_2804_112_2_17_22_14_49_1448 "ATC: SCPBW: index8 length is zero"
#define ATC_PHONEBOOK_2860_112_2_17_22_14_49_1449 "ATC: SCPBW, <email> is not string!"
#define ATC_PHONEBOOK_2892_112_2_17_22_14_49_1450 "ATC:CPBW,Email : ATC_UCS2_CODE"
#define ATC_PHONEBOOK_2929_112_2_17_22_14_49_1451 "ATC:^scpbw,PARA12.str_ptr=%x,%x,%x"
#define ATC_PHONEBOOK_2931_112_2_17_22_14_49_1452 "ATC:additional_entry.email_addr.address=%x,%x,%x"
#define ATC_PHONEBOOK_2942_112_2_17_22_14_49_1453 "ATC: SCPBW, <SNE> is not string!"
#define ATC_PHONEBOOK_2981_112_2_17_22_14_50_1454 "ATC:CPBW,Email : ATC_UCS2_CODE"
#define ATC_PHONEBOOK_3027_112_2_17_22_14_50_1455 "ATC:^scpbw,PARA13.str_ptr=%x,%x,%x"
#define ATC_PHONEBOOK_3032_112_2_17_22_14_50_1456 "ATC:additional_entry.sne_alpha.alpha=%x,%x,%x"
#define ATC_PHONEBOOK_3047_112_2_17_22_14_50_1457 "ATC: SCPBW, <text> is not string!"
#define ATC_PHONEBOOK_3060_112_2_17_22_14_50_1458 "ATC: SCPBW: <text> length is not in range(1-%d)"
#define ATC_PHONEBOOK_3075_112_2_17_22_14_50_1459 "ATC:cpbw,tel_alpha=,%x,%x,%x,str_len,%x"
#define ATC_PHONEBOOK_3088_112_2_17_22_14_50_1460 "ATC:CPBW,ATC_UCS2_CODE == *entry.alpha"
#define ATC_PHONEBOOK_3105_112_2_17_22_14_50_1461 "ATC: SCPBW: index length is TOO long"
#define ATC_PHONEBOOK_3121_112_2_17_22_14_50_1462 "ATC:^scpbw,PARA10.str_ptr=%x,%x,%x"
#define ATC_PHONEBOOK_3123_112_2_17_22_14_50_1463 "ATC:cpbw,entry.alpha=%x,%x,%x"
#define ATC_PHONEBOOK_3200_112_2_17_22_14_50_1464 "ATC: SIM/USIM is not ready."
#define ATC_PHONEBOOK_3235_112_2_17_22_14_50_1465 "ATC: ^SCPBR, no phonebook entity."
#define ATC_PHONEBOOK_3252_112_2_17_22_14_50_1466 "ATC: SCPBR: index:%d is out of range(1-%d)"
#define ATC_PHONEBOOK_3265_112_2_17_22_14_50_1467 "ATC: no phonebook record."
#define ATC_PHONEBOOK_3282_112_2_17_22_14_50_1468 "AT^SCPBR: MMI is initializing phonebook(current id:%d,total:%d)."
#define ATC_PHONEBOOK_3426_112_2_17_22_14_50_1469 "ATC: +SPPBACTION, pb action ST_ACTION_READ_PB_SEC_TEXT is triggered."
#define ATC_PHONEBOOK_3431_112_2_17_22_14_50_1470 "ATC: USIM is not ready.call_read_flag[g_current_card_id]=%d,sim_type[g_current_card_id]=%d"
#define ATC_PHONEBOOK_3440_112_2_17_22_14_50_1471 "ATC: +SPPBACTION, no phonebook entity."
#define ATC_PHONEBOOK_3472_112_2_17_22_14_51_1472 "ATC: +SPPBACTION, not USIM card inserted."
#define ATC_PHONEBOOK_3482_112_2_17_22_14_51_1473 "ATC: SPPBACTION: do nothing."
#define ATC_PHONEBOOK_3506_112_2_17_22_14_51_1474 "ATC: SIM/USIM is not ready."
#define ATC_PHONEBOOK_3514_112_2_17_22_14_51_1475 "[AT*SP3GPBR]: pbr_num_tel=%d, pbr_num_adf=%d"
#define ATC_PLUS_GPRS_101_112_2_17_22_14_51_1476 "ATC: ATC_ProcessCGDCONT: only support IP (not %s)."
#define ATC_PLUS_GPRS_170_112_2_17_22_14_51_1477 "ATC: ATC_ProcessCGDCONT, param4 is not string!"
#define ATC_PLUS_GPRS_180_112_2_17_22_14_51_1478 "%s"
#define ATC_PLUS_GPRS_268_112_2_17_22_14_51_1479 "ATC : CONTEXT NUM %d"
#define ATC_PLUS_GPRS_280_112_2_17_22_14_51_1480 "ATC: ReadPDPCON pdp_con_num: %d"
#define ATC_PLUS_GPRS_310_112_2_17_22_14_51_1481 "ATC: ReadPDPCON pdp_con_arr[i].pdp_addr_str.length: %d"
#define ATC_PLUS_GPRS_387_112_2_17_22_14_52_1482 "ATC: ATC_ProcessCGPCO, param2 or param3 is not string!"
#define ATC_PLUS_GPRS_404_112_2_17_22_14_52_1483 "ATC: pco user or passwd too long."
#define ATC_PLUS_GPRS_418_112_2_17_22_14_52_1484 "ATC: pco user or passwd too long."
#define ATC_PLUS_GPRS_428_112_2_17_22_14_52_1485 "ATC: set pco error, result = %d"
#define ATC_PLUS_GPRS_434_112_2_17_22_14_52_1486 "ATC: set pco error parameter"
#define ATC_PLUS_GPRS_598_112_2_17_22_14_52_1487 "ATC: ATC_ProcessCGQREQ param error!"
#define ATC_PLUS_GPRS_607_112_2_17_22_14_52_1488 "ATC: MNGPRS_SetTotalQosProfile failure."
#define ATC_PLUS_GPRS_794_112_2_17_22_14_52_1489 "ATC: ATC_ProcessCGQREQ param error!"
#define ATC_PLUS_GPRS_802_112_2_17_22_14_52_1490 "ATC: MNGPRS_SetTotalMinQosProfile failure."
#define ATC_PLUS_GPRS_954_112_2_17_22_14_53_1491 "ATC: MNPHONE_GprsDetach failure."
#define ATC_PLUS_GPRS_1078_112_2_17_22_14_53_1492 "ATC: ATC_ProcessCGACT, the cid:%d is not allowed."
#define ATC_PLUS_GPRS_1091_112_2_17_22_14_53_1493 "ATC: ATC_ProcessCGACT, this pdp id is being deactivated."
#define ATC_PLUS_GPRS_1335_112_2_17_22_14_53_1494 "ATC: CGPADDR"
#define ATC_PLUS_GPRS_1477_112_2_17_22_14_54_1495 "ATC: layer 2 Protocol default?"
#define ATC_PLUS_GPRS_1502_112_2_17_22_14_54_1496 "ATC: CGDATA, not support to activate all pdps."
#define ATC_PLUS_GPRS_1511_112_2_17_22_14_54_1497 "ATC: CGDATA, the cid:%d is not allowed."
#define ATC_PLUS_GPRS_1526_112_2_17_22_14_54_1498 "ATC: CGDATA, this pdp id is being deactivated."
#define ATC_PLUS_GPRS_1687_112_2_17_22_14_54_1499 "ATC: CGANS, param2 is not string!"
#define ATC_PLUS_GPRS_1693_112_2_17_22_14_54_1500 "ATC: layer 2 Protocol:%s"
#define ATC_PLUS_GPRS_1707_112_2_17_22_14_54_1501 "ATC: default Layer 2 Protocol"
#define ATC_PLUS_GPRS_1714_112_2_17_22_14_54_1502 "ATC: para 3 pdp id:%d"
#define ATC_PLUS_GPRS_1730_112_2_17_22_14_54_1503 "ATC: CGANS, MNGPRS_SetResType, failure"
#define ATC_PLUS_GPRS_1734_112_2_17_22_14_54_1504 "ATC: CGANS, is_all:%d,pdp_id:%d"
#define ATC_PLUS_GPRS_1829_112_2_17_22_14_54_1505 "input class name class%s"
#define ATC_PLUS_GPRS_1884_112_2_17_22_14_55_1506 "ATC: CGSMS"
#define ATC_PLUS_GPRS_1897_112_2_17_22_14_55_1507 "CGSMS: %d"
#define ATC_PLUS_GPRS_1950_112_2_17_22_14_55_1508 "ATC: CGREG original value:%d"
#define ATC_PLUS_GPRS_1962_112_2_17_22_14_55_1509 "ATC: CGREG set value:%d"
#define ATC_PLUS_GPRS_1981_112_2_17_22_14_55_1510 "ATC: ATC_ProcessCGREG, plmn_status:%d,rat:%d,cell_cap:%d%d%d%d"
#define ATC_PLUS_GPRS_2047_112_2_17_22_14_55_1511 "ATC: CGEREP: mode=%d,bfr=%d"
#define ATC_PLUS_GPRS_2202_112_2_17_22_14_55_1512 "ATC: MNPHONE_GprsDetach failure."
#define ATC_PLUS_GPRS_2345_112_2_17_22_14_55_1513 "ATC: SGPRSDATA, ERROR: MNGPRS_ReadPdpContextStateEx failed."
#define ATC_PLUS_GPRS_2366_112_2_17_22_14_56_1514 "ATC: SGPRSDATA, ERROR: invalid active pdp count=%d."
#define ATC_PLUS_GPRS_2381_112_2_17_22_14_56_1515 "ATC: SGPRSDATA, too long length!"
#define ATC_PLUS_GPRS_2389_112_2_17_22_14_56_1516 "ATC: SGPRSDATA, The GPRS send data len beyond MAX_AT_GPRSDATA_SEND_LEN!\n"
#define ATC_PLUS_GPRS_2426_112_2_17_22_14_56_1517 "ATC: SGPRSDATA, MNGPRS_SendDataReqEx failed!"
#define ATC_PLUS_GPRS_2459_112_2_17_22_14_56_1518 "ATC: SGPRSDATA, MNGPRS_SendDataReqEx failed!"
#define ATC_PLUS_GPRS_2619_112_2_17_22_14_56_1519 "ATC:CGEQREQ, param9 is not string!"
#define ATC_PLUS_GPRS_2633_112_2_17_22_14_56_1520 "ATC: CGEQREQ, param10 is not string!"
#define ATC_PLUS_GPRS_2677_112_2_17_22_14_56_1521 "ATC: CGEQREQ param error!"
#define ATC_PLUS_GPRS_2699_112_2_17_22_14_56_1522 "ATC: CGEQREQ L4 param is %ld,%d,%d,%d,%d,%d,%d,%d,"
#define ATC_PLUS_GPRS_2702_112_2_17_22_14_56_1523 "ATC: CGEQREQ item_setted :%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"
#define ATC_PLUS_GPRS_2888_112_2_17_22_14_57_1524 "ATC: CGEQMIN, param9 is not string!"
#define ATC_PLUS_GPRS_2901_112_2_17_22_14_57_1525 "ATC: CGEQMIN, param10 is not string!"
#define ATC_PLUS_GPRS_2941_112_2_17_22_14_57_1526 "ATC: CGEQMIN param error!"
#define ATC_PLUS_GPRS_3170_112_2_17_22_14_57_1527 "ATC: MNGPRS_ReadExtNegQos No CID."
#define ATC_PLUS_GPRS_3177_112_2_17_22_14_57_1528 "ATC: MNGPRS_ReadExtNegQos failed."
#define ATC_PLUS_GPRS_3217_112_2_17_22_14_57_1529 "ATC: CGEQNEG, read PDP state failure."
#define ATC_PLUS_GPRS_3249_112_2_17_22_14_57_1530 "ATC:CGEQNEG, no active contexts"
#define ATC_PLUS_GPRS_3310_112_2_17_22_14_57_1531 "ATC: ATC_BuildGprsEvReportRsp, +cgerep mode is 0."
#define ATC_PLUS_GPRS_3330_112_2_17_22_14_57_1532 "ATC: ATC_BuildGprsEvReportRsp, +cgerep mode is 1, cid:%d is data mode."
#define ATC_PLUS_GPRS_3360_112_2_17_22_14_57_1533 "ATC: ATC_BuildGprsEvReportRsp, +cgerep mode is 2, cid:%d is data mode."
#define ATC_PLUS_GPRS_3527_112_2_17_22_14_58_1534 "ATC: Close MODEM service!"
#define ATC_PLUS_GPRS_3681_112_2_17_22_14_58_1535 "ATC: pdp activate is not started by atc."
#define ATC_PLUS_GPRS_3726_112_2_17_22_14_58_1536 "ATC: PDP (cid:%d) change to data mode context"
#define ATC_PLUS_GPRS_3781_112_2_17_22_14_59_1537 "ATC:  invalid cid for mux link\r"
#define ATC_PLUS_GPRS_3804_112_2_17_22_14_59_1538 "ATC:  MPPP_SetPPPInterface() fail"
#define ATC_PLUS_GPRS_3816_112_2_17_22_14_59_1539 "ATC: Open MODEM service!"
#define ATC_PLUS_GPRS_3822_112_2_17_22_14_59_1540 "ATC: APP_MN_ACTIVATE_PDP_CONTEXT_CNF(failure), cid:%d"
#define ATC_PLUS_GPRS_3834_112_2_17_22_14_59_1541 "ATC: ERROR: no found link_id of PSD!"
#define ATC_SLEEP_55_112_2_17_22_15_3_1542 "SCI_ATC_InitSleep: Init !"
#define ATC_SLEEP_83_112_2_17_22_15_3_1543 "SCI_ATC_WakeUpOtherMcu: Wake up the other MCU !"
#define ATC_SLEEP_119_112_2_17_22_15_3_1544 "SCI_ATC_SetSleepSwitchFlag: set it to %d"
#define ATC_SLEEP_141_112_2_17_22_15_3_1545 "SCI_ATC_GetSleepSwitchFlag: get the value is %d"
#define PHONESERVER_STK_ADAPT_450_112_2_17_22_15_7_1546 "SIMAT::ATC_GetPCName PC tag 0x%x is out of range. \n"
#define PHONESERVER_STK_ADAPT_676_112_2_17_22_15_8_1547 "SIMAT_Event_DownLoad_Env:enter"
#define PHONESERVER_STK_ADAPT_699_112_2_17_22_15_8_1548 "SIMAT_Event_DownLoad_Env:SIMAT_BTAG_EVENT_LIST"
#define PHONESERVER_STK_ADAPT_709_112_2_17_22_15_8_1549 "SIMAT_Event_DownLoad_Env:SIMAT_BTAG_DEVICE_ID"
#define PHONESERVER_STK_ADAPT_716_112_2_17_22_15_8_1550 "SIMAT_Event_DownLoad_Env:des/source"
#define PHONESERVER_STK_ADAPT_744_112_2_17_22_15_8_1551 "SIMAT_Event_DownLoad_Env invalid Event %d"
#define PHONESERVER_STK_ADAPT_791_112_2_17_22_15_8_1552 "PhoneSever_STK_Envelop_Handle envelop_tag is %d\n"
#define PHONESERVER_STK_ADAPT_809_112_2_17_22_15_8_1553 "PhoneSever_STK_Envelop_Handle ret is %d\n"
#define PHONESERVER_STK_ADAPT_1710_112_2_17_22_15_10_1554 "11,%d"
#define PHONESERVER_STK_ADAPT_1720_112_2_17_22_15_10_1555 "22,%d"
#define PHONESERVER_STK_ADAPT_2105_112_2_17_22_15_11_1556 "PhoneSever_STK_TerminalRsp_Handle pc name is %d "
#define PHONESERVER_STK_COMMON_FUNC_125_112_2_17_22_15_15_1557 "SIMAT::SIMAT_GetTLVLength point is NULL! \n"
#define PHONESERVER_STK_COMMON_FUNC_171_112_2_17_22_15_15_1558 "SIMAT::SIMAT_GetTLVLength point is NULL! \n"
#define PHONESERVER_STK_DECODE_220_112_2_17_22_15_17_1559 "SIMAT::SIMAT_DecodeDataObject tag error!\n"
#define PHONESERVER_STK_DECODE_297_112_2_17_22_15_17_1560 "SIMAT:the address len longer than the maximum we support, error and cut it!!!\n"
#define PHONESERVER_STK_DECODE_365_112_2_17_22_15_17_1561 "SIMAT::The real alpha id length is 0x%x."
#define PHONESERVER_STK_DECODE_694_112_2_17_22_15_18_1562 "SIMAT::the length of the duration is not 2.\n"
#define PHONESERVER_STK_DECODE_774_112_2_17_22_15_18_1563 "SIMAT:: DecodeItem content_len is zero!!! "
#define PHONESERVER_STK_DECODE_856_112_2_17_22_15_18_1564 "SIMAT::the length of item id is not 1.\n"
#define PHONESERVER_STK_DECODE_917_112_2_17_22_15_18_1565 "SIMAT::the length of the Response length is not 2.\n"
#define PHONESERVER_STK_DECODE_1186_112_2_17_22_15_19_1566 "SIMAT:: DecodeTextString content_len is zero!!! \n"
#define PHONESERVER_STK_DECODE_1269_112_2_17_22_15_19_1567 "SIMAT::the length of the tone is not 1.\n"
#define PHONESERVER_STK_DECODE_1490_112_2_17_22_15_19_1568 "SIMAT::length of LocationInfo is not 7!\n"
#define PHONESERVER_STK_DECODE_1556_112_2_17_22_15_20_1569 "SIMAT::length of IMEI is not 8!\n"
#define PHONESERVER_STK_DECODE_1614_112_2_17_22_15_20_1570 "SIMAT::length of Help request is not 0!\n"
#define PHONESERVER_STK_DECODE_1671_112_2_17_22_15_20_1571 "SIMAT::length of NMR is not 16!\n"
#define PHONESERVER_STK_DECODE_1759_112_2_17_22_15_20_1572 "SIMAT::DecodeDefaultText alloc from share buffer error!\n"
#define PHONESERVER_STK_DECODE_1993_112_2_17_22_15_20_1573 "SIMAT::length of location status is not 1.\n"
#define PHONESERVER_STK_DECODE_2208_112_2_17_22_15_21_1574 "SIMAT::DecodeCCReqAction alloc from share buffer error!\n"
#define PHONESERVER_STK_DECODE_2253_112_2_17_22_15_21_1575 "SIMAT::the length of icon id is not 2.\n"
#define PHONESERVER_STK_DECODE_2469_112_2_17_22_15_21_1576 "SIMAT::the length of timer id is not 1.\n"
#define PHONESERVER_STK_DECODE_2555_112_2_17_22_15_22_1577 "hour %d, min %d, sec %d, timer_value :%d "
#define PHONESERVER_STK_DECODE_2597_112_2_17_22_15_22_1578 "SIMAT::the length of the DTandDZ is not 7.\n"
#define PHONESERVER_STK_DECODE_2758_112_2_17_22_15_22_1579 "SIMAT::the length of the ImmediateResponse is not 0.\n"
#define PHONESERVER_STK_DECODE_2882_112_2_17_22_15_22_1580 "SIMAT::the length of the Language is not 2.\n"
#define PHONESERVER_STK_DECODE_2943_112_2_17_22_15_22_1581 "SIMAT::the length of the Time Advance is not 2.\n"
#define PHONESERVER_STK_DECODE_3017_112_2_17_22_15_22_1582 "SIMAT:: DecodeChannelData content_len is zero!!! "
#define PHONESERVER_STK_DECODE_3080_112_2_17_22_15_23_1583 "SIMAT::the length of channel data len  is not 1.\n"
#define PHONESERVER_STK_DECODE_3139_112_2_17_22_15_23_1584 "SIMAT::the length of browser id  is not 1.\n"
#define PHONESERVER_STK_DECODE_3197_112_2_17_22_15_23_1585 "SIMAT::the length of URL overflow.\n"
#define PHONESERVER_STK_DECODE_3204_112_2_17_22_15_23_1586 "SIMAT:: DecodeUrl content_len is zero!!! \n"
#define PHONESERVER_STK_DECODE_3260_112_2_17_22_15_23_1587 "SIMAT::the length of bear overflow.\n"
#define PHONESERVER_STK_DECODE_3268_112_2_17_22_15_23_1588 "SIMAT:: DecodeBearer content_len is zero!!! "
#define PHONESERVER_STK_DECODE_3325_112_2_17_22_15_23_1589 "SIMAT::the length of file path overflow.\n"
#define PHONESERVER_STK_DECODE_3333_112_2_17_22_15_23_1590 "SIMAT:: DecodeProvisionFileRef content_len is zero!!! "
#define PHONESERVER_STK_DECODE_3390_112_2_17_22_15_23_1591 "SIMAT::the length of bear qos overflow.\n"
#define PHONESERVER_STK_DECODE_3398_112_2_17_22_15_23_1592 "SIMAT:: DecodeBearDesc content_len is zero!!! "
#define PHONESERVER_STK_DECODE_3457_112_2_17_22_15_23_1593 "SIMAT::the length of buffer size is wrong.\n"
#define PHONESERVER_STK_DECODE_3513_112_2_17_22_15_23_1594 "SIMAT::the length of net access name overflow.\n"
#define PHONESERVER_STK_DECODE_3521_112_2_17_22_15_23_1595 "SIMAT:: DecodeNetAccessName content_len is zero!!! "
#define PHONESERVER_STK_DECODE_3577_112_2_17_22_15_24_1596 "SIMAT::the length of other address overflow.\n"
#define PHONESERVER_STK_DECODE_3585_112_2_17_22_15_24_1597 "SIMAT:: DecodeOtherAddress content_len is zero!!! "
#define PHONESERVER_STK_DECODE_3641_112_2_17_22_15_24_1598 "SIMAT::the length of trans level overflow.\n"
#define PHONESERVER_STK_DECODE_3660_112_2_17_22_15_24_1599 "[DecodeTransLevel] port= %x  "
#define PHONESERVER_STK_DECODE_3704_112_2_17_22_15_24_1600 "SIMAT:: DecodeMultiMsgRef content_len is zero!!! "
#define PHONESERVER_STK_DECODE_3764_112_2_17_22_15_24_1601 "SIMAT:: DecodeMultiMsgId content_len is zero!!! "
#define PHONESERVER_STK_DECODE_3823_112_2_17_22_15_24_1602 "SIMAT:: DecodeMmContentId content_len is zero!!! "
#define PHONESERVER_STK_DECODE_4139_112_2_17_22_15_25_1603 "SIMAT::the length of channel status is not 2.\n"
#define PHONESERVER_STK_ENCODE_192_112_2_17_22_15_27_1604 "SIMAT:the address len longer than the maximum we support, error and cut it!!!\n"
#define PHONESERVER_STK_ENCODE_2277_112_2_17_22_15_31_1605 "EncodeTransLevel occuring error point is NULL!\n"
#define PHONESERVER_STK_ENCODE_3090_112_2_17_22_15_33_1606 "EncodeNetAccessName occuring error point is NULL!\n"
#define PHONESERVER_STK_ENCODE_3161_112_2_17_22_15_33_1607 "EncodeBrowserId occuring error point is NULL!\n"
#define PHONESERVER_STK_ENCODE_3201_112_2_17_22_15_33_1608 "EncodeUrl occuring error point is NULL!\n"
#define PHONESERVER_STK_ENCODE_3241_112_2_17_22_15_33_1609 "EncodeBearer occuring error point is NULL!\n"
#define PHONESERVER_STK_ENCODE_3281_112_2_17_22_15_33_1610 "EncodeProvisionFileRef occuring error point is NULL!\n"
#define PHONESERVER_STK_FUNC_59_112_2_17_22_15_33_1611 "SIMAT::SIMAT_DecodeUnknowObject point is NULL! \n"
#define PHONESERVER_STK_FUNC_95_112_2_17_22_15_33_1612 "SIMAT::SIMAT_DecodeDataObject point is NULL! \n"
#define PHONESERVER_STK_FUNC_102_112_2_17_22_15_33_1613 "SIMAT::SIMAT_DecodeDataObject error! \n"
#define PHONESERVER_STK_FUNC_129_112_2_17_22_15_33_1614 "SIMAT::ATC_EncodeDataObject point is NULL! \n"
#define PHONESERVER_STK_FUNC_265_112_2_17_22_15_34_1615 "SIMAT::ATC_EncodeDataObject point is NULL! \n"
#define PHONESERVER_STK_FUNC_273_112_2_17_22_15_34_1616 "SIMAT::ATC_EncodeDataObject error! \n"
#define PHONESERVER_STK_FUNC_276_112_2_17_22_15_34_1617 "SIMAT:object_type:%d"
#define ATC_UTMAIN_692_112_2_17_22_16_8_1618 "ATC: ATC_Task, APP_MN_CALL_ERR_IND, disconnect VT mux link, call_id:%d, cause:%d"
#define ATC_UTMAIN_721_112_2_17_22_16_8_1619 "ATC: ATC_Task, Signal Code: APP_MN_CALL_DISCONNECTED_IND, set atc_err_protected_flag"
#define ATC_UTMAIN_728_112_2_17_22_16_8_1620 "ATC: ATC_Task, APP_MN_CALL_DISCONNECTED_IND, disconnect VT mux link, call_id:%d, cause:%d"
#define ATC_UTMAIN_776_112_2_17_22_16_9_1621 "ATC: ATC_Task, Signal Code: APP_MN_ACTIVATE_PDP_CONTEXT_CNF"
#define ATC_UTMAIN_833_112_2_17_22_16_9_1622 "ATC:  invalid cid for mux link\r"
#define ATC_UTMAIN_850_112_2_17_22_16_9_1623 "ATC:  MPPP_SetPPPInterface() fail"
#define ATC_UTMAIN_855_112_2_17_22_16_9_1624 "ATC: Switch to DATA MODE"
#define ATC_UTMAIN_860_112_2_17_22_16_9_1625 "ATC: APP_MN_ACTIVATE_PDP_CONTEXT_CNF(failure), cid:%d"
#define ATC_UTMAIN_900_112_2_17_22_16_9_1626 "ATC: ATC_Task, Signal Code: APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, result:%d,pdp_num:%d,cid:%d, set atc_err_protected_flag"
#define ATC_UTMAIN_925_112_2_17_22_16_9_1627 "ATC: cid:%d is non-data-mode"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(ATC_3G_TRC)
TRACE_MSG(ATC_959_112_2_17_22_12_3_0,"ATC: write nv itemid: %d")
TRACE_MSG(ATC_966_112_2_17_22_12_3_1,"ATC: write NV_ATC_CONFIG_ID failed.")
TRACE_MSG(ATC_974_112_2_17_22_12_3_2,"ATC:  write NV_ATC_MIDI_SIZE_ID failed.")
TRACE_MSG(ATC_1018_112_2_17_22_12_3_3,"ATC: read atc_config_nv failed")
TRACE_MSG(ATC_1031_112_2_17_22_12_3_4,"ATC: read midi_size_arr failed")
TRACE_MSG(ATC_1051_112_2_17_22_12_3_5,"ATC: ATC_Mux_Setup_Link, WARNING! link_id:%d")
TRACE_MSG(ATC_1057_112_2_17_22_12_3_6,"ATC: ATC_Mux_Setup_Link, WARNING! link_id:%d is already setup")
TRACE_MSG(ATC_1070_112_2_17_22_12_3_7,"ATC: ATC_Mux_Setup_Link, ERROR! no mem to setup link_id:%d")
TRACE_MSG(ATC_1078_112_2_17_22_12_3_8,"ATC: ATC_Mux_Setup_Link, link_id: %d")
TRACE_MSG(ATC_1092_112_2_17_22_12_3_9,"ATC: ATC_Mux_Release_Link, link_id: %d")
TRACE_MSG(ATC_1155_112_2_17_22_12_4_10,"ATC: ATC_Mux_Release_Link ERROR!!!!")
TRACE_MSG(ATC_1186_112_2_17_22_12_4_11,"ATC:ATC_Add_New_Pdp_Link ERROR!!!!")
TRACE_MSG(ATC_1578_112_2_17_22_12_4_12,"ATC: ATC_Receive_Expected_Event, id:%d,event:%s")
TRACE_MSG(ATC_1582_112_2_17_22_12_4_13,"ATC: ATC_Receive_Expected_Event, id:%d,event:%d")
TRACE_MSG(ATC_1591_112_2_17_22_12_4_14,"ATC: ATC_Receive_Expected_Event, WARNING! NOT FOUND, set link id: %d")
TRACE_MSG(ATC_1596_112_2_17_22_12_4_15,"ATC: ATC_Receive_Expected_Event, WARNING! NOT FOUND, set urc link id: %d")
TRACE_MSG(ATC_1611_112_2_17_22_12_5_16,"ATC: ATC_Receive_Expected_Event, FOUND, ERROR! match_link_id:%d != call_link_id:%d")
TRACE_MSG(ATC_1616_112_2_17_22_12_5_17,"ATC: ATC_Receive_Expected_Event, FOUND, WARNING! match_link_id:%d, no call_link_id, NOT ADD EARLIER?")
TRACE_MSG(ATC_1624_112_2_17_22_12_5_18,"ATC: ATC_Receive_Expected_Event, FOUND, match_link_id:%d, call_link_id:%d")
TRACE_MSG(ATC_1629_112_2_17_22_12_5_19,"ATC: ATC_Receive_Expected_Event, FOUND, match_link_id:%d")
TRACE_MSG(ATC_1640_112_2_17_22_12_5_20,"ATC: ATC_Receive_Expected_Event over as follow")
TRACE_MSG(ATC_1656_112_2_17_22_12_5_21,"ATC: ATC_Add_Expected_Event, link_id:%d, event:%s")
TRACE_MSG(ATC_1660_112_2_17_22_12_5_22,"ATC: ATC_Add_Expected_Event, link_id:%d, event:%d")
TRACE_MSG(ATC_1698_112_2_17_22_12_5_23,"ATC: ATC_Add_Expected_Event, unknown domain_id: %d")
TRACE_MSG(ATC_1724_112_2_17_22_12_5_24,"ATC: ATC_Add_Expected_Event, Warning :The atc expected event buffer is full!Remove the first one for adding!")
TRACE_MSG(ATC_2057_112_2_17_22_12_6_25,"ATC: ATC_ReadCmdLine Warning: only read %d < %d.")
TRACE_MSG(ATC_2061_112_2_17_22_12_6_26,"ATC: ATC_ReadCmdLine Warning: garbage data ???")
TRACE_MSG(ATC_2135_112_2_17_22_12_6_27,"ATC: ATC_SendNewATInd, WARNING,s_atc_global_info_ptr is NULL.")
TRACE_MSG(ATC_2142_112_2_17_22_12_6_28,"ATC: ATC_SendNewATInd,link_id:%d,data_length:%d ,%x, %x, close:%d,count:%d,discard:%d,atc_end_ch1:%d,atc_end_ch2:%d")
TRACE_MSG(ATC_2170_112_2_17_22_12_6_29,"ATC: ATC_SendNewATInd, is_close_atc_queue is TRUE, return")
TRACE_MSG(ATC_2176_112_2_17_22_12_6_30,"ATC: ATC_SendNewATInd, link is not setup!!")
TRACE_MSG(ATC_2182_112_2_17_22_12_6_31,"ATC: ATC_SendNewATInd, atc_is_echo is TRUE, send copy back")
TRACE_MSG(ATC_2201_112_2_17_22_12_6_32,"ATC:ERROR AT BUFFER FULL  LINK %d !!!! ")
TRACE_MSG(ATC_2261_112_2_17_22_12_6_33,"ATC: ATC_Debug_SendATInd, len:%d")
TRACE_MSG(ATC_2265_112_2_17_22_12_6_34,"ATC: ATC_Debug_SendATInd, WARNING,s_atc_global_info_ptr is NULL.")
TRACE_MSG(ATC_2292_112_2_17_22_12_6_35,"ATC: ATC_Debug_SendATInd, is_close_atc_queue is TRUE, return")
TRACE_MSG(ATC_2311_112_2_17_22_12_6_36,"ATC:ERROR AT BUFFER FULL  LINK ATC_DEBUG_LINK!!!! ")
TRACE_MSG(ATC_2362_112_2_17_22_12_6_37,"ATC: ATC_Get_Link_Id, get call link id: %d")
TRACE_MSG(ATC_2397_112_2_17_22_12_6_38,"ATC: ATC_Clear_All_Exp_Result id:%d,domain_id:%d")
TRACE_MSG(ATC_2416_112_2_17_22_12_6_39,"ATC: ATC_Clear_All_Exp_Result, cid:%d, exp event: %s")
TRACE_MSG(ATC_2421_112_2_17_22_12_6_40,"ATC: ATC_Clear_All_Exp_Result, cid:%d, exp event: %d")
TRACE_MSG(ATC_2461_112_2_17_22_12_6_41,"ATC: ATC_Clear_All_Exp_Result, call_id:%d, exp event: %d")
TRACE_MSG(ATC_2519_112_2_17_22_12_7_42,"|---|-------|-----|-----|-------|------------------------------")
TRACE_MSG(ATC_2520_112_2_17_22_12_7_43,"|Idx|Link-ID| SIM | CID |Call-ID| Event")
TRACE_MSG(ATC_2521_112_2_17_22_12_7_44,"|---|-------|-----|-----|-------|------------------------------")
TRACE_MSG(ATC_2534_112_2_17_22_12_7_45,"|%-3d|  %d    |   %d   | %-3d |  %-3d  | %s")
TRACE_MSG(ATC_2543_112_2_17_22_12_7_46,"|%-3d|  %d    |   %d   | %-3d |  %-3d  | %d")
TRACE_MSG(ATC_2554_112_2_17_22_12_7_47,"|---|-------|-----|-------|------------------------------")
TRACE_MSG(ATC_2557_112_2_17_22_12_7_48,"Total:%d")
TRACE_MSG(ATC_2570_112_2_17_22_12_7_49,"ATC: SendNewATInd, len:%d")
TRACE_MSG(ATC_2574_112_2_17_22_12_7_50,"ATC: SendNewATInd, WARNING,s_atc_global_info_ptr is NULL.")
TRACE_MSG(ATC_2600_112_2_17_22_12_7_51,"ATC: SendNewATInd, is_close_atc_queue is TRUE")
TRACE_MSG(ATC_2639_112_2_17_22_12_7_52,"ATC: ATC_Create_PSD_Call,PSD_CALL,cid:%d,link_id:%d,%d,data_mode:%d,state:%d,init_mod_state:%d")
TRACE_MSG(ATC_2652_112_2_17_22_12_7_53,"ATC: ATC_Create_PSD_Call,PSD_CALL,ERROR,cid:%d,(!)link_id:%d-->%d,data_mode:%d,(!)state:%d")
TRACE_MSG(ATC_2662_112_2_17_22_12_7_54,"ATC: ATC_Create_PSD_Call,PSD_CALL,ERROR,cid:%d,link_id:%d,(!)data_mode:%d-->%d,(!)state:%d")
TRACE_MSG(ATC_2671_112_2_17_22_12_7_55,"ATC: ATC_Create_PSD_Call,PSD_CALL,WARNING,cid:%d->%d,link_id:%d,%d->%d,%d,data_mode:%d->%d,(!)state:%d")
TRACE_MSG(ATC_2684_112_2_17_22_12_7_56,"ATC: ATC_Create_PSD_Call,PSD_CALL,cid:%d is not in range")
TRACE_MSG(ATC_2707_112_2_17_22_12_7_57,"ATC: ATC_Set_PSD_Call_State,PSD_CALL,op-cid:%d,cid:%d,link_id:%d,%d,data_mode:%d,state:%d->%d")
TRACE_MSG(ATC_2713_112_2_17_22_12_7_58,"ATC: ATC_Set_PSD_Call_State,PSD_CALL,cid:%d is not in range")
TRACE_MSG(ATC_2736_112_2_17_22_12_7_59,"ATC: ATC_Clear_PSD_Call_Info,PSD_CALL,op-cid:%d,cid:%d,link_id:%d,%d,data_mode:%d,state:%d,init_mod_state:%d")
TRACE_MSG(ATC_2752_112_2_17_22_12_7_60,"ATC: ATC_Clear_PSD_Call_Info,PSD_CALL,cid:%d is not in range")
TRACE_MSG(ATC_2775_112_2_17_22_12_7_61,"ATC: ATC_List_PSD_Call_Info,PSD_CALL,op-cid:%d,cid:%d,link_id:%d,%d,data_mode:%d,state:%d,init_mod_state:%d")
TRACE_MSG(ATC_2859_112_2_17_22_12_7_62,"ATC: CMD COUNT ERROR %d, SigCode=%d")
TRACE_MSG(ATC_2874_112_2_17_22_12_7_63,"ATC: Error atc_info_ptr = %x")
TRACE_MSG(ATC_2897_112_2_17_22_12_7_64,"ATC: ATC_Task, Signal Code: ATC_MUX_LINK_SETUP_REQ, link_id:%d")
TRACE_MSG(ATC_2902_112_2_17_22_12_7_65,"ATC: ATC_Task, Signal Code: ATC_MUX_LINK_RELEASE_REQ, link_id:%d")
TRACE_MSG(ATC_2910_112_2_17_22_12_8_66,"ATC: ATC_Task, Signal Code: ATC_MUX_CLOSE_REQ")
TRACE_MSG(ATC_2917_112_2_17_22_12_8_67,"ATC: ATC_Task, Signal Code: ATC_MUX_RECV_NEW_AT, link_id:%d,len:%d,close:%d,count:%d")
TRACE_MSG(ATC_2925_112_2_17_22_12_8_68,"ATC: buffered_at_cmd_count is zero in close  ")
TRACE_MSG(ATC_2945_112_2_17_22_12_8_69,"ATC: buffered_at_cmd_count is zero in NOT close ")
TRACE_MSG(ATC_3001_112_2_17_22_12_8_70,"ATC: ATC_Task, Signal Code: SIO_RECV_NEW_AT")
TRACE_MSG(ATC_3109_112_2_17_22_12_8_71,"ATC: ATC_Task, Signal Code: APP_MN_ACTIVATE_PDP_CONTEXT_CNF")
TRACE_MSG(ATC_3126_112_2_17_22_12_8_72,"ATC: ATC_Task, Signal Code: APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, result:%d,pdp_num:%d,cid:%d, set atc_err_protected_flag")
TRACE_MSG(ATC_3203_112_2_17_22_12_8_73,"ATC: APP_MN_GPRS_DETACH_RESULT, set atc_err_protected_flag")
TRACE_MSG(ATC_3290_112_2_17_22_12_8_74,"ATC: ATC_Task, auto answer call, id:%d,type:%d")
TRACE_MSG(ATC_3402_112_2_17_22_12_9_75,"ATC: ATC_Task, Signal Code: APP_MN_CALL_DISCONNECTED_IND, set atc_err_protected_flag")
TRACE_MSG(ATC_3415_112_2_17_22_12_9_76,"ATC: ATC_Task, disconnect VT call, destroy mux link, call_id:%d")
TRACE_MSG(ATC_3420_112_2_17_22_12_9_77,"ATC: ATC_Task, APP_MN_CALL_DISCONNECTED_IND, disconnect VT mux link, call_id:%d, cause:%d")
TRACE_MSG(ATC_3449_112_2_17_22_12_9_78,"ATC: ATC_Task, APP_MN_CALL_ERR_IND, disconnect VT mux link, call_id:%d, cause:%d")
TRACE_MSG(ATC_3462_112_2_17_22_12_9_79,"ATC: ATC_Task, Signal Code: APP_MN_CALL_DISCONNECTED_IND, set atc_err_protected_flag")
TRACE_MSG(ATC_3497_112_2_17_22_12_9_80,"ATC:call ATC_ProcessReadDSPCnf")
TRACE_MSG(ATC_3505_112_2_17_22_12_9_81,"ATC: APP_MN_DEACTIVE_PS_CNF, set atc_err_protected_flag")
TRACE_MSG(ATC_3636_112_2_17_22_12_9_82,"ATC, WARNING, APP_MN_READ_SMS_CNF is not for the request of ATC")
TRACE_MSG(ATC_3673_112_2_17_22_12_9_83,"ATC, WARNING, APP_MN_SEND_SMS_CNF is not for the request of ATC")
TRACE_MSG(ATC_3703_112_2_17_22_12_9_84,"ATC, WARNING, APP_MN_WRITE_SMS_CNF is not for the request of ATC")
TRACE_MSG(ATC_3719_112_2_17_22_12_9_85,"ATC, WARNING, APP_MN_UPDATE_SMS_STATE_CNF is not for the request of ATC")
TRACE_MSG(ATC_3735_112_2_17_22_12_9_86,"ATC, WARNING, APP_MN_DELETE_ALL_SMS_CNF is not for the request of ATC")
TRACE_MSG(ATC_3780_112_2_17_22_12_9_87,"ATC:APP_MN_DEACTIVATE_SS_CNF")
TRACE_MSG(ATC_3802_112_2_17_22_12_10_88,"ATC:APP_MN_ACTIVATE_SS_CNF")
TRACE_MSG(ATC_3874_112_2_17_22_12_10_89,"ATC:call ATC_ProcessCusdInd")
TRACE_MSG(ATC_3882_112_2_17_22_12_10_90,"ATC:call ATC_ProcessCusdInd")
TRACE_MSG(ATC_3890_112_2_17_22_12_10_91,"ATC:call ATC_ProcessCusdInd")
TRACE_MSG(ATC_3919_112_2_17_22_12_10_92,"ATC:APP_MN_GET_PASSWORD_IND")
TRACE_MSG(ATC_3926_112_2_17_22_12_10_93,"ATC:APP_MN_REGISTER_PASSWORD_CNF")
TRACE_MSG(ATC_3958_112_2_17_22_12_10_94,"Eddie.Wang ******** SIM is ready, not busy, start power on PS if needed")
TRACE_MSG(ATC_3962_112_2_17_22_12_10_95,"Eddie.Wang ******** sim post power on after sim ready!!")
TRACE_MSG(ATC_4034_112_2_17_22_12_10_96,"ATC: arrive at ATC_TASK APP_MN_CALL_READY_IND")
TRACE_MSG(ATC_4038_112_2_17_22_12_10_97,"ATC: arrive at ATC_TASK APP_MN_CALL_READY_IND is_fdn_enable=%d")
TRACE_MSG(ATC_4041_112_2_17_22_12_10_98,"ATC: arrive at ATC_TASK APP_MN_CALL_READY_IND is_fdn_enable=%d")
TRACE_MSG(ATC_4051_112_2_17_22_12_10_99,"ATC: arrive at ATC_TASK APP_MN_FDN_SERVICE_IND")
TRACE_MSG(ATC_4053_112_2_17_22_12_10_100,"ATC: arrive at ATC_TASK APP_MN_FDN_SERVICE_IND is_fdn_enable=%d")
TRACE_MSG(ATC_4060_112_2_17_22_12_10_101,"ATC: arrive at ATC_TASK APP_MN_FDN_SERVICE_IND is_fdn_enable=%d")
TRACE_MSG(ATC_4127_112_2_17_22_12_10_102,"ATC:call ATC_ProcessCusdInd")
TRACE_MSG(ATC_4142_112_2_17_22_12_10_103,"ATC:ATC_STSF_EXP_IND")
TRACE_MSG(ATC_4232_112_2_17_22_12_10_104,"ATC:call ATC_ProcessSetBandCnf")
TRACE_MSG(ATC_4245_112_2_17_22_12_10_105,"ATC: APP_MN_PS_POWER_OFF_CNF: reset_flag=%d.")
TRACE_MSG(ATC_4297_112_2_17_22_12_10_106,"ATC: cfun_flag:%d,APP_MN_PS_POWER_ON_CNF,URC:+SIND: 8")
TRACE_MSG(ATC_4370_112_2_17_22_12_11_107,"Eddie.Wang ******** sim is powered on, not busy!!!sim_flag[g_current_card_id]=%d, need ps power on?%d")
TRACE_MSG(ATC_4413_112_2_17_22_12_11_108,"ATC: cfun_flag:%d,APP_MN_SIM_POWER_OFF_CNF,URC:+SIND: 8")
TRACE_MSG(ATC_4518_112_2_17_22_12_11_109,"ATC: rcv atcmd rsp,dlci:%d,info:%s")
TRACE_MSG(ATC_4543_112_2_17_22_12_11_110,"ATC: rcv atcmd result,dlci:%d,info:%s")
TRACE_MSG(ATC_4567_112_2_17_22_12_11_111,"ATC: ATC_Task: Unknown Signal Code (%d)!")
TRACE_MSG(ATC_4589_112_2_17_22_12_11_112,"ATC: ATC_Initialize failed in ATCM_CreateHeap.")
TRACE_MSG(ATC_4600_112_2_17_22_12_11_113,"ATC: ATC_Initialize failed in SCI_ALLOC for s_atc_global_info_ptr.")
TRACE_MSG(ATC_4773_112_2_17_22_12_11_114,"ATC: ATC_RecNewLineSig,link_id:%d,len:%d")
TRACE_MSG(ATC_4833_112_2_17_22_12_12_115,"ATC: backspace? len: %d->%d")
TRACE_MSG(ATC_4850_112_2_17_22_12_12_116,"ATC: ATC_RecNewLineSig, len:%d,hex:%s,line:%s")
TRACE_MSG(ATC_4855_112_2_17_22_12_12_117,"ATC: ATC_RecNewLineSig, len:%d")
TRACE_MSG(ATC_5028_112_2_17_22_12_12_118,"ATC: ATC_RecNewLineSig, ignore ip data, len:%d->%d, new atcmd:%s")
TRACE_MSG(ATC_5055_112_2_17_22_12_12_119,"ATC: ATC_RecNewLineSig, Assert Failure sharp len too long.")
TRACE_MSG(ATC_5062_112_2_17_22_12_12_120,"ATC: ATC_RecNewLineSig, Call the external function")
TRACE_MSG(ATC_5083_112_2_17_22_12_12_121,"ATC: ATC_RecNewLineSig, Parse AT cmd line success.")
TRACE_MSG(ATC_5107_112_2_17_22_12_12_122,"ATC: ATC_RecNewLineSig, Parse SMS PDU successful")
TRACE_MSG(ATC_5111_112_2_17_22_12_12_123,"ATC: ATC_RecNewLineSig, Parse AT command line failed.")
TRACE_MSG(ATC_5293_112_2_17_22_12_12_124,"ATC: ATC_RecNewLineSig, len:%d,hex:%s,line:%s")
TRACE_MSG(ATC_5298_112_2_17_22_12_12_125,"ATC: ATC_RecNewLineSig, len:%d")
TRACE_MSG(ATC_5465_112_2_17_22_12_13_126,"ATC: ATC_RecNewLineSig, ignore ip data, len:%d->%d, new atcmd:%s")
TRACE_MSG(ATC_5491_112_2_17_22_12_13_127,"ATC: ATC_RecNewLineSig, Assert Failure sharp len too long.")
TRACE_MSG(ATC_5498_112_2_17_22_12_13_128,"ATC: ATC_RecNewLineSig, Call the external function")
TRACE_MSG(ATC_5519_112_2_17_22_12_13_129,"ATC: ATC_RecNewLineSig, Parse AT cmd line success.")
TRACE_MSG(ATC_5543_112_2_17_22_12_13_130,"ATC: ATC_RecNewLineSig, Parse SMS PDU successful")
TRACE_MSG(ATC_5547_112_2_17_22_12_13_131,"ATC: ATC_RecNewLineSig, Parse AT command line failed.")
TRACE_MSG(ATC_5639_112_2_17_22_12_13_132,"ATC: ATC_DespatchCmd, cmd_type:%d(1-exe,2-set,4-read,8-test)")
TRACE_MSG(ATC_5646_112_2_17_22_12_13_133,"ATC: ATC_DespatchCmd, sim_flag[g_current_card_id]:%d, cpin:%d")
TRACE_MSG(ATC_5752_112_2_17_22_12_13_134,"ATC: ATC_DespatchCmd, %s Test Command: Default Response")
TRACE_MSG(ATC_5915_112_2_17_22_12_14_135,"ATC: ATC_CheckCmd,Check AT Command valid.")
TRACE_MSG(ATC_5919_112_2_17_22_12_14_136,"ATC: ATC_CheckCmd,Check AT Command invalid.")
TRACE_MSG(ATC_5954_112_2_17_22_12_14_137,"ATC: filter state, not report unsolicited code.")
TRACE_MSG(ATC_5967_112_2_17_22_12_14_138,"ATC: ATC_BuildResultCodeRsp, err text string too long")
TRACE_MSG(ATC_6010_112_2_17_22_12_14_139,"ATC: ATC_BuildResultCodeRsp, q_value is TRUE")
TRACE_MSG(ATC_6070_112_2_17_22_12_14_140,"ATC: ATC_BuildResultCodeRsp,The sent string is %s")
TRACE_MSG(ATC_6138_112_2_17_22_12_14_141,"ATC: ATC_BuildResultErrCodeRsp, q_value is TRUE")
TRACE_MSG(ATC_6157_112_2_17_22_12_14_142,"ATC: ATC_BuildResultErrCodeRsp,The sent string is %s")
TRACE_MSG(ATC_6249_112_2_17_22_12_14_143,"ATC: ATC_BuildSmsCodeRsp,The sent string is %s")
TRACE_MSG(ATC_6311_112_2_17_22_12_15_144,"ATC: filter state, not report unsolicited code: %s.")
TRACE_MSG(ATC_6326_112_2_17_22_12_15_145,"ATC: ATC_BuildInfoRsp, q_value is TRUE")
TRACE_MSG(ATC_6332_112_2_17_22_12_15_146,"ATC: ATC_BuildInfoRsp, rsp_str_ptr's length is zero")
TRACE_MSG(ATC_6386_112_2_17_22_12_15_147,"ATC: ATC_BuildInfoRsp,The sent string is %s")
TRACE_MSG(ATC_6491_112_2_17_22_12_15_148,"ATC: ATC_BuildTestCmdResponse, q_value is TRUE")
TRACE_MSG(ATC_6575_112_2_17_22_12_15_149,"ATC:ATC_BuildTestCmdResponse,The sent string is %s")
TRACE_MSG(ATC_6915_112_2_17_22_12_16_150,"ATC: ATC_InitConfig cfun_flag %d ")
TRACE_MSG(ATC_7130_112_2_17_22_12_16_151,"ATC:SCI_RegisterMsg,STK")
TRACE_MSG(ATC_7203_112_2_17_22_12_16_152,"ATC: ATC_DisconnectModem ")
TRACE_MSG(ATC_7212_112_2_17_22_12_16_153,"ATC: ATC_DisconnectModem,ERROR,s_atc_global_info_ptr is NULL.")
TRACE_MSG(ATC_7218_112_2_17_22_12_16_154,"ATC: ATC_DisconnectModem, illegal cid:%d, not in range")
TRACE_MSG(ATC_7227_112_2_17_22_12_16_155,"ATC: ATC_DisconnectModem be invoked for more times, cid:%d is already inactive!!!")
TRACE_MSG(ATC_7248_112_2_17_22_12_16_156,"ATC: ATC_DisconnectModem, no call(cid:%d) link id! PSD is not existed!")
TRACE_MSG(ATC_7254_112_2_17_22_12_16_157,"ATC: ATC_DisconnectModem, failure in MNGPRS_DeactivatePdpContext")
TRACE_MSG(ATC_7275_112_2_17_22_12_16_158,"ATC: ATC_DisconnectModem, no call(cid:%d) link id! PSD is not existed!")
TRACE_MSG(ATC_7281_112_2_17_22_12_16_159,"ATC: ATC_DisconnectModem, failure in MNGPRS_DeactivatePdpContext")
TRACE_MSG(ATC_7316_112_2_17_22_12_17_160,"ATC: ATC_DisconnectModem,ERROR,s_atc_global_info_ptr is NULL.")
TRACE_MSG(ATC_7392_112_2_17_22_12_17_161,"ATC: Assert Failure unknow chr signal.")
TRACE_MSG(ATC_7497_112_2_17_22_12_17_162,"ATC: ATHEventHandle,ev:%d,ERROR!!! link_id:%d not in range!")
TRACE_MSG(ATC_7518_112_2_17_22_12_17_163,"ATC: ATHEventHandle,NEW_PDP(cid:%d),ERROR:link_id:%d,cid:%d,ath_stat:(!)LISTENING")
TRACE_MSG(ATC_7524_112_2_17_22_12_17_164,"ATC: ATHEventHandle,NEW_PDP(cid:%d),Reset,link_id:%d,cid:%d->INVALID,ath_stat:WAITING->NULL")
TRACE_MSG(ATC_7535_112_2_17_22_12_17_165,"ATC: ATHEventHandle,NEW_PDP(cid:%d),Reset,link_id:%d,cid:%d->INVALID,ath_stat:AVOID_REPETITION->NULL")
TRACE_MSG(ATC_7548_112_2_17_22_12_17_166,"ATC: ATHEventHandle,NEW_PPP(cid:%d),link_id:%d,cid:%d->%d,ath_stat:NULL->LISTENING")
TRACE_MSG(ATC_7563_112_2_17_22_12_17_167,"ATC: ATHEventHandle,NEW_CS,ERROR:link_id:%d,cid:%d,ath_stat:%d==LISTENING")
TRACE_MSG(ATC_7569_112_2_17_22_12_17_168,"ATC: ATHEventHandle,NEW_CS,link_id:%d,cid:%d,ath_stat:%d")
TRACE_MSG(ATC_7582_112_2_17_22_12_17_169,"ATC: ATHEventHandle,NEW_CS,link_id:%d,cid:%d,ath_stat:%d")
TRACE_MSG(ATC_7598_112_2_17_22_12_17_170,"ATC: ATHEventHandle,RECV_ATH,link_id:%d,cid:%d,ath_stat:LISTENING->AVOID_REPETITION")
TRACE_MSG(ATC_7636_112_2_17_22_12_17_171,"ATC: ATHEventHandle,RECV_ATH,link_id:%d,change [%d] expected event: ACT->DEACT")
TRACE_MSG(ATC_7649_112_2_17_22_12_17_172,"ATC: ATHEventHandle,RECV_ATH,add deact expected event. cid:%d")
TRACE_MSG(ATC_7667_112_2_17_22_12_17_173,"ATC: ATHEventHandle,RECV_ATH,link_id:%d,cid:%d,ath_stat:WAITING->AVOID_REPETITION")
TRACE_MSG(ATC_7683_112_2_17_22_12_17_174,"ATC: ATHEventHandle,RECV_ATH,link_id:%d,cid:%d,ath_stat:AVOID_REPETITION")
TRACE_MSG(ATC_7698_112_2_17_22_12_17_175,"ATC: ATHEventHandle,ACT_REJ,link_id:%d,cid:%d,ath_stat:LISTENING->WAITING")
TRACE_MSG(ATC_7713_112_2_17_22_12_17_176,"ATC: ATHEventHandle,ACT_REJ,ERROR:link_id:%d,cid:%d,%d,ath_stat:%d")
TRACE_MSG(ATC_7734_112_2_17_22_12_17_177,"ATC: ATHEventHandle,DEACT,link_id:%d,cid:%d,ath_stat:LISTENING->WAITING")
TRACE_MSG(ATC_7757_112_2_17_22_12_17_178,"ATC: ATHEventHandle,DEACT,ERROR:link_id:%d,cid:%d,%d,ath_stat:%d")
TRACE_MSG(ATC_7768_112_2_17_22_12_17_179,"ATC: ATHEventHandle,WAIT_TIMEOUT,link_id:%d,cid:%d,ath_stat:WAITING->NULL")
TRACE_MSG(ATC_7779_112_2_17_22_12_17_180,"ATC: ATHEventHandle,AVOID_REP_TIMEOUT,link_id:%d,cid:%d,ath_stat:AVOID_REPETITION->NULL")
TRACE_MSG(ATC_7787_112_2_17_22_12_17_181,"ATC: ATHEventHandle,unknown event:%d,link_id:%d")
TRACE_MSG(ATC_7804_112_2_17_22_12_18_182,"ATC: ATC_Link_Property invalid link id: %d.")
TRACE_MSG(ATC_7825_112_2_17_22_12_18_183,"ATC: ATC_Link_Is_Existed_PSD invalid link id: %d.")
TRACE_MSG(ATC_7899_112_2_17_22_12_18_184,"ATC: get call state error")
TRACE_MSG(ATC_7914_112_2_17_22_12_18_185,"ATC: ATC_SynchronizeService, dual_sys= %d")
TRACE_MSG(ATC_7922_112_2_17_22_12_18_186,"ATC: SynchronizeService, ERROR!!! status: %d, MN_SS_CFU")
TRACE_MSG(ATC_7934_112_2_17_22_12_18_187,"ATC: SynchronizeService, ERROR!!! status: %d, MN_SS_CFB")
TRACE_MSG(ATC_7946_112_2_17_22_12_18_188,"ATC: SynchronizeService, ERROR!!! status: %d, MN_SS_CFNRY")
TRACE_MSG(ATC_7958_112_2_17_22_12_18_189,"ATC: SynchronizeService, ERROR!!! status: %d, MN_SS_CFNRC")
TRACE_MSG(ATC_7976_112_2_17_22_12_18_190,"ATC: SynchronizeService, status: %d->%d")
TRACE_MSG(ATC_7988_112_2_17_22_12_18_191,"ATC: SynchronizeService, finished! cfall:%d,cfu:%d,cfcond:%d,cfb:%d,cfnrc:%d,cfnry:%d")
TRACE_MSG(ATC_8013_112_2_17_22_12_18_192,"ATC: ATC_ProcessEsqoptTimeout,+ECSQ rxlev=%d,rxqual=%d")
TRACE_MSG(ATC_8030_112_2_17_22_12_18_193,"ATC: +ECSQ deactivate timer first while time out")
TRACE_MSG(ATC_8061_112_2_17_22_12_18_194,"ATC: ATC_Get_Data_Link_Id: invalid cid for MUX max")
TRACE_MSG(ATC_8415_112_2_17_22_12_19_195,"ATC: ATC_Find_Expected_Event find event: %s, number: %d")
TRACE_MSG(ATC_8419_112_2_17_22_12_19_196,"ATC: ATC_Find_Expected_Event find event: %d, number: %d")
TRACE_MSG(ATC_8448_112_2_17_22_12_19_197,"ATC: ATC_UpDSCIInfo  dsci_flag: %d, expected_event: %s , cardid: %d")
TRACE_MSG(ATC_8453_112_2_17_22_12_19_198,"ATC: ATC_UpDSCIInfo  dsci_flag: %d, expected_event: %d, cardid: %d")
TRACE_MSG(ATC_8513_112_2_17_22_12_19_199,"ATC: ATC_UpSINDInfo, IndLevel: %d, atc_wind_bit_flag: %d, upValue: %d, UpFlag:%d")
TRACE_MSG(ATC_8772_112_2_17_22_12_19_200,"ATC: UP +CREG, mode:%d,unsolicited flag:%d,oper_status:%d")
TRACE_MSG(ATC_8798_112_2_17_22_12_20_201,"ATC: ATC_UpCREGInfo, unknow creg enum:%d.")
TRACE_MSG(ATC_8832_112_2_17_22_12_20_202,"ATC: ATC_UpCREGInfo, unknow creg enum:%d.")
TRACE_MSG(ATC_8857_112_2_17_22_12_20_203,"ATC: UP +CGREG, mode:%d,unsolicited flag:%d,gprs_oper_status:%d")
TRACE_MSG(ATC_8886_112_2_17_22_12_20_204,"ATC: ATC_UpCGREGInfo, unknow cgreg enum:%d.")
TRACE_MSG(ATC_8921_112_2_17_22_12_20_205,"ATC: ATC_UpCGREGInfo, unknow cgreg enum:%d.")
TRACE_MSG(ATC_8960_112_2_17_22_12_20_206,"ATC: ATC_UpCENDInfo, invalid signal code:%d.")
TRACE_MSG(ATC_9364_112_2_17_22_12_21_207,"ATC_ATC_MsgIsRelatedToSim: event not checked return FALSE")
TRACE_MSG(ATC_9502_112_2_17_22_12_21_208,"ATC: filter state, not report unsolicited code: %s.")
TRACE_MSG(ATC_9520_112_2_17_22_12_21_209,"ATC: ATC_BuildUnsolicitedInfo, q_value is TRUE")
TRACE_MSG(ATC_9526_112_2_17_22_12_21_210,"ATC: ATC_BuildUnsolicitedInfo, rsp_str_ptr's length is zero")
TRACE_MSG(ATC_9579_112_2_17_22_12_21_211,"ATC: ATC_BuildUnsolicitedInfo,1")
TRACE_MSG(ATC_9586_112_2_17_22_12_21_212,"ATC: ATC_BuildUnsolicitedInfo,2")
TRACE_MSG(ATC_9595_112_2_17_22_12_21_213,"ATC: ATC_BuildUnsolicitedInfo,3")
TRACE_MSG(ATC_9603_112_2_17_22_12_21_214,"ATC: ATC_BuildUnsolicitedInfo,4")
TRACE_MSG(ATC_BASIC_CMD_477_112_2_17_22_12_25_215,"ATC ATC_ProcessV the param num is %d, invalid!")
TRACE_MSG(ATC_BASIC_CMD_487_112_2_17_22_12_25_216,"ATC: ATC_ProcessV EXE: cur_param_num:%d")
TRACE_MSG(ATC_BASIC_CMD_493_112_2_17_22_12_25_217,"ATC: ATC_ProcessV EXE: PARAM1_FLAG, %d")
TRACE_MSG(ATC_BASIC_CMD_496_112_2_17_22_12_25_218,"ATC: ATC_ProcessV the param num is %d, invalid!")
TRACE_MSG(ATC_BASIC_CMD_737_112_2_17_22_12_25_219,"ATC: ATC_ProcessCREG, plmn_status:%d,rat:%d,cell_cap:%d%d%d%d")
TRACE_MSG(ATC_BASIC_CMD_847_112_2_17_22_12_26_220,"ATC: ATC_ProcessCSQ,+CSQ rxlev=%d,rxqual=%d")
TRACE_MSG(ATC_BASIC_CMD_884_112_2_17_22_12_26_221,"ATC: +ESQOPT  deactivate timer while active")
TRACE_MSG(ATC_BASIC_CMD_894_112_2_17_22_12_26_222,"ATC: +ESQOPT , use defaul value 10!")
TRACE_MSG(ATC_BASIC_CMD_905_112_2_17_22_12_26_223,"ATC: +ESQOPT  ATC_ESQOPT_TIMER_INTERVAL!")
TRACE_MSG(ATC_BASIC_CMD_911_112_2_17_22_12_26_224,"ATC: +ESQOPT  ATC_ESQOPT_RSSI_THRESHOLD!")
TRACE_MSG(ATC_BASIC_CMD_918_112_2_17_22_12_26_225,"ATC: +ESQOPT deactivate timer first while active")
TRACE_MSG(ATC_BASIC_CMD_931_112_2_17_22_12_26_226,"ATC: +ESQOPT activate timer")
TRACE_MSG(ATC_BASIC_CMD_939_112_2_17_22_12_26_227,"ATC: +ESQOPT option=%d,timer_value=%d,thres_value=%d")
TRACE_MSG(ATC_BASIC_CMD_947_112_2_17_22_12_26_228,"ATC: ATC_ProcessESQOPT, +ECSQ rxlev=%d,rxqual=%d")
TRACE_MSG(ATC_BASIC_CMD_1401_112_2_17_22_12_27_229,"ATC: +CGMR: %s")
TRACE_MSG(ATC_BASIC_CMD_1544_112_2_17_22_12_27_230,"ATC: Assert Failure pin len error.")
TRACE_MSG(ATC_BASIC_CMD_1568_112_2_17_22_12_27_231,"ATC: Assert Failure pin length error.")
TRACE_MSG(ATC_BASIC_CMD_1591_112_2_17_22_12_27_232,"ATC:sim_flag=%d,cpin=%d,sim_ready[g_current_card_id]=%d|%d")
TRACE_MSG(ATC_BASIC_CMD_1610_112_2_17_22_12_27_233,"ATC:ATC_ProcessCPIN,  APP_MN_SIM_READY_IND")
TRACE_MSG(ATC_BASIC_CMD_1616_112_2_17_22_12_27_234,"ATC:ATC_ProcessCPIN,  APP_MN_SIM_PIN_FUNC_CNF")
TRACE_MSG(ATC_BASIC_CMD_1624_112_2_17_22_12_27_235,"ATC: ATC_ProcessCPIN, PIN1,failure ")
TRACE_MSG(ATC_BASIC_CMD_1652_112_2_17_22_12_27_236,"ATC: ATC_ProcessCPIN, PIN2, failure ")
TRACE_MSG(ATC_BASIC_CMD_1701_112_2_17_22_12_28_237,"ATC: ATC_ProcessCPIN, sim_flag:TRUE, PUK1, failure ")
TRACE_MSG(ATC_BASIC_CMD_1722_112_2_17_22_12_28_238,"ATC: ATC_ProcessCPIN, sim_flag:TRUE,PUK1, failure in MNPHONE_ResponsePinEx")
TRACE_MSG(ATC_BASIC_CMD_1769_112_2_17_22_12_28_239,"ATC: ATC_ProcessCPIN, PUK2, failure in MNPHONE_ResponsePinEx")
TRACE_MSG(ATC_BASIC_CMD_1807_112_2_17_22_12_28_240,"ATC: Assert Failure unknow cpin state %d")
TRACE_MSG(ATC_BASIC_CMD_1817_112_2_17_22_12_28_241,"ATC: Assert Failure cpin state not allowed.")
TRACE_MSG(ATC_BASIC_CMD_1876_112_2_17_22_12_28_242,"ATC: ecpin2 the param is error")
TRACE_MSG(ATC_BASIC_CMD_1890_112_2_17_22_12_28_243,"ATC: Assert Failure pin len error.")
TRACE_MSG(ATC_BASIC_CMD_1918_112_2_17_22_12_28_244,"ATC: Assert Failure pin length error.")
TRACE_MSG(ATC_BASIC_CMD_1952_112_2_17_22_12_28_245,"ATC: ATC_ProcessCPIN, PIN2, failure in MNPHONE_OperatePinEx")
TRACE_MSG(ATC_BASIC_CMD_1995_112_2_17_22_12_28_246,"ATC: ATC_ProcessCPIN, PIN2, failure in MNPHONE_ResponsePinEx")
TRACE_MSG(ATC_BASIC_CMD_2050_112_2_17_22_12_29_247,"ATC: Assert Failure unknow ecpin2 state %d")
TRACE_MSG(ATC_BASIC_CMD_2097_112_2_17_22_12_29_248,"ATC: unknow pin type %d.")
TRACE_MSG(ATC_BASIC_CMD_2141_112_2_17_22_12_29_249,"ATC:ATC_ProcessCIMI,imsi_len=%d")
TRACE_MSG(ATC_BASIC_CMD_2388_112_2_17_22_12_29_250,"ATC: receive the APP_MN_SYNC_IND signal when no call in outgoing state")
TRACE_MSG(ATC_BASIC_CMD_2397_112_2_17_22_12_29_251,"ATC: receive the APP_MN_SYNC_IND signal")
TRACE_MSG(ATC_BASIC_CMD_2437_112_2_17_22_12_29_252,"ATC: ATC_ProcessMnSyncInd is_emergency_call:%d, call_amount: %d")
TRACE_MSG(ATC_BASIC_CMD_2537_112_2_17_22_12_30_253,"ATC: ATC_ProcessNetworkStatusInd, plmn_status:%d,rat:%d,cell_cap:%d%d%d%d")
TRACE_MSG(ATC_BASIC_CMD_2925_112_2_17_22_12_31_254,"ATC: ATC_ProcessCPBW, param1 is not string!")
TRACE_MSG(ATC_BASIC_CMD_2930_112_2_17_22_12_31_255,"ATC: $$$ the input  is %s")
TRACE_MSG(ATC_BASIC_CMD_2935_112_2_17_22_12_31_256,"ATC: $$ set timer success")
TRACE_MSG(ATC_BASIC_CMD_2940_112_2_17_22_12_31_257,"ATC:  set timer fail")
TRACE_MSG(ATC_BASIC_CMD_2947_112_2_17_22_12_31_258,"ATC:  the parameter check fail")
TRACE_MSG(ATC_BASIC_CMD_2973_112_2_17_22_12_31_259,"ATC: $$$ the status is %d")
TRACE_MSG(ATC_BASIC_CMD_3208_112_2_17_22_12_31_260,"ATC:param_len = %d")
TRACE_MSG(ATC_BASIC_CMD_3223_112_2_17_22_12_31_261,"ATC:AT+SSEA,audio_data.UL_equalizer_coeff[%d]=%x")
TRACE_MSG(ATC_BASIC_CMD_3229_112_2_17_22_12_31_262,"ATC:AT+SSEA,audio_data.DL_equalizer_coeff[%d]=%x")
TRACE_MSG(ATC_BASIC_CMD_3233_112_2_17_22_12_31_263,"ATC:audio_data.DL_equalizer_coeff=%x,%x,%x")
TRACE_MSG(ATC_BASIC_CMD_3235_112_2_17_22_12_31_264,"ATC:audio_data.UL_PGA_gain=%x")
TRACE_MSG(ATC_BASIC_CMD_3238_112_2_17_22_12_31_265,"ATC:audio_data.DL_PGA_gain=%x")
TRACE_MSG(ATC_BASIC_CMD_3241_112_2_17_22_12_31_266,"ATC:audio_data.UL_digital_gain=%x")
TRACE_MSG(ATC_BASIC_CMD_3244_112_2_17_22_12_31_267,"ATC:audio_data.UL_digital_gain=%x")
TRACE_MSG(ATC_BASIC_CMD_3247_112_2_17_22_12_31_268,"ATC:audio_data.UL_digital_scale=%x")
TRACE_MSG(ATC_BASIC_CMD_3250_112_2_17_22_12_31_269,"ATC:audio_data.DL_digital_scale=%x")
TRACE_MSG(ATC_BASIC_CMD_3253_112_2_17_22_12_31_270,"ATC:audio_data.midi_PGA_gain_base=%x")
TRACE_MSG(ATC_BASIC_CMD_3256_112_2_17_22_12_31_271,"ATC:audio_data.digital_sidetone_gain=%x")
TRACE_MSG(ATC_BASIC_CMD_3261_112_2_17_22_12_31_272,"ATC:audio_data.DL_IIR1_coeff[%d]=%x")
TRACE_MSG(ATC_BASIC_CMD_3267_112_2_17_22_12_31_273,"ATC:audio_data.DL_IIR2_coeff[%d]=%x")
TRACE_MSG(ATC_BASIC_CMD_3279_112_2_17_22_12_31_274,"ATC:flag = %d")
TRACE_MSG(ATC_BASIC_CMD_3386_112_2_17_22_12_32_275,"ATC: bat capacity %d.")
TRACE_MSG(ATC_BASIC_CMD_3504_112_2_17_22_12_32_276,"ATC: call MNPHONE_GetMeasReportEx")
TRACE_MSG(ATC_BASIC_CMD_3510_112_2_17_22_12_32_277,"ATC: get ncell info succ")
TRACE_MSG(ATC_BASIC_CMD_3528_112_2_17_22_12_32_278,"ATC: the index: %d")
TRACE_MSG(ATC_BASIC_CMD_3553_112_2_17_22_12_32_279,"ATC: the string length is %d")
TRACE_MSG(ATC_BASIC_CMD_3561_112_2_17_22_12_32_280,"ATC: Set csq false")
TRACE_MSG(ATC_BASIC_CMD_3644_112_2_17_22_12_32_281,"ATC: Enter the function of ATC_ProcessCCED()")
TRACE_MSG(ATC_BASIC_CMD_3666_112_2_17_22_12_32_282,"ATC: Set csq true")
TRACE_MSG(ATC_BASIC_CMD_4078_112_2_17_22_12_33_283,"ATC: The remain size is %d")
TRACE_MSG(ATC_BASIC_CMD_4107_112_2_17_22_12_33_284,"ATC: create a new file")
TRACE_MSG(ATC_BASIC_CMD_4150_112_2_17_22_12_33_285,"ATC: add to the present file")
TRACE_MSG(ATC_BASIC_CMD_4187_112_2_17_22_12_33_286,"ATC: The parameters is error for the midi files")
TRACE_MSG(ATC_BASIC_CMD_4210_112_2_17_22_12_33_287,"ATC: The parameters is error for the midi files")
TRACE_MSG(ATC_BASIC_CMD_4263_112_2_17_22_12_33_288,"ATC: SIO string lenght=%d,s_pre_block_num=%d,Data1=%x,Data2=%x,Data3=%x,Data4=%x,Data5=%x")
TRACE_MSG(ATC_BASIC_CMD_4283_112_2_17_22_12_33_289,"ATC: write file error")
TRACE_MSG(ATC_BASIC_CMD_4285_112_2_17_22_12_33_290,"ATC: the added file:Name=%s, size=%d")
TRACE_MSG(ATC_BASIC_CMD_4291_112_2_17_22_12_33_291,"ATC: open file error")
TRACE_MSG(ATC_BASIC_CMD_4301_112_2_17_22_12_33_292,"ATC: write file error")
TRACE_MSG(ATC_BASIC_CMD_4303_112_2_17_22_12_33_293,"ATC: the added file size is %d")
TRACE_MSG(ATC_BASIC_CMD_4309_112_2_17_22_12_33_294,"ATC: open file error")
TRACE_MSG(ATC_BASIC_CMD_4317_112_2_17_22_12_33_295,"ATC: cancel the given midi files")
TRACE_MSG(ATC_BASIC_CMD_4322_112_2_17_22_12_33_296,"ATC: the midi file format is error")
TRACE_MSG(ATC_BASIC_CMD_4346_112_2_17_22_12_33_297,"ATC: smgv SIO string lenght=%d,s_pre_block_chk_num=%d,Data1=%x,Data2=%x,Data3=%x,Data4=%x,Data5=%x, terminator = 0x%x")
TRACE_MSG(ATC_BASIC_CMD_4355_112_2_17_22_12_34_298,"ATC: smgv receive a frame head.")
TRACE_MSG(ATC_BASIC_CMD_4364_112_2_17_22_12_34_299,"ATC: smgv receive a frame stop, switch to AT command mode.")
TRACE_MSG(ATC_BASIC_CMD_4365_112_2_17_22_12_34_300,"ATC: smgv ctrlz_end set to false.")
TRACE_MSG(ATC_BASIC_CMD_4379_112_2_17_22_12_34_301,"ATC: the midi file format is error")
TRACE_MSG(ATC_BASIC_CMD_4578_112_2_17_22_12_34_302,"ATC: smgv out_len = %d, s_file_chk_len = %d, s_file_received_size = %d")
TRACE_MSG(ATC_BASIC_CMD_4579_112_2_17_22_12_34_303,"ATC: smgv ctrlz_end set to false.")
TRACE_MSG(ATC_BASIC_CMD_4603_112_2_17_22_12_34_304,"ATC: smgv write file error")
TRACE_MSG(ATC_BASIC_CMD_4605_112_2_17_22_12_34_305,"ATC: the added file:Name=%s, size=%d")
TRACE_MSG(ATC_BASIC_CMD_4612_112_2_17_22_12_34_306,"ATC: smgv open file error")
TRACE_MSG(ATC_BASIC_CMD_4623_112_2_17_22_12_34_307,"ATC: smgv write file error")
TRACE_MSG(ATC_BASIC_CMD_4625_112_2_17_22_12_34_308,"ATC: the added file size is %d")
TRACE_MSG(ATC_BASIC_CMD_4632_112_2_17_22_12_34_309,"ATC: smgv open file error")
TRACE_MSG(ATC_BASIC_CMD_4644_112_2_17_22_12_34_310,"ATC: smgv send success frame num! num = %d")
TRACE_MSG(ATC_BASIC_CMD_4654_112_2_17_22_12_34_311,"ATC: smgv send error frame num! num = %d")
TRACE_MSG(ATC_BASIC_CMD_4702_112_2_17_22_12_34_312,"ATC: file length = %d,block_num = %d,block_id =")
TRACE_MSG(ATC_BASIC_CMD_4731_112_2_17_22_12_34_313,"ATC: open file error")
TRACE_MSG(ATC_BASIC_CMD_4758_112_2_17_22_12_34_314,"ATC: open file error")
TRACE_MSG(ATC_BASIC_CMD_4783_112_2_17_22_12_34_315,"ATC: create a new file,file_name=%s,file_name_len=%d")
TRACE_MSG(ATC_BASIC_CMD_4798_112_2_17_22_12_34_316,"ATC: error in block_num,s_file_name")
TRACE_MSG(ATC_BASIC_CMD_4854_112_2_17_22_12_35_317,"ATC: create a new file,file_name=%s,file_name_len=%d")
TRACE_MSG(ATC_BASIC_CMD_4881_112_2_17_22_12_35_318,"ATC: smgv ctrlz_end set to true.")
TRACE_MSG(ATC_BASIC_CMD_4932_112_2_17_22_12_35_319,"ATC: ATC_ProcessSMGF, param2 is not string!")
TRACE_MSG(ATC_BASIC_CMD_4944_112_2_17_22_12_35_320,"ATC: handle file:Name=%s")
TRACE_MSG(ATC_BASIC_CMD_4955_112_2_17_22_12_35_321,"ATC: the parameters is error")
TRACE_MSG(ATC_BASIC_CMD_4985_112_2_17_22_12_35_322,"ATC: the parameters is error")
TRACE_MSG(ATC_BASIC_CMD_4998_112_2_17_22_12_35_323,"ATC: ATC_ProcessSMGF, param6 is not string!")
TRACE_MSG(ATC_BASIC_CMD_5005_112_2_17_22_12_35_324,"ATC: handle file:NewName=%s")
TRACE_MSG(ATC_BASIC_CMD_5023_112_2_17_22_12_35_325,"ATC: the parameters is error")
TRACE_MSG(ATC_BASIC_CMD_5076_112_2_17_22_12_35_326,"ATC: ATC_ProcessSMGV, param2 is not string!")
TRACE_MSG(ATC_BASIC_CMD_5088_112_2_17_22_12_35_327,"ATC: handle file:Name=%s")
TRACE_MSG(ATC_BASIC_CMD_5099_112_2_17_22_12_35_328,"ATC: the parameters is error")
TRACE_MSG(ATC_BASIC_CMD_5130_112_2_17_22_12_35_329,"ATC: ATC_ProcessSMGV, param4 is not num!")
TRACE_MSG(ATC_BASIC_CMD_5139_112_2_17_22_12_35_330,"ATC: the parameters is error")
TRACE_MSG(ATC_BASIC_CMD_5151_112_2_17_22_12_35_331,"ATC: ATC_ProcessSMGV, param6 is not string!")
TRACE_MSG(ATC_BASIC_CMD_5158_112_2_17_22_12_35_332,"ATC: handle file:NewName=%s")
TRACE_MSG(ATC_BASIC_CMD_5175_112_2_17_22_12_35_333,"ATC: the parameters is error")
TRACE_MSG(ATC_BASIC_CMD_5220_112_2_17_22_12_35_334,"SMGD:error in para1")
TRACE_MSG(ATC_BASIC_CMD_5226_112_2_17_22_12_35_335,"SMGD:error in para2")
TRACE_MSG(ATC_BASIC_CMD_5232_112_2_17_22_12_35_336,"ATC: SMGD, param2 is not string!")
TRACE_MSG(ATC_BASIC_CMD_5239_112_2_17_22_12_35_337,"SMGD:error in length of para2")
TRACE_MSG(ATC_BASIC_CMD_5245_112_2_17_22_12_35_338,"ATC: handle file:Name=%s")
TRACE_MSG(ATC_BASIC_CMD_5250_112_2_17_22_12_35_339,"SMGD:%s doesnt exist")
TRACE_MSG(ATC_BASIC_CMD_5268_112_2_17_22_12_35_340,"ATC: run in the loop")
TRACE_MSG(ATC_BASIC_CMD_5272_112_2_17_22_12_35_341,"ATC: run the if")
TRACE_MSG(ATC_BASIC_CMD_5279_112_2_17_22_12_35_342,"ATC: run the else")
TRACE_MSG(ATC_BASIC_CMD_5327_112_2_17_22_12_36_343,"ATC: ATC_ProcessSMGD, param3 is not string!")
TRACE_MSG(ATC_BASIC_CMD_5343_112_2_17_22_12_36_344,"ATC: the parameters is error")
TRACE_MSG(ATC_BASIC_CMD_5381_112_2_17_22_12_36_345,"ATC: receive the midi file from SIO string lenght: %d")
TRACE_MSG(ATC_BASIC_CMD_5395_112_2_17_22_12_36_346,"ATC: write file error")
TRACE_MSG(ATC_BASIC_CMD_5397_112_2_17_22_12_36_347,"ATC: the added file size is %d")
TRACE_MSG(ATC_BASIC_CMD_5408_112_2_17_22_12_36_348,"ATC: open file error")
TRACE_MSG(ATC_BASIC_CMD_5418_112_2_17_22_12_36_349,"ATC: write file error")
TRACE_MSG(ATC_BASIC_CMD_5421_112_2_17_22_12_36_350,"ATC: the added file size is %d")
TRACE_MSG(ATC_BASIC_CMD_5429_112_2_17_22_12_36_351,"ATC: open file error")
TRACE_MSG(ATC_BASIC_CMD_5436_112_2_17_22_12_36_352,"ATC: cancel the given midi files")
TRACE_MSG(ATC_BASIC_CMD_5441_112_2_17_22_12_36_353,"ATC: the midi file format is error")
TRACE_MSG(ATC_BASIC_CMD_5505_112_2_17_22_12_36_354,"ATC: ProcessCFUN: not get the pending event")
TRACE_MSG(ATC_BASIC_CMD_5519_112_2_17_22_12_36_355,"ATC: ATC_ProcessCFUN: set, PARA1:%d")
TRACE_MSG(ATC_BASIC_CMD_5542_112_2_17_22_12_36_356,"ATC: ATC_ProcessCFUN: AT+CFUN=4 return S_ATC_SUCCESS when open macro _REF_SC8800H_MODEM_USB_ or TIANLONG_MOCOR")
TRACE_MSG(ATC_BASIC_CMD_5954_112_2_17_22_12_37_357,"ATC: pga_gain = %d, digital_gain = %d")
TRACE_MSG(ATC_BASIC_CMD_6096_112_2_17_22_12_37_358,"ATC: sadm mode_index = %d, dev_number = %d")
TRACE_MSG(ATC_BASIC_CMD_6134_112_2_17_22_12_37_359,"ATC: sadm mode_index = %d, dev_number = %d")
TRACE_MSG(ATC_BASIC_CMD_6233_112_2_17_22_12_37_360,"ATC: set dev mode, can't get valid mode type, mode name:%s.")
TRACE_MSG(ATC_BASIC_CMD_6241_112_2_17_22_12_37_361,"ATC: set dev mode result = %d")
TRACE_MSG(ATC_BASIC_CMD_6255_112_2_17_22_12_37_362,"ATC: set dev mode result = %d")
TRACE_MSG(ATC_BASIC_CMD_6269_112_2_17_22_12_37_363,"ATC: set aud dev param error.")
TRACE_MSG(ATC_BASIC_CMD_6332_112_2_17_22_12_38_364,"ATC: sadm mode_index = %d, dev_number = %d")
TRACE_MSG(ATC_BASIC_CMD_6357_112_2_17_22_12_38_365,"ATC: PROD_ReadAudioParamFromFlash failed.")
TRACE_MSG(ATC_BASIC_CMD_6387_112_2_17_22_12_38_366,"ATC: sadm mode_index = %d, dev_number = %d")
TRACE_MSG(ATC_BASIC_CMD_6415_112_2_17_22_12_38_367,"ATC: sadm mode_index = %d, dev_number = %d, %d")
TRACE_MSG(ATC_BASIC_CMD_6416_112_2_17_22_12_38_368,"ATC:atc_config_ptr->q_value=%d, 0x%x ")
TRACE_MSG(ATC_BASIC_CMD_6425_112_2_17_22_12_38_369,"AGC_Get_Param failed to get mode name, mode:%d.")
TRACE_MSG(ATC_BASIC_CMD_6430_112_2_17_22_12_38_370,"ATC:mode_name_ptr: %s, 0x%x, 0x%x")
TRACE_MSG(ATC_BASIC_CMD_6433_112_2_17_22_12_38_371,"AGC_Get_Param failed to get mode para.")
TRACE_MSG(ATC_BASIC_CMD_6439_112_2_17_22_12_38_372,"ATC:atc_config_ptr->q_value=%d, 0x%x, 0x%x, 0x%x")
TRACE_MSG(ATC_BASIC_CMD_6442_112_2_17_22_12_38_373,"ATC:atc_config_ptr->q_value=%d, 0x%x, 0x%x, 0x%x")
TRACE_MSG(ATC_BASIC_CMD_6452_112_2_17_22_12_38_374,"ATC:atc_config_ptr->q_value=%d, ")
TRACE_MSG(ATC_BASIC_CMD_6522_112_2_17_22_12_38_375,"ATC: sadm mode_index = %d, dev_number = %d")
TRACE_MSG(ATC_BASIC_CMD_6530_112_2_17_22_12_38_376,"AGC_Get_Param failed to get mode name, mode:%d.")
TRACE_MSG(ATC_BASIC_CMD_6533_112_2_17_22_12_38_377,"AGC_Get_Param to get mode name, mode:%d, %s.")
TRACE_MSG(ATC_BASIC_CMD_6536_112_2_17_22_12_38_378,"ATC: AUDIONVARM_ReadModeParamFromFlash failed.")
TRACE_MSG(ATC_BASIC_CMD_6639_112_2_17_22_12_38_379,"ATC: siosadm param_len = %d")
TRACE_MSG(ATC_BASIC_CMD_6672_112_2_17_22_12_38_380,"ATC: siosadm setmode param result = %d")
TRACE_MSG(ATC_BASIC_CMD_6679_112_2_17_22_12_38_381,"ATC: siosadm input data length error.")
TRACE_MSG(ATC_BASIC_CMD_6684_112_2_17_22_12_38_382,"Lost the escape character is %s is line %d")
TRACE_MSG(ATC_BASIC_CMD_6738_112_2_17_22_12_39_383,"ATC: siosadm for arm, param_len = %d")
TRACE_MSG(ATC_BASIC_CMD_6761_112_2_17_22_12_39_384,"ATC:ATC_ProcessSioARMSADMData %s, %s, %d.")
TRACE_MSG(ATC_BASIC_CMD_6767_112_2_17_22_12_39_385,"ATC:ATC_ProcessSioARMSADMData 2 %s, %s, %d.")
TRACE_MSG(ATC_BASIC_CMD_6774_112_2_17_22_12_39_386,"ATC: siosadm for arm, setmode param result = %d")
TRACE_MSG(ATC_BASIC_CMD_6785_112_2_17_22_12_39_387,"ATC: siosadm for arm , input data index error.")
TRACE_MSG(ATC_BASIC_CMD_6790_112_2_17_22_12_39_388,"ATC: siosadm for arm , input data length error.")
TRACE_MSG(ATC_BASIC_CMD_6795_112_2_17_22_12_39_389,"Lost the escape character is %s is line %d")
TRACE_MSG(ATC_BASIC_CMD_6851_112_2_17_22_12_39_390,"ATC:ATC_ProcessSioARMSADMNVData:length:%d,terminater_char:%d,mode index::%d")
TRACE_MSG(ATC_BASIC_CMD_6858_112_2_17_22_12_39_391,"ATC:cancel the snvm audio data!")
TRACE_MSG(ATC_BASIC_CMD_6875_112_2_17_22_12_39_392,"ATC:AUDIONVARM_WriteModeParamToFlash %s, %s, %d.")
TRACE_MSG(ATC_BASIC_CMD_6881_112_2_17_22_12_39_393,"ATC:AUDIONVARM_WriteModeParamToFlash 2 %s, %s, %d.")
TRACE_MSG(ATC_BASIC_CMD_6885_112_2_17_22_12_39_394,"ATC:ConvertHexToBin failed.")
TRACE_MSG(ATC_BASIC_CMD_6893_112_2_17_22_12_39_395,"ATC:AUDIONVARM_WriteModeParamToFlash failed. result:%d")
TRACE_MSG(ATC_BASIC_CMD_6900_112_2_17_22_12_39_396,"ATC: siosadm for arm , input data index error.")
TRACE_MSG(ATC_BASIC_CMD_6906_112_2_17_22_12_39_397,"ATC: siosadmnv for arm, input audio data length error.length:%d != %d")
TRACE_MSG(ATC_BASIC_CMD_6911_112_2_17_22_12_39_398,"ATC: snvm audio data format is error.")
TRACE_MSG(ATC_BASIC_CMD_6964_112_2_17_22_12_39_399,"ATC:ATC_ProcessSioSADMNVData:length:%d,terminater_char:%d,mode index::%d")
TRACE_MSG(ATC_BASIC_CMD_6971_112_2_17_22_12_39_400,"ATC:cancel the snvm audio data!")
TRACE_MSG(ATC_BASIC_CMD_6995_112_2_17_22_12_39_401,"ATC:ConvertHexToBin failed.")
TRACE_MSG(ATC_BASIC_CMD_7003_112_2_17_22_12_39_402,"ATC:PROD_WriteAudioParamToFlash failed. result:%d")
TRACE_MSG(ATC_BASIC_CMD_7011_112_2_17_22_12_39_403,"ATC: siosadmnv input audio data length error.length:%d != %d")
TRACE_MSG(ATC_BASIC_CMD_7016_112_2_17_22_12_39_404,"ATC: snvm audio data format is error.")
TRACE_MSG(ATC_BASIC_CMD_7158_112_2_17_22_12_39_405,"ATC_ProcessPinInd MNPHONE_ActivePs!~")
TRACE_MSG(ATC_BASIC_CMD_7171_112_2_17_22_12_39_406,"ATC_ProcessPinInd MNPHONE_ActivePs!~")
TRACE_MSG(ATC_BASIC_CMD_7250_112_2_17_22_12_40_407,"ATC:No sim reason = %d")
TRACE_MSG(ATC_BASIC_CMD_7447_112_2_17_22_12_40_408,"ATC: ATC_ProcessRssiInd,+CSQ rxlev=%d,rxqual=%d,csq_flag:%d")
TRACE_MSG(ATC_BASIC_CMD_7516_112_2_17_22_12_40_409,"ATC: ATC_ProcessRssiIndEx, +ECSQ old_rxlev=%d,rxlev=%d,rxqual=%d,esqopt thres:%d")
TRACE_MSG(ATC_BASIC_CMD_7643_112_2_17_22_12_40_410,"ATC: battery starts charge")
TRACE_MSG(ATC_BASIC_CMD_7669_112_2_17_22_12_40_411,"ATC: the battery is out of charge")
TRACE_MSG(ATC_BASIC_CMD_7964_112_2_17_22_12_41_412,"ATC:error MNPHONE_SelectBandEx")
TRACE_MSG(ATC_BASIC_CMD_8030_112_2_17_22_12_41_413,"ATC: unknow band %d")
TRACE_MSG(ATC_BASIC_CMD_8155_112_2_17_22_12_41_414,"ATC: nv item length = %d")
TRACE_MSG(ATC_BASIC_CMD_8168_112_2_17_22_12_41_415,"ATC: SNVM read nv item id = %d fail, err = %d")
TRACE_MSG(ATC_BASIC_CMD_8230_112_2_17_22_12_42_416,"ATC: nv_item_id_end < nv_item_id")
TRACE_MSG(ATC_BASIC_CMD_8242_112_2_17_22_12_42_417,"ATC: SNVM read nv item id = %d fail, err = %d")
TRACE_MSG(ATC_BASIC_CMD_8294_112_2_17_22_12_42_418,"ATC: write snvm data fail, err = %d.")
TRACE_MSG(ATC_BASIC_CMD_8299_112_2_17_22_12_42_419,"ATC: cancel the snvm data.")
TRACE_MSG(ATC_BASIC_CMD_8303_112_2_17_22_12_42_420,"ATC: snvm data format is error.")
TRACE_MSG(ATC_BASIC_CMD_8354_112_2_17_22_12_42_421,"ATC: AT+SPSNVM input nv item data length error.length:%d != %d")
TRACE_MSG(ATC_BASIC_CMD_8367_112_2_17_22_12_42_422,"ATC: cancel the snvm data.")
TRACE_MSG(ATC_BASIC_CMD_8371_112_2_17_22_12_42_423,"ATC: snvm data format is error.")
TRACE_MSG(ATC_BASIC_CMD_8418_112_2_17_22_12_42_424,"ATC:dest_Addr=%x,ch=%x")
TRACE_MSG(ATC_BASIC_CMD_8435_112_2_17_22_12_42_425,"ATC:enter ATC_ProcessReadDSPCnf")
TRACE_MSG(ATC_BASIC_CMD_8492_112_2_17_22_12_42_426,"ATC:AT+SRWD,ConvertHexToBin=FALSE")
TRACE_MSG(ATC_BASIC_CMD_8495_112_2_17_22_12_42_427,"ATC:AT+SRWD,value_or_source_address=%d")
TRACE_MSG(ATC_BASIC_CMD_8509_112_2_17_22_12_42_428,"ATC:AT+SRWD,value_or_source_address=%x,%x,%x")
TRACE_MSG(ATC_BASIC_CMD_8519_112_2_17_22_12_42_429,"ATC:flag = %d")
TRACE_MSG(ATC_BASIC_CMD_8546_112_2_17_22_12_42_430,"ATC:AT+SRWD,error in parameters")
TRACE_MSG(ATC_BASIC_CMD_8560_112_2_17_22_12_42_431,"ATC: ATC SRWD, param4 is not string!")
TRACE_MSG(ATC_BASIC_CMD_8567_112_2_17_22_12_42_432,"ATC:Length of string is error!")
TRACE_MSG(ATC_BASIC_CMD_8576_112_2_17_22_12_42_433,"ATC:AT+SRWD,ConvertInt16ToBin=FALSE")
TRACE_MSG(ATC_BASIC_CMD_8808_112_2_17_22_12_43_434,"ATC: need gpio direction parameter!")
TRACE_MSG(ATC_BASIC_CMD_8818_112_2_17_22_12_43_435,"ATC: set direction gpio id = %d, type = %d, value = %d")
TRACE_MSG(ATC_BASIC_CMD_8832_112_2_17_22_12_43_436,"ATC: Assert Failure receive gpio type = %d")
TRACE_MSG(ATC_BASIC_CMD_8891_112_2_17_22_12_43_437,"ATC: need gpio set parameter!")
TRACE_MSG(ATC_BASIC_CMD_8905_112_2_17_22_12_43_438,"ATC: gpio:%d not enabled!")
TRACE_MSG(ATC_BASIC_CMD_8911_112_2_17_22_12_43_439,"ATC: set value gpio id = %d type = %d, value = %d")
TRACE_MSG(ATC_BASIC_CMD_8919_112_2_17_22_12_43_440,"ATC: gpio %d set to input directiong, set value not permit.")
TRACE_MSG(ATC_BASIC_CMD_8929_112_2_17_22_12_43_441,"ATC: gpio %d set to input directiong, get value not permit.")
TRACE_MSG(ATC_BASIC_CMD_8938_112_2_17_22_12_43_442,"ATC: Assert Failure receive gpio type = %d")
TRACE_MSG(ATC_BASIC_CMD_9074_112_2_17_22_12_43_443,"ATC: $$$ the timer string length is error %d")
TRACE_MSG(ATC_BASIC_CMD_9085_112_2_17_22_12_43_444,"ATC: $$$ The timer string format error")
TRACE_MSG(ATC_BASIC_CMD_9094_112_2_17_22_12_43_445,"ATC: $$$ the value of year is error")
TRACE_MSG(ATC_BASIC_CMD_9105_112_2_17_22_12_43_446,"ATC: $$$ the month digital is error")
TRACE_MSG(ATC_BASIC_CMD_9111_112_2_17_22_12_43_447,"ATC: $$$ the month value is error")
TRACE_MSG(ATC_BASIC_CMD_9119_112_2_17_22_12_43_448,"ATC: $$$ the day value is error")
TRACE_MSG(ATC_BASIC_CMD_9127_112_2_17_22_12_43_449,"ATC: $$$ the hour value is error")
TRACE_MSG(ATC_BASIC_CMD_9135_112_2_17_22_12_43_450,"ATC: $$$ the minute value is error")
TRACE_MSG(ATC_BASIC_CMD_9144_112_2_17_22_12_43_451,"ATC: $$$ the second value is error")
TRACE_MSG(ATC_BASIC_CMD_9148_112_2_17_22_12_43_452,"ATC: $$$ the timer parameter is right")
TRACE_MSG(ATC_BASIC_CMD_9367_112_2_17_22_12_44_453,"ATC: Assert Failure unknow clck_code.")
TRACE_MSG(ATC_BASIC_CMD_9628_112_2_17_22_12_44_454,"ATC: ATC_ProcessERGA, busy")
TRACE_MSG(ATC_BASIC_CMD_9638_112_2_17_22_12_44_455,"ATC: ATC_ProcessERGA, result: %d")
TRACE_MSG(ATC_BASIC_CMD_9654_112_2_17_22_12_44_456,"ATC: ATC_ProcessERGA, param1 is invalid!")
TRACE_MSG(ATC_BASIC_CMD_9684_112_2_17_22_12_45_457,"ATC: ATC_ProcessERGA, busy")
TRACE_MSG(ATC_BASIC_CMD_9692_112_2_17_22_12_45_458,"ATC: ATC_ProcessERTCA, param1 or param2 is not string")
TRACE_MSG(ATC_BASIC_CMD_9705_112_2_17_22_12_45_459,"ATC: ATC_ProcessERTCA, result: %d")
TRACE_MSG(ATC_BASIC_CMD_9722_112_2_17_22_12_45_460,"ATC: ATC_ProcessERTCA, param1 or param2 is invalid")
TRACE_MSG(ATC_BASIC_CMD_9757_112_2_17_22_12_45_461,"ATC: ATC_Processxor_MBAU, param1 or param2 is not string")
TRACE_MSG(ATC_BASIC_CMD_9769_112_2_17_22_12_45_462,"ATC: ATC_Processxor_MBAU, result: %d")
TRACE_MSG(ATC_BASIC_CMD_9793_112_2_17_22_12_45_463,"ATC: ATC_Processxor_MBAU, result: %d")
TRACE_MSG(ATC_BASIC_CMD_9811_112_2_17_22_12_45_464,"ATC: ATC_Processxor_MBAU, param1 or param2 is invalid")
TRACE_MSG(ATC_BASIC_CMD_9876_112_2_17_22_12_45_465,"ATC: $$$ the status is %d")
TRACE_MSG(ATC_BASIC_CMD_9892_112_2_17_22_12_45_466,"ATC: ATC_ProcessSIMmbbmsAlgoCnf_Mbau, mbbms context:%d, Rsp:%d ")
TRACE_MSG(ATC_BASIC_CMD_9978_112_2_17_22_12_45_467,"ATC: ATC_ProcessSIMmbbmsAlgoCnf, mbbms context:%d, Rsp:%d ")
TRACE_MSG(ATC_BASIC_CMD_10100_112_2_17_22_12_45_468,"ATC: ATC_ProcessM.")
TRACE_MSG(ATC_BASIC_CMD_10168_112_2_17_22_12_46_469,"ATC: ATC_ProcessNVCOOPERATE, param1 or param2 is not numeric")
TRACE_MSG(ATC_BASIC_CMD_10328_112_2_17_22_12_46_470,"No any time info!")
TRACE_MSG(ATC_BASIC_CMD_10451_112_2_17_22_12_46_471,"ATC:ATC_ProcessCrsmCnf")
TRACE_MSG(ATC_BASIC_CMD_10454_112_2_17_22_12_46_472,"ATC:ATC_ProcessCrsmCnf,true")
TRACE_MSG(ATC_BASIC_CMD_10469_112_2_17_22_12_46_473,"ATC:ATC_ProcessCrsmCnf,FAIL")
TRACE_MSG(ATC_BASIC_CMD_10562_112_2_17_22_12_47_474,"ATC: get the CRSM parameters %d")
TRACE_MSG(ATC_BASIC_CMD_10592_112_2_17_22_12_47_475,"ATC:ATC_ProcessCRSM")
TRACE_MSG(ATC_BASIC_CMD_10619_112_2_17_22_12_47_476,"temp %s  %s")
TRACE_MSG(ATC_BASIC_CMD_10657_112_2_17_22_12_47_477,"ATC:ATC_ProcessCRSM  data len is too long")
TRACE_MSG(ATC_BASIC_CMD_10712_112_2_17_22_12_47_478,"binli add sim_result %d ")
TRACE_MSG(ATC_BASIC_CMD_10746_112_2_17_22_12_47_479,"status %d")
TRACE_MSG(ATC_BASIC_CMD_10852_112_2_17_22_12_47_480,"ATC: spenha eq mode_index = %d, dev_number = %d, %d")
TRACE_MSG(ATC_BASIC_CMD_10864_112_2_17_22_12_47_481,"AGC_Get_Param failed to get mode_index, mode_index :%d.")
TRACE_MSG(ATC_BASIC_CMD_10884_112_2_17_22_12_47_482,"ATC: spenha tun_eq mode_index = %d, dev_number = %d, %d")
TRACE_MSG(ATC_BASIC_CMD_10896_112_2_17_22_12_47_483,"AGC_Get_Param failed to get mode_index, mode_index :%d.")
TRACE_MSG(ATC_BASIC_CMD_10998_112_2_17_22_12_48_484,"ATC: spenha eq mode_index = %d, dev_number = %d, %d")
TRACE_MSG(ATC_BASIC_CMD_11009_112_2_17_22_12_48_485,"AGC_Get_Param failed to get mode_index, mode_index:%d.")
TRACE_MSG(ATC_BASIC_CMD_11027_112_2_17_22_12_48_486,"ATC: spenha tun_eq mode_index = %d, dev_number = %d, %d")
TRACE_MSG(ATC_BASIC_CMD_11038_112_2_17_22_12_48_487,"AGC_Get_Param failed to get mode_index, mode_index:%d.")
TRACE_MSG(ATC_BASIC_CMD_11158_112_2_17_22_12_48_488,"ATC: siospenha for enha, param_len = %d")
TRACE_MSG(ATC_BASIC_CMD_11181_112_2_17_22_12_48_489,"ATC: siospenha for enha, set result = %d")
TRACE_MSG(ATC_BASIC_CMD_11192_112_2_17_22_12_48_490,"ATC: siospenha for enha, input data index error.")
TRACE_MSG(ATC_BASIC_CMD_11197_112_2_17_22_12_48_491,"ATC: siospenha for enha, input data length error.")
TRACE_MSG(ATC_BASIC_CMD_11202_112_2_17_22_12_48_492,"Lost the escape character is %s is line %d")
TRACE_MSG(ATC_BASIC_CMD_11255_112_2_17_22_12_48_493,"ATC: siospenha for enha tun, param_len = %d")
TRACE_MSG(ATC_BASIC_CMD_11278_112_2_17_22_12_48_494,"ATC: siospenha for enha tun, set result = %d")
TRACE_MSG(ATC_BASIC_CMD_11289_112_2_17_22_12_48_495,"ATC: siospenha for enha tun, input data index error.")
TRACE_MSG(ATC_BASIC_CMD_11294_112_2_17_22_12_48_496,"ATC: siospenha for enha tun, input data length error.")
TRACE_MSG(ATC_BASIC_CMD_11299_112_2_17_22_12_48_497,"Lost the escape character is %s is line %d")
TRACE_MSG(ATC_BASIC_CMD_11336_112_2_17_22_12_48_498,"ATC:ATC_ProcessSNVM")
TRACE_MSG(ATC_BASIC_CMD_11371_112_2_17_22_12_48_499,"ATC: nv item length = %d")
TRACE_MSG(ATC_BASIC_CMD_11422_112_2_17_22_12_49_500,"ATC: ERR_INVALID_INDEX mode = %d")
TRACE_MSG(ATC_BASIC_CMD_11470_112_2_17_22_12_49_501,"ATC: siospenha for enha, param_len = %d")
TRACE_MSG(ATC_BASIC_CMD_11494_112_2_17_22_12_49_502,"ATC: siospenha for enha, set result = %d")
TRACE_MSG(ATC_BASIC_CMD_11505_112_2_17_22_12_49_503,"ATC: siospenha for enha, input data index error.")
TRACE_MSG(ATC_BASIC_CMD_11510_112_2_17_22_12_49_504,"ATC: siospenha for enha, input data length error.")
TRACE_MSG(ATC_BASIC_CMD_11515_112_2_17_22_12_49_505,"Lost the escape character is %s is line %d")
TRACE_MSG(ATC_BASIC_CMD_11569_112_2_17_22_12_49_506,"ATC: siospenha for enha tun, param_len = %d")
TRACE_MSG(ATC_BASIC_CMD_11593_112_2_17_22_12_49_507,"ATC: siospenha for enha tun, set result = %d")
TRACE_MSG(ATC_BASIC_CMD_11604_112_2_17_22_12_49_508,"ATC: siospenha for enha tun, input data index error.")
TRACE_MSG(ATC_BASIC_CMD_11609_112_2_17_22_12_49_509,"ATC: siospenha for enha tun, input data length error.")
TRACE_MSG(ATC_BASIC_CMD_11614_112_2_17_22_12_49_510,"Lost the escape character is %s is line %d")
TRACE_MSG(ATC_BASIC_CMD_11718_112_2_17_22_12_49_511,"ATC: call amount = %d, call_flag = %d")
TRACE_MSG(ATC_BASIC_CMD_11754_112_2_17_22_12_49_512,"record MUX Channel is %d")
TRACE_MSG(ATC_BASIC_CMD_11765_112_2_17_22_12_49_513,"s_record_mux_chl not build up")
TRACE_MSG(ATC_BASIC_CMD_11784_112_2_17_22_12_49_514,"[ATAUDIO] ATC_ProcessSPAUDRECORD")
TRACE_MSG(ATC_BASIC_CMD_11813_112_2_17_22_12_49_515,"[ATAUDIO] +AUDR :error")
TRACE_MSG(ATC_BASIC_CMD_11818_112_2_17_22_12_50_516,"[ATAUDIO] +AUDR :STOP UPLOAD DATA")
TRACE_MSG(ATC_BASIC_CMD_11832_112_2_17_22_12_50_517,"[ATAUDIO] +AUDR :CREATE HANDLE")
TRACE_MSG(ATC_BASIC_CMD_11839_112_2_17_22_12_50_518,"[ATAUDIO] +AUDR :CREATE HANDLE SUCCESS")
TRACE_MSG(ATC_BASIC_CMD_11854_112_2_17_22_12_50_519,"[ATAUDIO] +AUDR :error")
TRACE_MSG(ATC_BASIC_CMD_11885_112_2_17_22_12_50_520,"ATC:ATC_ProcessCTZR")
TRACE_MSG(ATC_COMMON_275_112_2_17_22_12_50_521,"The state is %d")
TRACE_MSG(ATC_COMMON_338_112_2_17_22_12_51_522,"Log ATC infomation:\n")
TRACE_MSG(ATC_COMMON_342_112_2_17_22_12_51_523,"AT Command Index : %d\n")
TRACE_MSG(ATC_COMMON_343_112_2_17_22_12_51_524,"AT Command Type :")
TRACE_MSG(ATC_COMMON_347_112_2_17_22_12_51_525," BASIC")
TRACE_MSG(ATC_COMMON_352_112_2_17_22_12_51_526," EXTEND")
TRACE_MSG(ATC_COMMON_357_112_2_17_22_12_51_527," EXECUTE")
TRACE_MSG(ATC_COMMON_362_112_2_17_22_12_51_528," SET")
TRACE_MSG(ATC_COMMON_367_112_2_17_22_12_51_529," READ")
TRACE_MSG(ATC_COMMON_372_112_2_17_22_12_51_530," TEST")
TRACE_MSG(ATC_COMMON_374_112_2_17_22_12_51_531,"\n")
TRACE_MSG(ATC_COMMON_376_112_2_17_22_12_51_532,"AT Command Parameter Number : %d\n")
TRACE_MSG(ATC_COMMON_388_112_2_17_22_12_51_533,"AT Command Parameter %d : %dn")
TRACE_MSG(ATC_COMMON_394_112_2_17_22_12_51_534,"AT Command Parameter %d : %sn")
TRACE_MSG(ATC_COMMON_398_112_2_17_22_12_51_535,"AT Command Parameter %d : Default\n")
TRACE_MSG(ATC_COMMON_421_112_2_17_22_12_51_536,"%s\n")
TRACE_MSG(ATC_COMMON_456_112_2_17_22_12_51_537,"ATC: Assert Failure bcd len.")
TRACE_MSG(ATC_COMMON_458_112_2_17_22_12_51_538,"ATC: the BCD CODE length is %d")
TRACE_MSG(ATC_COMMON_736_112_2_17_22_12_51_539," pin1_status: %d pin2_status: %d, phone_status:%d")
TRACE_MSG(ATC_COMMON_861_112_2_17_22_12_52_540,"ATC: get the password parameters %d")
TRACE_MSG(ATC_COMMON_899_112_2_17_22_12_52_541,"ATC:ATC_GetCommandParams,parsing,<data:> %s,<len:> %d")
TRACE_MSG(ATC_COMMON_943_112_2_17_22_12_52_542,"ATC: invalid string param, overstep param max length:%d")
TRACE_MSG(ATC_COMMON_958_112_2_17_22_12_52_543,"ATC: invalid string param")
TRACE_MSG(ATC_COMMON_982_112_2_17_22_12_52_544,"ATC: param overflow, max count of params is %d")
TRACE_MSG(ATC_COMMON_992_112_2_17_22_12_52_545,"ATC: invalid param, quotation mark wrong position, return position:%d")
TRACE_MSG(ATC_COMMON_999_112_2_17_22_12_52_546,"ATC: invalid param, overstep param max length , return position:%d")
TRACE_MSG(ATC_COMMON_1027_112_2_17_22_12_52_547,"ATC: ATC_GetCommandParams, success, offset:%d,other_cmd_flag:%d")
TRACE_MSG(ATC_COMMON_1035_112_2_17_22_12_52_548,"PARAM[%d]: | %d | %d | %s |")
TRACE_MSG(ATC_COMMON_1040_112_2_17_22_12_52_549,"ATC: ATC_GetCommandParams, failed!")
TRACE_MSG(ATC_COMMON_1066_112_2_17_22_12_52_550,"ATC: invalid param, numeric value overflow, return position:%d")
TRACE_MSG(ATC_COMMON_1073_112_2_17_22_12_52_551,"ATC: invalid param, not numeric, return position:%d")
TRACE_MSG(ATC_COMMON_1078_112_2_17_22_12_52_552,"ATC: get the number: %d")
TRACE_MSG(ATC_COMMON_1109_112_2_17_22_12_52_553,"ATC: Get the facility type %d")
TRACE_MSG(ATC_COMMON_1141_112_2_17_22_12_52_554,"ATC: Get the CLCK facility type %d")
TRACE_MSG(ATC_COMMON_1190_112_2_17_22_12_52_555,"ATC: Parse dtmf string %d")
TRACE_MSG(ATC_COMMON_1217_112_2_17_22_12_52_556,"ATC: check pin string %d")
TRACE_MSG(ATC_COMMON_1234_112_2_17_22_12_52_557,"ATC: the called number len: %d")
TRACE_MSG(ATC_COMMON_1284_112_2_17_22_12_53_558,"ATC: ATC_ConvertCalledNumToStr,Convert called number={%d,%d,%d}....")
TRACE_MSG(ATC_COMMON_1313_112_2_17_22_12_53_559,"ATC: ATC_ConvertCalledNumToStr,Get telephone type:%d")
TRACE_MSG(ATC_COMMON_1334_112_2_17_22_12_53_560,"ATC: ATC_ConvertCalledNumToStr,Get telephone num:%s")
TRACE_MSG(ATC_COMMON_1340_112_2_17_22_12_53_561,"ATC: ATC_ConvertCalledNumToStr,Get telephone num:%s,the length:%d")
TRACE_MSG(ATC_COMMON_1378_112_2_17_22_12_53_562,"ATC:High temp=%x")
TRACE_MSG(ATC_COMMON_1402_112_2_17_22_12_53_563,"ATC:low temp=%x")
TRACE_MSG(ATC_COMMON_1952_112_2_17_22_12_54_564,"ATC: the midi file length: %d")
TRACE_MSG(ATC_COMMON_1973_112_2_17_22_12_54_565,"ATC: allocate memory failed")
TRACE_MSG(ATC_COMMON_1982_112_2_17_22_12_54_566,"ATC: open file error")
TRACE_MSG(ATC_COMMON_1987_112_2_17_22_12_54_567,"ATC: The specific midi index isn't present")
TRACE_MSG(ATC_COMMON_2022_112_2_17_22_12_54_568,"ATC: Assert Failure rsp len.")
TRACE_MSG(ATC_COMMON_2196_112_2_17_22_12_54_569,"ATC: smgv check frame num error.")
TRACE_MSG(ATC_COMMON_2202_112_2_17_22_12_54_570,"ATC: smgv check frame data len error.")
TRACE_MSG(ATC_COMMON_2208_112_2_17_22_12_54_571,"ATC: smgv check crc error.")
TRACE_MSG(ATC_COMMON_2245_112_2_17_22_12_54_572,"ATC: GetIndexOfCallContext out of range, dual_sys = %d ,call_id = %d")
TRACE_MSG(ATC_COMMON_2448_112_2_17_22_12_55_573,"ATC: not GPRS neither TD rat.")
TRACE_MSG(ATC_COMMON_2475_112_2_17_22_12_55_574,"ATC: not GPRS neither TD rat.")
TRACE_MSG(ATC_COMMON_2769_112_2_17_22_12_56_575,"ATC:transform chset IRA,WARNING,invalid string length. src_str_length:%d,dest_str_max_len:%d")
TRACE_MSG(ATC_COMMON_2780_112_2_17_22_12_56_576,"ATC:transform chset IRA=>IRA,WARNING,dest string buffer overflow.")
TRACE_MSG(ATC_COMMON_2784_112_2_17_22_12_56_577,"ATC:originator chset and destination chset are both IRA.")
TRACE_MSG(ATC_COMMON_2808_112_2_17_22_12_56_578,"ATC:transform chset IRA,GSM string length:%d,sevent bit offset:%d.")
TRACE_MSG(ATC_COMMON_2815_112_2_17_22_12_56_579,"ATC:transform chset IRA=>GSM,WARNING,dest string buffer overflow.")
TRACE_MSG(ATC_COMMON_2828_112_2_17_22_12_56_580,"ATC:transform chset IRA=>HEX,WARNING,dest string buffer overflow.")
TRACE_MSG(ATC_COMMON_2841_112_2_17_22_12_56_581,"ATC:transform chset IRA=>UCS2,WARNING,dest string buffer overflow.")
TRACE_MSG(ATC_COMMON_2871_112_2_17_22_12_56_582,"ATC:transform chset IRA => %s success. string length %d=>%d")
TRACE_MSG(ATC_COMMON_2875_112_2_17_22_12_56_583,"ATC:transform chset IRA => %s failed.")
TRACE_MSG(ATC_COMMON_2912_112_2_17_22_12_56_584,"ATC:transform chset GSM,WARNING,invalid string length.src_str_length:%d,dest_str_max_len:%d")
TRACE_MSG(ATC_COMMON_2922_112_2_17_22_12_56_585,"ATC:transform chset GSM=>GSM,WARNING,dest string buffer overflow.")
TRACE_MSG(ATC_COMMON_2926_112_2_17_22_12_56_586,"ATC:originator chset and destination chset are both GSM.")
TRACE_MSG(ATC_COMMON_2943_112_2_17_22_12_56_587,"ATC:transform chset first step GSM(hex=>bin) success, gsm string len:%d.")
TRACE_MSG(ATC_COMMON_2958_112_2_17_22_12_56_588,"ATC:transform chset second step GSM(bin)=>IRA success, IRA string length:%d.")
TRACE_MSG(ATC_COMMON_2993_112_2_17_22_12_56_589,"ATC:transform chset GSM => %s success. string length %d=>%d")
TRACE_MSG(ATC_COMMON_2997_112_2_17_22_12_56_590,"ATC:transform chset GSM => %s failed.")
TRACE_MSG(ATC_COMMON_3030_112_2_17_22_12_56_591,"ATC:transform chset HEX,WARNING,invalid string length.src_str_length:%d,dest_str_max_len:%d")
TRACE_MSG(ATC_COMMON_3040_112_2_17_22_12_56_592,"ATC:transform chset HEX=>HEX,WARNING,dest string buffer overflow.")
TRACE_MSG(ATC_COMMON_3044_112_2_17_22_12_56_593,"ATC:originator chset and destination chset are both HEX.")
TRACE_MSG(ATC_COMMON_3065_112_2_17_22_12_57_594,"ATC:transform chset first step HEX=>IRA success, ira string len:%d.")
TRACE_MSG(ATC_COMMON_3087_112_2_17_22_12_57_595,"ATC:transform chset HEX=>IRA,WARNING,dest string buffer overflow.")
TRACE_MSG(ATC_COMMON_3098_112_2_17_22_12_57_596,"ATC: ATC_ConvertHEXToUCS2 failed.")
TRACE_MSG(ATC_COMMON_3120_112_2_17_22_12_57_597,"ATC:transform chset HEX => %s success. string length %d=>%d")
TRACE_MSG(ATC_COMMON_3124_112_2_17_22_12_57_598,"ATC:transform chset HEX => %s failed.")
TRACE_MSG(ATC_COMMON_3156_112_2_17_22_12_57_599,"ATC:transform chset UCS2,WARNING,invalid string length.")
TRACE_MSG(ATC_COMMON_3166_112_2_17_22_12_57_600,"ATC:transform chset UCS2=>UCS2,WARNING,dest string buffer overflow.")
TRACE_MSG(ATC_COMMON_3170_112_2_17_22_12_57_601,"ATC:originator chset and destination chset are both UCS2.")
TRACE_MSG(ATC_COMMON_3178_112_2_17_22_12_57_602,"ATC:ATC_ConvertUCS2ToIRA failed.")
TRACE_MSG(ATC_COMMON_3192_112_2_17_22_12_57_603,"ATC:ATC_ConvertUCS2ToIRA failed.")
TRACE_MSG(ATC_COMMON_3196_112_2_17_22_12_57_604,"ATC:transform chset first step UCS2=>IRA success, ira string len:%d.")
TRACE_MSG(ATC_COMMON_3214_112_2_17_22_12_57_605,"ATC:ATC_ConvertUCS2ToHEX failed.")
TRACE_MSG(ATC_COMMON_3236_112_2_17_22_12_57_606,"ATC:transform chset UCS2 => %s success. string length %d=>%d")
TRACE_MSG(ATC_COMMON_3240_112_2_17_22_12_57_607,"ATC:transform chset UCS2 => %s failed.")
TRACE_MSG(ATC_COMMON_3280_112_2_17_22_12_57_608,"ATC:transform chset %s => %s not support yet.")
TRACE_MSG(ATC_ENG_340_112_2_17_22_12_58_609,"ATC: service_type = %d, ul_rate = %d,dl_rate")
TRACE_MSG(ATC_ENG_446_112_2_17_22_12_58_610,"WARNING: set_real_nw_opt_enable_flag is unsupported!")
TRACE_MSG(ATC_ENG_814_112_2_17_22_12_59_611,"aucPlmn[0]=%x,aucPlmn[1]=%x,aucPlmn[2]=%x")
TRACE_MSG(ATC_ENG_821_112_2_17_22_12_59_612,"atc write uplmn wact file error\n")
TRACE_MSG(ATC_ENG_847_112_2_17_22_12_59_613,"at uplmn num=%d\n")
TRACE_MSG(ATC_ENG_879_112_2_17_22_12_59_614,"the uplmn id =%s\n")
TRACE_MSG(ATC_ENG_981_112_2_17_22_12_59_615," the %d freq have %d cells\n")
TRACE_MSG(ATC_ENG_984_112_2_17_22_12_59_616," total freq point =%d\n")
TRACE_MSG(ATC_ENG_1046_112_2_17_22_12_59_617,"ATENG:ATC_Processxor_SYSINFO\n")
TRACE_MSG(ATC_ENG_1073_112_2_17_22_12_59_618,"ATENG:ATC_ProcessSPDSP\n")
TRACE_MSG(ATC_ENG_1201_112_2_17_22_13_0_619,"ATC: ^HVER: %s")
TRACE_MSG(ATC_ENG_1395_112_2_17_22_13_0_620,"ATC:Set rat:%d, dm_prefer_rat:%d")
TRACE_MSG(ATC_ENG_1533_112_2_17_22_13_0_621,"ATC:Set rat:%d, dm_prefer_rat:%d")
TRACE_MSG(ATC_ENG_1948_112_2_17_22_13_1_622,"ATC: get the CRSM parameters %d")
TRACE_MSG(ATC_ENG_1969_112_2_17_22_13_1_623,"ATC:ATC_ProcessCRSM")
TRACE_MSG(ATC_ENG_1999_112_2_17_22_13_1_624,"ATC:ATC_ProcessCRSM  data len is too long")
TRACE_MSG(ATC_ENG_2105_112_2_17_22_13_1_625,"ATC:ATC_ProcessCrsmCnf")
TRACE_MSG(ATC_ENG_2108_112_2_17_22_13_1_626,"ATC:ATC_ProcessCrsmCnf,true")
TRACE_MSG(ATC_ENG_2123_112_2_17_22_13_1_627,"ATC:ATC_ProcessCrsmCnf,FAIL")
TRACE_MSG(ATC_ENG_2198_112_2_17_22_13_2_628,"ATC: ECMMB, NO param1 !")
TRACE_MSG(ATC_ENG_2203_112_2_17_22_13_2_629,"ATC:ECMMB, set command, oper=%d")
TRACE_MSG(ATC_ENG_2815_112_2_17_22_13_3_630,"ATC: Invalid param1, dummy gsm cells' num is %d")
TRACE_MSG(ATC_ENG_2837_112_2_17_22_13_3_631,"ATC: Invalid param2")
TRACE_MSG(ATC_ENG_2849_112_2_17_22_13_3_632,"ATC: Invalid param3")
TRACE_MSG(ATC_ENG_2860_112_2_17_22_13_3_633,"ATC: Invalid param4")
TRACE_MSG(ATC_ENG_2873_112_2_17_22_13_3_634,"ATC: wrong dummy gsm cell num, %d == %d, Max:%d")
TRACE_MSG(ATC_ENG_3187_112_2_17_22_13_3_635,"ATC: ATC_ProcessAutoTestInfoFromMMI,ERROR,wrong string length:%d")
TRACE_MSG(ATC_ENG_3230_112_2_17_22_13_4_636,"ATC: SIMULATOR KPD: generate down key, key code is 0x%x")
TRACE_MSG(ATC_ENG_3238_112_2_17_22_13_4_637,"ATC: SIMULATOR KPD: generate up key, key code is 0x%x")
TRACE_MSG(ATC_ENG_3252_112_2_17_22_13_4_638,"ATC: SIMULATOR KPD: generate down and up key, key code is 0x%x")
TRACE_MSG(ATC_ENG_3486_112_2_17_22_13_4_639,"ATC: SPEQPARA: <n> is invalid.")
TRACE_MSG(ATC_ENG_3501_112_2_17_22_13_4_640,"ATC: SPEQPARA: <type> or <dest> is invalid.")
TRACE_MSG(ATC_ENG_3519_112_2_17_22_13_4_641,"ATC: SPEQPARA: Get EQ PARA failed.")
TRACE_MSG(ATC_ENG_3539_112_2_17_22_13_4_642,"ATC: SPEQPARA: <type> or <dest> or <data> is invalid.")
TRACE_MSG(ATC_ENG_3546_112_2_17_22_13_4_643,"ATC: SPEQPARA: <data> type is not string.")
TRACE_MSG(ATC_ENG_3552_112_2_17_22_13_4_644,"ATC: SPEQPARA: <data> length:%d != %d*2.")
TRACE_MSG(ATC_ENG_3561_112_2_17_22_13_4_645,"ATC: SPEQPARA: ConvertHexToBin failed.")
TRACE_MSG(ATC_ENG_3582_112_2_17_22_13_4_646,"ATC: SPEQPARA: Set EQ PARA failed.")
TRACE_MSG(ATC_ENG_4081_112_2_17_22_13_5_647,"test spinterrat!")
TRACE_MSG(ATC_ENG_4175_112_2_17_22_13_5_648,"GetAdcCalibrateInfoStr %x")
TRACE_MSG(ATC_ENG_4178_112_2_17_22_13_5_649,"ATC: SPCALIBRATE.")
TRACE_MSG(ATC_ENG_4312_112_2_17_22_13_6_650,"test : %s %s ")
TRACE_MSG(ATC_ENG_4418_112_2_17_22_13_6_651,"test : %s %s ")
TRACE_MSG(ATC_ENG_4504_112_2_17_22_13_6_652,"STEVE nvm_band_info.no_range = %d")
TRACE_MSG(ATC_ENG_4719_112_2_17_22_13_7_653,"ATC: ATC_ProcessSPAUTOPOWER.")
TRACE_MSG(ATC_ENG_4727_112_2_17_22_13_7_654,"ATC: auto_poweron_flag = %d")
TRACE_MSG(ATC_ENG_4756_112_2_17_22_13_7_655,"ATC:SPENGMD params1-3 less")
TRACE_MSG(ATC_ENG_4766_112_2_17_22_13_7_656,"ATC:SPENGMD param1-3 is error")
TRACE_MSG(ATC_ENG_4805_112_2_17_22_13_7_657,"ATC: SPENGMD: mainType=%d, subType=%d")
TRACE_MSG(ATC_ENG_4808_112_2_17_22_13_7_658,"ATC:SPENGMD param check fail")
TRACE_MSG(ATC_ENG_4835_112_2_17_22_13_7_659,"ATC: SPENGMD   item_count = %d")
TRACE_MSG(ATC_ENG_4842_112_2_17_22_13_7_660,"ATC: SPENGMD   item.arr_count = %d")
TRACE_MSG(ATC_ENG_4879_112_2_17_22_13_7_661,"ATC: SPENGMD set param check index fail")
TRACE_MSG(ATC_ENG_4886_112_2_17_22_13_7_662,"ATC: SPENGMD set_fptr == null")
TRACE_MSG(ATC_ENG_4894_112_2_17_22_13_7_663,"ATC: SPENGMD set l1 info param")
TRACE_MSG(ATC_ENG_4935_112_2_17_22_13_7_664,"ATC: SPENGMD set l1 info param %d %d %d %d")
TRACE_MSG(ATC_ENG_4953_112_2_17_22_13_7_665,"ATC: SPENGMD set mem deinit assert param")
TRACE_MSG(ATC_ENG_4966_112_2_17_22_13_7_666,"ATC: SPENGMD set save smebuffer udisc")
TRACE_MSG(ATC_ENG_5001_112_2_17_22_13_7_667,"ATC:SPENGMD check paramters %d ,mainType=%d, subType=%d")
TRACE_MSG(ATC_GPRS_MODEM_197_112_2_17_22_13_8_668,"ATC: ATC_ProcessCPBW, param3 is not string!")
TRACE_MSG(ATC_GPRS_MODEM_283_112_2_17_22_13_8_669,"ATGPRS: ATC_ProcessSDATASTART\n")
TRACE_MSG(ATC_GPRS_MODEM_435_112_2_17_22_13_9_670,"ATGPRS: ATC_ProcessSDATASEND\n")
TRACE_MSG(ATC_GPRS_MODEM_464_112_2_17_22_13_9_671,"ATC: ATC_ProcessSDATASEND, param2 is not num!")
TRACE_MSG(ATC_GPRS_MODEM_501_112_2_17_22_13_9_672,"ATGPRS:set atc Z gprs end char")
TRACE_MSG(ATC_GPRS_MODEM_545_112_2_17_22_13_9_673,"ATC: ATC_ProcessSSTRSEND, param1, param2 is need!")
TRACE_MSG(ATC_GPRS_MODEM_558_112_2_17_22_13_9_674,"ATC: ATC_ProcessSSTRSEND, param2 is not string!")
TRACE_MSG(ATC_GPRS_MODEM_820_112_2_17_22_13_9_675,"ATGPRS:Send PDU data:")
TRACE_MSG(ATC_GPRS_MODEM_823_112_2_17_22_13_9_676," %02x")
TRACE_MSG(ATC_GPRS_MODEM_869_112_2_17_22_13_9_677,"ATC: Assert Failure data_len.")
TRACE_MSG(ATC_GPRS_MODEM_893_112_2_17_22_13_10_678,"AT MODEM:update udp fport as %04x")
TRACE_MSG(ATC_GPRS_MODEM_1120_112_2_17_22_13_10_679,"ATC: close tcp upcall.")
TRACE_MSG(ATC_GPRS_MODEM_1218_112_2_17_22_13_10_680,"ATGPRS:+tcp Send Failed!")
TRACE_MSG(ATC_GPRS_MODEM_1236_112_2_17_22_13_10_681,"ATGPRS:+udp Send Failed!")
TRACE_MSG(ATC_GPRS_MODEM_1315_112_2_17_22_13_10_682,"modem:read tcp len =%d")
TRACE_MSG(ATC_GPRS_MODEM_1339_112_2_17_22_13_10_683,"ATC: close tcp upcall.")
TRACE_MSG(ATC_GPRS_MODEM_1370_112_2_17_22_13_11_684,"ATC: enter tcp rcv callback!")
TRACE_MSG(ATC_GPRS_MODEM_1380_112_2_17_22_13_11_685,"ATC: send stcpd notification!")
TRACE_MSG(ATC_GPRS_MODEM_1391_112_2_17_22_13_11_686,"ATC: socket closed notification!")
TRACE_MSG(ATC_GPRS_MODEM_1405_112_2_17_22_13_11_687,"ATC: so error index, so =%d")
TRACE_MSG(ATC_GSM_991_112_2_17_22_13_13_688,"ATC: ATC_ProcessA: there is an active call, not allowed another!")
TRACE_MSG(ATC_GSM_997_112_2_17_22_13_13_689,"ATC: ATC_ProcessA, no waiting call!")
TRACE_MSG(ATC_GSM_1008_112_2_17_22_13_13_690,"ATC: ATC_ProcessA, invalid call index!")
TRACE_MSG(ATC_GSM_1028_112_2_17_22_13_13_691,"ATC: ATC_ProcessA, MNCALL_ConnectCall failure!")
TRACE_MSG(ATC_GSM_1484_112_2_17_22_13_14_692,"ATC: this is a emergency call!")
TRACE_MSG(ATC_GSM_1539_112_2_17_22_13_14_693,"ATC: this is a voice call!")
TRACE_MSG(ATC_GSM_1544_112_2_17_22_13_14_694,"ATC: this is a data call!")
TRACE_MSG(ATC_GSM_1659_112_2_17_22_13_14_695,"ATC:ATC_ProcessD atc_config_ptr->call_link_id = %d ")
TRACE_MSG(ATC_GSM_1720_112_2_17_22_13_14_696,"ATC: Assert Failure call_amount error %d.")
TRACE_MSG(ATC_GSM_1729_112_2_17_22_13_14_697,"ATC: ATC_ProcessCallStartInd call_type:%d, call_amount: %d")
TRACE_MSG(ATC_GSM_1961_112_2_17_22_13_15_698,"ATC: ProcessGprs99Dial, the cid:%d is not allowed.")
TRACE_MSG(ATC_GSM_1969_112_2_17_22_13_15_699,"ATC: set dial apn ...")
TRACE_MSG(ATC_GSM_1986_112_2_17_22_13_15_700,"ATC: ProcessGprs99Dial, this pdp id is being deactivated.")
TRACE_MSG(ATC_GSM_2081_112_2_17_22_13_15_701,"ATC: Gprs98Dial, the cid:%d is not allowed.")
TRACE_MSG(ATC_GSM_2097_112_2_17_22_13_15_702,"ATC: Gprs98Dial, this pdp id is being deactivated.")
TRACE_MSG(ATC_GSM_2141_112_2_17_22_13_15_703,"ATC: ATC_ProcessGprsDial, 99 modem, cid=%d")
TRACE_MSG(ATC_GSM_2164_112_2_17_22_13_15_704,"ATC: ATC_ProcessGprsDial, 99 failure")
TRACE_MSG(ATC_GSM_2180_112_2_17_22_13_15_705,"ATC: ATC_ProcessGprsDial, 98 modem, cid=%d")
TRACE_MSG(ATC_GSM_2203_112_2_17_22_13_15_706,"ATC: ATC_ProcessGprsDial, 98 failure")
TRACE_MSG(ATC_GSM_2271_112_2_17_22_13_16_707,"ATC: StartDTMFKey, call_id >= MN_CALL_MAX_CALL_NUM")
TRACE_MSG(ATC_GSM_2278_112_2_17_22_13_16_708,"ATC: MNCALL_StartDTMF, call id:%d dtmf key:%c")
TRACE_MSG(ATC_GSM_2285_112_2_17_22_13_16_709,"ATC: MNCALL_StartDTMF failed.")
TRACE_MSG(ATC_GSM_2302_112_2_17_22_13_16_710,"ATC: StopDTMFKey, call_id >= MN_CALL_MAX_CALL_NUM")
TRACE_MSG(ATC_GSM_2309_112_2_17_22_13_16_711,"ATC: MNCALL_StopDTMF, call id:%d.")
TRACE_MSG(ATC_GSM_2316_112_2_17_22_13_16_712,"ATC: MNCALL_StopDTMF failed.")
TRACE_MSG(ATC_GSM_2338_112_2_17_22_13_16_713,"ATC: GetDTMFKey, <dtmf> is not character or not in range.")
TRACE_MSG(ATC_GSM_2345_112_2_17_22_13_16_714,"ATC: GetDTMFKey, %d.")
TRACE_MSG(ATC_GSM_2378_112_2_17_22_13_16_715,"ATC: <DTMF> string format wrong.")
TRACE_MSG(ATC_GSM_2395_112_2_17_22_13_16_716,"ATC: GetDTMFString, %s.")
TRACE_MSG(ATC_GSM_2476_112_2_17_22_13_16_717,"ATC: enter the DTMF timer expire")
TRACE_MSG(ATC_GSM_2497_112_2_17_22_13_16_718,"ATC: ATC_ProcessDTMFTimerExpInd")
TRACE_MSG(ATC_GSM_2500_112_2_17_22_13_16_719,"ATC: no dtmf key to stop.")
TRACE_MSG(ATC_GSM_2530_112_2_17_22_13_16_720,"ATC:ATC_ProcessStartDTMFCnf,callid:%d == %d,DTMF key:%c, AT cmd type:%d")
TRACE_MSG(ATC_GSM_2578_112_2_17_22_13_16_721,"ATC: DTMF SIG WRONG.")
TRACE_MSG(ATC_GSM_2584_112_2_17_22_13_16_722,"ATC: DTMF TIMER WRONG.")
TRACE_MSG(ATC_GSM_2624_112_2_17_22_13_16_723,"ATC:ATC_ProcessStopDTMFCnf,callid:%d == %d,AT cmd type:%d")
TRACE_MSG(ATC_GSM_2710_112_2_17_22_13_16_724,"ATC: VTS: no active call")
TRACE_MSG(ATC_GSM_2716_112_2_17_22_13_16_725,"ATC: VTS: no active call found")
TRACE_MSG(ATC_GSM_2720_112_2_17_22_13_16_726,"ATC: VTS: call-id:%d")
TRACE_MSG(ATC_GSM_2726_112_2_17_22_13_16_727,"ATC: VTS: the mode of the call is not voice")
TRACE_MSG(ATC_GSM_2733_112_2_17_22_13_16_728,"ATC: VTS: the previous dtmf keys have not done.")
TRACE_MSG(ATC_GSM_2764_112_2_17_22_13_17_729,"ATC: VTS: <duration> param error. not numeric.")
TRACE_MSG(ATC_GSM_2775_112_2_17_22_13_17_730,"ATC: VTS: <duration> not in range (0-255).")
TRACE_MSG(ATC_GSM_2792_112_2_17_22_13_17_731,"ATC: ATC_ProcessVTS: too long dtmf string.")
TRACE_MSG(ATC_GSM_2820_112_2_17_22_13_17_732,"ATC: VTS: <DTMF> param set error.")
TRACE_MSG(ATC_GSM_2826_112_2_17_22_13_17_733,"ATC: VTS: params not supported.")
TRACE_MSG(ATC_GSM_2838_112_2_17_22_13_17_734,"ATC: send dtmf:%s,duration:%d*100ms")
TRACE_MSG(ATC_GSM_2848_112_2_17_22_13_17_735,"ATC: VTS, MNCALL_StartDTMF failed.")
TRACE_MSG(ATC_GSM_2944_112_2_17_22_13_17_736,"ATC: EVTS: no active call")
TRACE_MSG(ATC_GSM_2951_112_2_17_22_13_17_737,"ATC: EVTS: no active call found")
TRACE_MSG(ATC_GSM_2959_112_2_17_22_13_17_738,"ATC: EVTS: the call:%d is not voice mode")
TRACE_MSG(ATC_GSM_2964_112_2_17_22_13_17_739,"ATC:EVTS: call-id:%d")
TRACE_MSG(ATC_GSM_2968_112_2_17_22_13_17_740,"ATC: EVTS: AT+VTS has not done.")
TRACE_MSG(ATC_GSM_2987_112_2_17_22_13_17_741,"ATC: EVTS: <mode> param error. not numeric.")
TRACE_MSG(ATC_GSM_2993_112_2_17_22_13_17_742,"ATC: EVTS: <mode> set error")
TRACE_MSG(ATC_GSM_3025_112_2_17_22_13_17_743,"ATC: StopDTMF = %c")
TRACE_MSG(ATC_GSM_3029_112_2_17_22_13_17_744,"ATC: ATC_ProcessEVTS: MNCALL_StopDTMF failed.")
TRACE_MSG(ATC_GSM_3035_112_2_17_22_13_17_745,"ATC: WARN:%d,%c")
TRACE_MSG(ATC_GSM_3060_112_2_17_22_13_17_746,"ATC: ATC_ProcessEVTS: DTMF is already started, pls stop firstly.")
TRACE_MSG(ATC_GSM_3066_112_2_17_22_13_17_747,"ATC: ATC_ProcessEVTS: <mode> set error")
TRACE_MSG(ATC_GSM_3141_112_2_17_22_13_17_748,"ATC: ATC_ProcessCOPS, not allowed when CALL is existed.")
TRACE_MSG(ATC_GSM_3154_112_2_17_22_13_17_749,"ATC: ATC_ProcessCOPS, sim is not ready or cpin code is not ready.")
TRACE_MSG(ATC_GSM_3169_112_2_17_22_13_17_750,"ATC: ATC_ProcessCOPS, APP_MN_PLMN_SELECT_CNF is being handled.")
TRACE_MSG(ATC_GSM_3191_112_2_17_22_13_17_751,"ATC: MNPHONE_SelectPLMN failure")
TRACE_MSG(ATC_GSM_3350_112_2_17_22_13_18_752,"ATC: RAT value is wrong")
TRACE_MSG(ATC_GSM_3422_112_2_17_22_13_18_753,"ATC: Assert Failure plmn_status.")
TRACE_MSG(ATC_GSM_3432_112_2_17_22_13_18_754,"ATC: search the plmn list")
TRACE_MSG(ATC_GSM_3451_112_2_17_22_13_18_755,"ATC: ATC_ProcessCOPS, failure in MNPHONE_ListPLMN.")
TRACE_MSG(ATC_GSM_3499_112_2_17_22_13_18_756,"ATC: plmn list type is invaild")
TRACE_MSG(ATC_GSM_3571_112_2_17_22_13_18_757,"ATC: MNPHONE_WritePLMNtoSim failure.")
TRACE_MSG(ATC_GSM_3605_112_2_17_22_13_18_758,"ATC:CPOL FULL_NAME not found!")
TRACE_MSG(ATC_GSM_3617_112_2_17_22_13_18_759,"ATC:CPOL SHORT_NAME not found!")
TRACE_MSG(ATC_GSM_3633_112_2_17_22_13_18_760,"ATC:CPOL not support this plmn")
TRACE_MSG(ATC_GSM_3638_112_2_17_22_13_18_761,"ATC:cpol plmn = %d")
TRACE_MSG(ATC_GSM_3654_112_2_17_22_13_18_762,"ATC: cpol not support this plmn")
TRACE_MSG(ATC_GSM_3670_112_2_17_22_13_18_763,"ATC: cpol not support to set gsm_compact_act ")
TRACE_MSG(ATC_GSM_3686_112_2_17_22_13_18_764,"ATC: MNPHONE_WritePLMNtoSim failure")
TRACE_MSG(ATC_GSM_3696_112_2_17_22_13_18_765,"ATC: reading the plmn list")
TRACE_MSG(ATC_GSM_3706_112_2_17_22_13_18_766,"ATC: failure in MNPHONE_ReadPLMNfromSim")
TRACE_MSG(ATC_GSM_3730_112_2_17_22_13_18_767,"ATC: failure in MNPHONE_ReadPLMNfromSim")
TRACE_MSG(ATC_GSM_3878_112_2_17_22_13_19_768,"ATC: enter ATC_ProcessSSMP")
TRACE_MSG(ATC_GSM_3911_112_2_17_22_13_19_769,"ATC: read plmn_num = %d,is_valid_result = %d")
TRACE_MSG(ATC_GSM_3953_112_2_17_22_13_19_770,"atc: is_valid_plmn[%d]=%d, is_gsm_act[%d]=%dn")
TRACE_MSG(ATC_GSM_3955_112_2_17_22_13_19_771,"atc: is_gsm_compact_act[%d]=%d,is_td_act[%d]=%dn")
TRACE_MSG(ATC_GSM_3957_112_2_17_22_13_19_772,"atc: plmn_list=%s\n")
TRACE_MSG(ATC_GSM_4026_112_2_17_22_13_19_773,"ATC: write plmn confirm ")
TRACE_MSG(ATC_GSM_4060_112_2_17_22_13_19_774,"ATC: received the plmn list confirm ")
TRACE_MSG(ATC_GSM_4061_112_2_17_22_13_19_775,"ATC: %d,%d")
TRACE_MSG(ATC_GSM_4096_112_2_17_22_13_19_776,"ACT: RAT value is wrong")
TRACE_MSG(ATC_GSM_4217_112_2_17_22_13_19_777,"ATC: Assert Failure unknow plmn_status.")
TRACE_MSG(ATC_GSM_4226_112_2_17_22_13_19_778,"ATC: PlmnSelCnf sig_ptr->cause: 0x%x")
TRACE_MSG(ATC_GSM_4243_112_2_17_22_13_19_779,"ATC: ATC_ProcessPlmnSelCnf, is_plmn_success:FALSE, MANUAL_MATO_MODE")
TRACE_MSG(ATC_GSM_4306_112_2_17_22_13_20_780,"ATC: ManualSelectPlmn, FULL_NAME, not found!")
TRACE_MSG(ATC_GSM_4318_112_2_17_22_13_20_781,"ATC: ManualSelectPlmn, SHORT_NAME, not found!")
TRACE_MSG(ATC_GSM_4329_112_2_17_22_13_20_782,"ATC: ManualSelectPlmn, NUMBERIC, cops not support this plmn")
TRACE_MSG(ATC_GSM_4339_112_2_17_22_13_20_783,"ATC: ManualSelectPlmn, plmn = %d")
TRACE_MSG(ATC_GSM_4355_112_2_17_22_13_20_784,"ATC: ManualSelectPlmn, cops not support this plmn")
TRACE_MSG(ATC_GSM_4362_112_2_17_22_13_20_785,"ATC: ManualSelectPlmn, failure in MN")
TRACE_MSG(ATC_GSM_4403_112_2_17_22_13_20_786,"ATC: link_id:%d,g_calls_state.call_amout= %d")
TRACE_MSG(ATC_GSM_4418_112_2_17_22_13_20_787,"ATC: ATC_ProcessH, cause_value=%d")
TRACE_MSG(ATC_GSM_4462_112_2_17_22_13_20_788,"ATC: Assert Failure call_amount %d.")
TRACE_MSG(ATC_GSM_4515_112_2_17_22_13_20_789,"ATC: the call number len: %d")
TRACE_MSG(ATC_GSM_4619_112_2_17_22_13_20_790,"ATC: the call number len: %d")
TRACE_MSG(ATC_GSM_4627_112_2_17_22_13_20_791,"ATC: no call number present!")
TRACE_MSG(ATC_GSM_4649_112_2_17_22_13_20_792,"ATC: ATC_ProcessAlertingInd, invalid call index!")
TRACE_MSG(ATC_GSM_4803_112_2_17_22_13_21_793,"ATC: Response the CONNECT result code")
TRACE_MSG(ATC_GSM_5069_112_2_17_22_13_21_794,"ATC:ATC_ProcessSTAR_SPNTRG")
TRACE_MSG(ATC_GSM_5094_112_2_17_22_13_21_795,"ATC: ATC_ProcessSTAR_SPNTRG, plmn_status:%d,rat:%d,cell_cap:%d%d%d%d%d")
TRACE_MSG(ATC_GSM_5204_112_2_17_22_13_21_796,"ATC: Assert Failure unknow disc code.")
TRACE_MSG(ATC_GSM_5236_112_2_17_22_13_21_797,"ss state %x,%d")
TRACE_MSG(ATC_GSM_5274_112_2_17_22_13_22_798,"ATC: ATC_ProcessCallDiscInd, call_amount:%d,g_atc_call_flag:%d,is_voice_call:%d")
TRACE_MSG(ATC_GSM_5357_112_2_17_22_13_22_799,"ss state %x")
TRACE_MSG(ATC_GSM_5437_112_2_17_22_13_22_800,"connect call id = %d")
TRACE_MSG(ATC_GSM_5448_112_2_17_22_13_22_801,"ATC: retrieve call id = %d")
TRACE_MSG(ATC_GSM_5456_112_2_17_22_13_22_802,"ATC: chld disconnect call success")
TRACE_MSG(ATC_GSM_5556_112_2_17_22_13_22_803,"ATC: enter ATC_ProcessAcmInd, dual_sys =%d")
TRACE_MSG(ATC_GSM_5580_112_2_17_22_13_22_804,"ATC: enter the timer expire dual_sys =%d")
TRACE_MSG(ATC_GSM_5584_112_2_17_22_13_22_805,"ATC: unexpected timer expire")
TRACE_MSG(ATC_GSM_5590_112_2_17_22_13_22_806,"ATC: ATC_ProcessCallExpInd, CheckPresentWaitCall failure!")
TRACE_MSG(ATC_GSM_5598_112_2_17_22_13_22_807,"ATC: ATC_ProcessCallExpInd, invalid call index!")
TRACE_MSG(ATC_GSM_5658_112_2_17_22_13_22_808,"ATC: ATC_ProcessCallExpInd, MNCALL_ConnectCall failure!")
TRACE_MSG(ATC_GSM_5708_112_2_17_22_13_22_809,"ATC: ATC_ProcessCBST")
TRACE_MSG(ATC_GSM_5757_112_2_17_22_13_22_810,"ATC: ATC_ProcessCMOD")
TRACE_MSG(ATC_GSM_5806_112_2_17_22_13_23_811,"ATC: ATC_ProcessDL")
TRACE_MSG(ATC_GSM_5820_112_2_17_22_13_23_812,"ATC:ATC_ProcessDL atc_config_ptr->call_link_id = %d ")
TRACE_MSG(ATC_GSM_5828_112_2_17_22_13_23_813,"ATC: failure in MNCALL_StartCallEx")
TRACE_MSG(ATC_GSM_5834_112_2_17_22_13_23_814,"ATC: no call number recently.")
TRACE_MSG(ATC_GSM_5855_112_2_17_22_13_23_815,"ATC: ATC_ProcessCHUP\n")
TRACE_MSG(ATC_GSM_5885_112_2_17_22_13_23_816,"ATC: g_calls_state.call_amout= %d")
TRACE_MSG(ATC_GSM_6102_112_2_17_22_13_23_817,"ATC: ATC_ProcessCHUPVT")
TRACE_MSG(ATC_GSM_6106_112_2_17_22_13_23_818,"ATC: ATC_ProcessCHUPVT, NOT set command!")
TRACE_MSG(ATC_GSM_6112_112_2_17_22_13_23_819,"ATC: ATC_ProcessCHUPVT, no param1")
TRACE_MSG(ATC_GSM_6127_112_2_17_22_13_23_820,"ATC: g_calls_state.call_amout= %d")
TRACE_MSG(ATC_GSM_6174_112_2_17_22_13_23_821,"ATC: the dtmf value is %c")
TRACE_MSG(ATC_GSM_6249_112_2_17_22_13_23_822,"ATC:invalid plmn num %c")
TRACE_MSG(ATC_GSM_6265_112_2_17_22_13_23_823,"ATC: ATC_ProcessTRACE")
TRACE_MSG(ATC_GSM_6275_112_2_17_22_13_24_824,"ATC: ATC_ProcessTRACE set:%d->%d")
TRACE_MSG(ATC_GSM_6312_112_2_17_22_13_24_825,"ATC: ATC_ProcessOFF")
TRACE_MSG(ATC_GSM_6343_112_2_17_22_13_24_826,"ATC: ATC_ProcessRESET")
TRACE_MSG(ATC_GSM_6351_112_2_17_22_13_24_827,"ATC: no parameter 1")
TRACE_MSG(ATC_GSM_6356_112_2_17_22_13_24_828,"ATC: no parameter 1")
TRACE_MSG(ATC_GSM_6370_112_2_17_22_13_24_829,"ATC: Previous Reset is in progress.")
TRACE_MSG(ATC_GSM_6413_112_2_17_22_13_24_830,"ATC: ATC_ProcessSPAUTO, invalid parameter or not execute command")
TRACE_MSG(ATC_GSM_6427_112_2_17_22_13_24_831,"ATC: ATC_ProcessSPAUTO, invalid parameter")
TRACE_MSG(ATC_GSM_6438_112_2_17_22_13_24_832,"ATC: ATC_ProcessSPAUTO, invalid parameter")
TRACE_MSG(ATC_GSM_6466_112_2_17_22_13_24_833,"ATC: ATC_ProcessSPAUTO, invalide parameter(not 0 & 1)")
TRACE_MSG(ATC_GSM_6506_112_2_17_22_13_24_834,"ATC: ATC_GetPlmnDetailedInfo, Invalid parameters")
TRACE_MSG(ATC_GSM_6517_112_2_17_22_13_24_835,"ATC: ATC_GetPlmnDetailedInfo, FULL_NAME is too long (%d)")
TRACE_MSG(ATC_GSM_6538_112_2_17_22_13_24_836,"ATC: ATC_GetPlmnDetailedInfo, SHORT_NAME is too long (%d)")
TRACE_MSG(ATC_GSM_6766_112_2_17_22_13_24_837,"ATC:CUFP not support this plmn")
TRACE_MSG(ATC_GSM_6771_112_2_17_22_13_24_838,"ATC:CUFP plmn = %d")
TRACE_MSG(ATC_GSM_6787_112_2_17_22_13_25_839,"ATC: CUFP not support this plmn")
TRACE_MSG(ATC_GSM_6793_112_2_17_22_13_25_840,"ATC: MNPHONE_WritePLMNtoSim failure")
TRACE_MSG(ATC_GSM_SMS_636_112_2_17_22_13_26_841,"ATC: Get sc failure")
TRACE_MSG(ATC_GSM_SMS_643_112_2_17_22_13_26_842,"ATC: Read s_sms_nv_config failure")
TRACE_MSG(ATC_GSM_SMS_658_112_2_17_22_13_26_843,"ATC: CSAS, sms is not ready!!!")
TRACE_MSG(ATC_GSM_SMS_669_112_2_17_22_13_26_844,"ATC: write NV_ATC_CONFIG_ID fail")
TRACE_MSG(ATC_GSM_SMS_704_112_2_17_22_13_26_845,"ATC: ATC_ProcessCPMS: sms is not ready")
TRACE_MSG(ATC_GSM_SMS_734_112_2_17_22_13_26_846,"ATC:AT+CPMS,error in para1")
TRACE_MSG(ATC_GSM_SMS_757_112_2_17_22_13_26_847,"ATC:AT+CPMS,error in para2")
TRACE_MSG(ATC_GSM_SMS_782_112_2_17_22_13_26_848,"ATC:AT+CPMS,error in para2")
TRACE_MSG(ATC_GSM_SMS_796_112_2_17_22_13_26_849,"ATC: CPMS: sms is not ready")
TRACE_MSG(ATC_GSM_SMS_841_112_2_17_22_13_26_850,"ATC: CPMS: sms is not ready")
TRACE_MSG(ATC_GSM_SMS_1112_112_2_17_22_13_27_851,"ATC: CSCA, sms is not ready!!!")
TRACE_MSG(ATC_GSM_SMS_1151_112_2_17_22_13_27_852,"ATC: Get sc failure")
TRACE_MSG(ATC_GSM_SMS_1205_112_2_17_22_13_27_853,"ATC: CNMI, only support <bm>=0,1 others as 1 (%d->1)")
TRACE_MSG(ATC_GSM_SMS_1230_112_2_17_22_13_27_854,"ATC: CNMI, bfr:%d")
TRACE_MSG(ATC_GSM_SMS_1454_112_2_17_22_13_28_855,"ATC:CMGS,Deactive CMMS timer")
TRACE_MSG(ATC_GSM_SMS_1468_112_2_17_22_13_28_856,"ATC:s_sms_record[g_current_card_id].length=%d")
TRACE_MSG(ATC_GSM_SMS_1604_112_2_17_22_13_28_857,"ATC: CMGW, no PARAM1")
TRACE_MSG(ATC_GSM_SMS_1617_112_2_17_22_13_28_858,"ATC: CMGW, text mode, invalid parameters")
TRACE_MSG(ATC_GSM_SMS_1639_112_2_17_22_13_28_859,"ATC: ATC_ProcessCMGW, text mode, ATC_ConvertTeleStrToStruct")
TRACE_MSG(ATC_GSM_SMS_1650_112_2_17_22_13_28_860,"ATC: ATC_ProcessCMGW, text mode, invalid <tooa> parameter(%d).")
TRACE_MSG(ATC_GSM_SMS_1673_112_2_17_22_13_28_861,"ATC: CMGW, text mode, stat need less than ATC_SMS_ALL(%d)")
TRACE_MSG(ATC_GSM_SMS_1688_112_2_17_22_13_28_862,"ATC: CMGW, text mode, oa=%s,tooa=%d,stat=%d")
TRACE_MSG(ATC_GSM_SMS_1700_112_2_17_22_13_28_863,"ATC: CMGW, PDU mode, invalid parameters")
TRACE_MSG(ATC_GSM_SMS_1711_112_2_17_22_13_28_864,"ATC: CMGW, PDU mode, length: NOT (%d-%d)")
TRACE_MSG(ATC_GSM_SMS_1719_112_2_17_22_13_28_865,"ATC: CMGW, PDU mode, stat need less than ATC_SMS_ALL(%d)")
TRACE_MSG(ATC_GSM_SMS_1728_112_2_17_22_13_28_866,"ATC: CMGW, PDU mode, length=%d,stat=%d")
TRACE_MSG(ATC_GSM_SMS_2002_112_2_17_22_13_29_867,"ATC: CMSS,ERROR, param2 is not string!")
TRACE_MSG(ATC_GSM_SMS_2032_112_2_17_22_13_29_868,"ATC: ATC_ProcessCMSS, ERROR, invalid PDU mode paramete")
TRACE_MSG(ATC_GSM_SMS_2065_112_2_17_22_13_29_869,"ATC: +CMSS toda=%d")
TRACE_MSG(ATC_GSM_SMS_2159_112_2_17_22_13_29_870,"ATC: Read msg and language from NV fail")
TRACE_MSG(ATC_GSM_SMS_2195_112_2_17_22_13_29_871,"ATC: CSCB param2 is not string type.")
TRACE_MSG(ATC_GSM_SMS_2202_112_2_17_22_13_29_872,"ATC: CSCB param2 is not correct values.")
TRACE_MSG(ATC_GSM_SMS_2218_112_2_17_22_13_29_873,"ATC:  CSCB param3 is not string type.")
TRACE_MSG(ATC_GSM_SMS_2224_112_2_17_22_13_29_874,"ATC: CSCB param2 is not correct values.")
TRACE_MSG(ATC_GSM_SMS_2329_112_2_17_22_13_30_875,"ATC:enter SMSC")
TRACE_MSG(ATC_GSM_SMS_2333_112_2_17_22_13_30_876,"ATC: SMSC: sms is not ready")
TRACE_MSG(ATC_GSM_SMS_2377_112_2_17_22_13_30_877,"ATC: SMSC, status_flag = TRUE")
TRACE_MSG(ATC_GSM_SMS_2386_112_2_17_22_13_30_878,"ATC:SMSC, status_flag = FALSE")
TRACE_MSG(ATC_GSM_SMS_2392_112_2_17_22_13_30_879,"ATC: SMSC, ChangeSmsStatus is FALSE")
TRACE_MSG(ATC_GSM_SMS_2396_112_2_17_22_13_30_880,"ATC:cmgf:0,status_ptr=%d, sms_status=%d,at_sms_status=%d,status_flag=%d")
TRACE_MSG(ATC_GSM_SMS_2427_112_2_17_22_13_30_881,"ATC:cmgf:1,status_ptr=%d, sms_status=%d,at_sms_status=%d,status_flag=%d")
TRACE_MSG(ATC_GSM_SMS_2554_112_2_17_22_13_30_882,"ATC: CSMS = 0 ,CNMA not support")
TRACE_MSG(ATC_GSM_SMS_2637_112_2_17_22_13_30_883,"ATC: CSMS = 0 ,CNMA not support")
TRACE_MSG(ATC_GSM_SMS_2796_112_2_17_22_13_31_884,"ATC: The record length is %d,pdu_len:%d")
TRACE_MSG(ATC_GSM_SMS_2821_112_2_17_22_13_31_885,"ATC:Call HandlePduModeSms()")
TRACE_MSG(ATC_GSM_SMS_2833_112_2_17_22_13_31_886,"ATC:flag = %d")
TRACE_MSG(ATC_GSM_SMS_2875_112_2_17_22_13_31_887,"ATC: The cnma input length is error %d != %d")
TRACE_MSG(ATC_GSM_SMS_3015_112_2_17_22_13_31_888,"ATC: write NV_ATC_CONFIG_ID fail")
TRACE_MSG(ATC_GSM_SMS_3051_112_2_17_22_13_31_889,"ATC: ATC_RestoreSmsConfig, Get sc failure")
TRACE_MSG(ATC_GSM_SMS_3056_112_2_17_22_13_31_890,"ATC: ATC_RestoreSmsConfig, SIM not ready, cannot get sc")
TRACE_MSG(ATC_GSM_SMS_3131_112_2_17_22_13_31_891,"ATC:error ConvertHexToBin")
TRACE_MSG(ATC_GSM_SMS_3140_112_2_17_22_13_31_892,"ATC:no SC address")
TRACE_MSG(ATC_GSM_SMS_3155_112_2_17_22_13_32_893,"ATC: HandlePduModeSms sc_addr.length = %d")
TRACE_MSG(ATC_GSM_SMS_3170_112_2_17_22_13_32_894,"ATC:have SC address")
TRACE_MSG(ATC_GSM_SMS_3235_112_2_17_22_13_32_895,"ATC: failure in MNSMS_AtcWriteSmsEx")
TRACE_MSG(ATC_GSM_SMS_3313_112_2_17_22_13_32_896,"ATC_ProcessWriteSmsCnf: max_num=%d")
TRACE_MSG(ATC_GSM_SMS_3314_112_2_17_22_13_32_897,"ATC_ProcessWriteSmsCnf: used_num=%d")
TRACE_MSG(ATC_GSM_SMS_3323_112_2_17_22_13_32_898,"ATC: failure in MNSMS_AtcWriteSmsEx")
TRACE_MSG(ATC_GSM_SMS_3355_112_2_17_22_13_32_899,"ATC: full in HandlePduSms_SpsmsFl")
TRACE_MSG(ATC_GSM_SMS_3577_112_2_17_22_13_33_900,"ATC:ATC_ProcessSendSmsCnf, Active CMMS timer")
TRACE_MSG(ATC_GSM_SMS_3584_112_2_17_22_13_33_901,"ATC: Receive the error status: %d")
TRACE_MSG(ATC_GSM_SMS_3644_112_2_17_22_13_33_902,"ATC: Received APP_MN_UPDATE_SMS_STATE_CNF, sleep 30ms")
TRACE_MSG(ATC_GSM_SMS_3754_112_2_17_22_13_33_903,"ATC: GetStorageInfo: WARNING, SmsNum is 0")
TRACE_MSG(ATC_GSM_SMS_3764_112_2_17_22_13_33_904,"ATC: GetStorageInfo: no mem, SmsNum is %d")
TRACE_MSG(ATC_GSM_SMS_3795_112_2_17_22_13_33_905,"ATC: total amout: %d, used amount:%d,unread flag:%d")
TRACE_MSG(ATC_GSM_SMS_3911_112_2_17_22_13_33_906,"ATC: the current cmgl index: %d")
TRACE_MSG(ATC_GSM_SMS_3938_112_2_17_22_13_33_907,"ATC:WRITE error, Stopping Filling SMS Storage!")
TRACE_MSG(ATC_GSM_SMS_3957_112_2_17_22_13_33_908,"ATC:WRITE error, Stopping Filling SMS Storage!")
TRACE_MSG(ATC_GSM_SMS_3985_112_2_17_22_13_33_909,"ATC:WRITE error!")
TRACE_MSG(ATC_GSM_SMS_4003_112_2_17_22_13_33_910,"ATC:WRITE error!")
TRACE_MSG(ATC_GSM_SMS_4021_112_2_17_22_13_33_911,"ATC:ATC_ProcessSmsFullInd, mem_status = %d")
TRACE_MSG(ATC_GSM_SMS_4075_112_2_17_22_13_33_912,"ATC: Get sc failure")
TRACE_MSG(ATC_GSM_SMS_4104_112_2_17_22_13_34_913,"ATC: ATC_ProcessSmsStatusReportInd  sig->dual_sys: %d")
TRACE_MSG(ATC_GSM_SMS_4172_112_2_17_22_13_34_914,"ATC: disable the status report")
TRACE_MSG(ATC_GSM_SMS_4254_112_2_17_22_13_34_915,"ATC: invalid <text> length:%d.")
TRACE_MSG(ATC_GSM_SMS_4276_112_2_17_22_13_34_916,"ATC: text sms length:%d,dcs:%d.")
TRACE_MSG(ATC_GSM_SMS_4343_112_2_17_22_13_34_917,"ATC: MNSMS_EncodeUserData failed.")
TRACE_MSG(ATC_GSM_SMS_4389_112_2_17_22_13_34_918,"ATC: MNSMS_EncodeUserData failed.")
TRACE_MSG(ATC_GSM_SMS_4398_112_2_17_22_13_34_919,"ATC:status_report_is_request=%d")
TRACE_MSG(ATC_GSM_SMS_4446_112_2_17_22_13_34_920,"ATC: call MNSMS_WriteMtSmsEx")
TRACE_MSG(ATC_GSM_SMS_4450_112_2_17_22_13_34_921,"ATC: failure in MNSMS_WriteMtSmsEx")
TRACE_MSG(ATC_GSM_SMS_4465_112_2_17_22_13_34_922,"ATC: failure in MNSMS_WriteMoSmsEx")
TRACE_MSG(ATC_GSM_SMS_4546_112_2_17_22_13_34_923,"ATC_ProcessWriteSmsCnf: max_num=%d")
TRACE_MSG(ATC_GSM_SMS_4547_112_2_17_22_13_34_924,"ATC_ProcessWriteSmsCnf: used_num=%d")
TRACE_MSG(ATC_GSM_SMS_4553_112_2_17_22_13_34_925,"ATC: call MNSMS_WriteMtSmsEx")
TRACE_MSG(ATC_GSM_SMS_4557_112_2_17_22_13_34_926,"ATC: failure in MNSMS_WriteMtSmsEx")
TRACE_MSG(ATC_GSM_SMS_4586_112_2_17_22_13_35_927,"ATC: failure in MNSMS_WriteMoSmsEx")
TRACE_MSG(ATC_GSM_SMS_4615_112_2_17_22_13_35_928,"ATC: full in HandleTextModeSms_SpsmsFL")
TRACE_MSG(ATC_GSM_SMS_4705_112_2_17_22_13_35_929,"ATC: HandleTextModeSmsReadCnf, dual_sys: %d sms_status:%d,record_id:%d")
TRACE_MSG(ATC_GSM_SMS_4713_112_2_17_22_13_35_930,"ATC: SMS FREE SPACE!")
TRACE_MSG(ATC_GSM_SMS_5014_112_2_17_22_13_35_931,"ATC: Received  APP_MN_READ_SMS_CNF, text mode, sleep 30ms")
TRACE_MSG(ATC_GSM_SMS_5181_112_2_17_22_13_36_932,"ATC: HandlePduModeSmsReadCnf,dual_sys:%d, sms_status:%d,record_id:%d")
TRACE_MSG(ATC_GSM_SMS_5189_112_2_17_22_13_36_933,"ATC: Read a free SMS space.")
TRACE_MSG(ATC_GSM_SMS_5243_112_2_17_22_13_36_934,"atc: george for status report")
TRACE_MSG(ATC_GSM_SMS_5321_112_2_17_22_13_36_935,"ATC: Received  APP_MN_READ_SMS_CNF, pdu mode, sleep 30ms")
TRACE_MSG(ATC_GSM_SMS_5518_112_2_17_22_13_36_936,"ATC: The temp_len is %d")
TRACE_MSG(ATC_GSM_SMS_5548_112_2_17_22_13_36_937,"ATC: ReadNextSMS, dual_sys = %d")
TRACE_MSG(ATC_GSM_SMS_5552_112_2_17_22_13_36_938,"ATC: ReadNextSMS, sleep 30ms")
TRACE_MSG(ATC_GSM_SMS_5561_112_2_17_22_13_36_939,"ATC: ReadNextSMS, finish")
TRACE_MSG(ATC_GSM_SMS_5675_112_2_17_22_13_37_940,"ATC: NO MEM!!!in DeleteSpecSms ")
TRACE_MSG(ATC_GSM_SMS_5750_112_2_17_22_13_37_941,"ATC:the current cmgl index: %d")
TRACE_MSG(ATC_GSM_SMS_5951_112_2_17_22_13_37_942,"ATC: Write NV_ATC_CB_MSG_ID failed")
TRACE_MSG(ATC_GSM_SMS_5956_112_2_17_22_13_37_943,"ATC: Write NV_ATC_CB_LANG_ID failed")
TRACE_MSG(ATC_GSM_SMS_6015_112_2_17_22_13_37_944,"ATC: The temp_len is %d")
TRACE_MSG(ATC_GSM_SMS_6055_112_2_17_22_13_37_945,"ATC: ATC_ProcessCMMS, no PARAM1")
TRACE_MSG(ATC_GSM_SMS_6119_112_2_17_22_13_38_946,"ATC: CMMSTimeout: WARNING! DISABLE == cmms_value")
TRACE_MSG(ATC_GSM_SMS_6124_112_2_17_22_13_38_947,"ATC: CMMSTimeout: cmms_value: KEEP_ENABLED->DISABLE")
TRACE_MSG(ATC_GSM_SMS_6130_112_2_17_22_13_38_948,"ATC: CMMSTimeout: cmms_value: ENABLED")
TRACE_MSG(ATC_GSM_SMS_6135_112_2_17_22_13_38_949,"ATC: CMMSTimeout: WARNING! unknown cmms_value:%d")
TRACE_MSG(ATC_GSM_SMS_6149_112_2_17_22_13_38_950,"ATC: ATC_ActiveCMMSTimer, dual_sys = %d")
TRACE_MSG(ATC_GSM_SMS_6160_112_2_17_22_13_38_951,"ATC: ATC_ActiveCMMSTimer, active timer(cmms_value=%d,cmms_flag=%d)")
TRACE_MSG(ATC_GSM_SMS_6168_112_2_17_22_13_38_952,"ATC: ATC_ActiveCMMSTimer, deactive timer(cmms_value=%d,cmms_flag=%d)")
TRACE_MSG(ATC_GSM_SMS_6287_112_2_17_22_13_38_953,"ATC: GetValidityPeriod, warning, invalid absolute period")
TRACE_MSG(ATC_GSM_SMS_6360_112_2_17_22_13_38_954,"ATC: TransfromStrToBcd failed.")
TRACE_MSG(ATC_GSM_SMS_6366_112_2_17_22_13_38_955,"ATC: invalid bcd length:%d")
TRACE_MSG(ATC_GSM_SMS_6440_112_2_17_22_13_38_956,"ATC: ATC_EncodeSMSDataToTE, invalid parameters.")
TRACE_MSG(ATC_GSM_SMS_6480_112_2_17_22_13_38_957,"enter into Atc_FillMtMoSmsEx")
TRACE_MSG(ATC_GSM_SMS_6526_112_2_17_22_13_38_958,"enter into Atc_FreeMemAppliedForSfsmsm")
TRACE_MSG(ATC_GSM_SS_268_112_2_17_22_13_39_959,"ATC_EnableVoiceCodec: is_enable = %d")
TRACE_MSG(ATC_GSM_SS_330_112_2_17_22_13_39_960,"ATC: ATC_ProcessCCFC, too many params(more than 2 is allowed if REGISTRATION)")
TRACE_MSG(ATC_GSM_SS_337_112_2_17_22_13_39_961,"ATC: CCFC,  param 4 or 7 wrong values!")
TRACE_MSG(ATC_GSM_SS_363_112_2_17_22_13_39_962,"ATC: CCFC, Class not allowed")
TRACE_MSG(ATC_GSM_SS_380_112_2_17_22_13_39_963,"ATC: CCFC, MNSS_DeactivateSS, failure:%d")
TRACE_MSG(ATC_GSM_SS_388_112_2_17_22_13_39_964,"ATC: CCFC, Class not allowed")
TRACE_MSG(ATC_GSM_SS_405_112_2_17_22_13_39_965,"ATC: CCFC, MNSS_ActivateSS, failure:%d")
TRACE_MSG(ATC_GSM_SS_413_112_2_17_22_13_39_966,"ATC: CCFC, Class not allowed")
TRACE_MSG(ATC_GSM_SS_435_112_2_17_22_13_39_967,"ATC: CCFC, MNSS_InterrogateService, failure:%d")
TRACE_MSG(ATC_GSM_SS_447_112_2_17_22_13_39_968,"ATC: reg fail1")
TRACE_MSG(ATC_GSM_SS_454_112_2_17_22_13_39_969,"ATC: CCFC, param3 is not string!")
TRACE_MSG(ATC_GSM_SS_469_112_2_17_22_13_40_970,"ATC: reg fail2")
TRACE_MSG(ATC_GSM_SS_480_112_2_17_22_13_40_971,"ATC: reg fail3")
TRACE_MSG(ATC_GSM_SS_488_112_2_17_22_13_40_972,"ATC: Subaddr is not string")
TRACE_MSG(ATC_GSM_SS_496_112_2_17_22_13_40_973,"ATC: reg fail4")
TRACE_MSG(ATC_GSM_SS_516_112_2_17_22_13_40_974,"ATC: CCFC, Class not allowed")
TRACE_MSG(ATC_GSM_SS_532_112_2_17_22_13_40_975,"ATC: CCFC, MNSS_RegisterSSInfo, failure:%d")
TRACE_MSG(ATC_GSM_SS_544_112_2_17_22_13_40_976,"ATC: CCFC, Class not allowed")
TRACE_MSG(ATC_GSM_SS_561_112_2_17_22_13_40_977,"ATC: CCFC, MNSS_EraseSSInfo, failure:%d")
TRACE_MSG(ATC_GSM_SS_570_112_2_17_22_13_40_978,"ATC: re_valuse=%x")
TRACE_MSG(ATC_GSM_SS_792_112_2_17_22_13_40_979,"ATC: APP_MN_SETUP_COMPLETE_CNF")
TRACE_MSG(ATC_GSM_SS_799_112_2_17_22_13_40_980,"ATC: APP_MN_SETUP_COMPLETE_IND")
TRACE_MSG(ATC_GSM_SS_813_112_2_17_22_13_40_981,"ATC: ATC_ProcessSetupCompleteSignal, invalid call index.")
TRACE_MSG(ATC_GSM_SS_827_112_2_17_22_13_40_982,"ATC: setup complete expire timer check.")
TRACE_MSG(ATC_GSM_SS_1154_112_2_17_22_13_41_983,"ATC: user set the AOC mode")
TRACE_MSG(ATC_GSM_SS_1215_112_2_17_22_13_41_984,"ATC: CACM 1")
TRACE_MSG(ATC_GSM_SS_1222_112_2_17_22_13_41_985,"ATC:CACM 1-1")
TRACE_MSG(ATC_GSM_SS_1227_112_2_17_22_13_41_986,"ATC: CACM 1-2")
TRACE_MSG(ATC_GSM_SS_1250_112_2_17_22_13_41_987,"ATC: CACM 2(1)")
TRACE_MSG(ATC_GSM_SS_1252_112_2_17_22_13_41_988,"ATC: CACM 2(2)")
TRACE_MSG(ATC_GSM_SS_1255_112_2_17_22_13_41_989,"ATC:CACM 2-1")
TRACE_MSG(ATC_GSM_SS_1260_112_2_17_22_13_41_990,"ATC: CACM 2-2")
TRACE_MSG(ATC_GSM_SS_1315_112_2_17_22_13_41_991,"ATC: CAMM, param1 is not string!")
TRACE_MSG(ATC_GSM_SS_1320_112_2_17_22_13_41_992,"ATC: CAMM, param1 length is 0!")
TRACE_MSG(ATC_GSM_SS_1325_112_2_17_22_13_41_993,"ATC: CAMM, param1 length is too long >6 !")
TRACE_MSG(ATC_GSM_SS_1332_112_2_17_22_13_41_994,"ATC: CAMM, param1 format error!")
TRACE_MSG(ATC_GSM_SS_1350_112_2_17_22_13_41_995,"ATC: CAMM, param2 is not string!")
TRACE_MSG(ATC_GSM_SS_1414_112_2_17_22_13_41_996,"ATC: ATC_DecStrToE, no valid string data!")
TRACE_MSG(ATC_GSM_SS_1426_112_2_17_22_13_41_997,"ATC: ATC_DecStrToE, more '.'")
TRACE_MSG(ATC_GSM_SS_1435_112_2_17_22_13_41_998,"ATC: ATC_DecStrToE, NOT '0' ~ '9'!")
TRACE_MSG(ATC_GSM_SS_1444_112_2_17_22_13_41_999,"ATC: ATC_DecStrToE, too many digits!")
TRACE_MSG(ATC_GSM_SS_1449_112_2_17_22_13_41_1000,"ATC: ATC_DecStrToE, too big value!")
TRACE_MSG(ATC_GSM_SS_1515_112_2_17_22_13_42_1001,"ATC: CPUC, param1 is not string!")
TRACE_MSG(ATC_GSM_SS_1540_112_2_17_22_13_42_1002,"ATC: CPUC, param2 is not string!")
TRACE_MSG(ATC_GSM_SS_1545_112_2_17_22_13_42_1003,"ATC: CPUC, param2 len is 0")
TRACE_MSG(ATC_GSM_SS_1552_112_2_17_22_13_42_1004,"ATC: CPUC, ATC_DecStrToE error!")
TRACE_MSG(ATC_GSM_SS_1570_112_2_17_22_13_42_1005,"ATC: CPUC, param3 is not string!")
TRACE_MSG(ATC_GSM_SS_1639_112_2_17_22_13_42_1006,"ATC: ATC_PROCESSCHVCNF, chv_status=%d, pin_function_type=%d")
TRACE_MSG(ATC_GSM_SS_1658_112_2_17_22_13_42_1007,"ATC: ATC_PROCESSCHVCNF, ATC_PIN_RESET_ACM_UNIT")
TRACE_MSG(ATC_GSM_SS_1670_112_2_17_22_13_42_1008,"ATC: ATC_PROCESSCHVCNF, ATC_PIN_ENABLE_FDN")
TRACE_MSG(ATC_GSM_SS_1675_112_2_17_22_13_42_1009,"ATC: ATC_PROCESSCHVCNF, ATC_PIN_DISABLE_FDN")
TRACE_MSG(ATC_GSM_SS_1760_112_2_17_22_13_42_1010,"ATC: ATC_ProcessPuctCnf, Read: eppu:%d,ex_sign:%d,ex:%d,ex_value:%d")
TRACE_MSG(ATC_GSM_SS_2164_112_2_17_22_13_43_1011,"ATC: MNSS_UserOriginateUSSD")
TRACE_MSG(ATC_GSM_SS_2183_112_2_17_22_13_43_1012,"ATC: MNSS_ResponseUSSD")
TRACE_MSG(ATC_GSM_SS_2249_112_2_17_22_13_43_1013,"ATC: CUSD, param3 is not num!")
TRACE_MSG(ATC_GSM_SS_2274_112_2_17_22_13_43_1014,"ATC: CUSD, param2 is not string!")
TRACE_MSG(ATC_GSM_SS_2281_112_2_17_22_13_43_1015,"ATC: CUSD, the string is too long")
TRACE_MSG(ATC_GSM_SS_2307_112_2_17_22_13_43_1016,"ATC: <dcs> is %d.")
TRACE_MSG(ATC_GSM_SS_2346_112_2_17_22_13_43_1017,"ATC: ATC_ProcessUssdInd, dual_sys: %d, s_cusd_presentation:%d,s_cusd_isresponse:%d")
TRACE_MSG(ATC_GSM_SS_2376_112_2_17_22_13_43_1018,"ATC: ATC_ProcessUssdNotifyInd, dual_sys: %d, s_cusd_isresponse:%d,s_cusd_presentation:%d")
TRACE_MSG(ATC_GSM_SS_2401_112_2_17_22_13_43_1019,"ATC: ATC_ProcessUssdServiceCnf, dual_sys: %d, s_cusd_isresponse:%d,s_cusd_presentation:%d")
TRACE_MSG(ATC_GSM_SS_2406_112_2_17_22_13_43_1020,"ATC: MN_SS_OPERATION_SUCC")
TRACE_MSG(ATC_GSM_SS_2411_112_2_17_22_13_43_1021,"ATC: MN_SS_OPERATION_ERROR,error code:%d")
TRACE_MSG(ATC_GSM_SS_2416_112_2_17_22_13_43_1022,"ATC: MN_SS_OPERATION_REJECT,error code:%d")
TRACE_MSG(ATC_GSM_SS_2702_112_2_17_22_13_44_1023,"ATC: ATC_ProcessBarredInd, unknown barred_operation:%d")
TRACE_MSG(ATC_GSM_SS_3024_112_2_17_22_13_45_1024,"ATC: ATC_ProcessInterCnf,cfall=%d,cfu=%d,cfcond=%d,cfb=%d,cfnry=%d,cfnrc=%d")
TRACE_MSG(ATC_GSM_SS_3042_112_2_17_22_13_45_1025,"ATC: ATC_ProcessInterCnf, unknown s_ss_context.cmd_id: %d")
TRACE_MSG(ATC_GSM_SS_3152_112_2_17_22_13_45_1026,"ATC: ATC_GetParams=FALSE")
TRACE_MSG(ATC_GSM_SS_3186_112_2_17_22_13_45_1027,"ATC: WARNING, the length of passward is wrong!")
TRACE_MSG(ATC_GSM_SS_3230_112_2_17_22_13_45_1028,"ATC: is_sim_busy=%d")
TRACE_MSG(ATC_GSM_SS_3234_112_2_17_22_13_45_1029,"ATC: SIM is busy")
TRACE_MSG(ATC_GSM_SS_3407_112_2_17_22_13_45_1030,"ATC: CLCK ATC_CLCK_QUERY_STATUS is_fdn_enable=%d")
TRACE_MSG(ATC_GSM_SS_3426_112_2_17_22_13_45_1031,"ATC: CPIN pin1 enable flag: %d")
TRACE_MSG(ATC_GSM_SS_3479_112_2_17_22_13_45_1032,"ATC: basic_service.service_code=%d")
TRACE_MSG(ATC_GSM_SS_3534_112_2_17_22_13_46_1033,"ATC: cpwd parameter is not correct")
TRACE_MSG(ATC_GSM_SS_3545_112_2_17_22_13_46_1034,"ATC: cpwd command type is set")
TRACE_MSG(ATC_GSM_SS_3568_112_2_17_22_13_46_1035,"ATC: WARNING, the length of passward is wrong!")
TRACE_MSG(ATC_GSM_SS_3587_112_2_17_22_13_46_1036,"ATC: WARNING, the length of passward is wrong!")
TRACE_MSG(ATC_GSM_SS_3609_112_2_17_22_13_46_1037,"ATC: cpwd sim is not ready")
TRACE_MSG(ATC_GSM_SS_3631_112_2_17_22_13_46_1038,"ATC: cpwd sim is not ready")
TRACE_MSG(ATC_GSM_SS_3660_112_2_17_22_13_46_1039,"ATC: cpwd password length is large")
TRACE_MSG(ATC_GSM_SS_3680_112_2_17_22_13_46_1040,"ATC: cpwd command type is read")
TRACE_MSG(ATC_GSM_SS_3684_112_2_17_22_13_46_1041,"ATC: cpwd command type is test")
TRACE_MSG(ATC_GSM_SS_3713_112_2_17_22_13_46_1042,"ATC:ATC_ProcessGetPswInd")
TRACE_MSG(ATC_GSM_SS_3717_112_2_17_22_13_46_1043,"ATC:AT_CMD_CPWD")
TRACE_MSG(ATC_GSM_SS_3721_112_2_17_22_13_46_1044,"ATC:s_old_password")
TRACE_MSG(ATC_GSM_SS_3727_112_2_17_22_13_46_1045,"ATC:s_new_password")
TRACE_MSG(ATC_GSM_SS_3732_112_2_17_22_13_46_1046,"ATC:s_new_password again")
TRACE_MSG(ATC_GSM_SS_3737_112_2_17_22_13_46_1047,"ATC: received error singal in get password indicate!!!")
TRACE_MSG(ATC_GSM_SS_3741_112_2_17_22_13_46_1048,"ATC:s_clck_psw")
TRACE_MSG(ATC_GSM_SS_3785_112_2_17_22_13_46_1049,"ATC: APP_MN_RELEASE_SS_IND")
TRACE_MSG(ATC_GSM_SS_3941_112_2_17_22_13_46_1050,"ATC: ATC_ProcessSSCnf result=%d,list_num=%d,ss_status_present=%d,ss_status=%d,cmd_id=%d,ss_code=%d")
TRACE_MSG(ATC_GSM_SS_4035_112_2_17_22_13_47_1051,"ATC: ATC_ProcessSSCnf,cfall=%d,cfu=%d,cfcond=%d,cfb=%d,cfnry=%d,cfnrc=%d")
TRACE_MSG(ATC_GSM_SS_4069_112_2_17_22_13_47_1052,"ATC: ATC_CheckInterrogateCnf result=%d,list_num=%d,ss_status_present=%d,ss_status=%d,service_type=%d,service_code=%d")
TRACE_MSG(ATC_GSM_SS_4392_112_2_17_22_13_47_1053,"ATC: build MPTY call successfully")
TRACE_MSG(ATC_GSM_SS_4398_112_2_17_22_13_47_1054,"ATC: build MPTY call failed")
TRACE_MSG(ATC_GSM_SS_4418_112_2_17_22_13_47_1055,"ATC: receive the APP_MN_CALL_RETRIEVED_CNF signal")
TRACE_MSG(ATC_GSM_SS_4528_112_2_17_22_13_48_1056,"ATC: receive the APP_MN_CALL_HELD_CNF signal")
TRACE_MSG(ATC_GSM_SS_4599_112_2_17_22_13_48_1057,"ATC:is_swap")
TRACE_MSG(ATC_GSM_SS_4642_112_2_17_22_13_48_1058,"ATC: receive the APP_MN_SPLIT_MPTY_CNF signal")
TRACE_MSG(ATC_GSM_SS_4760_112_2_17_22_13_48_1059,"ATC: enter ATC_CheckSSStr, dual_sys = %d")
TRACE_MSG(ATC_GSM_SS_4761_112_2_17_22_13_48_1060,"ATC: length = %d, str_ptr = %s")
TRACE_MSG(ATC_GSM_SS_4767_112_2_17_22_13_48_1061,"ATC: MNPHONE_ParseBStringEx operation = %d!")
TRACE_MSG(ATC_GSM_SS_4786_112_2_17_22_13_48_1062,"ATC: begain MNSS_UserOriginateUSSDEx")
TRACE_MSG(ATC_GSM_SS_4792_112_2_17_22_13_48_1063,"ATC: MNSS_UserOriginateUSSDEx result = %d")
TRACE_MSG(ATC_GSM_SS_4804_112_2_17_22_13_48_1064,"ATC: begain MNSS_ResponseUSSDEx")
TRACE_MSG(ATC_GSM_SS_4808_112_2_17_22_13_48_1065,"ATC: MNSS_ResponseUSSDEx result = %d")
TRACE_MSG(ATC_GSM_SS_4891_112_2_17_22_13_48_1066,"ATC: unknow ss code = %d")
TRACE_MSG(ATC_GSM_SS_4918_112_2_17_22_13_48_1067,"ATC: enter ATC_SendReqToSs dual_sys=%d")
TRACE_MSG(ATC_GSM_SS_4935_112_2_17_22_13_48_1068,"ATC: mn register ss error code = %d")
TRACE_MSG(ATC_GSM_SS_4953_112_2_17_22_13_48_1069,"ATC: mn erase ss info error code = %d")
TRACE_MSG(ATC_GSM_SS_4975_112_2_17_22_13_48_1070,"ATC: mn active ss error code %d")
TRACE_MSG(ATC_GSM_SS_4995_112_2_17_22_13_48_1071,"ATC: mn deactive ss error code = %d")
TRACE_MSG(ATC_GSM_SS_5015_112_2_17_22_13_49_1072,"ATC: mn interrogate service error code = %d")
TRACE_MSG(ATC_GSM_SS_5034_112_2_17_22_13_49_1073,"ATC: registerpassword ss_code = %d")
TRACE_MSG(ATC_GSM_SS_5038_112_2_17_22_13_49_1074,"ATC: mn register ss password error code = %d")
TRACE_MSG(ATC_GSM_SS_5052_112_2_17_22_13_49_1075,"ATC: SendReqToSs in default!")
TRACE_MSG(ATC_GSM_SS_5140_112_2_17_22_13_49_1076,"ATC_getclass_from_interrogate_cnf:service_class=%d")
TRACE_MSG(ATC_GSM_SS_5265_112_2_17_22_13_49_1077,"ATC: ATC_InterrogateCFService, NOT Call Forward Code (%d)!")
TRACE_MSG(ATC_GSM_SS_5273_112_2_17_22_13_49_1078,"ATC: ATC_InterrogateCFService, failure in MN (ss_code:%d)")
TRACE_MSG(ATC_GSM_SS_5280_112_2_17_22_13_49_1079,"ATC: ATC_InterrogateCFService, WARNING! s_ss_context[dual_sys](%d,%d) is already existed.")
TRACE_MSG(ATC_GSM_SS_5301_112_2_17_22_13_49_1080,"ATC: CCWE, no param1")
TRACE_MSG(ATC_GSM_STK_220_112_2_17_22_13_50_1081,"ATC:ATC_InitCnfSetMenu, dual_sys:%d")
TRACE_MSG(ATC_GSM_STK_241_112_2_17_22_13_50_1082,"ATC:enter SetupMenu, dual_sys=%d")
TRACE_MSG(ATC_GSM_STK_245_112_2_17_22_13_50_1083,"ATC: SetupMenu,ERROR,s_stk_mainmenu_ptr[%d] is NULL.")
TRACE_MSG(ATC_GSM_STK_440_112_2_17_22_13_50_1084,"ATC:DisplayText:STK_DCS_8_BIT_ALPHABET or STK_DCS_UNPACKED_ALPHABET ")
TRACE_MSG(ATC_GSM_STK_455_112_2_17_22_13_50_1085,"ATC:DisplayText:STK_DCS_DEFAULT_ALPHABET")
TRACE_MSG(ATC_GSM_STK_466_112_2_17_22_13_50_1086,"ATC: error length in simat.")
TRACE_MSG(ATC_GSM_STK_484_112_2_17_22_13_50_1087,"ATC:DisplayText:STK_DCS_UCS2_ALPHABET")
TRACE_MSG(ATC_GSM_STK_514_112_2_17_22_13_50_1088,"ATC:DisplayText,%d,%d")
TRACE_MSG(ATC_GSM_STK_551_112_2_17_22_13_50_1089,"ATC:GetInkey:STK_DCS_8_BIT_ALPHABET or STK_DCS_UNPACKED_ALPHABET")
TRACE_MSG(ATC_GSM_STK_574_112_2_17_22_13_50_1090,"ATC: error data length in simat")
TRACE_MSG(ATC_GSM_STK_591_112_2_17_22_13_50_1091,"ATC:GetInkey:STK_DCS_UCS2_ALPHABET")
TRACE_MSG(ATC_GSM_STK_622_112_2_17_22_13_51_1092,"ATC:GetInkey,%d,%d")
TRACE_MSG(ATC_GSM_STK_651_112_2_17_22_13_51_1093,"ATC: process get input dcs = %d, str_len = %d")
TRACE_MSG(ATC_GSM_STK_697_112_2_17_22_13_51_1094,"ATC:GetInput:STK_DCS_8_BIT_ALPHABET or STK_DCS_UNPACKED_ALPHABET")
TRACE_MSG(ATC_GSM_STK_722_112_2_17_22_13_51_1095,"ATC: error length in simat.")
TRACE_MSG(ATC_GSM_STK_744_112_2_17_22_13_51_1096,"ATC:GetInput:STK_DCS_UCS2_ALPHABET")
TRACE_MSG(ATC_GSM_STK_794_112_2_17_22_13_51_1097,"ATC: enter ATC_ProcessSetupCall")
TRACE_MSG(ATC_GSM_STK_937_112_2_17_22_13_51_1098,"ATC:select_item.is_alpha_id==TRUE")
TRACE_MSG(ATC_GSM_STK_952_112_2_17_22_13_51_1099,"ATC:select_item.is_alpha_id.id_ptr[0]==ATC_UCS2_CODE")
TRACE_MSG(ATC_GSM_STK_963_112_2_17_22_13_51_1100,"ATC:select_item.is_alpha_id.id_ptr[0]!=ATC_UCS2_CODE")
TRACE_MSG(ATC_GSM_STK_1011_112_2_17_22_13_52_1101,"s_stk_signal.menu_item_id[%d]:%d")
TRACE_MSG(ATC_GSM_STK_1149_112_2_17_22_13_52_1102,"ATC: SendSMS SC is exist.")
TRACE_MSG(ATC_GSM_STK_1161_112_2_17_22_13_52_1103,"ATC: SendSMS sc_addr.length = %d")
TRACE_MSG(ATC_GSM_STK_1173_112_2_17_22_13_52_1104,"ATC: SendSMS sc_addr.length = %d")
TRACE_MSG(ATC_GSM_STK_1178_112_2_17_22_13_52_1105,"ATC: address is not exist.")
TRACE_MSG(ATC_GSM_STK_1247_112_2_17_22_13_52_1106,"ATC: SMS is packed.")
TRACE_MSG(ATC_GSM_STK_1254_112_2_17_22_13_52_1107,"ATC: send sms pdu err code = %d.")
TRACE_MSG(ATC_GSM_STK_1266_112_2_17_22_13_52_1108,"ATC: SMS is unpacked.")
TRACE_MSG(ATC_GSM_STK_1274_112_2_17_22_13_52_1109,"ATC: send sms pdu err code = %d.")
TRACE_MSG(ATC_GSM_STK_1301_112_2_17_22_13_52_1110,"ATC:enter ATC_ProcessPlayTone")
TRACE_MSG(ATC_GSM_STK_1580_112_2_17_22_13_53_1111,"ATC: user originate ussd fail, err code = %d.")
TRACE_MSG(ATC_GSM_STK_1625_112_2_17_22_13_53_1112,"ATC:SetupEventList,num of events:%d")
TRACE_MSG(ATC_GSM_STK_1673_112_2_17_22_13_53_1113,"ATC:SetupIdleModeText:STK_DEFAULT_ALPHABET")
TRACE_MSG(ATC_GSM_STK_1685_112_2_17_22_13_53_1114,"ATC:SetupIdleModeText:STK_DCS_DEFAULT_ALPHABET")
TRACE_MSG(ATC_GSM_STK_1690_112_2_17_22_13_53_1115,"ATC: error data length in simat")
TRACE_MSG(ATC_GSM_STK_1704_112_2_17_22_13_53_1116,"ATC:SetupIdleModeText:STK_DCS_UCS2_ALPHABET")
TRACE_MSG(ATC_GSM_STK_1729_112_2_17_22_13_53_1117,"ATC:SetupIdleModeText,dcs:%d,text_len:%d")
TRACE_MSG(ATC_GSM_STK_1805_112_2_17_22_13_53_1118,"ATC:SendDtmf")
TRACE_MSG(ATC_GSM_STK_1880_112_2_17_22_13_53_1119,"%s")
TRACE_MSG(ATC_GSM_STK_1882_112_2_17_22_13_53_1120,"ATC:Refresh")
TRACE_MSG(ATC_GSM_STK_2004_112_2_17_22_13_54_1121,"ATC:LauchBrowser")
TRACE_MSG(ATC_GSM_STK_2248_112_2_17_22_13_54_1122,"ATC:OpenChannel")
TRACE_MSG(ATC_GSM_STK_2326_112_2_17_22_13_54_1123,"ATC:ATC_ProcessCloseChannel")
TRACE_MSG(ATC_GSM_STK_2409_112_2_17_22_13_55_1124,"ATC:ReceiveData")
TRACE_MSG(ATC_GSM_STK_2500_112_2_17_22_13_55_1125,"ATC:LauchBrowser")
TRACE_MSG(ATC_GSM_STK_2523_112_2_17_22_13_55_1126,"ATC:LauchBrowser")
TRACE_MSG(ATC_GSM_STK_2543_112_2_17_22_13_55_1127,"ATC: RefreshCnf, is_ok=%d,is_file_changed=%d")
TRACE_MSG(ATC_GSM_STK_2586_112_2_17_22_13_55_1128,"ATC:ATC_STK SmsCnf,MN_SMS_OPERATE_SUCCESS")
TRACE_MSG(ATC_GSM_STK_2594_112_2_17_22_13_55_1129,"ATC:ATC_STK SendSmsCnf,MN_SMS_FAIL")
TRACE_MSG(ATC_GSM_STK_2625_112_2_17_22_13_55_1130,"ATC:ATC_STK SendUssdCnf,MN_SMS_OPERATE_SUCCESS")
TRACE_MSG(ATC_GSM_STK_2635_112_2_17_22_13_55_1131,"ATC:ATC_STK SendUssdCnf,MN_USSD_FAIL")
TRACE_MSG(ATC_GSM_STK_2662_112_2_17_22_13_55_1132,"ATC:ATC_STK SendSsCnf,MN_SMS_OPERATE_SUCCESS")
TRACE_MSG(ATC_GSM_STK_2673_112_2_17_22_13_55_1133,"ATC:ATC_STK SendSsCnf,MN_SS_FAIL")
TRACE_MSG(ATC_GSM_STK_2700_112_2_17_22_13_55_1134,"ATC:ATC_STKProcessSendSsRegPwdCnf,MN_SMS_OPERATE_SUCCESS")
TRACE_MSG(ATC_GSM_STK_2709_112_2_17_22_13_55_1135,"ATC:ATC_STKProcessSendSsRegPwdCnf,MN_SS_FAIL")
TRACE_MSG(ATC_GSM_STK_2735_112_2_17_22_13_55_1136,"[ATC_MainMenuCnf]::n_select= %d")
TRACE_MSG(ATC_GSM_STK_2740_112_2_17_22_13_55_1137,"ATC: ATC_MainMenuCnf,ERROR,s_stk_mainmenu_ptr is NULL.")
TRACE_MSG(ATC_GSM_STK_2775_112_2_17_22_13_55_1138,"ATC:ATC_MainMenuCnf=%d")
TRACE_MSG(ATC_GSM_STK_2866_112_2_17_22_13_55_1139,"ATC:ATC_CheckAndHandleGetInkey,SIMAT_DIGIT_ONLY,%d")
TRACE_MSG(ATC_GSM_STK_2879_112_2_17_22_13_55_1140,"ATC:ATC_CheckAndHandleGetInkey,SIMAT_SMS_ALPHABET,%d")
TRACE_MSG(ATC_GSM_STK_2897_112_2_17_22_13_55_1141,"ATC:ATC_CheckAndHandleGetInkey,SIMAT_UCS2_ALPHABET,%d")
TRACE_MSG(ATC_GSM_STK_2923_112_2_17_22_13_56_1142,"ATC:ATC_CheckAndHandleGetInkey,SIMAT_UCS2_ALPHABET,%d")
TRACE_MSG(ATC_GSM_STK_2935_112_2_17_22_13_56_1143,"ATC: re_value=%d")
TRACE_MSG(ATC_GSM_STK_2984_112_2_17_22_13_56_1144,"[ATC_GetInkeyCnf]::ATC_CheckAndHandleGetInkey==FALSE")
TRACE_MSG(ATC_GSM_STK_3001_112_2_17_22_13_56_1145,"[ATC_GetInkeyCnf]::re_value=%d")
TRACE_MSG(ATC_GSM_STK_3053_112_2_17_22_13_56_1146,"ATC: getinput type = %d, str_len = %d, min_len = %d max_len = %d")
TRACE_MSG(ATC_GSM_STK_3067_112_2_17_22_13_56_1147,"ATC:error in length")
TRACE_MSG(ATC_GSM_STK_3082_112_2_17_22_13_56_1148,"ATC:ATC_CheckAndHandleGetInput,SIMAT_DIGIT_ONLY,%d")
TRACE_MSG(ATC_GSM_STK_3089_112_2_17_22_13_56_1149,"ATC:error in length")
TRACE_MSG(ATC_GSM_STK_3099_112_2_17_22_13_56_1150,"ATC:ATC_CheckAndHandleGetInput,SIMAT_INPUT_SMS_ALPHABET,%d")
TRACE_MSG(ATC_GSM_STK_3107_112_2_17_22_13_56_1151,"ATC:error in length")
TRACE_MSG(ATC_GSM_STK_3121_112_2_17_22_13_56_1152,"ATC:SIMAT_INPUT_UCS2_ALPHABET,%d")
TRACE_MSG(ATC_GSM_STK_3128_112_2_17_22_13_56_1153,"ATC:error in length")
TRACE_MSG(ATC_GSM_STK_3141_112_2_17_22_13_56_1154,"ATC:SIMAT_INPUT_DUAL_DIGIT,%d")
TRACE_MSG(ATC_GSM_STK_3186_112_2_17_22_13_56_1155,"[ATC_GetInputCnf]::enter ATC_GetInputCnf,%d,%d")
TRACE_MSG(ATC_GSM_STK_3215_112_2_17_22_13_56_1156,"[ATC_GetInputCnf]::ATC_CheckAndHandleGetInput==FALSE")
TRACE_MSG(ATC_GSM_STK_3373_112_2_17_22_13_56_1157,"ATC:ATC_StkProcessCallSignals, dual_sys=%d")
TRACE_MSG(ATC_GSM_STK_3378_112_2_17_22_13_56_1158,"ATC:IsCallSucc == TRUE")
TRACE_MSG(ATC_GSM_STK_3385_112_2_17_22_13_56_1159,"ATC:IsCallSucc == FALSE")
TRACE_MSG(ATC_GSM_STK_3479_112_2_17_22_13_57_1160,"ATC:ATC_HandleSetupCall atc_config_ptr->call_link_id = %d ")
TRACE_MSG(ATC_GSM_STK_3501_112_2_17_22_13_57_1161,"ATC:ATC_HandleSetupCall atc_config_ptr->call_link_id = %d ")
TRACE_MSG(ATC_GSM_STK_3538_112_2_17_22_13_57_1162,"[ATC_SetupCallCnf]::ATC_SetupCallCnf , dual_sys:%d")
TRACE_MSG(ATC_GSM_STK_3565_112_2_17_22_13_57_1163,"[ATC_SetupCallCnf]::ATC_SetupCallCnf,ATC_SETUP_CALL")
TRACE_MSG(ATC_GSM_STK_3606_112_2_17_22_13_57_1164,"ATC_SelectItemHandle n_select %d")
TRACE_MSG(ATC_GSM_STK_3612_112_2_17_22_13_57_1165,"ATC_SelectItemHandle select_item.item_id %d")
TRACE_MSG(ATC_GSM_STK_3748_112_2_17_22_13_57_1166,"bin.li add sim_terminal_profile!!!!!!")
TRACE_MSG(ATC_GSM_STK_3772_112_2_17_22_13_57_1167,"ATC:: ATC_StringToByte pointer is NULL!")
TRACE_MSG(ATC_GSM_STK_3823_112_2_17_22_13_57_1168,"ATC:: invalid number:%x")
TRACE_MSG(ATC_GSM_STK_3859_112_2_17_22_13_57_1169,"ATC:before SIMAT_GetMeProfile")
TRACE_MSG(ATC_GSM_STK_3863_112_2_17_22_13_57_1170,"result_terminal_profile[%d]:%x ")
TRACE_MSG(ATC_GSM_STK_3866_112_2_17_22_13_57_1171,"add length:%d")
TRACE_MSG(ATC_GSM_STK_3887_112_2_17_22_13_57_1172," ATC:ATC_STK_DEACTIVATE")
TRACE_MSG(ATC_GSM_STK_3890_112_2_17_22_13_57_1173,"ATC:TRUE == s_stk_signal[g_current_card_id].is_support_stk")
TRACE_MSG(ATC_GSM_STK_3918_112_2_17_22_13_57_1174,"ATC: stk deactivate fail.")
TRACE_MSG(ATC_GSM_STK_3924_112_2_17_22_13_58_1175,"ATC:return_value = ERR_OPERATION_NOT_ALLOWED")
TRACE_MSG(ATC_GSM_STK_3930_112_2_17_22_13_58_1176," ATC_STK_ACTIVATE !s_stk_signal.is_support_stk:%d")
TRACE_MSG(ATC_GSM_STK_3966_112_2_17_22_13_58_1177,"ATC:FALSE == s_stk_signal[g_current_card_id].is_support_stk")
TRACE_MSG(ATC_GSM_STK_3973_112_2_17_22_13_58_1178," ATC_STK_CONFIGURE!")
TRACE_MSG(ATC_GSM_STK_3981_112_2_17_22_13_58_1179,"ATC:ERR_INVALID_INDEX")
TRACE_MSG(ATC_GSM_STK_3988_112_2_17_22_13_58_1180,"ATC: ATC_ProcessSTSF, param2 is not string!")
TRACE_MSG(ATC_GSM_STK_3993_112_2_17_22_13_58_1181,"PARA2.str_ptr->str_len: %d")
TRACE_MSG(ATC_GSM_STK_3997_112_2_17_22_13_58_1182,"ATC:ATC_CheckAndHandleConfig is false")
TRACE_MSG(ATC_GSM_STK_4116_112_2_17_22_13_58_1183,"ATC: sio stk data_len = %d")
TRACE_MSG(ATC_GSM_STK_4134_112_2_17_22_13_58_1184,"ATC:flag = %d")
TRACE_MSG(ATC_GSM_STK_4200_112_2_17_22_13_58_1185,"ATC_CheckCondAndPara1 %d,%d")
TRACE_MSG(ATC_GSM_STK_4209_112_2_17_22_13_58_1186,"para1_flag %d")
TRACE_MSG(ATC_GSM_STK_4220_112_2_17_22_13_58_1187,"ERR_INVALID_INDEX")
TRACE_MSG(ATC_GSM_STK_4251_112_2_17_22_13_58_1188,"ATC:ATC_MainMenuCnf=FALSE")
TRACE_MSG(ATC_GSM_STK_5499_112_2_17_22_14_1_1189,"ATC:s_stk_signal.cur_status=%d")
TRACE_MSG(ATC_GSM_STK_5502_112_2_17_22_14_1_1190,"ATC_GetCmdNumByString %d ")
TRACE_MSG(ATC_GSM_STK_5510_112_2_17_22_14_1_1191,"ATC_GetCmdNumByString %d ")
TRACE_MSG(ATC_GSM_STK_5515_112_2_17_22_14_1_1192,"ATC_GetCmdNumByString %d ")
TRACE_MSG(ATC_GSM_STK_5579_112_2_17_22_14_1_1193,"ATC:s_stk_signal.cur_status=%d")
TRACE_MSG(ATC_GSM_STK_5600_112_2_17_22_14_1_1194,"ATC:s_stk_signal.cur_status=%d")
TRACE_MSG(ATC_GSM_STK_5694_112_2_17_22_14_1_1195,"PARAM4_FLAG %d para4_num %d")
TRACE_MSG(ATC_GSM_STK_5995_112_2_17_22_14_2_1196,"ATC_ProcessSTED stk_actflag %d,sim_flag %d,is_support_stk %d")
TRACE_MSG(ATC_GSM_STK_6156_112_2_17_22_14_2_1197,"ATC:ATC_HandleProvidLocalInfo,info_type = %d")
TRACE_MSG(ATC_GSM_STK_6187_112_2_17_22_14_2_1198,"ATC: get imei failed.")
TRACE_MSG(ATC_GSM_STK_6246_112_2_17_22_14_2_1199,"ATC_ProcessUpSTIN dual_sys:%d")
TRACE_MSG(ATC_GSM_STK_6254_112_2_17_22_14_2_1200,"ATC_ProcessUpSTIN SCI_SET_TIMER 1s")
TRACE_MSG(ATC_GSM_STK_6342_112_2_17_22_14_2_1201,"ATC_ProcessUpSTIN invalid status %d")
TRACE_MSG(ATC_GSM_STK_6360_112_2_17_22_14_2_1202,"ATC:ATC_ProcessSTKSignal , dual_sys:%d")
TRACE_MSG(ATC_GSM_STK_6374_112_2_17_22_14_2_1203,"ATC: ATC_ProcessSTKSignal,ERROR,s_stk_mainmenu_ptr is NULL.")
TRACE_MSG(ATC_GSM_STK_6403_112_2_17_22_14_2_1204,"ATC:STK_SETUP_MENU_REQ,%d")
TRACE_MSG(ATC_GSM_STK_6421_112_2_17_22_14_2_1205,"ATC:STK_DISPLAY_TEXT_REQ")
TRACE_MSG(ATC_GSM_STK_6440_112_2_17_22_14_3_1206,"ATC:STK_GET_INKEY_REQ")
TRACE_MSG(ATC_GSM_STK_6458_112_2_17_22_14_3_1207,"ATC:STK_GET_INPUT_REQ")
TRACE_MSG(ATC_GSM_STK_6475_112_2_17_22_14_3_1208,"ATC:STK_SETUP_CALL_REQ")
TRACE_MSG(ATC_GSM_STK_6493_112_2_17_22_14_3_1209,"ATC:STK_SELECT_ITEM_REQ")
TRACE_MSG(ATC_GSM_STK_6511_112_2_17_22_14_3_1210,"ATC:STK_SEND_SMS_REQ")
TRACE_MSG(ATC_GSM_STK_6523_112_2_17_22_14_3_1211,"ATC:SIMAT_SEND_SS_REQ")
TRACE_MSG(ATC_GSM_STK_6533_112_2_17_22_14_3_1212,"ATC:SIMAT_SEND_USSD_REQ")
TRACE_MSG(ATC_GSM_STK_6543_112_2_17_22_14_3_1213,"ATC:SIMAT_PLAY_TONE_REQ")
TRACE_MSG(ATC_GSM_STK_6553_112_2_17_22_14_3_1214,"ATC:SIMAT_PROVIDE_LOCAL_INFO")
TRACE_MSG(ATC_GSM_STK_6558_112_2_17_22_14_3_1215,"ATC:STK_SESSION_TERMINATE_REQ")
TRACE_MSG(ATC_GSM_STK_6589_112_2_17_22_14_3_1216,"ATC:SIMAT_SETUP_EVENT_LIST_REQ")
TRACE_MSG(ATC_GSM_STK_6599_112_2_17_22_14_3_1217,"ATC:EV_MN_APP_SIMAT_SETUP_IDLE_TEXT_IND_F")
TRACE_MSG(ATC_GSM_STK_6617_112_2_17_22_14_3_1218,"ATC:EV_MN_APP_SIMAT_SEND_DTMF_IND_F")
TRACE_MSG(ATC_GSM_STK_6636_112_2_17_22_14_3_1219,"ATC:APP_MN_SIMAT_LANG_NOTIFY_IND")
TRACE_MSG(ATC_GSM_STK_6641_112_2_17_22_14_3_1220,"ATC:SIMAT_EVENT_REFRESH")
TRACE_MSG(ATC_GSM_STK_6645_112_2_17_22_14_3_1221,"ATC:SIMAT_EVENT_CC_RSP")
TRACE_MSG(ATC_GSM_STK_6649_112_2_17_22_14_3_1222,"ATC:SIMAT_EVENT_SS_CON_RSP")
TRACE_MSG(ATC_GSM_STK_6653_112_2_17_22_14_3_1223,"ATC:SIMAT_EVENT_USSD_CON_RSP")
TRACE_MSG(ATC_GSM_STK_6657_112_2_17_22_14_3_1224,"ATC:APP_MN_SIMAT_SMS_CTL_TO_MMI_IND")
TRACE_MSG(ATC_GSM_STK_6661_112_2_17_22_14_3_1225,"ATC:SIMAT_EVENT_REFRESH")
TRACE_MSG(ATC_GSM_STK_6665_112_2_17_22_14_3_1226,"ATC:SIMAT_EVENT_SMS_PP_RSP")
TRACE_MSG(ATC_GSM_STK_6668_112_2_17_22_14_3_1227,"ATC:SIMAT_REFRSH_IND")
TRACE_MSG(ATC_GSM_STK_6680_112_2_17_22_14_3_1228,"ATC:APP_MN_SIMAT_LAUNCH_BROWSER_IND")
TRACE_MSG(ATC_GSM_STK_6691_112_2_17_22_14_3_1229,"ATC:APP_MN_SIMAT_SEND_DATA_IND")
TRACE_MSG(ATC_GSM_STK_6704_112_2_17_22_14_3_1230,"ATC:APP_MN_SIMAT_RECEIVE_DATA_IND")
TRACE_MSG(ATC_GSM_STK_6714_112_2_17_22_14_3_1231,"ATC:APP_SIMAT_OPEN_CHANNEL_IND")
TRACE_MSG(ATC_GSM_STK_6728_112_2_17_22_14_3_1232,"ATC:APP_SIMAT_CLOSE_CHANNEL_IND")
TRACE_MSG(ATC_GSM_STK_6739_112_2_17_22_14_3_1233,"ATC:APP_SIMAT_GET_CHANNEL_STATUS_IND")
TRACE_MSG(ATC_GSM_STK_6765_112_2_17_22_14_3_1234,"ATC_ProcessTimerEXp s_stk_signal.cur_status %d ")
TRACE_MSG(ATC_GSM_STK_6779_112_2_17_22_14_3_1235,"ATC_ProcessTimerEXp SCI_SET_TIMER 1s")
TRACE_MSG(ATC_GSM_STK_6842_112_2_17_22_14_3_1236,"ATC_STK_HandleSocketGetData_Timer times = %d")
TRACE_MSG(ATC_GSM_STK_6877_112_2_17_22_14_3_1237," ATCSTK_GetSocketData = %ld, channel id= %d")
TRACE_MSG(ATC_GSM_STK_6922_112_2_17_22_14_3_1238,"ATCSTK_GetSocketData Create timer")
TRACE_MSG(ATC_GSM_STK_6940_112_2_17_22_14_4_1239,"ata_dest_address.addr_len = %d, type = %d, dest_address = %s")
TRACE_MSG(ATC_GSM_STK_6946_112_2_17_22_14_4_1240,"port = %d, trans_level_type = %d")
TRACE_MSG(ATC_GSM_STK_6975_112_2_17_22_14_4_1241,"ATC_STK_HandleConnectSocket_Timer ConnectSocket success!")
TRACE_MSG(ATC_GSM_STK_6984_112_2_17_22_14_4_1242,"ATC_STK_HandleConnectSocket_Timer ConnectSocket error!")
TRACE_MSG(ATC_GSM_STK_7011_112_2_17_22_14_4_1243,"ATCSTK_ConnectSocket  is established, sock=%ld")
TRACE_MSG(ATC_GSM_STK_7016_112_2_17_22_14_4_1244,"ATCSTK_ConnectSocket sci_sock_connect, err = %d")
TRACE_MSG(ATC_GSM_STK_7033_112_2_17_22_14_4_1245,"ATCSTK_ConnectSocket, sock=%d, return=%d")
TRACE_MSG(ATC_GSM_STK_7067_112_2_17_22_14_4_1246,"ATC_Status_OpenChannel sci_sock_socket error = %d")
TRACE_MSG(ATC_GSM_STK_7078_112_2_17_22_14_4_1247,"ATC_Status_OpenChannel sci_sock_setsockopt, result=%d")
TRACE_MSG(ATC_GSM_STK_7127_112_2_17_22_14_4_1248,"tp_pdp_writedata error= %d")
TRACE_MSG(ATC_GSM_STK_7131_112_2_17_22_14_4_1249,"tp_pdp_writedata, s=%d, return %d")
TRACE_MSG(ATC_GSM_STK_OMS_436_112_2_17_22_14_5_1250,"ATC:ATC_OmsInitCnfSetMenu, dual_sys:%d")
TRACE_MSG(ATC_GSM_STK_OMS_701_112_2_17_22_14_5_1251,"ATC: ATC_ProcessSendSMS SC is exist.")
TRACE_MSG(ATC_GSM_STK_OMS_712_112_2_17_22_14_5_1252,"ATC: ATC_ProcessSendSMS sc_addr.length = %d")
TRACE_MSG(ATC_GSM_STK_OMS_719_112_2_17_22_14_5_1253,"ATC: ATC_ProcessSendSMS sc_addr.length = %d")
TRACE_MSG(ATC_GSM_STK_OMS_724_112_2_17_22_14_5_1254,"ATC: address is not exist.")
TRACE_MSG(ATC_GSM_STK_OMS_732_112_2_17_22_14_5_1255,"ATC: send sms pdu len error !.")
TRACE_MSG(ATC_GSM_STK_OMS_750_112_2_17_22_14_6_1256,"ATC: SMS is packed.")
TRACE_MSG(ATC_GSM_STK_OMS_754_112_2_17_22_14_6_1257,"ATC: send sms pdu err code = %d.")
TRACE_MSG(ATC_GSM_STK_OMS_766_112_2_17_22_14_6_1258,"ATC: SMS is unpacked.")
TRACE_MSG(ATC_GSM_STK_OMS_771_112_2_17_22_14_6_1259,"ATC: send sms pdu err code = %d.")
TRACE_MSG(ATC_GSM_STK_OMS_958_112_2_17_22_14_6_1260,"ATC: user originate ussd fail, err code = %d.")
TRACE_MSG(ATC_GSM_STK_OMS_1016_112_2_17_22_14_6_1261,"ATC:EV_MN_APP_SIMAT_SETUP_IDLE_TEXT_IND_F")
TRACE_MSG(ATC_GSM_STK_OMS_1100_112_2_17_22_14_6_1262,"ATC:ATC_OmsSTKProcessSendSmsCnf,MN_SMS_OPERATE_SUCCESS")
TRACE_MSG(ATC_GSM_STK_OMS_1109_112_2_17_22_14_6_1263,"ATC:ATC_OmsSTKProcessSendSmsCnf,MN_SMS_FAIL")
TRACE_MSG(ATC_GSM_STK_OMS_1140_112_2_17_22_14_6_1264,"ATC:ATC_STKProcessSendUssdCnf,MN_SMS_OPERATE_SUCCESS")
TRACE_MSG(ATC_GSM_STK_OMS_1150_112_2_17_22_14_6_1265,"ATC:ATC_STKProcessSendUssdCnf,MN_USSD_FAIL")
TRACE_MSG(ATC_GSM_STK_OMS_1177_112_2_17_22_14_7_1266,"ATC:ATC_OmsSTKProcessSendSsCnf,MN_SMS_OPERATE_SUCCESS")
TRACE_MSG(ATC_GSM_STK_OMS_1188_112_2_17_22_14_7_1267,"ATC:ATC_OmsSTKProcessSendSsCnf,MN_SS_FAIL")
TRACE_MSG(ATC_GSM_STK_OMS_1215_112_2_17_22_14_7_1268,"ATC:ATC_STKProcessSendSsRegPwdCnf,MN_SMS_OPERATE_SUCCESS")
TRACE_MSG(ATC_GSM_STK_OMS_1224_112_2_17_22_14_7_1269,"ATC:ATC_STKProcessSendSsRegPwdCnf,MN_SS_FAIL")
TRACE_MSG(ATC_GSM_STK_OMS_1318_112_2_17_22_14_7_1270,"ATC: re_value=%d")
TRACE_MSG(ATC_GSM_STK_OMS_1386_112_2_17_22_14_7_1271,"ATC:IsCallSucc == TRUE")
TRACE_MSG(ATC_GSM_STK_OMS_1394_112_2_17_22_14_7_1272,"ATC:IsCallSucc == FALSE")
TRACE_MSG(ATC_GSM_STK_OMS_1889_112_2_17_22_14_8_1273,"ATC: ATC_ProcessRefreshInd, refresh_type = %d,is_files_exist=%d")
TRACE_MSG(ATC_GSM_STK_OMS_1923_112_2_17_22_14_8_1274,"ATC: ATC_ProcessRefreshCnf, is_ok=%d,is_file_changed=%d")
TRACE_MSG(ATC_GSM_STK_OMS_1960_112_2_17_22_14_8_1275,"ATC: ATC_ProcessSendData, channel id:%d")
TRACE_MSG(ATC_GSM_STK_OMS_1992_112_2_17_22_14_8_1276,"ATC: ATC_ProcessReceiveData, channel id:%d")
TRACE_MSG(ATC_GSM_STK_OMS_2079_112_2_17_22_14_8_1277,"ATC: ATC_ProcessCloseChannel, channel_id = %d")
TRACE_MSG(ATC_GSM_STK_OMS_2134_112_2_17_22_14_8_1278,"ATC:: ATC_OmsStringToByte pointer is NULL!")
TRACE_MSG(ATC_GSM_STK_OMS_2185_112_2_17_22_14_9_1279,"ATC:: invalid number:%x")
TRACE_MSG(ATC_GSM_STK_OMS_2226_112_2_17_22_14_9_1280,"Param1  invalid\n")
TRACE_MSG(ATC_GSM_STK_OMS_2310_112_2_17_22_14_9_1281,"ATC:ATC_ProcessSPUSATCALLSETUP atc_config_ptr->call_link_id = %d ")
TRACE_MSG(ATC_GSM_STK_OMS_2335_112_2_17_22_14_9_1282,"ATC:ATC_ProcessSPUSATCALLSETUP atc_config_ptr->call_link_id = %d ")
TRACE_MSG(ATC_GSM_STK_OMS_2379_112_2_17_22_14_9_1283,"ATC:Param1  invalid\n")
TRACE_MSG(ATC_GSM_STK_OMS_2382_112_2_17_22_14_9_1284,"ATC:+SPUSATPROFILE: Param1 length=%d\n")
TRACE_MSG(ATC_GSM_STK_OMS_2386_112_2_17_22_14_9_1285,"ATC:+SPUSATPROFILE: Param1 is not string or string length is too long\n")
TRACE_MSG(ATC_GSM_STK_OMS_2392_112_2_17_22_14_9_1286,"ATC:+SPUSATPROFILE,s_ap_is_active is already TRUE.")
TRACE_MSG(ATC_GSM_STK_OMS_2403_112_2_17_22_14_9_1287,"ATC:+SPUSATPROFILE, String to Byte  error\n")
TRACE_MSG(ATC_GSM_STK_OMS_2420_112_2_17_22_14_9_1288,"ATC:+SPUSATPROFILE,Profile download :s_stk_signal.is_support_stk:%d")
TRACE_MSG(ATC_GSM_STK_OMS_2445_112_2_17_22_14_9_1289,"ATC: SPUSATCAPREQ command is read command")
TRACE_MSG(ATC_GSM_STK_OMS_2489_112_2_17_22_14_9_1290,"ATC:+SPUSATENVECMD,s_ap_is_active:%d==FALSE or s_stk_is_active:%d==FALSE or s_stk_is_pending==TRUE")
TRACE_MSG(ATC_GSM_STK_OMS_2514_112_2_17_22_14_9_1291,"ATC: SPUSATENVECMD set command ")
TRACE_MSG(ATC_GSM_STK_OMS_2522_112_2_17_22_14_9_1292,"SPUSATENVECMD event_tag =%x ")
TRACE_MSG(ATC_GSM_STK_OMS_2530_112_2_17_22_14_9_1293,"ATC  SPUSATENVECMD Menu Selection\n")
TRACE_MSG(ATC_GSM_STK_OMS_2538_112_2_17_22_14_9_1294,"SPUSATENVECMD set cmd:stk current status is not setupmenu")
TRACE_MSG(ATC_GSM_STK_OMS_2545_112_2_17_22_14_9_1295,"SPUSATENVECMD set cmd: tag is invalid")
TRACE_MSG(ATC_GSM_STK_OMS_2599_112_2_17_22_14_9_1296,"ATC:+SPUSATTERMINAL,s_ap_is_active:%d==FALSE or s_stk_is_active:%d==FALSE")
TRACE_MSG(ATC_GSM_STK_OMS_2635_112_2_17_22_14_9_1297,"ATC: terminal set command ")
TRACE_MSG(ATC_GSM_STK_OMS_2790_112_2_17_22_14_10_1298,"ATC:ATC_HandleProvidLocalInfo,info_type = %d")
TRACE_MSG(ATC_GSM_STK_OMS_2818_112_2_17_22_14_10_1299,"ATC: get imei failed.")
TRACE_MSG(ATC_GSM_STK_OMS_2882_112_2_17_22_14_10_1300,"ATC:ATC_OmsProcessSTKSignal,s_ap_is_active:%d,s_stk_is_pending:%d")
TRACE_MSG(ATC_GSM_STK_OMS_2932_112_2_17_22_14_10_1301,"ATC:SIMAT_PROVIDE_LOCAL_INFO")
TRACE_MSG(ATC_GSM_STK_OMS_2937_112_2_17_22_14_10_1302,"ATC:STK_SESSION_TERMINATE_REQ")
TRACE_MSG(ATC_GSM_STK_OMS_2962_112_2_17_22_14_10_1303,"ATC:APP_MN_SIMAT_LANG_NOTIFY_IND")
TRACE_MSG(ATC_GSM_STK_OMS_2993_112_2_17_22_14_10_1304,"ATC:SIMAT_EVENT_CC_RSP")
TRACE_MSG(ATC_GSM_STK_OMS_2997_112_2_17_22_14_10_1305,"ATC:SIMAT_EVENT_SS_CON_RSP")
TRACE_MSG(ATC_GSM_STK_OMS_3001_112_2_17_22_14_10_1306,"ATC:SIMAT_EVENT_USSD_CON_RSP")
TRACE_MSG(ATC_GSM_STK_OMS_3005_112_2_17_22_14_10_1307,"ATC:SIMAT_EVENT_SMS_CON_RSP")
TRACE_MSG(ATC_GSM_STK_OMS_3009_112_2_17_22_14_10_1308,"ATC:SIMAT_EVENT_REFRESH")
TRACE_MSG(ATC_GSM_STK_OMS_3013_112_2_17_22_14_10_1309,"ATC:SIMAT_EVENT_SMS_PP_RSP")
TRACE_MSG(ATC_GSM_STK_OMS_3037_112_2_17_22_14_10_1310,"ATC_OmsSignalCacheFreeSize:size=%d\n")
TRACE_MSG(ATC_GSM_STK_OMS_3051_112_2_17_22_14_10_1311,"ATC_OmsSignalCacheSignalNum:size=%d\n")
TRACE_MSG(ATC_GSM_STK_OMS_3114_112_2_17_22_14_10_1312,"ATC:Status error")
TRACE_MSG(ATC_GSM_STK_OMS_3119_112_2_17_22_14_10_1313,"ATC :Sig_code=0x%04x")
TRACE_MSG(ATC_GSM_STK_OMS_3224_112_2_17_22_14_11_1314,"ATC:SIMAT_PROVIDE_LOCAL_INFO")
TRACE_MSG(ATC_GSM_STK_OMS_3264_112_2_17_22_14_11_1315,"ATC:EV_MN_APP_SIMAT_LANG_NOTIFY_IND_F")
TRACE_MSG(ATC_GSM_STK_OMS_3360_112_2_17_22_14_11_1316,"ATC:Status error or NO Pending Event")
TRACE_MSG(ATC_GSM_STK_OMS_3366_112_2_17_22_14_11_1317,"ATC:No pending Event\n")
TRACE_MSG(ATC_GSM_STK_OMS_3374_112_2_17_22_14_11_1318,"ATC:Sig_Ptr error")
TRACE_MSG(ATC_GSM_STK_OMS_3404_112_2_17_22_14_11_1319,"ATC: signal code:0x%04x, do nothing.")
TRACE_MSG(ATC_GSM_STK_OMS_3631_112_2_17_22_14_11_1320,"bin.li add sim_terminal_profile!!!!!!")
TRACE_MSG(ATC_GSM_STK_OMS_3641_112_2_17_22_14_11_1321,"ATC:ATC_CheckAndHandleConfig,re_value=%d")
TRACE_MSG(ATC_GSM_STK_OMS_3660_112_2_17_22_14_11_1322,"ATC:ATC_ProcessSPSTPARAM, param1 and param2 are not both present.")
TRACE_MSG(ATC_GSM_STK_OMS_3665_112_2_17_22_14_11_1323,"ATC:ATC_ProcessSPSTPARAM, param1=%d,param2=%d")
TRACE_MSG(ATC_GSM_STK_OMS_3688_112_2_17_22_14_12_1324,"ATC:ATC_ProcessSPSTPARAM, param1 is not support.")
TRACE_MSG(ATC_GSM_STK_OMS_3718_112_2_17_22_14_12_1325,"ATC: ATC_ProcessRefreshCmd, refresh_type = %d ")
TRACE_MSG(ATC_GSM_STK_OMS_3727_112_2_17_22_14_12_1326,"ATC: ATC_ProcessRefreshCmd, refresh cmd execute failure!")
TRACE_MSG(ATC_GSM_STK_OMS_3737_112_2_17_22_14_12_1327,"ATC: ATC_ProcessRefreshCmd, refresh cmd execute failure!")
TRACE_MSG(ATC_GSM_STK_OMS_3742_112_2_17_22_14_12_1328,"ATC: ATC_ProcessRefreshCmd, current refresh type is not support yet!")
TRACE_MSG(ATC_GSM_STK_OMS_3794_112_2_17_22_14_12_1329,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_ACTIVE_PS, param2=%d,param3=%d, one of them is invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_3800_112_2_17_22_14_12_1330,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_ACTIVE_PS, param2 and param3 are not both valid.")
TRACE_MSG(ATC_GSM_STK_OMS_3834_112_2_17_22_14_12_1331,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_SET_EXT_PROFILE, param2,param3 are not both valid.")
TRACE_MSG(ATC_GSM_STK_OMS_3854_112_2_17_22_14_12_1332,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_STARTUP_PS, param2 are not valid.")
TRACE_MSG(ATC_GSM_STK_OMS_3860_112_2_17_22_14_12_1333,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_STARTUP_PS, param2 are not inputed.")
TRACE_MSG(ATC_GSM_STK_OMS_3893_112_2_17_22_14_12_1334,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_STARTUP_PS, param2 are not valid.")
TRACE_MSG(ATC_GSM_STK_OMS_3911_112_2_17_22_14_12_1335,"ATC: PHONEBOOK_Get3GPhonebookStatus fail, code: %d")
TRACE_MSG(ATC_GSM_STK_OMS_3968_112_2_17_22_14_12_1336,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_MALLOC_MEMORY, SUCCESS.")
TRACE_MSG(ATC_GSM_STK_OMS_3972_112_2_17_22_14_12_1337,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_MALLOC_MEMORY, param2 are not valid.")
TRACE_MSG(ATC_GSM_STK_OMS_3978_112_2_17_22_14_12_1338,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_MALLOC_MEMORY, param2 are not inputed.")
TRACE_MSG(ATC_GSM_STK_OMS_4005_112_2_17_22_14_12_1339,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_GETPLMNNAME, param3 or param4 are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4017_112_2_17_22_14_12_1340,"ATC_ProcessSPSTACTION,MNPHONE_SelectGSMBandEx,fail.")
TRACE_MSG(ATC_GSM_STK_OMS_4021_112_2_17_22_14_12_1341,"ATC_ProcessSPSTACTION,MNPHONE_SelectGSMBandEx,success.")
TRACE_MSG(ATC_GSM_STK_OMS_4026_112_2_17_22_14_12_1342,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_SELGSMBAND, param3 are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4038_112_2_17_22_14_12_1343,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_SETGCFCALIWITHSIM, param3 are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4070_112_2_17_22_14_12_1344,"ATC_ProcessSPSTACTION,MNSIM_GetEFInfo,fail.")
TRACE_MSG(ATC_GSM_STK_OMS_4074_112_2_17_22_14_12_1345,"ATC_ProcessSPSTACTION,MNSIM_GetEFInfo,success.")
TRACE_MSG(ATC_GSM_STK_OMS_4079_112_2_17_22_14_12_1346,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_GETEFINFO, param3 or param4 are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4098_112_2_17_22_14_12_1347,"ATC_ProcessSPSTACTION,MNSIM_ReadSimEF,fail.")
TRACE_MSG(ATC_GSM_STK_OMS_4102_112_2_17_22_14_12_1348,"ATC_ProcessSPSTACTION,MNSIM_ReadSimEF,success.")
TRACE_MSG(ATC_GSM_STK_OMS_4107_112_2_17_22_14_12_1349,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_READSIMEF, param3 or param4 are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4131_112_2_17_22_14_12_1350,"ATC_ProcessSPSTACTION,MNSIM_WriteSimEF,fail.")
TRACE_MSG(ATC_GSM_STK_OMS_4135_112_2_17_22_14_12_1351,"ATC_ProcessSPSTACTION,MNSIM_WriteSimEF,success.")
TRACE_MSG(ATC_GSM_STK_OMS_4140_112_2_17_22_14_12_1352,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_GETSIMFILEINFO, param3 are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4176_112_2_17_22_14_13_1353,"ATC_ProcessSPSTACTION,MNSIM_ReadSimRecordFile,fail.")
TRACE_MSG(ATC_GSM_STK_OMS_4180_112_2_17_22_14_13_1354,"ATC_ProcessSPSTACTION,MNSIM_ReadSimRecordFile,success.")
TRACE_MSG(ATC_GSM_STK_OMS_4185_112_2_17_22_14_13_1355,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_READSIMRECORDFILE, params are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4246_112_2_17_22_14_13_1356,"ATC_ProcessSPSTACTION,MNSIM_UpdateSimRecordFile,fail.")
TRACE_MSG(ATC_GSM_STK_OMS_4250_112_2_17_22_14_13_1357,"ATC_ProcessSPSTACTION,MNSIM_UpdateSimRecordFile,success.")
TRACE_MSG(ATC_GSM_STK_OMS_4255_112_2_17_22_14_13_1358,"ATC:ATC_ProcessSPSTACTION,MNSIM_UpdateSimRecordFile, param3 are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4296_112_2_17_22_14_13_1359,"ATC_ProcessSPSTACTION,MNSIM_SearchSimRecord,fail.")
TRACE_MSG(ATC_GSM_STK_OMS_4300_112_2_17_22_14_13_1360,"ATC_ProcessSPSTACTION,MNSIM_SearchSimRecord,success.")
TRACE_MSG(ATC_GSM_STK_OMS_4305_112_2_17_22_14_13_1361,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_SEARCHSIMRECORD, params are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4321_112_2_17_22_14_13_1362,"ATC_ProcessSPSTACTION,MNSIM_SetSimEFService,fail.")
TRACE_MSG(ATC_GSM_STK_OMS_4325_112_2_17_22_14_13_1363,"ATC_ProcessSPSTACTION,MNSIM_SetSimEFService,success.")
TRACE_MSG(ATC_GSM_STK_OMS_4330_112_2_17_22_14_13_1364,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_SETSIMEFSERVICE, param3 or param4 are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4348_112_2_17_22_14_13_1365,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_GETSIMEFSERVICE, param3 are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4366_112_2_17_22_14_13_1366,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_GETSIMFILEINFO, param3 are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4393_112_2_17_22_14_13_1367,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_GETSIMATRRAWDATA, param3 are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4409_112_2_17_22_14_13_1368,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_SETSAVESMS, param3 is invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4467_112_2_17_22_14_13_1369,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_INDMMIMOCC, param3 are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4505_112_2_17_22_14_13_1370,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_STARTCALLNOCC, param3 are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4576_112_2_17_22_14_13_1371,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_INDMMISMSC, param3 are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4636_112_2_17_22_14_13_1372,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_APPSENDSMSNOCC, param3 are invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4647_112_2_17_22_14_13_1373,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_SAMSUNG_INTERFACE_TEST, param2 are not valid.")
TRACE_MSG(ATC_GSM_STK_OMS_4684_112_2_17_22_14_14_1374,"ueit nvm: read nvm_read_of_gsm suc")
TRACE_MSG(ATC_GSM_STK_OMS_4697_112_2_17_22_14_14_1375,"ueit nvm: read nvm_read_of_gsm fail.")
TRACE_MSG(ATC_GSM_STK_OMS_4705_112_2_17_22_14_14_1376,"ueit nvm: read nvm_read_of_gsm begin")
TRACE_MSG(ATC_GSM_STK_OMS_4733_112_2_17_22_14_14_1377,"ueit nvm: invalid parameter")
TRACE_MSG(ATC_GSM_STK_OMS_4739_112_2_17_22_14_14_1378,"ATC:ATC_ProcessSPSTACTION,ST_ACTION_UEIT_NV, one of them is invalid.")
TRACE_MSG(ATC_GSM_STK_OMS_4794_112_2_17_22_14_14_1379,"ATC: AT+SPUEITNVM input nv item data length error.length:%d != %d")
TRACE_MSG(ATC_GSM_STK_OMS_4807_112_2_17_22_14_14_1380,"ATC: cancel the SPUEITNVM data.")
TRACE_MSG(ATC_GSM_STK_OMS_4811_112_2_17_22_14_14_1381,"ATC: snvm data format is error.")
TRACE_MSG(ATC_MALLOC_254_112_2_17_22_14_36_1382,"Add heap success, heap size = %d.\n")
TRACE_MSG(ATC_PHONEBOOK_182_112_2_17_22_14_44_1383,"ATC: CPBS, param1 is not string!")
TRACE_MSG(ATC_PHONEBOOK_324_112_2_17_22_14_44_1384,"ATC: SIM/USIM is not ready.")
TRACE_MSG(ATC_PHONEBOOK_334_112_2_17_22_14_44_1385,"ATC: +CPBR, no phonebook entity.")
TRACE_MSG(ATC_PHONEBOOK_351_112_2_17_22_14_44_1386,"ATC: CPBR: index:%d is out of range(1-%d)")
TRACE_MSG(ATC_PHONEBOOK_365_112_2_17_22_14_44_1387,"ATC: no phonebook record.")
TRACE_MSG(ATC_PHONEBOOK_490_112_2_17_22_14_44_1388,"ATC: ATC_ProcessCPBF, param1 is not string!")
TRACE_MSG(ATC_PHONEBOOK_513_112_2_17_22_14_44_1389,"ATC:ConvertAscUcs2IntoBin IS TRUE")
TRACE_MSG(ATC_PHONEBOOK_518_112_2_17_22_14_44_1390,"ATC:ConvertAscUcs2IntoBin is FALSE")
TRACE_MSG(ATC_PHONEBOOK_559_112_2_17_22_14_44_1391,"ATC:ConvertAscUcs2IntoBin")
TRACE_MSG(ATC_PHONEBOOK_609_112_2_17_22_14_45_1392,"ATC: SIM/USIM is not ready.")
TRACE_MSG(ATC_PHONEBOOK_618_112_2_17_22_14_45_1393,"ATC: +CPBW, no phonebook entity.")
TRACE_MSG(ATC_PHONEBOOK_632_112_2_17_22_14_45_1394,"ATC: CPBW: index:%d is out of range(1-%d)")
TRACE_MSG(ATC_PHONEBOOK_649_112_2_17_22_14_45_1395,"ATC: CPBW: no index")
TRACE_MSG(ATC_PHONEBOOK_663_112_2_17_22_14_45_1396,"ATC:CPBW: memory full.")
TRACE_MSG(ATC_PHONEBOOK_673_112_2_17_22_14_45_1397,"ATC: CPBW, <number> is not string!")
TRACE_MSG(ATC_PHONEBOOK_728_112_2_17_22_14_45_1398,"ATC: CPBW, param4 is not string!")
TRACE_MSG(ATC_PHONEBOOK_744_112_2_17_22_14_45_1399,"ATC:cpbw,tel_alpha=,%x,%x,%x,str_len,%x")
TRACE_MSG(ATC_PHONEBOOK_750_112_2_17_22_14_45_1400,"ATC:CPBW,ATC_UCS2_CODE == *entry.alpha")
TRACE_MSG(ATC_PHONEBOOK_782_112_2_17_22_14_45_1401,"ATC:cpbw,PARA4.str_ptr=%x,%x,%x")
TRACE_MSG(ATC_PHONEBOOK_784_112_2_17_22_14_45_1402,"ATC:cpbw,entry.alpha=%x,%x,%x")
TRACE_MSG(ATC_PHONEBOOK_834_112_2_17_22_14_45_1403,"ATC:ATC_ProcessDelItemCnf, delete entry(id=%d) by ATC.")
TRACE_MSG(ATC_PHONEBOOK_850_112_2_17_22_14_45_1404,"ATC:ATC_ProcessDelItemCnf, delete entry(id=%d) by MMI.")
TRACE_MSG(ATC_PHONEBOOK_890_112_2_17_22_14_45_1405,"ATC: CPBP, param1 is not string!")
TRACE_MSG(ATC_PHONEBOOK_1196_112_2_17_22_14_46_1406,"ATC: ATC_CMD_TYPE_TEST")
TRACE_MSG(ATC_PHONEBOOK_1276_112_2_17_22_14_46_1407,"ATC:PHONEBOOK_ReadVoiceMailAddr IS OK")
TRACE_MSG(ATC_PHONEBOOK_1288_112_2_17_22_14_46_1408,"ATC: PHONEBOOK_ReadVoiceMailAddr IS ERROR!")
TRACE_MSG(ATC_PHONEBOOK_1301_112_2_17_22_14_46_1409,"ATC:PHONEBOOK_EnableVoiceMailAddr failed")
TRACE_MSG(ATC_PHONEBOOK_1317_112_2_17_22_14_46_1410,"ATC: CSVM, param2 is not string!")
TRACE_MSG(ATC_PHONEBOOK_1326_112_2_17_22_14_46_1411,"ATC:AT+CSVM,error in length of input")
TRACE_MSG(ATC_PHONEBOOK_1345_112_2_17_22_14_46_1412,"ATC:ERR_OPERATION_NOT_SUPPORTED")
TRACE_MSG(ATC_PHONEBOOK_1360_112_2_17_22_14_46_1413,"ATC:PHONEBOOK_UpdateVoiceMailAddr failed")
TRACE_MSG(ATC_PHONEBOOK_1367_112_2_17_22_14_46_1414,"ATC:PHONEBOOK_EnableVoiceMailAddr failed")
TRACE_MSG(ATC_PHONEBOOK_1412_112_2_17_22_14_46_1415,"ATC: ATC_CMD_TYPE_READ")
TRACE_MSG(ATC_PHONEBOOK_1419_112_2_17_22_14_46_1416,"ATC: ERR_INVALID_INDEX")
TRACE_MSG(ATC_PHONEBOOK_1432_112_2_17_22_14_46_1417,"ATC: para_ptr=atc_storage_str")
TRACE_MSG(ATC_PHONEBOOK_1498_112_2_17_22_14_47_1418,"ATC: : phonebook used records number:%d")
TRACE_MSG(ATC_PHONEBOOK_1530_112_2_17_22_14_47_1419,"ATC_AddNationChToDIAL:%d")
TRACE_MSG(ATC_PHONEBOOK_1572_112_2_17_22_14_47_1420,"ATC:HandlePhonebookRead,entry_id:%d,is_usim:%d")
TRACE_MSG(ATC_PHONEBOOK_1924_112_2_17_22_14_47_1421,"ATC: Read phonebook additional info failed.entry_id=%d,cause:%d")
TRACE_MSG(ATC_PHONEBOOK_1970_112_2_17_22_14_47_1422,"ATC:PHONEBOOK_Delete3GPhonebook failed,cause:%u")
TRACE_MSG(ATC_PHONEBOOK_1995_112_2_17_22_14_47_1423,"ATC:PHONEBOOK_DeletePhonebookEntry failed,cause:%u")
TRACE_MSG(ATC_PHONEBOOK_2050_112_2_17_22_14_48_1424,"ATC:PHONEBOOK_Update3GPhonebook failed,cause:%d")
TRACE_MSG(ATC_PHONEBOOK_2081_112_2_17_22_14_48_1425,"ATC:PHONEBOOK_UpdatePhonebookEntry failed,cause:%lu")
TRACE_MSG(ATC_PHONEBOOK_2243_112_2_17_22_14_48_1426,"ATC: ATC_ProcessUSIMReadAddCnf: read entry(id=%d) by ATC")
TRACE_MSG(ATC_PHONEBOOK_2285_112_2_17_22_14_48_1427,"ATC: read ADN entry(id:%d) failed")
TRACE_MSG(ATC_PHONEBOOK_2344_112_2_17_22_14_48_1428,"ATC: ATC_ProcessUSIMReadAddCnf: read entry(id=%d) by MMI")
TRACE_MSG(ATC_PHONEBOOK_2368_112_2_17_22_14_48_1429,"ATC: ATC_ProcessDelUserInfoCnf: delete entry(id=%d) by ATC.")
TRACE_MSG(ATC_PHONEBOOK_2381_112_2_17_22_14_48_1430,"ATC: ATC_ProcessDelUserInfoCnf: delete entry(id=%d) by MMI.")
TRACE_MSG(ATC_PHONEBOOK_2404_112_2_17_22_14_48_1431,"ATC: ATC_ProcessUpdateUserInfoCnf: update entry(id=%d) by ATC.")
TRACE_MSG(ATC_PHONEBOOK_2434_112_2_17_22_14_48_1432,"ATC: ATC_ProcessUpdateUserInfoCnf: update entry(id=%d) by MMI.")
TRACE_MSG(ATC_PHONEBOOK_2479_112_2_17_22_14_49_1433,"ATC: SIM/USIM is not ready.")
TRACE_MSG(ATC_PHONEBOOK_2517_112_2_17_22_14_49_1434,"ATC: ^SCPBW, no phonebook entity.")
TRACE_MSG(ATC_PHONEBOOK_2536_112_2_17_22_14_49_1435,"AT^SCPBW: MMI is initializing phonebook(current id:%d,total:%d).")
TRACE_MSG(ATC_PHONEBOOK_2548_112_2_17_22_14_49_1436,"ATC: SCPBW, <index> is not numeric!")
TRACE_MSG(ATC_PHONEBOOK_2555_112_2_17_22_14_49_1437,"ATC: SCPBW: index:%d is out of range(1-%d)")
TRACE_MSG(ATC_PHONEBOOK_2572_112_2_17_22_14_49_1438,"ATC: SCPBW: <index1> use default value")
TRACE_MSG(ATC_PHONEBOOK_2583_112_2_17_22_14_49_1439,"ATC: SCPBW: no index 1 and 2")
TRACE_MSG(ATC_PHONEBOOK_2590_112_2_17_22_14_49_1440,"ATC: SCPBW: memory full.")
TRACE_MSG(ATC_PHONEBOOK_2601_112_2_17_22_14_49_1441,"ATC: SCPBW, <num1> is not string!")
TRACE_MSG(ATC_PHONEBOOK_2613_112_2_17_22_14_49_1442,"ATC: SCPBW: index2 length is zero")
TRACE_MSG(ATC_PHONEBOOK_2669_112_2_17_22_14_49_1443,"ATC: SCPBW, <num2> is not string!")
TRACE_MSG(ATC_PHONEBOOK_2677_112_2_17_22_14_49_1444,"ATC: SCPBW: index4 length is zero")
TRACE_MSG(ATC_PHONEBOOK_2733_112_2_17_22_14_49_1445,"ATC: SCPBW, <num3> is not string!")
TRACE_MSG(ATC_PHONEBOOK_2741_112_2_17_22_14_49_1446,"ATC: SCPBW: index6 length is zero")
TRACE_MSG(ATC_PHONEBOOK_2796_112_2_17_22_14_49_1447,"ATC: SCPBW, <num4> is not string!")
TRACE_MSG(ATC_PHONEBOOK_2804_112_2_17_22_14_49_1448,"ATC: SCPBW: index8 length is zero")
TRACE_MSG(ATC_PHONEBOOK_2860_112_2_17_22_14_49_1449,"ATC: SCPBW, <email> is not string!")
TRACE_MSG(ATC_PHONEBOOK_2892_112_2_17_22_14_49_1450,"ATC:CPBW,Email : ATC_UCS2_CODE")
TRACE_MSG(ATC_PHONEBOOK_2929_112_2_17_22_14_49_1451,"ATC:^scpbw,PARA12.str_ptr=%x,%x,%x")
TRACE_MSG(ATC_PHONEBOOK_2931_112_2_17_22_14_49_1452,"ATC:additional_entry.email_addr.address=%x,%x,%x")
TRACE_MSG(ATC_PHONEBOOK_2942_112_2_17_22_14_49_1453,"ATC: SCPBW, <SNE> is not string!")
TRACE_MSG(ATC_PHONEBOOK_2981_112_2_17_22_14_50_1454,"ATC:CPBW,Email : ATC_UCS2_CODE")
TRACE_MSG(ATC_PHONEBOOK_3027_112_2_17_22_14_50_1455,"ATC:^scpbw,PARA13.str_ptr=%x,%x,%x")
TRACE_MSG(ATC_PHONEBOOK_3032_112_2_17_22_14_50_1456,"ATC:additional_entry.sne_alpha.alpha=%x,%x,%x")
TRACE_MSG(ATC_PHONEBOOK_3047_112_2_17_22_14_50_1457,"ATC: SCPBW, <text> is not string!")
TRACE_MSG(ATC_PHONEBOOK_3060_112_2_17_22_14_50_1458,"ATC: SCPBW: <text> length is not in range(1-%d)")
TRACE_MSG(ATC_PHONEBOOK_3075_112_2_17_22_14_50_1459,"ATC:cpbw,tel_alpha=,%x,%x,%x,str_len,%x")
TRACE_MSG(ATC_PHONEBOOK_3088_112_2_17_22_14_50_1460,"ATC:CPBW,ATC_UCS2_CODE == *entry.alpha")
TRACE_MSG(ATC_PHONEBOOK_3105_112_2_17_22_14_50_1461,"ATC: SCPBW: index length is TOO long")
TRACE_MSG(ATC_PHONEBOOK_3121_112_2_17_22_14_50_1462,"ATC:^scpbw,PARA10.str_ptr=%x,%x,%x")
TRACE_MSG(ATC_PHONEBOOK_3123_112_2_17_22_14_50_1463,"ATC:cpbw,entry.alpha=%x,%x,%x")
TRACE_MSG(ATC_PHONEBOOK_3200_112_2_17_22_14_50_1464,"ATC: SIM/USIM is not ready.")
TRACE_MSG(ATC_PHONEBOOK_3235_112_2_17_22_14_50_1465,"ATC: ^SCPBR, no phonebook entity.")
TRACE_MSG(ATC_PHONEBOOK_3252_112_2_17_22_14_50_1466,"ATC: SCPBR: index:%d is out of range(1-%d)")
TRACE_MSG(ATC_PHONEBOOK_3265_112_2_17_22_14_50_1467,"ATC: no phonebook record.")
TRACE_MSG(ATC_PHONEBOOK_3282_112_2_17_22_14_50_1468,"AT^SCPBR: MMI is initializing phonebook(current id:%d,total:%d).")
TRACE_MSG(ATC_PHONEBOOK_3426_112_2_17_22_14_50_1469,"ATC: +SPPBACTION, pb action ST_ACTION_READ_PB_SEC_TEXT is triggered.")
TRACE_MSG(ATC_PHONEBOOK_3431_112_2_17_22_14_50_1470,"ATC: USIM is not ready.call_read_flag[g_current_card_id]=%d,sim_type[g_current_card_id]=%d")
TRACE_MSG(ATC_PHONEBOOK_3440_112_2_17_22_14_50_1471,"ATC: +SPPBACTION, no phonebook entity.")
TRACE_MSG(ATC_PHONEBOOK_3472_112_2_17_22_14_51_1472,"ATC: +SPPBACTION, not USIM card inserted.")
TRACE_MSG(ATC_PHONEBOOK_3482_112_2_17_22_14_51_1473,"ATC: SPPBACTION: do nothing.")
TRACE_MSG(ATC_PHONEBOOK_3506_112_2_17_22_14_51_1474,"ATC: SIM/USIM is not ready.")
TRACE_MSG(ATC_PHONEBOOK_3514_112_2_17_22_14_51_1475,"[AT*SP3GPBR]: pbr_num_tel=%d, pbr_num_adf=%d")
TRACE_MSG(ATC_PLUS_GPRS_101_112_2_17_22_14_51_1476,"ATC: ATC_ProcessCGDCONT: only support IP (not %s).")
TRACE_MSG(ATC_PLUS_GPRS_170_112_2_17_22_14_51_1477,"ATC: ATC_ProcessCGDCONT, param4 is not string!")
TRACE_MSG(ATC_PLUS_GPRS_180_112_2_17_22_14_51_1478,"%s")
TRACE_MSG(ATC_PLUS_GPRS_268_112_2_17_22_14_51_1479,"ATC : CONTEXT NUM %d")
TRACE_MSG(ATC_PLUS_GPRS_280_112_2_17_22_14_51_1480,"ATC: ReadPDPCON pdp_con_num: %d")
TRACE_MSG(ATC_PLUS_GPRS_310_112_2_17_22_14_51_1481,"ATC: ReadPDPCON pdp_con_arr[i].pdp_addr_str.length: %d")
TRACE_MSG(ATC_PLUS_GPRS_387_112_2_17_22_14_52_1482,"ATC: ATC_ProcessCGPCO, param2 or param3 is not string!")
TRACE_MSG(ATC_PLUS_GPRS_404_112_2_17_22_14_52_1483,"ATC: pco user or passwd too long.")
TRACE_MSG(ATC_PLUS_GPRS_418_112_2_17_22_14_52_1484,"ATC: pco user or passwd too long.")
TRACE_MSG(ATC_PLUS_GPRS_428_112_2_17_22_14_52_1485,"ATC: set pco error, result = %d")
TRACE_MSG(ATC_PLUS_GPRS_434_112_2_17_22_14_52_1486,"ATC: set pco error parameter")
TRACE_MSG(ATC_PLUS_GPRS_598_112_2_17_22_14_52_1487,"ATC: ATC_ProcessCGQREQ param error!")
TRACE_MSG(ATC_PLUS_GPRS_607_112_2_17_22_14_52_1488,"ATC: MNGPRS_SetTotalQosProfile failure.")
TRACE_MSG(ATC_PLUS_GPRS_794_112_2_17_22_14_52_1489,"ATC: ATC_ProcessCGQREQ param error!")
TRACE_MSG(ATC_PLUS_GPRS_802_112_2_17_22_14_52_1490,"ATC: MNGPRS_SetTotalMinQosProfile failure.")
TRACE_MSG(ATC_PLUS_GPRS_954_112_2_17_22_14_53_1491,"ATC: MNPHONE_GprsDetach failure.")
TRACE_MSG(ATC_PLUS_GPRS_1078_112_2_17_22_14_53_1492,"ATC: ATC_ProcessCGACT, the cid:%d is not allowed.")
TRACE_MSG(ATC_PLUS_GPRS_1091_112_2_17_22_14_53_1493,"ATC: ATC_ProcessCGACT, this pdp id is being deactivated.")
TRACE_MSG(ATC_PLUS_GPRS_1335_112_2_17_22_14_53_1494,"ATC: CGPADDR")
TRACE_MSG(ATC_PLUS_GPRS_1477_112_2_17_22_14_54_1495,"ATC: layer 2 Protocol default?")
TRACE_MSG(ATC_PLUS_GPRS_1502_112_2_17_22_14_54_1496,"ATC: CGDATA, not support to activate all pdps.")
TRACE_MSG(ATC_PLUS_GPRS_1511_112_2_17_22_14_54_1497,"ATC: CGDATA, the cid:%d is not allowed.")
TRACE_MSG(ATC_PLUS_GPRS_1526_112_2_17_22_14_54_1498,"ATC: CGDATA, this pdp id is being deactivated.")
TRACE_MSG(ATC_PLUS_GPRS_1687_112_2_17_22_14_54_1499,"ATC: CGANS, param2 is not string!")
TRACE_MSG(ATC_PLUS_GPRS_1693_112_2_17_22_14_54_1500,"ATC: layer 2 Protocol:%s")
TRACE_MSG(ATC_PLUS_GPRS_1707_112_2_17_22_14_54_1501,"ATC: default Layer 2 Protocol")
TRACE_MSG(ATC_PLUS_GPRS_1714_112_2_17_22_14_54_1502,"ATC: para 3 pdp id:%d")
TRACE_MSG(ATC_PLUS_GPRS_1730_112_2_17_22_14_54_1503,"ATC: CGANS, MNGPRS_SetResType, failure")
TRACE_MSG(ATC_PLUS_GPRS_1734_112_2_17_22_14_54_1504,"ATC: CGANS, is_all:%d,pdp_id:%d")
TRACE_MSG(ATC_PLUS_GPRS_1829_112_2_17_22_14_54_1505,"input class name class%s")
TRACE_MSG(ATC_PLUS_GPRS_1884_112_2_17_22_14_55_1506,"ATC: CGSMS")
TRACE_MSG(ATC_PLUS_GPRS_1897_112_2_17_22_14_55_1507,"CGSMS: %d")
TRACE_MSG(ATC_PLUS_GPRS_1950_112_2_17_22_14_55_1508,"ATC: CGREG original value:%d")
TRACE_MSG(ATC_PLUS_GPRS_1962_112_2_17_22_14_55_1509,"ATC: CGREG set value:%d")
TRACE_MSG(ATC_PLUS_GPRS_1981_112_2_17_22_14_55_1510,"ATC: ATC_ProcessCGREG, plmn_status:%d,rat:%d,cell_cap:%d%d%d%d")
TRACE_MSG(ATC_PLUS_GPRS_2047_112_2_17_22_14_55_1511,"ATC: CGEREP: mode=%d,bfr=%d")
TRACE_MSG(ATC_PLUS_GPRS_2202_112_2_17_22_14_55_1512,"ATC: MNPHONE_GprsDetach failure.")
TRACE_MSG(ATC_PLUS_GPRS_2345_112_2_17_22_14_55_1513,"ATC: SGPRSDATA, ERROR: MNGPRS_ReadPdpContextStateEx failed.")
TRACE_MSG(ATC_PLUS_GPRS_2366_112_2_17_22_14_56_1514,"ATC: SGPRSDATA, ERROR: invalid active pdp count=%d.")
TRACE_MSG(ATC_PLUS_GPRS_2381_112_2_17_22_14_56_1515,"ATC: SGPRSDATA, too long length!")
TRACE_MSG(ATC_PLUS_GPRS_2389_112_2_17_22_14_56_1516,"ATC: SGPRSDATA, The GPRS send data len beyond MAX_AT_GPRSDATA_SEND_LEN!\n")
TRACE_MSG(ATC_PLUS_GPRS_2426_112_2_17_22_14_56_1517,"ATC: SGPRSDATA, MNGPRS_SendDataReqEx failed!")
TRACE_MSG(ATC_PLUS_GPRS_2459_112_2_17_22_14_56_1518,"ATC: SGPRSDATA, MNGPRS_SendDataReqEx failed!")
TRACE_MSG(ATC_PLUS_GPRS_2619_112_2_17_22_14_56_1519,"ATC:CGEQREQ, param9 is not string!")
TRACE_MSG(ATC_PLUS_GPRS_2633_112_2_17_22_14_56_1520,"ATC: CGEQREQ, param10 is not string!")
TRACE_MSG(ATC_PLUS_GPRS_2677_112_2_17_22_14_56_1521,"ATC: CGEQREQ param error!")
TRACE_MSG(ATC_PLUS_GPRS_2699_112_2_17_22_14_56_1522,"ATC: CGEQREQ L4 param is %ld,%d,%d,%d,%d,%d,%d,%d,")
TRACE_MSG(ATC_PLUS_GPRS_2702_112_2_17_22_14_56_1523,"ATC: CGEQREQ item_setted :%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d")
TRACE_MSG(ATC_PLUS_GPRS_2888_112_2_17_22_14_57_1524,"ATC: CGEQMIN, param9 is not string!")
TRACE_MSG(ATC_PLUS_GPRS_2901_112_2_17_22_14_57_1525,"ATC: CGEQMIN, param10 is not string!")
TRACE_MSG(ATC_PLUS_GPRS_2941_112_2_17_22_14_57_1526,"ATC: CGEQMIN param error!")
TRACE_MSG(ATC_PLUS_GPRS_3170_112_2_17_22_14_57_1527,"ATC: MNGPRS_ReadExtNegQos No CID.")
TRACE_MSG(ATC_PLUS_GPRS_3177_112_2_17_22_14_57_1528,"ATC: MNGPRS_ReadExtNegQos failed.")
TRACE_MSG(ATC_PLUS_GPRS_3217_112_2_17_22_14_57_1529,"ATC: CGEQNEG, read PDP state failure.")
TRACE_MSG(ATC_PLUS_GPRS_3249_112_2_17_22_14_57_1530,"ATC:CGEQNEG, no active contexts")
TRACE_MSG(ATC_PLUS_GPRS_3310_112_2_17_22_14_57_1531,"ATC: ATC_BuildGprsEvReportRsp, +cgerep mode is 0.")
TRACE_MSG(ATC_PLUS_GPRS_3330_112_2_17_22_14_57_1532,"ATC: ATC_BuildGprsEvReportRsp, +cgerep mode is 1, cid:%d is data mode.")
TRACE_MSG(ATC_PLUS_GPRS_3360_112_2_17_22_14_57_1533,"ATC: ATC_BuildGprsEvReportRsp, +cgerep mode is 2, cid:%d is data mode.")
TRACE_MSG(ATC_PLUS_GPRS_3527_112_2_17_22_14_58_1534,"ATC: Close MODEM service!")
TRACE_MSG(ATC_PLUS_GPRS_3681_112_2_17_22_14_58_1535,"ATC: pdp activate is not started by atc.")
TRACE_MSG(ATC_PLUS_GPRS_3726_112_2_17_22_14_58_1536,"ATC: PDP (cid:%d) change to data mode context")
TRACE_MSG(ATC_PLUS_GPRS_3781_112_2_17_22_14_59_1537,"ATC:  invalid cid for mux link\r")
TRACE_MSG(ATC_PLUS_GPRS_3804_112_2_17_22_14_59_1538,"ATC:  MPPP_SetPPPInterface() fail")
TRACE_MSG(ATC_PLUS_GPRS_3816_112_2_17_22_14_59_1539,"ATC: Open MODEM service!")
TRACE_MSG(ATC_PLUS_GPRS_3822_112_2_17_22_14_59_1540,"ATC: APP_MN_ACTIVATE_PDP_CONTEXT_CNF(failure), cid:%d")
TRACE_MSG(ATC_PLUS_GPRS_3834_112_2_17_22_14_59_1541,"ATC: ERROR: no found link_id of PSD!")
TRACE_MSG(ATC_SLEEP_55_112_2_17_22_15_3_1542,"SCI_ATC_InitSleep: Init !")
TRACE_MSG(ATC_SLEEP_83_112_2_17_22_15_3_1543,"SCI_ATC_WakeUpOtherMcu: Wake up the other MCU !")
TRACE_MSG(ATC_SLEEP_119_112_2_17_22_15_3_1544,"SCI_ATC_SetSleepSwitchFlag: set it to %d")
TRACE_MSG(ATC_SLEEP_141_112_2_17_22_15_3_1545,"SCI_ATC_GetSleepSwitchFlag: get the value is %d")
TRACE_MSG(PHONESERVER_STK_ADAPT_450_112_2_17_22_15_7_1546,"SIMAT::ATC_GetPCName PC tag 0x%x is out of range. \n")
TRACE_MSG(PHONESERVER_STK_ADAPT_676_112_2_17_22_15_8_1547,"SIMAT_Event_DownLoad_Env:enter")
TRACE_MSG(PHONESERVER_STK_ADAPT_699_112_2_17_22_15_8_1548,"SIMAT_Event_DownLoad_Env:SIMAT_BTAG_EVENT_LIST")
TRACE_MSG(PHONESERVER_STK_ADAPT_709_112_2_17_22_15_8_1549,"SIMAT_Event_DownLoad_Env:SIMAT_BTAG_DEVICE_ID")
TRACE_MSG(PHONESERVER_STK_ADAPT_716_112_2_17_22_15_8_1550,"SIMAT_Event_DownLoad_Env:des/source")
TRACE_MSG(PHONESERVER_STK_ADAPT_744_112_2_17_22_15_8_1551,"SIMAT_Event_DownLoad_Env invalid Event %d")
TRACE_MSG(PHONESERVER_STK_ADAPT_791_112_2_17_22_15_8_1552,"PhoneSever_STK_Envelop_Handle envelop_tag is %d\n")
TRACE_MSG(PHONESERVER_STK_ADAPT_809_112_2_17_22_15_8_1553,"PhoneSever_STK_Envelop_Handle ret is %d\n")
TRACE_MSG(PHONESERVER_STK_ADAPT_1710_112_2_17_22_15_10_1554,"11,%d")
TRACE_MSG(PHONESERVER_STK_ADAPT_1720_112_2_17_22_15_10_1555,"22,%d")
TRACE_MSG(PHONESERVER_STK_ADAPT_2105_112_2_17_22_15_11_1556,"PhoneSever_STK_TerminalRsp_Handle pc name is %d ")
TRACE_MSG(PHONESERVER_STK_COMMON_FUNC_125_112_2_17_22_15_15_1557,"SIMAT::SIMAT_GetTLVLength point is NULL! \n")
TRACE_MSG(PHONESERVER_STK_COMMON_FUNC_171_112_2_17_22_15_15_1558,"SIMAT::SIMAT_GetTLVLength point is NULL! \n")
TRACE_MSG(PHONESERVER_STK_DECODE_220_112_2_17_22_15_17_1559,"SIMAT::SIMAT_DecodeDataObject tag error!\n")
TRACE_MSG(PHONESERVER_STK_DECODE_297_112_2_17_22_15_17_1560,"SIMAT:the address len longer than the maximum we support, error and cut it!!!\n")
TRACE_MSG(PHONESERVER_STK_DECODE_365_112_2_17_22_15_17_1561,"SIMAT::The real alpha id length is 0x%x.")
TRACE_MSG(PHONESERVER_STK_DECODE_694_112_2_17_22_15_18_1562,"SIMAT::the length of the duration is not 2.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_774_112_2_17_22_15_18_1563,"SIMAT:: DecodeItem content_len is zero!!! ")
TRACE_MSG(PHONESERVER_STK_DECODE_856_112_2_17_22_15_18_1564,"SIMAT::the length of item id is not 1.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_917_112_2_17_22_15_18_1565,"SIMAT::the length of the Response length is not 2.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_1186_112_2_17_22_15_19_1566,"SIMAT:: DecodeTextString content_len is zero!!! \n")
TRACE_MSG(PHONESERVER_STK_DECODE_1269_112_2_17_22_15_19_1567,"SIMAT::the length of the tone is not 1.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_1490_112_2_17_22_15_19_1568,"SIMAT::length of LocationInfo is not 7!\n")
TRACE_MSG(PHONESERVER_STK_DECODE_1556_112_2_17_22_15_20_1569,"SIMAT::length of IMEI is not 8!\n")
TRACE_MSG(PHONESERVER_STK_DECODE_1614_112_2_17_22_15_20_1570,"SIMAT::length of Help request is not 0!\n")
TRACE_MSG(PHONESERVER_STK_DECODE_1671_112_2_17_22_15_20_1571,"SIMAT::length of NMR is not 16!\n")
TRACE_MSG(PHONESERVER_STK_DECODE_1759_112_2_17_22_15_20_1572,"SIMAT::DecodeDefaultText alloc from share buffer error!\n")
TRACE_MSG(PHONESERVER_STK_DECODE_1993_112_2_17_22_15_20_1573,"SIMAT::length of location status is not 1.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_2208_112_2_17_22_15_21_1574,"SIMAT::DecodeCCReqAction alloc from share buffer error!\n")
TRACE_MSG(PHONESERVER_STK_DECODE_2253_112_2_17_22_15_21_1575,"SIMAT::the length of icon id is not 2.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_2469_112_2_17_22_15_21_1576,"SIMAT::the length of timer id is not 1.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_2555_112_2_17_22_15_22_1577,"hour %d, min %d, sec %d, timer_value :%d ")
TRACE_MSG(PHONESERVER_STK_DECODE_2597_112_2_17_22_15_22_1578,"SIMAT::the length of the DTandDZ is not 7.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_2758_112_2_17_22_15_22_1579,"SIMAT::the length of the ImmediateResponse is not 0.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_2882_112_2_17_22_15_22_1580,"SIMAT::the length of the Language is not 2.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_2943_112_2_17_22_15_22_1581,"SIMAT::the length of the Time Advance is not 2.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_3017_112_2_17_22_15_22_1582,"SIMAT:: DecodeChannelData content_len is zero!!! ")
TRACE_MSG(PHONESERVER_STK_DECODE_3080_112_2_17_22_15_23_1583,"SIMAT::the length of channel data len  is not 1.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_3139_112_2_17_22_15_23_1584,"SIMAT::the length of browser id  is not 1.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_3197_112_2_17_22_15_23_1585,"SIMAT::the length of URL overflow.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_3204_112_2_17_22_15_23_1586,"SIMAT:: DecodeUrl content_len is zero!!! \n")
TRACE_MSG(PHONESERVER_STK_DECODE_3260_112_2_17_22_15_23_1587,"SIMAT::the length of bear overflow.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_3268_112_2_17_22_15_23_1588,"SIMAT:: DecodeBearer content_len is zero!!! ")
TRACE_MSG(PHONESERVER_STK_DECODE_3325_112_2_17_22_15_23_1589,"SIMAT::the length of file path overflow.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_3333_112_2_17_22_15_23_1590,"SIMAT:: DecodeProvisionFileRef content_len is zero!!! ")
TRACE_MSG(PHONESERVER_STK_DECODE_3390_112_2_17_22_15_23_1591,"SIMAT::the length of bear qos overflow.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_3398_112_2_17_22_15_23_1592,"SIMAT:: DecodeBearDesc content_len is zero!!! ")
TRACE_MSG(PHONESERVER_STK_DECODE_3457_112_2_17_22_15_23_1593,"SIMAT::the length of buffer size is wrong.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_3513_112_2_17_22_15_23_1594,"SIMAT::the length of net access name overflow.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_3521_112_2_17_22_15_23_1595,"SIMAT:: DecodeNetAccessName content_len is zero!!! ")
TRACE_MSG(PHONESERVER_STK_DECODE_3577_112_2_17_22_15_24_1596,"SIMAT::the length of other address overflow.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_3585_112_2_17_22_15_24_1597,"SIMAT:: DecodeOtherAddress content_len is zero!!! ")
TRACE_MSG(PHONESERVER_STK_DECODE_3641_112_2_17_22_15_24_1598,"SIMAT::the length of trans level overflow.\n")
TRACE_MSG(PHONESERVER_STK_DECODE_3660_112_2_17_22_15_24_1599,"[DecodeTransLevel] port= %x  ")
TRACE_MSG(PHONESERVER_STK_DECODE_3704_112_2_17_22_15_24_1600,"SIMAT:: DecodeMultiMsgRef content_len is zero!!! ")
TRACE_MSG(PHONESERVER_STK_DECODE_3764_112_2_17_22_15_24_1601,"SIMAT:: DecodeMultiMsgId content_len is zero!!! ")
TRACE_MSG(PHONESERVER_STK_DECODE_3823_112_2_17_22_15_24_1602,"SIMAT:: DecodeMmContentId content_len is zero!!! ")
TRACE_MSG(PHONESERVER_STK_DECODE_4139_112_2_17_22_15_25_1603,"SIMAT::the length of channel status is not 2.\n")
TRACE_MSG(PHONESERVER_STK_ENCODE_192_112_2_17_22_15_27_1604,"SIMAT:the address len longer than the maximum we support, error and cut it!!!\n")
TRACE_MSG(PHONESERVER_STK_ENCODE_2277_112_2_17_22_15_31_1605,"EncodeTransLevel occuring error point is NULL!\n")
TRACE_MSG(PHONESERVER_STK_ENCODE_3090_112_2_17_22_15_33_1606,"EncodeNetAccessName occuring error point is NULL!\n")
TRACE_MSG(PHONESERVER_STK_ENCODE_3161_112_2_17_22_15_33_1607,"EncodeBrowserId occuring error point is NULL!\n")
TRACE_MSG(PHONESERVER_STK_ENCODE_3201_112_2_17_22_15_33_1608,"EncodeUrl occuring error point is NULL!\n")
TRACE_MSG(PHONESERVER_STK_ENCODE_3241_112_2_17_22_15_33_1609,"EncodeBearer occuring error point is NULL!\n")
TRACE_MSG(PHONESERVER_STK_ENCODE_3281_112_2_17_22_15_33_1610,"EncodeProvisionFileRef occuring error point is NULL!\n")
TRACE_MSG(PHONESERVER_STK_FUNC_59_112_2_17_22_15_33_1611,"SIMAT::SIMAT_DecodeUnknowObject point is NULL! \n")
TRACE_MSG(PHONESERVER_STK_FUNC_95_112_2_17_22_15_33_1612,"SIMAT::SIMAT_DecodeDataObject point is NULL! \n")
TRACE_MSG(PHONESERVER_STK_FUNC_102_112_2_17_22_15_33_1613,"SIMAT::SIMAT_DecodeDataObject error! \n")
TRACE_MSG(PHONESERVER_STK_FUNC_129_112_2_17_22_15_33_1614,"SIMAT::ATC_EncodeDataObject point is NULL! \n")
TRACE_MSG(PHONESERVER_STK_FUNC_265_112_2_17_22_15_34_1615,"SIMAT::ATC_EncodeDataObject point is NULL! \n")
TRACE_MSG(PHONESERVER_STK_FUNC_273_112_2_17_22_15_34_1616,"SIMAT::ATC_EncodeDataObject error! \n")
TRACE_MSG(PHONESERVER_STK_FUNC_276_112_2_17_22_15_34_1617,"SIMAT:object_type:%d")
TRACE_MSG(ATC_UTMAIN_692_112_2_17_22_16_8_1618,"ATC: ATC_Task, APP_MN_CALL_ERR_IND, disconnect VT mux link, call_id:%d, cause:%d")
TRACE_MSG(ATC_UTMAIN_721_112_2_17_22_16_8_1619,"ATC: ATC_Task, Signal Code: APP_MN_CALL_DISCONNECTED_IND, set atc_err_protected_flag")
TRACE_MSG(ATC_UTMAIN_728_112_2_17_22_16_8_1620,"ATC: ATC_Task, APP_MN_CALL_DISCONNECTED_IND, disconnect VT mux link, call_id:%d, cause:%d")
TRACE_MSG(ATC_UTMAIN_776_112_2_17_22_16_9_1621,"ATC: ATC_Task, Signal Code: APP_MN_ACTIVATE_PDP_CONTEXT_CNF")
TRACE_MSG(ATC_UTMAIN_833_112_2_17_22_16_9_1622,"ATC:  invalid cid for mux link\r")
TRACE_MSG(ATC_UTMAIN_850_112_2_17_22_16_9_1623,"ATC:  MPPP_SetPPPInterface() fail")
TRACE_MSG(ATC_UTMAIN_855_112_2_17_22_16_9_1624,"ATC: Switch to DATA MODE")
TRACE_MSG(ATC_UTMAIN_860_112_2_17_22_16_9_1625,"ATC: APP_MN_ACTIVATE_PDP_CONTEXT_CNF(failure), cid:%d")
TRACE_MSG(ATC_UTMAIN_900_112_2_17_22_16_9_1626,"ATC: ATC_Task, Signal Code: APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, result:%d,pdp_num:%d,cid:%d, set atc_err_protected_flag")
TRACE_MSG(ATC_UTMAIN_925_112_2_17_22_16_9_1627,"ATC: cid:%d is non-data-mode")
END_TRACE_MAP(ATC_3G_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _ATC_3G_TRC_H_

