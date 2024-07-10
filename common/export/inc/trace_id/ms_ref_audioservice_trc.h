/******************************************************************************
 ** File Name:      ms_ref_audioservice_trc.h                                         *
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
//trace_id:35
#ifndef _MS_REF_AUDIOSERVICE_TRC_H_
#define _MS_REF_AUDIOSERVICE_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define AUDIO_API_132_112_2_18_1_2_48_0 "audio_api.c:AUDIO_CreateFileHandle:error:PNULL"
#define AUDIO_API_140_112_2_18_1_2_48_1 "audio_api.c:AUDIO_CreateFileHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_160_112_2_18_1_2_48_2 "audio_api.c:AUDIO_CreateFileHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_170_112_2_18_1_2_48_3 "audio_api.c:AUDIO_CreateFileHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_187_112_2_18_1_2_48_4 "audio_api.c:AUDIO_CreateFileHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_198_112_2_18_1_2_48_5 "audio_api.c:AUDIO_CreateFileHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_214_112_2_18_1_2_48_6 "audio_api.c:AUDIO_CreateFileHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_225_112_2_18_1_2_48_7 "audio_api.c:AUDIO_CreateFileHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_303_112_2_18_1_2_49_8 "audio_api.c:AUDIO_ASYCreateFileHandle:error:PNULL"
#define AUDIO_API_311_112_2_18_1_2_49_9 "audio_api.c:AUDIO_ASYCreateFileHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_331_112_2_18_1_2_49_10 "audio_api.c:AUDIO_ASYCreateFileHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_341_112_2_18_1_2_49_11 "audio_api.c:AUDIO_ASYCreateFileHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_357_112_2_18_1_2_49_12 "audio_api.c:AUDIO_ASYCreateFileHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_368_112_2_18_1_2_49_13 "audio_api.c:AUDIO_ASYCreateFileHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_384_112_2_18_1_2_49_14 "audio_api.c:AUDIO_ASYCreateFileHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_396_112_2_18_1_2_49_15 "audio_api.c:AUDIO_ASYCreateFileHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_471_112_2_18_1_2_49_16 "audio_api.c:AUDIO_CreateBufferHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_491_112_2_18_1_2_49_17 "audio_api.c:AUDIO_CreateBufferHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_501_112_2_18_1_2_49_18 "audio_api.c:AUDIO_CreateBufferHandle:sigAUDIO_NO_ERROR != AUDIO_PM_StrCpLL"
#define AUDIO_API_517_112_2_18_1_2_49_19 "audio_api.c:AUDIO_CreateBufferHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_527_112_2_18_1_2_49_20 "audio_api.c:AUDIO_CreateBufferHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCp"
#define AUDIO_API_608_112_2_18_1_2_49_21 "audio_api.c:AUDIO_CreateBufferHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_628_112_2_18_1_2_49_22 "audio_api.c:AUDIO_CreateBufferHandle:codec_name_ptr == SCI_NULL"
#define AUDIO_API_638_112_2_18_1_2_49_23 "audio_api.c:AUDIO_CreateBufferHandle:O_ERROR != AUDIO_PM_"
#define AUDIO_API_654_112_2_18_1_2_49_24 "audio_api.c:AUDIO_CreateBufferHandle:device_name_ptr==SCI_NULL"
#define AUDIO_API_665_112_2_18_1_2_49_25 "audio_api.c:AUDIO_CreateBufferHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_744_112_2_18_1_2_49_26 "audio_api.c:AUDIO_CreateStreamBufferHandle:error:CallbackFunc==PNULL"
#define AUDIO_API_752_112_2_18_1_2_50_27 "audio_api.c:AUDIO_CreateStreamBufferHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_772_112_2_18_1_2_50_28 "audio_api.c:AUDIO_CreateStreamBufferHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_782_112_2_18_1_2_50_29 "audio_api.c:AUDIO_CreateStreamBufferHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_798_112_2_18_1_2_50_30 "audio_api.c:AUDIO_CreateStreamBufferHandle:device_name_ptr==SCI_NULL"
#define AUDIO_API_809_112_2_18_1_2_50_31 "audio_api.c:AUDIO_CreateStreamBufferHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_894_112_2_18_1_2_50_32 "audio_api.c:AUDIO_ASYCreateStreamBufferHandle:error:CallbackFunc==PNULL"
#define AUDIO_API_902_112_2_18_1_2_50_33 "audio_api.c:AUDIO_ASYCreateStreamBufferHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_922_112_2_18_1_2_50_34 "audio_api.c:AUDIO_ASYCreateStreamBufferHandle:codec_name_ptr==SCI_NULL"
#define AUDIO_API_932_112_2_18_1_2_50_35 "audio_api.c:AUDIO_ASYCreateStreamBufferHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_948_112_2_18_1_2_50_36 "audio_api.c:AUDIO_ASYCreateStreamBufferHandle:device_name_ptr==SCI_NULL"
#define AUDIO_API_959_112_2_18_1_2_50_37 "audio_api.c:AUDIO_ASYCreateStreamBufferHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_1009_112_2_18_1_2_50_38 "audio_api.c:AUDIO_CloseHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_1064_112_2_18_1_2_50_39 "audio_api.c:AUDIO_ASYCloseHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_1119_112_2_18_1_2_50_40 "audio_api.c:AUDIO_Play:sig_ptr == SCI_NULL"
#define AUDIO_API_1181_112_2_18_1_2_50_41 "audio_api.c:AUDIO_ASYPlay:sig_ptr == SCI_NULL"
#define AUDIO_API_1234_112_2_18_1_2_50_42 "audio_api.c:AUDIO_Stop:sig_ptr == SCI_NULL"
#define AUDIO_API_1299_112_2_18_1_2_51_43 "audio_api.c:AUDIO_ASYStop:sig_ptr == SCI_NULL"
#define AUDIO_API_1349_112_2_18_1_2_51_44 "audio_api.c:AUDIO_Pause:sig_ptr == SCI_NULL"
#define AUDIO_API_1416_112_2_18_1_2_51_45 "audio_api.c:AUDIO_ASYPause:sig_ptr == SCI_NULL"
#define AUDIO_API_1464_112_2_18_1_2_51_46 "audio_api.c:AUDIO_Resume:sig_ptr == SCI_NULL"
#define AUDIO_API_1520_112_2_18_1_2_51_47 "audio_api.c:AUDIO_ASYResume:sig_ptr == SCI_NULL"
#define AUDIO_API_1570_112_2_18_1_2_51_48 "audio_api.c:AUDIO_Seek:sig_ptr == SCI_NULL"
#define AUDIO_API_1627_112_2_18_1_2_51_49 "audio_api.c:AUDIO_Seek:sig_ptr == SCI_NULL"
#define AUDIO_API_1687_112_2_18_1_2_51_50 "audio_api.c:AUDIO_ASYSeek:sig_ptr == SCI_NULL"
#define AUDIO_API_1739_112_2_18_1_2_51_51 "audio_api.c:AUDIO_SetSpeed:sig_ptr == SCI_NULL"
#define AUDIO_API_1800_112_2_18_1_2_52_52 "audio_api.c:AUDIO_ASYSetSpeed:sig_ptr == SCI_NULL"
#define AUDIO_API_1855_112_2_18_1_2_52_53 "audio_api.c:AUDIO_SetDigitalGain:sig_ptr == SCI_NULL"
#define AUDIO_API_1915_112_2_18_1_2_52_54 "audio_api.c:AUDIO_ASYSetDigitalGain:sig_ptr == SCI_NULL"
#define AUDIO_API_1967_112_2_18_1_2_52_55 "audio_api.c:AUDIO_GetDigitalGain:sig_ptr == SCI_NULL"
#define AUDIO_API_2025_112_2_18_1_2_52_56 "audio_api.c:AUDIO_ASYGetDigitalGain:sig_ptr == SCI_NULL"
#define AUDIO_API_2076_112_2_18_1_2_52_57 "audio_api.c:AUDIO_SendStreamSrcData:sig_ptr == SCI_NULL"
#define AUDIO_API_2153_112_2_18_1_2_52_58 "audio_api.c:AUDIO_GetDigitalGain:sig_ptr == SCI_NULL"
#define AUDIO_API_2268_112_2_18_1_2_53_59 "audio_api.c:AUDIO_CreateRecordFileHandle:error:PNULL"
#define AUDIO_API_2276_112_2_18_1_2_53_60 "audio_api.c:AUDIO_CreateRecordFileHandle:sig_ptr == SCI_NULL"
#define AUDIO_API_2298_112_2_18_1_2_53_61 "audio_api.c:AUDIO_CreateRecordFileHandle:codec_name_ptr==SCI_NULL"
#define AUDIO_API_2308_112_2_18_1_2_53_62 "audio_api.c:AUDIO_CreateRecordFileHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16("
#define AUDIO_API_2324_112_2_18_1_2_53_63 "audio_api.c:AUDIO_CreateRecordFileHandle:device_name_ptr==SCI_NULL"
#define AUDIO_API_2335_112_2_18_1_2_53_64 "audio_api.c:AUDIO_CreateRecordFileHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy1"
#define AUDIO_API_2350_112_2_18_1_2_53_65 "audio_api.c:AUDIO_CreateRecordFileHandle:file_name_ptr==SCI_NULL"
#define AUDIO_API_2362_112_2_18_1_2_53_66 "audio_api.c:AUDIO_CreateRecordFileHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_2436_112_2_18_1_2_53_67 "audio_api.c:AUDIO_ASYCreateRecordFileHandle:error:PNULL"
#define AUDIO_API_2444_112_2_18_1_2_53_68 "audio_api.c:AUDIO_ASYCreateRecordFileHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_2466_112_2_18_1_2_53_69 "audio_api.c:AUDIO_ASYCreateRecordFileHandle:codec_name_ptr==SCI_NULL"
#define AUDIO_API_2476_112_2_18_1_2_53_70 "audio_api.c:AUDIO_ASYCreateRecordFileHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_2492_112_2_18_1_2_53_71 "audio_api.c:AUDIO_ASYCreateRecordFileHandle:device_name_ptr==SCI_NULL"
#define AUDIO_API_2503_112_2_18_1_2_53_72 "audio_api.c:AUDIO_ASYCreateRecordFileHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_2519_112_2_18_1_2_53_73 "audio_api.c:AUDIO_ASYCreateRecordFileHandle:file_name_ptr==SCI_NULL"
#define AUDIO_API_2531_112_2_18_1_2_53_74 "audio_api.c:AUDIO_ASYCreateRecordFileHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_2605_112_2_18_1_2_53_75 "audio_api.c:AUDIO_CreateRecordFileHandle_FileCreated:error:sig_ptr == SCI_NULL"
#define AUDIO_API_2627_112_2_18_1_2_53_76 "audio_api.c:AUDIO_CreateRecordFileHandle_FileCreated:error:codec_name_ptr==SCI_NULL"
#define AUDIO_API_2637_112_2_18_1_2_53_77 "audio_api.c:AUDIO_CreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_2653_112_2_18_1_2_53_78 "audio_api.c:AUDIO_CreateRecordFileHandle_FileCreated:error:device_name_ptr==SCI_NULL"
#define AUDIO_API_2664_112_2_18_1_2_53_79 "audio_api.c:AUDIO_CreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_2680_112_2_18_1_2_53_80 "audio_api.c:AUDIO_CreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_2692_112_2_18_1_2_53_81 "audio_api.c:AUDIO_CreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_2769_112_2_18_1_2_54_82 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:PNULL"
#define AUDIO_API_2777_112_2_18_1_2_54_83 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:sig_ptr == SCI_NULL"
#define AUDIO_API_2799_112_2_18_1_2_54_84 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_2809_112_2_18_1_2_54_85 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_2825_112_2_18_1_2_54_86 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:device_name_ptr==SCI_NULL"
#define AUDIO_API_2836_112_2_18_1_2_54_87 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_2852_112_2_18_1_2_54_88 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:file_name_ptr==SCI_NULL"
#define AUDIO_API_2864_112_2_18_1_2_54_89 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_2936_112_2_18_1_2_54_90 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:sig_ptr == SCI_NULL"
#define AUDIO_API_2956_112_2_18_1_2_54_91 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:codec_name_ptr==SCI_NULL"
#define AUDIO_API_2966_112_2_18_1_2_54_92 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16(cod"
#define AUDIO_API_2982_112_2_18_1_2_54_93 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:device_name_ptr==SCI_NULL"
#define AUDIO_API_2993_112_2_18_1_2_54_94 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_3071_112_2_18_1_2_54_95 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:sig_ptr == SCI_NULL"
#define AUDIO_API_3091_112_2_18_1_2_54_96 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:codec_name_ptr==SCI_NULL"
#define AUDIO_API_3101_112_2_18_1_2_54_97 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_3117_112_2_18_1_2_54_98 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:device_name_ptr==SCI_NULL"
#define AUDIO_API_3128_112_2_18_1_2_54_99 "audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_3188_112_2_18_1_2_54_100 "audio_api.c:AUDIO_CreateNormalHandle:error:sig_ptr == SCI_NULL"
#define AUDIO_API_3208_112_2_18_1_2_54_101 "audio_api.c:AUDIO_CreateNormalHandle:error:codec_name_ptr==SCI_NULL"
#define AUDIO_API_3218_112_2_18_1_2_54_102 "audio_api.c:AUDIO_CreateNormalHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_3234_112_2_18_1_2_54_103 "audio_api.c:AUDIO_CreateNormalHandle:error:pusDevName!=PNULL"
#define AUDIO_API_3245_112_2_18_1_2_55_104 "audio_api.c:AUDIO_CreateNormalHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_3292_112_2_18_1_2_55_105 "audio_api.c:AUDIO_CreateCloneHandle:error:sig_ptr == SCI_NULL"
#define AUDIO_API_3313_112_2_18_1_2_55_106 "audio_api.c:AUDIO_CreateCloneHandle:error:pusDevName!=PNULL"
#define AUDIO_API_3324_112_2_18_1_2_55_107 "audio_api.c:AUDIO_CreateCloneHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_3387_112_2_18_1_2_55_108 "audio_api.c:AUDIO_ASYCreateNormalHandle:error:sig_ptr == SCI_NULL"
#define AUDIO_API_3407_112_2_18_1_2_55_109 "audio_api.c:AUDIO_ASYCreateNormalHandle:error:sig_ptr == SCI_NULL"
#define AUDIO_API_3417_112_2_18_1_2_55_110 "audio_api.c:AUDIO_ASYCreateNormalHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_3433_112_2_18_1_2_55_111 "audio_api.c:AUDIO_ASYCreateNormalHandle:error:device_name_ptr==SCI_NULL"
#define AUDIO_API_3444_112_2_18_1_2_55_112 "audio_api.c:AUDIO_ASYCreateNormalHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_3586_112_2_18_1_2_55_113 "audio_api.c:AUDIO_SwitchDevice:error:sig_ptr == SCI_NULL"
#define AUDIO_API_3606_112_2_18_1_2_55_114 "audio_api.c:AUDIO_SwitchDevice:error:sig_ptr->pusDevName==SCI_NULL"
#define AUDIO_API_3614_112_2_18_1_2_55_115 "audio_api.c:AUDIO_SwitchDevice:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_3659_112_2_18_1_2_55_116 "audio_api.c:AUDIO_ASYSwitchDevice:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_3678_112_2_18_1_2_55_117 "audio_api.c:AUDIO_ASYSwitchDevice:error:sig_ptr->pusDevName==SCI_NULL"
#define AUDIO_API_3686_112_2_18_1_2_55_118 "audio_api.c:AUDIO_ASYSwitchDevice:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16"
#define AUDIO_API_3844_112_2_18_1_2_56_119 "audio_api.c:AUDIO_SetVolume:error:sig_ptr == SCI_NULL"
#define AUDIO_API_3900_112_2_18_1_2_56_120 "audio_api.c:AUDIO_SetVolume:error:sig_ptr == SCI_NULL"
#define AUDIO_API_3964_112_2_18_1_2_56_121 "audio_api.c:AUDIO_SetDevMode:error:sig_ptr == SCI_NULL"
#define AUDIO_API_3970_112_2_18_1_2_56_122 "AUDIO_SetDevMode new mode:%d, thread:0x%x."
#define AUDIO_API_4022_112_2_18_1_2_56_123 "AUDIO_SetDevModeEx, audio alloc failed."
#define AUDIO_API_4026_112_2_18_1_2_56_124 "AUDIO_SetDevModeEx new mode:%d, thread:0x%x."
#define AUDIO_API_4082_112_2_18_1_2_56_125 "audio_api.c:AUDIO_SetUpLinkMute:error:sig_ptr == SCI_NULL"
#define AUDIO_API_4141_112_2_18_1_2_56_126 "audio_api.c:AUDIO_SetDownLinkMute:error:sig_ptr == SCI_NULL"
#define AUDIO_API_4203_112_2_18_1_2_56_127 "audio_api.c:AUDIO_SetDownLinkLocalNetMute:Can't alloc enough memory for sig_ptr"
#define AUDIO_API_4266_112_2_18_1_2_57_128 "audio_api.c:AUDIO_SetDevDigitalGain:error:sig_ptr == SCI_NULL"
#define AUDIO_API_4328_112_2_18_1_2_57_129 "audio_api.c:AUDIO_SetDataOutputChannelChoice:error:sig_ptr == SCI_NULL"
#define AUDIO_API_4477_112_2_18_1_2_57_130 "audio_api.c:AUDIO_SetTrackOn:error:sig_ptr == SCI_NULL"
#define ADMM_249_112_2_18_1_2_58_131 "admm.c:AUDMOD_SetDevMode:error:ptModNode is SCI_NULL, name:%s."
#define ADMM_284_112_2_18_1_2_58_132 "AUDMOD_SetDevMode set value: %d, %d, %d, %d, %d, %d, %d."
#define ADMM_285_112_2_18_1_2_58_133 "AUDMOD_SetDevMode: ad device original value =0x%4x"
#define ADMM_289_112_2_18_1_2_58_134 "AUDMOD_SetDevMode: ad device modified value =0x%4x"
#define ADMM_294_112_2_18_1_2_58_135 "AUDMOD_SetDevMode: M da device original value =0x%4x"
#define ADMM_305_112_2_18_1_2_58_136 "AUDMOD_SetDevMode: M da device modified value =0x%4x"
#define ADMM_315_112_2_18_1_2_58_137 "AUDMOD_SetDevMode: dolphin da device original value =0x%4x"
#define ADMM_356_112_2_18_1_2_58_138 "AUDMOD_SetDevMode: dolphin da device modified value =0x%4x"
#define ADMM_404_112_2_18_1_2_58_139 "admm.c:AUDMOD_SetDevModeEx:error:tModNode:0x%x, ptModNodetd:0x%x, name:%s(gsm), %s(td)."
#define ADMM_441_112_2_18_1_2_58_140 "AUDMOD_SetDevModeEx set value: %d, %d, %d, %d, %d, %d, %d."
#define ADMM_443_112_2_18_1_2_58_141 "AUDMOD_SetDevModeEx: ad device original value =0x%4x"
#define ADMM_447_112_2_18_1_2_58_142 "AUDMOD_SetDevModeEx: ad device modified value =0x%4x"
#define ADMM_452_112_2_18_1_2_58_143 "AUDMOD_SetDevModeEx: M da device original value =0x%4x"
#define ADMM_462_112_2_18_1_2_58_144 "AUDMOD_SetDevModeEx: M da device modified value =0x%4x"
#define ADMM_472_112_2_18_1_2_58_145 "AUDMOD_SetDevModeEx: dolphin da device original value =0x%4x"
#define ADMM_481_112_2_18_1_2_59_146 "AUDMOD_SetDevModeEx: dolphin da device modified value =0x%4x"
#define ADMM_491_112_2_18_1_2_59_147 "AUDMOD_SetDevModeEx:  device modified value =0x%4x,0x%4x"
#define ADMM_538_112_2_18_1_2_59_148 "admm.c:AUDMOD_GetNextDevMode:error:ptPreDevMode == SCI_NULL"
#define ADMM_567_112_2_18_1_2_59_149 "admm.c:AUDMOD_AddDevMode:error:ptDevMode == SCI_NULL"
#define ADMM_577_112_2_18_1_2_59_150 "admm.c:AUDMOD_AddDevMode:error:2 ptDevMode == SCI_NULL"
#define ADMM_628_112_2_18_1_2_59_151 "admm.c:AUDMOD_RemoveDevMode:error:ptDevMode == SCI_NULL"
#define ADMM_923_112_2_18_1_3_0_152 "admm.c:AUDMOD_GetModeParam:error:iceMode == SCI_NULL) ||  (pucModeName == SC"
#define ADMM_959_112_2_18_1_3_0_153 "admm.c:AUDMOD_SetModeParam:error:iceMode == SCI_NULL) ||  (pucModeName == SC"
#define ADMM_965_112_2_18_1_3_0_154 "AUDMOD_SetModeParam name :%s"
#define ADMM_985_112_2_18_1_3_0_155 "AUDMOD_SetModeParam, extend[110]:0x%x, extend[99-100]:0x%x, 0x%x, switch:%d, gain:0x%x"
#define ADMM_1048_112_2_18_1_3_0_156 "admm.c:AUDMOD_GetDevModNode:error:SCI_NULL==pucModeName"
#define ADMM_1084_112_2_18_1_3_0_157 "admm.c AUDMOD_GetAudioDevModeFromName pcModeName is null."
#define ADMM_1088_112_2_18_1_3_0_158 "admm.c AUDMOD_GetAudioDevModeFromName pcModeName is %s."
#define ADMM_1098_112_2_18_1_3_0_159 "admm.c AUDMOD_GetAudioDevModeFromName NV dsp audio mode don't match, pls check name:%s."
#define ADMM_1142_112_2_18_1_3_0_160 "admm.c:ADMM_AddCallbackFuncToSomeTypeList input param ptList is PNULL."
#define ADMM_1147_112_2_18_1_3_0_161 "admm.c:ADMM_AddCallbackFuncToSomeTypeList input param ptList->ptFirstTypeInfo is PNULL."
#define ADMM_1159_112_2_18_1_3_0_162 "admm.c:ADMM_AddCallbackFuncToSomeTypeList alloc failed , size:%d."
#define ADMM_1184_112_2_18_1_3_0_163 "admm.c:AUDMOD_RegCallback ADMM_AddCallbackFuncAndType alloc failed, size:%d."
#define ADMM_1199_112_2_18_1_3_0_164 "admm.c:AUDMOD_RegCallback ADMM_AddCallbackFuncAndType alloc failed 2, size:%d."
#define ADMM_1251_112_2_18_1_3_0_165 "admm.c:AUDMOD_ExeCallbackFunc the callback is null,  eType:%d, index:%d."
#define ADMM_1280_112_2_18_1_3_0_166 "admm.c:AUDMOD_RegCallback eCallbackFuncType is not valid, %d, valid range:(0-%d)."
#define ADMM_1285_112_2_18_1_3_0_167 "admm.c:AUDMOD_RegCallbackFunc eCallbackFuncType pCallbackFunc is null."
#define ADMM_1294_112_2_18_1_3_0_168 "admm.c:AUDMOD_RegCallbackFunc fail 1, result:%d, eType:%d, callbackFunc:0x%x.."
#define ADMM_1300_112_2_18_1_3_0_169 "admm.c:AUDMOD_RegCallbackFunc success 1, eType:%d, callbackFunc:0x%x."
#define ADMM_1314_112_2_18_1_3_0_170 "admm.c:AUDMOD_RegCallbackFunc ptCurCallbackFuncList  is null, this case should not occur."
#define ADMM_1323_112_2_18_1_3_0_171 "admm.c:AUDMOD_RegCallbackFunc fail 2, result:%d, eType:%d, callbackFunc:0x%x.."
#define ADMM_1328_112_2_18_1_3_0_172 "admm.c:AUDMOD_RegCallbackFunc success 2, eType:%d, callbackFunc:0x%x."
#define ADMM_1338_112_2_18_1_3_0_173 "admm.c:AUDMOD_RegCallbackFunc fail 3, result:%d, eType:%d, callbackFunc:0x%x.."
#define ADMM_1343_112_2_18_1_3_0_174 "admm.c:AUDMOD_RegCallbackFunc success 3, eType:%d, callbackFunc:0x%x."
#define ADMM_1351_112_2_18_1_3_1_175 "admm.c AUDMOD_RegCallbackFunc start trace list."
#define ADMM_1354_112_2_18_1_3_1_176 "admm.c AUDMOD_RegCallbackFunc eType:%d."
#define ADMM_1358_112_2_18_1_3_1_177 "admm.c AUDMOD_RegCallbackFunc callbackFunc:0x%x."
#define ADMM_1363_112_2_18_1_3_1_178 "admm.c AUDMOD_RegCallbackFunc stop trace list."
#define ADMM_1387_112_2_18_1_3_1_179 "admm.c:AUDMOD_UnRegCallbackFunc eCallbackFuncType is not valid, %d, valid range:(0-%d)."
#define ADMM_1392_112_2_18_1_3_1_180 "admm.c:AUDMOD_UnRegCallbackFunc eCallbackFuncType pCallbackFunc is null."
#define ADMM_1397_112_2_18_1_3_1_181 "admm.c:AUDMOD_UnRegCallbackFunc s_audio_device_mode_callback_func_ptr is null."
#define ADMM_1406_112_2_18_1_3_1_182 "admm.c:AUDMOD_UnRegCallbackFunc s_audio_device_mode_callback_func_ptr->ptFirstTypeInfo  is null."
#define ADMM_1421_112_2_18_1_3_1_183 "admm.c:AUDMOD_UnRegCallbackFunc success 0, eType:%d, callbackFunc:0x%x."
#define ADMM_1426_112_2_18_1_3_1_184 "admm.c:AUDMOD_UnRegCallbackFunc success 1, eType: %d, and callback 0x%x."
#define ADMM_1440_112_2_18_1_3_1_185 "admm.c:AUDMOD_UnRegCallbackFunc can't find the callbackType %d, and callback 0x%x regitered 1."
#define ADMM_1450_112_2_18_1_3_1_186 "admm.c:AUDMOD_UnRegCallbackFunc success 2, eType:%d, callbackFunc:0x%x."
#define ADMM_1458_112_2_18_1_3_1_187 "admm.c:AUDMOD_UnRegCallbackFunc can't find the callbackType %d, and callback 0x%x regitered 2."
#define ADMM_1474_112_2_18_1_3_1_188 "admm.c:AUDMOD_UnRegCallbackFunc can't find the callbackType %d, and callback 0x%x regitered 3."
#define ADMM_1489_112_2_18_1_3_1_189 "admm.c:AUDMOD_UnRegCallbackFunc success 3_0, eType:%d, callbackFunc:0x%x."
#define ADMM_1494_112_2_18_1_3_1_190 "admm.c:AUDMOD_UnRegCallbackFunc success 3_1, eType:%d, callbackFunc:0x%x."
#define ADMM_1510_112_2_18_1_3_1_191 "admm.c:AUDMOD_UnRegCallbackFunc success 4, eType:%d, callbackFunc:0x%x."
#define ADMM_1517_112_2_18_1_3_1_192 "admm.c:AUDMOD_UnRegCallbackFunc can't find the callbackType %d, and callback 0x%x regitered 4."
#define ADMM_1524_112_2_18_1_3_1_193 "admm.c:AUDMOD_UnRegCallbackFunc can't find the callbackType %d, and callback 0x%x regitered 5."
#define ADMM_1533_112_2_18_1_3_1_194 "admm.c AUDMOD_UnRegCallbackFunc start trace list."
#define ADMM_1536_112_2_18_1_3_1_195 "admm.c AUDMOD_UnRegCallbackFunc eType:%d."
#define ADMM_1540_112_2_18_1_3_1_196 "admm.c AUDMOD_UnRegCallbackFunc callbackFunc:0x%x."
#define ADMM_1545_112_2_18_1_3_1_197 "admm.c AUDMOD_UnRegCallbackFunc stop trace list."
#define AUDIO_NV_ARM_112_112_2_18_1_3_1_198 "audio_nv_arm.c:_AUDIONVARM_GetModeInfo:error:SCI_NULL==pucModeName"
#define AUDIO_NV_ARM_119_112_2_18_1_3_1_199 "audio_nv_arm.c:_AUDIONVARM_GetModeInfo PNULL==spAudioNvArmModeListHead to read arm nv."
#define AUDIO_NV_ARM_155_112_2_18_1_3_1_200 "audio_nv_arm.c:AUDIONVARM_InitModeManager has been called !"
#define AUDIO_NV_ARM_161_112_2_18_1_3_1_201 "audio_nv_arm.c:AUDIONVARM_InitModeManager has refuse to reed!"
#define AUDIO_NV_ARM_184_112_2_18_1_3_1_202 "audio_nv_arm.c:AUDIONVARM_InitModeManager NV_ARM_AUDIO_MODE_NUM read failed, status:%d."
#define AUDIO_NV_ARM_192_112_2_18_1_3_1_203 "audio_nv_arm.c:AUDIONVARM_InitModeManager modeInfo:0x%x, usModeCount:%d, everyModeSize:%d, totalSize:%d.!"
#define AUDIO_NV_ARM_219_112_2_18_1_3_1_204 "audio_nv_arm.c:AUDIONVARM_InitModeManager NV_ARM_AUDIO_MODE_PARA read failed, status:%d."
#define AUDIO_NV_ARM_229_112_2_18_1_3_1_205 "audio_nv_arm.c:AUDIONVARM_InitModeManager Exit !"
#define AUDIO_NV_ARM_246_112_2_18_1_3_1_206 "audio_nv_arm.c:AUDIONVARM_GetModeNumber total count:%d!"
#define AUDIO_NV_ARM_268_112_2_18_1_3_2_207 "audio_nv_arm.c:AUDIONVARM_AddMode:error:ptMode == SCI_NULL"
#define AUDIO_NV_ARM_278_112_2_18_1_3_2_208 "audio_nv_arm.c:AUDIONVARM_AddMode:error:2 ptMode == SCI_NULL"
#define AUDIO_NV_ARM_310_112_2_18_1_3_2_209 "audio_nv_arm.c:AUDIONVARM_AddMode success to add one new arm nv mode, add:0x%x"
#define AUDIO_NV_ARM_331_112_2_18_1_3_2_210 "audio_nv_arm.c:AUDIONVARM_RemoveMode:error:pucModeName == SCI_NULL"
#define AUDIO_NV_ARM_401_112_2_18_1_3_2_211 "audio_nv_arm.c:AUDIONVARM_GetModeParam:error:Mode:0x%x, pucModeName:%s"
#define AUDIO_NV_ARM_411_112_2_18_1_3_2_212 "audio_nv_arm.c:AUDIONVARM_GetModeParam:error:Mode:0x%x, pucModeName:%s, listHeader:0x%x"
#define AUDIO_NV_ARM_420_112_2_18_1_3_2_213 "audio_nv_arm.c:AUDIONVARM_GetModeParam success, nodeInfoAddress:0x%x, size:%d(withName),%d(noName), name:%s."
#define AUDIO_NV_ARM_448_112_2_18_1_3_2_214 "audio_nv_arm.c:AUDIONVARM_SetModeParam:error:Mode:0x%x, pucModeName:0x%x"
#define AUDIO_NV_ARM_459_112_2_18_1_3_2_215 "audio_nv_arm.c:AUDIONVARM_SetModeParam:error:Mode:0x%x, pucModeName:0x%x"
#define AUDIO_NV_ARM_467_112_2_18_1_3_2_216 "audio_nv_arm.c:AUDIONVARM_SetModeParam:failed to getModeInfo, Mode:0x%x, pucModeName:0x%x"
#define AUDIO_NV_ARM_472_112_2_18_1_3_2_217 "audio_nv_arm.c:AUDIONVARM_SetModeParam:vol2:Mode:0x%x, pucModeName:%s, 0x%x"
#define AUDIO_NV_ARM_475_112_2_18_1_3_2_218 "audio_nv_arm.c:AUDIONVARM_SetModeParam:vol3:Mode:0x%x, pucModeName:%s, 0x%x"
#define AUDIO_NV_ARM_478_112_2_18_1_3_2_219 "audio_nv_arm.c:AUDIONVARM_SetModeParam:vol4:Mode:0x%x, pucModeName:%s, 0x%x"
#define AUDIO_NV_ARM_503_112_2_18_1_3_2_220 "audio_nv_arm.c AUDIONVARM_GetAudioModeName count:%d."
#define AUDIO_NV_ARM_514_112_2_18_1_3_2_221 "audio_nv_arm.c AUDIONVARM_GetAudioModeName NV arm audio mode don't match, pls check type:%d."
#define AUDIO_NV_ARM_536_112_2_18_1_3_2_222 "audio_nv_arm.c AUDIONVARM_GetAudioModeType mode_name_ptr is PNULL."
#define AUDIO_NV_ARM_540_112_2_18_1_3_2_223 "audio_nv_arm.c AUDIONVARM_GetAudioModeType count:%d, name:%s."
#define AUDIO_NV_ARM_551_112_2_18_1_3_2_224 "audio_nv_arm.c AUDIONVARM_GetAudioModeType NV arm audio mode don't match, pls check name:%s."
#define AUDIO_NV_ARM_582_112_2_18_1_3_2_225 "audio_nv_arm.c:AUDIONVARM_GetAudioModeDevCtrInfo:error:ptDevCtrInfo:0x%x, pucModeName:0x%x, type:%d."
#define AUDIO_NV_ARM_606_112_2_18_1_3_2_226 "audio_nv_arm.c:AUDIONVARM_GetAudioModeDevCtrInfo, mode:%s, dev_set:%d."
#define AUDIO_NV_ARM_636_112_2_18_1_3_2_227 "audio_nv_arm.c:AUDIONVARM_ReadModeParamFromFlash:error:Mode == SCI_NULL"
#define AUDIO_NV_ARM_697_112_2_18_1_3_2_228 "audio_nv_arm.c:AUDIONVARM_ReadModeParamFromFlash: curMode:%s."
#define AUDIO_NV_ARM_705_112_2_18_1_3_2_229 "audio_nv_arm.c:AUDIONVARM_ReadModeParamFromFlash: success, dataSizeEveryMode(noNameLen):%d, name:%s."
#define AUDIO_NV_ARM_710_112_2_18_1_3_2_230 "audio_nv_arm.c:AUDIONVARM_ReadModeParamFromFlash: this mode is not exist:%s."
#define AUDIO_NV_ARM_740_112_2_18_1_3_2_231 "audio_nv_arm.c:AUDIONVARM_WriteModeParamToFlash:error:Mode == SCI_NULL"
#define AUDIO_NV_ARM_804_112_2_18_1_3_3_232 "audio_nv_arm.c:AUDIONVARM_WriteModeParamToFlash: curMode:%s."
#define AUDIO_NV_ARM_816_112_2_18_1_3_3_233 "audio_nv_arm.c:AUDIONVARM_WriteModeParamToFlash: success, dataSizeEveryMode(noNameLen):%d, name:%s."
#define AUDIO_NV_ARM_823_112_2_18_1_3_3_234 "audio_nv_arm.c:AUDIONVARM_WriteModeParamToFlash: failed."
#define AUDIO_NV_ARM_830_112_2_18_1_3_3_235 "audio_nv_arm.c:AUDIONVARM_WriteModeParamToFlash: this mode is not exist, %s."
#define AUDIO_NV_DSP_104_112_2_18_1_3_3_236 "audio_nv_dsp.c:_AUDIONVDSP_GetModeInfo:error:SCI_NULL==pucModeName"
#define AUDIO_NV_DSP_111_112_2_18_1_3_3_237 "audio_nv_dsp.c:_AUDIONVDSP_GetModeInfo PNULL==spAudioNvDspModeListHead to read dsp nv."
#define AUDIO_NV_DSP_147_112_2_18_1_3_3_238 "audio_nv_dsp.c:AUDIONVDSP_InitModeManager has been called !"
#define AUDIO_NV_DSP_153_112_2_18_1_3_3_239 "audio_nv_dsp.c:AUDIONVDSP_InitModeManager has refuse to reed!"
#define AUDIO_NV_DSP_175_112_2_18_1_3_3_240 "audio_nv_dsp.c:AUDIONVDSP_InitModeManager NV_DSP_CODEC_INFO read failed, status:%d."
#define AUDIO_NV_DSP_185_112_2_18_1_3_3_241 "audio_nv_dsp.c:AUDIONVDSP_InitModeManager modeInfo:0x%x, b_is_dspctrl:%d usModeCount:%d, everyModeSize:%d, totalSize:%d.!"
#define AUDIO_NV_DSP_213_112_2_18_1_3_3_242 "audio_nv_dsp.c:AUDIONVDSP_InitModeManager NV_DSP_AUDIO_MODE_PARA read failed, status:%d."
#define AUDIO_NV_DSP_223_112_2_18_1_3_3_243 "audio_nv_dsp.c:AUDIONVDSP_InitModeManager Exit !"
#define AUDIO_NV_DSP_240_112_2_18_1_3_3_244 "audio_nv_dsp.c:AUDIONVDSP_GetModeNumber total count:%d!"
#define AUDIO_NV_DSP_275_112_2_18_1_3_3_245 "audio_nv_dsp.c:AUDIONVDSP_GetNextDevMode:error:ptPreDevMode == SCI_NULL"
#define AUDIO_NV_DSP_303_112_2_18_1_3_3_246 "audio_nv_dsp.c:AUDIONVDSP_AddMode:error:ptMode == SCI_NULL"
#define AUDIO_NV_DSP_313_112_2_18_1_3_3_247 "audio_nv_dsp.c:AUDIONVDSP_AddMode:error:2 ptMode == SCI_NULL"
#define AUDIO_NV_DSP_345_112_2_18_1_3_3_248 "audio_nv_dsp.c:AUDIONVDSP_AddMode success to add one new dsp nv mode, add:0x%x ucModeName:%s"
#define AUDIO_NV_DSP_366_112_2_18_1_3_3_249 "audio_nv_dsp.c:AUDIONVDSP_RemoveMode:error:pucModeName == SCI_NULL"
#define AUDIO_NV_DSP_436_112_2_18_1_3_4_250 "audio_nv_dsp.c:AUDIONVDSP_GetModeParam:error:Mode:0x%x, pucModeName:%s"
#define AUDIO_NV_DSP_446_112_2_18_1_3_4_251 "audio_nv_dsp.c:AUDIONVDSP_GetModeParam:error:Mode:0x%x, pucModeName:%s, listHeader:0x%x"
#define AUDIO_NV_DSP_455_112_2_18_1_3_4_252 "audio_nv_dsp.c:AUDIONVDSP_GetModeParam success, nodeInfoAddress:0x%x, size:%d(withName),%d(noName), name:%s."
#define AUDIO_NV_DSP_483_112_2_18_1_3_4_253 "audio_nv_dsp.c:AUDIONVDSP_SetModeParam:error:Mode:0x%x, pucModeName:0x%x"
#define AUDIO_NV_DSP_494_112_2_18_1_3_4_254 "audio_nv_dsp.c:AUDIONVDSP_SetModeParam:error:Mode:0x%x, pucModeName:0x%x"
#define AUDIO_NV_DSP_502_112_2_18_1_3_4_255 "audio_nv_dsp.c:AUDIONVDSP_SetModeParam:failed to getModeInfo, Mode:0x%x, pucModeName:0x%x"
#define AUDIO_NV_DSP_507_112_2_18_1_3_4_256 "audio_nv_dsp.c:AUDIONVDSP_SetModeParam::Mode:0x%x, pucModeName:%s, vol9: 0x%x, 0x%x"
#define AUDIO_NV_DSP_510_112_2_18_1_3_4_257 "audio_nv_dsp.c:AUDIONVDSP_SetModeParam::Mode:0x%x, pucModeName:%s, vol8: 0x%x, 0x%x"
#define AUDIO_NV_DSP_535_112_2_18_1_3_4_258 "audio_nv_dsp.c AUDIONVDSP_GetAudioModeName count:%d."
#define AUDIO_NV_DSP_546_112_2_18_1_3_4_259 "audio_nv_dsp.c AUDIONVDSP_GetAudioModeName NV dsp audio mode don't match, pls check type:%d."
#define AUDIO_NV_DSP_568_112_2_18_1_3_4_260 "audio_nv_dsp.c AUDIONVDSP_GetAudioModeType mode_name_ptr is PNULL."
#define AUDIO_NV_DSP_572_112_2_18_1_3_4_261 "audio_nv_dsp.c AUDIONVDSP_GetAudioModeType count:%d, name:%s."
#define AUDIO_NV_DSP_583_112_2_18_1_3_4_262 "audio_nv_dsp.c AUDIONVDSP_GetAudioModeType NV dsp audio mode don't match, pls check name:%s."
#define AUDIO_NV_DSP_597_112_2_18_1_3_4_263 "audio_nv_dsp.c:AUDIONVDSP_GetAudioIsDspCtrlFromRam PNULL==spAudioNvDspModeListHead to read dsp nv."
#define AUDIO_NV_DSP_601_112_2_18_1_3_4_264 "audio_nv_dsp.c:AUDIONVDSP_GetAudioIsDspCtrlFromRam: success, is_dspctrl:%d ."
#define AUDIO_NV_DSP_613_112_2_18_1_3_4_265 "audio_nv_dsp.c:AUDIONVDSP_SetAudioIsDspCtrlToRam PNULL==spAudioNvDspModeListHead to read dsp nv."
#define AUDIO_NV_DSP_617_112_2_18_1_3_4_266 "audio_nv_dsp.c:AUDIONVDSP_SetAudioIsDspCtrlToRam: success, is_dspctrl:%d ."
#define AUDIO_NV_DSP_643_112_2_18_1_3_4_267 "audio_nv_dsp.c:AUDIONVDSP_GetAudioIsDspCtrlFromFlash: success, usModeInfo:0x%x is_dspctrl:%d ."
#define AUDIO_NV_DSP_682_112_2_18_1_3_4_268 "audio_nv_dsp.c:AUDIONVDSP_SetAudioIsDspCtrlToFlash: success, usModeInfo:0x%x is_dspctrl:%d ."
#define AUDIO_NV_DSP_686_112_2_18_1_3_4_269 "audio_nv_dsp.c:AUDIONVDSP_SetAudioIsDspCtrlToFlash: failed."
#define AUDIO_NV_DSP_717_112_2_18_1_3_4_270 "audio_nv_dsp.c:AUDIONVDSP_ReadModeParamFromFlash:error:Mode == SCI_NULL"
#define AUDIO_NV_DSP_778_112_2_18_1_3_4_271 "audio_nv_dsp.c:AUDIONVDSP_ReadModeParamFromFlash: curMode:%s."
#define AUDIO_NV_DSP_786_112_2_18_1_3_4_272 "audio_nv_dsp.c:AUDIONVDSP_ReadModeParamFromFlash: success, dataSizeEveryMode(noNameLen):%d, name:%s."
#define AUDIO_NV_DSP_791_112_2_18_1_3_4_273 "audio_nv_dsp.c:AUDIONVDSP_ReadModeParamFromFlash: this mode is not exist:%s."
#define AUDIO_NV_DSP_821_112_2_18_1_3_4_274 "audio_nv_dsp.c:AUDIONVDSP_WriteModeParamToFlash:error:Mode == SCI_NULL"
#define AUDIO_NV_DSP_885_112_2_18_1_3_4_275 "audio_nv_dsp.c:AUDIONVDSP_WriteModeParamToFlash: curMode:%s."
#define AUDIO_NV_DSP_897_112_2_18_1_3_4_276 "audio_nv_dsp.c:AUDIONVDSP_WriteModeParamToFlash: success, dataSizeEveryMode(noNameLen):%d, name:%s."
#define AUDIO_NV_DSP_904_112_2_18_1_3_4_277 "audio_nv_dsp.c:AUDIONVDSP_WriteModeParamToFlash: failed."
#define AUDIO_NV_DSP_911_112_2_18_1_3_4_278 "audio_nv_dsp.c:AUDIONVDSP_WriteModeParamToFlash: this mode is not exist, %s."
#define LVVE_CTRL_PARAM_NV_117_112_2_18_1_3_5_279 "lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_GetDevModeName] nxp_mode_e(%d) is error"
#define LVVE_CTRL_PARAM_NV_134_112_2_18_1_3_5_280 "lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_GetDevModeParam] Input params error."
#define LVVE_CTRL_PARAM_NV_141_112_2_18_1_3_5_281 "lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_GetDevModeParam] LVVE_CTRL_PARAM_GetDevModeNode error."
#define LVVE_CTRL_PARAM_NV_147_112_2_18_1_3_5_282 "lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_GetDevModeParam] Success."
#define LVVE_CTRL_PARAM_NV_165_112_2_18_1_3_5_283 "lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_SetDevModeParam] Input params error."
#define LVVE_CTRL_PARAM_NV_171_112_2_18_1_3_5_284 "lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_SetDevModeParam] Input mode name mismatch."
#define LVVE_CTRL_PARAM_NV_179_112_2_18_1_3_5_285 "lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_SetDevModeParam] LVVE_CTRL_PARAM_GetDevModeNode error."
#define LVVE_CTRL_PARAM_NV_185_112_2_18_1_3_5_286 "lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_SetDevModeParam] Success."
#define LVVE_CTRL_PARAM_NV_206_112_2_18_1_3_5_287 "lvve_ctrl_param_nv.c Input params error."
#define LVVE_CTRL_PARAM_NV_213_112_2_18_1_3_5_288 "lvve_ctrl_param_nv.c EFS_NvitemRead error, eResult=%d"
#define LVVE_CTRL_PARAM_NV_221_112_2_18_1_3_5_289 "lvve_ctrl_param_nv.c SCI_ALLOC_APP failed"
#define LVVE_CTRL_PARAM_NV_229_112_2_18_1_3_5_290 "lvve_ctrl_param_nv.c EFS_NvitemRead failed, eResult=%d"
#define LVVE_CTRL_PARAM_NV_238_112_2_18_1_3_5_291 "lvve_ctrl_param_nv.c ReadModeParamFromFlash, dev_mode_name=%s"
#define LVVE_CTRL_PARAM_NV_246_112_2_18_1_3_5_292 "lvve_ctrl_param_nv.c [ReadModeParamFromFlash] failed, not found device mode name"
#define LVVE_CTRL_PARAM_NV_249_112_2_18_1_3_5_293 "lvve_ctrl_param_nv.c [ReadModeParamFromFlash] Success."
#define LVVE_CTRL_PARAM_NV_271_112_2_18_1_3_5_294 "lvve_ctrl_param_nv.c [WriteModeParamToFlash] Input params error."
#define LVVE_CTRL_PARAM_NV_278_112_2_18_1_3_5_295 "lvve_ctrl_param_nv.c EFS_NvitemRead error, eResult=%d"
#define LVVE_CTRL_PARAM_NV_286_112_2_18_1_3_5_296 "lvve_ctrl_param_nv.c SCI_ALLOC_APP failed"
#define LVVE_CTRL_PARAM_NV_294_112_2_18_1_3_5_297 "lvve_ctrl_param_nv.c EFS_NvitemRead failed, eResult=%d"
#define LVVE_CTRL_PARAM_NV_304_112_2_18_1_3_5_298 "lvve_ctrl_param_nv.c [WriteModeParamToFlash], dev_mode_name=%s"
#define LVVE_CTRL_PARAM_NV_312_112_2_18_1_3_5_299 "lvve_ctrl_param_nv.c [WriteModeParamToFlash] failed, not found device mode name"
#define LVVE_CTRL_PARAM_NV_326_112_2_18_1_3_5_300 "lvve_ctrl_param_nv.c [WriteModeParamToFlash] NVITEM_UpdateNvParam_Protect failed"
#define LVVE_CTRL_PARAM_NV_329_112_2_18_1_3_5_301 "lvve_ctrl_param_nv.c [WriteModeParamToFlash] Success."
#define LVVE_CTRL_PARAM_NV_345_112_2_18_1_3_5_302 "lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_AddDevModeNode] Input params error."
#define LVVE_CTRL_PARAM_NV_351_112_2_18_1_3_5_303 "lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_AddDevModeNode] SCI_ALLOC_APP Failed."
#define LVVE_CTRL_PARAM_NV_372_112_2_18_1_3_6_304 "lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_AddDevModeNode] Success."
#define LVVE_CTRL_PARAM_NV_404_112_2_18_1_3_6_305 "lvve_ctrl_param_nv.c EFS_NvitemRead error, eResult=%d"
#define LVVE_CTRL_PARAM_NV_412_112_2_18_1_3_6_306 "lvve_ctrl_param_nv.c SCI_ALLOC_APP failed"
#define LVVE_CTRL_PARAM_NV_420_112_2_18_1_3_6_307 "lvve_ctrl_param_nv.c EFS_NvitemRead failed, eResult=%d"
#define LVVE_CTRL_PARAM_NV_446_112_2_18_1_3_6_308 "lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_GetDevModeNode] Input params error."
#define LVVE_CTRL_PARAM_NV_465_112_2_18_1_3_6_309 "lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_GetDevModeNode] Failed."
#define ADM_DP_127_112_2_18_1_3_8_310 "AUDIO_DP_WriteRingBuffer exp_count[j]==0  OPSITION1"
#define ADM_DP_182_112_2_18_1_3_8_311 "AUDIO_DP_WriteRingBuffer exp_count[j]==0 OPSITION 2"
#define ADM_DP_206_112_2_18_1_3_9_312 "AUDIO_DP_WriteRingBuffer exp_count[j]==0 OPSITION 3"
#define ADM_DP_283_112_2_18_1_3_9_313 "adm_mng.c:AUDIO_DP_InitRingBuffer:error:invalid tracknum"
#define ADM_DP_294_112_2_18_1_3_9_314 "AUDIO_DP_InitRingBuffer GetChannelChoice failed! result:%d."
#define ADM_DP_367_112_2_18_1_3_9_315 "adm_mng.c:AUDIO_DP_InitRingBuffer:error:invalid tracknum"
#define ADM_DP_373_112_2_18_1_3_9_316 "AUDIO_DP_WriteRingBuffer uiCount==0"
#define ADM_DP_382_112_2_18_1_3_9_317 "adm_dp.c:AUDIO_DP_WriteRingBuffer:error:PNULL==ptAudioObject"
#define ADM_DP_410_112_2_18_1_3_9_318 "adm_dp.c:AUDIO_DP_WriteRingBuffer:error:_AUDIO_DP_ExeExpList"
#define ADM_DP_422_112_2_18_1_3_9_319 "g_adm_dec_output_buf %d, %d, %d."
#define ADM_DP_508_112_2_18_1_3_9_320 "ADM,WriteBuf:Wait Device To Take Data: Timeout!"
#define ADM_DP_574_112_2_18_1_3_9_321 "AUDIO_DP_WriteRingBuffer, playing flag:%d, lose data len:%d(halfword)."
#define ADM_DP_610_112_2_18_1_3_9_322 "adm_dp.c:AUDIO_DP_WriteRingBuffer:error:sig_out_ptr == SCI_NULL"
#define ADM_DP_622_112_2_18_1_3_9_323 "AUDIO_DP_WriteRingBuffer Prohibit to change priority of codec task:count:%d, ifhighPri:%d."
#define ADM_DP_646_112_2_18_1_3_9_324 "adm_dp.c:AUDIO_DP_WriteRingBuffer:error:_AUDIO_DP_ExeExpList reExeExp"
#define ADM_DP_652_112_2_18_1_3_9_325 "AUDIO_DP_WriteRingBuffer reExeExp data count:%d"
#define ADM_DP_666_112_2_18_1_3_9_326 "peter: waiting and g_adm_dec_output_buf[uiTrackNum].data_count  is %d"
#define ADM_DP_727_112_2_18_1_3_10_327 "adm_mng.c:AUDIO_DP_WaitBufferEmpty:error:invalid tracknum"
#define ADM_DP_750_112_2_18_1_3_10_328 "ADM,WaitBufferEmpty:Wait Device To Take Data: Timeout!"
#define ADM_DP_812_112_2_18_1_3_10_329 "adm_mng.c:AUDIO_DP_ResetRingBuffer:error:invalid tracknum"
#define ADM_DP_846_112_2_18_1_3_10_330 "adm_mng.c:AUDIO_DP_ResetRingBufferData:error:invalid tracknum"
#define ADM_MNG_143_112_2_18_1_3_11_331 "[AUDIO_DM_SetSwitchDevice] s_b_switch_on=%d"
#define ADM_MNG_165_112_2_18_1_3_11_332 "adm_mng.c:AUDIO_DM_RegAudioDevice:error:ptDeviceInfo == SCI_NULL"
#define ADM_MNG_277_112_2_18_1_3_11_333 "AUDIO_DM_Open AUDIO_SMHC_VerifyHandle failed."
#define ADM_MNG_284_112_2_18_1_3_11_334 "AUDIO_DM_Open AUDIO_DM_GetDeviceHandleStruct failed."
#define ADM_MNG_315_112_2_18_1_3_11_335 "AUDIO_DM_Open AUDIO_DP_InitRingBuffer failed! result:%d."
#define ADM_MNG_327_112_2_18_1_3_11_336 "AUDIO_DM_Open device:0x%08x, ucDevOpenCnt:%d."
#define ADM_MNG_331_112_2_18_1_3_11_337 "adm_mng.c:AUDIO_DM_Open:error:.tAudioDevOpe.pOpen != SCI_NULL"
#define ADM_MNG_350_112_2_18_1_3_11_338 "AUDIO_DM_Open puiBufferStartAddr:0x%x, bNeedTrack:%d."
#define ADM_MNG_358_112_2_18_1_3_11_339 "AUDIO_DM_Open device:0x%08x, ucDevOpenCnt:%d."
#define ADM_MNG_413_112_2_18_1_3_11_340 "adm_mng.c:AUDIO_DM_Close:error:invalid tracknum:%d."
#define ADM_MNG_421_112_2_18_1_3_11_341 "adm_mng.c:AUDIO_DM_Close:error:invalid tracknum"
#define ADM_MNG_441_112_2_18_1_3_11_342 "AUDIO_DM_Close! has paused. device:0x%08x, 0x%08x,uiTotalTrackon:%d."
#define ADM_MNG_452_112_2_18_1_3_11_343 "AUDIO_DM_Close device:0x%08x, ucDevOpenCnt:%d."
#define ADM_MNG_457_112_2_18_1_3_11_344 "AUDIO_DM_Close dont close! device:0x%08x, 0x%08x, uiTotalTrackon:%d."
#define ADM_MNG_482_112_2_18_1_3_11_345 "adm_mng.c:AUDIO_DM_Close:error:invalid tracknum:%d, device:0x%x, 0x%x."
#define ADM_MNG_494_112_2_18_1_3_11_346 "AUDIO_DM_Close! has paused. device:0x%08x, 0x%08x."
#define ADM_MNG_504_112_2_18_1_3_11_347 "AUDIO_DM_Close device:0x%08x, ucDevOpenCnt:%d."
#define ADM_MNG_509_112_2_18_1_3_11_348 "AUDIO_DM_Close dont close! device:0x%08x, uiTotalTrackon:%d."
#define ADM_MNG_554_112_2_18_1_3_12_349 "AUDIO_DM_PreClose! has paused. device:0x%08x, 0x%08x,uiTotalTrackon:%d."
#define ADM_MNG_561_112_2_18_1_3_12_350 "adm_mng.c: AUDIO_DM_PreClose in hDevice is %x track num is %d,bHasPaused is %d"
#define ADM_MNG_608_112_2_18_1_3_12_351 "adm_mng.c:AUDIO_DM_Pause:error:invalid tracknum"
#define ADM_MNG_626_112_2_18_1_3_12_352 "AUDIO_DM_Pause device:0x%08x, ucDevOpenCnt:%d."
#define ADM_MNG_630_112_2_18_1_3_12_353 "adm_mng.c:AUDIO_DM_Pause:error:vOpe.pPause failed!"
#define ADM_MNG_643_112_2_18_1_3_12_354 "adm_mng.c:AUDIO_DM_Pause:error:no matching device"
#define ADM_MNG_656_112_2_18_1_3_12_355 "AUDIO_DM_Pause device:0x%08x, ucDevOpenCnt:%d."
#define ADM_MNG_660_112_2_18_1_3_12_356 "adm_mng.c:AUDIO_DM_Pause:error:2 vOpe.pPause failed!"
#define ADM_MNG_737_112_2_18_1_3_12_357 "adm_mng.c:AUDIO_DM_Resume:error:invalid tracknum"
#define ADM_MNG_763_112_2_18_1_3_12_358 "AUDIO_DM_Resume device:0x%08x, ucDevOpenCnt:%d."
#define ADM_MNG_767_112_2_18_1_3_12_359 "adm_mng.c:AUDIO_DM_Resume:error:vOpe.AUDIO_DM_Resume failed!"
#define ADM_MNG_785_112_2_18_1_3_12_360 "adm_mng.c:AUDIO_DM_Resume:error:no matching device"
#define ADM_MNG_801_112_2_18_1_3_12_361 "AUDIO_DM_Resume device:0x%08x, ucDevOpenCnt:%d."
#define ADM_MNG_805_112_2_18_1_3_12_362 "adm_mng.c:AUDIO_DM_Resume:error:vOpe.AUDIO_DM_Resume failed!"
#define ADM_MNG_897_112_2_18_1_3_12_363 "adm_mng.c:AUDIO_DM_AddTrackIntoDevice:error:invalid tracknum"
#define ADM_MNG_918_112_2_18_1_3_12_364 "adm_mng.c:AUDIO_DM_AddTrackIntoDevice:error:new_item == SCI_NULL"
#define ADM_MNG_975_112_2_18_1_3_13_365 "adm_mng.c:AUDIO_DM_RemoveTrackFromDevice:error:invalid tracknum"
#define ADM_MNG_1032_112_2_18_1_3_13_366 "adm_mng.c:AUDIO_DM_RemoveTrackFromDevice:error:can't find the track in the tracklis"
#define ADM_MNG_1095_112_2_18_1_3_13_367 "adm_mng.c:AUDIO_DM_GetDeviceHandleFromName:error:= SCI_NULL)||(PNULL==pusDevName"
#define ADM_MNG_1131_112_2_18_1_3_13_368 "adm_mng.c:AUDIO_DM_AddDeviceIntoADM"
#define ADM_MNG_1174_112_2_18_1_3_13_369 "adm_mng.c:AUDIO_DM_RemoveDeviceFromADM:error:PNULL==ptDeviceHandle"
#define ADM_MNG_1224_112_2_18_1_3_13_370 "AUDIO_DM_SwitchDevice input hSrcDev:0x%x, hDestDev:0x%x."
#define ADM_MNG_1232_112_2_18_1_3_13_371 "AUDIO_DM_SwitchDevice input hSrcDev:0x%x, hDestDev:0x%x, ERROR."
#define ADM_MNG_1239_112_2_18_1_3_13_372 "AUDIO_DM_SwitchDevice input hSrcDev:0x%x invalid."
#define ADM_MNG_1246_112_2_18_1_3_13_373 "AUDIO_DM_SwitchDevice input hDestDev:0x%x invalid."
#define ADM_MNG_1253_112_2_18_1_3_13_374 "AUDIO_DM_SwitchDevice bIsNeedTrack is different. hSrcDev:0x%x, hDestDev:0x%x."
#define ADM_MNG_1275_112_2_18_1_3_13_375 "AUDIO_DM_SwitchDevice, NeedTrack:%d, srcDevInfo:0x%x, destDevInfo:0x%x."
#define ADM_MNG_1287_112_2_18_1_3_13_376 "AUDIO_DM_SwitchDevice, 0 discard data:%d."
#define ADM_MNG_1303_112_2_18_1_3_13_377 "Error: uiTrackNum(%d)"
#define ADM_MNG_1318_112_2_18_1_3_13_378 "adm_mng.c:AUDIO_DM_SwitchDevice:error:pClose failed"
#define ADM_MNG_1324_112_2_18_1_3_13_379 "AUDIO_DM_SwitchDevice Success to close src device:0x%x"
#define ADM_MNG_1334_112_2_18_1_3_13_380 "AUDIO_DM_SwitchDevice fail to AUDIO_SM_DeInitExpPara."
#define ADM_MNG_1349_112_2_18_1_3_14_381 "AUDIO_DM_SwitchDevice Success to open device:0x%x"
#define ADM_MNG_1365_112_2_18_1_3_14_382 "AUDIO_DM_SwitchDevice fail to AUDIO_SM_InitExpPara."
#define ADM_MNG_1375_112_2_18_1_3_14_383 "AUDIO_DM_SwitchDevice:error:Open dest device failed!"
#define ADM_MNG_1384_112_2_18_1_3_14_384 "AUDIO_DM_SwitchDevice Success to open src device again:0x%x"
#define ADM_MNG_1392_112_2_18_1_3_14_385 "AUDIO_DM_SwitchDevice fail to AUDIO_SM_InitExpPara."
#define ADM_MNG_1401_112_2_18_1_3_14_386 "AUDIO_DM_SwitchDevice:error:open src device failed"
#define ADM_MNG_1416_112_2_18_1_3_14_387 "adm_mng.c:AUDIO_DM_SwitchDevice:error:2 pClose failed"
#define ADM_MNG_1444_112_2_18_1_3_14_388 "adm_mng.c:AUDIO_DM_SwitchDevice:error:2 pOpen failed!"
#define ADM_MNG_1455_112_2_18_1_3_14_389 "adm_mng.c:AUDIO_DM_SwitchDevice:error:3 pOpen src failed!"
#define ADM_MNG_1466_112_2_18_1_3_14_390 "AUDIO_DM_SwitchDevice input hSrcDev:0x%x, hDestDev:0x%x successfully finished."
#define ADM_MNG_1487_112_2_18_1_3_14_391 "adm_mng.c:AUDIO_DM_TrackOn:error:invalid tracknum"
#define ADM_MNG_1495_112_2_18_1_3_14_392 "AUDIO_DM_TrackOn track_num:%d, track_on:%d, device:0x%08x, 0x%x, total_track_on:%d."
#define ADM_MNG_1522_112_2_18_1_3_14_393 "adm_mng.c:AUDIO_DM_TrackOff:error:invalid tracknum"
#define ADM_MNG_1531_112_2_18_1_3_14_394 "AUDIO_DM_TrackOff track_num:%d, track_on:%d, device:0x%08x, 0x%x, total_track_on:%d."
#define ADM_MNG_1589_112_2_18_1_3_14_395 "adm_mng.c:AUDIO_DM_GetTrackStatus:error:invalid tracknum"
#define ADM_MNG_1616_112_2_18_1_3_14_396 "AUDIO_DM_SetDevMode set the same mode:%d."
#define ADM_MNG_1621_112_2_18_1_3_14_397 "AUDIO_DM_SetDevMode ori mode:%d, new mode:%d."
#define ADM_MNG_1685_112_2_18_1_3_14_398 "AUDIO_DM_SetDevModeEx, No device opened yet!"
#define ADM_MNG_1694_112_2_18_1_3_14_399 "AUDIO_DM_SetDevModeEx ori mode:%d, new mode:%d."
#define ADM_MNG_1727_112_2_18_1_3_14_400 "AUDIO_DM_SetDevModeEx, hAudioHandle illegal (0x%x)"
#define ADM_MNG_1734_112_2_18_1_3_14_401 "AUDIO_DM_SetDevModeEx, Error,hAudioHandle is not exist(0x%x)"
#define ADM_MNG_1741_112_2_18_1_3_14_402 "AUDIO_DM_SetDevModeEx, Error audio device handle(0x%x)"
#define ADM_MNG_1748_112_2_18_1_3_14_403 "AUDIO_DM_SetDevModeEx, No device handle struct find!"
#define ADM_MNG_1752_112_2_18_1_3_14_404 "AUDIO_DM_SetDevModeEx ori mode:%d, new mode:%d."
#define ADM_MNG_1765_112_2_18_1_3_14_405 "AUDIO_DM_SetDevModeEx, set mode failed."
#define ADM_MNG_1811_112_2_18_1_3_14_406 "AUDIO_DM_SetVolume set the same uiVol:%d."
#define ADM_MNG_1818_112_2_18_1_3_14_407 "AUDIO_DM_SetVolume ori vol:%d, appType:%d, new vol:%d, appType:%d."
#define ADM_MNG_1895_112_2_18_1_3_15_408 "AUDIO_DM_SetVolumeAndTypeEx, No device opened yet!"
#define ADM_MNG_1906_112_2_18_1_3_15_409 "AUDIO_DM_SetVolumeAndTypeEx ori vol:%d, appType:%d, new vol:%d, appType:%d."
#define ADM_MNG_1957_112_2_18_1_3_15_410 "AUDIO_DM_SetVolumeAndTypeEx, hAudioHandle illegal (0x%x)"
#define ADM_MNG_1964_112_2_18_1_3_15_411 "AUDIO_DM_SetVolumeAndTypeEx, Error,hAudioHandle is not exist(0x%x)"
#define ADM_MNG_1971_112_2_18_1_3_15_412 "AUDIO_DM_SetVolumeAndTypeEx, Error audio device handle(0x%x)"
#define ADM_MNG_1978_112_2_18_1_3_15_413 "AUDIO_DM_SetVolumeAndTypeEx, No device handle struct find!"
#define ADM_MNG_1985_112_2_18_1_3_15_414 "AUDIO_DM_SetVolumeAndTypeEx ori vol:%d, appType:%d, new vol:%d, appType:%d, handle:0x%x."
#define ADM_MNG_1998_112_2_18_1_3_15_415 "AUDIO_DM_SetVolumeAndTypeEx, set mode failed."
#define ADM_MNG_2021_112_2_18_1_3_15_416 "AUDIO_DM_GetVolume:eAppType:%d,uiVol:%d"
#define ADM_MNG_2046_112_2_18_1_3_15_417 "AUDIO_DM_SetUplinkMute set the same bUplinkMute:%d."
#define ADM_MNG_2122_112_2_18_1_3_15_418 "AUDIO_DM_SetDownlinkMute set the same bDownlinkMute:%d."
#define ADM_MNG_2251_112_2_18_1_3_15_419 "AUDIO_DM_GetDownLinkLocalNetMute input para error:0x%x, 0x%x."
#define ADM_MNG_2278_112_2_18_1_3_15_420 "AUDIO_DM_SetDigitalGain set the same uiGain:%d."
#define ADM_MNG_2354_112_2_18_1_3_15_421 "AUDIO_DM_SetDataOutputChannelChoice set the same eChannelChoice:%d."
#define ADM_MNG_2427_112_2_18_1_3_16_422 "adm_mng.c:AUDIO_DM_AddOpenDeviceIntoADM:error:no free device space to add"
#define ADM_MNG_2453_112_2_18_1_3_16_423 "adm_mng.c:AUDIO_DM_AddOpenDeviceIntoADM:error:can't find device to remove"
#define ADM_MNG_2568_112_2_18_1_3_16_424 "AUDIO_DM_GetAppropriateOutputSamplerate device:0x%x, output samplerate %d is not support."
#define APM_CODEC_518_112_2_18_1_3_17_425 "AUDIO_PM_AddCodecExpress failed. the express 0x%x is in the exp list of this codec:0x%x."
#define APM_CODEC_598_112_2_18_1_3_17_426 "the express be deleted doesn't exist in the codec express list."
#define APM_CODEC_612_112_2_18_1_3_18_427 "the express be deleted doesn't exist in the codec express list."
#define APM_CODEC_1020_112_2_18_1_3_19_428 "no matching codec ext operation in AUDIO_PM_ExeCodecExtOpe."
#define APM_CODEC_1032_112_2_18_1_3_19_429 "ptOneExtOpe->uiExtOpePointer is PNULL in AUDIO_PM_ExeCodecExtOpe."
#define APM_CODEC_1044_112_2_18_1_3_19_430 "apm_codec.c: AUDIO_PM_ExeCodecExtOpe:error:sig_ptr == SCI_NULL"
#define APM_CODEC_1050_112_2_18_1_3_19_431 "AUDIO_PM_ExeCodecExtOpe handle:0x%x, symbol:%s."
#define APM_CODEC_1244_112_2_18_1_3_19_432 "not any reg codec plugger."
#define APM_CODEC_1249_112_2_18_1_3_19_433 "para - codec Handle error."
#define APM_CODEC_1265_112_2_18_1_3_19_434 "not such reg codec plugger match the para."
#define APM_CODEC_1297_112_2_18_1_3_19_435 "not any reg codec plugger."
#define APM_CODEC_1302_112_2_18_1_3_19_436 "usCodecName is SCI_NULL."
#define APM_CODEC_1318_112_2_18_1_3_19_437 "not such reg codec plugger match the para."
#define APM_EXPRESS_389_112_2_18_1_3_20_438 "AUDIO_PM_ExeExpOpe not inited, 0x%x."
#define APM_EXPRESS_443_112_2_18_1_3_21_439 "AUDIO_PM_ExeExpSetParaOpe AUDIO_NO_EXPHANDLE."
#define APM_EXPRESS_449_112_2_18_1_3_21_440 "AUDIO_PM_ExeExpSetParaOpe AUDIO_NO_EXPHANDLE."
#define APM_EXPRESS_496_112_2_18_1_3_21_441 "AUDIO_PM_ExeExpSetParaOpe AUDIO_NO_EXPHANDLE."
#define APM_EXPRESS_502_112_2_18_1_3_21_442 "AUDIO_PM_ExeExpSetParaOpe AUDIO_NO_EXPHANDLE."
#define APM_EXPRESS_544_112_2_18_1_3_21_443 "AUDIO_PM_ExeExpInitParaOpe AUDIO_NO_EXPHANDLE 1."
#define APM_EXPRESS_550_112_2_18_1_3_21_444 "AUDIO_PM_ExeExpInitParaOpe AUDIO_NO_EXPHANDLE 2."
#define APM_EXPRESS_556_112_2_18_1_3_21_445 "AUDIO_PM_ExeExpInitParaOpe not working, 0x%x."
#define APM_EXPRESS_599_112_2_18_1_3_21_446 "AUDIO_PM_ExeExpDeInitParaOpe AUDIO_NO_EXPHANDLE 1."
#define APM_EXPRESS_605_112_2_18_1_3_21_447 "AUDIO_PM_ExeExpDeInitParaOpe AUDIO_NO_EXPHANDLE 2."
#define APM_EXPRESS_611_112_2_18_1_3_21_448 "AUDIO_PM_ExeExpDeInitParaOpe not working, 0x%x."
#define APM_EXPRESS_736_112_2_18_1_3_21_449 "not any reg express plugger."
#define APM_EXPRESS_778_112_2_18_1_3_21_450 "not any reg express plugger."
#define APM_EXPRESS_794_112_2_18_1_3_21_451 "no matching reg express plugger."
#define ASM_HC_165_112_2_18_1_3_22_452 "%s, %d, audio handle is invalid, hAudioHandle:0x%08x."
#define ASM_HC_171_112_2_18_1_3_22_453 "%s, %d, audio handle is invalid, hAudioHandle:0x%08x."
#define ASM_HC_177_112_2_18_1_3_22_454 "%s, %d, audio handle is invalid, hAudioHandle:0x%08x."
#define ASPEC_CALC_50_112_2_18_1_3_23_455 "AUDIO:AUDIO_SPEC_Open hAudioHandle:0x%08x"
#define ASPEC_CALC_56_112_2_18_1_3_23_456 "%s, %d, error. hAudioHandle:0x%08x."
#define ASPEC_CALC_64_112_2_18_1_3_23_457 "%s, %d, error. hAudioHandle:0x%08x."
#define ASPEC_CALC_83_112_2_18_1_3_23_458 "AUDIO:AUDIO_SPEC_Close hAudioHandle:0x%08x"
#define ASPEC_CALC_89_112_2_18_1_3_23_459 "%s, %d, error. hAudioHandle:0x%08x."
#define ASPEC_CALC_96_112_2_18_1_3_23_460 "%s, %d, error. hAudioHandle:0x%08x."
#define ASPEC_CALC_131_112_2_18_1_3_23_461 "%s, %d, error. hAudioHandle:0x%08x."
#define ASPEC_CALC_138_112_2_18_1_3_23_462 "%s, %d, error. hAudioHandle:0x%08x."
#define ASPEC_CALC_145_112_2_18_1_3_23_463 "%s, %d,the caculator is already closed! "
#define ASPEC_CALC_152_112_2_18_1_3_23_464 "%s, %d, error. puiSpecBuf:0x%08x."
#define ASPEC_CALC_161_112_2_18_1_3_23_465 "%s, %d, error. sample_num:%d."
#define ASM_MAIN_316_112_2_18_1_3_24_466 "_AUDIO_Enable_DumpPcmData_InMusicPlay error. hAudioHandle:0x%08x."
#define ASM_MAIN_332_112_2_18_1_3_24_467 "_AUDIO_Enable_DumpPcmData_InMusicPlay appTaskPri(%d) is lower than codecTaskPri(%d). hAudioHandle:0x%08x."
#define ASM_MAIN_340_112_2_18_1_3_24_468 "_AUDIO_Enable_DumpPcmData_InMusicPlay audio handle type: %d. hAudioHandle:0x%08x."
#define ASM_MAIN_347_112_2_18_1_3_24_469 "_AUDIO_Enable_DumpPcmData_InMusicPlay mode:%d."
#define ASM_MAIN_354_112_2_18_1_3_25_470 "_AUDIO_Enable_DumpPcmData_InMusicPlay, alloc fail, size: %d"
#define ASM_MAIN_361_112_2_18_1_3_25_471 "_AUDIO_Enable_DumpPcmData_InMusicPlay failed to get mode param."
#define ASM_MAIN_370_112_2_18_1_3_25_472 "_AUDIO_Enable_DumpPcmData_InMusicPlay failed to AUDIO_GetDebugOutputDataFileCount."
#define ASM_MAIN_377_112_2_18_1_3_25_473 "audio_api.c:_AUDIO_Enable_DumpPcmData_InMusicPlay, codecHandle:0x%x, expHandle:0x%x, i:0."
#define ASM_MAIN_381_112_2_18_1_3_25_474 "audio_api.c:_AUDIO_Enable_DumpPcmData_InMusicPlay:expHandle:0x%x, i:%d."
#define ASM_MAIN_397_112_2_18_1_3_25_475 "_AUDIO_Enable_DumpPcmData_InMusicPlay don't  dump pcm data, usDebugFileCount:%d, Config:0x%x."
#define ASM_MAIN_425_112_2_18_1_3_25_476 "_AUDIO_Enable_DumpPcmData_InMusicPlay don't support other file system, config:0x%x."
#define ASM_MAIN_443_112_2_18_1_3_25_477 "_AUDIO_Enable_DumpPcmData_InMusicPlay dump pcm data, usDebugFileCount:%d, Config:0x%x, usmask:0x%x."
#define ASM_MAIN_447_112_2_18_1_3_25_478 "_AUDIO_Enable_DumpPcmData_InMusicPlay failed to AUDIO_OfferDebugDataFileName."
#define ASM_MAIN_463_112_2_18_1_3_25_479 "_AUDIO_Enable_DumpPcmData_InMusicPlay dump pcm data, usDebugFileCount:%d, Config:0x%x, usmask:0x%x."
#define ASM_MAIN_467_112_2_18_1_3_25_480 "_AUDIO_Enable_DumpPcmData_InMusicPlay failed to AUDIO_OfferDebugDataFileName."
#define ASM_MAIN_472_112_2_18_1_3_25_481 "_AUDIO_Enable_DumpPcmData_InMusicPlay don't support other file system, config:0x%x."
#define ASM_MAIN_601_112_2_18_1_3_25_482 "AUDIO:AUDIO_SM_CreateFileHandle hAudioCodec:0x%08x"
#define ASM_MAIN_684_112_2_18_1_3_25_483 "asm_main.c:AUDIO_SM_CreateFileHandle:error:new_item == SCI_NULL"
#define ASM_MAIN_762_112_2_18_1_3_25_484 "asm_main.c:AUDIO_SM_CreateFileHandle:error:new_item == SCI_NULL"
#define ASM_MAIN_835_112_2_18_1_3_25_485 "AUDIO:AUDIO_SM_CreateFileHandle audio handle:0x%08x"
#define ASM_MAIN_887_112_2_18_1_3_26_486 "AUDIO:AUDIO_SM_CreateBufferHandle hAudioCodec:0x%08x"
#define ASM_MAIN_952_112_2_18_1_3_26_487 "asm_main.c:AUDIO_SM_CreateBufferHandle:error:new_item == SCI_NULL"
#define ASM_MAIN_1025_112_2_18_1_3_26_488 "asm_main.c:AUDIO_SM_CreateBufferHandle:error:new_item == SCI_NULL"
#define ASM_MAIN_1093_112_2_18_1_3_26_489 "AUDIO:AUDIO_SM_CreateBufferHandle audio handle:0x%08x"
#define ASM_MAIN_1149_112_2_18_1_3_26_490 "AUDIO:AUDIO_SM_CreateStreamBufferHandle hAudioCodec:0x%08x"
#define ASM_MAIN_1213_112_2_18_1_3_26_491 "asm_main.c:AUDIO_SM_CreateStreamBufferHandle:error:new_item == SCI_NULL"
#define ASM_MAIN_1287_112_2_18_1_3_26_492 "asm_main.c:AUDIO_SM_CreateStreamBufferHandle:error:new_item == SCI_NULL"
#define ASM_MAIN_1356_112_2_18_1_3_26_493 "AUDIO:AUDIO_SM_CreateStreamBufferHandle audio handle:0x%08x"
#define ASM_MAIN_1379_112_2_18_1_3_27_494 "AUDIO:AUDIO_SM_CloseHandle hAudioHandle:0x%08x"
#define ASM_MAIN_1414_112_2_18_1_3_27_495 "%s, %d, codec_handle:0x%x."
#define ASM_MAIN_1423_112_2_18_1_3_27_496 "AUDIO_SM_CloseHandle pDeconstruct error:%d."
#define ASM_MAIN_1517_112_2_18_1_3_27_497 "asm_main.c:AUDIO_SM_CloseHandle:error:The audio link list is NULL"
#define ASM_MAIN_1549_112_2_18_1_3_27_498 "AUDIO:AUDIO_SM_Play hAudioHandle:0x%08x"
#define ASM_MAIN_1557_112_2_18_1_3_27_499 "%s, %d, error. hAudioHandle:0x%08x."
#define ASM_MAIN_1564_112_2_18_1_3_27_500 "%s, %d, error. hAudioHandle:0x%08x."
#define ASM_MAIN_1578_112_2_18_1_3_27_501 "%s, %d, input para error. hAudioHandle:0x%08x. offset:%d, file_total_len:%d."
#define ASM_MAIN_1612_112_2_18_1_3_27_502 "AUDIO:AUDIO_SM_Play sample_rate:%d"
#define ASM_MAIN_1623_112_2_18_1_3_27_503 "AUDIO:AUDIO_SM_Play device appropriate sample_rate:%d, device:0x%x."
#define ASM_MAIN_1660_112_2_18_1_3_27_504 "AUDIO:AUDIO_SM_Play track_num:%d"
#define ASM_MAIN_1673_112_2_18_1_3_27_505 "AUDIO_SM_Play AUDIO_SM_InitExpPara error:%d."
#define ASM_MAIN_1702_112_2_18_1_3_27_506 "audio:0x%x, ASM:uiCodecHighPriority = %d,uiCodecLowPriority = %d, track_on_point:%d."
#define ASM_MAIN_1713_112_2_18_1_3_27_507 "asm_main.c:AUDIO_SM_Play:error:SCI_NULL == audio_obj->ptTrackBufTimer"
#define ASM_MAIN_1732_112_2_18_1_3_27_508 "asm_main.c:AUDIO_SM_Play:error:SCI_NULL == audio_obj->ptTrackOnTimer"
#define ASM_MAIN_1751_112_2_18_1_3_27_509 "%s, %d, error. hAudioHandle:0x%08x, hCodecHandle:0x%08x."
#define ASM_MAIN_1770_112_2_18_1_3_27_510 "AUDIO:AUDIO_SM_Play result1:%d"
#define ASM_MAIN_1778_112_2_18_1_3_27_511 "asm_main.c:AUDIO_SM_Play:error:SCI_ERROR == SCI_ChangeThreadPriority"
#define ASM_MAIN_1790_112_2_18_1_3_27_512 "AUDIO_SM_Play, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d, time:%d."
#define ASM_MAIN_1795_112_2_18_1_3_27_513 "asm_main.c:AUDIO_SM_Play:assiTask error:SCI_ERROR == SCI_ChangeThreadPriority"
#define ASM_MAIN_1807_112_2_18_1_3_27_514 "AUDIO_SM_Play, success to ChangeThreadPriority assiTask:src_pri:%d, dest_pri:%d, time:%d."
#define ASM_MAIN_1839_112_2_18_1_3_27_515 "AUDIO_SM_Play pStopDeconstruct error:%d."
#define ASM_MAIN_1863_112_2_18_1_3_27_516 "AUDIO:AUDIO_SM_Play result2:%d"
#define ASM_MAIN_1876_112_2_18_1_3_27_517 "asm_main.c:AUDIO_SM_PauseEnd:error:audio_obj == SCI_NULL"
#define ASM_MAIN_1885_112_2_18_1_3_27_518 "asm_main.c:AUDIO_SM_StopSelf:error:sig_out_ptr == SCI_NULL"
#define ASM_MAIN_1891_112_2_18_1_3_27_519 "AUDIO_SM_StopSelf  hAudioHandle:0x%08x"
#define ASM_MAIN_1925_112_2_18_1_3_28_520 "AUDIO:AUDIO_SM_Stop hAudioHandle:0x%08x"
#define ASM_MAIN_1957_112_2_18_1_3_28_521 "%s, %d, codec_handle:0x%x."
#define ASM_MAIN_1963_112_2_18_1_3_28_522 "%s, %d, codec_handle:0x%x."
#define ASM_MAIN_1973_112_2_18_1_3_28_523 "asm_main.c:AUDIO_SM_Stop:error:SCI_ERROR == SCI_ChangeThreadPriority"
#define ASM_MAIN_1981_112_2_18_1_3_28_524 "AUDIO_SM_Stop, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d."
#define ASM_MAIN_1986_112_2_18_1_3_28_525 "asm_main.c:AUDIO_SM_Stop:error:SCI_ERROR == SCI_ChangeThreadPriority 2"
#define ASM_MAIN_1991_112_2_18_1_3_28_526 "AUDIO_SM_Stop, success to ChangeThreadPriority assi task:src_pri:%d, dest_pri:%d."
#define ASM_MAIN_2011_112_2_18_1_3_28_527 "AUDIO_SM_Stop pStopDeconstruct error:%d."
#define ASM_MAIN_2051_112_2_18_1_3_28_528 "AUDIO_SM_Stop, stop fail, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d."
#define ASM_MAIN_2061_112_2_18_1_3_28_529 "AUDIO_SM_Stop, stop fail, success to ChangeThreadPriority: assiTask src_pri:%d, dest_pri:%d."
#define ASM_MAIN_2098_112_2_18_1_3_28_530 "AUDIO:AUDIO_SM_Pause hAudioHandle:0x%08x"
#define ASM_MAIN_2128_112_2_18_1_3_28_531 "%s, %d, codec_handle:0x%x."
#define ASM_MAIN_2134_112_2_18_1_3_28_532 "%s, %d, codec_handle:0x%x."
#define ASM_MAIN_2144_112_2_18_1_3_28_533 "asm_main.c:AUDIO_SM_Pause:error:SCI_ERROR == SCI_ChangeThreadPriority"
#define ASM_MAIN_2151_112_2_18_1_3_28_534 "AUDIO_SM_Pause, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d."
#define ASM_MAIN_2156_112_2_18_1_3_28_535 "asm_main.c:AUDIO_SM_Pause:error:SCI_ERROR == SCI_ChangeThreadPriority assiTask."
#define ASM_MAIN_2163_112_2_18_1_3_28_536 "AUDIO_SM_Pause, success to ChangeThreadPriority assiTask:src_pri:%d, dest_pri:%d."
#define ASM_MAIN_2182_112_2_18_1_3_28_537 "asm_main.c:AUDIO_SM_Pause:error:SCI_ERROR  result = gen_operation->pPauseDec"
#define ASM_MAIN_2220_112_2_18_1_3_28_538 "AUDIO_SM_Pause, pause fail.success to ChangeThreadPriority:src_pri:%d, dest_pri:%d."
#define ASM_MAIN_2230_112_2_18_1_3_28_539 "AUDIO_SM_Pause, pause fail. success to ChangeThreadPriority assiTask:src_pri:%d, dest_pri:%d."
#define ASM_MAIN_2261_112_2_18_1_3_28_540 "AUDIO:AUDIO_SM_Resume hAudioHandle:0x%08x"
#define ASM_MAIN_2301_112_2_18_1_3_28_541 "AUDIO:AUDIO_SM_Resume sample_rate:%d"
#define ASM_MAIN_2312_112_2_18_1_3_28_542 "AUDIO:AUDIO_SM_Resume device appropriate sample_rate:%d, device:0x%x."
#define ASM_MAIN_2333_112_2_18_1_3_28_543 "AUDIO_SM_Resume AUDIO_SM_InitExpPara error:%d."
#define ASM_MAIN_2352_112_2_18_1_3_28_544 "AUDIO_SM_Resume pResumeConstruct error:%d."
#define ASM_MAIN_2381_112_2_18_1_3_28_545 "AUDIO_SM_Resume, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d, time:%d."
#define ASM_MAIN_2392_112_2_18_1_3_28_546 "AUDIO_SM_Resume, success to ChangeThreadPriority assiTask:src_pri:%d, dest_pri:%d, time:%d."
#define ASM_MAIN_2416_112_2_18_1_3_29_547 "asm_main.c:AUDIO_SM_Resume:error:SCI_ERROR  result  is %d"
#define ASM_MAIN_2492_112_2_18_1_3_29_548 "AUDIO_SM_TimeSeek: seek to end!"
#define ASM_MAIN_2498_112_2_18_1_3_29_549 "%s, %d, input para error. hAudioHandle:0x%08x. offset:%d, file_total_len:%d."
#define ASM_MAIN_2516_112_2_18_1_3_29_550 "AUDIO_SM_TimeSeek: gen_operation->pSeek not exist!"
#define ASM_MAIN_2568_112_2_18_1_3_29_551 "AUDIO_SM_Seek: seek to end!"
#define ASM_MAIN_2574_112_2_18_1_3_29_552 "%s, %d, input para error. hAudioHandle:0x%08x. offset:%d, file_total_len:%d."
#define ASM_MAIN_2808_112_2_18_1_3_29_553 "%s, %d, audio_handle:0x%x, playing_state:%d, audio is not playing."
#define ASM_MAIN_2853_112_2_18_1_3_29_554 "asm_main.c:AUDIO_SM_IdentifyAudioFormat:error:ptFormatInfo == SCI_NULL"
#define ASM_MAIN_2921_112_2_18_1_3_29_555 "AUDIO_SM_GetPlayingInfo, audio is not playing, state:%d."
#define ASM_MAIN_3136_112_2_18_1_3_30_556 "asm_main.c:AUDIO_SM_GetFileSize:error:PNULL==audio_obj"
#define ASM_MAIN_3250_112_2_18_1_3_30_557 "%s, %d, uiSamplere==0."
#define ASM_MAIN_3267_112_2_18_1_3_30_558 "AUDIO_SM_SetOutputSampleRate handle:0x%x, samplerate:%d."
#define ASM_MAIN_3406_112_2_18_1_3_30_559 "AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x."
#define ASM_MAIN_3431_112_2_18_1_3_30_560 "AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 1."
#define ASM_MAIN_3445_112_2_18_1_3_31_561 "AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 2."
#define ASM_MAIN_3455_112_2_18_1_3_31_562 "AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 3 ."
#define ASM_MAIN_3467_112_2_18_1_3_31_563 "AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 4."
#define ASM_MAIN_3474_112_2_18_1_3_31_564 "AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 5."
#define ASM_MAIN_3493_112_2_18_1_3_31_565 "AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 6."
#define ASM_MAIN_3495_112_2_18_1_3_31_566 "asm_main.c:AUDIO_SM_CreateRecordFileHandle:error:new_item == SCI_NULL"
#define ASM_MAIN_3505_112_2_18_1_3_31_567 "AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 7."
#define ASM_MAIN_3560_112_2_18_1_3_31_568 "AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 8."
#define ASM_MAIN_3574_112_2_18_1_3_31_569 "AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 9."
#define ASM_MAIN_3576_112_2_18_1_3_31_570 "asm_main.c:AUDIO_SM_CreateRecordFileHandle:error:new_item == SCI_NULL"
#define ASM_MAIN_3586_112_2_18_1_3_31_571 "AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 10."
#define ASM_MAIN_3641_112_2_18_1_3_31_572 "AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 11."
#define ASM_MAIN_3702_112_2_18_1_3_31_573 "AUDIO:AUDIO_SM_CreateRecordBufferHandle hAudioCodec:0x%08x"
#define ASM_MAIN_3766_112_2_18_1_3_31_574 "asm_main.c:AUDIO_SM_CreateRecordBufferHandle:error:new_item == SCI_NULL"
#define ASM_MAIN_3838_112_2_18_1_3_31_575 "asm_main.c:AUDIO_SM_CreateRecordBufferHandle:error:new_item == SCI_NULL"
#define ASM_MAIN_3943_112_2_18_1_3_31_576 "AUDIO:AUDIO_SM_CreateNormalHandle hAudioCodec:0x%08x"
#define ASM_MAIN_4001_112_2_18_1_3_32_577 "asm_main.c:AUDIO_SM_CreateNormalHandle:error:new_item == SCI_NULL"
#define ASM_MAIN_4073_112_2_18_1_3_32_578 "asm_main.c:AUDIO_SM_CreateNormalHandle:error:new_item == SCI_NULL"
#define ASM_MAIN_4162_112_2_18_1_3_32_579 "AUDIO:AUDIO_SM_CreateCloneHandle hAudioCodec:0x%08x, OrgHdl:%08X"
#define ASM_MAIN_4220_112_2_18_1_3_32_580 "asm_main.c:AUDIO_SM_CreateNormalHandle:error:new_item == SCI_NULL"
#define ASM_MAIN_4292_112_2_18_1_3_32_581 "asm_main.c:AUDIO_SM_CreateNormalHandle:error:new_item == SCI_NULL"
#define ASM_MAIN_4364_112_2_18_1_3_32_582 "[AUDIO_SM_CreateCloneHandle] org output:%d, codec out %d."
#define ASM_MAIN_4403_112_2_18_1_3_32_583 "AUDIO_SM_SwitchDevice hAudioHandle:0x%x, hDevice:0x%x."
#define ASM_MAIN_4438_112_2_18_1_3_32_584 "AUDIO_SM_SwitchDevice srcDevice == destDevice:0x%x."
#define ASM_MAIN_4453_112_2_18_1_3_32_585 "AUDIO_SM_SwitchDevice AUDIO_DM_SwitchDevice failed."
#define ASM_MAIN_4535_112_2_18_1_3_33_586 "asm_main.c:AUDIO_SM_GetDecoderDataTime:error:audio_obj->uiOutputSamplerate==0"
#define ASM_MAIN_4558_112_2_18_1_3_33_587 "%s, %d, thread_id is invalid."
#define ASM_MAIN_4583_112_2_18_1_3_33_588 "%s, %d, thread_id is invalid."
#define ASM_MAIN_4620_112_2_18_1_3_33_589 "asm_main.c:AUDIO_SM_PlayEnd:error:audio_obj == SCI_NULL"
#define ASM_MAIN_4629_112_2_18_1_3_33_590 "asm_main.c:AUDIO_SM_PlayEnd:error:sig_out_ptr == SCI_NULL"
#define ASM_MAIN_4635_112_2_18_1_3_33_591 "AUDIO_SM_PlayEnd hAudioHandle:0x%08x"
#define ASM_MAIN_4672_112_2_18_1_3_33_592 "asm_main.c:AUDIO_SM_PauseEnd:error:audio_obj == SCI_NULL"
#define ASM_MAIN_4682_112_2_18_1_3_33_593 "asm_main.c:AUDIO_SM_PauseEnd:error:sig_out_ptr == SCI_NULL"
#define ASM_MAIN_4723_112_2_18_1_3_33_594 "asm_main.c:AUDIO_SM_SendErrorRep:error:audio_obj == SCI_NULL"
#define ASM_MAIN_4732_112_2_18_1_3_33_595 "asm_main.c:AUDIO_SM_SendErrorRep:error:sig_out_ptr == SCI_NULL"
#define ASM_MAIN_4776_112_2_18_1_3_33_596 "AUDIO_SM_AllocMemory, ptr:0x%08x, len:0x%x."
#define ASM_MAIN_4833_112_2_18_1_3_33_597 "asm_main.c:thread_entry:AUDIO_CREATE_FILE_REQ::temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_4880_112_2_18_1_3_33_598 "asm_main.c:thread_entry:AUDIO_CREATE_BUFFER_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_4928_112_2_18_1_3_33_599 "asm_main.c:thread_entry:AUDIO_CREATE_STREAMBUFFER_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_4977_112_2_18_1_3_34_600 "asm_main.c:thread_entry:AUDIO_CREATE_RECORD_FILE_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_5024_112_2_18_1_3_34_601 "asm_main.c:thread_entry:AUDIO_CREATE_RECORD_BUFFER_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_5066_112_2_18_1_3_34_602 "asm_main.c:thread_entry:AUDIO_CREATE_NORMAL_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_5109_112_2_18_1_3_34_603 "asm_main.c:thread_entry:AUDIO_CREATE_CLONE_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_5137_112_2_18_1_3_34_604 "ASM:AUDIO_PLAY_REQ handdle 0x%08x not existed!\r\n"
#define ASM_MAIN_5162_112_2_18_1_3_34_605 "asm_main.c:thread_entry:AUDIO_PLAY_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_5187_112_2_18_1_3_34_606 "ASM:AUDIO_STOP_REQ handdle 0x%08xnot existed!\r\n"
#define ASM_MAIN_5219_112_2_18_1_3_34_607 "asm_main.c:thread_entry:AUDIO_STOP_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_5253_112_2_18_1_3_34_608 "ASM:AUDIO_PAUSE_REQ handdle 0x%08x not existed!\r\n"
#define ASM_MAIN_5285_112_2_18_1_3_34_609 "asm_main.c:thread_entry:AUDIO_PAUSE_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_5319_112_2_18_1_3_34_610 "ASM:AUDIO_RESUME_REQ handdle 0x%08x not existed!\r\n"
#define ASM_MAIN_5344_112_2_18_1_3_34_611 "asm_main.c:thread_entry:AUDIO_RESUME_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_5369_112_2_18_1_3_34_612 "ASM:AUDIO_SEEK_REQ handdle 0x%08x not existed!\r\n"
#define ASM_MAIN_5394_112_2_18_1_3_34_613 "asm_main.c:thread_entry:AUDIO_SEEK_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_5419_112_2_18_1_3_34_614 "ASM:AUDIO_SETSPEED_REQ handdle 0x%08x not existed!\r\n"
#define ASM_MAIN_5444_112_2_18_1_3_34_615 "asm_main.c:thread_entry:AUDIO_SETSPEED_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_5469_112_2_18_1_3_34_616 "ASM:AUDIO_SETDIGIGAIN_REQ handdle 0x%08xnot existed!\r\n"
#define ASM_MAIN_5494_112_2_18_1_3_35_617 "asm_main.c:thread_entry:AUDIO_SETDIGIGAIN_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_5520_112_2_18_1_3_35_618 "ASM:AUDIO_GETDIGIGAIN_REQ handdle 0x%08x not existed!\r\n"
#define ASM_MAIN_5555_112_2_18_1_3_35_619 "asm_main.c:thread_entry:AUDIO_GETDIGIGAIN_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_5594_112_2_18_1_3_35_620 "AS: Received AUDIO_ERROR_RSP!"
#define ASM_MAIN_5615_112_2_18_1_3_35_621 "asm_main.c:thread_entry:AUDIO_ERROR_REP:error:ptAudio == SCI_NULL"
#define ASM_MAIN_5622_112_2_18_1_3_35_622 "asm_main.c:thread_entry:AUDIO_ERROR_REP::ptAudio->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_5635_112_2_18_1_3_35_623 "asm_main.c:thread_entry:AUDIO_ERROR_REP:error:verify handle error"
#define ASM_MAIN_5641_112_2_18_1_3_35_624 "AS: Received AUDIO_PLAYEND_RSP!"
#define ASM_MAIN_5657_112_2_18_1_3_35_625 "asm_main.c:thread_entry:AUDIO_PLAYEND_REP:error:verify handle error"
#define ASM_MAIN_5665_112_2_18_1_3_35_626 "asm_main.c:thread_entry:AUDIO_PLAYEND_REP:error:ptAudio == SCI_NULL"
#define ASM_MAIN_5671_112_2_18_1_3_35_627 "AUDIO_PLAYEND_REP, AUDIO_PLAYING_FLAG_STOPPED == ptAudio->ePlayingFlag"
#define ASM_MAIN_5677_112_2_18_1_3_35_628 "AUDIO_PLAYEND_REP, AUDIO_PLAYING_FLAG_STARTPAUSE == ptAudio->ePlayingFlag"
#define ASM_MAIN_5680_112_2_18_1_3_35_629 "AUDIO_PLAYEND_REP hAudioHandle:0x%08x"
#define ASM_MAIN_5690_112_2_18_1_3_35_630 "asm_main.c:thread_entry:AUDIO_PLAYEND_REP:error:codec_handle == INVALID_HANDLE"
#define ASM_MAIN_5704_112_2_18_1_3_35_631 "AUDIO_PLAYEND_REP pStopDeconstruct error:%d."
#define ASM_MAIN_5712_112_2_18_1_3_35_632 "asm_main.c:thread_entry:AUDIO_PLAYEND_REP:error:gen_operation != SCI_NULL"
#define ASM_MAIN_5748_112_2_18_1_3_35_633 "asm_main.c:thread_entry:AUDIO_PLAYEND_REP:SCI_NULL == ptAudio->pNotifyCallbackFunc"
#define ASM_MAIN_5760_112_2_18_1_3_35_634 "asm_main.c:thread_entry:AUDIO_PLAYEND_REP:SCI_NULL == ptAudio->pNotifyCallbackFunc"
#define ASM_MAIN_5785_112_2_18_1_3_35_635 "AS: Received AUDIO_PAUSEEND_RSP!"
#define ASM_MAIN_5801_112_2_18_1_3_35_636 "asm_main.c:thread_entry:AUDIO_PAUSEEND_REP:error:verify handle error"
#define ASM_MAIN_5809_112_2_18_1_3_35_637 "asm_main.c:thread_entry:AUDIO_PAUSEEND_REP:error:ptAudio == SCI_NULL"
#define ASM_MAIN_5822_112_2_18_1_3_35_638 "asm_main.c:thread_entry:AUDIO_PAUSEEND_REP:error:ptAudio == SCI_NULL"
#define ASM_MAIN_5838_112_2_18_1_3_35_639 "asm_main.c:thread_entry:AUDIO_PAUSEEND_REP:error:result = gen_operation->pPauseDeconstruct"
#define ASM_MAIN_5847_112_2_18_1_3_35_640 "asm_main.c:thread_entry:AUDIO_PAUSEEND_REP:error:gen_operation != SCI_NULL"
#define ASM_MAIN_5880_112_2_18_1_3_35_641 "asm_main.c:thread_entry:AUDIO_PAUSEEND_REP:SCI_NULL == ptAudio->pNotifyCallbackFunc"
#define ASM_MAIN_5905_112_2_18_1_3_35_642 "ASM:AUDIO_CLOSE_HANDLE_REQ handdle 0x%08x  is not existed!\r\n"
#define ASM_MAIN_5930_112_2_18_1_3_35_643 "asm_main.c:thread_entry:AUDIO_CLOSE_HANDLE_REQ:SCI_NULL == ptAudio->pNotifyCallbackFunc"
#define ASM_MAIN_5959_112_2_18_1_3_35_644 "ASM:AUDIO_GETDIGIGAIN_REQ handdle 0x%08x not existed!\r\n"
#define ASM_MAIN_5989_112_2_18_1_3_36_645 "asm_main.c:thread_entry:AUDIO_SWITCH_DEVICE_REQ:SCI_NULL == ptAudio->pNotifyCallbackFunc"
#define ASM_MAIN_6012_112_2_18_1_3_36_646 "asm_main.c:thread_entry:AUDIO_SWITCH_DEVICE_REQ:error:verify handle error"
#define ASM_MAIN_6029_112_2_18_1_3_36_647 "ASM:Receive the Trackbuf full signal so later!\r\n"
#define ASM_MAIN_6049_112_2_18_1_3_36_648 "asm_main.c:thread_entry:AUDIO_TRACKBUF_STATUS_REP:ptAudio->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_6061_112_2_18_1_3_36_649 "ASM:Trackbuf Empty,SCI_ChangeThreadPriority! "
#define ASM_MAIN_6070_112_2_18_1_3_36_650 "asm_main.c:thread_entry:AUDIO_TRACKBUF_STATUS_REP:ptAudio->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_6083_112_2_18_1_3_36_651 "AS: Received AUDIO_TRACKBUF_STATUS_RSP!"
#define ASM_MAIN_6210_112_2_18_1_3_36_652 "AS: Received AUDIO_CHANGE_CODEC_TASK_PRIORITY_REQ! time:%d, result:%d."
#define ASM_MAIN_6222_112_2_18_1_3_36_653 "AS: Received AUDIO_SET_TRACK_ON_REQ! audio:0x%08x, on:%d, result:%d"
#define ASM_MAIN_6250_112_2_18_1_3_36_654 "AUDIO_EXE_CODEC_EXT_OPE_REQ uiExtOpePointer APM_EXT_ENTER failed:%d."
#define ASM_MAIN_6254_112_2_18_1_3_36_655 "AudioThread: AUDIO_CM_ExitDispose 1 failed:%d."
#define ASM_MAIN_6260_112_2_18_1_3_36_656 "AudioThread: AUDIO_CM_EnterDispose failed:%d."
#define ASM_MAIN_6272_112_2_18_1_3_36_657 "AudioThread: AUDIO_CM_ExitDispose 2 failed:%d."
#define ASM_MAIN_6277_112_2_18_1_3_36_658 "AudioThread: uiExtOpePointer APM_EXT_EXIT failed:%d."
#define ASM_MAIN_6298_112_2_18_1_3_36_659 "ASM:AUDIO_SEEK_REQ handdle 0x%08x not existed!\r\n"
#define ASM_MAIN_6323_112_2_18_1_3_36_660 "asm_main.c:thread_entry:AUDIO_SEEK_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL"
#define ASM_MAIN_6347_112_2_18_1_3_36_661 "ASM:AUDIO_GETPLAYINGINFO_REQ handdle 0x%08x not existed!\r\n"
#define ASM_MAIN_6397_112_2_18_1_3_36_662 "asm_main.c:AUDIO_STOPSELF_IND process :error:SCI_ERROR == SCI_ChangeThreadPriority"
#define ASM_MAIN_6400_112_2_18_1_3_36_663 "AUDIO_SM_Stop, success to ChangeThreadPriority decode task:src_pri:%d, dest_pri:%d."
#define ASM_MAIN_6406_112_2_18_1_3_36_664 "asm_main.c:AUDIO_SM_Stop:error:SCI_ERROR == SCI_ChangeThreadPriority 2"
#define ASM_MAIN_6409_112_2_18_1_3_36_665 "AUDIO_SM_Stop, success to ChangeThreadPriority assi task:src_pri:%d, dest_pri:%d."
#define ASM_MAIN_6420_112_2_18_1_3_36_666 "asm_main.c:AUDIO_STOPSELF_IND process :error:SCI_ERROR == SCI_ChangeThreadPriority"
#define ASM_MAIN_6423_112_2_18_1_3_36_667 "AUDIO_SM_Stop, success to ChangeThreadPriority decode task:src_pri:%d, dest_pri:%d."
#define ASM_MAIN_6429_112_2_18_1_3_36_668 "asm_main.c:AUDIO_SM_Stop:error:SCI_ERROR == SCI_ChangeThreadPriority 2"
#define ASM_MAIN_6432_112_2_18_1_3_36_669 "AUDIO_SM_Stop, success to ChangeThreadPriority assi task:src_pri:%d, dest_pri:%d."
#define ASM_MAIN_6487_112_2_18_1_3_37_670 "ASM:AUDIO_SM_TrackbufTimer_Callback!"
#define ASM_MAIN_6504_112_2_18_1_3_37_671 "asm_main.c:AUDIO_SM_TrackbufTimer_Callback:error:sig_out_ptr == SCI_NULL"
#define ASM_MAIN_6519_112_2_18_1_3_37_672 "ASM:Continue to Check Trackbuf!uiTimerExpire: %d.\r\n"
#define ASM_MAIN_6536_112_2_18_1_3_37_673 "asm_main.c:AUDIO_SM_TrackbufTimer_Callback:error:sig_out_ptr == SCI_NULL"
#define ASM_MAIN_6566_112_2_18_1_3_37_674 "ASM:AUDIO_SM_TrackOnTimer_Callback! time:%d."
#define ASM_MAIN_6577_112_2_18_1_3_37_675 "asm_main.c:AUDIO_SM_TrackOnTimer_Callback:error:sig_out_ptr == SCI_NULL"
#define ASM_MAIN_6614_112_2_18_1_3_37_676 "asm_main.c:AUDIO_SM_LookforAppropriateCodec:error:audio_obj == SCI_NULL"
#define ASM_MAIN_6640_112_2_18_1_3_37_677 "asm_main.c:AUDIO_SM_LookforAppropriateCodec:error:SCI_NULL == ptCodecExtCfgInfo"
#define ASM_MAIN_6657_112_2_18_1_3_37_678 "AUDIO_SM_LookforAppropriateCodec, construct error, codec handle:0x%08x."
#define ASM_MAIN_6671_112_2_18_1_3_37_679 "AUDIO_SM_LookforAppropriateCodec pDeconstruct error:%d."
#define ASM_MAIN_6711_112_2_18_1_3_37_680 "%s, %d, error."
#define ASM_MAIN_6721_112_2_18_1_3_37_681 "asm_mai.c:AUDIO_SM_IdentifyFormat:error:gen_operation_ptr==PNULL"
#define ASM_MAIN_6730_112_2_18_1_3_37_682 "asm_mai.c:AUDIO_SM_IdentifyFormat:error:ptCodecExtCfgInfo==PNULL"
#define ASM_MAIN_6749_112_2_18_1_3_37_683 "%s, %d, error. hAudioHandle:0x%08x, hCodecHandle:0x%08x."
#define ASM_MAIN_6755_112_2_18_1_3_37_684 "%s, %d, error. hAudioHandle:0x%08x."
#define ASM_MAIN_6770_112_2_18_1_3_37_685 "AUDIO_SM_IdentifyFormat pDeconstruct error:%d."
#define ASM_MAIN_6777_112_2_18_1_3_37_686 "AUDIO_SM_IdentifyFormat, not verify format, codec::0x%08x."
#define ASM_MAIN_6786_112_2_18_1_3_37_687 "AUDIO_SM_IdentifyFormat, can't find appropriate codec."
#define ASM_MAIN_6801_112_2_18_1_3_37_688 "AUDIO_SM_IdentifyFormat, handle:0x%08x, total_len:%d, total_time:%d."
#define ASM_MAIN_6811_112_2_18_1_3_37_689 "%s, %d, uiOutputSamplerate is zero, codec handle:0x%x."
#define ASM_MAIN_6951_112_2_18_1_3_37_690 "%s, %d, error. hAudioHandle:0x%08x."
#define ASM_MAIN_6958_112_2_18_1_3_37_691 "%s, %d, error. hAudioHandle:0x%08x."
#define ASM_MAIN_6985_112_2_18_1_3_38_692 "%s, %d, error. hAudioHandle:0x%08x."
#define ASM_MAIN_6992_112_2_18_1_3_38_693 "%s, %d, error. hAudioHandle:0x%08x."
#define ASM_MAIN_7024_112_2_18_1_3_38_694 "AUDIO:AUDIO_SM_ChangeCodecPriority hAudioHandle:0x%08x,bHighPriority:%d"
#define ASM_MAIN_7030_112_2_18_1_3_38_695 "%s, %d, error. hAudioHandle:0x%08x."
#define ASM_MAIN_7037_112_2_18_1_3_38_696 "%s, %d, error. hAudioHandle:0x%08x."
#define ASM_MAIN_7056_112_2_18_1_3_38_697 "AUDIO_SM_ChangeCodecPriority, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d."
#define ASM_MAIN_7070_112_2_18_1_3_38_698 "AUDIO_SM_ChangeCodecPriority, success to ChangeAssiThreadPriority:src_pri:%d, dest_pri:%d."
#define ASM_MAIN_7102_112_2_18_1_3_38_699 "AUDIO_SM_GetCodecPriorityBeHigh audio handle error. hAudioHandle:0x%08x."
#define ASM_MAIN_7109_112_2_18_1_3_38_700 "AUDIO_SM_GetCodecPriorityBeHigh audio handle not exist. hAudioHandle:0x%08x."
#define ASM_MAIN_7133_112_2_18_1_3_38_701 "AUDIO_SM_GetCodecPriorityBeHigh, codec task pri unnormal: %d, low:%d, high:%d, hAudioHandle:0x%08x."
#define ASM_MAIN_7139_112_2_18_1_3_38_702 "AUDIO_SM_GetCodecPriorityBeHigh, play state unnormal:%d. hAudioHandle:0x%08x."
#define ASM_MAIN_7163_112_2_18_1_3_38_703 "AUDIO_SM_PauseOutputData audio_obj is null."
#define ASM_MAIN_7168_112_2_18_1_3_38_704 "AUDIO_SM_PauseOutputData hAudioHandle:0x%08x"
#define ASM_MAIN_7181_112_2_18_1_3_38_705 "AUDIO_SM_PauseOutputData:error:sig_out_ptr == SCI_NULL"
#define ASM_MAIN_7222_112_2_18_1_3_38_706 "AUDIO_SM_GetDebugOutputDataFileCount pusFileCount is PNULL."
#define ASM_MAIN_7230_112_2_18_1_3_38_707 "AUDIO_SM_GetDebugOutputDataFileCount audio handle error. hAudioHandle:0x%08x."
#define ASM_MAIN_7237_112_2_18_1_3_38_708 "AUDIO_SM_GetDebugOutputDataFileCount audio handle not exist. hAudioHandle:0x%08x."
#define ASM_MAIN_7290_112_2_18_1_3_38_709 "AUDIO_SM_EnableSomePosDebugOutputDataToFile audio handle error. hAudioHandle:0x%08x."
#define ASM_MAIN_7297_112_2_18_1_3_38_710 "AUDIO_SM_EnableSomePosDebugOutputDataToFile audio handle not exist. hAudioHandle:0x%08x."
#define ASM_MAIN_7314_112_2_18_1_3_38_711 "AUDIO_SM_EnableSomePosDebugOutputDataToFile pos:%d should less than max:%d"
#define ASM_MAIN_7327_112_2_18_1_3_38_712 "AUDIO_SM_EnableSomePosDebugOutputDataToFile pbEnableDebugDataOutput create memory failed, sizeof:%d."
#define ASM_MAIN_7364_112_2_18_1_3_38_713 "AUDIO_SM_EnableOutputDataToFile audio handle error. hAudioHandle:0x%08x."
#define ASM_MAIN_7371_112_2_18_1_3_38_714 "AUDIO_SM_EnableOutputDataToFile audio handle not exist. hAudioHandle:0x%08x."
#define ASM_MAIN_7407_112_2_18_1_3_38_715 "AUDIO_SM_OfferDebugDataFileName VerifyHandle failed."
#define ASM_MAIN_7418_112_2_18_1_3_38_716 "AUDIO_SM_OfferDebugDataFileName audio_obj is null."
#define ASM_MAIN_7435_112_2_18_1_3_38_717 "AUDIO_SM_OfferDebugDataFileName the name has been offered."
#define ASM_MAIN_7464_112_2_18_1_3_38_718 "AUDIO_SM_OfferDebugDataFileName hFileHanlde create failed Left:%s."
#define ASM_MAIN_7478_112_2_18_1_3_38_719 "AUDIO_SM_OfferDebugDataFileName hFileHanlde create failed Right:%s."
#define ASM_MAIN_7485_112_2_18_1_3_38_720 "AUDIO_SM_OfferDebugDataFileName hFileHanlde create memory failed size:%d."
#define ASM_MAIN_7499_112_2_18_1_3_39_721 "AUDIO_SM_EnableSomePosDebugOutputDataToFile pbEnableDebugDataOutput create memory failed, sizeof:%d."
#define ASM_MAIN_7580_112_2_18_1_3_39_722 "AUDIO_SM_ConfigIfExpWorking exp:0x%x, ifWork:%d, index:%d, free 1."
#define ASM_MAIN_7589_112_2_18_1_3_39_723 "AUDIO_SM_ConfigIfExpWorking exp:0x%x, ifWork:%d, index:%d, free 2."
#define ASM_MAIN_7613_112_2_18_1_3_39_724 "AUDIO_SM_ConfigIfExpWorking exp:0x%x, ifWork:%d, index:%d, add 1."
#define ASM_MAIN_7618_112_2_18_1_3_39_725 "AUDIO_SM_ConfigIfExpWorking alloc error 1, size:%d."
#define ASM_MAIN_7640_112_2_18_1_3_39_726 "AUDIO_SM_ConfigIfExpWorking exp:0x%x, ifWork:%d, index:%d, add 2."
#define ASM_MAIN_7646_112_2_18_1_3_39_727 "AUDIO_SM_ConfigIfExpWorking alloc error 2, size:%d."
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_AUDIOSERVICE_TRC)
TRACE_MSG(AUDIO_API_132_112_2_18_1_2_48_0,"audio_api.c:AUDIO_CreateFileHandle:error:PNULL")
TRACE_MSG(AUDIO_API_140_112_2_18_1_2_48_1,"audio_api.c:AUDIO_CreateFileHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_160_112_2_18_1_2_48_2,"audio_api.c:AUDIO_CreateFileHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_170_112_2_18_1_2_48_3,"audio_api.c:AUDIO_CreateFileHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_187_112_2_18_1_2_48_4,"audio_api.c:AUDIO_CreateFileHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_198_112_2_18_1_2_48_5,"audio_api.c:AUDIO_CreateFileHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_214_112_2_18_1_2_48_6,"audio_api.c:AUDIO_CreateFileHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_225_112_2_18_1_2_48_7,"audio_api.c:AUDIO_CreateFileHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_303_112_2_18_1_2_49_8,"audio_api.c:AUDIO_ASYCreateFileHandle:error:PNULL")
TRACE_MSG(AUDIO_API_311_112_2_18_1_2_49_9,"audio_api.c:AUDIO_ASYCreateFileHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_331_112_2_18_1_2_49_10,"audio_api.c:AUDIO_ASYCreateFileHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_341_112_2_18_1_2_49_11,"audio_api.c:AUDIO_ASYCreateFileHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_357_112_2_18_1_2_49_12,"audio_api.c:AUDIO_ASYCreateFileHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_368_112_2_18_1_2_49_13,"audio_api.c:AUDIO_ASYCreateFileHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_384_112_2_18_1_2_49_14,"audio_api.c:AUDIO_ASYCreateFileHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_396_112_2_18_1_2_49_15,"audio_api.c:AUDIO_ASYCreateFileHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_471_112_2_18_1_2_49_16,"audio_api.c:AUDIO_CreateBufferHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_491_112_2_18_1_2_49_17,"audio_api.c:AUDIO_CreateBufferHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_501_112_2_18_1_2_49_18,"audio_api.c:AUDIO_CreateBufferHandle:sigAUDIO_NO_ERROR != AUDIO_PM_StrCpLL")
TRACE_MSG(AUDIO_API_517_112_2_18_1_2_49_19,"audio_api.c:AUDIO_CreateBufferHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_527_112_2_18_1_2_49_20,"audio_api.c:AUDIO_CreateBufferHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCp")
TRACE_MSG(AUDIO_API_608_112_2_18_1_2_49_21,"audio_api.c:AUDIO_CreateBufferHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_628_112_2_18_1_2_49_22,"audio_api.c:AUDIO_CreateBufferHandle:codec_name_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_638_112_2_18_1_2_49_23,"audio_api.c:AUDIO_CreateBufferHandle:O_ERROR != AUDIO_PM_")
TRACE_MSG(AUDIO_API_654_112_2_18_1_2_49_24,"audio_api.c:AUDIO_CreateBufferHandle:device_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_665_112_2_18_1_2_49_25,"audio_api.c:AUDIO_CreateBufferHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_744_112_2_18_1_2_49_26,"audio_api.c:AUDIO_CreateStreamBufferHandle:error:CallbackFunc==PNULL")
TRACE_MSG(AUDIO_API_752_112_2_18_1_2_50_27,"audio_api.c:AUDIO_CreateStreamBufferHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_772_112_2_18_1_2_50_28,"audio_api.c:AUDIO_CreateStreamBufferHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_782_112_2_18_1_2_50_29,"audio_api.c:AUDIO_CreateStreamBufferHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_798_112_2_18_1_2_50_30,"audio_api.c:AUDIO_CreateStreamBufferHandle:device_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_809_112_2_18_1_2_50_31,"audio_api.c:AUDIO_CreateStreamBufferHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_894_112_2_18_1_2_50_32,"audio_api.c:AUDIO_ASYCreateStreamBufferHandle:error:CallbackFunc==PNULL")
TRACE_MSG(AUDIO_API_902_112_2_18_1_2_50_33,"audio_api.c:AUDIO_ASYCreateStreamBufferHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_922_112_2_18_1_2_50_34,"audio_api.c:AUDIO_ASYCreateStreamBufferHandle:codec_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_932_112_2_18_1_2_50_35,"audio_api.c:AUDIO_ASYCreateStreamBufferHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_948_112_2_18_1_2_50_36,"audio_api.c:AUDIO_ASYCreateStreamBufferHandle:device_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_959_112_2_18_1_2_50_37,"audio_api.c:AUDIO_ASYCreateStreamBufferHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_1009_112_2_18_1_2_50_38,"audio_api.c:AUDIO_CloseHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1064_112_2_18_1_2_50_39,"audio_api.c:AUDIO_ASYCloseHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1119_112_2_18_1_2_50_40,"audio_api.c:AUDIO_Play:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1181_112_2_18_1_2_50_41,"audio_api.c:AUDIO_ASYPlay:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1234_112_2_18_1_2_50_42,"audio_api.c:AUDIO_Stop:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1299_112_2_18_1_2_51_43,"audio_api.c:AUDIO_ASYStop:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1349_112_2_18_1_2_51_44,"audio_api.c:AUDIO_Pause:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1416_112_2_18_1_2_51_45,"audio_api.c:AUDIO_ASYPause:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1464_112_2_18_1_2_51_46,"audio_api.c:AUDIO_Resume:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1520_112_2_18_1_2_51_47,"audio_api.c:AUDIO_ASYResume:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1570_112_2_18_1_2_51_48,"audio_api.c:AUDIO_Seek:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1627_112_2_18_1_2_51_49,"audio_api.c:AUDIO_Seek:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1687_112_2_18_1_2_51_50,"audio_api.c:AUDIO_ASYSeek:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1739_112_2_18_1_2_51_51,"audio_api.c:AUDIO_SetSpeed:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1800_112_2_18_1_2_52_52,"audio_api.c:AUDIO_ASYSetSpeed:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1855_112_2_18_1_2_52_53,"audio_api.c:AUDIO_SetDigitalGain:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1915_112_2_18_1_2_52_54,"audio_api.c:AUDIO_ASYSetDigitalGain:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_1967_112_2_18_1_2_52_55,"audio_api.c:AUDIO_GetDigitalGain:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_2025_112_2_18_1_2_52_56,"audio_api.c:AUDIO_ASYGetDigitalGain:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_2076_112_2_18_1_2_52_57,"audio_api.c:AUDIO_SendStreamSrcData:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_2153_112_2_18_1_2_52_58,"audio_api.c:AUDIO_GetDigitalGain:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_2268_112_2_18_1_2_53_59,"audio_api.c:AUDIO_CreateRecordFileHandle:error:PNULL")
TRACE_MSG(AUDIO_API_2276_112_2_18_1_2_53_60,"audio_api.c:AUDIO_CreateRecordFileHandle:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_2298_112_2_18_1_2_53_61,"audio_api.c:AUDIO_CreateRecordFileHandle:codec_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_2308_112_2_18_1_2_53_62,"audio_api.c:AUDIO_CreateRecordFileHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16(")
TRACE_MSG(AUDIO_API_2324_112_2_18_1_2_53_63,"audio_api.c:AUDIO_CreateRecordFileHandle:device_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_2335_112_2_18_1_2_53_64,"audio_api.c:AUDIO_CreateRecordFileHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy1")
TRACE_MSG(AUDIO_API_2350_112_2_18_1_2_53_65,"audio_api.c:AUDIO_CreateRecordFileHandle:file_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_2362_112_2_18_1_2_53_66,"audio_api.c:AUDIO_CreateRecordFileHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_2436_112_2_18_1_2_53_67,"audio_api.c:AUDIO_ASYCreateRecordFileHandle:error:PNULL")
TRACE_MSG(AUDIO_API_2444_112_2_18_1_2_53_68,"audio_api.c:AUDIO_ASYCreateRecordFileHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_2466_112_2_18_1_2_53_69,"audio_api.c:AUDIO_ASYCreateRecordFileHandle:codec_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_2476_112_2_18_1_2_53_70,"audio_api.c:AUDIO_ASYCreateRecordFileHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_2492_112_2_18_1_2_53_71,"audio_api.c:AUDIO_ASYCreateRecordFileHandle:device_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_2503_112_2_18_1_2_53_72,"audio_api.c:AUDIO_ASYCreateRecordFileHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_2519_112_2_18_1_2_53_73,"audio_api.c:AUDIO_ASYCreateRecordFileHandle:file_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_2531_112_2_18_1_2_53_74,"audio_api.c:AUDIO_ASYCreateRecordFileHandle:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_2605_112_2_18_1_2_53_75,"audio_api.c:AUDIO_CreateRecordFileHandle_FileCreated:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_2627_112_2_18_1_2_53_76,"audio_api.c:AUDIO_CreateRecordFileHandle_FileCreated:error:codec_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_2637_112_2_18_1_2_53_77,"audio_api.c:AUDIO_CreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_2653_112_2_18_1_2_53_78,"audio_api.c:AUDIO_CreateRecordFileHandle_FileCreated:error:device_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_2664_112_2_18_1_2_53_79,"audio_api.c:AUDIO_CreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_2680_112_2_18_1_2_53_80,"audio_api.c:AUDIO_CreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_2692_112_2_18_1_2_53_81,"audio_api.c:AUDIO_CreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_2769_112_2_18_1_2_54_82,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:PNULL")
TRACE_MSG(AUDIO_API_2777_112_2_18_1_2_54_83,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_2799_112_2_18_1_2_54_84,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_2809_112_2_18_1_2_54_85,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_2825_112_2_18_1_2_54_86,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:device_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_2836_112_2_18_1_2_54_87,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_2852_112_2_18_1_2_54_88,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:file_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_2864_112_2_18_1_2_54_89,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_2936_112_2_18_1_2_54_90,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_2956_112_2_18_1_2_54_91,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:codec_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_2966_112_2_18_1_2_54_92,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16(cod")
TRACE_MSG(AUDIO_API_2982_112_2_18_1_2_54_93,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:device_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_2993_112_2_18_1_2_54_94,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_3071_112_2_18_1_2_54_95,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_3091_112_2_18_1_2_54_96,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:codec_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_3101_112_2_18_1_2_54_97,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_3117_112_2_18_1_2_54_98,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:device_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_3128_112_2_18_1_2_54_99,"audio_api.c:AUDIO_ASYCreateRecordFileHandle_FileCreated:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_3188_112_2_18_1_2_54_100,"audio_api.c:AUDIO_CreateNormalHandle:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_3208_112_2_18_1_2_54_101,"audio_api.c:AUDIO_CreateNormalHandle:error:codec_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_3218_112_2_18_1_2_54_102,"audio_api.c:AUDIO_CreateNormalHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_3234_112_2_18_1_2_54_103,"audio_api.c:AUDIO_CreateNormalHandle:error:pusDevName!=PNULL")
TRACE_MSG(AUDIO_API_3245_112_2_18_1_2_55_104,"audio_api.c:AUDIO_CreateNormalHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_3292_112_2_18_1_2_55_105,"audio_api.c:AUDIO_CreateCloneHandle:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_3313_112_2_18_1_2_55_106,"audio_api.c:AUDIO_CreateCloneHandle:error:pusDevName!=PNULL")
TRACE_MSG(AUDIO_API_3324_112_2_18_1_2_55_107,"audio_api.c:AUDIO_CreateCloneHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_3387_112_2_18_1_2_55_108,"audio_api.c:AUDIO_ASYCreateNormalHandle:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_3407_112_2_18_1_2_55_109,"audio_api.c:AUDIO_ASYCreateNormalHandle:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_3417_112_2_18_1_2_55_110,"audio_api.c:AUDIO_ASYCreateNormalHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_3433_112_2_18_1_2_55_111,"audio_api.c:AUDIO_ASYCreateNormalHandle:error:device_name_ptr==SCI_NULL")
TRACE_MSG(AUDIO_API_3444_112_2_18_1_2_55_112,"audio_api.c:AUDIO_ASYCreateNormalHandle:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_3586_112_2_18_1_2_55_113,"audio_api.c:AUDIO_SwitchDevice:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_3606_112_2_18_1_2_55_114,"audio_api.c:AUDIO_SwitchDevice:error:sig_ptr->pusDevName==SCI_NULL")
TRACE_MSG(AUDIO_API_3614_112_2_18_1_2_55_115,"audio_api.c:AUDIO_SwitchDevice:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_3659_112_2_18_1_2_55_116,"audio_api.c:AUDIO_ASYSwitchDevice:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_3678_112_2_18_1_2_55_117,"audio_api.c:AUDIO_ASYSwitchDevice:error:sig_ptr->pusDevName==SCI_NULL")
TRACE_MSG(AUDIO_API_3686_112_2_18_1_2_55_118,"audio_api.c:AUDIO_ASYSwitchDevice:error:AUDIO_NO_ERROR != AUDIO_PM_StrCpy16")
TRACE_MSG(AUDIO_API_3844_112_2_18_1_2_56_119,"audio_api.c:AUDIO_SetVolume:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_3900_112_2_18_1_2_56_120,"audio_api.c:AUDIO_SetVolume:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_3964_112_2_18_1_2_56_121,"audio_api.c:AUDIO_SetDevMode:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_3970_112_2_18_1_2_56_122,"AUDIO_SetDevMode new mode:%d, thread:0x%x.")
TRACE_MSG(AUDIO_API_4022_112_2_18_1_2_56_123,"AUDIO_SetDevModeEx, audio alloc failed.")
TRACE_MSG(AUDIO_API_4026_112_2_18_1_2_56_124,"AUDIO_SetDevModeEx new mode:%d, thread:0x%x.")
TRACE_MSG(AUDIO_API_4082_112_2_18_1_2_56_125,"audio_api.c:AUDIO_SetUpLinkMute:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_4141_112_2_18_1_2_56_126,"audio_api.c:AUDIO_SetDownLinkMute:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_4203_112_2_18_1_2_56_127,"audio_api.c:AUDIO_SetDownLinkLocalNetMute:Can't alloc enough memory for sig_ptr")
TRACE_MSG(AUDIO_API_4266_112_2_18_1_2_57_128,"audio_api.c:AUDIO_SetDevDigitalGain:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_4328_112_2_18_1_2_57_129,"audio_api.c:AUDIO_SetDataOutputChannelChoice:error:sig_ptr == SCI_NULL")
TRACE_MSG(AUDIO_API_4477_112_2_18_1_2_57_130,"audio_api.c:AUDIO_SetTrackOn:error:sig_ptr == SCI_NULL")
TRACE_MSG(ADMM_249_112_2_18_1_2_58_131,"admm.c:AUDMOD_SetDevMode:error:ptModNode is SCI_NULL, name:%s.")
TRACE_MSG(ADMM_284_112_2_18_1_2_58_132,"AUDMOD_SetDevMode set value: %d, %d, %d, %d, %d, %d, %d.")
TRACE_MSG(ADMM_285_112_2_18_1_2_58_133,"AUDMOD_SetDevMode: ad device original value =0x%4x")
TRACE_MSG(ADMM_289_112_2_18_1_2_58_134,"AUDMOD_SetDevMode: ad device modified value =0x%4x")
TRACE_MSG(ADMM_294_112_2_18_1_2_58_135,"AUDMOD_SetDevMode: M da device original value =0x%4x")
TRACE_MSG(ADMM_305_112_2_18_1_2_58_136,"AUDMOD_SetDevMode: M da device modified value =0x%4x")
TRACE_MSG(ADMM_315_112_2_18_1_2_58_137,"AUDMOD_SetDevMode: dolphin da device original value =0x%4x")
TRACE_MSG(ADMM_356_112_2_18_1_2_58_138,"AUDMOD_SetDevMode: dolphin da device modified value =0x%4x")
TRACE_MSG(ADMM_404_112_2_18_1_2_58_139,"admm.c:AUDMOD_SetDevModeEx:error:tModNode:0x%x, ptModNodetd:0x%x, name:%s(gsm), %s(td).")
TRACE_MSG(ADMM_441_112_2_18_1_2_58_140,"AUDMOD_SetDevModeEx set value: %d, %d, %d, %d, %d, %d, %d.")
TRACE_MSG(ADMM_443_112_2_18_1_2_58_141,"AUDMOD_SetDevModeEx: ad device original value =0x%4x")
TRACE_MSG(ADMM_447_112_2_18_1_2_58_142,"AUDMOD_SetDevModeEx: ad device modified value =0x%4x")
TRACE_MSG(ADMM_452_112_2_18_1_2_58_143,"AUDMOD_SetDevModeEx: M da device original value =0x%4x")
TRACE_MSG(ADMM_462_112_2_18_1_2_58_144,"AUDMOD_SetDevModeEx: M da device modified value =0x%4x")
TRACE_MSG(ADMM_472_112_2_18_1_2_58_145,"AUDMOD_SetDevModeEx: dolphin da device original value =0x%4x")
TRACE_MSG(ADMM_481_112_2_18_1_2_59_146,"AUDMOD_SetDevModeEx: dolphin da device modified value =0x%4x")
TRACE_MSG(ADMM_491_112_2_18_1_2_59_147,"AUDMOD_SetDevModeEx:  device modified value =0x%4x,0x%4x")
TRACE_MSG(ADMM_538_112_2_18_1_2_59_148,"admm.c:AUDMOD_GetNextDevMode:error:ptPreDevMode == SCI_NULL")
TRACE_MSG(ADMM_567_112_2_18_1_2_59_149,"admm.c:AUDMOD_AddDevMode:error:ptDevMode == SCI_NULL")
TRACE_MSG(ADMM_577_112_2_18_1_2_59_150,"admm.c:AUDMOD_AddDevMode:error:2 ptDevMode == SCI_NULL")
TRACE_MSG(ADMM_628_112_2_18_1_2_59_151,"admm.c:AUDMOD_RemoveDevMode:error:ptDevMode == SCI_NULL")
TRACE_MSG(ADMM_923_112_2_18_1_3_0_152,"admm.c:AUDMOD_GetModeParam:error:iceMode == SCI_NULL) ||  (pucModeName == SC")
TRACE_MSG(ADMM_959_112_2_18_1_3_0_153,"admm.c:AUDMOD_SetModeParam:error:iceMode == SCI_NULL) ||  (pucModeName == SC")
TRACE_MSG(ADMM_965_112_2_18_1_3_0_154,"AUDMOD_SetModeParam name :%s")
TRACE_MSG(ADMM_985_112_2_18_1_3_0_155,"AUDMOD_SetModeParam, extend[110]:0x%x, extend[99-100]:0x%x, 0x%x, switch:%d, gain:0x%x")
TRACE_MSG(ADMM_1048_112_2_18_1_3_0_156,"admm.c:AUDMOD_GetDevModNode:error:SCI_NULL==pucModeName")
TRACE_MSG(ADMM_1084_112_2_18_1_3_0_157,"admm.c AUDMOD_GetAudioDevModeFromName pcModeName is null.")
TRACE_MSG(ADMM_1088_112_2_18_1_3_0_158,"admm.c AUDMOD_GetAudioDevModeFromName pcModeName is %s.")
TRACE_MSG(ADMM_1098_112_2_18_1_3_0_159,"admm.c AUDMOD_GetAudioDevModeFromName NV dsp audio mode don't match, pls check name:%s.")
TRACE_MSG(ADMM_1142_112_2_18_1_3_0_160,"admm.c:ADMM_AddCallbackFuncToSomeTypeList input param ptList is PNULL.")
TRACE_MSG(ADMM_1147_112_2_18_1_3_0_161,"admm.c:ADMM_AddCallbackFuncToSomeTypeList input param ptList->ptFirstTypeInfo is PNULL.")
TRACE_MSG(ADMM_1159_112_2_18_1_3_0_162,"admm.c:ADMM_AddCallbackFuncToSomeTypeList alloc failed , size:%d.")
TRACE_MSG(ADMM_1184_112_2_18_1_3_0_163,"admm.c:AUDMOD_RegCallback ADMM_AddCallbackFuncAndType alloc failed, size:%d.")
TRACE_MSG(ADMM_1199_112_2_18_1_3_0_164,"admm.c:AUDMOD_RegCallback ADMM_AddCallbackFuncAndType alloc failed 2, size:%d.")
TRACE_MSG(ADMM_1251_112_2_18_1_3_0_165,"admm.c:AUDMOD_ExeCallbackFunc the callback is null,  eType:%d, index:%d.")
TRACE_MSG(ADMM_1280_112_2_18_1_3_0_166,"admm.c:AUDMOD_RegCallback eCallbackFuncType is not valid, %d, valid range:(0-%d).")
TRACE_MSG(ADMM_1285_112_2_18_1_3_0_167,"admm.c:AUDMOD_RegCallbackFunc eCallbackFuncType pCallbackFunc is null.")
TRACE_MSG(ADMM_1294_112_2_18_1_3_0_168,"admm.c:AUDMOD_RegCallbackFunc fail 1, result:%d, eType:%d, callbackFunc:0x%x..")
TRACE_MSG(ADMM_1300_112_2_18_1_3_0_169,"admm.c:AUDMOD_RegCallbackFunc success 1, eType:%d, callbackFunc:0x%x.")
TRACE_MSG(ADMM_1314_112_2_18_1_3_0_170,"admm.c:AUDMOD_RegCallbackFunc ptCurCallbackFuncList  is null, this case should not occur.")
TRACE_MSG(ADMM_1323_112_2_18_1_3_0_171,"admm.c:AUDMOD_RegCallbackFunc fail 2, result:%d, eType:%d, callbackFunc:0x%x..")
TRACE_MSG(ADMM_1328_112_2_18_1_3_0_172,"admm.c:AUDMOD_RegCallbackFunc success 2, eType:%d, callbackFunc:0x%x.")
TRACE_MSG(ADMM_1338_112_2_18_1_3_0_173,"admm.c:AUDMOD_RegCallbackFunc fail 3, result:%d, eType:%d, callbackFunc:0x%x..")
TRACE_MSG(ADMM_1343_112_2_18_1_3_0_174,"admm.c:AUDMOD_RegCallbackFunc success 3, eType:%d, callbackFunc:0x%x.")
TRACE_MSG(ADMM_1351_112_2_18_1_3_1_175,"admm.c AUDMOD_RegCallbackFunc start trace list.")
TRACE_MSG(ADMM_1354_112_2_18_1_3_1_176,"admm.c AUDMOD_RegCallbackFunc eType:%d.")
TRACE_MSG(ADMM_1358_112_2_18_1_3_1_177,"admm.c AUDMOD_RegCallbackFunc callbackFunc:0x%x.")
TRACE_MSG(ADMM_1363_112_2_18_1_3_1_178,"admm.c AUDMOD_RegCallbackFunc stop trace list.")
TRACE_MSG(ADMM_1387_112_2_18_1_3_1_179,"admm.c:AUDMOD_UnRegCallbackFunc eCallbackFuncType is not valid, %d, valid range:(0-%d).")
TRACE_MSG(ADMM_1392_112_2_18_1_3_1_180,"admm.c:AUDMOD_UnRegCallbackFunc eCallbackFuncType pCallbackFunc is null.")
TRACE_MSG(ADMM_1397_112_2_18_1_3_1_181,"admm.c:AUDMOD_UnRegCallbackFunc s_audio_device_mode_callback_func_ptr is null.")
TRACE_MSG(ADMM_1406_112_2_18_1_3_1_182,"admm.c:AUDMOD_UnRegCallbackFunc s_audio_device_mode_callback_func_ptr->ptFirstTypeInfo  is null.")
TRACE_MSG(ADMM_1421_112_2_18_1_3_1_183,"admm.c:AUDMOD_UnRegCallbackFunc success 0, eType:%d, callbackFunc:0x%x.")
TRACE_MSG(ADMM_1426_112_2_18_1_3_1_184,"admm.c:AUDMOD_UnRegCallbackFunc success 1, eType: %d, and callback 0x%x.")
TRACE_MSG(ADMM_1440_112_2_18_1_3_1_185,"admm.c:AUDMOD_UnRegCallbackFunc can't find the callbackType %d, and callback 0x%x regitered 1.")
TRACE_MSG(ADMM_1450_112_2_18_1_3_1_186,"admm.c:AUDMOD_UnRegCallbackFunc success 2, eType:%d, callbackFunc:0x%x.")
TRACE_MSG(ADMM_1458_112_2_18_1_3_1_187,"admm.c:AUDMOD_UnRegCallbackFunc can't find the callbackType %d, and callback 0x%x regitered 2.")
TRACE_MSG(ADMM_1474_112_2_18_1_3_1_188,"admm.c:AUDMOD_UnRegCallbackFunc can't find the callbackType %d, and callback 0x%x regitered 3.")
TRACE_MSG(ADMM_1489_112_2_18_1_3_1_189,"admm.c:AUDMOD_UnRegCallbackFunc success 3_0, eType:%d, callbackFunc:0x%x.")
TRACE_MSG(ADMM_1494_112_2_18_1_3_1_190,"admm.c:AUDMOD_UnRegCallbackFunc success 3_1, eType:%d, callbackFunc:0x%x.")
TRACE_MSG(ADMM_1510_112_2_18_1_3_1_191,"admm.c:AUDMOD_UnRegCallbackFunc success 4, eType:%d, callbackFunc:0x%x.")
TRACE_MSG(ADMM_1517_112_2_18_1_3_1_192,"admm.c:AUDMOD_UnRegCallbackFunc can't find the callbackType %d, and callback 0x%x regitered 4.")
TRACE_MSG(ADMM_1524_112_2_18_1_3_1_193,"admm.c:AUDMOD_UnRegCallbackFunc can't find the callbackType %d, and callback 0x%x regitered 5.")
TRACE_MSG(ADMM_1533_112_2_18_1_3_1_194,"admm.c AUDMOD_UnRegCallbackFunc start trace list.")
TRACE_MSG(ADMM_1536_112_2_18_1_3_1_195,"admm.c AUDMOD_UnRegCallbackFunc eType:%d.")
TRACE_MSG(ADMM_1540_112_2_18_1_3_1_196,"admm.c AUDMOD_UnRegCallbackFunc callbackFunc:0x%x.")
TRACE_MSG(ADMM_1545_112_2_18_1_3_1_197,"admm.c AUDMOD_UnRegCallbackFunc stop trace list.")
TRACE_MSG(AUDIO_NV_ARM_112_112_2_18_1_3_1_198,"audio_nv_arm.c:_AUDIONVARM_GetModeInfo:error:SCI_NULL==pucModeName")
TRACE_MSG(AUDIO_NV_ARM_119_112_2_18_1_3_1_199,"audio_nv_arm.c:_AUDIONVARM_GetModeInfo PNULL==spAudioNvArmModeListHead to read arm nv.")
TRACE_MSG(AUDIO_NV_ARM_155_112_2_18_1_3_1_200,"audio_nv_arm.c:AUDIONVARM_InitModeManager has been called !")
TRACE_MSG(AUDIO_NV_ARM_161_112_2_18_1_3_1_201,"audio_nv_arm.c:AUDIONVARM_InitModeManager has refuse to reed!")
TRACE_MSG(AUDIO_NV_ARM_184_112_2_18_1_3_1_202,"audio_nv_arm.c:AUDIONVARM_InitModeManager NV_ARM_AUDIO_MODE_NUM read failed, status:%d.")
TRACE_MSG(AUDIO_NV_ARM_192_112_2_18_1_3_1_203,"audio_nv_arm.c:AUDIONVARM_InitModeManager modeInfo:0x%x, usModeCount:%d, everyModeSize:%d, totalSize:%d.!")
TRACE_MSG(AUDIO_NV_ARM_219_112_2_18_1_3_1_204,"audio_nv_arm.c:AUDIONVARM_InitModeManager NV_ARM_AUDIO_MODE_PARA read failed, status:%d.")
TRACE_MSG(AUDIO_NV_ARM_229_112_2_18_1_3_1_205,"audio_nv_arm.c:AUDIONVARM_InitModeManager Exit !")
TRACE_MSG(AUDIO_NV_ARM_246_112_2_18_1_3_1_206,"audio_nv_arm.c:AUDIONVARM_GetModeNumber total count:%d!")
TRACE_MSG(AUDIO_NV_ARM_268_112_2_18_1_3_2_207,"audio_nv_arm.c:AUDIONVARM_AddMode:error:ptMode == SCI_NULL")
TRACE_MSG(AUDIO_NV_ARM_278_112_2_18_1_3_2_208,"audio_nv_arm.c:AUDIONVARM_AddMode:error:2 ptMode == SCI_NULL")
TRACE_MSG(AUDIO_NV_ARM_310_112_2_18_1_3_2_209,"audio_nv_arm.c:AUDIONVARM_AddMode success to add one new arm nv mode, add:0x%x")
TRACE_MSG(AUDIO_NV_ARM_331_112_2_18_1_3_2_210,"audio_nv_arm.c:AUDIONVARM_RemoveMode:error:pucModeName == SCI_NULL")
TRACE_MSG(AUDIO_NV_ARM_401_112_2_18_1_3_2_211,"audio_nv_arm.c:AUDIONVARM_GetModeParam:error:Mode:0x%x, pucModeName:%s")
TRACE_MSG(AUDIO_NV_ARM_411_112_2_18_1_3_2_212,"audio_nv_arm.c:AUDIONVARM_GetModeParam:error:Mode:0x%x, pucModeName:%s, listHeader:0x%x")
TRACE_MSG(AUDIO_NV_ARM_420_112_2_18_1_3_2_213,"audio_nv_arm.c:AUDIONVARM_GetModeParam success, nodeInfoAddress:0x%x, size:%d(withName),%d(noName), name:%s.")
TRACE_MSG(AUDIO_NV_ARM_448_112_2_18_1_3_2_214,"audio_nv_arm.c:AUDIONVARM_SetModeParam:error:Mode:0x%x, pucModeName:0x%x")
TRACE_MSG(AUDIO_NV_ARM_459_112_2_18_1_3_2_215,"audio_nv_arm.c:AUDIONVARM_SetModeParam:error:Mode:0x%x, pucModeName:0x%x")
TRACE_MSG(AUDIO_NV_ARM_467_112_2_18_1_3_2_216,"audio_nv_arm.c:AUDIONVARM_SetModeParam:failed to getModeInfo, Mode:0x%x, pucModeName:0x%x")
TRACE_MSG(AUDIO_NV_ARM_472_112_2_18_1_3_2_217,"audio_nv_arm.c:AUDIONVARM_SetModeParam:vol2:Mode:0x%x, pucModeName:%s, 0x%x")
TRACE_MSG(AUDIO_NV_ARM_475_112_2_18_1_3_2_218,"audio_nv_arm.c:AUDIONVARM_SetModeParam:vol3:Mode:0x%x, pucModeName:%s, 0x%x")
TRACE_MSG(AUDIO_NV_ARM_478_112_2_18_1_3_2_219,"audio_nv_arm.c:AUDIONVARM_SetModeParam:vol4:Mode:0x%x, pucModeName:%s, 0x%x")
TRACE_MSG(AUDIO_NV_ARM_503_112_2_18_1_3_2_220,"audio_nv_arm.c AUDIONVARM_GetAudioModeName count:%d.")
TRACE_MSG(AUDIO_NV_ARM_514_112_2_18_1_3_2_221,"audio_nv_arm.c AUDIONVARM_GetAudioModeName NV arm audio mode don't match, pls check type:%d.")
TRACE_MSG(AUDIO_NV_ARM_536_112_2_18_1_3_2_222,"audio_nv_arm.c AUDIONVARM_GetAudioModeType mode_name_ptr is PNULL.")
TRACE_MSG(AUDIO_NV_ARM_540_112_2_18_1_3_2_223,"audio_nv_arm.c AUDIONVARM_GetAudioModeType count:%d, name:%s.")
TRACE_MSG(AUDIO_NV_ARM_551_112_2_18_1_3_2_224,"audio_nv_arm.c AUDIONVARM_GetAudioModeType NV arm audio mode don't match, pls check name:%s.")
TRACE_MSG(AUDIO_NV_ARM_582_112_2_18_1_3_2_225,"audio_nv_arm.c:AUDIONVARM_GetAudioModeDevCtrInfo:error:ptDevCtrInfo:0x%x, pucModeName:0x%x, type:%d.")
TRACE_MSG(AUDIO_NV_ARM_606_112_2_18_1_3_2_226,"audio_nv_arm.c:AUDIONVARM_GetAudioModeDevCtrInfo, mode:%s, dev_set:%d.")
TRACE_MSG(AUDIO_NV_ARM_636_112_2_18_1_3_2_227,"audio_nv_arm.c:AUDIONVARM_ReadModeParamFromFlash:error:Mode == SCI_NULL")
TRACE_MSG(AUDIO_NV_ARM_697_112_2_18_1_3_2_228,"audio_nv_arm.c:AUDIONVARM_ReadModeParamFromFlash: curMode:%s.")
TRACE_MSG(AUDIO_NV_ARM_705_112_2_18_1_3_2_229,"audio_nv_arm.c:AUDIONVARM_ReadModeParamFromFlash: success, dataSizeEveryMode(noNameLen):%d, name:%s.")
TRACE_MSG(AUDIO_NV_ARM_710_112_2_18_1_3_2_230,"audio_nv_arm.c:AUDIONVARM_ReadModeParamFromFlash: this mode is not exist:%s.")
TRACE_MSG(AUDIO_NV_ARM_740_112_2_18_1_3_2_231,"audio_nv_arm.c:AUDIONVARM_WriteModeParamToFlash:error:Mode == SCI_NULL")
TRACE_MSG(AUDIO_NV_ARM_804_112_2_18_1_3_3_232,"audio_nv_arm.c:AUDIONVARM_WriteModeParamToFlash: curMode:%s.")
TRACE_MSG(AUDIO_NV_ARM_816_112_2_18_1_3_3_233,"audio_nv_arm.c:AUDIONVARM_WriteModeParamToFlash: success, dataSizeEveryMode(noNameLen):%d, name:%s.")
TRACE_MSG(AUDIO_NV_ARM_823_112_2_18_1_3_3_234,"audio_nv_arm.c:AUDIONVARM_WriteModeParamToFlash: failed.")
TRACE_MSG(AUDIO_NV_ARM_830_112_2_18_1_3_3_235,"audio_nv_arm.c:AUDIONVARM_WriteModeParamToFlash: this mode is not exist, %s.")
TRACE_MSG(AUDIO_NV_DSP_104_112_2_18_1_3_3_236,"audio_nv_dsp.c:_AUDIONVDSP_GetModeInfo:error:SCI_NULL==pucModeName")
TRACE_MSG(AUDIO_NV_DSP_111_112_2_18_1_3_3_237,"audio_nv_dsp.c:_AUDIONVDSP_GetModeInfo PNULL==spAudioNvDspModeListHead to read dsp nv.")
TRACE_MSG(AUDIO_NV_DSP_147_112_2_18_1_3_3_238,"audio_nv_dsp.c:AUDIONVDSP_InitModeManager has been called !")
TRACE_MSG(AUDIO_NV_DSP_153_112_2_18_1_3_3_239,"audio_nv_dsp.c:AUDIONVDSP_InitModeManager has refuse to reed!")
TRACE_MSG(AUDIO_NV_DSP_175_112_2_18_1_3_3_240,"audio_nv_dsp.c:AUDIONVDSP_InitModeManager NV_DSP_CODEC_INFO read failed, status:%d.")
TRACE_MSG(AUDIO_NV_DSP_185_112_2_18_1_3_3_241,"audio_nv_dsp.c:AUDIONVDSP_InitModeManager modeInfo:0x%x, b_is_dspctrl:%d usModeCount:%d, everyModeSize:%d, totalSize:%d.!")
TRACE_MSG(AUDIO_NV_DSP_213_112_2_18_1_3_3_242,"audio_nv_dsp.c:AUDIONVDSP_InitModeManager NV_DSP_AUDIO_MODE_PARA read failed, status:%d.")
TRACE_MSG(AUDIO_NV_DSP_223_112_2_18_1_3_3_243,"audio_nv_dsp.c:AUDIONVDSP_InitModeManager Exit !")
TRACE_MSG(AUDIO_NV_DSP_240_112_2_18_1_3_3_244,"audio_nv_dsp.c:AUDIONVDSP_GetModeNumber total count:%d!")
TRACE_MSG(AUDIO_NV_DSP_275_112_2_18_1_3_3_245,"audio_nv_dsp.c:AUDIONVDSP_GetNextDevMode:error:ptPreDevMode == SCI_NULL")
TRACE_MSG(AUDIO_NV_DSP_303_112_2_18_1_3_3_246,"audio_nv_dsp.c:AUDIONVDSP_AddMode:error:ptMode == SCI_NULL")
TRACE_MSG(AUDIO_NV_DSP_313_112_2_18_1_3_3_247,"audio_nv_dsp.c:AUDIONVDSP_AddMode:error:2 ptMode == SCI_NULL")
TRACE_MSG(AUDIO_NV_DSP_345_112_2_18_1_3_3_248,"audio_nv_dsp.c:AUDIONVDSP_AddMode success to add one new dsp nv mode, add:0x%x ucModeName:%s")
TRACE_MSG(AUDIO_NV_DSP_366_112_2_18_1_3_3_249,"audio_nv_dsp.c:AUDIONVDSP_RemoveMode:error:pucModeName == SCI_NULL")
TRACE_MSG(AUDIO_NV_DSP_436_112_2_18_1_3_4_250,"audio_nv_dsp.c:AUDIONVDSP_GetModeParam:error:Mode:0x%x, pucModeName:%s")
TRACE_MSG(AUDIO_NV_DSP_446_112_2_18_1_3_4_251,"audio_nv_dsp.c:AUDIONVDSP_GetModeParam:error:Mode:0x%x, pucModeName:%s, listHeader:0x%x")
TRACE_MSG(AUDIO_NV_DSP_455_112_2_18_1_3_4_252,"audio_nv_dsp.c:AUDIONVDSP_GetModeParam success, nodeInfoAddress:0x%x, size:%d(withName),%d(noName), name:%s.")
TRACE_MSG(AUDIO_NV_DSP_483_112_2_18_1_3_4_253,"audio_nv_dsp.c:AUDIONVDSP_SetModeParam:error:Mode:0x%x, pucModeName:0x%x")
TRACE_MSG(AUDIO_NV_DSP_494_112_2_18_1_3_4_254,"audio_nv_dsp.c:AUDIONVDSP_SetModeParam:error:Mode:0x%x, pucModeName:0x%x")
TRACE_MSG(AUDIO_NV_DSP_502_112_2_18_1_3_4_255,"audio_nv_dsp.c:AUDIONVDSP_SetModeParam:failed to getModeInfo, Mode:0x%x, pucModeName:0x%x")
TRACE_MSG(AUDIO_NV_DSP_507_112_2_18_1_3_4_256,"audio_nv_dsp.c:AUDIONVDSP_SetModeParam::Mode:0x%x, pucModeName:%s, vol9: 0x%x, 0x%x")
TRACE_MSG(AUDIO_NV_DSP_510_112_2_18_1_3_4_257,"audio_nv_dsp.c:AUDIONVDSP_SetModeParam::Mode:0x%x, pucModeName:%s, vol8: 0x%x, 0x%x")
TRACE_MSG(AUDIO_NV_DSP_535_112_2_18_1_3_4_258,"audio_nv_dsp.c AUDIONVDSP_GetAudioModeName count:%d.")
TRACE_MSG(AUDIO_NV_DSP_546_112_2_18_1_3_4_259,"audio_nv_dsp.c AUDIONVDSP_GetAudioModeName NV dsp audio mode don't match, pls check type:%d.")
TRACE_MSG(AUDIO_NV_DSP_568_112_2_18_1_3_4_260,"audio_nv_dsp.c AUDIONVDSP_GetAudioModeType mode_name_ptr is PNULL.")
TRACE_MSG(AUDIO_NV_DSP_572_112_2_18_1_3_4_261,"audio_nv_dsp.c AUDIONVDSP_GetAudioModeType count:%d, name:%s.")
TRACE_MSG(AUDIO_NV_DSP_583_112_2_18_1_3_4_262,"audio_nv_dsp.c AUDIONVDSP_GetAudioModeType NV dsp audio mode don't match, pls check name:%s.")
TRACE_MSG(AUDIO_NV_DSP_597_112_2_18_1_3_4_263,"audio_nv_dsp.c:AUDIONVDSP_GetAudioIsDspCtrlFromRam PNULL==spAudioNvDspModeListHead to read dsp nv.")
TRACE_MSG(AUDIO_NV_DSP_601_112_2_18_1_3_4_264,"audio_nv_dsp.c:AUDIONVDSP_GetAudioIsDspCtrlFromRam: success, is_dspctrl:%d .")
TRACE_MSG(AUDIO_NV_DSP_613_112_2_18_1_3_4_265,"audio_nv_dsp.c:AUDIONVDSP_SetAudioIsDspCtrlToRam PNULL==spAudioNvDspModeListHead to read dsp nv.")
TRACE_MSG(AUDIO_NV_DSP_617_112_2_18_1_3_4_266,"audio_nv_dsp.c:AUDIONVDSP_SetAudioIsDspCtrlToRam: success, is_dspctrl:%d .")
TRACE_MSG(AUDIO_NV_DSP_643_112_2_18_1_3_4_267,"audio_nv_dsp.c:AUDIONVDSP_GetAudioIsDspCtrlFromFlash: success, usModeInfo:0x%x is_dspctrl:%d .")
TRACE_MSG(AUDIO_NV_DSP_682_112_2_18_1_3_4_268,"audio_nv_dsp.c:AUDIONVDSP_SetAudioIsDspCtrlToFlash: success, usModeInfo:0x%x is_dspctrl:%d .")
TRACE_MSG(AUDIO_NV_DSP_686_112_2_18_1_3_4_269,"audio_nv_dsp.c:AUDIONVDSP_SetAudioIsDspCtrlToFlash: failed.")
TRACE_MSG(AUDIO_NV_DSP_717_112_2_18_1_3_4_270,"audio_nv_dsp.c:AUDIONVDSP_ReadModeParamFromFlash:error:Mode == SCI_NULL")
TRACE_MSG(AUDIO_NV_DSP_778_112_2_18_1_3_4_271,"audio_nv_dsp.c:AUDIONVDSP_ReadModeParamFromFlash: curMode:%s.")
TRACE_MSG(AUDIO_NV_DSP_786_112_2_18_1_3_4_272,"audio_nv_dsp.c:AUDIONVDSP_ReadModeParamFromFlash: success, dataSizeEveryMode(noNameLen):%d, name:%s.")
TRACE_MSG(AUDIO_NV_DSP_791_112_2_18_1_3_4_273,"audio_nv_dsp.c:AUDIONVDSP_ReadModeParamFromFlash: this mode is not exist:%s.")
TRACE_MSG(AUDIO_NV_DSP_821_112_2_18_1_3_4_274,"audio_nv_dsp.c:AUDIONVDSP_WriteModeParamToFlash:error:Mode == SCI_NULL")
TRACE_MSG(AUDIO_NV_DSP_885_112_2_18_1_3_4_275,"audio_nv_dsp.c:AUDIONVDSP_WriteModeParamToFlash: curMode:%s.")
TRACE_MSG(AUDIO_NV_DSP_897_112_2_18_1_3_4_276,"audio_nv_dsp.c:AUDIONVDSP_WriteModeParamToFlash: success, dataSizeEveryMode(noNameLen):%d, name:%s.")
TRACE_MSG(AUDIO_NV_DSP_904_112_2_18_1_3_4_277,"audio_nv_dsp.c:AUDIONVDSP_WriteModeParamToFlash: failed.")
TRACE_MSG(AUDIO_NV_DSP_911_112_2_18_1_3_4_278,"audio_nv_dsp.c:AUDIONVDSP_WriteModeParamToFlash: this mode is not exist, %s.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_117_112_2_18_1_3_5_279,"lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_GetDevModeName] nxp_mode_e(%d) is error")
TRACE_MSG(LVVE_CTRL_PARAM_NV_134_112_2_18_1_3_5_280,"lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_GetDevModeParam] Input params error.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_141_112_2_18_1_3_5_281,"lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_GetDevModeParam] LVVE_CTRL_PARAM_GetDevModeNode error.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_147_112_2_18_1_3_5_282,"lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_GetDevModeParam] Success.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_165_112_2_18_1_3_5_283,"lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_SetDevModeParam] Input params error.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_171_112_2_18_1_3_5_284,"lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_SetDevModeParam] Input mode name mismatch.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_179_112_2_18_1_3_5_285,"lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_SetDevModeParam] LVVE_CTRL_PARAM_GetDevModeNode error.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_185_112_2_18_1_3_5_286,"lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_SetDevModeParam] Success.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_206_112_2_18_1_3_5_287,"lvve_ctrl_param_nv.c Input params error.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_213_112_2_18_1_3_5_288,"lvve_ctrl_param_nv.c EFS_NvitemRead error, eResult=%d")
TRACE_MSG(LVVE_CTRL_PARAM_NV_221_112_2_18_1_3_5_289,"lvve_ctrl_param_nv.c SCI_ALLOC_APP failed")
TRACE_MSG(LVVE_CTRL_PARAM_NV_229_112_2_18_1_3_5_290,"lvve_ctrl_param_nv.c EFS_NvitemRead failed, eResult=%d")
TRACE_MSG(LVVE_CTRL_PARAM_NV_238_112_2_18_1_3_5_291,"lvve_ctrl_param_nv.c ReadModeParamFromFlash, dev_mode_name=%s")
TRACE_MSG(LVVE_CTRL_PARAM_NV_246_112_2_18_1_3_5_292,"lvve_ctrl_param_nv.c [ReadModeParamFromFlash] failed, not found device mode name")
TRACE_MSG(LVVE_CTRL_PARAM_NV_249_112_2_18_1_3_5_293,"lvve_ctrl_param_nv.c [ReadModeParamFromFlash] Success.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_271_112_2_18_1_3_5_294,"lvve_ctrl_param_nv.c [WriteModeParamToFlash] Input params error.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_278_112_2_18_1_3_5_295,"lvve_ctrl_param_nv.c EFS_NvitemRead error, eResult=%d")
TRACE_MSG(LVVE_CTRL_PARAM_NV_286_112_2_18_1_3_5_296,"lvve_ctrl_param_nv.c SCI_ALLOC_APP failed")
TRACE_MSG(LVVE_CTRL_PARAM_NV_294_112_2_18_1_3_5_297,"lvve_ctrl_param_nv.c EFS_NvitemRead failed, eResult=%d")
TRACE_MSG(LVVE_CTRL_PARAM_NV_304_112_2_18_1_3_5_298,"lvve_ctrl_param_nv.c [WriteModeParamToFlash], dev_mode_name=%s")
TRACE_MSG(LVVE_CTRL_PARAM_NV_312_112_2_18_1_3_5_299,"lvve_ctrl_param_nv.c [WriteModeParamToFlash] failed, not found device mode name")
TRACE_MSG(LVVE_CTRL_PARAM_NV_326_112_2_18_1_3_5_300,"lvve_ctrl_param_nv.c [WriteModeParamToFlash] NVITEM_UpdateNvParam_Protect failed")
TRACE_MSG(LVVE_CTRL_PARAM_NV_329_112_2_18_1_3_5_301,"lvve_ctrl_param_nv.c [WriteModeParamToFlash] Success.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_345_112_2_18_1_3_5_302,"lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_AddDevModeNode] Input params error.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_351_112_2_18_1_3_5_303,"lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_AddDevModeNode] SCI_ALLOC_APP Failed.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_372_112_2_18_1_3_6_304,"lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_AddDevModeNode] Success.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_404_112_2_18_1_3_6_305,"lvve_ctrl_param_nv.c EFS_NvitemRead error, eResult=%d")
TRACE_MSG(LVVE_CTRL_PARAM_NV_412_112_2_18_1_3_6_306,"lvve_ctrl_param_nv.c SCI_ALLOC_APP failed")
TRACE_MSG(LVVE_CTRL_PARAM_NV_420_112_2_18_1_3_6_307,"lvve_ctrl_param_nv.c EFS_NvitemRead failed, eResult=%d")
TRACE_MSG(LVVE_CTRL_PARAM_NV_446_112_2_18_1_3_6_308,"lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_GetDevModeNode] Input params error.")
TRACE_MSG(LVVE_CTRL_PARAM_NV_465_112_2_18_1_3_6_309,"lvve_ctrl_param_nv.c [LVVE_CTRL_PARAM_GetDevModeNode] Failed.")
TRACE_MSG(ADM_DP_127_112_2_18_1_3_8_310,"AUDIO_DP_WriteRingBuffer exp_count[j]==0  OPSITION1")
TRACE_MSG(ADM_DP_182_112_2_18_1_3_8_311,"AUDIO_DP_WriteRingBuffer exp_count[j]==0 OPSITION 2")
TRACE_MSG(ADM_DP_206_112_2_18_1_3_9_312,"AUDIO_DP_WriteRingBuffer exp_count[j]==0 OPSITION 3")
TRACE_MSG(ADM_DP_283_112_2_18_1_3_9_313,"adm_mng.c:AUDIO_DP_InitRingBuffer:error:invalid tracknum")
TRACE_MSG(ADM_DP_294_112_2_18_1_3_9_314,"AUDIO_DP_InitRingBuffer GetChannelChoice failed! result:%d.")
TRACE_MSG(ADM_DP_367_112_2_18_1_3_9_315,"adm_mng.c:AUDIO_DP_InitRingBuffer:error:invalid tracknum")
TRACE_MSG(ADM_DP_373_112_2_18_1_3_9_316,"AUDIO_DP_WriteRingBuffer uiCount==0")
TRACE_MSG(ADM_DP_382_112_2_18_1_3_9_317,"adm_dp.c:AUDIO_DP_WriteRingBuffer:error:PNULL==ptAudioObject")
TRACE_MSG(ADM_DP_410_112_2_18_1_3_9_318,"adm_dp.c:AUDIO_DP_WriteRingBuffer:error:_AUDIO_DP_ExeExpList")
TRACE_MSG(ADM_DP_422_112_2_18_1_3_9_319,"g_adm_dec_output_buf %d, %d, %d.")
TRACE_MSG(ADM_DP_508_112_2_18_1_3_9_320,"ADM,WriteBuf:Wait Device To Take Data: Timeout!")
TRACE_MSG(ADM_DP_574_112_2_18_1_3_9_321,"AUDIO_DP_WriteRingBuffer, playing flag:%d, lose data len:%d(halfword).")
TRACE_MSG(ADM_DP_610_112_2_18_1_3_9_322,"adm_dp.c:AUDIO_DP_WriteRingBuffer:error:sig_out_ptr == SCI_NULL")
TRACE_MSG(ADM_DP_622_112_2_18_1_3_9_323,"AUDIO_DP_WriteRingBuffer Prohibit to change priority of codec task:count:%d, ifhighPri:%d.")
TRACE_MSG(ADM_DP_646_112_2_18_1_3_9_324,"adm_dp.c:AUDIO_DP_WriteRingBuffer:error:_AUDIO_DP_ExeExpList reExeExp")
TRACE_MSG(ADM_DP_652_112_2_18_1_3_9_325,"AUDIO_DP_WriteRingBuffer reExeExp data count:%d")
TRACE_MSG(ADM_DP_666_112_2_18_1_3_9_326,"peter: waiting and g_adm_dec_output_buf[uiTrackNum].data_count  is %d")
TRACE_MSG(ADM_DP_727_112_2_18_1_3_10_327,"adm_mng.c:AUDIO_DP_WaitBufferEmpty:error:invalid tracknum")
TRACE_MSG(ADM_DP_750_112_2_18_1_3_10_328,"ADM,WaitBufferEmpty:Wait Device To Take Data: Timeout!")
TRACE_MSG(ADM_DP_812_112_2_18_1_3_10_329,"adm_mng.c:AUDIO_DP_ResetRingBuffer:error:invalid tracknum")
TRACE_MSG(ADM_DP_846_112_2_18_1_3_10_330,"adm_mng.c:AUDIO_DP_ResetRingBufferData:error:invalid tracknum")
TRACE_MSG(ADM_MNG_143_112_2_18_1_3_11_331,"[AUDIO_DM_SetSwitchDevice] s_b_switch_on=%d")
TRACE_MSG(ADM_MNG_165_112_2_18_1_3_11_332,"adm_mng.c:AUDIO_DM_RegAudioDevice:error:ptDeviceInfo == SCI_NULL")
TRACE_MSG(ADM_MNG_277_112_2_18_1_3_11_333,"AUDIO_DM_Open AUDIO_SMHC_VerifyHandle failed.")
TRACE_MSG(ADM_MNG_284_112_2_18_1_3_11_334,"AUDIO_DM_Open AUDIO_DM_GetDeviceHandleStruct failed.")
TRACE_MSG(ADM_MNG_315_112_2_18_1_3_11_335,"AUDIO_DM_Open AUDIO_DP_InitRingBuffer failed! result:%d.")
TRACE_MSG(ADM_MNG_327_112_2_18_1_3_11_336,"AUDIO_DM_Open device:0x%08x, ucDevOpenCnt:%d.")
TRACE_MSG(ADM_MNG_331_112_2_18_1_3_11_337,"adm_mng.c:AUDIO_DM_Open:error:.tAudioDevOpe.pOpen != SCI_NULL")
TRACE_MSG(ADM_MNG_350_112_2_18_1_3_11_338,"AUDIO_DM_Open puiBufferStartAddr:0x%x, bNeedTrack:%d.")
TRACE_MSG(ADM_MNG_358_112_2_18_1_3_11_339,"AUDIO_DM_Open device:0x%08x, ucDevOpenCnt:%d.")
TRACE_MSG(ADM_MNG_413_112_2_18_1_3_11_340,"adm_mng.c:AUDIO_DM_Close:error:invalid tracknum:%d.")
TRACE_MSG(ADM_MNG_421_112_2_18_1_3_11_341,"adm_mng.c:AUDIO_DM_Close:error:invalid tracknum")
TRACE_MSG(ADM_MNG_441_112_2_18_1_3_11_342,"AUDIO_DM_Close! has paused. device:0x%08x, 0x%08x,uiTotalTrackon:%d.")
TRACE_MSG(ADM_MNG_452_112_2_18_1_3_11_343,"AUDIO_DM_Close device:0x%08x, ucDevOpenCnt:%d.")
TRACE_MSG(ADM_MNG_457_112_2_18_1_3_11_344,"AUDIO_DM_Close dont close! device:0x%08x, 0x%08x, uiTotalTrackon:%d.")
TRACE_MSG(ADM_MNG_482_112_2_18_1_3_11_345,"adm_mng.c:AUDIO_DM_Close:error:invalid tracknum:%d, device:0x%x, 0x%x.")
TRACE_MSG(ADM_MNG_494_112_2_18_1_3_11_346,"AUDIO_DM_Close! has paused. device:0x%08x, 0x%08x.")
TRACE_MSG(ADM_MNG_504_112_2_18_1_3_11_347,"AUDIO_DM_Close device:0x%08x, ucDevOpenCnt:%d.")
TRACE_MSG(ADM_MNG_509_112_2_18_1_3_11_348,"AUDIO_DM_Close dont close! device:0x%08x, uiTotalTrackon:%d.")
TRACE_MSG(ADM_MNG_554_112_2_18_1_3_12_349,"AUDIO_DM_PreClose! has paused. device:0x%08x, 0x%08x,uiTotalTrackon:%d.")
TRACE_MSG(ADM_MNG_561_112_2_18_1_3_12_350,"adm_mng.c: AUDIO_DM_PreClose in hDevice is %x track num is %d,bHasPaused is %d")
TRACE_MSG(ADM_MNG_608_112_2_18_1_3_12_351,"adm_mng.c:AUDIO_DM_Pause:error:invalid tracknum")
TRACE_MSG(ADM_MNG_626_112_2_18_1_3_12_352,"AUDIO_DM_Pause device:0x%08x, ucDevOpenCnt:%d.")
TRACE_MSG(ADM_MNG_630_112_2_18_1_3_12_353,"adm_mng.c:AUDIO_DM_Pause:error:vOpe.pPause failed!")
TRACE_MSG(ADM_MNG_643_112_2_18_1_3_12_354,"adm_mng.c:AUDIO_DM_Pause:error:no matching device")
TRACE_MSG(ADM_MNG_656_112_2_18_1_3_12_355,"AUDIO_DM_Pause device:0x%08x, ucDevOpenCnt:%d.")
TRACE_MSG(ADM_MNG_660_112_2_18_1_3_12_356,"adm_mng.c:AUDIO_DM_Pause:error:2 vOpe.pPause failed!")
TRACE_MSG(ADM_MNG_737_112_2_18_1_3_12_357,"adm_mng.c:AUDIO_DM_Resume:error:invalid tracknum")
TRACE_MSG(ADM_MNG_763_112_2_18_1_3_12_358,"AUDIO_DM_Resume device:0x%08x, ucDevOpenCnt:%d.")
TRACE_MSG(ADM_MNG_767_112_2_18_1_3_12_359,"adm_mng.c:AUDIO_DM_Resume:error:vOpe.AUDIO_DM_Resume failed!")
TRACE_MSG(ADM_MNG_785_112_2_18_1_3_12_360,"adm_mng.c:AUDIO_DM_Resume:error:no matching device")
TRACE_MSG(ADM_MNG_801_112_2_18_1_3_12_361,"AUDIO_DM_Resume device:0x%08x, ucDevOpenCnt:%d.")
TRACE_MSG(ADM_MNG_805_112_2_18_1_3_12_362,"adm_mng.c:AUDIO_DM_Resume:error:vOpe.AUDIO_DM_Resume failed!")
TRACE_MSG(ADM_MNG_897_112_2_18_1_3_12_363,"adm_mng.c:AUDIO_DM_AddTrackIntoDevice:error:invalid tracknum")
TRACE_MSG(ADM_MNG_918_112_2_18_1_3_12_364,"adm_mng.c:AUDIO_DM_AddTrackIntoDevice:error:new_item == SCI_NULL")
TRACE_MSG(ADM_MNG_975_112_2_18_1_3_13_365,"adm_mng.c:AUDIO_DM_RemoveTrackFromDevice:error:invalid tracknum")
TRACE_MSG(ADM_MNG_1032_112_2_18_1_3_13_366,"adm_mng.c:AUDIO_DM_RemoveTrackFromDevice:error:can't find the track in the tracklis")
TRACE_MSG(ADM_MNG_1095_112_2_18_1_3_13_367,"adm_mng.c:AUDIO_DM_GetDeviceHandleFromName:error:= SCI_NULL)||(PNULL==pusDevName")
TRACE_MSG(ADM_MNG_1131_112_2_18_1_3_13_368,"adm_mng.c:AUDIO_DM_AddDeviceIntoADM")
TRACE_MSG(ADM_MNG_1174_112_2_18_1_3_13_369,"adm_mng.c:AUDIO_DM_RemoveDeviceFromADM:error:PNULL==ptDeviceHandle")
TRACE_MSG(ADM_MNG_1224_112_2_18_1_3_13_370,"AUDIO_DM_SwitchDevice input hSrcDev:0x%x, hDestDev:0x%x.")
TRACE_MSG(ADM_MNG_1232_112_2_18_1_3_13_371,"AUDIO_DM_SwitchDevice input hSrcDev:0x%x, hDestDev:0x%x, ERROR.")
TRACE_MSG(ADM_MNG_1239_112_2_18_1_3_13_372,"AUDIO_DM_SwitchDevice input hSrcDev:0x%x invalid.")
TRACE_MSG(ADM_MNG_1246_112_2_18_1_3_13_373,"AUDIO_DM_SwitchDevice input hDestDev:0x%x invalid.")
TRACE_MSG(ADM_MNG_1253_112_2_18_1_3_13_374,"AUDIO_DM_SwitchDevice bIsNeedTrack is different. hSrcDev:0x%x, hDestDev:0x%x.")
TRACE_MSG(ADM_MNG_1275_112_2_18_1_3_13_375,"AUDIO_DM_SwitchDevice, NeedTrack:%d, srcDevInfo:0x%x, destDevInfo:0x%x.")
TRACE_MSG(ADM_MNG_1287_112_2_18_1_3_13_376,"AUDIO_DM_SwitchDevice, 0 discard data:%d.")
TRACE_MSG(ADM_MNG_1303_112_2_18_1_3_13_377,"Error: uiTrackNum(%d)")
TRACE_MSG(ADM_MNG_1318_112_2_18_1_3_13_378,"adm_mng.c:AUDIO_DM_SwitchDevice:error:pClose failed")
TRACE_MSG(ADM_MNG_1324_112_2_18_1_3_13_379,"AUDIO_DM_SwitchDevice Success to close src device:0x%x")
TRACE_MSG(ADM_MNG_1334_112_2_18_1_3_13_380,"AUDIO_DM_SwitchDevice fail to AUDIO_SM_DeInitExpPara.")
TRACE_MSG(ADM_MNG_1349_112_2_18_1_3_14_381,"AUDIO_DM_SwitchDevice Success to open device:0x%x")
TRACE_MSG(ADM_MNG_1365_112_2_18_1_3_14_382,"AUDIO_DM_SwitchDevice fail to AUDIO_SM_InitExpPara.")
TRACE_MSG(ADM_MNG_1375_112_2_18_1_3_14_383,"AUDIO_DM_SwitchDevice:error:Open dest device failed!")
TRACE_MSG(ADM_MNG_1384_112_2_18_1_3_14_384,"AUDIO_DM_SwitchDevice Success to open src device again:0x%x")
TRACE_MSG(ADM_MNG_1392_112_2_18_1_3_14_385,"AUDIO_DM_SwitchDevice fail to AUDIO_SM_InitExpPara.")
TRACE_MSG(ADM_MNG_1401_112_2_18_1_3_14_386,"AUDIO_DM_SwitchDevice:error:open src device failed")
TRACE_MSG(ADM_MNG_1416_112_2_18_1_3_14_387,"adm_mng.c:AUDIO_DM_SwitchDevice:error:2 pClose failed")
TRACE_MSG(ADM_MNG_1444_112_2_18_1_3_14_388,"adm_mng.c:AUDIO_DM_SwitchDevice:error:2 pOpen failed!")
TRACE_MSG(ADM_MNG_1455_112_2_18_1_3_14_389,"adm_mng.c:AUDIO_DM_SwitchDevice:error:3 pOpen src failed!")
TRACE_MSG(ADM_MNG_1466_112_2_18_1_3_14_390,"AUDIO_DM_SwitchDevice input hSrcDev:0x%x, hDestDev:0x%x successfully finished.")
TRACE_MSG(ADM_MNG_1487_112_2_18_1_3_14_391,"adm_mng.c:AUDIO_DM_TrackOn:error:invalid tracknum")
TRACE_MSG(ADM_MNG_1495_112_2_18_1_3_14_392,"AUDIO_DM_TrackOn track_num:%d, track_on:%d, device:0x%08x, 0x%x, total_track_on:%d.")
TRACE_MSG(ADM_MNG_1522_112_2_18_1_3_14_393,"adm_mng.c:AUDIO_DM_TrackOff:error:invalid tracknum")
TRACE_MSG(ADM_MNG_1531_112_2_18_1_3_14_394,"AUDIO_DM_TrackOff track_num:%d, track_on:%d, device:0x%08x, 0x%x, total_track_on:%d.")
TRACE_MSG(ADM_MNG_1589_112_2_18_1_3_14_395,"adm_mng.c:AUDIO_DM_GetTrackStatus:error:invalid tracknum")
TRACE_MSG(ADM_MNG_1616_112_2_18_1_3_14_396,"AUDIO_DM_SetDevMode set the same mode:%d.")
TRACE_MSG(ADM_MNG_1621_112_2_18_1_3_14_397,"AUDIO_DM_SetDevMode ori mode:%d, new mode:%d.")
TRACE_MSG(ADM_MNG_1685_112_2_18_1_3_14_398,"AUDIO_DM_SetDevModeEx, No device opened yet!")
TRACE_MSG(ADM_MNG_1694_112_2_18_1_3_14_399,"AUDIO_DM_SetDevModeEx ori mode:%d, new mode:%d.")
TRACE_MSG(ADM_MNG_1727_112_2_18_1_3_14_400,"AUDIO_DM_SetDevModeEx, hAudioHandle illegal (0x%x)")
TRACE_MSG(ADM_MNG_1734_112_2_18_1_3_14_401,"AUDIO_DM_SetDevModeEx, Error,hAudioHandle is not exist(0x%x)")
TRACE_MSG(ADM_MNG_1741_112_2_18_1_3_14_402,"AUDIO_DM_SetDevModeEx, Error audio device handle(0x%x)")
TRACE_MSG(ADM_MNG_1748_112_2_18_1_3_14_403,"AUDIO_DM_SetDevModeEx, No device handle struct find!")
TRACE_MSG(ADM_MNG_1752_112_2_18_1_3_14_404,"AUDIO_DM_SetDevModeEx ori mode:%d, new mode:%d.")
TRACE_MSG(ADM_MNG_1765_112_2_18_1_3_14_405,"AUDIO_DM_SetDevModeEx, set mode failed.")
TRACE_MSG(ADM_MNG_1811_112_2_18_1_3_14_406,"AUDIO_DM_SetVolume set the same uiVol:%d.")
TRACE_MSG(ADM_MNG_1818_112_2_18_1_3_14_407,"AUDIO_DM_SetVolume ori vol:%d, appType:%d, new vol:%d, appType:%d.")
TRACE_MSG(ADM_MNG_1895_112_2_18_1_3_15_408,"AUDIO_DM_SetVolumeAndTypeEx, No device opened yet!")
TRACE_MSG(ADM_MNG_1906_112_2_18_1_3_15_409,"AUDIO_DM_SetVolumeAndTypeEx ori vol:%d, appType:%d, new vol:%d, appType:%d.")
TRACE_MSG(ADM_MNG_1957_112_2_18_1_3_15_410,"AUDIO_DM_SetVolumeAndTypeEx, hAudioHandle illegal (0x%x)")
TRACE_MSG(ADM_MNG_1964_112_2_18_1_3_15_411,"AUDIO_DM_SetVolumeAndTypeEx, Error,hAudioHandle is not exist(0x%x)")
TRACE_MSG(ADM_MNG_1971_112_2_18_1_3_15_412,"AUDIO_DM_SetVolumeAndTypeEx, Error audio device handle(0x%x)")
TRACE_MSG(ADM_MNG_1978_112_2_18_1_3_15_413,"AUDIO_DM_SetVolumeAndTypeEx, No device handle struct find!")
TRACE_MSG(ADM_MNG_1985_112_2_18_1_3_15_414,"AUDIO_DM_SetVolumeAndTypeEx ori vol:%d, appType:%d, new vol:%d, appType:%d, handle:0x%x.")
TRACE_MSG(ADM_MNG_1998_112_2_18_1_3_15_415,"AUDIO_DM_SetVolumeAndTypeEx, set mode failed.")
TRACE_MSG(ADM_MNG_2021_112_2_18_1_3_15_416,"AUDIO_DM_GetVolume:eAppType:%d,uiVol:%d")
TRACE_MSG(ADM_MNG_2046_112_2_18_1_3_15_417,"AUDIO_DM_SetUplinkMute set the same bUplinkMute:%d.")
TRACE_MSG(ADM_MNG_2122_112_2_18_1_3_15_418,"AUDIO_DM_SetDownlinkMute set the same bDownlinkMute:%d.")
TRACE_MSG(ADM_MNG_2251_112_2_18_1_3_15_419,"AUDIO_DM_GetDownLinkLocalNetMute input para error:0x%x, 0x%x.")
TRACE_MSG(ADM_MNG_2278_112_2_18_1_3_15_420,"AUDIO_DM_SetDigitalGain set the same uiGain:%d.")
TRACE_MSG(ADM_MNG_2354_112_2_18_1_3_15_421,"AUDIO_DM_SetDataOutputChannelChoice set the same eChannelChoice:%d.")
TRACE_MSG(ADM_MNG_2427_112_2_18_1_3_16_422,"adm_mng.c:AUDIO_DM_AddOpenDeviceIntoADM:error:no free device space to add")
TRACE_MSG(ADM_MNG_2453_112_2_18_1_3_16_423,"adm_mng.c:AUDIO_DM_AddOpenDeviceIntoADM:error:can't find device to remove")
TRACE_MSG(ADM_MNG_2568_112_2_18_1_3_16_424,"AUDIO_DM_GetAppropriateOutputSamplerate device:0x%x, output samplerate %d is not support.")
TRACE_MSG(APM_CODEC_518_112_2_18_1_3_17_425,"AUDIO_PM_AddCodecExpress failed. the express 0x%x is in the exp list of this codec:0x%x.")
TRACE_MSG(APM_CODEC_598_112_2_18_1_3_17_426,"the express be deleted doesn't exist in the codec express list.")
TRACE_MSG(APM_CODEC_612_112_2_18_1_3_18_427,"the express be deleted doesn't exist in the codec express list.")
TRACE_MSG(APM_CODEC_1020_112_2_18_1_3_19_428,"no matching codec ext operation in AUDIO_PM_ExeCodecExtOpe.")
TRACE_MSG(APM_CODEC_1032_112_2_18_1_3_19_429,"ptOneExtOpe->uiExtOpePointer is PNULL in AUDIO_PM_ExeCodecExtOpe.")
TRACE_MSG(APM_CODEC_1044_112_2_18_1_3_19_430,"apm_codec.c: AUDIO_PM_ExeCodecExtOpe:error:sig_ptr == SCI_NULL")
TRACE_MSG(APM_CODEC_1050_112_2_18_1_3_19_431,"AUDIO_PM_ExeCodecExtOpe handle:0x%x, symbol:%s.")
TRACE_MSG(APM_CODEC_1244_112_2_18_1_3_19_432,"not any reg codec plugger.")
TRACE_MSG(APM_CODEC_1249_112_2_18_1_3_19_433,"para - codec Handle error.")
TRACE_MSG(APM_CODEC_1265_112_2_18_1_3_19_434,"not such reg codec plugger match the para.")
TRACE_MSG(APM_CODEC_1297_112_2_18_1_3_19_435,"not any reg codec plugger.")
TRACE_MSG(APM_CODEC_1302_112_2_18_1_3_19_436,"usCodecName is SCI_NULL.")
TRACE_MSG(APM_CODEC_1318_112_2_18_1_3_19_437,"not such reg codec plugger match the para.")
TRACE_MSG(APM_EXPRESS_389_112_2_18_1_3_20_438,"AUDIO_PM_ExeExpOpe not inited, 0x%x.")
TRACE_MSG(APM_EXPRESS_443_112_2_18_1_3_21_439,"AUDIO_PM_ExeExpSetParaOpe AUDIO_NO_EXPHANDLE.")
TRACE_MSG(APM_EXPRESS_449_112_2_18_1_3_21_440,"AUDIO_PM_ExeExpSetParaOpe AUDIO_NO_EXPHANDLE.")
TRACE_MSG(APM_EXPRESS_496_112_2_18_1_3_21_441,"AUDIO_PM_ExeExpSetParaOpe AUDIO_NO_EXPHANDLE.")
TRACE_MSG(APM_EXPRESS_502_112_2_18_1_3_21_442,"AUDIO_PM_ExeExpSetParaOpe AUDIO_NO_EXPHANDLE.")
TRACE_MSG(APM_EXPRESS_544_112_2_18_1_3_21_443,"AUDIO_PM_ExeExpInitParaOpe AUDIO_NO_EXPHANDLE 1.")
TRACE_MSG(APM_EXPRESS_550_112_2_18_1_3_21_444,"AUDIO_PM_ExeExpInitParaOpe AUDIO_NO_EXPHANDLE 2.")
TRACE_MSG(APM_EXPRESS_556_112_2_18_1_3_21_445,"AUDIO_PM_ExeExpInitParaOpe not working, 0x%x.")
TRACE_MSG(APM_EXPRESS_599_112_2_18_1_3_21_446,"AUDIO_PM_ExeExpDeInitParaOpe AUDIO_NO_EXPHANDLE 1.")
TRACE_MSG(APM_EXPRESS_605_112_2_18_1_3_21_447,"AUDIO_PM_ExeExpDeInitParaOpe AUDIO_NO_EXPHANDLE 2.")
TRACE_MSG(APM_EXPRESS_611_112_2_18_1_3_21_448,"AUDIO_PM_ExeExpDeInitParaOpe not working, 0x%x.")
TRACE_MSG(APM_EXPRESS_736_112_2_18_1_3_21_449,"not any reg express plugger.")
TRACE_MSG(APM_EXPRESS_778_112_2_18_1_3_21_450,"not any reg express plugger.")
TRACE_MSG(APM_EXPRESS_794_112_2_18_1_3_21_451,"no matching reg express plugger.")
TRACE_MSG(ASM_HC_165_112_2_18_1_3_22_452,"%s, %d, audio handle is invalid, hAudioHandle:0x%08x.")
TRACE_MSG(ASM_HC_171_112_2_18_1_3_22_453,"%s, %d, audio handle is invalid, hAudioHandle:0x%08x.")
TRACE_MSG(ASM_HC_177_112_2_18_1_3_22_454,"%s, %d, audio handle is invalid, hAudioHandle:0x%08x.")
TRACE_MSG(ASPEC_CALC_50_112_2_18_1_3_23_455,"AUDIO:AUDIO_SPEC_Open hAudioHandle:0x%08x")
TRACE_MSG(ASPEC_CALC_56_112_2_18_1_3_23_456,"%s, %d, error. hAudioHandle:0x%08x.")
TRACE_MSG(ASPEC_CALC_64_112_2_18_1_3_23_457,"%s, %d, error. hAudioHandle:0x%08x.")
TRACE_MSG(ASPEC_CALC_83_112_2_18_1_3_23_458,"AUDIO:AUDIO_SPEC_Close hAudioHandle:0x%08x")
TRACE_MSG(ASPEC_CALC_89_112_2_18_1_3_23_459,"%s, %d, error. hAudioHandle:0x%08x.")
TRACE_MSG(ASPEC_CALC_96_112_2_18_1_3_23_460,"%s, %d, error. hAudioHandle:0x%08x.")
TRACE_MSG(ASPEC_CALC_131_112_2_18_1_3_23_461,"%s, %d, error. hAudioHandle:0x%08x.")
TRACE_MSG(ASPEC_CALC_138_112_2_18_1_3_23_462,"%s, %d, error. hAudioHandle:0x%08x.")
TRACE_MSG(ASPEC_CALC_145_112_2_18_1_3_23_463,"%s, %d,the caculator is already closed! ")
TRACE_MSG(ASPEC_CALC_152_112_2_18_1_3_23_464,"%s, %d, error. puiSpecBuf:0x%08x.")
TRACE_MSG(ASPEC_CALC_161_112_2_18_1_3_23_465,"%s, %d, error. sample_num:%d.")
TRACE_MSG(ASM_MAIN_316_112_2_18_1_3_24_466,"_AUDIO_Enable_DumpPcmData_InMusicPlay error. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_332_112_2_18_1_3_24_467,"_AUDIO_Enable_DumpPcmData_InMusicPlay appTaskPri(%d) is lower than codecTaskPri(%d). hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_340_112_2_18_1_3_24_468,"_AUDIO_Enable_DumpPcmData_InMusicPlay audio handle type: %d. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_347_112_2_18_1_3_24_469,"_AUDIO_Enable_DumpPcmData_InMusicPlay mode:%d.")
TRACE_MSG(ASM_MAIN_354_112_2_18_1_3_25_470,"_AUDIO_Enable_DumpPcmData_InMusicPlay, alloc fail, size: %d")
TRACE_MSG(ASM_MAIN_361_112_2_18_1_3_25_471,"_AUDIO_Enable_DumpPcmData_InMusicPlay failed to get mode param.")
TRACE_MSG(ASM_MAIN_370_112_2_18_1_3_25_472,"_AUDIO_Enable_DumpPcmData_InMusicPlay failed to AUDIO_GetDebugOutputDataFileCount.")
TRACE_MSG(ASM_MAIN_377_112_2_18_1_3_25_473,"audio_api.c:_AUDIO_Enable_DumpPcmData_InMusicPlay, codecHandle:0x%x, expHandle:0x%x, i:0.")
TRACE_MSG(ASM_MAIN_381_112_2_18_1_3_25_474,"audio_api.c:_AUDIO_Enable_DumpPcmData_InMusicPlay:expHandle:0x%x, i:%d.")
TRACE_MSG(ASM_MAIN_397_112_2_18_1_3_25_475,"_AUDIO_Enable_DumpPcmData_InMusicPlay don't  dump pcm data, usDebugFileCount:%d, Config:0x%x.")
TRACE_MSG(ASM_MAIN_425_112_2_18_1_3_25_476,"_AUDIO_Enable_DumpPcmData_InMusicPlay don't support other file system, config:0x%x.")
TRACE_MSG(ASM_MAIN_443_112_2_18_1_3_25_477,"_AUDIO_Enable_DumpPcmData_InMusicPlay dump pcm data, usDebugFileCount:%d, Config:0x%x, usmask:0x%x.")
TRACE_MSG(ASM_MAIN_447_112_2_18_1_3_25_478,"_AUDIO_Enable_DumpPcmData_InMusicPlay failed to AUDIO_OfferDebugDataFileName.")
TRACE_MSG(ASM_MAIN_463_112_2_18_1_3_25_479,"_AUDIO_Enable_DumpPcmData_InMusicPlay dump pcm data, usDebugFileCount:%d, Config:0x%x, usmask:0x%x.")
TRACE_MSG(ASM_MAIN_467_112_2_18_1_3_25_480,"_AUDIO_Enable_DumpPcmData_InMusicPlay failed to AUDIO_OfferDebugDataFileName.")
TRACE_MSG(ASM_MAIN_472_112_2_18_1_3_25_481,"_AUDIO_Enable_DumpPcmData_InMusicPlay don't support other file system, config:0x%x.")
TRACE_MSG(ASM_MAIN_601_112_2_18_1_3_25_482,"AUDIO:AUDIO_SM_CreateFileHandle hAudioCodec:0x%08x")
TRACE_MSG(ASM_MAIN_684_112_2_18_1_3_25_483,"asm_main.c:AUDIO_SM_CreateFileHandle:error:new_item == SCI_NULL")
TRACE_MSG(ASM_MAIN_762_112_2_18_1_3_25_484,"asm_main.c:AUDIO_SM_CreateFileHandle:error:new_item == SCI_NULL")
TRACE_MSG(ASM_MAIN_835_112_2_18_1_3_25_485,"AUDIO:AUDIO_SM_CreateFileHandle audio handle:0x%08x")
TRACE_MSG(ASM_MAIN_887_112_2_18_1_3_26_486,"AUDIO:AUDIO_SM_CreateBufferHandle hAudioCodec:0x%08x")
TRACE_MSG(ASM_MAIN_952_112_2_18_1_3_26_487,"asm_main.c:AUDIO_SM_CreateBufferHandle:error:new_item == SCI_NULL")
TRACE_MSG(ASM_MAIN_1025_112_2_18_1_3_26_488,"asm_main.c:AUDIO_SM_CreateBufferHandle:error:new_item == SCI_NULL")
TRACE_MSG(ASM_MAIN_1093_112_2_18_1_3_26_489,"AUDIO:AUDIO_SM_CreateBufferHandle audio handle:0x%08x")
TRACE_MSG(ASM_MAIN_1149_112_2_18_1_3_26_490,"AUDIO:AUDIO_SM_CreateStreamBufferHandle hAudioCodec:0x%08x")
TRACE_MSG(ASM_MAIN_1213_112_2_18_1_3_26_491,"asm_main.c:AUDIO_SM_CreateStreamBufferHandle:error:new_item == SCI_NULL")
TRACE_MSG(ASM_MAIN_1287_112_2_18_1_3_26_492,"asm_main.c:AUDIO_SM_CreateStreamBufferHandle:error:new_item == SCI_NULL")
TRACE_MSG(ASM_MAIN_1356_112_2_18_1_3_26_493,"AUDIO:AUDIO_SM_CreateStreamBufferHandle audio handle:0x%08x")
TRACE_MSG(ASM_MAIN_1379_112_2_18_1_3_27_494,"AUDIO:AUDIO_SM_CloseHandle hAudioHandle:0x%08x")
TRACE_MSG(ASM_MAIN_1414_112_2_18_1_3_27_495,"%s, %d, codec_handle:0x%x.")
TRACE_MSG(ASM_MAIN_1423_112_2_18_1_3_27_496,"AUDIO_SM_CloseHandle pDeconstruct error:%d.")
TRACE_MSG(ASM_MAIN_1517_112_2_18_1_3_27_497,"asm_main.c:AUDIO_SM_CloseHandle:error:The audio link list is NULL")
TRACE_MSG(ASM_MAIN_1549_112_2_18_1_3_27_498,"AUDIO:AUDIO_SM_Play hAudioHandle:0x%08x")
TRACE_MSG(ASM_MAIN_1557_112_2_18_1_3_27_499,"%s, %d, error. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_1564_112_2_18_1_3_27_500,"%s, %d, error. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_1578_112_2_18_1_3_27_501,"%s, %d, input para error. hAudioHandle:0x%08x. offset:%d, file_total_len:%d.")
TRACE_MSG(ASM_MAIN_1612_112_2_18_1_3_27_502,"AUDIO:AUDIO_SM_Play sample_rate:%d")
TRACE_MSG(ASM_MAIN_1623_112_2_18_1_3_27_503,"AUDIO:AUDIO_SM_Play device appropriate sample_rate:%d, device:0x%x.")
TRACE_MSG(ASM_MAIN_1660_112_2_18_1_3_27_504,"AUDIO:AUDIO_SM_Play track_num:%d")
TRACE_MSG(ASM_MAIN_1673_112_2_18_1_3_27_505,"AUDIO_SM_Play AUDIO_SM_InitExpPara error:%d.")
TRACE_MSG(ASM_MAIN_1702_112_2_18_1_3_27_506,"audio:0x%x, ASM:uiCodecHighPriority = %d,uiCodecLowPriority = %d, track_on_point:%d.")
TRACE_MSG(ASM_MAIN_1713_112_2_18_1_3_27_507,"asm_main.c:AUDIO_SM_Play:error:SCI_NULL == audio_obj->ptTrackBufTimer")
TRACE_MSG(ASM_MAIN_1732_112_2_18_1_3_27_508,"asm_main.c:AUDIO_SM_Play:error:SCI_NULL == audio_obj->ptTrackOnTimer")
TRACE_MSG(ASM_MAIN_1751_112_2_18_1_3_27_509,"%s, %d, error. hAudioHandle:0x%08x, hCodecHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_1770_112_2_18_1_3_27_510,"AUDIO:AUDIO_SM_Play result1:%d")
TRACE_MSG(ASM_MAIN_1778_112_2_18_1_3_27_511,"asm_main.c:AUDIO_SM_Play:error:SCI_ERROR == SCI_ChangeThreadPriority")
TRACE_MSG(ASM_MAIN_1790_112_2_18_1_3_27_512,"AUDIO_SM_Play, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d, time:%d.")
TRACE_MSG(ASM_MAIN_1795_112_2_18_1_3_27_513,"asm_main.c:AUDIO_SM_Play:assiTask error:SCI_ERROR == SCI_ChangeThreadPriority")
TRACE_MSG(ASM_MAIN_1807_112_2_18_1_3_27_514,"AUDIO_SM_Play, success to ChangeThreadPriority assiTask:src_pri:%d, dest_pri:%d, time:%d.")
TRACE_MSG(ASM_MAIN_1839_112_2_18_1_3_27_515,"AUDIO_SM_Play pStopDeconstruct error:%d.")
TRACE_MSG(ASM_MAIN_1863_112_2_18_1_3_27_516,"AUDIO:AUDIO_SM_Play result2:%d")
TRACE_MSG(ASM_MAIN_1876_112_2_18_1_3_27_517,"asm_main.c:AUDIO_SM_PauseEnd:error:audio_obj == SCI_NULL")
TRACE_MSG(ASM_MAIN_1885_112_2_18_1_3_27_518,"asm_main.c:AUDIO_SM_StopSelf:error:sig_out_ptr == SCI_NULL")
TRACE_MSG(ASM_MAIN_1891_112_2_18_1_3_27_519,"AUDIO_SM_StopSelf  hAudioHandle:0x%08x")
TRACE_MSG(ASM_MAIN_1925_112_2_18_1_3_28_520,"AUDIO:AUDIO_SM_Stop hAudioHandle:0x%08x")
TRACE_MSG(ASM_MAIN_1957_112_2_18_1_3_28_521,"%s, %d, codec_handle:0x%x.")
TRACE_MSG(ASM_MAIN_1963_112_2_18_1_3_28_522,"%s, %d, codec_handle:0x%x.")
TRACE_MSG(ASM_MAIN_1973_112_2_18_1_3_28_523,"asm_main.c:AUDIO_SM_Stop:error:SCI_ERROR == SCI_ChangeThreadPriority")
TRACE_MSG(ASM_MAIN_1981_112_2_18_1_3_28_524,"AUDIO_SM_Stop, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d.")
TRACE_MSG(ASM_MAIN_1986_112_2_18_1_3_28_525,"asm_main.c:AUDIO_SM_Stop:error:SCI_ERROR == SCI_ChangeThreadPriority 2")
TRACE_MSG(ASM_MAIN_1991_112_2_18_1_3_28_526,"AUDIO_SM_Stop, success to ChangeThreadPriority assi task:src_pri:%d, dest_pri:%d.")
TRACE_MSG(ASM_MAIN_2011_112_2_18_1_3_28_527,"AUDIO_SM_Stop pStopDeconstruct error:%d.")
TRACE_MSG(ASM_MAIN_2051_112_2_18_1_3_28_528,"AUDIO_SM_Stop, stop fail, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d.")
TRACE_MSG(ASM_MAIN_2061_112_2_18_1_3_28_529,"AUDIO_SM_Stop, stop fail, success to ChangeThreadPriority: assiTask src_pri:%d, dest_pri:%d.")
TRACE_MSG(ASM_MAIN_2098_112_2_18_1_3_28_530,"AUDIO:AUDIO_SM_Pause hAudioHandle:0x%08x")
TRACE_MSG(ASM_MAIN_2128_112_2_18_1_3_28_531,"%s, %d, codec_handle:0x%x.")
TRACE_MSG(ASM_MAIN_2134_112_2_18_1_3_28_532,"%s, %d, codec_handle:0x%x.")
TRACE_MSG(ASM_MAIN_2144_112_2_18_1_3_28_533,"asm_main.c:AUDIO_SM_Pause:error:SCI_ERROR == SCI_ChangeThreadPriority")
TRACE_MSG(ASM_MAIN_2151_112_2_18_1_3_28_534,"AUDIO_SM_Pause, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d.")
TRACE_MSG(ASM_MAIN_2156_112_2_18_1_3_28_535,"asm_main.c:AUDIO_SM_Pause:error:SCI_ERROR == SCI_ChangeThreadPriority assiTask.")
TRACE_MSG(ASM_MAIN_2163_112_2_18_1_3_28_536,"AUDIO_SM_Pause, success to ChangeThreadPriority assiTask:src_pri:%d, dest_pri:%d.")
TRACE_MSG(ASM_MAIN_2182_112_2_18_1_3_28_537,"asm_main.c:AUDIO_SM_Pause:error:SCI_ERROR  result = gen_operation->pPauseDec")
TRACE_MSG(ASM_MAIN_2220_112_2_18_1_3_28_538,"AUDIO_SM_Pause, pause fail.success to ChangeThreadPriority:src_pri:%d, dest_pri:%d.")
TRACE_MSG(ASM_MAIN_2230_112_2_18_1_3_28_539,"AUDIO_SM_Pause, pause fail. success to ChangeThreadPriority assiTask:src_pri:%d, dest_pri:%d.")
TRACE_MSG(ASM_MAIN_2261_112_2_18_1_3_28_540,"AUDIO:AUDIO_SM_Resume hAudioHandle:0x%08x")
TRACE_MSG(ASM_MAIN_2301_112_2_18_1_3_28_541,"AUDIO:AUDIO_SM_Resume sample_rate:%d")
TRACE_MSG(ASM_MAIN_2312_112_2_18_1_3_28_542,"AUDIO:AUDIO_SM_Resume device appropriate sample_rate:%d, device:0x%x.")
TRACE_MSG(ASM_MAIN_2333_112_2_18_1_3_28_543,"AUDIO_SM_Resume AUDIO_SM_InitExpPara error:%d.")
TRACE_MSG(ASM_MAIN_2352_112_2_18_1_3_28_544,"AUDIO_SM_Resume pResumeConstruct error:%d.")
TRACE_MSG(ASM_MAIN_2381_112_2_18_1_3_28_545,"AUDIO_SM_Resume, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d, time:%d.")
TRACE_MSG(ASM_MAIN_2392_112_2_18_1_3_28_546,"AUDIO_SM_Resume, success to ChangeThreadPriority assiTask:src_pri:%d, dest_pri:%d, time:%d.")
TRACE_MSG(ASM_MAIN_2416_112_2_18_1_3_29_547,"asm_main.c:AUDIO_SM_Resume:error:SCI_ERROR  result  is %d")
TRACE_MSG(ASM_MAIN_2492_112_2_18_1_3_29_548,"AUDIO_SM_TimeSeek: seek to end!")
TRACE_MSG(ASM_MAIN_2498_112_2_18_1_3_29_549,"%s, %d, input para error. hAudioHandle:0x%08x. offset:%d, file_total_len:%d.")
TRACE_MSG(ASM_MAIN_2516_112_2_18_1_3_29_550,"AUDIO_SM_TimeSeek: gen_operation->pSeek not exist!")
TRACE_MSG(ASM_MAIN_2568_112_2_18_1_3_29_551,"AUDIO_SM_Seek: seek to end!")
TRACE_MSG(ASM_MAIN_2574_112_2_18_1_3_29_552,"%s, %d, input para error. hAudioHandle:0x%08x. offset:%d, file_total_len:%d.")
TRACE_MSG(ASM_MAIN_2808_112_2_18_1_3_29_553,"%s, %d, audio_handle:0x%x, playing_state:%d, audio is not playing.")
TRACE_MSG(ASM_MAIN_2853_112_2_18_1_3_29_554,"asm_main.c:AUDIO_SM_IdentifyAudioFormat:error:ptFormatInfo == SCI_NULL")
TRACE_MSG(ASM_MAIN_2921_112_2_18_1_3_29_555,"AUDIO_SM_GetPlayingInfo, audio is not playing, state:%d.")
TRACE_MSG(ASM_MAIN_3136_112_2_18_1_3_30_556,"asm_main.c:AUDIO_SM_GetFileSize:error:PNULL==audio_obj")
TRACE_MSG(ASM_MAIN_3250_112_2_18_1_3_30_557,"%s, %d, uiSamplere==0.")
TRACE_MSG(ASM_MAIN_3267_112_2_18_1_3_30_558,"AUDIO_SM_SetOutputSampleRate handle:0x%x, samplerate:%d.")
TRACE_MSG(ASM_MAIN_3406_112_2_18_1_3_30_559,"AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x.")
TRACE_MSG(ASM_MAIN_3431_112_2_18_1_3_30_560,"AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 1.")
TRACE_MSG(ASM_MAIN_3445_112_2_18_1_3_31_561,"AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 2.")
TRACE_MSG(ASM_MAIN_3455_112_2_18_1_3_31_562,"AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 3 .")
TRACE_MSG(ASM_MAIN_3467_112_2_18_1_3_31_563,"AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 4.")
TRACE_MSG(ASM_MAIN_3474_112_2_18_1_3_31_564,"AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 5.")
TRACE_MSG(ASM_MAIN_3493_112_2_18_1_3_31_565,"AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 6.")
TRACE_MSG(ASM_MAIN_3495_112_2_18_1_3_31_566,"asm_main.c:AUDIO_SM_CreateRecordFileHandle:error:new_item == SCI_NULL")
TRACE_MSG(ASM_MAIN_3505_112_2_18_1_3_31_567,"AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 7.")
TRACE_MSG(ASM_MAIN_3560_112_2_18_1_3_31_568,"AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 8.")
TRACE_MSG(ASM_MAIN_3574_112_2_18_1_3_31_569,"AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 9.")
TRACE_MSG(ASM_MAIN_3576_112_2_18_1_3_31_570,"asm_main.c:AUDIO_SM_CreateRecordFileHandle:error:new_item == SCI_NULL")
TRACE_MSG(ASM_MAIN_3586_112_2_18_1_3_31_571,"AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 10.")
TRACE_MSG(ASM_MAIN_3641_112_2_18_1_3_31_572,"AUDIO:AUDIO_SM_CreateRecordFileHandle hAudioCodec:0x%08x, 0x%08x 11.")
TRACE_MSG(ASM_MAIN_3702_112_2_18_1_3_31_573,"AUDIO:AUDIO_SM_CreateRecordBufferHandle hAudioCodec:0x%08x")
TRACE_MSG(ASM_MAIN_3766_112_2_18_1_3_31_574,"asm_main.c:AUDIO_SM_CreateRecordBufferHandle:error:new_item == SCI_NULL")
TRACE_MSG(ASM_MAIN_3838_112_2_18_1_3_31_575,"asm_main.c:AUDIO_SM_CreateRecordBufferHandle:error:new_item == SCI_NULL")
TRACE_MSG(ASM_MAIN_3943_112_2_18_1_3_31_576,"AUDIO:AUDIO_SM_CreateNormalHandle hAudioCodec:0x%08x")
TRACE_MSG(ASM_MAIN_4001_112_2_18_1_3_32_577,"asm_main.c:AUDIO_SM_CreateNormalHandle:error:new_item == SCI_NULL")
TRACE_MSG(ASM_MAIN_4073_112_2_18_1_3_32_578,"asm_main.c:AUDIO_SM_CreateNormalHandle:error:new_item == SCI_NULL")
TRACE_MSG(ASM_MAIN_4162_112_2_18_1_3_32_579,"AUDIO:AUDIO_SM_CreateCloneHandle hAudioCodec:0x%08x, OrgHdl:%08X")
TRACE_MSG(ASM_MAIN_4220_112_2_18_1_3_32_580,"asm_main.c:AUDIO_SM_CreateNormalHandle:error:new_item == SCI_NULL")
TRACE_MSG(ASM_MAIN_4292_112_2_18_1_3_32_581,"asm_main.c:AUDIO_SM_CreateNormalHandle:error:new_item == SCI_NULL")
TRACE_MSG(ASM_MAIN_4364_112_2_18_1_3_32_582,"[AUDIO_SM_CreateCloneHandle] org output:%d, codec out %d.")
TRACE_MSG(ASM_MAIN_4403_112_2_18_1_3_32_583,"AUDIO_SM_SwitchDevice hAudioHandle:0x%x, hDevice:0x%x.")
TRACE_MSG(ASM_MAIN_4438_112_2_18_1_3_32_584,"AUDIO_SM_SwitchDevice srcDevice == destDevice:0x%x.")
TRACE_MSG(ASM_MAIN_4453_112_2_18_1_3_32_585,"AUDIO_SM_SwitchDevice AUDIO_DM_SwitchDevice failed.")
TRACE_MSG(ASM_MAIN_4535_112_2_18_1_3_33_586,"asm_main.c:AUDIO_SM_GetDecoderDataTime:error:audio_obj->uiOutputSamplerate==0")
TRACE_MSG(ASM_MAIN_4558_112_2_18_1_3_33_587,"%s, %d, thread_id is invalid.")
TRACE_MSG(ASM_MAIN_4583_112_2_18_1_3_33_588,"%s, %d, thread_id is invalid.")
TRACE_MSG(ASM_MAIN_4620_112_2_18_1_3_33_589,"asm_main.c:AUDIO_SM_PlayEnd:error:audio_obj == SCI_NULL")
TRACE_MSG(ASM_MAIN_4629_112_2_18_1_3_33_590,"asm_main.c:AUDIO_SM_PlayEnd:error:sig_out_ptr == SCI_NULL")
TRACE_MSG(ASM_MAIN_4635_112_2_18_1_3_33_591,"AUDIO_SM_PlayEnd hAudioHandle:0x%08x")
TRACE_MSG(ASM_MAIN_4672_112_2_18_1_3_33_592,"asm_main.c:AUDIO_SM_PauseEnd:error:audio_obj == SCI_NULL")
TRACE_MSG(ASM_MAIN_4682_112_2_18_1_3_33_593,"asm_main.c:AUDIO_SM_PauseEnd:error:sig_out_ptr == SCI_NULL")
TRACE_MSG(ASM_MAIN_4723_112_2_18_1_3_33_594,"asm_main.c:AUDIO_SM_SendErrorRep:error:audio_obj == SCI_NULL")
TRACE_MSG(ASM_MAIN_4732_112_2_18_1_3_33_595,"asm_main.c:AUDIO_SM_SendErrorRep:error:sig_out_ptr == SCI_NULL")
TRACE_MSG(ASM_MAIN_4776_112_2_18_1_3_33_596,"AUDIO_SM_AllocMemory, ptr:0x%08x, len:0x%x.")
TRACE_MSG(ASM_MAIN_4833_112_2_18_1_3_33_597,"asm_main.c:thread_entry:AUDIO_CREATE_FILE_REQ::temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_4880_112_2_18_1_3_33_598,"asm_main.c:thread_entry:AUDIO_CREATE_BUFFER_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_4928_112_2_18_1_3_33_599,"asm_main.c:thread_entry:AUDIO_CREATE_STREAMBUFFER_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_4977_112_2_18_1_3_34_600,"asm_main.c:thread_entry:AUDIO_CREATE_RECORD_FILE_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_5024_112_2_18_1_3_34_601,"asm_main.c:thread_entry:AUDIO_CREATE_RECORD_BUFFER_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_5066_112_2_18_1_3_34_602,"asm_main.c:thread_entry:AUDIO_CREATE_NORMAL_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_5109_112_2_18_1_3_34_603,"asm_main.c:thread_entry:AUDIO_CREATE_CLONE_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_5137_112_2_18_1_3_34_604,"ASM:AUDIO_PLAY_REQ handdle 0x%08x not existed!\r\n")
TRACE_MSG(ASM_MAIN_5162_112_2_18_1_3_34_605,"asm_main.c:thread_entry:AUDIO_PLAY_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_5187_112_2_18_1_3_34_606,"ASM:AUDIO_STOP_REQ handdle 0x%08xnot existed!\r\n")
TRACE_MSG(ASM_MAIN_5219_112_2_18_1_3_34_607,"asm_main.c:thread_entry:AUDIO_STOP_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_5253_112_2_18_1_3_34_608,"ASM:AUDIO_PAUSE_REQ handdle 0x%08x not existed!\r\n")
TRACE_MSG(ASM_MAIN_5285_112_2_18_1_3_34_609,"asm_main.c:thread_entry:AUDIO_PAUSE_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_5319_112_2_18_1_3_34_610,"ASM:AUDIO_RESUME_REQ handdle 0x%08x not existed!\r\n")
TRACE_MSG(ASM_MAIN_5344_112_2_18_1_3_34_611,"asm_main.c:thread_entry:AUDIO_RESUME_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_5369_112_2_18_1_3_34_612,"ASM:AUDIO_SEEK_REQ handdle 0x%08x not existed!\r\n")
TRACE_MSG(ASM_MAIN_5394_112_2_18_1_3_34_613,"asm_main.c:thread_entry:AUDIO_SEEK_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_5419_112_2_18_1_3_34_614,"ASM:AUDIO_SETSPEED_REQ handdle 0x%08x not existed!\r\n")
TRACE_MSG(ASM_MAIN_5444_112_2_18_1_3_34_615,"asm_main.c:thread_entry:AUDIO_SETSPEED_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_5469_112_2_18_1_3_34_616,"ASM:AUDIO_SETDIGIGAIN_REQ handdle 0x%08xnot existed!\r\n")
TRACE_MSG(ASM_MAIN_5494_112_2_18_1_3_35_617,"asm_main.c:thread_entry:AUDIO_SETDIGIGAIN_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_5520_112_2_18_1_3_35_618,"ASM:AUDIO_GETDIGIGAIN_REQ handdle 0x%08x not existed!\r\n")
TRACE_MSG(ASM_MAIN_5555_112_2_18_1_3_35_619,"asm_main.c:thread_entry:AUDIO_GETDIGIGAIN_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_5594_112_2_18_1_3_35_620,"AS: Received AUDIO_ERROR_RSP!")
TRACE_MSG(ASM_MAIN_5615_112_2_18_1_3_35_621,"asm_main.c:thread_entry:AUDIO_ERROR_REP:error:ptAudio == SCI_NULL")
TRACE_MSG(ASM_MAIN_5622_112_2_18_1_3_35_622,"asm_main.c:thread_entry:AUDIO_ERROR_REP::ptAudio->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_5635_112_2_18_1_3_35_623,"asm_main.c:thread_entry:AUDIO_ERROR_REP:error:verify handle error")
TRACE_MSG(ASM_MAIN_5641_112_2_18_1_3_35_624,"AS: Received AUDIO_PLAYEND_RSP!")
TRACE_MSG(ASM_MAIN_5657_112_2_18_1_3_35_625,"asm_main.c:thread_entry:AUDIO_PLAYEND_REP:error:verify handle error")
TRACE_MSG(ASM_MAIN_5665_112_2_18_1_3_35_626,"asm_main.c:thread_entry:AUDIO_PLAYEND_REP:error:ptAudio == SCI_NULL")
TRACE_MSG(ASM_MAIN_5671_112_2_18_1_3_35_627,"AUDIO_PLAYEND_REP, AUDIO_PLAYING_FLAG_STOPPED == ptAudio->ePlayingFlag")
TRACE_MSG(ASM_MAIN_5677_112_2_18_1_3_35_628,"AUDIO_PLAYEND_REP, AUDIO_PLAYING_FLAG_STARTPAUSE == ptAudio->ePlayingFlag")
TRACE_MSG(ASM_MAIN_5680_112_2_18_1_3_35_629,"AUDIO_PLAYEND_REP hAudioHandle:0x%08x")
TRACE_MSG(ASM_MAIN_5690_112_2_18_1_3_35_630,"asm_main.c:thread_entry:AUDIO_PLAYEND_REP:error:codec_handle == INVALID_HANDLE")
TRACE_MSG(ASM_MAIN_5704_112_2_18_1_3_35_631,"AUDIO_PLAYEND_REP pStopDeconstruct error:%d.")
TRACE_MSG(ASM_MAIN_5712_112_2_18_1_3_35_632,"asm_main.c:thread_entry:AUDIO_PLAYEND_REP:error:gen_operation != SCI_NULL")
TRACE_MSG(ASM_MAIN_5748_112_2_18_1_3_35_633,"asm_main.c:thread_entry:AUDIO_PLAYEND_REP:SCI_NULL == ptAudio->pNotifyCallbackFunc")
TRACE_MSG(ASM_MAIN_5760_112_2_18_1_3_35_634,"asm_main.c:thread_entry:AUDIO_PLAYEND_REP:SCI_NULL == ptAudio->pNotifyCallbackFunc")
TRACE_MSG(ASM_MAIN_5785_112_2_18_1_3_35_635,"AS: Received AUDIO_PAUSEEND_RSP!")
TRACE_MSG(ASM_MAIN_5801_112_2_18_1_3_35_636,"asm_main.c:thread_entry:AUDIO_PAUSEEND_REP:error:verify handle error")
TRACE_MSG(ASM_MAIN_5809_112_2_18_1_3_35_637,"asm_main.c:thread_entry:AUDIO_PAUSEEND_REP:error:ptAudio == SCI_NULL")
TRACE_MSG(ASM_MAIN_5822_112_2_18_1_3_35_638,"asm_main.c:thread_entry:AUDIO_PAUSEEND_REP:error:ptAudio == SCI_NULL")
TRACE_MSG(ASM_MAIN_5838_112_2_18_1_3_35_639,"asm_main.c:thread_entry:AUDIO_PAUSEEND_REP:error:result = gen_operation->pPauseDeconstruct")
TRACE_MSG(ASM_MAIN_5847_112_2_18_1_3_35_640,"asm_main.c:thread_entry:AUDIO_PAUSEEND_REP:error:gen_operation != SCI_NULL")
TRACE_MSG(ASM_MAIN_5880_112_2_18_1_3_35_641,"asm_main.c:thread_entry:AUDIO_PAUSEEND_REP:SCI_NULL == ptAudio->pNotifyCallbackFunc")
TRACE_MSG(ASM_MAIN_5905_112_2_18_1_3_35_642,"ASM:AUDIO_CLOSE_HANDLE_REQ handdle 0x%08x  is not existed!\r\n")
TRACE_MSG(ASM_MAIN_5930_112_2_18_1_3_35_643,"asm_main.c:thread_entry:AUDIO_CLOSE_HANDLE_REQ:SCI_NULL == ptAudio->pNotifyCallbackFunc")
TRACE_MSG(ASM_MAIN_5959_112_2_18_1_3_35_644,"ASM:AUDIO_GETDIGIGAIN_REQ handdle 0x%08x not existed!\r\n")
TRACE_MSG(ASM_MAIN_5989_112_2_18_1_3_36_645,"asm_main.c:thread_entry:AUDIO_SWITCH_DEVICE_REQ:SCI_NULL == ptAudio->pNotifyCallbackFunc")
TRACE_MSG(ASM_MAIN_6012_112_2_18_1_3_36_646,"asm_main.c:thread_entry:AUDIO_SWITCH_DEVICE_REQ:error:verify handle error")
TRACE_MSG(ASM_MAIN_6029_112_2_18_1_3_36_647,"ASM:Receive the Trackbuf full signal so later!\r\n")
TRACE_MSG(ASM_MAIN_6049_112_2_18_1_3_36_648,"asm_main.c:thread_entry:AUDIO_TRACKBUF_STATUS_REP:ptAudio->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_6061_112_2_18_1_3_36_649,"ASM:Trackbuf Empty,SCI_ChangeThreadPriority! ")
TRACE_MSG(ASM_MAIN_6070_112_2_18_1_3_36_650,"asm_main.c:thread_entry:AUDIO_TRACKBUF_STATUS_REP:ptAudio->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_6083_112_2_18_1_3_36_651,"AS: Received AUDIO_TRACKBUF_STATUS_RSP!")
TRACE_MSG(ASM_MAIN_6210_112_2_18_1_3_36_652,"AS: Received AUDIO_CHANGE_CODEC_TASK_PRIORITY_REQ! time:%d, result:%d.")
TRACE_MSG(ASM_MAIN_6222_112_2_18_1_3_36_653,"AS: Received AUDIO_SET_TRACK_ON_REQ! audio:0x%08x, on:%d, result:%d")
TRACE_MSG(ASM_MAIN_6250_112_2_18_1_3_36_654,"AUDIO_EXE_CODEC_EXT_OPE_REQ uiExtOpePointer APM_EXT_ENTER failed:%d.")
TRACE_MSG(ASM_MAIN_6254_112_2_18_1_3_36_655,"AudioThread: AUDIO_CM_ExitDispose 1 failed:%d.")
TRACE_MSG(ASM_MAIN_6260_112_2_18_1_3_36_656,"AudioThread: AUDIO_CM_EnterDispose failed:%d.")
TRACE_MSG(ASM_MAIN_6272_112_2_18_1_3_36_657,"AudioThread: AUDIO_CM_ExitDispose 2 failed:%d.")
TRACE_MSG(ASM_MAIN_6277_112_2_18_1_3_36_658,"AudioThread: uiExtOpePointer APM_EXT_EXIT failed:%d.")
TRACE_MSG(ASM_MAIN_6298_112_2_18_1_3_36_659,"ASM:AUDIO_SEEK_REQ handdle 0x%08x not existed!\r\n")
TRACE_MSG(ASM_MAIN_6323_112_2_18_1_3_36_660,"asm_main.c:thread_entry:AUDIO_SEEK_REQ:temp_sig_in_ptr->pNotifyCallbackFunc == SCI_NULL")
TRACE_MSG(ASM_MAIN_6347_112_2_18_1_3_36_661,"ASM:AUDIO_GETPLAYINGINFO_REQ handdle 0x%08x not existed!\r\n")
TRACE_MSG(ASM_MAIN_6397_112_2_18_1_3_36_662,"asm_main.c:AUDIO_STOPSELF_IND process :error:SCI_ERROR == SCI_ChangeThreadPriority")
TRACE_MSG(ASM_MAIN_6400_112_2_18_1_3_36_663,"AUDIO_SM_Stop, success to ChangeThreadPriority decode task:src_pri:%d, dest_pri:%d.")
TRACE_MSG(ASM_MAIN_6406_112_2_18_1_3_36_664,"asm_main.c:AUDIO_SM_Stop:error:SCI_ERROR == SCI_ChangeThreadPriority 2")
TRACE_MSG(ASM_MAIN_6409_112_2_18_1_3_36_665,"AUDIO_SM_Stop, success to ChangeThreadPriority assi task:src_pri:%d, dest_pri:%d.")
TRACE_MSG(ASM_MAIN_6420_112_2_18_1_3_36_666,"asm_main.c:AUDIO_STOPSELF_IND process :error:SCI_ERROR == SCI_ChangeThreadPriority")
TRACE_MSG(ASM_MAIN_6423_112_2_18_1_3_36_667,"AUDIO_SM_Stop, success to ChangeThreadPriority decode task:src_pri:%d, dest_pri:%d.")
TRACE_MSG(ASM_MAIN_6429_112_2_18_1_3_36_668,"asm_main.c:AUDIO_SM_Stop:error:SCI_ERROR == SCI_ChangeThreadPriority 2")
TRACE_MSG(ASM_MAIN_6432_112_2_18_1_3_36_669,"AUDIO_SM_Stop, success to ChangeThreadPriority assi task:src_pri:%d, dest_pri:%d.")
TRACE_MSG(ASM_MAIN_6487_112_2_18_1_3_37_670,"ASM:AUDIO_SM_TrackbufTimer_Callback!")
TRACE_MSG(ASM_MAIN_6504_112_2_18_1_3_37_671,"asm_main.c:AUDIO_SM_TrackbufTimer_Callback:error:sig_out_ptr == SCI_NULL")
TRACE_MSG(ASM_MAIN_6519_112_2_18_1_3_37_672,"ASM:Continue to Check Trackbuf!uiTimerExpire: %d.\r\n")
TRACE_MSG(ASM_MAIN_6536_112_2_18_1_3_37_673,"asm_main.c:AUDIO_SM_TrackbufTimer_Callback:error:sig_out_ptr == SCI_NULL")
TRACE_MSG(ASM_MAIN_6566_112_2_18_1_3_37_674,"ASM:AUDIO_SM_TrackOnTimer_Callback! time:%d.")
TRACE_MSG(ASM_MAIN_6577_112_2_18_1_3_37_675,"asm_main.c:AUDIO_SM_TrackOnTimer_Callback:error:sig_out_ptr == SCI_NULL")
TRACE_MSG(ASM_MAIN_6614_112_2_18_1_3_37_676,"asm_main.c:AUDIO_SM_LookforAppropriateCodec:error:audio_obj == SCI_NULL")
TRACE_MSG(ASM_MAIN_6640_112_2_18_1_3_37_677,"asm_main.c:AUDIO_SM_LookforAppropriateCodec:error:SCI_NULL == ptCodecExtCfgInfo")
TRACE_MSG(ASM_MAIN_6657_112_2_18_1_3_37_678,"AUDIO_SM_LookforAppropriateCodec, construct error, codec handle:0x%08x.")
TRACE_MSG(ASM_MAIN_6671_112_2_18_1_3_37_679,"AUDIO_SM_LookforAppropriateCodec pDeconstruct error:%d.")
TRACE_MSG(ASM_MAIN_6711_112_2_18_1_3_37_680,"%s, %d, error.")
TRACE_MSG(ASM_MAIN_6721_112_2_18_1_3_37_681,"asm_mai.c:AUDIO_SM_IdentifyFormat:error:gen_operation_ptr==PNULL")
TRACE_MSG(ASM_MAIN_6730_112_2_18_1_3_37_682,"asm_mai.c:AUDIO_SM_IdentifyFormat:error:ptCodecExtCfgInfo==PNULL")
TRACE_MSG(ASM_MAIN_6749_112_2_18_1_3_37_683,"%s, %d, error. hAudioHandle:0x%08x, hCodecHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_6755_112_2_18_1_3_37_684,"%s, %d, error. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_6770_112_2_18_1_3_37_685,"AUDIO_SM_IdentifyFormat pDeconstruct error:%d.")
TRACE_MSG(ASM_MAIN_6777_112_2_18_1_3_37_686,"AUDIO_SM_IdentifyFormat, not verify format, codec::0x%08x.")
TRACE_MSG(ASM_MAIN_6786_112_2_18_1_3_37_687,"AUDIO_SM_IdentifyFormat, can't find appropriate codec.")
TRACE_MSG(ASM_MAIN_6801_112_2_18_1_3_37_688,"AUDIO_SM_IdentifyFormat, handle:0x%08x, total_len:%d, total_time:%d.")
TRACE_MSG(ASM_MAIN_6811_112_2_18_1_3_37_689,"%s, %d, uiOutputSamplerate is zero, codec handle:0x%x.")
TRACE_MSG(ASM_MAIN_6951_112_2_18_1_3_37_690,"%s, %d, error. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_6958_112_2_18_1_3_37_691,"%s, %d, error. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_6985_112_2_18_1_3_38_692,"%s, %d, error. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_6992_112_2_18_1_3_38_693,"%s, %d, error. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_7024_112_2_18_1_3_38_694,"AUDIO:AUDIO_SM_ChangeCodecPriority hAudioHandle:0x%08x,bHighPriority:%d")
TRACE_MSG(ASM_MAIN_7030_112_2_18_1_3_38_695,"%s, %d, error. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_7037_112_2_18_1_3_38_696,"%s, %d, error. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_7056_112_2_18_1_3_38_697,"AUDIO_SM_ChangeCodecPriority, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d.")
TRACE_MSG(ASM_MAIN_7070_112_2_18_1_3_38_698,"AUDIO_SM_ChangeCodecPriority, success to ChangeAssiThreadPriority:src_pri:%d, dest_pri:%d.")
TRACE_MSG(ASM_MAIN_7102_112_2_18_1_3_38_699,"AUDIO_SM_GetCodecPriorityBeHigh audio handle error. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_7109_112_2_18_1_3_38_700,"AUDIO_SM_GetCodecPriorityBeHigh audio handle not exist. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_7133_112_2_18_1_3_38_701,"AUDIO_SM_GetCodecPriorityBeHigh, codec task pri unnormal: %d, low:%d, high:%d, hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_7139_112_2_18_1_3_38_702,"AUDIO_SM_GetCodecPriorityBeHigh, play state unnormal:%d. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_7163_112_2_18_1_3_38_703,"AUDIO_SM_PauseOutputData audio_obj is null.")
TRACE_MSG(ASM_MAIN_7168_112_2_18_1_3_38_704,"AUDIO_SM_PauseOutputData hAudioHandle:0x%08x")
TRACE_MSG(ASM_MAIN_7181_112_2_18_1_3_38_705,"AUDIO_SM_PauseOutputData:error:sig_out_ptr == SCI_NULL")
TRACE_MSG(ASM_MAIN_7222_112_2_18_1_3_38_706,"AUDIO_SM_GetDebugOutputDataFileCount pusFileCount is PNULL.")
TRACE_MSG(ASM_MAIN_7230_112_2_18_1_3_38_707,"AUDIO_SM_GetDebugOutputDataFileCount audio handle error. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_7237_112_2_18_1_3_38_708,"AUDIO_SM_GetDebugOutputDataFileCount audio handle not exist. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_7290_112_2_18_1_3_38_709,"AUDIO_SM_EnableSomePosDebugOutputDataToFile audio handle error. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_7297_112_2_18_1_3_38_710,"AUDIO_SM_EnableSomePosDebugOutputDataToFile audio handle not exist. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_7314_112_2_18_1_3_38_711,"AUDIO_SM_EnableSomePosDebugOutputDataToFile pos:%d should less than max:%d")
TRACE_MSG(ASM_MAIN_7327_112_2_18_1_3_38_712,"AUDIO_SM_EnableSomePosDebugOutputDataToFile pbEnableDebugDataOutput create memory failed, sizeof:%d.")
TRACE_MSG(ASM_MAIN_7364_112_2_18_1_3_38_713,"AUDIO_SM_EnableOutputDataToFile audio handle error. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_7371_112_2_18_1_3_38_714,"AUDIO_SM_EnableOutputDataToFile audio handle not exist. hAudioHandle:0x%08x.")
TRACE_MSG(ASM_MAIN_7407_112_2_18_1_3_38_715,"AUDIO_SM_OfferDebugDataFileName VerifyHandle failed.")
TRACE_MSG(ASM_MAIN_7418_112_2_18_1_3_38_716,"AUDIO_SM_OfferDebugDataFileName audio_obj is null.")
TRACE_MSG(ASM_MAIN_7435_112_2_18_1_3_38_717,"AUDIO_SM_OfferDebugDataFileName the name has been offered.")
TRACE_MSG(ASM_MAIN_7464_112_2_18_1_3_38_718,"AUDIO_SM_OfferDebugDataFileName hFileHanlde create failed Left:%s.")
TRACE_MSG(ASM_MAIN_7478_112_2_18_1_3_38_719,"AUDIO_SM_OfferDebugDataFileName hFileHanlde create failed Right:%s.")
TRACE_MSG(ASM_MAIN_7485_112_2_18_1_3_38_720,"AUDIO_SM_OfferDebugDataFileName hFileHanlde create memory failed size:%d.")
TRACE_MSG(ASM_MAIN_7499_112_2_18_1_3_39_721,"AUDIO_SM_EnableSomePosDebugOutputDataToFile pbEnableDebugDataOutput create memory failed, sizeof:%d.")
TRACE_MSG(ASM_MAIN_7580_112_2_18_1_3_39_722,"AUDIO_SM_ConfigIfExpWorking exp:0x%x, ifWork:%d, index:%d, free 1.")
TRACE_MSG(ASM_MAIN_7589_112_2_18_1_3_39_723,"AUDIO_SM_ConfigIfExpWorking exp:0x%x, ifWork:%d, index:%d, free 2.")
TRACE_MSG(ASM_MAIN_7613_112_2_18_1_3_39_724,"AUDIO_SM_ConfigIfExpWorking exp:0x%x, ifWork:%d, index:%d, add 1.")
TRACE_MSG(ASM_MAIN_7618_112_2_18_1_3_39_725,"AUDIO_SM_ConfigIfExpWorking alloc error 1, size:%d.")
TRACE_MSG(ASM_MAIN_7640_112_2_18_1_3_39_726,"AUDIO_SM_ConfigIfExpWorking exp:0x%x, ifWork:%d, index:%d, add 2.")
TRACE_MSG(ASM_MAIN_7646_112_2_18_1_3_39_727,"AUDIO_SM_ConfigIfExpWorking alloc error 2, size:%d.")
END_TRACE_MAP(MS_REF_AUDIOSERVICE_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_AUDIOSERVICE_TRC_H_

