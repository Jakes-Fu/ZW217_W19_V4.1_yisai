/******************************************************************************
 ** File Name:      mmi_app_audio_trc.h                                         *
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
//trace_id:151
#ifndef _MMI_APP_AUDIO_TRC_H_
#define _MMI_APP_AUDIO_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIAUDIO_127_112_2_18_2_3_26_0 "[MMISRV]: GetHandleByType() exit, type=%d, handle=%d"
#define MMIAUDIO_186_112_2_18_2_3_26_1 "[MMISRV]: GetTypeInfo() exit, type=%d, type_ptr=%d"
#define MMIAUDIO_194_112_2_18_2_3_26_2 "[MMISRV]: SetCurType() exit, cur_type=%d"
#define MMIAUDIO_238_112_2_18_2_3_26_3 "[MMISRV]: ConvertTypeToFmt() exit, ring_type=%d, fmt=%d"
#define MMIAUDIO_273_112_2_18_2_3_27_4 "[MMISRV]: ConvertEqMode() exit, eq_mode=%d, eq=%d"
#define MMIAUDIO_306_112_2_18_2_3_27_5 "[MMISRV]: ConvertDevMode() exit, mode=%d, route=%d"
#define MMIAUDIO_340_112_2_18_2_3_27_6 "[MMISRV]: ConvertRoute() exit, route=%d, mode=%d"
#define MMIAUDIO_370_112_2_18_2_3_27_7 "[MMISRV]: ConvertRate() exit, rate=%d, srv_rate=%d"
#define MMIAUDIO_470_112_2_18_2_3_27_8 "[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x"
#define MMIAUDIO_476_112_2_18_2_3_27_9 "[MMISRV]: HandleNotify(), result=%d, report_ptr=0x%x"
#define MMIAUDIO_480_112_2_18_2_3_27_10 "[MMISRV]: HandleNotify(), report_ptr->report=%d"
#define MMIAUDIO_568_112_2_18_2_3_27_11 "[MMISRV]: MMIAUDIO_SetVolume() entry, mode=%d, speaker_vol=%d"
#define MMIAUDIO_612_112_2_18_2_3_27_12 "[MMISRV]: MMIAUDIO_IsInVoiceLoop() entry, enable=%d"
#define MMIAUDIO_653_112_2_18_2_3_27_13 "[MMISRV]: MMIAUDIO_GetIsPlaying() entry, Is not playing!!!"
#define MMIAUDIO_659_112_2_18_2_3_27_14 "[MMISRV]: MMIAUDIO_GetIsPlaying() entry, Is playing!!!"
#define MMIAUDIO_731_112_2_18_2_3_28_15 "[MMISRV]: MMIAUDIO_IsSeekSupport() exit, audio type = %d, result = %d"
#define MMIAUDIO_764_112_2_18_2_3_28_16 "[MMISRV]: MMIAUDIO_AudioPlay() entry, handle=%d, type_ptr=%d"
#define MMIAUDIO_786_112_2_18_2_3_28_17 "[MMISRV]: MMIAUDIO_AudioPlay() exit, res=%d"
#define MMIAUDIO_806_112_2_18_2_3_28_18 "[MMISRV]: MMIAUDIO_AudioPlay() entry, handle=%d, type_ptr=%d"
#define MMIAUDIO_828_112_2_18_2_3_28_19 "[MMISRV]: MMIAUDIO_AudioPlay() exit, res=%d"
#define MMIAUDIO_850_112_2_18_2_3_28_20 "[MMISRV]: MMIAUDIO_AudioPause() exit, handle=%d, res=%d"
#define MMIAUDIO_881_112_2_18_2_3_28_21 "[MMISRV]: MMIAUDIO_AudioResume() exit, handle=%d, res=%d"
#define MMIAUDIO_911_112_2_18_2_3_28_22 "[MMISRV]: MMIAUDIO_AudioStop() exit, handle=%d, res=%d"
#define MMIAUDIO_931_112_2_18_2_3_28_23 "[MMISRV]: MMIAUDIO_AudioSeek() exit, handle=%d, offset, res=%d"
#define MMIAUDIO_961_112_2_18_2_3_28_24 "[MMISRV]: MMIAUDIO_AudioSeekByTime() exit, handle=0x%x, time_offset=%d, byte_offset=%d, res=%d"
#define MMIAUDIO_982_112_2_18_2_3_28_25 "[MMISRV]: MMIAUDIO_PlayTone() entry, handle=%d, tone_type=%d, tone_id=%d, duration=%d,freq=%d"
#define MMIAUDIO_1017_112_2_18_2_3_28_26 "[MMISRV]: MMIAUDIO_PlayTone() exit, handle=0x%x, res=%d"
#define MMIAUDIO_1052_112_2_18_2_3_28_27 "[MMISRV]: MMIAUDIO_StopTone() exit, handle=%d, res=%d"
#define MMIAUDIO_1100_112_2_18_2_3_28_28 "[MMIAUDIO]:MMIAUDIO_StartIQData:s_record_handle=%lx"
#define MMIAUDIO_1181_112_2_18_2_3_28_29 "[MMIAUDIO]:MMIAUDIO_StartRecord exit: handle=0x%x, res = %d"
#define MMIAUDIO_1201_112_2_18_2_3_28_30 "[MMIAUDIO]:MMIAUDIO_EndRecord exit: handle=0x%x, res = %d"
#define MMIAUDIO_1234_112_2_18_2_3_29_31 "[MMIAUDIO]:MMIAUDIO_StopRecord exit: handle=0x%x, res = %d"
#define MMIAUDIO_1257_112_2_18_2_3_29_32 "[MMIAUDIO]:MMIAUDIO_PauseRecord exit: handle=0x%x, res = %d"
#define MMIAUDIO_1280_112_2_18_2_3_29_33 "[MMIAUDIO]:MMIAUDIO_ResumeRecord exit: handle=0x%x, res = %d"
#define MMIAUDIO_1330_112_2_18_2_3_29_34 "[MMIAUDIO]:MMIAUDIO_PlayFM exit: handle=0x%x, res = %d"
#define MMIAUDIO_1359_112_2_18_2_3_29_35 "[MMIAUDIO]:MMIAUDIO_StopFM exit: handle=0x%x, res = %d"
#define MMIAUDIO_1417_112_2_18_2_3_29_36 "[MMISRV]: MMIAUDIO_SetEQMode() exit, eq_mode=%d, eq=%d, res=%d"
#define MMIAUDIO_1450_112_2_18_2_3_29_37 "[MMISRV]: MMIAUDIO_SetVolumeDeviceModeExt() entry"
#define MMIAUDIO_1485_112_2_18_2_3_29_38 "[MMISRV]: MMIAUDIO_SerDeviceModeInCall() exit, mode=%d,handle=0x%x,res=%d"
#define MMIAUDIO_1530_112_2_18_2_3_29_39 "[MMISRV]: MMIAUDIO_SwitchDeviceExt() exit, handle=%d, device=%d,res=%d"
#define MMIAUDIO_1542_112_2_18_2_3_29_40 "[MMISRV]: MMIAUDIO_GetAudioHandle() exit, handle=0x%x"
#define MMIAUDIO_1580_112_2_18_2_3_29_41 "[MMISRV]:MMIAUDIO_GetIsPermitPlay:cur_type=%d, audio_htype=%d"
#define MMIAUDIO_1602_112_2_18_2_3_29_42 "[MMISRV]:MMIAUDIO_GetAudioHtype:cur_type=%d"
#define MMIAUDIO_2447_112_2_18_2_3_31_43 "[MMIAUDIO]:AudioEndCallback:haudiohandle=0x%X, s_cur_audio_handle_type=%d!"
#define MMIAUDIO_2603_112_2_18_2_3_31_44 "[MMIAUDIO]:AudioStopCallback:haudiohandle=0x%X, s_cur_audio_handle_type=%d!"
#define MMIAUDIO_2694_112_2_18_2_3_31_45 "[MMIAUDIO]:AudioNotifyCallback:enter:hAudio = 0x%X, notify_info = 0x%X, affix_info = %ld"
#define MMIAUDIO_2762_112_2_18_2_3_32_46 "[MMIAUDIO]:MMIAPIAUDIO_HandleCallBackMsg:handle = 0x%X,msg_id = 0x%X, aud_result = %d, s_cur_audio_handle_type = %d, IsEnd=%d,app_handle = 0x%X, player_handle = 0x%X, music_handle = 0x%X, kuro_handle = 0x%X, normal_handle = 0x%X"
#define MMIAUDIO_2764_112_2_18_2_3_32_47 "[MMIAUDIO]:MMIAPIAUDIO_HandleCallBackMsg:app_callback = 0x%X,player_callback = 0x%X, music_callback = 0x%X, kuro_callback = 0x%X,normal_callback = 0x%X"
#define MMIAUDIO_2789_112_2_18_2_3_32_48 "[MMIAUDIO]:MMIAPIAUDIO_HandleCallBackMsg,  send play error"
#define MMIAUDIO_2846_112_2_18_2_3_32_49 "[MMIAUDIO]:RemovePlayCallback: hAudiohandle =0x%x, i = %d"
#define MMIAUDIO_2873_112_2_18_2_3_32_50 "[MMIAUDIO]:IsPlayCallbackValid: hAudiohandle =0x%x, i = %d, result"
#define MMIAUDIO_2909_112_2_18_2_3_32_51 "[MMIAUDIO]:GetAudioCodec:audio_codec=0x%x"
#define MMIAUDIO_2969_112_2_18_2_3_32_52 "[MMIAUDIO]:GetAudioCodec:ring_type=%d"
#define MMIAUDIO_3083_112_2_18_2_3_32_53 "[MMIAUDIO]:GetAudioDevice: ring_type = %d, hDevice = 0x%x"
#define MMIAUDIO_3168_112_2_18_2_3_32_54 "[MMIAUDIO]:CreateAudioBufferHandle:ring_type error!!!! orig = %x, new = %x, aud_result = %d"
#define MMIAUDIO_3173_112_2_18_2_3_32_55 "[MMIAUDIO]:CreateAudioBufferHandle:ring_type=%d, data_ptr = 0x%x, data_len = %d, audiohandle = 0x%x"
#define MMIAUDIO_3220_112_2_18_2_3_33_56 "[MMIAUDIO]:CreateAudioBufferHandle:ring_type error!!!! orig = %x, new = %x, aud_result = %d"
#define MMIAUDIO_3224_112_2_18_2_3_33_57 "[MMIAUDIO]:CreateAudioFileHandle:ring_type=%d, audiohandle = 0x%x"
#define MMIAUDIO_3280_112_2_18_2_3_33_58 "[MMIAUDIO]:CreateAudioBufferHandle:ring_type error!!!! orig = %x, new = %x, aud_result = %d"
#define MMIAUDIO_3285_112_2_18_2_3_33_59 "[MMIAUDIO]:CreateAudioStreamingHandle:ring_type=%d, data_ptr = 0x%x, data_len = %d, audiohandle = 0x%x"
#define MMIAUDIO_3762_112_2_18_2_3_34_60 "[MMIAUDIO]:MMIAUDIO_AudioPlay,  audio_handle_type = 0x%x, uiOffset = %d, play_times = %d, func = 0x%x, aud_result = %d"
#define MMIAUDIO_3850_112_2_18_2_3_34_61 "[MMIAUDIO]:MMIAUDIO_AudioPlayAsy,  audio_handle_type = 0x%x, uiOffset = %d, play_times = %d, func = 0x%x, aud_result = %d"
#define MMIAUDIO_3883_112_2_18_2_3_34_62 "[MMIAUDIO]:HandleAudioPriority, hAudioHandle = 0x%x, aud_result = %d"
#define MMIAUDIO_3894_112_2_18_2_3_34_63 "[MMIAUDIO]:MMIAUDIO_AudioStop:audio_type=%d, cur_type = %d, app_handle = 0x%X, player_handle = 0x%X, music_handle = 0x%X, kuro_handle = 0x%X,normal_handle = 0x%X"
#define MMIAUDIO_4032_112_2_18_2_3_34_64 "[MMIAUDIO]:MMIAUDIO_CloseAudioHandle, Alert!, s_cur_audio_handle_type = %d, audio_handle_type = %d"
#define MMIAUDIO_4104_112_2_18_2_3_34_65 "[MMIAUDIO]: MMIAUDIO_CloseAudioHandle:audio_handle_type=%d, aud_result = %d"
#define MMIAUDIO_4196_112_2_18_2_3_34_66 "[MMIAUDIO]:MMIAUDIO_AudioPause:audio_handle_type=%d,aud_result = %d"
#define MMIAUDIO_4279_112_2_18_2_3_35_67 "[MMIAUDIO]:MMIAUDIO_AudioResume:audio_handle_type=%d,aud_result = %d"
#define MMIAUDIO_4352_112_2_18_2_3_35_68 "[MMIAUDIO]:MMIAUDIO_AudioSeek:audio_handle_type = %d, uiOffset=%ld,audio_result = %d"
#define MMIAUDIO_4404_112_2_18_2_3_35_69 "[MMIAUDIO]: MMIAUDIO_AudioSeekByTime, audio_handle_type = %d, time_offset = %d, byte_offset = %d, audio_result = %d"
#define MMIAUDIO_4464_112_2_18_2_3_35_70 "[MMIAUDIO]: MMIAUDIO_IsSeekSupport, audio type = %d, result = %d"
#define MMIAUDIO_4486_112_2_18_2_3_35_71 "[MMIAUDIO]:AudioPlayStop:hAudioHandle = %x, stop_result=%ld,close_result = %d"
#define MMIAUDIO_4495_112_2_18_2_3_35_72 "[MMIAUDIO]:MMIAUDIO_AudioSetSpeed:audio_handle_type=%d, uiSpeedX=%ld"
#define MMIAUDIO_4594_112_2_18_2_3_35_73 "[MMIAUDIO]:MMIAUDIO_AudioGetFileInfo:audio_handle_type=%d,aud_result = %d"
#define MMIAUDIO_4669_112_2_18_2_3_35_74 "[MMIAUDIO]:MMIAUDIO_AudioGetPlayingFileInfo:audio_handle_type=%x, cur_time = %d, cur_offset = %d, t_time = %d, t_len = %d, aud_result = %d"
#define MMIAUDIO_4728_112_2_18_2_3_36_75 "[MMIAUDIO]:MMIAUDIO_OpenSpectrum:audio_handle_type=%x, aud_result = %d"
#define MMIAUDIO_4851_112_2_18_2_3_36_76 "[MMIAUDIO]:MMIAUDIO_CloseSpectrum:audio_handle_type=%x, aud_result = %d"
#define MMIAUDIO_5007_112_2_18_2_3_36_77 "[MMIAUDIO]:MMIAUDIO_StartRecord:s_record_handle=0x%lx, file_handle = 0x%x, record_type = %d, aud_result = %d, result = %d"
#define MMIAUDIO_5039_112_2_18_2_3_36_78 "[MMIAUDIO]:MMIAUDIO_StopRecord:s_record_handle = %x, aud_result=%ld, result = %d"
#define MMIAUDIO_5053_112_2_18_2_3_36_79 "[MMIAUDIO]:MMIAUDIO_EndRecord:s_record_handle = %x"
#define MMIAUDIO_5069_112_2_18_2_3_36_80 "[MMIAUDIO]:MMIAUDIO_EndRecord:aud_result=%ld, result = %d"
#define MMIAUDIO_5091_112_2_18_2_3_36_81 "[MMIAUDIO]:MMIAUDIO_PauseRecord:s_record_handle = %x, aud_result=%ld, result = %d"
#define MMIAUDIO_5113_112_2_18_2_3_36_82 "[MMIAUDIO]:MMIAUDIO_ResumeRecord:s_record_handle = %x, aud_result=%ld, result = %d"
#define MMIAUDIO_5140_112_2_18_2_3_36_83 "[MMIAUDIO]:MMIAUDIO_GetIsPermitPlay:s_cur_audio_handle_type=%d, audio_htype=%d"
#define MMIAUDIO_5172_112_2_18_2_3_36_84 "[MMIAUDIO]:MMIAUDIO_GetAudioHandle:audio_handle_type=%d"
#define MMIAUDIO_5266_112_2_18_2_3_37_85 "[MMIAUDIO]:MMIAUDIO_PlayTone:audio_type=%d"
#define MMIAUDIO_5346_112_2_18_2_3_37_86 "[MMIAUDIO]:MMIAUDIO_StopTone:audio_type=%d"
#define MMIAUDIO_5397_112_2_18_2_3_37_87 "[MMIAUDIO]: MMIAUDIO_SetEQMode(), input error, MMI eq_mode = %d, "
#define MMIAUDIO_5430_112_2_18_2_3_37_88 "[MMIAUDIO]: MMIAUDIO_SetEQMode(), orig_EQ = %d, new_EQ = %d, aud_result = %d"
#define MMIAUDIO_5500_112_2_18_2_3_37_89 "[MMIAUDIO] MMIAUDIO_SetPlayRate:rate = %d"
#define MMIAUDIO_5515_112_2_18_2_3_37_90 "[MMIAUDIO]: MMIAUDIO_SetPlayRate(), orig_RATE = %d, new_RATE = %d, aud_result = %d"
#define MMIAUDIO_5590_112_2_18_2_3_37_91 "[MMIAUDIO]:MMIAUDIO_SetVolume(), orig_mod = %d,orig_vol = %d,new_mod = %d,new_vol = %d, aud_result_dev = %d,aud_result_vol = %d"
#define MMIAUDIO_5624_112_2_18_2_3_37_92 "[MMIAUDIO]: MMIAUDIO_SetCurDevMode: orig_dev=%d, new_dev = %d, aud_result = %d"
#define MMIAUDIO_5668_112_2_18_2_3_37_93 "[MMIAUDIO]: MMIAUDIO_SetMute: orig_is_mute=%d, new_is_mute = %d, aud_result = %d"
#define MMIAUDIO_5701_112_2_18_2_3_38_94 "[MMIAUDIO]: MMIAUDIO_SetUpLinkMute: is_mute=%d,aud_result = %d"
#define MMIAUDIO_5749_112_2_18_2_3_38_95 "[MMIAUDIO]:MMIAUDIO_SwitchDeviceExt,  audio_handle_type = 0x%x, hAudioHandle = 0x%x, format_type = %d, device = %d, aud_result = %d"
#define MMIAUDIO_5789_112_2_18_2_3_38_96 "[MMIAUDIO]:MMIAUDIO_StartIQData BUFFER ALLOC FAILED!!!!!!!!!!"
#define MMIAUDIO_5804_112_2_18_2_3_38_97 "[MMIAUDIO]:MMIAUDIO_StartIQData:s_record_handle=%lx"
#define MMIAUDIO_5849_112_2_18_2_3_38_98 "[MMIAUDIO]:MMIAUDIO_GetRecordEclipseTime, time = %d!"
#define MMIAUDIO_5874_112_2_18_2_3_38_99 "[MMIAUDIO]:MMIAUDIO_GetRecordVoiceDB, db = %d!"
#define MMIAUDIO_5935_112_2_18_2_3_38_100 "[MMIAUDIO]: MMIAUDIO_EnableVoiceCodec: is_enable=%d, aud_result = %d"
#define MMIAUDIO_5960_112_2_18_2_3_38_101 "[MMIAUDIO]: MMIAUDIO_SerDeviceModeInCall: orig_dev=%d, new_dev = %d, aud_result = %d"
#define MMIAUDIO_5990_112_2_18_2_3_38_102 "[MMIAUDIO]: MMIAUDIO_EnableVoiceCodec: is_loop_enable=%d, is_force_dev_mode = %d,force_dev_mode = %d,,volume =%d"
#define MMIAUDIO_6033_112_2_18_2_3_38_103 "[MMIAUDIO]: MMIAUDIO_IsInVoiceLoop: s_is_voice_loop_enable = %d"
#define MMIAUDIO_6045_112_2_18_2_3_38_104 "[MMIAUDIO]: MMIAUDIO_IsVoiceLoopForceMode: s_is_voice_loop_force_mode = %d"
#define MMIAUDIO_6059_112_2_18_2_3_38_105 "MMIAUDIO_SM_GetAudioObject true"
#define MMIAUDIO_6064_112_2_18_2_3_38_106 "MMIAUDIO_SM_GetAudioObject False"
#define MMIAUDIO_6107_112_2_18_2_3_38_107 "[MMIAUDIO]:MMIAUDIO_SetVolumeDeviceModeExt, orig_mod = %d,orig_vol = %d,new_mod = %d,new_vol = %d, aud_result_dev = %d,aud_result_vol = %d"
#define MMIAUDIO_6159_112_2_18_2_3_38_108 "[MMIAUDIO]:MMIAUDIO_SetMidiLowPolyNum, audio_htype = %d, midi_handle = 0x%x, poly_num = %d,aud_result = %d"
#define MMIAUDIO_6190_112_2_18_2_3_39_109 "[mmiaudio] MMIAUDIO_StartAGC dev_mode = %d, eq_mode = %d"
#define MMIAUDIO_6214_112_2_18_2_3_39_110 "[mmiaudio] MMIAUDIO_StopAGC"
#define MMIAUDIO_6234_112_2_18_2_3_39_111 "[mmiaudio] MMIAUDIO_EnableEQ is_enable = %d, s_audio_cur_eq_mode = %d"
#define MMIAUDIO_6266_112_2_18_2_3_39_112 "[mmiaudio] MMIAUDIO_EnableEQ is_enable = %d, s_audio_cur_eq_mode = %d"
#define MMIAUDIO_6292_112_2_18_2_3_39_113 "[mmiaudio] MMIAUDIO_EnableEQAndAGC is_enable = %d"
#define MMIAUDIO_6353_112_2_18_2_3_39_114 "[MMIAUDIO]:MMIAUDIO_PlayFM:s_fm_handle=0x%lx, aud_result = %d, result = %d"
#define MMIAUDIO_6366_112_2_18_2_3_39_115 "[MMIAUDIO]:MMIAUDIO_StopFM:s_fm_handle=0x%lx"
#define MMIAUDIO_6370_112_2_18_2_3_39_116 "[MMIAUDIO]:MMIAUDIO_StopFM:s_fm_handle=0x%lx"
#define MMIAUDIO_CTRL_391_112_2_18_2_3_40_117 "[MMIAUDIO]:IsBgPlayInStack,bg_type = 0x%x, result = %d"
#define MMIAUDIO_CTRL_482_112_2_18_2_3_40_118 "[MMIAUDIO]:PauseBgPlay: bg_type = %d, module = 0x%x"
#define MMIAUDIO_CTRL_538_112_2_18_2_3_40_119 "[MMIAUDIO]:ResumeBgPlay,bg_type = 0x%x,module = 0x%x"
#define MMIAUDIO_CTRL_619_112_2_18_2_3_41_120 "[MMIAUDIO]:MMIAUDIO_StopBgPlay,module = 0x%x"
#define MMIAUDIO_CTRL_709_112_2_18_2_3_41_121 "[MMIAUDIO]:MMIAUDIO_IsBgPlayIdleDisplay,bg_type = 0x%x, local_bg_type = 0x%x"
#define MMIAUDIO_CTRL_722_112_2_18_2_3_41_122 "[MMIAUDIO]:MMIAUDIO_PauseOtherBgPlay,bg_type = 0x%x, active_type = 0x%x"
#define MMIAUDIO_CTRL_773_112_2_18_2_3_41_123 "[MMIAUDIO]:MMIAUDIO_ResumeOtherBgPlay,bg_type = 0x%x, active_type = 0x%x"
#define MMIAUDIO_CTRL_807_112_2_18_2_3_41_124 "[MMIAUDIO]:MMIAUDIO_StopOtherBgPlay,bg_type = 0x%x"
#define MMIAUDIO_CTRL_870_112_2_18_2_3_41_125 "MMIAUDIO_IsBgPlayPausedByModule: bg = %d has suspend by module id = 0x%x"
#define MMIAUDIO_CTRL_882_112_2_18_2_3_41_126 "MMIAUDIO_IsBgPlayPausedByModule: bg = %d has suspend by module id = 0x%x"
#define MMIAUDIO_CTRL_912_112_2_18_2_3_41_127 "MMIAUDIO_IsBgPlayPaused: bg = %d has suspend by local_bg_type = %d"
#define MMIAUDIO_CTRL_924_112_2_18_2_3_41_128 "MMIAUDIO_IsBgPlayPaused: bg = %d has suspend by module id = 0x%x"
#define MMIAUDIO_CTRL_936_112_2_18_2_3_41_129 "MMIAUDIO_IsBgPlayPaused: bg = %d has suspend by module id = 0x%x"
#define MMIAUDIO_CTRL_1251_112_2_18_2_3_42_130 "[MMIAUDIO] MMIAUDIO_EnableKeyRing: ring_type = 0x%x, module = 0x%x, is_enable = 0x%x, key = 0x%x 0x%x, tp = 0x%x 0x%x!"
#define MMIAUDIO_CTRL_1271_112_2_18_2_3_42_131 "[MMIAUDIO] MMIAUDIO_EnableKeyRingEx: ring_type = 0x%x, module = 0x%x, is_enable = 0x%x, key = 0x%x 0x%x, tp = 0x%x 0x%x!"
#define MMIAUDIO_CTRL_1286_112_2_18_2_3_42_132 "[MMIAUDIO] MMIAUDIO_LastKeyTpkeyNotify: win_id = 0x%x, msg_id = 0x%x!"
#define MMIAUDIO_CTRL_1319_112_2_18_2_3_42_133 "[MMIAUDIO] MMIAUDIO_SendLastKeyTpkeyNotify: POST MSG win_id = 0x%x, msg_id = 0x%x!"
#define MMIAUDIO_CTRL_1443_112_2_18_2_3_42_134 "[MMIAUDIO]:MMIAUDIO_HandleRingCallBack"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_AUDIO_TRC)
TRACE_MSG(MMIAUDIO_127_112_2_18_2_3_26_0,"[MMISRV]: GetHandleByType() exit, type=%d, handle=%d")
TRACE_MSG(MMIAUDIO_186_112_2_18_2_3_26_1,"[MMISRV]: GetTypeInfo() exit, type=%d, type_ptr=%d")
TRACE_MSG(MMIAUDIO_194_112_2_18_2_3_26_2,"[MMISRV]: SetCurType() exit, cur_type=%d")
TRACE_MSG(MMIAUDIO_238_112_2_18_2_3_26_3,"[MMISRV]: ConvertTypeToFmt() exit, ring_type=%d, fmt=%d")
TRACE_MSG(MMIAUDIO_273_112_2_18_2_3_27_4,"[MMISRV]: ConvertEqMode() exit, eq_mode=%d, eq=%d")
TRACE_MSG(MMIAUDIO_306_112_2_18_2_3_27_5,"[MMISRV]: ConvertDevMode() exit, mode=%d, route=%d")
TRACE_MSG(MMIAUDIO_340_112_2_18_2_3_27_6,"[MMISRV]: ConvertRoute() exit, route=%d, mode=%d")
TRACE_MSG(MMIAUDIO_370_112_2_18_2_3_27_7,"[MMISRV]: ConvertRate() exit, rate=%d, srv_rate=%d")
TRACE_MSG(MMIAUDIO_470_112_2_18_2_3_27_8,"[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x")
TRACE_MSG(MMIAUDIO_476_112_2_18_2_3_27_9,"[MMISRV]: HandleNotify(), result=%d, report_ptr=0x%x")
TRACE_MSG(MMIAUDIO_480_112_2_18_2_3_27_10,"[MMISRV]: HandleNotify(), report_ptr->report=%d")
TRACE_MSG(MMIAUDIO_568_112_2_18_2_3_27_11,"[MMISRV]: MMIAUDIO_SetVolume() entry, mode=%d, speaker_vol=%d")
TRACE_MSG(MMIAUDIO_612_112_2_18_2_3_27_12,"[MMISRV]: MMIAUDIO_IsInVoiceLoop() entry, enable=%d")
TRACE_MSG(MMIAUDIO_653_112_2_18_2_3_27_13,"[MMISRV]: MMIAUDIO_GetIsPlaying() entry, Is not playing!!!")
TRACE_MSG(MMIAUDIO_659_112_2_18_2_3_27_14,"[MMISRV]: MMIAUDIO_GetIsPlaying() entry, Is playing!!!")
TRACE_MSG(MMIAUDIO_731_112_2_18_2_3_28_15,"[MMISRV]: MMIAUDIO_IsSeekSupport() exit, audio type = %d, result = %d")
TRACE_MSG(MMIAUDIO_764_112_2_18_2_3_28_16,"[MMISRV]: MMIAUDIO_AudioPlay() entry, handle=%d, type_ptr=%d")
TRACE_MSG(MMIAUDIO_786_112_2_18_2_3_28_17,"[MMISRV]: MMIAUDIO_AudioPlay() exit, res=%d")
TRACE_MSG(MMIAUDIO_806_112_2_18_2_3_28_18,"[MMISRV]: MMIAUDIO_AudioPlay() entry, handle=%d, type_ptr=%d")
TRACE_MSG(MMIAUDIO_828_112_2_18_2_3_28_19,"[MMISRV]: MMIAUDIO_AudioPlay() exit, res=%d")
TRACE_MSG(MMIAUDIO_850_112_2_18_2_3_28_20,"[MMISRV]: MMIAUDIO_AudioPause() exit, handle=%d, res=%d")
TRACE_MSG(MMIAUDIO_881_112_2_18_2_3_28_21,"[MMISRV]: MMIAUDIO_AudioResume() exit, handle=%d, res=%d")
TRACE_MSG(MMIAUDIO_911_112_2_18_2_3_28_22,"[MMISRV]: MMIAUDIO_AudioStop() exit, handle=%d, res=%d")
TRACE_MSG(MMIAUDIO_931_112_2_18_2_3_28_23,"[MMISRV]: MMIAUDIO_AudioSeek() exit, handle=%d, offset, res=%d")
TRACE_MSG(MMIAUDIO_961_112_2_18_2_3_28_24,"[MMISRV]: MMIAUDIO_AudioSeekByTime() exit, handle=0x%x, time_offset=%d, byte_offset=%d, res=%d")
TRACE_MSG(MMIAUDIO_982_112_2_18_2_3_28_25,"[MMISRV]: MMIAUDIO_PlayTone() entry, handle=%d, tone_type=%d, tone_id=%d, duration=%d,freq=%d")
TRACE_MSG(MMIAUDIO_1017_112_2_18_2_3_28_26,"[MMISRV]: MMIAUDIO_PlayTone() exit, handle=0x%x, res=%d")
TRACE_MSG(MMIAUDIO_1052_112_2_18_2_3_28_27,"[MMISRV]: MMIAUDIO_StopTone() exit, handle=%d, res=%d")
TRACE_MSG(MMIAUDIO_1100_112_2_18_2_3_28_28,"[MMIAUDIO]:MMIAUDIO_StartIQData:s_record_handle=%lx")
TRACE_MSG(MMIAUDIO_1181_112_2_18_2_3_28_29,"[MMIAUDIO]:MMIAUDIO_StartRecord exit: handle=0x%x, res = %d")
TRACE_MSG(MMIAUDIO_1201_112_2_18_2_3_28_30,"[MMIAUDIO]:MMIAUDIO_EndRecord exit: handle=0x%x, res = %d")
TRACE_MSG(MMIAUDIO_1234_112_2_18_2_3_29_31,"[MMIAUDIO]:MMIAUDIO_StopRecord exit: handle=0x%x, res = %d")
TRACE_MSG(MMIAUDIO_1257_112_2_18_2_3_29_32,"[MMIAUDIO]:MMIAUDIO_PauseRecord exit: handle=0x%x, res = %d")
TRACE_MSG(MMIAUDIO_1280_112_2_18_2_3_29_33,"[MMIAUDIO]:MMIAUDIO_ResumeRecord exit: handle=0x%x, res = %d")
TRACE_MSG(MMIAUDIO_1330_112_2_18_2_3_29_34,"[MMIAUDIO]:MMIAUDIO_PlayFM exit: handle=0x%x, res = %d")
TRACE_MSG(MMIAUDIO_1359_112_2_18_2_3_29_35,"[MMIAUDIO]:MMIAUDIO_StopFM exit: handle=0x%x, res = %d")
TRACE_MSG(MMIAUDIO_1417_112_2_18_2_3_29_36,"[MMISRV]: MMIAUDIO_SetEQMode() exit, eq_mode=%d, eq=%d, res=%d")
TRACE_MSG(MMIAUDIO_1450_112_2_18_2_3_29_37,"[MMISRV]: MMIAUDIO_SetVolumeDeviceModeExt() entry")
TRACE_MSG(MMIAUDIO_1485_112_2_18_2_3_29_38,"[MMISRV]: MMIAUDIO_SerDeviceModeInCall() exit, mode=%d,handle=0x%x,res=%d")
TRACE_MSG(MMIAUDIO_1530_112_2_18_2_3_29_39,"[MMISRV]: MMIAUDIO_SwitchDeviceExt() exit, handle=%d, device=%d,res=%d")
TRACE_MSG(MMIAUDIO_1542_112_2_18_2_3_29_40,"[MMISRV]: MMIAUDIO_GetAudioHandle() exit, handle=0x%x")
TRACE_MSG(MMIAUDIO_1580_112_2_18_2_3_29_41,"[MMISRV]:MMIAUDIO_GetIsPermitPlay:cur_type=%d, audio_htype=%d")
TRACE_MSG(MMIAUDIO_1602_112_2_18_2_3_29_42,"[MMISRV]:MMIAUDIO_GetAudioHtype:cur_type=%d")
TRACE_MSG(MMIAUDIO_2447_112_2_18_2_3_31_43,"[MMIAUDIO]:AudioEndCallback:haudiohandle=0x%X, s_cur_audio_handle_type=%d!")
TRACE_MSG(MMIAUDIO_2603_112_2_18_2_3_31_44,"[MMIAUDIO]:AudioStopCallback:haudiohandle=0x%X, s_cur_audio_handle_type=%d!")
TRACE_MSG(MMIAUDIO_2694_112_2_18_2_3_31_45,"[MMIAUDIO]:AudioNotifyCallback:enter:hAudio = 0x%X, notify_info = 0x%X, affix_info = %ld")
TRACE_MSG(MMIAUDIO_2762_112_2_18_2_3_32_46,"[MMIAUDIO]:MMIAPIAUDIO_HandleCallBackMsg:handle = 0x%X,msg_id = 0x%X, aud_result = %d, s_cur_audio_handle_type = %d, IsEnd=%d,app_handle = 0x%X, player_handle = 0x%X, music_handle = 0x%X, kuro_handle = 0x%X, normal_handle = 0x%X")
TRACE_MSG(MMIAUDIO_2764_112_2_18_2_3_32_47,"[MMIAUDIO]:MMIAPIAUDIO_HandleCallBackMsg:app_callback = 0x%X,player_callback = 0x%X, music_callback = 0x%X, kuro_callback = 0x%X,normal_callback = 0x%X")
TRACE_MSG(MMIAUDIO_2789_112_2_18_2_3_32_48,"[MMIAUDIO]:MMIAPIAUDIO_HandleCallBackMsg,  send play error")
TRACE_MSG(MMIAUDIO_2846_112_2_18_2_3_32_49,"[MMIAUDIO]:RemovePlayCallback: hAudiohandle =0x%x, i = %d")
TRACE_MSG(MMIAUDIO_2873_112_2_18_2_3_32_50,"[MMIAUDIO]:IsPlayCallbackValid: hAudiohandle =0x%x, i = %d, result")
TRACE_MSG(MMIAUDIO_2909_112_2_18_2_3_32_51,"[MMIAUDIO]:GetAudioCodec:audio_codec=0x%x")
TRACE_MSG(MMIAUDIO_2969_112_2_18_2_3_32_52,"[MMIAUDIO]:GetAudioCodec:ring_type=%d")
TRACE_MSG(MMIAUDIO_3083_112_2_18_2_3_32_53,"[MMIAUDIO]:GetAudioDevice: ring_type = %d, hDevice = 0x%x")
TRACE_MSG(MMIAUDIO_3168_112_2_18_2_3_32_54,"[MMIAUDIO]:CreateAudioBufferHandle:ring_type error!!!! orig = %x, new = %x, aud_result = %d")
TRACE_MSG(MMIAUDIO_3173_112_2_18_2_3_32_55,"[MMIAUDIO]:CreateAudioBufferHandle:ring_type=%d, data_ptr = 0x%x, data_len = %d, audiohandle = 0x%x")
TRACE_MSG(MMIAUDIO_3220_112_2_18_2_3_33_56,"[MMIAUDIO]:CreateAudioBufferHandle:ring_type error!!!! orig = %x, new = %x, aud_result = %d")
TRACE_MSG(MMIAUDIO_3224_112_2_18_2_3_33_57,"[MMIAUDIO]:CreateAudioFileHandle:ring_type=%d, audiohandle = 0x%x")
TRACE_MSG(MMIAUDIO_3280_112_2_18_2_3_33_58,"[MMIAUDIO]:CreateAudioBufferHandle:ring_type error!!!! orig = %x, new = %x, aud_result = %d")
TRACE_MSG(MMIAUDIO_3285_112_2_18_2_3_33_59,"[MMIAUDIO]:CreateAudioStreamingHandle:ring_type=%d, data_ptr = 0x%x, data_len = %d, audiohandle = 0x%x")
TRACE_MSG(MMIAUDIO_3762_112_2_18_2_3_34_60,"[MMIAUDIO]:MMIAUDIO_AudioPlay,  audio_handle_type = 0x%x, uiOffset = %d, play_times = %d, func = 0x%x, aud_result = %d")
TRACE_MSG(MMIAUDIO_3850_112_2_18_2_3_34_61,"[MMIAUDIO]:MMIAUDIO_AudioPlayAsy,  audio_handle_type = 0x%x, uiOffset = %d, play_times = %d, func = 0x%x, aud_result = %d")
TRACE_MSG(MMIAUDIO_3883_112_2_18_2_3_34_62,"[MMIAUDIO]:HandleAudioPriority, hAudioHandle = 0x%x, aud_result = %d")
TRACE_MSG(MMIAUDIO_3894_112_2_18_2_3_34_63,"[MMIAUDIO]:MMIAUDIO_AudioStop:audio_type=%d, cur_type = %d, app_handle = 0x%X, player_handle = 0x%X, music_handle = 0x%X, kuro_handle = 0x%X,normal_handle = 0x%X")
TRACE_MSG(MMIAUDIO_4032_112_2_18_2_3_34_64,"[MMIAUDIO]:MMIAUDIO_CloseAudioHandle, Alert!, s_cur_audio_handle_type = %d, audio_handle_type = %d")
TRACE_MSG(MMIAUDIO_4104_112_2_18_2_3_34_65,"[MMIAUDIO]: MMIAUDIO_CloseAudioHandle:audio_handle_type=%d, aud_result = %d")
TRACE_MSG(MMIAUDIO_4196_112_2_18_2_3_34_66,"[MMIAUDIO]:MMIAUDIO_AudioPause:audio_handle_type=%d,aud_result = %d")
TRACE_MSG(MMIAUDIO_4279_112_2_18_2_3_35_67,"[MMIAUDIO]:MMIAUDIO_AudioResume:audio_handle_type=%d,aud_result = %d")
TRACE_MSG(MMIAUDIO_4352_112_2_18_2_3_35_68,"[MMIAUDIO]:MMIAUDIO_AudioSeek:audio_handle_type = %d, uiOffset=%ld,audio_result = %d")
TRACE_MSG(MMIAUDIO_4404_112_2_18_2_3_35_69,"[MMIAUDIO]: MMIAUDIO_AudioSeekByTime, audio_handle_type = %d, time_offset = %d, byte_offset = %d, audio_result = %d")
TRACE_MSG(MMIAUDIO_4464_112_2_18_2_3_35_70,"[MMIAUDIO]: MMIAUDIO_IsSeekSupport, audio type = %d, result = %d")
TRACE_MSG(MMIAUDIO_4486_112_2_18_2_3_35_71,"[MMIAUDIO]:AudioPlayStop:hAudioHandle = %x, stop_result=%ld,close_result = %d")
TRACE_MSG(MMIAUDIO_4495_112_2_18_2_3_35_72,"[MMIAUDIO]:MMIAUDIO_AudioSetSpeed:audio_handle_type=%d, uiSpeedX=%ld")
TRACE_MSG(MMIAUDIO_4594_112_2_18_2_3_35_73,"[MMIAUDIO]:MMIAUDIO_AudioGetFileInfo:audio_handle_type=%d,aud_result = %d")
TRACE_MSG(MMIAUDIO_4669_112_2_18_2_3_35_74,"[MMIAUDIO]:MMIAUDIO_AudioGetPlayingFileInfo:audio_handle_type=%x, cur_time = %d, cur_offset = %d, t_time = %d, t_len = %d, aud_result = %d")
TRACE_MSG(MMIAUDIO_4728_112_2_18_2_3_36_75,"[MMIAUDIO]:MMIAUDIO_OpenSpectrum:audio_handle_type=%x, aud_result = %d")
TRACE_MSG(MMIAUDIO_4851_112_2_18_2_3_36_76,"[MMIAUDIO]:MMIAUDIO_CloseSpectrum:audio_handle_type=%x, aud_result = %d")
TRACE_MSG(MMIAUDIO_5007_112_2_18_2_3_36_77,"[MMIAUDIO]:MMIAUDIO_StartRecord:s_record_handle=0x%lx, file_handle = 0x%x, record_type = %d, aud_result = %d, result = %d")
TRACE_MSG(MMIAUDIO_5039_112_2_18_2_3_36_78,"[MMIAUDIO]:MMIAUDIO_StopRecord:s_record_handle = %x, aud_result=%ld, result = %d")
TRACE_MSG(MMIAUDIO_5053_112_2_18_2_3_36_79,"[MMIAUDIO]:MMIAUDIO_EndRecord:s_record_handle = %x")
TRACE_MSG(MMIAUDIO_5069_112_2_18_2_3_36_80,"[MMIAUDIO]:MMIAUDIO_EndRecord:aud_result=%ld, result = %d")
TRACE_MSG(MMIAUDIO_5091_112_2_18_2_3_36_81,"[MMIAUDIO]:MMIAUDIO_PauseRecord:s_record_handle = %x, aud_result=%ld, result = %d")
TRACE_MSG(MMIAUDIO_5113_112_2_18_2_3_36_82,"[MMIAUDIO]:MMIAUDIO_ResumeRecord:s_record_handle = %x, aud_result=%ld, result = %d")
TRACE_MSG(MMIAUDIO_5140_112_2_18_2_3_36_83,"[MMIAUDIO]:MMIAUDIO_GetIsPermitPlay:s_cur_audio_handle_type=%d, audio_htype=%d")
TRACE_MSG(MMIAUDIO_5172_112_2_18_2_3_36_84,"[MMIAUDIO]:MMIAUDIO_GetAudioHandle:audio_handle_type=%d")
TRACE_MSG(MMIAUDIO_5266_112_2_18_2_3_37_85,"[MMIAUDIO]:MMIAUDIO_PlayTone:audio_type=%d")
TRACE_MSG(MMIAUDIO_5346_112_2_18_2_3_37_86,"[MMIAUDIO]:MMIAUDIO_StopTone:audio_type=%d")
TRACE_MSG(MMIAUDIO_5397_112_2_18_2_3_37_87,"[MMIAUDIO]: MMIAUDIO_SetEQMode(), input error, MMI eq_mode = %d, ")
TRACE_MSG(MMIAUDIO_5430_112_2_18_2_3_37_88,"[MMIAUDIO]: MMIAUDIO_SetEQMode(), orig_EQ = %d, new_EQ = %d, aud_result = %d")
TRACE_MSG(MMIAUDIO_5500_112_2_18_2_3_37_89,"[MMIAUDIO] MMIAUDIO_SetPlayRate:rate = %d")
TRACE_MSG(MMIAUDIO_5515_112_2_18_2_3_37_90,"[MMIAUDIO]: MMIAUDIO_SetPlayRate(), orig_RATE = %d, new_RATE = %d, aud_result = %d")
TRACE_MSG(MMIAUDIO_5590_112_2_18_2_3_37_91,"[MMIAUDIO]:MMIAUDIO_SetVolume(), orig_mod = %d,orig_vol = %d,new_mod = %d,new_vol = %d, aud_result_dev = %d,aud_result_vol = %d")
TRACE_MSG(MMIAUDIO_5624_112_2_18_2_3_37_92,"[MMIAUDIO]: MMIAUDIO_SetCurDevMode: orig_dev=%d, new_dev = %d, aud_result = %d")
TRACE_MSG(MMIAUDIO_5668_112_2_18_2_3_37_93,"[MMIAUDIO]: MMIAUDIO_SetMute: orig_is_mute=%d, new_is_mute = %d, aud_result = %d")
TRACE_MSG(MMIAUDIO_5701_112_2_18_2_3_38_94,"[MMIAUDIO]: MMIAUDIO_SetUpLinkMute: is_mute=%d,aud_result = %d")
TRACE_MSG(MMIAUDIO_5749_112_2_18_2_3_38_95,"[MMIAUDIO]:MMIAUDIO_SwitchDeviceExt,  audio_handle_type = 0x%x, hAudioHandle = 0x%x, format_type = %d, device = %d, aud_result = %d")
TRACE_MSG(MMIAUDIO_5789_112_2_18_2_3_38_96,"[MMIAUDIO]:MMIAUDIO_StartIQData BUFFER ALLOC FAILED!!!!!!!!!!")
TRACE_MSG(MMIAUDIO_5804_112_2_18_2_3_38_97,"[MMIAUDIO]:MMIAUDIO_StartIQData:s_record_handle=%lx")
TRACE_MSG(MMIAUDIO_5849_112_2_18_2_3_38_98,"[MMIAUDIO]:MMIAUDIO_GetRecordEclipseTime, time = %d!")
TRACE_MSG(MMIAUDIO_5874_112_2_18_2_3_38_99,"[MMIAUDIO]:MMIAUDIO_GetRecordVoiceDB, db = %d!")
TRACE_MSG(MMIAUDIO_5935_112_2_18_2_3_38_100,"[MMIAUDIO]: MMIAUDIO_EnableVoiceCodec: is_enable=%d, aud_result = %d")
TRACE_MSG(MMIAUDIO_5960_112_2_18_2_3_38_101,"[MMIAUDIO]: MMIAUDIO_SerDeviceModeInCall: orig_dev=%d, new_dev = %d, aud_result = %d")
TRACE_MSG(MMIAUDIO_5990_112_2_18_2_3_38_102,"[MMIAUDIO]: MMIAUDIO_EnableVoiceCodec: is_loop_enable=%d, is_force_dev_mode = %d,force_dev_mode = %d,,volume =%d")
TRACE_MSG(MMIAUDIO_6033_112_2_18_2_3_38_103,"[MMIAUDIO]: MMIAUDIO_IsInVoiceLoop: s_is_voice_loop_enable = %d")
TRACE_MSG(MMIAUDIO_6045_112_2_18_2_3_38_104,"[MMIAUDIO]: MMIAUDIO_IsVoiceLoopForceMode: s_is_voice_loop_force_mode = %d")
TRACE_MSG(MMIAUDIO_6059_112_2_18_2_3_38_105,"MMIAUDIO_SM_GetAudioObject true")
TRACE_MSG(MMIAUDIO_6064_112_2_18_2_3_38_106,"MMIAUDIO_SM_GetAudioObject False")
TRACE_MSG(MMIAUDIO_6107_112_2_18_2_3_38_107,"[MMIAUDIO]:MMIAUDIO_SetVolumeDeviceModeExt, orig_mod = %d,orig_vol = %d,new_mod = %d,new_vol = %d, aud_result_dev = %d,aud_result_vol = %d")
TRACE_MSG(MMIAUDIO_6159_112_2_18_2_3_38_108,"[MMIAUDIO]:MMIAUDIO_SetMidiLowPolyNum, audio_htype = %d, midi_handle = 0x%x, poly_num = %d,aud_result = %d")
TRACE_MSG(MMIAUDIO_6190_112_2_18_2_3_39_109,"[mmiaudio] MMIAUDIO_StartAGC dev_mode = %d, eq_mode = %d")
TRACE_MSG(MMIAUDIO_6214_112_2_18_2_3_39_110,"[mmiaudio] MMIAUDIO_StopAGC")
TRACE_MSG(MMIAUDIO_6234_112_2_18_2_3_39_111,"[mmiaudio] MMIAUDIO_EnableEQ is_enable = %d, s_audio_cur_eq_mode = %d")
TRACE_MSG(MMIAUDIO_6266_112_2_18_2_3_39_112,"[mmiaudio] MMIAUDIO_EnableEQ is_enable = %d, s_audio_cur_eq_mode = %d")
TRACE_MSG(MMIAUDIO_6292_112_2_18_2_3_39_113,"[mmiaudio] MMIAUDIO_EnableEQAndAGC is_enable = %d")
TRACE_MSG(MMIAUDIO_6353_112_2_18_2_3_39_114,"[MMIAUDIO]:MMIAUDIO_PlayFM:s_fm_handle=0x%lx, aud_result = %d, result = %d")
TRACE_MSG(MMIAUDIO_6366_112_2_18_2_3_39_115,"[MMIAUDIO]:MMIAUDIO_StopFM:s_fm_handle=0x%lx")
TRACE_MSG(MMIAUDIO_6370_112_2_18_2_3_39_116,"[MMIAUDIO]:MMIAUDIO_StopFM:s_fm_handle=0x%lx")
TRACE_MSG(MMIAUDIO_CTRL_391_112_2_18_2_3_40_117,"[MMIAUDIO]:IsBgPlayInStack,bg_type = 0x%x, result = %d")
TRACE_MSG(MMIAUDIO_CTRL_482_112_2_18_2_3_40_118,"[MMIAUDIO]:PauseBgPlay: bg_type = %d, module = 0x%x")
TRACE_MSG(MMIAUDIO_CTRL_538_112_2_18_2_3_40_119,"[MMIAUDIO]:ResumeBgPlay,bg_type = 0x%x,module = 0x%x")
TRACE_MSG(MMIAUDIO_CTRL_619_112_2_18_2_3_41_120,"[MMIAUDIO]:MMIAUDIO_StopBgPlay,module = 0x%x")
TRACE_MSG(MMIAUDIO_CTRL_709_112_2_18_2_3_41_121,"[MMIAUDIO]:MMIAUDIO_IsBgPlayIdleDisplay,bg_type = 0x%x, local_bg_type = 0x%x")
TRACE_MSG(MMIAUDIO_CTRL_722_112_2_18_2_3_41_122,"[MMIAUDIO]:MMIAUDIO_PauseOtherBgPlay,bg_type = 0x%x, active_type = 0x%x")
TRACE_MSG(MMIAUDIO_CTRL_773_112_2_18_2_3_41_123,"[MMIAUDIO]:MMIAUDIO_ResumeOtherBgPlay,bg_type = 0x%x, active_type = 0x%x")
TRACE_MSG(MMIAUDIO_CTRL_807_112_2_18_2_3_41_124,"[MMIAUDIO]:MMIAUDIO_StopOtherBgPlay,bg_type = 0x%x")
TRACE_MSG(MMIAUDIO_CTRL_870_112_2_18_2_3_41_125,"MMIAUDIO_IsBgPlayPausedByModule: bg = %d has suspend by module id = 0x%x")
TRACE_MSG(MMIAUDIO_CTRL_882_112_2_18_2_3_41_126,"MMIAUDIO_IsBgPlayPausedByModule: bg = %d has suspend by module id = 0x%x")
TRACE_MSG(MMIAUDIO_CTRL_912_112_2_18_2_3_41_127,"MMIAUDIO_IsBgPlayPaused: bg = %d has suspend by local_bg_type = %d")
TRACE_MSG(MMIAUDIO_CTRL_924_112_2_18_2_3_41_128,"MMIAUDIO_IsBgPlayPaused: bg = %d has suspend by module id = 0x%x")
TRACE_MSG(MMIAUDIO_CTRL_936_112_2_18_2_3_41_129,"MMIAUDIO_IsBgPlayPaused: bg = %d has suspend by module id = 0x%x")
TRACE_MSG(MMIAUDIO_CTRL_1251_112_2_18_2_3_42_130,"[MMIAUDIO] MMIAUDIO_EnableKeyRing: ring_type = 0x%x, module = 0x%x, is_enable = 0x%x, key = 0x%x 0x%x, tp = 0x%x 0x%x!")
TRACE_MSG(MMIAUDIO_CTRL_1271_112_2_18_2_3_42_131,"[MMIAUDIO] MMIAUDIO_EnableKeyRingEx: ring_type = 0x%x, module = 0x%x, is_enable = 0x%x, key = 0x%x 0x%x, tp = 0x%x 0x%x!")
TRACE_MSG(MMIAUDIO_CTRL_1286_112_2_18_2_3_42_132,"[MMIAUDIO] MMIAUDIO_LastKeyTpkeyNotify: win_id = 0x%x, msg_id = 0x%x!")
TRACE_MSG(MMIAUDIO_CTRL_1319_112_2_18_2_3_42_133,"[MMIAUDIO] MMIAUDIO_SendLastKeyTpkeyNotify: POST MSG win_id = 0x%x, msg_id = 0x%x!")
TRACE_MSG(MMIAUDIO_CTRL_1443_112_2_18_2_3_42_134,"[MMIAUDIO]:MMIAUDIO_HandleRingCallBack")
END_TRACE_MAP(MMI_APP_AUDIO_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_AUDIO_TRC_H_

