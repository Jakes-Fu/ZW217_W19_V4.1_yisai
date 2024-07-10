/******************************************************************************
 ** File Name:      base_trc.h                                         *
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
//trace_id:2
#ifndef _BASE_TRC_H_
#define _BASE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define ATC_330_112_2_17_22_16_18_0 "ATC: write nv fail"
#define ATC_379_112_2_17_22_16_18_1 "ATC:read atc_config_nv fail"
#define ATC_386_112_2_17_22_16_18_2 "ATC:write NV_ATC_CONFIG_ID fail"
#define ATC_396_112_2_17_22_16_18_3 "ATC:read midi_size_arr fail"
#define ATC_403_112_2_17_22_16_18_4 "ATC:write NV_ATC_MIDI_SIZE_ID fail"
#define ATC_419_112_2_17_22_16_18_5 "ATC: ATC_Mux_Setup_Link, WARNING! link_id:%d"
#define ATC_424_112_2_17_22_16_18_6 "ATC: ATC_Mux_Setup_Link, WARNING! link_id:%d is already setup"
#define ATC_438_112_2_17_22_16_18_7 "ATC: ATC_Mux_Setup_Link, ERROR! no mem to setup link_id:%d"
#define ATC_447_112_2_17_22_16_18_8 "ATC: ATC_Mux_Setup_Link, link_id: %d"
#define ATC_462_112_2_17_22_16_18_9 "ATC: ATC_Mux_Release_Link, link_id: %d"
#define ATC_500_112_2_17_22_16_18_10 "ATC: ATC_Mux_Release_Link ERROR!!!!"
#define ATC_529_112_2_17_22_16_18_11 "ATC SET link id to %d, old %d"
#define ATC_574_112_2_17_22_16_18_12 "ATC: ATC_List_Expected_Event as follows:"
#define ATC_588_112_2_17_22_16_18_13 "|---|-------|-----|-------|------------------------------"
#define ATC_589_112_2_17_22_16_18_14 "|Idx|Link-ID| CID |Call-ID| Event"
#define ATC_590_112_2_17_22_16_18_15 "|---|-------|-----|-------|------------------------------"
#define ATC_602_112_2_17_22_16_19_16 "|%-3d|   %d   | %-3d |  %-3d  | %s"
#define ATC_610_112_2_17_22_16_19_17 "|%-3d|   %d   | %-3d |  %-3d  | %d"
#define ATC_623_112_2_17_22_16_19_18 "|---|-------|-----|-------|------------------------------"
#define ATC_626_112_2_17_22_16_19_19 "Total:%d"
#define ATC_657_112_2_17_22_16_19_20 "ATC: ATC_Clear_All_Exp_Result id:%d,domain_id:%d"
#define ATC_674_112_2_17_22_16_19_21 "ATC: ATC_Clear_All_Exp_Result, cid:%d, exp event: %s"
#define ATC_679_112_2_17_22_16_19_22 "ATC: ATC_Clear_All_Exp_Result, cid:%d, exp event: %d"
#define ATC_718_112_2_17_22_16_19_23 "ATC: ATC_Clear_All_Exp_Result, call_id:%d, exp event: %d"
#define ATC_965_112_2_17_22_16_19_24 "ATC: recv default event %x"
#define ATC_983_112_2_17_22_16_19_25 "ATC: recv event %x, not found in expected list"
#define ATC_1001_112_2_17_22_16_19_26 "ATC: ATC_Receive_Expected_Event, id:%d,event:%s"
#define ATC_1005_112_2_17_22_16_19_27 "ATC: ATC_Receive_Expected_Event, id:%d,event:%d"
#define ATC_1012_112_2_17_22_16_19_28 "ATC: ATC_Receive_Expected_Event, WARNING! NOT FOUND, set link id: %d"
#define ATC_1017_112_2_17_22_16_19_29 "ATC: ATC_Receive_Expected_Event, WARNING! NOT FOUND, set urc link id: %d"
#define ATC_1032_112_2_17_22_16_19_30 "ATC: ATC_Receive_Expected_Event, FOUND, ERROR! match_link_id:%d != call_link_id:%d"
#define ATC_1037_112_2_17_22_16_19_31 "ATC: ATC_Receive_Expected_Event, FOUND, WARNING! match_link_id:%d, no call_link_id, NOT ADD EARLIER?"
#define ATC_1045_112_2_17_22_16_19_32 "ATC: ATC_Receive_Expected_Event, FOUND, match_link_id:%d, call_link_id:%d"
#define ATC_1050_112_2_17_22_16_19_33 "ATC: ATC_Receive_Expected_Event, FOUND, match_link_id:%d"
#define ATC_1085_112_2_17_22_16_19_34 "ATC: ATC_Add_Expected_Event, link_id:%d, event:%s"
#define ATC_1089_112_2_17_22_16_19_35 "ATC: ATC_Add_Expected_Event, link_id:%d, event:%d"
#define ATC_1114_112_2_17_22_16_20_36 "ATC: ATC_Add_Expected_Event, unknown domain_id: %d"
#define ATC_1126_112_2_17_22_16_20_37 "ATC: ATC_Add_Expected_Event,Error :The atc response buffer is full ,clear first message,pls check"
#define ATC_1346_112_2_17_22_16_20_38 "ATC: ATC_ReadCmdLine Warning: only read %d < %d."
#define ATC_1350_112_2_17_22_16_20_39 "ATC: ATC_ReadCmdLine Warning: garbage data ???"
#define ATC_1421_112_2_17_22_16_20_40 "ATC: ATC_SendNewATInd,link_id:%d,data_length:%d,data %x, %x, %x, %x"
#define ATC_1449_112_2_17_22_16_20_41 "ATC: ATC_SendNewATInd, is_close_atc_queue is TRUE, return"
#define ATC_1455_112_2_17_22_16_20_42 "ATC: ATC_SendNewATInd, link is not setup!!"
#define ATC_1461_112_2_17_22_16_21_43 "ATC: ATC_SendNewATInd, atc_is_echo is TRUE, send copy back"
#define ATC_1472_112_2_17_22_16_21_44 "ATC: ERROR AT BUFFER FULL  LINK %d "
#define ATC_1510_112_2_17_22_16_21_45 "ATC: ATC_Debug_SendATInd, len:%d"
#define ATC_1533_112_2_17_22_16_21_46 "ATC: ATC_Debug_SendATInd, is_close_atc_queue is TRUE, return"
#define ATC_1538_112_2_17_22_16_21_47 "temp_line_length %d"
#define ATC_1552_112_2_17_22_16_21_48 "ATC: ERROR AT BUFFER FULL  LINK ATC_DEBUG_LINK "
#define ATC_1579_112_2_17_22_16_21_49 "ATC: CAL POST MODE, return ATC_DEBUG_LINK"
#define ATC_1585_112_2_17_22_16_21_50 "ATC_GetDualSimMuxLinkId return ctl_link %d"
#define ATC_1750_112_2_17_22_16_21_51 "ATC: ATC_Task, Signal Code: ATC_MUX_LINK_SETUP_REQ, link_id:%d"
#define ATC_1757_112_2_17_22_16_21_52 "ATC: ATC_Task, Signal Code: ATC_MUX_LINK_RELEASE_REQ, link_id:%d"
#define ATC_1764_112_2_17_22_16_21_53 "ATC: ATC_Task, Signal Code: ATC_MUX_CLOSE_REQ"
#define ATC_1771_112_2_17_22_16_21_54 "ATC: ATC_Task, Signal Code: ATC_MUX_RECV_NEW_AT, link_id:%d,len:%d"
#define ATC_1782_112_2_17_22_16_21_55 "ATC: 2 atc_global_info config link %d"
#define ATC_1793_112_2_17_22_16_21_56 "ATC: atc_task, atc_global_info config link %d"
#define ATC_1860_112_2_17_22_16_22_57 "atc: network deactivate PDP"
#define ATC_1869_112_2_17_22_16_22_58 "atc: receive pdp activate message from network"
#define ATC_1898_112_2_17_22_16_22_59 "atc: user deactivate PDP "
#define ATC_1913_112_2_17_22_16_22_60 "ATC: Switch to AT MODE for PCLink!"
#define ATC_1927_112_2_17_22_16_22_61 "case APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,is deactivate AT pid,pid:%d"
#define ATC_1988_112_2_17_22_16_22_62 "ATC: g_atc_ignore_one_detach_cnf %d"
#define ATC_2096_112_2_17_22_16_22_63 "ATC: ATC_Task, Signal Code: APP_MN_CALL_DISCONNECTED_IND, set atc_err_protected_flag"
#define ATC_2577_112_2_17_22_16_23_64 "ATC:APP_MN_ADN_UPDATE_CNF"
#define ATC_2704_112_2_17_22_16_23_65 "ATC initialize failed."
#define ATC_2923_112_2_17_22_16_24_66 "ATC: ATC_RecNewLineSig,link_id:%d,len:%d"
#define ATC_2984_112_2_17_22_16_24_67 "ATC: backspace? len: %d->%d"
#define ATC_3004_112_2_17_22_16_24_68 "ATC: ATC_RecNewLineSig, len:%d,hex:%s,line:%s"
#define ATC_3008_112_2_17_22_16_24_69 "ATC: ATC_RecNewLineSig, len:%d"
#define ATC_3236_112_2_17_22_16_25_70 "ATC: ATC_RecNewLineSig, ignore ip data, len:%d->%d, new atcmd:%s"
#define ATC_3257_112_2_17_22_16_25_71 "ATC: ATC_RecNewLineSig, Assert Failure sharp len too long."
#define ATC_3264_112_2_17_22_16_25_72 "ATC: ATC_RecNewLineSig, Call the external function"
#define ATC_3300_112_2_17_22_16_25_73 "ATC: ATC_RecNewLineSig, Parse AT cmd line success."
#define ATC_3304_112_2_17_22_16_25_74 "ATC: cmd next %x"
#define ATC_3331_112_2_17_22_16_25_75 "ATC: ATC_RecNewLineSig, Parse SMS PDU successful"
#define ATC_3337_112_2_17_22_16_25_76 "ATC: ATC_RecNewLineSig, Parse AT command line failed."
#define ATC_3363_112_2_17_22_16_25_77 "ATC: prcesscmds link %d"
#define ATC_3405_112_2_17_22_16_25_78 "ATC: 5 atc_global_info config link %d"
#define ATC_3530_112_2_17_22_16_25_79 "ATC:RECV AT len = %d,Content = %s"
#define ATC_3535_112_2_17_22_16_25_80 "ATC:Recv AT len = %d,too long!"
#define ATC_3616_112_2_17_22_16_25_81 "atc: ATC_CTRLZ_GPRS, status=%d"
#define ATC_3723_112_2_17_22_16_26_82 "ATC:ASSERT invlaid ctrlz=%d"
#define ATC_3744_112_2_17_22_16_26_83 "ATC: ATC_RecNewLineSig, ignore ip data, len:%d->%d, new atcmd:%s"
#define ATC_3775_112_2_17_22_16_26_84 "ATC:Assert Failure sharp too long."
#define ATC_3800_112_2_17_22_16_26_85 "ATC:Parse AT cmd OK"
#define ATC_3826_112_2_17_22_16_26_86 "ATC:Parse SMS PDU successful"
#define ATC_3832_112_2_17_22_16_26_87 "ATC:Parse AT command line failed.\n"
#define ATC_3879_112_2_17_22_16_26_88 "ATC:Assert issue for processCMDs!"
#define ATC_4126_112_2_17_22_16_26_89 "ATC:Check AT Command valid.\n"
#define ATC_4130_112_2_17_22_16_26_90 "ATC:Check AT Command invalid.\n"
#define ATC_4158_112_2_17_22_16_27_91 "atc: mux enable link id = %d"
#define ATC_4230_112_2_17_22_16_27_92 "ATC:assert invlaid cmee type!"
#define ATC_4249_112_2_17_22_16_27_93 "ATC: BuildResultCodeRsp,The string:%s"
#define ATC_4311_112_2_17_22_16_27_94 "ATC:assert invalid cmee type!"
#define ATC_4316_112_2_17_22_16_27_95 "ATC:BuildSmsCodeRsp,The string=%s"
#define ATC_4356_112_2_17_22_16_27_96 "ATC: ATC_BuildInfoRsp \"%s\""
#define ATC_4360_112_2_17_22_16_27_97 "ATC: ATC_BuildInfoRsp \"%c%c%c%c%c%c%c%c%c%c%c%c\""
#define ATC_4366_112_2_17_22_16_27_98 "ATC: ATC_BuildInfoRsp debug link %d"
#define ATC_4370_112_2_17_22_16_27_99 "ATC: ATC_BuildInfoRsp link_id %d, work mode %d"
#define ATC_4560_112_2_17_22_16_27_100 "ATC:BuildTestCmdResponse,The string=%s"
#define ATC_5091_112_2_17_22_16_28_101 "ATC:RegisterMsg STK"
#define ATC_5180_112_2_17_22_16_29_102 "ATC: ATC_DisconnectModem "
#define ATC_5207_112_2_17_22_16_29_103 "ATC: ATC_DisconnectModem, no call(cid:%d) link id! PSD is not existed!"
#define ATC_5212_112_2_17_22_16_29_104 "ATC: ATC_DisconnectModem, failure in MNGPRS_DeactivatePdpContext"
#define ATC_5236_112_2_17_22_16_29_105 "pid %d is ATC activated"
#define ATC_5248_112_2_17_22_16_29_106 "ATC:DisconnectModem!"
#define ATC_5324_112_2_17_22_16_29_107 "ATC: Assert Failure unknow chr"
#define ATC_5395_112_2_17_22_16_29_108 "ATC:call state error"
#define ATC_5444_112_2_17_22_16_29_109 "pid %d is activated:%d"
#define ATC_5458_112_2_17_22_16_29_110 "ATC_ProcessDeactPdpContextInd,manual attach,send detach"
#define ATC_BASIC_CMD_423_112_2_17_22_16_30_111 "ATC: Assert SMUX,invalid type."
#define ATC_BASIC_CMD_585_112_2_17_22_16_31_112 "ATC_ProcessPreferPlmnChangCnf result = %d"
#define ATC_BASIC_CMD_663_112_2_17_22_16_31_113 "ATC: ReadPreferredPlmnMaxNum max_num = %d"
#define ATC_BASIC_CMD_715_112_2_17_22_16_31_114 "ConvertPlmn PLMN(%d, %d)"
#define ATC_BASIC_CMD_812_112_2_17_22_16_31_115 "ATC: Assert Failure unknow creg."
#define ATC_BASIC_CMD_903_112_2_17_22_16_31_116 "ATC: Assert Failure unknow creg enum."
#define ATC_BASIC_CMD_914_112_2_17_22_16_31_117 "ATC: Assert CREG,invalid type."
#define ATC_BASIC_CMD_1004_112_2_17_22_16_31_118 "ATC: Assert S0,invalid type."
#define ATC_BASIC_CMD_1042_112_2_17_22_16_32_119 "ATC: Assert CGMI,invalid type."
#define ATC_BASIC_CMD_1092_112_2_17_22_16_32_120 "ATC: sarft sign data fail."
#define ATC_BASIC_CMD_1115_112_2_17_22_16_32_121 "ATC: Assert CGSN,invalid type."
#define ATC_BASIC_CMD_1207_112_2_17_22_16_32_122 "ATC: CSCS TYPE SET"
#define ATC_BASIC_CMD_1239_112_2_17_22_16_32_123 "ATC: CSCS TYPE SET"
#define ATC_BASIC_CMD_1247_112_2_17_22_16_32_124 "ATC: Assert CSCS,invalid type."
#define ATC_BASIC_CMD_1279_112_2_17_22_16_32_125 "ATC: SPIN param error"
#define ATC_BASIC_CMD_1301_112_2_17_22_16_32_126 "ATC: Assert Failure pin len error."
#define ATC_BASIC_CMD_1323_112_2_17_22_16_32_127 "ATC: Assert Failure pin length error."
#define ATC_BASIC_CMD_1363_112_2_17_22_16_32_128 "ATC: ATC_ProcessCPIN, PIN1,failure in MNPHONE_ResponsePin"
#define ATC_BASIC_CMD_1390_112_2_17_22_16_32_129 "ATC: ATC_ProcessCPIN, PIN2, failure in MNPHONE_ResponsePin"
#define ATC_BASIC_CMD_1432_112_2_17_22_16_32_130 "ATC: ATC_ProcessCPIN, sim_flag:TRUE, PUK1, failure in MNPHONE_ResponsePin"
#define ATC_BASIC_CMD_1449_112_2_17_22_16_32_131 "ATC: ATC_ProcessCPIN, sim_flag:TRUE,PUK1, failure in MNPHONE_ResponsePin"
#define ATC_BASIC_CMD_1492_112_2_17_22_16_32_132 "ATC: ATC_ProcessCPIN, PUK2, failure in MNPHONE_ResponsePin"
#define ATC_BASIC_CMD_1526_112_2_17_22_16_33_133 "ATC: Assert Failure unknow cpin state %d"
#define ATC_BASIC_CMD_1539_112_2_17_22_16_33_134 "ATC: Assert Failure cpin state wrong"
#define ATC_BASIC_CMD_1562_112_2_17_22_16_33_135 "ATC: Invalid parameter value been input"
#define ATC_BASIC_CMD_1618_112_2_17_22_16_33_136 "ATC: unknow pin type %d."
#define ATC_BASIC_CMD_1953_112_2_17_22_16_33_137 "ATC: recv APP_MN_SYNC_IND signal"
#define ATC_BASIC_CMD_2110_112_2_17_22_16_34_138 "ATC: Assert Failure unknow creg."
#define ATC_BASIC_CMD_2171_112_2_17_22_16_34_139 "ATC: Assert Failure unknow plmn status."
#define ATC_BASIC_CMD_2228_112_2_17_22_16_34_140 "ATC: Assert single command,invalid type."
#define ATC_BASIC_CMD_2347_112_2_17_22_16_34_141 "atc: mux enable"
#define ATC_BASIC_CMD_2393_112_2_17_22_16_34_142 "ATC: CCLK input string is %s"
#define ATC_BASIC_CMD_2431_112_2_17_22_16_34_143 "ATC: Assert CCLK,invalid type."
#define ATC_BASIC_CMD_2492_112_2_17_22_16_34_144 "ATC: Assert VGR,invalid type."
#define ATC_BASIC_CMD_2565_112_2_17_22_16_35_145 "ATC:param_len = %d"
#define ATC_BASIC_CMD_2712_112_2_17_22_16_35_146 "ATC: CIND value %d."
#define ATC_BASIC_CMD_2916_112_2_17_22_16_35_147 "ATC: Set csq true"
#define ATC_BASIC_CMD_2945_112_2_17_22_16_35_148 "ATC: Assert CCED,invalid type."
#define ATC_BASIC_CMD_3079_112_2_17_22_16_36_149 "ATC: Assert CRSL,invalid type."
#define ATC_BASIC_CMD_3132_112_2_17_22_16_36_150 "ATC: Assert CMUT,invalid type."
#define ATC_BASIC_CMD_3330_112_2_17_22_16_36_151 "ATC: Assert SSAM,invalid type."
#define ATC_BASIC_CMD_3362_112_2_17_22_16_36_152 "ATC: SSAP pga_gain= %d, digi_gain= %d"
#define ATC_BASIC_CMD_3417_112_2_17_22_16_36_153 "ATC: Assert SSAP,invalid type."
#define ATC_BASIC_CMD_3460_112_2_17_22_16_36_154 "ATC: ATC_ProcessSADMDSP"
#define ATC_BASIC_CMD_3481_112_2_17_22_16_36_155 "ATC: mode_index=%d"
#define ATC_BASIC_CMD_3487_112_2_17_22_16_36_156 "ATC: audmod_devmod_ptr=0x%x, name %s"
#define ATC_BASIC_CMD_3537_112_2_17_22_16_37_157 "ATC: str_rsp_len=%d "
#define ATC_BASIC_CMD_3547_112_2_17_22_16_37_158 "ATC: str_len_rsp=%d "
#define ATC_BASIC_CMD_3630_112_2_17_22_16_37_159 "ATC: audio_ctrl_mode=%d"
#define ATC_BASIC_CMD_3709_112_2_17_22_16_37_160 "ATC: AUDIONVDSP_GetAudioModeName failed mode:%d."
#define ATC_BASIC_CMD_3715_112_2_17_22_16_37_161 "ATC: AUDIONVDSP_ReadModeParamFromFlash failed."
#define ATC_BASIC_CMD_3738_112_2_17_22_16_37_162 "ATC: Assert SADMDSP,invalid type."
#define ATC_BASIC_CMD_3812_112_2_17_22_16_37_163 "ATC: sadm mode_index = %d, dev_number = %d"
#define ATC_BASIC_CMD_3852_112_2_17_22_16_37_164 "ATC: sadm mode_index = %d, dev_number = %d"
#define ATC_BASIC_CMD_3873_112_2_17_22_16_37_165 "atc: str_rsp_len=%d "
#define ATC_BASIC_CMD_3883_112_2_17_22_16_37_166 "atc: str_len_rsp=%d "
#define ATC_BASIC_CMD_3970_112_2_17_22_16_37_167 "ATC: set dev mode, can't get valid mode type, mode name:%s."
#define ATC_BASIC_CMD_3978_112_2_17_22_16_37_168 "ATC: set dev mode result = %d"
#define ATC_BASIC_CMD_3986_112_2_17_22_16_37_169 "ATC: set dev mode result = %d"
#define ATC_BASIC_CMD_3993_112_2_17_22_16_37_170 "ATC: set aud dev param error."
#define ATC_BASIC_CMD_4060_112_2_17_22_16_38_171 "ATC: sadm mode_index = %d, dev_number = %d"
#define ATC_BASIC_CMD_4082_112_2_17_22_16_38_172 "ATC: PROD_ReadAudioParamFromFlash failed."
#define ATC_BASIC_CMD_4122_112_2_17_22_16_38_173 "ATC: sadm mode_index = %d, dev_number = %d"
#define ATC_BASIC_CMD_4151_112_2_17_22_16_38_174 "ATC: sadm mode_index = %d, dev_number = %d, %d"
#define ATC_BASIC_CMD_4152_112_2_17_22_16_38_175 "ATC:atc_config_ptr->q_value=%d, 0x%x "
#define ATC_BASIC_CMD_4159_112_2_17_22_16_38_176 "AGC_Get_Param failed to get mode name, mode:%d."
#define ATC_BASIC_CMD_4164_112_2_17_22_16_38_177 "ATC:mode_name_ptr: %s, 0x%x, 0x%x"
#define ATC_BASIC_CMD_4167_112_2_17_22_16_38_178 "AGC_Get_Param failed to get mode para."
#define ATC_BASIC_CMD_4172_112_2_17_22_16_38_179 "ATC:atc_config_ptr->q_value=%d, 0x%x, 0x%x, 0x%x"
#define ATC_BASIC_CMD_4175_112_2_17_22_16_38_180 "ATC:atc_config_ptr->q_value=%d, 0x%x, 0x%x, 0x%x"
#define ATC_BASIC_CMD_4185_112_2_17_22_16_38_181 "ATC:atc_config_ptr->q_value=%d, "
#define ATC_BASIC_CMD_4262_112_2_17_22_16_38_182 "ATC: sadm mode_index = %d, dev_number = %d"
#define ATC_BASIC_CMD_4269_112_2_17_22_16_38_183 "AGC_Get_Param failed to get mode name, mode:%d."
#define ATC_BASIC_CMD_4272_112_2_17_22_16_38_184 "AGC_Get_Param to get mode name, mode:%d, %s."
#define ATC_BASIC_CMD_4275_112_2_17_22_16_38_185 "ATC: AUDIONVARM_ReadModeParamFromFlash failed."
#define ATC_BASIC_CMD_4355_112_2_17_22_16_38_186 "ATC: Assert SADM,invalid type."
#define ATC_BASIC_CMD_4379_112_2_17_22_16_38_187 "ATC: ATC_ProcessSPADLVVE"
#define ATC_BASIC_CMD_4392_112_2_17_22_16_38_188 "ATC: param_1 is wrong"
#define ATC_BASIC_CMD_4404_112_2_17_22_16_38_189 "ATC: param_2 is wrong"
#define ATC_BASIC_CMD_4410_112_2_17_22_16_38_190 "ATC: param_2 is wrong"
#define ATC_BASIC_CMD_4426_112_2_17_22_16_38_191 "ATC: SADLVVE type = %d,mode_index = %d"
#define ATC_BASIC_CMD_4431_112_2_17_22_16_38_192 "ATC: LVVE_CTRL_PARAM_GetDevModeName failed to get mode name, mode:%d."
#define ATC_BASIC_CMD_4446_112_2_17_22_16_38_193 "ATC: SADLVVE type = %d,mode_index = %d, %d"
#define ATC_BASIC_CMD_4455_112_2_17_22_16_38_194 "LVVE_CTRL_PARAM_GetDevModeName failed to get mode name, mode:%d."
#define ATC_BASIC_CMD_4461_112_2_17_22_16_38_195 "ATC:mode_name_ptr: %s, 0x%x"
#define ATC_BASIC_CMD_4464_112_2_17_22_16_38_196 "LVVE_CTRL_PARAM_GetDevModeParam failed to get mode para."
#define ATC_BASIC_CMD_4522_112_2_17_22_16_39_197 "ATC: LVVE type = %d, mode_index = %d"
#define ATC_BASIC_CMD_4531_112_2_17_22_16_39_198 "ATC: LVVE_CTRL_PARAM_GetDevModeName failed to get mode name, mode:%d."
#define ATC_BASIC_CMD_4536_112_2_17_22_16_39_199 "ATC: AGC_Get_Param to get mode name, mode:%d, %s."
#define ATC_BASIC_CMD_4539_112_2_17_22_16_39_200 "ATC: LVVE_CTRL_PARAM_ReadModeParamFromFlash failed."
#define ATC_BASIC_CMD_4627_112_2_17_22_16_39_201 "ATC: SPLO get fplmn fail!\n"
#define ATC_BASIC_CMD_4632_112_2_17_22_16_39_202 "ATC: SPLO get fplmn success!\n"
#define ATC_BASIC_CMD_4652_112_2_17_22_16_39_203 "ATC: SPLO index = %d"
#define ATC_BASIC_CMD_4659_112_2_17_22_16_39_204 "ATC: SPLO plmn = %d"
#define ATC_BASIC_CMD_4667_112_2_17_22_16_39_205 "+CMS ERROR: mnc_digit_num can only be 2 or 3\n"
#define ATC_BASIC_CMD_4673_112_2_17_22_16_39_206 "+CMS ERROR: cannot input string in para3\n"
#define ATC_BASIC_CMD_4703_112_2_17_22_16_39_207 "+CMS ERROR: set FPLMN list fail\n"
#define ATC_BASIC_CMD_4716_112_2_17_22_16_39_208 "+CMS ERROR: set FPLMN list fail\n"
#define ATC_BASIC_CMD_4744_112_2_17_22_16_39_209 "+CMS ERROR: set FPLMN list fail\n"
#define ATC_BASIC_CMD_4774_112_2_17_22_16_39_210 "ATC: Assert SPLO,invalid type."
#define ATC_BASIC_CMD_4848_112_2_17_22_16_39_211 "ATC: siosadm setmode param result = %d"
#define ATC_BASIC_CMD_4855_112_2_17_22_16_39_212 "ATC: siosadm input data length error."
#define ATC_BASIC_CMD_4927_112_2_17_22_16_39_213 "ATC: snvm audio data fail, err = %d."
#define ATC_BASIC_CMD_4936_112_2_17_22_16_39_214 "ATC:SioSADMNVData,length:%d,terminater_char:%d,mode index:%d"
#define ATC_BASIC_CMD_4944_112_2_17_22_16_39_215 "ATC:cancel the snvm audio data!"
#define ATC_BASIC_CMD_4967_112_2_17_22_16_39_216 "ATC: write snvm audio data successful"
#define ATC_BASIC_CMD_4972_112_2_17_22_16_39_217 "ATC: write snvm audio data fail, err = %d."
#define ATC_BASIC_CMD_4992_112_2_17_22_16_40_218 "ATC:ConvertHexToBin failed."
#define ATC_BASIC_CMD_5005_112_2_17_22_16_40_219 "ATC:PROD_WriteAudioParamToFlash failed. result:%d"
#define ATC_BASIC_CMD_5016_112_2_17_22_16_40_220 "ATC:SioSADMNVData len error.length:%d != %d"
#define ATC_BASIC_CMD_5023_112_2_17_22_16_40_221 "ATC: snvm audio data format is error."
#define ATC_BASIC_CMD_5081_112_2_17_22_16_40_222 "ATC: siosadm for arm, param_len = %d"
#define ATC_BASIC_CMD_5094_112_2_17_22_16_40_223 "atc:ctrl_z sizeof(LVVE_CTRL_PARAM_NV_T)*2=%d"
#define ATC_BASIC_CMD_5098_112_2_17_22_16_40_224 "ATC:ATC_ProcessSioSADLVVEData %s, %d"
#define ATC_BASIC_CMD_5106_112_2_17_22_16_40_225 "atc: lvve_ctrl_param_nv_ptr->name len=%d"
#define ATC_BASIC_CMD_5109_112_2_17_22_16_40_226 "atc: ->name [%d]=%d "
#define ATC_BASIC_CMD_5117_112_2_17_22_16_40_227 "ATC: LVVE_CTRL_PARAM_SetDevModeParam , result = %d"
#define ATC_BASIC_CMD_5127_112_2_17_22_16_40_228 "ATC: ATC_ProcessSioSADLVVEData, input data length error."
#define ATC_BASIC_CMD_5135_112_2_17_22_16_40_229 "Lost the escape character is %s is line %d"
#define ATC_BASIC_CMD_5187_112_2_17_22_16_40_230 "ATC: siosadm for arm, param_len = %d"
#define ATC_BASIC_CMD_5197_112_2_17_22_16_40_231 "atc: ctrl_z, sizeof(LVVE_CTRL_PARAM_NV_T)*2=%d"
#define ATC_BASIC_CMD_5206_112_2_17_22_16_40_232 "ATC:ATC_ProcessSioSADLVVENVData %s, %d"
#define ATC_BASIC_CMD_5213_112_2_17_22_16_40_233 "ATC: ATC_ProcessSioSADLVVENVData , result = %d"
#define ATC_BASIC_CMD_5223_112_2_17_22_16_40_234 "ATC: ATC_ProcessSioSADLVVEData, input data length error."
#define ATC_BASIC_CMD_5231_112_2_17_22_16_40_235 "Lost the escape character is %s is line %d"
#define ATC_BASIC_CMD_5370_112_2_17_22_16_40_236 "ATC: Assert ACM,invalid type."
#define ATC_BASIC_CMD_5387_112_2_17_22_16_40_237 "ATC:No sim reason = %d"
#define ATC_BASIC_CMD_5510_112_2_17_22_16_41_238 "ATC:+WIND: 1    "
#define ATC_BASIC_CMD_5580_112_2_17_22_16_41_239 "ATC:+WIND: 2    "
#define ATC_BASIC_CMD_5798_112_2_17_22_16_41_240 "ATC: Assert Failure unknow creg."
#define ATC_BASIC_CMD_5821_112_2_17_22_16_41_241 "ATC: Assert Failure unknow cgreg."
#define ATC_BASIC_CMD_5871_112_2_17_22_16_41_242 "ATC: Assert Failure unknow creg."
#define ATC_BASIC_CMD_5894_112_2_17_22_16_41_243 "ATC: Assert Failure unknow cgreg."
#define ATC_BASIC_CMD_6176_112_2_17_22_16_42_244 "ATC: assert case dcs pcs."
#define ATC_BASIC_CMD_6184_112_2_17_22_16_42_245 "ATC: assert case dcs pcs."
#define ATC_BASIC_CMD_6202_112_2_17_22_16_42_246 "ATC: unknow band %d"
#define ATC_BASIC_CMD_6345_112_2_17_22_16_42_247 "ATC: snvm data format is error."
#define ATC_BASIC_CMD_6465_112_2_17_22_16_42_248 "ATC:+SRWD,source_address[0,1,2]=%d"
#define ATC_BASIC_CMD_6478_112_2_17_22_16_43_249 "ATC:+SRWD,source_address[0,1,2]=%x,%x,%x"
#define ATC_BASIC_CMD_6515_112_2_17_22_16_43_250 "ATC:+SRWD,error in params"
#define ATC_BASIC_CMD_6525_112_2_17_22_16_43_251 "ATC:Length of string is error!"
#define ATC_BASIC_CMD_6617_112_2_17_22_16_43_252 "ATC: Assert SIND,invalid type."
#define ATC_BASIC_CMD_6661_112_2_17_22_16_43_253 "ATC: Assert SUPS,invalid type."
#define ATC_BASIC_CMD_6730_112_2_17_22_16_43_254 "ATC:ATC_ProcessCrsmCnf"
#define ATC_BASIC_CMD_6733_112_2_17_22_16_43_255 "ATC:ATC_ProcessCrsmCnf,true"
#define ATC_BASIC_CMD_6748_112_2_17_22_16_43_256 "ATC:ATC_ProcessCrsmCnf,FAIL"
#define ATC_BASIC_CMD_6914_112_2_17_22_16_43_257 "ATC:ATC_ProcessSCINF"
#define ATC_BASIC_CMD_6982_112_2_17_22_16_44_258 "ATC: no network"
#define ATC_BASIC_CMD_7064_112_2_17_22_16_44_259 "ATC: no network"
#define ATC_BASIC_CMD_7078_112_2_17_22_16_44_260 "ATC: Test Scinf CMD"
#define ATC_BASIC_CMD_7085_112_2_17_22_16_44_261 "ATC: Assert SCINF,invalid type."
#define ATC_BASIC_CMD_7109_112_2_17_22_16_44_262 "ATC: ATC_ProcessSPEQPARA: <n> is invalid."
#define ATC_BASIC_CMD_7132_112_2_17_22_16_44_263 "ATC: ATC_ProcessSPEQPARA: <type> or <dest> is invalid."
#define ATC_BASIC_CMD_7154_112_2_17_22_16_44_264 "ATC: ATC_ProcessSPEQPARA: Get EQ PARA failed."
#define ATC_BASIC_CMD_7176_112_2_17_22_16_44_265 "ATC: ATC_ProcessSPEQPARA: <type> or <dest> or <data> is invalid."
#define ATC_BASIC_CMD_7183_112_2_17_22_16_44_266 "ATC: ATC_ProcessSPEQPARA: <data> type is not string."
#define ATC_BASIC_CMD_7189_112_2_17_22_16_44_267 "ATC: ATC_ProcessSPEQPARA: <data> length:%d != %d*2."
#define ATC_BASIC_CMD_7197_112_2_17_22_16_44_268 "ATC: ATC_ProcessSPEQPARA: ConvertHexToBin failed."
#define ATC_BASIC_CMD_7221_112_2_17_22_16_44_269 "ATC: ATC_ProcessSPEQPARA: Set EQ PARA failed."
#define ATC_BASIC_CMD_7256_112_2_17_22_16_44_270 "ATC:ATC_ProcessFTIN!"
#define ATC_BASIC_CMD_7265_112_2_17_22_16_44_271 "ATC: ATC_ProcessSFTIN not string."
#define ATC_BASIC_CMD_7317_112_2_17_22_16_44_272 "ATC_ProcessSFTIN: the operation type error!"
#define ATC_BASIC_CMD_7342_112_2_17_22_16_44_273 "ATC:ATC_ProcessSYTIN!"
#define ATC_BASIC_CMD_7351_112_2_17_22_16_44_274 "ATC: ATC_ProcessSYTIN not string."
#define ATC_BASIC_CMD_7399_112_2_17_22_16_44_275 "ATC_ProcessSYTIN: the operation type error!"
#define ATC_BASIC_CMD_7499_112_2_17_22_16_45_276 "ATC: spenha eq mode_index = %d, dev_number = %d, %d"
#define ATC_BASIC_CMD_7509_112_2_17_22_16_45_277 "AGC_Get_Param failed to get mode_index, mode_index :%d."
#define ATC_BASIC_CMD_7528_112_2_17_22_16_45_278 "ATC: spenha tun_eq mode_index = %d, dev_number = %d, %d"
#define ATC_BASIC_CMD_7538_112_2_17_22_16_45_279 "AGC_Get_Param failed to get mode_index, mode_index :%d."
#define ATC_BASIC_CMD_7644_112_2_17_22_16_45_280 "ATC: spenha eq mode_index = %d, dev_number = %d, %d"
#define ATC_BASIC_CMD_7653_112_2_17_22_16_45_281 "AGC_Get_Param failed to get mode_index, mode_index:%d."
#define ATC_BASIC_CMD_7671_112_2_17_22_16_45_282 "ATC: spenha tun_eq mode_index = %d, dev_number = %d, %d"
#define ATC_BASIC_CMD_7681_112_2_17_22_16_45_283 "AGC_Get_Param failed to get mode_index, mode_index:%d."
#define ATC_BASIC_CMD_7802_112_2_17_22_16_45_284 "ATC: siospenha for enha, param_len = %d"
#define ATC_BASIC_CMD_7824_112_2_17_22_16_45_285 "ATC: siospenha for enha, set result = %d"
#define ATC_BASIC_CMD_7834_112_2_17_22_16_45_286 "ATC: siospenha for enha, input data index error."
#define ATC_BASIC_CMD_7839_112_2_17_22_16_45_287 "ATC: siospenha for enha, input data length error."
#define ATC_BASIC_CMD_7844_112_2_17_22_16_45_288 "Lost the escape character is %s is line %d"
#define ATC_BASIC_CMD_7894_112_2_17_22_16_45_289 "ATC: siospenha for enha tun, param_len = %d"
#define ATC_BASIC_CMD_7916_112_2_17_22_16_45_290 "ATC: siospenha for enha tun, set result = %d"
#define ATC_BASIC_CMD_7927_112_2_17_22_16_45_291 "ATC: siospenha for enha tun, input data index error."
#define ATC_BASIC_CMD_7932_112_2_17_22_16_45_292 "ATC: siospenha for enha tun, input data length error."
#define ATC_BASIC_CMD_7937_112_2_17_22_16_45_293 "Lost the escape character is %s is line %d"
#define ATC_BASIC_CMD_7986_112_2_17_22_16_46_294 "ATC: siospenha for enha, param_len = %d"
#define ATC_BASIC_CMD_7995_112_2_17_22_16_46_295 "ATC: sizeof(AUDIO_ENHA_EQ_STRUCT_T)=%d"
#define ATC_BASIC_CMD_8010_112_2_17_22_16_46_296 "ATC: siospenha for enha, set result = %d"
#define ATC_BASIC_CMD_8020_112_2_17_22_16_46_297 "ATC: siospenha for enha, input data index error."
#define ATC_BASIC_CMD_8025_112_2_17_22_16_46_298 "ATC: siospenha for enha, input data length error."
#define ATC_BASIC_CMD_8030_112_2_17_22_16_46_299 "Lost the escape character is %s is line %d"
#define ATC_BASIC_CMD_8082_112_2_17_22_16_46_300 "ATC: siospenha for enha tun, param_len = %d"
#define ATC_BASIC_CMD_8091_112_2_17_22_16_46_301 "ATC: sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T)=%d"
#define ATC_BASIC_CMD_8106_112_2_17_22_16_46_302 "ATC: siospenha for enha tun, set result = %d"
#define ATC_BASIC_CMD_8117_112_2_17_22_16_46_303 "ATC: siospenha for enha tun, input data index error."
#define ATC_BASIC_CMD_8122_112_2_17_22_16_46_304 "ATC: siospenha for enha tun, input data length error."
#define ATC_BASIC_CMD_8127_112_2_17_22_16_46_305 "Lost the escape character is %s is line %d"
#define ATC_BASIC_CMD_8232_112_2_17_22_16_46_306 "ATC: timer string len error %d"
#define ATC_BASIC_CMD_8243_112_2_17_22_16_46_307 "ATC: timer string format error"
#define ATC_BASIC_CMD_8252_112_2_17_22_16_46_308 "ATC: value of year error"
#define ATC_BASIC_CMD_8263_112_2_17_22_16_46_309 "ATC: month digital error"
#define ATC_BASIC_CMD_8269_112_2_17_22_16_46_310 "ATC: month value error"
#define ATC_BASIC_CMD_8277_112_2_17_22_16_46_311 "ATC: day value error"
#define ATC_BASIC_CMD_8285_112_2_17_22_16_46_312 "ATC: hour value error"
#define ATC_BASIC_CMD_8293_112_2_17_22_16_46_313 "ATC: minute value error"
#define ATC_BASIC_CMD_8302_112_2_17_22_16_46_314 "ATC: second value error"
#define ATC_BASIC_CMD_8357_112_2_17_22_16_46_315 "ATC: save ate value: %d"
#define ATC_BASIC_CMD_8397_112_2_17_22_16_46_316 "ATC: siosadm for arm, param_len = %d"
#define ATC_BASIC_CMD_8418_112_2_17_22_16_46_317 "ATC:ATC_ProcessSioARMSADMData %s, %s, %d."
#define ATC_BASIC_CMD_8428_112_2_17_22_16_46_318 "ATC: Audio Mode Name too long: %d"
#define ATC_BASIC_CMD_8434_112_2_17_22_16_46_319 "ATC:ATC_ProcessSioARMSADMData 2 %s, %s, %d."
#define ATC_BASIC_CMD_8439_112_2_17_22_16_46_320 "ATC: siosadm for arm, setmode param result = %d"
#define ATC_BASIC_CMD_8455_112_2_17_22_16_46_321 "ATC:ATC_ProcessSioARMSADMData: setModeInfo, Mode:0x%x, pucModeName:%s, mode:%d, oldMode:%d."
#define ATC_BASIC_CMD_8462_112_2_17_22_16_47_322 "ATC: siosadm for arm , input data index error."
#define ATC_BASIC_CMD_8467_112_2_17_22_16_47_323 "ATC: siosadm for arm , input data length error."
#define ATC_BASIC_CMD_8475_112_2_17_22_16_47_324 "Lost the escape character is %s is line %d"
#define ATC_BASIC_CMD_8522_112_2_17_22_16_47_325 "ATC:SioARMSADMNVData:length:%d,terminater_char:%d,mode index::%d"
#define ATC_BASIC_CMD_8530_112_2_17_22_16_47_326 "ATC:cancel the snvm audio data!"
#define ATC_BASIC_CMD_8546_112_2_17_22_16_47_327 "ATC:SioARMSADMNVData %s, %s, %d."
#define ATC_BASIC_CMD_8556_112_2_17_22_16_47_328 "ATC: Audio Mode Name too long: %d"
#define ATC_BASIC_CMD_8562_112_2_17_22_16_47_329 "ATC:SioARMSADMNVData 2 %s, %s, %d."
#define ATC_BASIC_CMD_8566_112_2_17_22_16_47_330 "ATC:ConvertHexToBin failed."
#define ATC_BASIC_CMD_8573_112_2_17_22_16_47_331 "ATC:SioARMSADMNVData failed. result:%d"
#define ATC_BASIC_CMD_8580_112_2_17_22_16_47_332 "ATC: siosadm for arm , input data index error."
#define ATC_BASIC_CMD_8586_112_2_17_22_16_47_333 "ATC: siosadmnv for arm, input audio data length error.length:%d != %d"
#define ATC_BASIC_CMD_8593_112_2_17_22_16_47_334 "ATC: snvm audio data format is error."
#define ATC_BASIC_CMD_8645_112_2_17_22_16_47_335 "ATC: siosadm for dsp, param_len = %d,terminater_char=%d"
#define ATC_BASIC_CMD_8656_112_2_17_22_16_47_336 "atc: sizeof(AUDIO_NV_DSP_MODE_STRUCT_T)=%d"
#define ATC_BASIC_CMD_8673_112_2_17_22_16_47_337 "ATC:ATC_ProcessSioDSPSADMData %s, %d."
#define ATC_BASIC_CMD_8683_112_2_17_22_16_47_338 "ATC: Audio Mode Name too long: %d"
#define ATC_BASIC_CMD_8689_112_2_17_22_16_47_339 "ATC:ATC_ProcessSioDSPSADMData 2, %s, %d."
#define ATC_BASIC_CMD_8694_112_2_17_22_16_47_340 "ATC: siosadm for dsp, setmode param result = %d"
#define ATC_BASIC_CMD_8704_112_2_17_22_16_47_341 "ATC:ATC_ProcessSioDSPSADMData: setModeInfo, Mode:0x%x, pucModeName:%s, mode:%d, oldMode:%d."
#define ATC_BASIC_CMD_8711_112_2_17_22_16_47_342 "ATC: siosadm for dsp , input data index error."
#define ATC_BASIC_CMD_8716_112_2_17_22_16_47_343 "ATC: siosadm for dsp , input data length error."
#define ATC_BASIC_CMD_8724_112_2_17_22_16_47_344 "Lost the escape character is %s is line %d"
#define ATC_BASIC_CMD_8771_112_2_17_22_16_47_345 "ATC:SioDSPSADMNVData:length:%d,terminater_char:%d,mode index::%d"
#define ATC_BASIC_CMD_8779_112_2_17_22_16_47_346 "ATC:cancel the snvm audio data!"
#define ATC_BASIC_CMD_8801_112_2_17_22_16_47_347 "ATC:SioDSPSADMNVData %s, %d."
#define ATC_BASIC_CMD_8811_112_2_17_22_16_47_348 "ATC: Audio Mode Name too long: %d"
#define ATC_BASIC_CMD_8818_112_2_17_22_16_47_349 "ATC:ConvertHexToBin failed."
#define ATC_BASIC_CMD_8825_112_2_17_22_16_47_350 "ATC:SioDSPSADMNVData failed. result:%d"
#define ATC_BASIC_CMD_8832_112_2_17_22_16_47_351 "ATC: siosadm for dsp , input data index error."
#define ATC_BASIC_CMD_8838_112_2_17_22_16_47_352 "ATC: siosadmnv for dsp, input audio data length error.length:%d != %d"
#define ATC_BASIC_CMD_8845_112_2_17_22_16_47_353 "ATC: snvm audio data format is error."
#define ATC_BASIC_CMD_8884_112_2_17_22_16_47_354 "ATC: SIO string len=%d,Data %x,%x,%x,%x,%x"
#define ATC_BASIC_CMD_8896_112_2_17_22_16_47_355 "ATC: cancel the given midi files"
#define ATC_BASIC_CMD_8903_112_2_17_22_16_47_356 "ATC: the midi file format is error"
#define ATC_BASIC_CMD_8929_112_2_17_22_16_47_357 "ATC: smgv SIO string lenght=%d,s_pre_block_chk_num=%d,Data1=%x,Data2=%x,Data3=%x,Data4=%x,Data5=%x, terminator = 0x%x"
#define ATC_BASIC_CMD_8939_112_2_17_22_16_48_358 "ATC: smgv receive a frame head."
#define ATC_BASIC_CMD_8969_112_2_17_22_16_48_359 "ATC:midi file format error"
#define ATC_BASIC_CMD_9247_112_2_17_22_16_48_360 "ATC: Assert CBC,invalid type."
#define ATC_BASIC_CMD_9408_112_2_17_22_16_49_361 "ATC: Assert S2,invalid type."
#define ATC_BASIC_CMD_9619_112_2_17_22_16_49_362 "ATC: Assert ECHO,invalid type."
#define ATC_BASIC_CMD_9664_112_2_17_22_16_50_363 "ATC: Assert CICB,invalid type."
#define ATC_BASIC_CMD_9742_112_2_17_22_16_50_364 "ATC: Assert IPR,invalid type."
#define ATC_BASIC_CMD_9767_112_2_17_22_16_50_365 "ATC: CRMP"
#define ATC_BASIC_CMD_9800_112_2_17_22_16_50_366 "VOIC, DATA and FAX CALLS are not supported now"
#define ATC_BASIC_CMD_9813_112_2_17_22_16_50_367 "ATC: Assert CRMP,invalid type."
#define ATC_BASIC_CMD_9966_112_2_17_22_16_50_368 "ATC: SNVM read nv item id = %d fail, err = %d"
#define ATC_BASIC_CMD_10043_112_2_17_22_16_51_369 "ATC: Assert SLOG,invalid type."
#define ATC_BASIC_CMD_10095_112_2_17_22_16_51_370 "ATC: need gpio direction parameter!"
#define ATC_BASIC_CMD_10127_112_2_17_22_16_51_371 "ATC: Assert Failure receive gpio type = %d"
#define ATC_BASIC_CMD_10136_112_2_17_22_16_51_372 "ATC: Assert SSGF,invalid type."
#define ATC_BASIC_CMD_10189_112_2_17_22_16_51_373 "ATC: need gpio set parameter!"
#define ATC_BASIC_CMD_10239_112_2_17_22_16_51_374 "ATC: Assert Failure receive gpio type = %d"
#define ATC_BASIC_CMD_10248_112_2_17_22_16_51_375 "ATC: Assert SSGS,invalid type."
#define ATC_BASIC_CMD_10276_112_2_17_22_16_51_376 "ATC:ATC_ProcessCRSM"
#define ATC_BASIC_CMD_10585_112_2_17_22_16_52_377 "FIRBOOT:EFS_NvitemWrite error"
#define ATC_BASIC_CMD_10591_112_2_17_22_16_52_378 "winnie:read seed:%s"
#define ATC_BASIC_CMD_10653_112_2_17_22_16_52_379 "ATC: sarft sign data fail."
#define ATC_BASIC_CMD_10738_112_2_17_22_16_52_380 "ATC: sarft sign data fail."
#define ATC_BASIC_CMD_10754_112_2_17_22_16_52_381 "ATC: sarft sign data fail."
#define ATC_BASIC_CMD_10771_112_2_17_22_16_52_382 "ATC: sarft sign data fail."
#define ATC_BASIC_CMD_10785_112_2_17_22_16_52_383 "ATC: sarft sign data fail."
#define ATC_BASIC_CMD_10833_112_2_17_22_16_52_384 "ATC: sim_id=%d, ncell_index=%d, arfcn=%d, bsic=%d, rxlev=%d, mcc=%d, mnc=%d, lac=%d, mnc_digit_num=%d, cell_id=%d"
#define ATC_BASIC_CMD_10864_112_2_17_22_16_52_385 "ATC: sim_id=%d, scell_index=%d, arfcn=%d, bsic=%d, rxlev=%d, mcc=%d, mnc=%d, lac=%d, mnc_digit_num=%d, cell_id=%d"
#define ATC_BASIC_CMD_10870_112_2_17_22_16_52_386 "ATC_ProcessPOSI sim %d, ncell_has_scell %d"
#define ATC_BASIC_CMD_10878_112_2_17_22_16_52_387 "ATC_ProcessPOSI ncell_num = 0"
#define ATC_BASIC_CMD_10897_112_2_17_22_16_53_388 "ATC: cell_num=%d, cell_power[%d].rxlev=%d "
#define ATC_BASIC_CMD_10902_112_2_17_22_16_53_389 "ATC_ProcessPOSI invalid cell mnc %d,cell_id %x, rxlev %d"
#define ATC_BASIC_CMD_10910_112_2_17_22_16_53_390 "ATC POSI: %s"
#define ATC_BASIC_CMD_10928_112_2_17_22_16_53_391 "ATC: sarft sign data fail."
#define ATC_BASIC_CMD_10930_112_2_17_22_16_53_392 "ATC_ProcessPOSI sign str %s"
#define ATC_BASIC_CMD_10940_112_2_17_22_16_53_393 "ATC: sarft sign data fail."
#define ATC_BASIC_CMD_10949_112_2_17_22_16_53_394 "ATC_ProcessPOSI: %s"
#define ATC_BASIC_CMD_10950_112_2_17_22_16_53_395 "ATC: the string length is %d,cell_num=%d"
#define ATC_COMMON_457_112_2_17_22_16_54_396 "Log ATC infomation:\n"
#define ATC_COMMON_461_112_2_17_22_16_54_397 "AT Command Index : %d\n"
#define ATC_COMMON_462_112_2_17_22_16_54_398 "AT Command Type :"
#define ATC_COMMON_466_112_2_17_22_16_54_399 " BASIC"
#define ATC_COMMON_471_112_2_17_22_16_54_400 " EXTEND"
#define ATC_COMMON_476_112_2_17_22_16_54_401 " EXECUTE"
#define ATC_COMMON_481_112_2_17_22_16_54_402 " SET"
#define ATC_COMMON_486_112_2_17_22_16_54_403 " READ"
#define ATC_COMMON_491_112_2_17_22_16_54_404 " TEST"
#define ATC_COMMON_494_112_2_17_22_16_54_405 "AT Command Parameter Number : %d\n"
#define ATC_COMMON_507_112_2_17_22_16_54_406 "AT Command Parameter %d : %dn"
#define ATC_COMMON_515_112_2_17_22_16_54_407 "AT Command Parameter %d : %sn"
#define ATC_COMMON_521_112_2_17_22_16_54_408 "AT Command Parameter %d : Default\n"
#define ATC_COMMON_545_112_2_17_22_16_54_409 "%s\n"
#define ATC_COMMON_566_112_2_17_22_16_54_410 "ATC: Assert Failure bcd len."
#define ATC_COMMON_1344_112_2_17_22_16_56_411 "atc: txt %x, %x, %x, %x"
#define ATC_COMMON_1351_112_2_17_22_16_56_412 "atc: tmp %x, %x, %x, %x"
#define ATC_COMMON_1379_112_2_17_22_16_56_413 "atc: tmp %x, %x, %x, %x"
#define ATC_COMMON_1419_112_2_17_22_16_56_414 "atc: tmp %x, %x, %x, %x"
#define ATC_COMMON_1529_112_2_17_22_16_56_415 "ATC: Assert Failure rsp len."
#define ATC_COMMON_1562_112_2_17_22_16_56_416 "ATC: GetIndexOfCallContext out of range, call_id = %d"
#define ATC_GPRS_MODEM_50_112_2_17_22_16_57_417 "ATC: ATC_ProcessNETAPN"
#define ATC_GPRS_MODEM_91_112_2_17_22_16_57_418 "ATC: MNGPRS_SetPdpContextEx FAIL,%d"
#define ATC_GPRS_MODEM_110_112_2_17_22_16_57_419 "ATC: set username or password error, result = %d"
#define ATC_GPRS_MODEM_116_112_2_17_22_16_57_420 "ATC: username or password too long."
#define ATC_GPRS_MODEM_142_112_2_17_22_16_57_421 "atc: ProcessSOCKET"
#define ATC_GPRS_MODEM_163_112_2_17_22_16_57_422 "atc: socket %d is still alive, not allowed to change it"
#define ATC_GPRS_MODEM_185_112_2_17_22_16_57_423 "atc: connection type=%s"
#define ATC_GPRS_MODEM_210_112_2_17_22_16_58_424 "atc: ProcessSOCKET, param3 is not string!"
#define ATC_GPRS_MODEM_219_112_2_17_22_16_58_425 "atc: malloc for sock %d IP address fail"
#define ATC_GPRS_MODEM_223_112_2_17_22_16_58_426 "atc: sock %d IP address malloc here"
#define ATC_GPRS_MODEM_227_112_2_17_22_16_58_427 "atc: sock %d IP address has been malloc, only memset it"
#define ATC_GPRS_MODEM_244_112_2_17_22_16_58_428 "atc: socket index %d port = %d"
#define ATC_GPRS_MODEM_277_112_2_17_22_16_58_429 "atc: ProcessIPOPEN\n"
#define ATC_GPRS_MODEM_281_112_2_17_22_16_58_430 "ATC: ProcessIPOPEN atc_config_ptr pointer is PNULL"
#define ATC_GPRS_MODEM_287_112_2_17_22_16_58_431 "ATC: ProcessIPOPEN cur_cmd_info pointer is PNULL"
#define ATC_GPRS_MODEM_304_112_2_17_22_16_58_432 "atc: socket %d is set previously!"
#define ATC_GPRS_MODEM_313_112_2_17_22_16_58_433 "atc: socket %d is still alive, not allowed to reopen it"
#define ATC_GPRS_MODEM_315_112_2_17_22_16_58_434 "atc: sock %d still alive, TCP +IPOPEN=%d"
#define ATC_GPRS_MODEM_322_112_2_17_22_16_58_435 "ATC: connect socket id error, not (0--9)"
#define ATC_GPRS_MODEM_331_112_2_17_22_16_58_436 "atc:PDP not activated, id = %x is set"
#define ATC_GPRS_MODEM_336_112_2_17_22_16_58_437 "atc: active pdp ret =%d"
#define ATC_GPRS_MODEM_340_112_2_17_22_16_58_438 "atc: active pdp fail"
#define ATC_GPRS_MODEM_346_112_2_17_22_16_58_439 "atc: s_is_pdp_active_flag = TRUE"
#define ATC_GPRS_MODEM_354_112_2_17_22_16_58_440 "atc:requested IP= %s, request id=%d"
#define ATC_GPRS_MODEM_358_112_2_17_22_16_58_441 "atc: sock index %d DNS fail"
#define ATC_GPRS_MODEM_369_112_2_17_22_16_58_442 "atc: PDP is not activated, pls dns later"
#define ATC_GPRS_MODEM_399_112_2_17_22_16_58_443 "atc: ProcessIPSEND\n"
#define ATC_GPRS_MODEM_403_112_2_17_22_16_58_444 "atc: ProcessIPSEND atc_config_ptr pointer is PNULL"
#define ATC_GPRS_MODEM_409_112_2_17_22_16_58_445 "atc: ProcessIPSEND cur_cmd_info pointer is PNULL"
#define ATC_GPRS_MODEM_426_112_2_17_22_16_58_446 "atc: socket %d is set previously!"
#define ATC_GPRS_MODEM_432_112_2_17_22_16_58_447 "atc: input ID outside of connect ID range!"
#define ATC_GPRS_MODEM_445_112_2_17_22_16_58_448 "atc: data state is not avtive"
#define ATC_GPRS_MODEM_453_112_2_17_22_16_58_449 "ATC: ProcessIPSEND, param2 is not numeral!"
#define ATC_GPRS_MODEM_485_112_2_17_22_16_58_450 "atc:  MUX ENABLE  link_id != ATC_DEBUG_LINK"
#define ATC_GPRS_MODEM_502_112_2_17_22_16_58_451 "atc:set ctrl Z gprs end char"
#define ATC_GPRS_MODEM_528_112_2_17_22_16_58_452 "ATC: ProcessIPCLOSE"
#define ATC_GPRS_MODEM_548_112_2_17_22_16_58_453 "ATC: the ID has not been set previously!"
#define ATC_GPRS_MODEM_554_112_2_17_22_16_58_454 "ATC: ProcessIPCLOSE, param ID out of its range!"
#define ATC_GPRS_MODEM_568_112_2_17_22_16_58_455 "atc: socket close failure ! "
#define ATC_GPRS_MODEM_575_112_2_17_22_16_58_456 "atc: sock_id %d addr_str free here"
#define ATC_GPRS_MODEM_578_112_2_17_22_16_58_457 "atc: current socket num is %d after closed"
#define ATC_GPRS_MODEM_586_112_2_17_22_16_58_458 "atc: deactive pdp context fail"
#define ATC_GPRS_MODEM_593_112_2_17_22_16_58_459 "atc: deactivate PDP success"
#define ATC_GPRS_MODEM_599_112_2_17_22_16_58_460 "atc: +IPCLOSE: %d"
#define ATC_GPRS_MODEM_605_112_2_17_22_16_58_461 "atc: socket do not exist"
#define ATC_GPRS_MODEM_627_112_2_17_22_16_58_462 "atc: _socketSend data_len=%d, unsend_len=%d"
#define ATC_GPRS_MODEM_632_112_2_17_22_16_58_463 "ATC:send data =%d"
#define ATC_GPRS_MODEM_651_112_2_17_22_16_58_464 "ATC: ATC_SocketSend sock_error_code=%d "
#define ATC_GPRS_MODEM_672_112_2_17_22_16_58_465 "ATC:ATC_SocketRecv rx_len =%d"
#define ATC_GPRS_MODEM_680_112_2_17_22_16_59_466 "ATC: g_rsp_str len=%d"
#define ATC_GPRS_MODEM_685_112_2_17_22_16_59_467 "ATC:g_rsp_str len=%d, rx_len=%d"
#define ATC_GPRS_MODEM_691_112_2_17_22_16_59_468 "atc: hex len=%d"
#define ATC_GPRS_MODEM_732_112_2_17_22_16_59_469 "atc:  _ProcessSioData pdu data =%s, terminater_char=%x"
#define ATC_GPRS_MODEM_749_112_2_17_22_16_59_470 "atc: receive ESC, sock index = %d "
#define ATC_GPRS_MODEM_771_112_2_17_22_16_59_471 "atc: sock index %d has process at+ipsend"
#define ATC_GPRS_MODEM_776_112_2_17_22_16_59_472 "atc: atc_tx_buf.data_len=%d"
#define ATC_GPRS_MODEM_787_112_2_17_22_16_59_473 "atc: ConvertHexToBin fail "
#define ATC_GPRS_MODEM_795_112_2_17_22_16_59_474 "atc: sock %d, socket id = %d"
#define ATC_GPRS_MODEM_821_112_2_17_22_16_59_475 "atc: ActPdpContextCnf"
#define ATC_GPRS_MODEM_827_112_2_17_22_16_59_476 "atc: PDP activate success s_nsapi=%d"
#define ATC_GPRS_MODEM_830_112_2_17_22_16_59_477 "atc:reqeuested sock %d IP= %s, ret=%d"
#define ATC_GPRS_MODEM_834_112_2_17_22_16_59_478 "atc: sock index %d dns fail"
#define ATC_GPRS_MODEM_849_112_2_17_22_16_59_479 "atc: sock index result unsuccess"
#define ATC_GPRS_MODEM_864_112_2_17_22_16_59_480 "atc: _ProcessSocketEvent, sig code =%d"
#define ATC_GPRS_MODEM_874_112_2_17_22_16_59_481 "atc: _GETHOSTBYNAME_CNF, error_code=%d"
#define ATC_GPRS_MODEM_887_112_2_17_22_16_59_482 "atc: request_id=%d, i=%d,sock index=%d"
#define ATC_GPRS_MODEM_898_112_2_17_22_16_59_483 "atc: UDP +IPOPEN=%d"
#define ATC_GPRS_MODEM_908_112_2_17_22_16_59_484 "atc:  Create And Connect TCP/UDP Socket fail, ret=%d"
#define ATC_GPRS_MODEM_920_112_2_17_22_16_59_485 "atc: SOCKET_CONNECT_EVENT_IND"
#define ATC_GPRS_MODEM_927_112_2_17_22_16_59_486 "atc: sock_index %d, socket_id=%d,  i=%d"
#define ATC_GPRS_MODEM_941_112_2_17_22_16_59_487 "atc: TCP +IPOPEN=%d"
#define ATC_GPRS_MODEM_946_112_2_17_22_16_59_488 "atc: connect error, error_code =%d"
#define ATC_GPRS_MODEM_957_112_2_17_22_16_59_489 "atc: SOCKET_WRITE_EVENT_IND"
#define ATC_GPRS_MODEM_966_112_2_17_22_16_59_490 "atc: socket_id=%d, i=%d,sock_index=%d"
#define ATC_GPRS_MODEM_976_112_2_17_22_16_59_491 "atc: SOCKET_READ_EVENT_IND"
#define ATC_GPRS_MODEM_984_112_2_17_22_16_59_492 "atc: socket_id=%d, i=%d,sock_index=%d"
#define ATC_GPRS_MODEM_997_112_2_17_22_16_59_493 "ATC: event type error"
#define ATC_GPRS_MODEM_1014_112_2_17_22_16_59_494 "ATC: _CreateAndConnectTCPSocket"
#define ATC_GPRS_MODEM_1020_112_2_17_22_16_59_495 "atc: create socket fail!"
#define ATC_GPRS_MODEM_1029_112_2_17_22_16_59_496 "ATC: sock index %d, socket id= %d, current sock num is %d"
#define ATC_GPRS_MODEM_1035_112_2_17_22_16_59_497 "atc: TCP SOCKET ERROR !"
#define ATC_GPRS_MODEM_1042_112_2_17_22_16_59_498 "ATC:IP=%d, port=%d"
#define ATC_GPRS_MODEM_1051_112_2_17_22_16_59_499 "ATC: socket connect sock_error_code=%d "
#define ATC_GPRS_MODEM_1058_112_2_17_22_16_59_500 "atc: socket connect to server fail, ret=%d"
#define ATC_GPRS_MODEM_1079_112_2_17_22_16_59_501 "atc: Create UDP Socket"
#define ATC_GPRS_MODEM_1084_112_2_17_22_16_59_502 "atc: create UDP socket fail!"
#define ATC_GPRS_MODEM_1092_112_2_17_22_16_59_503 "ATC: Create UDP Socket socket id= %d, current sock num is %d"
#define ATC_GPRS_MODEM_1098_112_2_17_22_16_59_504 "atc: UDP SOCKET ERROR !"
#define ATC_GPRS_MODEM_1105_112_2_17_22_16_59_505 "ATC: UDP IP=%d, port=%d"
#define ATC_GPRS_MODEM_1111_112_2_17_22_16_59_506 "ATC: UDP socket connect sock_error_code=%d "
#define ATC_GPRS_MODEM_1128_112_2_17_22_16_59_507 "ATC: ATC_ProcessIPDACT"
#define ATC_GPRS_MODEM_1138_112_2_17_22_16_59_508 "Detach failed!"
#define ATC_GPRS_MODEM_1163_112_2_17_22_17_0_509 "atc: PDP deactivated  by network"
#define ATC_GPRS_MODEM_1176_112_2_17_22_17_0_510 "atc: sock_id %d addr_str free here"
#define ATC_GSM_242_112_2_17_22_17_0_511 "ATC: ATC_ProcessA, MNCALL_ConnectCall failure!"
#define ATC_GSM_306_112_2_17_22_17_0_512 "ATC: rra is traffic when atd*99#"
#define ATC_GSM_377_112_2_17_22_17_0_513 "ATC: this is a emergency call!"
#define ATC_GSM_457_112_2_17_22_17_1_514 "ATC: rra is traffic when gsm atd"
#define ATC_GSM_519_112_2_17_22_17_1_515 "ATC: Assert Failure call_amount error %d."
#define ATC_GSM_704_112_2_17_22_17_1_516 "ATC: ATD*99 or 98#  RequestPdpId pdp_id:%d"
#define ATC_GSM_805_112_2_17_22_17_1_517 "ATC: no active call found."
#define ATC_GSM_809_112_2_17_22_17_1_518 "ATC: send dtmf id = %d"
#define ATC_GSM_830_112_2_17_22_17_1_519 "ATC: StartDTMF = %d"
#define ATC_GSM_844_112_2_17_22_17_1_520 "ATC: StopDTMF!"
#define ATC_GSM_860_112_2_17_22_17_1_521 "ATC: start a DTMF, but not stop it, operation refused!"
#define ATC_GSM_928_112_2_17_22_17_2_522 "ATC: ATC_ProcessCOPS, not allowed when CALL is existed."
#define ATC_GSM_942_112_2_17_22_17_2_523 "ATC: ATC_ProcessCOPS, sim is not ready or cpin code is not ready."
#define ATC_GSM_959_112_2_17_22_17_2_524 "ATC: MNPHONE_SelectPLMN failure"
#define ATC_GSM_1017_112_2_17_22_17_2_525 "ATC: Assert Failure unknow creg."
#define ATC_GSM_1040_112_2_17_22_17_2_526 "ATC: Assert Failure unknow cgreg"
#define ATC_GSM_1184_112_2_17_22_17_2_527 "ATC: Assert Failure plmn_status."
#define ATC_GSM_1196_112_2_17_22_17_2_528 "ATC: search the plmn list"
#define ATC_GSM_1206_112_2_17_22_17_2_529 "ATC: ATC_ProcessCOPS, failure in MNPHONE_ListPLMN."
#define ATC_GSM_1552_112_2_17_22_17_3_530 "ATC: received the plmn list confirm "
#define ATC_GSM_1553_112_2_17_22_17_3_531 "ATC: %d,%d"
#define ATC_GSM_1593_112_2_17_22_17_3_532 "ACT: RAT value is wrong"
#define ATC_GSM_1710_112_2_17_22_17_3_533 "ATC: Assert Failure unknow plmn_status."
#define ATC_GSM_1805_112_2_17_22_17_4_534 "ATC: ManualSelectPlmn, FULL_NAME, not found!"
#define ATC_GSM_1818_112_2_17_22_17_4_535 "ATC: ManualSelectPlmn, SHORT_NAME, not found!"
#define ATC_GSM_1835_112_2_17_22_17_4_536 "ATC: ManualSelectPlmn, NUMBERIC, cops not support this plmn"
#define ATC_GSM_1846_112_2_17_22_17_4_537 "ATC: ManualSelectPlmn, plmn = %d"
#define ATC_GSM_1862_112_2_17_22_17_4_538 "ATC: ManualSelectPlmn, cops not support this plmn"
#define ATC_GSM_1872_112_2_17_22_17_4_539 "ATC: ManualSelectPlmn, failure in MN"
#define ATC_GSM_1896_112_2_17_22_17_4_540 "ATC: ATC_GetPlmnDetailedInfo, Invalid parameters"
#define ATC_GSM_1908_112_2_17_22_17_4_541 "ATC: ATC_GetPlmnDetailedInfo, FULL_NAME is too long (%d)"
#define ATC_GSM_1933_112_2_17_22_17_4_542 "ATC: ATC_GetPlmnDetailedInfo, SHORT_NAME is too long (%d)"
#define ATC_GSM_2013_112_2_17_22_17_4_543 "ATC:invalid plmn num %c"
#define ATC_GSM_2100_112_2_17_22_17_4_544 "ATC: Assert Failure call_amount %d."
#define ATC_GSM_2127_112_2_17_22_17_4_545 "ATC: call amount %d"
#define ATC_GSM_2186_112_2_17_22_17_4_546 "ATC: no call number present!"
#define ATC_GSM_2217_112_2_17_22_17_5_547 "ATC: force detach sim %d pdp %d"
#define ATC_GSM_2234_112_2_17_22_17_5_548 "ATC: send sms fail when cring"
#define ATC_GSM_2239_112_2_17_22_17_5_549 "ATC: call amount %d"
#define ATC_GSM_2365_112_2_17_22_17_5_550 "ATC: plmn %s"
#define ATC_GSM_2397_112_2_17_22_17_5_551 "atc: ATC_ProcessCallDiscInd"
#define ATC_GSM_2405_112_2_17_22_17_5_552 "atc: receive APP_MN_CALL_DISCONNECTED_IND call id =%d"
#define ATC_GSM_2414_112_2_17_22_17_5_553 "atc: receive APP_MN_CALL_ERR_IND call id =%d"
#define ATC_GSM_2420_112_2_17_22_17_5_554 "ATC: Assert Failure unknow disc code."
#define ATC_GSM_2472_112_2_17_22_17_5_555 "atc: ss state %x,cpas %d"
#define ATC_GSM_2607_112_2_17_22_17_5_556 "ATC: unexpected timer expire"
#define ATC_GSM_2666_112_2_17_22_17_5_557 "ATC: the dtmf value is %c"
#define ATC_GSM_2693_112_2_17_22_17_6_558 "ATC: Assert Failure error call id %d."
#define ATC_GSM_SMS_411_112_2_17_22_17_7_559 "ATC:CMGL wrong type."
#define ATC_GSM_SMS_440_112_2_17_22_17_7_560 "ATC:CMGL wrong type."
#define ATC_GSM_SMS_498_112_2_17_22_17_7_561 "ATC:CSMS wrong type."
#define ATC_GSM_SMS_545_112_2_17_22_17_7_562 "ATC:CSDH wrong type."
#define ATC_GSM_SMS_575_112_2_17_22_17_7_563 "ATC: Get sc failure"
#define ATC_GSM_SMS_582_112_2_17_22_17_7_564 "ATC: Read s_sms_nv_config failure"
#define ATC_GSM_SMS_614_112_2_17_22_17_7_565 "ATC: write NV_ATC_CONFIG_ID fail"
#define ATC_GSM_SMS_668_112_2_17_22_17_7_566 "ATC:+CPMS,error in para1"
#define ATC_GSM_SMS_697_112_2_17_22_17_7_567 "ATC:+CPMS,error in para2"
#define ATC_GSM_SMS_770_112_2_17_22_17_8_568 "ATC:+CPMS,error in para"
#define ATC_GSM_SMS_1049_112_2_17_22_17_8_569 "ATC: Get sc failure"
#define ATC_GSM_SMS_1323_112_2_17_22_17_9_570 "ATC: Get sc failure"
#define ATC_GSM_SMS_1330_112_2_17_22_17_9_571 "ATC: Read s_sms_nv_config failure"
#define ATC_GSM_SMS_1593_112_2_17_22_17_9_572 "ATC: Get sc failure"
#define ATC_GSM_SMS_1600_112_2_17_22_17_9_573 "ATC: Read s_sms_nv_config failure"
#define ATC_GSM_SMS_1668_112_2_17_22_17_9_574 "ATC: Read msg and language from NV fail"
#define ATC_GSM_SMS_1800_112_2_17_22_17_10_575 "ATC:MNSMS_GetSmsStatus is error"
#define ATC_GSM_SMS_1823_112_2_17_22_17_10_576 "ATC:status_flag = FALSE"
#define ATC_GSM_SMS_1829_112_2_17_22_17_10_577 "ATC:ChangeSmsStatus is FALSE"
#define ATC_GSM_SMS_1896_112_2_17_22_17_10_578 "ATC: ATC_ProcessSFSMSM is called"
#define ATC_GSM_SMS_1970_112_2_17_22_17_10_579 "ATC:SFSMSM assert wrong para"
#define ATC_GSM_SMS_2133_112_2_17_22_17_10_580 "ATC: write NV_ATC_CONFIG_ID fail"
#define ATC_GSM_SMS_2199_112_2_17_22_17_10_581 "ATC:enter HandlePduModeSms"
#define ATC_GSM_SMS_2206_112_2_17_22_17_10_582 "ATC:error ConvertHexToBin"
#define ATC_GSM_SMS_2216_112_2_17_22_17_10_583 "ATC:no SC address"
#define ATC_GSM_SMS_2220_112_2_17_22_17_10_584 "ATC:please set the SC address."
#define ATC_GSM_SMS_2241_112_2_17_22_17_10_585 "ATC: HandlePduModeSms sc_addr.length = %d"
#define ATC_GSM_SMS_2259_112_2_17_22_17_10_586 "ATC:have SC address"
#define ATC_GSM_SMS_2263_112_2_17_22_17_10_587 "ATC:SC address length %d out range(1-%d)"
#define ATC_GSM_SMS_2478_112_2_17_22_17_11_588 "ATC:ATC_ProcessSendSmsCnf ignore result %d"
#define ATC_GSM_SMS_2523_112_2_17_22_17_11_589 "ATC: Receive the error status: %d"
#define ATC_GSM_SMS_2569_112_2_17_22_17_11_590 "ATC: Received APP_MN_UPDATE_SMS_STATE_CNF, sleep 30ms"
#define ATC_GSM_SMS_2764_112_2_17_22_17_11_591 "ATC: the ASSERT sms_state"
#define ATC_GSM_SMS_2772_112_2_17_22_17_11_592 "ATC: the current cmgl index: %d"
#define ATC_GSM_SMS_2789_112_2_17_22_17_12_593 "ATC: enter into ATC_ProcessWriteSmsCnf"
#define ATC_GSM_SMS_2807_112_2_17_22_17_12_594 "ATC:WRITE error!"
#define ATC_GSM_SMS_2868_112_2_17_22_17_12_595 "ATC:WRITE error, Stopping Filling SMS Storage!"
#define ATC_GSM_SMS_2896_112_2_17_22_17_12_596 "ATC: ATC_ProcessSmsFullInd is called"
#define ATC_GSM_SMS_2902_112_2_17_22_17_12_597 "ATC:SFSMSM Filled Successfully!"
#define ATC_GSM_SMS_2909_112_2_17_22_17_12_598 "ATC: mem full %d"
#define ATC_GSM_SMS_2933_112_2_17_22_17_12_599 "ATC:SFSMSM Filled Successfully!"
#define ATC_GSM_SMS_2958_112_2_17_22_17_12_600 "ATC: Get sc failure"
#define ATC_GSM_SMS_3007_112_2_17_22_17_12_601 "ATC:PDU allocation fail,exit +CMT!"
#define ATC_GSM_SMS_3046_112_2_17_22_17_12_602 "ATC: disable the status report"
#define ATC_GSM_SMS_3122_112_2_17_22_17_12_603 "ATC:assert wrong dcs type!!"
#define ATC_GSM_SMS_3213_112_2_17_22_17_12_604 "ATC:status_report_is_request=%d"
#define ATC_GSM_SMS_3400_112_2_17_22_17_13_605 "ATC: Enter the HandleTextModeSmsReadCnf routine"
#define ATC_GSM_SMS_3430_112_2_17_22_17_13_606 "ATC: SMS FREE SPACE!"
#define ATC_GSM_SMS_3583_112_2_17_22_17_13_607 "ATC:assert wrong sms_status!"
#define ATC_GSM_SMS_3590_112_2_17_22_17_13_608 "ATC: The SMS data len is %d"
#define ATC_GSM_SMS_3665_112_2_17_22_17_13_609 "ATC: Received  APP_MN_READ_SMS_CNF, text mode, sleep 30ms"
#define ATC_GSM_SMS_3710_112_2_17_22_17_13_610 "ATC: the SMS len is %d"
#define ATC_GSM_SMS_3780_112_2_17_22_17_14_611 "ATC: pdu read sms cnf, mux link id %d"
#define ATC_GSM_SMS_3789_112_2_17_22_17_14_612 "ATC: Read a free SMS space."
#define ATC_GSM_SMS_3892_112_2_17_22_17_14_613 "ATC: Received  APP_MN_READ_SMS_CNF, pdu mode, sleep 30ms"
#define ATC_GSM_SMS_3974_112_2_17_22_17_14_614 "ATC:PDU allocation fail,exit +CMT!"
#define ATC_GSM_SMS_4076_112_2_17_22_17_14_615 "ATC: The temp_len is %d"
#define ATC_GSM_SMS_4243_112_2_17_22_17_14_616 "ATC:assert wrong PduModeSmsReadCnf!"
#define ATC_GSM_SMS_4252_112_2_17_22_17_14_617 "ATC:the current cmgl index: %d"
#define ATC_GSM_SMS_4434_112_2_17_22_17_15_618 "ATC: Write NV_ATC_CB_MSG_ID failed"
#define ATC_GSM_SMS_4439_112_2_17_22_17_15_619 "ATC: Write NV_ATC_CB_LANG_ID failed"
#define ATC_GSM_SMS_4541_112_2_17_22_17_15_620 "ATC:BuildSmsCodeRsp,rl_cause is %d"
#define ATC_GSM_SMS_4542_112_2_17_22_17_15_621 "ATC:BuildSmsCodeRsp,The string=%s"
#define ATC_GSM_SMS_4587_112_2_17_22_17_15_622 "ATC: enter into Atc_FillMtMoSmsEx"
#define ATC_GSM_SMS_4629_112_2_17_22_17_15_623 "ATC: enter into Atc_FreeMemForSfsmsm"
#define ATC_GSM_SMS_4681_112_2_17_22_17_15_624 "ATC: Atc_UpdateSmsMemSts is called"
#define ATC_GSM_SMS_4696_112_2_17_22_17_15_625 "Get Me Storage inforamtion failure!"
#define ATC_GSM_SMS_4713_112_2_17_22_17_15_626 "Get SIM %d Storage inforamtion failure!"
#define ATC_GSM_SS_230_112_2_17_22_17_17_627 "ATC_EnableVoiceCodec: is_enable = %d"
#define ATC_GSM_SS_320_112_2_17_22_17_17_628 "ATC:CCFC reg fail1"
#define ATC_GSM_SS_327_112_2_17_22_17_17_629 "ATC command's parameter type wrong"
#define ATC_GSM_SS_343_112_2_17_22_17_17_630 "ATC:CCFC reg fail2"
#define ATC_GSM_SS_360_112_2_17_22_17_17_631 "ATC:CCFC reg fail3"
#define ATC_GSM_SS_727_112_2_17_22_17_18_632 "ATC: user set the AOC mode"
#define ATC_GSM_SS_1103_112_2_17_22_17_18_633 "ATC: OFFSET %d"
#define ATC_GSM_SS_1140_112_2_17_22_17_18_634 "ATC:USSD,PARAM1_FLAG = 0"
#define ATC_GSM_SS_1234_112_2_17_22_17_19_635 "ATC:MNSS_UserOriginateUSSDEx"
#define ATC_GSM_SS_1278_112_2_17_22_17_19_636 "ATC: unknown  cscs %s"
#define ATC_GSM_SS_1291_112_2_17_22_17_19_637 "ATC: ATC_ConvertAsciiToUcs2Hex return false"
#define ATC_GSM_SS_1386_112_2_17_22_17_19_638 "ATC: ussd ind ussd len is %d, str is %s"
#define ATC_GSM_SS_1433_112_2_17_22_17_19_639 "ATC: ussd_str is %s"
#define ATC_GSM_SS_1445_112_2_17_22_17_19_640 "ATC: ussd_str is pnull"
#define ATC_GSM_SS_1466_112_2_17_22_17_19_641 "ATC: ussd notify ussd len is %d, str is %s"
#define ATC_GSM_SS_1515_112_2_17_22_17_19_642 "ATC: ussd_str is %s"
#define ATC_GSM_SS_1527_112_2_17_22_17_19_643 "ATC: ussd_str is pnull"
#define ATC_GSM_SS_1550_112_2_17_22_17_19_644 "ATC: ussd service cnf ussd len is %d, str is %s"
#define ATC_GSM_SS_1621_112_2_17_22_17_19_645 "ATC: ussd_str is %s"
#define ATC_GSM_SS_1635_112_2_17_22_17_20_646 "ATC: ussd_str is pnull"
#define ATC_GSM_SS_2111_112_2_17_22_17_20_647 "ATC:ATC_GetParams=FALSE"
#define ATC_GSM_SS_2332_112_2_17_22_17_21_648 "ATC: CPIN pin1 enable flag: %d"
#define ATC_GSM_SS_2452_112_2_17_22_17_21_649 "ATC: cpwd parameter is not right"
#define ATC_GSM_SS_2519_112_2_17_22_17_21_650 "ATC: cpwd sim is not ready"
#define ATC_GSM_SS_2535_112_2_17_22_17_21_651 "ATC: cpwd sim is not ready"
#define ATC_GSM_SS_2563_112_2_17_22_17_21_652 "ATC:AT_PWD_AB"
#define ATC_GSM_SS_2610_112_2_17_22_17_21_653 "ATC:ATC_ProcessGetPswInd"
#define ATC_GSM_SS_2615_112_2_17_22_17_21_654 "ATC:AT_CMD_CPWD"
#define ATC_GSM_SS_2619_112_2_17_22_17_21_655 "ATC:s_old_password"
#define ATC_GSM_SS_2625_112_2_17_22_17_21_656 "ATC:s_new_password"
#define ATC_GSM_SS_2630_112_2_17_22_17_22_657 "ATC:s_new_password again"
#define ATC_GSM_SS_2635_112_2_17_22_17_22_658 "ATC: received error singal in get password indicate!!!"
#define ATC_GSM_SS_2723_112_2_17_22_17_22_659 "enter into CheckPresentHeldCall"
#define ATC_GSM_SS_2731_112_2_17_22_17_22_660 "found a held call %d"
#define ATC_GSM_SS_2752_112_2_17_22_17_22_661 "enter into RelPresentHeldCalls"
#define ATC_GSM_SS_2761_112_2_17_22_17_22_662 "held_id: %d"
#define ATC_GSM_SS_2795_112_2_17_22_17_22_663 "enter into CheckPresentActiveCall"
#define ATC_GSM_SS_2803_112_2_17_22_17_22_664 "found an active call %d"
#define ATC_GSM_SS_2823_112_2_17_22_17_22_665 "enter into RelPresentActiveCalls"
#define ATC_GSM_SS_2832_112_2_17_22_17_22_666 "call %d released"
#define ATC_GSM_SS_2856_112_2_17_22_17_22_667 "enter into CheckPresentWaitCall"
#define ATC_GSM_SS_2865_112_2_17_22_17_22_668 "found a waiting call %d"
#define ATC_GSM_SS_2884_112_2_17_22_17_22_669 "enter into SetUserBusyForWaitCall"
#define ATC_GSM_SS_2888_112_2_17_22_17_22_670 "waiting call %d set UDUB"
#define ATC_GSM_SS_2919_112_2_17_22_17_22_671 "ATC:ATC_ProcessSSCnf result=%d,list_num=%d,ss_status_present=%d,ss_status=%d"
#define ATC_GSM_SS_2994_112_2_17_22_17_22_672 "ATC:ATC_CheckInterrogateCnf result=%d,list_num=%d,ss_status_present=%d,ss_status=%d,service_type=%d,service_code=%d"
#define ATC_GSM_SS_3043_112_2_17_22_17_22_673 "enter into HandleChldUdub"
#define ATC_GSM_SS_3079_112_2_17_22_17_22_674 "enter into HandleChldRelAcceptOthers"
#define ATC_GSM_SS_3148_112_2_17_22_17_23_675 "enter into HandleChldHoldAcceptOthers"
#define ATC_GSM_SS_3213_112_2_17_22_17_23_676 "enter into HandleChldMultCall"
#define ATC_GSM_SS_3219_112_2_17_22_17_23_677 "held_id = %d, active_id = %d"
#define ATC_GSM_SS_3573_112_2_17_22_17_23_678 "ATC: MNSS_UserOriginateUSSD result = %d"
#define ATC_GSM_SS_3589_112_2_17_22_17_23_679 "ATC: MNSS_ResponseUSSD result = %d"
#define ATC_GSM_SS_3694_112_2_17_22_17_24_680 "ATC: unknow ss code = %d"
#define ATC_GSM_SS_3732_112_2_17_22_17_24_681 "ATC: mn register ss error code = %d"
#define ATC_GSM_SS_3748_112_2_17_22_17_24_682 "ATC: mn erase ss info error code = %d"
#define ATC_GSM_SS_3764_112_2_17_22_17_24_683 "ATC: mn active ss error code %d"
#define ATC_GSM_SS_3780_112_2_17_22_17_24_684 "ATC: mn deactive ss error code = %d"
#define ATC_GSM_SS_3797_112_2_17_22_17_24_685 "ATC: mn interrogate service error code = %d"
#define ATC_GSM_SS_3814_112_2_17_22_17_24_686 "ATC: mn register ss password error code = %d"
#define ATC_GSM_SS_3826_112_2_17_22_17_24_687 "ATC: SendReqToSs in default!"
#define ATC_GSM_SS_4133_112_2_17_22_17_25_688 "ATC: PARA2.str_ptr->str_len is %d, str is %s"
#define ATC_GSM_SS_4139_112_2_17_22_17_25_689 "ATC: ptr_para2_str str_len is %d, str is %s"
#define ATC_GSM_SS_4143_112_2_17_22_17_25_690 "ATC: PARA2.str_ptr->str_len is %d, str is %s"
#define ATC_GSM_SS_4149_112_2_17_22_17_25_691 "ATC: ptr_para2_str str_len is %d, str is %s"
#define ATC_GSM_STK_268_112_2_17_22_17_26_692 "ATC: error length in simat."
#define ATC_GSM_STK_338_112_2_17_22_17_26_693 "ATC: error data length in simat"
#define ATC_GSM_STK_369_112_2_17_22_17_26_694 "ATC:ATC_ProcessGetInkey,%d,%d"
#define ATC_GSM_STK_431_112_2_17_22_17_26_695 "ATC: error length in simat."
#define ATC_GSM_STK_656_112_2_17_22_17_27_696 "ATC: ATC_ProcessSendSMS sc_addr.length = %d"
#define ATC_GSM_STK_726_112_2_17_22_17_27_697 "ATC: send sms pdu err code = %d."
#define ATC_GSM_STK_735_112_2_17_22_17_27_698 "ATC: send sms pdu err code = %d."
#define ATC_GSM_STK_898_112_2_17_22_17_27_699 "ATC: user originate ussd fail, err code = %d."
#define ATC_GSM_STK_928_112_2_17_22_17_27_700 "ATC:enter ATC_STKProcessSendSmsCnf"
#define ATC_GSM_STK_941_112_2_17_22_17_27_701 "ATC: Receive the error status: %d"
#define ATC_GSM_STK_954_112_2_17_22_17_27_702 "ATC:ATC_STKProcessSendSmsCnf,MN_SMS_OPERATE_SUCCESS"
#define ATC_GSM_STK_962_112_2_17_22_17_27_703 "ATC:ATC_STKProcessSendSmsCnf,MN_SMS_FAIL"
#define ATC_GSM_STK_1108_112_2_17_22_17_28_704 "ATC:ATC_CheckAndHandleGetInkey,SIMAT_DIGIT_ONLY,%d"
#define ATC_GSM_STK_1234_112_2_17_22_17_28_705 "ATC:ATC_CheckAndHandleGetInkey==TRUE"
#define ATC_GSM_STK_1297_112_2_17_22_17_28_706 "ATC:error in length"
#define ATC_GSM_STK_1305_112_2_17_22_17_28_707 "ATC: getinput type = %d, str_len = %d, min_len = %d max_len = %d"
#define ATC_GSM_STK_1324_112_2_17_22_17_28_708 "ATC:error in length"
#define ATC_GSM_STK_1345_112_2_17_22_17_28_709 "ATC:error in length"
#define ATC_GSM_STK_1362_112_2_17_22_17_28_710 "ATC:error in length"
#define ATC_GSM_STK_1450_112_2_17_22_17_28_711 "ATC:ATC_CheckAndHandleGetInkey==TRUE"
#define ATC_GSM_STK_1935_112_2_17_22_17_29_712 "ATC: stk deactivate fail."
#define ATC_GSM_STK_1992_112_2_17_22_17_30_713 "ATC:ATC_CheckAndHandleConfig is false"
#define ATC_GSM_STK_2056_112_2_17_22_17_30_714 "ATC:PARA1.num != s_stk_signal[dual_sys].cur_status"
#define ATC_GSM_STK_2446_112_2_17_22_17_30_715 "ATC:s_cmd_stag=%d"
#define ATC_GSM_STK_2488_112_2_17_22_17_31_716 "ATC:ATC_MainMenuCnf=FALSE"
#define ATC_GSM_STK_2741_112_2_17_22_17_31_717 "ATC: get imei failed."
#define ATC_GSM_STK_3082_112_2_17_22_17_32_718 "ATC:EV_MN_APP_SIMAT_LANG_NOTIFY_IND_F"
#define ATC_GSM_STK_3087_112_2_17_22_17_32_719 "ATC:SIMAT_EVENT_REFRESH"
#define ATC_GSM_STK_3092_112_2_17_22_17_32_720 "ATC:SIMAT_EVENT_CC_RSP"
#define ATC_GSM_STK_3098_112_2_17_22_17_32_721 "ATC:SIMAT_EVENT_SS_CON_RSP"
#define ATC_GSM_STK_3104_112_2_17_22_17_32_722 "ATC:SIMAT_EVENT_USSD_CON_RSP"
#define ATC_GSM_STK_3110_112_2_17_22_17_32_723 "ATC:SIMAT_EVENT_SMS_CON_RSP"
#define ATC_GSM_STK_3116_112_2_17_22_17_32_724 "ATC:SIMAT_EVENT_REFRESH"
#define ATC_GSM_STK_3122_112_2_17_22_17_32_725 "ATC:SIMAT_EVENT_SMS_PP_RSP"
#define ATC_GSM_STK_3222_112_2_17_22_17_32_726 "ATC: HandleStkPduModeSms"
#define ATC_GSM_STK_3231_112_2_17_22_17_32_727 "ATC: HandleStkPduModeSms sc_addr.length = %d"
#define ATC_GSM_STK_3238_112_2_17_22_17_32_728 "ATC:error ConvertHexToBin"
#define ATC_GSM_STK_3252_112_2_17_22_17_32_729 "ATC: Send Sms PDU failed!"
#define ATC_GSM_STK_3278_112_2_17_22_17_32_730 "ATC: ATC_ProcessSioStkSmsData"
#define ATC_GSM_STK_3342_112_2_17_22_17_32_731 "ATC: ATC_ProcessSIMRefreshCnf"
#define ATC_GSM_STK_3376_112_2_17_22_17_32_732 "OnRefreshCnfSimFileChange:sim_file = %d"
#define ATC_GSM_STK_3421_112_2_17_22_17_33_733 "ATC: ATC_ProcessPSSTKE"
#define ATC_GSM_STK_3466_112_2_17_22_17_33_734 "ATC: ATC_ProcessPSSTKR"
#define ATC_GSM_STK_3508_112_2_17_22_17_33_735 "ATC:ATC_ProcessPSSTKI"
#define ATC_GSM_STK_3521_112_2_17_22_17_33_736 "ATC: save stk %d menu cnt = %d"
#define ATC_GSM_STK_3525_112_2_17_22_17_33_737 "ATC: send stk menu cnt = %d, len = %d"
#define ATC_GSM_STK_3584_112_2_17_22_17_33_738 "ATC: stk %d setup menu cnt = %d"
#define ATC_GSM_STK_3622_112_2_17_22_17_33_739 "ATC: ATC_ProcessPSSTKSMS"
#define ATC_GSM_STK_3677_112_2_17_22_17_33_740 "ATC: convert tele string failed!\r\n  PARA4.str_ptr->str_ptr:%s "
#define ATC_GSM_STK_3733_112_2_17_22_17_33_741 "ATC: ATC_ProcessSTKREFRESH"
#define ATC_GSM_STK_3786_112_2_17_22_17_33_742 "ATC: convert file list string failed!\r\n  PARA3.str_ptr->str_ptr:%s "
#define ATC_GSM_STK_3793_112_2_17_22_17_33_743 "ATC: file list decode failed!\r\n  data_len: %d, data_ptr: %p "
#define ATC_MALLOC_263_112_2_17_22_17_44_744 "Add heap success, heap size = %d.\n"
#define ATC_PHONEBOOK_409_112_2_17_22_17_45_745 "ATC:cpbr 80 type, %x, %x, %x"
#define ATC_PHONEBOOK_415_112_2_17_22_17_45_746 "ATC: CONVERT TO IRA FAIL"
#define ATC_PHONEBOOK_423_112_2_17_22_17_45_747 "ATC:cpbr 81 type, %x, %x, %x"
#define ATC_PHONEBOOK_438_112_2_17_22_17_45_748 "ATC:cpbr 82 type, %x, %x, %x"
#define ATC_PHONEBOOK_454_112_2_17_22_17_45_749 "ATC: cpbr str type, %x, %x, %x"
#define ATC_PHONEBOOK_463_112_2_17_22_17_45_750 "ATC: tel %x, %x, %x, %x"
#define ATC_PHONEBOOK_470_112_2_17_22_17_45_751 "ATC: unknown  cscs %s, use IRA"
#define ATC_PHONEBOOK_538_112_2_17_22_17_45_752 "ATC_CMD_PARAM_TYPE_STRING != PARAM1_TYPE"
#define ATC_PHONEBOOK_605_112_2_17_22_17_45_753 "__ 80 %x, %x, %x"
#define ATC_PHONEBOOK_617_112_2_17_22_17_45_754 "__ 81 %x, %x, %x"
#define ATC_PHONEBOOK_632_112_2_17_22_17_45_755 "__ 82 %x, %x, %x"
#define ATC_PHONEBOOK_656_112_2_17_22_17_45_756 "ATC: tel %x, %x, %x, %x"
#define ATC_PHONEBOOK_663_112_2_17_22_17_45_757 "ATC: unknown  cscs %s, use IRA"
#define ATC_PHONEBOOK_875_112_2_17_22_17_46_758 "ATC:cpbw,tel_alpha=,%x,%x,%x,str_len,%x"
#define ATC_PHONEBOOK_883_112_2_17_22_17_46_759 "ATC: %x %x %x %x %x len %d"
#define ATC_PHONEBOOK_905_112_2_17_22_17_46_760 "ATC:cpbw,PARA4.str_ptr=%x,%x,%x"
#define ATC_PHONEBOOK_1383_112_2_17_22_17_47_761 "ATC: ATC_CMD_TYPE_TEST"
#define ATC_PHONEBOOK_1458_112_2_17_22_17_47_762 "ATC: tel %x, %x, %x, %x"
#define ATC_PHONEBOOK_1465_112_2_17_22_17_47_763 "ATC: unknown  cscs %s, use IRA"
#define ATC_PHONEBOOK_1477_112_2_17_22_17_47_764 "ATC: PHONEBOOK_ReadPhonebookEntry read error"
#define ATC_PHONEBOOK_1486_112_2_17_22_17_47_765 "ATC: ATC_CMD_TYPE_READ"
#define ATC_PHONEBOOK_1586_112_2_17_22_17_47_766 "ATC:AT+CSVM,error in length of input"
#define ATC_PHONEBOOK_1605_112_2_17_22_17_47_767 "ATC:ERR_OPERATION_NOT_SUPPORTED"
#define ATC_PHONEBOOK_1618_112_2_17_22_17_47_768 "ATC:PHONEBOOK_UpdateVoiceMailAddr failed"
#define ATC_PHONEBOOK_1625_112_2_17_22_17_47_769 "ATC:PHONEBOOK_EnableVoiceMailAddr failed"
#define ATC_PHONEBOOK_1676_112_2_17_22_17_47_770 "ATC: ATC_CMD_TYPE_READ"
#define ATC_PHONEBOOK_1685_112_2_17_22_17_47_771 "ATC: ERR_INVALID_INDEX"
#define ATC_PLUS_GPRS_207_112_2_17_22_17_48_772 "ATGPRS:ATC_ProcessCGDCONT\n"
#define ATC_PLUS_GPRS_518_112_2_17_22_17_49_773 "ATC: pco user or passwd too long."
#define ATC_PLUS_GPRS_531_112_2_17_22_17_49_774 "ATC: pco user or passwd too long."
#define ATC_PLUS_GPRS_539_112_2_17_22_17_49_775 "ATC: set pco error, result = %d"
#define ATC_PLUS_GPRS_545_112_2_17_22_17_49_776 "ATC: set pco error parameter"
#define ATC_PLUS_GPRS_642_112_2_17_22_17_49_777 "ATGPRS:ATC_ProcessCGQREQ\n"
#define ATC_PLUS_GPRS_710_112_2_17_22_17_49_778 "ATGPRS:ATC_ProcessCGQREQ param error!\n"
#define ATC_PLUS_GPRS_812_112_2_17_22_17_49_779 "ATGPRS:ATC_ProcessCGQMIN\n"
#define ATC_PLUS_GPRS_876_112_2_17_22_17_49_780 "ATGPRS:ATC_ProcessCGQREQ param error!\n"
#define ATC_PLUS_GPRS_973_112_2_17_22_17_50_781 "ATGPRS:ATC_ProcessCGATT\n"
#define ATC_PLUS_GPRS_1073_112_2_17_22_17_50_782 "ATGPRS:ATC_ProcessCGACT\n"
#define ATC_PLUS_GPRS_1285_112_2_17_22_17_50_783 "ATGPRS:ATC_ProcessCGDATA\n"
#define ATC_PLUS_GPRS_1316_112_2_17_22_17_50_784 "ATGPRS:layer 2 Protocol default?"
#define ATC_PLUS_GPRS_1359_112_2_17_22_17_50_785 "ATC: AT+CGDATA RequestPdpId pdp_id:%d"
#define ATC_PLUS_GPRS_1436_112_2_17_22_17_51_786 "ATGPRS: ATC_ProcessCGSMS\n"
#define ATC_PLUS_GPRS_1486_112_2_17_22_17_51_787 "ATGPRS: CREG value:%d\n"
#define ATC_PLUS_GPRS_1495_112_2_17_22_17_51_788 "ATGPRS: CREG set value:%d\n"
#define ATC_PLUS_GPRS_1686_112_2_17_22_17_51_789 "ATGPRS: ATC_ProcessCGEREP\n"
#define ATC_PLUS_GPRS_1764_112_2_17_22_17_51_790 "ATGPRS: ATC_ProcessSelectServiceType\n"
#define ATC_PLUS_GPRS_1837_112_2_17_22_17_51_791 "ATGPRS: ATC_ProcessSATT\n"
#define ATC_PLUS_GPRS_1924_112_2_17_22_17_52_792 "ATGPRS: ATC_ProcessSATT\n"
#define ATC_PLUS_GPRS_2025_112_2_17_22_17_52_793 "ATC: ATC_ProcessSGPRSDATA, ERROR: MNGPRS_ReadPdpContextState failed."
#define ATC_PLUS_GPRS_2048_112_2_17_22_17_52_794 "ATC: ATC_ProcessSGPRSDATA, ERROR: invalid active pdp count=%d."
#define ATC_PLUS_GPRS_2063_112_2_17_22_17_52_795 "ATC: ATC_ProcessSGPRSDATA, too long length!"
#define ATC_PLUS_GPRS_2070_112_2_17_22_17_52_796 "ATGPRS: The GPRS send data len beyond MAX_AT_GPRSDATA_SEND_LEN!\n"
#define ATC_PLUS_GPRS_2089_112_2_17_22_17_52_797 "ATC: ATC_ProcessSGPRSDATA, send data len=%d,nsapi=%d"
#define ATC_PLUS_GPRS_2094_112_2_17_22_17_52_798 "ATC: ATC_ProcessSGPRSDATA, MNGPRS_SendDataReq failed!"
#define ATC_PLUS_GPRS_2111_112_2_17_22_17_52_799 "ATC: ATC_ProcessSGPRSDATA, send data len=%d,nsapi=%d"
#define ATC_PLUS_GPRS_2116_112_2_17_22_17_52_800 "ATC: ATC_ProcessSGPRSDATA, MNGPRS_SendDataReq failed!"
#define ATC_PLUS_GPRS_2152_112_2_17_22_17_52_801 "ATGPRS:ATC_ProcessCGEQREQ\n"
#define ATC_PLUS_GPRS_2321_112_2_17_22_17_52_802 "ATGPRS:ATC_ProcessCGEQREQ param error!\n"
#define ATC_PLUS_GPRS_2628_112_2_17_22_17_53_803 "ATGPRS:ATC_ProcessCGEQMIN\n"
#define ATC_PLUS_GPRS_2686_112_2_17_22_17_53_804 "ATC_ProcessDeactPdpContextInd,is deactivate AT pid,pid:%d"
#define ATC_PLUS_GPRS_2751_112_2_17_22_17_53_805 "ATC: Switch to DATA MODE for PCLink!"
#define ATC_PLUS_GPRS_2755_112_2_17_22_17_53_806 "ATC: Activate result is NOT Success in PCLink Mode!"
#define ATC_PLUS_GPRS_2768_112_2_17_22_17_53_807 "ATC:APP_MN_ACTIVATE_PDP_CONTEXT_CNF pid:%d"
#define ATC_PLUS_GPRS_2797_112_2_17_22_17_53_808 "ATC_ProcessActPdpContextCnf,pid is not AT activated,pid:%d"
#define ATC_PLUS_GPRS_3002_112_2_17_22_17_54_809 "ATGPRS:ATC_ProcessCGPADDR\n"
#define ATC_PLUS_GPRS_3038_112_2_17_22_17_54_810 "ATGPRS:ATC_ProcessCGPADDR\n"
#define ATC_PLUS_GPRS_3180_112_2_17_22_17_54_811 "ATGPRS:ATC_ProcessCGANS\n"
#define ATC_PLUS_GPRS_3209_112_2_17_22_17_54_812 "layer 2 Protocol:%s"
#define ATC_PLUS_GPRS_3222_112_2_17_22_17_54_813 "default Layer 2 Protocol"
#define ATC_PLUS_GPRS_3228_112_2_17_22_17_54_814 "para 3 pdp id:%d"
#define ATC_PLUS_GPRS_3288_112_2_17_22_17_54_815 "input class name class%s"
#define MNCALL_AOC_60_112_2_17_22_18_21_816 "MNCALL: AOC the fixed charge is %d"
#define MNCALL_AOC_98_112_2_17_22_18_21_817 "MNCALL: AOC the interval charge is %d"
#define MNCALL_AOC_224_112_2_17_22_18_21_818 "MNCALL: AOC the interval duration is %d"
#define MNCALL_API_601_112_2_17_22_18_23_819 "MNCALL: set bearer capability type = %d"
#define MNCALL_DTMF_75_112_2_17_22_18_26_820 "MNCALL: DTMF_CheckBufferIsEmpty result = %d"
#define MNCALL_DTMF_93_112_2_17_22_18_26_821 "MNCALL: DTMF the buffer is full"
#define MNCALL_DTMF_115_112_2_17_22_18_26_822 "MNCALL: DTMF the insert DTMF item is %d"
#define MNCALL_DTMF_132_112_2_17_22_18_26_823 "MNCALL: DTMF the insert DTMF stop entry"
#define MNCALL_DTMF_148_112_2_17_22_18_26_824 "MNCALL: DTMF the remove DTMF item is %d"
#define MNCALL_DTMF_190_112_2_17_22_18_26_825 "MNCALL: DTMF set status is %d"
#define MNCALL_DTMF_220_112_2_17_22_18_26_826 "MNCALL: DTMF set STOP status is %d"
#define MNCALL_DTMF_257_112_2_17_22_18_26_827 "MNCALL: DTMF_ReadStatusBits %d"
#define MNCALL_DTMF_289_112_2_17_22_18_26_828 "MNCALL: set the DTMF type bit error!\n"
#define MNCALL_MODULE_1693_112_2_17_22_18_33_829 "MNCALL: the call id: %d"
#define MNCALL_MODULE_1943_112_2_17_22_18_33_830 "MNCALL: Receive MN up signal call_index invalid"
#define MNCALL_MODULE_2346_112_2_17_22_18_34_831 "MNCALL::not retry."
#define MNCALL_MODULE_2350_112_2_17_22_18_34_832 "MNCALL::start mo retry timer."
#define MNCALL_MODULE_2474_112_2_17_22_18_35_833 "MNCALL:call index = %d,is_mt = %d."
#define MNCALL_MODULE_2479_112_2_17_22_18_35_834 "MNCALL: ti is %d."
#define MNCALL_MODULE_2533_112_2_17_22_18_35_835 "MNCALL:recv disc when mo call retry."
#define MNCALL_MODULE_2882_112_2_17_22_18_35_836 "MNCALL:stk_ti = %d."
#define MNCALL_MODULE_3113_112_2_17_22_18_36_837 "MNCALL:recv setup when mo call retry."
#define MNCALL_MODULE_3179_112_2_17_22_18_36_838 "MNCALL:call wait on."
#define MNCALL_MODULE_3859_112_2_17_22_18_37_839 "MN:ignore message too short\n"
#define MNCALL_MODULE_3863_112_2_17_22_18_37_840 "MN:unknown, unforeseen, erroneous data\n"
#define MNCALL_MODULE_4037_112_2_17_22_18_38_841 "MNCALL: The DTMF buffer is full"
#define MNCALL_MODULE_4067_112_2_17_22_18_38_842 "MNCALL: StopDTMFReq status:%d"
#define MNCALL_MODULE_4170_112_2_17_22_18_38_843 "MNCALL: StartDTMFCnf status:%d"
#define MNCALL_MODULE_4497_112_2_17_22_18_39_844 "MNCALL: ECT invoke condition is not satisfied!"
#define MNCALL_MODULE_5507_112_2_17_22_18_41_845 "MNCALL: recv undefine bc"
#define MNCALL_MODULE_5792_112_2_17_22_18_41_846 "MNCALL: recv unexpected result"
#define MNCALL_MODULE_5812_112_2_17_22_18_41_847 "MNCALL: the ccbs context is invalid!"
#define MNCALL_MODULE_5818_112_2_17_22_18_41_848 "MNCALL: call_index is invalid!"
#define MNCALL_MODULE_5855_112_2_17_22_18_41_849 "MNCALL: ect context is invalid!"
#define MNCALL_MODULE_5861_112_2_17_22_18_41_850 "MNCALL: call_index is invalid!"
#define MNCALL_MODULE_5897_112_2_17_22_18_41_851 "MNCALL: the MPTY context is invalid!"
#define MNCALL_MODULE_5903_112_2_17_22_18_41_852 "MNCALL: call_index is invalid!"
#define MNCALL_MODULE_6084_112_2_17_22_18_42_853 "MNCALL: recv unexpected error/reject"
#define MNCALL_MODULE_6109_112_2_17_22_18_42_854 "MNCALL: the ect context is invalid!"
#define MNCALL_MODULE_6115_112_2_17_22_18_42_855 "MNCALL: call_index is invalid!"
#define MNCALL_MODULE_6156_112_2_17_22_18_42_856 "MNCALL: the ect context is invalid!"
#define MNCALL_MODULE_6162_112_2_17_22_18_42_857 "MNCALL: call_index is invalid!"
#define MNCALL_MODULE_6195_112_2_17_22_18_42_858 "MNCALL: the MPTY context is invalid!"
#define MNCALL_MODULE_6201_112_2_17_22_18_42_859 "MNCALL: call_index is invalid!"
#define MNCALL_MODULE_6449_112_2_17_22_18_43_860 "MNCALL: MTPY context error"
#define MNCALL_MODULE_7623_112_2_17_22_18_45_861 "MNCALL::error! the nv not to support FS."
#define MNCALL_MODULE_7691_112_2_17_22_18_45_862 "MNCALL:: start retry timer."
#define MNCALL_MODULE_7750_112_2_17_22_18_45_863 "MNCALL:: begin mo call retry."
#define APP_MNNV_API_146_112_2_17_22_18_47_864 "MNNV: imeisvn %d"
#define APP_MNSIM_API_311_112_2_17_22_18_48_865 "MN: not equal ccid len"
#define APP_MNSIM_API_749_112_2_17_22_18_49_866 "MNPHONE: unknow line 1 call forward flag = 0x%x"
#define APP_MNSIM_API_764_112_2_17_22_18_49_867 "MNPHONE: unknow line 2 call forward flag = 0x%x"
#define APP_MNSIM_API_1650_112_2_17_22_18_51_868 "MNSIM_GetSimFileRecordNumEx file_name = %d"
#define APP_MNSIM_API_1691_112_2_17_22_18_51_869 "MNSIM_GetSimFileRecordEntryEx file_name = %d"
#define APP_MNSIM_API_1748_112_2_17_22_18_51_870 "MNSIM_GetSimBinaryFileEx file_name = %d"
#define APP_PHONEBOOK_API_1730_112_2_17_22_18_59_871 "MN: GetSimEntryInList pos = %d"
#define APP_PHONEBOOK_API_1844_112_2_17_22_18_59_872 "EncodeUcs2AlphaStr result = %d, alpha_type = %d"
#define APP_PHONEBOOK_API_2048_112_2_17_22_18_59_873 "MN: PHONEBOOK_InsertSimEntry num_len = %d"
#define APP_PHONEBOOK_API_2323_112_2_17_22_19_0_874 "delete sim book id = %d, use_entry_num = %d"
#define APP_PHONEBOOK_API_2474_112_2_17_22_19_0_875 "ConvertToPhoneEntry number_len = %d"
#define APP_PHONEBOOK_API_2531_112_2_17_22_19_0_876 "L4: al %x, %x, %x, %x, entry %x, %x, %x, %x "
#define APP_PHONEBOOK_API_3519_112_2_17_22_19_2_877 "MNPHONE: MAIN_GetFreeExtInfo storage = %d"
#define MNCLASSMARK_API_41_112_2_17_22_19_3_878 "mnclassmark: model type %x"
#define MNNV_API_100_112_2_17_22_19_4_879 "MnPhone: the value of service type from NV is error!"
#define MNNV_API_120_112_2_17_22_19_4_880 "MnPhone: the value of preference mode from NV is error!"
#define MNNV_API_142_112_2_17_22_19_4_881 "MnPhone: the value of roam mode from NV is error!"
#define MNNV_API_158_112_2_17_22_19_4_882 "MnPhone: user band from NV is error!"
#define MNNV_API_164_112_2_17_22_19_4_883 "MnPhone: Fail band reading, set Dual Band"
#define MNNV_API_173_112_2_17_22_19_4_884 "mnnv:RX_EX support is %d"
#define MNNV_API_174_112_2_17_22_19_4_885 "mnnv:RX_EX egsm thr is %d, val is %d"
#define MNNV_API_175_112_2_17_22_19_4_886 "mnnv:RX_EX dcs thr is %d, val is %d"
#define MNNV_API_176_112_2_17_22_19_4_887 "mnnv:RX_EX pcs thr is %d, val is %d"
#define MNNV_API_177_112_2_17_22_19_4_888 "mnnv:RX_EX gsm850 thr is %d, val is %d"
#define MNNV_API_329_112_2_17_22_19_5_889 "MN_NV: Assert Failure, DMNV_GetMSBand: user_band %d error."
#define MNNV_API_801_112_2_17_22_19_6_890 "MNNV: read nv error result %d\n"
#define MNNV_API_1067_112_2_17_22_19_6_891 "DMNV:DMNV_GetBaList=%x,%x"
#define MNNV_API_1089_112_2_17_22_19_6_892 "DMNV:DMNV_SetBaList=%x,%x"
#define MNSIM_API_277_112_2_17_22_19_8_893 "MN: HPLMN match, %d"
#define MNSIM_API_1267_112_2_17_22_19_10_894 "MNSIM: get EHPLMN, length is %d, item0 is (%d,%d), item1 is (%d,%d)"
#define MNSIM_API_1340_112_2_17_22_19_10_895 "DMSIM_IsSmsDownloadSupport result = %d"
#define MNSIM_API_1366_112_2_17_22_19_10_896 "DMSIM_IsCBDownloadSupport result = %d"
#define MNSIM_EVENT_370_112_2_17_22_19_11_897 "MNSIM_SetEventRecordReadCnf file_name = %d"
#define MNSIM_EVENT_412_112_2_17_22_19_11_898 "MNSIM_SetEventRecordReadCnf file_name = %d"
#define MNSIM_MODULE_770_112_2_17_22_19_13_899 "MNSIM: sim smsp is null."
#define MNSIM_MODULE_777_112_2_17_22_19_13_900 "MNSIM: sim smsp is null."
#define MNSIM_MODULE_789_112_2_17_22_19_13_901 "MNSIM:img service is support,img record num is %d."
#define MNSIM_MODULE_795_112_2_17_22_19_13_902 "MNSIM:img service not support,or img record num is %d."
#define MNSIM_MODULE_801_112_2_17_22_19_13_903 "MNSIM:get img record info failed."
#define MNSIM_MODULE_835_112_2_17_22_19_14_904 "MN%d: get the EXT2 max have %d records"
#define MNSIM_MODULE_929_112_2_17_22_19_14_905 "MN%d:SIM LDN ext-- %d %d"
#define MNSIM_MODULE_960_112_2_17_22_19_14_906 "MN%d:SIM MSISDN ext-- %d %d"
#define MNSIM_MODULE_1007_112_2_17_22_19_14_907 "MN%d:SIM ADN ext-- %d %d"
#define MNSIM_MODULE_1036_112_2_17_22_19_14_908 "MN%d:SIM update empty ext2 num %d"
#define MNSIM_MODULE_1041_112_2_17_22_19_14_909 "MN%d:SIM FDN ext-- %d %d"
#define MNSIM_MODULE_1199_112_2_17_22_19_14_910 "MN%d:SIM MSISDN EXT overflow %d %d %d"
#define MNSIM_MODULE_1231_112_2_17_22_19_14_911 "MN%d:SIM MSISDN EXT overflow %d %d %d"
#define MNSIM_MODULE_1238_112_2_17_22_19_14_912 "MNSIM: HandleSimUpdateDnCnf SIM_DN_T_MSISDN PHONEBOOK_UpdateSimEntry FAIL"
#define MNSIM_MODULE_1245_112_2_17_22_19_14_913 "MNSIM: HandleSimUpdateDnCnf SIM_DN_T_MSISDN entry_id = %d, dn_id = %d"
#define MNSIM_MODULE_1279_112_2_17_22_19_14_914 "MN%d:SIM ADN EXT overflow %d %d %d"
#define MNSIM_MODULE_1285_112_2_17_22_19_14_915 "MNSIM: HandleSimUpdateDnCnf PHONEBOOK_UpdateSimEntry FAIL"
#define MNSIM_MODULE_1292_112_2_17_22_19_14_916 "MNSIM: HandleSimUpdateDnCnf entry_id = %d, dn_id = %d"
#define MNSIM_MODULE_1322_112_2_17_22_19_15_917 "MN%d:SIM update empty ext2 num %d"
#define MNSIM_MODULE_1327_112_2_17_22_19_15_918 "MN%d:SIM FDN EXT overflow %d %d %d"
#define MNSIM_MODULE_1332_112_2_17_22_19_15_919 "MNSIM: HandleSimUpdateDnCnf SIM_DN_T_FDN PHONEBOOK_UpdateSimEntry FAIL"
#define MNSIM_MODULE_1339_112_2_17_22_19_15_920 "MNSIM: HandleSimUpdateDnCnf SIM_DN_T_FDN entry_id = %d, dn_id = %d"
#define MNSIM_MODULE_1350_112_2_17_22_19_15_921 "MNSIM:update dn result %d, type %d, id %d"
#define MNSIM_MODULE_1389_112_2_17_22_19_15_922 "MNSIM: HandleSimGetFreeExtInfoCnf not right dialling num type\n"
#define MNSIM_MODULE_1508_112_2_17_22_19_15_923 "MN%d:SIM erase ADN id has EXT,but 0"
#define MNSIM_MODULE_1551_112_2_17_22_19_15_924 "MN%d:SIM erase ADN id has EXT,but 0"
#define MNSIM_MODULE_1583_112_2_17_22_19_15_925 "MN%d:SIM erase ADN id has EXT,but 0"
#define MNSIM_MODULE_1588_112_2_17_22_19_15_926 "MNSIM: HandleSimEraseDnCnf SIM_DN_T_MSISDN PHONEBOOK_DeleteSimEntry FAIL"
#define MNSIM_MODULE_1595_112_2_17_22_19_15_927 "MNSIM: HandleSimEraseDnCnf SIM_DN_T_MSISDN entry_id = %d, dn_id = %d"
#define MNSIM_MODULE_1630_112_2_17_22_19_15_928 "MN%d:SIM erase ADN id has EXT,but 0"
#define MNSIM_MODULE_1635_112_2_17_22_19_15_929 "MNSIM: HandleSimEraseDnCnf SIM_DN_T_ADN PHONEBOOK_DeleteSimEntry FAIL"
#define MNSIM_MODULE_1642_112_2_17_22_19_15_930 "MNSIM: HandleSimEraseDnCnf SIM_DN_T_ADN entry_id = %d, dn_id = %d"
#define MNSIM_MODULE_1670_112_2_17_22_19_15_931 "MN%d:SIM update empty ext2 num %d"
#define MNSIM_MODULE_1675_112_2_17_22_19_15_932 "MNSIM: HandleSimEraseDnCnf SIM_DN_T_FDN PHONEBOOK_DeleteSimEntry FAIL"
#define MNSIM_MODULE_1682_112_2_17_22_19_15_933 "MNSIM: HandleSimEraseDnCnf SIM_DN_T_FDN entry_id = %d, dn_id = %d"
#define MNSIM_MODULE_1693_112_2_17_22_19_15_934 "MNSIM:erase dn result %d, type %d, id %d"
#define MNSIM_MODULE_1740_112_2_17_22_19_15_935 "MN%d:sim HandleFdnServiceCnf recover EXT2 num %d"
#define MNSIM_MODULE_2919_112_2_17_22_19_18_936 "mnsim_module: AllocateMem max_record_num = 0"
#define MNSIM_MODULE_3261_112_2_17_22_19_18_937 "MNSIM::refresh ADN should not go this branch! please check!"
#define MNSIM_MODULE_3262_112_2_17_22_19_18_938 "MNSIM::refresh LND should not go this branch! please check!"
#define MNSIM_MODULE_3263_112_2_17_22_19_18_939 "MNSIM::refresh MSISDN should not go this branch! please check!"
#define MNSIM_MODULE_3264_112_2_17_22_19_18_940 "MNSIM::refresh FDN should not go this branch! please check!"
#define MNSIM_MODULE_3265_112_2_17_22_19_18_941 "MNSIM::refresh SDN should not go this branch! please check!"
#define MNSIM_MODULE_3266_112_2_17_22_19_18_942 "MNSIM::refresh SMS should not go this branch! please check!"
#define MNSIM_MODULE_3267_112_2_17_22_19_18_943 "MNSIM::refresh SMSP should not go this branch! please check!"
#define MNSIM_MODULE_3268_112_2_17_22_19_18_944 "MNSIM::refresh SMSS should not go this branch! please check!"
#define MNSIM_MODULE_3270_112_2_17_22_19_18_945 "MNSIM::refresh file %d of card%d that not save in mn module."
#define MNSIM_MODULE_3334_112_2_17_22_19_18_946 "MNPHONE: the sim file %d is not cared by mn."
#define MNSIM_MODULE_3394_112_2_17_22_19_19_947 "MNPHONE: the sim file %d is not cared by mn."
#define MNSIM_MODULE_3509_112_2_17_22_19_19_948 "MNSIM: sim(%d) max_img_num = %d"
#define MNSIM_MODULE_3560_112_2_17_22_19_19_949 "MNSIM:width is %d,height is %d,cs is 0x%x,img_len is %d."
#define MNSIM_MODULE_3562_112_2_17_22_19_19_950 "MNSIM: img body:0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x."
#define MNSIM_MODULE_3655_112_2_17_22_19_19_951 "MN%d:SIM update empty ext1 num %d"
#define MNENG_MODULE_474_112_2_17_22_19_25_952 "Command type %d error!"
#define MNGPRS_API_636_112_2_17_22_19_27_953 "mn:MNGPRS_ReadPdpContextPco fail result = %d"
#define MNGPRS_API_732_112_2_17_22_19_27_954 "mn:MNGPRS_SetPdpContext fail id = %d"
#define MNGPRS_API_755_112_2_17_22_19_27_955 "MNGPRS: MAIN_SetPdpContextPco user_ptr = %ld, passwd_ptr = %ld"
#define MNGPRS_API_768_112_2_17_22_19_27_956 "mngprs:MAIN_SetPdpContextPco invalid auth_type"
#define MNGPRS_API_777_112_2_17_22_19_27_957 "mn:GPRS_HandleSetPdpContextPcoReq fail result = %d"
#define MNGPRS_API_836_112_2_17_22_19_28_958 "MNGPRS:MNGPRS_GetPclinkCfg retrun err"
#define MNGPRS_API_856_112_2_17_22_19_28_959 "mn:GPRS_SendDeactivePdpCnf fail result = %d"
#define MNGPRS_API_922_112_2_17_22_19_28_960 "MNGPRS:MNGPRS_GetPclinkCfg retrun err!"
#define MNGPRS_API_950_112_2_17_22_19_28_961 "mn:GPRS_SendPdpActiveCnf fail result = %d"
#define MNGPRS_API_986_112_2_17_22_19_28_962 "MNGPRS:sim card have been switched"
#define MNGPRS_API_1040_112_2_17_22_19_28_963 "MNGPRS:MNGPRS_GetPclinkCfg retrun err!"
#define MNGPRS_API_1059_112_2_17_22_19_28_964 "mn:GPRS_SendPdpActiveCnf fail result = %d"
#define MNGPRS_API_1077_112_2_17_22_19_28_965 "MNGPRS:the pdp context is still in deactivate pending state,force deactivate it"
#define MNGPRS_API_1153_112_2_17_22_19_28_966 "mn:MNGPRS_ReadPdpContextAddr fail result = %d"
#define MNGPRS_API_1460_112_2_17_22_19_29_967 "mn:MNGPRS_ReadCurrQosProfile fail result = %d"
#define MNGPRS_API_1482_112_2_17_22_19_29_968 "mn:MNGPRS_ReadMinQosProfile fail result = %d"
#define MNGPRS_DATA_519_112_2_17_22_19_32_969 "MNGPRS:discard signal(%x) because destination task(%d)'s queue is full"
#define MNGPRS_MODULE_793_112_2_17_22_19_34_970 "MNGPRS:String2PdpAddr result = %d"
#define MNGPRS_MODULE_822_112_2_17_22_19_34_971 "MNGPRS:PdpAddr2String result = %d"
#define MNGPRS_MODULE_1410_112_2_17_22_19_36_972 "MNGPRS:Pdp context %d may be active,set error!"
#define MNGPRS_MODULE_1490_112_2_17_22_19_36_973 "MNGPRS: Get pdp context %d error"
#define MNGPRS_MODULE_1497_112_2_17_22_19_36_974 "MNGPRS:Pdp context %d may be active,set error!"
#define MNGPRS_MODULE_1546_112_2_17_22_19_36_975 "MNGPRS:MN_GPRS_ERR_SUCCESS != result"
#define MNGPRS_MODULE_1616_112_2_17_22_19_36_976 "MNGPRS:Get PDP context not successfully!"
#define MNGPRS_MODULE_2552_112_2_17_22_19_38_977 "mngprs: s_mngprs_contexts_info[%d][%d].context_data_ptr = %x"
#define MNGPRS_MODULE_2928_112_2_17_22_19_39_978 "MNGPRS:s_mngprs_nsapi_info[%d][%d].pdp_context is null"
#define MNGPRS_MODULE_2950_112_2_17_22_19_39_979 "MNGPRS:we do not send PDP DEACTIVATE IND to APP"
#define MNGPRS_MODULE_3124_112_2_17_22_19_39_980 "mngprs: user pdp login and password length =0"
#define MNGPRS_MODULE_3566_112_2_17_22_19_40_981 "MNGPRS: unknow configuration protocol %d."
#define MNGPRS_MODULE_3579_112_2_17_22_19_40_982 "MNGPRS: protocol_id = 0x%x"
#define MNGPRS_MODULE_3619_112_2_17_22_19_40_983 "MNGPRS:the Length field error in IPCP packet"
#define MNGPRS_MODULE_3636_112_2_17_22_19_40_984 "MNGPRS: type = %d"
#define MNGPRS_NV_109_112_2_17_22_19_41_985 "MNGPRS:GPRS_SetNVPdpContext() invalid pdp_id"
#define MNGPRS_NV_337_112_2_17_22_19_42_986 "MNGPRS:EFS_NvitemRead return %d"
#define MNGPRS_NV_363_112_2_17_22_19_42_987 "MNGPRS:EFS_NvitemWrite return %d"
#define MNGPRS_NV_389_112_2_17_22_19_42_988 "MNGPRS:create nv item for pc link config"
#define MNGPRS_SIGNAL_174_112_2_17_22_19_42_989 "MNGPRS:tft =%p"
#define MN_MAIN_949_112_2_17_22_19_48_990 "MN: cann't find the msg %d\n"
#define MN_MAIN_961_112_2_17_22_19_49_991 "MN receive unexpected signal %x\n"
#define MN_MAIN_1202_112_2_17_22_19_49_992 "L4_RUN\n"
#define MNSMS_API_622_112_2_17_22_19_51_993 "MNSMS:MNSMS_SetAssertSmsDestAddr Invalid API!"
#define MNSMS_API_1666_112_2_17_22_19_53_994 "MNSMS_API: INPUT PTR is NULL!"
#define MNSMS_API_1698_112_2_17_22_19_53_995 "MNSMS_API:PTR is NULL!"
#define MNSMS_API_1709_112_2_17_22_19_53_996 "MNSMS::send Command SMS is not allowed for the sc addr is not in the FDN."
#define MNSMS_API_1719_112_2_17_22_19_53_997 "MNSMS::send Command SMS is not allowed for the dest addr is not in the FDN."
#define MNSMS_API_1782_112_2_17_22_19_54_998 "MNSMS: Error length for STK SMS PDU!!"
#define MNSMS_API_1859_112_2_17_22_19_54_999 "MNSMS:the stk sms is need compressed."
#define MNSMS_API_1870_112_2_17_22_19_54_1000 "MNSMS:the stk sms is need compressed but not compressed."
#define MNSMS_API_1885_112_2_17_22_19_54_1001 "MNSMS:the stk sms is not need compressed."
#define MNSMS_API_1931_112_2_17_22_19_54_1002 "MNSMS_API: INPUT PTR is NULL!"
#define MNSMS_API_1990_112_2_17_22_19_54_1003 "MNSMS::send SMS is not allowed for the sc addr is not in the FDN."
#define MNSMS_API_2000_112_2_17_22_19_54_1004 "MNSMS::send SMS is not allowed for the dest addr is not in the FDN."
#define MNSMS_API_2167_112_2_17_22_19_54_1005 "MNSMS_API:PTR is NULL!"
#define MNSMS_API_2184_112_2_17_22_19_54_1006 "MNSMS_API:record id is out of range in SIM!"
#define MNSMS_API_2194_112_2_17_22_19_54_1007 "MNSMS_API:record id is out of range in me!"
#define MNSMS_API_2203_112_2_17_22_19_54_1008 "MNSMS_API: NOT RIGHT STORAGE"
#define MNSMS_API_2263_112_2_17_22_19_55_1009 "MNSMS_API:record id is out of range in sim!"
#define MNSMS_API_2273_112_2_17_22_19_55_1010 "MNSMS_API:record id is out of range in me!"
#define MNSMS_API_2281_112_2_17_22_19_55_1011 "MNSMS_API: NOT RIGHT STATUS"
#define MNSMS_API_2341_112_2_17_22_19_55_1012 "MNSMS_API:PTR is NULL!"
#define MNSMS_API_2351_112_2_17_22_19_55_1013 "MNSMS_API: NOT RIGHT STATUS"
#define MNSMS_API_2441_112_2_17_22_19_55_1014 "MNSMS_API: NOT RIGHT STATUS"
#define MNSMS_API_2511_112_2_17_22_19_55_1015 "MNSMS:MAIN_GetHexSmsMo error is %d."
#define MNSMS_API_2656_112_2_17_22_19_56_1016 "MNSMS_API: INPUT PTR is NULL!"
#define MNSMS_API_2683_112_2_17_22_19_56_1017 "MNSMS_API:PTR is NULL!"
#define MNSMS_API_2759_112_2_17_22_19_56_1018 "MNSMS_API: not right status."
#define MNSMS_API_2930_112_2_17_22_19_56_1019 "MNSMS_API:PTR is NULL!"
#define MNSMS_API_2935_112_2_17_22_19_56_1020 "MNSMS_API:length is out of range"
#define MNSMS_API_2940_112_2_17_22_19_56_1021 "MNSMS_API: dest addr length is out of range"
#define MNSMS_API_3141_112_2_17_22_19_56_1022 "MNSMS_API: INVALID PARAMETER."
#define MNSMS_API_3391_112_2_17_22_19_57_1023 "MNSMS:MNSMS_SetAssertSmsDestAddr Invalid API!"
#define MNSMS_API_3669_112_2_17_22_19_58_1024 "MNSMS: status_report is PNULL"
#define MNSMS_API_3686_112_2_17_22_19_58_1025 "MNSMS: input sc_addr_ptr is PNULL."
#define MNSMS_API_3812_112_2_17_22_19_58_1026 "MNSMS_API: INPUT PTR is NULL!"
#define MNSMS_API_3877_112_2_17_22_19_58_1027 "MNSMS_API: INPUT PTR is NULL!"
#define MNSMS_API_4276_112_2_17_22_19_59_1028 "MNSMS: MNSMS_SetIsSaveMMSPush is_save=%d"
#define MNSMS_API_4498_112_2_17_22_19_59_1029 "MNSMS_API: INPUT PTR is NULL!"
#define MNSMS_API_4532_112_2_17_22_19_59_1030 "MNSMS_API: INPUT PTR is NULL!"
#define MNSMS_API_4547_112_2_17_22_19_59_1031 "MNSMS_GetSmsParamMaxNumEx service_info(%d %d)"
#define MNSMS_API_4573_112_2_17_22_20_0_1032 "MNSMS_API: INPUT PTR is NULL!"
#define MNSMS_EVENT_435_112_2_17_22_20_1_1033 "MNSMS_EVENT:Error msg type in sms callback function!"
#define MNSMS_MODULE_1729_112_2_17_22_20_4_1034 "MNSMS:replace sms not support."
#define MNSMS_MODULE_1733_112_2_17_22_20_4_1035 "MNSMS:replace sms supported."
#define MNSMS_MODULE_1839_112_2_17_22_20_5_1036 "MNSMS: The SIM Card is removed."
#define MNSMS_MODULE_1931_112_2_17_22_20_5_1037 "MNSMS: SIM phase is phase 1, the last used TP_MR is saved in NV."
#define MNSMS_MODULE_1981_112_2_17_22_20_5_1038 "MNSMS: s_sms_sim_info[%d] max_num = %d"
#define MNSMS_MODULE_2013_112_2_17_22_20_5_1039 "MNSMS: get SMS NUM from SIM cause = %d"
#define MNSMS_MODULE_2033_112_2_17_22_20_5_1040 "MNSMS: Read SMS states from SIM ERROR!\n"
#define MNSMS_MODULE_2037_112_2_17_22_20_5_1041 "MNSMS: the number of sms record in the sim is 0."
#define MNSMS_MODULE_2048_112_2_17_22_20_5_1042 "MNSMS: Init (%d) ok"
#define MNSMS_MODULE_2107_112_2_17_22_20_5_1043 "MNSMS: read SMS parameter(%d) from SIM cause = %d"
#define MNSMS_MODULE_2385_112_2_17_22_20_6_1044 "MNSMS: Init (%d) ok"
#define MNSMS_MODULE_2426_112_2_17_22_20_6_1045 "MNSMS:Error status in SMS WRITE CNF signal!"
#define MNSMS_MODULE_2460_112_2_17_22_20_6_1046 "MNSMS memory full status = %d"
#define MNSMS_MODULE_2569_112_2_17_22_20_6_1047 "MNSMS: sim memory capability has free space again."
#define MNSMS_MODULE_2617_112_2_17_22_20_6_1048 "MNSMS: UPDATE_SMS_STATUS_CNF delete SMS in SIM, s_sim_del_all[dual_sys].cur_id is %d\n"
#define MNSMS_MODULE_2641_112_2_17_22_20_6_1049 "MNSMS: UPDATE_SMS_STATUS_CNF delete sms in SIM, s_sim_del_all[dual_sys].cur_id is %d\n"
#define MNSMS_MODULE_2676_112_2_17_22_20_6_1050 "MNSMS: delete all SMS in SIM is success\n"
#define MNSMS_MODULE_2687_112_2_17_22_20_6_1051 "MNSMS:DeleteSmsInSim\n"
#define MNSMS_MODULE_2991_112_2_17_22_20_7_1052 "MNSMS: HandleAppMnReadSmsReq not right storage"
#define MNSMS_MODULE_3043_112_2_17_22_20_7_1053 "MNSMS: read NV SMS fail, No. is %d"
#define MNSMS_MODULE_3173_112_2_17_22_20_7_1054 "MNSMS: start delete all sms in sim\n"
#define MNSMS_MODULE_3300_112_2_17_22_20_7_1055 "MNSMS: read NV SMS fail, No. is %d"
#define MNSMS_MODULE_3366_112_2_17_22_20_8_1056 "MNSMS: HandleAppMnWriteSmsReq not right sm status(%d)"
#define MNSMS_MODULE_3449_112_2_17_22_20_8_1057 "MNSMS: HandleAppMnWriteSmsReq not right storage"
#define MNSMS_MODULE_3494_112_2_17_22_20_8_1058 "MNSMS: write SMS to SIM."
#define MNSMS_MODULE_3516_112_2_17_22_20_8_1059 "MNSMS: read NV SMS fail, No. is %d"
#define MNSMS_MODULE_3651_112_2_17_22_20_8_1060 "MNSMS: read NV SMS fail, No. is %d"
#define MNSMS_MODULE_3691_112_2_17_22_20_8_1061 "MNSMS: recv SMS_DELIVER."
#define MNSMS_MODULE_3695_112_2_17_22_20_8_1062 "MNSMS: recv SMS_STATE_REPORT."
#define MNSMS_MODULE_3699_112_2_17_22_20_8_1063 "MNSMS: MT procedure is failure."
#define MNSMS_MODULE_3969_112_2_17_22_20_9_1064 "MNSMS: waiting for set receive sms cnf\n"
#define MNSMS_MODULE_3976_112_2_17_22_20_9_1065 "MNSMS: set receive sms error!\n"
#define MNSMS_MODULE_3982_112_2_17_22_20_9_1066 "MNSMS: set not receive sms\n"
#define MNSMS_MODULE_4101_112_2_17_22_20_9_1067 "MNSMS: MO SMS operation is OK.\n"
#define MNSMS_MODULE_4129_112_2_17_22_20_9_1068 "MNSMS: Send SMS is not success.\n"
#define MNSMS_MODULE_4161_112_2_17_22_20_9_1069 "MNSMS: the fail cause type is %d,cause value is 0x%x"
#define MNSMS_MODULE_4298_112_2_17_22_20_9_1070 "MNSMS: SetVoimsgInd() decode character1 error in CPHS\n"
#define MNSMS_MODULE_4306_112_2_17_22_20_9_1071 "MNSMS: SetVoimsgInd() decode character2 error in CPHS\n"
#define MNSMS_MODULE_4339_112_2_17_22_20_10_1072 "MNSMS:receive a voice message waiting message need discard."
#define MNSMS_MODULE_4383_112_2_17_22_20_10_1073 "MNSMS: MT Short Message procedure is failure. Error = %lx"
#define MNSMS_MODULE_4390_112_2_17_22_20_10_1074 "MNSMS:receive SMS PP download data\n!"
#define MNSMS_MODULE_4407_112_2_17_22_20_10_1075 "MNSMS:receive a sms with type 0, discard it."
#define MNSMS_MODULE_4439_112_2_17_22_20_10_1076 "MNSMS: replace SMS storage(%d) #%d"
#define MNSMS_MODULE_4463_112_2_17_22_20_10_1077 "MNSMS: dont need replace SMS, Normal receive SMS"
#define MNSMS_MODULE_4478_112_2_17_22_20_10_1078 "MNSMS: Receive a discarded message, msg_is_waiting = %d"
#define MNSMS_MODULE_4484_112_2_17_22_20_10_1079 "MNSMS: deal sms voicemessage indicator error\n"
#define MNSMS_MODULE_4519_112_2_17_22_20_10_1080 "MNSMS: DealSmsVoimsgInd line error\n"
#define MNSMS_MODULE_4533_112_2_17_22_20_10_1081 "MNSMS :  DealSmsVoimsgInd need not to update sim files."
#define MNSMS_MODULE_4537_112_2_17_22_20_10_1082 "MNSMS: DealSmsVoimsgInd wait type error\n"
#define MNSMS_MODULE_4546_112_2_17_22_20_10_1083 "MNSMS: save VMW flag error\n"
#define MNSMS_MODULE_4551_112_2_17_22_20_10_1084 "MNSMS: save VMW flag success\n"
#define MNSMS_MODULE_4620_112_2_17_22_20_10_1085 "MNSMS: input sc_addr_ptr is PNULL."
#define MNSMS_MODULE_4671_112_2_17_22_20_10_1086 "MNSMS: MNSMS_SaveStatusReportInME: dual_sys = %d, position = %d"
#define MNSMS_MODULE_4740_112_2_17_22_20_10_1087 "SMS: DealStatusReport dual_sys = %d, storage = %d, position = %d"
#define MNSMS_MODULE_4813_112_2_17_22_20_10_1088 "MNSMS: MT Short Message procedure is failure. Error = %lx"
#define MNSMS_MODULE_4871_112_2_17_22_20_11_1089 "MNSMS: MT procedure failure for Decode Status Report Error."
#define MNSMS_MODULE_5147_112_2_17_22_20_11_1090 "MNSMS: deal sms voicemessage indicator error\n"
#define MNSMS_MODULE_5189_112_2_17_22_20_11_1091 "MNSMS memory full status = %d"
#define MNSMS_MODULE_5317_112_2_17_22_20_11_1092 "MNSMS: mms notification recv."
#define MNSMS_MODULE_5334_112_2_17_22_20_11_1093 "MNSMS: to save the MMS notification."
#define MNSMS_MODULE_5360_112_2_17_22_20_12_1094 "MNSMS: MT Short Message Procedure is success.\n"
#define MNSMS_MODULE_5380_112_2_17_22_20_12_1095 "MNSMS: DealMtSmsWithClass not right class"
#define MNSMS_MODULE_5402_112_2_17_22_20_12_1096 "MNSMS memory full status = %d"
#define MNSMS_MODULE_5422_112_2_17_22_20_12_1097 "MNSMS: save MT Short Message to ME"
#define MNSMS_MODULE_5459_112_2_17_22_20_12_1098 "MNSMS: malloc s_mt_sms_ptr[dual_sys] error.\n"
#define MNSMS_MODULE_5511_112_2_17_22_20_12_1099 "MNSMS: deal SMS VOICEMESSAGE indicator error\n"
#define MNSMS_MODULE_5522_112_2_17_22_20_12_1100 "MNSMS: MT Short Message procedure is failure. Error = %lx"
#define MNSMS_MODULE_5601_112_2_17_22_20_12_1101 "MNSMS: NVSMS_REFRESH_REPLACE_INFO_CNF result = %d"
#define MNSMS_MODULE_5750_112_2_17_22_20_12_1102 "MNSMS: delete all SMS status not right\n"
#define MNSMS_MODULE_5811_112_2_17_22_20_12_1103 "MNSMS: status_is_correct is FALSE"
#define MNSMS_MODULE_5867_112_2_17_22_20_13_1104 "MNSMS: writeMoMsg not need save."
#define MNSMS_MODULE_5937_112_2_17_22_20_13_1105 "MNSMS: write tp_mr to NV now"
#define MNSMS_MODULE_6287_112_2_17_22_20_13_1106 "MNSMS: there is no TP-Validity-Period\n"
#define MNSMS_MODULE_6565_112_2_17_22_20_14_1107 "MNSMS: the alphabet of Data Coding Scheme is error!"
#define MNSMS_MODULE_6569_112_2_17_22_20_14_1108 "MNSMS: SMS_EncodeUserData user data head length is %d,data length is %d."
#define MNSMS_MODULE_6618_112_2_17_22_20_14_1109 "MNSMS:APP send SMS command to SC with TP_MR = %d"
#define MNSMS_MODULE_6633_112_2_17_22_20_14_1110 "MNSMS: SMS_PackSmsCommand not right command_type_e."
#define MNSMS_MODULE_6741_112_2_17_22_20_14_1111 "MNSMS_API:sms length out of range!"
#define MNSMS_MODULE_6998_112_2_17_22_20_15_1112 "MNSMS: called number length is out of the range"
#define MNSMS_MODULE_7052_112_2_17_22_20_15_1113 "MNSMS: called number length is out of the range"
#define MNSMS_MODULE_7332_112_2_17_22_20_16_1114 "MNSMS: !!! SMS DEFAULT ALPHABET: the user data length is longer than the tpdu contained!"
#define MNSMS_MODULE_7337_112_2_17_22_20_16_1115 "MNSMS: !!! NOT DEFAULT ALPHABET:the user data length is longer than the tpdu contained!"
#define MNSMS_MODULE_7364_112_2_17_22_20_16_1116 "MNSMS:the msg type is 0."
#define MNSMS_MODULE_7407_112_2_17_22_20_16_1117 "MNSMS: decode length error\n"
#define MNSMS_MODULE_7429_112_2_17_22_20_16_1118 "MNSMS: UnPackStatusReport success! tpdu_ptr->l3_msg[%d] is %dn"
#define MNSMS_MODULE_7433_112_2_17_22_20_16_1119 "MNSMS: decode TP called num error\n"
#define MNSMS_MODULE_7514_112_2_17_22_20_16_1120 "MNSMS:UnPackMoTpduInSim, is deliver msg."
#define MNSMS_MODULE_7519_112_2_17_22_20_16_1121 "MNSMS:UnPackMoTpduInSim, is status report msg."
#define MNSMS_MODULE_7545_112_2_17_22_20_16_1122 "MNSMS:UnPackMoTpduInSim, is submit msg."
#define MNSMS_MODULE_7548_112_2_17_22_20_16_1123 "MNSMS:UnPackMoTpduInSim, error msg type."
#define MNSMS_MODULE_7589_112_2_17_22_20_16_1124 "MNSMS: Read SMS_DELIVER Message."
#define MNSMS_MODULE_7607_112_2_17_22_20_16_1125 "MNSMS: Read SMS_STATE_REPORT Message."
#define MNSMS_MODULE_7626_112_2_17_22_20_16_1126 "MNSMS:UnPackMtTpduInSim, submit msg."
#define MNSMS_MODULE_7644_112_2_17_22_20_16_1127 "MNSMS:UnPackMtTpduInSim error msg type."
#define MNSMS_MODULE_7707_112_2_17_22_20_17_1128 "MNSMS: UnPack MO TPDU, TP_MR = %d"
#define MNSMS_MODULE_7726_112_2_17_22_20_17_1129 "MNSMS: UnPackMoTpduInMe DecodeDcs result = %d"
#define MNSMS_MODULE_7800_112_2_17_22_20_17_1130 "MNSMS: Read SMS_DELIVER Message."
#define MNSMS_MODULE_7810_112_2_17_22_20_17_1131 "MNSMS: Read SMS_STATE_REPORT Message."
#define MNSMS_MODULE_7823_112_2_17_22_20_17_1132 "MNSMS: Read Error type message of MT SMS."
#define MNSMS_MODULE_8013_112_2_17_22_20_18_1133 "MNSMS: decode DCS unexcepted value"
#define MNSMS_MODULE_8072_112_2_17_22_20_18_1134 "MNSMS: there is no TP-Validity-Period\n"
#define MNSMS_MODULE_8203_112_2_17_22_20_18_1135 "MNSMS: input user head is invalid!"
#define MNSMS_MODULE_8924_112_2_17_22_20_21_1136 "MNSMS: unknown user head type!"
#define MNSMS_MODULE_9350_112_2_17_22_20_22_1137 "MNSMS:no space in SIM or ME."
#define MNSMS_MODULE_9408_112_2_17_22_20_22_1138 "MNSMS: GetRefAndMaxNumFromHead iei = %d"
#define MNSMS_MODULE_9453_112_2_17_22_20_22_1139 "MNSMS: SMS_CalculateSmsNumber SMS num in me is %d"
#define MNSMS_MODULE_9513_112_2_17_22_20_22_1140 "MNSMS: IsMemAvail is_sim_full = %d, is_me_full = %d, mem_status = %d"
#define MNSMS_MODULE_9761_112_2_17_22_20_22_1141 "MNSMS: Compare Replace Header result is %d"
#define MNSMS_MODULE_9868_112_2_17_22_20_23_1142 "MNSMS: replace sms to SIM."
#define MNSMS_MODULE_9922_112_2_17_22_20_23_1143 "MNSMS: MT replace Short Message Procedure is success."
#define MNSMS_MODULE_9927_112_2_17_22_20_23_1144 "MNSMS: deal SMS VOICEMESSAGE indicator error\n"
#define MNSMS_MODULE_10027_112_2_17_22_20_23_1145 "MNSMS: deal sms voicemessage indicator error\n"
#define MNSMS_MODULE_10034_112_2_17_22_20_23_1146 "MNSMS: replace sms error!\n"
#define MNSMS_MODULE_10162_112_2_17_22_20_23_1147 "MNSMS: write replace SMS flag from %d to %d"
#define MNSMS_MODULE_10183_112_2_17_22_20_23_1148 "MNSMS: write SMS replace support flag fail, write NV fail"
#define MNSMS_MODULE_10189_112_2_17_22_20_23_1149 "MNSMS: write sms replace support flag fail, sim not ready"
#define MNSMS_MODULE_10259_112_2_17_22_20_23_1150 "MNSMS: NVSMS_INIT_CNF result = %d"
#define MNSMS_MODULE_10335_112_2_17_22_20_24_1151 "MNSMS: write SMS to NV, TP_MR=%d."
#define MNSMS_MODULE_10534_112_2_17_22_20_24_1152 "MNSMS: decrease the hold num of card %d to %d."
#define MNSMS_MODULE_10585_112_2_17_22_20_24_1153 "MNSMS: SMS_DecodeSTKMoTpduData DecodeDcs result = %d"
#define MNSMS_MODULE_10598_112_2_17_22_20_24_1154 "MNSMS:the dcs from the SIM card is error."
#define MNSMS_MODULE_10608_112_2_17_22_20_24_1155 "MNSMS:SMS_DecodeSTKMoTpduData the stk dcs is 7bit and data len is %d."
#define MNSMS_MODULE_10620_112_2_17_22_20_24_1156 "MNSMS:modify the stk dcs from 7bit to 8bit."
#define MNSMS_MODULE_10708_112_2_17_22_20_24_1157 "MNSMS: SMS_DecodeSTKUserData user_data_length is %d"
#define MNSMS_MODULE_10716_112_2_17_22_20_24_1158 "MNSMS: user_data_header_ptr_t->length is %d"
#define MNSMS_MODULE_10755_112_2_17_22_20_24_1159 "MNSMS: SMS_DecodeSTKUserData sms_length is %d"
#define MNSMS_MODULE_10779_112_2_17_22_20_24_1160 "MNSMS:no user header in the stk sms user data."
#define MNSMS_MODULE_10786_112_2_17_22_20_25_1161 "MNSMS:there is user header in the stk sms user data."
#define MNSMS_MODULE_10811_112_2_17_22_20_25_1162 "MNSMS: SMS_DecodeSTKUserData user_valid_data_ptr_t->length is %d"
#define MNSMS_MODULE_11068_112_2_17_22_20_25_1163 "MNSMS: SMS_UnPackSmsCommand not right command_type_e."
#define MNSMS_MODULE_11076_112_2_17_22_20_25_1164 "MNSMS: SMS_UnPackSmsCommand not right command_type_e."
#define MNSMS_MODULE_11098_112_2_17_22_20_25_1165 "MNSMS: sim(%d) max_smsp_num = %d"
#define MNSMS_MODULE_11136_112_2_17_22_20_25_1166 "SMS_GetSimParamDefaultId max_num = %d, default_id = %d"
#define MNSMS_MODULE_11347_112_2_17_22_20_26_1167 "SMS: undefined sms_local_cause(0x%x)"
#define MNSMS_NV_MODULE_723_112_2_17_22_20_27_1168 "MNSMS: get s_sms_nv_info.max_num is %d"
#define MNSMS_NV_MODULE_830_112_2_17_22_20_28_1169 "MNSMS: write NV SMS, result = %d, rid = %d"
#define MNSMS_NV_MODULE_848_112_2_17_22_20_28_1170 "MNSMS: NVSMS_ReadNvSmsDataReq(%d, %d)"
#define MNSMS_NV_MODULE_1333_112_2_17_22_20_29_1171 "MNSMS: NVSMS_SaveStatusReportReq dual_sys = %d"
#define MNSMS_NV_MODULE_1755_112_2_17_22_20_30_1172 "MNSMS: delete all SMS in ME result = %d"
#define MNSMS_NV_MODULE_1774_112_2_17_22_20_30_1173 "MNSMS: NVSMS_IsMemFull used = %d, max = %d"
#define MNSMS_NV_MODULE_1851_112_2_17_22_20_30_1174 "MNSMS: NVSMS_ReadNvSmsNumInfo(%d, %d)"
#define MNSMSCB_MODULE_429_112_2_17_22_20_33_1175 "SMSCB: Initial CB service according to parameters requested by user "
#define MNSMSCB_MODULE_568_112_2_17_22_20_33_1176 "SMSCB:msg id %d is not new added."
#define MNSMSCB_MODULE_669_112_2_17_22_20_33_1177 "MNSMSCB NextState: Block1 pending."
#define MNSMSCB_MODULE_674_112_2_17_22_20_33_1178 "MNSMSCB NextState: Block2 pending."
#define MNSMSCB_MODULE_689_112_2_17_22_20_33_1179 "MNSMSCB NextState: Schedule Block1 pending."
#define MNSMSCB_MODULE_694_112_2_17_22_20_33_1180 "MNSMSCB NextState: Schedule Block2 pending."
#define MNSMSCB_MODULE_702_112_2_17_22_20_33_1181 "MNSMSCB: Discard one block in Block1 pending state."
#define MNSMSCB_MODULE_745_112_2_17_22_20_33_1182 "MNSMSCB NextState: Block1 pending."
#define MNSMSCB_MODULE_760_112_2_17_22_20_33_1183 "MNSMSCB NextState: Block1 pending."
#define MNSMSCB_MODULE_773_112_2_17_22_20_33_1184 "MNSMSCB NextState: Block1 pending."
#define MNSMSCB_MODULE_778_112_2_17_22_20_33_1185 "MNSMSCB NextState: Block2 pending."
#define MNSMSCB_MODULE_793_112_2_17_22_20_33_1186 "MNSMSCB NextState: Schedule Block1 pending."
#define MNSMSCB_MODULE_798_112_2_17_22_20_33_1187 "MNSMSCB NextState: Schedule Block2 pending."
#define MNSMSCB_MODULE_1145_112_2_17_22_20_34_1188 "MNSMSCB::enter IsNewSmscbMsg."
#define MNSMSCB_MODULE_1161_112_2_17_22_20_34_1189 "MNSMSCB:: class 3 msg,receive it."
#define MNSMSCB_MODULE_1181_112_2_17_22_20_34_1190 "MNSMSCB::plmn wide,msg code is same,update number is valid,so receive the msg."
#define MNSMSCB_MODULE_1187_112_2_17_22_20_34_1191 "MNSMSCB::plmn wide,msg code is same,receive in different plmn,so receive the msg."
#define MNSMSCB_MODULE_1204_112_2_17_22_20_34_1192 "MNSMSCB::lac wide,msg code is same,receive in same lac,but update number is valid,so receive the msg."
#define MNSMSCB_MODULE_1210_112_2_17_22_20_34_1193 "MNSMSCB::lac wide,msg code is same,receive in different lac,so receive the msg."
#define MNSMSCB_MODULE_1217_112_2_17_22_20_34_1194 "MNSMSCB::lac wide,msg code is same,receive in different plmn,so receive the msg."
#define MNSMSCB_MODULE_1238_112_2_17_22_20_34_1195 "MNSMSCB::cell wide,msg code is same,receive in same cell,update number is valid,so receive the msg."
#define MNSMSCB_MODULE_1244_112_2_17_22_20_34_1196 "MNSMSCB::cell wide,msg code is same,receive in different cell,so receive the msg."
#define MNSMSCB_MODULE_1250_112_2_17_22_20_34_1197 "MNSMSCB::cell wide,msg code is same,receive in different lac,so receive the msg."
#define MNSMSCB_MODULE_1257_112_2_17_22_20_34_1198 "MNSMSCB::cell wide,msg code is same,receive in different plmn,so receive the msg."
#define MNSMSCB_MODULE_1263_112_2_17_22_20_34_1199 "SMSCB: same msg-code,discard this message for geo scope or update number."
#define MNSMSCB_MODULE_1269_112_2_17_22_20_34_1200 "SMSCB: new msg-code,receive this message."
#define MNSMSCB_MODULE_1274_112_2_17_22_20_34_1201 "SMSCB:it is not interested msg id,discard this message."
#define MNSMSCB_MODULE_1298_112_2_17_22_20_35_1202 "cb msg id %d is download id"
#define MNSMSCB_MODULE_1303_112_2_17_22_20_35_1203 "cb msg id %d is not download id"
#define MNSMSCB_MODULE_1437_112_2_17_22_20_35_1204 " SMSCB:Ignore one stk download CB message which message has been downloaded."
#define MNSMSCB_MODULE_1446_112_2_17_22_20_35_1205 " SMSCB:Ignore one CB message which language ID isn't supported by user"
#define MNSMSCB_MODULE_1466_112_2_17_22_20_35_1206 "SMSCB:Deal CB message: Total Page = %d,Current Page = %d,msg_code = %x,msg_id=%x,dcs=%x"
#define MNSMSCB_MODULE_1480_112_2_17_22_20_35_1207 "SMSCB:receive one page sms cb msg."
#define MNSMSCB_MODULE_1535_112_2_17_22_20_35_1208 "SMSCB:receive %d page sms cb msg."
#define MNSMSCB_MODULE_1642_112_2_17_22_20_35_1209 "SMSCB:GET_BIT=1,i=%d"
#define MNSMSCB_MODULE_1646_112_2_17_22_20_35_1210 "SMSCB:MDT=1,offset_octets=%d"
#define MNSMSCB_MODULE_1663_112_2_17_22_20_35_1211 "SMSCB:MDT=0,offset_octets=%d"
#define MNSMSCB_MODULE_1696_112_2_17_22_20_35_1212 "SMSCB:msg_id=%x,marker_ptr=%x%x%x%x%x%x%x%x%x%x%x%x"
#define MNSMSCB_MODULE_1787_112_2_17_22_20_35_1213 "MNSMSCB:: class 3 msg,receive it."
#define MNSMSCB_MODULE_1807_112_2_17_22_20_36_1214 "MNSMSCB::plmn wide,msg code is same,update number is valid,so receive the msg."
#define MNSMSCB_MODULE_1814_112_2_17_22_20_36_1215 "MNSMSCB::plmn wide,msg code is same,receive in different plmn,so receive the msg."
#define MNSMSCB_MODULE_1831_112_2_17_22_20_36_1216 "MNSMSCB::lac wide,msg code is same,update number is valid,so receive the msg."
#define MNSMSCB_MODULE_1837_112_2_17_22_20_36_1217 "MNSMSCB::lac wide,msg code is same,receive in different lac,so receive the msg."
#define MNSMSCB_MODULE_1844_112_2_17_22_20_36_1218 "MNSMSCB::lac wide,msg code is same,receive in different plmn,so receive the msg."
#define MNSMSCB_MODULE_1865_112_2_17_22_20_36_1219 "MNSMSCB::cell wide,msg code is same,update num is valid,so receive the msg."
#define MNSMSCB_MODULE_1871_112_2_17_22_20_36_1220 "MNSMSCB::cell wide,msg code is same,receive in different cell,so receive the msg."
#define MNSMSCB_MODULE_1877_112_2_17_22_20_36_1221 "MNSMSCB::cell wide,msg code is same,receive in different lac,so receive the msg."
#define MNSMSCB_MODULE_1884_112_2_17_22_20_36_1222 "MNSMSCB::cell wide,msg code is same,receive in different plmn,so receive the msg."
#define MNSMSCB_MODULE_1891_112_2_17_22_20_36_1223 "SMSCB: new msg,so update msg marker."
#define MNSMSCB_MODULE_1905_112_2_17_22_20_36_1224 "SMSCB:a received msg,so not update msg marker."
#define MNSMSCB_MODULE_1929_112_2_17_22_20_36_1225 "SMSCB: new msg code,so update msg marker."
#define MNSMSCB_MODULE_1933_112_2_17_22_20_36_1226 "SMSCB:msg id is not interested,so not update msg marker."
#define MNSMSCB_MODULE_1989_112_2_17_22_20_36_1227 "SMSCB: old update num is %d,new update number is %d,update number is valid."
#define MNSMSCB_MODULE_1995_112_2_17_22_20_36_1228 "SMSCB: old update num is %d,new update number is %d,update number is invalid."
#define MNSS_API_233_112_2_17_22_20_37_1229 "MNSS:string len beyond the max len 255!"
#define MNSS_API_313_112_2_17_22_20_37_1230 "MNSS::encode ss string failed."
#define MNSS_API_478_112_2_17_22_20_38_1231 "MNSS:adjust the fill bit to 0x1a."
#define MNSS_API_665_112_2_17_22_20_38_1232 "MNSS_API:forward number length is invalid!"
#define MNSS_API_699_112_2_17_22_20_38_1233 "MNSS::register ss control by SIM."
#define MNSS_API_742_112_2_17_22_20_38_1234 "MNSS_API: The ss-code can not be operated by register.\n"
#define MNSS_API_822_112_2_17_22_20_38_1235 "MNSS: The ss-code can not be operated by erase.\n"
#define MNSS_API_903_112_2_17_22_20_38_1236 "MNSS: The ss-code can not be operated by activate.\n"
#define MNSS_API_985_112_2_17_22_20_39_1237 "MNSS: The ss-code can not be operated by deactivation.\n"
#define MNSS_API_1148_112_2_17_22_20_39_1238 "MNSS: the input password is not numeric"
#define MNSS_API_1242_112_2_17_22_20_39_1239 "MNSS_API: Str length is invalid!"
#define MNSS_API_1249_112_2_17_22_20_39_1240 "MNSS_API: input str_len is %d, out of range!"
#define MNSS_API_1286_112_2_17_22_20_39_1241 "MNSS_API: Str length is invalid!"
#define MNSS_API_1299_112_2_17_22_20_39_1242 "MNSS_API: input str_len is %d, out of range!"
#define MNSS_API_1336_112_2_17_22_20_39_1243 "MNSS:string len beyond the max len 255!"
#define MNSS_API_1464_112_2_17_22_20_40_1244 "MNSS::encode ss string failed."
#define MNSS_API_1652_112_2_17_22_20_40_1245 "MNSS::ss is prevented by FDN."
#define MNSS_API_1672_112_2_17_22_20_40_1246 "MNSS:adjust the fill bit to 0x1a."
#define MNSS_API_1781_112_2_17_22_20_40_1247 "MNSS: ccbs_index=%d is wrong.\n"
#define MNSS_API_1926_112_2_17_22_20_40_1248 "MNSS::SendSSControlResultToAPP operate_type is error!"
#define MNSS_API_1958_112_2_17_22_20_41_1249 "MNSS::encode ss string failed."
#define MNSS_API_1980_112_2_17_22_20_41_1250 "MNSS::ss is prevented by FDN."
#define MNSS_MODULE_626_112_2_17_22_20_44_1251 "DecodeSSDigitInfo:point is null"
#define MNSS_MODULE_659_112_2_17_22_20_44_1252 "DecodeSSStringInfo:point is null"
#define MNSS_MODULE_943_112_2_17_22_20_44_1253 "MNSS:error length = 0\n"
#define MNSS_MODULE_1282_112_2_17_22_20_45_1254 "Input error"
#define MNSS_MODULE_1431_112_2_17_22_20_45_1255 "MNSS: Idle_HandleBegineInd diagnose_type = %d"
#define MNSS_MODULE_1462_112_2_17_22_20_45_1256 "other_cause = %d"
#define MNSS_MODULE_2131_112_2_17_22_20_47_1257 "MNSS: received MNSS_BEGIN_IND while mnss in active state.\n"
#define MNSS_MODULE_2162_112_2_17_22_20_47_1258 "MNSS: send out release complete for reject new transaction \n"
#define MNSS_MODULE_2190_112_2_17_22_20_47_1259 "MNSS: Active_HandleFacilityMsg diagnose_type = %d"
#define MNSS_MODULE_2211_112_2_17_22_20_47_1260 "other_cause = %d"
#define MNSS_MODULE_2254_112_2_17_22_20_47_1261 "MNSS: resend ussd through Process_ussd_data operation.\n"
#define MNSS_MODULE_2275_112_2_17_22_20_47_1262 "MNSS: release complete received but component type is Invoke.\n"
#define MNSS_MODULE_2301_112_2_17_22_20_47_1263 "MNSS: release complete received but result component decode error.\n"
#define MNSS_MODULE_2309_112_2_17_22_20_47_1264 "MNSS: release complete received, operation is reject by network.\n"
#define MNSS_MODULE_2324_112_2_17_22_20_47_1265 "MNSS:Active_HandleEndMsg(), component type error!"
#define MNSS_MODULE_2333_112_2_17_22_20_47_1266 "MNSS: release complete received decode facility ie ERROR!"
#define MNSS_MODULE_2342_112_2_17_22_20_47_1267 "MNSS: release complete received but no facility ie included in message.\n"
#define MNSS_MODULE_2352_112_2_17_22_20_47_1268 "MNSS:notify MMI of the result of sending ussd data."
#define MNSS_MODULE_2373_112_2_17_22_20_47_1269 "MNSS: ss failed for mm connection release or error."
#define MNSS_MODULE_2378_112_2_17_22_20_47_1270 "MNSS:ss service retry."
#define MNSS_MODULE_2401_112_2_17_22_20_47_1271 "MNSS:ussd notify is released by network before user release."
#define MNSS_MODULE_2428_112_2_17_22_20_47_1272 "MNSS: User release active operation.\n"
#define MNSS_MODULE_2532_112_2_17_22_20_47_1273 "MNSS: user response ussd operation error.\n"
#define MNSS_MODULE_2613_112_2_17_22_20_48_1274 "MNSS: receive Unstructured service notify invoke from network at idle state. \n"
#define MNSS_MODULE_2634_112_2_17_22_20_48_1275 "MNSS: send out Unstructured service notify return result message.\n"
#define MNSS_MODULE_2665_112_2_17_22_20_48_1276 "MNSS: component tag is error. \n"
#define MNSS_MODULE_2757_112_2_17_22_20_48_1277 "MNSS: HandleFacilitySucc() component tag is error. "
#define MNSS_MODULE_2949_112_2_17_22_20_48_1278 "MNSS: HandleOperationResult operation_code = %d"
#define MNSS_MODULE_3003_112_2_17_22_20_48_1279 "MNSS:HandleOperationResult(),s_active_entity[dual_sys].operate.operation_code = %d"
#define MNSS_MODULE_3041_112_2_17_22_20_48_1280 "MNSS:operation_code =%d"
#define MNSS_MODULE_3074_112_2_17_22_20_49_1281 "MNSS:component_ptr->content_union.error = %d"
#define MNSS_MODULE_3099_112_2_17_22_20_49_1282 "MNSS:The Failure Operate is SS_UNSTRUCTSS_NOTIFY!\n"
#define MNSS_MODULE_3110_112_2_17_22_20_49_1283 "MNSS: assert faulure, operation_code = %d"
#define MNSS_MODULE_3134_112_2_17_22_20_49_1284 "MNSS: InitProcessUssdData unpack ussd string."
#define MNSS_MODULE_3514_112_2_17_22_20_49_1285 "MNSS:SS_EncodeRegisterSSStr,forward number length is invalid!"
#define MNSS_MODULE_3530_112_2_17_22_20_49_1286 "MNSS: SS_EncodeRegisterSSStr,The ss-code can not be operated by register.\n"
#define MNSS_MODULE_3576_112_2_17_22_20_50_1287 "MNSS: The ss-code can not be operated by erase.\n"
#define MNSS_MODULE_3623_112_2_17_22_20_50_1288 "MNSS: The ss-code can not be operated by activate.\n"
#define MNSS_MODULE_3669_112_2_17_22_20_50_1289 "MNSS: The ss-code can not be operated by deactivation.\n"
#define MNSS_MODULE_3760_112_2_17_22_20_50_1290 "MNSS::SS_EncodeRegisterPasswordSSStr for all car bar service."
#define MNSS_MODULE_3986_112_2_17_22_20_50_1291 "MNSS:unexpected operation code!"
#define MNSS_MSG_1105_112_2_17_22_20_53_1292 "MNSS::DecodeUSSDdata data len is 0."
#define MNSS_MSG_1878_112_2_17_22_20_55_1293 "MNSS: CALL BARRING INFO ss status is %d"
#define MNSS_MSG_2652_112_2_17_22_20_56_1294 "MNSS::network only return a list of basic service."
#define MNSS_MSG_2826_112_2_17_22_20_57_1295 "MNSS: DecodeResultRspSSinfo 0 == buf_length\n"
#define MNSS_MSG_2849_112_2_17_22_20_57_1296 "MNSS: decode callbarringinfo"
#define MNSS_MSG_2920_112_2_17_22_20_57_1297 "MNSS: DecodeEraseCcbsRspSSinfo 0 == buf_length\n"
#define MNSS_MSG_3052_112_2_17_22_20_57_1298 "MNSS: DecodeResultFollowedParam component_len.len is error\n"
#define MNSS_MSG_3065_112_2_17_22_20_57_1299 "MNSS: DecodeResultFollowedParam buf_length is error\n"
#define MNSS_MSG_3085_112_2_17_22_20_57_1300 "MNSS: DecodeResultFollowedParam SS_COMPONENT_OPERATION_CODE_TAG is error\n"
#define MNSS_MSG_3144_112_2_17_22_20_57_1301 "MNSS: DecodeResultFollowedParam decode register activatess erasess deactivatess\n"
#define MNSS_MSG_3157_112_2_17_22_20_57_1302 "MNSS: DecodeResultFollowedParam decode register activatess erasess deactivatess is error\n"
#define MNSS_MSG_3176_112_2_17_22_20_57_1303 "MNSS: DecodeResultFollowedParam decode register activatess erasess deactivatess is error\n"
#define MNSS_MSG_3385_112_2_17_22_20_58_1304 "MNSS: DecodeResultComponent length  is error\n"
#define MNSS_MSG_3398_112_2_17_22_20_58_1305 "MNSS: DecodeResultComponent buf_length error\n"
#define MNSS_MSG_3418_112_2_17_22_20_58_1306 "MNSS: DecodeResultComponent SS_COMPONENT_INVOKE_ID_TAG  is error\n"
#define MNSS_MSG_3435_112_2_17_22_20_58_1307 "MNSS: DecodeResultComponent SS_COMPONENT_SEQUENCE_TAG  is error\n"
#define MNSS_MSG_3690_112_2_17_22_20_58_1308 "MNSS: MNSSMSG_DecodeFacilityIe component_tag = %d"
#define MNSS_MSG_3828_112_2_17_22_20_59_1309 "MNSS::override category is present."
#define MNSS_MSG_3874_112_2_17_22_20_59_1310 "MNSS::default priority is present."
#define SSCALLRELATE_428_112_2_17_22_21_2_1311 "MNSS sscallrelate encode deflect number out of range "
#define SSCALLRELATE_443_112_2_17_22_21_2_1312 "MNSS sscallrelate encode subaddress out of range "
#define SSCALLRELATE_456_112_2_17_22_21_2_1313 "MNSS sscallrelate encode call deflect out of range "
#define SSCALLRELATE_540_112_2_17_22_21_2_1314 "MNCALL: not the right operation code\n"
#define SSCALLRELATE_669_112_2_17_22_21_2_1315 "MNCALL: component type error to encode.\n"
#define SSCALLRELATE_857_112_2_17_22_21_3_1316 "MNCALL: not the right tag\n"
#define SSCALLRELATE_862_112_2_17_22_21_3_1317 "MNCALL: not the right ss code\n"
#define SSCALLRELATE_1064_112_2_17_22_21_3_1318 "MNCALL: not the right tag\n"
#define SSCALLRELATE_1709_112_2_17_22_21_4_1319 "MNCALL: not the right operate code\n"
#define SSCALLRELATE_1845_112_2_17_22_21_5_1320 "MNCALL: ussd data length out of range\n"
#define SSCALLRELATE_1916_112_2_17_22_21_5_1321 "MNSS: SSBASIC_DecodeCcbsRsp component_len.len + cur_offset) <= buf_length  is error\n"
#define SSCALLRELATE_2444_112_2_17_22_21_6_1322 "MNCALL: component tage is error to encode\n"
#define SIMAT_API_934_112_2_17_22_21_8_1323 "SIMAT::call control is enabled."
#define SIMAT_API_958_112_2_17_22_21_8_1324 "SIMAT::MO SMS control is enabled."
#define SIMAT_API_2011_112_2_17_22_21_10_1325 "SIMAT::SIMAT_IsNeedSMSControl result = %d"
#define SIMAT_API_2433_112_2_17_22_21_11_1326 "MN: refresh_type error %d"
#define SIMAT_CONFIRM_324_112_2_17_22_21_13_1327 "SIMAT:PC list is null.error!!!"
#define SIMAT_CONFIRM_340_112_2_17_22_21_13_1328 "SIMAT:%d pc not been found,error!!!"
#define SIMAT_CONFIRM_369_112_2_17_22_21_13_1329 "SIMAT:SIMAT_SaveCurPCInfo,error com_len = %d,data buf(1-10):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2xn"
#define SIMAT_CONFIRM_464_112_2_17_22_21_14_1330 "SIMAT:PC list is null.error!!!"
#define SIMAT_CONFIRM_499_112_2_17_22_21_14_1331 "SIMAT:%d pc not been found,error!!!"
#define SIMAT_CONFIRM_640_112_2_17_22_21_14_1332 "SIMAT:SIMAT_CodePCResponse return result 0x%x out of range."
#define SIMAT_CONFIRM_773_112_2_17_22_21_14_1333 "SIMAT:SIMAT_CodePCResponse the return result 0x%x is out of range."
#define SIMAT_CONFIRM_966_112_2_17_22_21_15_1334 "SIMAT: SetupCall does not exist in the PC list, Coderesponse error and free the signal"
#define SIMAT_CONFIRM_978_112_2_17_22_21_15_1335 "SIMAT:SIMAT_TRSetupCall code call control error!"
#define SIMAT_CONFIRM_1059_112_2_17_22_21_15_1336 "SIMAT: SendSS does not exist in the PC list, Coderesponse error and free the signal"
#define SIMAT_CONFIRM_1071_112_2_17_22_21_15_1337 "SIMAT:SIMAT_TRSendSS code call control error!"
#define SIMAT_CONFIRM_1171_112_2_17_22_21_15_1338 "SIMAT: SendUSSD does not exist in the PC list, Coderesponse error and free the signal"
#define SIMAT_CONFIRM_1184_112_2_17_22_21_15_1339 "SIMAT:SIMAT_TRSendUSSD encode error!"
#define SIMAT_CONFIRM_1221_112_2_17_22_21_15_1340 "SIMAT: SendUSSD does not exist in the PC list, Coderesponse error and free the signal"
#define SIMAT_CONFIRM_1253_112_2_17_22_21_15_1341 "SIMAT:SIMAT_TRSendUSSD encode error!"
#define SIMAT_CONFIRM_1268_112_2_17_22_21_15_1342 "SIMAT:SIMAT_TRSendUSSD code call control error!"
#define SIMAT_CONFIRM_1412_112_2_17_22_21_15_1343 "SIMAT:SIMAT_TRGetInkey encode error!"
#define SIMAT_CONFIRM_1447_112_2_17_22_21_16_1344 "SIMAT:the user input text string length is out of range"
#define SIMAT_CONFIRM_1462_112_2_17_22_21_16_1345 "SIMAT:the input text string beyond 255, cut it!!!"
#define SIMAT_CONFIRM_1476_112_2_17_22_21_16_1346 "SIMAT:SIMAT_TRGetInput encode error!"
#define SIMAT_CONFIRM_1518_112_2_17_22_21_16_1347 "SIMAT:SIMAT_TRSelectItem encode error!"
#define SIMAT_CONFIRM_1581_112_2_17_22_21_16_1348 "SIMAT:SIMAT_TRGetLAI encode error!"
#define SIMAT_CONFIRM_1622_112_2_17_22_21_16_1349 "SIMAT:SIMAT_TRGetIMEI encode error!"
#define SIMAT_CONFIRM_1680_112_2_17_22_21_16_1350 "SIMAT:SIMAT_TRGetNMRBA LIST encode error!"
#define SIMAT_CONFIRM_1721_112_2_17_22_21_16_1351 "SIMAT:SIMAT_TRGetDTTZ encode error!"
#define SIMAT_CONFIRM_1762_112_2_17_22_21_16_1352 "SIMAT:SIMAT_TRGetLang encode error!"
#define SIMAT_CONFIRM_1804_112_2_17_22_21_16_1353 "SIMAT:SIMAT_TRGetTimingAD encode error!"
#define SIMAT_CONFIRM_1919_112_2_17_22_21_16_1354 "Code pc response error and free the signal"
#define SIMAT_CONFIRM_2012_112_2_17_22_21_17_1355 "SIMAT:PC list is null."
#define SIMAT_CONFIRM_2097_112_2_17_22_21_17_1356 "SIMAT:SIMAT_TROpenChannel encode error!"
#define SIMAT_CONFIRM_2137_112_2_17_22_21_17_1357 "SIMAT:SIMAT_TROpenChannel encode error!"
#define SIMAT_CONFIRM_2208_112_2_17_22_21_17_1358 "SIMAT:SIMAT_TRGetChannelStatus encode error!"
#define SIMAT_CONFIRM_2257_112_2_17_22_21_17_1359 "SIMAT:SIMAT_TRSendData encode error!"
#define SIMAT_CONFIRM_2318_112_2_17_22_21_17_1360 "SIMAT:SIMAT_TRReceiveData encode error!"
#define SIMAT_CONFIRM_2350_112_2_17_22_21_17_1361 "SIMAT:no pc is in list!"
#define SIMAT_CONFIRM_2368_112_2_17_22_21_17_1362 "SIMAT:pc %d is not in  the list."
#define SIMAT_DO_DECODE_1033_112_2_17_22_21_20_1363 "SIMAT::SIMAT_DecodeDataObject tag error!"
#define SIMAT_DO_DECODE_1120_112_2_17_22_21_20_1364 "SIMAT:the address len longer than the maximum we support, error and cut it!!!"
#define SIMAT_DO_DECODE_1284_112_2_17_22_21_20_1365 "SIMAT:the len of the cps is out of range."
#define SIMAT_DO_DECODE_1366_112_2_17_22_21_20_1366 "SIMAT:len of the ccp out of range."
#define SIMAT_DO_DECODE_1603_112_2_17_22_21_21_1367 "SIMAT: length of duration not 2."
#define SIMAT_DO_DECODE_1776_112_2_17_22_21_21_1368 "SIMAT: length of item id is not 1."
#define SIMAT_DO_DECODE_1848_112_2_17_22_21_21_1369 "SIMAT: length of Response length is not 2."
#define SIMAT_DO_DECODE_2022_112_2_17_22_21_22_1370 "SIMAT:len of sms tpdu is out of range."
#define SIMAT_DO_DECODE_2252_112_2_17_22_21_22_1371 "SIMAT: length of tone is not 1."
#define SIMAT_DO_DECODE_2352_112_2_17_22_21_22_1372 "SIMAT: ussd string len is out of range."
#define SIMAT_DO_DECODE_2461_112_2_17_22_21_22_1373 "SIMAT: file num of sim given is error,true num is %d."
#define SIMAT_DO_DECODE_2518_112_2_17_22_21_23_1374 "SIMAT:length of LocationInfo is not 7!"
#define SIMAT_DO_DECODE_2594_112_2_17_22_21_23_1375 "SIMAT:length of IMEI is not 8!"
#define SIMAT_DO_DECODE_2660_112_2_17_22_21_23_1376 "SIMAT:length of Help request is not 0!"
#define SIMAT_DO_DECODE_2726_112_2_17_22_21_23_1377 "SIMAT:length of NMR is not 16!"
#define SIMAT_DO_DECODE_3042_112_2_17_22_21_24_1378 "SIMAT: cause len is out of range."
#define SIMAT_DO_DECODE_3098_112_2_17_22_21_24_1379 "SIMAT:length of location status is not 1."
#define SIMAT_DO_DECODE_3267_112_2_17_22_21_24_1380 "SIMAT: len of the bcch list is out of range."
#define SIMAT_DO_DECODE_3344_112_2_17_22_21_24_1381 "SIMAT:DecodeCCReqAction alloc from share buffer error!"
#define SIMAT_DO_DECODE_3401_112_2_17_22_21_24_1382 "SIMAT: length of icon id is not 2."
#define SIMAT_DO_DECODE_3514_112_2_17_22_21_24_1383 "SIMAT: icon list is out of range."
#define SIMAT_DO_DECODE_3695_112_2_17_22_21_25_1384 "SIMAT: length of timer id is not 1."
#define SIMAT_DO_DECODE_3843_112_2_17_22_21_25_1385 "SIMAT: length of the DTandDZ is not 7."
#define SIMAT_DO_DECODE_4033_112_2_17_22_21_25_1386 "SIMAT: length of the ImmediateResponse is not 0."
#define SIMAT_DO_DECODE_4173_112_2_17_22_21_26_1387 "SIMAT: length of the Language is not 2."
#define SIMAT_DO_DECODE_4244_112_2_17_22_21_26_1388 "SIMAT: length of the Time Advance is not 2."
#define SIMAT_DO_DECODE_4460_112_2_17_22_21_26_1389 "SIMAT: DecodeChannelData content_len is zero!!! "
#define SIMAT_DO_DECODE_4521_112_2_17_22_21_26_1390 "SIMAT: length of channel data len  is not 1."
#define SIMAT_DO_DECODE_4580_112_2_17_22_21_27_1391 "SIMAT: length of browser id  is not 1."
#define SIMAT_DO_DECODE_4637_112_2_17_22_21_27_1392 "SIMAT: length of URL overflow."
#define SIMAT_DO_DECODE_4694_112_2_17_22_21_27_1393 "SIMAT: length of bear overflow."
#define SIMAT_DO_DECODE_4751_112_2_17_22_21_27_1394 "SIMAT: length of file path overflow."
#define SIMAT_DO_DECODE_4810_112_2_17_22_21_27_1395 "SIMAT: DecodeBrowserTerminationCause content_len is error!!! "
#define SIMAT_DO_DECODE_4863_112_2_17_22_21_27_1396 "SIMAT: DecodeChannelStatus content_len is erro!!! "
#define SIMAT_DO_DECODE_4968_112_2_17_22_21_27_1397 "SIMAT: length of bear overflow."
#define SIMAT_DO_DECODE_4976_112_2_17_22_21_27_1398 "SIMAT: DecodeBearDesc content_len is zero!!! "
#define SIMAT_DO_DECODE_5032_112_2_17_22_21_27_1399 "SIMAT: length of bear overflow."
#define SIMAT_DO_DECODE_5139_112_2_17_22_21_28_1400 "SIMAT: length of net access name overflow."
#define SIMAT_DO_DECODE_5147_112_2_17_22_21_28_1401 "SIMAT: DecodeNetAccessName content_len is zero!!! "
#define SIMAT_DO_DECODE_5204_112_2_17_22_21_28_1402 "SIMAT: length of bear overflow."
#define SIMAT_DO_DECODE_5212_112_2_17_22_21_28_1403 "SIMAT:DecodeOtherAddress content_len is zero!!! "
#define SIMAT_DO_DECODE_5268_112_2_17_22_21_28_1404 "SIMAT: length of trans level overflow."
#define SIMAT_DO_DECODE_5287_112_2_17_22_21_28_1405 "SIMAT:DecodeTransLevel port= %x"
#define SIMAT_DO_ENCODE_921_112_2_17_22_21_30_1406 "SIMAT address len longer than maximum, error and cut it!!!"
#define SIMAT_DO_ENCODE_1016_112_2_17_22_21_30_1407 "SIMAT: len of the cps is out of range in encode."
#define SIMAT_DO_ENCODE_1076_112_2_17_22_21_30_1408 "SIMAT: len of the ccp is out of range in encode."
#define SIMAT_DO_ENCODE_1125_112_2_17_22_21_31_1409 "SIMAT: len of the cb page is out of range in encode."
#define SIMAT_DO_ENCODE_1490_112_2_17_22_21_31_1410 "SIMAT: len of the sms tpdu is out of range in encode."
#define SIMAT_DO_ENCODE_1681_112_2_17_22_21_32_1411 "SIMAT: ussd string len is out of range in encode."
#define SIMAT_DO_ENCODE_2147_112_2_17_22_21_33_1412 "SIMAT: cause len is out of range in encode."
#define SIMAT_DO_ENCODE_2280_112_2_17_22_21_33_1413 "SIMAT: len of the bcch list is out of range in encode."
#define SIMAT_DO_ENCODE_2664_112_2_17_22_21_34_1414 "SIMAT:EncodeTimerValue,hour %d is out of range."
#define SIMAT_DO_ENCODE_2738_112_2_17_22_21_34_1415 "SIMAT:time_zone = %d."
#define SIMAT_DO_ENCODE_2749_112_2_17_22_21_34_1416 "SIMAT:time_zone encode value is 0x%x."
#define SIMAT_ENVELOPE_520_112_2_17_22_21_36_1417 "SIMAT:mnc digit num is %d,error."
#define SIMAT_ENVELOPE_538_112_2_17_22_21_36_1418 "SIMAT: SIMAT_MOCallControl data_type = %d"
#define SIMAT_ENVELOPE_1739_112_2_17_22_21_39_1419 "SIMAT:SIMAT_ProcessEnvelopeCnf recv unknown signal:0x%x."
#define SIMAT_ENVELOPE_1764_112_2_17_22_21_39_1420 "SIMAT:sim toolkit busy when timer expiration event."
#define SIMAT_ENVELOPE_1771_112_2_17_22_21_39_1421 "SIMAT:timer expiration event download failed for stk busy."
#define SIMAT_ENVELOPE_1776_112_2_17_22_21_39_1422 "SIMAT: add timer expiration envelope signal to not busy envelope request list."
#define SIMAT_ENVELOPE_1818_112_2_17_22_21_39_1423 "SIMAT:sim toolkit busy when language select event."
#define SIMAT_ENVELOPE_1869_112_2_17_22_21_39_1424 "SIMAT:sim toolkit busy when idle screen available event."
#define SIMAT_ENVELOPE_1920_112_2_17_22_21_39_1425 "SIMAT:sim toolkit busy when user activity event."
#define SIMAT_ENVELOPE_1969_112_2_17_22_21_39_1426 "SIMAT:sim toolkit busy when location status event."
#define SIMAT_ENVELOPE_1975_112_2_17_22_21_39_1427 "SIMAT:location status event download failed for stk busy."
#define SIMAT_ENVELOPE_2016_112_2_17_22_21_40_1428 "SIMAT:sim toolkit busy when call disconnect event."
#define SIMAT_ENVELOPE_2022_112_2_17_22_21_40_1429 "SIMAT:call disconnect event download failed for stk busy."
#define SIMAT_ENVELOPE_2063_112_2_17_22_21_40_1430 "SIMAT:sim toolkit busy when call connect event."
#define SIMAT_ENVELOPE_2069_112_2_17_22_21_40_1431 "SIMAT:call connect event download failed for stk busy."
#define SIMAT_ENVELOPE_2109_112_2_17_22_21_40_1432 "SIMAT:sim toolkit busy when MT call event."
#define SIMAT_ENVELOPE_2115_112_2_17_22_21_40_1433 "SIMAT:MT call event download failed for stk busy."
#define SIMAT_ENVELOPE_2164_112_2_17_22_21_40_1434 "SIMAT:sim toolkit busy when MO SM call control."
#define SIMAT_ENVELOPE_2203_112_2_17_22_21_40_1435 "SIMAT:MOSMControlCnf envelope result error!"
#define SIMAT_ENVELOPE_2262_112_2_17_22_21_40_1436 "SIMAT:MOSMControlCnf data_len error!"
#define SIMAT_ENVELOPE_2326_112_2_17_22_21_40_1437 "SIMAT:DecodeControlCnfData control result error!"
#define SIMAT_ENVELOPE_2470_112_2_17_22_21_40_1438 "SIMAT:DecodeControlCnfData data_len error!"
#define SIMAT_ENVELOPE_2505_112_2_17_22_21_41_1439 "SIMAT:sim toolkit busy when MO call control."
#define SIMAT_ENVELOPE_2526_112_2_17_22_21_41_1440 "SIMAT:call control evelope failed for unknown reason."
#define SIMAT_ENVELOPE_2555_112_2_17_22_21_41_1441 "SIMAT:Envelope confirm, the result is sim card not allowed, busy or error!"
#define SIMAT_ENVELOPE_2572_112_2_17_22_21_41_1442 "SIMAT:send ussd result %d"
#define SIMAT_ENVELOPE_2582_112_2_17_22_21_41_1443 "SIMAT:send ussd result %d"
#define SIMAT_ENVELOPE_2591_112_2_17_22_21_41_1444 "SIMAT:send originate ussd success"
#define SIMAT_ENVELOPE_2603_112_2_17_22_21_41_1445 "SIMAT:Call control from proactive command!"
#define SIMAT_ENVELOPE_2608_112_2_17_22_21_41_1446 "SIMAT:USSD Control result is SIMAT_SIMC_NOT_ALLOWED!"
#define SIMAT_ENVELOPE_2615_112_2_17_22_21_41_1447 "SIMAT:USSD Control result is %d!"
#define SIMAT_ENVELOPE_2622_112_2_17_22_21_41_1448 "SIMAT:USSD Control result is %d!"
#define SIMAT_ENVELOPE_2644_112_2_17_22_21_41_1449 "SIMAT:Error!,call contol, but the control type invalid!"
#define SIMAT_ENVELOPE_2692_112_2_17_22_21_41_1450 "SIMAT:SMSPPDownloadCnf ok with response data length error"
#define SIMAT_ENVELOPE_2707_112_2_17_22_21_41_1451 "SIMAT:SMSPPDownloadCnf error with response data length error"
#define SIMAT_ENVELOPE_2753_112_2_17_22_21_41_1452 "SIMAT:sim toolkit busy when CB download."
#define SIMAT_ENVELOPE_2797_112_2_17_22_21_41_1453 "SIMAT:menu selection failed for stk busy."
#define SIMAT_ENVELOPE_2867_112_2_17_22_21_41_1454 "SIMAT:the input mnc len is error,the len is 0x%x."
#define SIMAT_ENVELOPE_2932_112_2_17_22_21_41_1455 "SIMAT:session end,there is no other envelope command need to retry."
#define SIMAT_ENVELOPE_2961_112_2_17_22_21_41_1456 "SIMAT:session end,there is envelope command need to retry."
#define SIMAT_ENVELOPE_2966_112_2_17_22_21_41_1457 "SIMAT:there is unexpect signal put in retry list,signal code:0x%x."
#define SIMAT_ENVELOPE_2990_112_2_17_22_21_42_1458 "SIMAT:sim toolkit busy when data available."
#define SIMAT_ENVELOPE_3039_112_2_17_22_21_42_1459 "SIMAT:sim toolkit busy when channel status event."
#define SIMAT_ENVELOPE_3087_112_2_17_22_21_42_1460 "SIMAT:sim toolkit busy when browser termination event."
#define SIMAT_ENVELOPE_3127_112_2_17_22_21_42_1461 "SIMAT:there is PC wait terminal response,do envelope but delay fetch."
#define SIMAT_ENVELOPE_3186_112_2_17_22_21_42_1462 "SIMAT:sim toolkit busy when at cmd envelope"
#define SIMAT_ENVELOPE_3192_112_2_17_22_21_42_1463 "SIMAT:at cmd envelope failed for stk busy."
#define SIMAT_IMPLE_73_112_2_17_22_21_42_1464 "SIMAT:stk polling interval is %d"
#define SIMAT_IMPLE_99_112_2_17_22_21_42_1465 "SIMAT::start timer %d,current sys time is %d."
#define SIMAT_IMPLE_154_112_2_17_22_21_43_1466 "SIMAT:current time is %d."
#define SIMAT_IMPLE_159_112_2_17_22_21_43_1467 "SIMAT:time not wrap."
#define SIMAT_IMPLE_164_112_2_17_22_21_43_1468 "SIMAT:time wrap."
#define SIMAT_IMPLE_171_112_2_17_22_21_43_1469 "SIMAT:timer%d,run time is %d,left time_value is %d."
#define SIMAT_IMPLE_206_112_2_17_22_21_43_1470 "SIMAT:current time is %d."
#define SIMAT_IMPLE_211_112_2_17_22_21_43_1471 "SIMAT:time not wrap."
#define SIMAT_IMPLE_216_112_2_17_22_21_43_1472 "SIMAT:time wrap."
#define SIMAT_IMPLE_223_112_2_17_22_21_43_1473 "SIMAT:timer%d,run time is %d."
#define SIMAT_MAIN_114_112_2_17_22_21_43_1474 "SIMAT:receive signal 0x%x in stk poweroffstatus."
#define SIMAT_MAIN_266_112_2_17_22_21_43_1475 "SIMAT:command is poll int or off,don't send session end to up layer."
#define SIMAT_MAIN_305_112_2_17_22_21_44_1476 "SIMAT:discard signal 0x%x in stk active status."
#define SIMAT_MAIN_666_112_2_17_22_21_44_1477 "SIMAT:msg_type=%x"
#define SIMAT_PROACTIVE_720_112_2_17_22_21_46_1478 "SIMAT:SIMAT_GetPCTag PC name 0x%x is out of range."
#define SIMAT_PROACTIVE_749_112_2_17_22_21_46_1479 "SIMAT:SIMAT_GetPCName PC tag 0x%x is out of range."
#define SIMAT_PROACTIVE_1074_112_2_17_22_21_47_1480 "pc_num[%d] = %d."
#define SIMAT_PROACTIVE_1081_112_2_17_22_21_47_1481 "SIMAT::PC len error,data_len = %d,data buf(1-6):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2xn"
#define SIMAT_PROACTIVE_1090_112_2_17_22_21_47_1482 "SIMAT::ERROR data_len = %d,data buf(1-10):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2xn"
#define SIMAT_PROACTIVE_1099_112_2_17_22_21_47_1483 "SIMAT:SIMAT_ProcessProactiveCommand get tlv length error"
#define SIMAT_PROACTIVE_1111_112_2_17_22_21_47_1484 "SIMAT:the valid pc command len is modify to 0x%x"
#define SIMAT_PROACTIVE_1175_112_2_17_22_21_47_1485 "SIMAT:send session end to up layer."
#define SIMAT_PROACTIVE_3113_112_2_17_22_21_52_1486 "SIMAT:min_len is reduced to 0x%x byte!"
#define SIMAT_PROACTIVE_3120_112_2_17_22_21_52_1487 "SIMAT:max_len is reduced to 0x%x byte!"
#define SIMAT_PROACTIVE_3133_112_2_17_22_21_52_1488 "SIMAT:min_len is reduced to 0x%x byte!"
#define SIMAT_PROACTIVE_3141_112_2_17_22_21_52_1489 "SIMAT:max_len is reduced to 0x%x byte!"
#define SIMAT_PROACTIVE_3151_112_2_17_22_21_52_1490 "SIMAT:min_len is reduced to 0x%x byte!"
#define SIMAT_PROACTIVE_3158_112_2_17_22_21_52_1491 "SIMAT:max_len is reduced to 0x%x byte!"
#define SIMAT_PROACTIVE_3698_112_2_17_22_21_53_1492 "SIMAT:the pc data of setup menu is not correct,but the MS will ignore it."
#define SIMAT_PROACTIVE_3720_112_2_17_22_21_53_1493 "SIMAT:has received set up menu proactive command."
#define SIMAT_PROACTIVE_3922_112_2_17_22_21_53_1494 "SIMAT:In traffic mode,send NMR to SIM is not surpported temporarily"
#define SIMAT_PROACTIVE_3926_112_2_17_22_21_53_1495 "SIMAT:ba_ind = %d, rxlev-full-serving-cell = %d"
#define SIMAT_PROACTIVE_3973_112_2_17_22_21_53_1496 "SIMAT:is_me_idle = %d, timing advance value = %d"
#define SIMAT_PROACTIVE_4647_112_2_17_22_21_55_1497 "SIMAT:LaunchBrowser,the device id is error!Ignore it."
#define SIMAT_PROACTIVE_4879_112_2_17_22_21_55_1498 "SIMAT: OpenChannel!"
#define SIMAT_PROACTIVE_5038_112_2_17_22_21_56_1499 "SIMAT:there are too more text string in open channel,ignore it."
#define SIMAT_PROACTIVE_5068_112_2_17_22_21_56_1500 "SIMAT:[OpenChannel]::open_channel.trans_level type=%d, port=%x "
#define SIMAT_PROACTIVE_5081_112_2_17_22_21_56_1501 "SIMAT: support GPRS bear only!!!"
#define SIMAT_PROACTIVE_5098_112_2_17_22_21_56_1502 "SIMAT:leave open channel"
#define SIMAT_PROACTIVE_5152_112_2_17_22_21_56_1503 "SIMAT: CloseChannel!"
#define SIMAT_PROACTIVE_5292_112_2_17_22_21_56_1504 "SIMAT: ReceiveData!"
#define SIMAT_PROACTIVE_5454_112_2_17_22_21_56_1505 "SIMAT: SendData!"
#define SIMAT_PROACTIVE_5620_112_2_17_22_21_57_1506 "SIMAT: GetChannelStatus!"
#define SIMAT_PROACTIVE_5804_112_2_17_22_21_57_1507 "SIMAT_ForwardProactiveCommand"
#define SIMAT_PROACTIVE_5824_112_2_17_22_21_57_1508 "data_len = %d,data[0] = 0x%x,data[1]=0x%x."
#define SIMATDUM_COM_1038_112_2_17_22_22_0_1509 "SIMATDUM::read stk test case file ok."
#define SIMATDUM_COM_1086_112_2_17_22_22_0_1510 "SIMATDUM::write stk test case data file ok."
#define SIMATDUM_COM_DECODE_1476_112_2_17_22_22_3_1511 "SIMAT::unknown tag in the launch browser."
#define SIMATDUM_PROACTIVE_2230_112_2_17_22_22_16_1512 "SIMATDUM:Unknow Envelope type from simat!"
#define SIMATDUM_PROACTIVE_2341_112_2_17_22_22_17_1513 "SIMATDUM:Unknow or unsupported proactive command terminal response!"
#define SIMATDUM_PROACTIVE_2675_112_2_17_22_22_17_1514 "SIMATDUM:Error Menu selection envelope!"
#define SIMATDUM_PROACTIVE_2687_112_2_17_22_22_17_1515 "SIMATDUM:Error menu selection envelope encode!"
#define SIMATDUM_PROACTIVE_2696_112_2_17_22_22_17_1516 "SIMATDUM:Error device id data object encode in menu selection envelope!"
#define SIMATDUM_PROACTIVE_2706_112_2_17_22_22_17_1517 "SIMATDUM:Error device id data object in menu selection envelope!"
#define SIMATDUM_PROACTIVE_2715_112_2_17_22_22_17_1518 "SIMATDUM:Error Item Id data object encode in menu selection envelope!"
#define SIMATDUM_PROACTIVE_2730_112_2_17_22_22_17_1519 "SIMATDUM:Error help info data object in menu selection envelope!"
#define SIMATDUM_PROACTIVE_2745_112_2_17_22_22_17_1520 "SIMATDUM:Error Item id in menu selection envelope!"
#define SIMATDUM_PROACTIVE_2808_112_2_17_22_22_18_1521 "SIMATDUM:Error SMS pp download envelope!"
#define SIMATDUM_PROACTIVE_2820_112_2_17_22_22_18_1522 "SIMATDUM:Error SMS pp download envelope encode!"
#define SIMATDUM_PROACTIVE_2829_112_2_17_22_22_18_1523 "SIMATDUM:Error device id data object decode in SMS pp download envelope!"
#define SIMATDUM_PROACTIVE_2839_112_2_17_22_22_18_1524 "SIMATDUM:Error device id data object in SMS pp download envelope!"
#define SIMATDUM_PROACTIVE_2850_112_2_17_22_22_18_1525 "SIMATDUM:Error address info data object in sms pp download envelope!"
#define SIMATDUM_PROACTIVE_2862_112_2_17_22_22_18_1526 "SIMATDUM:Error sms tpdu data object decode in SMS pp download envelope!"
#define SIMATDUM_PROACTIVE_2903_112_2_17_22_22_18_1527 "SIMATDUM:Error CB download envelope!"
#define SIMATDUM_PROACTIVE_2915_112_2_17_22_22_18_1528 "SIMATDUM:Error CB download envelope encode!"
#define SIMATDUM_PROACTIVE_2924_112_2_17_22_22_18_1529 "SIMATDUM:Error device id data object decode in CB download envelope!"
#define SIMATDUM_PROACTIVE_2934_112_2_17_22_22_18_1530 "SIMATDUM:Error device id data object in CB download envelope!"
#define SIMATDUM_PROACTIVE_2943_112_2_17_22_22_18_1531 "SIMATDUM:Error cb page tpdu data object decode in cb download envelope!"
#define SIMATDUM_PROACTIVE_2991_112_2_17_22_22_18_1532 "SIMATDUM:Error call control envelope!"
#define SIMATDUM_PROACTIVE_3003_112_2_17_22_22_18_1533 "SIMATDUM:Error call control envelope encode!"
#define SIMATDUM_PROACTIVE_3012_112_2_17_22_22_18_1534 "SIMATDUM:Error device id data object encode in call control envelope!"
#define SIMATDUM_PROACTIVE_3022_112_2_17_22_22_18_1535 "SIMATDUM:Error device id data object in call control selection envelope!"
#define SIMATDUM_PROACTIVE_3033_112_2_17_22_22_18_1536 "SIMATDUM:Error address data object encode in call control envelope!"
#define SIMATDUM_PROACTIVE_3046_112_2_17_22_22_18_1537 "SIMATDUM:Error ss string data object encode in call control envelope!"
#define SIMATDUM_PROACTIVE_3059_112_2_17_22_22_18_1538 "SIMATDUM:Error ussd string data object encode in call control envelope!"
#define SIMATDUM_PROACTIVE_3069_112_2_17_22_22_18_1539 "SIMATDUM:The mandatory address data object is not exist in call control envelope!"
#define SIMATDUM_PROACTIVE_3080_112_2_17_22_22_18_1540 "SIMATDUM:Error ccp1 data object in call control envelope!"
#define SIMATDUM_PROACTIVE_3095_112_2_17_22_22_18_1541 "SIMATDUM:Error call party subaddress data object in call control envelope!"
#define SIMATDUM_PROACTIVE_3110_112_2_17_22_22_18_1542 "SIMATDUM:Error location information data object in call control envelope!"
#define SIMATDUM_PROACTIVE_3119_112_2_17_22_22_18_1543 "SIMATDUM:The mandatory location information data object is not exist in call control envelope!"
#define SIMATDUM_PROACTIVE_3130_112_2_17_22_22_18_1544 "SIMATDUM:Error ccp2 data object in call control envelope!"
#define SIMATDUM_PROACTIVE_3201_112_2_17_22_22_18_1545 "SIMATDUM:Error mo sms control envelope!"
#define SIMATDUM_PROACTIVE_3213_112_2_17_22_22_18_1546 "SIMATDUM:Error mo sms control envelope encode!"
#define SIMATDUM_PROACTIVE_3222_112_2_17_22_22_18_1547 "SIMATDUM:Error device id data object encode in mo sms control envelope!"
#define SIMATDUM_PROACTIVE_3232_112_2_17_22_22_18_1548 "SIMATDUM:Error device id data object in mo sms control envelope!"
#define SIMATDUM_PROACTIVE_3243_112_2_17_22_22_18_1549 "SIMATDUM:Error address data object encode in mo sms control envelope!"
#define SIMATDUM_PROACTIVE_3252_112_2_17_22_22_18_1550 "SIMATDUM:The mandatory address data object is not exist in mo sms control envelope!"
#define SIMATDUM_PROACTIVE_3263_112_2_17_22_22_18_1551 "SIMATDUM:Error address data object encode in mo sms control envelope!"
#define SIMATDUM_PROACTIVE_3272_112_2_17_22_22_19_1552 "SIMATDUM:The mandatory address data object is not exist in mo sms control envelope!"
#define SIMATDUM_PROACTIVE_3283_112_2_17_22_22_19_1553 "SIMATDUM:Error location information data object in mo sms control envelope!"
#define SIMATDUM_PROACTIVE_3292_112_2_17_22_22_19_1554 "SIMATDUM:The mandatory location information data object is not exist in mo sms control envelope!"
#define SIMATDUM_PROACTIVE_3355_112_2_17_22_22_19_1555 "SIMATDUM:Error Event download envelope!"
#define SIMATDUM_PROACTIVE_3367_112_2_17_22_22_19_1556 "SIMATDUM:Error event download encode!"
#define SIMATDUM_PROACTIVE_3376_112_2_17_22_22_19_1557 "SIMATDUM:Error event list data object encode in event download envelope!"
#define SIMATDUM_PROACTIVE_3387_112_2_17_22_22_19_1558 "SIMATDUM:Error event list data object in event download envelope!"
#define SIMATDUM_PROACTIVE_3396_112_2_17_22_22_19_1559 "SIMATDUM:Error device id data object encode in event download envelope!"
#define SIMATDUM_PROACTIVE_3410_112_2_17_22_22_19_1560 "SIMATDUM:Error device id data object in event download envelope!"
#define SIMATDUM_PROACTIVE_3420_112_2_17_22_22_19_1561 "SIMATDUM:Error device id data object in event download envelope!"
#define SIMATDUM_PROACTIVE_3430_112_2_17_22_22_19_1562 "SIMATDUM:Error device id data object in event download envelope!"
#define SIMATDUM_PROACTIVE_3444_112_2_17_22_22_19_1563 "SIMATDUM:Error transcation Id data object decode in event download envelope!"
#define SIMATDUM_PROACTIVE_3459_112_2_17_22_22_19_1564 "SIMATDUM:Error address data object decode in event download envelope!"
#define SIMATDUM_PROACTIVE_3473_112_2_17_22_22_19_1565 "SIMATDUM:Error call sub address data object decode in event download envelope!"
#define SIMATDUM_PROACTIVE_3489_112_2_17_22_22_19_1566 "SIMATDUM:Error cause data object decode in event download envelope!"
#define SIMATDUM_PROACTIVE_3506_112_2_17_22_22_19_1567 "SIMATDUM:Error location status data object in event download envelope!"
#define SIMATDUM_PROACTIVE_3519_112_2_17_22_22_19_1568 "SIMATDUM:Error location information data object decode in event download envelope!"
#define SIMATDUM_PROACTIVE_3532_112_2_17_22_22_19_1569 "SIMATDUM:Error language data object in event download envelope!"
#define SIMATDUM_PROACTIVE_3545_112_2_17_22_22_19_1570 "SIMATDUM:Error browser terminate cause data object decode in event download envelope!"
#define SIMATDUM_PROACTIVE_3555_112_2_17_22_22_19_1571 "SIMATDUM:No data object need to decode in event download envelope!"
#define SIMATDUM_PROACTIVE_3894_112_2_17_22_22_20_1572 "SIMATDUM:Error timer expiration envelope!"
#define SIMATDUM_PROACTIVE_3906_112_2_17_22_22_20_1573 "SIMATDUM:Error timer expiration envelope encode!"
#define SIMATDUM_PROACTIVE_3915_112_2_17_22_22_20_1574 "SIMATDUM:Error device id data object encode in timer expiration envelope!"
#define SIMATDUM_PROACTIVE_3925_112_2_17_22_22_20_1575 "SIMATDUM:Error device id data object in timer expiration envelope!"
#define SIMATDUM_PROACTIVE_3934_112_2_17_22_22_20_1576 "SIMATDUM:Error Timer Id data object encode in timer expiration envelope!"
#define SIMATDUM_PROACTIVE_3944_112_2_17_22_22_20_1577 "SIMATDUM:Error Timer Value data object encode in timer expiration envelope!"
#define SIMATDUM_PROACTIVE_4050_112_2_17_22_22_20_1578 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_4107_112_2_17_22_22_20_1579 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_4182_112_2_17_22_22_20_1580 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_4253_112_2_17_22_22_21_1581 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_4262_112_2_17_22_22_21_1582 "SIMATDUM:Setup Menu proactive command not successed!"
#define SIMATDUM_PROACTIVE_4311_112_2_17_22_22_21_1583 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_4334_112_2_17_22_22_21_1584 "SIMATDUM:Error Item id from the select item terminal response!"
#define SIMATDUM_PROACTIVE_4423_112_2_17_22_22_21_1585 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_4569_112_2_17_22_22_22_1586 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_4590_112_2_17_22_22_22_1587 "SIMATDUM:Error Text string length in get inkey terminal response!"
#define SIMATDUM_PROACTIVE_4669_112_2_17_22_22_22_1588 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_4756_112_2_17_22_22_22_1589 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_4770_112_2_17_22_22_22_1590 "SIMATDUM:Error in Decode Text string of terminal response!"
#define SIMATDUM_PROACTIVE_4863_112_2_17_22_22_22_1591 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_4948_112_2_17_22_22_22_1592 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_5035_112_2_17_22_22_23_1593 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_5121_112_2_17_22_22_23_1594 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_5132_112_2_17_22_22_23_1595 "SIMATDUM:Decode duration data object error in terminal response!"
#define SIMATDUM_PROACTIVE_5204_112_2_17_22_22_23_1596 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_5279_112_2_17_22_22_23_1597 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_5356_112_2_17_22_22_23_1598 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_5431_112_2_17_22_22_23_1599 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_5528_112_2_17_22_22_24_1600 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_5720_112_2_17_22_22_24_1601 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_5766_112_2_17_22_22_24_1602 "SIMATDUM:Error in Decode Text string of terminal response!"
#define SIMATDUM_PROACTIVE_5808_112_2_17_22_22_24_1603 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_5873_112_2_17_22_22_24_1604 "SIMATDUM:Error terminal reponse tag!"
#define SIMATDUM_PROACTIVE_5883_112_2_17_22_22_24_1605 "SIMATDUM:Error command detail length in terminal response!"
#define SIMATDUM_PROACTIVE_5891_112_2_17_22_22_24_1606 "SIMATDUM:Error command name in terminal response!"
#define SIMATDUM_PROACTIVE_5900_112_2_17_22_22_24_1607 "SIMATDUM:Error Device Id data object encode in terminal response!"
#define SIMATDUM_PROACTIVE_5907_112_2_17_22_22_24_1608 "SIMATDUM:Error Device Id data object in terminal response!"
#define SIMATDUM_PROACTIVE_10108_112_2_17_22_22_34_1609 "SIMAT:the valid pc command len is modify to 0x%x"
#define SIMATDUM_PROACTIVE_14749_112_2_17_22_22_44_1610 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_14828_112_2_17_22_22_44_1611 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_14843_112_2_17_22_22_44_1612 "SIMATDUM:Error in Decode Channel status of terminal response!"
#define SIMATDUM_PROACTIVE_14855_112_2_17_22_22_44_1613 "SIMATDUM:Error in Decode bear description of terminal response!"
#define SIMATDUM_PROACTIVE_14869_112_2_17_22_22_44_1614 "SIMATDUM:Error in Decode Buffer size of terminal response!"
#define SIMATDUM_PROACTIVE_14883_112_2_17_22_22_44_1615 "SIMATDUM:Error in Decode other address of terminal response!"
#define SIMATDUM_PROACTIVE_14985_112_2_17_22_22_44_1616 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_15061_112_2_17_22_22_44_1617 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_15076_112_2_17_22_22_44_1618 "SIMATDUM:Error in Decode Channel data of terminal response!"
#define SIMATDUM_PROACTIVE_15087_112_2_17_22_22_44_1619 "SIMATDUM:Error in Decode channel data len of terminal response!"
#define SIMATDUM_PROACTIVE_15159_112_2_17_22_22_45_1620 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_15174_112_2_17_22_22_45_1621 "SIMATDUM:Error in Decode channel data len of terminal response!"
#define SIMATDUM_PROACTIVE_15254_112_2_17_22_22_45_1622 "SIMATDUM:Decode Result data object error in terminal response!"
#define SIMATDUM_PROACTIVE_15272_112_2_17_22_22_45_1623 "SIMATDUM:Error in Decode Channel status of terminal response!"
#define MNPHONE_API_1317_112_2_17_22_23_8_1624 "MN: MAIN_SetPlmnSelectList validate_num=%d"
#define MNPHONE_API_2655_112_2_17_22_23_11_1625 "MNPHONE_WritePLMNtoSimEx is_delete_item = %d, result = %d, order_of_plmn=%d"
#define MNPHONE_API_3031_112_2_17_22_23_12_1626 "MNPHONE_SIMUnlockEx error_code %d"
#define MNPHONE_API_3139_112_2_17_22_23_12_1627 "mnphone: MNPHONE_SelectPlmnWithoutSim dual_sys:%d, mcc:%d, mnc:%d, mnc_digit_num:%d."
#define MNPHONE_API_3143_112_2_17_22_23_12_1628 "mnphone: Assert failure, selected card %d is more than MAX %d."
#define MNPHONE_API_3151_112_2_17_22_23_12_1629 "mnphone: Assert failure, selected plmn list length %d is more than MAX %d."
#define MNPHONE_MODULE_2177_112_2_17_22_23_18_1630 "MNPHONE%d::the location status not changed."
#define MNPHONE_MODULE_2184_112_2_17_22_23_18_1631 "MNPHONE%d:: act type is no service, so the location status not changed."
#define MNPHONE_MODULE_2413_112_2_17_22_23_19_1632 "MNPHONE: HPLMN in forbidden plmn list, delete hplmn from forbidden plmn list"
#define MNPHONE_MODULE_2424_112_2_17_22_23_19_1633 "MNPHONE: EHPLMN in forbidden plmn list, delete hplmn from forbidden plmn list"
#define MNPHONE_MODULE_2579_112_2_17_22_23_19_1634 "MNPHONE: Switch on with manual select, select last rplmn or hplmn %d, %d"
#define MNPHONE_MODULE_2618_112_2_17_22_23_19_1635 "MNPHONE: campon a plmn,mcc=%d,mnc=%d,any_plmn=%d,mnc_digit_num = %dn"
#define MNPHONE_MODULE_2763_112_2_17_22_23_20_1636 "BASE Version:%s"
#define MNPHONE_MODULE_2782_112_2_17_22_23_20_1637 "mnphone: SendNetworkStatusInd operate_mode = %d"
#define MNPHONE_MODULE_2873_112_2_17_22_23_20_1638 "mnphone: assert failure, NETWORK_MODE_E"
#define MNPHONE_MODULE_3115_112_2_17_22_23_20_1639 "MNPHONE:mnc %d,mcc %d,is_forbidden %d "
#define MNPHONE_MODULE_3717_112_2_17_22_23_22_1640 "mnphone: assert failure, MN_PHONE_PLMN_STATUS_E"
#define MNPHONE_MODULE_3793_112_2_17_22_23_22_1641 "MNPHONE: Assert Failure to read IMEI!!"
#define MNPHONE_MODULE_3827_112_2_17_22_23_22_1642 "MNPHONE: Assert Failure to read IMEISV!!"
#define MNPHONE_MODULE_3837_112_2_17_22_23_22_1643 "MNPHONE: IMEISVN is not presented!!"
#define MNPHONE_MODULE_3941_112_2_17_22_23_22_1644 "MNPHONE:SCI_CreatePeriodTimer,CheckDspIntTimer "
#define MNPHONE_MODULE_4114_112_2_17_22_23_22_1645 "mnphone: assert failure, RR_ACT_TYPE_E"
#define MNPHONE_MODULE_4154_112_2_17_22_23_22_1646 "mnphone: assert failure, RR_ACT_TYPE_E"
#define MNPHONE_MODULE_4371_112_2_17_22_23_23_1647 "MNPHONE:Camp reference is different, REQ Ref: %d, CNF Ref: %d"
#define MNPHONE_MODULE_4400_112_2_17_22_23_23_1648 "mnphone: assert failure, RR_ACT_TYPE_E"
#define MNPHONE_MODULE_4565_112_2_17_22_23_23_1649 "PHONE:InvalidSim_HandleSimChvCnf,s_mnphone_state=%d"
#define MNPHONE_MODULE_4579_112_2_17_22_23_23_1650 "MNPHONE:unblock the CHV1 successful"
#define MNPHONE_MODULE_4636_112_2_17_22_23_23_1651 "MNPHONE:ASTERISK_HandleSimChvCnf,s_mnphone_state=%d"
#define MNPHONE_MODULE_4716_112_2_17_22_23_23_1652 "MNPHONE: MnMmRegInd, read NV_RPLMN is_enable %d, mcc %d, mnc %d, mnc_digit_num %d"
#define MNPHONE_MODULE_4725_112_2_17_22_23_24_1653 "MNPHONE: MnMmRegInd, read NV_RPLMN, nv error"
#define MNPHONE_MODULE_4748_112_2_17_22_23_24_1654 "MNPHONE:ASTERISK_HandleMnMmRegInd error \n"
#define MNPHONE_MODULE_4809_112_2_17_22_23_24_1655 "MNPHONE: MnMmNregInd, read NV_RPLMN is_enable %d, mcc %d, mnc %d, mnc_digit_num %d"
#define MNPHONE_MODULE_4819_112_2_17_22_23_24_1656 "MNPHONE: MnMmNregInd, read NV_RPLMN, nv error"
#define MNPHONE_MODULE_4868_112_2_17_22_23_24_1657 "mnphone: assert failure, MN_MM_NREG_IND_CAUSE_E"
#define MNPHONE_MODULE_5146_112_2_17_22_23_24_1658 "MNPHONE:HandleGmmAttachCause, cause = %d\n"
#define MNPHONE_MODULE_5387_112_2_17_22_23_25_1659 "MNPHONE: change select plmn mode SUCCESS"
#define MNPHONE_MODULE_5407_112_2_17_22_23_25_1660 "MNPHONE: select plmn,mode_is_manual is %d"
#define MNPHONE_MODULE_5485_112_2_17_22_23_25_1661 "MNPHONE:camponed a plmn in No Cell state,mcc=%d,mnc=%d,type=%dn"
#define MNPHONE_MODULE_5511_112_2_17_22_23_25_1662 "MNPHONE: NO_SERVICE PLMN,do nothing."
#define MNPHONE_MODULE_5515_112_2_17_22_23_25_1663 "mnphone: assert failure, RR_ACT_TYPE_E"
#define MNPHONE_MODULE_5534_112_2_17_22_23_25_1664 "MNPHONE: camponed a plmn in camped on state,mcc=%d,mnc=%d,type=%d,plmn_status=%dn"
#define MNPHONE_MODULE_5555_112_2_17_22_23_25_1665 "MNPHONE: EMERGENCY_ONLY PLMN,do nothing,plmn_status=%d"
#define MNPHONE_MODULE_5567_112_2_17_22_23_25_1666 "mnphone: assert failure, RR_ACT_TYPE_E"
#define MNPHONE_MODULE_5635_112_2_17_22_23_25_1667 "mnphone: assert failure, RR_ACT_TYPE_E"
#define MNPHONE_MODULE_5693_112_2_17_22_23_25_1668 "MNPHONE: old band is %d"
#define MNPHONE_MODULE_5705_112_2_17_22_23_25_1669 "MNPHONE: param_ptr->reselect_param.band = %d"
#define MNPHONE_MODULE_5748_112_2_17_22_23_26_1670 "mnphone: assert failure, PLMN_RESELECT_CAUSE_E"
#define MNPHONE_MODULE_5777_112_2_17_22_23_26_1671 "mnphone: assert failure, PLMN_RESELECT_CAUSE_E"
#define MNPHONE_MODULE_5964_112_2_17_22_23_26_1672 "MNPHONE:HandleGmmDetachCause cause = %d\n"
#define MNPHONE_MODULE_6003_112_2_17_22_23_26_1673 "MNPHONE: reset ps module"
#define MNPHONE_MODULE_6439_112_2_17_22_23_27_1674 "enter Phone_IsSctTchLoopBackOn,s_is_loop_back_on[dual_sys]=%d"
#define MNPHONE_MODULE_6474_112_2_17_22_23_27_1675 "init SMS"
#define MNPHONE_MODULE_6941_112_2_17_22_23_28_1676 "MNPHONE: HPLMN in forbidden plmn list, delete hplmn from forbidden plmn list"
#define MNPHONE_MODULE_6954_112_2_17_22_23_28_1677 "MNPHONE: EHPLMN in forbidden plmn list, delete hplmn from forbidden plmn list"
#define MNPHONE_MODULE_6981_112_2_17_22_23_28_1678 "MNSMS: SIM phase is phase 1, the last used TP_MR is saved in NV."
#define MNPHONE_MODULE_7101_112_2_17_22_23_28_1679 "MNPHONE: deactive start select plmn."
#define MNPHONE_MODULE_7152_112_2_17_22_23_28_1680 "SCI_CreatePeriodTimer,CheckDspIntTimer "
#define MNPHONE_MODULE_7240_112_2_17_22_23_28_1681 "MNPHONE: s_is_ps_activate[dual_sys] = TRUE"
#define MNPHONE_MODULE_7574_112_2_17_22_23_29_1682 "PHONE:Deactive_HandleSimChvCnf,s_mnphone_state[dual_sys]=%d"
#define MNPHONE_MODULE_7587_112_2_17_22_23_29_1683 "MNPHONE:unblock the CHV1 successful when deactive "
#define MNPHONE_MODULE_7630_112_2_17_22_23_29_1684 "PHONE:Deactive_HandleSimChvCnf,s_mnphone_state[dual_sys]=%d"
#define MNPHONE_MODULE_7741_112_2_17_22_23_29_1685 "MNPHONE:StkRefreshIniCnf be called,result=%d!\n"
#define MNPHONE_MODULE_7763_112_2_17_22_23_30_1686 "MNPHONE::fdn changed,is_fdn_enable = %d."
#define MNPHONE_MODULE_7785_112_2_17_22_23_30_1687 "MNPHONE::fdn not change,is_fdn_enable = %d."
#define MNPHONE_MODULE_7810_112_2_17_22_23_30_1688 "MNPHONE:StkRefreshFileChgCnf be called,result=%d!\n"
#define MNPHONE_MODULE_7857_112_2_17_22_23_30_1689 "MNPHONE: EHPLMN(%d %d) in forbidden plmn list, delete"
#define MNPHONE_MODULE_7870_112_2_17_22_23_30_1690 "MNPHONE: HPLMN(%d, %d) in forbidden plmn list, delete"
#define MNPHONE_MODULE_7902_112_2_17_22_23_30_1691 "MNPHONE:StkRefreshIniFileChgCnf be called,result=%d!\n"
#define MNPHONE_MODULE_7909_112_2_17_22_23_30_1692 "StkRefreshIniFileChgCnf::fdn changed,is_fdn_enable = %d."
#define MNPHONE_MODULE_7918_112_2_17_22_23_30_1693 "StkRefreshIniFileChgCnf::fdn not changed,is_fdn_enable = %d."
#define MNPHONE_MODULE_7981_112_2_17_22_23_30_1694 "MNPHONE:StkRefreshIniFullCnf be called,result=%d!\n"
#define MNPHONE_MODULE_7989_112_2_17_22_23_30_1695 "StkRefreshIniFullCnf::fdn changed,is_fdn_enable = %d."
#define MNPHONE_MODULE_7998_112_2_17_22_23_30_1696 "StkRefreshIniFullCnf::fdn not changed,is_fdn_enable = %d."
#define MNPHONE_MODULE_8004_112_2_17_22_23_30_1697 "MNPHONE: imsi changed!"
#define MNPHONE_MODULE_8195_112_2_17_22_23_31_1698 "MNPHONE: the file %d will not update in MMI."
#define MNPHONE_MODULE_8281_112_2_17_22_23_31_1699 "MNPHONE: read sim redefine file list file 0x%x.!"
#define MNPHONE_MODULE_8285_112_2_17_22_23_31_1700 "MNPHONE: read sim redefine file list finished."
#define MNPHONE_MODULE_8304_112_2_17_22_23_31_1701 "MNPHONE: refresh read sim file finished."
#define MNPHONE_MODULE_8309_112_2_17_22_23_31_1702 "MNPHONE: read sim refresh table file 0x%x.!"
#define MNPHONE_MODULE_8314_112_2_17_22_23_31_1703 "MNPHONE: SIM file initialization finished!"
#define MNPHONE_MODULE_8321_112_2_17_22_23_31_1704 "MNPHONE: read sim ini table 0x%x.!"
#define MNPHONE_MODULE_8447_112_2_17_22_23_31_1705 "MNPHONE: SIM file read finished!"
#define MNPHONE_MODULE_8729_112_2_17_22_23_32_1706 "MNPHONE:spn_len =%d,spn = %x, %x, %x"
#define MNPHONE_MODULE_8739_112_2_17_22_23_32_1707 "MNPHONE:spn_len is 0,so do not support spn."
#define MNPHONE_MODULE_8823_112_2_17_22_23_33_1708 "MNPHONE: is_opn_need = %d,is_opn_exist = %d"
#define MNPHONE_MODULE_8854_112_2_17_22_23_33_1709 "MNPHONE:spn_supported = %d,plmn_is_roaming= %d."
#define MNPHONE_MODULE_9217_112_2_17_22_23_34_1710 "MNPHONE::is summer time."
#define MNPHONE_MODULE_9224_112_2_17_22_23_34_1711 "MNPHONE::is winter time."
#define MNPHONE_MODULE_9620_112_2_17_22_23_35_1712 "MNPHONE::HandleMePersonalisationVerify Result: %d"
#define MNPHONE_SIGNAL_233_112_2_17_22_23_35_1713 "MN: get paging_tbf_mask from Classmark : 0x%x"
#define MNPHONE_SIMLOCK_287_112_2_17_22_23_37_1714 "MNPHONE:WriteSIMLockCustomizeData encrypt fail %d"
#define MNPHONE_SIMLOCK_354_112_2_17_22_23_37_1715 "MNPHONE:WriteSIMLockUserData encrypt fail %d"
#define MNPHONE_SIMLOCK_404_112_2_17_22_23_38_1716 "MNPHONE:SIMLockNetworkCheck N_Pointer para"
#define MNPHONE_SIMLOCK_414_112_2_17_22_23_38_1717 "MNPHONE:SIMLockNetworkCheck no code exist"
#define MNPHONE_SIMLOCK_421_112_2_17_22_23_38_1718 "MNPHONE:SIMLockNetworkCheck num err=%d"
#define MNPHONE_SIMLOCK_453_112_2_17_22_23_38_1719 "MNPHONE:SIMLockNetworkSubsetCheck N_Pointer para"
#define MNPHONE_SIMLOCK_463_112_2_17_22_23_38_1720 "MNPHONE:SIMLockNetworkSubsetCheck no code exist"
#define MNPHONE_SIMLOCK_470_112_2_17_22_23_38_1721 "MNPHONE:SIMLockNetworkSubsetCheck num err=%d"
#define MNPHONE_SIMLOCK_505_112_2_17_22_23_38_1722 "MNPHONE:SIMLockSPCheck N_Pointer para"
#define MNPHONE_SIMLOCK_515_112_2_17_22_23_38_1723 "MNPHONE:SIMLockSPCheck no code exist"
#define MNPHONE_SIMLOCK_522_112_2_17_22_23_38_1724 "MNPHONE:SIMLockSPCheck num err=%d"
#define MNPHONE_SIMLOCK_557_112_2_17_22_23_38_1725 "MNPHONE:SIMLockCorporateCheck N_Pointer para"
#define MNPHONE_SIMLOCK_567_112_2_17_22_23_38_1726 "MNPHONE:SIMLockCorporateCheck no code exist"
#define MNPHONE_SIMLOCK_574_112_2_17_22_23_38_1727 "MNPHONE:SIMLockCorporateCheck num err=%d"
#define MNPHONE_SIMLOCK_608_112_2_17_22_23_38_1728 "MNPHONE:SIMLockUserCheck N_Pointer para"
#define MNPHONE_SIMLOCK_618_112_2_17_22_23_38_1729 "MNPHONE:SIMLockUserCheck no code exist"
#define MNPHONE_SIMLOCK_625_112_2_17_22_23_38_1730 "MNPHONE:SIMLockUserCheck num err=%d"
#define MNPHONE_SIMLOCK_704_112_2_17_22_23_38_1731 "MNPHONE:CustomizeDataIntegrity CRC: %d %d %d %d"
#define MNPHONE_SIMLOCK_711_112_2_17_22_23_38_1732 "MNPHONE:CustomizeDataIntegrity encrypt fail %d"
#define MNPHONE_SIMLOCK_746_112_2_17_22_23_38_1733 "MNPHONE:UserDataIntegrityCheck CRC: %d %d %d %d"
#define MNPHONE_SIMLOCK_753_112_2_17_22_23_38_1734 "MNPHONE:UserDataIntegrityCheck encrypt fail %d"
#define MNPHONE_SIMLOCK_789_112_2_17_22_23_38_1735 "MNPHONE:GetUnlockKey type err=%d"
#define MNPHONE_SIMLOCK_797_112_2_17_22_23_38_1736 "MNPHONE:GetUnlockKey IntegrityCheck=%d"
#define MNPHONE_SIMLOCK_834_112_2_17_22_23_38_1737 "MNPHONE:GetUnlockKey type err=%d"
#define MNPHONE_SIMLOCK_842_112_2_17_22_23_38_1738 "MNPHONE:GetUnlockKey IntegrityCheck=%d"
#define MNPHONE_SIMLOCK_880_112_2_17_22_23_38_1739 "MNPHONE:PHONE_IsKeySame key2 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d"
#define MNPHONE_SIMLOCK_884_112_2_17_22_23_39_1740 "MNPHONE:PHONE_IsKeySame encrypt fail %d"
#define MNPHONE_SIMLOCK_928_112_2_17_22_23_39_1741 "MNPHONE:SIMLockFirstBoot no customize lock"
#define MNPHONE_SIMLOCK_937_112_2_17_22_23_39_1742 "MNPHONE:SIMLockFirstBoot orange lock"
#define MNPHONE_SIMLOCK_998_112_2_17_22_23_39_1743 "MNPHONE:SIMLockFirstBoot SP lock fail, invalid GID"
#define MNPHONE_SIMLOCK_1024_112_2_17_22_23_39_1744 "MNPHONE:SIMLockFirstBoot CORPORATE lock fail, invalid GID"
#define MNPHONE_SIMLOCK_1075_112_2_17_22_23_39_1745 "MNPHONE:SIMLockFirstBoot lock_status %d, updateNV=%d, c_NV=%d, u_NV=%d"
#define MNPHONE_SIMLOCK_1095_112_2_17_22_23_39_1746 "MNPHONE:SIMLockDataReadAndIntegrityCheck control data read NV fail"
#define MNPHONE_SIMLOCK_1110_112_2_17_22_23_39_1747 "MNPHONE:For debug, IMEI: %d %d %d %d %d %d %d %d"
#define MNPHONE_SIMLOCK_1115_112_2_17_22_23_39_1748 "MNPHONE:For debug, Rand: %d %d %d %d"
#define MNPHONE_SIMLOCK_1133_112_2_17_22_23_39_1749 "MNPHONE:For debug, nck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"
#define MNPHONE_SIMLOCK_1151_112_2_17_22_23_39_1750 "MNPHONE:For debug, nsck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"
#define MNPHONE_SIMLOCK_1169_112_2_17_22_23_39_1751 "MNPHONE:For debug, spck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"
#define MNPHONE_SIMLOCK_1187_112_2_17_22_23_39_1752 "MNPHONE:For debug, cck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"
#define MNPHONE_SIMLOCK_1205_112_2_17_22_23_39_1753 "MNPHONE:For debug, pck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"
#define MNPHONE_SIMLOCK_1223_112_2_17_22_23_39_1754 "MNPHONE:For debug, un_nck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"
#define MNPHONE_SIMLOCK_1241_112_2_17_22_23_39_1755 "MNPHONE:For debug, un_nsck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"
#define MNPHONE_SIMLOCK_1259_112_2_17_22_23_39_1756 "MNPHONE:For debug, un_spck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"
#define MNPHONE_SIMLOCK_1277_112_2_17_22_23_39_1757 "MNPHONE:For debug, un_cck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"
#define MNPHONE_SIMLOCK_1295_112_2_17_22_23_39_1758 "MNPHONE:For debug, un_pck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"
#define MNPHONE_SIMLOCK_1301_112_2_17_22_23_39_1759 "MNPHONE:SIMLockDataReadAndIntegrityCheck control data check fail"
#define MNPHONE_SIMLOCK_1311_112_2_17_22_23_39_1760 "MNPHONE:SIMLockDataReadAndIntegrityCheck customize data read NV fail"
#define MNPHONE_SIMLOCK_1323_112_2_17_22_23_39_1761 "MNPHONE:For debug, COP_ID: %d C_L_Status: %d MAX_C %d TIME_INT %d TIME_MULT %d"
#define MNPHONE_SIMLOCK_1334_112_2_17_22_23_39_1762 "MNPHONE:For debug, NetworkLocks Len %d, PLMN1 %d %d %d, PLMN2 %d %d %d, PLMN3 %d %d %d"
#define MNPHONE_SIMLOCK_1351_112_2_17_22_23_39_1763 "MNPHONE:For debug, NetworkSubsetLocks Len %d, PLMN_SUB1 %d %d %d %d %d, PLMN_SUB2 %d %d %d %d %d, PLMN_SUB3 %d %d %d %d %d"
#define MNPHONE_SIMLOCK_1365_112_2_17_22_23_39_1764 "MNPHONE:For debug, SP_Locks Len %d, SP1 %d %d %d %d, SP2 %d %d %d %d, SP3 %d %d %d %d"
#define MNPHONE_SIMLOCK_1382_112_2_17_22_23_39_1765 "MNPHONE:For debug, CORPORATE_Locks Len %d, CORPORATE1 %d %d %d %d %d, CORPORATE2 %d %d %d %d %d, CORPORATE3 %d %d %d %d %d"
#define MNPHONE_SIMLOCK_1391_112_2_17_22_23_39_1766 "MNPHONE:For debug, Customize Data dummy1 %d dummy2 %d dummy3 %d dummy4 %d CRC: %d %d %d %d"
#define MNPHONE_SIMLOCK_1397_112_2_17_22_23_39_1767 "MNPHONE:SIMLockDataReadAndIntegrityCheck customize data check fail"
#define MNPHONE_SIMLOCK_1407_112_2_17_22_23_39_1768 "MNPHONE:SIMLockDataReadAndIntegrityCheck user data read NV fail"
#define MNPHONE_SIMLOCK_1425_112_2_17_22_23_39_1769 "MNPHONE:For debug, U_L_Status: %d nck: %d, %d nsck: %d, %d spck: %d, %d cck: %d, %d pck: %d, %d"
#define MNPHONE_SIMLOCK_1434_112_2_17_22_23_39_1770 "MNPHONE:For debug, User Data dummy1 %d dummy2 %d dummy3 %d dummy4 %d CRC: %d %d %d %d"
#define MNPHONE_SIMLOCK_1463_112_2_17_22_23_39_1771 "MNPHONE:For debug, User_Locks Len %d, IMSI1 len %d val %d %d %d %d %d %d %d %d, IMSI2 len %d val %d %d %d %d %d %d %d %d, IMSI3 len %d val %d %d %d %d %d %d %d %d"
#define MNPHONE_SIMLOCK_1469_112_2_17_22_23_39_1772 "MNPHONE:SIMLockDataReadAndIntegrityCheck user data check fail"
#define MNPHONE_SIMLOCK_1530_112_2_17_22_23_39_1773 "MNPHONE:GetUnlockConfigData IntegrityCheck=%d"
#define MNPHONE_SIMLOCK_1574_112_2_17_22_23_39_1774 "MNPHONE:UpdateUnlockRetryData type err=%d"
#define MNPHONE_SIMLOCK_1584_112_2_17_22_23_39_1775 "MNPHONE:UpdateUnlockRetryData IntegrityCheck=%d"
#define MNPHONE_SIMLOCK_1635_112_2_17_22_23_39_1776 "MNPHONE:SIMUnlockAction type err=%d"
#define MNPHONE_SIMLOCK_1644_112_2_17_22_23_39_1777 "MNPHONE:SIMUnlockAction lock_status=%d, unlock_type=%d"
#define MNPHONE_SIMLOCK_1671_112_2_17_22_23_40_1778 "MNPHONE:SIMUnlockAction updateNV fail"
#define MNPHONE_SIMLOCK_1688_112_2_17_22_23_40_1779 "MNPHONE:SIMUnlockAction IntegrityCheck=%d"
#define MNPHONE_SIMLOCK_1721_112_2_17_22_23_40_1780 "MNPHONE:SIMlockAction type err=%d"
#define MNPHONE_SIMLOCK_1793_112_2_17_22_23_40_1781 "MNPHONE:SIMlockAction GID1 not support"
#define MNPHONE_SIMLOCK_1819_112_2_17_22_23_40_1782 "MNPHONE:SIMlockAction GID1,2 not support"
#define MNPHONE_SIMLOCK_1856_112_2_17_22_23_40_1783 "MNPHONE:SIMlockAction type err=%d"
#define MNPHONE_SIMLOCK_1873_112_2_17_22_23_40_1784 "MNPHONE:SIMlockAction update customize NV fail"
#define MNPHONE_SIMLOCK_1885_112_2_17_22_23_40_1785 "MNPHONE:SIMlockAction update user NV fail"
#define MNPHONE_SIMLOCK_1900_112_2_17_22_23_40_1786 "MNPHONE:SIMlockAction already locked"
#define MNPHONE_SIMLOCK_1908_112_2_17_22_23_40_1787 "MNPHONE:SIMlockAction IntegrityCheck=%d"
#define MNPHONE_SIMLOCK_1943_112_2_17_22_23_40_1788 "MNPHONE:MEPersonalisationVerify Test card"
#define MNPHONE_SIMLOCK_1962_112_2_17_22_23_40_1789 "MNPHONE:MEPersonalisationVerify NetworkLock MCC=%d, MNC=%d, MNC_LEN=%d"
#define MNPHONE_SIMLOCK_1981_112_2_17_22_23_40_1790 "MNPHONE:MEPersonalisationVerify NetworkSubsetLock MCC=%d, MNC=%d, MNC_LEN=%d, SUB1=%d, SUB2=%d"
#define MNPHONE_SIMLOCK_1996_112_2_17_22_23_40_1791 "MNPHONE:MEPersonalisationVerify SPLock MCC=%d, MNC=%d, MNC_LEN=%d, GID1=%d"
#define MNPHONE_SIMLOCK_2011_112_2_17_22_23_40_1792 "MNPHONE:MEPersonalisationVerify CorporateLock MCC=%d, MNC=%d, MNC_LEN=%d, GID1=%d, GID2=%d"
#define MNPHONE_SIMLOCK_2027_112_2_17_22_23_40_1793 "MNPHONE:MEPersonalisationVerify UserLock IMSI validflag:%d, len:%d, val:%d %d %d %d %d %d %d %d"
#define MNPHONE_SIMLOCK_2067_112_2_17_22_23_40_1794 "MNPHONE:PHONE_IsOrangeAutoLock return TRUE,"
#define MNPHONE_SIMLOCK_2073_112_2_17_22_23_40_1795 "MNPHONE:PHONE_IsOrangeAutoLock return FALSE, lock status=%d"
#define MNPHONE_SIMLOCK_2079_112_2_17_22_23_40_1796 "MNPHONE:PHONE_IsOrangeAutoLock return FALSE, operator name=%s"
#define MNPHONE_SIMLOCK_2085_112_2_17_22_23_40_1797 "MNPHONE:PHONE_IsOrangeAutoLock return FALSE, gid1 valid=%d"
#define MNPHONE_SIMLOCK_2091_112_2_17_22_23_40_1798 "MNPHONE:PHONE_IsOrangeAutoLock return FALSE,access class=%d"
#define MNPHONE_SIMLOCK_2098_112_2_17_22_23_40_1799 "MNPHONE:PHONE_IsOrangeAutoLock return FALSE,mcc=%d,mnc=%d"
#define MNPHONE_SIMLOCK_2132_112_2_17_22_23_40_1800 "MNPHONE:PHONE_OrangeAutoLockAction update customize/user NV fail"
#define MNPHONE_SIMLOCK_2145_112_2_17_22_23_41_1801 "MNPHONE:PHONE_IsOrangeAutoLock return FALSE,company id=%d,LES=%d"
#define NVITEM_INIT_333_112_2_17_22_23_44_1802 "nvitem init value now\n"
#define NVITEM_INIT_337_112_2_17_22_23_44_1803 "get classmark\n"
#define NVITEM_INIT_339_112_2_17_22_23_44_1804 "get classmark over\n\n"
#define NVITEM_INIT_340_112_2_17_22_23_44_1805 "get nvitems\n"
#define NVITEM_INIT_342_112_2_17_22_23_44_1806 "get nvitems over\n\n"
#define NVITEM_INIT_478_112_2_17_22_23_44_1807 "open nvitems file failed,so create new file\n"
#define NVITEM_INIT_492_112_2_17_22_23_44_1808 "line %d is omittded\n"
#define NVITEM_INIT_497_112_2_17_22_23_44_1809 "line %d is unknown\n"
#define NVITEM_INIT_522_112_2_17_22_23_44_1810 "open classmark file failed\n"
#define NVITEM_INIT_536_112_2_17_22_23_44_1811 "line %d is omittded\n"
#define NVITEM_INIT_541_112_2_17_22_23_44_1812 "line %d is unknown\n"
#define MMDUMMY_450_112_2_17_22_24_6_1813 "MM: decode Msg Header,MESSAGE_TOO_SHORT"
#define MMDUMMY_478_112_2_17_22_24_6_1814 "MM: decode Msg Header,SKIP_INDICATOR_NOT_ZERO"
#define MMDUMMY_512_112_2_17_22_24_6_1815 "MM: decode Msg Header,INVALID_MESSAGE_TYPE"
#define MMDUMMY_524_112_2_17_22_24_6_1816 "MM: decode Msg Header,INVALID_PROTOCOL_DISCRIMINATOR"
#define PSDUMMY_MAIN_316_112_2_17_22_24_11_1817 "psdummy: cann't find the msg %d\n"
#define PSDUMMY_MAIN_348_112_2_17_22_24_11_1818 "MN receive unexpected signal %x\n"
#define SMDUMMY_DECODE_57_112_2_17_22_24_13_1819 "SM:decode Transaction identifier, TIE reserved!"
#define SMDUMMY_DECODE_84_112_2_17_22_24_13_1820 "SM:decode Transaction identifier, EXT is '0'!"
#define SMDUMMY_DECODE_119_112_2_17_22_24_13_1821 "SM:Receive APN Length > APN buffer!"
#define SMDUMMY_DECODE_185_112_2_17_22_24_13_1822 "SM:PDP address length too long!"
#define SMDUMMY_DECODE_208_112_2_17_22_24_13_1823 "SM:decode pdp address,PPP,PDP type reserved!"
#define SMDUMMY_DECODE_236_112_2_17_22_24_13_1824 "SM:decode pdp address,PDP org type reserved!"
#define SMDUMMY_DECODE_318_112_2_17_22_24_13_1825 "SM: LLC SAPI Spare Field non-0!"
#define SMDUMMY_DECODE_326_112_2_17_22_24_13_1826 "SM: LLC SAPI not assigned!"
#define SMDUMMY_DECODE_338_112_2_17_22_24_13_1827 "SM: LLC SAPI value reserved,invalid!"
#define SMDUMMY_DECODE_482_112_2_17_22_24_13_1828 "SM: is_r99 = %d"
#define SMDUMMY_DECODE_508_112_2_17_22_24_14_1829 "SM:decode Qos Field invalid!"
#define SMDUMMY_DECODE_540_112_2_17_22_24_14_1830 "SM:decode Qos Field invalid!"
#define SMDUMMY_DECODE_547_112_2_17_22_24_14_1831 "SM:ignore extented qos fields!"
#define SMDUMMY_DECODE_705_112_2_17_22_24_14_1832 "SM: PDP context accept,not support iei = %d"
#define SMDUMMY_DECODE_837_112_2_17_22_24_14_1833 "SM: PDP context accept,not support iei = %d"
#define SMDUMMY_ENCODE_250_112_2_17_22_24_15_1834 "SM:SM_EncodePdpaddr pdp_addr_length too long!"
#define SMDUMMY_ENCODE_282_112_2_17_22_24_15_1835 "SM:SM_EncodePdpaddr pdp_addr_length too long!"
#define SMDUMMY_ENCODE_297_112_2_17_22_24_15_1836 "SM:Assert Failure,SM_EncodePdpaddr"
#define SMDUMMY_ENCODE_330_112_2_17_22_24_15_1837 "SM:SM_EncodeApn apn too long!"
#define CALIBRATION_1497_112_2_17_22_24_47_1838 "L1:calibration,recv unknown sig %d"
#define CALIBRATION_1695_112_2_17_22_24_48_1839 "L1:caliberation diag paramter is wrong!"
#define CALIBRATION_1813_112_2_17_22_24_48_1840 "L1:band=%x,arfcn=%x,ind=%x,val=%x,type=%x,count=%x"
#define CALIBRATION_2329_112_2_17_22_24_49_1841 "\"\"handle tx request band = %d"
#define CALIBRATION_2429_112_2_17_22_24_49_1842 "\"\"handle tx request band = %d"
#define CALIBRATION_2495_112_2_17_22_24_49_1843 "SEND 8PSK TX REQ"
#define CALIBRATION_2534_112_2_17_22_24_49_1844 "\"STOP 8PSK TX "
#define CALIBRATION_2870_112_2_17_22_24_50_1845 "set EGSM index %d gain= %x"
#define CALIBRATION_2884_112_2_17_22_24_50_1846 "\"RX REQ g_layer1_state =%d "
#define CALIBRATION_2975_112_2_17_22_24_50_1847 "set EGSM index %d gain= %x"
#define CALIBRATION_2990_112_2_17_22_24_50_1848 "\"RX REQ g_layer1_state =%d "
#define CALIBRATION_3038_112_2_17_22_24_50_1849 "set EGSM index %d gain= %x"
#define CALIBRATION_3058_112_2_17_22_24_50_1850 "set DCS index %d gain= %x"
#define CALIBRATION_3077_112_2_17_22_24_51_1851 "set PCS index %d gain= %x"
#define CALIBRATION_3097_112_2_17_22_24_51_1852 "set GSM850 index %d gain= %x"
#define CALIBRATION_3115_112_2_17_22_24_51_1853 "\"RX REQ g_layer1_state =%d "
#define CALIBRATION_3288_112_2_17_22_24_51_1854 "L1:RSSI== %x"
#define CALIBRATION_3532_112_2_17_22_24_51_1855 "L1:RF control Request,value = %d"
#define CALIBRATION_3536_112_2_17_22_24_51_1856 "L1:RF control Request,length = %d"
#define CALIBRATION_3548_112_2_17_22_24_51_1857 "L1:RF control Request,gpip_no = %d value = %d"
#define CALIBRATION_3561_112_2_17_22_24_51_1858 "L1:RF control Request,tx = %d"
#define CALIBRATION_3848_112_2_17_22_24_52_1859 "L1:RAMP_ADD=%x,OFFSET=%x"
#define CALIBRATION_4059_112_2_17_22_24_52_1860 "rxtxloop, savesigal mode error = %d"
#define CALIBRATION_4074_112_2_17_22_24_53_1861 "rxtxloop, new pcl %d"
#define CALIBRATION_4094_112_2_17_22_24_53_1862 "rxtxloop, new cmd=%d"
#define CALIBRATION_4111_112_2_17_22_24_53_1863 "rxtxloop wakedsp1 =%d"
#define CALIBRATION_4181_112_2_17_22_24_53_1864 "rxtxloop init, save msg error."
#define CALIBRATION_4188_112_2_17_22_24_53_1865 "rxtxloop init, layer1 state error."
#define CALIBRATION_4193_112_2_17_22_24_53_1866 "rxtxloop init, substate error"
#define CALIBRATION_4467_112_2_17_22_24_53_1867 "rxtxloop, update nv start"
#define CALIBRATION_4471_112_2_17_22_24_53_1868 "rxtxloop, update nv end"
#define CALIBRATION_4528_112_2_17_22_24_53_1869 "rxtxloop, req sig, cmd=%d"
#define CALIBRATION_4564_112_2_17_22_24_53_1870 "loop s=%d fn=x%x"
#define CALIBRATION_4608_112_2_17_22_24_54_1871 "change arfcn"
#define CALIBRATION_4661_112_2_17_22_24_54_1872 "SetState=%d"
#define CALIBRATION_4739_112_2_17_22_24_54_1873 "rxlev2diag 0x%x"
#define CALIBRATION_4842_112_2_17_22_24_54_1874 "rxtxloop, errors %d, cmd %d, substate %d"
#define CALIBRATION_4920_112_2_17_22_24_54_1875 "rxtxloop, verify sch delay=%d, fn %d"
#define CALIBRATION_4952_112_2_17_22_24_54_1876 "rxtxloop2next=%d"
#define CALIBRATION_5018_112_2_17_22_24_54_1877 "rxtxloop, no bcch or wrong askmore passed here , which triggered in search"
#define CALIBRATION_5094_112_2_17_22_24_55_1878 "rxtxloop startcamp, target=%d"
#define CALIBRATION_5151_112_2_17_22_24_55_1879 "rxtxloop, target=%d,gain=%d"
#define CALIBRATION_5189_112_2_17_22_24_55_1880 "rxtxloop, meas n=%d"
#define CALIBRATION_5265_112_2_17_22_24_55_1881 "rxtxloop:RxBlockReady"
#define CALIBRATION_5307_112_2_17_22_24_55_1882 "rxtxloop, search/read state machine error.!!!"
#define CALIBRATION_5312_112_2_17_22_24_55_1883 "rxtxloop, rxblock channel tag error !!!"
#define CALIBRATION_5333_112_2_17_22_24_55_1884 "rxtxloop, sch verify sync error."
#define CALIBRATION_5343_112_2_17_22_24_55_1885 "rxtxloop, sch data sync error."
#define CALIBRATION_5482_112_2_17_22_24_55_1886 "rxtxloop, sch verify, deltaold=%d, deltanew=%d"
#define CALIBRATION_5509_112_2_17_22_24_55_1887 "rxtxloop, sch successed, oldfn=%d, curfn=%d, deltafn=%d"
#define CALIBRATION_5527_112_2_17_22_24_55_1888 "sch failed, cell=%d, count=%d"
#define CALIBRATION_5559_112_2_17_22_24_55_1889 "rxtxloop meas count=%d, result=%d"
#define CALIBRATION_5578_112_2_17_22_24_56_1890 "rxtxloop, not right arfcn queue"
#define CALIBRATION_5606_112_2_17_22_24_56_1891 "rxtxloop, measreport lev x%x, qual x%x"
#define CALIBRATION_5620_112_2_17_22_24_56_1892 "rxtxloop, readpwrque"
#define CALIBRATION_5679_112_2_17_22_24_56_1893 "rxtxloop, BELOW_NOISE_LO"
#define CALIBRATION_5684_112_2_17_22_24_56_1894 "rxtxloop, GOOD, arfcn=%d"
#define CALIBRATION_5755_112_2_17_22_24_56_1895 "txtxloop, agc=%d"
#define CALIBRATION_5777_112_2_17_22_24_56_1896 "rxtxloop, start search fn 0x%x"
#define CALIBRATION_5819_112_2_17_22_24_56_1897 "loop, change arfcn %d, tn %d, tsc %d"
#define CALIBRATION_5851_112_2_17_22_24_56_1898 "loop, arfcn %d, tn %d, tsc %d"
#define CALIBRATION_5922_112_2_17_22_24_56_1899 "idle askmore1"
#define CALIBRATION_5928_112_2_17_22_24_56_1900 "idle askmore2"
#define CALIBRATION_6077_112_2_17_22_24_57_1901 "fastcali SaveParamSet cmd %d, count %d"
#define CALIBRATION_6218_112_2_17_22_24_57_1902 "fastcali parse param %d, rxcount %d, tickcount %d"
#define CALIBRATION_6228_112_2_17_22_24_57_1903 "fastcali parse count %d"
#define CALIBRATION_6239_112_2_17_22_24_57_1904 "fastcali parse arfcn 0x%x mask %d slotnum %d, pcl[%d, %d, %d, %d]"
#define CALIBRATION_6245_112_2_17_22_24_57_1905 "fastcali error %d"
#define CALIBRATION_6360_112_2_17_22_24_57_1906 "fastcali err=%d, substate=%d"
#define CALIBRATION_6386_112_2_17_22_24_57_1907 "fastcali idle cmd %d"
#define CALIBRATION_6508_112_2_17_22_24_57_1908 "fastcali Read , arfcn %d, fn 0x%x"
#define CALIBRATION_6558_112_2_17_22_24_57_1909 "fastcali Search , arfcn %d, fn 0x%x"
#define CALIBRATION_6580_112_2_17_22_24_58_1910 "fastcali Search , end fn 0x%x"
#define CALIBRATION_6602_112_2_17_22_24_58_1911 "fastcali last askmore."
#define CALIBRATION_6616_112_2_17_22_24_58_1912 "fastcali askmore rx fn %d, state %d"
#define CALIBRATION_6652_112_2_17_22_24_58_1913 "fastcali askmore rx fn %d"
#define CALIBRATION_6713_112_2_17_22_24_58_1914 "s_fastcali_slot_rxlev_counter=%d, count=%d"
#define CALIBRATION_6722_112_2_17_22_24_58_1915 "RSSI[%d]=%d"
#define CALIBRATION_6765_112_2_17_22_24_58_1916 "fastcali rHandlePowerQue state %d"
#define CALIBRATION_6798_112_2_17_22_24_58_1917 "fastcali read from Power mon queue fail! "
#define CALIBRATION_6801_112_2_17_22_24_58_1918 "fastcali rHandlePowerQue counter %d"
#define CALIBRATION_6875_112_2_17_22_24_58_1919 "FASTCALI_ProcessRxSch, synced_fn 0x%x, oldfn 0x%x, cur_fn 0x%x, searchcount %d, spentcount %d"
#define CALIBRATION_6914_112_2_17_22_24_58_1920 "fastcali sch channel tag error !!!"
#define CALIBRATION_6920_112_2_17_22_24_58_1921 "fastcali sch read buffer fail !!!"
#define CALIBRATION_6952_112_2_17_22_24_58_1922 "fastcali sch resq sub state error %d"
#define CALIBRATION_6971_112_2_17_22_24_58_1923 "fastcali GetRxTstFrmDelay startfn %d, curfn %d"
#define CALIBRATION_7011_112_2_17_22_24_58_1924 "fastcali triggerrx error : can't find rx frame"
#define CALIBRATION_7033_112_2_17_22_24_58_1925 "fastcali FindFirstRxFrameIndex %d isfind 1"
#define CALIBRATION_7061_112_2_17_22_24_58_1926 "fastcali FindFirstRxFrameIndex %d isfind %d"
#define CALIBRATION_7092_112_2_17_22_24_59_1927 "fastcali plot fn 0x%x"
#define CALIBRATION_7116_112_2_17_22_24_59_1928 "fastcali plot tx 0x%x"
#define CALIBRATION_7483_112_2_17_22_24_59_1929 "fastcali sch fail , substate %d, fcch ind %d, cell index %d, search_timeout %d"
#define CALIBRATION_7505_112_2_17_22_24_59_1930 "fastcali sch read fail , substate %d, fcch ind %d, "
#define CALIBRATION_7524_112_2_17_22_24_59_1931 "fastcali FastCali_Initialize"
#define CALIBRATION_7618_112_2_17_22_25_0_1932 "fastcali SchRxTriggerDelay fn %d, delay %d"
#define CELLSYNCINFO_581_112_2_17_22_25_1_1933 "L1 init %d, cell_id %d,new arfcn_index %x;old %x,%x "
#define CELLSYNCINFO_1147_112_2_17_22_25_2_1934 "L1 cell_id invalid %x"
#define CELLSYNCINFO_1161_112_2_17_22_25_2_1935 "L1 Get_RR_S State cell_index %d"
#define CELLSYNCINFO_1710_112_2_17_22_25_3_1936 "L1: new ta %d"
#define CELLSYNCINFO_1720_112_2_17_22_25_3_1937 "Handover SYNC cell old_ta = %d,new_ta %d,otd %d"
#define CELLSYNCINFO_1730_112_2_17_22_25_3_1938 "Handover PSEUDO old ta %d,otd %d,rtd %d,new_ta %d"
#define CELLSYNCINFO_1742_112_2_17_22_25_3_1939 "Handover PSEUDO cell old_ta = %d,new_ta %d,otd %d"
#define CELLSYNCINFO_1871_112_2_17_22_25_4_1940 "L1 camp to same!"
#define CELLSYNCINFO_2521_112_2_17_22_25_5_1941 "L1 bcch select %x"
#define CELLSYNCINFO_2533_112_2_17_22_25_5_1942 "L1 receive main scell continue bcch"
#define CELLSYNCINFO_2818_112_2_17_22_25_6_1943 "L1: CellSyncInfo_GetNextCBCHBlkDelay schedule %ld, %d"
#define CELLSYNCINFO_2839_112_2_17_22_25_6_1944 "L1:CellSyncInfo_GetNextCBCHBlkDelay %d"
#define CELLSYNCINFO_2850_112_2_17_22_25_6_1945 "L1: MPHHandle_IsDiscardCBCHBlock %d"
#define CELLSYNCINFO_2918_112_2_17_22_25_6_1946 "L1:Handover fail, cell_id = %d,bsic %d,target bsic %d"
#define CELLSYNCINFO_2956_112_2_17_22_25_6_1947 "L1:handover cell %d,arfcn %d otd %d,scell %d,arfcn %d,otd %d, ba_index %d"
#define CELLSYNCINFO_2967_112_2_17_22_25_6_1948 "L1 no space to add a new arfcn to ba!"
#define CELLSYNCINFO_3503_112_2_17_22_25_7_1949 "L1: 1:%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,nL1: 2:%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d"
#define CELLSYNCINFO_3590_112_2_17_22_25_7_1950 "L1 start decode PSI,need decode %d"
#define CELLSYNCINFO_4765_112_2_17_22_25_10_1951 "L1 invalid cell id %d"
#define CELLSYNCINFO_4824_112_2_17_22_25_10_1952 "L1 sacch counter: %d, reset!"
#define CELLSYNCINFO_5073_112_2_17_22_25_10_1953 "L1: invalid tc"
#define CELLSYNCINFO_5296_112_2_17_22_25_11_1954 "L1 invlaid cell id %d"
#define CELLSYNCINFO_5326_112_2_17_22_25_11_1955 "L1 save bcch tc %d,%x,fn %x"
#define CELLSYNCINFO_5377_112_2_17_22_25_11_1956 "L1 invalid cell_id %d"
#define CELLSYNCINFO_5423_112_2_17_22_25_11_1957 "L1 s_b %x, tc %d"
#define CELLSYNCINFO_5428_112_2_17_22_25_11_1958 "L1 bcch has been invalid %d %d,%x"
#define CELLSYNCINFO_5435_112_2_17_22_25_11_1959 "L1 invalid cell_id %d"
#define CELLSYNCINFO_5474_112_2_17_22_25_11_1960 "L1 b_t  send %d, %d"
#define CELLSYNCINFO_5530_112_2_17_22_25_11_1961 "L1 b_t  set %d, %d"
#define CELLSYNCINFO_5533_112_2_17_22_25_11_1962 "L1 bcch_buf_index_queue full!"
#define CELLSYNCINFO_5566_112_2_17_22_25_11_1963 "L1 b_t set"
#define CELLSYNCINFO_5570_112_2_17_22_25_11_1964 "L1 b_t creat fail"
#define CELLSYNCINFO_5586_112_2_17_22_25_11_1965 "L1 b_t a"
#define CELLSYNCINFO_5593_112_2_17_22_25_11_1966 "L1 b_t  a f"
#define CELLSYNCINFO_5608_112_2_17_22_25_11_1967 "L1 b_t dea"
#define CELLSYNCINFO_5613_112_2_17_22_25_11_1968 "L1 b_t dea"
#define CELLSYNCINFO_5618_112_2_17_22_25_11_1969 "L1 b_t de f"
#define CELLSYNCINFO_5885_112_2_17_22_25_12_1970 "L1 pch delay %x, %x, %x"
#define CELLSYNCINFO_5961_112_2_17_22_25_12_1971 "L1 scell of card %d exist!%d"
#define CELLSYNCINFO_6030_112_2_17_22_25_12_1972 "L1 pch p_p co %x, se %x"
#define CELLSYNCINFO_6045_112_2_17_22_25_12_1973 "L1 pch p_b co %x"
#define CELLSYNCINFO_6224_112_2_17_22_25_13_1974 "L1 invalid fn %x,%x"
#define CELLSYNCINFO_6235_112_2_17_22_25_13_1975 "L1 invalid fn %x"
#define CELLSYNCINFO_6246_112_2_17_22_25_13_1976 "L1 invalid fn %x"
#define CELLSYNCINFO_6262_112_2_17_22_25_13_1977 "L1 fn adjust detected!"
#define CMDQ_135_112_2_17_22_25_13_1978 "L1 deactive all"
#define CMDQ_342_112_2_17_22_25_14_1979 "Layer1_cmd, audio in %d, audio out %d, is_enable %d"
#define CMDQ_353_112_2_17_22_25_14_1980 "arm control audio , cancel audio enable to dsp!"
#define CMDQ_394_112_2_17_22_25_14_1981 "layer1_cmd: command audio enable/disable"
#define CMDQ_417_112_2_17_22_25_14_1982 "layer1_cmd: command audio enable/disable"
#define CMDQ_1097_112_2_17_22_25_15_1983 "L1:SEND CMDQ_WAKEUP_DSP!\n"
#define CMDQ_1212_112_2_17_22_25_16_1984 "L1:start or stop transfer data! "
#define CMDQ_1341_112_2_17_22_25_16_1985 "L1: send open loop back cmd to dsp"
#define CMDQ_1424_112_2_17_22_25_16_1986 "L1: CmdQ_StartVoiceUpLoad, p0 0x%x, p1 0x%x, p2 0x%x, p3 0x%x"
#define CMDQ_1449_112_2_17_22_25_16_1987 "L1: CmdQ_StartVoiceUpLoad, p0 0x%x, p1 0x%x, p2 0x%x, p3 0x%x"
#define CMDQ_1475_112_2_17_22_25_16_1988 "L1:CmdQ_StartVoiceDownLoad"
#define CMDQ_1502_112_2_17_22_25_16_1989 "L1:CmdQ_NotifyPchReorgMode"
#define CMDQ_1736_112_2_17_22_25_17_1990 "L1:CMDQ_Dedicate_Establish: 0x%x, is_dsp_addr_sent %d"
#define CMDQ_1760_112_2_17_22_25_17_1991 "L1:CmdQ_StartMagicVoiceFun flag %d,type %d,uplink_gain %d,downlink_gain %d"
#define DATACNFPROC_118_112_2_17_22_25_17_1992 "L1: SendRachCNF!"
#define DATACNFPROC_245_112_2_17_22_25_17_1993 "L1: Handover_ACCESS_CNF!"
#define DATACNFPROC_286_112_2_17_22_25_18_1994 "L1: MPH_ERROR_IND!"
#define DATACNFPROC_300_112_2_17_22_25_18_1995 "L1: MPH_RXPOWER_SWEEP_CNF!"
#define DATACNFPROC_405_112_2_17_22_25_18_1996 "L1:FreqChangeCnf!"
#define DATACNFPROC_572_112_2_17_22_25_18_1997 "L1:DataCnfProc_MphSmsCbchDataInd normal message! %lx, %lx,%lx, %lx"
#define DATACNFPROC_578_112_2_17_22_25_18_1998 "L1:DataCnfProc_MphSmsCbchDataInd null message"
#define DATACNFPROC_650_112_2_17_22_25_18_1999 "L1: MPH_CHAN_ASSIGN_FAIL_CNF!"
#define DATACNFPROC_880_112_2_17_22_25_19_2000 "L1: MPH_CIPHER_MODE_CNF!"
#define DATACNFPROC_898_112_2_17_22_25_19_2001 "L1 set task id %x DataCnfProc_SetTaskIdForReadDspMemory"
#define DATACNFPROC_922_112_2_17_22_25_19_2002 "L1: dsp memory's response,value address 0x%x value %d"
#define DATACNFPROC_926_112_2_17_22_25_19_2003 "L1: dsp memory's response,value address 0x%x value %d"
#define DATACNFPROC_930_112_2_17_22_25_19_2004 "L1: dsp memory's response,value address 0x%x value %d"
#define DATACNFPROC_937_112_2_17_22_25_19_2005 "L1:dsp memory's response,value %d"
#define DATACNFPROC_951_112_2_17_22_25_19_2006 "L1:dsp memory's response,value Address %d"
#define DATACNFPROC_971_112_2_17_22_25_19_2007 "L1:layer1 should read DSP side memory!"
#define DATACNFPROC_1111_112_2_17_22_25_19_2008 "L1: MPH_CHANNEL_MODE_CNF!"
#define DATACNFPROC_1149_112_2_17_22_25_19_2009 "L1: non sync otd %d,%d,otd %d"
#define DATACNFPROC_1164_112_2_17_22_25_19_2010 "L1: MPH_HANDOVER_CNF!OTD = %d, old_TA = %d, MTD = %d"
#define DATACNFPROC_1182_112_2_17_22_25_19_2011 "L1: MPH_HANDOVER_FAIL_CNF!"
#define DATACNFPROC_1201_112_2_17_22_25_20_2012 "L1:PH_SCT_TCH_LOOP_ACK!"
#define DATACNFPROC_1245_112_2_17_22_25_20_2013 "L1:DataCnfProc_L1MnGetAFCCnf, AFC_Value = %d!"
#define DATACNFPROC_1297_112_2_17_22_25_20_2014 "L1:DataCnfProc_L1MnGetAFCCnf, PA_Value = %d"
#define DATACNFPROC_1328_112_2_17_22_25_20_2015 "L1:DataCnfProc_L1MnGetAGCCnf,AGC_Value = %d!"
#define DATACNFPROC_1358_112_2_17_22_25_20_2016 "L1:L1MnGetBlockErrorRatioCnf,total count= %d error count = %d!"
#define DATACNFPROC_1377_112_2_17_22_25_20_2017 "L1:DspVoiceLoopbackInd! Index = %d\n"
#define DATACNFPROC_1432_112_2_17_22_25_20_2018 "L1:DspVoiceLoopbackNxpInd! Index = %d\n"
#define DATACNFPROC_1440_112_2_17_22_25_20_2019 "L1:dsp index value in%d,out %d,total%d\n"
#define DATACNFPROC_1449_112_2_17_22_25_20_2020 "L1:dsp index value in%d,out %d,total%d\n"
#define DATAPROC_147_112_2_17_22_25_21_2021 "L1:Rx SA!,%d"
#define DATAPROC_159_112_2_17_22_25_21_2022 "L1:Rx bad SA counter = %d"
#define DATAPROC_200_112_2_17_22_25_21_2023 "L1:Rx bad FA,0x%x,0x%x"
#define DATAPROC_230_112_2_17_22_25_21_2024 "bad half rate FA,%3x,%3x"
#define DATAPROC_332_112_2_17_22_25_21_2025 "L1: AMR startreqacktimer fn 0x%x"
#define DATAPROC_577_112_2_17_22_25_21_2026 "L1:DataProc_RxRatscch: ACK_ERR or ACK_UNKNOW, type = 0x%x!"
#define DATAPROC_584_112_2_17_22_25_21_2027 "L1:DataProc_RxRatscch: Ignore RATSCCH in REQ_ACK periord, s_active_req = %d!"
#define DATAPROC_629_112_2_17_22_25_21_2028 "L1:bad RATSCCH,%3x,%3x"
#define DATAPROC_660_112_2_17_22_25_22_2029 "L1: bad SD,0x%x,0x%x"
#define DATAPROC_765_112_2_17_22_25_22_2030 "L1 invalid ref_scell_id %x main_card_id %x"
#define DATAPROC_824_112_2_17_22_25_22_2031 "L1:bsic suc %x, id %x,%x"
#define DATAPROC_873_112_2_17_22_25_22_2032 "L1 search invalid %x"
#define DATAPROC_885_112_2_17_22_25_22_2033 "L1: A change of bsic is detected!, old bsic %d, new bsic %d"
#define DATAPROC_914_112_2_17_22_25_22_2034 "L1 search invalid %x"
#define DATAPROC_926_112_2_17_22_25_22_2035 "L1: A change of bsic is detected!, old bsic %d, new bsic %d"
#define DATAPROC_953_112_2_17_22_25_22_2036 "L1:This cell maybe been replaced! Cell Id %d, arfcn %d!"
#define DATAPROC_965_112_2_17_22_25_22_2037 "L1:Note got sch but this cell's state is changed to NULL cell!"
#define DATAPROC_1029_112_2_17_22_25_22_2038 "L1:read tch suc: %x,cell %x,arfcn %x,otd %x,%x, ncc %x"
#define DATAPROC_1072_112_2_17_22_25_22_2039 "L1 search invalid %x"
#define DATAPROC_1133_112_2_17_22_25_22_2040 "L1 search invalid %x"
#define DATAPROC_1170_112_2_17_22_25_23_2041 "L1: A change of bsic is detected!, old bsic %d, new bsic %d"
#define DATAPROC_1199_112_2_17_22_25_23_2042 "This cell maybe have been substitut! Cell Id %d, arfcn %d!"
#define DATAPROC_1206_112_2_17_22_25_23_2043 "L1: Tch BSIC read success! Cell %d Arfcn = %d ncc = %d\r "
#define DATAPROC_1213_112_2_17_22_25_23_2044 "L1: got a sch but this cell's state is changed to NULL cell!"
#define DATAPROC_1227_112_2_17_22_25_23_2045 "L1 handover cell search suc !"
#define DATAPROC_1231_112_2_17_22_25_23_2046 "L1 handover cell search fail !cell is not permit !%x"
#define DATAPROC_1265_112_2_17_22_25_23_2047 "L1 bcch is not required by this cell %d, map %x,%x,%x"
#define DATAPROC_1280_112_2_17_22_25_23_2048 "L1:bad BCCH! Fn = 0x%lx, arfcn_index = %d!"
#define DATAPROC_1354_112_2_17_22_25_23_2049 "L1 bcch is not required by this cell %d, map %x,%x,%x"
#define DATAPROC_1364_112_2_17_22_25_23_2050 "L1:bad BCCH! %x,%x, fn = %d, arfcn_index = %d!"
#define DATAPROC_1444_112_2_17_22_25_23_2051 "L1 bcch is not required by this cell %d, map %x,%x,%x"
#define DATAPROC_1463_112_2_17_22_25_23_2052 "L1:bad BCCH! %x,%x, fn = 0x%x,arfcn_index = 0x%x!"
#define DATAPROC_1525_112_2_17_22_25_23_2053 "L1: Discard a agch block! due to error cell_index! scell_id = %d, error_cell_id = %d "
#define DATAPROC_1569_112_2_17_22_25_23_2054 "L1:bad AGCH! Fn = %lx, dsc %d"
#define DATAPROC_1625_112_2_17_22_25_23_2055 "L1: maintain DSC on pg_reorg with SCell changed"
#define DATAPROC_1642_112_2_17_22_25_23_2056 "L1: maintain DSC on pg_reorg"
#define DATAPROC_1834_112_2_17_22_25_24_2057 "L1:Wrong main pch frame number=0x%x"
#define DATAPROC_1844_112_2_17_22_25_24_2058 "L1 the card is deactived %x"
#define DATAPROC_1904_112_2_17_22_25_24_2059 "L1 bad pch 0x%x,dsc %d, card id %x "
#define DATAPROC_2336_112_2_17_22_25_25_2060 "L1: AMR getreqackdeltaspeechframe, new_fn 0x%x, old_fn 0x%x"
#define DATAPROC_2340_112_2_17_22_25_25_2061 "L1: AMR fn small"
#define DATAPROC_2365_112_2_17_22_25_25_2062 "GetREQACTDelta: fn_26=%d, fn_sp=%d, old_26=%d, old_sp=%d!"
#define DATAPROC_2368_112_2_17_22_25_25_2063 "GetREQACTDelta: old_fn=0x%x, delta=%d!"
#define DATAPROC_2386_112_2_17_22_25_25_2064 "GetACKACTDelta: fn=0x%x!"
#define DATAPROC_2392_112_2_17_22_25_25_2065 "L1: AMR getackackdeltaspeechframe, new_fn 0x%x, old_fn 0x%x"
#define DATAPROC_2397_112_2_17_22_25_25_2066 "L1: AMR fn small"
#define DATAPROC_2422_112_2_17_22_25_25_2067 "GetACKACTDelta: fn_26=%d, fn_sp=%d, old_26=%d, old_sp=%d!"
#define DATAPROC_2425_112_2_17_22_25_25_2068 "GetACKACTDelta: old_fn=0x%x, delta=%d!"
#define DATAPROC_2528_112_2_17_22_25_25_2069 "DataProc_TxFacch: applay new cmip = %d, fn = 0x%x!"
#define DATAPROC_2560_112_2_17_22_25_25_2070 "DataProc_TxFacch: DL, icm=%d, acs=0x%x, fn = 0x%x!"
#define DATAPROC_2582_112_2_17_22_25_25_2071 "DataProc_TxFacch: UL, icm=%d, acs=0x%x, fn=0x%x!"
#define DATAPROC_2611_112_2_17_22_25_25_2072 "DataProc_TxFacch: threshreq, one=%d,two=%d,three=%d, fn = 0x%x!"
#define DATAPROC_2695_112_2_17_22_25_26_2073 "DataProc_TxFacch: s_ratscch_ack_type=%d, fn=0x%x!"
#define DATAPROC_2708_112_2_17_22_25_26_2074 "DataProc_TxFacch ratmark: s_ratscch_ack_type=%d, fn=0x%x!"
#define DATAPROC_2734_112_2_17_22_25_26_2075 "DataProc_TxFacch ratdata: s_ratscch_ack_type=%d, fn=0x%x!"
#define DATAPROC_2860_112_2_17_22_25_26_2076 "L1:channel change! %d"
#define DATAPROC_3006_112_2_17_22_25_26_2077 "L1: bad PDTCH data!"
#define DATAPROC_3062_112_2_17_22_25_26_2078 "L1:fn 0x%x,dump unneed slots %d,slot mask:0x%x"
#define DATAPROC_3160_112_2_17_22_25_27_2079 "L1: same rx slot, fn %x,"
#define DATAPROC_3220_112_2_17_22_25_27_2080 "L1: ptcch fn %x: ta = %d"
#define DATAPROC_3232_112_2_17_22_25_27_2081 "L1: Invalid ptcch value = %d!"
#define DATAPROC_3238_112_2_17_22_25_27_2082 "L1: bad ptcch!"
#define DATAPROC_3268_112_2_17_22_25_27_2083 "L1: bad pbcch! "
#define DATAPROC_3290_112_2_17_22_25_27_2084 "L1: A WRONG bcch_dec_state.dec_mode!"
#define DATAPROC_3319_112_2_17_22_25_27_2085 "L1: bad pbcch! "
#define DATAPROC_3340_112_2_17_22_25_27_2086 "L1:psi decode num %d"
#define DATAPROC_3347_112_2_17_22_25_27_2087 "L1:bad pbcch!"
#define DATAPROC_3381_112_2_17_22_25_27_2088 "L1:bad pbcch!"
#define DATAPROC_3438_112_2_17_22_25_27_2089 "L1: bad ppch"
#define DATAPROC_3518_112_2_17_22_25_27_2090 "L1:bad ppch"
#define DATAPROC_3628_112_2_17_22_25_27_2091 "L1:bad ppch"
#define DATAPROC_3740_112_2_17_22_25_28_2092 "L1: a bad PAGCH in PRACH_PAGCH "
#define DATAPROC_3771_112_2_17_22_25_28_2093 "L1:RXCBCH!card_id:%d"
#define DATAPROC_3777_112_2_17_22_25_28_2094 "L1:bad CBCH!!"
#define DOWNLOADPARAM_2583_112_2_17_22_25_34_2095 "L1:Threshold = %d"
#define DOWNLOADPARAM_2626_112_2_17_22_25_34_2096 "L1:Encount error when read NV EQ parameters!"
#define DOWNLOADPARAM_2698_112_2_17_22_25_34_2097 "L1: Read NV selected band = %d"
#define DOWNLOADPARAM_2707_112_2_17_22_25_34_2098 "L1: Fail in NV selected band reading, set is as Dual Band"
#define DOWNLOADPARAM_2722_112_2_17_22_25_34_2099 "L1: Read NV selected deal flag = %d"
#define DOWNLOADPARAM_2731_112_2_17_22_25_34_2100 "L1: Fail in NV selected deal flag reading, set is as zeor"
#define DSPINTISR_531_112_2_17_22_25_36_2101 "L1: %d times .lstate=%d"
#define DSPINTISR_599_112_2_17_22_25_36_2102 "L1 invalid irq!%x"
#define DSPPROC_236_112_2_17_22_25_36_2103 "L1:access burst %d,%d"
#define DSPPROC_254_112_2_17_22_25_36_2104 "L1:access burst %d,%d,%d"
#define DSPPROC_310_112_2_17_22_25_37_2105 "L1:access burst %d,%d"
#define DSPPROC_326_112_2_17_22_25_37_2106 "L1:access burst %d,%d,%d"
#define DSPPROC_383_112_2_17_22_25_37_2107 "L1:param is ready!!"
#define DSPPROC_413_112_2_17_22_25_37_2108 "L1:param is ready!!"
#define DSPPROC_426_112_2_17_22_25_37_2109 "L1:WriteInitParaToDSP!g_is_download_param_done=%d"
#define DSPPROC_444_112_2_17_22_25_37_2110 "L1:param is ready!"
#define FAKE_RR_329_112_2_17_22_25_38_2111 "fake RR: sig code %x come in "
#define FAKE_RR_393_112_2_17_22_25_38_2112 "CALIBRATION: recv unknown signal "
#define FAKE_RR_455_112_2_17_22_25_38_2113 "fack RR: battery req send "
#define FRAMEINSTRUTIL_550_112_2_17_22_25_40_2114 "LAYER1: It's a wrong POWER SWEEP Number with 0! "
#define FRAMEINSTRUTIL_609_112_2_17_22_25_40_2115 "L1 rx_type %x"
#define FRAMEINSTRUTIL_1229_112_2_17_22_25_42_2116 "L1:card_id:%d Plot4FrInstCBCH %ld,fncard:%ld"
#define FRAMEINSTRUTIL_1459_112_2_17_22_25_42_2117 "L1 rx_type %x"
#define FRAMEINSTRUTIL_1462_112_2_17_22_25_42_2118 "L1 pch %x"
#define FRAMEINSTRUTIL_1580_112_2_17_22_25_43_2119 "L1 rx_type %x"
#define FRAMEINSTRUTIL_1882_112_2_17_22_25_43_2120 "L1 search %x"
#define FRAMEINSTRUTIL_1977_112_2_17_22_25_44_2121 "L1 search %x"
#define FRAMEINSTRUTIL_2025_112_2_17_22_25_44_2122 "L1: BCCH %x, %x,%x"
#define FRAMEINSTRUTIL_2153_112_2_17_22_25_44_2123 "L1:IT'S WRONG DEDICATE_MODE TYPE! "
#define FRAMEINSTRUTIL_2193_112_2_17_22_25_44_2124 "L1 invalid sd channel type %x"
#define FRAMEINSTRUTIL_2214_112_2_17_22_25_44_2125 "L1 fn %x, min %x;"
#define FRAMEINSTRUTIL_2658_112_2_17_22_25_45_2126 "L1 sdccch4 map: %d,%d,%d; %d,%d,%d"
#define FRAMEINSTRUTIL_2849_112_2_17_22_25_45_2127 "L1 sdccch8 map: %d,%d,%d; %d,%d,%d"
#define FRAMEINSTRUTIL_3179_112_2_17_22_25_46_2128 "L1: SDCCH searching!fn %x, min_delay %x"
#define FRAMEINSTRUTIL_3322_112_2_17_22_25_46_2129 "L1: IT's A WRONG s_dedicate_chan_map[fn_mod].channel_type!"
#define FRAMEINSTRUTIL_3384_112_2_17_22_25_46_2130 "L1: IT's A WRONG s_dedicate_chan_map[fn_mod].channel_type!"
#define FRAMEINSTRUTIL_3515_112_2_17_22_25_47_2131 "L1 tch read: %x"
#define FRAMEINSTRUTIL_3538_112_2_17_22_25_47_2132 "L1 tch search: %x"
#define FRAMEINSTRUTIL_4730_112_2_17_22_25_49_2133 "L1 set sacch int at handover fn %x"
#define FRAMEINSTRUTIL_4851_112_2_17_22_25_49_2134 "L1: TCH bsic read!Cell id %d"
#define FRAMEINSTRUTIL_4863_112_2_17_22_25_49_2135 "L1: TCH searching!"
#define FRAMEINSTRUTIL_4875_112_2_17_22_25_49_2136 "L1: TCH start search,cell_id %d"
#define FRAMEINSTRUTIL_5111_112_2_17_22_25_50_2137 "L1 sa dl %x"
#define FRAMEINSTRUTIL_5198_112_2_17_22_25_50_2138 "L1 sa dl %x"
#define FRAMEINSTRUTIL_5479_112_2_17_22_25_50_2139 "L1 sa tx %x"
#define FRAMEINSTRUTIL_5601_112_2_17_22_25_51_2140 "L1 sa dl %x"
#define FRAMEINSTRUTIL_5605_112_2_17_22_25_51_2141 "L1 sa tx %x"
#define FRAMEINSTRUTIL_6044_112_2_17_22_25_52_2142 "L1 sdcch read %x"
#define FRAMEINSTRUTIL_6830_112_2_17_22_25_53_2143 "L1:FrameInstrUtil_SetTxCmdPRACH:egprs rach,TS1"
#define FRAMEINSTRUTIL_6835_112_2_17_22_25_53_2144 "L1:FrameInstrUtil_SetTxCmdPRACH:egprs rach,TS2"
#define FRAMEINSTRUTIL_7150_112_2_17_22_25_54_2145 "L1:Rx data:fn curr:%d,tbf_start_fn :%d"
#define FRAMEINSTRUTIL_7221_112_2_17_22_25_54_2146 "L1: uplink tbf, plot rx frame, ta %d, data %d, polling %d"
#define FRAMEINSTRUTIL_7241_112_2_17_22_25_54_2147 "L1:No TA,not send dl polling 1"
#define FRAMEINSTRUTIL_7260_112_2_17_22_25_54_2148 "L1:No TA,not send dl polling 2"
#define FRAMEINSTRUTIL_7297_112_2_17_22_25_54_2149 "No TA,not send both link data 1"
#define FRAMEINSTRUTIL_7316_112_2_17_22_25_54_2150 "L1:No TA,not send both link data 2"
#define FRAMEINSTRUTIL_7868_112_2_17_22_25_55_2151 "L1 conflitnew rx slot 0x%x"
#define FRAMEINSTRUTIL_8618_112_2_17_22_25_57_2152 "L1:tbf read  = 0x%x"
#define FRAMEINSTRUTIL_8638_112_2_17_22_25_57_2153 "L1:tbf search  = 0x%x"
#define FRAMEINSTRUTIL_8997_112_2_17_22_25_57_2154 "L1:Assert Failure,Plot4AccessBurst "
#define FRAMEINSTRUTIL_9651_112_2_17_22_25_59_2155 "L1 PlotDspDebugFrames arfcn %x,band %d,gain %x,frametype %d"
#define FRAMEINSTRUTIL_9784_112_2_17_22_25_59_2156 "L1:FrameInstrUtil_SetTxCmdRACH:egprs rach,TS1"
#define FRAMEINSTRUTIL_9789_112_2_17_22_25_59_2157 "L1:FrameInstrUtil_SetTxCmdRACH:egprs rach,TS2"
#define GPRSFRAMEGEN_251_112_2_17_22_26_0_2158 "L1:Waiting for wakeup's response!"
#define GPRSFRAMEGEN_672_112_2_17_22_26_0_2159 "L1:starting time reach0: %d,%d"
#define GPRSFRAMEGEN_762_112_2_17_22_26_1_2160 "single fn reach1 : %d"
#define GPRSFRAMEGEN_834_112_2_17_22_26_1_2161 "single fn reach2 : %d"
#define GPRSFRAMEGEN_919_112_2_17_22_26_1_2162 "L1 ptcch firstly fn %x delay %x"
#define GPRSFRAMEGEN_1035_112_2_17_22_26_1_2163 "L1 invalid tbf state %d"
#define GPRSSYSINFO_740_112_2_17_22_26_3_2164 "L1:Assert Failure,NO Allocted Block Memory."
#define GPRSSYSINFO_1252_112_2_17_22_26_4_2165 "L1: polling ack and rx collision tx_slot 0x%x, rx_slot 0x%x, new rx_slot 0x%x"
#define GPRSSYSINFO_1356_112_2_17_22_26_4_2166 "L1API_GetTBFSlotMask: error tbf_direction = %d!"
#define GPRSSYSINFO_1409_112_2_17_22_26_4_2167 "wrong tbf mode %d!"
#define GPRSSYSINFO_1411_112_2_17_22_26_4_2168 "L1: reset tai,tbf_direction %d"
#define GPRSSYSINFO_1433_112_2_17_22_26_4_2169 "L1 access polling send_type %d"
#define GPRSSYSINFO_1442_112_2_17_22_26_4_2170 "L1 normal polling, ta_valid %d"
#define GPRSSYSINFO_1512_112_2_17_22_26_4_2171 "L1 invalid tbf!"
#define GPRSSYSINFO_1527_112_2_17_22_26_4_2172 "L1: exe tbf direction error"
#define GPRSSYSINFO_1701_112_2_17_22_26_5_2173 "L1: ptcch firstly cur_fn %x"
#define GSMFRAMEGEN_245_112_2_17_22_26_6_2174 "L1 GsmFrameGen_NullState arfcn 0x%x,band 0x%x,gain 0x%x"
#define GSMFRAMEGEN_760_112_2_17_22_26_7_2175 "L1 pwr sub state %d"
#define GSMFRAMEGEN_792_112_2_17_22_26_7_2176 "L1:optimize power on,enter POWERSWEEP_RESULT_PENDING"
#define GSMFRAMEGEN_900_112_2_17_22_26_7_2177 "L1:Waiting for wakeup's response!!!"
#define GSMFRAMEGEN_944_112_2_17_22_26_7_2178 "Waiting for wakeup's response!"
#define GSMFRAMEGEN_1006_112_2_17_22_26_7_2179 "Waiting for wakeup's response!!!"
#define GSMFRAMEGEN_1060_112_2_17_22_26_7_2180 "l1:PLMN CAMP, SEND ASKMORE TO SELF"
#define GSMFRAMEGEN_1199_112_2_17_22_26_8_2181 "Waiting for wakeup's response!!!"
#define GSMFRAMEGEN_1252_112_2_17_22_26_8_2182 "L1: Invalid PBCCH_CAMP sub state!"
#define GSMFRAMEGEN_1399_112_2_17_22_26_8_2183 "L1:Waiting for wakeup's response!"
#define GSMFRAMEGEN_1609_112_2_17_22_26_9_2184 "L1:Waiting for wakeup's response!"
#define GSMFRAMEGEN_1734_112_2_17_22_26_9_2185 "L1 adjust first send fn %x,current fn %x"
#define GSMFRAMEGEN_2178_112_2_17_22_26_10_2186 "L1:to cell's arfcn %d"
#define GSMFRAMEGEN_2273_112_2_17_22_26_10_2187 "L1:current chan matched with next chan!"
#define GSMFRAMEGEN_2401_112_2_17_22_26_10_2188 "L1 s_delay %x, p_delay %x, min_delay %x, count %x"
#define GSMFRAMEGEN_2437_112_2_17_22_26_10_2189 "L1 sdcch block finish count %d,i %x"
#define GSMFRAMEGEN_2444_112_2_17_22_26_10_2190 "L1 pch<0! count %d,is_search_ploted %d,i %x"
#define GSMFRAMEGEN_2450_112_2_17_22_26_10_2191 "L1:startfn is coming,not allowed to plot other frame"
#define GSMFRAMEGEN_2815_112_2_17_22_26_11_2192 "L1: search handover cell 0x%x"
#define LAYER1_AUDIO_192_112_2_17_22_26_13_2193 "L1:s_silencecallback_pfunc!"
#define LAYER1_AUDIO_197_112_2_17_22_26_13_2194 "L1: s_silencecallback_pfunc is PNULL!"
#define LAYER1_AUDIO_575_112_2_17_22_26_14_2195 "LAYER1_SetAudioEnable %d %d %d %d"
#define LAYER1_AUDIO_602_112_2_17_22_26_14_2196 "LAYER1_SetCodecEnable: is_enable = %d, dsp_state = %d"
#define LAYER1_AUDIO_610_112_2_17_22_26_14_2197 "LAYER1_SetCodecEnable: dsp_state = %d"
#define LAYER1_AUDIO_634_112_2_17_22_26_14_2198 "LAYER1_SetCodecEnable %d %d %d"
#define LAYER1_AUDIO_701_112_2_17_22_26_14_2199 "LAYER1_SetAudioEnable %d %d %d %d"
#define LAYER1_AUDIO_729_112_2_17_22_26_14_2200 "LAYER1_SetAudioEnable2 %d %d %d %d"
#define LAYER1_AUDIO_731_112_2_17_22_26_14_2201 "LAYER1_SetAudioEnable: is_enable = %d, dsp_state = %d"
#define LAYER1_AUDIO_739_112_2_17_22_26_14_2202 "LAYER1_SetAudioEnable: dsp_state = %d"
#define LAYER1_AUDIO_763_112_2_17_22_26_14_2203 "LAYER1_SetAudioEnable %d %d %d"
#define LAYER1_AUDIO_1046_112_2_17_22_26_15_2204 " send loopback open msg to layer1"
#define LAYER1_AUDIO_1130_112_2_17_22_26_15_2205 "L1: LSYER1_ReadIQData error"
#define LAYER1_AUDIO_1146_112_2_17_22_26_15_2206 "LOGSAVE FAIL: start gsm dsp log record in td mode!"
#define LAYER1_AUDIO_1320_112_2_17_22_26_15_2207 "L1: voice_format %d,dsp_upload_format= %x,is_from_mic=%x,is_from_net=%x,block_count %d,block_length %d iq switch 0x%x"
#define LAYER1_AUDIO_1380_112_2_17_22_26_16_2208 "LOGSAVE FAIL: start gsm dsp log record in td mode!"
#define LAYER1_AUDIO_1716_112_2_17_22_26_16_2209 "Layer1: eq_type= 0x%x"
#define LAYER1_AUDIO_1985_112_2_17_22_26_17_2210 "L1:LAYER1_Mp3SynthFrame.State:MP3_SYNTH,Time=%d ms£¬report_data_type = %d "
#define LAYER1_AUDIO_2063_112_2_17_22_26_17_2211 "L1:overlay Mp3 synthesis code,overlay_address = 0x%x"
#define LAYER1_AUDIO_2070_112_2_17_22_26_17_2212 "L1:overlay SBC code"
#define LAYER1_AUDIO_2077_112_2_17_22_26_17_2213 "L1:overlay Audio code"
#define LAYER1_AUDIO_2217_112_2_17_22_26_17_2214 "L1 mag: start 1 type %d"
#define LAYER1_AUDIO_2221_112_2_17_22_26_17_2215 "L1 mag: start 2 type %d"
#define LAYER1_AUDIO_2223_112_2_17_22_26_17_2216 "L1 mag: start 3 type %d"
#define LAYER1_AUDIO_2227_112_2_17_22_26_17_2217 "L1 mag: start 4 type %d"
#define LAYER1_AUDIO_2233_112_2_17_22_26_17_2218 "L1: start magic voice err, type is 0"
#define LAYER1_AUDIO_2236_112_2_17_22_26_17_2219 "L1 mag: start type 5 %d"
#define LAYER1_AUDIO_2267_112_2_17_22_26_17_2220 "LAYER1:LAYER1_StartRecord_Downlink register downlink callback function"
#define LAYER1_AUDIO_2278_112_2_17_22_26_17_2221 "LAYER1:LAYER1_StopRecord_Downlink unregister downlink callback function"
#define LAYER1_AUDIO_2289_112_2_17_22_26_17_2222 "LAYER1:LAYER1_StartRecord_Ad register uplink callback function"
#define LAYER1_AUDIO_2304_112_2_17_22_26_17_2223 "LAYER1:LAYER1_StopRecord_Ad unregister uplink callback function"
#define LAYER1_AUDIO_CODEC_84_112_2_17_22_26_18_2224 "LAYER1_AUDIO_CODEC_Data 0x%x"
#define LAYER1_AUDIO_CODEC_173_112_2_17_22_26_18_2225 "LAYER1_AUDIO_CODEC_CopyData data count is 0"
#define LAYER1_AUDIO_CODEC_214_112_2_17_22_26_18_2226 "LAYER1_AUDIO_CODEC_CopyLRData data count is 0"
#define LAYER1_DIAG_299_112_2_17_22_26_19_2227 "bef fastcali isnv %d, band %d, type %d, index %d, length %d"
#define LAYER1_DIAG_401_112_2_17_22_26_19_2228 "layer1_diag parameters is wrong!"
#define LAYER1_DIAG_881_112_2_17_22_26_20_2229 "the NV_ID_IMEI is wrong!"
#define LAYER1_DIAG_935_112_2_17_22_26_20_2230 "L1:NV RF solution hasn't been read out!"
#define LAYER1_DIAG_1053_112_2_17_22_26_20_2231 "L1 this function is implemented only at 8800s (Layer1_HandleDspDebug())"
#define LAYER1_DIAG_1080_112_2_17_22_26_20_2232 "fastcali diag ptr1 0x%x, pc_sig_ptr 0x%x, *pc_sig_ptr 0x%x"
#define LAYER1_DIAG_1102_112_2_17_22_26_20_2233 "fastcali diag cmd 0x%x, count %d, p1 %d, p2 %d, p3 %d, p4 %d"
#define LAYER1_FREQ_91_112_2_17_22_26_21_2234 "L1: IT's a WRONG frequency arfcn! 0x%x\n"
#define LAYER1_MAC_PROC_211_112_2_17_22_26_22_2235 "L1:map gmsk mean=%x,cv=%x;psk8 mean=%x,cv=%x;present=%x"
#define LAYER1_MAC_PROC_294_112_2_17_22_26_22_2236 "L1:dump the polling,out of dsp's ability,polling_slot=%d,first rx_slot=%d"
#define LAYER1_MAC_PROC_424_112_2_17_22_26_22_2237 "L1 polling data is not write  %d, %d!"
#define LAYER1_MAC_PROC_465_112_2_17_22_26_22_2238 "LAYER1: It's Layer1Mac_GetDataAndWriteData function\n"
#define LAYER1_MAC_PROC_503_112_2_17_22_26_23_2239 "L1 polling data is not write to dsp %d, %d!"
#define LAYER1_MAIN_2332_112_2_17_22_26_28_2240 "L1:save signal 0x%x\n"
#define LAYER1_MAIN_2344_112_2_17_22_26_28_2241 "L1:discard 0x%x, sender 0x%x\n"
#define LAYER1_MAIN_2412_112_2_17_22_26_28_2242 "L1: Initializing Physical Layer All Tasks\n\r"
#define LAYER1_MAIN_2463_112_2_17_22_26_28_2243 "L1: Initializing for active\n\r"
#define LAYER1_MAIN_2698_112_2_17_22_26_28_2244 "L1Main_calibration_timer"
#define LAYER1_MAIN_2818_112_2_17_22_26_29_2245 "L1: calibration timer register failed"
#define LAYER1_MAIN_2823_112_2_17_22_26_29_2246 "L1: calibration timer is not active, duration is equal 0"
#define LAYER1_MAIN_2952_112_2_17_22_26_29_2247 "L1Main_HandleLongTermBcchMeasTimer"
#define LAYER1_MAIN_3002_112_2_17_22_26_29_2248 "L1Main_HandleBcchMeasReportTimerReq"
#define LAYER1_MAIN_3121_112_2_17_22_26_29_2249 "L1:CheckIfBCCHCollide,invalid arfcn,old=0x%x,new=0x%x"
#define LAYER1_MAIN_3130_112_2_17_22_26_29_2250 "L1:CheckIfBCCHCollide: invalid cell_id,old=0x%x, new=0x%x"
#define LAYER1_MAIN_3153_112_2_17_22_26_29_2251 "L1:CheckIfBCCHCollide,fn_old_mod51=%d,fn_new_mod51=%d"
#define LAYER1_MAIN_3171_112_2_17_22_26_29_2252 "L1 set tbf flag!"
#define LAYER1_MAIN_3439_112_2_17_22_26_30_2253 "L1 dsp ready event not arrived!"
#define LAYER1_MAIN_3490_112_2_17_22_26_30_2254 "L1:L1Main_CreateToneDelayTimer "
#define LAYER1_MAIN_3544_112_2_17_22_26_30_2255 "L1 sim event not arrived!"
#define LAYER1_OUTPORT_191_112_2_17_22_26_31_2256 "L1:Start layer1_calibration thread!\n"
#define LAYER1_OUTPORT_222_112_2_17_22_26_31_2257 "L1:Start layer1_normal thread!\n"
#define LAYER1_OUTPORT_287_112_2_17_22_26_31_2258 "L1:The last operation is not finished!"
#define LAYER1_OUTPORT_293_112_2_17_22_26_31_2259 "L1 set task id %x L1API_GetAFC"
#define LAYER1_OUTPORT_340_112_2_17_22_26_31_2260 "L1:The last operation is not finished!"
#define LAYER1_OUTPORT_346_112_2_17_22_26_31_2261 "L1 set task id %x L1API_GetPA"
#define LAYER1_OUTPORT_380_112_2_17_22_26_32_2262 "L1:The last operation is not finished!"
#define LAYER1_OUTPORT_386_112_2_17_22_26_32_2263 "L1 set task id %x L1API_GetAGC"
#define LAYER1_OUTPORT_417_112_2_17_22_26_32_2264 "L1:The last operation is not finished!"
#define LAYER1_OUTPORT_422_112_2_17_22_26_32_2265 "L1 set task id %x L1API_GetBlockErrorRatio"
#define LAYER1_OUTPORT_529_112_2_17_22_26_32_2266 "Start DSP RX with max power"
#define LAYER1_OUTPORT_542_112_2_17_22_26_32_2267 "L1:Stop DSP RX with max power"
#define LAYER1_OUTPORT_617_112_2_17_22_26_32_2268 "L1:Start DSP TX with max power"
#define LAYER1_OUTPORT_630_112_2_17_22_26_32_2269 "L1:Stop DSP TX with max power"
#define LAYER1_OUTPORT_716_112_2_17_22_26_32_2270 "Start DSP TX with max power"
#define LAYER1_OUTPORT_729_112_2_17_22_26_32_2271 "Stop DSP TX with max power"
#define LAYER1_OUTPORT_821_112_2_17_22_26_33_2272 "L1:GPSVALIDATION audio is using!"
#define LAYER1_OUTPORT_828_112_2_17_22_26_33_2273 "L1:Other function code in overlay! "
#define LAYER1_OUTPORT_957_112_2_17_22_26_33_2274 "L1PagingTimer_RegisterCallback func 0x%x , num %d"
#define LAYER1_OUTPORT_986_112_2_17_22_26_33_2275 "L1PagingTimer_UnRegisterCallback func 0x%x "
#define LAYER1_OUTPORT_1013_112_2_17_22_26_33_2276 "L1PagingTimer_DoCallback num %d, tickcount %d "
#define LAYER1_OUTPORT_1052_112_2_17_22_26_33_2277 "L1PagingTimer_CountPlus, delta %d, last %d, pattern %d, next %d, curtick"
#define LAYER1_OUTPORT_1133_112_2_17_22_26_33_2278 "L1: MPHHandle_MPhSMSCBDiscardBlock wakeup dsp"
#define LAYER1_OUTPORT_1156_112_2_17_22_26_33_2279 "L1 cali mode %d,is_debug_on %d,arfcn 0x%x,band %d"
#define LAYER1_OUTPORT_1184_112_2_17_22_26_33_2280 "L1 "
#define LAYER1_OUTPORT_1201_112_2_17_22_26_33_2281 "L1: MPHHandle_MPhSMSCBDiscardBlock wakeup dsp"
#define LAYER1_SBC_113_112_2_17_22_26_34_2282 "L1:load GPS code in overlay.!"
#define LAYER1_SBC_116_112_2_17_22_26_34_2283 "OVERLAY_TEST: Overlay_code_base_ptr is %x"
#define LAYER1_SBC_117_112_2_17_22_26_34_2284 "OVERLAY_TEST: Overlay_Load_Code %x %x %x %x %x"
#define LAYER1_SBC_193_112_2_17_22_26_34_2285 "OVERLAY: overlay_context.overlay_mutex %x"
#define LAYER1_SBC_194_112_2_17_22_26_34_2286 "OVERLAY: overlay_context.mem_state %d"
#define LAYER1_SBC_195_112_2_17_22_26_34_2287 "OVERLAY: overlay_context.overlay_code_base_ptr %x"
#define LAYER1_SBC_227_112_2_17_22_26_34_2288 "SBC Function: L1_SBC_Config_Encoder "
#define LAYER1_SBC_241_112_2_17_22_26_34_2289 "SBC Function mutex: %x "
#define LAYER1_SBC_345_112_2_17_22_26_34_2290 "layer1_cmd: command  CmdQ_SBC_Config_Encoder "
#define LAYER1_SBC_371_112_2_17_22_26_34_2291 "SBC Function: L1_SBC_Encode_Frame %d %d %d"
#define LAYER1_SBC_373_112_2_17_22_26_34_2292 "SBC Function: L1_SBC_Encode_Frame"
#define LAYER1_SBC_552_112_2_17_22_26_35_2293 "layer1_cmd: command  CMDQ_TRANPCM_CONFIG "
#define LAYER1_SBC_568_112_2_17_22_26_35_2294 "layer1_cmd: command  CMDQ_TRANPCM_PROCESS "
#define LAYER1_VOICECALL_82_112_2_17_22_26_35_2295 "Transfer a frame data to DSP !!!! "
#define MPHHANDLE_303_112_2_17_22_26_36_2296 "l1 active meet response delay !!!"
#define MPHHANDLE_345_112_2_17_22_26_36_2297 "L1 deactive mask %x"
#define MPHHANDLE_378_112_2_17_22_26_36_2298 "L1 main card id %x"
#define MPHHANDLE_844_112_2_17_22_26_37_2299 "L1 pch allowed nv %x net %x"
#define MPHHANDLE_1278_112_2_17_22_26_38_2300 "L1 pwrswp state %d"
#define MPHHANDLE_1319_112_2_17_22_26_38_2301 "L1:calipost_param = 0x%x"
#define MPHHANDLE_1356_112_2_17_22_26_38_2302 "L1:band = %d,arfcn = 0x%x, cali mode = %x, cali param %x"
#define MPHHANDLE_1497_112_2_17_22_26_39_2303 "L1 pch allowed nv %x net %x"
#define MPHHANDLE_2071_112_2_17_22_26_40_2304 "L1 page mode req param3 %d"
#define MPHHANDLE_2420_112_2_17_22_26_40_2305 "L1 pch allowed nv %x net %x"
#define MPHHANDLE_3183_112_2_17_22_26_42_2306 "L1 pch allowed nv %x net %x"
#define MPHHANDLE_3416_112_2_17_22_26_42_2307 "L1: start_fn %d"
#define MPHHANDLE_3626_112_2_17_22_26_43_2308 "L1: chan assign start fn %d"
#define MPHHANDLE_3820_112_2_17_22_26_43_2309 "L1:chan_mode %d, is_change %d"
#define MPHHANDLE_3910_112_2_17_22_26_43_2310 "L1:Cipher mode %d,Cipher_algarithm %d,n"
#define MPHHANDLE_3915_112_2_17_22_26_43_2311 "L1:Cipher Key %d,%d,%d,%d,%d,%d,%d,%dn"
#define MPHHANDLE_3970_112_2_17_22_26_43_2312 "L1: Freq Change start fn %d"
#define MPHHANDLE_4037_112_2_17_22_26_44_2313 "L1: chan mode %d!\n"
#define MPHHANDLE_4152_112_2_17_22_26_44_2314 "L1 init handover cell fail! "
#define MPHHANDLE_4159_112_2_17_22_26_44_2315 "L1 handover cell id %d,  arfcn index %x "
#define MPHHANDLE_4198_112_2_17_22_26_44_2316 "L1 handover to resync pending cell arfcn index %x"
#define MPHHANDLE_4218_112_2_17_22_26_44_2317 "L1:Handover fail, cell_id = %d,bsic %d,target bsic %d"
#define MPHHANDLE_4275_112_2_17_22_26_44_2318 "L1:cipher_mode %d,cipher_algorithm %d"
#define MPHHANDLE_4591_112_2_17_22_26_45_2319 "L1:set cell %d to NOT_SYN_CALL"
#define MPHHANDLE_4791_112_2_17_22_26_45_2320 "L1:No TCH avtive,ignor LOOP cmd"
#define MPHHANDLE_4825_112_2_17_22_26_45_2321 "L1:rec disable loop back %d!"
#define MPHHANDLE_4844_112_2_17_22_26_45_2322 "L1:rec loop back cmd!"
#define MPHHANDLE_5050_112_2_17_22_26_46_2323 "L1: MPHHandle_MphSMSCBStartReq  cbch_required_bitmap %d"
#define MPHHANDLE_5172_112_2_17_22_26_46_2324 "L1: MPHHandle_MPhSMSCBDiscardBlock %d"
#define MPHHANDLE_5189_112_2_17_22_26_46_2325 "L1: MPHHandle_MPhSMSCBDiscardBlock wakeup dsp"
#define MPHHANDLE_5240_112_2_17_22_26_46_2326 "L1 uiCellIndex is INVALID_CELL! %x"
#define MPHHANDLE_5261_112_2_17_22_26_46_2327 "L1 cell state is not sync %x, %x!"
#define MPHHANDLE_5344_112_2_17_22_26_46_2328 "L1 scell "
#define MPHHANDLE_5355_112_2_17_22_26_46_2329 "L1 valid cell! 0x%x"
#define MPHHANDLE_5369_112_2_17_22_26_46_2330 "L1 searching %x"
#define PHHANDLE_369_112_2_17_22_26_47_2331 "L1 enable codec device"
#define PHHANDLE_401_112_2_17_22_26_47_2332 "layer1_cmd: audio enable, is_pre %d is_enable %d"
#define PHHANDLE_624_112_2_17_22_26_48_2333 "L1 test mode is not supportted now!"
#define PHHANDLE_638_112_2_17_22_26_48_2334 "L1:PHHandle_AppPhMp3A2dpReq:Time = %d ms"
#define PHHANDLE_794_112_2_17_22_26_48_2335 "L1 dl rlc mode %x"
#define PHHANDLE_806_112_2_17_22_26_48_2336 "L1:Switch Band: ma_arfcn_index = 0x%x, pdch_band = %d!"
#define PHHANDLE_933_112_2_17_22_26_48_2337 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_1055_112_2_17_22_26_48_2338 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_1180_112_2_17_22_26_49_2339 "L1 dl rlc mode %x"
#define PHHANDLE_1190_112_2_17_22_26_49_2340 "L1:Switch Band1: ma_arfcn_index = 0x%x, pdch_band=%d!"
#define PHHANDLE_1293_112_2_17_22_26_49_2341 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_1391_112_2_17_22_26_49_2342 "L1 dl rlc mode %x"
#define PHHANDLE_1496_112_2_17_22_26_49_2343 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_1591_112_2_17_22_26_49_2344 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_1664_112_2_17_22_26_50_2345 "L1 dl rlc mode %x"
#define PHHANDLE_1765_112_2_17_22_26_50_2346 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_1865_112_2_17_22_26_50_2347 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_1954_112_2_17_22_26_50_2348 "L1 dl rlc mode %x"
#define PHHANDLE_1995_112_2_17_22_26_50_2349 "L1:Switch Band2,ma_arfcn_index = 0x%x, pdch_band = %d!"
#define PHHANDLE_2098_112_2_17_22_26_51_2350 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_2306_112_2_17_22_26_51_2351 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_2532_112_2_17_22_26_51_2352 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_2688_112_2_17_22_26_52_2353 "split cyccle %d"
#define PHHANDLE_2781_112_2_17_22_26_52_2354 "L1: use old ta %d"
#define PHHANDLE_2826_112_2_17_22_26_52_2355 "L1: use old tai %d"
#define PHHANDLE_3042_112_2_17_22_26_52_2356 "Switch Band5: ma_arfcn_index = 0x%x, pdch_band = %d!"
#define PHHANDLE_3123_112_2_17_22_26_53_2357 "L1:set cell %d to NOT_SYN_CALL"
#define PHHANDLE_3248_112_2_17_22_26_53_2358 "L1:set cell %d to NOT_SYN_CALL"
#define PHHANDLE_3321_112_2_17_22_26_53_2359 "L1:set cell %d to NOT_SYN_CALL"
#define PHHANDLE_3584_112_2_17_22_26_53_2360 "L1: open loop back function %x"
#define PHHANDLE_3603_112_2_17_22_26_53_2361 "L1: close loop back function"
#define PHHANDLE_3721_112_2_17_22_26_54_2362 "L1 mag: phhandle type %d"
#define RESQ_234_112_2_17_22_26_54_2363 "L1:Rx a PBCCH in BCCH_CAMPED state"
#define RESQ_245_112_2_17_22_26_54_2364 "L1:BCCH_CAMPED,C_t %d!\n"
#define RESQ_327_112_2_17_22_26_55_2365 "L1:PBCCH_CAMPED state,Discard a block,Ch_tag %d!\n"
#define RESQ_369_112_2_17_22_26_55_2366 "L1:Rx a PPCH in PLMN_CAMPED state"
#define RESQ_383_112_2_17_22_26_55_2367 "L1: Rx a SCH in PLMN_CAMPED state"
#define RESQ_406_112_2_17_22_26_55_2368 "L1:Rx a PBCCH in BCCH_CAMPED state"
#define RESQ_417_112_2_17_22_26_55_2369 "L1:PLMN_CAMPED state,Discard a block,Ch_tag %d!\n"
#define RESQ_447_112_2_17_22_26_55_2370 "L1:Rx a CBCH in PG_REORG_CAMPED state"
#define RESQ_506_112_2_17_22_26_55_2371 "L1: Rx a SCH in PG_REORG state"
#define RESQ_516_112_2_17_22_26_55_2372 "L1:PG_REORG CAMPED,Discard a block,Ch_tag %d!\n"
#define RESQ_569_112_2_17_22_26_55_2373 "L1:Rx a PBCCH in RACHAGCH state"
#define RESQ_576_112_2_17_22_26_55_2374 "L1:Rx a PPCH in RACHAGCH state"
#define RESQ_586_112_2_17_22_26_55_2375 "L1:Rx a PAGCH in RACHAGCH state"
#define RESQ_596_112_2_17_22_26_55_2376 "L1: Rx a SCH in RACHAGCH state"
#define RESQ_651_112_2_17_22_26_55_2377 "L1:Rx a PPCH in RACHAGCH state"
#define RESQ_668_112_2_17_22_26_55_2378 "L1: Rx a SCH in PRACHAGCH state"
#define RESQ_674_112_2_17_22_26_55_2379 "L1:Discard a block,Ch_tag %d!\n"
#define RESQ_754_112_2_17_22_26_55_2380 "L1:SDCCHTCH state, Discard a block,Ch_tag %d!\n"
#define RESQ_795_112_2_17_22_26_56_2381 "L1:Rx a PTCCH in TBF state"
#define RESQ_883_112_2_17_22_26_56_2382 "L1:Rx a BCCH in traffic"
#define RESQ_1170_112_2_17_22_26_56_2383 "L1: FrameInstr Queue DSP Over Run"
#define RESQ_1175_112_2_17_22_26_56_2384 "L1: Cmd Queue DSP Over Run"
#define RESQ_1180_112_2_17_22_26_56_2385 "L1: TxData Queue DSP Over Run"
#define RESQ_1185_112_2_17_22_26_56_2386 "L1: Resp Queue DSP Over RunU"
#define RESQ_1190_112_2_17_22_26_56_2387 "L1: RxData Queue DSP Over Run"
#define RESQ_1196_112_2_17_22_26_56_2388 "L1: PowerMon Queue DSP Over Run"
#define RESQ_1201_112_2_17_22_26_56_2389 "L1: MeasReport Queue DSP Over Run"
#define RESQ_1206_112_2_17_22_26_56_2390 "L1: BlockSend Queue DSP Over Run"
#define RESQ_1272_112_2_17_22_26_56_2391 "L1 bsic fail:  %x, is_sch_conflict %d,is_freqoffset_exist %d"
#define RESQ_1425_112_2_17_22_26_57_2392 "L1 bsic fail:  %x, fail count %d,is_sch_conflict %d,is_freqoffset_exist %d"
#define RESQ_1534_112_2_17_22_26_57_2393 "L1 handover cell search fail !%x"
#define RESQ_1614_112_2_17_22_26_57_2394 "L1:WakeUp! %d,%d"
#define RESQ_1641_112_2_17_22_26_57_2395 "L1:WakeUp! %d,%d"
#define RESQ_1666_112_2_17_22_26_57_2396 "L1:WakeUp! %d,%d"
#define RESQ_1713_112_2_17_22_26_57_2397 "L1 wakeup resp2"
#define RESQ_1746_112_2_17_22_26_57_2398 "L1:WakeUp in idle! %d,%d"
#define RESQ_1769_112_2_17_22_26_57_2399 "L1:WakeUp in power sweep! %d,%d"
#define RESQ_1922_112_2_17_22_26_58_2400 "L1:Q start fn %d ,fn_curr %d,elapse %d\n"
#define RESQ_2017_112_2_17_22_26_58_2401 "L1:Q start fn %d ,fn_curr %d,elapse %d\n"
#define RESQ_2166_112_2_17_22_26_58_2402 "L1:WakeUp pbcch! %d,%d"
#define RESQ_2267_112_2_17_22_26_58_2403 "L1:Q start fn %d ,fn_curr %d,elapse %d\n"
#define RESQ_2426_112_2_17_22_26_59_2404 "L1:Q start fn %d ,fn_curr %d,elapse %d\n"
#define RESQ_2516_112_2_17_22_26_59_2405 "L1:WakeUp! %d,%d"
#define RESQ_2592_112_2_17_22_26_59_2406 "L1:Q start fn %d ,fn_curr %d,elapse %d\n"
#define RESQ_2728_112_2_17_22_26_59_2407 "L1 scell adjust %x, %x"
#define RESQ_2734_112_2_17_22_26_59_2408 "L1 ncell adjust %x, %x; ncell %x, scell %x"
#define RESQ_2858_112_2_17_22_27_0_2409 "L1: echo_testing_CNF!"
#define RESQ_3276_112_2_17_22_27_0_2410 "L1:State:SBC_CONFIG,Time = %d ms"
#define RESQ_3287_112_2_17_22_27_0_2411 "L1:State:SBC_ENCODE,Time = %d ms"
#define RESQ_3299_112_2_17_22_27_0_2412 "L1:State:PCM_TRANS,Time = %d ms"
#define RESQ_3346_112_2_17_22_27_1_2413 "L1:State:NONE_IN_USE,left_ptr=0x%x,right_ptr=0x%x,length = %d,Time = %d ms"
#define RESQ_3417_112_2_17_22_27_1_2414 "L1:Get SBC Frame.State:NONE_IN_USE,frame_ptr=0x%x,frame_length=0x%x,frame_num=%d, Time = %d ms"
#define RXLEVMON_440_112_2_17_22_27_2_2415 "L1 ba count %d %d, %d,total %d,%d"
#define RXLEVMON_513_112_2_17_22_27_2_2416 "L1 main card id , card id num %d"
#define RXLEVMON_624_112_2_17_22_27_2_2417 "L1 ba count de %d %d, %d,total %d,%d"
#define RXLEVMON_886_112_2_17_22_27_3_2418 "L1 not permit %x"
#define RXLEVMON_1101_112_2_17_22_27_3_2419 "L1:pwr_swp saturate with low gain, arfcn_index %d"
#define RXLEVMON_1147_112_2_17_22_27_3_2420 "L1: meas_ptr->is_valid is wrong!"
#define RXLEVMON_1208_112_2_17_22_27_3_2421 "L1:sat_low arfcn %x, , num %d"
#define RXLEVMON_1263_112_2_17_22_27_3_2422 "L1:Monitor saturate with low gain, arfcn_index %d"
#define RXLEVMON_1276_112_2_17_22_27_3_2423 "L1:sat_hi arfcn %x, num %d"
#define RXLEVMON_1293_112_2_17_22_27_3_2424 "L1: meas_ptr->is_valid is wrong! \n"
#define RXLEVMON_1471_112_2_17_22_27_4_2425 "L1 ba not valid"
#define RXLEVMON_1497_112_2_17_22_27_4_2426 "L1 warning: can't get valid ba"
#define RXLEVMON_1734_112_2_17_22_27_4_2427 "L1: IT's A WRONG g_measurement_ba_ptr->ba_table_state.band_ind!"
#define RXLEVMON_1766_112_2_17_22_27_4_2428 "full band swp: arfcn_index(%x) rxlev %x times "
#define RXLEVMON_1812_112_2_17_22_27_5_2429 "full band swp: arfcn_index(%x) rxlev %x times "
#define RXLEVMON_1819_112_2_17_22_27_5_2430 "L1:now %d,n1 %d,n2 %d"
#define RXLEVMON_1968_112_2_17_22_27_5_2431 "L1:rla %d,sum %d,count %d"
#define RXLEVMON_2039_112_2_17_22_27_5_2432 "L1:t %d  %x,%x,%x,%x,%x,%x;%x,%x,%x,%x,%x,%x"
#define RXLEVMON_2083_112_2_17_22_27_5_2433 "L1:idle  %d, %d, %d;%x,%x,%x,%x,%x,%x;%x,%x,%x,%x,%x,%x;%x,%x,%x,%x,%x,%x"
#define RXLEVMON_2153_112_2_17_22_27_5_2434 "Layer1 %x turn strong"
#define RXLEVMON_2590_112_2_17_22_27_6_2435 "powersweep duration %d"
#define RXLEVMON_2735_112_2_17_22_27_6_2436 "layer1: %x search fail too much, needn't sync"
#define RXLEVMON_2749_112_2_17_22_27_6_2437 "L1: This arfcn did not in current BA , arfcn_index %d \n"
#define RXLEVMON_2794_112_2_17_22_27_6_2438 "L1 the arfcn index is not in ba %x"
#define RXLEVMON_2799_112_2_17_22_27_6_2439 "L1 invalid arfcn index %x"
#define RXLEVMON_2921_112_2_17_22_27_7_2440 "L1 ba table is invalid!"
#define RXLEVMON_3168_112_2_17_22_27_7_2441 "L1 card_relation erro! arfcn_index %x, type %d"
#define RXLEVMON_3173_112_2_17_22_27_7_2442 "L1 the cell is not in ba! arfcn_index %x"
#define RXLEVMON_3288_112_2_17_22_27_7_2443 "L1 the arfcn is in old ba arfcn_index %x, type %d"
#define RXLEVMON_3293_112_2_17_22_27_7_2444 "L1 the arfcn is in old ba! arfcn_index %x"
#define RXLEVMON_3324_112_2_17_22_27_7_2445 "L1 the arfcn is in old ba arfcn_index %x, type %d"
#define RXLEVMON_3329_112_2_17_22_27_7_2446 "L1 the arfcn is in old ba! arfcn_index %x"
#define RXLEVMON_3631_112_2_17_22_27_8_2447 "L1:forgetting factor=%d"
#define RXLEVMON_3822_112_2_17_22_27_9_2448 "L1:input %x,%x ; output  %x, %x ; %x, %x;e %x,r %x,slot %x,rx_tfi %x,%x,slotmask%x,loopback %d"
#define RXLEVMON_3977_112_2_17_22_27_9_2449 "L1 RxLevMon_GetTestArfcnMoniParam rxlev_result %d, rxdbm %d, count %d"
#define RXLEVMON_4042_112_2_17_22_27_9_2450 "L1: s_save_prev_ba_table is full!"
#define RXLEVMON_4143_112_2_17_22_27_9_2451 "L1: RRA_MPH_RF_SCAN_IND!"
#define RXLEVMON_4215_112_2_17_22_27_9_2452 "L1 quick pwrswp normal finished!"
#define RXLEVMON_4221_112_2_17_22_27_9_2453 "L1 quick pwrswp abort no valid ba!"
#define RXLEVMON_4226_112_2_17_22_27_9_2454 "L1 pwr ++ index %x, %x"
#define RXLEVMON_4350_112_2_17_22_27_10_2455 "L1 add a scell card id %d!%x"
#define RXLEVMON_4376_112_2_17_22_27_10_2456 "L1 invalid scell ba_index!%x arfcn_index %x"
#define RXLEVMON_4493_112_2_17_22_27_10_2457 "L1 cell is omitted %x"
#define SHAREDMEM_PROC_308_112_2_17_22_27_11_2458 "L1:something wrong when to read status words!!"
#define SHAREDMEM_PROC_338_112_2_17_22_27_11_2459 "something is wrong when to write status words 5 times!!"
#define SHAREDMEM_PROC_396_112_2_17_22_27_11_2460 "LAYER1: The powermoni queue is empty! \n"
#define SHAREDMEM_PROC_500_112_2_17_22_27_11_2461 "L1: New ReducedFn 0x%x"
#define SHAREDMEM_PROC_822_112_2_17_22_27_12_2462 "L1: send chanchange sig"
#define SHAREDMEM_PROC_983_112_2_17_22_27_12_2463 "L1: The rx queue is empty!"
#define SHAREDMEM_PROC_1303_112_2_17_22_27_13_2464 "SetTxFramePcl, base address 0x%x, writein 0x%x"
#define SHAREDMEM_PROC_1469_112_2_17_22_27_13_2465 "L1 cancel frame after wake up!, %x"
#define SHAREDMEM_PROC_1711_112_2_17_22_27_14_2466 "L1:Wake up DSP, and sleep frame is at out + 1!!"
#define SHAREDMEM_PROC_1874_112_2_17_22_27_14_2467 "L1:still Waiting for wakeup's response!"
#define SHAREDMEM_PROC_1929_112_2_17_22_27_14_2468 "L1 cancel search :%x"
#define SHAREDMEM_PROC_1967_112_2_17_22_27_14_2469 "SharedMemProc_CancelObsoleteFrames: total = %d!"
#define SHAREDMEM_PROC_2042_112_2_17_22_27_14_2470 "L1 find ask more flag more than once!"
#define SHAREDMEM_PROC_2197_112_2_17_22_27_15_2471 "L1 cancel margin %d, iscancel %d, newin %d, oldin %d, out %d, askmore %d, newaskmore %d"
#define SHAREDMEM_PROC_2236_112_2_17_22_27_15_2472 "L1:distance between in and out is 0"
#define SHAREDMEM_PROC_2330_112_2_17_22_27_15_2473 "SharedMemProc_CancelRemainingFrames:channel_mode_update"
#define SHAREDMEM_PROC_2337_112_2_17_22_27_15_2474 "SharedMemProc_CancelRemainingFrames:TA update"
#define SHAREDMEM_PROC_2344_112_2_17_22_27_15_2475 "SharedMemProc_CancelRemainingFrames:power update"
#define SHAREDMEM_PROC_2352_112_2_17_22_27_15_2476 "SharedMemProc_CancelRemainingFrames:channel change"
#define SHAREDMEM_PROC_2363_112_2_17_22_27_15_2477 "SharedMemProc_CancelRemainingFrames:gprs ta update"
#define SHAREDMEM_PROC_2371_112_2_17_22_27_15_2478 "SharedMemProc_CancelRemainingFrames:gprs usf update"
#define SHAREDMEM_PROC_2377_112_2_17_22_27_15_2479 "SharedMemProc_CancelRemainingFrames:gprs channel update"
#define SHAREDMEM_PROC_2480_112_2_17_22_27_15_2480 "L1 error: UpdateRxDataOutPtr rx buffer empty, rxdata in %d, rxdata out %d"
#define SHAREDMEM_PROC_3830_112_2_17_22_27_18_2481 "L1:divide slp.slp cnt 0x%x,sec slp 0x%x,out %x,crr t:0x%x,nea t:0x%x"
#define SHAREDMEM_PROC_4080_112_2_17_22_27_19_2482 "L1:cancel rx_tx_rxlev -> rx_tx"
#define SHAREDMEM_PROC_4096_112_2_17_22_27_19_2483 "L1:cancel rx_rxlev -> rx"
#define SHAREDMEM_PROC_4105_112_2_17_22_27_19_2484 "L1:cancel tch_rx_tx_rxlev -> tch_rx_tx"
#define SHAREDMEM_PROC_4114_112_2_17_22_27_19_2485 "L1:cancel rxlev_tx_rxlev -> rxlew_rx"
#define SHAREDMEM_PROC_4134_112_2_17_22_27_19_2486 "L1:cancel rxlev rxlev_rxlev -> nop"
#define SHAREDMEM_PROC_4605_112_2_17_22_27_20_2487 "download param %ld, %ld"
#define SHAREDMEM_PROC_4947_112_2_17_22_27_21_2488 "L1:test_count = %d"
#define SHAREDMEM_PROC_5186_112_2_17_22_27_21_2489 "L1 relation:%x,%x,%x,%x,%x,%x,%x,%x"
#define SHAREDMEM_PROC_5284_112_2_17_22_27_21_2490 "LAYER1:SharedMemProc_InitializeNxp"
#define SHAREDMEM_PROC_5299_112_2_17_22_27_21_2491 "L1:SharedMemProc_SetDAPCMDate"
#define SHAREDMEM_PROC_5312_112_2_17_22_27_21_2492 "L1:SharedMemProc_SetNetPCMDate"
#define SYSTEMINFO_886_112_2_17_22_27_24_2493 "L1: freq_redefine changed!!"
#define SYSTEMINFO_951_112_2_17_22_27_24_2494 "L1: freq_redefine changed!!"
#define SYSTEMINFO_1185_112_2_17_22_27_24_2495 "curr_fn %d,delta_fn %d,change delay %d,start_fn %d"
#define SYSTEMINFO_1374_112_2_17_22_27_25_2496 " Current chan same as next chan"
#define SYSTEMINFO_1379_112_2_17_22_27_25_2497 " Current chan different from next chan"
#define SYSTEMINFO_1436_112_2_17_22_27_25_2498 "L1:loop_back paras: p1= %d, p2= %d!\n"
#define SYSTEMINFO_1936_112_2_17_22_27_26_2499 "L1:NOTE: NOT ANY response from DSP for TCH search!! fn %d"
#define SYSTEMINFO_2141_112_2_17_22_27_26_2500 "L1: cbch schedule start tb %d, tb"
#define SYSTEMINFO_2148_112_2_17_22_27_26_2501 "L1:card id: %d,first schedule s_fn 0x%x,block num %d"
#define SYSTEMINFO_2213_112_2_17_22_27_26_2502 "L1: schedule blocki %d"
#define SYSTEMINFO_2236_112_2_17_22_27_26_2503 "L1:current schedule out 0x%x, blocki %d"
#define SYSTEMINFO_2241_112_2_17_22_27_26_2504 "L1:end schedule and start receive all block 0-4"
#define SYSTEMINFO_2687_112_2_17_22_27_27_2505 "L1 is first for tch %d"
#define SYSTEMINFO_2695_112_2_17_22_27_27_2506 "L1 set frame type: is_debug_on %d,type %d, arfcn %x,gain %x"
#define SYSTEMINFO_2719_112_2_17_22_27_27_2507 "L1 frametype %d, is_debug_mode %d"
#define TAPWR_124_112_2_17_22_27_28_2508 "SetTxFramePcl, pwr_frame_ind %d, pwr_buf_ind %d, pcl[%d, %d, %d, %d]"
#define TAPWR_226_112_2_17_22_27_28_2509 "L1: TaPwr_GetTxPwrCapabilty not suitable band\n"
#define TAPWR_258_112_2_17_22_27_28_2510 "L1: SetMaxOutPutPowerLevel not suitable band\n"
#define LAYER1_CALL_MAC_PH_150_112_2_17_22_27_31_2511 "\n s_polling_num = %d"
#define LAYER1_CALL_MAC_PH_159_112_2_17_22_27_31_2512 "polling data send_type = %d"
#define LAYER1_CALL_MAC_PH_172_112_2_17_22_27_31_2513 "polling data send_type = %d"
#define PROCESS_MSG_876_112_2_17_22_27_34_2514 "Script file is wrong!"
#define PROCESS_MSG_1774_112_2_17_22_27_36_2515 "L1:this rxlev data line is invalid,please update!"
#define PROCESS_MSG_1824_112_2_17_22_27_36_2516 "L1:this sch data line is invalid,please update!"
#define TEST_HANDLE_MSG_423_112_2_17_22_27_38_2517 "sleep counter =%d\n"
#define TEST_HANDLE_MSG_809_112_2_17_22_27_39_2518 "sleep counter =%d\n"
#define TEST_HANDLE_MSG_1507_112_2_17_22_27_40_2519 "LAYER1: RFoperation is wrong!\n"
#define TEST_HANDLE_MSG_1730_112_2_17_22_27_41_2520 "\n SARAH test_search_response arfcn %d,T1 %d,T2 %d,T3P %d,BSIC %d"
#define TEST_HANDLE_MSG_1785_112_2_17_22_27_41_2521 "\n SARAH test_send_SCH_data_to_MCU T1 %d,T2 %d,T3P %d,BSIC %d"
#define TEST_HANDLE_MSG_1977_112_2_17_22_27_41_2522 "wake up:sleep counter %d\n"
#define TEST_RW_SHARED_MEM_266_112_2_17_22_27_43_2523 "L1:command is NULL!!!"
#define TEST_RW_SHARED_MEM_526_112_2_17_22_27_43_2524 "Rx data from channel has been read over!"
#define LAYER1_AUDIO_SIMULATOR_426_112_2_17_22_27_46_2525 "peter:LAYER1_PlayCallback no data....!!!!"
#define SHAREDMEM_PROC_SIMULATOR_20_112_2_17_22_27_47_2526 "Add_One_CMD to share mem faile! queue full"
#define SHAREDMEM_PROC_SIMULATOR_53_112_2_17_22_27_47_2527 "Add_One_CMD to share mem faile! queue full"
#define MMI_OSBRIDGE_39_112_2_17_22_27_48_2528 "MMI : alloc signal error, signal szie = %d"
#define DSP_OPERATE_1367_112_2_17_22_27_52_2529 "0x8B000038 value is %x"
#define LAYER1_DSP_DOWNLOAD_485_112_2_17_22_46_15_2530 "L1:met error in unzip process in download dsp"
#define LAYER1_DSP_DOWNLOAD_523_112_2_17_22_46_16_2531 "L1:dsp code address is %x"
#define LAYER1_DSP_DOWNLOAD_914_112_2_17_22_46_16_2532 "L1:dsp code has inited!"
#define LAYER1_DSP_DOWNLOAD_928_112_2_17_22_46_16_2533 "L1:Use dsp code in NV!!, chip type 0x%x"
#define LAYER1_DSP_DOWNLOAD_929_112_2_17_22_46_16_2534 "codesize = %d"
#define LAYER1_DSP_DOWNLOAD_938_112_2_17_22_46_16_2535 "L1:Use dsp code in NV!!, chip type 0x%x"
#define LAYER1_DSP_DOWNLOAD_939_112_2_17_22_46_16_2536 "codesize = %d"
#define LAYER1_DSP_DOWNLOAD_945_112_2_17_22_46_16_2537 "L1:Use dsp_pdata.c!!, chip type 0x%x"
#define LAYER1_DSP_DOWNLOAD_1378_112_2_17_22_46_17_2538 "L1_DSP lzma_load error %d"
#define LOGSAVE_BUFFER_73_112_2_17_22_46_18_2539 "logsave: arm buffer addr 0x%x"
#define LOGSAVE_BUFFER_90_112_2_17_22_46_18_2540 "logsave: dsp buffer addr 0x%x"
#define LOGSAVE_BUFFER_107_112_2_17_22_46_18_2541 "logsave: iq buffer addr 0x%x"
#define LOGSAVE_BUFFER_505_112_2_17_22_46_19_2542 "LOGSAVE BUFF: LogSaveBuff_Assert_Info_Func no size to alloc ass str!"
#define LOGSAVE_BUFFER_512_112_2_17_22_46_19_2543 "LOGSAVE BUFF: LogSaveFM_InitFile, assert info error, finish_time %d"
#define LOGSAVE_BUFFER_580_112_2_17_22_46_19_2544 "LOGSAVE FM: LogSaveFM_InitFile, assert memory error, finish_time %d"
#define LOGSAVE_BUFFER_584_112_2_17_22_46_19_2545 "LOGSAVE BUFF: start save mem time: %d"
#define LOGSAVE_BUFFER_589_112_2_17_22_46_19_2546 "LOGSAVE BUFF: end sdram time: %d"
#define LOGSAVE_BUFFER_631_112_2_17_22_46_19_2547 "LOGSAVE BUFF: end thread info time: %d"
#define LOGSAVE_BUFFER_636_112_2_17_22_46_19_2548 "LOGSAVE BUFF: end shared mem time: %d"
#define LOGSAVE_BUFFER_641_112_2_17_22_46_19_2549 "LOGSAVE BUFF: end internal mem time: %d"
#define LOGSAVE_BUFFER_668_112_2_17_22_46_19_2550 "LOGSAVE BUFF: end registers time: %d"
#define LOGSAVE_BUFFER_712_112_2_17_22_46_19_2551 "LOGSAVE BUFF: dsp is running"
#define LOGSAVE_BUFFER_806_112_2_17_22_46_20_2552 "LOGSAVE BUFF: dump dsp mem, dump_count %d"
#define LOGSAVE_BUFFER_828_112_2_17_22_46_20_2553 "LOGSAVE BUFF: dump dsp mem, dump_count %d"
#define LOGSAVE_BUFFER_841_112_2_17_22_46_20_2554 "LOGSAVE BUFF: end save mem time: %d, total length %d"
#define LOGSAVE_BUFFER_900_112_2_17_22_46_20_2555 "IQ: LogSaveBuff_StartIQRecord, arfcn %d, block_count %d, block_length %d, para1 0x%x"
#define LOGSAVE_CONFIG_166_112_2_17_22_46_20_2556 "LOGSAVE CF: nv iq switch wrong 0x%x"
#define LOGSAVE_CONFIG_170_112_2_17_22_46_20_2557 "LOGSAVE CF: nv iq switch setting 0x%x"
#define LOGSAVE_CONFIG_228_112_2_17_22_46_20_2558 "LOGSAVE CF: nv iq switch setting 0x%x"
#define LOGSAVE_FILEMANAGE_148_112_2_17_22_46_21_2559 "LOGSAVE FM: g_lost_message_num arm log: %d, dsp log: %d, iq_data: %d, is_assert %d, start_time %d"
#define LOGSAVE_FILEMANAGE_180_112_2_17_22_46_21_2560 "LOGSAVE FM: finish_time %d"
#define LOGSAVE_MAIN_159_112_2_17_22_46_24_2561 "LOGSAVE: Close all"
#define LOGSAVE_OUTPUT_247_112_2_17_22_46_25_2562 "LOGSAVE: assert not in thread"
#define LOGSAVE_OUTPUT_253_112_2_17_22_46_25_2563 "LOGSAVE: is layer1 assert, stop dsp, cmd done "
#define LOGSAVE_OUTPUT_345_112_2_17_22_46_26_2564 "LOGSAVE: start"
#define LOGSAVE_OUTPUT_366_112_2_17_22_46_26_2565 "LOGSAVE: stop"
#define CC_ENTITY_570_112_2_17_22_46_27_2566 "CC: send start dtmf at %d state"
#define CC_ENTITY_614_112_2_17_22_46_27_2567 "CC: send stop dtmf at %d state"
#define CC_MSG_1487_112_2_17_22_46_35_2568 "CC: The message is valid CC message. \n"
#define CC_MSG_1491_112_2_17_22_46_35_2569 "CC: The message is invalid CC message. \n"
#define CC_MSG_1766_112_2_17_22_46_36_2570 "CC: the IE is not valid IE of Setup message. \n"
#define CC_MSG_1773_112_2_17_22_46_36_2571 "CC: Decode Setup message and find serious ie error.\n"
#define CC_MSG_2031_112_2_17_22_46_36_2572 "CC: the IE is not valid IE of Setup message. \n"
#define CC_MSG_2063_112_2_17_22_46_36_2573 "CC: Progress message is too short to contain mandatory IE.\n"
#define CC_MSG_2083_112_2_17_22_46_36_2574 "CC: Progress message mandatory IE error.\n"
#define CC_MSG_2096_112_2_17_22_46_36_2575 "CC: Progress Message Ie sequence error.\n"
#define CC_MSG_2135_112_2_17_22_46_36_2576 "CC: Facility message is too short to contain mandatory IE.\n"
#define CC_MSG_2169_112_2_17_22_46_37_2577 "CC: Notify message is too short to contain mandatory IE.\n"
#define CC_MSG_2302_112_2_17_22_46_37_2578 "CC: Error IE in Call Proceeding Message. \n"
#define CC_MSG_2384_112_2_17_22_46_37_2579 "CC: Error IE in Alerting Message. \n"
#define CC_MSG_2481_112_2_17_22_46_37_2580 "CC: Error IE in Connect Message. \n"
#define CC_MSG_2590_112_2_17_22_46_37_2581 "CC: Error IE in Disconnect Message. \n"
#define CC_MSG_2690_112_2_17_22_46_38_2582 "CC: Error IE in Modify Message. \n"
#define CC_MSG_2786_112_2_17_22_46_38_2583 "CC: Error IE in Modify Complete Message. \n"
#define CC_MSG_2885_112_2_17_22_46_38_2584 "CC: Error IE in Modify Reject Message. \n"
#define CC_MSG_2925_112_2_17_22_46_38_2585 "CC: Message is too short.\n"
#define CC_MSG_2966_112_2_17_22_46_38_2586 "CC: Message is too short.\n"
#define CC_MSG_3205_112_2_17_22_46_39_2587 "CC: Error IE in Release Message. \n"
#define CC_MSG_3289_112_2_17_22_46_39_2588 "CC: Error IE in Release Complete Message. \n"
#define CC_MSG_3436_112_2_17_22_46_39_2589 "CC: Cc-Establishment message is too short to contain mandatory IE.\n"
#define CC_MSG_3472_112_2_17_22_46_39_2590 "CC: Recall message is too short to contain mandatory IE.\n"
#define CC_MSG_4286_112_2_17_22_46_41_2591 "CC: bc ie is type error"
#define CC_MSG_4328_112_2_17_22_46_41_2592 "CC: CheckIE_Type4 iei = %d iei_type = %d"
#define CC_MSG_4410_112_2_17_22_46_41_2593 "CC: progress ie is type error"
#define CC_MSG_4813_112_2_17_22_46_42_2594 "CC: progress ie's type error"
#define CC_MSG_4865_112_2_17_22_46_42_2595 "CC: progress ie's type error"
#define CC_MSG_4925_112_2_17_22_46_42_2596 "CC: backup bc ie is type error"
#define CC_MSG_4968_112_2_17_22_46_42_2597 "CC: allowed action ie is type error"
#define CC_NETMSG_185_112_2_17_22_46_43_2598 "cc GetStatus same state = %d"
#define CC_NETMSG_359_112_2_17_22_46_43_2599 "CC_START_DTMF_ACKNOWLEDGE state=%d"
#define CC_NETMSG_404_112_2_17_22_46_43_2600 "CC_USER_INFOMATION state=%d"
#define CC_NETMSG_449_112_2_17_22_46_44_2601 "CC_CONGESTION_CONTROL state=%d"
#define CC_NETMSG_491_112_2_17_22_46_44_2602 "CC_FACILITY state=%d"
#define CC_NETMSG_533_112_2_17_22_46_44_2603 "CC_NOTIFY state=%d"
#define CC_NETMSG_575_112_2_17_22_46_44_2604 "CC_STATUS state=%d"
#define CC_NETMSG_640_112_2_17_22_46_44_2605 "CC_START_DTMF_ACKNOWLEDGE state=%d"
#define CC_NETMSG_698_112_2_17_22_46_44_2606 "CC_NOTIFY state=%d"
#define CC_NETMSG_740_112_2_17_22_46_44_2607 "CC_RELEASE_COMPLETE state=%d"
#define CC_NETMSG_885_112_2_17_22_46_44_2608 "CC_STATUS state=%d"
#define CC_NETMSG_930_112_2_17_22_46_44_2609 "CC_STATUS state=%d"
#define CC_NETMSG_1530_112_2_17_22_46_46_2610 "CC_RELEASE_COMPLETE state=%d"
#define CC_NETMSG_1585_112_2_17_22_46_46_2611 "CC_STATUS state=%d"
#define CC_TIMER_81_112_2_17_22_46_53_2612 "CC_SetTimer timer_type = %d"
#define CC_TIMER_110_112_2_17_22_46_53_2613 "CC_KillTimer timer_type = %d"
#define CCMSG_1867_112_2_17_22_46_57_2614 "CC: The message is valid CC message. \n"
#define CCMSG_1871_112_2_17_22_46_57_2615 "CC: The message is invalid CC message. \n"
#define CCMSG_2222_112_2_17_22_46_58_2616 "CC: the IE is not valid IE of Setup message. \n"
#define CCMSG_2229_112_2_17_22_46_58_2617 "CC: Decode Setup message and find serious ie error.\n"
#define CCMSG_2558_112_2_17_22_46_59_2618 "CC: the IE is not valid IE of Setup message. \n"
#define CCMSG_2604_112_2_17_22_46_59_2619 "CC: Progress message is too short to contain mandatory IE.\n"
#define CCMSG_2624_112_2_17_22_46_59_2620 "CC: Progress message mandatory IE error.\n"
#define CCMSG_2637_112_2_17_22_46_59_2621 "CC: Progress Message Ie sequence error.\n"
#define CCMSG_2690_112_2_17_22_46_59_2622 "CC: Facility message is too short to contain mandatory IE.\n"
#define CCMSG_2736_112_2_17_22_46_59_2623 "CC: Notify message is too short to contain mandatory IE.\n"
#define CCMSG_2900_112_2_17_22_46_59_2624 "CC: Error IE in Call Proceeding Message. \n"
#define CCMSG_3000_112_2_17_22_47_0_2625 "CC: Error IE in Alerting Message. \n"
#define CCMSG_3121_112_2_17_22_47_0_2626 "CC: Error IE in Connect Message. \n"
#define CCMSG_3249_112_2_17_22_47_0_2627 "CC: Error IE in Disconnect Message. \n"
#define CCMSG_3372_112_2_17_22_47_0_2628 "CC: Error IE in Modify Message. \n"
#define CCMSG_3489_112_2_17_22_47_1_2629 "CC: Error IE in Modify Complete Message. \n"
#define CCMSG_3609_112_2_17_22_47_1_2630 "CC: Error IE in Modify Reject Message. \n"
#define CCMSG_3658_112_2_17_22_47_1_2631 "CC: Message is too short.\n"
#define CCMSG_3699_112_2_17_22_47_1_2632 "CC: Message is too short.\n"
#define CCMSG_4020_112_2_17_22_47_2_2633 "CC: Error IE in Release Message. \n"
#define CCMSG_4122_112_2_17_22_47_2_2634 "CC: Error IE in Release Complete Message. \n"
#define CCMSG_4308_112_2_17_22_47_2_2635 "CC: Cc-Establishment message is too short to contain mandatory IE.\n"
#define CCMSG_4359_112_2_17_22_47_2_2636 "CC: Recall message is too short to contain mandatory IE.\n"
#define CCMSG_5221_112_2_17_22_47_4_2637 "CC: bc ie is type error"
#define CCMSG_5268_112_2_17_22_47_4_2638 "CC: facility ie is type error"
#define CCMSG_5325_112_2_17_22_47_4_2639 "CC: progress ie is type error"
#define CCMSG_5831_112_2_17_22_47_6_2640 "CC: progress ie's type error"
#define CCMSG_5900_112_2_17_22_47_6_2641 "CC: progress ie's type error"
#define CCMSG_5997_112_2_17_22_47_6_2642 "CC: backup bc ie is type error"
#define CCMSG_6050_112_2_17_22_47_6_2643 "CC: allowed action ie is type error"
#define L3FUNC_58_112_2_17_22_47_11_2644 "L3FUNC: Assert Failure, PLMN list length %d is more than MAX %d."
#define L3FUNC_94_112_2_17_22_47_11_2645 "L3FUNC: Assert Failure, FPLMN list length %d is more than MAX %d."
#define L3FUNC_187_112_2_17_22_47_11_2646 "L3FUNC: Assert Failure, LongFPLMN list length %d is more than MAX %d."
#define L3FUNC_401_112_2_17_22_47_12_2647 "L3FUNC: Assert Failure, FLai list length %d is more than MAX %d."
#define L3FUNC_439_112_2_17_22_47_12_2648 "L3FUNC: Assert Failure, FPLMN list length %d is more than MAX %d."
#define L3FUNC_472_112_2_17_22_47_12_2649 "L3FUNC: Assert Failure, LongFPLMN list length %d is more than MAX %d."
#define L3FUNC_504_112_2_17_22_47_12_2650 "L3FUNC: Assert Failure, PLMN list length %d is more than MAX %d."
#define L3FUNC_533_112_2_17_22_47_12_2651 "L3FUNC: Assert Failure, FLai list length %d is more than MAX %d."
#define L3FUNC_570_112_2_17_22_47_12_2652 "L3FUNC: Assert Failure, EPLMN list length %d is more than MAX %d."
#define L3FUNC_615_112_2_17_22_47_12_2653 "L3FUNC: Assert Failure, EPLMN list length %d is more than MAX %d."
#define L3FUNC_706_112_2_17_22_47_12_2654 "L3FUNC: Assert Failure, Select PLMN list length %d is more than MAX %d."
#define L3FUNC_735_112_2_17_22_47_12_2655 "L3FUNC: Assert Failure, Available PLMN list length %d is more than MAX %d."
#define MMGMM_COMMON_IE_133_112_2_17_22_47_13_2656 "MMGMM: Assert Failure, ie_type %d error, can't decode EC number list."
#define MMGMM_COMMON_IE_210_112_2_17_22_47_13_2657 "MMGMM: DecodeEmgNum data_len=%d, data_ptr[0]=%d"
#define MMGMM_COMMON_IE_266_112_2_17_22_47_13_2658 "MMGMM:The length of the network name is too long"
#define MMGMM_COMMON_IE_293_112_2_17_22_47_13_2659 "MMGMM:The length of the network name is too long"
#define MMGMM_COMMON_IE_328_112_2_17_22_47_13_2660 "MMGMM: Assert Failure, date_len %d error, can't decode TimeZone."
#define MMGMM_COMMON_IE_353_112_2_17_22_47_13_2661 "MMGMM: Assert Failure, date_len %d error, can't decode TimeZone and Time."
#define MMGMM_COMMON_IE_458_112_2_17_22_47_13_2662 "MMGMM: decode MM/GMM information msg, MANDATORY_IE_CONTENT_ERROR"
#define MMGMM_COMMON_IE_468_112_2_17_22_47_13_2663 "MMGMM: decode MM/GMM information msg, DECODE_SUCCESS, No Optional IE"
#define MMGMM_COMMON_IE_578_112_2_17_22_47_14_2664 "MMGMM: Assert Failure, ie_type %d error, can't decode MobileIdentify."
#define PS_MAIN_535_112_2_17_22_47_17_2665 "SDL_RUN\n"
#define SCI_AIR_INTERFACE_232_112_2_17_22_47_20_2666 "SCIAIR:SCIAI_BSAIRMsgToPSMsg Receive unknown SignalCode"
#define SCI_AIR_INTERFACE_307_112_2_17_22_47_20_2667 "SCIAIR:SCIAI_BSPSMsgToAIRMsg Receive unknown SignalCode"
#define STATE_MACHINE_33_112_2_17_22_47_53_2668 "save signal %x\n"
#define STATE_MACHINE_46_112_2_17_22_47_53_2669 "discard signal %x\n"
#define DL_BUFFER_322_112_2_17_22_47_58_2670 "L2:unit(not SACCH) frame_ready\n"
#define DL_BUFFER_731_112_2_17_22_47_59_2671 "L2:ack SEQ%d frame \n"
#define DL_BUFFER_742_112_2_17_22_47_59_2672 " NR error, ns=%d ,new=%d ,old=%d n"
#define DL_BUFFER_753_112_2_17_22_47_59_2673 "L2:  frame_sending now\n"
#define DL_BUFFER_760_112_2_17_22_47_59_2674 "L2:  frame_wait_for_resend now\n"
#define DL_BUFFER_768_112_2_17_22_47_59_2675 "L2: receive ack but frame not send , so nr err \n"
#define DL_BUFFER_799_112_2_17_22_47_59_2676 "L2:  frame acked \n"
#define DL_BUFFER_806_112_2_17_22_47_59_2677 "L2:  all frame acked \n"
#define DL_BUFFER_1146_112_2_17_22_48_0_2678 "L2:send_num= %d; recv_num= %d\n"
#define DL_BUFFER_1158_112_2_17_22_48_0_2679 "L2:RR come\n"
#define DL_BUFFER_1165_112_2_17_22_48_0_2680 "L2:REJ come\n"
#define DL_BUFFER_1191_112_2_17_22_48_0_2681 "L2: UA frame come\n"
#define DL_BUFFER_1200_112_2_17_22_48_0_2682 "L2:pf:%d\n"
#define DL_BUFFER_1466_112_2_17_22_48_0_2683 "L2:package RR frame\n"
#define DL_BUFFER_1473_112_2_17_22_48_0_2684 "L2:package REJ frame\n"
#define DL_BUFFER_1502_112_2_17_22_48_0_2685 "L2:package I frame S:%d,R:%d\n"
#define DL_BUFFER_1855_112_2_17_22_48_1_2686 "L2:package SABM \n"
#define DL_BUFFER_1859_112_2_17_22_48_1_2687 "L2:package DISC \n"
#define DL_BUFFER_1863_112_2_17_22_48_1_2688 "L2:package DM \n"
#define DL_BUFFER_1867_112_2_17_22_48_1_2689 "L2:package UA \n"
#define DL_BUFFER_2051_112_2_17_22_48_1_2690 "L2:unit(SACCH) frame_ready\n"
#define DL_BUFFER_2074_112_2_17_22_48_1_2691 "contension data fail,length wrong\n"
#define DL_BUFFER_2081_112_2_17_22_48_1_2692 "contension data fail,data wrong\n"
#define DL_BUFFER_2162_112_2_17_22_48_2_2693 "DL: DL_GetUnsentSAPI3MsgHeader, MSG_PTR IS NOT NULL"
#define DL_BUFFER_2168_112_2_17_22_48_2_2694 "DL: MSG header is %d,%d"
#define DL_BUFFER_2170_112_2_17_22_48_2_2695 "DL:There is a message to be retrans!"
#define DL_BUFFER_2202_112_2_17_22_48_2_2696 "DL: receive SABM in established state, but not init dlc according to special condition"
#define GMM_CLASSMARK_477_112_2_17_22_48_3_2697 "GMM: classmark InitMsBand, ms_band=%d"
#define GMM_ENCODE_DECODE_180_112_2_17_22_48_4_2698 "GMMCODE_Convert2GprsTimeValue input seconds = %d"
#define GMM_ENCODE_DECODE_281_112_2_17_22_48_4_2699 "GMM:mobile_identity->type_identity =%d"
#define GMM_ENCODE_DECODE_568_112_2_17_22_48_5_2700 "GMM:detach type %d is interpreted as re-attach not required"
#define GMM_ENCODE_DECODE_598_112_2_17_22_48_5_2701 "GMM:message too short,len = %d!"
#define GMM_ENCODE_DECODE_813_112_2_17_22_48_5_2702 "GMM:Attach Accept msg, unknown field!"
#define GMM_ENCODE_DECODE_837_112_2_17_22_48_5_2703 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_869_112_2_17_22_48_5_2704 "GMM:Attach Reject, unknown field!"
#define GMM_ENCODE_DECODE_893_112_2_17_22_48_5_2705 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_928_112_2_17_22_48_5_2706 "GMM:Detach Request,unknown field!"
#define GMM_ENCODE_DECODE_952_112_2_17_22_48_5_2707 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_969_112_2_17_22_48_5_2708 "GMM:detach accept,unknown field!"
#define GMM_ENCODE_DECODE_995_112_2_17_22_48_5_2709 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_1044_112_2_17_22_48_5_2710 "GMM:PTMSI reallocation,unknown field!"
#define GMM_ENCODE_DECODE_1068_112_2_17_22_48_6_2711 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_1127_112_2_17_22_48_6_2712 "GMM:Authentication Msg error,rand-cksn should as both!"
#define GMM_ENCODE_DECODE_1134_112_2_17_22_48_6_2713 "GMM:Authentication&Ciper msg, unknown field!"
#define GMM_ENCODE_DECODE_1157_112_2_17_22_48_6_2714 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_1176_112_2_17_22_48_6_2715 "GMM:Identify Request,unknown field!"
#define GMM_ENCODE_DECODE_1205_112_2_17_22_48_6_2716 "GMM error:npdu_list->number=%d"
#define GMM_ENCODE_DECODE_1247_112_2_17_22_48_6_2717 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_1559_112_2_17_22_48_7_2718 "GMM:rau accept,unknown field!"
#define GMM_ENCODE_DECODE_1612_112_2_17_22_48_7_2719 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_1647_112_2_17_22_48_7_2720 "GMM:RAU Reject msg, unknown field!"
#define GMM_ENCODE_DECODE_1670_112_2_17_22_48_7_2721 "GMM:message too short,length = %d"
#define GMM_ENCODE_DECODE_1685_112_2_17_22_48_7_2722 "GMM:GMM Status msg, unknown field!"
#define GMM_ENCODE_DECODE_1780_112_2_17_22_48_7_2723 "GMM: rl_ind = %d"
#define GMM_ENCODE_DECODE_1956_112_2_17_22_48_7_2724 "GMM:Assert Failure,invalid type_identity,"
#define GMM_ENCODE_DECODE_2480_112_2_17_22_48_8_2725 "GMM: TLLI_FOREIGN ptmsi.valid_flag is %d"
#define GMM_ENCODE_DECODE_2486_112_2_17_22_48_8_2726 "GMM: TLLI_LOCAL ptmsi.valid_flag is %d"
#define GMM_ENCODE_DECODE_2504_112_2_17_22_48_9_2727 "GMM:TLLI error"
#define GMM_ENCODE_DECODE_2515_112_2_17_22_48_9_2728 "GMM: Type = %d, new TLLI allocated = %x"
#define GMM_MAIN_820_112_2_17_22_48_10_2729 "REGISTERED_INITIATED GMM_ATTACH_ACCEPT return_state = %d"
#define GMM_MAIN_850_112_2_17_22_48_11_2730 "REGISTERED_INITIATED GMM_DETACH_REQUEST return_state = %d"
#define GMM_MAIN_1237_112_2_17_22_48_11_2731 "GMM_REGISTERED_INITIATED LL_GMM_UNITDATA_IND msg_type = %d"
#define GMM_MAIN_1307_112_2_17_22_48_11_2732 "GMM_REGISTERED LL_GMM_UNITDATA_IND msg_type = %d"
#define GMM_MAIN_1375_112_2_17_22_48_12_2733 "GMM_RAU_INITIATED LL_GMM_UNITDATA_IND msg_type = %d"
#define GMM_MAIN_1430_112_2_17_22_48_12_2734 "GMM_RAU_INITIATED LL_GMM_UNITDATA_IND msg_type = %d"
#define GMM_MAIN_2327_112_2_17_22_48_14_2735 "ATTACH REQ sent, but ATTACH ACCEPT not received"
#define GMM_MAIN_2334_112_2_17_22_48_14_2736 "send ATTACH REQ"
#define GMM_MAIN_2531_112_2_17_22_48_14_2737 "send attach request"
#define GMM_SUBPROC_190_112_2_17_22_48_20_2738 "%s"
#define GMM_SUBPROC_2345_112_2_17_22_48_25_2739 "Gmm: rau attempt count > 5"
#define GMM_SUBPROC_3925_112_2_17_22_48_28_2740 "GMM:info decode error"
#define GMM_TIMER_77_112_2_17_22_48_28_2741 "GMM_SetTimer timer_type = %d"
#define GMM_TIMER_103_112_2_17_22_48_29_2742 "GMM_KillTimer timer_type = %d"
#define GMM_TIMER_145_112_2_17_22_48_29_2743 "GMM_IsTimerActive timer_type = %d"
#define LLC_ANALYSE_FRAME_206_112_2_17_22_48_32_2744 "LLC:add oc_unack_downlink,512 for missing N(u)"
#define LLC_ANALYSE_FRAME_217_112_2_17_22_48_32_2745 "LLC:retransmit encrypted llc frame with the N(S) belonging to previous 0~511 round"
#define LLC_ANALYSE_FRAME_226_112_2_17_22_48_32_2746 "LLC:the received UI frame is enciphered but no cipher algorithm!"
#define LLC_ANALYSE_FRAME_263_112_2_17_22_48_32_2747 "LLC:before_crc 512+oc_unack_downlink=%d"
#define LLC_ANALYSE_FRAME_313_112_2_17_22_48_32_2748 "LLE: UI frame,invalid FCS! "
#define LLC_ANALYSE_FRAME_339_112_2_17_22_48_32_2749 "LLE: U frame,invalid FCS! "
#define LLC_ANALYSE_FRAME_359_112_2_17_22_48_32_2750 "LLE: S frame,invalid FCS! "
#define LLC_ANALYSE_FRAME_402_112_2_17_22_48_32_2751 "LLC: I frame need to be deciphered!"
#define LLC_ANALYSE_FRAME_422_112_2_17_22_48_33_2752 "LLC: I frame length invalid,discard"
#define LLC_ANALYSE_FRAME_451_112_2_17_22_48_33_2753 "LLC: I frame FCS invalid,discard"
#define LLC_ANALYSE_FRAME_535_112_2_17_22_48_33_2754 "LLE: Received a duplicate frame"
#define LLC_ANALYSE_FRAME_782_112_2_17_22_48_33_2755 "LLC: I frame N(S) invalid,discard"
#define LLC_ANALYSE_FRAME_809_112_2_17_22_48_33_2756 "LLC:Assert Failure,DeleteSHeadNode empty 0!"
#define LLC_ANALYSE_FRAME_946_112_2_17_22_48_34_2757 "LLC:Assert Failure,DeleteSHeadNode empty!"
#define LLC_CIPHER_81_112_2_17_22_48_34_2758 "LLC:gmm need cipher misstate!"
#define LLC_CIPHER_152_112_2_17_22_48_34_2759 "LLC:KC changed and reset IOV_UI/I"
#define LLC_CIPHER_217_112_2_17_22_48_34_2760 "LLC:Cipher ASSERT !! gea_ciphering failed!"
#define LLC_CIPHER_269_112_2_17_22_48_34_2761 "LLC:Cipher ASSERT !! gea_ciphering failed!"
#define LLC_COMMON_200_112_2_17_22_48_35_2762 "LLC: in set frame before n(r) acked\n"
#define LLC_COMMON_CODE_326_112_2_17_22_48_37_2763 "LLC:Assert Failure,wrong sapi value!"
#define LLC_COMMON_CODE_427_112_2_17_22_48_37_2764 "LLC:Assert Failure,SetFRMRErrMsg wrong error type!"
#define LLC_COMMON_CODE_843_112_2_17_22_48_38_2765 "LLC:Assert Failure,Polling next U node wrong value!"
#define LLC_COMMON_CODE_891_112_2_17_22_48_38_2766 "LLC:Assert Failure,InvalidXID next U node wrong value!"
#define LLC_COMMON_CODE_1038_112_2_17_22_48_38_2767 "LLC:KC changed and reset IOV_UI/I"
#define LLC_COMMON_CODE_1315_112_2_17_22_48_39_2768 "LLC:Cipher ASSERT !! gea_ciphering failed!"
#define LLC_COMMON_CODE_1378_112_2_17_22_48_39_2769 "LLC:Cipher ASSERT !! gea_ciphering failed!"
#define LLC_COMMON_CODE_1428_112_2_17_22_48_39_2770 "LLC: Xid negotiation N201-U=%d"
#define LLC_COMMON_CODE_1468_112_2_17_22_48_39_2771 "LLC:gmm need cipher misstate!"
#define LLC_COMPOSE_FRAME_158_112_2_17_22_48_41_2772 "LLC:UPLINK XID PARAMETES-->T200:%d"
#define LLC_COMPOSE_FRAME_168_112_2_17_22_48_41_2773 "LLC:UPLINK XID PARAMETES-->N200:%d"
#define LLC_COMPOSE_FRAME_180_112_2_17_22_48_41_2774 "LLC:UPLINK XID PARAMETES-->N201 U:%d"
#define LLC_COMPOSE_FRAME_197_112_2_17_22_48_41_2775 "LLC:UPLINK XID PARAMETES-->N201 I:%d"
#define LLC_COMPOSE_FRAME_209_112_2_17_22_48_41_2776 "LLC:UPLINK XID PARAMETES-->mD:%d"
#define LLC_COMPOSE_FRAME_221_112_2_17_22_48_41_2777 "LLC:UPLINK XID PARAMETES-->mU:%d"
#define LLC_COMPOSE_FRAME_231_112_2_17_22_48_41_2778 "LLC:UPLINK XID PARAMETES-->kD:%d"
#define LLC_COMPOSE_FRAME_241_112_2_17_22_48_41_2779 "LLC:UPLINK XID PARAMETES-->kU:%d"
#define LLC_COMPOSE_FRAME_275_112_2_17_22_48_41_2780 "LLC:UPLINK XID PARAMETES-->zero-length Layer 3 Parameters"
#define LLC_COMPOSE_FRAME_282_112_2_17_22_48_41_2781 "LLC:UPLINK XID PARAMETES-->zero-length Layer 3 Parameters"
#define LLC_COMPOSE_FRAME_656_112_2_17_22_48_42_2782 "LLC:Assert Failure,ComposeIFrame S empty!"
#define LLC_COMPOSE_FRAME_1070_112_2_17_22_48_42_2783 "LLC: in set frame status\n"
#define LLC_COMPOSE_FRAME_1111_112_2_17_22_48_42_2784 "LLC: in set frame before n(r) acked\n"
#define LLC_COMPOSE_FRAME_1282_112_2_17_22_48_43_2785 "LLC:decode I frame SACK frame error!"
#define LLC_COMPOSE_FRAME_1368_112_2_17_22_48_43_2786 "LLC: SACK error bitmap"
#define LLC_COMPOSE_FRAME_1542_112_2_17_22_48_43_2787 "LLC:Assert Failure,SACK DeleteSNode S rnr invalid or empty"
#define LLC_COMPOSE_FRAME_1558_112_2_17_22_48_43_2788 "LLC:Assert Failure,SACK DeleteSNode S invalid or empty"
#define LLC_COMPOSE_FRAME_1670_112_2_17_22_48_44_2789 "LLC:Assert Failure,DeleteSNode S rnr invalid or empty"
#define LLC_COMPOSE_FRAME_1686_112_2_17_22_48_44_2790 "LLC:Assert Failure,DeleteSNode S invalid or empty"
#define LLC_COMPOSE_FRAME_1764_112_2_17_22_48_44_2791 "LLC:Assert Failure,DeleteUINode empty or invalid"
#define LLC_COMPOSE_FRAME_1796_112_2_17_22_48_44_2792 "LLC:Assert Failure,ComposeNewFrame  UI empty or invalid"
#define LLC_DECODE_174_112_2_17_22_48_45_2793 "LLC:add oc_unack_downlink,512 for missing N(u)"
#define LLC_DECODE_185_112_2_17_22_48_45_2794 "LLC:retransmit encrypted llc frame with the N(S) belonging to previous 0~511 round"
#define LLC_DECODE_194_112_2_17_22_48_45_2795 "LLC:the received UI frame is enciphered but no cipher algorithm!"
#define LLC_DECODE_231_112_2_17_22_48_45_2796 "LLC:before_crc 512+oc_unack_downlink=%d"
#define LLC_DECODE_281_112_2_17_22_48_45_2797 "LLE: UI frame,invalid FCS! "
#define LLC_DECODE_367_112_2_17_22_48_45_2798 "LLE: U frame,invalid FCS! "
#define LLC_DECODE_497_112_2_17_22_48_45_2799 "LLE: S frame,invalid FCS! "
#define LLC_DECODE_614_112_2_17_22_48_46_2800 "LLC: I frame need to be deciphered!"
#define LLC_DECODE_633_112_2_17_22_48_46_2801 "LLC: I frame length invalid,discard"
#define LLC_DECODE_661_112_2_17_22_48_46_2802 "LLC: I frame FCS invalid,discard"
#define LLC_DECODE_688_112_2_17_22_48_46_2803 "LLC: I frame N(S) invalid,discard"
#define LLC_ENCODE_894_112_2_17_22_48_48_2804 "LLC:error need check,frame type expected is sack,but bitmap length is 0,change sack to rr"
#define LLC_ENCODE_957_112_2_17_22_48_48_2805 "LLC:error need check,frame type expected is sack,but bitmap length is 0,change sack to rr"
#define LLC_ENCODE_1496_112_2_17_22_48_49_2806 "LLC:UPLINK XID PARAMETES-->T200:%d"
#define LLC_ENCODE_1506_112_2_17_22_48_49_2807 "LLC:UPLINK XID PARAMETES-->N200:%d"
#define LLC_ENCODE_1518_112_2_17_22_48_49_2808 "LLC:UPLINK XID PARAMETES-->N201 U:%d"
#define LLC_ENCODE_1535_112_2_17_22_48_49_2809 "LLC:UPLINK XID PARAMETES-->N201 I:%d"
#define LLC_ENCODE_1547_112_2_17_22_48_49_2810 "LLC:UPLINK XID PARAMETES-->mD:%d"
#define LLC_ENCODE_1559_112_2_17_22_48_49_2811 "LLC:UPLINK XID PARAMETES-->mU:%d"
#define LLC_ENCODE_1569_112_2_17_22_48_49_2812 "LLC:UPLINK XID PARAMETES-->kD:%d"
#define LLC_ENCODE_1579_112_2_17_22_48_49_2813 "LLC:UPLINK XID PARAMETES-->kU:%d"
#define LLC_ENCODE_1613_112_2_17_22_48_50_2814 "LLC:UPLINK XID PARAMETES-->zero-length Layer 3 Parameters"
#define LLC_ENCODE_1620_112_2_17_22_48_50_2815 "LLC:UPLINK XID PARAMETES-->zero-length Layer 3 Parameters"
#define LLC_ENTITY_810_112_2_17_22_48_52_2816 "LLC:u pdu queue is full in abm or adm state!"
#define LLC_ENTITY_1292_112_2_17_22_48_52_2817 "LLC:discard message:msg id=%d"
#define LLC_FRAME_293_112_2_17_22_48_54_2818 "LLC:impossible U frame type"
#define LLC_FRAME_370_112_2_17_22_48_55_2819 "LLC:impossible S frame type"
#define LLC_FRAME_463_112_2_17_22_48_55_2820 "LLC:impossible I frame type"
#define LLC_FRAME_528_112_2_17_22_48_55_2821 "LLC:Assert Failure,SetFRMRErrMsg wrong error type!"
#define LLC_MAIN_567_112_2_17_22_48_58_2822 "LLC: discard GMM_LLME_ASSIGN_REQ due to gmm pid mismatch!"
#define LLC_MAIN_888_112_2_17_22_48_59_2823 "LLC: discard GMM_LLME_ASSIGN_REQ due to gmm pid mismatch!"
#define LLC_MAIN_1422_112_2_17_22_49_0_2824 "LLC:discard rlc pdu due to sapi error."
#define LLC_MAIN_1428_112_2_17_22_49_0_2825 "LLC:discard rlc pdu due to tlli mismatch."
#define LLC_MAIN_1484_112_2_17_22_49_0_2826 "LLC:discard rlc pdu due to sapi error."
#define LLC_MAIN_1490_112_2_17_22_49_0_2827 "LLC:discard rlc pdu due to tlli mismatch."
#define LLC_MAIN_1626_112_2_17_22_49_1_2828 "LLC: dispatch msg %d to entity %d"
#define LLC_MAIN_1791_112_2_17_22_49_1_2829 "LLC_TASK: discard signal:%d"
#define LLC_SDL_145_112_2_17_22_49_2_2830 "LLentity:free,reliability error!"
#define LLC_SDL_294_112_2_17_22_49_3_2831 "invalid lle_reliability = %d"
#define LLC_SDL_454_112_2_17_22_49_3_2832 "LLC_SendUIQueue i = %d"
#define LLC_SDL_584_112_2_17_22_49_3_2833 "LLentity: discard l3 pdu, abnormal"
#define LLC_TEST_47_112_2_17_22_49_5_2834 "LLC_TEST:lle_ResetT201 expire!"
#define LLC_TEST_52_112_2_17_22_49_5_2835 "LLC_TEST:lle_SetT200 expire!"
#define LLC_TEST_57_112_2_17_22_49_5_2836 "LLC_TEST:lle_SetT201 expire!"
#define LLC_TIMER_65_112_2_17_22_49_6_2837 "LLC: LLC_StartTimer timer_type %d is not present!"
#define LLC_TIMER_97_112_2_17_22_49_6_2838 "LLC: LLC_StopTimer timer_type %d is not present!"
#define LLC_TIMER_156_112_2_17_22_49_6_2839 "LLC: LLC_IsTimerActive timer_type %d is not present!"
#define LLC_XID_391_112_2_17_22_49_7_2840 "LLC:the received U frame does not include xid parameter!"
#define LLC_XID_808_112_2_17_22_49_8_2841 "LLC:Assert Failure,wrong sapi value!"
#define LLC_XID_855_112_2_17_22_49_8_2842 "LLC: Xid negotiation N201-U=%d"
#define LLC_XID_1806_112_2_17_22_49_10_2843 "LLC:Log XID frame parameter:"
#define LLC_XID_1809_112_2_17_22_49_10_2844 "LLC:xid Reset"
#define LLC_XID_1813_112_2_17_22_49_10_2845 "LLC:Version = %d"
#define LLC_XID_1817_112_2_17_22_49_10_2846 "LLC:T200 = %d"
#define LLC_XID_1821_112_2_17_22_49_10_2847 "LLC:N200 = %d"
#define LLC_XID_1825_112_2_17_22_49_10_2848 "LLC:N201_U = %d"
#define LLC_XID_1829_112_2_17_22_49_10_2849 "LLC:N201_I = %d"
#define LLC_XID_1833_112_2_17_22_49_10_2850 "LLC:mD = %d"
#define LLC_XID_1837_112_2_17_22_49_10_2851 "LLC:mU = %d"
#define LLC_XID_1841_112_2_17_22_49_10_2852 "LLC:kD = %d"
#define LLC_XID_1845_112_2_17_22_49_10_2853 "LLC:kU = %d"
#define LLC_XID_1849_112_2_17_22_49_10_2854 "LLC:IOV_UI = 0x%08x"
#define LLC_XID_1853_112_2_17_22_49_10_2855 "LLC:IOV_I = 0x%08x"
#define LLC_XID_1857_112_2_17_22_49_10_2856 "LLC:SNDCP xid Parameters included"
#define LLC_XID_CODE_972_112_2_17_22_49_12_2857 "LLC:Log XID frame parameter:"
#define LLC_XID_CODE_975_112_2_17_22_49_12_2858 "LLC:xid Reset"
#define LLC_XID_CODE_979_112_2_17_22_49_12_2859 "LLC:Version = %d"
#define LLC_XID_CODE_983_112_2_17_22_49_12_2860 "LLC:T200 = %d"
#define LLC_XID_CODE_987_112_2_17_22_49_12_2861 "LLC:N200 = %d"
#define LLC_XID_CODE_991_112_2_17_22_49_12_2862 "LLC:N201_U = %d"
#define LLC_XID_CODE_995_112_2_17_22_49_12_2863 "LLC:N201_I = %d"
#define LLC_XID_CODE_999_112_2_17_22_49_12_2864 "LLC:mD = %d"
#define LLC_XID_CODE_1003_112_2_17_22_49_12_2865 "LLC:mU = %d"
#define LLC_XID_CODE_1007_112_2_17_22_49_12_2866 "LLC:kD = %d"
#define LLC_XID_CODE_1011_112_2_17_22_49_12_2867 "LLC:kU = %d"
#define LLC_XID_CODE_1015_112_2_17_22_49_12_2868 "LLC:IOV_UI = 0x%08x"
#define LLC_XID_CODE_1019_112_2_17_22_49_12_2869 "LLC:IOV_I = 0x%08x"
#define LLC_XID_CODE_1023_112_2_17_22_49_12_2870 "LLC:SNDCP xid Parameters included"
#define LLC_XID_CODE_1064_112_2_17_22_49_12_2871 "LLC:the received U frame does not include xid parameter!"
#define MAC_CSN_MSG_1033_112_2_17_22_49_16_2872 "MAC:egprs message!"
#define MAC_CSN_MSG_1465_112_2_17_22_49_16_2873 "MAC:DL PWR CTRL %d, MODE %d, PR MODE %d"
#define MAC_CSN_MSG_1512_112_2_17_22_49_17_2874 "MAC: EGPRS window size packet DL assign"
#define MAC_CSN_MSG_2108_112_2_17_22_49_18_2875 "MAC:ul ack alloc fix alloction,ms not support!"
#define MAC_CSN_MSG_2188_112_2_17_22_49_18_2876 "MAC:ul ack alloc fix alloction,ms not support!"
#define MAC_CSN_MSG_2297_112_2_17_22_49_18_2877 "MAC:DL PWR CTRL %d, MODE %d, PR MODE %d"
#define MAC_CSN_MSG_2311_112_2_17_22_49_18_2878 "MAC:uplink ass allo fix allocation,ms not support!"
#define MAC_CSN_MSG_2399_112_2_17_22_49_19_2879 "MAC:uplink ass allo fix allocation,ms not support!"
#define MAC_CSN_MSG_2477_112_2_17_22_49_19_2880 "MAC:uplink ass allo fix allocation,ms not support!"
#define MAC_CSN_MSG_2685_112_2_17_22_49_19_2881 "MAC:Packet Cell Change Failure have FDD_ind!! ERR!!!"
#define MAC_CSN_MSG_2690_112_2_17_22_49_19_2882 "MAC:Packet Cell Change Failure have TDD_ind!! ERR!!!"
#define MAC_CSN_MSG_2879_112_2_17_22_49_20_2883 "MAC:Unknown access type, error!!!"
#define MAC_CSN_MSG_3173_112_2_17_22_49_20_2884 "MAC:Packet Measurement Report have 3g_meas_report!ERR!!!"
#define MAC_CSN_MSG_3257_112_2_17_22_49_20_2885 "MAC:EGPRS Ack/Nack Description IE have L =%d"
#define MAC_CSN_MSG_3285_112_2_17_22_49_20_2886 "MAC:L_length =%d is not enough when encode crbb_len!!\n"
#define MAC_CSN_MSG_3296_112_2_17_22_49_20_2887 "MAC:L_length =%d is not enough when encode comp_bitmap_start_color_code!!\n"
#define MAC_CSN_MSG_3343_112_2_17_22_49_21_2888 "MAC:L_length =%d is not enough when encode crbb!!\n"
#define MAC_CSN_MSG_3394_112_2_17_22_49_21_2889 "MAC:L_length =%d is not enough when encode urbb!!\n"
#define MAC_CSN_MSG_5708_112_2_17_22_49_26_2890 "MAC:Error,the Access Technology Type num beyond 7!"
#define MAC_CSN_MSG_5882_112_2_17_22_49_26_2891 "MAC:DL PWR CTRL %d, PR MODE %d"
#define MAC_CSN_MSG_5980_112_2_17_22_49_26_2892 "MAC: Current cell band = %d"
#define MAC_CSN_MSG_6849_112_2_17_22_49_28_2893 "MAC:DL PWR CTRL %d, MODE %d, PR MODE %d"
#define MAC_CSN_MSG_6862_112_2_17_22_49_28_2894 "MAC:  RrPacketUlAssi fix alloc"
#define MAC_CSN_MSG_6867_112_2_17_22_49_28_2895 "MAC: EGPRS msg Ext alloc"
#define MAC_CSN_MSG_6975_112_2_17_22_49_28_2896 "MAC:DL PWR CTRL %d, MODE %d, PR MODE %d"
#define MAC_DECODE_DATA_IND_388_112_2_17_22_49_29_2897 "MAC: tfi ie not present"
#define MAC_DECODE_DATA_IND_402_112_2_17_22_49_29_2898 "MAC: msg rx on slot%d's global tfi matched the ul tfi, but not on the assigned ul slot!, "
#define MAC_DECODE_DATA_IND_421_112_2_17_22_49_29_2899 "MAC: msg rx on slot%d's global tfi matched the dl tfi, but not on the assigned dl slot!, "
#define MAC_DECODE_DATA_IND_432_112_2_17_22_49_29_2900 "MAC:Assert Failure, tfi type invalid"
#define MAC_DECODE_DATA_IND_541_112_2_17_22_49_29_2901 "MAC: ignore msg due to tlli mismatch"
#define MAC_DECODE_DATA_IND_544_112_2_17_22_49_29_2902 "MAC: match the old tlli"
#define MAC_DECODE_DATA_IND_548_112_2_17_22_49_29_2903 "MAC: match the new tlli"
#define MAC_DECODE_DATA_IND_559_112_2_17_22_49_29_2904 "MAC: Assert Failure,Invalid addr"
#define MAC_DECODE_DATA_IND_616_112_2_17_22_49_30_2905 "MAC: ignore msg due to tlli mismatch"
#define MAC_DECODE_DATA_IND_619_112_2_17_22_49_30_2906 "MAC: match the old tlli"
#define MAC_DECODE_DATA_IND_623_112_2_17_22_49_30_2907 "MAC: match the new tlli"
#define MAC_DECODE_DATA_IND_630_112_2_17_22_49_30_2908 "MAC: Assert Failure,measur order msg error"
#define MAC_DECODE_DATA_IND_657_112_2_17_22_49_30_2909 "MAC: ignore msg due to tlli mismatch"
#define MAC_DECODE_DATA_IND_660_112_2_17_22_49_30_2910 "MAC: match the old tlli"
#define MAC_DECODE_DATA_IND_664_112_2_17_22_49_30_2911 "MAC: match the new tlli"
#define MAC_DECODE_DATA_IND_671_112_2_17_22_49_30_2912 "MAC: Assert Failure,polling req msg error"
#define MAC_DECODE_DATA_IND_683_112_2_17_22_49_30_2913 "MAC:The packet queueing nitify message has not request reference!"
#define MAC_DECODE_DATA_IND_738_112_2_17_22_49_30_2914 "MAC: PAN on slot%d's tfi matched the ul tfi, but not on the assigned ul slot!, "
#define MAC_DECODE_DATA_IND_779_112_2_17_22_49_30_2915 "MAC: Assert Failure,unknow msg"
#define MAC_DECODE_DATA_IND_785_112_2_17_22_49_30_2916 "MAC: MSG decode error!"
#define MAC_DECODE_DATA_IND_789_112_2_17_22_49_30_2917 "MAC: MSG decode error!"
#define MAC_DECODE_DATA_IND_795_112_2_17_22_49_30_2918 "MAC: MSG decode error!"
#define MAC_DECODE_DATA_IND_919_112_2_17_22_49_30_2919 "MAC: Assert Failure,unknow decode msg error"
#define MAC_DECODE_DATA_IND_1024_112_2_17_22_49_30_2920 "MAC:CTRL REASSEMBLE:T3200 EXPIRE TS %d"
#define MAC_DECODE_DATA_IND_1053_112_2_17_22_49_31_2921 "MAC: msg rx on slot%d's header tfi is dl tfi, but not on the assigned dl slot!, "
#define MAC_DECODE_DATA_IND_1065_112_2_17_22_49_31_2922 "MAC: msg rx on slot%d's header tfi is ul tfi, but not on the assigned ul slot!, "
#define MAC_DECODE_DATA_IND_1106_112_2_17_22_49_31_2923 "MAC:CTRL REASSEMBLE PDCH TS %d"
#define MAC_DECODE_DATA_IND_1184_112_2_17_22_49_31_2924 "MAC: Assert Failure,assemb control eror"
#define MAC_DECODE_DATA_IND_1193_112_2_17_22_49_31_2925 "MAC:CTRL REASSEMBLE SAVE BLOCK TS %d"
#define MAC_DECODE_DATA_IND_1263_112_2_17_22_49_31_2926 "MAC: invalid rrbp value,ignore it!!"
#define MAC_DECODE_DATA_IND_1314_112_2_17_22_49_31_2927 "MAC: recv data block on slot%d tfi match but not on the assigned dl slots"
#define MAC_DECODE_DATA_IND_1739_112_2_17_22_49_32_2928 "MAC: discard the second ul ack"
#define MAC_DECODE_DATA_IND_1746_112_2_17_22_49_32_2929 "MAC: ingore msg %x"
#define MAC_DECODE_DATA_IND_1757_112_2_17_22_49_32_2930 "MAC: in wrong state receive packet uplink ack/unack"
#define MAC_DECODE_DATA_IND_1819_112_2_17_22_49_32_2931 "MAC: presdo tbf starting time, pkt dl assignment!"
#define MAC_DECODE_DATA_IND_1828_112_2_17_22_49_32_2932 "MAC: presdo tbf starting time, pkt ul assignment!"
#define MAC_DECODE_DATA_IND_1837_112_2_17_22_49_32_2933 "MAC: presdo tbf starting time, pkt ts reconfig!"
#define MAC_DECODE_DATA_IND_1857_112_2_17_22_49_32_2934 "MAC: unknown msg type %d"
#define MAC_DECODE_DATA_IND_1901_112_2_17_22_49_32_2935 "MAC: Ingore the polling flag!"
#define MAC_DECODE_DATA_IND_1921_112_2_17_22_49_32_2936 "MAC: Assert Failure,Invalid control ack type"
#define MAC_DECODE_DATA_IND_1951_112_2_17_22_49_32_2937 "MAC: Assert failure1,Invalid control ack type"
#define MAC_DECODE_DATA_IND_2033_112_2_17_22_49_32_2938 "MAC:wrong MCS/CS, discard invalid data"
#define MAC_DECODE_DATA_IND_2092_112_2_17_22_49_33_2939 "mac:error control data type!"
#define MAC_DECODE_DATA_IND_2110_112_2_17_22_49_33_2940 "mac:error control data type!"
#define MAC_DECODE_DATA_IND_2116_112_2_17_22_49_33_2941 "MAC: error payload type %d"
#define MAC_DECODE_DATA_IND_2204_112_2_17_22_49_33_2942 "MAC: error payload type %d"
#define MAC_DECODE_DATA_IND_2290_112_2_17_22_49_33_2943 "MAC: error,pdch assi invalid pd type %d"
#define MAC_DECODE_DATA_IND_2349_112_2_17_22_49_33_2944 "MAC:error mutislot control message!"
#define MAC_DECODE_DATA_IND_2413_112_2_17_22_49_33_2945 "MAC: tlli mismatch in PAR msg!Select next!"
#define MAC_DECODE_MA_446_112_2_17_22_49_35_2946 "MAC: GPRS class > 12 %d"
#define MAC_DECODE_MA_480_112_2_17_22_49_35_2947 "MAC: Multislot mismatch, UL timeslot assi num: %d"
#define MAC_DECODE_MA_488_112_2_17_22_49_35_2948 "MAC: Multislot mismatch, DL timeslot assi num: %d"
#define MAC_DECODE_MA_496_112_2_17_22_49_35_2949 "MAC: Multislot mismatch, UL timeslot assi num: %d DL timeslot assi num: %d"
#define MAC_DECODE_MA_524_112_2_17_22_49_35_2950 "MAC: UL slot %x mismatch DL slot %x"
#define MAC_DECODE_MA_581_112_2_17_22_49_35_2951 "MAC: polling slot before first rx slot, discard!"
#define MAC_DECODE_MA_588_112_2_17_22_49_35_2952 "MAC: old polling occupy new assignment slot after changed, discard!"
#define MAC_DECODE_MA_595_112_2_17_22_49_35_2953 "MAC: check polling validate, polling slot and the rx slot collision"
#define MAC_DECODE_MA_634_112_2_17_22_49_35_2954 "MAC: GPRS class > 12 %d"
#define MAC_DECODE_MA_639_112_2_17_22_49_35_2955 "MAC: check polling validate, violates MS capability"
#define MAC_DECODE_MA_911_112_2_17_22_49_35_2956 "MAC: Decode ma error, ref arfcn num=0!"
#define MAC_DECODE_MA_959_112_2_17_22_49_36_2957 "MAC: Decode ma error, arfcn num!=bitmap len %d %d!"
#define MAC_DECODE_MA_1022_112_2_17_22_49_36_2958 "MAC: Decode ma error, arfcn num=0!"
#define MAC_DECODE_MA_1029_112_2_17_22_49_36_2959 "MAC: Decode ma error, arfcn not in band!"
#define MAC_DECODE_MA_1086_112_2_17_22_49_36_2960 "MAC: Freq para update error in PSI2, invalid ma number!"
#define MAC_DECODE_MA_1098_112_2_17_22_49_36_2961 "MAC: Freq para update error in PSI2, decode ma error!"
#define MAC_DECODE_MA_1124_112_2_17_22_49_36_2962 "MAC: Freq para update error in SI13, decode ma error!"
#define MAC_DECODE_MA_1146_112_2_17_22_49_36_2963 "MAC: Invalid freq param type"
#define MAC_DECODE_MA_1182_112_2_17_22_49_36_2964 "MAC: Decode freq para error, decode ma error,freq no in band!"
#define MAC_DECODE_MA_1197_112_2_17_22_49_36_2965 "MAC: Decode freq para error, invalid ma number %d!"
#define MAC_DECODE_MA_1206_112_2_17_22_49_36_2966 "MAC: Decode freq para error, ma number not referenced %d!"
#define MAC_DECODE_MA_1217_112_2_17_22_49_36_2967 "MAC: Decode freq para error, ma number=15 but change mark present!"
#define MAC_DECODE_MA_1241_112_2_17_22_49_36_2968 "MAC: Decode freq para error, change mark not match!"
#define MAC_DECODE_MA_1249_112_2_17_22_49_36_2969 "MAC: Decode freq para error, change mark not match!"
#define MAC_DECODE_MA_1270_112_2_17_22_49_36_2970 "MAC: Decode freq para error, decode ma error!"
#define MAC_DECODE_MA_1288_112_2_17_22_49_36_2971 "MAC: Decode freq bitmap error!"
#define MAC_DECODE_MA_1295_112_2_17_22_49_36_2972 "MAC: Decode freq para error, decode ma error,freq no in band!"
#define MAC_DECODE_MA_1308_112_2_17_22_49_36_2973 "MAC: Assert Failure,Invalid freq pars selector %d"
#define MAC_DECODE_MA_1554_112_2_17_22_49_37_2974 "MAC: MAC mode mismatch, receive ext-dynamic allocation when ul is dynamic!"
#define MAC_DECODE_MA_1559_112_2_17_22_49_37_2975 "MAC: network alloc EXTENDED_DYNAMIC!"
#define MAC_DECODE_MA_1568_112_2_17_22_49_37_2976 "MAC: MAC mode mismatch, receive dynamic allocation when ul is ext-dynamic!"
#define MAC_DECODE_MA_1642_112_2_17_22_49_37_2977 "MAC: receive pkt uplink ass with no tfi when establish uplink TBF"
#define MAC_DECODE_MA_1819_112_2_17_22_49_37_2978 "MAC: single tbf get slot mask, not on starting time, st = %d, fn = %d"
#define MAC_DECODE_MA_1936_112_2_17_22_49_38_2979 "absolute frame number"
#define MAC_DECODE_MA_1945_112_2_17_22_49_38_2980 "MAC:MACMA_DecodeUlMultiBlockAllocation:%d"
#define MAC_DECODE_MA_2008_112_2_17_22_49_38_2981 "MAC: Decode freq para error, decode ma error,freq no in band!"
#define MAC_DECODE_MA_2032_112_2_17_22_49_38_2982 "MAC: Decode packet channel descri freq error(invalid ma number)!\n"
#define MAC_DECODE_MA_2048_112_2_17_22_49_38_2983 "MAC: Decode packet channel descri freq error(change mark not match)\n"
#define MAC_DECODE_MA_2066_112_2_17_22_49_38_2984 "MAC: Decode packet channel descri freq error(num=0)\n"
#define MAC_DECODE_MA_2098_112_2_17_22_49_38_2985 "MAC: invalid polling frame number=%d"
#define MAC_DECODE_MA_2125_112_2_17_22_49_38_2986 "MAC:Invalid control ack type for response imm ass!"
#define MAC_DECODE_MA_2190_112_2_17_22_49_38_2987 "MAC: Decode ia rest octets error, hh not present!"
#define MAC_DECODE_MA_2325_112_2_17_22_49_38_2988 "MAC: network alloc fix allocation,ms not support!"
#define MAC_DECODE_MA_2455_112_2_17_22_49_39_2989 "MAC: network alloc fix allocation,ms not support!"
#define MAC_DECODE_MA_2509_112_2_17_22_49_39_2990 "MAC: tlli mismatch in DL assi! MS: %x  Assi: %x"
#define MAC_DECODE_MA_2512_112_2_17_22_49_39_2991 "MAC: match the old tlli"
#define MAC_DECODE_MA_2516_112_2_17_22_49_39_2992 "MAC: match the new tlli"
#define MAC_DECODE_MA_2596_112_2_17_22_49_39_2993 "MAC: !!tbf_mode in imm assi is different from current!!"
#define MAC_DECODE_MA_2617_112_2_17_22_49_39_2994 "MAC: !!EGPRS imm DL assi,but MS don't support EGPRS!!"
#define MAC_DECODE_MA_2711_112_2_17_22_49_39_2995 "MAC: No ta_value in global_timing_advance!"
#define MAC_DECODE_MA_2781_112_2_17_22_49_39_2996 "MAC: No valid tfi to encode in psi status msg"
#define MAC_DECODE_MA_2794_112_2_17_22_49_39_2997 "MAC: rlc add %d in control queue"
#define MAC_DECODE_MA_3119_112_2_17_22_49_40_2998 "MAC: prach persist_level is wrong!"
#define MAC_PH_410_112_2_17_22_49_41_2999 "MAC:del num > amount in control queue,check!"
#define MAC_PH_540_112_2_17_22_49_41_3000 "MAC: add the second timeslot polling data!"
#define MAC_PH_564_112_2_17_22_49_41_3001 "MAC: replace the more than 2 timeslot polling data!"
#define MAC_PH_569_112_2_17_22_49_41_3002 "MAC: discard the more than 2 timeslot polling data!"
#define MAC_PH_940_112_2_17_22_49_42_3003 "MAC:MS band type error,not support! band = %d, ms_band = %d"
#define MAC_PH_1021_112_2_17_22_49_42_3004 "MAC:Wrong band indicator!!"
#define MAC_PH_1023_112_2_17_22_49_42_3005 "MAC: get actual band = %d"
#define MAC_PH_1071_112_2_17_22_49_42_3006 "MAC:Wrong band indicator!!"
#define MAC_PH_1076_112_2_17_22_49_43_3007 "MAC:error! the required additional access technologies currently not support!!!"
#define MAC_PH_1131_112_2_17_22_49_43_3008 "MAC:requested access type %d not supported!"
#define MAC_PH_1323_112_2_17_22_49_43_3009 "MAC:can only encode %d access tech type,the next should be encoded is"
#define MAC_PH_1771_112_2_17_22_49_44_3010 "MAC:Encode ARAC, but no access technology in ul assign!!!"
#define MAC_PH_2003_112_2_17_22_49_44_3011 "MAC: get block %d to send in poll"
#define MAC_PH_2067_112_2_17_22_49_45_3012 "MAC:send packet resource request, don't start 3168"
#define MAC_PH_2350_112_2_17_22_49_45_3013 "MAC:Mac is already start t3192 when send dl ack!"
#define MAC_PH_2426_112_2_17_22_49_45_3014 "MAC:link_quality_meas_mode =%d!!\n"
#define MAC_PH_2510_112_2_17_22_49_45_3015 "MAC:Mac is already start t3192 when send egprs dl ack!"
#define MAC_PH_3034_112_2_17_22_49_46_3016 "MAC: polling message type error"
#define MM_CONNECTION_MANAGE_774_112_2_17_22_49_59_3017 "MM TASK: MMCNEC_OutGoingMm_RrSyncInd():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_828_112_2_17_22_50_0_3018 "MM TASK: MMCNEC_AdditionalMmConnect_RrSyncInd():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_985_112_2_17_22_50_0_3019 "MM TASK: MMCNEC_HandleCmPromptRsp():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_1014_112_2_17_22_50_0_3020 "MM_TASK: receive CM_MM_PROMPT_RSP in wrong state"
#define MM_CONNECTION_MANAGE_1154_112_2_17_22_50_0_3021 "MM TASK: MMCNEC_HandleRrRelIndConnect():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_1170_112_2_17_22_50_0_3022 "MM TASK: MMCNEC_HandleRrRelIndConnect():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_1248_112_2_17_22_50_0_3023 "MM TASK: MMCNEC_HandleRrAbortIndConnect():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_1268_112_2_17_22_50_1_3024 "MM TASK: MMCNEC_HandleRrAbortIndConnect():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_1281_112_2_17_22_50_1_3025 "MM TASK: MMCNEC_HandleRrAbortIndConnect():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_1407_112_2_17_22_50_1_3026 "MM TASK: MNCNEC_WaitOutgoingMm_T3230Exp():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_1470_112_2_17_22_50_1_3027 "MM_TASK:MMCNEC_TiAllocation, pd=%d, ti=%d"
#define MM_CONNECTION_MANAGE_1520_112_2_17_22_50_1_3028 "MM_TASK:MMCNEC_ClearTi, pd=%d, ti=%d"
#define MM_CONNECTION_MANAGE_1656_112_2_17_22_50_1_3029 "MM TASK: MMCNEC_WaitOutgoingMm_RrRelInd():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_1680_112_2_17_22_50_1_3030 "MM TASK: MMCNEC_WaitRrRelease_RrRelInd():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_1727_112_2_17_22_50_1_3031 "MM:begin saved call"
#define MM_CONNECTION_MANAGE_2169_112_2_17_22_50_2_3032 "MM TASK: MMCNEC_OutGoingMm_CmRelReq():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_2185_112_2_17_22_50_2_3033 "MM TASK: MMCNEC_WaitOutgoingMm_RrRelInd():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_2192_112_2_17_22_50_2_3034 "MM TASK: mm connection is not exist for the pd&ti!"
#define MM_CONNECTION_MANAGE_2215_112_2_17_22_50_2_3035 "MM TASK: MMCNEC_MmConnectActive_CmRelReq():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_2228_112_2_17_22_50_2_3036 "MM TASK: mm connection is not exist for the pd&ti!"
#define MM_CONNECTION_MANAGE_2255_112_2_17_22_50_2_3037 "MM TASK: MMCNEC_AdditionalMmConnect_CmRelReq():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_2267_112_2_17_22_50_2_3038 "MM TASK: mm connection is not exist for the pd&ti!"
#define MM_CONNECTION_MANAGE_2289_112_2_17_22_50_3_3039 "MM TASK: MMCNEC_WaitCmResponse_CmRelReq():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_2311_112_2_17_22_50_3_3040 "MM TASK: mm connection is not exist for the pd&ti!"
#define MM_CONNECTION_MANAGE_2330_112_2_17_22_50_3_3041 "MM TASK: MMCNEC_WaitRrReest_CmRelReq():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_2349_112_2_17_22_50_3_3042 "MM TASK: mm connection is not exist for the pd&ti!"
#define MM_CONNECTION_MANAGE_2377_112_2_17_22_50_3_3043 "MM TASK: MMCNEC_HandleCmServiceAcceptMsg():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_2452_112_2_17_22_50_3_3044 "MM TASK: MMCNEC_HandleCmServiceRejMsg():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_2471_112_2_17_22_50_3_3045 "MM TASK: MMCNEC_HandleCmServiceRejMsg():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_2521_112_2_17_22_50_3_3046 "MM TASK: MMCNEC_HandleAbortMsg():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_2631_112_2_17_22_50_3_3047 "MM TASK: MMCNEC_HandleOtherPdMsg():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_2676_112_2_17_22_50_3_3048 "MM TASK: MMCNEC_HandleMtMsg():mm_connection_counter=%d"
#define MM_CONNECTION_MANAGE_2717_112_2_17_22_50_3_3049 "MM:begin saved call"
#define MM_GMM_PROCEDURE_133_112_2_17_22_50_4_3050 "MM: begin saved call"
#define MM_MAIN_822_112_2_17_22_50_6_3051 "MM: receive MN_MM_REG_REQ, discard"
#define MM_MAIN_906_112_2_17_22_50_6_3052 "MM: receive MN_MM_CAMPON_REQ, idle state %d"
#define MM_MAIN_955_112_2_17_22_50_6_3053 "MM: wait sim reading"
#define MM_MAIN_1110_112_2_17_22_50_7_3054 "MM: HPLMN updated! reselect PLMN"
#define MM_MAIN_1203_112_2_17_22_50_7_3055 "MM: receive MN_MM_INIT_REQ"
#define MM_MAIN_1404_112_2_17_22_50_7_3056 "MM: read sim data error"
#define MM_MAIN_1462_112_2_17_22_50_7_3057 "MM: wrong state %d receive MM_SIM_DATA_CNF, discard"
#define MM_MAIN_1482_112_2_17_22_50_8_3058 "MM: SIM Auth Error %d"
#define MM_MAIN_1515_112_2_17_22_50_8_3059 "MM: receive AUTH_CNF in wrong procedure %d"
#define MM_MAIN_1635_112_2_17_22_50_8_3060 "MM: receive MM_RR_REL_IND in wrong procedure %d"
#define MM_MAIN_1728_112_2_17_22_50_8_3061 "MM: discard MM_RR_DATA_IND in IMSI detach"
#define MM_MAIN_1766_112_2_17_22_50_8_3062 "MM: receive ABORT IND in idle state, discard it"
#define MM_MAIN_1939_112_2_17_22_50_8_3063 "MM: receive MM_RR_FAILED_MSG_IND in wrong connect substate %d"
#define MM_MAIN_1944_112_2_17_22_50_8_3064 "MM: receive MM_RR_FAILED_MSG_IND in wrong procedure %d"
#define MM_MAIN_1965_112_2_17_22_50_8_3065 "MM: receive pro ind in wrong state %d, discard it"
#define MM_MAIN_2021_112_2_17_22_50_9_3066 "MM: receive pro success in wrong state, discard it"
#define MM_MAIN_2045_112_2_17_22_50_9_3067 "MM: receive pro success in wrong state, discard it"
#define MM_MAIN_2072_112_2_17_22_50_9_3068 "MM: receive gmm detach ind in wrong state, discard it"
#define MM_MAIN_2116_112_2_17_22_50_9_3069 "MM: receive gmm detach ind in wrong state, discard it"
#define MM_MAIN_2166_112_2_17_22_50_9_3070 "MM: receive T3210_EXP_IND in wrong procedure %d"
#define MM_MAIN_2269_112_2_17_22_50_9_3071 "MM: receive T3220_EXP_IND in wrong procedure %d"
#define MM_MAIN_2298_112_2_17_22_50_9_3072 "MM: receive T3230_EXP_IND in wrong substate %d"
#define MM_MAIN_2303_112_2_17_22_50_9_3073 "MM: receive T3230_EXP_IND in wrong procedure %d"
#define MM_MAIN_2339_112_2_17_22_50_9_3074 "MM: receive T3240_EXP_IND in wrong procedure %d"
#define MM_MAIN_2402_112_2_17_22_50_9_3075 "MM: begin saved call"
#define MM_MSG_452_112_2_17_22_50_10_3076 "MM: decode CM service reject msg,MANDATORY_IE_CONTENT_ERROR"
#define MM_MSG_498_112_2_17_22_50_11_3077 "MM CODEC: Assert Failure, DecodeMobileIdentity: offset %d error, there is no MobileIdentity to decode"
#define MM_MSG_755_112_2_17_22_50_11_3078 "MM: MM_IsIeiValid temp_byte=%d, offset = %d, l3_msg_len = %d"
#define MM_MSG_776_112_2_17_22_50_11_3079 "MM: decode Msg Header,MESSAGE_TOO_SHORT"
#define MM_MSG_804_112_2_17_22_50_11_3080 "MM: decode Msg Header,SKIP_INDICATOR_NOT_ZERO"
#define MM_MSG_838_112_2_17_22_50_11_3081 "MM: decode Msg Header,INVALID_MESSAGE_TYPE"
#define MM_MSG_850_112_2_17_22_50_11_3082 "MM: decode Msg Header,INVALID_PROTOCOL_DISCRIMINATOR"
#define MM_MSG_876_112_2_17_22_50_11_3083 "MM CODEC: Assert Failure, EncodeCMServiceReq: CKSN %d error, change to invalid"
#define MM_MSG_970_112_2_17_22_50_12_3084 "MM: decode Location updating accept msg,min len error."
#define MM_MSG_981_112_2_17_22_50_12_3085 "MM: decode Location updating accept msg,LAI invalid."
#define MM_MSG_989_112_2_17_22_50_12_3086 "MM:Location updating accept's LAI != current cell LAI"
#define MM_MSG_1012_112_2_17_22_50_12_3087 "MM: decode Location updating accept msg,%d"
#define MM_MSG_1036_112_2_17_22_50_12_3088 "MM: decode MM_MOBILE_IDENTITY_IEI failure"
#define MM_MSG_1082_112_2_17_22_50_12_3089 "MM: decode MM_EMERGENCY_NUM_LIST_IEI failure"
#define MM_MSG_1087_112_2_17_22_50_12_3090 "MM: receive wrong msg %d"
#define MM_MSG_1111_112_2_17_22_50_12_3091 "MM: decode Location updating reject msg,MANDATORY_IE_CONTENT_ERROR"
#define MM_MSG_1136_112_2_17_22_50_12_3092 "MM CODEC: Assert Failure, EncodeLocUpdReq: CKSN %d error, change to invalid"
#define MM_MSG_1200_112_2_17_22_50_12_3093 "MM: decode Authentication request msg,MANDATORY_IE_CONTENT_ERROR"
#define MM_MSG_1260_112_2_17_22_50_12_3094 "MM: decode Identity request msg,MANDATORY_IE_CONTENT_ERROR"
#define MM_MSG_1274_112_2_17_22_50_12_3095 "MM: decode Identity request msg,MANDATORY_IE_CONTENT_ERROR"
#define MM_MSG_1343_112_2_17_22_50_12_3096 "MM: decode TMSI reallocation command msg,min len error"
#define MM_MSG_1351_112_2_17_22_50_12_3097 "MM: decode TMSI reallocation command msg,LAI failure"
#define MM_MSG_1359_112_2_17_22_50_12_3098 "MM:TMSI reallocation command's LAI != current cell LAI"
#define MM_MSG_1371_112_2_17_22_50_13_3099 "MM: decode TMSI reallocation command msg,Mobile Identity failure"
#define MM_MSG_1413_112_2_17_22_50_13_3100 "MM: decode CM service reject msg,MANDATORY_IE_CONTENT_ERROR"
#define MM_MSG_1458_112_2_17_22_50_13_3101 "MM CODEC: Assert Failure, EncodeCMReestReq: CKSN %d error, change to invalid"
#define MM_MSG_1520_112_2_17_22_50_13_3102 "MM: decode Abort msg,MANDATORY_IE_CONTENT_ERROR"
#define MM_MSG_1543_112_2_17_22_50_13_3103 "MM CODEC: Assert Failure, EncodeMMStatus: Reject cause %d error, set to Protocol error, unspecified"
#define MM_PLMN_SELECT_503_112_2_17_22_50_14_3104 "MM: discard additional act ind"
#define MM_PLMN_SELECT_521_112_2_17_22_50_14_3105 "MM: sim present but in NO SIM state!"
#define MM_PLMN_SELECT_526_112_2_17_22_50_14_3106 "MM: discard additional act ind"
#define MM_PLMN_SELECT_705_112_2_17_22_50_15_3107 "MM: AddPLMNToPLMNList,plmn:mcc=%d,mnc=%d,mcc_digit_num = %dn"
#define MM_PLMN_SELECT_783_112_2_17_22_50_15_3108 "MM: AddPLMNToAvailablePLMNList,plmn:mcc=%d,mnc=%d,mnc_digit_num = %dn"
#define MM_PLMN_SELECT_819_112_2_17_22_50_15_3109 "MM: add manual plmn (%d, %d)"
#define MM_PLMN_SELECT_829_112_2_17_22_50_15_3110 "MM: disable roaming, add home plmn (%d, %d)"
#define MM_PLMN_SELECT_833_112_2_17_22_50_15_3111 "MM: auto select mode, add prefer select plmn"
#define MM_PLMN_SELECT_881_112_2_17_22_50_15_3112 "MM: add registered plmn (%d, %d)"
#define MM_PLMN_SELECT_890_112_2_17_22_50_15_3113 "MM: CreatePlmnSelectTable, read NV_RPLMN is_enable, mcc %d, mnc %d, mnc_digit_num %d"
#define MM_PLMN_SELECT_900_112_2_17_22_50_15_3114 "MM: CreatePlmnSelectTable, read NV_RPLMN nv error"
#define MM_PLMN_SELECT_943_112_2_17_22_50_15_3115 "MM: add PLMN Selector data file plmn, validate_num=%d"
#define MM_PLMN_SELECT_1060_112_2_17_22_50_15_3116 "MM: get from select plmn list"
#define MM_PLMN_SELECT_1070_112_2_17_22_50_15_3117 "MM: get from available plmn list"
#define MM_PLMN_SELECT_1082_112_2_17_22_50_15_3118 "MM: manual mode and all plmn searched, select the user selected plmn(%d, %d)"
#define MM_PLMN_SELECT_1097_112_2_17_22_50_15_3119 "MM: forbid roaming and all plmn searched, select hplmn"
#define MM_PLMN_SELECT_1106_112_2_17_22_50_15_3120 "MM: auto mode and all plmn searched, select rplmn or hplmn (%d, %d)"
#define MM_PLMN_SELECT_1144_112_2_17_22_50_16_3121 "MM: GetPlmnFromSelectPlmnList result = %d"
#define MM_PLMN_SELECT_1178_112_2_17_22_50_16_3122 "MM: GetPLMNFromAvailableList result = %d"
#define MM_PLMN_SELECT_1203_112_2_17_22_50_16_3123 "MM: plmn (%d, %d),IsPlmnInSelectPlmnList result = %d"
#define MM_PLMN_SELECT_1221_112_2_17_22_50_16_3124 "MM: plmn(%d, %d), MMPLMN_FindPlmnInSelectedList result = %d"
#define MM_PLMN_SELECT_1255_112_2_17_22_50_16_3125 "MM: plmn(%d, %d), MMPLMN_FindPlmnInAvailList result = %d"
#define MM_PLMN_SELECT_1287_112_2_17_22_50_16_3126 "MM: find la (%d,%d,%d) in flai_s"
#define MM_PLMN_SELECT_1305_112_2_17_22_50_16_3127 "MM: find la (%d,%d,%d) in flai_r"
#define MM_PLMN_SELECT_1317_112_2_17_22_50_16_3128 "MM: all la in plmn(%d, %d) is flai"
#define MM_PLMN_SELECT_1363_112_2_17_22_50_16_3129 "MM: sim is not present but state %d is wrong"
#define MM_PLMN_SELECT_1444_112_2_17_22_50_16_3130 "MM: modified act type %d, ignore_forbid_list %d, select_any_plmn %d"
#define MM_PLMN_SELECT_1597_112_2_17_22_50_16_3131 "MM: lai in fplmn list"
#define MM_PLMN_SELECT_1602_112_2_17_22_50_16_3132 "MM: lai in flai for roaming list"
#define MM_PLMN_SELECT_1607_112_2_17_22_50_16_3133 "MM: lai in flai for LA not allowed list"
#define MM_PLMN_SELECT_1703_112_2_17_22_50_17_3134 "MM: begin saved call"
#define MM_PLMN_SELECT_1734_112_2_17_22_50_17_3135 "MM: begin saved call"
#define MM_PLMN_SELECT_1770_112_2_17_22_50_17_3136 "MM: begin saved call"
#define MM_PLMN_SELECT_1799_112_2_17_22_50_17_3137 "MM: begin saved call"
#define MM_PLMN_SELECT_1997_112_2_17_22_50_17_3138 "MM: begin saved call"
#define MM_PLMN_SELECT_2646_112_2_17_22_50_19_3139 "MM: network mode I and gprs support by MS, wait GMM do ATTACH/RAU"
#define MM_PLMN_SELECT_2686_112_2_17_22_50_19_3140 "MM: MS not support CS domain, do nothing"
#define MM_PLMN_SELECT_2745_112_2_17_22_50_19_3141 "MM: MS not support CS domain, do nothing"
#define MM_PLMN_SELECT_2859_112_2_17_22_50_19_3142 "MM: need period search PLMN"
#define MM_PLMN_SELECT_2872_112_2_17_22_50_19_3143 "MM: in manual mode and current PLMN is not the user select PLMN, start 1min timer"
#define MM_PLMN_SELECT_2882_112_2_17_22_50_19_3144 "MM: in FLAI_S or 4 LU attemp fail, start 1min timer"
#define MM_PLMN_SELECT_2890_112_2_17_22_50_19_3145 "MM: is_mm_hplmn_search %d"
#define MM_PLMN_SELECT_2898_112_2_17_22_50_19_3146 "MM: start 3min search timer after power on"
#define MM_PLMN_SELECT_2904_112_2_17_22_50_19_3147 "MM: start %d s search timer periodically"
#define MM_PLMN_SELECT_2913_112_2_17_22_50_19_3148 "MM: in HPLMN or EPLMN, not need start timer in normal mode"
#define MM_PLMN_SELECT_2924_112_2_17_22_50_19_3149 "MM: in HPLMN or EPLMN, but LU fail 4 times, start 1min timer search"
#define MM_PLMN_SELECT_3095_112_2_17_22_50_20_3150 "MM: MmRrPlmnList, read NV_RPLMN is_enable, mcc %d, mnc %d, mnc_digit_num %d"
#define MM_PLMN_SELECT_3111_112_2_17_22_50_20_3151 "MM: Find the NV_RPLMN plmn (%d, %d) in availabel PLMN list"
#define MM_PLMN_SELECT_3122_112_2_17_22_50_20_3152 "MM: Find a higher priority plmn (%d, %d)"
#define MM_PLMN_SELECT_3136_112_2_17_22_50_20_3153 "MM: Find a registered plmn (%d, %d) in limit state"
#define MM_PLMN_SELECT_3152_112_2_17_22_50_20_3154 "MM: Find a unknown plmn (%d, %d) in limit state"
#define MM_PLMN_SELECT_3167_112_2_17_22_50_20_3155 "MM: Find hplmn in limit state"
#define MM_PLMN_SELECT_3192_112_2_17_22_50_20_3156 "MM: Find user select plmn (%d, %d), try to camp it"
#define MM_PLMN_SELECT_3362_112_2_17_22_50_20_3157 "MNPHONE:  add PLMN Selector data file plmn, validate_num=%d"
#define MM_SIGNAL_1030_112_2_17_22_50_23_3158 "MM: unknown PD: %d"
#define MM_SPECIFIC_PROCEDURE_268_112_2_17_22_50_25_3159 "MM: Send Lu First Time, reset all LU counter and timer"
#define MM_SPECIFIC_PROCEDURE_393_112_2_17_22_50_25_3160 "MM: receive MM RR ABORT IND in wrong sub state"
#define MM_SPECIFIC_PROCEDURE_574_112_2_17_22_50_25_3161 "MM: receive data from network in wrong LU state"
#define MM_SPECIFIC_PROCEDURE_619_112_2_17_22_50_25_3162 "MM: PD Error %d in LUP_INITIATED state"
#define MM_SPECIFIC_PROCEDURE_629_112_2_17_22_50_25_3163 "MM: Decode Error %d in LUP_INITIATED state"
#define MM_SPECIFIC_PROCEDURE_717_112_2_17_22_50_25_3164 "MM: Lup Acc identity type error %d"
#define MM_SPECIFIC_PROCEDURE_767_112_2_17_22_50_26_3165 "MM: LOC UPD Acc Msg decode error %d"
#define MM_SPECIFIC_PROCEDURE_1000_112_2_17_22_50_26_3166 "MM: begin saved CM request"
#define MM_SPECIFIC_PROCEDURE_1049_112_2_17_22_50_26_3167 "MM:receive MM_PD MSG at WAIT_FOR_NETWORK_COMMAND\n"
#define MM_SPECIFIC_PROCEDURE_1127_112_2_17_22_50_26_3168 "MM: receive MM RR REL IND in wrong sub state"
#define MM_SPECIFIC_PROCEDURE_1394_112_2_17_22_50_27_3169 "MM: current T3212 duration is %d"
#define MM_SPECIFIC_PROCEDURE_1410_112_2_17_22_50_27_3170 "MM: Start T3212, value %d"
#define MM_SPECIFIC_PROCEDURE_1450_112_2_17_22_50_27_3171 "MM: Common Procedure msg_type not support %d"
#define MM_SPECIFIC_PROCEDURE_1458_112_2_17_22_50_27_3172 "MM: Common Procedure PD Error %d"
#define MM_SPECIFIC_PROCEDURE_1493_112_2_17_22_50_27_3173 "MM: Auth Req Msg decode error %d"
#define MM_SPECIFIC_PROCEDURE_1561_112_2_17_22_50_27_3174 "MM: Identity Req Msg decode error %d"
#define MM_SPECIFIC_PROCEDURE_1604_112_2_17_22_50_27_3175 "MM: TmisReallocComm iden type %d error"
#define MM_SPECIFIC_PROCEDURE_1622_112_2_17_22_50_27_3176 "MM: TMSI reallocation command Msg decode error %d"
#define MM_SPECIFIC_PROCEDURE_1653_112_2_17_22_50_27_3177 "MM: MM Information Msg decode error %d"
#define MM_SPECIFIC_PROCEDURE_1761_112_2_17_22_50_28_3178 "MM: Identity type error %d"
#define MM_TIMER_81_112_2_17_22_50_32_3179 "MM_SetTimer timer_type = %d"
#define MM_TIMER_107_112_2_17_22_50_32_3180 "MM_KillTimer timer_type = %d"
#define MM_TIMER_149_112_2_17_22_50_32_3181 "MM_IsTimerActive timer_type = %d"
#define RLC_DOWNLINK_612_112_2_17_22_50_35_3182 "RLC: too many option octet in data block: %d\n"
#define RLC_DOWNLINK_618_112_2_17_22_50_35_3183 "RLC: total data len error in data block: %d\n"
#define RLC_DOWNLINK_695_112_2_17_22_50_35_3184 "RLC: too many option octet in data block: %d\n"
#define RLC_DOWNLINK_701_112_2_17_22_50_35_3185 "RLC: total data len error in data block: %d\n"
#define RLC_DOWNLINK_897_112_2_17_22_50_35_3186 "RLC: Reserved M+E: ME = '00'B\n"
#define RLC_DOWNLINK_998_112_2_17_22_50_36_3187 "RLC: Error option octet: LI = %d, E = %d\n"
#define RLC_SCT_793_112_2_17_22_50_40_3188 "rlcul: in loopback mode no data recved yet"
#define RLC_TX_PDU_POOL_73_112_2_17_22_50_43_3189 "rlcul: alloc pdu, pid = %d"
#define RLC_TX_PDU_POOL_96_112_2_17_22_50_43_3190 "rlcul: free pdu, pid = %d"
#define RLC_UPLINK_447_112_2_17_22_50_44_3191 "rlcul: network support extended uplink TBF: %d"
#define RLC_UPLINK_679_112_2_17_22_50_44_3192 "rlcul: MS support extended uplink TBF: %d"
#define RLC_UPLINK_1084_112_2_17_22_50_45_3193 "rlcul: release error!"
#define RLC_UPLINK_2915_112_2_17_22_50_49_3194 "rlcul: all data block have been send, still get data block\n"
#define RLC_UPLINK_2926_112_2_17_22_50_49_3195 "rlcul: error! can't select any pending block"
#define RLC_UPLINK_3184_112_2_17_22_50_50_3196 "rlcul: error! can't select any pending block"
#define RLC_UPLINK_3723_112_2_17_22_50_51_3197 "rlcul: error! can't select any pending block"
#define RLC_UPLINK_4217_112_2_17_22_50_52_3198 "rlcul: gprs tx blk cs%d bsn = %d, va = %d, vs = %d"
#define RLC_UPLINK_4226_112_2_17_22_50_52_3199 "rlcul: gprs retx %s blk cs%d bsn = %d, va = %d, vs = %d"
#define RLC_UPLINK_4340_112_2_17_22_50_52_3200 "rlcul: egprs tx blk mcs%d bsn = %d, va = %d, vs = %d"
#define RLC_UPLINK_4349_112_2_17_22_50_52_3201 "rlcul: egprs retx %s blk mcs%d bsn = %d, va = %d, vs = %d"
#define RLC_UPLINK_4371_112_2_17_22_50_52_3202 "rlcul: egprs retx %s blk mcs%d bsn = %d, va = %d, vs = %d"
#define RLC_UPLINK_4377_112_2_17_22_50_52_3203 "rlcul: egprs tx blk mcs%d bsn = %d, va = %d, vs = %d"
#define RLC_UPLINK_4587_112_2_17_22_50_52_3204 "rlcul: in RLCUP_ProcessAckUnackMsg sendnumcnf"
#define RLC_UPLINK_4796_112_2_17_22_50_53_3205 "rlcul: decode crbb error!\n"
#define RLC_UPLINK_4914_112_2_17_22_50_53_3206 "rlcul: in RLCUP_EGPRS_ProcessAckNackMsg sendnumcnf"
#define RLC_UPLINK_5261_112_2_17_22_50_54_3207 "rlcul: in RLCUP_SetDataEnd sendnumcnf"
#define RLC_UPLINK_5354_112_2_17_22_50_54_3208 "rlcul: in RLCUP_SetDataEnd sendnumcnf"
#define RLC_UPLINK_6270_112_2_17_22_50_56_3209 "rlcul:Error!unkown tlli_present!"
#define RLC_UPLINK_6289_112_2_17_22_50_56_3210 "rlcul: in change tlli sendnumcnf"
#define RLC_UPLINK_6324_112_2_17_22_50_56_3211 "rlcul: update timeslot mismatch! Error"
#define RLC_UPLINK_6508_112_2_17_22_50_56_3212 "rlcul: set flow ctrl, llc num = %d, spare num = %d, line = %d"
#define RLC_UPLINK_6521_112_2_17_22_50_56_3213 "rlcul: set flow ctrl, octet num = %d, line = %d"
#define RLC_UPLINK_6547_112_2_17_22_50_56_3214 "rlcul: reset flow ctrl, line = %d"
#define RR_CSN_MSG_4283_112_2_17_22_51_39_3215 "RRC:received System Information 9, not supported now!"
#define RR_CSN_MSG_4307_112_2_17_22_51_39_3216 "RRC:received System Information 16, not supported now!"
#define RR_CSN_MSG_4317_112_2_17_22_51_39_3217 "RRC:received System Information 17, not supported now!"
#define RR_CSN_MSG_4327_112_2_17_22_51_39_3218 "RRC:received System Information 18, not supported now!"
#define RR_CSN_MSG_4337_112_2_17_22_51_39_3219 "RRC:received System Information 19, not supported now!"
#define RR_CSN_MSG_4348_112_2_17_22_51_39_3220 "RRC:received System Information 20, not supported now!"
#define RR_CSN_MSG_6523_112_2_17_22_51_43_3221 "RRC: 2 quater,  index %d, count %d"
#define RR_CSN_MSG_6634_112_2_17_22_51_44_3222 "rrc: no Optional Selection Parameters in Si4RestOctets, line = %d"
#define RR_CSN_MSG_6697_112_2_17_22_51_44_3223 "RRC: si2quater available"
#define RR_CSN_MSG_6722_112_2_17_22_51_44_3224 "rrc: no Optional Selection Parameters in Si4RestOctets, line = %d"
#define RR_CSN_MSG_6740_112_2_17_22_51_44_3225 "RRC:receive LSA connent in System Information 8,not support!line=%d"
#define RR_CSN_MSG_6860_112_2_17_22_51_44_3226 "RRC: SGSNR %d"
#define RR_CSN_MSG_6870_112_2_17_22_51_44_3227 "RRC: si_status_ind %d"
#define RRCELL_617_112_2_17_22_51_52_3228 "RRC: ref_sen %d, num %d"
#define RRCELL_622_112_2_17_22_51_52_3229 "RRC: ref_sen %d, num %d"
#define RRCELL_627_112_2_17_22_51_52_3230 "RRC: ref_sen is 0, num %d"
#define RRCELL_647_112_2_17_22_51_52_3231 "RRC: clean rxlev count %d"
#define RRCELL_947_112_2_17_22_51_53_3232 "RRC: In RRCELL_SortRssi, freq_count=%d"
#define RRCELL_953_112_2_17_22_51_53_3233 "RRC:A = 0x%x, B %d, %d\n"
#define RRCELL_1240_112_2_17_22_51_53_3234 "RRC: %d In UpdateNCellRssi"
#define RRCELL_1319_112_2_17_22_51_54_3235 "RRC:RR NCELL: cell from store to temp, arfcn = %d, bsic = %d"
#define RRCELL_1406_112_2_17_22_51_54_3236 "RRC:RR NCELL: cell from backup to store, arfcn = %d, bsic = %d"
#define RRCELL_1441_112_2_17_22_51_54_3237 "RRC: NCELL cell from temp to backup, arfcn = %d, bsic = %d"
#define RRCELL_1643_112_2_17_22_51_54_3238 "RRC: arfcn %d, si7_not_present %d, "
#define RRCELL_1651_112_2_17_22_51_54_3239 "RRC: arfcn %d, si8_not_present %d, "
#define RRCELL_1817_112_2_17_22_51_55_3240 "RRC: assert unknown si %d"
#define RRCELL_1854_112_2_17_22_51_55_3241 "RRC: %d checkncellSi, cur %d"
#define RRCELL_2145_112_2_17_22_51_55_3242 "RRC: GetReselPriority cell_ptr->cell_prio = %d "
#define RRCELL_2159_112_2_17_22_51_55_3243 "RRC: %d reselprior %d arfcn 0x%x c1 %d"
#define RRCELL_2163_112_2_17_22_51_55_3244 "RRC: %d cell prio NO_CELL arfcn = %d, c1 = %d\n"
#define RRCELL_2268_112_2_17_22_51_56_3245 "RRC:In RRCELL_UpdateBcchSweepResult\n"
#define RRCELL_2269_112_2_17_22_51_56_3246 "RRC:ARFCN = %d, rssi = %d \n"
#define RRCELL_3135_112_2_17_22_51_57_3247 "RRC: NCELL cell to store, arfcn = %d, bsic = %d"
#define RRCELL_3151_112_2_17_22_51_57_3248 "RRC: NCELL cell clear, arfcn = %d, bsic = %d"
#define RRCELL_3546_112_2_17_22_51_58_3249 "RRC: assert error, exceptional exp"
#define RRCELL_3557_112_2_17_22_51_58_3250 "RRC: assert error, exceptional exp"
#define RRCELL_3611_112_2_17_22_51_58_3251 "RRC: %d g_ind %d, 13 %d, ba-ext %d, 2bis %d, 2t_ind %d, 2ter %d"
#define RRCELL_3654_112_2_17_22_51_58_3252 "RRC: %d EssSiBad %d %d %d %d"
#define RRCELL_3920_112_2_17_22_51_59_3253 "RRC: %d UpdateBadSi tc 0x%x, pos %d"
#define RRCELL_3939_112_2_17_22_51_59_3254 "RRC: %d bad si tomax, cell a 0x%x, fn 0x%x"
#define RRCELL_4014_112_2_17_22_51_59_3255 "RRC: %d BadSiTO MAX tc 0x%x, pos %d"
#define RRCELL_4131_112_2_17_22_51_59_3256 "RRC: assert error, illogical si, tc %d, pos %d, fn %d"
#define RRCELL_4162_112_2_17_22_51_59_3257 "RRC: %d gprs_ind %d, si13 %d, ba-ext %d, si2bis %d, 2ter_ind %d, 2ter %d"
#define RRCELL_4209_112_2_17_22_52_0_3258 "RRC:%d ALL SI OK"
#define RRCELL_4214_112_2_17_22_52_0_3259 "RRC:%d cell exp, set allneedsi to true"
#define RRCELL_4235_112_2_17_22_52_0_3260 "RRC: %d IsEssSiOk T, a 0x%x"
#define RRCELL_4240_112_2_17_22_52_0_3261 "RRC: %d, a 0x%x, p%d,%d %d %d %d"
#define RRCELL_4262_112_2_17_22_52_0_3262 "RRC: %d MaxRssi, %d not max"
#define RRCELL_4266_112_2_17_22_52_0_3263 "RRC: %d MaxRssi %d is max"
#define RRCELL_4293_112_2_17_22_52_0_3264 "RRC: %d %d not bestcell"
#define RRCELL_4298_112_2_17_22_52_0_3265 "RRC: %d %d is bestcell"
#define RRCELL_4327_112_2_17_22_52_0_3266 "RRC: %d %d not bestncell"
#define RRCELL_4332_112_2_17_22_52_0_3267 "RRC: %d %d is bestncell"
#define RRCELL_4355_112_2_17_22_52_0_3268 "RRC: %d SelBestCell"
#define RRCELL_4359_112_2_17_22_52_0_3269 "RRC: cell[%d] p %d, rssi %d, pri %d"
#define RRCELL_4398_112_2_17_22_52_0_3270 "RRC: SelBestHCell sel %d"
#define RRCELL_4405_112_2_17_22_52_0_3271 "RRC: SelBestRCell sel %d"
#define RRCELL_4412_112_2_17_22_52_0_3272 "RRC: SelBestCell sel %d"
#define RRCELL_4463_112_2_17_22_52_0_3273 "RRC: SelBestHCell sel %d"
#define RRCELL_4470_112_2_17_22_52_0_3274 "RRC: SelBestRCell sel %d"
#define RRCELL_4477_112_2_17_22_52_0_3275 "RRC: SelBestCell sel %d"
#define RRCELL_4500_112_2_17_22_52_0_3276 "RRC: SelBestCell sel %d"
#define RRCELL_4541_112_2_17_22_52_0_3277 "RRC: keep cell %d, a 0x %x"
#define RRCELL_4581_112_2_17_22_52_1_3278 "%s"
#define RRCELL_4586_112_2_17_22_52_1_3279 "%s"
#define RRCELL_4680_112_2_17_22_52_1_3280 "RRC: %d CleanExp"
#define RRCELL_4685_112_2_17_22_52_1_3281 "C[%d] p %d, a 0x%x, tc 0x%x, du %d, si %d, %d, %d, %d"
#define RRCELL_4691_112_2_17_22_52_1_3282 "RRC: assert, bsic not ret dur 10s"
#define RRCELL_4803_112_2_17_22_52_1_3283 "invalid bcch pos"
#define RRCELL_4821_112_2_17_22_52_1_3284 "RRC: %d SET a 0x%x to bad %d"
#define RRCELL_4839_112_2_17_22_52_1_3285 "RRC: %d, a 0x%x is bad"
#define RRCELL_4865_112_2_17_22_52_1_3286 "RRC: %d get free rcell, %d,%d,%d,%d,%d,%d"
#define RRCELL_4978_112_2_17_22_52_1_3287 "RRC: %d tc_mask_2 0x%x, rcell tc 0x%x"
#define RRCELL_5140_112_2_17_22_52_2_3288 "RRC: %d plmnscan end s %d, num %d"
#define RRCELL_5155_112_2_17_22_52_2_3289 "RRC: %d plmnend %d"
#define RRCELL_COMMON_482_112_2_17_22_52_4_3290 "RRC: %d clearC p %d a 0x%x"
#define RRCELL_COMMON_503_112_2_17_22_52_4_3291 "RRC: %d clearRc p %d a 0x%x"
#define RRCELL_COMMON_544_112_2_17_22_52_4_3292 "RRC: %d In  RRCELL_IsAcceptableCell\n"
#define RRCELL_COMMON_958_112_2_17_22_52_5_3293 "RRC: %d C1 All Low %d"
#define RRCELL_FAST_PLMN_100_112_2_17_22_52_5_3294 "RRC: assert error bsic>search num"
#define RRCELL_FAST_PLMN_222_112_2_17_22_52_6_3295 "RRC: assert error bcch>search num"
#define RRCELL_FAST_PLMN_298_112_2_17_22_52_6_3296 "RRC: assert error bcch>search num"
#define RRCELL_FAST_PLMN_336_112_2_17_22_52_6_3297 "RRC: %d FillBcch fast p %d, a 0x%x, tc 0x%x"
#define RRCELL_FAST_PLMN_366_112_2_17_22_52_6_3298 "RRC: %d FillBcch old_tc 0x%x, new 0x%x"
#define RRCELL_FAST_PLMN_451_112_2_17_22_52_6_3299 "RRC: %d RRCELL_FastPlmnEnd process,scell_pos %d"
#define RRCELL_FAST_PLMN_457_112_2_17_22_52_6_3300 "RRC: %d check cell %d cell_present %d bsic_exist %d"
#define RRCELL_FAST_PLMN_522_112_2_17_22_52_6_3301 "RRC: %d all cell search complete,return TRUE"
#define RRCELL_FAST_PLMN_535_112_2_17_22_52_6_3302 "RRC: %d select result, pos=%d"
#define RRCELL_FAST_PLMN_549_112_2_17_22_52_6_3303 "RRC: %d select scell, find better ncell %d,return false"
#define RRCELL_FAST_PLMN_564_112_2_17_22_52_6_3304 "RRC: %d select ncell, scell %d is better,return false"
#define RRCELL_FAST_PLMN_579_112_2_17_22_52_6_3305 "RRC: %d select ncell, find better ncell %d,return false"
#define RRCELL_FAST_PLMN_648_112_2_17_22_52_6_3306 "RRC: %d fast sel %d a 0x%x"
#define RRCELL_FAST_PLMN_651_112_2_17_22_52_6_3307 "RRC: %d fast nocell"
#define RRCELL_GPRS_163_112_2_17_22_52_7_3308 "RRC: Assert Failure, unknown extend report type."
#define RRCELL_GPRS_970_112_2_17_22_52_9_3309 "RRC:TLLI in PacketCellChangeOrder is wrong!new_tiil=%d, old_tlli=%d, rec_tlli=%d"
#define RRCELL_GPRS_978_112_2_17_22_52_9_3310 "RRC: pcco get tlli fail"
#define RRCELL_GPRS_996_112_2_17_22_52_9_3311 "RRC: pcco check nc meas param fail"
#define RRCELL_GPRS_1007_112_2_17_22_52_9_3312 "RRC: pcco check lsa fail"
#define RRCELL_GPRS_1018_112_2_17_22_52_9_3313 "RRC: pcco check enhance meas param fail"
#define RRCELL_GPRS_1805_112_2_17_22_52_11_3314 "RRC: cell reselection is controled by network\n"
#define RRCELL_GPRS_1888_112_2_17_22_52_11_3315 "RRC:TLLI in PacketMeasureOrder is wrong!new_tiil=%d, old_tlli=%d, rec_tlli=%d"
#define RRCELL_GPRS_2492_112_2_17_22_52_12_3316 "RRC: pcco check arfcn fail"
#define RRCELL_GPRS_2564_112_2_17_22_52_12_3317 "RRC: scell is barred cba2 = 1\n"
#define RRCELL_GPRS_2573_112_2_17_22_52_12_3318 "RRC: scell c1 <= 0\n"
#define RRCELL_GPRS_2592_112_2_17_22_52_12_3319 "RRC:In  RRCELL_ProcessReselTimerGprs\n"
#define RRCELL_GPRS_2593_112_2_17_22_52_12_3320 "RRC:ARFCN = %d, rssi = %d, c1 = %d, c31 = %d, c32 = %d\n"
#define RRCELL_GPRS_2598_112_2_17_22_52_12_3321 "RRC:ARFCN = %d, rssi = %d, c1 = %d, c31 = %d, c32 = %d\n"
#define RRCELL_GPRS_3143_112_2_17_22_52_13_3322 "RRC: %d scell ba_ext %d, 2bis %d, tc5bad %d"
#define RRCELL_GPRS_3144_112_2_17_22_52_13_3323 "RRC: 2ter_ind %d, 2ter_p %d, tc4bad %d"
#define RRCELL_RESEL_953_112_2_17_22_52_16_3324 "RRC: Select cell 0x%x ignore si when scell access fail."
#define RRCELL_RESEL_1135_112_2_17_22_52_16_3325 "RR: find a cell support emergency call, arfcn = 0x%x"
#define RRCELL_RESEL_1552_112_2_17_22_52_17_3326 "RRC:%d In ReselTimer\n"
#define RRCELL_RESEL_1553_112_2_17_22_52_17_3327 "RRC:A = 0x%x, rssi = %d, c1 = %d, c2 = %d\n"
#define RRCELL_RESEL_1557_112_2_17_22_52_17_3328 "RRC:A = 0x%x, rssi = %d, c1 = %d, c2 = %d\n"
#define RRCELL_RESEL_1569_112_2_17_22_52_17_3329 "RRC resel: scell: c1=%d, c2=%d; max rssi ncell: arfcn=%d, c1=%d, c2=%d; ct=%d"
#define RRCELL_SEARCH_PLMN_263_112_2_17_22_52_18_3330 "RRC: %d ASSERT ERROR, no space when search"
#define RRCELL_SEARCH_PLMN_323_112_2_17_22_52_18_3331 "RRC: %d research num %d"
#define RRCELL_SEARCH_PLMN_376_112_2_17_22_52_18_3332 "RRC: cell[%d] p %d"
#define RRCELL_SEARCH_PLMN_391_112_2_17_22_52_18_3333 "RRC: %d PlmnEnd? s %d, num %d, end %d"
#define RRCELL_SEARCH_PLMN_440_112_2_17_22_52_18_3334 "RRC: %d ASSERT ERROR, no space when search"
#define RRDM_325_112_2_17_22_52_20_3335 "Invalid rat change req mode"
#define RRDM_382_112_2_17_22_52_20_3336 "@@@@@@ SEND class mark to layer 1"
#define RRDM_386_112_2_17_22_52_20_3337 "@@@@@@ DM Get arfcn , arfcn=%d"
#define RRDM_428_112_2_17_22_52_20_3338 "@@@@@@ DM handle ho cmd , message error"
#define RRDM_434_112_2_17_22_52_20_3339 "@@@@@@ DM handle ho cmd , PD or msg-type ERROR"
#define RRDM_440_112_2_17_22_52_20_3340 "@@@@@@ DM call decode function to decode ho cmd"
#define RRDM_534_112_2_17_22_52_20_3341 "@@@@@@ RRDM_CheckArfcnBsic , BSIC not matched"
#define RRDM_935_112_2_17_22_52_21_3342 "DGY_COMP_1"
#define RRDM_950_112_2_17_22_52_21_3343 "DGY_COMP_2"
#define RRDM_966_112_2_17_22_52_21_3344 "DGY_COMP_3"
#define RRDM_974_112_2_17_22_52_21_3345 "BREAK_3"
#define RRDM_1000_112_2_17_22_52_21_3346 "@@@@@@ SEND MPH_STOP_TD_MEAS_REQ to layer 1"
#define RRDM_1019_112_2_17_22_52_21_3347 "TD ncell not present"
#define RRDM_1029_112_2_17_22_52_21_3348 "TD neighbour cell is 0"
#define RRDM_1038_112_2_17_22_52_21_3349 "RRDM_Fill_TD_Meas:AFTER uptade :t_ptr->arfcn_num = %d"
#define RRDM_1050_112_2_17_22_52_21_3350 "RRDM_Fill_TD_Meas:arfcn_list_arr[ %d ] = %d"
#define RRDM_1057_112_2_17_22_52_21_3351 "RRDM_Fill_TD_Meas:cell_param_id_list_arr[ %d ]= %d"
#define RRDM_1062_112_2_17_22_52_21_3352 "RRDM_Fill_TD_Meas:arfcn_list_chg_ind = TRUE"
#define RRDM_1074_112_2_17_22_52_21_3353 "i = %d,arfcn = %d"
#define RRDM_1075_112_2_17_22_52_21_3354 "i = %d,cell_param_id_num = %d"
#define RRDM_1080_112_2_17_22_52_21_3355 "i = %d,arfcn = %d"
#define RRDM_1081_112_2_17_22_52_21_3356 "i = %d,cell_param_id_num = %d"
#define RRDM_1092_112_2_17_22_52_21_3357 "t_ptr->arfcn_num = %d"
#define RRDM_1096_112_2_17_22_52_21_3358 "i = %d,arfcn = %d"
#define RRDM_1097_112_2_17_22_52_21_3359 "i = %d,cell_param_id_num = %d"
#define RRDM_1141_112_2_17_22_52_21_3360 "@@@@@@ Fill td cell table , num = %d"
#define RRDM_1194_112_2_17_22_52_21_3361 "@@@@@@ Fill td cell table , num = %d"
#define RRDM_1275_112_2_17_22_52_21_3362 "MS is not Dule-Mode,does not need to start TD measurement"
#define RRDM_1283_112_2_17_22_52_21_3363 "is_to_fill_td_param = %d,\n meas_param_desc_3g_present = %d,\ngsm_scell.rssi = %d,\nqsearch_i = %d,s_q_search_i_table = %d"
#define RRDM_1290_112_2_17_22_52_21_3364 "START_TD_ALREADY"
#define RRDM_1293_112_2_17_22_52_21_3365 "TD info changed"
#define RRDM_1372_112_2_17_22_52_22_3366 "Q_offset =0,resel to TDD immediate"
#define RRDM_1389_112_2_17_22_52_22_3367 "NCELL %d RSSI = %d\n,TDCELL RSCP = %d\n,penny = %d\n ,qoffset = %d\n,offset = %d "
#define RRDM_1435_112_2_17_22_52_22_3368 "DM:non-dule mode"
#define RRDM_1507_112_2_17_22_52_22_3369 "index = %d,arfcn = %d,cell_id = %d,rscp = %d"
#define RRDM_1603_112_2_17_22_52_22_3370 "@@@@@@ DM , SEND GSM2TD IND TO TD"
#define RRDM_1739_112_2_17_22_52_22_3371 "Invalid rat change req mode"
#define RRDM_1843_112_2_17_22_52_23_3372 "@@@@@@ start to waiting for td layer 1 download gsm dsp"
#define RRDM_1845_112_2_17_22_52_23_3373 "@@@@@@  td layer 1 download gsm dsp complete "
#define RRDM_1962_112_2_17_22_52_23_3374 "Invalid rat change req mode during CCOFU procedure"
#define RRDM_2172_112_2_17_22_52_23_3375 "Is eplmn"
#define RRDM_2212_112_2_17_22_52_23_3376 "tdd_meas_ptr->cell_num after clear = %d"
#define RRDM_2249_112_2_17_22_52_23_3377 "SI2QUATER %d segment not received!"
#define RRDM_2257_112_2_17_22_52_23_3378 "SI2QUATER all segment received!"
#define RRDM_2322_112_2_17_22_52_24_3379 "Q_offset =0,resel to TDD immediate"
#define RRDM_2350_112_2_17_22_52_24_3380 "cancel candidate_times TDD CELL %d "
#define RRDM_2351_112_2_17_22_52_24_3381 "rssi = %d,rscp = %d ,offset = %d"
#define RRDM_2431_112_2_17_22_52_24_3382 "DM resel:TDD cell %d"
#define RRDM_2654_112_2_17_22_52_24_3383 "&&&&&& GUOYAN:PMO is received!"
#define RRDM_2691_112_2_17_22_52_24_3384 "PMO :ENH Measurement Parameters  included"
#define RRDM_2717_112_2_17_22_52_24_3385 "PMO :3G Neighbour Cell Description  included"
#define RRDM_2825_112_2_17_22_52_25_3386 "PMO :GPRS 3G MEASUREMENT Parameters  included"
#define RRDM_2866_112_2_17_22_52_25_3387 "&&&&&& GUOYAN:PMO is received!"
#define RRDM_2895_112_2_17_22_52_25_3388 "%%%%%%GUOYAN: PCCO NC_MODE = %d"
#define RRDM_2908_112_2_17_22_52_25_3389 "PMO :EXT Measurement Parameters  included"
#define RRDM_2982_112_2_17_22_52_25_3390 "    temp_ba_ind = %d,\n    temp_3g_ba_ind = %d,\n    temp_mp_change_mark = %d,\n    temp_si2quater_index = %d,\n   temp_si2quater_count = %d"
#define RRDM_2987_112_2_17_22_52_25_3391 "instance %d hans been received,return"
#define RRDM_3048_112_2_17_22_52_25_3392 "SIQuater:Measurement_Parameters Description is included!\n"
#define RRDM_3057_112_2_17_22_52_25_3393 "SIQuater:GPRS RTD description is not supported by this version!\n"
#define RRDM_3063_112_2_17_22_52_25_3394 "SIQuater:GPRS_BSIC Descriptionis included!\n"
#define RRDM_3071_112_2_17_22_52_25_3395 "SIQuater:GPRS_REPORT PRIORITY Description is included!\n"
#define RRDM_3087_112_2_17_22_52_25_3396 "SIQuater:GPRS_MEASUREMENT_Parameters Description is included!\n"
#define RRDM_3098_112_2_17_22_52_25_3397 "SIQuater :NC Measurement Parameters included"
#define RRDM_3105_112_2_17_22_52_25_3398 "SIQuater :extension length included"
#define RRDM_3123_112_2_17_22_52_25_3399 "SIQuater :3G Neighbour Cell Description  included"
#define RRDM_3139_112_2_17_22_52_25_3400 "SIQuater :3G Measurement Parameters Description  included"
#define RRDM_3144_112_2_17_22_52_25_3401 "SIQuater :GPRS_3G_MEASUREMENT Parameters Description  included"
#define RRDM_3174_112_2_17_22_52_25_3402 "Index_Start_3G included"
#define RRDM_3184_112_2_17_22_52_25_3403 "Index_Start_3G included"
#define RRDM_3195_112_2_17_22_52_25_3404 "FDD neighbor cell is assigned  in SI2Quater!\n"
#define RRDM_3202_112_2_17_22_52_25_3405 "UTRAN TDD Description included"
#define RRDM_3244_112_2_17_22_52_25_3406 "Bandwidth_TDD included"
#define RRDM_3248_112_2_17_22_52_25_3407 "@@@@@384 Mcps band is received,which is not supported in this version!\n"
#define RRDM_3256_112_2_17_22_52_25_3408 "@@@@@384 Mcps band is received,which is not supported in this version!\n"
#define RRDM_3261_112_2_17_22_52_25_3409 "Repeated UMTS TDD Neighbour Cells included"
#define RRDM_3285_112_2_17_22_52_25_3410 "L= %d,tdd_arfcn_valid_ind = %d"
#define RRDM_3301_112_2_17_22_52_25_3411 "L= %d,arfcn = %d"
#define RRDM_3303_112_2_17_22_52_25_3412 "L= %d,nr_of_tdd_cells = %d"
#define RRDM_3425_112_2_17_22_52_26_3413 "Bandwidth_FDD included"
#define RRDM_3430_112_2_17_22_52_26_3414 "Repeated UMTS FDD Neighbour Cells included"
#define RRDM_3466_112_2_17_22_52_26_3415 "3GMeasurementParamentsDescription:search_i = %d"
#define RRDM_3480_112_2_17_22_52_26_3416 "3GMeasurementParamentsDescription:fdd measurement information is included,need skip all bits in now version"
#define RRDM_3494_112_2_17_22_52_26_3417 "3GMeasurementParamentsDescription:tdd_qoffset = %d\n,tdd_multirat_reporting = %d"
#define RRDM_3515_112_2_17_22_52_26_3418 "GPRS3GMeasurementParamentsDescription:search_p = %d"
#define RRDM_3517_112_2_17_22_52_26_3419 "GPRS3GMeasurementParamentsDescription:earch_prio_3g = %d"
#define RRDM_3533_112_2_17_22_52_26_3420 "GPRS3GMeasurementParamentsDescription:dd_multirat_reporting = %d"
#define RRDM_3538_112_2_17_22_52_26_3421 "GPRS3GMeasurementParamentsDescription:dd_reporting_offset = %d"
#define RRDM_3540_112_2_17_22_52_26_3422 "GPRS3GMeasurementParamentsDescription:dd_reporting_threshold = %d"
#define RRDM_3762_112_2_17_22_52_26_3423 "SIQuater:GPRS_MEASUREMENT_Parameters Description is included!\n"
#define RRDM_3895_112_2_17_22_52_27_3424 "SIQuater:skip following bits!\n"
#define RRDM_3899_112_2_17_22_52_27_3425 "SIQuater:RTD6 exist!\n"
#define RRDM_3928_112_2_17_22_52_27_3426 "SIQuater:RTD12 exist!\n"
#define RRDM_3976_112_2_17_22_52_27_3427 "Extension Information LENGTH =  %d"
#define RRDM_3978_112_2_17_22_52_27_3428 "RR_DecodeGprsCellOption:left length = %d"
#define RRDM_3981_112_2_17_22_52_27_3429 "EGPRS Information is included  in GPRS_CELL_OPTION IE"
#define RRDM_3996_112_2_17_22_52_27_3430 "CCN_ACTIVE is %d  in GPRS_CELL_OPTION IE"
#define RRDM_4029_112_2_17_22_52_27_3431 "3G neighbour cell desc not present"
#define RRDM_4081_112_2_17_22_52_27_3432 "@@@@@@ dm_td_cells_info , cell_num = %d"
#define RRDM_4139_112_2_17_22_52_27_3433 "%%%%%%GUOYAN :tdd measurement report is included"
#define RRDM_4273_112_2_17_22_52_27_3434 "@@@@@@ CALC TD RANK , arfcn=%d , cpi=%d ,rscp=%d , offset=%d , rank =%d"
#define RRDM_4353_112_2_17_22_52_28_3435 "R[%d] = %d,R[%d]= %d"
#define RRDM_4358_112_2_17_22_52_28_3436 "R[%d] = %d,R[%d]= %d"
#define RRDM_4363_112_2_17_22_52_28_3437 "i=%d,rn= %d"
#define RRDM_4417_112_2_17_22_52_28_3438 "PCCN:Downlink_tfi present: %d"
#define RRDM_4424_112_2_17_22_52_28_3439 "PCCN:Uplink_tfi present: %d"
#define RRDM_4433_112_2_17_22_52_28_3440 "V44060_5j0"
#define RRDM_4517_112_2_17_22_52_28_3441 "V44060_5j0"
#define RRDM_4659_112_2_17_22_52_28_3442 "&&&&&&&modify for dongguan:\n SI2QUATER instance %d not present,need wait it!"
#define RRDM_4665_112_2_17_22_52_28_3443 "all sengment is received!"
#define RRDM_4696_112_2_17_22_52_28_3444 "this arfcn no cell! ,discard this arfcn"
#define RRDM_4709_112_2_17_22_52_28_3445 "RRMSG.C:SCELL ARFCN = %d"
#define RRDM_4710_112_2_17_22_52_28_3446 "nr_repeated_utran_tdd_neighbour_cell = %d"
#define RRDM_4711_112_2_17_22_52_28_3447 "cell_ptr->neighbour_cell_desc_3g.neighbour_cell_desc_3g_present = %d"
#define RRMSG_1845_112_2_17_22_52_32_3448 "RR:si 1, pcs ind is TRUE "
#define RRMSG_2215_112_2_17_22_52_33_3449 "RRC: si2q av"
#define RRMSG_2223_112_2_17_22_52_33_3450 "RRC: si2q p = %d"
#define RRMSG_2230_112_2_17_22_52_33_3451 "RRC: A = %d, ra_colour = %d\n "
#define RRMSG_2340_112_2_17_22_52_33_3452 "RRC: pcs ind %d"
#define RRMSG_2343_112_2_17_22_52_33_3453 "RRC: SI6 rest len wrong"
#define RRMSG_2376_112_2_17_22_52_33_3454 "RRC: network assign half rate channel!"
#define RRMSG_3326_112_2_17_22_52_35_3455 "RRC: SGSNR %d"
#define RRMSG_3364_112_2_17_22_52_35_3456 "RRC: si2bis is not present, ba_0 is %d"
#define RRMSG_3375_112_2_17_22_52_35_3457 "RRC: ba_0 is %d"
#define RRMSG_3473_112_2_17_22_52_36_3458 "RRC: cell bar"
#define RRMSG_3556_112_2_17_22_52_36_3459 "RRC: cell bar"
#define RRMSG_4085_112_2_17_22_52_37_3460 "RRC: egprs packet uplink extend_RA %d"
#define RRMSG_4189_112_2_17_22_52_37_3461 "RRC: gprs packet uplink extend_RA %d"
#define RRMSG_4231_112_2_17_22_52_37_3462 "RRC:Timing Advance is out of range!"
#define RRMSG_4667_112_2_17_22_52_38_3463 "RRC: ba_5_ind and si5bis not present"
#define RRMSG_4683_112_2_17_22_52_38_3464 "RRC: ba_2_ind and si2bis not present"
#define RRMSG_4801_112_2_17_22_52_38_3465 "RRC: use bcch ba when traffic."
#define RRMSG_4934_112_2_17_22_52_39_3466 "RRC: unknown msg type %d"
#define RRMSG_5084_112_2_17_22_52_39_3467 "RRC: immed_assign->request_ref->random_ab %x"
#define RRMSG_5114_112_2_17_22_52_39_3468 "RRC: hopping false."
#define RRMSG_5374_112_2_17_22_52_39_3469 "RRC: recv imm reject"
#define RRMSG_5408_112_2_17_22_52_40_3470 "CASE207:get si2bis ba"
#define RRMSG_5414_112_2_17_22_52_40_3471 "CASE207:invalid si2bis"
#define RRMSG_DECODE_DEDICATED_298_112_2_17_22_52_41_3472 "RR:MR config icmi %d start_mode %d acs 0x%x"
#define RRMSG_DECODE_DEDICATED_441_112_2_17_22_52_42_3473 "RR: MULTI SLOT ERROR!"
#define RRMSG_DECODE_DEDICATED_954_112_2_17_22_52_43_3474 "RRC: chan_rel ba_pref len wrong."
#define RRMSG_DECODE_DEDICATED_1168_112_2_17_22_52_43_3475 "RRC: VGCS???"
#define RRMSG_DECODE_DEDICATED_1179_112_2_17_22_52_43_3476 "RR:Channel modify msg error"
#define RRMSG_DECODE_DEDICATED_1579_112_2_17_22_52_44_3477 "RRC: VGCS???"
#define RRMSG_DECODE_DEDICATED_1589_112_2_17_22_52_44_3478 "RR:Channel assign msg error"
#define RRMSG_DECODE_DEDICATED_1609_112_2_17_22_52_44_3479 "RRC: assert error, assign_cmd start cipher but no cipher cmd recv before"
#define RRMSG_DECODE_DEDICATED_2144_112_2_17_22_52_45_3480 "RRC: VGCS???"
#define RRMSG_DECODE_DEDICATED_2154_112_2_17_22_52_45_3481 "RR:Channel assign msg error"
#define RRMSG_DECODE_DEDICATED_2174_112_2_17_22_52_45_3482 "RRC: assert error, handover start cipher but no cipher cmd recv before"
#define RRMSG_DECODE_DEDICATED_2365_112_2_17_22_52_46_3483 "RRC:received Extended Mearsurement Order!"
#define RRMSG_GPRS_134_112_2_17_22_52_48_3484 "RRC: 2 quater, index %d, count %d"
#define RRMSG_GPRS_258_112_2_17_22_52_49_3485 "RRC: si2quater, nc_order, non_drx %d, i %d, t %d"
#define RRMSG_GPRS_278_112_2_17_22_52_49_3486 "RRC:received System Information 9, not supported now!"
#define RRMSG_GPRS_296_112_2_17_22_52_49_3487 "RRC:received System Information 16, not supported now!"
#define RRMSG_GPRS_314_112_2_17_22_52_49_3488 "RRC:received System Information 17, not supported now!"
#define RRMSG_GPRS_332_112_2_17_22_52_49_3489 "RRC:received System Information 18, not supported now!"
#define RRMSG_GPRS_350_112_2_17_22_52_49_3490 "RRC:received System Information 19, not supported now!"
#define RRMSG_GPRS_368_112_2_17_22_52_49_3491 "RRC:received System Information 20, not supported now!"
#define RRPROC_115_112_2_17_22_52_58_3492 "RR_PAGING_TIMER: accur = %d"
#define RRPROC_131_112_2_17_22_52_58_3493 "RR_PAGING_TIMER: SI13[%d] duration:%d accurate:%d"
#define RRPROC_150_112_2_17_22_52_58_3494 "RR_PAGING_TIMER: SI13[%d] still active"
#define RRPROC_395_112_2_17_22_52_59_3495 "RRPROC:Default case!"
#define RRPROC_605_112_2_17_22_52_59_3496 "RRC: cell [%d] cell_present %d bad %d g_ind %d rssi %d"
#define RRPROC_630_112_2_17_22_52_59_3497 "RRC: find? %d, ncell %d"
#define RRPROC_777_112_2_17_22_53_0_3498 "RRC:RR NCELL: cell clear all"
#define RRPROC_1386_112_2_17_22_53_1_3499 "RRC: use band %d"
#define RRPROC_1411_112_2_17_22_53_1_3500 "RRC: Assert error, invalid arfcn %d"
#define RRPROC_1550_112_2_17_22_53_1_3501 "RRC: Gprs meas freq should not over MAX_MEAS_NUM_GRPS"
#define RRPROC_1616_112_2_17_22_53_1_3502 "RRC: traffic ba is invalid, not clear ncell"
#define RRPROC_1631_112_2_17_22_53_1_3503 "RRC: clear cell %d, arfcn %d"
#define RRPROC_1670_112_2_17_22_53_2_3504 "RRC:RRC_ForceCamping for sim %d"
#define RRPROC_1673_112_2_17_22_53_2_3505 "RRC: = %d"
#define RRPROC_1692_112_2_17_22_53_2_3506 "RRC: RRC_CancelForceCamping \n"
#define RRPROC_2113_112_2_17_22_53_2_3507 "RRC: RECV si BAD"
#define RRPROC_2160_112_2_17_22_53_3_3508 "RRC: FTA RRMSG_GetPacketMsgType is TRUE"
#define RRPROC_2163_112_2_17_22_53_3_3509 "RRC: FTA msg type is paging"
#define RRPROC_2167_112_2_17_22_53_3_3510 "RRC: FTA paging msg decode is ok"
#define RRPROC_2170_112_2_17_22_53_3_3511 "RRC: FTA paging address is ok, save it"
#define RRPROC_2259_112_2_17_22_53_3_3512 "%s"
#define RRPROC_2267_112_2_17_22_53_3_3513 "%s"
#define RRPROC_2306_112_2_17_22_53_3_3514 "RRC: %d findRcell, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d"
#define RRPROC_2311_112_2_17_22_53_3_3515 "RRC: tc 0x%x, tc 0x%x, tc 0x%x, tc 0x%x, tc 0x%x, tc 0x%x"
#define RRPROC_2379_112_2_17_22_53_3_3516 "RRC: FAB CAMP"
#define RRPROC_2477_112_2_17_22_53_3_3517 "RRC: Assert failure, selected plmn list length %d is more than MAX %d."
#define RRPROC_2489_112_2_17_22_53_3_3518 "RRC %d: Selected plmn mcc:%d mnc:%d mnc_digit_num %d with no sim."
#define RRPROC_2494_112_2_17_22_53_3_3519 "RRC: Current lai not in selected plmn list with no sim, select next cell."
#define RRPROC_2615_112_2_17_22_53_3_3520 "RR_PAGING_TIMER: RR_RegisterPagingTimer timer type: %d, time out: %d"
#define RRPROC_2628_112_2_17_22_53_3_3521 "RR_PAGING_TIMER:  is_si13_callback_reg %d"
#define RRPROC_2634_112_2_17_22_53_3_3522 "RR_PAGING_TIMER:  active timer[%d]"
#define RRPROC_2663_112_2_17_22_53_4_3523 "RR_PAGING_TIMER: RR_UnRegisterPagingTimer timer type: %d"
#define RRPROC_2670_112_2_17_22_53_4_3524 "RR_PAGING_TIMER:  deactive timer[%d]"
#define RRPROC_2691_112_2_17_22_53_4_3525 "RR_PAGING_TIMER: is_si13_callback_reg : %d"
#define RRPROC_2713_112_2_17_22_53_4_3526 "RR_PAGING_TIMER: RR_IsPagingTimerActive timer type: %d"
#define RRPROC_2722_112_2_17_22_53_4_3527 "RR_PAGING_TIMER:  timer[%d] status : %d"
#define RRPROC_GPRS_113_112_2_17_22_53_4_3528 "RRC: current card num = %d\n"
#define RRPROC_GPRS_114_112_2_17_22_53_4_3529 "RRC: paging_tbf_mask = %x\n"
#define RRPROC_GPRS_136_112_2_17_22_53_5_3530 "RRC:bss_pag_coord = %d nmo = %d\n"
#define RRPROC_GPRS_142_112_2_17_22_53_5_3531 "RRC: nv_allowed = %d  net_allowed = %d\n"
#define RRPROC_GPRS_319_112_2_17_22_53_5_3532 "RRC: transfer nc_reporting_period_i = %d\n"
#define RRPROC_GPRS_350_112_2_17_22_53_5_3533 "RRC: transfer nc_reporting_period_t = %d\n"
#define RRPROC_GPRS_597_112_2_17_22_53_6_3534 "RR: Assert Failure, unknown page mode."
#define RR_ADAPTER_891_112_2_17_22_53_46_3535 "RRA: enter RRA_Init"
#define RR_ADAPTER_1128_112_2_17_22_53_47_3536 "RRA: s_rra_gprs_id 0x%x is invalid, sig_code = 0x%x."
#define RR_ADAPTER_1164_112_2_17_22_53_47_3537 "RRA: s_rra_gprs_id 0x%x is invalid, sig_code = 0x%x."
#define RR_ADAPTER_1331_112_2_17_22_53_47_3538 "RRA: assert error, unprocessed signal 0x%x"
#define RR_ADAPTER_1371_112_2_17_22_53_47_3539 "RRA: rrc already send act req."
#define RR_ADAPTER_1381_112_2_17_22_53_47_3540 "RRA: rr %d, rr_task_id = 0x%x"
#define RR_ADAPTER_1432_112_2_17_22_53_47_3541 "RRA: powersweep req %d state0 = %d, state1 = %d, state2 = %d, state3 = %d"
#define RR_ADAPTER_1438_112_2_17_22_53_47_3542 "RRA: powersweep req %d state0 = %d, state1 = %d, state2 = %d"
#define RR_ADAPTER_1443_112_2_17_22_53_47_3543 "RRA: powersweep req %d state0 = %d, state1 = %d"
#define RR_ADAPTER_1459_112_2_17_22_53_48_3544 "RRA: band change from %d to %d, must do power sweep"
#define RR_ADAPTER_1473_112_2_17_22_53_48_3545 "RRA: discard powersweep req %d, wait layer1 powersweep cnf."
#define RR_ADAPTER_1518_112_2_17_22_53_48_3546 "RRA: dummy powersweep cnf state0 = %d, state1 = %d, state2 = %d, state3 = %d, last traffic id = %d"
#define RR_ADAPTER_1524_112_2_17_22_53_48_3547 "RRA: dummy powersweep cnf state0 = %d, state1 = %d, state2 = %d, last traffic id = %d"
#define RR_ADAPTER_1529_112_2_17_22_53_48_3548 "RRA: dummy powersweep cnf state0 = %d, state1 = %d, last traffic id = %d"
#define RR_ADAPTER_1544_112_2_17_22_53_48_3549 "RRA: return dummy cnf."
#define RR_ADAPTER_1564_112_2_17_22_53_48_3550 "RRA: rr %d in find plmn flag = %d, state = %d"
#define RR_ADAPTER_1588_112_2_17_22_53_48_3551 "RRA: rr %d enter RRA_RR_SEARCH."
#define RR_ADAPTER_1603_112_2_17_22_53_48_3552 "powersweepreq:master id %d"
#define RR_ADAPTER_1639_112_2_17_22_53_48_3553 "RRA: po task id = %d, %d, %d, d, find plmn flag = %d, %d, %d, %d , state = %d, %d, %d, %d, master = %d"
#define RR_ADAPTER_1652_112_2_17_22_53_48_3554 "RRA: po task id = %d, %d, %d, find plmn flag = %d, %d, %d , state = %d, %d, %d, master = %d"
#define RR_ADAPTER_1662_112_2_17_22_53_48_3555 "RRA: po task id = %d, %d, find plmn flag = %d, %d, state = %d, %d,, master = %d"
#define RR_ADAPTER_1685_112_2_17_22_53_48_3556 "RRA: layer1 return dummy powersweep cnf."
#define RR_ADAPTER_1926_112_2_17_22_53_48_3557 "RRA: recv pbcch decode req, req_rr = %d"
#define RR_ADAPTER_2013_112_2_17_22_53_49_3558 "RRA: recv pbcch info ind, only send to rr %d"
#define RR_ADAPTER_2074_112_2_17_22_53_49_3559 "RRA: can't do bcch camp, because there is pbcch camp"
#define RR_ADAPTER_2121_112_2_17_22_53_49_3560 "RRA: clear mac rlc flag %d"
#define RR_ADAPTER_2132_112_2_17_22_53_49_3561 "RRA: camp param of is same as before, discard it, req_rr = %d"
#define RR_ADAPTER_2151_112_2_17_22_53_49_3562 "RRA: camp arfcn = 0x%x, req_rr = %d"
#define RR_ADAPTER_2265_112_2_17_22_53_49_3563 "RRA: req_rr %d dismap %d"
#define RR_ADAPTER_2275_112_2_17_22_53_49_3564 "RRA: rrc already send deact req."
#define RR_ADAPTER_2286_112_2_17_22_53_49_3565 "RRA: exist active rr, rr %d donot send deactive to layer1."
#define RR_ADAPTER_2302_112_2_17_22_53_49_3566 "RRA: deactive req_rr = %d, s_rra_master_id = %d"
#define RR_ADAPTER_2345_112_2_17_22_53_49_3567 "RRA: send main campon to layer1, master rr = %d"
#define RR_ADAPTER_2360_112_2_17_22_53_49_3568 "RRA: layer1 enter deactive."
#define RR_ADAPTER_2374_112_2_17_22_53_49_3569 "RRA: rr %d enter RRA_RR_NULL, master id = %d"
#define RR_ADAPTER_2481_112_2_17_22_53_50_3570 "RRA: dis card rrc %d page mode %d."
#define RR_ADAPTER_2503_112_2_17_22_53_50_3571 "RRA: send MPH_BCCH_CAMP_REQ, rr = %d"
#define RR_ADAPTER_2632_112_2_17_22_53_50_3572 "RRA: master %d, p5 %d"
#define RR_ADAPTER_2635_112_2_17_22_53_50_3573 "RRA: mt override lau"
#define RR_ADAPTER_2661_112_2_17_22_53_50_3574 "RRA: cs override ps"
#define RR_ADAPTER_2693_112_2_17_22_53_50_3575 "RRA: current s_rra_override_mask = %x."
#define RR_ADAPTER_2694_112_2_17_22_53_50_3576 "RRA: rr %d is override card no need delay in rf scan protect."
#define RR_ADAPTER_2700_112_2_17_22_53_50_3577 "RRA: rr %d rach delay 1s while in rf scan protect."
#define RR_ADAPTER_2765_112_2_17_22_53_50_3578 "RRA: send 4 MPH_BCCH_CAMP_REQ, rr = %d"
#define RR_ADAPTER_2781_112_2_17_22_53_50_3579 "RRA: is answer paging %d"
#define RR_ADAPTER_2792_112_2_17_22_53_50_3580 "RRA: xxrr %d enter RRA_RR_TRAFFIC."
#define RR_ADAPTER_2868_112_2_17_22_53_50_3581 "RRA: rr %d enter RRA_RR_IDLE."
#define RR_ADAPTER_3051_112_2_17_22_53_51_3582 "RRA: use quadsim dsp code, card num = %d."
#define RR_ADAPTER_3055_112_2_17_22_53_51_3583 "RRA: use trisim dsp code, card num = %d."
#define RR_ADAPTER_3063_112_2_17_22_53_51_3584 "RRA: rr %d map to layer1 %d"
#define RR_ADAPTER_3064_112_2_17_22_53_51_3585 "RRA: layer1 %d map to rr %d"
#define RR_ADAPTER_3098_112_2_17_22_53_51_3586 "RRA: do not send %d meas report while in rf scan protect."
#define RR_ADAPTER_3252_112_2_17_22_53_51_3587 "RRA: ba combinate exceed, discard some info."
#define RR_ADAPTER_3490_112_2_17_22_53_52_3588 "RRA: act gprs = %d"
#define RR_ADAPTER_3613_112_2_17_22_53_52_3589 "RRA: DeliverToRRCSig rr id = %d"
#define RR_ADAPTER_3757_112_2_17_22_53_52_3590 "RRA: receive PBCCH CAMP REQ but can't do pbcch camp, because another exist"
#define RR_ADAPTER_3772_112_2_17_22_53_52_3591 "RRA: enter PBCCH CAMP"
#define RR_ADAPTER_3782_112_2_17_22_53_52_3592 "RRA: clear mac rlc flag %d"
#define RR_ADAPTER_3788_112_2_17_22_53_52_3593 "RRA: pbcch camp arfcn = 0x%x, req_rr = %d"
#define RR_ADAPTER_3899_112_2_17_22_53_53_3594 "RRA: handoverReq req_rr %d, arfcn_before_handover %d, camp_arfcn %d"
#define RR_ADAPTER_3982_112_2_17_22_53_53_3595 "RRA: rach delay exp, master id = %d"
#define RR_ADAPTER_4036_112_2_17_22_53_53_3596 "RRA: rr to mac, s_rra_gprs_id = 0x%x"
#define RR_ADAPTER_4127_112_2_17_22_53_53_3597 "RRA: discard immed assign!"
#define RR_ADAPTER_4134_112_2_17_22_53_53_3598 "RRA: discard MAC_IMMED_ASSIGN. rr = %d"
#define RR_ADAPTER_4157_112_2_17_22_53_53_3599 "RRA: discard mac rach rsp. rr = %d, s_rra_gprs_id = %d"
#define RR_ADAPTER_4206_112_2_17_22_53_54_3600 "RRA: rr to rlc, s_rra_gprs_id = 0x%x"
#define RR_ADAPTER_4254_112_2_17_22_53_54_3601 "RRA: deact,need_trans %d, gprs_id %d, gprs_suspend_by_other_sim %d,need_gprs_resume_by_gprs_sim %d"
#define RR_ADAPTER_4275_112_2_17_22_53_54_3602 "RRA: handle deact req, req_rr = %d, s_rra_master_id = %d, s_rra_gprs_id = %d"
#define RR_ADAPTER_4280_112_2_17_22_53_54_3603 "RRA: rr %d state = %d"
#define RR_ADAPTER_4286_112_2_17_22_53_54_3604 "RRA: suspend,need_trans %d, gprs_id %d"
#define RR_ADAPTER_4302_112_2_17_22_53_54_3605 "RRA: resume,need_trans %d, gprs_id %d, gprs_suspend_by_other_sim"
#define RR_ADAPTER_4333_112_2_17_22_53_54_3606 "RRA: invalid s_rra_gprs_id."
#define RR_ADAPTER_4345_112_2_17_22_53_54_3607 "RRA: use rr %d param to act rlc."
#define RR_ADAPTER_4374_112_2_17_22_53_54_3608 "RRA: SetGprsParaReq,req_rr = %d, gprs id = %d"
#define RR_ADAPTER_4395_112_2_17_22_53_54_3609 "RRA: rlc act flag %d"
#define RR_ADAPTER_4505_112_2_17_22_53_54_3610 "RRA: latest traffic id %d"
#define RR_ADAPTER_4568_112_2_17_22_53_54_3611 "RRA: rf scan ind: rr %d in state %d"
#define RR_ADAPTER_4639_112_2_17_22_53_54_3612 "RRA: copy rf scan num = %d"
#define RR_ADAPTER_4672_112_2_17_22_53_55_3613 "RRA: layer1 rach expired, master id = %d"
#define RR_ADAPTER_4742_112_2_17_22_53_55_3614 "RRA: discard smscb signal 0x%x"
#define RR_ADAPTER_4857_112_2_17_22_53_55_3615 "RRA: start rf scan protect timer."
#define RR_ADAPTER_4880_112_2_17_22_53_55_3616 "RRA: active rrc %d map to layer1 %d"
#define RR_ADAPTER_5028_112_2_17_22_53_55_3617 "RRA: mac freq %d, type = %d"
#define RR_ADAPTER_5052_112_2_17_22_53_55_3618 "RRA: unknow mac freq type %d"
#define RR_ADAPTER_5324_112_2_17_22_53_56_3619 "RRA: bcch camp req_rr = %d, before camp, s_rra_master_id = %d"
#define RR_ADAPTER_5632_112_2_17_22_53_56_3620 "RRA: recv unexpected tc = 0."
#define RR_ADAPTER_5707_112_2_17_22_53_57_3621 "RRA: bcch list decode arfcn = 0x%x, band = %d, tc in = 0x%x, out = 0x%x"
#define RR_ADAPTER_6082_112_2_17_22_53_57_3622 "RRA: unknow signal 0x%x,free."
#define RR_ADAPTER_6137_112_2_17_22_53_57_3623 "RRA: invalid arfcn index, BAND %d, arfcn %d"
#define SCT_PROC_87_112_2_17_22_53_58_3624 "SCT: Close tch loop type I"
#define SCT_PROC_92_112_2_17_22_53_58_3625 "SCT: cmd error C %d, B%d, A%d"
#define SCT_PROC_102_112_2_17_22_53_58_3626 "SCT: Close tch loop type F"
#define SCT_PROC_107_112_2_17_22_53_58_3627 "SCT loop type E"
#define SCT_PROC_113_112_2_17_22_53_58_3628 "SCT: Close tch loop type D"
#define SCT_PROC_120_112_2_17_22_53_58_3629 "SCT: Close tch loop type C"
#define SCT_PROC_127_112_2_17_22_53_58_3630 "SCT: Close tch loop type B"
#define SCT_PROC_132_112_2_17_22_53_58_3631 "SCT: Close tch loop type A"
#define SCT_PROC_141_112_2_17_22_53_58_3632 "SCT: Open loop command"
#define SCT_PROC_147_112_2_17_22_53_58_3633 "SCT: Test interfaec %d "
#define SCT_PROC_205_112_2_17_22_53_58_3634 "SCT: GPRS loop command"
#define SCT_PROC_216_112_2_17_22_53_58_3635 "SCT:EGPRS SRB loopback command"
#define SCT_PROC_250_112_2_17_22_53_58_3636 "SCT: Close tch loop ack"
#define SCT_PROC_263_112_2_17_22_53_58_3637 "SCT: Open tch loop ack"
#define DLR_258_112_2_17_22_54_7_3638 "DL: release req 0,but no entity"
#define DLR_281_112_2_17_22_54_7_3639 "DL: release req 3,but no entity"
#define DLR_424_112_2_17_22_54_7_3640 "L2: unexpected sapi\n"
#define DLR_740_112_2_17_22_54_8_3641 "PH_READY_TO_SEND_IND recved, but task released\n"
#define LAYER1_527_112_2_17_22_54_12_3642 "deactive req.\n"
#define LAYER1_592_112_2_17_22_54_13_3643 "rxpower_sweep_req asked and confirm replied.\n"
#define LAYER1_610_112_2_17_22_54_13_3644 "rxpower from BS come.\n"
#define LAYER1_628_112_2_17_22_54_13_3645 "bcch req asked.\n"
#define LAYER1_639_112_2_17_22_54_13_3646 "the bcch req is single %d.\n"
#define LAYER1_656_112_2_17_22_54_13_3647 "bcch req stop.\n"
#define LAYER1_672_112_2_17_22_54_13_3648 "bsic req asked.\n"
#define LAYER1_748_112_2_17_22_54_13_3649 "\n*****Layer1 Rx BCCH:*****\n"
#define LAYER1_749_112_2_17_22_54_13_3650 "BCCH_ARFCN:       %d \n\n"
#define LAYER1_790_112_2_17_22_54_13_3651 "\n*****L1Sim Rx BCCH:*****\n"
#define LAYER1_791_112_2_17_22_54_13_3652 "BCCH_ARFCN:%d \n\n"
#define LAYER1_834_112_2_17_22_54_13_3653 "bcch ind true\n"
#define LAYER1_995_112_2_17_22_54_13_3654 "campon req&confirmed.\n"
#define LAYER1_1070_112_2_17_22_54_14_3655 "TBF Downlink Assignment Recv\n"
#define LAYER1_1143_112_2_17_22_54_14_3656 "Send the first RA\n"
#define LAYER1_1190_112_2_17_22_54_14_3657 "AGCH recv\n"
#define LAYER1_1322_112_2_17_22_54_14_3658 "PLMN search asked and  rxpower confirm replied.\n"
#define LAYER1_1666_112_2_17_22_54_15_3659 "chan_assign:%d\n"
#define LAYER1_1708_112_2_17_22_54_15_3660 "chan_assign_fail:%d\n"
#define LAYER1_1791_112_2_17_22_54_15_3661 "handover_fail:%d\n"
#define LAYER1_1905_112_2_17_22_54_15_3662 "Received Data block from new DCCH in non-syn handover\n"
#define LAYER1_1914_112_2_17_22_54_15_3663 "handover :%d\n"
#define LAYER1_1979_112_2_17_22_54_15_3664 "handover_fail:%d\n"
#define LAYER1_2015_112_2_17_22_54_15_3665 "L1:sacch data recv\n"
#define LAYER1_2033_112_2_17_22_54_15_3666 "l1 recv data:%d\n"
#define LAYER1_2244_112_2_17_22_54_16_3667 "This bcch block is unexpected\n"
#define LAYER1_2282_112_2_17_22_54_16_3668 "bcch ind,true\n"
#define LAYER1_2289_112_2_17_22_54_16_3669 "bcch ind,not requested\n"
#define LAYER1_2402_112_2_17_22_54_16_3670 "SI_ID: 1"
#define LAYER1_2406_112_2_17_22_54_16_3671 "SI_ID: 2"
#define LAYER1_2410_112_2_17_22_54_16_3672 "SI_ID: 2Bis"
#define LAYER1_2414_112_2_17_22_54_16_3673 "SI_ID: 2Ter"
#define LAYER1_2418_112_2_17_22_54_16_3674 "SI_ID: 3"
#define LAYER1_2422_112_2_17_22_54_16_3675 "SI_ID: 4"
#define LAYER1_2426_112_2_17_22_54_16_3676 "SI_ID: 13"
#define LAYER1_2430_112_2_17_22_54_16_3677 "SI_ID: 5BIS"
#define LAYER1_2434_112_2_17_22_54_16_3678 "SI_ID: 5BIS"
#define LAYER1_2438_112_2_17_22_54_16_3679 "SI_ID: 6"
#define LAYER1_2442_112_2_17_22_54_16_3680 "SI_ID: 7"
#define LAYER1_2446_112_2_17_22_54_16_3681 "PSI_ID: 13"
#define LAYER1_2450_112_2_17_22_54_16_3682 "SI_ID: 4"
#define LAYER1_2454_112_2_17_22_54_16_3683 "PSI_ID: 3Bis"
#define LAYER1_2458_112_2_17_22_54_16_3684 "SI_ID: 8"
#define LAYER1_2462_112_2_17_22_54_16_3685 "SI_ID: 9"
#define LAYER1_2466_112_2_17_22_54_16_3686 "SI_ID: 13"
#define LAYER1_2470_112_2_17_22_54_16_3687 "PSI_ID: 1"
#define LAYER1_2474_112_2_17_22_54_16_3688 "PSI_ID: 2"
#define LAYER1_2478_112_2_17_22_54_16_3689 "PSI_ID: 3 "
#define MAC_469_112_2_17_22_54_17_3690 "MAC:try to start t3192 0 ms"
#define MAC_924_112_2_17_22_54_18_3691 "MAC: Decode grr freq req error!\n"
#define MAC_961_112_2_17_22_54_18_3692 "MAC: DL imm assi,freq Acquare Sys Info error"
#define MAC_1113_112_2_17_22_54_19_3693 "MAC: EGPRS imm DL assi,but MS don't support EGPRS! \n"
#define MAC_1138_112_2_17_22_54_19_3694 "MAC: DL imm assi,Decode ia rest octets error!"
#define MAC_1145_112_2_17_22_54_19_3695 "MAC: DL imm assi,Decode chan descri error!"
#define MAC_1319_112_2_17_22_54_19_3696 "MAC: No tfi assign,ignore the packet DL assi from ppch!"
#define MAC_1402_112_2_17_22_54_19_3697 "MAC: Decode freq para error! Acquare Sys Info"
#define MAC_1427_112_2_17_22_54_19_3698 "MAC: Multislot capability not match! Ignore pdch dl req!"
#define MAC_1434_112_2_17_22_54_19_3699 "MAC: Decode packet DL error in idle, ignore the msg!"
#define MAC_1616_112_2_17_22_54_20_3700 "MAC: Decode rach response error! Acquare Sys Info"
#define MAC_1638_112_2_17_22_54_20_3701 "MAC: One phase received egprs ul assi£¬but we don't support EDGE!"
#define MAC_1650_112_2_17_22_54_20_3702 "MAC: One phase received Dl single assi!\n"
#define MAC_1963_112_2_17_22_54_20_3703 "MAC: Single block without TBF!"
#define MAC_2105_112_2_17_22_54_21_3704 "MAC: One phase received Dl assi!"
#define MAC_2112_112_2_17_22_54_21_3705 "MAC: One phase received egprs dl assi£¬but we don't support EDGE!"
#define MAC_2120_112_2_17_22_54_21_3706 "MAC: Decode rach response error! Rach retry!"
#define MAC_2161_112_2_17_22_54_21_3707 "MAC: wrong condition receive rach rsp"
#define MAC_2212_112_2_17_22_54_21_3708 "MAC: Rach response reject, ul tbf failure!\n"
#define MAC_2229_112_2_17_22_54_21_3709 "MAC: Rach response reject, Rach retry!"
#define MAC_2373_112_2_17_22_54_21_3710 "MAC: not support GRR_MAC_PDCH_ASSIGN_REQ"
#define MAC_2501_112_2_17_22_54_21_3711 "MAC: access abort invalid"
#define MAC_2551_112_2_17_22_54_21_3712 "MAC: access abort invalid"
#define MAC_2777_112_2_17_22_54_22_3713 "MAC: tbf mode invalid in connect cnf %d"
#define MAC_2842_112_2_17_22_54_22_3714 "MAC: Ul release, send ctrl ack request new TBF!"
#define MAC_2853_112_2_17_22_54_22_3715 "MAC: Ul release, send res req, request new TBF!"
#define MAC_2858_112_2_17_22_54_22_3716 "MAC: Ul release, unexpected ul_exp_action!!"
#define MAC_2943_112_2_17_22_54_22_3717 "MAC: receive ul_rel_req but ul not active"
#define MAC_3035_112_2_17_22_54_22_3718 "MAC:tbf mode invalid in rlc_mac_rel_req"
#define MAC_3043_112_2_17_22_54_22_3719 "MAC:Assert Failure,tbf mode invalid in rlc_mac_rel_req"
#define MAC_3147_112_2_17_22_54_23_3720 "MAC:Assert Failure,release both link tbf but not in rel state"
#define MAC_3173_112_2_17_22_54_23_3721 "MAC:tbf mode invalid,uplink tbf starting time?"
#define MAC_3297_112_2_17_22_54_23_3722 "MAC:rlc_mac_tbf_req in ul_active!!"
#define MAC_3306_112_2_17_22_54_23_3723 "MAC:ignore rlc tbf req when prach have been send but not receive ass!"
#define MAC_3443_112_2_17_22_54_23_3724 "MAC:rlc realloc req but ul not active"
#define MAC_3549_112_2_17_22_54_23_3725 "MAC: wrong state 3186 expire"
#define MAC_3560_112_2_17_22_54_23_3726 "MAC: T3170 expiry! T3170 Dur: %d \n"
#define MAC_3657_112_2_17_22_54_24_3727 "MAC: wrong state t3170 expire"
#define MAC_3723_112_2_17_22_54_24_3728 "MAC: T3168 expiry! T3168 Dur: %d\n"
#define MAC_3857_112_2_17_22_54_24_3729 "MAC: wrong state rev t3168 expire"
#define MAC_3974_112_2_17_22_54_24_3730 "MAC: wrong state rev t3164 expire"
#define MAC_4052_112_2_17_22_54_24_3731 "MAC: wrong state rev t3162 expire"
#define MAC_4069_112_2_17_22_54_24_3732 "MAC: wrong state rev t3180 expire"
#define MAC_4091_112_2_17_22_54_25_3733 "MAC: T3192 expiry! T3192 Dur: %d\n"
#define MAC_4127_112_2_17_22_54_25_3734 "MAC: wrong dl state, t3192 exp\n"
#define MAC_4263_112_2_17_22_54_25_3735 "MAC: Prach access not permitted!"
#define MAC_4378_112_2_17_22_54_25_3736 "MAC: Decode rach response error! False TBF Mode or MCS\n"
#define MAC_4641_112_2_17_22_54_26_3737 "MAC: Mac mode mismatch! Ignore packet ul assi!"
#define MAC_4666_112_2_17_22_54_26_3738 "MAC: rev ul assi in ul idle or other invalid state"
#define MAC_4744_112_2_17_22_54_26_3739 "MAC: one_phase_access_error!!"
#define MAC_4864_112_2_17_22_54_26_3740 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_4870_112_2_17_22_54_26_3741 "MAC: Frequency not in band! Rach retry!\n"
#define MAC_4876_112_2_17_22_54_26_3742 "MAC: decode freq error return invalid value"
#define MAC_4920_112_2_17_22_54_26_3743 "MAC: Frequency not in band! Rach retry!\n"
#define MAC_4932_112_2_17_22_54_26_3744 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_4995_112_2_17_22_54_26_3745 "MAC: decode freq error return invalid value"
#define MAC_5011_112_2_17_22_54_26_3746 "MAC: DL est when waiting L1 release cnf"
#define MAC_5034_112_2_17_22_54_26_3747 "MAC:Ignore packet dl assi when two phase or contention phase!"
#define MAC_5044_112_2_17_22_54_26_3748 "MAC: rev dl assi when ul access two phase or other state"
#define MAC_5271_112_2_17_22_54_27_3749 "MAC: No tfi assign in dl assi! Rach retry!\n"
#define MAC_5333_112_2_17_22_54_27_3750 "MAC: wrong state when rev dl assi"
#define MAC_5342_112_2_17_22_54_27_3751 "MAC: Decode freq error! Rach retry!"
#define MAC_5354_112_2_17_22_54_27_3752 "MAC: Multislot capability not match! Rach retry!"
#define MAC_5377_112_2_17_22_54_27_3753 "MAC: Multislot capability not match! Ignore the msg!"
#define MAC_5384_112_2_17_22_54_27_3754 "MAC: Rlc mode mismatch! Ignore dl ass msg in dl_active!"
#define MAC_5444_112_2_17_22_54_27_3755 "MAC:contention phase, ignore TS RECONFIG"
#define MAC_5462_112_2_17_22_54_27_3756 "MAC: error state to receive ts reconfig"
#define MAC_5496_112_2_17_22_54_27_3757 "MAC: in ul active recv pkt ts reconfig allocation no dl slot! Rach retry!\n"
#define MAC_5556_112_2_17_22_54_27_3758 "MAC: Decode error! Acquare Sys Info\n"
#define MAC_5561_112_2_17_22_54_27_3759 "MAC: Decode freq error! UL release and Rach retry!\n"
#define MAC_5581_112_2_17_22_54_28_3760 "MAC: Decode allocation error! UL release and Rach retry!\n"
#define MAC_5719_112_2_17_22_54_28_3761 "MAC: Timeslot reconfig not include dl tfi! Ul release and Rach retry!\n"
#define MAC_5746_112_2_17_22_54_28_3762 "MAC: in dl active recv pkt ts reconfig allocation no dl slot! Rach retry!\n"
#define MAC_5841_112_2_17_22_54_28_3763 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_5846_112_2_17_22_54_28_3764 "MAC: wrong state when rev ts realloc assi"
#define MAC_5852_112_2_17_22_54_28_3765 "MAC: Decode freq error! Rach retry!\n"
#define MAC_5864_112_2_17_22_54_28_3766 "MAC: Timeslot reconfig not include dl tfi! Rach retry!\n"
#define MAC_5874_112_2_17_22_54_28_3767 "MAC: Rlc mode mismatch! Ignore ts reconfig msg in dl_active!"
#define MAC_5954_112_2_17_22_54_28_3768 "MAC: in ul dl active recv pkt ts reconfig allocation no dl slot! Rach retry!\n"
#define MAC_6003_112_2_17_22_54_28_3769 "MAC: Decode error! Acquare Sys Info\n"
#define MAC_6008_112_2_17_22_54_28_3770 "MAC: Decode freq error! UL release and Rach retry!\n"
#define MAC_6185_112_2_17_22_54_29_3771 "MAC: Decode allocation error! Rach retry!"
#define MAC_6198_112_2_17_22_54_29_3772 "MAC: Rlc mode mismatch! Ignore ts reconfig msg in ul_dl_active!"
#define MAC_6213_112_2_17_22_54_29_3773 "MAC: Ignore additional PACKET ACCESS REJECT messages!"
#define MAC_6515_112_2_17_22_54_29_3774 "MAC: Receive reject, UL release!\n"
#define MAC_6569_112_2_17_22_54_29_3775 "MAC: ignore PACKET_QUEUING_NOTIFICATION while waiting TBF Starting Time of ul ass!"
#define MAC_6780_112_2_17_22_54_30_3776 "MAC: wrong ul state recv Packet tbf realse msg!"
#define MAC_6912_112_2_17_22_54_30_3777 "MAC: wrong PPR"
#define MAC_7312_112_2_17_22_54_31_3778 "MAC: wrong state rev t3166 expire"
#define MAC_7634_112_2_17_22_54_32_3779 "MAC: Abnormal release with retry!"
#define MAC_7658_112_2_17_22_54_32_3780 "MAC: !!Impossible ul_release_state!!"
#define MAC_7698_112_2_17_22_54_32_3781 "MAC: Abnormal release with rach retry!"
#define MAC_7710_112_2_17_22_54_32_3782 "MAC: release_cnf for ul but ul not active,maybe hold for connect cnf"
#define MAC_7779_112_2_17_22_54_32_3783 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_7791_112_2_17_22_54_32_3784 "MAC: Frequency not in band! Rach retry!\n"
#define MAC_7797_112_2_17_22_54_32_3785 "MAC: decode freq error,invalid return value"
#define MAC_7925_112_2_17_22_54_32_3786 "MAC: Single block without TBF!\n"
#define MAC_8063_112_2_17_22_54_32_3787 "MAC: invalid one phase state when rev single block assi"
#define MAC_8200_112_2_17_22_54_33_3788 "MAC: invalid one phase state when rev ul assi"
#define MAC_8274_112_2_17_22_54_33_3789 "MAC: Decode allocation error! Rach retry!\n"
#define MAC_8281_112_2_17_22_54_33_3790 "MAC: Ingore the ul assignment msg"
#define MAC_8293_112_2_17_22_54_33_3791 "MAC: wrong state when rev single block assi"
#define MAC_8453_112_2_17_22_54_33_3792 "MAC: Decode allocation error! Rach retry!"
#define MAC_8460_112_2_17_22_54_33_3793 "MAC: Ingore the ul assi S"
#define MAC_8579_112_2_17_22_54_33_3794 "MAC: Decode allocation error! Rach retry!\n"
#define MAC_8586_112_2_17_22_54_33_3795 "MAC: Ingore the ul assi msg!"
#define MAC_8596_112_2_17_22_54_33_3796 "MAC: rev single block assi when ul active"
#define MAC_8609_112_2_17_22_54_33_3797 "MAC: Decode freq error! Acquare Sys Info"
#define MAC_8680_112_2_17_22_54_34_3798 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_8685_112_2_17_22_54_34_3799 "MAC: Decode freq error! Ignore the msg!"
#define MAC_8823_112_2_17_22_54_34_3800 "MAC: Dl assi not include tfi!"
#define MAC_8829_112_2_17_22_54_34_3801 "MAC: Multislot capability not match! Ignore the msg!\n"
#define MAC_8949_112_2_17_22_54_34_3802 "MAC: Decode freq error! Acquare Sys Info\n"
#define MAC_9034_112_2_17_22_54_34_3803 "MAC: Ignore dl ass while T3192 run!ctrl_ack=%d"
#define MAC_9152_112_2_17_22_54_35_3804 "MAC: Decode error! acquare si!\n"
#define MAC_9204_112_2_17_22_54_35_3805 "MAC: Decode allocation error! Rach retry!"
#define MAC_9439_112_2_17_22_54_35_3806 "MAC: wrong state rev ul ack"
#define MAC_9556_112_2_17_22_54_35_3807 "MAC: Contention failure!"
#define RLC_703_112_2_17_22_54_37_3808 "rlc:remain llc tbf request!"
#define RLC_720_112_2_17_22_54_38_3809 "rlc:cell reselect tbf!"
#define RLC_939_112_2_17_22_54_38_3810 "rlc:Up Single Block Access!\n"
#define RLC_1017_112_2_17_22_54_38_3811 "RLC: SCT est fail, downlink tbf is not acitved!"
#define RLC_1816_112_2_17_22_54_40_3812 "RLC: recv data error in loop back mode"
#define RLC_1851_112_2_17_22_54_40_3813 "rlc:error data_ind when dl is not actived!"
#define RLC_1867_112_2_17_22_54_40_3814 "rlc: error, suspend or null state recv data block!"
#define RLC_1940_112_2_17_22_54_40_3815 "rlc:SCT end!\n"
#define RLC_2129_112_2_17_22_54_40_3816 "rlc:should not recv up release"
#define RLC_2355_112_2_17_22_54_41_3817 "rlc:Access Fail with CS Page!\n"
#define RLC_2795_112_2_17_22_54_42_3818 "rlc: suspend state recv tbf ind!\n"
#define RLC_2801_112_2_17_22_54_42_3819 "rlc:Up tbf fail with dl tbf is esting!\n"
#define RLC_2805_112_2_17_22_54_42_3820 "RLC: est sigle block without tbf interrupt by dl tbf ass"
#define RLC_2848_112_2_17_22_54_42_3821 "rlc:Up tbf ok for single block!\n"
#define RLC_2875_112_2_17_22_54_42_3822 "rlc:IS Rejected Access!\n"
#define RLC_3037_112_2_17_22_54_42_3823 "rlc:mac retry,but no need!\n"
#define RLC_3071_112_2_17_22_54_42_3824 "rlc:Up tbf fail with t3172 in mac ul_active!\n"
#define RLC_3149_112_2_17_22_54_43_3825 "rlc:error normal release"
#define RLC_3338_112_2_17_22_54_43_3826 "rlc:Up fail and perform cell reselect!\n"
#define RLC_3406_112_2_17_22_54_43_3827 "rlc:Up tbf fail with t3172 in mac ul_active!\n"
#define RLC_3487_112_2_17_22_54_43_3828 "rlc:Up tbf fail with timer t3172!\n"
#define RLC_3553_112_2_17_22_54_44_3829 "rlc:Receive Second mac_rlc_tbf_cnf msg!"
#define RLC_3568_112_2_17_22_54_44_3830 "rlc:Acess state is not one or two phase!"
#define RLC_3623_112_2_17_22_54_44_3831 "RLC:SCT Test Mode B Ested!"
#define RLC_3630_112_2_17_22_54_44_3832 "RLC:SCT Test Mode A Ested!"
#define RRC_3758_112_2_17_22_54_52_3833 "RRC: unknown bsic cnf"
#define RRC_3825_112_2_17_22_54_53_3834 "RRC: ASSERT, research>6"
#define RRC_3844_112_2_17_22_54_53_3835 "RRC: unknown bsic fail"
#define RRC_4575_112_2_17_22_54_54_3836 "RRC: Plmn search is interrupted, while candidate_cell_exist"
#define RRC_6803_112_2_17_22_54_59_3837 "RRC: discard IA,because mm est saved"
#define RRC_6811_112_2_17_22_54_59_3838 "RRC: discard IA,because plmn list req"
#define RRC_8042_112_2_17_22_55_1_3839 "RRC: proc new bsic of scell."
#define RRC_8252_112_2_17_22_55_2_3840 "RRC: proc new bsic of scell."
#define RRC_9604_112_2_17_22_55_4_3841 "RR:receive_psi_resel Get Packet paging!!\n"
#define RRC_10009_112_2_17_22_55_5_3842 "RRC: assert error, not fastplmn"
#define RRC_10148_112_2_17_22_55_6_3843 "RRC: assert error, not fastplmn"
#define RRC_10626_112_2_17_22_55_7_3844 "RRC: unknown bsic cnf"
#define RRC_10705_112_2_17_22_55_7_3845 "RRC: unknown bsic fail"
#define RRC_11250_112_2_17_22_55_8_3846 "RR: discard immed_assig"
#define RRC_11283_112_2_17_22_55_8_3847 "RR: rx immediate rej"
#define RRC_12172_112_2_17_22_55_10_3848 "RRC wrong sapi %d"
#define RRC_12953_112_2_17_22_55_11_3849 "RRC: wait assign dcch est error in dl chan type"
#define RRC_13687_112_2_17_22_55_13_3850 "RRC wrong sapi %d"
#define RRC_16762_112_2_17_22_55_19_3851 "RRC: Receive data block on PPCH, discard it!"
#define RRC_17504_112_2_17_22_55_20_3852 "RRC: Decode Pmo err!"
#define RRC_18268_112_2_17_22_55_22_3853 "RRC: ASSERT ERROR recv prach when bad"
#define RRC_21388_112_2_17_22_55_28_3854 "RRC: discard IA,because plmn list req"
#define RRC_21605_112_2_17_22_55_29_3855 "RRC: Receive data block on PPCH, discard it!"
#define RRC_21767_112_2_17_22_55_29_3856 "RRC: Decode pmo err!"
#define RRC_21845_112_2_17_22_55_29_3857 "RRC: find ncell fail"
#define RRC_21857_112_2_17_22_55_29_3858 "RRC: gmm state not ready"
#define RRC_21865_112_2_17_22_55_29_3859 "RRC: decode pcco fail"
#define RRC_23559_112_2_17_22_55_32_3860 "RRC: process saved ppch"
#define RRC_23648_112_2_17_22_55_33_3861 "RRC: proc saved pch "
#define RRC_24338_112_2_17_22_55_34_3862 "RRC: resel scell arfcn 0x%x"
#define RRC_24570_112_2_17_22_55_34_3863 "RRC: cell resel arfcn 0x%x"
#define RRC_24822_112_2_17_22_55_35_3864 " RRC: switch to another cell, clear PSI Msg Buffer \n"
#define RRC_25261_112_2_17_22_55_36_3865 "RRC: unknown bsic cnf"
#define RRC_26078_112_2_17_22_55_37_3866 "RRC: temp = %d"
#define RRC_27704_112_2_17_22_55_41_3867 "RRC: Decode Pmo err!"
#define RRC_27774_112_2_17_22_55_41_3868 "RRC: Receive data block on PPCH, discard it!"
#define DLR_258_112_2_17_22_55_55_3869 "DL: release req 0,but no entity"
#define DLR_281_112_2_17_22_55_55_3870 "DL: release req 3,but no entity"
#define DLR_424_112_2_17_22_55_55_3871 "L2: unexpected sapi\n"
#define DLR_740_112_2_17_22_55_56_3872 "PH_READY_TO_SEND_IND recved, but task released\n"
#define LAYER1_527_112_2_17_22_55_59_3873 "deactive req.\n"
#define LAYER1_592_112_2_17_22_56_0_3874 "rxpower_sweep_req asked and confirm replied.\n"
#define LAYER1_610_112_2_17_22_56_0_3875 "rxpower from BS come.\n"
#define LAYER1_628_112_2_17_22_56_0_3876 "bcch req asked.\n"
#define LAYER1_639_112_2_17_22_56_0_3877 "the bcch req is single %d.\n"
#define LAYER1_656_112_2_17_22_56_0_3878 "bcch req stop.\n"
#define LAYER1_672_112_2_17_22_56_0_3879 "bsic req asked.\n"
#define LAYER1_748_112_2_17_22_56_0_3880 "\n*****Layer1 Rx BCCH:*****\n"
#define LAYER1_749_112_2_17_22_56_0_3881 "BCCH_ARFCN:       %d \n\n"
#define LAYER1_790_112_2_17_22_56_0_3882 "\n*****L1Sim Rx BCCH:*****\n"
#define LAYER1_791_112_2_17_22_56_0_3883 "BCCH_ARFCN:%d \n\n"
#define LAYER1_834_112_2_17_22_56_0_3884 "bcch ind true\n"
#define LAYER1_995_112_2_17_22_56_0_3885 "campon req&confirmed.\n"
#define LAYER1_1070_112_2_17_22_56_1_3886 "TBF Downlink Assignment Recv\n"
#define LAYER1_1143_112_2_17_22_56_1_3887 "Send the first RA\n"
#define LAYER1_1190_112_2_17_22_56_1_3888 "AGCH recv\n"
#define LAYER1_1322_112_2_17_22_56_1_3889 "PLMN search asked and  rxpower confirm replied.\n"
#define LAYER1_1486_112_2_17_22_56_1_3890 "chan_assign:%d\n"
#define LAYER1_1528_112_2_17_22_56_1_3891 "chan_assign_fail:%d\n"
#define LAYER1_1611_112_2_17_22_56_2_3892 "handover_fail:%d\n"
#define LAYER1_1725_112_2_17_22_56_2_3893 "Received Data block from new DCCH in non-syn handover\n"
#define LAYER1_1734_112_2_17_22_56_2_3894 "handover :%d\n"
#define LAYER1_1799_112_2_17_22_56_2_3895 "handover_fail:%d\n"
#define LAYER1_1835_112_2_17_22_56_2_3896 "L1:sacch data recv\n"
#define LAYER1_1853_112_2_17_22_56_2_3897 "l1 recv data:%d\n"
#define LAYER1_2064_112_2_17_22_56_2_3898 "This bcch block is unexpected\n"
#define LAYER1_2102_112_2_17_22_56_3_3899 "bcch ind,true\n"
#define LAYER1_2109_112_2_17_22_56_3_3900 "bcch ind,not requested\n"
#define LAYER1_2222_112_2_17_22_56_3_3901 "SI_ID: 1"
#define LAYER1_2226_112_2_17_22_56_3_3902 "SI_ID: 2"
#define LAYER1_2230_112_2_17_22_56_3_3903 "SI_ID: 2Bis"
#define LAYER1_2234_112_2_17_22_56_3_3904 "SI_ID: 2Ter"
#define LAYER1_2238_112_2_17_22_56_3_3905 "SI_ID: 3"
#define LAYER1_2242_112_2_17_22_56_3_3906 "SI_ID: 4"
#define LAYER1_2246_112_2_17_22_56_3_3907 "SI_ID: 13"
#define LAYER1_2250_112_2_17_22_56_3_3908 "SI_ID: 5BIS"
#define LAYER1_2254_112_2_17_22_56_3_3909 "SI_ID: 5BIS"
#define LAYER1_2258_112_2_17_22_56_3_3910 "SI_ID: 6"
#define LAYER1_2262_112_2_17_22_56_3_3911 "SI_ID: 7"
#define LAYER1_2266_112_2_17_22_56_3_3912 "PSI_ID: 13"
#define LAYER1_2270_112_2_17_22_56_3_3913 "SI_ID: 4"
#define LAYER1_2274_112_2_17_22_56_3_3914 "PSI_ID: 3Bis"
#define LAYER1_2278_112_2_17_22_56_3_3915 "SI_ID: 8"
#define LAYER1_2282_112_2_17_22_56_3_3916 "SI_ID: 9"
#define LAYER1_2286_112_2_17_22_56_3_3917 "SI_ID: 13"
#define LAYER1_2290_112_2_17_22_56_3_3918 "PSI_ID: 1"
#define LAYER1_2294_112_2_17_22_56_3_3919 "PSI_ID: 2"
#define LAYER1_2298_112_2_17_22_56_3_3920 "PSI_ID: 3 "
#define RRC_2489_112_2_17_22_56_8_3921 "RRC: unknown bsic cnf"
#define RRC_2553_112_2_17_22_56_8_3922 "RRC: ASSERT, research>6"
#define RRC_2572_112_2_17_22_56_8_3923 "RRC: unknown bsic fail"
#define RRC_3262_112_2_17_22_56_10_3924 "RRC: Plmn search is interrupted, while candidate_cell_exist"
#define RRC_4834_112_2_17_22_56_13_3925 "RRC: proc new bsic of scell."
#define RRC_5383_112_2_17_22_56_14_3926 "RRC: assert error, not fastplmn"
#define RRC_5513_112_2_17_22_56_14_3927 "RRC: assert error, not fastplmn"
#define RRC_5967_112_2_17_22_56_15_3928 "RRC: unknown bsic cnf"
#define RRC_6043_112_2_17_22_56_16_3929 "RRC: unknown bsic fail"
#define RRC_6434_112_2_17_22_56_16_3930 "RR: discard immed_assig"
#define RRC_6467_112_2_17_22_56_16_3931 "RR: rx immediate rej"
#define RRC_7180_112_2_17_22_56_18_3932 "RRC wrong sapi %d"
#define RRC_7934_112_2_17_22_56_19_3933 "RRC: wait assign dcch est error in dl chan type"
#define RRC_8615_112_2_17_22_56_21_3934 "RRC wrong sapi %d"
#define RRC_11787_112_2_17_22_56_27_3935 "RRC: proc saved pch "
#define RRC_12325_112_2_17_22_56_28_3936 "RRC: resel scell arfcn 0x%x"
#define RRC_12444_112_2_17_22_56_28_3937 "RRC: cell resel arfcn 0x%x"
#define RRC_12823_112_2_17_22_56_29_3938 "RRC: unknown bsic cnf"
#define RRC_13614_112_2_17_22_56_31_3939 "RR temp = %d"
#define SM_COMMON_241_112_2_17_22_56_57_3940 "SMCOMMON:delete the activation pdp context request(nsapi %d)"
#define SM_COMMON_271_112_2_17_22_56_57_3941 "SMCOMMON: handle nsapi %d"
#define SM_DECODE_94_112_2_17_22_56_57_3942 "SM:decode Transaction identifier, TIE reserved!"
#define SM_DECODE_121_112_2_17_22_56_58_3943 "SM:decode Transaction identifier, EXT is '0'!"
#define SM_DECODE_249_112_2_17_22_56_58_3944 "SM:PDP address length too long!"
#define SM_DECODE_272_112_2_17_22_56_58_3945 "SM:decode pdp address,PPP,PDP type reserved!"
#define SM_DECODE_300_112_2_17_22_56_58_3946 "SM:decode pdp address,PDP org type reserved!"
#define SM_DECODE_382_112_2_17_22_56_58_3947 "SM: LLC SAPI Spare Field non-0!"
#define SM_DECODE_390_112_2_17_22_56_58_3948 "SM: LLC SAPI not assigned!"
#define SM_DECODE_402_112_2_17_22_56_58_3949 "SM: LLC SAPI value reserved,invalid!"
#define SM_DECODE_546_112_2_17_22_56_58_3950 "SM: is_r99 = %d"
#define SM_DECODE_577_112_2_17_22_56_59_3951 "SM:decode Qos Field invalid!"
#define SM_DECODE_677_112_2_17_22_56_59_3952 "SM:decode Qos Field invalid!"
#define SM_DECODE_684_112_2_17_22_56_59_3953 "SM:ignore extented qos fields!"
#define SM_DECODE_726_112_2_17_22_56_59_3954 "SM: Radio Priority Spare Field non-0!"
#define SM_DECODE_731_112_2_17_22_56_59_3955 "SM: Radio Priority reserved,level 4!"
#define SM_DECODE_772_112_2_17_22_56_59_3956 "SM:Receive APN Length > APN buffer!"
#define SM_DECODE_1669_112_2_17_22_57_1_3957 "SM:PDP context accept Qos < MS Min_Qos Setting!!"
#define SM_DECODE_1674_112_2_17_22_57_1_3958 "SM:PDP context accept Qos > MS Min_Qos Setting!!\n"
#define SM_DECODE_1705_112_2_17_22_57_1_3959 "APN:invalid char appear!"
#define SM_ENCODE_280_112_2_17_22_57_2_3960 "SM:SM_EncodePdpaddr pdp_addr_length too long!"
#define SM_ENCODE_312_112_2_17_22_57_2_3961 "SM:SM_EncodePdpaddr pdp_addr_length too long!"
#define SM_ENCODE_327_112_2_17_22_57_2_3962 "SM:Assert Failure,SM_EncodePdpaddr"
#define SM_ENCODE_360_112_2_17_22_57_2_3963 "SM:SM_EncodeApn apn too long!"
#define SM_ENTITY_444_112_2_17_22_57_4_3964 "SM: Invalid state!"
#define SM_ENTITY_696_112_2_17_22_57_5_3965 "SMentity:sapi not match."
#define SM_ENTITY_712_112_2_17_22_57_5_3966 "SMentity:sapi not match."
#define SM_ENTITY_749_112_2_17_22_57_5_3967 "SMentity:sapi not match."
#define SM_ENTITY_773_112_2_17_22_57_5_3968 "SMentity:sapi not match."
#define SM_ENTITY_779_112_2_17_22_57_5_3969 "SM: Discard SNSM_STATUS_REQ in state %d!"
#define SM_ENTITY_867_112_2_17_22_57_5_3970 "SM: Invalid state!"
#define SM_ENTITY_955_112_2_17_22_57_5_3971 "SM: Invalid state!"
#define SM_ENTITY_1667_112_2_17_22_57_7_3972 "SM: ignore the message!"
#define SM_MAIN_918_112_2_17_22_57_9_3973 "SM: discard SNSM_ACTIVATE_RES in INACTIVATE state"
#define SM_MAIN_945_112_2_17_22_57_9_3974 "SM: discard SNSM_DEACTIVATE_RES in INACTIVATE state"
#define SM_MAIN_972_112_2_17_22_57_9_3975 "SM: discard SNSM_MODIFY_RES in INACTIVATE state"
#define SM_MAIN_998_112_2_17_22_57_9_3976 "SM: discard SNSM_STATUS_REQ in INACTIVATE state"
#define SM_MAIN_1420_112_2_17_22_57_10_3977 "SM_TASK: save signal %d"
#define SM_MAIN_1734_112_2_17_22_57_11_3978 "SM_TASK: discard signal:%d"
#define SM_TIMER_72_112_2_17_22_57_18_3979 "SM_StartTimer timer_type = %d"
#define SM_TIMER_108_112_2_17_22_57_18_3980 "SM_StopTimer timer_type = %d"
#define SMC_COMMON_232_112_2_17_22_57_19_3981 "SMC: msg_id is 0x%x"
#define SMC_COMMON_260_112_2_17_22_57_19_3982 "SMC: SMC_GetGsmCause, unknown msg_id: 0x%x"
#define SMC_COMMON_398_112_2_17_22_57_19_3983 "SMC: CP-msg, AnalyzeResult:%d,pCause:%d"
#define SMC_COMMON_506_112_2_17_22_57_19_3984 "SMC: send CM_MM_REL_REQ to MM, TI:%d"
#define SMC_COMMON_575_112_2_17_22_57_19_3985 "SMC: send CM_MM_DATA_REQ for CP-DATA, TI: %d, length: %d!"
#define SMC_COMMON_621_112_2_17_22_57_19_3986 "SMC: send CM_MM_DATA_REQ for CP-ACK, TI: %d, length: %d!"
#define SMC_COMMON_672_112_2_17_22_57_20_3987 "SMC: send CM_MM_DATA_REQ for CP-ERROR, TI: %d, length: %d"
#define SMC_COMMON_1033_112_2_17_22_57_20_3988 "SMC:cause moduletype:%d,mm cause:%d"
#define SMC_CS_CTRL_49_112_2_17_22_57_21_3989 "SMC: CS-SMC_MO_IDLE, recv Msg: %s"
#define SMC_CS_CTRL_97_112_2_17_22_57_21_3990 "SMC: CS received %s and discarded under state SMC_MO_IDLE !"
#define SMC_CS_CTRL_103_112_2_17_22_57_21_3991 "SMC: CS received unknown 0x%x under state SMC_MO_IDLE !"
#define SMC_CS_CTRL_122_112_2_17_22_57_21_3992 "SMC: CS-SMC_MO_MM_CONNECTION_PENDING, recv Msg: %s"
#define SMC_CS_CTRL_171_112_2_17_22_57_21_3993 "SMC: CS received %s and discarded under state SMC_MO_MM_CONNECTION_PENDING !"
#define SMC_CS_CTRL_177_112_2_17_22_57_21_3994 "SMC: CS received unknown 0x%x under state SMC_MO_MM_CONNECTION_PENDING !"
#define SMC_CS_CTRL_196_112_2_17_22_57_21_3995 "SMC: CS-SMC_MO_WAIT_FOR_CP_ACK, recv Msg: %s"
#define SMC_CS_CTRL_263_112_2_17_22_57_21_3996 "SMC: CS received %s and discarded under state SMC_MO_WAIT_FOR_CP_ACK !"
#define SMC_CS_CTRL_269_112_2_17_22_57_21_3997 "SMC: CS received unknown 0x%x under state SMC_MO_WAIT_FOR_CP_ACK !"
#define SMC_CS_CTRL_288_112_2_17_22_57_21_3998 "SMC: CS-SMC_MO_MM_CONNECTION_ESTABLISHED, recv Msg: %s"
#define SMC_CS_CTRL_430_112_2_17_22_57_22_3999 "SMC: CS received %s and discarded under state SMC_MO_MM_CONNECTION_ESTABLISHED !"
#define SMC_CS_CTRL_436_112_2_17_22_57_22_4000 "SMC: CS received unknown 0x%x under state SMC_MO_MM_CONNECTION_ESTABLISHED !"
#define SMC_CS_CTRL_474_112_2_17_22_57_22_4001 "SMC: unknown SMC CS MO state 0x%x"
#define SMC_CS_CTRL_492_112_2_17_22_57_22_4002 "SMC: CS-SMC_MT_IDLE, recv Msg: %s"
#define SMC_CS_CTRL_520_112_2_17_22_57_22_4003 "SMC: CS received %s and discarded under state SMC_MT_IDLE !"
#define SMC_CS_CTRL_526_112_2_17_22_57_22_4004 "SMC: CS received unknown 0x%x under state SMC_MT_IDLE !"
#define SMC_CS_CTRL_545_112_2_17_22_57_22_4005 "SMC: CS-SMC_MT_MM_CONNECTION_ESTABLISHED, recv Msg: %s"
#define SMC_CS_CTRL_601_112_2_17_22_57_22_4006 "SMC: CS received %s and discarded under state SMC_MT_MM_CONNECTION_ESTABLISHED !"
#define SMC_CS_CTRL_607_112_2_17_22_57_22_4007 "SMC: CS received unknown 0x%x under state SMC_MT_MM_CONNECTION_ESTABLISHED !"
#define SMC_CS_CTRL_626_112_2_17_22_57_22_4008 "SMC: CS-SMC_MT_WAIT_FOR_CP_ACK, recv Msg: %s"
#define SMC_CS_CTRL_717_112_2_17_22_57_22_4009 "SMC: CS received %s and discarded under state SMC_MT_WAIT_FOR_CP_ACK !"
#define SMC_CS_CTRL_723_112_2_17_22_57_22_4010 "SMC: CS received unknown 0x%x under state SMC_MT_WAIT_FOR_CP_ACK !"
#define SMC_CS_CTRL_757_112_2_17_22_57_22_4011 "SMC: unknown SMC CS MT state 0x%x"
#define SMC_CS_CTRL_797_112_2_17_22_57_22_4012 "SMC: unknown SMC CS message direction 0x%x"
#define SMC_CS_FUNC_92_112_2_17_22_57_23_4013 "SMC:SMC_CS_HandleMmEstablishCnf,cur_ti:%d,EstablishCnf.TI:%d"
#define SMC_CS_FUNC_179_112_2_17_22_57_23_4014 "SMC: receive MO_CP_MsgType: %d,cur_ti_nw:%d, recv TI: %d"
#define SMC_CS_FUNC_574_112_2_17_22_57_24_4015 "SMC: received MT_CP_MsgType: %d, cur_ti_nw: %d, TempTI: %d"
#define SMC_PS_CTRL_45_112_2_17_22_57_25_4016 "SMC: PS-SMC_MO_IDLE, recv Msg: %s"
#define SMC_PS_CTRL_102_112_2_17_22_57_25_4017 "SMC: PS received %s and discarded under state SMC_MO_IDLE !"
#define SMC_PS_CTRL_108_112_2_17_22_57_25_4018 "SMC: PS received unknown 0x%x under state SMC_MO_IDLE !"
#define SMC_PS_CTRL_124_112_2_17_22_57_25_4019 "SMC: PS-SMC_MO_GMM_CONNECTION_PENDING, recv Msg: %s"
#define SMC_PS_CTRL_162_112_2_17_22_57_25_4020 "SMC: PS received %s and discarded under state SMC_MO_GMM_CONNECTION_PENDING !"
#define SMC_PS_CTRL_168_112_2_17_22_57_25_4021 "SMC: PS received unknown 0x%x under state SMC_MO_GMM_CONNECTION_PENDING !"
#define SMC_PS_CTRL_187_112_2_17_22_57_26_4022 "SMC: PS-SMC_MO_WAIT_FOR_CP_ACK, recv Msg: %s"
#define SMC_PS_CTRL_258_112_2_17_22_57_26_4023 "SMC: PS received %s and discarded under state SMC_MO_WAIT_FOR_CP_ACK !"
#define SMC_PS_CTRL_264_112_2_17_22_57_26_4024 "SMC: PS received unknown 0x%x under state SMC_MO_WAIT_FOR_CP_ACK !"
#define SMC_PS_CTRL_283_112_2_17_22_57_26_4025 "SMC: PS-SMC_MO_WAIT_FOR_CP_DATA, recv Msg: %s"
#define SMC_PS_CTRL_379_112_2_17_22_57_26_4026 "SMC: PS received %s and discarded under state SMC_MO_WAIT_FOR_CP_DATA !"
#define SMC_PS_CTRL_385_112_2_17_22_57_26_4027 "SMC: PS received unknown 0x%x under state SMC_MO_WAIT_FOR_CP_DATA !"
#define SMC_PS_CTRL_423_112_2_17_22_57_26_4028 "SMC: unknown SMC PS MO state 0x%x"
#define SMC_PS_CTRL_441_112_2_17_22_57_26_4029 "SMC: PS-SMC_MT_IDLE, recv Msg: %s"
#define SMC_PS_CTRL_489_112_2_17_22_57_26_4030 "SMC: PS received %s and discarded under state SMC_MT_IDLE !"
#define SMC_PS_CTRL_495_112_2_17_22_57_26_4031 "SMC: PS received unknown 0x%x under state SMC_MT_IDLE !"
#define SMC_PS_CTRL_514_112_2_17_22_57_26_4032 "SMC: PS-SMC_MT_WAIT_FOR_RP_ACK, recv Msg: %s"
#define SMC_PS_CTRL_571_112_2_17_22_57_26_4033 "SMC: PS received %s and discarded under state SMC_MT_WAIT_FOR_RP_ACK !"
#define SMC_PS_CTRL_577_112_2_17_22_57_26_4034 "SMC: PS received unknown 0x%x under state SMC_MT_WAIT_FOR_RP_ACK !"
#define SMC_PS_CTRL_596_112_2_17_22_57_26_4035 "SMC: PS-SMC_MT_WAIT_FOR_CP_ACK, recv Msg: %s"
#define SMC_PS_CTRL_666_112_2_17_22_57_26_4036 "SMC: PS received %s and discarded under state SMC_MT_WAIT_FOR_CP_ACK !"
#define SMC_PS_CTRL_672_112_2_17_22_57_26_4037 "SMC: PS received unknown 0x%x under state SMC_MT_WAIT_FOR_CP_ACK !"
#define SMC_PS_CTRL_705_112_2_17_22_57_27_4038 "SMC: unknown SMC PS MT state 0x%x"
#define SMC_PS_CTRL_745_112_2_17_22_57_27_4039 "SMC: unknown SMC PS message direction 0x%x"
#define SMC_PS_FUNC_89_112_2_17_22_57_27_4040 "SMC:smc_data_ptr->ps_entity.is_ps_mm_conn=%d,don't need send MM_SMS_ESTABLISH_REQ!!"
#define SMC_PS_FUNC_103_112_2_17_22_57_27_4041 "SMC:smc_data_ptr->ps_entity.is_ps_mm_conn=%d,so smc need send MM_SMS_ESTABLISH_REQ!!"
#define SMC_PS_FUNC_839_112_2_17_22_57_29_4042 "SMC: SMC_PS_MO_HandleLlcUnitDataInd, cp_msg_type:%d, nw_cause:%d"
#define SMC_PS_FUNC_986_112_2_17_22_57_29_4043 "SMC: SMC_PS_MT_HandleLLCUnitDataInd, cp_msg_type:%d, nw_cause:%d"
#define SMR_MO_CONTROL_48_112_2_17_22_57_29_4044 "SMR: SMR_MO_STATUS_IDLE, received MsgName: %s"
#define SMR_MO_CONTROL_84_112_2_17_22_57_30_4045 "SMR: received %s and discarded under state SMR_MO_STATUS_IDLE !"
#define SMR_MO_CONTROL_90_112_2_17_22_57_30_4046 "SMR: received unknown 0x%x under state SMR_MO_STATUS_IDLE!"
#define SMR_MO_CONTROL_108_112_2_17_22_57_30_4047 "SMR: SMR_MO_STATUS_WAIT_FOR_RPACK, received MsgName: %s"
#define SMR_MO_CONTROL_116_112_2_17_22_57_30_4048 "SMR:SMR_MO_STATUS_WAIT_FOR_RPACK,smr_pid: %x, proc_type: %d"
#define SMR_MO_CONTROL_196_112_2_17_22_57_30_4049 "SMR: received %s and discarded under state SMR_MO_STATUS_WAIT_FOR_RPACK !"
#define SMR_MO_CONTROL_202_112_2_17_22_57_30_4050 "SMR: received unknown 0x%x under state SMR_MO_STATUS_WAIT_FOR_RPACK !"
#define SMR_MO_CONTROL_221_112_2_17_22_57_30_4051 "SMR: SMR_MO_STATUS_WAIT_FOR_RETRANTIMER, received MsgName: %s"
#define SMR_MO_CONTROL_267_112_2_17_22_57_30_4052 "SMR: received %s and discarded under state SMR_MO_STATUS_WAIT_FOR_RETRANTIMER !"
#define SMR_MO_CONTROL_273_112_2_17_22_57_30_4053 "SMR: received unknown message 0x%x under state SMR_MO_STATUS_WAIT_FOR_RETRANTIMER !"
#define SMR_MO_CONTROL_311_112_2_17_22_57_30_4054 "SMR: unknown SMR MO state 0x%x"
#define SMR_MO_FUNC_48_112_2_17_22_57_30_4055 "SMR: SMR_SMC_EST_REQ domain: %d; RAT : %d"
#define SMR_MO_FUNC_87_112_2_17_22_57_30_4056 "SMR: CheckDataInd failed, data_ind_ptr->data_len: %d is TOO short"
#define SMR_MO_FUNC_101_112_2_17_22_57_30_4057 "SMR: CheckDataInd failed, data_ind_ptr->rp_data_arr[1]: %d, smr_pid:%x, msg_ref: %d"
#define SMR_MO_FUNC_139_112_2_17_22_57_31_4058 "SMR: recv msg_ref:%d, rp_type:%d"
#define SMR_MO_FUNC_354_112_2_17_22_57_31_4059 "SMR: recv msg_ref:%d, rp_msg_type:%d"
#define SMR_MT_CONTROL_45_112_2_17_22_57_31_4060 "SMR: SMR_MT_STATUS_IDLE, received MsgName: %s"
#define SMR_MT_CONTROL_76_112_2_17_22_57_32_4061 "SMR: received %s and discarded under state SMR_MT_STATUS_IDLE !"
#define SMR_MT_CONTROL_82_112_2_17_22_57_32_4062 "SMR: received unknown 0x%x under state SMR_MT_STATUS_IDLE !"
#define SMR_MT_CONTROL_99_112_2_17_22_57_32_4063 "SMR: SMR_MT_STATUS_WAIT_TO_SEND_RPACK, received MsgName: %s"
#define SMR_MT_CONTROL_129_112_2_17_22_57_32_4064 "SMR: received %s and discarded under state SMR_MT_STATUS_WAIT_TO_SEND_RPACK !"
#define SMR_MT_CONTROL_135_112_2_17_22_57_32_4065 "SMR: received unknown 0x%x under state SMR_MT_STATUS_WAIT_TO_SEND_RPACK !"
#define SMR_MT_CONTROL_167_112_2_17_22_57_32_4066 "SMR: unknown SMR MT state 0x%x"
#define SMR_PUBLIC_185_112_2_17_22_57_33_4067 "SMR:element_len > SMR_RPDU_MAX_LENGTH - 1,element_len=%d,"
#define SMR_PUBLIC_196_112_2_17_22_57_33_4068 "SMR:total_len != data_len ,total_len=%d,data_len=%d"
#define SMS_COMMON_API_65_112_2_17_22_57_34_4069 "SMS: TimerStartCmd, timer_type(%d) not found"
#define SMS_COMMON_API_98_112_2_17_22_57_34_4070 "SMS: DeactiveTimer, timer_type(%d) not found"
#define SMS_RL_186_112_2_17_22_57_35_4071 "SMSRL_DecodeRpError: the cause is %d"
#define RFC2507C_105_112_2_17_22_57_38_4072 "\n"
#define RFC2507C_106_112_2_17_22_57_38_4073 "v6s_full_c \t %d \n"
#define RFC2507C_107_112_2_17_22_57_38_4074 "v6s_comp_c \t %d \n"
#define RFC2507C_108_112_2_17_22_57_38_4075 "v6s_unmod_c \t %d \n"
#define RFC2507C_109_112_2_17_22_57_38_4076 "v6s_bbefore_c \t %d \n"
#define RFC2507C_110_112_2_17_22_57_38_4077 "v6s_bafter_c \t %d \n"
#define RFC2507C_111_112_2_17_22_57_38_4078 "v6s_changes_c \t %d \n"
#define RFC2507C_112_112_2_17_22_57_39_4079 "v6s_unknown_c \t %d \n"
#define RFC2507C_113_112_2_17_22_57_39_4080 "v6s_full_d \t %d \n"
#define RFC2507C_114_112_2_17_22_57_39_4081 "v6s_comp_d \t %d \n"
#define RFC2507C_115_112_2_17_22_57_39_4082 "v6s_unmod_d \t %d \n"
#define RFC2507C_116_112_2_17_22_57_39_4083 "v6s_dropped_d \t %d \n"
#define RFC2507C_117_112_2_17_22_57_39_4084 "v6s_badsum_d \t %d \n"
#define RFC2507C_118_112_2_17_22_57_39_4085 "v6s_head_req_revc \t %d \n"
#define RFC2507C_119_112_2_17_22_57_39_4086 "\n"
#define RFC2507C_172_112_2_17_22_57_39_4087 "free_all: start\n"
#define RFC2507C_208_112_2_17_22_57_39_4088 "free_all: done\n"
#define RFC2507C_414_112_2_17_22_57_39_4089 "iphc_init: start sizeof(*csList) == %u\n"
#define RFC2507C_416_112_2_17_22_57_39_4090 "iphc_init: sizeof(*comp) == %u sizeof(*decomp) == %un"
#define RFC2507C_419_112_2_17_22_57_39_4091 "iphc_init: csList == NULL\n"
#define RFC2507C_428_112_2_17_22_57_39_4092 "iphc_init: cfg == NULL\n"
#define RFC2507C_433_112_2_17_22_57_39_4093 "iphc_init: len_send_t %d \n"
#define RFC2507C_434_112_2_17_22_57_39_4094 "iphc_init: len_recv_t %d \n"
#define RFC2507C_435_112_2_17_22_57_39_4095 "iphc_init: len_send_u %d \n"
#define RFC2507C_436_112_2_17_22_57_39_4096 "iphc_init: len_recv_u %d \n"
#define RFC2507C_437_112_2_17_22_57_39_4097 "iphc_init: max_header %d \n"
#define RFC2507C_438_112_2_17_22_57_39_4098 "iphc_init: max_period %d \n"
#define RFC2507C_439_112_2_17_22_57_39_4099 "iphc_init: max_time   %d \n"
#define RFC2507C_440_112_2_17_22_57_39_4100 "iphc_init: min_wrap   %d \n"
#define RFC2507C_441_112_2_17_22_57_39_4101 "iphc_init: reordering %d \n"
#define RFC2507C_442_112_2_17_22_57_39_4102 "iphc_init: nodelta    %d \n"
#define RFC2507C_443_112_2_17_22_57_39_4103 "iphc_init: udp_prime  %d \n"
#define RFC2507C_444_112_2_17_22_57_39_4104 "iphc_init: tcp_prime  %d \n"
#define RFC2507C_459_112_2_17_22_57_39_4105 "iphc_init: bad init values\n"
#define RFC2507C_464_112_2_17_22_57_39_4106 "iphc_init: bad value for tcp_prime\n"
#define RFC2507C_468_112_2_17_22_57_39_4107 "iphc_init: bad value for udp_prime\n"
#define RFC2507C_539_112_2_17_22_57_39_4108 "iphc_init: init done.\n"
#define RFC2507C_673_112_2_17_22_57_40_4109 "hash_keys: *** UNKNOWN IP version %d ***\n"
#define RFC2507C_678_112_2_17_22_57_40_4110 "iphc_hashkeys: IPv4 hdr corrupt\n"
#define RFC2507C_681_112_2_17_22_57_40_4111 "hash_keys: IP4\n"
#define RFC2507C_688_112_2_17_22_57_40_4112 "hash_keys: length fields for IP4 do not match! Bailing out!\n"
#define RFC2507C_690_112_2_17_22_57_40_4113 "hash_keys: length fields are: pkt %u len %u ip %un"
#define RFC2507C_691_112_2_17_22_57_40_4114 "hash_keys: %s\n"
#define RFC2507C_704_112_2_17_22_57_40_4115 "hash_keys: ip4 fragment\n"
#define RFC2507C_716_112_2_17_22_57_40_4116 "hash_keys: ip4 options len=%u  \n"
#define RFC2507C_739_112_2_17_22_57_40_4117 "hash_keys: mine hdr corrupt\n"
#define RFC2507C_745_112_2_17_22_57_40_4118 "hash_keys: IPPROTO_MIN_CAP  length %u\n"
#define RFC2507C_751_112_2_17_22_57_40_4119 "hash_keys: IPPROTO_MIN_CAP  next %x \n"
#define RFC2507C_757_112_2_17_22_57_40_4120 "hash_keys: auth hdr corrupt\n"
#define RFC2507C_762_112_2_17_22_57_40_4121 "hash_keys: IPPROTO_AH4  length %u \n"
#define RFC2507C_767_112_2_17_22_57_40_4122 "hash_keys: IPPROTO_AH4  next %x \n"
#define RFC2507C_773_112_2_17_22_57_40_4123 "hash_keys: unknown proto %d in ip4\n"
#define RFC2507C_785_112_2_17_22_57_40_4124 "hash_keys: IP6\n"
#define RFC2507C_793_112_2_17_22_57_40_4125 "hash_keys: length fields for IP6 does not match! Bailing out!\n"
#define RFC2507C_816_112_2_17_22_57_40_4126 "hash_keys: extention hdr corrupt\n"
#define RFC2507C_829_112_2_17_22_57_40_4127 "hash_keys: ROUTING\n"
#define RFC2507C_831_112_2_17_22_57_40_4128 "hash_keys: extension hdr corrupt\n"
#define RFC2507C_846_112_2_17_22_57_40_4129 "hash_keys: IPPROTO_AH6  length %u \n"
#define RFC2507C_848_112_2_17_22_57_40_4130 "hash_keys: auth hdr corrupt\n"
#define RFC2507C_857_112_2_17_22_57_40_4131 "hash_keys: IPPROTO_AH6  next %x \n"
#define RFC2507C_874_112_2_17_22_57_40_4132 "hash_keys: FRAGMENT\n"
#define RFC2507C_882_112_2_17_22_57_40_4133 "hash_keys: IPPROTO_NONE\n"
#define RFC2507C_887_112_2_17_22_57_40_4134 "hash_keys: unknown proto %d in ip6_hdr\n"
#define RFC2507C_899_112_2_17_22_57_40_4135 "hash_keys: ESP\n"
#define RFC2507C_908_112_2_17_22_57_40_4136 "hash_keys: UDP\n"
#define RFC2507C_916_112_2_17_22_57_40_4137 "hash_keys: length fields for UDP does not match! Bailing out!\n"
#define RFC2507C_922_112_2_17_22_57_40_4138 "hash_keys: udp ports s %d, d %dn"
#define RFC2507C_928_112_2_17_22_57_40_4139 "hash_keys: TCP\n"
#define RFC2507C_930_112_2_17_22_57_40_4140 "iphc_hashkeys: TCP hdr corrupt\n"
#define RFC2507C_939_112_2_17_22_57_40_4141 "hash_keys: tcp ports s %d, d %dn"
#define RFC2507C_959_112_2_17_22_57_40_4142 "equal_flows: ipv = %d\n"
#define RFC2507C_968_112_2_17_22_57_40_4143 "equal_flows: ip4\n"
#define RFC2507C_978_112_2_17_22_57_40_4144 "equal_flows: ip4 fragment\n"
#define RFC2507C_985_112_2_17_22_57_40_4145 "equal_flows: ip4 options len=%u\n"
#define RFC2507C_994_112_2_17_22_57_40_4146 "equal_flows: ip4h\n"
#define RFC2507C_1033_112_2_17_22_57_40_4147 "equal_flows: unknown proto %d in ip4\n"
#define RFC2507C_1039_112_2_17_22_57_40_4148 "equal_flows: ip6\n"
#define RFC2507C_1058_112_2_17_22_57_40_4149 "equal_flows: ip6 options i=%x t=%x\n"
#define RFC2507C_1111_112_2_17_22_57_41_4150 "equal_flows: frament\n"
#define RFC2507C_1118_112_2_17_22_57_41_4151 "equal_flows: none\n"
#define RFC2507C_1122_112_2_17_22_57_41_4152 "equal_flows: unknown proto %d in ip6_hdr\n"
#define RFC2507C_1128_112_2_17_22_57_41_4153 "equal_flows: esp\n"
#define RFC2507C_1135_112_2_17_22_57_41_4154 "equal_flows: udp\n"
#define RFC2507C_1139_112_2_17_22_57_41_4155 "equal_flows: udp ports s %d, d %dn"
#define RFC2507C_1145_112_2_17_22_57_41_4156 "equal_flows: tcp\n"
#define RFC2507C_1150_112_2_17_22_57_41_4157 "equal_flows: tcp ports s %d, d %dn"
#define RFC2507C_1227_112_2_17_22_57_41_4158 "compress: LARGE_CID %u \n"
#define RFC2507C_1250_112_2_17_22_57_41_4159 "compress: ip4:\n"
#define RFC2507C_1258_112_2_17_22_57_41_4160 "compress: ip4 fragment\n"
#define RFC2507C_1270_112_2_17_22_57_41_4161 "compress: ip4 options len=%u\n"
#define RFC2507C_1299_112_2_17_22_57_41_4162 "compress: ip4h:\n"
#define RFC2507C_1333_112_2_17_22_57_41_4163 "compress: unknown proto %x in ipv4\n"
#define RFC2507C_1339_112_2_17_22_57_41_4164 "compress: ip6:\n"
#define RFC2507C_1468_112_2_17_22_57_41_4165 "compress: proto NONE %x in ip6_hdr\n"
#define RFC2507C_1472_112_2_17_22_57_41_4166 "compress: unknown proto %x in ip6_hdr\n"
#define RFC2507C_1482_112_2_17_22_57_41_4167 "compress: tcp\n"
#define RFC2507C_1487_112_2_17_22_57_41_4168 "compress: tcp seq %u  ack %un"
#define RFC2507C_1491_112_2_17_22_57_41_4169 "compress: doing NODELTA\n"
#define RFC2507C_1510_112_2_17_22_57_41_4170 "compress: header request for cid %u ch %u\n"
#define RFC2507C_1530_112_2_17_22_57_41_4171 "compress: ipv4 r_byte is SET to %x  ip_ecn_save %xn"
#define RFC2507C_1541_112_2_17_22_57_41_4172 "compress: ipv6 r_byte is SET to %x  ip_ecn_save %xn"
#define RFC2507C_1556_112_2_17_22_57_41_4173 "compress: deltaS = %u iseq %u tseq %un"
#define RFC2507C_1614_112_2_17_22_57_42_4174 "compress: NOTHING\n"
#define RFC2507C_1622_112_2_17_22_57_42_4175 "compress: SPECIAL -> full\n"
#define RFC2507C_1626_112_2_17_22_57_42_4176 "compress: NEW_S | NEW_A\n"
#define RFC2507C_1631_112_2_17_22_57_42_4177 "compress: SPECIAL_I\n"
#define RFC2507C_1636_112_2_17_22_57_42_4178 "compress: NEW_S\n"
#define RFC2507C_1641_112_2_17_22_57_42_4179 "compress: SPECIAL_D deltaS = %u\n"
#define RFC2507C_1658_112_2_17_22_57_42_4180 "compress: tcp options %u bytes\n"
#define RFC2507C_1666_112_2_17_22_57_42_4181 "compress: TcpHc= %s\n"
#define RFC2507C_1675_112_2_17_22_57_42_4182 "compress: r_byte  restore ipv4 %x  newvalue=%xn"
#define RFC2507C_1679_112_2_17_22_57_42_4183 "compress: r_byte  restore ipv6 %x  newvalue=%xn"
#define RFC2507C_1689_112_2_17_22_57_42_4184 "compress: done_tcp cinfo=%x \" rdp %x chdr %xn"
#define RFC2507C_1738_112_2_17_22_57_42_4185 "compress: done NODELTA\n"
#define RFC2507C_1746_112_2_17_22_57_42_4186 "compress: CH %s\n"
#define RFC2507C_1748_112_2_17_22_57_42_4187 "compress: comp %d - %d = %dn"
#define RFC2507C_1758_112_2_17_22_57_42_4188 "compress: error cinfo"
#define RFC2507C_1764_112_2_17_22_57_42_4189 "compress: udp\n"
#define RFC2507C_1785_112_2_17_22_57_42_4190 "compress: max_time now - last == xxx %u - %u == %u  max %un"
#define RFC2507C_1804_112_2_17_22_57_42_4191 "compress: bailing out on min wrap violation (cid %u)n"
#define RFC2507C_1812_112_2_17_22_57_42_4192 "compress: min wrap not violated (cid %u)\n"
#define RFC2507C_1851_112_2_17_22_57_42_4193 "compress: max_time expired  sending full\n"
#define RFC2507C_1860_112_2_17_22_57_42_4194 "compress: done_udp %d \" rdp %x chdr %xn"
#define RFC2507C_1956_112_2_17_22_57_42_4195 "compress: CH %s\n"
#define RFC2507C_1958_112_2_17_22_57_42_4196 "compress: comp %d - %d = %dn"
#define RFC2507C_1965_112_2_17_22_57_42_4197 "compress: error cinfo"
#define RFC2507C_1972_112_2_17_22_57_42_4198 "compress: bailing out\n"
#define RFC2507C_2001_112_2_17_22_57_42_4199 "iphc_comp: length of incoming packet < IPv4 hdr\n"
#define RFC2507C_2009_112_2_17_22_57_42_4200 "iphc_comp: ---------- ipv4 iphdr_len %d,  real len %dn"
#define RFC2507C_2014_112_2_17_22_57_42_4201 "iphc_comp: ---------- ipv6 iphdr_len %d,  real len %dn"
#define RFC2507C_2022_112_2_17_22_57_42_4202 "iphc_comp: H  %s\n"
#define RFC2507C_2023_112_2_17_22_57_42_4203 "iphc_comp: H  %s\n"
#define RFC2507C_2048_112_2_17_22_57_43_4204 "iphc_comp: compress => ci_regu\n"
#define RFC2507C_2054_112_2_17_22_57_43_4205 "compress => unknown\n"
#define RFC2507C_2079_112_2_17_22_57_43_4206 "iphc_comp: compress => ci_full\n"
#define RFC2507C_2088_112_2_17_22_57_43_4207 "iphc_comp: compress => ci_comp\n"
#define RFC2507C_2095_112_2_17_22_57_43_4208 "iphc_comp: compress => ci_regu\n"
#define RFC2507C_2101_112_2_17_22_57_43_4209 "compress => unknown\n"
#define RFC2507D_209_112_2_17_22_57_43_4210 "iphc_decompf: ----------  len=%d\n"
#define RFC2507D_210_112_2_17_22_57_43_4211 "iphc_decompf: FH %s\n"
#define RFC2507D_211_112_2_17_22_57_43_4212 "iphc_decompf: FH %s\n"
#define RFC2507D_214_112_2_17_22_57_43_4213 "iphc_decompf: Too large packet, don't try to decompress\n"
#define RFC2507D_222_112_2_17_22_57_43_4214 "iphc_decompf: length of incoming packet smaller < IPv4 hdrn"
#define RFC2507D_230_112_2_17_22_57_43_4215 "iphc_decompf: ipv4\n"
#define RFC2507D_244_112_2_17_22_57_43_4216 "iphc_decompf: ip6_hdr\n"
#define RFC2507D_259_112_2_17_22_57_43_4217 "iphc_decompf: ip4\n"
#define RFC2507D_262_112_2_17_22_57_43_4218 "iphc_decompf: IPv4 hdr corrupt\n"
#define RFC2507D_267_112_2_17_22_57_43_4219 "iphc_decompf: IPv4 hdr length field too small\n"
#define RFC2507D_275_112_2_17_22_57_43_4220 "iphc_decompf: ************ IP4\n"
#define RFC2507D_284_112_2_17_22_57_43_4221 "iphc_decompf: ip4 fragment\n"
#define RFC2507D_306_112_2_17_22_57_43_4222 "hash_keys: mine hdr corrupt\n"
#define RFC2507D_318_112_2_17_22_57_43_4223 "iphc_decompf: auth in ip4_hdr\n"
#define RFC2507D_320_112_2_17_22_57_44_4224 "hash_keys: auth hdr corrupt\n"
#define RFC2507D_333_112_2_17_22_57_44_4225 "iphc_decompf: unknown proto %x in ip4_hdr\n"
#define RFC2507D_339_112_2_17_22_57_44_4226 "iphc_decompf: ip6\n"
#define RFC2507D_344_112_2_17_22_57_44_4227 "iphc_decompf: ************ IP6\n"
#define RFC2507D_362_112_2_17_22_57_44_4228 "hash_keys: extention hdr corrupt\n"
#define RFC2507D_374_112_2_17_22_57_44_4229 "hash_keys: auth hdr corrupt\n"
#define RFC2507D_403_112_2_17_22_57_44_4230 "iphc_decompf: proto NONE %x in ip6_hdr\n"
#define RFC2507D_407_112_2_17_22_57_44_4231 "iphc_decompf: unknown proto %x in ip6_hdr\n"
#define RFC2507D_416_112_2_17_22_57_44_4232 "iphc_decompf: esp\n"
#define RFC2507D_420_112_2_17_22_57_44_4233 "iphc_decompf: udp\n"
#define RFC2507D_430_112_2_17_22_57_44_4234 "iphc_decompf: ************ UDP %u  n"
#define RFC2507D_437_112_2_17_22_57_44_4235 "iphc_decompf: TCP header too small %d\n"
#define RFC2507D_442_112_2_17_22_57_44_4236 "iphc_decompf: tcp\n"
#define RFC2507D_447_112_2_17_22_57_44_4237 "iphc_decompf: tcp header is %d bytes longn"
#define RFC2507D_450_112_2_17_22_57_44_4238 "iphc_decompf: too large tcp cid %d\n"
#define RFC2507D_469_112_2_17_22_57_44_4239 "iphc_decompf: saved r_byte [TTttttII] %x\n"
#define RFC2507D_475_112_2_17_22_57_44_4240 "iphc_decompf: bailing out\n"
#define RFC2507D_481_112_2_17_22_57_44_4241 "decompf: LARGE CID \n"
#define RFC2507D_485_112_2_17_22_57_44_4242 "decompf: LARGE CID %u\n"
#define RFC2507D_490_112_2_17_22_57_44_4243 "iphc_decompf: too large tcp cid %d\n"
#define RFC2507D_497_112_2_17_22_57_44_4244 "iphc_decompf: too large udp cid %d\n"
#define RFC2507D_506_112_2_17_22_57_44_4245 "iphc_decompf: donefull cid %d gen %d len %d \n"
#define RFC2507D_507_112_2_17_22_57_44_4246 "iphc_decompf: FH %s\n"
#define RFC2507D_508_112_2_17_22_57_44_4247 "iphc_decompf: FH %s\n"
#define RFC2507D_526_112_2_17_22_57_44_4248 "iphc_decompf: removed cid %u from queue\n"
#define RFC2507D_571_112_2_17_22_57_44_4249 "iphc_decomp: CH %s\n"
#define RFC2507D_574_112_2_17_22_57_44_4250 "iphc_decomp: Too large packet, don't try to decompress\n"
#define RFC2507D_590_112_2_17_22_57_44_4251 "iphc_decomp: type TCP\n"
#define RFC2507D_605_112_2_17_22_57_44_4252 "iphc_decomp: too large tcp cid %d\n"
#define RFC2507D_609_112_2_17_22_57_44_4253 "iphc_decomp: tcp cid not in use %d\n"
#define RFC2507D_629_112_2_17_22_57_44_4254 "iphc_decomp: type UDP\n"
#define RFC2507D_633_112_2_17_22_57_44_4255 "iphc_decomp: too large udp cid %d\n"
#define RFC2507D_640_112_2_17_22_57_44_4256 "iphc_decomp: udp cid not in use %d\n"
#define RFC2507D_644_112_2_17_22_57_44_4257 "iphc_decomp: bad generation %u %u\n"
#define RFC2507D_662_112_2_17_22_57_44_4258 "iphc_decomp: hmm... ip version %u\n"
#define RFC2507D_669_112_2_17_22_57_44_4259 "iphc_decomp: IPv4 hdr length field too small\n"
#define RFC2507D_672_112_2_17_22_57_44_4260 "iphc_decomp: IP4\n"
#define RFC2507D_677_112_2_17_22_57_44_4261 "decomp: ip4 fragment\n"
#define RFC2507D_690_112_2_17_22_57_44_4262 "iphc_decomp: IPv4 hdr length field too small\n"
#define RFC2507D_713_112_2_17_22_57_44_4263 "decomp: ip4 options size changed \n"
#define RFC2507D_791_112_2_17_22_57_45_4264 "iphc_decomp: unknown proto %x in ip4_hdr\n"
#define RFC2507D_797_112_2_17_22_57_45_4265 "iphc_decomp: IP6\n"
#define RFC2507D_862_112_2_17_22_57_45_4266 "iphc_decomp: IPPROTO_ROUTING segleft=%u n"
#define RFC2507D_863_112_2_17_22_57_45_4267 "iphc_decomp:  %s\n"
#define RFC2507D_867_112_2_17_22_57_45_4268 "iphc_decomp: IPPROTO_ROUTING OPTLEN=%u i=%un"
#define RFC2507D_869_112_2_17_22_57_45_4269 "iphc_decomp: Bad optlen for routing option\n"
#define RFC2507D_909_112_2_17_22_57_45_4270 "iphc_decomp: proto NONE  %x in ip6_hdr\n"
#define RFC2507D_913_112_2_17_22_57_45_4271 "iphc_decomp: unknown proto %x in ip6_hdr\n"
#define RFC2507D_923_112_2_17_22_57_45_4272 "iphc_decomp: TCP  left = %u\n"
#define RFC2507D_953_112_2_17_22_57_45_4273 "iphc_decomp: r_byte *SET* to %6x\n"
#define RFC2507D_956_112_2_17_22_57_45_4274 "iphc_decomp: r_byte default  %6x\n"
#define RFC2507D_974_112_2_17_22_57_45_4275 "iphc_decomp: hmm... ip version %un"
#define RFC2507D_987_112_2_17_22_57_45_4276 "iphc_decomp: SPECIAL_I\n"
#define RFC2507D_994_112_2_17_22_57_45_4277 "iphc_decomp: SPECIAL_DATA seq = %un"
#define RFC2507D_1031_112_2_17_22_57_45_4278 "iphc_decomp: NEW_O but no options in table\n"
#define RFC2507D_1045_112_2_17_22_57_45_4279 "iphc_decomp: UDP space left = %u\n"
#define RFC2507D_1060_112_2_17_22_57_45_4280 "iphc_decomp: bailing out\n"
#define RFC2507D_1132_112_2_17_22_57_45_4281 "iphc_decomp: twice0_cksum %x   recv_cksum %xn"
#define RFC2507D_1141_112_2_17_22_57_45_4282 "iphc_decomp: try twice once with delta = %xn"
#define RFC2507D_1150_112_2_17_22_57_45_4283 "iphc_decomp: twice1_cksum %x   recv_cksum %xn"
#define RFC2507D_1154_112_2_17_22_57_45_4284 "iphc_decomp: success with twice.\n"
#define RFC2507D_1161_112_2_17_22_57_45_4285 "iphc_decomp: failed twice \n"
#define RFC2507D_1175_112_2_17_22_57_45_4286 "iphc_decomp: Checksum mismatch, Twice not supported. \n"
#define RFC2507D_1195_112_2_17_22_57_45_4287 "iphc_decomp: header request queued cid = %u\n"
#define RFC2507D_1210_112_2_17_22_57_45_4288 "iphc_decomp: hlen %d   cs->len %d\n"
#define RFC2507D_1214_112_2_17_22_57_45_4289 "iphc_decomp: cid %d  gen %d  hlen %d  comp_head_len %dn"
#define RFC2507D_1215_112_2_17_22_57_45_4290 "iphc_decomp: DH %s\n"
#define RFC2507D_1216_112_2_17_22_57_45_4291 "iphc_decomp: DH %s\n"
#define SNDCP_ENTITY_745_112_2_17_22_57_49_4292 "SNCP: nsapi %d discard SDU on %d state"
#define SNDCP_ENTITY_821_112_2_17_22_57_49_4293 "SNCP: NSAPI%d discard PDU on %d state"
#define SNDCP_ENTITY_851_112_2_17_22_57_49_4294 "SNDCP: Discard NPDU when reaching on the max buffer_size!"
#define SNDCP_ENTITY_885_112_2_17_22_57_49_4295 "SNDCP: NSAPI %d Discard NPDU on %d state!"
#define SNDCP_ENTITY_989_112_2_17_22_57_49_4296 "SNCP: NSAPI%d discard PDU on %d state"
#define SNDCP_ENTITY_1527_112_2_17_22_57_50_4297 "SNDCP: received release_ind with NORMAL_RELEASE on esting state"
#define SNDCP_ENTITY_1537_112_2_17_22_57_50_4298 "SNDCP: received not defined cause: %d"
#define SNDCP_MAIN_639_112_2_17_22_57_52_4299 "SNDCP: invalue nsapi"
#define SNDCP_MAIN_650_112_2_17_22_57_52_4300 "SNDCP: invalue sapi"
#define SNDCP_MAIN_661_112_2_17_22_57_52_4301 "SNDCP: NSAPI already existed"
#define SNDCP_MAIN_717_112_2_17_22_57_52_4302 "SNDCP: NSAPI entity doesnt exist."
#define SNDCP_MAIN_1073_112_2_17_22_57_53_4303 "SNDCP: SN_UNITDATA_REQ - Null pointer"
#define SNDCP_MAIN_1113_112_2_17_22_57_53_4304 "SNDCP: LL_SNDC_UNITDATA_IND - Null pointer"
#define SNDCP_MAIN_1121_112_2_17_22_57_53_4305 "SNDCP: LL_SNDC_UNITDATA_IND - wrong nsapi"
#define SNDCP_MAIN_1128_112_2_17_22_57_53_4306 "SNDCP: LL_SNDC_UNITDATA_IND - wrong sapi"
#define SNDCP_MAIN_1135_112_2_17_22_57_53_4307 "SNDCP: LL_SNDC_UNITDATA_IND - wrong nsapi"
#define SNDCP_MAIN_1162_112_2_17_22_57_53_4308 "SNDCP:SN_DATA_REQ - Null pointer"
#define SNDCP_MAIN_1202_112_2_17_22_57_53_4309 "SNDCP: LL_SNDC_DATA_IND - Null pointer"
#define SNDCP_MAIN_1210_112_2_17_22_57_53_4310 "SNDCP: LL_SNDC_DATA_IND - wrong nsapi"
#define SNDCP_MAIN_1217_112_2_17_22_57_53_4311 "SNDCP: LL_SNDC_UNITDATA_IND - wrong nsapi"
#define SNDCP_MAIN_1224_112_2_17_22_57_53_4312 "SNDCP: LL_SNDC_UNITDATA_IND - wrong nsapi"
#define SNDCP_MAIN_1278_112_2_17_22_57_53_4313 "SNDCP: suspend sapi not exist."
#define SNDCP_MAIN_1316_112_2_17_22_57_54_4314 "SNDCP: resume sapi not exist."
#define SNDCP_MAIN_1406_112_2_17_22_57_54_4315 "SNDCP: modify invalue nsapi"
#define SNDCP_MAIN_1417_112_2_17_22_57_54_4316 "SNDCP: modify invalue sapi"
#define SNDCP_MAIN_1711_112_2_17_22_57_54_4317 "SNDCP: save signal %d"
#define SNDCP_MAIN_1724_112_2_17_22_57_54_4318 "SNDCP: discard signal %d"
#define SNDCP_PC_2629_112_2_17_22_58_1_4319 "SNDCP:pc_decompress result = %d, s_sndcp_pc0.nsapis = %d, nsapi = %d"
#define SNDCP_SDL_452_112_2_17_22_58_3_4320 "SNDCP: Unack seg_count = %d"
#define SNDCP_SDL_726_112_2_17_22_58_4_4321 "SNDCP_HandleUnsequentUnackBuffer reset_flag = %d, seg_count = %d"
#define SNDCP_SDL_789_112_2_17_22_58_4_4322 "SNDCP: first segment"
#define SNDCP_SDL_913_112_2_17_22_58_4_4323 "SNDCP: SNDCP_HandleTllSndcUnitdataInd receiving_state = %d"
#define SNDCP_SDL_969_112_2_17_22_58_4_4324 "SNDCP: DATA-SNPDU F field set in receive subsequent state.\n"
#define SNDCP_SNPDU_447_112_2_17_22_58_5_4325 "SNDCP:invalid nsapi %d"
#define SNDCP_SNPDU_464_112_2_17_22_58_5_4326 "SNDCP:invalid sapi %d"
#define XID_SDL_428_112_2_17_22_58_8_4327 "SNDCP: RenegotiateReq result = %d"
#define SS_EVENT_45_112_2_17_22_58_10_4328 "SS_TASK: send MNSS_BEGIN_IND to task id =%d."
#define SS_EVENT_70_112_2_17_22_58_10_4329 "SS_TASK: send MNSS_FACILITY_IND to task id =%d."
#define SS_EVENT_102_112_2_17_22_58_11_4330 "SS_TASK: send MNSS_END_IND to task id =%d."
#define SS_MAIN_567_112_2_17_22_58_12_4331 "SS_TASK: ti=%d entity is present"
#define SS_MAIN_571_112_2_17_22_58_12_4332 "SS_TASK: ti=%d entity is not present"
#define SS_MAIN_585_112_2_17_22_58_12_4333 "SS_TASK: initial value"
#define SS_MAIN_619_112_2_17_22_58_12_4334 "SS_TASK: receive SS_MM_EST_IND"
#define SS_MAIN_623_112_2_17_22_58_12_4335 "SS_TASK: receive SS_MM_EST_CNF"
#define SS_MAIN_627_112_2_17_22_58_12_4336 "SS_TASK: receive SS_MM_REL_IND"
#define SS_MAIN_632_112_2_17_22_58_12_4337 "SS_TASK: receive SS_MM_DATA_IND"
#define SS_MAIN_636_112_2_17_22_58_12_4338 "SS_TASK: receive SS_MM_ERR_IND"
#define SS_MAIN_648_112_2_17_22_58_12_4339 "SS_TASK: receive MNSS_BEGIN_REQ"
#define SS_MAIN_652_112_2_17_22_58_12_4340 "SS_TASK: receive MNSS_FACILITY_REQ"
#define SS_MAIN_656_112_2_17_22_58_12_4341 "SS_TASK: receive MNSS_END_REQ"
#define SS_MAIN_660_112_2_17_22_58_12_4342 "SS_TASK: diacard received signals"
#define SS_SIGNAL_46_112_2_17_22_58_14_4343 "SS_TASK: send CM_MM_DATA_REQ to task id =%d."
#define SS_SIGNAL_68_112_2_17_22_58_14_4344 "SS_TASK: send CM_MM_REL_REQ to task id =%d."
#define SS_SIGNAL_94_112_2_17_22_58_14_4345 "SS_TASK: send CM_MM_EST_REQ to task id =%d."
#define SIM_DIALING_NUMBER_433_112_2_17_22_58_17_4346 "SIM: EXT1 exceed max %d, %d"
#define SIM_DIALING_NUMBER_451_112_2_17_22_58_17_4347 "SIM: EXT2 exceed max %d, %d "
#define SIM_DIALING_NUMBER_470_112_2_17_22_58_17_4348 "SIM: EXT3 exceed max %d, %d "
#define SIM_DIALING_NUMBER_636_112_2_17_22_58_18_4349 "SIM::record number of extfile 0x%x is %d"
#define SIM_DIALING_NUMBER_686_112_2_17_22_58_18_4350 "SIM::dn_id 0x%x is related with ext_id 0x%x"
#define SIM_DIALING_NUMBER_703_112_2_17_22_58_18_4351 "SIM::next id of ext_id 0x%x is 0x%x"
#define SIM_DIALING_NUMBER_711_112_2_17_22_58_18_4352 "SIM::next id of ext_id 0x%x is null"
#define SIM_DIALING_NUMBER_729_112_2_17_22_58_18_4353 "SIM::No.0x%x record of the ext file is free"
#define SIM_DIALING_NUMBER_736_112_2_17_22_58_18_4354 "SIM::update the record to free is failed"
#define SIM_DIALING_NUMBER_952_112_2_17_22_58_18_4355 "SIM%d:DN EXT found multi dn_id %d use ext_id %d. %d"
#define SIM_DIALING_NUMBER_985_112_2_17_22_58_18_4356 "SIM%d:related DN EXT found multi dn_id %d use ext_id %d. %d"
#define SIM_DIALING_NUMBER_1066_112_2_17_22_58_19_4357 "SIM%d DN read started %d,"
#define SIM_DIALING_NUMBER_1199_112_2_17_22_58_19_4358 "SIM::read ext_id:%d failed when read DN,result is %d."
#define SIM_DIALING_NUMBER_1308_112_2_17_22_58_19_4359 "SIM%d DN_%d read finished %d, ext_num is %d, ext_id %d"
#define SIM_DIALING_NUMBER_1346_112_2_17_22_58_19_4360 "GetExtInfoPtr dn_type = %d"
#define SIM_DIALING_NUMBER_1433_112_2_17_22_58_20_4361 "SIM%d: -- EXT %d"
#define SIM_DIALING_NUMBER_1437_112_2_17_22_58_20_4362 "SIM%d:DN delete dn_id_%d 's ext rec as 0xFF"
#define SIM_DIALING_NUMBER_1448_112_2_17_22_58_20_4363 "SIM%d: -- EXT recover %d"
#define SIM_DIALING_NUMBER_1525_112_2_17_22_58_20_4364 "SIM%d: ++ EXT %d"
#define SIM_DIALING_NUMBER_1651_112_2_17_22_58_20_4365 "SIM: update DN EXT test. %d"
#define SIM_DIALING_NUMBER_2706_112_2_17_22_58_22_4366 "SIM::enter SIMDN_GetFreeExtInfo"
#define SIM_DIALING_NUMBER_2738_112_2_17_22_58_22_4367 "SIM::total record number of extfile 0x%x is %d"
#define SIM_DIALING_NUMBER_2803_112_2_17_22_58_22_4368 "SIM::seek result = 0x%x"
#define SIM_DIALING_NUMBER_2848_112_2_17_22_58_22_4369 "SIM::result is 0x%x,free ext record number is 0x%x"
#define SIM_DIALING_NUMBER_2950_112_2_17_22_58_23_4370 "SIM%d:DN_SetIsADNinDFteleDFpb ADN 0x%x, EXT1 0x%x. %d sets"
#define SIM_DIALING_NUMBER_3050_112_2_17_22_58_23_4371 "SIM%d DN GetDNFileName 0x%x, remap from %d to %d"
#define SIM_DIALING_NUMBER_3112_112_2_17_22_58_23_4372 "SIM%d: PBR rec1 Invalid ADN file id 0x%x!!!"
#define SIM_DIALING_NUMBER_3123_112_2_17_22_58_23_4373 "SIM%d: PBR rec%d Invalid ADN file id 0x%x!!!"
#define SIM_DIALING_NUMBER_3193_112_2_17_22_58_23_4374 "SIMDN_GetUSIMADNRecordInfomation, 1st ADN has %d rec, max rec%d. %d %d %d %d"
#define SIM_DIALING_NUMBER_3232_112_2_17_22_58_23_4375 "SIMDN_GetUSIMADNRecordInfomation, 2nd ADN has %d rec, max rec%d. %d %d %d %d"
#define SIM_DIALING_NUMBER_3423_112_2_17_22_58_24_4376 "SIMDN%d: search the empty record number is %d"
#define SIM_EF_DECODE_1826_112_2_17_22_58_28_4377 "SIM%d:DecodeELP 0x%x 0x%x"
#define SIM_EF_DECODE_1871_112_2_17_22_58_28_4378 "SIM%d:DecodeLP 0x%x"
#define SIM_EF_DECODE_1914_112_2_17_22_58_28_4379 "SIM::The IMSI contains 8 bytes of 0xFF."
#define SIM_EF_DECODE_1935_112_2_17_22_58_28_4380 "SIM::ss imsi The MCC&MNC are 0xFF."
#define SIM_EF_DECODE_2158_112_2_17_22_58_28_4381 "SIM%d:DecodePLMNsel i=%d, 0x%x 0x%x 0x%x"
#define SIM_EF_DECODE_2225_112_2_17_22_58_28_4382 "SIM::warning,the len of EFacmmax is less than 3."
#define SIM_EF_DECODE_2261_112_2_17_22_58_28_4383 "SIM::warning,the len of EFsst is less than 2."
#define SIM_EF_DECODE_2281_112_2_17_22_58_28_4384 "SIM%d SST len %d, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
#define SIM_EF_DECODE_2350_112_2_17_22_58_29_4385 "SIM::warning,the len of EFacm is less than 3."
#define SIM_EF_DECODE_2383_112_2_17_22_58_29_4386 "SIM::warning,the len of the GID1 is out of range."
#define SIM_EF_DECODE_2415_112_2_17_22_58_29_4387 "SIM::warning,the len of the GID2 is out of range."
#define SIM_EF_DECODE_2446_112_2_17_22_58_29_4388 "SIM::warning,the len of EFpuct is less than 5."
#define SIM_EF_DECODE_2525_112_2_17_22_58_29_4389 "SIM::warning the len of the EFspn is %d."
#define SIM_EF_DECODE_2599_112_2_17_22_58_29_4390 "SIM::warning,the len of EFbcch is less than 16."
#define SIM_EF_DECODE_2629_112_2_17_22_58_29_4391 "SIM::warning,the len of EFpuct is less than 2."
#define SIM_EF_DECODE_2700_112_2_17_22_58_29_4392 "SIM: DecodeFPLMN data_len = %d, max_num = %d."
#define SIM_EF_DECODE_2727_112_2_17_22_58_29_4393 "SIM::warning,the len of EFloci is not 11."
#define SIM_EF_DECODE_2754_112_2_17_22_58_29_4394 "SIM::DecodeLOCI mcc-mnc is 0xff 0xff 0xff."
#define SIM_EF_DECODE_2796_112_2_17_22_58_30_4395 "SIM::DecodeLOCI update_status byte is 0x%x and is invalid."
#define SIM_EF_DECODE_2827_112_2_17_22_58_30_4396 "SIM::warning,the len of EFad is less than 3."
#define SIM_EF_DECODE_2914_112_2_17_22_58_30_4397 "SIM::warning,the len of EFphase is less than 1."
#define SIM_EF_DECODE_3361_112_2_17_22_58_31_4398 "SIM::warning,the len of EFdck is less than 16."
#define SIM_EF_DECODE_3524_112_2_17_22_58_31_4399 "SIM::warning,the len of EFkcgprs is not 9."
#define SIM_EF_DECODE_3561_112_2_17_22_58_31_4400 "SIM::warning,the len of EFlocigprs is not 14."
#define SIM_EF_DECODE_3613_112_2_17_22_58_31_4401 "SIM::DecodeLOCIGPRS mcc-mnc is 0xff 0xff 0xff."
#define SIM_EF_DECODE_3787_112_2_17_22_58_32_4402 "MM:The length of the network name is too long"
#define SIM_EF_DECODE_3861_112_2_17_22_58_32_4403 "MM:The length of the network name is too long"
#define SIM_EF_DECODE_3986_112_2_17_22_58_32_4404 "SIM::the alpha id len is bigger than 60."
#define SIM_EF_DECODE_4529_112_2_17_22_58_33_4405 "SIM::warning:the length of the EFccp is %d.fill data in it."
#define SIM_EF_DECODE_4598_112_2_17_22_58_33_4406 "SIM::warning,the len of EFsmsp is less than 28."
#define SIM_EF_DECODE_4632_112_2_17_22_58_33_4407 "SIM::warning the alpha id len of the EFsmsp is %d."
#define SIM_EF_DECODE_4744_112_2_17_22_58_34_4408 "SIM::warning,the len of EFsmss is less than 2."
#define SIM_EF_DECODE_4819_112_2_17_22_58_34_4409 "SIM::warning,the len of EFsmsr is less than 30."
#define SIM_EF_DECODE_4885_112_2_17_22_58_34_4410 "SIM::warning,the len of EFext1 is less than 13."
#define SIM_EF_DECODE_4981_112_2_17_22_58_34_4411 "SIM::warning the alpha id len of the EFlnd is %d."
#define SIM_EF_DECODE_5178_112_2_17_22_58_34_4412 "SIM::decode mcc&mnc error."
#define SIM_EF_DECODE_5360_112_2_17_22_58_35_4413 "SIM:: DecodeCPHSInfo, data_len is %d, 0x%x 0x%x 0x%x"
#define SIM_EF_DECODE_5418_112_2_17_22_58_35_4414 "SIM:: DecodeVoiceMsgWtFlag, data_len is %d "
#define SIM_EF_DECODE_5507_112_2_17_22_58_35_4415 "SIM::the alpha id len is bigger than 20."
#define SIM_EF_DECODE_5611_112_2_17_22_58_35_4416 "SIM::the alpha id len is bigger than 20."
#define SIM_EF_DECODE_5641_112_2_17_22_58_35_4417 "SIM::the alpha id len is bigger than 20."
#define SIM_EF_DECODE_5960_112_2_17_22_58_36_4418 "SIM%d:DecodePLMNWACT i=%d, 0x%x 0x%x 0x%x"
#define SIM_EF_DECODE_6065_112_2_17_22_58_36_4419 "SIM%d:DecodeOPLMNWACT i=%d, 0x%x 0x%x 0x%x"
#define SIM_EF_DECODE_6573_112_2_17_22_58_37_4420 "SIM%d UST len %d, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
#define SIM_EF_DECODE_7275_112_2_17_22_58_39_4421 "SIM::warning,the len of EFpsloci is not 14."
#define SIM_EF_DECODE_7325_112_2_17_22_58_39_4422 "SIM::DecodeUsimPSLOCI mcc-mnc is 0xff 0xff 0xff."
#define SIM_EF_DECODE_7477_112_2_17_22_58_39_4423 "SIM PBR 2  0x%x, %d %d, len %d"
#define SIM_EF_DECODE_7538_112_2_17_22_58_39_4424 "Decode EHPLMN"
#define SIM_EF_DECODE_7676_112_2_17_22_58_40_4425 "SIM::warning,the len of EFcst is 0"
#define SIM_EF_ENCODE_4031_112_2_17_22_58_49_4426 "SIM::EncodeSMS,Unrecognized sms_status=%d received."
#define SIM_EF_ENCODE_4796_112_2_17_22_58_50_4427 "SIM:: EncodeCCFlag, data_len is %d "
#define SIM_EF_ENCODE_4857_112_2_17_22_58_50_4428 "SIM:: EncodeVoiceMsgWtFlag, data_len is %d "
#define SIM_EF_ENCODE_4906_112_2_17_22_58_50_4429 "SIM:: call func EncodeCSP()"
#define SIM_EF_ENCODE_5030_112_2_17_22_58_51_4430 "SIM:: EncodeCSP, data_len is %d "
#define SIM_EF_ENCODE_6421_112_2_17_22_58_54_4431 "SIM:: EncodeCSP, data_len is %d "
#define SIM_FILE_SYSTEM_1240_112_2_17_22_58_57_4432 "SIM%d: remap EF %d to %d"
#define SIM_HAL_1008_112_2_17_22_59_0_4433 "SIMHAL%d: init DMA"
#define SIM_HAL_1385_112_2_17_22_59_0_4434 "SIMHAL%d::Wrong procedure byte 0x%x"
#define SIM_HAL_2045_112_2_17_22_59_2_4435 "SIMHAL%d::DMA RX T.O, %d 0x%x 0x%x. %d %d %d %d. i %d"
#define SIM_HAL_2049_112_2_17_22_59_2_4436 "SIMHAL%d::DMA RX T.O, %d 0x%x 0x%x. %d %d %d %d. i %d"
#define SIM_HAL_2086_112_2_17_22_59_2_4437 "SIMHAL%d:DMA got the last 1 byte 0x%x"
#define SIM_HAL_2306_112_2_17_22_59_2_4438 "SIMHAL:do not deal active done Int"
#define SIM_HAL_2565_112_2_17_22_59_3_4439 "SIMHAL0::Reset wait ldo timer expired. %d"
#define SIM_HAL_2635_112_2_17_22_59_3_4440 "SIMHAL1::Reset wait ldo timer expired."
#define SIM_HAL_2959_112_2_17_22_59_4_4441 "SIMHAL%d  %d:ReportCardStatus %d"
#define SIM_HAL_2988_112_2_17_22_59_4_4442 "SIMHAL%d::SIM_ResetDriver %d. %d %d %d %d %d %d"
#define SIM_HAL_2992_112_2_17_22_59_4_4443 "SIMHAL%d::SIM_ResetDriver %d. %d %d %d %d, %d %d %d %d"
#define SIM_HAL_2995_112_2_17_22_59_4_4444 "SIMHAL%d::SIM_ResetDriver %d. %d %d %d %d"
#define SIM_HAL_3085_112_2_17_22_59_4_4445 "SIMHAL0::Init wait ldo timer expired. %d %d"
#define SIM_HAL_3152_112_2_17_22_59_4_4446 "SIMHAL%d::Init wait ldo timer expired."
#define SIM_HAL_3463_112_2_17_22_59_5_4447 "SIMHAL%d::rx parity error!"
#define SIM_HAL_3482_112_2_17_22_59_5_4448 "SIMHAL%d::rx parity error!"
#define SIM_HAL_3499_112_2_17_22_59_5_4449 "SIMHAL%d::TxParityErrorIntP"
#define SIM_HAL_3553_112_2_17_22_59_5_4450 "SIMHAL::DMA Card Unresponse Int"
#define SIM_HAL_3559_112_2_17_22_59_5_4451 "SIMHAL%d::CardUnResponseIntP"
#define SIM_HAL_3794_112_2_17_22_59_5_4452 "SIMHAL%d::Tx fifo not empty"
#define SIM_HAL_3805_112_2_17_22_59_5_4453 "SIMHAL%d:cmd 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
#define SIM_HAL_3809_112_2_17_22_59_5_4454 "SIMHAL%d:cmd 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
#define SIM_HAL_3948_112_2_17_22_59_6_4455 "SIMHAL%d: status words 0x%02x,0x%02x"
#define SIM_HAL_4219_112_2_17_22_59_6_4456 "SIMHAL%d: set volt %d"
#define SIM_HAL_4233_112_2_17_22_59_6_4457 "SIMHAL%d: set volt %d"
#define SIM_HAL_4349_112_2_17_22_59_6_4458 "SIMHAL%d::Reset the SIM card!%d"
#define SIM_HAL_4429_112_2_17_22_59_7_4459 "SIMHAL::Reset first time!%d"
#define SIM_HAL_4450_112_2_17_22_59_7_4460 "SIMHAL%d::Reset to change power."
#define SIM_HAL_4468_112_2_17_22_59_7_4461 "SIMHAL%d::Reject for no next available volt. %d"
#define SIM_HAL_4476_112_2_17_22_59_7_4462 "SIMHAL%d::Reset %d time for power change."
#define SIM_HAL_4481_112_2_17_22_59_7_4463 "SIMHAL%d::Reset to change bit convention! %d"
#define SIM_HAL_4495_112_2_17_22_59_7_4464 "SIMHAL%d::Reset to change parity check! %d"
#define SIM_HAL_4509_112_2_17_22_59_7_4465 "ResetSIMCard reset_type = %d"
#define SIM_HAL_4950_112_2_17_22_59_8_4466 "SIMHAL%d:RejectSIMCard 0x%x"
#define SIM_HAL_5300_112_2_17_22_59_8_4467 "SIMHAL%d:high TA1 0x%x"
#define SIM_HAL_5404_112_2_17_22_59_8_4468 "SIMHAL%d:atr 1.8V only is %d, 0x%x"
#define SIM_HAL_5441_112_2_17_22_59_9_4469 "SIMHAL%d::PPS to sim 0x%x 0x%x 0x%x 0x%x"
#define SIM_HAL_5463_112_2_17_22_59_9_4470 "SIMHAL%d::enable the seh function for b5 and higher version chip!0x%x"
#define SIM_HAL_5522_112_2_17_22_59_9_4471 "SIMHAL%d::SIM DMA enable!0x%x."
#define SIM_HAL_5597_112_2_17_22_59_9_4472 "SIMHAL%d:unexpected rx. %d"
#define SIM_HAL_6019_112_2_17_22_59_10_4473 "SIMHAL%d::Power Off SIM card. %d %d %d"
#define SIM_HAL_6021_112_2_17_22_59_10_4474 "SIMHAL%d::Power Off SIM card. %d %d"
#define SIM_HAL_6249_112_2_17_22_59_10_4475 "SIMHAL%d:switch to sim%d. 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x."
#define SIM_HAL_6254_112_2_17_22_59_10_4476 "SIMHAL%d:switch to sim%d. 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x. %d %d"
#define SIM_HAL_7231_112_2_17_22_59_12_4477 "SIMHAL%d:after select. interface_index %d... %d %d... %d %d"
#define SIM_HAL_7259_112_2_17_22_59_12_4478 "SIMHAL%d:Set RST HIGH 0x%x 0x%x 0x%x"
#define SIM_HAL_7261_112_2_17_22_59_12_4479 "SIMHAL%d:Set RST HIGH 0x%x 0x%x 0x%x 0x%x"
#define SIM_HAL_7263_112_2_17_22_59_12_4480 "SIMHAL%d:Set RST HIGH 0x%x 0x%x"
#define SIM_HAL_7407_112_2_17_22_59_13_4481 "SIMHAL%d:SIM1_RST_After_Tb_or_Te. 0x%x"
#define SIM_HAL_7409_112_2_17_22_59_13_4482 "SIMHAL%d:SIM1_RST_After_Tb_or_Te."
#define SIM_HAL_7523_112_2_17_22_59_13_4483 "SIMHAL1:SIM2_RST_After_Tb_or_Te"
#define SIM_HAL_7804_112_2_17_22_59_13_4484 "SIMHAL:SPI init"
#define SIM_HAL_8029_112_2_17_22_59_14_4485 "SIMHAL%d:SPI to 6302 reg%d, value 0x%x"
#define SIM_HAL_8065_112_2_17_22_59_14_4486 "SIMHAL%d:SPI 1st write 0x%x"
#define SIM_HAL_8082_112_2_17_22_59_14_4487 "SIMHAL%d:SPI%d to 6303 reg%d, value 0x%x.... RST0x%x CLK0x%x DATA0x%x VSIM0x%x, RST0x%x CLK0x%x DATA0x%x VSIM0x%x, RST0x%x CLK0x%x DATA0x%x VSIM0x%x"
#define SIM_HAL_8109_112_2_17_22_59_14_4488 "SIMHAL%d:SPI to 6302 reg%d, value 0x%x.... RST0x%x CLK0x%x DATA0x%x VSIM0x%x, RST0x%x CLK0x%x DATA0x%x VSIM0x%x"
#define SIM_HAL_8495_112_2_17_22_59_15_4489 "SIMHAL_SIM0_Plugin_GPIO_Callback %d"
#define SIM_HAL_8565_112_2_17_22_59_15_4490 "SIMHAL_SIM0_Hotswap_Stable_TimeOut %d"
#define SIM_HAL_8612_112_2_17_22_59_15_4491 "SIMCard Status: generated SIMCard code=0x%x"
#define SIM_HAL_8654_112_2_17_22_59_15_4492 "SIMHAL%d::Rx data buf(1-16):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x n"
#define SIM_HAL_8681_112_2_17_22_59_15_4493 "SIMHAL%d::Tx total len:%d,Sent len:%d,Tx buf(1-16):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x n"
#define SIM_HAL_8699_112_2_17_22_59_15_4494 "SIMHAL::atr data len = %d, data buf(%d-%d):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x n"
#define SIM_HAL_8706_112_2_17_22_59_15_4495 "SIMHAL::atr data buf(%d-%d):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x n"
#define SIM_INSTRUCTION_712_112_2_17_22_59_17_4496 "SIM::Fetch Proactive Command Error!"
#define SIM_INSTRUCTION_1302_112_2_17_22_59_18_4497 "SIM::Status_word = %d\n"
#define SIM_INSTRUCTION_2338_112_2_17_22_59_20_4498 "SIM SELECT %d, %d %d, file_route 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
#define SIM_INSTRUCTION_2392_112_2_17_22_59_20_4499 "SIM::sw is 91xx when do select instruction."
#define SIM_INSTRUCTION_2666_112_2_17_22_59_21_4500 "SIM::redo the select when sw is 91xx. %d %d... 0x%x"
#define SIM_INSTRUCTION_2724_112_2_17_22_59_21_4501 "SIM%d:file_name=%d, ins_name=%d, sw1=%x, sw2=%x, data_len=%d, data=%x, %x, %x"
#define SIM_INSTRUCTION_2740_112_2_17_22_59_21_4502 "SIM%d:get response: ins_name=%d, sw1=%x, sw2=%x, data_len=%d, data=%x, %x, %x"
#define SIM_INSTRUCTION_2898_112_2_17_22_59_21_4503 "SIM::timer expired and the card do not need waiting."
#define SIM_INSTRUCTION_2909_112_2_17_22_59_21_4504 "SIM%d::timer expired but the card need waiting."
#define SIM_INSTRUCTION_2916_112_2_17_22_59_21_4505 "SIM%d::timer expired and the card do not need waiting. %d"
#define SIM_INSTRUCTION_2944_112_2_17_22_59_21_4506 "SIM%d::too many 0x60, do not need waiting. %d"
#define SIM_INSTRUCTION_2962_112_2_17_22_59_21_4507 "SIM%d::SIMINSTR_ReceiveInstrResponse default %d"
#define SIM_INSTRUCTION_3006_112_2_17_22_59_21_4508 "SIM%d::Rcv Power off req in Receive Response"
#define SIM_INSTRUCTION_3113_112_2_17_22_59_22_4509 "SIM%d:AddThisSIGNALInTempSignalQueue 0x%x"
#define SIM_INSTRUCTION_3186_112_2_17_22_59_22_4510 "SIM::volt 5v ony. 0x%x"
#define SIM_INSTRUCTION_3190_112_2_17_22_59_22_4511 "SIM::volt 3v tech. 0x%x"
#define SIM_INSTRUCTION_3194_112_2_17_22_59_22_4512 "SIM::volt 1.8v tech. 0x%x"
#define SIM_INSTRUCTION_3198_112_2_17_22_59_22_4513 "SIM::volt future class. 0x%x"
#define SIM_INSTRUCTION_3202_112_2_17_22_59_22_4514 "SIM::volt not correct value. 0x%x"
#define SIM_INSTRUCTION_3207_112_2_17_22_59_22_4515 "GetCardSupplyVot supply_volt = %d"
#define SIM_INSTRUCTION_3409_112_2_17_22_59_22_4516 "SIM%d:large transparent EF %d, %d"
#define SIM_INSTRUCTION_3615_112_2_17_22_59_23_4517 "SIM::SIMINSTR_UpdateEF,ERROR"
#define SIM_INSTRUCTION_6124_112_2_17_22_59_28_4518 "SIM::SIMINSTR_SelectFileByPath fileid = 0x%x. file_route = 0x%x,0x%x,0x%x"
#define SIM_INSTRUCTION_6274_112_2_17_22_59_28_4519 "SIM:set cur file id 0x%x."
#define SIM_INSTRUCTION_7801_112_2_17_22_59_31_4520 "SIM fcp 0x%x: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
#define SIM_INSTRUCTION_7833_112_2_17_22_59_31_4521 "SIM:SIMINSTR_DecodeUSIMFCP_FileDescriptor,ERROR"
#define SIM_INSTRUCTION_7892_112_2_17_22_59_31_4522 "SIM:SIMINSTR_DecodeUSIMFCP_LifeCycleStatus,ERROR"
#define SIM_INSTRUCTION_7904_112_2_17_22_59_31_4523 "SIM:SIMINSTR_DecodeUSIMFCP_SecurityAttributes,ERROR"
#define SIM_INSTRUCTION_7973_112_2_17_22_59_31_4524 "SIM:SIMINSTR_DecodeUSIMFCP_FileDescriptor,ERROR"
#define SIM_INSTRUCTION_7986_112_2_17_22_59_31_4525 "SIM:SIMINSTR_DecodeUSIMFCP_FileID,ERROR"
#define SIM_INSTRUCTION_8005_112_2_17_22_59_31_4526 "SIM:SIMINSTR_DecodeUSIMFCP_LifeCycleStatus,ERROR"
#define SIM_INSTRUCTION_8017_112_2_17_22_59_31_4527 "SIM:SIMINSTR_DecodeUSIMFCP_SecurityAttributes,ERROR"
#define SIM_INSTRUCTION_8031_112_2_17_22_59_31_4528 "SIM:SIMINSTR_DecodeUSIMFCP_FileSize,ERROR"
#define SIM_INSTRUCTION_8063_112_2_17_22_59_31_4529 "SIM:DecodeUSIMFCP final: filesize 0x%x, fileid 0x%x, chara 0x%x, rec_len 0x%x, status 0x%x"
#define SIM_INSTRUCTION_8212_112_2_17_22_59_32_4530 "SIM EFarr test point2 %d"
#define SIM_INSTRUCTION_8260_112_2_17_22_59_32_4531 "SIM EFarr decode %d,  0x%x 0x%x"
#define SIM_MAIN_772_112_2_17_22_59_34_4532 "SIM::DriverNotifySIM, msg->message= %d"
#define SIM_MAIN_807_112_2_17_22_59_34_4533 "\n\rSIM::SIM Layer Task %d StartUp! %d."
#define SIM_MAIN_1331_112_2_17_22_59_35_4534 "SIM%d::NoPowerState enable deepsleep."
#define SIM_MAIN_1417_112_2_17_22_59_35_4535 "SIM::No power state received abnormal signal:signal_code =0x%x,sender =0x%x\n"
#define SIM_MAIN_1624_112_2_17_22_59_36_4536 "SIM%d::NullState enable deepsleep."
#define SIM_MAIN_1693_112_2_17_22_59_36_4537 "SIM::Null state received abnormal signal:signal_code =0x%x,sender =0x%x\n"
#define SIM_MAIN_1838_112_2_17_22_59_36_4538 "SIM%d: from atr class change. %d"
#define SIM_MAIN_1864_112_2_17_22_59_36_4539 "SIM%d: ATR indicate 1.8V only."
#define SIM_MAIN_1944_112_2_17_22_59_36_4540 "SIM%d::card supply volt is:%x"
#define SIM_MAIN_1972_112_2_17_22_59_37_4541 "SIM::the card is 5v card."
#define SIM_MAIN_2023_112_2_17_22_59_37_4542 "SIM%d::rise the working voltage to 3v."
#define SIM_MAIN_2051_112_2_17_22_59_37_4543 "SIM%d::the current working voltage is matched with the card supply voltage."
#define SIM_MAIN_2060_112_2_17_22_59_37_4544 "\n\rSIM%d::SIM prepare sim card present failed."
#define SIM_MAIN_2074_112_2_17_22_59_37_4545 "\n\rSIM%d::SIM Inserted OK!"
#define SIM_MAIN_2112_112_2_17_22_59_37_4546 "SIM%d: class change %d."
#define SIM_MAIN_2171_112_2_17_22_59_37_4547 "\n\rSIM%d::Wrong SIM inserted!"
#define SIM_MAIN_2237_112_2_17_22_59_37_4548 "SIM%d::ATRState disable deepsleep."
#define SIM_MAIN_2290_112_2_17_22_59_37_4549 "SIM::ATR state received abnormal signal:signal_code =0x%x,sender =0x%x\n"
#define SIM_MAIN_2333_112_2_17_22_59_37_4550 "SIM%d::InitilaizeState disable deepsleep."
#define SIM_MAIN_2521_112_2_17_22_59_38_4551 "SIM::Initilaize state received abnormal signal:signal_code =0x%x,sender =0x%x\n"
#define SIM_MAIN_3492_112_2_17_22_59_40_4552 "SIM::Active state received abnormal signal:signal_code =0x%x,sender =0x%x\n"
#define SIM_MAIN_3514_112_2_17_22_59_40_4553 "SIM%d::ActiveState enable deepsleep."
#define SIM_MAIN_3643_112_2_17_22_59_40_4554 "SIM::Wrong SIM Interrupt Type %d"
#define SIM_MAIN_3700_112_2_17_22_59_40_4555 "SIM%d::card unresponse before initialization."
#define SIM_MAIN_3707_112_2_17_22_59_40_4556 "SIM%d::sim restore count is out of the range."
#define SIM_MAIN_3748_112_2_17_22_59_40_4557 "SIM%d::begin to restore the sim card."
#define SIM_MAIN_3758_112_2_17_22_59_40_4558 "SIM%d::the card is present."
#define SIM_MAIN_3803_112_2_17_22_59_40_4559 "SIM%d:restore fail1. %d"
#define SIM_MAIN_3828_112_2_17_22_59_40_4560 "SIM%d:restore fail2. %d"
#define SIM_MAIN_3836_112_2_17_22_59_40_4561 "SIM%d::the card is not present."
#define SIM_MAIN_3848_112_2_17_22_59_40_4562 "SIM%d:restore failed T.O. %d"
#define SIM_MAIN_3856_112_2_17_22_59_41_4563 "SIM%d::the card no response when restore timeout."
#define SIM_MAIN_3862_112_2_17_22_59_41_4564 "SIM%d::receive signal 0x%x during restore."
#define SIM_MAIN_3956_112_2_17_22_59_41_4565 "SIM%d:cardout %d"
#define SIM_MAIN_4000_112_2_17_22_59_41_4566 "SIM%d:RX Parity Error"
#define SIM_MAIN_4041_112_2_17_22_59_41_4567 "SIM%d:TX Parity Error"
#define SIM_MAIN_4252_112_2_17_22_59_41_4568 "SIM%d::ME is in call,begin to check SIM card every %d ms."
#define SIM_MAIN_4303_112_2_17_22_59_41_4569 "SIM%d::check card present."
#define SIM_MAIN_4317_112_2_17_22_59_41_4570 "SIM%d::Card not present because the current directory is 0x%x!"
#define SIM_MAIN_4346_112_2_17_22_59_41_4571 "SIM%d::CheckCardPresent,Card not present because the Status instruction is not ok\n"
#define SIM_MAIN_4372_112_2_17_22_59_42_4572 "SIM%d:Card unresponse!"
#define SIM_MAIN_4402_112_2_17_22_59_42_4573 "SIM%d:Card give no any response during get the gsm data in 3s!"
#define SIM_MAIN_4406_112_2_17_22_59_42_4574 "SIM%d:Card has give part response,wait until 100s timer expired!"
#define SIM_MAIN_4415_112_2_17_22_59_42_4575 "SIM:Card give no any response during get the gsm data in 3s!"
#define SIM_MAIN_4592_112_2_17_22_59_42_4576 "SIM_IsCallControlSupport result = %d"
#define SIM_MAIN_4853_112_2_17_22_59_42_4577 "SIM%d: this is a USIM. AID len%d, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
#define SIM_MAIN_4861_112_2_17_22_59_42_4578 "SIM%d: this is a USIM card"
#define SIM_MAIN_4866_112_2_17_22_59_42_4579 "SIM%d: this is a SIM card"
#define SIM_MAIN_6567_112_2_17_22_59_46_4580 "SIM::mutex operate error."
#define SIM_MAIN_6572_112_2_17_22_59_46_4581 "SIM::task %d take the sem."
#define SIM_MAIN_6639_112_2_17_22_59_46_4582 "SIM::task %d put the sem."
#define SIM_MAIN_6642_112_2_17_22_59_46_4583 "SIM::task %d put the sem."
#define SIM_MAIN_6651_112_2_17_22_59_46_4584 "SIM::task %d put the sem."
#define SIM_MAIN_6673_112_2_17_22_59_46_4585 "SIM%d:MAIN_Quadsim_CheckDetect %d."
#define SIM_MM_SIGNAL_410_112_2_17_22_59_47_4586 "SIM%d::Read KcGPRS from NV temply."
#define SIM_MM_SIGNAL_422_112_2_17_22_59_47_4587 "SIM::Read KcGPRS from NV."
#define SIM_MM_SIGNAL_446_112_2_17_22_59_47_4588 "SIM::Read LociGPRS from NV."
#define SIM_MM_SIGNAL_467_112_2_17_22_59_47_4589 "SIM:no valid rai using lai !"
#define SIM_MM_SIGNAL_479_112_2_17_22_59_47_4590 "SIM:read KcGPRS and LociGPRS infor from NV!"
#define SIM_MM_SIGNAL_564_112_2_17_22_59_48_4591 "SIM::begin to re-select the file"
#define SIM_MM_SIGNAL_575_112_2_17_22_59_48_4592 "SIM::select file failed before mm auth. %d"
#define SIM_MM_SIGNAL_620_112_2_17_22_59_48_4593 "SIM::Run gsm algorithm failed in mm auth. %d"
#define SIM_MM_SIGNAL_651_112_2_17_22_59_48_4594 "SIM::update kc failed in mm auth."
#define SIM_MM_SIGNAL_820_112_2_17_22_59_48_4595 "SIM::select kc file failed in Gmm auth."
#define SIM_MM_SIGNAL_865_112_2_17_22_59_48_4596 "SIM::Run gsm algorithm failed in Gmm auth."
#define SIM_MM_SIGNAL_898_112_2_17_22_59_48_4597 "SIM::Update Kcgprs of SIM card failed in Gmm auth for result 0x%x."
#define SIM_MM_SIGNAL_928_112_2_17_22_59_48_4598 "SIM::Update Kcgprs of NV failed in Gmm auth."
#define SIM_MM_SIGNAL_973_112_2_17_22_59_48_4599 "SIM::Save RAI information result is:0x%x."
#define SIM_MM_SIGNAL_993_112_2_17_22_59_48_4600 "SIM::Save RAI failed for result is:0x%x."
#define SIM_MM_SIGNAL_1036_112_2_17_22_59_48_4601 "SIM::Save Cipher information result is:0x%x."
#define SIM_SER_PROCEDURE_148_112_2_17_22_59_49_4602 "SIM: re-do increase"
#define SIM_SER_PROCEDURE_195_112_2_17_22_59_49_4603 "SIM::SIMSER_ReadWholeTransparentEF,ERROR."
#define SIM_SER_PROCEDURE_239_112_2_17_22_59_49_4604 "SIM: re-do read binary"
#define SIM_SER_PROCEDURE_282_112_2_17_22_59_49_4605 "SIM::SIMSER_ReadEFRecord,ERROR."
#define SIM_SER_PROCEDURE_328_112_2_17_22_59_50_4606 "SIM: re-do ef record"
#define SIM_SER_PROCEDURE_372_112_2_17_22_59_50_4607 "SIM::SIMSER_SeekInFile,ERROR."
#define SIM_SER_PROCEDURE_431_112_2_17_22_59_50_4608 "SIM: re-do seek ef record"
#define SIM_SER_PROCEDURE_469_112_2_17_22_59_50_4609 "SIM::SIMSER_UpdateWholeTransparentEF,ERROR."
#define SIM_SER_PROCEDURE_521_112_2_17_22_59_50_4610 "SIM: re-do update t ef record"
#define SIM_SER_PROCEDURE_565_112_2_17_22_59_50_4611 "SIM::SIMSER_UpdateEFRecord,ERROR."
#define SIM_SER_PROCEDURE_760_112_2_17_22_59_50_4612 "SIM::SIMSER_GetRecordInfo,ERROR."
#define SIM_SER_PROCEDURE_891_112_2_17_22_59_51_4613 "SIM::SIMSER_ReadWholeTransparentEF_static,ERROR."
#define SIM_SER_PROCEDURE_941_112_2_17_22_59_51_4614 "SIM::SIMSER_ReadEFRecord_static,ERROR."
#define SIM_SER_PROCEDURE_990_112_2_17_22_59_51_4615 "SIM::SIMSER_UpdateWholeTransparentEF_static,ERROR."
#define SIM_SER_PROCEDURE_1039_112_2_17_22_59_51_4616 "SIM::SIMSER_UpdateEFRecord_static,ERROR."
#define SIM_SIGNAL_994_112_2_17_22_59_54_4617 "SIM: Dphone refresh timeout!"
#define SIM_SIGNAL_2512_112_2_17_22_59_57_4618 "SIM:: select DFcdma ser_result = %d"
#define SIM_SIGNAL_2538_112_2_17_22_59_57_4619 "SIM:: select DFgsm ser_result = %d"
#define SIM_SIGNAL_2568_112_2_17_22_59_57_4620 "SIM::select imsi failed."
#define SIM_SIGNAL_2586_112_2_17_22_59_57_4621 "SIM:: EFimsi ser_result = %d, msin = %d, %d, %d"
#define SIM_SIGNAL_2602_112_2_17_22_59_57_4622 "SIM::the imsi file is invalidate."
#define SIM_SIGNAL_2671_112_2_17_22_59_57_4623 "SIM: g_sim_card_state = %d"
#define SIM_SIGNAL_2692_112_2_17_22_59_57_4624 "SIM:: select DFcdma ser_result = %d"
#define SIM_SIGNAL_2719_112_2_17_22_59_57_4625 "SIM::no EFimsi in the DFcdma."
#define SIM_SIGNAL_2726_112_2_17_22_59_57_4626 "SIM::select EFimsi in the DFcdma failed."
#define SIM_SIGNAL_2744_112_2_17_22_59_57_4627 "SIM::read cdma imsi failed."
#define SIM_SIGNAL_2751_112_2_17_22_59_57_4628 "SIM::read cdma imsi OK."
#define SIM_SIGNAL_2760_112_2_17_22_59_57_4629 "SIM::the EFimsi of the DFcdma is invalidate."
#define SIM_SIGNAL_2810_112_2_17_22_59_57_4630 "SIM::Verify CHV1 failed in SIM initialization!"
#define SIM_SIGNAL_2876_112_2_17_22_59_57_4631 "SIM::SIM initialization success!"
#define SIM_SIGNAL_2886_112_2_17_22_59_57_4632 "SIM::ss imsi is full 0xff during the initialization. len=%d"
#define SIM_SIGNAL_2895_112_2_17_22_59_57_4633 "SIM::imsi is full 0xff during the initialization."
#define SIM_SIGNAL_2959_112_2_17_22_59_58_4634 "SIM::GetAOCAccessCondition filename = %d, condition = %x"
#define SIM_SIGNAL_3078_112_2_17_22_59_58_4635 "SIM::Read EFecc failed in Card Presented! %d"
#define SIM_SIGNAL_3104_112_2_17_22_59_58_4636 "SIM::Read EFphase failed in Card Presented!"
#define SIM_SIGNAL_3123_112_2_17_22_59_58_4637 "SIM::Read EFad failed in Card Presented!%d"
#define SIM_SIGNAL_3197_112_2_17_22_59_58_4638 "SIM::Read EFiccid failed in SIM initialization!"
#define SIM_SIGNAL_3508_112_2_17_22_59_59_4639 "SIM::SIMSIGNAL_CHVRequest,ERROR!"
#define SIM_SIGNAL_3541_112_2_17_22_59_59_4640 "SIM::VerifyCHV1AtInit,ERROR."
#define SIM_SIGNAL_3689_112_2_17_22_59_59_4641 "SIM::read loci failed when init."
#define SIM_SIGNAL_3758_112_2_17_23_0_0_4642 "SIM::gid1 not support."
#define SIM_SIGNAL_3803_112_2_17_23_0_0_4643 "SIM::gid2 not support."
#define SIM_SIGNAL_4242_112_2_17_23_0_0_4644 "SIM:get img record info."
#define SIM_SIGNAL_4247_112_2_17_23_0_0_4645 "SIM: img service support."
#define SIM_SIGNAL_4253_112_2_17_23_0_0_4646 "SIM: img service not support."
#define SIM_SIGNAL_4486_112_2_17_23_0_1_4647 "SIM::Receive SIM_PC_EXIST_IND signal when card not ready!"
#define SIM_SIGNAL_4491_112_2_17_23_0_1_4648 "SIM::Receive SIM_PC_EXIST_IND signal when card not support STK!"
#define SIM_SIGNAL_4518_112_2_17_23_0_1_4649 "SIM::Receive terminal response request when card not ready!"
#define SIM_SIGNAL_4527_112_2_17_23_0_1_4650 "SIM::Receive terminal response request when card not support STK!"
#define SIM_SIGNAL_4535_112_2_17_23_0_1_4651 "SIM::Terminal response failed!"
#define SIM_SIGNAL_4570_112_2_17_23_0_1_4652 "SIM::Envelope failed!"
#define SIM_SIGNAL_4665_112_2_17_23_0_1_4653 "SIM::SIMSIGNAL_ReadFileBufByPath input parameter error!"
#define SIM_SIGNAL_4676_112_2_17_23_0_1_4654 "SIM:: select MF ser_result = %d"
#define SIM_SIGNAL_4694_112_2_17_23_0_1_4655 "SIM::SIMSIGNAL_ReadFileBufByPath,select 0x%x failed."
#define SIM_SIGNAL_4708_112_2_17_23_0_1_4656 "SIM::SIMSIGNAL_ReadFileBufByPath,reback to MF failed."
#define SIM_SIGNAL_4714_112_2_17_23_0_1_4657 "SIM::SIMSIGNAL_ReadFileBufByPath,select 0x%x ok."
#define SIM_SIGNAL_4740_112_2_17_23_0_1_4658 "SIM:: SIMINSTR_ImiReadEF, read 0x%x ser_result = %d"
#define SIM_SIGNAL_4758_112_2_17_23_0_2_4659 "SIM:: SIMSIGNAL_ReadFileBufByPath, reback to MF ser_result = %d"
#define SIM_SIGNAL_4761_112_2_17_23_0_2_4660 "SIM::SIMSIGNAL_ReadFileBufByPath,reback to MF failed2."
#define SIM_SIGNAL_4819_112_2_17_23_0_2_4661 "SIM::Verify CHV1 failed in SIM ReInitialization!"
#define SIM_SIGNAL_4830_112_2_17_23_0_2_4662 "SIM::Read EFsst Failed in SIM ReInitialization!"
#define SIM_SIGNAL_4856_112_2_17_23_0_2_4663 "SIM::STK terminal profile fail %d"
#define SIM_SIGNAL_4860_112_2_17_23_0_2_4664 "SIM%d::SIM ReInitialization success!"
#define SIM_SIGNAL_4884_112_2_17_23_0_2_4665 "SIM:IsFatalError TRUE"
#define SIM_SIGNAL_4960_112_2_17_23_0_2_4666 "SIM:: Init_GetSimFileTable Read EFsst Failed %d"
#define SIM_SIGNAL_4982_112_2_17_23_0_2_4667 "SIM::SIM Application Toolkit terminal profile fail %d"
#define SIM_SIGNAL_5016_112_2_17_23_0_2_4668 "SIM:: Init_GetSimFileTable Read EFimsi failed %d"
#define SIM_SIGNAL_5041_112_2_17_23_0_2_4669 "SIM:: Init_GetSimFileTable Read EFacc failed %d"
#define SIM_SIGNAL_5054_112_2_17_23_0_2_4670 "SIM:: Init_GetSimFileTable Read EFhplmn failed %d"
#define SIM_SIGNAL_5074_112_2_17_23_0_2_4671 "SIM:: Init_GetSimFileTable Read EFplmnwact failed %d"
#define SIM_SIGNAL_5107_112_2_17_23_0_2_4672 "SIM:: Init_GetSimFileTable Read EFplmnsel failed %d"
#define SIM_SIGNAL_5136_112_2_17_23_0_2_4673 "SIM:: Init_GetSimFileTable Read EFoplmnwact failed %d"
#define SIM_SIGNAL_5163_112_2_17_23_0_2_4674 "SIM:: Init_GetSimFileTable Read EFhplmnwact failed %d"
#define SIM_SIGNAL_5186_112_2_17_23_0_2_4675 "SIM:: Init_GetSimFileTable Read EFloci failed %d"
#define SIM_SIGNAL_5203_112_2_17_23_0_2_4676 "SIM:: Init_GetSimFileTable Read EFkc failed %d"
#define SIM_SIGNAL_5222_112_2_17_23_0_2_4677 "SIM:: Init_GetSimFileTable EFbcch failed %d"
#define SIM_SIGNAL_5246_112_2_17_23_0_2_4678 "SIM::Init_GetSimFileTable Read EFfplmn failed %d"
#define SIM_SIGNAL_5311_112_2_17_23_0_3_4679 "SIM:: Init_GetSimFileTable EFspn failed %d"
#define SIM_SIGNAL_5349_112_2_17_23_0_3_4680 "SIM:: Init_GetSimFileTable EFgid1 failed %d"
#define SIM_SIGNAL_5375_112_2_17_23_0_3_4681 "SIM:: Init_GetSimFileTable EFgid2 failed %d"
#define SIM_SIGNAL_5422_112_2_17_23_0_3_4682 "SIM:: Read EFehplmn cause = %d"
#define SIM_SIGNAL_5431_112_2_17_23_0_3_4683 "SIM:: Read EFehplmnpi cause = %d"
#define SIM_SIGNAL_5458_112_2_17_23_0_3_4684 "SIM:: CMCC special HPLMN"
#define SIM_SIGNAL_5487_112_2_17_23_0_3_4685 "SIM::Refresh Ini begin!"
#define SIM_SIGNAL_5498_112_2_17_23_0_3_4686 "SIM::SIMSIGNAL_RefreshIni Read EFiccid failed!"
#define SIM_SIGNAL_5506_112_2_17_23_0_3_4687 "SIM::SIMSIGNAL_RefreshIni select DFgsm failed!"
#define SIM_SIGNAL_5521_112_2_17_23_0_3_4688 "SIM::Read EFphase failed in Refresh Ini!"
#define SIM_SIGNAL_5544_112_2_17_23_0_3_4689 "SIM::SIMSIGNAL_RefreshIni Read EFad failed!"
#define SIM_SIGNAL_5558_112_2_17_23_0_3_4690 "SIM::SIM Refresh ini success!FDN %d"
#define SIM_SIGNAL_5567_112_2_17_23_0_3_4691 "SIM::SIMSIGNAL_RefreshIni imsi is full 0xff."
#define SIM_SIGNAL_5597_112_2_17_23_0_3_4692 "SIM::Refresh File change begin!"
#define SIM_SIGNAL_5643_112_2_17_23_0_3_4693 "SIM::SIMSIGNAL_RefreshFileChg select MF failed."
#define SIM_SIGNAL_5648_112_2_17_23_0_3_4694 "SIM::Refresh File change end!"
#define SIM_SIGNAL_5680_112_2_17_23_0_3_4695 "SIM::Refresh Ini File change begin!"
#define SIM_SIGNAL_5692_112_2_17_23_0_3_4696 "SIM::SIMSIGNAL_RefreshIniFileChg Read EFiccid failed!"
#define SIM_SIGNAL_5700_112_2_17_23_0_3_4697 "SIM::SIMSIGNAL_RefreshIniFileChg select DFgsm failed!"
#define SIM_SIGNAL_5715_112_2_17_23_0_3_4698 "SIM::Read EFphase failed in Refresh Ini!"
#define SIM_SIGNAL_5738_112_2_17_23_0_3_4699 "SIM::Read EFad failed in SIM initialization!"
#define SIM_SIGNAL_5752_112_2_17_23_0_4_4700 "SIM::SIM ini file change success!FDN %d"
#define SIM_SIGNAL_5761_112_2_17_23_0_4_4701 "SIM::SIMSIGNAL_RefreshIniFileChg imsi is full 0xff."
#define SIM_SIGNAL_5831_112_2_17_23_0_4_4702 "SIM::Refresh Ini Full begin!"
#define SIM_SIGNAL_5842_112_2_17_23_0_4_4703 "SIM::SIMSIGNAL_RefreshIniFull Read EFiccid failed!"
#define SIM_SIGNAL_5849_112_2_17_23_0_4_4704 "SIM::SIMSIGNAL_RefreshIniFull select DFgsm failed!"
#define SIM_SIGNAL_5864_112_2_17_23_0_4_4705 "SIM::Read EFphase failed in Refresh Ini!"
#define SIM_SIGNAL_5887_112_2_17_23_0_4_4706 "SIM::Read EFad failed in SIM initialization!"
#define SIM_SIGNAL_5902_112_2_17_23_0_4_4707 "SIM::SIM Refresh Ini Full success!FDN %d"
#define SIM_SIGNAL_5911_112_2_17_23_0_4_4708 "SIM::SIMSIGNAL_RefreshIniFull imsi is full 0xff."
#define SIM_SIGNAL_5941_112_2_17_23_0_4_4709 "SIM::Refresh read File!"
#define SIM_SIGNAL_6051_112_2_17_23_0_4_4710 "SIM%d:acm value = 0x%x"
#define SIM_SIGNAL_6073_112_2_17_23_0_4_4711 "SIM%d::acm max is %x."
#define SIM_SIGNAL_6560_112_2_17_23_0_5_4712 "SIM::Envelope failed!"
#define SIM_SIGNAL_6608_112_2_17_23_0_5_4713 "SIM::the img len is error!"
#define SIM_SIGNAL_6644_112_2_17_23_0_5_4714 "SIM::the bits per img point is %d,out of range."
#define SIM_SIGNAL_6737_112_2_17_23_0_5_4715 "SIM:begin to read the %d img record."
#define SIM_SIGNAL_6751_112_2_17_23_0_5_4716 "SIM:read img failed."
#define SIM_SIGNAL_6778_112_2_17_23_0_6_4717 "SIM::SIMSIGNAL_ReadImg,select 0x%x failed."
#define SIM_SIGNAL_6793_112_2_17_23_0_6_4718 "SIM::SIMSIGNAL_ReadImg,reback to MF failed."
#define SIM_SIGNAL_6799_112_2_17_23_0_6_4719 "SIM::SIMSIGNAL_ReadImg,select 0x%x ok."
#define SIM_SIGNAL_6827_112_2_17_23_0_6_4720 "SIM:: ImiReadEF, read img 0x%x ser_result = %d"
#define SIM_SIGNAL_6842_112_2_17_23_0_6_4721 "SIM:decode  img instance data file failed."
#define SIM_SIGNAL_6849_112_2_17_23_0_6_4722 "SIM:decode  img instance data file ok."
#define SIM_SIGNAL_6851_112_2_17_23_0_6_4723 "SIM:img width is %d,height is %d."
#define SIM_SIGNAL_6862_112_2_17_23_0_6_4724 "SIM:: SIMSIGNAL_ReadFileBufByPath, reback to MF ser_result = %d"
#define SIM_SIGNAL_6866_112_2_17_23_0_6_4725 "SIM: !!SIMSIGNAL_ReadFileBufByPath, reback to MF Fail"
#define SIM_SIGNAL_6886_112_2_17_23_0_6_4726 "SIM:decode  img instance data file failed."
#define SIM_SIGNAL_6893_112_2_17_23_0_6_4727 "SIM:decode  img instance data file ok."
#define SIM_SIGNAL_6895_112_2_17_23_0_6_4728 "SIM:img width is %d,height is %d."
#define SIM_SIGNAL_6902_112_2_17_23_0_6_4729 "SIM::img service not support."
#define SIM_SMS_174_112_2_17_23_0_6_4730 "SIMSMS: g_sim_card_setting.sms_info.valide_sms_num is %d"
#define SIM_TEST_DRIVER_174_112_2_17_23_0_9_4731 "Rx_Buf : %s"
#define SIM_TEST_DRIVER_220_112_2_17_23_0_9_4732 "SIM::SIM card inserted ok!"
#define SIM_TEST_DRIVER_225_112_2_17_23_0_9_4733 "SIM::SIM removed1!"
#define SIM_TEST_DRIVER_235_112_2_17_23_0_9_4734 "SIM::SIM inserted!"
#define SIM_TEST_DRIVER_240_112_2_17_23_0_9_4735 "SIM::SIM Removed!"
#define SIM_TEST_DRIVER_245_112_2_17_23_0_9_4736 "SIM::SIM error1!"
#define SIM_TEST_DRIVER_250_112_2_17_23_0_9_4737 "SIM::SIM error2!"
#define SIM_TEST_DRIVER_255_112_2_17_23_0_9_4738 "SIM::SIM error3!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(BASE_TRC)
TRACE_MSG(ATC_330_112_2_17_22_16_18_0,"ATC: write nv fail")
TRACE_MSG(ATC_379_112_2_17_22_16_18_1,"ATC:read atc_config_nv fail")
TRACE_MSG(ATC_386_112_2_17_22_16_18_2,"ATC:write NV_ATC_CONFIG_ID fail")
TRACE_MSG(ATC_396_112_2_17_22_16_18_3,"ATC:read midi_size_arr fail")
TRACE_MSG(ATC_403_112_2_17_22_16_18_4,"ATC:write NV_ATC_MIDI_SIZE_ID fail")
TRACE_MSG(ATC_419_112_2_17_22_16_18_5,"ATC: ATC_Mux_Setup_Link, WARNING! link_id:%d")
TRACE_MSG(ATC_424_112_2_17_22_16_18_6,"ATC: ATC_Mux_Setup_Link, WARNING! link_id:%d is already setup")
TRACE_MSG(ATC_438_112_2_17_22_16_18_7,"ATC: ATC_Mux_Setup_Link, ERROR! no mem to setup link_id:%d")
TRACE_MSG(ATC_447_112_2_17_22_16_18_8,"ATC: ATC_Mux_Setup_Link, link_id: %d")
TRACE_MSG(ATC_462_112_2_17_22_16_18_9,"ATC: ATC_Mux_Release_Link, link_id: %d")
TRACE_MSG(ATC_500_112_2_17_22_16_18_10,"ATC: ATC_Mux_Release_Link ERROR!!!!")
TRACE_MSG(ATC_529_112_2_17_22_16_18_11,"ATC SET link id to %d, old %d")
TRACE_MSG(ATC_574_112_2_17_22_16_18_12,"ATC: ATC_List_Expected_Event as follows:")
TRACE_MSG(ATC_588_112_2_17_22_16_18_13,"|---|-------|-----|-------|------------------------------")
TRACE_MSG(ATC_589_112_2_17_22_16_18_14,"|Idx|Link-ID| CID |Call-ID| Event")
TRACE_MSG(ATC_590_112_2_17_22_16_18_15,"|---|-------|-----|-------|------------------------------")
TRACE_MSG(ATC_602_112_2_17_22_16_19_16,"|%-3d|   %d   | %-3d |  %-3d  | %s")
TRACE_MSG(ATC_610_112_2_17_22_16_19_17,"|%-3d|   %d   | %-3d |  %-3d  | %d")
TRACE_MSG(ATC_623_112_2_17_22_16_19_18,"|---|-------|-----|-------|------------------------------")
TRACE_MSG(ATC_626_112_2_17_22_16_19_19,"Total:%d")
TRACE_MSG(ATC_657_112_2_17_22_16_19_20,"ATC: ATC_Clear_All_Exp_Result id:%d,domain_id:%d")
TRACE_MSG(ATC_674_112_2_17_22_16_19_21,"ATC: ATC_Clear_All_Exp_Result, cid:%d, exp event: %s")
TRACE_MSG(ATC_679_112_2_17_22_16_19_22,"ATC: ATC_Clear_All_Exp_Result, cid:%d, exp event: %d")
TRACE_MSG(ATC_718_112_2_17_22_16_19_23,"ATC: ATC_Clear_All_Exp_Result, call_id:%d, exp event: %d")
TRACE_MSG(ATC_965_112_2_17_22_16_19_24,"ATC: recv default event %x")
TRACE_MSG(ATC_983_112_2_17_22_16_19_25,"ATC: recv event %x, not found in expected list")
TRACE_MSG(ATC_1001_112_2_17_22_16_19_26,"ATC: ATC_Receive_Expected_Event, id:%d,event:%s")
TRACE_MSG(ATC_1005_112_2_17_22_16_19_27,"ATC: ATC_Receive_Expected_Event, id:%d,event:%d")
TRACE_MSG(ATC_1012_112_2_17_22_16_19_28,"ATC: ATC_Receive_Expected_Event, WARNING! NOT FOUND, set link id: %d")
TRACE_MSG(ATC_1017_112_2_17_22_16_19_29,"ATC: ATC_Receive_Expected_Event, WARNING! NOT FOUND, set urc link id: %d")
TRACE_MSG(ATC_1032_112_2_17_22_16_19_30,"ATC: ATC_Receive_Expected_Event, FOUND, ERROR! match_link_id:%d != call_link_id:%d")
TRACE_MSG(ATC_1037_112_2_17_22_16_19_31,"ATC: ATC_Receive_Expected_Event, FOUND, WARNING! match_link_id:%d, no call_link_id, NOT ADD EARLIER?")
TRACE_MSG(ATC_1045_112_2_17_22_16_19_32,"ATC: ATC_Receive_Expected_Event, FOUND, match_link_id:%d, call_link_id:%d")
TRACE_MSG(ATC_1050_112_2_17_22_16_19_33,"ATC: ATC_Receive_Expected_Event, FOUND, match_link_id:%d")
TRACE_MSG(ATC_1085_112_2_17_22_16_19_34,"ATC: ATC_Add_Expected_Event, link_id:%d, event:%s")
TRACE_MSG(ATC_1089_112_2_17_22_16_19_35,"ATC: ATC_Add_Expected_Event, link_id:%d, event:%d")
TRACE_MSG(ATC_1114_112_2_17_22_16_20_36,"ATC: ATC_Add_Expected_Event, unknown domain_id: %d")
TRACE_MSG(ATC_1126_112_2_17_22_16_20_37,"ATC: ATC_Add_Expected_Event,Error :The atc response buffer is full ,clear first message,pls check")
TRACE_MSG(ATC_1346_112_2_17_22_16_20_38,"ATC: ATC_ReadCmdLine Warning: only read %d < %d.")
TRACE_MSG(ATC_1350_112_2_17_22_16_20_39,"ATC: ATC_ReadCmdLine Warning: garbage data ???")
TRACE_MSG(ATC_1421_112_2_17_22_16_20_40,"ATC: ATC_SendNewATInd,link_id:%d,data_length:%d,data %x, %x, %x, %x")
TRACE_MSG(ATC_1449_112_2_17_22_16_20_41,"ATC: ATC_SendNewATInd, is_close_atc_queue is TRUE, return")
TRACE_MSG(ATC_1455_112_2_17_22_16_20_42,"ATC: ATC_SendNewATInd, link is not setup!!")
TRACE_MSG(ATC_1461_112_2_17_22_16_21_43,"ATC: ATC_SendNewATInd, atc_is_echo is TRUE, send copy back")
TRACE_MSG(ATC_1472_112_2_17_22_16_21_44,"ATC: ERROR AT BUFFER FULL  LINK %d ")
TRACE_MSG(ATC_1510_112_2_17_22_16_21_45,"ATC: ATC_Debug_SendATInd, len:%d")
TRACE_MSG(ATC_1533_112_2_17_22_16_21_46,"ATC: ATC_Debug_SendATInd, is_close_atc_queue is TRUE, return")
TRACE_MSG(ATC_1538_112_2_17_22_16_21_47,"temp_line_length %d")
TRACE_MSG(ATC_1552_112_2_17_22_16_21_48,"ATC: ERROR AT BUFFER FULL  LINK ATC_DEBUG_LINK ")
TRACE_MSG(ATC_1579_112_2_17_22_16_21_49,"ATC: CAL POST MODE, return ATC_DEBUG_LINK")
TRACE_MSG(ATC_1585_112_2_17_22_16_21_50,"ATC_GetDualSimMuxLinkId return ctl_link %d")
TRACE_MSG(ATC_1750_112_2_17_22_16_21_51,"ATC: ATC_Task, Signal Code: ATC_MUX_LINK_SETUP_REQ, link_id:%d")
TRACE_MSG(ATC_1757_112_2_17_22_16_21_52,"ATC: ATC_Task, Signal Code: ATC_MUX_LINK_RELEASE_REQ, link_id:%d")
TRACE_MSG(ATC_1764_112_2_17_22_16_21_53,"ATC: ATC_Task, Signal Code: ATC_MUX_CLOSE_REQ")
TRACE_MSG(ATC_1771_112_2_17_22_16_21_54,"ATC: ATC_Task, Signal Code: ATC_MUX_RECV_NEW_AT, link_id:%d,len:%d")
TRACE_MSG(ATC_1782_112_2_17_22_16_21_55,"ATC: 2 atc_global_info config link %d")
TRACE_MSG(ATC_1793_112_2_17_22_16_21_56,"ATC: atc_task, atc_global_info config link %d")
TRACE_MSG(ATC_1860_112_2_17_22_16_22_57,"atc: network deactivate PDP")
TRACE_MSG(ATC_1869_112_2_17_22_16_22_58,"atc: receive pdp activate message from network")
TRACE_MSG(ATC_1898_112_2_17_22_16_22_59,"atc: user deactivate PDP ")
TRACE_MSG(ATC_1913_112_2_17_22_16_22_60,"ATC: Switch to AT MODE for PCLink!")
TRACE_MSG(ATC_1927_112_2_17_22_16_22_61,"case APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,is deactivate AT pid,pid:%d")
TRACE_MSG(ATC_1988_112_2_17_22_16_22_62,"ATC: g_atc_ignore_one_detach_cnf %d")
TRACE_MSG(ATC_2096_112_2_17_22_16_22_63,"ATC: ATC_Task, Signal Code: APP_MN_CALL_DISCONNECTED_IND, set atc_err_protected_flag")
TRACE_MSG(ATC_2577_112_2_17_22_16_23_64,"ATC:APP_MN_ADN_UPDATE_CNF")
TRACE_MSG(ATC_2704_112_2_17_22_16_23_65,"ATC initialize failed.")
TRACE_MSG(ATC_2923_112_2_17_22_16_24_66,"ATC: ATC_RecNewLineSig,link_id:%d,len:%d")
TRACE_MSG(ATC_2984_112_2_17_22_16_24_67,"ATC: backspace? len: %d->%d")
TRACE_MSG(ATC_3004_112_2_17_22_16_24_68,"ATC: ATC_RecNewLineSig, len:%d,hex:%s,line:%s")
TRACE_MSG(ATC_3008_112_2_17_22_16_24_69,"ATC: ATC_RecNewLineSig, len:%d")
TRACE_MSG(ATC_3236_112_2_17_22_16_25_70,"ATC: ATC_RecNewLineSig, ignore ip data, len:%d->%d, new atcmd:%s")
TRACE_MSG(ATC_3257_112_2_17_22_16_25_71,"ATC: ATC_RecNewLineSig, Assert Failure sharp len too long.")
TRACE_MSG(ATC_3264_112_2_17_22_16_25_72,"ATC: ATC_RecNewLineSig, Call the external function")
TRACE_MSG(ATC_3300_112_2_17_22_16_25_73,"ATC: ATC_RecNewLineSig, Parse AT cmd line success.")
TRACE_MSG(ATC_3304_112_2_17_22_16_25_74,"ATC: cmd next %x")
TRACE_MSG(ATC_3331_112_2_17_22_16_25_75,"ATC: ATC_RecNewLineSig, Parse SMS PDU successful")
TRACE_MSG(ATC_3337_112_2_17_22_16_25_76,"ATC: ATC_RecNewLineSig, Parse AT command line failed.")
TRACE_MSG(ATC_3363_112_2_17_22_16_25_77,"ATC: prcesscmds link %d")
TRACE_MSG(ATC_3405_112_2_17_22_16_25_78,"ATC: 5 atc_global_info config link %d")
TRACE_MSG(ATC_3530_112_2_17_22_16_25_79,"ATC:RECV AT len = %d,Content = %s")
TRACE_MSG(ATC_3535_112_2_17_22_16_25_80,"ATC:Recv AT len = %d,too long!")
TRACE_MSG(ATC_3616_112_2_17_22_16_25_81,"atc: ATC_CTRLZ_GPRS, status=%d")
TRACE_MSG(ATC_3723_112_2_17_22_16_26_82,"ATC:ASSERT invlaid ctrlz=%d")
TRACE_MSG(ATC_3744_112_2_17_22_16_26_83,"ATC: ATC_RecNewLineSig, ignore ip data, len:%d->%d, new atcmd:%s")
TRACE_MSG(ATC_3775_112_2_17_22_16_26_84,"ATC:Assert Failure sharp too long.")
TRACE_MSG(ATC_3800_112_2_17_22_16_26_85,"ATC:Parse AT cmd OK")
TRACE_MSG(ATC_3826_112_2_17_22_16_26_86,"ATC:Parse SMS PDU successful")
TRACE_MSG(ATC_3832_112_2_17_22_16_26_87,"ATC:Parse AT command line failed.\n")
TRACE_MSG(ATC_3879_112_2_17_22_16_26_88,"ATC:Assert issue for processCMDs!")
TRACE_MSG(ATC_4126_112_2_17_22_16_26_89,"ATC:Check AT Command valid.\n")
TRACE_MSG(ATC_4130_112_2_17_22_16_26_90,"ATC:Check AT Command invalid.\n")
TRACE_MSG(ATC_4158_112_2_17_22_16_27_91,"atc: mux enable link id = %d")
TRACE_MSG(ATC_4230_112_2_17_22_16_27_92,"ATC:assert invlaid cmee type!")
TRACE_MSG(ATC_4249_112_2_17_22_16_27_93,"ATC: BuildResultCodeRsp,The string:%s")
TRACE_MSG(ATC_4311_112_2_17_22_16_27_94,"ATC:assert invalid cmee type!")
TRACE_MSG(ATC_4316_112_2_17_22_16_27_95,"ATC:BuildSmsCodeRsp,The string=%s")
TRACE_MSG(ATC_4356_112_2_17_22_16_27_96,"ATC: ATC_BuildInfoRsp \"%s\"")
TRACE_MSG(ATC_4360_112_2_17_22_16_27_97,"ATC: ATC_BuildInfoRsp \"%c%c%c%c%c%c%c%c%c%c%c%c\"")
TRACE_MSG(ATC_4366_112_2_17_22_16_27_98,"ATC: ATC_BuildInfoRsp debug link %d")
TRACE_MSG(ATC_4370_112_2_17_22_16_27_99,"ATC: ATC_BuildInfoRsp link_id %d, work mode %d")
TRACE_MSG(ATC_4560_112_2_17_22_16_27_100,"ATC:BuildTestCmdResponse,The string=%s")
TRACE_MSG(ATC_5091_112_2_17_22_16_28_101,"ATC:RegisterMsg STK")
TRACE_MSG(ATC_5180_112_2_17_22_16_29_102,"ATC: ATC_DisconnectModem ")
TRACE_MSG(ATC_5207_112_2_17_22_16_29_103,"ATC: ATC_DisconnectModem, no call(cid:%d) link id! PSD is not existed!")
TRACE_MSG(ATC_5212_112_2_17_22_16_29_104,"ATC: ATC_DisconnectModem, failure in MNGPRS_DeactivatePdpContext")
TRACE_MSG(ATC_5236_112_2_17_22_16_29_105,"pid %d is ATC activated")
TRACE_MSG(ATC_5248_112_2_17_22_16_29_106,"ATC:DisconnectModem!")
TRACE_MSG(ATC_5324_112_2_17_22_16_29_107,"ATC: Assert Failure unknow chr")
TRACE_MSG(ATC_5395_112_2_17_22_16_29_108,"ATC:call state error")
TRACE_MSG(ATC_5444_112_2_17_22_16_29_109,"pid %d is activated:%d")
TRACE_MSG(ATC_5458_112_2_17_22_16_29_110,"ATC_ProcessDeactPdpContextInd,manual attach,send detach")
TRACE_MSG(ATC_BASIC_CMD_423_112_2_17_22_16_30_111,"ATC: Assert SMUX,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_585_112_2_17_22_16_31_112,"ATC_ProcessPreferPlmnChangCnf result = %d")
TRACE_MSG(ATC_BASIC_CMD_663_112_2_17_22_16_31_113,"ATC: ReadPreferredPlmnMaxNum max_num = %d")
TRACE_MSG(ATC_BASIC_CMD_715_112_2_17_22_16_31_114,"ConvertPlmn PLMN(%d, %d)")
TRACE_MSG(ATC_BASIC_CMD_812_112_2_17_22_16_31_115,"ATC: Assert Failure unknow creg.")
TRACE_MSG(ATC_BASIC_CMD_903_112_2_17_22_16_31_116,"ATC: Assert Failure unknow creg enum.")
TRACE_MSG(ATC_BASIC_CMD_914_112_2_17_22_16_31_117,"ATC: Assert CREG,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_1004_112_2_17_22_16_31_118,"ATC: Assert S0,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_1042_112_2_17_22_16_32_119,"ATC: Assert CGMI,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_1092_112_2_17_22_16_32_120,"ATC: sarft sign data fail.")
TRACE_MSG(ATC_BASIC_CMD_1115_112_2_17_22_16_32_121,"ATC: Assert CGSN,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_1207_112_2_17_22_16_32_122,"ATC: CSCS TYPE SET")
TRACE_MSG(ATC_BASIC_CMD_1239_112_2_17_22_16_32_123,"ATC: CSCS TYPE SET")
TRACE_MSG(ATC_BASIC_CMD_1247_112_2_17_22_16_32_124,"ATC: Assert CSCS,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_1279_112_2_17_22_16_32_125,"ATC: SPIN param error")
TRACE_MSG(ATC_BASIC_CMD_1301_112_2_17_22_16_32_126,"ATC: Assert Failure pin len error.")
TRACE_MSG(ATC_BASIC_CMD_1323_112_2_17_22_16_32_127,"ATC: Assert Failure pin length error.")
TRACE_MSG(ATC_BASIC_CMD_1363_112_2_17_22_16_32_128,"ATC: ATC_ProcessCPIN, PIN1,failure in MNPHONE_ResponsePin")
TRACE_MSG(ATC_BASIC_CMD_1390_112_2_17_22_16_32_129,"ATC: ATC_ProcessCPIN, PIN2, failure in MNPHONE_ResponsePin")
TRACE_MSG(ATC_BASIC_CMD_1432_112_2_17_22_16_32_130,"ATC: ATC_ProcessCPIN, sim_flag:TRUE, PUK1, failure in MNPHONE_ResponsePin")
TRACE_MSG(ATC_BASIC_CMD_1449_112_2_17_22_16_32_131,"ATC: ATC_ProcessCPIN, sim_flag:TRUE,PUK1, failure in MNPHONE_ResponsePin")
TRACE_MSG(ATC_BASIC_CMD_1492_112_2_17_22_16_32_132,"ATC: ATC_ProcessCPIN, PUK2, failure in MNPHONE_ResponsePin")
TRACE_MSG(ATC_BASIC_CMD_1526_112_2_17_22_16_33_133,"ATC: Assert Failure unknow cpin state %d")
TRACE_MSG(ATC_BASIC_CMD_1539_112_2_17_22_16_33_134,"ATC: Assert Failure cpin state wrong")
TRACE_MSG(ATC_BASIC_CMD_1562_112_2_17_22_16_33_135,"ATC: Invalid parameter value been input")
TRACE_MSG(ATC_BASIC_CMD_1618_112_2_17_22_16_33_136,"ATC: unknow pin type %d.")
TRACE_MSG(ATC_BASIC_CMD_1953_112_2_17_22_16_33_137,"ATC: recv APP_MN_SYNC_IND signal")
TRACE_MSG(ATC_BASIC_CMD_2110_112_2_17_22_16_34_138,"ATC: Assert Failure unknow creg.")
TRACE_MSG(ATC_BASIC_CMD_2171_112_2_17_22_16_34_139,"ATC: Assert Failure unknow plmn status.")
TRACE_MSG(ATC_BASIC_CMD_2228_112_2_17_22_16_34_140,"ATC: Assert single command,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_2347_112_2_17_22_16_34_141,"atc: mux enable")
TRACE_MSG(ATC_BASIC_CMD_2393_112_2_17_22_16_34_142,"ATC: CCLK input string is %s")
TRACE_MSG(ATC_BASIC_CMD_2431_112_2_17_22_16_34_143,"ATC: Assert CCLK,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_2492_112_2_17_22_16_34_144,"ATC: Assert VGR,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_2565_112_2_17_22_16_35_145,"ATC:param_len = %d")
TRACE_MSG(ATC_BASIC_CMD_2712_112_2_17_22_16_35_146,"ATC: CIND value %d.")
TRACE_MSG(ATC_BASIC_CMD_2916_112_2_17_22_16_35_147,"ATC: Set csq true")
TRACE_MSG(ATC_BASIC_CMD_2945_112_2_17_22_16_35_148,"ATC: Assert CCED,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_3079_112_2_17_22_16_36_149,"ATC: Assert CRSL,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_3132_112_2_17_22_16_36_150,"ATC: Assert CMUT,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_3330_112_2_17_22_16_36_151,"ATC: Assert SSAM,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_3362_112_2_17_22_16_36_152,"ATC: SSAP pga_gain= %d, digi_gain= %d")
TRACE_MSG(ATC_BASIC_CMD_3417_112_2_17_22_16_36_153,"ATC: Assert SSAP,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_3460_112_2_17_22_16_36_154,"ATC: ATC_ProcessSADMDSP")
TRACE_MSG(ATC_BASIC_CMD_3481_112_2_17_22_16_36_155,"ATC: mode_index=%d")
TRACE_MSG(ATC_BASIC_CMD_3487_112_2_17_22_16_36_156,"ATC: audmod_devmod_ptr=0x%x, name %s")
TRACE_MSG(ATC_BASIC_CMD_3537_112_2_17_22_16_37_157,"ATC: str_rsp_len=%d ")
TRACE_MSG(ATC_BASIC_CMD_3547_112_2_17_22_16_37_158,"ATC: str_len_rsp=%d ")
TRACE_MSG(ATC_BASIC_CMD_3630_112_2_17_22_16_37_159,"ATC: audio_ctrl_mode=%d")
TRACE_MSG(ATC_BASIC_CMD_3709_112_2_17_22_16_37_160,"ATC: AUDIONVDSP_GetAudioModeName failed mode:%d.")
TRACE_MSG(ATC_BASIC_CMD_3715_112_2_17_22_16_37_161,"ATC: AUDIONVDSP_ReadModeParamFromFlash failed.")
TRACE_MSG(ATC_BASIC_CMD_3738_112_2_17_22_16_37_162,"ATC: Assert SADMDSP,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_3812_112_2_17_22_16_37_163,"ATC: sadm mode_index = %d, dev_number = %d")
TRACE_MSG(ATC_BASIC_CMD_3852_112_2_17_22_16_37_164,"ATC: sadm mode_index = %d, dev_number = %d")
TRACE_MSG(ATC_BASIC_CMD_3873_112_2_17_22_16_37_165,"atc: str_rsp_len=%d ")
TRACE_MSG(ATC_BASIC_CMD_3883_112_2_17_22_16_37_166,"atc: str_len_rsp=%d ")
TRACE_MSG(ATC_BASIC_CMD_3970_112_2_17_22_16_37_167,"ATC: set dev mode, can't get valid mode type, mode name:%s.")
TRACE_MSG(ATC_BASIC_CMD_3978_112_2_17_22_16_37_168,"ATC: set dev mode result = %d")
TRACE_MSG(ATC_BASIC_CMD_3986_112_2_17_22_16_37_169,"ATC: set dev mode result = %d")
TRACE_MSG(ATC_BASIC_CMD_3993_112_2_17_22_16_37_170,"ATC: set aud dev param error.")
TRACE_MSG(ATC_BASIC_CMD_4060_112_2_17_22_16_38_171,"ATC: sadm mode_index = %d, dev_number = %d")
TRACE_MSG(ATC_BASIC_CMD_4082_112_2_17_22_16_38_172,"ATC: PROD_ReadAudioParamFromFlash failed.")
TRACE_MSG(ATC_BASIC_CMD_4122_112_2_17_22_16_38_173,"ATC: sadm mode_index = %d, dev_number = %d")
TRACE_MSG(ATC_BASIC_CMD_4151_112_2_17_22_16_38_174,"ATC: sadm mode_index = %d, dev_number = %d, %d")
TRACE_MSG(ATC_BASIC_CMD_4152_112_2_17_22_16_38_175,"ATC:atc_config_ptr->q_value=%d, 0x%x ")
TRACE_MSG(ATC_BASIC_CMD_4159_112_2_17_22_16_38_176,"AGC_Get_Param failed to get mode name, mode:%d.")
TRACE_MSG(ATC_BASIC_CMD_4164_112_2_17_22_16_38_177,"ATC:mode_name_ptr: %s, 0x%x, 0x%x")
TRACE_MSG(ATC_BASIC_CMD_4167_112_2_17_22_16_38_178,"AGC_Get_Param failed to get mode para.")
TRACE_MSG(ATC_BASIC_CMD_4172_112_2_17_22_16_38_179,"ATC:atc_config_ptr->q_value=%d, 0x%x, 0x%x, 0x%x")
TRACE_MSG(ATC_BASIC_CMD_4175_112_2_17_22_16_38_180,"ATC:atc_config_ptr->q_value=%d, 0x%x, 0x%x, 0x%x")
TRACE_MSG(ATC_BASIC_CMD_4185_112_2_17_22_16_38_181,"ATC:atc_config_ptr->q_value=%d, ")
TRACE_MSG(ATC_BASIC_CMD_4262_112_2_17_22_16_38_182,"ATC: sadm mode_index = %d, dev_number = %d")
TRACE_MSG(ATC_BASIC_CMD_4269_112_2_17_22_16_38_183,"AGC_Get_Param failed to get mode name, mode:%d.")
TRACE_MSG(ATC_BASIC_CMD_4272_112_2_17_22_16_38_184,"AGC_Get_Param to get mode name, mode:%d, %s.")
TRACE_MSG(ATC_BASIC_CMD_4275_112_2_17_22_16_38_185,"ATC: AUDIONVARM_ReadModeParamFromFlash failed.")
TRACE_MSG(ATC_BASIC_CMD_4355_112_2_17_22_16_38_186,"ATC: Assert SADM,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_4379_112_2_17_22_16_38_187,"ATC: ATC_ProcessSPADLVVE")
TRACE_MSG(ATC_BASIC_CMD_4392_112_2_17_22_16_38_188,"ATC: param_1 is wrong")
TRACE_MSG(ATC_BASIC_CMD_4404_112_2_17_22_16_38_189,"ATC: param_2 is wrong")
TRACE_MSG(ATC_BASIC_CMD_4410_112_2_17_22_16_38_190,"ATC: param_2 is wrong")
TRACE_MSG(ATC_BASIC_CMD_4426_112_2_17_22_16_38_191,"ATC: SADLVVE type = %d,mode_index = %d")
TRACE_MSG(ATC_BASIC_CMD_4431_112_2_17_22_16_38_192,"ATC: LVVE_CTRL_PARAM_GetDevModeName failed to get mode name, mode:%d.")
TRACE_MSG(ATC_BASIC_CMD_4446_112_2_17_22_16_38_193,"ATC: SADLVVE type = %d,mode_index = %d, %d")
TRACE_MSG(ATC_BASIC_CMD_4455_112_2_17_22_16_38_194,"LVVE_CTRL_PARAM_GetDevModeName failed to get mode name, mode:%d.")
TRACE_MSG(ATC_BASIC_CMD_4461_112_2_17_22_16_38_195,"ATC:mode_name_ptr: %s, 0x%x")
TRACE_MSG(ATC_BASIC_CMD_4464_112_2_17_22_16_38_196,"LVVE_CTRL_PARAM_GetDevModeParam failed to get mode para.")
TRACE_MSG(ATC_BASIC_CMD_4522_112_2_17_22_16_39_197,"ATC: LVVE type = %d, mode_index = %d")
TRACE_MSG(ATC_BASIC_CMD_4531_112_2_17_22_16_39_198,"ATC: LVVE_CTRL_PARAM_GetDevModeName failed to get mode name, mode:%d.")
TRACE_MSG(ATC_BASIC_CMD_4536_112_2_17_22_16_39_199,"ATC: AGC_Get_Param to get mode name, mode:%d, %s.")
TRACE_MSG(ATC_BASIC_CMD_4539_112_2_17_22_16_39_200,"ATC: LVVE_CTRL_PARAM_ReadModeParamFromFlash failed.")
TRACE_MSG(ATC_BASIC_CMD_4627_112_2_17_22_16_39_201,"ATC: SPLO get fplmn fail!\n")
TRACE_MSG(ATC_BASIC_CMD_4632_112_2_17_22_16_39_202,"ATC: SPLO get fplmn success!\n")
TRACE_MSG(ATC_BASIC_CMD_4652_112_2_17_22_16_39_203,"ATC: SPLO index = %d")
TRACE_MSG(ATC_BASIC_CMD_4659_112_2_17_22_16_39_204,"ATC: SPLO plmn = %d")
TRACE_MSG(ATC_BASIC_CMD_4667_112_2_17_22_16_39_205,"+CMS ERROR: mnc_digit_num can only be 2 or 3\n")
TRACE_MSG(ATC_BASIC_CMD_4673_112_2_17_22_16_39_206,"+CMS ERROR: cannot input string in para3\n")
TRACE_MSG(ATC_BASIC_CMD_4703_112_2_17_22_16_39_207,"+CMS ERROR: set FPLMN list fail\n")
TRACE_MSG(ATC_BASIC_CMD_4716_112_2_17_22_16_39_208,"+CMS ERROR: set FPLMN list fail\n")
TRACE_MSG(ATC_BASIC_CMD_4744_112_2_17_22_16_39_209,"+CMS ERROR: set FPLMN list fail\n")
TRACE_MSG(ATC_BASIC_CMD_4774_112_2_17_22_16_39_210,"ATC: Assert SPLO,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_4848_112_2_17_22_16_39_211,"ATC: siosadm setmode param result = %d")
TRACE_MSG(ATC_BASIC_CMD_4855_112_2_17_22_16_39_212,"ATC: siosadm input data length error.")
TRACE_MSG(ATC_BASIC_CMD_4927_112_2_17_22_16_39_213,"ATC: snvm audio data fail, err = %d.")
TRACE_MSG(ATC_BASIC_CMD_4936_112_2_17_22_16_39_214,"ATC:SioSADMNVData,length:%d,terminater_char:%d,mode index:%d")
TRACE_MSG(ATC_BASIC_CMD_4944_112_2_17_22_16_39_215,"ATC:cancel the snvm audio data!")
TRACE_MSG(ATC_BASIC_CMD_4967_112_2_17_22_16_39_216,"ATC: write snvm audio data successful")
TRACE_MSG(ATC_BASIC_CMD_4972_112_2_17_22_16_39_217,"ATC: write snvm audio data fail, err = %d.")
TRACE_MSG(ATC_BASIC_CMD_4992_112_2_17_22_16_40_218,"ATC:ConvertHexToBin failed.")
TRACE_MSG(ATC_BASIC_CMD_5005_112_2_17_22_16_40_219,"ATC:PROD_WriteAudioParamToFlash failed. result:%d")
TRACE_MSG(ATC_BASIC_CMD_5016_112_2_17_22_16_40_220,"ATC:SioSADMNVData len error.length:%d != %d")
TRACE_MSG(ATC_BASIC_CMD_5023_112_2_17_22_16_40_221,"ATC: snvm audio data format is error.")
TRACE_MSG(ATC_BASIC_CMD_5081_112_2_17_22_16_40_222,"ATC: siosadm for arm, param_len = %d")
TRACE_MSG(ATC_BASIC_CMD_5094_112_2_17_22_16_40_223,"atc:ctrl_z sizeof(LVVE_CTRL_PARAM_NV_T)*2=%d")
TRACE_MSG(ATC_BASIC_CMD_5098_112_2_17_22_16_40_224,"ATC:ATC_ProcessSioSADLVVEData %s, %d")
TRACE_MSG(ATC_BASIC_CMD_5106_112_2_17_22_16_40_225,"atc: lvve_ctrl_param_nv_ptr->name len=%d")
TRACE_MSG(ATC_BASIC_CMD_5109_112_2_17_22_16_40_226,"atc: ->name [%d]=%d ")
TRACE_MSG(ATC_BASIC_CMD_5117_112_2_17_22_16_40_227,"ATC: LVVE_CTRL_PARAM_SetDevModeParam , result = %d")
TRACE_MSG(ATC_BASIC_CMD_5127_112_2_17_22_16_40_228,"ATC: ATC_ProcessSioSADLVVEData, input data length error.")
TRACE_MSG(ATC_BASIC_CMD_5135_112_2_17_22_16_40_229,"Lost the escape character is %s is line %d")
TRACE_MSG(ATC_BASIC_CMD_5187_112_2_17_22_16_40_230,"ATC: siosadm for arm, param_len = %d")
TRACE_MSG(ATC_BASIC_CMD_5197_112_2_17_22_16_40_231,"atc: ctrl_z, sizeof(LVVE_CTRL_PARAM_NV_T)*2=%d")
TRACE_MSG(ATC_BASIC_CMD_5206_112_2_17_22_16_40_232,"ATC:ATC_ProcessSioSADLVVENVData %s, %d")
TRACE_MSG(ATC_BASIC_CMD_5213_112_2_17_22_16_40_233,"ATC: ATC_ProcessSioSADLVVENVData , result = %d")
TRACE_MSG(ATC_BASIC_CMD_5223_112_2_17_22_16_40_234,"ATC: ATC_ProcessSioSADLVVEData, input data length error.")
TRACE_MSG(ATC_BASIC_CMD_5231_112_2_17_22_16_40_235,"Lost the escape character is %s is line %d")
TRACE_MSG(ATC_BASIC_CMD_5370_112_2_17_22_16_40_236,"ATC: Assert ACM,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_5387_112_2_17_22_16_40_237,"ATC:No sim reason = %d")
TRACE_MSG(ATC_BASIC_CMD_5510_112_2_17_22_16_41_238,"ATC:+WIND: 1    ")
TRACE_MSG(ATC_BASIC_CMD_5580_112_2_17_22_16_41_239,"ATC:+WIND: 2    ")
TRACE_MSG(ATC_BASIC_CMD_5798_112_2_17_22_16_41_240,"ATC: Assert Failure unknow creg.")
TRACE_MSG(ATC_BASIC_CMD_5821_112_2_17_22_16_41_241,"ATC: Assert Failure unknow cgreg.")
TRACE_MSG(ATC_BASIC_CMD_5871_112_2_17_22_16_41_242,"ATC: Assert Failure unknow creg.")
TRACE_MSG(ATC_BASIC_CMD_5894_112_2_17_22_16_41_243,"ATC: Assert Failure unknow cgreg.")
TRACE_MSG(ATC_BASIC_CMD_6176_112_2_17_22_16_42_244,"ATC: assert case dcs pcs.")
TRACE_MSG(ATC_BASIC_CMD_6184_112_2_17_22_16_42_245,"ATC: assert case dcs pcs.")
TRACE_MSG(ATC_BASIC_CMD_6202_112_2_17_22_16_42_246,"ATC: unknow band %d")
TRACE_MSG(ATC_BASIC_CMD_6345_112_2_17_22_16_42_247,"ATC: snvm data format is error.")
TRACE_MSG(ATC_BASIC_CMD_6465_112_2_17_22_16_42_248,"ATC:+SRWD,source_address[0,1,2]=%d")
TRACE_MSG(ATC_BASIC_CMD_6478_112_2_17_22_16_43_249,"ATC:+SRWD,source_address[0,1,2]=%x,%x,%x")
TRACE_MSG(ATC_BASIC_CMD_6515_112_2_17_22_16_43_250,"ATC:+SRWD,error in params")
TRACE_MSG(ATC_BASIC_CMD_6525_112_2_17_22_16_43_251,"ATC:Length of string is error!")
TRACE_MSG(ATC_BASIC_CMD_6617_112_2_17_22_16_43_252,"ATC: Assert SIND,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_6661_112_2_17_22_16_43_253,"ATC: Assert SUPS,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_6730_112_2_17_22_16_43_254,"ATC:ATC_ProcessCrsmCnf")
TRACE_MSG(ATC_BASIC_CMD_6733_112_2_17_22_16_43_255,"ATC:ATC_ProcessCrsmCnf,true")
TRACE_MSG(ATC_BASIC_CMD_6748_112_2_17_22_16_43_256,"ATC:ATC_ProcessCrsmCnf,FAIL")
TRACE_MSG(ATC_BASIC_CMD_6914_112_2_17_22_16_43_257,"ATC:ATC_ProcessSCINF")
TRACE_MSG(ATC_BASIC_CMD_6982_112_2_17_22_16_44_258,"ATC: no network")
TRACE_MSG(ATC_BASIC_CMD_7064_112_2_17_22_16_44_259,"ATC: no network")
TRACE_MSG(ATC_BASIC_CMD_7078_112_2_17_22_16_44_260,"ATC: Test Scinf CMD")
TRACE_MSG(ATC_BASIC_CMD_7085_112_2_17_22_16_44_261,"ATC: Assert SCINF,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_7109_112_2_17_22_16_44_262,"ATC: ATC_ProcessSPEQPARA: <n> is invalid.")
TRACE_MSG(ATC_BASIC_CMD_7132_112_2_17_22_16_44_263,"ATC: ATC_ProcessSPEQPARA: <type> or <dest> is invalid.")
TRACE_MSG(ATC_BASIC_CMD_7154_112_2_17_22_16_44_264,"ATC: ATC_ProcessSPEQPARA: Get EQ PARA failed.")
TRACE_MSG(ATC_BASIC_CMD_7176_112_2_17_22_16_44_265,"ATC: ATC_ProcessSPEQPARA: <type> or <dest> or <data> is invalid.")
TRACE_MSG(ATC_BASIC_CMD_7183_112_2_17_22_16_44_266,"ATC: ATC_ProcessSPEQPARA: <data> type is not string.")
TRACE_MSG(ATC_BASIC_CMD_7189_112_2_17_22_16_44_267,"ATC: ATC_ProcessSPEQPARA: <data> length:%d != %d*2.")
TRACE_MSG(ATC_BASIC_CMD_7197_112_2_17_22_16_44_268,"ATC: ATC_ProcessSPEQPARA: ConvertHexToBin failed.")
TRACE_MSG(ATC_BASIC_CMD_7221_112_2_17_22_16_44_269,"ATC: ATC_ProcessSPEQPARA: Set EQ PARA failed.")
TRACE_MSG(ATC_BASIC_CMD_7256_112_2_17_22_16_44_270,"ATC:ATC_ProcessFTIN!")
TRACE_MSG(ATC_BASIC_CMD_7265_112_2_17_22_16_44_271,"ATC: ATC_ProcessSFTIN not string.")
TRACE_MSG(ATC_BASIC_CMD_7317_112_2_17_22_16_44_272,"ATC_ProcessSFTIN: the operation type error!")
TRACE_MSG(ATC_BASIC_CMD_7342_112_2_17_22_16_44_273,"ATC:ATC_ProcessSYTIN!")
TRACE_MSG(ATC_BASIC_CMD_7351_112_2_17_22_16_44_274,"ATC: ATC_ProcessSYTIN not string.")
TRACE_MSG(ATC_BASIC_CMD_7399_112_2_17_22_16_44_275,"ATC_ProcessSYTIN: the operation type error!")
TRACE_MSG(ATC_BASIC_CMD_7499_112_2_17_22_16_45_276,"ATC: spenha eq mode_index = %d, dev_number = %d, %d")
TRACE_MSG(ATC_BASIC_CMD_7509_112_2_17_22_16_45_277,"AGC_Get_Param failed to get mode_index, mode_index :%d.")
TRACE_MSG(ATC_BASIC_CMD_7528_112_2_17_22_16_45_278,"ATC: spenha tun_eq mode_index = %d, dev_number = %d, %d")
TRACE_MSG(ATC_BASIC_CMD_7538_112_2_17_22_16_45_279,"AGC_Get_Param failed to get mode_index, mode_index :%d.")
TRACE_MSG(ATC_BASIC_CMD_7644_112_2_17_22_16_45_280,"ATC: spenha eq mode_index = %d, dev_number = %d, %d")
TRACE_MSG(ATC_BASIC_CMD_7653_112_2_17_22_16_45_281,"AGC_Get_Param failed to get mode_index, mode_index:%d.")
TRACE_MSG(ATC_BASIC_CMD_7671_112_2_17_22_16_45_282,"ATC: spenha tun_eq mode_index = %d, dev_number = %d, %d")
TRACE_MSG(ATC_BASIC_CMD_7681_112_2_17_22_16_45_283,"AGC_Get_Param failed to get mode_index, mode_index:%d.")
TRACE_MSG(ATC_BASIC_CMD_7802_112_2_17_22_16_45_284,"ATC: siospenha for enha, param_len = %d")
TRACE_MSG(ATC_BASIC_CMD_7824_112_2_17_22_16_45_285,"ATC: siospenha for enha, set result = %d")
TRACE_MSG(ATC_BASIC_CMD_7834_112_2_17_22_16_45_286,"ATC: siospenha for enha, input data index error.")
TRACE_MSG(ATC_BASIC_CMD_7839_112_2_17_22_16_45_287,"ATC: siospenha for enha, input data length error.")
TRACE_MSG(ATC_BASIC_CMD_7844_112_2_17_22_16_45_288,"Lost the escape character is %s is line %d")
TRACE_MSG(ATC_BASIC_CMD_7894_112_2_17_22_16_45_289,"ATC: siospenha for enha tun, param_len = %d")
TRACE_MSG(ATC_BASIC_CMD_7916_112_2_17_22_16_45_290,"ATC: siospenha for enha tun, set result = %d")
TRACE_MSG(ATC_BASIC_CMD_7927_112_2_17_22_16_45_291,"ATC: siospenha for enha tun, input data index error.")
TRACE_MSG(ATC_BASIC_CMD_7932_112_2_17_22_16_45_292,"ATC: siospenha for enha tun, input data length error.")
TRACE_MSG(ATC_BASIC_CMD_7937_112_2_17_22_16_45_293,"Lost the escape character is %s is line %d")
TRACE_MSG(ATC_BASIC_CMD_7986_112_2_17_22_16_46_294,"ATC: siospenha for enha, param_len = %d")
TRACE_MSG(ATC_BASIC_CMD_7995_112_2_17_22_16_46_295,"ATC: sizeof(AUDIO_ENHA_EQ_STRUCT_T)=%d")
TRACE_MSG(ATC_BASIC_CMD_8010_112_2_17_22_16_46_296,"ATC: siospenha for enha, set result = %d")
TRACE_MSG(ATC_BASIC_CMD_8020_112_2_17_22_16_46_297,"ATC: siospenha for enha, input data index error.")
TRACE_MSG(ATC_BASIC_CMD_8025_112_2_17_22_16_46_298,"ATC: siospenha for enha, input data length error.")
TRACE_MSG(ATC_BASIC_CMD_8030_112_2_17_22_16_46_299,"Lost the escape character is %s is line %d")
TRACE_MSG(ATC_BASIC_CMD_8082_112_2_17_22_16_46_300,"ATC: siospenha for enha tun, param_len = %d")
TRACE_MSG(ATC_BASIC_CMD_8091_112_2_17_22_16_46_301,"ATC: sizeof(AUDIO_ENHA_TUNABLE_EQ_STRUCT_T)=%d")
TRACE_MSG(ATC_BASIC_CMD_8106_112_2_17_22_16_46_302,"ATC: siospenha for enha tun, set result = %d")
TRACE_MSG(ATC_BASIC_CMD_8117_112_2_17_22_16_46_303,"ATC: siospenha for enha tun, input data index error.")
TRACE_MSG(ATC_BASIC_CMD_8122_112_2_17_22_16_46_304,"ATC: siospenha for enha tun, input data length error.")
TRACE_MSG(ATC_BASIC_CMD_8127_112_2_17_22_16_46_305,"Lost the escape character is %s is line %d")
TRACE_MSG(ATC_BASIC_CMD_8232_112_2_17_22_16_46_306,"ATC: timer string len error %d")
TRACE_MSG(ATC_BASIC_CMD_8243_112_2_17_22_16_46_307,"ATC: timer string format error")
TRACE_MSG(ATC_BASIC_CMD_8252_112_2_17_22_16_46_308,"ATC: value of year error")
TRACE_MSG(ATC_BASIC_CMD_8263_112_2_17_22_16_46_309,"ATC: month digital error")
TRACE_MSG(ATC_BASIC_CMD_8269_112_2_17_22_16_46_310,"ATC: month value error")
TRACE_MSG(ATC_BASIC_CMD_8277_112_2_17_22_16_46_311,"ATC: day value error")
TRACE_MSG(ATC_BASIC_CMD_8285_112_2_17_22_16_46_312,"ATC: hour value error")
TRACE_MSG(ATC_BASIC_CMD_8293_112_2_17_22_16_46_313,"ATC: minute value error")
TRACE_MSG(ATC_BASIC_CMD_8302_112_2_17_22_16_46_314,"ATC: second value error")
TRACE_MSG(ATC_BASIC_CMD_8357_112_2_17_22_16_46_315,"ATC: save ate value: %d")
TRACE_MSG(ATC_BASIC_CMD_8397_112_2_17_22_16_46_316,"ATC: siosadm for arm, param_len = %d")
TRACE_MSG(ATC_BASIC_CMD_8418_112_2_17_22_16_46_317,"ATC:ATC_ProcessSioARMSADMData %s, %s, %d.")
TRACE_MSG(ATC_BASIC_CMD_8428_112_2_17_22_16_46_318,"ATC: Audio Mode Name too long: %d")
TRACE_MSG(ATC_BASIC_CMD_8434_112_2_17_22_16_46_319,"ATC:ATC_ProcessSioARMSADMData 2 %s, %s, %d.")
TRACE_MSG(ATC_BASIC_CMD_8439_112_2_17_22_16_46_320,"ATC: siosadm for arm, setmode param result = %d")
TRACE_MSG(ATC_BASIC_CMD_8455_112_2_17_22_16_46_321,"ATC:ATC_ProcessSioARMSADMData: setModeInfo, Mode:0x%x, pucModeName:%s, mode:%d, oldMode:%d.")
TRACE_MSG(ATC_BASIC_CMD_8462_112_2_17_22_16_47_322,"ATC: siosadm for arm , input data index error.")
TRACE_MSG(ATC_BASIC_CMD_8467_112_2_17_22_16_47_323,"ATC: siosadm for arm , input data length error.")
TRACE_MSG(ATC_BASIC_CMD_8475_112_2_17_22_16_47_324,"Lost the escape character is %s is line %d")
TRACE_MSG(ATC_BASIC_CMD_8522_112_2_17_22_16_47_325,"ATC:SioARMSADMNVData:length:%d,terminater_char:%d,mode index::%d")
TRACE_MSG(ATC_BASIC_CMD_8530_112_2_17_22_16_47_326,"ATC:cancel the snvm audio data!")
TRACE_MSG(ATC_BASIC_CMD_8546_112_2_17_22_16_47_327,"ATC:SioARMSADMNVData %s, %s, %d.")
TRACE_MSG(ATC_BASIC_CMD_8556_112_2_17_22_16_47_328,"ATC: Audio Mode Name too long: %d")
TRACE_MSG(ATC_BASIC_CMD_8562_112_2_17_22_16_47_329,"ATC:SioARMSADMNVData 2 %s, %s, %d.")
TRACE_MSG(ATC_BASIC_CMD_8566_112_2_17_22_16_47_330,"ATC:ConvertHexToBin failed.")
TRACE_MSG(ATC_BASIC_CMD_8573_112_2_17_22_16_47_331,"ATC:SioARMSADMNVData failed. result:%d")
TRACE_MSG(ATC_BASIC_CMD_8580_112_2_17_22_16_47_332,"ATC: siosadm for arm , input data index error.")
TRACE_MSG(ATC_BASIC_CMD_8586_112_2_17_22_16_47_333,"ATC: siosadmnv for arm, input audio data length error.length:%d != %d")
TRACE_MSG(ATC_BASIC_CMD_8593_112_2_17_22_16_47_334,"ATC: snvm audio data format is error.")
TRACE_MSG(ATC_BASIC_CMD_8645_112_2_17_22_16_47_335,"ATC: siosadm for dsp, param_len = %d,terminater_char=%d")
TRACE_MSG(ATC_BASIC_CMD_8656_112_2_17_22_16_47_336,"atc: sizeof(AUDIO_NV_DSP_MODE_STRUCT_T)=%d")
TRACE_MSG(ATC_BASIC_CMD_8673_112_2_17_22_16_47_337,"ATC:ATC_ProcessSioDSPSADMData %s, %d.")
TRACE_MSG(ATC_BASIC_CMD_8683_112_2_17_22_16_47_338,"ATC: Audio Mode Name too long: %d")
TRACE_MSG(ATC_BASIC_CMD_8689_112_2_17_22_16_47_339,"ATC:ATC_ProcessSioDSPSADMData 2, %s, %d.")
TRACE_MSG(ATC_BASIC_CMD_8694_112_2_17_22_16_47_340,"ATC: siosadm for dsp, setmode param result = %d")
TRACE_MSG(ATC_BASIC_CMD_8704_112_2_17_22_16_47_341,"ATC:ATC_ProcessSioDSPSADMData: setModeInfo, Mode:0x%x, pucModeName:%s, mode:%d, oldMode:%d.")
TRACE_MSG(ATC_BASIC_CMD_8711_112_2_17_22_16_47_342,"ATC: siosadm for dsp , input data index error.")
TRACE_MSG(ATC_BASIC_CMD_8716_112_2_17_22_16_47_343,"ATC: siosadm for dsp , input data length error.")
TRACE_MSG(ATC_BASIC_CMD_8724_112_2_17_22_16_47_344,"Lost the escape character is %s is line %d")
TRACE_MSG(ATC_BASIC_CMD_8771_112_2_17_22_16_47_345,"ATC:SioDSPSADMNVData:length:%d,terminater_char:%d,mode index::%d")
TRACE_MSG(ATC_BASIC_CMD_8779_112_2_17_22_16_47_346,"ATC:cancel the snvm audio data!")
TRACE_MSG(ATC_BASIC_CMD_8801_112_2_17_22_16_47_347,"ATC:SioDSPSADMNVData %s, %d.")
TRACE_MSG(ATC_BASIC_CMD_8811_112_2_17_22_16_47_348,"ATC: Audio Mode Name too long: %d")
TRACE_MSG(ATC_BASIC_CMD_8818_112_2_17_22_16_47_349,"ATC:ConvertHexToBin failed.")
TRACE_MSG(ATC_BASIC_CMD_8825_112_2_17_22_16_47_350,"ATC:SioDSPSADMNVData failed. result:%d")
TRACE_MSG(ATC_BASIC_CMD_8832_112_2_17_22_16_47_351,"ATC: siosadm for dsp , input data index error.")
TRACE_MSG(ATC_BASIC_CMD_8838_112_2_17_22_16_47_352,"ATC: siosadmnv for dsp, input audio data length error.length:%d != %d")
TRACE_MSG(ATC_BASIC_CMD_8845_112_2_17_22_16_47_353,"ATC: snvm audio data format is error.")
TRACE_MSG(ATC_BASIC_CMD_8884_112_2_17_22_16_47_354,"ATC: SIO string len=%d,Data %x,%x,%x,%x,%x")
TRACE_MSG(ATC_BASIC_CMD_8896_112_2_17_22_16_47_355,"ATC: cancel the given midi files")
TRACE_MSG(ATC_BASIC_CMD_8903_112_2_17_22_16_47_356,"ATC: the midi file format is error")
TRACE_MSG(ATC_BASIC_CMD_8929_112_2_17_22_16_47_357,"ATC: smgv SIO string lenght=%d,s_pre_block_chk_num=%d,Data1=%x,Data2=%x,Data3=%x,Data4=%x,Data5=%x, terminator = 0x%x")
TRACE_MSG(ATC_BASIC_CMD_8939_112_2_17_22_16_48_358,"ATC: smgv receive a frame head.")
TRACE_MSG(ATC_BASIC_CMD_8969_112_2_17_22_16_48_359,"ATC:midi file format error")
TRACE_MSG(ATC_BASIC_CMD_9247_112_2_17_22_16_48_360,"ATC: Assert CBC,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_9408_112_2_17_22_16_49_361,"ATC: Assert S2,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_9619_112_2_17_22_16_49_362,"ATC: Assert ECHO,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_9664_112_2_17_22_16_50_363,"ATC: Assert CICB,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_9742_112_2_17_22_16_50_364,"ATC: Assert IPR,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_9767_112_2_17_22_16_50_365,"ATC: CRMP")
TRACE_MSG(ATC_BASIC_CMD_9800_112_2_17_22_16_50_366,"VOIC, DATA and FAX CALLS are not supported now")
TRACE_MSG(ATC_BASIC_CMD_9813_112_2_17_22_16_50_367,"ATC: Assert CRMP,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_9966_112_2_17_22_16_50_368,"ATC: SNVM read nv item id = %d fail, err = %d")
TRACE_MSG(ATC_BASIC_CMD_10043_112_2_17_22_16_51_369,"ATC: Assert SLOG,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_10095_112_2_17_22_16_51_370,"ATC: need gpio direction parameter!")
TRACE_MSG(ATC_BASIC_CMD_10127_112_2_17_22_16_51_371,"ATC: Assert Failure receive gpio type = %d")
TRACE_MSG(ATC_BASIC_CMD_10136_112_2_17_22_16_51_372,"ATC: Assert SSGF,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_10189_112_2_17_22_16_51_373,"ATC: need gpio set parameter!")
TRACE_MSG(ATC_BASIC_CMD_10239_112_2_17_22_16_51_374,"ATC: Assert Failure receive gpio type = %d")
TRACE_MSG(ATC_BASIC_CMD_10248_112_2_17_22_16_51_375,"ATC: Assert SSGS,invalid type.")
TRACE_MSG(ATC_BASIC_CMD_10276_112_2_17_22_16_51_376,"ATC:ATC_ProcessCRSM")
TRACE_MSG(ATC_BASIC_CMD_10585_112_2_17_22_16_52_377,"FIRBOOT:EFS_NvitemWrite error")
TRACE_MSG(ATC_BASIC_CMD_10591_112_2_17_22_16_52_378,"winnie:read seed:%s")
TRACE_MSG(ATC_BASIC_CMD_10653_112_2_17_22_16_52_379,"ATC: sarft sign data fail.")
TRACE_MSG(ATC_BASIC_CMD_10738_112_2_17_22_16_52_380,"ATC: sarft sign data fail.")
TRACE_MSG(ATC_BASIC_CMD_10754_112_2_17_22_16_52_381,"ATC: sarft sign data fail.")
TRACE_MSG(ATC_BASIC_CMD_10771_112_2_17_22_16_52_382,"ATC: sarft sign data fail.")
TRACE_MSG(ATC_BASIC_CMD_10785_112_2_17_22_16_52_383,"ATC: sarft sign data fail.")
TRACE_MSG(ATC_BASIC_CMD_10833_112_2_17_22_16_52_384,"ATC: sim_id=%d, ncell_index=%d, arfcn=%d, bsic=%d, rxlev=%d, mcc=%d, mnc=%d, lac=%d, mnc_digit_num=%d, cell_id=%d")
TRACE_MSG(ATC_BASIC_CMD_10864_112_2_17_22_16_52_385,"ATC: sim_id=%d, scell_index=%d, arfcn=%d, bsic=%d, rxlev=%d, mcc=%d, mnc=%d, lac=%d, mnc_digit_num=%d, cell_id=%d")
TRACE_MSG(ATC_BASIC_CMD_10870_112_2_17_22_16_52_386,"ATC_ProcessPOSI sim %d, ncell_has_scell %d")
TRACE_MSG(ATC_BASIC_CMD_10878_112_2_17_22_16_52_387,"ATC_ProcessPOSI ncell_num = 0")
TRACE_MSG(ATC_BASIC_CMD_10897_112_2_17_22_16_53_388,"ATC: cell_num=%d, cell_power[%d].rxlev=%d ")
TRACE_MSG(ATC_BASIC_CMD_10902_112_2_17_22_16_53_389,"ATC_ProcessPOSI invalid cell mnc %d,cell_id %x, rxlev %d")
TRACE_MSG(ATC_BASIC_CMD_10910_112_2_17_22_16_53_390,"ATC POSI: %s")
TRACE_MSG(ATC_BASIC_CMD_10928_112_2_17_22_16_53_391,"ATC: sarft sign data fail.")
TRACE_MSG(ATC_BASIC_CMD_10930_112_2_17_22_16_53_392,"ATC_ProcessPOSI sign str %s")
TRACE_MSG(ATC_BASIC_CMD_10940_112_2_17_22_16_53_393,"ATC: sarft sign data fail.")
TRACE_MSG(ATC_BASIC_CMD_10949_112_2_17_22_16_53_394,"ATC_ProcessPOSI: %s")
TRACE_MSG(ATC_BASIC_CMD_10950_112_2_17_22_16_53_395,"ATC: the string length is %d,cell_num=%d")
TRACE_MSG(ATC_COMMON_457_112_2_17_22_16_54_396,"Log ATC infomation:\n")
TRACE_MSG(ATC_COMMON_461_112_2_17_22_16_54_397,"AT Command Index : %d\n")
TRACE_MSG(ATC_COMMON_462_112_2_17_22_16_54_398,"AT Command Type :")
TRACE_MSG(ATC_COMMON_466_112_2_17_22_16_54_399," BASIC")
TRACE_MSG(ATC_COMMON_471_112_2_17_22_16_54_400," EXTEND")
TRACE_MSG(ATC_COMMON_476_112_2_17_22_16_54_401," EXECUTE")
TRACE_MSG(ATC_COMMON_481_112_2_17_22_16_54_402," SET")
TRACE_MSG(ATC_COMMON_486_112_2_17_22_16_54_403," READ")
TRACE_MSG(ATC_COMMON_491_112_2_17_22_16_54_404," TEST")
TRACE_MSG(ATC_COMMON_494_112_2_17_22_16_54_405,"AT Command Parameter Number : %d\n")
TRACE_MSG(ATC_COMMON_507_112_2_17_22_16_54_406,"AT Command Parameter %d : %dn")
TRACE_MSG(ATC_COMMON_515_112_2_17_22_16_54_407,"AT Command Parameter %d : %sn")
TRACE_MSG(ATC_COMMON_521_112_2_17_22_16_54_408,"AT Command Parameter %d : Default\n")
TRACE_MSG(ATC_COMMON_545_112_2_17_22_16_54_409,"%s\n")
TRACE_MSG(ATC_COMMON_566_112_2_17_22_16_54_410,"ATC: Assert Failure bcd len.")
TRACE_MSG(ATC_COMMON_1344_112_2_17_22_16_56_411,"atc: txt %x, %x, %x, %x")
TRACE_MSG(ATC_COMMON_1351_112_2_17_22_16_56_412,"atc: tmp %x, %x, %x, %x")
TRACE_MSG(ATC_COMMON_1379_112_2_17_22_16_56_413,"atc: tmp %x, %x, %x, %x")
TRACE_MSG(ATC_COMMON_1419_112_2_17_22_16_56_414,"atc: tmp %x, %x, %x, %x")
TRACE_MSG(ATC_COMMON_1529_112_2_17_22_16_56_415,"ATC: Assert Failure rsp len.")
TRACE_MSG(ATC_COMMON_1562_112_2_17_22_16_56_416,"ATC: GetIndexOfCallContext out of range, call_id = %d")
TRACE_MSG(ATC_GPRS_MODEM_50_112_2_17_22_16_57_417,"ATC: ATC_ProcessNETAPN")
TRACE_MSG(ATC_GPRS_MODEM_91_112_2_17_22_16_57_418,"ATC: MNGPRS_SetPdpContextEx FAIL,%d")
TRACE_MSG(ATC_GPRS_MODEM_110_112_2_17_22_16_57_419,"ATC: set username or password error, result = %d")
TRACE_MSG(ATC_GPRS_MODEM_116_112_2_17_22_16_57_420,"ATC: username or password too long.")
TRACE_MSG(ATC_GPRS_MODEM_142_112_2_17_22_16_57_421,"atc: ProcessSOCKET")
TRACE_MSG(ATC_GPRS_MODEM_163_112_2_17_22_16_57_422,"atc: socket %d is still alive, not allowed to change it")
TRACE_MSG(ATC_GPRS_MODEM_185_112_2_17_22_16_57_423,"atc: connection type=%s")
TRACE_MSG(ATC_GPRS_MODEM_210_112_2_17_22_16_58_424,"atc: ProcessSOCKET, param3 is not string!")
TRACE_MSG(ATC_GPRS_MODEM_219_112_2_17_22_16_58_425,"atc: malloc for sock %d IP address fail")
TRACE_MSG(ATC_GPRS_MODEM_223_112_2_17_22_16_58_426,"atc: sock %d IP address malloc here")
TRACE_MSG(ATC_GPRS_MODEM_227_112_2_17_22_16_58_427,"atc: sock %d IP address has been malloc, only memset it")
TRACE_MSG(ATC_GPRS_MODEM_244_112_2_17_22_16_58_428,"atc: socket index %d port = %d")
TRACE_MSG(ATC_GPRS_MODEM_277_112_2_17_22_16_58_429,"atc: ProcessIPOPEN\n")
TRACE_MSG(ATC_GPRS_MODEM_281_112_2_17_22_16_58_430,"ATC: ProcessIPOPEN atc_config_ptr pointer is PNULL")
TRACE_MSG(ATC_GPRS_MODEM_287_112_2_17_22_16_58_431,"ATC: ProcessIPOPEN cur_cmd_info pointer is PNULL")
TRACE_MSG(ATC_GPRS_MODEM_304_112_2_17_22_16_58_432,"atc: socket %d is set previously!")
TRACE_MSG(ATC_GPRS_MODEM_313_112_2_17_22_16_58_433,"atc: socket %d is still alive, not allowed to reopen it")
TRACE_MSG(ATC_GPRS_MODEM_315_112_2_17_22_16_58_434,"atc: sock %d still alive, TCP +IPOPEN=%d")
TRACE_MSG(ATC_GPRS_MODEM_322_112_2_17_22_16_58_435,"ATC: connect socket id error, not (0--9)")
TRACE_MSG(ATC_GPRS_MODEM_331_112_2_17_22_16_58_436,"atc:PDP not activated, id = %x is set")
TRACE_MSG(ATC_GPRS_MODEM_336_112_2_17_22_16_58_437,"atc: active pdp ret =%d")
TRACE_MSG(ATC_GPRS_MODEM_340_112_2_17_22_16_58_438,"atc: active pdp fail")
TRACE_MSG(ATC_GPRS_MODEM_346_112_2_17_22_16_58_439,"atc: s_is_pdp_active_flag = TRUE")
TRACE_MSG(ATC_GPRS_MODEM_354_112_2_17_22_16_58_440,"atc:requested IP= %s, request id=%d")
TRACE_MSG(ATC_GPRS_MODEM_358_112_2_17_22_16_58_441,"atc: sock index %d DNS fail")
TRACE_MSG(ATC_GPRS_MODEM_369_112_2_17_22_16_58_442,"atc: PDP is not activated, pls dns later")
TRACE_MSG(ATC_GPRS_MODEM_399_112_2_17_22_16_58_443,"atc: ProcessIPSEND\n")
TRACE_MSG(ATC_GPRS_MODEM_403_112_2_17_22_16_58_444,"atc: ProcessIPSEND atc_config_ptr pointer is PNULL")
TRACE_MSG(ATC_GPRS_MODEM_409_112_2_17_22_16_58_445,"atc: ProcessIPSEND cur_cmd_info pointer is PNULL")
TRACE_MSG(ATC_GPRS_MODEM_426_112_2_17_22_16_58_446,"atc: socket %d is set previously!")
TRACE_MSG(ATC_GPRS_MODEM_432_112_2_17_22_16_58_447,"atc: input ID outside of connect ID range!")
TRACE_MSG(ATC_GPRS_MODEM_445_112_2_17_22_16_58_448,"atc: data state is not avtive")
TRACE_MSG(ATC_GPRS_MODEM_453_112_2_17_22_16_58_449,"ATC: ProcessIPSEND, param2 is not numeral!")
TRACE_MSG(ATC_GPRS_MODEM_485_112_2_17_22_16_58_450,"atc:  MUX ENABLE  link_id != ATC_DEBUG_LINK")
TRACE_MSG(ATC_GPRS_MODEM_502_112_2_17_22_16_58_451,"atc:set ctrl Z gprs end char")
TRACE_MSG(ATC_GPRS_MODEM_528_112_2_17_22_16_58_452,"ATC: ProcessIPCLOSE")
TRACE_MSG(ATC_GPRS_MODEM_548_112_2_17_22_16_58_453,"ATC: the ID has not been set previously!")
TRACE_MSG(ATC_GPRS_MODEM_554_112_2_17_22_16_58_454,"ATC: ProcessIPCLOSE, param ID out of its range!")
TRACE_MSG(ATC_GPRS_MODEM_568_112_2_17_22_16_58_455,"atc: socket close failure ! ")
TRACE_MSG(ATC_GPRS_MODEM_575_112_2_17_22_16_58_456,"atc: sock_id %d addr_str free here")
TRACE_MSG(ATC_GPRS_MODEM_578_112_2_17_22_16_58_457,"atc: current socket num is %d after closed")
TRACE_MSG(ATC_GPRS_MODEM_586_112_2_17_22_16_58_458,"atc: deactive pdp context fail")
TRACE_MSG(ATC_GPRS_MODEM_593_112_2_17_22_16_58_459,"atc: deactivate PDP success")
TRACE_MSG(ATC_GPRS_MODEM_599_112_2_17_22_16_58_460,"atc: +IPCLOSE: %d")
TRACE_MSG(ATC_GPRS_MODEM_605_112_2_17_22_16_58_461,"atc: socket do not exist")
TRACE_MSG(ATC_GPRS_MODEM_627_112_2_17_22_16_58_462,"atc: _socketSend data_len=%d, unsend_len=%d")
TRACE_MSG(ATC_GPRS_MODEM_632_112_2_17_22_16_58_463,"ATC:send data =%d")
TRACE_MSG(ATC_GPRS_MODEM_651_112_2_17_22_16_58_464,"ATC: ATC_SocketSend sock_error_code=%d ")
TRACE_MSG(ATC_GPRS_MODEM_672_112_2_17_22_16_58_465,"ATC:ATC_SocketRecv rx_len =%d")
TRACE_MSG(ATC_GPRS_MODEM_680_112_2_17_22_16_59_466,"ATC: g_rsp_str len=%d")
TRACE_MSG(ATC_GPRS_MODEM_685_112_2_17_22_16_59_467,"ATC:g_rsp_str len=%d, rx_len=%d")
TRACE_MSG(ATC_GPRS_MODEM_691_112_2_17_22_16_59_468,"atc: hex len=%d")
TRACE_MSG(ATC_GPRS_MODEM_732_112_2_17_22_16_59_469,"atc:  _ProcessSioData pdu data =%s, terminater_char=%x")
TRACE_MSG(ATC_GPRS_MODEM_749_112_2_17_22_16_59_470,"atc: receive ESC, sock index = %d ")
TRACE_MSG(ATC_GPRS_MODEM_771_112_2_17_22_16_59_471,"atc: sock index %d has process at+ipsend")
TRACE_MSG(ATC_GPRS_MODEM_776_112_2_17_22_16_59_472,"atc: atc_tx_buf.data_len=%d")
TRACE_MSG(ATC_GPRS_MODEM_787_112_2_17_22_16_59_473,"atc: ConvertHexToBin fail ")
TRACE_MSG(ATC_GPRS_MODEM_795_112_2_17_22_16_59_474,"atc: sock %d, socket id = %d")
TRACE_MSG(ATC_GPRS_MODEM_821_112_2_17_22_16_59_475,"atc: ActPdpContextCnf")
TRACE_MSG(ATC_GPRS_MODEM_827_112_2_17_22_16_59_476,"atc: PDP activate success s_nsapi=%d")
TRACE_MSG(ATC_GPRS_MODEM_830_112_2_17_22_16_59_477,"atc:reqeuested sock %d IP= %s, ret=%d")
TRACE_MSG(ATC_GPRS_MODEM_834_112_2_17_22_16_59_478,"atc: sock index %d dns fail")
TRACE_MSG(ATC_GPRS_MODEM_849_112_2_17_22_16_59_479,"atc: sock index result unsuccess")
TRACE_MSG(ATC_GPRS_MODEM_864_112_2_17_22_16_59_480,"atc: _ProcessSocketEvent, sig code =%d")
TRACE_MSG(ATC_GPRS_MODEM_874_112_2_17_22_16_59_481,"atc: _GETHOSTBYNAME_CNF, error_code=%d")
TRACE_MSG(ATC_GPRS_MODEM_887_112_2_17_22_16_59_482,"atc: request_id=%d, i=%d,sock index=%d")
TRACE_MSG(ATC_GPRS_MODEM_898_112_2_17_22_16_59_483,"atc: UDP +IPOPEN=%d")
TRACE_MSG(ATC_GPRS_MODEM_908_112_2_17_22_16_59_484,"atc:  Create And Connect TCP/UDP Socket fail, ret=%d")
TRACE_MSG(ATC_GPRS_MODEM_920_112_2_17_22_16_59_485,"atc: SOCKET_CONNECT_EVENT_IND")
TRACE_MSG(ATC_GPRS_MODEM_927_112_2_17_22_16_59_486,"atc: sock_index %d, socket_id=%d,  i=%d")
TRACE_MSG(ATC_GPRS_MODEM_941_112_2_17_22_16_59_487,"atc: TCP +IPOPEN=%d")
TRACE_MSG(ATC_GPRS_MODEM_946_112_2_17_22_16_59_488,"atc: connect error, error_code =%d")
TRACE_MSG(ATC_GPRS_MODEM_957_112_2_17_22_16_59_489,"atc: SOCKET_WRITE_EVENT_IND")
TRACE_MSG(ATC_GPRS_MODEM_966_112_2_17_22_16_59_490,"atc: socket_id=%d, i=%d,sock_index=%d")
TRACE_MSG(ATC_GPRS_MODEM_976_112_2_17_22_16_59_491,"atc: SOCKET_READ_EVENT_IND")
TRACE_MSG(ATC_GPRS_MODEM_984_112_2_17_22_16_59_492,"atc: socket_id=%d, i=%d,sock_index=%d")
TRACE_MSG(ATC_GPRS_MODEM_997_112_2_17_22_16_59_493,"ATC: event type error")
TRACE_MSG(ATC_GPRS_MODEM_1014_112_2_17_22_16_59_494,"ATC: _CreateAndConnectTCPSocket")
TRACE_MSG(ATC_GPRS_MODEM_1020_112_2_17_22_16_59_495,"atc: create socket fail!")
TRACE_MSG(ATC_GPRS_MODEM_1029_112_2_17_22_16_59_496,"ATC: sock index %d, socket id= %d, current sock num is %d")
TRACE_MSG(ATC_GPRS_MODEM_1035_112_2_17_22_16_59_497,"atc: TCP SOCKET ERROR !")
TRACE_MSG(ATC_GPRS_MODEM_1042_112_2_17_22_16_59_498,"ATC:IP=%d, port=%d")
TRACE_MSG(ATC_GPRS_MODEM_1051_112_2_17_22_16_59_499,"ATC: socket connect sock_error_code=%d ")
TRACE_MSG(ATC_GPRS_MODEM_1058_112_2_17_22_16_59_500,"atc: socket connect to server fail, ret=%d")
TRACE_MSG(ATC_GPRS_MODEM_1079_112_2_17_22_16_59_501,"atc: Create UDP Socket")
TRACE_MSG(ATC_GPRS_MODEM_1084_112_2_17_22_16_59_502,"atc: create UDP socket fail!")
TRACE_MSG(ATC_GPRS_MODEM_1092_112_2_17_22_16_59_503,"ATC: Create UDP Socket socket id= %d, current sock num is %d")
TRACE_MSG(ATC_GPRS_MODEM_1098_112_2_17_22_16_59_504,"atc: UDP SOCKET ERROR !")
TRACE_MSG(ATC_GPRS_MODEM_1105_112_2_17_22_16_59_505,"ATC: UDP IP=%d, port=%d")
TRACE_MSG(ATC_GPRS_MODEM_1111_112_2_17_22_16_59_506,"ATC: UDP socket connect sock_error_code=%d ")
TRACE_MSG(ATC_GPRS_MODEM_1128_112_2_17_22_16_59_507,"ATC: ATC_ProcessIPDACT")
TRACE_MSG(ATC_GPRS_MODEM_1138_112_2_17_22_16_59_508,"Detach failed!")
TRACE_MSG(ATC_GPRS_MODEM_1163_112_2_17_22_17_0_509,"atc: PDP deactivated  by network")
TRACE_MSG(ATC_GPRS_MODEM_1176_112_2_17_22_17_0_510,"atc: sock_id %d addr_str free here")
TRACE_MSG(ATC_GSM_242_112_2_17_22_17_0_511,"ATC: ATC_ProcessA, MNCALL_ConnectCall failure!")
TRACE_MSG(ATC_GSM_306_112_2_17_22_17_0_512,"ATC: rra is traffic when atd*99#")
TRACE_MSG(ATC_GSM_377_112_2_17_22_17_0_513,"ATC: this is a emergency call!")
TRACE_MSG(ATC_GSM_457_112_2_17_22_17_1_514,"ATC: rra is traffic when gsm atd")
TRACE_MSG(ATC_GSM_519_112_2_17_22_17_1_515,"ATC: Assert Failure call_amount error %d.")
TRACE_MSG(ATC_GSM_704_112_2_17_22_17_1_516,"ATC: ATD*99 or 98#  RequestPdpId pdp_id:%d")
TRACE_MSG(ATC_GSM_805_112_2_17_22_17_1_517,"ATC: no active call found.")
TRACE_MSG(ATC_GSM_809_112_2_17_22_17_1_518,"ATC: send dtmf id = %d")
TRACE_MSG(ATC_GSM_830_112_2_17_22_17_1_519,"ATC: StartDTMF = %d")
TRACE_MSG(ATC_GSM_844_112_2_17_22_17_1_520,"ATC: StopDTMF!")
TRACE_MSG(ATC_GSM_860_112_2_17_22_17_1_521,"ATC: start a DTMF, but not stop it, operation refused!")
TRACE_MSG(ATC_GSM_928_112_2_17_22_17_2_522,"ATC: ATC_ProcessCOPS, not allowed when CALL is existed.")
TRACE_MSG(ATC_GSM_942_112_2_17_22_17_2_523,"ATC: ATC_ProcessCOPS, sim is not ready or cpin code is not ready.")
TRACE_MSG(ATC_GSM_959_112_2_17_22_17_2_524,"ATC: MNPHONE_SelectPLMN failure")
TRACE_MSG(ATC_GSM_1017_112_2_17_22_17_2_525,"ATC: Assert Failure unknow creg.")
TRACE_MSG(ATC_GSM_1040_112_2_17_22_17_2_526,"ATC: Assert Failure unknow cgreg")
TRACE_MSG(ATC_GSM_1184_112_2_17_22_17_2_527,"ATC: Assert Failure plmn_status.")
TRACE_MSG(ATC_GSM_1196_112_2_17_22_17_2_528,"ATC: search the plmn list")
TRACE_MSG(ATC_GSM_1206_112_2_17_22_17_2_529,"ATC: ATC_ProcessCOPS, failure in MNPHONE_ListPLMN.")
TRACE_MSG(ATC_GSM_1552_112_2_17_22_17_3_530,"ATC: received the plmn list confirm ")
TRACE_MSG(ATC_GSM_1553_112_2_17_22_17_3_531,"ATC: %d,%d")
TRACE_MSG(ATC_GSM_1593_112_2_17_22_17_3_532,"ACT: RAT value is wrong")
TRACE_MSG(ATC_GSM_1710_112_2_17_22_17_3_533,"ATC: Assert Failure unknow plmn_status.")
TRACE_MSG(ATC_GSM_1805_112_2_17_22_17_4_534,"ATC: ManualSelectPlmn, FULL_NAME, not found!")
TRACE_MSG(ATC_GSM_1818_112_2_17_22_17_4_535,"ATC: ManualSelectPlmn, SHORT_NAME, not found!")
TRACE_MSG(ATC_GSM_1835_112_2_17_22_17_4_536,"ATC: ManualSelectPlmn, NUMBERIC, cops not support this plmn")
TRACE_MSG(ATC_GSM_1846_112_2_17_22_17_4_537,"ATC: ManualSelectPlmn, plmn = %d")
TRACE_MSG(ATC_GSM_1862_112_2_17_22_17_4_538,"ATC: ManualSelectPlmn, cops not support this plmn")
TRACE_MSG(ATC_GSM_1872_112_2_17_22_17_4_539,"ATC: ManualSelectPlmn, failure in MN")
TRACE_MSG(ATC_GSM_1896_112_2_17_22_17_4_540,"ATC: ATC_GetPlmnDetailedInfo, Invalid parameters")
TRACE_MSG(ATC_GSM_1908_112_2_17_22_17_4_541,"ATC: ATC_GetPlmnDetailedInfo, FULL_NAME is too long (%d)")
TRACE_MSG(ATC_GSM_1933_112_2_17_22_17_4_542,"ATC: ATC_GetPlmnDetailedInfo, SHORT_NAME is too long (%d)")
TRACE_MSG(ATC_GSM_2013_112_2_17_22_17_4_543,"ATC:invalid plmn num %c")
TRACE_MSG(ATC_GSM_2100_112_2_17_22_17_4_544,"ATC: Assert Failure call_amount %d.")
TRACE_MSG(ATC_GSM_2127_112_2_17_22_17_4_545,"ATC: call amount %d")
TRACE_MSG(ATC_GSM_2186_112_2_17_22_17_4_546,"ATC: no call number present!")
TRACE_MSG(ATC_GSM_2217_112_2_17_22_17_5_547,"ATC: force detach sim %d pdp %d")
TRACE_MSG(ATC_GSM_2234_112_2_17_22_17_5_548,"ATC: send sms fail when cring")
TRACE_MSG(ATC_GSM_2239_112_2_17_22_17_5_549,"ATC: call amount %d")
TRACE_MSG(ATC_GSM_2365_112_2_17_22_17_5_550,"ATC: plmn %s")
TRACE_MSG(ATC_GSM_2397_112_2_17_22_17_5_551,"atc: ATC_ProcessCallDiscInd")
TRACE_MSG(ATC_GSM_2405_112_2_17_22_17_5_552,"atc: receive APP_MN_CALL_DISCONNECTED_IND call id =%d")
TRACE_MSG(ATC_GSM_2414_112_2_17_22_17_5_553,"atc: receive APP_MN_CALL_ERR_IND call id =%d")
TRACE_MSG(ATC_GSM_2420_112_2_17_22_17_5_554,"ATC: Assert Failure unknow disc code.")
TRACE_MSG(ATC_GSM_2472_112_2_17_22_17_5_555,"atc: ss state %x,cpas %d")
TRACE_MSG(ATC_GSM_2607_112_2_17_22_17_5_556,"ATC: unexpected timer expire")
TRACE_MSG(ATC_GSM_2666_112_2_17_22_17_5_557,"ATC: the dtmf value is %c")
TRACE_MSG(ATC_GSM_2693_112_2_17_22_17_6_558,"ATC: Assert Failure error call id %d.")
TRACE_MSG(ATC_GSM_SMS_411_112_2_17_22_17_7_559,"ATC:CMGL wrong type.")
TRACE_MSG(ATC_GSM_SMS_440_112_2_17_22_17_7_560,"ATC:CMGL wrong type.")
TRACE_MSG(ATC_GSM_SMS_498_112_2_17_22_17_7_561,"ATC:CSMS wrong type.")
TRACE_MSG(ATC_GSM_SMS_545_112_2_17_22_17_7_562,"ATC:CSDH wrong type.")
TRACE_MSG(ATC_GSM_SMS_575_112_2_17_22_17_7_563,"ATC: Get sc failure")
TRACE_MSG(ATC_GSM_SMS_582_112_2_17_22_17_7_564,"ATC: Read s_sms_nv_config failure")
TRACE_MSG(ATC_GSM_SMS_614_112_2_17_22_17_7_565,"ATC: write NV_ATC_CONFIG_ID fail")
TRACE_MSG(ATC_GSM_SMS_668_112_2_17_22_17_7_566,"ATC:+CPMS,error in para1")
TRACE_MSG(ATC_GSM_SMS_697_112_2_17_22_17_7_567,"ATC:+CPMS,error in para2")
TRACE_MSG(ATC_GSM_SMS_770_112_2_17_22_17_8_568,"ATC:+CPMS,error in para")
TRACE_MSG(ATC_GSM_SMS_1049_112_2_17_22_17_8_569,"ATC: Get sc failure")
TRACE_MSG(ATC_GSM_SMS_1323_112_2_17_22_17_9_570,"ATC: Get sc failure")
TRACE_MSG(ATC_GSM_SMS_1330_112_2_17_22_17_9_571,"ATC: Read s_sms_nv_config failure")
TRACE_MSG(ATC_GSM_SMS_1593_112_2_17_22_17_9_572,"ATC: Get sc failure")
TRACE_MSG(ATC_GSM_SMS_1600_112_2_17_22_17_9_573,"ATC: Read s_sms_nv_config failure")
TRACE_MSG(ATC_GSM_SMS_1668_112_2_17_22_17_9_574,"ATC: Read msg and language from NV fail")
TRACE_MSG(ATC_GSM_SMS_1800_112_2_17_22_17_10_575,"ATC:MNSMS_GetSmsStatus is error")
TRACE_MSG(ATC_GSM_SMS_1823_112_2_17_22_17_10_576,"ATC:status_flag = FALSE")
TRACE_MSG(ATC_GSM_SMS_1829_112_2_17_22_17_10_577,"ATC:ChangeSmsStatus is FALSE")
TRACE_MSG(ATC_GSM_SMS_1896_112_2_17_22_17_10_578,"ATC: ATC_ProcessSFSMSM is called")
TRACE_MSG(ATC_GSM_SMS_1970_112_2_17_22_17_10_579,"ATC:SFSMSM assert wrong para")
TRACE_MSG(ATC_GSM_SMS_2133_112_2_17_22_17_10_580,"ATC: write NV_ATC_CONFIG_ID fail")
TRACE_MSG(ATC_GSM_SMS_2199_112_2_17_22_17_10_581,"ATC:enter HandlePduModeSms")
TRACE_MSG(ATC_GSM_SMS_2206_112_2_17_22_17_10_582,"ATC:error ConvertHexToBin")
TRACE_MSG(ATC_GSM_SMS_2216_112_2_17_22_17_10_583,"ATC:no SC address")
TRACE_MSG(ATC_GSM_SMS_2220_112_2_17_22_17_10_584,"ATC:please set the SC address.")
TRACE_MSG(ATC_GSM_SMS_2241_112_2_17_22_17_10_585,"ATC: HandlePduModeSms sc_addr.length = %d")
TRACE_MSG(ATC_GSM_SMS_2259_112_2_17_22_17_10_586,"ATC:have SC address")
TRACE_MSG(ATC_GSM_SMS_2263_112_2_17_22_17_10_587,"ATC:SC address length %d out range(1-%d)")
TRACE_MSG(ATC_GSM_SMS_2478_112_2_17_22_17_11_588,"ATC:ATC_ProcessSendSmsCnf ignore result %d")
TRACE_MSG(ATC_GSM_SMS_2523_112_2_17_22_17_11_589,"ATC: Receive the error status: %d")
TRACE_MSG(ATC_GSM_SMS_2569_112_2_17_22_17_11_590,"ATC: Received APP_MN_UPDATE_SMS_STATE_CNF, sleep 30ms")
TRACE_MSG(ATC_GSM_SMS_2764_112_2_17_22_17_11_591,"ATC: the ASSERT sms_state")
TRACE_MSG(ATC_GSM_SMS_2772_112_2_17_22_17_11_592,"ATC: the current cmgl index: %d")
TRACE_MSG(ATC_GSM_SMS_2789_112_2_17_22_17_12_593,"ATC: enter into ATC_ProcessWriteSmsCnf")
TRACE_MSG(ATC_GSM_SMS_2807_112_2_17_22_17_12_594,"ATC:WRITE error!")
TRACE_MSG(ATC_GSM_SMS_2868_112_2_17_22_17_12_595,"ATC:WRITE error, Stopping Filling SMS Storage!")
TRACE_MSG(ATC_GSM_SMS_2896_112_2_17_22_17_12_596,"ATC: ATC_ProcessSmsFullInd is called")
TRACE_MSG(ATC_GSM_SMS_2902_112_2_17_22_17_12_597,"ATC:SFSMSM Filled Successfully!")
TRACE_MSG(ATC_GSM_SMS_2909_112_2_17_22_17_12_598,"ATC: mem full %d")
TRACE_MSG(ATC_GSM_SMS_2933_112_2_17_22_17_12_599,"ATC:SFSMSM Filled Successfully!")
TRACE_MSG(ATC_GSM_SMS_2958_112_2_17_22_17_12_600,"ATC: Get sc failure")
TRACE_MSG(ATC_GSM_SMS_3007_112_2_17_22_17_12_601,"ATC:PDU allocation fail,exit +CMT!")
TRACE_MSG(ATC_GSM_SMS_3046_112_2_17_22_17_12_602,"ATC: disable the status report")
TRACE_MSG(ATC_GSM_SMS_3122_112_2_17_22_17_12_603,"ATC:assert wrong dcs type!!")
TRACE_MSG(ATC_GSM_SMS_3213_112_2_17_22_17_12_604,"ATC:status_report_is_request=%d")
TRACE_MSG(ATC_GSM_SMS_3400_112_2_17_22_17_13_605,"ATC: Enter the HandleTextModeSmsReadCnf routine")
TRACE_MSG(ATC_GSM_SMS_3430_112_2_17_22_17_13_606,"ATC: SMS FREE SPACE!")
TRACE_MSG(ATC_GSM_SMS_3583_112_2_17_22_17_13_607,"ATC:assert wrong sms_status!")
TRACE_MSG(ATC_GSM_SMS_3590_112_2_17_22_17_13_608,"ATC: The SMS data len is %d")
TRACE_MSG(ATC_GSM_SMS_3665_112_2_17_22_17_13_609,"ATC: Received  APP_MN_READ_SMS_CNF, text mode, sleep 30ms")
TRACE_MSG(ATC_GSM_SMS_3710_112_2_17_22_17_13_610,"ATC: the SMS len is %d")
TRACE_MSG(ATC_GSM_SMS_3780_112_2_17_22_17_14_611,"ATC: pdu read sms cnf, mux link id %d")
TRACE_MSG(ATC_GSM_SMS_3789_112_2_17_22_17_14_612,"ATC: Read a free SMS space.")
TRACE_MSG(ATC_GSM_SMS_3892_112_2_17_22_17_14_613,"ATC: Received  APP_MN_READ_SMS_CNF, pdu mode, sleep 30ms")
TRACE_MSG(ATC_GSM_SMS_3974_112_2_17_22_17_14_614,"ATC:PDU allocation fail,exit +CMT!")
TRACE_MSG(ATC_GSM_SMS_4076_112_2_17_22_17_14_615,"ATC: The temp_len is %d")
TRACE_MSG(ATC_GSM_SMS_4243_112_2_17_22_17_14_616,"ATC:assert wrong PduModeSmsReadCnf!")
TRACE_MSG(ATC_GSM_SMS_4252_112_2_17_22_17_14_617,"ATC:the current cmgl index: %d")
TRACE_MSG(ATC_GSM_SMS_4434_112_2_17_22_17_15_618,"ATC: Write NV_ATC_CB_MSG_ID failed")
TRACE_MSG(ATC_GSM_SMS_4439_112_2_17_22_17_15_619,"ATC: Write NV_ATC_CB_LANG_ID failed")
TRACE_MSG(ATC_GSM_SMS_4541_112_2_17_22_17_15_620,"ATC:BuildSmsCodeRsp,rl_cause is %d")
TRACE_MSG(ATC_GSM_SMS_4542_112_2_17_22_17_15_621,"ATC:BuildSmsCodeRsp,The string=%s")
TRACE_MSG(ATC_GSM_SMS_4587_112_2_17_22_17_15_622,"ATC: enter into Atc_FillMtMoSmsEx")
TRACE_MSG(ATC_GSM_SMS_4629_112_2_17_22_17_15_623,"ATC: enter into Atc_FreeMemForSfsmsm")
TRACE_MSG(ATC_GSM_SMS_4681_112_2_17_22_17_15_624,"ATC: Atc_UpdateSmsMemSts is called")
TRACE_MSG(ATC_GSM_SMS_4696_112_2_17_22_17_15_625,"Get Me Storage inforamtion failure!")
TRACE_MSG(ATC_GSM_SMS_4713_112_2_17_22_17_15_626,"Get SIM %d Storage inforamtion failure!")
TRACE_MSG(ATC_GSM_SS_230_112_2_17_22_17_17_627,"ATC_EnableVoiceCodec: is_enable = %d")
TRACE_MSG(ATC_GSM_SS_320_112_2_17_22_17_17_628,"ATC:CCFC reg fail1")
TRACE_MSG(ATC_GSM_SS_327_112_2_17_22_17_17_629,"ATC command's parameter type wrong")
TRACE_MSG(ATC_GSM_SS_343_112_2_17_22_17_17_630,"ATC:CCFC reg fail2")
TRACE_MSG(ATC_GSM_SS_360_112_2_17_22_17_17_631,"ATC:CCFC reg fail3")
TRACE_MSG(ATC_GSM_SS_727_112_2_17_22_17_18_632,"ATC: user set the AOC mode")
TRACE_MSG(ATC_GSM_SS_1103_112_2_17_22_17_18_633,"ATC: OFFSET %d")
TRACE_MSG(ATC_GSM_SS_1140_112_2_17_22_17_18_634,"ATC:USSD,PARAM1_FLAG = 0")
TRACE_MSG(ATC_GSM_SS_1234_112_2_17_22_17_19_635,"ATC:MNSS_UserOriginateUSSDEx")
TRACE_MSG(ATC_GSM_SS_1278_112_2_17_22_17_19_636,"ATC: unknown  cscs %s")
TRACE_MSG(ATC_GSM_SS_1291_112_2_17_22_17_19_637,"ATC: ATC_ConvertAsciiToUcs2Hex return false")
TRACE_MSG(ATC_GSM_SS_1386_112_2_17_22_17_19_638,"ATC: ussd ind ussd len is %d, str is %s")
TRACE_MSG(ATC_GSM_SS_1433_112_2_17_22_17_19_639,"ATC: ussd_str is %s")
TRACE_MSG(ATC_GSM_SS_1445_112_2_17_22_17_19_640,"ATC: ussd_str is pnull")
TRACE_MSG(ATC_GSM_SS_1466_112_2_17_22_17_19_641,"ATC: ussd notify ussd len is %d, str is %s")
TRACE_MSG(ATC_GSM_SS_1515_112_2_17_22_17_19_642,"ATC: ussd_str is %s")
TRACE_MSG(ATC_GSM_SS_1527_112_2_17_22_17_19_643,"ATC: ussd_str is pnull")
TRACE_MSG(ATC_GSM_SS_1550_112_2_17_22_17_19_644,"ATC: ussd service cnf ussd len is %d, str is %s")
TRACE_MSG(ATC_GSM_SS_1621_112_2_17_22_17_19_645,"ATC: ussd_str is %s")
TRACE_MSG(ATC_GSM_SS_1635_112_2_17_22_17_20_646,"ATC: ussd_str is pnull")
TRACE_MSG(ATC_GSM_SS_2111_112_2_17_22_17_20_647,"ATC:ATC_GetParams=FALSE")
TRACE_MSG(ATC_GSM_SS_2332_112_2_17_22_17_21_648,"ATC: CPIN pin1 enable flag: %d")
TRACE_MSG(ATC_GSM_SS_2452_112_2_17_22_17_21_649,"ATC: cpwd parameter is not right")
TRACE_MSG(ATC_GSM_SS_2519_112_2_17_22_17_21_650,"ATC: cpwd sim is not ready")
TRACE_MSG(ATC_GSM_SS_2535_112_2_17_22_17_21_651,"ATC: cpwd sim is not ready")
TRACE_MSG(ATC_GSM_SS_2563_112_2_17_22_17_21_652,"ATC:AT_PWD_AB")
TRACE_MSG(ATC_GSM_SS_2610_112_2_17_22_17_21_653,"ATC:ATC_ProcessGetPswInd")
TRACE_MSG(ATC_GSM_SS_2615_112_2_17_22_17_21_654,"ATC:AT_CMD_CPWD")
TRACE_MSG(ATC_GSM_SS_2619_112_2_17_22_17_21_655,"ATC:s_old_password")
TRACE_MSG(ATC_GSM_SS_2625_112_2_17_22_17_21_656,"ATC:s_new_password")
TRACE_MSG(ATC_GSM_SS_2630_112_2_17_22_17_22_657,"ATC:s_new_password again")
TRACE_MSG(ATC_GSM_SS_2635_112_2_17_22_17_22_658,"ATC: received error singal in get password indicate!!!")
TRACE_MSG(ATC_GSM_SS_2723_112_2_17_22_17_22_659,"enter into CheckPresentHeldCall")
TRACE_MSG(ATC_GSM_SS_2731_112_2_17_22_17_22_660,"found a held call %d")
TRACE_MSG(ATC_GSM_SS_2752_112_2_17_22_17_22_661,"enter into RelPresentHeldCalls")
TRACE_MSG(ATC_GSM_SS_2761_112_2_17_22_17_22_662,"held_id: %d")
TRACE_MSG(ATC_GSM_SS_2795_112_2_17_22_17_22_663,"enter into CheckPresentActiveCall")
TRACE_MSG(ATC_GSM_SS_2803_112_2_17_22_17_22_664,"found an active call %d")
TRACE_MSG(ATC_GSM_SS_2823_112_2_17_22_17_22_665,"enter into RelPresentActiveCalls")
TRACE_MSG(ATC_GSM_SS_2832_112_2_17_22_17_22_666,"call %d released")
TRACE_MSG(ATC_GSM_SS_2856_112_2_17_22_17_22_667,"enter into CheckPresentWaitCall")
TRACE_MSG(ATC_GSM_SS_2865_112_2_17_22_17_22_668,"found a waiting call %d")
TRACE_MSG(ATC_GSM_SS_2884_112_2_17_22_17_22_669,"enter into SetUserBusyForWaitCall")
TRACE_MSG(ATC_GSM_SS_2888_112_2_17_22_17_22_670,"waiting call %d set UDUB")
TRACE_MSG(ATC_GSM_SS_2919_112_2_17_22_17_22_671,"ATC:ATC_ProcessSSCnf result=%d,list_num=%d,ss_status_present=%d,ss_status=%d")
TRACE_MSG(ATC_GSM_SS_2994_112_2_17_22_17_22_672,"ATC:ATC_CheckInterrogateCnf result=%d,list_num=%d,ss_status_present=%d,ss_status=%d,service_type=%d,service_code=%d")
TRACE_MSG(ATC_GSM_SS_3043_112_2_17_22_17_22_673,"enter into HandleChldUdub")
TRACE_MSG(ATC_GSM_SS_3079_112_2_17_22_17_22_674,"enter into HandleChldRelAcceptOthers")
TRACE_MSG(ATC_GSM_SS_3148_112_2_17_22_17_23_675,"enter into HandleChldHoldAcceptOthers")
TRACE_MSG(ATC_GSM_SS_3213_112_2_17_22_17_23_676,"enter into HandleChldMultCall")
TRACE_MSG(ATC_GSM_SS_3219_112_2_17_22_17_23_677,"held_id = %d, active_id = %d")
TRACE_MSG(ATC_GSM_SS_3573_112_2_17_22_17_23_678,"ATC: MNSS_UserOriginateUSSD result = %d")
TRACE_MSG(ATC_GSM_SS_3589_112_2_17_22_17_23_679,"ATC: MNSS_ResponseUSSD result = %d")
TRACE_MSG(ATC_GSM_SS_3694_112_2_17_22_17_24_680,"ATC: unknow ss code = %d")
TRACE_MSG(ATC_GSM_SS_3732_112_2_17_22_17_24_681,"ATC: mn register ss error code = %d")
TRACE_MSG(ATC_GSM_SS_3748_112_2_17_22_17_24_682,"ATC: mn erase ss info error code = %d")
TRACE_MSG(ATC_GSM_SS_3764_112_2_17_22_17_24_683,"ATC: mn active ss error code %d")
TRACE_MSG(ATC_GSM_SS_3780_112_2_17_22_17_24_684,"ATC: mn deactive ss error code = %d")
TRACE_MSG(ATC_GSM_SS_3797_112_2_17_22_17_24_685,"ATC: mn interrogate service error code = %d")
TRACE_MSG(ATC_GSM_SS_3814_112_2_17_22_17_24_686,"ATC: mn register ss password error code = %d")
TRACE_MSG(ATC_GSM_SS_3826_112_2_17_22_17_24_687,"ATC: SendReqToSs in default!")
TRACE_MSG(ATC_GSM_SS_4133_112_2_17_22_17_25_688,"ATC: PARA2.str_ptr->str_len is %d, str is %s")
TRACE_MSG(ATC_GSM_SS_4139_112_2_17_22_17_25_689,"ATC: ptr_para2_str str_len is %d, str is %s")
TRACE_MSG(ATC_GSM_SS_4143_112_2_17_22_17_25_690,"ATC: PARA2.str_ptr->str_len is %d, str is %s")
TRACE_MSG(ATC_GSM_SS_4149_112_2_17_22_17_25_691,"ATC: ptr_para2_str str_len is %d, str is %s")
TRACE_MSG(ATC_GSM_STK_268_112_2_17_22_17_26_692,"ATC: error length in simat.")
TRACE_MSG(ATC_GSM_STK_338_112_2_17_22_17_26_693,"ATC: error data length in simat")
TRACE_MSG(ATC_GSM_STK_369_112_2_17_22_17_26_694,"ATC:ATC_ProcessGetInkey,%d,%d")
TRACE_MSG(ATC_GSM_STK_431_112_2_17_22_17_26_695,"ATC: error length in simat.")
TRACE_MSG(ATC_GSM_STK_656_112_2_17_22_17_27_696,"ATC: ATC_ProcessSendSMS sc_addr.length = %d")
TRACE_MSG(ATC_GSM_STK_726_112_2_17_22_17_27_697,"ATC: send sms pdu err code = %d.")
TRACE_MSG(ATC_GSM_STK_735_112_2_17_22_17_27_698,"ATC: send sms pdu err code = %d.")
TRACE_MSG(ATC_GSM_STK_898_112_2_17_22_17_27_699,"ATC: user originate ussd fail, err code = %d.")
TRACE_MSG(ATC_GSM_STK_928_112_2_17_22_17_27_700,"ATC:enter ATC_STKProcessSendSmsCnf")
TRACE_MSG(ATC_GSM_STK_941_112_2_17_22_17_27_701,"ATC: Receive the error status: %d")
TRACE_MSG(ATC_GSM_STK_954_112_2_17_22_17_27_702,"ATC:ATC_STKProcessSendSmsCnf,MN_SMS_OPERATE_SUCCESS")
TRACE_MSG(ATC_GSM_STK_962_112_2_17_22_17_27_703,"ATC:ATC_STKProcessSendSmsCnf,MN_SMS_FAIL")
TRACE_MSG(ATC_GSM_STK_1108_112_2_17_22_17_28_704,"ATC:ATC_CheckAndHandleGetInkey,SIMAT_DIGIT_ONLY,%d")
TRACE_MSG(ATC_GSM_STK_1234_112_2_17_22_17_28_705,"ATC:ATC_CheckAndHandleGetInkey==TRUE")
TRACE_MSG(ATC_GSM_STK_1297_112_2_17_22_17_28_706,"ATC:error in length")
TRACE_MSG(ATC_GSM_STK_1305_112_2_17_22_17_28_707,"ATC: getinput type = %d, str_len = %d, min_len = %d max_len = %d")
TRACE_MSG(ATC_GSM_STK_1324_112_2_17_22_17_28_708,"ATC:error in length")
TRACE_MSG(ATC_GSM_STK_1345_112_2_17_22_17_28_709,"ATC:error in length")
TRACE_MSG(ATC_GSM_STK_1362_112_2_17_22_17_28_710,"ATC:error in length")
TRACE_MSG(ATC_GSM_STK_1450_112_2_17_22_17_28_711,"ATC:ATC_CheckAndHandleGetInkey==TRUE")
TRACE_MSG(ATC_GSM_STK_1935_112_2_17_22_17_29_712,"ATC: stk deactivate fail.")
TRACE_MSG(ATC_GSM_STK_1992_112_2_17_22_17_30_713,"ATC:ATC_CheckAndHandleConfig is false")
TRACE_MSG(ATC_GSM_STK_2056_112_2_17_22_17_30_714,"ATC:PARA1.num != s_stk_signal[dual_sys].cur_status")
TRACE_MSG(ATC_GSM_STK_2446_112_2_17_22_17_30_715,"ATC:s_cmd_stag=%d")
TRACE_MSG(ATC_GSM_STK_2488_112_2_17_22_17_31_716,"ATC:ATC_MainMenuCnf=FALSE")
TRACE_MSG(ATC_GSM_STK_2741_112_2_17_22_17_31_717,"ATC: get imei failed.")
TRACE_MSG(ATC_GSM_STK_3082_112_2_17_22_17_32_718,"ATC:EV_MN_APP_SIMAT_LANG_NOTIFY_IND_F")
TRACE_MSG(ATC_GSM_STK_3087_112_2_17_22_17_32_719,"ATC:SIMAT_EVENT_REFRESH")
TRACE_MSG(ATC_GSM_STK_3092_112_2_17_22_17_32_720,"ATC:SIMAT_EVENT_CC_RSP")
TRACE_MSG(ATC_GSM_STK_3098_112_2_17_22_17_32_721,"ATC:SIMAT_EVENT_SS_CON_RSP")
TRACE_MSG(ATC_GSM_STK_3104_112_2_17_22_17_32_722,"ATC:SIMAT_EVENT_USSD_CON_RSP")
TRACE_MSG(ATC_GSM_STK_3110_112_2_17_22_17_32_723,"ATC:SIMAT_EVENT_SMS_CON_RSP")
TRACE_MSG(ATC_GSM_STK_3116_112_2_17_22_17_32_724,"ATC:SIMAT_EVENT_REFRESH")
TRACE_MSG(ATC_GSM_STK_3122_112_2_17_22_17_32_725,"ATC:SIMAT_EVENT_SMS_PP_RSP")
TRACE_MSG(ATC_GSM_STK_3222_112_2_17_22_17_32_726,"ATC: HandleStkPduModeSms")
TRACE_MSG(ATC_GSM_STK_3231_112_2_17_22_17_32_727,"ATC: HandleStkPduModeSms sc_addr.length = %d")
TRACE_MSG(ATC_GSM_STK_3238_112_2_17_22_17_32_728,"ATC:error ConvertHexToBin")
TRACE_MSG(ATC_GSM_STK_3252_112_2_17_22_17_32_729,"ATC: Send Sms PDU failed!")
TRACE_MSG(ATC_GSM_STK_3278_112_2_17_22_17_32_730,"ATC: ATC_ProcessSioStkSmsData")
TRACE_MSG(ATC_GSM_STK_3342_112_2_17_22_17_32_731,"ATC: ATC_ProcessSIMRefreshCnf")
TRACE_MSG(ATC_GSM_STK_3376_112_2_17_22_17_32_732,"OnRefreshCnfSimFileChange:sim_file = %d")
TRACE_MSG(ATC_GSM_STK_3421_112_2_17_22_17_33_733,"ATC: ATC_ProcessPSSTKE")
TRACE_MSG(ATC_GSM_STK_3466_112_2_17_22_17_33_734,"ATC: ATC_ProcessPSSTKR")
TRACE_MSG(ATC_GSM_STK_3508_112_2_17_22_17_33_735,"ATC:ATC_ProcessPSSTKI")
TRACE_MSG(ATC_GSM_STK_3521_112_2_17_22_17_33_736,"ATC: save stk %d menu cnt = %d")
TRACE_MSG(ATC_GSM_STK_3525_112_2_17_22_17_33_737,"ATC: send stk menu cnt = %d, len = %d")
TRACE_MSG(ATC_GSM_STK_3584_112_2_17_22_17_33_738,"ATC: stk %d setup menu cnt = %d")
TRACE_MSG(ATC_GSM_STK_3622_112_2_17_22_17_33_739,"ATC: ATC_ProcessPSSTKSMS")
TRACE_MSG(ATC_GSM_STK_3677_112_2_17_22_17_33_740,"ATC: convert tele string failed!\r\n  PARA4.str_ptr->str_ptr:%s ")
TRACE_MSG(ATC_GSM_STK_3733_112_2_17_22_17_33_741,"ATC: ATC_ProcessSTKREFRESH")
TRACE_MSG(ATC_GSM_STK_3786_112_2_17_22_17_33_742,"ATC: convert file list string failed!\r\n  PARA3.str_ptr->str_ptr:%s ")
TRACE_MSG(ATC_GSM_STK_3793_112_2_17_22_17_33_743,"ATC: file list decode failed!\r\n  data_len: %d, data_ptr: %p ")
TRACE_MSG(ATC_MALLOC_263_112_2_17_22_17_44_744,"Add heap success, heap size = %d.\n")
TRACE_MSG(ATC_PHONEBOOK_409_112_2_17_22_17_45_745,"ATC:cpbr 80 type, %x, %x, %x")
TRACE_MSG(ATC_PHONEBOOK_415_112_2_17_22_17_45_746,"ATC: CONVERT TO IRA FAIL")
TRACE_MSG(ATC_PHONEBOOK_423_112_2_17_22_17_45_747,"ATC:cpbr 81 type, %x, %x, %x")
TRACE_MSG(ATC_PHONEBOOK_438_112_2_17_22_17_45_748,"ATC:cpbr 82 type, %x, %x, %x")
TRACE_MSG(ATC_PHONEBOOK_454_112_2_17_22_17_45_749,"ATC: cpbr str type, %x, %x, %x")
TRACE_MSG(ATC_PHONEBOOK_463_112_2_17_22_17_45_750,"ATC: tel %x, %x, %x, %x")
TRACE_MSG(ATC_PHONEBOOK_470_112_2_17_22_17_45_751,"ATC: unknown  cscs %s, use IRA")
TRACE_MSG(ATC_PHONEBOOK_538_112_2_17_22_17_45_752,"ATC_CMD_PARAM_TYPE_STRING != PARAM1_TYPE")
TRACE_MSG(ATC_PHONEBOOK_605_112_2_17_22_17_45_753,"__ 80 %x, %x, %x")
TRACE_MSG(ATC_PHONEBOOK_617_112_2_17_22_17_45_754,"__ 81 %x, %x, %x")
TRACE_MSG(ATC_PHONEBOOK_632_112_2_17_22_17_45_755,"__ 82 %x, %x, %x")
TRACE_MSG(ATC_PHONEBOOK_656_112_2_17_22_17_45_756,"ATC: tel %x, %x, %x, %x")
TRACE_MSG(ATC_PHONEBOOK_663_112_2_17_22_17_45_757,"ATC: unknown  cscs %s, use IRA")
TRACE_MSG(ATC_PHONEBOOK_875_112_2_17_22_17_46_758,"ATC:cpbw,tel_alpha=,%x,%x,%x,str_len,%x")
TRACE_MSG(ATC_PHONEBOOK_883_112_2_17_22_17_46_759,"ATC: %x %x %x %x %x len %d")
TRACE_MSG(ATC_PHONEBOOK_905_112_2_17_22_17_46_760,"ATC:cpbw,PARA4.str_ptr=%x,%x,%x")
TRACE_MSG(ATC_PHONEBOOK_1383_112_2_17_22_17_47_761,"ATC: ATC_CMD_TYPE_TEST")
TRACE_MSG(ATC_PHONEBOOK_1458_112_2_17_22_17_47_762,"ATC: tel %x, %x, %x, %x")
TRACE_MSG(ATC_PHONEBOOK_1465_112_2_17_22_17_47_763,"ATC: unknown  cscs %s, use IRA")
TRACE_MSG(ATC_PHONEBOOK_1477_112_2_17_22_17_47_764,"ATC: PHONEBOOK_ReadPhonebookEntry read error")
TRACE_MSG(ATC_PHONEBOOK_1486_112_2_17_22_17_47_765,"ATC: ATC_CMD_TYPE_READ")
TRACE_MSG(ATC_PHONEBOOK_1586_112_2_17_22_17_47_766,"ATC:AT+CSVM,error in length of input")
TRACE_MSG(ATC_PHONEBOOK_1605_112_2_17_22_17_47_767,"ATC:ERR_OPERATION_NOT_SUPPORTED")
TRACE_MSG(ATC_PHONEBOOK_1618_112_2_17_22_17_47_768,"ATC:PHONEBOOK_UpdateVoiceMailAddr failed")
TRACE_MSG(ATC_PHONEBOOK_1625_112_2_17_22_17_47_769,"ATC:PHONEBOOK_EnableVoiceMailAddr failed")
TRACE_MSG(ATC_PHONEBOOK_1676_112_2_17_22_17_47_770,"ATC: ATC_CMD_TYPE_READ")
TRACE_MSG(ATC_PHONEBOOK_1685_112_2_17_22_17_47_771,"ATC: ERR_INVALID_INDEX")
TRACE_MSG(ATC_PLUS_GPRS_207_112_2_17_22_17_48_772,"ATGPRS:ATC_ProcessCGDCONT\n")
TRACE_MSG(ATC_PLUS_GPRS_518_112_2_17_22_17_49_773,"ATC: pco user or passwd too long.")
TRACE_MSG(ATC_PLUS_GPRS_531_112_2_17_22_17_49_774,"ATC: pco user or passwd too long.")
TRACE_MSG(ATC_PLUS_GPRS_539_112_2_17_22_17_49_775,"ATC: set pco error, result = %d")
TRACE_MSG(ATC_PLUS_GPRS_545_112_2_17_22_17_49_776,"ATC: set pco error parameter")
TRACE_MSG(ATC_PLUS_GPRS_642_112_2_17_22_17_49_777,"ATGPRS:ATC_ProcessCGQREQ\n")
TRACE_MSG(ATC_PLUS_GPRS_710_112_2_17_22_17_49_778,"ATGPRS:ATC_ProcessCGQREQ param error!\n")
TRACE_MSG(ATC_PLUS_GPRS_812_112_2_17_22_17_49_779,"ATGPRS:ATC_ProcessCGQMIN\n")
TRACE_MSG(ATC_PLUS_GPRS_876_112_2_17_22_17_49_780,"ATGPRS:ATC_ProcessCGQREQ param error!\n")
TRACE_MSG(ATC_PLUS_GPRS_973_112_2_17_22_17_50_781,"ATGPRS:ATC_ProcessCGATT\n")
TRACE_MSG(ATC_PLUS_GPRS_1073_112_2_17_22_17_50_782,"ATGPRS:ATC_ProcessCGACT\n")
TRACE_MSG(ATC_PLUS_GPRS_1285_112_2_17_22_17_50_783,"ATGPRS:ATC_ProcessCGDATA\n")
TRACE_MSG(ATC_PLUS_GPRS_1316_112_2_17_22_17_50_784,"ATGPRS:layer 2 Protocol default?")
TRACE_MSG(ATC_PLUS_GPRS_1359_112_2_17_22_17_50_785,"ATC: AT+CGDATA RequestPdpId pdp_id:%d")
TRACE_MSG(ATC_PLUS_GPRS_1436_112_2_17_22_17_51_786,"ATGPRS: ATC_ProcessCGSMS\n")
TRACE_MSG(ATC_PLUS_GPRS_1486_112_2_17_22_17_51_787,"ATGPRS: CREG value:%d\n")
TRACE_MSG(ATC_PLUS_GPRS_1495_112_2_17_22_17_51_788,"ATGPRS: CREG set value:%d\n")
TRACE_MSG(ATC_PLUS_GPRS_1686_112_2_17_22_17_51_789,"ATGPRS: ATC_ProcessCGEREP\n")
TRACE_MSG(ATC_PLUS_GPRS_1764_112_2_17_22_17_51_790,"ATGPRS: ATC_ProcessSelectServiceType\n")
TRACE_MSG(ATC_PLUS_GPRS_1837_112_2_17_22_17_51_791,"ATGPRS: ATC_ProcessSATT\n")
TRACE_MSG(ATC_PLUS_GPRS_1924_112_2_17_22_17_52_792,"ATGPRS: ATC_ProcessSATT\n")
TRACE_MSG(ATC_PLUS_GPRS_2025_112_2_17_22_17_52_793,"ATC: ATC_ProcessSGPRSDATA, ERROR: MNGPRS_ReadPdpContextState failed.")
TRACE_MSG(ATC_PLUS_GPRS_2048_112_2_17_22_17_52_794,"ATC: ATC_ProcessSGPRSDATA, ERROR: invalid active pdp count=%d.")
TRACE_MSG(ATC_PLUS_GPRS_2063_112_2_17_22_17_52_795,"ATC: ATC_ProcessSGPRSDATA, too long length!")
TRACE_MSG(ATC_PLUS_GPRS_2070_112_2_17_22_17_52_796,"ATGPRS: The GPRS send data len beyond MAX_AT_GPRSDATA_SEND_LEN!\n")
TRACE_MSG(ATC_PLUS_GPRS_2089_112_2_17_22_17_52_797,"ATC: ATC_ProcessSGPRSDATA, send data len=%d,nsapi=%d")
TRACE_MSG(ATC_PLUS_GPRS_2094_112_2_17_22_17_52_798,"ATC: ATC_ProcessSGPRSDATA, MNGPRS_SendDataReq failed!")
TRACE_MSG(ATC_PLUS_GPRS_2111_112_2_17_22_17_52_799,"ATC: ATC_ProcessSGPRSDATA, send data len=%d,nsapi=%d")
TRACE_MSG(ATC_PLUS_GPRS_2116_112_2_17_22_17_52_800,"ATC: ATC_ProcessSGPRSDATA, MNGPRS_SendDataReq failed!")
TRACE_MSG(ATC_PLUS_GPRS_2152_112_2_17_22_17_52_801,"ATGPRS:ATC_ProcessCGEQREQ\n")
TRACE_MSG(ATC_PLUS_GPRS_2321_112_2_17_22_17_52_802,"ATGPRS:ATC_ProcessCGEQREQ param error!\n")
TRACE_MSG(ATC_PLUS_GPRS_2628_112_2_17_22_17_53_803,"ATGPRS:ATC_ProcessCGEQMIN\n")
TRACE_MSG(ATC_PLUS_GPRS_2686_112_2_17_22_17_53_804,"ATC_ProcessDeactPdpContextInd,is deactivate AT pid,pid:%d")
TRACE_MSG(ATC_PLUS_GPRS_2751_112_2_17_22_17_53_805,"ATC: Switch to DATA MODE for PCLink!")
TRACE_MSG(ATC_PLUS_GPRS_2755_112_2_17_22_17_53_806,"ATC: Activate result is NOT Success in PCLink Mode!")
TRACE_MSG(ATC_PLUS_GPRS_2768_112_2_17_22_17_53_807,"ATC:APP_MN_ACTIVATE_PDP_CONTEXT_CNF pid:%d")
TRACE_MSG(ATC_PLUS_GPRS_2797_112_2_17_22_17_53_808,"ATC_ProcessActPdpContextCnf,pid is not AT activated,pid:%d")
TRACE_MSG(ATC_PLUS_GPRS_3002_112_2_17_22_17_54_809,"ATGPRS:ATC_ProcessCGPADDR\n")
TRACE_MSG(ATC_PLUS_GPRS_3038_112_2_17_22_17_54_810,"ATGPRS:ATC_ProcessCGPADDR\n")
TRACE_MSG(ATC_PLUS_GPRS_3180_112_2_17_22_17_54_811,"ATGPRS:ATC_ProcessCGANS\n")
TRACE_MSG(ATC_PLUS_GPRS_3209_112_2_17_22_17_54_812,"layer 2 Protocol:%s")
TRACE_MSG(ATC_PLUS_GPRS_3222_112_2_17_22_17_54_813,"default Layer 2 Protocol")
TRACE_MSG(ATC_PLUS_GPRS_3228_112_2_17_22_17_54_814,"para 3 pdp id:%d")
TRACE_MSG(ATC_PLUS_GPRS_3288_112_2_17_22_17_54_815,"input class name class%s")
TRACE_MSG(MNCALL_AOC_60_112_2_17_22_18_21_816,"MNCALL: AOC the fixed charge is %d")
TRACE_MSG(MNCALL_AOC_98_112_2_17_22_18_21_817,"MNCALL: AOC the interval charge is %d")
TRACE_MSG(MNCALL_AOC_224_112_2_17_22_18_21_818,"MNCALL: AOC the interval duration is %d")
TRACE_MSG(MNCALL_API_601_112_2_17_22_18_23_819,"MNCALL: set bearer capability type = %d")
TRACE_MSG(MNCALL_DTMF_75_112_2_17_22_18_26_820,"MNCALL: DTMF_CheckBufferIsEmpty result = %d")
TRACE_MSG(MNCALL_DTMF_93_112_2_17_22_18_26_821,"MNCALL: DTMF the buffer is full")
TRACE_MSG(MNCALL_DTMF_115_112_2_17_22_18_26_822,"MNCALL: DTMF the insert DTMF item is %d")
TRACE_MSG(MNCALL_DTMF_132_112_2_17_22_18_26_823,"MNCALL: DTMF the insert DTMF stop entry")
TRACE_MSG(MNCALL_DTMF_148_112_2_17_22_18_26_824,"MNCALL: DTMF the remove DTMF item is %d")
TRACE_MSG(MNCALL_DTMF_190_112_2_17_22_18_26_825,"MNCALL: DTMF set status is %d")
TRACE_MSG(MNCALL_DTMF_220_112_2_17_22_18_26_826,"MNCALL: DTMF set STOP status is %d")
TRACE_MSG(MNCALL_DTMF_257_112_2_17_22_18_26_827,"MNCALL: DTMF_ReadStatusBits %d")
TRACE_MSG(MNCALL_DTMF_289_112_2_17_22_18_26_828,"MNCALL: set the DTMF type bit error!\n")
TRACE_MSG(MNCALL_MODULE_1693_112_2_17_22_18_33_829,"MNCALL: the call id: %d")
TRACE_MSG(MNCALL_MODULE_1943_112_2_17_22_18_33_830,"MNCALL: Receive MN up signal call_index invalid")
TRACE_MSG(MNCALL_MODULE_2346_112_2_17_22_18_34_831,"MNCALL::not retry.")
TRACE_MSG(MNCALL_MODULE_2350_112_2_17_22_18_34_832,"MNCALL::start mo retry timer.")
TRACE_MSG(MNCALL_MODULE_2474_112_2_17_22_18_35_833,"MNCALL:call index = %d,is_mt = %d.")
TRACE_MSG(MNCALL_MODULE_2479_112_2_17_22_18_35_834,"MNCALL: ti is %d.")
TRACE_MSG(MNCALL_MODULE_2533_112_2_17_22_18_35_835,"MNCALL:recv disc when mo call retry.")
TRACE_MSG(MNCALL_MODULE_2882_112_2_17_22_18_35_836,"MNCALL:stk_ti = %d.")
TRACE_MSG(MNCALL_MODULE_3113_112_2_17_22_18_36_837,"MNCALL:recv setup when mo call retry.")
TRACE_MSG(MNCALL_MODULE_3179_112_2_17_22_18_36_838,"MNCALL:call wait on.")
TRACE_MSG(MNCALL_MODULE_3859_112_2_17_22_18_37_839,"MN:ignore message too short\n")
TRACE_MSG(MNCALL_MODULE_3863_112_2_17_22_18_37_840,"MN:unknown, unforeseen, erroneous data\n")
TRACE_MSG(MNCALL_MODULE_4037_112_2_17_22_18_38_841,"MNCALL: The DTMF buffer is full")
TRACE_MSG(MNCALL_MODULE_4067_112_2_17_22_18_38_842,"MNCALL: StopDTMFReq status:%d")
TRACE_MSG(MNCALL_MODULE_4170_112_2_17_22_18_38_843,"MNCALL: StartDTMFCnf status:%d")
TRACE_MSG(MNCALL_MODULE_4497_112_2_17_22_18_39_844,"MNCALL: ECT invoke condition is not satisfied!")
TRACE_MSG(MNCALL_MODULE_5507_112_2_17_22_18_41_845,"MNCALL: recv undefine bc")
TRACE_MSG(MNCALL_MODULE_5792_112_2_17_22_18_41_846,"MNCALL: recv unexpected result")
TRACE_MSG(MNCALL_MODULE_5812_112_2_17_22_18_41_847,"MNCALL: the ccbs context is invalid!")
TRACE_MSG(MNCALL_MODULE_5818_112_2_17_22_18_41_848,"MNCALL: call_index is invalid!")
TRACE_MSG(MNCALL_MODULE_5855_112_2_17_22_18_41_849,"MNCALL: ect context is invalid!")
TRACE_MSG(MNCALL_MODULE_5861_112_2_17_22_18_41_850,"MNCALL: call_index is invalid!")
TRACE_MSG(MNCALL_MODULE_5897_112_2_17_22_18_41_851,"MNCALL: the MPTY context is invalid!")
TRACE_MSG(MNCALL_MODULE_5903_112_2_17_22_18_41_852,"MNCALL: call_index is invalid!")
TRACE_MSG(MNCALL_MODULE_6084_112_2_17_22_18_42_853,"MNCALL: recv unexpected error/reject")
TRACE_MSG(MNCALL_MODULE_6109_112_2_17_22_18_42_854,"MNCALL: the ect context is invalid!")
TRACE_MSG(MNCALL_MODULE_6115_112_2_17_22_18_42_855,"MNCALL: call_index is invalid!")
TRACE_MSG(MNCALL_MODULE_6156_112_2_17_22_18_42_856,"MNCALL: the ect context is invalid!")
TRACE_MSG(MNCALL_MODULE_6162_112_2_17_22_18_42_857,"MNCALL: call_index is invalid!")
TRACE_MSG(MNCALL_MODULE_6195_112_2_17_22_18_42_858,"MNCALL: the MPTY context is invalid!")
TRACE_MSG(MNCALL_MODULE_6201_112_2_17_22_18_42_859,"MNCALL: call_index is invalid!")
TRACE_MSG(MNCALL_MODULE_6449_112_2_17_22_18_43_860,"MNCALL: MTPY context error")
TRACE_MSG(MNCALL_MODULE_7623_112_2_17_22_18_45_861,"MNCALL::error! the nv not to support FS.")
TRACE_MSG(MNCALL_MODULE_7691_112_2_17_22_18_45_862,"MNCALL:: start retry timer.")
TRACE_MSG(MNCALL_MODULE_7750_112_2_17_22_18_45_863,"MNCALL:: begin mo call retry.")
TRACE_MSG(APP_MNNV_API_146_112_2_17_22_18_47_864,"MNNV: imeisvn %d")
TRACE_MSG(APP_MNSIM_API_311_112_2_17_22_18_48_865,"MN: not equal ccid len")
TRACE_MSG(APP_MNSIM_API_749_112_2_17_22_18_49_866,"MNPHONE: unknow line 1 call forward flag = 0x%x")
TRACE_MSG(APP_MNSIM_API_764_112_2_17_22_18_49_867,"MNPHONE: unknow line 2 call forward flag = 0x%x")
TRACE_MSG(APP_MNSIM_API_1650_112_2_17_22_18_51_868,"MNSIM_GetSimFileRecordNumEx file_name = %d")
TRACE_MSG(APP_MNSIM_API_1691_112_2_17_22_18_51_869,"MNSIM_GetSimFileRecordEntryEx file_name = %d")
TRACE_MSG(APP_MNSIM_API_1748_112_2_17_22_18_51_870,"MNSIM_GetSimBinaryFileEx file_name = %d")
TRACE_MSG(APP_PHONEBOOK_API_1730_112_2_17_22_18_59_871,"MN: GetSimEntryInList pos = %d")
TRACE_MSG(APP_PHONEBOOK_API_1844_112_2_17_22_18_59_872,"EncodeUcs2AlphaStr result = %d, alpha_type = %d")
TRACE_MSG(APP_PHONEBOOK_API_2048_112_2_17_22_18_59_873,"MN: PHONEBOOK_InsertSimEntry num_len = %d")
TRACE_MSG(APP_PHONEBOOK_API_2323_112_2_17_22_19_0_874,"delete sim book id = %d, use_entry_num = %d")
TRACE_MSG(APP_PHONEBOOK_API_2474_112_2_17_22_19_0_875,"ConvertToPhoneEntry number_len = %d")
TRACE_MSG(APP_PHONEBOOK_API_2531_112_2_17_22_19_0_876,"L4: al %x, %x, %x, %x, entry %x, %x, %x, %x ")
TRACE_MSG(APP_PHONEBOOK_API_3519_112_2_17_22_19_2_877,"MNPHONE: MAIN_GetFreeExtInfo storage = %d")
TRACE_MSG(MNCLASSMARK_API_41_112_2_17_22_19_3_878,"mnclassmark: model type %x")
TRACE_MSG(MNNV_API_100_112_2_17_22_19_4_879,"MnPhone: the value of service type from NV is error!")
TRACE_MSG(MNNV_API_120_112_2_17_22_19_4_880,"MnPhone: the value of preference mode from NV is error!")
TRACE_MSG(MNNV_API_142_112_2_17_22_19_4_881,"MnPhone: the value of roam mode from NV is error!")
TRACE_MSG(MNNV_API_158_112_2_17_22_19_4_882,"MnPhone: user band from NV is error!")
TRACE_MSG(MNNV_API_164_112_2_17_22_19_4_883,"MnPhone: Fail band reading, set Dual Band")
TRACE_MSG(MNNV_API_173_112_2_17_22_19_4_884,"mnnv:RX_EX support is %d")
TRACE_MSG(MNNV_API_174_112_2_17_22_19_4_885,"mnnv:RX_EX egsm thr is %d, val is %d")
TRACE_MSG(MNNV_API_175_112_2_17_22_19_4_886,"mnnv:RX_EX dcs thr is %d, val is %d")
TRACE_MSG(MNNV_API_176_112_2_17_22_19_4_887,"mnnv:RX_EX pcs thr is %d, val is %d")
TRACE_MSG(MNNV_API_177_112_2_17_22_19_4_888,"mnnv:RX_EX gsm850 thr is %d, val is %d")
TRACE_MSG(MNNV_API_329_112_2_17_22_19_5_889,"MN_NV: Assert Failure, DMNV_GetMSBand: user_band %d error.")
TRACE_MSG(MNNV_API_801_112_2_17_22_19_6_890,"MNNV: read nv error result %d\n")
TRACE_MSG(MNNV_API_1067_112_2_17_22_19_6_891,"DMNV:DMNV_GetBaList=%x,%x")
TRACE_MSG(MNNV_API_1089_112_2_17_22_19_6_892,"DMNV:DMNV_SetBaList=%x,%x")
TRACE_MSG(MNSIM_API_277_112_2_17_22_19_8_893,"MN: HPLMN match, %d")
TRACE_MSG(MNSIM_API_1267_112_2_17_22_19_10_894,"MNSIM: get EHPLMN, length is %d, item0 is (%d,%d), item1 is (%d,%d)")
TRACE_MSG(MNSIM_API_1340_112_2_17_22_19_10_895,"DMSIM_IsSmsDownloadSupport result = %d")
TRACE_MSG(MNSIM_API_1366_112_2_17_22_19_10_896,"DMSIM_IsCBDownloadSupport result = %d")
TRACE_MSG(MNSIM_EVENT_370_112_2_17_22_19_11_897,"MNSIM_SetEventRecordReadCnf file_name = %d")
TRACE_MSG(MNSIM_EVENT_412_112_2_17_22_19_11_898,"MNSIM_SetEventRecordReadCnf file_name = %d")
TRACE_MSG(MNSIM_MODULE_770_112_2_17_22_19_13_899,"MNSIM: sim smsp is null.")
TRACE_MSG(MNSIM_MODULE_777_112_2_17_22_19_13_900,"MNSIM: sim smsp is null.")
TRACE_MSG(MNSIM_MODULE_789_112_2_17_22_19_13_901,"MNSIM:img service is support,img record num is %d.")
TRACE_MSG(MNSIM_MODULE_795_112_2_17_22_19_13_902,"MNSIM:img service not support,or img record num is %d.")
TRACE_MSG(MNSIM_MODULE_801_112_2_17_22_19_13_903,"MNSIM:get img record info failed.")
TRACE_MSG(MNSIM_MODULE_835_112_2_17_22_19_14_904,"MN%d: get the EXT2 max have %d records")
TRACE_MSG(MNSIM_MODULE_929_112_2_17_22_19_14_905,"MN%d:SIM LDN ext-- %d %d")
TRACE_MSG(MNSIM_MODULE_960_112_2_17_22_19_14_906,"MN%d:SIM MSISDN ext-- %d %d")
TRACE_MSG(MNSIM_MODULE_1007_112_2_17_22_19_14_907,"MN%d:SIM ADN ext-- %d %d")
TRACE_MSG(MNSIM_MODULE_1036_112_2_17_22_19_14_908,"MN%d:SIM update empty ext2 num %d")
TRACE_MSG(MNSIM_MODULE_1041_112_2_17_22_19_14_909,"MN%d:SIM FDN ext-- %d %d")
TRACE_MSG(MNSIM_MODULE_1199_112_2_17_22_19_14_910,"MN%d:SIM MSISDN EXT overflow %d %d %d")
TRACE_MSG(MNSIM_MODULE_1231_112_2_17_22_19_14_911,"MN%d:SIM MSISDN EXT overflow %d %d %d")
TRACE_MSG(MNSIM_MODULE_1238_112_2_17_22_19_14_912,"MNSIM: HandleSimUpdateDnCnf SIM_DN_T_MSISDN PHONEBOOK_UpdateSimEntry FAIL")
TRACE_MSG(MNSIM_MODULE_1245_112_2_17_22_19_14_913,"MNSIM: HandleSimUpdateDnCnf SIM_DN_T_MSISDN entry_id = %d, dn_id = %d")
TRACE_MSG(MNSIM_MODULE_1279_112_2_17_22_19_14_914,"MN%d:SIM ADN EXT overflow %d %d %d")
TRACE_MSG(MNSIM_MODULE_1285_112_2_17_22_19_14_915,"MNSIM: HandleSimUpdateDnCnf PHONEBOOK_UpdateSimEntry FAIL")
TRACE_MSG(MNSIM_MODULE_1292_112_2_17_22_19_14_916,"MNSIM: HandleSimUpdateDnCnf entry_id = %d, dn_id = %d")
TRACE_MSG(MNSIM_MODULE_1322_112_2_17_22_19_15_917,"MN%d:SIM update empty ext2 num %d")
TRACE_MSG(MNSIM_MODULE_1327_112_2_17_22_19_15_918,"MN%d:SIM FDN EXT overflow %d %d %d")
TRACE_MSG(MNSIM_MODULE_1332_112_2_17_22_19_15_919,"MNSIM: HandleSimUpdateDnCnf SIM_DN_T_FDN PHONEBOOK_UpdateSimEntry FAIL")
TRACE_MSG(MNSIM_MODULE_1339_112_2_17_22_19_15_920,"MNSIM: HandleSimUpdateDnCnf SIM_DN_T_FDN entry_id = %d, dn_id = %d")
TRACE_MSG(MNSIM_MODULE_1350_112_2_17_22_19_15_921,"MNSIM:update dn result %d, type %d, id %d")
TRACE_MSG(MNSIM_MODULE_1389_112_2_17_22_19_15_922,"MNSIM: HandleSimGetFreeExtInfoCnf not right dialling num type\n")
TRACE_MSG(MNSIM_MODULE_1508_112_2_17_22_19_15_923,"MN%d:SIM erase ADN id has EXT,but 0")
TRACE_MSG(MNSIM_MODULE_1551_112_2_17_22_19_15_924,"MN%d:SIM erase ADN id has EXT,but 0")
TRACE_MSG(MNSIM_MODULE_1583_112_2_17_22_19_15_925,"MN%d:SIM erase ADN id has EXT,but 0")
TRACE_MSG(MNSIM_MODULE_1588_112_2_17_22_19_15_926,"MNSIM: HandleSimEraseDnCnf SIM_DN_T_MSISDN PHONEBOOK_DeleteSimEntry FAIL")
TRACE_MSG(MNSIM_MODULE_1595_112_2_17_22_19_15_927,"MNSIM: HandleSimEraseDnCnf SIM_DN_T_MSISDN entry_id = %d, dn_id = %d")
TRACE_MSG(MNSIM_MODULE_1630_112_2_17_22_19_15_928,"MN%d:SIM erase ADN id has EXT,but 0")
TRACE_MSG(MNSIM_MODULE_1635_112_2_17_22_19_15_929,"MNSIM: HandleSimEraseDnCnf SIM_DN_T_ADN PHONEBOOK_DeleteSimEntry FAIL")
TRACE_MSG(MNSIM_MODULE_1642_112_2_17_22_19_15_930,"MNSIM: HandleSimEraseDnCnf SIM_DN_T_ADN entry_id = %d, dn_id = %d")
TRACE_MSG(MNSIM_MODULE_1670_112_2_17_22_19_15_931,"MN%d:SIM update empty ext2 num %d")
TRACE_MSG(MNSIM_MODULE_1675_112_2_17_22_19_15_932,"MNSIM: HandleSimEraseDnCnf SIM_DN_T_FDN PHONEBOOK_DeleteSimEntry FAIL")
TRACE_MSG(MNSIM_MODULE_1682_112_2_17_22_19_15_933,"MNSIM: HandleSimEraseDnCnf SIM_DN_T_FDN entry_id = %d, dn_id = %d")
TRACE_MSG(MNSIM_MODULE_1693_112_2_17_22_19_15_934,"MNSIM:erase dn result %d, type %d, id %d")
TRACE_MSG(MNSIM_MODULE_1740_112_2_17_22_19_15_935,"MN%d:sim HandleFdnServiceCnf recover EXT2 num %d")
TRACE_MSG(MNSIM_MODULE_2919_112_2_17_22_19_18_936,"mnsim_module: AllocateMem max_record_num = 0")
TRACE_MSG(MNSIM_MODULE_3261_112_2_17_22_19_18_937,"MNSIM::refresh ADN should not go this branch! please check!")
TRACE_MSG(MNSIM_MODULE_3262_112_2_17_22_19_18_938,"MNSIM::refresh LND should not go this branch! please check!")
TRACE_MSG(MNSIM_MODULE_3263_112_2_17_22_19_18_939,"MNSIM::refresh MSISDN should not go this branch! please check!")
TRACE_MSG(MNSIM_MODULE_3264_112_2_17_22_19_18_940,"MNSIM::refresh FDN should not go this branch! please check!")
TRACE_MSG(MNSIM_MODULE_3265_112_2_17_22_19_18_941,"MNSIM::refresh SDN should not go this branch! please check!")
TRACE_MSG(MNSIM_MODULE_3266_112_2_17_22_19_18_942,"MNSIM::refresh SMS should not go this branch! please check!")
TRACE_MSG(MNSIM_MODULE_3267_112_2_17_22_19_18_943,"MNSIM::refresh SMSP should not go this branch! please check!")
TRACE_MSG(MNSIM_MODULE_3268_112_2_17_22_19_18_944,"MNSIM::refresh SMSS should not go this branch! please check!")
TRACE_MSG(MNSIM_MODULE_3270_112_2_17_22_19_18_945,"MNSIM::refresh file %d of card%d that not save in mn module.")
TRACE_MSG(MNSIM_MODULE_3334_112_2_17_22_19_18_946,"MNPHONE: the sim file %d is not cared by mn.")
TRACE_MSG(MNSIM_MODULE_3394_112_2_17_22_19_19_947,"MNPHONE: the sim file %d is not cared by mn.")
TRACE_MSG(MNSIM_MODULE_3509_112_2_17_22_19_19_948,"MNSIM: sim(%d) max_img_num = %d")
TRACE_MSG(MNSIM_MODULE_3560_112_2_17_22_19_19_949,"MNSIM:width is %d,height is %d,cs is 0x%x,img_len is %d.")
TRACE_MSG(MNSIM_MODULE_3562_112_2_17_22_19_19_950,"MNSIM: img body:0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x,0x%2x.")
TRACE_MSG(MNSIM_MODULE_3655_112_2_17_22_19_19_951,"MN%d:SIM update empty ext1 num %d")
TRACE_MSG(MNENG_MODULE_474_112_2_17_22_19_25_952,"Command type %d error!")
TRACE_MSG(MNGPRS_API_636_112_2_17_22_19_27_953,"mn:MNGPRS_ReadPdpContextPco fail result = %d")
TRACE_MSG(MNGPRS_API_732_112_2_17_22_19_27_954,"mn:MNGPRS_SetPdpContext fail id = %d")
TRACE_MSG(MNGPRS_API_755_112_2_17_22_19_27_955,"MNGPRS: MAIN_SetPdpContextPco user_ptr = %ld, passwd_ptr = %ld")
TRACE_MSG(MNGPRS_API_768_112_2_17_22_19_27_956,"mngprs:MAIN_SetPdpContextPco invalid auth_type")
TRACE_MSG(MNGPRS_API_777_112_2_17_22_19_27_957,"mn:GPRS_HandleSetPdpContextPcoReq fail result = %d")
TRACE_MSG(MNGPRS_API_836_112_2_17_22_19_28_958,"MNGPRS:MNGPRS_GetPclinkCfg retrun err")
TRACE_MSG(MNGPRS_API_856_112_2_17_22_19_28_959,"mn:GPRS_SendDeactivePdpCnf fail result = %d")
TRACE_MSG(MNGPRS_API_922_112_2_17_22_19_28_960,"MNGPRS:MNGPRS_GetPclinkCfg retrun err!")
TRACE_MSG(MNGPRS_API_950_112_2_17_22_19_28_961,"mn:GPRS_SendPdpActiveCnf fail result = %d")
TRACE_MSG(MNGPRS_API_986_112_2_17_22_19_28_962,"MNGPRS:sim card have been switched")
TRACE_MSG(MNGPRS_API_1040_112_2_17_22_19_28_963,"MNGPRS:MNGPRS_GetPclinkCfg retrun err!")
TRACE_MSG(MNGPRS_API_1059_112_2_17_22_19_28_964,"mn:GPRS_SendPdpActiveCnf fail result = %d")
TRACE_MSG(MNGPRS_API_1077_112_2_17_22_19_28_965,"MNGPRS:the pdp context is still in deactivate pending state,force deactivate it")
TRACE_MSG(MNGPRS_API_1153_112_2_17_22_19_28_966,"mn:MNGPRS_ReadPdpContextAddr fail result = %d")
TRACE_MSG(MNGPRS_API_1460_112_2_17_22_19_29_967,"mn:MNGPRS_ReadCurrQosProfile fail result = %d")
TRACE_MSG(MNGPRS_API_1482_112_2_17_22_19_29_968,"mn:MNGPRS_ReadMinQosProfile fail result = %d")
TRACE_MSG(MNGPRS_DATA_519_112_2_17_22_19_32_969,"MNGPRS:discard signal(%x) because destination task(%d)'s queue is full")
TRACE_MSG(MNGPRS_MODULE_793_112_2_17_22_19_34_970,"MNGPRS:String2PdpAddr result = %d")
TRACE_MSG(MNGPRS_MODULE_822_112_2_17_22_19_34_971,"MNGPRS:PdpAddr2String result = %d")
TRACE_MSG(MNGPRS_MODULE_1410_112_2_17_22_19_36_972,"MNGPRS:Pdp context %d may be active,set error!")
TRACE_MSG(MNGPRS_MODULE_1490_112_2_17_22_19_36_973,"MNGPRS: Get pdp context %d error")
TRACE_MSG(MNGPRS_MODULE_1497_112_2_17_22_19_36_974,"MNGPRS:Pdp context %d may be active,set error!")
TRACE_MSG(MNGPRS_MODULE_1546_112_2_17_22_19_36_975,"MNGPRS:MN_GPRS_ERR_SUCCESS != result")
TRACE_MSG(MNGPRS_MODULE_1616_112_2_17_22_19_36_976,"MNGPRS:Get PDP context not successfully!")
TRACE_MSG(MNGPRS_MODULE_2552_112_2_17_22_19_38_977,"mngprs: s_mngprs_contexts_info[%d][%d].context_data_ptr = %x")
TRACE_MSG(MNGPRS_MODULE_2928_112_2_17_22_19_39_978,"MNGPRS:s_mngprs_nsapi_info[%d][%d].pdp_context is null")
TRACE_MSG(MNGPRS_MODULE_2950_112_2_17_22_19_39_979,"MNGPRS:we do not send PDP DEACTIVATE IND to APP")
TRACE_MSG(MNGPRS_MODULE_3124_112_2_17_22_19_39_980,"mngprs: user pdp login and password length =0")
TRACE_MSG(MNGPRS_MODULE_3566_112_2_17_22_19_40_981,"MNGPRS: unknow configuration protocol %d.")
TRACE_MSG(MNGPRS_MODULE_3579_112_2_17_22_19_40_982,"MNGPRS: protocol_id = 0x%x")
TRACE_MSG(MNGPRS_MODULE_3619_112_2_17_22_19_40_983,"MNGPRS:the Length field error in IPCP packet")
TRACE_MSG(MNGPRS_MODULE_3636_112_2_17_22_19_40_984,"MNGPRS: type = %d")
TRACE_MSG(MNGPRS_NV_109_112_2_17_22_19_41_985,"MNGPRS:GPRS_SetNVPdpContext() invalid pdp_id")
TRACE_MSG(MNGPRS_NV_337_112_2_17_22_19_42_986,"MNGPRS:EFS_NvitemRead return %d")
TRACE_MSG(MNGPRS_NV_363_112_2_17_22_19_42_987,"MNGPRS:EFS_NvitemWrite return %d")
TRACE_MSG(MNGPRS_NV_389_112_2_17_22_19_42_988,"MNGPRS:create nv item for pc link config")
TRACE_MSG(MNGPRS_SIGNAL_174_112_2_17_22_19_42_989,"MNGPRS:tft =%p")
TRACE_MSG(MN_MAIN_949_112_2_17_22_19_48_990,"MN: cann't find the msg %d\n")
TRACE_MSG(MN_MAIN_961_112_2_17_22_19_49_991,"MN receive unexpected signal %x\n")
TRACE_MSG(MN_MAIN_1202_112_2_17_22_19_49_992,"L4_RUN\n")
TRACE_MSG(MNSMS_API_622_112_2_17_22_19_51_993,"MNSMS:MNSMS_SetAssertSmsDestAddr Invalid API!")
TRACE_MSG(MNSMS_API_1666_112_2_17_22_19_53_994,"MNSMS_API: INPUT PTR is NULL!")
TRACE_MSG(MNSMS_API_1698_112_2_17_22_19_53_995,"MNSMS_API:PTR is NULL!")
TRACE_MSG(MNSMS_API_1709_112_2_17_22_19_53_996,"MNSMS::send Command SMS is not allowed for the sc addr is not in the FDN.")
TRACE_MSG(MNSMS_API_1719_112_2_17_22_19_53_997,"MNSMS::send Command SMS is not allowed for the dest addr is not in the FDN.")
TRACE_MSG(MNSMS_API_1782_112_2_17_22_19_54_998,"MNSMS: Error length for STK SMS PDU!!")
TRACE_MSG(MNSMS_API_1859_112_2_17_22_19_54_999,"MNSMS:the stk sms is need compressed.")
TRACE_MSG(MNSMS_API_1870_112_2_17_22_19_54_1000,"MNSMS:the stk sms is need compressed but not compressed.")
TRACE_MSG(MNSMS_API_1885_112_2_17_22_19_54_1001,"MNSMS:the stk sms is not need compressed.")
TRACE_MSG(MNSMS_API_1931_112_2_17_22_19_54_1002,"MNSMS_API: INPUT PTR is NULL!")
TRACE_MSG(MNSMS_API_1990_112_2_17_22_19_54_1003,"MNSMS::send SMS is not allowed for the sc addr is not in the FDN.")
TRACE_MSG(MNSMS_API_2000_112_2_17_22_19_54_1004,"MNSMS::send SMS is not allowed for the dest addr is not in the FDN.")
TRACE_MSG(MNSMS_API_2167_112_2_17_22_19_54_1005,"MNSMS_API:PTR is NULL!")
TRACE_MSG(MNSMS_API_2184_112_2_17_22_19_54_1006,"MNSMS_API:record id is out of range in SIM!")
TRACE_MSG(MNSMS_API_2194_112_2_17_22_19_54_1007,"MNSMS_API:record id is out of range in me!")
TRACE_MSG(MNSMS_API_2203_112_2_17_22_19_54_1008,"MNSMS_API: NOT RIGHT STORAGE")
TRACE_MSG(MNSMS_API_2263_112_2_17_22_19_55_1009,"MNSMS_API:record id is out of range in sim!")
TRACE_MSG(MNSMS_API_2273_112_2_17_22_19_55_1010,"MNSMS_API:record id is out of range in me!")
TRACE_MSG(MNSMS_API_2281_112_2_17_22_19_55_1011,"MNSMS_API: NOT RIGHT STATUS")
TRACE_MSG(MNSMS_API_2341_112_2_17_22_19_55_1012,"MNSMS_API:PTR is NULL!")
TRACE_MSG(MNSMS_API_2351_112_2_17_22_19_55_1013,"MNSMS_API: NOT RIGHT STATUS")
TRACE_MSG(MNSMS_API_2441_112_2_17_22_19_55_1014,"MNSMS_API: NOT RIGHT STATUS")
TRACE_MSG(MNSMS_API_2511_112_2_17_22_19_55_1015,"MNSMS:MAIN_GetHexSmsMo error is %d.")
TRACE_MSG(MNSMS_API_2656_112_2_17_22_19_56_1016,"MNSMS_API: INPUT PTR is NULL!")
TRACE_MSG(MNSMS_API_2683_112_2_17_22_19_56_1017,"MNSMS_API:PTR is NULL!")
TRACE_MSG(MNSMS_API_2759_112_2_17_22_19_56_1018,"MNSMS_API: not right status.")
TRACE_MSG(MNSMS_API_2930_112_2_17_22_19_56_1019,"MNSMS_API:PTR is NULL!")
TRACE_MSG(MNSMS_API_2935_112_2_17_22_19_56_1020,"MNSMS_API:length is out of range")
TRACE_MSG(MNSMS_API_2940_112_2_17_22_19_56_1021,"MNSMS_API: dest addr length is out of range")
TRACE_MSG(MNSMS_API_3141_112_2_17_22_19_56_1022,"MNSMS_API: INVALID PARAMETER.")
TRACE_MSG(MNSMS_API_3391_112_2_17_22_19_57_1023,"MNSMS:MNSMS_SetAssertSmsDestAddr Invalid API!")
TRACE_MSG(MNSMS_API_3669_112_2_17_22_19_58_1024,"MNSMS: status_report is PNULL")
TRACE_MSG(MNSMS_API_3686_112_2_17_22_19_58_1025,"MNSMS: input sc_addr_ptr is PNULL.")
TRACE_MSG(MNSMS_API_3812_112_2_17_22_19_58_1026,"MNSMS_API: INPUT PTR is NULL!")
TRACE_MSG(MNSMS_API_3877_112_2_17_22_19_58_1027,"MNSMS_API: INPUT PTR is NULL!")
TRACE_MSG(MNSMS_API_4276_112_2_17_22_19_59_1028,"MNSMS: MNSMS_SetIsSaveMMSPush is_save=%d")
TRACE_MSG(MNSMS_API_4498_112_2_17_22_19_59_1029,"MNSMS_API: INPUT PTR is NULL!")
TRACE_MSG(MNSMS_API_4532_112_2_17_22_19_59_1030,"MNSMS_API: INPUT PTR is NULL!")
TRACE_MSG(MNSMS_API_4547_112_2_17_22_19_59_1031,"MNSMS_GetSmsParamMaxNumEx service_info(%d %d)")
TRACE_MSG(MNSMS_API_4573_112_2_17_22_20_0_1032,"MNSMS_API: INPUT PTR is NULL!")
TRACE_MSG(MNSMS_EVENT_435_112_2_17_22_20_1_1033,"MNSMS_EVENT:Error msg type in sms callback function!")
TRACE_MSG(MNSMS_MODULE_1729_112_2_17_22_20_4_1034,"MNSMS:replace sms not support.")
TRACE_MSG(MNSMS_MODULE_1733_112_2_17_22_20_4_1035,"MNSMS:replace sms supported.")
TRACE_MSG(MNSMS_MODULE_1839_112_2_17_22_20_5_1036,"MNSMS: The SIM Card is removed.")
TRACE_MSG(MNSMS_MODULE_1931_112_2_17_22_20_5_1037,"MNSMS: SIM phase is phase 1, the last used TP_MR is saved in NV.")
TRACE_MSG(MNSMS_MODULE_1981_112_2_17_22_20_5_1038,"MNSMS: s_sms_sim_info[%d] max_num = %d")
TRACE_MSG(MNSMS_MODULE_2013_112_2_17_22_20_5_1039,"MNSMS: get SMS NUM from SIM cause = %d")
TRACE_MSG(MNSMS_MODULE_2033_112_2_17_22_20_5_1040,"MNSMS: Read SMS states from SIM ERROR!\n")
TRACE_MSG(MNSMS_MODULE_2037_112_2_17_22_20_5_1041,"MNSMS: the number of sms record in the sim is 0.")
TRACE_MSG(MNSMS_MODULE_2048_112_2_17_22_20_5_1042,"MNSMS: Init (%d) ok")
TRACE_MSG(MNSMS_MODULE_2107_112_2_17_22_20_5_1043,"MNSMS: read SMS parameter(%d) from SIM cause = %d")
TRACE_MSG(MNSMS_MODULE_2385_112_2_17_22_20_6_1044,"MNSMS: Init (%d) ok")
TRACE_MSG(MNSMS_MODULE_2426_112_2_17_22_20_6_1045,"MNSMS:Error status in SMS WRITE CNF signal!")
TRACE_MSG(MNSMS_MODULE_2460_112_2_17_22_20_6_1046,"MNSMS memory full status = %d")
TRACE_MSG(MNSMS_MODULE_2569_112_2_17_22_20_6_1047,"MNSMS: sim memory capability has free space again.")
TRACE_MSG(MNSMS_MODULE_2617_112_2_17_22_20_6_1048,"MNSMS: UPDATE_SMS_STATUS_CNF delete SMS in SIM, s_sim_del_all[dual_sys].cur_id is %d\n")
TRACE_MSG(MNSMS_MODULE_2641_112_2_17_22_20_6_1049,"MNSMS: UPDATE_SMS_STATUS_CNF delete sms in SIM, s_sim_del_all[dual_sys].cur_id is %d\n")
TRACE_MSG(MNSMS_MODULE_2676_112_2_17_22_20_6_1050,"MNSMS: delete all SMS in SIM is success\n")
TRACE_MSG(MNSMS_MODULE_2687_112_2_17_22_20_6_1051,"MNSMS:DeleteSmsInSim\n")
TRACE_MSG(MNSMS_MODULE_2991_112_2_17_22_20_7_1052,"MNSMS: HandleAppMnReadSmsReq not right storage")
TRACE_MSG(MNSMS_MODULE_3043_112_2_17_22_20_7_1053,"MNSMS: read NV SMS fail, No. is %d")
TRACE_MSG(MNSMS_MODULE_3173_112_2_17_22_20_7_1054,"MNSMS: start delete all sms in sim\n")
TRACE_MSG(MNSMS_MODULE_3300_112_2_17_22_20_7_1055,"MNSMS: read NV SMS fail, No. is %d")
TRACE_MSG(MNSMS_MODULE_3366_112_2_17_22_20_8_1056,"MNSMS: HandleAppMnWriteSmsReq not right sm status(%d)")
TRACE_MSG(MNSMS_MODULE_3449_112_2_17_22_20_8_1057,"MNSMS: HandleAppMnWriteSmsReq not right storage")
TRACE_MSG(MNSMS_MODULE_3494_112_2_17_22_20_8_1058,"MNSMS: write SMS to SIM.")
TRACE_MSG(MNSMS_MODULE_3516_112_2_17_22_20_8_1059,"MNSMS: read NV SMS fail, No. is %d")
TRACE_MSG(MNSMS_MODULE_3651_112_2_17_22_20_8_1060,"MNSMS: read NV SMS fail, No. is %d")
TRACE_MSG(MNSMS_MODULE_3691_112_2_17_22_20_8_1061,"MNSMS: recv SMS_DELIVER.")
TRACE_MSG(MNSMS_MODULE_3695_112_2_17_22_20_8_1062,"MNSMS: recv SMS_STATE_REPORT.")
TRACE_MSG(MNSMS_MODULE_3699_112_2_17_22_20_8_1063,"MNSMS: MT procedure is failure.")
TRACE_MSG(MNSMS_MODULE_3969_112_2_17_22_20_9_1064,"MNSMS: waiting for set receive sms cnf\n")
TRACE_MSG(MNSMS_MODULE_3976_112_2_17_22_20_9_1065,"MNSMS: set receive sms error!\n")
TRACE_MSG(MNSMS_MODULE_3982_112_2_17_22_20_9_1066,"MNSMS: set not receive sms\n")
TRACE_MSG(MNSMS_MODULE_4101_112_2_17_22_20_9_1067,"MNSMS: MO SMS operation is OK.\n")
TRACE_MSG(MNSMS_MODULE_4129_112_2_17_22_20_9_1068,"MNSMS: Send SMS is not success.\n")
TRACE_MSG(MNSMS_MODULE_4161_112_2_17_22_20_9_1069,"MNSMS: the fail cause type is %d,cause value is 0x%x")
TRACE_MSG(MNSMS_MODULE_4298_112_2_17_22_20_9_1070,"MNSMS: SetVoimsgInd() decode character1 error in CPHS\n")
TRACE_MSG(MNSMS_MODULE_4306_112_2_17_22_20_9_1071,"MNSMS: SetVoimsgInd() decode character2 error in CPHS\n")
TRACE_MSG(MNSMS_MODULE_4339_112_2_17_22_20_10_1072,"MNSMS:receive a voice message waiting message need discard.")
TRACE_MSG(MNSMS_MODULE_4383_112_2_17_22_20_10_1073,"MNSMS: MT Short Message procedure is failure. Error = %lx")
TRACE_MSG(MNSMS_MODULE_4390_112_2_17_22_20_10_1074,"MNSMS:receive SMS PP download data\n!")
TRACE_MSG(MNSMS_MODULE_4407_112_2_17_22_20_10_1075,"MNSMS:receive a sms with type 0, discard it.")
TRACE_MSG(MNSMS_MODULE_4439_112_2_17_22_20_10_1076,"MNSMS: replace SMS storage(%d) #%d")
TRACE_MSG(MNSMS_MODULE_4463_112_2_17_22_20_10_1077,"MNSMS: dont need replace SMS, Normal receive SMS")
TRACE_MSG(MNSMS_MODULE_4478_112_2_17_22_20_10_1078,"MNSMS: Receive a discarded message, msg_is_waiting = %d")
TRACE_MSG(MNSMS_MODULE_4484_112_2_17_22_20_10_1079,"MNSMS: deal sms voicemessage indicator error\n")
TRACE_MSG(MNSMS_MODULE_4519_112_2_17_22_20_10_1080,"MNSMS: DealSmsVoimsgInd line error\n")
TRACE_MSG(MNSMS_MODULE_4533_112_2_17_22_20_10_1081,"MNSMS :  DealSmsVoimsgInd need not to update sim files.")
TRACE_MSG(MNSMS_MODULE_4537_112_2_17_22_20_10_1082,"MNSMS: DealSmsVoimsgInd wait type error\n")
TRACE_MSG(MNSMS_MODULE_4546_112_2_17_22_20_10_1083,"MNSMS: save VMW flag error\n")
TRACE_MSG(MNSMS_MODULE_4551_112_2_17_22_20_10_1084,"MNSMS: save VMW flag success\n")
TRACE_MSG(MNSMS_MODULE_4620_112_2_17_22_20_10_1085,"MNSMS: input sc_addr_ptr is PNULL.")
TRACE_MSG(MNSMS_MODULE_4671_112_2_17_22_20_10_1086,"MNSMS: MNSMS_SaveStatusReportInME: dual_sys = %d, position = %d")
TRACE_MSG(MNSMS_MODULE_4740_112_2_17_22_20_10_1087,"SMS: DealStatusReport dual_sys = %d, storage = %d, position = %d")
TRACE_MSG(MNSMS_MODULE_4813_112_2_17_22_20_10_1088,"MNSMS: MT Short Message procedure is failure. Error = %lx")
TRACE_MSG(MNSMS_MODULE_4871_112_2_17_22_20_11_1089,"MNSMS: MT procedure failure for Decode Status Report Error.")
TRACE_MSG(MNSMS_MODULE_5147_112_2_17_22_20_11_1090,"MNSMS: deal sms voicemessage indicator error\n")
TRACE_MSG(MNSMS_MODULE_5189_112_2_17_22_20_11_1091,"MNSMS memory full status = %d")
TRACE_MSG(MNSMS_MODULE_5317_112_2_17_22_20_11_1092,"MNSMS: mms notification recv.")
TRACE_MSG(MNSMS_MODULE_5334_112_2_17_22_20_11_1093,"MNSMS: to save the MMS notification.")
TRACE_MSG(MNSMS_MODULE_5360_112_2_17_22_20_12_1094,"MNSMS: MT Short Message Procedure is success.\n")
TRACE_MSG(MNSMS_MODULE_5380_112_2_17_22_20_12_1095,"MNSMS: DealMtSmsWithClass not right class")
TRACE_MSG(MNSMS_MODULE_5402_112_2_17_22_20_12_1096,"MNSMS memory full status = %d")
TRACE_MSG(MNSMS_MODULE_5422_112_2_17_22_20_12_1097,"MNSMS: save MT Short Message to ME")
TRACE_MSG(MNSMS_MODULE_5459_112_2_17_22_20_12_1098,"MNSMS: malloc s_mt_sms_ptr[dual_sys] error.\n")
TRACE_MSG(MNSMS_MODULE_5511_112_2_17_22_20_12_1099,"MNSMS: deal SMS VOICEMESSAGE indicator error\n")
TRACE_MSG(MNSMS_MODULE_5522_112_2_17_22_20_12_1100,"MNSMS: MT Short Message procedure is failure. Error = %lx")
TRACE_MSG(MNSMS_MODULE_5601_112_2_17_22_20_12_1101,"MNSMS: NVSMS_REFRESH_REPLACE_INFO_CNF result = %d")
TRACE_MSG(MNSMS_MODULE_5750_112_2_17_22_20_12_1102,"MNSMS: delete all SMS status not right\n")
TRACE_MSG(MNSMS_MODULE_5811_112_2_17_22_20_12_1103,"MNSMS: status_is_correct is FALSE")
TRACE_MSG(MNSMS_MODULE_5867_112_2_17_22_20_13_1104,"MNSMS: writeMoMsg not need save.")
TRACE_MSG(MNSMS_MODULE_5937_112_2_17_22_20_13_1105,"MNSMS: write tp_mr to NV now")
TRACE_MSG(MNSMS_MODULE_6287_112_2_17_22_20_13_1106,"MNSMS: there is no TP-Validity-Period\n")
TRACE_MSG(MNSMS_MODULE_6565_112_2_17_22_20_14_1107,"MNSMS: the alphabet of Data Coding Scheme is error!")
TRACE_MSG(MNSMS_MODULE_6569_112_2_17_22_20_14_1108,"MNSMS: SMS_EncodeUserData user data head length is %d,data length is %d.")
TRACE_MSG(MNSMS_MODULE_6618_112_2_17_22_20_14_1109,"MNSMS:APP send SMS command to SC with TP_MR = %d")
TRACE_MSG(MNSMS_MODULE_6633_112_2_17_22_20_14_1110,"MNSMS: SMS_PackSmsCommand not right command_type_e.")
TRACE_MSG(MNSMS_MODULE_6741_112_2_17_22_20_14_1111,"MNSMS_API:sms length out of range!")
TRACE_MSG(MNSMS_MODULE_6998_112_2_17_22_20_15_1112,"MNSMS: called number length is out of the range")
TRACE_MSG(MNSMS_MODULE_7052_112_2_17_22_20_15_1113,"MNSMS: called number length is out of the range")
TRACE_MSG(MNSMS_MODULE_7332_112_2_17_22_20_16_1114,"MNSMS: !!! SMS DEFAULT ALPHABET: the user data length is longer than the tpdu contained!")
TRACE_MSG(MNSMS_MODULE_7337_112_2_17_22_20_16_1115,"MNSMS: !!! NOT DEFAULT ALPHABET:the user data length is longer than the tpdu contained!")
TRACE_MSG(MNSMS_MODULE_7364_112_2_17_22_20_16_1116,"MNSMS:the msg type is 0.")
TRACE_MSG(MNSMS_MODULE_7407_112_2_17_22_20_16_1117,"MNSMS: decode length error\n")
TRACE_MSG(MNSMS_MODULE_7429_112_2_17_22_20_16_1118,"MNSMS: UnPackStatusReport success! tpdu_ptr->l3_msg[%d] is %dn")
TRACE_MSG(MNSMS_MODULE_7433_112_2_17_22_20_16_1119,"MNSMS: decode TP called num error\n")
TRACE_MSG(MNSMS_MODULE_7514_112_2_17_22_20_16_1120,"MNSMS:UnPackMoTpduInSim, is deliver msg.")
TRACE_MSG(MNSMS_MODULE_7519_112_2_17_22_20_16_1121,"MNSMS:UnPackMoTpduInSim, is status report msg.")
TRACE_MSG(MNSMS_MODULE_7545_112_2_17_22_20_16_1122,"MNSMS:UnPackMoTpduInSim, is submit msg.")
TRACE_MSG(MNSMS_MODULE_7548_112_2_17_22_20_16_1123,"MNSMS:UnPackMoTpduInSim, error msg type.")
TRACE_MSG(MNSMS_MODULE_7589_112_2_17_22_20_16_1124,"MNSMS: Read SMS_DELIVER Message.")
TRACE_MSG(MNSMS_MODULE_7607_112_2_17_22_20_16_1125,"MNSMS: Read SMS_STATE_REPORT Message.")
TRACE_MSG(MNSMS_MODULE_7626_112_2_17_22_20_16_1126,"MNSMS:UnPackMtTpduInSim, submit msg.")
TRACE_MSG(MNSMS_MODULE_7644_112_2_17_22_20_16_1127,"MNSMS:UnPackMtTpduInSim error msg type.")
TRACE_MSG(MNSMS_MODULE_7707_112_2_17_22_20_17_1128,"MNSMS: UnPack MO TPDU, TP_MR = %d")
TRACE_MSG(MNSMS_MODULE_7726_112_2_17_22_20_17_1129,"MNSMS: UnPackMoTpduInMe DecodeDcs result = %d")
TRACE_MSG(MNSMS_MODULE_7800_112_2_17_22_20_17_1130,"MNSMS: Read SMS_DELIVER Message.")
TRACE_MSG(MNSMS_MODULE_7810_112_2_17_22_20_17_1131,"MNSMS: Read SMS_STATE_REPORT Message.")
TRACE_MSG(MNSMS_MODULE_7823_112_2_17_22_20_17_1132,"MNSMS: Read Error type message of MT SMS.")
TRACE_MSG(MNSMS_MODULE_8013_112_2_17_22_20_18_1133,"MNSMS: decode DCS unexcepted value")
TRACE_MSG(MNSMS_MODULE_8072_112_2_17_22_20_18_1134,"MNSMS: there is no TP-Validity-Period\n")
TRACE_MSG(MNSMS_MODULE_8203_112_2_17_22_20_18_1135,"MNSMS: input user head is invalid!")
TRACE_MSG(MNSMS_MODULE_8924_112_2_17_22_20_21_1136,"MNSMS: unknown user head type!")
TRACE_MSG(MNSMS_MODULE_9350_112_2_17_22_20_22_1137,"MNSMS:no space in SIM or ME.")
TRACE_MSG(MNSMS_MODULE_9408_112_2_17_22_20_22_1138,"MNSMS: GetRefAndMaxNumFromHead iei = %d")
TRACE_MSG(MNSMS_MODULE_9453_112_2_17_22_20_22_1139,"MNSMS: SMS_CalculateSmsNumber SMS num in me is %d")
TRACE_MSG(MNSMS_MODULE_9513_112_2_17_22_20_22_1140,"MNSMS: IsMemAvail is_sim_full = %d, is_me_full = %d, mem_status = %d")
TRACE_MSG(MNSMS_MODULE_9761_112_2_17_22_20_22_1141,"MNSMS: Compare Replace Header result is %d")
TRACE_MSG(MNSMS_MODULE_9868_112_2_17_22_20_23_1142,"MNSMS: replace sms to SIM.")
TRACE_MSG(MNSMS_MODULE_9922_112_2_17_22_20_23_1143,"MNSMS: MT replace Short Message Procedure is success.")
TRACE_MSG(MNSMS_MODULE_9927_112_2_17_22_20_23_1144,"MNSMS: deal SMS VOICEMESSAGE indicator error\n")
TRACE_MSG(MNSMS_MODULE_10027_112_2_17_22_20_23_1145,"MNSMS: deal sms voicemessage indicator error\n")
TRACE_MSG(MNSMS_MODULE_10034_112_2_17_22_20_23_1146,"MNSMS: replace sms error!\n")
TRACE_MSG(MNSMS_MODULE_10162_112_2_17_22_20_23_1147,"MNSMS: write replace SMS flag from %d to %d")
TRACE_MSG(MNSMS_MODULE_10183_112_2_17_22_20_23_1148,"MNSMS: write SMS replace support flag fail, write NV fail")
TRACE_MSG(MNSMS_MODULE_10189_112_2_17_22_20_23_1149,"MNSMS: write sms replace support flag fail, sim not ready")
TRACE_MSG(MNSMS_MODULE_10259_112_2_17_22_20_23_1150,"MNSMS: NVSMS_INIT_CNF result = %d")
TRACE_MSG(MNSMS_MODULE_10335_112_2_17_22_20_24_1151,"MNSMS: write SMS to NV, TP_MR=%d.")
TRACE_MSG(MNSMS_MODULE_10534_112_2_17_22_20_24_1152,"MNSMS: decrease the hold num of card %d to %d.")
TRACE_MSG(MNSMS_MODULE_10585_112_2_17_22_20_24_1153,"MNSMS: SMS_DecodeSTKMoTpduData DecodeDcs result = %d")
TRACE_MSG(MNSMS_MODULE_10598_112_2_17_22_20_24_1154,"MNSMS:the dcs from the SIM card is error.")
TRACE_MSG(MNSMS_MODULE_10608_112_2_17_22_20_24_1155,"MNSMS:SMS_DecodeSTKMoTpduData the stk dcs is 7bit and data len is %d.")
TRACE_MSG(MNSMS_MODULE_10620_112_2_17_22_20_24_1156,"MNSMS:modify the stk dcs from 7bit to 8bit.")
TRACE_MSG(MNSMS_MODULE_10708_112_2_17_22_20_24_1157,"MNSMS: SMS_DecodeSTKUserData user_data_length is %d")
TRACE_MSG(MNSMS_MODULE_10716_112_2_17_22_20_24_1158,"MNSMS: user_data_header_ptr_t->length is %d")
TRACE_MSG(MNSMS_MODULE_10755_112_2_17_22_20_24_1159,"MNSMS: SMS_DecodeSTKUserData sms_length is %d")
TRACE_MSG(MNSMS_MODULE_10779_112_2_17_22_20_24_1160,"MNSMS:no user header in the stk sms user data.")
TRACE_MSG(MNSMS_MODULE_10786_112_2_17_22_20_25_1161,"MNSMS:there is user header in the stk sms user data.")
TRACE_MSG(MNSMS_MODULE_10811_112_2_17_22_20_25_1162,"MNSMS: SMS_DecodeSTKUserData user_valid_data_ptr_t->length is %d")
TRACE_MSG(MNSMS_MODULE_11068_112_2_17_22_20_25_1163,"MNSMS: SMS_UnPackSmsCommand not right command_type_e.")
TRACE_MSG(MNSMS_MODULE_11076_112_2_17_22_20_25_1164,"MNSMS: SMS_UnPackSmsCommand not right command_type_e.")
TRACE_MSG(MNSMS_MODULE_11098_112_2_17_22_20_25_1165,"MNSMS: sim(%d) max_smsp_num = %d")
TRACE_MSG(MNSMS_MODULE_11136_112_2_17_22_20_25_1166,"SMS_GetSimParamDefaultId max_num = %d, default_id = %d")
TRACE_MSG(MNSMS_MODULE_11347_112_2_17_22_20_26_1167,"SMS: undefined sms_local_cause(0x%x)")
TRACE_MSG(MNSMS_NV_MODULE_723_112_2_17_22_20_27_1168,"MNSMS: get s_sms_nv_info.max_num is %d")
TRACE_MSG(MNSMS_NV_MODULE_830_112_2_17_22_20_28_1169,"MNSMS: write NV SMS, result = %d, rid = %d")
TRACE_MSG(MNSMS_NV_MODULE_848_112_2_17_22_20_28_1170,"MNSMS: NVSMS_ReadNvSmsDataReq(%d, %d)")
TRACE_MSG(MNSMS_NV_MODULE_1333_112_2_17_22_20_29_1171,"MNSMS: NVSMS_SaveStatusReportReq dual_sys = %d")
TRACE_MSG(MNSMS_NV_MODULE_1755_112_2_17_22_20_30_1172,"MNSMS: delete all SMS in ME result = %d")
TRACE_MSG(MNSMS_NV_MODULE_1774_112_2_17_22_20_30_1173,"MNSMS: NVSMS_IsMemFull used = %d, max = %d")
TRACE_MSG(MNSMS_NV_MODULE_1851_112_2_17_22_20_30_1174,"MNSMS: NVSMS_ReadNvSmsNumInfo(%d, %d)")
TRACE_MSG(MNSMSCB_MODULE_429_112_2_17_22_20_33_1175,"SMSCB: Initial CB service according to parameters requested by user ")
TRACE_MSG(MNSMSCB_MODULE_568_112_2_17_22_20_33_1176,"SMSCB:msg id %d is not new added.")
TRACE_MSG(MNSMSCB_MODULE_669_112_2_17_22_20_33_1177,"MNSMSCB NextState: Block1 pending.")
TRACE_MSG(MNSMSCB_MODULE_674_112_2_17_22_20_33_1178,"MNSMSCB NextState: Block2 pending.")
TRACE_MSG(MNSMSCB_MODULE_689_112_2_17_22_20_33_1179,"MNSMSCB NextState: Schedule Block1 pending.")
TRACE_MSG(MNSMSCB_MODULE_694_112_2_17_22_20_33_1180,"MNSMSCB NextState: Schedule Block2 pending.")
TRACE_MSG(MNSMSCB_MODULE_702_112_2_17_22_20_33_1181,"MNSMSCB: Discard one block in Block1 pending state.")
TRACE_MSG(MNSMSCB_MODULE_745_112_2_17_22_20_33_1182,"MNSMSCB NextState: Block1 pending.")
TRACE_MSG(MNSMSCB_MODULE_760_112_2_17_22_20_33_1183,"MNSMSCB NextState: Block1 pending.")
TRACE_MSG(MNSMSCB_MODULE_773_112_2_17_22_20_33_1184,"MNSMSCB NextState: Block1 pending.")
TRACE_MSG(MNSMSCB_MODULE_778_112_2_17_22_20_33_1185,"MNSMSCB NextState: Block2 pending.")
TRACE_MSG(MNSMSCB_MODULE_793_112_2_17_22_20_33_1186,"MNSMSCB NextState: Schedule Block1 pending.")
TRACE_MSG(MNSMSCB_MODULE_798_112_2_17_22_20_33_1187,"MNSMSCB NextState: Schedule Block2 pending.")
TRACE_MSG(MNSMSCB_MODULE_1145_112_2_17_22_20_34_1188,"MNSMSCB::enter IsNewSmscbMsg.")
TRACE_MSG(MNSMSCB_MODULE_1161_112_2_17_22_20_34_1189,"MNSMSCB:: class 3 msg,receive it.")
TRACE_MSG(MNSMSCB_MODULE_1181_112_2_17_22_20_34_1190,"MNSMSCB::plmn wide,msg code is same,update number is valid,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1187_112_2_17_22_20_34_1191,"MNSMSCB::plmn wide,msg code is same,receive in different plmn,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1204_112_2_17_22_20_34_1192,"MNSMSCB::lac wide,msg code is same,receive in same lac,but update number is valid,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1210_112_2_17_22_20_34_1193,"MNSMSCB::lac wide,msg code is same,receive in different lac,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1217_112_2_17_22_20_34_1194,"MNSMSCB::lac wide,msg code is same,receive in different plmn,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1238_112_2_17_22_20_34_1195,"MNSMSCB::cell wide,msg code is same,receive in same cell,update number is valid,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1244_112_2_17_22_20_34_1196,"MNSMSCB::cell wide,msg code is same,receive in different cell,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1250_112_2_17_22_20_34_1197,"MNSMSCB::cell wide,msg code is same,receive in different lac,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1257_112_2_17_22_20_34_1198,"MNSMSCB::cell wide,msg code is same,receive in different plmn,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1263_112_2_17_22_20_34_1199,"SMSCB: same msg-code,discard this message for geo scope or update number.")
TRACE_MSG(MNSMSCB_MODULE_1269_112_2_17_22_20_34_1200,"SMSCB: new msg-code,receive this message.")
TRACE_MSG(MNSMSCB_MODULE_1274_112_2_17_22_20_34_1201,"SMSCB:it is not interested msg id,discard this message.")
TRACE_MSG(MNSMSCB_MODULE_1298_112_2_17_22_20_35_1202,"cb msg id %d is download id")
TRACE_MSG(MNSMSCB_MODULE_1303_112_2_17_22_20_35_1203,"cb msg id %d is not download id")
TRACE_MSG(MNSMSCB_MODULE_1437_112_2_17_22_20_35_1204," SMSCB:Ignore one stk download CB message which message has been downloaded.")
TRACE_MSG(MNSMSCB_MODULE_1446_112_2_17_22_20_35_1205," SMSCB:Ignore one CB message which language ID isn't supported by user")
TRACE_MSG(MNSMSCB_MODULE_1466_112_2_17_22_20_35_1206,"SMSCB:Deal CB message: Total Page = %d,Current Page = %d,msg_code = %x,msg_id=%x,dcs=%x")
TRACE_MSG(MNSMSCB_MODULE_1480_112_2_17_22_20_35_1207,"SMSCB:receive one page sms cb msg.")
TRACE_MSG(MNSMSCB_MODULE_1535_112_2_17_22_20_35_1208,"SMSCB:receive %d page sms cb msg.")
TRACE_MSG(MNSMSCB_MODULE_1642_112_2_17_22_20_35_1209,"SMSCB:GET_BIT=1,i=%d")
TRACE_MSG(MNSMSCB_MODULE_1646_112_2_17_22_20_35_1210,"SMSCB:MDT=1,offset_octets=%d")
TRACE_MSG(MNSMSCB_MODULE_1663_112_2_17_22_20_35_1211,"SMSCB:MDT=0,offset_octets=%d")
TRACE_MSG(MNSMSCB_MODULE_1696_112_2_17_22_20_35_1212,"SMSCB:msg_id=%x,marker_ptr=%x%x%x%x%x%x%x%x%x%x%x%x")
TRACE_MSG(MNSMSCB_MODULE_1787_112_2_17_22_20_35_1213,"MNSMSCB:: class 3 msg,receive it.")
TRACE_MSG(MNSMSCB_MODULE_1807_112_2_17_22_20_36_1214,"MNSMSCB::plmn wide,msg code is same,update number is valid,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1814_112_2_17_22_20_36_1215,"MNSMSCB::plmn wide,msg code is same,receive in different plmn,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1831_112_2_17_22_20_36_1216,"MNSMSCB::lac wide,msg code is same,update number is valid,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1837_112_2_17_22_20_36_1217,"MNSMSCB::lac wide,msg code is same,receive in different lac,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1844_112_2_17_22_20_36_1218,"MNSMSCB::lac wide,msg code is same,receive in different plmn,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1865_112_2_17_22_20_36_1219,"MNSMSCB::cell wide,msg code is same,update num is valid,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1871_112_2_17_22_20_36_1220,"MNSMSCB::cell wide,msg code is same,receive in different cell,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1877_112_2_17_22_20_36_1221,"MNSMSCB::cell wide,msg code is same,receive in different lac,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1884_112_2_17_22_20_36_1222,"MNSMSCB::cell wide,msg code is same,receive in different plmn,so receive the msg.")
TRACE_MSG(MNSMSCB_MODULE_1891_112_2_17_22_20_36_1223,"SMSCB: new msg,so update msg marker.")
TRACE_MSG(MNSMSCB_MODULE_1905_112_2_17_22_20_36_1224,"SMSCB:a received msg,so not update msg marker.")
TRACE_MSG(MNSMSCB_MODULE_1929_112_2_17_22_20_36_1225,"SMSCB: new msg code,so update msg marker.")
TRACE_MSG(MNSMSCB_MODULE_1933_112_2_17_22_20_36_1226,"SMSCB:msg id is not interested,so not update msg marker.")
TRACE_MSG(MNSMSCB_MODULE_1989_112_2_17_22_20_36_1227,"SMSCB: old update num is %d,new update number is %d,update number is valid.")
TRACE_MSG(MNSMSCB_MODULE_1995_112_2_17_22_20_36_1228,"SMSCB: old update num is %d,new update number is %d,update number is invalid.")
TRACE_MSG(MNSS_API_233_112_2_17_22_20_37_1229,"MNSS:string len beyond the max len 255!")
TRACE_MSG(MNSS_API_313_112_2_17_22_20_37_1230,"MNSS::encode ss string failed.")
TRACE_MSG(MNSS_API_478_112_2_17_22_20_38_1231,"MNSS:adjust the fill bit to 0x1a.")
TRACE_MSG(MNSS_API_665_112_2_17_22_20_38_1232,"MNSS_API:forward number length is invalid!")
TRACE_MSG(MNSS_API_699_112_2_17_22_20_38_1233,"MNSS::register ss control by SIM.")
TRACE_MSG(MNSS_API_742_112_2_17_22_20_38_1234,"MNSS_API: The ss-code can not be operated by register.\n")
TRACE_MSG(MNSS_API_822_112_2_17_22_20_38_1235,"MNSS: The ss-code can not be operated by erase.\n")
TRACE_MSG(MNSS_API_903_112_2_17_22_20_38_1236,"MNSS: The ss-code can not be operated by activate.\n")
TRACE_MSG(MNSS_API_985_112_2_17_22_20_39_1237,"MNSS: The ss-code can not be operated by deactivation.\n")
TRACE_MSG(MNSS_API_1148_112_2_17_22_20_39_1238,"MNSS: the input password is not numeric")
TRACE_MSG(MNSS_API_1242_112_2_17_22_20_39_1239,"MNSS_API: Str length is invalid!")
TRACE_MSG(MNSS_API_1249_112_2_17_22_20_39_1240,"MNSS_API: input str_len is %d, out of range!")
TRACE_MSG(MNSS_API_1286_112_2_17_22_20_39_1241,"MNSS_API: Str length is invalid!")
TRACE_MSG(MNSS_API_1299_112_2_17_22_20_39_1242,"MNSS_API: input str_len is %d, out of range!")
TRACE_MSG(MNSS_API_1336_112_2_17_22_20_39_1243,"MNSS:string len beyond the max len 255!")
TRACE_MSG(MNSS_API_1464_112_2_17_22_20_40_1244,"MNSS::encode ss string failed.")
TRACE_MSG(MNSS_API_1652_112_2_17_22_20_40_1245,"MNSS::ss is prevented by FDN.")
TRACE_MSG(MNSS_API_1672_112_2_17_22_20_40_1246,"MNSS:adjust the fill bit to 0x1a.")
TRACE_MSG(MNSS_API_1781_112_2_17_22_20_40_1247,"MNSS: ccbs_index=%d is wrong.\n")
TRACE_MSG(MNSS_API_1926_112_2_17_22_20_40_1248,"MNSS::SendSSControlResultToAPP operate_type is error!")
TRACE_MSG(MNSS_API_1958_112_2_17_22_20_41_1249,"MNSS::encode ss string failed.")
TRACE_MSG(MNSS_API_1980_112_2_17_22_20_41_1250,"MNSS::ss is prevented by FDN.")
TRACE_MSG(MNSS_MODULE_626_112_2_17_22_20_44_1251,"DecodeSSDigitInfo:point is null")
TRACE_MSG(MNSS_MODULE_659_112_2_17_22_20_44_1252,"DecodeSSStringInfo:point is null")
TRACE_MSG(MNSS_MODULE_943_112_2_17_22_20_44_1253,"MNSS:error length = 0\n")
TRACE_MSG(MNSS_MODULE_1282_112_2_17_22_20_45_1254,"Input error")
TRACE_MSG(MNSS_MODULE_1431_112_2_17_22_20_45_1255,"MNSS: Idle_HandleBegineInd diagnose_type = %d")
TRACE_MSG(MNSS_MODULE_1462_112_2_17_22_20_45_1256,"other_cause = %d")
TRACE_MSG(MNSS_MODULE_2131_112_2_17_22_20_47_1257,"MNSS: received MNSS_BEGIN_IND while mnss in active state.\n")
TRACE_MSG(MNSS_MODULE_2162_112_2_17_22_20_47_1258,"MNSS: send out release complete for reject new transaction \n")
TRACE_MSG(MNSS_MODULE_2190_112_2_17_22_20_47_1259,"MNSS: Active_HandleFacilityMsg diagnose_type = %d")
TRACE_MSG(MNSS_MODULE_2211_112_2_17_22_20_47_1260,"other_cause = %d")
TRACE_MSG(MNSS_MODULE_2254_112_2_17_22_20_47_1261,"MNSS: resend ussd through Process_ussd_data operation.\n")
TRACE_MSG(MNSS_MODULE_2275_112_2_17_22_20_47_1262,"MNSS: release complete received but component type is Invoke.\n")
TRACE_MSG(MNSS_MODULE_2301_112_2_17_22_20_47_1263,"MNSS: release complete received but result component decode error.\n")
TRACE_MSG(MNSS_MODULE_2309_112_2_17_22_20_47_1264,"MNSS: release complete received, operation is reject by network.\n")
TRACE_MSG(MNSS_MODULE_2324_112_2_17_22_20_47_1265,"MNSS:Active_HandleEndMsg(), component type error!")
TRACE_MSG(MNSS_MODULE_2333_112_2_17_22_20_47_1266,"MNSS: release complete received decode facility ie ERROR!")
TRACE_MSG(MNSS_MODULE_2342_112_2_17_22_20_47_1267,"MNSS: release complete received but no facility ie included in message.\n")
TRACE_MSG(MNSS_MODULE_2352_112_2_17_22_20_47_1268,"MNSS:notify MMI of the result of sending ussd data.")
TRACE_MSG(MNSS_MODULE_2373_112_2_17_22_20_47_1269,"MNSS: ss failed for mm connection release or error.")
TRACE_MSG(MNSS_MODULE_2378_112_2_17_22_20_47_1270,"MNSS:ss service retry.")
TRACE_MSG(MNSS_MODULE_2401_112_2_17_22_20_47_1271,"MNSS:ussd notify is released by network before user release.")
TRACE_MSG(MNSS_MODULE_2428_112_2_17_22_20_47_1272,"MNSS: User release active operation.\n")
TRACE_MSG(MNSS_MODULE_2532_112_2_17_22_20_47_1273,"MNSS: user response ussd operation error.\n")
TRACE_MSG(MNSS_MODULE_2613_112_2_17_22_20_48_1274,"MNSS: receive Unstructured service notify invoke from network at idle state. \n")
TRACE_MSG(MNSS_MODULE_2634_112_2_17_22_20_48_1275,"MNSS: send out Unstructured service notify return result message.\n")
TRACE_MSG(MNSS_MODULE_2665_112_2_17_22_20_48_1276,"MNSS: component tag is error. \n")
TRACE_MSG(MNSS_MODULE_2757_112_2_17_22_20_48_1277,"MNSS: HandleFacilitySucc() component tag is error. ")
TRACE_MSG(MNSS_MODULE_2949_112_2_17_22_20_48_1278,"MNSS: HandleOperationResult operation_code = %d")
TRACE_MSG(MNSS_MODULE_3003_112_2_17_22_20_48_1279,"MNSS:HandleOperationResult(),s_active_entity[dual_sys].operate.operation_code = %d")
TRACE_MSG(MNSS_MODULE_3041_112_2_17_22_20_48_1280,"MNSS:operation_code =%d")
TRACE_MSG(MNSS_MODULE_3074_112_2_17_22_20_49_1281,"MNSS:component_ptr->content_union.error = %d")
TRACE_MSG(MNSS_MODULE_3099_112_2_17_22_20_49_1282,"MNSS:The Failure Operate is SS_UNSTRUCTSS_NOTIFY!\n")
TRACE_MSG(MNSS_MODULE_3110_112_2_17_22_20_49_1283,"MNSS: assert faulure, operation_code = %d")
TRACE_MSG(MNSS_MODULE_3134_112_2_17_22_20_49_1284,"MNSS: InitProcessUssdData unpack ussd string.")
TRACE_MSG(MNSS_MODULE_3514_112_2_17_22_20_49_1285,"MNSS:SS_EncodeRegisterSSStr,forward number length is invalid!")
TRACE_MSG(MNSS_MODULE_3530_112_2_17_22_20_49_1286,"MNSS: SS_EncodeRegisterSSStr,The ss-code can not be operated by register.\n")
TRACE_MSG(MNSS_MODULE_3576_112_2_17_22_20_50_1287,"MNSS: The ss-code can not be operated by erase.\n")
TRACE_MSG(MNSS_MODULE_3623_112_2_17_22_20_50_1288,"MNSS: The ss-code can not be operated by activate.\n")
TRACE_MSG(MNSS_MODULE_3669_112_2_17_22_20_50_1289,"MNSS: The ss-code can not be operated by deactivation.\n")
TRACE_MSG(MNSS_MODULE_3760_112_2_17_22_20_50_1290,"MNSS::SS_EncodeRegisterPasswordSSStr for all car bar service.")
TRACE_MSG(MNSS_MODULE_3986_112_2_17_22_20_50_1291,"MNSS:unexpected operation code!")
TRACE_MSG(MNSS_MSG_1105_112_2_17_22_20_53_1292,"MNSS::DecodeUSSDdata data len is 0.")
TRACE_MSG(MNSS_MSG_1878_112_2_17_22_20_55_1293,"MNSS: CALL BARRING INFO ss status is %d")
TRACE_MSG(MNSS_MSG_2652_112_2_17_22_20_56_1294,"MNSS::network only return a list of basic service.")
TRACE_MSG(MNSS_MSG_2826_112_2_17_22_20_57_1295,"MNSS: DecodeResultRspSSinfo 0 == buf_length\n")
TRACE_MSG(MNSS_MSG_2849_112_2_17_22_20_57_1296,"MNSS: decode callbarringinfo")
TRACE_MSG(MNSS_MSG_2920_112_2_17_22_20_57_1297,"MNSS: DecodeEraseCcbsRspSSinfo 0 == buf_length\n")
TRACE_MSG(MNSS_MSG_3052_112_2_17_22_20_57_1298,"MNSS: DecodeResultFollowedParam component_len.len is error\n")
TRACE_MSG(MNSS_MSG_3065_112_2_17_22_20_57_1299,"MNSS: DecodeResultFollowedParam buf_length is error\n")
TRACE_MSG(MNSS_MSG_3085_112_2_17_22_20_57_1300,"MNSS: DecodeResultFollowedParam SS_COMPONENT_OPERATION_CODE_TAG is error\n")
TRACE_MSG(MNSS_MSG_3144_112_2_17_22_20_57_1301,"MNSS: DecodeResultFollowedParam decode register activatess erasess deactivatess\n")
TRACE_MSG(MNSS_MSG_3157_112_2_17_22_20_57_1302,"MNSS: DecodeResultFollowedParam decode register activatess erasess deactivatess is error\n")
TRACE_MSG(MNSS_MSG_3176_112_2_17_22_20_57_1303,"MNSS: DecodeResultFollowedParam decode register activatess erasess deactivatess is error\n")
TRACE_MSG(MNSS_MSG_3385_112_2_17_22_20_58_1304,"MNSS: DecodeResultComponent length  is error\n")
TRACE_MSG(MNSS_MSG_3398_112_2_17_22_20_58_1305,"MNSS: DecodeResultComponent buf_length error\n")
TRACE_MSG(MNSS_MSG_3418_112_2_17_22_20_58_1306,"MNSS: DecodeResultComponent SS_COMPONENT_INVOKE_ID_TAG  is error\n")
TRACE_MSG(MNSS_MSG_3435_112_2_17_22_20_58_1307,"MNSS: DecodeResultComponent SS_COMPONENT_SEQUENCE_TAG  is error\n")
TRACE_MSG(MNSS_MSG_3690_112_2_17_22_20_58_1308,"MNSS: MNSSMSG_DecodeFacilityIe component_tag = %d")
TRACE_MSG(MNSS_MSG_3828_112_2_17_22_20_59_1309,"MNSS::override category is present.")
TRACE_MSG(MNSS_MSG_3874_112_2_17_22_20_59_1310,"MNSS::default priority is present.")
TRACE_MSG(SSCALLRELATE_428_112_2_17_22_21_2_1311,"MNSS sscallrelate encode deflect number out of range ")
TRACE_MSG(SSCALLRELATE_443_112_2_17_22_21_2_1312,"MNSS sscallrelate encode subaddress out of range ")
TRACE_MSG(SSCALLRELATE_456_112_2_17_22_21_2_1313,"MNSS sscallrelate encode call deflect out of range ")
TRACE_MSG(SSCALLRELATE_540_112_2_17_22_21_2_1314,"MNCALL: not the right operation code\n")
TRACE_MSG(SSCALLRELATE_669_112_2_17_22_21_2_1315,"MNCALL: component type error to encode.\n")
TRACE_MSG(SSCALLRELATE_857_112_2_17_22_21_3_1316,"MNCALL: not the right tag\n")
TRACE_MSG(SSCALLRELATE_862_112_2_17_22_21_3_1317,"MNCALL: not the right ss code\n")
TRACE_MSG(SSCALLRELATE_1064_112_2_17_22_21_3_1318,"MNCALL: not the right tag\n")
TRACE_MSG(SSCALLRELATE_1709_112_2_17_22_21_4_1319,"MNCALL: not the right operate code\n")
TRACE_MSG(SSCALLRELATE_1845_112_2_17_22_21_5_1320,"MNCALL: ussd data length out of range\n")
TRACE_MSG(SSCALLRELATE_1916_112_2_17_22_21_5_1321,"MNSS: SSBASIC_DecodeCcbsRsp component_len.len + cur_offset) <= buf_length  is error\n")
TRACE_MSG(SSCALLRELATE_2444_112_2_17_22_21_6_1322,"MNCALL: component tage is error to encode\n")
TRACE_MSG(SIMAT_API_934_112_2_17_22_21_8_1323,"SIMAT::call control is enabled.")
TRACE_MSG(SIMAT_API_958_112_2_17_22_21_8_1324,"SIMAT::MO SMS control is enabled.")
TRACE_MSG(SIMAT_API_2011_112_2_17_22_21_10_1325,"SIMAT::SIMAT_IsNeedSMSControl result = %d")
TRACE_MSG(SIMAT_API_2433_112_2_17_22_21_11_1326,"MN: refresh_type error %d")
TRACE_MSG(SIMAT_CONFIRM_324_112_2_17_22_21_13_1327,"SIMAT:PC list is null.error!!!")
TRACE_MSG(SIMAT_CONFIRM_340_112_2_17_22_21_13_1328,"SIMAT:%d pc not been found,error!!!")
TRACE_MSG(SIMAT_CONFIRM_369_112_2_17_22_21_13_1329,"SIMAT:SIMAT_SaveCurPCInfo,error com_len = %d,data buf(1-10):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2xn")
TRACE_MSG(SIMAT_CONFIRM_464_112_2_17_22_21_14_1330,"SIMAT:PC list is null.error!!!")
TRACE_MSG(SIMAT_CONFIRM_499_112_2_17_22_21_14_1331,"SIMAT:%d pc not been found,error!!!")
TRACE_MSG(SIMAT_CONFIRM_640_112_2_17_22_21_14_1332,"SIMAT:SIMAT_CodePCResponse return result 0x%x out of range.")
TRACE_MSG(SIMAT_CONFIRM_773_112_2_17_22_21_14_1333,"SIMAT:SIMAT_CodePCResponse the return result 0x%x is out of range.")
TRACE_MSG(SIMAT_CONFIRM_966_112_2_17_22_21_15_1334,"SIMAT: SetupCall does not exist in the PC list, Coderesponse error and free the signal")
TRACE_MSG(SIMAT_CONFIRM_978_112_2_17_22_21_15_1335,"SIMAT:SIMAT_TRSetupCall code call control error!")
TRACE_MSG(SIMAT_CONFIRM_1059_112_2_17_22_21_15_1336,"SIMAT: SendSS does not exist in the PC list, Coderesponse error and free the signal")
TRACE_MSG(SIMAT_CONFIRM_1071_112_2_17_22_21_15_1337,"SIMAT:SIMAT_TRSendSS code call control error!")
TRACE_MSG(SIMAT_CONFIRM_1171_112_2_17_22_21_15_1338,"SIMAT: SendUSSD does not exist in the PC list, Coderesponse error and free the signal")
TRACE_MSG(SIMAT_CONFIRM_1184_112_2_17_22_21_15_1339,"SIMAT:SIMAT_TRSendUSSD encode error!")
TRACE_MSG(SIMAT_CONFIRM_1221_112_2_17_22_21_15_1340,"SIMAT: SendUSSD does not exist in the PC list, Coderesponse error and free the signal")
TRACE_MSG(SIMAT_CONFIRM_1253_112_2_17_22_21_15_1341,"SIMAT:SIMAT_TRSendUSSD encode error!")
TRACE_MSG(SIMAT_CONFIRM_1268_112_2_17_22_21_15_1342,"SIMAT:SIMAT_TRSendUSSD code call control error!")
TRACE_MSG(SIMAT_CONFIRM_1412_112_2_17_22_21_15_1343,"SIMAT:SIMAT_TRGetInkey encode error!")
TRACE_MSG(SIMAT_CONFIRM_1447_112_2_17_22_21_16_1344,"SIMAT:the user input text string length is out of range")
TRACE_MSG(SIMAT_CONFIRM_1462_112_2_17_22_21_16_1345,"SIMAT:the input text string beyond 255, cut it!!!")
TRACE_MSG(SIMAT_CONFIRM_1476_112_2_17_22_21_16_1346,"SIMAT:SIMAT_TRGetInput encode error!")
TRACE_MSG(SIMAT_CONFIRM_1518_112_2_17_22_21_16_1347,"SIMAT:SIMAT_TRSelectItem encode error!")
TRACE_MSG(SIMAT_CONFIRM_1581_112_2_17_22_21_16_1348,"SIMAT:SIMAT_TRGetLAI encode error!")
TRACE_MSG(SIMAT_CONFIRM_1622_112_2_17_22_21_16_1349,"SIMAT:SIMAT_TRGetIMEI encode error!")
TRACE_MSG(SIMAT_CONFIRM_1680_112_2_17_22_21_16_1350,"SIMAT:SIMAT_TRGetNMRBA LIST encode error!")
TRACE_MSG(SIMAT_CONFIRM_1721_112_2_17_22_21_16_1351,"SIMAT:SIMAT_TRGetDTTZ encode error!")
TRACE_MSG(SIMAT_CONFIRM_1762_112_2_17_22_21_16_1352,"SIMAT:SIMAT_TRGetLang encode error!")
TRACE_MSG(SIMAT_CONFIRM_1804_112_2_17_22_21_16_1353,"SIMAT:SIMAT_TRGetTimingAD encode error!")
TRACE_MSG(SIMAT_CONFIRM_1919_112_2_17_22_21_16_1354,"Code pc response error and free the signal")
TRACE_MSG(SIMAT_CONFIRM_2012_112_2_17_22_21_17_1355,"SIMAT:PC list is null.")
TRACE_MSG(SIMAT_CONFIRM_2097_112_2_17_22_21_17_1356,"SIMAT:SIMAT_TROpenChannel encode error!")
TRACE_MSG(SIMAT_CONFIRM_2137_112_2_17_22_21_17_1357,"SIMAT:SIMAT_TROpenChannel encode error!")
TRACE_MSG(SIMAT_CONFIRM_2208_112_2_17_22_21_17_1358,"SIMAT:SIMAT_TRGetChannelStatus encode error!")
TRACE_MSG(SIMAT_CONFIRM_2257_112_2_17_22_21_17_1359,"SIMAT:SIMAT_TRSendData encode error!")
TRACE_MSG(SIMAT_CONFIRM_2318_112_2_17_22_21_17_1360,"SIMAT:SIMAT_TRReceiveData encode error!")
TRACE_MSG(SIMAT_CONFIRM_2350_112_2_17_22_21_17_1361,"SIMAT:no pc is in list!")
TRACE_MSG(SIMAT_CONFIRM_2368_112_2_17_22_21_17_1362,"SIMAT:pc %d is not in  the list.")
TRACE_MSG(SIMAT_DO_DECODE_1033_112_2_17_22_21_20_1363,"SIMAT::SIMAT_DecodeDataObject tag error!")
TRACE_MSG(SIMAT_DO_DECODE_1120_112_2_17_22_21_20_1364,"SIMAT:the address len longer than the maximum we support, error and cut it!!!")
TRACE_MSG(SIMAT_DO_DECODE_1284_112_2_17_22_21_20_1365,"SIMAT:the len of the cps is out of range.")
TRACE_MSG(SIMAT_DO_DECODE_1366_112_2_17_22_21_20_1366,"SIMAT:len of the ccp out of range.")
TRACE_MSG(SIMAT_DO_DECODE_1603_112_2_17_22_21_21_1367,"SIMAT: length of duration not 2.")
TRACE_MSG(SIMAT_DO_DECODE_1776_112_2_17_22_21_21_1368,"SIMAT: length of item id is not 1.")
TRACE_MSG(SIMAT_DO_DECODE_1848_112_2_17_22_21_21_1369,"SIMAT: length of Response length is not 2.")
TRACE_MSG(SIMAT_DO_DECODE_2022_112_2_17_22_21_22_1370,"SIMAT:len of sms tpdu is out of range.")
TRACE_MSG(SIMAT_DO_DECODE_2252_112_2_17_22_21_22_1371,"SIMAT: length of tone is not 1.")
TRACE_MSG(SIMAT_DO_DECODE_2352_112_2_17_22_21_22_1372,"SIMAT: ussd string len is out of range.")
TRACE_MSG(SIMAT_DO_DECODE_2461_112_2_17_22_21_22_1373,"SIMAT: file num of sim given is error,true num is %d.")
TRACE_MSG(SIMAT_DO_DECODE_2518_112_2_17_22_21_23_1374,"SIMAT:length of LocationInfo is not 7!")
TRACE_MSG(SIMAT_DO_DECODE_2594_112_2_17_22_21_23_1375,"SIMAT:length of IMEI is not 8!")
TRACE_MSG(SIMAT_DO_DECODE_2660_112_2_17_22_21_23_1376,"SIMAT:length of Help request is not 0!")
TRACE_MSG(SIMAT_DO_DECODE_2726_112_2_17_22_21_23_1377,"SIMAT:length of NMR is not 16!")
TRACE_MSG(SIMAT_DO_DECODE_3042_112_2_17_22_21_24_1378,"SIMAT: cause len is out of range.")
TRACE_MSG(SIMAT_DO_DECODE_3098_112_2_17_22_21_24_1379,"SIMAT:length of location status is not 1.")
TRACE_MSG(SIMAT_DO_DECODE_3267_112_2_17_22_21_24_1380,"SIMAT: len of the bcch list is out of range.")
TRACE_MSG(SIMAT_DO_DECODE_3344_112_2_17_22_21_24_1381,"SIMAT:DecodeCCReqAction alloc from share buffer error!")
TRACE_MSG(SIMAT_DO_DECODE_3401_112_2_17_22_21_24_1382,"SIMAT: length of icon id is not 2.")
TRACE_MSG(SIMAT_DO_DECODE_3514_112_2_17_22_21_24_1383,"SIMAT: icon list is out of range.")
TRACE_MSG(SIMAT_DO_DECODE_3695_112_2_17_22_21_25_1384,"SIMAT: length of timer id is not 1.")
TRACE_MSG(SIMAT_DO_DECODE_3843_112_2_17_22_21_25_1385,"SIMAT: length of the DTandDZ is not 7.")
TRACE_MSG(SIMAT_DO_DECODE_4033_112_2_17_22_21_25_1386,"SIMAT: length of the ImmediateResponse is not 0.")
TRACE_MSG(SIMAT_DO_DECODE_4173_112_2_17_22_21_26_1387,"SIMAT: length of the Language is not 2.")
TRACE_MSG(SIMAT_DO_DECODE_4244_112_2_17_22_21_26_1388,"SIMAT: length of the Time Advance is not 2.")
TRACE_MSG(SIMAT_DO_DECODE_4460_112_2_17_22_21_26_1389,"SIMAT: DecodeChannelData content_len is zero!!! ")
TRACE_MSG(SIMAT_DO_DECODE_4521_112_2_17_22_21_26_1390,"SIMAT: length of channel data len  is not 1.")
TRACE_MSG(SIMAT_DO_DECODE_4580_112_2_17_22_21_27_1391,"SIMAT: length of browser id  is not 1.")
TRACE_MSG(SIMAT_DO_DECODE_4637_112_2_17_22_21_27_1392,"SIMAT: length of URL overflow.")
TRACE_MSG(SIMAT_DO_DECODE_4694_112_2_17_22_21_27_1393,"SIMAT: length of bear overflow.")
TRACE_MSG(SIMAT_DO_DECODE_4751_112_2_17_22_21_27_1394,"SIMAT: length of file path overflow.")
TRACE_MSG(SIMAT_DO_DECODE_4810_112_2_17_22_21_27_1395,"SIMAT: DecodeBrowserTerminationCause content_len is error!!! ")
TRACE_MSG(SIMAT_DO_DECODE_4863_112_2_17_22_21_27_1396,"SIMAT: DecodeChannelStatus content_len is erro!!! ")
TRACE_MSG(SIMAT_DO_DECODE_4968_112_2_17_22_21_27_1397,"SIMAT: length of bear overflow.")
TRACE_MSG(SIMAT_DO_DECODE_4976_112_2_17_22_21_27_1398,"SIMAT: DecodeBearDesc content_len is zero!!! ")
TRACE_MSG(SIMAT_DO_DECODE_5032_112_2_17_22_21_27_1399,"SIMAT: length of bear overflow.")
TRACE_MSG(SIMAT_DO_DECODE_5139_112_2_17_22_21_28_1400,"SIMAT: length of net access name overflow.")
TRACE_MSG(SIMAT_DO_DECODE_5147_112_2_17_22_21_28_1401,"SIMAT: DecodeNetAccessName content_len is zero!!! ")
TRACE_MSG(SIMAT_DO_DECODE_5204_112_2_17_22_21_28_1402,"SIMAT: length of bear overflow.")
TRACE_MSG(SIMAT_DO_DECODE_5212_112_2_17_22_21_28_1403,"SIMAT:DecodeOtherAddress content_len is zero!!! ")
TRACE_MSG(SIMAT_DO_DECODE_5268_112_2_17_22_21_28_1404,"SIMAT: length of trans level overflow.")
TRACE_MSG(SIMAT_DO_DECODE_5287_112_2_17_22_21_28_1405,"SIMAT:DecodeTransLevel port= %x")
TRACE_MSG(SIMAT_DO_ENCODE_921_112_2_17_22_21_30_1406,"SIMAT address len longer than maximum, error and cut it!!!")
TRACE_MSG(SIMAT_DO_ENCODE_1016_112_2_17_22_21_30_1407,"SIMAT: len of the cps is out of range in encode.")
TRACE_MSG(SIMAT_DO_ENCODE_1076_112_2_17_22_21_30_1408,"SIMAT: len of the ccp is out of range in encode.")
TRACE_MSG(SIMAT_DO_ENCODE_1125_112_2_17_22_21_31_1409,"SIMAT: len of the cb page is out of range in encode.")
TRACE_MSG(SIMAT_DO_ENCODE_1490_112_2_17_22_21_31_1410,"SIMAT: len of the sms tpdu is out of range in encode.")
TRACE_MSG(SIMAT_DO_ENCODE_1681_112_2_17_22_21_32_1411,"SIMAT: ussd string len is out of range in encode.")
TRACE_MSG(SIMAT_DO_ENCODE_2147_112_2_17_22_21_33_1412,"SIMAT: cause len is out of range in encode.")
TRACE_MSG(SIMAT_DO_ENCODE_2280_112_2_17_22_21_33_1413,"SIMAT: len of the bcch list is out of range in encode.")
TRACE_MSG(SIMAT_DO_ENCODE_2664_112_2_17_22_21_34_1414,"SIMAT:EncodeTimerValue,hour %d is out of range.")
TRACE_MSG(SIMAT_DO_ENCODE_2738_112_2_17_22_21_34_1415,"SIMAT:time_zone = %d.")
TRACE_MSG(SIMAT_DO_ENCODE_2749_112_2_17_22_21_34_1416,"SIMAT:time_zone encode value is 0x%x.")
TRACE_MSG(SIMAT_ENVELOPE_520_112_2_17_22_21_36_1417,"SIMAT:mnc digit num is %d,error.")
TRACE_MSG(SIMAT_ENVELOPE_538_112_2_17_22_21_36_1418,"SIMAT: SIMAT_MOCallControl data_type = %d")
TRACE_MSG(SIMAT_ENVELOPE_1739_112_2_17_22_21_39_1419,"SIMAT:SIMAT_ProcessEnvelopeCnf recv unknown signal:0x%x.")
TRACE_MSG(SIMAT_ENVELOPE_1764_112_2_17_22_21_39_1420,"SIMAT:sim toolkit busy when timer expiration event.")
TRACE_MSG(SIMAT_ENVELOPE_1771_112_2_17_22_21_39_1421,"SIMAT:timer expiration event download failed for stk busy.")
TRACE_MSG(SIMAT_ENVELOPE_1776_112_2_17_22_21_39_1422,"SIMAT: add timer expiration envelope signal to not busy envelope request list.")
TRACE_MSG(SIMAT_ENVELOPE_1818_112_2_17_22_21_39_1423,"SIMAT:sim toolkit busy when language select event.")
TRACE_MSG(SIMAT_ENVELOPE_1869_112_2_17_22_21_39_1424,"SIMAT:sim toolkit busy when idle screen available event.")
TRACE_MSG(SIMAT_ENVELOPE_1920_112_2_17_22_21_39_1425,"SIMAT:sim toolkit busy when user activity event.")
TRACE_MSG(SIMAT_ENVELOPE_1969_112_2_17_22_21_39_1426,"SIMAT:sim toolkit busy when location status event.")
TRACE_MSG(SIMAT_ENVELOPE_1975_112_2_17_22_21_39_1427,"SIMAT:location status event download failed for stk busy.")
TRACE_MSG(SIMAT_ENVELOPE_2016_112_2_17_22_21_40_1428,"SIMAT:sim toolkit busy when call disconnect event.")
TRACE_MSG(SIMAT_ENVELOPE_2022_112_2_17_22_21_40_1429,"SIMAT:call disconnect event download failed for stk busy.")
TRACE_MSG(SIMAT_ENVELOPE_2063_112_2_17_22_21_40_1430,"SIMAT:sim toolkit busy when call connect event.")
TRACE_MSG(SIMAT_ENVELOPE_2069_112_2_17_22_21_40_1431,"SIMAT:call connect event download failed for stk busy.")
TRACE_MSG(SIMAT_ENVELOPE_2109_112_2_17_22_21_40_1432,"SIMAT:sim toolkit busy when MT call event.")
TRACE_MSG(SIMAT_ENVELOPE_2115_112_2_17_22_21_40_1433,"SIMAT:MT call event download failed for stk busy.")
TRACE_MSG(SIMAT_ENVELOPE_2164_112_2_17_22_21_40_1434,"SIMAT:sim toolkit busy when MO SM call control.")
TRACE_MSG(SIMAT_ENVELOPE_2203_112_2_17_22_21_40_1435,"SIMAT:MOSMControlCnf envelope result error!")
TRACE_MSG(SIMAT_ENVELOPE_2262_112_2_17_22_21_40_1436,"SIMAT:MOSMControlCnf data_len error!")
TRACE_MSG(SIMAT_ENVELOPE_2326_112_2_17_22_21_40_1437,"SIMAT:DecodeControlCnfData control result error!")
TRACE_MSG(SIMAT_ENVELOPE_2470_112_2_17_22_21_40_1438,"SIMAT:DecodeControlCnfData data_len error!")
TRACE_MSG(SIMAT_ENVELOPE_2505_112_2_17_22_21_41_1439,"SIMAT:sim toolkit busy when MO call control.")
TRACE_MSG(SIMAT_ENVELOPE_2526_112_2_17_22_21_41_1440,"SIMAT:call control evelope failed for unknown reason.")
TRACE_MSG(SIMAT_ENVELOPE_2555_112_2_17_22_21_41_1441,"SIMAT:Envelope confirm, the result is sim card not allowed, busy or error!")
TRACE_MSG(SIMAT_ENVELOPE_2572_112_2_17_22_21_41_1442,"SIMAT:send ussd result %d")
TRACE_MSG(SIMAT_ENVELOPE_2582_112_2_17_22_21_41_1443,"SIMAT:send ussd result %d")
TRACE_MSG(SIMAT_ENVELOPE_2591_112_2_17_22_21_41_1444,"SIMAT:send originate ussd success")
TRACE_MSG(SIMAT_ENVELOPE_2603_112_2_17_22_21_41_1445,"SIMAT:Call control from proactive command!")
TRACE_MSG(SIMAT_ENVELOPE_2608_112_2_17_22_21_41_1446,"SIMAT:USSD Control result is SIMAT_SIMC_NOT_ALLOWED!")
TRACE_MSG(SIMAT_ENVELOPE_2615_112_2_17_22_21_41_1447,"SIMAT:USSD Control result is %d!")
TRACE_MSG(SIMAT_ENVELOPE_2622_112_2_17_22_21_41_1448,"SIMAT:USSD Control result is %d!")
TRACE_MSG(SIMAT_ENVELOPE_2644_112_2_17_22_21_41_1449,"SIMAT:Error!,call contol, but the control type invalid!")
TRACE_MSG(SIMAT_ENVELOPE_2692_112_2_17_22_21_41_1450,"SIMAT:SMSPPDownloadCnf ok with response data length error")
TRACE_MSG(SIMAT_ENVELOPE_2707_112_2_17_22_21_41_1451,"SIMAT:SMSPPDownloadCnf error with response data length error")
TRACE_MSG(SIMAT_ENVELOPE_2753_112_2_17_22_21_41_1452,"SIMAT:sim toolkit busy when CB download.")
TRACE_MSG(SIMAT_ENVELOPE_2797_112_2_17_22_21_41_1453,"SIMAT:menu selection failed for stk busy.")
TRACE_MSG(SIMAT_ENVELOPE_2867_112_2_17_22_21_41_1454,"SIMAT:the input mnc len is error,the len is 0x%x.")
TRACE_MSG(SIMAT_ENVELOPE_2932_112_2_17_22_21_41_1455,"SIMAT:session end,there is no other envelope command need to retry.")
TRACE_MSG(SIMAT_ENVELOPE_2961_112_2_17_22_21_41_1456,"SIMAT:session end,there is envelope command need to retry.")
TRACE_MSG(SIMAT_ENVELOPE_2966_112_2_17_22_21_41_1457,"SIMAT:there is unexpect signal put in retry list,signal code:0x%x.")
TRACE_MSG(SIMAT_ENVELOPE_2990_112_2_17_22_21_42_1458,"SIMAT:sim toolkit busy when data available.")
TRACE_MSG(SIMAT_ENVELOPE_3039_112_2_17_22_21_42_1459,"SIMAT:sim toolkit busy when channel status event.")
TRACE_MSG(SIMAT_ENVELOPE_3087_112_2_17_22_21_42_1460,"SIMAT:sim toolkit busy when browser termination event.")
TRACE_MSG(SIMAT_ENVELOPE_3127_112_2_17_22_21_42_1461,"SIMAT:there is PC wait terminal response,do envelope but delay fetch.")
TRACE_MSG(SIMAT_ENVELOPE_3186_112_2_17_22_21_42_1462,"SIMAT:sim toolkit busy when at cmd envelope")
TRACE_MSG(SIMAT_ENVELOPE_3192_112_2_17_22_21_42_1463,"SIMAT:at cmd envelope failed for stk busy.")
TRACE_MSG(SIMAT_IMPLE_73_112_2_17_22_21_42_1464,"SIMAT:stk polling interval is %d")
TRACE_MSG(SIMAT_IMPLE_99_112_2_17_22_21_42_1465,"SIMAT::start timer %d,current sys time is %d.")
TRACE_MSG(SIMAT_IMPLE_154_112_2_17_22_21_43_1466,"SIMAT:current time is %d.")
TRACE_MSG(SIMAT_IMPLE_159_112_2_17_22_21_43_1467,"SIMAT:time not wrap.")
TRACE_MSG(SIMAT_IMPLE_164_112_2_17_22_21_43_1468,"SIMAT:time wrap.")
TRACE_MSG(SIMAT_IMPLE_171_112_2_17_22_21_43_1469,"SIMAT:timer%d,run time is %d,left time_value is %d.")
TRACE_MSG(SIMAT_IMPLE_206_112_2_17_22_21_43_1470,"SIMAT:current time is %d.")
TRACE_MSG(SIMAT_IMPLE_211_112_2_17_22_21_43_1471,"SIMAT:time not wrap.")
TRACE_MSG(SIMAT_IMPLE_216_112_2_17_22_21_43_1472,"SIMAT:time wrap.")
TRACE_MSG(SIMAT_IMPLE_223_112_2_17_22_21_43_1473,"SIMAT:timer%d,run time is %d.")
TRACE_MSG(SIMAT_MAIN_114_112_2_17_22_21_43_1474,"SIMAT:receive signal 0x%x in stk poweroffstatus.")
TRACE_MSG(SIMAT_MAIN_266_112_2_17_22_21_43_1475,"SIMAT:command is poll int or off,don't send session end to up layer.")
TRACE_MSG(SIMAT_MAIN_305_112_2_17_22_21_44_1476,"SIMAT:discard signal 0x%x in stk active status.")
TRACE_MSG(SIMAT_MAIN_666_112_2_17_22_21_44_1477,"SIMAT:msg_type=%x")
TRACE_MSG(SIMAT_PROACTIVE_720_112_2_17_22_21_46_1478,"SIMAT:SIMAT_GetPCTag PC name 0x%x is out of range.")
TRACE_MSG(SIMAT_PROACTIVE_749_112_2_17_22_21_46_1479,"SIMAT:SIMAT_GetPCName PC tag 0x%x is out of range.")
TRACE_MSG(SIMAT_PROACTIVE_1074_112_2_17_22_21_47_1480,"pc_num[%d] = %d.")
TRACE_MSG(SIMAT_PROACTIVE_1081_112_2_17_22_21_47_1481,"SIMAT::PC len error,data_len = %d,data buf(1-6):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2xn")
TRACE_MSG(SIMAT_PROACTIVE_1090_112_2_17_22_21_47_1482,"SIMAT::ERROR data_len = %d,data buf(1-10):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2xn")
TRACE_MSG(SIMAT_PROACTIVE_1099_112_2_17_22_21_47_1483,"SIMAT:SIMAT_ProcessProactiveCommand get tlv length error")
TRACE_MSG(SIMAT_PROACTIVE_1111_112_2_17_22_21_47_1484,"SIMAT:the valid pc command len is modify to 0x%x")
TRACE_MSG(SIMAT_PROACTIVE_1175_112_2_17_22_21_47_1485,"SIMAT:send session end to up layer.")
TRACE_MSG(SIMAT_PROACTIVE_3113_112_2_17_22_21_52_1486,"SIMAT:min_len is reduced to 0x%x byte!")
TRACE_MSG(SIMAT_PROACTIVE_3120_112_2_17_22_21_52_1487,"SIMAT:max_len is reduced to 0x%x byte!")
TRACE_MSG(SIMAT_PROACTIVE_3133_112_2_17_22_21_52_1488,"SIMAT:min_len is reduced to 0x%x byte!")
TRACE_MSG(SIMAT_PROACTIVE_3141_112_2_17_22_21_52_1489,"SIMAT:max_len is reduced to 0x%x byte!")
TRACE_MSG(SIMAT_PROACTIVE_3151_112_2_17_22_21_52_1490,"SIMAT:min_len is reduced to 0x%x byte!")
TRACE_MSG(SIMAT_PROACTIVE_3158_112_2_17_22_21_52_1491,"SIMAT:max_len is reduced to 0x%x byte!")
TRACE_MSG(SIMAT_PROACTIVE_3698_112_2_17_22_21_53_1492,"SIMAT:the pc data of setup menu is not correct,but the MS will ignore it.")
TRACE_MSG(SIMAT_PROACTIVE_3720_112_2_17_22_21_53_1493,"SIMAT:has received set up menu proactive command.")
TRACE_MSG(SIMAT_PROACTIVE_3922_112_2_17_22_21_53_1494,"SIMAT:In traffic mode,send NMR to SIM is not surpported temporarily")
TRACE_MSG(SIMAT_PROACTIVE_3926_112_2_17_22_21_53_1495,"SIMAT:ba_ind = %d, rxlev-full-serving-cell = %d")
TRACE_MSG(SIMAT_PROACTIVE_3973_112_2_17_22_21_53_1496,"SIMAT:is_me_idle = %d, timing advance value = %d")
TRACE_MSG(SIMAT_PROACTIVE_4647_112_2_17_22_21_55_1497,"SIMAT:LaunchBrowser,the device id is error!Ignore it.")
TRACE_MSG(SIMAT_PROACTIVE_4879_112_2_17_22_21_55_1498,"SIMAT: OpenChannel!")
TRACE_MSG(SIMAT_PROACTIVE_5038_112_2_17_22_21_56_1499,"SIMAT:there are too more text string in open channel,ignore it.")
TRACE_MSG(SIMAT_PROACTIVE_5068_112_2_17_22_21_56_1500,"SIMAT:[OpenChannel]::open_channel.trans_level type=%d, port=%x ")
TRACE_MSG(SIMAT_PROACTIVE_5081_112_2_17_22_21_56_1501,"SIMAT: support GPRS bear only!!!")
TRACE_MSG(SIMAT_PROACTIVE_5098_112_2_17_22_21_56_1502,"SIMAT:leave open channel")
TRACE_MSG(SIMAT_PROACTIVE_5152_112_2_17_22_21_56_1503,"SIMAT: CloseChannel!")
TRACE_MSG(SIMAT_PROACTIVE_5292_112_2_17_22_21_56_1504,"SIMAT: ReceiveData!")
TRACE_MSG(SIMAT_PROACTIVE_5454_112_2_17_22_21_56_1505,"SIMAT: SendData!")
TRACE_MSG(SIMAT_PROACTIVE_5620_112_2_17_22_21_57_1506,"SIMAT: GetChannelStatus!")
TRACE_MSG(SIMAT_PROACTIVE_5804_112_2_17_22_21_57_1507,"SIMAT_ForwardProactiveCommand")
TRACE_MSG(SIMAT_PROACTIVE_5824_112_2_17_22_21_57_1508,"data_len = %d,data[0] = 0x%x,data[1]=0x%x.")
TRACE_MSG(SIMATDUM_COM_1038_112_2_17_22_22_0_1509,"SIMATDUM::read stk test case file ok.")
TRACE_MSG(SIMATDUM_COM_1086_112_2_17_22_22_0_1510,"SIMATDUM::write stk test case data file ok.")
TRACE_MSG(SIMATDUM_COM_DECODE_1476_112_2_17_22_22_3_1511,"SIMAT::unknown tag in the launch browser.")
TRACE_MSG(SIMATDUM_PROACTIVE_2230_112_2_17_22_22_16_1512,"SIMATDUM:Unknow Envelope type from simat!")
TRACE_MSG(SIMATDUM_PROACTIVE_2341_112_2_17_22_22_17_1513,"SIMATDUM:Unknow or unsupported proactive command terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_2675_112_2_17_22_22_17_1514,"SIMATDUM:Error Menu selection envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_2687_112_2_17_22_22_17_1515,"SIMATDUM:Error menu selection envelope encode!")
TRACE_MSG(SIMATDUM_PROACTIVE_2696_112_2_17_22_22_17_1516,"SIMATDUM:Error device id data object encode in menu selection envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_2706_112_2_17_22_22_17_1517,"SIMATDUM:Error device id data object in menu selection envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_2715_112_2_17_22_22_17_1518,"SIMATDUM:Error Item Id data object encode in menu selection envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_2730_112_2_17_22_22_17_1519,"SIMATDUM:Error help info data object in menu selection envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_2745_112_2_17_22_22_17_1520,"SIMATDUM:Error Item id in menu selection envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_2808_112_2_17_22_22_18_1521,"SIMATDUM:Error SMS pp download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_2820_112_2_17_22_22_18_1522,"SIMATDUM:Error SMS pp download envelope encode!")
TRACE_MSG(SIMATDUM_PROACTIVE_2829_112_2_17_22_22_18_1523,"SIMATDUM:Error device id data object decode in SMS pp download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_2839_112_2_17_22_22_18_1524,"SIMATDUM:Error device id data object in SMS pp download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_2850_112_2_17_22_22_18_1525,"SIMATDUM:Error address info data object in sms pp download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_2862_112_2_17_22_22_18_1526,"SIMATDUM:Error sms tpdu data object decode in SMS pp download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_2903_112_2_17_22_22_18_1527,"SIMATDUM:Error CB download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_2915_112_2_17_22_22_18_1528,"SIMATDUM:Error CB download envelope encode!")
TRACE_MSG(SIMATDUM_PROACTIVE_2924_112_2_17_22_22_18_1529,"SIMATDUM:Error device id data object decode in CB download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_2934_112_2_17_22_22_18_1530,"SIMATDUM:Error device id data object in CB download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_2943_112_2_17_22_22_18_1531,"SIMATDUM:Error cb page tpdu data object decode in cb download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_2991_112_2_17_22_22_18_1532,"SIMATDUM:Error call control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3003_112_2_17_22_22_18_1533,"SIMATDUM:Error call control envelope encode!")
TRACE_MSG(SIMATDUM_PROACTIVE_3012_112_2_17_22_22_18_1534,"SIMATDUM:Error device id data object encode in call control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3022_112_2_17_22_22_18_1535,"SIMATDUM:Error device id data object in call control selection envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3033_112_2_17_22_22_18_1536,"SIMATDUM:Error address data object encode in call control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3046_112_2_17_22_22_18_1537,"SIMATDUM:Error ss string data object encode in call control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3059_112_2_17_22_22_18_1538,"SIMATDUM:Error ussd string data object encode in call control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3069_112_2_17_22_22_18_1539,"SIMATDUM:The mandatory address data object is not exist in call control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3080_112_2_17_22_22_18_1540,"SIMATDUM:Error ccp1 data object in call control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3095_112_2_17_22_22_18_1541,"SIMATDUM:Error call party subaddress data object in call control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3110_112_2_17_22_22_18_1542,"SIMATDUM:Error location information data object in call control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3119_112_2_17_22_22_18_1543,"SIMATDUM:The mandatory location information data object is not exist in call control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3130_112_2_17_22_22_18_1544,"SIMATDUM:Error ccp2 data object in call control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3201_112_2_17_22_22_18_1545,"SIMATDUM:Error mo sms control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3213_112_2_17_22_22_18_1546,"SIMATDUM:Error mo sms control envelope encode!")
TRACE_MSG(SIMATDUM_PROACTIVE_3222_112_2_17_22_22_18_1547,"SIMATDUM:Error device id data object encode in mo sms control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3232_112_2_17_22_22_18_1548,"SIMATDUM:Error device id data object in mo sms control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3243_112_2_17_22_22_18_1549,"SIMATDUM:Error address data object encode in mo sms control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3252_112_2_17_22_22_18_1550,"SIMATDUM:The mandatory address data object is not exist in mo sms control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3263_112_2_17_22_22_18_1551,"SIMATDUM:Error address data object encode in mo sms control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3272_112_2_17_22_22_19_1552,"SIMATDUM:The mandatory address data object is not exist in mo sms control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3283_112_2_17_22_22_19_1553,"SIMATDUM:Error location information data object in mo sms control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3292_112_2_17_22_22_19_1554,"SIMATDUM:The mandatory location information data object is not exist in mo sms control envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3355_112_2_17_22_22_19_1555,"SIMATDUM:Error Event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3367_112_2_17_22_22_19_1556,"SIMATDUM:Error event download encode!")
TRACE_MSG(SIMATDUM_PROACTIVE_3376_112_2_17_22_22_19_1557,"SIMATDUM:Error event list data object encode in event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3387_112_2_17_22_22_19_1558,"SIMATDUM:Error event list data object in event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3396_112_2_17_22_22_19_1559,"SIMATDUM:Error device id data object encode in event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3410_112_2_17_22_22_19_1560,"SIMATDUM:Error device id data object in event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3420_112_2_17_22_22_19_1561,"SIMATDUM:Error device id data object in event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3430_112_2_17_22_22_19_1562,"SIMATDUM:Error device id data object in event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3444_112_2_17_22_22_19_1563,"SIMATDUM:Error transcation Id data object decode in event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3459_112_2_17_22_22_19_1564,"SIMATDUM:Error address data object decode in event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3473_112_2_17_22_22_19_1565,"SIMATDUM:Error call sub address data object decode in event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3489_112_2_17_22_22_19_1566,"SIMATDUM:Error cause data object decode in event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3506_112_2_17_22_22_19_1567,"SIMATDUM:Error location status data object in event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3519_112_2_17_22_22_19_1568,"SIMATDUM:Error location information data object decode in event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3532_112_2_17_22_22_19_1569,"SIMATDUM:Error language data object in event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3545_112_2_17_22_22_19_1570,"SIMATDUM:Error browser terminate cause data object decode in event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3555_112_2_17_22_22_19_1571,"SIMATDUM:No data object need to decode in event download envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3894_112_2_17_22_22_20_1572,"SIMATDUM:Error timer expiration envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3906_112_2_17_22_22_20_1573,"SIMATDUM:Error timer expiration envelope encode!")
TRACE_MSG(SIMATDUM_PROACTIVE_3915_112_2_17_22_22_20_1574,"SIMATDUM:Error device id data object encode in timer expiration envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3925_112_2_17_22_22_20_1575,"SIMATDUM:Error device id data object in timer expiration envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3934_112_2_17_22_22_20_1576,"SIMATDUM:Error Timer Id data object encode in timer expiration envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_3944_112_2_17_22_22_20_1577,"SIMATDUM:Error Timer Value data object encode in timer expiration envelope!")
TRACE_MSG(SIMATDUM_PROACTIVE_4050_112_2_17_22_22_20_1578,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_4107_112_2_17_22_22_20_1579,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_4182_112_2_17_22_22_20_1580,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_4253_112_2_17_22_22_21_1581,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_4262_112_2_17_22_22_21_1582,"SIMATDUM:Setup Menu proactive command not successed!")
TRACE_MSG(SIMATDUM_PROACTIVE_4311_112_2_17_22_22_21_1583,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_4334_112_2_17_22_22_21_1584,"SIMATDUM:Error Item id from the select item terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_4423_112_2_17_22_22_21_1585,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_4569_112_2_17_22_22_22_1586,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_4590_112_2_17_22_22_22_1587,"SIMATDUM:Error Text string length in get inkey terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_4669_112_2_17_22_22_22_1588,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_4756_112_2_17_22_22_22_1589,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_4770_112_2_17_22_22_22_1590,"SIMATDUM:Error in Decode Text string of terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_4863_112_2_17_22_22_22_1591,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_4948_112_2_17_22_22_22_1592,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_5035_112_2_17_22_22_23_1593,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_5121_112_2_17_22_22_23_1594,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_5132_112_2_17_22_22_23_1595,"SIMATDUM:Decode duration data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_5204_112_2_17_22_22_23_1596,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_5279_112_2_17_22_22_23_1597,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_5356_112_2_17_22_22_23_1598,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_5431_112_2_17_22_22_23_1599,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_5528_112_2_17_22_22_24_1600,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_5720_112_2_17_22_22_24_1601,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_5766_112_2_17_22_22_24_1602,"SIMATDUM:Error in Decode Text string of terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_5808_112_2_17_22_22_24_1603,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_5873_112_2_17_22_22_24_1604,"SIMATDUM:Error terminal reponse tag!")
TRACE_MSG(SIMATDUM_PROACTIVE_5883_112_2_17_22_22_24_1605,"SIMATDUM:Error command detail length in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_5891_112_2_17_22_22_24_1606,"SIMATDUM:Error command name in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_5900_112_2_17_22_22_24_1607,"SIMATDUM:Error Device Id data object encode in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_5907_112_2_17_22_22_24_1608,"SIMATDUM:Error Device Id data object in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_10108_112_2_17_22_22_34_1609,"SIMAT:the valid pc command len is modify to 0x%x")
TRACE_MSG(SIMATDUM_PROACTIVE_14749_112_2_17_22_22_44_1610,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_14828_112_2_17_22_22_44_1611,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_14843_112_2_17_22_22_44_1612,"SIMATDUM:Error in Decode Channel status of terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_14855_112_2_17_22_22_44_1613,"SIMATDUM:Error in Decode bear description of terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_14869_112_2_17_22_22_44_1614,"SIMATDUM:Error in Decode Buffer size of terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_14883_112_2_17_22_22_44_1615,"SIMATDUM:Error in Decode other address of terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_14985_112_2_17_22_22_44_1616,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_15061_112_2_17_22_22_44_1617,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_15076_112_2_17_22_22_44_1618,"SIMATDUM:Error in Decode Channel data of terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_15087_112_2_17_22_22_44_1619,"SIMATDUM:Error in Decode channel data len of terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_15159_112_2_17_22_22_45_1620,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_15174_112_2_17_22_22_45_1621,"SIMATDUM:Error in Decode channel data len of terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_15254_112_2_17_22_22_45_1622,"SIMATDUM:Decode Result data object error in terminal response!")
TRACE_MSG(SIMATDUM_PROACTIVE_15272_112_2_17_22_22_45_1623,"SIMATDUM:Error in Decode Channel status of terminal response!")
TRACE_MSG(MNPHONE_API_1317_112_2_17_22_23_8_1624,"MN: MAIN_SetPlmnSelectList validate_num=%d")
TRACE_MSG(MNPHONE_API_2655_112_2_17_22_23_11_1625,"MNPHONE_WritePLMNtoSimEx is_delete_item = %d, result = %d, order_of_plmn=%d")
TRACE_MSG(MNPHONE_API_3031_112_2_17_22_23_12_1626,"MNPHONE_SIMUnlockEx error_code %d")
TRACE_MSG(MNPHONE_API_3139_112_2_17_22_23_12_1627,"mnphone: MNPHONE_SelectPlmnWithoutSim dual_sys:%d, mcc:%d, mnc:%d, mnc_digit_num:%d.")
TRACE_MSG(MNPHONE_API_3143_112_2_17_22_23_12_1628,"mnphone: Assert failure, selected card %d is more than MAX %d.")
TRACE_MSG(MNPHONE_API_3151_112_2_17_22_23_12_1629,"mnphone: Assert failure, selected plmn list length %d is more than MAX %d.")
TRACE_MSG(MNPHONE_MODULE_2177_112_2_17_22_23_18_1630,"MNPHONE%d::the location status not changed.")
TRACE_MSG(MNPHONE_MODULE_2184_112_2_17_22_23_18_1631,"MNPHONE%d:: act type is no service, so the location status not changed.")
TRACE_MSG(MNPHONE_MODULE_2413_112_2_17_22_23_19_1632,"MNPHONE: HPLMN in forbidden plmn list, delete hplmn from forbidden plmn list")
TRACE_MSG(MNPHONE_MODULE_2424_112_2_17_22_23_19_1633,"MNPHONE: EHPLMN in forbidden plmn list, delete hplmn from forbidden plmn list")
TRACE_MSG(MNPHONE_MODULE_2579_112_2_17_22_23_19_1634,"MNPHONE: Switch on with manual select, select last rplmn or hplmn %d, %d")
TRACE_MSG(MNPHONE_MODULE_2618_112_2_17_22_23_19_1635,"MNPHONE: campon a plmn,mcc=%d,mnc=%d,any_plmn=%d,mnc_digit_num = %dn")
TRACE_MSG(MNPHONE_MODULE_2763_112_2_17_22_23_20_1636,"BASE Version:%s")
TRACE_MSG(MNPHONE_MODULE_2782_112_2_17_22_23_20_1637,"mnphone: SendNetworkStatusInd operate_mode = %d")
TRACE_MSG(MNPHONE_MODULE_2873_112_2_17_22_23_20_1638,"mnphone: assert failure, NETWORK_MODE_E")
TRACE_MSG(MNPHONE_MODULE_3115_112_2_17_22_23_20_1639,"MNPHONE:mnc %d,mcc %d,is_forbidden %d ")
TRACE_MSG(MNPHONE_MODULE_3717_112_2_17_22_23_22_1640,"mnphone: assert failure, MN_PHONE_PLMN_STATUS_E")
TRACE_MSG(MNPHONE_MODULE_3793_112_2_17_22_23_22_1641,"MNPHONE: Assert Failure to read IMEI!!")
TRACE_MSG(MNPHONE_MODULE_3827_112_2_17_22_23_22_1642,"MNPHONE: Assert Failure to read IMEISV!!")
TRACE_MSG(MNPHONE_MODULE_3837_112_2_17_22_23_22_1643,"MNPHONE: IMEISVN is not presented!!")
TRACE_MSG(MNPHONE_MODULE_3941_112_2_17_22_23_22_1644,"MNPHONE:SCI_CreatePeriodTimer,CheckDspIntTimer ")
TRACE_MSG(MNPHONE_MODULE_4114_112_2_17_22_23_22_1645,"mnphone: assert failure, RR_ACT_TYPE_E")
TRACE_MSG(MNPHONE_MODULE_4154_112_2_17_22_23_22_1646,"mnphone: assert failure, RR_ACT_TYPE_E")
TRACE_MSG(MNPHONE_MODULE_4371_112_2_17_22_23_23_1647,"MNPHONE:Camp reference is different, REQ Ref: %d, CNF Ref: %d")
TRACE_MSG(MNPHONE_MODULE_4400_112_2_17_22_23_23_1648,"mnphone: assert failure, RR_ACT_TYPE_E")
TRACE_MSG(MNPHONE_MODULE_4565_112_2_17_22_23_23_1649,"PHONE:InvalidSim_HandleSimChvCnf,s_mnphone_state=%d")
TRACE_MSG(MNPHONE_MODULE_4579_112_2_17_22_23_23_1650,"MNPHONE:unblock the CHV1 successful")
TRACE_MSG(MNPHONE_MODULE_4636_112_2_17_22_23_23_1651,"MNPHONE:ASTERISK_HandleSimChvCnf,s_mnphone_state=%d")
TRACE_MSG(MNPHONE_MODULE_4716_112_2_17_22_23_23_1652,"MNPHONE: MnMmRegInd, read NV_RPLMN is_enable %d, mcc %d, mnc %d, mnc_digit_num %d")
TRACE_MSG(MNPHONE_MODULE_4725_112_2_17_22_23_24_1653,"MNPHONE: MnMmRegInd, read NV_RPLMN, nv error")
TRACE_MSG(MNPHONE_MODULE_4748_112_2_17_22_23_24_1654,"MNPHONE:ASTERISK_HandleMnMmRegInd error \n")
TRACE_MSG(MNPHONE_MODULE_4809_112_2_17_22_23_24_1655,"MNPHONE: MnMmNregInd, read NV_RPLMN is_enable %d, mcc %d, mnc %d, mnc_digit_num %d")
TRACE_MSG(MNPHONE_MODULE_4819_112_2_17_22_23_24_1656,"MNPHONE: MnMmNregInd, read NV_RPLMN, nv error")
TRACE_MSG(MNPHONE_MODULE_4868_112_2_17_22_23_24_1657,"mnphone: assert failure, MN_MM_NREG_IND_CAUSE_E")
TRACE_MSG(MNPHONE_MODULE_5146_112_2_17_22_23_24_1658,"MNPHONE:HandleGmmAttachCause, cause = %d\n")
TRACE_MSG(MNPHONE_MODULE_5387_112_2_17_22_23_25_1659,"MNPHONE: change select plmn mode SUCCESS")
TRACE_MSG(MNPHONE_MODULE_5407_112_2_17_22_23_25_1660,"MNPHONE: select plmn,mode_is_manual is %d")
TRACE_MSG(MNPHONE_MODULE_5485_112_2_17_22_23_25_1661,"MNPHONE:camponed a plmn in No Cell state,mcc=%d,mnc=%d,type=%dn")
TRACE_MSG(MNPHONE_MODULE_5511_112_2_17_22_23_25_1662,"MNPHONE: NO_SERVICE PLMN,do nothing.")
TRACE_MSG(MNPHONE_MODULE_5515_112_2_17_22_23_25_1663,"mnphone: assert failure, RR_ACT_TYPE_E")
TRACE_MSG(MNPHONE_MODULE_5534_112_2_17_22_23_25_1664,"MNPHONE: camponed a plmn in camped on state,mcc=%d,mnc=%d,type=%d,plmn_status=%dn")
TRACE_MSG(MNPHONE_MODULE_5555_112_2_17_22_23_25_1665,"MNPHONE: EMERGENCY_ONLY PLMN,do nothing,plmn_status=%d")
TRACE_MSG(MNPHONE_MODULE_5567_112_2_17_22_23_25_1666,"mnphone: assert failure, RR_ACT_TYPE_E")
TRACE_MSG(MNPHONE_MODULE_5635_112_2_17_22_23_25_1667,"mnphone: assert failure, RR_ACT_TYPE_E")
TRACE_MSG(MNPHONE_MODULE_5693_112_2_17_22_23_25_1668,"MNPHONE: old band is %d")
TRACE_MSG(MNPHONE_MODULE_5705_112_2_17_22_23_25_1669,"MNPHONE: param_ptr->reselect_param.band = %d")
TRACE_MSG(MNPHONE_MODULE_5748_112_2_17_22_23_26_1670,"mnphone: assert failure, PLMN_RESELECT_CAUSE_E")
TRACE_MSG(MNPHONE_MODULE_5777_112_2_17_22_23_26_1671,"mnphone: assert failure, PLMN_RESELECT_CAUSE_E")
TRACE_MSG(MNPHONE_MODULE_5964_112_2_17_22_23_26_1672,"MNPHONE:HandleGmmDetachCause cause = %d\n")
TRACE_MSG(MNPHONE_MODULE_6003_112_2_17_22_23_26_1673,"MNPHONE: reset ps module")
TRACE_MSG(MNPHONE_MODULE_6439_112_2_17_22_23_27_1674,"enter Phone_IsSctTchLoopBackOn,s_is_loop_back_on[dual_sys]=%d")
TRACE_MSG(MNPHONE_MODULE_6474_112_2_17_22_23_27_1675,"init SMS")
TRACE_MSG(MNPHONE_MODULE_6941_112_2_17_22_23_28_1676,"MNPHONE: HPLMN in forbidden plmn list, delete hplmn from forbidden plmn list")
TRACE_MSG(MNPHONE_MODULE_6954_112_2_17_22_23_28_1677,"MNPHONE: EHPLMN in forbidden plmn list, delete hplmn from forbidden plmn list")
TRACE_MSG(MNPHONE_MODULE_6981_112_2_17_22_23_28_1678,"MNSMS: SIM phase is phase 1, the last used TP_MR is saved in NV.")
TRACE_MSG(MNPHONE_MODULE_7101_112_2_17_22_23_28_1679,"MNPHONE: deactive start select plmn.")
TRACE_MSG(MNPHONE_MODULE_7152_112_2_17_22_23_28_1680,"SCI_CreatePeriodTimer,CheckDspIntTimer ")
TRACE_MSG(MNPHONE_MODULE_7240_112_2_17_22_23_28_1681,"MNPHONE: s_is_ps_activate[dual_sys] = TRUE")
TRACE_MSG(MNPHONE_MODULE_7574_112_2_17_22_23_29_1682,"PHONE:Deactive_HandleSimChvCnf,s_mnphone_state[dual_sys]=%d")
TRACE_MSG(MNPHONE_MODULE_7587_112_2_17_22_23_29_1683,"MNPHONE:unblock the CHV1 successful when deactive ")
TRACE_MSG(MNPHONE_MODULE_7630_112_2_17_22_23_29_1684,"PHONE:Deactive_HandleSimChvCnf,s_mnphone_state[dual_sys]=%d")
TRACE_MSG(MNPHONE_MODULE_7741_112_2_17_22_23_29_1685,"MNPHONE:StkRefreshIniCnf be called,result=%d!\n")
TRACE_MSG(MNPHONE_MODULE_7763_112_2_17_22_23_30_1686,"MNPHONE::fdn changed,is_fdn_enable = %d.")
TRACE_MSG(MNPHONE_MODULE_7785_112_2_17_22_23_30_1687,"MNPHONE::fdn not change,is_fdn_enable = %d.")
TRACE_MSG(MNPHONE_MODULE_7810_112_2_17_22_23_30_1688,"MNPHONE:StkRefreshFileChgCnf be called,result=%d!\n")
TRACE_MSG(MNPHONE_MODULE_7857_112_2_17_22_23_30_1689,"MNPHONE: EHPLMN(%d %d) in forbidden plmn list, delete")
TRACE_MSG(MNPHONE_MODULE_7870_112_2_17_22_23_30_1690,"MNPHONE: HPLMN(%d, %d) in forbidden plmn list, delete")
TRACE_MSG(MNPHONE_MODULE_7902_112_2_17_22_23_30_1691,"MNPHONE:StkRefreshIniFileChgCnf be called,result=%d!\n")
TRACE_MSG(MNPHONE_MODULE_7909_112_2_17_22_23_30_1692,"StkRefreshIniFileChgCnf::fdn changed,is_fdn_enable = %d.")
TRACE_MSG(MNPHONE_MODULE_7918_112_2_17_22_23_30_1693,"StkRefreshIniFileChgCnf::fdn not changed,is_fdn_enable = %d.")
TRACE_MSG(MNPHONE_MODULE_7981_112_2_17_22_23_30_1694,"MNPHONE:StkRefreshIniFullCnf be called,result=%d!\n")
TRACE_MSG(MNPHONE_MODULE_7989_112_2_17_22_23_30_1695,"StkRefreshIniFullCnf::fdn changed,is_fdn_enable = %d.")
TRACE_MSG(MNPHONE_MODULE_7998_112_2_17_22_23_30_1696,"StkRefreshIniFullCnf::fdn not changed,is_fdn_enable = %d.")
TRACE_MSG(MNPHONE_MODULE_8004_112_2_17_22_23_30_1697,"MNPHONE: imsi changed!")
TRACE_MSG(MNPHONE_MODULE_8195_112_2_17_22_23_31_1698,"MNPHONE: the file %d will not update in MMI.")
TRACE_MSG(MNPHONE_MODULE_8281_112_2_17_22_23_31_1699,"MNPHONE: read sim redefine file list file 0x%x.!")
TRACE_MSG(MNPHONE_MODULE_8285_112_2_17_22_23_31_1700,"MNPHONE: read sim redefine file list finished.")
TRACE_MSG(MNPHONE_MODULE_8304_112_2_17_22_23_31_1701,"MNPHONE: refresh read sim file finished.")
TRACE_MSG(MNPHONE_MODULE_8309_112_2_17_22_23_31_1702,"MNPHONE: read sim refresh table file 0x%x.!")
TRACE_MSG(MNPHONE_MODULE_8314_112_2_17_22_23_31_1703,"MNPHONE: SIM file initialization finished!")
TRACE_MSG(MNPHONE_MODULE_8321_112_2_17_22_23_31_1704,"MNPHONE: read sim ini table 0x%x.!")
TRACE_MSG(MNPHONE_MODULE_8447_112_2_17_22_23_31_1705,"MNPHONE: SIM file read finished!")
TRACE_MSG(MNPHONE_MODULE_8729_112_2_17_22_23_32_1706,"MNPHONE:spn_len =%d,spn = %x, %x, %x")
TRACE_MSG(MNPHONE_MODULE_8739_112_2_17_22_23_32_1707,"MNPHONE:spn_len is 0,so do not support spn.")
TRACE_MSG(MNPHONE_MODULE_8823_112_2_17_22_23_33_1708,"MNPHONE: is_opn_need = %d,is_opn_exist = %d")
TRACE_MSG(MNPHONE_MODULE_8854_112_2_17_22_23_33_1709,"MNPHONE:spn_supported = %d,plmn_is_roaming= %d.")
TRACE_MSG(MNPHONE_MODULE_9217_112_2_17_22_23_34_1710,"MNPHONE::is summer time.")
TRACE_MSG(MNPHONE_MODULE_9224_112_2_17_22_23_34_1711,"MNPHONE::is winter time.")
TRACE_MSG(MNPHONE_MODULE_9620_112_2_17_22_23_35_1712,"MNPHONE::HandleMePersonalisationVerify Result: %d")
TRACE_MSG(MNPHONE_SIGNAL_233_112_2_17_22_23_35_1713,"MN: get paging_tbf_mask from Classmark : 0x%x")
TRACE_MSG(MNPHONE_SIMLOCK_287_112_2_17_22_23_37_1714,"MNPHONE:WriteSIMLockCustomizeData encrypt fail %d")
TRACE_MSG(MNPHONE_SIMLOCK_354_112_2_17_22_23_37_1715,"MNPHONE:WriteSIMLockUserData encrypt fail %d")
TRACE_MSG(MNPHONE_SIMLOCK_404_112_2_17_22_23_38_1716,"MNPHONE:SIMLockNetworkCheck N_Pointer para")
TRACE_MSG(MNPHONE_SIMLOCK_414_112_2_17_22_23_38_1717,"MNPHONE:SIMLockNetworkCheck no code exist")
TRACE_MSG(MNPHONE_SIMLOCK_421_112_2_17_22_23_38_1718,"MNPHONE:SIMLockNetworkCheck num err=%d")
TRACE_MSG(MNPHONE_SIMLOCK_453_112_2_17_22_23_38_1719,"MNPHONE:SIMLockNetworkSubsetCheck N_Pointer para")
TRACE_MSG(MNPHONE_SIMLOCK_463_112_2_17_22_23_38_1720,"MNPHONE:SIMLockNetworkSubsetCheck no code exist")
TRACE_MSG(MNPHONE_SIMLOCK_470_112_2_17_22_23_38_1721,"MNPHONE:SIMLockNetworkSubsetCheck num err=%d")
TRACE_MSG(MNPHONE_SIMLOCK_505_112_2_17_22_23_38_1722,"MNPHONE:SIMLockSPCheck N_Pointer para")
TRACE_MSG(MNPHONE_SIMLOCK_515_112_2_17_22_23_38_1723,"MNPHONE:SIMLockSPCheck no code exist")
TRACE_MSG(MNPHONE_SIMLOCK_522_112_2_17_22_23_38_1724,"MNPHONE:SIMLockSPCheck num err=%d")
TRACE_MSG(MNPHONE_SIMLOCK_557_112_2_17_22_23_38_1725,"MNPHONE:SIMLockCorporateCheck N_Pointer para")
TRACE_MSG(MNPHONE_SIMLOCK_567_112_2_17_22_23_38_1726,"MNPHONE:SIMLockCorporateCheck no code exist")
TRACE_MSG(MNPHONE_SIMLOCK_574_112_2_17_22_23_38_1727,"MNPHONE:SIMLockCorporateCheck num err=%d")
TRACE_MSG(MNPHONE_SIMLOCK_608_112_2_17_22_23_38_1728,"MNPHONE:SIMLockUserCheck N_Pointer para")
TRACE_MSG(MNPHONE_SIMLOCK_618_112_2_17_22_23_38_1729,"MNPHONE:SIMLockUserCheck no code exist")
TRACE_MSG(MNPHONE_SIMLOCK_625_112_2_17_22_23_38_1730,"MNPHONE:SIMLockUserCheck num err=%d")
TRACE_MSG(MNPHONE_SIMLOCK_704_112_2_17_22_23_38_1731,"MNPHONE:CustomizeDataIntegrity CRC: %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_711_112_2_17_22_23_38_1732,"MNPHONE:CustomizeDataIntegrity encrypt fail %d")
TRACE_MSG(MNPHONE_SIMLOCK_746_112_2_17_22_23_38_1733,"MNPHONE:UserDataIntegrityCheck CRC: %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_753_112_2_17_22_23_38_1734,"MNPHONE:UserDataIntegrityCheck encrypt fail %d")
TRACE_MSG(MNPHONE_SIMLOCK_789_112_2_17_22_23_38_1735,"MNPHONE:GetUnlockKey type err=%d")
TRACE_MSG(MNPHONE_SIMLOCK_797_112_2_17_22_23_38_1736,"MNPHONE:GetUnlockKey IntegrityCheck=%d")
TRACE_MSG(MNPHONE_SIMLOCK_834_112_2_17_22_23_38_1737,"MNPHONE:GetUnlockKey type err=%d")
TRACE_MSG(MNPHONE_SIMLOCK_842_112_2_17_22_23_38_1738,"MNPHONE:GetUnlockKey IntegrityCheck=%d")
TRACE_MSG(MNPHONE_SIMLOCK_880_112_2_17_22_23_38_1739,"MNPHONE:PHONE_IsKeySame key2 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d")
TRACE_MSG(MNPHONE_SIMLOCK_884_112_2_17_22_23_39_1740,"MNPHONE:PHONE_IsKeySame encrypt fail %d")
TRACE_MSG(MNPHONE_SIMLOCK_928_112_2_17_22_23_39_1741,"MNPHONE:SIMLockFirstBoot no customize lock")
TRACE_MSG(MNPHONE_SIMLOCK_937_112_2_17_22_23_39_1742,"MNPHONE:SIMLockFirstBoot orange lock")
TRACE_MSG(MNPHONE_SIMLOCK_998_112_2_17_22_23_39_1743,"MNPHONE:SIMLockFirstBoot SP lock fail, invalid GID")
TRACE_MSG(MNPHONE_SIMLOCK_1024_112_2_17_22_23_39_1744,"MNPHONE:SIMLockFirstBoot CORPORATE lock fail, invalid GID")
TRACE_MSG(MNPHONE_SIMLOCK_1075_112_2_17_22_23_39_1745,"MNPHONE:SIMLockFirstBoot lock_status %d, updateNV=%d, c_NV=%d, u_NV=%d")
TRACE_MSG(MNPHONE_SIMLOCK_1095_112_2_17_22_23_39_1746,"MNPHONE:SIMLockDataReadAndIntegrityCheck control data read NV fail")
TRACE_MSG(MNPHONE_SIMLOCK_1110_112_2_17_22_23_39_1747,"MNPHONE:For debug, IMEI: %d %d %d %d %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1115_112_2_17_22_23_39_1748,"MNPHONE:For debug, Rand: %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1133_112_2_17_22_23_39_1749,"MNPHONE:For debug, nck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1151_112_2_17_22_23_39_1750,"MNPHONE:For debug, nsck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1169_112_2_17_22_23_39_1751,"MNPHONE:For debug, spck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1187_112_2_17_22_23_39_1752,"MNPHONE:For debug, cck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1205_112_2_17_22_23_39_1753,"MNPHONE:For debug, pck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1223_112_2_17_22_23_39_1754,"MNPHONE:For debug, un_nck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1241_112_2_17_22_23_39_1755,"MNPHONE:For debug, un_nsck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1259_112_2_17_22_23_39_1756,"MNPHONE:For debug, un_spck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1277_112_2_17_22_23_39_1757,"MNPHONE:For debug, un_cck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1295_112_2_17_22_23_39_1758,"MNPHONE:For debug, un_pck len: %d, val: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1301_112_2_17_22_23_39_1759,"MNPHONE:SIMLockDataReadAndIntegrityCheck control data check fail")
TRACE_MSG(MNPHONE_SIMLOCK_1311_112_2_17_22_23_39_1760,"MNPHONE:SIMLockDataReadAndIntegrityCheck customize data read NV fail")
TRACE_MSG(MNPHONE_SIMLOCK_1323_112_2_17_22_23_39_1761,"MNPHONE:For debug, COP_ID: %d C_L_Status: %d MAX_C %d TIME_INT %d TIME_MULT %d")
TRACE_MSG(MNPHONE_SIMLOCK_1334_112_2_17_22_23_39_1762,"MNPHONE:For debug, NetworkLocks Len %d, PLMN1 %d %d %d, PLMN2 %d %d %d, PLMN3 %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1351_112_2_17_22_23_39_1763,"MNPHONE:For debug, NetworkSubsetLocks Len %d, PLMN_SUB1 %d %d %d %d %d, PLMN_SUB2 %d %d %d %d %d, PLMN_SUB3 %d %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1365_112_2_17_22_23_39_1764,"MNPHONE:For debug, SP_Locks Len %d, SP1 %d %d %d %d, SP2 %d %d %d %d, SP3 %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1382_112_2_17_22_23_39_1765,"MNPHONE:For debug, CORPORATE_Locks Len %d, CORPORATE1 %d %d %d %d %d, CORPORATE2 %d %d %d %d %d, CORPORATE3 %d %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1391_112_2_17_22_23_39_1766,"MNPHONE:For debug, Customize Data dummy1 %d dummy2 %d dummy3 %d dummy4 %d CRC: %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1397_112_2_17_22_23_39_1767,"MNPHONE:SIMLockDataReadAndIntegrityCheck customize data check fail")
TRACE_MSG(MNPHONE_SIMLOCK_1407_112_2_17_22_23_39_1768,"MNPHONE:SIMLockDataReadAndIntegrityCheck user data read NV fail")
TRACE_MSG(MNPHONE_SIMLOCK_1425_112_2_17_22_23_39_1769,"MNPHONE:For debug, U_L_Status: %d nck: %d, %d nsck: %d, %d spck: %d, %d cck: %d, %d pck: %d, %d")
TRACE_MSG(MNPHONE_SIMLOCK_1434_112_2_17_22_23_39_1770,"MNPHONE:For debug, User Data dummy1 %d dummy2 %d dummy3 %d dummy4 %d CRC: %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1463_112_2_17_22_23_39_1771,"MNPHONE:For debug, User_Locks Len %d, IMSI1 len %d val %d %d %d %d %d %d %d %d, IMSI2 len %d val %d %d %d %d %d %d %d %d, IMSI3 len %d val %d %d %d %d %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_1469_112_2_17_22_23_39_1772,"MNPHONE:SIMLockDataReadAndIntegrityCheck user data check fail")
TRACE_MSG(MNPHONE_SIMLOCK_1530_112_2_17_22_23_39_1773,"MNPHONE:GetUnlockConfigData IntegrityCheck=%d")
TRACE_MSG(MNPHONE_SIMLOCK_1574_112_2_17_22_23_39_1774,"MNPHONE:UpdateUnlockRetryData type err=%d")
TRACE_MSG(MNPHONE_SIMLOCK_1584_112_2_17_22_23_39_1775,"MNPHONE:UpdateUnlockRetryData IntegrityCheck=%d")
TRACE_MSG(MNPHONE_SIMLOCK_1635_112_2_17_22_23_39_1776,"MNPHONE:SIMUnlockAction type err=%d")
TRACE_MSG(MNPHONE_SIMLOCK_1644_112_2_17_22_23_39_1777,"MNPHONE:SIMUnlockAction lock_status=%d, unlock_type=%d")
TRACE_MSG(MNPHONE_SIMLOCK_1671_112_2_17_22_23_40_1778,"MNPHONE:SIMUnlockAction updateNV fail")
TRACE_MSG(MNPHONE_SIMLOCK_1688_112_2_17_22_23_40_1779,"MNPHONE:SIMUnlockAction IntegrityCheck=%d")
TRACE_MSG(MNPHONE_SIMLOCK_1721_112_2_17_22_23_40_1780,"MNPHONE:SIMlockAction type err=%d")
TRACE_MSG(MNPHONE_SIMLOCK_1793_112_2_17_22_23_40_1781,"MNPHONE:SIMlockAction GID1 not support")
TRACE_MSG(MNPHONE_SIMLOCK_1819_112_2_17_22_23_40_1782,"MNPHONE:SIMlockAction GID1,2 not support")
TRACE_MSG(MNPHONE_SIMLOCK_1856_112_2_17_22_23_40_1783,"MNPHONE:SIMlockAction type err=%d")
TRACE_MSG(MNPHONE_SIMLOCK_1873_112_2_17_22_23_40_1784,"MNPHONE:SIMlockAction update customize NV fail")
TRACE_MSG(MNPHONE_SIMLOCK_1885_112_2_17_22_23_40_1785,"MNPHONE:SIMlockAction update user NV fail")
TRACE_MSG(MNPHONE_SIMLOCK_1900_112_2_17_22_23_40_1786,"MNPHONE:SIMlockAction already locked")
TRACE_MSG(MNPHONE_SIMLOCK_1908_112_2_17_22_23_40_1787,"MNPHONE:SIMlockAction IntegrityCheck=%d")
TRACE_MSG(MNPHONE_SIMLOCK_1943_112_2_17_22_23_40_1788,"MNPHONE:MEPersonalisationVerify Test card")
TRACE_MSG(MNPHONE_SIMLOCK_1962_112_2_17_22_23_40_1789,"MNPHONE:MEPersonalisationVerify NetworkLock MCC=%d, MNC=%d, MNC_LEN=%d")
TRACE_MSG(MNPHONE_SIMLOCK_1981_112_2_17_22_23_40_1790,"MNPHONE:MEPersonalisationVerify NetworkSubsetLock MCC=%d, MNC=%d, MNC_LEN=%d, SUB1=%d, SUB2=%d")
TRACE_MSG(MNPHONE_SIMLOCK_1996_112_2_17_22_23_40_1791,"MNPHONE:MEPersonalisationVerify SPLock MCC=%d, MNC=%d, MNC_LEN=%d, GID1=%d")
TRACE_MSG(MNPHONE_SIMLOCK_2011_112_2_17_22_23_40_1792,"MNPHONE:MEPersonalisationVerify CorporateLock MCC=%d, MNC=%d, MNC_LEN=%d, GID1=%d, GID2=%d")
TRACE_MSG(MNPHONE_SIMLOCK_2027_112_2_17_22_23_40_1793,"MNPHONE:MEPersonalisationVerify UserLock IMSI validflag:%d, len:%d, val:%d %d %d %d %d %d %d %d")
TRACE_MSG(MNPHONE_SIMLOCK_2067_112_2_17_22_23_40_1794,"MNPHONE:PHONE_IsOrangeAutoLock return TRUE,")
TRACE_MSG(MNPHONE_SIMLOCK_2073_112_2_17_22_23_40_1795,"MNPHONE:PHONE_IsOrangeAutoLock return FALSE, lock status=%d")
TRACE_MSG(MNPHONE_SIMLOCK_2079_112_2_17_22_23_40_1796,"MNPHONE:PHONE_IsOrangeAutoLock return FALSE, operator name=%s")
TRACE_MSG(MNPHONE_SIMLOCK_2085_112_2_17_22_23_40_1797,"MNPHONE:PHONE_IsOrangeAutoLock return FALSE, gid1 valid=%d")
TRACE_MSG(MNPHONE_SIMLOCK_2091_112_2_17_22_23_40_1798,"MNPHONE:PHONE_IsOrangeAutoLock return FALSE,access class=%d")
TRACE_MSG(MNPHONE_SIMLOCK_2098_112_2_17_22_23_40_1799,"MNPHONE:PHONE_IsOrangeAutoLock return FALSE,mcc=%d,mnc=%d")
TRACE_MSG(MNPHONE_SIMLOCK_2132_112_2_17_22_23_40_1800,"MNPHONE:PHONE_OrangeAutoLockAction update customize/user NV fail")
TRACE_MSG(MNPHONE_SIMLOCK_2145_112_2_17_22_23_41_1801,"MNPHONE:PHONE_IsOrangeAutoLock return FALSE,company id=%d,LES=%d")
TRACE_MSG(NVITEM_INIT_333_112_2_17_22_23_44_1802,"nvitem init value now\n")
TRACE_MSG(NVITEM_INIT_337_112_2_17_22_23_44_1803,"get classmark\n")
TRACE_MSG(NVITEM_INIT_339_112_2_17_22_23_44_1804,"get classmark over\n\n")
TRACE_MSG(NVITEM_INIT_340_112_2_17_22_23_44_1805,"get nvitems\n")
TRACE_MSG(NVITEM_INIT_342_112_2_17_22_23_44_1806,"get nvitems over\n\n")
TRACE_MSG(NVITEM_INIT_478_112_2_17_22_23_44_1807,"open nvitems file failed,so create new file\n")
TRACE_MSG(NVITEM_INIT_492_112_2_17_22_23_44_1808,"line %d is omittded\n")
TRACE_MSG(NVITEM_INIT_497_112_2_17_22_23_44_1809,"line %d is unknown\n")
TRACE_MSG(NVITEM_INIT_522_112_2_17_22_23_44_1810,"open classmark file failed\n")
TRACE_MSG(NVITEM_INIT_536_112_2_17_22_23_44_1811,"line %d is omittded\n")
TRACE_MSG(NVITEM_INIT_541_112_2_17_22_23_44_1812,"line %d is unknown\n")
TRACE_MSG(MMDUMMY_450_112_2_17_22_24_6_1813,"MM: decode Msg Header,MESSAGE_TOO_SHORT")
TRACE_MSG(MMDUMMY_478_112_2_17_22_24_6_1814,"MM: decode Msg Header,SKIP_INDICATOR_NOT_ZERO")
TRACE_MSG(MMDUMMY_512_112_2_17_22_24_6_1815,"MM: decode Msg Header,INVALID_MESSAGE_TYPE")
TRACE_MSG(MMDUMMY_524_112_2_17_22_24_6_1816,"MM: decode Msg Header,INVALID_PROTOCOL_DISCRIMINATOR")
TRACE_MSG(PSDUMMY_MAIN_316_112_2_17_22_24_11_1817,"psdummy: cann't find the msg %d\n")
TRACE_MSG(PSDUMMY_MAIN_348_112_2_17_22_24_11_1818,"MN receive unexpected signal %x\n")
TRACE_MSG(SMDUMMY_DECODE_57_112_2_17_22_24_13_1819,"SM:decode Transaction identifier, TIE reserved!")
TRACE_MSG(SMDUMMY_DECODE_84_112_2_17_22_24_13_1820,"SM:decode Transaction identifier, EXT is '0'!")
TRACE_MSG(SMDUMMY_DECODE_119_112_2_17_22_24_13_1821,"SM:Receive APN Length > APN buffer!")
TRACE_MSG(SMDUMMY_DECODE_185_112_2_17_22_24_13_1822,"SM:PDP address length too long!")
TRACE_MSG(SMDUMMY_DECODE_208_112_2_17_22_24_13_1823,"SM:decode pdp address,PPP,PDP type reserved!")
TRACE_MSG(SMDUMMY_DECODE_236_112_2_17_22_24_13_1824,"SM:decode pdp address,PDP org type reserved!")
TRACE_MSG(SMDUMMY_DECODE_318_112_2_17_22_24_13_1825,"SM: LLC SAPI Spare Field non-0!")
TRACE_MSG(SMDUMMY_DECODE_326_112_2_17_22_24_13_1826,"SM: LLC SAPI not assigned!")
TRACE_MSG(SMDUMMY_DECODE_338_112_2_17_22_24_13_1827,"SM: LLC SAPI value reserved,invalid!")
TRACE_MSG(SMDUMMY_DECODE_482_112_2_17_22_24_13_1828,"SM: is_r99 = %d")
TRACE_MSG(SMDUMMY_DECODE_508_112_2_17_22_24_14_1829,"SM:decode Qos Field invalid!")
TRACE_MSG(SMDUMMY_DECODE_540_112_2_17_22_24_14_1830,"SM:decode Qos Field invalid!")
TRACE_MSG(SMDUMMY_DECODE_547_112_2_17_22_24_14_1831,"SM:ignore extented qos fields!")
TRACE_MSG(SMDUMMY_DECODE_705_112_2_17_22_24_14_1832,"SM: PDP context accept,not support iei = %d")
TRACE_MSG(SMDUMMY_DECODE_837_112_2_17_22_24_14_1833,"SM: PDP context accept,not support iei = %d")
TRACE_MSG(SMDUMMY_ENCODE_250_112_2_17_22_24_15_1834,"SM:SM_EncodePdpaddr pdp_addr_length too long!")
TRACE_MSG(SMDUMMY_ENCODE_282_112_2_17_22_24_15_1835,"SM:SM_EncodePdpaddr pdp_addr_length too long!")
TRACE_MSG(SMDUMMY_ENCODE_297_112_2_17_22_24_15_1836,"SM:Assert Failure,SM_EncodePdpaddr")
TRACE_MSG(SMDUMMY_ENCODE_330_112_2_17_22_24_15_1837,"SM:SM_EncodeApn apn too long!")
TRACE_MSG(CALIBRATION_1497_112_2_17_22_24_47_1838,"L1:calibration,recv unknown sig %d")
TRACE_MSG(CALIBRATION_1695_112_2_17_22_24_48_1839,"L1:caliberation diag paramter is wrong!")
TRACE_MSG(CALIBRATION_1813_112_2_17_22_24_48_1840,"L1:band=%x,arfcn=%x,ind=%x,val=%x,type=%x,count=%x")
TRACE_MSG(CALIBRATION_2329_112_2_17_22_24_49_1841,"\"\"handle tx request band = %d")
TRACE_MSG(CALIBRATION_2429_112_2_17_22_24_49_1842,"\"\"handle tx request band = %d")
TRACE_MSG(CALIBRATION_2495_112_2_17_22_24_49_1843,"SEND 8PSK TX REQ")
TRACE_MSG(CALIBRATION_2534_112_2_17_22_24_49_1844,"\"STOP 8PSK TX ")
TRACE_MSG(CALIBRATION_2870_112_2_17_22_24_50_1845,"set EGSM index %d gain= %x")
TRACE_MSG(CALIBRATION_2884_112_2_17_22_24_50_1846,"\"RX REQ g_layer1_state =%d ")
TRACE_MSG(CALIBRATION_2975_112_2_17_22_24_50_1847,"set EGSM index %d gain= %x")
TRACE_MSG(CALIBRATION_2990_112_2_17_22_24_50_1848,"\"RX REQ g_layer1_state =%d ")
TRACE_MSG(CALIBRATION_3038_112_2_17_22_24_50_1849,"set EGSM index %d gain= %x")
TRACE_MSG(CALIBRATION_3058_112_2_17_22_24_50_1850,"set DCS index %d gain= %x")
TRACE_MSG(CALIBRATION_3077_112_2_17_22_24_51_1851,"set PCS index %d gain= %x")
TRACE_MSG(CALIBRATION_3097_112_2_17_22_24_51_1852,"set GSM850 index %d gain= %x")
TRACE_MSG(CALIBRATION_3115_112_2_17_22_24_51_1853,"\"RX REQ g_layer1_state =%d ")
TRACE_MSG(CALIBRATION_3288_112_2_17_22_24_51_1854,"L1:RSSI== %x")
TRACE_MSG(CALIBRATION_3532_112_2_17_22_24_51_1855,"L1:RF control Request,value = %d")
TRACE_MSG(CALIBRATION_3536_112_2_17_22_24_51_1856,"L1:RF control Request,length = %d")
TRACE_MSG(CALIBRATION_3548_112_2_17_22_24_51_1857,"L1:RF control Request,gpip_no = %d value = %d")
TRACE_MSG(CALIBRATION_3561_112_2_17_22_24_51_1858,"L1:RF control Request,tx = %d")
TRACE_MSG(CALIBRATION_3848_112_2_17_22_24_52_1859,"L1:RAMP_ADD=%x,OFFSET=%x")
TRACE_MSG(CALIBRATION_4059_112_2_17_22_24_52_1860,"rxtxloop, savesigal mode error = %d")
TRACE_MSG(CALIBRATION_4074_112_2_17_22_24_53_1861,"rxtxloop, new pcl %d")
TRACE_MSG(CALIBRATION_4094_112_2_17_22_24_53_1862,"rxtxloop, new cmd=%d")
TRACE_MSG(CALIBRATION_4111_112_2_17_22_24_53_1863,"rxtxloop wakedsp1 =%d")
TRACE_MSG(CALIBRATION_4181_112_2_17_22_24_53_1864,"rxtxloop init, save msg error.")
TRACE_MSG(CALIBRATION_4188_112_2_17_22_24_53_1865,"rxtxloop init, layer1 state error.")
TRACE_MSG(CALIBRATION_4193_112_2_17_22_24_53_1866,"rxtxloop init, substate error")
TRACE_MSG(CALIBRATION_4467_112_2_17_22_24_53_1867,"rxtxloop, update nv start")
TRACE_MSG(CALIBRATION_4471_112_2_17_22_24_53_1868,"rxtxloop, update nv end")
TRACE_MSG(CALIBRATION_4528_112_2_17_22_24_53_1869,"rxtxloop, req sig, cmd=%d")
TRACE_MSG(CALIBRATION_4564_112_2_17_22_24_53_1870,"loop s=%d fn=x%x")
TRACE_MSG(CALIBRATION_4608_112_2_17_22_24_54_1871,"change arfcn")
TRACE_MSG(CALIBRATION_4661_112_2_17_22_24_54_1872,"SetState=%d")
TRACE_MSG(CALIBRATION_4739_112_2_17_22_24_54_1873,"rxlev2diag 0x%x")
TRACE_MSG(CALIBRATION_4842_112_2_17_22_24_54_1874,"rxtxloop, errors %d, cmd %d, substate %d")
TRACE_MSG(CALIBRATION_4920_112_2_17_22_24_54_1875,"rxtxloop, verify sch delay=%d, fn %d")
TRACE_MSG(CALIBRATION_4952_112_2_17_22_24_54_1876,"rxtxloop2next=%d")
TRACE_MSG(CALIBRATION_5018_112_2_17_22_24_54_1877,"rxtxloop, no bcch or wrong askmore passed here , which triggered in search")
TRACE_MSG(CALIBRATION_5094_112_2_17_22_24_55_1878,"rxtxloop startcamp, target=%d")
TRACE_MSG(CALIBRATION_5151_112_2_17_22_24_55_1879,"rxtxloop, target=%d,gain=%d")
TRACE_MSG(CALIBRATION_5189_112_2_17_22_24_55_1880,"rxtxloop, meas n=%d")
TRACE_MSG(CALIBRATION_5265_112_2_17_22_24_55_1881,"rxtxloop:RxBlockReady")
TRACE_MSG(CALIBRATION_5307_112_2_17_22_24_55_1882,"rxtxloop, search/read state machine error.!!!")
TRACE_MSG(CALIBRATION_5312_112_2_17_22_24_55_1883,"rxtxloop, rxblock channel tag error !!!")
TRACE_MSG(CALIBRATION_5333_112_2_17_22_24_55_1884,"rxtxloop, sch verify sync error.")
TRACE_MSG(CALIBRATION_5343_112_2_17_22_24_55_1885,"rxtxloop, sch data sync error.")
TRACE_MSG(CALIBRATION_5482_112_2_17_22_24_55_1886,"rxtxloop, sch verify, deltaold=%d, deltanew=%d")
TRACE_MSG(CALIBRATION_5509_112_2_17_22_24_55_1887,"rxtxloop, sch successed, oldfn=%d, curfn=%d, deltafn=%d")
TRACE_MSG(CALIBRATION_5527_112_2_17_22_24_55_1888,"sch failed, cell=%d, count=%d")
TRACE_MSG(CALIBRATION_5559_112_2_17_22_24_55_1889,"rxtxloop meas count=%d, result=%d")
TRACE_MSG(CALIBRATION_5578_112_2_17_22_24_56_1890,"rxtxloop, not right arfcn queue")
TRACE_MSG(CALIBRATION_5606_112_2_17_22_24_56_1891,"rxtxloop, measreport lev x%x, qual x%x")
TRACE_MSG(CALIBRATION_5620_112_2_17_22_24_56_1892,"rxtxloop, readpwrque")
TRACE_MSG(CALIBRATION_5679_112_2_17_22_24_56_1893,"rxtxloop, BELOW_NOISE_LO")
TRACE_MSG(CALIBRATION_5684_112_2_17_22_24_56_1894,"rxtxloop, GOOD, arfcn=%d")
TRACE_MSG(CALIBRATION_5755_112_2_17_22_24_56_1895,"txtxloop, agc=%d")
TRACE_MSG(CALIBRATION_5777_112_2_17_22_24_56_1896,"rxtxloop, start search fn 0x%x")
TRACE_MSG(CALIBRATION_5819_112_2_17_22_24_56_1897,"loop, change arfcn %d, tn %d, tsc %d")
TRACE_MSG(CALIBRATION_5851_112_2_17_22_24_56_1898,"loop, arfcn %d, tn %d, tsc %d")
TRACE_MSG(CALIBRATION_5922_112_2_17_22_24_56_1899,"idle askmore1")
TRACE_MSG(CALIBRATION_5928_112_2_17_22_24_56_1900,"idle askmore2")
TRACE_MSG(CALIBRATION_6077_112_2_17_22_24_57_1901,"fastcali SaveParamSet cmd %d, count %d")
TRACE_MSG(CALIBRATION_6218_112_2_17_22_24_57_1902,"fastcali parse param %d, rxcount %d, tickcount %d")
TRACE_MSG(CALIBRATION_6228_112_2_17_22_24_57_1903,"fastcali parse count %d")
TRACE_MSG(CALIBRATION_6239_112_2_17_22_24_57_1904,"fastcali parse arfcn 0x%x mask %d slotnum %d, pcl[%d, %d, %d, %d]")
TRACE_MSG(CALIBRATION_6245_112_2_17_22_24_57_1905,"fastcali error %d")
TRACE_MSG(CALIBRATION_6360_112_2_17_22_24_57_1906,"fastcali err=%d, substate=%d")
TRACE_MSG(CALIBRATION_6386_112_2_17_22_24_57_1907,"fastcali idle cmd %d")
TRACE_MSG(CALIBRATION_6508_112_2_17_22_24_57_1908,"fastcali Read , arfcn %d, fn 0x%x")
TRACE_MSG(CALIBRATION_6558_112_2_17_22_24_57_1909,"fastcali Search , arfcn %d, fn 0x%x")
TRACE_MSG(CALIBRATION_6580_112_2_17_22_24_58_1910,"fastcali Search , end fn 0x%x")
TRACE_MSG(CALIBRATION_6602_112_2_17_22_24_58_1911,"fastcali last askmore.")
TRACE_MSG(CALIBRATION_6616_112_2_17_22_24_58_1912,"fastcali askmore rx fn %d, state %d")
TRACE_MSG(CALIBRATION_6652_112_2_17_22_24_58_1913,"fastcali askmore rx fn %d")
TRACE_MSG(CALIBRATION_6713_112_2_17_22_24_58_1914,"s_fastcali_slot_rxlev_counter=%d, count=%d")
TRACE_MSG(CALIBRATION_6722_112_2_17_22_24_58_1915,"RSSI[%d]=%d")
TRACE_MSG(CALIBRATION_6765_112_2_17_22_24_58_1916,"fastcali rHandlePowerQue state %d")
TRACE_MSG(CALIBRATION_6798_112_2_17_22_24_58_1917,"fastcali read from Power mon queue fail! ")
TRACE_MSG(CALIBRATION_6801_112_2_17_22_24_58_1918,"fastcali rHandlePowerQue counter %d")
TRACE_MSG(CALIBRATION_6875_112_2_17_22_24_58_1919,"FASTCALI_ProcessRxSch, synced_fn 0x%x, oldfn 0x%x, cur_fn 0x%x, searchcount %d, spentcount %d")
TRACE_MSG(CALIBRATION_6914_112_2_17_22_24_58_1920,"fastcali sch channel tag error !!!")
TRACE_MSG(CALIBRATION_6920_112_2_17_22_24_58_1921,"fastcali sch read buffer fail !!!")
TRACE_MSG(CALIBRATION_6952_112_2_17_22_24_58_1922,"fastcali sch resq sub state error %d")
TRACE_MSG(CALIBRATION_6971_112_2_17_22_24_58_1923,"fastcali GetRxTstFrmDelay startfn %d, curfn %d")
TRACE_MSG(CALIBRATION_7011_112_2_17_22_24_58_1924,"fastcali triggerrx error : can't find rx frame")
TRACE_MSG(CALIBRATION_7033_112_2_17_22_24_58_1925,"fastcali FindFirstRxFrameIndex %d isfind 1")
TRACE_MSG(CALIBRATION_7061_112_2_17_22_24_58_1926,"fastcali FindFirstRxFrameIndex %d isfind %d")
TRACE_MSG(CALIBRATION_7092_112_2_17_22_24_59_1927,"fastcali plot fn 0x%x")
TRACE_MSG(CALIBRATION_7116_112_2_17_22_24_59_1928,"fastcali plot tx 0x%x")
TRACE_MSG(CALIBRATION_7483_112_2_17_22_24_59_1929,"fastcali sch fail , substate %d, fcch ind %d, cell index %d, search_timeout %d")
TRACE_MSG(CALIBRATION_7505_112_2_17_22_24_59_1930,"fastcali sch read fail , substate %d, fcch ind %d, ")
TRACE_MSG(CALIBRATION_7524_112_2_17_22_24_59_1931,"fastcali FastCali_Initialize")
TRACE_MSG(CALIBRATION_7618_112_2_17_22_25_0_1932,"fastcali SchRxTriggerDelay fn %d, delay %d")
TRACE_MSG(CELLSYNCINFO_581_112_2_17_22_25_1_1933,"L1 init %d, cell_id %d,new arfcn_index %x;old %x,%x ")
TRACE_MSG(CELLSYNCINFO_1147_112_2_17_22_25_2_1934,"L1 cell_id invalid %x")
TRACE_MSG(CELLSYNCINFO_1161_112_2_17_22_25_2_1935,"L1 Get_RR_S State cell_index %d")
TRACE_MSG(CELLSYNCINFO_1710_112_2_17_22_25_3_1936,"L1: new ta %d")
TRACE_MSG(CELLSYNCINFO_1720_112_2_17_22_25_3_1937,"Handover SYNC cell old_ta = %d,new_ta %d,otd %d")
TRACE_MSG(CELLSYNCINFO_1730_112_2_17_22_25_3_1938,"Handover PSEUDO old ta %d,otd %d,rtd %d,new_ta %d")
TRACE_MSG(CELLSYNCINFO_1742_112_2_17_22_25_3_1939,"Handover PSEUDO cell old_ta = %d,new_ta %d,otd %d")
TRACE_MSG(CELLSYNCINFO_1871_112_2_17_22_25_4_1940,"L1 camp to same!")
TRACE_MSG(CELLSYNCINFO_2521_112_2_17_22_25_5_1941,"L1 bcch select %x")
TRACE_MSG(CELLSYNCINFO_2533_112_2_17_22_25_5_1942,"L1 receive main scell continue bcch")
TRACE_MSG(CELLSYNCINFO_2818_112_2_17_22_25_6_1943,"L1: CellSyncInfo_GetNextCBCHBlkDelay schedule %ld, %d")
TRACE_MSG(CELLSYNCINFO_2839_112_2_17_22_25_6_1944,"L1:CellSyncInfo_GetNextCBCHBlkDelay %d")
TRACE_MSG(CELLSYNCINFO_2850_112_2_17_22_25_6_1945,"L1: MPHHandle_IsDiscardCBCHBlock %d")
TRACE_MSG(CELLSYNCINFO_2918_112_2_17_22_25_6_1946,"L1:Handover fail, cell_id = %d,bsic %d,target bsic %d")
TRACE_MSG(CELLSYNCINFO_2956_112_2_17_22_25_6_1947,"L1:handover cell %d,arfcn %d otd %d,scell %d,arfcn %d,otd %d, ba_index %d")
TRACE_MSG(CELLSYNCINFO_2967_112_2_17_22_25_6_1948,"L1 no space to add a new arfcn to ba!")
TRACE_MSG(CELLSYNCINFO_3503_112_2_17_22_25_7_1949,"L1: 1:%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,nL1: 2:%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d")
TRACE_MSG(CELLSYNCINFO_3590_112_2_17_22_25_7_1950,"L1 start decode PSI,need decode %d")
TRACE_MSG(CELLSYNCINFO_4765_112_2_17_22_25_10_1951,"L1 invalid cell id %d")
TRACE_MSG(CELLSYNCINFO_4824_112_2_17_22_25_10_1952,"L1 sacch counter: %d, reset!")
TRACE_MSG(CELLSYNCINFO_5073_112_2_17_22_25_10_1953,"L1: invalid tc")
TRACE_MSG(CELLSYNCINFO_5296_112_2_17_22_25_11_1954,"L1 invlaid cell id %d")
TRACE_MSG(CELLSYNCINFO_5326_112_2_17_22_25_11_1955,"L1 save bcch tc %d,%x,fn %x")
TRACE_MSG(CELLSYNCINFO_5377_112_2_17_22_25_11_1956,"L1 invalid cell_id %d")
TRACE_MSG(CELLSYNCINFO_5423_112_2_17_22_25_11_1957,"L1 s_b %x, tc %d")
TRACE_MSG(CELLSYNCINFO_5428_112_2_17_22_25_11_1958,"L1 bcch has been invalid %d %d,%x")
TRACE_MSG(CELLSYNCINFO_5435_112_2_17_22_25_11_1959,"L1 invalid cell_id %d")
TRACE_MSG(CELLSYNCINFO_5474_112_2_17_22_25_11_1960,"L1 b_t  send %d, %d")
TRACE_MSG(CELLSYNCINFO_5530_112_2_17_22_25_11_1961,"L1 b_t  set %d, %d")
TRACE_MSG(CELLSYNCINFO_5533_112_2_17_22_25_11_1962,"L1 bcch_buf_index_queue full!")
TRACE_MSG(CELLSYNCINFO_5566_112_2_17_22_25_11_1963,"L1 b_t set")
TRACE_MSG(CELLSYNCINFO_5570_112_2_17_22_25_11_1964,"L1 b_t creat fail")
TRACE_MSG(CELLSYNCINFO_5586_112_2_17_22_25_11_1965,"L1 b_t a")
TRACE_MSG(CELLSYNCINFO_5593_112_2_17_22_25_11_1966,"L1 b_t  a f")
TRACE_MSG(CELLSYNCINFO_5608_112_2_17_22_25_11_1967,"L1 b_t dea")
TRACE_MSG(CELLSYNCINFO_5613_112_2_17_22_25_11_1968,"L1 b_t dea")
TRACE_MSG(CELLSYNCINFO_5618_112_2_17_22_25_11_1969,"L1 b_t de f")
TRACE_MSG(CELLSYNCINFO_5885_112_2_17_22_25_12_1970,"L1 pch delay %x, %x, %x")
TRACE_MSG(CELLSYNCINFO_5961_112_2_17_22_25_12_1971,"L1 scell of card %d exist!%d")
TRACE_MSG(CELLSYNCINFO_6030_112_2_17_22_25_12_1972,"L1 pch p_p co %x, se %x")
TRACE_MSG(CELLSYNCINFO_6045_112_2_17_22_25_12_1973,"L1 pch p_b co %x")
TRACE_MSG(CELLSYNCINFO_6224_112_2_17_22_25_13_1974,"L1 invalid fn %x,%x")
TRACE_MSG(CELLSYNCINFO_6235_112_2_17_22_25_13_1975,"L1 invalid fn %x")
TRACE_MSG(CELLSYNCINFO_6246_112_2_17_22_25_13_1976,"L1 invalid fn %x")
TRACE_MSG(CELLSYNCINFO_6262_112_2_17_22_25_13_1977,"L1 fn adjust detected!")
TRACE_MSG(CMDQ_135_112_2_17_22_25_13_1978,"L1 deactive all")
TRACE_MSG(CMDQ_342_112_2_17_22_25_14_1979,"Layer1_cmd, audio in %d, audio out %d, is_enable %d")
TRACE_MSG(CMDQ_353_112_2_17_22_25_14_1980,"arm control audio , cancel audio enable to dsp!")
TRACE_MSG(CMDQ_394_112_2_17_22_25_14_1981,"layer1_cmd: command audio enable/disable")
TRACE_MSG(CMDQ_417_112_2_17_22_25_14_1982,"layer1_cmd: command audio enable/disable")
TRACE_MSG(CMDQ_1097_112_2_17_22_25_15_1983,"L1:SEND CMDQ_WAKEUP_DSP!\n")
TRACE_MSG(CMDQ_1212_112_2_17_22_25_16_1984,"L1:start or stop transfer data! ")
TRACE_MSG(CMDQ_1341_112_2_17_22_25_16_1985,"L1: send open loop back cmd to dsp")
TRACE_MSG(CMDQ_1424_112_2_17_22_25_16_1986,"L1: CmdQ_StartVoiceUpLoad, p0 0x%x, p1 0x%x, p2 0x%x, p3 0x%x")
TRACE_MSG(CMDQ_1449_112_2_17_22_25_16_1987,"L1: CmdQ_StartVoiceUpLoad, p0 0x%x, p1 0x%x, p2 0x%x, p3 0x%x")
TRACE_MSG(CMDQ_1475_112_2_17_22_25_16_1988,"L1:CmdQ_StartVoiceDownLoad")
TRACE_MSG(CMDQ_1502_112_2_17_22_25_16_1989,"L1:CmdQ_NotifyPchReorgMode")
TRACE_MSG(CMDQ_1736_112_2_17_22_25_17_1990,"L1:CMDQ_Dedicate_Establish: 0x%x, is_dsp_addr_sent %d")
TRACE_MSG(CMDQ_1760_112_2_17_22_25_17_1991,"L1:CmdQ_StartMagicVoiceFun flag %d,type %d,uplink_gain %d,downlink_gain %d")
TRACE_MSG(DATACNFPROC_118_112_2_17_22_25_17_1992,"L1: SendRachCNF!")
TRACE_MSG(DATACNFPROC_245_112_2_17_22_25_17_1993,"L1: Handover_ACCESS_CNF!")
TRACE_MSG(DATACNFPROC_286_112_2_17_22_25_18_1994,"L1: MPH_ERROR_IND!")
TRACE_MSG(DATACNFPROC_300_112_2_17_22_25_18_1995,"L1: MPH_RXPOWER_SWEEP_CNF!")
TRACE_MSG(DATACNFPROC_405_112_2_17_22_25_18_1996,"L1:FreqChangeCnf!")
TRACE_MSG(DATACNFPROC_572_112_2_17_22_25_18_1997,"L1:DataCnfProc_MphSmsCbchDataInd normal message! %lx, %lx,%lx, %lx")
TRACE_MSG(DATACNFPROC_578_112_2_17_22_25_18_1998,"L1:DataCnfProc_MphSmsCbchDataInd null message")
TRACE_MSG(DATACNFPROC_650_112_2_17_22_25_18_1999,"L1: MPH_CHAN_ASSIGN_FAIL_CNF!")
TRACE_MSG(DATACNFPROC_880_112_2_17_22_25_19_2000,"L1: MPH_CIPHER_MODE_CNF!")
TRACE_MSG(DATACNFPROC_898_112_2_17_22_25_19_2001,"L1 set task id %x DataCnfProc_SetTaskIdForReadDspMemory")
TRACE_MSG(DATACNFPROC_922_112_2_17_22_25_19_2002,"L1: dsp memory's response,value address 0x%x value %d")
TRACE_MSG(DATACNFPROC_926_112_2_17_22_25_19_2003,"L1: dsp memory's response,value address 0x%x value %d")
TRACE_MSG(DATACNFPROC_930_112_2_17_22_25_19_2004,"L1: dsp memory's response,value address 0x%x value %d")
TRACE_MSG(DATACNFPROC_937_112_2_17_22_25_19_2005,"L1:dsp memory's response,value %d")
TRACE_MSG(DATACNFPROC_951_112_2_17_22_25_19_2006,"L1:dsp memory's response,value Address %d")
TRACE_MSG(DATACNFPROC_971_112_2_17_22_25_19_2007,"L1:layer1 should read DSP side memory!")
TRACE_MSG(DATACNFPROC_1111_112_2_17_22_25_19_2008,"L1: MPH_CHANNEL_MODE_CNF!")
TRACE_MSG(DATACNFPROC_1149_112_2_17_22_25_19_2009,"L1: non sync otd %d,%d,otd %d")
TRACE_MSG(DATACNFPROC_1164_112_2_17_22_25_19_2010,"L1: MPH_HANDOVER_CNF!OTD = %d, old_TA = %d, MTD = %d")
TRACE_MSG(DATACNFPROC_1182_112_2_17_22_25_19_2011,"L1: MPH_HANDOVER_FAIL_CNF!")
TRACE_MSG(DATACNFPROC_1201_112_2_17_22_25_20_2012,"L1:PH_SCT_TCH_LOOP_ACK!")
TRACE_MSG(DATACNFPROC_1245_112_2_17_22_25_20_2013,"L1:DataCnfProc_L1MnGetAFCCnf, AFC_Value = %d!")
TRACE_MSG(DATACNFPROC_1297_112_2_17_22_25_20_2014,"L1:DataCnfProc_L1MnGetAFCCnf, PA_Value = %d")
TRACE_MSG(DATACNFPROC_1328_112_2_17_22_25_20_2015,"L1:DataCnfProc_L1MnGetAGCCnf,AGC_Value = %d!")
TRACE_MSG(DATACNFPROC_1358_112_2_17_22_25_20_2016,"L1:L1MnGetBlockErrorRatioCnf,total count= %d error count = %d!")
TRACE_MSG(DATACNFPROC_1377_112_2_17_22_25_20_2017,"L1:DspVoiceLoopbackInd! Index = %d\n")
TRACE_MSG(DATACNFPROC_1432_112_2_17_22_25_20_2018,"L1:DspVoiceLoopbackNxpInd! Index = %d\n")
TRACE_MSG(DATACNFPROC_1440_112_2_17_22_25_20_2019,"L1:dsp index value in%d,out %d,total%d\n")
TRACE_MSG(DATACNFPROC_1449_112_2_17_22_25_20_2020,"L1:dsp index value in%d,out %d,total%d\n")
TRACE_MSG(DATAPROC_147_112_2_17_22_25_21_2021,"L1:Rx SA!,%d")
TRACE_MSG(DATAPROC_159_112_2_17_22_25_21_2022,"L1:Rx bad SA counter = %d")
TRACE_MSG(DATAPROC_200_112_2_17_22_25_21_2023,"L1:Rx bad FA,0x%x,0x%x")
TRACE_MSG(DATAPROC_230_112_2_17_22_25_21_2024,"bad half rate FA,%3x,%3x")
TRACE_MSG(DATAPROC_332_112_2_17_22_25_21_2025,"L1: AMR startreqacktimer fn 0x%x")
TRACE_MSG(DATAPROC_577_112_2_17_22_25_21_2026,"L1:DataProc_RxRatscch: ACK_ERR or ACK_UNKNOW, type = 0x%x!")
TRACE_MSG(DATAPROC_584_112_2_17_22_25_21_2027,"L1:DataProc_RxRatscch: Ignore RATSCCH in REQ_ACK periord, s_active_req = %d!")
TRACE_MSG(DATAPROC_629_112_2_17_22_25_21_2028,"L1:bad RATSCCH,%3x,%3x")
TRACE_MSG(DATAPROC_660_112_2_17_22_25_22_2029,"L1: bad SD,0x%x,0x%x")
TRACE_MSG(DATAPROC_765_112_2_17_22_25_22_2030,"L1 invalid ref_scell_id %x main_card_id %x")
TRACE_MSG(DATAPROC_824_112_2_17_22_25_22_2031,"L1:bsic suc %x, id %x,%x")
TRACE_MSG(DATAPROC_873_112_2_17_22_25_22_2032,"L1 search invalid %x")
TRACE_MSG(DATAPROC_885_112_2_17_22_25_22_2033,"L1: A change of bsic is detected!, old bsic %d, new bsic %d")
TRACE_MSG(DATAPROC_914_112_2_17_22_25_22_2034,"L1 search invalid %x")
TRACE_MSG(DATAPROC_926_112_2_17_22_25_22_2035,"L1: A change of bsic is detected!, old bsic %d, new bsic %d")
TRACE_MSG(DATAPROC_953_112_2_17_22_25_22_2036,"L1:This cell maybe been replaced! Cell Id %d, arfcn %d!")
TRACE_MSG(DATAPROC_965_112_2_17_22_25_22_2037,"L1:Note got sch but this cell's state is changed to NULL cell!")
TRACE_MSG(DATAPROC_1029_112_2_17_22_25_22_2038,"L1:read tch suc: %x,cell %x,arfcn %x,otd %x,%x, ncc %x")
TRACE_MSG(DATAPROC_1072_112_2_17_22_25_22_2039,"L1 search invalid %x")
TRACE_MSG(DATAPROC_1133_112_2_17_22_25_22_2040,"L1 search invalid %x")
TRACE_MSG(DATAPROC_1170_112_2_17_22_25_23_2041,"L1: A change of bsic is detected!, old bsic %d, new bsic %d")
TRACE_MSG(DATAPROC_1199_112_2_17_22_25_23_2042,"This cell maybe have been substitut! Cell Id %d, arfcn %d!")
TRACE_MSG(DATAPROC_1206_112_2_17_22_25_23_2043,"L1: Tch BSIC read success! Cell %d Arfcn = %d ncc = %d\r ")
TRACE_MSG(DATAPROC_1213_112_2_17_22_25_23_2044,"L1: got a sch but this cell's state is changed to NULL cell!")
TRACE_MSG(DATAPROC_1227_112_2_17_22_25_23_2045,"L1 handover cell search suc !")
TRACE_MSG(DATAPROC_1231_112_2_17_22_25_23_2046,"L1 handover cell search fail !cell is not permit !%x")
TRACE_MSG(DATAPROC_1265_112_2_17_22_25_23_2047,"L1 bcch is not required by this cell %d, map %x,%x,%x")
TRACE_MSG(DATAPROC_1280_112_2_17_22_25_23_2048,"L1:bad BCCH! Fn = 0x%lx, arfcn_index = %d!")
TRACE_MSG(DATAPROC_1354_112_2_17_22_25_23_2049,"L1 bcch is not required by this cell %d, map %x,%x,%x")
TRACE_MSG(DATAPROC_1364_112_2_17_22_25_23_2050,"L1:bad BCCH! %x,%x, fn = %d, arfcn_index = %d!")
TRACE_MSG(DATAPROC_1444_112_2_17_22_25_23_2051,"L1 bcch is not required by this cell %d, map %x,%x,%x")
TRACE_MSG(DATAPROC_1463_112_2_17_22_25_23_2052,"L1:bad BCCH! %x,%x, fn = 0x%x,arfcn_index = 0x%x!")
TRACE_MSG(DATAPROC_1525_112_2_17_22_25_23_2053,"L1: Discard a agch block! due to error cell_index! scell_id = %d, error_cell_id = %d ")
TRACE_MSG(DATAPROC_1569_112_2_17_22_25_23_2054,"L1:bad AGCH! Fn = %lx, dsc %d")
TRACE_MSG(DATAPROC_1625_112_2_17_22_25_23_2055,"L1: maintain DSC on pg_reorg with SCell changed")
TRACE_MSG(DATAPROC_1642_112_2_17_22_25_23_2056,"L1: maintain DSC on pg_reorg")
TRACE_MSG(DATAPROC_1834_112_2_17_22_25_24_2057,"L1:Wrong main pch frame number=0x%x")
TRACE_MSG(DATAPROC_1844_112_2_17_22_25_24_2058,"L1 the card is deactived %x")
TRACE_MSG(DATAPROC_1904_112_2_17_22_25_24_2059,"L1 bad pch 0x%x,dsc %d, card id %x ")
TRACE_MSG(DATAPROC_2336_112_2_17_22_25_25_2060,"L1: AMR getreqackdeltaspeechframe, new_fn 0x%x, old_fn 0x%x")
TRACE_MSG(DATAPROC_2340_112_2_17_22_25_25_2061,"L1: AMR fn small")
TRACE_MSG(DATAPROC_2365_112_2_17_22_25_25_2062,"GetREQACTDelta: fn_26=%d, fn_sp=%d, old_26=%d, old_sp=%d!")
TRACE_MSG(DATAPROC_2368_112_2_17_22_25_25_2063,"GetREQACTDelta: old_fn=0x%x, delta=%d!")
TRACE_MSG(DATAPROC_2386_112_2_17_22_25_25_2064,"GetACKACTDelta: fn=0x%x!")
TRACE_MSG(DATAPROC_2392_112_2_17_22_25_25_2065,"L1: AMR getackackdeltaspeechframe, new_fn 0x%x, old_fn 0x%x")
TRACE_MSG(DATAPROC_2397_112_2_17_22_25_25_2066,"L1: AMR fn small")
TRACE_MSG(DATAPROC_2422_112_2_17_22_25_25_2067,"GetACKACTDelta: fn_26=%d, fn_sp=%d, old_26=%d, old_sp=%d!")
TRACE_MSG(DATAPROC_2425_112_2_17_22_25_25_2068,"GetACKACTDelta: old_fn=0x%x, delta=%d!")
TRACE_MSG(DATAPROC_2528_112_2_17_22_25_25_2069,"DataProc_TxFacch: applay new cmip = %d, fn = 0x%x!")
TRACE_MSG(DATAPROC_2560_112_2_17_22_25_25_2070,"DataProc_TxFacch: DL, icm=%d, acs=0x%x, fn = 0x%x!")
TRACE_MSG(DATAPROC_2582_112_2_17_22_25_25_2071,"DataProc_TxFacch: UL, icm=%d, acs=0x%x, fn=0x%x!")
TRACE_MSG(DATAPROC_2611_112_2_17_22_25_25_2072,"DataProc_TxFacch: threshreq, one=%d,two=%d,three=%d, fn = 0x%x!")
TRACE_MSG(DATAPROC_2695_112_2_17_22_25_26_2073,"DataProc_TxFacch: s_ratscch_ack_type=%d, fn=0x%x!")
TRACE_MSG(DATAPROC_2708_112_2_17_22_25_26_2074,"DataProc_TxFacch ratmark: s_ratscch_ack_type=%d, fn=0x%x!")
TRACE_MSG(DATAPROC_2734_112_2_17_22_25_26_2075,"DataProc_TxFacch ratdata: s_ratscch_ack_type=%d, fn=0x%x!")
TRACE_MSG(DATAPROC_2860_112_2_17_22_25_26_2076,"L1:channel change! %d")
TRACE_MSG(DATAPROC_3006_112_2_17_22_25_26_2077,"L1: bad PDTCH data!")
TRACE_MSG(DATAPROC_3062_112_2_17_22_25_26_2078,"L1:fn 0x%x,dump unneed slots %d,slot mask:0x%x")
TRACE_MSG(DATAPROC_3160_112_2_17_22_25_27_2079,"L1: same rx slot, fn %x,")
TRACE_MSG(DATAPROC_3220_112_2_17_22_25_27_2080,"L1: ptcch fn %x: ta = %d")
TRACE_MSG(DATAPROC_3232_112_2_17_22_25_27_2081,"L1: Invalid ptcch value = %d!")
TRACE_MSG(DATAPROC_3238_112_2_17_22_25_27_2082,"L1: bad ptcch!")
TRACE_MSG(DATAPROC_3268_112_2_17_22_25_27_2083,"L1: bad pbcch! ")
TRACE_MSG(DATAPROC_3290_112_2_17_22_25_27_2084,"L1: A WRONG bcch_dec_state.dec_mode!")
TRACE_MSG(DATAPROC_3319_112_2_17_22_25_27_2085,"L1: bad pbcch! ")
TRACE_MSG(DATAPROC_3340_112_2_17_22_25_27_2086,"L1:psi decode num %d")
TRACE_MSG(DATAPROC_3347_112_2_17_22_25_27_2087,"L1:bad pbcch!")
TRACE_MSG(DATAPROC_3381_112_2_17_22_25_27_2088,"L1:bad pbcch!")
TRACE_MSG(DATAPROC_3438_112_2_17_22_25_27_2089,"L1: bad ppch")
TRACE_MSG(DATAPROC_3518_112_2_17_22_25_27_2090,"L1:bad ppch")
TRACE_MSG(DATAPROC_3628_112_2_17_22_25_27_2091,"L1:bad ppch")
TRACE_MSG(DATAPROC_3740_112_2_17_22_25_28_2092,"L1: a bad PAGCH in PRACH_PAGCH ")
TRACE_MSG(DATAPROC_3771_112_2_17_22_25_28_2093,"L1:RXCBCH!card_id:%d")
TRACE_MSG(DATAPROC_3777_112_2_17_22_25_28_2094,"L1:bad CBCH!!")
TRACE_MSG(DOWNLOADPARAM_2583_112_2_17_22_25_34_2095,"L1:Threshold = %d")
TRACE_MSG(DOWNLOADPARAM_2626_112_2_17_22_25_34_2096,"L1:Encount error when read NV EQ parameters!")
TRACE_MSG(DOWNLOADPARAM_2698_112_2_17_22_25_34_2097,"L1: Read NV selected band = %d")
TRACE_MSG(DOWNLOADPARAM_2707_112_2_17_22_25_34_2098,"L1: Fail in NV selected band reading, set is as Dual Band")
TRACE_MSG(DOWNLOADPARAM_2722_112_2_17_22_25_34_2099,"L1: Read NV selected deal flag = %d")
TRACE_MSG(DOWNLOADPARAM_2731_112_2_17_22_25_34_2100,"L1: Fail in NV selected deal flag reading, set is as zeor")
TRACE_MSG(DSPINTISR_531_112_2_17_22_25_36_2101,"L1: %d times .lstate=%d")
TRACE_MSG(DSPINTISR_599_112_2_17_22_25_36_2102,"L1 invalid irq!%x")
TRACE_MSG(DSPPROC_236_112_2_17_22_25_36_2103,"L1:access burst %d,%d")
TRACE_MSG(DSPPROC_254_112_2_17_22_25_36_2104,"L1:access burst %d,%d,%d")
TRACE_MSG(DSPPROC_310_112_2_17_22_25_37_2105,"L1:access burst %d,%d")
TRACE_MSG(DSPPROC_326_112_2_17_22_25_37_2106,"L1:access burst %d,%d,%d")
TRACE_MSG(DSPPROC_383_112_2_17_22_25_37_2107,"L1:param is ready!!")
TRACE_MSG(DSPPROC_413_112_2_17_22_25_37_2108,"L1:param is ready!!")
TRACE_MSG(DSPPROC_426_112_2_17_22_25_37_2109,"L1:WriteInitParaToDSP!g_is_download_param_done=%d")
TRACE_MSG(DSPPROC_444_112_2_17_22_25_37_2110,"L1:param is ready!")
TRACE_MSG(FAKE_RR_329_112_2_17_22_25_38_2111,"fake RR: sig code %x come in ")
TRACE_MSG(FAKE_RR_393_112_2_17_22_25_38_2112,"CALIBRATION: recv unknown signal ")
TRACE_MSG(FAKE_RR_455_112_2_17_22_25_38_2113,"fack RR: battery req send ")
TRACE_MSG(FRAMEINSTRUTIL_550_112_2_17_22_25_40_2114,"LAYER1: It's a wrong POWER SWEEP Number with 0! ")
TRACE_MSG(FRAMEINSTRUTIL_609_112_2_17_22_25_40_2115,"L1 rx_type %x")
TRACE_MSG(FRAMEINSTRUTIL_1229_112_2_17_22_25_42_2116,"L1:card_id:%d Plot4FrInstCBCH %ld,fncard:%ld")
TRACE_MSG(FRAMEINSTRUTIL_1459_112_2_17_22_25_42_2117,"L1 rx_type %x")
TRACE_MSG(FRAMEINSTRUTIL_1462_112_2_17_22_25_42_2118,"L1 pch %x")
TRACE_MSG(FRAMEINSTRUTIL_1580_112_2_17_22_25_43_2119,"L1 rx_type %x")
TRACE_MSG(FRAMEINSTRUTIL_1882_112_2_17_22_25_43_2120,"L1 search %x")
TRACE_MSG(FRAMEINSTRUTIL_1977_112_2_17_22_25_44_2121,"L1 search %x")
TRACE_MSG(FRAMEINSTRUTIL_2025_112_2_17_22_25_44_2122,"L1: BCCH %x, %x,%x")
TRACE_MSG(FRAMEINSTRUTIL_2153_112_2_17_22_25_44_2123,"L1:IT'S WRONG DEDICATE_MODE TYPE! ")
TRACE_MSG(FRAMEINSTRUTIL_2193_112_2_17_22_25_44_2124,"L1 invalid sd channel type %x")
TRACE_MSG(FRAMEINSTRUTIL_2214_112_2_17_22_25_44_2125,"L1 fn %x, min %x;")
TRACE_MSG(FRAMEINSTRUTIL_2658_112_2_17_22_25_45_2126,"L1 sdccch4 map: %d,%d,%d; %d,%d,%d")
TRACE_MSG(FRAMEINSTRUTIL_2849_112_2_17_22_25_45_2127,"L1 sdccch8 map: %d,%d,%d; %d,%d,%d")
TRACE_MSG(FRAMEINSTRUTIL_3179_112_2_17_22_25_46_2128,"L1: SDCCH searching!fn %x, min_delay %x")
TRACE_MSG(FRAMEINSTRUTIL_3322_112_2_17_22_25_46_2129,"L1: IT's A WRONG s_dedicate_chan_map[fn_mod].channel_type!")
TRACE_MSG(FRAMEINSTRUTIL_3384_112_2_17_22_25_46_2130,"L1: IT's A WRONG s_dedicate_chan_map[fn_mod].channel_type!")
TRACE_MSG(FRAMEINSTRUTIL_3515_112_2_17_22_25_47_2131,"L1 tch read: %x")
TRACE_MSG(FRAMEINSTRUTIL_3538_112_2_17_22_25_47_2132,"L1 tch search: %x")
TRACE_MSG(FRAMEINSTRUTIL_4730_112_2_17_22_25_49_2133,"L1 set sacch int at handover fn %x")
TRACE_MSG(FRAMEINSTRUTIL_4851_112_2_17_22_25_49_2134,"L1: TCH bsic read!Cell id %d")
TRACE_MSG(FRAMEINSTRUTIL_4863_112_2_17_22_25_49_2135,"L1: TCH searching!")
TRACE_MSG(FRAMEINSTRUTIL_4875_112_2_17_22_25_49_2136,"L1: TCH start search,cell_id %d")
TRACE_MSG(FRAMEINSTRUTIL_5111_112_2_17_22_25_50_2137,"L1 sa dl %x")
TRACE_MSG(FRAMEINSTRUTIL_5198_112_2_17_22_25_50_2138,"L1 sa dl %x")
TRACE_MSG(FRAMEINSTRUTIL_5479_112_2_17_22_25_50_2139,"L1 sa tx %x")
TRACE_MSG(FRAMEINSTRUTIL_5601_112_2_17_22_25_51_2140,"L1 sa dl %x")
TRACE_MSG(FRAMEINSTRUTIL_5605_112_2_17_22_25_51_2141,"L1 sa tx %x")
TRACE_MSG(FRAMEINSTRUTIL_6044_112_2_17_22_25_52_2142,"L1 sdcch read %x")
TRACE_MSG(FRAMEINSTRUTIL_6830_112_2_17_22_25_53_2143,"L1:FrameInstrUtil_SetTxCmdPRACH:egprs rach,TS1")
TRACE_MSG(FRAMEINSTRUTIL_6835_112_2_17_22_25_53_2144,"L1:FrameInstrUtil_SetTxCmdPRACH:egprs rach,TS2")
TRACE_MSG(FRAMEINSTRUTIL_7150_112_2_17_22_25_54_2145,"L1:Rx data:fn curr:%d,tbf_start_fn :%d")
TRACE_MSG(FRAMEINSTRUTIL_7221_112_2_17_22_25_54_2146,"L1: uplink tbf, plot rx frame, ta %d, data %d, polling %d")
TRACE_MSG(FRAMEINSTRUTIL_7241_112_2_17_22_25_54_2147,"L1:No TA,not send dl polling 1")
TRACE_MSG(FRAMEINSTRUTIL_7260_112_2_17_22_25_54_2148,"L1:No TA,not send dl polling 2")
TRACE_MSG(FRAMEINSTRUTIL_7297_112_2_17_22_25_54_2149,"No TA,not send both link data 1")
TRACE_MSG(FRAMEINSTRUTIL_7316_112_2_17_22_25_54_2150,"L1:No TA,not send both link data 2")
TRACE_MSG(FRAMEINSTRUTIL_7868_112_2_17_22_25_55_2151,"L1 conflitnew rx slot 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_8618_112_2_17_22_25_57_2152,"L1:tbf read  = 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_8638_112_2_17_22_25_57_2153,"L1:tbf search  = 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_8997_112_2_17_22_25_57_2154,"L1:Assert Failure,Plot4AccessBurst ")
TRACE_MSG(FRAMEINSTRUTIL_9651_112_2_17_22_25_59_2155,"L1 PlotDspDebugFrames arfcn %x,band %d,gain %x,frametype %d")
TRACE_MSG(FRAMEINSTRUTIL_9784_112_2_17_22_25_59_2156,"L1:FrameInstrUtil_SetTxCmdRACH:egprs rach,TS1")
TRACE_MSG(FRAMEINSTRUTIL_9789_112_2_17_22_25_59_2157,"L1:FrameInstrUtil_SetTxCmdRACH:egprs rach,TS2")
TRACE_MSG(GPRSFRAMEGEN_251_112_2_17_22_26_0_2158,"L1:Waiting for wakeup's response!")
TRACE_MSG(GPRSFRAMEGEN_672_112_2_17_22_26_0_2159,"L1:starting time reach0: %d,%d")
TRACE_MSG(GPRSFRAMEGEN_762_112_2_17_22_26_1_2160,"single fn reach1 : %d")
TRACE_MSG(GPRSFRAMEGEN_834_112_2_17_22_26_1_2161,"single fn reach2 : %d")
TRACE_MSG(GPRSFRAMEGEN_919_112_2_17_22_26_1_2162,"L1 ptcch firstly fn %x delay %x")
TRACE_MSG(GPRSFRAMEGEN_1035_112_2_17_22_26_1_2163,"L1 invalid tbf state %d")
TRACE_MSG(GPRSSYSINFO_740_112_2_17_22_26_3_2164,"L1:Assert Failure,NO Allocted Block Memory.")
TRACE_MSG(GPRSSYSINFO_1252_112_2_17_22_26_4_2165,"L1: polling ack and rx collision tx_slot 0x%x, rx_slot 0x%x, new rx_slot 0x%x")
TRACE_MSG(GPRSSYSINFO_1356_112_2_17_22_26_4_2166,"L1API_GetTBFSlotMask: error tbf_direction = %d!")
TRACE_MSG(GPRSSYSINFO_1409_112_2_17_22_26_4_2167,"wrong tbf mode %d!")
TRACE_MSG(GPRSSYSINFO_1411_112_2_17_22_26_4_2168,"L1: reset tai,tbf_direction %d")
TRACE_MSG(GPRSSYSINFO_1433_112_2_17_22_26_4_2169,"L1 access polling send_type %d")
TRACE_MSG(GPRSSYSINFO_1442_112_2_17_22_26_4_2170,"L1 normal polling, ta_valid %d")
TRACE_MSG(GPRSSYSINFO_1512_112_2_17_22_26_4_2171,"L1 invalid tbf!")
TRACE_MSG(GPRSSYSINFO_1527_112_2_17_22_26_4_2172,"L1: exe tbf direction error")
TRACE_MSG(GPRSSYSINFO_1701_112_2_17_22_26_5_2173,"L1: ptcch firstly cur_fn %x")
TRACE_MSG(GSMFRAMEGEN_245_112_2_17_22_26_6_2174,"L1 GsmFrameGen_NullState arfcn 0x%x,band 0x%x,gain 0x%x")
TRACE_MSG(GSMFRAMEGEN_760_112_2_17_22_26_7_2175,"L1 pwr sub state %d")
TRACE_MSG(GSMFRAMEGEN_792_112_2_17_22_26_7_2176,"L1:optimize power on,enter POWERSWEEP_RESULT_PENDING")
TRACE_MSG(GSMFRAMEGEN_900_112_2_17_22_26_7_2177,"L1:Waiting for wakeup's response!!!")
TRACE_MSG(GSMFRAMEGEN_944_112_2_17_22_26_7_2178,"Waiting for wakeup's response!")
TRACE_MSG(GSMFRAMEGEN_1006_112_2_17_22_26_7_2179,"Waiting for wakeup's response!!!")
TRACE_MSG(GSMFRAMEGEN_1060_112_2_17_22_26_7_2180,"l1:PLMN CAMP, SEND ASKMORE TO SELF")
TRACE_MSG(GSMFRAMEGEN_1199_112_2_17_22_26_8_2181,"Waiting for wakeup's response!!!")
TRACE_MSG(GSMFRAMEGEN_1252_112_2_17_22_26_8_2182,"L1: Invalid PBCCH_CAMP sub state!")
TRACE_MSG(GSMFRAMEGEN_1399_112_2_17_22_26_8_2183,"L1:Waiting for wakeup's response!")
TRACE_MSG(GSMFRAMEGEN_1609_112_2_17_22_26_9_2184,"L1:Waiting for wakeup's response!")
TRACE_MSG(GSMFRAMEGEN_1734_112_2_17_22_26_9_2185,"L1 adjust first send fn %x,current fn %x")
TRACE_MSG(GSMFRAMEGEN_2178_112_2_17_22_26_10_2186,"L1:to cell's arfcn %d")
TRACE_MSG(GSMFRAMEGEN_2273_112_2_17_22_26_10_2187,"L1:current chan matched with next chan!")
TRACE_MSG(GSMFRAMEGEN_2401_112_2_17_22_26_10_2188,"L1 s_delay %x, p_delay %x, min_delay %x, count %x")
TRACE_MSG(GSMFRAMEGEN_2437_112_2_17_22_26_10_2189,"L1 sdcch block finish count %d,i %x")
TRACE_MSG(GSMFRAMEGEN_2444_112_2_17_22_26_10_2190,"L1 pch<0! count %d,is_search_ploted %d,i %x")
TRACE_MSG(GSMFRAMEGEN_2450_112_2_17_22_26_10_2191,"L1:startfn is coming,not allowed to plot other frame")
TRACE_MSG(GSMFRAMEGEN_2815_112_2_17_22_26_11_2192,"L1: search handover cell 0x%x")
TRACE_MSG(LAYER1_AUDIO_192_112_2_17_22_26_13_2193,"L1:s_silencecallback_pfunc!")
TRACE_MSG(LAYER1_AUDIO_197_112_2_17_22_26_13_2194,"L1: s_silencecallback_pfunc is PNULL!")
TRACE_MSG(LAYER1_AUDIO_575_112_2_17_22_26_14_2195,"LAYER1_SetAudioEnable %d %d %d %d")
TRACE_MSG(LAYER1_AUDIO_602_112_2_17_22_26_14_2196,"LAYER1_SetCodecEnable: is_enable = %d, dsp_state = %d")
TRACE_MSG(LAYER1_AUDIO_610_112_2_17_22_26_14_2197,"LAYER1_SetCodecEnable: dsp_state = %d")
TRACE_MSG(LAYER1_AUDIO_634_112_2_17_22_26_14_2198,"LAYER1_SetCodecEnable %d %d %d")
TRACE_MSG(LAYER1_AUDIO_701_112_2_17_22_26_14_2199,"LAYER1_SetAudioEnable %d %d %d %d")
TRACE_MSG(LAYER1_AUDIO_729_112_2_17_22_26_14_2200,"LAYER1_SetAudioEnable2 %d %d %d %d")
TRACE_MSG(LAYER1_AUDIO_731_112_2_17_22_26_14_2201,"LAYER1_SetAudioEnable: is_enable = %d, dsp_state = %d")
TRACE_MSG(LAYER1_AUDIO_739_112_2_17_22_26_14_2202,"LAYER1_SetAudioEnable: dsp_state = %d")
TRACE_MSG(LAYER1_AUDIO_763_112_2_17_22_26_14_2203,"LAYER1_SetAudioEnable %d %d %d")
TRACE_MSG(LAYER1_AUDIO_1046_112_2_17_22_26_15_2204," send loopback open msg to layer1")
TRACE_MSG(LAYER1_AUDIO_1130_112_2_17_22_26_15_2205,"L1: LSYER1_ReadIQData error")
TRACE_MSG(LAYER1_AUDIO_1146_112_2_17_22_26_15_2206,"LOGSAVE FAIL: start gsm dsp log record in td mode!")
TRACE_MSG(LAYER1_AUDIO_1320_112_2_17_22_26_15_2207,"L1: voice_format %d,dsp_upload_format= %x,is_from_mic=%x,is_from_net=%x,block_count %d,block_length %d iq switch 0x%x")
TRACE_MSG(LAYER1_AUDIO_1380_112_2_17_22_26_16_2208,"LOGSAVE FAIL: start gsm dsp log record in td mode!")
TRACE_MSG(LAYER1_AUDIO_1716_112_2_17_22_26_16_2209,"Layer1: eq_type= 0x%x")
TRACE_MSG(LAYER1_AUDIO_1985_112_2_17_22_26_17_2210,"L1:LAYER1_Mp3SynthFrame.State:MP3_SYNTH,Time=%d ms£¬report_data_type = %d ")
TRACE_MSG(LAYER1_AUDIO_2063_112_2_17_22_26_17_2211,"L1:overlay Mp3 synthesis code,overlay_address = 0x%x")
TRACE_MSG(LAYER1_AUDIO_2070_112_2_17_22_26_17_2212,"L1:overlay SBC code")
TRACE_MSG(LAYER1_AUDIO_2077_112_2_17_22_26_17_2213,"L1:overlay Audio code")
TRACE_MSG(LAYER1_AUDIO_2217_112_2_17_22_26_17_2214,"L1 mag: start 1 type %d")
TRACE_MSG(LAYER1_AUDIO_2221_112_2_17_22_26_17_2215,"L1 mag: start 2 type %d")
TRACE_MSG(LAYER1_AUDIO_2223_112_2_17_22_26_17_2216,"L1 mag: start 3 type %d")
TRACE_MSG(LAYER1_AUDIO_2227_112_2_17_22_26_17_2217,"L1 mag: start 4 type %d")
TRACE_MSG(LAYER1_AUDIO_2233_112_2_17_22_26_17_2218,"L1: start magic voice err, type is 0")
TRACE_MSG(LAYER1_AUDIO_2236_112_2_17_22_26_17_2219,"L1 mag: start type 5 %d")
TRACE_MSG(LAYER1_AUDIO_2267_112_2_17_22_26_17_2220,"LAYER1:LAYER1_StartRecord_Downlink register downlink callback function")
TRACE_MSG(LAYER1_AUDIO_2278_112_2_17_22_26_17_2221,"LAYER1:LAYER1_StopRecord_Downlink unregister downlink callback function")
TRACE_MSG(LAYER1_AUDIO_2289_112_2_17_22_26_17_2222,"LAYER1:LAYER1_StartRecord_Ad register uplink callback function")
TRACE_MSG(LAYER1_AUDIO_2304_112_2_17_22_26_17_2223,"LAYER1:LAYER1_StopRecord_Ad unregister uplink callback function")
TRACE_MSG(LAYER1_AUDIO_CODEC_84_112_2_17_22_26_18_2224,"LAYER1_AUDIO_CODEC_Data 0x%x")
TRACE_MSG(LAYER1_AUDIO_CODEC_173_112_2_17_22_26_18_2225,"LAYER1_AUDIO_CODEC_CopyData data count is 0")
TRACE_MSG(LAYER1_AUDIO_CODEC_214_112_2_17_22_26_18_2226,"LAYER1_AUDIO_CODEC_CopyLRData data count is 0")
TRACE_MSG(LAYER1_DIAG_299_112_2_17_22_26_19_2227,"bef fastcali isnv %d, band %d, type %d, index %d, length %d")
TRACE_MSG(LAYER1_DIAG_401_112_2_17_22_26_19_2228,"layer1_diag parameters is wrong!")
TRACE_MSG(LAYER1_DIAG_881_112_2_17_22_26_20_2229,"the NV_ID_IMEI is wrong!")
TRACE_MSG(LAYER1_DIAG_935_112_2_17_22_26_20_2230,"L1:NV RF solution hasn't been read out!")
TRACE_MSG(LAYER1_DIAG_1053_112_2_17_22_26_20_2231,"L1 this function is implemented only at 8800s (Layer1_HandleDspDebug())")
TRACE_MSG(LAYER1_DIAG_1080_112_2_17_22_26_20_2232,"fastcali diag ptr1 0x%x, pc_sig_ptr 0x%x, *pc_sig_ptr 0x%x")
TRACE_MSG(LAYER1_DIAG_1102_112_2_17_22_26_20_2233,"fastcali diag cmd 0x%x, count %d, p1 %d, p2 %d, p3 %d, p4 %d")
TRACE_MSG(LAYER1_FREQ_91_112_2_17_22_26_21_2234,"L1: IT's a WRONG frequency arfcn! 0x%x\n")
TRACE_MSG(LAYER1_MAC_PROC_211_112_2_17_22_26_22_2235,"L1:map gmsk mean=%x,cv=%x;psk8 mean=%x,cv=%x;present=%x")
TRACE_MSG(LAYER1_MAC_PROC_294_112_2_17_22_26_22_2236,"L1:dump the polling,out of dsp's ability,polling_slot=%d,first rx_slot=%d")
TRACE_MSG(LAYER1_MAC_PROC_424_112_2_17_22_26_22_2237,"L1 polling data is not write  %d, %d!")
TRACE_MSG(LAYER1_MAC_PROC_465_112_2_17_22_26_22_2238,"LAYER1: It's Layer1Mac_GetDataAndWriteData function\n")
TRACE_MSG(LAYER1_MAC_PROC_503_112_2_17_22_26_23_2239,"L1 polling data is not write to dsp %d, %d!")
TRACE_MSG(LAYER1_MAIN_2332_112_2_17_22_26_28_2240,"L1:save signal 0x%x\n")
TRACE_MSG(LAYER1_MAIN_2344_112_2_17_22_26_28_2241,"L1:discard 0x%x, sender 0x%x\n")
TRACE_MSG(LAYER1_MAIN_2412_112_2_17_22_26_28_2242,"L1: Initializing Physical Layer All Tasks\n\r")
TRACE_MSG(LAYER1_MAIN_2463_112_2_17_22_26_28_2243,"L1: Initializing for active\n\r")
TRACE_MSG(LAYER1_MAIN_2698_112_2_17_22_26_28_2244,"L1Main_calibration_timer")
TRACE_MSG(LAYER1_MAIN_2818_112_2_17_22_26_29_2245,"L1: calibration timer register failed")
TRACE_MSG(LAYER1_MAIN_2823_112_2_17_22_26_29_2246,"L1: calibration timer is not active, duration is equal 0")
TRACE_MSG(LAYER1_MAIN_2952_112_2_17_22_26_29_2247,"L1Main_HandleLongTermBcchMeasTimer")
TRACE_MSG(LAYER1_MAIN_3002_112_2_17_22_26_29_2248,"L1Main_HandleBcchMeasReportTimerReq")
TRACE_MSG(LAYER1_MAIN_3121_112_2_17_22_26_29_2249,"L1:CheckIfBCCHCollide,invalid arfcn,old=0x%x,new=0x%x")
TRACE_MSG(LAYER1_MAIN_3130_112_2_17_22_26_29_2250,"L1:CheckIfBCCHCollide: invalid cell_id,old=0x%x, new=0x%x")
TRACE_MSG(LAYER1_MAIN_3153_112_2_17_22_26_29_2251,"L1:CheckIfBCCHCollide,fn_old_mod51=%d,fn_new_mod51=%d")
TRACE_MSG(LAYER1_MAIN_3171_112_2_17_22_26_29_2252,"L1 set tbf flag!")
TRACE_MSG(LAYER1_MAIN_3439_112_2_17_22_26_30_2253,"L1 dsp ready event not arrived!")
TRACE_MSG(LAYER1_MAIN_3490_112_2_17_22_26_30_2254,"L1:L1Main_CreateToneDelayTimer ")
TRACE_MSG(LAYER1_MAIN_3544_112_2_17_22_26_30_2255,"L1 sim event not arrived!")
TRACE_MSG(LAYER1_OUTPORT_191_112_2_17_22_26_31_2256,"L1:Start layer1_calibration thread!\n")
TRACE_MSG(LAYER1_OUTPORT_222_112_2_17_22_26_31_2257,"L1:Start layer1_normal thread!\n")
TRACE_MSG(LAYER1_OUTPORT_287_112_2_17_22_26_31_2258,"L1:The last operation is not finished!")
TRACE_MSG(LAYER1_OUTPORT_293_112_2_17_22_26_31_2259,"L1 set task id %x L1API_GetAFC")
TRACE_MSG(LAYER1_OUTPORT_340_112_2_17_22_26_31_2260,"L1:The last operation is not finished!")
TRACE_MSG(LAYER1_OUTPORT_346_112_2_17_22_26_31_2261,"L1 set task id %x L1API_GetPA")
TRACE_MSG(LAYER1_OUTPORT_380_112_2_17_22_26_32_2262,"L1:The last operation is not finished!")
TRACE_MSG(LAYER1_OUTPORT_386_112_2_17_22_26_32_2263,"L1 set task id %x L1API_GetAGC")
TRACE_MSG(LAYER1_OUTPORT_417_112_2_17_22_26_32_2264,"L1:The last operation is not finished!")
TRACE_MSG(LAYER1_OUTPORT_422_112_2_17_22_26_32_2265,"L1 set task id %x L1API_GetBlockErrorRatio")
TRACE_MSG(LAYER1_OUTPORT_529_112_2_17_22_26_32_2266,"Start DSP RX with max power")
TRACE_MSG(LAYER1_OUTPORT_542_112_2_17_22_26_32_2267,"L1:Stop DSP RX with max power")
TRACE_MSG(LAYER1_OUTPORT_617_112_2_17_22_26_32_2268,"L1:Start DSP TX with max power")
TRACE_MSG(LAYER1_OUTPORT_630_112_2_17_22_26_32_2269,"L1:Stop DSP TX with max power")
TRACE_MSG(LAYER1_OUTPORT_716_112_2_17_22_26_32_2270,"Start DSP TX with max power")
TRACE_MSG(LAYER1_OUTPORT_729_112_2_17_22_26_32_2271,"Stop DSP TX with max power")
TRACE_MSG(LAYER1_OUTPORT_821_112_2_17_22_26_33_2272,"L1:GPSVALIDATION audio is using!")
TRACE_MSG(LAYER1_OUTPORT_828_112_2_17_22_26_33_2273,"L1:Other function code in overlay! ")
TRACE_MSG(LAYER1_OUTPORT_957_112_2_17_22_26_33_2274,"L1PagingTimer_RegisterCallback func 0x%x , num %d")
TRACE_MSG(LAYER1_OUTPORT_986_112_2_17_22_26_33_2275,"L1PagingTimer_UnRegisterCallback func 0x%x ")
TRACE_MSG(LAYER1_OUTPORT_1013_112_2_17_22_26_33_2276,"L1PagingTimer_DoCallback num %d, tickcount %d ")
TRACE_MSG(LAYER1_OUTPORT_1052_112_2_17_22_26_33_2277,"L1PagingTimer_CountPlus, delta %d, last %d, pattern %d, next %d, curtick")
TRACE_MSG(LAYER1_OUTPORT_1133_112_2_17_22_26_33_2278,"L1: MPHHandle_MPhSMSCBDiscardBlock wakeup dsp")
TRACE_MSG(LAYER1_OUTPORT_1156_112_2_17_22_26_33_2279,"L1 cali mode %d,is_debug_on %d,arfcn 0x%x,band %d")
TRACE_MSG(LAYER1_OUTPORT_1184_112_2_17_22_26_33_2280,"L1 ")
TRACE_MSG(LAYER1_OUTPORT_1201_112_2_17_22_26_33_2281,"L1: MPHHandle_MPhSMSCBDiscardBlock wakeup dsp")
TRACE_MSG(LAYER1_SBC_113_112_2_17_22_26_34_2282,"L1:load GPS code in overlay.!")
TRACE_MSG(LAYER1_SBC_116_112_2_17_22_26_34_2283,"OVERLAY_TEST: Overlay_code_base_ptr is %x")
TRACE_MSG(LAYER1_SBC_117_112_2_17_22_26_34_2284,"OVERLAY_TEST: Overlay_Load_Code %x %x %x %x %x")
TRACE_MSG(LAYER1_SBC_193_112_2_17_22_26_34_2285,"OVERLAY: overlay_context.overlay_mutex %x")
TRACE_MSG(LAYER1_SBC_194_112_2_17_22_26_34_2286,"OVERLAY: overlay_context.mem_state %d")
TRACE_MSG(LAYER1_SBC_195_112_2_17_22_26_34_2287,"OVERLAY: overlay_context.overlay_code_base_ptr %x")
TRACE_MSG(LAYER1_SBC_227_112_2_17_22_26_34_2288,"SBC Function: L1_SBC_Config_Encoder ")
TRACE_MSG(LAYER1_SBC_241_112_2_17_22_26_34_2289,"SBC Function mutex: %x ")
TRACE_MSG(LAYER1_SBC_345_112_2_17_22_26_34_2290,"layer1_cmd: command  CmdQ_SBC_Config_Encoder ")
TRACE_MSG(LAYER1_SBC_371_112_2_17_22_26_34_2291,"SBC Function: L1_SBC_Encode_Frame %d %d %d")
TRACE_MSG(LAYER1_SBC_373_112_2_17_22_26_34_2292,"SBC Function: L1_SBC_Encode_Frame")
TRACE_MSG(LAYER1_SBC_552_112_2_17_22_26_35_2293,"layer1_cmd: command  CMDQ_TRANPCM_CONFIG ")
TRACE_MSG(LAYER1_SBC_568_112_2_17_22_26_35_2294,"layer1_cmd: command  CMDQ_TRANPCM_PROCESS ")
TRACE_MSG(LAYER1_VOICECALL_82_112_2_17_22_26_35_2295,"Transfer a frame data to DSP !!!! ")
TRACE_MSG(MPHHANDLE_303_112_2_17_22_26_36_2296,"l1 active meet response delay !!!")
TRACE_MSG(MPHHANDLE_345_112_2_17_22_26_36_2297,"L1 deactive mask %x")
TRACE_MSG(MPHHANDLE_378_112_2_17_22_26_36_2298,"L1 main card id %x")
TRACE_MSG(MPHHANDLE_844_112_2_17_22_26_37_2299,"L1 pch allowed nv %x net %x")
TRACE_MSG(MPHHANDLE_1278_112_2_17_22_26_38_2300,"L1 pwrswp state %d")
TRACE_MSG(MPHHANDLE_1319_112_2_17_22_26_38_2301,"L1:calipost_param = 0x%x")
TRACE_MSG(MPHHANDLE_1356_112_2_17_22_26_38_2302,"L1:band = %d,arfcn = 0x%x, cali mode = %x, cali param %x")
TRACE_MSG(MPHHANDLE_1497_112_2_17_22_26_39_2303,"L1 pch allowed nv %x net %x")
TRACE_MSG(MPHHANDLE_2071_112_2_17_22_26_40_2304,"L1 page mode req param3 %d")
TRACE_MSG(MPHHANDLE_2420_112_2_17_22_26_40_2305,"L1 pch allowed nv %x net %x")
TRACE_MSG(MPHHANDLE_3183_112_2_17_22_26_42_2306,"L1 pch allowed nv %x net %x")
TRACE_MSG(MPHHANDLE_3416_112_2_17_22_26_42_2307,"L1: start_fn %d")
TRACE_MSG(MPHHANDLE_3626_112_2_17_22_26_43_2308,"L1: chan assign start fn %d")
TRACE_MSG(MPHHANDLE_3820_112_2_17_22_26_43_2309,"L1:chan_mode %d, is_change %d")
TRACE_MSG(MPHHANDLE_3910_112_2_17_22_26_43_2310,"L1:Cipher mode %d,Cipher_algarithm %d,n")
TRACE_MSG(MPHHANDLE_3915_112_2_17_22_26_43_2311,"L1:Cipher Key %d,%d,%d,%d,%d,%d,%d,%dn")
TRACE_MSG(MPHHANDLE_3970_112_2_17_22_26_43_2312,"L1: Freq Change start fn %d")
TRACE_MSG(MPHHANDLE_4037_112_2_17_22_26_44_2313,"L1: chan mode %d!\n")
TRACE_MSG(MPHHANDLE_4152_112_2_17_22_26_44_2314,"L1 init handover cell fail! ")
TRACE_MSG(MPHHANDLE_4159_112_2_17_22_26_44_2315,"L1 handover cell id %d,  arfcn index %x ")
TRACE_MSG(MPHHANDLE_4198_112_2_17_22_26_44_2316,"L1 handover to resync pending cell arfcn index %x")
TRACE_MSG(MPHHANDLE_4218_112_2_17_22_26_44_2317,"L1:Handover fail, cell_id = %d,bsic %d,target bsic %d")
TRACE_MSG(MPHHANDLE_4275_112_2_17_22_26_44_2318,"L1:cipher_mode %d,cipher_algorithm %d")
TRACE_MSG(MPHHANDLE_4591_112_2_17_22_26_45_2319,"L1:set cell %d to NOT_SYN_CALL")
TRACE_MSG(MPHHANDLE_4791_112_2_17_22_26_45_2320,"L1:No TCH avtive,ignor LOOP cmd")
TRACE_MSG(MPHHANDLE_4825_112_2_17_22_26_45_2321,"L1:rec disable loop back %d!")
TRACE_MSG(MPHHANDLE_4844_112_2_17_22_26_45_2322,"L1:rec loop back cmd!")
TRACE_MSG(MPHHANDLE_5050_112_2_17_22_26_46_2323,"L1: MPHHandle_MphSMSCBStartReq  cbch_required_bitmap %d")
TRACE_MSG(MPHHANDLE_5172_112_2_17_22_26_46_2324,"L1: MPHHandle_MPhSMSCBDiscardBlock %d")
TRACE_MSG(MPHHANDLE_5189_112_2_17_22_26_46_2325,"L1: MPHHandle_MPhSMSCBDiscardBlock wakeup dsp")
TRACE_MSG(MPHHANDLE_5240_112_2_17_22_26_46_2326,"L1 uiCellIndex is INVALID_CELL! %x")
TRACE_MSG(MPHHANDLE_5261_112_2_17_22_26_46_2327,"L1 cell state is not sync %x, %x!")
TRACE_MSG(MPHHANDLE_5344_112_2_17_22_26_46_2328,"L1 scell ")
TRACE_MSG(MPHHANDLE_5355_112_2_17_22_26_46_2329,"L1 valid cell! 0x%x")
TRACE_MSG(MPHHANDLE_5369_112_2_17_22_26_46_2330,"L1 searching %x")
TRACE_MSG(PHHANDLE_369_112_2_17_22_26_47_2331,"L1 enable codec device")
TRACE_MSG(PHHANDLE_401_112_2_17_22_26_47_2332,"layer1_cmd: audio enable, is_pre %d is_enable %d")
TRACE_MSG(PHHANDLE_624_112_2_17_22_26_48_2333,"L1 test mode is not supportted now!")
TRACE_MSG(PHHANDLE_638_112_2_17_22_26_48_2334,"L1:PHHandle_AppPhMp3A2dpReq:Time = %d ms")
TRACE_MSG(PHHANDLE_794_112_2_17_22_26_48_2335,"L1 dl rlc mode %x")
TRACE_MSG(PHHANDLE_806_112_2_17_22_26_48_2336,"L1:Switch Band: ma_arfcn_index = 0x%x, pdch_band = %d!")
TRACE_MSG(PHHANDLE_933_112_2_17_22_26_48_2337,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_1055_112_2_17_22_26_48_2338,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_1180_112_2_17_22_26_49_2339,"L1 dl rlc mode %x")
TRACE_MSG(PHHANDLE_1190_112_2_17_22_26_49_2340,"L1:Switch Band1: ma_arfcn_index = 0x%x, pdch_band=%d!")
TRACE_MSG(PHHANDLE_1293_112_2_17_22_26_49_2341,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_1391_112_2_17_22_26_49_2342,"L1 dl rlc mode %x")
TRACE_MSG(PHHANDLE_1496_112_2_17_22_26_49_2343,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_1591_112_2_17_22_26_49_2344,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_1664_112_2_17_22_26_50_2345,"L1 dl rlc mode %x")
TRACE_MSG(PHHANDLE_1765_112_2_17_22_26_50_2346,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_1865_112_2_17_22_26_50_2347,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_1954_112_2_17_22_26_50_2348,"L1 dl rlc mode %x")
TRACE_MSG(PHHANDLE_1995_112_2_17_22_26_50_2349,"L1:Switch Band2,ma_arfcn_index = 0x%x, pdch_band = %d!")
TRACE_MSG(PHHANDLE_2098_112_2_17_22_26_51_2350,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_2306_112_2_17_22_26_51_2351,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_2532_112_2_17_22_26_51_2352,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_2688_112_2_17_22_26_52_2353,"split cyccle %d")
TRACE_MSG(PHHANDLE_2781_112_2_17_22_26_52_2354,"L1: use old ta %d")
TRACE_MSG(PHHANDLE_2826_112_2_17_22_26_52_2355,"L1: use old tai %d")
TRACE_MSG(PHHANDLE_3042_112_2_17_22_26_52_2356,"Switch Band5: ma_arfcn_index = 0x%x, pdch_band = %d!")
TRACE_MSG(PHHANDLE_3123_112_2_17_22_26_53_2357,"L1:set cell %d to NOT_SYN_CALL")
TRACE_MSG(PHHANDLE_3248_112_2_17_22_26_53_2358,"L1:set cell %d to NOT_SYN_CALL")
TRACE_MSG(PHHANDLE_3321_112_2_17_22_26_53_2359,"L1:set cell %d to NOT_SYN_CALL")
TRACE_MSG(PHHANDLE_3584_112_2_17_22_26_53_2360,"L1: open loop back function %x")
TRACE_MSG(PHHANDLE_3603_112_2_17_22_26_53_2361,"L1: close loop back function")
TRACE_MSG(PHHANDLE_3721_112_2_17_22_26_54_2362,"L1 mag: phhandle type %d")
TRACE_MSG(RESQ_234_112_2_17_22_26_54_2363,"L1:Rx a PBCCH in BCCH_CAMPED state")
TRACE_MSG(RESQ_245_112_2_17_22_26_54_2364,"L1:BCCH_CAMPED,C_t %d!\n")
TRACE_MSG(RESQ_327_112_2_17_22_26_55_2365,"L1:PBCCH_CAMPED state,Discard a block,Ch_tag %d!\n")
TRACE_MSG(RESQ_369_112_2_17_22_26_55_2366,"L1:Rx a PPCH in PLMN_CAMPED state")
TRACE_MSG(RESQ_383_112_2_17_22_26_55_2367,"L1: Rx a SCH in PLMN_CAMPED state")
TRACE_MSG(RESQ_406_112_2_17_22_26_55_2368,"L1:Rx a PBCCH in BCCH_CAMPED state")
TRACE_MSG(RESQ_417_112_2_17_22_26_55_2369,"L1:PLMN_CAMPED state,Discard a block,Ch_tag %d!\n")
TRACE_MSG(RESQ_447_112_2_17_22_26_55_2370,"L1:Rx a CBCH in PG_REORG_CAMPED state")
TRACE_MSG(RESQ_506_112_2_17_22_26_55_2371,"L1: Rx a SCH in PG_REORG state")
TRACE_MSG(RESQ_516_112_2_17_22_26_55_2372,"L1:PG_REORG CAMPED,Discard a block,Ch_tag %d!\n")
TRACE_MSG(RESQ_569_112_2_17_22_26_55_2373,"L1:Rx a PBCCH in RACHAGCH state")
TRACE_MSG(RESQ_576_112_2_17_22_26_55_2374,"L1:Rx a PPCH in RACHAGCH state")
TRACE_MSG(RESQ_586_112_2_17_22_26_55_2375,"L1:Rx a PAGCH in RACHAGCH state")
TRACE_MSG(RESQ_596_112_2_17_22_26_55_2376,"L1: Rx a SCH in RACHAGCH state")
TRACE_MSG(RESQ_651_112_2_17_22_26_55_2377,"L1:Rx a PPCH in RACHAGCH state")
TRACE_MSG(RESQ_668_112_2_17_22_26_55_2378,"L1: Rx a SCH in PRACHAGCH state")
TRACE_MSG(RESQ_674_112_2_17_22_26_55_2379,"L1:Discard a block,Ch_tag %d!\n")
TRACE_MSG(RESQ_754_112_2_17_22_26_55_2380,"L1:SDCCHTCH state, Discard a block,Ch_tag %d!\n")
TRACE_MSG(RESQ_795_112_2_17_22_26_56_2381,"L1:Rx a PTCCH in TBF state")
TRACE_MSG(RESQ_883_112_2_17_22_26_56_2382,"L1:Rx a BCCH in traffic")
TRACE_MSG(RESQ_1170_112_2_17_22_26_56_2383,"L1: FrameInstr Queue DSP Over Run")
TRACE_MSG(RESQ_1175_112_2_17_22_26_56_2384,"L1: Cmd Queue DSP Over Run")
TRACE_MSG(RESQ_1180_112_2_17_22_26_56_2385,"L1: TxData Queue DSP Over Run")
TRACE_MSG(RESQ_1185_112_2_17_22_26_56_2386,"L1: Resp Queue DSP Over RunU")
TRACE_MSG(RESQ_1190_112_2_17_22_26_56_2387,"L1: RxData Queue DSP Over Run")
TRACE_MSG(RESQ_1196_112_2_17_22_26_56_2388,"L1: PowerMon Queue DSP Over Run")
TRACE_MSG(RESQ_1201_112_2_17_22_26_56_2389,"L1: MeasReport Queue DSP Over Run")
TRACE_MSG(RESQ_1206_112_2_17_22_26_56_2390,"L1: BlockSend Queue DSP Over Run")
TRACE_MSG(RESQ_1272_112_2_17_22_26_56_2391,"L1 bsic fail:  %x, is_sch_conflict %d,is_freqoffset_exist %d")
TRACE_MSG(RESQ_1425_112_2_17_22_26_57_2392,"L1 bsic fail:  %x, fail count %d,is_sch_conflict %d,is_freqoffset_exist %d")
TRACE_MSG(RESQ_1534_112_2_17_22_26_57_2393,"L1 handover cell search fail !%x")
TRACE_MSG(RESQ_1614_112_2_17_22_26_57_2394,"L1:WakeUp! %d,%d")
TRACE_MSG(RESQ_1641_112_2_17_22_26_57_2395,"L1:WakeUp! %d,%d")
TRACE_MSG(RESQ_1666_112_2_17_22_26_57_2396,"L1:WakeUp! %d,%d")
TRACE_MSG(RESQ_1713_112_2_17_22_26_57_2397,"L1 wakeup resp2")
TRACE_MSG(RESQ_1746_112_2_17_22_26_57_2398,"L1:WakeUp in idle! %d,%d")
TRACE_MSG(RESQ_1769_112_2_17_22_26_57_2399,"L1:WakeUp in power sweep! %d,%d")
TRACE_MSG(RESQ_1922_112_2_17_22_26_58_2400,"L1:Q start fn %d ,fn_curr %d,elapse %d\n")
TRACE_MSG(RESQ_2017_112_2_17_22_26_58_2401,"L1:Q start fn %d ,fn_curr %d,elapse %d\n")
TRACE_MSG(RESQ_2166_112_2_17_22_26_58_2402,"L1:WakeUp pbcch! %d,%d")
TRACE_MSG(RESQ_2267_112_2_17_22_26_58_2403,"L1:Q start fn %d ,fn_curr %d,elapse %d\n")
TRACE_MSG(RESQ_2426_112_2_17_22_26_59_2404,"L1:Q start fn %d ,fn_curr %d,elapse %d\n")
TRACE_MSG(RESQ_2516_112_2_17_22_26_59_2405,"L1:WakeUp! %d,%d")
TRACE_MSG(RESQ_2592_112_2_17_22_26_59_2406,"L1:Q start fn %d ,fn_curr %d,elapse %d\n")
TRACE_MSG(RESQ_2728_112_2_17_22_26_59_2407,"L1 scell adjust %x, %x")
TRACE_MSG(RESQ_2734_112_2_17_22_26_59_2408,"L1 ncell adjust %x, %x; ncell %x, scell %x")
TRACE_MSG(RESQ_2858_112_2_17_22_27_0_2409,"L1: echo_testing_CNF!")
TRACE_MSG(RESQ_3276_112_2_17_22_27_0_2410,"L1:State:SBC_CONFIG,Time = %d ms")
TRACE_MSG(RESQ_3287_112_2_17_22_27_0_2411,"L1:State:SBC_ENCODE,Time = %d ms")
TRACE_MSG(RESQ_3299_112_2_17_22_27_0_2412,"L1:State:PCM_TRANS,Time = %d ms")
TRACE_MSG(RESQ_3346_112_2_17_22_27_1_2413,"L1:State:NONE_IN_USE,left_ptr=0x%x,right_ptr=0x%x,length = %d,Time = %d ms")
TRACE_MSG(RESQ_3417_112_2_17_22_27_1_2414,"L1:Get SBC Frame.State:NONE_IN_USE,frame_ptr=0x%x,frame_length=0x%x,frame_num=%d, Time = %d ms")
TRACE_MSG(RXLEVMON_440_112_2_17_22_27_2_2415,"L1 ba count %d %d, %d,total %d,%d")
TRACE_MSG(RXLEVMON_513_112_2_17_22_27_2_2416,"L1 main card id , card id num %d")
TRACE_MSG(RXLEVMON_624_112_2_17_22_27_2_2417,"L1 ba count de %d %d, %d,total %d,%d")
TRACE_MSG(RXLEVMON_886_112_2_17_22_27_3_2418,"L1 not permit %x")
TRACE_MSG(RXLEVMON_1101_112_2_17_22_27_3_2419,"L1:pwr_swp saturate with low gain, arfcn_index %d")
TRACE_MSG(RXLEVMON_1147_112_2_17_22_27_3_2420,"L1: meas_ptr->is_valid is wrong!")
TRACE_MSG(RXLEVMON_1208_112_2_17_22_27_3_2421,"L1:sat_low arfcn %x, , num %d")
TRACE_MSG(RXLEVMON_1263_112_2_17_22_27_3_2422,"L1:Monitor saturate with low gain, arfcn_index %d")
TRACE_MSG(RXLEVMON_1276_112_2_17_22_27_3_2423,"L1:sat_hi arfcn %x, num %d")
TRACE_MSG(RXLEVMON_1293_112_2_17_22_27_3_2424,"L1: meas_ptr->is_valid is wrong! \n")
TRACE_MSG(RXLEVMON_1471_112_2_17_22_27_4_2425,"L1 ba not valid")
TRACE_MSG(RXLEVMON_1497_112_2_17_22_27_4_2426,"L1 warning: can't get valid ba")
TRACE_MSG(RXLEVMON_1734_112_2_17_22_27_4_2427,"L1: IT's A WRONG g_measurement_ba_ptr->ba_table_state.band_ind!")
TRACE_MSG(RXLEVMON_1766_112_2_17_22_27_4_2428,"full band swp: arfcn_index(%x) rxlev %x times ")
TRACE_MSG(RXLEVMON_1812_112_2_17_22_27_5_2429,"full band swp: arfcn_index(%x) rxlev %x times ")
TRACE_MSG(RXLEVMON_1819_112_2_17_22_27_5_2430,"L1:now %d,n1 %d,n2 %d")
TRACE_MSG(RXLEVMON_1968_112_2_17_22_27_5_2431,"L1:rla %d,sum %d,count %d")
TRACE_MSG(RXLEVMON_2039_112_2_17_22_27_5_2432,"L1:t %d  %x,%x,%x,%x,%x,%x;%x,%x,%x,%x,%x,%x")
TRACE_MSG(RXLEVMON_2083_112_2_17_22_27_5_2433,"L1:idle  %d, %d, %d;%x,%x,%x,%x,%x,%x;%x,%x,%x,%x,%x,%x;%x,%x,%x,%x,%x,%x")
TRACE_MSG(RXLEVMON_2153_112_2_17_22_27_5_2434,"Layer1 %x turn strong")
TRACE_MSG(RXLEVMON_2590_112_2_17_22_27_6_2435,"powersweep duration %d")
TRACE_MSG(RXLEVMON_2735_112_2_17_22_27_6_2436,"layer1: %x search fail too much, needn't sync")
TRACE_MSG(RXLEVMON_2749_112_2_17_22_27_6_2437,"L1: This arfcn did not in current BA , arfcn_index %d \n")
TRACE_MSG(RXLEVMON_2794_112_2_17_22_27_6_2438,"L1 the arfcn index is not in ba %x")
TRACE_MSG(RXLEVMON_2799_112_2_17_22_27_6_2439,"L1 invalid arfcn index %x")
TRACE_MSG(RXLEVMON_2921_112_2_17_22_27_7_2440,"L1 ba table is invalid!")
TRACE_MSG(RXLEVMON_3168_112_2_17_22_27_7_2441,"L1 card_relation erro! arfcn_index %x, type %d")
TRACE_MSG(RXLEVMON_3173_112_2_17_22_27_7_2442,"L1 the cell is not in ba! arfcn_index %x")
TRACE_MSG(RXLEVMON_3288_112_2_17_22_27_7_2443,"L1 the arfcn is in old ba arfcn_index %x, type %d")
TRACE_MSG(RXLEVMON_3293_112_2_17_22_27_7_2444,"L1 the arfcn is in old ba! arfcn_index %x")
TRACE_MSG(RXLEVMON_3324_112_2_17_22_27_7_2445,"L1 the arfcn is in old ba arfcn_index %x, type %d")
TRACE_MSG(RXLEVMON_3329_112_2_17_22_27_7_2446,"L1 the arfcn is in old ba! arfcn_index %x")
TRACE_MSG(RXLEVMON_3631_112_2_17_22_27_8_2447,"L1:forgetting factor=%d")
TRACE_MSG(RXLEVMON_3822_112_2_17_22_27_9_2448,"L1:input %x,%x ; output  %x, %x ; %x, %x;e %x,r %x,slot %x,rx_tfi %x,%x,slotmask%x,loopback %d")
TRACE_MSG(RXLEVMON_3977_112_2_17_22_27_9_2449,"L1 RxLevMon_GetTestArfcnMoniParam rxlev_result %d, rxdbm %d, count %d")
TRACE_MSG(RXLEVMON_4042_112_2_17_22_27_9_2450,"L1: s_save_prev_ba_table is full!")
TRACE_MSG(RXLEVMON_4143_112_2_17_22_27_9_2451,"L1: RRA_MPH_RF_SCAN_IND!")
TRACE_MSG(RXLEVMON_4215_112_2_17_22_27_9_2452,"L1 quick pwrswp normal finished!")
TRACE_MSG(RXLEVMON_4221_112_2_17_22_27_9_2453,"L1 quick pwrswp abort no valid ba!")
TRACE_MSG(RXLEVMON_4226_112_2_17_22_27_9_2454,"L1 pwr ++ index %x, %x")
TRACE_MSG(RXLEVMON_4350_112_2_17_22_27_10_2455,"L1 add a scell card id %d!%x")
TRACE_MSG(RXLEVMON_4376_112_2_17_22_27_10_2456,"L1 invalid scell ba_index!%x arfcn_index %x")
TRACE_MSG(RXLEVMON_4493_112_2_17_22_27_10_2457,"L1 cell is omitted %x")
TRACE_MSG(SHAREDMEM_PROC_308_112_2_17_22_27_11_2458,"L1:something wrong when to read status words!!")
TRACE_MSG(SHAREDMEM_PROC_338_112_2_17_22_27_11_2459,"something is wrong when to write status words 5 times!!")
TRACE_MSG(SHAREDMEM_PROC_396_112_2_17_22_27_11_2460,"LAYER1: The powermoni queue is empty! \n")
TRACE_MSG(SHAREDMEM_PROC_500_112_2_17_22_27_11_2461,"L1: New ReducedFn 0x%x")
TRACE_MSG(SHAREDMEM_PROC_822_112_2_17_22_27_12_2462,"L1: send chanchange sig")
TRACE_MSG(SHAREDMEM_PROC_983_112_2_17_22_27_12_2463,"L1: The rx queue is empty!")
TRACE_MSG(SHAREDMEM_PROC_1303_112_2_17_22_27_13_2464,"SetTxFramePcl, base address 0x%x, writein 0x%x")
TRACE_MSG(SHAREDMEM_PROC_1469_112_2_17_22_27_13_2465,"L1 cancel frame after wake up!, %x")
TRACE_MSG(SHAREDMEM_PROC_1711_112_2_17_22_27_14_2466,"L1:Wake up DSP, and sleep frame is at out + 1!!")
TRACE_MSG(SHAREDMEM_PROC_1874_112_2_17_22_27_14_2467,"L1:still Waiting for wakeup's response!")
TRACE_MSG(SHAREDMEM_PROC_1929_112_2_17_22_27_14_2468,"L1 cancel search :%x")
TRACE_MSG(SHAREDMEM_PROC_1967_112_2_17_22_27_14_2469,"SharedMemProc_CancelObsoleteFrames: total = %d!")
TRACE_MSG(SHAREDMEM_PROC_2042_112_2_17_22_27_14_2470,"L1 find ask more flag more than once!")
TRACE_MSG(SHAREDMEM_PROC_2197_112_2_17_22_27_15_2471,"L1 cancel margin %d, iscancel %d, newin %d, oldin %d, out %d, askmore %d, newaskmore %d")
TRACE_MSG(SHAREDMEM_PROC_2236_112_2_17_22_27_15_2472,"L1:distance between in and out is 0")
TRACE_MSG(SHAREDMEM_PROC_2330_112_2_17_22_27_15_2473,"SharedMemProc_CancelRemainingFrames:channel_mode_update")
TRACE_MSG(SHAREDMEM_PROC_2337_112_2_17_22_27_15_2474,"SharedMemProc_CancelRemainingFrames:TA update")
TRACE_MSG(SHAREDMEM_PROC_2344_112_2_17_22_27_15_2475,"SharedMemProc_CancelRemainingFrames:power update")
TRACE_MSG(SHAREDMEM_PROC_2352_112_2_17_22_27_15_2476,"SharedMemProc_CancelRemainingFrames:channel change")
TRACE_MSG(SHAREDMEM_PROC_2363_112_2_17_22_27_15_2477,"SharedMemProc_CancelRemainingFrames:gprs ta update")
TRACE_MSG(SHAREDMEM_PROC_2371_112_2_17_22_27_15_2478,"SharedMemProc_CancelRemainingFrames:gprs usf update")
TRACE_MSG(SHAREDMEM_PROC_2377_112_2_17_22_27_15_2479,"SharedMemProc_CancelRemainingFrames:gprs channel update")
TRACE_MSG(SHAREDMEM_PROC_2480_112_2_17_22_27_15_2480,"L1 error: UpdateRxDataOutPtr rx buffer empty, rxdata in %d, rxdata out %d")
TRACE_MSG(SHAREDMEM_PROC_3830_112_2_17_22_27_18_2481,"L1:divide slp.slp cnt 0x%x,sec slp 0x%x,out %x,crr t:0x%x,nea t:0x%x")
TRACE_MSG(SHAREDMEM_PROC_4080_112_2_17_22_27_19_2482,"L1:cancel rx_tx_rxlev -> rx_tx")
TRACE_MSG(SHAREDMEM_PROC_4096_112_2_17_22_27_19_2483,"L1:cancel rx_rxlev -> rx")
TRACE_MSG(SHAREDMEM_PROC_4105_112_2_17_22_27_19_2484,"L1:cancel tch_rx_tx_rxlev -> tch_rx_tx")
TRACE_MSG(SHAREDMEM_PROC_4114_112_2_17_22_27_19_2485,"L1:cancel rxlev_tx_rxlev -> rxlew_rx")
TRACE_MSG(SHAREDMEM_PROC_4134_112_2_17_22_27_19_2486,"L1:cancel rxlev rxlev_rxlev -> nop")
TRACE_MSG(SHAREDMEM_PROC_4605_112_2_17_22_27_20_2487,"download param %ld, %ld")
TRACE_MSG(SHAREDMEM_PROC_4947_112_2_17_22_27_21_2488,"L1:test_count = %d")
TRACE_MSG(SHAREDMEM_PROC_5186_112_2_17_22_27_21_2489,"L1 relation:%x,%x,%x,%x,%x,%x,%x,%x")
TRACE_MSG(SHAREDMEM_PROC_5284_112_2_17_22_27_21_2490,"LAYER1:SharedMemProc_InitializeNxp")
TRACE_MSG(SHAREDMEM_PROC_5299_112_2_17_22_27_21_2491,"L1:SharedMemProc_SetDAPCMDate")
TRACE_MSG(SHAREDMEM_PROC_5312_112_2_17_22_27_21_2492,"L1:SharedMemProc_SetNetPCMDate")
TRACE_MSG(SYSTEMINFO_886_112_2_17_22_27_24_2493,"L1: freq_redefine changed!!")
TRACE_MSG(SYSTEMINFO_951_112_2_17_22_27_24_2494,"L1: freq_redefine changed!!")
TRACE_MSG(SYSTEMINFO_1185_112_2_17_22_27_24_2495,"curr_fn %d,delta_fn %d,change delay %d,start_fn %d")
TRACE_MSG(SYSTEMINFO_1374_112_2_17_22_27_25_2496," Current chan same as next chan")
TRACE_MSG(SYSTEMINFO_1379_112_2_17_22_27_25_2497," Current chan different from next chan")
TRACE_MSG(SYSTEMINFO_1436_112_2_17_22_27_25_2498,"L1:loop_back paras: p1= %d, p2= %d!\n")
TRACE_MSG(SYSTEMINFO_1936_112_2_17_22_27_26_2499,"L1:NOTE: NOT ANY response from DSP for TCH search!! fn %d")
TRACE_MSG(SYSTEMINFO_2141_112_2_17_22_27_26_2500,"L1: cbch schedule start tb %d, tb")
TRACE_MSG(SYSTEMINFO_2148_112_2_17_22_27_26_2501,"L1:card id: %d,first schedule s_fn 0x%x,block num %d")
TRACE_MSG(SYSTEMINFO_2213_112_2_17_22_27_26_2502,"L1: schedule blocki %d")
TRACE_MSG(SYSTEMINFO_2236_112_2_17_22_27_26_2503,"L1:current schedule out 0x%x, blocki %d")
TRACE_MSG(SYSTEMINFO_2241_112_2_17_22_27_26_2504,"L1:end schedule and start receive all block 0-4")
TRACE_MSG(SYSTEMINFO_2687_112_2_17_22_27_27_2505,"L1 is first for tch %d")
TRACE_MSG(SYSTEMINFO_2695_112_2_17_22_27_27_2506,"L1 set frame type: is_debug_on %d,type %d, arfcn %x,gain %x")
TRACE_MSG(SYSTEMINFO_2719_112_2_17_22_27_27_2507,"L1 frametype %d, is_debug_mode %d")
TRACE_MSG(TAPWR_124_112_2_17_22_27_28_2508,"SetTxFramePcl, pwr_frame_ind %d, pwr_buf_ind %d, pcl[%d, %d, %d, %d]")
TRACE_MSG(TAPWR_226_112_2_17_22_27_28_2509,"L1: TaPwr_GetTxPwrCapabilty not suitable band\n")
TRACE_MSG(TAPWR_258_112_2_17_22_27_28_2510,"L1: SetMaxOutPutPowerLevel not suitable band\n")
TRACE_MSG(LAYER1_CALL_MAC_PH_150_112_2_17_22_27_31_2511,"\n s_polling_num = %d")
TRACE_MSG(LAYER1_CALL_MAC_PH_159_112_2_17_22_27_31_2512,"polling data send_type = %d")
TRACE_MSG(LAYER1_CALL_MAC_PH_172_112_2_17_22_27_31_2513,"polling data send_type = %d")
TRACE_MSG(PROCESS_MSG_876_112_2_17_22_27_34_2514,"Script file is wrong!")
TRACE_MSG(PROCESS_MSG_1774_112_2_17_22_27_36_2515,"L1:this rxlev data line is invalid,please update!")
TRACE_MSG(PROCESS_MSG_1824_112_2_17_22_27_36_2516,"L1:this sch data line is invalid,please update!")
TRACE_MSG(TEST_HANDLE_MSG_423_112_2_17_22_27_38_2517,"sleep counter =%d\n")
TRACE_MSG(TEST_HANDLE_MSG_809_112_2_17_22_27_39_2518,"sleep counter =%d\n")
TRACE_MSG(TEST_HANDLE_MSG_1507_112_2_17_22_27_40_2519,"LAYER1: RFoperation is wrong!\n")
TRACE_MSG(TEST_HANDLE_MSG_1730_112_2_17_22_27_41_2520,"\n SARAH test_search_response arfcn %d,T1 %d,T2 %d,T3P %d,BSIC %d")
TRACE_MSG(TEST_HANDLE_MSG_1785_112_2_17_22_27_41_2521,"\n SARAH test_send_SCH_data_to_MCU T1 %d,T2 %d,T3P %d,BSIC %d")
TRACE_MSG(TEST_HANDLE_MSG_1977_112_2_17_22_27_41_2522,"wake up:sleep counter %d\n")
TRACE_MSG(TEST_RW_SHARED_MEM_266_112_2_17_22_27_43_2523,"L1:command is NULL!!!")
TRACE_MSG(TEST_RW_SHARED_MEM_526_112_2_17_22_27_43_2524,"Rx data from channel has been read over!")
TRACE_MSG(LAYER1_AUDIO_SIMULATOR_426_112_2_17_22_27_46_2525,"peter:LAYER1_PlayCallback no data....!!!!")
TRACE_MSG(SHAREDMEM_PROC_SIMULATOR_20_112_2_17_22_27_47_2526,"Add_One_CMD to share mem faile! queue full")
TRACE_MSG(SHAREDMEM_PROC_SIMULATOR_53_112_2_17_22_27_47_2527,"Add_One_CMD to share mem faile! queue full")
TRACE_MSG(MMI_OSBRIDGE_39_112_2_17_22_27_48_2528,"MMI : alloc signal error, signal szie = %d")
TRACE_MSG(DSP_OPERATE_1367_112_2_17_22_27_52_2529,"0x8B000038 value is %x")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_485_112_2_17_22_46_15_2530,"L1:met error in unzip process in download dsp")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_523_112_2_17_22_46_16_2531,"L1:dsp code address is %x")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_914_112_2_17_22_46_16_2532,"L1:dsp code has inited!")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_928_112_2_17_22_46_16_2533,"L1:Use dsp code in NV!!, chip type 0x%x")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_929_112_2_17_22_46_16_2534,"codesize = %d")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_938_112_2_17_22_46_16_2535,"L1:Use dsp code in NV!!, chip type 0x%x")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_939_112_2_17_22_46_16_2536,"codesize = %d")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_945_112_2_17_22_46_16_2537,"L1:Use dsp_pdata.c!!, chip type 0x%x")
TRACE_MSG(LAYER1_DSP_DOWNLOAD_1378_112_2_17_22_46_17_2538,"L1_DSP lzma_load error %d")
TRACE_MSG(LOGSAVE_BUFFER_73_112_2_17_22_46_18_2539,"logsave: arm buffer addr 0x%x")
TRACE_MSG(LOGSAVE_BUFFER_90_112_2_17_22_46_18_2540,"logsave: dsp buffer addr 0x%x")
TRACE_MSG(LOGSAVE_BUFFER_107_112_2_17_22_46_18_2541,"logsave: iq buffer addr 0x%x")
TRACE_MSG(LOGSAVE_BUFFER_505_112_2_17_22_46_19_2542,"LOGSAVE BUFF: LogSaveBuff_Assert_Info_Func no size to alloc ass str!")
TRACE_MSG(LOGSAVE_BUFFER_512_112_2_17_22_46_19_2543,"LOGSAVE BUFF: LogSaveFM_InitFile, assert info error, finish_time %d")
TRACE_MSG(LOGSAVE_BUFFER_580_112_2_17_22_46_19_2544,"LOGSAVE FM: LogSaveFM_InitFile, assert memory error, finish_time %d")
TRACE_MSG(LOGSAVE_BUFFER_584_112_2_17_22_46_19_2545,"LOGSAVE BUFF: start save mem time: %d")
TRACE_MSG(LOGSAVE_BUFFER_589_112_2_17_22_46_19_2546,"LOGSAVE BUFF: end sdram time: %d")
TRACE_MSG(LOGSAVE_BUFFER_631_112_2_17_22_46_19_2547,"LOGSAVE BUFF: end thread info time: %d")
TRACE_MSG(LOGSAVE_BUFFER_636_112_2_17_22_46_19_2548,"LOGSAVE BUFF: end shared mem time: %d")
TRACE_MSG(LOGSAVE_BUFFER_641_112_2_17_22_46_19_2549,"LOGSAVE BUFF: end internal mem time: %d")
TRACE_MSG(LOGSAVE_BUFFER_668_112_2_17_22_46_19_2550,"LOGSAVE BUFF: end registers time: %d")
TRACE_MSG(LOGSAVE_BUFFER_712_112_2_17_22_46_19_2551,"LOGSAVE BUFF: dsp is running")
TRACE_MSG(LOGSAVE_BUFFER_806_112_2_17_22_46_20_2552,"LOGSAVE BUFF: dump dsp mem, dump_count %d")
TRACE_MSG(LOGSAVE_BUFFER_828_112_2_17_22_46_20_2553,"LOGSAVE BUFF: dump dsp mem, dump_count %d")
TRACE_MSG(LOGSAVE_BUFFER_841_112_2_17_22_46_20_2554,"LOGSAVE BUFF: end save mem time: %d, total length %d")
TRACE_MSG(LOGSAVE_BUFFER_900_112_2_17_22_46_20_2555,"IQ: LogSaveBuff_StartIQRecord, arfcn %d, block_count %d, block_length %d, para1 0x%x")
TRACE_MSG(LOGSAVE_CONFIG_166_112_2_17_22_46_20_2556,"LOGSAVE CF: nv iq switch wrong 0x%x")
TRACE_MSG(LOGSAVE_CONFIG_170_112_2_17_22_46_20_2557,"LOGSAVE CF: nv iq switch setting 0x%x")
TRACE_MSG(LOGSAVE_CONFIG_228_112_2_17_22_46_20_2558,"LOGSAVE CF: nv iq switch setting 0x%x")
TRACE_MSG(LOGSAVE_FILEMANAGE_148_112_2_17_22_46_21_2559,"LOGSAVE FM: g_lost_message_num arm log: %d, dsp log: %d, iq_data: %d, is_assert %d, start_time %d")
TRACE_MSG(LOGSAVE_FILEMANAGE_180_112_2_17_22_46_21_2560,"LOGSAVE FM: finish_time %d")
TRACE_MSG(LOGSAVE_MAIN_159_112_2_17_22_46_24_2561,"LOGSAVE: Close all")
TRACE_MSG(LOGSAVE_OUTPUT_247_112_2_17_22_46_25_2562,"LOGSAVE: assert not in thread")
TRACE_MSG(LOGSAVE_OUTPUT_253_112_2_17_22_46_25_2563,"LOGSAVE: is layer1 assert, stop dsp, cmd done ")
TRACE_MSG(LOGSAVE_OUTPUT_345_112_2_17_22_46_26_2564,"LOGSAVE: start")
TRACE_MSG(LOGSAVE_OUTPUT_366_112_2_17_22_46_26_2565,"LOGSAVE: stop")
TRACE_MSG(CC_ENTITY_570_112_2_17_22_46_27_2566,"CC: send start dtmf at %d state")
TRACE_MSG(CC_ENTITY_614_112_2_17_22_46_27_2567,"CC: send stop dtmf at %d state")
TRACE_MSG(CC_MSG_1487_112_2_17_22_46_35_2568,"CC: The message is valid CC message. \n")
TRACE_MSG(CC_MSG_1491_112_2_17_22_46_35_2569,"CC: The message is invalid CC message. \n")
TRACE_MSG(CC_MSG_1766_112_2_17_22_46_36_2570,"CC: the IE is not valid IE of Setup message. \n")
TRACE_MSG(CC_MSG_1773_112_2_17_22_46_36_2571,"CC: Decode Setup message and find serious ie error.\n")
TRACE_MSG(CC_MSG_2031_112_2_17_22_46_36_2572,"CC: the IE is not valid IE of Setup message. \n")
TRACE_MSG(CC_MSG_2063_112_2_17_22_46_36_2573,"CC: Progress message is too short to contain mandatory IE.\n")
TRACE_MSG(CC_MSG_2083_112_2_17_22_46_36_2574,"CC: Progress message mandatory IE error.\n")
TRACE_MSG(CC_MSG_2096_112_2_17_22_46_36_2575,"CC: Progress Message Ie sequence error.\n")
TRACE_MSG(CC_MSG_2135_112_2_17_22_46_36_2576,"CC: Facility message is too short to contain mandatory IE.\n")
TRACE_MSG(CC_MSG_2169_112_2_17_22_46_37_2577,"CC: Notify message is too short to contain mandatory IE.\n")
TRACE_MSG(CC_MSG_2302_112_2_17_22_46_37_2578,"CC: Error IE in Call Proceeding Message. \n")
TRACE_MSG(CC_MSG_2384_112_2_17_22_46_37_2579,"CC: Error IE in Alerting Message. \n")
TRACE_MSG(CC_MSG_2481_112_2_17_22_46_37_2580,"CC: Error IE in Connect Message. \n")
TRACE_MSG(CC_MSG_2590_112_2_17_22_46_37_2581,"CC: Error IE in Disconnect Message. \n")
TRACE_MSG(CC_MSG_2690_112_2_17_22_46_38_2582,"CC: Error IE in Modify Message. \n")
TRACE_MSG(CC_MSG_2786_112_2_17_22_46_38_2583,"CC: Error IE in Modify Complete Message. \n")
TRACE_MSG(CC_MSG_2885_112_2_17_22_46_38_2584,"CC: Error IE in Modify Reject Message. \n")
TRACE_MSG(CC_MSG_2925_112_2_17_22_46_38_2585,"CC: Message is too short.\n")
TRACE_MSG(CC_MSG_2966_112_2_17_22_46_38_2586,"CC: Message is too short.\n")
TRACE_MSG(CC_MSG_3205_112_2_17_22_46_39_2587,"CC: Error IE in Release Message. \n")
TRACE_MSG(CC_MSG_3289_112_2_17_22_46_39_2588,"CC: Error IE in Release Complete Message. \n")
TRACE_MSG(CC_MSG_3436_112_2_17_22_46_39_2589,"CC: Cc-Establishment message is too short to contain mandatory IE.\n")
TRACE_MSG(CC_MSG_3472_112_2_17_22_46_39_2590,"CC: Recall message is too short to contain mandatory IE.\n")
TRACE_MSG(CC_MSG_4286_112_2_17_22_46_41_2591,"CC: bc ie is type error")
TRACE_MSG(CC_MSG_4328_112_2_17_22_46_41_2592,"CC: CheckIE_Type4 iei = %d iei_type = %d")
TRACE_MSG(CC_MSG_4410_112_2_17_22_46_41_2593,"CC: progress ie is type error")
TRACE_MSG(CC_MSG_4813_112_2_17_22_46_42_2594,"CC: progress ie's type error")
TRACE_MSG(CC_MSG_4865_112_2_17_22_46_42_2595,"CC: progress ie's type error")
TRACE_MSG(CC_MSG_4925_112_2_17_22_46_42_2596,"CC: backup bc ie is type error")
TRACE_MSG(CC_MSG_4968_112_2_17_22_46_42_2597,"CC: allowed action ie is type error")
TRACE_MSG(CC_NETMSG_185_112_2_17_22_46_43_2598,"cc GetStatus same state = %d")
TRACE_MSG(CC_NETMSG_359_112_2_17_22_46_43_2599,"CC_START_DTMF_ACKNOWLEDGE state=%d")
TRACE_MSG(CC_NETMSG_404_112_2_17_22_46_43_2600,"CC_USER_INFOMATION state=%d")
TRACE_MSG(CC_NETMSG_449_112_2_17_22_46_44_2601,"CC_CONGESTION_CONTROL state=%d")
TRACE_MSG(CC_NETMSG_491_112_2_17_22_46_44_2602,"CC_FACILITY state=%d")
TRACE_MSG(CC_NETMSG_533_112_2_17_22_46_44_2603,"CC_NOTIFY state=%d")
TRACE_MSG(CC_NETMSG_575_112_2_17_22_46_44_2604,"CC_STATUS state=%d")
TRACE_MSG(CC_NETMSG_640_112_2_17_22_46_44_2605,"CC_START_DTMF_ACKNOWLEDGE state=%d")
TRACE_MSG(CC_NETMSG_698_112_2_17_22_46_44_2606,"CC_NOTIFY state=%d")
TRACE_MSG(CC_NETMSG_740_112_2_17_22_46_44_2607,"CC_RELEASE_COMPLETE state=%d")
TRACE_MSG(CC_NETMSG_885_112_2_17_22_46_44_2608,"CC_STATUS state=%d")
TRACE_MSG(CC_NETMSG_930_112_2_17_22_46_44_2609,"CC_STATUS state=%d")
TRACE_MSG(CC_NETMSG_1530_112_2_17_22_46_46_2610,"CC_RELEASE_COMPLETE state=%d")
TRACE_MSG(CC_NETMSG_1585_112_2_17_22_46_46_2611,"CC_STATUS state=%d")
TRACE_MSG(CC_TIMER_81_112_2_17_22_46_53_2612,"CC_SetTimer timer_type = %d")
TRACE_MSG(CC_TIMER_110_112_2_17_22_46_53_2613,"CC_KillTimer timer_type = %d")
TRACE_MSG(CCMSG_1867_112_2_17_22_46_57_2614,"CC: The message is valid CC message. \n")
TRACE_MSG(CCMSG_1871_112_2_17_22_46_57_2615,"CC: The message is invalid CC message. \n")
TRACE_MSG(CCMSG_2222_112_2_17_22_46_58_2616,"CC: the IE is not valid IE of Setup message. \n")
TRACE_MSG(CCMSG_2229_112_2_17_22_46_58_2617,"CC: Decode Setup message and find serious ie error.\n")
TRACE_MSG(CCMSG_2558_112_2_17_22_46_59_2618,"CC: the IE is not valid IE of Setup message. \n")
TRACE_MSG(CCMSG_2604_112_2_17_22_46_59_2619,"CC: Progress message is too short to contain mandatory IE.\n")
TRACE_MSG(CCMSG_2624_112_2_17_22_46_59_2620,"CC: Progress message mandatory IE error.\n")
TRACE_MSG(CCMSG_2637_112_2_17_22_46_59_2621,"CC: Progress Message Ie sequence error.\n")
TRACE_MSG(CCMSG_2690_112_2_17_22_46_59_2622,"CC: Facility message is too short to contain mandatory IE.\n")
TRACE_MSG(CCMSG_2736_112_2_17_22_46_59_2623,"CC: Notify message is too short to contain mandatory IE.\n")
TRACE_MSG(CCMSG_2900_112_2_17_22_46_59_2624,"CC: Error IE in Call Proceeding Message. \n")
TRACE_MSG(CCMSG_3000_112_2_17_22_47_0_2625,"CC: Error IE in Alerting Message. \n")
TRACE_MSG(CCMSG_3121_112_2_17_22_47_0_2626,"CC: Error IE in Connect Message. \n")
TRACE_MSG(CCMSG_3249_112_2_17_22_47_0_2627,"CC: Error IE in Disconnect Message. \n")
TRACE_MSG(CCMSG_3372_112_2_17_22_47_0_2628,"CC: Error IE in Modify Message. \n")
TRACE_MSG(CCMSG_3489_112_2_17_22_47_1_2629,"CC: Error IE in Modify Complete Message. \n")
TRACE_MSG(CCMSG_3609_112_2_17_22_47_1_2630,"CC: Error IE in Modify Reject Message. \n")
TRACE_MSG(CCMSG_3658_112_2_17_22_47_1_2631,"CC: Message is too short.\n")
TRACE_MSG(CCMSG_3699_112_2_17_22_47_1_2632,"CC: Message is too short.\n")
TRACE_MSG(CCMSG_4020_112_2_17_22_47_2_2633,"CC: Error IE in Release Message. \n")
TRACE_MSG(CCMSG_4122_112_2_17_22_47_2_2634,"CC: Error IE in Release Complete Message. \n")
TRACE_MSG(CCMSG_4308_112_2_17_22_47_2_2635,"CC: Cc-Establishment message is too short to contain mandatory IE.\n")
TRACE_MSG(CCMSG_4359_112_2_17_22_47_2_2636,"CC: Recall message is too short to contain mandatory IE.\n")
TRACE_MSG(CCMSG_5221_112_2_17_22_47_4_2637,"CC: bc ie is type error")
TRACE_MSG(CCMSG_5268_112_2_17_22_47_4_2638,"CC: facility ie is type error")
TRACE_MSG(CCMSG_5325_112_2_17_22_47_4_2639,"CC: progress ie is type error")
TRACE_MSG(CCMSG_5831_112_2_17_22_47_6_2640,"CC: progress ie's type error")
TRACE_MSG(CCMSG_5900_112_2_17_22_47_6_2641,"CC: progress ie's type error")
TRACE_MSG(CCMSG_5997_112_2_17_22_47_6_2642,"CC: backup bc ie is type error")
TRACE_MSG(CCMSG_6050_112_2_17_22_47_6_2643,"CC: allowed action ie is type error")
TRACE_MSG(L3FUNC_58_112_2_17_22_47_11_2644,"L3FUNC: Assert Failure, PLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_94_112_2_17_22_47_11_2645,"L3FUNC: Assert Failure, FPLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_187_112_2_17_22_47_11_2646,"L3FUNC: Assert Failure, LongFPLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_401_112_2_17_22_47_12_2647,"L3FUNC: Assert Failure, FLai list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_439_112_2_17_22_47_12_2648,"L3FUNC: Assert Failure, FPLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_472_112_2_17_22_47_12_2649,"L3FUNC: Assert Failure, LongFPLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_504_112_2_17_22_47_12_2650,"L3FUNC: Assert Failure, PLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_533_112_2_17_22_47_12_2651,"L3FUNC: Assert Failure, FLai list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_570_112_2_17_22_47_12_2652,"L3FUNC: Assert Failure, EPLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_615_112_2_17_22_47_12_2653,"L3FUNC: Assert Failure, EPLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_706_112_2_17_22_47_12_2654,"L3FUNC: Assert Failure, Select PLMN list length %d is more than MAX %d.")
TRACE_MSG(L3FUNC_735_112_2_17_22_47_12_2655,"L3FUNC: Assert Failure, Available PLMN list length %d is more than MAX %d.")
TRACE_MSG(MMGMM_COMMON_IE_133_112_2_17_22_47_13_2656,"MMGMM: Assert Failure, ie_type %d error, can't decode EC number list.")
TRACE_MSG(MMGMM_COMMON_IE_210_112_2_17_22_47_13_2657,"MMGMM: DecodeEmgNum data_len=%d, data_ptr[0]=%d")
TRACE_MSG(MMGMM_COMMON_IE_266_112_2_17_22_47_13_2658,"MMGMM:The length of the network name is too long")
TRACE_MSG(MMGMM_COMMON_IE_293_112_2_17_22_47_13_2659,"MMGMM:The length of the network name is too long")
TRACE_MSG(MMGMM_COMMON_IE_328_112_2_17_22_47_13_2660,"MMGMM: Assert Failure, date_len %d error, can't decode TimeZone.")
TRACE_MSG(MMGMM_COMMON_IE_353_112_2_17_22_47_13_2661,"MMGMM: Assert Failure, date_len %d error, can't decode TimeZone and Time.")
TRACE_MSG(MMGMM_COMMON_IE_458_112_2_17_22_47_13_2662,"MMGMM: decode MM/GMM information msg, MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MMGMM_COMMON_IE_468_112_2_17_22_47_13_2663,"MMGMM: decode MM/GMM information msg, DECODE_SUCCESS, No Optional IE")
TRACE_MSG(MMGMM_COMMON_IE_578_112_2_17_22_47_14_2664,"MMGMM: Assert Failure, ie_type %d error, can't decode MobileIdentify.")
TRACE_MSG(PS_MAIN_535_112_2_17_22_47_17_2665,"SDL_RUN\n")
TRACE_MSG(SCI_AIR_INTERFACE_232_112_2_17_22_47_20_2666,"SCIAIR:SCIAI_BSAIRMsgToPSMsg Receive unknown SignalCode")
TRACE_MSG(SCI_AIR_INTERFACE_307_112_2_17_22_47_20_2667,"SCIAIR:SCIAI_BSPSMsgToAIRMsg Receive unknown SignalCode")
TRACE_MSG(STATE_MACHINE_33_112_2_17_22_47_53_2668,"save signal %x\n")
TRACE_MSG(STATE_MACHINE_46_112_2_17_22_47_53_2669,"discard signal %x\n")
TRACE_MSG(DL_BUFFER_322_112_2_17_22_47_58_2670,"L2:unit(not SACCH) frame_ready\n")
TRACE_MSG(DL_BUFFER_731_112_2_17_22_47_59_2671,"L2:ack SEQ%d frame \n")
TRACE_MSG(DL_BUFFER_742_112_2_17_22_47_59_2672," NR error, ns=%d ,new=%d ,old=%d n")
TRACE_MSG(DL_BUFFER_753_112_2_17_22_47_59_2673,"L2:  frame_sending now\n")
TRACE_MSG(DL_BUFFER_760_112_2_17_22_47_59_2674,"L2:  frame_wait_for_resend now\n")
TRACE_MSG(DL_BUFFER_768_112_2_17_22_47_59_2675,"L2: receive ack but frame not send , so nr err \n")
TRACE_MSG(DL_BUFFER_799_112_2_17_22_47_59_2676,"L2:  frame acked \n")
TRACE_MSG(DL_BUFFER_806_112_2_17_22_47_59_2677,"L2:  all frame acked \n")
TRACE_MSG(DL_BUFFER_1146_112_2_17_22_48_0_2678,"L2:send_num= %d; recv_num= %d\n")
TRACE_MSG(DL_BUFFER_1158_112_2_17_22_48_0_2679,"L2:RR come\n")
TRACE_MSG(DL_BUFFER_1165_112_2_17_22_48_0_2680,"L2:REJ come\n")
TRACE_MSG(DL_BUFFER_1191_112_2_17_22_48_0_2681,"L2: UA frame come\n")
TRACE_MSG(DL_BUFFER_1200_112_2_17_22_48_0_2682,"L2:pf:%d\n")
TRACE_MSG(DL_BUFFER_1466_112_2_17_22_48_0_2683,"L2:package RR frame\n")
TRACE_MSG(DL_BUFFER_1473_112_2_17_22_48_0_2684,"L2:package REJ frame\n")
TRACE_MSG(DL_BUFFER_1502_112_2_17_22_48_0_2685,"L2:package I frame S:%d,R:%d\n")
TRACE_MSG(DL_BUFFER_1855_112_2_17_22_48_1_2686,"L2:package SABM \n")
TRACE_MSG(DL_BUFFER_1859_112_2_17_22_48_1_2687,"L2:package DISC \n")
TRACE_MSG(DL_BUFFER_1863_112_2_17_22_48_1_2688,"L2:package DM \n")
TRACE_MSG(DL_BUFFER_1867_112_2_17_22_48_1_2689,"L2:package UA \n")
TRACE_MSG(DL_BUFFER_2051_112_2_17_22_48_1_2690,"L2:unit(SACCH) frame_ready\n")
TRACE_MSG(DL_BUFFER_2074_112_2_17_22_48_1_2691,"contension data fail,length wrong\n")
TRACE_MSG(DL_BUFFER_2081_112_2_17_22_48_1_2692,"contension data fail,data wrong\n")
TRACE_MSG(DL_BUFFER_2162_112_2_17_22_48_2_2693,"DL: DL_GetUnsentSAPI3MsgHeader, MSG_PTR IS NOT NULL")
TRACE_MSG(DL_BUFFER_2168_112_2_17_22_48_2_2694,"DL: MSG header is %d,%d")
TRACE_MSG(DL_BUFFER_2170_112_2_17_22_48_2_2695,"DL:There is a message to be retrans!")
TRACE_MSG(DL_BUFFER_2202_112_2_17_22_48_2_2696,"DL: receive SABM in established state, but not init dlc according to special condition")
TRACE_MSG(GMM_CLASSMARK_477_112_2_17_22_48_3_2697,"GMM: classmark InitMsBand, ms_band=%d")
TRACE_MSG(GMM_ENCODE_DECODE_180_112_2_17_22_48_4_2698,"GMMCODE_Convert2GprsTimeValue input seconds = %d")
TRACE_MSG(GMM_ENCODE_DECODE_281_112_2_17_22_48_4_2699,"GMM:mobile_identity->type_identity =%d")
TRACE_MSG(GMM_ENCODE_DECODE_568_112_2_17_22_48_5_2700,"GMM:detach type %d is interpreted as re-attach not required")
TRACE_MSG(GMM_ENCODE_DECODE_598_112_2_17_22_48_5_2701,"GMM:message too short,len = %d!")
TRACE_MSG(GMM_ENCODE_DECODE_813_112_2_17_22_48_5_2702,"GMM:Attach Accept msg, unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_837_112_2_17_22_48_5_2703,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_869_112_2_17_22_48_5_2704,"GMM:Attach Reject, unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_893_112_2_17_22_48_5_2705,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_928_112_2_17_22_48_5_2706,"GMM:Detach Request,unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_952_112_2_17_22_48_5_2707,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_969_112_2_17_22_48_5_2708,"GMM:detach accept,unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_995_112_2_17_22_48_5_2709,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_1044_112_2_17_22_48_5_2710,"GMM:PTMSI reallocation,unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_1068_112_2_17_22_48_6_2711,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_1127_112_2_17_22_48_6_2712,"GMM:Authentication Msg error,rand-cksn should as both!")
TRACE_MSG(GMM_ENCODE_DECODE_1134_112_2_17_22_48_6_2713,"GMM:Authentication&Ciper msg, unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_1157_112_2_17_22_48_6_2714,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_1176_112_2_17_22_48_6_2715,"GMM:Identify Request,unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_1205_112_2_17_22_48_6_2716,"GMM error:npdu_list->number=%d")
TRACE_MSG(GMM_ENCODE_DECODE_1247_112_2_17_22_48_6_2717,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_1559_112_2_17_22_48_7_2718,"GMM:rau accept,unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_1612_112_2_17_22_48_7_2719,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_1647_112_2_17_22_48_7_2720,"GMM:RAU Reject msg, unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_1670_112_2_17_22_48_7_2721,"GMM:message too short,length = %d")
TRACE_MSG(GMM_ENCODE_DECODE_1685_112_2_17_22_48_7_2722,"GMM:GMM Status msg, unknown field!")
TRACE_MSG(GMM_ENCODE_DECODE_1780_112_2_17_22_48_7_2723,"GMM: rl_ind = %d")
TRACE_MSG(GMM_ENCODE_DECODE_1956_112_2_17_22_48_7_2724,"GMM:Assert Failure,invalid type_identity,")
TRACE_MSG(GMM_ENCODE_DECODE_2480_112_2_17_22_48_8_2725,"GMM: TLLI_FOREIGN ptmsi.valid_flag is %d")
TRACE_MSG(GMM_ENCODE_DECODE_2486_112_2_17_22_48_8_2726,"GMM: TLLI_LOCAL ptmsi.valid_flag is %d")
TRACE_MSG(GMM_ENCODE_DECODE_2504_112_2_17_22_48_9_2727,"GMM:TLLI error")
TRACE_MSG(GMM_ENCODE_DECODE_2515_112_2_17_22_48_9_2728,"GMM: Type = %d, new TLLI allocated = %x")
TRACE_MSG(GMM_MAIN_820_112_2_17_22_48_10_2729,"REGISTERED_INITIATED GMM_ATTACH_ACCEPT return_state = %d")
TRACE_MSG(GMM_MAIN_850_112_2_17_22_48_11_2730,"REGISTERED_INITIATED GMM_DETACH_REQUEST return_state = %d")
TRACE_MSG(GMM_MAIN_1237_112_2_17_22_48_11_2731,"GMM_REGISTERED_INITIATED LL_GMM_UNITDATA_IND msg_type = %d")
TRACE_MSG(GMM_MAIN_1307_112_2_17_22_48_11_2732,"GMM_REGISTERED LL_GMM_UNITDATA_IND msg_type = %d")
TRACE_MSG(GMM_MAIN_1375_112_2_17_22_48_12_2733,"GMM_RAU_INITIATED LL_GMM_UNITDATA_IND msg_type = %d")
TRACE_MSG(GMM_MAIN_1430_112_2_17_22_48_12_2734,"GMM_RAU_INITIATED LL_GMM_UNITDATA_IND msg_type = %d")
TRACE_MSG(GMM_MAIN_2327_112_2_17_22_48_14_2735,"ATTACH REQ sent, but ATTACH ACCEPT not received")
TRACE_MSG(GMM_MAIN_2334_112_2_17_22_48_14_2736,"send ATTACH REQ")
TRACE_MSG(GMM_MAIN_2531_112_2_17_22_48_14_2737,"send attach request")
TRACE_MSG(GMM_SUBPROC_190_112_2_17_22_48_20_2738,"%s")
TRACE_MSG(GMM_SUBPROC_2345_112_2_17_22_48_25_2739,"Gmm: rau attempt count > 5")
TRACE_MSG(GMM_SUBPROC_3925_112_2_17_22_48_28_2740,"GMM:info decode error")
TRACE_MSG(GMM_TIMER_77_112_2_17_22_48_28_2741,"GMM_SetTimer timer_type = %d")
TRACE_MSG(GMM_TIMER_103_112_2_17_22_48_29_2742,"GMM_KillTimer timer_type = %d")
TRACE_MSG(GMM_TIMER_145_112_2_17_22_48_29_2743,"GMM_IsTimerActive timer_type = %d")
TRACE_MSG(LLC_ANALYSE_FRAME_206_112_2_17_22_48_32_2744,"LLC:add oc_unack_downlink,512 for missing N(u)")
TRACE_MSG(LLC_ANALYSE_FRAME_217_112_2_17_22_48_32_2745,"LLC:retransmit encrypted llc frame with the N(S) belonging to previous 0~511 round")
TRACE_MSG(LLC_ANALYSE_FRAME_226_112_2_17_22_48_32_2746,"LLC:the received UI frame is enciphered but no cipher algorithm!")
TRACE_MSG(LLC_ANALYSE_FRAME_263_112_2_17_22_48_32_2747,"LLC:before_crc 512+oc_unack_downlink=%d")
TRACE_MSG(LLC_ANALYSE_FRAME_313_112_2_17_22_48_32_2748,"LLE: UI frame,invalid FCS! ")
TRACE_MSG(LLC_ANALYSE_FRAME_339_112_2_17_22_48_32_2749,"LLE: U frame,invalid FCS! ")
TRACE_MSG(LLC_ANALYSE_FRAME_359_112_2_17_22_48_32_2750,"LLE: S frame,invalid FCS! ")
TRACE_MSG(LLC_ANALYSE_FRAME_402_112_2_17_22_48_32_2751,"LLC: I frame need to be deciphered!")
TRACE_MSG(LLC_ANALYSE_FRAME_422_112_2_17_22_48_33_2752,"LLC: I frame length invalid,discard")
TRACE_MSG(LLC_ANALYSE_FRAME_451_112_2_17_22_48_33_2753,"LLC: I frame FCS invalid,discard")
TRACE_MSG(LLC_ANALYSE_FRAME_535_112_2_17_22_48_33_2754,"LLE: Received a duplicate frame")
TRACE_MSG(LLC_ANALYSE_FRAME_782_112_2_17_22_48_33_2755,"LLC: I frame N(S) invalid,discard")
TRACE_MSG(LLC_ANALYSE_FRAME_809_112_2_17_22_48_33_2756,"LLC:Assert Failure,DeleteSHeadNode empty 0!")
TRACE_MSG(LLC_ANALYSE_FRAME_946_112_2_17_22_48_34_2757,"LLC:Assert Failure,DeleteSHeadNode empty!")
TRACE_MSG(LLC_CIPHER_81_112_2_17_22_48_34_2758,"LLC:gmm need cipher misstate!")
TRACE_MSG(LLC_CIPHER_152_112_2_17_22_48_34_2759,"LLC:KC changed and reset IOV_UI/I")
TRACE_MSG(LLC_CIPHER_217_112_2_17_22_48_34_2760,"LLC:Cipher ASSERT !! gea_ciphering failed!")
TRACE_MSG(LLC_CIPHER_269_112_2_17_22_48_34_2761,"LLC:Cipher ASSERT !! gea_ciphering failed!")
TRACE_MSG(LLC_COMMON_200_112_2_17_22_48_35_2762,"LLC: in set frame before n(r) acked\n")
TRACE_MSG(LLC_COMMON_CODE_326_112_2_17_22_48_37_2763,"LLC:Assert Failure,wrong sapi value!")
TRACE_MSG(LLC_COMMON_CODE_427_112_2_17_22_48_37_2764,"LLC:Assert Failure,SetFRMRErrMsg wrong error type!")
TRACE_MSG(LLC_COMMON_CODE_843_112_2_17_22_48_38_2765,"LLC:Assert Failure,Polling next U node wrong value!")
TRACE_MSG(LLC_COMMON_CODE_891_112_2_17_22_48_38_2766,"LLC:Assert Failure,InvalidXID next U node wrong value!")
TRACE_MSG(LLC_COMMON_CODE_1038_112_2_17_22_48_38_2767,"LLC:KC changed and reset IOV_UI/I")
TRACE_MSG(LLC_COMMON_CODE_1315_112_2_17_22_48_39_2768,"LLC:Cipher ASSERT !! gea_ciphering failed!")
TRACE_MSG(LLC_COMMON_CODE_1378_112_2_17_22_48_39_2769,"LLC:Cipher ASSERT !! gea_ciphering failed!")
TRACE_MSG(LLC_COMMON_CODE_1428_112_2_17_22_48_39_2770,"LLC: Xid negotiation N201-U=%d")
TRACE_MSG(LLC_COMMON_CODE_1468_112_2_17_22_48_39_2771,"LLC:gmm need cipher misstate!")
TRACE_MSG(LLC_COMPOSE_FRAME_158_112_2_17_22_48_41_2772,"LLC:UPLINK XID PARAMETES-->T200:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_168_112_2_17_22_48_41_2773,"LLC:UPLINK XID PARAMETES-->N200:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_180_112_2_17_22_48_41_2774,"LLC:UPLINK XID PARAMETES-->N201 U:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_197_112_2_17_22_48_41_2775,"LLC:UPLINK XID PARAMETES-->N201 I:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_209_112_2_17_22_48_41_2776,"LLC:UPLINK XID PARAMETES-->mD:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_221_112_2_17_22_48_41_2777,"LLC:UPLINK XID PARAMETES-->mU:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_231_112_2_17_22_48_41_2778,"LLC:UPLINK XID PARAMETES-->kD:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_241_112_2_17_22_48_41_2779,"LLC:UPLINK XID PARAMETES-->kU:%d")
TRACE_MSG(LLC_COMPOSE_FRAME_275_112_2_17_22_48_41_2780,"LLC:UPLINK XID PARAMETES-->zero-length Layer 3 Parameters")
TRACE_MSG(LLC_COMPOSE_FRAME_282_112_2_17_22_48_41_2781,"LLC:UPLINK XID PARAMETES-->zero-length Layer 3 Parameters")
TRACE_MSG(LLC_COMPOSE_FRAME_656_112_2_17_22_48_42_2782,"LLC:Assert Failure,ComposeIFrame S empty!")
TRACE_MSG(LLC_COMPOSE_FRAME_1070_112_2_17_22_48_42_2783,"LLC: in set frame status\n")
TRACE_MSG(LLC_COMPOSE_FRAME_1111_112_2_17_22_48_42_2784,"LLC: in set frame before n(r) acked\n")
TRACE_MSG(LLC_COMPOSE_FRAME_1282_112_2_17_22_48_43_2785,"LLC:decode I frame SACK frame error!")
TRACE_MSG(LLC_COMPOSE_FRAME_1368_112_2_17_22_48_43_2786,"LLC: SACK error bitmap")
TRACE_MSG(LLC_COMPOSE_FRAME_1542_112_2_17_22_48_43_2787,"LLC:Assert Failure,SACK DeleteSNode S rnr invalid or empty")
TRACE_MSG(LLC_COMPOSE_FRAME_1558_112_2_17_22_48_43_2788,"LLC:Assert Failure,SACK DeleteSNode S invalid or empty")
TRACE_MSG(LLC_COMPOSE_FRAME_1670_112_2_17_22_48_44_2789,"LLC:Assert Failure,DeleteSNode S rnr invalid or empty")
TRACE_MSG(LLC_COMPOSE_FRAME_1686_112_2_17_22_48_44_2790,"LLC:Assert Failure,DeleteSNode S invalid or empty")
TRACE_MSG(LLC_COMPOSE_FRAME_1764_112_2_17_22_48_44_2791,"LLC:Assert Failure,DeleteUINode empty or invalid")
TRACE_MSG(LLC_COMPOSE_FRAME_1796_112_2_17_22_48_44_2792,"LLC:Assert Failure,ComposeNewFrame  UI empty or invalid")
TRACE_MSG(LLC_DECODE_174_112_2_17_22_48_45_2793,"LLC:add oc_unack_downlink,512 for missing N(u)")
TRACE_MSG(LLC_DECODE_185_112_2_17_22_48_45_2794,"LLC:retransmit encrypted llc frame with the N(S) belonging to previous 0~511 round")
TRACE_MSG(LLC_DECODE_194_112_2_17_22_48_45_2795,"LLC:the received UI frame is enciphered but no cipher algorithm!")
TRACE_MSG(LLC_DECODE_231_112_2_17_22_48_45_2796,"LLC:before_crc 512+oc_unack_downlink=%d")
TRACE_MSG(LLC_DECODE_281_112_2_17_22_48_45_2797,"LLE: UI frame,invalid FCS! ")
TRACE_MSG(LLC_DECODE_367_112_2_17_22_48_45_2798,"LLE: U frame,invalid FCS! ")
TRACE_MSG(LLC_DECODE_497_112_2_17_22_48_45_2799,"LLE: S frame,invalid FCS! ")
TRACE_MSG(LLC_DECODE_614_112_2_17_22_48_46_2800,"LLC: I frame need to be deciphered!")
TRACE_MSG(LLC_DECODE_633_112_2_17_22_48_46_2801,"LLC: I frame length invalid,discard")
TRACE_MSG(LLC_DECODE_661_112_2_17_22_48_46_2802,"LLC: I frame FCS invalid,discard")
TRACE_MSG(LLC_DECODE_688_112_2_17_22_48_46_2803,"LLC: I frame N(S) invalid,discard")
TRACE_MSG(LLC_ENCODE_894_112_2_17_22_48_48_2804,"LLC:error need check,frame type expected is sack,but bitmap length is 0,change sack to rr")
TRACE_MSG(LLC_ENCODE_957_112_2_17_22_48_48_2805,"LLC:error need check,frame type expected is sack,but bitmap length is 0,change sack to rr")
TRACE_MSG(LLC_ENCODE_1496_112_2_17_22_48_49_2806,"LLC:UPLINK XID PARAMETES-->T200:%d")
TRACE_MSG(LLC_ENCODE_1506_112_2_17_22_48_49_2807,"LLC:UPLINK XID PARAMETES-->N200:%d")
TRACE_MSG(LLC_ENCODE_1518_112_2_17_22_48_49_2808,"LLC:UPLINK XID PARAMETES-->N201 U:%d")
TRACE_MSG(LLC_ENCODE_1535_112_2_17_22_48_49_2809,"LLC:UPLINK XID PARAMETES-->N201 I:%d")
TRACE_MSG(LLC_ENCODE_1547_112_2_17_22_48_49_2810,"LLC:UPLINK XID PARAMETES-->mD:%d")
TRACE_MSG(LLC_ENCODE_1559_112_2_17_22_48_49_2811,"LLC:UPLINK XID PARAMETES-->mU:%d")
TRACE_MSG(LLC_ENCODE_1569_112_2_17_22_48_49_2812,"LLC:UPLINK XID PARAMETES-->kD:%d")
TRACE_MSG(LLC_ENCODE_1579_112_2_17_22_48_49_2813,"LLC:UPLINK XID PARAMETES-->kU:%d")
TRACE_MSG(LLC_ENCODE_1613_112_2_17_22_48_50_2814,"LLC:UPLINK XID PARAMETES-->zero-length Layer 3 Parameters")
TRACE_MSG(LLC_ENCODE_1620_112_2_17_22_48_50_2815,"LLC:UPLINK XID PARAMETES-->zero-length Layer 3 Parameters")
TRACE_MSG(LLC_ENTITY_810_112_2_17_22_48_52_2816,"LLC:u pdu queue is full in abm or adm state!")
TRACE_MSG(LLC_ENTITY_1292_112_2_17_22_48_52_2817,"LLC:discard message:msg id=%d")
TRACE_MSG(LLC_FRAME_293_112_2_17_22_48_54_2818,"LLC:impossible U frame type")
TRACE_MSG(LLC_FRAME_370_112_2_17_22_48_55_2819,"LLC:impossible S frame type")
TRACE_MSG(LLC_FRAME_463_112_2_17_22_48_55_2820,"LLC:impossible I frame type")
TRACE_MSG(LLC_FRAME_528_112_2_17_22_48_55_2821,"LLC:Assert Failure,SetFRMRErrMsg wrong error type!")
TRACE_MSG(LLC_MAIN_567_112_2_17_22_48_58_2822,"LLC: discard GMM_LLME_ASSIGN_REQ due to gmm pid mismatch!")
TRACE_MSG(LLC_MAIN_888_112_2_17_22_48_59_2823,"LLC: discard GMM_LLME_ASSIGN_REQ due to gmm pid mismatch!")
TRACE_MSG(LLC_MAIN_1422_112_2_17_22_49_0_2824,"LLC:discard rlc pdu due to sapi error.")
TRACE_MSG(LLC_MAIN_1428_112_2_17_22_49_0_2825,"LLC:discard rlc pdu due to tlli mismatch.")
TRACE_MSG(LLC_MAIN_1484_112_2_17_22_49_0_2826,"LLC:discard rlc pdu due to sapi error.")
TRACE_MSG(LLC_MAIN_1490_112_2_17_22_49_0_2827,"LLC:discard rlc pdu due to tlli mismatch.")
TRACE_MSG(LLC_MAIN_1626_112_2_17_22_49_1_2828,"LLC: dispatch msg %d to entity %d")
TRACE_MSG(LLC_MAIN_1791_112_2_17_22_49_1_2829,"LLC_TASK: discard signal:%d")
TRACE_MSG(LLC_SDL_145_112_2_17_22_49_2_2830,"LLentity:free,reliability error!")
TRACE_MSG(LLC_SDL_294_112_2_17_22_49_3_2831,"invalid lle_reliability = %d")
TRACE_MSG(LLC_SDL_454_112_2_17_22_49_3_2832,"LLC_SendUIQueue i = %d")
TRACE_MSG(LLC_SDL_584_112_2_17_22_49_3_2833,"LLentity: discard l3 pdu, abnormal")
TRACE_MSG(LLC_TEST_47_112_2_17_22_49_5_2834,"LLC_TEST:lle_ResetT201 expire!")
TRACE_MSG(LLC_TEST_52_112_2_17_22_49_5_2835,"LLC_TEST:lle_SetT200 expire!")
TRACE_MSG(LLC_TEST_57_112_2_17_22_49_5_2836,"LLC_TEST:lle_SetT201 expire!")
TRACE_MSG(LLC_TIMER_65_112_2_17_22_49_6_2837,"LLC: LLC_StartTimer timer_type %d is not present!")
TRACE_MSG(LLC_TIMER_97_112_2_17_22_49_6_2838,"LLC: LLC_StopTimer timer_type %d is not present!")
TRACE_MSG(LLC_TIMER_156_112_2_17_22_49_6_2839,"LLC: LLC_IsTimerActive timer_type %d is not present!")
TRACE_MSG(LLC_XID_391_112_2_17_22_49_7_2840,"LLC:the received U frame does not include xid parameter!")
TRACE_MSG(LLC_XID_808_112_2_17_22_49_8_2841,"LLC:Assert Failure,wrong sapi value!")
TRACE_MSG(LLC_XID_855_112_2_17_22_49_8_2842,"LLC: Xid negotiation N201-U=%d")
TRACE_MSG(LLC_XID_1806_112_2_17_22_49_10_2843,"LLC:Log XID frame parameter:")
TRACE_MSG(LLC_XID_1809_112_2_17_22_49_10_2844,"LLC:xid Reset")
TRACE_MSG(LLC_XID_1813_112_2_17_22_49_10_2845,"LLC:Version = %d")
TRACE_MSG(LLC_XID_1817_112_2_17_22_49_10_2846,"LLC:T200 = %d")
TRACE_MSG(LLC_XID_1821_112_2_17_22_49_10_2847,"LLC:N200 = %d")
TRACE_MSG(LLC_XID_1825_112_2_17_22_49_10_2848,"LLC:N201_U = %d")
TRACE_MSG(LLC_XID_1829_112_2_17_22_49_10_2849,"LLC:N201_I = %d")
TRACE_MSG(LLC_XID_1833_112_2_17_22_49_10_2850,"LLC:mD = %d")
TRACE_MSG(LLC_XID_1837_112_2_17_22_49_10_2851,"LLC:mU = %d")
TRACE_MSG(LLC_XID_1841_112_2_17_22_49_10_2852,"LLC:kD = %d")
TRACE_MSG(LLC_XID_1845_112_2_17_22_49_10_2853,"LLC:kU = %d")
TRACE_MSG(LLC_XID_1849_112_2_17_22_49_10_2854,"LLC:IOV_UI = 0x%08x")
TRACE_MSG(LLC_XID_1853_112_2_17_22_49_10_2855,"LLC:IOV_I = 0x%08x")
TRACE_MSG(LLC_XID_1857_112_2_17_22_49_10_2856,"LLC:SNDCP xid Parameters included")
TRACE_MSG(LLC_XID_CODE_972_112_2_17_22_49_12_2857,"LLC:Log XID frame parameter:")
TRACE_MSG(LLC_XID_CODE_975_112_2_17_22_49_12_2858,"LLC:xid Reset")
TRACE_MSG(LLC_XID_CODE_979_112_2_17_22_49_12_2859,"LLC:Version = %d")
TRACE_MSG(LLC_XID_CODE_983_112_2_17_22_49_12_2860,"LLC:T200 = %d")
TRACE_MSG(LLC_XID_CODE_987_112_2_17_22_49_12_2861,"LLC:N200 = %d")
TRACE_MSG(LLC_XID_CODE_991_112_2_17_22_49_12_2862,"LLC:N201_U = %d")
TRACE_MSG(LLC_XID_CODE_995_112_2_17_22_49_12_2863,"LLC:N201_I = %d")
TRACE_MSG(LLC_XID_CODE_999_112_2_17_22_49_12_2864,"LLC:mD = %d")
TRACE_MSG(LLC_XID_CODE_1003_112_2_17_22_49_12_2865,"LLC:mU = %d")
TRACE_MSG(LLC_XID_CODE_1007_112_2_17_22_49_12_2866,"LLC:kD = %d")
TRACE_MSG(LLC_XID_CODE_1011_112_2_17_22_49_12_2867,"LLC:kU = %d")
TRACE_MSG(LLC_XID_CODE_1015_112_2_17_22_49_12_2868,"LLC:IOV_UI = 0x%08x")
TRACE_MSG(LLC_XID_CODE_1019_112_2_17_22_49_12_2869,"LLC:IOV_I = 0x%08x")
TRACE_MSG(LLC_XID_CODE_1023_112_2_17_22_49_12_2870,"LLC:SNDCP xid Parameters included")
TRACE_MSG(LLC_XID_CODE_1064_112_2_17_22_49_12_2871,"LLC:the received U frame does not include xid parameter!")
TRACE_MSG(MAC_CSN_MSG_1033_112_2_17_22_49_16_2872,"MAC:egprs message!")
TRACE_MSG(MAC_CSN_MSG_1465_112_2_17_22_49_16_2873,"MAC:DL PWR CTRL %d, MODE %d, PR MODE %d")
TRACE_MSG(MAC_CSN_MSG_1512_112_2_17_22_49_17_2874,"MAC: EGPRS window size packet DL assign")
TRACE_MSG(MAC_CSN_MSG_2108_112_2_17_22_49_18_2875,"MAC:ul ack alloc fix alloction,ms not support!")
TRACE_MSG(MAC_CSN_MSG_2188_112_2_17_22_49_18_2876,"MAC:ul ack alloc fix alloction,ms not support!")
TRACE_MSG(MAC_CSN_MSG_2297_112_2_17_22_49_18_2877,"MAC:DL PWR CTRL %d, MODE %d, PR MODE %d")
TRACE_MSG(MAC_CSN_MSG_2311_112_2_17_22_49_18_2878,"MAC:uplink ass allo fix allocation,ms not support!")
TRACE_MSG(MAC_CSN_MSG_2399_112_2_17_22_49_19_2879,"MAC:uplink ass allo fix allocation,ms not support!")
TRACE_MSG(MAC_CSN_MSG_2477_112_2_17_22_49_19_2880,"MAC:uplink ass allo fix allocation,ms not support!")
TRACE_MSG(MAC_CSN_MSG_2685_112_2_17_22_49_19_2881,"MAC:Packet Cell Change Failure have FDD_ind!! ERR!!!")
TRACE_MSG(MAC_CSN_MSG_2690_112_2_17_22_49_19_2882,"MAC:Packet Cell Change Failure have TDD_ind!! ERR!!!")
TRACE_MSG(MAC_CSN_MSG_2879_112_2_17_22_49_20_2883,"MAC:Unknown access type, error!!!")
TRACE_MSG(MAC_CSN_MSG_3173_112_2_17_22_49_20_2884,"MAC:Packet Measurement Report have 3g_meas_report!ERR!!!")
TRACE_MSG(MAC_CSN_MSG_3257_112_2_17_22_49_20_2885,"MAC:EGPRS Ack/Nack Description IE have L =%d")
TRACE_MSG(MAC_CSN_MSG_3285_112_2_17_22_49_20_2886,"MAC:L_length =%d is not enough when encode crbb_len!!\n")
TRACE_MSG(MAC_CSN_MSG_3296_112_2_17_22_49_20_2887,"MAC:L_length =%d is not enough when encode comp_bitmap_start_color_code!!\n")
TRACE_MSG(MAC_CSN_MSG_3343_112_2_17_22_49_21_2888,"MAC:L_length =%d is not enough when encode crbb!!\n")
TRACE_MSG(MAC_CSN_MSG_3394_112_2_17_22_49_21_2889,"MAC:L_length =%d is not enough when encode urbb!!\n")
TRACE_MSG(MAC_CSN_MSG_5708_112_2_17_22_49_26_2890,"MAC:Error,the Access Technology Type num beyond 7!")
TRACE_MSG(MAC_CSN_MSG_5882_112_2_17_22_49_26_2891,"MAC:DL PWR CTRL %d, PR MODE %d")
TRACE_MSG(MAC_CSN_MSG_5980_112_2_17_22_49_26_2892,"MAC: Current cell band = %d")
TRACE_MSG(MAC_CSN_MSG_6849_112_2_17_22_49_28_2893,"MAC:DL PWR CTRL %d, MODE %d, PR MODE %d")
TRACE_MSG(MAC_CSN_MSG_6862_112_2_17_22_49_28_2894,"MAC:  RrPacketUlAssi fix alloc")
TRACE_MSG(MAC_CSN_MSG_6867_112_2_17_22_49_28_2895,"MAC: EGPRS msg Ext alloc")
TRACE_MSG(MAC_CSN_MSG_6975_112_2_17_22_49_28_2896,"MAC:DL PWR CTRL %d, MODE %d, PR MODE %d")
TRACE_MSG(MAC_DECODE_DATA_IND_388_112_2_17_22_49_29_2897,"MAC: tfi ie not present")
TRACE_MSG(MAC_DECODE_DATA_IND_402_112_2_17_22_49_29_2898,"MAC: msg rx on slot%d's global tfi matched the ul tfi, but not on the assigned ul slot!, ")
TRACE_MSG(MAC_DECODE_DATA_IND_421_112_2_17_22_49_29_2899,"MAC: msg rx on slot%d's global tfi matched the dl tfi, but not on the assigned dl slot!, ")
TRACE_MSG(MAC_DECODE_DATA_IND_432_112_2_17_22_49_29_2900,"MAC:Assert Failure, tfi type invalid")
TRACE_MSG(MAC_DECODE_DATA_IND_541_112_2_17_22_49_29_2901,"MAC: ignore msg due to tlli mismatch")
TRACE_MSG(MAC_DECODE_DATA_IND_544_112_2_17_22_49_29_2902,"MAC: match the old tlli")
TRACE_MSG(MAC_DECODE_DATA_IND_548_112_2_17_22_49_29_2903,"MAC: match the new tlli")
TRACE_MSG(MAC_DECODE_DATA_IND_559_112_2_17_22_49_29_2904,"MAC: Assert Failure,Invalid addr")
TRACE_MSG(MAC_DECODE_DATA_IND_616_112_2_17_22_49_30_2905,"MAC: ignore msg due to tlli mismatch")
TRACE_MSG(MAC_DECODE_DATA_IND_619_112_2_17_22_49_30_2906,"MAC: match the old tlli")
TRACE_MSG(MAC_DECODE_DATA_IND_623_112_2_17_22_49_30_2907,"MAC: match the new tlli")
TRACE_MSG(MAC_DECODE_DATA_IND_630_112_2_17_22_49_30_2908,"MAC: Assert Failure,measur order msg error")
TRACE_MSG(MAC_DECODE_DATA_IND_657_112_2_17_22_49_30_2909,"MAC: ignore msg due to tlli mismatch")
TRACE_MSG(MAC_DECODE_DATA_IND_660_112_2_17_22_49_30_2910,"MAC: match the old tlli")
TRACE_MSG(MAC_DECODE_DATA_IND_664_112_2_17_22_49_30_2911,"MAC: match the new tlli")
TRACE_MSG(MAC_DECODE_DATA_IND_671_112_2_17_22_49_30_2912,"MAC: Assert Failure,polling req msg error")
TRACE_MSG(MAC_DECODE_DATA_IND_683_112_2_17_22_49_30_2913,"MAC:The packet queueing nitify message has not request reference!")
TRACE_MSG(MAC_DECODE_DATA_IND_738_112_2_17_22_49_30_2914,"MAC: PAN on slot%d's tfi matched the ul tfi, but not on the assigned ul slot!, ")
TRACE_MSG(MAC_DECODE_DATA_IND_779_112_2_17_22_49_30_2915,"MAC: Assert Failure,unknow msg")
TRACE_MSG(MAC_DECODE_DATA_IND_785_112_2_17_22_49_30_2916,"MAC: MSG decode error!")
TRACE_MSG(MAC_DECODE_DATA_IND_789_112_2_17_22_49_30_2917,"MAC: MSG decode error!")
TRACE_MSG(MAC_DECODE_DATA_IND_795_112_2_17_22_49_30_2918,"MAC: MSG decode error!")
TRACE_MSG(MAC_DECODE_DATA_IND_919_112_2_17_22_49_30_2919,"MAC: Assert Failure,unknow decode msg error")
TRACE_MSG(MAC_DECODE_DATA_IND_1024_112_2_17_22_49_30_2920,"MAC:CTRL REASSEMBLE:T3200 EXPIRE TS %d")
TRACE_MSG(MAC_DECODE_DATA_IND_1053_112_2_17_22_49_31_2921,"MAC: msg rx on slot%d's header tfi is dl tfi, but not on the assigned dl slot!, ")
TRACE_MSG(MAC_DECODE_DATA_IND_1065_112_2_17_22_49_31_2922,"MAC: msg rx on slot%d's header tfi is ul tfi, but not on the assigned ul slot!, ")
TRACE_MSG(MAC_DECODE_DATA_IND_1106_112_2_17_22_49_31_2923,"MAC:CTRL REASSEMBLE PDCH TS %d")
TRACE_MSG(MAC_DECODE_DATA_IND_1184_112_2_17_22_49_31_2924,"MAC: Assert Failure,assemb control eror")
TRACE_MSG(MAC_DECODE_DATA_IND_1193_112_2_17_22_49_31_2925,"MAC:CTRL REASSEMBLE SAVE BLOCK TS %d")
TRACE_MSG(MAC_DECODE_DATA_IND_1263_112_2_17_22_49_31_2926,"MAC: invalid rrbp value,ignore it!!")
TRACE_MSG(MAC_DECODE_DATA_IND_1314_112_2_17_22_49_31_2927,"MAC: recv data block on slot%d tfi match but not on the assigned dl slots")
TRACE_MSG(MAC_DECODE_DATA_IND_1739_112_2_17_22_49_32_2928,"MAC: discard the second ul ack")
TRACE_MSG(MAC_DECODE_DATA_IND_1746_112_2_17_22_49_32_2929,"MAC: ingore msg %x")
TRACE_MSG(MAC_DECODE_DATA_IND_1757_112_2_17_22_49_32_2930,"MAC: in wrong state receive packet uplink ack/unack")
TRACE_MSG(MAC_DECODE_DATA_IND_1819_112_2_17_22_49_32_2931,"MAC: presdo tbf starting time, pkt dl assignment!")
TRACE_MSG(MAC_DECODE_DATA_IND_1828_112_2_17_22_49_32_2932,"MAC: presdo tbf starting time, pkt ul assignment!")
TRACE_MSG(MAC_DECODE_DATA_IND_1837_112_2_17_22_49_32_2933,"MAC: presdo tbf starting time, pkt ts reconfig!")
TRACE_MSG(MAC_DECODE_DATA_IND_1857_112_2_17_22_49_32_2934,"MAC: unknown msg type %d")
TRACE_MSG(MAC_DECODE_DATA_IND_1901_112_2_17_22_49_32_2935,"MAC: Ingore the polling flag!")
TRACE_MSG(MAC_DECODE_DATA_IND_1921_112_2_17_22_49_32_2936,"MAC: Assert Failure,Invalid control ack type")
TRACE_MSG(MAC_DECODE_DATA_IND_1951_112_2_17_22_49_32_2937,"MAC: Assert failure1,Invalid control ack type")
TRACE_MSG(MAC_DECODE_DATA_IND_2033_112_2_17_22_49_32_2938,"MAC:wrong MCS/CS, discard invalid data")
TRACE_MSG(MAC_DECODE_DATA_IND_2092_112_2_17_22_49_33_2939,"mac:error control data type!")
TRACE_MSG(MAC_DECODE_DATA_IND_2110_112_2_17_22_49_33_2940,"mac:error control data type!")
TRACE_MSG(MAC_DECODE_DATA_IND_2116_112_2_17_22_49_33_2941,"MAC: error payload type %d")
TRACE_MSG(MAC_DECODE_DATA_IND_2204_112_2_17_22_49_33_2942,"MAC: error payload type %d")
TRACE_MSG(MAC_DECODE_DATA_IND_2290_112_2_17_22_49_33_2943,"MAC: error,pdch assi invalid pd type %d")
TRACE_MSG(MAC_DECODE_DATA_IND_2349_112_2_17_22_49_33_2944,"MAC:error mutislot control message!")
TRACE_MSG(MAC_DECODE_DATA_IND_2413_112_2_17_22_49_33_2945,"MAC: tlli mismatch in PAR msg!Select next!")
TRACE_MSG(MAC_DECODE_MA_446_112_2_17_22_49_35_2946,"MAC: GPRS class > 12 %d")
TRACE_MSG(MAC_DECODE_MA_480_112_2_17_22_49_35_2947,"MAC: Multislot mismatch, UL timeslot assi num: %d")
TRACE_MSG(MAC_DECODE_MA_488_112_2_17_22_49_35_2948,"MAC: Multislot mismatch, DL timeslot assi num: %d")
TRACE_MSG(MAC_DECODE_MA_496_112_2_17_22_49_35_2949,"MAC: Multislot mismatch, UL timeslot assi num: %d DL timeslot assi num: %d")
TRACE_MSG(MAC_DECODE_MA_524_112_2_17_22_49_35_2950,"MAC: UL slot %x mismatch DL slot %x")
TRACE_MSG(MAC_DECODE_MA_581_112_2_17_22_49_35_2951,"MAC: polling slot before first rx slot, discard!")
TRACE_MSG(MAC_DECODE_MA_588_112_2_17_22_49_35_2952,"MAC: old polling occupy new assignment slot after changed, discard!")
TRACE_MSG(MAC_DECODE_MA_595_112_2_17_22_49_35_2953,"MAC: check polling validate, polling slot and the rx slot collision")
TRACE_MSG(MAC_DECODE_MA_634_112_2_17_22_49_35_2954,"MAC: GPRS class > 12 %d")
TRACE_MSG(MAC_DECODE_MA_639_112_2_17_22_49_35_2955,"MAC: check polling validate, violates MS capability")
TRACE_MSG(MAC_DECODE_MA_911_112_2_17_22_49_35_2956,"MAC: Decode ma error, ref arfcn num=0!")
TRACE_MSG(MAC_DECODE_MA_959_112_2_17_22_49_36_2957,"MAC: Decode ma error, arfcn num!=bitmap len %d %d!")
TRACE_MSG(MAC_DECODE_MA_1022_112_2_17_22_49_36_2958,"MAC: Decode ma error, arfcn num=0!")
TRACE_MSG(MAC_DECODE_MA_1029_112_2_17_22_49_36_2959,"MAC: Decode ma error, arfcn not in band!")
TRACE_MSG(MAC_DECODE_MA_1086_112_2_17_22_49_36_2960,"MAC: Freq para update error in PSI2, invalid ma number!")
TRACE_MSG(MAC_DECODE_MA_1098_112_2_17_22_49_36_2961,"MAC: Freq para update error in PSI2, decode ma error!")
TRACE_MSG(MAC_DECODE_MA_1124_112_2_17_22_49_36_2962,"MAC: Freq para update error in SI13, decode ma error!")
TRACE_MSG(MAC_DECODE_MA_1146_112_2_17_22_49_36_2963,"MAC: Invalid freq param type")
TRACE_MSG(MAC_DECODE_MA_1182_112_2_17_22_49_36_2964,"MAC: Decode freq para error, decode ma error,freq no in band!")
TRACE_MSG(MAC_DECODE_MA_1197_112_2_17_22_49_36_2965,"MAC: Decode freq para error, invalid ma number %d!")
TRACE_MSG(MAC_DECODE_MA_1206_112_2_17_22_49_36_2966,"MAC: Decode freq para error, ma number not referenced %d!")
TRACE_MSG(MAC_DECODE_MA_1217_112_2_17_22_49_36_2967,"MAC: Decode freq para error, ma number=15 but change mark present!")
TRACE_MSG(MAC_DECODE_MA_1241_112_2_17_22_49_36_2968,"MAC: Decode freq para error, change mark not match!")
TRACE_MSG(MAC_DECODE_MA_1249_112_2_17_22_49_36_2969,"MAC: Decode freq para error, change mark not match!")
TRACE_MSG(MAC_DECODE_MA_1270_112_2_17_22_49_36_2970,"MAC: Decode freq para error, decode ma error!")
TRACE_MSG(MAC_DECODE_MA_1288_112_2_17_22_49_36_2971,"MAC: Decode freq bitmap error!")
TRACE_MSG(MAC_DECODE_MA_1295_112_2_17_22_49_36_2972,"MAC: Decode freq para error, decode ma error,freq no in band!")
TRACE_MSG(MAC_DECODE_MA_1308_112_2_17_22_49_36_2973,"MAC: Assert Failure,Invalid freq pars selector %d")
TRACE_MSG(MAC_DECODE_MA_1554_112_2_17_22_49_37_2974,"MAC: MAC mode mismatch, receive ext-dynamic allocation when ul is dynamic!")
TRACE_MSG(MAC_DECODE_MA_1559_112_2_17_22_49_37_2975,"MAC: network alloc EXTENDED_DYNAMIC!")
TRACE_MSG(MAC_DECODE_MA_1568_112_2_17_22_49_37_2976,"MAC: MAC mode mismatch, receive dynamic allocation when ul is ext-dynamic!")
TRACE_MSG(MAC_DECODE_MA_1642_112_2_17_22_49_37_2977,"MAC: receive pkt uplink ass with no tfi when establish uplink TBF")
TRACE_MSG(MAC_DECODE_MA_1819_112_2_17_22_49_37_2978,"MAC: single tbf get slot mask, not on starting time, st = %d, fn = %d")
TRACE_MSG(MAC_DECODE_MA_1936_112_2_17_22_49_38_2979,"absolute frame number")
TRACE_MSG(MAC_DECODE_MA_1945_112_2_17_22_49_38_2980,"MAC:MACMA_DecodeUlMultiBlockAllocation:%d")
TRACE_MSG(MAC_DECODE_MA_2008_112_2_17_22_49_38_2981,"MAC: Decode freq para error, decode ma error,freq no in band!")
TRACE_MSG(MAC_DECODE_MA_2032_112_2_17_22_49_38_2982,"MAC: Decode packet channel descri freq error(invalid ma number)!\n")
TRACE_MSG(MAC_DECODE_MA_2048_112_2_17_22_49_38_2983,"MAC: Decode packet channel descri freq error(change mark not match)\n")
TRACE_MSG(MAC_DECODE_MA_2066_112_2_17_22_49_38_2984,"MAC: Decode packet channel descri freq error(num=0)\n")
TRACE_MSG(MAC_DECODE_MA_2098_112_2_17_22_49_38_2985,"MAC: invalid polling frame number=%d")
TRACE_MSG(MAC_DECODE_MA_2125_112_2_17_22_49_38_2986,"MAC:Invalid control ack type for response imm ass!")
TRACE_MSG(MAC_DECODE_MA_2190_112_2_17_22_49_38_2987,"MAC: Decode ia rest octets error, hh not present!")
TRACE_MSG(MAC_DECODE_MA_2325_112_2_17_22_49_38_2988,"MAC: network alloc fix allocation,ms not support!")
TRACE_MSG(MAC_DECODE_MA_2455_112_2_17_22_49_39_2989,"MAC: network alloc fix allocation,ms not support!")
TRACE_MSG(MAC_DECODE_MA_2509_112_2_17_22_49_39_2990,"MAC: tlli mismatch in DL assi! MS: %x  Assi: %x")
TRACE_MSG(MAC_DECODE_MA_2512_112_2_17_22_49_39_2991,"MAC: match the old tlli")
TRACE_MSG(MAC_DECODE_MA_2516_112_2_17_22_49_39_2992,"MAC: match the new tlli")
TRACE_MSG(MAC_DECODE_MA_2596_112_2_17_22_49_39_2993,"MAC: !!tbf_mode in imm assi is different from current!!")
TRACE_MSG(MAC_DECODE_MA_2617_112_2_17_22_49_39_2994,"MAC: !!EGPRS imm DL assi,but MS don't support EGPRS!!")
TRACE_MSG(MAC_DECODE_MA_2711_112_2_17_22_49_39_2995,"MAC: No ta_value in global_timing_advance!")
TRACE_MSG(MAC_DECODE_MA_2781_112_2_17_22_49_39_2996,"MAC: No valid tfi to encode in psi status msg")
TRACE_MSG(MAC_DECODE_MA_2794_112_2_17_22_49_39_2997,"MAC: rlc add %d in control queue")
TRACE_MSG(MAC_DECODE_MA_3119_112_2_17_22_49_40_2998,"MAC: prach persist_level is wrong!")
TRACE_MSG(MAC_PH_410_112_2_17_22_49_41_2999,"MAC:del num > amount in control queue,check!")
TRACE_MSG(MAC_PH_540_112_2_17_22_49_41_3000,"MAC: add the second timeslot polling data!")
TRACE_MSG(MAC_PH_564_112_2_17_22_49_41_3001,"MAC: replace the more than 2 timeslot polling data!")
TRACE_MSG(MAC_PH_569_112_2_17_22_49_41_3002,"MAC: discard the more than 2 timeslot polling data!")
TRACE_MSG(MAC_PH_940_112_2_17_22_49_42_3003,"MAC:MS band type error,not support! band = %d, ms_band = %d")
TRACE_MSG(MAC_PH_1021_112_2_17_22_49_42_3004,"MAC:Wrong band indicator!!")
TRACE_MSG(MAC_PH_1023_112_2_17_22_49_42_3005,"MAC: get actual band = %d")
TRACE_MSG(MAC_PH_1071_112_2_17_22_49_42_3006,"MAC:Wrong band indicator!!")
TRACE_MSG(MAC_PH_1076_112_2_17_22_49_43_3007,"MAC:error! the required additional access technologies currently not support!!!")
TRACE_MSG(MAC_PH_1131_112_2_17_22_49_43_3008,"MAC:requested access type %d not supported!")
TRACE_MSG(MAC_PH_1323_112_2_17_22_49_43_3009,"MAC:can only encode %d access tech type,the next should be encoded is")
TRACE_MSG(MAC_PH_1771_112_2_17_22_49_44_3010,"MAC:Encode ARAC, but no access technology in ul assign!!!")
TRACE_MSG(MAC_PH_2003_112_2_17_22_49_44_3011,"MAC: get block %d to send in poll")
TRACE_MSG(MAC_PH_2067_112_2_17_22_49_45_3012,"MAC:send packet resource request, don't start 3168")
TRACE_MSG(MAC_PH_2350_112_2_17_22_49_45_3013,"MAC:Mac is already start t3192 when send dl ack!")
TRACE_MSG(MAC_PH_2426_112_2_17_22_49_45_3014,"MAC:link_quality_meas_mode =%d!!\n")
TRACE_MSG(MAC_PH_2510_112_2_17_22_49_45_3015,"MAC:Mac is already start t3192 when send egprs dl ack!")
TRACE_MSG(MAC_PH_3034_112_2_17_22_49_46_3016,"MAC: polling message type error")
TRACE_MSG(MM_CONNECTION_MANAGE_774_112_2_17_22_49_59_3017,"MM TASK: MMCNEC_OutGoingMm_RrSyncInd():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_828_112_2_17_22_50_0_3018,"MM TASK: MMCNEC_AdditionalMmConnect_RrSyncInd():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_985_112_2_17_22_50_0_3019,"MM TASK: MMCNEC_HandleCmPromptRsp():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_1014_112_2_17_22_50_0_3020,"MM_TASK: receive CM_MM_PROMPT_RSP in wrong state")
TRACE_MSG(MM_CONNECTION_MANAGE_1154_112_2_17_22_50_0_3021,"MM TASK: MMCNEC_HandleRrRelIndConnect():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_1170_112_2_17_22_50_0_3022,"MM TASK: MMCNEC_HandleRrRelIndConnect():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_1248_112_2_17_22_50_0_3023,"MM TASK: MMCNEC_HandleRrAbortIndConnect():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_1268_112_2_17_22_50_1_3024,"MM TASK: MMCNEC_HandleRrAbortIndConnect():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_1281_112_2_17_22_50_1_3025,"MM TASK: MMCNEC_HandleRrAbortIndConnect():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_1407_112_2_17_22_50_1_3026,"MM TASK: MNCNEC_WaitOutgoingMm_T3230Exp():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_1470_112_2_17_22_50_1_3027,"MM_TASK:MMCNEC_TiAllocation, pd=%d, ti=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_1520_112_2_17_22_50_1_3028,"MM_TASK:MMCNEC_ClearTi, pd=%d, ti=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_1656_112_2_17_22_50_1_3029,"MM TASK: MMCNEC_WaitOutgoingMm_RrRelInd():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_1680_112_2_17_22_50_1_3030,"MM TASK: MMCNEC_WaitRrRelease_RrRelInd():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_1727_112_2_17_22_50_1_3031,"MM:begin saved call")
TRACE_MSG(MM_CONNECTION_MANAGE_2169_112_2_17_22_50_2_3032,"MM TASK: MMCNEC_OutGoingMm_CmRelReq():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_2185_112_2_17_22_50_2_3033,"MM TASK: MMCNEC_WaitOutgoingMm_RrRelInd():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_2192_112_2_17_22_50_2_3034,"MM TASK: mm connection is not exist for the pd&ti!")
TRACE_MSG(MM_CONNECTION_MANAGE_2215_112_2_17_22_50_2_3035,"MM TASK: MMCNEC_MmConnectActive_CmRelReq():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_2228_112_2_17_22_50_2_3036,"MM TASK: mm connection is not exist for the pd&ti!")
TRACE_MSG(MM_CONNECTION_MANAGE_2255_112_2_17_22_50_2_3037,"MM TASK: MMCNEC_AdditionalMmConnect_CmRelReq():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_2267_112_2_17_22_50_2_3038,"MM TASK: mm connection is not exist for the pd&ti!")
TRACE_MSG(MM_CONNECTION_MANAGE_2289_112_2_17_22_50_3_3039,"MM TASK: MMCNEC_WaitCmResponse_CmRelReq():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_2311_112_2_17_22_50_3_3040,"MM TASK: mm connection is not exist for the pd&ti!")
TRACE_MSG(MM_CONNECTION_MANAGE_2330_112_2_17_22_50_3_3041,"MM TASK: MMCNEC_WaitRrReest_CmRelReq():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_2349_112_2_17_22_50_3_3042,"MM TASK: mm connection is not exist for the pd&ti!")
TRACE_MSG(MM_CONNECTION_MANAGE_2377_112_2_17_22_50_3_3043,"MM TASK: MMCNEC_HandleCmServiceAcceptMsg():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_2452_112_2_17_22_50_3_3044,"MM TASK: MMCNEC_HandleCmServiceRejMsg():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_2471_112_2_17_22_50_3_3045,"MM TASK: MMCNEC_HandleCmServiceRejMsg():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_2521_112_2_17_22_50_3_3046,"MM TASK: MMCNEC_HandleAbortMsg():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_2631_112_2_17_22_50_3_3047,"MM TASK: MMCNEC_HandleOtherPdMsg():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_2676_112_2_17_22_50_3_3048,"MM TASK: MMCNEC_HandleMtMsg():mm_connection_counter=%d")
TRACE_MSG(MM_CONNECTION_MANAGE_2717_112_2_17_22_50_3_3049,"MM:begin saved call")
TRACE_MSG(MM_GMM_PROCEDURE_133_112_2_17_22_50_4_3050,"MM: begin saved call")
TRACE_MSG(MM_MAIN_822_112_2_17_22_50_6_3051,"MM: receive MN_MM_REG_REQ, discard")
TRACE_MSG(MM_MAIN_906_112_2_17_22_50_6_3052,"MM: receive MN_MM_CAMPON_REQ, idle state %d")
TRACE_MSG(MM_MAIN_955_112_2_17_22_50_6_3053,"MM: wait sim reading")
TRACE_MSG(MM_MAIN_1110_112_2_17_22_50_7_3054,"MM: HPLMN updated! reselect PLMN")
TRACE_MSG(MM_MAIN_1203_112_2_17_22_50_7_3055,"MM: receive MN_MM_INIT_REQ")
TRACE_MSG(MM_MAIN_1404_112_2_17_22_50_7_3056,"MM: read sim data error")
TRACE_MSG(MM_MAIN_1462_112_2_17_22_50_7_3057,"MM: wrong state %d receive MM_SIM_DATA_CNF, discard")
TRACE_MSG(MM_MAIN_1482_112_2_17_22_50_8_3058,"MM: SIM Auth Error %d")
TRACE_MSG(MM_MAIN_1515_112_2_17_22_50_8_3059,"MM: receive AUTH_CNF in wrong procedure %d")
TRACE_MSG(MM_MAIN_1635_112_2_17_22_50_8_3060,"MM: receive MM_RR_REL_IND in wrong procedure %d")
TRACE_MSG(MM_MAIN_1728_112_2_17_22_50_8_3061,"MM: discard MM_RR_DATA_IND in IMSI detach")
TRACE_MSG(MM_MAIN_1766_112_2_17_22_50_8_3062,"MM: receive ABORT IND in idle state, discard it")
TRACE_MSG(MM_MAIN_1939_112_2_17_22_50_8_3063,"MM: receive MM_RR_FAILED_MSG_IND in wrong connect substate %d")
TRACE_MSG(MM_MAIN_1944_112_2_17_22_50_8_3064,"MM: receive MM_RR_FAILED_MSG_IND in wrong procedure %d")
TRACE_MSG(MM_MAIN_1965_112_2_17_22_50_8_3065,"MM: receive pro ind in wrong state %d, discard it")
TRACE_MSG(MM_MAIN_2021_112_2_17_22_50_9_3066,"MM: receive pro success in wrong state, discard it")
TRACE_MSG(MM_MAIN_2045_112_2_17_22_50_9_3067,"MM: receive pro success in wrong state, discard it")
TRACE_MSG(MM_MAIN_2072_112_2_17_22_50_9_3068,"MM: receive gmm detach ind in wrong state, discard it")
TRACE_MSG(MM_MAIN_2116_112_2_17_22_50_9_3069,"MM: receive gmm detach ind in wrong state, discard it")
TRACE_MSG(MM_MAIN_2166_112_2_17_22_50_9_3070,"MM: receive T3210_EXP_IND in wrong procedure %d")
TRACE_MSG(MM_MAIN_2269_112_2_17_22_50_9_3071,"MM: receive T3220_EXP_IND in wrong procedure %d")
TRACE_MSG(MM_MAIN_2298_112_2_17_22_50_9_3072,"MM: receive T3230_EXP_IND in wrong substate %d")
TRACE_MSG(MM_MAIN_2303_112_2_17_22_50_9_3073,"MM: receive T3230_EXP_IND in wrong procedure %d")
TRACE_MSG(MM_MAIN_2339_112_2_17_22_50_9_3074,"MM: receive T3240_EXP_IND in wrong procedure %d")
TRACE_MSG(MM_MAIN_2402_112_2_17_22_50_9_3075,"MM: begin saved call")
TRACE_MSG(MM_MSG_452_112_2_17_22_50_10_3076,"MM: decode CM service reject msg,MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MM_MSG_498_112_2_17_22_50_11_3077,"MM CODEC: Assert Failure, DecodeMobileIdentity: offset %d error, there is no MobileIdentity to decode")
TRACE_MSG(MM_MSG_755_112_2_17_22_50_11_3078,"MM: MM_IsIeiValid temp_byte=%d, offset = %d, l3_msg_len = %d")
TRACE_MSG(MM_MSG_776_112_2_17_22_50_11_3079,"MM: decode Msg Header,MESSAGE_TOO_SHORT")
TRACE_MSG(MM_MSG_804_112_2_17_22_50_11_3080,"MM: decode Msg Header,SKIP_INDICATOR_NOT_ZERO")
TRACE_MSG(MM_MSG_838_112_2_17_22_50_11_3081,"MM: decode Msg Header,INVALID_MESSAGE_TYPE")
TRACE_MSG(MM_MSG_850_112_2_17_22_50_11_3082,"MM: decode Msg Header,INVALID_PROTOCOL_DISCRIMINATOR")
TRACE_MSG(MM_MSG_876_112_2_17_22_50_11_3083,"MM CODEC: Assert Failure, EncodeCMServiceReq: CKSN %d error, change to invalid")
TRACE_MSG(MM_MSG_970_112_2_17_22_50_12_3084,"MM: decode Location updating accept msg,min len error.")
TRACE_MSG(MM_MSG_981_112_2_17_22_50_12_3085,"MM: decode Location updating accept msg,LAI invalid.")
TRACE_MSG(MM_MSG_989_112_2_17_22_50_12_3086,"MM:Location updating accept's LAI != current cell LAI")
TRACE_MSG(MM_MSG_1012_112_2_17_22_50_12_3087,"MM: decode Location updating accept msg,%d")
TRACE_MSG(MM_MSG_1036_112_2_17_22_50_12_3088,"MM: decode MM_MOBILE_IDENTITY_IEI failure")
TRACE_MSG(MM_MSG_1082_112_2_17_22_50_12_3089,"MM: decode MM_EMERGENCY_NUM_LIST_IEI failure")
TRACE_MSG(MM_MSG_1087_112_2_17_22_50_12_3090,"MM: receive wrong msg %d")
TRACE_MSG(MM_MSG_1111_112_2_17_22_50_12_3091,"MM: decode Location updating reject msg,MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MM_MSG_1136_112_2_17_22_50_12_3092,"MM CODEC: Assert Failure, EncodeLocUpdReq: CKSN %d error, change to invalid")
TRACE_MSG(MM_MSG_1200_112_2_17_22_50_12_3093,"MM: decode Authentication request msg,MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MM_MSG_1260_112_2_17_22_50_12_3094,"MM: decode Identity request msg,MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MM_MSG_1274_112_2_17_22_50_12_3095,"MM: decode Identity request msg,MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MM_MSG_1343_112_2_17_22_50_12_3096,"MM: decode TMSI reallocation command msg,min len error")
TRACE_MSG(MM_MSG_1351_112_2_17_22_50_12_3097,"MM: decode TMSI reallocation command msg,LAI failure")
TRACE_MSG(MM_MSG_1359_112_2_17_22_50_12_3098,"MM:TMSI reallocation command's LAI != current cell LAI")
TRACE_MSG(MM_MSG_1371_112_2_17_22_50_13_3099,"MM: decode TMSI reallocation command msg,Mobile Identity failure")
TRACE_MSG(MM_MSG_1413_112_2_17_22_50_13_3100,"MM: decode CM service reject msg,MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MM_MSG_1458_112_2_17_22_50_13_3101,"MM CODEC: Assert Failure, EncodeCMReestReq: CKSN %d error, change to invalid")
TRACE_MSG(MM_MSG_1520_112_2_17_22_50_13_3102,"MM: decode Abort msg,MANDATORY_IE_CONTENT_ERROR")
TRACE_MSG(MM_MSG_1543_112_2_17_22_50_13_3103,"MM CODEC: Assert Failure, EncodeMMStatus: Reject cause %d error, set to Protocol error, unspecified")
TRACE_MSG(MM_PLMN_SELECT_503_112_2_17_22_50_14_3104,"MM: discard additional act ind")
TRACE_MSG(MM_PLMN_SELECT_521_112_2_17_22_50_14_3105,"MM: sim present but in NO SIM state!")
TRACE_MSG(MM_PLMN_SELECT_526_112_2_17_22_50_14_3106,"MM: discard additional act ind")
TRACE_MSG(MM_PLMN_SELECT_705_112_2_17_22_50_15_3107,"MM: AddPLMNToPLMNList,plmn:mcc=%d,mnc=%d,mcc_digit_num = %dn")
TRACE_MSG(MM_PLMN_SELECT_783_112_2_17_22_50_15_3108,"MM: AddPLMNToAvailablePLMNList,plmn:mcc=%d,mnc=%d,mnc_digit_num = %dn")
TRACE_MSG(MM_PLMN_SELECT_819_112_2_17_22_50_15_3109,"MM: add manual plmn (%d, %d)")
TRACE_MSG(MM_PLMN_SELECT_829_112_2_17_22_50_15_3110,"MM: disable roaming, add home plmn (%d, %d)")
TRACE_MSG(MM_PLMN_SELECT_833_112_2_17_22_50_15_3111,"MM: auto select mode, add prefer select plmn")
TRACE_MSG(MM_PLMN_SELECT_881_112_2_17_22_50_15_3112,"MM: add registered plmn (%d, %d)")
TRACE_MSG(MM_PLMN_SELECT_890_112_2_17_22_50_15_3113,"MM: CreatePlmnSelectTable, read NV_RPLMN is_enable, mcc %d, mnc %d, mnc_digit_num %d")
TRACE_MSG(MM_PLMN_SELECT_900_112_2_17_22_50_15_3114,"MM: CreatePlmnSelectTable, read NV_RPLMN nv error")
TRACE_MSG(MM_PLMN_SELECT_943_112_2_17_22_50_15_3115,"MM: add PLMN Selector data file plmn, validate_num=%d")
TRACE_MSG(MM_PLMN_SELECT_1060_112_2_17_22_50_15_3116,"MM: get from select plmn list")
TRACE_MSG(MM_PLMN_SELECT_1070_112_2_17_22_50_15_3117,"MM: get from available plmn list")
TRACE_MSG(MM_PLMN_SELECT_1082_112_2_17_22_50_15_3118,"MM: manual mode and all plmn searched, select the user selected plmn(%d, %d)")
TRACE_MSG(MM_PLMN_SELECT_1097_112_2_17_22_50_15_3119,"MM: forbid roaming and all plmn searched, select hplmn")
TRACE_MSG(MM_PLMN_SELECT_1106_112_2_17_22_50_15_3120,"MM: auto mode and all plmn searched, select rplmn or hplmn (%d, %d)")
TRACE_MSG(MM_PLMN_SELECT_1144_112_2_17_22_50_16_3121,"MM: GetPlmnFromSelectPlmnList result = %d")
TRACE_MSG(MM_PLMN_SELECT_1178_112_2_17_22_50_16_3122,"MM: GetPLMNFromAvailableList result = %d")
TRACE_MSG(MM_PLMN_SELECT_1203_112_2_17_22_50_16_3123,"MM: plmn (%d, %d),IsPlmnInSelectPlmnList result = %d")
TRACE_MSG(MM_PLMN_SELECT_1221_112_2_17_22_50_16_3124,"MM: plmn(%d, %d), MMPLMN_FindPlmnInSelectedList result = %d")
TRACE_MSG(MM_PLMN_SELECT_1255_112_2_17_22_50_16_3125,"MM: plmn(%d, %d), MMPLMN_FindPlmnInAvailList result = %d")
TRACE_MSG(MM_PLMN_SELECT_1287_112_2_17_22_50_16_3126,"MM: find la (%d,%d,%d) in flai_s")
TRACE_MSG(MM_PLMN_SELECT_1305_112_2_17_22_50_16_3127,"MM: find la (%d,%d,%d) in flai_r")
TRACE_MSG(MM_PLMN_SELECT_1317_112_2_17_22_50_16_3128,"MM: all la in plmn(%d, %d) is flai")
TRACE_MSG(MM_PLMN_SELECT_1363_112_2_17_22_50_16_3129,"MM: sim is not present but state %d is wrong")
TRACE_MSG(MM_PLMN_SELECT_1444_112_2_17_22_50_16_3130,"MM: modified act type %d, ignore_forbid_list %d, select_any_plmn %d")
TRACE_MSG(MM_PLMN_SELECT_1597_112_2_17_22_50_16_3131,"MM: lai in fplmn list")
TRACE_MSG(MM_PLMN_SELECT_1602_112_2_17_22_50_16_3132,"MM: lai in flai for roaming list")
TRACE_MSG(MM_PLMN_SELECT_1607_112_2_17_22_50_16_3133,"MM: lai in flai for LA not allowed list")
TRACE_MSG(MM_PLMN_SELECT_1703_112_2_17_22_50_17_3134,"MM: begin saved call")
TRACE_MSG(MM_PLMN_SELECT_1734_112_2_17_22_50_17_3135,"MM: begin saved call")
TRACE_MSG(MM_PLMN_SELECT_1770_112_2_17_22_50_17_3136,"MM: begin saved call")
TRACE_MSG(MM_PLMN_SELECT_1799_112_2_17_22_50_17_3137,"MM: begin saved call")
TRACE_MSG(MM_PLMN_SELECT_1997_112_2_17_22_50_17_3138,"MM: begin saved call")
TRACE_MSG(MM_PLMN_SELECT_2646_112_2_17_22_50_19_3139,"MM: network mode I and gprs support by MS, wait GMM do ATTACH/RAU")
TRACE_MSG(MM_PLMN_SELECT_2686_112_2_17_22_50_19_3140,"MM: MS not support CS domain, do nothing")
TRACE_MSG(MM_PLMN_SELECT_2745_112_2_17_22_50_19_3141,"MM: MS not support CS domain, do nothing")
TRACE_MSG(MM_PLMN_SELECT_2859_112_2_17_22_50_19_3142,"MM: need period search PLMN")
TRACE_MSG(MM_PLMN_SELECT_2872_112_2_17_22_50_19_3143,"MM: in manual mode and current PLMN is not the user select PLMN, start 1min timer")
TRACE_MSG(MM_PLMN_SELECT_2882_112_2_17_22_50_19_3144,"MM: in FLAI_S or 4 LU attemp fail, start 1min timer")
TRACE_MSG(MM_PLMN_SELECT_2890_112_2_17_22_50_19_3145,"MM: is_mm_hplmn_search %d")
TRACE_MSG(MM_PLMN_SELECT_2898_112_2_17_22_50_19_3146,"MM: start 3min search timer after power on")
TRACE_MSG(MM_PLMN_SELECT_2904_112_2_17_22_50_19_3147,"MM: start %d s search timer periodically")
TRACE_MSG(MM_PLMN_SELECT_2913_112_2_17_22_50_19_3148,"MM: in HPLMN or EPLMN, not need start timer in normal mode")
TRACE_MSG(MM_PLMN_SELECT_2924_112_2_17_22_50_19_3149,"MM: in HPLMN or EPLMN, but LU fail 4 times, start 1min timer search")
TRACE_MSG(MM_PLMN_SELECT_3095_112_2_17_22_50_20_3150,"MM: MmRrPlmnList, read NV_RPLMN is_enable, mcc %d, mnc %d, mnc_digit_num %d")
TRACE_MSG(MM_PLMN_SELECT_3111_112_2_17_22_50_20_3151,"MM: Find the NV_RPLMN plmn (%d, %d) in availabel PLMN list")
TRACE_MSG(MM_PLMN_SELECT_3122_112_2_17_22_50_20_3152,"MM: Find a higher priority plmn (%d, %d)")
TRACE_MSG(MM_PLMN_SELECT_3136_112_2_17_22_50_20_3153,"MM: Find a registered plmn (%d, %d) in limit state")
TRACE_MSG(MM_PLMN_SELECT_3152_112_2_17_22_50_20_3154,"MM: Find a unknown plmn (%d, %d) in limit state")
TRACE_MSG(MM_PLMN_SELECT_3167_112_2_17_22_50_20_3155,"MM: Find hplmn in limit state")
TRACE_MSG(MM_PLMN_SELECT_3192_112_2_17_22_50_20_3156,"MM: Find user select plmn (%d, %d), try to camp it")
TRACE_MSG(MM_PLMN_SELECT_3362_112_2_17_22_50_20_3157,"MNPHONE:  add PLMN Selector data file plmn, validate_num=%d")
TRACE_MSG(MM_SIGNAL_1030_112_2_17_22_50_23_3158,"MM: unknown PD: %d")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_268_112_2_17_22_50_25_3159,"MM: Send Lu First Time, reset all LU counter and timer")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_393_112_2_17_22_50_25_3160,"MM: receive MM RR ABORT IND in wrong sub state")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_574_112_2_17_22_50_25_3161,"MM: receive data from network in wrong LU state")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_619_112_2_17_22_50_25_3162,"MM: PD Error %d in LUP_INITIATED state")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_629_112_2_17_22_50_25_3163,"MM: Decode Error %d in LUP_INITIATED state")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_717_112_2_17_22_50_25_3164,"MM: Lup Acc identity type error %d")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_767_112_2_17_22_50_26_3165,"MM: LOC UPD Acc Msg decode error %d")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_1000_112_2_17_22_50_26_3166,"MM: begin saved CM request")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_1049_112_2_17_22_50_26_3167,"MM:receive MM_PD MSG at WAIT_FOR_NETWORK_COMMAND\n")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_1127_112_2_17_22_50_26_3168,"MM: receive MM RR REL IND in wrong sub state")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_1394_112_2_17_22_50_27_3169,"MM: current T3212 duration is %d")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_1410_112_2_17_22_50_27_3170,"MM: Start T3212, value %d")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_1450_112_2_17_22_50_27_3171,"MM: Common Procedure msg_type not support %d")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_1458_112_2_17_22_50_27_3172,"MM: Common Procedure PD Error %d")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_1493_112_2_17_22_50_27_3173,"MM: Auth Req Msg decode error %d")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_1561_112_2_17_22_50_27_3174,"MM: Identity Req Msg decode error %d")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_1604_112_2_17_22_50_27_3175,"MM: TmisReallocComm iden type %d error")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_1622_112_2_17_22_50_27_3176,"MM: TMSI reallocation command Msg decode error %d")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_1653_112_2_17_22_50_27_3177,"MM: MM Information Msg decode error %d")
TRACE_MSG(MM_SPECIFIC_PROCEDURE_1761_112_2_17_22_50_28_3178,"MM: Identity type error %d")
TRACE_MSG(MM_TIMER_81_112_2_17_22_50_32_3179,"MM_SetTimer timer_type = %d")
TRACE_MSG(MM_TIMER_107_112_2_17_22_50_32_3180,"MM_KillTimer timer_type = %d")
TRACE_MSG(MM_TIMER_149_112_2_17_22_50_32_3181,"MM_IsTimerActive timer_type = %d")
TRACE_MSG(RLC_DOWNLINK_612_112_2_17_22_50_35_3182,"RLC: too many option octet in data block: %d\n")
TRACE_MSG(RLC_DOWNLINK_618_112_2_17_22_50_35_3183,"RLC: total data len error in data block: %d\n")
TRACE_MSG(RLC_DOWNLINK_695_112_2_17_22_50_35_3184,"RLC: too many option octet in data block: %d\n")
TRACE_MSG(RLC_DOWNLINK_701_112_2_17_22_50_35_3185,"RLC: total data len error in data block: %d\n")
TRACE_MSG(RLC_DOWNLINK_897_112_2_17_22_50_35_3186,"RLC: Reserved M+E: ME = '00'B\n")
TRACE_MSG(RLC_DOWNLINK_998_112_2_17_22_50_36_3187,"RLC: Error option octet: LI = %d, E = %d\n")
TRACE_MSG(RLC_SCT_793_112_2_17_22_50_40_3188,"rlcul: in loopback mode no data recved yet")
TRACE_MSG(RLC_TX_PDU_POOL_73_112_2_17_22_50_43_3189,"rlcul: alloc pdu, pid = %d")
TRACE_MSG(RLC_TX_PDU_POOL_96_112_2_17_22_50_43_3190,"rlcul: free pdu, pid = %d")
TRACE_MSG(RLC_UPLINK_447_112_2_17_22_50_44_3191,"rlcul: network support extended uplink TBF: %d")
TRACE_MSG(RLC_UPLINK_679_112_2_17_22_50_44_3192,"rlcul: MS support extended uplink TBF: %d")
TRACE_MSG(RLC_UPLINK_1084_112_2_17_22_50_45_3193,"rlcul: release error!")
TRACE_MSG(RLC_UPLINK_2915_112_2_17_22_50_49_3194,"rlcul: all data block have been send, still get data block\n")
TRACE_MSG(RLC_UPLINK_2926_112_2_17_22_50_49_3195,"rlcul: error! can't select any pending block")
TRACE_MSG(RLC_UPLINK_3184_112_2_17_22_50_50_3196,"rlcul: error! can't select any pending block")
TRACE_MSG(RLC_UPLINK_3723_112_2_17_22_50_51_3197,"rlcul: error! can't select any pending block")
TRACE_MSG(RLC_UPLINK_4217_112_2_17_22_50_52_3198,"rlcul: gprs tx blk cs%d bsn = %d, va = %d, vs = %d")
TRACE_MSG(RLC_UPLINK_4226_112_2_17_22_50_52_3199,"rlcul: gprs retx %s blk cs%d bsn = %d, va = %d, vs = %d")
TRACE_MSG(RLC_UPLINK_4340_112_2_17_22_50_52_3200,"rlcul: egprs tx blk mcs%d bsn = %d, va = %d, vs = %d")
TRACE_MSG(RLC_UPLINK_4349_112_2_17_22_50_52_3201,"rlcul: egprs retx %s blk mcs%d bsn = %d, va = %d, vs = %d")
TRACE_MSG(RLC_UPLINK_4371_112_2_17_22_50_52_3202,"rlcul: egprs retx %s blk mcs%d bsn = %d, va = %d, vs = %d")
TRACE_MSG(RLC_UPLINK_4377_112_2_17_22_50_52_3203,"rlcul: egprs tx blk mcs%d bsn = %d, va = %d, vs = %d")
TRACE_MSG(RLC_UPLINK_4587_112_2_17_22_50_52_3204,"rlcul: in RLCUP_ProcessAckUnackMsg sendnumcnf")
TRACE_MSG(RLC_UPLINK_4796_112_2_17_22_50_53_3205,"rlcul: decode crbb error!\n")
TRACE_MSG(RLC_UPLINK_4914_112_2_17_22_50_53_3206,"rlcul: in RLCUP_EGPRS_ProcessAckNackMsg sendnumcnf")
TRACE_MSG(RLC_UPLINK_5261_112_2_17_22_50_54_3207,"rlcul: in RLCUP_SetDataEnd sendnumcnf")
TRACE_MSG(RLC_UPLINK_5354_112_2_17_22_50_54_3208,"rlcul: in RLCUP_SetDataEnd sendnumcnf")
TRACE_MSG(RLC_UPLINK_6270_112_2_17_22_50_56_3209,"rlcul:Error!unkown tlli_present!")
TRACE_MSG(RLC_UPLINK_6289_112_2_17_22_50_56_3210,"rlcul: in change tlli sendnumcnf")
TRACE_MSG(RLC_UPLINK_6324_112_2_17_22_50_56_3211,"rlcul: update timeslot mismatch! Error")
TRACE_MSG(RLC_UPLINK_6508_112_2_17_22_50_56_3212,"rlcul: set flow ctrl, llc num = %d, spare num = %d, line = %d")
TRACE_MSG(RLC_UPLINK_6521_112_2_17_22_50_56_3213,"rlcul: set flow ctrl, octet num = %d, line = %d")
TRACE_MSG(RLC_UPLINK_6547_112_2_17_22_50_56_3214,"rlcul: reset flow ctrl, line = %d")
TRACE_MSG(RR_CSN_MSG_4283_112_2_17_22_51_39_3215,"RRC:received System Information 9, not supported now!")
TRACE_MSG(RR_CSN_MSG_4307_112_2_17_22_51_39_3216,"RRC:received System Information 16, not supported now!")
TRACE_MSG(RR_CSN_MSG_4317_112_2_17_22_51_39_3217,"RRC:received System Information 17, not supported now!")
TRACE_MSG(RR_CSN_MSG_4327_112_2_17_22_51_39_3218,"RRC:received System Information 18, not supported now!")
TRACE_MSG(RR_CSN_MSG_4337_112_2_17_22_51_39_3219,"RRC:received System Information 19, not supported now!")
TRACE_MSG(RR_CSN_MSG_4348_112_2_17_22_51_39_3220,"RRC:received System Information 20, not supported now!")
TRACE_MSG(RR_CSN_MSG_6523_112_2_17_22_51_43_3221,"RRC: 2 quater,  index %d, count %d")
TRACE_MSG(RR_CSN_MSG_6634_112_2_17_22_51_44_3222,"rrc: no Optional Selection Parameters in Si4RestOctets, line = %d")
TRACE_MSG(RR_CSN_MSG_6697_112_2_17_22_51_44_3223,"RRC: si2quater available")
TRACE_MSG(RR_CSN_MSG_6722_112_2_17_22_51_44_3224,"rrc: no Optional Selection Parameters in Si4RestOctets, line = %d")
TRACE_MSG(RR_CSN_MSG_6740_112_2_17_22_51_44_3225,"RRC:receive LSA connent in System Information 8,not support!line=%d")
TRACE_MSG(RR_CSN_MSG_6860_112_2_17_22_51_44_3226,"RRC: SGSNR %d")
TRACE_MSG(RR_CSN_MSG_6870_112_2_17_22_51_44_3227,"RRC: si_status_ind %d")
TRACE_MSG(RRCELL_617_112_2_17_22_51_52_3228,"RRC: ref_sen %d, num %d")
TRACE_MSG(RRCELL_622_112_2_17_22_51_52_3229,"RRC: ref_sen %d, num %d")
TRACE_MSG(RRCELL_627_112_2_17_22_51_52_3230,"RRC: ref_sen is 0, num %d")
TRACE_MSG(RRCELL_647_112_2_17_22_51_52_3231,"RRC: clean rxlev count %d")
TRACE_MSG(RRCELL_947_112_2_17_22_51_53_3232,"RRC: In RRCELL_SortRssi, freq_count=%d")
TRACE_MSG(RRCELL_953_112_2_17_22_51_53_3233,"RRC:A = 0x%x, B %d, %d\n")
TRACE_MSG(RRCELL_1240_112_2_17_22_51_53_3234,"RRC: %d In UpdateNCellRssi")
TRACE_MSG(RRCELL_1319_112_2_17_22_51_54_3235,"RRC:RR NCELL: cell from store to temp, arfcn = %d, bsic = %d")
TRACE_MSG(RRCELL_1406_112_2_17_22_51_54_3236,"RRC:RR NCELL: cell from backup to store, arfcn = %d, bsic = %d")
TRACE_MSG(RRCELL_1441_112_2_17_22_51_54_3237,"RRC: NCELL cell from temp to backup, arfcn = %d, bsic = %d")
TRACE_MSG(RRCELL_1643_112_2_17_22_51_54_3238,"RRC: arfcn %d, si7_not_present %d, ")
TRACE_MSG(RRCELL_1651_112_2_17_22_51_54_3239,"RRC: arfcn %d, si8_not_present %d, ")
TRACE_MSG(RRCELL_1817_112_2_17_22_51_55_3240,"RRC: assert unknown si %d")
TRACE_MSG(RRCELL_1854_112_2_17_22_51_55_3241,"RRC: %d checkncellSi, cur %d")
TRACE_MSG(RRCELL_2145_112_2_17_22_51_55_3242,"RRC: GetReselPriority cell_ptr->cell_prio = %d ")
TRACE_MSG(RRCELL_2159_112_2_17_22_51_55_3243,"RRC: %d reselprior %d arfcn 0x%x c1 %d")
TRACE_MSG(RRCELL_2163_112_2_17_22_51_55_3244,"RRC: %d cell prio NO_CELL arfcn = %d, c1 = %d\n")
TRACE_MSG(RRCELL_2268_112_2_17_22_51_56_3245,"RRC:In RRCELL_UpdateBcchSweepResult\n")
TRACE_MSG(RRCELL_2269_112_2_17_22_51_56_3246,"RRC:ARFCN = %d, rssi = %d \n")
TRACE_MSG(RRCELL_3135_112_2_17_22_51_57_3247,"RRC: NCELL cell to store, arfcn = %d, bsic = %d")
TRACE_MSG(RRCELL_3151_112_2_17_22_51_57_3248,"RRC: NCELL cell clear, arfcn = %d, bsic = %d")
TRACE_MSG(RRCELL_3546_112_2_17_22_51_58_3249,"RRC: assert error, exceptional exp")
TRACE_MSG(RRCELL_3557_112_2_17_22_51_58_3250,"RRC: assert error, exceptional exp")
TRACE_MSG(RRCELL_3611_112_2_17_22_51_58_3251,"RRC: %d g_ind %d, 13 %d, ba-ext %d, 2bis %d, 2t_ind %d, 2ter %d")
TRACE_MSG(RRCELL_3654_112_2_17_22_51_58_3252,"RRC: %d EssSiBad %d %d %d %d")
TRACE_MSG(RRCELL_3920_112_2_17_22_51_59_3253,"RRC: %d UpdateBadSi tc 0x%x, pos %d")
TRACE_MSG(RRCELL_3939_112_2_17_22_51_59_3254,"RRC: %d bad si tomax, cell a 0x%x, fn 0x%x")
TRACE_MSG(RRCELL_4014_112_2_17_22_51_59_3255,"RRC: %d BadSiTO MAX tc 0x%x, pos %d")
TRACE_MSG(RRCELL_4131_112_2_17_22_51_59_3256,"RRC: assert error, illogical si, tc %d, pos %d, fn %d")
TRACE_MSG(RRCELL_4162_112_2_17_22_51_59_3257,"RRC: %d gprs_ind %d, si13 %d, ba-ext %d, si2bis %d, 2ter_ind %d, 2ter %d")
TRACE_MSG(RRCELL_4209_112_2_17_22_52_0_3258,"RRC:%d ALL SI OK")
TRACE_MSG(RRCELL_4214_112_2_17_22_52_0_3259,"RRC:%d cell exp, set allneedsi to true")
TRACE_MSG(RRCELL_4235_112_2_17_22_52_0_3260,"RRC: %d IsEssSiOk T, a 0x%x")
TRACE_MSG(RRCELL_4240_112_2_17_22_52_0_3261,"RRC: %d, a 0x%x, p%d,%d %d %d %d")
TRACE_MSG(RRCELL_4262_112_2_17_22_52_0_3262,"RRC: %d MaxRssi, %d not max")
TRACE_MSG(RRCELL_4266_112_2_17_22_52_0_3263,"RRC: %d MaxRssi %d is max")
TRACE_MSG(RRCELL_4293_112_2_17_22_52_0_3264,"RRC: %d %d not bestcell")
TRACE_MSG(RRCELL_4298_112_2_17_22_52_0_3265,"RRC: %d %d is bestcell")
TRACE_MSG(RRCELL_4327_112_2_17_22_52_0_3266,"RRC: %d %d not bestncell")
TRACE_MSG(RRCELL_4332_112_2_17_22_52_0_3267,"RRC: %d %d is bestncell")
TRACE_MSG(RRCELL_4355_112_2_17_22_52_0_3268,"RRC: %d SelBestCell")
TRACE_MSG(RRCELL_4359_112_2_17_22_52_0_3269,"RRC: cell[%d] p %d, rssi %d, pri %d")
TRACE_MSG(RRCELL_4398_112_2_17_22_52_0_3270,"RRC: SelBestHCell sel %d")
TRACE_MSG(RRCELL_4405_112_2_17_22_52_0_3271,"RRC: SelBestRCell sel %d")
TRACE_MSG(RRCELL_4412_112_2_17_22_52_0_3272,"RRC: SelBestCell sel %d")
TRACE_MSG(RRCELL_4463_112_2_17_22_52_0_3273,"RRC: SelBestHCell sel %d")
TRACE_MSG(RRCELL_4470_112_2_17_22_52_0_3274,"RRC: SelBestRCell sel %d")
TRACE_MSG(RRCELL_4477_112_2_17_22_52_0_3275,"RRC: SelBestCell sel %d")
TRACE_MSG(RRCELL_4500_112_2_17_22_52_0_3276,"RRC: SelBestCell sel %d")
TRACE_MSG(RRCELL_4541_112_2_17_22_52_0_3277,"RRC: keep cell %d, a 0x %x")
TRACE_MSG(RRCELL_4581_112_2_17_22_52_1_3278,"%s")
TRACE_MSG(RRCELL_4586_112_2_17_22_52_1_3279,"%s")
TRACE_MSG(RRCELL_4680_112_2_17_22_52_1_3280,"RRC: %d CleanExp")
TRACE_MSG(RRCELL_4685_112_2_17_22_52_1_3281,"C[%d] p %d, a 0x%x, tc 0x%x, du %d, si %d, %d, %d, %d")
TRACE_MSG(RRCELL_4691_112_2_17_22_52_1_3282,"RRC: assert, bsic not ret dur 10s")
TRACE_MSG(RRCELL_4803_112_2_17_22_52_1_3283,"invalid bcch pos")
TRACE_MSG(RRCELL_4821_112_2_17_22_52_1_3284,"RRC: %d SET a 0x%x to bad %d")
TRACE_MSG(RRCELL_4839_112_2_17_22_52_1_3285,"RRC: %d, a 0x%x is bad")
TRACE_MSG(RRCELL_4865_112_2_17_22_52_1_3286,"RRC: %d get free rcell, %d,%d,%d,%d,%d,%d")
TRACE_MSG(RRCELL_4978_112_2_17_22_52_1_3287,"RRC: %d tc_mask_2 0x%x, rcell tc 0x%x")
TRACE_MSG(RRCELL_5140_112_2_17_22_52_2_3288,"RRC: %d plmnscan end s %d, num %d")
TRACE_MSG(RRCELL_5155_112_2_17_22_52_2_3289,"RRC: %d plmnend %d")
TRACE_MSG(RRCELL_COMMON_482_112_2_17_22_52_4_3290,"RRC: %d clearC p %d a 0x%x")
TRACE_MSG(RRCELL_COMMON_503_112_2_17_22_52_4_3291,"RRC: %d clearRc p %d a 0x%x")
TRACE_MSG(RRCELL_COMMON_544_112_2_17_22_52_4_3292,"RRC: %d In  RRCELL_IsAcceptableCell\n")
TRACE_MSG(RRCELL_COMMON_958_112_2_17_22_52_5_3293,"RRC: %d C1 All Low %d")
TRACE_MSG(RRCELL_FAST_PLMN_100_112_2_17_22_52_5_3294,"RRC: assert error bsic>search num")
TRACE_MSG(RRCELL_FAST_PLMN_222_112_2_17_22_52_6_3295,"RRC: assert error bcch>search num")
TRACE_MSG(RRCELL_FAST_PLMN_298_112_2_17_22_52_6_3296,"RRC: assert error bcch>search num")
TRACE_MSG(RRCELL_FAST_PLMN_336_112_2_17_22_52_6_3297,"RRC: %d FillBcch fast p %d, a 0x%x, tc 0x%x")
TRACE_MSG(RRCELL_FAST_PLMN_366_112_2_17_22_52_6_3298,"RRC: %d FillBcch old_tc 0x%x, new 0x%x")
TRACE_MSG(RRCELL_FAST_PLMN_451_112_2_17_22_52_6_3299,"RRC: %d RRCELL_FastPlmnEnd process,scell_pos %d")
TRACE_MSG(RRCELL_FAST_PLMN_457_112_2_17_22_52_6_3300,"RRC: %d check cell %d cell_present %d bsic_exist %d")
TRACE_MSG(RRCELL_FAST_PLMN_522_112_2_17_22_52_6_3301,"RRC: %d all cell search complete,return TRUE")
TRACE_MSG(RRCELL_FAST_PLMN_535_112_2_17_22_52_6_3302,"RRC: %d select result, pos=%d")
TRACE_MSG(RRCELL_FAST_PLMN_549_112_2_17_22_52_6_3303,"RRC: %d select scell, find better ncell %d,return false")
TRACE_MSG(RRCELL_FAST_PLMN_564_112_2_17_22_52_6_3304,"RRC: %d select ncell, scell %d is better,return false")
TRACE_MSG(RRCELL_FAST_PLMN_579_112_2_17_22_52_6_3305,"RRC: %d select ncell, find better ncell %d,return false")
TRACE_MSG(RRCELL_FAST_PLMN_648_112_2_17_22_52_6_3306,"RRC: %d fast sel %d a 0x%x")
TRACE_MSG(RRCELL_FAST_PLMN_651_112_2_17_22_52_6_3307,"RRC: %d fast nocell")
TRACE_MSG(RRCELL_GPRS_163_112_2_17_22_52_7_3308,"RRC: Assert Failure, unknown extend report type.")
TRACE_MSG(RRCELL_GPRS_970_112_2_17_22_52_9_3309,"RRC:TLLI in PacketCellChangeOrder is wrong!new_tiil=%d, old_tlli=%d, rec_tlli=%d")
TRACE_MSG(RRCELL_GPRS_978_112_2_17_22_52_9_3310,"RRC: pcco get tlli fail")
TRACE_MSG(RRCELL_GPRS_996_112_2_17_22_52_9_3311,"RRC: pcco check nc meas param fail")
TRACE_MSG(RRCELL_GPRS_1007_112_2_17_22_52_9_3312,"RRC: pcco check lsa fail")
TRACE_MSG(RRCELL_GPRS_1018_112_2_17_22_52_9_3313,"RRC: pcco check enhance meas param fail")
TRACE_MSG(RRCELL_GPRS_1805_112_2_17_22_52_11_3314,"RRC: cell reselection is controled by network\n")
TRACE_MSG(RRCELL_GPRS_1888_112_2_17_22_52_11_3315,"RRC:TLLI in PacketMeasureOrder is wrong!new_tiil=%d, old_tlli=%d, rec_tlli=%d")
TRACE_MSG(RRCELL_GPRS_2492_112_2_17_22_52_12_3316,"RRC: pcco check arfcn fail")
TRACE_MSG(RRCELL_GPRS_2564_112_2_17_22_52_12_3317,"RRC: scell is barred cba2 = 1\n")
TRACE_MSG(RRCELL_GPRS_2573_112_2_17_22_52_12_3318,"RRC: scell c1 <= 0\n")
TRACE_MSG(RRCELL_GPRS_2592_112_2_17_22_52_12_3319,"RRC:In  RRCELL_ProcessReselTimerGprs\n")
TRACE_MSG(RRCELL_GPRS_2593_112_2_17_22_52_12_3320,"RRC:ARFCN = %d, rssi = %d, c1 = %d, c31 = %d, c32 = %d\n")
TRACE_MSG(RRCELL_GPRS_2598_112_2_17_22_52_12_3321,"RRC:ARFCN = %d, rssi = %d, c1 = %d, c31 = %d, c32 = %d\n")
TRACE_MSG(RRCELL_GPRS_3143_112_2_17_22_52_13_3322,"RRC: %d scell ba_ext %d, 2bis %d, tc5bad %d")
TRACE_MSG(RRCELL_GPRS_3144_112_2_17_22_52_13_3323,"RRC: 2ter_ind %d, 2ter_p %d, tc4bad %d")
TRACE_MSG(RRCELL_RESEL_953_112_2_17_22_52_16_3324,"RRC: Select cell 0x%x ignore si when scell access fail.")
TRACE_MSG(RRCELL_RESEL_1135_112_2_17_22_52_16_3325,"RR: find a cell support emergency call, arfcn = 0x%x")
TRACE_MSG(RRCELL_RESEL_1552_112_2_17_22_52_17_3326,"RRC:%d In ReselTimer\n")
TRACE_MSG(RRCELL_RESEL_1553_112_2_17_22_52_17_3327,"RRC:A = 0x%x, rssi = %d, c1 = %d, c2 = %d\n")
TRACE_MSG(RRCELL_RESEL_1557_112_2_17_22_52_17_3328,"RRC:A = 0x%x, rssi = %d, c1 = %d, c2 = %d\n")
TRACE_MSG(RRCELL_RESEL_1569_112_2_17_22_52_17_3329,"RRC resel: scell: c1=%d, c2=%d; max rssi ncell: arfcn=%d, c1=%d, c2=%d; ct=%d")
TRACE_MSG(RRCELL_SEARCH_PLMN_263_112_2_17_22_52_18_3330,"RRC: %d ASSERT ERROR, no space when search")
TRACE_MSG(RRCELL_SEARCH_PLMN_323_112_2_17_22_52_18_3331,"RRC: %d research num %d")
TRACE_MSG(RRCELL_SEARCH_PLMN_376_112_2_17_22_52_18_3332,"RRC: cell[%d] p %d")
TRACE_MSG(RRCELL_SEARCH_PLMN_391_112_2_17_22_52_18_3333,"RRC: %d PlmnEnd? s %d, num %d, end %d")
TRACE_MSG(RRCELL_SEARCH_PLMN_440_112_2_17_22_52_18_3334,"RRC: %d ASSERT ERROR, no space when search")
TRACE_MSG(RRDM_325_112_2_17_22_52_20_3335,"Invalid rat change req mode")
TRACE_MSG(RRDM_382_112_2_17_22_52_20_3336,"@@@@@@ SEND class mark to layer 1")
TRACE_MSG(RRDM_386_112_2_17_22_52_20_3337,"@@@@@@ DM Get arfcn , arfcn=%d")
TRACE_MSG(RRDM_428_112_2_17_22_52_20_3338,"@@@@@@ DM handle ho cmd , message error")
TRACE_MSG(RRDM_434_112_2_17_22_52_20_3339,"@@@@@@ DM handle ho cmd , PD or msg-type ERROR")
TRACE_MSG(RRDM_440_112_2_17_22_52_20_3340,"@@@@@@ DM call decode function to decode ho cmd")
TRACE_MSG(RRDM_534_112_2_17_22_52_20_3341,"@@@@@@ RRDM_CheckArfcnBsic , BSIC not matched")
TRACE_MSG(RRDM_935_112_2_17_22_52_21_3342,"DGY_COMP_1")
TRACE_MSG(RRDM_950_112_2_17_22_52_21_3343,"DGY_COMP_2")
TRACE_MSG(RRDM_966_112_2_17_22_52_21_3344,"DGY_COMP_3")
TRACE_MSG(RRDM_974_112_2_17_22_52_21_3345,"BREAK_3")
TRACE_MSG(RRDM_1000_112_2_17_22_52_21_3346,"@@@@@@ SEND MPH_STOP_TD_MEAS_REQ to layer 1")
TRACE_MSG(RRDM_1019_112_2_17_22_52_21_3347,"TD ncell not present")
TRACE_MSG(RRDM_1029_112_2_17_22_52_21_3348,"TD neighbour cell is 0")
TRACE_MSG(RRDM_1038_112_2_17_22_52_21_3349,"RRDM_Fill_TD_Meas:AFTER uptade :t_ptr->arfcn_num = %d")
TRACE_MSG(RRDM_1050_112_2_17_22_52_21_3350,"RRDM_Fill_TD_Meas:arfcn_list_arr[ %d ] = %d")
TRACE_MSG(RRDM_1057_112_2_17_22_52_21_3351,"RRDM_Fill_TD_Meas:cell_param_id_list_arr[ %d ]= %d")
TRACE_MSG(RRDM_1062_112_2_17_22_52_21_3352,"RRDM_Fill_TD_Meas:arfcn_list_chg_ind = TRUE")
TRACE_MSG(RRDM_1074_112_2_17_22_52_21_3353,"i = %d,arfcn = %d")
TRACE_MSG(RRDM_1075_112_2_17_22_52_21_3354,"i = %d,cell_param_id_num = %d")
TRACE_MSG(RRDM_1080_112_2_17_22_52_21_3355,"i = %d,arfcn = %d")
TRACE_MSG(RRDM_1081_112_2_17_22_52_21_3356,"i = %d,cell_param_id_num = %d")
TRACE_MSG(RRDM_1092_112_2_17_22_52_21_3357,"t_ptr->arfcn_num = %d")
TRACE_MSG(RRDM_1096_112_2_17_22_52_21_3358,"i = %d,arfcn = %d")
TRACE_MSG(RRDM_1097_112_2_17_22_52_21_3359,"i = %d,cell_param_id_num = %d")
TRACE_MSG(RRDM_1141_112_2_17_22_52_21_3360,"@@@@@@ Fill td cell table , num = %d")
TRACE_MSG(RRDM_1194_112_2_17_22_52_21_3361,"@@@@@@ Fill td cell table , num = %d")
TRACE_MSG(RRDM_1275_112_2_17_22_52_21_3362,"MS is not Dule-Mode,does not need to start TD measurement")
TRACE_MSG(RRDM_1283_112_2_17_22_52_21_3363,"is_to_fill_td_param = %d,\n meas_param_desc_3g_present = %d,\ngsm_scell.rssi = %d,\nqsearch_i = %d,s_q_search_i_table = %d")
TRACE_MSG(RRDM_1290_112_2_17_22_52_21_3364,"START_TD_ALREADY")
TRACE_MSG(RRDM_1293_112_2_17_22_52_21_3365,"TD info changed")
TRACE_MSG(RRDM_1372_112_2_17_22_52_22_3366,"Q_offset =0,resel to TDD immediate")
TRACE_MSG(RRDM_1389_112_2_17_22_52_22_3367,"NCELL %d RSSI = %d\n,TDCELL RSCP = %d\n,penny = %d\n ,qoffset = %d\n,offset = %d ")
TRACE_MSG(RRDM_1435_112_2_17_22_52_22_3368,"DM:non-dule mode")
TRACE_MSG(RRDM_1507_112_2_17_22_52_22_3369,"index = %d,arfcn = %d,cell_id = %d,rscp = %d")
TRACE_MSG(RRDM_1603_112_2_17_22_52_22_3370,"@@@@@@ DM , SEND GSM2TD IND TO TD")
TRACE_MSG(RRDM_1739_112_2_17_22_52_22_3371,"Invalid rat change req mode")
TRACE_MSG(RRDM_1843_112_2_17_22_52_23_3372,"@@@@@@ start to waiting for td layer 1 download gsm dsp")
TRACE_MSG(RRDM_1845_112_2_17_22_52_23_3373,"@@@@@@  td layer 1 download gsm dsp complete ")
TRACE_MSG(RRDM_1962_112_2_17_22_52_23_3374,"Invalid rat change req mode during CCOFU procedure")
TRACE_MSG(RRDM_2172_112_2_17_22_52_23_3375,"Is eplmn")
TRACE_MSG(RRDM_2212_112_2_17_22_52_23_3376,"tdd_meas_ptr->cell_num after clear = %d")
TRACE_MSG(RRDM_2249_112_2_17_22_52_23_3377,"SI2QUATER %d segment not received!")
TRACE_MSG(RRDM_2257_112_2_17_22_52_23_3378,"SI2QUATER all segment received!")
TRACE_MSG(RRDM_2322_112_2_17_22_52_24_3379,"Q_offset =0,resel to TDD immediate")
TRACE_MSG(RRDM_2350_112_2_17_22_52_24_3380,"cancel candidate_times TDD CELL %d ")
TRACE_MSG(RRDM_2351_112_2_17_22_52_24_3381,"rssi = %d,rscp = %d ,offset = %d")
TRACE_MSG(RRDM_2431_112_2_17_22_52_24_3382,"DM resel:TDD cell %d")
TRACE_MSG(RRDM_2654_112_2_17_22_52_24_3383,"&&&&&& GUOYAN:PMO is received!")
TRACE_MSG(RRDM_2691_112_2_17_22_52_24_3384,"PMO :ENH Measurement Parameters  included")
TRACE_MSG(RRDM_2717_112_2_17_22_52_24_3385,"PMO :3G Neighbour Cell Description  included")
TRACE_MSG(RRDM_2825_112_2_17_22_52_25_3386,"PMO :GPRS 3G MEASUREMENT Parameters  included")
TRACE_MSG(RRDM_2866_112_2_17_22_52_25_3387,"&&&&&& GUOYAN:PMO is received!")
TRACE_MSG(RRDM_2895_112_2_17_22_52_25_3388,"%%%%%%GUOYAN: PCCO NC_MODE = %d")
TRACE_MSG(RRDM_2908_112_2_17_22_52_25_3389,"PMO :EXT Measurement Parameters  included")
TRACE_MSG(RRDM_2982_112_2_17_22_52_25_3390,"    temp_ba_ind = %d,\n    temp_3g_ba_ind = %d,\n    temp_mp_change_mark = %d,\n    temp_si2quater_index = %d,\n   temp_si2quater_count = %d")
TRACE_MSG(RRDM_2987_112_2_17_22_52_25_3391,"instance %d hans been received,return")
TRACE_MSG(RRDM_3048_112_2_17_22_52_25_3392,"SIQuater:Measurement_Parameters Description is included!\n")
TRACE_MSG(RRDM_3057_112_2_17_22_52_25_3393,"SIQuater:GPRS RTD description is not supported by this version!\n")
TRACE_MSG(RRDM_3063_112_2_17_22_52_25_3394,"SIQuater:GPRS_BSIC Descriptionis included!\n")
TRACE_MSG(RRDM_3071_112_2_17_22_52_25_3395,"SIQuater:GPRS_REPORT PRIORITY Description is included!\n")
TRACE_MSG(RRDM_3087_112_2_17_22_52_25_3396,"SIQuater:GPRS_MEASUREMENT_Parameters Description is included!\n")
TRACE_MSG(RRDM_3098_112_2_17_22_52_25_3397,"SIQuater :NC Measurement Parameters included")
TRACE_MSG(RRDM_3105_112_2_17_22_52_25_3398,"SIQuater :extension length included")
TRACE_MSG(RRDM_3123_112_2_17_22_52_25_3399,"SIQuater :3G Neighbour Cell Description  included")
TRACE_MSG(RRDM_3139_112_2_17_22_52_25_3400,"SIQuater :3G Measurement Parameters Description  included")
TRACE_MSG(RRDM_3144_112_2_17_22_52_25_3401,"SIQuater :GPRS_3G_MEASUREMENT Parameters Description  included")
TRACE_MSG(RRDM_3174_112_2_17_22_52_25_3402,"Index_Start_3G included")
TRACE_MSG(RRDM_3184_112_2_17_22_52_25_3403,"Index_Start_3G included")
TRACE_MSG(RRDM_3195_112_2_17_22_52_25_3404,"FDD neighbor cell is assigned  in SI2Quater!\n")
TRACE_MSG(RRDM_3202_112_2_17_22_52_25_3405,"UTRAN TDD Description included")
TRACE_MSG(RRDM_3244_112_2_17_22_52_25_3406,"Bandwidth_TDD included")
TRACE_MSG(RRDM_3248_112_2_17_22_52_25_3407,"@@@@@384 Mcps band is received,which is not supported in this version!\n")
TRACE_MSG(RRDM_3256_112_2_17_22_52_25_3408,"@@@@@384 Mcps band is received,which is not supported in this version!\n")
TRACE_MSG(RRDM_3261_112_2_17_22_52_25_3409,"Repeated UMTS TDD Neighbour Cells included")
TRACE_MSG(RRDM_3285_112_2_17_22_52_25_3410,"L= %d,tdd_arfcn_valid_ind = %d")
TRACE_MSG(RRDM_3301_112_2_17_22_52_25_3411,"L= %d,arfcn = %d")
TRACE_MSG(RRDM_3303_112_2_17_22_52_25_3412,"L= %d,nr_of_tdd_cells = %d")
TRACE_MSG(RRDM_3425_112_2_17_22_52_26_3413,"Bandwidth_FDD included")
TRACE_MSG(RRDM_3430_112_2_17_22_52_26_3414,"Repeated UMTS FDD Neighbour Cells included")
TRACE_MSG(RRDM_3466_112_2_17_22_52_26_3415,"3GMeasurementParamentsDescription:search_i = %d")
TRACE_MSG(RRDM_3480_112_2_17_22_52_26_3416,"3GMeasurementParamentsDescription:fdd measurement information is included,need skip all bits in now version")
TRACE_MSG(RRDM_3494_112_2_17_22_52_26_3417,"3GMeasurementParamentsDescription:tdd_qoffset = %d\n,tdd_multirat_reporting = %d")
TRACE_MSG(RRDM_3515_112_2_17_22_52_26_3418,"GPRS3GMeasurementParamentsDescription:search_p = %d")
TRACE_MSG(RRDM_3517_112_2_17_22_52_26_3419,"GPRS3GMeasurementParamentsDescription:earch_prio_3g = %d")
TRACE_MSG(RRDM_3533_112_2_17_22_52_26_3420,"GPRS3GMeasurementParamentsDescription:dd_multirat_reporting = %d")
TRACE_MSG(RRDM_3538_112_2_17_22_52_26_3421,"GPRS3GMeasurementParamentsDescription:dd_reporting_offset = %d")
TRACE_MSG(RRDM_3540_112_2_17_22_52_26_3422,"GPRS3GMeasurementParamentsDescription:dd_reporting_threshold = %d")
TRACE_MSG(RRDM_3762_112_2_17_22_52_26_3423,"SIQuater:GPRS_MEASUREMENT_Parameters Description is included!\n")
TRACE_MSG(RRDM_3895_112_2_17_22_52_27_3424,"SIQuater:skip following bits!\n")
TRACE_MSG(RRDM_3899_112_2_17_22_52_27_3425,"SIQuater:RTD6 exist!\n")
TRACE_MSG(RRDM_3928_112_2_17_22_52_27_3426,"SIQuater:RTD12 exist!\n")
TRACE_MSG(RRDM_3976_112_2_17_22_52_27_3427,"Extension Information LENGTH =  %d")
TRACE_MSG(RRDM_3978_112_2_17_22_52_27_3428,"RR_DecodeGprsCellOption:left length = %d")
TRACE_MSG(RRDM_3981_112_2_17_22_52_27_3429,"EGPRS Information is included  in GPRS_CELL_OPTION IE")
TRACE_MSG(RRDM_3996_112_2_17_22_52_27_3430,"CCN_ACTIVE is %d  in GPRS_CELL_OPTION IE")
TRACE_MSG(RRDM_4029_112_2_17_22_52_27_3431,"3G neighbour cell desc not present")
TRACE_MSG(RRDM_4081_112_2_17_22_52_27_3432,"@@@@@@ dm_td_cells_info , cell_num = %d")
TRACE_MSG(RRDM_4139_112_2_17_22_52_27_3433,"%%%%%%GUOYAN :tdd measurement report is included")
TRACE_MSG(RRDM_4273_112_2_17_22_52_27_3434,"@@@@@@ CALC TD RANK , arfcn=%d , cpi=%d ,rscp=%d , offset=%d , rank =%d")
TRACE_MSG(RRDM_4353_112_2_17_22_52_28_3435,"R[%d] = %d,R[%d]= %d")
TRACE_MSG(RRDM_4358_112_2_17_22_52_28_3436,"R[%d] = %d,R[%d]= %d")
TRACE_MSG(RRDM_4363_112_2_17_22_52_28_3437,"i=%d,rn= %d")
TRACE_MSG(RRDM_4417_112_2_17_22_52_28_3438,"PCCN:Downlink_tfi present: %d")
TRACE_MSG(RRDM_4424_112_2_17_22_52_28_3439,"PCCN:Uplink_tfi present: %d")
TRACE_MSG(RRDM_4433_112_2_17_22_52_28_3440,"V44060_5j0")
TRACE_MSG(RRDM_4517_112_2_17_22_52_28_3441,"V44060_5j0")
TRACE_MSG(RRDM_4659_112_2_17_22_52_28_3442,"&&&&&&&modify for dongguan:\n SI2QUATER instance %d not present,need wait it!")
TRACE_MSG(RRDM_4665_112_2_17_22_52_28_3443,"all sengment is received!")
TRACE_MSG(RRDM_4696_112_2_17_22_52_28_3444,"this arfcn no cell! ,discard this arfcn")
TRACE_MSG(RRDM_4709_112_2_17_22_52_28_3445,"RRMSG.C:SCELL ARFCN = %d")
TRACE_MSG(RRDM_4710_112_2_17_22_52_28_3446,"nr_repeated_utran_tdd_neighbour_cell = %d")
TRACE_MSG(RRDM_4711_112_2_17_22_52_28_3447,"cell_ptr->neighbour_cell_desc_3g.neighbour_cell_desc_3g_present = %d")
TRACE_MSG(RRMSG_1845_112_2_17_22_52_32_3448,"RR:si 1, pcs ind is TRUE ")
TRACE_MSG(RRMSG_2215_112_2_17_22_52_33_3449,"RRC: si2q av")
TRACE_MSG(RRMSG_2223_112_2_17_22_52_33_3450,"RRC: si2q p = %d")
TRACE_MSG(RRMSG_2230_112_2_17_22_52_33_3451,"RRC: A = %d, ra_colour = %d\n ")
TRACE_MSG(RRMSG_2340_112_2_17_22_52_33_3452,"RRC: pcs ind %d")
TRACE_MSG(RRMSG_2343_112_2_17_22_52_33_3453,"RRC: SI6 rest len wrong")
TRACE_MSG(RRMSG_2376_112_2_17_22_52_33_3454,"RRC: network assign half rate channel!")
TRACE_MSG(RRMSG_3326_112_2_17_22_52_35_3455,"RRC: SGSNR %d")
TRACE_MSG(RRMSG_3364_112_2_17_22_52_35_3456,"RRC: si2bis is not present, ba_0 is %d")
TRACE_MSG(RRMSG_3375_112_2_17_22_52_35_3457,"RRC: ba_0 is %d")
TRACE_MSG(RRMSG_3473_112_2_17_22_52_36_3458,"RRC: cell bar")
TRACE_MSG(RRMSG_3556_112_2_17_22_52_36_3459,"RRC: cell bar")
TRACE_MSG(RRMSG_4085_112_2_17_22_52_37_3460,"RRC: egprs packet uplink extend_RA %d")
TRACE_MSG(RRMSG_4189_112_2_17_22_52_37_3461,"RRC: gprs packet uplink extend_RA %d")
TRACE_MSG(RRMSG_4231_112_2_17_22_52_37_3462,"RRC:Timing Advance is out of range!")
TRACE_MSG(RRMSG_4667_112_2_17_22_52_38_3463,"RRC: ba_5_ind and si5bis not present")
TRACE_MSG(RRMSG_4683_112_2_17_22_52_38_3464,"RRC: ba_2_ind and si2bis not present")
TRACE_MSG(RRMSG_4801_112_2_17_22_52_38_3465,"RRC: use bcch ba when traffic.")
TRACE_MSG(RRMSG_4934_112_2_17_22_52_39_3466,"RRC: unknown msg type %d")
TRACE_MSG(RRMSG_5084_112_2_17_22_52_39_3467,"RRC: immed_assign->request_ref->random_ab %x")
TRACE_MSG(RRMSG_5114_112_2_17_22_52_39_3468,"RRC: hopping false.")
TRACE_MSG(RRMSG_5374_112_2_17_22_52_39_3469,"RRC: recv imm reject")
TRACE_MSG(RRMSG_5408_112_2_17_22_52_40_3470,"CASE207:get si2bis ba")
TRACE_MSG(RRMSG_5414_112_2_17_22_52_40_3471,"CASE207:invalid si2bis")
TRACE_MSG(RRMSG_DECODE_DEDICATED_298_112_2_17_22_52_41_3472,"RR:MR config icmi %d start_mode %d acs 0x%x")
TRACE_MSG(RRMSG_DECODE_DEDICATED_441_112_2_17_22_52_42_3473,"RR: MULTI SLOT ERROR!")
TRACE_MSG(RRMSG_DECODE_DEDICATED_954_112_2_17_22_52_43_3474,"RRC: chan_rel ba_pref len wrong.")
TRACE_MSG(RRMSG_DECODE_DEDICATED_1168_112_2_17_22_52_43_3475,"RRC: VGCS???")
TRACE_MSG(RRMSG_DECODE_DEDICATED_1179_112_2_17_22_52_43_3476,"RR:Channel modify msg error")
TRACE_MSG(RRMSG_DECODE_DEDICATED_1579_112_2_17_22_52_44_3477,"RRC: VGCS???")
TRACE_MSG(RRMSG_DECODE_DEDICATED_1589_112_2_17_22_52_44_3478,"RR:Channel assign msg error")
TRACE_MSG(RRMSG_DECODE_DEDICATED_1609_112_2_17_22_52_44_3479,"RRC: assert error, assign_cmd start cipher but no cipher cmd recv before")
TRACE_MSG(RRMSG_DECODE_DEDICATED_2144_112_2_17_22_52_45_3480,"RRC: VGCS???")
TRACE_MSG(RRMSG_DECODE_DEDICATED_2154_112_2_17_22_52_45_3481,"RR:Channel assign msg error")
TRACE_MSG(RRMSG_DECODE_DEDICATED_2174_112_2_17_22_52_45_3482,"RRC: assert error, handover start cipher but no cipher cmd recv before")
TRACE_MSG(RRMSG_DECODE_DEDICATED_2365_112_2_17_22_52_46_3483,"RRC:received Extended Mearsurement Order!")
TRACE_MSG(RRMSG_GPRS_134_112_2_17_22_52_48_3484,"RRC: 2 quater, index %d, count %d")
TRACE_MSG(RRMSG_GPRS_258_112_2_17_22_52_49_3485,"RRC: si2quater, nc_order, non_drx %d, i %d, t %d")
TRACE_MSG(RRMSG_GPRS_278_112_2_17_22_52_49_3486,"RRC:received System Information 9, not supported now!")
TRACE_MSG(RRMSG_GPRS_296_112_2_17_22_52_49_3487,"RRC:received System Information 16, not supported now!")
TRACE_MSG(RRMSG_GPRS_314_112_2_17_22_52_49_3488,"RRC:received System Information 17, not supported now!")
TRACE_MSG(RRMSG_GPRS_332_112_2_17_22_52_49_3489,"RRC:received System Information 18, not supported now!")
TRACE_MSG(RRMSG_GPRS_350_112_2_17_22_52_49_3490,"RRC:received System Information 19, not supported now!")
TRACE_MSG(RRMSG_GPRS_368_112_2_17_22_52_49_3491,"RRC:received System Information 20, not supported now!")
TRACE_MSG(RRPROC_115_112_2_17_22_52_58_3492,"RR_PAGING_TIMER: accur = %d")
TRACE_MSG(RRPROC_131_112_2_17_22_52_58_3493,"RR_PAGING_TIMER: SI13[%d] duration:%d accurate:%d")
TRACE_MSG(RRPROC_150_112_2_17_22_52_58_3494,"RR_PAGING_TIMER: SI13[%d] still active")
TRACE_MSG(RRPROC_395_112_2_17_22_52_59_3495,"RRPROC:Default case!")
TRACE_MSG(RRPROC_605_112_2_17_22_52_59_3496,"RRC: cell [%d] cell_present %d bad %d g_ind %d rssi %d")
TRACE_MSG(RRPROC_630_112_2_17_22_52_59_3497,"RRC: find? %d, ncell %d")
TRACE_MSG(RRPROC_777_112_2_17_22_53_0_3498,"RRC:RR NCELL: cell clear all")
TRACE_MSG(RRPROC_1386_112_2_17_22_53_1_3499,"RRC: use band %d")
TRACE_MSG(RRPROC_1411_112_2_17_22_53_1_3500,"RRC: Assert error, invalid arfcn %d")
TRACE_MSG(RRPROC_1550_112_2_17_22_53_1_3501,"RRC: Gprs meas freq should not over MAX_MEAS_NUM_GRPS")
TRACE_MSG(RRPROC_1616_112_2_17_22_53_1_3502,"RRC: traffic ba is invalid, not clear ncell")
TRACE_MSG(RRPROC_1631_112_2_17_22_53_1_3503,"RRC: clear cell %d, arfcn %d")
TRACE_MSG(RRPROC_1670_112_2_17_22_53_2_3504,"RRC:RRC_ForceCamping for sim %d")
TRACE_MSG(RRPROC_1673_112_2_17_22_53_2_3505,"RRC: = %d")
TRACE_MSG(RRPROC_1692_112_2_17_22_53_2_3506,"RRC: RRC_CancelForceCamping \n")
TRACE_MSG(RRPROC_2113_112_2_17_22_53_2_3507,"RRC: RECV si BAD")
TRACE_MSG(RRPROC_2160_112_2_17_22_53_3_3508,"RRC: FTA RRMSG_GetPacketMsgType is TRUE")
TRACE_MSG(RRPROC_2163_112_2_17_22_53_3_3509,"RRC: FTA msg type is paging")
TRACE_MSG(RRPROC_2167_112_2_17_22_53_3_3510,"RRC: FTA paging msg decode is ok")
TRACE_MSG(RRPROC_2170_112_2_17_22_53_3_3511,"RRC: FTA paging address is ok, save it")
TRACE_MSG(RRPROC_2259_112_2_17_22_53_3_3512,"%s")
TRACE_MSG(RRPROC_2267_112_2_17_22_53_3_3513,"%s")
TRACE_MSG(RRPROC_2306_112_2_17_22_53_3_3514,"RRC: %d findRcell, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d, a 0x%x p %d")
TRACE_MSG(RRPROC_2311_112_2_17_22_53_3_3515,"RRC: tc 0x%x, tc 0x%x, tc 0x%x, tc 0x%x, tc 0x%x, tc 0x%x")
TRACE_MSG(RRPROC_2379_112_2_17_22_53_3_3516,"RRC: FAB CAMP")
TRACE_MSG(RRPROC_2477_112_2_17_22_53_3_3517,"RRC: Assert failure, selected plmn list length %d is more than MAX %d.")
TRACE_MSG(RRPROC_2489_112_2_17_22_53_3_3518,"RRC %d: Selected plmn mcc:%d mnc:%d mnc_digit_num %d with no sim.")
TRACE_MSG(RRPROC_2494_112_2_17_22_53_3_3519,"RRC: Current lai not in selected plmn list with no sim, select next cell.")
TRACE_MSG(RRPROC_2615_112_2_17_22_53_3_3520,"RR_PAGING_TIMER: RR_RegisterPagingTimer timer type: %d, time out: %d")
TRACE_MSG(RRPROC_2628_112_2_17_22_53_3_3521,"RR_PAGING_TIMER:  is_si13_callback_reg %d")
TRACE_MSG(RRPROC_2634_112_2_17_22_53_3_3522,"RR_PAGING_TIMER:  active timer[%d]")
TRACE_MSG(RRPROC_2663_112_2_17_22_53_4_3523,"RR_PAGING_TIMER: RR_UnRegisterPagingTimer timer type: %d")
TRACE_MSG(RRPROC_2670_112_2_17_22_53_4_3524,"RR_PAGING_TIMER:  deactive timer[%d]")
TRACE_MSG(RRPROC_2691_112_2_17_22_53_4_3525,"RR_PAGING_TIMER: is_si13_callback_reg : %d")
TRACE_MSG(RRPROC_2713_112_2_17_22_53_4_3526,"RR_PAGING_TIMER: RR_IsPagingTimerActive timer type: %d")
TRACE_MSG(RRPROC_2722_112_2_17_22_53_4_3527,"RR_PAGING_TIMER:  timer[%d] status : %d")
TRACE_MSG(RRPROC_GPRS_113_112_2_17_22_53_4_3528,"RRC: current card num = %d\n")
TRACE_MSG(RRPROC_GPRS_114_112_2_17_22_53_4_3529,"RRC: paging_tbf_mask = %x\n")
TRACE_MSG(RRPROC_GPRS_136_112_2_17_22_53_5_3530,"RRC:bss_pag_coord = %d nmo = %d\n")
TRACE_MSG(RRPROC_GPRS_142_112_2_17_22_53_5_3531,"RRC: nv_allowed = %d  net_allowed = %d\n")
TRACE_MSG(RRPROC_GPRS_319_112_2_17_22_53_5_3532,"RRC: transfer nc_reporting_period_i = %d\n")
TRACE_MSG(RRPROC_GPRS_350_112_2_17_22_53_5_3533,"RRC: transfer nc_reporting_period_t = %d\n")
TRACE_MSG(RRPROC_GPRS_597_112_2_17_22_53_6_3534,"RR: Assert Failure, unknown page mode.")
TRACE_MSG(RR_ADAPTER_891_112_2_17_22_53_46_3535,"RRA: enter RRA_Init")
TRACE_MSG(RR_ADAPTER_1128_112_2_17_22_53_47_3536,"RRA: s_rra_gprs_id 0x%x is invalid, sig_code = 0x%x.")
TRACE_MSG(RR_ADAPTER_1164_112_2_17_22_53_47_3537,"RRA: s_rra_gprs_id 0x%x is invalid, sig_code = 0x%x.")
TRACE_MSG(RR_ADAPTER_1331_112_2_17_22_53_47_3538,"RRA: assert error, unprocessed signal 0x%x")
TRACE_MSG(RR_ADAPTER_1371_112_2_17_22_53_47_3539,"RRA: rrc already send act req.")
TRACE_MSG(RR_ADAPTER_1381_112_2_17_22_53_47_3540,"RRA: rr %d, rr_task_id = 0x%x")
TRACE_MSG(RR_ADAPTER_1432_112_2_17_22_53_47_3541,"RRA: powersweep req %d state0 = %d, state1 = %d, state2 = %d, state3 = %d")
TRACE_MSG(RR_ADAPTER_1438_112_2_17_22_53_47_3542,"RRA: powersweep req %d state0 = %d, state1 = %d, state2 = %d")
TRACE_MSG(RR_ADAPTER_1443_112_2_17_22_53_47_3543,"RRA: powersweep req %d state0 = %d, state1 = %d")
TRACE_MSG(RR_ADAPTER_1459_112_2_17_22_53_48_3544,"RRA: band change from %d to %d, must do power sweep")
TRACE_MSG(RR_ADAPTER_1473_112_2_17_22_53_48_3545,"RRA: discard powersweep req %d, wait layer1 powersweep cnf.")
TRACE_MSG(RR_ADAPTER_1518_112_2_17_22_53_48_3546,"RRA: dummy powersweep cnf state0 = %d, state1 = %d, state2 = %d, state3 = %d, last traffic id = %d")
TRACE_MSG(RR_ADAPTER_1524_112_2_17_22_53_48_3547,"RRA: dummy powersweep cnf state0 = %d, state1 = %d, state2 = %d, last traffic id = %d")
TRACE_MSG(RR_ADAPTER_1529_112_2_17_22_53_48_3548,"RRA: dummy powersweep cnf state0 = %d, state1 = %d, last traffic id = %d")
TRACE_MSG(RR_ADAPTER_1544_112_2_17_22_53_48_3549,"RRA: return dummy cnf.")
TRACE_MSG(RR_ADAPTER_1564_112_2_17_22_53_48_3550,"RRA: rr %d in find plmn flag = %d, state = %d")
TRACE_MSG(RR_ADAPTER_1588_112_2_17_22_53_48_3551,"RRA: rr %d enter RRA_RR_SEARCH.")
TRACE_MSG(RR_ADAPTER_1603_112_2_17_22_53_48_3552,"powersweepreq:master id %d")
TRACE_MSG(RR_ADAPTER_1639_112_2_17_22_53_48_3553,"RRA: po task id = %d, %d, %d, d, find plmn flag = %d, %d, %d, %d , state = %d, %d, %d, %d, master = %d")
TRACE_MSG(RR_ADAPTER_1652_112_2_17_22_53_48_3554,"RRA: po task id = %d, %d, %d, find plmn flag = %d, %d, %d , state = %d, %d, %d, master = %d")
TRACE_MSG(RR_ADAPTER_1662_112_2_17_22_53_48_3555,"RRA: po task id = %d, %d, find plmn flag = %d, %d, state = %d, %d,, master = %d")
TRACE_MSG(RR_ADAPTER_1685_112_2_17_22_53_48_3556,"RRA: layer1 return dummy powersweep cnf.")
TRACE_MSG(RR_ADAPTER_1926_112_2_17_22_53_48_3557,"RRA: recv pbcch decode req, req_rr = %d")
TRACE_MSG(RR_ADAPTER_2013_112_2_17_22_53_49_3558,"RRA: recv pbcch info ind, only send to rr %d")
TRACE_MSG(RR_ADAPTER_2074_112_2_17_22_53_49_3559,"RRA: can't do bcch camp, because there is pbcch camp")
TRACE_MSG(RR_ADAPTER_2121_112_2_17_22_53_49_3560,"RRA: clear mac rlc flag %d")
TRACE_MSG(RR_ADAPTER_2132_112_2_17_22_53_49_3561,"RRA: camp param of is same as before, discard it, req_rr = %d")
TRACE_MSG(RR_ADAPTER_2151_112_2_17_22_53_49_3562,"RRA: camp arfcn = 0x%x, req_rr = %d")
TRACE_MSG(RR_ADAPTER_2265_112_2_17_22_53_49_3563,"RRA: req_rr %d dismap %d")
TRACE_MSG(RR_ADAPTER_2275_112_2_17_22_53_49_3564,"RRA: rrc already send deact req.")
TRACE_MSG(RR_ADAPTER_2286_112_2_17_22_53_49_3565,"RRA: exist active rr, rr %d donot send deactive to layer1.")
TRACE_MSG(RR_ADAPTER_2302_112_2_17_22_53_49_3566,"RRA: deactive req_rr = %d, s_rra_master_id = %d")
TRACE_MSG(RR_ADAPTER_2345_112_2_17_22_53_49_3567,"RRA: send main campon to layer1, master rr = %d")
TRACE_MSG(RR_ADAPTER_2360_112_2_17_22_53_49_3568,"RRA: layer1 enter deactive.")
TRACE_MSG(RR_ADAPTER_2374_112_2_17_22_53_49_3569,"RRA: rr %d enter RRA_RR_NULL, master id = %d")
TRACE_MSG(RR_ADAPTER_2481_112_2_17_22_53_50_3570,"RRA: dis card rrc %d page mode %d.")
TRACE_MSG(RR_ADAPTER_2503_112_2_17_22_53_50_3571,"RRA: send MPH_BCCH_CAMP_REQ, rr = %d")
TRACE_MSG(RR_ADAPTER_2632_112_2_17_22_53_50_3572,"RRA: master %d, p5 %d")
TRACE_MSG(RR_ADAPTER_2635_112_2_17_22_53_50_3573,"RRA: mt override lau")
TRACE_MSG(RR_ADAPTER_2661_112_2_17_22_53_50_3574,"RRA: cs override ps")
TRACE_MSG(RR_ADAPTER_2693_112_2_17_22_53_50_3575,"RRA: current s_rra_override_mask = %x.")
TRACE_MSG(RR_ADAPTER_2694_112_2_17_22_53_50_3576,"RRA: rr %d is override card no need delay in rf scan protect.")
TRACE_MSG(RR_ADAPTER_2700_112_2_17_22_53_50_3577,"RRA: rr %d rach delay 1s while in rf scan protect.")
TRACE_MSG(RR_ADAPTER_2765_112_2_17_22_53_50_3578,"RRA: send 4 MPH_BCCH_CAMP_REQ, rr = %d")
TRACE_MSG(RR_ADAPTER_2781_112_2_17_22_53_50_3579,"RRA: is answer paging %d")
TRACE_MSG(RR_ADAPTER_2792_112_2_17_22_53_50_3580,"RRA: xxrr %d enter RRA_RR_TRAFFIC.")
TRACE_MSG(RR_ADAPTER_2868_112_2_17_22_53_50_3581,"RRA: rr %d enter RRA_RR_IDLE.")
TRACE_MSG(RR_ADAPTER_3051_112_2_17_22_53_51_3582,"RRA: use quadsim dsp code, card num = %d.")
TRACE_MSG(RR_ADAPTER_3055_112_2_17_22_53_51_3583,"RRA: use trisim dsp code, card num = %d.")
TRACE_MSG(RR_ADAPTER_3063_112_2_17_22_53_51_3584,"RRA: rr %d map to layer1 %d")
TRACE_MSG(RR_ADAPTER_3064_112_2_17_22_53_51_3585,"RRA: layer1 %d map to rr %d")
TRACE_MSG(RR_ADAPTER_3098_112_2_17_22_53_51_3586,"RRA: do not send %d meas report while in rf scan protect.")
TRACE_MSG(RR_ADAPTER_3252_112_2_17_22_53_51_3587,"RRA: ba combinate exceed, discard some info.")
TRACE_MSG(RR_ADAPTER_3490_112_2_17_22_53_52_3588,"RRA: act gprs = %d")
TRACE_MSG(RR_ADAPTER_3613_112_2_17_22_53_52_3589,"RRA: DeliverToRRCSig rr id = %d")
TRACE_MSG(RR_ADAPTER_3757_112_2_17_22_53_52_3590,"RRA: receive PBCCH CAMP REQ but can't do pbcch camp, because another exist")
TRACE_MSG(RR_ADAPTER_3772_112_2_17_22_53_52_3591,"RRA: enter PBCCH CAMP")
TRACE_MSG(RR_ADAPTER_3782_112_2_17_22_53_52_3592,"RRA: clear mac rlc flag %d")
TRACE_MSG(RR_ADAPTER_3788_112_2_17_22_53_52_3593,"RRA: pbcch camp arfcn = 0x%x, req_rr = %d")
TRACE_MSG(RR_ADAPTER_3899_112_2_17_22_53_53_3594,"RRA: handoverReq req_rr %d, arfcn_before_handover %d, camp_arfcn %d")
TRACE_MSG(RR_ADAPTER_3982_112_2_17_22_53_53_3595,"RRA: rach delay exp, master id = %d")
TRACE_MSG(RR_ADAPTER_4036_112_2_17_22_53_53_3596,"RRA: rr to mac, s_rra_gprs_id = 0x%x")
TRACE_MSG(RR_ADAPTER_4127_112_2_17_22_53_53_3597,"RRA: discard immed assign!")
TRACE_MSG(RR_ADAPTER_4134_112_2_17_22_53_53_3598,"RRA: discard MAC_IMMED_ASSIGN. rr = %d")
TRACE_MSG(RR_ADAPTER_4157_112_2_17_22_53_53_3599,"RRA: discard mac rach rsp. rr = %d, s_rra_gprs_id = %d")
TRACE_MSG(RR_ADAPTER_4206_112_2_17_22_53_54_3600,"RRA: rr to rlc, s_rra_gprs_id = 0x%x")
TRACE_MSG(RR_ADAPTER_4254_112_2_17_22_53_54_3601,"RRA: deact,need_trans %d, gprs_id %d, gprs_suspend_by_other_sim %d,need_gprs_resume_by_gprs_sim %d")
TRACE_MSG(RR_ADAPTER_4275_112_2_17_22_53_54_3602,"RRA: handle deact req, req_rr = %d, s_rra_master_id = %d, s_rra_gprs_id = %d")
TRACE_MSG(RR_ADAPTER_4280_112_2_17_22_53_54_3603,"RRA: rr %d state = %d")
TRACE_MSG(RR_ADAPTER_4286_112_2_17_22_53_54_3604,"RRA: suspend,need_trans %d, gprs_id %d")
TRACE_MSG(RR_ADAPTER_4302_112_2_17_22_53_54_3605,"RRA: resume,need_trans %d, gprs_id %d, gprs_suspend_by_other_sim")
TRACE_MSG(RR_ADAPTER_4333_112_2_17_22_53_54_3606,"RRA: invalid s_rra_gprs_id.")
TRACE_MSG(RR_ADAPTER_4345_112_2_17_22_53_54_3607,"RRA: use rr %d param to act rlc.")
TRACE_MSG(RR_ADAPTER_4374_112_2_17_22_53_54_3608,"RRA: SetGprsParaReq,req_rr = %d, gprs id = %d")
TRACE_MSG(RR_ADAPTER_4395_112_2_17_22_53_54_3609,"RRA: rlc act flag %d")
TRACE_MSG(RR_ADAPTER_4505_112_2_17_22_53_54_3610,"RRA: latest traffic id %d")
TRACE_MSG(RR_ADAPTER_4568_112_2_17_22_53_54_3611,"RRA: rf scan ind: rr %d in state %d")
TRACE_MSG(RR_ADAPTER_4639_112_2_17_22_53_54_3612,"RRA: copy rf scan num = %d")
TRACE_MSG(RR_ADAPTER_4672_112_2_17_22_53_55_3613,"RRA: layer1 rach expired, master id = %d")
TRACE_MSG(RR_ADAPTER_4742_112_2_17_22_53_55_3614,"RRA: discard smscb signal 0x%x")
TRACE_MSG(RR_ADAPTER_4857_112_2_17_22_53_55_3615,"RRA: start rf scan protect timer.")
TRACE_MSG(RR_ADAPTER_4880_112_2_17_22_53_55_3616,"RRA: active rrc %d map to layer1 %d")
TRACE_MSG(RR_ADAPTER_5028_112_2_17_22_53_55_3617,"RRA: mac freq %d, type = %d")
TRACE_MSG(RR_ADAPTER_5052_112_2_17_22_53_55_3618,"RRA: unknow mac freq type %d")
TRACE_MSG(RR_ADAPTER_5324_112_2_17_22_53_56_3619,"RRA: bcch camp req_rr = %d, before camp, s_rra_master_id = %d")
TRACE_MSG(RR_ADAPTER_5632_112_2_17_22_53_56_3620,"RRA: recv unexpected tc = 0.")
TRACE_MSG(RR_ADAPTER_5707_112_2_17_22_53_57_3621,"RRA: bcch list decode arfcn = 0x%x, band = %d, tc in = 0x%x, out = 0x%x")
TRACE_MSG(RR_ADAPTER_6082_112_2_17_22_53_57_3622,"RRA: unknow signal 0x%x,free.")
TRACE_MSG(RR_ADAPTER_6137_112_2_17_22_53_57_3623,"RRA: invalid arfcn index, BAND %d, arfcn %d")
TRACE_MSG(SCT_PROC_87_112_2_17_22_53_58_3624,"SCT: Close tch loop type I")
TRACE_MSG(SCT_PROC_92_112_2_17_22_53_58_3625,"SCT: cmd error C %d, B%d, A%d")
TRACE_MSG(SCT_PROC_102_112_2_17_22_53_58_3626,"SCT: Close tch loop type F")
TRACE_MSG(SCT_PROC_107_112_2_17_22_53_58_3627,"SCT loop type E")
TRACE_MSG(SCT_PROC_113_112_2_17_22_53_58_3628,"SCT: Close tch loop type D")
TRACE_MSG(SCT_PROC_120_112_2_17_22_53_58_3629,"SCT: Close tch loop type C")
TRACE_MSG(SCT_PROC_127_112_2_17_22_53_58_3630,"SCT: Close tch loop type B")
TRACE_MSG(SCT_PROC_132_112_2_17_22_53_58_3631,"SCT: Close tch loop type A")
TRACE_MSG(SCT_PROC_141_112_2_17_22_53_58_3632,"SCT: Open loop command")
TRACE_MSG(SCT_PROC_147_112_2_17_22_53_58_3633,"SCT: Test interfaec %d ")
TRACE_MSG(SCT_PROC_205_112_2_17_22_53_58_3634,"SCT: GPRS loop command")
TRACE_MSG(SCT_PROC_216_112_2_17_22_53_58_3635,"SCT:EGPRS SRB loopback command")
TRACE_MSG(SCT_PROC_250_112_2_17_22_53_58_3636,"SCT: Close tch loop ack")
TRACE_MSG(SCT_PROC_263_112_2_17_22_53_58_3637,"SCT: Open tch loop ack")
TRACE_MSG(DLR_258_112_2_17_22_54_7_3638,"DL: release req 0,but no entity")
TRACE_MSG(DLR_281_112_2_17_22_54_7_3639,"DL: release req 3,but no entity")
TRACE_MSG(DLR_424_112_2_17_22_54_7_3640,"L2: unexpected sapi\n")
TRACE_MSG(DLR_740_112_2_17_22_54_8_3641,"PH_READY_TO_SEND_IND recved, but task released\n")
TRACE_MSG(LAYER1_527_112_2_17_22_54_12_3642,"deactive req.\n")
TRACE_MSG(LAYER1_592_112_2_17_22_54_13_3643,"rxpower_sweep_req asked and confirm replied.\n")
TRACE_MSG(LAYER1_610_112_2_17_22_54_13_3644,"rxpower from BS come.\n")
TRACE_MSG(LAYER1_628_112_2_17_22_54_13_3645,"bcch req asked.\n")
TRACE_MSG(LAYER1_639_112_2_17_22_54_13_3646,"the bcch req is single %d.\n")
TRACE_MSG(LAYER1_656_112_2_17_22_54_13_3647,"bcch req stop.\n")
TRACE_MSG(LAYER1_672_112_2_17_22_54_13_3648,"bsic req asked.\n")
TRACE_MSG(LAYER1_748_112_2_17_22_54_13_3649,"\n*****Layer1 Rx BCCH:*****\n")
TRACE_MSG(LAYER1_749_112_2_17_22_54_13_3650,"BCCH_ARFCN:       %d \n\n")
TRACE_MSG(LAYER1_790_112_2_17_22_54_13_3651,"\n*****L1Sim Rx BCCH:*****\n")
TRACE_MSG(LAYER1_791_112_2_17_22_54_13_3652,"BCCH_ARFCN:%d \n\n")
TRACE_MSG(LAYER1_834_112_2_17_22_54_13_3653,"bcch ind true\n")
TRACE_MSG(LAYER1_995_112_2_17_22_54_13_3654,"campon req&confirmed.\n")
TRACE_MSG(LAYER1_1070_112_2_17_22_54_14_3655,"TBF Downlink Assignment Recv\n")
TRACE_MSG(LAYER1_1143_112_2_17_22_54_14_3656,"Send the first RA\n")
TRACE_MSG(LAYER1_1190_112_2_17_22_54_14_3657,"AGCH recv\n")
TRACE_MSG(LAYER1_1322_112_2_17_22_54_14_3658,"PLMN search asked and  rxpower confirm replied.\n")
TRACE_MSG(LAYER1_1666_112_2_17_22_54_15_3659,"chan_assign:%d\n")
TRACE_MSG(LAYER1_1708_112_2_17_22_54_15_3660,"chan_assign_fail:%d\n")
TRACE_MSG(LAYER1_1791_112_2_17_22_54_15_3661,"handover_fail:%d\n")
TRACE_MSG(LAYER1_1905_112_2_17_22_54_15_3662,"Received Data block from new DCCH in non-syn handover\n")
TRACE_MSG(LAYER1_1914_112_2_17_22_54_15_3663,"handover :%d\n")
TRACE_MSG(LAYER1_1979_112_2_17_22_54_15_3664,"handover_fail:%d\n")
TRACE_MSG(LAYER1_2015_112_2_17_22_54_15_3665,"L1:sacch data recv\n")
TRACE_MSG(LAYER1_2033_112_2_17_22_54_15_3666,"l1 recv data:%d\n")
TRACE_MSG(LAYER1_2244_112_2_17_22_54_16_3667,"This bcch block is unexpected\n")
TRACE_MSG(LAYER1_2282_112_2_17_22_54_16_3668,"bcch ind,true\n")
TRACE_MSG(LAYER1_2289_112_2_17_22_54_16_3669,"bcch ind,not requested\n")
TRACE_MSG(LAYER1_2402_112_2_17_22_54_16_3670,"SI_ID: 1")
TRACE_MSG(LAYER1_2406_112_2_17_22_54_16_3671,"SI_ID: 2")
TRACE_MSG(LAYER1_2410_112_2_17_22_54_16_3672,"SI_ID: 2Bis")
TRACE_MSG(LAYER1_2414_112_2_17_22_54_16_3673,"SI_ID: 2Ter")
TRACE_MSG(LAYER1_2418_112_2_17_22_54_16_3674,"SI_ID: 3")
TRACE_MSG(LAYER1_2422_112_2_17_22_54_16_3675,"SI_ID: 4")
TRACE_MSG(LAYER1_2426_112_2_17_22_54_16_3676,"SI_ID: 13")
TRACE_MSG(LAYER1_2430_112_2_17_22_54_16_3677,"SI_ID: 5BIS")
TRACE_MSG(LAYER1_2434_112_2_17_22_54_16_3678,"SI_ID: 5BIS")
TRACE_MSG(LAYER1_2438_112_2_17_22_54_16_3679,"SI_ID: 6")
TRACE_MSG(LAYER1_2442_112_2_17_22_54_16_3680,"SI_ID: 7")
TRACE_MSG(LAYER1_2446_112_2_17_22_54_16_3681,"PSI_ID: 13")
TRACE_MSG(LAYER1_2450_112_2_17_22_54_16_3682,"SI_ID: 4")
TRACE_MSG(LAYER1_2454_112_2_17_22_54_16_3683,"PSI_ID: 3Bis")
TRACE_MSG(LAYER1_2458_112_2_17_22_54_16_3684,"SI_ID: 8")
TRACE_MSG(LAYER1_2462_112_2_17_22_54_16_3685,"SI_ID: 9")
TRACE_MSG(LAYER1_2466_112_2_17_22_54_16_3686,"SI_ID: 13")
TRACE_MSG(LAYER1_2470_112_2_17_22_54_16_3687,"PSI_ID: 1")
TRACE_MSG(LAYER1_2474_112_2_17_22_54_16_3688,"PSI_ID: 2")
TRACE_MSG(LAYER1_2478_112_2_17_22_54_16_3689,"PSI_ID: 3 ")
TRACE_MSG(MAC_469_112_2_17_22_54_17_3690,"MAC:try to start t3192 0 ms")
TRACE_MSG(MAC_924_112_2_17_22_54_18_3691,"MAC: Decode grr freq req error!\n")
TRACE_MSG(MAC_961_112_2_17_22_54_18_3692,"MAC: DL imm assi,freq Acquare Sys Info error")
TRACE_MSG(MAC_1113_112_2_17_22_54_19_3693,"MAC: EGPRS imm DL assi,but MS don't support EGPRS! \n")
TRACE_MSG(MAC_1138_112_2_17_22_54_19_3694,"MAC: DL imm assi,Decode ia rest octets error!")
TRACE_MSG(MAC_1145_112_2_17_22_54_19_3695,"MAC: DL imm assi,Decode chan descri error!")
TRACE_MSG(MAC_1319_112_2_17_22_54_19_3696,"MAC: No tfi assign,ignore the packet DL assi from ppch!")
TRACE_MSG(MAC_1402_112_2_17_22_54_19_3697,"MAC: Decode freq para error! Acquare Sys Info")
TRACE_MSG(MAC_1427_112_2_17_22_54_19_3698,"MAC: Multislot capability not match! Ignore pdch dl req!")
TRACE_MSG(MAC_1434_112_2_17_22_54_19_3699,"MAC: Decode packet DL error in idle, ignore the msg!")
TRACE_MSG(MAC_1616_112_2_17_22_54_20_3700,"MAC: Decode rach response error! Acquare Sys Info")
TRACE_MSG(MAC_1638_112_2_17_22_54_20_3701,"MAC: One phase received egprs ul assi£¬but we don't support EDGE!")
TRACE_MSG(MAC_1650_112_2_17_22_54_20_3702,"MAC: One phase received Dl single assi!\n")
TRACE_MSG(MAC_1963_112_2_17_22_54_20_3703,"MAC: Single block without TBF!")
TRACE_MSG(MAC_2105_112_2_17_22_54_21_3704,"MAC: One phase received Dl assi!")
TRACE_MSG(MAC_2112_112_2_17_22_54_21_3705,"MAC: One phase received egprs dl assi£¬but we don't support EDGE!")
TRACE_MSG(MAC_2120_112_2_17_22_54_21_3706,"MAC: Decode rach response error! Rach retry!")
TRACE_MSG(MAC_2161_112_2_17_22_54_21_3707,"MAC: wrong condition receive rach rsp")
TRACE_MSG(MAC_2212_112_2_17_22_54_21_3708,"MAC: Rach response reject, ul tbf failure!\n")
TRACE_MSG(MAC_2229_112_2_17_22_54_21_3709,"MAC: Rach response reject, Rach retry!")
TRACE_MSG(MAC_2373_112_2_17_22_54_21_3710,"MAC: not support GRR_MAC_PDCH_ASSIGN_REQ")
TRACE_MSG(MAC_2501_112_2_17_22_54_21_3711,"MAC: access abort invalid")
TRACE_MSG(MAC_2551_112_2_17_22_54_21_3712,"MAC: access abort invalid")
TRACE_MSG(MAC_2777_112_2_17_22_54_22_3713,"MAC: tbf mode invalid in connect cnf %d")
TRACE_MSG(MAC_2842_112_2_17_22_54_22_3714,"MAC: Ul release, send ctrl ack request new TBF!")
TRACE_MSG(MAC_2853_112_2_17_22_54_22_3715,"MAC: Ul release, send res req, request new TBF!")
TRACE_MSG(MAC_2858_112_2_17_22_54_22_3716,"MAC: Ul release, unexpected ul_exp_action!!")
TRACE_MSG(MAC_2943_112_2_17_22_54_22_3717,"MAC: receive ul_rel_req but ul not active")
TRACE_MSG(MAC_3035_112_2_17_22_54_22_3718,"MAC:tbf mode invalid in rlc_mac_rel_req")
TRACE_MSG(MAC_3043_112_2_17_22_54_22_3719,"MAC:Assert Failure,tbf mode invalid in rlc_mac_rel_req")
TRACE_MSG(MAC_3147_112_2_17_22_54_23_3720,"MAC:Assert Failure,release both link tbf but not in rel state")
TRACE_MSG(MAC_3173_112_2_17_22_54_23_3721,"MAC:tbf mode invalid,uplink tbf starting time?")
TRACE_MSG(MAC_3297_112_2_17_22_54_23_3722,"MAC:rlc_mac_tbf_req in ul_active!!")
TRACE_MSG(MAC_3306_112_2_17_22_54_23_3723,"MAC:ignore rlc tbf req when prach have been send but not receive ass!")
TRACE_MSG(MAC_3443_112_2_17_22_54_23_3724,"MAC:rlc realloc req but ul not active")
TRACE_MSG(MAC_3549_112_2_17_22_54_23_3725,"MAC: wrong state 3186 expire")
TRACE_MSG(MAC_3560_112_2_17_22_54_23_3726,"MAC: T3170 expiry! T3170 Dur: %d \n")
TRACE_MSG(MAC_3657_112_2_17_22_54_24_3727,"MAC: wrong state t3170 expire")
TRACE_MSG(MAC_3723_112_2_17_22_54_24_3728,"MAC: T3168 expiry! T3168 Dur: %d\n")
TRACE_MSG(MAC_3857_112_2_17_22_54_24_3729,"MAC: wrong state rev t3168 expire")
TRACE_MSG(MAC_3974_112_2_17_22_54_24_3730,"MAC: wrong state rev t3164 expire")
TRACE_MSG(MAC_4052_112_2_17_22_54_24_3731,"MAC: wrong state rev t3162 expire")
TRACE_MSG(MAC_4069_112_2_17_22_54_24_3732,"MAC: wrong state rev t3180 expire")
TRACE_MSG(MAC_4091_112_2_17_22_54_25_3733,"MAC: T3192 expiry! T3192 Dur: %d\n")
TRACE_MSG(MAC_4127_112_2_17_22_54_25_3734,"MAC: wrong dl state, t3192 exp\n")
TRACE_MSG(MAC_4263_112_2_17_22_54_25_3735,"MAC: Prach access not permitted!")
TRACE_MSG(MAC_4378_112_2_17_22_54_25_3736,"MAC: Decode rach response error! False TBF Mode or MCS\n")
TRACE_MSG(MAC_4641_112_2_17_22_54_26_3737,"MAC: Mac mode mismatch! Ignore packet ul assi!")
TRACE_MSG(MAC_4666_112_2_17_22_54_26_3738,"MAC: rev ul assi in ul idle or other invalid state")
TRACE_MSG(MAC_4744_112_2_17_22_54_26_3739,"MAC: one_phase_access_error!!")
TRACE_MSG(MAC_4864_112_2_17_22_54_26_3740,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_4870_112_2_17_22_54_26_3741,"MAC: Frequency not in band! Rach retry!\n")
TRACE_MSG(MAC_4876_112_2_17_22_54_26_3742,"MAC: decode freq error return invalid value")
TRACE_MSG(MAC_4920_112_2_17_22_54_26_3743,"MAC: Frequency not in band! Rach retry!\n")
TRACE_MSG(MAC_4932_112_2_17_22_54_26_3744,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_4995_112_2_17_22_54_26_3745,"MAC: decode freq error return invalid value")
TRACE_MSG(MAC_5011_112_2_17_22_54_26_3746,"MAC: DL est when waiting L1 release cnf")
TRACE_MSG(MAC_5034_112_2_17_22_54_26_3747,"MAC:Ignore packet dl assi when two phase or contention phase!")
TRACE_MSG(MAC_5044_112_2_17_22_54_26_3748,"MAC: rev dl assi when ul access two phase or other state")
TRACE_MSG(MAC_5271_112_2_17_22_54_27_3749,"MAC: No tfi assign in dl assi! Rach retry!\n")
TRACE_MSG(MAC_5333_112_2_17_22_54_27_3750,"MAC: wrong state when rev dl assi")
TRACE_MSG(MAC_5342_112_2_17_22_54_27_3751,"MAC: Decode freq error! Rach retry!")
TRACE_MSG(MAC_5354_112_2_17_22_54_27_3752,"MAC: Multislot capability not match! Rach retry!")
TRACE_MSG(MAC_5377_112_2_17_22_54_27_3753,"MAC: Multislot capability not match! Ignore the msg!")
TRACE_MSG(MAC_5384_112_2_17_22_54_27_3754,"MAC: Rlc mode mismatch! Ignore dl ass msg in dl_active!")
TRACE_MSG(MAC_5444_112_2_17_22_54_27_3755,"MAC:contention phase, ignore TS RECONFIG")
TRACE_MSG(MAC_5462_112_2_17_22_54_27_3756,"MAC: error state to receive ts reconfig")
TRACE_MSG(MAC_5496_112_2_17_22_54_27_3757,"MAC: in ul active recv pkt ts reconfig allocation no dl slot! Rach retry!\n")
TRACE_MSG(MAC_5556_112_2_17_22_54_27_3758,"MAC: Decode error! Acquare Sys Info\n")
TRACE_MSG(MAC_5561_112_2_17_22_54_27_3759,"MAC: Decode freq error! UL release and Rach retry!\n")
TRACE_MSG(MAC_5581_112_2_17_22_54_28_3760,"MAC: Decode allocation error! UL release and Rach retry!\n")
TRACE_MSG(MAC_5719_112_2_17_22_54_28_3761,"MAC: Timeslot reconfig not include dl tfi! Ul release and Rach retry!\n")
TRACE_MSG(MAC_5746_112_2_17_22_54_28_3762,"MAC: in dl active recv pkt ts reconfig allocation no dl slot! Rach retry!\n")
TRACE_MSG(MAC_5841_112_2_17_22_54_28_3763,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_5846_112_2_17_22_54_28_3764,"MAC: wrong state when rev ts realloc assi")
TRACE_MSG(MAC_5852_112_2_17_22_54_28_3765,"MAC: Decode freq error! Rach retry!\n")
TRACE_MSG(MAC_5864_112_2_17_22_54_28_3766,"MAC: Timeslot reconfig not include dl tfi! Rach retry!\n")
TRACE_MSG(MAC_5874_112_2_17_22_54_28_3767,"MAC: Rlc mode mismatch! Ignore ts reconfig msg in dl_active!")
TRACE_MSG(MAC_5954_112_2_17_22_54_28_3768,"MAC: in ul dl active recv pkt ts reconfig allocation no dl slot! Rach retry!\n")
TRACE_MSG(MAC_6003_112_2_17_22_54_28_3769,"MAC: Decode error! Acquare Sys Info\n")
TRACE_MSG(MAC_6008_112_2_17_22_54_28_3770,"MAC: Decode freq error! UL release and Rach retry!\n")
TRACE_MSG(MAC_6185_112_2_17_22_54_29_3771,"MAC: Decode allocation error! Rach retry!")
TRACE_MSG(MAC_6198_112_2_17_22_54_29_3772,"MAC: Rlc mode mismatch! Ignore ts reconfig msg in ul_dl_active!")
TRACE_MSG(MAC_6213_112_2_17_22_54_29_3773,"MAC: Ignore additional PACKET ACCESS REJECT messages!")
TRACE_MSG(MAC_6515_112_2_17_22_54_29_3774,"MAC: Receive reject, UL release!\n")
TRACE_MSG(MAC_6569_112_2_17_22_54_29_3775,"MAC: ignore PACKET_QUEUING_NOTIFICATION while waiting TBF Starting Time of ul ass!")
TRACE_MSG(MAC_6780_112_2_17_22_54_30_3776,"MAC: wrong ul state recv Packet tbf realse msg!")
TRACE_MSG(MAC_6912_112_2_17_22_54_30_3777,"MAC: wrong PPR")
TRACE_MSG(MAC_7312_112_2_17_22_54_31_3778,"MAC: wrong state rev t3166 expire")
TRACE_MSG(MAC_7634_112_2_17_22_54_32_3779,"MAC: Abnormal release with retry!")
TRACE_MSG(MAC_7658_112_2_17_22_54_32_3780,"MAC: !!Impossible ul_release_state!!")
TRACE_MSG(MAC_7698_112_2_17_22_54_32_3781,"MAC: Abnormal release with rach retry!")
TRACE_MSG(MAC_7710_112_2_17_22_54_32_3782,"MAC: release_cnf for ul but ul not active,maybe hold for connect cnf")
TRACE_MSG(MAC_7779_112_2_17_22_54_32_3783,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_7791_112_2_17_22_54_32_3784,"MAC: Frequency not in band! Rach retry!\n")
TRACE_MSG(MAC_7797_112_2_17_22_54_32_3785,"MAC: decode freq error,invalid return value")
TRACE_MSG(MAC_7925_112_2_17_22_54_32_3786,"MAC: Single block without TBF!\n")
TRACE_MSG(MAC_8063_112_2_17_22_54_32_3787,"MAC: invalid one phase state when rev single block assi")
TRACE_MSG(MAC_8200_112_2_17_22_54_33_3788,"MAC: invalid one phase state when rev ul assi")
TRACE_MSG(MAC_8274_112_2_17_22_54_33_3789,"MAC: Decode allocation error! Rach retry!\n")
TRACE_MSG(MAC_8281_112_2_17_22_54_33_3790,"MAC: Ingore the ul assignment msg")
TRACE_MSG(MAC_8293_112_2_17_22_54_33_3791,"MAC: wrong state when rev single block assi")
TRACE_MSG(MAC_8453_112_2_17_22_54_33_3792,"MAC: Decode allocation error! Rach retry!")
TRACE_MSG(MAC_8460_112_2_17_22_54_33_3793,"MAC: Ingore the ul assi S")
TRACE_MSG(MAC_8579_112_2_17_22_54_33_3794,"MAC: Decode allocation error! Rach retry!\n")
TRACE_MSG(MAC_8586_112_2_17_22_54_33_3795,"MAC: Ingore the ul assi msg!")
TRACE_MSG(MAC_8596_112_2_17_22_54_33_3796,"MAC: rev single block assi when ul active")
TRACE_MSG(MAC_8609_112_2_17_22_54_33_3797,"MAC: Decode freq error! Acquare Sys Info")
TRACE_MSG(MAC_8680_112_2_17_22_54_34_3798,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_8685_112_2_17_22_54_34_3799,"MAC: Decode freq error! Ignore the msg!")
TRACE_MSG(MAC_8823_112_2_17_22_54_34_3800,"MAC: Dl assi not include tfi!")
TRACE_MSG(MAC_8829_112_2_17_22_54_34_3801,"MAC: Multislot capability not match! Ignore the msg!\n")
TRACE_MSG(MAC_8949_112_2_17_22_54_34_3802,"MAC: Decode freq error! Acquare Sys Info\n")
TRACE_MSG(MAC_9034_112_2_17_22_54_34_3803,"MAC: Ignore dl ass while T3192 run!ctrl_ack=%d")
TRACE_MSG(MAC_9152_112_2_17_22_54_35_3804,"MAC: Decode error! acquare si!\n")
TRACE_MSG(MAC_9204_112_2_17_22_54_35_3805,"MAC: Decode allocation error! Rach retry!")
TRACE_MSG(MAC_9439_112_2_17_22_54_35_3806,"MAC: wrong state rev ul ack")
TRACE_MSG(MAC_9556_112_2_17_22_54_35_3807,"MAC: Contention failure!")
TRACE_MSG(RLC_703_112_2_17_22_54_37_3808,"rlc:remain llc tbf request!")
TRACE_MSG(RLC_720_112_2_17_22_54_38_3809,"rlc:cell reselect tbf!")
TRACE_MSG(RLC_939_112_2_17_22_54_38_3810,"rlc:Up Single Block Access!\n")
TRACE_MSG(RLC_1017_112_2_17_22_54_38_3811,"RLC: SCT est fail, downlink tbf is not acitved!")
TRACE_MSG(RLC_1816_112_2_17_22_54_40_3812,"RLC: recv data error in loop back mode")
TRACE_MSG(RLC_1851_112_2_17_22_54_40_3813,"rlc:error data_ind when dl is not actived!")
TRACE_MSG(RLC_1867_112_2_17_22_54_40_3814,"rlc: error, suspend or null state recv data block!")
TRACE_MSG(RLC_1940_112_2_17_22_54_40_3815,"rlc:SCT end!\n")
TRACE_MSG(RLC_2129_112_2_17_22_54_40_3816,"rlc:should not recv up release")
TRACE_MSG(RLC_2355_112_2_17_22_54_41_3817,"rlc:Access Fail with CS Page!\n")
TRACE_MSG(RLC_2795_112_2_17_22_54_42_3818,"rlc: suspend state recv tbf ind!\n")
TRACE_MSG(RLC_2801_112_2_17_22_54_42_3819,"rlc:Up tbf fail with dl tbf is esting!\n")
TRACE_MSG(RLC_2805_112_2_17_22_54_42_3820,"RLC: est sigle block without tbf interrupt by dl tbf ass")
TRACE_MSG(RLC_2848_112_2_17_22_54_42_3821,"rlc:Up tbf ok for single block!\n")
TRACE_MSG(RLC_2875_112_2_17_22_54_42_3822,"rlc:IS Rejected Access!\n")
TRACE_MSG(RLC_3037_112_2_17_22_54_42_3823,"rlc:mac retry,but no need!\n")
TRACE_MSG(RLC_3071_112_2_17_22_54_42_3824,"rlc:Up tbf fail with t3172 in mac ul_active!\n")
TRACE_MSG(RLC_3149_112_2_17_22_54_43_3825,"rlc:error normal release")
TRACE_MSG(RLC_3338_112_2_17_22_54_43_3826,"rlc:Up fail and perform cell reselect!\n")
TRACE_MSG(RLC_3406_112_2_17_22_54_43_3827,"rlc:Up tbf fail with t3172 in mac ul_active!\n")
TRACE_MSG(RLC_3487_112_2_17_22_54_43_3828,"rlc:Up tbf fail with timer t3172!\n")
TRACE_MSG(RLC_3553_112_2_17_22_54_44_3829,"rlc:Receive Second mac_rlc_tbf_cnf msg!")
TRACE_MSG(RLC_3568_112_2_17_22_54_44_3830,"rlc:Acess state is not one or two phase!")
TRACE_MSG(RLC_3623_112_2_17_22_54_44_3831,"RLC:SCT Test Mode B Ested!")
TRACE_MSG(RLC_3630_112_2_17_22_54_44_3832,"RLC:SCT Test Mode A Ested!")
TRACE_MSG(RRC_3758_112_2_17_22_54_52_3833,"RRC: unknown bsic cnf")
TRACE_MSG(RRC_3825_112_2_17_22_54_53_3834,"RRC: ASSERT, research>6")
TRACE_MSG(RRC_3844_112_2_17_22_54_53_3835,"RRC: unknown bsic fail")
TRACE_MSG(RRC_4575_112_2_17_22_54_54_3836,"RRC: Plmn search is interrupted, while candidate_cell_exist")
TRACE_MSG(RRC_6803_112_2_17_22_54_59_3837,"RRC: discard IA,because mm est saved")
TRACE_MSG(RRC_6811_112_2_17_22_54_59_3838,"RRC: discard IA,because plmn list req")
TRACE_MSG(RRC_8042_112_2_17_22_55_1_3839,"RRC: proc new bsic of scell.")
TRACE_MSG(RRC_8252_112_2_17_22_55_2_3840,"RRC: proc new bsic of scell.")
TRACE_MSG(RRC_9604_112_2_17_22_55_4_3841,"RR:receive_psi_resel Get Packet paging!!\n")
TRACE_MSG(RRC_10009_112_2_17_22_55_5_3842,"RRC: assert error, not fastplmn")
TRACE_MSG(RRC_10148_112_2_17_22_55_6_3843,"RRC: assert error, not fastplmn")
TRACE_MSG(RRC_10626_112_2_17_22_55_7_3844,"RRC: unknown bsic cnf")
TRACE_MSG(RRC_10705_112_2_17_22_55_7_3845,"RRC: unknown bsic fail")
TRACE_MSG(RRC_11250_112_2_17_22_55_8_3846,"RR: discard immed_assig")
TRACE_MSG(RRC_11283_112_2_17_22_55_8_3847,"RR: rx immediate rej")
TRACE_MSG(RRC_12172_112_2_17_22_55_10_3848,"RRC wrong sapi %d")
TRACE_MSG(RRC_12953_112_2_17_22_55_11_3849,"RRC: wait assign dcch est error in dl chan type")
TRACE_MSG(RRC_13687_112_2_17_22_55_13_3850,"RRC wrong sapi %d")
TRACE_MSG(RRC_16762_112_2_17_22_55_19_3851,"RRC: Receive data block on PPCH, discard it!")
TRACE_MSG(RRC_17504_112_2_17_22_55_20_3852,"RRC: Decode Pmo err!")
TRACE_MSG(RRC_18268_112_2_17_22_55_22_3853,"RRC: ASSERT ERROR recv prach when bad")
TRACE_MSG(RRC_21388_112_2_17_22_55_28_3854,"RRC: discard IA,because plmn list req")
TRACE_MSG(RRC_21605_112_2_17_22_55_29_3855,"RRC: Receive data block on PPCH, discard it!")
TRACE_MSG(RRC_21767_112_2_17_22_55_29_3856,"RRC: Decode pmo err!")
TRACE_MSG(RRC_21845_112_2_17_22_55_29_3857,"RRC: find ncell fail")
TRACE_MSG(RRC_21857_112_2_17_22_55_29_3858,"RRC: gmm state not ready")
TRACE_MSG(RRC_21865_112_2_17_22_55_29_3859,"RRC: decode pcco fail")
TRACE_MSG(RRC_23559_112_2_17_22_55_32_3860,"RRC: process saved ppch")
TRACE_MSG(RRC_23648_112_2_17_22_55_33_3861,"RRC: proc saved pch ")
TRACE_MSG(RRC_24338_112_2_17_22_55_34_3862,"RRC: resel scell arfcn 0x%x")
TRACE_MSG(RRC_24570_112_2_17_22_55_34_3863,"RRC: cell resel arfcn 0x%x")
TRACE_MSG(RRC_24822_112_2_17_22_55_35_3864," RRC: switch to another cell, clear PSI Msg Buffer \n")
TRACE_MSG(RRC_25261_112_2_17_22_55_36_3865,"RRC: unknown bsic cnf")
TRACE_MSG(RRC_26078_112_2_17_22_55_37_3866,"RRC: temp = %d")
TRACE_MSG(RRC_27704_112_2_17_22_55_41_3867,"RRC: Decode Pmo err!")
TRACE_MSG(RRC_27774_112_2_17_22_55_41_3868,"RRC: Receive data block on PPCH, discard it!")
TRACE_MSG(DLR_258_112_2_17_22_55_55_3869,"DL: release req 0,but no entity")
TRACE_MSG(DLR_281_112_2_17_22_55_55_3870,"DL: release req 3,but no entity")
TRACE_MSG(DLR_424_112_2_17_22_55_55_3871,"L2: unexpected sapi\n")
TRACE_MSG(DLR_740_112_2_17_22_55_56_3872,"PH_READY_TO_SEND_IND recved, but task released\n")
TRACE_MSG(LAYER1_527_112_2_17_22_55_59_3873,"deactive req.\n")
TRACE_MSG(LAYER1_592_112_2_17_22_56_0_3874,"rxpower_sweep_req asked and confirm replied.\n")
TRACE_MSG(LAYER1_610_112_2_17_22_56_0_3875,"rxpower from BS come.\n")
TRACE_MSG(LAYER1_628_112_2_17_22_56_0_3876,"bcch req asked.\n")
TRACE_MSG(LAYER1_639_112_2_17_22_56_0_3877,"the bcch req is single %d.\n")
TRACE_MSG(LAYER1_656_112_2_17_22_56_0_3878,"bcch req stop.\n")
TRACE_MSG(LAYER1_672_112_2_17_22_56_0_3879,"bsic req asked.\n")
TRACE_MSG(LAYER1_748_112_2_17_22_56_0_3880,"\n*****Layer1 Rx BCCH:*****\n")
TRACE_MSG(LAYER1_749_112_2_17_22_56_0_3881,"BCCH_ARFCN:       %d \n\n")
TRACE_MSG(LAYER1_790_112_2_17_22_56_0_3882,"\n*****L1Sim Rx BCCH:*****\n")
TRACE_MSG(LAYER1_791_112_2_17_22_56_0_3883,"BCCH_ARFCN:%d \n\n")
TRACE_MSG(LAYER1_834_112_2_17_22_56_0_3884,"bcch ind true\n")
TRACE_MSG(LAYER1_995_112_2_17_22_56_0_3885,"campon req&confirmed.\n")
TRACE_MSG(LAYER1_1070_112_2_17_22_56_1_3886,"TBF Downlink Assignment Recv\n")
TRACE_MSG(LAYER1_1143_112_2_17_22_56_1_3887,"Send the first RA\n")
TRACE_MSG(LAYER1_1190_112_2_17_22_56_1_3888,"AGCH recv\n")
TRACE_MSG(LAYER1_1322_112_2_17_22_56_1_3889,"PLMN search asked and  rxpower confirm replied.\n")
TRACE_MSG(LAYER1_1486_112_2_17_22_56_1_3890,"chan_assign:%d\n")
TRACE_MSG(LAYER1_1528_112_2_17_22_56_1_3891,"chan_assign_fail:%d\n")
TRACE_MSG(LAYER1_1611_112_2_17_22_56_2_3892,"handover_fail:%d\n")
TRACE_MSG(LAYER1_1725_112_2_17_22_56_2_3893,"Received Data block from new DCCH in non-syn handover\n")
TRACE_MSG(LAYER1_1734_112_2_17_22_56_2_3894,"handover :%d\n")
TRACE_MSG(LAYER1_1799_112_2_17_22_56_2_3895,"handover_fail:%d\n")
TRACE_MSG(LAYER1_1835_112_2_17_22_56_2_3896,"L1:sacch data recv\n")
TRACE_MSG(LAYER1_1853_112_2_17_22_56_2_3897,"l1 recv data:%d\n")
TRACE_MSG(LAYER1_2064_112_2_17_22_56_2_3898,"This bcch block is unexpected\n")
TRACE_MSG(LAYER1_2102_112_2_17_22_56_3_3899,"bcch ind,true\n")
TRACE_MSG(LAYER1_2109_112_2_17_22_56_3_3900,"bcch ind,not requested\n")
TRACE_MSG(LAYER1_2222_112_2_17_22_56_3_3901,"SI_ID: 1")
TRACE_MSG(LAYER1_2226_112_2_17_22_56_3_3902,"SI_ID: 2")
TRACE_MSG(LAYER1_2230_112_2_17_22_56_3_3903,"SI_ID: 2Bis")
TRACE_MSG(LAYER1_2234_112_2_17_22_56_3_3904,"SI_ID: 2Ter")
TRACE_MSG(LAYER1_2238_112_2_17_22_56_3_3905,"SI_ID: 3")
TRACE_MSG(LAYER1_2242_112_2_17_22_56_3_3906,"SI_ID: 4")
TRACE_MSG(LAYER1_2246_112_2_17_22_56_3_3907,"SI_ID: 13")
TRACE_MSG(LAYER1_2250_112_2_17_22_56_3_3908,"SI_ID: 5BIS")
TRACE_MSG(LAYER1_2254_112_2_17_22_56_3_3909,"SI_ID: 5BIS")
TRACE_MSG(LAYER1_2258_112_2_17_22_56_3_3910,"SI_ID: 6")
TRACE_MSG(LAYER1_2262_112_2_17_22_56_3_3911,"SI_ID: 7")
TRACE_MSG(LAYER1_2266_112_2_17_22_56_3_3912,"PSI_ID: 13")
TRACE_MSG(LAYER1_2270_112_2_17_22_56_3_3913,"SI_ID: 4")
TRACE_MSG(LAYER1_2274_112_2_17_22_56_3_3914,"PSI_ID: 3Bis")
TRACE_MSG(LAYER1_2278_112_2_17_22_56_3_3915,"SI_ID: 8")
TRACE_MSG(LAYER1_2282_112_2_17_22_56_3_3916,"SI_ID: 9")
TRACE_MSG(LAYER1_2286_112_2_17_22_56_3_3917,"SI_ID: 13")
TRACE_MSG(LAYER1_2290_112_2_17_22_56_3_3918,"PSI_ID: 1")
TRACE_MSG(LAYER1_2294_112_2_17_22_56_3_3919,"PSI_ID: 2")
TRACE_MSG(LAYER1_2298_112_2_17_22_56_3_3920,"PSI_ID: 3 ")
TRACE_MSG(RRC_2489_112_2_17_22_56_8_3921,"RRC: unknown bsic cnf")
TRACE_MSG(RRC_2553_112_2_17_22_56_8_3922,"RRC: ASSERT, research>6")
TRACE_MSG(RRC_2572_112_2_17_22_56_8_3923,"RRC: unknown bsic fail")
TRACE_MSG(RRC_3262_112_2_17_22_56_10_3924,"RRC: Plmn search is interrupted, while candidate_cell_exist")
TRACE_MSG(RRC_4834_112_2_17_22_56_13_3925,"RRC: proc new bsic of scell.")
TRACE_MSG(RRC_5383_112_2_17_22_56_14_3926,"RRC: assert error, not fastplmn")
TRACE_MSG(RRC_5513_112_2_17_22_56_14_3927,"RRC: assert error, not fastplmn")
TRACE_MSG(RRC_5967_112_2_17_22_56_15_3928,"RRC: unknown bsic cnf")
TRACE_MSG(RRC_6043_112_2_17_22_56_16_3929,"RRC: unknown bsic fail")
TRACE_MSG(RRC_6434_112_2_17_22_56_16_3930,"RR: discard immed_assig")
TRACE_MSG(RRC_6467_112_2_17_22_56_16_3931,"RR: rx immediate rej")
TRACE_MSG(RRC_7180_112_2_17_22_56_18_3932,"RRC wrong sapi %d")
TRACE_MSG(RRC_7934_112_2_17_22_56_19_3933,"RRC: wait assign dcch est error in dl chan type")
TRACE_MSG(RRC_8615_112_2_17_22_56_21_3934,"RRC wrong sapi %d")
TRACE_MSG(RRC_11787_112_2_17_22_56_27_3935,"RRC: proc saved pch ")
TRACE_MSG(RRC_12325_112_2_17_22_56_28_3936,"RRC: resel scell arfcn 0x%x")
TRACE_MSG(RRC_12444_112_2_17_22_56_28_3937,"RRC: cell resel arfcn 0x%x")
TRACE_MSG(RRC_12823_112_2_17_22_56_29_3938,"RRC: unknown bsic cnf")
TRACE_MSG(RRC_13614_112_2_17_22_56_31_3939,"RR temp = %d")
TRACE_MSG(SM_COMMON_241_112_2_17_22_56_57_3940,"SMCOMMON:delete the activation pdp context request(nsapi %d)")
TRACE_MSG(SM_COMMON_271_112_2_17_22_56_57_3941,"SMCOMMON: handle nsapi %d")
TRACE_MSG(SM_DECODE_94_112_2_17_22_56_57_3942,"SM:decode Transaction identifier, TIE reserved!")
TRACE_MSG(SM_DECODE_121_112_2_17_22_56_58_3943,"SM:decode Transaction identifier, EXT is '0'!")
TRACE_MSG(SM_DECODE_249_112_2_17_22_56_58_3944,"SM:PDP address length too long!")
TRACE_MSG(SM_DECODE_272_112_2_17_22_56_58_3945,"SM:decode pdp address,PPP,PDP type reserved!")
TRACE_MSG(SM_DECODE_300_112_2_17_22_56_58_3946,"SM:decode pdp address,PDP org type reserved!")
TRACE_MSG(SM_DECODE_382_112_2_17_22_56_58_3947,"SM: LLC SAPI Spare Field non-0!")
TRACE_MSG(SM_DECODE_390_112_2_17_22_56_58_3948,"SM: LLC SAPI not assigned!")
TRACE_MSG(SM_DECODE_402_112_2_17_22_56_58_3949,"SM: LLC SAPI value reserved,invalid!")
TRACE_MSG(SM_DECODE_546_112_2_17_22_56_58_3950,"SM: is_r99 = %d")
TRACE_MSG(SM_DECODE_577_112_2_17_22_56_59_3951,"SM:decode Qos Field invalid!")
TRACE_MSG(SM_DECODE_677_112_2_17_22_56_59_3952,"SM:decode Qos Field invalid!")
TRACE_MSG(SM_DECODE_684_112_2_17_22_56_59_3953,"SM:ignore extented qos fields!")
TRACE_MSG(SM_DECODE_726_112_2_17_22_56_59_3954,"SM: Radio Priority Spare Field non-0!")
TRACE_MSG(SM_DECODE_731_112_2_17_22_56_59_3955,"SM: Radio Priority reserved,level 4!")
TRACE_MSG(SM_DECODE_772_112_2_17_22_56_59_3956,"SM:Receive APN Length > APN buffer!")
TRACE_MSG(SM_DECODE_1669_112_2_17_22_57_1_3957,"SM:PDP context accept Qos < MS Min_Qos Setting!!")
TRACE_MSG(SM_DECODE_1674_112_2_17_22_57_1_3958,"SM:PDP context accept Qos > MS Min_Qos Setting!!\n")
TRACE_MSG(SM_DECODE_1705_112_2_17_22_57_1_3959,"APN:invalid char appear!")
TRACE_MSG(SM_ENCODE_280_112_2_17_22_57_2_3960,"SM:SM_EncodePdpaddr pdp_addr_length too long!")
TRACE_MSG(SM_ENCODE_312_112_2_17_22_57_2_3961,"SM:SM_EncodePdpaddr pdp_addr_length too long!")
TRACE_MSG(SM_ENCODE_327_112_2_17_22_57_2_3962,"SM:Assert Failure,SM_EncodePdpaddr")
TRACE_MSG(SM_ENCODE_360_112_2_17_22_57_2_3963,"SM:SM_EncodeApn apn too long!")
TRACE_MSG(SM_ENTITY_444_112_2_17_22_57_4_3964,"SM: Invalid state!")
TRACE_MSG(SM_ENTITY_696_112_2_17_22_57_5_3965,"SMentity:sapi not match.")
TRACE_MSG(SM_ENTITY_712_112_2_17_22_57_5_3966,"SMentity:sapi not match.")
TRACE_MSG(SM_ENTITY_749_112_2_17_22_57_5_3967,"SMentity:sapi not match.")
TRACE_MSG(SM_ENTITY_773_112_2_17_22_57_5_3968,"SMentity:sapi not match.")
TRACE_MSG(SM_ENTITY_779_112_2_17_22_57_5_3969,"SM: Discard SNSM_STATUS_REQ in state %d!")
TRACE_MSG(SM_ENTITY_867_112_2_17_22_57_5_3970,"SM: Invalid state!")
TRACE_MSG(SM_ENTITY_955_112_2_17_22_57_5_3971,"SM: Invalid state!")
TRACE_MSG(SM_ENTITY_1667_112_2_17_22_57_7_3972,"SM: ignore the message!")
TRACE_MSG(SM_MAIN_918_112_2_17_22_57_9_3973,"SM: discard SNSM_ACTIVATE_RES in INACTIVATE state")
TRACE_MSG(SM_MAIN_945_112_2_17_22_57_9_3974,"SM: discard SNSM_DEACTIVATE_RES in INACTIVATE state")
TRACE_MSG(SM_MAIN_972_112_2_17_22_57_9_3975,"SM: discard SNSM_MODIFY_RES in INACTIVATE state")
TRACE_MSG(SM_MAIN_998_112_2_17_22_57_9_3976,"SM: discard SNSM_STATUS_REQ in INACTIVATE state")
TRACE_MSG(SM_MAIN_1420_112_2_17_22_57_10_3977,"SM_TASK: save signal %d")
TRACE_MSG(SM_MAIN_1734_112_2_17_22_57_11_3978,"SM_TASK: discard signal:%d")
TRACE_MSG(SM_TIMER_72_112_2_17_22_57_18_3979,"SM_StartTimer timer_type = %d")
TRACE_MSG(SM_TIMER_108_112_2_17_22_57_18_3980,"SM_StopTimer timer_type = %d")
TRACE_MSG(SMC_COMMON_232_112_2_17_22_57_19_3981,"SMC: msg_id is 0x%x")
TRACE_MSG(SMC_COMMON_260_112_2_17_22_57_19_3982,"SMC: SMC_GetGsmCause, unknown msg_id: 0x%x")
TRACE_MSG(SMC_COMMON_398_112_2_17_22_57_19_3983,"SMC: CP-msg, AnalyzeResult:%d,pCause:%d")
TRACE_MSG(SMC_COMMON_506_112_2_17_22_57_19_3984,"SMC: send CM_MM_REL_REQ to MM, TI:%d")
TRACE_MSG(SMC_COMMON_575_112_2_17_22_57_19_3985,"SMC: send CM_MM_DATA_REQ for CP-DATA, TI: %d, length: %d!")
TRACE_MSG(SMC_COMMON_621_112_2_17_22_57_19_3986,"SMC: send CM_MM_DATA_REQ for CP-ACK, TI: %d, length: %d!")
TRACE_MSG(SMC_COMMON_672_112_2_17_22_57_20_3987,"SMC: send CM_MM_DATA_REQ for CP-ERROR, TI: %d, length: %d")
TRACE_MSG(SMC_COMMON_1033_112_2_17_22_57_20_3988,"SMC:cause moduletype:%d,mm cause:%d")
TRACE_MSG(SMC_CS_CTRL_49_112_2_17_22_57_21_3989,"SMC: CS-SMC_MO_IDLE, recv Msg: %s")
TRACE_MSG(SMC_CS_CTRL_97_112_2_17_22_57_21_3990,"SMC: CS received %s and discarded under state SMC_MO_IDLE !")
TRACE_MSG(SMC_CS_CTRL_103_112_2_17_22_57_21_3991,"SMC: CS received unknown 0x%x under state SMC_MO_IDLE !")
TRACE_MSG(SMC_CS_CTRL_122_112_2_17_22_57_21_3992,"SMC: CS-SMC_MO_MM_CONNECTION_PENDING, recv Msg: %s")
TRACE_MSG(SMC_CS_CTRL_171_112_2_17_22_57_21_3993,"SMC: CS received %s and discarded under state SMC_MO_MM_CONNECTION_PENDING !")
TRACE_MSG(SMC_CS_CTRL_177_112_2_17_22_57_21_3994,"SMC: CS received unknown 0x%x under state SMC_MO_MM_CONNECTION_PENDING !")
TRACE_MSG(SMC_CS_CTRL_196_112_2_17_22_57_21_3995,"SMC: CS-SMC_MO_WAIT_FOR_CP_ACK, recv Msg: %s")
TRACE_MSG(SMC_CS_CTRL_263_112_2_17_22_57_21_3996,"SMC: CS received %s and discarded under state SMC_MO_WAIT_FOR_CP_ACK !")
TRACE_MSG(SMC_CS_CTRL_269_112_2_17_22_57_21_3997,"SMC: CS received unknown 0x%x under state SMC_MO_WAIT_FOR_CP_ACK !")
TRACE_MSG(SMC_CS_CTRL_288_112_2_17_22_57_21_3998,"SMC: CS-SMC_MO_MM_CONNECTION_ESTABLISHED, recv Msg: %s")
TRACE_MSG(SMC_CS_CTRL_430_112_2_17_22_57_22_3999,"SMC: CS received %s and discarded under state SMC_MO_MM_CONNECTION_ESTABLISHED !")
TRACE_MSG(SMC_CS_CTRL_436_112_2_17_22_57_22_4000,"SMC: CS received unknown 0x%x under state SMC_MO_MM_CONNECTION_ESTABLISHED !")
TRACE_MSG(SMC_CS_CTRL_474_112_2_17_22_57_22_4001,"SMC: unknown SMC CS MO state 0x%x")
TRACE_MSG(SMC_CS_CTRL_492_112_2_17_22_57_22_4002,"SMC: CS-SMC_MT_IDLE, recv Msg: %s")
TRACE_MSG(SMC_CS_CTRL_520_112_2_17_22_57_22_4003,"SMC: CS received %s and discarded under state SMC_MT_IDLE !")
TRACE_MSG(SMC_CS_CTRL_526_112_2_17_22_57_22_4004,"SMC: CS received unknown 0x%x under state SMC_MT_IDLE !")
TRACE_MSG(SMC_CS_CTRL_545_112_2_17_22_57_22_4005,"SMC: CS-SMC_MT_MM_CONNECTION_ESTABLISHED, recv Msg: %s")
TRACE_MSG(SMC_CS_CTRL_601_112_2_17_22_57_22_4006,"SMC: CS received %s and discarded under state SMC_MT_MM_CONNECTION_ESTABLISHED !")
TRACE_MSG(SMC_CS_CTRL_607_112_2_17_22_57_22_4007,"SMC: CS received unknown 0x%x under state SMC_MT_MM_CONNECTION_ESTABLISHED !")
TRACE_MSG(SMC_CS_CTRL_626_112_2_17_22_57_22_4008,"SMC: CS-SMC_MT_WAIT_FOR_CP_ACK, recv Msg: %s")
TRACE_MSG(SMC_CS_CTRL_717_112_2_17_22_57_22_4009,"SMC: CS received %s and discarded under state SMC_MT_WAIT_FOR_CP_ACK !")
TRACE_MSG(SMC_CS_CTRL_723_112_2_17_22_57_22_4010,"SMC: CS received unknown 0x%x under state SMC_MT_WAIT_FOR_CP_ACK !")
TRACE_MSG(SMC_CS_CTRL_757_112_2_17_22_57_22_4011,"SMC: unknown SMC CS MT state 0x%x")
TRACE_MSG(SMC_CS_CTRL_797_112_2_17_22_57_22_4012,"SMC: unknown SMC CS message direction 0x%x")
TRACE_MSG(SMC_CS_FUNC_92_112_2_17_22_57_23_4013,"SMC:SMC_CS_HandleMmEstablishCnf,cur_ti:%d,EstablishCnf.TI:%d")
TRACE_MSG(SMC_CS_FUNC_179_112_2_17_22_57_23_4014,"SMC: receive MO_CP_MsgType: %d,cur_ti_nw:%d, recv TI: %d")
TRACE_MSG(SMC_CS_FUNC_574_112_2_17_22_57_24_4015,"SMC: received MT_CP_MsgType: %d, cur_ti_nw: %d, TempTI: %d")
TRACE_MSG(SMC_PS_CTRL_45_112_2_17_22_57_25_4016,"SMC: PS-SMC_MO_IDLE, recv Msg: %s")
TRACE_MSG(SMC_PS_CTRL_102_112_2_17_22_57_25_4017,"SMC: PS received %s and discarded under state SMC_MO_IDLE !")
TRACE_MSG(SMC_PS_CTRL_108_112_2_17_22_57_25_4018,"SMC: PS received unknown 0x%x under state SMC_MO_IDLE !")
TRACE_MSG(SMC_PS_CTRL_124_112_2_17_22_57_25_4019,"SMC: PS-SMC_MO_GMM_CONNECTION_PENDING, recv Msg: %s")
TRACE_MSG(SMC_PS_CTRL_162_112_2_17_22_57_25_4020,"SMC: PS received %s and discarded under state SMC_MO_GMM_CONNECTION_PENDING !")
TRACE_MSG(SMC_PS_CTRL_168_112_2_17_22_57_25_4021,"SMC: PS received unknown 0x%x under state SMC_MO_GMM_CONNECTION_PENDING !")
TRACE_MSG(SMC_PS_CTRL_187_112_2_17_22_57_26_4022,"SMC: PS-SMC_MO_WAIT_FOR_CP_ACK, recv Msg: %s")
TRACE_MSG(SMC_PS_CTRL_258_112_2_17_22_57_26_4023,"SMC: PS received %s and discarded under state SMC_MO_WAIT_FOR_CP_ACK !")
TRACE_MSG(SMC_PS_CTRL_264_112_2_17_22_57_26_4024,"SMC: PS received unknown 0x%x under state SMC_MO_WAIT_FOR_CP_ACK !")
TRACE_MSG(SMC_PS_CTRL_283_112_2_17_22_57_26_4025,"SMC: PS-SMC_MO_WAIT_FOR_CP_DATA, recv Msg: %s")
TRACE_MSG(SMC_PS_CTRL_379_112_2_17_22_57_26_4026,"SMC: PS received %s and discarded under state SMC_MO_WAIT_FOR_CP_DATA !")
TRACE_MSG(SMC_PS_CTRL_385_112_2_17_22_57_26_4027,"SMC: PS received unknown 0x%x under state SMC_MO_WAIT_FOR_CP_DATA !")
TRACE_MSG(SMC_PS_CTRL_423_112_2_17_22_57_26_4028,"SMC: unknown SMC PS MO state 0x%x")
TRACE_MSG(SMC_PS_CTRL_441_112_2_17_22_57_26_4029,"SMC: PS-SMC_MT_IDLE, recv Msg: %s")
TRACE_MSG(SMC_PS_CTRL_489_112_2_17_22_57_26_4030,"SMC: PS received %s and discarded under state SMC_MT_IDLE !")
TRACE_MSG(SMC_PS_CTRL_495_112_2_17_22_57_26_4031,"SMC: PS received unknown 0x%x under state SMC_MT_IDLE !")
TRACE_MSG(SMC_PS_CTRL_514_112_2_17_22_57_26_4032,"SMC: PS-SMC_MT_WAIT_FOR_RP_ACK, recv Msg: %s")
TRACE_MSG(SMC_PS_CTRL_571_112_2_17_22_57_26_4033,"SMC: PS received %s and discarded under state SMC_MT_WAIT_FOR_RP_ACK !")
TRACE_MSG(SMC_PS_CTRL_577_112_2_17_22_57_26_4034,"SMC: PS received unknown 0x%x under state SMC_MT_WAIT_FOR_RP_ACK !")
TRACE_MSG(SMC_PS_CTRL_596_112_2_17_22_57_26_4035,"SMC: PS-SMC_MT_WAIT_FOR_CP_ACK, recv Msg: %s")
TRACE_MSG(SMC_PS_CTRL_666_112_2_17_22_57_26_4036,"SMC: PS received %s and discarded under state SMC_MT_WAIT_FOR_CP_ACK !")
TRACE_MSG(SMC_PS_CTRL_672_112_2_17_22_57_26_4037,"SMC: PS received unknown 0x%x under state SMC_MT_WAIT_FOR_CP_ACK !")
TRACE_MSG(SMC_PS_CTRL_705_112_2_17_22_57_27_4038,"SMC: unknown SMC PS MT state 0x%x")
TRACE_MSG(SMC_PS_CTRL_745_112_2_17_22_57_27_4039,"SMC: unknown SMC PS message direction 0x%x")
TRACE_MSG(SMC_PS_FUNC_89_112_2_17_22_57_27_4040,"SMC:smc_data_ptr->ps_entity.is_ps_mm_conn=%d,don't need send MM_SMS_ESTABLISH_REQ!!")
TRACE_MSG(SMC_PS_FUNC_103_112_2_17_22_57_27_4041,"SMC:smc_data_ptr->ps_entity.is_ps_mm_conn=%d,so smc need send MM_SMS_ESTABLISH_REQ!!")
TRACE_MSG(SMC_PS_FUNC_839_112_2_17_22_57_29_4042,"SMC: SMC_PS_MO_HandleLlcUnitDataInd, cp_msg_type:%d, nw_cause:%d")
TRACE_MSG(SMC_PS_FUNC_986_112_2_17_22_57_29_4043,"SMC: SMC_PS_MT_HandleLLCUnitDataInd, cp_msg_type:%d, nw_cause:%d")
TRACE_MSG(SMR_MO_CONTROL_48_112_2_17_22_57_29_4044,"SMR: SMR_MO_STATUS_IDLE, received MsgName: %s")
TRACE_MSG(SMR_MO_CONTROL_84_112_2_17_22_57_30_4045,"SMR: received %s and discarded under state SMR_MO_STATUS_IDLE !")
TRACE_MSG(SMR_MO_CONTROL_90_112_2_17_22_57_30_4046,"SMR: received unknown 0x%x under state SMR_MO_STATUS_IDLE!")
TRACE_MSG(SMR_MO_CONTROL_108_112_2_17_22_57_30_4047,"SMR: SMR_MO_STATUS_WAIT_FOR_RPACK, received MsgName: %s")
TRACE_MSG(SMR_MO_CONTROL_116_112_2_17_22_57_30_4048,"SMR:SMR_MO_STATUS_WAIT_FOR_RPACK,smr_pid: %x, proc_type: %d")
TRACE_MSG(SMR_MO_CONTROL_196_112_2_17_22_57_30_4049,"SMR: received %s and discarded under state SMR_MO_STATUS_WAIT_FOR_RPACK !")
TRACE_MSG(SMR_MO_CONTROL_202_112_2_17_22_57_30_4050,"SMR: received unknown 0x%x under state SMR_MO_STATUS_WAIT_FOR_RPACK !")
TRACE_MSG(SMR_MO_CONTROL_221_112_2_17_22_57_30_4051,"SMR: SMR_MO_STATUS_WAIT_FOR_RETRANTIMER, received MsgName: %s")
TRACE_MSG(SMR_MO_CONTROL_267_112_2_17_22_57_30_4052,"SMR: received %s and discarded under state SMR_MO_STATUS_WAIT_FOR_RETRANTIMER !")
TRACE_MSG(SMR_MO_CONTROL_273_112_2_17_22_57_30_4053,"SMR: received unknown message 0x%x under state SMR_MO_STATUS_WAIT_FOR_RETRANTIMER !")
TRACE_MSG(SMR_MO_CONTROL_311_112_2_17_22_57_30_4054,"SMR: unknown SMR MO state 0x%x")
TRACE_MSG(SMR_MO_FUNC_48_112_2_17_22_57_30_4055,"SMR: SMR_SMC_EST_REQ domain: %d; RAT : %d")
TRACE_MSG(SMR_MO_FUNC_87_112_2_17_22_57_30_4056,"SMR: CheckDataInd failed, data_ind_ptr->data_len: %d is TOO short")
TRACE_MSG(SMR_MO_FUNC_101_112_2_17_22_57_30_4057,"SMR: CheckDataInd failed, data_ind_ptr->rp_data_arr[1]: %d, smr_pid:%x, msg_ref: %d")
TRACE_MSG(SMR_MO_FUNC_139_112_2_17_22_57_31_4058,"SMR: recv msg_ref:%d, rp_type:%d")
TRACE_MSG(SMR_MO_FUNC_354_112_2_17_22_57_31_4059,"SMR: recv msg_ref:%d, rp_msg_type:%d")
TRACE_MSG(SMR_MT_CONTROL_45_112_2_17_22_57_31_4060,"SMR: SMR_MT_STATUS_IDLE, received MsgName: %s")
TRACE_MSG(SMR_MT_CONTROL_76_112_2_17_22_57_32_4061,"SMR: received %s and discarded under state SMR_MT_STATUS_IDLE !")
TRACE_MSG(SMR_MT_CONTROL_82_112_2_17_22_57_32_4062,"SMR: received unknown 0x%x under state SMR_MT_STATUS_IDLE !")
TRACE_MSG(SMR_MT_CONTROL_99_112_2_17_22_57_32_4063,"SMR: SMR_MT_STATUS_WAIT_TO_SEND_RPACK, received MsgName: %s")
TRACE_MSG(SMR_MT_CONTROL_129_112_2_17_22_57_32_4064,"SMR: received %s and discarded under state SMR_MT_STATUS_WAIT_TO_SEND_RPACK !")
TRACE_MSG(SMR_MT_CONTROL_135_112_2_17_22_57_32_4065,"SMR: received unknown 0x%x under state SMR_MT_STATUS_WAIT_TO_SEND_RPACK !")
TRACE_MSG(SMR_MT_CONTROL_167_112_2_17_22_57_32_4066,"SMR: unknown SMR MT state 0x%x")
TRACE_MSG(SMR_PUBLIC_185_112_2_17_22_57_33_4067,"SMR:element_len > SMR_RPDU_MAX_LENGTH - 1,element_len=%d,")
TRACE_MSG(SMR_PUBLIC_196_112_2_17_22_57_33_4068,"SMR:total_len != data_len ,total_len=%d,data_len=%d")
TRACE_MSG(SMS_COMMON_API_65_112_2_17_22_57_34_4069,"SMS: TimerStartCmd, timer_type(%d) not found")
TRACE_MSG(SMS_COMMON_API_98_112_2_17_22_57_34_4070,"SMS: DeactiveTimer, timer_type(%d) not found")
TRACE_MSG(SMS_RL_186_112_2_17_22_57_35_4071,"SMSRL_DecodeRpError: the cause is %d")
TRACE_MSG(RFC2507C_105_112_2_17_22_57_38_4072,"\n")
TRACE_MSG(RFC2507C_106_112_2_17_22_57_38_4073,"v6s_full_c \t %d \n")
TRACE_MSG(RFC2507C_107_112_2_17_22_57_38_4074,"v6s_comp_c \t %d \n")
TRACE_MSG(RFC2507C_108_112_2_17_22_57_38_4075,"v6s_unmod_c \t %d \n")
TRACE_MSG(RFC2507C_109_112_2_17_22_57_38_4076,"v6s_bbefore_c \t %d \n")
TRACE_MSG(RFC2507C_110_112_2_17_22_57_38_4077,"v6s_bafter_c \t %d \n")
TRACE_MSG(RFC2507C_111_112_2_17_22_57_38_4078,"v6s_changes_c \t %d \n")
TRACE_MSG(RFC2507C_112_112_2_17_22_57_39_4079,"v6s_unknown_c \t %d \n")
TRACE_MSG(RFC2507C_113_112_2_17_22_57_39_4080,"v6s_full_d \t %d \n")
TRACE_MSG(RFC2507C_114_112_2_17_22_57_39_4081,"v6s_comp_d \t %d \n")
TRACE_MSG(RFC2507C_115_112_2_17_22_57_39_4082,"v6s_unmod_d \t %d \n")
TRACE_MSG(RFC2507C_116_112_2_17_22_57_39_4083,"v6s_dropped_d \t %d \n")
TRACE_MSG(RFC2507C_117_112_2_17_22_57_39_4084,"v6s_badsum_d \t %d \n")
TRACE_MSG(RFC2507C_118_112_2_17_22_57_39_4085,"v6s_head_req_revc \t %d \n")
TRACE_MSG(RFC2507C_119_112_2_17_22_57_39_4086,"\n")
TRACE_MSG(RFC2507C_172_112_2_17_22_57_39_4087,"free_all: start\n")
TRACE_MSG(RFC2507C_208_112_2_17_22_57_39_4088,"free_all: done\n")
TRACE_MSG(RFC2507C_414_112_2_17_22_57_39_4089,"iphc_init: start sizeof(*csList) == %u\n")
TRACE_MSG(RFC2507C_416_112_2_17_22_57_39_4090,"iphc_init: sizeof(*comp) == %u sizeof(*decomp) == %un")
TRACE_MSG(RFC2507C_419_112_2_17_22_57_39_4091,"iphc_init: csList == NULL\n")
TRACE_MSG(RFC2507C_428_112_2_17_22_57_39_4092,"iphc_init: cfg == NULL\n")
TRACE_MSG(RFC2507C_433_112_2_17_22_57_39_4093,"iphc_init: len_send_t %d \n")
TRACE_MSG(RFC2507C_434_112_2_17_22_57_39_4094,"iphc_init: len_recv_t %d \n")
TRACE_MSG(RFC2507C_435_112_2_17_22_57_39_4095,"iphc_init: len_send_u %d \n")
TRACE_MSG(RFC2507C_436_112_2_17_22_57_39_4096,"iphc_init: len_recv_u %d \n")
TRACE_MSG(RFC2507C_437_112_2_17_22_57_39_4097,"iphc_init: max_header %d \n")
TRACE_MSG(RFC2507C_438_112_2_17_22_57_39_4098,"iphc_init: max_period %d \n")
TRACE_MSG(RFC2507C_439_112_2_17_22_57_39_4099,"iphc_init: max_time   %d \n")
TRACE_MSG(RFC2507C_440_112_2_17_22_57_39_4100,"iphc_init: min_wrap   %d \n")
TRACE_MSG(RFC2507C_441_112_2_17_22_57_39_4101,"iphc_init: reordering %d \n")
TRACE_MSG(RFC2507C_442_112_2_17_22_57_39_4102,"iphc_init: nodelta    %d \n")
TRACE_MSG(RFC2507C_443_112_2_17_22_57_39_4103,"iphc_init: udp_prime  %d \n")
TRACE_MSG(RFC2507C_444_112_2_17_22_57_39_4104,"iphc_init: tcp_prime  %d \n")
TRACE_MSG(RFC2507C_459_112_2_17_22_57_39_4105,"iphc_init: bad init values\n")
TRACE_MSG(RFC2507C_464_112_2_17_22_57_39_4106,"iphc_init: bad value for tcp_prime\n")
TRACE_MSG(RFC2507C_468_112_2_17_22_57_39_4107,"iphc_init: bad value for udp_prime\n")
TRACE_MSG(RFC2507C_539_112_2_17_22_57_39_4108,"iphc_init: init done.\n")
TRACE_MSG(RFC2507C_673_112_2_17_22_57_40_4109,"hash_keys: *** UNKNOWN IP version %d ***\n")
TRACE_MSG(RFC2507C_678_112_2_17_22_57_40_4110,"iphc_hashkeys: IPv4 hdr corrupt\n")
TRACE_MSG(RFC2507C_681_112_2_17_22_57_40_4111,"hash_keys: IP4\n")
TRACE_MSG(RFC2507C_688_112_2_17_22_57_40_4112,"hash_keys: length fields for IP4 do not match! Bailing out!\n")
TRACE_MSG(RFC2507C_690_112_2_17_22_57_40_4113,"hash_keys: length fields are: pkt %u len %u ip %un")
TRACE_MSG(RFC2507C_691_112_2_17_22_57_40_4114,"hash_keys: %s\n")
TRACE_MSG(RFC2507C_704_112_2_17_22_57_40_4115,"hash_keys: ip4 fragment\n")
TRACE_MSG(RFC2507C_716_112_2_17_22_57_40_4116,"hash_keys: ip4 options len=%u  \n")
TRACE_MSG(RFC2507C_739_112_2_17_22_57_40_4117,"hash_keys: mine hdr corrupt\n")
TRACE_MSG(RFC2507C_745_112_2_17_22_57_40_4118,"hash_keys: IPPROTO_MIN_CAP  length %u\n")
TRACE_MSG(RFC2507C_751_112_2_17_22_57_40_4119,"hash_keys: IPPROTO_MIN_CAP  next %x \n")
TRACE_MSG(RFC2507C_757_112_2_17_22_57_40_4120,"hash_keys: auth hdr corrupt\n")
TRACE_MSG(RFC2507C_762_112_2_17_22_57_40_4121,"hash_keys: IPPROTO_AH4  length %u \n")
TRACE_MSG(RFC2507C_767_112_2_17_22_57_40_4122,"hash_keys: IPPROTO_AH4  next %x \n")
TRACE_MSG(RFC2507C_773_112_2_17_22_57_40_4123,"hash_keys: unknown proto %d in ip4\n")
TRACE_MSG(RFC2507C_785_112_2_17_22_57_40_4124,"hash_keys: IP6\n")
TRACE_MSG(RFC2507C_793_112_2_17_22_57_40_4125,"hash_keys: length fields for IP6 does not match! Bailing out!\n")
TRACE_MSG(RFC2507C_816_112_2_17_22_57_40_4126,"hash_keys: extention hdr corrupt\n")
TRACE_MSG(RFC2507C_829_112_2_17_22_57_40_4127,"hash_keys: ROUTING\n")
TRACE_MSG(RFC2507C_831_112_2_17_22_57_40_4128,"hash_keys: extension hdr corrupt\n")
TRACE_MSG(RFC2507C_846_112_2_17_22_57_40_4129,"hash_keys: IPPROTO_AH6  length %u \n")
TRACE_MSG(RFC2507C_848_112_2_17_22_57_40_4130,"hash_keys: auth hdr corrupt\n")
TRACE_MSG(RFC2507C_857_112_2_17_22_57_40_4131,"hash_keys: IPPROTO_AH6  next %x \n")
TRACE_MSG(RFC2507C_874_112_2_17_22_57_40_4132,"hash_keys: FRAGMENT\n")
TRACE_MSG(RFC2507C_882_112_2_17_22_57_40_4133,"hash_keys: IPPROTO_NONE\n")
TRACE_MSG(RFC2507C_887_112_2_17_22_57_40_4134,"hash_keys: unknown proto %d in ip6_hdr\n")
TRACE_MSG(RFC2507C_899_112_2_17_22_57_40_4135,"hash_keys: ESP\n")
TRACE_MSG(RFC2507C_908_112_2_17_22_57_40_4136,"hash_keys: UDP\n")
TRACE_MSG(RFC2507C_916_112_2_17_22_57_40_4137,"hash_keys: length fields for UDP does not match! Bailing out!\n")
TRACE_MSG(RFC2507C_922_112_2_17_22_57_40_4138,"hash_keys: udp ports s %d, d %dn")
TRACE_MSG(RFC2507C_928_112_2_17_22_57_40_4139,"hash_keys: TCP\n")
TRACE_MSG(RFC2507C_930_112_2_17_22_57_40_4140,"iphc_hashkeys: TCP hdr corrupt\n")
TRACE_MSG(RFC2507C_939_112_2_17_22_57_40_4141,"hash_keys: tcp ports s %d, d %dn")
TRACE_MSG(RFC2507C_959_112_2_17_22_57_40_4142,"equal_flows: ipv = %d\n")
TRACE_MSG(RFC2507C_968_112_2_17_22_57_40_4143,"equal_flows: ip4\n")
TRACE_MSG(RFC2507C_978_112_2_17_22_57_40_4144,"equal_flows: ip4 fragment\n")
TRACE_MSG(RFC2507C_985_112_2_17_22_57_40_4145,"equal_flows: ip4 options len=%u\n")
TRACE_MSG(RFC2507C_994_112_2_17_22_57_40_4146,"equal_flows: ip4h\n")
TRACE_MSG(RFC2507C_1033_112_2_17_22_57_40_4147,"equal_flows: unknown proto %d in ip4\n")
TRACE_MSG(RFC2507C_1039_112_2_17_22_57_40_4148,"equal_flows: ip6\n")
TRACE_MSG(RFC2507C_1058_112_2_17_22_57_40_4149,"equal_flows: ip6 options i=%x t=%x\n")
TRACE_MSG(RFC2507C_1111_112_2_17_22_57_41_4150,"equal_flows: frament\n")
TRACE_MSG(RFC2507C_1118_112_2_17_22_57_41_4151,"equal_flows: none\n")
TRACE_MSG(RFC2507C_1122_112_2_17_22_57_41_4152,"equal_flows: unknown proto %d in ip6_hdr\n")
TRACE_MSG(RFC2507C_1128_112_2_17_22_57_41_4153,"equal_flows: esp\n")
TRACE_MSG(RFC2507C_1135_112_2_17_22_57_41_4154,"equal_flows: udp\n")
TRACE_MSG(RFC2507C_1139_112_2_17_22_57_41_4155,"equal_flows: udp ports s %d, d %dn")
TRACE_MSG(RFC2507C_1145_112_2_17_22_57_41_4156,"equal_flows: tcp\n")
TRACE_MSG(RFC2507C_1150_112_2_17_22_57_41_4157,"equal_flows: tcp ports s %d, d %dn")
TRACE_MSG(RFC2507C_1227_112_2_17_22_57_41_4158,"compress: LARGE_CID %u \n")
TRACE_MSG(RFC2507C_1250_112_2_17_22_57_41_4159,"compress: ip4:\n")
TRACE_MSG(RFC2507C_1258_112_2_17_22_57_41_4160,"compress: ip4 fragment\n")
TRACE_MSG(RFC2507C_1270_112_2_17_22_57_41_4161,"compress: ip4 options len=%u\n")
TRACE_MSG(RFC2507C_1299_112_2_17_22_57_41_4162,"compress: ip4h:\n")
TRACE_MSG(RFC2507C_1333_112_2_17_22_57_41_4163,"compress: unknown proto %x in ipv4\n")
TRACE_MSG(RFC2507C_1339_112_2_17_22_57_41_4164,"compress: ip6:\n")
TRACE_MSG(RFC2507C_1468_112_2_17_22_57_41_4165,"compress: proto NONE %x in ip6_hdr\n")
TRACE_MSG(RFC2507C_1472_112_2_17_22_57_41_4166,"compress: unknown proto %x in ip6_hdr\n")
TRACE_MSG(RFC2507C_1482_112_2_17_22_57_41_4167,"compress: tcp\n")
TRACE_MSG(RFC2507C_1487_112_2_17_22_57_41_4168,"compress: tcp seq %u  ack %un")
TRACE_MSG(RFC2507C_1491_112_2_17_22_57_41_4169,"compress: doing NODELTA\n")
TRACE_MSG(RFC2507C_1510_112_2_17_22_57_41_4170,"compress: header request for cid %u ch %u\n")
TRACE_MSG(RFC2507C_1530_112_2_17_22_57_41_4171,"compress: ipv4 r_byte is SET to %x  ip_ecn_save %xn")
TRACE_MSG(RFC2507C_1541_112_2_17_22_57_41_4172,"compress: ipv6 r_byte is SET to %x  ip_ecn_save %xn")
TRACE_MSG(RFC2507C_1556_112_2_17_22_57_41_4173,"compress: deltaS = %u iseq %u tseq %un")
TRACE_MSG(RFC2507C_1614_112_2_17_22_57_42_4174,"compress: NOTHING\n")
TRACE_MSG(RFC2507C_1622_112_2_17_22_57_42_4175,"compress: SPECIAL -> full\n")
TRACE_MSG(RFC2507C_1626_112_2_17_22_57_42_4176,"compress: NEW_S | NEW_A\n")
TRACE_MSG(RFC2507C_1631_112_2_17_22_57_42_4177,"compress: SPECIAL_I\n")
TRACE_MSG(RFC2507C_1636_112_2_17_22_57_42_4178,"compress: NEW_S\n")
TRACE_MSG(RFC2507C_1641_112_2_17_22_57_42_4179,"compress: SPECIAL_D deltaS = %u\n")
TRACE_MSG(RFC2507C_1658_112_2_17_22_57_42_4180,"compress: tcp options %u bytes\n")
TRACE_MSG(RFC2507C_1666_112_2_17_22_57_42_4181,"compress: TcpHc= %s\n")
TRACE_MSG(RFC2507C_1675_112_2_17_22_57_42_4182,"compress: r_byte  restore ipv4 %x  newvalue=%xn")
TRACE_MSG(RFC2507C_1679_112_2_17_22_57_42_4183,"compress: r_byte  restore ipv6 %x  newvalue=%xn")
TRACE_MSG(RFC2507C_1689_112_2_17_22_57_42_4184,"compress: done_tcp cinfo=%x \" rdp %x chdr %xn")
TRACE_MSG(RFC2507C_1738_112_2_17_22_57_42_4185,"compress: done NODELTA\n")
TRACE_MSG(RFC2507C_1746_112_2_17_22_57_42_4186,"compress: CH %s\n")
TRACE_MSG(RFC2507C_1748_112_2_17_22_57_42_4187,"compress: comp %d - %d = %dn")
TRACE_MSG(RFC2507C_1758_112_2_17_22_57_42_4188,"compress: error cinfo")
TRACE_MSG(RFC2507C_1764_112_2_17_22_57_42_4189,"compress: udp\n")
TRACE_MSG(RFC2507C_1785_112_2_17_22_57_42_4190,"compress: max_time now - last == xxx %u - %u == %u  max %un")
TRACE_MSG(RFC2507C_1804_112_2_17_22_57_42_4191,"compress: bailing out on min wrap violation (cid %u)n")
TRACE_MSG(RFC2507C_1812_112_2_17_22_57_42_4192,"compress: min wrap not violated (cid %u)\n")
TRACE_MSG(RFC2507C_1851_112_2_17_22_57_42_4193,"compress: max_time expired  sending full\n")
TRACE_MSG(RFC2507C_1860_112_2_17_22_57_42_4194,"compress: done_udp %d \" rdp %x chdr %xn")
TRACE_MSG(RFC2507C_1956_112_2_17_22_57_42_4195,"compress: CH %s\n")
TRACE_MSG(RFC2507C_1958_112_2_17_22_57_42_4196,"compress: comp %d - %d = %dn")
TRACE_MSG(RFC2507C_1965_112_2_17_22_57_42_4197,"compress: error cinfo")
TRACE_MSG(RFC2507C_1972_112_2_17_22_57_42_4198,"compress: bailing out\n")
TRACE_MSG(RFC2507C_2001_112_2_17_22_57_42_4199,"iphc_comp: length of incoming packet < IPv4 hdr\n")
TRACE_MSG(RFC2507C_2009_112_2_17_22_57_42_4200,"iphc_comp: ---------- ipv4 iphdr_len %d,  real len %dn")
TRACE_MSG(RFC2507C_2014_112_2_17_22_57_42_4201,"iphc_comp: ---------- ipv6 iphdr_len %d,  real len %dn")
TRACE_MSG(RFC2507C_2022_112_2_17_22_57_42_4202,"iphc_comp: H  %s\n")
TRACE_MSG(RFC2507C_2023_112_2_17_22_57_42_4203,"iphc_comp: H  %s\n")
TRACE_MSG(RFC2507C_2048_112_2_17_22_57_43_4204,"iphc_comp: compress => ci_regu\n")
TRACE_MSG(RFC2507C_2054_112_2_17_22_57_43_4205,"compress => unknown\n")
TRACE_MSG(RFC2507C_2079_112_2_17_22_57_43_4206,"iphc_comp: compress => ci_full\n")
TRACE_MSG(RFC2507C_2088_112_2_17_22_57_43_4207,"iphc_comp: compress => ci_comp\n")
TRACE_MSG(RFC2507C_2095_112_2_17_22_57_43_4208,"iphc_comp: compress => ci_regu\n")
TRACE_MSG(RFC2507C_2101_112_2_17_22_57_43_4209,"compress => unknown\n")
TRACE_MSG(RFC2507D_209_112_2_17_22_57_43_4210,"iphc_decompf: ----------  len=%d\n")
TRACE_MSG(RFC2507D_210_112_2_17_22_57_43_4211,"iphc_decompf: FH %s\n")
TRACE_MSG(RFC2507D_211_112_2_17_22_57_43_4212,"iphc_decompf: FH %s\n")
TRACE_MSG(RFC2507D_214_112_2_17_22_57_43_4213,"iphc_decompf: Too large packet, don't try to decompress\n")
TRACE_MSG(RFC2507D_222_112_2_17_22_57_43_4214,"iphc_decompf: length of incoming packet smaller < IPv4 hdrn")
TRACE_MSG(RFC2507D_230_112_2_17_22_57_43_4215,"iphc_decompf: ipv4\n")
TRACE_MSG(RFC2507D_244_112_2_17_22_57_43_4216,"iphc_decompf: ip6_hdr\n")
TRACE_MSG(RFC2507D_259_112_2_17_22_57_43_4217,"iphc_decompf: ip4\n")
TRACE_MSG(RFC2507D_262_112_2_17_22_57_43_4218,"iphc_decompf: IPv4 hdr corrupt\n")
TRACE_MSG(RFC2507D_267_112_2_17_22_57_43_4219,"iphc_decompf: IPv4 hdr length field too small\n")
TRACE_MSG(RFC2507D_275_112_2_17_22_57_43_4220,"iphc_decompf: ************ IP4\n")
TRACE_MSG(RFC2507D_284_112_2_17_22_57_43_4221,"iphc_decompf: ip4 fragment\n")
TRACE_MSG(RFC2507D_306_112_2_17_22_57_43_4222,"hash_keys: mine hdr corrupt\n")
TRACE_MSG(RFC2507D_318_112_2_17_22_57_43_4223,"iphc_decompf: auth in ip4_hdr\n")
TRACE_MSG(RFC2507D_320_112_2_17_22_57_44_4224,"hash_keys: auth hdr corrupt\n")
TRACE_MSG(RFC2507D_333_112_2_17_22_57_44_4225,"iphc_decompf: unknown proto %x in ip4_hdr\n")
TRACE_MSG(RFC2507D_339_112_2_17_22_57_44_4226,"iphc_decompf: ip6\n")
TRACE_MSG(RFC2507D_344_112_2_17_22_57_44_4227,"iphc_decompf: ************ IP6\n")
TRACE_MSG(RFC2507D_362_112_2_17_22_57_44_4228,"hash_keys: extention hdr corrupt\n")
TRACE_MSG(RFC2507D_374_112_2_17_22_57_44_4229,"hash_keys: auth hdr corrupt\n")
TRACE_MSG(RFC2507D_403_112_2_17_22_57_44_4230,"iphc_decompf: proto NONE %x in ip6_hdr\n")
TRACE_MSG(RFC2507D_407_112_2_17_22_57_44_4231,"iphc_decompf: unknown proto %x in ip6_hdr\n")
TRACE_MSG(RFC2507D_416_112_2_17_22_57_44_4232,"iphc_decompf: esp\n")
TRACE_MSG(RFC2507D_420_112_2_17_22_57_44_4233,"iphc_decompf: udp\n")
TRACE_MSG(RFC2507D_430_112_2_17_22_57_44_4234,"iphc_decompf: ************ UDP %u  n")
TRACE_MSG(RFC2507D_437_112_2_17_22_57_44_4235,"iphc_decompf: TCP header too small %d\n")
TRACE_MSG(RFC2507D_442_112_2_17_22_57_44_4236,"iphc_decompf: tcp\n")
TRACE_MSG(RFC2507D_447_112_2_17_22_57_44_4237,"iphc_decompf: tcp header is %d bytes longn")
TRACE_MSG(RFC2507D_450_112_2_17_22_57_44_4238,"iphc_decompf: too large tcp cid %d\n")
TRACE_MSG(RFC2507D_469_112_2_17_22_57_44_4239,"iphc_decompf: saved r_byte [TTttttII] %x\n")
TRACE_MSG(RFC2507D_475_112_2_17_22_57_44_4240,"iphc_decompf: bailing out\n")
TRACE_MSG(RFC2507D_481_112_2_17_22_57_44_4241,"decompf: LARGE CID \n")
TRACE_MSG(RFC2507D_485_112_2_17_22_57_44_4242,"decompf: LARGE CID %u\n")
TRACE_MSG(RFC2507D_490_112_2_17_22_57_44_4243,"iphc_decompf: too large tcp cid %d\n")
TRACE_MSG(RFC2507D_497_112_2_17_22_57_44_4244,"iphc_decompf: too large udp cid %d\n")
TRACE_MSG(RFC2507D_506_112_2_17_22_57_44_4245,"iphc_decompf: donefull cid %d gen %d len %d \n")
TRACE_MSG(RFC2507D_507_112_2_17_22_57_44_4246,"iphc_decompf: FH %s\n")
TRACE_MSG(RFC2507D_508_112_2_17_22_57_44_4247,"iphc_decompf: FH %s\n")
TRACE_MSG(RFC2507D_526_112_2_17_22_57_44_4248,"iphc_decompf: removed cid %u from queue\n")
TRACE_MSG(RFC2507D_571_112_2_17_22_57_44_4249,"iphc_decomp: CH %s\n")
TRACE_MSG(RFC2507D_574_112_2_17_22_57_44_4250,"iphc_decomp: Too large packet, don't try to decompress\n")
TRACE_MSG(RFC2507D_590_112_2_17_22_57_44_4251,"iphc_decomp: type TCP\n")
TRACE_MSG(RFC2507D_605_112_2_17_22_57_44_4252,"iphc_decomp: too large tcp cid %d\n")
TRACE_MSG(RFC2507D_609_112_2_17_22_57_44_4253,"iphc_decomp: tcp cid not in use %d\n")
TRACE_MSG(RFC2507D_629_112_2_17_22_57_44_4254,"iphc_decomp: type UDP\n")
TRACE_MSG(RFC2507D_633_112_2_17_22_57_44_4255,"iphc_decomp: too large udp cid %d\n")
TRACE_MSG(RFC2507D_640_112_2_17_22_57_44_4256,"iphc_decomp: udp cid not in use %d\n")
TRACE_MSG(RFC2507D_644_112_2_17_22_57_44_4257,"iphc_decomp: bad generation %u %u\n")
TRACE_MSG(RFC2507D_662_112_2_17_22_57_44_4258,"iphc_decomp: hmm... ip version %u\n")
TRACE_MSG(RFC2507D_669_112_2_17_22_57_44_4259,"iphc_decomp: IPv4 hdr length field too small\n")
TRACE_MSG(RFC2507D_672_112_2_17_22_57_44_4260,"iphc_decomp: IP4\n")
TRACE_MSG(RFC2507D_677_112_2_17_22_57_44_4261,"decomp: ip4 fragment\n")
TRACE_MSG(RFC2507D_690_112_2_17_22_57_44_4262,"iphc_decomp: IPv4 hdr length field too small\n")
TRACE_MSG(RFC2507D_713_112_2_17_22_57_44_4263,"decomp: ip4 options size changed \n")
TRACE_MSG(RFC2507D_791_112_2_17_22_57_45_4264,"iphc_decomp: unknown proto %x in ip4_hdr\n")
TRACE_MSG(RFC2507D_797_112_2_17_22_57_45_4265,"iphc_decomp: IP6\n")
TRACE_MSG(RFC2507D_862_112_2_17_22_57_45_4266,"iphc_decomp: IPPROTO_ROUTING segleft=%u n")
TRACE_MSG(RFC2507D_863_112_2_17_22_57_45_4267,"iphc_decomp:  %s\n")
TRACE_MSG(RFC2507D_867_112_2_17_22_57_45_4268,"iphc_decomp: IPPROTO_ROUTING OPTLEN=%u i=%un")
TRACE_MSG(RFC2507D_869_112_2_17_22_57_45_4269,"iphc_decomp: Bad optlen for routing option\n")
TRACE_MSG(RFC2507D_909_112_2_17_22_57_45_4270,"iphc_decomp: proto NONE  %x in ip6_hdr\n")
TRACE_MSG(RFC2507D_913_112_2_17_22_57_45_4271,"iphc_decomp: unknown proto %x in ip6_hdr\n")
TRACE_MSG(RFC2507D_923_112_2_17_22_57_45_4272,"iphc_decomp: TCP  left = %u\n")
TRACE_MSG(RFC2507D_953_112_2_17_22_57_45_4273,"iphc_decomp: r_byte *SET* to %6x\n")
TRACE_MSG(RFC2507D_956_112_2_17_22_57_45_4274,"iphc_decomp: r_byte default  %6x\n")
TRACE_MSG(RFC2507D_974_112_2_17_22_57_45_4275,"iphc_decomp: hmm... ip version %un")
TRACE_MSG(RFC2507D_987_112_2_17_22_57_45_4276,"iphc_decomp: SPECIAL_I\n")
TRACE_MSG(RFC2507D_994_112_2_17_22_57_45_4277,"iphc_decomp: SPECIAL_DATA seq = %un")
TRACE_MSG(RFC2507D_1031_112_2_17_22_57_45_4278,"iphc_decomp: NEW_O but no options in table\n")
TRACE_MSG(RFC2507D_1045_112_2_17_22_57_45_4279,"iphc_decomp: UDP space left = %u\n")
TRACE_MSG(RFC2507D_1060_112_2_17_22_57_45_4280,"iphc_decomp: bailing out\n")
TRACE_MSG(RFC2507D_1132_112_2_17_22_57_45_4281,"iphc_decomp: twice0_cksum %x   recv_cksum %xn")
TRACE_MSG(RFC2507D_1141_112_2_17_22_57_45_4282,"iphc_decomp: try twice once with delta = %xn")
TRACE_MSG(RFC2507D_1150_112_2_17_22_57_45_4283,"iphc_decomp: twice1_cksum %x   recv_cksum %xn")
TRACE_MSG(RFC2507D_1154_112_2_17_22_57_45_4284,"iphc_decomp: success with twice.\n")
TRACE_MSG(RFC2507D_1161_112_2_17_22_57_45_4285,"iphc_decomp: failed twice \n")
TRACE_MSG(RFC2507D_1175_112_2_17_22_57_45_4286,"iphc_decomp: Checksum mismatch, Twice not supported. \n")
TRACE_MSG(RFC2507D_1195_112_2_17_22_57_45_4287,"iphc_decomp: header request queued cid = %u\n")
TRACE_MSG(RFC2507D_1210_112_2_17_22_57_45_4288,"iphc_decomp: hlen %d   cs->len %d\n")
TRACE_MSG(RFC2507D_1214_112_2_17_22_57_45_4289,"iphc_decomp: cid %d  gen %d  hlen %d  comp_head_len %dn")
TRACE_MSG(RFC2507D_1215_112_2_17_22_57_45_4290,"iphc_decomp: DH %s\n")
TRACE_MSG(RFC2507D_1216_112_2_17_22_57_45_4291,"iphc_decomp: DH %s\n")
TRACE_MSG(SNDCP_ENTITY_745_112_2_17_22_57_49_4292,"SNCP: nsapi %d discard SDU on %d state")
TRACE_MSG(SNDCP_ENTITY_821_112_2_17_22_57_49_4293,"SNCP: NSAPI%d discard PDU on %d state")
TRACE_MSG(SNDCP_ENTITY_851_112_2_17_22_57_49_4294,"SNDCP: Discard NPDU when reaching on the max buffer_size!")
TRACE_MSG(SNDCP_ENTITY_885_112_2_17_22_57_49_4295,"SNDCP: NSAPI %d Discard NPDU on %d state!")
TRACE_MSG(SNDCP_ENTITY_989_112_2_17_22_57_49_4296,"SNCP: NSAPI%d discard PDU on %d state")
TRACE_MSG(SNDCP_ENTITY_1527_112_2_17_22_57_50_4297,"SNDCP: received release_ind with NORMAL_RELEASE on esting state")
TRACE_MSG(SNDCP_ENTITY_1537_112_2_17_22_57_50_4298,"SNDCP: received not defined cause: %d")
TRACE_MSG(SNDCP_MAIN_639_112_2_17_22_57_52_4299,"SNDCP: invalue nsapi")
TRACE_MSG(SNDCP_MAIN_650_112_2_17_22_57_52_4300,"SNDCP: invalue sapi")
TRACE_MSG(SNDCP_MAIN_661_112_2_17_22_57_52_4301,"SNDCP: NSAPI already existed")
TRACE_MSG(SNDCP_MAIN_717_112_2_17_22_57_52_4302,"SNDCP: NSAPI entity doesnt exist.")
TRACE_MSG(SNDCP_MAIN_1073_112_2_17_22_57_53_4303,"SNDCP: SN_UNITDATA_REQ - Null pointer")
TRACE_MSG(SNDCP_MAIN_1113_112_2_17_22_57_53_4304,"SNDCP: LL_SNDC_UNITDATA_IND - Null pointer")
TRACE_MSG(SNDCP_MAIN_1121_112_2_17_22_57_53_4305,"SNDCP: LL_SNDC_UNITDATA_IND - wrong nsapi")
TRACE_MSG(SNDCP_MAIN_1128_112_2_17_22_57_53_4306,"SNDCP: LL_SNDC_UNITDATA_IND - wrong sapi")
TRACE_MSG(SNDCP_MAIN_1135_112_2_17_22_57_53_4307,"SNDCP: LL_SNDC_UNITDATA_IND - wrong nsapi")
TRACE_MSG(SNDCP_MAIN_1162_112_2_17_22_57_53_4308,"SNDCP:SN_DATA_REQ - Null pointer")
TRACE_MSG(SNDCP_MAIN_1202_112_2_17_22_57_53_4309,"SNDCP: LL_SNDC_DATA_IND - Null pointer")
TRACE_MSG(SNDCP_MAIN_1210_112_2_17_22_57_53_4310,"SNDCP: LL_SNDC_DATA_IND - wrong nsapi")
TRACE_MSG(SNDCP_MAIN_1217_112_2_17_22_57_53_4311,"SNDCP: LL_SNDC_UNITDATA_IND - wrong nsapi")
TRACE_MSG(SNDCP_MAIN_1224_112_2_17_22_57_53_4312,"SNDCP: LL_SNDC_UNITDATA_IND - wrong nsapi")
TRACE_MSG(SNDCP_MAIN_1278_112_2_17_22_57_53_4313,"SNDCP: suspend sapi not exist.")
TRACE_MSG(SNDCP_MAIN_1316_112_2_17_22_57_54_4314,"SNDCP: resume sapi not exist.")
TRACE_MSG(SNDCP_MAIN_1406_112_2_17_22_57_54_4315,"SNDCP: modify invalue nsapi")
TRACE_MSG(SNDCP_MAIN_1417_112_2_17_22_57_54_4316,"SNDCP: modify invalue sapi")
TRACE_MSG(SNDCP_MAIN_1711_112_2_17_22_57_54_4317,"SNDCP: save signal %d")
TRACE_MSG(SNDCP_MAIN_1724_112_2_17_22_57_54_4318,"SNDCP: discard signal %d")
TRACE_MSG(SNDCP_PC_2629_112_2_17_22_58_1_4319,"SNDCP:pc_decompress result = %d, s_sndcp_pc0.nsapis = %d, nsapi = %d")
TRACE_MSG(SNDCP_SDL_452_112_2_17_22_58_3_4320,"SNDCP: Unack seg_count = %d")
TRACE_MSG(SNDCP_SDL_726_112_2_17_22_58_4_4321,"SNDCP_HandleUnsequentUnackBuffer reset_flag = %d, seg_count = %d")
TRACE_MSG(SNDCP_SDL_789_112_2_17_22_58_4_4322,"SNDCP: first segment")
TRACE_MSG(SNDCP_SDL_913_112_2_17_22_58_4_4323,"SNDCP: SNDCP_HandleTllSndcUnitdataInd receiving_state = %d")
TRACE_MSG(SNDCP_SDL_969_112_2_17_22_58_4_4324,"SNDCP: DATA-SNPDU F field set in receive subsequent state.\n")
TRACE_MSG(SNDCP_SNPDU_447_112_2_17_22_58_5_4325,"SNDCP:invalid nsapi %d")
TRACE_MSG(SNDCP_SNPDU_464_112_2_17_22_58_5_4326,"SNDCP:invalid sapi %d")
TRACE_MSG(XID_SDL_428_112_2_17_22_58_8_4327,"SNDCP: RenegotiateReq result = %d")
TRACE_MSG(SS_EVENT_45_112_2_17_22_58_10_4328,"SS_TASK: send MNSS_BEGIN_IND to task id =%d.")
TRACE_MSG(SS_EVENT_70_112_2_17_22_58_10_4329,"SS_TASK: send MNSS_FACILITY_IND to task id =%d.")
TRACE_MSG(SS_EVENT_102_112_2_17_22_58_11_4330,"SS_TASK: send MNSS_END_IND to task id =%d.")
TRACE_MSG(SS_MAIN_567_112_2_17_22_58_12_4331,"SS_TASK: ti=%d entity is present")
TRACE_MSG(SS_MAIN_571_112_2_17_22_58_12_4332,"SS_TASK: ti=%d entity is not present")
TRACE_MSG(SS_MAIN_585_112_2_17_22_58_12_4333,"SS_TASK: initial value")
TRACE_MSG(SS_MAIN_619_112_2_17_22_58_12_4334,"SS_TASK: receive SS_MM_EST_IND")
TRACE_MSG(SS_MAIN_623_112_2_17_22_58_12_4335,"SS_TASK: receive SS_MM_EST_CNF")
TRACE_MSG(SS_MAIN_627_112_2_17_22_58_12_4336,"SS_TASK: receive SS_MM_REL_IND")
TRACE_MSG(SS_MAIN_632_112_2_17_22_58_12_4337,"SS_TASK: receive SS_MM_DATA_IND")
TRACE_MSG(SS_MAIN_636_112_2_17_22_58_12_4338,"SS_TASK: receive SS_MM_ERR_IND")
TRACE_MSG(SS_MAIN_648_112_2_17_22_58_12_4339,"SS_TASK: receive MNSS_BEGIN_REQ")
TRACE_MSG(SS_MAIN_652_112_2_17_22_58_12_4340,"SS_TASK: receive MNSS_FACILITY_REQ")
TRACE_MSG(SS_MAIN_656_112_2_17_22_58_12_4341,"SS_TASK: receive MNSS_END_REQ")
TRACE_MSG(SS_MAIN_660_112_2_17_22_58_12_4342,"SS_TASK: diacard received signals")
TRACE_MSG(SS_SIGNAL_46_112_2_17_22_58_14_4343,"SS_TASK: send CM_MM_DATA_REQ to task id =%d.")
TRACE_MSG(SS_SIGNAL_68_112_2_17_22_58_14_4344,"SS_TASK: send CM_MM_REL_REQ to task id =%d.")
TRACE_MSG(SS_SIGNAL_94_112_2_17_22_58_14_4345,"SS_TASK: send CM_MM_EST_REQ to task id =%d.")
TRACE_MSG(SIM_DIALING_NUMBER_433_112_2_17_22_58_17_4346,"SIM: EXT1 exceed max %d, %d")
TRACE_MSG(SIM_DIALING_NUMBER_451_112_2_17_22_58_17_4347,"SIM: EXT2 exceed max %d, %d ")
TRACE_MSG(SIM_DIALING_NUMBER_470_112_2_17_22_58_17_4348,"SIM: EXT3 exceed max %d, %d ")
TRACE_MSG(SIM_DIALING_NUMBER_636_112_2_17_22_58_18_4349,"SIM::record number of extfile 0x%x is %d")
TRACE_MSG(SIM_DIALING_NUMBER_686_112_2_17_22_58_18_4350,"SIM::dn_id 0x%x is related with ext_id 0x%x")
TRACE_MSG(SIM_DIALING_NUMBER_703_112_2_17_22_58_18_4351,"SIM::next id of ext_id 0x%x is 0x%x")
TRACE_MSG(SIM_DIALING_NUMBER_711_112_2_17_22_58_18_4352,"SIM::next id of ext_id 0x%x is null")
TRACE_MSG(SIM_DIALING_NUMBER_729_112_2_17_22_58_18_4353,"SIM::No.0x%x record of the ext file is free")
TRACE_MSG(SIM_DIALING_NUMBER_736_112_2_17_22_58_18_4354,"SIM::update the record to free is failed")
TRACE_MSG(SIM_DIALING_NUMBER_952_112_2_17_22_58_18_4355,"SIM%d:DN EXT found multi dn_id %d use ext_id %d. %d")
TRACE_MSG(SIM_DIALING_NUMBER_985_112_2_17_22_58_18_4356,"SIM%d:related DN EXT found multi dn_id %d use ext_id %d. %d")
TRACE_MSG(SIM_DIALING_NUMBER_1066_112_2_17_22_58_19_4357,"SIM%d DN read started %d,")
TRACE_MSG(SIM_DIALING_NUMBER_1199_112_2_17_22_58_19_4358,"SIM::read ext_id:%d failed when read DN,result is %d.")
TRACE_MSG(SIM_DIALING_NUMBER_1308_112_2_17_22_58_19_4359,"SIM%d DN_%d read finished %d, ext_num is %d, ext_id %d")
TRACE_MSG(SIM_DIALING_NUMBER_1346_112_2_17_22_58_19_4360,"GetExtInfoPtr dn_type = %d")
TRACE_MSG(SIM_DIALING_NUMBER_1433_112_2_17_22_58_20_4361,"SIM%d: -- EXT %d")
TRACE_MSG(SIM_DIALING_NUMBER_1437_112_2_17_22_58_20_4362,"SIM%d:DN delete dn_id_%d 's ext rec as 0xFF")
TRACE_MSG(SIM_DIALING_NUMBER_1448_112_2_17_22_58_20_4363,"SIM%d: -- EXT recover %d")
TRACE_MSG(SIM_DIALING_NUMBER_1525_112_2_17_22_58_20_4364,"SIM%d: ++ EXT %d")
TRACE_MSG(SIM_DIALING_NUMBER_1651_112_2_17_22_58_20_4365,"SIM: update DN EXT test. %d")
TRACE_MSG(SIM_DIALING_NUMBER_2706_112_2_17_22_58_22_4366,"SIM::enter SIMDN_GetFreeExtInfo")
TRACE_MSG(SIM_DIALING_NUMBER_2738_112_2_17_22_58_22_4367,"SIM::total record number of extfile 0x%x is %d")
TRACE_MSG(SIM_DIALING_NUMBER_2803_112_2_17_22_58_22_4368,"SIM::seek result = 0x%x")
TRACE_MSG(SIM_DIALING_NUMBER_2848_112_2_17_22_58_22_4369,"SIM::result is 0x%x,free ext record number is 0x%x")
TRACE_MSG(SIM_DIALING_NUMBER_2950_112_2_17_22_58_23_4370,"SIM%d:DN_SetIsADNinDFteleDFpb ADN 0x%x, EXT1 0x%x. %d sets")
TRACE_MSG(SIM_DIALING_NUMBER_3050_112_2_17_22_58_23_4371,"SIM%d DN GetDNFileName 0x%x, remap from %d to %d")
TRACE_MSG(SIM_DIALING_NUMBER_3112_112_2_17_22_58_23_4372,"SIM%d: PBR rec1 Invalid ADN file id 0x%x!!!")
TRACE_MSG(SIM_DIALING_NUMBER_3123_112_2_17_22_58_23_4373,"SIM%d: PBR rec%d Invalid ADN file id 0x%x!!!")
TRACE_MSG(SIM_DIALING_NUMBER_3193_112_2_17_22_58_23_4374,"SIMDN_GetUSIMADNRecordInfomation, 1st ADN has %d rec, max rec%d. %d %d %d %d")
TRACE_MSG(SIM_DIALING_NUMBER_3232_112_2_17_22_58_23_4375,"SIMDN_GetUSIMADNRecordInfomation, 2nd ADN has %d rec, max rec%d. %d %d %d %d")
TRACE_MSG(SIM_DIALING_NUMBER_3423_112_2_17_22_58_24_4376,"SIMDN%d: search the empty record number is %d")
TRACE_MSG(SIM_EF_DECODE_1826_112_2_17_22_58_28_4377,"SIM%d:DecodeELP 0x%x 0x%x")
TRACE_MSG(SIM_EF_DECODE_1871_112_2_17_22_58_28_4378,"SIM%d:DecodeLP 0x%x")
TRACE_MSG(SIM_EF_DECODE_1914_112_2_17_22_58_28_4379,"SIM::The IMSI contains 8 bytes of 0xFF.")
TRACE_MSG(SIM_EF_DECODE_1935_112_2_17_22_58_28_4380,"SIM::ss imsi The MCC&MNC are 0xFF.")
TRACE_MSG(SIM_EF_DECODE_2158_112_2_17_22_58_28_4381,"SIM%d:DecodePLMNsel i=%d, 0x%x 0x%x 0x%x")
TRACE_MSG(SIM_EF_DECODE_2225_112_2_17_22_58_28_4382,"SIM::warning,the len of EFacmmax is less than 3.")
TRACE_MSG(SIM_EF_DECODE_2261_112_2_17_22_58_28_4383,"SIM::warning,the len of EFsst is less than 2.")
TRACE_MSG(SIM_EF_DECODE_2281_112_2_17_22_58_28_4384,"SIM%d SST len %d, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x")
TRACE_MSG(SIM_EF_DECODE_2350_112_2_17_22_58_29_4385,"SIM::warning,the len of EFacm is less than 3.")
TRACE_MSG(SIM_EF_DECODE_2383_112_2_17_22_58_29_4386,"SIM::warning,the len of the GID1 is out of range.")
TRACE_MSG(SIM_EF_DECODE_2415_112_2_17_22_58_29_4387,"SIM::warning,the len of the GID2 is out of range.")
TRACE_MSG(SIM_EF_DECODE_2446_112_2_17_22_58_29_4388,"SIM::warning,the len of EFpuct is less than 5.")
TRACE_MSG(SIM_EF_DECODE_2525_112_2_17_22_58_29_4389,"SIM::warning the len of the EFspn is %d.")
TRACE_MSG(SIM_EF_DECODE_2599_112_2_17_22_58_29_4390,"SIM::warning,the len of EFbcch is less than 16.")
TRACE_MSG(SIM_EF_DECODE_2629_112_2_17_22_58_29_4391,"SIM::warning,the len of EFpuct is less than 2.")
TRACE_MSG(SIM_EF_DECODE_2700_112_2_17_22_58_29_4392,"SIM: DecodeFPLMN data_len = %d, max_num = %d.")
TRACE_MSG(SIM_EF_DECODE_2727_112_2_17_22_58_29_4393,"SIM::warning,the len of EFloci is not 11.")
TRACE_MSG(SIM_EF_DECODE_2754_112_2_17_22_58_29_4394,"SIM::DecodeLOCI mcc-mnc is 0xff 0xff 0xff.")
TRACE_MSG(SIM_EF_DECODE_2796_112_2_17_22_58_30_4395,"SIM::DecodeLOCI update_status byte is 0x%x and is invalid.")
TRACE_MSG(SIM_EF_DECODE_2827_112_2_17_22_58_30_4396,"SIM::warning,the len of EFad is less than 3.")
TRACE_MSG(SIM_EF_DECODE_2914_112_2_17_22_58_30_4397,"SIM::warning,the len of EFphase is less than 1.")
TRACE_MSG(SIM_EF_DECODE_3361_112_2_17_22_58_31_4398,"SIM::warning,the len of EFdck is less than 16.")
TRACE_MSG(SIM_EF_DECODE_3524_112_2_17_22_58_31_4399,"SIM::warning,the len of EFkcgprs is not 9.")
TRACE_MSG(SIM_EF_DECODE_3561_112_2_17_22_58_31_4400,"SIM::warning,the len of EFlocigprs is not 14.")
TRACE_MSG(SIM_EF_DECODE_3613_112_2_17_22_58_31_4401,"SIM::DecodeLOCIGPRS mcc-mnc is 0xff 0xff 0xff.")
TRACE_MSG(SIM_EF_DECODE_3787_112_2_17_22_58_32_4402,"MM:The length of the network name is too long")
TRACE_MSG(SIM_EF_DECODE_3861_112_2_17_22_58_32_4403,"MM:The length of the network name is too long")
TRACE_MSG(SIM_EF_DECODE_3986_112_2_17_22_58_32_4404,"SIM::the alpha id len is bigger than 60.")
TRACE_MSG(SIM_EF_DECODE_4529_112_2_17_22_58_33_4405,"SIM::warning:the length of the EFccp is %d.fill data in it.")
TRACE_MSG(SIM_EF_DECODE_4598_112_2_17_22_58_33_4406,"SIM::warning,the len of EFsmsp is less than 28.")
TRACE_MSG(SIM_EF_DECODE_4632_112_2_17_22_58_33_4407,"SIM::warning the alpha id len of the EFsmsp is %d.")
TRACE_MSG(SIM_EF_DECODE_4744_112_2_17_22_58_34_4408,"SIM::warning,the len of EFsmss is less than 2.")
TRACE_MSG(SIM_EF_DECODE_4819_112_2_17_22_58_34_4409,"SIM::warning,the len of EFsmsr is less than 30.")
TRACE_MSG(SIM_EF_DECODE_4885_112_2_17_22_58_34_4410,"SIM::warning,the len of EFext1 is less than 13.")
TRACE_MSG(SIM_EF_DECODE_4981_112_2_17_22_58_34_4411,"SIM::warning the alpha id len of the EFlnd is %d.")
TRACE_MSG(SIM_EF_DECODE_5178_112_2_17_22_58_34_4412,"SIM::decode mcc&mnc error.")
TRACE_MSG(SIM_EF_DECODE_5360_112_2_17_22_58_35_4413,"SIM:: DecodeCPHSInfo, data_len is %d, 0x%x 0x%x 0x%x")
TRACE_MSG(SIM_EF_DECODE_5418_112_2_17_22_58_35_4414,"SIM:: DecodeVoiceMsgWtFlag, data_len is %d ")
TRACE_MSG(SIM_EF_DECODE_5507_112_2_17_22_58_35_4415,"SIM::the alpha id len is bigger than 20.")
TRACE_MSG(SIM_EF_DECODE_5611_112_2_17_22_58_35_4416,"SIM::the alpha id len is bigger than 20.")
TRACE_MSG(SIM_EF_DECODE_5641_112_2_17_22_58_35_4417,"SIM::the alpha id len is bigger than 20.")
TRACE_MSG(SIM_EF_DECODE_5960_112_2_17_22_58_36_4418,"SIM%d:DecodePLMNWACT i=%d, 0x%x 0x%x 0x%x")
TRACE_MSG(SIM_EF_DECODE_6065_112_2_17_22_58_36_4419,"SIM%d:DecodeOPLMNWACT i=%d, 0x%x 0x%x 0x%x")
TRACE_MSG(SIM_EF_DECODE_6573_112_2_17_22_58_37_4420,"SIM%d UST len %d, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x")
TRACE_MSG(SIM_EF_DECODE_7275_112_2_17_22_58_39_4421,"SIM::warning,the len of EFpsloci is not 14.")
TRACE_MSG(SIM_EF_DECODE_7325_112_2_17_22_58_39_4422,"SIM::DecodeUsimPSLOCI mcc-mnc is 0xff 0xff 0xff.")
TRACE_MSG(SIM_EF_DECODE_7477_112_2_17_22_58_39_4423,"SIM PBR 2  0x%x, %d %d, len %d")
TRACE_MSG(SIM_EF_DECODE_7538_112_2_17_22_58_39_4424,"Decode EHPLMN")
TRACE_MSG(SIM_EF_DECODE_7676_112_2_17_22_58_40_4425,"SIM::warning,the len of EFcst is 0")
TRACE_MSG(SIM_EF_ENCODE_4031_112_2_17_22_58_49_4426,"SIM::EncodeSMS,Unrecognized sms_status=%d received.")
TRACE_MSG(SIM_EF_ENCODE_4796_112_2_17_22_58_50_4427,"SIM:: EncodeCCFlag, data_len is %d ")
TRACE_MSG(SIM_EF_ENCODE_4857_112_2_17_22_58_50_4428,"SIM:: EncodeVoiceMsgWtFlag, data_len is %d ")
TRACE_MSG(SIM_EF_ENCODE_4906_112_2_17_22_58_50_4429,"SIM:: call func EncodeCSP()")
TRACE_MSG(SIM_EF_ENCODE_5030_112_2_17_22_58_51_4430,"SIM:: EncodeCSP, data_len is %d ")
TRACE_MSG(SIM_EF_ENCODE_6421_112_2_17_22_58_54_4431,"SIM:: EncodeCSP, data_len is %d ")
TRACE_MSG(SIM_FILE_SYSTEM_1240_112_2_17_22_58_57_4432,"SIM%d: remap EF %d to %d")
TRACE_MSG(SIM_HAL_1008_112_2_17_22_59_0_4433,"SIMHAL%d: init DMA")
TRACE_MSG(SIM_HAL_1385_112_2_17_22_59_0_4434,"SIMHAL%d::Wrong procedure byte 0x%x")
TRACE_MSG(SIM_HAL_2045_112_2_17_22_59_2_4435,"SIMHAL%d::DMA RX T.O, %d 0x%x 0x%x. %d %d %d %d. i %d")
TRACE_MSG(SIM_HAL_2049_112_2_17_22_59_2_4436,"SIMHAL%d::DMA RX T.O, %d 0x%x 0x%x. %d %d %d %d. i %d")
TRACE_MSG(SIM_HAL_2086_112_2_17_22_59_2_4437,"SIMHAL%d:DMA got the last 1 byte 0x%x")
TRACE_MSG(SIM_HAL_2306_112_2_17_22_59_2_4438,"SIMHAL:do not deal active done Int")
TRACE_MSG(SIM_HAL_2565_112_2_17_22_59_3_4439,"SIMHAL0::Reset wait ldo timer expired. %d")
TRACE_MSG(SIM_HAL_2635_112_2_17_22_59_3_4440,"SIMHAL1::Reset wait ldo timer expired.")
TRACE_MSG(SIM_HAL_2959_112_2_17_22_59_4_4441,"SIMHAL%d  %d:ReportCardStatus %d")
TRACE_MSG(SIM_HAL_2988_112_2_17_22_59_4_4442,"SIMHAL%d::SIM_ResetDriver %d. %d %d %d %d %d %d")
TRACE_MSG(SIM_HAL_2992_112_2_17_22_59_4_4443,"SIMHAL%d::SIM_ResetDriver %d. %d %d %d %d, %d %d %d %d")
TRACE_MSG(SIM_HAL_2995_112_2_17_22_59_4_4444,"SIMHAL%d::SIM_ResetDriver %d. %d %d %d %d")
TRACE_MSG(SIM_HAL_3085_112_2_17_22_59_4_4445,"SIMHAL0::Init wait ldo timer expired. %d %d")
TRACE_MSG(SIM_HAL_3152_112_2_17_22_59_4_4446,"SIMHAL%d::Init wait ldo timer expired.")
TRACE_MSG(SIM_HAL_3463_112_2_17_22_59_5_4447,"SIMHAL%d::rx parity error!")
TRACE_MSG(SIM_HAL_3482_112_2_17_22_59_5_4448,"SIMHAL%d::rx parity error!")
TRACE_MSG(SIM_HAL_3499_112_2_17_22_59_5_4449,"SIMHAL%d::TxParityErrorIntP")
TRACE_MSG(SIM_HAL_3553_112_2_17_22_59_5_4450,"SIMHAL::DMA Card Unresponse Int")
TRACE_MSG(SIM_HAL_3559_112_2_17_22_59_5_4451,"SIMHAL%d::CardUnResponseIntP")
TRACE_MSG(SIM_HAL_3794_112_2_17_22_59_5_4452,"SIMHAL%d::Tx fifo not empty")
TRACE_MSG(SIM_HAL_3805_112_2_17_22_59_5_4453,"SIMHAL%d:cmd 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x")
TRACE_MSG(SIM_HAL_3809_112_2_17_22_59_5_4454,"SIMHAL%d:cmd 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x")
TRACE_MSG(SIM_HAL_3948_112_2_17_22_59_6_4455,"SIMHAL%d: status words 0x%02x,0x%02x")
TRACE_MSG(SIM_HAL_4219_112_2_17_22_59_6_4456,"SIMHAL%d: set volt %d")
TRACE_MSG(SIM_HAL_4233_112_2_17_22_59_6_4457,"SIMHAL%d: set volt %d")
TRACE_MSG(SIM_HAL_4349_112_2_17_22_59_6_4458,"SIMHAL%d::Reset the SIM card!%d")
TRACE_MSG(SIM_HAL_4429_112_2_17_22_59_7_4459,"SIMHAL::Reset first time!%d")
TRACE_MSG(SIM_HAL_4450_112_2_17_22_59_7_4460,"SIMHAL%d::Reset to change power.")
TRACE_MSG(SIM_HAL_4468_112_2_17_22_59_7_4461,"SIMHAL%d::Reject for no next available volt. %d")
TRACE_MSG(SIM_HAL_4476_112_2_17_22_59_7_4462,"SIMHAL%d::Reset %d time for power change.")
TRACE_MSG(SIM_HAL_4481_112_2_17_22_59_7_4463,"SIMHAL%d::Reset to change bit convention! %d")
TRACE_MSG(SIM_HAL_4495_112_2_17_22_59_7_4464,"SIMHAL%d::Reset to change parity check! %d")
TRACE_MSG(SIM_HAL_4509_112_2_17_22_59_7_4465,"ResetSIMCard reset_type = %d")
TRACE_MSG(SIM_HAL_4950_112_2_17_22_59_8_4466,"SIMHAL%d:RejectSIMCard 0x%x")
TRACE_MSG(SIM_HAL_5300_112_2_17_22_59_8_4467,"SIMHAL%d:high TA1 0x%x")
TRACE_MSG(SIM_HAL_5404_112_2_17_22_59_8_4468,"SIMHAL%d:atr 1.8V only is %d, 0x%x")
TRACE_MSG(SIM_HAL_5441_112_2_17_22_59_9_4469,"SIMHAL%d::PPS to sim 0x%x 0x%x 0x%x 0x%x")
TRACE_MSG(SIM_HAL_5463_112_2_17_22_59_9_4470,"SIMHAL%d::enable the seh function for b5 and higher version chip!0x%x")
TRACE_MSG(SIM_HAL_5522_112_2_17_22_59_9_4471,"SIMHAL%d::SIM DMA enable!0x%x.")
TRACE_MSG(SIM_HAL_5597_112_2_17_22_59_9_4472,"SIMHAL%d:unexpected rx. %d")
TRACE_MSG(SIM_HAL_6019_112_2_17_22_59_10_4473,"SIMHAL%d::Power Off SIM card. %d %d %d")
TRACE_MSG(SIM_HAL_6021_112_2_17_22_59_10_4474,"SIMHAL%d::Power Off SIM card. %d %d")
TRACE_MSG(SIM_HAL_6249_112_2_17_22_59_10_4475,"SIMHAL%d:switch to sim%d. 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x.")
TRACE_MSG(SIM_HAL_6254_112_2_17_22_59_10_4476,"SIMHAL%d:switch to sim%d. 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x. %d %d")
TRACE_MSG(SIM_HAL_7231_112_2_17_22_59_12_4477,"SIMHAL%d:after select. interface_index %d... %d %d... %d %d")
TRACE_MSG(SIM_HAL_7259_112_2_17_22_59_12_4478,"SIMHAL%d:Set RST HIGH 0x%x 0x%x 0x%x")
TRACE_MSG(SIM_HAL_7261_112_2_17_22_59_12_4479,"SIMHAL%d:Set RST HIGH 0x%x 0x%x 0x%x 0x%x")
TRACE_MSG(SIM_HAL_7263_112_2_17_22_59_12_4480,"SIMHAL%d:Set RST HIGH 0x%x 0x%x")
TRACE_MSG(SIM_HAL_7407_112_2_17_22_59_13_4481,"SIMHAL%d:SIM1_RST_After_Tb_or_Te. 0x%x")
TRACE_MSG(SIM_HAL_7409_112_2_17_22_59_13_4482,"SIMHAL%d:SIM1_RST_After_Tb_or_Te.")
TRACE_MSG(SIM_HAL_7523_112_2_17_22_59_13_4483,"SIMHAL1:SIM2_RST_After_Tb_or_Te")
TRACE_MSG(SIM_HAL_7804_112_2_17_22_59_13_4484,"SIMHAL:SPI init")
TRACE_MSG(SIM_HAL_8029_112_2_17_22_59_14_4485,"SIMHAL%d:SPI to 6302 reg%d, value 0x%x")
TRACE_MSG(SIM_HAL_8065_112_2_17_22_59_14_4486,"SIMHAL%d:SPI 1st write 0x%x")
TRACE_MSG(SIM_HAL_8082_112_2_17_22_59_14_4487,"SIMHAL%d:SPI%d to 6303 reg%d, value 0x%x.... RST0x%x CLK0x%x DATA0x%x VSIM0x%x, RST0x%x CLK0x%x DATA0x%x VSIM0x%x, RST0x%x CLK0x%x DATA0x%x VSIM0x%x")
TRACE_MSG(SIM_HAL_8109_112_2_17_22_59_14_4488,"SIMHAL%d:SPI to 6302 reg%d, value 0x%x.... RST0x%x CLK0x%x DATA0x%x VSIM0x%x, RST0x%x CLK0x%x DATA0x%x VSIM0x%x")
TRACE_MSG(SIM_HAL_8495_112_2_17_22_59_15_4489,"SIMHAL_SIM0_Plugin_GPIO_Callback %d")
TRACE_MSG(SIM_HAL_8565_112_2_17_22_59_15_4490,"SIMHAL_SIM0_Hotswap_Stable_TimeOut %d")
TRACE_MSG(SIM_HAL_8612_112_2_17_22_59_15_4491,"SIMCard Status: generated SIMCard code=0x%x")
TRACE_MSG(SIM_HAL_8654_112_2_17_22_59_15_4492,"SIMHAL%d::Rx data buf(1-16):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x n")
TRACE_MSG(SIM_HAL_8681_112_2_17_22_59_15_4493,"SIMHAL%d::Tx total len:%d,Sent len:%d,Tx buf(1-16):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x n")
TRACE_MSG(SIM_HAL_8699_112_2_17_22_59_15_4494,"SIMHAL::atr data len = %d, data buf(%d-%d):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x n")
TRACE_MSG(SIM_HAL_8706_112_2_17_22_59_15_4495,"SIMHAL::atr data buf(%d-%d):0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x n")
TRACE_MSG(SIM_INSTRUCTION_712_112_2_17_22_59_17_4496,"SIM::Fetch Proactive Command Error!")
TRACE_MSG(SIM_INSTRUCTION_1302_112_2_17_22_59_18_4497,"SIM::Status_word = %d\n")
TRACE_MSG(SIM_INSTRUCTION_2338_112_2_17_22_59_20_4498,"SIM SELECT %d, %d %d, file_route 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x")
TRACE_MSG(SIM_INSTRUCTION_2392_112_2_17_22_59_20_4499,"SIM::sw is 91xx when do select instruction.")
TRACE_MSG(SIM_INSTRUCTION_2666_112_2_17_22_59_21_4500,"SIM::redo the select when sw is 91xx. %d %d... 0x%x")
TRACE_MSG(SIM_INSTRUCTION_2724_112_2_17_22_59_21_4501,"SIM%d:file_name=%d, ins_name=%d, sw1=%x, sw2=%x, data_len=%d, data=%x, %x, %x")
TRACE_MSG(SIM_INSTRUCTION_2740_112_2_17_22_59_21_4502,"SIM%d:get response: ins_name=%d, sw1=%x, sw2=%x, data_len=%d, data=%x, %x, %x")
TRACE_MSG(SIM_INSTRUCTION_2898_112_2_17_22_59_21_4503,"SIM::timer expired and the card do not need waiting.")
TRACE_MSG(SIM_INSTRUCTION_2909_112_2_17_22_59_21_4504,"SIM%d::timer expired but the card need waiting.")
TRACE_MSG(SIM_INSTRUCTION_2916_112_2_17_22_59_21_4505,"SIM%d::timer expired and the card do not need waiting. %d")
TRACE_MSG(SIM_INSTRUCTION_2944_112_2_17_22_59_21_4506,"SIM%d::too many 0x60, do not need waiting. %d")
TRACE_MSG(SIM_INSTRUCTION_2962_112_2_17_22_59_21_4507,"SIM%d::SIMINSTR_ReceiveInstrResponse default %d")
TRACE_MSG(SIM_INSTRUCTION_3006_112_2_17_22_59_21_4508,"SIM%d::Rcv Power off req in Receive Response")
TRACE_MSG(SIM_INSTRUCTION_3113_112_2_17_22_59_22_4509,"SIM%d:AddThisSIGNALInTempSignalQueue 0x%x")
TRACE_MSG(SIM_INSTRUCTION_3186_112_2_17_22_59_22_4510,"SIM::volt 5v ony. 0x%x")
TRACE_MSG(SIM_INSTRUCTION_3190_112_2_17_22_59_22_4511,"SIM::volt 3v tech. 0x%x")
TRACE_MSG(SIM_INSTRUCTION_3194_112_2_17_22_59_22_4512,"SIM::volt 1.8v tech. 0x%x")
TRACE_MSG(SIM_INSTRUCTION_3198_112_2_17_22_59_22_4513,"SIM::volt future class. 0x%x")
TRACE_MSG(SIM_INSTRUCTION_3202_112_2_17_22_59_22_4514,"SIM::volt not correct value. 0x%x")
TRACE_MSG(SIM_INSTRUCTION_3207_112_2_17_22_59_22_4515,"GetCardSupplyVot supply_volt = %d")
TRACE_MSG(SIM_INSTRUCTION_3409_112_2_17_22_59_22_4516,"SIM%d:large transparent EF %d, %d")
TRACE_MSG(SIM_INSTRUCTION_3615_112_2_17_22_59_23_4517,"SIM::SIMINSTR_UpdateEF,ERROR")
TRACE_MSG(SIM_INSTRUCTION_6124_112_2_17_22_59_28_4518,"SIM::SIMINSTR_SelectFileByPath fileid = 0x%x. file_route = 0x%x,0x%x,0x%x")
TRACE_MSG(SIM_INSTRUCTION_6274_112_2_17_22_59_28_4519,"SIM:set cur file id 0x%x.")
TRACE_MSG(SIM_INSTRUCTION_7801_112_2_17_22_59_31_4520,"SIM fcp 0x%x: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x")
TRACE_MSG(SIM_INSTRUCTION_7833_112_2_17_22_59_31_4521,"SIM:SIMINSTR_DecodeUSIMFCP_FileDescriptor,ERROR")
TRACE_MSG(SIM_INSTRUCTION_7892_112_2_17_22_59_31_4522,"SIM:SIMINSTR_DecodeUSIMFCP_LifeCycleStatus,ERROR")
TRACE_MSG(SIM_INSTRUCTION_7904_112_2_17_22_59_31_4523,"SIM:SIMINSTR_DecodeUSIMFCP_SecurityAttributes,ERROR")
TRACE_MSG(SIM_INSTRUCTION_7973_112_2_17_22_59_31_4524,"SIM:SIMINSTR_DecodeUSIMFCP_FileDescriptor,ERROR")
TRACE_MSG(SIM_INSTRUCTION_7986_112_2_17_22_59_31_4525,"SIM:SIMINSTR_DecodeUSIMFCP_FileID,ERROR")
TRACE_MSG(SIM_INSTRUCTION_8005_112_2_17_22_59_31_4526,"SIM:SIMINSTR_DecodeUSIMFCP_LifeCycleStatus,ERROR")
TRACE_MSG(SIM_INSTRUCTION_8017_112_2_17_22_59_31_4527,"SIM:SIMINSTR_DecodeUSIMFCP_SecurityAttributes,ERROR")
TRACE_MSG(SIM_INSTRUCTION_8031_112_2_17_22_59_31_4528,"SIM:SIMINSTR_DecodeUSIMFCP_FileSize,ERROR")
TRACE_MSG(SIM_INSTRUCTION_8063_112_2_17_22_59_31_4529,"SIM:DecodeUSIMFCP final: filesize 0x%x, fileid 0x%x, chara 0x%x, rec_len 0x%x, status 0x%x")
TRACE_MSG(SIM_INSTRUCTION_8212_112_2_17_22_59_32_4530,"SIM EFarr test point2 %d")
TRACE_MSG(SIM_INSTRUCTION_8260_112_2_17_22_59_32_4531,"SIM EFarr decode %d,  0x%x 0x%x")
TRACE_MSG(SIM_MAIN_772_112_2_17_22_59_34_4532,"SIM::DriverNotifySIM, msg->message= %d")
TRACE_MSG(SIM_MAIN_807_112_2_17_22_59_34_4533,"\n\rSIM::SIM Layer Task %d StartUp! %d.")
TRACE_MSG(SIM_MAIN_1331_112_2_17_22_59_35_4534,"SIM%d::NoPowerState enable deepsleep.")
TRACE_MSG(SIM_MAIN_1417_112_2_17_22_59_35_4535,"SIM::No power state received abnormal signal:signal_code =0x%x,sender =0x%x\n")
TRACE_MSG(SIM_MAIN_1624_112_2_17_22_59_36_4536,"SIM%d::NullState enable deepsleep.")
TRACE_MSG(SIM_MAIN_1693_112_2_17_22_59_36_4537,"SIM::Null state received abnormal signal:signal_code =0x%x,sender =0x%x\n")
TRACE_MSG(SIM_MAIN_1838_112_2_17_22_59_36_4538,"SIM%d: from atr class change. %d")
TRACE_MSG(SIM_MAIN_1864_112_2_17_22_59_36_4539,"SIM%d: ATR indicate 1.8V only.")
TRACE_MSG(SIM_MAIN_1944_112_2_17_22_59_36_4540,"SIM%d::card supply volt is:%x")
TRACE_MSG(SIM_MAIN_1972_112_2_17_22_59_37_4541,"SIM::the card is 5v card.")
TRACE_MSG(SIM_MAIN_2023_112_2_17_22_59_37_4542,"SIM%d::rise the working voltage to 3v.")
TRACE_MSG(SIM_MAIN_2051_112_2_17_22_59_37_4543,"SIM%d::the current working voltage is matched with the card supply voltage.")
TRACE_MSG(SIM_MAIN_2060_112_2_17_22_59_37_4544,"\n\rSIM%d::SIM prepare sim card present failed.")
TRACE_MSG(SIM_MAIN_2074_112_2_17_22_59_37_4545,"\n\rSIM%d::SIM Inserted OK!")
TRACE_MSG(SIM_MAIN_2112_112_2_17_22_59_37_4546,"SIM%d: class change %d.")
TRACE_MSG(SIM_MAIN_2171_112_2_17_22_59_37_4547,"\n\rSIM%d::Wrong SIM inserted!")
TRACE_MSG(SIM_MAIN_2237_112_2_17_22_59_37_4548,"SIM%d::ATRState disable deepsleep.")
TRACE_MSG(SIM_MAIN_2290_112_2_17_22_59_37_4549,"SIM::ATR state received abnormal signal:signal_code =0x%x,sender =0x%x\n")
TRACE_MSG(SIM_MAIN_2333_112_2_17_22_59_37_4550,"SIM%d::InitilaizeState disable deepsleep.")
TRACE_MSG(SIM_MAIN_2521_112_2_17_22_59_38_4551,"SIM::Initilaize state received abnormal signal:signal_code =0x%x,sender =0x%x\n")
TRACE_MSG(SIM_MAIN_3492_112_2_17_22_59_40_4552,"SIM::Active state received abnormal signal:signal_code =0x%x,sender =0x%x\n")
TRACE_MSG(SIM_MAIN_3514_112_2_17_22_59_40_4553,"SIM%d::ActiveState enable deepsleep.")
TRACE_MSG(SIM_MAIN_3643_112_2_17_22_59_40_4554,"SIM::Wrong SIM Interrupt Type %d")
TRACE_MSG(SIM_MAIN_3700_112_2_17_22_59_40_4555,"SIM%d::card unresponse before initialization.")
TRACE_MSG(SIM_MAIN_3707_112_2_17_22_59_40_4556,"SIM%d::sim restore count is out of the range.")
TRACE_MSG(SIM_MAIN_3748_112_2_17_22_59_40_4557,"SIM%d::begin to restore the sim card.")
TRACE_MSG(SIM_MAIN_3758_112_2_17_22_59_40_4558,"SIM%d::the card is present.")
TRACE_MSG(SIM_MAIN_3803_112_2_17_22_59_40_4559,"SIM%d:restore fail1. %d")
TRACE_MSG(SIM_MAIN_3828_112_2_17_22_59_40_4560,"SIM%d:restore fail2. %d")
TRACE_MSG(SIM_MAIN_3836_112_2_17_22_59_40_4561,"SIM%d::the card is not present.")
TRACE_MSG(SIM_MAIN_3848_112_2_17_22_59_40_4562,"SIM%d:restore failed T.O. %d")
TRACE_MSG(SIM_MAIN_3856_112_2_17_22_59_41_4563,"SIM%d::the card no response when restore timeout.")
TRACE_MSG(SIM_MAIN_3862_112_2_17_22_59_41_4564,"SIM%d::receive signal 0x%x during restore.")
TRACE_MSG(SIM_MAIN_3956_112_2_17_22_59_41_4565,"SIM%d:cardout %d")
TRACE_MSG(SIM_MAIN_4000_112_2_17_22_59_41_4566,"SIM%d:RX Parity Error")
TRACE_MSG(SIM_MAIN_4041_112_2_17_22_59_41_4567,"SIM%d:TX Parity Error")
TRACE_MSG(SIM_MAIN_4252_112_2_17_22_59_41_4568,"SIM%d::ME is in call,begin to check SIM card every %d ms.")
TRACE_MSG(SIM_MAIN_4303_112_2_17_22_59_41_4569,"SIM%d::check card present.")
TRACE_MSG(SIM_MAIN_4317_112_2_17_22_59_41_4570,"SIM%d::Card not present because the current directory is 0x%x!")
TRACE_MSG(SIM_MAIN_4346_112_2_17_22_59_41_4571,"SIM%d::CheckCardPresent,Card not present because the Status instruction is not ok\n")
TRACE_MSG(SIM_MAIN_4372_112_2_17_22_59_42_4572,"SIM%d:Card unresponse!")
TRACE_MSG(SIM_MAIN_4402_112_2_17_22_59_42_4573,"SIM%d:Card give no any response during get the gsm data in 3s!")
TRACE_MSG(SIM_MAIN_4406_112_2_17_22_59_42_4574,"SIM%d:Card has give part response,wait until 100s timer expired!")
TRACE_MSG(SIM_MAIN_4415_112_2_17_22_59_42_4575,"SIM:Card give no any response during get the gsm data in 3s!")
TRACE_MSG(SIM_MAIN_4592_112_2_17_22_59_42_4576,"SIM_IsCallControlSupport result = %d")
TRACE_MSG(SIM_MAIN_4853_112_2_17_22_59_42_4577,"SIM%d: this is a USIM. AID len%d, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x")
TRACE_MSG(SIM_MAIN_4861_112_2_17_22_59_42_4578,"SIM%d: this is a USIM card")
TRACE_MSG(SIM_MAIN_4866_112_2_17_22_59_42_4579,"SIM%d: this is a SIM card")
TRACE_MSG(SIM_MAIN_6567_112_2_17_22_59_46_4580,"SIM::mutex operate error.")
TRACE_MSG(SIM_MAIN_6572_112_2_17_22_59_46_4581,"SIM::task %d take the sem.")
TRACE_MSG(SIM_MAIN_6639_112_2_17_22_59_46_4582,"SIM::task %d put the sem.")
TRACE_MSG(SIM_MAIN_6642_112_2_17_22_59_46_4583,"SIM::task %d put the sem.")
TRACE_MSG(SIM_MAIN_6651_112_2_17_22_59_46_4584,"SIM::task %d put the sem.")
TRACE_MSG(SIM_MAIN_6673_112_2_17_22_59_46_4585,"SIM%d:MAIN_Quadsim_CheckDetect %d.")
TRACE_MSG(SIM_MM_SIGNAL_410_112_2_17_22_59_47_4586,"SIM%d::Read KcGPRS from NV temply.")
TRACE_MSG(SIM_MM_SIGNAL_422_112_2_17_22_59_47_4587,"SIM::Read KcGPRS from NV.")
TRACE_MSG(SIM_MM_SIGNAL_446_112_2_17_22_59_47_4588,"SIM::Read LociGPRS from NV.")
TRACE_MSG(SIM_MM_SIGNAL_467_112_2_17_22_59_47_4589,"SIM:no valid rai using lai !")
TRACE_MSG(SIM_MM_SIGNAL_479_112_2_17_22_59_47_4590,"SIM:read KcGPRS and LociGPRS infor from NV!")
TRACE_MSG(SIM_MM_SIGNAL_564_112_2_17_22_59_48_4591,"SIM::begin to re-select the file")
TRACE_MSG(SIM_MM_SIGNAL_575_112_2_17_22_59_48_4592,"SIM::select file failed before mm auth. %d")
TRACE_MSG(SIM_MM_SIGNAL_620_112_2_17_22_59_48_4593,"SIM::Run gsm algorithm failed in mm auth. %d")
TRACE_MSG(SIM_MM_SIGNAL_651_112_2_17_22_59_48_4594,"SIM::update kc failed in mm auth.")
TRACE_MSG(SIM_MM_SIGNAL_820_112_2_17_22_59_48_4595,"SIM::select kc file failed in Gmm auth.")
TRACE_MSG(SIM_MM_SIGNAL_865_112_2_17_22_59_48_4596,"SIM::Run gsm algorithm failed in Gmm auth.")
TRACE_MSG(SIM_MM_SIGNAL_898_112_2_17_22_59_48_4597,"SIM::Update Kcgprs of SIM card failed in Gmm auth for result 0x%x.")
TRACE_MSG(SIM_MM_SIGNAL_928_112_2_17_22_59_48_4598,"SIM::Update Kcgprs of NV failed in Gmm auth.")
TRACE_MSG(SIM_MM_SIGNAL_973_112_2_17_22_59_48_4599,"SIM::Save RAI information result is:0x%x.")
TRACE_MSG(SIM_MM_SIGNAL_993_112_2_17_22_59_48_4600,"SIM::Save RAI failed for result is:0x%x.")
TRACE_MSG(SIM_MM_SIGNAL_1036_112_2_17_22_59_48_4601,"SIM::Save Cipher information result is:0x%x.")
TRACE_MSG(SIM_SER_PROCEDURE_148_112_2_17_22_59_49_4602,"SIM: re-do increase")
TRACE_MSG(SIM_SER_PROCEDURE_195_112_2_17_22_59_49_4603,"SIM::SIMSER_ReadWholeTransparentEF,ERROR.")
TRACE_MSG(SIM_SER_PROCEDURE_239_112_2_17_22_59_49_4604,"SIM: re-do read binary")
TRACE_MSG(SIM_SER_PROCEDURE_282_112_2_17_22_59_49_4605,"SIM::SIMSER_ReadEFRecord,ERROR.")
TRACE_MSG(SIM_SER_PROCEDURE_328_112_2_17_22_59_50_4606,"SIM: re-do ef record")
TRACE_MSG(SIM_SER_PROCEDURE_372_112_2_17_22_59_50_4607,"SIM::SIMSER_SeekInFile,ERROR.")
TRACE_MSG(SIM_SER_PROCEDURE_431_112_2_17_22_59_50_4608,"SIM: re-do seek ef record")
TRACE_MSG(SIM_SER_PROCEDURE_469_112_2_17_22_59_50_4609,"SIM::SIMSER_UpdateWholeTransparentEF,ERROR.")
TRACE_MSG(SIM_SER_PROCEDURE_521_112_2_17_22_59_50_4610,"SIM: re-do update t ef record")
TRACE_MSG(SIM_SER_PROCEDURE_565_112_2_17_22_59_50_4611,"SIM::SIMSER_UpdateEFRecord,ERROR.")
TRACE_MSG(SIM_SER_PROCEDURE_760_112_2_17_22_59_50_4612,"SIM::SIMSER_GetRecordInfo,ERROR.")
TRACE_MSG(SIM_SER_PROCEDURE_891_112_2_17_22_59_51_4613,"SIM::SIMSER_ReadWholeTransparentEF_static,ERROR.")
TRACE_MSG(SIM_SER_PROCEDURE_941_112_2_17_22_59_51_4614,"SIM::SIMSER_ReadEFRecord_static,ERROR.")
TRACE_MSG(SIM_SER_PROCEDURE_990_112_2_17_22_59_51_4615,"SIM::SIMSER_UpdateWholeTransparentEF_static,ERROR.")
TRACE_MSG(SIM_SER_PROCEDURE_1039_112_2_17_22_59_51_4616,"SIM::SIMSER_UpdateEFRecord_static,ERROR.")
TRACE_MSG(SIM_SIGNAL_994_112_2_17_22_59_54_4617,"SIM: Dphone refresh timeout!")
TRACE_MSG(SIM_SIGNAL_2512_112_2_17_22_59_57_4618,"SIM:: select DFcdma ser_result = %d")
TRACE_MSG(SIM_SIGNAL_2538_112_2_17_22_59_57_4619,"SIM:: select DFgsm ser_result = %d")
TRACE_MSG(SIM_SIGNAL_2568_112_2_17_22_59_57_4620,"SIM::select imsi failed.")
TRACE_MSG(SIM_SIGNAL_2586_112_2_17_22_59_57_4621,"SIM:: EFimsi ser_result = %d, msin = %d, %d, %d")
TRACE_MSG(SIM_SIGNAL_2602_112_2_17_22_59_57_4622,"SIM::the imsi file is invalidate.")
TRACE_MSG(SIM_SIGNAL_2671_112_2_17_22_59_57_4623,"SIM: g_sim_card_state = %d")
TRACE_MSG(SIM_SIGNAL_2692_112_2_17_22_59_57_4624,"SIM:: select DFcdma ser_result = %d")
TRACE_MSG(SIM_SIGNAL_2719_112_2_17_22_59_57_4625,"SIM::no EFimsi in the DFcdma.")
TRACE_MSG(SIM_SIGNAL_2726_112_2_17_22_59_57_4626,"SIM::select EFimsi in the DFcdma failed.")
TRACE_MSG(SIM_SIGNAL_2744_112_2_17_22_59_57_4627,"SIM::read cdma imsi failed.")
TRACE_MSG(SIM_SIGNAL_2751_112_2_17_22_59_57_4628,"SIM::read cdma imsi OK.")
TRACE_MSG(SIM_SIGNAL_2760_112_2_17_22_59_57_4629,"SIM::the EFimsi of the DFcdma is invalidate.")
TRACE_MSG(SIM_SIGNAL_2810_112_2_17_22_59_57_4630,"SIM::Verify CHV1 failed in SIM initialization!")
TRACE_MSG(SIM_SIGNAL_2876_112_2_17_22_59_57_4631,"SIM::SIM initialization success!")
TRACE_MSG(SIM_SIGNAL_2886_112_2_17_22_59_57_4632,"SIM::ss imsi is full 0xff during the initialization. len=%d")
TRACE_MSG(SIM_SIGNAL_2895_112_2_17_22_59_57_4633,"SIM::imsi is full 0xff during the initialization.")
TRACE_MSG(SIM_SIGNAL_2959_112_2_17_22_59_58_4634,"SIM::GetAOCAccessCondition filename = %d, condition = %x")
TRACE_MSG(SIM_SIGNAL_3078_112_2_17_22_59_58_4635,"SIM::Read EFecc failed in Card Presented! %d")
TRACE_MSG(SIM_SIGNAL_3104_112_2_17_22_59_58_4636,"SIM::Read EFphase failed in Card Presented!")
TRACE_MSG(SIM_SIGNAL_3123_112_2_17_22_59_58_4637,"SIM::Read EFad failed in Card Presented!%d")
TRACE_MSG(SIM_SIGNAL_3197_112_2_17_22_59_58_4638,"SIM::Read EFiccid failed in SIM initialization!")
TRACE_MSG(SIM_SIGNAL_3508_112_2_17_22_59_59_4639,"SIM::SIMSIGNAL_CHVRequest,ERROR!")
TRACE_MSG(SIM_SIGNAL_3541_112_2_17_22_59_59_4640,"SIM::VerifyCHV1AtInit,ERROR.")
TRACE_MSG(SIM_SIGNAL_3689_112_2_17_22_59_59_4641,"SIM::read loci failed when init.")
TRACE_MSG(SIM_SIGNAL_3758_112_2_17_23_0_0_4642,"SIM::gid1 not support.")
TRACE_MSG(SIM_SIGNAL_3803_112_2_17_23_0_0_4643,"SIM::gid2 not support.")
TRACE_MSG(SIM_SIGNAL_4242_112_2_17_23_0_0_4644,"SIM:get img record info.")
TRACE_MSG(SIM_SIGNAL_4247_112_2_17_23_0_0_4645,"SIM: img service support.")
TRACE_MSG(SIM_SIGNAL_4253_112_2_17_23_0_0_4646,"SIM: img service not support.")
TRACE_MSG(SIM_SIGNAL_4486_112_2_17_23_0_1_4647,"SIM::Receive SIM_PC_EXIST_IND signal when card not ready!")
TRACE_MSG(SIM_SIGNAL_4491_112_2_17_23_0_1_4648,"SIM::Receive SIM_PC_EXIST_IND signal when card not support STK!")
TRACE_MSG(SIM_SIGNAL_4518_112_2_17_23_0_1_4649,"SIM::Receive terminal response request when card not ready!")
TRACE_MSG(SIM_SIGNAL_4527_112_2_17_23_0_1_4650,"SIM::Receive terminal response request when card not support STK!")
TRACE_MSG(SIM_SIGNAL_4535_112_2_17_23_0_1_4651,"SIM::Terminal response failed!")
TRACE_MSG(SIM_SIGNAL_4570_112_2_17_23_0_1_4652,"SIM::Envelope failed!")
TRACE_MSG(SIM_SIGNAL_4665_112_2_17_23_0_1_4653,"SIM::SIMSIGNAL_ReadFileBufByPath input parameter error!")
TRACE_MSG(SIM_SIGNAL_4676_112_2_17_23_0_1_4654,"SIM:: select MF ser_result = %d")
TRACE_MSG(SIM_SIGNAL_4694_112_2_17_23_0_1_4655,"SIM::SIMSIGNAL_ReadFileBufByPath,select 0x%x failed.")
TRACE_MSG(SIM_SIGNAL_4708_112_2_17_23_0_1_4656,"SIM::SIMSIGNAL_ReadFileBufByPath,reback to MF failed.")
TRACE_MSG(SIM_SIGNAL_4714_112_2_17_23_0_1_4657,"SIM::SIMSIGNAL_ReadFileBufByPath,select 0x%x ok.")
TRACE_MSG(SIM_SIGNAL_4740_112_2_17_23_0_1_4658,"SIM:: SIMINSTR_ImiReadEF, read 0x%x ser_result = %d")
TRACE_MSG(SIM_SIGNAL_4758_112_2_17_23_0_2_4659,"SIM:: SIMSIGNAL_ReadFileBufByPath, reback to MF ser_result = %d")
TRACE_MSG(SIM_SIGNAL_4761_112_2_17_23_0_2_4660,"SIM::SIMSIGNAL_ReadFileBufByPath,reback to MF failed2.")
TRACE_MSG(SIM_SIGNAL_4819_112_2_17_23_0_2_4661,"SIM::Verify CHV1 failed in SIM ReInitialization!")
TRACE_MSG(SIM_SIGNAL_4830_112_2_17_23_0_2_4662,"SIM::Read EFsst Failed in SIM ReInitialization!")
TRACE_MSG(SIM_SIGNAL_4856_112_2_17_23_0_2_4663,"SIM::STK terminal profile fail %d")
TRACE_MSG(SIM_SIGNAL_4860_112_2_17_23_0_2_4664,"SIM%d::SIM ReInitialization success!")
TRACE_MSG(SIM_SIGNAL_4884_112_2_17_23_0_2_4665,"SIM:IsFatalError TRUE")
TRACE_MSG(SIM_SIGNAL_4960_112_2_17_23_0_2_4666,"SIM:: Init_GetSimFileTable Read EFsst Failed %d")
TRACE_MSG(SIM_SIGNAL_4982_112_2_17_23_0_2_4667,"SIM::SIM Application Toolkit terminal profile fail %d")
TRACE_MSG(SIM_SIGNAL_5016_112_2_17_23_0_2_4668,"SIM:: Init_GetSimFileTable Read EFimsi failed %d")
TRACE_MSG(SIM_SIGNAL_5041_112_2_17_23_0_2_4669,"SIM:: Init_GetSimFileTable Read EFacc failed %d")
TRACE_MSG(SIM_SIGNAL_5054_112_2_17_23_0_2_4670,"SIM:: Init_GetSimFileTable Read EFhplmn failed %d")
TRACE_MSG(SIM_SIGNAL_5074_112_2_17_23_0_2_4671,"SIM:: Init_GetSimFileTable Read EFplmnwact failed %d")
TRACE_MSG(SIM_SIGNAL_5107_112_2_17_23_0_2_4672,"SIM:: Init_GetSimFileTable Read EFplmnsel failed %d")
TRACE_MSG(SIM_SIGNAL_5136_112_2_17_23_0_2_4673,"SIM:: Init_GetSimFileTable Read EFoplmnwact failed %d")
TRACE_MSG(SIM_SIGNAL_5163_112_2_17_23_0_2_4674,"SIM:: Init_GetSimFileTable Read EFhplmnwact failed %d")
TRACE_MSG(SIM_SIGNAL_5186_112_2_17_23_0_2_4675,"SIM:: Init_GetSimFileTable Read EFloci failed %d")
TRACE_MSG(SIM_SIGNAL_5203_112_2_17_23_0_2_4676,"SIM:: Init_GetSimFileTable Read EFkc failed %d")
TRACE_MSG(SIM_SIGNAL_5222_112_2_17_23_0_2_4677,"SIM:: Init_GetSimFileTable EFbcch failed %d")
TRACE_MSG(SIM_SIGNAL_5246_112_2_17_23_0_2_4678,"SIM::Init_GetSimFileTable Read EFfplmn failed %d")
TRACE_MSG(SIM_SIGNAL_5311_112_2_17_23_0_3_4679,"SIM:: Init_GetSimFileTable EFspn failed %d")
TRACE_MSG(SIM_SIGNAL_5349_112_2_17_23_0_3_4680,"SIM:: Init_GetSimFileTable EFgid1 failed %d")
TRACE_MSG(SIM_SIGNAL_5375_112_2_17_23_0_3_4681,"SIM:: Init_GetSimFileTable EFgid2 failed %d")
TRACE_MSG(SIM_SIGNAL_5422_112_2_17_23_0_3_4682,"SIM:: Read EFehplmn cause = %d")
TRACE_MSG(SIM_SIGNAL_5431_112_2_17_23_0_3_4683,"SIM:: Read EFehplmnpi cause = %d")
TRACE_MSG(SIM_SIGNAL_5458_112_2_17_23_0_3_4684,"SIM:: CMCC special HPLMN")
TRACE_MSG(SIM_SIGNAL_5487_112_2_17_23_0_3_4685,"SIM::Refresh Ini begin!")
TRACE_MSG(SIM_SIGNAL_5498_112_2_17_23_0_3_4686,"SIM::SIMSIGNAL_RefreshIni Read EFiccid failed!")
TRACE_MSG(SIM_SIGNAL_5506_112_2_17_23_0_3_4687,"SIM::SIMSIGNAL_RefreshIni select DFgsm failed!")
TRACE_MSG(SIM_SIGNAL_5521_112_2_17_23_0_3_4688,"SIM::Read EFphase failed in Refresh Ini!")
TRACE_MSG(SIM_SIGNAL_5544_112_2_17_23_0_3_4689,"SIM::SIMSIGNAL_RefreshIni Read EFad failed!")
TRACE_MSG(SIM_SIGNAL_5558_112_2_17_23_0_3_4690,"SIM::SIM Refresh ini success!FDN %d")
TRACE_MSG(SIM_SIGNAL_5567_112_2_17_23_0_3_4691,"SIM::SIMSIGNAL_RefreshIni imsi is full 0xff.")
TRACE_MSG(SIM_SIGNAL_5597_112_2_17_23_0_3_4692,"SIM::Refresh File change begin!")
TRACE_MSG(SIM_SIGNAL_5643_112_2_17_23_0_3_4693,"SIM::SIMSIGNAL_RefreshFileChg select MF failed.")
TRACE_MSG(SIM_SIGNAL_5648_112_2_17_23_0_3_4694,"SIM::Refresh File change end!")
TRACE_MSG(SIM_SIGNAL_5680_112_2_17_23_0_3_4695,"SIM::Refresh Ini File change begin!")
TRACE_MSG(SIM_SIGNAL_5692_112_2_17_23_0_3_4696,"SIM::SIMSIGNAL_RefreshIniFileChg Read EFiccid failed!")
TRACE_MSG(SIM_SIGNAL_5700_112_2_17_23_0_3_4697,"SIM::SIMSIGNAL_RefreshIniFileChg select DFgsm failed!")
TRACE_MSG(SIM_SIGNAL_5715_112_2_17_23_0_3_4698,"SIM::Read EFphase failed in Refresh Ini!")
TRACE_MSG(SIM_SIGNAL_5738_112_2_17_23_0_3_4699,"SIM::Read EFad failed in SIM initialization!")
TRACE_MSG(SIM_SIGNAL_5752_112_2_17_23_0_4_4700,"SIM::SIM ini file change success!FDN %d")
TRACE_MSG(SIM_SIGNAL_5761_112_2_17_23_0_4_4701,"SIM::SIMSIGNAL_RefreshIniFileChg imsi is full 0xff.")
TRACE_MSG(SIM_SIGNAL_5831_112_2_17_23_0_4_4702,"SIM::Refresh Ini Full begin!")
TRACE_MSG(SIM_SIGNAL_5842_112_2_17_23_0_4_4703,"SIM::SIMSIGNAL_RefreshIniFull Read EFiccid failed!")
TRACE_MSG(SIM_SIGNAL_5849_112_2_17_23_0_4_4704,"SIM::SIMSIGNAL_RefreshIniFull select DFgsm failed!")
TRACE_MSG(SIM_SIGNAL_5864_112_2_17_23_0_4_4705,"SIM::Read EFphase failed in Refresh Ini!")
TRACE_MSG(SIM_SIGNAL_5887_112_2_17_23_0_4_4706,"SIM::Read EFad failed in SIM initialization!")
TRACE_MSG(SIM_SIGNAL_5902_112_2_17_23_0_4_4707,"SIM::SIM Refresh Ini Full success!FDN %d")
TRACE_MSG(SIM_SIGNAL_5911_112_2_17_23_0_4_4708,"SIM::SIMSIGNAL_RefreshIniFull imsi is full 0xff.")
TRACE_MSG(SIM_SIGNAL_5941_112_2_17_23_0_4_4709,"SIM::Refresh read File!")
TRACE_MSG(SIM_SIGNAL_6051_112_2_17_23_0_4_4710,"SIM%d:acm value = 0x%x")
TRACE_MSG(SIM_SIGNAL_6073_112_2_17_23_0_4_4711,"SIM%d::acm max is %x.")
TRACE_MSG(SIM_SIGNAL_6560_112_2_17_23_0_5_4712,"SIM::Envelope failed!")
TRACE_MSG(SIM_SIGNAL_6608_112_2_17_23_0_5_4713,"SIM::the img len is error!")
TRACE_MSG(SIM_SIGNAL_6644_112_2_17_23_0_5_4714,"SIM::the bits per img point is %d,out of range.")
TRACE_MSG(SIM_SIGNAL_6737_112_2_17_23_0_5_4715,"SIM:begin to read the %d img record.")
TRACE_MSG(SIM_SIGNAL_6751_112_2_17_23_0_5_4716,"SIM:read img failed.")
TRACE_MSG(SIM_SIGNAL_6778_112_2_17_23_0_6_4717,"SIM::SIMSIGNAL_ReadImg,select 0x%x failed.")
TRACE_MSG(SIM_SIGNAL_6793_112_2_17_23_0_6_4718,"SIM::SIMSIGNAL_ReadImg,reback to MF failed.")
TRACE_MSG(SIM_SIGNAL_6799_112_2_17_23_0_6_4719,"SIM::SIMSIGNAL_ReadImg,select 0x%x ok.")
TRACE_MSG(SIM_SIGNAL_6827_112_2_17_23_0_6_4720,"SIM:: ImiReadEF, read img 0x%x ser_result = %d")
TRACE_MSG(SIM_SIGNAL_6842_112_2_17_23_0_6_4721,"SIM:decode  img instance data file failed.")
TRACE_MSG(SIM_SIGNAL_6849_112_2_17_23_0_6_4722,"SIM:decode  img instance data file ok.")
TRACE_MSG(SIM_SIGNAL_6851_112_2_17_23_0_6_4723,"SIM:img width is %d,height is %d.")
TRACE_MSG(SIM_SIGNAL_6862_112_2_17_23_0_6_4724,"SIM:: SIMSIGNAL_ReadFileBufByPath, reback to MF ser_result = %d")
TRACE_MSG(SIM_SIGNAL_6866_112_2_17_23_0_6_4725,"SIM: !!SIMSIGNAL_ReadFileBufByPath, reback to MF Fail")
TRACE_MSG(SIM_SIGNAL_6886_112_2_17_23_0_6_4726,"SIM:decode  img instance data file failed.")
TRACE_MSG(SIM_SIGNAL_6893_112_2_17_23_0_6_4727,"SIM:decode  img instance data file ok.")
TRACE_MSG(SIM_SIGNAL_6895_112_2_17_23_0_6_4728,"SIM:img width is %d,height is %d.")
TRACE_MSG(SIM_SIGNAL_6902_112_2_17_23_0_6_4729,"SIM::img service not support.")
TRACE_MSG(SIM_SMS_174_112_2_17_23_0_6_4730,"SIMSMS: g_sim_card_setting.sms_info.valide_sms_num is %d")
TRACE_MSG(SIM_TEST_DRIVER_174_112_2_17_23_0_9_4731,"Rx_Buf : %s")
TRACE_MSG(SIM_TEST_DRIVER_220_112_2_17_23_0_9_4732,"SIM::SIM card inserted ok!")
TRACE_MSG(SIM_TEST_DRIVER_225_112_2_17_23_0_9_4733,"SIM::SIM removed1!")
TRACE_MSG(SIM_TEST_DRIVER_235_112_2_17_23_0_9_4734,"SIM::SIM inserted!")
TRACE_MSG(SIM_TEST_DRIVER_240_112_2_17_23_0_9_4735,"SIM::SIM Removed!")
TRACE_MSG(SIM_TEST_DRIVER_245_112_2_17_23_0_9_4736,"SIM::SIM error1!")
TRACE_MSG(SIM_TEST_DRIVER_250_112_2_17_23_0_9_4737,"SIM::SIM error2!")
TRACE_MSG(SIM_TEST_DRIVER_255_112_2_17_23_0_9_4738,"SIM::SIM error3!")
END_TRACE_MAP(BASE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _BASE_TRC_H_

