/******************************************************************************
 ** File Name:      ms_ref_aud_exp_trc.h                                         *
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
//trace_id:34
#ifndef _MS_REF_AUD_EXP_TRC_H_
#define _MS_REF_AUD_EXP_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define AGC_EXP_100_112_2_18_1_2_24_0 "AGC_EXP:Lock Iram Area!"
#define AGC_EXP_122_112_2_18_1_2_24_1 "AGC_EXP:UnLock Iram Area!"
#define AGC_EXP_178_112_2_18_1_2_24_2 "s_agc_nv_param[%d][%d] b_agc_enable = %d,b_lcf_enable = %d,iAgcControlWord = 0x%x,0x%x, iAgcInputGain = %d, controlInfo:0x%x, 0x%x, agcInputGain:%d."
#define AGC_EXP_193_112_2_18_1_2_24_3 "AGC_Get_Param input para is error:agc_type:%d, para_ptr:0x%x."
#define AGC_EXP_199_112_2_18_1_2_24_4 "AGC_Get_Param failed to get mode name, mode:%d."
#define AGC_EXP_207_112_2_18_1_2_24_5 "AGC_Get_Param failed to alloc info."
#define AGC_EXP_212_112_2_18_1_2_24_6 "AGC_Get_Param failed to get mode para."
#define AGC_EXP_244_112_2_18_1_2_24_7 "Get_Audio_AgcType codec:0x%x, agc_type:%d"
#define AGC_EXP_307_112_2_18_1_2_24_8 "agc_exp.c::AGC_Set_Param:agc_param_ptr->bAgcEnable is %d  ,agc_par.bLcfEnable is %d,  agc_param_ptr->iDigitalGain is %d"
#define AGC_EXP_378_112_2_18_1_2_24_9 "agc_exp.c::AGC_Set_Param:agc_param_ptr->bAgcEnable is %d  ,agc_par.bLcfEnable is %d,  agc_param_ptr->iDigitalGain is %d"
#define AGC_EXP_400_112_2_18_1_2_24_10 "agc_exp.c:bEnable is %d, s_lcf_enable is %d"
#define AGC_EXP_414_112_2_18_1_2_24_11 "agc_exp.c:AGC_Set_DigitalGain is %d"
#define AGC_EXP_428_112_2_18_1_2_24_12 "agc_exp.c:AGC_ReInit_Param......"
#define AGC_EXP_435_112_2_18_1_2_24_13 "agc_exp.c:AGC_ReInit_Param failed to alloc info."
#define AGC_EXP_440_112_2_18_1_2_24_14 "agc_exp.c:AGC_ReInit_Param mode is null"
#define AGC_EXP_459_112_2_18_1_2_24_15 "agc_exp.c:AGC_ReInit_Param failed to get mode name."
#define AGC_EXP_582_112_2_18_1_2_25_16 "agc_exp.c:AGC_EXP_InitPara, mode:%d"
#define AGC_EXP_593_112_2_18_1_2_25_17 "agc_exp.c:: alloc fail, size: %d"
#define AGC_EXP_598_112_2_18_1_2_25_18 "agc_exp.c:: mode is null,s_digital_gain is %d"
#define AGC_EXP_606_112_2_18_1_2_25_19 "agc_exp.c::s_agc_type is %d"
#define AGC_EXP_623_112_2_18_1_2_25_20 "agc_exp.c:: s_cur_agc_type is AUDIO_AGC_INPUG_GAIN_MAX "
#define AGC_EXP_632_112_2_18_1_2_25_21 "agc_exp.c:AGC_ReInit_Param failed to get mode name, mode:%d."
#define AUD_AGC_191_112_2_18_1_2_26_22 "aud_agc.c,[AUD_Set_Param] !para check! %d, %d,%d, %d,0x%x, 0x%x, %d"
#define AUD_AGC_225_112_2_18_1_2_26_23 "AUD_Set_Param samplerate not report!!"
#define AUD_AGC_283_112_2_18_1_2_26_24 "lcf need not be set again"
#define AUD_AGC_290_112_2_18_1_2_26_25 "vol digital gain need not be set again"
#define AUD_AGC_352_112_2_18_1_2_26_26 "aud_agc.c,[AUD_Set_Param]:agc_gain:0x%x,agc_dg_gain:0x%x,dg_gain_12:0x%x,dg_gain_1:0x%x,dg_gain_2:0x%x"
#define AUD_AGC_374_112_2_18_1_2_26_27 "aud_agc.c,[AUD_AGC_Init] !para check! %d, %d,%d, %d,0x%x, 0x%x,%d"
#define AUD_AGC_408_112_2_18_1_2_26_28 "AUD_AGC_Init samplerate not report!!"
#define AUD_AGC_585_112_2_18_1_2_26_29 "aud_agc.c,[AUD_AGC_Init] :agc_gain:0x%x,agc_dg_gain:0x%x,dg_gain_12:0x%x,dg_gain_1:0x%x,dg_gain_2:0x%x"
#define AUD_ENHA_CONFIG_164_112_2_18_1_2_27_30 "aud_eha_config.c,[AUDENHA_SetHpfs] encounters error when caculating filter para!"
#define AUD_ENHA_CONFIG_179_112_2_18_1_2_27_31 "aud_eha_config.c,[AUDENHA_SetHpfs]lcf:lcf_sw:%d  filter_type:%dn"
#define AUD_ENHA_CONFIG_181_112_2_18_1_2_27_32 "aud_eha_config.c,[AUDENHA_SetHpfs]lcf:f1_g0:%d  f1_g1:%d  f1_fp:%d  f2_g0:%d f2_g1:%d f2_fp:%dn"
#define AUD_ENHA_CONFIG_184_112_2_18_1_2_27_33 "aud_eha_config.c,[AUDENHA_SetHpfs]lcf:S:%d,B0:%d B1:%d B2:%d; A0:%d A1:%d A2:%d;samplerate:%dn"
#define AUD_ENHA_CONFIG_220_112_2_18_1_2_27_34 "aud_eha_config.c,[AUDENHA_SetHpfs]band:%d encounters error when caculating filter para!"
#define AUD_ENHA_CONFIG_234_112_2_18_1_2_27_35 "aud_eha_config.c,[AUDENHA_SetHpfs]band %d:sw:%d,fo:%d  q:%d  boost:%d  gain:%dn"
#define AUD_ENHA_CONFIG_238_112_2_18_1_2_27_36 "aud_eha_config.c,[AUDPROC_Seteq]band %d:S:%d,B0:%d B1:%d B2:%d, A0:%d A1:%d A2:%d, samplerate:%dn"
#define AUD_ENHA_CONFIG_260_112_2_18_1_2_27_37 "aud_eha_config.c,[AUDENHA_SetHpf]:s0-s6 registers:%d  %d  %d  %d  %d  %d n"
#define AUD_ENHA_CONFIG_421_112_2_18_1_2_28_38 "aud_eha_config.c,[AUDENHA_InitHpf]  HPF is on!"
#define AUD_ENHA_CONFIG_433_112_2_18_1_2_28_39 "aud_eha_config.c,[AUDENHA_InitHpf]  set eq failed!"
#define AUD_ENHA_CONFIG_464_112_2_18_1_2_28_40 "aud_eha_config.c,[AUDENHA_SetEqboost]ERROR:eq_band_index:%d\n"
#define AUD_ENHA_CONFIG_498_112_2_18_1_2_28_41 "aud_eha_config.c,[AUDENHA_SetEqboost]band:%d encounters error when caculating filter para!"
#define AUD_ENHA_CONFIG_553_112_2_18_1_2_28_42 "aud_eha_config.c,[AUDENHA_SetEqMode]  set hpf failed!"
#define AUD_ENHA_CONFIG_604_112_2_18_1_2_28_43 "aud_eha_config.c,[AUDENHA_Init] Hpf init failed"
#define AUD_ENHA_CONFIG_622_112_2_18_1_2_28_44 "aud_eha_config.c,[AUDENHA_InitHpf]  eq is off!"
#define AUD_ENHA_EXP_334_112_2_18_1_2_29_45 "aud_enha_exp.c,[AUDENHA_GetParaFromEqset]is_eq_para_tunable:%d,eq_para_set_index:%d,tunable_eq_para_set_index:%d,s_cur_eq_mode_sel:%d"
#define AUD_ENHA_EXP_356_112_2_18_1_2_29_46 "aud_enha_exp.c,[AUDENHA_GetParaFromEqset] eq_para_set_index == 0!"
#define AUD_ENHA_EXP_442_112_2_18_1_2_30_47 "aud_enha_exp.c,[AUDENHA_GetParaFromEqset] tunable eq !current boost array:%d,%d,%d,%d,%d"
#define AUD_ENHA_EXP_477_112_2_18_1_2_30_48 "aud_enha_exp.c,[AUDENHA_GetParaFromAudMode] aud_proc_control[0]:0x%x;aud_proc_control[1]:0x%x"
#define AUD_ENHA_EXP_483_112_2_18_1_2_30_49 "aud_enha_exp.c,[AUDENHA_GetParaFromAudMode] GetVolumeAndType failed.error:%d."
#define AUD_ENHA_EXP_487_112_2_18_1_2_30_50 "aud_enha_exp.c,[AUDENHA_GetParaFromAudMode] uiVol:%d;eAppType:%d"
#define AUD_ENHA_EXP_494_112_2_18_1_2_30_51 "aud_enha_exp.c,[AUDENHA_GetParaFromAudMode] s_cur_alc_param.alc_sw:%d;s_cur_alc_param.alc_input_gain:%d"
#define AUD_ENHA_EXP_520_112_2_18_1_2_30_52 "aud_enha_exp.c,[AUDENHA_GetParaFromAudMode] lcf_sw:%d;filter_type:%d"
#define AUD_ENHA_EXP_550_112_2_18_1_2_30_53 "aud_enha_exp.c AUDENHA_GetPara mode:%d."
#define AUD_ENHA_EXP_556_112_2_18_1_2_30_54 "aud_enha_exp.c AUDENHA_GetPara failed to get mode param."
#define AUD_ENHA_EXP_630_112_2_18_1_2_30_55 "aud_enha_exp.c,[AUDENHA_SetPara] old digital gain:%d,new digital gain:%d"
#define AUD_ENHA_EXP_650_112_2_18_1_2_30_56 "aud_enha_exp.c,[AUDENHA_SetPara] old eq mode:%d,new eq mode:%d,bandboostarray:%d,%d,%d,%d,%d"
#define AUD_ENHA_EXP_681_112_2_18_1_2_30_57 "aud_enha_exp.c,[AUDENHA_SetPara] old eq mode:%d,new eq mode:%d"
#define AUD_ENHA_EXP_695_112_2_18_1_2_30_58 "aud_enha_exp.c,[AUDENHA_SetPara] AUD_ENHA_PARA_SET_DEFAULT:old eq mode:%d,new eq mode:%d"
#define AUD_ENHA_EXP_722_112_2_18_1_2_30_59 "aud_enha_exp.c [AUDENHA_SetPara] AUD_ENHA_PARA_BAND_BOOST:band_index:%d,band_boost:%d,eq_mode:%d"
#define AUD_ENHA_EXP_743_112_2_18_1_2_30_60 "aud_enha_exp.c,[AUDENHA_SetPara] AUD_ENHA_PARA_EQ_SET:old eq mode:%d,new eq mode:%d"
#define AUD_ENHA_EXP_754_112_2_18_1_2_30_61 "aud_enha_exp.c,[AUDENHA_SetPara] AUD_ENHA_PARA_TUNABLE_EQ_SET:old eq mode:%d,new eq mode:%d"
#define AUD_ENHA_EXP_776_112_2_18_1_2_30_62 "aud_enha_exp.c,[AUDENHA_SetPara] AUD_ENHA_PARA_DEV_MODE:"
#define AUD_ENHA_EXP_782_112_2_18_1_2_30_63 "aud_enha_exp.c [AUDENHA_SetPara] failed to get mode param."
#define AUD_ENHA_EXP_853_112_2_18_1_2_30_64 "aud_enha_exp.c,[AUDENHA_SetPara] invalid para type:%d"
#define AUD_ENHA_EXP_877_112_2_18_1_2_31_65 "aud_enha_exp.c,[AUDENHA_InitPara]:hdl:%08X,CodecSamplerate:%d,OutputSamplerate:%d"
#define AUD_ENHA_EXP_882_112_2_18_1_2_31_66 "aud_enha_exp.c,[AUDENHA_InitPara]:ENHA has been opened for hdl:%08X, exit"
#define AUD_ENHA_EXP_909_112_2_18_1_2_31_67 "aud_enha_exp.c AUDENHA_InitPara mode:%d."
#define AUD_ENHA_EXP_915_112_2_18_1_2_31_68 "aud_enha_exp.c AUDENHA_InitPara failed to get mode param."
#define AUD_ENHA_EXP_995_112_2_18_1_2_31_69 "aud_enha_exp.c,[AUDENHA_DeInitPara] hdl:%08X"
#define AUD_ENHA_EXP_999_112_2_18_1_2_31_70 "aud_enha_exp.c,[AUDENHA_DeInitPara] current hdl is:%08X, exit"
#define AUD_ENHA_EXP_1108_112_2_18_1_2_31_71 "aud_enha_exp.c,[AUDENHA_EXP_SetEqSetToRam]eq_para_set_index:%d,eq_mode_sel:%d"
#define AUD_ENHA_EXP_1282_112_2_18_1_2_31_72 "aud_enha_exp.c,[AUDENHA_EXP_SetTunableEqSetToRam]eq_para_set_index:%d,eq_mode_sel:%d"
#define AUD_PROC_CONFIG_128_112_2_18_1_2_32_73 "aud_proc_config.c,[AUDPROC_SetAgcIng] AGC_SW_n:%d,input_gain:%d,in_gain_set:%d"
#define AUD_PROC_CONFIG_176_112_2_18_1_2_32_74 "aud_proc_config.c,[AUDPROC_Seteq]-bass-:fo:%d  df:%d  boost:%d  gain:%d"
#define AUD_PROC_CONFIG_181_112_2_18_1_2_32_75 "aud_proc_config.c,[AUDPROC_Seteq] eq bass:nS:%dnB:%d %d %drn A:%d %d %drn"
#define AUD_PROC_CONFIG_195_112_2_18_1_2_32_76 "aud_proc_config.c,[AUDPROC_Seteq]-treble-:fo:%d  df:%d  boost:%d  gain:%d"
#define AUD_PROC_CONFIG_199_112_2_18_1_2_32_77 "aud_proc_config.c,[AUDPROC_Seteq] neq treble:nS:%dnB:%d %d %drn A:%d %d %drn"
#define AUD_PROC_CONFIG_208_112_2_18_1_2_32_78 "aud_proc_config.c,[AUDPROC_Seteq]:s2_cur_value:%d,s1_cur_value:%d,s0_cur_value:%d"
#define AUD_PROC_CONFIG_320_112_2_18_1_2_32_79 "aud_proc_config.c,[AUDPROC_Initlcf] lcf buttorworth left:S:%dn, B:%d %d %drn A:%d %d %drn"
#define AUD_PROC_CONFIG_341_112_2_18_1_2_32_80 "aud_proc_config.c,[AUDPROC_Initlcf] lcf buttorworth right:S:%dn, B:%d %d %drn A:%d %d %drn"
#define AUD_PROC_CONFIG_369_112_2_18_1_2_33_81 "aud_proc_config.c,[AUDPROC_Initlcf] lcf f1f1:S:%dn, B:%d %d %drn A:%d %d %drn"
#define AUD_PROC_CONFIG_387_112_2_18_1_2_33_82 "aud_proc_config.c,[AUDPROC_Initlcf] lcf type is %d.Not support!"
#define AUD_PROC_CONFIG_596_112_2_18_1_2_33_83 "[AUDPROC_Initeq]  eq is on!"
#define AUD_PROC_CONFIG_608_112_2_18_1_2_33_84 "[AUDPROC_Initeq] set eq failed!"
#define AUD_PROC_CONFIG_614_112_2_18_1_2_33_85 "[AUDPROC_Initeq]  eq is off!"
#define AUD_PROC_CONFIG_649_112_2_18_1_2_33_86 "aud_proc_config.c,[AUDPROC_SetAgcDg] digtal_gain:%d"
#define AUD_PROC_CONFIG_700_112_2_18_1_2_33_87 "aud_proc_config.c,[AUDPROC_Set] lcf set failed"
#define AUD_PROC_CONFIG_709_112_2_18_1_2_33_88 "aud_proc_config.c,[AUDPROC_Set] agc set failed"
#define AUD_PROC_CONFIG_718_112_2_18_1_2_33_89 "aud_proc_config.c,[AUDPROC_Set] eq set failed"
#define AUD_PROC_CONFIG_749_112_2_18_1_2_33_90 "aud_proc_config.c,[AUDPROC_Init] lcf init failed"
#define AUD_PROC_CONFIG_759_112_2_18_1_2_33_91 "aud_proc_config.c,[AUDPROC_Init] agc init failed"
#define AUD_PROC_CONFIG_771_112_2_18_1_2_33_92 "aud_proc_config.c,[AUDPROC_Init] eq init failed"
#define AUD_PROC_EXP_324_112_2_18_1_2_34_93 "aud_proc_exp.c,[AUDPROC_SetPara] GetVolumeAndType failed.error:%d."
#define AUD_PROC_EXP_330_112_2_18_1_2_34_94 "aud_proc_exp.c,[AUDPROC_SetPara] para type:%d, appType:%d."
#define AUD_PROC_EXP_339_112_2_18_1_2_34_95 "aud_proc_exp.c,[AUDPROC_SetPara] new digital gain:%d"
#define AUD_PROC_EXP_351_112_2_18_1_2_35_96 "aud_proc_exp.c,[AUDPROC_SetPara] old eq mode:%d,new eq mode:%d"
#define AUD_PROC_EXP_360_112_2_18_1_2_35_97 "aud_proc_exp.c,[AUDPROC_SetPara]AUDIO_GetDevMode:%d"
#define AUD_PROC_EXP_372_112_2_18_1_2_35_98 "aud_proc_exp.c:: alloc fail, size: %d"
#define AUD_PROC_EXP_377_112_2_18_1_2_35_99 "aud_proc_exp.c,[AUDPROC_SetPara] failed to get mode param."
#define AUD_PROC_EXP_394_112_2_18_1_2_35_100 "aud_proc_exp.c,[AUDPROC_SetPara]:eq para set index:%d"
#define AUD_PROC_EXP_570_112_2_18_1_2_35_101 "aud_proc_exp.c,[AUDPROC_InitPara]:AUDPROC has been opened for hdl:%08X, exit"
#define AUD_PROC_EXP_574_112_2_18_1_2_35_102 "aud_proc_exp.c,[AUDPROC_InitPara] restore digital gain:%d"
#define AUD_PROC_EXP_580_112_2_18_1_2_35_103 "aud_proc_exp.c,[AUDPROC_SetPara] GetVolumeAndType failed.error:%d."
#define AUD_PROC_EXP_606_112_2_18_1_2_35_104 "aud_proc_exp.c AUDPROC_InitPara mode:%d."
#define AUD_PROC_EXP_613_112_2_18_1_2_35_105 "aud_proc_exp.c:: alloc fail, size: %d"
#define AUD_PROC_EXP_619_112_2_18_1_2_35_106 "aud_proc_exp.c AUDPROC_InitPara failed to get mode param."
#define AUD_PROC_EXP_683_112_2_18_1_2_35_107 "aud_proc_exp.c, [AUDPROC_InitPara]-1- failed!"
#define AUD_PROC_EXP_735_112_2_18_1_2_35_108 "aud_proc_exp.c, [AUDPROC_InitPara] eq mode Num:%d,eq_mode_index:%d,uiSamplerate:%d,s_music_type:%d,eq_mode_para_set_num:%d"
#define AUD_PROC_EXP_799_112_2_18_1_2_36_109 "aud_proc_exp.c, [AUDPROC_InitPara] -2-failed!"
#define AUD_PROC_EXP_824_112_2_18_1_2_36_110 "aud_proc_exp.c,[AUDPROC_DeInitPara] current hdl is:%08X, exit"
#define AUD_PROC_EXP_832_112_2_18_1_2_36_111 "aud_proc_exp.c,[AUDPROC_DeInitPara]de init aud proc plugger!"
#define UP_SAMPLERATE_65_112_2_18_1_2_39_112 "up_samplerate.c,[UPSAMP_Initiate]:input samplerate needs not to be transformed!inSamplerate:%d"
#define UP_SAMPLERATE_79_112_2_18_1_2_39_113 "up_samplerate.c,[UPSAMP_Initiate]:inSamplerate:%d,outSamplerate:%d,s_is_up_two:%d"
#define DIGITALGAIN_132_112_2_18_1_2_40_114 "DG_Process reConfig, curTime:%d(ms), curSamples:%d, outputSamRate:%d, playTime:%d(ms)."
#define DIGITALGAIN_221_112_2_18_1_2_40_115 "DG_SetGain dg_level:0x%x, ui3_dg:0x%x, dg_Gi:0x%x, dg_M:0x%x, currentTime:%d(ms)."
#define EQ_EXP_546_112_2_18_1_2_41_116 "EQ_EXP:UnLock Iram Area!"
#define EQ_EXP_551_112_2_18_1_2_41_117 "EQ_EXP_Process: SET EQ_EXP_MODE_PASS MODE."
#define EQ_EXP_559_112_2_18_1_2_41_118 "EQ_EXP_Process s_uiSamNum %d or s_uiEqModeNum:%d is error "
#define EQ_EXP_568_112_2_18_1_2_41_119 "EQ_EXP:Lock Iram Area!"
#define EQ_EXP_577_112_2_18_1_2_41_120 "EQ_EXP:Lock Iram Area Fail!"
#define EQ_EXP_666_112_2_18_1_2_42_121 "EQ_EXP_Process: eq_cur_para=%d, %d, %d, %d, %d."
#define EQ_EXP_673_112_2_18_1_2_42_122 "EQ_EXP_Process 0==uiSrcCount."
#define EQ_EXP_732_112_2_18_1_2_42_123 "EQ_EXP:EQ_SetPara Error!"
#define EQ_EXP_747_112_2_18_1_2_42_124 "EQ_EXP_SetPara eq mode input %d is error."
#define EQ_EXP_752_112_2_18_1_2_42_125 "EQ_EXP_SetPara set mode:%d"
#define EQ_EXP_805_112_2_18_1_2_42_126 "EQ_EXP_SetPara 8,11,12,16,22,24,32,44,48khz are valid, the current samplerate is %d."
#define EQ_EXP_810_112_2_18_1_2_42_127 "EQ_EXP_SetPara set samplerate is %d."
#define EQ_EXP_888_112_2_18_1_2_42_128 "EQ_EXP:Lock Iram Area!"
#define EQ_EXP_895_112_2_18_1_2_42_129 "EQ_EXP:Lock Iram Area Fail!"
#define EQ_EXP_952_112_2_18_1_2_42_130 "EQ_EXP:UnLock Iram Area!"
#define RATECONTROL_EXP_144_112_2_18_1_2_43_131 "RATECONTROL_EXP_Process 0==uiSrcCount."
#define RATECONTROL_EXP_175_112_2_18_1_2_43_132 "RATECONTROL_Process initpara, samplerate:%d, rate:%f."
#define RATECONTROL_EXP_186_112_2_18_1_2_43_133 "RATECONTROL_Process enter changeSpeed"
#define RATECONTROL_EXP_193_112_2_18_1_2_43_134 "RATECONTROL_Process DestCount:%d"
#define RATECONTROL_EXP_220_112_2_18_1_2_43_135 "RATECONTROL_SetPara Error"
#define RATECONTROL_EXP_228_112_2_18_1_2_43_136 "RATECONTROL_SetPara error. rate:%d is not in range."
#define RATECONTROL_EXP_259_112_2_18_1_2_43_137 "RATECONTROL_InitPara uiOutputSamplerate = 0"
#define SOUNDSPEEDCHG_444_112_2_18_1_2_44_138 "soundSpeedChg_Construct pcmLeft:0x%x."
#define SOUNDSPEEDCHG_454_112_2_18_1_2_44_139 "soundSpeedChg_Construct pcmRight:0x%x."
#define SOUNDSPEEDCHG_468_112_2_18_1_2_45_140 "soundSpeedChg_Desconstruct pcmLeft is null."
#define SOUNDSPEEDCHG_478_112_2_18_1_2_45_141 "soundSpeedChg_Desconstruct pcmRight is null."
#define TRANSSAMPLERATE_250_112_2_18_1_2_46_142 "TRASSAM: Init transsamplerate for audio handle:%08X, %d-%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_AUD_EXP_TRC)
TRACE_MSG(AGC_EXP_100_112_2_18_1_2_24_0,"AGC_EXP:Lock Iram Area!")
TRACE_MSG(AGC_EXP_122_112_2_18_1_2_24_1,"AGC_EXP:UnLock Iram Area!")
TRACE_MSG(AGC_EXP_178_112_2_18_1_2_24_2,"s_agc_nv_param[%d][%d] b_agc_enable = %d,b_lcf_enable = %d,iAgcControlWord = 0x%x,0x%x, iAgcInputGain = %d, controlInfo:0x%x, 0x%x, agcInputGain:%d.")
TRACE_MSG(AGC_EXP_193_112_2_18_1_2_24_3,"AGC_Get_Param input para is error:agc_type:%d, para_ptr:0x%x.")
TRACE_MSG(AGC_EXP_199_112_2_18_1_2_24_4,"AGC_Get_Param failed to get mode name, mode:%d.")
TRACE_MSG(AGC_EXP_207_112_2_18_1_2_24_5,"AGC_Get_Param failed to alloc info.")
TRACE_MSG(AGC_EXP_212_112_2_18_1_2_24_6,"AGC_Get_Param failed to get mode para.")
TRACE_MSG(AGC_EXP_244_112_2_18_1_2_24_7,"Get_Audio_AgcType codec:0x%x, agc_type:%d")
TRACE_MSG(AGC_EXP_307_112_2_18_1_2_24_8,"agc_exp.c::AGC_Set_Param:agc_param_ptr->bAgcEnable is %d  ,agc_par.bLcfEnable is %d,  agc_param_ptr->iDigitalGain is %d")
TRACE_MSG(AGC_EXP_378_112_2_18_1_2_24_9,"agc_exp.c::AGC_Set_Param:agc_param_ptr->bAgcEnable is %d  ,agc_par.bLcfEnable is %d,  agc_param_ptr->iDigitalGain is %d")
TRACE_MSG(AGC_EXP_400_112_2_18_1_2_24_10,"agc_exp.c:bEnable is %d, s_lcf_enable is %d")
TRACE_MSG(AGC_EXP_414_112_2_18_1_2_24_11,"agc_exp.c:AGC_Set_DigitalGain is %d")
TRACE_MSG(AGC_EXP_428_112_2_18_1_2_24_12,"agc_exp.c:AGC_ReInit_Param......")
TRACE_MSG(AGC_EXP_435_112_2_18_1_2_24_13,"agc_exp.c:AGC_ReInit_Param failed to alloc info.")
TRACE_MSG(AGC_EXP_440_112_2_18_1_2_24_14,"agc_exp.c:AGC_ReInit_Param mode is null")
TRACE_MSG(AGC_EXP_459_112_2_18_1_2_24_15,"agc_exp.c:AGC_ReInit_Param failed to get mode name.")
TRACE_MSG(AGC_EXP_582_112_2_18_1_2_25_16,"agc_exp.c:AGC_EXP_InitPara, mode:%d")
TRACE_MSG(AGC_EXP_593_112_2_18_1_2_25_17,"agc_exp.c:: alloc fail, size: %d")
TRACE_MSG(AGC_EXP_598_112_2_18_1_2_25_18,"agc_exp.c:: mode is null,s_digital_gain is %d")
TRACE_MSG(AGC_EXP_606_112_2_18_1_2_25_19,"agc_exp.c::s_agc_type is %d")
TRACE_MSG(AGC_EXP_623_112_2_18_1_2_25_20,"agc_exp.c:: s_cur_agc_type is AUDIO_AGC_INPUG_GAIN_MAX ")
TRACE_MSG(AGC_EXP_632_112_2_18_1_2_25_21,"agc_exp.c:AGC_ReInit_Param failed to get mode name, mode:%d.")
TRACE_MSG(AUD_AGC_191_112_2_18_1_2_26_22,"aud_agc.c,[AUD_Set_Param] !para check! %d, %d,%d, %d,0x%x, 0x%x, %d")
TRACE_MSG(AUD_AGC_225_112_2_18_1_2_26_23,"AUD_Set_Param samplerate not report!!")
TRACE_MSG(AUD_AGC_283_112_2_18_1_2_26_24,"lcf need not be set again")
TRACE_MSG(AUD_AGC_290_112_2_18_1_2_26_25,"vol digital gain need not be set again")
TRACE_MSG(AUD_AGC_352_112_2_18_1_2_26_26,"aud_agc.c,[AUD_Set_Param]:agc_gain:0x%x,agc_dg_gain:0x%x,dg_gain_12:0x%x,dg_gain_1:0x%x,dg_gain_2:0x%x")
TRACE_MSG(AUD_AGC_374_112_2_18_1_2_26_27,"aud_agc.c,[AUD_AGC_Init] !para check! %d, %d,%d, %d,0x%x, 0x%x,%d")
TRACE_MSG(AUD_AGC_408_112_2_18_1_2_26_28,"AUD_AGC_Init samplerate not report!!")
TRACE_MSG(AUD_AGC_585_112_2_18_1_2_26_29,"aud_agc.c,[AUD_AGC_Init] :agc_gain:0x%x,agc_dg_gain:0x%x,dg_gain_12:0x%x,dg_gain_1:0x%x,dg_gain_2:0x%x")
TRACE_MSG(AUD_ENHA_CONFIG_164_112_2_18_1_2_27_30,"aud_eha_config.c,[AUDENHA_SetHpfs] encounters error when caculating filter para!")
TRACE_MSG(AUD_ENHA_CONFIG_179_112_2_18_1_2_27_31,"aud_eha_config.c,[AUDENHA_SetHpfs]lcf:lcf_sw:%d  filter_type:%dn")
TRACE_MSG(AUD_ENHA_CONFIG_181_112_2_18_1_2_27_32,"aud_eha_config.c,[AUDENHA_SetHpfs]lcf:f1_g0:%d  f1_g1:%d  f1_fp:%d  f2_g0:%d f2_g1:%d f2_fp:%dn")
TRACE_MSG(AUD_ENHA_CONFIG_184_112_2_18_1_2_27_33,"aud_eha_config.c,[AUDENHA_SetHpfs]lcf:S:%d,B0:%d B1:%d B2:%d; A0:%d A1:%d A2:%d;samplerate:%dn")
TRACE_MSG(AUD_ENHA_CONFIG_220_112_2_18_1_2_27_34,"aud_eha_config.c,[AUDENHA_SetHpfs]band:%d encounters error when caculating filter para!")
TRACE_MSG(AUD_ENHA_CONFIG_234_112_2_18_1_2_27_35,"aud_eha_config.c,[AUDENHA_SetHpfs]band %d:sw:%d,fo:%d  q:%d  boost:%d  gain:%dn")
TRACE_MSG(AUD_ENHA_CONFIG_238_112_2_18_1_2_27_36,"aud_eha_config.c,[AUDPROC_Seteq]band %d:S:%d,B0:%d B1:%d B2:%d, A0:%d A1:%d A2:%d, samplerate:%dn")
TRACE_MSG(AUD_ENHA_CONFIG_260_112_2_18_1_2_27_37,"aud_eha_config.c,[AUDENHA_SetHpf]:s0-s6 registers:%d  %d  %d  %d  %d  %d n")
TRACE_MSG(AUD_ENHA_CONFIG_421_112_2_18_1_2_28_38,"aud_eha_config.c,[AUDENHA_InitHpf]  HPF is on!")
TRACE_MSG(AUD_ENHA_CONFIG_433_112_2_18_1_2_28_39,"aud_eha_config.c,[AUDENHA_InitHpf]  set eq failed!")
TRACE_MSG(AUD_ENHA_CONFIG_464_112_2_18_1_2_28_40,"aud_eha_config.c,[AUDENHA_SetEqboost]ERROR:eq_band_index:%d\n")
TRACE_MSG(AUD_ENHA_CONFIG_498_112_2_18_1_2_28_41,"aud_eha_config.c,[AUDENHA_SetEqboost]band:%d encounters error when caculating filter para!")
TRACE_MSG(AUD_ENHA_CONFIG_553_112_2_18_1_2_28_42,"aud_eha_config.c,[AUDENHA_SetEqMode]  set hpf failed!")
TRACE_MSG(AUD_ENHA_CONFIG_604_112_2_18_1_2_28_43,"aud_eha_config.c,[AUDENHA_Init] Hpf init failed")
TRACE_MSG(AUD_ENHA_CONFIG_622_112_2_18_1_2_28_44,"aud_eha_config.c,[AUDENHA_InitHpf]  eq is off!")
TRACE_MSG(AUD_ENHA_EXP_334_112_2_18_1_2_29_45,"aud_enha_exp.c,[AUDENHA_GetParaFromEqset]is_eq_para_tunable:%d,eq_para_set_index:%d,tunable_eq_para_set_index:%d,s_cur_eq_mode_sel:%d")
TRACE_MSG(AUD_ENHA_EXP_356_112_2_18_1_2_29_46,"aud_enha_exp.c,[AUDENHA_GetParaFromEqset] eq_para_set_index == 0!")
TRACE_MSG(AUD_ENHA_EXP_442_112_2_18_1_2_30_47,"aud_enha_exp.c,[AUDENHA_GetParaFromEqset] tunable eq !current boost array:%d,%d,%d,%d,%d")
TRACE_MSG(AUD_ENHA_EXP_477_112_2_18_1_2_30_48,"aud_enha_exp.c,[AUDENHA_GetParaFromAudMode] aud_proc_control[0]:0x%x;aud_proc_control[1]:0x%x")
TRACE_MSG(AUD_ENHA_EXP_483_112_2_18_1_2_30_49,"aud_enha_exp.c,[AUDENHA_GetParaFromAudMode] GetVolumeAndType failed.error:%d.")
TRACE_MSG(AUD_ENHA_EXP_487_112_2_18_1_2_30_50,"aud_enha_exp.c,[AUDENHA_GetParaFromAudMode] uiVol:%d;eAppType:%d")
TRACE_MSG(AUD_ENHA_EXP_494_112_2_18_1_2_30_51,"aud_enha_exp.c,[AUDENHA_GetParaFromAudMode] s_cur_alc_param.alc_sw:%d;s_cur_alc_param.alc_input_gain:%d")
TRACE_MSG(AUD_ENHA_EXP_520_112_2_18_1_2_30_52,"aud_enha_exp.c,[AUDENHA_GetParaFromAudMode] lcf_sw:%d;filter_type:%d")
TRACE_MSG(AUD_ENHA_EXP_550_112_2_18_1_2_30_53,"aud_enha_exp.c AUDENHA_GetPara mode:%d.")
TRACE_MSG(AUD_ENHA_EXP_556_112_2_18_1_2_30_54,"aud_enha_exp.c AUDENHA_GetPara failed to get mode param.")
TRACE_MSG(AUD_ENHA_EXP_630_112_2_18_1_2_30_55,"aud_enha_exp.c,[AUDENHA_SetPara] old digital gain:%d,new digital gain:%d")
TRACE_MSG(AUD_ENHA_EXP_650_112_2_18_1_2_30_56,"aud_enha_exp.c,[AUDENHA_SetPara] old eq mode:%d,new eq mode:%d,bandboostarray:%d,%d,%d,%d,%d")
TRACE_MSG(AUD_ENHA_EXP_681_112_2_18_1_2_30_57,"aud_enha_exp.c,[AUDENHA_SetPara] old eq mode:%d,new eq mode:%d")
TRACE_MSG(AUD_ENHA_EXP_695_112_2_18_1_2_30_58,"aud_enha_exp.c,[AUDENHA_SetPara] AUD_ENHA_PARA_SET_DEFAULT:old eq mode:%d,new eq mode:%d")
TRACE_MSG(AUD_ENHA_EXP_722_112_2_18_1_2_30_59,"aud_enha_exp.c [AUDENHA_SetPara] AUD_ENHA_PARA_BAND_BOOST:band_index:%d,band_boost:%d,eq_mode:%d")
TRACE_MSG(AUD_ENHA_EXP_743_112_2_18_1_2_30_60,"aud_enha_exp.c,[AUDENHA_SetPara] AUD_ENHA_PARA_EQ_SET:old eq mode:%d,new eq mode:%d")
TRACE_MSG(AUD_ENHA_EXP_754_112_2_18_1_2_30_61,"aud_enha_exp.c,[AUDENHA_SetPara] AUD_ENHA_PARA_TUNABLE_EQ_SET:old eq mode:%d,new eq mode:%d")
TRACE_MSG(AUD_ENHA_EXP_776_112_2_18_1_2_30_62,"aud_enha_exp.c,[AUDENHA_SetPara] AUD_ENHA_PARA_DEV_MODE:")
TRACE_MSG(AUD_ENHA_EXP_782_112_2_18_1_2_30_63,"aud_enha_exp.c [AUDENHA_SetPara] failed to get mode param.")
TRACE_MSG(AUD_ENHA_EXP_853_112_2_18_1_2_30_64,"aud_enha_exp.c,[AUDENHA_SetPara] invalid para type:%d")
TRACE_MSG(AUD_ENHA_EXP_877_112_2_18_1_2_31_65,"aud_enha_exp.c,[AUDENHA_InitPara]:hdl:%08X,CodecSamplerate:%d,OutputSamplerate:%d")
TRACE_MSG(AUD_ENHA_EXP_882_112_2_18_1_2_31_66,"aud_enha_exp.c,[AUDENHA_InitPara]:ENHA has been opened for hdl:%08X, exit")
TRACE_MSG(AUD_ENHA_EXP_909_112_2_18_1_2_31_67,"aud_enha_exp.c AUDENHA_InitPara mode:%d.")
TRACE_MSG(AUD_ENHA_EXP_915_112_2_18_1_2_31_68,"aud_enha_exp.c AUDENHA_InitPara failed to get mode param.")
TRACE_MSG(AUD_ENHA_EXP_995_112_2_18_1_2_31_69,"aud_enha_exp.c,[AUDENHA_DeInitPara] hdl:%08X")
TRACE_MSG(AUD_ENHA_EXP_999_112_2_18_1_2_31_70,"aud_enha_exp.c,[AUDENHA_DeInitPara] current hdl is:%08X, exit")
TRACE_MSG(AUD_ENHA_EXP_1108_112_2_18_1_2_31_71,"aud_enha_exp.c,[AUDENHA_EXP_SetEqSetToRam]eq_para_set_index:%d,eq_mode_sel:%d")
TRACE_MSG(AUD_ENHA_EXP_1282_112_2_18_1_2_31_72,"aud_enha_exp.c,[AUDENHA_EXP_SetTunableEqSetToRam]eq_para_set_index:%d,eq_mode_sel:%d")
TRACE_MSG(AUD_PROC_CONFIG_128_112_2_18_1_2_32_73,"aud_proc_config.c,[AUDPROC_SetAgcIng] AGC_SW_n:%d,input_gain:%d,in_gain_set:%d")
TRACE_MSG(AUD_PROC_CONFIG_176_112_2_18_1_2_32_74,"aud_proc_config.c,[AUDPROC_Seteq]-bass-:fo:%d  df:%d  boost:%d  gain:%d")
TRACE_MSG(AUD_PROC_CONFIG_181_112_2_18_1_2_32_75,"aud_proc_config.c,[AUDPROC_Seteq] eq bass:nS:%dnB:%d %d %drn A:%d %d %drn")
TRACE_MSG(AUD_PROC_CONFIG_195_112_2_18_1_2_32_76,"aud_proc_config.c,[AUDPROC_Seteq]-treble-:fo:%d  df:%d  boost:%d  gain:%d")
TRACE_MSG(AUD_PROC_CONFIG_199_112_2_18_1_2_32_77,"aud_proc_config.c,[AUDPROC_Seteq] neq treble:nS:%dnB:%d %d %drn A:%d %d %drn")
TRACE_MSG(AUD_PROC_CONFIG_208_112_2_18_1_2_32_78,"aud_proc_config.c,[AUDPROC_Seteq]:s2_cur_value:%d,s1_cur_value:%d,s0_cur_value:%d")
TRACE_MSG(AUD_PROC_CONFIG_320_112_2_18_1_2_32_79,"aud_proc_config.c,[AUDPROC_Initlcf] lcf buttorworth left:S:%dn, B:%d %d %drn A:%d %d %drn")
TRACE_MSG(AUD_PROC_CONFIG_341_112_2_18_1_2_32_80,"aud_proc_config.c,[AUDPROC_Initlcf] lcf buttorworth right:S:%dn, B:%d %d %drn A:%d %d %drn")
TRACE_MSG(AUD_PROC_CONFIG_369_112_2_18_1_2_33_81,"aud_proc_config.c,[AUDPROC_Initlcf] lcf f1f1:S:%dn, B:%d %d %drn A:%d %d %drn")
TRACE_MSG(AUD_PROC_CONFIG_387_112_2_18_1_2_33_82,"aud_proc_config.c,[AUDPROC_Initlcf] lcf type is %d.Not support!")
TRACE_MSG(AUD_PROC_CONFIG_596_112_2_18_1_2_33_83,"[AUDPROC_Initeq]  eq is on!")
TRACE_MSG(AUD_PROC_CONFIG_608_112_2_18_1_2_33_84,"[AUDPROC_Initeq] set eq failed!")
TRACE_MSG(AUD_PROC_CONFIG_614_112_2_18_1_2_33_85,"[AUDPROC_Initeq]  eq is off!")
TRACE_MSG(AUD_PROC_CONFIG_649_112_2_18_1_2_33_86,"aud_proc_config.c,[AUDPROC_SetAgcDg] digtal_gain:%d")
TRACE_MSG(AUD_PROC_CONFIG_700_112_2_18_1_2_33_87,"aud_proc_config.c,[AUDPROC_Set] lcf set failed")
TRACE_MSG(AUD_PROC_CONFIG_709_112_2_18_1_2_33_88,"aud_proc_config.c,[AUDPROC_Set] agc set failed")
TRACE_MSG(AUD_PROC_CONFIG_718_112_2_18_1_2_33_89,"aud_proc_config.c,[AUDPROC_Set] eq set failed")
TRACE_MSG(AUD_PROC_CONFIG_749_112_2_18_1_2_33_90,"aud_proc_config.c,[AUDPROC_Init] lcf init failed")
TRACE_MSG(AUD_PROC_CONFIG_759_112_2_18_1_2_33_91,"aud_proc_config.c,[AUDPROC_Init] agc init failed")
TRACE_MSG(AUD_PROC_CONFIG_771_112_2_18_1_2_33_92,"aud_proc_config.c,[AUDPROC_Init] eq init failed")
TRACE_MSG(AUD_PROC_EXP_324_112_2_18_1_2_34_93,"aud_proc_exp.c,[AUDPROC_SetPara] GetVolumeAndType failed.error:%d.")
TRACE_MSG(AUD_PROC_EXP_330_112_2_18_1_2_34_94,"aud_proc_exp.c,[AUDPROC_SetPara] para type:%d, appType:%d.")
TRACE_MSG(AUD_PROC_EXP_339_112_2_18_1_2_34_95,"aud_proc_exp.c,[AUDPROC_SetPara] new digital gain:%d")
TRACE_MSG(AUD_PROC_EXP_351_112_2_18_1_2_35_96,"aud_proc_exp.c,[AUDPROC_SetPara] old eq mode:%d,new eq mode:%d")
TRACE_MSG(AUD_PROC_EXP_360_112_2_18_1_2_35_97,"aud_proc_exp.c,[AUDPROC_SetPara]AUDIO_GetDevMode:%d")
TRACE_MSG(AUD_PROC_EXP_372_112_2_18_1_2_35_98,"aud_proc_exp.c:: alloc fail, size: %d")
TRACE_MSG(AUD_PROC_EXP_377_112_2_18_1_2_35_99,"aud_proc_exp.c,[AUDPROC_SetPara] failed to get mode param.")
TRACE_MSG(AUD_PROC_EXP_394_112_2_18_1_2_35_100,"aud_proc_exp.c,[AUDPROC_SetPara]:eq para set index:%d")
TRACE_MSG(AUD_PROC_EXP_570_112_2_18_1_2_35_101,"aud_proc_exp.c,[AUDPROC_InitPara]:AUDPROC has been opened for hdl:%08X, exit")
TRACE_MSG(AUD_PROC_EXP_574_112_2_18_1_2_35_102,"aud_proc_exp.c,[AUDPROC_InitPara] restore digital gain:%d")
TRACE_MSG(AUD_PROC_EXP_580_112_2_18_1_2_35_103,"aud_proc_exp.c,[AUDPROC_SetPara] GetVolumeAndType failed.error:%d.")
TRACE_MSG(AUD_PROC_EXP_606_112_2_18_1_2_35_104,"aud_proc_exp.c AUDPROC_InitPara mode:%d.")
TRACE_MSG(AUD_PROC_EXP_613_112_2_18_1_2_35_105,"aud_proc_exp.c:: alloc fail, size: %d")
TRACE_MSG(AUD_PROC_EXP_619_112_2_18_1_2_35_106,"aud_proc_exp.c AUDPROC_InitPara failed to get mode param.")
TRACE_MSG(AUD_PROC_EXP_683_112_2_18_1_2_35_107,"aud_proc_exp.c, [AUDPROC_InitPara]-1- failed!")
TRACE_MSG(AUD_PROC_EXP_735_112_2_18_1_2_35_108,"aud_proc_exp.c, [AUDPROC_InitPara] eq mode Num:%d,eq_mode_index:%d,uiSamplerate:%d,s_music_type:%d,eq_mode_para_set_num:%d")
TRACE_MSG(AUD_PROC_EXP_799_112_2_18_1_2_36_109,"aud_proc_exp.c, [AUDPROC_InitPara] -2-failed!")
TRACE_MSG(AUD_PROC_EXP_824_112_2_18_1_2_36_110,"aud_proc_exp.c,[AUDPROC_DeInitPara] current hdl is:%08X, exit")
TRACE_MSG(AUD_PROC_EXP_832_112_2_18_1_2_36_111,"aud_proc_exp.c,[AUDPROC_DeInitPara]de init aud proc plugger!")
TRACE_MSG(UP_SAMPLERATE_65_112_2_18_1_2_39_112,"up_samplerate.c,[UPSAMP_Initiate]:input samplerate needs not to be transformed!inSamplerate:%d")
TRACE_MSG(UP_SAMPLERATE_79_112_2_18_1_2_39_113,"up_samplerate.c,[UPSAMP_Initiate]:inSamplerate:%d,outSamplerate:%d,s_is_up_two:%d")
TRACE_MSG(DIGITALGAIN_132_112_2_18_1_2_40_114,"DG_Process reConfig, curTime:%d(ms), curSamples:%d, outputSamRate:%d, playTime:%d(ms).")
TRACE_MSG(DIGITALGAIN_221_112_2_18_1_2_40_115,"DG_SetGain dg_level:0x%x, ui3_dg:0x%x, dg_Gi:0x%x, dg_M:0x%x, currentTime:%d(ms).")
TRACE_MSG(EQ_EXP_546_112_2_18_1_2_41_116,"EQ_EXP:UnLock Iram Area!")
TRACE_MSG(EQ_EXP_551_112_2_18_1_2_41_117,"EQ_EXP_Process: SET EQ_EXP_MODE_PASS MODE.")
TRACE_MSG(EQ_EXP_559_112_2_18_1_2_41_118,"EQ_EXP_Process s_uiSamNum %d or s_uiEqModeNum:%d is error ")
TRACE_MSG(EQ_EXP_568_112_2_18_1_2_41_119,"EQ_EXP:Lock Iram Area!")
TRACE_MSG(EQ_EXP_577_112_2_18_1_2_41_120,"EQ_EXP:Lock Iram Area Fail!")
TRACE_MSG(EQ_EXP_666_112_2_18_1_2_42_121,"EQ_EXP_Process: eq_cur_para=%d, %d, %d, %d, %d.")
TRACE_MSG(EQ_EXP_673_112_2_18_1_2_42_122,"EQ_EXP_Process 0==uiSrcCount.")
TRACE_MSG(EQ_EXP_732_112_2_18_1_2_42_123,"EQ_EXP:EQ_SetPara Error!")
TRACE_MSG(EQ_EXP_747_112_2_18_1_2_42_124,"EQ_EXP_SetPara eq mode input %d is error.")
TRACE_MSG(EQ_EXP_752_112_2_18_1_2_42_125,"EQ_EXP_SetPara set mode:%d")
TRACE_MSG(EQ_EXP_805_112_2_18_1_2_42_126,"EQ_EXP_SetPara 8,11,12,16,22,24,32,44,48khz are valid, the current samplerate is %d.")
TRACE_MSG(EQ_EXP_810_112_2_18_1_2_42_127,"EQ_EXP_SetPara set samplerate is %d.")
TRACE_MSG(EQ_EXP_888_112_2_18_1_2_42_128,"EQ_EXP:Lock Iram Area!")
TRACE_MSG(EQ_EXP_895_112_2_18_1_2_42_129,"EQ_EXP:Lock Iram Area Fail!")
TRACE_MSG(EQ_EXP_952_112_2_18_1_2_42_130,"EQ_EXP:UnLock Iram Area!")
TRACE_MSG(RATECONTROL_EXP_144_112_2_18_1_2_43_131,"RATECONTROL_EXP_Process 0==uiSrcCount.")
TRACE_MSG(RATECONTROL_EXP_175_112_2_18_1_2_43_132,"RATECONTROL_Process initpara, samplerate:%d, rate:%f.")
TRACE_MSG(RATECONTROL_EXP_186_112_2_18_1_2_43_133,"RATECONTROL_Process enter changeSpeed")
TRACE_MSG(RATECONTROL_EXP_193_112_2_18_1_2_43_134,"RATECONTROL_Process DestCount:%d")
TRACE_MSG(RATECONTROL_EXP_220_112_2_18_1_2_43_135,"RATECONTROL_SetPara Error")
TRACE_MSG(RATECONTROL_EXP_228_112_2_18_1_2_43_136,"RATECONTROL_SetPara error. rate:%d is not in range.")
TRACE_MSG(RATECONTROL_EXP_259_112_2_18_1_2_43_137,"RATECONTROL_InitPara uiOutputSamplerate = 0")
TRACE_MSG(SOUNDSPEEDCHG_444_112_2_18_1_2_44_138,"soundSpeedChg_Construct pcmLeft:0x%x.")
TRACE_MSG(SOUNDSPEEDCHG_454_112_2_18_1_2_44_139,"soundSpeedChg_Construct pcmRight:0x%x.")
TRACE_MSG(SOUNDSPEEDCHG_468_112_2_18_1_2_45_140,"soundSpeedChg_Desconstruct pcmLeft is null.")
TRACE_MSG(SOUNDSPEEDCHG_478_112_2_18_1_2_45_141,"soundSpeedChg_Desconstruct pcmRight is null.")
TRACE_MSG(TRANSSAMPLERATE_250_112_2_18_1_2_46_142,"TRASSAM: Init transsamplerate for audio handle:%08X, %d-%d")
END_TRACE_MAP(MS_REF_AUD_EXP_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_AUD_EXP_TRC_H_

