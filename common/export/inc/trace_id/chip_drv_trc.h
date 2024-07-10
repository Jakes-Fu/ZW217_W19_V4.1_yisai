/******************************************************************************
 ** File Name:      chip_drv_trc.h                                         *
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
//trace_id:5
#ifndef _CHIP_DRV_TRC_H_
#define _CHIP_DRV_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define ADC_PHY_V0_48_112_2_17_23_2_27_0 "[ADC:ADC_Init]"
#define ADC_PHY_V0_93_112_2_17_23_2_27_1 "[ADC:ADC_ConfigTpcChannel]"
#define ADC_PHY_V0_105_112_2_17_23_2_27_2 "[ADC:ADC_OpenTpcChannel]"
#define ADC_PHY_V0_117_112_2_17_23_2_27_3 "[ADC:ADC_CloseTpcChannel]"
#define ADC_PHY_V0_152_112_2_17_23_2_27_4 "0x%x 0x%x 0x%x 0x%x"
#define ADC_PHY_V0_153_112_2_17_23_2_27_5 "--%x"
#define ADC_PHY_V3_54_112_2_17_23_2_28_6 "[ADC:ADC_Init]"
#define ADC_PHY_V3_96_112_2_17_23_2_28_7 "[ADC:ADC_ConfigTPC]"
#define ADC_PHY_V3_106_112_2_17_23_2_29_8 "[ADC:ADC_OpenTPC]"
#define ADC_PHY_V3_116_112_2_17_23_2_29_9 "[ADC:ADC_CloseTPC]"
#define ADC_PHY_V3_155_112_2_17_23_2_29_10 "[ADC:ADC_GetResultDirectly timeout...."
#define ADC_PHY_V5_92_112_2_17_23_2_29_11 "ADC_IsrHandler interrupt ERROR\n"
#define ADC_PHY_V5_100_112_2_17_23_2_29_12 "[ADC_PHY_V5]ADC_IsrHandler.adc_data=%d\n"
#define ADC_PHY_V5_217_112_2_17_23_2_29_13 "[ADC:ADC_ConfigTPC] TPC_X,Y is bounded to ADC_SLOW_HW_CH0/CH1"
#define ADC_PHY_V5_224_112_2_17_23_2_29_14 "[ADC:ADC_CloseTPC] is null for sc6530."
#define ADC_PHY_V5_262_112_2_17_23_2_29_15 "[ADC:ADC_GetResultDirectly timeout...."
#define ADC_PHY_V5_273_112_2_17_23_2_29_16 "[ADC:ADC_GetResultDirectly] IN(%d) OUT(%d)"
#define ADC_PHY_V5___92_112_2_17_23_2_30_17 "ADC_IsrHandler interrupt ERROR\n"
#define ADC_PHY_V5___100_112_2_17_23_2_30_18 "[ADC_PHY_V5]ADC_IsrHandler.adc_data=%d\n"
#define ADC_PHY_V5___119_112_2_17_23_2_30_19 "[ADC:ADC_OpenTPC] is null for sc6530."
#define ADC_PHY_V5___220_112_2_17_23_2_30_20 "[ADC:ADC_ConfigTPC] TPC_X,Y is bounded to ADC_SLOW_HW_CH0/CH1"
#define ADC_PHY_V5___227_112_2_17_23_2_30_21 "[ADC:ADC_CloseTPC] is null for sc6530."
#define ADC_PHY_V5___265_112_2_17_23_2_30_22 "[ADC:ADC_GetResultDirectly timeout...."
#define ADC_PHY_V5___276_112_2_17_23_2_30_23 "[ADC:ADC_GetResultDirectly] IN(%d) OUT(%d)"
#define ANALOG_HAL_155_112_2_17_23_2_32_24 "_ANA_GetDevCurrentMaxValue: old_index = %d, new_index = %d, max_level = %d, vbat = %d, time = %d"
#define ANALOG_HAL_262_112_2_17_23_2_32_25 "ANA_SetDevValule: can't get analog device table point !! Fail !"
#define ANALOG_HAL_291_112_2_17_23_2_32_26 "ANA_SetDevValule: id = %d, value = %d, real_value = %d, time = %d"
#define ANALOG_HAL_295_112_2_17_23_2_32_27 "ANA_SetDevValule: Not Init !!"
#define ANALOG_PHY_V0_82_112_2_17_23_2_32_28 "_ANA_SetVibrator: Value=%d\r\n"
#define AUDIO_HAL_186_112_2_17_23_2_35_29 "[AUDDRV][HAL]init magic_num=0x%x"
#define AUDIO_HAL_198_112_2_17_23_2_35_30 "[AUDDRV][HAL]Init DAI Failed!"
#define AUDIO_HAL_210_112_2_17_23_2_35_31 "[AUDDRV][HAL]Init CODEC Failed!"
#define AUDIO_HAL_251_112_2_17_23_2_35_32 "[AUDDRV][HAL]open fun_flg=0x%x,fs=%d"
#define AUDIO_HAL_259_112_2_17_23_2_35_33 "[AUDDRV][HAL]real open 0x%x"
#define AUDIO_HAL_272_112_2_17_23_2_35_34 "[AUDDRV][HAL]open dai failed! 0x%x"
#define AUDIO_HAL_287_112_2_17_23_2_35_35 "[AUDDRV][HAL]open codec failed! 0x%x"
#define AUDIO_HAL_315_112_2_17_23_2_35_36 "[AUDDRV][HAL]close fun_flg=0x%x"
#define AUDIO_HAL_323_112_2_17_23_2_35_37 "[AUDDRV][HAL]real close 0x%x"
#define AUDIO_HAL_328_112_2_17_23_2_35_38 "[AUDDRV][HAL]close dai failed! 0x%x"
#define AUDIO_HAL_335_112_2_17_23_2_35_39 "[AUDDRV][HAL]close codec failed! 0x%x"
#define AUDIO_HAL_355_112_2_17_23_2_35_40 "[AUDDRV][HAL]ioctl cmd=%d,arg=0x%x"
#define AUDIO_HAL_401_112_2_17_23_2_35_41 "[AUDDRV][HAL]dac_set_buf_info pLHead=0x%x,pRHead=0x%x,cnt=%d"
#define AUDIO_HAL_456_112_2_17_23_2_35_42 "[AUDDRV][HAL]adc_set_buf_info pLHead=0x%x,pRHead=0x%x,cnt=%d"
#define AUDIO_HAL_492_112_2_17_23_2_35_43 "[AUDDRV][HAL]adc_trigger"
#define AUDIO_HAL_509_112_2_17_23_2_35_44 "[AUDDRV][HAL]register_playback 0x%x"
#define AUDIO_HAL_526_112_2_17_23_2_36_45 "[AUDDRV][HAL]register_capture 0x%x"
#define AUDIO_HAL_547_112_2_17_23_2_36_46 "[AUDDRV][HAL]inc 0x%x,0x%x,ref %d,ret %d"
#define AUDIO_HAL_566_112_2_17_23_2_36_47 "[AUDDRV][HAL]dec 0x%x,0x%x,ref %d,ret %d"
#define AUDIO_HAL_883_112_2_17_23_2_36_48 "[AUDDRV][HAL]codec notify %d"
#define AUDIO_HAL_919_112_2_17_23_2_36_49 "AUDIO CODEC CHR_CAP_IND"
#define AUDIO_OLD_API_128_112_2_17_23_2_37_50 "[VB_SwitchHpf]:is_enable:%d"
#define AUDIO_OLD_API_146_112_2_17_23_2_37_51 "[VB_SetHpfMode]:is_reset_mode:%d"
#define AUDIO_OLD_API_186_112_2_17_23_2_37_52 "[VB_SetHpfGain]:gain_index:%d,gain_value:%d"
#define AUDIO_OLD_API_200_112_2_17_23_2_37_53 "[VB_GetHpfGain]:gain_index:%d"
#define AUDIO_OLD_API_213_112_2_17_23_2_37_54 "[VB_SetHpfLimit]:rLimit:%d"
#define AUDIO_OLD_API_224_112_2_17_23_2_37_55 "[VB_SetHpfWidth]:width:%d"
#define AUDIO_OLD_API_235_112_2_17_23_2_37_56 "[VB_ALCSwitch]:is_enable:%d"
#define AUDIO_OLD_API_247_112_2_17_23_2_37_57 "[VB_SetALCParas]:hold:%d,rise:%d,fall:%d,limit:%d,threshold:%d,ratio:%d"
#define AUDIO_OLD_API_249_112_2_17_23_2_37_58 "[VB_SetALCParas]:cg_var:%d,release_rate:%d,attack_rate:%d,release_rate_ex:%d,attack_rate_ex:%d"
#define AUDIO_OLD_API_264_112_2_17_23_2_37_59 "[VB_SetFMMixMode]:da_index:%d,mix_mode:%d"
#define AUDIO_OLD_API_276_112_2_17_23_2_37_60 "[VB_DGSwitch]:da_index:%d,is_enable:%d"
#define AUDIO_OLD_API_288_112_2_17_23_2_37_61 "[VB_SetDG]:da_index:%d,dg_value:%d"
#define AUDIO_CODEC_DOLPHIN_253_112_2_17_23_2_38_62 "[AUDDRV][DOL]open err %08x"
#define AUDIO_CODEC_DOLPHIN_287_112_2_17_23_2_38_63 "[AUDDRV][DOL]close err %08x"
#define AUDIO_CODEC_DOLPHIN_329_112_2_17_23_2_38_64 "[AUDDRV][DOL]pga param"
#define AUDIO_CODEC_DOLPHIN_384_112_2_17_23_2_38_65 "[AUDDRV][DOL]drv out 0x%x"
#define AUDIO_CODEC_DOLPHIN_391_112_2_17_23_2_38_66 "[AUDDRV][DOL]drv out param"
#define AUDIO_CODEC_DOLPHIN_427_112_2_17_23_2_38_67 "[AUDDRV][DOL]drv in linein?"
#define AUDIO_CODEC_DOLPHIN_432_112_2_17_23_2_38_68 "[AUDDRV][DOL]drv in 0x%x"
#define AUDIO_CODEC_DOLPHIN_439_112_2_17_23_2_38_69 "[AUDDRV][DOL]drv in param"
#define AUDIO_CODEC_DOLPHIN_462_112_2_17_23_2_38_70 "[AUDDRV][DOL]dac mute"
#define AUDIO_CODEC_DOLPHIN_479_112_2_17_23_2_38_71 "[AUDDRV][DOL]get fs param"
#define AUDIO_CODEC_DOLPHIN_537_112_2_17_23_2_38_72 "[AUDDRV][DOL]ioctl param"
#define DOL_PHY_V1_126_112_2_17_23_2_39_73 "_dol_get_sample_rate_map, Can not support samplerate : %d"
#define DOL_PHY_V1_216_112_2_17_23_2_39_74 "__dol_s2mono %d"
#define DOL_PHY_V1_240_112_2_17_23_2_39_75 "__dol_mic_sel %d"
#define DOL_PHY_V1_252_112_2_17_23_2_39_76 "__dol_init"
#define DOL_PHY_V1_293_112_2_17_23_2_40_77 "__dol_open"
#define DOL_PHY_V1_302_112_2_17_23_2_40_78 "__dol_reset"
#define DOL_PHY_V1_321_112_2_17_23_2_40_79 "__dol_sb %d"
#define DOL_PHY_V1_341_112_2_17_23_2_40_80 "__dol_sb_sleep %d"
#define DOL_PHY_V1_360_112_2_17_23_2_40_81 "_dol_sb_micbias %d"
#define DOL_PHY_V1_380_112_2_17_23_2_40_82 "__dol_sb_adc %d"
#define DOL_PHY_V1_397_112_2_17_23_2_40_83 "__dol_linein_rec %d"
#define DOL_PHY_V1_424_112_2_17_23_2_40_84 "__dol_sb_dac %d"
#define DOL_PHY_V1_449_112_2_17_23_2_40_85 "__dol_sb_lin %d"
#define DOL_PHY_V1_468_112_2_17_23_2_40_86 "__dol_sb_mix %d"
#define DOL_PHY_V1_488_112_2_17_23_2_40_87 "__dol_sb_lout %d"
#define DOL_PHY_V1_501_112_2_17_23_2_40_88 "__dol_sb_out %d"
#define DOL_PHY_V1_540_112_2_17_23_2_40_89 "__dol_dac_mute %d"
#define DOL_PHY_V1_585_112_2_17_23_2_40_90 "__dol_hp_mute %d"
#define DOL_PHY_V1_604_112_2_17_23_2_40_91 "__dol_sb_blt %d"
#define DOL_PHY_V1_625_112_2_17_23_2_40_92 "__dol_ep_mute %d"
#define DOL_PHY_V1_634_112_2_17_23_2_40_93 "__dol_ramp_done %d 0x%x"
#define DOL_PHY_V1_663_112_2_17_23_2_40_94 "__dol_clr_ramp %d 0x%x"
#define DOL_PHY_V1_673_112_2_17_23_2_40_95 "__dol_gain_done %d 0x%x"
#define DOL_PHY_V1_702_112_2_17_23_2_40_96 "__dol_clr_gain %d 0x%x"
#define DOL_PHY_V1_714_112_2_17_23_2_40_97 "__dol_set_pag_gim 0x%x"
#define DOL_PHY_V1_722_112_2_17_23_2_40_98 "__dol_set_pag_gi 0x%x"
#define DOL_PHY_V1_728_112_2_17_23_2_40_99 "__dol_set_pag_golr 0x%x"
#define DOL_PHY_V1_736_112_2_17_23_2_40_100 "__dol_set_pag_gobtl 0x%x"
#define DOL_PHY_V1_760_112_2_17_23_2_41_101 "__dol_set_pag_lrgod %d 0x%x,0x%x"
#define DOL_PHY_V1_786_112_2_17_23_2_41_102 "__dol_set_pag_lrgo %d 0x%x,0x%x"
#define DOL_PHY_V1_810_112_2_17_23_2_41_103 "__dol_set_pag_lrgob %d 0x%x,0x%x"
#define DOL_PHY_V1_940_112_2_17_23_2_41_104 "__dol_set_da_samplerate : samplerate: %d"
#define DOL_PHY_V1_943_112_2_17_23_2_41_105 "__dol_set_da_samplerate, invalid DA samplerate: %d"
#define DOL_PHY_V1_958_112_2_17_23_2_41_106 "__dol_set_ad_samplerate : samplerate: %d"
#define DOL_PHY_V1_962_112_2_17_23_2_41_107 "__dol_set_ad_samplerate, invalid AD samplerate: %d"
#define DOL_PHY_V1_992_112_2_17_23_2_41_108 "[REG] VBAICR =0x%x"
#define DOL_PHY_V1_993_112_2_17_23_2_41_109 "[REG] VBCR1  =0x%x"
#define DOL_PHY_V1_994_112_2_17_23_2_41_110 "[REG] VBCR2  =0x%x"
#define DOL_PHY_V1_995_112_2_17_23_2_41_111 "[REG] VBCCR1 =0x%x"
#define DOL_PHY_V1_996_112_2_17_23_2_41_112 "[REG] VBCCR2 =0x%x"
#define DOL_PHY_V1_997_112_2_17_23_2_41_113 "[REG] VBPMR1 =0x%x"
#define DOL_PHY_V1_998_112_2_17_23_2_41_114 "[REG] VBPMR2 =0x%x"
#define DOL_PHY_V1_999_112_2_17_23_2_41_115 "[REG] VBCRR  =0x%x"
#define DOL_PHY_V1_1000_112_2_17_23_2_41_116 "[REG] VBICR  =0x%x"
#define DOL_PHY_V1_1001_112_2_17_23_2_41_117 "[REG] VBIFR  =0x%x"
#define DOL_PHY_V1_1002_112_2_17_23_2_41_118 "[REG] VBCGR1 =0x%x"
#define DOL_PHY_V1_1003_112_2_17_23_2_41_119 "[REG] VBCGR2 =0x%x"
#define DOL_PHY_V1_1004_112_2_17_23_2_41_120 "[REG] VBCGR3 =0x%x"
#define DOL_PHY_V1_1005_112_2_17_23_2_41_121 "[REG] VBCGR8 =0x%x"
#define DOL_PHY_V1_1006_112_2_17_23_2_41_122 "[REG] VBCGR9 =0x%x"
#define DOL_PHY_V1_1007_112_2_17_23_2_41_123 "[REG] VBCGR10=0x%x"
#define DOL_PHY_V1_1008_112_2_17_23_2_41_124 "[REG] VBTR1  =0x%x"
#define DOL_PHY_V1_1009_112_2_17_23_2_41_125 "[REG] VBTR2  =0x%x"
#define DOL_PHY_V1_1042_112_2_17_23_2_41_126 "__dol_ext_pa_ctl:%d"
#define DOL_PHY_V1_1048_112_2_17_23_2_41_127 "__dol_ext_hp_pa_ctl:%d"
#define DOL_STATE_V1_412_112_2_17_23_2_42_128 "[DOLST]DOL_SM_Process"
#define DOL_STATE_V1_416_112_2_17_23_2_42_129 "[DOLST]arm not run"
#define DOL_STATE_V1_474_112_2_17_23_2_42_130 "[DOLST]CHANGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
#define DOL_STATE_V1_697_112_2_17_23_2_43_131 "[DOLST]on_stop pre=%s,cur=%s"
#define DOL_STATE_V1_1057_112_2_17_23_2_43_132 "[AUDDRV][DOL]SB_OUT is on"
#define DOL_STATE_V1_1068_112_2_17_23_2_43_133 "[AUDDRV][DOL]RAMP is UP"
#define DOL_STATE_V1_1114_112_2_17_23_2_43_134 "[AUDDRV][DOL]SB_OUT is off"
#define DOL_STATE_V1_1125_112_2_17_23_2_43_135 "[AUDDRV][DOL]RAMP is DOWN"
#define DOL_STATE_V1_1207_112_2_17_23_2_44_136 "[AUDDRV][DOL]UnMuted"
#define DOL_STATE_V1_1218_112_2_17_23_2_44_137 "[AUDDRV][DOL]GAIN is UP"
#define DOL_STATE_V1_1258_112_2_17_23_2_44_138 "[AUDDRV][DOL]Muted"
#define DOL_STATE_V1_1269_112_2_17_23_2_44_139 "[AUDDRV][DOL]GAIN is DOWN"
#define DOL_STATE_V1_1587_112_2_17_23_2_44_140 "[DOLST]MERGED ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
#define DOL_STATE_V1_1610_112_2_17_23_2_44_141 "[DOLST] MERGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
#define DOL_STATE_V1_1625_112_2_17_23_2_44_142 "[DOLST] APPLY ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
#define DOL_STATE_V1_1647_112_2_17_23_2_44_143 "[DOLST]change state pre=%s,cur=%s,set=%s"
#define DOL_STATE_V1_1861_112_2_17_23_2_45_144 "[DOLST]index=%d"
#define DOLPHIN_PHY_CFG_78_112_2_17_23_2_45_145 "[AUDDRV][DOL]real set pga 0x%x"
#define DOLPHIN_PHY_CFG_83_112_2_17_23_2_45_146 "[AUDDRV][DOL]find pga 0x%x"
#define DOLPHIN_PHY_CFG_104_112_2_17_23_2_45_147 "[AUDDRV][DOL]all pga_mode 0x%x"
#define DOLPHIN_PHY_CFG_109_112_2_17_23_2_45_148 "[AUDDRV][DOL]set pga_mode 0x%x"
#define DOLPHIN_PHY_CFG_153_112_2_17_23_2_45_149 "[AUDDRV][DOL]pga set"
#define AUDIO_CODEC_LM49370_100_112_2_17_23_2_46_150 "LM49370_Config : Unconfigurable parameter!"
#define AUDIO_CODEC_LM49370_197_112_2_17_23_2_46_151 "LM49370 set Fail!\r\n"
#define AUDIO_CODEC_LM49370_211_112_2_17_23_2_46_152 "[AUDDRV][LM49370]init lm49370"
#define AUDIO_CODEC_LM49370_229_112_2_17_23_2_46_153 "[AUDDRV][LM49370]open lm49370 0x%x,%d"
#define AUDIO_CODEC_LM49370_254_112_2_17_23_2_46_154 "[AUDDRV][LM49370]close lm49370 0x%x"
#define AUDIO_CODEC_LM49370_274_112_2_17_23_2_46_155 "[AUDDRV][LM49370]ioctl lm49370 %d,0x%x"
#define AUDIO_CODEC_LM49370_295_112_2_17_23_2_46_156 "[AUDDRV][LM49370]ioctl param"
#define SPI_LM49370_V3_39_112_2_17_23_2_46_157 "SCM: This SPI Port Has been occupied!"
#define SPI_LM49370_V3_121_112_2_17_23_2_46_158 "Error in LM49370 Register 0x%02XrnValue Write In : 0x%02XrnValue Read Out : 0x%02Xrnrn"
#define SPI_LM49370_V3_127_112_2_17_23_2_46_159 "\r\nLM49370 Register Test Done, OK!\r\n"
#define SPI_LM49370_V3_1361_112_2_17_23_2_49_160 "Do not support 25bit per channel in IIS mode!(44.1KHz)\r\n"
#define SPI_LM49370_V3_1408_112_2_17_23_2_49_161 "Codec LM49370 do not support 8bit per channel in PCM mode!(44.1KHz)\r\n"
#define SPI_LM49370_V3_1461_112_2_17_23_2_49_162 "Do not support 25bit per channel in IIS mode!\r\n"
#define SPI_LM49370_V3_1505_112_2_17_23_2_49_163 "Do not support 25bit per channel in IIS mode!\r\n"
#define SPI_LM49370_V3_1613_112_2_17_23_2_49_164 "Do not support 25bit per channel in IIS mode!\r\n"
#define SPI_LM49370_V3_1717_112_2_17_23_2_50_165 "Do not support 25bit per channel in IIS mode!(16KHz)\r\n"
#define SPI_LM49370_V3_1826_112_2_17_23_2_50_166 "Do not support 25bit per channel in IIS mode!(32KHz)\r\n"
#define SPI_LM49370_V3_1941_112_2_17_23_2_50_167 "Do not support 25bit per channel in IIS mode!(48KHz)\r\n"
#define SPI_LM49370_V3_1987_112_2_17_23_2_50_168 "Codec LM49370 do not support 8bit per channel in PCM mode!(48KHz)\r\n"
#define AUDIO_CODEC_SPRD_253_112_2_17_23_2_51_169 "[AUDDRV][SPRD]init sprd_codec"
#define AUDIO_CODEC_SPRD_278_112_2_17_23_2_51_170 "[AUDDRV][SPRD]open sprd_codec 0x%x,%d"
#define AUDIO_CODEC_SPRD_282_112_2_17_23_2_51_171 "[AUDDRV][SPRD]open err %08x"
#define AUDIO_CODEC_SPRD_316_112_2_17_23_2_51_172 "[AUDDRV][SPRD]close sprd_codec 0x%x"
#define AUDIO_CODEC_SPRD_326_112_2_17_23_2_51_173 "[AUDDRV][SPRD]close err %08x"
#define AUDIO_CODEC_SPRD_368_112_2_17_23_2_51_174 "[AUDDRV][SPRD]pga param"
#define AUDIO_CODEC_SPRD_382_112_2_17_23_2_52_175 "[AUDDRV][SPRD]drv out ctl 0x%x,%d"
#define AUDIO_CODEC_SPRD_417_112_2_17_23_2_52_176 "[AUDDRV][SPRD]drv out 0x%x"
#define AUDIO_CODEC_SPRD_424_112_2_17_23_2_52_177 "[AUDDRV][SPRD]drv out param"
#define AUDIO_CODEC_SPRD_437_112_2_17_23_2_52_178 "[AUDDRV][SPRD]drv in ctl 0x%x,%d"
#define AUDIO_CODEC_SPRD_472_112_2_17_23_2_52_179 "[AUDDRV][SPRD]drv in 0x%x"
#define AUDIO_CODEC_SPRD_479_112_2_17_23_2_52_180 "[AUDDRV][SPRD]drv in param"
#define AUDIO_CODEC_SPRD_502_112_2_17_23_2_52_181 "[AUDDRV][SPRD]dac mute"
#define AUDIO_CODEC_SPRD_519_112_2_17_23_2_52_182 "[AUDDRV][SPRD]get fs param"
#define AUDIO_CODEC_SPRD_550_112_2_17_23_2_52_183 "[AUDDRV][SPRD]get fs param"
#define AUDIO_CODEC_SPRD_569_112_2_17_23_2_52_184 "[AUDDRV][SPRD]ioctl sprd_codec %d,0x%x"
#define AUDIO_CODEC_SPRD_606_112_2_17_23_2_52_185 "[AUDDRV][SPRD]ioctl param"
#define AUDIO_CODEC_SPRD_619_112_2_17_23_2_52_186 "[AUDDRV][SPRD]PA %d,%d,0x%x"
#define AUDIO_CODEC_SPRD_638_112_2_17_23_2_52_187 "[AUDDRV][SPRD]HP %d,%d,0x%x"
#define AUDIO_CODEC_SPRD_665_112_2_17_23_2_52_188 "[AUDDRV][SPRD]EP %d,%d,0x%x"
#define AUDIO_CODEC_SPRD_680_112_2_17_23_2_52_189 "[AUDDRV][SPRD]ADCPGA %d,%d,0x%x"
#define AUDIO_CODEC_SPRD_767_112_2_17_23_2_52_190 "_run_callback:%d"
#define AUDIO_CODEC_SPRD_775_112_2_17_23_2_52_191 "AUDIO_PA_Ctl:%d,%d"
#define SPRD_CODEC_PHY_99_112_2_17_23_2_53_192 "__sprd_codec_hp_mute:%d"
#define SPRD_CODEC_PHY_116_112_2_17_23_2_53_193 "__sprd_codec_ep_mute:%d"
#define SPRD_CODEC_PHY_131_112_2_17_23_2_53_194 "__sprd_codec_ext_pa_ctl:%d,%d"
#define SPRD_CODEC_PHY_137_112_2_17_23_2_53_195 "__sprd_codec_ext_hp_pa_ctl:%d"
#define SPRD_CODEC_PHY_150_112_2_17_23_2_53_196 "__sprd_codec_update_ldo_vol %d"
#define SPRD_CODEC_PHY_CFG_81_112_2_17_23_2_53_197 "[AUDDRV][SPRD_CODEC]real set pga 0x%x"
#define SPRD_CODEC_PHY_CFG_86_112_2_17_23_2_53_198 "[AUDDRV][SPRD_CODEC]find pga 0x%x"
#define SPRD_CODEC_PHY_CFG_107_112_2_17_23_2_53_199 "[AUDDRV][SPRD_CODEC]all pga_mode 0x%x"
#define SPRD_CODEC_PHY_CFG_112_112_2_17_23_2_53_200 "[AUDDRV][SPRD_CODEC]set pga_mode 0x%x"
#define SPRD_CODEC_PHY_CFG_156_112_2_17_23_2_53_201 "[AUDDRV][SPRD_CODEC]pga set"
#define SPRD_CODEC_AP_PHY_V0_234_112_2_17_23_2_54_202 "__sprd_codec_ap_en %d"
#define SPRD_CODEC_AP_PHY_V0_327_112_2_17_23_2_54_203 "__sprd_codec_ap_adc %d"
#define SPRD_CODEC_AP_PHY_V0_347_112_2_17_23_2_54_204 "__sprd_codec_ap_linein_rec %d"
#define SPRD_CODEC_AP_PHY_V0_355_112_2_17_23_2_54_205 "__sprd_codec_ap_dac_pwr %d"
#define SPRD_CODEC_AP_PHY_V0_361_112_2_17_23_2_54_206 "__sprd_codec_ap_dac %d"
#define SPRD_CODEC_AP_PHY_V0_416_112_2_17_23_2_54_207 "__sprd_codec_ap_lin_adc %d,%x"
#define SPRD_CODEC_AP_PHY_V0_470_112_2_17_23_2_54_208 "__sprd_codec_ap_lin %d"
#define SPRD_CODEC_AP_PHY_V0_510_112_2_17_23_2_55_209 "__sprd_codec_ap_ear %d"
#define SPRD_CODEC_AP_PHY_V0_518_112_2_17_23_2_55_210 "__sprd_codec_ap_lout %d"
#define SPRD_CODEC_AP_PHY_V0_526_112_2_17_23_2_55_211 "__sprd_codec_ap_ear_en %d"
#define SPRD_CODEC_AP_PHY_V0_587_112_2_17_23_2_55_212 "__sprd_codec_ap_hp %d"
#define SPRD_CODEC_AP_PHY_V0_593_112_2_17_23_2_55_213 "__sprd_codec_ap_hp_pop_en %d"
#define SPRD_CODEC_AP_PHY_V0_637_112_2_17_23_2_55_214 "__sprd_codec_ap_mic_bias %d"
#define SPRD_CODEC_AP_PHY_V0_652_112_2_17_23_2_55_215 "__sprd_codec_ap_main_mic %d"
#define SPRD_CODEC_AP_PHY_V0_664_112_2_17_23_2_55_216 "__sprd_codec_ap_aux_mic %d"
#define SPRD_CODEC_AP_PHY_V0_676_112_2_17_23_2_55_217 "__sprd_codec_ap_hp_mic %d"
#define SPRD_CODEC_AP_PHY_V0_696_112_2_17_23_2_55_218 "__sprd_codec_ap_dac_clk_en %d"
#define SPRD_CODEC_AP_PHY_V0_761_112_2_17_23_2_55_219 "__sprd_codec_ap_pa_dtri_f_sel %d"
#define SPRD_CODEC_AP_PHY_V0_770_112_2_17_23_2_55_220 "__sprd_codec_ap_pa_demi_en %d"
#define SPRD_CODEC_AP_PHY_V0_850_112_2_17_23_2_55_221 "0x%02x | 0x%02x 0x%02x 0x%02x 0x%02x"
#define SPRD_CODEC_DP_PHY_V0_251_112_2_17_23_2_56_222 "__sprd_codec_dp_dac_mute_div0 %d"
#define SPRD_CODEC_DP_PHY_V0_269_112_2_17_23_2_56_223 "__sprd_codec_dp_dac_mute_en %d"
#define SPRD_CODEC_DP_PHY_V0_484_112_2_17_23_2_56_224 "0x%02x | 0x%04x 0x%04x 0x%04x 0x%04x"
#define SPRD_CODEC_STATE_V0_425_112_2_17_23_2_57_225 "[AUDST]SPRD_CODEC_SM_Process"
#define SPRD_CODEC_STATE_V0_429_112_2_17_23_2_57_226 "[AUDST]arm not run"
#define SPRD_CODEC_STATE_V0_488_112_2_17_23_2_57_227 "[AUDST]CHANGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,ep=%d,mu=%d"
#define SPRD_CODEC_STATE_V0_707_112_2_17_23_2_58_228 "[AUDST]on_stop pre=%s,cur=%s"
#define SPRD_CODEC_STATE_V0_972_112_2_17_23_2_59_229 "[AUDDRV][SPRD]hp pop is up"
#define SPRD_CODEC_STATE_V0_984_112_2_17_23_2_59_230 "[AUDDRV][SPRD]RAMP UP"
#define SPRD_CODEC_STATE_V0_1030_112_2_17_23_2_59_231 "[AUDDRV][SPRD]hp pop is down"
#define SPRD_CODEC_STATE_V0_1042_112_2_17_23_2_59_232 "[AUDDRV][SPRD]RAMP DOWN"
#define SPRD_CODEC_STATE_V0_1239_112_2_17_23_2_59_233 "[AUDDRV][SPRD]UnMuted"
#define SPRD_CODEC_STATE_V0_1250_112_2_17_23_2_59_234 "[AUDDRV][SPRD]GAIN is UP"
#define SPRD_CODEC_STATE_V0_1283_112_2_17_23_2_59_235 "[AUDDRV][SPRD]Muted"
#define SPRD_CODEC_STATE_V0_1294_112_2_17_23_2_59_236 "[AUDDRV][SPRD]GAIN is DOWN"
#define SPRD_CODEC_STATE_V0_1649_112_2_17_23_3_0_237 "[AUDST]MERGED ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,ep=%d,mu=%d"
#define SPRD_CODEC_STATE_V0_1673_112_2_17_23_3_0_238 "[AUDST] MERGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,ep=%d,mu=%d"
#define SPRD_CODEC_STATE_V0_1689_112_2_17_23_3_0_239 "[AUDST] APPLY ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,ep=%d,mu=%d"
#define SPRD_CODEC_STATE_V0_1712_112_2_17_23_3_0_240 "[AUDST]change state pre=%s,cur=%s,set=%s"
#define SPRD_CODEC_STATE_V0_1926_112_2_17_23_3_0_241 "[AUDST]index=%d"
#define AUDIO_CODEC_USC15241_212_112_2_17_23_3_1_242 "[AUDDRV][USC15241]init usc15241"
#define AUDIO_CODEC_USC15241_236_112_2_17_23_3_1_243 "[AUDDRV][USC15241]open usc15241 0x%x,%d"
#define AUDIO_CODEC_USC15241_240_112_2_17_23_3_1_244 "[AUDDRV][USC15241]open err %08x"
#define AUDIO_CODEC_USC15241_270_112_2_17_23_3_1_245 "[AUDDRV][USC15241]close usc15241 0x%x"
#define AUDIO_CODEC_USC15241_274_112_2_17_23_3_1_246 "[AUDDRV][USC15241]close err %08x"
#define AUDIO_CODEC_USC15241_316_112_2_17_23_3_1_247 "[AUDDRV][USC15241]pga param"
#define AUDIO_CODEC_USC15241_342_112_2_17_23_3_1_248 "[AUDDRV][USC15241]drv out ctl 0x%x,%d"
#define AUDIO_CODEC_USC15241_371_112_2_17_23_3_1_249 "[AUDDRV][USC15241]drv out 0x%x"
#define AUDIO_CODEC_USC15241_378_112_2_17_23_3_1_250 "[AUDDRV][USC15241]drv out param"
#define AUDIO_CODEC_USC15241_413_112_2_17_23_3_2_251 "[AUDDRV][USC15241]drv in 0x%x"
#define AUDIO_CODEC_USC15241_420_112_2_17_23_3_2_252 "[AUDDRV][USC15241]drv in param"
#define AUDIO_CODEC_USC15241_443_112_2_17_23_3_2_253 "[AUDDRV][USC15241]dac mute"
#define AUDIO_CODEC_USC15241_460_112_2_17_23_3_2_254 "[AUDDRV][USC15241]get fs param"
#define AUDIO_CODEC_USC15241_480_112_2_17_23_3_2_255 "[AUDDRV][USC15241]ioctl usc15241 %d,0x%x"
#define AUDIO_CODEC_USC15241_506_112_2_17_23_3_2_256 "[AUDDRV][USC15241]ioctl param"
#define AUDIO_CODEC_USC15241_519_112_2_17_23_3_2_257 "[AUDDRV][USC15241]GOD %d,0x%x"
#define AUDIO_CODEC_USC15241_543_112_2_17_23_3_2_258 "[AUDDRV][USC15241]GO %d,0x%x"
#define AUDIO_CODEC_USC15241_566_112_2_17_23_3_2_259 "[AUDDRV][USC15241]GOBTL %d,0x%x"
#define AUDIO_CODEC_USC15241_581_112_2_17_23_3_2_260 "[AUDDRV][USC15241]GOB %d,0x%x"
#define AUDIO_CODEC_USC15241_604_112_2_17_23_3_2_261 "[AUDDRV][USC15241]GIM %d,0x%x"
#define AUDIO_CODEC_USC15241_618_112_2_17_23_3_2_262 "[AUDDRV][USC15241]GI %d,0x%x"
#define AUDIO_CODEC_USC15241_632_112_2_17_23_3_2_263 "[AUDDRV][USC15241]GOLR %d,0x%x"
#define AUDIO_CODEC_USC15241_706_112_2_17_23_3_2_264 "_run_callback:%d"
#define USC15241_124_112_2_17_23_3_4_265 "__usc15241_init"
#define USC15241_131_112_2_17_23_3_4_266 "__usc15241_en %d"
#define USC15241_187_112_2_17_23_3_4_267 "__usc15241_adc %d"
#define USC15241_264_112_2_17_23_3_4_268 "__usc15241_dac_pwr %d"
#define USC15241_279_112_2_17_23_3_4_269 "__usc15241_dac %d"
#define USC15241_361_112_2_17_23_3_4_270 "USC15241 I2C/SPI TEST START"
#define USC15241_369_112_2_17_23_3_4_271 "USE_I2C def reg(0x%x)=0x%x"
#define USC15241_382_112_2_17_23_3_4_272 "USE_SPI wr reg(0x%x)=0x%x"
#define USC15241_391_112_2_17_23_3_4_273 "USE_SPI wr2 reg(0x%x)=0x%x"
#define USC15241_400_112_2_17_23_3_4_274 "USE_I2C test FAILE!"
#define USC15241_404_112_2_17_23_3_4_275 "USE_SPI test FAILE!"
#define USC15241_420_112_2_17_23_3_4_276 "USC15241 I2C/SPI TEST END\r\n"
#define USC15241_431_112_2_17_23_3_4_277 "USC15241 REG"
#define USC15241_435_112_2_17_23_3_4_278 "I2C[0x%02x]=0x%x"
#define USC15241_441_112_2_17_23_3_4_279 "SPI[%04d]=0x%x"
#define USC15241_501_112_2_17_23_3_4_280 "__usc15241_ext_pa_ctl:%d"
#define USC15241_507_112_2_17_23_3_4_281 "__usc15241_ext_hp_pa_ctl:%d"
#define USC15241_PHY_CFG_77_112_2_17_23_3_5_282 "[AUDDRV][USC15241]real set pga 0x%x"
#define USC15241_PHY_CFG_82_112_2_17_23_3_5_283 "[AUDDRV][USC15241]find pga 0x%x"
#define USC15241_PHY_CFG_103_112_2_17_23_3_5_284 "[AUDDRV][USC15241]all pga_mode 0x%x"
#define USC15241_PHY_CFG_108_112_2_17_23_3_5_285 "[AUDDRV][USC15241]set pga_mode 0x%x"
#define USC15241_PHY_CFG_152_112_2_17_23_3_5_286 "[AUDDRV][USC15241]pga set"
#define USC15241_STATE_V0_414_112_2_17_23_3_6_287 "[USC15241ST]USC15241_SM_Process"
#define USC15241_STATE_V0_418_112_2_17_23_3_6_288 "[USC15241ST]arm not run"
#define USC15241_STATE_V0_476_112_2_17_23_3_6_289 "[USC15241ST]CHANGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
#define USC15241_STATE_V0_655_112_2_17_23_3_6_290 "[USC15241ST]on_stop pre=%s,cur=%s"
#define USC15241_STATE_V0_1058_112_2_17_23_3_7_291 "[AUDDRV][USC15241]UnMuted"
#define USC15241_STATE_V0_1069_112_2_17_23_3_7_292 "[AUDDRV][USC15241]GAIN is UP"
#define USC15241_STATE_V0_1103_112_2_17_23_3_7_293 "[AUDDRV][USC15241]Muted"
#define USC15241_STATE_V0_1114_112_2_17_23_3_7_294 "[AUDDRV][USC15241]GAIN is DOWN"
#define USC15241_STATE_V0_1432_112_2_17_23_3_8_295 "[USC15241ST]MERGED ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
#define USC15241_STATE_V0_1455_112_2_17_23_3_8_296 "[USC15241ST] MERGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
#define USC15241_STATE_V0_1470_112_2_17_23_3_8_297 "[USC15241ST] APPLY ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
#define USC15241_STATE_V0_1492_112_2_17_23_3_8_298 "[USC15241ST]change state pre=%s,cur=%s,set=%s"
#define USC15241_STATE_V0_1706_112_2_17_23_3_9_299 "[USC15241ST]index=%d"
#define AUDIO_DAI_VBC_266_112_2_17_23_3_10_300 "[AUDDRV][VBC]enable"
#define AUDIO_DAI_VBC_273_112_2_17_23_3_10_301 "[AUDDRV][VBC]vbc trigger,!open"
#define AUDIO_DAI_VBC_299_112_2_17_23_3_11_302 "[AUDDRV][VBC]da pLHead=0"
#define AUDIO_DAI_VBC_311_112_2_17_23_3_11_303 "[AUDDRV][VBC]da pRHead=0"
#define AUDIO_DAI_VBC_321_112_2_17_23_3_11_304 "[AUDDRV][VBC]da get buffer:chan=0x%x"
#define AUDIO_DAI_VBC_352_112_2_17_23_3_11_305 "[AUDDRV][VBC][REF]no data send"
#define AUDIO_DAI_VBC_356_112_2_17_23_3_11_306 "[AUDDRV][VBC]start send"
#define AUDIO_DAI_VBC_389_112_2_17_23_3_11_307 "[AUDDRV][VBC]dma send err"
#define AUDIO_DAI_VBC_405_112_2_17_23_3_11_308 "[AUDDRV][VBC]ad pLHead=0"
#define AUDIO_DAI_VBC_417_112_2_17_23_3_11_309 "[AUDDRV][VBC]ad pRHead=0"
#define AUDIO_DAI_VBC_427_112_2_17_23_3_11_310 "[AUDDRV][VBC]ad get buffer:chan=0x%x"
#define AUDIO_DAI_VBC_453_112_2_17_23_3_11_311 "[AUDDRV][VBC][REF]recv data not write"
#define AUDIO_DAI_VBC_457_112_2_17_23_3_11_312 "[AUDDRV][VBC]start recv"
#define AUDIO_DAI_VBC_488_112_2_17_23_3_11_313 "[AUDDRV][VBC]dma recv err"
#define AUDIO_DAI_VBC_529_112_2_17_23_3_11_314 "[AUDDRV][VBC]dma start:path=0x%x,chan=0x%x"
#define AUDIO_DAI_VBC_545_112_2_17_23_3_11_315 "[AUDDRV][VBC]dma start path=0x%x,xc0 %d,x1 %d"
#define AUDIO_DAI_VBC_591_112_2_17_23_3_11_316 "[AUDDRV][VBC]tried"
#define AUDIO_DAI_VBC_732_112_2_17_23_3_11_317 "[AUDDRV][VBC]base action 0x%x,%d,%d"
#define AUDIO_DAI_VBC_739_112_2_17_23_3_12_318 "[AUDDRV][VBC]dac0"
#define AUDIO_DAI_VBC_752_112_2_17_23_3_12_319 "[AUDDRV][VBC]dac1"
#define AUDIO_DAI_VBC_768_112_2_17_23_3_12_320 "[AUDDRV][VBC]adc0"
#define AUDIO_DAI_VBC_781_112_2_17_23_3_12_321 "[AUDDRV][VBC]adc1"
#define AUDIO_DAI_VBC_802_112_2_17_23_3_12_322 "[AUDDRV][VBC]dsp open, must close all other!"
#define AUDIO_DAI_VBC_811_112_2_17_23_3_12_323 "[AUDDRV][VBC]dsp, must close dsp,%08x"
#define AUDIO_DAI_VBC_824_112_2_17_23_3_12_324 "[AUDDRV][VBC]fm da , can't dsp,%08x"
#define AUDIO_DAI_VBC_833_112_2_17_23_3_12_325 "[AUDDRV][VBC]fm ad, can't dsp,%08x"
#define AUDIO_DAI_VBC_840_112_2_17_23_3_12_326 "[AUDDRV][VBC]fm ad , can't ad,%08x"
#define AUDIO_DAI_VBC_850_112_2_17_23_3_12_327 "[AUDDRV][VBC]ext action 0x%x,%d"
#define AUDIO_DAI_VBC_1002_112_2_17_23_3_12_328 "[AUDDRV][VBC]init vbc"
#define AUDIO_DAI_VBC_1044_112_2_17_23_3_12_329 "[AUDDRV][VBC]open vbc 0x%x,%d"
#define AUDIO_DAI_VBC_1048_112_2_17_23_3_12_330 "[AUDDRV][VBC]open err %08x"
#define AUDIO_DAI_VBC_1053_112_2_17_23_3_12_331 "[AUDDRV][VBC]real open vbc"
#define AUDIO_DAI_VBC_1083_112_2_17_23_3_12_332 "[AUDDRV][VBC]close vbc 0x%x"
#define AUDIO_DAI_VBC_1088_112_2_17_23_3_12_333 "[AUDDRV][VBC]close err %08x"
#define AUDIO_DAI_VBC_1105_112_2_17_23_3_12_334 "[AUDDRV][VBC]real close vbc"
#define AUDIO_DAI_VBC_1131_112_2_17_23_3_12_335 "[AUDDRV][VBC]ioctl vbc %d,0x%x"
#define AUDIO_DAI_VBC_1146_112_2_17_23_3_12_336 "[AUDDRV][VBC]da used"
#define AUDIO_DAI_VBC_1156_112_2_17_23_3_12_337 "[AUDDRV][VBC]da df err"
#define AUDIO_DAI_VBC_1166_112_2_17_23_3_12_338 "[AUDDRV][VBC]ad used"
#define AUDIO_DAI_VBC_1176_112_2_17_23_3_12_339 "[AUDDRV][VBC]ad df err"
#define AUDIO_DAI_VBC_1182_112_2_17_23_3_12_340 "[AUDDRV][VBC]set:dir err"
#define AUDIO_DAI_VBC_1198_112_2_17_23_3_12_341 "[AUDDRV][VBC]get:dir err"
#define AUDIO_DAI_VBC_1213_112_2_17_23_3_12_342 "[AUDDRV][VBC]set da buffer div:%d"
#define AUDIO_DAI_VBC_1219_112_2_17_23_3_13_343 "[AUDDRV][VBC]set ad buffer div:%d"
#define AUDIO_DAI_VBC_1297_112_2_17_23_3_13_344 "[AUDDRV][VBC]set dac buffer 0x%x,0x%x,%d"
#define AUDIO_DAI_VBC_1378_112_2_17_23_3_13_345 "[AUDDRV][VBC][REF]why trigger?"
#define AUDIO_DAI_VBC_1405_112_2_17_23_3_13_346 "[AUDDRV][VBC]set adc buffer 0x%x,0x%x,%d"
#define AUDIO_DAI_VBC_1464_112_2_17_23_3_13_347 "[AUDDRV][VBC]adc trigger"
#define AUDIO_DAI_VBC_1487_112_2_17_23_3_13_348 "[AUDDRV][VBC]register dac CB 0x%x"
#define AUDIO_DAI_VBC_1507_112_2_17_23_3_13_349 "[AUDDRV][VBC]register adc CB 0x%x"
#define AUDIO_DAI_VBC_1517_112_2_17_23_3_13_350 "[AUDDRV][VBC]codec notify %d"
#define AUDIO_DAI_VBC_1524_112_2_17_23_3_13_351 "[AUDDRV][VBC]switch to dsp"
#define AUDIO_DAI_VBC_1542_112_2_17_23_3_13_352 "[AUDDRV][VBC]close all"
#define AUDIO_DAI_VBC_1561_112_2_17_23_3_13_353 "[AUDDRV][VBC]codec notfiy err"
#define VBC_ALG_PHY_V3_420_112_2_17_23_3_15_354 "vbc_alg_phy_v3.c,[VB_PHY_SetFMMixMode] invalid da index :%d"
#define VBC_ALG_PHY_V3_458_112_2_17_23_3_15_355 "vbc_alg_phy_v3.c,[VB_PHY_DGSwitch] invalid da index :%d"
#define VBC_ALG_PHY_V3_495_112_2_17_23_3_15_356 "vbc_alg_phy_v3.c,[VB_PHY_SetDG] invalid da index :%d"
#define VBC_PHY_V0_419_112_2_17_23_3_16_357 "[AUDDRV][VBC] dac pls set cfg chan."
#define VBC_PHY_V0_451_112_2_17_23_3_16_358 "[AUDDRV][VBC] adc pls set cfg chan."
#define VBC_PHY_V0_484_112_2_17_23_3_16_359 "[AUDDRV][VBC] dac dma chan en fail."
#define VBC_PHY_V0_512_112_2_17_23_3_16_360 "[AUDDRV][VBC] adc dma chan en fail."
#define VBC_PHY_V3_435_112_2_17_23_3_18_361 "[AUDDRV][VBC] dac pls set cfg chan."
#define VBC_PHY_V3_467_112_2_17_23_3_18_362 "[AUDDRV][VBC] adc pls set cfg chan."
#define VBC_PHY_V3_500_112_2_17_23_3_18_363 "[AUDDRV][VBC] dac dma chan en fail."
#define VBC_PHY_V3_528_112_2_17_23_3_18_364 "[AUDDRV][VBC] adc dma chan en fail."
#define VBC_PHY_V5_439_112_2_17_23_3_19_365 "[AUDDRV][VBC] dac pls set cfg chan."
#define VBC_PHY_V5_471_112_2_17_23_3_19_366 "[AUDDRV][VBC] adc pls set cfg chan."
#define VBC_PHY_V5_504_112_2_17_23_3_19_367 "[AUDDRV][VBC] dac dma chan en fail."
#define VBC_PHY_V5_532_112_2_17_23_3_19_368 "[AUDDRV][VBC] adc dma chan en fail."
#define BUSMONITOR_HAL_85_112_2_17_23_3_19_369 "[BM DRV:]bm_monitor_set"
#define BUSMONITOR_HAL_149_112_2_17_23_3_19_370 "[BM DRV:]bm_test"
#define BUSMONITOR_HAL_153_112_2_17_23_3_19_371 "[BM DRV:]write begin"
#define BUSMONITOR_HAL_157_112_2_17_23_3_19_372 "[BM DRV:]write finish"
#define BUSMONITOR_PHY_V0_82_112_2_17_23_3_20_373 "[BM DRV:]bm_open"
#define BUSMONITOR_PHY_V0_108_112_2_17_23_3_20_374 "[BM DRV:]bm_clean_point"
#define BUSMONITOR_PHY_V0_218_112_2_17_23_3_20_375 "[BM DRV:]bm_set_point"
#define BUSMONITOR_PHY_V1_76_112_2_17_23_3_20_376 "[BM DRV:]bm_clk_eb"
#define BUSMONITOR_PHY_V1_99_112_2_17_23_3_20_377 "[BM DRV:]bm_open"
#define BUSMONITOR_PHY_V1_127_112_2_17_23_3_21_378 "[BM DRV:]bm_clean_point"
#define BUSMONITOR_PHY_V1_252_112_2_17_23_3_21_379 "[BM DRV:]bm_set_point"
#define BUSMONITOR_PHY_V3_79_112_2_17_23_3_21_380 "[BM DRV:]bm_clk_eb"
#define BUSMONITOR_PHY_V3_100_112_2_17_23_3_21_381 "[BM DRV:]bm_open"
#define BUSMONITOR_PHY_V3_127_112_2_17_23_3_21_382 "[BM DRV:]bm_clean_point"
#define BUSMONITOR_PHY_V3_245_112_2_17_23_3_22_383 "[BM DRV:]bm_set_point"
#define BUSMONITOR_PHY_V5_82_112_2_17_23_3_22_384 "[BM DRV:]bm_open"
#define BUSMONITOR_PHY_V5_109_112_2_17_23_3_22_385 "[BM DRV:]bm_clean_point"
#define BUSMONITOR_PHY_V5_219_112_2_17_23_3_22_386 "[BM DRV:]bm_set_point"
#define CHARGE_236_112_2_17_23_3_25_387 "CHGMNG:module_state.chgmng_state:%d"
#define CHARGE_237_112_2_17_23_3_25_388 "CHGMNG:module_state.bat_statistic_vol:%d"
#define CHARGE_238_112_2_17_23_3_25_389 "CHGMNG:module_state.bat_cur_vol:%d"
#define CHARGE_239_112_2_17_23_3_25_390 "CHGMNG:module_state.bat_remain_cap:%d"
#define CHARGE_240_112_2_17_23_3_25_391 "CHGMNG:x0_adc:%d,y0_vol:%d,slope:%d,zoom_in:%d"
#define CHARGE_244_112_2_17_23_3_25_392 "CHGMNG:module_state.charging_current:%d"
#define CHARGE_245_112_2_17_23_3_25_393 "CHGMNG:module_state.adp_type:%d"
#define CHARGE_246_112_2_17_23_3_25_394 "CHGMNG:module_state.charging_stop_reason:%d"
#define CHARGE_247_112_2_17_23_3_25_395 "CHGMNG:hw_switch_point:%d"
#define CHARGE_248_112_2_17_23_3_25_396 "CHGMNG:chg_end_vol:%d"
#define CHARGE_252_112_2_17_23_3_25_397 "CHGMNG:otp_type:%d, over_low:%d, over_high:%d,resume_low:%d,resume_high:%d,charging_temperature:%d"
#define CHARGE_257_112_2_17_23_3_25_398 "CHGMNG:module_state.bat_remain_cap:%d"
#define CHARGE_258_112_2_17_23_3_25_399 "CHGMNG:dischg_param.shutdown_vol:%d"
#define CHARGE_259_112_2_17_23_3_25_400 "CHGMNG:dischg_param.deadline_vol:%d"
#define CHARGE_260_112_2_17_23_3_25_401 "CHGMNG:dischg_param.warning_vol:%d"
#define CHARGE_262_112_2_17_23_3_25_402 "CHGMNG:....................vbat_queue.pointer:%d"
#define CHARGE_321_112_2_17_23_3_25_403 "CHGMNG:_CHGMNG_FSMProcess fsm_event:%d"
#define CHARGE_507_112_2_17_23_3_26_404 "CHGMNG:check_f_GSM_TX_on!"
#define CHARGE_516_112_2_17_23_3_26_405 "CHGMNG:vbat_result[%d]:%d"
#define CHARGE_533_112_2_17_23_3_26_406 "CHGMNG:_CHGMNG_GetVBATADCResult:%d"
#define CHARGE_542_112_2_17_23_3_26_407 "CHGMNG:_CHGMNG_GetVbatVol:%d"
#define CHARGE_699_112_2_17_23_3_26_408 "CHGMNG:%d has been send000!"
#define CHARGE_713_112_2_17_23_3_26_409 "CHGMNG:%d has been send2222!"
#define CHARGE_717_112_2_17_23_3_26_410 "CHGMNG:%d has been saved111!"
#define CHARGE_831_112_2_17_23_3_26_411 "CHGMNG:CHR_WARNING_IND!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#define CHARGE_847_112_2_17_23_3_26_412 "CHGMNG:CHR_WARNING_IND Before shutdown!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#define CHARGE_853_112_2_17_23_3_26_413 "CHGMNG:CHR_SHUTDOWN_IND!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#define CHARGE_902_112_2_17_23_3_27_414 "CHGMNG_CHARGING: ovp"
#define CHARGE_942_112_2_17_23_3_27_415 "CHGMNG_CHARGING: charger over power"
#define CHARGE_988_112_2_17_23_3_27_416 "CHGMNG:chg_current:%d"
#define CHARGE_993_112_2_17_23_3_27_417 "CHGMNG:STOP CHGMNG_VBATEND!"
#define CHARGE_1004_112_2_17_23_3_27_418 "CHGMNG:STOP CHGMNG_TIMEOUT!"
#define CHARGE_1039_112_2_17_23_3_27_419 "CHGMNG:UpdateSwitchoverPoint!"
#define CHARGE_1098_112_2_17_23_3_27_420 "CHGMNG:STOPPING! recharge:%d"
#define CHARGE_1117_112_2_17_23_3_27_421 "CHGMNG:OTP enable charger!"
#define CHARGE_1134_112_2_17_23_3_27_422 "CHGMNG:CHGMNG_ChargeStartHandler!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#define CHARGE_1145_112_2_17_23_3_27_423 "CHGMNG:CHR_CHARGE_START_IND!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#define CHARGE_1157_112_2_17_23_3_27_424 "CHGMNG:Invalid g_ovp_timer timer pointer!"
#define CHARGE_1167_112_2_17_23_3_27_425 "CHGMNG:Invalid application timer pointer!"
#define CHARGE_1200_112_2_17_23_3_27_426 "CHGMNG:Invalid application timer pointer!"
#define CHARGE_1209_112_2_17_23_3_27_427 "[Vcharge]CHGMNG:Invalid application timer pointer!"
#define CHARGE_1213_112_2_17_23_3_27_428 "CHGMNG:CHR_CHARGE_DISCONNECT1!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#define CHARGE_1220_112_2_17_23_3_27_429 "CHGMNG:CHR_CHARGE_FINISH!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#define CHARGE_1510_112_2_17_23_3_28_430 "CHGMNG:CHGMNG_VoltageToAdcvalue votage:%d adc:%d"
#define CHARGE_1619_112_2_17_23_3_28_431 "CHGMNG:CHGMNG_ModuleReset!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#define DUALBAT_HAL_103_112_2_17_23_3_29_432 "_DBAT_PrintTestLog start!!!"
#define DUALBAT_HAL_104_112_2_17_23_3_29_433 "DBAT:module state: %d"
#define DUALBAT_HAL_105_112_2_17_23_3_29_434 "DBAT:current bat: %d"
#define DUALBAT_HAL_106_112_2_17_23_3_29_435 "DBAT:current vol: %d"
#define DUALBAT_HAL_107_112_2_17_23_3_29_436 "DBAT:statistic_vol: %d"
#define DUALBAT_HAL_108_112_2_17_23_3_29_437 "DBAT:bat_remain_cap: %d"
#define DUALBAT_HAL_109_112_2_17_23_3_29_438 "DBAT:Idle bat: %d"
#define DUALBAT_HAL_110_112_2_17_23_3_29_439 "DBAT:main bat info vol: %d,cap: %d"
#define DUALBAT_HAL_111_112_2_17_23_3_29_440 "DBAT:aux bat info vol: %d,cap: %d"
#define DUALBAT_HAL_112_112_2_17_23_3_29_441 "_DBAT_PrintTestLog end!!!"
#define DUALBAT_HAL_205_112_2_17_23_3_29_442 "DBAT:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!SCI_SLEEP"
#define DUALBAT_HAL_255_112_2_17_23_3_29_443 "DBAT:!!!!!!!!on == record!!!!!!!!!!!record %d"
#define DUALBAT_HAL_261_112_2_17_23_3_29_444 "DBAT:-------on != record---------record %d"
#define DUALBAT_HAL_275_112_2_17_23_3_29_445 "DBAT:ChgFinish m_vol: %d,a_vol: %d"
#define DUALBAT_HAL_279_112_2_17_23_3_29_446 "DBAT_CHARGE_FINISH0"
#define DUALBAT_HAL_288_112_2_17_23_3_29_447 "DBAT_CHARGE_FINISH1"
#define DUALBAT_HAL_306_112_2_17_23_3_29_448 "DBAT_CHARGE_FINISH2"
#define DUALBAT_HAL_371_112_2_17_23_3_29_449 "DBAT:Shutdown3 m_vol: %d,a_vol: %d"
#define DUALBAT_HAL_379_112_2_17_23_3_29_450 "DBAT:Shutdown1 m_vol: %d,a_vol: %d"
#define DUALBAT_HAL_384_112_2_17_23_3_29_451 "DBAT:Shutdown2 m_vol: %d,a_vol: %d"
#define DUALBAT_HAL_398_112_2_17_23_3_29_452 "DBAT:Warning1 m_vol: %d,a_vol: %d"
#define DUALBAT_HAL_415_112_2_17_23_3_29_453 "DBAT:Warning2 m_vol: %d,a_vol: %d"
#define DUALBAT_HAL_430_112_2_17_23_3_29_454 "_DBAT_ChgStartHandler!!!"
#define DUALBAT_HAL_463_112_2_17_23_3_29_455 "_DBAT_ChgFaultHandler!!!"
#define DUALBAT_HAL_492_112_2_17_23_3_29_456 "DBAT:_DBAT_AdpPlugOutHandler!!!!!!!!!!!!!!!!!!!"
#define DUALBAT_HAL_521_112_2_17_23_3_29_457 "DBAT:Battery PLUGIN Again!!!!"
#define DUALBAT_HAL_538_112_2_17_23_3_30_458 "DBAT:Battery plugin:%d m_vol: %d,a_vol: %d......................"
#define DUALBAT_HAL_576_112_2_17_23_3_30_459 "DBAT:Battery PLUGOUT Again!!!!"
#define DUALBAT_HAL_582_112_2_17_23_3_30_460 "DBAT:Battery PLUGOUT INT DETECT!!!!"
#define DUALBAT_HAL_590_112_2_17_23_3_30_461 "DBAT:Battery PLUGOUT INT ERR!!!!"
#define DUALBAT_HAL_599_112_2_17_23_3_30_462 "DBAT:Battery plugout:%d m_vol: %d,a_vol: %d................................."
#define DUALBAT_HAL_629_112_2_17_23_3_30_463 "_DBAT_MutualChgRoutineInCharging: >>>>>>>>"
#define DUALBAT_HAL_640_112_2_17_23_3_30_464 "DBAT:_DBAT_MutualChgRoutineInCharging!!!!!!!_DBAT_SwitchBatHandler!!!!!!!!!!!bat:%d"
#define DUALBAT_HAL_736_112_2_17_23_3_30_465 "DBAT:Sw1 aim_bat:%d"
#define DUALBAT_HAL_746_112_2_17_23_3_30_466 "DBAT:Sw2 aim_bat:%d m_vol: %d,a_vol: %d"
#define DUALBAT_HAL_753_112_2_17_23_3_30_467 "DBAT:CHGMNG_SendMsgToChgTask CHGMNG_MODULE_RESET_MSG!"
#define DUALBAT_HAL_836_112_2_17_23_3_30_468 "DBAT: ADCDetect DBAT_A_BAT_PLUGIN_MSG"
#define DUALBAT_HAL_846_112_2_17_23_3_30_469 "DBAT: ADCDetect DBAT_M_BAT_PLUGIN_MSG"
#define DUALBAT_HAL_856_112_2_17_23_3_30_470 "DBAT: ADCDetect DBAT_A_BAT_PLUGOUT_MSG"
#define DUALBAT_HAL_862_112_2_17_23_3_30_471 "DBAT: not present A bat"
#define DUALBAT_HAL_873_112_2_17_23_3_30_472 "DBAT: ADCDetect DBAT_M_BAT_PLUGOUT_MSG"
#define DUALBAT_HAL_879_112_2_17_23_3_30_473 "DBAT: not present M bat"
#define DUALBAT_HAL_922_112_2_17_23_3_30_474 "DBAT:_DBAT_CHGMNGCallBack err msg!!!"
#define DUALBAT_HAL_963_112_2_17_23_3_30_475 "DBAT:_DBAT_GetVBatAdc:%d,bat:%d"
#define DUALBAT_HAL_978_112_2_17_23_3_30_476 "DBAT:_DBAT_AdcCvtToVol slope:%d,zoom_in:%d,x0_adc:%d,y0_vol:%d,adc:%d!"
#define DUALBAT_HAL_992_112_2_17_23_3_30_477 "DBAT:_DBAT_AdcCvtToVol use vbat!vbat_vol:%d"
#define DUALBAT_HAL_1061_112_2_17_23_3_31_478 "DBAT:_DBAT_BatInfoInit,bat:%d vol:%d"
#define DUALBAT_HAL_1079_112_2_17_23_3_31_479 "DBAT:state init"
#define DUALBAT_HAL_1091_112_2_17_23_3_31_480 "DBAT:1init SelectBat(DBAT_MAIN_BAT)"
#define DUALBAT_HAL_1103_112_2_17_23_3_31_481 "DBAT:2init SelectBat(DBAT_AUX_BAT)"
#define DUALBAT_HAL_1118_112_2_17_23_3_31_482 "DBAT:err: _DBAT_StateInit DBAT_MAIN_BAT"
#define DUALBAT_HAL_1127_112_2_17_23_3_31_483 "DBAT:err: _DBAT_StateInit DBAT_AUX_BAT"
#define DUALBAT_HAL_1211_112_2_17_23_3_31_484 "DBAT3:%d has been send!"
#define DUALBAT_HAL_1225_112_2_17_23_3_31_485 "DBAT1:%d has been saved!"
#define DUALBAT_HAL_1263_112_2_17_23_3_31_486 "DBAT:DBAT_Init"
#define CHG_PHY_V0_117_112_2_17_23_3_31_487 "CHGMNG:CHG_SetUSBChargeCurrent=%d"
#define CHG_PHY_V0_292_112_2_17_23_3_32_488 "CHG_UpdateSwitchoverPoint: chg_switchpoint = %d\n"
#define CHG_PHY_V0_309_112_2_17_23_3_32_489 "CHGMNG:CHG_PHY_SetChgCurrent=%d"
#define CHG_PHY_V3_116_112_2_17_23_3_33_490 "CHGMNG:CHG_SetUSBChargeCurrent=%d"
#define CHG_PHY_V3_215_112_2_17_23_3_33_491 "CHGMNG:CHG_StopRecharge"
#define CHG_PHY_V3_291_112_2_17_23_3_33_492 "CHG_UpdateSwitchoverPoint: chg_switchpoint = %d\n"
#define CHG_PHY_V3_312_112_2_17_23_3_33_493 "CHGMNG:CHG_PHY_SetChgCurrent=%d"
#define CHG_PHY_V5_114_112_2_17_23_3_33_494 "CHGMNG:CHG_StopRecharge"
#define CHG_PHY_V5_186_112_2_17_23_3_34_495 "CHG_UpdateSwitchoverPoint: chg_switchpoint = %d\n"
#define CHG_PHY_V5_207_112_2_17_23_3_34_496 "CHGMNG:CHG_PHY_SetChgCurrent=%d"
#define CHIP_PHY_V5_414_112_2_17_23_3_42_497 "%x, %x"
#define CLOCK_243_112_2_17_23_3_44_498 "[_CLOCK_SourceSet] 0x%x 0x%x"
#define CLOCK_269_112_2_17_23_3_44_499 "[DEVICE_ClockEnable] %s"
#define CLOCK_289_112_2_17_23_3_44_500 "[DEVICE_DisableClock] %s"
#define CLOCK_309_112_2_17_23_3_44_501 "[DEVICE_Reset] %s"
#define CLOCK_329_112_2_17_23_3_44_502 "[_DEVICE_Open] %s"
#define CLOCK_333_112_2_17_23_3_44_503 "[DEVICE_Open] The device has been opened"
#define CLOCK_376_112_2_17_23_3_44_504 "[DEVICE_Close] %s"
#define CLOCK_386_112_2_17_23_3_44_505 "[DEVICE_Close] Device has already been closed"
#define CLOCK_400_112_2_17_23_3_44_506 "[DEVICE_Close] Device %s was opened yet"
#define CLOCK_456_112_2_17_23_3_44_507 "[DEVICE_SetClock] Fail"
#define CLOCK_504_112_2_17_23_3_44_508 "[DEVICE_SetClock] Fail"
#define CLOCK_570_112_2_17_23_3_44_509 "[DEVICE_GetClock] %s"
#define CLOCK_695_112_2_17_23_3_45_510 "[_OpenClock] IN (ClkObj = 0x%x)\r\n"
#define CLOCK_699_112_2_17_23_3_45_511 "[_OpenClock] Clock has already been opened"
#define CLOCK_734_112_2_17_23_3_45_512 "[_CloseClock] IN (%s)\r\n"
#define CLOCK_740_112_2_17_23_3_45_513 "[_CloseClock] Clock has already been closed"
#define CLOCK_806_112_2_17_23_3_45_514 "[_DeviceObjectCreate] IN (pDev = 0x%x)\r\n"
#define CLOCK_815_112_2_17_23_3_45_515 "[_DeviceObjectCreate] <%s> has been added\r\n"
#define CLOCK_823_112_2_17_23_3_45_516 "[_DeviceObjectCreate] Fail to allocate memoryrn"
#define CLOCK_833_112_2_17_23_3_45_517 "[_DeviceObjectCreate] Fail to add device Name to "
#define SC6530_CLOCK_CFG_96_112_2_17_23_3_46_518 "[_SensorClkEnable]"
#define SC6530_CLOCK_CFG_106_112_2_17_23_3_46_519 "[_SensorClkDisable]"
#define SC6530_CLOCK_CFG_447_112_2_17_23_3_47_520 "[_RtcXtlEnable] default is open"
#define SC6530_CLOCK_CFG_455_112_2_17_23_3_47_521 "[_RtcXtlDisable] can't be closed."
#define SC6530_CLOCK_CFG_463_112_2_17_23_3_47_522 "[_MpllEnable] default is open"
#define SC6530_CLOCK_CFG_471_112_2_17_23_3_47_523 "[_MpllDisable] can't be closed."
#define SC6530_CLOCK_CFG_479_112_2_17_23_3_47_524 "[_26MXtlEnable] default is open"
#define SC6530_CLOCK_CFG_487_112_2_17_23_3_47_525 "[_26MXtlEnable] can't be closed."
#define SC6530_CLOCK_CFG_495_112_2_17_23_3_47_526 "[_TimerEnable]"
#define SC6530_CLOCK_CFG_506_112_2_17_23_3_47_527 "[_TimerDisable]"
#define SC6530_CLOCK_CFG_515_112_2_17_23_3_47_528 "[_TimerReset]"
#define SC6530_CLOCK_CFG_529_112_2_17_23_3_47_529 "[_TimerbkEnable]"
#define SC6530_CLOCK_CFG_540_112_2_17_23_3_47_530 "[_TimerbkDisable]"
#define SC6530_CLOCK_CFG_549_112_2_17_23_3_47_531 "[_TimerbkReset]"
#define SC6530_CLOCK_CFG_563_112_2_17_23_3_47_532 "[_KpdEnable]"
#define SC6530_CLOCK_CFG_574_112_2_17_23_3_47_533 "[_KpdDisable]"
#define SC6530_CLOCK_CFG_584_112_2_17_23_3_47_534 "[_KpdReset]"
#define SC6530_CLOCK_CFG_599_112_2_17_23_3_47_535 "[_IIC0Enable]"
#define SC6530_CLOCK_CFG_609_112_2_17_23_3_47_536 "[_IIC0Enable]"
#define SC6530_CLOCK_CFG_618_112_2_17_23_3_47_537 "[_IIC0Reset]"
#define SC6530_CLOCK_CFG_633_112_2_17_23_3_47_538 "[_IIC1Enable]"
#define SC6530_CLOCK_CFG_643_112_2_17_23_3_47_539 "[_IIC1Enable]"
#define SC6530_CLOCK_CFG_652_112_2_17_23_3_47_540 "[_IIC1Reset]"
#define SC6530_CLOCK_CFG_667_112_2_17_23_3_47_541 "[_SyscntEnable]"
#define SC6530_CLOCK_CFG_678_112_2_17_23_3_47_542 "[_SyscntDisable]"
#define SC6530_CLOCK_CFG_688_112_2_17_23_3_47_543 "[_SyscntReset]"
#define SC6530_CLOCK_CFG_703_112_2_17_23_3_47_544 "[_AdiEnable]"
#define SC6530_CLOCK_CFG_713_112_2_17_23_3_47_545 "[_AdiDisable]"
#define SC6530_CLOCK_CFG_722_112_2_17_23_3_47_546 "[_AdiReset]"
#define SC6530_CLOCK_CFG_736_112_2_17_23_3_47_547 "[_RtcEnable]"
#define SC6530_CLOCK_CFG_747_112_2_17_23_3_47_548 "[_RtcDisable]"
#define SC6530_CLOCK_CFG_757_112_2_17_23_3_47_549 "[_RtcReset]"
#define SC6530_CLOCK_CFG_771_112_2_17_23_3_47_550 "[_WdgEnable]"
#define SC6530_CLOCK_CFG_782_112_2_17_23_3_47_551 "[_WdgDisable]"
#define SC6530_CLOCK_CFG_792_112_2_17_23_3_47_552 "[_WdgReset]"
#define SC6530_CLOCK_CFG_806_112_2_17_23_3_47_553 "[_ChgrwdgEnable]"
#define SC6530_CLOCK_CFG_817_112_2_17_23_3_47_554 "[_ChgrwdgDisable]"
#define SC6530_CLOCK_CFG_827_112_2_17_23_3_47_555 "[_ChgrwdgReset]"
#define SC6530_CLOCK_CFG_842_112_2_17_23_3_47_556 "[_TpcEnable]"
#define SC6530_CLOCK_CFG_853_112_2_17_23_3_47_557 "[_TpcDisable]"
#define SC6530_CLOCK_CFG_863_112_2_17_23_3_47_558 "[_TpcReset]"
#define SC6530_CLOCK_CFG_877_112_2_17_23_3_47_559 "[_AdcEnable]"
#define SC6530_CLOCK_CFG_890_112_2_17_23_3_47_560 "[_AdcDisable]"
#define SC6530_CLOCK_CFG_903_112_2_17_23_3_47_561 "[_AdcReset]"
#define SC6530_CLOCK_CFG_917_112_2_17_23_3_47_562 "[_GpiodEnable]"
#define SC6530_CLOCK_CFG_929_112_2_17_23_3_47_563 "[_GpiodDisable]"
#define SC6530_CLOCK_CFG_940_112_2_17_23_3_47_564 "[_GpioReset]"
#define SC6530_CLOCK_CFG_955_112_2_17_23_3_47_565 "[_GpioaEnable]"
#define SC6530_CLOCK_CFG_966_112_2_17_23_3_47_566 "[_GpioDisable]"
#define SC6530_CLOCK_CFG_976_112_2_17_23_3_48_567 "[_GpioaReset]"
#define SC6530_CLOCK_CFG_991_112_2_17_23_3_48_568 "[_EicdEnable]"
#define SC6530_CLOCK_CFG_1003_112_2_17_23_3_48_569 "[_EicdDisable]"
#define SC6530_CLOCK_CFG_1014_112_2_17_23_3_48_570 "[_EicdReset]"
#define SC6530_CLOCK_CFG_1029_112_2_17_23_3_48_571 "[_EicaEnable]"
#define SC6530_CLOCK_CFG_1041_112_2_17_23_3_48_572 "[_EicaDisable]"
#define SC6530_CLOCK_CFG_1052_112_2_17_23_3_48_573 "[_EicaReset]"
#define SC6530_CLOCK_CFG_1067_112_2_17_23_3_48_574 "[_Sim0Enable]"
#define SC6530_CLOCK_CFG_1077_112_2_17_23_3_48_575 "[_Sim0Disable]"
#define SC6530_CLOCK_CFG_1086_112_2_17_23_3_48_576 "[_Sim0Reset]"
#define SC6530_CLOCK_CFG_1101_112_2_17_23_3_48_577 "[_GeaEnable]"
#define SC6530_CLOCK_CFG_1111_112_2_17_23_3_48_578 "[_GeaDisable]"
#define SC6530_CLOCK_CFG_1120_112_2_17_23_3_48_579 "[_GeaReset]"
#define SC6530_CLOCK_CFG_1135_112_2_17_23_3_48_580 "[_EfuseEnable]"
#define SC6530_CLOCK_CFG_1145_112_2_17_23_3_48_581 "[_EfuseDisable]"
#define SC6530_CLOCK_CFG_1155_112_2_17_23_3_48_582 "[_Spi0Enable]"
#define SC6530_CLOCK_CFG_1166_112_2_17_23_3_48_583 "[_SpiDisable]"
#define SC6530_CLOCK_CFG_1175_112_2_17_23_3_48_584 "[_Spi1Enable]"
#define SC6530_CLOCK_CFG_1185_112_2_17_23_3_48_585 "[_Spi1Disable]"
#define SC6530_CLOCK_CFG_1194_112_2_17_23_3_48_586 "[_Spi1Reset]"
#define SC6530_CLOCK_CFG_1209_112_2_17_23_3_48_587 "[_I2s0Enable]"
#define SC6530_CLOCK_CFG_1219_112_2_17_23_3_48_588 "[_I2s0Disable]"
#define SC6530_CLOCK_CFG_1228_112_2_17_23_3_48_589 "[_I2sReset]"
#define SC6530_CLOCK_CFG_1243_112_2_17_23_3_48_590 "[_Uart0Enable]"
#define SC6530_CLOCK_CFG_1253_112_2_17_23_3_48_591 "[_Uart0Disable]"
#define SC6530_CLOCK_CFG_1262_112_2_17_23_3_48_592 "[_Uart0Reset]"
#define SC6530_CLOCK_CFG_1277_112_2_17_23_3_48_593 "[_Uart1Enable]"
#define SC6530_CLOCK_CFG_1287_112_2_17_23_3_48_594 "[_Uart1Disable]"
#define SC6530_CLOCK_CFG_1296_112_2_17_23_3_48_595 "[_Uart1Reset]"
#define SC6530_CLOCK_CFG_1311_112_2_17_23_3_48_596 "[_PwmEnable]"
#define SC6530_CLOCK_CFG_1321_112_2_17_23_3_48_597 "[_PwmDisable]"
#define SC6530_CLOCK_CFG_1331_112_2_17_23_3_48_598 "[_PinEnable]"
#define SC6530_CLOCK_CFG_1342_112_2_17_23_3_48_599 "[_PinDisable]"
#define SC6530_CLOCK_CFG_1352_112_2_17_23_3_48_600 "[_PwmReset]"
#define SC6530_CLOCK_CFG_1367_112_2_17_23_3_48_601 "[_Aux0Enable]"
#define SC6530_CLOCK_CFG_1377_112_2_17_23_3_48_602 "[_Aux0Disable]"
#define SC6530_CLOCK_CFG_1386_112_2_17_23_3_48_603 "[_CcirReset]"
#define SC6530_CLOCK_CFG_1401_112_2_17_23_3_48_604 "[_DcamEnable]"
#define SC6530_CLOCK_CFG_1411_112_2_17_23_3_48_605 "[_DcamDisable]"
#define SC6530_CLOCK_CFG_1420_112_2_17_23_3_48_606 "[_DcamReset]"
#define SC6530_CLOCK_CFG_1435_112_2_17_23_3_48_607 "[_LzmaEnable]"
#define SC6530_CLOCK_CFG_1445_112_2_17_23_3_48_608 "[_LzmaDisable]"
#define SC6530_CLOCK_CFG_1454_112_2_17_23_3_48_609 "[_LzmaReset]"
#define SC6530_CLOCK_CFG_1468_112_2_17_23_3_48_610 "[_LcmEnable]"
#define SC6530_CLOCK_CFG_1478_112_2_17_23_3_48_611 "[_LcmDisable]"
#define SC6530_CLOCK_CFG_1487_112_2_17_23_3_48_612 "[_LcmReset]"
#define SC6530_CLOCK_CFG_1501_112_2_17_23_3_49_613 "[_LcdcEnable]"
#define SC6530_CLOCK_CFG_1511_112_2_17_23_3_49_614 "[_LcdcDisable]"
#define SC6530_CLOCK_CFG_1520_112_2_17_23_3_49_615 "[_LcdcReset]"
#define SC6530_CLOCK_CFG_1534_112_2_17_23_3_49_616 "[_RotaEnable]"
#define SC6530_CLOCK_CFG_1544_112_2_17_23_3_49_617 "[_RotaDisable]"
#define SC6530_CLOCK_CFG_1553_112_2_17_23_3_49_618 "[_RotaReset]"
#define SC6530_CLOCK_CFG_1567_112_2_17_23_3_49_619 "[_MidiEnable]"
#define SC6530_CLOCK_CFG_1577_112_2_17_23_3_49_620 "[_MidiDisable]"
#define SC6530_CLOCK_CFG_1586_112_2_17_23_3_49_621 "[_MidiReset]"
#define SC6530_CLOCK_CFG_1600_112_2_17_23_3_49_622 "[_TicEnable]"
#define SC6530_CLOCK_CFG_1610_112_2_17_23_3_49_623 "[_TicDisable]"
#define SC6530_CLOCK_CFG_1620_112_2_17_23_3_49_624 "[_SfcEnable]"
#define SC6530_CLOCK_CFG_1630_112_2_17_23_3_49_625 "[_SfcDisable]"
#define SC6530_CLOCK_CFG_1639_112_2_17_23_3_49_626 "[_SfcReset]"
#define SC6530_CLOCK_CFG_1652_112_2_17_23_3_49_627 "[_VspReset]"
#define SC6530_CLOCK_CFG_1666_112_2_17_23_3_49_628 "[_SdioEnable]"
#define SC6530_CLOCK_CFG_1676_112_2_17_23_3_49_629 "[_SdioDisable]"
#define SC6530_CLOCK_CFG_1685_112_2_17_23_3_49_630 "[_SdioReset]"
#define SC6530_CLOCK_CFG_1699_112_2_17_23_3_49_631 "[_UsbEnable]"
#define SC6530_CLOCK_CFG_1709_112_2_17_23_3_49_632 "[_UsbDisable]"
#define SC6530_CLOCK_CFG_1718_112_2_17_23_3_49_633 "[_UsbReset]"
#define SC6530_CLOCK_CFG_1732_112_2_17_23_3_49_634 "[_DmaEnable]"
#define SC6530_CLOCK_CFG_1742_112_2_17_23_3_49_635 "[_DmaDisable]"
#define SC6530_CLOCK_CFG_1751_112_2_17_23_3_49_636 "[_DmaReset]"
#define SC6530_CLOCK_CFG_1765_112_2_17_23_3_49_637 "[_Bm0Enable]"
#define SC6530_CLOCK_CFG_1775_112_2_17_23_3_49_638 "[_Bm0Disable]"
#define SC6530_CLOCK_CFG_1785_112_2_17_23_3_49_639 "[_Bm1Enable]"
#define SC6530_CLOCK_CFG_1795_112_2_17_23_3_49_640 "[_Bm1Disable]"
#define SC6530_CLOCK_CFG_1805_112_2_17_23_3_49_641 "[_Bm2Enable]"
#define SC6530_CLOCK_CFG_1815_112_2_17_23_3_49_642 "[_Bm2Disable]"
#define SC6530_CLOCK_CFG_1824_112_2_17_23_3_49_643 "[_AhbEnable] is NULL"
#define SC6530_CLOCK_CFG_1833_112_2_17_23_3_49_644 "[_AhbDisable] is NULL"
#define SC6530_CLOCK_CFG_1842_112_2_17_23_3_49_645 "[_ApbEnable] is NULL"
#define SC6530_CLOCK_CFG_1851_112_2_17_23_3_49_646 "[_ApbDisable] is NULL"
#define SC6530_CLOCK_CFG_1860_112_2_17_23_3_49_647 "[_EmcEnable]"
#define SC6530_CLOCK_CFG_1870_112_2_17_23_3_49_648 "[_AhbDisable]"
#define SC6530_CLOCK_CFG_1880_112_2_17_23_3_49_649 "[_EmcReset]"
#define SC6530_CLOCK_CFG_1896_112_2_17_23_3_49_650 "[_AudioEnable] IS NULL"
#define SC6530_CLOCK_CFG_1906_112_2_17_23_3_49_651 "[_AudioDisable] IS NULL"
#define SC6530_CLOCK_CFG_1914_112_2_17_23_3_49_652 "[_AudioReset] IS NULL"
#define SC6530_CLOCK_CFG_1922_112_2_17_23_3_49_653 "[_DummyEnable]"
#define SC6530_CLOCK_CFG_1931_112_2_17_23_3_49_654 "[_DummyDisable]"
#define SC6530_CLOCK_CFG_1940_112_2_17_23_3_49_655 "[_DummyReset]"
#define SC6600L_CLOCK_CFG_22_112_2_17_23_3_50_656 "[_WdgEnable]"
#define SC6600L_CLOCK_CFG_34_112_2_17_23_3_50_657 "[_WdgDisable]"
#define SC6600L_CLOCK_CFG_43_112_2_17_23_3_50_658 "[_TmrEnable]"
#define SC6600L_CLOCK_CFG_55_112_2_17_23_3_50_659 "[_TmrDisable]"
#define SC6600L_CLOCK_CFG_64_112_2_17_23_3_50_660 "[_RtcEnable]"
#define SC6600L_CLOCK_CFG_76_112_2_17_23_3_50_661 "[_RtcDisable]"
#define SC6600L_CLOCK_CFG_85_112_2_17_23_3_50_662 "[_KpdEnable]"
#define SC6600L_CLOCK_CFG_97_112_2_17_23_3_50_663 "[_KpdDisable]"
#define SC6600L_CLOCK_CFG_106_112_2_17_23_3_50_664 "[_SyscntEnable]"
#define SC6600L_CLOCK_CFG_118_112_2_17_23_3_50_665 "[_SyscntDisable]"
#define SC6600L_CLOCK_CFG_127_112_2_17_23_3_50_666 "[_TpcEnable]"
#define SC6600L_CLOCK_CFG_137_112_2_17_23_3_50_667 "[_TpcDisable]"
#define SC6600L_CLOCK_CFG_146_112_2_17_23_3_50_668 "[_GpioEnable]"
#define SC6600L_CLOCK_CFG_158_112_2_17_23_3_50_669 "[_GpioDisable]"
#define SC6600L_CLOCK_CFG_167_112_2_17_23_3_50_670 "[_IisEnable]"
#define SC6600L_CLOCK_CFG_178_112_2_17_23_3_50_671 "[_IisDisable]"
#define SC6600L_CLOCK_CFG_187_112_2_17_23_3_50_672 "[_AdcEnable]"
#define SC6600L_CLOCK_CFG_198_112_2_17_23_3_50_673 "[_AdcDisable]"
#define SC6600L_CLOCK_CFG_207_112_2_17_23_3_50_674 "[_SimEnable]"
#define SC6600L_CLOCK_CFG_218_112_2_17_23_3_50_675 "[_SimDisable]"
#define SC6600L_CLOCK_CFG_227_112_2_17_23_3_50_676 "[_GeaEnable]"
#define SC6600L_CLOCK_CFG_238_112_2_17_23_3_50_677 "[_GeaDisable]"
#define SC6600L_CLOCK_CFG_247_112_2_17_23_3_50_678 "[_I2cEnable]"
#define SC6600L_CLOCK_CFG_258_112_2_17_23_3_50_679 "[_I2cDisable]"
#define SC6600L_CLOCK_CFG_267_112_2_17_23_3_51_680 "[_PwmEnable]"
#define SC6600L_CLOCK_CFG_278_112_2_17_23_3_51_681 "[_PwmDisable]"
#define SC6600L_CLOCK_CFG_287_112_2_17_23_3_51_682 "[_IntEnable]"
#define SC6600L_CLOCK_CFG_299_112_2_17_23_3_51_683 "[_IntDisable]"
#define SC6600L_CLOCK_CFG_308_112_2_17_23_3_51_684 "[_Uart0Enable]"
#define SC6600L_CLOCK_CFG_319_112_2_17_23_3_51_685 "[_Uart0Disable]"
#define SC6600L_CLOCK_CFG_328_112_2_17_23_3_51_686 "[_Uart1Enable]"
#define SC6600L_CLOCK_CFG_339_112_2_17_23_3_51_687 "[_Uart1Disable]"
#define SC6600L_CLOCK_CFG_348_112_2_17_23_3_51_688 "[_VbcEnable]"
#define SC6600L_CLOCK_CFG_357_112_2_17_23_3_51_689 "[_VbcDisable]"
#define SC6600L_CLOCK_CFG_366_112_2_17_23_3_51_690 "[_Spi0Enable]"
#define SC6600L_CLOCK_CFG_377_112_2_17_23_3_51_691 "[_Spi0Disable]"
#define SC6600L_CLOCK_CFG_386_112_2_17_23_3_51_692 "[_Spi1Enable]"
#define SC6600L_CLOCK_CFG_397_112_2_17_23_3_51_693 "[_Spi1Disable]"
#define SC6600L_CLOCK_CFG_408_112_2_17_23_3_51_694 "[_DcamEnable]"
#define SC6600L_CLOCK_CFG_420_112_2_17_23_3_51_695 "[_DcamEnable]"
#define SC6600L_CLOCK_CFG_432_112_2_17_23_3_51_696 "[_UsbEnable]"
#define SC6600L_CLOCK_CFG_444_112_2_17_23_3_51_697 "[_UsbEnable]"
#define SC6600L_CLOCK_CFG_456_112_2_17_23_3_51_698 "[_UsbEnable]"
#define SC6600L_CLOCK_CFG_468_112_2_17_23_3_51_699 "[_UsbEnable]"
#define SC6600L_CLOCK_CFG_480_112_2_17_23_3_51_700 "[_UsbEnable]"
#define SC6600L_CLOCK_CFG_492_112_2_17_23_3_51_701 "[_UsbEnable]"
#define SC6600L_CLOCK_CFG_504_112_2_17_23_3_51_702 "[_UsbEnable]"
#define SC6600L_CLOCK_CFG_516_112_2_17_23_3_51_703 "[_UsbEnable]"
#define SC6600L_CLOCK_CFG_526_112_2_17_23_3_51_704 "[_AuxEnable]"
#define SC6600L_CLOCK_CFG_534_112_2_17_23_3_51_705 "[_AuxDisable]"
#define SC6600L_CLOCK_CFG_544_112_2_17_23_3_51_706 "[_AuxadcEnable]"
#define SC6600L_CLOCK_CFG_555_112_2_17_23_3_51_707 "[_AuxadcDisable]"
#define SC6600L_CLOCK_CFG_564_112_2_17_23_3_51_708 "[_VbEnable]"
#define SC6600L_CLOCK_CFG_573_112_2_17_23_3_51_709 "[_VbDisable]"
#define SC6600L_CLOCK_CFG_594_112_2_17_23_3_51_710 "[_VbEnable]"
#define SC6600L_CLOCK_CFG_601_112_2_17_23_3_51_711 "[_ApbDisable]"
#define SC6600L_CLOCK_CFG_613_112_2_17_23_3_51_712 "[_McuSrcSet]:--no divide."
#define SC6600L_CLOCK_CFG_656_112_2_17_23_3_51_713 "[_DspSrcSet]:---no divide."
#define SC6600L_CLOCK_CFG_698_112_2_17_23_3_51_714 "[_SrcSet]:--fixed with 26m, no divide"
#define SC6600L_CLOCK_CFG_719_112_2_17_23_3_51_715 "[_DcamSrcSet]:---no divide."
#define SC6600L_CLOCK_CFG_761_112_2_17_23_3_51_716 "[_Spi0SrcSet]:"
#define SC6600L_CLOCK_CFG_817_112_2_17_23_3_52_717 "[_Spi1SrcSet]:"
#define SC6600L_CLOCK_CFG_876_112_2_17_23_3_52_718 "[_IisSrcSet]"
#define SC6600L_CLOCK_CFG_952_112_2_17_23_3_52_719 "[_AuxSrcSet]:"
#define SC6600L_CLOCK_CFG_1019_112_2_17_23_3_52_720 "[_VbmclkSrcSet]:--fixed with 12M, no divide"
#define SC6600L_CLOCK_CFG_1042_112_2_17_23_3_52_721 "[_CcirmclkSrcSet]:"
#define SC6600L_CLOCK_CFG_1114_112_2_17_23_3_52_722 "[_SdioSrcSet]:"
#define SC6800H_CLOCK_CFG_678_112_2_17_23_3_55_723 "[_RtcxtlEnable]"
#define SC6800H_CLOCK_CFG_688_112_2_17_23_3_55_724 "[_RtcxtlDisable]"
#define SC6800H_CLOCK_CFG_698_112_2_17_23_3_55_725 "[_MpllEnable]"
#define SC6800H_CLOCK_CFG_712_112_2_17_23_3_55_726 "[_MpllDisable]"
#define SC6800H_CLOCK_CFG_728_112_2_17_23_3_55_727 "[_BpllEnable]"
#define SC6800H_CLOCK_CFG_753_112_2_17_23_3_55_728 "[_BpllDisable]"
#define SC6800H_CLOCK_CFG_774_112_2_17_23_3_55_729 "[_26MXtlEnable]"
#define SC6800H_CLOCK_CFG_783_112_2_17_23_3_55_730 "[_26MXtlEnable]"
#define SC6800H_CLOCK_CFG_792_112_2_17_23_3_55_731 "[_WdgEnable]"
#define SC6800H_CLOCK_CFG_803_112_2_17_23_3_55_732 "[_WdgDisable]"
#define SC6800H_CLOCK_CFG_813_112_2_17_23_3_55_733 "[_TimerEnable]"
#define SC6800H_CLOCK_CFG_824_112_2_17_23_3_55_734 "[_TimerDisable]"
#define SC6800H_CLOCK_CFG_834_112_2_17_23_3_55_735 "[_RtcEnable]"
#define SC6800H_CLOCK_CFG_845_112_2_17_23_3_55_736 "[_RtcDisable]"
#define SC6800H_CLOCK_CFG_855_112_2_17_23_3_55_737 "[_KpdEnable]"
#define SC6800H_CLOCK_CFG_866_112_2_17_23_3_55_738 "[_IIC0Enable]"
#define SC6800H_CLOCK_CFG_876_112_2_17_23_3_55_739 "[_IIC0Enable]"
#define SC6800H_CLOCK_CFG_890_112_2_17_23_3_55_740 "[_IIC0Enable]"
#define SC6800H_CLOCK_CFG_903_112_2_17_23_3_55_741 "[_IIC0Enable]"
#define SC6800H_CLOCK_CFG_914_112_2_17_23_3_55_742 "[_KpdDisable]"
#define SC6800H_CLOCK_CFG_925_112_2_17_23_3_55_743 "[_SyscntEnable]"
#define SC6800H_CLOCK_CFG_936_112_2_17_23_3_55_744 "[_SyscntDisable]"
#define SC6800H_CLOCK_CFG_946_112_2_17_23_3_55_745 "[_TpcEnable]"
#define SC6800H_CLOCK_CFG_957_112_2_17_23_3_55_746 "[_TpcDisable]"
#define SC6800H_CLOCK_CFG_967_112_2_17_23_3_55_747 "[_TpcEnable]"
#define SC6800H_CLOCK_CFG_977_112_2_17_23_3_55_748 "[_TpcDisable]"
#define SC6800H_CLOCK_CFG_987_112_2_17_23_3_55_749 "[_AdcReset]"
#define SC6800H_CLOCK_CFG_1005_112_2_17_23_3_55_750 "[_GpioEnable]"
#define SC6800H_CLOCK_CFG_1016_112_2_17_23_3_55_751 "[_GpioDisable]"
#define SC6800H_CLOCK_CFG_1026_112_2_17_23_3_55_752 "[_Sim0Enable]"
#define SC6800H_CLOCK_CFG_1036_112_2_17_23_3_55_753 "[_Sim0Disable]"
#define SC6800H_CLOCK_CFG_1046_112_2_17_23_3_55_754 "[_Sim1Enable]"
#define SC6800H_CLOCK_CFG_1056_112_2_17_23_3_56_755 "[_Sim1Disable]"
#define SC6800H_CLOCK_CFG_1066_112_2_17_23_3_56_756 "[_GeaEnable]"
#define SC6800H_CLOCK_CFG_1076_112_2_17_23_3_56_757 "[_GeaDisable]"
#define SC6800H_CLOCK_CFG_1086_112_2_17_23_3_56_758 "[_EfuseEnable]"
#define SC6800H_CLOCK_CFG_1096_112_2_17_23_3_56_759 "[_EfuseDisable]"
#define SC6800H_CLOCK_CFG_1106_112_2_17_23_3_56_760 "[_SpiEnable]"
#define SC6800H_CLOCK_CFG_1116_112_2_17_23_3_56_761 "[_SpiDisable]"
#define SC6800H_CLOCK_CFG_1126_112_2_17_23_3_56_762 "[_I2s0Enable]"
#define SC6800H_CLOCK_CFG_1137_112_2_17_23_3_56_763 "[_I2s0Disable]"
#define SC6800H_CLOCK_CFG_1147_112_2_17_23_3_56_764 "[_Uart0Enable]"
#define SC6800H_CLOCK_CFG_1157_112_2_17_23_3_56_765 "[_Uart0Disable]"
#define SC6800H_CLOCK_CFG_1167_112_2_17_23_3_56_766 "[_Pwm0Enable]"
#define SC6800H_CLOCK_CFG_1177_112_2_17_23_3_56_767 "[_Pwm0Disable]"
#define SC6800H_CLOCK_CFG_1187_112_2_17_23_3_56_768 "[_Pwm1Enable]"
#define SC6800H_CLOCK_CFG_1197_112_2_17_23_3_56_769 "[_Pwm1Disable]"
#define SC6800H_CLOCK_CFG_1207_112_2_17_23_3_56_770 "[_Pwm2Enable]"
#define SC6800H_CLOCK_CFG_1217_112_2_17_23_3_56_771 "[_Pwm2Disable]"
#define SC6800H_CLOCK_CFG_1227_112_2_17_23_3_56_772 "[_Pwm3Enable]"
#define SC6800H_CLOCK_CFG_1237_112_2_17_23_3_56_773 "[_Pwm3Disable]"
#define SC6800H_CLOCK_CFG_1247_112_2_17_23_3_56_774 "[_Pwm4Enable]"
#define SC6800H_CLOCK_CFG_1257_112_2_17_23_3_56_775 "[_Pwm4Disable]"
#define SC6800H_CLOCK_CFG_1267_112_2_17_23_3_56_776 "[_Aux0Enable]"
#define SC6800H_CLOCK_CFG_1277_112_2_17_23_3_56_777 "[_Aux0Disable]"
#define SC6800H_CLOCK_CFG_1287_112_2_17_23_3_56_778 "[_CcirEnable]"
#define SC6800H_CLOCK_CFG_1297_112_2_17_23_3_56_779 "[_CcirDisable]"
#define SC6800H_CLOCK_CFG_1308_112_2_17_23_3_56_780 "[_CcirReset]"
#define SC6800H_CLOCK_CFG_1325_112_2_17_23_3_56_781 "[_DcamEnable]"
#define SC6800H_CLOCK_CFG_1335_112_2_17_23_3_56_782 "[_DcamDisable]"
#define SC6800H_CLOCK_CFG_1346_112_2_17_23_3_56_783 "[_DcamReset]"
#define SC6800H_CLOCK_CFG_1363_112_2_17_23_3_56_784 "[_SdioEnable]"
#define SC6800H_CLOCK_CFG_1373_112_2_17_23_3_56_785 "[_SdioDisable]"
#define SC6800H_CLOCK_CFG_1384_112_2_17_23_3_56_786 "[_SdioReset]"
#define SC6800H_CLOCK_CFG_1401_112_2_17_23_3_56_787 "[_UsbEnable]"
#define SC6800H_CLOCK_CFG_1411_112_2_17_23_3_56_788 "[_UsbDisable]"
#define SC6800H_CLOCK_CFG_1422_112_2_17_23_3_56_789 "[_UsbReset]"
#define SC6800H_CLOCK_CFG_1439_112_2_17_23_3_56_790 "[_DmaEnable]"
#define SC6800H_CLOCK_CFG_1449_112_2_17_23_3_56_791 "[_DmaDisable]"
#define SC6800H_CLOCK_CFG_1460_112_2_17_23_3_56_792 "[_DmaReset]"
#define SC6800H_CLOCK_CFG_1478_112_2_17_23_3_56_793 "[_Bm0Enable]"
#define SC6800H_CLOCK_CFG_1488_112_2_17_23_3_56_794 "[_Bm0Disable]"
#define SC6800H_CLOCK_CFG_1499_112_2_17_23_3_56_795 "[_Bm0Reset]"
#define SC6800H_CLOCK_CFG_1516_112_2_17_23_3_56_796 "[_Bm1Enable]"
#define SC6800H_CLOCK_CFG_1526_112_2_17_23_3_56_797 "[_Bm1Disable]"
#define SC6800H_CLOCK_CFG_1537_112_2_17_23_3_56_798 "[_Bm1Reset]"
#define SC6800H_CLOCK_CFG_1554_112_2_17_23_3_56_799 "[_Bm2Enable]"
#define SC6800H_CLOCK_CFG_1564_112_2_17_23_3_56_800 "[_Bm2Disable]"
#define SC6800H_CLOCK_CFG_1575_112_2_17_23_3_56_801 "[_Bm2Reset]"
#define SC6800H_CLOCK_CFG_1591_112_2_17_23_3_57_802 "[_AhbEnable]"
#define SC6800H_CLOCK_CFG_1600_112_2_17_23_3_57_803 "[_AhbDisable]"
#define SC6800H_CLOCK_CFG_1609_112_2_17_23_3_57_804 "[_ApbEnable]"
#define SC6800H_CLOCK_CFG_1618_112_2_17_23_3_57_805 "[_ApbDisable]"
#define SC6800H_CLOCK_CFG_1629_112_2_17_23_3_57_806 "[_EmcReset]"
#define SC6800H_CLOCK_CFG_1649_112_2_17_23_3_57_807 "[_NfcReset]"
#define SC8800G_CLOCK_CFG_97_112_2_17_23_3_59_808 "[_DcamEnable]"
#define SC8800G_CLOCK_CFG_108_112_2_17_23_3_59_809 "[_DcamDisable]"
#define SC8800G_CLOCK_CFG_153_112_2_17_23_3_59_810 "[_VspEnable]"
#define SC8800G_CLOCK_CFG_163_112_2_17_23_3_59_811 "[_VspDisable]"
#define SC8800G_CLOCK_CFG_174_112_2_17_23_4_0_812 "[_VspReset]"
#define SC8800G_CLOCK_CFG_228_112_2_17_23_4_0_813 "[_LcdcEnable]"
#define SC8800G_CLOCK_CFG_239_112_2_17_23_4_0_814 "[_LcdcDisable]"
#define SC8800G_CLOCK_CFG_251_112_2_17_23_4_0_815 "[_LcdcReset]"
#define SC8800G_CLOCK_CFG_302_112_2_17_23_4_0_816 "[_SdioEnable]"
#define SC8800G_CLOCK_CFG_313_112_2_17_23_4_0_817 "[_SdioDisable]"
#define SC8800G_CLOCK_CFG_323_112_2_17_23_4_0_818 "[_SdioReset]"
#define SC8800G_CLOCK_CFG_383_112_2_17_23_4_0_819 "[_SpiEnable]"
#define SC8800G_CLOCK_CFG_392_112_2_17_23_4_0_820 "[_SpiDisable]"
#define SC8800G_CLOCK_CFG_438_112_2_17_23_4_0_821 "[_I2sEnable]"
#define SC8800G_CLOCK_CFG_447_112_2_17_23_4_0_822 "[_I2sDisable]"
#define SC8800G_CLOCK_CFG_457_112_2_17_23_4_0_823 "[_TpcEnable]"
#define SC8800G_CLOCK_CFG_468_112_2_17_23_4_0_824 "[_TpcDisable]"
#define SC8800G_CLOCK_CFG_477_112_2_17_23_4_0_825 "[_RtcEnable]"
#define SC8800G_CLOCK_CFG_487_112_2_17_23_4_0_826 "[_RtcDisable]"
#define SC8800G_CLOCK_CFG_497_112_2_17_23_4_0_827 "[_RtcTimer0Enable]"
#define SC8800G_CLOCK_CFG_506_112_2_17_23_4_0_828 "[_RtcTimer0Disable]"
#define SC8800G_CLOCK_CFG_517_112_2_17_23_4_0_829 "[_AdcEnable]"
#define SC8800G_CLOCK_CFG_528_112_2_17_23_4_0_830 "[_AdcDisable]"
#define SC8800G_CLOCK_CFG_538_112_2_17_23_4_0_831 "[_I2C0Enable]"
#define SC8800G_CLOCK_CFG_550_112_2_17_23_4_0_832 "[_I2C0Enable]"
#define SC8800G_CLOCK_CFG_560_112_2_17_23_4_0_833 "[_KpdDisable]"
#define SC8800G_CLOCK_CFG_570_112_2_17_23_4_0_834 "[_KpdDisable]"
#define SC8800G_CLOCK_CFG_580_112_2_17_23_4_0_835 "[_SyscntEnable]"
#define SC8800G_CLOCK_CFG_590_112_2_17_23_4_0_836 "[_SyscntDisable]"
#define SC8800G_CLOCK_CFG_637_112_2_17_23_4_0_837 "[_Uart0Enable]"
#define SC8800G_CLOCK_CFG_647_112_2_17_23_4_0_838 "[_Uart0Disable]"
#define SC8800G_CLOCK_CFG_694_112_2_17_23_4_1_839 "[_Uart1Enable]"
#define SC8800G_CLOCK_CFG_704_112_2_17_23_4_1_840 "[_Uart1Disable]"
#define SC8800G_CLOCK_CFG_751_112_2_17_23_4_1_841 "[_Uart2Enable]"
#define SC8800G_CLOCK_CFG_760_112_2_17_23_4_1_842 "[_Uart2Disable]"
#define SC8800G_CLOCK_CFG_790_112_2_17_23_4_1_843 "[_Pwm0Enable]"
#define SC8800G_CLOCK_CFG_799_112_2_17_23_4_1_844 "[_Pwm0Disable]"
#define SC8800G_CLOCK_CFG_828_112_2_17_23_4_1_845 "[_Pwm1Enable]"
#define SC8800G_CLOCK_CFG_837_112_2_17_23_4_1_846 "[_Pwm1Disable]"
#define SC8800G_CLOCK_CFG_866_112_2_17_23_4_1_847 "[_Pwm2Enable]"
#define SC8800G_CLOCK_CFG_875_112_2_17_23_4_1_848 "[_Pwm2Disable]"
#define SC8800G_CLOCK_CFG_904_112_2_17_23_4_1_849 "[_Pwm3Enable]"
#define SC8800G_CLOCK_CFG_913_112_2_17_23_4_1_850 "[_Pwm3Disable]"
#define SC8800G_CLOCK_CFG_959_112_2_17_23_4_1_851 "[_Aux0Enable]"
#define SC8800G_CLOCK_CFG_968_112_2_17_23_4_1_852 "[_Aux0Disable]"
#define SC8800G_CLOCK_CFG_1015_112_2_17_23_4_1_853 "[_Aux1Enable]"
#define SC8800G_CLOCK_CFG_1024_112_2_17_23_4_1_854 "[_Aux1Disable]"
#define SC8800G_CLOCK_CFG_1036_112_2_17_23_4_1_855 "[_Sim0Enable]"
#define SC8800G_CLOCK_CFG_1046_112_2_17_23_4_1_856 "[_Sim0Disable]"
#define SC8800G_CLOCK_CFG_1056_112_2_17_23_4_1_857 "[_Sim1Enable]"
#define SC8800G_CLOCK_CFG_1066_112_2_17_23_4_1_858 "[_Sim1Disable]"
#define SC8800G_CLOCK_CFG_1076_112_2_17_23_4_1_859 "[_EptEnable]"
#define SC8800G_CLOCK_CFG_1086_112_2_17_23_4_1_860 "[_EptDisable]"
#define SC8800G_CLOCK_CFG_1095_112_2_17_23_4_1_861 "[_DmaEnable]"
#define SC8800G_CLOCK_CFG_1105_112_2_17_23_4_1_862 "[_DmaEnable]"
#define SC8800G_CLOCK_CFG_1117_112_2_17_23_4_1_863 "[_DmaReset]"
#define SC8800G_CLOCK_CFG_1134_112_2_17_23_4_1_864 "[_NfcEnable]"
#define SC8800G_CLOCK_CFG_1144_112_2_17_23_4_1_865 "[_NfcDisable]"
#define SC8800G_CLOCK_CFG_1156_112_2_17_23_4_1_866 "[_DmaReset]"
#define SC8800G_CLOCK_CFG_1172_112_2_17_23_4_1_867 "[_Bm0Enable]"
#define SC8800G_CLOCK_CFG_1182_112_2_17_23_4_1_868 "[_Bm0Disable]"
#define SC8800G_CLOCK_CFG_1192_112_2_17_23_4_1_869 "[_Bm1Enable]"
#define SC8800G_CLOCK_CFG_1202_112_2_17_23_4_1_870 "[_Bm1Disable]"
#define SC8800G_CLOCK_CFG_1212_112_2_17_23_4_1_871 "[_RotEnable]"
#define SC8800G_CLOCK_CFG_1221_112_2_17_23_4_1_872 "[_RotDisable]"
#define SC8800G_CLOCK_CFG_1233_112_2_17_23_4_2_873 "[_RotReset]"
#define SC8800G_CLOCK_CFG_1268_112_2_17_23_4_2_874 "[_UsbEnable]"
#define SC8800G_CLOCK_CFG_1278_112_2_17_23_4_2_875 "[_UsbDisable]"
#define SC8800G_CLOCK_CFG_1290_112_2_17_23_4_2_876 "[_IspReset]"
#define SC8800G_CLOCK_CFG_1314_112_2_17_23_4_2_877 "[_CcirEnable]"
#define SC8800G_CLOCK_CFG_1324_112_2_17_23_4_2_878 "[_CcirDisable]"
#define SC8800G_CLOCK_CFG_1383_112_2_17_23_4_2_879 "[_CcirReset]"
#define SC8800G_CLOCK_CFG_1399_112_2_17_23_4_2_880 "[_AhbEnable]"
#define SC8800G_CLOCK_CFG_1408_112_2_17_23_4_2_881 "[_AhbDisable]"
#define SC8800G_CLOCK_CFG_1417_112_2_17_23_4_2_882 "[_ApbEnable]"
#define SC8800G_CLOCK_CFG_1426_112_2_17_23_4_2_883 "[_ApbDisable]"
#define SC8800G_CLOCK_CFG_1436_112_2_17_23_4_2_884 "[_WdgEnable]"
#define SC8800G_CLOCK_CFG_1446_112_2_17_23_4_2_885 "[_WdgDisable]"
#define SC8800G_CLOCK_CFG_1491_112_2_17_23_4_2_886 "[_PcmEnable]"
#define SC8800G_CLOCK_CFG_1505_112_2_17_23_4_2_887 "[_PcmDisable]"
#define SC8800G_CLOCK_CFG_1515_112_2_17_23_4_2_888 "[_GeaEnable]"
#define SC8800G_CLOCK_CFG_1525_112_2_17_23_4_2_889 "[_GeaDisable]"
#define SC8800G_CLOCK_CFG_1534_112_2_17_23_4_2_890 "[_AudioEnable]"
#define SC8800G_CLOCK_CFG_1545_112_2_17_23_4_2_891 "[_AudioDisable]"
#define SC8800H_CLOCK_CFG_24_112_2_17_23_4_3_892 "[_TpcEnable]"
#define SC8800H_CLOCK_CFG_36_112_2_17_23_4_4_893 "[_TpcDisable]"
#define SC8800H_CLOCK_CFG_45_112_2_17_23_4_4_894 "[_RtcEnable]"
#define SC8800H_CLOCK_CFG_54_112_2_17_23_4_4_895 "[_RtcDisable]"
#define SC8800H_CLOCK_CFG_63_112_2_17_23_4_4_896 "[_RtcTimer0Enable]"
#define SC8800H_CLOCK_CFG_72_112_2_17_23_4_4_897 "[_RtcTimer0Disable]"
#define SC8800H_CLOCK_CFG_83_112_2_17_23_4_4_898 "[_AdcEnable]"
#define SC8800H_CLOCK_CFG_93_112_2_17_23_4_4_899 "[_AdcDisable]"
#define SC8800H_CLOCK_CFG_103_112_2_17_23_4_4_900 "[_I2C0Enable]"
#define SC8800H_CLOCK_CFG_115_112_2_17_23_4_4_901 "[_I2C0Enable]"
#define SC8800H_CLOCK_CFG_125_112_2_17_23_4_4_902 "[_KpdDisable]"
#define SC8800H_CLOCK_CFG_135_112_2_17_23_4_4_903 "[_KpdDisable]"
#define SC8800H_CLOCK_CFG_145_112_2_17_23_4_4_904 "[_SyscntEnable]"
#define SC8800H_CLOCK_CFG_155_112_2_17_23_4_4_905 "[_SyscntDisable]"
#define SC8800H_CLOCK_CFG_165_112_2_17_23_4_4_906 "[_Uart0Enable]"
#define SC8800H_CLOCK_CFG_175_112_2_17_23_4_4_907 "[_Uart0Disable]"
#define SC8800H_CLOCK_CFG_185_112_2_17_23_4_4_908 "[_Sim0Enable]"
#define SC8800H_CLOCK_CFG_195_112_2_17_23_4_4_909 "[_Sim0Disable]"
#define SC8800H_CLOCK_CFG_205_112_2_17_23_4_4_910 "[_Sim1Enable]"
#define SC8800H_CLOCK_CFG_215_112_2_17_23_4_4_911 "[_Sim1Disable]"
#define SC8800H_CLOCK_CFG_225_112_2_17_23_4_4_912 "[_EptEnable]"
#define SC8800H_CLOCK_CFG_235_112_2_17_23_4_4_913 "[_EptDisable]"
#define SC8800H_CLOCK_CFG_246_112_2_17_23_4_4_914 "[_DmaEnable]"
#define SC8800H_CLOCK_CFG_258_112_2_17_23_4_4_915 "[_DmaEnable]"
#define SC8800H_CLOCK_CFG_270_112_2_17_23_4_4_916 "[_DmaReset]"
#define SC8800H_CLOCK_CFG_288_112_2_17_23_4_4_917 "[_LcmEnable]"
#define SC8800H_CLOCK_CFG_300_112_2_17_23_4_4_918 "[_LcmDisable]"
#define SC8800H_CLOCK_CFG_311_112_2_17_23_4_4_919 "[_WdgEnable]"
#define SC8800H_CLOCK_CFG_321_112_2_17_23_4_4_920 "[_WdgDisable]"
#define SC8800H_CLOCK_CFG_332_112_2_17_23_4_4_921 "[_NfcEnable]"
#define SC8800H_CLOCK_CFG_344_112_2_17_23_4_4_922 "[_NfcDisable]"
#define SC8800H_CLOCK_CFG_356_112_2_17_23_4_4_923 "[_DmaReset]"
#define SC8800H_CLOCK_CFG_374_112_2_17_23_4_4_924 "[_BmEnable]"
#define SC8800H_CLOCK_CFG_386_112_2_17_23_4_4_925 "[_Bm0Disable]"
#define SC8800H_CLOCK_CFG_399_112_2_17_23_4_4_926 "[_RotEnable]"
#define SC8800H_CLOCK_CFG_411_112_2_17_23_4_4_927 "[_RotDisable]"
#define SC8800H_CLOCK_CFG_423_112_2_17_23_4_4_928 "[_RotReset]"
#define SC8800H_CLOCK_CFG_441_112_2_17_23_4_4_929 "[_LcdcEnable]"
#define SC8800H_CLOCK_CFG_453_112_2_17_23_4_4_930 "[_LcdcDisable]"
#define SC8800H_CLOCK_CFG_465_112_2_17_23_4_4_931 "[_LcdcReset]"
#define SC8800H_CLOCK_CFG_483_112_2_17_23_4_4_932 "[_IspEnable]"
#define SC8800H_CLOCK_CFG_495_112_2_17_23_4_4_933 "[_IspDisable]"
#define SC8800H_CLOCK_CFG_507_112_2_17_23_4_4_934 "[_IspReset]"
#define SC8800H_CLOCK_CFG_525_112_2_17_23_4_4_935 "[_UsbEnable]"
#define SC8800H_CLOCK_CFG_537_112_2_17_23_4_4_936 "[_UsbDisable]"
#define SC8800H_CLOCK_CFG_549_112_2_17_23_4_4_937 "[_IspReset]"
#define SC8800H_CLOCK_CFG_575_112_2_17_23_4_5_938 "[_CcirEnable]"
#define SC8800H_CLOCK_CFG_587_112_2_17_23_4_5_939 "[_CcirDisable]"
#define SC8800H_CLOCK_CFG_599_112_2_17_23_4_5_940 "[_CcirReset]"
#define SC8800H_CLOCK_CFG_615_112_2_17_23_4_5_941 "[_AhbEnable]"
#define SC8800H_CLOCK_CFG_624_112_2_17_23_4_5_942 "[_AhbDisable]"
#define SC8800H_CLOCK_CFG_633_112_2_17_23_4_5_943 "[_ApbEnable]"
#define SC8800H_CLOCK_CFG_642_112_2_17_23_4_5_944 "[_ApbDisable]"
#define DMA_HAL_V1_237_112_2_17_23_4_7_945 "DMA_init\r\n"
#define DMA_HAL_V1_807_112_2_17_23_4_8_946 "DMA linkinit\r\n"
#define DMA_PHY_V1_109_112_2_17_23_4_11_947 "DMA_HalInit\r\n"
#define DMA_PHY_V1_157_112_2_17_23_4_11_948 "init enable flag 0x%08x\r\n"
#define EIC_PHY_V5_95_112_2_17_23_4_20_949 "EICDRV: EIC_PHY_Open"
#define EIC_PHY_V5_133_112_2_17_23_4_20_950 "EICDRV: EIC_PHY_Close"
#define EIC_PHY_V5_149_112_2_17_23_4_20_951 "EICDRV: EIC_PHY_IntEn[%d] is %d"
#define EIC_PHY_V5_182_112_2_17_23_4_20_952 "EICDRV: EIC_PHY_DbncEn[%d] is %d"
#define EIC_PHY_V5_209_112_2_17_23_4_20_953 "EICDRV: EIC_PHY_MskData[%d] "
#define EIC_PHY_V5_237_112_2_17_23_4_20_954 "EICDRV: EIC_PHY_GetData[%d] == %d"
#define EIC_PHY_V5_251_112_2_17_23_4_20_955 "EICDRV: EIC_PHY_SetTrg[%d] is %d"
#define EIC_PHY_V5_411_112_2_17_23_4_21_956 "EICDRV: EIC_PHY_StartTrg[%d]"
#define EIC_PHY_V5_432_112_2_17_23_4_21_957 "EICDRV: EIC_PHY_SetDbnc[%d] %d"
#define EIC_PHY_V5_542_112_2_17_23_4_21_958 "EICDRV: EIC_PHY_ClrIntSts[%d] "
#define EIC_PHY_V5_589_112_2_17_23_4_21_959 "EICD_DbncIsr chan[%d]'s callback is DONE."
#define EIC_PHY_V5_630_112_2_17_23_4_21_960 "EICA_DbncIsr chan[%d]'s callback is DONE."
#define EIC_PHY_V5_758_112_2_17_23_4_21_961 "EICD_Dbnc_RegCallback"
#define EIC_PHY_V5_766_112_2_17_23_4_21_962 "EICD_Dbnc_RegCallback-----callback is NULL."
#define EIC_PHY_V5_778_112_2_17_23_4_21_963 "EICA_Dbnc_RegCallback"
#define EIC_PHY_V5_786_112_2_17_23_4_21_964 "EICA_Dbnc_RegCallback-----callback is NULL."
#define FREQ_HAL_854_112_2_17_23_4_31_965 "chng_freq_c.c CHNG_FREQ_SetArmClk %d."
#define GEA_PHY_V0_332_112_2_17_23_4_35_966 "GEA Driver memory alloc failed!"
#define GEA_PHY_V0_388_112_2_17_23_4_35_967 "GEA Driver wait timeout:%ums!"
#define GEA_PHY_V1_186_112_2_17_23_4_36_968 " gea_stream:kc=0x%x0x%x0x%x0x%x,input=0x%x,ddir=%d,wh=%d,len=%d"
#define EPT_PHY_V3_645_112_2_17_23_4_38_969 "err: GEA1/2 length bigger than %d!!"
#define EPT_PHY_V3_655_112_2_17_23_4_38_970 "err: GEA3 length bigger than %d!!"
#define EPT_PHY_V3_665_112_2_17_23_4_38_971 "err: Kasumi length is not %d!!"
#define EPT_PHY_V3_675_112_2_17_23_4_38_972 "err: F8 length bigger than %d!!"
#define EPT_PHY_V3_685_112_2_17_23_4_38_973 "err: F9 length bigger than %d!!"
#define EPT_PHY_V3_896_112_2_17_23_4_38_974 "GEA Driver wait timeout:%ums!"
#define EPT_PHY_V3_1013_112_2_17_23_4_38_975 "GEA Driver wait timeout:%ums!"
#define GEA_PHY_V5_283_112_2_17_23_4_40_976 "GEADRV: memory alloc failed!"
#define GEA_PHY_V5_381_112_2_17_23_4_41_977 "GEADRV:...Polling Done-->"
#define GEA_PHY_V5_547_112_2_17_23_4_41_978 "GEA Driver wait timeout:%ums!"
#define GEA_PHY_V5_580_112_2_17_23_4_41_979 "GEADRV:...Dma Int Done-->"
#define GPIO_HAL_147_112_2_17_23_4_42_980 "GPIO_Init: Init GPIO !"
#define GPIO_HAL_190_112_2_17_23_4_42_981 "GPIO_DRIVER: GPIO_%d interrupt has not been registed!\n"
#define GPIO_HAL_304_112_2_17_23_4_42_982 "GPIO_GetGPIOState UnRegister interrupt for this pin."
#define GPIO_HAL_708_112_2_17_23_4_43_983 "GPIO_AddToIntTable: GPIO_%d has been added !\n"
#define GPIO_HAL_752_112_2_17_23_4_43_984 "[GPIO_DRV]GPIO_AddToIntTable: We strongly suggest that the deshaking for GPI should be less than 256!,otherwise refer to GPIO\n"
#define GPIO_HAL_829_112_2_17_23_4_43_985 "Can't delete deshaking timer of GPIO_%d!\n"
#define GPIO_HAL_875_112_2_17_23_4_44_986 "GPIO_SetInitState ERROR gpio_id=%d,i=%d"
#define GPIO_HAL_905_112_2_17_23_4_44_987 "GPIO_GetInitState ERROR gpio_id=%d,i=%d"
#define GPIO_HAL_989_112_2_17_23_4_44_988 "GPIO_DRIVER: GPIO_%d is shaking(INVALID)!\n"
#define GPIO_HAL_997_112_2_17_23_4_44_989 "GPIO_DRIVER: GPIO_%d state is %d!\n"
#define GPIO_HAL_1082_112_2_17_23_4_44_990 "GPIO_GetShakeInterval ERROR gpio_id=%d,i=%d"
#define GPIO_HAL_1119_112_2_17_23_4_44_991 "GPIO_GetCallBack ERROR gpio_id=%d,i=%d"
#define GPIO_PHY_V0_194_112_2_17_23_4_45_992 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V0_263_112_2_17_23_4_45_993 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V0_267_112_2_17_23_4_45_994 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V0_323_112_2_17_23_4_45_995 "[GPIO_DRV]this opretion can not belong to GPO"
#define GPIO_PHY_V0_331_112_2_17_23_4_45_996 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V0_431_112_2_17_23_4_45_997 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V0_458_112_2_17_23_4_45_998 "[GPIO_DRV]GPIO_SetDirection error"
#define GPIO_PHY_V0_468_112_2_17_23_4_45_999 "[GPIO_DRV]GPIO_SetDirection error"
#define GPIO_PHY_V0_478_112_2_17_23_4_45_1000 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V0_509_112_2_17_23_4_45_1001 "[GPIO_DRV]The corresponding reg of this GPIO_ID is a GPO! No Intr!"
#define GPIO_PHY_V0_516_112_2_17_23_4_45_1002 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V0_546_112_2_17_23_4_45_1003 "[GPIO_DRV]this opretion can not belong to GPO"
#define GPIO_PHY_V0_552_112_2_17_23_4_45_1004 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V0_589_112_2_17_23_4_46_1005 "This Intr sense type is invalid for GPI. \n"
#define GPIO_PHY_V0_597_112_2_17_23_4_46_1006 "[GPIO_DRV]this opretion can not belong to GPO"
#define GPIO_PHY_V0_636_112_2_17_23_4_46_1007 "This operation is invalid. \n"
#define GPIO_PHY_V0_641_112_2_17_23_4_46_1008 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V0_677_112_2_17_23_4_46_1009 "[GPIO_DRV]this opretion can not belong to SetGPIDebounce"
#define GPIO_PHY_V0_715_112_2_17_23_4_46_1010 "[GPIO_DRV]this opretion can not belong to GPO/GPIO"
#define GPIO_PHY_V0_719_112_2_17_23_4_46_1011 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V5_193_112_2_17_23_4_46_1012 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V5_262_112_2_17_23_4_47_1013 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V5_266_112_2_17_23_4_47_1014 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V5_322_112_2_17_23_4_47_1015 "[GPIO_DRV]this opretion can not belong to GPO"
#define GPIO_PHY_V5_330_112_2_17_23_4_47_1016 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V5_430_112_2_17_23_4_47_1017 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V5_457_112_2_17_23_4_47_1018 "[GPIO_DRV]GPIO_SetDirection error"
#define GPIO_PHY_V5_467_112_2_17_23_4_47_1019 "[GPIO_DRV]GPIO_SetDirection error"
#define GPIO_PHY_V5_477_112_2_17_23_4_47_1020 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V5_508_112_2_17_23_4_47_1021 "[GPIO_DRV]The corresponding reg of this GPIO_ID is a GPO! No Intr!"
#define GPIO_PHY_V5_515_112_2_17_23_4_47_1022 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V5_545_112_2_17_23_4_47_1023 "[GPIO_DRV]this opretion can not belong to GPO"
#define GPIO_PHY_V5_551_112_2_17_23_4_47_1024 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V5_588_112_2_17_23_4_47_1025 "This Intr sense type is invalid for GPI. \n"
#define GPIO_PHY_V5_596_112_2_17_23_4_47_1026 "[GPIO_DRV]this opretion can not belong to GPO"
#define GPIO_PHY_V5_635_112_2_17_23_4_47_1027 "This operation is invalid. \n"
#define GPIO_PHY_V5_640_112_2_17_23_4_47_1028 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define GPIO_PHY_V5_676_112_2_17_23_4_47_1029 "[GPIO_DRV]this opretion can not belong to SetGPIDebounce"
#define GPIO_PHY_V5_714_112_2_17_23_4_48_1030 "[GPIO_DRV]this opretion can not belong to GPO/GPIO"
#define GPIO_PHY_V5_718_112_2_17_23_4_48_1031 "[GPIO_DRV]the GPIO_ID is Invalid in this chip"
#define I2C_PHY_V0_293_112_2_17_23_4_51_1032 "[IIC DRV:]__I2C_PHY_SetPort: Fatal error, if you use qwerty keypad, i2c fun of multiplex pins are disabled"
#define INT_HAL_583_112_2_17_23_4_57_1033 "IRQ HAS USED, intLogicNum = 0x%xn"
#define INT_HAL_599_112_2_17_23_4_57_1034 "FIQ HAS USED, intLogicNum = 0x%xn"
#define INT_HAL_HISR_149_112_2_17_23_4_57_1035 "CHIPDRVHISR_WaitActive:Get Semaphore Fail!"
#define INT_HAL_HISR_172_112_2_17_23_4_57_1036 "CHIPDRVHISR_Activate:put Semaphore Fail!"
#define KPD_PHY_V0_419_112_2_17_23_5_3_1037 "KPD_DRIVER: Key code %d is UP!\n"
#define KPD_PHY_V5_420_112_2_17_23_5_4_1038 "KPD_DRIVER: Key code %d is UP!\n"
#define LCM_PHY_V0_631_112_2_17_23_5_6_1039 "_LCM_WaitDMA_AllLinkDone: time out !!"
#define LCM_PHY_V0_773_112_2_17_23_5_7_1040 "LCM_DmaWrite(): rot_angele=%d, brush_mode=%d"
#define LCM_PHY_V0_774_112_2_17_23_5_7_1041 "LCM_DmaWrite(): src_addr=0x%x, src_w=%d,src_h=%d,buf_width=%d"
#define LCM_PHY_V0_775_112_2_17_23_5_7_1042 "LCM_DmaWrite(): in_timer=%d"
#define LCM_PHY_V0_968_112_2_17_23_5_7_1043 "LCM: w = %d, h = %d, rotate=%d"
#define LCM_PHY_V1_228_112_2_17_23_5_8_1044 "_LCM_WaitDMADone: time out !!"
#define LCD_IF_SPI_450_112_2_17_23_5_11_1045 "---FIFOFULL:rxt_cnt=0x%x"
#define LCD_IF_SPI_455_112_2_17_23_5_11_1046 "---FIFOFULL: SPI_ReadData =0x%x"
#define LCD_IF_SPI_469_112_2_17_23_5_11_1047 "---Finish: SPI_ReadData =0x%x"
#define LCD_IF_SPI_627_112_2_17_23_5_11_1048 "SPI_HAL_Open(): SUCCESS!"
#define LCD_IF_SPI_840_112_2_17_23_5_12_1049 "_LCM_WaitDMADone: time out !!"
#define LCD_IF_SPI_917_112_2_17_23_5_12_1050 "LCM:w = %d,h = %d,bufw=%d rotate=%d"
#define LCD_IF_SPI_1086_112_2_17_23_5_12_1051 "LCM:DMA_channel=%d"
#define LCM_PHY_V5_98_112_2_17_23_5_13_1052 "_lcm_set_buswidth: the cs_pin value of lcd_spec_ptr should not large than 4.."
#define LCM_PHY_V5_262_112_2_17_23_5_13_1053 "_LCM_WaitDMADone: time out !!"
#define LCM_PHY_V5_375_112_2_17_23_5_13_1054 "LCM_SetParam: the cs_pin value of lcd_spec_ptr should not large than 4.."
#define LCM_PHY_V5_471_112_2_17_23_5_13_1055 "LCM:w = %d,h = %d,bufw=%d rotate=%d"
#define LCM_PHY_V5_640_112_2_17_23_5_14_1056 "LCM:DMA_channel=%d"
#define LCM_PHY_V5_659_112_2_17_23_5_14_1057 "LCM_DmaWrite(): out_timer=%d"
#define LDO_HAL_309_112_2_17_23_5_15_1058 "VCAMPad is not opened!"
#define LZMA_PHY_V5_259_112_2_17_23_5_17_1059 "LZMA_WaitingFinish: time out !!"
#define LZMA_TEST_101_112_2_17_23_5_17_1060 "_lzma_int_finish_callback is called."
#define LZMA_TEST_119_112_2_17_23_5_17_1061 "LZMA_CheckMem:-->src_addr[0x%x],dst_addr[0x%x]"
#define LZMA_TEST_147_112_2_17_23_5_17_1062 "LZMA_IntstsTest:--------------->start"
#define LZMA_TEST_168_112_2_17_23_5_17_1063 "LZMA_IntstsTest:-->ERROR,LZMA_INT_FINISH"
#define LZMA_TEST_171_112_2_17_23_5_17_1064 "LZMA_IntstsTest:-->ERROR,LZMA_INT_BUF_LEN_ERR"
#define LZMA_TEST_195_112_2_17_23_5_18_1065 "LZMA_IntstsTest:-->ERROR,LZMA_INT_FINISH"
#define LZMA_TEST_198_112_2_17_23_5_18_1066 "LZMA_IntstsTest:-->ERROR,LZMA_INT_DST_LEN_ERR"
#define LZMA_TEST_219_112_2_17_23_5_18_1067 "LZMA_IntstsTest:-->ERROR,LZMA_INT_FINISH"
#define LZMA_TEST_222_112_2_17_23_5_18_1068 "LZMA_IntstsTest:-->ERROR,LZMA_INT_DEC_ERR"
#define LZMA_TEST_226_112_2_17_23_5_18_1069 "LZMA_IntstsTest:--------------->end"
#define LZMA_TEST_239_112_2_17_23_5_18_1070 "LZMA_PollingTest:-->buf_en=%d, buf_full_en=%d"
#define LZMA_TEST_275_112_2_17_23_5_18_1071 "--------->Case[%d] succuss."
#define LZMA_TEST_290_112_2_17_23_5_18_1072 "LZMA_IntTest:-->buf_en=%d, buf_full_en=%d"
#define LZMA_TEST_336_112_2_17_23_5_18_1073 "LZMA_Case0Test:-->pattern[%d] is error"
#define LZMA_TEST_343_112_2_17_23_5_18_1074 "--------->Case[%d] succuss."
#define LZMA_TEST_372_112_2_17_23_5_18_1075 "test entry"
#define RETARGET_141_112_2_17_23_5_20_1076 "DIV 0 error, Return address 0x%X"
#define RETARGET_153_112_2_17_23_5_20_1077 "Stack Over flow, return address 0x%X, Thread ID:%d, Thread Name:%s"
#define SIO_1443_112_2_17_23_5_24_1078 "SIO_FreeUartDmaChannel, channel is empty\n"
#define SIO_3619_112_2_17_23_5_28_1079 "@@@@send tick used %d"
#define SIO_4032_112_2_17_23_5_29_1080 "SIO writecmdres, mutex operate error\n"
#define SIO_4174_112_2_17_23_5_29_1081 "Length 1 of Tx buffer is not long enought.the buffer size is %d , the space is %d \n"
#define SIO_4186_112_2_17_23_5_29_1082 "Length 2 of Tx buffer is not long enought. the buffer size is %d , the space is %d \n"
#define SIO_4448_112_2_17_23_5_30_1083 "SIO_ATC_WriteCmdRes -gprs_debug=0x%X\r\n"
#define SIO_4455_112_2_17_23_5_30_1084 "SIO: discard AT response in DATA_MODE \n"
#define SIO_4462_112_2_17_23_5_30_1085 "SIO_ATC_WriteCmdRes \n"
#define SIO_4470_112_2_17_23_5_30_1086 "SIO_ATC_WriteCmdRes : uart_mix_sem_take \n"
#define SIO_4785_112_2_17_23_5_30_1087 "jim: in SIO_SendDataPacket : uart_mix_check \n"
#define SIO_4791_112_2_17_23_5_30_1088 "jim: in SIO_SendDataPacket : uart_mix_sem_take \n"
#define SIO_5054_112_2_17_23_5_31_1089 "sio:datalen = %d"
#define SIO_5060_112_2_17_23_5_31_1090 "sio:reset mix 1."
#define SIO_5071_112_2_17_23_5_31_1091 "sio:reset mix 2."
#define SIO_5079_112_2_17_23_5_31_1092 "sio:reset mix 3."
#define SIO_5097_112_2_17_23_5_31_1093 "mix mode data type error."
#define SIO_5157_112_2_17_23_5_31_1094 "MIX SIO: buffer full!"
#define SIO_5262_112_2_17_23_5_31_1095 "SIO: discard the pending AT packets."
#define SIO_5323_112_2_17_23_5_31_1096 "SIO SIO_SetUserPortMode error, current mode is DATA_MODE\n"
#define SIO_5910_112_2_17_23_5_32_1097 "Software flow control:send XOFF"
#define SIO_6483_112_2_17_23_5_34_1098 "@@@@tick used %d"
#define SIO_6488_112_2_17_23_5_34_1099 "@@@@tick used %d"
#define SIO_6496_112_2_17_23_5_34_1100 "@@@@tick used %d"
#define SIO_6507_112_2_17_23_5_34_1101 "@@@@tick used %d"
#define SIO_6517_112_2_17_23_5_34_1102 "@@@@tick used %d"
#define SIO_6521_112_2_17_23_5_34_1103 "@@@@max tick used %d"
#define THREADX_ASSERT_3534_112_2_17_23_5_41_1104 "LOGSAVE: CHIPDRV SCI_PAssert"
#define MMU_BZP_952_112_2_17_23_5_47_1105 "mmu_help_set_TEX error\n %d, %d, %d, %d"
#define MMU_BZP_997_112_2_17_23_5_47_1106 "BUG: can not be mapped using pages, ignoring.addr = 0x%x, phy = 0x%x, length =0x%x\n"
#define FLASH_1000_112_2_17_23_5_58_1107 "Flash:Get Semaphore Fail!"
#define PCM_DRV_V0_590_112_2_17_23_6_0_1108 "PCM Parameter Error\r\n"
#define PCM_DRV_V0_631_112_2_17_23_6_0_1109 "PCM_Open End\r\n"
#define PCM_DRV_V0_662_112_2_17_23_6_0_1110 "==PCM Close===\r\n"
#define PCM_DRV_V1_618_112_2_17_23_6_2_1111 "PCM Parameter Error\r\n"
#define PCM_DRV_V1_645_112_2_17_23_6_2_1112 "PCM_Open End\r\n"
#define PCM_DRV_V1_676_112_2_17_23_6_2_1113 "==PCM Close===\r\n"
#define PCM_DRV_V5_608_112_2_17_23_6_3_1114 "PCM Parameter Error\r\n"
#define PCM_DRV_V5_651_112_2_17_23_6_3_1115 "PCM_Open End\r\n"
#define PCM_DRV_V5_684_112_2_17_23_6_3_1116 "==PCM Close===\r\n"
#define SDHOST_PHY_552_112_2_17_23_6_15_1117 "   clkDiv: %d"
#define SDHOST_PHY_559_112_2_17_23_6_15_1118 "   clkDiv: %d is too big!!!!!"
#define SDHOST_PHY_615_112_2_17_23_6_15_1119 "sd clk: %d KHz."
#define SDHOST_PHY_1283_112_2_17_23_6_16_1120 "capbility_function = %x"
#define SDHOST_PHY_1284_112_2_17_23_6_16_1121 "cability_Max_BlkLen = %x"
#define SDHOST_PHY_1285_112_2_17_23_6_16_1122 "sd_Base_Max_Clk = %x"
#define SDHOST_PHY_1286_112_2_17_23_6_16_1123 "timeOut_Clk_unit = %x"
#define SDHOST_PHY_1287_112_2_17_23_6_16_1124 "timeOut_Base_Clk = %x"
#define SDHOST_PHY_1288_112_2_17_23_6_16_1125 "max_current_for_1_8 = %x"
#define SDHOST_PHY_1289_112_2_17_23_6_16_1126 "max_current_for_3_0 = %x"
#define SDHOST_PHY_1290_112_2_17_23_6_16_1127 "max_current_for_3_3 = %x"
#define SDHOST_PHY_V5_443_112_2_17_23_6_19_1128 "   clkDiv: %d"
#define SDHOST_PHY_V5_450_112_2_17_23_6_19_1129 "   clkDiv: %d is too big!!!!!"
#define SDHOST_PHY_V5_506_112_2_17_23_6_20_1130 "sd clk: %d KHz."
#define SDHOST_PHY_V5_1175_112_2_17_23_6_21_1131 "capbility_function = %x"
#define SDHOST_PHY_V5_1176_112_2_17_23_6_21_1132 "cability_Max_BlkLen = %x"
#define SDHOST_PHY_V5_1177_112_2_17_23_6_21_1133 "sd_Base_Max_Clk = %x"
#define SDHOST_PHY_V5_1178_112_2_17_23_6_21_1134 "timeOut_Clk_unit = %x"
#define SDHOST_PHY_V5_1179_112_2_17_23_6_21_1135 "timeOut_Base_Clk = %x"
#define SDHOST_PHY_V5_1180_112_2_17_23_6_21_1136 "max_current_for_1_8 = %x"
#define SDHOST_PHY_V5_1181_112_2_17_23_6_21_1137 "max_current_for_3_0 = %x"
#define SDHOST_PHY_V5_1182_112_2_17_23_6_21_1138 "max_current_for_3_3 = %x"
#define SIM_DRV_347_112_2_17_23_6_26_1139 "SIMDRV%d:init sim drv"
#define SIM_DRV_1177_112_2_17_23_6_28_1140 "\r\nSIMDRV%d::set the working volt to 1.8v."
#define SIM_DRV_1182_112_2_17_23_6_28_1141 "\r\nSIMDRV%d::set the working volt to 3.0v."
#define SIM_DRV_1187_112_2_17_23_6_28_1142 "\r\nSIMDRV%d::do not change the working volt."
#define SIM_DRV_1196_112_2_17_23_6_28_1143 "\r\nSIMDRV%d::set the working volt to 1.8v."
#define SIM_DRV_1201_112_2_17_23_6_28_1144 "\r\nSIMDRV%d::set the working volt to 3.0v."
#define SIM_DRV_1206_112_2_17_23_6_28_1145 "\r\nSIMDRV%d::do not change the working volt."
#define SIM_DRV_1215_112_2_17_23_6_28_1146 "SIMDRV2::set the working volt to 1.8v."
#define SIM_DRV_1220_112_2_17_23_6_28_1147 "SIMDRV2::set the working volt to 3v."
#define SIM_DRV_1237_112_2_17_23_6_28_1148 "\r\nSIMDRV%d::set the working volt to 1.8v."
#define SIM_DRV_1242_112_2_17_23_6_28_1149 "\r\nSIMDRV%d::set the working volt to 3.0v."
#define SIM_DRV_1254_112_2_17_23_6_28_1150 "\r\nSIMDRV%d::do not change the working volt."
#define SIM_DRV_1263_112_2_17_23_6_28_1151 "\r\nSIMDRV%d::set the working volt to 1.8v."
#define SIM_DRV_1268_112_2_17_23_6_28_1152 "\r\nSIMDRV%d::set the working volt to 3.0v."
#define SIM_DRV_1280_112_2_17_23_6_28_1153 "\r\nSIMDRV%d::do not change the working volt."
#define SIM_DRV_1289_112_2_17_23_6_28_1154 "SIMDRV2::set the working volt to 1.8v."
#define SIM_DRV_1294_112_2_17_23_6_28_1155 "SIMDRV2::set the working volt to 3v."
#define SIM_DRV_1303_112_2_17_23_6_28_1156 "SIMDRV3::set the working volt to 1.8v."
#define SIM_DRV_1308_112_2_17_23_6_28_1157 "SIMDRV3::set the working volt to 3v."
#define SIM_DRV_1599_112_2_17_23_6_28_1158 "SIMDRV%d:power off"
#define SIM_DRV_2167_112_2_17_23_6_29_1159 "SIMDRV:reg 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
#define USIM_DRV_1175_112_2_17_23_6_32_1160 "USIMDRV:[%d]:DATA::RX 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X"
#define USIM_DRV_1590_112_2_17_23_6_33_1161 "USIMDRV:[%d]:[ResetDriver] Cur_Volt[%d],Tick[%d]"
#define USIM_DRV_1602_112_2_17_23_6_33_1162 "USIMDRV:[%d]:[ResetDriver] DMA_Support_Flag[%d]"
#define USIM_DRV_1625_112_2_17_23_6_33_1163 "USIMDRV:[0]: [Sim1InitDrvAfterLdoValid] Tick[%d]"
#define USIM_DRV_1649_112_2_17_23_6_33_1164 "USIMDRV:[0]: [Sim1InitDrvAfterLdoValid] Cur_Clk[%d],Clock_Mode[%d]"
#define USIM_DRV_1697_112_2_17_23_6_33_1165 "USIMDRV:[1]: [Sim2InitDrvAfterLdoValid] Tick[%d]"
#define USIM_DRV_1715_112_2_17_23_6_33_1166 "USIMDRV:[1]: [Sim2InitDrvAfterLdoValid] Cur_Clk[%d],Clock_Mode[%d]"
#define USIM_DRV_1797_112_2_17_23_6_33_1167 "USIMDRV:[%d]:API [SIM_WarmResetSimCard] Warm Reset the SIM card!"
#define USIM_DRV_1863_112_2_17_23_6_33_1168 "USIMDRV:[%d]:API [SIM_WarmResetSimCard] Tick[%d]"
#define USIM_DRV_1908_112_2_17_23_6_34_1169 "USIMDRV:[%d]: s_is_sim_out = TRUE;TX:Need De-active SIM!!!"
#define USIM_DRV_2098_112_2_17_23_6_34_1170 "USIMDRV:[%d]:tx_data_len[%d], rx_expected_len[%d]"
#define USIM_DRV_2106_112_2_17_23_6_34_1171 "USIMDRV:[%d]:0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X"
#define USIM_DRV_2224_112_2_17_23_6_34_1172 "USIMDRV:[%d]:[SendInstrCode_T1] IFSC[0x%x], Ins_Type[0x%x], P3[0x%x]"
#define USIM_DRV_2494_112_2_17_23_6_35_1173 "USIMDRV:[%d]:[SIM_PowerOffSIM]"
#define USIM_DRV_2693_112_2_17_23_6_35_1174 "USIMDRV:[%d]:[SIM_SelMFAckClkVol] Clk_Phase_in_MF != Clk_Phase_in_ATR, Clk_Phase_in_MF[%d]"
#define USIM_DRV_2700_112_2_17_23_6_35_1175 "USIMDRV:[%d]:[SIM_SelMFAckClkVol] Expected_Voltage[%d], Current_Voltage[%d]"
#define USIM_DRV_2762_112_2_17_23_6_35_1176 "USIMDRV:[%d]:[SIM_SelMFAckClkVol] Expected_Voltage != Working_Voltage ,SIM Reset Needed!"
#define USIM_DRV_2784_112_2_17_23_6_35_1177 "USIMDRV:[0]: [Sim1DriverTimerExpired] ERROR:SIM1 Module Disalbe but Receive Timer Interrupt[%d]!"
#define USIM_DRV_2792_112_2_17_23_6_35_1178 "USIMDRV:[0]: [Sim1DriverTimerExpired] ATR Wait timer expired"
#define USIM_DRV_2815_112_2_17_23_6_35_1179 "USIMDRV:[0]: [Sim1DriverTimerExpired] Parity_Change_Timer expired"
#define USIM_DRV_2825_112_2_17_23_6_35_1180 "USIMDRV:[0]: [Sim1DriverTimerExpired] Bit_Convention_Timer expired"
#define USIM_DRV_2837_112_2_17_23_6_35_1181 "USIMDRV:[0]: [Sim1DriverTimerExpired] Status_Word_Timer expired SW1[0x%02x], SW2[0x%02x]"
#define USIM_DRV_2858_112_2_17_23_6_35_1182 "USIMDRV:[0]: [Sim1DriverTimerExpired] BWT_Timer expired,Tick[0x%x],Rx_Cur_Pos[0x%x],Rx_Blk_Ext_Byte_Cnt[0x%x],Temp[0x%x]"
#define USIM_DRV_2868_112_2_17_23_6_35_1183 "USIMDRV:[0]: [Sim1DriverTimerExpired] Wait_Response_Timer expired ,Tick[0x%x]"
#define USIM_DRV_2876_112_2_17_23_6_36_1184 "USIMDRV:[0]: [Sim1DriverTimerExpired] Wait_Response_Timer expired, SIM_Status[%d]"
#define USIM_DRV_2901_112_2_17_23_6_36_1185 "USIMDRV:[0]: [Sim1DriverTimerExpired] Wait_Response_Timer expired, add NotifyExceptionError (TB_SIM_PROCESSING_PPS) !"
#define USIM_DRV_2932_112_2_17_23_6_36_1186 "USIMDRV:[0]: [Sim1DriverTimerExpired] T1_CWT_Timer expired, Tick[0x%x], Parity[0x%x]"
#define USIM_DRV_2945_112_2_17_23_6_36_1187 "USIMDRV:[0]: [Sim1DriverTimerExpired] RX_Timeout_Timer expired, Tick[0x%x],Rcv_Byte_Cnt[0x%x]"
#define USIM_DRV_2968_112_2_17_23_6_36_1188 "USIMDRV:[0]: [Sim1DriverTimerExpired] ERROR:Invalid_Timer g_user_mode_flag is TRUE."
#define USIM_DRV_2988_112_2_17_23_6_36_1189 "USIMDRV:[1]: [Sim2DriverTimerExpired] ERROR:SIM2 Module Disalbe but Receive Timer Interrupt[%d]!"
#define USIM_DRV_2995_112_2_17_23_6_36_1190 "USIMDRV:[1]: [Sim2DriverTimerExpired] ATR_Wait_Timer expired"
#define USIM_DRV_3018_112_2_17_23_6_36_1191 "USIMDRV:[1]: [Sim2DriverTimerExpired] Parity_Change_Timer expired"
#define USIM_DRV_3028_112_2_17_23_6_36_1192 "USIMDRV:[1]: [Sim2DriverTimerExpired] Bit_Convention_Timer expired"
#define USIM_DRV_3040_112_2_17_23_6_36_1193 "USIMDRV:[1]: [Sim2DriverTimerExpired] Status_Word_Timer expired, SW1[0x%02x], SW2[0x%02x]"
#define USIM_DRV_3061_112_2_17_23_6_36_1194 "USIMDRV:[1]: [Sim2DriverTimerExpired] BWT_Timer expired,Tick[0x%x],Rx_Cur_Pos[0x%x],Rx_Blk_Ext_Byte_Cnt[0x%x],Temp[0x%x]"
#define USIM_DRV_3071_112_2_17_23_6_36_1195 "USIMDRV:[1]: [Sim2DriverTimerExpired] Wait_Response_Timer expired ,Tick[0x%x]"
#define USIM_DRV_3079_112_2_17_23_6_36_1196 "USIMDRV:[1]: [Sim2DriverTimerExpired] Wait_Response_Timer expired, SIM_Status[%d]"
#define USIM_DRV_3104_112_2_17_23_6_36_1197 "USIMDRV:[1]: [Sim2DriverTimerExpired] Wait_Response_Timer expired, NotifyExceptionError (TB_SIM_PROCESSING_PPS) !"
#define USIM_DRV_3117_112_2_17_23_6_36_1198 "usimdrv:: t0 wwt timer expired [0x%x],rcv byte[0x%x]"
#define USIM_DRV_3140_112_2_17_23_6_36_1199 "USIMDRV:[1]: [Sim2DriverTimerExpired] T1_CWT_Timer expired, Tick[0x%x], Parity[0x%x]"
#define USIM_DRV_3153_112_2_17_23_6_36_1200 "USIMDRV:[1]: [Sim2DriverTimerExpired] RX_Timeout_Timer expired, Tick[0x%x],Rcv_Byte_Cnt[0x%x]"
#define USIM_DRV_3176_112_2_17_23_6_36_1201 "USIMDRV:[1]: [Sim2DriverTimerExpired] ERROR:Invalid_Timer g_user_mode_flag is TRUE."
#define USIM_DRV_3314_112_2_17_23_6_36_1202 "USIMDRV:[%d]:[ReportCardStatus] insert_status[%d]"
#define USIM_DRV_3662_112_2_17_23_6_37_1203 "USIMDRV:[%d] s_is_sim_out = TRUE;RX:Need De-active SIM!!!"
#define USIM_DRV_3783_112_2_17_23_6_37_1204 "USIMDRV:[%d]:[RxFifoFullIntP_T0] Section1, cur_pos[%d]"
#define USIM_DRV_3790_112_2_17_23_6_37_1205 "USIMDRV:[%d]:[RxFifoFullIntP_T0] NULL byte recieved before ACK!!!"
#define USIM_DRV_3821_112_2_17_23_6_37_1206 "USIMDRV:[%d]:[RxFifoFullIntP_T0] NULL byte(0x60) received!!!"
#define USIM_DRV_3835_112_2_17_23_6_37_1207 "USIMDRV:[%d]:[RxFifoFullIntP_T0] Section2, cur_pos[%d]"
#define USIM_DRV_3909_112_2_17_23_6_38_1208 "USIMDRV:[%d]:[RxFifoFullIntP_T1] Tick[0x%x], CWT[0x%x]"
#define USIM_DRV_3995_112_2_17_23_6_38_1209 "USIMDRV:[%d]:[RxFifoFullIntP_T1] Tick[0x%x],CWT[0x%x],Temp[0x%x],IFSD[0x%x]"
#define USIM_DRV_4029_112_2_17_23_6_38_1210 "USIMDRV:[%d]:[RxFifoFullIntP_T1]Ext_count[0x%x], Ext_byte[0x%x,0x%x,0x%x,0x%x,0x%x],EDC_Flag[0x%x]"
#define USIM_DRV_4067_112_2_17_23_6_38_1211 "USIMDRV:[%d]:[RxFifoFullIntP_T1]Ext_count[0x%x],Ext_byte[0x%x,0x%x,0x%x,0x%x,0x%x],EDC_Flag[0x%x],cur_pos[0x%x]"
#define USIM_DRV_4200_112_2_17_23_6_38_1212 "USIMDRV:[%d]:[EarlyATRIntP] card OUT in early ATR interrup"
#define USIM_DRV_4205_112_2_17_23_6_38_1213 "USIMDRV:[%d]:[EarlyATRIntP] Early ATR Interrupt!"
#define USIM_DRV_4234_112_2_17_23_6_38_1214 "USIMDRV:[%d]:[RxParityErrorIntP] sim_status[0x%x]"
#define USIM_DRV_4241_112_2_17_23_6_38_1215 "USIMDRV:: RX parity error happens in PPS procedure"
#define USIM_DRV_4307_112_2_17_23_6_38_1216 "USIMDRV:[%d]:[TxParityErrorIntP] WARNING"
#define USIM_DRV_4353_112_2_17_23_6_39_1217 "USIMDRV:[%d]:[CardUnResponseIntP] Tick[0x%x],Rx_Count[0x%x]"
#define USIM_DRV_4457_112_2_17_23_6_39_1218 "USIMDRV:[%d]:[SendResponseUp] NULL byte data!!!"
#define USIM_DRV_4461_112_2_17_23_6_39_1219 "USIMDRV:[%d]:[SendResponseUp] rev_data_len > exp_data_len"
#define USIM_DRV_4501_112_2_17_23_6_39_1220 "USIMDRV:[%d]:resp_data_len[%d],sw [0x%02x, 0x%02x]"
#define USIM_DRV_4509_112_2_17_23_6_39_1221 "USIMDRV:[%d]:0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X"
#define USIM_DRV_4528_112_2_17_23_6_39_1222 "USIMDRV:[%d]:[SendResponseUp] [0x%x, 0x%x, 0x%x]"
#define USIM_DRV_4594_112_2_17_23_6_39_1223 "USIMDRV:[%d]:[SendPPSRequest] DO PPS Request"
#define USIM_DRV_4655_112_2_17_23_6_39_1224 "USIMDRV:[%d]:[ResetSIMCard] Reset_type[%d]!"
#define USIM_DRV_4707_112_2_17_23_6_39_1225 "USIMDRV:[%d]:[ResetSIMCard] Reset the first time!"
#define USIM_DRV_4739_112_2_17_23_6_39_1226 "USIMDRV:[%d]:[ResetSIMCard] Reset for change_power!"
#define USIM_DRV_4751_112_2_17_23_6_39_1227 "USIMDRV:[%d]:[ResetSIMCard] Reject for NO next available voltage any more!"
#define USIM_DRV_4760_112_2_17_23_6_39_1228 "USIMDRV:[%d]:[ResetSIMCard] Reset for no_change!"
#define USIM_DRV_4770_112_2_17_23_6_39_1229 "USIMDRV:[%d]:[ResetSIMCard]  Reset for next_power LDO_Level[%d]"
#define USIM_DRV_4783_112_2_17_23_6_39_1230 "USIMDRV:[%d]:[ResetSIMCard] Reset for bit_convention,bit_convent[%d]"
#define USIM_DRV_4809_112_2_17_23_6_39_1231 "USIMDRV:[%d]:[ResetSIMCard] Reset for change_parity!"
#define USIM_DRV_5000_112_2_17_23_6_40_1232 "USIMDRV:[%d]:[IdleSIMCard] ATR Default ,MF == CLOCK_STOP_NOT_SUPPORT ,the UICC NOT support clock stop mode"
#define USIM_DRV_5157_112_2_17_23_6_40_1233 "USIMDRV:[%d]:[CheckReceiveBuf] ATR[0x%x], cur_pos[0x%x]"
#define USIM_DRV_5209_112_2_17_23_6_40_1234 "USIMDRV:[%d]:[CheckReceiveBuf] corrupted ATR data"
#define USIM_DRV_5217_112_2_17_23_6_40_1235 "USIMDRV:[%d]:[CheckReceiveBuf] active s_sim_wait_resp_timer!"
#define USIM_DRV_5250_112_2_17_23_6_40_1236 "USIMDRV:[%d]:[CheckReceiveBuf] ALL ATR received ,cur_pos[0x%x], exp_len[0x%x]"
#define USIM_DRV_5256_112_2_17_23_6_40_1237 "USIMDRV:[%d]:[CheckReceiveBuf] deactive s_sim_wait_resp_timer!"
#define USIM_DRV_5299_112_2_17_23_6_41_1238 "USIMDRV:[%d]:[CheckReceiveBuf]  Reset for next_voltage ATR_Vol[%d]"
#define USIM_DRV_5341_112_2_17_23_6_41_1239 "USIMDRV:[%d]:[CheckReceiveBuf] use enhanced PPS data 512 8"
#define USIM_DRV_5348_112_2_17_23_6_41_1240 "USIMDRV:[%d]:[CheckReceiveBuf] use enhanced PPS data 512 16"
#define USIM_DRV_5356_112_2_17_23_6_41_1241 "USIMDRV:[%d]:[CheckReceiveBuf] use enhanced PPS data 512 32"
#define USIM_DRV_5364_112_2_17_23_6_41_1242 "USIMDRV:[%d]:[CheckReceiveBuf] use default PPS data 372 1"
#define USIM_DRV_5383_112_2_17_23_6_41_1243 "USIMDRV:[%d]:[CheckReceiveBuf] NOT do PPS procedure!"
#define USIM_DRV_5410_112_2_17_23_6_41_1244 "USIMDRV:[%d]:[CheckReceiveBuf] All ATR received! sim_status[0x%x]"
#define USIM_DRV_5415_112_2_17_23_6_41_1245 "USIMDRV:[%d]:[CheckReceiveBuf] NOT all ATR received !"
#define USIM_DRV_5419_112_2_17_23_6_41_1246 "USIMDRV:[%d]:[CheckReceiveBuf] active s_sim_wait_resp_timer!"
#define USIM_DRV_5448_112_2_17_23_6_41_1247 "USIMDRV:[%d]:[CheckReceiveBuf] Received PPS data error!!! Reset with no PPS!!!"
#define USIM_DRV_5457_112_2_17_23_6_41_1248 "USIMDRV:[%d]:[CheckReceiveBuf] NotifyExceptionError (TB_SIMEXP_CARD_OUT) !"
#define USIM_DRV_5492_112_2_17_23_6_41_1249 "USIMDRV:[%d]:[CheckReceiveBuf] Enhanced pps supported!"
#define USIM_DRV_5527_112_2_17_23_6_41_1250 "USIMDRV:[%d]:[CheckReceiveBuf] received NULL_Byte(0x60), like status word, not sendresponse up, some data may report later !"
#define USIM_DRV_5562_112_2_17_23_6_41_1251 "USIMDRV:[%d]:[CheckReceiveBuf] expected_len[%d] <= cur_pos[%d]"
#define USIM_DRV_5677_112_2_17_23_6_41_1252 "USIMDRV:[%d]:[CheckBlockValid] Section1 Tick[0x%x], CWT_Timer[0x%x], block_valid[0x%x], block_type[0x%x]"
#define USIM_DRV_5745_112_2_17_23_6_41_1253 "USIMDRV:[%d]:[CheckBlockValid] Section2 tx_block_type[0x%x], rx_block_type[0x%x],block_validity[0x%x],Tick[0x%x],info_len[0x%x]"
#define USIM_DRV_5806_112_2_17_23_6_42_1254 "USIMDRV:[%d]:[CheckBlockValid] send_next block_begin_pos[0x%x], endpos[0x%x], tx_pcb[0x%x]"
#define USIM_DRV_5825_112_2_17_23_6_42_1255 "USIMDRV:[%d]:[CheckBlockValid] resend block_begin_pos[0x%x], endpos[0x%x], tx_pcb[0x%x]"
#define USIM_DRV_5878_112_2_17_23_6_42_1256 "USIMDRV:[%d]:[CheckNRValid] input pcb_nr[0x%x], ns[0x%x], block_type[0x%x]"
#define USIM_DRV_5907_112_2_17_23_6_42_1257 "USIMDRV:[%d]:[CheckBlockLenValid] ifsd[0x%x],act_len[0x%x],block_type[0x%x],chain[0x%x]"
#define USIM_DRV_6203_112_2_17_23_6_42_1258 "USIMDRV:[%d]:[DecodeATRData] TC1 default[0x%x]to be [0x%x]"
#define USIM_DRV_6237_112_2_17_23_6_42_1259 "USIMDRV:[%d]:[ErrorHandler] reset_count[0x%x], block_type[0x%x], error_code[0x%x]"
#define USIM_DRV_6255_112_2_17_23_6_43_1260 "USIMDRV:[%d]:[ErrorHandler] When I-Block BWT time-out,send a r-block"
#define USIM_DRV_6263_112_2_17_23_6_43_1261 "USIMDRV:[%d]:[ErrorHandler] When R-block BWT time-out,resent a s-block request"
#define USIM_DRV_6272_112_2_17_23_6_43_1262 "USIMDRV:[%d]:[ErrorHandler] When S-block request BWT time-out,resent a s-block request"
#define USIM_DRV_6278_112_2_17_23_6_43_1263 "USIMDRV:[%d]:[ErrorHandler] When S-block response BWT time-out,send a r-block"
#define USIM_DRV_6321_112_2_17_23_6_43_1264 "USIMDRV:[%d]:[ErrorHandler] Unknown Block Type ,refer to 7.3.13"
#define USIM_DRV_6329_112_2_17_23_6_43_1265 "USIMDRV:[%d]:[ErrorHandler] resynch req needed, refer to 7.3.13"
#define USIM_DRV_6338_112_2_17_23_6_43_1266 "USIMDRV:[%d]:[ErrorHandler] continous BWT time-out or invalid block to the max time"
#define USIM_DRV_6485_112_2_17_23_6_43_1267 "USIMDRV:[%d]:[GetT0WWT] WI[0x%x] WWT[0x%x] REAL_WWT = WWT +20"
#define USIM_DRV_6511_112_2_17_23_6_43_1268 "USIMDRV:[%d]:[GetT1CWT] CWI[0x%x], CWT[0x%x]"
#define USIM_DRV_6544_112_2_17_23_6_43_1269 "USIMDRV:[%d]:[GetBWT] BWI[0x%x] BWT[0x%x], NEW_BWT[0x%x]"
#define USIM_DRV_6648_112_2_17_23_6_43_1270 "USIMDRV:[%d]:[GetBlockBaseInfo] PCB_Byte[0x%x], Tick[0x%x]"
#define USIM_DRV_6716_112_2_17_23_6_44_1271 "USIMDRV:[%d]:[GetBlockBaseInfo] NR[0x%x], NS[0x%x], block_type[0x%x]"
#define USIM_DRV_6748_112_2_17_23_6_44_1272 "USIMDRV:[%d]:[GetBlockBaseInfo] ERROR: Invalid PCB"
#define USIM_DRV_6909_112_2_17_23_6_44_1273 "USIMDRV:[%d]:[SendRBlock] Tick[0x%x] temp[0x%x,0x%x,0x%x,0x%x]"
#define USIM_DRV_6920_112_2_17_23_6_44_1274 "USIMDRV:[%d]:[SendRBlock] BWT[0x%x]"
#define USIM_DRV_7019_112_2_17_23_6_44_1275 "USIMDRV:[%d]:[GetT0WWT]  reason[0x%x],0x%x,[0x%x,0x%x,0x%x,0x%x,0x%x]"
#define USIM_DRV_7037_112_2_17_23_6_44_1276 "USIMDRV:[%d]:[GetT0WWT] sendsblock the bwt is 0x%x"
#define USIM_DRV_7053_112_2_17_23_6_44_1277 "\r\nUSIMDRV::Callback func is null"
#define USIM_DRV_7073_112_2_17_23_6_44_1278 "USIMDRV:[%d]:[GetNextSupplyVoltage] 1.8V -> 3V"
#define USIM_DRV_7137_112_2_17_23_6_44_1279 "USIMDRV:[%d]:[SetSIMPower] do not support 5v"
#define USIM_DRV_7155_112_2_17_23_6_44_1280 "USIMDRV:[%d]:[SetSIMPower] Before setting LDO Level[%d]"
#define USIM_DRV_7167_112_2_17_23_6_44_1281 "USIMDRV:[%d]:[SetSIMPower] do not support 5v"
#define USIM_DRV_7173_112_2_17_23_6_44_1282 "USIMDRV:[%d]:[SetSIMPower] After seting LDO Level[%d]"
#define USIM_DRV_7191_112_2_17_23_6_44_1283 "USIMDRV:[0]:[Sim1ResetAfterLdoValid] Tick[%d]"
#define USIM_DRV_7283_112_2_17_23_6_45_1284 "USIMDRV:[1]:[Sim2ResetAfterLdoValid] Tick[%d]"
#define USIM_DRV_7422_112_2_17_23_6_45_1285 "USIMDRV:[%d]:[HandleUICCSBlock] wrong block type 0x%x?"
#define USIM_DRV_7463_112_2_17_23_6_45_1286 "USIMDRV:[%d]:[HandleResyncRsp]  handle resync rsp nr[0x%x]ns[0x%x]tx_len[0x%x]"
#define USIM_DRV_7803_112_2_17_23_6_46_1287 "USIMDRV:[%d]:[DMA_SendSBLOCK] Tick[0x%x],Reason[0x%x],Ext_Cnt[0x%x],Ext_data[0x%x,0x%x,0x%x,0x%x,0x%x]"
#define USIM_DRV_7822_112_2_17_23_6_46_1288 "USIMDRV:[%d]:[DMA_SendSBLOCK] ATR.BWT[0x%x]"
#define USIM_DRV_7905_112_2_17_23_6_46_1289 "USIMDRV:[%d]:[DMA_SendRBlock] Tick[0x%x], Buff[0x%x,0x%x,0x%x,0x%x]"
#define USIM_DRV_7916_112_2_17_23_6_46_1290 "USIMDRV:[%d]:[DMA_SendRBlock] ATR.BWT[0x%x]"
#define USIM_DRV_7969_112_2_17_23_6_46_1291 "USIMDRV:[%d]:[DMA_CheckBlockValid] Tick[0x%x], CWT_Timer[0x%x], Status[0x%x], Tx_Block_Type[0x%x]"
#define USIM_DRV_8037_112_2_17_23_6_46_1292 "USIMDRV:[%d]:[DMA_CheckBlockValid] tx_type[0x%x], rx_type[0x%x],block_validity[0x%x],Tick[0x%x], Info_Len[0x%x]"
#define USIM_DRV_8098_112_2_17_23_6_46_1293 "USIMDRV:[%d]:[DMA_CheckBlockValid] Send_Next curpos[0x%x], endpos[0x%x], tx pcb[0x%x]"
#define USIM_DRV_8117_112_2_17_23_6_46_1294 "USIMDRV:[%d]:[DMA_CheckBlockValid] Resend curpos[0x%x], endpos[0x%x], tx pcb[0x%x]"
#define USIM_DRV_8191_112_2_17_23_6_46_1295 "USIMDRV:[%d]:[DMA_CheckReceiveBuf] expected len[%d] <= cur_pos[%d]"
#define USIM_DRV_8265_112_2_17_23_6_47_1296 "USIMDRV:[%d]:[DMA_TX_Callback_T0] tx_data_len[%d],tx_cur_pos[%d], ACK_Flag[%d]"
#define USIM_DRV_8495_112_2_17_23_6_47_1297 "USIMDRV:[%d]:[DMA_RX_Callback_T0] data_temp[0x%2x], rx_cur_pos[%d]"
#define USIM_DRV_8500_112_2_17_23_6_47_1298 "USIMDRV:[%d]:[DMA_RX_Callback_T0] rx_buf is locked!!!"
#define USIM_DRV_8582_112_2_17_23_6_47_1299 "USIMDRV:[%d]:[DMA_RX_Callback_T0]  reset s_sim_rx_buf[card_id], cur_pos[%d]"
#define USIM_DRV_8589_112_2_17_23_6_47_1300 "USIMDRV:[%d]:[DMA_RX_Callback_T0]  NULL_byte(0x60) before ACK!!!"
#define USIM_DRV_8624_112_2_17_23_6_47_1301 "USIMDRV:[%d]:[DMA_RX_Callback_T0]  reset s_sim_rx_buf[card_id], cur_pos[%d]"
#define USIM_DRV_8701_112_2_17_23_6_47_1302 "USIMDRV:[%d]:[DMA_RX_Callback_T1] rxfifofull Tick[0x%x], CWT_Timer[0x%x]"
#define USIM_DRV_8808_112_2_17_23_6_48_1303 "USIMDRV:[%d]:[DMA_RX_Callback_T1] rxfifofull t1 Tick[0x%x],CWT_Timer[0x%x],[0x%x,0x%x]"
#define USIM_DRV_8843_112_2_17_23_6_48_1304 "USIMDRV:[%d]:[DMA_RX_Callback_T1] Section1 s_sim_rx_block Ext_Cnt[0x%x], EXT[0x%x,0x%x,0x%x,0x%x,0x%x], EDC[0x%x]"
#define USIM_DRV_8876_112_2_17_23_6_48_1305 "USIMDRV:[%d]:[DMA_RX_Callback_T1] Section2 Ext_Cnt[0x%x], EXT[0x%x,0x%x,0x%x,0x%x,0x%x],EDC[0x%x],rcv[0x%x]"
#define USIM_DRV_8905_112_2_17_23_6_48_1306 "USIMDRV:[%d]:[USIM_DMA_Init]"
#define USIM_DRV_9070_112_2_17_23_6_48_1307 "USIMDRV:[%d]:tx_data_len is[%d], rx_expected_len[%d]"
#define USIM_DRV_9078_112_2_17_23_6_48_1308 "USIMDRV:[%d]:0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X"
#define USIM_DRV_9154_112_2_17_23_6_48_1309 "USIMDRV:[%d]:[DMA_SendInstrCode_T1] IFSC[0x%x], instr_type[0x%x], P3[0x%x]"
#define USIM_DRV_9358_112_2_17_23_6_49_1310 "USIMDRV:[0]:[Sim1DMATimerExpired] Tick[0x%x],rcv_byte[0x%x]"
#define USIM_DRV_9392_112_2_17_23_6_49_1311 "USIMDRV:[0]:[Sim1DMATimerExpired] DMA BWT timer expired[0x%x][0x%x,0x%x],0x%x"
#define USIM_DRV_9404_112_2_17_23_6_49_1312 "USIMDRV:[0]:[Sim1DMATimerExpired] DMA WWT timer expired[0x%x][0x%x],0x%x"
#define USIM_DRV_9447_112_2_17_23_6_49_1313 "USIMDRV:[1]:[Sim2DMATimerExpired]  DMATimerExpired[0x%x],rcv byte[0x%x]"
#define USIM_DRV_9481_112_2_17_23_6_49_1314 "USIMDRV:[1]:[Sim2DMATimerExpired] DMA BWT timer expired[0x%x][0x%x,0x%x],0x%x"
#define USIM_DRV_9493_112_2_17_23_6_49_1315 "USIMDRV:[1]:[Sim2DMATimerExpired] DMA WWT timer expired[0x%x][0x%x],0x%x"
#define SPI_HAL_TEST_206_112_2_17_23_6_59_1316 "Spi_Thput_Test: case_size %d"
#define SPI_HAL_TEST_265_112_2_17_23_6_59_1317 "Spi_Thput_Test: write: pre_tick %d, cur_tick=%d, time:%d"
#define SPI_HAL_TEST_287_112_2_17_23_6_59_1318 "Spi_Thput_Test: read: pre_tick %d, cur_tick=%d, time:%d"
#define TPC_PHY_V0_179_112_2_17_23_7_23_1319 "[TPDRV:TPDRV_EnableCoord]"
#define TPC_PHY_V0_191_112_2_17_23_7_23_1320 "[TPDRV:TPDRV_DisableCoord]"
#define TPC_PHY_V0_301_112_2_17_23_7_23_1321 "[TPDRV:Down at %d] "
#define TPC_PHY_V0_354_112_2_17_23_7_23_1322 "[TPDRV:Done at %d] "
#define TPC_PHY_V0_512_112_2_17_23_7_23_1323 "[TPDRV:TPDRV_Run %d]"
#define TPC_PHY_V0_519_112_2_17_23_7_23_1324 "[TPDRV:TPDRV_Run %d, temp > 800]"
#define TPC_PHY_V0_575_112_2_17_23_7_24_1325 "[TPDRV:TPDRV_Stop]"
#define TPC_PHY_V0_595_112_2_17_23_7_24_1326 "[TPDRV:TPDRV_FetchData] buffer empty"
#define TPC_PHY_V0_626_112_2_17_23_7_24_1327 "[TPDRV:TPDRV_FetchData x %x] "
#define TPC_PHY_V0_651_112_2_17_23_7_24_1328 "[TPDRV:TPDRV_FetchData y %x] "
#define TPC_PHY_V0_682_112_2_17_23_7_24_1329 "[TPDRV:TPDRV_FetchData] buffer empty"
#define TPC_PHY_V0_698_112_2_17_23_7_24_1330 "[TPDRV:_FetchData]current pressure factor:%d"
#define TPC_PHY_V1_447_112_2_17_23_7_25_1331 "tp_conf_cal_parameter:x_slop =%d, x_offset = %d, y_slop = %d, y_offset = %d "
#define TPC_PHY_V3_177_112_2_17_23_7_26_1332 "[TPDRV:TPDRV_EnableCoord]"
#define TPC_PHY_V3_186_112_2_17_23_7_26_1333 "[TPDRV:TPDRV_DisableCoord]"
#define TPC_PHY_V3_235_112_2_17_23_7_26_1334 "[TPDRV:TPDRV_ConfigCoord]"
#define TPC_PHY_V3_376_112_2_17_23_7_26_1335 "[TPDRV:TPDRV_Run ]"
#define TPC_PHY_V3_395_112_2_17_23_7_26_1336 "[TPDRV:TPDRV_Stop]"
#define TPC_PHY_V3_413_112_2_17_23_7_26_1337 "[TPDRV:TPDRV_FetchData] buffer not full 0x%x"
#define TPC_PHY_V3_435_112_2_17_23_7_26_1338 "[TPDRV:TPDRV_FetchData] delta_x = %d delta_y = %d"
#define TPC_ALGORITHM_263_112_2_17_23_7_27_1339 "%s"
#define TPC_ALGORITHM_265_112_2_17_23_7_27_1340 "%s"
#define TPC_ALGORITHM_303_112_2_17_23_7_27_1341 "%s"
#define TPC_ALGORITHM_305_112_2_17_23_7_27_1342 "%s"
#define TPC_ALGORITHM_308_112_2_17_23_7_27_1343 "%s"
#define TPC_ALGORITHM_312_112_2_17_23_7_27_1344 "%s"
#define TPC_ALGORITHM_317_112_2_17_23_7_27_1345 "%s"
#define TPC_ALGORITHM_321_112_2_17_23_7_27_1346 "%s"
#define TPC_ALGORITHM_329_112_2_17_23_7_27_1347 "%s"
#define TPC_ALGORITHM_331_112_2_17_23_7_27_1348 "%s"
#define TPC_ALGORITHM_333_112_2_17_23_7_27_1349 "%s"
#define TPC_ALGORITHM_343_112_2_17_23_7_27_1350 "TPDRV_algorithm:----single_point---------------------==OK==----"
#define TPC_ALGORITHM_347_112_2_17_23_7_27_1351 "TPDRV_algorithm:----single_point: -----------* -*invalid-* -*---"
#define TPC_ALGORITHM_357_112_2_17_23_7_27_1352 "TPDRV_algorithm:----double_point: ----------------==OK==-------"
#define TPC_ALGORITHM_362_112_2_17_23_7_27_1353 "TPDRV_algorithm:----double_point:-* -*-* -* -* -*invalid-* -* "
#define TPC_ALGORITHM_402_112_2_17_23_7_27_1354 "%s"
#define TPC_ALGORITHM_405_112_2_17_23_7_27_1355 "TPDRV_algorithm:---------------interator_count=%d---------\r\n"
#define TPC_ALGORITHM_409_112_2_17_23_7_27_1356 "TPDRV_algorithm:---------------interator_count=%d---------\r\n"
#define TPC_ALGORITHM_433_112_2_17_23_7_27_1357 "[TPDRV_algorithm:(Xd1,Yd1)=[%d,%d], (Xd2,Yd2)=[%d,%d]rn"
#define TPC_PHY_V5_255_112_2_17_23_7_28_1358 "\r\n %s"
#define TPC_PHY_V5_256_112_2_17_23_7_28_1359 " --->:ADC_HW_CH_DELAY =0x%8x"
#define TPC_PHY_V5_257_112_2_17_23_7_28_1360 " --->:ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL) =0x%8x"
#define TPC_PHY_V5_258_112_2_17_23_7_28_1361 " --->:ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL) =0x%8x"
#define TPC_PHY_V5_259_112_2_17_23_7_28_1362 " --->:ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL) =0x%8x"
#define TPC_PHY_V5_260_112_2_17_23_7_28_1363 " --->:ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL) =0x%8x"
#define TPC_PHY_V5_262_112_2_17_23_7_28_1364 " --->:TPC_SAMPLE_CTRL0 =0x%8x"
#define TPC_PHY_V5_263_112_2_17_23_7_28_1365 " --->:TPC_SAMPLE_CTRL1 =0x%8x"
#define TPC_PHY_V5_264_112_2_17_23_7_28_1366 " --->:TPC_SAMPLE_CTRL2 =0x%8x"
#define TPC_PHY_V5_265_112_2_17_23_7_28_1367 " --->:TPC_BOUNCE_CTRL   =0x%8x"
#define TPC_PHY_V5_266_112_2_17_23_7_28_1368 " --->:TPC_FILTER_CTRL   =0x%8x"
#define TPC_PHY_V5_267_112_2_17_23_7_28_1369 " --->:TPC_CALC_CTRL    =0x%8x"
#define TPC_PHY_V5_268_112_2_17_23_7_28_1370 " --->:TPC_BUF_CTRL    =0x%8x"
#define TPC_PHY_V5_270_112_2_17_23_7_28_1371 " --->:TPC_CTRL         =0x%8x"
#define TPC_PHY_V5_271_112_2_17_23_7_28_1372 " --->:TPC_DATA_MIN     =0x%8x"
#define TPC_PHY_V5_272_112_2_17_23_7_28_1373 " --->:TPC_DATA_MAX     =0x%8x"
#define TPC_PHY_V5_273_112_2_17_23_7_28_1374 " --->:TPC_CALC_X_COEF_A =0x%8x"
#define TPC_PHY_V5_274_112_2_17_23_7_28_1375 " --->:TPC_CALC_X_COEF_B =0x%8x"
#define TPC_PHY_V5_275_112_2_17_23_7_28_1376 " --->:TPC_CALC_Y_COEF_A =0x%8x"
#define TPC_PHY_V5_276_112_2_17_23_7_28_1377 " --->:TPC_CALC_Y_COEF_B =0x%8x"
#define TPC_PHY_V5_278_112_2_17_23_7_28_1378 " --->:TPC_INT_EN         =0x%8x"
#define TPC_PHY_V5_279_112_2_17_23_7_28_1379 " --->:TPC_INT_RAW         =0x%8x"
#define TPC_PHY_V5_280_112_2_17_23_7_28_1380 " --->:TPC_INT_CLR         =0x%8x"
#define TPC_PHY_V5_282_112_2_17_23_7_28_1381 " --->:TPC_OUT_SEQ1     =0x%8x"
#define TPC_PHY_V5_283_112_2_17_23_7_28_1382 " --->:TPC_OUT_SEQ0     =0x%8x"
#define TPC_PHY_V5_284_112_2_17_23_7_28_1383 " --->:TPC_OE_SEQ1      =0x%8x"
#define TPC_PHY_V5_285_112_2_17_23_7_28_1384 " --->:TPC_OE_SEQ0      =0x%8x"
#define TPC_PHY_V5_286_112_2_17_23_7_28_1385 " --->:TPC_CH_SEQ       =0x%8x"
#define TPC_PHY_V5_287_112_2_17_23_7_28_1386 " --->:TPC_RES_SEQ      =0x%8x"
#define TPC_PHY_V5_288_112_2_17_23_7_28_1387 " --->:TPC_SWT_SEQ      =0x%8x"
#define TPC_PHY_V5_290_112_2_17_23_7_28_1388 "\r\n"
#define TPC_PHY_V5_565_112_2_17_23_7_29_1389 "TPDRV:----------->TP_XY_EXCHANGE=1 --------->"
#define TPC_PHY_V5_578_112_2_17_23_7_29_1390 "TPDRV:----------->_SeqCalibration------------>"
#define TPC_PHY_V5_616_112_2_17_23_7_29_1391 "TPDRV:----------->TP_XY_EXCHANGE=1------------>"
#define TPC_PHY_V5_626_112_2_17_23_7_29_1392 "TPDRV:----------->_SeqMultitouch------------>"
#define TPC_PHY_V5_650_112_2_17_23_7_29_1393 "[TPDRV:TPDRV_Run ]"
#define TPC_PHY_V5_665_112_2_17_23_7_29_1394 "[TPDRV:TPDRV_Stop]"
#define TPC_PHY_V5_692_112_2_17_23_7_29_1395 "TPDRV:_FetchMultiData:"
#define TPC_PHY_V5_748_112_2_17_23_7_29_1396 "rnTPDRV:Original_bef: Y=%df Y1=%df X=%df X1=%df Ry=%df Rx=%df RT=%df"
#define TPC_PHY_V5_768_112_2_17_23_7_29_1397 "TPDRV:Original_ohm:x1_pixels=%f y1_pixels=%f RT_Y=%f RT_X=%f RT=%f"
#define TPC_PHY_V5_777_112_2_17_23_7_29_1398 "TPDRV:Original_aft: Y=%f Y1=%f X=%f X1=%f Ry=%f Rx=%f RT=%f"
#define TPC_PHY_V5_803_112_2_17_23_7_29_1399 "TPDRV:_FetchCalibrateData:\r\n"
#define TPC_PHY_V5_859_112_2_17_23_7_29_1400 "%s"
#define TPC_PHY_V5_865_112_2_17_23_7_29_1401 "TPDRV: adc raw data(%d,%d)\r\n"
#define TPC_PHY_V5_886_112_2_17_23_7_29_1402 "TPDRV: TP_REF_U=%d TP_REF_R=%d"
#define TPC_PHY_V5_898_112_2_17_23_7_29_1403 "%s"
#define TPC_PHY_V5_1062_112_2_17_23_7_30_1404 "[TPDRV: TPDRV_Init]"
#define TPC_PHY_V5_1077_112_2_17_23_7_30_1405 "[TPDRV:TPDRV_EnableCoord]"
#define TPC_PHY_V5_1087_112_2_17_23_7_30_1406 "[TPDRV:TPDRV_DisableCoord]"
#define TPC_PHY_V5_1128_112_2_17_23_7_30_1407 "[TPDRV:TPDRV_ConfigCoord]==%d"
#define TPC_PHY_V5_1141_112_2_17_23_7_30_1408 "[TPDRV:TPDRV_EnterCalibration]"
#define TPC_PHY_V5_1154_112_2_17_23_7_30_1409 "[TPDRV:TPDRV_EnterCalibration]"
#define TPC_PHY_V5_1244_112_2_17_23_7_30_1410 "----\r\n\r\n"
#define TPC_PHY_V5_1246_112_2_17_23_7_30_1411 "changde=========%s"
#define TPC_PHY_V5_1248_112_2_17_23_7_30_1412 "changde=========%s"
#define TPC_PHY_V5_1249_112_2_17_23_7_30_1413 "changde=========-%f"
#define TPC_PHY_V5_1263_112_2_17_23_7_30_1414 "[TPDRV:_FetchData] (%d,%d),(%d,%d),size=%d"
#define TPC_PHY_V5_1273_112_2_17_23_7_30_1415 "[TPDRV:_FetchData] return--1"
#define TPC_PHY_V5_1281_112_2_17_23_7_30_1416 "[TPDRV:_FetchData] return--2"
#define TPC_PHY_V5_1287_112_2_17_23_7_30_1417 "[TPDRV:_FetchData] return--3"
#define TPC_PHY_V5_1293_112_2_17_23_7_30_1418 "[TPDRV:_FetchData] return--4"
#define TPC_PHY_V5_1366_112_2_17_23_7_30_1419 "[TPDRV:_GetPoints] (%d,%d),size=%d"
#define TPC_PHY_V5___110_112_2_17_23_7_31_1420 "\r\n %s"
#define TPC_PHY_V5___111_112_2_17_23_7_31_1421 " --->TPDRV:TPC_SAMPLE_DEBUG =0x%8x"
#define TPC_PHY_V5___112_112_2_17_23_7_31_1422 " --->TPDRV:TPC_DBNC_DEBUG   =0x%8x"
#define TPC_PHY_V5___113_112_2_17_23_7_31_1423 " --->TPDRV:TPC_BUF_DEBUG    =0x%8x"
#define TPC_PHY_V5___114_112_2_17_23_7_31_1424 " --->TPDRV:ADC_DEBUG        =0x%8x"
#define TPC_PHY_V5___115_112_2_17_23_7_31_1425 " --->TPDRV:ADC_CTRL         =0x%8x"
#define TPC_PHY_V5___117_112_2_17_23_7_31_1426 " --->TPDRV:0x820016ac       =0x%8x"
#define TPC_PHY_V5___118_112_2_17_23_7_31_1427 " --->TPDRV:0x820016a8       =0x%8x"
#define TPC_PHY_V5___119_112_2_17_23_7_31_1428 " --->TPDRV:0x820016b0       =0x%8x"
#define TPC_PHY_V5___120_112_2_17_23_7_31_1429 " --->TPDRV:0x820016b4       =0x%8x"
#define TPC_PHY_V5___122_112_2_17_23_7_31_1430 " --->TPDRV:TPC_OUT_SEQ0     =0x%8x"
#define TPC_PHY_V5___123_112_2_17_23_7_31_1431 " --->TPDRV:TPC_OE_SEQ0      =0x%8x"
#define TPC_PHY_V5___124_112_2_17_23_7_31_1432 " --->TPDRV:TPC_CH_SEQ       =0x%8x"
#define TPC_PHY_V5___126_112_2_17_23_7_31_1433 " --->TPDRV:REG_ANA_MISC_STS0=0x%8x"
#define TPC_PHY_V5___127_112_2_17_23_7_31_1434 " --->TPDRV:TPC_SAMPLE_CTRL0 =0x%8x"
#define TPC_PHY_V5___128_112_2_17_23_7_31_1435 " --->TPDRV:TPC_INT_EN =0x%8x"
#define TPC_PHY_V5___129_112_2_17_23_7_31_1436 " --->TPDRV:TPC_INT_RAW=0x%8x"
#define TPC_PHY_V5___130_112_2_17_23_7_31_1437 " --->TPDRV:TPC_CTRL   =0x%8x"
#define TPC_PHY_V5___132_112_2_17_23_7_31_1438 "\r\n"
#define TPC_PHY_V5___301_112_2_17_23_7_31_1439 "[TPDRV: TPDRV_Init]"
#define TPC_PHY_V5___314_112_2_17_23_7_31_1440 "[TPDRV: TPDRV_EnableCoord]"
#define TPC_PHY_V5___323_112_2_17_23_7_31_1441 "[TPDRV: TPDRV_DisableCoord]"
#define TPC_PHY_V5___376_112_2_17_23_7_31_1442 "[TPDRV:TPDRV_ConfigCoord]==%d"
#define TPC_PHY_V5___468_112_2_17_23_7_31_1443 "[TPDRV:_HandleTPCHisr ] pressed_time is not enough."
#define TPC_PHY_V5___529_112_2_17_23_7_31_1444 "[TPDRV:TPDRV_Run ]"
#define TPC_PHY_V5___543_112_2_17_23_7_31_1445 "[TPDRV:TPDRV_Stop]"
#define TPC_PHY_V5___625_112_2_17_23_7_32_1446 "TPDRV:_FetchData x=%d y=%d "
#define GX_UDC_1066_112_2_17_23_7_45_1447 "\ngx_udc_free_request() queue not empty. can not release\n"
#define GX_UDC_1394_112_2_17_23_7_45_1448 "\nusb_gadget_register_driver(): bind gadget failed\n"
#define GX_UDC_1419_112_2_17_23_7_45_1449 "\nusb_gadget_unregister_driver(): exit\n"
#define SX_UDC_617_112_2_17_23_7_47_1450 "ep_no(%d) is invalid !!!\n"
#define USB20_DRV_203_112_2_17_23_7_54_1451 "epno_out %d MSP =0, reconfigure it now\n"
#define USB20_DRV_231_112_2_17_23_7_54_1452 "epno_in %d MSP =0, reconfigure it now\n"
#define USB20_DRV_947_112_2_17_23_7_56_1453 "USB_EPxReceiveData: ep_id %d  int_sts 0x%x\n"
#define USB20_DRV_968_112_2_17_23_7_56_1454 "USB_EPxSendData: ep_id %d  int_sts 0x%x\n"
#define USB20_DRV_995_112_2_17_23_7_56_1455 "ep_id %d  int_sts 0x%x count %d \n"
#define USB_COMMON_120_112_2_17_23_7_57_1456 "USB_SetPlugInCount: count %d, is increase %d"
#define USB_COMMON_138_112_2_17_23_7_57_1457 "USB_SetPlugOutCount: count %d, is increase %d"
#define USB_COMMON_224_112_2_17_23_7_57_1458 "USB LDO disable!"
#define USB_COMMON_240_112_2_17_23_7_57_1459 "USB send plugout signal to UPM!"
#define USB_COMMON_328_112_2_17_23_7_57_1460 "usb_init+++++++++++++++++++++++++++++++++++++\r\n"
#define USB_DRV_E_113_112_2_17_23_7_58_1461 "USB LDO enable  ! "
#define USB_DRV_E_125_112_2_17_23_7_58_1462 "USB LDO disable!"
#define USB_DRV_E_137_112_2_17_23_7_58_1463 "USB send plugout signal to UPM!"
#define USB_DRV_E_277_112_2_17_23_7_58_1464 "usb intoken ack timeout"
#define USB_DRV_E_286_112_2_17_23_7_58_1465 "USB_REQ_CLASS error setup value:0x%08x,0x%08x"
#define USB_DRV_E_295_112_2_17_23_7_58_1466 "USB_REQ_VENDOR error setup value:0x%08x,0x%08x"
#define USB_DRV_E_301_112_2_17_23_7_58_1467 "bsd get device info error"
#define USB_DRV_E_307_112_2_17_23_7_58_1468 "bsd open device fail"
#define USB_DRV_E_313_112_2_17_23_7_58_1469 "bsd close device fail"
#define USB_DRV_E_318_112_2_17_23_7_58_1470 "UPCC requese type error"
#define USB_DRV_I_106_112_2_17_23_7_59_1471 "USB_SetPlugInCount: count %d, is increase %d"
#define USB_DRV_I_124_112_2_17_23_7_59_1472 "USB_SetPlugOutCount: count %d, is increase %d"
#define USB_DRV_I_134_112_2_17_23_7_59_1473 "USB_GetPlugInCount: %d"
#define USB_DRV_I_144_112_2_17_23_7_59_1474 "USB_GetPlugOutCount: %d"
#define USB_DRV_I_165_112_2_17_23_7_59_1475 "USB irq is 0x%x "
#define USB_DRV_I_180_112_2_17_23_7_59_1476 "USB_irq:USB send PLUG_IN signal to UPM "
#define USB_DRV_I_202_112_2_17_23_7_59_1477 "USB_irq:USB send PLUG_IN signal to UPM and service"
#define USB_DRV_I_217_112_2_17_23_7_59_1478 "USB current UPM's service is disabled !"
#define USB_DRV_I_239_112_2_17_23_7_59_1479 "USB current UPM's service is disabled !"
#define USB_DRV_I_255_112_2_17_23_7_59_1480 "USB current UPM's service is disabled !"
#define USB_DRV_I_271_112_2_17_23_7_59_1481 "USB current UPM's service is disabled !"
#define USB_DRV_I_287_112_2_17_23_7_59_1482 "USB current UPM's service is disabled !"
#define USB_DRV_I_303_112_2_17_23_7_59_1483 "USB current UPM's service is disabled !"
#define USB_DRV_I_319_112_2_17_23_7_59_1484 "USB current UPM's service is disabled !"
#define USB_DRV_I_450_112_2_17_23_8_0_1485 "USB : Disable all end-point DMA !!"
#define VCOM_USB_151_112_2_17_23_8_1_1486 "USB LDO disable!"
#define VCOM_USB20_302_112_2_17_23_8_4_1487 "Enter VCOM_usb_boot\n"
#define WATCHDOG_PHY_V0_42_112_2_17_23_8_5_1488 "Watch Dog Trace: Watch Dog Value 0x%8.8x"
#define WATCHDOG_PHY_V0_79_112_2_17_23_8_5_1489 "Watch Dog Trace: Watch Dog Control 0x%8.8x"
#define WATCHDOG_PHY_V0_80_112_2_17_23_8_5_1490 "Watch Dog Trace: Watch Dog LOAD    0x%8.8x"
#define WATCHDOG_PHY_V3_100_112_2_17_23_8_5_1491 "Watch Dog Trace: Watch Dog Control 0x%8.8x"
#define WATCHDOG_PHY_V5_115_112_2_17_23_8_6_1492 "Watch Dog Trace: Watch Dog Control 0x%8.8x"
#define WATCHDOG_HAL_44_112_2_17_23_8_6_1493 "Watch Dog Trace: WDG_TimerStop"
#define WATCHDOG_HAL_70_112_2_17_23_8_6_1494 "Watch Dog Trace: WDG_TimerStart"
#define WATCHDOG_HAL_101_112_2_17_23_8_6_1495 "Watch Dog Trace: WDG_TimerInit"
#define WATCHDOG_HAL_121_112_2_17_23_8_6_1496 "Watch Dog Trace: WDG_ResetMCU"
#define WATCHDOG_HAL_139_112_2_17_23_8_6_1497 "Watch Dog Trace: WDG_TimerLoad %d"
#define AUDIO_WIN_125_112_2_17_23_8_21_1498 "AUD_Init: Initialize audio interface."
#define AUDIO_WIN_139_112_2_17_23_8_21_1499 "AUD_SetEngineeringParameter: parameter_type = %d"
#define AUDIO_WIN_152_112_2_17_23_8_21_1500 "AUD_SwitchTestMode: is_test_mode = %d"
#define CHARGE_349_112_2_17_23_8_22_1501 "battery Try send message(%d, %d) to Client"
#define CHARGE_394_112_2_17_23_8_22_1502 "charge : has sent to client. (%d)"
#define CHARGE_428_112_2_17_23_8_22_1503 "is_charge_connect: 0x%X"
#define CHARGE_478_112_2_17_23_8_22_1504 "CHR : Has reach max level"
#define CHARGE_486_112_2_17_23_8_22_1505 "CHR : need charge"
#define CHARGE_491_112_2_17_23_8_22_1506 "CHR : needn't charge"
#define CHARGE_508_112_2_17_23_8_22_1507 "chr_stm( %d, %d )"
#define CHARGE_569_112_2_17_23_8_22_1508 "time slot is too long and need stop charge"
#define CHARGE_582_112_2_17_23_8_22_1509 "CHR: time_slot : %d"
#define CHARGE_657_112_2_17_23_8_22_1510 "The Batt ADC( %d/%d,  %d)"
#define CHARGE_796_112_2_17_23_8_23_1511 "In charge_State and try check connect"
#define CHARGE_800_112_2_17_23_8_23_1512 "charge not connect, and STM"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(CHIP_DRV_TRC)
TRACE_MSG(ADC_PHY_V0_48_112_2_17_23_2_27_0,"[ADC:ADC_Init]")
TRACE_MSG(ADC_PHY_V0_93_112_2_17_23_2_27_1,"[ADC:ADC_ConfigTpcChannel]")
TRACE_MSG(ADC_PHY_V0_105_112_2_17_23_2_27_2,"[ADC:ADC_OpenTpcChannel]")
TRACE_MSG(ADC_PHY_V0_117_112_2_17_23_2_27_3,"[ADC:ADC_CloseTpcChannel]")
TRACE_MSG(ADC_PHY_V0_152_112_2_17_23_2_27_4,"0x%x 0x%x 0x%x 0x%x")
TRACE_MSG(ADC_PHY_V0_153_112_2_17_23_2_27_5,"--%x")
TRACE_MSG(ADC_PHY_V3_54_112_2_17_23_2_28_6,"[ADC:ADC_Init]")
TRACE_MSG(ADC_PHY_V3_96_112_2_17_23_2_28_7,"[ADC:ADC_ConfigTPC]")
TRACE_MSG(ADC_PHY_V3_106_112_2_17_23_2_29_8,"[ADC:ADC_OpenTPC]")
TRACE_MSG(ADC_PHY_V3_116_112_2_17_23_2_29_9,"[ADC:ADC_CloseTPC]")
TRACE_MSG(ADC_PHY_V3_155_112_2_17_23_2_29_10,"[ADC:ADC_GetResultDirectly timeout....")
TRACE_MSG(ADC_PHY_V5_92_112_2_17_23_2_29_11,"ADC_IsrHandler interrupt ERROR\n")
TRACE_MSG(ADC_PHY_V5_100_112_2_17_23_2_29_12,"[ADC_PHY_V5]ADC_IsrHandler.adc_data=%d\n")
TRACE_MSG(ADC_PHY_V5_217_112_2_17_23_2_29_13,"[ADC:ADC_ConfigTPC] TPC_X,Y is bounded to ADC_SLOW_HW_CH0/CH1")
TRACE_MSG(ADC_PHY_V5_224_112_2_17_23_2_29_14,"[ADC:ADC_CloseTPC] is null for sc6530.")
TRACE_MSG(ADC_PHY_V5_262_112_2_17_23_2_29_15,"[ADC:ADC_GetResultDirectly timeout....")
TRACE_MSG(ADC_PHY_V5_273_112_2_17_23_2_29_16,"[ADC:ADC_GetResultDirectly] IN(%d) OUT(%d)")
TRACE_MSG(ADC_PHY_V5___92_112_2_17_23_2_30_17,"ADC_IsrHandler interrupt ERROR\n")
TRACE_MSG(ADC_PHY_V5___100_112_2_17_23_2_30_18,"[ADC_PHY_V5]ADC_IsrHandler.adc_data=%d\n")
TRACE_MSG(ADC_PHY_V5___119_112_2_17_23_2_30_19,"[ADC:ADC_OpenTPC] is null for sc6530.")
TRACE_MSG(ADC_PHY_V5___220_112_2_17_23_2_30_20,"[ADC:ADC_ConfigTPC] TPC_X,Y is bounded to ADC_SLOW_HW_CH0/CH1")
TRACE_MSG(ADC_PHY_V5___227_112_2_17_23_2_30_21,"[ADC:ADC_CloseTPC] is null for sc6530.")
TRACE_MSG(ADC_PHY_V5___265_112_2_17_23_2_30_22,"[ADC:ADC_GetResultDirectly timeout....")
TRACE_MSG(ADC_PHY_V5___276_112_2_17_23_2_30_23,"[ADC:ADC_GetResultDirectly] IN(%d) OUT(%d)")
TRACE_MSG(ANALOG_HAL_155_112_2_17_23_2_32_24,"_ANA_GetDevCurrentMaxValue: old_index = %d, new_index = %d, max_level = %d, vbat = %d, time = %d")
TRACE_MSG(ANALOG_HAL_262_112_2_17_23_2_32_25,"ANA_SetDevValule: can't get analog device table point !! Fail !")
TRACE_MSG(ANALOG_HAL_291_112_2_17_23_2_32_26,"ANA_SetDevValule: id = %d, value = %d, real_value = %d, time = %d")
TRACE_MSG(ANALOG_HAL_295_112_2_17_23_2_32_27,"ANA_SetDevValule: Not Init !!")
TRACE_MSG(ANALOG_PHY_V0_82_112_2_17_23_2_32_28,"_ANA_SetVibrator: Value=%d\r\n")
TRACE_MSG(AUDIO_HAL_186_112_2_17_23_2_35_29,"[AUDDRV][HAL]init magic_num=0x%x")
TRACE_MSG(AUDIO_HAL_198_112_2_17_23_2_35_30,"[AUDDRV][HAL]Init DAI Failed!")
TRACE_MSG(AUDIO_HAL_210_112_2_17_23_2_35_31,"[AUDDRV][HAL]Init CODEC Failed!")
TRACE_MSG(AUDIO_HAL_251_112_2_17_23_2_35_32,"[AUDDRV][HAL]open fun_flg=0x%x,fs=%d")
TRACE_MSG(AUDIO_HAL_259_112_2_17_23_2_35_33,"[AUDDRV][HAL]real open 0x%x")
TRACE_MSG(AUDIO_HAL_272_112_2_17_23_2_35_34,"[AUDDRV][HAL]open dai failed! 0x%x")
TRACE_MSG(AUDIO_HAL_287_112_2_17_23_2_35_35,"[AUDDRV][HAL]open codec failed! 0x%x")
TRACE_MSG(AUDIO_HAL_315_112_2_17_23_2_35_36,"[AUDDRV][HAL]close fun_flg=0x%x")
TRACE_MSG(AUDIO_HAL_323_112_2_17_23_2_35_37,"[AUDDRV][HAL]real close 0x%x")
TRACE_MSG(AUDIO_HAL_328_112_2_17_23_2_35_38,"[AUDDRV][HAL]close dai failed! 0x%x")
TRACE_MSG(AUDIO_HAL_335_112_2_17_23_2_35_39,"[AUDDRV][HAL]close codec failed! 0x%x")
TRACE_MSG(AUDIO_HAL_355_112_2_17_23_2_35_40,"[AUDDRV][HAL]ioctl cmd=%d,arg=0x%x")
TRACE_MSG(AUDIO_HAL_401_112_2_17_23_2_35_41,"[AUDDRV][HAL]dac_set_buf_info pLHead=0x%x,pRHead=0x%x,cnt=%d")
TRACE_MSG(AUDIO_HAL_456_112_2_17_23_2_35_42,"[AUDDRV][HAL]adc_set_buf_info pLHead=0x%x,pRHead=0x%x,cnt=%d")
TRACE_MSG(AUDIO_HAL_492_112_2_17_23_2_35_43,"[AUDDRV][HAL]adc_trigger")
TRACE_MSG(AUDIO_HAL_509_112_2_17_23_2_35_44,"[AUDDRV][HAL]register_playback 0x%x")
TRACE_MSG(AUDIO_HAL_526_112_2_17_23_2_36_45,"[AUDDRV][HAL]register_capture 0x%x")
TRACE_MSG(AUDIO_HAL_547_112_2_17_23_2_36_46,"[AUDDRV][HAL]inc 0x%x,0x%x,ref %d,ret %d")
TRACE_MSG(AUDIO_HAL_566_112_2_17_23_2_36_47,"[AUDDRV][HAL]dec 0x%x,0x%x,ref %d,ret %d")
TRACE_MSG(AUDIO_HAL_883_112_2_17_23_2_36_48,"[AUDDRV][HAL]codec notify %d")
TRACE_MSG(AUDIO_HAL_919_112_2_17_23_2_36_49,"AUDIO CODEC CHR_CAP_IND")
TRACE_MSG(AUDIO_OLD_API_128_112_2_17_23_2_37_50,"[VB_SwitchHpf]:is_enable:%d")
TRACE_MSG(AUDIO_OLD_API_146_112_2_17_23_2_37_51,"[VB_SetHpfMode]:is_reset_mode:%d")
TRACE_MSG(AUDIO_OLD_API_186_112_2_17_23_2_37_52,"[VB_SetHpfGain]:gain_index:%d,gain_value:%d")
TRACE_MSG(AUDIO_OLD_API_200_112_2_17_23_2_37_53,"[VB_GetHpfGain]:gain_index:%d")
TRACE_MSG(AUDIO_OLD_API_213_112_2_17_23_2_37_54,"[VB_SetHpfLimit]:rLimit:%d")
TRACE_MSG(AUDIO_OLD_API_224_112_2_17_23_2_37_55,"[VB_SetHpfWidth]:width:%d")
TRACE_MSG(AUDIO_OLD_API_235_112_2_17_23_2_37_56,"[VB_ALCSwitch]:is_enable:%d")
TRACE_MSG(AUDIO_OLD_API_247_112_2_17_23_2_37_57,"[VB_SetALCParas]:hold:%d,rise:%d,fall:%d,limit:%d,threshold:%d,ratio:%d")
TRACE_MSG(AUDIO_OLD_API_249_112_2_17_23_2_37_58,"[VB_SetALCParas]:cg_var:%d,release_rate:%d,attack_rate:%d,release_rate_ex:%d,attack_rate_ex:%d")
TRACE_MSG(AUDIO_OLD_API_264_112_2_17_23_2_37_59,"[VB_SetFMMixMode]:da_index:%d,mix_mode:%d")
TRACE_MSG(AUDIO_OLD_API_276_112_2_17_23_2_37_60,"[VB_DGSwitch]:da_index:%d,is_enable:%d")
TRACE_MSG(AUDIO_OLD_API_288_112_2_17_23_2_37_61,"[VB_SetDG]:da_index:%d,dg_value:%d")
TRACE_MSG(AUDIO_CODEC_DOLPHIN_253_112_2_17_23_2_38_62,"[AUDDRV][DOL]open err %08x")
TRACE_MSG(AUDIO_CODEC_DOLPHIN_287_112_2_17_23_2_38_63,"[AUDDRV][DOL]close err %08x")
TRACE_MSG(AUDIO_CODEC_DOLPHIN_329_112_2_17_23_2_38_64,"[AUDDRV][DOL]pga param")
TRACE_MSG(AUDIO_CODEC_DOLPHIN_384_112_2_17_23_2_38_65,"[AUDDRV][DOL]drv out 0x%x")
TRACE_MSG(AUDIO_CODEC_DOLPHIN_391_112_2_17_23_2_38_66,"[AUDDRV][DOL]drv out param")
TRACE_MSG(AUDIO_CODEC_DOLPHIN_427_112_2_17_23_2_38_67,"[AUDDRV][DOL]drv in linein?")
TRACE_MSG(AUDIO_CODEC_DOLPHIN_432_112_2_17_23_2_38_68,"[AUDDRV][DOL]drv in 0x%x")
TRACE_MSG(AUDIO_CODEC_DOLPHIN_439_112_2_17_23_2_38_69,"[AUDDRV][DOL]drv in param")
TRACE_MSG(AUDIO_CODEC_DOLPHIN_462_112_2_17_23_2_38_70,"[AUDDRV][DOL]dac mute")
TRACE_MSG(AUDIO_CODEC_DOLPHIN_479_112_2_17_23_2_38_71,"[AUDDRV][DOL]get fs param")
TRACE_MSG(AUDIO_CODEC_DOLPHIN_537_112_2_17_23_2_38_72,"[AUDDRV][DOL]ioctl param")
TRACE_MSG(DOL_PHY_V1_126_112_2_17_23_2_39_73,"_dol_get_sample_rate_map, Can not support samplerate : %d")
TRACE_MSG(DOL_PHY_V1_216_112_2_17_23_2_39_74,"__dol_s2mono %d")
TRACE_MSG(DOL_PHY_V1_240_112_2_17_23_2_39_75,"__dol_mic_sel %d")
TRACE_MSG(DOL_PHY_V1_252_112_2_17_23_2_39_76,"__dol_init")
TRACE_MSG(DOL_PHY_V1_293_112_2_17_23_2_40_77,"__dol_open")
TRACE_MSG(DOL_PHY_V1_302_112_2_17_23_2_40_78,"__dol_reset")
TRACE_MSG(DOL_PHY_V1_321_112_2_17_23_2_40_79,"__dol_sb %d")
TRACE_MSG(DOL_PHY_V1_341_112_2_17_23_2_40_80,"__dol_sb_sleep %d")
TRACE_MSG(DOL_PHY_V1_360_112_2_17_23_2_40_81,"_dol_sb_micbias %d")
TRACE_MSG(DOL_PHY_V1_380_112_2_17_23_2_40_82,"__dol_sb_adc %d")
TRACE_MSG(DOL_PHY_V1_397_112_2_17_23_2_40_83,"__dol_linein_rec %d")
TRACE_MSG(DOL_PHY_V1_424_112_2_17_23_2_40_84,"__dol_sb_dac %d")
TRACE_MSG(DOL_PHY_V1_449_112_2_17_23_2_40_85,"__dol_sb_lin %d")
TRACE_MSG(DOL_PHY_V1_468_112_2_17_23_2_40_86,"__dol_sb_mix %d")
TRACE_MSG(DOL_PHY_V1_488_112_2_17_23_2_40_87,"__dol_sb_lout %d")
TRACE_MSG(DOL_PHY_V1_501_112_2_17_23_2_40_88,"__dol_sb_out %d")
TRACE_MSG(DOL_PHY_V1_540_112_2_17_23_2_40_89,"__dol_dac_mute %d")
TRACE_MSG(DOL_PHY_V1_585_112_2_17_23_2_40_90,"__dol_hp_mute %d")
TRACE_MSG(DOL_PHY_V1_604_112_2_17_23_2_40_91,"__dol_sb_blt %d")
TRACE_MSG(DOL_PHY_V1_625_112_2_17_23_2_40_92,"__dol_ep_mute %d")
TRACE_MSG(DOL_PHY_V1_634_112_2_17_23_2_40_93,"__dol_ramp_done %d 0x%x")
TRACE_MSG(DOL_PHY_V1_663_112_2_17_23_2_40_94,"__dol_clr_ramp %d 0x%x")
TRACE_MSG(DOL_PHY_V1_673_112_2_17_23_2_40_95,"__dol_gain_done %d 0x%x")
TRACE_MSG(DOL_PHY_V1_702_112_2_17_23_2_40_96,"__dol_clr_gain %d 0x%x")
TRACE_MSG(DOL_PHY_V1_714_112_2_17_23_2_40_97,"__dol_set_pag_gim 0x%x")
TRACE_MSG(DOL_PHY_V1_722_112_2_17_23_2_40_98,"__dol_set_pag_gi 0x%x")
TRACE_MSG(DOL_PHY_V1_728_112_2_17_23_2_40_99,"__dol_set_pag_golr 0x%x")
TRACE_MSG(DOL_PHY_V1_736_112_2_17_23_2_40_100,"__dol_set_pag_gobtl 0x%x")
TRACE_MSG(DOL_PHY_V1_760_112_2_17_23_2_41_101,"__dol_set_pag_lrgod %d 0x%x,0x%x")
TRACE_MSG(DOL_PHY_V1_786_112_2_17_23_2_41_102,"__dol_set_pag_lrgo %d 0x%x,0x%x")
TRACE_MSG(DOL_PHY_V1_810_112_2_17_23_2_41_103,"__dol_set_pag_lrgob %d 0x%x,0x%x")
TRACE_MSG(DOL_PHY_V1_940_112_2_17_23_2_41_104,"__dol_set_da_samplerate : samplerate: %d")
TRACE_MSG(DOL_PHY_V1_943_112_2_17_23_2_41_105,"__dol_set_da_samplerate, invalid DA samplerate: %d")
TRACE_MSG(DOL_PHY_V1_958_112_2_17_23_2_41_106,"__dol_set_ad_samplerate : samplerate: %d")
TRACE_MSG(DOL_PHY_V1_962_112_2_17_23_2_41_107,"__dol_set_ad_samplerate, invalid AD samplerate: %d")
TRACE_MSG(DOL_PHY_V1_992_112_2_17_23_2_41_108,"[REG] VBAICR =0x%x")
TRACE_MSG(DOL_PHY_V1_993_112_2_17_23_2_41_109,"[REG] VBCR1  =0x%x")
TRACE_MSG(DOL_PHY_V1_994_112_2_17_23_2_41_110,"[REG] VBCR2  =0x%x")
TRACE_MSG(DOL_PHY_V1_995_112_2_17_23_2_41_111,"[REG] VBCCR1 =0x%x")
TRACE_MSG(DOL_PHY_V1_996_112_2_17_23_2_41_112,"[REG] VBCCR2 =0x%x")
TRACE_MSG(DOL_PHY_V1_997_112_2_17_23_2_41_113,"[REG] VBPMR1 =0x%x")
TRACE_MSG(DOL_PHY_V1_998_112_2_17_23_2_41_114,"[REG] VBPMR2 =0x%x")
TRACE_MSG(DOL_PHY_V1_999_112_2_17_23_2_41_115,"[REG] VBCRR  =0x%x")
TRACE_MSG(DOL_PHY_V1_1000_112_2_17_23_2_41_116,"[REG] VBICR  =0x%x")
TRACE_MSG(DOL_PHY_V1_1001_112_2_17_23_2_41_117,"[REG] VBIFR  =0x%x")
TRACE_MSG(DOL_PHY_V1_1002_112_2_17_23_2_41_118,"[REG] VBCGR1 =0x%x")
TRACE_MSG(DOL_PHY_V1_1003_112_2_17_23_2_41_119,"[REG] VBCGR2 =0x%x")
TRACE_MSG(DOL_PHY_V1_1004_112_2_17_23_2_41_120,"[REG] VBCGR3 =0x%x")
TRACE_MSG(DOL_PHY_V1_1005_112_2_17_23_2_41_121,"[REG] VBCGR8 =0x%x")
TRACE_MSG(DOL_PHY_V1_1006_112_2_17_23_2_41_122,"[REG] VBCGR9 =0x%x")
TRACE_MSG(DOL_PHY_V1_1007_112_2_17_23_2_41_123,"[REG] VBCGR10=0x%x")
TRACE_MSG(DOL_PHY_V1_1008_112_2_17_23_2_41_124,"[REG] VBTR1  =0x%x")
TRACE_MSG(DOL_PHY_V1_1009_112_2_17_23_2_41_125,"[REG] VBTR2  =0x%x")
TRACE_MSG(DOL_PHY_V1_1042_112_2_17_23_2_41_126,"__dol_ext_pa_ctl:%d")
TRACE_MSG(DOL_PHY_V1_1048_112_2_17_23_2_41_127,"__dol_ext_hp_pa_ctl:%d")
TRACE_MSG(DOL_STATE_V1_412_112_2_17_23_2_42_128,"[DOLST]DOL_SM_Process")
TRACE_MSG(DOL_STATE_V1_416_112_2_17_23_2_42_129,"[DOLST]arm not run")
TRACE_MSG(DOL_STATE_V1_474_112_2_17_23_2_42_130,"[DOLST]CHANGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d")
TRACE_MSG(DOL_STATE_V1_697_112_2_17_23_2_43_131,"[DOLST]on_stop pre=%s,cur=%s")
TRACE_MSG(DOL_STATE_V1_1057_112_2_17_23_2_43_132,"[AUDDRV][DOL]SB_OUT is on")
TRACE_MSG(DOL_STATE_V1_1068_112_2_17_23_2_43_133,"[AUDDRV][DOL]RAMP is UP")
TRACE_MSG(DOL_STATE_V1_1114_112_2_17_23_2_43_134,"[AUDDRV][DOL]SB_OUT is off")
TRACE_MSG(DOL_STATE_V1_1125_112_2_17_23_2_43_135,"[AUDDRV][DOL]RAMP is DOWN")
TRACE_MSG(DOL_STATE_V1_1207_112_2_17_23_2_44_136,"[AUDDRV][DOL]UnMuted")
TRACE_MSG(DOL_STATE_V1_1218_112_2_17_23_2_44_137,"[AUDDRV][DOL]GAIN is UP")
TRACE_MSG(DOL_STATE_V1_1258_112_2_17_23_2_44_138,"[AUDDRV][DOL]Muted")
TRACE_MSG(DOL_STATE_V1_1269_112_2_17_23_2_44_139,"[AUDDRV][DOL]GAIN is DOWN")
TRACE_MSG(DOL_STATE_V1_1587_112_2_17_23_2_44_140,"[DOLST]MERGED ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d")
TRACE_MSG(DOL_STATE_V1_1610_112_2_17_23_2_44_141,"[DOLST] MERGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d")
TRACE_MSG(DOL_STATE_V1_1625_112_2_17_23_2_44_142,"[DOLST] APPLY ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d")
TRACE_MSG(DOL_STATE_V1_1647_112_2_17_23_2_44_143,"[DOLST]change state pre=%s,cur=%s,set=%s")
TRACE_MSG(DOL_STATE_V1_1861_112_2_17_23_2_45_144,"[DOLST]index=%d")
TRACE_MSG(DOLPHIN_PHY_CFG_78_112_2_17_23_2_45_145,"[AUDDRV][DOL]real set pga 0x%x")
TRACE_MSG(DOLPHIN_PHY_CFG_83_112_2_17_23_2_45_146,"[AUDDRV][DOL]find pga 0x%x")
TRACE_MSG(DOLPHIN_PHY_CFG_104_112_2_17_23_2_45_147,"[AUDDRV][DOL]all pga_mode 0x%x")
TRACE_MSG(DOLPHIN_PHY_CFG_109_112_2_17_23_2_45_148,"[AUDDRV][DOL]set pga_mode 0x%x")
TRACE_MSG(DOLPHIN_PHY_CFG_153_112_2_17_23_2_45_149,"[AUDDRV][DOL]pga set")
TRACE_MSG(AUDIO_CODEC_LM49370_100_112_2_17_23_2_46_150,"LM49370_Config : Unconfigurable parameter!")
TRACE_MSG(AUDIO_CODEC_LM49370_197_112_2_17_23_2_46_151,"LM49370 set Fail!\r\n")
TRACE_MSG(AUDIO_CODEC_LM49370_211_112_2_17_23_2_46_152,"[AUDDRV][LM49370]init lm49370")
TRACE_MSG(AUDIO_CODEC_LM49370_229_112_2_17_23_2_46_153,"[AUDDRV][LM49370]open lm49370 0x%x,%d")
TRACE_MSG(AUDIO_CODEC_LM49370_254_112_2_17_23_2_46_154,"[AUDDRV][LM49370]close lm49370 0x%x")
TRACE_MSG(AUDIO_CODEC_LM49370_274_112_2_17_23_2_46_155,"[AUDDRV][LM49370]ioctl lm49370 %d,0x%x")
TRACE_MSG(AUDIO_CODEC_LM49370_295_112_2_17_23_2_46_156,"[AUDDRV][LM49370]ioctl param")
TRACE_MSG(SPI_LM49370_V3_39_112_2_17_23_2_46_157,"SCM: This SPI Port Has been occupied!")
TRACE_MSG(SPI_LM49370_V3_121_112_2_17_23_2_46_158,"Error in LM49370 Register 0x%02XrnValue Write In : 0x%02XrnValue Read Out : 0x%02Xrnrn")
TRACE_MSG(SPI_LM49370_V3_127_112_2_17_23_2_46_159,"\r\nLM49370 Register Test Done, OK!\r\n")
TRACE_MSG(SPI_LM49370_V3_1361_112_2_17_23_2_49_160,"Do not support 25bit per channel in IIS mode!(44.1KHz)\r\n")
TRACE_MSG(SPI_LM49370_V3_1408_112_2_17_23_2_49_161,"Codec LM49370 do not support 8bit per channel in PCM mode!(44.1KHz)\r\n")
TRACE_MSG(SPI_LM49370_V3_1461_112_2_17_23_2_49_162,"Do not support 25bit per channel in IIS mode!\r\n")
TRACE_MSG(SPI_LM49370_V3_1505_112_2_17_23_2_49_163,"Do not support 25bit per channel in IIS mode!\r\n")
TRACE_MSG(SPI_LM49370_V3_1613_112_2_17_23_2_49_164,"Do not support 25bit per channel in IIS mode!\r\n")
TRACE_MSG(SPI_LM49370_V3_1717_112_2_17_23_2_50_165,"Do not support 25bit per channel in IIS mode!(16KHz)\r\n")
TRACE_MSG(SPI_LM49370_V3_1826_112_2_17_23_2_50_166,"Do not support 25bit per channel in IIS mode!(32KHz)\r\n")
TRACE_MSG(SPI_LM49370_V3_1941_112_2_17_23_2_50_167,"Do not support 25bit per channel in IIS mode!(48KHz)\r\n")
TRACE_MSG(SPI_LM49370_V3_1987_112_2_17_23_2_50_168,"Codec LM49370 do not support 8bit per channel in PCM mode!(48KHz)\r\n")
TRACE_MSG(AUDIO_CODEC_SPRD_253_112_2_17_23_2_51_169,"[AUDDRV][SPRD]init sprd_codec")
TRACE_MSG(AUDIO_CODEC_SPRD_278_112_2_17_23_2_51_170,"[AUDDRV][SPRD]open sprd_codec 0x%x,%d")
TRACE_MSG(AUDIO_CODEC_SPRD_282_112_2_17_23_2_51_171,"[AUDDRV][SPRD]open err %08x")
TRACE_MSG(AUDIO_CODEC_SPRD_316_112_2_17_23_2_51_172,"[AUDDRV][SPRD]close sprd_codec 0x%x")
TRACE_MSG(AUDIO_CODEC_SPRD_326_112_2_17_23_2_51_173,"[AUDDRV][SPRD]close err %08x")
TRACE_MSG(AUDIO_CODEC_SPRD_368_112_2_17_23_2_51_174,"[AUDDRV][SPRD]pga param")
TRACE_MSG(AUDIO_CODEC_SPRD_382_112_2_17_23_2_52_175,"[AUDDRV][SPRD]drv out ctl 0x%x,%d")
TRACE_MSG(AUDIO_CODEC_SPRD_417_112_2_17_23_2_52_176,"[AUDDRV][SPRD]drv out 0x%x")
TRACE_MSG(AUDIO_CODEC_SPRD_424_112_2_17_23_2_52_177,"[AUDDRV][SPRD]drv out param")
TRACE_MSG(AUDIO_CODEC_SPRD_437_112_2_17_23_2_52_178,"[AUDDRV][SPRD]drv in ctl 0x%x,%d")
TRACE_MSG(AUDIO_CODEC_SPRD_472_112_2_17_23_2_52_179,"[AUDDRV][SPRD]drv in 0x%x")
TRACE_MSG(AUDIO_CODEC_SPRD_479_112_2_17_23_2_52_180,"[AUDDRV][SPRD]drv in param")
TRACE_MSG(AUDIO_CODEC_SPRD_502_112_2_17_23_2_52_181,"[AUDDRV][SPRD]dac mute")
TRACE_MSG(AUDIO_CODEC_SPRD_519_112_2_17_23_2_52_182,"[AUDDRV][SPRD]get fs param")
TRACE_MSG(AUDIO_CODEC_SPRD_550_112_2_17_23_2_52_183,"[AUDDRV][SPRD]get fs param")
TRACE_MSG(AUDIO_CODEC_SPRD_569_112_2_17_23_2_52_184,"[AUDDRV][SPRD]ioctl sprd_codec %d,0x%x")
TRACE_MSG(AUDIO_CODEC_SPRD_606_112_2_17_23_2_52_185,"[AUDDRV][SPRD]ioctl param")
TRACE_MSG(AUDIO_CODEC_SPRD_619_112_2_17_23_2_52_186,"[AUDDRV][SPRD]PA %d,%d,0x%x")
TRACE_MSG(AUDIO_CODEC_SPRD_638_112_2_17_23_2_52_187,"[AUDDRV][SPRD]HP %d,%d,0x%x")
TRACE_MSG(AUDIO_CODEC_SPRD_665_112_2_17_23_2_52_188,"[AUDDRV][SPRD]EP %d,%d,0x%x")
TRACE_MSG(AUDIO_CODEC_SPRD_680_112_2_17_23_2_52_189,"[AUDDRV][SPRD]ADCPGA %d,%d,0x%x")
TRACE_MSG(AUDIO_CODEC_SPRD_767_112_2_17_23_2_52_190,"_run_callback:%d")
TRACE_MSG(AUDIO_CODEC_SPRD_775_112_2_17_23_2_52_191,"AUDIO_PA_Ctl:%d,%d")
TRACE_MSG(SPRD_CODEC_PHY_99_112_2_17_23_2_53_192,"__sprd_codec_hp_mute:%d")
TRACE_MSG(SPRD_CODEC_PHY_116_112_2_17_23_2_53_193,"__sprd_codec_ep_mute:%d")
TRACE_MSG(SPRD_CODEC_PHY_131_112_2_17_23_2_53_194,"__sprd_codec_ext_pa_ctl:%d,%d")
TRACE_MSG(SPRD_CODEC_PHY_137_112_2_17_23_2_53_195,"__sprd_codec_ext_hp_pa_ctl:%d")
TRACE_MSG(SPRD_CODEC_PHY_150_112_2_17_23_2_53_196,"__sprd_codec_update_ldo_vol %d")
TRACE_MSG(SPRD_CODEC_PHY_CFG_81_112_2_17_23_2_53_197,"[AUDDRV][SPRD_CODEC]real set pga 0x%x")
TRACE_MSG(SPRD_CODEC_PHY_CFG_86_112_2_17_23_2_53_198,"[AUDDRV][SPRD_CODEC]find pga 0x%x")
TRACE_MSG(SPRD_CODEC_PHY_CFG_107_112_2_17_23_2_53_199,"[AUDDRV][SPRD_CODEC]all pga_mode 0x%x")
TRACE_MSG(SPRD_CODEC_PHY_CFG_112_112_2_17_23_2_53_200,"[AUDDRV][SPRD_CODEC]set pga_mode 0x%x")
TRACE_MSG(SPRD_CODEC_PHY_CFG_156_112_2_17_23_2_53_201,"[AUDDRV][SPRD_CODEC]pga set")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_234_112_2_17_23_2_54_202,"__sprd_codec_ap_en %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_327_112_2_17_23_2_54_203,"__sprd_codec_ap_adc %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_347_112_2_17_23_2_54_204,"__sprd_codec_ap_linein_rec %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_355_112_2_17_23_2_54_205,"__sprd_codec_ap_dac_pwr %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_361_112_2_17_23_2_54_206,"__sprd_codec_ap_dac %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_416_112_2_17_23_2_54_207,"__sprd_codec_ap_lin_adc %d,%x")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_470_112_2_17_23_2_54_208,"__sprd_codec_ap_lin %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_510_112_2_17_23_2_55_209,"__sprd_codec_ap_ear %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_518_112_2_17_23_2_55_210,"__sprd_codec_ap_lout %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_526_112_2_17_23_2_55_211,"__sprd_codec_ap_ear_en %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_587_112_2_17_23_2_55_212,"__sprd_codec_ap_hp %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_593_112_2_17_23_2_55_213,"__sprd_codec_ap_hp_pop_en %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_637_112_2_17_23_2_55_214,"__sprd_codec_ap_mic_bias %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_652_112_2_17_23_2_55_215,"__sprd_codec_ap_main_mic %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_664_112_2_17_23_2_55_216,"__sprd_codec_ap_aux_mic %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_676_112_2_17_23_2_55_217,"__sprd_codec_ap_hp_mic %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_696_112_2_17_23_2_55_218,"__sprd_codec_ap_dac_clk_en %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_761_112_2_17_23_2_55_219,"__sprd_codec_ap_pa_dtri_f_sel %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_770_112_2_17_23_2_55_220,"__sprd_codec_ap_pa_demi_en %d")
TRACE_MSG(SPRD_CODEC_AP_PHY_V0_850_112_2_17_23_2_55_221,"0x%02x | 0x%02x 0x%02x 0x%02x 0x%02x")
TRACE_MSG(SPRD_CODEC_DP_PHY_V0_251_112_2_17_23_2_56_222,"__sprd_codec_dp_dac_mute_div0 %d")
TRACE_MSG(SPRD_CODEC_DP_PHY_V0_269_112_2_17_23_2_56_223,"__sprd_codec_dp_dac_mute_en %d")
TRACE_MSG(SPRD_CODEC_DP_PHY_V0_484_112_2_17_23_2_56_224,"0x%02x | 0x%04x 0x%04x 0x%04x 0x%04x")
TRACE_MSG(SPRD_CODEC_STATE_V0_425_112_2_17_23_2_57_225,"[AUDST]SPRD_CODEC_SM_Process")
TRACE_MSG(SPRD_CODEC_STATE_V0_429_112_2_17_23_2_57_226,"[AUDST]arm not run")
TRACE_MSG(SPRD_CODEC_STATE_V0_488_112_2_17_23_2_57_227,"[AUDST]CHANGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,ep=%d,mu=%d")
TRACE_MSG(SPRD_CODEC_STATE_V0_707_112_2_17_23_2_58_228,"[AUDST]on_stop pre=%s,cur=%s")
TRACE_MSG(SPRD_CODEC_STATE_V0_972_112_2_17_23_2_59_229,"[AUDDRV][SPRD]hp pop is up")
TRACE_MSG(SPRD_CODEC_STATE_V0_984_112_2_17_23_2_59_230,"[AUDDRV][SPRD]RAMP UP")
TRACE_MSG(SPRD_CODEC_STATE_V0_1030_112_2_17_23_2_59_231,"[AUDDRV][SPRD]hp pop is down")
TRACE_MSG(SPRD_CODEC_STATE_V0_1042_112_2_17_23_2_59_232,"[AUDDRV][SPRD]RAMP DOWN")
TRACE_MSG(SPRD_CODEC_STATE_V0_1239_112_2_17_23_2_59_233,"[AUDDRV][SPRD]UnMuted")
TRACE_MSG(SPRD_CODEC_STATE_V0_1250_112_2_17_23_2_59_234,"[AUDDRV][SPRD]GAIN is UP")
TRACE_MSG(SPRD_CODEC_STATE_V0_1283_112_2_17_23_2_59_235,"[AUDDRV][SPRD]Muted")
TRACE_MSG(SPRD_CODEC_STATE_V0_1294_112_2_17_23_2_59_236,"[AUDDRV][SPRD]GAIN is DOWN")
TRACE_MSG(SPRD_CODEC_STATE_V0_1649_112_2_17_23_3_0_237,"[AUDST]MERGED ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,ep=%d,mu=%d")
TRACE_MSG(SPRD_CODEC_STATE_V0_1673_112_2_17_23_3_0_238,"[AUDST] MERGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,ep=%d,mu=%d")
TRACE_MSG(SPRD_CODEC_STATE_V0_1689_112_2_17_23_3_0_239,"[AUDST] APPLY ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,ep=%d,mu=%d")
TRACE_MSG(SPRD_CODEC_STATE_V0_1712_112_2_17_23_3_0_240,"[AUDST]change state pre=%s,cur=%s,set=%s")
TRACE_MSG(SPRD_CODEC_STATE_V0_1926_112_2_17_23_3_0_241,"[AUDST]index=%d")
TRACE_MSG(AUDIO_CODEC_USC15241_212_112_2_17_23_3_1_242,"[AUDDRV][USC15241]init usc15241")
TRACE_MSG(AUDIO_CODEC_USC15241_236_112_2_17_23_3_1_243,"[AUDDRV][USC15241]open usc15241 0x%x,%d")
TRACE_MSG(AUDIO_CODEC_USC15241_240_112_2_17_23_3_1_244,"[AUDDRV][USC15241]open err %08x")
TRACE_MSG(AUDIO_CODEC_USC15241_270_112_2_17_23_3_1_245,"[AUDDRV][USC15241]close usc15241 0x%x")
TRACE_MSG(AUDIO_CODEC_USC15241_274_112_2_17_23_3_1_246,"[AUDDRV][USC15241]close err %08x")
TRACE_MSG(AUDIO_CODEC_USC15241_316_112_2_17_23_3_1_247,"[AUDDRV][USC15241]pga param")
TRACE_MSG(AUDIO_CODEC_USC15241_342_112_2_17_23_3_1_248,"[AUDDRV][USC15241]drv out ctl 0x%x,%d")
TRACE_MSG(AUDIO_CODEC_USC15241_371_112_2_17_23_3_1_249,"[AUDDRV][USC15241]drv out 0x%x")
TRACE_MSG(AUDIO_CODEC_USC15241_378_112_2_17_23_3_1_250,"[AUDDRV][USC15241]drv out param")
TRACE_MSG(AUDIO_CODEC_USC15241_413_112_2_17_23_3_2_251,"[AUDDRV][USC15241]drv in 0x%x")
TRACE_MSG(AUDIO_CODEC_USC15241_420_112_2_17_23_3_2_252,"[AUDDRV][USC15241]drv in param")
TRACE_MSG(AUDIO_CODEC_USC15241_443_112_2_17_23_3_2_253,"[AUDDRV][USC15241]dac mute")
TRACE_MSG(AUDIO_CODEC_USC15241_460_112_2_17_23_3_2_254,"[AUDDRV][USC15241]get fs param")
TRACE_MSG(AUDIO_CODEC_USC15241_480_112_2_17_23_3_2_255,"[AUDDRV][USC15241]ioctl usc15241 %d,0x%x")
TRACE_MSG(AUDIO_CODEC_USC15241_506_112_2_17_23_3_2_256,"[AUDDRV][USC15241]ioctl param")
TRACE_MSG(AUDIO_CODEC_USC15241_519_112_2_17_23_3_2_257,"[AUDDRV][USC15241]GOD %d,0x%x")
TRACE_MSG(AUDIO_CODEC_USC15241_543_112_2_17_23_3_2_258,"[AUDDRV][USC15241]GO %d,0x%x")
TRACE_MSG(AUDIO_CODEC_USC15241_566_112_2_17_23_3_2_259,"[AUDDRV][USC15241]GOBTL %d,0x%x")
TRACE_MSG(AUDIO_CODEC_USC15241_581_112_2_17_23_3_2_260,"[AUDDRV][USC15241]GOB %d,0x%x")
TRACE_MSG(AUDIO_CODEC_USC15241_604_112_2_17_23_3_2_261,"[AUDDRV][USC15241]GIM %d,0x%x")
TRACE_MSG(AUDIO_CODEC_USC15241_618_112_2_17_23_3_2_262,"[AUDDRV][USC15241]GI %d,0x%x")
TRACE_MSG(AUDIO_CODEC_USC15241_632_112_2_17_23_3_2_263,"[AUDDRV][USC15241]GOLR %d,0x%x")
TRACE_MSG(AUDIO_CODEC_USC15241_706_112_2_17_23_3_2_264,"_run_callback:%d")
TRACE_MSG(USC15241_124_112_2_17_23_3_4_265,"__usc15241_init")
TRACE_MSG(USC15241_131_112_2_17_23_3_4_266,"__usc15241_en %d")
TRACE_MSG(USC15241_187_112_2_17_23_3_4_267,"__usc15241_adc %d")
TRACE_MSG(USC15241_264_112_2_17_23_3_4_268,"__usc15241_dac_pwr %d")
TRACE_MSG(USC15241_279_112_2_17_23_3_4_269,"__usc15241_dac %d")
TRACE_MSG(USC15241_361_112_2_17_23_3_4_270,"USC15241 I2C/SPI TEST START")
TRACE_MSG(USC15241_369_112_2_17_23_3_4_271,"USE_I2C def reg(0x%x)=0x%x")
TRACE_MSG(USC15241_382_112_2_17_23_3_4_272,"USE_SPI wr reg(0x%x)=0x%x")
TRACE_MSG(USC15241_391_112_2_17_23_3_4_273,"USE_SPI wr2 reg(0x%x)=0x%x")
TRACE_MSG(USC15241_400_112_2_17_23_3_4_274,"USE_I2C test FAILE!")
TRACE_MSG(USC15241_404_112_2_17_23_3_4_275,"USE_SPI test FAILE!")
TRACE_MSG(USC15241_420_112_2_17_23_3_4_276,"USC15241 I2C/SPI TEST END\r\n")
TRACE_MSG(USC15241_431_112_2_17_23_3_4_277,"USC15241 REG")
TRACE_MSG(USC15241_435_112_2_17_23_3_4_278,"I2C[0x%02x]=0x%x")
TRACE_MSG(USC15241_441_112_2_17_23_3_4_279,"SPI[%04d]=0x%x")
TRACE_MSG(USC15241_501_112_2_17_23_3_4_280,"__usc15241_ext_pa_ctl:%d")
TRACE_MSG(USC15241_507_112_2_17_23_3_4_281,"__usc15241_ext_hp_pa_ctl:%d")
TRACE_MSG(USC15241_PHY_CFG_77_112_2_17_23_3_5_282,"[AUDDRV][USC15241]real set pga 0x%x")
TRACE_MSG(USC15241_PHY_CFG_82_112_2_17_23_3_5_283,"[AUDDRV][USC15241]find pga 0x%x")
TRACE_MSG(USC15241_PHY_CFG_103_112_2_17_23_3_5_284,"[AUDDRV][USC15241]all pga_mode 0x%x")
TRACE_MSG(USC15241_PHY_CFG_108_112_2_17_23_3_5_285,"[AUDDRV][USC15241]set pga_mode 0x%x")
TRACE_MSG(USC15241_PHY_CFG_152_112_2_17_23_3_5_286,"[AUDDRV][USC15241]pga set")
TRACE_MSG(USC15241_STATE_V0_414_112_2_17_23_3_6_287,"[USC15241ST]USC15241_SM_Process")
TRACE_MSG(USC15241_STATE_V0_418_112_2_17_23_3_6_288,"[USC15241ST]arm not run")
TRACE_MSG(USC15241_STATE_V0_476_112_2_17_23_3_6_289,"[USC15241ST]CHANGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d")
TRACE_MSG(USC15241_STATE_V0_655_112_2_17_23_3_6_290,"[USC15241ST]on_stop pre=%s,cur=%s")
TRACE_MSG(USC15241_STATE_V0_1058_112_2_17_23_3_7_291,"[AUDDRV][USC15241]UnMuted")
TRACE_MSG(USC15241_STATE_V0_1069_112_2_17_23_3_7_292,"[AUDDRV][USC15241]GAIN is UP")
TRACE_MSG(USC15241_STATE_V0_1103_112_2_17_23_3_7_293,"[AUDDRV][USC15241]Muted")
TRACE_MSG(USC15241_STATE_V0_1114_112_2_17_23_3_7_294,"[AUDDRV][USC15241]GAIN is DOWN")
TRACE_MSG(USC15241_STATE_V0_1432_112_2_17_23_3_8_295,"[USC15241ST]MERGED ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d")
TRACE_MSG(USC15241_STATE_V0_1455_112_2_17_23_3_8_296,"[USC15241ST] MERGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d")
TRACE_MSG(USC15241_STATE_V0_1470_112_2_17_23_3_8_297,"[USC15241ST] APPLY ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d")
TRACE_MSG(USC15241_STATE_V0_1492_112_2_17_23_3_8_298,"[USC15241ST]change state pre=%s,cur=%s,set=%s")
TRACE_MSG(USC15241_STATE_V0_1706_112_2_17_23_3_9_299,"[USC15241ST]index=%d")
TRACE_MSG(AUDIO_DAI_VBC_266_112_2_17_23_3_10_300,"[AUDDRV][VBC]enable")
TRACE_MSG(AUDIO_DAI_VBC_273_112_2_17_23_3_10_301,"[AUDDRV][VBC]vbc trigger,!open")
TRACE_MSG(AUDIO_DAI_VBC_299_112_2_17_23_3_11_302,"[AUDDRV][VBC]da pLHead=0")
TRACE_MSG(AUDIO_DAI_VBC_311_112_2_17_23_3_11_303,"[AUDDRV][VBC]da pRHead=0")
TRACE_MSG(AUDIO_DAI_VBC_321_112_2_17_23_3_11_304,"[AUDDRV][VBC]da get buffer:chan=0x%x")
TRACE_MSG(AUDIO_DAI_VBC_352_112_2_17_23_3_11_305,"[AUDDRV][VBC][REF]no data send")
TRACE_MSG(AUDIO_DAI_VBC_356_112_2_17_23_3_11_306,"[AUDDRV][VBC]start send")
TRACE_MSG(AUDIO_DAI_VBC_389_112_2_17_23_3_11_307,"[AUDDRV][VBC]dma send err")
TRACE_MSG(AUDIO_DAI_VBC_405_112_2_17_23_3_11_308,"[AUDDRV][VBC]ad pLHead=0")
TRACE_MSG(AUDIO_DAI_VBC_417_112_2_17_23_3_11_309,"[AUDDRV][VBC]ad pRHead=0")
TRACE_MSG(AUDIO_DAI_VBC_427_112_2_17_23_3_11_310,"[AUDDRV][VBC]ad get buffer:chan=0x%x")
TRACE_MSG(AUDIO_DAI_VBC_453_112_2_17_23_3_11_311,"[AUDDRV][VBC][REF]recv data not write")
TRACE_MSG(AUDIO_DAI_VBC_457_112_2_17_23_3_11_312,"[AUDDRV][VBC]start recv")
TRACE_MSG(AUDIO_DAI_VBC_488_112_2_17_23_3_11_313,"[AUDDRV][VBC]dma recv err")
TRACE_MSG(AUDIO_DAI_VBC_529_112_2_17_23_3_11_314,"[AUDDRV][VBC]dma start:path=0x%x,chan=0x%x")
TRACE_MSG(AUDIO_DAI_VBC_545_112_2_17_23_3_11_315,"[AUDDRV][VBC]dma start path=0x%x,xc0 %d,x1 %d")
TRACE_MSG(AUDIO_DAI_VBC_591_112_2_17_23_3_11_316,"[AUDDRV][VBC]tried")
TRACE_MSG(AUDIO_DAI_VBC_732_112_2_17_23_3_11_317,"[AUDDRV][VBC]base action 0x%x,%d,%d")
TRACE_MSG(AUDIO_DAI_VBC_739_112_2_17_23_3_12_318,"[AUDDRV][VBC]dac0")
TRACE_MSG(AUDIO_DAI_VBC_752_112_2_17_23_3_12_319,"[AUDDRV][VBC]dac1")
TRACE_MSG(AUDIO_DAI_VBC_768_112_2_17_23_3_12_320,"[AUDDRV][VBC]adc0")
TRACE_MSG(AUDIO_DAI_VBC_781_112_2_17_23_3_12_321,"[AUDDRV][VBC]adc1")
TRACE_MSG(AUDIO_DAI_VBC_802_112_2_17_23_3_12_322,"[AUDDRV][VBC]dsp open, must close all other!")
TRACE_MSG(AUDIO_DAI_VBC_811_112_2_17_23_3_12_323,"[AUDDRV][VBC]dsp, must close dsp,%08x")
TRACE_MSG(AUDIO_DAI_VBC_824_112_2_17_23_3_12_324,"[AUDDRV][VBC]fm da , can't dsp,%08x")
TRACE_MSG(AUDIO_DAI_VBC_833_112_2_17_23_3_12_325,"[AUDDRV][VBC]fm ad, can't dsp,%08x")
TRACE_MSG(AUDIO_DAI_VBC_840_112_2_17_23_3_12_326,"[AUDDRV][VBC]fm ad , can't ad,%08x")
TRACE_MSG(AUDIO_DAI_VBC_850_112_2_17_23_3_12_327,"[AUDDRV][VBC]ext action 0x%x,%d")
TRACE_MSG(AUDIO_DAI_VBC_1002_112_2_17_23_3_12_328,"[AUDDRV][VBC]init vbc")
TRACE_MSG(AUDIO_DAI_VBC_1044_112_2_17_23_3_12_329,"[AUDDRV][VBC]open vbc 0x%x,%d")
TRACE_MSG(AUDIO_DAI_VBC_1048_112_2_17_23_3_12_330,"[AUDDRV][VBC]open err %08x")
TRACE_MSG(AUDIO_DAI_VBC_1053_112_2_17_23_3_12_331,"[AUDDRV][VBC]real open vbc")
TRACE_MSG(AUDIO_DAI_VBC_1083_112_2_17_23_3_12_332,"[AUDDRV][VBC]close vbc 0x%x")
TRACE_MSG(AUDIO_DAI_VBC_1088_112_2_17_23_3_12_333,"[AUDDRV][VBC]close err %08x")
TRACE_MSG(AUDIO_DAI_VBC_1105_112_2_17_23_3_12_334,"[AUDDRV][VBC]real close vbc")
TRACE_MSG(AUDIO_DAI_VBC_1131_112_2_17_23_3_12_335,"[AUDDRV][VBC]ioctl vbc %d,0x%x")
TRACE_MSG(AUDIO_DAI_VBC_1146_112_2_17_23_3_12_336,"[AUDDRV][VBC]da used")
TRACE_MSG(AUDIO_DAI_VBC_1156_112_2_17_23_3_12_337,"[AUDDRV][VBC]da df err")
TRACE_MSG(AUDIO_DAI_VBC_1166_112_2_17_23_3_12_338,"[AUDDRV][VBC]ad used")
TRACE_MSG(AUDIO_DAI_VBC_1176_112_2_17_23_3_12_339,"[AUDDRV][VBC]ad df err")
TRACE_MSG(AUDIO_DAI_VBC_1182_112_2_17_23_3_12_340,"[AUDDRV][VBC]set:dir err")
TRACE_MSG(AUDIO_DAI_VBC_1198_112_2_17_23_3_12_341,"[AUDDRV][VBC]get:dir err")
TRACE_MSG(AUDIO_DAI_VBC_1213_112_2_17_23_3_12_342,"[AUDDRV][VBC]set da buffer div:%d")
TRACE_MSG(AUDIO_DAI_VBC_1219_112_2_17_23_3_13_343,"[AUDDRV][VBC]set ad buffer div:%d")
TRACE_MSG(AUDIO_DAI_VBC_1297_112_2_17_23_3_13_344,"[AUDDRV][VBC]set dac buffer 0x%x,0x%x,%d")
TRACE_MSG(AUDIO_DAI_VBC_1378_112_2_17_23_3_13_345,"[AUDDRV][VBC][REF]why trigger?")
TRACE_MSG(AUDIO_DAI_VBC_1405_112_2_17_23_3_13_346,"[AUDDRV][VBC]set adc buffer 0x%x,0x%x,%d")
TRACE_MSG(AUDIO_DAI_VBC_1464_112_2_17_23_3_13_347,"[AUDDRV][VBC]adc trigger")
TRACE_MSG(AUDIO_DAI_VBC_1487_112_2_17_23_3_13_348,"[AUDDRV][VBC]register dac CB 0x%x")
TRACE_MSG(AUDIO_DAI_VBC_1507_112_2_17_23_3_13_349,"[AUDDRV][VBC]register adc CB 0x%x")
TRACE_MSG(AUDIO_DAI_VBC_1517_112_2_17_23_3_13_350,"[AUDDRV][VBC]codec notify %d")
TRACE_MSG(AUDIO_DAI_VBC_1524_112_2_17_23_3_13_351,"[AUDDRV][VBC]switch to dsp")
TRACE_MSG(AUDIO_DAI_VBC_1542_112_2_17_23_3_13_352,"[AUDDRV][VBC]close all")
TRACE_MSG(AUDIO_DAI_VBC_1561_112_2_17_23_3_13_353,"[AUDDRV][VBC]codec notfiy err")
TRACE_MSG(VBC_ALG_PHY_V3_420_112_2_17_23_3_15_354,"vbc_alg_phy_v3.c,[VB_PHY_SetFMMixMode] invalid da index :%d")
TRACE_MSG(VBC_ALG_PHY_V3_458_112_2_17_23_3_15_355,"vbc_alg_phy_v3.c,[VB_PHY_DGSwitch] invalid da index :%d")
TRACE_MSG(VBC_ALG_PHY_V3_495_112_2_17_23_3_15_356,"vbc_alg_phy_v3.c,[VB_PHY_SetDG] invalid da index :%d")
TRACE_MSG(VBC_PHY_V0_419_112_2_17_23_3_16_357,"[AUDDRV][VBC] dac pls set cfg chan.")
TRACE_MSG(VBC_PHY_V0_451_112_2_17_23_3_16_358,"[AUDDRV][VBC] adc pls set cfg chan.")
TRACE_MSG(VBC_PHY_V0_484_112_2_17_23_3_16_359,"[AUDDRV][VBC] dac dma chan en fail.")
TRACE_MSG(VBC_PHY_V0_512_112_2_17_23_3_16_360,"[AUDDRV][VBC] adc dma chan en fail.")
TRACE_MSG(VBC_PHY_V3_435_112_2_17_23_3_18_361,"[AUDDRV][VBC] dac pls set cfg chan.")
TRACE_MSG(VBC_PHY_V3_467_112_2_17_23_3_18_362,"[AUDDRV][VBC] adc pls set cfg chan.")
TRACE_MSG(VBC_PHY_V3_500_112_2_17_23_3_18_363,"[AUDDRV][VBC] dac dma chan en fail.")
TRACE_MSG(VBC_PHY_V3_528_112_2_17_23_3_18_364,"[AUDDRV][VBC] adc dma chan en fail.")
TRACE_MSG(VBC_PHY_V5_439_112_2_17_23_3_19_365,"[AUDDRV][VBC] dac pls set cfg chan.")
TRACE_MSG(VBC_PHY_V5_471_112_2_17_23_3_19_366,"[AUDDRV][VBC] adc pls set cfg chan.")
TRACE_MSG(VBC_PHY_V5_504_112_2_17_23_3_19_367,"[AUDDRV][VBC] dac dma chan en fail.")
TRACE_MSG(VBC_PHY_V5_532_112_2_17_23_3_19_368,"[AUDDRV][VBC] adc dma chan en fail.")
TRACE_MSG(BUSMONITOR_HAL_85_112_2_17_23_3_19_369,"[BM DRV:]bm_monitor_set")
TRACE_MSG(BUSMONITOR_HAL_149_112_2_17_23_3_19_370,"[BM DRV:]bm_test")
TRACE_MSG(BUSMONITOR_HAL_153_112_2_17_23_3_19_371,"[BM DRV:]write begin")
TRACE_MSG(BUSMONITOR_HAL_157_112_2_17_23_3_19_372,"[BM DRV:]write finish")
TRACE_MSG(BUSMONITOR_PHY_V0_82_112_2_17_23_3_20_373,"[BM DRV:]bm_open")
TRACE_MSG(BUSMONITOR_PHY_V0_108_112_2_17_23_3_20_374,"[BM DRV:]bm_clean_point")
TRACE_MSG(BUSMONITOR_PHY_V0_218_112_2_17_23_3_20_375,"[BM DRV:]bm_set_point")
TRACE_MSG(BUSMONITOR_PHY_V1_76_112_2_17_23_3_20_376,"[BM DRV:]bm_clk_eb")
TRACE_MSG(BUSMONITOR_PHY_V1_99_112_2_17_23_3_20_377,"[BM DRV:]bm_open")
TRACE_MSG(BUSMONITOR_PHY_V1_127_112_2_17_23_3_21_378,"[BM DRV:]bm_clean_point")
TRACE_MSG(BUSMONITOR_PHY_V1_252_112_2_17_23_3_21_379,"[BM DRV:]bm_set_point")
TRACE_MSG(BUSMONITOR_PHY_V3_79_112_2_17_23_3_21_380,"[BM DRV:]bm_clk_eb")
TRACE_MSG(BUSMONITOR_PHY_V3_100_112_2_17_23_3_21_381,"[BM DRV:]bm_open")
TRACE_MSG(BUSMONITOR_PHY_V3_127_112_2_17_23_3_21_382,"[BM DRV:]bm_clean_point")
TRACE_MSG(BUSMONITOR_PHY_V3_245_112_2_17_23_3_22_383,"[BM DRV:]bm_set_point")
TRACE_MSG(BUSMONITOR_PHY_V5_82_112_2_17_23_3_22_384,"[BM DRV:]bm_open")
TRACE_MSG(BUSMONITOR_PHY_V5_109_112_2_17_23_3_22_385,"[BM DRV:]bm_clean_point")
TRACE_MSG(BUSMONITOR_PHY_V5_219_112_2_17_23_3_22_386,"[BM DRV:]bm_set_point")
TRACE_MSG(CHARGE_236_112_2_17_23_3_25_387,"CHGMNG:module_state.chgmng_state:%d")
TRACE_MSG(CHARGE_237_112_2_17_23_3_25_388,"CHGMNG:module_state.bat_statistic_vol:%d")
TRACE_MSG(CHARGE_238_112_2_17_23_3_25_389,"CHGMNG:module_state.bat_cur_vol:%d")
TRACE_MSG(CHARGE_239_112_2_17_23_3_25_390,"CHGMNG:module_state.bat_remain_cap:%d")
TRACE_MSG(CHARGE_240_112_2_17_23_3_25_391,"CHGMNG:x0_adc:%d,y0_vol:%d,slope:%d,zoom_in:%d")
TRACE_MSG(CHARGE_244_112_2_17_23_3_25_392,"CHGMNG:module_state.charging_current:%d")
TRACE_MSG(CHARGE_245_112_2_17_23_3_25_393,"CHGMNG:module_state.adp_type:%d")
TRACE_MSG(CHARGE_246_112_2_17_23_3_25_394,"CHGMNG:module_state.charging_stop_reason:%d")
TRACE_MSG(CHARGE_247_112_2_17_23_3_25_395,"CHGMNG:hw_switch_point:%d")
TRACE_MSG(CHARGE_248_112_2_17_23_3_25_396,"CHGMNG:chg_end_vol:%d")
TRACE_MSG(CHARGE_252_112_2_17_23_3_25_397,"CHGMNG:otp_type:%d, over_low:%d, over_high:%d,resume_low:%d,resume_high:%d,charging_temperature:%d")
TRACE_MSG(CHARGE_257_112_2_17_23_3_25_398,"CHGMNG:module_state.bat_remain_cap:%d")
TRACE_MSG(CHARGE_258_112_2_17_23_3_25_399,"CHGMNG:dischg_param.shutdown_vol:%d")
TRACE_MSG(CHARGE_259_112_2_17_23_3_25_400,"CHGMNG:dischg_param.deadline_vol:%d")
TRACE_MSG(CHARGE_260_112_2_17_23_3_25_401,"CHGMNG:dischg_param.warning_vol:%d")
TRACE_MSG(CHARGE_262_112_2_17_23_3_25_402,"CHGMNG:....................vbat_queue.pointer:%d")
TRACE_MSG(CHARGE_321_112_2_17_23_3_25_403,"CHGMNG:_CHGMNG_FSMProcess fsm_event:%d")
TRACE_MSG(CHARGE_507_112_2_17_23_3_26_404,"CHGMNG:check_f_GSM_TX_on!")
TRACE_MSG(CHARGE_516_112_2_17_23_3_26_405,"CHGMNG:vbat_result[%d]:%d")
TRACE_MSG(CHARGE_533_112_2_17_23_3_26_406,"CHGMNG:_CHGMNG_GetVBATADCResult:%d")
TRACE_MSG(CHARGE_542_112_2_17_23_3_26_407,"CHGMNG:_CHGMNG_GetVbatVol:%d")
TRACE_MSG(CHARGE_699_112_2_17_23_3_26_408,"CHGMNG:%d has been send000!")
TRACE_MSG(CHARGE_713_112_2_17_23_3_26_409,"CHGMNG:%d has been send2222!")
TRACE_MSG(CHARGE_717_112_2_17_23_3_26_410,"CHGMNG:%d has been saved111!")
TRACE_MSG(CHARGE_831_112_2_17_23_3_26_411,"CHGMNG:CHR_WARNING_IND!!!!!!!!!!!!!!!!!!!!!!!!!!!")
TRACE_MSG(CHARGE_847_112_2_17_23_3_26_412,"CHGMNG:CHR_WARNING_IND Before shutdown!!!!!!!!!!!!!!!!!!!!!!!!!!!")
TRACE_MSG(CHARGE_853_112_2_17_23_3_26_413,"CHGMNG:CHR_SHUTDOWN_IND!!!!!!!!!!!!!!!!!!!!!!!!!!!")
TRACE_MSG(CHARGE_902_112_2_17_23_3_27_414,"CHGMNG_CHARGING: ovp")
TRACE_MSG(CHARGE_942_112_2_17_23_3_27_415,"CHGMNG_CHARGING: charger over power")
TRACE_MSG(CHARGE_988_112_2_17_23_3_27_416,"CHGMNG:chg_current:%d")
TRACE_MSG(CHARGE_993_112_2_17_23_3_27_417,"CHGMNG:STOP CHGMNG_VBATEND!")
TRACE_MSG(CHARGE_1004_112_2_17_23_3_27_418,"CHGMNG:STOP CHGMNG_TIMEOUT!")
TRACE_MSG(CHARGE_1039_112_2_17_23_3_27_419,"CHGMNG:UpdateSwitchoverPoint!")
TRACE_MSG(CHARGE_1098_112_2_17_23_3_27_420,"CHGMNG:STOPPING! recharge:%d")
TRACE_MSG(CHARGE_1117_112_2_17_23_3_27_421,"CHGMNG:OTP enable charger!")
TRACE_MSG(CHARGE_1134_112_2_17_23_3_27_422,"CHGMNG:CHGMNG_ChargeStartHandler!!!!!!!!!!!!!!!!!!!!!!!!!!!")
TRACE_MSG(CHARGE_1145_112_2_17_23_3_27_423,"CHGMNG:CHR_CHARGE_START_IND!!!!!!!!!!!!!!!!!!!!!!!!!!!")
TRACE_MSG(CHARGE_1157_112_2_17_23_3_27_424,"CHGMNG:Invalid g_ovp_timer timer pointer!")
TRACE_MSG(CHARGE_1167_112_2_17_23_3_27_425,"CHGMNG:Invalid application timer pointer!")
TRACE_MSG(CHARGE_1200_112_2_17_23_3_27_426,"CHGMNG:Invalid application timer pointer!")
TRACE_MSG(CHARGE_1209_112_2_17_23_3_27_427,"[Vcharge]CHGMNG:Invalid application timer pointer!")
TRACE_MSG(CHARGE_1213_112_2_17_23_3_27_428,"CHGMNG:CHR_CHARGE_DISCONNECT1!!!!!!!!!!!!!!!!!!!!!!!!!!!")
TRACE_MSG(CHARGE_1220_112_2_17_23_3_27_429,"CHGMNG:CHR_CHARGE_FINISH!!!!!!!!!!!!!!!!!!!!!!!!!!!")
TRACE_MSG(CHARGE_1510_112_2_17_23_3_28_430,"CHGMNG:CHGMNG_VoltageToAdcvalue votage:%d adc:%d")
TRACE_MSG(CHARGE_1619_112_2_17_23_3_28_431,"CHGMNG:CHGMNG_ModuleReset!!!!!!!!!!!!!!!!!!!!!!!!!!!")
TRACE_MSG(DUALBAT_HAL_103_112_2_17_23_3_29_432,"_DBAT_PrintTestLog start!!!")
TRACE_MSG(DUALBAT_HAL_104_112_2_17_23_3_29_433,"DBAT:module state: %d")
TRACE_MSG(DUALBAT_HAL_105_112_2_17_23_3_29_434,"DBAT:current bat: %d")
TRACE_MSG(DUALBAT_HAL_106_112_2_17_23_3_29_435,"DBAT:current vol: %d")
TRACE_MSG(DUALBAT_HAL_107_112_2_17_23_3_29_436,"DBAT:statistic_vol: %d")
TRACE_MSG(DUALBAT_HAL_108_112_2_17_23_3_29_437,"DBAT:bat_remain_cap: %d")
TRACE_MSG(DUALBAT_HAL_109_112_2_17_23_3_29_438,"DBAT:Idle bat: %d")
TRACE_MSG(DUALBAT_HAL_110_112_2_17_23_3_29_439,"DBAT:main bat info vol: %d,cap: %d")
TRACE_MSG(DUALBAT_HAL_111_112_2_17_23_3_29_440,"DBAT:aux bat info vol: %d,cap: %d")
TRACE_MSG(DUALBAT_HAL_112_112_2_17_23_3_29_441,"_DBAT_PrintTestLog end!!!")
TRACE_MSG(DUALBAT_HAL_205_112_2_17_23_3_29_442,"DBAT:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!SCI_SLEEP")
TRACE_MSG(DUALBAT_HAL_255_112_2_17_23_3_29_443,"DBAT:!!!!!!!!on == record!!!!!!!!!!!record %d")
TRACE_MSG(DUALBAT_HAL_261_112_2_17_23_3_29_444,"DBAT:-------on != record---------record %d")
TRACE_MSG(DUALBAT_HAL_275_112_2_17_23_3_29_445,"DBAT:ChgFinish m_vol: %d,a_vol: %d")
TRACE_MSG(DUALBAT_HAL_279_112_2_17_23_3_29_446,"DBAT_CHARGE_FINISH0")
TRACE_MSG(DUALBAT_HAL_288_112_2_17_23_3_29_447,"DBAT_CHARGE_FINISH1")
TRACE_MSG(DUALBAT_HAL_306_112_2_17_23_3_29_448,"DBAT_CHARGE_FINISH2")
TRACE_MSG(DUALBAT_HAL_371_112_2_17_23_3_29_449,"DBAT:Shutdown3 m_vol: %d,a_vol: %d")
TRACE_MSG(DUALBAT_HAL_379_112_2_17_23_3_29_450,"DBAT:Shutdown1 m_vol: %d,a_vol: %d")
TRACE_MSG(DUALBAT_HAL_384_112_2_17_23_3_29_451,"DBAT:Shutdown2 m_vol: %d,a_vol: %d")
TRACE_MSG(DUALBAT_HAL_398_112_2_17_23_3_29_452,"DBAT:Warning1 m_vol: %d,a_vol: %d")
TRACE_MSG(DUALBAT_HAL_415_112_2_17_23_3_29_453,"DBAT:Warning2 m_vol: %d,a_vol: %d")
TRACE_MSG(DUALBAT_HAL_430_112_2_17_23_3_29_454,"_DBAT_ChgStartHandler!!!")
TRACE_MSG(DUALBAT_HAL_463_112_2_17_23_3_29_455,"_DBAT_ChgFaultHandler!!!")
TRACE_MSG(DUALBAT_HAL_492_112_2_17_23_3_29_456,"DBAT:_DBAT_AdpPlugOutHandler!!!!!!!!!!!!!!!!!!!")
TRACE_MSG(DUALBAT_HAL_521_112_2_17_23_3_29_457,"DBAT:Battery PLUGIN Again!!!!")
TRACE_MSG(DUALBAT_HAL_538_112_2_17_23_3_30_458,"DBAT:Battery plugin:%d m_vol: %d,a_vol: %d......................")
TRACE_MSG(DUALBAT_HAL_576_112_2_17_23_3_30_459,"DBAT:Battery PLUGOUT Again!!!!")
TRACE_MSG(DUALBAT_HAL_582_112_2_17_23_3_30_460,"DBAT:Battery PLUGOUT INT DETECT!!!!")
TRACE_MSG(DUALBAT_HAL_590_112_2_17_23_3_30_461,"DBAT:Battery PLUGOUT INT ERR!!!!")
TRACE_MSG(DUALBAT_HAL_599_112_2_17_23_3_30_462,"DBAT:Battery plugout:%d m_vol: %d,a_vol: %d.................................")
TRACE_MSG(DUALBAT_HAL_629_112_2_17_23_3_30_463,"_DBAT_MutualChgRoutineInCharging: >>>>>>>>")
TRACE_MSG(DUALBAT_HAL_640_112_2_17_23_3_30_464,"DBAT:_DBAT_MutualChgRoutineInCharging!!!!!!!_DBAT_SwitchBatHandler!!!!!!!!!!!bat:%d")
TRACE_MSG(DUALBAT_HAL_736_112_2_17_23_3_30_465,"DBAT:Sw1 aim_bat:%d")
TRACE_MSG(DUALBAT_HAL_746_112_2_17_23_3_30_466,"DBAT:Sw2 aim_bat:%d m_vol: %d,a_vol: %d")
TRACE_MSG(DUALBAT_HAL_753_112_2_17_23_3_30_467,"DBAT:CHGMNG_SendMsgToChgTask CHGMNG_MODULE_RESET_MSG!")
TRACE_MSG(DUALBAT_HAL_836_112_2_17_23_3_30_468,"DBAT: ADCDetect DBAT_A_BAT_PLUGIN_MSG")
TRACE_MSG(DUALBAT_HAL_846_112_2_17_23_3_30_469,"DBAT: ADCDetect DBAT_M_BAT_PLUGIN_MSG")
TRACE_MSG(DUALBAT_HAL_856_112_2_17_23_3_30_470,"DBAT: ADCDetect DBAT_A_BAT_PLUGOUT_MSG")
TRACE_MSG(DUALBAT_HAL_862_112_2_17_23_3_30_471,"DBAT: not present A bat")
TRACE_MSG(DUALBAT_HAL_873_112_2_17_23_3_30_472,"DBAT: ADCDetect DBAT_M_BAT_PLUGOUT_MSG")
TRACE_MSG(DUALBAT_HAL_879_112_2_17_23_3_30_473,"DBAT: not present M bat")
TRACE_MSG(DUALBAT_HAL_922_112_2_17_23_3_30_474,"DBAT:_DBAT_CHGMNGCallBack err msg!!!")
TRACE_MSG(DUALBAT_HAL_963_112_2_17_23_3_30_475,"DBAT:_DBAT_GetVBatAdc:%d,bat:%d")
TRACE_MSG(DUALBAT_HAL_978_112_2_17_23_3_30_476,"DBAT:_DBAT_AdcCvtToVol slope:%d,zoom_in:%d,x0_adc:%d,y0_vol:%d,adc:%d!")
TRACE_MSG(DUALBAT_HAL_992_112_2_17_23_3_30_477,"DBAT:_DBAT_AdcCvtToVol use vbat!vbat_vol:%d")
TRACE_MSG(DUALBAT_HAL_1061_112_2_17_23_3_31_478,"DBAT:_DBAT_BatInfoInit,bat:%d vol:%d")
TRACE_MSG(DUALBAT_HAL_1079_112_2_17_23_3_31_479,"DBAT:state init")
TRACE_MSG(DUALBAT_HAL_1091_112_2_17_23_3_31_480,"DBAT:1init SelectBat(DBAT_MAIN_BAT)")
TRACE_MSG(DUALBAT_HAL_1103_112_2_17_23_3_31_481,"DBAT:2init SelectBat(DBAT_AUX_BAT)")
TRACE_MSG(DUALBAT_HAL_1118_112_2_17_23_3_31_482,"DBAT:err: _DBAT_StateInit DBAT_MAIN_BAT")
TRACE_MSG(DUALBAT_HAL_1127_112_2_17_23_3_31_483,"DBAT:err: _DBAT_StateInit DBAT_AUX_BAT")
TRACE_MSG(DUALBAT_HAL_1211_112_2_17_23_3_31_484,"DBAT3:%d has been send!")
TRACE_MSG(DUALBAT_HAL_1225_112_2_17_23_3_31_485,"DBAT1:%d has been saved!")
TRACE_MSG(DUALBAT_HAL_1263_112_2_17_23_3_31_486,"DBAT:DBAT_Init")
TRACE_MSG(CHG_PHY_V0_117_112_2_17_23_3_31_487,"CHGMNG:CHG_SetUSBChargeCurrent=%d")
TRACE_MSG(CHG_PHY_V0_292_112_2_17_23_3_32_488,"CHG_UpdateSwitchoverPoint: chg_switchpoint = %d\n")
TRACE_MSG(CHG_PHY_V0_309_112_2_17_23_3_32_489,"CHGMNG:CHG_PHY_SetChgCurrent=%d")
TRACE_MSG(CHG_PHY_V3_116_112_2_17_23_3_33_490,"CHGMNG:CHG_SetUSBChargeCurrent=%d")
TRACE_MSG(CHG_PHY_V3_215_112_2_17_23_3_33_491,"CHGMNG:CHG_StopRecharge")
TRACE_MSG(CHG_PHY_V3_291_112_2_17_23_3_33_492,"CHG_UpdateSwitchoverPoint: chg_switchpoint = %d\n")
TRACE_MSG(CHG_PHY_V3_312_112_2_17_23_3_33_493,"CHGMNG:CHG_PHY_SetChgCurrent=%d")
TRACE_MSG(CHG_PHY_V5_114_112_2_17_23_3_33_494,"CHGMNG:CHG_StopRecharge")
TRACE_MSG(CHG_PHY_V5_186_112_2_17_23_3_34_495,"CHG_UpdateSwitchoverPoint: chg_switchpoint = %d\n")
TRACE_MSG(CHG_PHY_V5_207_112_2_17_23_3_34_496,"CHGMNG:CHG_PHY_SetChgCurrent=%d")
TRACE_MSG(CHIP_PHY_V5_414_112_2_17_23_3_42_497,"%x, %x")
TRACE_MSG(CLOCK_243_112_2_17_23_3_44_498,"[_CLOCK_SourceSet] 0x%x 0x%x")
TRACE_MSG(CLOCK_269_112_2_17_23_3_44_499,"[DEVICE_ClockEnable] %s")
TRACE_MSG(CLOCK_289_112_2_17_23_3_44_500,"[DEVICE_DisableClock] %s")
TRACE_MSG(CLOCK_309_112_2_17_23_3_44_501,"[DEVICE_Reset] %s")
TRACE_MSG(CLOCK_329_112_2_17_23_3_44_502,"[_DEVICE_Open] %s")
TRACE_MSG(CLOCK_333_112_2_17_23_3_44_503,"[DEVICE_Open] The device has been opened")
TRACE_MSG(CLOCK_376_112_2_17_23_3_44_504,"[DEVICE_Close] %s")
TRACE_MSG(CLOCK_386_112_2_17_23_3_44_505,"[DEVICE_Close] Device has already been closed")
TRACE_MSG(CLOCK_400_112_2_17_23_3_44_506,"[DEVICE_Close] Device %s was opened yet")
TRACE_MSG(CLOCK_456_112_2_17_23_3_44_507,"[DEVICE_SetClock] Fail")
TRACE_MSG(CLOCK_504_112_2_17_23_3_44_508,"[DEVICE_SetClock] Fail")
TRACE_MSG(CLOCK_570_112_2_17_23_3_44_509,"[DEVICE_GetClock] %s")
TRACE_MSG(CLOCK_695_112_2_17_23_3_45_510,"[_OpenClock] IN (ClkObj = 0x%x)\r\n")
TRACE_MSG(CLOCK_699_112_2_17_23_3_45_511,"[_OpenClock] Clock has already been opened")
TRACE_MSG(CLOCK_734_112_2_17_23_3_45_512,"[_CloseClock] IN (%s)\r\n")
TRACE_MSG(CLOCK_740_112_2_17_23_3_45_513,"[_CloseClock] Clock has already been closed")
TRACE_MSG(CLOCK_806_112_2_17_23_3_45_514,"[_DeviceObjectCreate] IN (pDev = 0x%x)\r\n")
TRACE_MSG(CLOCK_815_112_2_17_23_3_45_515,"[_DeviceObjectCreate] <%s> has been added\r\n")
TRACE_MSG(CLOCK_823_112_2_17_23_3_45_516,"[_DeviceObjectCreate] Fail to allocate memoryrn")
TRACE_MSG(CLOCK_833_112_2_17_23_3_45_517,"[_DeviceObjectCreate] Fail to add device Name to ")
TRACE_MSG(SC6530_CLOCK_CFG_96_112_2_17_23_3_46_518,"[_SensorClkEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_106_112_2_17_23_3_46_519,"[_SensorClkDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_447_112_2_17_23_3_47_520,"[_RtcXtlEnable] default is open")
TRACE_MSG(SC6530_CLOCK_CFG_455_112_2_17_23_3_47_521,"[_RtcXtlDisable] can't be closed.")
TRACE_MSG(SC6530_CLOCK_CFG_463_112_2_17_23_3_47_522,"[_MpllEnable] default is open")
TRACE_MSG(SC6530_CLOCK_CFG_471_112_2_17_23_3_47_523,"[_MpllDisable] can't be closed.")
TRACE_MSG(SC6530_CLOCK_CFG_479_112_2_17_23_3_47_524,"[_26MXtlEnable] default is open")
TRACE_MSG(SC6530_CLOCK_CFG_487_112_2_17_23_3_47_525,"[_26MXtlEnable] can't be closed.")
TRACE_MSG(SC6530_CLOCK_CFG_495_112_2_17_23_3_47_526,"[_TimerEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_506_112_2_17_23_3_47_527,"[_TimerDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_515_112_2_17_23_3_47_528,"[_TimerReset]")
TRACE_MSG(SC6530_CLOCK_CFG_529_112_2_17_23_3_47_529,"[_TimerbkEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_540_112_2_17_23_3_47_530,"[_TimerbkDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_549_112_2_17_23_3_47_531,"[_TimerbkReset]")
TRACE_MSG(SC6530_CLOCK_CFG_563_112_2_17_23_3_47_532,"[_KpdEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_574_112_2_17_23_3_47_533,"[_KpdDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_584_112_2_17_23_3_47_534,"[_KpdReset]")
TRACE_MSG(SC6530_CLOCK_CFG_599_112_2_17_23_3_47_535,"[_IIC0Enable]")
TRACE_MSG(SC6530_CLOCK_CFG_609_112_2_17_23_3_47_536,"[_IIC0Enable]")
TRACE_MSG(SC6530_CLOCK_CFG_618_112_2_17_23_3_47_537,"[_IIC0Reset]")
TRACE_MSG(SC6530_CLOCK_CFG_633_112_2_17_23_3_47_538,"[_IIC1Enable]")
TRACE_MSG(SC6530_CLOCK_CFG_643_112_2_17_23_3_47_539,"[_IIC1Enable]")
TRACE_MSG(SC6530_CLOCK_CFG_652_112_2_17_23_3_47_540,"[_IIC1Reset]")
TRACE_MSG(SC6530_CLOCK_CFG_667_112_2_17_23_3_47_541,"[_SyscntEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_678_112_2_17_23_3_47_542,"[_SyscntDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_688_112_2_17_23_3_47_543,"[_SyscntReset]")
TRACE_MSG(SC6530_CLOCK_CFG_703_112_2_17_23_3_47_544,"[_AdiEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_713_112_2_17_23_3_47_545,"[_AdiDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_722_112_2_17_23_3_47_546,"[_AdiReset]")
TRACE_MSG(SC6530_CLOCK_CFG_736_112_2_17_23_3_47_547,"[_RtcEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_747_112_2_17_23_3_47_548,"[_RtcDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_757_112_2_17_23_3_47_549,"[_RtcReset]")
TRACE_MSG(SC6530_CLOCK_CFG_771_112_2_17_23_3_47_550,"[_WdgEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_782_112_2_17_23_3_47_551,"[_WdgDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_792_112_2_17_23_3_47_552,"[_WdgReset]")
TRACE_MSG(SC6530_CLOCK_CFG_806_112_2_17_23_3_47_553,"[_ChgrwdgEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_817_112_2_17_23_3_47_554,"[_ChgrwdgDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_827_112_2_17_23_3_47_555,"[_ChgrwdgReset]")
TRACE_MSG(SC6530_CLOCK_CFG_842_112_2_17_23_3_47_556,"[_TpcEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_853_112_2_17_23_3_47_557,"[_TpcDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_863_112_2_17_23_3_47_558,"[_TpcReset]")
TRACE_MSG(SC6530_CLOCK_CFG_877_112_2_17_23_3_47_559,"[_AdcEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_890_112_2_17_23_3_47_560,"[_AdcDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_903_112_2_17_23_3_47_561,"[_AdcReset]")
TRACE_MSG(SC6530_CLOCK_CFG_917_112_2_17_23_3_47_562,"[_GpiodEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_929_112_2_17_23_3_47_563,"[_GpiodDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_940_112_2_17_23_3_47_564,"[_GpioReset]")
TRACE_MSG(SC6530_CLOCK_CFG_955_112_2_17_23_3_47_565,"[_GpioaEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_966_112_2_17_23_3_47_566,"[_GpioDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_976_112_2_17_23_3_48_567,"[_GpioaReset]")
TRACE_MSG(SC6530_CLOCK_CFG_991_112_2_17_23_3_48_568,"[_EicdEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1003_112_2_17_23_3_48_569,"[_EicdDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1014_112_2_17_23_3_48_570,"[_EicdReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1029_112_2_17_23_3_48_571,"[_EicaEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1041_112_2_17_23_3_48_572,"[_EicaDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1052_112_2_17_23_3_48_573,"[_EicaReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1067_112_2_17_23_3_48_574,"[_Sim0Enable]")
TRACE_MSG(SC6530_CLOCK_CFG_1077_112_2_17_23_3_48_575,"[_Sim0Disable]")
TRACE_MSG(SC6530_CLOCK_CFG_1086_112_2_17_23_3_48_576,"[_Sim0Reset]")
TRACE_MSG(SC6530_CLOCK_CFG_1101_112_2_17_23_3_48_577,"[_GeaEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1111_112_2_17_23_3_48_578,"[_GeaDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1120_112_2_17_23_3_48_579,"[_GeaReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1135_112_2_17_23_3_48_580,"[_EfuseEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1145_112_2_17_23_3_48_581,"[_EfuseDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1155_112_2_17_23_3_48_582,"[_Spi0Enable]")
TRACE_MSG(SC6530_CLOCK_CFG_1166_112_2_17_23_3_48_583,"[_SpiDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1175_112_2_17_23_3_48_584,"[_Spi1Enable]")
TRACE_MSG(SC6530_CLOCK_CFG_1185_112_2_17_23_3_48_585,"[_Spi1Disable]")
TRACE_MSG(SC6530_CLOCK_CFG_1194_112_2_17_23_3_48_586,"[_Spi1Reset]")
TRACE_MSG(SC6530_CLOCK_CFG_1209_112_2_17_23_3_48_587,"[_I2s0Enable]")
TRACE_MSG(SC6530_CLOCK_CFG_1219_112_2_17_23_3_48_588,"[_I2s0Disable]")
TRACE_MSG(SC6530_CLOCK_CFG_1228_112_2_17_23_3_48_589,"[_I2sReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1243_112_2_17_23_3_48_590,"[_Uart0Enable]")
TRACE_MSG(SC6530_CLOCK_CFG_1253_112_2_17_23_3_48_591,"[_Uart0Disable]")
TRACE_MSG(SC6530_CLOCK_CFG_1262_112_2_17_23_3_48_592,"[_Uart0Reset]")
TRACE_MSG(SC6530_CLOCK_CFG_1277_112_2_17_23_3_48_593,"[_Uart1Enable]")
TRACE_MSG(SC6530_CLOCK_CFG_1287_112_2_17_23_3_48_594,"[_Uart1Disable]")
TRACE_MSG(SC6530_CLOCK_CFG_1296_112_2_17_23_3_48_595,"[_Uart1Reset]")
TRACE_MSG(SC6530_CLOCK_CFG_1311_112_2_17_23_3_48_596,"[_PwmEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1321_112_2_17_23_3_48_597,"[_PwmDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1331_112_2_17_23_3_48_598,"[_PinEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1342_112_2_17_23_3_48_599,"[_PinDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1352_112_2_17_23_3_48_600,"[_PwmReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1367_112_2_17_23_3_48_601,"[_Aux0Enable]")
TRACE_MSG(SC6530_CLOCK_CFG_1377_112_2_17_23_3_48_602,"[_Aux0Disable]")
TRACE_MSG(SC6530_CLOCK_CFG_1386_112_2_17_23_3_48_603,"[_CcirReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1401_112_2_17_23_3_48_604,"[_DcamEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1411_112_2_17_23_3_48_605,"[_DcamDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1420_112_2_17_23_3_48_606,"[_DcamReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1435_112_2_17_23_3_48_607,"[_LzmaEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1445_112_2_17_23_3_48_608,"[_LzmaDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1454_112_2_17_23_3_48_609,"[_LzmaReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1468_112_2_17_23_3_48_610,"[_LcmEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1478_112_2_17_23_3_48_611,"[_LcmDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1487_112_2_17_23_3_48_612,"[_LcmReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1501_112_2_17_23_3_49_613,"[_LcdcEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1511_112_2_17_23_3_49_614,"[_LcdcDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1520_112_2_17_23_3_49_615,"[_LcdcReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1534_112_2_17_23_3_49_616,"[_RotaEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1544_112_2_17_23_3_49_617,"[_RotaDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1553_112_2_17_23_3_49_618,"[_RotaReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1567_112_2_17_23_3_49_619,"[_MidiEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1577_112_2_17_23_3_49_620,"[_MidiDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1586_112_2_17_23_3_49_621,"[_MidiReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1600_112_2_17_23_3_49_622,"[_TicEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1610_112_2_17_23_3_49_623,"[_TicDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1620_112_2_17_23_3_49_624,"[_SfcEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1630_112_2_17_23_3_49_625,"[_SfcDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1639_112_2_17_23_3_49_626,"[_SfcReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1652_112_2_17_23_3_49_627,"[_VspReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1666_112_2_17_23_3_49_628,"[_SdioEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1676_112_2_17_23_3_49_629,"[_SdioDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1685_112_2_17_23_3_49_630,"[_SdioReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1699_112_2_17_23_3_49_631,"[_UsbEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1709_112_2_17_23_3_49_632,"[_UsbDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1718_112_2_17_23_3_49_633,"[_UsbReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1732_112_2_17_23_3_49_634,"[_DmaEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1742_112_2_17_23_3_49_635,"[_DmaDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1751_112_2_17_23_3_49_636,"[_DmaReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1765_112_2_17_23_3_49_637,"[_Bm0Enable]")
TRACE_MSG(SC6530_CLOCK_CFG_1775_112_2_17_23_3_49_638,"[_Bm0Disable]")
TRACE_MSG(SC6530_CLOCK_CFG_1785_112_2_17_23_3_49_639,"[_Bm1Enable]")
TRACE_MSG(SC6530_CLOCK_CFG_1795_112_2_17_23_3_49_640,"[_Bm1Disable]")
TRACE_MSG(SC6530_CLOCK_CFG_1805_112_2_17_23_3_49_641,"[_Bm2Enable]")
TRACE_MSG(SC6530_CLOCK_CFG_1815_112_2_17_23_3_49_642,"[_Bm2Disable]")
TRACE_MSG(SC6530_CLOCK_CFG_1824_112_2_17_23_3_49_643,"[_AhbEnable] is NULL")
TRACE_MSG(SC6530_CLOCK_CFG_1833_112_2_17_23_3_49_644,"[_AhbDisable] is NULL")
TRACE_MSG(SC6530_CLOCK_CFG_1842_112_2_17_23_3_49_645,"[_ApbEnable] is NULL")
TRACE_MSG(SC6530_CLOCK_CFG_1851_112_2_17_23_3_49_646,"[_ApbDisable] is NULL")
TRACE_MSG(SC6530_CLOCK_CFG_1860_112_2_17_23_3_49_647,"[_EmcEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1870_112_2_17_23_3_49_648,"[_AhbDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1880_112_2_17_23_3_49_649,"[_EmcReset]")
TRACE_MSG(SC6530_CLOCK_CFG_1896_112_2_17_23_3_49_650,"[_AudioEnable] IS NULL")
TRACE_MSG(SC6530_CLOCK_CFG_1906_112_2_17_23_3_49_651,"[_AudioDisable] IS NULL")
TRACE_MSG(SC6530_CLOCK_CFG_1914_112_2_17_23_3_49_652,"[_AudioReset] IS NULL")
TRACE_MSG(SC6530_CLOCK_CFG_1922_112_2_17_23_3_49_653,"[_DummyEnable]")
TRACE_MSG(SC6530_CLOCK_CFG_1931_112_2_17_23_3_49_654,"[_DummyDisable]")
TRACE_MSG(SC6530_CLOCK_CFG_1940_112_2_17_23_3_49_655,"[_DummyReset]")
TRACE_MSG(SC6600L_CLOCK_CFG_22_112_2_17_23_3_50_656,"[_WdgEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_34_112_2_17_23_3_50_657,"[_WdgDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_43_112_2_17_23_3_50_658,"[_TmrEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_55_112_2_17_23_3_50_659,"[_TmrDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_64_112_2_17_23_3_50_660,"[_RtcEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_76_112_2_17_23_3_50_661,"[_RtcDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_85_112_2_17_23_3_50_662,"[_KpdEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_97_112_2_17_23_3_50_663,"[_KpdDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_106_112_2_17_23_3_50_664,"[_SyscntEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_118_112_2_17_23_3_50_665,"[_SyscntDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_127_112_2_17_23_3_50_666,"[_TpcEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_137_112_2_17_23_3_50_667,"[_TpcDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_146_112_2_17_23_3_50_668,"[_GpioEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_158_112_2_17_23_3_50_669,"[_GpioDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_167_112_2_17_23_3_50_670,"[_IisEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_178_112_2_17_23_3_50_671,"[_IisDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_187_112_2_17_23_3_50_672,"[_AdcEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_198_112_2_17_23_3_50_673,"[_AdcDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_207_112_2_17_23_3_50_674,"[_SimEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_218_112_2_17_23_3_50_675,"[_SimDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_227_112_2_17_23_3_50_676,"[_GeaEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_238_112_2_17_23_3_50_677,"[_GeaDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_247_112_2_17_23_3_50_678,"[_I2cEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_258_112_2_17_23_3_50_679,"[_I2cDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_267_112_2_17_23_3_51_680,"[_PwmEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_278_112_2_17_23_3_51_681,"[_PwmDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_287_112_2_17_23_3_51_682,"[_IntEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_299_112_2_17_23_3_51_683,"[_IntDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_308_112_2_17_23_3_51_684,"[_Uart0Enable]")
TRACE_MSG(SC6600L_CLOCK_CFG_319_112_2_17_23_3_51_685,"[_Uart0Disable]")
TRACE_MSG(SC6600L_CLOCK_CFG_328_112_2_17_23_3_51_686,"[_Uart1Enable]")
TRACE_MSG(SC6600L_CLOCK_CFG_339_112_2_17_23_3_51_687,"[_Uart1Disable]")
TRACE_MSG(SC6600L_CLOCK_CFG_348_112_2_17_23_3_51_688,"[_VbcEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_357_112_2_17_23_3_51_689,"[_VbcDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_366_112_2_17_23_3_51_690,"[_Spi0Enable]")
TRACE_MSG(SC6600L_CLOCK_CFG_377_112_2_17_23_3_51_691,"[_Spi0Disable]")
TRACE_MSG(SC6600L_CLOCK_CFG_386_112_2_17_23_3_51_692,"[_Spi1Enable]")
TRACE_MSG(SC6600L_CLOCK_CFG_397_112_2_17_23_3_51_693,"[_Spi1Disable]")
TRACE_MSG(SC6600L_CLOCK_CFG_408_112_2_17_23_3_51_694,"[_DcamEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_420_112_2_17_23_3_51_695,"[_DcamEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_432_112_2_17_23_3_51_696,"[_UsbEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_444_112_2_17_23_3_51_697,"[_UsbEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_456_112_2_17_23_3_51_698,"[_UsbEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_468_112_2_17_23_3_51_699,"[_UsbEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_480_112_2_17_23_3_51_700,"[_UsbEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_492_112_2_17_23_3_51_701,"[_UsbEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_504_112_2_17_23_3_51_702,"[_UsbEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_516_112_2_17_23_3_51_703,"[_UsbEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_526_112_2_17_23_3_51_704,"[_AuxEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_534_112_2_17_23_3_51_705,"[_AuxDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_544_112_2_17_23_3_51_706,"[_AuxadcEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_555_112_2_17_23_3_51_707,"[_AuxadcDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_564_112_2_17_23_3_51_708,"[_VbEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_573_112_2_17_23_3_51_709,"[_VbDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_594_112_2_17_23_3_51_710,"[_VbEnable]")
TRACE_MSG(SC6600L_CLOCK_CFG_601_112_2_17_23_3_51_711,"[_ApbDisable]")
TRACE_MSG(SC6600L_CLOCK_CFG_613_112_2_17_23_3_51_712,"[_McuSrcSet]:--no divide.")
TRACE_MSG(SC6600L_CLOCK_CFG_656_112_2_17_23_3_51_713,"[_DspSrcSet]:---no divide.")
TRACE_MSG(SC6600L_CLOCK_CFG_698_112_2_17_23_3_51_714,"[_SrcSet]:--fixed with 26m, no divide")
TRACE_MSG(SC6600L_CLOCK_CFG_719_112_2_17_23_3_51_715,"[_DcamSrcSet]:---no divide.")
TRACE_MSG(SC6600L_CLOCK_CFG_761_112_2_17_23_3_51_716,"[_Spi0SrcSet]:")
TRACE_MSG(SC6600L_CLOCK_CFG_817_112_2_17_23_3_52_717,"[_Spi1SrcSet]:")
TRACE_MSG(SC6600L_CLOCK_CFG_876_112_2_17_23_3_52_718,"[_IisSrcSet]")
TRACE_MSG(SC6600L_CLOCK_CFG_952_112_2_17_23_3_52_719,"[_AuxSrcSet]:")
TRACE_MSG(SC6600L_CLOCK_CFG_1019_112_2_17_23_3_52_720,"[_VbmclkSrcSet]:--fixed with 12M, no divide")
TRACE_MSG(SC6600L_CLOCK_CFG_1042_112_2_17_23_3_52_721,"[_CcirmclkSrcSet]:")
TRACE_MSG(SC6600L_CLOCK_CFG_1114_112_2_17_23_3_52_722,"[_SdioSrcSet]:")
TRACE_MSG(SC6800H_CLOCK_CFG_678_112_2_17_23_3_55_723,"[_RtcxtlEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_688_112_2_17_23_3_55_724,"[_RtcxtlDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_698_112_2_17_23_3_55_725,"[_MpllEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_712_112_2_17_23_3_55_726,"[_MpllDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_728_112_2_17_23_3_55_727,"[_BpllEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_753_112_2_17_23_3_55_728,"[_BpllDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_774_112_2_17_23_3_55_729,"[_26MXtlEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_783_112_2_17_23_3_55_730,"[_26MXtlEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_792_112_2_17_23_3_55_731,"[_WdgEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_803_112_2_17_23_3_55_732,"[_WdgDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_813_112_2_17_23_3_55_733,"[_TimerEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_824_112_2_17_23_3_55_734,"[_TimerDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_834_112_2_17_23_3_55_735,"[_RtcEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_845_112_2_17_23_3_55_736,"[_RtcDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_855_112_2_17_23_3_55_737,"[_KpdEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_866_112_2_17_23_3_55_738,"[_IIC0Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_876_112_2_17_23_3_55_739,"[_IIC0Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_890_112_2_17_23_3_55_740,"[_IIC0Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_903_112_2_17_23_3_55_741,"[_IIC0Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_914_112_2_17_23_3_55_742,"[_KpdDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_925_112_2_17_23_3_55_743,"[_SyscntEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_936_112_2_17_23_3_55_744,"[_SyscntDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_946_112_2_17_23_3_55_745,"[_TpcEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_957_112_2_17_23_3_55_746,"[_TpcDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_967_112_2_17_23_3_55_747,"[_TpcEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_977_112_2_17_23_3_55_748,"[_TpcDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_987_112_2_17_23_3_55_749,"[_AdcReset]")
TRACE_MSG(SC6800H_CLOCK_CFG_1005_112_2_17_23_3_55_750,"[_GpioEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1016_112_2_17_23_3_55_751,"[_GpioDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1026_112_2_17_23_3_55_752,"[_Sim0Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1036_112_2_17_23_3_55_753,"[_Sim0Disable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1046_112_2_17_23_3_55_754,"[_Sim1Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1056_112_2_17_23_3_56_755,"[_Sim1Disable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1066_112_2_17_23_3_56_756,"[_GeaEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1076_112_2_17_23_3_56_757,"[_GeaDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1086_112_2_17_23_3_56_758,"[_EfuseEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1096_112_2_17_23_3_56_759,"[_EfuseDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1106_112_2_17_23_3_56_760,"[_SpiEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1116_112_2_17_23_3_56_761,"[_SpiDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1126_112_2_17_23_3_56_762,"[_I2s0Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1137_112_2_17_23_3_56_763,"[_I2s0Disable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1147_112_2_17_23_3_56_764,"[_Uart0Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1157_112_2_17_23_3_56_765,"[_Uart0Disable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1167_112_2_17_23_3_56_766,"[_Pwm0Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1177_112_2_17_23_3_56_767,"[_Pwm0Disable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1187_112_2_17_23_3_56_768,"[_Pwm1Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1197_112_2_17_23_3_56_769,"[_Pwm1Disable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1207_112_2_17_23_3_56_770,"[_Pwm2Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1217_112_2_17_23_3_56_771,"[_Pwm2Disable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1227_112_2_17_23_3_56_772,"[_Pwm3Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1237_112_2_17_23_3_56_773,"[_Pwm3Disable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1247_112_2_17_23_3_56_774,"[_Pwm4Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1257_112_2_17_23_3_56_775,"[_Pwm4Disable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1267_112_2_17_23_3_56_776,"[_Aux0Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1277_112_2_17_23_3_56_777,"[_Aux0Disable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1287_112_2_17_23_3_56_778,"[_CcirEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1297_112_2_17_23_3_56_779,"[_CcirDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1308_112_2_17_23_3_56_780,"[_CcirReset]")
TRACE_MSG(SC6800H_CLOCK_CFG_1325_112_2_17_23_3_56_781,"[_DcamEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1335_112_2_17_23_3_56_782,"[_DcamDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1346_112_2_17_23_3_56_783,"[_DcamReset]")
TRACE_MSG(SC6800H_CLOCK_CFG_1363_112_2_17_23_3_56_784,"[_SdioEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1373_112_2_17_23_3_56_785,"[_SdioDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1384_112_2_17_23_3_56_786,"[_SdioReset]")
TRACE_MSG(SC6800H_CLOCK_CFG_1401_112_2_17_23_3_56_787,"[_UsbEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1411_112_2_17_23_3_56_788,"[_UsbDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1422_112_2_17_23_3_56_789,"[_UsbReset]")
TRACE_MSG(SC6800H_CLOCK_CFG_1439_112_2_17_23_3_56_790,"[_DmaEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1449_112_2_17_23_3_56_791,"[_DmaDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1460_112_2_17_23_3_56_792,"[_DmaReset]")
TRACE_MSG(SC6800H_CLOCK_CFG_1478_112_2_17_23_3_56_793,"[_Bm0Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1488_112_2_17_23_3_56_794,"[_Bm0Disable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1499_112_2_17_23_3_56_795,"[_Bm0Reset]")
TRACE_MSG(SC6800H_CLOCK_CFG_1516_112_2_17_23_3_56_796,"[_Bm1Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1526_112_2_17_23_3_56_797,"[_Bm1Disable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1537_112_2_17_23_3_56_798,"[_Bm1Reset]")
TRACE_MSG(SC6800H_CLOCK_CFG_1554_112_2_17_23_3_56_799,"[_Bm2Enable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1564_112_2_17_23_3_56_800,"[_Bm2Disable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1575_112_2_17_23_3_56_801,"[_Bm2Reset]")
TRACE_MSG(SC6800H_CLOCK_CFG_1591_112_2_17_23_3_57_802,"[_AhbEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1600_112_2_17_23_3_57_803,"[_AhbDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1609_112_2_17_23_3_57_804,"[_ApbEnable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1618_112_2_17_23_3_57_805,"[_ApbDisable]")
TRACE_MSG(SC6800H_CLOCK_CFG_1629_112_2_17_23_3_57_806,"[_EmcReset]")
TRACE_MSG(SC6800H_CLOCK_CFG_1649_112_2_17_23_3_57_807,"[_NfcReset]")
TRACE_MSG(SC8800G_CLOCK_CFG_97_112_2_17_23_3_59_808,"[_DcamEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_108_112_2_17_23_3_59_809,"[_DcamDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_153_112_2_17_23_3_59_810,"[_VspEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_163_112_2_17_23_3_59_811,"[_VspDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_174_112_2_17_23_4_0_812,"[_VspReset]")
TRACE_MSG(SC8800G_CLOCK_CFG_228_112_2_17_23_4_0_813,"[_LcdcEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_239_112_2_17_23_4_0_814,"[_LcdcDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_251_112_2_17_23_4_0_815,"[_LcdcReset]")
TRACE_MSG(SC8800G_CLOCK_CFG_302_112_2_17_23_4_0_816,"[_SdioEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_313_112_2_17_23_4_0_817,"[_SdioDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_323_112_2_17_23_4_0_818,"[_SdioReset]")
TRACE_MSG(SC8800G_CLOCK_CFG_383_112_2_17_23_4_0_819,"[_SpiEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_392_112_2_17_23_4_0_820,"[_SpiDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_438_112_2_17_23_4_0_821,"[_I2sEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_447_112_2_17_23_4_0_822,"[_I2sDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_457_112_2_17_23_4_0_823,"[_TpcEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_468_112_2_17_23_4_0_824,"[_TpcDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_477_112_2_17_23_4_0_825,"[_RtcEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_487_112_2_17_23_4_0_826,"[_RtcDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_497_112_2_17_23_4_0_827,"[_RtcTimer0Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_506_112_2_17_23_4_0_828,"[_RtcTimer0Disable]")
TRACE_MSG(SC8800G_CLOCK_CFG_517_112_2_17_23_4_0_829,"[_AdcEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_528_112_2_17_23_4_0_830,"[_AdcDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_538_112_2_17_23_4_0_831,"[_I2C0Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_550_112_2_17_23_4_0_832,"[_I2C0Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_560_112_2_17_23_4_0_833,"[_KpdDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_570_112_2_17_23_4_0_834,"[_KpdDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_580_112_2_17_23_4_0_835,"[_SyscntEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_590_112_2_17_23_4_0_836,"[_SyscntDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_637_112_2_17_23_4_0_837,"[_Uart0Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_647_112_2_17_23_4_0_838,"[_Uart0Disable]")
TRACE_MSG(SC8800G_CLOCK_CFG_694_112_2_17_23_4_1_839,"[_Uart1Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_704_112_2_17_23_4_1_840,"[_Uart1Disable]")
TRACE_MSG(SC8800G_CLOCK_CFG_751_112_2_17_23_4_1_841,"[_Uart2Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_760_112_2_17_23_4_1_842,"[_Uart2Disable]")
TRACE_MSG(SC8800G_CLOCK_CFG_790_112_2_17_23_4_1_843,"[_Pwm0Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_799_112_2_17_23_4_1_844,"[_Pwm0Disable]")
TRACE_MSG(SC8800G_CLOCK_CFG_828_112_2_17_23_4_1_845,"[_Pwm1Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_837_112_2_17_23_4_1_846,"[_Pwm1Disable]")
TRACE_MSG(SC8800G_CLOCK_CFG_866_112_2_17_23_4_1_847,"[_Pwm2Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_875_112_2_17_23_4_1_848,"[_Pwm2Disable]")
TRACE_MSG(SC8800G_CLOCK_CFG_904_112_2_17_23_4_1_849,"[_Pwm3Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_913_112_2_17_23_4_1_850,"[_Pwm3Disable]")
TRACE_MSG(SC8800G_CLOCK_CFG_959_112_2_17_23_4_1_851,"[_Aux0Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_968_112_2_17_23_4_1_852,"[_Aux0Disable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1015_112_2_17_23_4_1_853,"[_Aux1Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1024_112_2_17_23_4_1_854,"[_Aux1Disable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1036_112_2_17_23_4_1_855,"[_Sim0Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1046_112_2_17_23_4_1_856,"[_Sim0Disable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1056_112_2_17_23_4_1_857,"[_Sim1Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1066_112_2_17_23_4_1_858,"[_Sim1Disable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1076_112_2_17_23_4_1_859,"[_EptEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1086_112_2_17_23_4_1_860,"[_EptDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1095_112_2_17_23_4_1_861,"[_DmaEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1105_112_2_17_23_4_1_862,"[_DmaEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1117_112_2_17_23_4_1_863,"[_DmaReset]")
TRACE_MSG(SC8800G_CLOCK_CFG_1134_112_2_17_23_4_1_864,"[_NfcEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1144_112_2_17_23_4_1_865,"[_NfcDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1156_112_2_17_23_4_1_866,"[_DmaReset]")
TRACE_MSG(SC8800G_CLOCK_CFG_1172_112_2_17_23_4_1_867,"[_Bm0Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1182_112_2_17_23_4_1_868,"[_Bm0Disable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1192_112_2_17_23_4_1_869,"[_Bm1Enable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1202_112_2_17_23_4_1_870,"[_Bm1Disable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1212_112_2_17_23_4_1_871,"[_RotEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1221_112_2_17_23_4_1_872,"[_RotDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1233_112_2_17_23_4_2_873,"[_RotReset]")
TRACE_MSG(SC8800G_CLOCK_CFG_1268_112_2_17_23_4_2_874,"[_UsbEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1278_112_2_17_23_4_2_875,"[_UsbDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1290_112_2_17_23_4_2_876,"[_IspReset]")
TRACE_MSG(SC8800G_CLOCK_CFG_1314_112_2_17_23_4_2_877,"[_CcirEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1324_112_2_17_23_4_2_878,"[_CcirDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1383_112_2_17_23_4_2_879,"[_CcirReset]")
TRACE_MSG(SC8800G_CLOCK_CFG_1399_112_2_17_23_4_2_880,"[_AhbEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1408_112_2_17_23_4_2_881,"[_AhbDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1417_112_2_17_23_4_2_882,"[_ApbEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1426_112_2_17_23_4_2_883,"[_ApbDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1436_112_2_17_23_4_2_884,"[_WdgEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1446_112_2_17_23_4_2_885,"[_WdgDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1491_112_2_17_23_4_2_886,"[_PcmEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1505_112_2_17_23_4_2_887,"[_PcmDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1515_112_2_17_23_4_2_888,"[_GeaEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1525_112_2_17_23_4_2_889,"[_GeaDisable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1534_112_2_17_23_4_2_890,"[_AudioEnable]")
TRACE_MSG(SC8800G_CLOCK_CFG_1545_112_2_17_23_4_2_891,"[_AudioDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_24_112_2_17_23_4_3_892,"[_TpcEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_36_112_2_17_23_4_4_893,"[_TpcDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_45_112_2_17_23_4_4_894,"[_RtcEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_54_112_2_17_23_4_4_895,"[_RtcDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_63_112_2_17_23_4_4_896,"[_RtcTimer0Enable]")
TRACE_MSG(SC8800H_CLOCK_CFG_72_112_2_17_23_4_4_897,"[_RtcTimer0Disable]")
TRACE_MSG(SC8800H_CLOCK_CFG_83_112_2_17_23_4_4_898,"[_AdcEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_93_112_2_17_23_4_4_899,"[_AdcDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_103_112_2_17_23_4_4_900,"[_I2C0Enable]")
TRACE_MSG(SC8800H_CLOCK_CFG_115_112_2_17_23_4_4_901,"[_I2C0Enable]")
TRACE_MSG(SC8800H_CLOCK_CFG_125_112_2_17_23_4_4_902,"[_KpdDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_135_112_2_17_23_4_4_903,"[_KpdDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_145_112_2_17_23_4_4_904,"[_SyscntEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_155_112_2_17_23_4_4_905,"[_SyscntDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_165_112_2_17_23_4_4_906,"[_Uart0Enable]")
TRACE_MSG(SC8800H_CLOCK_CFG_175_112_2_17_23_4_4_907,"[_Uart0Disable]")
TRACE_MSG(SC8800H_CLOCK_CFG_185_112_2_17_23_4_4_908,"[_Sim0Enable]")
TRACE_MSG(SC8800H_CLOCK_CFG_195_112_2_17_23_4_4_909,"[_Sim0Disable]")
TRACE_MSG(SC8800H_CLOCK_CFG_205_112_2_17_23_4_4_910,"[_Sim1Enable]")
TRACE_MSG(SC8800H_CLOCK_CFG_215_112_2_17_23_4_4_911,"[_Sim1Disable]")
TRACE_MSG(SC8800H_CLOCK_CFG_225_112_2_17_23_4_4_912,"[_EptEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_235_112_2_17_23_4_4_913,"[_EptDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_246_112_2_17_23_4_4_914,"[_DmaEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_258_112_2_17_23_4_4_915,"[_DmaEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_270_112_2_17_23_4_4_916,"[_DmaReset]")
TRACE_MSG(SC8800H_CLOCK_CFG_288_112_2_17_23_4_4_917,"[_LcmEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_300_112_2_17_23_4_4_918,"[_LcmDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_311_112_2_17_23_4_4_919,"[_WdgEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_321_112_2_17_23_4_4_920,"[_WdgDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_332_112_2_17_23_4_4_921,"[_NfcEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_344_112_2_17_23_4_4_922,"[_NfcDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_356_112_2_17_23_4_4_923,"[_DmaReset]")
TRACE_MSG(SC8800H_CLOCK_CFG_374_112_2_17_23_4_4_924,"[_BmEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_386_112_2_17_23_4_4_925,"[_Bm0Disable]")
TRACE_MSG(SC8800H_CLOCK_CFG_399_112_2_17_23_4_4_926,"[_RotEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_411_112_2_17_23_4_4_927,"[_RotDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_423_112_2_17_23_4_4_928,"[_RotReset]")
TRACE_MSG(SC8800H_CLOCK_CFG_441_112_2_17_23_4_4_929,"[_LcdcEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_453_112_2_17_23_4_4_930,"[_LcdcDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_465_112_2_17_23_4_4_931,"[_LcdcReset]")
TRACE_MSG(SC8800H_CLOCK_CFG_483_112_2_17_23_4_4_932,"[_IspEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_495_112_2_17_23_4_4_933,"[_IspDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_507_112_2_17_23_4_4_934,"[_IspReset]")
TRACE_MSG(SC8800H_CLOCK_CFG_525_112_2_17_23_4_4_935,"[_UsbEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_537_112_2_17_23_4_4_936,"[_UsbDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_549_112_2_17_23_4_4_937,"[_IspReset]")
TRACE_MSG(SC8800H_CLOCK_CFG_575_112_2_17_23_4_5_938,"[_CcirEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_587_112_2_17_23_4_5_939,"[_CcirDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_599_112_2_17_23_4_5_940,"[_CcirReset]")
TRACE_MSG(SC8800H_CLOCK_CFG_615_112_2_17_23_4_5_941,"[_AhbEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_624_112_2_17_23_4_5_942,"[_AhbDisable]")
TRACE_MSG(SC8800H_CLOCK_CFG_633_112_2_17_23_4_5_943,"[_ApbEnable]")
TRACE_MSG(SC8800H_CLOCK_CFG_642_112_2_17_23_4_5_944,"[_ApbDisable]")
TRACE_MSG(DMA_HAL_V1_237_112_2_17_23_4_7_945,"DMA_init\r\n")
TRACE_MSG(DMA_HAL_V1_807_112_2_17_23_4_8_946,"DMA linkinit\r\n")
TRACE_MSG(DMA_PHY_V1_109_112_2_17_23_4_11_947,"DMA_HalInit\r\n")
TRACE_MSG(DMA_PHY_V1_157_112_2_17_23_4_11_948,"init enable flag 0x%08x\r\n")
TRACE_MSG(EIC_PHY_V5_95_112_2_17_23_4_20_949,"EICDRV: EIC_PHY_Open")
TRACE_MSG(EIC_PHY_V5_133_112_2_17_23_4_20_950,"EICDRV: EIC_PHY_Close")
TRACE_MSG(EIC_PHY_V5_149_112_2_17_23_4_20_951,"EICDRV: EIC_PHY_IntEn[%d] is %d")
TRACE_MSG(EIC_PHY_V5_182_112_2_17_23_4_20_952,"EICDRV: EIC_PHY_DbncEn[%d] is %d")
TRACE_MSG(EIC_PHY_V5_209_112_2_17_23_4_20_953,"EICDRV: EIC_PHY_MskData[%d] ")
TRACE_MSG(EIC_PHY_V5_237_112_2_17_23_4_20_954,"EICDRV: EIC_PHY_GetData[%d] == %d")
TRACE_MSG(EIC_PHY_V5_251_112_2_17_23_4_20_955,"EICDRV: EIC_PHY_SetTrg[%d] is %d")
TRACE_MSG(EIC_PHY_V5_411_112_2_17_23_4_21_956,"EICDRV: EIC_PHY_StartTrg[%d]")
TRACE_MSG(EIC_PHY_V5_432_112_2_17_23_4_21_957,"EICDRV: EIC_PHY_SetDbnc[%d] %d")
TRACE_MSG(EIC_PHY_V5_542_112_2_17_23_4_21_958,"EICDRV: EIC_PHY_ClrIntSts[%d] ")
TRACE_MSG(EIC_PHY_V5_589_112_2_17_23_4_21_959,"EICD_DbncIsr chan[%d]'s callback is DONE.")
TRACE_MSG(EIC_PHY_V5_630_112_2_17_23_4_21_960,"EICA_DbncIsr chan[%d]'s callback is DONE.")
TRACE_MSG(EIC_PHY_V5_758_112_2_17_23_4_21_961,"EICD_Dbnc_RegCallback")
TRACE_MSG(EIC_PHY_V5_766_112_2_17_23_4_21_962,"EICD_Dbnc_RegCallback-----callback is NULL.")
TRACE_MSG(EIC_PHY_V5_778_112_2_17_23_4_21_963,"EICA_Dbnc_RegCallback")
TRACE_MSG(EIC_PHY_V5_786_112_2_17_23_4_21_964,"EICA_Dbnc_RegCallback-----callback is NULL.")
TRACE_MSG(FREQ_HAL_854_112_2_17_23_4_31_965,"chng_freq_c.c CHNG_FREQ_SetArmClk %d.")
TRACE_MSG(GEA_PHY_V0_332_112_2_17_23_4_35_966,"GEA Driver memory alloc failed!")
TRACE_MSG(GEA_PHY_V0_388_112_2_17_23_4_35_967,"GEA Driver wait timeout:%ums!")
TRACE_MSG(GEA_PHY_V1_186_112_2_17_23_4_36_968," gea_stream:kc=0x%x0x%x0x%x0x%x,input=0x%x,ddir=%d,wh=%d,len=%d")
TRACE_MSG(EPT_PHY_V3_645_112_2_17_23_4_38_969,"err: GEA1/2 length bigger than %d!!")
TRACE_MSG(EPT_PHY_V3_655_112_2_17_23_4_38_970,"err: GEA3 length bigger than %d!!")
TRACE_MSG(EPT_PHY_V3_665_112_2_17_23_4_38_971,"err: Kasumi length is not %d!!")
TRACE_MSG(EPT_PHY_V3_675_112_2_17_23_4_38_972,"err: F8 length bigger than %d!!")
TRACE_MSG(EPT_PHY_V3_685_112_2_17_23_4_38_973,"err: F9 length bigger than %d!!")
TRACE_MSG(EPT_PHY_V3_896_112_2_17_23_4_38_974,"GEA Driver wait timeout:%ums!")
TRACE_MSG(EPT_PHY_V3_1013_112_2_17_23_4_38_975,"GEA Driver wait timeout:%ums!")
TRACE_MSG(GEA_PHY_V5_283_112_2_17_23_4_40_976,"GEADRV: memory alloc failed!")
TRACE_MSG(GEA_PHY_V5_381_112_2_17_23_4_41_977,"GEADRV:...Polling Done-->")
TRACE_MSG(GEA_PHY_V5_547_112_2_17_23_4_41_978,"GEA Driver wait timeout:%ums!")
TRACE_MSG(GEA_PHY_V5_580_112_2_17_23_4_41_979,"GEADRV:...Dma Int Done-->")
TRACE_MSG(GPIO_HAL_147_112_2_17_23_4_42_980,"GPIO_Init: Init GPIO !")
TRACE_MSG(GPIO_HAL_190_112_2_17_23_4_42_981,"GPIO_DRIVER: GPIO_%d interrupt has not been registed!\n")
TRACE_MSG(GPIO_HAL_304_112_2_17_23_4_42_982,"GPIO_GetGPIOState UnRegister interrupt for this pin.")
TRACE_MSG(GPIO_HAL_708_112_2_17_23_4_43_983,"GPIO_AddToIntTable: GPIO_%d has been added !\n")
TRACE_MSG(GPIO_HAL_752_112_2_17_23_4_43_984,"[GPIO_DRV]GPIO_AddToIntTable: We strongly suggest that the deshaking for GPI should be less than 256!,otherwise refer to GPIO\n")
TRACE_MSG(GPIO_HAL_829_112_2_17_23_4_43_985,"Can't delete deshaking timer of GPIO_%d!\n")
TRACE_MSG(GPIO_HAL_875_112_2_17_23_4_44_986,"GPIO_SetInitState ERROR gpio_id=%d,i=%d")
TRACE_MSG(GPIO_HAL_905_112_2_17_23_4_44_987,"GPIO_GetInitState ERROR gpio_id=%d,i=%d")
TRACE_MSG(GPIO_HAL_989_112_2_17_23_4_44_988,"GPIO_DRIVER: GPIO_%d is shaking(INVALID)!\n")
TRACE_MSG(GPIO_HAL_997_112_2_17_23_4_44_989,"GPIO_DRIVER: GPIO_%d state is %d!\n")
TRACE_MSG(GPIO_HAL_1082_112_2_17_23_4_44_990,"GPIO_GetShakeInterval ERROR gpio_id=%d,i=%d")
TRACE_MSG(GPIO_HAL_1119_112_2_17_23_4_44_991,"GPIO_GetCallBack ERROR gpio_id=%d,i=%d")
TRACE_MSG(GPIO_PHY_V0_194_112_2_17_23_4_45_992,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V0_263_112_2_17_23_4_45_993,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V0_267_112_2_17_23_4_45_994,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V0_323_112_2_17_23_4_45_995,"[GPIO_DRV]this opretion can not belong to GPO")
TRACE_MSG(GPIO_PHY_V0_331_112_2_17_23_4_45_996,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V0_431_112_2_17_23_4_45_997,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V0_458_112_2_17_23_4_45_998,"[GPIO_DRV]GPIO_SetDirection error")
TRACE_MSG(GPIO_PHY_V0_468_112_2_17_23_4_45_999,"[GPIO_DRV]GPIO_SetDirection error")
TRACE_MSG(GPIO_PHY_V0_478_112_2_17_23_4_45_1000,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V0_509_112_2_17_23_4_45_1001,"[GPIO_DRV]The corresponding reg of this GPIO_ID is a GPO! No Intr!")
TRACE_MSG(GPIO_PHY_V0_516_112_2_17_23_4_45_1002,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V0_546_112_2_17_23_4_45_1003,"[GPIO_DRV]this opretion can not belong to GPO")
TRACE_MSG(GPIO_PHY_V0_552_112_2_17_23_4_45_1004,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V0_589_112_2_17_23_4_46_1005,"This Intr sense type is invalid for GPI. \n")
TRACE_MSG(GPIO_PHY_V0_597_112_2_17_23_4_46_1006,"[GPIO_DRV]this opretion can not belong to GPO")
TRACE_MSG(GPIO_PHY_V0_636_112_2_17_23_4_46_1007,"This operation is invalid. \n")
TRACE_MSG(GPIO_PHY_V0_641_112_2_17_23_4_46_1008,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V0_677_112_2_17_23_4_46_1009,"[GPIO_DRV]this opretion can not belong to SetGPIDebounce")
TRACE_MSG(GPIO_PHY_V0_715_112_2_17_23_4_46_1010,"[GPIO_DRV]this opretion can not belong to GPO/GPIO")
TRACE_MSG(GPIO_PHY_V0_719_112_2_17_23_4_46_1011,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V5_193_112_2_17_23_4_46_1012,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V5_262_112_2_17_23_4_47_1013,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V5_266_112_2_17_23_4_47_1014,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V5_322_112_2_17_23_4_47_1015,"[GPIO_DRV]this opretion can not belong to GPO")
TRACE_MSG(GPIO_PHY_V5_330_112_2_17_23_4_47_1016,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V5_430_112_2_17_23_4_47_1017,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V5_457_112_2_17_23_4_47_1018,"[GPIO_DRV]GPIO_SetDirection error")
TRACE_MSG(GPIO_PHY_V5_467_112_2_17_23_4_47_1019,"[GPIO_DRV]GPIO_SetDirection error")
TRACE_MSG(GPIO_PHY_V5_477_112_2_17_23_4_47_1020,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V5_508_112_2_17_23_4_47_1021,"[GPIO_DRV]The corresponding reg of this GPIO_ID is a GPO! No Intr!")
TRACE_MSG(GPIO_PHY_V5_515_112_2_17_23_4_47_1022,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V5_545_112_2_17_23_4_47_1023,"[GPIO_DRV]this opretion can not belong to GPO")
TRACE_MSG(GPIO_PHY_V5_551_112_2_17_23_4_47_1024,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V5_588_112_2_17_23_4_47_1025,"This Intr sense type is invalid for GPI. \n")
TRACE_MSG(GPIO_PHY_V5_596_112_2_17_23_4_47_1026,"[GPIO_DRV]this opretion can not belong to GPO")
TRACE_MSG(GPIO_PHY_V5_635_112_2_17_23_4_47_1027,"This operation is invalid. \n")
TRACE_MSG(GPIO_PHY_V5_640_112_2_17_23_4_47_1028,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(GPIO_PHY_V5_676_112_2_17_23_4_47_1029,"[GPIO_DRV]this opretion can not belong to SetGPIDebounce")
TRACE_MSG(GPIO_PHY_V5_714_112_2_17_23_4_48_1030,"[GPIO_DRV]this opretion can not belong to GPO/GPIO")
TRACE_MSG(GPIO_PHY_V5_718_112_2_17_23_4_48_1031,"[GPIO_DRV]the GPIO_ID is Invalid in this chip")
TRACE_MSG(I2C_PHY_V0_293_112_2_17_23_4_51_1032,"[IIC DRV:]__I2C_PHY_SetPort: Fatal error, if you use qwerty keypad, i2c fun of multiplex pins are disabled")
TRACE_MSG(INT_HAL_583_112_2_17_23_4_57_1033,"IRQ HAS USED, intLogicNum = 0x%xn")
TRACE_MSG(INT_HAL_599_112_2_17_23_4_57_1034,"FIQ HAS USED, intLogicNum = 0x%xn")
TRACE_MSG(INT_HAL_HISR_149_112_2_17_23_4_57_1035,"CHIPDRVHISR_WaitActive:Get Semaphore Fail!")
TRACE_MSG(INT_HAL_HISR_172_112_2_17_23_4_57_1036,"CHIPDRVHISR_Activate:put Semaphore Fail!")
TRACE_MSG(KPD_PHY_V0_419_112_2_17_23_5_3_1037,"KPD_DRIVER: Key code %d is UP!\n")
TRACE_MSG(KPD_PHY_V5_420_112_2_17_23_5_4_1038,"KPD_DRIVER: Key code %d is UP!\n")
TRACE_MSG(LCM_PHY_V0_631_112_2_17_23_5_6_1039,"_LCM_WaitDMA_AllLinkDone: time out !!")
TRACE_MSG(LCM_PHY_V0_773_112_2_17_23_5_7_1040,"LCM_DmaWrite(): rot_angele=%d, brush_mode=%d")
TRACE_MSG(LCM_PHY_V0_774_112_2_17_23_5_7_1041,"LCM_DmaWrite(): src_addr=0x%x, src_w=%d,src_h=%d,buf_width=%d")
TRACE_MSG(LCM_PHY_V0_775_112_2_17_23_5_7_1042,"LCM_DmaWrite(): in_timer=%d")
TRACE_MSG(LCM_PHY_V0_968_112_2_17_23_5_7_1043,"LCM: w = %d, h = %d, rotate=%d")
TRACE_MSG(LCM_PHY_V1_228_112_2_17_23_5_8_1044,"_LCM_WaitDMADone: time out !!")
TRACE_MSG(LCD_IF_SPI_450_112_2_17_23_5_11_1045,"---FIFOFULL:rxt_cnt=0x%x")
TRACE_MSG(LCD_IF_SPI_455_112_2_17_23_5_11_1046,"---FIFOFULL: SPI_ReadData =0x%x")
TRACE_MSG(LCD_IF_SPI_469_112_2_17_23_5_11_1047,"---Finish: SPI_ReadData =0x%x")
TRACE_MSG(LCD_IF_SPI_627_112_2_17_23_5_11_1048,"SPI_HAL_Open(): SUCCESS!")
TRACE_MSG(LCD_IF_SPI_840_112_2_17_23_5_12_1049,"_LCM_WaitDMADone: time out !!")
TRACE_MSG(LCD_IF_SPI_917_112_2_17_23_5_12_1050,"LCM:w = %d,h = %d,bufw=%d rotate=%d")
TRACE_MSG(LCD_IF_SPI_1086_112_2_17_23_5_12_1051,"LCM:DMA_channel=%d")
TRACE_MSG(LCM_PHY_V5_98_112_2_17_23_5_13_1052,"_lcm_set_buswidth: the cs_pin value of lcd_spec_ptr should not large than 4..")
TRACE_MSG(LCM_PHY_V5_262_112_2_17_23_5_13_1053,"_LCM_WaitDMADone: time out !!")
TRACE_MSG(LCM_PHY_V5_375_112_2_17_23_5_13_1054,"LCM_SetParam: the cs_pin value of lcd_spec_ptr should not large than 4..")
TRACE_MSG(LCM_PHY_V5_471_112_2_17_23_5_13_1055,"LCM:w = %d,h = %d,bufw=%d rotate=%d")
TRACE_MSG(LCM_PHY_V5_640_112_2_17_23_5_14_1056,"LCM:DMA_channel=%d")
TRACE_MSG(LCM_PHY_V5_659_112_2_17_23_5_14_1057,"LCM_DmaWrite(): out_timer=%d")
TRACE_MSG(LDO_HAL_309_112_2_17_23_5_15_1058,"VCAMPad is not opened!")
TRACE_MSG(LZMA_PHY_V5_259_112_2_17_23_5_17_1059,"LZMA_WaitingFinish: time out !!")
TRACE_MSG(LZMA_TEST_101_112_2_17_23_5_17_1060,"_lzma_int_finish_callback is called.")
TRACE_MSG(LZMA_TEST_119_112_2_17_23_5_17_1061,"LZMA_CheckMem:-->src_addr[0x%x],dst_addr[0x%x]")
TRACE_MSG(LZMA_TEST_147_112_2_17_23_5_17_1062,"LZMA_IntstsTest:--------------->start")
TRACE_MSG(LZMA_TEST_168_112_2_17_23_5_17_1063,"LZMA_IntstsTest:-->ERROR,LZMA_INT_FINISH")
TRACE_MSG(LZMA_TEST_171_112_2_17_23_5_17_1064,"LZMA_IntstsTest:-->ERROR,LZMA_INT_BUF_LEN_ERR")
TRACE_MSG(LZMA_TEST_195_112_2_17_23_5_18_1065,"LZMA_IntstsTest:-->ERROR,LZMA_INT_FINISH")
TRACE_MSG(LZMA_TEST_198_112_2_17_23_5_18_1066,"LZMA_IntstsTest:-->ERROR,LZMA_INT_DST_LEN_ERR")
TRACE_MSG(LZMA_TEST_219_112_2_17_23_5_18_1067,"LZMA_IntstsTest:-->ERROR,LZMA_INT_FINISH")
TRACE_MSG(LZMA_TEST_222_112_2_17_23_5_18_1068,"LZMA_IntstsTest:-->ERROR,LZMA_INT_DEC_ERR")
TRACE_MSG(LZMA_TEST_226_112_2_17_23_5_18_1069,"LZMA_IntstsTest:--------------->end")
TRACE_MSG(LZMA_TEST_239_112_2_17_23_5_18_1070,"LZMA_PollingTest:-->buf_en=%d, buf_full_en=%d")
TRACE_MSG(LZMA_TEST_275_112_2_17_23_5_18_1071,"--------->Case[%d] succuss.")
TRACE_MSG(LZMA_TEST_290_112_2_17_23_5_18_1072,"LZMA_IntTest:-->buf_en=%d, buf_full_en=%d")
TRACE_MSG(LZMA_TEST_336_112_2_17_23_5_18_1073,"LZMA_Case0Test:-->pattern[%d] is error")
TRACE_MSG(LZMA_TEST_343_112_2_17_23_5_18_1074,"--------->Case[%d] succuss.")
TRACE_MSG(LZMA_TEST_372_112_2_17_23_5_18_1075,"test entry")
TRACE_MSG(RETARGET_141_112_2_17_23_5_20_1076,"DIV 0 error, Return address 0x%X")
TRACE_MSG(RETARGET_153_112_2_17_23_5_20_1077,"Stack Over flow, return address 0x%X, Thread ID:%d, Thread Name:%s")
TRACE_MSG(SIO_1443_112_2_17_23_5_24_1078,"SIO_FreeUartDmaChannel, channel is empty\n")
TRACE_MSG(SIO_3619_112_2_17_23_5_28_1079,"@@@@send tick used %d")
TRACE_MSG(SIO_4032_112_2_17_23_5_29_1080,"SIO writecmdres, mutex operate error\n")
TRACE_MSG(SIO_4174_112_2_17_23_5_29_1081,"Length 1 of Tx buffer is not long enought.the buffer size is %d , the space is %d \n")
TRACE_MSG(SIO_4186_112_2_17_23_5_29_1082,"Length 2 of Tx buffer is not long enought. the buffer size is %d , the space is %d \n")
TRACE_MSG(SIO_4448_112_2_17_23_5_30_1083,"SIO_ATC_WriteCmdRes -gprs_debug=0x%X\r\n")
TRACE_MSG(SIO_4455_112_2_17_23_5_30_1084,"SIO: discard AT response in DATA_MODE \n")
TRACE_MSG(SIO_4462_112_2_17_23_5_30_1085,"SIO_ATC_WriteCmdRes \n")
TRACE_MSG(SIO_4470_112_2_17_23_5_30_1086,"SIO_ATC_WriteCmdRes : uart_mix_sem_take \n")
TRACE_MSG(SIO_4785_112_2_17_23_5_30_1087,"jim: in SIO_SendDataPacket : uart_mix_check \n")
TRACE_MSG(SIO_4791_112_2_17_23_5_30_1088,"jim: in SIO_SendDataPacket : uart_mix_sem_take \n")
TRACE_MSG(SIO_5054_112_2_17_23_5_31_1089,"sio:datalen = %d")
TRACE_MSG(SIO_5060_112_2_17_23_5_31_1090,"sio:reset mix 1.")
TRACE_MSG(SIO_5071_112_2_17_23_5_31_1091,"sio:reset mix 2.")
TRACE_MSG(SIO_5079_112_2_17_23_5_31_1092,"sio:reset mix 3.")
TRACE_MSG(SIO_5097_112_2_17_23_5_31_1093,"mix mode data type error.")
TRACE_MSG(SIO_5157_112_2_17_23_5_31_1094,"MIX SIO: buffer full!")
TRACE_MSG(SIO_5262_112_2_17_23_5_31_1095,"SIO: discard the pending AT packets.")
TRACE_MSG(SIO_5323_112_2_17_23_5_31_1096,"SIO SIO_SetUserPortMode error, current mode is DATA_MODE\n")
TRACE_MSG(SIO_5910_112_2_17_23_5_32_1097,"Software flow control:send XOFF")
TRACE_MSG(SIO_6483_112_2_17_23_5_34_1098,"@@@@tick used %d")
TRACE_MSG(SIO_6488_112_2_17_23_5_34_1099,"@@@@tick used %d")
TRACE_MSG(SIO_6496_112_2_17_23_5_34_1100,"@@@@tick used %d")
TRACE_MSG(SIO_6507_112_2_17_23_5_34_1101,"@@@@tick used %d")
TRACE_MSG(SIO_6517_112_2_17_23_5_34_1102,"@@@@tick used %d")
TRACE_MSG(SIO_6521_112_2_17_23_5_34_1103,"@@@@max tick used %d")
TRACE_MSG(THREADX_ASSERT_3534_112_2_17_23_5_41_1104,"LOGSAVE: CHIPDRV SCI_PAssert")
TRACE_MSG(MMU_BZP_952_112_2_17_23_5_47_1105,"mmu_help_set_TEX error\n %d, %d, %d, %d")
TRACE_MSG(MMU_BZP_997_112_2_17_23_5_47_1106,"BUG: can not be mapped using pages, ignoring.addr = 0x%x, phy = 0x%x, length =0x%x\n")
TRACE_MSG(FLASH_1000_112_2_17_23_5_58_1107,"Flash:Get Semaphore Fail!")
TRACE_MSG(PCM_DRV_V0_590_112_2_17_23_6_0_1108,"PCM Parameter Error\r\n")
TRACE_MSG(PCM_DRV_V0_631_112_2_17_23_6_0_1109,"PCM_Open End\r\n")
TRACE_MSG(PCM_DRV_V0_662_112_2_17_23_6_0_1110,"==PCM Close===\r\n")
TRACE_MSG(PCM_DRV_V1_618_112_2_17_23_6_2_1111,"PCM Parameter Error\r\n")
TRACE_MSG(PCM_DRV_V1_645_112_2_17_23_6_2_1112,"PCM_Open End\r\n")
TRACE_MSG(PCM_DRV_V1_676_112_2_17_23_6_2_1113,"==PCM Close===\r\n")
TRACE_MSG(PCM_DRV_V5_608_112_2_17_23_6_3_1114,"PCM Parameter Error\r\n")
TRACE_MSG(PCM_DRV_V5_651_112_2_17_23_6_3_1115,"PCM_Open End\r\n")
TRACE_MSG(PCM_DRV_V5_684_112_2_17_23_6_3_1116,"==PCM Close===\r\n")
TRACE_MSG(SDHOST_PHY_552_112_2_17_23_6_15_1117,"   clkDiv: %d")
TRACE_MSG(SDHOST_PHY_559_112_2_17_23_6_15_1118,"   clkDiv: %d is too big!!!!!")
TRACE_MSG(SDHOST_PHY_615_112_2_17_23_6_15_1119,"sd clk: %d KHz.")
TRACE_MSG(SDHOST_PHY_1283_112_2_17_23_6_16_1120,"capbility_function = %x")
TRACE_MSG(SDHOST_PHY_1284_112_2_17_23_6_16_1121,"cability_Max_BlkLen = %x")
TRACE_MSG(SDHOST_PHY_1285_112_2_17_23_6_16_1122,"sd_Base_Max_Clk = %x")
TRACE_MSG(SDHOST_PHY_1286_112_2_17_23_6_16_1123,"timeOut_Clk_unit = %x")
TRACE_MSG(SDHOST_PHY_1287_112_2_17_23_6_16_1124,"timeOut_Base_Clk = %x")
TRACE_MSG(SDHOST_PHY_1288_112_2_17_23_6_16_1125,"max_current_for_1_8 = %x")
TRACE_MSG(SDHOST_PHY_1289_112_2_17_23_6_16_1126,"max_current_for_3_0 = %x")
TRACE_MSG(SDHOST_PHY_1290_112_2_17_23_6_16_1127,"max_current_for_3_3 = %x")
TRACE_MSG(SDHOST_PHY_V5_443_112_2_17_23_6_19_1128,"   clkDiv: %d")
TRACE_MSG(SDHOST_PHY_V5_450_112_2_17_23_6_19_1129,"   clkDiv: %d is too big!!!!!")
TRACE_MSG(SDHOST_PHY_V5_506_112_2_17_23_6_20_1130,"sd clk: %d KHz.")
TRACE_MSG(SDHOST_PHY_V5_1175_112_2_17_23_6_21_1131,"capbility_function = %x")
TRACE_MSG(SDHOST_PHY_V5_1176_112_2_17_23_6_21_1132,"cability_Max_BlkLen = %x")
TRACE_MSG(SDHOST_PHY_V5_1177_112_2_17_23_6_21_1133,"sd_Base_Max_Clk = %x")
TRACE_MSG(SDHOST_PHY_V5_1178_112_2_17_23_6_21_1134,"timeOut_Clk_unit = %x")
TRACE_MSG(SDHOST_PHY_V5_1179_112_2_17_23_6_21_1135,"timeOut_Base_Clk = %x")
TRACE_MSG(SDHOST_PHY_V5_1180_112_2_17_23_6_21_1136,"max_current_for_1_8 = %x")
TRACE_MSG(SDHOST_PHY_V5_1181_112_2_17_23_6_21_1137,"max_current_for_3_0 = %x")
TRACE_MSG(SDHOST_PHY_V5_1182_112_2_17_23_6_21_1138,"max_current_for_3_3 = %x")
TRACE_MSG(SIM_DRV_347_112_2_17_23_6_26_1139,"SIMDRV%d:init sim drv")
TRACE_MSG(SIM_DRV_1177_112_2_17_23_6_28_1140,"\r\nSIMDRV%d::set the working volt to 1.8v.")
TRACE_MSG(SIM_DRV_1182_112_2_17_23_6_28_1141,"\r\nSIMDRV%d::set the working volt to 3.0v.")
TRACE_MSG(SIM_DRV_1187_112_2_17_23_6_28_1142,"\r\nSIMDRV%d::do not change the working volt.")
TRACE_MSG(SIM_DRV_1196_112_2_17_23_6_28_1143,"\r\nSIMDRV%d::set the working volt to 1.8v.")
TRACE_MSG(SIM_DRV_1201_112_2_17_23_6_28_1144,"\r\nSIMDRV%d::set the working volt to 3.0v.")
TRACE_MSG(SIM_DRV_1206_112_2_17_23_6_28_1145,"\r\nSIMDRV%d::do not change the working volt.")
TRACE_MSG(SIM_DRV_1215_112_2_17_23_6_28_1146,"SIMDRV2::set the working volt to 1.8v.")
TRACE_MSG(SIM_DRV_1220_112_2_17_23_6_28_1147,"SIMDRV2::set the working volt to 3v.")
TRACE_MSG(SIM_DRV_1237_112_2_17_23_6_28_1148,"\r\nSIMDRV%d::set the working volt to 1.8v.")
TRACE_MSG(SIM_DRV_1242_112_2_17_23_6_28_1149,"\r\nSIMDRV%d::set the working volt to 3.0v.")
TRACE_MSG(SIM_DRV_1254_112_2_17_23_6_28_1150,"\r\nSIMDRV%d::do not change the working volt.")
TRACE_MSG(SIM_DRV_1263_112_2_17_23_6_28_1151,"\r\nSIMDRV%d::set the working volt to 1.8v.")
TRACE_MSG(SIM_DRV_1268_112_2_17_23_6_28_1152,"\r\nSIMDRV%d::set the working volt to 3.0v.")
TRACE_MSG(SIM_DRV_1280_112_2_17_23_6_28_1153,"\r\nSIMDRV%d::do not change the working volt.")
TRACE_MSG(SIM_DRV_1289_112_2_17_23_6_28_1154,"SIMDRV2::set the working volt to 1.8v.")
TRACE_MSG(SIM_DRV_1294_112_2_17_23_6_28_1155,"SIMDRV2::set the working volt to 3v.")
TRACE_MSG(SIM_DRV_1303_112_2_17_23_6_28_1156,"SIMDRV3::set the working volt to 1.8v.")
TRACE_MSG(SIM_DRV_1308_112_2_17_23_6_28_1157,"SIMDRV3::set the working volt to 3v.")
TRACE_MSG(SIM_DRV_1599_112_2_17_23_6_28_1158,"SIMDRV%d:power off")
TRACE_MSG(SIM_DRV_2167_112_2_17_23_6_29_1159,"SIMDRV:reg 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x")
TRACE_MSG(USIM_DRV_1175_112_2_17_23_6_32_1160,"USIMDRV:[%d]:DATA::RX 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X")
TRACE_MSG(USIM_DRV_1590_112_2_17_23_6_33_1161,"USIMDRV:[%d]:[ResetDriver] Cur_Volt[%d],Tick[%d]")
TRACE_MSG(USIM_DRV_1602_112_2_17_23_6_33_1162,"USIMDRV:[%d]:[ResetDriver] DMA_Support_Flag[%d]")
TRACE_MSG(USIM_DRV_1625_112_2_17_23_6_33_1163,"USIMDRV:[0]: [Sim1InitDrvAfterLdoValid] Tick[%d]")
TRACE_MSG(USIM_DRV_1649_112_2_17_23_6_33_1164,"USIMDRV:[0]: [Sim1InitDrvAfterLdoValid] Cur_Clk[%d],Clock_Mode[%d]")
TRACE_MSG(USIM_DRV_1697_112_2_17_23_6_33_1165,"USIMDRV:[1]: [Sim2InitDrvAfterLdoValid] Tick[%d]")
TRACE_MSG(USIM_DRV_1715_112_2_17_23_6_33_1166,"USIMDRV:[1]: [Sim2InitDrvAfterLdoValid] Cur_Clk[%d],Clock_Mode[%d]")
TRACE_MSG(USIM_DRV_1797_112_2_17_23_6_33_1167,"USIMDRV:[%d]:API [SIM_WarmResetSimCard] Warm Reset the SIM card!")
TRACE_MSG(USIM_DRV_1863_112_2_17_23_6_33_1168,"USIMDRV:[%d]:API [SIM_WarmResetSimCard] Tick[%d]")
TRACE_MSG(USIM_DRV_1908_112_2_17_23_6_34_1169,"USIMDRV:[%d]: s_is_sim_out = TRUE;TX:Need De-active SIM!!!")
TRACE_MSG(USIM_DRV_2098_112_2_17_23_6_34_1170,"USIMDRV:[%d]:tx_data_len[%d], rx_expected_len[%d]")
TRACE_MSG(USIM_DRV_2106_112_2_17_23_6_34_1171,"USIMDRV:[%d]:0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X")
TRACE_MSG(USIM_DRV_2224_112_2_17_23_6_34_1172,"USIMDRV:[%d]:[SendInstrCode_T1] IFSC[0x%x], Ins_Type[0x%x], P3[0x%x]")
TRACE_MSG(USIM_DRV_2494_112_2_17_23_6_35_1173,"USIMDRV:[%d]:[SIM_PowerOffSIM]")
TRACE_MSG(USIM_DRV_2693_112_2_17_23_6_35_1174,"USIMDRV:[%d]:[SIM_SelMFAckClkVol] Clk_Phase_in_MF != Clk_Phase_in_ATR, Clk_Phase_in_MF[%d]")
TRACE_MSG(USIM_DRV_2700_112_2_17_23_6_35_1175,"USIMDRV:[%d]:[SIM_SelMFAckClkVol] Expected_Voltage[%d], Current_Voltage[%d]")
TRACE_MSG(USIM_DRV_2762_112_2_17_23_6_35_1176,"USIMDRV:[%d]:[SIM_SelMFAckClkVol] Expected_Voltage != Working_Voltage ,SIM Reset Needed!")
TRACE_MSG(USIM_DRV_2784_112_2_17_23_6_35_1177,"USIMDRV:[0]: [Sim1DriverTimerExpired] ERROR:SIM1 Module Disalbe but Receive Timer Interrupt[%d]!")
TRACE_MSG(USIM_DRV_2792_112_2_17_23_6_35_1178,"USIMDRV:[0]: [Sim1DriverTimerExpired] ATR Wait timer expired")
TRACE_MSG(USIM_DRV_2815_112_2_17_23_6_35_1179,"USIMDRV:[0]: [Sim1DriverTimerExpired] Parity_Change_Timer expired")
TRACE_MSG(USIM_DRV_2825_112_2_17_23_6_35_1180,"USIMDRV:[0]: [Sim1DriverTimerExpired] Bit_Convention_Timer expired")
TRACE_MSG(USIM_DRV_2837_112_2_17_23_6_35_1181,"USIMDRV:[0]: [Sim1DriverTimerExpired] Status_Word_Timer expired SW1[0x%02x], SW2[0x%02x]")
TRACE_MSG(USIM_DRV_2858_112_2_17_23_6_35_1182,"USIMDRV:[0]: [Sim1DriverTimerExpired] BWT_Timer expired,Tick[0x%x],Rx_Cur_Pos[0x%x],Rx_Blk_Ext_Byte_Cnt[0x%x],Temp[0x%x]")
TRACE_MSG(USIM_DRV_2868_112_2_17_23_6_35_1183,"USIMDRV:[0]: [Sim1DriverTimerExpired] Wait_Response_Timer expired ,Tick[0x%x]")
TRACE_MSG(USIM_DRV_2876_112_2_17_23_6_36_1184,"USIMDRV:[0]: [Sim1DriverTimerExpired] Wait_Response_Timer expired, SIM_Status[%d]")
TRACE_MSG(USIM_DRV_2901_112_2_17_23_6_36_1185,"USIMDRV:[0]: [Sim1DriverTimerExpired] Wait_Response_Timer expired, add NotifyExceptionError (TB_SIM_PROCESSING_PPS) !")
TRACE_MSG(USIM_DRV_2932_112_2_17_23_6_36_1186,"USIMDRV:[0]: [Sim1DriverTimerExpired] T1_CWT_Timer expired, Tick[0x%x], Parity[0x%x]")
TRACE_MSG(USIM_DRV_2945_112_2_17_23_6_36_1187,"USIMDRV:[0]: [Sim1DriverTimerExpired] RX_Timeout_Timer expired, Tick[0x%x],Rcv_Byte_Cnt[0x%x]")
TRACE_MSG(USIM_DRV_2968_112_2_17_23_6_36_1188,"USIMDRV:[0]: [Sim1DriverTimerExpired] ERROR:Invalid_Timer g_user_mode_flag is TRUE.")
TRACE_MSG(USIM_DRV_2988_112_2_17_23_6_36_1189,"USIMDRV:[1]: [Sim2DriverTimerExpired] ERROR:SIM2 Module Disalbe but Receive Timer Interrupt[%d]!")
TRACE_MSG(USIM_DRV_2995_112_2_17_23_6_36_1190,"USIMDRV:[1]: [Sim2DriverTimerExpired] ATR_Wait_Timer expired")
TRACE_MSG(USIM_DRV_3018_112_2_17_23_6_36_1191,"USIMDRV:[1]: [Sim2DriverTimerExpired] Parity_Change_Timer expired")
TRACE_MSG(USIM_DRV_3028_112_2_17_23_6_36_1192,"USIMDRV:[1]: [Sim2DriverTimerExpired] Bit_Convention_Timer expired")
TRACE_MSG(USIM_DRV_3040_112_2_17_23_6_36_1193,"USIMDRV:[1]: [Sim2DriverTimerExpired] Status_Word_Timer expired, SW1[0x%02x], SW2[0x%02x]")
TRACE_MSG(USIM_DRV_3061_112_2_17_23_6_36_1194,"USIMDRV:[1]: [Sim2DriverTimerExpired] BWT_Timer expired,Tick[0x%x],Rx_Cur_Pos[0x%x],Rx_Blk_Ext_Byte_Cnt[0x%x],Temp[0x%x]")
TRACE_MSG(USIM_DRV_3071_112_2_17_23_6_36_1195,"USIMDRV:[1]: [Sim2DriverTimerExpired] Wait_Response_Timer expired ,Tick[0x%x]")
TRACE_MSG(USIM_DRV_3079_112_2_17_23_6_36_1196,"USIMDRV:[1]: [Sim2DriverTimerExpired] Wait_Response_Timer expired, SIM_Status[%d]")
TRACE_MSG(USIM_DRV_3104_112_2_17_23_6_36_1197,"USIMDRV:[1]: [Sim2DriverTimerExpired] Wait_Response_Timer expired, NotifyExceptionError (TB_SIM_PROCESSING_PPS) !")
TRACE_MSG(USIM_DRV_3117_112_2_17_23_6_36_1198,"usimdrv:: t0 wwt timer expired [0x%x],rcv byte[0x%x]")
TRACE_MSG(USIM_DRV_3140_112_2_17_23_6_36_1199,"USIMDRV:[1]: [Sim2DriverTimerExpired] T1_CWT_Timer expired, Tick[0x%x], Parity[0x%x]")
TRACE_MSG(USIM_DRV_3153_112_2_17_23_6_36_1200,"USIMDRV:[1]: [Sim2DriverTimerExpired] RX_Timeout_Timer expired, Tick[0x%x],Rcv_Byte_Cnt[0x%x]")
TRACE_MSG(USIM_DRV_3176_112_2_17_23_6_36_1201,"USIMDRV:[1]: [Sim2DriverTimerExpired] ERROR:Invalid_Timer g_user_mode_flag is TRUE.")
TRACE_MSG(USIM_DRV_3314_112_2_17_23_6_36_1202,"USIMDRV:[%d]:[ReportCardStatus] insert_status[%d]")
TRACE_MSG(USIM_DRV_3662_112_2_17_23_6_37_1203,"USIMDRV:[%d] s_is_sim_out = TRUE;RX:Need De-active SIM!!!")
TRACE_MSG(USIM_DRV_3783_112_2_17_23_6_37_1204,"USIMDRV:[%d]:[RxFifoFullIntP_T0] Section1, cur_pos[%d]")
TRACE_MSG(USIM_DRV_3790_112_2_17_23_6_37_1205,"USIMDRV:[%d]:[RxFifoFullIntP_T0] NULL byte recieved before ACK!!!")
TRACE_MSG(USIM_DRV_3821_112_2_17_23_6_37_1206,"USIMDRV:[%d]:[RxFifoFullIntP_T0] NULL byte(0x60) received!!!")
TRACE_MSG(USIM_DRV_3835_112_2_17_23_6_37_1207,"USIMDRV:[%d]:[RxFifoFullIntP_T0] Section2, cur_pos[%d]")
TRACE_MSG(USIM_DRV_3909_112_2_17_23_6_38_1208,"USIMDRV:[%d]:[RxFifoFullIntP_T1] Tick[0x%x], CWT[0x%x]")
TRACE_MSG(USIM_DRV_3995_112_2_17_23_6_38_1209,"USIMDRV:[%d]:[RxFifoFullIntP_T1] Tick[0x%x],CWT[0x%x],Temp[0x%x],IFSD[0x%x]")
TRACE_MSG(USIM_DRV_4029_112_2_17_23_6_38_1210,"USIMDRV:[%d]:[RxFifoFullIntP_T1]Ext_count[0x%x], Ext_byte[0x%x,0x%x,0x%x,0x%x,0x%x],EDC_Flag[0x%x]")
TRACE_MSG(USIM_DRV_4067_112_2_17_23_6_38_1211,"USIMDRV:[%d]:[RxFifoFullIntP_T1]Ext_count[0x%x],Ext_byte[0x%x,0x%x,0x%x,0x%x,0x%x],EDC_Flag[0x%x],cur_pos[0x%x]")
TRACE_MSG(USIM_DRV_4200_112_2_17_23_6_38_1212,"USIMDRV:[%d]:[EarlyATRIntP] card OUT in early ATR interrup")
TRACE_MSG(USIM_DRV_4205_112_2_17_23_6_38_1213,"USIMDRV:[%d]:[EarlyATRIntP] Early ATR Interrupt!")
TRACE_MSG(USIM_DRV_4234_112_2_17_23_6_38_1214,"USIMDRV:[%d]:[RxParityErrorIntP] sim_status[0x%x]")
TRACE_MSG(USIM_DRV_4241_112_2_17_23_6_38_1215,"USIMDRV:: RX parity error happens in PPS procedure")
TRACE_MSG(USIM_DRV_4307_112_2_17_23_6_38_1216,"USIMDRV:[%d]:[TxParityErrorIntP] WARNING")
TRACE_MSG(USIM_DRV_4353_112_2_17_23_6_39_1217,"USIMDRV:[%d]:[CardUnResponseIntP] Tick[0x%x],Rx_Count[0x%x]")
TRACE_MSG(USIM_DRV_4457_112_2_17_23_6_39_1218,"USIMDRV:[%d]:[SendResponseUp] NULL byte data!!!")
TRACE_MSG(USIM_DRV_4461_112_2_17_23_6_39_1219,"USIMDRV:[%d]:[SendResponseUp] rev_data_len > exp_data_len")
TRACE_MSG(USIM_DRV_4501_112_2_17_23_6_39_1220,"USIMDRV:[%d]:resp_data_len[%d],sw [0x%02x, 0x%02x]")
TRACE_MSG(USIM_DRV_4509_112_2_17_23_6_39_1221,"USIMDRV:[%d]:0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X")
TRACE_MSG(USIM_DRV_4528_112_2_17_23_6_39_1222,"USIMDRV:[%d]:[SendResponseUp] [0x%x, 0x%x, 0x%x]")
TRACE_MSG(USIM_DRV_4594_112_2_17_23_6_39_1223,"USIMDRV:[%d]:[SendPPSRequest] DO PPS Request")
TRACE_MSG(USIM_DRV_4655_112_2_17_23_6_39_1224,"USIMDRV:[%d]:[ResetSIMCard] Reset_type[%d]!")
TRACE_MSG(USIM_DRV_4707_112_2_17_23_6_39_1225,"USIMDRV:[%d]:[ResetSIMCard] Reset the first time!")
TRACE_MSG(USIM_DRV_4739_112_2_17_23_6_39_1226,"USIMDRV:[%d]:[ResetSIMCard] Reset for change_power!")
TRACE_MSG(USIM_DRV_4751_112_2_17_23_6_39_1227,"USIMDRV:[%d]:[ResetSIMCard] Reject for NO next available voltage any more!")
TRACE_MSG(USIM_DRV_4760_112_2_17_23_6_39_1228,"USIMDRV:[%d]:[ResetSIMCard] Reset for no_change!")
TRACE_MSG(USIM_DRV_4770_112_2_17_23_6_39_1229,"USIMDRV:[%d]:[ResetSIMCard]  Reset for next_power LDO_Level[%d]")
TRACE_MSG(USIM_DRV_4783_112_2_17_23_6_39_1230,"USIMDRV:[%d]:[ResetSIMCard] Reset for bit_convention,bit_convent[%d]")
TRACE_MSG(USIM_DRV_4809_112_2_17_23_6_39_1231,"USIMDRV:[%d]:[ResetSIMCard] Reset for change_parity!")
TRACE_MSG(USIM_DRV_5000_112_2_17_23_6_40_1232,"USIMDRV:[%d]:[IdleSIMCard] ATR Default ,MF == CLOCK_STOP_NOT_SUPPORT ,the UICC NOT support clock stop mode")
TRACE_MSG(USIM_DRV_5157_112_2_17_23_6_40_1233,"USIMDRV:[%d]:[CheckReceiveBuf] ATR[0x%x], cur_pos[0x%x]")
TRACE_MSG(USIM_DRV_5209_112_2_17_23_6_40_1234,"USIMDRV:[%d]:[CheckReceiveBuf] corrupted ATR data")
TRACE_MSG(USIM_DRV_5217_112_2_17_23_6_40_1235,"USIMDRV:[%d]:[CheckReceiveBuf] active s_sim_wait_resp_timer!")
TRACE_MSG(USIM_DRV_5250_112_2_17_23_6_40_1236,"USIMDRV:[%d]:[CheckReceiveBuf] ALL ATR received ,cur_pos[0x%x], exp_len[0x%x]")
TRACE_MSG(USIM_DRV_5256_112_2_17_23_6_40_1237,"USIMDRV:[%d]:[CheckReceiveBuf] deactive s_sim_wait_resp_timer!")
TRACE_MSG(USIM_DRV_5299_112_2_17_23_6_41_1238,"USIMDRV:[%d]:[CheckReceiveBuf]  Reset for next_voltage ATR_Vol[%d]")
TRACE_MSG(USIM_DRV_5341_112_2_17_23_6_41_1239,"USIMDRV:[%d]:[CheckReceiveBuf] use enhanced PPS data 512 8")
TRACE_MSG(USIM_DRV_5348_112_2_17_23_6_41_1240,"USIMDRV:[%d]:[CheckReceiveBuf] use enhanced PPS data 512 16")
TRACE_MSG(USIM_DRV_5356_112_2_17_23_6_41_1241,"USIMDRV:[%d]:[CheckReceiveBuf] use enhanced PPS data 512 32")
TRACE_MSG(USIM_DRV_5364_112_2_17_23_6_41_1242,"USIMDRV:[%d]:[CheckReceiveBuf] use default PPS data 372 1")
TRACE_MSG(USIM_DRV_5383_112_2_17_23_6_41_1243,"USIMDRV:[%d]:[CheckReceiveBuf] NOT do PPS procedure!")
TRACE_MSG(USIM_DRV_5410_112_2_17_23_6_41_1244,"USIMDRV:[%d]:[CheckReceiveBuf] All ATR received! sim_status[0x%x]")
TRACE_MSG(USIM_DRV_5415_112_2_17_23_6_41_1245,"USIMDRV:[%d]:[CheckReceiveBuf] NOT all ATR received !")
TRACE_MSG(USIM_DRV_5419_112_2_17_23_6_41_1246,"USIMDRV:[%d]:[CheckReceiveBuf] active s_sim_wait_resp_timer!")
TRACE_MSG(USIM_DRV_5448_112_2_17_23_6_41_1247,"USIMDRV:[%d]:[CheckReceiveBuf] Received PPS data error!!! Reset with no PPS!!!")
TRACE_MSG(USIM_DRV_5457_112_2_17_23_6_41_1248,"USIMDRV:[%d]:[CheckReceiveBuf] NotifyExceptionError (TB_SIMEXP_CARD_OUT) !")
TRACE_MSG(USIM_DRV_5492_112_2_17_23_6_41_1249,"USIMDRV:[%d]:[CheckReceiveBuf] Enhanced pps supported!")
TRACE_MSG(USIM_DRV_5527_112_2_17_23_6_41_1250,"USIMDRV:[%d]:[CheckReceiveBuf] received NULL_Byte(0x60), like status word, not sendresponse up, some data may report later !")
TRACE_MSG(USIM_DRV_5562_112_2_17_23_6_41_1251,"USIMDRV:[%d]:[CheckReceiveBuf] expected_len[%d] <= cur_pos[%d]")
TRACE_MSG(USIM_DRV_5677_112_2_17_23_6_41_1252,"USIMDRV:[%d]:[CheckBlockValid] Section1 Tick[0x%x], CWT_Timer[0x%x], block_valid[0x%x], block_type[0x%x]")
TRACE_MSG(USIM_DRV_5745_112_2_17_23_6_41_1253,"USIMDRV:[%d]:[CheckBlockValid] Section2 tx_block_type[0x%x], rx_block_type[0x%x],block_validity[0x%x],Tick[0x%x],info_len[0x%x]")
TRACE_MSG(USIM_DRV_5806_112_2_17_23_6_42_1254,"USIMDRV:[%d]:[CheckBlockValid] send_next block_begin_pos[0x%x], endpos[0x%x], tx_pcb[0x%x]")
TRACE_MSG(USIM_DRV_5825_112_2_17_23_6_42_1255,"USIMDRV:[%d]:[CheckBlockValid] resend block_begin_pos[0x%x], endpos[0x%x], tx_pcb[0x%x]")
TRACE_MSG(USIM_DRV_5878_112_2_17_23_6_42_1256,"USIMDRV:[%d]:[CheckNRValid] input pcb_nr[0x%x], ns[0x%x], block_type[0x%x]")
TRACE_MSG(USIM_DRV_5907_112_2_17_23_6_42_1257,"USIMDRV:[%d]:[CheckBlockLenValid] ifsd[0x%x],act_len[0x%x],block_type[0x%x],chain[0x%x]")
TRACE_MSG(USIM_DRV_6203_112_2_17_23_6_42_1258,"USIMDRV:[%d]:[DecodeATRData] TC1 default[0x%x]to be [0x%x]")
TRACE_MSG(USIM_DRV_6237_112_2_17_23_6_42_1259,"USIMDRV:[%d]:[ErrorHandler] reset_count[0x%x], block_type[0x%x], error_code[0x%x]")
TRACE_MSG(USIM_DRV_6255_112_2_17_23_6_43_1260,"USIMDRV:[%d]:[ErrorHandler] When I-Block BWT time-out,send a r-block")
TRACE_MSG(USIM_DRV_6263_112_2_17_23_6_43_1261,"USIMDRV:[%d]:[ErrorHandler] When R-block BWT time-out,resent a s-block request")
TRACE_MSG(USIM_DRV_6272_112_2_17_23_6_43_1262,"USIMDRV:[%d]:[ErrorHandler] When S-block request BWT time-out,resent a s-block request")
TRACE_MSG(USIM_DRV_6278_112_2_17_23_6_43_1263,"USIMDRV:[%d]:[ErrorHandler] When S-block response BWT time-out,send a r-block")
TRACE_MSG(USIM_DRV_6321_112_2_17_23_6_43_1264,"USIMDRV:[%d]:[ErrorHandler] Unknown Block Type ,refer to 7.3.13")
TRACE_MSG(USIM_DRV_6329_112_2_17_23_6_43_1265,"USIMDRV:[%d]:[ErrorHandler] resynch req needed, refer to 7.3.13")
TRACE_MSG(USIM_DRV_6338_112_2_17_23_6_43_1266,"USIMDRV:[%d]:[ErrorHandler] continous BWT time-out or invalid block to the max time")
TRACE_MSG(USIM_DRV_6485_112_2_17_23_6_43_1267,"USIMDRV:[%d]:[GetT0WWT] WI[0x%x] WWT[0x%x] REAL_WWT = WWT +20")
TRACE_MSG(USIM_DRV_6511_112_2_17_23_6_43_1268,"USIMDRV:[%d]:[GetT1CWT] CWI[0x%x], CWT[0x%x]")
TRACE_MSG(USIM_DRV_6544_112_2_17_23_6_43_1269,"USIMDRV:[%d]:[GetBWT] BWI[0x%x] BWT[0x%x], NEW_BWT[0x%x]")
TRACE_MSG(USIM_DRV_6648_112_2_17_23_6_43_1270,"USIMDRV:[%d]:[GetBlockBaseInfo] PCB_Byte[0x%x], Tick[0x%x]")
TRACE_MSG(USIM_DRV_6716_112_2_17_23_6_44_1271,"USIMDRV:[%d]:[GetBlockBaseInfo] NR[0x%x], NS[0x%x], block_type[0x%x]")
TRACE_MSG(USIM_DRV_6748_112_2_17_23_6_44_1272,"USIMDRV:[%d]:[GetBlockBaseInfo] ERROR: Invalid PCB")
TRACE_MSG(USIM_DRV_6909_112_2_17_23_6_44_1273,"USIMDRV:[%d]:[SendRBlock] Tick[0x%x] temp[0x%x,0x%x,0x%x,0x%x]")
TRACE_MSG(USIM_DRV_6920_112_2_17_23_6_44_1274,"USIMDRV:[%d]:[SendRBlock] BWT[0x%x]")
TRACE_MSG(USIM_DRV_7019_112_2_17_23_6_44_1275,"USIMDRV:[%d]:[GetT0WWT]  reason[0x%x],0x%x,[0x%x,0x%x,0x%x,0x%x,0x%x]")
TRACE_MSG(USIM_DRV_7037_112_2_17_23_6_44_1276,"USIMDRV:[%d]:[GetT0WWT] sendsblock the bwt is 0x%x")
TRACE_MSG(USIM_DRV_7053_112_2_17_23_6_44_1277,"\r\nUSIMDRV::Callback func is null")
TRACE_MSG(USIM_DRV_7073_112_2_17_23_6_44_1278,"USIMDRV:[%d]:[GetNextSupplyVoltage] 1.8V -> 3V")
TRACE_MSG(USIM_DRV_7137_112_2_17_23_6_44_1279,"USIMDRV:[%d]:[SetSIMPower] do not support 5v")
TRACE_MSG(USIM_DRV_7155_112_2_17_23_6_44_1280,"USIMDRV:[%d]:[SetSIMPower] Before setting LDO Level[%d]")
TRACE_MSG(USIM_DRV_7167_112_2_17_23_6_44_1281,"USIMDRV:[%d]:[SetSIMPower] do not support 5v")
TRACE_MSG(USIM_DRV_7173_112_2_17_23_6_44_1282,"USIMDRV:[%d]:[SetSIMPower] After seting LDO Level[%d]")
TRACE_MSG(USIM_DRV_7191_112_2_17_23_6_44_1283,"USIMDRV:[0]:[Sim1ResetAfterLdoValid] Tick[%d]")
TRACE_MSG(USIM_DRV_7283_112_2_17_23_6_45_1284,"USIMDRV:[1]:[Sim2ResetAfterLdoValid] Tick[%d]")
TRACE_MSG(USIM_DRV_7422_112_2_17_23_6_45_1285,"USIMDRV:[%d]:[HandleUICCSBlock] wrong block type 0x%x?")
TRACE_MSG(USIM_DRV_7463_112_2_17_23_6_45_1286,"USIMDRV:[%d]:[HandleResyncRsp]  handle resync rsp nr[0x%x]ns[0x%x]tx_len[0x%x]")
TRACE_MSG(USIM_DRV_7803_112_2_17_23_6_46_1287,"USIMDRV:[%d]:[DMA_SendSBLOCK] Tick[0x%x],Reason[0x%x],Ext_Cnt[0x%x],Ext_data[0x%x,0x%x,0x%x,0x%x,0x%x]")
TRACE_MSG(USIM_DRV_7822_112_2_17_23_6_46_1288,"USIMDRV:[%d]:[DMA_SendSBLOCK] ATR.BWT[0x%x]")
TRACE_MSG(USIM_DRV_7905_112_2_17_23_6_46_1289,"USIMDRV:[%d]:[DMA_SendRBlock] Tick[0x%x], Buff[0x%x,0x%x,0x%x,0x%x]")
TRACE_MSG(USIM_DRV_7916_112_2_17_23_6_46_1290,"USIMDRV:[%d]:[DMA_SendRBlock] ATR.BWT[0x%x]")
TRACE_MSG(USIM_DRV_7969_112_2_17_23_6_46_1291,"USIMDRV:[%d]:[DMA_CheckBlockValid] Tick[0x%x], CWT_Timer[0x%x], Status[0x%x], Tx_Block_Type[0x%x]")
TRACE_MSG(USIM_DRV_8037_112_2_17_23_6_46_1292,"USIMDRV:[%d]:[DMA_CheckBlockValid] tx_type[0x%x], rx_type[0x%x],block_validity[0x%x],Tick[0x%x], Info_Len[0x%x]")
TRACE_MSG(USIM_DRV_8098_112_2_17_23_6_46_1293,"USIMDRV:[%d]:[DMA_CheckBlockValid] Send_Next curpos[0x%x], endpos[0x%x], tx pcb[0x%x]")
TRACE_MSG(USIM_DRV_8117_112_2_17_23_6_46_1294,"USIMDRV:[%d]:[DMA_CheckBlockValid] Resend curpos[0x%x], endpos[0x%x], tx pcb[0x%x]")
TRACE_MSG(USIM_DRV_8191_112_2_17_23_6_46_1295,"USIMDRV:[%d]:[DMA_CheckReceiveBuf] expected len[%d] <= cur_pos[%d]")
TRACE_MSG(USIM_DRV_8265_112_2_17_23_6_47_1296,"USIMDRV:[%d]:[DMA_TX_Callback_T0] tx_data_len[%d],tx_cur_pos[%d], ACK_Flag[%d]")
TRACE_MSG(USIM_DRV_8495_112_2_17_23_6_47_1297,"USIMDRV:[%d]:[DMA_RX_Callback_T0] data_temp[0x%2x], rx_cur_pos[%d]")
TRACE_MSG(USIM_DRV_8500_112_2_17_23_6_47_1298,"USIMDRV:[%d]:[DMA_RX_Callback_T0] rx_buf is locked!!!")
TRACE_MSG(USIM_DRV_8582_112_2_17_23_6_47_1299,"USIMDRV:[%d]:[DMA_RX_Callback_T0]  reset s_sim_rx_buf[card_id], cur_pos[%d]")
TRACE_MSG(USIM_DRV_8589_112_2_17_23_6_47_1300,"USIMDRV:[%d]:[DMA_RX_Callback_T0]  NULL_byte(0x60) before ACK!!!")
TRACE_MSG(USIM_DRV_8624_112_2_17_23_6_47_1301,"USIMDRV:[%d]:[DMA_RX_Callback_T0]  reset s_sim_rx_buf[card_id], cur_pos[%d]")
TRACE_MSG(USIM_DRV_8701_112_2_17_23_6_47_1302,"USIMDRV:[%d]:[DMA_RX_Callback_T1] rxfifofull Tick[0x%x], CWT_Timer[0x%x]")
TRACE_MSG(USIM_DRV_8808_112_2_17_23_6_48_1303,"USIMDRV:[%d]:[DMA_RX_Callback_T1] rxfifofull t1 Tick[0x%x],CWT_Timer[0x%x],[0x%x,0x%x]")
TRACE_MSG(USIM_DRV_8843_112_2_17_23_6_48_1304,"USIMDRV:[%d]:[DMA_RX_Callback_T1] Section1 s_sim_rx_block Ext_Cnt[0x%x], EXT[0x%x,0x%x,0x%x,0x%x,0x%x], EDC[0x%x]")
TRACE_MSG(USIM_DRV_8876_112_2_17_23_6_48_1305,"USIMDRV:[%d]:[DMA_RX_Callback_T1] Section2 Ext_Cnt[0x%x], EXT[0x%x,0x%x,0x%x,0x%x,0x%x],EDC[0x%x],rcv[0x%x]")
TRACE_MSG(USIM_DRV_8905_112_2_17_23_6_48_1306,"USIMDRV:[%d]:[USIM_DMA_Init]")
TRACE_MSG(USIM_DRV_9070_112_2_17_23_6_48_1307,"USIMDRV:[%d]:tx_data_len is[%d], rx_expected_len[%d]")
TRACE_MSG(USIM_DRV_9078_112_2_17_23_6_48_1308,"USIMDRV:[%d]:0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X, 0x%02X 0x%02X 0x%02X 0x%02X")
TRACE_MSG(USIM_DRV_9154_112_2_17_23_6_48_1309,"USIMDRV:[%d]:[DMA_SendInstrCode_T1] IFSC[0x%x], instr_type[0x%x], P3[0x%x]")
TRACE_MSG(USIM_DRV_9358_112_2_17_23_6_49_1310,"USIMDRV:[0]:[Sim1DMATimerExpired] Tick[0x%x],rcv_byte[0x%x]")
TRACE_MSG(USIM_DRV_9392_112_2_17_23_6_49_1311,"USIMDRV:[0]:[Sim1DMATimerExpired] DMA BWT timer expired[0x%x][0x%x,0x%x],0x%x")
TRACE_MSG(USIM_DRV_9404_112_2_17_23_6_49_1312,"USIMDRV:[0]:[Sim1DMATimerExpired] DMA WWT timer expired[0x%x][0x%x],0x%x")
TRACE_MSG(USIM_DRV_9447_112_2_17_23_6_49_1313,"USIMDRV:[1]:[Sim2DMATimerExpired]  DMATimerExpired[0x%x],rcv byte[0x%x]")
TRACE_MSG(USIM_DRV_9481_112_2_17_23_6_49_1314,"USIMDRV:[1]:[Sim2DMATimerExpired] DMA BWT timer expired[0x%x][0x%x,0x%x],0x%x")
TRACE_MSG(USIM_DRV_9493_112_2_17_23_6_49_1315,"USIMDRV:[1]:[Sim2DMATimerExpired] DMA WWT timer expired[0x%x][0x%x],0x%x")
TRACE_MSG(SPI_HAL_TEST_206_112_2_17_23_6_59_1316,"Spi_Thput_Test: case_size %d")
TRACE_MSG(SPI_HAL_TEST_265_112_2_17_23_6_59_1317,"Spi_Thput_Test: write: pre_tick %d, cur_tick=%d, time:%d")
TRACE_MSG(SPI_HAL_TEST_287_112_2_17_23_6_59_1318,"Spi_Thput_Test: read: pre_tick %d, cur_tick=%d, time:%d")
TRACE_MSG(TPC_PHY_V0_179_112_2_17_23_7_23_1319,"[TPDRV:TPDRV_EnableCoord]")
TRACE_MSG(TPC_PHY_V0_191_112_2_17_23_7_23_1320,"[TPDRV:TPDRV_DisableCoord]")
TRACE_MSG(TPC_PHY_V0_301_112_2_17_23_7_23_1321,"[TPDRV:Down at %d] ")
TRACE_MSG(TPC_PHY_V0_354_112_2_17_23_7_23_1322,"[TPDRV:Done at %d] ")
TRACE_MSG(TPC_PHY_V0_512_112_2_17_23_7_23_1323,"[TPDRV:TPDRV_Run %d]")
TRACE_MSG(TPC_PHY_V0_519_112_2_17_23_7_23_1324,"[TPDRV:TPDRV_Run %d, temp > 800]")
TRACE_MSG(TPC_PHY_V0_575_112_2_17_23_7_24_1325,"[TPDRV:TPDRV_Stop]")
TRACE_MSG(TPC_PHY_V0_595_112_2_17_23_7_24_1326,"[TPDRV:TPDRV_FetchData] buffer empty")
TRACE_MSG(TPC_PHY_V0_626_112_2_17_23_7_24_1327,"[TPDRV:TPDRV_FetchData x %x] ")
TRACE_MSG(TPC_PHY_V0_651_112_2_17_23_7_24_1328,"[TPDRV:TPDRV_FetchData y %x] ")
TRACE_MSG(TPC_PHY_V0_682_112_2_17_23_7_24_1329,"[TPDRV:TPDRV_FetchData] buffer empty")
TRACE_MSG(TPC_PHY_V0_698_112_2_17_23_7_24_1330,"[TPDRV:_FetchData]current pressure factor:%d")
TRACE_MSG(TPC_PHY_V1_447_112_2_17_23_7_25_1331,"tp_conf_cal_parameter:x_slop =%d, x_offset = %d, y_slop = %d, y_offset = %d ")
TRACE_MSG(TPC_PHY_V3_177_112_2_17_23_7_26_1332,"[TPDRV:TPDRV_EnableCoord]")
TRACE_MSG(TPC_PHY_V3_186_112_2_17_23_7_26_1333,"[TPDRV:TPDRV_DisableCoord]")
TRACE_MSG(TPC_PHY_V3_235_112_2_17_23_7_26_1334,"[TPDRV:TPDRV_ConfigCoord]")
TRACE_MSG(TPC_PHY_V3_376_112_2_17_23_7_26_1335,"[TPDRV:TPDRV_Run ]")
TRACE_MSG(TPC_PHY_V3_395_112_2_17_23_7_26_1336,"[TPDRV:TPDRV_Stop]")
TRACE_MSG(TPC_PHY_V3_413_112_2_17_23_7_26_1337,"[TPDRV:TPDRV_FetchData] buffer not full 0x%x")
TRACE_MSG(TPC_PHY_V3_435_112_2_17_23_7_26_1338,"[TPDRV:TPDRV_FetchData] delta_x = %d delta_y = %d")
TRACE_MSG(TPC_ALGORITHM_263_112_2_17_23_7_27_1339,"%s")
TRACE_MSG(TPC_ALGORITHM_265_112_2_17_23_7_27_1340,"%s")
TRACE_MSG(TPC_ALGORITHM_303_112_2_17_23_7_27_1341,"%s")
TRACE_MSG(TPC_ALGORITHM_305_112_2_17_23_7_27_1342,"%s")
TRACE_MSG(TPC_ALGORITHM_308_112_2_17_23_7_27_1343,"%s")
TRACE_MSG(TPC_ALGORITHM_312_112_2_17_23_7_27_1344,"%s")
TRACE_MSG(TPC_ALGORITHM_317_112_2_17_23_7_27_1345,"%s")
TRACE_MSG(TPC_ALGORITHM_321_112_2_17_23_7_27_1346,"%s")
TRACE_MSG(TPC_ALGORITHM_329_112_2_17_23_7_27_1347,"%s")
TRACE_MSG(TPC_ALGORITHM_331_112_2_17_23_7_27_1348,"%s")
TRACE_MSG(TPC_ALGORITHM_333_112_2_17_23_7_27_1349,"%s")
TRACE_MSG(TPC_ALGORITHM_343_112_2_17_23_7_27_1350,"TPDRV_algorithm:----single_point---------------------==OK==----")
TRACE_MSG(TPC_ALGORITHM_347_112_2_17_23_7_27_1351,"TPDRV_algorithm:----single_point: -----------* -*invalid-* -*---")
TRACE_MSG(TPC_ALGORITHM_357_112_2_17_23_7_27_1352,"TPDRV_algorithm:----double_point: ----------------==OK==-------")
TRACE_MSG(TPC_ALGORITHM_362_112_2_17_23_7_27_1353,"TPDRV_algorithm:----double_point:-* -*-* -* -* -*invalid-* -* ")
TRACE_MSG(TPC_ALGORITHM_402_112_2_17_23_7_27_1354,"%s")
TRACE_MSG(TPC_ALGORITHM_405_112_2_17_23_7_27_1355,"TPDRV_algorithm:---------------interator_count=%d---------\r\n")
TRACE_MSG(TPC_ALGORITHM_409_112_2_17_23_7_27_1356,"TPDRV_algorithm:---------------interator_count=%d---------\r\n")
TRACE_MSG(TPC_ALGORITHM_433_112_2_17_23_7_27_1357,"[TPDRV_algorithm:(Xd1,Yd1)=[%d,%d], (Xd2,Yd2)=[%d,%d]rn")
TRACE_MSG(TPC_PHY_V5_255_112_2_17_23_7_28_1358,"\r\n %s")
TRACE_MSG(TPC_PHY_V5_256_112_2_17_23_7_28_1359," --->:ADC_HW_CH_DELAY =0x%8x")
TRACE_MSG(TPC_PHY_V5_257_112_2_17_23_7_28_1360," --->:ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL) =0x%8x")
TRACE_MSG(TPC_PHY_V5_258_112_2_17_23_7_28_1361," --->:ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL) =0x%8x")
TRACE_MSG(TPC_PHY_V5_259_112_2_17_23_7_28_1362," --->:ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL) =0x%8x")
TRACE_MSG(TPC_PHY_V5_260_112_2_17_23_7_28_1363," --->:ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL) =0x%8x")
TRACE_MSG(TPC_PHY_V5_262_112_2_17_23_7_28_1364," --->:TPC_SAMPLE_CTRL0 =0x%8x")
TRACE_MSG(TPC_PHY_V5_263_112_2_17_23_7_28_1365," --->:TPC_SAMPLE_CTRL1 =0x%8x")
TRACE_MSG(TPC_PHY_V5_264_112_2_17_23_7_28_1366," --->:TPC_SAMPLE_CTRL2 =0x%8x")
TRACE_MSG(TPC_PHY_V5_265_112_2_17_23_7_28_1367," --->:TPC_BOUNCE_CTRL   =0x%8x")
TRACE_MSG(TPC_PHY_V5_266_112_2_17_23_7_28_1368," --->:TPC_FILTER_CTRL   =0x%8x")
TRACE_MSG(TPC_PHY_V5_267_112_2_17_23_7_28_1369," --->:TPC_CALC_CTRL    =0x%8x")
TRACE_MSG(TPC_PHY_V5_268_112_2_17_23_7_28_1370," --->:TPC_BUF_CTRL    =0x%8x")
TRACE_MSG(TPC_PHY_V5_270_112_2_17_23_7_28_1371," --->:TPC_CTRL         =0x%8x")
TRACE_MSG(TPC_PHY_V5_271_112_2_17_23_7_28_1372," --->:TPC_DATA_MIN     =0x%8x")
TRACE_MSG(TPC_PHY_V5_272_112_2_17_23_7_28_1373," --->:TPC_DATA_MAX     =0x%8x")
TRACE_MSG(TPC_PHY_V5_273_112_2_17_23_7_28_1374," --->:TPC_CALC_X_COEF_A =0x%8x")
TRACE_MSG(TPC_PHY_V5_274_112_2_17_23_7_28_1375," --->:TPC_CALC_X_COEF_B =0x%8x")
TRACE_MSG(TPC_PHY_V5_275_112_2_17_23_7_28_1376," --->:TPC_CALC_Y_COEF_A =0x%8x")
TRACE_MSG(TPC_PHY_V5_276_112_2_17_23_7_28_1377," --->:TPC_CALC_Y_COEF_B =0x%8x")
TRACE_MSG(TPC_PHY_V5_278_112_2_17_23_7_28_1378," --->:TPC_INT_EN         =0x%8x")
TRACE_MSG(TPC_PHY_V5_279_112_2_17_23_7_28_1379," --->:TPC_INT_RAW         =0x%8x")
TRACE_MSG(TPC_PHY_V5_280_112_2_17_23_7_28_1380," --->:TPC_INT_CLR         =0x%8x")
TRACE_MSG(TPC_PHY_V5_282_112_2_17_23_7_28_1381," --->:TPC_OUT_SEQ1     =0x%8x")
TRACE_MSG(TPC_PHY_V5_283_112_2_17_23_7_28_1382," --->:TPC_OUT_SEQ0     =0x%8x")
TRACE_MSG(TPC_PHY_V5_284_112_2_17_23_7_28_1383," --->:TPC_OE_SEQ1      =0x%8x")
TRACE_MSG(TPC_PHY_V5_285_112_2_17_23_7_28_1384," --->:TPC_OE_SEQ0      =0x%8x")
TRACE_MSG(TPC_PHY_V5_286_112_2_17_23_7_28_1385," --->:TPC_CH_SEQ       =0x%8x")
TRACE_MSG(TPC_PHY_V5_287_112_2_17_23_7_28_1386," --->:TPC_RES_SEQ      =0x%8x")
TRACE_MSG(TPC_PHY_V5_288_112_2_17_23_7_28_1387," --->:TPC_SWT_SEQ      =0x%8x")
TRACE_MSG(TPC_PHY_V5_290_112_2_17_23_7_28_1388,"\r\n")
TRACE_MSG(TPC_PHY_V5_565_112_2_17_23_7_29_1389,"TPDRV:----------->TP_XY_EXCHANGE=1 --------->")
TRACE_MSG(TPC_PHY_V5_578_112_2_17_23_7_29_1390,"TPDRV:----------->_SeqCalibration------------>")
TRACE_MSG(TPC_PHY_V5_616_112_2_17_23_7_29_1391,"TPDRV:----------->TP_XY_EXCHANGE=1------------>")
TRACE_MSG(TPC_PHY_V5_626_112_2_17_23_7_29_1392,"TPDRV:----------->_SeqMultitouch------------>")
TRACE_MSG(TPC_PHY_V5_650_112_2_17_23_7_29_1393,"[TPDRV:TPDRV_Run ]")
TRACE_MSG(TPC_PHY_V5_665_112_2_17_23_7_29_1394,"[TPDRV:TPDRV_Stop]")
TRACE_MSG(TPC_PHY_V5_692_112_2_17_23_7_29_1395,"TPDRV:_FetchMultiData:")
TRACE_MSG(TPC_PHY_V5_748_112_2_17_23_7_29_1396,"rnTPDRV:Original_bef: Y=%df Y1=%df X=%df X1=%df Ry=%df Rx=%df RT=%df")
TRACE_MSG(TPC_PHY_V5_768_112_2_17_23_7_29_1397,"TPDRV:Original_ohm:x1_pixels=%f y1_pixels=%f RT_Y=%f RT_X=%f RT=%f")
TRACE_MSG(TPC_PHY_V5_777_112_2_17_23_7_29_1398,"TPDRV:Original_aft: Y=%f Y1=%f X=%f X1=%f Ry=%f Rx=%f RT=%f")
TRACE_MSG(TPC_PHY_V5_803_112_2_17_23_7_29_1399,"TPDRV:_FetchCalibrateData:\r\n")
TRACE_MSG(TPC_PHY_V5_859_112_2_17_23_7_29_1400,"%s")
TRACE_MSG(TPC_PHY_V5_865_112_2_17_23_7_29_1401,"TPDRV: adc raw data(%d,%d)\r\n")
TRACE_MSG(TPC_PHY_V5_886_112_2_17_23_7_29_1402,"TPDRV: TP_REF_U=%d TP_REF_R=%d")
TRACE_MSG(TPC_PHY_V5_898_112_2_17_23_7_29_1403,"%s")
TRACE_MSG(TPC_PHY_V5_1062_112_2_17_23_7_30_1404,"[TPDRV: TPDRV_Init]")
TRACE_MSG(TPC_PHY_V5_1077_112_2_17_23_7_30_1405,"[TPDRV:TPDRV_EnableCoord]")
TRACE_MSG(TPC_PHY_V5_1087_112_2_17_23_7_30_1406,"[TPDRV:TPDRV_DisableCoord]")
TRACE_MSG(TPC_PHY_V5_1128_112_2_17_23_7_30_1407,"[TPDRV:TPDRV_ConfigCoord]==%d")
TRACE_MSG(TPC_PHY_V5_1141_112_2_17_23_7_30_1408,"[TPDRV:TPDRV_EnterCalibration]")
TRACE_MSG(TPC_PHY_V5_1154_112_2_17_23_7_30_1409,"[TPDRV:TPDRV_EnterCalibration]")
TRACE_MSG(TPC_PHY_V5_1244_112_2_17_23_7_30_1410,"----\r\n\r\n")
TRACE_MSG(TPC_PHY_V5_1246_112_2_17_23_7_30_1411,"changde=========%s")
TRACE_MSG(TPC_PHY_V5_1248_112_2_17_23_7_30_1412,"changde=========%s")
TRACE_MSG(TPC_PHY_V5_1249_112_2_17_23_7_30_1413,"changde=========-%f")
TRACE_MSG(TPC_PHY_V5_1263_112_2_17_23_7_30_1414,"[TPDRV:_FetchData] (%d,%d),(%d,%d),size=%d")
TRACE_MSG(TPC_PHY_V5_1273_112_2_17_23_7_30_1415,"[TPDRV:_FetchData] return--1")
TRACE_MSG(TPC_PHY_V5_1281_112_2_17_23_7_30_1416,"[TPDRV:_FetchData] return--2")
TRACE_MSG(TPC_PHY_V5_1287_112_2_17_23_7_30_1417,"[TPDRV:_FetchData] return--3")
TRACE_MSG(TPC_PHY_V5_1293_112_2_17_23_7_30_1418,"[TPDRV:_FetchData] return--4")
TRACE_MSG(TPC_PHY_V5_1366_112_2_17_23_7_30_1419,"[TPDRV:_GetPoints] (%d,%d),size=%d")
TRACE_MSG(TPC_PHY_V5___110_112_2_17_23_7_31_1420,"\r\n %s")
TRACE_MSG(TPC_PHY_V5___111_112_2_17_23_7_31_1421," --->TPDRV:TPC_SAMPLE_DEBUG =0x%8x")
TRACE_MSG(TPC_PHY_V5___112_112_2_17_23_7_31_1422," --->TPDRV:TPC_DBNC_DEBUG   =0x%8x")
TRACE_MSG(TPC_PHY_V5___113_112_2_17_23_7_31_1423," --->TPDRV:TPC_BUF_DEBUG    =0x%8x")
TRACE_MSG(TPC_PHY_V5___114_112_2_17_23_7_31_1424," --->TPDRV:ADC_DEBUG        =0x%8x")
TRACE_MSG(TPC_PHY_V5___115_112_2_17_23_7_31_1425," --->TPDRV:ADC_CTRL         =0x%8x")
TRACE_MSG(TPC_PHY_V5___117_112_2_17_23_7_31_1426," --->TPDRV:0x820016ac       =0x%8x")
TRACE_MSG(TPC_PHY_V5___118_112_2_17_23_7_31_1427," --->TPDRV:0x820016a8       =0x%8x")
TRACE_MSG(TPC_PHY_V5___119_112_2_17_23_7_31_1428," --->TPDRV:0x820016b0       =0x%8x")
TRACE_MSG(TPC_PHY_V5___120_112_2_17_23_7_31_1429," --->TPDRV:0x820016b4       =0x%8x")
TRACE_MSG(TPC_PHY_V5___122_112_2_17_23_7_31_1430," --->TPDRV:TPC_OUT_SEQ0     =0x%8x")
TRACE_MSG(TPC_PHY_V5___123_112_2_17_23_7_31_1431," --->TPDRV:TPC_OE_SEQ0      =0x%8x")
TRACE_MSG(TPC_PHY_V5___124_112_2_17_23_7_31_1432," --->TPDRV:TPC_CH_SEQ       =0x%8x")
TRACE_MSG(TPC_PHY_V5___126_112_2_17_23_7_31_1433," --->TPDRV:REG_ANA_MISC_STS0=0x%8x")
TRACE_MSG(TPC_PHY_V5___127_112_2_17_23_7_31_1434," --->TPDRV:TPC_SAMPLE_CTRL0 =0x%8x")
TRACE_MSG(TPC_PHY_V5___128_112_2_17_23_7_31_1435," --->TPDRV:TPC_INT_EN =0x%8x")
TRACE_MSG(TPC_PHY_V5___129_112_2_17_23_7_31_1436," --->TPDRV:TPC_INT_RAW=0x%8x")
TRACE_MSG(TPC_PHY_V5___130_112_2_17_23_7_31_1437," --->TPDRV:TPC_CTRL   =0x%8x")
TRACE_MSG(TPC_PHY_V5___132_112_2_17_23_7_31_1438,"\r\n")
TRACE_MSG(TPC_PHY_V5___301_112_2_17_23_7_31_1439,"[TPDRV: TPDRV_Init]")
TRACE_MSG(TPC_PHY_V5___314_112_2_17_23_7_31_1440,"[TPDRV: TPDRV_EnableCoord]")
TRACE_MSG(TPC_PHY_V5___323_112_2_17_23_7_31_1441,"[TPDRV: TPDRV_DisableCoord]")
TRACE_MSG(TPC_PHY_V5___376_112_2_17_23_7_31_1442,"[TPDRV:TPDRV_ConfigCoord]==%d")
TRACE_MSG(TPC_PHY_V5___468_112_2_17_23_7_31_1443,"[TPDRV:_HandleTPCHisr ] pressed_time is not enough.")
TRACE_MSG(TPC_PHY_V5___529_112_2_17_23_7_31_1444,"[TPDRV:TPDRV_Run ]")
TRACE_MSG(TPC_PHY_V5___543_112_2_17_23_7_31_1445,"[TPDRV:TPDRV_Stop]")
TRACE_MSG(TPC_PHY_V5___625_112_2_17_23_7_32_1446,"TPDRV:_FetchData x=%d y=%d ")
TRACE_MSG(GX_UDC_1066_112_2_17_23_7_45_1447,"\ngx_udc_free_request() queue not empty. can not release\n")
TRACE_MSG(GX_UDC_1394_112_2_17_23_7_45_1448,"\nusb_gadget_register_driver(): bind gadget failed\n")
TRACE_MSG(GX_UDC_1419_112_2_17_23_7_45_1449,"\nusb_gadget_unregister_driver(): exit\n")
TRACE_MSG(SX_UDC_617_112_2_17_23_7_47_1450,"ep_no(%d) is invalid !!!\n")
TRACE_MSG(USB20_DRV_203_112_2_17_23_7_54_1451,"epno_out %d MSP =0, reconfigure it now\n")
TRACE_MSG(USB20_DRV_231_112_2_17_23_7_54_1452,"epno_in %d MSP =0, reconfigure it now\n")
TRACE_MSG(USB20_DRV_947_112_2_17_23_7_56_1453,"USB_EPxReceiveData: ep_id %d  int_sts 0x%x\n")
TRACE_MSG(USB20_DRV_968_112_2_17_23_7_56_1454,"USB_EPxSendData: ep_id %d  int_sts 0x%x\n")
TRACE_MSG(USB20_DRV_995_112_2_17_23_7_56_1455,"ep_id %d  int_sts 0x%x count %d \n")
TRACE_MSG(USB_COMMON_120_112_2_17_23_7_57_1456,"USB_SetPlugInCount: count %d, is increase %d")
TRACE_MSG(USB_COMMON_138_112_2_17_23_7_57_1457,"USB_SetPlugOutCount: count %d, is increase %d")
TRACE_MSG(USB_COMMON_224_112_2_17_23_7_57_1458,"USB LDO disable!")
TRACE_MSG(USB_COMMON_240_112_2_17_23_7_57_1459,"USB send plugout signal to UPM!")
TRACE_MSG(USB_COMMON_328_112_2_17_23_7_57_1460,"usb_init+++++++++++++++++++++++++++++++++++++\r\n")
TRACE_MSG(USB_DRV_E_113_112_2_17_23_7_58_1461,"USB LDO enable  ! ")
TRACE_MSG(USB_DRV_E_125_112_2_17_23_7_58_1462,"USB LDO disable!")
TRACE_MSG(USB_DRV_E_137_112_2_17_23_7_58_1463,"USB send plugout signal to UPM!")
TRACE_MSG(USB_DRV_E_277_112_2_17_23_7_58_1464,"usb intoken ack timeout")
TRACE_MSG(USB_DRV_E_286_112_2_17_23_7_58_1465,"USB_REQ_CLASS error setup value:0x%08x,0x%08x")
TRACE_MSG(USB_DRV_E_295_112_2_17_23_7_58_1466,"USB_REQ_VENDOR error setup value:0x%08x,0x%08x")
TRACE_MSG(USB_DRV_E_301_112_2_17_23_7_58_1467,"bsd get device info error")
TRACE_MSG(USB_DRV_E_307_112_2_17_23_7_58_1468,"bsd open device fail")
TRACE_MSG(USB_DRV_E_313_112_2_17_23_7_58_1469,"bsd close device fail")
TRACE_MSG(USB_DRV_E_318_112_2_17_23_7_58_1470,"UPCC requese type error")
TRACE_MSG(USB_DRV_I_106_112_2_17_23_7_59_1471,"USB_SetPlugInCount: count %d, is increase %d")
TRACE_MSG(USB_DRV_I_124_112_2_17_23_7_59_1472,"USB_SetPlugOutCount: count %d, is increase %d")
TRACE_MSG(USB_DRV_I_134_112_2_17_23_7_59_1473,"USB_GetPlugInCount: %d")
TRACE_MSG(USB_DRV_I_144_112_2_17_23_7_59_1474,"USB_GetPlugOutCount: %d")
TRACE_MSG(USB_DRV_I_165_112_2_17_23_7_59_1475,"USB irq is 0x%x ")
TRACE_MSG(USB_DRV_I_180_112_2_17_23_7_59_1476,"USB_irq:USB send PLUG_IN signal to UPM ")
TRACE_MSG(USB_DRV_I_202_112_2_17_23_7_59_1477,"USB_irq:USB send PLUG_IN signal to UPM and service")
TRACE_MSG(USB_DRV_I_217_112_2_17_23_7_59_1478,"USB current UPM's service is disabled !")
TRACE_MSG(USB_DRV_I_239_112_2_17_23_7_59_1479,"USB current UPM's service is disabled !")
TRACE_MSG(USB_DRV_I_255_112_2_17_23_7_59_1480,"USB current UPM's service is disabled !")
TRACE_MSG(USB_DRV_I_271_112_2_17_23_7_59_1481,"USB current UPM's service is disabled !")
TRACE_MSG(USB_DRV_I_287_112_2_17_23_7_59_1482,"USB current UPM's service is disabled !")
TRACE_MSG(USB_DRV_I_303_112_2_17_23_7_59_1483,"USB current UPM's service is disabled !")
TRACE_MSG(USB_DRV_I_319_112_2_17_23_7_59_1484,"USB current UPM's service is disabled !")
TRACE_MSG(USB_DRV_I_450_112_2_17_23_8_0_1485,"USB : Disable all end-point DMA !!")
TRACE_MSG(VCOM_USB_151_112_2_17_23_8_1_1486,"USB LDO disable!")
TRACE_MSG(VCOM_USB20_302_112_2_17_23_8_4_1487,"Enter VCOM_usb_boot\n")
TRACE_MSG(WATCHDOG_PHY_V0_42_112_2_17_23_8_5_1488,"Watch Dog Trace: Watch Dog Value 0x%8.8x")
TRACE_MSG(WATCHDOG_PHY_V0_79_112_2_17_23_8_5_1489,"Watch Dog Trace: Watch Dog Control 0x%8.8x")
TRACE_MSG(WATCHDOG_PHY_V0_80_112_2_17_23_8_5_1490,"Watch Dog Trace: Watch Dog LOAD    0x%8.8x")
TRACE_MSG(WATCHDOG_PHY_V3_100_112_2_17_23_8_5_1491,"Watch Dog Trace: Watch Dog Control 0x%8.8x")
TRACE_MSG(WATCHDOG_PHY_V5_115_112_2_17_23_8_6_1492,"Watch Dog Trace: Watch Dog Control 0x%8.8x")
TRACE_MSG(WATCHDOG_HAL_44_112_2_17_23_8_6_1493,"Watch Dog Trace: WDG_TimerStop")
TRACE_MSG(WATCHDOG_HAL_70_112_2_17_23_8_6_1494,"Watch Dog Trace: WDG_TimerStart")
TRACE_MSG(WATCHDOG_HAL_101_112_2_17_23_8_6_1495,"Watch Dog Trace: WDG_TimerInit")
TRACE_MSG(WATCHDOG_HAL_121_112_2_17_23_8_6_1496,"Watch Dog Trace: WDG_ResetMCU")
TRACE_MSG(WATCHDOG_HAL_139_112_2_17_23_8_6_1497,"Watch Dog Trace: WDG_TimerLoad %d")
TRACE_MSG(AUDIO_WIN_125_112_2_17_23_8_21_1498,"AUD_Init: Initialize audio interface.")
TRACE_MSG(AUDIO_WIN_139_112_2_17_23_8_21_1499,"AUD_SetEngineeringParameter: parameter_type = %d")
TRACE_MSG(AUDIO_WIN_152_112_2_17_23_8_21_1500,"AUD_SwitchTestMode: is_test_mode = %d")
TRACE_MSG(CHARGE_349_112_2_17_23_8_22_1501,"battery Try send message(%d, %d) to Client")
TRACE_MSG(CHARGE_394_112_2_17_23_8_22_1502,"charge : has sent to client. (%d)")
TRACE_MSG(CHARGE_428_112_2_17_23_8_22_1503,"is_charge_connect: 0x%X")
TRACE_MSG(CHARGE_478_112_2_17_23_8_22_1504,"CHR : Has reach max level")
TRACE_MSG(CHARGE_486_112_2_17_23_8_22_1505,"CHR : need charge")
TRACE_MSG(CHARGE_491_112_2_17_23_8_22_1506,"CHR : needn't charge")
TRACE_MSG(CHARGE_508_112_2_17_23_8_22_1507,"chr_stm( %d, %d )")
TRACE_MSG(CHARGE_569_112_2_17_23_8_22_1508,"time slot is too long and need stop charge")
TRACE_MSG(CHARGE_582_112_2_17_23_8_22_1509,"CHR: time_slot : %d")
TRACE_MSG(CHARGE_657_112_2_17_23_8_22_1510,"The Batt ADC( %d/%d,  %d)")
TRACE_MSG(CHARGE_796_112_2_17_23_8_23_1511,"In charge_State and try check connect")
TRACE_MSG(CHARGE_800_112_2_17_23_8_23_1512,"charge not connect, and STM")
END_TRACE_MAP(CHIP_DRV_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _CHIP_DRV_TRC_H_

