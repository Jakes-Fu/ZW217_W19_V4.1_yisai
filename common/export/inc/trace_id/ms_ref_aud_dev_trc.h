/******************************************************************************
 ** File Name:      ms_ref_aud_dev_trc.h                                         *
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
//trace_id:33
#ifndef _MS_REF_AUD_DEV_TRC_H_
#define _MS_REF_AUD_DEV_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define ARMVB_AS_140_112_2_18_1_2_10_0 "armvb_as.c, ARMVB_DrvOutSwitch, alloc fail, size: %d"
#define ARMVB_AS_150_112_2_18_1_2_10_1 "armvb_as.c, ARMVB_DrvOutSwitch, ioctl fail, eMode: %d, is_on: %d "
#define ARMVB_AS_165_112_2_18_1_2_10_2 "armvb_as.c, ARMVB_DrvInSwitch, alloc fail, size: %d"
#define ARMVB_AS_175_112_2_18_1_2_10_3 "armvb_as.c, ARMVB_DrvInSwitch, ioctl fail, eMode: %d, is_on: %d "
#define ARMVB_AS_207_112_2_18_1_2_10_4 "armvb_as.c, ARMVB_SetPgaLevel, alloc fail, size: %d"
#define ARMVB_AS_217_112_2_18_1_2_10_5 "armvb_as.c, ARMVB_SetPgaLevel, ioctl fail, pga_level: %d eCodecMode: %d "
#define ARMVB_AS_231_112_2_18_1_2_10_6 "armvb_as.c, ARMVB_GetSupportSamplerate, ioctl fail, uiSampleRate: %d "
#define ARMVB_AS_431_112_2_18_1_2_11_7 "VB_SetMute : %d"
#define ARMVB_AS_436_112_2_18_1_2_11_8 "armvb_as.c, ARMVB_SetDownlinkMute, ioctl fail, is_mute: %d "
#define ARMVB_AS_461_112_2_18_1_2_11_9 "armvb_as.c ARMVB_SetVolumeType mode:%d."
#define ARMVB_AS_467_112_2_18_1_2_11_10 "armvb_as.c, ARMVB_SetVolumeType, alloc fail, size: %d"
#define ARMVB_AS_471_112_2_18_1_2_11_11 "[ARMVB_SetVolumeType] uiVol=%d, mode:%d, appType:%d."
#define ARMVB_AS_476_112_2_18_1_2_11_12 "armvb_as.c ARMVB_SetVolumeType failed to get mode param."
#define ARMVB_AS_482_112_2_18_1_2_11_13 "[ARMVB_SetVolumeType] armVolume = 0x%x"
#define ARMVB_AS_500_112_2_18_1_2_11_14 "[ARMVB_SetVolumeType] pga gain= 0x%4x"
#define ARMVB_AS_522_112_2_18_1_2_11_15 "armvb_as.c, ARMVB_SetInnerPA, alloc fail, size: %d"
#define ARMVB_AS_535_112_2_18_1_2_11_16 "armvb_as.c, ARMVB_SetInnerPA, ioctl fail, pa_l_setting: 0x%x  "
#define ARMVB_AS_568_112_2_18_1_2_11_17 "armvb_as.c,[ARMVB_SetDevMode] GetVolumeAndType failed.error:%d."
#define ARMVB_AS_575_112_2_18_1_2_11_18 "armvb_as.c ARMVB_SetDevMode mode:%d."
#define ARMVB_AS_582_112_2_18_1_2_11_19 "armvb_as.c ARMVB_SetDevMode failed to get dev ctr info, result:%d."
#define ARMVB_AS_597_112_2_18_1_2_11_20 "ARMVB_SetDevMode : unknown ucDAChanne!"
#define ARMVB_AS_614_112_2_18_1_2_11_21 "ARMVB_SetDevMode : unknown ucStereoEn!"
#define ARMVB_AS_622_112_2_18_1_2_11_22 "armvb_as.c, ARMVB_SetDevMode, alloc fail, size: %d"
#define ARMVB_AS_629_112_2_18_1_2_11_23 "armvb_as.c ARMVB_SetDevMode failed to get mode param."
#define ARMVB_AS_668_112_2_18_1_2_11_24 "armvb_as.c,[ARMVB_GetAppropriateOutputSamplerate]:uiSampleRate:%d"
#define ARMVB_AS_696_112_2_18_1_2_11_25 "armvb_as.c ARMVB_Play mode:%d."
#define ARMVB_AS_702_112_2_18_1_2_11_26 "armvb_as.c ARMVB_Play failed to get dev ctr info, result:%d."
#define ARMVB_AS_727_112_2_18_1_2_11_27 "armvb_as.c,ARMVB_Play alloc fail, no memory."
#define ARMVB_AS_737_112_2_18_1_2_11_28 "armvb_as.c,ARMVB_Play AUDIO_HAL_Open failed."
#define ARMVB_AS_743_112_2_18_1_2_11_29 "armvb_as.c ARMVB_Play uiSampleRate:%d ,mix-buffer size:%d (half-word) "
#define ARMVB_AS_761_112_2_18_1_2_11_30 "armvb_as.c ARMVB_Play has been opened, opencount:%d."
#define ARMVB_AS_787_112_2_18_1_2_12_31 "armvb_as.c ARMVB_Stop mode:%d."
#define ARMVB_AS_794_112_2_18_1_2_12_32 "armvb_as.c ARMVB_Stop failed to get dev ctr info, result:%d."
#define ARMVB_AS_809_112_2_18_1_2_12_33 "armvb_as.c ARMVB_Stop OK!s_is_vb_stop:%d "
#define ARMVB_AS_814_112_2_18_1_2_12_34 "ARMVB_Stop more opened:%d."
#define ARMVB_AS_876_112_2_18_1_2_12_35 "set aud proc explugger!"
#define ARMVB_AS_962_112_2_18_1_2_12_36 "armvb_as.c ARMVB_RECORD_EnableAmplifier mode:%d."
#define ARMVB_AS_969_112_2_18_1_2_12_37 "armvb_as.c ARMVB_RECORD_EnableAmplifier failed to get dev ctr info, result:%d."
#define ARMVB_AS_1020_112_2_18_1_2_12_38 "armvb_as.c VB_MOD_SetVolume mode:%d."
#define ARMVB_AS_1024_112_2_18_1_2_12_39 "[VB_MOD_SetVolume] uiVol=%d, mode:%d, appType:%d."
#define ARMVB_AS_1029_112_2_18_1_2_12_40 "armvb_as.c, VB_MOD_SetVolume, alloc fail, size: %d"
#define ARMVB_AS_1036_112_2_18_1_2_12_41 "armvb_as.c VB_MOD_SetVolume failed to get mode param."
#define ARMVB_AS_1042_112_2_18_1_2_12_42 "VB_MOD_SetVolume 0 == uiVol"
#define ARMVB_AS_1052_112_2_18_1_2_12_43 "[VB_MOD_SetVolume] gain= 0x%4x"
#define ARMVB_AS_1080_112_2_18_1_2_12_44 "MTVVB_Play AUDDEV_Open failed."
#define ARMVB_AS_1095_112_2_18_1_2_12_45 "MTVVB_Play has been opend, opencount:%d."
#define ARMVB_AS_1113_112_2_18_1_2_12_46 "MTVVB_Stop more opened:%d."
#define ARMVB_AS_1129_112_2_18_1_2_12_47 "MTVVB_Pause more opened:%d."
#define ARMVB_AS_1147_112_2_18_1_2_12_48 "MTVVB_Resume AUDDEV_Open failed."
#define ARMVB_AS_1157_112_2_18_1_2_12_49 "MTVVB_Resume has been opened, opencount:%d."
#define ARMVB_AS_1240_112_2_18_1_2_12_50 "armvb_as.c ARMVB_Record_SetVolume mode:%d."
#define ARMVB_AS_1244_112_2_18_1_2_12_51 "[ARMVB_Record_SetVolume] uiVol=%d, mode:%d, appType:%d."
#define ARMVB_AS_1249_112_2_18_1_2_12_52 "armvb_as.c, ARMVB_Record_SetVolumeType, alloc fail, size: %d"
#define ARMVB_AS_1256_112_2_18_1_2_12_53 "armvb_as.c ARMVB_Record_SetVolume failed to get mode param."
#define ARMVB_AS_1264_112_2_18_1_2_12_54 "[ARMVB_Record_SetVolume] gain= 0x%4x"
#define ARMVB_AS_1295_112_2_18_1_2_13_55 "armvb_as.c,[ARMVB_SetDevMode] GetVolumeAndType failed.error:%d."
#define ARMVB_AS_1302_112_2_18_1_2_13_56 "armvb_as.c ARMVB_SetDevMode mode:%d."
#define ARMVB_AS_1309_112_2_18_1_2_13_57 "armvb_as.c ARMVB_SetDevMode failed to get dev ctr info, result:%d."
#define ARMVB_AS_1324_112_2_18_1_2_13_58 "ARMVB_Record_SetDevMode : unknown ucADChannel!"
#define ARMVB_AS_1339_112_2_18_1_2_13_59 "ARMVB_Record_SetDevMode : unknown ucDAChanne!"
#define ARMVB_AS_1353_112_2_18_1_2_13_60 "ARMVB_Record_SetDevMode : unknown ucStereoEn!"
#define ARMVB_AS_1513_112_2_18_1_2_13_61 "ARMVB_Record_Play AUDDEV_Open failed."
#define ARMVB_AS_1522_112_2_18_1_2_13_62 "ARMVB_Record_Play alloc fail, no memory."
#define ARMVB_AS_1620_112_2_18_1_2_13_63 "armvb_as.c ARMVB_Linein_SetMute mode:%d."
#define ARMVB_AS_1627_112_2_18_1_2_13_64 "armvb_as.c ARMVB_Linein_SetMute failed to get dev ctr info, result:%d."
#define ARMVB_AS_1631_112_2_18_1_2_13_65 "ARMVB_Linein_SetMute : %d"
#define ARMVB_AS_1654_112_2_18_1_2_13_66 "ARMVB_Linein_SetMute : unknown ucDAChanne!"
#define ARMVB_AS_1668_112_2_18_1_2_13_67 "ARMVB_Linein_SetMute : unknown ucStereoEn!"
#define ARMVB_AS_1676_112_2_18_1_2_13_68 "armvb_as.c, ARMVB_Linein_SetMute, alloc fail, size: %d"
#define ARMVB_AS_1682_112_2_18_1_2_13_69 "armvb_as.c failed to get mode param."
#define ARMVB_AS_1710_112_2_18_1_2_13_70 "ARMVB_Linein_SetVolume uiVol:%d, max:%d."
#define ARMVB_AS_1721_112_2_18_1_2_13_71 "armvb_as.c ARMVB_Linein_SetVolume mode:%d."
#define ARMVB_AS_1728_112_2_18_1_2_13_72 "armvb_as.c, ARMVB_Linein_SetVolume, alloc fail, size: %d"
#define ARMVB_AS_1735_112_2_18_1_2_13_73 "armvb_as.c ARMVB_Linein_SetVolume failed to get mode param."
#define ARMVB_AS_1783_112_2_18_1_2_14_74 "armvb_as.c,[ARMVB_SetDevMode] GetVolumeAndType failed.error:%d."
#define ARMVB_AS_1790_112_2_18_1_2_14_75 "armvb_as.c ARMVB_SetDevMode mode:%d."
#define ARMVB_AS_1797_112_2_18_1_2_14_76 "armvb_as.c ARMVB_SetDevMode failed to get dev ctr info, result:%d."
#define ARMVB_AS_1822_112_2_18_1_2_14_77 "ARMVB_SetDevMode : unknown ucDAChanne!"
#define ARMVB_AS_1836_112_2_18_1_2_14_78 "ARMVB_SetDevMode : unknown ucStereoEn!"
#define ARMVB_AS_1842_112_2_18_1_2_14_79 "armvb_as.c, ARMVB_Linein_SetDevMode, alloc fail, size: %d"
#define ARMVB_AS_1849_112_2_18_1_2_14_80 "armvb_as.c failed to get mode param."
#define ARMVB_AS_1875_112_2_18_1_2_14_81 "armvb_as.c ARMVB_Play mode:%d."
#define ARMVB_AS_1882_112_2_18_1_2_14_82 "armvb_as.c ARMVB_Linein_Play failed to get dev ctr info, result:%d."
#define ARMVB_AS_1897_112_2_18_1_2_14_83 "ARMVB_Linein_Play AUDDEV_Open failed."
#define ARMVB_AS_1908_112_2_18_1_2_14_84 "ARMVB_Linein_Play AUDDEV_Open failed."
#define ARMVB_AS_1938_112_2_18_1_2_14_85 "armvb_as.c ARMVB_Linein_Stop mode:%d."
#define ARMVB_AS_1945_112_2_18_1_2_14_86 "armvb_as.c ARMVB_Linein_Stop failed to get dev ctr info, result:%d."
#define ARMVB_AS_1964_112_2_18_1_2_14_87 "ARMVB_Linein_Stop failed."
#define ARMVB_AS_2022_112_2_18_1_2_14_88 "armvb_as.c ARMVB_LineinAd_SetVolume mode:%d."
#define ARMVB_AS_2026_112_2_18_1_2_14_89 "[ARMVB_LineinAd_SetVolume] uiVol=%d, mode:%d, appType:%d."
#define ARMVB_AS_2031_112_2_18_1_2_14_90 "armvb_as.c, ARMVB_LineinAd_SetVolume, alloc fail, size: %d"
#define ARMVB_AS_2038_112_2_18_1_2_14_91 "armvb_as.c ARMVB_LineinAd_SetVolume failed to get mode param."
#define ARMVB_AS_2044_112_2_18_1_2_14_92 "[ARMVB_LineinAd_SetVolume] gain= 0x%4x"
#define ARMVB_AS_2064_112_2_18_1_2_14_93 "ARMVB_LineinAd_SetMute : %d"
#define ARMVB_AS_2085_112_2_18_1_2_14_94 "armvb_as.c,[ARMVB_SetDevMode] GetVolumeAndType failed.error:%d."
#define ARMVB_AS_2092_112_2_18_1_2_14_95 "armvb_as.c ARMVB_LineinAd_SetDevMode mode:%d."
#define ARMVB_AS_2099_112_2_18_1_2_14_96 "armvb_as.c ARMVB_LineinAd_SetDevMode failed to get dev ctr info, result:%d."
#define ARMVB_AS_2114_112_2_18_1_2_14_97 "ARMVB_LineinAd_SetDevMode : unknown ucADChannel!"
#define ARMVB_AS_2132_112_2_18_1_2_14_98 "ARMVB_LineinAd_SetDevMode : unknown ucDAChanne!"
#define ARMVB_AS_2146_112_2_18_1_2_14_99 "ARMVB_LineinAd_SetDevMode : unknown ucStereoEn!"
#define ARMVB_AS_2163_112_2_18_1_2_14_100 "ARMVB_LineinAd_Play AUDDEV_Open failed."
#define ARMVB_AS_2170_112_2_18_1_2_14_101 "ARMVB_Record_Play alloc fail, no memory."
#define DSPVB_AS_75_112_2_18_1_2_15_102 "dspvb_as.c, DSPVB_DrvOutSwitch, alloc fail, size: %d"
#define DSPVB_AS_85_112_2_18_1_2_15_103 "dspvb_as.c, DSPVB_DrvOutSwitch, ioctl fail, eMode: %d, is_on: %d "
#define DSPVB_AS_100_112_2_18_1_2_15_104 "dspvb_as.c, DSPVB_DrvInSwitch, alloc fail, size: %d"
#define DSPVB_AS_110_112_2_18_1_2_15_105 "dspvb_as.c, DSPVB_DrvInSwitch, ioctl fail, eMode: %d, is_on: %d "
#define DSPVB_AS_125_112_2_18_1_2_15_106 "dspvb_as.c, DSPVB_SetInnerPA, alloc fail, size: %d"
#define DSPVB_AS_138_112_2_18_1_2_15_107 "dspvb_as.c, DSPVB_SetInnerPA, ioctl fail, pa_l_setting: 0x%x  "
#define DSPVB_AS_153_112_2_18_1_2_15_108 "dspvb_as.c, DSPVB_SetPgaLevel, alloc fail, size: %d"
#define DSPVB_AS_163_112_2_18_1_2_15_109 "dspvb_as.c, DSPVB_SetPgaLevel, ioctl fail, pga_level: %d eCodecMode: %d "
#define DSPVB_AS_210_112_2_18_1_2_15_110 "dspvb_as.c:DSPVB_Codec_SetMode:error:Mode:0x%x "
#define DSPVB_AS_299_112_2_18_1_2_15_111 "dspvb_as.c, DSPVB_Codec_SetVolume, alloc fail, size: %d"
#define DSPVB_AS_307_112_2_18_1_2_15_112 "dspvb_as.c DSPVB_Codec_SetVolume mode:%d."
#define DSPVB_AS_313_112_2_18_1_2_15_113 "dspvb_as.c DSPVB_Codec_SetVolume failed to get mode param."
#define DSPVB_AS_335_112_2_18_1_2_15_114 "DSPVB_Codec_SetVolume vol_cfg0, 0x%x, 0x%x, 0x%x, 0x%x."
#define DSPVB_AS_360_112_2_18_1_2_15_115 "DSPVB_Codec_SetVolume vol_cfg1, 0x%x, 0x%x, 0x%x, 0x%x."
#define DSPVB_AS_385_112_2_18_1_2_15_116 "_DSPVB_OpenDev AUDIO_HAL_Open failed."
#define DSPVB_AS_417_112_2_18_1_2_16_117 "[DSPVB_SetUplinkMute]: %d"
#define DSPVB_AS_428_112_2_18_1_2_16_118 "[DSPVB_SetVolume] uiVol= 0x%4x"
#define DSPVB_AS_446_112_2_18_1_2_16_119 "dspvb_as.c, DSPVB_GetSupportFs, ioctl fail, uiSampleRate: %d "
#define DSPVB_AS_470_112_2_18_1_2_16_120 "[LAYER1_SetDownlinkMute]: %d"
#define DSPVB_AS_475_112_2_18_1_2_16_121 "dspvb_as.c, LAYER1_SetDownlinkMute, ioctl fail, is_mute: %d "
#define DSPVB_AS_491_112_2_18_1_2_16_122 "[LAYER1_SetDownlinkLocalNetMute]: local:%d, net:%d."
#define DSPVB_AS_515_112_2_18_1_2_16_123 "[RECORD_SetDownlinkMute]: %d"
#define DSPVB_AS_541_112_2_18_1_2_16_124 "[MP4RECORD_SetDownlinkMute]: %d"
#define DSPVB_AS_568_112_2_18_1_2_16_125 "[LOOPBACK_SetDownlinkMute]: %d"
#define DSPVB_AS_573_112_2_18_1_2_16_126 "dspvb_as.c, LOOPBACK_SetDownlinkMute, ioctl fail, is_mute: %d "
#define DSPVB_AS_590_112_2_18_1_2_16_127 "dspvb_as.c, LAYER1_SetDevMode, alloc fail, size: %d"
#define DSPVB_AS_593_112_2_18_1_2_16_128 "LAYER1_SetDevMode eMode:%d."
#define DSPVB_AS_614_112_2_18_1_2_16_129 "LAYER1_SetDevMode s_pcm_on_count>0."
#define DSPVB_AS_629_112_2_18_1_2_16_130 "[LAYER1_SetDevMode]:Wrong mode = %d"
#define DSPVB_AS_663_112_2_18_1_2_16_131 "[LAYER1_SetDevMode]:Wrong mode = %d"
#define DSPVB_AS_683_112_2_18_1_2_16_132 "dspvb_as.c, RECORD_SetDevMode, alloc fail, size: %d"
#define DSPVB_AS_689_112_2_18_1_2_16_133 "[RECORD_SetDevMode]: enter  mode = %d"
#define DSPVB_AS_705_112_2_18_1_2_16_134 "[RECORD_SetDevMode]:Wrong mode = %d"
#define DSPVB_AS_720_112_2_18_1_2_16_135 "[RECORD_SetDevMode]: mode = %d"
#define DSPVB_AS_740_112_2_18_1_2_16_136 "dspvb_as.c, MP4RECORD_SetDevMode, alloc fail, size: %d"
#define DSPVB_AS_745_112_2_18_1_2_16_137 "[MP4RECORD_SetDevMode]: enter  mode = %d"
#define DSPVB_AS_762_112_2_18_1_2_16_138 "[MP4RECORD_SetDevMode]:Wrong mode = %d"
#define DSPVB_AS_777_112_2_18_1_2_16_139 "[MP4RECORD_SetDevMode]:Wrong mode = %d"
#define DSPVB_AS_795_112_2_18_1_2_16_140 "dspvb_as.c, LOOPBACK_SetDevMode, alloc fail, size: %d"
#define DSPVB_AS_819_112_2_18_1_2_16_141 "LOOPBACK_SetDevMode s_pcm_on_count>0."
#define DSPVB_AS_834_112_2_18_1_2_16_142 "[LOOPBACK_SetDevMode]:Wrong mode = %d"
#define DSPVB_AS_869_112_2_18_1_2_16_143 "[LOOPBACK_SetDevMode]:Wrong mode = %d"
#define DSPVB_AS_886_112_2_18_1_2_17_144 "[LAYER1_Play]:s_b_dspvb_playing:%d,s_b_layer1_playing:%d"
#define DSPVB_AS_897_112_2_18_1_2_17_145 "[LAYER1_Play] _DSPVB_OpenDev failed."
#define DSPVB_AS_911_112_2_18_1_2_17_146 "[LAYER1_Play]: dsp vb is already opened by other device!"
#define DSPVB_AS_934_112_2_18_1_2_17_147 "[LAYER1_Stop]: s_b_dspvb_playing:%d,s_b_layer1_playing:%d"
#define DSPVB_AS_951_112_2_18_1_2_17_148 "[LAYER1_Stop]: dspdata is using vb,so we should not close vb!"
#define DSPVB_AS_956_112_2_18_1_2_17_149 "[LAYER1_Stop]: stop exception!"
#define DSPVB_AS_984_112_2_18_1_2_17_150 "[LAYER1_Pause]: dspdata is using vb,so we should not close vb!"
#define DSPVB_AS_989_112_2_18_1_2_17_151 "[LAYER1_Pause]: pause exception!"
#define DSPVB_AS_1014_112_2_18_1_2_17_152 "[LAYER1_Resume] _DSPVB_OpenDev failed."
#define DSPVB_AS_1022_112_2_18_1_2_17_153 "[LAYER1_Resume]: dsp vb is already opened by other device!"
#define DSPVB_AS_1048_112_2_18_1_2_17_154 "[RECORD_Play] _DSPVB_OpenDev failed."
#define DSPVB_AS_1062_112_2_18_1_2_17_155 "dspvb_as.c:RECORD_Play: dsp is playing"
#define DSPVB_AS_1085_112_2_18_1_2_17_156 "dspvb_as.c:RECORD_Stop: dsp is playing"
#define DSPVB_AS_1107_112_2_18_1_2_17_157 "dspvb_as.c:RECORD_Pause: dsp is playing"
#define DSPVB_AS_1126_112_2_18_1_2_17_158 "[RECORD_Resume] _DSPVB_OpenDev failed."
#define DSPVB_AS_1141_112_2_18_1_2_17_159 "dspvb_as.c:RECORD_Resume: dsp is playing"
#define DSPVB_AS_1159_112_2_18_1_2_17_160 "[MP4RECORD_Play] _DSPVB_OpenDev failed."
#define DSPVB_AS_1175_112_2_18_1_2_17_161 "dspvb_as.c:MP4RECORD_Play: dsp is playing"
#define DSPVB_AS_1200_112_2_18_1_2_17_162 "dspvb_as.c:MP4RECORD_Stop: dsp is playing"
#define DSPVB_AS_1225_112_2_18_1_2_17_163 "dspvb_as.c:MP4RECORD_Pause: dsp is playing"
#define DSPVB_AS_1246_112_2_18_1_2_17_164 "[MP4RECORD_Resume] _DSPVB_OpenDev failed."
#define DSPVB_AS_1261_112_2_18_1_2_17_165 "dspvb_as.c:MP4RECORD_Resume: dsp is playing"
#define DSPVB_AS_1274_112_2_18_1_2_17_166 "[LOOPBACK_Play]:s_b_dspvb_playing:%d,s_b_layer1_playing:%d"
#define DSPVB_AS_1282_112_2_18_1_2_17_167 "[LOOPBACK_Play] _DSPVB_OpenDev failed."
#define DSPVB_AS_1296_112_2_18_1_2_17_168 "[LOOPBACK_Play]: dsp vb is already opened by other device!"
#define DSPVB_AS_1319_112_2_18_1_2_17_169 "[LOOPBACK_Stop]: s_b_dspvb_playing:%d,s_b_layer1_playing:%d"
#define DSPVB_AS_1331_112_2_18_1_2_17_170 "[LOOPBACK_Stop]: dspdata is using vb,so we should not close vb!"
#define DSPVB_AS_1336_112_2_18_1_2_17_171 "[LOOPBACK_Stop]: stop exception!"
#define DSPVB_AS_1490_112_2_18_1_2_18_172 "[DSPDATA_Play]:s_b_dspvb_playing:%d,s_b_dspdata_playing:%d"
#define DSPVB_AS_1498_112_2_18_1_2_18_173 "[DSPDATA_Play] _DSPVB_OpenDev failed."
#define DSPVB_AS_1509_112_2_18_1_2_18_174 "[DSPDATA_Play]: dsp vb is already opened by other device!"
#define DSPVB_AS_1524_112_2_18_1_2_18_175 "[DSPDATA_Stop]:s_b_dspvb_playing:%d,s_b_dspdata_playing:%d"
#define DSPVB_AS_1536_112_2_18_1_2_18_176 "[DSPDATA_Stop]: layer1 is using vb,so we should not close vb!"
#define DSPVB_AS_1541_112_2_18_1_2_18_177 "[DSPDATA_Stop]: stop exception!"
#define DSPVB_AS_1564_112_2_18_1_2_18_178 "[DSPDATA_Pause]: layer1 is using vb,so we should not close vb!"
#define DSPVB_AS_1569_112_2_18_1_2_18_179 "[DSPDATA_Pause]: pause exception!"
#define DSPVB_AS_1588_112_2_18_1_2_18_180 "[DSPDATA_Resume] _DSPVB_OpenDev failed."
#define DSPVB_AS_1600_112_2_18_1_2_18_181 "[DSPDATA_Play]: dsp vb is already opened by other device!"
#define PCM_DEV_255_112_2_18_1_2_19_182 "_PcmDev_EnableDigitalGain add express:0x%x to codec:0x%x."
#define PCM_DEV_269_112_2_18_1_2_19_183 "_PcmDev_EnableDigitalGain delete express:0x%x from codec:0x%x."
#define PCM_DEV_294_112_2_18_1_2_19_184 "PcmDev_SetVolumeType vol:%d, appType:%d."
#define PCM_DEV_299_112_2_18_1_2_19_185 "PcmDev_SetVolumeType volume level is too bit(%d), will be adjusted to be:%d."
#define PCM_DEV_306_112_2_18_1_2_19_186 "PcmDev_SetVolumeType filed to get modeName:%d."
#define PCM_DEV_312_112_2_18_1_2_19_187 "PcmDev_SetVolumeType, alloc fail, size: %d"
#define PCM_DEV_319_112_2_18_1_2_19_188 "PcmDev_SetVolumeType failed to get mode param, mode:%d, modeName:%s."
#define PCM_DEV_325_112_2_18_1_2_19_189 "PcmDev_SetVolumeType uiVol=%d, mode:%d, appType:%d, modeName:%s."
#define PCM_DEV_338_112_2_18_1_2_19_190 "PcmDev_SetVolumeType gain:0x%4x, dg_level= 0x%4x"
#define PCM_DEV_359_112_2_18_1_2_20_191 "PcmDev_SetDownlinkMute, mute:%d, vol:%d, appType:%d."
#define PCM_DEV_535_112_2_18_1_2_20_192 "====PCM_Open====="
#define PCM_DEV_537_112_2_18_1_2_20_193 "[PCMDEV_Play] IN (s_pcm_on_count = %d)"
#define PCM_DEV_709_112_2_18_1_2_20_194 "PCMDEV_SetDevMode:%d."
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_AUD_DEV_TRC)
TRACE_MSG(ARMVB_AS_140_112_2_18_1_2_10_0,"armvb_as.c, ARMVB_DrvOutSwitch, alloc fail, size: %d")
TRACE_MSG(ARMVB_AS_150_112_2_18_1_2_10_1,"armvb_as.c, ARMVB_DrvOutSwitch, ioctl fail, eMode: %d, is_on: %d ")
TRACE_MSG(ARMVB_AS_165_112_2_18_1_2_10_2,"armvb_as.c, ARMVB_DrvInSwitch, alloc fail, size: %d")
TRACE_MSG(ARMVB_AS_175_112_2_18_1_2_10_3,"armvb_as.c, ARMVB_DrvInSwitch, ioctl fail, eMode: %d, is_on: %d ")
TRACE_MSG(ARMVB_AS_207_112_2_18_1_2_10_4,"armvb_as.c, ARMVB_SetPgaLevel, alloc fail, size: %d")
TRACE_MSG(ARMVB_AS_217_112_2_18_1_2_10_5,"armvb_as.c, ARMVB_SetPgaLevel, ioctl fail, pga_level: %d eCodecMode: %d ")
TRACE_MSG(ARMVB_AS_231_112_2_18_1_2_10_6,"armvb_as.c, ARMVB_GetSupportSamplerate, ioctl fail, uiSampleRate: %d ")
TRACE_MSG(ARMVB_AS_431_112_2_18_1_2_11_7,"VB_SetMute : %d")
TRACE_MSG(ARMVB_AS_436_112_2_18_1_2_11_8,"armvb_as.c, ARMVB_SetDownlinkMute, ioctl fail, is_mute: %d ")
TRACE_MSG(ARMVB_AS_461_112_2_18_1_2_11_9,"armvb_as.c ARMVB_SetVolumeType mode:%d.")
TRACE_MSG(ARMVB_AS_467_112_2_18_1_2_11_10,"armvb_as.c, ARMVB_SetVolumeType, alloc fail, size: %d")
TRACE_MSG(ARMVB_AS_471_112_2_18_1_2_11_11,"[ARMVB_SetVolumeType] uiVol=%d, mode:%d, appType:%d.")
TRACE_MSG(ARMVB_AS_476_112_2_18_1_2_11_12,"armvb_as.c ARMVB_SetVolumeType failed to get mode param.")
TRACE_MSG(ARMVB_AS_482_112_2_18_1_2_11_13,"[ARMVB_SetVolumeType] armVolume = 0x%x")
TRACE_MSG(ARMVB_AS_500_112_2_18_1_2_11_14,"[ARMVB_SetVolumeType] pga gain= 0x%4x")
TRACE_MSG(ARMVB_AS_522_112_2_18_1_2_11_15,"armvb_as.c, ARMVB_SetInnerPA, alloc fail, size: %d")
TRACE_MSG(ARMVB_AS_535_112_2_18_1_2_11_16,"armvb_as.c, ARMVB_SetInnerPA, ioctl fail, pa_l_setting: 0x%x  ")
TRACE_MSG(ARMVB_AS_568_112_2_18_1_2_11_17,"armvb_as.c,[ARMVB_SetDevMode] GetVolumeAndType failed.error:%d.")
TRACE_MSG(ARMVB_AS_575_112_2_18_1_2_11_18,"armvb_as.c ARMVB_SetDevMode mode:%d.")
TRACE_MSG(ARMVB_AS_582_112_2_18_1_2_11_19,"armvb_as.c ARMVB_SetDevMode failed to get dev ctr info, result:%d.")
TRACE_MSG(ARMVB_AS_597_112_2_18_1_2_11_20,"ARMVB_SetDevMode : unknown ucDAChanne!")
TRACE_MSG(ARMVB_AS_614_112_2_18_1_2_11_21,"ARMVB_SetDevMode : unknown ucStereoEn!")
TRACE_MSG(ARMVB_AS_622_112_2_18_1_2_11_22,"armvb_as.c, ARMVB_SetDevMode, alloc fail, size: %d")
TRACE_MSG(ARMVB_AS_629_112_2_18_1_2_11_23,"armvb_as.c ARMVB_SetDevMode failed to get mode param.")
TRACE_MSG(ARMVB_AS_668_112_2_18_1_2_11_24,"armvb_as.c,[ARMVB_GetAppropriateOutputSamplerate]:uiSampleRate:%d")
TRACE_MSG(ARMVB_AS_696_112_2_18_1_2_11_25,"armvb_as.c ARMVB_Play mode:%d.")
TRACE_MSG(ARMVB_AS_702_112_2_18_1_2_11_26,"armvb_as.c ARMVB_Play failed to get dev ctr info, result:%d.")
TRACE_MSG(ARMVB_AS_727_112_2_18_1_2_11_27,"armvb_as.c,ARMVB_Play alloc fail, no memory.")
TRACE_MSG(ARMVB_AS_737_112_2_18_1_2_11_28,"armvb_as.c,ARMVB_Play AUDIO_HAL_Open failed.")
TRACE_MSG(ARMVB_AS_743_112_2_18_1_2_11_29,"armvb_as.c ARMVB_Play uiSampleRate:%d ,mix-buffer size:%d (half-word) ")
TRACE_MSG(ARMVB_AS_761_112_2_18_1_2_11_30,"armvb_as.c ARMVB_Play has been opened, opencount:%d.")
TRACE_MSG(ARMVB_AS_787_112_2_18_1_2_12_31,"armvb_as.c ARMVB_Stop mode:%d.")
TRACE_MSG(ARMVB_AS_794_112_2_18_1_2_12_32,"armvb_as.c ARMVB_Stop failed to get dev ctr info, result:%d.")
TRACE_MSG(ARMVB_AS_809_112_2_18_1_2_12_33,"armvb_as.c ARMVB_Stop OK!s_is_vb_stop:%d ")
TRACE_MSG(ARMVB_AS_814_112_2_18_1_2_12_34,"ARMVB_Stop more opened:%d.")
TRACE_MSG(ARMVB_AS_876_112_2_18_1_2_12_35,"set aud proc explugger!")
TRACE_MSG(ARMVB_AS_962_112_2_18_1_2_12_36,"armvb_as.c ARMVB_RECORD_EnableAmplifier mode:%d.")
TRACE_MSG(ARMVB_AS_969_112_2_18_1_2_12_37,"armvb_as.c ARMVB_RECORD_EnableAmplifier failed to get dev ctr info, result:%d.")
TRACE_MSG(ARMVB_AS_1020_112_2_18_1_2_12_38,"armvb_as.c VB_MOD_SetVolume mode:%d.")
TRACE_MSG(ARMVB_AS_1024_112_2_18_1_2_12_39,"[VB_MOD_SetVolume] uiVol=%d, mode:%d, appType:%d.")
TRACE_MSG(ARMVB_AS_1029_112_2_18_1_2_12_40,"armvb_as.c, VB_MOD_SetVolume, alloc fail, size: %d")
TRACE_MSG(ARMVB_AS_1036_112_2_18_1_2_12_41,"armvb_as.c VB_MOD_SetVolume failed to get mode param.")
TRACE_MSG(ARMVB_AS_1042_112_2_18_1_2_12_42,"VB_MOD_SetVolume 0 == uiVol")
TRACE_MSG(ARMVB_AS_1052_112_2_18_1_2_12_43,"[VB_MOD_SetVolume] gain= 0x%4x")
TRACE_MSG(ARMVB_AS_1080_112_2_18_1_2_12_44,"MTVVB_Play AUDDEV_Open failed.")
TRACE_MSG(ARMVB_AS_1095_112_2_18_1_2_12_45,"MTVVB_Play has been opend, opencount:%d.")
TRACE_MSG(ARMVB_AS_1113_112_2_18_1_2_12_46,"MTVVB_Stop more opened:%d.")
TRACE_MSG(ARMVB_AS_1129_112_2_18_1_2_12_47,"MTVVB_Pause more opened:%d.")
TRACE_MSG(ARMVB_AS_1147_112_2_18_1_2_12_48,"MTVVB_Resume AUDDEV_Open failed.")
TRACE_MSG(ARMVB_AS_1157_112_2_18_1_2_12_49,"MTVVB_Resume has been opened, opencount:%d.")
TRACE_MSG(ARMVB_AS_1240_112_2_18_1_2_12_50,"armvb_as.c ARMVB_Record_SetVolume mode:%d.")
TRACE_MSG(ARMVB_AS_1244_112_2_18_1_2_12_51,"[ARMVB_Record_SetVolume] uiVol=%d, mode:%d, appType:%d.")
TRACE_MSG(ARMVB_AS_1249_112_2_18_1_2_12_52,"armvb_as.c, ARMVB_Record_SetVolumeType, alloc fail, size: %d")
TRACE_MSG(ARMVB_AS_1256_112_2_18_1_2_12_53,"armvb_as.c ARMVB_Record_SetVolume failed to get mode param.")
TRACE_MSG(ARMVB_AS_1264_112_2_18_1_2_12_54,"[ARMVB_Record_SetVolume] gain= 0x%4x")
TRACE_MSG(ARMVB_AS_1295_112_2_18_1_2_13_55,"armvb_as.c,[ARMVB_SetDevMode] GetVolumeAndType failed.error:%d.")
TRACE_MSG(ARMVB_AS_1302_112_2_18_1_2_13_56,"armvb_as.c ARMVB_SetDevMode mode:%d.")
TRACE_MSG(ARMVB_AS_1309_112_2_18_1_2_13_57,"armvb_as.c ARMVB_SetDevMode failed to get dev ctr info, result:%d.")
TRACE_MSG(ARMVB_AS_1324_112_2_18_1_2_13_58,"ARMVB_Record_SetDevMode : unknown ucADChannel!")
TRACE_MSG(ARMVB_AS_1339_112_2_18_1_2_13_59,"ARMVB_Record_SetDevMode : unknown ucDAChanne!")
TRACE_MSG(ARMVB_AS_1353_112_2_18_1_2_13_60,"ARMVB_Record_SetDevMode : unknown ucStereoEn!")
TRACE_MSG(ARMVB_AS_1513_112_2_18_1_2_13_61,"ARMVB_Record_Play AUDDEV_Open failed.")
TRACE_MSG(ARMVB_AS_1522_112_2_18_1_2_13_62,"ARMVB_Record_Play alloc fail, no memory.")
TRACE_MSG(ARMVB_AS_1620_112_2_18_1_2_13_63,"armvb_as.c ARMVB_Linein_SetMute mode:%d.")
TRACE_MSG(ARMVB_AS_1627_112_2_18_1_2_13_64,"armvb_as.c ARMVB_Linein_SetMute failed to get dev ctr info, result:%d.")
TRACE_MSG(ARMVB_AS_1631_112_2_18_1_2_13_65,"ARMVB_Linein_SetMute : %d")
TRACE_MSG(ARMVB_AS_1654_112_2_18_1_2_13_66,"ARMVB_Linein_SetMute : unknown ucDAChanne!")
TRACE_MSG(ARMVB_AS_1668_112_2_18_1_2_13_67,"ARMVB_Linein_SetMute : unknown ucStereoEn!")
TRACE_MSG(ARMVB_AS_1676_112_2_18_1_2_13_68,"armvb_as.c, ARMVB_Linein_SetMute, alloc fail, size: %d")
TRACE_MSG(ARMVB_AS_1682_112_2_18_1_2_13_69,"armvb_as.c failed to get mode param.")
TRACE_MSG(ARMVB_AS_1710_112_2_18_1_2_13_70,"ARMVB_Linein_SetVolume uiVol:%d, max:%d.")
TRACE_MSG(ARMVB_AS_1721_112_2_18_1_2_13_71,"armvb_as.c ARMVB_Linein_SetVolume mode:%d.")
TRACE_MSG(ARMVB_AS_1728_112_2_18_1_2_13_72,"armvb_as.c, ARMVB_Linein_SetVolume, alloc fail, size: %d")
TRACE_MSG(ARMVB_AS_1735_112_2_18_1_2_13_73,"armvb_as.c ARMVB_Linein_SetVolume failed to get mode param.")
TRACE_MSG(ARMVB_AS_1783_112_2_18_1_2_14_74,"armvb_as.c,[ARMVB_SetDevMode] GetVolumeAndType failed.error:%d.")
TRACE_MSG(ARMVB_AS_1790_112_2_18_1_2_14_75,"armvb_as.c ARMVB_SetDevMode mode:%d.")
TRACE_MSG(ARMVB_AS_1797_112_2_18_1_2_14_76,"armvb_as.c ARMVB_SetDevMode failed to get dev ctr info, result:%d.")
TRACE_MSG(ARMVB_AS_1822_112_2_18_1_2_14_77,"ARMVB_SetDevMode : unknown ucDAChanne!")
TRACE_MSG(ARMVB_AS_1836_112_2_18_1_2_14_78,"ARMVB_SetDevMode : unknown ucStereoEn!")
TRACE_MSG(ARMVB_AS_1842_112_2_18_1_2_14_79,"armvb_as.c, ARMVB_Linein_SetDevMode, alloc fail, size: %d")
TRACE_MSG(ARMVB_AS_1849_112_2_18_1_2_14_80,"armvb_as.c failed to get mode param.")
TRACE_MSG(ARMVB_AS_1875_112_2_18_1_2_14_81,"armvb_as.c ARMVB_Play mode:%d.")
TRACE_MSG(ARMVB_AS_1882_112_2_18_1_2_14_82,"armvb_as.c ARMVB_Linein_Play failed to get dev ctr info, result:%d.")
TRACE_MSG(ARMVB_AS_1897_112_2_18_1_2_14_83,"ARMVB_Linein_Play AUDDEV_Open failed.")
TRACE_MSG(ARMVB_AS_1908_112_2_18_1_2_14_84,"ARMVB_Linein_Play AUDDEV_Open failed.")
TRACE_MSG(ARMVB_AS_1938_112_2_18_1_2_14_85,"armvb_as.c ARMVB_Linein_Stop mode:%d.")
TRACE_MSG(ARMVB_AS_1945_112_2_18_1_2_14_86,"armvb_as.c ARMVB_Linein_Stop failed to get dev ctr info, result:%d.")
TRACE_MSG(ARMVB_AS_1964_112_2_18_1_2_14_87,"ARMVB_Linein_Stop failed.")
TRACE_MSG(ARMVB_AS_2022_112_2_18_1_2_14_88,"armvb_as.c ARMVB_LineinAd_SetVolume mode:%d.")
TRACE_MSG(ARMVB_AS_2026_112_2_18_1_2_14_89,"[ARMVB_LineinAd_SetVolume] uiVol=%d, mode:%d, appType:%d.")
TRACE_MSG(ARMVB_AS_2031_112_2_18_1_2_14_90,"armvb_as.c, ARMVB_LineinAd_SetVolume, alloc fail, size: %d")
TRACE_MSG(ARMVB_AS_2038_112_2_18_1_2_14_91,"armvb_as.c ARMVB_LineinAd_SetVolume failed to get mode param.")
TRACE_MSG(ARMVB_AS_2044_112_2_18_1_2_14_92,"[ARMVB_LineinAd_SetVolume] gain= 0x%4x")
TRACE_MSG(ARMVB_AS_2064_112_2_18_1_2_14_93,"ARMVB_LineinAd_SetMute : %d")
TRACE_MSG(ARMVB_AS_2085_112_2_18_1_2_14_94,"armvb_as.c,[ARMVB_SetDevMode] GetVolumeAndType failed.error:%d.")
TRACE_MSG(ARMVB_AS_2092_112_2_18_1_2_14_95,"armvb_as.c ARMVB_LineinAd_SetDevMode mode:%d.")
TRACE_MSG(ARMVB_AS_2099_112_2_18_1_2_14_96,"armvb_as.c ARMVB_LineinAd_SetDevMode failed to get dev ctr info, result:%d.")
TRACE_MSG(ARMVB_AS_2114_112_2_18_1_2_14_97,"ARMVB_LineinAd_SetDevMode : unknown ucADChannel!")
TRACE_MSG(ARMVB_AS_2132_112_2_18_1_2_14_98,"ARMVB_LineinAd_SetDevMode : unknown ucDAChanne!")
TRACE_MSG(ARMVB_AS_2146_112_2_18_1_2_14_99,"ARMVB_LineinAd_SetDevMode : unknown ucStereoEn!")
TRACE_MSG(ARMVB_AS_2163_112_2_18_1_2_14_100,"ARMVB_LineinAd_Play AUDDEV_Open failed.")
TRACE_MSG(ARMVB_AS_2170_112_2_18_1_2_14_101,"ARMVB_Record_Play alloc fail, no memory.")
TRACE_MSG(DSPVB_AS_75_112_2_18_1_2_15_102,"dspvb_as.c, DSPVB_DrvOutSwitch, alloc fail, size: %d")
TRACE_MSG(DSPVB_AS_85_112_2_18_1_2_15_103,"dspvb_as.c, DSPVB_DrvOutSwitch, ioctl fail, eMode: %d, is_on: %d ")
TRACE_MSG(DSPVB_AS_100_112_2_18_1_2_15_104,"dspvb_as.c, DSPVB_DrvInSwitch, alloc fail, size: %d")
TRACE_MSG(DSPVB_AS_110_112_2_18_1_2_15_105,"dspvb_as.c, DSPVB_DrvInSwitch, ioctl fail, eMode: %d, is_on: %d ")
TRACE_MSG(DSPVB_AS_125_112_2_18_1_2_15_106,"dspvb_as.c, DSPVB_SetInnerPA, alloc fail, size: %d")
TRACE_MSG(DSPVB_AS_138_112_2_18_1_2_15_107,"dspvb_as.c, DSPVB_SetInnerPA, ioctl fail, pa_l_setting: 0x%x  ")
TRACE_MSG(DSPVB_AS_153_112_2_18_1_2_15_108,"dspvb_as.c, DSPVB_SetPgaLevel, alloc fail, size: %d")
TRACE_MSG(DSPVB_AS_163_112_2_18_1_2_15_109,"dspvb_as.c, DSPVB_SetPgaLevel, ioctl fail, pga_level: %d eCodecMode: %d ")
TRACE_MSG(DSPVB_AS_210_112_2_18_1_2_15_110,"dspvb_as.c:DSPVB_Codec_SetMode:error:Mode:0x%x ")
TRACE_MSG(DSPVB_AS_299_112_2_18_1_2_15_111,"dspvb_as.c, DSPVB_Codec_SetVolume, alloc fail, size: %d")
TRACE_MSG(DSPVB_AS_307_112_2_18_1_2_15_112,"dspvb_as.c DSPVB_Codec_SetVolume mode:%d.")
TRACE_MSG(DSPVB_AS_313_112_2_18_1_2_15_113,"dspvb_as.c DSPVB_Codec_SetVolume failed to get mode param.")
TRACE_MSG(DSPVB_AS_335_112_2_18_1_2_15_114,"DSPVB_Codec_SetVolume vol_cfg0, 0x%x, 0x%x, 0x%x, 0x%x.")
TRACE_MSG(DSPVB_AS_360_112_2_18_1_2_15_115,"DSPVB_Codec_SetVolume vol_cfg1, 0x%x, 0x%x, 0x%x, 0x%x.")
TRACE_MSG(DSPVB_AS_385_112_2_18_1_2_15_116,"_DSPVB_OpenDev AUDIO_HAL_Open failed.")
TRACE_MSG(DSPVB_AS_417_112_2_18_1_2_16_117,"[DSPVB_SetUplinkMute]: %d")
TRACE_MSG(DSPVB_AS_428_112_2_18_1_2_16_118,"[DSPVB_SetVolume] uiVol= 0x%4x")
TRACE_MSG(DSPVB_AS_446_112_2_18_1_2_16_119,"dspvb_as.c, DSPVB_GetSupportFs, ioctl fail, uiSampleRate: %d ")
TRACE_MSG(DSPVB_AS_470_112_2_18_1_2_16_120,"[LAYER1_SetDownlinkMute]: %d")
TRACE_MSG(DSPVB_AS_475_112_2_18_1_2_16_121,"dspvb_as.c, LAYER1_SetDownlinkMute, ioctl fail, is_mute: %d ")
TRACE_MSG(DSPVB_AS_491_112_2_18_1_2_16_122,"[LAYER1_SetDownlinkLocalNetMute]: local:%d, net:%d.")
TRACE_MSG(DSPVB_AS_515_112_2_18_1_2_16_123,"[RECORD_SetDownlinkMute]: %d")
TRACE_MSG(DSPVB_AS_541_112_2_18_1_2_16_124,"[MP4RECORD_SetDownlinkMute]: %d")
TRACE_MSG(DSPVB_AS_568_112_2_18_1_2_16_125,"[LOOPBACK_SetDownlinkMute]: %d")
TRACE_MSG(DSPVB_AS_573_112_2_18_1_2_16_126,"dspvb_as.c, LOOPBACK_SetDownlinkMute, ioctl fail, is_mute: %d ")
TRACE_MSG(DSPVB_AS_590_112_2_18_1_2_16_127,"dspvb_as.c, LAYER1_SetDevMode, alloc fail, size: %d")
TRACE_MSG(DSPVB_AS_593_112_2_18_1_2_16_128,"LAYER1_SetDevMode eMode:%d.")
TRACE_MSG(DSPVB_AS_614_112_2_18_1_2_16_129,"LAYER1_SetDevMode s_pcm_on_count>0.")
TRACE_MSG(DSPVB_AS_629_112_2_18_1_2_16_130,"[LAYER1_SetDevMode]:Wrong mode = %d")
TRACE_MSG(DSPVB_AS_663_112_2_18_1_2_16_131,"[LAYER1_SetDevMode]:Wrong mode = %d")
TRACE_MSG(DSPVB_AS_683_112_2_18_1_2_16_132,"dspvb_as.c, RECORD_SetDevMode, alloc fail, size: %d")
TRACE_MSG(DSPVB_AS_689_112_2_18_1_2_16_133,"[RECORD_SetDevMode]: enter  mode = %d")
TRACE_MSG(DSPVB_AS_705_112_2_18_1_2_16_134,"[RECORD_SetDevMode]:Wrong mode = %d")
TRACE_MSG(DSPVB_AS_720_112_2_18_1_2_16_135,"[RECORD_SetDevMode]: mode = %d")
TRACE_MSG(DSPVB_AS_740_112_2_18_1_2_16_136,"dspvb_as.c, MP4RECORD_SetDevMode, alloc fail, size: %d")
TRACE_MSG(DSPVB_AS_745_112_2_18_1_2_16_137,"[MP4RECORD_SetDevMode]: enter  mode = %d")
TRACE_MSG(DSPVB_AS_762_112_2_18_1_2_16_138,"[MP4RECORD_SetDevMode]:Wrong mode = %d")
TRACE_MSG(DSPVB_AS_777_112_2_18_1_2_16_139,"[MP4RECORD_SetDevMode]:Wrong mode = %d")
TRACE_MSG(DSPVB_AS_795_112_2_18_1_2_16_140,"dspvb_as.c, LOOPBACK_SetDevMode, alloc fail, size: %d")
TRACE_MSG(DSPVB_AS_819_112_2_18_1_2_16_141,"LOOPBACK_SetDevMode s_pcm_on_count>0.")
TRACE_MSG(DSPVB_AS_834_112_2_18_1_2_16_142,"[LOOPBACK_SetDevMode]:Wrong mode = %d")
TRACE_MSG(DSPVB_AS_869_112_2_18_1_2_16_143,"[LOOPBACK_SetDevMode]:Wrong mode = %d")
TRACE_MSG(DSPVB_AS_886_112_2_18_1_2_17_144,"[LAYER1_Play]:s_b_dspvb_playing:%d,s_b_layer1_playing:%d")
TRACE_MSG(DSPVB_AS_897_112_2_18_1_2_17_145,"[LAYER1_Play] _DSPVB_OpenDev failed.")
TRACE_MSG(DSPVB_AS_911_112_2_18_1_2_17_146,"[LAYER1_Play]: dsp vb is already opened by other device!")
TRACE_MSG(DSPVB_AS_934_112_2_18_1_2_17_147,"[LAYER1_Stop]: s_b_dspvb_playing:%d,s_b_layer1_playing:%d")
TRACE_MSG(DSPVB_AS_951_112_2_18_1_2_17_148,"[LAYER1_Stop]: dspdata is using vb,so we should not close vb!")
TRACE_MSG(DSPVB_AS_956_112_2_18_1_2_17_149,"[LAYER1_Stop]: stop exception!")
TRACE_MSG(DSPVB_AS_984_112_2_18_1_2_17_150,"[LAYER1_Pause]: dspdata is using vb,so we should not close vb!")
TRACE_MSG(DSPVB_AS_989_112_2_18_1_2_17_151,"[LAYER1_Pause]: pause exception!")
TRACE_MSG(DSPVB_AS_1014_112_2_18_1_2_17_152,"[LAYER1_Resume] _DSPVB_OpenDev failed.")
TRACE_MSG(DSPVB_AS_1022_112_2_18_1_2_17_153,"[LAYER1_Resume]: dsp vb is already opened by other device!")
TRACE_MSG(DSPVB_AS_1048_112_2_18_1_2_17_154,"[RECORD_Play] _DSPVB_OpenDev failed.")
TRACE_MSG(DSPVB_AS_1062_112_2_18_1_2_17_155,"dspvb_as.c:RECORD_Play: dsp is playing")
TRACE_MSG(DSPVB_AS_1085_112_2_18_1_2_17_156,"dspvb_as.c:RECORD_Stop: dsp is playing")
TRACE_MSG(DSPVB_AS_1107_112_2_18_1_2_17_157,"dspvb_as.c:RECORD_Pause: dsp is playing")
TRACE_MSG(DSPVB_AS_1126_112_2_18_1_2_17_158,"[RECORD_Resume] _DSPVB_OpenDev failed.")
TRACE_MSG(DSPVB_AS_1141_112_2_18_1_2_17_159,"dspvb_as.c:RECORD_Resume: dsp is playing")
TRACE_MSG(DSPVB_AS_1159_112_2_18_1_2_17_160,"[MP4RECORD_Play] _DSPVB_OpenDev failed.")
TRACE_MSG(DSPVB_AS_1175_112_2_18_1_2_17_161,"dspvb_as.c:MP4RECORD_Play: dsp is playing")
TRACE_MSG(DSPVB_AS_1200_112_2_18_1_2_17_162,"dspvb_as.c:MP4RECORD_Stop: dsp is playing")
TRACE_MSG(DSPVB_AS_1225_112_2_18_1_2_17_163,"dspvb_as.c:MP4RECORD_Pause: dsp is playing")
TRACE_MSG(DSPVB_AS_1246_112_2_18_1_2_17_164,"[MP4RECORD_Resume] _DSPVB_OpenDev failed.")
TRACE_MSG(DSPVB_AS_1261_112_2_18_1_2_17_165,"dspvb_as.c:MP4RECORD_Resume: dsp is playing")
TRACE_MSG(DSPVB_AS_1274_112_2_18_1_2_17_166,"[LOOPBACK_Play]:s_b_dspvb_playing:%d,s_b_layer1_playing:%d")
TRACE_MSG(DSPVB_AS_1282_112_2_18_1_2_17_167,"[LOOPBACK_Play] _DSPVB_OpenDev failed.")
TRACE_MSG(DSPVB_AS_1296_112_2_18_1_2_17_168,"[LOOPBACK_Play]: dsp vb is already opened by other device!")
TRACE_MSG(DSPVB_AS_1319_112_2_18_1_2_17_169,"[LOOPBACK_Stop]: s_b_dspvb_playing:%d,s_b_layer1_playing:%d")
TRACE_MSG(DSPVB_AS_1331_112_2_18_1_2_17_170,"[LOOPBACK_Stop]: dspdata is using vb,so we should not close vb!")
TRACE_MSG(DSPVB_AS_1336_112_2_18_1_2_17_171,"[LOOPBACK_Stop]: stop exception!")
TRACE_MSG(DSPVB_AS_1490_112_2_18_1_2_18_172,"[DSPDATA_Play]:s_b_dspvb_playing:%d,s_b_dspdata_playing:%d")
TRACE_MSG(DSPVB_AS_1498_112_2_18_1_2_18_173,"[DSPDATA_Play] _DSPVB_OpenDev failed.")
TRACE_MSG(DSPVB_AS_1509_112_2_18_1_2_18_174,"[DSPDATA_Play]: dsp vb is already opened by other device!")
TRACE_MSG(DSPVB_AS_1524_112_2_18_1_2_18_175,"[DSPDATA_Stop]:s_b_dspvb_playing:%d,s_b_dspdata_playing:%d")
TRACE_MSG(DSPVB_AS_1536_112_2_18_1_2_18_176,"[DSPDATA_Stop]: layer1 is using vb,so we should not close vb!")
TRACE_MSG(DSPVB_AS_1541_112_2_18_1_2_18_177,"[DSPDATA_Stop]: stop exception!")
TRACE_MSG(DSPVB_AS_1564_112_2_18_1_2_18_178,"[DSPDATA_Pause]: layer1 is using vb,so we should not close vb!")
TRACE_MSG(DSPVB_AS_1569_112_2_18_1_2_18_179,"[DSPDATA_Pause]: pause exception!")
TRACE_MSG(DSPVB_AS_1588_112_2_18_1_2_18_180,"[DSPDATA_Resume] _DSPVB_OpenDev failed.")
TRACE_MSG(DSPVB_AS_1600_112_2_18_1_2_18_181,"[DSPDATA_Play]: dsp vb is already opened by other device!")
TRACE_MSG(PCM_DEV_255_112_2_18_1_2_19_182,"_PcmDev_EnableDigitalGain add express:0x%x to codec:0x%x.")
TRACE_MSG(PCM_DEV_269_112_2_18_1_2_19_183,"_PcmDev_EnableDigitalGain delete express:0x%x from codec:0x%x.")
TRACE_MSG(PCM_DEV_294_112_2_18_1_2_19_184,"PcmDev_SetVolumeType vol:%d, appType:%d.")
TRACE_MSG(PCM_DEV_299_112_2_18_1_2_19_185,"PcmDev_SetVolumeType volume level is too bit(%d), will be adjusted to be:%d.")
TRACE_MSG(PCM_DEV_306_112_2_18_1_2_19_186,"PcmDev_SetVolumeType filed to get modeName:%d.")
TRACE_MSG(PCM_DEV_312_112_2_18_1_2_19_187,"PcmDev_SetVolumeType, alloc fail, size: %d")
TRACE_MSG(PCM_DEV_319_112_2_18_1_2_19_188,"PcmDev_SetVolumeType failed to get mode param, mode:%d, modeName:%s.")
TRACE_MSG(PCM_DEV_325_112_2_18_1_2_19_189,"PcmDev_SetVolumeType uiVol=%d, mode:%d, appType:%d, modeName:%s.")
TRACE_MSG(PCM_DEV_338_112_2_18_1_2_19_190,"PcmDev_SetVolumeType gain:0x%4x, dg_level= 0x%4x")
TRACE_MSG(PCM_DEV_359_112_2_18_1_2_20_191,"PcmDev_SetDownlinkMute, mute:%d, vol:%d, appType:%d.")
TRACE_MSG(PCM_DEV_535_112_2_18_1_2_20_192,"====PCM_Open=====")
TRACE_MSG(PCM_DEV_537_112_2_18_1_2_20_193,"[PCMDEV_Play] IN (s_pcm_on_count = %d)")
TRACE_MSG(PCM_DEV_709_112_2_18_1_2_20_194,"PCMDEV_SetDevMode:%d.")
END_TRACE_MAP(MS_REF_AUD_DEV_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_AUD_DEV_TRC_H_

