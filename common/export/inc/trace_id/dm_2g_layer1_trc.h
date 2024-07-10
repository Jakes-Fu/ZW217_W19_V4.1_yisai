/******************************************************************************
 ** File Name:      dm_2g_layer1_trc.h                                         *
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
//trace_id:8
#ifndef _DM_2G_LAYER1_TRC_H_
#define _DM_2G_LAYER1_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define CALIBRATION_1485_112_2_17_23_53_17_0 "L1_TD: Transfer Gsm Rf Para in Calibration"
#define CALIBRATION_1521_112_2_17_23_53_17_1 "L1_TD: Calibration_Entry wait dsp ready start %d"
#define CALIBRATION_1523_112_2_17_23_53_18_2 "L1_TD: Calibration_Entry wait dsp ready finish %d"
#define CALIBRATION_1572_112_2_17_23_53_18_3 "L1:calibration,recv unknown sig %d"
#define CALIBRATION_1782_112_2_17_23_53_18_4 "L1:caliberation diag paramter is wrong!"
#define CALIBRATION_1915_112_2_17_23_53_18_5 "L1:band=%x,arfcn=%x,ind=%x,val=%x,type=%x,count=%x"
#define CALIBRATION_2489_112_2_17_23_53_20_6 "L1:handle tx request band = %d"
#define CALIBRATION_2590_112_2_17_23_53_20_7 "L1:handle tx request band = %d"
#define CALIBRATION_2660_112_2_17_23_53_20_8 "L1:SEND 8PSK TX REQ"
#define CALIBRATION_2699_112_2_17_23_53_20_9 "L1:STOP 8PSK TX "
#define CALIBRATION_3022_112_2_17_23_53_21_10 "L1:set EGSM index %d gain= %x"
#define CALIBRATION_3037_112_2_17_23_53_21_11 "L1:RX REQ g_layer1_state =%d "
#define CALIBRATION_3193_112_2_17_23_53_21_12 "L1:RX REQ g_layer1_state =%d "
#define CALIBRATION_3256_112_2_17_23_53_21_13 "L1:set EGSM index %d gain= %x"
#define CALIBRATION_3291_112_2_17_23_53_21_14 "L1:set DCS index %d gain= %x"
#define CALIBRATION_3325_112_2_17_23_53_21_15 "L1:set PCS index %d gain= %x"
#define CALIBRATION_3361_112_2_17_23_53_21_16 "L1:set GSM850 index %d gain= %x"
#define CALIBRATION_3380_112_2_17_23_53_21_17 "L1:RX REQ g_layer1_state =%d "
#define CALIBRATION_3554_112_2_17_23_53_22_18 "L1:RSSI== %x"
#define CALIBRATION_3798_112_2_17_23_53_22_19 "L1:RF control Request,value = %d"
#define CALIBRATION_3802_112_2_17_23_53_22_20 "L1:RF control Request,length = %d"
#define CALIBRATION_3814_112_2_17_23_53_22_21 "L1:RF control Request,gpip_no = %d value = %d"
#define CALIBRATION_3827_112_2_17_23_53_22_22 "L1:RF control Request,tx = %d"
#define CALIBRATION_4015_112_2_17_23_53_23_23 "L1 DC voltage i %x, q %x, count %d"
#define CALIBRATION_4133_112_2_17_23_53_23_24 "L1:RAMP_ADD=%x,OFFSET=%x"
#define CALIBRATION_4252_112_2_17_23_53_23_25 "GET iq avg == %x, count %d"
#define CALIBRATION_4393_112_2_17_23_53_23_26 "L1:rxtxloop, savesigal mode error = %d"
#define CALIBRATION_4409_112_2_17_23_53_23_27 "L1:rxtxloop, new pcl %d"
#define CALIBRATION_4431_112_2_17_23_53_23_28 "L1:rxtxloop, new cmd=%d"
#define CALIBRATION_4449_112_2_17_23_53_23_29 "L1:rxtxloop wakedsp1 =%d"
#define CALIBRATION_4524_112_2_17_23_53_24_30 "L1:rxtxloop init, save msg error."
#define CALIBRATION_4531_112_2_17_23_53_24_31 "L1:rxtxloop init, layer1 state error."
#define CALIBRATION_4536_112_2_17_23_53_24_32 "L1:rxtxloop init, substate error"
#define CALIBRATION_4818_112_2_17_23_53_24_33 "L1:rxtxloop, update nv start"
#define CALIBRATION_4834_112_2_17_23_53_24_34 "L1:rxtxloop, update nv end"
#define CALIBRATION_4892_112_2_17_23_53_24_35 "L1:rxtxloop, req sig, cmd=%d"
#define CALIBRATION_4928_112_2_17_23_53_24_36 "L1:loop s=%d fn=x%x"
#define CALIBRATION_4973_112_2_17_23_53_25_37 "L1:change arfcn"
#define CALIBRATION_5026_112_2_17_23_53_25_38 "L1:SetState=%d"
#define CALIBRATION_5105_112_2_17_23_53_25_39 "L1:rxlev2diag 0x%x"
#define CALIBRATION_5209_112_2_17_23_53_25_40 "L1:rxtxloop, errors %d, cmd %d, substate %d"
#define CALIBRATION_5288_112_2_17_23_53_25_41 "L1:rxtxloop, verify sch delay=%d, fn %d"
#define CALIBRATION_5320_112_2_17_23_53_25_42 "L1:rxtxloop2next=%d"
#define CALIBRATION_5386_112_2_17_23_53_25_43 "L1:rxtxloop, no bcch or wrong askmore passed here , which triggered in search"
#define CALIBRATION_5460_112_2_17_23_53_26_44 "L1:rxtxloop startcamp, target=%d"
#define CALIBRATION_5518_112_2_17_23_53_26_45 "L1:rxtxloop, target=%d,gain=%d"
#define CALIBRATION_5556_112_2_17_23_53_26_46 "L1:rxtxloop, meas n=%d"
#define CALIBRATION_5638_112_2_17_23_53_26_47 "rxtxloop:RxBlockReady"
#define CALIBRATION_5681_112_2_17_23_53_26_48 "L1:rxtxloop, search/read state machine error.!!!"
#define CALIBRATION_5687_112_2_17_23_53_26_49 "L1:rxtxloop, rxblock channel tag error !!!"
#define CALIBRATION_5708_112_2_17_23_53_26_50 "L1:rxtxloop, sch verify sync error."
#define CALIBRATION_5718_112_2_17_23_53_26_51 "L1:rxtxloop, sch data sync error."
#define CALIBRATION_5849_112_2_17_23_53_26_52 "rxtxloop, sch verify, deltaold=%d, deltanew=%d"
#define CALIBRATION_5876_112_2_17_23_53_27_53 "L1:rxtxloop, sch successed, oldfn=%d, curfn=%d, deltafn=%d"
#define CALIBRATION_5894_112_2_17_23_53_27_54 "L1:sch failed, cell=%d, count=%d"
#define CALIBRATION_5926_112_2_17_23_53_27_55 "L1:rxtxloop meas count=%d, result=%d"
#define CALIBRATION_5946_112_2_17_23_53_27_56 "rxtxloop, not right arfcn queue"
#define CALIBRATION_5976_112_2_17_23_53_27_57 "L1:rxtxloop, measreport lev x%x, qual x%x"
#define CALIBRATION_5991_112_2_17_23_53_27_58 "L1:rxtxloop, readpwrque"
#define CALIBRATION_6054_112_2_17_23_53_27_59 "L1:rxtxloop, BELOW_NOISE_LO"
#define CALIBRATION_6059_112_2_17_23_53_27_60 "L1:rxtxloop, GOOD, arfcn=%d"
#define CALIBRATION_6129_112_2_17_23_53_27_61 "L1:txtxloop, agc=%d"
#define CALIBRATION_6151_112_2_17_23_53_27_62 "L1:rxtxloop, start search fn 0x%x"
#define CALIBRATION_6193_112_2_17_23_53_27_63 "L1:loop, change arfcn %d, tn %d, tsc %d"
#define CALIBRATION_6225_112_2_17_23_53_27_64 "L1:loop, arfcn %d, tn %d, tsc %d"
#define CALIBRATION_6296_112_2_17_23_53_27_65 "L1:idle askmore1"
#define CALIBRATION_6302_112_2_17_23_53_27_66 "L1:idle askmore2"
#define CALIBRATION_6369_112_2_17_23_53_27_67 "L1_TD: Transfer Gsm Rf Para in Calibration"
#define CALIBRATION_6405_112_2_17_23_53_28_68 "L1_TD: Calibration_Entry wait dsp ready start %d"
#define CALIBRATION_6407_112_2_17_23_53_28_69 "L1_TD: Calibration_Entry wait dsp ready finish %d"
#define CALIBRATION_6518_112_2_17_23_53_28_70 "fastcali SaveParamSet cmd %d, count %d"
#define CALIBRATION_6658_112_2_17_23_53_28_71 "fastcali parse param %d, rxcount %d"
#define CALIBRATION_6668_112_2_17_23_53_28_72 "fastcali parse count %d"
#define CALIBRATION_6678_112_2_17_23_53_28_73 "fastcali error %d"
#define CALIBRATION_6691_112_2_17_23_53_28_74 "fastcali parse factor %d, index %d"
#define CALIBRATION_6726_112_2_17_23_53_28_75 "fastcali parse agc ind %d, agc val 0x%x"
#define CALIBRATION_6794_112_2_17_23_53_28_76 "fastcali err=%d, substate=%d"
#define CALIBRATION_6821_112_2_17_23_53_28_77 "fastcali idle cmd %d"
#define CALIBRATION_6860_112_2_17_23_53_28_78 " FASTCALI_START_TX_8PSK parameter type is WRONG! "
#define CALIBRATION_6872_112_2_17_23_53_28_79 " FASTCALI get starting 8psk parameter %d,%d %d"
#define CALIBRATION_6929_112_2_17_23_53_29_80 "L1 GetSCHSet arfcn 0x%x, band %d, gain ind %d, gain val 0x%x"
#define CALIBRATION_6958_112_2_17_23_53_29_81 "fastcali Read , arfcn %d, fn 0x%x"
#define CALIBRATION_7008_112_2_17_23_53_29_82 "fastcali Search , arfcn %d, fn 0x%x"
#define CALIBRATION_7030_112_2_17_23_53_29_83 "fastcali Search , end fn 0x%x"
#define CALIBRATION_7051_112_2_17_23_53_29_84 "fastcali last askmore."
#define CALIBRATION_7065_112_2_17_23_53_29_85 "fastcali askmore rx fn %d, state %d"
#define CALIBRATION_7101_112_2_17_23_53_29_86 "fastcali askmore rx fn %d"
#define CALIBRATION_7167_112_2_17_23_53_29_87 "s_fastcali_slot_rxlev_counter=%d, count=%d"
#define CALIBRATION_7176_112_2_17_23_53_29_88 "RSSI[%d]=%d"
#define CALIBRATION_7218_112_2_17_23_53_29_89 "fastcali rHandlePowerQue state %d"
#define CALIBRATION_7251_112_2_17_23_53_29_90 "fastcali read from Power mon queue fail! "
#define CALIBRATION_7254_112_2_17_23_53_29_91 "fastcali rHandlePowerQue counter %d"
#define CALIBRATION_7328_112_2_17_23_53_29_92 "FASTCALI_ProcessRxSch, synced_fn 0x%x, oldfn 0x%x, cur_fn 0x%x, searchcount %d, spentcount %d"
#define CALIBRATION_7366_112_2_17_23_53_29_93 "fastcali sch channel tag error !!!"
#define CALIBRATION_7372_112_2_17_23_53_29_94 "fastcali sch read buffer fail !!!"
#define CALIBRATION_7404_112_2_17_23_53_30_95 "fastcali sch resq sub state error %d"
#define CALIBRATION_7423_112_2_17_23_53_30_96 "fastcali GetRxTstFrmDelay startfn %d, curfn %d"
#define CALIBRATION_7462_112_2_17_23_53_30_97 "fastcali triggerrx error : can't find rx frame"
#define CALIBRATION_7484_112_2_17_23_53_30_98 "fastcali FindFirstRxFrameIndex %d isfind 1"
#define CALIBRATION_7512_112_2_17_23_53_30_99 "fastcali FindFirstRxFrameIndex %d isfind %d"
#define CALIBRATION_7542_112_2_17_23_53_30_100 "fastcali plot fn 0x%x"
#define CALIBRATION_7565_112_2_17_23_53_30_101 "fastcali plot tx 0x%x"
#define CALIBRATION_7966_112_2_17_23_53_31_102 "FASTCALI PlotEdgeTxFrames arfcn%d:txpareptr = 0x%x"
#define CALIBRATION_7992_112_2_17_23_53_31_103 "FASTCALI txgain_level_index %d:txgain_level = %d"
#define CALIBRATION_8029_112_2_17_23_53_31_104 "fastcali sch fail , substate %d, fcch ind %d, cell index %d, search_timeout %d"
#define CALIBRATION_8051_112_2_17_23_53_31_105 "fastcali sch read fail , substate %d, fcch ind %d, "
#define CALIBRATION_8070_112_2_17_23_53_31_106 "fastcali FastCali_Initialize"
#define CALIBRATION_8164_112_2_17_23_53_31_107 "fastcali SchRxTriggerDelay fn %d, delay %d"
#define CELLSYNCINFO_612_112_2_17_23_53_33_108 "L1 init %d, cell_id %d,new arfcn_index %x;old %x,%x "
#define CELLSYNCINFO_1108_112_2_17_23_53_34_109 "L1 cell_id invalid %x"
#define CELLSYNCINFO_1122_112_2_17_23_53_34_110 "L1 Get_RR_S State cell_index %d"
#define CELLSYNCINFO_1342_112_2_17_23_53_34_111 "L1 vice card two strong num:%d when receiving vice card paging"
#define CELLSYNCINFO_1375_112_2_17_23_53_34_112 "L1,vice card two strong in main card six strong\n"
#define CELLSYNCINFO_1380_112_2_17_23_53_34_113 "L1,vice card one strong in main card six strong\n"
#define CELLSYNCINFO_1387_112_2_17_23_53_34_114 "L1,vice card two strong not in main card six strong\n"
#define CELLSYNCINFO_1754_112_2_17_23_53_35_115 "L1: new ta %d"
#define CELLSYNCINFO_1771_112_2_17_23_53_35_116 "L1:Handover SYNC cell old_ta = %d,new_ta %d,otd %d"
#define CELLSYNCINFO_1781_112_2_17_23_53_35_117 "L1:Handover PSEUDO old ta %d,otd %d,rtd %d,new_ta %d"
#define CELLSYNCINFO_1800_112_2_17_23_53_35_118 "L1:Handover PSEUDO cell old_ta = %d,new_ta %d,otd %d"
#define CELLSYNCINFO_1938_112_2_17_23_53_35_119 "L1 camp to same!"
#define CELLSYNCINFO_2603_112_2_17_23_53_37_120 "L1 bcch select %x"
#define CELLSYNCINFO_2902_112_2_17_23_53_37_121 "L1: CellSyncInfo_GetNextCBCHBlkDelay schedule %ld, %d"
#define CELLSYNCINFO_2923_112_2_17_23_53_37_122 "L1:CellSyncInfo_GetNextCBCHBlkDelay %d"
#define CELLSYNCINFO_2935_112_2_17_23_53_37_123 "L1: MPHHandle_IsDiscardCBCHBlock %d"
#define CELLSYNCINFO_3006_112_2_17_23_53_38_124 "L1:Handover fail, cell_id = %d,bsic %d,target bsic %d"
#define CELLSYNCINFO_3047_112_2_17_23_53_38_125 "L1:handover cell %d,arfcn %d otd %d,scell %d,arfcn %d,otd %d, ba_index %d"
#define CELLSYNCINFO_3059_112_2_17_23_53_38_126 "L1 no space to add a new arfcn to ba!"
#define CELLSYNCINFO_3604_112_2_17_23_53_39_127 "L1: 1:%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,nL1: 2:%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d"
#define CELLSYNCINFO_3681_112_2_17_23_53_39_128 "L1 start decode PSI,need decode %d"
#define CELLSYNCINFO_3791_112_2_17_23_53_39_129 "L1_TD QQQ CellSyncInfo_UpdateCellDeltaFn: g_td_cell_result.sync_td.delta_position %d, old_current_fn 0x%x, fn_count %d"
#define CELLSYNCINFO_4905_112_2_17_23_53_41_130 "L1 get conflict %d,cellid %d"
#define CELLSYNCINFO_4939_112_2_17_23_53_41_131 "L1 LastTBFPollingImpFlag fn %d, delay %d"
#define CELLSYNCINFO_4966_112_2_17_23_53_41_132 "L1 conflict cell id %d, conflict %d"
#define CELLSYNCINFO_4971_112_2_17_23_53_41_133 "L1 invalid cell id %d"
#define CELLSYNCINFO_5032_112_2_17_23_53_42_134 "L1 sacch counter: %d, reset!"
#define CELLSYNCINFO_5292_112_2_17_23_53_42_135 "L1: invalid tc"
#define CELLSYNCINFO_5549_112_2_17_23_53_43_136 "L1 invlaid cell id %d"
#define CELLSYNCINFO_5586_112_2_17_23_53_43_137 "L1 save bcch tc %d,%x,fn %x"
#define CELLSYNCINFO_5644_112_2_17_23_53_43_138 "L1 invalid cell_id %d"
#define CELLSYNCINFO_5699_112_2_17_23_53_43_139 "L1 s_b %x, tc %d"
#define CELLSYNCINFO_5704_112_2_17_23_53_43_140 "L1 bcch has been invalid %d %d,%x"
#define CELLSYNCINFO_5711_112_2_17_23_53_43_141 "L1 invalid cell_id %d"
#define CELLSYNCINFO_5755_112_2_17_23_53_43_142 "L1 b_t  send %d, %d"
#define CELLSYNCINFO_5815_112_2_17_23_53_43_143 "L1 b_t  set %d, %d"
#define CELLSYNCINFO_5819_112_2_17_23_53_43_144 "L1 bcch_buf_index_queue full!"
#define CELLSYNCINFO_5852_112_2_17_23_53_43_145 "L1 b_t set"
#define CELLSYNCINFO_5857_112_2_17_23_53_43_146 "L1 b_t creat fail"
#define CELLSYNCINFO_5873_112_2_17_23_53_43_147 "L1 b_t a"
#define CELLSYNCINFO_5880_112_2_17_23_53_43_148 "L1 b_t  a f"
#define CELLSYNCINFO_5895_112_2_17_23_53_43_149 "L1 b_t dea"
#define CELLSYNCINFO_5900_112_2_17_23_53_43_150 "L1 b_t dea"
#define CELLSYNCINFO_5905_112_2_17_23_53_43_151 "L1 b_t de f"
#define CELLSYNCINFO_5963_112_2_17_23_53_43_152 "L1: vice card two strongest arfcn is in main card six strongest"
#define CELLSYNCINFO_5971_112_2_17_23_53_43_153 "L1: arfcn_index 0x%x is minor card's one strong"
#define CELLSYNCINFO_5984_112_2_17_23_53_43_154 "L1: arfcn_index 0x%x is minor card's second strong."
#define CELLSYNCINFO_6177_112_2_17_23_53_44_155 "L1:main_card_id: %d, select_card_id: %d,active_card_num:%d,card_bitmap:0x%x,pch_delay:%d"
#define CELLSYNCINFO_6253_112_2_17_23_53_44_156 "L1 scell of card %d exist!%d"
#define CELLSYNCINFO_6297_112_2_17_23_53_44_157 "L1 pch p_p co %x, se %x"
#define CELLSYNCINFO_6314_112_2_17_23_53_44_158 "L1 pch p_b co %x"
#define CELLSYNCINFO_6512_112_2_17_23_53_44_159 "L1 invalid fn %x,%x"
#define CELLSYNCINFO_6523_112_2_17_23_53_45_160 "L1 invalid fn %x"
#define CELLSYNCINFO_6534_112_2_17_23_53_45_161 "L1 invalid fn %x"
#define CELLSYNCINFO_6550_112_2_17_23_53_45_162 "L1 fn adjust detected!"
#define CMDQ_145_112_2_17_23_53_45_163 "L1 deactive all"
#define CMDQ_342_112_2_17_23_53_46_164 "Layer1_cmd, audio in %d, audio out %d, is_input_enable %d"
#define CMDQ_353_112_2_17_23_53_46_165 "arm control audio , cancel audio enable to dsp!"
#define CMDQ_399_112_2_17_23_53_46_166 "layer1_cmd: command audio enable/disable"
#define CMDQ_416_112_2_17_23_53_46_167 "L1:sent cmd AUDQ_SET_AUDIO_ENABLE input,%d,output%d"
#define CMDQ_444_112_2_17_23_53_46_168 "AUD_NEW: CmdQ_SetCodecEnable enc_en %d, dec_en %d"
#define CMDQ_531_112_2_17_23_53_46_169 "AUD_NEW: CmdQ_SetVolume output_volume %d"
#define CMDQ_612_112_2_17_23_53_46_170 "AUD_NEW: CmdQ_SetMuteEnable is_mute_enable %d, mute_type %d"
#define CMDQ_783_112_2_17_23_53_46_171 "AUD_NEW: CmdQ_SpecialToneGeneration tone_id %d, is_needing_to_encode %d"
#define CMDQ_835_112_2_17_23_53_47_172 "AUD_NEW: CmdQ_GenericToneGeneration frequency1 %d, frequency2 %d, frequency3 %d"
#define CMDQ_875_112_2_17_23_53_47_173 "AUD_NEW: CmdQ_StopTone"
#define CMDQ_1205_112_2_17_23_53_47_174 "L1:SEND CMDQ_WAKEUP_DSP!\n"
#define CMDQ_1284_112_2_17_23_53_47_175 "AUD_NEW: CmdQ_AudioLoop bEnableLoop %d"
#define CMDQ_1410_112_2_17_23_53_48_176 "AUD_NEW: CmdQ_EnableVoiceLoopback is_enable %d,%d,%d,%d"
#define CMDQ_1443_112_2_17_23_53_48_177 "AUD_NEW: CmdQ_EnableDownlinkMute is_enable %d, dlnk_mute_type %d"
#define CMDQ_1477_112_2_17_23_53_48_178 "AUD_NEW: CmdQ_SetEchoTestEnable b_is_start_echo_testing %d, freq %d"
#define CMDQ_1514_112_2_17_23_53_48_179 "L1: CmdQ_StartVoiceUpLoad, p0 0x%x, p1 0x%x, p2 0x%x, p3 0x%x"
#define CMDQ_1516_112_2_17_23_53_48_180 "AUD_NEW: CmdQ_StartVoiceUpLoad is_from_mic %d, is_from_net %d, voice_format %d, upload_amr_mode %d"
#define CMDQ_1553_112_2_17_23_53_48_181 "L1:CmdQ_StartVoiceDownLoad"
#define CMDQ_1555_112_2_17_23_53_48_182 "AUD_NEW: CmdQ_StartVoiceDownLoad is_from_mic %d, is_to_net %d, voice_format %d"
#define CMDQ_1587_112_2_17_23_53_48_183 "L1:CmdQ_NotifyPchReorgMode"
#define CMDQ_1825_112_2_17_23_53_49_184 "L1:CMDQ_Dedicate_Establish: 0x%x, is_dsp_addr_sent %d"
#define CMDQ_1841_112_2_17_23_53_49_185 "AUD_NEW: CmdQ_TransNewAudioPara length %d"
#define CMDQ_1917_112_2_17_23_53_49_186 "L1:CMDQ_RXADC_DCVOLTAGE"
#define CMDQ_1932_112_2_17_23_53_49_187 "L1: cmd %x,%x"
#define CMDQ_1952_112_2_17_23_53_49_188 "L1: L1 Assert Dsp! fn 0x%x"
#define CMDQ_1964_112_2_17_23_53_49_189 "L1: cmd %x,%x"
#define CMDQ_1999_112_2_17_23_53_49_190 "L1Q:CmdQ_ChangeMode: 0x%x, mode_change_flage %x, card_id %d"
#define CMDQ_2041_112_2_17_23_53_49_191 "L1Q:CMDQ_CHANGEMODE_BACK! %x"
#define CMDQ_2073_112_2_17_23_53_49_192 "L1: Not Support CmdQ_StartPCMUpLoad in Old AUDIO"
#define CMDQ_2100_112_2_17_23_53_49_193 "L1: Not Support CmdQ_StartPCMDownLoad in Old AUDIO"
#define CMDQ_2123_112_2_17_23_53_49_194 "L1:CMDQ_IQ_CATCH"
#define CMDQ_2180_112_2_17_23_53_49_195 "L1:CmdQ_StartMixVoiceFun flag %d,type %d,uplink_gain %d,downlink_gain %d"
#define DATACNFPROC_125_112_2_17_23_53_50_196 "L1: SendRachCNF!"
#define DATACNFPROC_228_112_2_17_23_53_50_197 "PRACH: send success, fn = %d!"
#define DATACNFPROC_260_112_2_17_23_53_50_198 "L1: Handover_ACCESS_CNF!"
#define DATACNFPROC_309_112_2_17_23_53_50_199 "L1: MPH_ERROR_IND!"
#define DATACNFPROC_323_112_2_17_23_53_50_200 "L1: MPH_RXPOWER_SWEEP_CNF!"
#define DATACNFPROC_428_112_2_17_23_53_50_201 "L1:FreqChangeCnf!"
#define DATACNFPROC_598_112_2_17_23_53_51_202 "L1:DataCnfProc_MphSmsCbchDataInd normal message! %lx, %lx,%lx, %lx"
#define DATACNFPROC_604_112_2_17_23_53_51_203 "L1:DataCnfProc_MphSmsCbchDataInd null message"
#define DATACNFPROC_682_112_2_17_23_53_51_204 "L1: MPH_CHAN_ASSIGN_FAIL_CNF!"
#define DATACNFPROC_919_112_2_17_23_53_51_205 "L1: MPH_NC_MEAS_REPORT_IND"
#define DATACNFPROC_943_112_2_17_23_53_52_206 "L1: MPH_CIPHER_MODE_CNF!"
#define DATACNFPROC_961_112_2_17_23_53_52_207 "L1 set task id %x DataCnfProc_SetTaskIdForReadDspMemory"
#define DATACNFPROC_985_112_2_17_23_53_52_208 "L1: dsp memory's response,value address 0x%x value %d"
#define DATACNFPROC_989_112_2_17_23_53_52_209 "L1: dsp memory's response,value address 0x%x value %d"
#define DATACNFPROC_993_112_2_17_23_53_52_210 "L1: dsp memory's response,value address 0x%x value %d"
#define DATACNFPROC_1001_112_2_17_23_53_52_211 "L1:dsp memory's response,value %d"
#define DATACNFPROC_1015_112_2_17_23_53_52_212 "L1:dsp memory's response,value Address %d"
#define DATACNFPROC_1036_112_2_17_23_53_52_213 "L1:layer1 should read DSP side memory!"
#define DATACNFPROC_1185_112_2_17_23_53_52_214 "L1: MPH_CHANNEL_MODE_CNF!"
#define DATACNFPROC_1223_112_2_17_23_53_52_215 "L1: non sync otd %d,%d,otd %d"
#define DATACNFPROC_1240_112_2_17_23_53_52_216 "L1: MPH_HANDOVER_CNF!OTD = %d, old_TA = %d, MTD = %d"
#define DATACNFPROC_1258_112_2_17_23_53_52_217 "L1: MPH_HANDOVER_FAIL_CNF!"
#define DATACNFPROC_1277_112_2_17_23_53_52_218 "L1:PH_SCT_TCH_LOOP_ACK!"
#define DATACNFPROC_1321_112_2_17_23_53_52_219 "L1:DataCnfProc_L1MnGetAFCCnf, AFC_Value = %d!"
#define DATACNFPROC_1373_112_2_17_23_53_52_220 "L1:DataCnfProc_L1MnGetAFCCnf, PA_Value = %d"
#define DATACNFPROC_1404_112_2_17_23_53_52_221 "L1:DataCnfProc_L1MnGetAGCCnf,AGC_Value = %d!"
#define DATACNFPROC_1434_112_2_17_23_53_52_222 "L1:L1MnGetBlockErrorRatioCnf,total count= %d error count = %d!"
#define DATACNFPROC_1454_112_2_17_23_53_53_223 "L1:DspVoiceLoopbackInd! Index = %d\n"
#define DATACNFPROC_1550_112_2_17_23_53_53_224 "L1:TDMeasResult sync is wrong, %d, %d"
#define DATACNFPROC_1566_112_2_17_23_53_53_225 "L1_TD: don't change to TD"
#define DATACNFPROC_1610_112_2_17_23_53_53_226 "L1:TDMeasResult cellparam 0x%x rscp 0x%x"
#define DATACNFPROC_1626_112_2_17_23_53_53_227 "L1_TD: td ncell meas rscp add 0x20"
#define DATACNFPROC_1669_112_2_17_23_53_53_228 "L1_TD: td ncell meas rscp add 0x20"
#define DATACNFPROC_1689_112_2_17_23_53_53_229 "L1_TD: td ncell meas rscp add 0x20"
#define DATAPROC_133_112_2_17_23_53_53_230 "L1:Rx SA!,%d"
#define DATAPROC_138_112_2_17_23_53_53_231 "L1:Rx bad SA counter = %d"
#define DATAPROC_191_112_2_17_23_53_54_232 "L1:Rx bad FA,0x%x,0x%x"
#define DATAPROC_222_112_2_17_23_53_54_233 "bad half rate FA,%3x,%3x"
#define DATAPROC_324_112_2_17_23_53_54_234 "L1: AMR startreqacktimer fn 0x%x"
#define DATAPROC_557_112_2_17_23_53_54_235 "L1:DataProc_RxRatscch: ACK_ERR or ACK_UNKNOW, type = 0x%x!"
#define DATAPROC_564_112_2_17_23_53_54_236 "L1:DataProc_RxRatscch: Ignore RATSCCH in REQ_ACK periord, s_active_req = %d!"
#define DATAPROC_608_112_2_17_23_53_54_237 "L1:bad RATSCCH,%3x,%3x"
#define DATAPROC_639_112_2_17_23_53_54_238 "L1: bad SD,0x%x,0x%x"
#define DATAPROC_749_112_2_17_23_53_55_239 "L1 invalid ref_scell_id %x main_card_id %x"
#define DATAPROC_803_112_2_17_23_53_55_240 "L1:ref_scell_id %x, main_card_id %x"
#define DATAPROC_824_112_2_17_23_53_55_241 "L1:bsic suc %x, id %x,%x"
#define DATAPROC_875_112_2_17_23_53_55_242 "L1 search invalid %x"
#define DATAPROC_889_112_2_17_23_53_55_243 "L1: A change of bsic is detected!, old bsic %d, new bsic %d"
#define DATAPROC_920_112_2_17_23_53_55_244 "L1 search invalid %x"
#define DATAPROC_933_112_2_17_23_53_55_245 "L1: A change of bsic is detected!, old bsic %d, new bsic %d"
#define DATAPROC_961_112_2_17_23_53_55_246 "L1:This cell maybe been replaced! Cell Id %d, arfcn %d!"
#define DATAPROC_973_112_2_17_23_53_55_247 "L1:Note got sch but this cell's state is changed to NULL cell!"
#define DATAPROC_1035_112_2_17_23_53_55_248 "L1:read tch suc: %x,cell %x,arfcn %x,otd %x,%x, ncc %x"
#define DATAPROC_1079_112_2_17_23_53_55_249 "L1 search invalid %x"
#define DATAPROC_1142_112_2_17_23_53_55_250 "L1 search invalid %x"
#define DATAPROC_1178_112_2_17_23_53_56_251 "L1: A change of bsic is detected!, old bsic %d, new bsic %d"
#define DATAPROC_1208_112_2_17_23_53_56_252 "This cell maybe have been substitut! Cell Id %d, arfcn %d!"
#define DATAPROC_1215_112_2_17_23_53_56_253 "L1: Tch BSIC read success! Cell %d Arfcn = %d ncc = %d\r "
#define DATAPROC_1222_112_2_17_23_53_56_254 "L1: got a sch but this cell's state is changed to NULL cell!"
#define DATAPROC_1234_112_2_17_23_53_56_255 "L1 handover cell search suc !"
#define DATAPROC_1267_112_2_17_23_53_56_256 "L1 bcch is not required by this cell %d, map %x,%x,%x"
#define DATAPROC_1283_112_2_17_23_53_56_257 "L1:bad BCCH! Fn = 0x%lx, arfcn_index = %d!"
#define DATAPROC_1359_112_2_17_23_53_56_258 "L1 bcch is not required by this cell %d, map %x,%x,%x"
#define DATAPROC_1369_112_2_17_23_53_56_259 "L1:bad BCCH! %x,%x, fn = %d, arfcn_index = %d!"
#define DATAPROC_1449_112_2_17_23_53_56_260 "L1 bcch is not required by this cell %d, map %x,%x,%x"
#define DATAPROC_1469_112_2_17_23_53_56_261 "L1:bad BCCH! %x,%x, fn = 0x%x,arfcn_index = 0x%x!"
#define DATAPROC_1528_112_2_17_23_53_56_262 "L1: Discard a agch block! due to error cell_index! scell_id = %d, error_cell_id = %d "
#define DATAPROC_1577_112_2_17_23_53_56_263 "L1:bad AGCH! Fn = %lx, dsc %d"
#define DATAPROC_1636_112_2_17_23_53_56_264 "L1: maintain DSC on pg_reorg with SCell changed"
#define DATAPROC_1654_112_2_17_23_53_56_265 "L1: maintain DSC on pg_reorg"
#define DATAPROC_1854_112_2_17_23_53_57_266 "L1:Wrong main pch frame number=0x%x"
#define DATAPROC_1859_112_2_17_23_53_57_267 "L1 page_type invalid! %x, %x"
#define DATAPROC_1870_112_2_17_23_53_57_268 "L1 the card is deactived %x"
#define DATAPROC_1931_112_2_17_23_53_57_269 "L1 bad pch 0x%x,dsc %d, %x, %x"
#define DATAPROC_1977_112_2_17_23_53_57_270 "L1 pch dummy  0x%x,fn: 0x%x,card mask %x, tk %x"
#define DATAPROC_2381_112_2_17_23_53_58_271 "L1: AMR getreqackdeltaspeechframe, new_fn 0x%x, old_fn 0x%x"
#define DATAPROC_2386_112_2_17_23_53_58_272 "L1: AMR fn small"
#define DATAPROC_2411_112_2_17_23_53_58_273 "GetREQACTDelta: fn_26=%d, fn_sp=%d, old_26=%d, old_sp=%d!"
#define DATAPROC_2414_112_2_17_23_53_58_274 "GetREQACTDelta: old_fn=0x%x, delta=%d!"
#define DATAPROC_2432_112_2_17_23_53_58_275 "GetACKACTDelta: fn=0x%x!"
#define DATAPROC_2438_112_2_17_23_53_58_276 "L1: AMR getackackdeltaspeechframe, new_fn 0x%x, old_fn 0x%x"
#define DATAPROC_2443_112_2_17_23_53_58_277 "L1: AMR fn small"
#define DATAPROC_2468_112_2_17_23_53_58_278 "GetACKACTDelta: fn_26=%d, fn_sp=%d, old_26=%d, old_sp=%d!"
#define DATAPROC_2471_112_2_17_23_53_58_279 "GetACKACTDelta: old_fn=0x%x, delta=%d!"
#define DATAPROC_2584_112_2_17_23_53_58_280 "DataProc_TxFacch: applay new cmip = %d, fn = 0x%x!"
#define DATAPROC_2618_112_2_17_23_53_59_281 "DataProc_TxFacch: DL, icm=%d, acs=0x%x, fn = 0x%x!"
#define DATAPROC_2641_112_2_17_23_53_59_282 "DataProc_TxFacch: UL, icm=%d, acs=0x%x, fn=0x%x!"
#define DATAPROC_2672_112_2_17_23_53_59_283 "DataProc_TxFacch: threshreq, one=%d,two=%d,three=%d, fn = 0x%x!"
#define DATAPROC_2759_112_2_17_23_53_59_284 "DataProc_TxFacch: s_ratscch_ack_type=%d, fn=0x%x!"
#define DATAPROC_2772_112_2_17_23_53_59_285 "DataProc_TxFacch ratmark: s_ratscch_ack_type=%d, fn=0x%x!"
#define DATAPROC_2798_112_2_17_23_53_59_286 "DataProc_TxFacch ratdata: s_ratscch_ack_type=%d, fn=0x%x!"
#define DATAPROC_2936_112_2_17_23_53_59_287 "L1:channel change! %d"
#define DATAPROC_3046_112_2_17_23_53_59_288 "TBF Channel Change: exe_band = %d, new_band = %d, pwr_idx = %d!"
#define DATAPROC_3122_112_2_17_23_54_0_289 "L1: bad PDTCH data!"
#define DATAPROC_3180_112_2_17_23_54_0_290 "L1:fn 0x%x,dump unneed slots %d,slot mask:0x%x"
#define DATAPROC_3281_112_2_17_23_54_0_291 "L1: same rx slot, fn %x,"
#define DATAPROC_3298_112_2_17_23_54_0_292 "L1:bad block fn 0x%x"
#define DATAPROC_3307_112_2_17_23_54_0_293 "L1 good num %d, bad num %d, bad rate : %d"
#define DATAPROC_3340_112_2_17_23_54_0_294 "L1: ta index %d,msg 0x%x"
#define DATAPROC_3370_112_2_17_23_54_0_295 "L1: ptcch fn %x: ta = %d"
#define DATAPROC_3384_112_2_17_23_54_0_296 "L1: Invalid ptcch value = %d!"
#define DATAPROC_3390_112_2_17_23_54_0_297 "L1: bad ptcch!"
#define DATAPROC_3421_112_2_17_23_54_0_298 "L1: bad pbcch! "
#define DATAPROC_3444_112_2_17_23_54_0_299 "L1: A WRONG bcch_dec_state.dec_mode!"
#define DATAPROC_3476_112_2_17_23_54_0_300 "L1: bad pbcch! "
#define DATAPROC_3499_112_2_17_23_54_0_301 "L1:psi decode num %d"
#define DATAPROC_3506_112_2_17_23_54_0_302 "L1:bad pbcch!"
#define DATAPROC_3542_112_2_17_23_54_0_303 "L1:bad pbcch!"
#define DATAPROC_3601_112_2_17_23_54_1_304 "L1: bad ppch"
#define DATAPROC_3683_112_2_17_23_54_1_305 "L1:bad ppch"
#define DATAPROC_3805_112_2_17_23_54_1_306 "L1:bad ppch"
#define DATAPROC_3924_112_2_17_23_54_1_307 "L1: a bad PAGCH in PRACH_PAGCH "
#define DATAPROC_3955_112_2_17_23_54_1_308 "L1:RXCBCH!card_id:%d"
#define DATAPROC_3961_112_2_17_23_54_1_309 "L1:bad CBCH!!"
#define DOWNLOADPARAM_1992_112_2_17_23_54_6_310 "L1:Threshold = %d"
#define DOWNLOADPARAM_2002_112_2_17_23_54_6_311 "L1:THreshold not get because audio3 T_T"
#define DOWNLOADPARAM_2042_112_2_17_23_54_6_312 "L1:Encount error when read NV EQ parameters!"
#define DOWNLOADPARAM_2115_112_2_17_23_54_6_313 "L1: Read NV selected band = %d"
#define DOWNLOADPARAM_2125_112_2_17_23_54_6_314 "L1: Fail in NV selected band reading, set is as Dual Band"
#define DOWNLOADPARAM_2140_112_2_17_23_54_6_315 "L1: Read NV selected deal flag = %d"
#define DOWNLOADPARAM_2150_112_2_17_23_54_6_316 "L1: Fail in NV selected deal flag reading, set is as zeor"
#define DSPINTISR_234_112_2_17_23_54_7_317 "L1: sharedmem GSM DSP size %d, L1 size %d"
#define DSPINTISR_574_112_2_17_23_54_8_318 "L1: %d times .lstate=%d"
#define DSPINTISR_653_112_2_17_23_54_8_319 "L1 invalid irq!%x"
#define DSPINTISR_752_112_2_17_23_54_8_320 "DSP Interrupt:STATUS_CANCLE_TBF_SEARCH,out:%d.\n"
#define DSPPROC_235_112_2_17_23_54_9_321 "L1:access burst %d,%d"
#define DSPPROC_253_112_2_17_23_54_9_322 "L1:access burst %d,%d,%d"
#define DSPPROC_310_112_2_17_23_54_10_323 "L1:access burst %d,%d"
#define DSPPROC_326_112_2_17_23_54_10_324 "L1:access burst %d,%d,%d"
#define DSPPROC_381_112_2_17_23_54_10_325 "L1:param is ready!!"
#define DSPPROC_391_112_2_17_23_54_10_326 "L1:WriteInitParaToDSP!g_is_download_param_done=%d"
#define DSPPROC_401_112_2_17_23_54_10_327 "L1:param is ready!"
#define FAKE_RR_331_112_2_17_23_54_11_328 "fake RR: sig code %x come in "
#define FAKE_RR_396_112_2_17_23_54_11_329 "CALIBRATION: recv unknown signal "
#define FAKE_RR_460_112_2_17_23_54_11_330 "fack RR: battery req send "
#define FRAMEINSTRUTIL_579_112_2_17_23_54_13_331 "LAYER1: It's a wrong POWER SWEEP Number with 0! "
#define FRAMEINSTRUTIL_636_112_2_17_23_54_14_332 "L1 rx_type %x"
#define FRAMEINSTRUTIL_1256_112_2_17_23_54_15_333 "L1:card_id:%d Plot4FrInstCBCH %ld,fncard:%ld"
#define FRAMEINSTRUTIL_1493_112_2_17_23_54_15_334 "L1 rx_type %x"
#define FRAMEINSTRUTIL_1518_112_2_17_23_54_15_335 "L1 pch %x, %x, %x"
#define FRAMEINSTRUTIL_1615_112_2_17_23_54_16_336 "L1 tbf receive minor card paging,card_bitmap:0x%x,fn_pch:0x%x,arfcn:%d"
#define FRAMEINSTRUTIL_1818_112_2_17_23_54_16_337 "L1: bsic read!arfcn_index = 0x%x,cell_index = %d"
#define FRAMEINSTRUTIL_1875_112_2_17_23_54_16_338 "L1 search %x"
#define FRAMEINSTRUTIL_1888_112_2_17_23_54_16_339 "L1 read conflict six sync strong cell!"
#define FRAMEINSTRUTIL_1971_112_2_17_23_54_16_340 "L1 search %x"
#define FRAMEINSTRUTIL_2016_112_2_17_23_54_16_341 "L1: BCCH %x, %x"
#define FRAMEINSTRUTIL_2133_112_2_17_23_54_17_342 "L1:IT'S WRONG DEDICATE_MODE TYPE! "
#define FRAMEINSTRUTIL_3145_112_2_17_23_54_19_343 "L1: IT's A WRONG s_dedicate_chan_map[fn_mod].channel_type!"
#define FRAMEINSTRUTIL_3207_112_2_17_23_54_19_344 "L1: IT's A WRONG s_dedicate_chan_map[fn_mod].channel_type!"
#define FRAMEINSTRUTIL_3337_112_2_17_23_54_19_345 "L1 tch read: %x"
#define FRAMEINSTRUTIL_3360_112_2_17_23_54_19_346 "L1 tch search: %x"
#define FRAMEINSTRUTIL_4675_112_2_17_23_54_22_347 "L1: TCH bsic read!Cell id %d"
#define FRAMEINSTRUTIL_4687_112_2_17_23_54_22_348 "L1: TCH searching!"
#define FRAMEINSTRUTIL_4699_112_2_17_23_54_22_349 "L1: TCH start search,cell_id %d"
#define FRAMEINSTRUTIL_5870_112_2_17_23_54_24_350 "L1 sdcch read %x"
#define FRAMEINSTRUTIL_6704_112_2_17_23_54_26_351 "L1:FrameInstrUtil_SetTxCmdPRACH:egprs rach,TS1"
#define FRAMEINSTRUTIL_6709_112_2_17_23_54_26_352 "L1:FrameInstrUtil_SetTxCmdPRACH:egprs rach,TS2"
#define FRAMEINSTRUTIL_7160_112_2_17_23_54_27_353 "L1:Rx data:fn curr:%d,tbf_start_fn :%d"
#define FRAMEINSTRUTIL_7196_112_2_17_23_54_27_354 "L1:TBFCon: 1, exe_band = %d, new_band = %d, pwr_idx = %d!"
#define FRAMEINSTRUTIL_7257_112_2_17_23_54_27_355 "L1: uplink tbf, plot rx frame, ta %d, data %d, polling %d"
#define FRAMEINSTRUTIL_7277_112_2_17_23_54_27_356 "L1:No TA,not send dl polling 1"
#define FRAMEINSTRUTIL_7296_112_2_17_23_54_27_357 "L1:No TA,not send dl polling 2"
#define FRAMEINSTRUTIL_7333_112_2_17_23_54_27_358 "No TA,not send both link data 1"
#define FRAMEINSTRUTIL_7352_112_2_17_23_54_27_359 "L1:No TA,not send both link data 2"
#define FRAMEINSTRUTIL_7434_112_2_17_23_54_27_360 "L1(polling) ready to replace TxPDTCH with plot pch frame, i:%d,bcch_delay: %d,polling_delay: %d"
#define FRAMEINSTRUTIL_7470_112_2_17_23_54_27_361 "L1_TD: Plot Gprs Tx Rscp, fn 0x%x"
#define FRAMEINSTRUTIL_7672_112_2_17_23_54_28_362 "L1 ready to replace RxPDTCH with plot pch frame, i:%d,bcch_delay: %d,polling_delay: %d"
#define FRAMEINSTRUTIL_7998_112_2_17_23_54_28_363 "L1(polling) ready to replace RxTxPDTCH with plot pch frame, i:%d,bcch_delay: %d,polling_delay: %d"
#define FRAMEINSTRUTIL_8108_112_2_17_23_54_28_364 "L1 conflitnew rx slot 0x%x"
#define FRAMEINSTRUTIL_8859_112_2_17_23_54_30_365 "L1_TD: Plot Gprs Pttch Rscp, fn 0x%x"
#define FRAMEINSTRUTIL_8865_112_2_17_23_54_30_366 "L1_TD: Plot Gprs Pttch before sync, fn 0x%x"
#define FRAMEINSTRUTIL_9027_112_2_17_23_54_30_367 "L1,Plot Tbf Search Frame.\n"
#define FRAMEINSTRUTIL_9050_112_2_17_23_54_30_368 "L1:TD SYNC ON GPRS SEARCH FRAME 0x%x"
#define FRAMEINSTRUTIL_9087_112_2_17_23_54_30_369 "L1:tbf read  = 0x%x"
#define FRAMEINSTRUTIL_9108_112_2_17_23_54_30_370 "L1:tbf search  = 0x%x"
#define FRAMEINSTRUTIL_9240_112_2_17_23_54_31_371 "L1 Search is not imp tbfPolling!"
#define FRAMEINSTRUTIL_9474_112_2_17_23_54_31_372 "L1:Assert Failure,Plot4AccessBurst "
#define FRAMEINSTRUTIL_9559_112_2_17_23_54_31_373 "L1(polling) ready to replace PollingRxTx with plot pch frame, i:%d,bcch_delay: %d,polling_delay: %d"
#define FRAMEINSTRUTIL_9728_112_2_17_23_54_32_374 "L1(polling) ready to replace accessburstrx with plot pch frame, i:%d,bcch_delay: %d,polling_delay: %d"
#define FRAMEINSTRUTIL_10217_112_2_17_23_54_33_375 "L1:FrameInstrUtil_SetTxCmdRACH:egprs rach,TS1"
#define FRAMEINSTRUTIL_10222_112_2_17_23_54_33_376 "L1:FrameInstrUtil_SetTxCmdRACH:egprs rach,TS2"
#define FRAMEINSTRUTIL_10391_112_2_17_23_54_34_377 "L1_TD: Plot Idle Agc Nop, dwpts_pos %d"
#define FRAMEINSTRUTIL_10405_112_2_17_23_54_34_378 "L1: idle agc no agc freq be found! "
#define FRAMEINSTRUTIL_10437_112_2_17_23_54_34_379 "L1:Try2PlotNopForDspTimingAjusting, ccchtn %d, 0x%x"
#define FRAMEINSTRUTIL_10506_112_2_17_23_54_34_380 "L1:PlotTS0RssiBCCHCamped delay %d,  fn 0x%x"
#define FRAMEINSTRUTIL_10511_112_2_17_23_54_34_381 "L1:PlotTS0RssiBCCHCamped delay %d, ind %d, mind %d"
#define FRAMEINSTRUTIL_10519_112_2_17_23_54_34_382 "L1:PlotTS0RssiBCCHCamped deltapos %d"
#define FRAMEINSTRUTIL_10530_112_2_17_23_54_34_383 "L1:PlotTS0RssiBCCHCamped idle ++, new postion %d"
#define FRAMEINSTRUTIL_10534_112_2_17_23_54_34_384 "L1:PlotTS0RssiBCCHCamped 1 ploted num %d"
#define FRAMEINSTRUTIL_10561_112_2_17_23_54_34_385 "L1:Plot idle TS0Rssi  delta %d, arfcn 0x%x"
#define FRAMEINSTRUTIL_10576_112_2_17_23_54_34_386 "L1:PlotTS0RssiBCCHCamped 1 ploted num %d"
#define FRAMEINSTRUTIL_10585_112_2_17_23_54_34_387 "L1:PlotTS0RssiBCCHCamped not finished yet , ind %d, plotnum %d"
#define FRAMEINSTRUTIL_10621_112_2_17_23_54_34_388 "L1_TD: FrameInstrUtil_PlotIdleTDTS0RssiMeas, curr_fn 0x%x, pch_delay %d, td_meas_fn_num %d sync_fn 0x%x, fn_pass_mod13 %d"
#define FRAMEINSTRUTIL_10633_112_2_17_23_54_34_389 "L1_TD: FrameInstrUtil_PlotIdleTDTS0RssiMeas, can't plot td, plot sleep %d frame"
#define FRAMEINSTRUTIL_10639_112_2_17_23_54_34_390 "L1_TD: Plot Idle TS0 Rssi Nop, ts0_delta_pos %d"
#define FRAMEINSTRUTIL_10648_112_2_17_23_54_34_391 "L1_TD: FrameInstrUtil_PlotIdleTDTS0RssiMeas, idle_frame_no %d, ts0_delta_pos %d"
#define FRAMEINSTRUTIL_10661_112_2_17_23_54_34_392 "L1_TD: FrameInstrUtil_PlotIdleTDTS0RssiMeas, plot sleep fn 0x%x, %d frame before td meas"
#define FRAMEINSTRUTIL_10672_112_2_17_23_54_34_393 "L1_TD: FrameInstrUtil_PlotIdleTDTS0RssiMeas, plot idle 0x%x after td meas, idle_frame_no %d"
#define FRAMEINSTRUTIL_10717_112_2_17_23_54_35_394 "L1_TD: Plot Idle Syncdl Nop, dwpts_delta_pos %d"
#define FRAMEINSTRUTIL_10768_112_2_17_23_54_35_395 "L1_TD: Plot Idle Mid Nop, mid_delta_pos %d"
#define FRAMEINSTRUTIL_10803_112_2_17_23_54_35_396 "L1_TD: FrameInstrUtil_PlotIdleTDRscpMeas, curr_fn 0x%x, pch_delay %d, td_meas_fn_num %d sync_fn 0x%x, fn_pass_mod13 %d"
#define FRAMEINSTRUTIL_10811_112_2_17_23_54_35_397 "L1_TD: g_td_cell_result.meas_td[%d].cell_num %d"
#define FRAMEINSTRUTIL_10813_112_2_17_23_54_35_398 "L1_TD: FrameInstrUtil_PlotIdleTDRscpMeas, can't plot td, plot sleep %d frame"
#define FRAMEINSTRUTIL_10825_112_2_17_23_54_35_399 "L1_TD: FrameInstrUtil_PlotIdleTDRscpMeas, can't plot td, plot sleep %d frame"
#define FRAMEINSTRUTIL_10830_112_2_17_23_54_35_400 "L1_TD: Plot Idle Rscp Nop, midam_pos %d"
#define FRAMEINSTRUTIL_10837_112_2_17_23_54_35_401 "L1_TD: FrameInstrUtil_PlotIdleTDRscpMeas, idle_frame_no %d, midam_pos %d"
#define FRAMEINSTRUTIL_10842_112_2_17_23_54_35_402 "L1_TD: FrameInstrUtil_PlotIdleTDRscpMeas, plot sleep fn 0x%x, %d frame before td meas"
#define FRAMEINSTRUTIL_10846_112_2_17_23_54_35_403 "L1_TD: Plot Idle Rscp, midam_pos %d, IDLE_TD_RSCP_MEAS_AND_SYNC"
#define FRAMEINSTRUTIL_10849_112_2_17_23_54_35_404 "L1_TD: g_td_cell_result.meas_td[%d].cell_num %d"
#define FRAMEINSTRUTIL_10857_112_2_17_23_54_35_405 "L1_TD: FrameInstrUtil_PlotIdleTDRscpMeas, plot idle 0x%x after td meas, idle_frame_no %d"
#define FRAMEINSTRUTIL_10884_112_2_17_23_54_35_406 "L1_TD: FrameInstrUtil_PlotIdleTDRssiMeas, curr_fn 0x%x, pch_delay %d, td_meas_fn_num %d sync_fn 0x%x, fn_pass_mod13 %d"
#define FRAMEINSTRUTIL_10896_112_2_17_23_54_35_407 "L1_TD: FrameInstrUtil_PlotIdleTDRssiMeas, can't plot td, plot sleep %d frame"
#define FRAMEINSTRUTIL_10901_112_2_17_23_54_35_408 "L1_TD: Plot Idle Rssi Nop, dwpts_delta_pos %d"
#define FRAMEINSTRUTIL_10908_112_2_17_23_54_35_409 "L1_TD: FrameInstrUtil_PlotIdleTDRssiMeas, idle_frame_no %d, dwpts_delta_pos %d"
#define FRAMEINSTRUTIL_10913_112_2_17_23_54_35_410 "L1_TD: FrameInstrUtil_PlotIdleTDRssiMeas, plot sleep fn 0x%x, %d frame before td meas"
#define FRAMEINSTRUTIL_10923_112_2_17_23_54_35_411 "L1_TD: FrameInstrUtil_PlotIdleTDRssiMeas, plot idle 0x%x after td meas, idle_frame_no %d"
#define FRAMEINSTRUTIL_10967_112_2_17_23_54_35_412 "L1_TD: txpdtch g_td_gprs_meas_result_reveived is %d, status %d"
#define FRAMEINSTRUTIL_10984_112_2_17_23_54_35_413 "L1_TD: dwpts_pos %d, midam_pos %d"
#define FRAMEINSTRUTIL_10991_112_2_17_23_54_35_414 "L1_TD: BetweenRxTx AfterTx, begin %d, end %d"
#define FRAMEINSTRUTIL_10997_112_2_17_23_54_35_415 "L1_TD: AfterTx, begin %d, end %d"
#define FRAMEINSTRUTIL_11024_112_2_17_23_54_35_416 "L1_TD: g_td_cell_result.meas_td[%d].cell_num %d"
#define FRAMEINSTRUTIL_11038_112_2_17_23_54_35_417 "L1_TD: g_td_cell_result.meas_td[%d].cell_num %d"
#define FRAMEINSTRUTIL_11076_112_2_17_23_54_35_418 "L1_TD: rxpdtch meas_result_reveived is %d, status %d"
#define FRAMEINSTRUTIL_11093_112_2_17_23_54_36_419 "L1_TD: dwpts_pos %d, midam_pos %d"
#define FRAMEINSTRUTIL_11100_112_2_17_23_54_36_420 "L1_TD: BetweenRxTx, begin %d, end %d"
#define FRAMEINSTRUTIL_11110_112_2_17_23_54_36_421 "L1_TD: g_td_cell_result.meas_td[%d].cell_num %d"
#define FRAMEINSTRUTIL_11124_112_2_17_23_54_36_422 "L1_TD: g_td_cell_result.meas_td[%d].cell_num %d"
#define FRAMEINSTRUTIL_11176_112_2_17_23_54_36_423 "L1_TD: rxtxpdtch meas_result_reveived is %d, status %d"
#define FRAMEINSTRUTIL_11193_112_2_17_23_54_36_424 "L1_TD: dwpts_pos %d, midam_pos %d"
#define FRAMEINSTRUTIL_11199_112_2_17_23_54_36_425 "L1_TD: BetweenRxTx, begin %d, end %d"
#define FRAMEINSTRUTIL_11207_112_2_17_23_54_36_426 "L1_TD: AfterTx, begin %d, end %d"
#define FRAMEINSTRUTIL_11215_112_2_17_23_54_36_427 "L1_TD: LastTwoBurst, begin %d, end %d"
#define FRAMEINSTRUTIL_11228_112_2_17_23_54_36_428 "L1_TD: g_td_cell_result.meas_td[%d].cell_num %d"
#define FRAMEINSTRUTIL_11241_112_2_17_23_54_36_429 "L1_TD: g_td_cell_result.meas_td[%d].cell_num %d"
#define FRAMEINSTRUTIL_11290_112_2_17_23_54_36_430 "L1_TD: FrameInstrUtil_RxPtcchPlotTdRscpMeas g_td_gprs_meas_result_reveived is %d"
#define FRAMEINSTRUTIL_11291_112_2_17_23_54_36_431 "L1_TD: g_td_cell_result.meas_td[%d].cell_num %d"
#define FRAMEINSTRUTIL_11310_112_2_17_23_54_36_432 "L1_TD: midam_pos %d, begin %d, end %d"
#define FRAMEINSTRUTIL_11316_112_2_17_23_54_36_433 "L1_TD: g_td_cell_result.meas_td[%d].cell_num %d"
#define FRAMEINSTRUTIL_11367_112_2_17_23_54_36_434 "L1_TD: for tbf search tch meas no frame5!"
#define FRAMEINSTRUTIL_11375_112_2_17_23_54_36_435 "L1_TD: Plot Gprs Idle1 Before Sync, fn 0x%x"
#define FRAMEINSTRUTIL_11413_112_2_17_23_54_37_436 "L1_TD: g_td_gprs_meas_result_reveived is %d"
#define FRAMEINSTRUTIL_11414_112_2_17_23_54_37_437 "L1_TD: g_td_cell_result.meas_td[%d].cell_num %d"
#define FRAMEINSTRUTIL_11432_112_2_17_23_54_37_438 "L1_TD: midam_pos %d, begin %d, end %d"
#define FRAMEINSTRUTIL_11438_112_2_17_23_54_37_439 "L1_TD: g_td_cell_result.meas_td[%d].cell_num %d"
#define FRAMEINSTRUTIL_11520_112_2_17_23_54_37_440 "L1_TD: g_td_gprs_meas_result_reveived is FALSE"
#define FRAMEINSTRUTIL_11535_112_2_17_23_54_37_441 "L1_TD: fn 0x%x, dwpts_pos %d, ts0_delta_pos %d"
#define FRAMEINSTRUTIL_11542_112_2_17_23_54_37_442 "L1_TD: BetweenRxTx AfterTx, begin %d, end %d"
#define FRAMEINSTRUTIL_11548_112_2_17_23_54_37_443 "L1_TD: AfterTx, begin %d, end %d"
#define FRAMEINSTRUTIL_11568_112_2_17_23_54_37_444 "L1_TD: TS0_RSSI"
#define FRAMEINSTRUTIL_11613_112_2_17_23_54_37_445 "L1_TD: g_td_gprs_meas_result_reveived is FALSE"
#define FRAMEINSTRUTIL_11629_112_2_17_23_54_37_446 "L1_TD: fn 0x%x, dwpts_pos %d, ts0_delta_pos %d"
#define FRAMEINSTRUTIL_11636_112_2_17_23_54_37_447 "L1_TD: BetweenRxTx, begin %d, end %d"
#define FRAMEINSTRUTIL_11650_112_2_17_23_54_37_448 "L1_TD: TS0_RSSI"
#define FRAMEINSTRUTIL_11697_112_2_17_23_54_37_449 "L1_TD: g_td_gprs_meas_result_reveived is FALSE"
#define FRAMEINSTRUTIL_11713_112_2_17_23_54_37_450 "L1_TD: fn 0x%x, dwpts_pos %d, ts0_delta_pos %d"
#define FRAMEINSTRUTIL_11750_112_2_17_23_54_37_451 "L1_TD: TS0_RSSI"
#define FRAMEINSTRUTIL_11794_112_2_17_23_54_37_452 "L1: RxPtcchPlotTdProcess coper Locked."
#define FRAMEINSTRUTIL_11813_112_2_17_23_54_37_453 "L1_TD: ts0_delta_pos %d, midam_pos %d, dwpts_pos %d, begin %d, end %d"
#define FRAMEINSTRUTIL_11820_112_2_17_23_54_38_454 "L1_TD: Plot Pttch Rssi"
#define FRAMEINSTRUTIL_11841_112_2_17_23_54_38_455 "L1_TD: Plot Pttch Ts0 Rssi"
#define FRAMEINSTRUTIL_11863_112_2_17_23_54_38_456 "L1_TD: Plot Pttch Syncdl"
#define FRAMEINSTRUTIL_11888_112_2_17_23_54_38_457 "L1_TD: Plot Pttch Mid"
#define FRAMEINSTRUTIL_11931_112_2_17_23_54_38_458 "L1_TD: Plot Pttch Agc"
#define FRAMEINSTRUTIL_11972_112_2_17_23_54_38_459 "L1:GprsIdleFramePlotTdProcess coper locked."
#define FRAMEINSTRUTIL_11994_112_2_17_23_54_38_460 "L1_TD: mid_pos %d, ts0_delta_pos %d, dwpts_pos %d, begin %d, end %d"
#define FRAMEINSTRUTIL_12001_112_2_17_23_54_38_461 "L1_TD: Plot Gprs Idle Rssi"
#define FRAMEINSTRUTIL_12032_112_2_17_23_54_38_462 "L1_TD: Plot Gprs Idle Ts0 Rssi"
#define FRAMEINSTRUTIL_12054_112_2_17_23_54_38_463 "L1_TD: for tbf search tch meas no frame1!"
#define FRAMEINSTRUTIL_12073_112_2_17_23_54_38_464 "L1_TD: Plot Gprs Idle Syncdl, counter %d,"
#define FRAMEINSTRUTIL_12086_112_2_17_23_54_38_465 "L1_TD: td_meas_ind %d"
#define FRAMEINSTRUTIL_12112_112_2_17_23_54_38_466 "L1_TD: for tbf search tch meas no frame2!"
#define FRAMEINSTRUTIL_12123_112_2_17_23_54_38_467 "L1_TD: Plot Gprs Idle Mid, counter %d,"
#define FRAMEINSTRUTIL_12136_112_2_17_23_54_38_468 "L1_TD: td_meas_ind %d"
#define FRAMEINSTRUTIL_12160_112_2_17_23_54_38_469 "L1_TD: for tbf search tch meas no frame3!"
#define FRAMEINSTRUTIL_12170_112_2_17_23_54_38_470 "L1_TD: Plot Gprs Idle Agc"
#define FRAMEINSTRUTIL_12211_112_2_17_23_54_38_471 "L1_TD: for tbf search tch meas no frame4!"
#define FRAMEINSTRUTIL_12236_112_2_17_23_54_38_472 "L1_TD: Plot Gprs Pwr, ts0_delta_pos %d, fn 0x%x"
#define FRAMEINSTRUTIL_12265_112_2_17_23_54_38_473 "L1:PlotGprsTDSync coper locked."
#define FRAMEINSTRUTIL_12299_112_2_17_23_54_38_474 "L1_TD: Plot Gprs Sync, fn 0x%x, process_counter %d, ts0_delta_pos %d"
#define FRAMEINSTRUTIL_12305_112_2_17_23_54_38_475 "L1_TD: Plot Gprs Sync, Wrong overflowed counter %d, fn 0x%x"
#define FRAMEINSTRUTIL_12381_112_2_17_23_54_39_476 "L1:PlotTxPdtchTdAdjustGain coper locked."
#define FRAMEINSTRUTIL_12392_112_2_17_23_54_39_477 "L1_TD: Plot Gprs Agc BetweenRxTx AfterTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d, end %d"
#define FRAMEINSTRUTIL_12398_112_2_17_23_54_39_478 "L1_TD: Plot Gprs Agc AfterTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d, end %d"
#define FRAMEINSTRUTIL_12449_112_2_17_23_54_39_479 "L1:PlotRxPdtchTdAdjustGain coper locked"
#define FRAMEINSTRUTIL_12467_112_2_17_23_54_39_480 "L1_TD: Plot Gprs Agc BetweenRxTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d, end %d"
#define FRAMEINSTRUTIL_12513_112_2_17_23_54_39_481 "L1:PlotRxTxPdtchTdAdjustGain coper locked"
#define FRAMEINSTRUTIL_12524_112_2_17_23_54_39_482 "L1_TD: Plot Gprs Agc BetweenRxTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d, end %d"
#define FRAMEINSTRUTIL_12531_112_2_17_23_54_39_483 "L1_TD: Plot Gprs Agc AfterTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d, end %d"
#define FRAMEINSTRUTIL_12538_112_2_17_23_54_39_484 "L1_TD: Plot Gprs Agc LastTwoBurst, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d, end %d"
#define FRAMEINSTRUTIL_12591_112_2_17_23_54_39_485 "L1:PlotGPRSTDSyncdlSearch coper locked."
#define FRAMEINSTRUTIL_12613_112_2_17_23_54_39_486 "L1_TD: Plot Gprs Syncdl BetweenRxTx AfterTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d"
#define FRAMEINSTRUTIL_12618_112_2_17_23_54_39_487 "L1_TD: Plot Gprs Syncdl AfterTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d"
#define FRAMEINSTRUTIL_12629_112_2_17_23_54_39_488 "L1_TD: Plot Gprs Syncdl BetweenRxTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d"
#define FRAMEINSTRUTIL_12640_112_2_17_23_54_39_489 "L1_TD: Plot Gprs Syncdl BetweenRxTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d"
#define FRAMEINSTRUTIL_12645_112_2_17_23_54_39_490 "L1_TD: Plot Gprs Syncdl AfterTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d"
#define FRAMEINSTRUTIL_12650_112_2_17_23_54_39_491 "L1_TD: Plot Gprs Syncdl LastTwoBurst, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d"
#define FRAMEINSTRUTIL_12709_112_2_17_23_54_39_492 "L1: PlotGPRSTDMidConf coper Locked."
#define FRAMEINSTRUTIL_12731_112_2_17_23_54_39_493 "L1_TD: Plot Gprs Syncdl BetweenRxTx AfterTx, fn 0x%x, process_counter %d, begin %d"
#define FRAMEINSTRUTIL_12736_112_2_17_23_54_39_494 "L1_TD: Plot Gprs Syncdl AfterTx, fn 0x%x, process_counter %d, begin %d"
#define FRAMEINSTRUTIL_12747_112_2_17_23_54_39_495 "L1_TD: Plot Gprs Syncdl BetweenRxTx, fn 0x%x, process_counter %d, begin %d"
#define FRAMEINSTRUTIL_12758_112_2_17_23_54_39_496 "L1_TD: Plot Gprs Syncdl BetweenRxTx, fn 0x%x, process_counter %d, begin %d"
#define FRAMEINSTRUTIL_12763_112_2_17_23_54_39_497 "L1_TD: Plot Gprs Syncdl AfterTx, fn 0x%x, process_counter %d, begin %d"
#define FRAMEINSTRUTIL_12768_112_2_17_23_54_39_498 "L1_TD: Plot Gprs Syncdl LastTwoBurst, fn 0x%x, process_counter %d, begin %d"
#define GPRSFRAMEGEN_588_112_2_17_23_54_41_499 "L1:Waiting for wakeup's response!"
#define GPRSFRAMEGEN_725_112_2_17_23_54_41_500 "L1: block_delay %d,  fn %x "
#define GPRSFRAMEGEN_744_112_2_17_23_54_41_501 "send polling data before assign"
#define GPRSFRAMEGEN_947_112_2_17_23_54_42_502 "L1:starting time reach0: %d,%d"
#define GPRSFRAMEGEN_1039_112_2_17_23_54_42_503 "single fn reach1 : %d"
#define GPRSFRAMEGEN_1112_112_2_17_23_54_42_504 "single fn reach2 : %d"
#define GPRSFRAMEGEN_1169_112_2_17_23_54_42_505 "L1 should receive vicecard paging :%d from NV"
#define GPRSFRAMEGEN_1187_112_2_17_23_54_42_506 "L1 bcch delay %x,pch_delay %x,min_delay %x,tbf_delay %x, fn_m52 %x, polling_delay:%d,map %d"
#define GPRSFRAMEGEN_1288_112_2_17_23_54_43_507 "L1 ready to plot pch frame in remnant frame before PDTCH,i:%d,delay:%d,bcch_delay:%d"
#define GPRSFRAMEGEN_1341_112_2_17_23_54_43_508 "L1 invalid tbf state %d"
#define GPRSSYSINFO_234_112_2_17_23_54_43_509 "L1 dl-ul collision at tbf param,tx_mask %x, primitive rx_mask %x, new rx_mask %x"
#define GPRSSYSINFO_302_112_2_17_23_54_44_510 "L1 the overframes slots(tx) is cancelled! %x"
#define GPRSSYSINFO_308_112_2_17_23_54_44_511 "L1 the the overframes slots(rx) is cancelled! %x"
#define GPRSSYSINFO_766_112_2_17_23_54_45_512 "L1:Assert Failure,NO Allocted Block Memory."
#define GPRSSYSINFO_1272_112_2_17_23_54_46_513 "L1: polling ack and rx collision tx_slot 0x%x, rx_slot 0x%x, new rx_slot 0x%x"
#define GPRSSYSINFO_1375_112_2_17_23_54_46_514 "L1API_GetTBFSlotMask: error tbf_direction = %d!"
#define GPRSSYSINFO_1428_112_2_17_23_54_46_515 "L1:wrong tbf mode %d!"
#define GPRSSYSINFO_1431_112_2_17_23_54_46_516 "L1: reset tai,tbf_direction %d"
#define GPRSSYSINFO_1454_112_2_17_23_54_46_517 "L1 access polling send_type %d"
#define GPRSSYSINFO_1464_112_2_17_23_54_46_518 "L1 normal polling, ta_valid %d"
#define GPRSSYSINFO_1538_112_2_17_23_54_46_519 "L1 invalid tbf!"
#define GPRSSYSINFO_1554_112_2_17_23_54_46_520 "L1: exe tbf direction error"
#define GPRSSYSINFO_1772_112_2_17_23_54_47_521 "L1_TD: gprs tx_rx_slot_in_one_frame 0x%x"
#define GSMFRAMEGEN_308_112_2_17_23_54_48_522 "L1 GsmFrameGen_NullState arfcn 0x%x,band 0x%x,gain 0x%x"
#define GSMFRAMEGEN_555_112_2_17_23_54_48_523 "L1:pchbcch_select_type=%d,delay:%d ,%d,fn=x%x"
#define GSMFRAMEGEN_559_112_2_17_23_54_48_524 "bcch %x, cell id %d"
#define GSMFRAMEGEN_712_112_2_17_23_54_48_525 "L1:pchbcch_select_type=%d,delay:%d ,%d,fn=x%x"
#define GSMFRAMEGEN_716_112_2_17_23_54_48_526 "L1 bcch cell id 0x%x, arfcn 0x%x"
#define GSMFRAMEGEN_784_112_2_17_23_54_49_527 "L1 sub_state 0x%x, cbch_delay 0x%x,%x,%x"
#define GSMFRAMEGEN_1119_112_2_17_23_54_49_528 "L1 pwr sub state %d"
#define GSMFRAMEGEN_1150_112_2_17_23_54_49_529 "L1:optimize power on,enter POWERSWEEP_RESULT_PENDING"
#define GSMFRAMEGEN_1319_112_2_17_23_54_50_530 "Waiting for wakeup's response!!!"
#define GSMFRAMEGEN_1373_112_2_17_23_54_50_531 "Waiting for wakeup's response!"
#define GSMFRAMEGEN_1478_112_2_17_23_54_50_532 "L1:Invalid BCCH_CAMP sub state!"
#define GSMFRAMEGEN_1534_112_2_17_23_54_50_533 "Waiting for wakeup's response!!!"
#define GSMFRAMEGEN_1591_112_2_17_23_54_50_534 "l1:PLMN CAMP, SEND ASKMORE TO SELF"
#define GSMFRAMEGEN_1734_112_2_17_23_54_51_535 "Waiting for wakeup's response!!!"
#define GSMFRAMEGEN_1789_112_2_17_23_54_51_536 "L1: Invalid PBCCH_CAMP sub state!"
#define GSMFRAMEGEN_1928_112_2_17_23_54_51_537 "L1:Waiting for wakeup's response!"
#define GSMFRAMEGEN_2139_112_2_17_23_54_51_538 "L1:Waiting for wakeup's response!"
#define GSMFRAMEGEN_2263_112_2_17_23_54_52_539 "L1 adjust first send fn %x,current fn %x"
#define GSMFRAMEGEN_2706_112_2_17_23_54_52_540 "L1:to cell's arfcn %d"
#define GSMFRAMEGEN_2800_112_2_17_23_54_53_541 "L1:current chan matched with next chan!"
#define GSMFRAMEGEN_3244_112_2_17_23_54_54_542 "L1: search handover cell 0x%x"
#define GSMFRAMEGEN_3361_112_2_17_23_54_54_543 "L1:TDMeasPCH every page delay is %d, count is 0"
#define GSMFRAMEGEN_3367_112_2_17_23_54_54_544 "L1:GenTDMeasEveryPCH start fn 0x%x, pchdelay 0x%x"
#define GSMFRAMEGEN_3382_112_2_17_23_54_54_545 "L1:GenTDMeasEveryPCH end fn 0x%x, pchtemp 0x%x"
#define GSMFRAMEGEN_3411_112_2_17_23_54_54_546 "L1:td meas pch  a 0 pchdelay hanpped."
#define GSMFRAMEGEN_3417_112_2_17_23_54_54_547 "L1_TD: pch delay %d, is_td_meas %d"
#define GSMFRAMEGEN_3503_112_2_17_23_54_54_548 "L1 suppored cbch 0x%x, required 0x%x"
#define GSMFRAMEGEN_3522_112_2_17_23_54_54_549 "L1 sub_state 0x%x, cbch_delay 0x%x"
#define GSMFRAMEGEN_3526_112_2_17_23_54_54_550 "L1 quick pwr frame min delay %d"
#define GSMFRAMEGEN_3561_112_2_17_23_54_54_551 "L1:GenTDSync FALSE,g_is_td_meas %d, g_is_td_syncd %d, g_is_td_syncdl_mid_cnf %d, g_td_is_after_bcch %d, g_is_dsp_wake_up %d, g_is_td_sync_fail %d"
#define LAYER1_AUDIO_236_112_2_17_23_54_56_552 "LAYER1_PlaySpecialTone: tone_id = %d, b_uplink = %d,volume %d"
#define LAYER1_AUDIO_292_112_2_17_23_54_56_553 "LAYER1_PlayGenericTone: freq1 = %d, freq2 = %d, freq3 = %d"
#define LAYER1_AUDIO_348_112_2_17_23_54_56_554 "LAYER1_StopTone: TRUE"
#define LAYER1_AUDIO_400_112_2_17_23_54_57_555 "LAYER1_SetVolume: AD_gain = %d"
#define LAYER1_AUDIO_409_112_2_17_23_54_57_556 "L1 low: SetVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX_ID\n"
#define LAYER1_AUDIO_420_112_2_17_23_54_57_557 "L1 low: TD SetVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX\n"
#define LAYER1_AUDIO_455_112_2_17_23_54_57_558 "L1 low: SetVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX\n"
#define LAYER1_AUDIO_470_112_2_17_23_54_57_559 "L1 low: SetVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX\n"
#define LAYER1_AUDIO_492_112_2_17_23_54_57_560 "L1 low: SetVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX\n"
#define LAYER1_AUDIO_507_112_2_17_23_54_57_561 "L1 low: SetVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX\n"
#define LAYER1_AUDIO_535_112_2_17_23_54_57_562 "LAYER1_MuteSwitch: b_mute = %d"
#define LAYER1_AUDIO_615_112_2_17_23_54_57_563 "LAYER1_OpenSoundDevice1: Mic = %d, Speaker = %d"
#define LAYER1_AUDIO_646_112_2_17_23_54_57_564 "LAYER1_SwitchSoundDevice: Mic = %d, Speaker = %d, DevMode = %d"
#define LAYER1_AUDIO_659_112_2_17_23_54_57_565 "L1c:td NO realize"
#define LAYER1_AUDIO_736_112_2_17_23_54_57_566 "LAYER1_SwitchTestMode: is_test_mode = %d"
#define LAYER1_AUDIO_774_112_2_17_23_54_57_567 "LAYER1_PlaySideTone: tone_gain = %d"
#define LAYER1_AUDIO_788_112_2_17_23_54_57_568 "LAYER1_SwitchEchoCancel: is_echo_cancel_on = %d"
#define LAYER1_AUDIO_878_112_2_17_23_54_57_569 "L1C:Start VT AMR data exchange!Codec:%d"
#define LAYER1_AUDIO_882_112_2_17_23_54_58_570 "L1C:Ignore VT start command in GSM!"
#define LAYER1_AUDIO_901_112_2_17_23_54_58_571 "L1C:Stop VT AMR data exchange!Codec:%d"
#define LAYER1_AUDIO_906_112_2_17_23_54_58_572 "L1:send audio&codec stop command in GSM!"
#define LAYER1_AUDIO_916_112_2_17_23_54_58_573 "LAYER1:LAYER1_StartRecord_Downlink,mode:%d"
#define LAYER1_AUDIO_937_112_2_17_23_54_58_574 "LAYER1:LAYER1_StartRecord_Ad,mode:%d"
#define LAYER1_AUDIO_959_112_2_17_23_54_58_575 "LAYER1:LAYER1_SendVoiceDataToDA,mode:%d"
#define LAYER1_AUDIO_975_112_2_17_23_54_58_576 "LAYER1:LAYER1_SendVoiceDataToNet,mode:%d"
#define LAYER1_AUDIO_998_112_2_17_23_54_58_577 "L1:cs amr record uplink call back fun not registered loopback!"
#define LAYER1_AUDIO_1010_112_2_17_23_54_58_578 "L1:cs amr record downlink call back fun not registered! loopback"
#define LAYER1_AUDIO_1032_112_2_17_23_54_58_579 "LAYER1_SetAudioEngineeringParameter"
#define LAYER1_AUDIO_1126_112_2_17_23_54_58_580 "LAYER1_GetAudioEngineeringParameter"
#define LAYER1_AUDIO_1203_112_2_17_23_54_58_581 "layer1 commanding DSP echo testing"
#define LAYER1_AUDIO_1239_112_2_17_23_54_58_582 "LAYER1_SendEchoResult"
#define LAYER1_AUDIO_1428_112_2_17_23_54_59_583 "L1: voice_format %d,dsp_upload_format= %x,is_from_mic=%x,is_from_net=%x,block_count %d,block_length %d arfcn %x,td_flag %d"
#define LAYER1_AUDIO_1443_112_2_17_23_54_59_584 "L1:AMR_MODE = %d"
#define LAYER1_AUDIO_1538_112_2_17_23_54_59_585 "LAYER1_StopRecord"
#define LAYER1_AUDIO_1593_112_2_17_23_54_59_586 "\"\"# VOICE FORMAT = %x,speaker = %d, net=%d"
#define LAYER1_AUDIO_1615_112_2_17_23_54_59_587 "LAYER1_WriteVoiceData"
#define LAYER1_AUDIO_1688_112_2_17_23_54_59_588 "\"\"# HANDLE NODATA format==%x"
#define LAYER1_AUDIO_1764_112_2_17_23_54_59_589 "\"\"DL_equalizer_coeff handset[0] = %x"
#define LAYER1_AUDIO_1765_112_2_17_23_54_59_590 "\"\"UL_equalizer_coeff handfree[0] = %x"
#define LAYER1_AUDIO_1766_112_2_17_23_54_59_591 "\"\"DL_equalizer_coeff handfree[0] = %x"
#define LAYER1_AUDIO_1767_112_2_17_23_54_59_592 "\"\"UL_equalizer_coeff earphone[0] = %x"
#define LAYER1_AUDIO_1768_112_2_17_23_54_59_593 "\"\"DL_equalizer_coeff earphone[0] = %x"
#define LAYER1_AUDIO_1770_112_2_17_23_54_59_594 "\"\"UL_equalizer_coeff handset[32] = %X"
#define LAYER1_AUDIO_1771_112_2_17_23_54_59_595 "\"\"DL_equalizer_coeff handset[32] = %x"
#define LAYER1_AUDIO_1772_112_2_17_23_54_59_596 "\"\"UL_equalizer_coeff handfree[32] = %x"
#define LAYER1_AUDIO_1773_112_2_17_23_54_59_597 "\"\"DL_equalizer_coeff handfree[32] = %x"
#define LAYER1_AUDIO_1774_112_2_17_23_54_59_598 "\"\"UL_equalizer_coeff earphone[32] = %x"
#define LAYER1_AUDIO_1775_112_2_17_23_54_59_599 "\"\"DL_equalizer_coeff earphone[32] = %x"
#define LAYER1_AUDIO_1777_112_2_17_23_54_59_600 "\"\"earphone DL_IIR1_coeff[0] = %X"
#define LAYER1_AUDIO_1778_112_2_17_23_54_59_601 "\"\"handfree DL_IIR2_coeff[0] = %X"
#define LAYER1_AUDIO_1780_112_2_17_23_54_59_602 "\"\"earphone DL_IIR1_coeff[5] = %X"
#define LAYER1_AUDIO_1781_112_2_17_23_54_59_603 "\"\"handfree DL_IIR2_coeff[5] = %X"
#define LAYER1_AUDIO_1783_112_2_17_23_54_59_604 "\"\" UL_PGA_gain = %x"
#define LAYER1_AUDIO_1784_112_2_17_23_54_59_605 "\"\" DL_PGA_gain = %x"
#define LAYER1_AUDIO_1785_112_2_17_23_54_59_606 "\"\" UL_digital_gain = %x"
#define LAYER1_AUDIO_1786_112_2_17_23_54_59_607 "\"\" DL_digital_gain = %x"
#define LAYER1_AUDIO_1787_112_2_17_23_54_59_608 "\"\" UL_digital_scale = %x"
#define LAYER1_AUDIO_1788_112_2_17_23_54_59_609 "\"\" DL_digital_scale = %x"
#define LAYER1_AUDIO_1789_112_2_17_23_54_59_610 "\"\" midi_PGA_gain_base = %x"
#define LAYER1_AUDIO_1790_112_2_17_23_54_59_611 "\"\" digital_sidetone_gain = %x"
#define LAYER1_AUDIO_1797_112_2_17_23_54_59_612 "\"\" eq_type= %X"
#define LAYER1_AUDIO_1827_112_2_17_23_54_59_613 "LAYER1_SetDevModeParamEx: audio3 param_buffer_ptr->dl_da_device_l 0x%x, g_audio_length %d"
#define LAYER1_AUDIO_1828_112_2_17_23_54_59_614 "LAYER1_SetDevModeParamEx: audio3 td_param_buffer_ptr->dl_da_device_l 0x%x, g_td_audio_length %d"
#define LAYER1_AUDIO_1922_112_2_17_23_55_0_615 "audio3: LAYER1_SetDevModeParam API, g_mode_flag %d"
#define LAYER1_AUDIO_1931_112_2_17_23_55_0_616 "audio3: 1 param_buffer_ptr 0x%x, g_audio_length %d"
#define LAYER1_AUDIO_1970_112_2_17_23_55_0_617 "audio3: 2 param_buffer_ptr 0x%x, g_audio_length %d"
#define LAYER1_AUDIO_1972_112_2_17_23_55_0_618 "LAYER1_SetDevModeParam,length=%d"
#define LAYER1_AUDIO_1990_112_2_17_23_55_0_619 "L1 low: SetVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX_ID\n"
#define LAYER1_AUDIO_2000_112_2_17_23_55_0_620 "L1 low: TD SetDevtVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX\n"
#define LAYER1_AUDIO_2046_112_2_17_23_55_0_621 "L1 low: SetVolume speak_vol = %d "
#define LAYER1_AUDIO_2084_112_2_17_23_55_0_622 "LAYER1_SetUlnkMutel "
#define LAYER1_AUDIO_2119_112_2_17_23_55_0_623 "LAYER1_SetDlnkMute "
#define LAYER1_AUDIO_2133_112_2_17_23_55_0_624 "L1 iq catch param %d, %d, %d ,%d"
#define LAYER1_AUDIO_2151_112_2_17_23_55_0_625 "L1:s_silencecallback_pfunc!"
#define LAYER1_AUDIO_2156_112_2_17_23_55_0_626 "L1: s_silencecallback_pfunc is PNULL!"
#define LAYER1_AUDIO_2206_112_2_17_23_55_0_627 "L1:LAYER1_SetMagicVoiceParam,%d,%d,%d,%d,%d"
#define LAYER1_AUDIO_2262_112_2_17_23_55_0_628 "L1:LAYER1_StopMagicVoiceFunc,%d,%d,%d,%d,%d"
#define LAYER1_AUDIO_2291_112_2_17_23_55_0_629 "L1:LAYER1_ExchangeMagicVoiceData call back fun not registered!"
#define LAYER1_AUDIO_2326_112_2_17_23_55_0_630 "L1:LAYER1_SetMixVoiceParam,%d,%d,%d,%d,%d"
#define LAYER1_AUDIO_2377_112_2_17_23_55_1_631 "L1:LAYER1_StopMixVoiceFunc,%d,%d,%d,%d,%d"
#define LAYER1_AUDIO_2422_112_2_17_23_55_1_632 "L1:LAYER1_SetMixVoiceData call back fun not registered!"
#define LAYER1_AUDIO_2466_112_2_17_23_55_1_633 "UplinkPcmDataTrans_Start flag:0x%x,speaker = %d, net=%d"
#define LAYER1_AUDIO_2502_112_2_17_23_55_1_634 "LAYER1_UplinkPcmDataTrans_Stop flag:0x%x"
#define LAYER1_AUDIO_2554_112_2_17_23_55_1_635 "LAYER1_DownlinkPcmDataTrans_Start flag:0x%x"
#define LAYER1_AUDIO_2590_112_2_17_23_55_1_636 "LAYER1_DownlinkPcmDataTrans_Stop"
#define LAYER1_AUDIO_2616_112_2_17_23_55_1_637 "L1:downlink_pcm_callback call back fun not registered!"
#define LAYER1_AUDIO_2635_112_2_17_23_55_1_638 "L1:uplink_pcm_callback function not registered!"
#define LAYER1_AUDIO_2640_112_2_17_23_55_1_639 "LAYER1_SetBTUploadData not Succ!!"
#define LAYER1_AUDIO_CODEC_78_112_2_17_23_55_1_640 "LAYER1_AUDIO_CODEC_Data 0x%x"
#define LAYER1_AUDIO_CODEC_144_112_2_17_23_55_2_641 "LAYER1_AUDIO_CODEC_CopyData to:0x%x, from:0x%x, len:0x%x."
#define LAYER1_AUDIO_CODEC_168_112_2_17_23_55_2_642 "LAYER1_AUDIO_CODEC_CopyData data count is 0"
#define LAYER1_AUDIO_CODEC_185_112_2_17_23_55_2_643 "LAYER1_AUDIO_CODEC_CopyLRData l:0x%x, r:0x%x."
#define LAYER1_AUDIO_CODEC_206_112_2_17_23_55_2_644 "LAYER1_AUDIO_CODEC_CopyLRData data count is 0"
#define LAYER1_DIAG_235_112_2_17_23_55_2_645 "DIAG: send signal to RR\n"
#define LAYER1_DIAG_389_112_2_17_23_55_3_646 "bef fastcali isnv %d, band %d, type %d, index %d, length %d"
#define LAYER1_DIAG_493_112_2_17_23_55_3_647 "L1 diag: diag_msg_ptr->index is out range of data_length!"
#define LAYER1_DIAG_624_112_2_17_23_55_3_648 "layer1 diag: para_band error!"
#define LAYER1_DIAG_981_112_2_17_23_55_4_649 "L1:the NV_ID_IMEI is wrong!"
#define LAYER1_DIAG_1035_112_2_17_23_55_4_650 "L1:NV RF solution hasn't been read out!"
#define LAYER1_DIAG_1153_112_2_17_23_55_4_651 "L1 this function is implemented only at 8800s (Layer1_HandleDspDebug())"
#define LAYER1_DIAG_1231_112_2_17_23_55_4_652 "cali flag %d"
#define LAYER1_DIAG_1258_112_2_17_23_55_4_653 "fastcali diag ptr1 0x%x, pc_sig_ptr 0x%x, *pc_sig_ptr 0x%x"
#define LAYER1_DIAG_1280_112_2_17_23_55_4_654 "fastcali diag cmd 0x%x, count %d, p1 %d, p2 %d, p3 %d, p4 %d"
#define LAYER1_FREQ_93_112_2_17_23_55_5_655 "L1: IT's a WRONG frequency arfcn! 0x%x\n"
#define LAYER1_MAC_PROC_230_112_2_17_23_55_6_656 "L1:overall_bep:gmsk_mean_bep=%d,gmsk_cv_bep=%d,psk8_mean_bep=%d,psk8_cv_bep=%d,mean_bep_tn_present=%d"
#define LAYER1_MAC_PROC_323_112_2_17_23_55_6_657 "L1:dump the polling,out of dsp's ability,polling_slot=%d,first rx_slot=%d"
#define LAYER1_MAC_PROC_455_112_2_17_23_55_6_658 "L1 polling data is not write  %d, %d!"
#define LAYER1_MAC_PROC_497_112_2_17_23_55_6_659 "LAYER1: It's Layer1Mac_GetDataAndWriteData function\n"
#define LAYER1_MAC_PROC_535_112_2_17_23_55_7_660 "L1 polling data is not write to dsp %d, %d!"
#define LAYER1_MAIN_2966_112_2_17_23_55_13_661 "L1_READ NV: start %d"
#define LAYER1_MAIN_2985_112_2_17_23_55_13_662 " td sync fail timer inteval : %d"
#define LAYER1_MAIN_3022_112_2_17_23_55_13_663 "L1_TD: nv flag, test_bsic_fail %d, test_bcch_fail %d, test_ab_mode %d, test_mode_change %d"
#define LAYER1_MAIN_3023_112_2_17_23_55_13_664 "L1_TD: nv flag, test_tbf_time %d, g_td_first_time_syncdl_arfcn_num %d, g_td_max_syncdl_arfcn_num %d"
#define LAYER1_MAIN_3026_112_2_17_23_55_13_665 "L1_READ NV: finish %d"
#define LAYER1_MAIN_3236_112_2_17_23_55_13_666 "L1_TD: yPAD_LAYER1 wait dsp ready start %d"
#define LAYER1_MAIN_3238_112_2_17_23_55_13_667 "L1_TD: yPAD_LAYER1 wait dsp ready finish %d"
#define LAYER1_MAIN_3242_112_2_17_23_55_14_668 "L1: enter begin to L1_Trans_GsmRf_Para"
#define LAYER1_MAIN_3261_112_2_17_23_55_14_669 "QQQ: reg isr"
#define LAYER1_MAIN_3276_112_2_17_23_55_14_670 "TEST: before init para %d"
#define LAYER1_MAIN_3277_112_2_17_23_55_14_671 "L1_TD: yPAD_LAYER1 wait dsp ready start %d"
#define LAYER1_MAIN_3279_112_2_17_23_55_14_672 "L1_TD: yPAD_LAYER1 wait dsp ready finish %d"
#define LAYER1_MAIN_3280_112_2_17_23_55_14_673 "TEST: after init para %d"
#define LAYER1_MAIN_3281_112_2_17_23_55_14_674 "layer1: first download dsp para"
#define LAYER1_MAIN_3481_112_2_17_23_55_14_675 "L1:save signal 0x%x\n"
#define LAYER1_MAIN_3493_112_2_17_23_55_14_676 "L1:discard 0x%x, sender 0x%x\n"
#define LAYER1_MAIN_3580_112_2_17_23_55_14_677 "L1: Initializing Physical Layer All Tasks\n\r"
#define LAYER1_MAIN_3636_112_2_17_23_55_14_678 "L1: Initializing for active\n\r"
#define LAYER1_MAIN_3992_112_2_17_23_55_15_679 "L1: calibration create timer failed 0"
#define LAYER1_MAIN_3997_112_2_17_23_55_15_680 "L1: calibration timer is not active, duration is equal 0"
#define LAYER1_MAIN_4293_112_2_17_23_55_16_681 "L1:CheckIfBCCHCollide,invalid arfcn,old=0x%x,new=0x%x"
#define LAYER1_MAIN_4302_112_2_17_23_55_16_682 "L1:CheckIfBCCHCollide: invalid cell_id,old=0x%x, new=0x%x"
#define LAYER1_MAIN_4325_112_2_17_23_55_16_683 "L1:CheckIfBCCHCollide,fn_old_mod51=%d,fn_new_mod51=%d"
#define LAYER1_MAIN_4344_112_2_17_23_55_16_684 "L1 set tbf flag!"
#define LAYER1_MAIN_4715_112_2_17_23_55_16_685 "QQQ: reg isr"
#define LAYER1_MAIN_4732_112_2_17_23_55_16_686 "layer1: download dsp para"
#define LAYER1_MAIN_4733_112_2_17_23_55_16_687 "L1_TD: L1Main_DspInit wait dsp ready start %d"
#define LAYER1_MAIN_4735_112_2_17_23_55_16_688 "L1_TD: L1Main_DspInit wait dsp ready finish %d"
#define LAYER1_MAIN_4810_112_2_17_23_55_17_689 "after dsp download transmit tx max pwr to dsp"
#define LAYER1_MAIN_4875_112_2_17_23_55_17_690 "L1_TD: 30s Resync"
#define LAYER1_MAIN_4885_112_2_17_23_55_17_691 "L1_TD: wrong state %d"
#define LAYER1_MAIN_4903_112_2_17_23_55_17_692 "L1_TD: no valid td arfcn, return to TD_PWR"
#define LAYER1_MAIN_4922_112_2_17_23_55_17_693 "L1_TD: 30s resync goto TD_TS0_RSSI_SUBSTATE"
#define LAYER1_MAIN_4941_112_2_17_23_55_17_694 "L1_TD: 30s Sync Again!!"
#define LAYER1_MAIN_4975_112_2_17_23_55_17_695 "L1:SetTDResyncTimer failed, duration is too short %d"
#define LAYER1_MAIN_4981_112_2_17_23_55_17_696 "L1:SetTDResyncTimer timer already is on %d"
#define LAYER1_MAIN_5012_112_2_17_23_55_17_697 "L1_TD: wenjun.chen:L1Main_SetTDResyncTimer, timespan %d"
#define LAYER1_MAIN_5057_112_2_17_23_55_17_698 "L1_TD: wenjun.chen:L1Main_SetTDSyncFailTimer, timespan %d"
#define LAYER1_MAIN_5069_112_2_17_23_55_17_699 "L1_TD: create sync fail timer"
#define LAYER1_MAIN_5076_112_2_17_23_55_17_700 "L1_TD: active sync fail timer"
#define LAYER1_MAIN_5110_112_2_17_23_55_17_701 "L1_TD: delete sync fail timer"
#define LAYER1_MAIN_5125_112_2_17_23_55_17_702 "L1: em_get_l1_param_timer active"
#define LAYER1_MAIN_5130_112_2_17_23_55_17_703 "L1: em_get_l1_param_timer active"
#define LAYER1_MAIN_5135_112_2_17_23_55_17_704 "L1: em_get_l1_param create timer failed "
#define LAYER1_MIDI_88_112_2_17_23_55_18_705 "L1 L1Midi_OutputData() became a void one"
#define LAYER1_MIDI_97_112_2_17_23_55_18_706 "L1 L1Midi_Stop() became a void one"
#define LAYER1_OUTPORT_193_112_2_17_23_55_18_707 "L1:Start layer1_calibration thread!\n"
#define LAYER1_OUTPORT_225_112_2_17_23_55_19_708 "L1:Start layer1_normal thread!\n"
#define LAYER1_OUTPORT_292_112_2_17_23_55_19_709 "L1:The last operation is not finished!"
#define LAYER1_OUTPORT_298_112_2_17_23_55_19_710 "L1 set task id %x L1API_GetAFC"
#define LAYER1_OUTPORT_346_112_2_17_23_55_19_711 "L1:The last operation is not finished!"
#define LAYER1_OUTPORT_352_112_2_17_23_55_19_712 "L1 set task id %x L1API_GetPA"
#define LAYER1_OUTPORT_387_112_2_17_23_55_19_713 "L1:The last operation is not finished!"
#define LAYER1_OUTPORT_393_112_2_17_23_55_19_714 "L1 set task id %x L1API_GetAGC"
#define LAYER1_OUTPORT_425_112_2_17_23_55_19_715 "L1:The last operation is not finished!"
#define LAYER1_OUTPORT_430_112_2_17_23_55_19_716 "L1 set task id %x L1API_GetBlockErrorRatio"
#define LAYER1_OUTPORT_553_112_2_17_23_55_19_717 "L1:Start DSP RX with max power"
#define LAYER1_OUTPORT_566_112_2_17_23_55_19_718 "L1:Stop DSP RX with max power"
#define LAYER1_OUTPORT_632_112_2_17_23_55_20_719 "L1:Start DSP TX with max power"
#define LAYER1_OUTPORT_645_112_2_17_23_55_20_720 "L1:Stop DSP TX with max power"
#define LAYER1_OUTPORT_719_112_2_17_23_55_20_721 "L1:Start DSP TX with max power"
#define LAYER1_OUTPORT_732_112_2_17_23_55_20_722 "L1:Stop DSP TX with max power"
#define LAYER1_OUTPORT_827_112_2_17_23_55_20_723 "L1:GPSVALIDATION audio is using!"
#define LAYER1_OUTPORT_834_112_2_17_23_55_20_724 "L1:Other function code in overlay! "
#define LAYER1_OUTPORT_902_112_2_17_23_55_20_725 "L1_TD: Get Td Nv Param"
#define LAYER1_OUTPORT_915_112_2_17_23_55_20_726 "L1_TD: Set Td Nv Param: test_bsic_fail %d"
#define LAYER1_OUTPORT_920_112_2_17_23_55_20_727 "L1_TD: Set Td Nv Param: test_bcch_fail %d"
#define LAYER1_OUTPORT_925_112_2_17_23_55_20_728 "L1_TD: Set Td Nv Param: test_ab_mode %d"
#define LAYER1_OUTPORT_930_112_2_17_23_55_20_729 "L1_TD: Set Td Nv Param: test_mode_change %d"
#define LAYER1_OUTPORT_935_112_2_17_23_55_20_730 "L1_TD: Set Td Nv Param: test_tbf_time %d"
#define LAYER1_OUTPORT_940_112_2_17_23_55_20_731 "L1_TD: Set Td Nv Param: g_td_first_time_syncdl_arfcn_num %d"
#define LAYER1_OUTPORT_953_112_2_17_23_55_20_732 "L1_TD: Set Td Nv Param: g_td_max_syncdl_arfcn_num %d"
#define LAYER1_OUTPORT_966_112_2_17_23_55_20_733 "L1_TD: Set Td Nv Param: g_td_power_alpha %d"
#define LAYER1_OUTPORT_975_112_2_17_23_55_20_734 "L1_TD: Set Td Nv Param: s_td_sync_fail_timer_inteval %d"
#define LAYER1_OUTPORT_1004_112_2_17_23_55_20_735 "TEST TEMP: arfcn 0x%x, rscp %d"
#define LAYER1_OUTPORT_1008_112_2_17_23_55_20_736 "L1_TD: Report TD NCELL ENG, num %d"
#define LAYER1_OUTPORT_1020_112_2_17_23_55_20_737 "TEST TEMP: arfcn 0x%x, cell_id %d, rscp %d"
#define LAYER1_OUTPORT_1024_112_2_17_23_55_20_738 "L1_TD: Report TD NCELL ENG, num %d"
#define LAYER1_OUTPORT_1036_112_2_17_23_55_20_739 "L1_TD: Report TD NCELL ENG, num %d"
#define LAYER1_OUTPORT_1122_112_2_17_23_55_21_740 "L1: MPHHandle_MPhSMSCBDiscardBlock wakeup dsp"
#define LAYER1_OUTPORT_1170_112_2_17_23_55_21_741 "L1: MPHHandle_MPhSMSCBDiscardBlock wakeup dsp"
#define LAYER1_SBC_105_112_2_17_23_55_21_742 "L1:load GPS code in overlay.!"
#define LAYER1_SBC_109_112_2_17_23_55_21_743 "L1:Overlay_code_base_ptr is %x"
#define LAYER1_SBC_110_112_2_17_23_55_21_744 "L1:Overlay_Load_Code %x %x %x %x %x"
#define LAYER1_SBC_184_112_2_17_23_55_21_745 "L1: overlay_context.overlay_mutex %x"
#define LAYER1_SBC_185_112_2_17_23_55_21_746 "L1: overlay_context.mem_state %d"
#define LAYER1_SBC_186_112_2_17_23_55_21_747 "L1: overlay_context.overlay_code_base_ptr %x"
#define LAYER1_SBC_218_112_2_17_23_55_21_748 "L1: L1_SBC_Config_Encoder "
#define LAYER1_SBC_233_112_2_17_23_55_21_749 "L1:SBC Function mutex: %x "
#define LAYER1_SBC_327_112_2_17_23_55_21_750 "L1: command  CmdQ_SBC_Config_Encoder "
#define LAYER1_SBC_352_112_2_17_23_55_22_751 "L1:L1_SBC_Encode_Frame %d %d %d"
#define LAYER1_SBC_354_112_2_17_23_55_22_752 "L1:L1_SBC_Encode_Frame"
#define LAYER1_SBC_426_112_2_17_23_55_22_753 "L1: command  CMDQ_SBC_ENCODE_FRAME "
#define LAYER1_SBC_490_112_2_17_23_55_22_754 "L1:frame_count=%d,frame_len_return_from_dsp[in 16bits]=%d,frame_len=%d"
#define LAYER1_SBC_535_112_2_17_23_55_22_755 "L1: command  CMDQ_TRANPCM_CONFIG "
#define LAYER1_SBC_551_112_2_17_23_55_22_756 "L1: command  CMDQ_TRANPCM_PROCESS "
#define LAYER1_VOICECALL_68_112_2_17_23_55_22_757 "L1:Transfer a frame data to DSP !!!! "
#define MPHHANDLE_333_112_2_17_23_55_23_758 "l1 SharedMemProc_WakeupDsp!!!"
#define MPHHANDLE_338_112_2_17_23_55_23_759 "l1 L1Main_ActiveDspByCmds!!!"
#define MPHHANDLE_359_112_2_17_23_55_23_760 "l1 active meet response delay !!!"
#define MPHHANDLE_418_112_2_17_23_55_24_761 "l1:deactive rat %d, gsm sync %d, change mode %d"
#define MPHHANDLE_432_112_2_17_23_55_24_762 "L1 deactive mask %x"
#define MPHHANDLE_466_112_2_17_23_55_24_763 "L1 main card id %x"
#define MPHHANDLE_538_112_2_17_23_55_24_764 "l1:deactive rat %d, gsm sync %d, change mode %d"
#define MPHHANDLE_606_112_2_17_23_55_24_765 "l1_td: in td mode receive classmark req!"
#define MPHHANDLE_839_112_2_17_23_55_24_766 "8800h bcch fail"
#define MPHHANDLE_1162_112_2_17_23_55_25_767 "L1Q: MPHHandle_MphBcchCampReqIdle"
#define MPHHANDLE_1480_112_2_17_23_55_26_768 "L1 pwrswp state %d"
#define MPHHANDLE_1523_112_2_17_23_55_26_769 "L1:calipost_param = 0x%x"
#define MPHHANDLE_1557_112_2_17_23_55_26_770 "L1:band = %d,arfcn = 0x%x, cali mode = %x, cali param %x"
#define MPHHANDLE_1636_112_2_17_23_55_26_771 "L1: bcch stop is received, arfcn %d, bsic %d"
#define MPHHANDLE_1897_112_2_17_23_55_26_772 "L1Q: MPHHandle_MphBcchCampReqIdle"
#define MPHHANDLE_2317_112_2_17_23_55_27_773 "L1 page mode req param3 %d"
#define MPHHANDLE_2669_112_2_17_23_55_28_774 "L1,sent rf scan ind in pg_reorg.\n"
#define MPHHANDLE_3708_112_2_17_23_55_30_775 "L1: start_fn %d"
#define MPHHANDLE_3918_112_2_17_23_55_30_776 "L1: chan assign start fn %d"
#define MPHHANDLE_4113_112_2_17_23_55_31_777 "L1:chan_mode %d, is_change %d"
#define MPHHANDLE_4205_112_2_17_23_55_31_778 "L1:Cipher mode %d,Cipher_algarithm %d,n"
#define MPHHANDLE_4210_112_2_17_23_55_31_779 "L1:Cipher Key %d,%d,%d,%d,%d,%d,%d,%dn"
#define MPHHANDLE_4268_112_2_17_23_55_31_780 "L1: Freq Change start fn %d"
#define MPHHANDLE_4338_112_2_17_23_55_31_781 "L1: chan mode %d!\n"
#define MPHHANDLE_4463_112_2_17_23_55_31_782 "L1 arfcn %x not sync"
#define MPHHANDLE_4472_112_2_17_23_55_31_783 "L1 init handover cell fail! "
#define MPHHANDLE_4479_112_2_17_23_55_31_784 "L1 handover cell id %d,  arfcn index %x "
#define MPHHANDLE_4517_112_2_17_23_55_32_785 "L1 handover to resync pending cell arfcn index %x"
#define MPHHANDLE_4537_112_2_17_23_55_32_786 "L1:Handover fail, cell_id = %d,bsic %d,target bsic %d"
#define MPHHANDLE_4594_112_2_17_23_55_32_787 "L1:cipher_mode %d,cipher_algorithm %d"
#define MPHHANDLE_4920_112_2_17_23_55_32_788 "L1:set cell %d to NOT_SYN_CALL"
#define MPHHANDLE_5047_112_2_17_23_55_33_789 "L1_TD: sync ho, recieve new bad sa counter, update it"
#define MPHHANDLE_5137_112_2_17_23_55_33_790 "L1:No TCH avtive,ignor LOOP cmd"
#define MPHHANDLE_5176_112_2_17_23_55_33_791 "L1:rec disable loop back %d!"
#define MPHHANDLE_5195_112_2_17_23_55_33_792 "L1:rec loop back cmd!"
#define MPHHANDLE_5399_112_2_17_23_55_33_793 "L1: MPHHandle_MphSMSCBStartReq  cbch_required_bitmap %d"
#define MPHHANDLE_5529_112_2_17_23_55_34_794 "L1: MPHHandle_MPhSMSCBDiscardBlock %d"
#define MPHHANDLE_5546_112_2_17_23_55_34_795 "L1: MPHHandle_MPhSMSCBDiscardBlock wakeup dsp"
#define MPHHANDLE_5597_112_2_17_23_55_34_796 "L1 uiCellIndex is INVALID_CELL! %x"
#define MPHHANDLE_5602_112_2_17_23_55_34_797 "L1: tc mask %d"
#define MPHHANDLE_5625_112_2_17_23_55_34_798 "L1 cell state is not sync %x, %x!"
#define MPHHANDLE_5713_112_2_17_23_55_34_799 "L1 scell "
#define MPHHANDLE_5725_112_2_17_23_55_34_800 "L1 valid cell! 0x%x"
#define MPHHANDLE_5740_112_2_17_23_55_34_801 "L1 searching %x"
#define MPHHANDLE_5802_112_2_17_23_55_34_802 "Download GSM Code"
#define MPHHANDLE_5819_112_2_17_23_55_34_803 "Download GSM Code2"
#define MPHHANDLE_5962_112_2_17_23_55_34_804 "L1Q: g_is_sync_mode_change %d"
#define MPHHANDLE_5965_112_2_17_23_55_34_805 "L1Q: MODE CHANGE: HO_FAIL_REVERT_TD or RESEL_FAIL_REVERT_TD"
#define MPHHANDLE_5970_112_2_17_23_55_34_806 "L1Q: g_is_revert_to_td %d"
#define MPHHANDLE_5999_112_2_17_23_55_35_807 "L1Q: mode change gsm2td SharedMemProc_SetTDSyncPara"
#define MPHHANDLE_6004_112_2_17_23_55_35_808 "L1Q: MODE CHANGE: HO_FAIL_REVERT_TD or RESEL_FAIL_REVERT_TD"
#define MPHHANDLE_6011_112_2_17_23_55_35_809 "L1_TD: manuel to td before sync 3 to 2 finish"
#define MPHHANDLE_6108_112_2_17_23_55_35_810 "L1:MergeTDMeasInfo, numold %d, numnew %d, issynced %d"
#define MPHHANDLE_6128_112_2_17_23_55_35_811 "L1:MergeTDMeasInfo, i=%d freqold[0x%x], freqnew[0x%x]"
#define MPHHANDLE_6365_112_2_17_23_55_35_812 "L1:StartTDMeasReq, td meas has been actived, a duplicated request by RRC"
#define MPHHANDLE_6608_112_2_17_23_55_36_813 "L1_TD: RR stop td meas, issynced %d"
#define MPHHANDLE_6697_112_2_17_23_55_36_814 "L1:StartTDMeasReqTBF, td meas has been actived, a duplicated request by RRC"
#define MPHHANDLE_6847_112_2_17_23_55_36_815 "L1Q: cell reselect, g_is_sync_mode_change true"
#define MPHHANDLE_6850_112_2_17_23_55_36_816 "L1Q: reselect to gsm after plot nop %d"
#define MPHHANDLE_6853_112_2_17_23_55_36_817 "L1Q: cell reselect, g_is_sync_mode_change false"
#define MPHHANDLE_6984_112_2_17_23_55_36_818 "L1_TD: sync mode, set default badsa counter"
#define MPHHANDLE_6986_112_2_17_23_55_36_819 "L1_TD: sync mode change before SharedMemProc_GetGSMSyncPara %d"
#define MPHHANDLE_6988_112_2_17_23_55_37_820 "L1_TD: sync mode change after SharedMemProc_GetGSMSyncPara %d"
#define MPHHANDLE_6991_112_2_17_23_55_37_821 "L1Q: handover, g_is_sync_mode_change true"
#define MPHHANDLE_6993_112_2_17_23_55_37_822 "L1_TD: sync mode change before handover para set %d"
#define MPHHANDLE_6997_112_2_17_23_55_37_823 "L1 mode change : plot one idle frame [%d] before handover "
#define MPHHANDLE_7003_112_2_17_23_55_37_824 "L1_TD: sync mode change after handover para set %d"
#define MPHHANDLE_7006_112_2_17_23_55_37_825 "L1Q: handover, g_is_sync_mode_change false"
#define MPHHANDLE_7037_112_2_17_23_55_37_826 "L1Q: handover suc finish"
#define PHHANDLE_408_112_2_17_23_55_38_827 "L1 enable codec device"
#define PHHANDLE_685_112_2_17_23_55_38_828 "L1:PHHandle_AppPhMp3A2dpReq:Time = %d ms"
#define PHHANDLE_854_112_2_17_23_55_38_829 "L1:Switch Band: ma_arfcn_index = 0x%x, pdch_band = %d!"
#define PHHANDLE_986_112_2_17_23_55_39_830 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_1117_112_2_17_23_55_39_831 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_1170_112_2_17_23_55_39_832 "L1:TBF: switch band,pmax=%d,pdch_band=%d,bcch_band=%d"
#define PHHANDLE_1260_112_2_17_23_55_39_833 "L1:Switch Band1: ma_arfcn_index = 0x%x, pdch_band=%d!"
#define PHHANDLE_1377_112_2_17_23_55_40_834 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_1432_112_2_17_23_55_40_835 "L1:tbf switch band, pmax=%d, pdch_band=%d, bcch_band=%d!"
#define PHHANDLE_1590_112_2_17_23_55_40_836 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_1690_112_2_17_23_55_40_837 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_1865_112_2_17_23_55_40_838 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_1971_112_2_17_23_55_41_839 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_2105_112_2_17_23_55_41_840 "L1:Switch Band2,ma_arfcn_index = 0x%x, pdch_band = %d!"
#define PHHANDLE_2190_112_2_17_23_55_41_841 "L1:TBFCon: 1, exe_band = %d, new_band = %d, pwr_idx = %d!"
#define PHHANDLE_2219_112_2_17_23_55_41_842 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_2251_112_2_17_23_55_41_843 "L1:TBFCon: 2, exe_band = %d, new_band = %d, pwr_idx = %d!"
#define PHHANDLE_2428_112_2_17_23_55_42_844 "L1:TBFCon: 3, exe_band = %d, new_band = %d, pwr_idx = %d!"
#define PHHANDLE_2458_112_2_17_23_55_42_845 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_2528_112_2_17_23_55_42_846 "L1:TBFCon: 4, exe_band = %d, new_band = %d, pwr_idx = %d!"
#define PHHANDLE_2688_112_2_17_23_55_42_847 "L1:TBFCon: 5, exe_band = %d, new_band = %d, pwr_idx = %d!"
#define PHHANDLE_2716_112_2_17_23_55_42_848 "L1:W start fn %d ,%d\n,curr_fn %d"
#define PHHANDLE_2762_112_2_17_23_55_42_849 "L1:TBFCon: 6, exe_band = %d, new_band = %d, pwr_idx = %d!"
#define PHHANDLE_2891_112_2_17_23_55_43_850 "split cyccle %d"
#define PHHANDLE_2986_112_2_17_23_55_43_851 "L1: use old ta %d"
#define PHHANDLE_3031_112_2_17_23_55_43_852 "L1: use old tai %d"
#define PHHANDLE_3257_112_2_17_23_55_43_853 "L1 need to receive minor paging, is_receive_vicecard_paging:%d"
#define PHHANDLE_3275_112_2_17_23_55_43_854 "Switch Band5: ma_arfcn_index = 0x%x, pdch_band = %d!"
#define PHHANDLE_3358_112_2_17_23_55_44_855 "L1:set cell %d to NOT_SYN_CALL"
#define PHHANDLE_3493_112_2_17_23_55_44_856 "L1:set cell %d to NOT_SYN_CALL"
#define PHHANDLE_3573_112_2_17_23_55_44_857 "L1:set cell %d to NOT_SYN_CALL"
#define PHHANDLE_3642_112_2_17_23_55_44_858 "L1:TBF Release: switch band, pmax=%d, pdch_band=%d, bcch_band=%d!"
#define PHHANDLE_3970_112_2_17_23_55_45_859 "audio3: PHHandle_AppSetDevModeReq ph_set_dev_mode_ptr->Param1 %d, Param2 %d, Param3 %d, Param4 %d"
#define PHHANDLE_4014_112_2_17_23_55_45_860 "L1 PHHandle_StartMagicVoiceFunc type %d"
#define RESQ_253_112_2_17_23_55_46_861 "L1:Rx a PBCCH in BCCH_CAMPED state"
#define RESQ_263_112_2_17_23_55_46_862 "L1:BCCH_CAMPED,C_t %d!\n"
#define RESQ_345_112_2_17_23_55_46_863 "L1:PBCCH_CAMPED state,Discard a block,Ch_tag %d!\n"
#define RESQ_386_112_2_17_23_55_46_864 "L1:Rx a PPCH in PLMN_CAMPED state"
#define RESQ_399_112_2_17_23_55_46_865 "L1: Rx a SCH in PLMN_CAMPED state"
#define RESQ_421_112_2_17_23_55_46_866 "L1:Rx a PBCCH in BCCH_CAMPED state"
#define RESQ_432_112_2_17_23_55_46_867 "L1:PLMN_CAMPED state,Discard a block,Ch_tag %d!\n"
#define RESQ_463_112_2_17_23_55_46_868 "L1:Rx a CBCH in PG_REORG_CAMPED state"
#define RESQ_521_112_2_17_23_55_46_869 "L1: Rx a SCH in PG_REORG state"
#define RESQ_531_112_2_17_23_55_46_870 "L1:PG_REORG CAMPED,Discard a block,Ch_tag %d!\n"
#define RESQ_584_112_2_17_23_55_46_871 "L1:Rx a PBCCH in RACHAGCH state"
#define RESQ_591_112_2_17_23_55_46_872 "L1:Rx a PPCH in RACHAGCH state"
#define RESQ_601_112_2_17_23_55_46_873 "L1:Rx a PAGCH in RACHAGCH state"
#define RESQ_611_112_2_17_23_55_46_874 "L1: Rx a SCH in RACHAGCH state"
#define RESQ_665_112_2_17_23_55_47_875 "L1:Rx a PPCH in RACHAGCH state"
#define RESQ_682_112_2_17_23_55_47_876 "L1: Rx a SCH in PRACHAGCH state"
#define RESQ_688_112_2_17_23_55_47_877 "L1:Discard a block,Ch_tag %d!\n"
#define RESQ_763_112_2_17_23_55_47_878 "L1:SDCCHTCH state, Discard a block,Ch_tag %d!\n"
#define RESQ_804_112_2_17_23_55_47_879 "L1:Rx a PTCCH in TBF state"
#define RESQ_900_112_2_17_23_55_47_880 "L1:Rx a BCCH in traffic"
#define RESQ_906_112_2_17_23_55_47_881 "L1:Rx a PCH in traffic"
#define RESQ_1257_112_2_17_23_55_48_882 "L1: FrameInstr Queue DSP Over Run"
#define RESQ_1262_112_2_17_23_55_48_883 "L1: Cmd Queue DSP Over Run"
#define RESQ_1267_112_2_17_23_55_48_884 "L1: TxData Queue DSP Over Run"
#define RESQ_1272_112_2_17_23_55_48_885 "L1: Resp Queue DSP Over RunU"
#define RESQ_1277_112_2_17_23_55_48_886 "L1: RxData Queue DSP Over Run"
#define RESQ_1283_112_2_17_23_55_48_887 "L1: PowerMon Queue DSP Over Run"
#define RESQ_1288_112_2_17_23_55_48_888 "L1: MeasReport Queue DSP Over Run"
#define RESQ_1293_112_2_17_23_55_48_889 "L1: BlockSend Queue DSP Over Run"
#define RESQ_1356_112_2_17_23_55_48_890 "L1 bsic fail %x,%x"
#define RESQ_1422_112_2_17_23_55_48_891 "L1 is_sch_conflict %x,%x"
#define RESQ_1487_112_2_17_23_55_48_892 "L1 bsic fail:  %x, fail count %d"
#define RESQ_1496_112_2_17_23_55_48_893 "L1 is_sch_conflict in sdcchtch %d"
#define RESQ_1577_112_2_17_23_55_48_894 "L1 handover cell search fail !%x"
#define RESQ_1657_112_2_17_23_55_49_895 "L1:WakeUp! %d,%d"
#define RESQ_1684_112_2_17_23_55_49_896 "L1:WakeUp! %d,%d"
#define RESQ_1709_112_2_17_23_55_49_897 "L1:WakeUp! %d,%d"
#define RESQ_1757_112_2_17_23_55_49_898 "L1 wakeup resp2"
#define RESQ_1791_112_2_17_23_55_49_899 "L1:WakeUp in idle! %d,%d"
#define RESQ_1815_112_2_17_23_55_49_900 "L1:WakeUp in power sweep! %d,%d"
#define RESQ_1975_112_2_17_23_55_49_901 "L1:Q start fn %d ,fn_curr %d,elapse %d\n"
#define RESQ_2075_112_2_17_23_55_49_902 "L1:Q start fn %d ,fn_curr %d,elapse %d\n"
#define RESQ_2228_112_2_17_23_55_50_903 "L1:WakeUp pbcch! %d,%d"
#define RESQ_2335_112_2_17_23_55_50_904 "L1:Q start fn %d ,fn_curr %d,elapse %d\n"
#define RESQ_2502_112_2_17_23_55_50_905 "L1:Q start fn %d ,fn_curr %d,elapse %d\n"
#define RESQ_2594_112_2_17_23_55_50_906 "L1:WakeUp! %d,%d"
#define RESQ_2676_112_2_17_23_55_51_907 "L1:Q start fn %d ,fn_curr %d,elapse %d\n"
#define RESQ_2895_112_2_17_23_55_51_908 "L1 scell adjust %x, %x"
#define RESQ_2902_112_2_17_23_55_51_909 "L1 ncell adjust %x, %x; ncell %x, scell %x"
#define RESQ_3027_112_2_17_23_55_51_910 "L1: echo_testing_CNF!"
#define RESQ_3294_112_2_17_23_55_52_911 "L1: overrun %d p0 %d, p1 %d, num %d, que_num %d"
#define RESQ_3551_112_2_17_23_55_52_912 "L1:State:SBC_CONFIG,Time = %d ms"
#define RESQ_3562_112_2_17_23_55_52_913 "L1:State:SBC_ENCODE,Time = %d ms"
#define RESQ_3574_112_2_17_23_55_52_914 "L1:State:PCM_TRANS,Time = %d ms"
#define RESQ_3621_112_2_17_23_55_52_915 "L1:State:NONE_IN_USE,left_ptr=0x%x,right_ptr=0x%x,length = %d,Time = %d ms"
#define RESQ_3692_112_2_17_23_55_53_916 "L1:Get SBC Frame.State:NONE_IN_USE,frame_ptr=0x%x,frame_length=0x%x,frame_num=%d, Time = %d ms"
#define RESQ_3720_112_2_17_23_55_53_917 "L1: overrun %d p0 %d, p1 %d, num %d, que_num %d"
#define RESQ_3758_112_2_17_23_55_53_918 "L1: overrun %d p0 %d, p1 %d, num %d, que_num %d"
#define RESQ_3810_112_2_17_23_55_53_919 "L1: overrun %d p0 %d, p1 %d, num %d, que_num %d"
#define RESQ_3869_112_2_17_23_55_53_920 "L1: TD PWRSWP, freq 0x%x, status %d, rssi %d"
#define RESQ_3885_112_2_17_23_55_53_921 "L1: pwrswp dsp report arfcn not in list"
#define RESQ_3964_112_2_17_23_55_53_922 "L1: TD MAX_R  rssi %d R_val 0x%x"
#define RESQ_3972_112_2_17_23_55_53_923 "L1: pwrswp uarfcn 0x%x to agc"
#define RESQ_4054_112_2_17_23_55_53_924 "L1:TDSYNC response wrongstate & coper, deactive coper"
#define RESQ_4056_112_2_17_23_55_53_925 "L1_TD: Recieve Sync Idle in not TD_SYNC_SUBSTATE"
#define RESQ_4084_112_2_17_23_55_53_926 "L1: sync dsp report arfcn not in list"
#define RESQ_4095_112_2_17_23_55_53_927 "L1_TD: Recieve Sync Idle, freq 0x%x, status %d, sync_mode %d"
#define RESQ_4134_112_2_17_23_55_53_928 "L1_TD: Recieve Sync Idle, freq 0x%x, status %d, sync_mode %d, delta_pos %d, max_dwpts_ratio %d, rxslot %d"
#define RESQ_4143_112_2_17_23_55_53_929 "L1_TD: Recieve Sync Gprs, freq 0x%x, status %d, sync_mode %d, process_counter %d"
#define RESQ_4150_112_2_17_23_55_53_930 "L1_TD: Recieve Sync Gprs, freq 0x%x, delta_pos %d"
#define RESQ_4166_112_2_17_23_55_53_931 "L1_TD: Recieve Sync Gprs, freq 0x%x, status %d, sync_mode %d, process_counter %d, g_delta_pos[%d] %d, fristrxslot=%d"
#define RESQ_4193_112_2_17_23_55_54_932 "L1_TD: Recieve Sync Gprs, delta_pos %d, rxslot %d"
#define RESQ_4208_112_2_17_23_55_54_933 "L1: uarfcn 0x%x to agc"
#define RESQ_4260_112_2_17_23_55_54_934 "L1_TD: Recieve Agc, freq 0x%x, status %d, gain %d"
#define RESQ_4280_112_2_17_23_55_54_935 "L1:AGC response freq 0x%x not be found"
#define RESQ_4291_112_2_17_23_55_54_936 "L1: AGC try disable coper %d"
#define RESQ_4402_112_2_17_23_55_54_937 "L1_TD: discard meas result because of 30s resync"
#define RESQ_4420_112_2_17_23_55_54_938 "L1_CONFLICT: p0 0x%x, p1 %d, p2 %d, p3 %d"
#define RESQ_4454_112_2_17_23_55_54_939 "L1 ERROR td freqindex not find  frq 0x%x"
#define RESQ_4554_112_2_17_23_55_54_940 "gsm_l1c:drop agc result!"
#define RESQ_4576_112_2_17_23_55_54_941 "L1_TD QQQ: ResQ_DspOtdChangeForTDReq old delta_pos %d, otd %d"
#define RESQ_4590_112_2_17_23_55_54_942 "L1_TD QQQ: ResQ_DspOtdChangeForTDReq new delta_pos %d"
#define RESQ_4604_112_2_17_23_55_54_943 "LAYER1: enter Handle DSP_is_READY_REQ!\r\n"
#define RESQ_4618_112_2_17_23_55_54_944 "audio3: ResQ_DspIsReadyReqModeChange g_audioPara_buffer %d, g_audio_length %d, g_td_audioPara_buffer %d, g_td_audio_length %d"
#define RESQ_4673_112_2_17_23_55_54_945 "8800h bsic fail"
#define RESQ_4688_112_2_17_23_55_55_946 "L1: Rx a SCH!\n"
#define RESQ_4730_112_2_17_23_55_55_947 "L1: Wrong RX tag %d,\r\n!\n"
#define RESQ_4761_112_2_17_23_55_55_948 "L1Q: ResQ_DspWakeUpModeChange SharedMemProc_SetTDSyncPara"
#define RESQ_4766_112_2_17_23_55_55_949 "L1Q: MODE CHANGE: HO_FAIL_REVERT_TD or RESEL_FAIL_REVERT_TD"
#define RESQ_4773_112_2_17_23_55_55_950 "L1_TD: manuel to td before sync 3 to 2 finish"
#define RESQ_4787_112_2_17_23_55_55_951 "L1:WakeUp in Mode Change!"
#define RESQ_4886_112_2_17_23_55_55_952 "L1IT_DMCase-0:ht=%d,curTyp=%d,setType=%d,p1=%d,p2=%d,p3=%d,p4=%d,cellid=0x%x,num=%d,cfreq=%d,sfreq=%d,span=%d,succ=%d,trytm=%d,r1=%d,r2=%d,gc=%d"
#define RESQ_4899_112_2_17_23_55_55_953 "L1IT_DMCase-S: STOP Test!"
#define RESQ_4952_112_2_17_23_55_55_954 "L1IT_DMCase-4:sdl0=%d,sdl1=%d,mid0=%d,mid1=%d,cellNum=%d,sdlNum=%d,p1=%d,p2=%d"
#define RESQ_5023_112_2_17_23_55_55_955 "L1IT_DMCase-5:sdl0=%d,sdl1=%d,mid0=%d,mid1=%d,cellNum=%d,sdlNum=%d,p1=%d,p2=%d"
#define RESQ_5112_112_2_17_23_55_55_956 "L1ITDMCase-0:Param Error!(hand_type=%d)"
#define RXLEVMON_748_112_2_17_23_55_57_957 "L1: ba count %d %d, %d,total %d,%d"
#define RXLEVMON_826_112_2_17_23_55_57_958 "L1: main_card_id:%d,card_id:%d,arfcn num: %d"
#define RXLEVMON_958_112_2_17_23_55_58_959 "L1: ba count de %d %d, %d,total %d,%d"
#define RXLEVMON_1228_112_2_17_23_55_58_960 "L1: not permit %x"
#define RXLEVMON_1459_112_2_17_23_55_59_961 "L1:pwr_swp saturate with low gain, arfcn_index %d"
#define RXLEVMON_1506_112_2_17_23_55_59_962 "L1: meas_ptr->is_valid is wrong!"
#define RXLEVMON_1569_112_2_17_23_55_59_963 "L1:sat_low arfcn %x, , num %d"
#define RXLEVMON_1651_112_2_17_23_55_59_964 "L1:Monitor saturate with low gain, arfcn_index %d"
#define RXLEVMON_1664_112_2_17_23_55_59_965 "L1:sat_hi arfcn %x, num %d"
#define RXLEVMON_1681_112_2_17_23_55_59_966 "L1: meas_ptr->is_valid is wrong!%d \n"
#define RXLEVMON_1716_112_2_17_23_55_59_967 "L1 init sort ba in tbf and receive paging,vice card six strong not celar"
#define RXLEVMON_1880_112_2_17_23_55_59_968 "L1: ba not valid"
#define RXLEVMON_1908_112_2_17_23_56_0_969 "L1 warning: can't get valid ba"
#define RXLEVMON_2152_112_2_17_23_56_0_970 "L1: IT's A WRONG g_measurement_ba_ptr->ba_table_state.band_ind!"
#define RXLEVMON_2183_112_2_17_23_56_0_971 "L1:full band swp: arfcn_index(%x) rxlev %x times "
#define RXLEVMON_2236_112_2_17_23_56_0_972 "L1:full band swp: arfcn_index(%x) rxlev %x times "
#define RXLEVMON_2247_112_2_17_23_56_0_973 "L1:now %d,n1 %d,n2 %d"
#define RXLEVMON_2371_112_2_17_23_56_0_974 "L1 vice card strongest arfcn_index 0x%x\n"
#define RXLEVMON_2404_112_2_17_23_56_1_975 "L1 vicecard sixstrong arfcn 0x%x in maincard sixstrong num:%d"
#define RXLEVMON_2507_112_2_17_23_56_1_976 "L1:rla %d,sum %d,count %d"
#define RXLEVMON_2755_112_2_17_23_56_1_977 "L1 six strong main_card_id:%d"
#define RXLEVMON_2778_112_2_17_23_56_1_978 "L1:traffic card_id: %d, six_strong_num:%d,arfcn: %x,%x,%x,%x,%x,%x;rla: %x,%x,%x,%x,%x,%x"
#define RXLEVMON_2823_112_2_17_23_56_1_979 "L1:idle  %d, %d, %d;%x,%x,%x,%x,%x,%x;%x,%x,%x,%x,%x,%x;%x,%x,%x,%x,%x,%x"
#define RXLEVMON_2903_112_2_17_23_56_2_980 "Layer1 %x turn strong"
#define RXLEVMON_3135_112_2_17_23_56_2_981 "L1:ncell means report, is_multi_card:%d,sync_strong_num_tbf[0]:%d,sync_strong_num_tbf[1]:%d"
#define RXLEVMON_3260_112_2_17_23_56_2_982 "L1 ba num %x, monitor num %x,no_monitor_period %x;%x,%x"
#define RXLEVMON_3408_112_2_17_23_56_3_983 "L1:powersweep duration %d"
#define RXLEVMON_3554_112_2_17_23_56_3_984 "L1: %x search fail too much, needn't sync"
#define RXLEVMON_3568_112_2_17_23_56_3_985 "L1: This arfcn did not in current BA , arfcn_index %d \n"
#define RXLEVMON_3617_112_2_17_23_56_3_986 "L1 the arfcn index is not in ba %x"
#define RXLEVMON_3622_112_2_17_23_56_3_987 "L1 invalid arfcn index %x"
#define RXLEVMON_3754_112_2_17_23_56_3_988 "L1 ba table is invalid!"
#define RXLEVMON_4027_112_2_17_23_56_4_989 "L1 card_relation erro! arfcn_index %x, type %d"
#define RXLEVMON_4033_112_2_17_23_56_4_990 "L1 the cell is not in ba! arfcn_index %x"
#define RXLEVMON_4170_112_2_17_23_56_4_991 "L1 the arfcn is in old ba arfcn_index %x, type %d"
#define RXLEVMON_4176_112_2_17_23_56_4_992 "L1 the arfcn is in old ba! arfcn_index %x"
#define RXLEVMON_4215_112_2_17_23_56_4_993 "L1 the arfcn is in old ba arfcn_index %x, type %d"
#define RXLEVMON_4221_112_2_17_23_56_4_994 "L1 the arfcn is in old ba! arfcn_index %x"
#define RXLEVMON_4549_112_2_17_23_56_5_995 "L1:forgetting factor=%d"
#define RXLEVMON_4682_112_2_17_23_56_5_996 "L1 input: mean_curr %x,cv_curr %x, e %x,r %x,rx_slot %x, cs %d"
#define RXLEVMON_4727_112_2_17_23_56_5_997 "L1:input %x,%x ; output  %x, %x ; %x, %x;e %x,r %x,slot %x,rx_tfi %x,%x,slotmask%x,loopback %d"
#define RXLEVMON_4888_112_2_17_23_56_5_998 "L1: s_save_prev_ba_table is full!"
#define RXLEVMON_4991_112_2_17_23_56_6_999 "L1: RRA_MPH_RF_SCAN_IND!"
#define RXLEVMON_5068_112_2_17_23_56_6_1000 "L1 vice card scell_ptr is NULL."
#define RXLEVMON_5108_112_2_17_23_56_6_1001 "L1,in tbf release report vicecard thirdstrong: %d,arfcn:0x%x(%d), 0x%x(%d), 0x%x(%d)"
#define RXLEVMON_5148_112_2_17_23_56_6_1002 "L1 quick pwrswp normal finished!"
#define RXLEVMON_5154_112_2_17_23_56_6_1003 "L1 quick pwrswp abort no valid ba!"
#define RXLEVMON_5159_112_2_17_23_56_6_1004 "L1 pwr ++ index %x, %x"
#define RXLEVMON_5286_112_2_17_23_56_6_1005 "L1 add a scell card id %d!%x"
#define RXLEVMON_5315_112_2_17_23_56_6_1006 "L1 invalid scell ba_index!%x arfcn_index %x"
#define RXLEVMON_5440_112_2_17_23_56_7_1007 "L1: cell is omitted %x"
#define RXLEVMON_5580_112_2_17_23_56_7_1008 "L1 Emr is not started!"
#define RXLEVMON_5601_112_2_17_23_56_7_1009 "L1 Emr is not valid %x, %x"
#define RXLEVMON_5682_112_2_17_23_56_7_1010 "L1:ClearVeracity arfcn 0x%x veracity %d, timer %d"
#define RXLEVMON_5713_112_2_17_23_56_7_1011 "L1:MidamVeracity arfcn 0x%x veracity %d, timer %d"
#define RXLEVMON_5784_112_2_17_23_56_7_1012 "L1:UpdateVeracityTimer arfcn 0x%x veracity %d, timer %d"
#define RXLEVMON_5854_112_2_17_23_56_7_1013 "L1: IdleTDSync sub_state %d, fn 0x%x"
#define RXLEVMON_5957_112_2_17_23_56_8_1014 "L1_TD: RxLevMon_IdleTDMeasProcess td_meas_sub_state %d, fn 0x%x"
#define RXLEVMON_5993_112_2_17_23_56_8_1015 "L1_TD: ERR in Idle RSCP, td_meas_ind %d meas_status %d"
#define RXLEVMON_6037_112_2_17_23_56_8_1016 "L1:CheckIfMeasTD3rdFreq wrong state %d"
#define RXLEVMON_6072_112_2_17_23_56_8_1017 "L1:CheckIfMeasTD3rdFreq freq num %d, rssi3 %d, threadhold %d"
#define RXLEVMON_6095_112_2_17_23_56_8_1018 "L1:CheckIfMeasTD3rdFreq max1 %d, max2 %d"
#define RXLEVMON_6107_112_2_17_23_56_8_1019 "L1:CheckIfMeasTD3rdFreq rssi3 be strongest %d"
#define RXLEVMON_6121_112_2_17_23_56_8_1020 "L1:ResetMeasCtrlInd"
#define RXLEVMON_6225_112_2_17_23_56_8_1021 "L1:GenTDMeasFreqIndex, no meas freq in status %d"
#define RXLEVMON_6233_112_2_17_23_56_8_1022 "L1:GenTDMeasFreqIndex, status %d, start 0x%x, end 0x%x"
#define RXLEVMON_6279_112_2_17_23_56_8_1023 "L1:GetNextTDMeasCtrlIndex, status %d, cur %d,start %d,end %d"
#define RXLEVMON_6347_112_2_17_23_56_8_1024 "L1:GetNextTDMeasCtrlIndex,FALSE1 meas_cur_ind %d"
#define RXLEVMON_6361_112_2_17_23_56_8_1025 "L1:GetNextTDMeasCtrlIndex, FALSE2 meas_cur_ind %d"
#define RXLEVMON_6372_112_2_17_23_56_8_1026 "L1:GetNextTDMeasCtrlIndex, TRUE meas_cur_ind %d"
#define RXLEVMON_6511_112_2_17_23_56_9_1027 "L1:CancelTDMeasCtrlIndex, meas_cmd %d, cur %d,start %d,end %d"
#define RXLEVMON_6521_112_2_17_23_56_9_1028 "L1:error CancelTDMeasCtrlIndex no cancel, smain %d, sfreq %d"
#define RXLEVMON_6538_112_2_17_23_56_9_1029 "L1:CancelCtrlIndex, wrong ind %d meas_cmd %d, freq 0x%x"
#define RXLEVMON_6547_112_2_17_23_56_9_1030 "L1:error CancelTDMeasCtrlIndex, meas_cmd %d, freq 0x%x"
#define RXLEVMON_6591_112_2_17_23_56_9_1031 "L1:CancelTDMeasByndex, freq_index %d, cur %d,start %d,end %d"
#define RXLEVMON_6611_112_2_17_23_56_9_1032 "RxLevMon_GetTDNomalMeasFreqNum %d"
#define RXLEVMON_6628_112_2_17_23_56_9_1033 "RxLevMon_GetTDNomalMeasFreqNum 2 %d"
#define RXLEVMON_6656_112_2_17_23_56_9_1034 "L1:TD syncdl freq num = %d"
#define RXLEVMON_6702_112_2_17_23_56_9_1035 "L1:Td IncPagingCycle %d, max %d"
#define RXLEVMON_6749_112_2_17_23_56_9_1036 "L1:NewPagingCycle %d"
#define RXLEVMON_6775_112_2_17_23_56_9_1037 "L1 : New TDMeasPolicy %d"
#define RXLEVMON_6821_112_2_17_23_56_9_1038 "L1 : New RSCPNumPerPage %d"
#define RXLEVMON_6907_112_2_17_23_56_10_1039 "L1:Td pch meas state %d"
#define RXLEVMON_6911_112_2_17_23_56_10_1040 "L1:Td pch meas policy %d"
#define RXLEVMON_6956_112_2_17_23_56_10_1041 "L1:GenerateTDRSCPMeasConfig, invalid pamfrms %d"
#define RXLEVMON_6963_112_2_17_23_56_10_1042 "L1:GenerateTDRSCPMeasConfig, num %d"
#define RXLEVMON_6967_112_2_17_23_56_10_1043 "L1:GenerateTDRSCPMeasConfig, rscp_num_per5sec = 0"
#define RXLEVMON_7003_112_2_17_23_56_10_1044 "L1:CheckIfCoperEnable state %d, ind 0x%x"
#define RXLEVMON_7025_112_2_17_23_56_10_1045 "L1:try2disableCoper state %d, oldind 0x%x, newind 0x%x"
#define RXLEVMON_7086_112_2_17_23_56_10_1046 "L1:SetTDSyncCoperState new %d, old %d"
#define RXLEVMON_7117_112_2_17_23_56_10_1047 "L1:SetTDSyncCoperIndex new 0x%x, old 0x%x"
#define RXLEVMON_7201_112_2_17_23_56_10_1048 "L1_TD QQQ: RxLevMon_SetFrameIntChange g_pre_td_meas_frame_int_pos %d, cur_frame_int_pos %d, old_delta_pos %d"
#define RXLEVMON_7233_112_2_17_23_56_10_1049 "L1_TD QQQ: RxLevMon_SetFrameIntChange delta_slot_qbc %d, new delta_pos %d"
#define RXLEVMON_7237_112_2_17_23_56_10_1050 "L1: RxLevMon_SetFrameIntChange first rx slot %d, delta_pos %d"
#define RXLEVMON_7256_112_2_17_23_56_10_1051 "L1_TD QQQ: RxLevMon_TdSyncFrameIntChange pre_frame_int_pos = %d, cur_frame_int_pos = %d"
#define RXLEVMON_7298_112_2_17_23_56_10_1052 "L1_TD: Set Pwr Default Value arfcn[%d] 0x%x, gain %d"
#define RXLEVMON_7663_112_2_17_23_56_11_1053 "L1:BuildTS0Cell ts0cell num %d, firepoint %d"
#define RXLEVMON_7708_112_2_17_23_56_11_1054 "L1_TD: arfcn 0x%x TD_SYNC_DL_FIND"
#define RXLEVMON_7717_112_2_17_23_56_11_1055 "L1_TD: arfcn 0x%x TD_SYNC_DL_FIND"
#define RXLEVMON_7731_112_2_17_23_56_11_1056 "L1:BuildTS0Cell sdlcell num %d, nomalrssi num %d"
#define RXLEVMON_7823_112_2_17_23_56_11_1057 "L1: new cell 2 Syncdl_search, old status %d, freq 0x%x"
#define RXLEVMON_7846_112_2_17_23_56_11_1058 "L1_TD: lose sync pos"
#define RXLEVMON_7913_112_2_17_23_56_12_1059 "L1_TD: delta_pos too big %d"
#define RXLEVMON_7917_112_2_17_23_56_12_1060 "L1_TD: Meas Only rscp = %d"
#define RXLEVMON_7927_112_2_17_23_56_12_1061 "L1_TD: lose sync pos in td sync meas"
#define RXLEVMON_7962_112_2_17_23_56_12_1062 "L1:only td meas cell_meas %d, cell_old %d, status %d"
#define RXLEVMON_7987_112_2_17_23_56_12_1063 "rscp dsp %d, average %d"
#define RXLEVMON_7991_112_2_17_23_56_12_1064 "L1_TD: new max rscp value %d"
#define RXLEVMON_8004_112_2_17_23_56_12_1065 "L1_TD_TEST: meas_td_ptr->meas_td_cell[%d].average_rscp %d"
#define RXLEVMON_8018_112_2_17_23_56_12_1066 "L1_TD: max_id 0x%x, max_rscp %d, snd_id 0x%x, snd_rscp %d"
#define RXLEVMON_8063_112_2_17_23_56_12_1067 "L1: meas rssi3 %d"
#define RXLEVMON_8087_112_2_17_23_56_12_1068 "L1_TD: Only Td Sync ratio = %d, delta_pos = %d"
#define RXLEVMON_8101_112_2_17_23_56_12_1069 "L1_TD: change delta_pos %d"
#define RXLEVMON_8117_112_2_17_23_56_12_1070 "L1_TD: delta_pos too big %d"
#define RXLEVMON_8127_112_2_17_23_56_12_1071 "L1_TD: lose sync pos in only td sync"
#define RXLEVMON_8149_112_2_17_23_56_12_1072 "L1_TD: Both Meas Sync ratio = %d, delta_pos = %d, rscp = %d"
#define RXLEVMON_8181_112_2_17_23_56_12_1073 "L1_TD: delta_pos too big %d"
#define RXLEVMON_8194_112_2_17_23_56_12_1074 "L1_TD: lose sync pos in td sync meas"
#define RXLEVMON_8228_112_2_17_23_56_12_1075 "l1 : ConvertTDCellMid2SDL 0x%x, num %d"
#define RXLEVMON_8233_112_2_17_23_56_12_1076 "l1 : convert mid 0x%x"
#define RXLEVMON_8274_112_2_17_23_56_12_1077 "L1_TD: cell_param_id[%d] 0x%x, delta_pos %d"
#define RXLEVMON_8373_112_2_17_23_56_12_1078 "L1_TD: syncdl process_counter %d"
#define RXLEVMON_8419_112_2_17_23_56_13_1079 "L1: GetTDFreqNum, freq num %d, status %d"
#define RXLEVMON_8445_112_2_17_23_56_13_1080 "L1_TD: Recieve Syncdl, arfcn 0x%x, valid_cell %d, td_meas_sub_type %d, state %d"
#define RXLEVMON_8472_112_2_17_23_56_13_1081 "L1:sdlnum %d, cellnum %d, needmid %d"
#define RXLEVMON_8572_112_2_17_23_56_13_1082 "L1_TD: cell num %d, g_td_need_mid_freq_num %d"
#define RXLEVMON_8596_112_2_17_23_56_13_1083 "L1_TD: syncdl goto TD_MIDAM_CONF_SUBSTATE"
#define RXLEVMON_8607_112_2_17_23_56_13_1084 "L1_TD: syncdl search all fail"
#define RXLEVMON_8619_112_2_17_23_56_13_1085 "L1_TD: resyncdl goto TD_MIDAM_CONF_SUBSTATE"
#define RXLEVMON_8631_112_2_17_23_56_13_1086 "L1_TD: resync goto TD_RSCP_MEAS_SUBSTATE 0 new freq has syncdl"
#define RXLEVMON_8648_112_2_17_23_56_13_1087 "L1_TD: resyncdl search all fail"
#define RXLEVMON_8661_112_2_17_23_56_13_1088 "L1_TD: syncdl goto TD_MIDAM_CONF_SUBSTATE"
#define RXLEVMON_8678_112_2_17_23_56_13_1089 "L1_TD: syncdl search all fail"
#define RXLEVMON_8726_112_2_17_23_56_13_1090 "L1_TD: arfcn 0x%x mid[%d]=0x%x "
#define RXLEVMON_8749_112_2_17_23_56_13_1091 "L1_TD: arfcn 0x%x new mid[%d]=0x%x "
#define RXLEVMON_8761_112_2_17_23_56_13_1092 "L1_TD: Mids lose %d "
#define RXLEVMON_8838_112_2_17_23_56_13_1093 "L1:MergeTDMidFromNW, insert %d, cell num %d"
#define RXLEVMON_8857_112_2_17_23_56_13_1094 "L1:MergeTDMidFromNW, insertnum %d, cell num %d"
#define RXLEVMON_8879_112_2_17_23_56_14_1095 "L1_TD: Recieve Mid, freq 0x%x, valid_cell %d, td_meas_sub_type %d"
#define RXLEVMON_8892_112_2_17_23_56_14_1096 "L1: Mid, sdlcellnum %d, midvalidnum %d, cellnum %d"
#define RXLEVMON_8920_112_2_17_23_56_14_1097 "L1: Mid, freq 0x%x to RSCP"
#define RXLEVMON_8925_112_2_17_23_56_14_1098 "L1: Mid, freq 0x%x to NOMAL RSSI"
#define RXLEVMON_8971_112_2_17_23_56_14_1099 "L1_TD: mid goto IDLE_TD_RSCP_MEAS"
#define RXLEVMON_8986_112_2_17_23_56_14_1100 "L1_TD: mid goto IDLE_TD_RSCP_MEAS"
#define RXLEVMON_9059_112_2_17_23_56_14_1101 "TD CONFLICT: P0 0x%x, P1 0x%x, P2 0x%x, P3 0x%x"
#define RXLEVMON_9066_112_2_17_23_56_14_1102 "L1:meas response conflict & coper"
#define RXLEVMON_9080_112_2_17_23_56_14_1103 "TD CONFLICT dedi num %d, cause %d"
#define RXLEVMON_9096_112_2_17_23_56_14_1104 "TD CONFLICT: TD_PWR"
#define RXLEVMON_9101_112_2_17_23_56_14_1105 "TD CONFLICT: TD_SYNC"
#define RXLEVMON_9120_112_2_17_23_56_14_1106 "TD CONFLICT: TD_RSCP"
#define RXLEVMON_9124_112_2_17_23_56_14_1107 "TD CONFLICT: TD_SYNC_ONLY"
#define RXLEVMON_9129_112_2_17_23_56_14_1108 "TD CONFLICT: TD_SYNC_RSCP"
#define RXLEVMON_9137_112_2_17_23_56_14_1109 "TD CONFLICT: TD_RSSI"
#define RXLEVMON_9143_112_2_17_23_56_14_1110 "TD CONFLICT: TD_SYNCDL"
#define RXLEVMON_9150_112_2_17_23_56_14_1111 "TD CONFLICT: TD_MIDCONF"
#define RXLEVMON_9192_112_2_17_23_56_14_1112 "L1:set prefer td meas in dedi search idle %d, %d"
#define RXLEVMON_9341_112_2_17_23_56_14_1113 "L1_TD: lose sync pos in td sync meas"
#define RXLEVMON_9369_112_2_17_23_56_15_1114 "L1:TS0Rssi cell num %d"
#define RXLEVMON_9374_112_2_17_23_56_15_1115 "L1:TS0Rssi 0x%x, newts0rssi %d, oldts0rssi %d"
#define RXLEVMON_9397_112_2_17_23_56_15_1116 "L1:TS0Rssi 0x%x, wrong state %d, mainstate %d"
#define RXLEVMON_9407_112_2_17_23_56_15_1117 "L1: ts0 rssi result freq index %d, num %d"
#define RXLEVMON_9445_112_2_17_23_56_15_1118 "L1_TD: Recieve Rssi, ratio %d, delta_pos %d, rscp %d"
#define RXLEVMON_9462_112_2_17_23_56_15_1119 "L1_TD: rssi change delta_pos %d"
#define RXLEVMON_9478_112_2_17_23_56_15_1120 "L1_TD: rssi change too big delta_pos 0x%x"
#define RXLEVMON_9489_112_2_17_23_56_15_1121 "L1_TD:  lose sync pos in rssi meas"
#define RXLEVMON_9559_112_2_17_23_56_15_1122 "L1: GprsTDMeasProcess coper Locked."
#define RXLEVMON_9734_112_2_17_23_56_15_1123 "L1 different first rx slot for gprs dwpts , base %d, new %d, before change %d"
#define RXLEVMON_9812_112_2_17_23_56_15_1124 "This arfcn DO NOT happen in GSM mode!!!!"
#define RXLEVMON_9948_112_2_17_23_56_16_1125 "L1 RxLevMon_GetTestArfcnMoniParam rxlev_result %d, rxdbm %d, count %d"
#define SHAREDMEM_PROC_397_112_2_17_23_56_17_1126 "L1Q: sizeof(td_sharedMem_T) %d"
#define SHAREDMEM_PROC_398_112_2_17_23_56_17_1127 "L1Q: sizeof(gsm_rf_para_T) %d"
#define SHAREDMEM_PROC_399_112_2_17_23_56_17_1128 "L1Q: sizeof(DM_SHARE_CTRL_INFO) %d"
#define SHAREDMEM_PROC_432_112_2_17_23_56_17_1129 "L1:get share memory base address,arm:%x,dsp:%x"
#define SHAREDMEM_PROC_461_112_2_17_23_56_17_1130 "L1:print sharemem addr: 2g share 0x%x, shareend 0x%x"
#define SHAREDMEM_PROC_465_112_2_17_23_56_17_1131 "L1:print sharemem addr:gsmrf 0x%x,aud share 0x%x"
#define SHAREDMEM_PROC_506_112_2_17_23_56_17_1132 "L1:print sharemem addr: 2g share 0x%x,gsmrf 0x%x,aud share 0x%x"
#define SHAREDMEM_PROC_515_112_2_17_23_56_17_1133 "L1:base address: 2g share 0x%x,td share 0x%x,gsmrf 0x%x,tdrf 0x%x,aud share 0x%x"
#define SHAREDMEM_PROC_536_112_2_17_23_56_17_1134 "L1:address1: armbase 0x%x, dspbase 0x%x, sharedMem 0x%x, shareend 0x%x"
#define SHAREDMEM_PROC_542_112_2_17_23_56_17_1135 "L1:address2: gsmrf 0x%x, aud share 0x%x"
#define SHAREDMEM_PROC_598_112_2_17_23_56_17_1136 "L1:something wrong when to read status words!!"
#define SHAREDMEM_PROC_629_112_2_17_23_56_17_1137 "L1:something is wrong when to write status words 5 times!!"
#define SHAREDMEM_PROC_662_112_2_17_23_56_17_1138 "L1:something wrong when to read status words!!"
#define SHAREDMEM_PROC_698_112_2_17_23_56_17_1139 "something is wrong when to write status words 5 times!!!!!!!!!!!!!!"
#define SHAREDMEM_PROC_758_112_2_17_23_56_17_1140 "LAYER1: The powermoni queue is empty! \n"
#define SHAREDMEM_PROC_843_112_2_17_23_56_18_1141 "L1_TD: SharedMemProc_GetGsmRfParaMemPtr"
#define SHAREDMEM_PROC_877_112_2_17_23_56_18_1142 "L1: New ReducedFn 0x%x"
#define SHAREDMEM_PROC_1262_112_2_17_23_56_18_1143 "L1: send chanchange sig"
#define SHAREDMEM_PROC_1514_112_2_17_23_56_19_1144 "AUD_NEW: SharedMemProc_PostAudCmdQ next_cmd_in32 %d, next_cmd_out32 %d"
#define SHAREDMEM_PROC_1553_112_2_17_23_56_19_1145 "AUD_NEW: SharedMemProc_PostCmdQ next_cmd_in16 %d, next_cmd_out16 %d"
#define SHAREDMEM_PROC_1599_112_2_17_23_56_19_1146 "AUD_NEW: SharedMemProc_PostAudCmdQ next_cmd_in %d, next_cmd_out %d"
#define SHAREDMEM_PROC_1642_112_2_17_23_56_19_1147 "L1: The rx queue is empty!"
#define SHAREDMEM_PROC_1790_112_2_17_23_56_19_1148 "audio in out: %d,%d"
#define SHAREDMEM_PROC_1973_112_2_17_23_56_20_1149 "TALENT: index %d, pwr %d"
#define SHAREDMEM_PROC_1995_112_2_17_23_56_20_1150 "SetTxFramePcl, base address 0x%x, writein 0x%x"
#define SHAREDMEM_PROC_2177_112_2_17_23_56_20_1151 "L1 cancel frame after wake up!, %x"
#define SHAREDMEM_PROC_2442_112_2_17_23_56_21_1152 "L1:Wake up DSP, and sleep frame is at out + 1!!"
#define SHAREDMEM_PROC_2624_112_2_17_23_56_21_1153 "L1_TD sync mode change: SharedMemProc_CancelObsoleteFrames distance_between_in_out = 0"
#define SHAREDMEM_PROC_2632_112_2_17_23_56_21_1154 "L1:still Waiting for wakeup's response!"
#define SHAREDMEM_PROC_2644_112_2_17_23_56_21_1155 "L1: TD not cancel frame1"
#define SHAREDMEM_PROC_2694_112_2_17_23_56_21_1156 "L1 cancel search :%x"
#define SHAREDMEM_PROC_2734_112_2_17_23_56_21_1157 "L1:SharedMemProc_CancelObsoleteFrames: total = %d!"
#define SHAREDMEM_PROC_2817_112_2_17_23_56_21_1158 "L1 find ask more flag more than once!"
#define SHAREDMEM_PROC_2873_112_2_17_23_56_22_1159 "L1 cancel margin %d, iscancel %d, newin %d, oldin %d, out %d, askmore %d"
#define SHAREDMEM_PROC_2949_112_2_17_23_56_22_1160 "L1 cancel newaskmore  %d"
#define SHAREDMEM_PROC_3008_112_2_17_23_56_22_1161 "L1:still Waiting for wakeup's response!"
#define SHAREDMEM_PROC_3016_112_2_17_23_56_22_1162 "L1,CancelTBFSearchFrames,frame_index_out :%d,rf_op:%d.n"
#define SHAREDMEM_PROC_3093_112_2_17_23_56_22_1163 "L1:distance between in and out is 0"
#define SHAREDMEM_PROC_3187_112_2_17_23_56_22_1164 "SharedMemProc_CancelRemainingFrames:channel_mode_update"
#define SHAREDMEM_PROC_3194_112_2_17_23_56_22_1165 "SharedMemProc_CancelRemainingFrames:TA update"
#define SHAREDMEM_PROC_3201_112_2_17_23_56_22_1166 "SharedMemProc_CancelRemainingFrames:power update"
#define SHAREDMEM_PROC_3209_112_2_17_23_56_22_1167 "SharedMemProc_CancelRemainingFrames:channel change"
#define SHAREDMEM_PROC_3221_112_2_17_23_56_22_1168 "SharedMemProc_CancelRemainingFrames:gprs ta update"
#define SHAREDMEM_PROC_3229_112_2_17_23_56_22_1169 "SharedMemProc_CancelRemainingFrames:gprs usf update"
#define SHAREDMEM_PROC_3235_112_2_17_23_56_22_1170 "SharedMemProc_CancelRemainingFrames:gprs channel update"
#define SHAREDMEM_PROC_3327_112_2_17_23_56_22_1171 "L1 set scell gain : result %x"
#define SHAREDMEM_PROC_3394_112_2_17_23_56_23_1172 "L1 error: UpdateRxDataOutPtr rx buffer empty, rxdata in %d, rxdata out %d"
#define SHAREDMEM_PROC_4033_112_2_17_23_56_24_1173 "audio3: write init para aud_sharedMem->gsm_aud_para_buf[%d].dl_da_device_l %d, aud_sharedMem->td_aud_para_buf[%d].dl_da_device_l %d"
#define SHAREDMEM_PROC_4426_112_2_17_23_56_25_1174 "WriteGsmRfpara2DspRam: write init para aud_sharedMem->gsm_aud_para_buf[%d].dl_da_device_l %d, aud_sharedMem->td_aud_para_buf[%d].dl_da_device_l %d"
#define SHAREDMEM_PROC_4430_112_2_17_23_56_25_1175 "L1_TD: WriteGsmRfpara2DspRam"
#define SHAREDMEM_PROC_4439_112_2_17_23_56_25_1176 "system_param_dsp_use_ptr 0x%x"
#define SHAREDMEM_PROC_4440_112_2_17_23_56_25_1177 "system_param_and_others_version 0x%x"
#define SHAREDMEM_PROC_4505_112_2_17_23_56_25_1178 "L1_TD: memset td monitor item"
#define SHAREDMEM_PROC_4525_112_2_17_23_56_25_1179 "L1: L1_Set_Cali_Flag, flag %d"
#define SHAREDMEM_PROC_4533_112_2_17_23_56_25_1180 "L1: L1_Get_Cali_Flag, flag %d"
#define SHAREDMEM_PROC_4555_112_2_17_23_56_25_1181 "L1: --assert_count %d, dsp_is_ready_flag %d, addr 0x%x"
#define SHAREDMEM_PROC_4558_112_2_17_23_56_25_1182 "L1: assert_count %d, dsp_is_ready_flag %d, addr 0x%x"
#define SHAREDMEM_PROC_4561_112_2_17_23_56_25_1183 "L1_TD:dsp rat state %d"
#define SHAREDMEM_PROC_5172_112_2_17_23_56_26_1184 "L1:divide slp.slp cnt 0x%x,sec slp 0x%x,out %x,crr t:0x%x,nea t:0x%x"
#define SHAREDMEM_PROC_5711_112_2_17_23_56_27_1185 "L1: download_voice_data_buf[%x][0] = %X"
#define SHAREDMEM_PROC_5713_112_2_17_23_56_27_1186 "L1: download_voice_data_buf[%x][0] = %X"
#define SHAREDMEM_PROC_5747_112_2_17_23_56_27_1187 "L1_TD: SharedMemProc_SetRfParameter"
#define SHAREDMEM_PROC_5934_112_2_17_23_56_28_1188 "L1_TD: SharedMemProc_SetDevModeParam"
#define SHAREDMEM_PROC_5948_112_2_17_23_56_28_1189 "audio3: SharedMemProc_SetDevModeParam gsmRfParaMem->audio_param.dl_da_device_l %d"
#define SHAREDMEM_PROC_5962_112_2_17_23_56_28_1190 "AUD_NEW: SharedMemProc_SetDevModeParam aud_sharedMem->gsm_aud_para_buf[%d].dl_da_device_l %d, aud_sharedMem->td_aud_para_buf[%d].dl_da_device_l %d"
#define SHAREDMEM_PROC_5988_112_2_17_23_56_28_1191 "AUD_NEW: SharedMemProc_SetDevModeParam, audio_buff_index %d"
#define SHAREDMEM_PROC_6111_112_2_17_23_56_28_1192 "L1_TD: SharedMemProc_SetDevModeParam"
#define SHAREDMEM_PROC_6125_112_2_17_23_56_28_1193 "audio3: SharedMemProc_SetDevModeParam gsmRfParaMem->audio_param.dl_da_device_l %d"
#define SHAREDMEM_PROC_6134_112_2_17_23_56_28_1194 "AUD_NEW: SharedMemProc_SetDevModeParam aud_sharedMem->gsm_aud_para_buf[%d].dl_da_device_l %d, aud_sharedMem->td_aud_para_buf[%d].dl_da_device_l %d"
#define SHAREDMEM_PROC_6164_112_2_17_23_56_28_1195 "AUD_NEW: SharedMemProc_SetDevModeParam, audio_buff_index %d"
#define SHAREDMEM_PROC_6224_112_2_17_23_56_28_1196 "download param %ld, %ld"
#define SHAREDMEM_PROC_6593_112_2_17_23_56_29_1197 "L1:test_count = %d"
#define SHAREDMEM_PROC_6842_112_2_17_23_56_29_1198 "L1 relation:%x,%x,%x,%x,%x,%x,%x,%x"
#define SHAREDMEM_PROC_6868_112_2_17_23_56_29_1199 "AUD_NEW: SharedMemProc_GetAudioParaBuffIndex return value is err!"
#define SHAREDMEM_PROC_6907_112_2_17_23_56_29_1200 "L1:chipid 0x%x,set endian %d"
#define SHAREDMEM_PROC_6946_112_2_17_23_56_30_1201 "L1G: *(td_shared_rf + ANTEN_OFFSET) 0x%x, *(td_shared_rf + TDPA_OFFSET) 0x%x"
#define SHAREDMEM_PROC_6950_112_2_17_23_56_30_1202 "L1G: get td rf para from nv fail!"
#define SHAREDMEM_PROC_6972_112_2_17_23_56_30_1203 "L1_TD: memset td monitor item"
#define SHAREDMEM_PROC_7005_112_2_17_23_56_30_1204 "L1_Trans_GsmRf_Para: write init para aud_sharedMem->gsm_aud_para_buf[%d].dl_da_device_l %d, aud_sharedMem->td_aud_para_buf[%d].dl_da_device_l %d"
#define SHAREDMEM_PROC_7013_112_2_17_23_56_30_1205 "L1_TD: L1_Trans_GsmRf_Para"
#define SHAREDMEM_PROC_7028_112_2_17_23_56_30_1206 "system_param_dsp_use_ptr 0x%x"
#define SHAREDMEM_PROC_7029_112_2_17_23_56_30_1207 "system_param_and_others_version 0x%x"
#define SHAREDMEM_PROC_7101_112_2_17_23_56_30_1208 "L1: L1_Set_Cali_Flag, flag %d"
#define SHAREDMEM_PROC_7126_112_2_17_23_56_30_1209 "L1: assert_count %d, dsp_is_ready_flag %d, addr 0x%x"
#define SHAREDMEM_PROC_7129_112_2_17_23_56_30_1210 "L1_TD:dsp rat state %d"
#define SHAREDMEM_PROC_7158_112_2_17_23_56_30_1211 "L1:base address: 2g share 0x%x,td share 0x%x,gsmrf 0x%x,tdrf 0x%x,aud share 0x%x"
#define SHAREDMEM_PROC_7216_112_2_17_23_56_30_1212 "2cancel tdmeas frame para out=%d, in=%d, index=%d"
#define SHAREDMEM_PROC_7243_112_2_17_23_56_30_1213 "L1_TD: SharedMemProc_TD_Cancel_plot_include_PTCCH frame_index_in %d"
#define SHAREDMEM_PROC_7266_112_2_17_23_56_30_1214 "TDMeasParam in=0x%x,  arfcn=0x%x"
#define SHAREDMEM_PROC_7296_112_2_17_23_56_30_1215 "WriteTDPara in %d, out %d"
#define SHAREDMEM_PROC_7356_112_2_17_23_56_30_1216 "L1:Reset tdmeas para queue in=%d, out=%d"
#define SHAREDMEM_PROC_7395_112_2_17_23_56_30_1217 "cancel tdmeas frame para out=%d, in=%d, index=%d"
#define SHAREDMEM_PROC_7533_112_2_17_23_56_31_1218 "L1: Idle Sync, arfcn 0x%x, gain %d"
#define SHAREDMEM_PROC_7561_112_2_17_23_56_31_1219 "L1_TD:Idle AGC arfcn 0x%x, gain %d, ts0_delta_pos %d, status %d"
#define SHAREDMEM_PROC_7647_112_2_17_23_56_31_1220 "L1_TD: cell[%d] 0x%x"
#define SHAREDMEM_PROC_7651_112_2_17_23_56_31_1221 "L1: RSCP Plot arfcn 0x%x, gain %d, td_meas_sub_type %d, cellnum %d"
#define SHAREDMEM_PROC_7717_112_2_17_23_56_31_1222 "L1_TD: arfcn 0x%x, gain %d, delta_pos %d, td_meas_sub_type %d"
#define SHAREDMEM_PROC_7731_112_2_17_23_56_31_1223 "L1_TD: arfcn 0x%x, gain %d, delta_pos %d, td_meas_sub_type %d"
#define SHAREDMEM_PROC_7743_112_2_17_23_56_31_1224 "L1_TD: arfcn 0x%x, gain %d, delta_pos %d, td_meas_sub_type %d"
#define SHAREDMEM_PROC_7752_112_2_17_23_56_31_1225 "L1_TD: arfcn 0x%x, gain %d, delta_pos %d, td_meas_sub_type %d"
#define SHAREDMEM_PROC_7761_112_2_17_23_56_31_1226 "L1_TD: arfcn 0x%x, gain %d, delta_pos %d, td_meas_sub_type %d"
#define SHAREDMEM_PROC_7816_112_2_17_23_56_31_1227 "L1_TD: arfcn 0x%x, gain %d, delta_pos %d, td_meas_sub_type %d, syncmid %d"
#define SHAREDMEM_PROC_7822_112_2_17_23_56_31_1228 "L1_TD: cell[%d] 0x%x"
#define SHAREDMEM_PROC_7876_112_2_17_23_56_31_1229 "L1_TD: arfcn 0x%x, gain %d, delta_pos %d, td_meas_sub_type %d, td_meas_ind=%d, mid_valid=%d, sdl_num=%d"
#define SHAREDMEM_PROC_7881_112_2_17_23_56_31_1230 "L1_TD: cell[%d] 0x%x"
#define SHAREDMEM_PROC_7897_112_2_17_23_56_31_1231 "L1_TD: cell[%d] 0x%x"
#define SHAREDMEM_PROC_7971_112_2_17_23_56_32_1232 "L1_TD: gsm scell not exist, nonsync mode change"
#define SHAREDMEM_PROC_8036_112_2_17_23_56_32_1233 "L1_TD: set td sync para arfcn_index %d, gain %d, bsic %d"
#define SHAREDMEM_PROC_8062_112_2_17_23_56_32_1234 "L1Q: SharedMemProc_GetGSMSyncPara start"
#define SHAREDMEM_PROC_8067_112_2_17_23_56_32_1235 "L1Q: g_dualmode_gsm_sfn is invalid, return"
#define SHAREDMEM_PROC_8071_112_2_17_23_56_32_1236 "L1Q: g_dualmode_gsm_sfn 0x%x"
#define SHAREDMEM_PROC_8072_112_2_17_23_56_32_1237 "L1Q: gsm_cell_num %d"
#define SHAREDMEM_PROC_8080_112_2_17_23_56_32_1238 "L1Q: find arfcn 0x%x at %d  addfn %d"
#define SHAREDMEM_PROC_8087_112_2_17_23_56_32_1239 "L1Q: arfcn_index 0x%x, Agc 0x%x, Fn 0x%x"
#define SHAREDMEM_PROC_8095_112_2_17_23_56_32_1240 "audio3: SharedMemProc_GetGSMSyncPara g_audioPara_buffer %d, g_audio_length %d"
#define SHAREDMEM_PROC_8096_112_2_17_23_56_32_1241 "L1Q: SharedMemProc_SetDevModeParam"
#define SHAREDMEM_PROC_8106_112_2_17_23_56_32_1242 "L1Q: cell_index 0x%x, bsic 0x%x"
#define SHAREDMEM_PROC_8113_112_2_17_23_56_32_1243 "L1Q: is handover"
#define SHAREDMEM_PROC_8136_112_2_17_23_56_32_1244 "L1Q: is cell reselect"
#define SHAREDMEM_PROC_8145_112_2_17_23_56_32_1245 "L1Q: arfcn not match, require 0x%x, in array[%d] 0x%x"
#define SHAREDMEM_PROC_8150_112_2_17_23_56_32_1246 "L1Q: dm_share_ctrl_info->gsm_cell_is_valid[%d] is 0"
#define SHAREDMEM_PROC_8156_112_2_17_23_56_32_1247 "L1Q: arfcn is not match, return"
#define SHAREDMEM_PROC_8160_112_2_17_23_56_32_1248 "L1Q: goto sync mode change, return"
#define SHAREDMEM_PROC_8180_112_2_17_23_56_32_1249 "L1:GetGSMSyncStatus %d"
#define SHAREDMEM_PROC_8200_112_2_17_23_56_32_1250 "L1:GetRatChgStatus %d"
#define SHAREDMEM_PROC_8216_112_2_17_23_56_32_1251 "L1:SharedMemProc_SetDAPCMDate"
#define SHAREDMEM_PROC_8229_112_2_17_23_56_32_1252 "L1:SharedMemProc_SetNetPCMDate"
#define SHAREDMEM_PROC_8423_112_2_17_23_56_33_1253 "L1_TD: main card otd %d"
#define SHAREDMEM_PROC_8441_112_2_17_23_56_33_1254 "L1_TD: fn_card %d, delta_pos %d, first_slot %d"
#define SHAREDMEM_PROC_8477_112_2_17_23_56_33_1255 "L1_TD: fn:0x%x, delta_pos %d, first_slot %d"
#define SHAREDMEM_PROC_8494_112_2_17_23_56_33_1256 "L1_TD: i %d, card_id %d, cell_index %d, arfcn_index %d, otd %d"
#define SHAREDMEM_PROC_8517_112_2_17_23_56_33_1257 "\"#3 ext_cmd_in=%d,out= %d"
#define SHAREDMEM_PROC_8524_112_2_17_23_56_33_1258 "buffer_add_in=%x"
#define SHAREDMEM_PROC_8551_112_2_17_23_56_33_1259 "\"#2 ext_in=%d,out= %d, id= %d"
#define SHAREDMEM_PROC_8554_112_2_17_23_56_33_1260 "buffer_add=%x"
#define SHAREDMEM_PROC_8578_112_2_17_23_56_33_1261 "\"# moved_extern_count=%d"
#define SYSTEMINFO_874_112_2_17_23_56_35_1262 "L1: freq_redefine changed!!"
#define SYSTEMINFO_939_112_2_17_23_56_35_1263 "L1: freq_redefine changed!!"
#define SYSTEMINFO_1198_112_2_17_23_56_35_1264 "curr_fn %d,delta_fn %d,change delay %d,start_fn %d"
#define SYSTEMINFO_1385_112_2_17_23_56_36_1265 " Current chan same as next chan"
#define SYSTEMINFO_1390_112_2_17_23_56_36_1266 " Current chan different from next chan"
#define SYSTEMINFO_1447_112_2_17_23_56_36_1267 "L1:loop_back paras: p1= %d, p2= %d!\n"
#define SYSTEMINFO_1955_112_2_17_23_56_37_1268 "L1:NOTE: NOT ANY response from DSP for TCH search!! fn %d"
#define SYSTEMINFO_2163_112_2_17_23_56_37_1269 "L1: cbch schedule start tb %d, tb"
#define SYSTEMINFO_2170_112_2_17_23_56_37_1270 "L1:card id: %d,first schedule s_fn 0x%x,block num %d"
#define SYSTEMINFO_2237_112_2_17_23_56_38_1271 "L1: schedule blocki %d"
#define SYSTEMINFO_2260_112_2_17_23_56_38_1272 "L1:current schedule out 0x%x, blocki %d"
#define SYSTEMINFO_2265_112_2_17_23_56_38_1273 "L1:end schedule and start receive all block 0-4"
#define SYSTEMINFO_2765_112_2_17_23_56_39_1274 "L1 is first for tch %d"
#define SYSTEMINFO_2917_112_2_17_23_56_39_1275 "L1 fn_sacchint %x,fn_next_sacch %x,fn_mod_sacch %x, fn_period %x"
#define SYSTEMINFO_2961_112_2_17_23_56_39_1276 "L1 set frame type: is_debug_on %d,type %d, arfcn %x,gain %x"
#define SYSTEMINFO_2985_112_2_17_23_56_39_1277 "L1 frametype %d, is_debug_mode %d"
#define TAPWR_126_112_2_17_23_56_39_1278 "SetTxFramePcl, pwr_frame_ind %d, pwr_buf_ind %d, pcl[%d, %d, %d, %d]"
#define TAPWR_223_112_2_17_23_56_40_1279 "L1: TaPwr_GetTxPwrCapabilty not suitable band\n"
#define TAPWR_256_112_2_17_23_56_40_1280 "L1: SetMaxOutPutPowerLevel not suitable band\n"
#define LAYER1_CALL_MAC_PH_150_112_2_17_23_56_43_1281 "\n s_polling_num = %d"
#define LAYER1_CALL_MAC_PH_159_112_2_17_23_56_43_1282 "polling data send_type = %d"
#define LAYER1_CALL_MAC_PH_172_112_2_17_23_56_43_1283 "polling data send_type = %d"
#define PROCESS_MSG_876_112_2_17_23_56_46_1284 "Script file is wrong!"
#define PROCESS_MSG_1774_112_2_17_23_56_47_1285 "L1:this rxlev data line is invalid,please update!"
#define PROCESS_MSG_1824_112_2_17_23_56_48_1286 "L1:this sch data line is invalid,please update!"
#define TEST_HANDLE_MSG_423_112_2_17_23_56_50_1287 "sleep counter =%d\n"
#define TEST_HANDLE_MSG_809_112_2_17_23_56_51_1288 "sleep counter =%d\n"
#define TEST_HANDLE_MSG_1507_112_2_17_23_56_52_1289 "LAYER1: RFoperation is wrong!\n"
#define TEST_HANDLE_MSG_1730_112_2_17_23_56_53_1290 "\n SARAH test_search_response arfcn %d,T1 %d,T2 %d,T3P %d,BSIC %d"
#define TEST_HANDLE_MSG_1785_112_2_17_23_56_53_1291 "\n SARAH test_send_SCH_data_to_MCU T1 %d,T2 %d,T3P %d,BSIC %d"
#define TEST_HANDLE_MSG_1977_112_2_17_23_56_53_1292 "wake up:sleep counter %d\n"
#define TEST_RW_SHARED_MEM_266_112_2_17_23_56_55_1293 "L1:command is NULL!!!"
#define TEST_RW_SHARED_MEM_526_112_2_17_23_56_55_1294 "Rx data from channel has been read over!"
#define MMI_OSBRIDGE_39_112_2_17_23_56_56_1295 "MMI : alloc signal error, signal szie = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(DM_2G_LAYER1_TRC)
TRACE_MSG(CALIBRATION_1485_112_2_17_23_53_17_0,"L1_TD: Transfer Gsm Rf Para in Calibration")
TRACE_MSG(CALIBRATION_1521_112_2_17_23_53_17_1,"L1_TD: Calibration_Entry wait dsp ready start %d")
TRACE_MSG(CALIBRATION_1523_112_2_17_23_53_18_2,"L1_TD: Calibration_Entry wait dsp ready finish %d")
TRACE_MSG(CALIBRATION_1572_112_2_17_23_53_18_3,"L1:calibration,recv unknown sig %d")
TRACE_MSG(CALIBRATION_1782_112_2_17_23_53_18_4,"L1:caliberation diag paramter is wrong!")
TRACE_MSG(CALIBRATION_1915_112_2_17_23_53_18_5,"L1:band=%x,arfcn=%x,ind=%x,val=%x,type=%x,count=%x")
TRACE_MSG(CALIBRATION_2489_112_2_17_23_53_20_6,"L1:handle tx request band = %d")
TRACE_MSG(CALIBRATION_2590_112_2_17_23_53_20_7,"L1:handle tx request band = %d")
TRACE_MSG(CALIBRATION_2660_112_2_17_23_53_20_8,"L1:SEND 8PSK TX REQ")
TRACE_MSG(CALIBRATION_2699_112_2_17_23_53_20_9,"L1:STOP 8PSK TX ")
TRACE_MSG(CALIBRATION_3022_112_2_17_23_53_21_10,"L1:set EGSM index %d gain= %x")
TRACE_MSG(CALIBRATION_3037_112_2_17_23_53_21_11,"L1:RX REQ g_layer1_state =%d ")
TRACE_MSG(CALIBRATION_3193_112_2_17_23_53_21_12,"L1:RX REQ g_layer1_state =%d ")
TRACE_MSG(CALIBRATION_3256_112_2_17_23_53_21_13,"L1:set EGSM index %d gain= %x")
TRACE_MSG(CALIBRATION_3291_112_2_17_23_53_21_14,"L1:set DCS index %d gain= %x")
TRACE_MSG(CALIBRATION_3325_112_2_17_23_53_21_15,"L1:set PCS index %d gain= %x")
TRACE_MSG(CALIBRATION_3361_112_2_17_23_53_21_16,"L1:set GSM850 index %d gain= %x")
TRACE_MSG(CALIBRATION_3380_112_2_17_23_53_21_17,"L1:RX REQ g_layer1_state =%d ")
TRACE_MSG(CALIBRATION_3554_112_2_17_23_53_22_18,"L1:RSSI== %x")
TRACE_MSG(CALIBRATION_3798_112_2_17_23_53_22_19,"L1:RF control Request,value = %d")
TRACE_MSG(CALIBRATION_3802_112_2_17_23_53_22_20,"L1:RF control Request,length = %d")
TRACE_MSG(CALIBRATION_3814_112_2_17_23_53_22_21,"L1:RF control Request,gpip_no = %d value = %d")
TRACE_MSG(CALIBRATION_3827_112_2_17_23_53_22_22,"L1:RF control Request,tx = %d")
TRACE_MSG(CALIBRATION_4015_112_2_17_23_53_23_23,"L1 DC voltage i %x, q %x, count %d")
TRACE_MSG(CALIBRATION_4133_112_2_17_23_53_23_24,"L1:RAMP_ADD=%x,OFFSET=%x")
TRACE_MSG(CALIBRATION_4252_112_2_17_23_53_23_25,"GET iq avg == %x, count %d")
TRACE_MSG(CALIBRATION_4393_112_2_17_23_53_23_26,"L1:rxtxloop, savesigal mode error = %d")
TRACE_MSG(CALIBRATION_4409_112_2_17_23_53_23_27,"L1:rxtxloop, new pcl %d")
TRACE_MSG(CALIBRATION_4431_112_2_17_23_53_23_28,"L1:rxtxloop, new cmd=%d")
TRACE_MSG(CALIBRATION_4449_112_2_17_23_53_23_29,"L1:rxtxloop wakedsp1 =%d")
TRACE_MSG(CALIBRATION_4524_112_2_17_23_53_24_30,"L1:rxtxloop init, save msg error.")
TRACE_MSG(CALIBRATION_4531_112_2_17_23_53_24_31,"L1:rxtxloop init, layer1 state error.")
TRACE_MSG(CALIBRATION_4536_112_2_17_23_53_24_32,"L1:rxtxloop init, substate error")
TRACE_MSG(CALIBRATION_4818_112_2_17_23_53_24_33,"L1:rxtxloop, update nv start")
TRACE_MSG(CALIBRATION_4834_112_2_17_23_53_24_34,"L1:rxtxloop, update nv end")
TRACE_MSG(CALIBRATION_4892_112_2_17_23_53_24_35,"L1:rxtxloop, req sig, cmd=%d")
TRACE_MSG(CALIBRATION_4928_112_2_17_23_53_24_36,"L1:loop s=%d fn=x%x")
TRACE_MSG(CALIBRATION_4973_112_2_17_23_53_25_37,"L1:change arfcn")
TRACE_MSG(CALIBRATION_5026_112_2_17_23_53_25_38,"L1:SetState=%d")
TRACE_MSG(CALIBRATION_5105_112_2_17_23_53_25_39,"L1:rxlev2diag 0x%x")
TRACE_MSG(CALIBRATION_5209_112_2_17_23_53_25_40,"L1:rxtxloop, errors %d, cmd %d, substate %d")
TRACE_MSG(CALIBRATION_5288_112_2_17_23_53_25_41,"L1:rxtxloop, verify sch delay=%d, fn %d")
TRACE_MSG(CALIBRATION_5320_112_2_17_23_53_25_42,"L1:rxtxloop2next=%d")
TRACE_MSG(CALIBRATION_5386_112_2_17_23_53_25_43,"L1:rxtxloop, no bcch or wrong askmore passed here , which triggered in search")
TRACE_MSG(CALIBRATION_5460_112_2_17_23_53_26_44,"L1:rxtxloop startcamp, target=%d")
TRACE_MSG(CALIBRATION_5518_112_2_17_23_53_26_45,"L1:rxtxloop, target=%d,gain=%d")
TRACE_MSG(CALIBRATION_5556_112_2_17_23_53_26_46,"L1:rxtxloop, meas n=%d")
TRACE_MSG(CALIBRATION_5638_112_2_17_23_53_26_47,"rxtxloop:RxBlockReady")
TRACE_MSG(CALIBRATION_5681_112_2_17_23_53_26_48,"L1:rxtxloop, search/read state machine error.!!!")
TRACE_MSG(CALIBRATION_5687_112_2_17_23_53_26_49,"L1:rxtxloop, rxblock channel tag error !!!")
TRACE_MSG(CALIBRATION_5708_112_2_17_23_53_26_50,"L1:rxtxloop, sch verify sync error.")
TRACE_MSG(CALIBRATION_5718_112_2_17_23_53_26_51,"L1:rxtxloop, sch data sync error.")
TRACE_MSG(CALIBRATION_5849_112_2_17_23_53_26_52,"rxtxloop, sch verify, deltaold=%d, deltanew=%d")
TRACE_MSG(CALIBRATION_5876_112_2_17_23_53_27_53,"L1:rxtxloop, sch successed, oldfn=%d, curfn=%d, deltafn=%d")
TRACE_MSG(CALIBRATION_5894_112_2_17_23_53_27_54,"L1:sch failed, cell=%d, count=%d")
TRACE_MSG(CALIBRATION_5926_112_2_17_23_53_27_55,"L1:rxtxloop meas count=%d, result=%d")
TRACE_MSG(CALIBRATION_5946_112_2_17_23_53_27_56,"rxtxloop, not right arfcn queue")
TRACE_MSG(CALIBRATION_5976_112_2_17_23_53_27_57,"L1:rxtxloop, measreport lev x%x, qual x%x")
TRACE_MSG(CALIBRATION_5991_112_2_17_23_53_27_58,"L1:rxtxloop, readpwrque")
TRACE_MSG(CALIBRATION_6054_112_2_17_23_53_27_59,"L1:rxtxloop, BELOW_NOISE_LO")
TRACE_MSG(CALIBRATION_6059_112_2_17_23_53_27_60,"L1:rxtxloop, GOOD, arfcn=%d")
TRACE_MSG(CALIBRATION_6129_112_2_17_23_53_27_61,"L1:txtxloop, agc=%d")
TRACE_MSG(CALIBRATION_6151_112_2_17_23_53_27_62,"L1:rxtxloop, start search fn 0x%x")
TRACE_MSG(CALIBRATION_6193_112_2_17_23_53_27_63,"L1:loop, change arfcn %d, tn %d, tsc %d")
TRACE_MSG(CALIBRATION_6225_112_2_17_23_53_27_64,"L1:loop, arfcn %d, tn %d, tsc %d")
TRACE_MSG(CALIBRATION_6296_112_2_17_23_53_27_65,"L1:idle askmore1")
TRACE_MSG(CALIBRATION_6302_112_2_17_23_53_27_66,"L1:idle askmore2")
TRACE_MSG(CALIBRATION_6369_112_2_17_23_53_27_67,"L1_TD: Transfer Gsm Rf Para in Calibration")
TRACE_MSG(CALIBRATION_6405_112_2_17_23_53_28_68,"L1_TD: Calibration_Entry wait dsp ready start %d")
TRACE_MSG(CALIBRATION_6407_112_2_17_23_53_28_69,"L1_TD: Calibration_Entry wait dsp ready finish %d")
TRACE_MSG(CALIBRATION_6518_112_2_17_23_53_28_70,"fastcali SaveParamSet cmd %d, count %d")
TRACE_MSG(CALIBRATION_6658_112_2_17_23_53_28_71,"fastcali parse param %d, rxcount %d")
TRACE_MSG(CALIBRATION_6668_112_2_17_23_53_28_72,"fastcali parse count %d")
TRACE_MSG(CALIBRATION_6678_112_2_17_23_53_28_73,"fastcali error %d")
TRACE_MSG(CALIBRATION_6691_112_2_17_23_53_28_74,"fastcali parse factor %d, index %d")
TRACE_MSG(CALIBRATION_6726_112_2_17_23_53_28_75,"fastcali parse agc ind %d, agc val 0x%x")
TRACE_MSG(CALIBRATION_6794_112_2_17_23_53_28_76,"fastcali err=%d, substate=%d")
TRACE_MSG(CALIBRATION_6821_112_2_17_23_53_28_77,"fastcali idle cmd %d")
TRACE_MSG(CALIBRATION_6860_112_2_17_23_53_28_78," FASTCALI_START_TX_8PSK parameter type is WRONG! ")
TRACE_MSG(CALIBRATION_6872_112_2_17_23_53_28_79," FASTCALI get starting 8psk parameter %d,%d %d")
TRACE_MSG(CALIBRATION_6929_112_2_17_23_53_29_80,"L1 GetSCHSet arfcn 0x%x, band %d, gain ind %d, gain val 0x%x")
TRACE_MSG(CALIBRATION_6958_112_2_17_23_53_29_81,"fastcali Read , arfcn %d, fn 0x%x")
TRACE_MSG(CALIBRATION_7008_112_2_17_23_53_29_82,"fastcali Search , arfcn %d, fn 0x%x")
TRACE_MSG(CALIBRATION_7030_112_2_17_23_53_29_83,"fastcali Search , end fn 0x%x")
TRACE_MSG(CALIBRATION_7051_112_2_17_23_53_29_84,"fastcali last askmore.")
TRACE_MSG(CALIBRATION_7065_112_2_17_23_53_29_85,"fastcali askmore rx fn %d, state %d")
TRACE_MSG(CALIBRATION_7101_112_2_17_23_53_29_86,"fastcali askmore rx fn %d")
TRACE_MSG(CALIBRATION_7167_112_2_17_23_53_29_87,"s_fastcali_slot_rxlev_counter=%d, count=%d")
TRACE_MSG(CALIBRATION_7176_112_2_17_23_53_29_88,"RSSI[%d]=%d")
TRACE_MSG(CALIBRATION_7218_112_2_17_23_53_29_89,"fastcali rHandlePowerQue state %d")
TRACE_MSG(CALIBRATION_7251_112_2_17_23_53_29_90,"fastcali read from Power mon queue fail! ")
TRACE_MSG(CALIBRATION_7254_112_2_17_23_53_29_91,"fastcali rHandlePowerQue counter %d")
TRACE_MSG(CALIBRATION_7328_112_2_17_23_53_29_92,"FASTCALI_ProcessRxSch, synced_fn 0x%x, oldfn 0x%x, cur_fn 0x%x, searchcount %d, spentcount %d")
TRACE_MSG(CALIBRATION_7366_112_2_17_23_53_29_93,"fastcali sch channel tag error !!!")
TRACE_MSG(CALIBRATION_7372_112_2_17_23_53_29_94,"fastcali sch read buffer fail !!!")
TRACE_MSG(CALIBRATION_7404_112_2_17_23_53_30_95,"fastcali sch resq sub state error %d")
TRACE_MSG(CALIBRATION_7423_112_2_17_23_53_30_96,"fastcali GetRxTstFrmDelay startfn %d, curfn %d")
TRACE_MSG(CALIBRATION_7462_112_2_17_23_53_30_97,"fastcali triggerrx error : can't find rx frame")
TRACE_MSG(CALIBRATION_7484_112_2_17_23_53_30_98,"fastcali FindFirstRxFrameIndex %d isfind 1")
TRACE_MSG(CALIBRATION_7512_112_2_17_23_53_30_99,"fastcali FindFirstRxFrameIndex %d isfind %d")
TRACE_MSG(CALIBRATION_7542_112_2_17_23_53_30_100,"fastcali plot fn 0x%x")
TRACE_MSG(CALIBRATION_7565_112_2_17_23_53_30_101,"fastcali plot tx 0x%x")
TRACE_MSG(CALIBRATION_7966_112_2_17_23_53_31_102,"FASTCALI PlotEdgeTxFrames arfcn%d:txpareptr = 0x%x")
TRACE_MSG(CALIBRATION_7992_112_2_17_23_53_31_103,"FASTCALI txgain_level_index %d:txgain_level = %d")
TRACE_MSG(CALIBRATION_8029_112_2_17_23_53_31_104,"fastcali sch fail , substate %d, fcch ind %d, cell index %d, search_timeout %d")
TRACE_MSG(CALIBRATION_8051_112_2_17_23_53_31_105,"fastcali sch read fail , substate %d, fcch ind %d, ")
TRACE_MSG(CALIBRATION_8070_112_2_17_23_53_31_106,"fastcali FastCali_Initialize")
TRACE_MSG(CALIBRATION_8164_112_2_17_23_53_31_107,"fastcali SchRxTriggerDelay fn %d, delay %d")
TRACE_MSG(CELLSYNCINFO_612_112_2_17_23_53_33_108,"L1 init %d, cell_id %d,new arfcn_index %x;old %x,%x ")
TRACE_MSG(CELLSYNCINFO_1108_112_2_17_23_53_34_109,"L1 cell_id invalid %x")
TRACE_MSG(CELLSYNCINFO_1122_112_2_17_23_53_34_110,"L1 Get_RR_S State cell_index %d")
TRACE_MSG(CELLSYNCINFO_1342_112_2_17_23_53_34_111,"L1 vice card two strong num:%d when receiving vice card paging")
TRACE_MSG(CELLSYNCINFO_1375_112_2_17_23_53_34_112,"L1,vice card two strong in main card six strong\n")
TRACE_MSG(CELLSYNCINFO_1380_112_2_17_23_53_34_113,"L1,vice card one strong in main card six strong\n")
TRACE_MSG(CELLSYNCINFO_1387_112_2_17_23_53_34_114,"L1,vice card two strong not in main card six strong\n")
TRACE_MSG(CELLSYNCINFO_1754_112_2_17_23_53_35_115,"L1: new ta %d")
TRACE_MSG(CELLSYNCINFO_1771_112_2_17_23_53_35_116,"L1:Handover SYNC cell old_ta = %d,new_ta %d,otd %d")
TRACE_MSG(CELLSYNCINFO_1781_112_2_17_23_53_35_117,"L1:Handover PSEUDO old ta %d,otd %d,rtd %d,new_ta %d")
TRACE_MSG(CELLSYNCINFO_1800_112_2_17_23_53_35_118,"L1:Handover PSEUDO cell old_ta = %d,new_ta %d,otd %d")
TRACE_MSG(CELLSYNCINFO_1938_112_2_17_23_53_35_119,"L1 camp to same!")
TRACE_MSG(CELLSYNCINFO_2603_112_2_17_23_53_37_120,"L1 bcch select %x")
TRACE_MSG(CELLSYNCINFO_2902_112_2_17_23_53_37_121,"L1: CellSyncInfo_GetNextCBCHBlkDelay schedule %ld, %d")
TRACE_MSG(CELLSYNCINFO_2923_112_2_17_23_53_37_122,"L1:CellSyncInfo_GetNextCBCHBlkDelay %d")
TRACE_MSG(CELLSYNCINFO_2935_112_2_17_23_53_37_123,"L1: MPHHandle_IsDiscardCBCHBlock %d")
TRACE_MSG(CELLSYNCINFO_3006_112_2_17_23_53_38_124,"L1:Handover fail, cell_id = %d,bsic %d,target bsic %d")
TRACE_MSG(CELLSYNCINFO_3047_112_2_17_23_53_38_125,"L1:handover cell %d,arfcn %d otd %d,scell %d,arfcn %d,otd %d, ba_index %d")
TRACE_MSG(CELLSYNCINFO_3059_112_2_17_23_53_38_126,"L1 no space to add a new arfcn to ba!")
TRACE_MSG(CELLSYNCINFO_3604_112_2_17_23_53_39_127,"L1: 1:%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,nL1: 2:%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d,%x/%x/%d")
TRACE_MSG(CELLSYNCINFO_3681_112_2_17_23_53_39_128,"L1 start decode PSI,need decode %d")
TRACE_MSG(CELLSYNCINFO_3791_112_2_17_23_53_39_129,"L1_TD QQQ CellSyncInfo_UpdateCellDeltaFn: g_td_cell_result.sync_td.delta_position %d, old_current_fn 0x%x, fn_count %d")
TRACE_MSG(CELLSYNCINFO_4905_112_2_17_23_53_41_130,"L1 get conflict %d,cellid %d")
TRACE_MSG(CELLSYNCINFO_4939_112_2_17_23_53_41_131,"L1 LastTBFPollingImpFlag fn %d, delay %d")
TRACE_MSG(CELLSYNCINFO_4966_112_2_17_23_53_41_132,"L1 conflict cell id %d, conflict %d")
TRACE_MSG(CELLSYNCINFO_4971_112_2_17_23_53_41_133,"L1 invalid cell id %d")
TRACE_MSG(CELLSYNCINFO_5032_112_2_17_23_53_42_134,"L1 sacch counter: %d, reset!")
TRACE_MSG(CELLSYNCINFO_5292_112_2_17_23_53_42_135,"L1: invalid tc")
TRACE_MSG(CELLSYNCINFO_5549_112_2_17_23_53_43_136,"L1 invlaid cell id %d")
TRACE_MSG(CELLSYNCINFO_5586_112_2_17_23_53_43_137,"L1 save bcch tc %d,%x,fn %x")
TRACE_MSG(CELLSYNCINFO_5644_112_2_17_23_53_43_138,"L1 invalid cell_id %d")
TRACE_MSG(CELLSYNCINFO_5699_112_2_17_23_53_43_139,"L1 s_b %x, tc %d")
TRACE_MSG(CELLSYNCINFO_5704_112_2_17_23_53_43_140,"L1 bcch has been invalid %d %d,%x")
TRACE_MSG(CELLSYNCINFO_5711_112_2_17_23_53_43_141,"L1 invalid cell_id %d")
TRACE_MSG(CELLSYNCINFO_5755_112_2_17_23_53_43_142,"L1 b_t  send %d, %d")
TRACE_MSG(CELLSYNCINFO_5815_112_2_17_23_53_43_143,"L1 b_t  set %d, %d")
TRACE_MSG(CELLSYNCINFO_5819_112_2_17_23_53_43_144,"L1 bcch_buf_index_queue full!")
TRACE_MSG(CELLSYNCINFO_5852_112_2_17_23_53_43_145,"L1 b_t set")
TRACE_MSG(CELLSYNCINFO_5857_112_2_17_23_53_43_146,"L1 b_t creat fail")
TRACE_MSG(CELLSYNCINFO_5873_112_2_17_23_53_43_147,"L1 b_t a")
TRACE_MSG(CELLSYNCINFO_5880_112_2_17_23_53_43_148,"L1 b_t  a f")
TRACE_MSG(CELLSYNCINFO_5895_112_2_17_23_53_43_149,"L1 b_t dea")
TRACE_MSG(CELLSYNCINFO_5900_112_2_17_23_53_43_150,"L1 b_t dea")
TRACE_MSG(CELLSYNCINFO_5905_112_2_17_23_53_43_151,"L1 b_t de f")
TRACE_MSG(CELLSYNCINFO_5963_112_2_17_23_53_43_152,"L1: vice card two strongest arfcn is in main card six strongest")
TRACE_MSG(CELLSYNCINFO_5971_112_2_17_23_53_43_153,"L1: arfcn_index 0x%x is minor card's one strong")
TRACE_MSG(CELLSYNCINFO_5984_112_2_17_23_53_43_154,"L1: arfcn_index 0x%x is minor card's second strong.")
TRACE_MSG(CELLSYNCINFO_6177_112_2_17_23_53_44_155,"L1:main_card_id: %d, select_card_id: %d,active_card_num:%d,card_bitmap:0x%x,pch_delay:%d")
TRACE_MSG(CELLSYNCINFO_6253_112_2_17_23_53_44_156,"L1 scell of card %d exist!%d")
TRACE_MSG(CELLSYNCINFO_6297_112_2_17_23_53_44_157,"L1 pch p_p co %x, se %x")
TRACE_MSG(CELLSYNCINFO_6314_112_2_17_23_53_44_158,"L1 pch p_b co %x")
TRACE_MSG(CELLSYNCINFO_6512_112_2_17_23_53_44_159,"L1 invalid fn %x,%x")
TRACE_MSG(CELLSYNCINFO_6523_112_2_17_23_53_45_160,"L1 invalid fn %x")
TRACE_MSG(CELLSYNCINFO_6534_112_2_17_23_53_45_161,"L1 invalid fn %x")
TRACE_MSG(CELLSYNCINFO_6550_112_2_17_23_53_45_162,"L1 fn adjust detected!")
TRACE_MSG(CMDQ_145_112_2_17_23_53_45_163,"L1 deactive all")
TRACE_MSG(CMDQ_342_112_2_17_23_53_46_164,"Layer1_cmd, audio in %d, audio out %d, is_input_enable %d")
TRACE_MSG(CMDQ_353_112_2_17_23_53_46_165,"arm control audio , cancel audio enable to dsp!")
TRACE_MSG(CMDQ_399_112_2_17_23_53_46_166,"layer1_cmd: command audio enable/disable")
TRACE_MSG(CMDQ_416_112_2_17_23_53_46_167,"L1:sent cmd AUDQ_SET_AUDIO_ENABLE input,%d,output%d")
TRACE_MSG(CMDQ_444_112_2_17_23_53_46_168,"AUD_NEW: CmdQ_SetCodecEnable enc_en %d, dec_en %d")
TRACE_MSG(CMDQ_531_112_2_17_23_53_46_169,"AUD_NEW: CmdQ_SetVolume output_volume %d")
TRACE_MSG(CMDQ_612_112_2_17_23_53_46_170,"AUD_NEW: CmdQ_SetMuteEnable is_mute_enable %d, mute_type %d")
TRACE_MSG(CMDQ_783_112_2_17_23_53_46_171,"AUD_NEW: CmdQ_SpecialToneGeneration tone_id %d, is_needing_to_encode %d")
TRACE_MSG(CMDQ_835_112_2_17_23_53_47_172,"AUD_NEW: CmdQ_GenericToneGeneration frequency1 %d, frequency2 %d, frequency3 %d")
TRACE_MSG(CMDQ_875_112_2_17_23_53_47_173,"AUD_NEW: CmdQ_StopTone")
TRACE_MSG(CMDQ_1205_112_2_17_23_53_47_174,"L1:SEND CMDQ_WAKEUP_DSP!\n")
TRACE_MSG(CMDQ_1284_112_2_17_23_53_47_175,"AUD_NEW: CmdQ_AudioLoop bEnableLoop %d")
TRACE_MSG(CMDQ_1410_112_2_17_23_53_48_176,"AUD_NEW: CmdQ_EnableVoiceLoopback is_enable %d,%d,%d,%d")
TRACE_MSG(CMDQ_1443_112_2_17_23_53_48_177,"AUD_NEW: CmdQ_EnableDownlinkMute is_enable %d, dlnk_mute_type %d")
TRACE_MSG(CMDQ_1477_112_2_17_23_53_48_178,"AUD_NEW: CmdQ_SetEchoTestEnable b_is_start_echo_testing %d, freq %d")
TRACE_MSG(CMDQ_1514_112_2_17_23_53_48_179,"L1: CmdQ_StartVoiceUpLoad, p0 0x%x, p1 0x%x, p2 0x%x, p3 0x%x")
TRACE_MSG(CMDQ_1516_112_2_17_23_53_48_180,"AUD_NEW: CmdQ_StartVoiceUpLoad is_from_mic %d, is_from_net %d, voice_format %d, upload_amr_mode %d")
TRACE_MSG(CMDQ_1553_112_2_17_23_53_48_181,"L1:CmdQ_StartVoiceDownLoad")
TRACE_MSG(CMDQ_1555_112_2_17_23_53_48_182,"AUD_NEW: CmdQ_StartVoiceDownLoad is_from_mic %d, is_to_net %d, voice_format %d")
TRACE_MSG(CMDQ_1587_112_2_17_23_53_48_183,"L1:CmdQ_NotifyPchReorgMode")
TRACE_MSG(CMDQ_1825_112_2_17_23_53_49_184,"L1:CMDQ_Dedicate_Establish: 0x%x, is_dsp_addr_sent %d")
TRACE_MSG(CMDQ_1841_112_2_17_23_53_49_185,"AUD_NEW: CmdQ_TransNewAudioPara length %d")
TRACE_MSG(CMDQ_1917_112_2_17_23_53_49_186,"L1:CMDQ_RXADC_DCVOLTAGE")
TRACE_MSG(CMDQ_1932_112_2_17_23_53_49_187,"L1: cmd %x,%x")
TRACE_MSG(CMDQ_1952_112_2_17_23_53_49_188,"L1: L1 Assert Dsp! fn 0x%x")
TRACE_MSG(CMDQ_1964_112_2_17_23_53_49_189,"L1: cmd %x,%x")
TRACE_MSG(CMDQ_1999_112_2_17_23_53_49_190,"L1Q:CmdQ_ChangeMode: 0x%x, mode_change_flage %x, card_id %d")
TRACE_MSG(CMDQ_2041_112_2_17_23_53_49_191,"L1Q:CMDQ_CHANGEMODE_BACK! %x")
TRACE_MSG(CMDQ_2073_112_2_17_23_53_49_192,"L1: Not Support CmdQ_StartPCMUpLoad in Old AUDIO")
TRACE_MSG(CMDQ_2100_112_2_17_23_53_49_193,"L1: Not Support CmdQ_StartPCMDownLoad in Old AUDIO")
TRACE_MSG(CMDQ_2123_112_2_17_23_53_49_194,"L1:CMDQ_IQ_CATCH")
TRACE_MSG(CMDQ_2180_112_2_17_23_53_49_195,"L1:CmdQ_StartMixVoiceFun flag %d,type %d,uplink_gain %d,downlink_gain %d")
TRACE_MSG(DATACNFPROC_125_112_2_17_23_53_50_196,"L1: SendRachCNF!")
TRACE_MSG(DATACNFPROC_228_112_2_17_23_53_50_197,"PRACH: send success, fn = %d!")
TRACE_MSG(DATACNFPROC_260_112_2_17_23_53_50_198,"L1: Handover_ACCESS_CNF!")
TRACE_MSG(DATACNFPROC_309_112_2_17_23_53_50_199,"L1: MPH_ERROR_IND!")
TRACE_MSG(DATACNFPROC_323_112_2_17_23_53_50_200,"L1: MPH_RXPOWER_SWEEP_CNF!")
TRACE_MSG(DATACNFPROC_428_112_2_17_23_53_50_201,"L1:FreqChangeCnf!")
TRACE_MSG(DATACNFPROC_598_112_2_17_23_53_51_202,"L1:DataCnfProc_MphSmsCbchDataInd normal message! %lx, %lx,%lx, %lx")
TRACE_MSG(DATACNFPROC_604_112_2_17_23_53_51_203,"L1:DataCnfProc_MphSmsCbchDataInd null message")
TRACE_MSG(DATACNFPROC_682_112_2_17_23_53_51_204,"L1: MPH_CHAN_ASSIGN_FAIL_CNF!")
TRACE_MSG(DATACNFPROC_919_112_2_17_23_53_51_205,"L1: MPH_NC_MEAS_REPORT_IND")
TRACE_MSG(DATACNFPROC_943_112_2_17_23_53_52_206,"L1: MPH_CIPHER_MODE_CNF!")
TRACE_MSG(DATACNFPROC_961_112_2_17_23_53_52_207,"L1 set task id %x DataCnfProc_SetTaskIdForReadDspMemory")
TRACE_MSG(DATACNFPROC_985_112_2_17_23_53_52_208,"L1: dsp memory's response,value address 0x%x value %d")
TRACE_MSG(DATACNFPROC_989_112_2_17_23_53_52_209,"L1: dsp memory's response,value address 0x%x value %d")
TRACE_MSG(DATACNFPROC_993_112_2_17_23_53_52_210,"L1: dsp memory's response,value address 0x%x value %d")
TRACE_MSG(DATACNFPROC_1001_112_2_17_23_53_52_211,"L1:dsp memory's response,value %d")
TRACE_MSG(DATACNFPROC_1015_112_2_17_23_53_52_212,"L1:dsp memory's response,value Address %d")
TRACE_MSG(DATACNFPROC_1036_112_2_17_23_53_52_213,"L1:layer1 should read DSP side memory!")
TRACE_MSG(DATACNFPROC_1185_112_2_17_23_53_52_214,"L1: MPH_CHANNEL_MODE_CNF!")
TRACE_MSG(DATACNFPROC_1223_112_2_17_23_53_52_215,"L1: non sync otd %d,%d,otd %d")
TRACE_MSG(DATACNFPROC_1240_112_2_17_23_53_52_216,"L1: MPH_HANDOVER_CNF!OTD = %d, old_TA = %d, MTD = %d")
TRACE_MSG(DATACNFPROC_1258_112_2_17_23_53_52_217,"L1: MPH_HANDOVER_FAIL_CNF!")
TRACE_MSG(DATACNFPROC_1277_112_2_17_23_53_52_218,"L1:PH_SCT_TCH_LOOP_ACK!")
TRACE_MSG(DATACNFPROC_1321_112_2_17_23_53_52_219,"L1:DataCnfProc_L1MnGetAFCCnf, AFC_Value = %d!")
TRACE_MSG(DATACNFPROC_1373_112_2_17_23_53_52_220,"L1:DataCnfProc_L1MnGetAFCCnf, PA_Value = %d")
TRACE_MSG(DATACNFPROC_1404_112_2_17_23_53_52_221,"L1:DataCnfProc_L1MnGetAGCCnf,AGC_Value = %d!")
TRACE_MSG(DATACNFPROC_1434_112_2_17_23_53_52_222,"L1:L1MnGetBlockErrorRatioCnf,total count= %d error count = %d!")
TRACE_MSG(DATACNFPROC_1454_112_2_17_23_53_53_223,"L1:DspVoiceLoopbackInd! Index = %d\n")
TRACE_MSG(DATACNFPROC_1550_112_2_17_23_53_53_224,"L1:TDMeasResult sync is wrong, %d, %d")
TRACE_MSG(DATACNFPROC_1566_112_2_17_23_53_53_225,"L1_TD: don't change to TD")
TRACE_MSG(DATACNFPROC_1610_112_2_17_23_53_53_226,"L1:TDMeasResult cellparam 0x%x rscp 0x%x")
TRACE_MSG(DATACNFPROC_1626_112_2_17_23_53_53_227,"L1_TD: td ncell meas rscp add 0x20")
TRACE_MSG(DATACNFPROC_1669_112_2_17_23_53_53_228,"L1_TD: td ncell meas rscp add 0x20")
TRACE_MSG(DATACNFPROC_1689_112_2_17_23_53_53_229,"L1_TD: td ncell meas rscp add 0x20")
TRACE_MSG(DATAPROC_133_112_2_17_23_53_53_230,"L1:Rx SA!,%d")
TRACE_MSG(DATAPROC_138_112_2_17_23_53_53_231,"L1:Rx bad SA counter = %d")
TRACE_MSG(DATAPROC_191_112_2_17_23_53_54_232,"L1:Rx bad FA,0x%x,0x%x")
TRACE_MSG(DATAPROC_222_112_2_17_23_53_54_233,"bad half rate FA,%3x,%3x")
TRACE_MSG(DATAPROC_324_112_2_17_23_53_54_234,"L1: AMR startreqacktimer fn 0x%x")
TRACE_MSG(DATAPROC_557_112_2_17_23_53_54_235,"L1:DataProc_RxRatscch: ACK_ERR or ACK_UNKNOW, type = 0x%x!")
TRACE_MSG(DATAPROC_564_112_2_17_23_53_54_236,"L1:DataProc_RxRatscch: Ignore RATSCCH in REQ_ACK periord, s_active_req = %d!")
TRACE_MSG(DATAPROC_608_112_2_17_23_53_54_237,"L1:bad RATSCCH,%3x,%3x")
TRACE_MSG(DATAPROC_639_112_2_17_23_53_54_238,"L1: bad SD,0x%x,0x%x")
TRACE_MSG(DATAPROC_749_112_2_17_23_53_55_239,"L1 invalid ref_scell_id %x main_card_id %x")
TRACE_MSG(DATAPROC_803_112_2_17_23_53_55_240,"L1:ref_scell_id %x, main_card_id %x")
TRACE_MSG(DATAPROC_824_112_2_17_23_53_55_241,"L1:bsic suc %x, id %x,%x")
TRACE_MSG(DATAPROC_875_112_2_17_23_53_55_242,"L1 search invalid %x")
TRACE_MSG(DATAPROC_889_112_2_17_23_53_55_243,"L1: A change of bsic is detected!, old bsic %d, new bsic %d")
TRACE_MSG(DATAPROC_920_112_2_17_23_53_55_244,"L1 search invalid %x")
TRACE_MSG(DATAPROC_933_112_2_17_23_53_55_245,"L1: A change of bsic is detected!, old bsic %d, new bsic %d")
TRACE_MSG(DATAPROC_961_112_2_17_23_53_55_246,"L1:This cell maybe been replaced! Cell Id %d, arfcn %d!")
TRACE_MSG(DATAPROC_973_112_2_17_23_53_55_247,"L1:Note got sch but this cell's state is changed to NULL cell!")
TRACE_MSG(DATAPROC_1035_112_2_17_23_53_55_248,"L1:read tch suc: %x,cell %x,arfcn %x,otd %x,%x, ncc %x")
TRACE_MSG(DATAPROC_1079_112_2_17_23_53_55_249,"L1 search invalid %x")
TRACE_MSG(DATAPROC_1142_112_2_17_23_53_55_250,"L1 search invalid %x")
TRACE_MSG(DATAPROC_1178_112_2_17_23_53_56_251,"L1: A change of bsic is detected!, old bsic %d, new bsic %d")
TRACE_MSG(DATAPROC_1208_112_2_17_23_53_56_252,"This cell maybe have been substitut! Cell Id %d, arfcn %d!")
TRACE_MSG(DATAPROC_1215_112_2_17_23_53_56_253,"L1: Tch BSIC read success! Cell %d Arfcn = %d ncc = %d\r ")
TRACE_MSG(DATAPROC_1222_112_2_17_23_53_56_254,"L1: got a sch but this cell's state is changed to NULL cell!")
TRACE_MSG(DATAPROC_1234_112_2_17_23_53_56_255,"L1 handover cell search suc !")
TRACE_MSG(DATAPROC_1267_112_2_17_23_53_56_256,"L1 bcch is not required by this cell %d, map %x,%x,%x")
TRACE_MSG(DATAPROC_1283_112_2_17_23_53_56_257,"L1:bad BCCH! Fn = 0x%lx, arfcn_index = %d!")
TRACE_MSG(DATAPROC_1359_112_2_17_23_53_56_258,"L1 bcch is not required by this cell %d, map %x,%x,%x")
TRACE_MSG(DATAPROC_1369_112_2_17_23_53_56_259,"L1:bad BCCH! %x,%x, fn = %d, arfcn_index = %d!")
TRACE_MSG(DATAPROC_1449_112_2_17_23_53_56_260,"L1 bcch is not required by this cell %d, map %x,%x,%x")
TRACE_MSG(DATAPROC_1469_112_2_17_23_53_56_261,"L1:bad BCCH! %x,%x, fn = 0x%x,arfcn_index = 0x%x!")
TRACE_MSG(DATAPROC_1528_112_2_17_23_53_56_262,"L1: Discard a agch block! due to error cell_index! scell_id = %d, error_cell_id = %d ")
TRACE_MSG(DATAPROC_1577_112_2_17_23_53_56_263,"L1:bad AGCH! Fn = %lx, dsc %d")
TRACE_MSG(DATAPROC_1636_112_2_17_23_53_56_264,"L1: maintain DSC on pg_reorg with SCell changed")
TRACE_MSG(DATAPROC_1654_112_2_17_23_53_56_265,"L1: maintain DSC on pg_reorg")
TRACE_MSG(DATAPROC_1854_112_2_17_23_53_57_266,"L1:Wrong main pch frame number=0x%x")
TRACE_MSG(DATAPROC_1859_112_2_17_23_53_57_267,"L1 page_type invalid! %x, %x")
TRACE_MSG(DATAPROC_1870_112_2_17_23_53_57_268,"L1 the card is deactived %x")
TRACE_MSG(DATAPROC_1931_112_2_17_23_53_57_269,"L1 bad pch 0x%x,dsc %d, %x, %x")
TRACE_MSG(DATAPROC_1977_112_2_17_23_53_57_270,"L1 pch dummy  0x%x,fn: 0x%x,card mask %x, tk %x")
TRACE_MSG(DATAPROC_2381_112_2_17_23_53_58_271,"L1: AMR getreqackdeltaspeechframe, new_fn 0x%x, old_fn 0x%x")
TRACE_MSG(DATAPROC_2386_112_2_17_23_53_58_272,"L1: AMR fn small")
TRACE_MSG(DATAPROC_2411_112_2_17_23_53_58_273,"GetREQACTDelta: fn_26=%d, fn_sp=%d, old_26=%d, old_sp=%d!")
TRACE_MSG(DATAPROC_2414_112_2_17_23_53_58_274,"GetREQACTDelta: old_fn=0x%x, delta=%d!")
TRACE_MSG(DATAPROC_2432_112_2_17_23_53_58_275,"GetACKACTDelta: fn=0x%x!")
TRACE_MSG(DATAPROC_2438_112_2_17_23_53_58_276,"L1: AMR getackackdeltaspeechframe, new_fn 0x%x, old_fn 0x%x")
TRACE_MSG(DATAPROC_2443_112_2_17_23_53_58_277,"L1: AMR fn small")
TRACE_MSG(DATAPROC_2468_112_2_17_23_53_58_278,"GetACKACTDelta: fn_26=%d, fn_sp=%d, old_26=%d, old_sp=%d!")
TRACE_MSG(DATAPROC_2471_112_2_17_23_53_58_279,"GetACKACTDelta: old_fn=0x%x, delta=%d!")
TRACE_MSG(DATAPROC_2584_112_2_17_23_53_58_280,"DataProc_TxFacch: applay new cmip = %d, fn = 0x%x!")
TRACE_MSG(DATAPROC_2618_112_2_17_23_53_59_281,"DataProc_TxFacch: DL, icm=%d, acs=0x%x, fn = 0x%x!")
TRACE_MSG(DATAPROC_2641_112_2_17_23_53_59_282,"DataProc_TxFacch: UL, icm=%d, acs=0x%x, fn=0x%x!")
TRACE_MSG(DATAPROC_2672_112_2_17_23_53_59_283,"DataProc_TxFacch: threshreq, one=%d,two=%d,three=%d, fn = 0x%x!")
TRACE_MSG(DATAPROC_2759_112_2_17_23_53_59_284,"DataProc_TxFacch: s_ratscch_ack_type=%d, fn=0x%x!")
TRACE_MSG(DATAPROC_2772_112_2_17_23_53_59_285,"DataProc_TxFacch ratmark: s_ratscch_ack_type=%d, fn=0x%x!")
TRACE_MSG(DATAPROC_2798_112_2_17_23_53_59_286,"DataProc_TxFacch ratdata: s_ratscch_ack_type=%d, fn=0x%x!")
TRACE_MSG(DATAPROC_2936_112_2_17_23_53_59_287,"L1:channel change! %d")
TRACE_MSG(DATAPROC_3046_112_2_17_23_53_59_288,"TBF Channel Change: exe_band = %d, new_band = %d, pwr_idx = %d!")
TRACE_MSG(DATAPROC_3122_112_2_17_23_54_0_289,"L1: bad PDTCH data!")
TRACE_MSG(DATAPROC_3180_112_2_17_23_54_0_290,"L1:fn 0x%x,dump unneed slots %d,slot mask:0x%x")
TRACE_MSG(DATAPROC_3281_112_2_17_23_54_0_291,"L1: same rx slot, fn %x,")
TRACE_MSG(DATAPROC_3298_112_2_17_23_54_0_292,"L1:bad block fn 0x%x")
TRACE_MSG(DATAPROC_3307_112_2_17_23_54_0_293,"L1 good num %d, bad num %d, bad rate : %d")
TRACE_MSG(DATAPROC_3340_112_2_17_23_54_0_294,"L1: ta index %d,msg 0x%x")
TRACE_MSG(DATAPROC_3370_112_2_17_23_54_0_295,"L1: ptcch fn %x: ta = %d")
TRACE_MSG(DATAPROC_3384_112_2_17_23_54_0_296,"L1: Invalid ptcch value = %d!")
TRACE_MSG(DATAPROC_3390_112_2_17_23_54_0_297,"L1: bad ptcch!")
TRACE_MSG(DATAPROC_3421_112_2_17_23_54_0_298,"L1: bad pbcch! ")
TRACE_MSG(DATAPROC_3444_112_2_17_23_54_0_299,"L1: A WRONG bcch_dec_state.dec_mode!")
TRACE_MSG(DATAPROC_3476_112_2_17_23_54_0_300,"L1: bad pbcch! ")
TRACE_MSG(DATAPROC_3499_112_2_17_23_54_0_301,"L1:psi decode num %d")
TRACE_MSG(DATAPROC_3506_112_2_17_23_54_0_302,"L1:bad pbcch!")
TRACE_MSG(DATAPROC_3542_112_2_17_23_54_0_303,"L1:bad pbcch!")
TRACE_MSG(DATAPROC_3601_112_2_17_23_54_1_304,"L1: bad ppch")
TRACE_MSG(DATAPROC_3683_112_2_17_23_54_1_305,"L1:bad ppch")
TRACE_MSG(DATAPROC_3805_112_2_17_23_54_1_306,"L1:bad ppch")
TRACE_MSG(DATAPROC_3924_112_2_17_23_54_1_307,"L1: a bad PAGCH in PRACH_PAGCH ")
TRACE_MSG(DATAPROC_3955_112_2_17_23_54_1_308,"L1:RXCBCH!card_id:%d")
TRACE_MSG(DATAPROC_3961_112_2_17_23_54_1_309,"L1:bad CBCH!!")
TRACE_MSG(DOWNLOADPARAM_1992_112_2_17_23_54_6_310,"L1:Threshold = %d")
TRACE_MSG(DOWNLOADPARAM_2002_112_2_17_23_54_6_311,"L1:THreshold not get because audio3 T_T")
TRACE_MSG(DOWNLOADPARAM_2042_112_2_17_23_54_6_312,"L1:Encount error when read NV EQ parameters!")
TRACE_MSG(DOWNLOADPARAM_2115_112_2_17_23_54_6_313,"L1: Read NV selected band = %d")
TRACE_MSG(DOWNLOADPARAM_2125_112_2_17_23_54_6_314,"L1: Fail in NV selected band reading, set is as Dual Band")
TRACE_MSG(DOWNLOADPARAM_2140_112_2_17_23_54_6_315,"L1: Read NV selected deal flag = %d")
TRACE_MSG(DOWNLOADPARAM_2150_112_2_17_23_54_6_316,"L1: Fail in NV selected deal flag reading, set is as zeor")
TRACE_MSG(DSPINTISR_234_112_2_17_23_54_7_317,"L1: sharedmem GSM DSP size %d, L1 size %d")
TRACE_MSG(DSPINTISR_574_112_2_17_23_54_8_318,"L1: %d times .lstate=%d")
TRACE_MSG(DSPINTISR_653_112_2_17_23_54_8_319,"L1 invalid irq!%x")
TRACE_MSG(DSPINTISR_752_112_2_17_23_54_8_320,"DSP Interrupt:STATUS_CANCLE_TBF_SEARCH,out:%d.\n")
TRACE_MSG(DSPPROC_235_112_2_17_23_54_9_321,"L1:access burst %d,%d")
TRACE_MSG(DSPPROC_253_112_2_17_23_54_9_322,"L1:access burst %d,%d,%d")
TRACE_MSG(DSPPROC_310_112_2_17_23_54_10_323,"L1:access burst %d,%d")
TRACE_MSG(DSPPROC_326_112_2_17_23_54_10_324,"L1:access burst %d,%d,%d")
TRACE_MSG(DSPPROC_381_112_2_17_23_54_10_325,"L1:param is ready!!")
TRACE_MSG(DSPPROC_391_112_2_17_23_54_10_326,"L1:WriteInitParaToDSP!g_is_download_param_done=%d")
TRACE_MSG(DSPPROC_401_112_2_17_23_54_10_327,"L1:param is ready!")
TRACE_MSG(FAKE_RR_331_112_2_17_23_54_11_328,"fake RR: sig code %x come in ")
TRACE_MSG(FAKE_RR_396_112_2_17_23_54_11_329,"CALIBRATION: recv unknown signal ")
TRACE_MSG(FAKE_RR_460_112_2_17_23_54_11_330,"fack RR: battery req send ")
TRACE_MSG(FRAMEINSTRUTIL_579_112_2_17_23_54_13_331,"LAYER1: It's a wrong POWER SWEEP Number with 0! ")
TRACE_MSG(FRAMEINSTRUTIL_636_112_2_17_23_54_14_332,"L1 rx_type %x")
TRACE_MSG(FRAMEINSTRUTIL_1256_112_2_17_23_54_15_333,"L1:card_id:%d Plot4FrInstCBCH %ld,fncard:%ld")
TRACE_MSG(FRAMEINSTRUTIL_1493_112_2_17_23_54_15_334,"L1 rx_type %x")
TRACE_MSG(FRAMEINSTRUTIL_1518_112_2_17_23_54_15_335,"L1 pch %x, %x, %x")
TRACE_MSG(FRAMEINSTRUTIL_1615_112_2_17_23_54_16_336,"L1 tbf receive minor card paging,card_bitmap:0x%x,fn_pch:0x%x,arfcn:%d")
TRACE_MSG(FRAMEINSTRUTIL_1818_112_2_17_23_54_16_337,"L1: bsic read!arfcn_index = 0x%x,cell_index = %d")
TRACE_MSG(FRAMEINSTRUTIL_1875_112_2_17_23_54_16_338,"L1 search %x")
TRACE_MSG(FRAMEINSTRUTIL_1888_112_2_17_23_54_16_339,"L1 read conflict six sync strong cell!")
TRACE_MSG(FRAMEINSTRUTIL_1971_112_2_17_23_54_16_340,"L1 search %x")
TRACE_MSG(FRAMEINSTRUTIL_2016_112_2_17_23_54_16_341,"L1: BCCH %x, %x")
TRACE_MSG(FRAMEINSTRUTIL_2133_112_2_17_23_54_17_342,"L1:IT'S WRONG DEDICATE_MODE TYPE! ")
TRACE_MSG(FRAMEINSTRUTIL_3145_112_2_17_23_54_19_343,"L1: IT's A WRONG s_dedicate_chan_map[fn_mod].channel_type!")
TRACE_MSG(FRAMEINSTRUTIL_3207_112_2_17_23_54_19_344,"L1: IT's A WRONG s_dedicate_chan_map[fn_mod].channel_type!")
TRACE_MSG(FRAMEINSTRUTIL_3337_112_2_17_23_54_19_345,"L1 tch read: %x")
TRACE_MSG(FRAMEINSTRUTIL_3360_112_2_17_23_54_19_346,"L1 tch search: %x")
TRACE_MSG(FRAMEINSTRUTIL_4675_112_2_17_23_54_22_347,"L1: TCH bsic read!Cell id %d")
TRACE_MSG(FRAMEINSTRUTIL_4687_112_2_17_23_54_22_348,"L1: TCH searching!")
TRACE_MSG(FRAMEINSTRUTIL_4699_112_2_17_23_54_22_349,"L1: TCH start search,cell_id %d")
TRACE_MSG(FRAMEINSTRUTIL_5870_112_2_17_23_54_24_350,"L1 sdcch read %x")
TRACE_MSG(FRAMEINSTRUTIL_6704_112_2_17_23_54_26_351,"L1:FrameInstrUtil_SetTxCmdPRACH:egprs rach,TS1")
TRACE_MSG(FRAMEINSTRUTIL_6709_112_2_17_23_54_26_352,"L1:FrameInstrUtil_SetTxCmdPRACH:egprs rach,TS2")
TRACE_MSG(FRAMEINSTRUTIL_7160_112_2_17_23_54_27_353,"L1:Rx data:fn curr:%d,tbf_start_fn :%d")
TRACE_MSG(FRAMEINSTRUTIL_7196_112_2_17_23_54_27_354,"L1:TBFCon: 1, exe_band = %d, new_band = %d, pwr_idx = %d!")
TRACE_MSG(FRAMEINSTRUTIL_7257_112_2_17_23_54_27_355,"L1: uplink tbf, plot rx frame, ta %d, data %d, polling %d")
TRACE_MSG(FRAMEINSTRUTIL_7277_112_2_17_23_54_27_356,"L1:No TA,not send dl polling 1")
TRACE_MSG(FRAMEINSTRUTIL_7296_112_2_17_23_54_27_357,"L1:No TA,not send dl polling 2")
TRACE_MSG(FRAMEINSTRUTIL_7333_112_2_17_23_54_27_358,"No TA,not send both link data 1")
TRACE_MSG(FRAMEINSTRUTIL_7352_112_2_17_23_54_27_359,"L1:No TA,not send both link data 2")
TRACE_MSG(FRAMEINSTRUTIL_7434_112_2_17_23_54_27_360,"L1(polling) ready to replace TxPDTCH with plot pch frame, i:%d,bcch_delay: %d,polling_delay: %d")
TRACE_MSG(FRAMEINSTRUTIL_7470_112_2_17_23_54_27_361,"L1_TD: Plot Gprs Tx Rscp, fn 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_7672_112_2_17_23_54_28_362,"L1 ready to replace RxPDTCH with plot pch frame, i:%d,bcch_delay: %d,polling_delay: %d")
TRACE_MSG(FRAMEINSTRUTIL_7998_112_2_17_23_54_28_363,"L1(polling) ready to replace RxTxPDTCH with plot pch frame, i:%d,bcch_delay: %d,polling_delay: %d")
TRACE_MSG(FRAMEINSTRUTIL_8108_112_2_17_23_54_28_364,"L1 conflitnew rx slot 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_8859_112_2_17_23_54_30_365,"L1_TD: Plot Gprs Pttch Rscp, fn 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_8865_112_2_17_23_54_30_366,"L1_TD: Plot Gprs Pttch before sync, fn 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_9027_112_2_17_23_54_30_367,"L1,Plot Tbf Search Frame.\n")
TRACE_MSG(FRAMEINSTRUTIL_9050_112_2_17_23_54_30_368,"L1:TD SYNC ON GPRS SEARCH FRAME 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_9087_112_2_17_23_54_30_369,"L1:tbf read  = 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_9108_112_2_17_23_54_30_370,"L1:tbf search  = 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_9240_112_2_17_23_54_31_371,"L1 Search is not imp tbfPolling!")
TRACE_MSG(FRAMEINSTRUTIL_9474_112_2_17_23_54_31_372,"L1:Assert Failure,Plot4AccessBurst ")
TRACE_MSG(FRAMEINSTRUTIL_9559_112_2_17_23_54_31_373,"L1(polling) ready to replace PollingRxTx with plot pch frame, i:%d,bcch_delay: %d,polling_delay: %d")
TRACE_MSG(FRAMEINSTRUTIL_9728_112_2_17_23_54_32_374,"L1(polling) ready to replace accessburstrx with plot pch frame, i:%d,bcch_delay: %d,polling_delay: %d")
TRACE_MSG(FRAMEINSTRUTIL_10217_112_2_17_23_54_33_375,"L1:FrameInstrUtil_SetTxCmdRACH:egprs rach,TS1")
TRACE_MSG(FRAMEINSTRUTIL_10222_112_2_17_23_54_33_376,"L1:FrameInstrUtil_SetTxCmdRACH:egprs rach,TS2")
TRACE_MSG(FRAMEINSTRUTIL_10391_112_2_17_23_54_34_377,"L1_TD: Plot Idle Agc Nop, dwpts_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_10405_112_2_17_23_54_34_378,"L1: idle agc no agc freq be found! ")
TRACE_MSG(FRAMEINSTRUTIL_10437_112_2_17_23_54_34_379,"L1:Try2PlotNopForDspTimingAjusting, ccchtn %d, 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_10506_112_2_17_23_54_34_380,"L1:PlotTS0RssiBCCHCamped delay %d,  fn 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_10511_112_2_17_23_54_34_381,"L1:PlotTS0RssiBCCHCamped delay %d, ind %d, mind %d")
TRACE_MSG(FRAMEINSTRUTIL_10519_112_2_17_23_54_34_382,"L1:PlotTS0RssiBCCHCamped deltapos %d")
TRACE_MSG(FRAMEINSTRUTIL_10530_112_2_17_23_54_34_383,"L1:PlotTS0RssiBCCHCamped idle ++, new postion %d")
TRACE_MSG(FRAMEINSTRUTIL_10534_112_2_17_23_54_34_384,"L1:PlotTS0RssiBCCHCamped 1 ploted num %d")
TRACE_MSG(FRAMEINSTRUTIL_10561_112_2_17_23_54_34_385,"L1:Plot idle TS0Rssi  delta %d, arfcn 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_10576_112_2_17_23_54_34_386,"L1:PlotTS0RssiBCCHCamped 1 ploted num %d")
TRACE_MSG(FRAMEINSTRUTIL_10585_112_2_17_23_54_34_387,"L1:PlotTS0RssiBCCHCamped not finished yet , ind %d, plotnum %d")
TRACE_MSG(FRAMEINSTRUTIL_10621_112_2_17_23_54_34_388,"L1_TD: FrameInstrUtil_PlotIdleTDTS0RssiMeas, curr_fn 0x%x, pch_delay %d, td_meas_fn_num %d sync_fn 0x%x, fn_pass_mod13 %d")
TRACE_MSG(FRAMEINSTRUTIL_10633_112_2_17_23_54_34_389,"L1_TD: FrameInstrUtil_PlotIdleTDTS0RssiMeas, can't plot td, plot sleep %d frame")
TRACE_MSG(FRAMEINSTRUTIL_10639_112_2_17_23_54_34_390,"L1_TD: Plot Idle TS0 Rssi Nop, ts0_delta_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_10648_112_2_17_23_54_34_391,"L1_TD: FrameInstrUtil_PlotIdleTDTS0RssiMeas, idle_frame_no %d, ts0_delta_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_10661_112_2_17_23_54_34_392,"L1_TD: FrameInstrUtil_PlotIdleTDTS0RssiMeas, plot sleep fn 0x%x, %d frame before td meas")
TRACE_MSG(FRAMEINSTRUTIL_10672_112_2_17_23_54_34_393,"L1_TD: FrameInstrUtil_PlotIdleTDTS0RssiMeas, plot idle 0x%x after td meas, idle_frame_no %d")
TRACE_MSG(FRAMEINSTRUTIL_10717_112_2_17_23_54_35_394,"L1_TD: Plot Idle Syncdl Nop, dwpts_delta_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_10768_112_2_17_23_54_35_395,"L1_TD: Plot Idle Mid Nop, mid_delta_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_10803_112_2_17_23_54_35_396,"L1_TD: FrameInstrUtil_PlotIdleTDRscpMeas, curr_fn 0x%x, pch_delay %d, td_meas_fn_num %d sync_fn 0x%x, fn_pass_mod13 %d")
TRACE_MSG(FRAMEINSTRUTIL_10811_112_2_17_23_54_35_397,"L1_TD: g_td_cell_result.meas_td[%d].cell_num %d")
TRACE_MSG(FRAMEINSTRUTIL_10813_112_2_17_23_54_35_398,"L1_TD: FrameInstrUtil_PlotIdleTDRscpMeas, can't plot td, plot sleep %d frame")
TRACE_MSG(FRAMEINSTRUTIL_10825_112_2_17_23_54_35_399,"L1_TD: FrameInstrUtil_PlotIdleTDRscpMeas, can't plot td, plot sleep %d frame")
TRACE_MSG(FRAMEINSTRUTIL_10830_112_2_17_23_54_35_400,"L1_TD: Plot Idle Rscp Nop, midam_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_10837_112_2_17_23_54_35_401,"L1_TD: FrameInstrUtil_PlotIdleTDRscpMeas, idle_frame_no %d, midam_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_10842_112_2_17_23_54_35_402,"L1_TD: FrameInstrUtil_PlotIdleTDRscpMeas, plot sleep fn 0x%x, %d frame before td meas")
TRACE_MSG(FRAMEINSTRUTIL_10846_112_2_17_23_54_35_403,"L1_TD: Plot Idle Rscp, midam_pos %d, IDLE_TD_RSCP_MEAS_AND_SYNC")
TRACE_MSG(FRAMEINSTRUTIL_10849_112_2_17_23_54_35_404,"L1_TD: g_td_cell_result.meas_td[%d].cell_num %d")
TRACE_MSG(FRAMEINSTRUTIL_10857_112_2_17_23_54_35_405,"L1_TD: FrameInstrUtil_PlotIdleTDRscpMeas, plot idle 0x%x after td meas, idle_frame_no %d")
TRACE_MSG(FRAMEINSTRUTIL_10884_112_2_17_23_54_35_406,"L1_TD: FrameInstrUtil_PlotIdleTDRssiMeas, curr_fn 0x%x, pch_delay %d, td_meas_fn_num %d sync_fn 0x%x, fn_pass_mod13 %d")
TRACE_MSG(FRAMEINSTRUTIL_10896_112_2_17_23_54_35_407,"L1_TD: FrameInstrUtil_PlotIdleTDRssiMeas, can't plot td, plot sleep %d frame")
TRACE_MSG(FRAMEINSTRUTIL_10901_112_2_17_23_54_35_408,"L1_TD: Plot Idle Rssi Nop, dwpts_delta_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_10908_112_2_17_23_54_35_409,"L1_TD: FrameInstrUtil_PlotIdleTDRssiMeas, idle_frame_no %d, dwpts_delta_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_10913_112_2_17_23_54_35_410,"L1_TD: FrameInstrUtil_PlotIdleTDRssiMeas, plot sleep fn 0x%x, %d frame before td meas")
TRACE_MSG(FRAMEINSTRUTIL_10923_112_2_17_23_54_35_411,"L1_TD: FrameInstrUtil_PlotIdleTDRssiMeas, plot idle 0x%x after td meas, idle_frame_no %d")
TRACE_MSG(FRAMEINSTRUTIL_10967_112_2_17_23_54_35_412,"L1_TD: txpdtch g_td_gprs_meas_result_reveived is %d, status %d")
TRACE_MSG(FRAMEINSTRUTIL_10984_112_2_17_23_54_35_413,"L1_TD: dwpts_pos %d, midam_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_10991_112_2_17_23_54_35_414,"L1_TD: BetweenRxTx AfterTx, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_10997_112_2_17_23_54_35_415,"L1_TD: AfterTx, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_11024_112_2_17_23_54_35_416,"L1_TD: g_td_cell_result.meas_td[%d].cell_num %d")
TRACE_MSG(FRAMEINSTRUTIL_11038_112_2_17_23_54_35_417,"L1_TD: g_td_cell_result.meas_td[%d].cell_num %d")
TRACE_MSG(FRAMEINSTRUTIL_11076_112_2_17_23_54_35_418,"L1_TD: rxpdtch meas_result_reveived is %d, status %d")
TRACE_MSG(FRAMEINSTRUTIL_11093_112_2_17_23_54_36_419,"L1_TD: dwpts_pos %d, midam_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_11100_112_2_17_23_54_36_420,"L1_TD: BetweenRxTx, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_11110_112_2_17_23_54_36_421,"L1_TD: g_td_cell_result.meas_td[%d].cell_num %d")
TRACE_MSG(FRAMEINSTRUTIL_11124_112_2_17_23_54_36_422,"L1_TD: g_td_cell_result.meas_td[%d].cell_num %d")
TRACE_MSG(FRAMEINSTRUTIL_11176_112_2_17_23_54_36_423,"L1_TD: rxtxpdtch meas_result_reveived is %d, status %d")
TRACE_MSG(FRAMEINSTRUTIL_11193_112_2_17_23_54_36_424,"L1_TD: dwpts_pos %d, midam_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_11199_112_2_17_23_54_36_425,"L1_TD: BetweenRxTx, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_11207_112_2_17_23_54_36_426,"L1_TD: AfterTx, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_11215_112_2_17_23_54_36_427,"L1_TD: LastTwoBurst, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_11228_112_2_17_23_54_36_428,"L1_TD: g_td_cell_result.meas_td[%d].cell_num %d")
TRACE_MSG(FRAMEINSTRUTIL_11241_112_2_17_23_54_36_429,"L1_TD: g_td_cell_result.meas_td[%d].cell_num %d")
TRACE_MSG(FRAMEINSTRUTIL_11290_112_2_17_23_54_36_430,"L1_TD: FrameInstrUtil_RxPtcchPlotTdRscpMeas g_td_gprs_meas_result_reveived is %d")
TRACE_MSG(FRAMEINSTRUTIL_11291_112_2_17_23_54_36_431,"L1_TD: g_td_cell_result.meas_td[%d].cell_num %d")
TRACE_MSG(FRAMEINSTRUTIL_11310_112_2_17_23_54_36_432,"L1_TD: midam_pos %d, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_11316_112_2_17_23_54_36_433,"L1_TD: g_td_cell_result.meas_td[%d].cell_num %d")
TRACE_MSG(FRAMEINSTRUTIL_11367_112_2_17_23_54_36_434,"L1_TD: for tbf search tch meas no frame5!")
TRACE_MSG(FRAMEINSTRUTIL_11375_112_2_17_23_54_36_435,"L1_TD: Plot Gprs Idle1 Before Sync, fn 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_11413_112_2_17_23_54_37_436,"L1_TD: g_td_gprs_meas_result_reveived is %d")
TRACE_MSG(FRAMEINSTRUTIL_11414_112_2_17_23_54_37_437,"L1_TD: g_td_cell_result.meas_td[%d].cell_num %d")
TRACE_MSG(FRAMEINSTRUTIL_11432_112_2_17_23_54_37_438,"L1_TD: midam_pos %d, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_11438_112_2_17_23_54_37_439,"L1_TD: g_td_cell_result.meas_td[%d].cell_num %d")
TRACE_MSG(FRAMEINSTRUTIL_11520_112_2_17_23_54_37_440,"L1_TD: g_td_gprs_meas_result_reveived is FALSE")
TRACE_MSG(FRAMEINSTRUTIL_11535_112_2_17_23_54_37_441,"L1_TD: fn 0x%x, dwpts_pos %d, ts0_delta_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_11542_112_2_17_23_54_37_442,"L1_TD: BetweenRxTx AfterTx, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_11548_112_2_17_23_54_37_443,"L1_TD: AfterTx, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_11568_112_2_17_23_54_37_444,"L1_TD: TS0_RSSI")
TRACE_MSG(FRAMEINSTRUTIL_11613_112_2_17_23_54_37_445,"L1_TD: g_td_gprs_meas_result_reveived is FALSE")
TRACE_MSG(FRAMEINSTRUTIL_11629_112_2_17_23_54_37_446,"L1_TD: fn 0x%x, dwpts_pos %d, ts0_delta_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_11636_112_2_17_23_54_37_447,"L1_TD: BetweenRxTx, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_11650_112_2_17_23_54_37_448,"L1_TD: TS0_RSSI")
TRACE_MSG(FRAMEINSTRUTIL_11697_112_2_17_23_54_37_449,"L1_TD: g_td_gprs_meas_result_reveived is FALSE")
TRACE_MSG(FRAMEINSTRUTIL_11713_112_2_17_23_54_37_450,"L1_TD: fn 0x%x, dwpts_pos %d, ts0_delta_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_11750_112_2_17_23_54_37_451,"L1_TD: TS0_RSSI")
TRACE_MSG(FRAMEINSTRUTIL_11794_112_2_17_23_54_37_452,"L1: RxPtcchPlotTdProcess coper Locked.")
TRACE_MSG(FRAMEINSTRUTIL_11813_112_2_17_23_54_37_453,"L1_TD: ts0_delta_pos %d, midam_pos %d, dwpts_pos %d, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_11820_112_2_17_23_54_38_454,"L1_TD: Plot Pttch Rssi")
TRACE_MSG(FRAMEINSTRUTIL_11841_112_2_17_23_54_38_455,"L1_TD: Plot Pttch Ts0 Rssi")
TRACE_MSG(FRAMEINSTRUTIL_11863_112_2_17_23_54_38_456,"L1_TD: Plot Pttch Syncdl")
TRACE_MSG(FRAMEINSTRUTIL_11888_112_2_17_23_54_38_457,"L1_TD: Plot Pttch Mid")
TRACE_MSG(FRAMEINSTRUTIL_11931_112_2_17_23_54_38_458,"L1_TD: Plot Pttch Agc")
TRACE_MSG(FRAMEINSTRUTIL_11972_112_2_17_23_54_38_459,"L1:GprsIdleFramePlotTdProcess coper locked.")
TRACE_MSG(FRAMEINSTRUTIL_11994_112_2_17_23_54_38_460,"L1_TD: mid_pos %d, ts0_delta_pos %d, dwpts_pos %d, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_12001_112_2_17_23_54_38_461,"L1_TD: Plot Gprs Idle Rssi")
TRACE_MSG(FRAMEINSTRUTIL_12032_112_2_17_23_54_38_462,"L1_TD: Plot Gprs Idle Ts0 Rssi")
TRACE_MSG(FRAMEINSTRUTIL_12054_112_2_17_23_54_38_463,"L1_TD: for tbf search tch meas no frame1!")
TRACE_MSG(FRAMEINSTRUTIL_12073_112_2_17_23_54_38_464,"L1_TD: Plot Gprs Idle Syncdl, counter %d,")
TRACE_MSG(FRAMEINSTRUTIL_12086_112_2_17_23_54_38_465,"L1_TD: td_meas_ind %d")
TRACE_MSG(FRAMEINSTRUTIL_12112_112_2_17_23_54_38_466,"L1_TD: for tbf search tch meas no frame2!")
TRACE_MSG(FRAMEINSTRUTIL_12123_112_2_17_23_54_38_467,"L1_TD: Plot Gprs Idle Mid, counter %d,")
TRACE_MSG(FRAMEINSTRUTIL_12136_112_2_17_23_54_38_468,"L1_TD: td_meas_ind %d")
TRACE_MSG(FRAMEINSTRUTIL_12160_112_2_17_23_54_38_469,"L1_TD: for tbf search tch meas no frame3!")
TRACE_MSG(FRAMEINSTRUTIL_12170_112_2_17_23_54_38_470,"L1_TD: Plot Gprs Idle Agc")
TRACE_MSG(FRAMEINSTRUTIL_12211_112_2_17_23_54_38_471,"L1_TD: for tbf search tch meas no frame4!")
TRACE_MSG(FRAMEINSTRUTIL_12236_112_2_17_23_54_38_472,"L1_TD: Plot Gprs Pwr, ts0_delta_pos %d, fn 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_12265_112_2_17_23_54_38_473,"L1:PlotGprsTDSync coper locked.")
TRACE_MSG(FRAMEINSTRUTIL_12299_112_2_17_23_54_38_474,"L1_TD: Plot Gprs Sync, fn 0x%x, process_counter %d, ts0_delta_pos %d")
TRACE_MSG(FRAMEINSTRUTIL_12305_112_2_17_23_54_38_475,"L1_TD: Plot Gprs Sync, Wrong overflowed counter %d, fn 0x%x")
TRACE_MSG(FRAMEINSTRUTIL_12381_112_2_17_23_54_39_476,"L1:PlotTxPdtchTdAdjustGain coper locked.")
TRACE_MSG(FRAMEINSTRUTIL_12392_112_2_17_23_54_39_477,"L1_TD: Plot Gprs Agc BetweenRxTx AfterTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_12398_112_2_17_23_54_39_478,"L1_TD: Plot Gprs Agc AfterTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_12449_112_2_17_23_54_39_479,"L1:PlotRxPdtchTdAdjustGain coper locked")
TRACE_MSG(FRAMEINSTRUTIL_12467_112_2_17_23_54_39_480,"L1_TD: Plot Gprs Agc BetweenRxTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_12513_112_2_17_23_54_39_481,"L1:PlotRxTxPdtchTdAdjustGain coper locked")
TRACE_MSG(FRAMEINSTRUTIL_12524_112_2_17_23_54_39_482,"L1_TD: Plot Gprs Agc BetweenRxTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_12531_112_2_17_23_54_39_483,"L1_TD: Plot Gprs Agc AfterTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_12538_112_2_17_23_54_39_484,"L1_TD: Plot Gprs Agc LastTwoBurst, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d, end %d")
TRACE_MSG(FRAMEINSTRUTIL_12591_112_2_17_23_54_39_485,"L1:PlotGPRSTDSyncdlSearch coper locked.")
TRACE_MSG(FRAMEINSTRUTIL_12613_112_2_17_23_54_39_486,"L1_TD: Plot Gprs Syncdl BetweenRxTx AfterTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d")
TRACE_MSG(FRAMEINSTRUTIL_12618_112_2_17_23_54_39_487,"L1_TD: Plot Gprs Syncdl AfterTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d")
TRACE_MSG(FRAMEINSTRUTIL_12629_112_2_17_23_54_39_488,"L1_TD: Plot Gprs Syncdl BetweenRxTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d")
TRACE_MSG(FRAMEINSTRUTIL_12640_112_2_17_23_54_39_489,"L1_TD: Plot Gprs Syncdl BetweenRxTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d")
TRACE_MSG(FRAMEINSTRUTIL_12645_112_2_17_23_54_39_490,"L1_TD: Plot Gprs Syncdl AfterTx, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d")
TRACE_MSG(FRAMEINSTRUTIL_12650_112_2_17_23_54_39_491,"L1_TD: Plot Gprs Syncdl LastTwoBurst, fn 0x%x, process_counter %d, dwpts_pos %d, begin %d")
TRACE_MSG(FRAMEINSTRUTIL_12709_112_2_17_23_54_39_492,"L1: PlotGPRSTDMidConf coper Locked.")
TRACE_MSG(FRAMEINSTRUTIL_12731_112_2_17_23_54_39_493,"L1_TD: Plot Gprs Syncdl BetweenRxTx AfterTx, fn 0x%x, process_counter %d, begin %d")
TRACE_MSG(FRAMEINSTRUTIL_12736_112_2_17_23_54_39_494,"L1_TD: Plot Gprs Syncdl AfterTx, fn 0x%x, process_counter %d, begin %d")
TRACE_MSG(FRAMEINSTRUTIL_12747_112_2_17_23_54_39_495,"L1_TD: Plot Gprs Syncdl BetweenRxTx, fn 0x%x, process_counter %d, begin %d")
TRACE_MSG(FRAMEINSTRUTIL_12758_112_2_17_23_54_39_496,"L1_TD: Plot Gprs Syncdl BetweenRxTx, fn 0x%x, process_counter %d, begin %d")
TRACE_MSG(FRAMEINSTRUTIL_12763_112_2_17_23_54_39_497,"L1_TD: Plot Gprs Syncdl AfterTx, fn 0x%x, process_counter %d, begin %d")
TRACE_MSG(FRAMEINSTRUTIL_12768_112_2_17_23_54_39_498,"L1_TD: Plot Gprs Syncdl LastTwoBurst, fn 0x%x, process_counter %d, begin %d")
TRACE_MSG(GPRSFRAMEGEN_588_112_2_17_23_54_41_499,"L1:Waiting for wakeup's response!")
TRACE_MSG(GPRSFRAMEGEN_725_112_2_17_23_54_41_500,"L1: block_delay %d,  fn %x ")
TRACE_MSG(GPRSFRAMEGEN_744_112_2_17_23_54_41_501,"send polling data before assign")
TRACE_MSG(GPRSFRAMEGEN_947_112_2_17_23_54_42_502,"L1:starting time reach0: %d,%d")
TRACE_MSG(GPRSFRAMEGEN_1039_112_2_17_23_54_42_503,"single fn reach1 : %d")
TRACE_MSG(GPRSFRAMEGEN_1112_112_2_17_23_54_42_504,"single fn reach2 : %d")
TRACE_MSG(GPRSFRAMEGEN_1169_112_2_17_23_54_42_505,"L1 should receive vicecard paging :%d from NV")
TRACE_MSG(GPRSFRAMEGEN_1187_112_2_17_23_54_42_506,"L1 bcch delay %x,pch_delay %x,min_delay %x,tbf_delay %x, fn_m52 %x, polling_delay:%d,map %d")
TRACE_MSG(GPRSFRAMEGEN_1288_112_2_17_23_54_43_507,"L1 ready to plot pch frame in remnant frame before PDTCH,i:%d,delay:%d,bcch_delay:%d")
TRACE_MSG(GPRSFRAMEGEN_1341_112_2_17_23_54_43_508,"L1 invalid tbf state %d")
TRACE_MSG(GPRSSYSINFO_234_112_2_17_23_54_43_509,"L1 dl-ul collision at tbf param,tx_mask %x, primitive rx_mask %x, new rx_mask %x")
TRACE_MSG(GPRSSYSINFO_302_112_2_17_23_54_44_510,"L1 the overframes slots(tx) is cancelled! %x")
TRACE_MSG(GPRSSYSINFO_308_112_2_17_23_54_44_511,"L1 the the overframes slots(rx) is cancelled! %x")
TRACE_MSG(GPRSSYSINFO_766_112_2_17_23_54_45_512,"L1:Assert Failure,NO Allocted Block Memory.")
TRACE_MSG(GPRSSYSINFO_1272_112_2_17_23_54_46_513,"L1: polling ack and rx collision tx_slot 0x%x, rx_slot 0x%x, new rx_slot 0x%x")
TRACE_MSG(GPRSSYSINFO_1375_112_2_17_23_54_46_514,"L1API_GetTBFSlotMask: error tbf_direction = %d!")
TRACE_MSG(GPRSSYSINFO_1428_112_2_17_23_54_46_515,"L1:wrong tbf mode %d!")
TRACE_MSG(GPRSSYSINFO_1431_112_2_17_23_54_46_516,"L1: reset tai,tbf_direction %d")
TRACE_MSG(GPRSSYSINFO_1454_112_2_17_23_54_46_517,"L1 access polling send_type %d")
TRACE_MSG(GPRSSYSINFO_1464_112_2_17_23_54_46_518,"L1 normal polling, ta_valid %d")
TRACE_MSG(GPRSSYSINFO_1538_112_2_17_23_54_46_519,"L1 invalid tbf!")
TRACE_MSG(GPRSSYSINFO_1554_112_2_17_23_54_46_520,"L1: exe tbf direction error")
TRACE_MSG(GPRSSYSINFO_1772_112_2_17_23_54_47_521,"L1_TD: gprs tx_rx_slot_in_one_frame 0x%x")
TRACE_MSG(GSMFRAMEGEN_308_112_2_17_23_54_48_522,"L1 GsmFrameGen_NullState arfcn 0x%x,band 0x%x,gain 0x%x")
TRACE_MSG(GSMFRAMEGEN_555_112_2_17_23_54_48_523,"L1:pchbcch_select_type=%d,delay:%d ,%d,fn=x%x")
TRACE_MSG(GSMFRAMEGEN_559_112_2_17_23_54_48_524,"bcch %x, cell id %d")
TRACE_MSG(GSMFRAMEGEN_712_112_2_17_23_54_48_525,"L1:pchbcch_select_type=%d,delay:%d ,%d,fn=x%x")
TRACE_MSG(GSMFRAMEGEN_716_112_2_17_23_54_48_526,"L1 bcch cell id 0x%x, arfcn 0x%x")
TRACE_MSG(GSMFRAMEGEN_784_112_2_17_23_54_49_527,"L1 sub_state 0x%x, cbch_delay 0x%x,%x,%x")
TRACE_MSG(GSMFRAMEGEN_1119_112_2_17_23_54_49_528,"L1 pwr sub state %d")
TRACE_MSG(GSMFRAMEGEN_1150_112_2_17_23_54_49_529,"L1:optimize power on,enter POWERSWEEP_RESULT_PENDING")
TRACE_MSG(GSMFRAMEGEN_1319_112_2_17_23_54_50_530,"Waiting for wakeup's response!!!")
TRACE_MSG(GSMFRAMEGEN_1373_112_2_17_23_54_50_531,"Waiting for wakeup's response!")
TRACE_MSG(GSMFRAMEGEN_1478_112_2_17_23_54_50_532,"L1:Invalid BCCH_CAMP sub state!")
TRACE_MSG(GSMFRAMEGEN_1534_112_2_17_23_54_50_533,"Waiting for wakeup's response!!!")
TRACE_MSG(GSMFRAMEGEN_1591_112_2_17_23_54_50_534,"l1:PLMN CAMP, SEND ASKMORE TO SELF")
TRACE_MSG(GSMFRAMEGEN_1734_112_2_17_23_54_51_535,"Waiting for wakeup's response!!!")
TRACE_MSG(GSMFRAMEGEN_1789_112_2_17_23_54_51_536,"L1: Invalid PBCCH_CAMP sub state!")
TRACE_MSG(GSMFRAMEGEN_1928_112_2_17_23_54_51_537,"L1:Waiting for wakeup's response!")
TRACE_MSG(GSMFRAMEGEN_2139_112_2_17_23_54_51_538,"L1:Waiting for wakeup's response!")
TRACE_MSG(GSMFRAMEGEN_2263_112_2_17_23_54_52_539,"L1 adjust first send fn %x,current fn %x")
TRACE_MSG(GSMFRAMEGEN_2706_112_2_17_23_54_52_540,"L1:to cell's arfcn %d")
TRACE_MSG(GSMFRAMEGEN_2800_112_2_17_23_54_53_541,"L1:current chan matched with next chan!")
TRACE_MSG(GSMFRAMEGEN_3244_112_2_17_23_54_54_542,"L1: search handover cell 0x%x")
TRACE_MSG(GSMFRAMEGEN_3361_112_2_17_23_54_54_543,"L1:TDMeasPCH every page delay is %d, count is 0")
TRACE_MSG(GSMFRAMEGEN_3367_112_2_17_23_54_54_544,"L1:GenTDMeasEveryPCH start fn 0x%x, pchdelay 0x%x")
TRACE_MSG(GSMFRAMEGEN_3382_112_2_17_23_54_54_545,"L1:GenTDMeasEveryPCH end fn 0x%x, pchtemp 0x%x")
TRACE_MSG(GSMFRAMEGEN_3411_112_2_17_23_54_54_546,"L1:td meas pch  a 0 pchdelay hanpped.")
TRACE_MSG(GSMFRAMEGEN_3417_112_2_17_23_54_54_547,"L1_TD: pch delay %d, is_td_meas %d")
TRACE_MSG(GSMFRAMEGEN_3503_112_2_17_23_54_54_548,"L1 suppored cbch 0x%x, required 0x%x")
TRACE_MSG(GSMFRAMEGEN_3522_112_2_17_23_54_54_549,"L1 sub_state 0x%x, cbch_delay 0x%x")
TRACE_MSG(GSMFRAMEGEN_3526_112_2_17_23_54_54_550,"L1 quick pwr frame min delay %d")
TRACE_MSG(GSMFRAMEGEN_3561_112_2_17_23_54_54_551,"L1:GenTDSync FALSE,g_is_td_meas %d, g_is_td_syncd %d, g_is_td_syncdl_mid_cnf %d, g_td_is_after_bcch %d, g_is_dsp_wake_up %d, g_is_td_sync_fail %d")
TRACE_MSG(LAYER1_AUDIO_236_112_2_17_23_54_56_552,"LAYER1_PlaySpecialTone: tone_id = %d, b_uplink = %d,volume %d")
TRACE_MSG(LAYER1_AUDIO_292_112_2_17_23_54_56_553,"LAYER1_PlayGenericTone: freq1 = %d, freq2 = %d, freq3 = %d")
TRACE_MSG(LAYER1_AUDIO_348_112_2_17_23_54_56_554,"LAYER1_StopTone: TRUE")
TRACE_MSG(LAYER1_AUDIO_400_112_2_17_23_54_57_555,"LAYER1_SetVolume: AD_gain = %d")
TRACE_MSG(LAYER1_AUDIO_409_112_2_17_23_54_57_556,"L1 low: SetVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX_ID\n")
TRACE_MSG(LAYER1_AUDIO_420_112_2_17_23_54_57_557,"L1 low: TD SetVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX\n")
TRACE_MSG(LAYER1_AUDIO_455_112_2_17_23_54_57_558,"L1 low: SetVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX\n")
TRACE_MSG(LAYER1_AUDIO_470_112_2_17_23_54_57_559,"L1 low: SetVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX\n")
TRACE_MSG(LAYER1_AUDIO_492_112_2_17_23_54_57_560,"L1 low: SetVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX\n")
TRACE_MSG(LAYER1_AUDIO_507_112_2_17_23_54_57_561,"L1 low: SetVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX\n")
TRACE_MSG(LAYER1_AUDIO_535_112_2_17_23_54_57_562,"LAYER1_MuteSwitch: b_mute = %d")
TRACE_MSG(LAYER1_AUDIO_615_112_2_17_23_54_57_563,"LAYER1_OpenSoundDevice1: Mic = %d, Speaker = %d")
TRACE_MSG(LAYER1_AUDIO_646_112_2_17_23_54_57_564,"LAYER1_SwitchSoundDevice: Mic = %d, Speaker = %d, DevMode = %d")
TRACE_MSG(LAYER1_AUDIO_659_112_2_17_23_54_57_565,"L1c:td NO realize")
TRACE_MSG(LAYER1_AUDIO_736_112_2_17_23_54_57_566,"LAYER1_SwitchTestMode: is_test_mode = %d")
TRACE_MSG(LAYER1_AUDIO_774_112_2_17_23_54_57_567,"LAYER1_PlaySideTone: tone_gain = %d")
TRACE_MSG(LAYER1_AUDIO_788_112_2_17_23_54_57_568,"LAYER1_SwitchEchoCancel: is_echo_cancel_on = %d")
TRACE_MSG(LAYER1_AUDIO_878_112_2_17_23_54_57_569,"L1C:Start VT AMR data exchange!Codec:%d")
TRACE_MSG(LAYER1_AUDIO_882_112_2_17_23_54_58_570,"L1C:Ignore VT start command in GSM!")
TRACE_MSG(LAYER1_AUDIO_901_112_2_17_23_54_58_571,"L1C:Stop VT AMR data exchange!Codec:%d")
TRACE_MSG(LAYER1_AUDIO_906_112_2_17_23_54_58_572,"L1:send audio&codec stop command in GSM!")
TRACE_MSG(LAYER1_AUDIO_916_112_2_17_23_54_58_573,"LAYER1:LAYER1_StartRecord_Downlink,mode:%d")
TRACE_MSG(LAYER1_AUDIO_937_112_2_17_23_54_58_574,"LAYER1:LAYER1_StartRecord_Ad,mode:%d")
TRACE_MSG(LAYER1_AUDIO_959_112_2_17_23_54_58_575,"LAYER1:LAYER1_SendVoiceDataToDA,mode:%d")
TRACE_MSG(LAYER1_AUDIO_975_112_2_17_23_54_58_576,"LAYER1:LAYER1_SendVoiceDataToNet,mode:%d")
TRACE_MSG(LAYER1_AUDIO_998_112_2_17_23_54_58_577,"L1:cs amr record uplink call back fun not registered loopback!")
TRACE_MSG(LAYER1_AUDIO_1010_112_2_17_23_54_58_578,"L1:cs amr record downlink call back fun not registered! loopback")
TRACE_MSG(LAYER1_AUDIO_1032_112_2_17_23_54_58_579,"LAYER1_SetAudioEngineeringParameter")
TRACE_MSG(LAYER1_AUDIO_1126_112_2_17_23_54_58_580,"LAYER1_GetAudioEngineeringParameter")
TRACE_MSG(LAYER1_AUDIO_1203_112_2_17_23_54_58_581,"layer1 commanding DSP echo testing")
TRACE_MSG(LAYER1_AUDIO_1239_112_2_17_23_54_58_582,"LAYER1_SendEchoResult")
TRACE_MSG(LAYER1_AUDIO_1428_112_2_17_23_54_59_583,"L1: voice_format %d,dsp_upload_format= %x,is_from_mic=%x,is_from_net=%x,block_count %d,block_length %d arfcn %x,td_flag %d")
TRACE_MSG(LAYER1_AUDIO_1443_112_2_17_23_54_59_584,"L1:AMR_MODE = %d")
TRACE_MSG(LAYER1_AUDIO_1538_112_2_17_23_54_59_585,"LAYER1_StopRecord")
TRACE_MSG(LAYER1_AUDIO_1593_112_2_17_23_54_59_586,"\"\"# VOICE FORMAT = %x,speaker = %d, net=%d")
TRACE_MSG(LAYER1_AUDIO_1615_112_2_17_23_54_59_587,"LAYER1_WriteVoiceData")
TRACE_MSG(LAYER1_AUDIO_1688_112_2_17_23_54_59_588,"\"\"# HANDLE NODATA format==%x")
TRACE_MSG(LAYER1_AUDIO_1764_112_2_17_23_54_59_589,"\"\"DL_equalizer_coeff handset[0] = %x")
TRACE_MSG(LAYER1_AUDIO_1765_112_2_17_23_54_59_590,"\"\"UL_equalizer_coeff handfree[0] = %x")
TRACE_MSG(LAYER1_AUDIO_1766_112_2_17_23_54_59_591,"\"\"DL_equalizer_coeff handfree[0] = %x")
TRACE_MSG(LAYER1_AUDIO_1767_112_2_17_23_54_59_592,"\"\"UL_equalizer_coeff earphone[0] = %x")
TRACE_MSG(LAYER1_AUDIO_1768_112_2_17_23_54_59_593,"\"\"DL_equalizer_coeff earphone[0] = %x")
TRACE_MSG(LAYER1_AUDIO_1770_112_2_17_23_54_59_594,"\"\"UL_equalizer_coeff handset[32] = %X")
TRACE_MSG(LAYER1_AUDIO_1771_112_2_17_23_54_59_595,"\"\"DL_equalizer_coeff handset[32] = %x")
TRACE_MSG(LAYER1_AUDIO_1772_112_2_17_23_54_59_596,"\"\"UL_equalizer_coeff handfree[32] = %x")
TRACE_MSG(LAYER1_AUDIO_1773_112_2_17_23_54_59_597,"\"\"DL_equalizer_coeff handfree[32] = %x")
TRACE_MSG(LAYER1_AUDIO_1774_112_2_17_23_54_59_598,"\"\"UL_equalizer_coeff earphone[32] = %x")
TRACE_MSG(LAYER1_AUDIO_1775_112_2_17_23_54_59_599,"\"\"DL_equalizer_coeff earphone[32] = %x")
TRACE_MSG(LAYER1_AUDIO_1777_112_2_17_23_54_59_600,"\"\"earphone DL_IIR1_coeff[0] = %X")
TRACE_MSG(LAYER1_AUDIO_1778_112_2_17_23_54_59_601,"\"\"handfree DL_IIR2_coeff[0] = %X")
TRACE_MSG(LAYER1_AUDIO_1780_112_2_17_23_54_59_602,"\"\"earphone DL_IIR1_coeff[5] = %X")
TRACE_MSG(LAYER1_AUDIO_1781_112_2_17_23_54_59_603,"\"\"handfree DL_IIR2_coeff[5] = %X")
TRACE_MSG(LAYER1_AUDIO_1783_112_2_17_23_54_59_604,"\"\" UL_PGA_gain = %x")
TRACE_MSG(LAYER1_AUDIO_1784_112_2_17_23_54_59_605,"\"\" DL_PGA_gain = %x")
TRACE_MSG(LAYER1_AUDIO_1785_112_2_17_23_54_59_606,"\"\" UL_digital_gain = %x")
TRACE_MSG(LAYER1_AUDIO_1786_112_2_17_23_54_59_607,"\"\" DL_digital_gain = %x")
TRACE_MSG(LAYER1_AUDIO_1787_112_2_17_23_54_59_608,"\"\" UL_digital_scale = %x")
TRACE_MSG(LAYER1_AUDIO_1788_112_2_17_23_54_59_609,"\"\" DL_digital_scale = %x")
TRACE_MSG(LAYER1_AUDIO_1789_112_2_17_23_54_59_610,"\"\" midi_PGA_gain_base = %x")
TRACE_MSG(LAYER1_AUDIO_1790_112_2_17_23_54_59_611,"\"\" digital_sidetone_gain = %x")
TRACE_MSG(LAYER1_AUDIO_1797_112_2_17_23_54_59_612,"\"\" eq_type= %X")
TRACE_MSG(LAYER1_AUDIO_1827_112_2_17_23_54_59_613,"LAYER1_SetDevModeParamEx: audio3 param_buffer_ptr->dl_da_device_l 0x%x, g_audio_length %d")
TRACE_MSG(LAYER1_AUDIO_1828_112_2_17_23_54_59_614,"LAYER1_SetDevModeParamEx: audio3 td_param_buffer_ptr->dl_da_device_l 0x%x, g_td_audio_length %d")
TRACE_MSG(LAYER1_AUDIO_1922_112_2_17_23_55_0_615,"audio3: LAYER1_SetDevModeParam API, g_mode_flag %d")
TRACE_MSG(LAYER1_AUDIO_1931_112_2_17_23_55_0_616,"audio3: 1 param_buffer_ptr 0x%x, g_audio_length %d")
TRACE_MSG(LAYER1_AUDIO_1970_112_2_17_23_55_0_617,"audio3: 2 param_buffer_ptr 0x%x, g_audio_length %d")
TRACE_MSG(LAYER1_AUDIO_1972_112_2_17_23_55_0_618,"LAYER1_SetDevModeParam,length=%d")
TRACE_MSG(LAYER1_AUDIO_1990_112_2_17_23_55_0_619,"L1 low: SetVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX_ID\n")
TRACE_MSG(LAYER1_AUDIO_2000_112_2_17_23_55_0_620,"L1 low: TD SetDevtVolume speak_vol = %d > DSP_VOL_SPEAKER_MAX\n")
TRACE_MSG(LAYER1_AUDIO_2046_112_2_17_23_55_0_621,"L1 low: SetVolume speak_vol = %d ")
TRACE_MSG(LAYER1_AUDIO_2084_112_2_17_23_55_0_622,"LAYER1_SetUlnkMutel ")
TRACE_MSG(LAYER1_AUDIO_2119_112_2_17_23_55_0_623,"LAYER1_SetDlnkMute ")
TRACE_MSG(LAYER1_AUDIO_2133_112_2_17_23_55_0_624,"L1 iq catch param %d, %d, %d ,%d")
TRACE_MSG(LAYER1_AUDIO_2151_112_2_17_23_55_0_625,"L1:s_silencecallback_pfunc!")
TRACE_MSG(LAYER1_AUDIO_2156_112_2_17_23_55_0_626,"L1: s_silencecallback_pfunc is PNULL!")
TRACE_MSG(LAYER1_AUDIO_2206_112_2_17_23_55_0_627,"L1:LAYER1_SetMagicVoiceParam,%d,%d,%d,%d,%d")
TRACE_MSG(LAYER1_AUDIO_2262_112_2_17_23_55_0_628,"L1:LAYER1_StopMagicVoiceFunc,%d,%d,%d,%d,%d")
TRACE_MSG(LAYER1_AUDIO_2291_112_2_17_23_55_0_629,"L1:LAYER1_ExchangeMagicVoiceData call back fun not registered!")
TRACE_MSG(LAYER1_AUDIO_2326_112_2_17_23_55_0_630,"L1:LAYER1_SetMixVoiceParam,%d,%d,%d,%d,%d")
TRACE_MSG(LAYER1_AUDIO_2377_112_2_17_23_55_1_631,"L1:LAYER1_StopMixVoiceFunc,%d,%d,%d,%d,%d")
TRACE_MSG(LAYER1_AUDIO_2422_112_2_17_23_55_1_632,"L1:LAYER1_SetMixVoiceData call back fun not registered!")
TRACE_MSG(LAYER1_AUDIO_2466_112_2_17_23_55_1_633,"UplinkPcmDataTrans_Start flag:0x%x,speaker = %d, net=%d")
TRACE_MSG(LAYER1_AUDIO_2502_112_2_17_23_55_1_634,"LAYER1_UplinkPcmDataTrans_Stop flag:0x%x")
TRACE_MSG(LAYER1_AUDIO_2554_112_2_17_23_55_1_635,"LAYER1_DownlinkPcmDataTrans_Start flag:0x%x")
TRACE_MSG(LAYER1_AUDIO_2590_112_2_17_23_55_1_636,"LAYER1_DownlinkPcmDataTrans_Stop")
TRACE_MSG(LAYER1_AUDIO_2616_112_2_17_23_55_1_637,"L1:downlink_pcm_callback call back fun not registered!")
TRACE_MSG(LAYER1_AUDIO_2635_112_2_17_23_55_1_638,"L1:uplink_pcm_callback function not registered!")
TRACE_MSG(LAYER1_AUDIO_2640_112_2_17_23_55_1_639,"LAYER1_SetBTUploadData not Succ!!")
TRACE_MSG(LAYER1_AUDIO_CODEC_78_112_2_17_23_55_1_640,"LAYER1_AUDIO_CODEC_Data 0x%x")
TRACE_MSG(LAYER1_AUDIO_CODEC_144_112_2_17_23_55_2_641,"LAYER1_AUDIO_CODEC_CopyData to:0x%x, from:0x%x, len:0x%x.")
TRACE_MSG(LAYER1_AUDIO_CODEC_168_112_2_17_23_55_2_642,"LAYER1_AUDIO_CODEC_CopyData data count is 0")
TRACE_MSG(LAYER1_AUDIO_CODEC_185_112_2_17_23_55_2_643,"LAYER1_AUDIO_CODEC_CopyLRData l:0x%x, r:0x%x.")
TRACE_MSG(LAYER1_AUDIO_CODEC_206_112_2_17_23_55_2_644,"LAYER1_AUDIO_CODEC_CopyLRData data count is 0")
TRACE_MSG(LAYER1_DIAG_235_112_2_17_23_55_2_645,"DIAG: send signal to RR\n")
TRACE_MSG(LAYER1_DIAG_389_112_2_17_23_55_3_646,"bef fastcali isnv %d, band %d, type %d, index %d, length %d")
TRACE_MSG(LAYER1_DIAG_493_112_2_17_23_55_3_647,"L1 diag: diag_msg_ptr->index is out range of data_length!")
TRACE_MSG(LAYER1_DIAG_624_112_2_17_23_55_3_648,"layer1 diag: para_band error!")
TRACE_MSG(LAYER1_DIAG_981_112_2_17_23_55_4_649,"L1:the NV_ID_IMEI is wrong!")
TRACE_MSG(LAYER1_DIAG_1035_112_2_17_23_55_4_650,"L1:NV RF solution hasn't been read out!")
TRACE_MSG(LAYER1_DIAG_1153_112_2_17_23_55_4_651,"L1 this function is implemented only at 8800s (Layer1_HandleDspDebug())")
TRACE_MSG(LAYER1_DIAG_1231_112_2_17_23_55_4_652,"cali flag %d")
TRACE_MSG(LAYER1_DIAG_1258_112_2_17_23_55_4_653,"fastcali diag ptr1 0x%x, pc_sig_ptr 0x%x, *pc_sig_ptr 0x%x")
TRACE_MSG(LAYER1_DIAG_1280_112_2_17_23_55_4_654,"fastcali diag cmd 0x%x, count %d, p1 %d, p2 %d, p3 %d, p4 %d")
TRACE_MSG(LAYER1_FREQ_93_112_2_17_23_55_5_655,"L1: IT's a WRONG frequency arfcn! 0x%x\n")
TRACE_MSG(LAYER1_MAC_PROC_230_112_2_17_23_55_6_656,"L1:overall_bep:gmsk_mean_bep=%d,gmsk_cv_bep=%d,psk8_mean_bep=%d,psk8_cv_bep=%d,mean_bep_tn_present=%d")
TRACE_MSG(LAYER1_MAC_PROC_323_112_2_17_23_55_6_657,"L1:dump the polling,out of dsp's ability,polling_slot=%d,first rx_slot=%d")
TRACE_MSG(LAYER1_MAC_PROC_455_112_2_17_23_55_6_658,"L1 polling data is not write  %d, %d!")
TRACE_MSG(LAYER1_MAC_PROC_497_112_2_17_23_55_6_659,"LAYER1: It's Layer1Mac_GetDataAndWriteData function\n")
TRACE_MSG(LAYER1_MAC_PROC_535_112_2_17_23_55_7_660,"L1 polling data is not write to dsp %d, %d!")
TRACE_MSG(LAYER1_MAIN_2966_112_2_17_23_55_13_661,"L1_READ NV: start %d")
TRACE_MSG(LAYER1_MAIN_2985_112_2_17_23_55_13_662," td sync fail timer inteval : %d")
TRACE_MSG(LAYER1_MAIN_3022_112_2_17_23_55_13_663,"L1_TD: nv flag, test_bsic_fail %d, test_bcch_fail %d, test_ab_mode %d, test_mode_change %d")
TRACE_MSG(LAYER1_MAIN_3023_112_2_17_23_55_13_664,"L1_TD: nv flag, test_tbf_time %d, g_td_first_time_syncdl_arfcn_num %d, g_td_max_syncdl_arfcn_num %d")
TRACE_MSG(LAYER1_MAIN_3026_112_2_17_23_55_13_665,"L1_READ NV: finish %d")
TRACE_MSG(LAYER1_MAIN_3236_112_2_17_23_55_13_666,"L1_TD: yPAD_LAYER1 wait dsp ready start %d")
TRACE_MSG(LAYER1_MAIN_3238_112_2_17_23_55_13_667,"L1_TD: yPAD_LAYER1 wait dsp ready finish %d")
TRACE_MSG(LAYER1_MAIN_3242_112_2_17_23_55_14_668,"L1: enter begin to L1_Trans_GsmRf_Para")
TRACE_MSG(LAYER1_MAIN_3261_112_2_17_23_55_14_669,"QQQ: reg isr")
TRACE_MSG(LAYER1_MAIN_3276_112_2_17_23_55_14_670,"TEST: before init para %d")
TRACE_MSG(LAYER1_MAIN_3277_112_2_17_23_55_14_671,"L1_TD: yPAD_LAYER1 wait dsp ready start %d")
TRACE_MSG(LAYER1_MAIN_3279_112_2_17_23_55_14_672,"L1_TD: yPAD_LAYER1 wait dsp ready finish %d")
TRACE_MSG(LAYER1_MAIN_3280_112_2_17_23_55_14_673,"TEST: after init para %d")
TRACE_MSG(LAYER1_MAIN_3281_112_2_17_23_55_14_674,"layer1: first download dsp para")
TRACE_MSG(LAYER1_MAIN_3481_112_2_17_23_55_14_675,"L1:save signal 0x%x\n")
TRACE_MSG(LAYER1_MAIN_3493_112_2_17_23_55_14_676,"L1:discard 0x%x, sender 0x%x\n")
TRACE_MSG(LAYER1_MAIN_3580_112_2_17_23_55_14_677,"L1: Initializing Physical Layer All Tasks\n\r")
TRACE_MSG(LAYER1_MAIN_3636_112_2_17_23_55_14_678,"L1: Initializing for active\n\r")
TRACE_MSG(LAYER1_MAIN_3992_112_2_17_23_55_15_679,"L1: calibration create timer failed 0")
TRACE_MSG(LAYER1_MAIN_3997_112_2_17_23_55_15_680,"L1: calibration timer is not active, duration is equal 0")
TRACE_MSG(LAYER1_MAIN_4293_112_2_17_23_55_16_681,"L1:CheckIfBCCHCollide,invalid arfcn,old=0x%x,new=0x%x")
TRACE_MSG(LAYER1_MAIN_4302_112_2_17_23_55_16_682,"L1:CheckIfBCCHCollide: invalid cell_id,old=0x%x, new=0x%x")
TRACE_MSG(LAYER1_MAIN_4325_112_2_17_23_55_16_683,"L1:CheckIfBCCHCollide,fn_old_mod51=%d,fn_new_mod51=%d")
TRACE_MSG(LAYER1_MAIN_4344_112_2_17_23_55_16_684,"L1 set tbf flag!")
TRACE_MSG(LAYER1_MAIN_4715_112_2_17_23_55_16_685,"QQQ: reg isr")
TRACE_MSG(LAYER1_MAIN_4732_112_2_17_23_55_16_686,"layer1: download dsp para")
TRACE_MSG(LAYER1_MAIN_4733_112_2_17_23_55_16_687,"L1_TD: L1Main_DspInit wait dsp ready start %d")
TRACE_MSG(LAYER1_MAIN_4735_112_2_17_23_55_16_688,"L1_TD: L1Main_DspInit wait dsp ready finish %d")
TRACE_MSG(LAYER1_MAIN_4810_112_2_17_23_55_17_689,"after dsp download transmit tx max pwr to dsp")
TRACE_MSG(LAYER1_MAIN_4875_112_2_17_23_55_17_690,"L1_TD: 30s Resync")
TRACE_MSG(LAYER1_MAIN_4885_112_2_17_23_55_17_691,"L1_TD: wrong state %d")
TRACE_MSG(LAYER1_MAIN_4903_112_2_17_23_55_17_692,"L1_TD: no valid td arfcn, return to TD_PWR")
TRACE_MSG(LAYER1_MAIN_4922_112_2_17_23_55_17_693,"L1_TD: 30s resync goto TD_TS0_RSSI_SUBSTATE")
TRACE_MSG(LAYER1_MAIN_4941_112_2_17_23_55_17_694,"L1_TD: 30s Sync Again!!")
TRACE_MSG(LAYER1_MAIN_4975_112_2_17_23_55_17_695,"L1:SetTDResyncTimer failed, duration is too short %d")
TRACE_MSG(LAYER1_MAIN_4981_112_2_17_23_55_17_696,"L1:SetTDResyncTimer timer already is on %d")
TRACE_MSG(LAYER1_MAIN_5012_112_2_17_23_55_17_697,"L1_TD: wenjun.chen:L1Main_SetTDResyncTimer, timespan %d")
TRACE_MSG(LAYER1_MAIN_5057_112_2_17_23_55_17_698,"L1_TD: wenjun.chen:L1Main_SetTDSyncFailTimer, timespan %d")
TRACE_MSG(LAYER1_MAIN_5069_112_2_17_23_55_17_699,"L1_TD: create sync fail timer")
TRACE_MSG(LAYER1_MAIN_5076_112_2_17_23_55_17_700,"L1_TD: active sync fail timer")
TRACE_MSG(LAYER1_MAIN_5110_112_2_17_23_55_17_701,"L1_TD: delete sync fail timer")
TRACE_MSG(LAYER1_MAIN_5125_112_2_17_23_55_17_702,"L1: em_get_l1_param_timer active")
TRACE_MSG(LAYER1_MAIN_5130_112_2_17_23_55_17_703,"L1: em_get_l1_param_timer active")
TRACE_MSG(LAYER1_MAIN_5135_112_2_17_23_55_17_704,"L1: em_get_l1_param create timer failed ")
TRACE_MSG(LAYER1_MIDI_88_112_2_17_23_55_18_705,"L1 L1Midi_OutputData() became a void one")
TRACE_MSG(LAYER1_MIDI_97_112_2_17_23_55_18_706,"L1 L1Midi_Stop() became a void one")
TRACE_MSG(LAYER1_OUTPORT_193_112_2_17_23_55_18_707,"L1:Start layer1_calibration thread!\n")
TRACE_MSG(LAYER1_OUTPORT_225_112_2_17_23_55_19_708,"L1:Start layer1_normal thread!\n")
TRACE_MSG(LAYER1_OUTPORT_292_112_2_17_23_55_19_709,"L1:The last operation is not finished!")
TRACE_MSG(LAYER1_OUTPORT_298_112_2_17_23_55_19_710,"L1 set task id %x L1API_GetAFC")
TRACE_MSG(LAYER1_OUTPORT_346_112_2_17_23_55_19_711,"L1:The last operation is not finished!")
TRACE_MSG(LAYER1_OUTPORT_352_112_2_17_23_55_19_712,"L1 set task id %x L1API_GetPA")
TRACE_MSG(LAYER1_OUTPORT_387_112_2_17_23_55_19_713,"L1:The last operation is not finished!")
TRACE_MSG(LAYER1_OUTPORT_393_112_2_17_23_55_19_714,"L1 set task id %x L1API_GetAGC")
TRACE_MSG(LAYER1_OUTPORT_425_112_2_17_23_55_19_715,"L1:The last operation is not finished!")
TRACE_MSG(LAYER1_OUTPORT_430_112_2_17_23_55_19_716,"L1 set task id %x L1API_GetBlockErrorRatio")
TRACE_MSG(LAYER1_OUTPORT_553_112_2_17_23_55_19_717,"L1:Start DSP RX with max power")
TRACE_MSG(LAYER1_OUTPORT_566_112_2_17_23_55_19_718,"L1:Stop DSP RX with max power")
TRACE_MSG(LAYER1_OUTPORT_632_112_2_17_23_55_20_719,"L1:Start DSP TX with max power")
TRACE_MSG(LAYER1_OUTPORT_645_112_2_17_23_55_20_720,"L1:Stop DSP TX with max power")
TRACE_MSG(LAYER1_OUTPORT_719_112_2_17_23_55_20_721,"L1:Start DSP TX with max power")
TRACE_MSG(LAYER1_OUTPORT_732_112_2_17_23_55_20_722,"L1:Stop DSP TX with max power")
TRACE_MSG(LAYER1_OUTPORT_827_112_2_17_23_55_20_723,"L1:GPSVALIDATION audio is using!")
TRACE_MSG(LAYER1_OUTPORT_834_112_2_17_23_55_20_724,"L1:Other function code in overlay! ")
TRACE_MSG(LAYER1_OUTPORT_902_112_2_17_23_55_20_725,"L1_TD: Get Td Nv Param")
TRACE_MSG(LAYER1_OUTPORT_915_112_2_17_23_55_20_726,"L1_TD: Set Td Nv Param: test_bsic_fail %d")
TRACE_MSG(LAYER1_OUTPORT_920_112_2_17_23_55_20_727,"L1_TD: Set Td Nv Param: test_bcch_fail %d")
TRACE_MSG(LAYER1_OUTPORT_925_112_2_17_23_55_20_728,"L1_TD: Set Td Nv Param: test_ab_mode %d")
TRACE_MSG(LAYER1_OUTPORT_930_112_2_17_23_55_20_729,"L1_TD: Set Td Nv Param: test_mode_change %d")
TRACE_MSG(LAYER1_OUTPORT_935_112_2_17_23_55_20_730,"L1_TD: Set Td Nv Param: test_tbf_time %d")
TRACE_MSG(LAYER1_OUTPORT_940_112_2_17_23_55_20_731,"L1_TD: Set Td Nv Param: g_td_first_time_syncdl_arfcn_num %d")
TRACE_MSG(LAYER1_OUTPORT_953_112_2_17_23_55_20_732,"L1_TD: Set Td Nv Param: g_td_max_syncdl_arfcn_num %d")
TRACE_MSG(LAYER1_OUTPORT_966_112_2_17_23_55_20_733,"L1_TD: Set Td Nv Param: g_td_power_alpha %d")
TRACE_MSG(LAYER1_OUTPORT_975_112_2_17_23_55_20_734,"L1_TD: Set Td Nv Param: s_td_sync_fail_timer_inteval %d")
TRACE_MSG(LAYER1_OUTPORT_1004_112_2_17_23_55_20_735,"TEST TEMP: arfcn 0x%x, rscp %d")
TRACE_MSG(LAYER1_OUTPORT_1008_112_2_17_23_55_20_736,"L1_TD: Report TD NCELL ENG, num %d")
TRACE_MSG(LAYER1_OUTPORT_1020_112_2_17_23_55_20_737,"TEST TEMP: arfcn 0x%x, cell_id %d, rscp %d")
TRACE_MSG(LAYER1_OUTPORT_1024_112_2_17_23_55_20_738,"L1_TD: Report TD NCELL ENG, num %d")
TRACE_MSG(LAYER1_OUTPORT_1036_112_2_17_23_55_20_739,"L1_TD: Report TD NCELL ENG, num %d")
TRACE_MSG(LAYER1_OUTPORT_1122_112_2_17_23_55_21_740,"L1: MPHHandle_MPhSMSCBDiscardBlock wakeup dsp")
TRACE_MSG(LAYER1_OUTPORT_1170_112_2_17_23_55_21_741,"L1: MPHHandle_MPhSMSCBDiscardBlock wakeup dsp")
TRACE_MSG(LAYER1_SBC_105_112_2_17_23_55_21_742,"L1:load GPS code in overlay.!")
TRACE_MSG(LAYER1_SBC_109_112_2_17_23_55_21_743,"L1:Overlay_code_base_ptr is %x")
TRACE_MSG(LAYER1_SBC_110_112_2_17_23_55_21_744,"L1:Overlay_Load_Code %x %x %x %x %x")
TRACE_MSG(LAYER1_SBC_184_112_2_17_23_55_21_745,"L1: overlay_context.overlay_mutex %x")
TRACE_MSG(LAYER1_SBC_185_112_2_17_23_55_21_746,"L1: overlay_context.mem_state %d")
TRACE_MSG(LAYER1_SBC_186_112_2_17_23_55_21_747,"L1: overlay_context.overlay_code_base_ptr %x")
TRACE_MSG(LAYER1_SBC_218_112_2_17_23_55_21_748,"L1: L1_SBC_Config_Encoder ")
TRACE_MSG(LAYER1_SBC_233_112_2_17_23_55_21_749,"L1:SBC Function mutex: %x ")
TRACE_MSG(LAYER1_SBC_327_112_2_17_23_55_21_750,"L1: command  CmdQ_SBC_Config_Encoder ")
TRACE_MSG(LAYER1_SBC_352_112_2_17_23_55_22_751,"L1:L1_SBC_Encode_Frame %d %d %d")
TRACE_MSG(LAYER1_SBC_354_112_2_17_23_55_22_752,"L1:L1_SBC_Encode_Frame")
TRACE_MSG(LAYER1_SBC_426_112_2_17_23_55_22_753,"L1: command  CMDQ_SBC_ENCODE_FRAME ")
TRACE_MSG(LAYER1_SBC_490_112_2_17_23_55_22_754,"L1:frame_count=%d,frame_len_return_from_dsp[in 16bits]=%d,frame_len=%d")
TRACE_MSG(LAYER1_SBC_535_112_2_17_23_55_22_755,"L1: command  CMDQ_TRANPCM_CONFIG ")
TRACE_MSG(LAYER1_SBC_551_112_2_17_23_55_22_756,"L1: command  CMDQ_TRANPCM_PROCESS ")
TRACE_MSG(LAYER1_VOICECALL_68_112_2_17_23_55_22_757,"L1:Transfer a frame data to DSP !!!! ")
TRACE_MSG(MPHHANDLE_333_112_2_17_23_55_23_758,"l1 SharedMemProc_WakeupDsp!!!")
TRACE_MSG(MPHHANDLE_338_112_2_17_23_55_23_759,"l1 L1Main_ActiveDspByCmds!!!")
TRACE_MSG(MPHHANDLE_359_112_2_17_23_55_23_760,"l1 active meet response delay !!!")
TRACE_MSG(MPHHANDLE_418_112_2_17_23_55_24_761,"l1:deactive rat %d, gsm sync %d, change mode %d")
TRACE_MSG(MPHHANDLE_432_112_2_17_23_55_24_762,"L1 deactive mask %x")
TRACE_MSG(MPHHANDLE_466_112_2_17_23_55_24_763,"L1 main card id %x")
TRACE_MSG(MPHHANDLE_538_112_2_17_23_55_24_764,"l1:deactive rat %d, gsm sync %d, change mode %d")
TRACE_MSG(MPHHANDLE_606_112_2_17_23_55_24_765,"l1_td: in td mode receive classmark req!")
TRACE_MSG(MPHHANDLE_839_112_2_17_23_55_24_766,"8800h bcch fail")
TRACE_MSG(MPHHANDLE_1162_112_2_17_23_55_25_767,"L1Q: MPHHandle_MphBcchCampReqIdle")
TRACE_MSG(MPHHANDLE_1480_112_2_17_23_55_26_768,"L1 pwrswp state %d")
TRACE_MSG(MPHHANDLE_1523_112_2_17_23_55_26_769,"L1:calipost_param = 0x%x")
TRACE_MSG(MPHHANDLE_1557_112_2_17_23_55_26_770,"L1:band = %d,arfcn = 0x%x, cali mode = %x, cali param %x")
TRACE_MSG(MPHHANDLE_1636_112_2_17_23_55_26_771,"L1: bcch stop is received, arfcn %d, bsic %d")
TRACE_MSG(MPHHANDLE_1897_112_2_17_23_55_26_772,"L1Q: MPHHandle_MphBcchCampReqIdle")
TRACE_MSG(MPHHANDLE_2317_112_2_17_23_55_27_773,"L1 page mode req param3 %d")
TRACE_MSG(MPHHANDLE_2669_112_2_17_23_55_28_774,"L1,sent rf scan ind in pg_reorg.\n")
TRACE_MSG(MPHHANDLE_3708_112_2_17_23_55_30_775,"L1: start_fn %d")
TRACE_MSG(MPHHANDLE_3918_112_2_17_23_55_30_776,"L1: chan assign start fn %d")
TRACE_MSG(MPHHANDLE_4113_112_2_17_23_55_31_777,"L1:chan_mode %d, is_change %d")
TRACE_MSG(MPHHANDLE_4205_112_2_17_23_55_31_778,"L1:Cipher mode %d,Cipher_algarithm %d,n")
TRACE_MSG(MPHHANDLE_4210_112_2_17_23_55_31_779,"L1:Cipher Key %d,%d,%d,%d,%d,%d,%d,%dn")
TRACE_MSG(MPHHANDLE_4268_112_2_17_23_55_31_780,"L1: Freq Change start fn %d")
TRACE_MSG(MPHHANDLE_4338_112_2_17_23_55_31_781,"L1: chan mode %d!\n")
TRACE_MSG(MPHHANDLE_4463_112_2_17_23_55_31_782,"L1 arfcn %x not sync")
TRACE_MSG(MPHHANDLE_4472_112_2_17_23_55_31_783,"L1 init handover cell fail! ")
TRACE_MSG(MPHHANDLE_4479_112_2_17_23_55_31_784,"L1 handover cell id %d,  arfcn index %x ")
TRACE_MSG(MPHHANDLE_4517_112_2_17_23_55_32_785,"L1 handover to resync pending cell arfcn index %x")
TRACE_MSG(MPHHANDLE_4537_112_2_17_23_55_32_786,"L1:Handover fail, cell_id = %d,bsic %d,target bsic %d")
TRACE_MSG(MPHHANDLE_4594_112_2_17_23_55_32_787,"L1:cipher_mode %d,cipher_algorithm %d")
TRACE_MSG(MPHHANDLE_4920_112_2_17_23_55_32_788,"L1:set cell %d to NOT_SYN_CALL")
TRACE_MSG(MPHHANDLE_5047_112_2_17_23_55_33_789,"L1_TD: sync ho, recieve new bad sa counter, update it")
TRACE_MSG(MPHHANDLE_5137_112_2_17_23_55_33_790,"L1:No TCH avtive,ignor LOOP cmd")
TRACE_MSG(MPHHANDLE_5176_112_2_17_23_55_33_791,"L1:rec disable loop back %d!")
TRACE_MSG(MPHHANDLE_5195_112_2_17_23_55_33_792,"L1:rec loop back cmd!")
TRACE_MSG(MPHHANDLE_5399_112_2_17_23_55_33_793,"L1: MPHHandle_MphSMSCBStartReq  cbch_required_bitmap %d")
TRACE_MSG(MPHHANDLE_5529_112_2_17_23_55_34_794,"L1: MPHHandle_MPhSMSCBDiscardBlock %d")
TRACE_MSG(MPHHANDLE_5546_112_2_17_23_55_34_795,"L1: MPHHandle_MPhSMSCBDiscardBlock wakeup dsp")
TRACE_MSG(MPHHANDLE_5597_112_2_17_23_55_34_796,"L1 uiCellIndex is INVALID_CELL! %x")
TRACE_MSG(MPHHANDLE_5602_112_2_17_23_55_34_797,"L1: tc mask %d")
TRACE_MSG(MPHHANDLE_5625_112_2_17_23_55_34_798,"L1 cell state is not sync %x, %x!")
TRACE_MSG(MPHHANDLE_5713_112_2_17_23_55_34_799,"L1 scell ")
TRACE_MSG(MPHHANDLE_5725_112_2_17_23_55_34_800,"L1 valid cell! 0x%x")
TRACE_MSG(MPHHANDLE_5740_112_2_17_23_55_34_801,"L1 searching %x")
TRACE_MSG(MPHHANDLE_5802_112_2_17_23_55_34_802,"Download GSM Code")
TRACE_MSG(MPHHANDLE_5819_112_2_17_23_55_34_803,"Download GSM Code2")
TRACE_MSG(MPHHANDLE_5962_112_2_17_23_55_34_804,"L1Q: g_is_sync_mode_change %d")
TRACE_MSG(MPHHANDLE_5965_112_2_17_23_55_34_805,"L1Q: MODE CHANGE: HO_FAIL_REVERT_TD or RESEL_FAIL_REVERT_TD")
TRACE_MSG(MPHHANDLE_5970_112_2_17_23_55_34_806,"L1Q: g_is_revert_to_td %d")
TRACE_MSG(MPHHANDLE_5999_112_2_17_23_55_35_807,"L1Q: mode change gsm2td SharedMemProc_SetTDSyncPara")
TRACE_MSG(MPHHANDLE_6004_112_2_17_23_55_35_808,"L1Q: MODE CHANGE: HO_FAIL_REVERT_TD or RESEL_FAIL_REVERT_TD")
TRACE_MSG(MPHHANDLE_6011_112_2_17_23_55_35_809,"L1_TD: manuel to td before sync 3 to 2 finish")
TRACE_MSG(MPHHANDLE_6108_112_2_17_23_55_35_810,"L1:MergeTDMeasInfo, numold %d, numnew %d, issynced %d")
TRACE_MSG(MPHHANDLE_6128_112_2_17_23_55_35_811,"L1:MergeTDMeasInfo, i=%d freqold[0x%x], freqnew[0x%x]")
TRACE_MSG(MPHHANDLE_6365_112_2_17_23_55_35_812,"L1:StartTDMeasReq, td meas has been actived, a duplicated request by RRC")
TRACE_MSG(MPHHANDLE_6608_112_2_17_23_55_36_813,"L1_TD: RR stop td meas, issynced %d")
TRACE_MSG(MPHHANDLE_6697_112_2_17_23_55_36_814,"L1:StartTDMeasReqTBF, td meas has been actived, a duplicated request by RRC")
TRACE_MSG(MPHHANDLE_6847_112_2_17_23_55_36_815,"L1Q: cell reselect, g_is_sync_mode_change true")
TRACE_MSG(MPHHANDLE_6850_112_2_17_23_55_36_816,"L1Q: reselect to gsm after plot nop %d")
TRACE_MSG(MPHHANDLE_6853_112_2_17_23_55_36_817,"L1Q: cell reselect, g_is_sync_mode_change false")
TRACE_MSG(MPHHANDLE_6984_112_2_17_23_55_36_818,"L1_TD: sync mode, set default badsa counter")
TRACE_MSG(MPHHANDLE_6986_112_2_17_23_55_36_819,"L1_TD: sync mode change before SharedMemProc_GetGSMSyncPara %d")
TRACE_MSG(MPHHANDLE_6988_112_2_17_23_55_37_820,"L1_TD: sync mode change after SharedMemProc_GetGSMSyncPara %d")
TRACE_MSG(MPHHANDLE_6991_112_2_17_23_55_37_821,"L1Q: handover, g_is_sync_mode_change true")
TRACE_MSG(MPHHANDLE_6993_112_2_17_23_55_37_822,"L1_TD: sync mode change before handover para set %d")
TRACE_MSG(MPHHANDLE_6997_112_2_17_23_55_37_823,"L1 mode change : plot one idle frame [%d] before handover ")
TRACE_MSG(MPHHANDLE_7003_112_2_17_23_55_37_824,"L1_TD: sync mode change after handover para set %d")
TRACE_MSG(MPHHANDLE_7006_112_2_17_23_55_37_825,"L1Q: handover, g_is_sync_mode_change false")
TRACE_MSG(MPHHANDLE_7037_112_2_17_23_55_37_826,"L1Q: handover suc finish")
TRACE_MSG(PHHANDLE_408_112_2_17_23_55_38_827,"L1 enable codec device")
TRACE_MSG(PHHANDLE_685_112_2_17_23_55_38_828,"L1:PHHandle_AppPhMp3A2dpReq:Time = %d ms")
TRACE_MSG(PHHANDLE_854_112_2_17_23_55_38_829,"L1:Switch Band: ma_arfcn_index = 0x%x, pdch_band = %d!")
TRACE_MSG(PHHANDLE_986_112_2_17_23_55_39_830,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_1117_112_2_17_23_55_39_831,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_1170_112_2_17_23_55_39_832,"L1:TBF: switch band,pmax=%d,pdch_band=%d,bcch_band=%d")
TRACE_MSG(PHHANDLE_1260_112_2_17_23_55_39_833,"L1:Switch Band1: ma_arfcn_index = 0x%x, pdch_band=%d!")
TRACE_MSG(PHHANDLE_1377_112_2_17_23_55_40_834,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_1432_112_2_17_23_55_40_835,"L1:tbf switch band, pmax=%d, pdch_band=%d, bcch_band=%d!")
TRACE_MSG(PHHANDLE_1590_112_2_17_23_55_40_836,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_1690_112_2_17_23_55_40_837,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_1865_112_2_17_23_55_40_838,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_1971_112_2_17_23_55_41_839,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_2105_112_2_17_23_55_41_840,"L1:Switch Band2,ma_arfcn_index = 0x%x, pdch_band = %d!")
TRACE_MSG(PHHANDLE_2190_112_2_17_23_55_41_841,"L1:TBFCon: 1, exe_band = %d, new_band = %d, pwr_idx = %d!")
TRACE_MSG(PHHANDLE_2219_112_2_17_23_55_41_842,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_2251_112_2_17_23_55_41_843,"L1:TBFCon: 2, exe_band = %d, new_band = %d, pwr_idx = %d!")
TRACE_MSG(PHHANDLE_2428_112_2_17_23_55_42_844,"L1:TBFCon: 3, exe_band = %d, new_band = %d, pwr_idx = %d!")
TRACE_MSG(PHHANDLE_2458_112_2_17_23_55_42_845,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_2528_112_2_17_23_55_42_846,"L1:TBFCon: 4, exe_band = %d, new_band = %d, pwr_idx = %d!")
TRACE_MSG(PHHANDLE_2688_112_2_17_23_55_42_847,"L1:TBFCon: 5, exe_band = %d, new_band = %d, pwr_idx = %d!")
TRACE_MSG(PHHANDLE_2716_112_2_17_23_55_42_848,"L1:W start fn %d ,%d\n,curr_fn %d")
TRACE_MSG(PHHANDLE_2762_112_2_17_23_55_42_849,"L1:TBFCon: 6, exe_band = %d, new_band = %d, pwr_idx = %d!")
TRACE_MSG(PHHANDLE_2891_112_2_17_23_55_43_850,"split cyccle %d")
TRACE_MSG(PHHANDLE_2986_112_2_17_23_55_43_851,"L1: use old ta %d")
TRACE_MSG(PHHANDLE_3031_112_2_17_23_55_43_852,"L1: use old tai %d")
TRACE_MSG(PHHANDLE_3257_112_2_17_23_55_43_853,"L1 need to receive minor paging, is_receive_vicecard_paging:%d")
TRACE_MSG(PHHANDLE_3275_112_2_17_23_55_43_854,"Switch Band5: ma_arfcn_index = 0x%x, pdch_band = %d!")
TRACE_MSG(PHHANDLE_3358_112_2_17_23_55_44_855,"L1:set cell %d to NOT_SYN_CALL")
TRACE_MSG(PHHANDLE_3493_112_2_17_23_55_44_856,"L1:set cell %d to NOT_SYN_CALL")
TRACE_MSG(PHHANDLE_3573_112_2_17_23_55_44_857,"L1:set cell %d to NOT_SYN_CALL")
TRACE_MSG(PHHANDLE_3642_112_2_17_23_55_44_858,"L1:TBF Release: switch band, pmax=%d, pdch_band=%d, bcch_band=%d!")
TRACE_MSG(PHHANDLE_3970_112_2_17_23_55_45_859,"audio3: PHHandle_AppSetDevModeReq ph_set_dev_mode_ptr->Param1 %d, Param2 %d, Param3 %d, Param4 %d")
TRACE_MSG(PHHANDLE_4014_112_2_17_23_55_45_860,"L1 PHHandle_StartMagicVoiceFunc type %d")
TRACE_MSG(RESQ_253_112_2_17_23_55_46_861,"L1:Rx a PBCCH in BCCH_CAMPED state")
TRACE_MSG(RESQ_263_112_2_17_23_55_46_862,"L1:BCCH_CAMPED,C_t %d!\n")
TRACE_MSG(RESQ_345_112_2_17_23_55_46_863,"L1:PBCCH_CAMPED state,Discard a block,Ch_tag %d!\n")
TRACE_MSG(RESQ_386_112_2_17_23_55_46_864,"L1:Rx a PPCH in PLMN_CAMPED state")
TRACE_MSG(RESQ_399_112_2_17_23_55_46_865,"L1: Rx a SCH in PLMN_CAMPED state")
TRACE_MSG(RESQ_421_112_2_17_23_55_46_866,"L1:Rx a PBCCH in BCCH_CAMPED state")
TRACE_MSG(RESQ_432_112_2_17_23_55_46_867,"L1:PLMN_CAMPED state,Discard a block,Ch_tag %d!\n")
TRACE_MSG(RESQ_463_112_2_17_23_55_46_868,"L1:Rx a CBCH in PG_REORG_CAMPED state")
TRACE_MSG(RESQ_521_112_2_17_23_55_46_869,"L1: Rx a SCH in PG_REORG state")
TRACE_MSG(RESQ_531_112_2_17_23_55_46_870,"L1:PG_REORG CAMPED,Discard a block,Ch_tag %d!\n")
TRACE_MSG(RESQ_584_112_2_17_23_55_46_871,"L1:Rx a PBCCH in RACHAGCH state")
TRACE_MSG(RESQ_591_112_2_17_23_55_46_872,"L1:Rx a PPCH in RACHAGCH state")
TRACE_MSG(RESQ_601_112_2_17_23_55_46_873,"L1:Rx a PAGCH in RACHAGCH state")
TRACE_MSG(RESQ_611_112_2_17_23_55_46_874,"L1: Rx a SCH in RACHAGCH state")
TRACE_MSG(RESQ_665_112_2_17_23_55_47_875,"L1:Rx a PPCH in RACHAGCH state")
TRACE_MSG(RESQ_682_112_2_17_23_55_47_876,"L1: Rx a SCH in PRACHAGCH state")
TRACE_MSG(RESQ_688_112_2_17_23_55_47_877,"L1:Discard a block,Ch_tag %d!\n")
TRACE_MSG(RESQ_763_112_2_17_23_55_47_878,"L1:SDCCHTCH state, Discard a block,Ch_tag %d!\n")
TRACE_MSG(RESQ_804_112_2_17_23_55_47_879,"L1:Rx a PTCCH in TBF state")
TRACE_MSG(RESQ_900_112_2_17_23_55_47_880,"L1:Rx a BCCH in traffic")
TRACE_MSG(RESQ_906_112_2_17_23_55_47_881,"L1:Rx a PCH in traffic")
TRACE_MSG(RESQ_1257_112_2_17_23_55_48_882,"L1: FrameInstr Queue DSP Over Run")
TRACE_MSG(RESQ_1262_112_2_17_23_55_48_883,"L1: Cmd Queue DSP Over Run")
TRACE_MSG(RESQ_1267_112_2_17_23_55_48_884,"L1: TxData Queue DSP Over Run")
TRACE_MSG(RESQ_1272_112_2_17_23_55_48_885,"L1: Resp Queue DSP Over RunU")
TRACE_MSG(RESQ_1277_112_2_17_23_55_48_886,"L1: RxData Queue DSP Over Run")
TRACE_MSG(RESQ_1283_112_2_17_23_55_48_887,"L1: PowerMon Queue DSP Over Run")
TRACE_MSG(RESQ_1288_112_2_17_23_55_48_888,"L1: MeasReport Queue DSP Over Run")
TRACE_MSG(RESQ_1293_112_2_17_23_55_48_889,"L1: BlockSend Queue DSP Over Run")
TRACE_MSG(RESQ_1356_112_2_17_23_55_48_890,"L1 bsic fail %x,%x")
TRACE_MSG(RESQ_1422_112_2_17_23_55_48_891,"L1 is_sch_conflict %x,%x")
TRACE_MSG(RESQ_1487_112_2_17_23_55_48_892,"L1 bsic fail:  %x, fail count %d")
TRACE_MSG(RESQ_1496_112_2_17_23_55_48_893,"L1 is_sch_conflict in sdcchtch %d")
TRACE_MSG(RESQ_1577_112_2_17_23_55_48_894,"L1 handover cell search fail !%x")
TRACE_MSG(RESQ_1657_112_2_17_23_55_49_895,"L1:WakeUp! %d,%d")
TRACE_MSG(RESQ_1684_112_2_17_23_55_49_896,"L1:WakeUp! %d,%d")
TRACE_MSG(RESQ_1709_112_2_17_23_55_49_897,"L1:WakeUp! %d,%d")
TRACE_MSG(RESQ_1757_112_2_17_23_55_49_898,"L1 wakeup resp2")
TRACE_MSG(RESQ_1791_112_2_17_23_55_49_899,"L1:WakeUp in idle! %d,%d")
TRACE_MSG(RESQ_1815_112_2_17_23_55_49_900,"L1:WakeUp in power sweep! %d,%d")
TRACE_MSG(RESQ_1975_112_2_17_23_55_49_901,"L1:Q start fn %d ,fn_curr %d,elapse %d\n")
TRACE_MSG(RESQ_2075_112_2_17_23_55_49_902,"L1:Q start fn %d ,fn_curr %d,elapse %d\n")
TRACE_MSG(RESQ_2228_112_2_17_23_55_50_903,"L1:WakeUp pbcch! %d,%d")
TRACE_MSG(RESQ_2335_112_2_17_23_55_50_904,"L1:Q start fn %d ,fn_curr %d,elapse %d\n")
TRACE_MSG(RESQ_2502_112_2_17_23_55_50_905,"L1:Q start fn %d ,fn_curr %d,elapse %d\n")
TRACE_MSG(RESQ_2594_112_2_17_23_55_50_906,"L1:WakeUp! %d,%d")
TRACE_MSG(RESQ_2676_112_2_17_23_55_51_907,"L1:Q start fn %d ,fn_curr %d,elapse %d\n")
TRACE_MSG(RESQ_2895_112_2_17_23_55_51_908,"L1 scell adjust %x, %x")
TRACE_MSG(RESQ_2902_112_2_17_23_55_51_909,"L1 ncell adjust %x, %x; ncell %x, scell %x")
TRACE_MSG(RESQ_3027_112_2_17_23_55_51_910,"L1: echo_testing_CNF!")
TRACE_MSG(RESQ_3294_112_2_17_23_55_52_911,"L1: overrun %d p0 %d, p1 %d, num %d, que_num %d")
TRACE_MSG(RESQ_3551_112_2_17_23_55_52_912,"L1:State:SBC_CONFIG,Time = %d ms")
TRACE_MSG(RESQ_3562_112_2_17_23_55_52_913,"L1:State:SBC_ENCODE,Time = %d ms")
TRACE_MSG(RESQ_3574_112_2_17_23_55_52_914,"L1:State:PCM_TRANS,Time = %d ms")
TRACE_MSG(RESQ_3621_112_2_17_23_55_52_915,"L1:State:NONE_IN_USE,left_ptr=0x%x,right_ptr=0x%x,length = %d,Time = %d ms")
TRACE_MSG(RESQ_3692_112_2_17_23_55_53_916,"L1:Get SBC Frame.State:NONE_IN_USE,frame_ptr=0x%x,frame_length=0x%x,frame_num=%d, Time = %d ms")
TRACE_MSG(RESQ_3720_112_2_17_23_55_53_917,"L1: overrun %d p0 %d, p1 %d, num %d, que_num %d")
TRACE_MSG(RESQ_3758_112_2_17_23_55_53_918,"L1: overrun %d p0 %d, p1 %d, num %d, que_num %d")
TRACE_MSG(RESQ_3810_112_2_17_23_55_53_919,"L1: overrun %d p0 %d, p1 %d, num %d, que_num %d")
TRACE_MSG(RESQ_3869_112_2_17_23_55_53_920,"L1: TD PWRSWP, freq 0x%x, status %d, rssi %d")
TRACE_MSG(RESQ_3885_112_2_17_23_55_53_921,"L1: pwrswp dsp report arfcn not in list")
TRACE_MSG(RESQ_3964_112_2_17_23_55_53_922,"L1: TD MAX_R  rssi %d R_val 0x%x")
TRACE_MSG(RESQ_3972_112_2_17_23_55_53_923,"L1: pwrswp uarfcn 0x%x to agc")
TRACE_MSG(RESQ_4054_112_2_17_23_55_53_924,"L1:TDSYNC response wrongstate & coper, deactive coper")
TRACE_MSG(RESQ_4056_112_2_17_23_55_53_925,"L1_TD: Recieve Sync Idle in not TD_SYNC_SUBSTATE")
TRACE_MSG(RESQ_4084_112_2_17_23_55_53_926,"L1: sync dsp report arfcn not in list")
TRACE_MSG(RESQ_4095_112_2_17_23_55_53_927,"L1_TD: Recieve Sync Idle, freq 0x%x, status %d, sync_mode %d")
TRACE_MSG(RESQ_4134_112_2_17_23_55_53_928,"L1_TD: Recieve Sync Idle, freq 0x%x, status %d, sync_mode %d, delta_pos %d, max_dwpts_ratio %d, rxslot %d")
TRACE_MSG(RESQ_4143_112_2_17_23_55_53_929,"L1_TD: Recieve Sync Gprs, freq 0x%x, status %d, sync_mode %d, process_counter %d")
TRACE_MSG(RESQ_4150_112_2_17_23_55_53_930,"L1_TD: Recieve Sync Gprs, freq 0x%x, delta_pos %d")
TRACE_MSG(RESQ_4166_112_2_17_23_55_53_931,"L1_TD: Recieve Sync Gprs, freq 0x%x, status %d, sync_mode %d, process_counter %d, g_delta_pos[%d] %d, fristrxslot=%d")
TRACE_MSG(RESQ_4193_112_2_17_23_55_54_932,"L1_TD: Recieve Sync Gprs, delta_pos %d, rxslot %d")
TRACE_MSG(RESQ_4208_112_2_17_23_55_54_933,"L1: uarfcn 0x%x to agc")
TRACE_MSG(RESQ_4260_112_2_17_23_55_54_934,"L1_TD: Recieve Agc, freq 0x%x, status %d, gain %d")
TRACE_MSG(RESQ_4280_112_2_17_23_55_54_935,"L1:AGC response freq 0x%x not be found")
TRACE_MSG(RESQ_4291_112_2_17_23_55_54_936,"L1: AGC try disable coper %d")
TRACE_MSG(RESQ_4402_112_2_17_23_55_54_937,"L1_TD: discard meas result because of 30s resync")
TRACE_MSG(RESQ_4420_112_2_17_23_55_54_938,"L1_CONFLICT: p0 0x%x, p1 %d, p2 %d, p3 %d")
TRACE_MSG(RESQ_4454_112_2_17_23_55_54_939,"L1 ERROR td freqindex not find  frq 0x%x")
TRACE_MSG(RESQ_4554_112_2_17_23_55_54_940,"gsm_l1c:drop agc result!")
TRACE_MSG(RESQ_4576_112_2_17_23_55_54_941,"L1_TD QQQ: ResQ_DspOtdChangeForTDReq old delta_pos %d, otd %d")
TRACE_MSG(RESQ_4590_112_2_17_23_55_54_942,"L1_TD QQQ: ResQ_DspOtdChangeForTDReq new delta_pos %d")
TRACE_MSG(RESQ_4604_112_2_17_23_55_54_943,"LAYER1: enter Handle DSP_is_READY_REQ!\r\n")
TRACE_MSG(RESQ_4618_112_2_17_23_55_54_944,"audio3: ResQ_DspIsReadyReqModeChange g_audioPara_buffer %d, g_audio_length %d, g_td_audioPara_buffer %d, g_td_audio_length %d")
TRACE_MSG(RESQ_4673_112_2_17_23_55_54_945,"8800h bsic fail")
TRACE_MSG(RESQ_4688_112_2_17_23_55_55_946,"L1: Rx a SCH!\n")
TRACE_MSG(RESQ_4730_112_2_17_23_55_55_947,"L1: Wrong RX tag %d,\r\n!\n")
TRACE_MSG(RESQ_4761_112_2_17_23_55_55_948,"L1Q: ResQ_DspWakeUpModeChange SharedMemProc_SetTDSyncPara")
TRACE_MSG(RESQ_4766_112_2_17_23_55_55_949,"L1Q: MODE CHANGE: HO_FAIL_REVERT_TD or RESEL_FAIL_REVERT_TD")
TRACE_MSG(RESQ_4773_112_2_17_23_55_55_950,"L1_TD: manuel to td before sync 3 to 2 finish")
TRACE_MSG(RESQ_4787_112_2_17_23_55_55_951,"L1:WakeUp in Mode Change!")
TRACE_MSG(RESQ_4886_112_2_17_23_55_55_952,"L1IT_DMCase-0:ht=%d,curTyp=%d,setType=%d,p1=%d,p2=%d,p3=%d,p4=%d,cellid=0x%x,num=%d,cfreq=%d,sfreq=%d,span=%d,succ=%d,trytm=%d,r1=%d,r2=%d,gc=%d")
TRACE_MSG(RESQ_4899_112_2_17_23_55_55_953,"L1IT_DMCase-S: STOP Test!")
TRACE_MSG(RESQ_4952_112_2_17_23_55_55_954,"L1IT_DMCase-4:sdl0=%d,sdl1=%d,mid0=%d,mid1=%d,cellNum=%d,sdlNum=%d,p1=%d,p2=%d")
TRACE_MSG(RESQ_5023_112_2_17_23_55_55_955,"L1IT_DMCase-5:sdl0=%d,sdl1=%d,mid0=%d,mid1=%d,cellNum=%d,sdlNum=%d,p1=%d,p2=%d")
TRACE_MSG(RESQ_5112_112_2_17_23_55_55_956,"L1ITDMCase-0:Param Error!(hand_type=%d)")
TRACE_MSG(RXLEVMON_748_112_2_17_23_55_57_957,"L1: ba count %d %d, %d,total %d,%d")
TRACE_MSG(RXLEVMON_826_112_2_17_23_55_57_958,"L1: main_card_id:%d,card_id:%d,arfcn num: %d")
TRACE_MSG(RXLEVMON_958_112_2_17_23_55_58_959,"L1: ba count de %d %d, %d,total %d,%d")
TRACE_MSG(RXLEVMON_1228_112_2_17_23_55_58_960,"L1: not permit %x")
TRACE_MSG(RXLEVMON_1459_112_2_17_23_55_59_961,"L1:pwr_swp saturate with low gain, arfcn_index %d")
TRACE_MSG(RXLEVMON_1506_112_2_17_23_55_59_962,"L1: meas_ptr->is_valid is wrong!")
TRACE_MSG(RXLEVMON_1569_112_2_17_23_55_59_963,"L1:sat_low arfcn %x, , num %d")
TRACE_MSG(RXLEVMON_1651_112_2_17_23_55_59_964,"L1:Monitor saturate with low gain, arfcn_index %d")
TRACE_MSG(RXLEVMON_1664_112_2_17_23_55_59_965,"L1:sat_hi arfcn %x, num %d")
TRACE_MSG(RXLEVMON_1681_112_2_17_23_55_59_966,"L1: meas_ptr->is_valid is wrong!%d \n")
TRACE_MSG(RXLEVMON_1716_112_2_17_23_55_59_967,"L1 init sort ba in tbf and receive paging,vice card six strong not celar")
TRACE_MSG(RXLEVMON_1880_112_2_17_23_55_59_968,"L1: ba not valid")
TRACE_MSG(RXLEVMON_1908_112_2_17_23_56_0_969,"L1 warning: can't get valid ba")
TRACE_MSG(RXLEVMON_2152_112_2_17_23_56_0_970,"L1: IT's A WRONG g_measurement_ba_ptr->ba_table_state.band_ind!")
TRACE_MSG(RXLEVMON_2183_112_2_17_23_56_0_971,"L1:full band swp: arfcn_index(%x) rxlev %x times ")
TRACE_MSG(RXLEVMON_2236_112_2_17_23_56_0_972,"L1:full band swp: arfcn_index(%x) rxlev %x times ")
TRACE_MSG(RXLEVMON_2247_112_2_17_23_56_0_973,"L1:now %d,n1 %d,n2 %d")
TRACE_MSG(RXLEVMON_2371_112_2_17_23_56_0_974,"L1 vice card strongest arfcn_index 0x%x\n")
TRACE_MSG(RXLEVMON_2404_112_2_17_23_56_1_975,"L1 vicecard sixstrong arfcn 0x%x in maincard sixstrong num:%d")
TRACE_MSG(RXLEVMON_2507_112_2_17_23_56_1_976,"L1:rla %d,sum %d,count %d")
TRACE_MSG(RXLEVMON_2755_112_2_17_23_56_1_977,"L1 six strong main_card_id:%d")
TRACE_MSG(RXLEVMON_2778_112_2_17_23_56_1_978,"L1:traffic card_id: %d, six_strong_num:%d,arfcn: %x,%x,%x,%x,%x,%x;rla: %x,%x,%x,%x,%x,%x")
TRACE_MSG(RXLEVMON_2823_112_2_17_23_56_1_979,"L1:idle  %d, %d, %d;%x,%x,%x,%x,%x,%x;%x,%x,%x,%x,%x,%x;%x,%x,%x,%x,%x,%x")
TRACE_MSG(RXLEVMON_2903_112_2_17_23_56_2_980,"Layer1 %x turn strong")
TRACE_MSG(RXLEVMON_3135_112_2_17_23_56_2_981,"L1:ncell means report, is_multi_card:%d,sync_strong_num_tbf[0]:%d,sync_strong_num_tbf[1]:%d")
TRACE_MSG(RXLEVMON_3260_112_2_17_23_56_2_982,"L1 ba num %x, monitor num %x,no_monitor_period %x;%x,%x")
TRACE_MSG(RXLEVMON_3408_112_2_17_23_56_3_983,"L1:powersweep duration %d")
TRACE_MSG(RXLEVMON_3554_112_2_17_23_56_3_984,"L1: %x search fail too much, needn't sync")
TRACE_MSG(RXLEVMON_3568_112_2_17_23_56_3_985,"L1: This arfcn did not in current BA , arfcn_index %d \n")
TRACE_MSG(RXLEVMON_3617_112_2_17_23_56_3_986,"L1 the arfcn index is not in ba %x")
TRACE_MSG(RXLEVMON_3622_112_2_17_23_56_3_987,"L1 invalid arfcn index %x")
TRACE_MSG(RXLEVMON_3754_112_2_17_23_56_3_988,"L1 ba table is invalid!")
TRACE_MSG(RXLEVMON_4027_112_2_17_23_56_4_989,"L1 card_relation erro! arfcn_index %x, type %d")
TRACE_MSG(RXLEVMON_4033_112_2_17_23_56_4_990,"L1 the cell is not in ba! arfcn_index %x")
TRACE_MSG(RXLEVMON_4170_112_2_17_23_56_4_991,"L1 the arfcn is in old ba arfcn_index %x, type %d")
TRACE_MSG(RXLEVMON_4176_112_2_17_23_56_4_992,"L1 the arfcn is in old ba! arfcn_index %x")
TRACE_MSG(RXLEVMON_4215_112_2_17_23_56_4_993,"L1 the arfcn is in old ba arfcn_index %x, type %d")
TRACE_MSG(RXLEVMON_4221_112_2_17_23_56_4_994,"L1 the arfcn is in old ba! arfcn_index %x")
TRACE_MSG(RXLEVMON_4549_112_2_17_23_56_5_995,"L1:forgetting factor=%d")
TRACE_MSG(RXLEVMON_4682_112_2_17_23_56_5_996,"L1 input: mean_curr %x,cv_curr %x, e %x,r %x,rx_slot %x, cs %d")
TRACE_MSG(RXLEVMON_4727_112_2_17_23_56_5_997,"L1:input %x,%x ; output  %x, %x ; %x, %x;e %x,r %x,slot %x,rx_tfi %x,%x,slotmask%x,loopback %d")
TRACE_MSG(RXLEVMON_4888_112_2_17_23_56_5_998,"L1: s_save_prev_ba_table is full!")
TRACE_MSG(RXLEVMON_4991_112_2_17_23_56_6_999,"L1: RRA_MPH_RF_SCAN_IND!")
TRACE_MSG(RXLEVMON_5068_112_2_17_23_56_6_1000,"L1 vice card scell_ptr is NULL.")
TRACE_MSG(RXLEVMON_5108_112_2_17_23_56_6_1001,"L1,in tbf release report vicecard thirdstrong: %d,arfcn:0x%x(%d), 0x%x(%d), 0x%x(%d)")
TRACE_MSG(RXLEVMON_5148_112_2_17_23_56_6_1002,"L1 quick pwrswp normal finished!")
TRACE_MSG(RXLEVMON_5154_112_2_17_23_56_6_1003,"L1 quick pwrswp abort no valid ba!")
TRACE_MSG(RXLEVMON_5159_112_2_17_23_56_6_1004,"L1 pwr ++ index %x, %x")
TRACE_MSG(RXLEVMON_5286_112_2_17_23_56_6_1005,"L1 add a scell card id %d!%x")
TRACE_MSG(RXLEVMON_5315_112_2_17_23_56_6_1006,"L1 invalid scell ba_index!%x arfcn_index %x")
TRACE_MSG(RXLEVMON_5440_112_2_17_23_56_7_1007,"L1: cell is omitted %x")
TRACE_MSG(RXLEVMON_5580_112_2_17_23_56_7_1008,"L1 Emr is not started!")
TRACE_MSG(RXLEVMON_5601_112_2_17_23_56_7_1009,"L1 Emr is not valid %x, %x")
TRACE_MSG(RXLEVMON_5682_112_2_17_23_56_7_1010,"L1:ClearVeracity arfcn 0x%x veracity %d, timer %d")
TRACE_MSG(RXLEVMON_5713_112_2_17_23_56_7_1011,"L1:MidamVeracity arfcn 0x%x veracity %d, timer %d")
TRACE_MSG(RXLEVMON_5784_112_2_17_23_56_7_1012,"L1:UpdateVeracityTimer arfcn 0x%x veracity %d, timer %d")
TRACE_MSG(RXLEVMON_5854_112_2_17_23_56_7_1013,"L1: IdleTDSync sub_state %d, fn 0x%x")
TRACE_MSG(RXLEVMON_5957_112_2_17_23_56_8_1014,"L1_TD: RxLevMon_IdleTDMeasProcess td_meas_sub_state %d, fn 0x%x")
TRACE_MSG(RXLEVMON_5993_112_2_17_23_56_8_1015,"L1_TD: ERR in Idle RSCP, td_meas_ind %d meas_status %d")
TRACE_MSG(RXLEVMON_6037_112_2_17_23_56_8_1016,"L1:CheckIfMeasTD3rdFreq wrong state %d")
TRACE_MSG(RXLEVMON_6072_112_2_17_23_56_8_1017,"L1:CheckIfMeasTD3rdFreq freq num %d, rssi3 %d, threadhold %d")
TRACE_MSG(RXLEVMON_6095_112_2_17_23_56_8_1018,"L1:CheckIfMeasTD3rdFreq max1 %d, max2 %d")
TRACE_MSG(RXLEVMON_6107_112_2_17_23_56_8_1019,"L1:CheckIfMeasTD3rdFreq rssi3 be strongest %d")
TRACE_MSG(RXLEVMON_6121_112_2_17_23_56_8_1020,"L1:ResetMeasCtrlInd")
TRACE_MSG(RXLEVMON_6225_112_2_17_23_56_8_1021,"L1:GenTDMeasFreqIndex, no meas freq in status %d")
TRACE_MSG(RXLEVMON_6233_112_2_17_23_56_8_1022,"L1:GenTDMeasFreqIndex, status %d, start 0x%x, end 0x%x")
TRACE_MSG(RXLEVMON_6279_112_2_17_23_56_8_1023,"L1:GetNextTDMeasCtrlIndex, status %d, cur %d,start %d,end %d")
TRACE_MSG(RXLEVMON_6347_112_2_17_23_56_8_1024,"L1:GetNextTDMeasCtrlIndex,FALSE1 meas_cur_ind %d")
TRACE_MSG(RXLEVMON_6361_112_2_17_23_56_8_1025,"L1:GetNextTDMeasCtrlIndex, FALSE2 meas_cur_ind %d")
TRACE_MSG(RXLEVMON_6372_112_2_17_23_56_8_1026,"L1:GetNextTDMeasCtrlIndex, TRUE meas_cur_ind %d")
TRACE_MSG(RXLEVMON_6511_112_2_17_23_56_9_1027,"L1:CancelTDMeasCtrlIndex, meas_cmd %d, cur %d,start %d,end %d")
TRACE_MSG(RXLEVMON_6521_112_2_17_23_56_9_1028,"L1:error CancelTDMeasCtrlIndex no cancel, smain %d, sfreq %d")
TRACE_MSG(RXLEVMON_6538_112_2_17_23_56_9_1029,"L1:CancelCtrlIndex, wrong ind %d meas_cmd %d, freq 0x%x")
TRACE_MSG(RXLEVMON_6547_112_2_17_23_56_9_1030,"L1:error CancelTDMeasCtrlIndex, meas_cmd %d, freq 0x%x")
TRACE_MSG(RXLEVMON_6591_112_2_17_23_56_9_1031,"L1:CancelTDMeasByndex, freq_index %d, cur %d,start %d,end %d")
TRACE_MSG(RXLEVMON_6611_112_2_17_23_56_9_1032,"RxLevMon_GetTDNomalMeasFreqNum %d")
TRACE_MSG(RXLEVMON_6628_112_2_17_23_56_9_1033,"RxLevMon_GetTDNomalMeasFreqNum 2 %d")
TRACE_MSG(RXLEVMON_6656_112_2_17_23_56_9_1034,"L1:TD syncdl freq num = %d")
TRACE_MSG(RXLEVMON_6702_112_2_17_23_56_9_1035,"L1:Td IncPagingCycle %d, max %d")
TRACE_MSG(RXLEVMON_6749_112_2_17_23_56_9_1036,"L1:NewPagingCycle %d")
TRACE_MSG(RXLEVMON_6775_112_2_17_23_56_9_1037,"L1 : New TDMeasPolicy %d")
TRACE_MSG(RXLEVMON_6821_112_2_17_23_56_9_1038,"L1 : New RSCPNumPerPage %d")
TRACE_MSG(RXLEVMON_6907_112_2_17_23_56_10_1039,"L1:Td pch meas state %d")
TRACE_MSG(RXLEVMON_6911_112_2_17_23_56_10_1040,"L1:Td pch meas policy %d")
TRACE_MSG(RXLEVMON_6956_112_2_17_23_56_10_1041,"L1:GenerateTDRSCPMeasConfig, invalid pamfrms %d")
TRACE_MSG(RXLEVMON_6963_112_2_17_23_56_10_1042,"L1:GenerateTDRSCPMeasConfig, num %d")
TRACE_MSG(RXLEVMON_6967_112_2_17_23_56_10_1043,"L1:GenerateTDRSCPMeasConfig, rscp_num_per5sec = 0")
TRACE_MSG(RXLEVMON_7003_112_2_17_23_56_10_1044,"L1:CheckIfCoperEnable state %d, ind 0x%x")
TRACE_MSG(RXLEVMON_7025_112_2_17_23_56_10_1045,"L1:try2disableCoper state %d, oldind 0x%x, newind 0x%x")
TRACE_MSG(RXLEVMON_7086_112_2_17_23_56_10_1046,"L1:SetTDSyncCoperState new %d, old %d")
TRACE_MSG(RXLEVMON_7117_112_2_17_23_56_10_1047,"L1:SetTDSyncCoperIndex new 0x%x, old 0x%x")
TRACE_MSG(RXLEVMON_7201_112_2_17_23_56_10_1048,"L1_TD QQQ: RxLevMon_SetFrameIntChange g_pre_td_meas_frame_int_pos %d, cur_frame_int_pos %d, old_delta_pos %d")
TRACE_MSG(RXLEVMON_7233_112_2_17_23_56_10_1049,"L1_TD QQQ: RxLevMon_SetFrameIntChange delta_slot_qbc %d, new delta_pos %d")
TRACE_MSG(RXLEVMON_7237_112_2_17_23_56_10_1050,"L1: RxLevMon_SetFrameIntChange first rx slot %d, delta_pos %d")
TRACE_MSG(RXLEVMON_7256_112_2_17_23_56_10_1051,"L1_TD QQQ: RxLevMon_TdSyncFrameIntChange pre_frame_int_pos = %d, cur_frame_int_pos = %d")
TRACE_MSG(RXLEVMON_7298_112_2_17_23_56_10_1052,"L1_TD: Set Pwr Default Value arfcn[%d] 0x%x, gain %d")
TRACE_MSG(RXLEVMON_7663_112_2_17_23_56_11_1053,"L1:BuildTS0Cell ts0cell num %d, firepoint %d")
TRACE_MSG(RXLEVMON_7708_112_2_17_23_56_11_1054,"L1_TD: arfcn 0x%x TD_SYNC_DL_FIND")
TRACE_MSG(RXLEVMON_7717_112_2_17_23_56_11_1055,"L1_TD: arfcn 0x%x TD_SYNC_DL_FIND")
TRACE_MSG(RXLEVMON_7731_112_2_17_23_56_11_1056,"L1:BuildTS0Cell sdlcell num %d, nomalrssi num %d")
TRACE_MSG(RXLEVMON_7823_112_2_17_23_56_11_1057,"L1: new cell 2 Syncdl_search, old status %d, freq 0x%x")
TRACE_MSG(RXLEVMON_7846_112_2_17_23_56_11_1058,"L1_TD: lose sync pos")
TRACE_MSG(RXLEVMON_7913_112_2_17_23_56_12_1059,"L1_TD: delta_pos too big %d")
TRACE_MSG(RXLEVMON_7917_112_2_17_23_56_12_1060,"L1_TD: Meas Only rscp = %d")
TRACE_MSG(RXLEVMON_7927_112_2_17_23_56_12_1061,"L1_TD: lose sync pos in td sync meas")
TRACE_MSG(RXLEVMON_7962_112_2_17_23_56_12_1062,"L1:only td meas cell_meas %d, cell_old %d, status %d")
TRACE_MSG(RXLEVMON_7987_112_2_17_23_56_12_1063,"rscp dsp %d, average %d")
TRACE_MSG(RXLEVMON_7991_112_2_17_23_56_12_1064,"L1_TD: new max rscp value %d")
TRACE_MSG(RXLEVMON_8004_112_2_17_23_56_12_1065,"L1_TD_TEST: meas_td_ptr->meas_td_cell[%d].average_rscp %d")
TRACE_MSG(RXLEVMON_8018_112_2_17_23_56_12_1066,"L1_TD: max_id 0x%x, max_rscp %d, snd_id 0x%x, snd_rscp %d")
TRACE_MSG(RXLEVMON_8063_112_2_17_23_56_12_1067,"L1: meas rssi3 %d")
TRACE_MSG(RXLEVMON_8087_112_2_17_23_56_12_1068,"L1_TD: Only Td Sync ratio = %d, delta_pos = %d")
TRACE_MSG(RXLEVMON_8101_112_2_17_23_56_12_1069,"L1_TD: change delta_pos %d")
TRACE_MSG(RXLEVMON_8117_112_2_17_23_56_12_1070,"L1_TD: delta_pos too big %d")
TRACE_MSG(RXLEVMON_8127_112_2_17_23_56_12_1071,"L1_TD: lose sync pos in only td sync")
TRACE_MSG(RXLEVMON_8149_112_2_17_23_56_12_1072,"L1_TD: Both Meas Sync ratio = %d, delta_pos = %d, rscp = %d")
TRACE_MSG(RXLEVMON_8181_112_2_17_23_56_12_1073,"L1_TD: delta_pos too big %d")
TRACE_MSG(RXLEVMON_8194_112_2_17_23_56_12_1074,"L1_TD: lose sync pos in td sync meas")
TRACE_MSG(RXLEVMON_8228_112_2_17_23_56_12_1075,"l1 : ConvertTDCellMid2SDL 0x%x, num %d")
TRACE_MSG(RXLEVMON_8233_112_2_17_23_56_12_1076,"l1 : convert mid 0x%x")
TRACE_MSG(RXLEVMON_8274_112_2_17_23_56_12_1077,"L1_TD: cell_param_id[%d] 0x%x, delta_pos %d")
TRACE_MSG(RXLEVMON_8373_112_2_17_23_56_12_1078,"L1_TD: syncdl process_counter %d")
TRACE_MSG(RXLEVMON_8419_112_2_17_23_56_13_1079,"L1: GetTDFreqNum, freq num %d, status %d")
TRACE_MSG(RXLEVMON_8445_112_2_17_23_56_13_1080,"L1_TD: Recieve Syncdl, arfcn 0x%x, valid_cell %d, td_meas_sub_type %d, state %d")
TRACE_MSG(RXLEVMON_8472_112_2_17_23_56_13_1081,"L1:sdlnum %d, cellnum %d, needmid %d")
TRACE_MSG(RXLEVMON_8572_112_2_17_23_56_13_1082,"L1_TD: cell num %d, g_td_need_mid_freq_num %d")
TRACE_MSG(RXLEVMON_8596_112_2_17_23_56_13_1083,"L1_TD: syncdl goto TD_MIDAM_CONF_SUBSTATE")
TRACE_MSG(RXLEVMON_8607_112_2_17_23_56_13_1084,"L1_TD: syncdl search all fail")
TRACE_MSG(RXLEVMON_8619_112_2_17_23_56_13_1085,"L1_TD: resyncdl goto TD_MIDAM_CONF_SUBSTATE")
TRACE_MSG(RXLEVMON_8631_112_2_17_23_56_13_1086,"L1_TD: resync goto TD_RSCP_MEAS_SUBSTATE 0 new freq has syncdl")
TRACE_MSG(RXLEVMON_8648_112_2_17_23_56_13_1087,"L1_TD: resyncdl search all fail")
TRACE_MSG(RXLEVMON_8661_112_2_17_23_56_13_1088,"L1_TD: syncdl goto TD_MIDAM_CONF_SUBSTATE")
TRACE_MSG(RXLEVMON_8678_112_2_17_23_56_13_1089,"L1_TD: syncdl search all fail")
TRACE_MSG(RXLEVMON_8726_112_2_17_23_56_13_1090,"L1_TD: arfcn 0x%x mid[%d]=0x%x ")
TRACE_MSG(RXLEVMON_8749_112_2_17_23_56_13_1091,"L1_TD: arfcn 0x%x new mid[%d]=0x%x ")
TRACE_MSG(RXLEVMON_8761_112_2_17_23_56_13_1092,"L1_TD: Mids lose %d ")
TRACE_MSG(RXLEVMON_8838_112_2_17_23_56_13_1093,"L1:MergeTDMidFromNW, insert %d, cell num %d")
TRACE_MSG(RXLEVMON_8857_112_2_17_23_56_13_1094,"L1:MergeTDMidFromNW, insertnum %d, cell num %d")
TRACE_MSG(RXLEVMON_8879_112_2_17_23_56_14_1095,"L1_TD: Recieve Mid, freq 0x%x, valid_cell %d, td_meas_sub_type %d")
TRACE_MSG(RXLEVMON_8892_112_2_17_23_56_14_1096,"L1: Mid, sdlcellnum %d, midvalidnum %d, cellnum %d")
TRACE_MSG(RXLEVMON_8920_112_2_17_23_56_14_1097,"L1: Mid, freq 0x%x to RSCP")
TRACE_MSG(RXLEVMON_8925_112_2_17_23_56_14_1098,"L1: Mid, freq 0x%x to NOMAL RSSI")
TRACE_MSG(RXLEVMON_8971_112_2_17_23_56_14_1099,"L1_TD: mid goto IDLE_TD_RSCP_MEAS")
TRACE_MSG(RXLEVMON_8986_112_2_17_23_56_14_1100,"L1_TD: mid goto IDLE_TD_RSCP_MEAS")
TRACE_MSG(RXLEVMON_9059_112_2_17_23_56_14_1101,"TD CONFLICT: P0 0x%x, P1 0x%x, P2 0x%x, P3 0x%x")
TRACE_MSG(RXLEVMON_9066_112_2_17_23_56_14_1102,"L1:meas response conflict & coper")
TRACE_MSG(RXLEVMON_9080_112_2_17_23_56_14_1103,"TD CONFLICT dedi num %d, cause %d")
TRACE_MSG(RXLEVMON_9096_112_2_17_23_56_14_1104,"TD CONFLICT: TD_PWR")
TRACE_MSG(RXLEVMON_9101_112_2_17_23_56_14_1105,"TD CONFLICT: TD_SYNC")
TRACE_MSG(RXLEVMON_9120_112_2_17_23_56_14_1106,"TD CONFLICT: TD_RSCP")
TRACE_MSG(RXLEVMON_9124_112_2_17_23_56_14_1107,"TD CONFLICT: TD_SYNC_ONLY")
TRACE_MSG(RXLEVMON_9129_112_2_17_23_56_14_1108,"TD CONFLICT: TD_SYNC_RSCP")
TRACE_MSG(RXLEVMON_9137_112_2_17_23_56_14_1109,"TD CONFLICT: TD_RSSI")
TRACE_MSG(RXLEVMON_9143_112_2_17_23_56_14_1110,"TD CONFLICT: TD_SYNCDL")
TRACE_MSG(RXLEVMON_9150_112_2_17_23_56_14_1111,"TD CONFLICT: TD_MIDCONF")
TRACE_MSG(RXLEVMON_9192_112_2_17_23_56_14_1112,"L1:set prefer td meas in dedi search idle %d, %d")
TRACE_MSG(RXLEVMON_9341_112_2_17_23_56_14_1113,"L1_TD: lose sync pos in td sync meas")
TRACE_MSG(RXLEVMON_9369_112_2_17_23_56_15_1114,"L1:TS0Rssi cell num %d")
TRACE_MSG(RXLEVMON_9374_112_2_17_23_56_15_1115,"L1:TS0Rssi 0x%x, newts0rssi %d, oldts0rssi %d")
TRACE_MSG(RXLEVMON_9397_112_2_17_23_56_15_1116,"L1:TS0Rssi 0x%x, wrong state %d, mainstate %d")
TRACE_MSG(RXLEVMON_9407_112_2_17_23_56_15_1117,"L1: ts0 rssi result freq index %d, num %d")
TRACE_MSG(RXLEVMON_9445_112_2_17_23_56_15_1118,"L1_TD: Recieve Rssi, ratio %d, delta_pos %d, rscp %d")
TRACE_MSG(RXLEVMON_9462_112_2_17_23_56_15_1119,"L1_TD: rssi change delta_pos %d")
TRACE_MSG(RXLEVMON_9478_112_2_17_23_56_15_1120,"L1_TD: rssi change too big delta_pos 0x%x")
TRACE_MSG(RXLEVMON_9489_112_2_17_23_56_15_1121,"L1_TD:  lose sync pos in rssi meas")
TRACE_MSG(RXLEVMON_9559_112_2_17_23_56_15_1122,"L1: GprsTDMeasProcess coper Locked.")
TRACE_MSG(RXLEVMON_9734_112_2_17_23_56_15_1123,"L1 different first rx slot for gprs dwpts , base %d, new %d, before change %d")
TRACE_MSG(RXLEVMON_9812_112_2_17_23_56_15_1124,"This arfcn DO NOT happen in GSM mode!!!!")
TRACE_MSG(RXLEVMON_9948_112_2_17_23_56_16_1125,"L1 RxLevMon_GetTestArfcnMoniParam rxlev_result %d, rxdbm %d, count %d")
TRACE_MSG(SHAREDMEM_PROC_397_112_2_17_23_56_17_1126,"L1Q: sizeof(td_sharedMem_T) %d")
TRACE_MSG(SHAREDMEM_PROC_398_112_2_17_23_56_17_1127,"L1Q: sizeof(gsm_rf_para_T) %d")
TRACE_MSG(SHAREDMEM_PROC_399_112_2_17_23_56_17_1128,"L1Q: sizeof(DM_SHARE_CTRL_INFO) %d")
TRACE_MSG(SHAREDMEM_PROC_432_112_2_17_23_56_17_1129,"L1:get share memory base address,arm:%x,dsp:%x")
TRACE_MSG(SHAREDMEM_PROC_461_112_2_17_23_56_17_1130,"L1:print sharemem addr: 2g share 0x%x, shareend 0x%x")
TRACE_MSG(SHAREDMEM_PROC_465_112_2_17_23_56_17_1131,"L1:print sharemem addr:gsmrf 0x%x,aud share 0x%x")
TRACE_MSG(SHAREDMEM_PROC_506_112_2_17_23_56_17_1132,"L1:print sharemem addr: 2g share 0x%x,gsmrf 0x%x,aud share 0x%x")
TRACE_MSG(SHAREDMEM_PROC_515_112_2_17_23_56_17_1133,"L1:base address: 2g share 0x%x,td share 0x%x,gsmrf 0x%x,tdrf 0x%x,aud share 0x%x")
TRACE_MSG(SHAREDMEM_PROC_536_112_2_17_23_56_17_1134,"L1:address1: armbase 0x%x, dspbase 0x%x, sharedMem 0x%x, shareend 0x%x")
TRACE_MSG(SHAREDMEM_PROC_542_112_2_17_23_56_17_1135,"L1:address2: gsmrf 0x%x, aud share 0x%x")
TRACE_MSG(SHAREDMEM_PROC_598_112_2_17_23_56_17_1136,"L1:something wrong when to read status words!!")
TRACE_MSG(SHAREDMEM_PROC_629_112_2_17_23_56_17_1137,"L1:something is wrong when to write status words 5 times!!")
TRACE_MSG(SHAREDMEM_PROC_662_112_2_17_23_56_17_1138,"L1:something wrong when to read status words!!")
TRACE_MSG(SHAREDMEM_PROC_698_112_2_17_23_56_17_1139,"something is wrong when to write status words 5 times!!!!!!!!!!!!!!")
TRACE_MSG(SHAREDMEM_PROC_758_112_2_17_23_56_17_1140,"LAYER1: The powermoni queue is empty! \n")
TRACE_MSG(SHAREDMEM_PROC_843_112_2_17_23_56_18_1141,"L1_TD: SharedMemProc_GetGsmRfParaMemPtr")
TRACE_MSG(SHAREDMEM_PROC_877_112_2_17_23_56_18_1142,"L1: New ReducedFn 0x%x")
TRACE_MSG(SHAREDMEM_PROC_1262_112_2_17_23_56_18_1143,"L1: send chanchange sig")
TRACE_MSG(SHAREDMEM_PROC_1514_112_2_17_23_56_19_1144,"AUD_NEW: SharedMemProc_PostAudCmdQ next_cmd_in32 %d, next_cmd_out32 %d")
TRACE_MSG(SHAREDMEM_PROC_1553_112_2_17_23_56_19_1145,"AUD_NEW: SharedMemProc_PostCmdQ next_cmd_in16 %d, next_cmd_out16 %d")
TRACE_MSG(SHAREDMEM_PROC_1599_112_2_17_23_56_19_1146,"AUD_NEW: SharedMemProc_PostAudCmdQ next_cmd_in %d, next_cmd_out %d")
TRACE_MSG(SHAREDMEM_PROC_1642_112_2_17_23_56_19_1147,"L1: The rx queue is empty!")
TRACE_MSG(SHAREDMEM_PROC_1790_112_2_17_23_56_19_1148,"audio in out: %d,%d")
TRACE_MSG(SHAREDMEM_PROC_1973_112_2_17_23_56_20_1149,"TALENT: index %d, pwr %d")
TRACE_MSG(SHAREDMEM_PROC_1995_112_2_17_23_56_20_1150,"SetTxFramePcl, base address 0x%x, writein 0x%x")
TRACE_MSG(SHAREDMEM_PROC_2177_112_2_17_23_56_20_1151,"L1 cancel frame after wake up!, %x")
TRACE_MSG(SHAREDMEM_PROC_2442_112_2_17_23_56_21_1152,"L1:Wake up DSP, and sleep frame is at out + 1!!")
TRACE_MSG(SHAREDMEM_PROC_2624_112_2_17_23_56_21_1153,"L1_TD sync mode change: SharedMemProc_CancelObsoleteFrames distance_between_in_out = 0")
TRACE_MSG(SHAREDMEM_PROC_2632_112_2_17_23_56_21_1154,"L1:still Waiting for wakeup's response!")
TRACE_MSG(SHAREDMEM_PROC_2644_112_2_17_23_56_21_1155,"L1: TD not cancel frame1")
TRACE_MSG(SHAREDMEM_PROC_2694_112_2_17_23_56_21_1156,"L1 cancel search :%x")
TRACE_MSG(SHAREDMEM_PROC_2734_112_2_17_23_56_21_1157,"L1:SharedMemProc_CancelObsoleteFrames: total = %d!")
TRACE_MSG(SHAREDMEM_PROC_2817_112_2_17_23_56_21_1158,"L1 find ask more flag more than once!")
TRACE_MSG(SHAREDMEM_PROC_2873_112_2_17_23_56_22_1159,"L1 cancel margin %d, iscancel %d, newin %d, oldin %d, out %d, askmore %d")
TRACE_MSG(SHAREDMEM_PROC_2949_112_2_17_23_56_22_1160,"L1 cancel newaskmore  %d")
TRACE_MSG(SHAREDMEM_PROC_3008_112_2_17_23_56_22_1161,"L1:still Waiting for wakeup's response!")
TRACE_MSG(SHAREDMEM_PROC_3016_112_2_17_23_56_22_1162,"L1,CancelTBFSearchFrames,frame_index_out :%d,rf_op:%d.n")
TRACE_MSG(SHAREDMEM_PROC_3093_112_2_17_23_56_22_1163,"L1:distance between in and out is 0")
TRACE_MSG(SHAREDMEM_PROC_3187_112_2_17_23_56_22_1164,"SharedMemProc_CancelRemainingFrames:channel_mode_update")
TRACE_MSG(SHAREDMEM_PROC_3194_112_2_17_23_56_22_1165,"SharedMemProc_CancelRemainingFrames:TA update")
TRACE_MSG(SHAREDMEM_PROC_3201_112_2_17_23_56_22_1166,"SharedMemProc_CancelRemainingFrames:power update")
TRACE_MSG(SHAREDMEM_PROC_3209_112_2_17_23_56_22_1167,"SharedMemProc_CancelRemainingFrames:channel change")
TRACE_MSG(SHAREDMEM_PROC_3221_112_2_17_23_56_22_1168,"SharedMemProc_CancelRemainingFrames:gprs ta update")
TRACE_MSG(SHAREDMEM_PROC_3229_112_2_17_23_56_22_1169,"SharedMemProc_CancelRemainingFrames:gprs usf update")
TRACE_MSG(SHAREDMEM_PROC_3235_112_2_17_23_56_22_1170,"SharedMemProc_CancelRemainingFrames:gprs channel update")
TRACE_MSG(SHAREDMEM_PROC_3327_112_2_17_23_56_22_1171,"L1 set scell gain : result %x")
TRACE_MSG(SHAREDMEM_PROC_3394_112_2_17_23_56_23_1172,"L1 error: UpdateRxDataOutPtr rx buffer empty, rxdata in %d, rxdata out %d")
TRACE_MSG(SHAREDMEM_PROC_4033_112_2_17_23_56_24_1173,"audio3: write init para aud_sharedMem->gsm_aud_para_buf[%d].dl_da_device_l %d, aud_sharedMem->td_aud_para_buf[%d].dl_da_device_l %d")
TRACE_MSG(SHAREDMEM_PROC_4426_112_2_17_23_56_25_1174,"WriteGsmRfpara2DspRam: write init para aud_sharedMem->gsm_aud_para_buf[%d].dl_da_device_l %d, aud_sharedMem->td_aud_para_buf[%d].dl_da_device_l %d")
TRACE_MSG(SHAREDMEM_PROC_4430_112_2_17_23_56_25_1175,"L1_TD: WriteGsmRfpara2DspRam")
TRACE_MSG(SHAREDMEM_PROC_4439_112_2_17_23_56_25_1176,"system_param_dsp_use_ptr 0x%x")
TRACE_MSG(SHAREDMEM_PROC_4440_112_2_17_23_56_25_1177,"system_param_and_others_version 0x%x")
TRACE_MSG(SHAREDMEM_PROC_4505_112_2_17_23_56_25_1178,"L1_TD: memset td monitor item")
TRACE_MSG(SHAREDMEM_PROC_4525_112_2_17_23_56_25_1179,"L1: L1_Set_Cali_Flag, flag %d")
TRACE_MSG(SHAREDMEM_PROC_4533_112_2_17_23_56_25_1180,"L1: L1_Get_Cali_Flag, flag %d")
TRACE_MSG(SHAREDMEM_PROC_4555_112_2_17_23_56_25_1181,"L1: --assert_count %d, dsp_is_ready_flag %d, addr 0x%x")
TRACE_MSG(SHAREDMEM_PROC_4558_112_2_17_23_56_25_1182,"L1: assert_count %d, dsp_is_ready_flag %d, addr 0x%x")
TRACE_MSG(SHAREDMEM_PROC_4561_112_2_17_23_56_25_1183,"L1_TD:dsp rat state %d")
TRACE_MSG(SHAREDMEM_PROC_5172_112_2_17_23_56_26_1184,"L1:divide slp.slp cnt 0x%x,sec slp 0x%x,out %x,crr t:0x%x,nea t:0x%x")
TRACE_MSG(SHAREDMEM_PROC_5711_112_2_17_23_56_27_1185,"L1: download_voice_data_buf[%x][0] = %X")
TRACE_MSG(SHAREDMEM_PROC_5713_112_2_17_23_56_27_1186,"L1: download_voice_data_buf[%x][0] = %X")
TRACE_MSG(SHAREDMEM_PROC_5747_112_2_17_23_56_27_1187,"L1_TD: SharedMemProc_SetRfParameter")
TRACE_MSG(SHAREDMEM_PROC_5934_112_2_17_23_56_28_1188,"L1_TD: SharedMemProc_SetDevModeParam")
TRACE_MSG(SHAREDMEM_PROC_5948_112_2_17_23_56_28_1189,"audio3: SharedMemProc_SetDevModeParam gsmRfParaMem->audio_param.dl_da_device_l %d")
TRACE_MSG(SHAREDMEM_PROC_5962_112_2_17_23_56_28_1190,"AUD_NEW: SharedMemProc_SetDevModeParam aud_sharedMem->gsm_aud_para_buf[%d].dl_da_device_l %d, aud_sharedMem->td_aud_para_buf[%d].dl_da_device_l %d")
TRACE_MSG(SHAREDMEM_PROC_5988_112_2_17_23_56_28_1191,"AUD_NEW: SharedMemProc_SetDevModeParam, audio_buff_index %d")
TRACE_MSG(SHAREDMEM_PROC_6111_112_2_17_23_56_28_1192,"L1_TD: SharedMemProc_SetDevModeParam")
TRACE_MSG(SHAREDMEM_PROC_6125_112_2_17_23_56_28_1193,"audio3: SharedMemProc_SetDevModeParam gsmRfParaMem->audio_param.dl_da_device_l %d")
TRACE_MSG(SHAREDMEM_PROC_6134_112_2_17_23_56_28_1194,"AUD_NEW: SharedMemProc_SetDevModeParam aud_sharedMem->gsm_aud_para_buf[%d].dl_da_device_l %d, aud_sharedMem->td_aud_para_buf[%d].dl_da_device_l %d")
TRACE_MSG(SHAREDMEM_PROC_6164_112_2_17_23_56_28_1195,"AUD_NEW: SharedMemProc_SetDevModeParam, audio_buff_index %d")
TRACE_MSG(SHAREDMEM_PROC_6224_112_2_17_23_56_28_1196,"download param %ld, %ld")
TRACE_MSG(SHAREDMEM_PROC_6593_112_2_17_23_56_29_1197,"L1:test_count = %d")
TRACE_MSG(SHAREDMEM_PROC_6842_112_2_17_23_56_29_1198,"L1 relation:%x,%x,%x,%x,%x,%x,%x,%x")
TRACE_MSG(SHAREDMEM_PROC_6868_112_2_17_23_56_29_1199,"AUD_NEW: SharedMemProc_GetAudioParaBuffIndex return value is err!")
TRACE_MSG(SHAREDMEM_PROC_6907_112_2_17_23_56_29_1200,"L1:chipid 0x%x,set endian %d")
TRACE_MSG(SHAREDMEM_PROC_6946_112_2_17_23_56_30_1201,"L1G: *(td_shared_rf + ANTEN_OFFSET) 0x%x, *(td_shared_rf + TDPA_OFFSET) 0x%x")
TRACE_MSG(SHAREDMEM_PROC_6950_112_2_17_23_56_30_1202,"L1G: get td rf para from nv fail!")
TRACE_MSG(SHAREDMEM_PROC_6972_112_2_17_23_56_30_1203,"L1_TD: memset td monitor item")
TRACE_MSG(SHAREDMEM_PROC_7005_112_2_17_23_56_30_1204,"L1_Trans_GsmRf_Para: write init para aud_sharedMem->gsm_aud_para_buf[%d].dl_da_device_l %d, aud_sharedMem->td_aud_para_buf[%d].dl_da_device_l %d")
TRACE_MSG(SHAREDMEM_PROC_7013_112_2_17_23_56_30_1205,"L1_TD: L1_Trans_GsmRf_Para")
TRACE_MSG(SHAREDMEM_PROC_7028_112_2_17_23_56_30_1206,"system_param_dsp_use_ptr 0x%x")
TRACE_MSG(SHAREDMEM_PROC_7029_112_2_17_23_56_30_1207,"system_param_and_others_version 0x%x")
TRACE_MSG(SHAREDMEM_PROC_7101_112_2_17_23_56_30_1208,"L1: L1_Set_Cali_Flag, flag %d")
TRACE_MSG(SHAREDMEM_PROC_7126_112_2_17_23_56_30_1209,"L1: assert_count %d, dsp_is_ready_flag %d, addr 0x%x")
TRACE_MSG(SHAREDMEM_PROC_7129_112_2_17_23_56_30_1210,"L1_TD:dsp rat state %d")
TRACE_MSG(SHAREDMEM_PROC_7158_112_2_17_23_56_30_1211,"L1:base address: 2g share 0x%x,td share 0x%x,gsmrf 0x%x,tdrf 0x%x,aud share 0x%x")
TRACE_MSG(SHAREDMEM_PROC_7216_112_2_17_23_56_30_1212,"2cancel tdmeas frame para out=%d, in=%d, index=%d")
TRACE_MSG(SHAREDMEM_PROC_7243_112_2_17_23_56_30_1213,"L1_TD: SharedMemProc_TD_Cancel_plot_include_PTCCH frame_index_in %d")
TRACE_MSG(SHAREDMEM_PROC_7266_112_2_17_23_56_30_1214,"TDMeasParam in=0x%x,  arfcn=0x%x")
TRACE_MSG(SHAREDMEM_PROC_7296_112_2_17_23_56_30_1215,"WriteTDPara in %d, out %d")
TRACE_MSG(SHAREDMEM_PROC_7356_112_2_17_23_56_30_1216,"L1:Reset tdmeas para queue in=%d, out=%d")
TRACE_MSG(SHAREDMEM_PROC_7395_112_2_17_23_56_30_1217,"cancel tdmeas frame para out=%d, in=%d, index=%d")
TRACE_MSG(SHAREDMEM_PROC_7533_112_2_17_23_56_31_1218,"L1: Idle Sync, arfcn 0x%x, gain %d")
TRACE_MSG(SHAREDMEM_PROC_7561_112_2_17_23_56_31_1219,"L1_TD:Idle AGC arfcn 0x%x, gain %d, ts0_delta_pos %d, status %d")
TRACE_MSG(SHAREDMEM_PROC_7647_112_2_17_23_56_31_1220,"L1_TD: cell[%d] 0x%x")
TRACE_MSG(SHAREDMEM_PROC_7651_112_2_17_23_56_31_1221,"L1: RSCP Plot arfcn 0x%x, gain %d, td_meas_sub_type %d, cellnum %d")
TRACE_MSG(SHAREDMEM_PROC_7717_112_2_17_23_56_31_1222,"L1_TD: arfcn 0x%x, gain %d, delta_pos %d, td_meas_sub_type %d")
TRACE_MSG(SHAREDMEM_PROC_7731_112_2_17_23_56_31_1223,"L1_TD: arfcn 0x%x, gain %d, delta_pos %d, td_meas_sub_type %d")
TRACE_MSG(SHAREDMEM_PROC_7743_112_2_17_23_56_31_1224,"L1_TD: arfcn 0x%x, gain %d, delta_pos %d, td_meas_sub_type %d")
TRACE_MSG(SHAREDMEM_PROC_7752_112_2_17_23_56_31_1225,"L1_TD: arfcn 0x%x, gain %d, delta_pos %d, td_meas_sub_type %d")
TRACE_MSG(SHAREDMEM_PROC_7761_112_2_17_23_56_31_1226,"L1_TD: arfcn 0x%x, gain %d, delta_pos %d, td_meas_sub_type %d")
TRACE_MSG(SHAREDMEM_PROC_7816_112_2_17_23_56_31_1227,"L1_TD: arfcn 0x%x, gain %d, delta_pos %d, td_meas_sub_type %d, syncmid %d")
TRACE_MSG(SHAREDMEM_PROC_7822_112_2_17_23_56_31_1228,"L1_TD: cell[%d] 0x%x")
TRACE_MSG(SHAREDMEM_PROC_7876_112_2_17_23_56_31_1229,"L1_TD: arfcn 0x%x, gain %d, delta_pos %d, td_meas_sub_type %d, td_meas_ind=%d, mid_valid=%d, sdl_num=%d")
TRACE_MSG(SHAREDMEM_PROC_7881_112_2_17_23_56_31_1230,"L1_TD: cell[%d] 0x%x")
TRACE_MSG(SHAREDMEM_PROC_7897_112_2_17_23_56_31_1231,"L1_TD: cell[%d] 0x%x")
TRACE_MSG(SHAREDMEM_PROC_7971_112_2_17_23_56_32_1232,"L1_TD: gsm scell not exist, nonsync mode change")
TRACE_MSG(SHAREDMEM_PROC_8036_112_2_17_23_56_32_1233,"L1_TD: set td sync para arfcn_index %d, gain %d, bsic %d")
TRACE_MSG(SHAREDMEM_PROC_8062_112_2_17_23_56_32_1234,"L1Q: SharedMemProc_GetGSMSyncPara start")
TRACE_MSG(SHAREDMEM_PROC_8067_112_2_17_23_56_32_1235,"L1Q: g_dualmode_gsm_sfn is invalid, return")
TRACE_MSG(SHAREDMEM_PROC_8071_112_2_17_23_56_32_1236,"L1Q: g_dualmode_gsm_sfn 0x%x")
TRACE_MSG(SHAREDMEM_PROC_8072_112_2_17_23_56_32_1237,"L1Q: gsm_cell_num %d")
TRACE_MSG(SHAREDMEM_PROC_8080_112_2_17_23_56_32_1238,"L1Q: find arfcn 0x%x at %d  addfn %d")
TRACE_MSG(SHAREDMEM_PROC_8087_112_2_17_23_56_32_1239,"L1Q: arfcn_index 0x%x, Agc 0x%x, Fn 0x%x")
TRACE_MSG(SHAREDMEM_PROC_8095_112_2_17_23_56_32_1240,"audio3: SharedMemProc_GetGSMSyncPara g_audioPara_buffer %d, g_audio_length %d")
TRACE_MSG(SHAREDMEM_PROC_8096_112_2_17_23_56_32_1241,"L1Q: SharedMemProc_SetDevModeParam")
TRACE_MSG(SHAREDMEM_PROC_8106_112_2_17_23_56_32_1242,"L1Q: cell_index 0x%x, bsic 0x%x")
TRACE_MSG(SHAREDMEM_PROC_8113_112_2_17_23_56_32_1243,"L1Q: is handover")
TRACE_MSG(SHAREDMEM_PROC_8136_112_2_17_23_56_32_1244,"L1Q: is cell reselect")
TRACE_MSG(SHAREDMEM_PROC_8145_112_2_17_23_56_32_1245,"L1Q: arfcn not match, require 0x%x, in array[%d] 0x%x")
TRACE_MSG(SHAREDMEM_PROC_8150_112_2_17_23_56_32_1246,"L1Q: dm_share_ctrl_info->gsm_cell_is_valid[%d] is 0")
TRACE_MSG(SHAREDMEM_PROC_8156_112_2_17_23_56_32_1247,"L1Q: arfcn is not match, return")
TRACE_MSG(SHAREDMEM_PROC_8160_112_2_17_23_56_32_1248,"L1Q: goto sync mode change, return")
TRACE_MSG(SHAREDMEM_PROC_8180_112_2_17_23_56_32_1249,"L1:GetGSMSyncStatus %d")
TRACE_MSG(SHAREDMEM_PROC_8200_112_2_17_23_56_32_1250,"L1:GetRatChgStatus %d")
TRACE_MSG(SHAREDMEM_PROC_8216_112_2_17_23_56_32_1251,"L1:SharedMemProc_SetDAPCMDate")
TRACE_MSG(SHAREDMEM_PROC_8229_112_2_17_23_56_32_1252,"L1:SharedMemProc_SetNetPCMDate")
TRACE_MSG(SHAREDMEM_PROC_8423_112_2_17_23_56_33_1253,"L1_TD: main card otd %d")
TRACE_MSG(SHAREDMEM_PROC_8441_112_2_17_23_56_33_1254,"L1_TD: fn_card %d, delta_pos %d, first_slot %d")
TRACE_MSG(SHAREDMEM_PROC_8477_112_2_17_23_56_33_1255,"L1_TD: fn:0x%x, delta_pos %d, first_slot %d")
TRACE_MSG(SHAREDMEM_PROC_8494_112_2_17_23_56_33_1256,"L1_TD: i %d, card_id %d, cell_index %d, arfcn_index %d, otd %d")
TRACE_MSG(SHAREDMEM_PROC_8517_112_2_17_23_56_33_1257,"\"#3 ext_cmd_in=%d,out= %d")
TRACE_MSG(SHAREDMEM_PROC_8524_112_2_17_23_56_33_1258,"buffer_add_in=%x")
TRACE_MSG(SHAREDMEM_PROC_8551_112_2_17_23_56_33_1259,"\"#2 ext_in=%d,out= %d, id= %d")
TRACE_MSG(SHAREDMEM_PROC_8554_112_2_17_23_56_33_1260,"buffer_add=%x")
TRACE_MSG(SHAREDMEM_PROC_8578_112_2_17_23_56_33_1261,"\"# moved_extern_count=%d")
TRACE_MSG(SYSTEMINFO_874_112_2_17_23_56_35_1262,"L1: freq_redefine changed!!")
TRACE_MSG(SYSTEMINFO_939_112_2_17_23_56_35_1263,"L1: freq_redefine changed!!")
TRACE_MSG(SYSTEMINFO_1198_112_2_17_23_56_35_1264,"curr_fn %d,delta_fn %d,change delay %d,start_fn %d")
TRACE_MSG(SYSTEMINFO_1385_112_2_17_23_56_36_1265," Current chan same as next chan")
TRACE_MSG(SYSTEMINFO_1390_112_2_17_23_56_36_1266," Current chan different from next chan")
TRACE_MSG(SYSTEMINFO_1447_112_2_17_23_56_36_1267,"L1:loop_back paras: p1= %d, p2= %d!\n")
TRACE_MSG(SYSTEMINFO_1955_112_2_17_23_56_37_1268,"L1:NOTE: NOT ANY response from DSP for TCH search!! fn %d")
TRACE_MSG(SYSTEMINFO_2163_112_2_17_23_56_37_1269,"L1: cbch schedule start tb %d, tb")
TRACE_MSG(SYSTEMINFO_2170_112_2_17_23_56_37_1270,"L1:card id: %d,first schedule s_fn 0x%x,block num %d")
TRACE_MSG(SYSTEMINFO_2237_112_2_17_23_56_38_1271,"L1: schedule blocki %d")
TRACE_MSG(SYSTEMINFO_2260_112_2_17_23_56_38_1272,"L1:current schedule out 0x%x, blocki %d")
TRACE_MSG(SYSTEMINFO_2265_112_2_17_23_56_38_1273,"L1:end schedule and start receive all block 0-4")
TRACE_MSG(SYSTEMINFO_2765_112_2_17_23_56_39_1274,"L1 is first for tch %d")
TRACE_MSG(SYSTEMINFO_2917_112_2_17_23_56_39_1275,"L1 fn_sacchint %x,fn_next_sacch %x,fn_mod_sacch %x, fn_period %x")
TRACE_MSG(SYSTEMINFO_2961_112_2_17_23_56_39_1276,"L1 set frame type: is_debug_on %d,type %d, arfcn %x,gain %x")
TRACE_MSG(SYSTEMINFO_2985_112_2_17_23_56_39_1277,"L1 frametype %d, is_debug_mode %d")
TRACE_MSG(TAPWR_126_112_2_17_23_56_39_1278,"SetTxFramePcl, pwr_frame_ind %d, pwr_buf_ind %d, pcl[%d, %d, %d, %d]")
TRACE_MSG(TAPWR_223_112_2_17_23_56_40_1279,"L1: TaPwr_GetTxPwrCapabilty not suitable band\n")
TRACE_MSG(TAPWR_256_112_2_17_23_56_40_1280,"L1: SetMaxOutPutPowerLevel not suitable band\n")
TRACE_MSG(LAYER1_CALL_MAC_PH_150_112_2_17_23_56_43_1281,"\n s_polling_num = %d")
TRACE_MSG(LAYER1_CALL_MAC_PH_159_112_2_17_23_56_43_1282,"polling data send_type = %d")
TRACE_MSG(LAYER1_CALL_MAC_PH_172_112_2_17_23_56_43_1283,"polling data send_type = %d")
TRACE_MSG(PROCESS_MSG_876_112_2_17_23_56_46_1284,"Script file is wrong!")
TRACE_MSG(PROCESS_MSG_1774_112_2_17_23_56_47_1285,"L1:this rxlev data line is invalid,please update!")
TRACE_MSG(PROCESS_MSG_1824_112_2_17_23_56_48_1286,"L1:this sch data line is invalid,please update!")
TRACE_MSG(TEST_HANDLE_MSG_423_112_2_17_23_56_50_1287,"sleep counter =%d\n")
TRACE_MSG(TEST_HANDLE_MSG_809_112_2_17_23_56_51_1288,"sleep counter =%d\n")
TRACE_MSG(TEST_HANDLE_MSG_1507_112_2_17_23_56_52_1289,"LAYER1: RFoperation is wrong!\n")
TRACE_MSG(TEST_HANDLE_MSG_1730_112_2_17_23_56_53_1290,"\n SARAH test_search_response arfcn %d,T1 %d,T2 %d,T3P %d,BSIC %d")
TRACE_MSG(TEST_HANDLE_MSG_1785_112_2_17_23_56_53_1291,"\n SARAH test_send_SCH_data_to_MCU T1 %d,T2 %d,T3P %d,BSIC %d")
TRACE_MSG(TEST_HANDLE_MSG_1977_112_2_17_23_56_53_1292,"wake up:sleep counter %d\n")
TRACE_MSG(TEST_RW_SHARED_MEM_266_112_2_17_23_56_55_1293,"L1:command is NULL!!!")
TRACE_MSG(TEST_RW_SHARED_MEM_526_112_2_17_23_56_55_1294,"Rx data from channel has been read over!")
TRACE_MSG(MMI_OSBRIDGE_39_112_2_17_23_56_56_1295,"MMI : alloc signal error, signal szie = %d")
END_TRACE_MAP(DM_2G_LAYER1_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _DM_2G_LAYER1_TRC_H_

