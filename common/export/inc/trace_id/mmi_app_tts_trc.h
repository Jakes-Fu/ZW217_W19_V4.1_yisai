/******************************************************************************
 ** File Name:      mmi_app_tts_trc.h                                         *
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
//trace_id:230
#ifndef _MMI_APP_TTS_TRC_H_
#define _MMI_APP_TTS_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define TTS_AUDIO_57_112_2_18_3_3_37_0 "enter funciton MMITTS_CreatePCMStreamHandle()"
#define TTS_AUDIO_129_112_2_18_3_3_37_1 "enter funciton MMITTS_STOP_CALLBACK() g_tts_is_running=%d"
#define TTS_AUDIO_164_112_2_18_3_3_37_2 "play_stream_id = %d, cur_stream_id = %d"
#define TTS_AUDIO_168_112_2_18_3_3_37_3 "enter funciton MMIAPITTS_PlayPcmStream()"
#define TTS_AUDIO_172_112_2_18_3_3_37_4 "MMIAPITTS_PlayPcmStream() Handle error !!!!"
#define TTS_AUDIO_223_112_2_18_3_3_37_5 "enter funciton MMITTS_StopAndClosePcmStreamHandle()"
#define EJTTSPLAYER_API_140_112_2_18_3_3_38_6 "TTSAPI_PlayText enter txt_len_in_byte = %d"
#define EJTTSPLAYER_API_208_112_2_18_3_3_38_7 "TTSAPI_PlayText %x, %x"
#define EJTTSPLAYER_API_209_112_2_18_3_3_38_8 "TTSAPI_PlayText ring_type = %d, tts_vol = %d"
#define EJTTSPLAYER_API_217_112_2_18_3_3_38_9 "TTSAPI_PlayText enter stop jttsplayer"
#define EJTTSPLAYER_API_246_112_2_18_3_3_38_10 " GetTTSResouceFileFullName result = %d"
#define EJTTSPLAYER_API_275_112_2_18_3_3_38_11 " GetTTSENResouceFileFullName result = %d"
#define EJTTSPLAYER_API_294_112_2_18_3_3_38_12 "MMITTS_HandlePlayTextTimer"
#define EJTTSPLAYER_API_299_112_2_18_3_3_38_13 "MMITTS_HandlePlayTextTimer | ejTTSPlayer_GetStatus = %d"
#define EJTTSPLAYER_API_329_112_2_18_3_3_38_14 "HandlePlayTextTimer stop jtts palyer"
#define EJTTSPLAYER_API_341_112_2_18_3_3_38_15 "MMITTS_StopPlayText"
#define EJTTSPLAYER_API_383_112_2_18_3_3_38_16 "TTSAPI_GetAudioIsOpen | audio_handle = %d"
#define EJTTSPLAYER_API_408_112_2_18_3_3_38_17 "TTSAPI_SynthTextByTask enter"
#define EJTTSPLAYER_API_440_112_2_18_3_3_38_18 "TTSAPI_SynthTextByTask stop jtts"
#define EJTTSPLAYER_API_445_112_2_18_3_3_38_19 "TTSAPI_SynthTextByTask | ejTTSPlayer_GetStatus = %d"
#define EJTTSPLAYER_API_479_112_2_18_3_3_38_20 "TTSAPI_NotifyTTSOver enter, s_ttsinfo.win_handle = %d"
#define EJTTSPLAYER_API_492_112_2_18_3_3_38_21 "TTSAPI_StopPlayTextByType ring_type=%d,  s_ttsinfo.ring_type = %d"
#define EJTTSPLAYER_API_636_112_2_18_3_3_38_22 "[MMITTS]:TTSAPI_PlayTextForControl ERROR!!!!!!!!!!!!"
#define EJTTSPLAYER_API_641_112_2_18_3_3_39_23 "TTSAPI_PlayTextForControl enter"
#define EJTTSPLAYER_API_753_112_2_18_3_3_39_24 "HandleAudioEnv is_begin = %d audio_handle = %d"
#define EJTTSPLAYER_API_760_112_2_18_3_3_39_25 "HandleAudioEnv() Handle error !!!!"
#define EJTTSPLAYER_API_784_112_2_18_3_3_39_26 "JTTS_PlayCallback() uMessage = %d s_is_manual_stop = %d"
#define EJTTSPLAYER_API_797_112_2_18_3_3_39_27 "JTTS_PlayCallback() post MSG_TTS_OVER"
#define EJTTSPLAYER_API_823_112_2_18_3_3_39_28 "TTS_GetManualStop s_is_manual_stop = %d"
#define EJTTSPLAYER_AUDIO_321_112_2_18_3_3_40_29 "TTSPlayer|Entery JTTS_AudioCallBackFunc tick = %d g_bplayed = %d, g_SyncData.Status = %d, hAudio = %d, g_playsize = %d"
#define EJTTSPLAYER_AUDIO_326_112_2_18_3_3_40_30 "TTSPlayer|JTTS_AudioCallBackFunc audio handle is not g_audio_handle"
#define EJTTSPLAYER_AUDIO_335_112_2_18_3_3_40_31 "TTSPlayer|JTTS_AudioCallBackFunc finish play"
#define EJTTSPLAYER_AUDIO_339_112_2_18_3_3_40_32 "TTSPlayer|AudioCallBack wait for g_playsize > 0"
#define EJTTSPLAYER_AUDIO_344_112_2_18_3_3_40_33 "TTSPlayer|JTTS_AudioCallBackFunc g_playsize = %d"
#define EJTTSPLAYER_AUDIO_351_112_2_18_3_3_40_34 "TTSPlayer|JTTS_Audio_CallBackFunc|Out %x %x"
#define EJTTSPLAYER_AUDIO_374_112_2_18_3_3_40_35 "TTSPlayer|OuputVoiceToFile|In iSize = %d"
#define EJTTSPLAYER_AUDIO_398_112_2_18_3_3_40_36 "TTSPlayer|OutputVoiceToPlayProc tick count = %d iSize = %d"
#define EJTTSPLAYER_AUDIO_403_112_2_18_3_3_40_37 "TTSPlayer|synthe time used = %d"
#define EJTTSPLAYER_AUDIO_413_112_2_18_3_3_40_38 "TTSPlayer|set g_playpos = 0"
#define EJTTSPLAYER_AUDIO_423_112_2_18_3_3_40_39 "TTSPlayer|wait for write"
#define EJTTSPLAYER_AUDIO_429_112_2_18_3_3_40_40 "TTSPlayer|pause sleep 100"
#define EJTTSPLAYER_AUDIO_441_112_2_18_3_3_40_41 "TTSPlayer|Leave OutputVoiceToPlayProc tickcount = %d"
#define EJTTSPLAYER_AUDIO_457_112_2_18_3_3_40_42 "** ejTTSPlayer_Init in"
#define EJTTSPLAYER_AUDIO_462_112_2_18_3_3_40_43 "** Error! TTS is already init"
#define EJTTSPLAYER_AUDIO_470_112_2_18_3_3_40_44 "Create Mutex failed"
#define EJTTSPLAYER_AUDIO_473_112_2_18_3_3_40_45 "TTSPlayer|Create Mutex success"
#define EJTTSPLAYER_AUDIO_479_112_2_18_3_3_40_46 "TTSPlayer|Create Play Mutex failed"
#define EJTTSPLAYER_AUDIO_482_112_2_18_3_3_40_47 "TTSPlayer|Create Play Mutex success"
#define EJTTSPLAYER_AUDIO_491_112_2_18_3_3_40_48 "Create JTTS_GROUP_EVENT failed"
#define EJTTSPLAYER_AUDIO_511_112_2_18_3_3_40_49 "TTSPlayer|Create Thread failed"
#define EJTTSPLAYER_AUDIO_515_112_2_18_3_3_40_50 "TTSPlayer|Create JTTS_Thread success"
#define EJTTSPLAYER_AUDIO_520_112_2_18_3_3_40_51 "JTTS_Init Failed"
#define EJTTSPLAYER_AUDIO_525_112_2_18_3_3_40_52 "ejTTSPlayer_Init success"
#define EJTTSPLAYER_AUDIO_536_112_2_18_3_3_40_53 "TTSPlayer|Entry ejTTSPlayer_End"
#define EJTTSPLAYER_AUDIO_559_112_2_18_3_3_40_54 "TTSPlayer|End Playing"
#define EJTTSPLAYER_AUDIO_565_112_2_18_3_3_40_55 "TTSPlayer|set exit status"
#define EJTTSPLAYER_AUDIO_569_112_2_18_3_3_40_56 "TTSPlayer|Set child event"
#define EJTTSPLAYER_AUDIO_572_112_2_18_3_3_40_57 "TTSPlayer|end wait for parent event"
#define EJTTSPLAYER_AUDIO_577_112_2_18_3_3_40_58 "TTSPlayer|end wait g_jtts_thread_id = 0"
#define EJTTSPLAYER_AUDIO_608_112_2_18_3_3_40_59 "TTSPlayer|Leave ejTTSPlayer_End"
#define EJTTSPLAYER_AUDIO_625_112_2_18_3_3_40_60 "TTSPlayer|Entry ejTTSPlayer_Play"
#define EJTTSPLAYER_AUDIO_658_112_2_18_3_3_40_61 "** ejTTSPlayer_Play uTextLen = %d"
#define EJTTSPLAYER_AUDIO_664_112_2_18_3_3_40_62 "** ejTTSPlayer_Play commamd status =%d"
#define EJTTSPLAYER_AUDIO_689_112_2_18_3_3_40_63 "TTSPlayer|Leave ejTTSPlayer_Play"
#define EJTTSPLAYER_AUDIO_696_112_2_18_3_3_40_64 "TTSPlayer|wait parent event failed"
#define EJTTSPLAYER_AUDIO_713_112_2_18_3_3_41_65 "Entry ejTTSPlayer_PlayToFile"
#define EJTTSPLAYER_AUDIO_716_112_2_18_3_3_41_66 "TTSPlayer|Invalid thread id"
#define EJTTSPLAYER_AUDIO_722_112_2_18_3_3_41_67 "TTSPlayer|Invalid input text"
#define EJTTSPLAYER_AUDIO_738_112_2_18_3_3_41_68 "TTSPlayer|Leave ejTTSPlayer_PlayToFile"
#define EJTTSPLAYER_AUDIO_749_112_2_18_3_3_41_69 "TTSPlayer|entry ejTTSPlayer_Stop g_jtts_thread_id = %d"
#define EJTTSPLAYER_AUDIO_756_112_2_18_3_3_41_70 "TTSPlayer|Status is %d"
#define EJTTSPLAYER_AUDIO_765_112_2_18_3_3_41_71 "TTSPlayer|Stop SynthText"
#define EJTTSPLAYER_AUDIO_774_112_2_18_3_3_41_72 "TTSPlayer|Sleep 100 wait for play finish"
#define EJTTSPLAYER_AUDIO_788_112_2_18_3_3_41_73 "TTSPlayer|leave ejTTSPlayer_Stop"
#define EJTTSPLAYER_AUDIO_810_112_2_18_3_3_41_74 "TTSPlayer|ejTTSPlayer_Resume status=%d"
#define EJTTSPLAYER_AUDIO_813_112_2_18_3_3_41_75 "TTSPlayer|Set ~Pause status"
#define EJTTSPLAYER_AUDIO_825_112_2_18_3_3_41_76 "TTSPlayer|ejTTSPlayer_Resume status=%d"
#define EJTTSPLAYER_AUDIO_827_112_2_18_3_3_41_77 "TTSPlayer|Leave ejTTSPlayer_Resume"
#define EJTTSPLAYER_AUDIO_839_112_2_18_3_3_41_78 "TTSPlayer|Entry ejTTSPlayer_Pause"
#define EJTTSPLAYER_AUDIO_851_112_2_18_3_3_41_79 "TTSPlayer|begin status = %d"
#define EJTTSPLAYER_AUDIO_854_112_2_18_3_3_41_80 "TTSPlayer|Set pause status"
#define EJTTSPLAYER_AUDIO_871_112_2_18_3_3_41_81 "TTSPlayer|Leave ejTTSPlayer_Pause"
#define EJTTSPLAYER_AUDIO_881_112_2_18_3_3_41_82 "ejTTSPlayer_GetStatus g_jtts_thread_id = %d, g_SyncData.Status = %d"
#define EJTTSPLAYER_AUDIO_973_112_2_18_3_3_41_83 "TTSPlayer|Entry JTTS_Thread"
#define EJTTSPLAYER_AUDIO_977_112_2_18_3_3_41_84 "** JTTTS_Thread g_SyncData.Status = %d"
#define EJTTSPLAYER_AUDIO_984_112_2_18_3_3_41_85 "JTTS: SigCode is NULL"
#define EJTTSPLAYER_AUDIO_987_112_2_18_3_3_41_86 "JTTS: SigCode = %d"
#define EJTTSPLAYER_AUDIO_996_112_2_18_3_3_41_87 "TTSPlayer|wait child event failed"
#define EJTTSPLAYER_AUDIO_999_112_2_18_3_3_41_88 "TTSPlayer|Event Wait Child event success"
#define EJTTSPLAYER_AUDIO_1003_112_2_18_3_3_41_89 "TTSPlayer|status exit"
#define EJTTSPLAYER_AUDIO_1012_112_2_18_3_3_41_90 "TTSPlayer|stop status -> idle status %d"
#define EJTTSPLAYER_AUDIO_1021_112_2_18_3_3_41_91 "TTSPlayer|command status is not init, continue"
#define EJTTSPLAYER_AUDIO_1031_112_2_18_3_3_41_92 "TTSPlayer|Case TTSPlayer|COMMAND PLAY "
#define EJTTSPLAYER_AUDIO_1035_112_2_18_3_3_41_93 "TTSPlayer|ERROR CODE = %d"
#define EJTTSPLAYER_AUDIO_1069_112_2_18_3_3_41_94 "TTSPlayer|Exit JTTS_Thread"
#define EJTTSPLAYER_AUDIO_1092_112_2_18_3_3_41_95 "TTSPlayer|lSize=%d, dwErrCode = %d"
#define EJTTSPLAYER_AUDIO_1102_112_2_18_3_3_41_96 "TTSPlayer|alloc memory failed"
#define EJTTSPLAYER_AUDIO_1106_112_2_18_3_3_41_97 "TTSPlayer|global data address is %d"
#define EJTTSPLAYER_AUDIO_1110_112_2_18_3_3_41_98 "TTSPlayer|Init errcode=%d"
#define EJTTSPLAYER_AUDIO_1134_112_2_18_3_3_41_99 "TTSPlayer|entry JTTS_Stop audio_handle = %d"
#define EJTTSPLAYER_AUDIO_1137_112_2_18_3_3_41_100 "TTSPlayer|Status JTTS_Stop is %d"
#define EJTTSPLAYER_AUDIO_1147_112_2_18_3_3_41_101 "TTSPlayer|JTTS_Stop g_playsize 0 g_bplayed True g_bWrite false"
#define EJTTSPLAYER_AUDIO_1155_112_2_18_3_3_41_102 "TTSPlayer|Stop SynthText"
#define EJTTSPLAYER_AUDIO_1168_112_2_18_3_3_41_103 "TTSPlayer|leave JTTS_Stop;"
#define EJTTSPLAYER_AUDIO_1180_112_2_18_3_3_41_104 "TTSPlayer|JTTS_Resume audio_handle = %d"
#define EJTTSPLAYER_AUDIO_1189_112_2_18_3_3_41_105 "TTSPlayer|JTTS_Resume status=%d"
#define EJTTSPLAYER_AUDIO_1191_112_2_18_3_3_41_106 "TTSPlayer|Set ~Pause status"
#define EJTTSPLAYER_AUDIO_1201_112_2_18_3_3_41_107 "TTSPlayer|JTTS_Resume status=%d"
#define EJTTSPLAYER_AUDIO_1203_112_2_18_3_3_41_108 "TTSPlayer|Leave JTTS_Resume"
#define EJTTSPLAYER_AUDIO_1215_112_2_18_3_3_42_109 "TTSPlayer|Entry JTTS_Pause audio_handle = %d"
#define EJTTSPLAYER_AUDIO_1223_112_2_18_3_3_42_110 "TTSPlayer|begin JTTS_Pause status = %d"
#define EJTTSPLAYER_AUDIO_1226_112_2_18_3_3_42_111 "TTSPlayer|JTTS_Pause Set pause status"
#define EJTTSPLAYER_AUDIO_1240_112_2_18_3_3_42_112 "TTSPlayer|Leave JTTS_Pause"
#define EJTTSPLAYER_AUDIO_1266_112_2_18_3_3_42_113 "TTSPlayer|Entry JTTS_Play"
#define EJTTSPLAYER_AUDIO_1270_112_2_18_3_3_42_114 "TTSPlayer|Invalid input text"
#define EJTTSPLAYER_AUDIO_1284_112_2_18_3_3_42_115 "TTSPlayer|Get audio format failed"
#define EJTTSPLAYER_AUDIO_1286_112_2_18_3_3_42_116 "TTSPlayer|SampleRate=%d Bitpersample=%d"
#define EJTTSPLAYER_AUDIO_1334_112_2_18_3_3_42_117 "TTSPlayer|Entry JTTS_Play audio_handle = %d"
#define EJTTSPLAYER_AUDIO_1348_112_2_18_3_3_42_118 "TTSPlayer|Entry JTTS_Play g_playsize = 0; g_bplayed = FALSE; g_bWrite = TRUE"
#define EJTTSPLAYER_AUDIO_1353_112_2_18_3_3_42_119 "TTSPlayer|tts handle =%d"
#define EJTTSPLAYER_AUDIO_1385_112_2_18_3_3_42_120 "TTSPlayer|Ready to Call jtTTS_SynthesizeText tick = %d"
#define EJTTSPLAYER_AUDIO_1387_112_2_18_3_3_42_121 "TTSPlayer|jtTTS_SynthesizeText dwErrCode =%d"
#define EJTTSPLAYER_AUDIO_1394_112_2_18_3_3_42_122 "TTSPlayer|play wait for play finish"
#define EJTTSPLAYER_AUDIO_1405_112_2_18_3_3_42_123 "TTSPlayer|play wait for play all buffer finish %d size %d"
#define EJTTSPLAYER_AUDIO_1420_112_2_18_3_3_42_124 "TTSPlayer|Leave JTTS_Play"
#define EJTTSPLAYER_AUDIO_1453_112_2_18_3_3_42_125 "TTSPlayer|file name len is %d"
#define EJTTSPLAYER_AUDIO_1459_112_2_18_3_3_42_126 "Entry JTTS_PlayToFile"
#define EJTTSPLAYER_AUDIO_1463_112_2_18_3_3_42_127 "TTSPlayer|NOT INIT"
#define EJTTSPLAYER_AUDIO_1472_112_2_18_3_3_42_128 "TTSPlayer|Set Wave format failed"
#define EJTTSPLAYER_AUDIO_1478_112_2_18_3_3_42_129 "TTSPlayer|Get audio format failed"
#define EJTTSPLAYER_AUDIO_1480_112_2_18_3_3_42_130 "TTSPlayer|SampleRate=%d Bitpersample=%d"
#define EJTTSPLAYER_AUDIO_1487_112_2_18_3_3_42_131 "TTSPlayer|Can not open file"
#define EJTTSPLAYER_AUDIO_1495_112_2_18_3_3_42_132 "TTSPlayer|Get audio format failed"
#define EJTTSPLAYER_AUDIO_1502_112_2_18_3_3_42_133 "TTSPlayer|Get audio format failed"
#define EJTTSPLAYER_AUDIO_1508_112_2_18_3_3_42_134 "TTSPlayer|Get audio format failed"
#define EJTTSPLAYER_AUDIO_1515_112_2_18_3_3_42_135 "TTSPlayer|Get audio format failed"
#define EJTTSPLAYER_AUDIO_1525_112_2_18_3_3_42_136 "TTSPlayer|Leave JTTS_Playtofile"
#define EJTTSPLAYER_AUDIO_1589_112_2_18_3_3_42_137 "** JTTS_Event_Wait event_id=%d"
#define EJTTSPLAYER_AUDIO_1683_112_2_18_3_3_42_138 "JTTS_AudioNotifyCallback enter"
#define EJTTSPLAYER_AUDIO_1694_112_2_18_3_3_42_139 "TTS_GetAudiohandle s_tts_audio_handle= %d"
#define EJTTSPLAYER_AUDIO_1713_112_2_18_3_3_42_140 "** TTS_IsCommandStatusNone nStatus=%d"
#define EJTTSPLAYER_AUDIO_1731_112_2_18_3_3_43_141 "** TTS_IsPlayStatus nStatus=%d"
#define EJTTSPLAYER_AUDIO_1830_112_2_18_3_3_43_142 "TTSPlayer|ejTTSPlayer_Init in"
#define EJTTSPLAYER_AUDIO_1834_112_2_18_3_3_43_143 "TTSPlayer|lSize=%d"
#define EJTTSPLAYER_AUDIO_1835_112_2_18_3_3_43_144 "TTSPlayer|GetExtBufSize|dwErrCode=%d"
#define EJTTSPLAYER_AUDIO_1840_112_2_18_3_3_43_145 "TTSPlayer|alloc memory failed"
#define EJTTSPLAYER_AUDIO_1843_112_2_18_3_3_43_146 "TTSPlayer|global data address is %d"
#define EJTTSPLAYER_AUDIO_1847_112_2_18_3_3_43_147 "TTSPlayer|Init errcode=%d"
#define EJTTSPLAYER_AUDIO_1851_112_2_18_3_3_43_148 "TTSPlayer|ejTTSPlayer_Init success"
#define EJTTSPLAYER_AUDIO_1853_112_2_18_3_3_43_149 "TTSPlayer|Entry JTTS_PlayToFile"
#define EJTTSPLAYER_AUDIO_1857_112_2_18_3_3_43_150 "TTSPlayer|TTSPlayer|NOT INIT"
#define EJTTSPLAYER_AUDIO_1864_112_2_18_3_3_43_151 "TTSPlayer|Set Wave format failed"
#define EJTTSPLAYER_AUDIO_1870_112_2_18_3_3_43_152 "TTSPlayer|Get audio format failed"
#define EJTTSPLAYER_AUDIO_1872_112_2_18_3_3_43_153 "TTSPlayer|SampleRate=%d Bitpersample=%d"
#define EJTTSPLAYER_AUDIO_1878_112_2_18_3_3_43_154 "TTSPlayer|Can not open file"
#define EJTTSPLAYER_AUDIO_1885_112_2_18_3_3_43_155 "TTSPlayer|INPUTTXT_MODE=%d"
#define EJTTSPLAYER_AUDIO_1890_112_2_18_3_3_43_156 "TTSPlayer|OUTPUT_CALLBACK=%d"
#define EJTTSPLAYER_AUDIO_1894_112_2_18_3_3_43_157 "TTSPlayer|CALLBACK_USERDATA=%d"
#define EJTTSPLAYER_AUDIO_1909_112_2_18_3_3_43_158 "TTSPlayer|Leave JTTS_Playtofile"
#define TTS_IFLYTEK_API_307_112_2_18_3_3_44_159 "AUDIO_PcmStreamPlayCallback()  enter1 time:%u used =%d"
#define TTS_IFLYTEK_API_310_112_2_18_3_3_44_160 "[MMITTS]: AUDIO_PcmStreamPlayCallback s_tts_stream_buffer ERROR!!!!!!!!!!!!!!!"
#define TTS_IFLYTEK_API_327_112_2_18_3_3_44_161 "AUDIO_PcmStreamPlayCallback()  waiting time:%u used =%d"
#define TTS_IFLYTEK_API_343_112_2_18_3_3_44_162 "AUDIO_PcmStreamPlayCallback()  enter2 time:%u used =%d"
#define TTS_IFLYTEK_API_355_112_2_18_3_3_44_163 "AUDIO_PcmStreamPlayCallback()  g_tts_pcm_stream_is_over SET TRUE!!!"
#define TTS_IFLYTEK_API_373_112_2_18_3_3_44_164 "KiTTS_PcmStreamOutputCallback()  enter time:%u counter =%d"
#define TTS_IFLYTEK_API_381_112_2_18_3_3_44_165 "KiTTS_PcmStreamOutputCallback()  waiting time:%u empty =%d"
#define TTS_IFLYTEK_API_396_112_2_18_3_3_44_166 "KiTTS_PcmStreamOutputCallback()  error exit time:%u counter =%d"
#define TTS_IFLYTEK_API_404_112_2_18_3_3_44_167 "KiTTS_PcmStreamOutputCallback()  s_kitts_callback_count==TTS_CALLBACK_CHECKPOINT "
#define TTS_IFLYTEK_API_405_112_2_18_3_3_44_168 "KiTTS_PcmStreamOutputCallback() begin to play:%u used =%d"
#define TTS_IFLYTEK_API_412_112_2_18_3_3_44_169 "KiTTS_PcmStreamOutputCallback()  enter time:%u used =%d"
#define TTS_IFLYTEK_API_530_112_2_18_3_3_44_170 "TTS_GetQueueAvilable: %d"
#define TTS_IFLYTEK_API_551_112_2_18_3_3_44_171 "[MMITTS]:MMITTS_SendSignaltoTask sig_ptr error!!!!!!!!!!!!!!!"
#define TTS_IFLYTEK_API_562_112_2_18_3_3_44_172 "[MMITTS]:MMITTS_SendSignaltoTask SCI_SendSignal error!!!!!!!!!!!!!!!"
#define TTS_IFLYTEK_API_576_112_2_18_3_3_44_173 "TTS_Task start"
#define TTS_IFLYTEK_API_588_112_2_18_3_3_45_174 "TTS_Task :recieve MMITTS_MSG_EXIT"
#define TTS_IFLYTEK_API_595_112_2_18_3_3_45_175 "TTS_Task :recieve MMITTS_MSG_START"
#define TTS_IFLYTEK_API_618_112_2_18_3_3_45_176 "TTS_Task :MMIAPIFMM_CreateFile: handle=%u"
#define TTS_IFLYTEK_API_644_112_2_18_3_3_45_177 "TTS_Task :recieve MMITTS_MSG_STOP"
#define TTS_IFLYTEK_API_651_112_2_18_3_3_45_178 "TTS_Task :recieve MMITTS_MSG_OVER"
#define TTS_IFLYTEK_API_683_112_2_18_3_3_45_179 "TTS_Task exit"
#define TTS_IFLYTEK_API_711_112_2_18_3_3_45_180 "TTSAPI_SynthTextByTask enter"
#define TTS_IFLYTEK_API_716_112_2_18_3_3_45_181 "TTSAPI_SynthTextByTask no res file exit"
#define TTS_IFLYTEK_API_721_112_2_18_3_3_45_182 "TTSAPI_SynthTextByTask res file find"
#define TTS_IFLYTEK_API_732_112_2_18_3_3_45_183 "TTSAPI_SynthTextByTask no memory exit"
#define TTS_IFLYTEK_API_766_112_2_18_3_3_45_184 "TTSAPI_SynthTextByTask task created"
#define TTS_IFLYTEK_API_773_112_2_18_3_3_45_185 "TTSAPI_SynthTextByTask task while"
#define TTS_IFLYTEK_API_778_112_2_18_3_3_45_186 "TTSAPI_SynthTextByTask task quue exit"
#define TTS_IFLYTEK_API_791_112_2_18_3_3_45_187 "TTSAPI_SynthTextByTask task exit"
#define TTS_IFLYTEK_API_843_112_2_18_3_3_45_188 " tts tts_resDataCathe  is not null"
#define TTS_IFLYTEK_API_857_112_2_18_3_3_45_189 " tts KiTTS_HeapBuffer   is not null"
#define TTS_IFLYTEK_API_870_112_2_18_3_3_45_190 "tts s_tts_voice_data   is not null"
#define TTS_IFLYTEK_API_884_112_2_18_3_3_45_191 "[MMITTS]:AllocTTS_StreamBuffer PNULL != s_tts_stream_buffer ERROR!!!!!!!!!!!!!"
#define TTS_IFLYTEK_API_899_112_2_18_3_3_45_192 "[MMITTS]:FreeTTS_StreamBuffer"
#define TTS_IFLYTEK_API_913_112_2_18_3_3_45_193 " [MMITTS]: TTS_MemoryAlloc entered"
#define TTS_IFLYTEK_API_916_112_2_18_3_3_45_194 " [MMITTS]: MemoryAlloc HeapBuffer failed!!"
#define TTS_IFLYTEK_API_922_112_2_18_3_3_45_195 " [MMITTS]:  AllocTTS_VoiceData failed!!"
#define TTS_IFLYTEK_API_928_112_2_18_3_3_45_196 " [MMITTS]:  AllocTTS_StreamBuffer failed!!"
#define TTS_IFLYTEK_API_935_112_2_18_3_3_45_197 "[MMITTS]:  AllocTTS_ResDataCathe failed!!"
#define TTS_IFLYTEK_API_941_112_2_18_3_3_45_198 "[MMITTS]:KiTTS_HeapBuffer address = %x."
#define TTS_IFLYTEK_API_942_112_2_18_3_3_45_199 "[MMITTS]:KiTTS_voice data address = %x."
#define TTS_IFLYTEK_API_962_112_2_18_3_3_45_200 " tts TTS_MemoryRelease entered"
#define TTS_IFLYTEK_API_981_112_2_18_3_3_45_201 "MMITTS TTSAPI_StopSynthTextByTask entry time: %u"
#define TTS_IFLYTEK_API_1022_112_2_18_3_3_45_202 "TTSAPI_StopSynthTextByTask exit time: %u"
#define TTS_IFLYTEK_API_1031_112_2_18_3_3_46_203 "MMITTS TTSAPI_StopASynchTextByTask entry time: %u"
#define TTS_IFLYTEK_API_1087_112_2_18_3_3_46_204 "[MMITTS]:ReadResCB FAIL!!!!!!!!"
#define TTS_IFLYTEK_API_1119_112_2_18_3_3_46_205 "MMITTS: InputCB pParameter= %u, txtPar=%d ,copyLen=%d,pParam=%u,txtPar->position=%d"
#define TTS_IFLYTEK_API_1141_112_2_18_3_3_46_206 "MMITTS: TTS_SynthText txtPar= %u, HeapBuffer=%d ,HEAP_BUFFER_SIZE=%d,ResData=%u"
#define TTS_IFLYTEK_API_1222_112_2_18_3_3_46_207 "[MMITTS]:MMITTS_SynthText ivTTS_SynthStart"
#define TTS_IFLYTEK_API_1235_112_2_18_3_3_46_208 "[MMITTS]:MMITTS_SynthText jump_value=%d"
#define TTS_IFLYTEK_API_1259_112_2_18_3_3_46_209 "MMITTS, HandleTTSPlayOverTimerMsg entered"
#define TTS_IFLYTEK_API_1262_112_2_18_3_3_46_210 "TTS_HandleTTSPlayOverTimerMsg:handle s_tts_playover_timer_id!"
#define TTS_IFLYTEK_API_1299_112_2_18_3_3_46_211 "TTSAPI_PlayText() txt_len_in_byte= %d"
#define TTS_IFLYTEK_API_1300_112_2_18_3_3_46_212 "TTSAPI_PlayText()  s_tts_text.tts_start_pos = %d,s_tts_text.tts_str_len = %d"
#define TTS_IFLYTEK_API_1417_112_2_18_3_3_46_213 "TTSAPI_PlayText()  s_tts_text.tts_start_pos = %d,s_tts_text.tts_str_len = %d"
#define TTS_IFLYTEK_API_1445_112_2_18_3_3_46_214 "TTSAPI_StopPlayTextByType ring_type=%d"
#define TTS_IFLYTEK_API_1465_112_2_18_3_3_46_215 "TTSAPI_StopPlayText entry time: %u"
#define TTS_IFLYTEK_API_1472_112_2_18_3_3_46_216 "TTSAPI_StopPlayText exit time: %u"
#define TTS_IFLYTEK_API_1506_112_2_18_3_3_46_217 "TTSAPI_StopPlayTextByType ring_type=%d"
#define TTS_IFLYTEK_API_1575_112_2_18_3_3_47_218 "MMIDCD create dcd main dir fail"
#define TTS_IFLYTEK_API_1608_112_2_18_3_3_47_219 "TTS GetTTSResouceFileFullName error"
#define TTS_IFLYTEK_API_1643_112_2_18_3_3_47_220 "TTS c: disk not exist resouce file"
#define TTS_IFLYTEK_API_1670_112_2_18_3_3_47_221 "TTS %d disk not exist resouce file"
#define TTS_IFLYTEK_API_1703_112_2_18_3_3_47_222 "TTS TTSAPI_checkResFile has passed "
#define TTS_IFLYTEK_API_1712_112_2_18_3_3_47_223 "TTS TTSAPI_checkResFile not find the resouce file  "
#define TTS_IFLYTEK_API_1732_112_2_18_3_3_47_224 "[MMITTS]:MMIAPITTS_PlayTextForControl ERROR!!!!!!!!!!!!"
#define TTS_IFLYTEK_AUDIO_66_112_2_18_3_3_48_225 "enter funciton TTS_CreatePcmStreamHandle()"
#define TTS_IFLYTEK_AUDIO_190_112_2_18_3_3_48_226 "enter funciton TTS_STOP_CALLBACK() g_tts_is_running=%d"
#define TTS_IFLYTEK_AUDIO_228_112_2_18_3_3_48_227 "play_stream_id = %d, cur_stream_id = %d"
#define TTS_IFLYTEK_AUDIO_232_112_2_18_3_3_48_228 "enter funciton VTTSAPI_PlayPcmStream()"
#define TTS_IFLYTEK_AUDIO_236_112_2_18_3_3_48_229 "TTSAPI_PlayPcmStream() Handle error !!!!"
#define TTS_IFLYTEK_AUDIO_295_112_2_18_3_3_48_230 "enter funciton TTS_StopAndClosePcmStreamHandle()"
#define TTS_IFLYTEK_AUDIO_355_112_2_18_3_3_48_231 "[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x"
#define TTS_IFLYTEK_AUDIO_360_112_2_18_3_3_48_232 "[MMISRV]: HandleNotify() report_ptr=0x%x"
#define TTS_IFLYTEK_AUDIO_364_112_2_18_3_3_48_233 "[MMISRV]: HandleNotify(), report_ptr->report=%d"
#define TTS_YOUNGTONG_API_390_112_2_18_3_3_49_234 "AUDIO_PcmStreamPlayCallback()  enter1 time:%u used =%d"
#define TTS_YOUNGTONG_API_393_112_2_18_3_3_49_235 "[MMITTS]: AUDIO_PcmStreamPlayCallback s_tts_stream_buffer ERROR!!!!!!!!!!!!!!!"
#define TTS_YOUNGTONG_API_410_112_2_18_3_3_49_236 "AUDIO_PcmStreamPlayCallback()  waiting time:%u used =%d"
#define TTS_YOUNGTONG_API_426_112_2_18_3_3_49_237 "AUDIO_PcmStreamPlayCallback()  enter2 time:%u used =%d"
#define TTS_YOUNGTONG_API_438_112_2_18_3_3_49_238 "AUDIO_PcmStreamPlayCallback()  g_tts_pcm_stream_is_over SET TRUE!!!"
#define TTS_YOUNGTONG_API_460_112_2_18_3_3_49_239 "call back enter save file"
#define TTS_YOUNGTONG_API_471_112_2_18_3_3_49_240 "create file"
#define TTS_YOUNGTONG_API_481_112_2_18_3_3_49_241 "callback_function_for_yt_tts() enter"
#define TTS_YOUNGTONG_API_498_112_2_18_3_3_49_242 "nSampleNumber:%d,write_size:%d"
#define TTS_YOUNGTONG_API_511_112_2_18_3_3_49_243 "callback_function_for_yt_tts()  enter time:%u counter =%d dataLen= %d"
#define TTS_YOUNGTONG_API_520_112_2_18_3_3_49_244 "callback_function_for_yt_tts()  cycle error exit time:%u counter =%d"
#define TTS_YOUNGTONG_API_529_112_2_18_3_3_50_245 "callback_function_for_yt_tts()  waiting time:%u empty =%d"
#define TTS_YOUNGTONG_API_537_112_2_18_3_3_50_246 "callback_function_for_yt_tts()  error exit time:%u counter =%d"
#define TTS_YOUNGTONG_API_545_112_2_18_3_3_50_247 "callback_function_for_yt_tts()  s_kitts_callback_count==TTS_CALLBACK_CHECKPOINT "
#define TTS_YOUNGTONG_API_553_112_2_18_3_3_50_248 "callback_function_for_yt_tts()  enter time:%u used =%d"
#define TTS_YOUNGTONG_API_559_112_2_18_3_3_50_249 "callback_function_for_yt_tts()  enter time:%u used =%d"
#define TTS_YOUNGTONG_API_569_112_2_18_3_3_50_250 "YT LOG: %s"
#define TTS_YOUNGTONG_API_782_112_2_18_3_3_50_251 "TTS_GetQueueAvilable: %d"
#define TTS_YOUNGTONG_API_803_112_2_18_3_3_50_252 "[MMITTS]:MMITTS_SendSignaltoTask sig_ptr error!!!!!!!!!!!!!!!"
#define TTS_YOUNGTONG_API_814_112_2_18_3_3_50_253 "[MMITTS]:MMITTS_SendSignaltoTask SCI_SendSignal error!!!!!!!!!!!!!!!"
#define TTS_YOUNGTONG_API_830_112_2_18_3_3_50_254 "TTS_Task start"
#define TTS_YOUNGTONG_API_842_112_2_18_3_3_50_255 "TTS_Task :recieve MMITTS_MSG_EXIT"
#define TTS_YOUNGTONG_API_849_112_2_18_3_3_50_256 "TTS_Task :recieve MMITTS_MSG_START"
#define TTS_YOUNGTONG_API_873_112_2_18_3_3_50_257 "TTS_Task :MMIAPIFMM_CreateFile: handle=%u"
#define TTS_YOUNGTONG_API_904_112_2_18_3_3_50_258 "TTS_Task :recieve MMITTS_MSG_STOP"
#define TTS_YOUNGTONG_API_911_112_2_18_3_3_50_259 "TTS_Task :recieve MMITTS_MSG_OVER"
#define TTS_YOUNGTONG_API_943_112_2_18_3_3_50_260 "TTS_Task exit"
#define TTS_YOUNGTONG_API_971_112_2_18_3_3_50_261 "TTSAPI_SynthTextByTask enter"
#define TTS_YOUNGTONG_API_976_112_2_18_3_3_50_262 "TTSAPI_SynthTextByTask no res file exit"
#define TTS_YOUNGTONG_API_981_112_2_18_3_3_50_263 "TTSAPI_SynthTextByTask res file find"
#define TTS_YOUNGTONG_API_992_112_2_18_3_3_50_264 "TTSAPI_SynthTextByTask no memory exit"
#define TTS_YOUNGTONG_API_1003_112_2_18_3_3_50_265 "TTSAPI_SynthTextByTask :MMIAPIFMM_CreateFile: handle=%u"
#define TTS_YOUNGTONG_API_1022_112_2_18_3_3_51_266 "TTSAPI_SynthTextByTask :MMIAPIFMM_GetFileSize=%u"
#define TTS_YOUNGTONG_API_1075_112_2_18_3_3_51_267 "TTSAPI_SynthTextByTask task created"
#define TTS_YOUNGTONG_API_1082_112_2_18_3_3_51_268 "TTSAPI_SynthTextByTask task while"
#define TTS_YOUNGTONG_API_1087_112_2_18_3_3_51_269 "TTSAPI_SynthTextByTask task quue exit"
#define TTS_YOUNGTONG_API_1099_112_2_18_3_3_51_270 "TTSAPI_SynthTextByTask task exit"
#define TTS_YOUNGTONG_API_1151_112_2_18_3_3_51_271 " tts tts_resDataCathe  is not null"
#define TTS_YOUNGTONG_API_1165_112_2_18_3_3_51_272 " tts KiTTS_HeapBuffer   is not null"
#define TTS_YOUNGTONG_API_1178_112_2_18_3_3_51_273 "tts s_tts_voice_data   is not null"
#define TTS_YOUNGTONG_API_1192_112_2_18_3_3_51_274 "[MMITTS]:AllocTTS_StreamBuffer PNULL != s_tts_stream_buffer ERROR!!!!!!!!!!!!!"
#define TTS_YOUNGTONG_API_1207_112_2_18_3_3_51_275 "[MMITTS]:FreeTTS_StreamBuffer"
#define TTS_YOUNGTONG_API_1221_112_2_18_3_3_51_276 " [MMITTS]: TTS_MemoryAlloc entered"
#define TTS_YOUNGTONG_API_1224_112_2_18_3_3_51_277 " [MMITTS]: MemoryAlloc HeapBuffer failed!!"
#define TTS_YOUNGTONG_API_1230_112_2_18_3_3_51_278 " [MMITTS]:  AllocTTS_VoiceData failed!!"
#define TTS_YOUNGTONG_API_1236_112_2_18_3_3_51_279 " [MMITTS]:  AllocTTS_StreamBuffer failed!!"
#define TTS_YOUNGTONG_API_1244_112_2_18_3_3_51_280 "[MMITTS]:  AllocTTS_ResDataCathe failed!!"
#define TTS_YOUNGTONG_API_1251_112_2_18_3_3_51_281 "[MMITTS]:KiTTS_HeapBuffer address = %x."
#define TTS_YOUNGTONG_API_1252_112_2_18_3_3_51_282 "[MMITTS]:KiTTS_voice data address = %x."
#define TTS_YOUNGTONG_API_1274_112_2_18_3_3_51_283 " tts TTS_MemoryRelease entered"
#define TTS_YOUNGTONG_API_1295_112_2_18_3_3_51_284 "MMITTS TTSAPI_StopSynthTextByTask entry time: %u"
#define TTS_YOUNGTONG_API_1336_112_2_18_3_3_51_285 "TTSAPI_StopSynthTextByTask exit time: %u"
#define TTS_YOUNGTONG_API_1345_112_2_18_3_3_51_286 "MMITTS TTSAPI_StopASynchTextByTask entry time: %u"
#define TTS_YOUNGTONG_API_1401_112_2_18_3_3_51_287 "[MMITTS]:ReadResCB FAIL!!!!!!!!"
#define TTS_YOUNGTONG_API_1433_112_2_18_3_3_51_288 "MMITTS: InputCB pParameter= %u, txtPar=%d ,copyLen=%d,pParam=%u,txtPar->position=%d"
#define TTS_YOUNGTONG_API_1455_112_2_18_3_3_51_289 "TTS_SynthText enter"
#define TTS_YOUNGTONG_API_1461_112_2_18_3_3_51_290 "TTS_SynthText set mem"
#define TTS_YOUNGTONG_API_1465_112_2_18_3_3_51_291 "TTS_SynthText yt init"
#define TTS_YOUNGTONG_API_1480_112_2_18_3_3_51_292 "hidi length %d"
#define TTS_YOUNGTONG_API_1486_112_2_18_3_3_51_293 "hidi times %d,time:%u"
#define TTS_YOUNGTONG_API_1494_112_2_18_3_3_51_294 "hidi stop after time:%u"
#define TTS_YOUNGTONG_API_1500_112_2_18_3_3_51_295 "1:%d,2:%d,3:%d,4:%d"
#define TTS_YOUNGTONG_API_1523_112_2_18_3_3_52_296 "TTS_SynthText enter"
#define TTS_YOUNGTONG_API_1526_112_2_18_3_3_52_297 "TTS_SynthText set mem"
#define TTS_YOUNGTONG_API_1534_112_2_18_3_3_52_298 "string ptr before ini:%x"
#define TTS_YOUNGTONG_API_1538_112_2_18_3_3_52_299 "string ptr after ini:%x"
#define TTS_YOUNGTONG_API_1540_112_2_18_3_3_52_300 "TTS_SynthText yt init"
#define TTS_YOUNGTONG_API_1554_112_2_18_3_3_52_301 "hidi length %d"
#define TTS_YOUNGTONG_API_1556_112_2_18_3_3_52_302 "before yt_tts_input_text_utf16"
#define TTS_YOUNGTONG_API_1563_112_2_18_3_3_52_303 "after yt_tts_input_text_utf16"
#define TTS_YOUNGTONG_API_1570_112_2_18_3_3_52_304 "hidi times %d,time:%u"
#define TTS_YOUNGTONG_API_1578_112_2_18_3_3_52_305 "hidi stop after time:%u"
#define TTS_YOUNGTONG_API_1584_112_2_18_3_3_52_306 "1:%d,2:%d,3:%d,4:%d"
#define TTS_YOUNGTONG_API_1590_112_2_18_3_3_52_307 "close file handle:%x"
#define TTS_YOUNGTONG_API_1595_112_2_18_3_3_52_308 "SAVE_PCM_STREAM close file"
#define TTS_YOUNGTONG_API_1603_112_2_18_3_3_52_309 "[MMITTS]:MMITTS_SynthText ivTTS_SynthStart"
#define TTS_YOUNGTONG_API_1615_112_2_18_3_3_52_310 "[MMITTS]:MMITTS_SynthText jump_value=%d"
#define TTS_YOUNGTONG_API_1631_112_2_18_3_3_52_311 "MMITTS, HandleTTSPlayOverTimerMsg entered"
#define TTS_YOUNGTONG_API_1634_112_2_18_3_3_52_312 "TTS_HandleTTSPlayOverTimerMsg:handle s_tts_playover_timer_id!"
#define TTS_YOUNGTONG_API_1671_112_2_18_3_3_52_313 "TTSAPI_PlayText() txt_len_in_byte= %d"
#define TTS_YOUNGTONG_API_1672_112_2_18_3_3_52_314 "TTSAPI_PlayText()  s_tts_text.tts_start_pos = %d,s_tts_text.tts_str_len = %d"
#define TTS_YOUNGTONG_API_1766_112_2_18_3_3_52_315 "s_tts_text.str_arr:%x"
#define TTS_YOUNGTONG_API_1790_112_2_18_3_3_52_316 "TTSAPI_PlayText()  s_tts_text.tts_start_pos = %d,s_tts_text.tts_str_len = %d"
#define TTS_YOUNGTONG_API_1818_112_2_18_3_3_52_317 "TTSAPI_StopPlayTextByType ring_type=%d"
#define TTS_YOUNGTONG_API_1838_112_2_18_3_3_52_318 "TTSAPI_StopPlayText entry time: %u"
#define TTS_YOUNGTONG_API_1845_112_2_18_3_3_52_319 "TTSAPI_StopPlayText exit time: %u"
#define TTS_YOUNGTONG_API_1879_112_2_18_3_3_52_320 "TTSAPI_StopPlayTextByType ring_type=%d"
#define TTS_YOUNGTONG_API_1948_112_2_18_3_3_52_321 "MMIDCD create dcd main dir fail"
#define TTS_YOUNGTONG_API_1982_112_2_18_3_3_52_322 "[MMITTS]:GetTTSResouceFileFullName error!!!!!!!!!!!!"
#define TTS_YOUNGTONG_API_1999_112_2_18_3_3_52_323 "TTS GetTTSResouceFileFullName entry"
#define TTS_YOUNGTONG_API_2032_112_2_18_3_3_53_324 "TTS GetTTSResouceFileFullName error"
#define TTS_YOUNGTONG_API_2035_112_2_18_3_3_53_325 "TTS GetTTSResouceFileFullName + full_path_name_ptr:%s"
#define TTS_YOUNGTONG_API_2072_112_2_18_3_3_53_326 "TTS c: disk not exist resouce file"
#define TTS_YOUNGTONG_API_2099_112_2_18_3_3_53_327 "TTS %d disk not exist resouce file"
#define TTS_YOUNGTONG_API_2131_112_2_18_3_3_53_328 "TTS TTSAPI_checkResFile not find the resouce file  "
#define TTS_YOUNGTONG_API_2153_112_2_18_3_3_53_329 "TTS_copyFileInfo tts_language_type:%x"
#define TTS_YOUNGTONG_API_2239_112_2_18_3_3_53_330 "TTS_copyFileInfo s_mmitts_res_file_name_1:%s"
#define TTS_YOUNGTONG_API_2240_112_2_18_3_3_53_331 "TTS_copyFileInfo s_mmitts_default_file_path_1:%s"
#define TTS_YOUNGTONG_API_2241_112_2_18_3_3_53_332 "TTS_copyFileInfo s_char_mmitts_default_file_path_1:%s"
#define TTS_YOUNGTONG_API_2243_112_2_18_3_3_53_333 "TTS_copyFileInfo s_mmitts_res_file_name_2:%s"
#define TTS_YOUNGTONG_API_2244_112_2_18_3_3_53_334 "TTS_copyFileInfo s_mmitts_default_file_path_2:%s"
#define TTS_YOUNGTONG_API_2245_112_2_18_3_3_53_335 "TTS_copyFileInfo s_char_mmitts_default_file_path_2:%s"
#define TTS_YOUNGTONG_API_2350_112_2_18_3_3_53_336 "TTSAPI_checkResFile: TTS Udisk exist resouce file"
#define TTS_YOUNGTONG_API_2363_112_2_18_3_3_53_337 "TTSAPI_checkResFile: TTS Udisk not exist resouce file"
#define TTS_YOUNGTONG_API_2385_112_2_18_3_3_53_338 "TTSAPI_checkResFile: TTS SDCard exist resouce file"
#define TTS_YOUNGTONG_API_2398_112_2_18_3_3_53_339 "TTS SDCard not exist resouce file"
#define TTS_YOUNGTONG_API_2420_112_2_18_3_3_53_340 "[MMITTS]:MMIAPITTS_PlayTextForControl ERROR!!!!!!!!!!!!"
#define TTS_YOUNGTONG_AUDIO_71_112_2_18_3_3_54_341 "enter funciton TTS_CreatePcmStreamHandle()"
#define TTS_YOUNGTONG_AUDIO_193_112_2_18_3_3_54_342 "enter funciton TTS_STOP_CALLBACK() tts_is_running=%d"
#define TTS_YOUNGTONG_AUDIO_230_112_2_18_3_3_55_343 "play_stream_id = %d, cur_stream_id = %d"
#define TTS_YOUNGTONG_AUDIO_234_112_2_18_3_3_55_344 "enter funciton TTSAPI_PlayPcmStream()"
#define TTS_YOUNGTONG_AUDIO_238_112_2_18_3_3_55_345 "TTSAPI_PlayPcmStream() Handle error !!!!"
#define TTS_YOUNGTONG_AUDIO_297_112_2_18_3_3_55_346 "enter funciton TTS_StopAndClosePcmStreamHandle()"
#define TTS_YOUNGTONG_AUDIO_359_112_2_18_3_3_55_347 "[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x"
#define TTS_YOUNGTONG_AUDIO_364_112_2_18_3_3_55_348 "[MMISRV]: HandleNotify() report_ptr=0x%x"
#define TTS_YOUNGTONG_AUDIO_368_112_2_18_3_3_55_349 "[MMISRV]: HandleNotify(), report_ptr->report=%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_TTS_TRC)
TRACE_MSG(TTS_AUDIO_57_112_2_18_3_3_37_0,"enter funciton MMITTS_CreatePCMStreamHandle()")
TRACE_MSG(TTS_AUDIO_129_112_2_18_3_3_37_1,"enter funciton MMITTS_STOP_CALLBACK() g_tts_is_running=%d")
TRACE_MSG(TTS_AUDIO_164_112_2_18_3_3_37_2,"play_stream_id = %d, cur_stream_id = %d")
TRACE_MSG(TTS_AUDIO_168_112_2_18_3_3_37_3,"enter funciton MMIAPITTS_PlayPcmStream()")
TRACE_MSG(TTS_AUDIO_172_112_2_18_3_3_37_4,"MMIAPITTS_PlayPcmStream() Handle error !!!!")
TRACE_MSG(TTS_AUDIO_223_112_2_18_3_3_37_5,"enter funciton MMITTS_StopAndClosePcmStreamHandle()")
TRACE_MSG(EJTTSPLAYER_API_140_112_2_18_3_3_38_6,"TTSAPI_PlayText enter txt_len_in_byte = %d")
TRACE_MSG(EJTTSPLAYER_API_208_112_2_18_3_3_38_7,"TTSAPI_PlayText %x, %x")
TRACE_MSG(EJTTSPLAYER_API_209_112_2_18_3_3_38_8,"TTSAPI_PlayText ring_type = %d, tts_vol = %d")
TRACE_MSG(EJTTSPLAYER_API_217_112_2_18_3_3_38_9,"TTSAPI_PlayText enter stop jttsplayer")
TRACE_MSG(EJTTSPLAYER_API_246_112_2_18_3_3_38_10," GetTTSResouceFileFullName result = %d")
TRACE_MSG(EJTTSPLAYER_API_275_112_2_18_3_3_38_11," GetTTSENResouceFileFullName result = %d")
TRACE_MSG(EJTTSPLAYER_API_294_112_2_18_3_3_38_12,"MMITTS_HandlePlayTextTimer")
TRACE_MSG(EJTTSPLAYER_API_299_112_2_18_3_3_38_13,"MMITTS_HandlePlayTextTimer | ejTTSPlayer_GetStatus = %d")
TRACE_MSG(EJTTSPLAYER_API_329_112_2_18_3_3_38_14,"HandlePlayTextTimer stop jtts palyer")
TRACE_MSG(EJTTSPLAYER_API_341_112_2_18_3_3_38_15,"MMITTS_StopPlayText")
TRACE_MSG(EJTTSPLAYER_API_383_112_2_18_3_3_38_16,"TTSAPI_GetAudioIsOpen | audio_handle = %d")
TRACE_MSG(EJTTSPLAYER_API_408_112_2_18_3_3_38_17,"TTSAPI_SynthTextByTask enter")
TRACE_MSG(EJTTSPLAYER_API_440_112_2_18_3_3_38_18,"TTSAPI_SynthTextByTask stop jtts")
TRACE_MSG(EJTTSPLAYER_API_445_112_2_18_3_3_38_19,"TTSAPI_SynthTextByTask | ejTTSPlayer_GetStatus = %d")
TRACE_MSG(EJTTSPLAYER_API_479_112_2_18_3_3_38_20,"TTSAPI_NotifyTTSOver enter, s_ttsinfo.win_handle = %d")
TRACE_MSG(EJTTSPLAYER_API_492_112_2_18_3_3_38_21,"TTSAPI_StopPlayTextByType ring_type=%d,  s_ttsinfo.ring_type = %d")
TRACE_MSG(EJTTSPLAYER_API_636_112_2_18_3_3_38_22,"[MMITTS]:TTSAPI_PlayTextForControl ERROR!!!!!!!!!!!!")
TRACE_MSG(EJTTSPLAYER_API_641_112_2_18_3_3_39_23,"TTSAPI_PlayTextForControl enter")
TRACE_MSG(EJTTSPLAYER_API_753_112_2_18_3_3_39_24,"HandleAudioEnv is_begin = %d audio_handle = %d")
TRACE_MSG(EJTTSPLAYER_API_760_112_2_18_3_3_39_25,"HandleAudioEnv() Handle error !!!!")
TRACE_MSG(EJTTSPLAYER_API_784_112_2_18_3_3_39_26,"JTTS_PlayCallback() uMessage = %d s_is_manual_stop = %d")
TRACE_MSG(EJTTSPLAYER_API_797_112_2_18_3_3_39_27,"JTTS_PlayCallback() post MSG_TTS_OVER")
TRACE_MSG(EJTTSPLAYER_API_823_112_2_18_3_3_39_28,"TTS_GetManualStop s_is_manual_stop = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_321_112_2_18_3_3_40_29,"TTSPlayer|Entery JTTS_AudioCallBackFunc tick = %d g_bplayed = %d, g_SyncData.Status = %d, hAudio = %d, g_playsize = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_326_112_2_18_3_3_40_30,"TTSPlayer|JTTS_AudioCallBackFunc audio handle is not g_audio_handle")
TRACE_MSG(EJTTSPLAYER_AUDIO_335_112_2_18_3_3_40_31,"TTSPlayer|JTTS_AudioCallBackFunc finish play")
TRACE_MSG(EJTTSPLAYER_AUDIO_339_112_2_18_3_3_40_32,"TTSPlayer|AudioCallBack wait for g_playsize > 0")
TRACE_MSG(EJTTSPLAYER_AUDIO_344_112_2_18_3_3_40_33,"TTSPlayer|JTTS_AudioCallBackFunc g_playsize = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_351_112_2_18_3_3_40_34,"TTSPlayer|JTTS_Audio_CallBackFunc|Out %x %x")
TRACE_MSG(EJTTSPLAYER_AUDIO_374_112_2_18_3_3_40_35,"TTSPlayer|OuputVoiceToFile|In iSize = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_398_112_2_18_3_3_40_36,"TTSPlayer|OutputVoiceToPlayProc tick count = %d iSize = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_403_112_2_18_3_3_40_37,"TTSPlayer|synthe time used = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_413_112_2_18_3_3_40_38,"TTSPlayer|set g_playpos = 0")
TRACE_MSG(EJTTSPLAYER_AUDIO_423_112_2_18_3_3_40_39,"TTSPlayer|wait for write")
TRACE_MSG(EJTTSPLAYER_AUDIO_429_112_2_18_3_3_40_40,"TTSPlayer|pause sleep 100")
TRACE_MSG(EJTTSPLAYER_AUDIO_441_112_2_18_3_3_40_41,"TTSPlayer|Leave OutputVoiceToPlayProc tickcount = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_457_112_2_18_3_3_40_42,"** ejTTSPlayer_Init in")
TRACE_MSG(EJTTSPLAYER_AUDIO_462_112_2_18_3_3_40_43,"** Error! TTS is already init")
TRACE_MSG(EJTTSPLAYER_AUDIO_470_112_2_18_3_3_40_44,"Create Mutex failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_473_112_2_18_3_3_40_45,"TTSPlayer|Create Mutex success")
TRACE_MSG(EJTTSPLAYER_AUDIO_479_112_2_18_3_3_40_46,"TTSPlayer|Create Play Mutex failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_482_112_2_18_3_3_40_47,"TTSPlayer|Create Play Mutex success")
TRACE_MSG(EJTTSPLAYER_AUDIO_491_112_2_18_3_3_40_48,"Create JTTS_GROUP_EVENT failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_511_112_2_18_3_3_40_49,"TTSPlayer|Create Thread failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_515_112_2_18_3_3_40_50,"TTSPlayer|Create JTTS_Thread success")
TRACE_MSG(EJTTSPLAYER_AUDIO_520_112_2_18_3_3_40_51,"JTTS_Init Failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_525_112_2_18_3_3_40_52,"ejTTSPlayer_Init success")
TRACE_MSG(EJTTSPLAYER_AUDIO_536_112_2_18_3_3_40_53,"TTSPlayer|Entry ejTTSPlayer_End")
TRACE_MSG(EJTTSPLAYER_AUDIO_559_112_2_18_3_3_40_54,"TTSPlayer|End Playing")
TRACE_MSG(EJTTSPLAYER_AUDIO_565_112_2_18_3_3_40_55,"TTSPlayer|set exit status")
TRACE_MSG(EJTTSPLAYER_AUDIO_569_112_2_18_3_3_40_56,"TTSPlayer|Set child event")
TRACE_MSG(EJTTSPLAYER_AUDIO_572_112_2_18_3_3_40_57,"TTSPlayer|end wait for parent event")
TRACE_MSG(EJTTSPLAYER_AUDIO_577_112_2_18_3_3_40_58,"TTSPlayer|end wait g_jtts_thread_id = 0")
TRACE_MSG(EJTTSPLAYER_AUDIO_608_112_2_18_3_3_40_59,"TTSPlayer|Leave ejTTSPlayer_End")
TRACE_MSG(EJTTSPLAYER_AUDIO_625_112_2_18_3_3_40_60,"TTSPlayer|Entry ejTTSPlayer_Play")
TRACE_MSG(EJTTSPLAYER_AUDIO_658_112_2_18_3_3_40_61,"** ejTTSPlayer_Play uTextLen = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_664_112_2_18_3_3_40_62,"** ejTTSPlayer_Play commamd status =%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_689_112_2_18_3_3_40_63,"TTSPlayer|Leave ejTTSPlayer_Play")
TRACE_MSG(EJTTSPLAYER_AUDIO_696_112_2_18_3_3_40_64,"TTSPlayer|wait parent event failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_713_112_2_18_3_3_41_65,"Entry ejTTSPlayer_PlayToFile")
TRACE_MSG(EJTTSPLAYER_AUDIO_716_112_2_18_3_3_41_66,"TTSPlayer|Invalid thread id")
TRACE_MSG(EJTTSPLAYER_AUDIO_722_112_2_18_3_3_41_67,"TTSPlayer|Invalid input text")
TRACE_MSG(EJTTSPLAYER_AUDIO_738_112_2_18_3_3_41_68,"TTSPlayer|Leave ejTTSPlayer_PlayToFile")
TRACE_MSG(EJTTSPLAYER_AUDIO_749_112_2_18_3_3_41_69,"TTSPlayer|entry ejTTSPlayer_Stop g_jtts_thread_id = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_756_112_2_18_3_3_41_70,"TTSPlayer|Status is %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_765_112_2_18_3_3_41_71,"TTSPlayer|Stop SynthText")
TRACE_MSG(EJTTSPLAYER_AUDIO_774_112_2_18_3_3_41_72,"TTSPlayer|Sleep 100 wait for play finish")
TRACE_MSG(EJTTSPLAYER_AUDIO_788_112_2_18_3_3_41_73,"TTSPlayer|leave ejTTSPlayer_Stop")
TRACE_MSG(EJTTSPLAYER_AUDIO_810_112_2_18_3_3_41_74,"TTSPlayer|ejTTSPlayer_Resume status=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_813_112_2_18_3_3_41_75,"TTSPlayer|Set ~Pause status")
TRACE_MSG(EJTTSPLAYER_AUDIO_825_112_2_18_3_3_41_76,"TTSPlayer|ejTTSPlayer_Resume status=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_827_112_2_18_3_3_41_77,"TTSPlayer|Leave ejTTSPlayer_Resume")
TRACE_MSG(EJTTSPLAYER_AUDIO_839_112_2_18_3_3_41_78,"TTSPlayer|Entry ejTTSPlayer_Pause")
TRACE_MSG(EJTTSPLAYER_AUDIO_851_112_2_18_3_3_41_79,"TTSPlayer|begin status = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_854_112_2_18_3_3_41_80,"TTSPlayer|Set pause status")
TRACE_MSG(EJTTSPLAYER_AUDIO_871_112_2_18_3_3_41_81,"TTSPlayer|Leave ejTTSPlayer_Pause")
TRACE_MSG(EJTTSPLAYER_AUDIO_881_112_2_18_3_3_41_82,"ejTTSPlayer_GetStatus g_jtts_thread_id = %d, g_SyncData.Status = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_973_112_2_18_3_3_41_83,"TTSPlayer|Entry JTTS_Thread")
TRACE_MSG(EJTTSPLAYER_AUDIO_977_112_2_18_3_3_41_84,"** JTTTS_Thread g_SyncData.Status = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_984_112_2_18_3_3_41_85,"JTTS: SigCode is NULL")
TRACE_MSG(EJTTSPLAYER_AUDIO_987_112_2_18_3_3_41_86,"JTTS: SigCode = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_996_112_2_18_3_3_41_87,"TTSPlayer|wait child event failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_999_112_2_18_3_3_41_88,"TTSPlayer|Event Wait Child event success")
TRACE_MSG(EJTTSPLAYER_AUDIO_1003_112_2_18_3_3_41_89,"TTSPlayer|status exit")
TRACE_MSG(EJTTSPLAYER_AUDIO_1012_112_2_18_3_3_41_90,"TTSPlayer|stop status -> idle status %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1021_112_2_18_3_3_41_91,"TTSPlayer|command status is not init, continue")
TRACE_MSG(EJTTSPLAYER_AUDIO_1031_112_2_18_3_3_41_92,"TTSPlayer|Case TTSPlayer|COMMAND PLAY ")
TRACE_MSG(EJTTSPLAYER_AUDIO_1035_112_2_18_3_3_41_93,"TTSPlayer|ERROR CODE = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1069_112_2_18_3_3_41_94,"TTSPlayer|Exit JTTS_Thread")
TRACE_MSG(EJTTSPLAYER_AUDIO_1092_112_2_18_3_3_41_95,"TTSPlayer|lSize=%d, dwErrCode = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1102_112_2_18_3_3_41_96,"TTSPlayer|alloc memory failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_1106_112_2_18_3_3_41_97,"TTSPlayer|global data address is %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1110_112_2_18_3_3_41_98,"TTSPlayer|Init errcode=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1134_112_2_18_3_3_41_99,"TTSPlayer|entry JTTS_Stop audio_handle = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1137_112_2_18_3_3_41_100,"TTSPlayer|Status JTTS_Stop is %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1147_112_2_18_3_3_41_101,"TTSPlayer|JTTS_Stop g_playsize 0 g_bplayed True g_bWrite false")
TRACE_MSG(EJTTSPLAYER_AUDIO_1155_112_2_18_3_3_41_102,"TTSPlayer|Stop SynthText")
TRACE_MSG(EJTTSPLAYER_AUDIO_1168_112_2_18_3_3_41_103,"TTSPlayer|leave JTTS_Stop;")
TRACE_MSG(EJTTSPLAYER_AUDIO_1180_112_2_18_3_3_41_104,"TTSPlayer|JTTS_Resume audio_handle = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1189_112_2_18_3_3_41_105,"TTSPlayer|JTTS_Resume status=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1191_112_2_18_3_3_41_106,"TTSPlayer|Set ~Pause status")
TRACE_MSG(EJTTSPLAYER_AUDIO_1201_112_2_18_3_3_41_107,"TTSPlayer|JTTS_Resume status=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1203_112_2_18_3_3_41_108,"TTSPlayer|Leave JTTS_Resume")
TRACE_MSG(EJTTSPLAYER_AUDIO_1215_112_2_18_3_3_42_109,"TTSPlayer|Entry JTTS_Pause audio_handle = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1223_112_2_18_3_3_42_110,"TTSPlayer|begin JTTS_Pause status = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1226_112_2_18_3_3_42_111,"TTSPlayer|JTTS_Pause Set pause status")
TRACE_MSG(EJTTSPLAYER_AUDIO_1240_112_2_18_3_3_42_112,"TTSPlayer|Leave JTTS_Pause")
TRACE_MSG(EJTTSPLAYER_AUDIO_1266_112_2_18_3_3_42_113,"TTSPlayer|Entry JTTS_Play")
TRACE_MSG(EJTTSPLAYER_AUDIO_1270_112_2_18_3_3_42_114,"TTSPlayer|Invalid input text")
TRACE_MSG(EJTTSPLAYER_AUDIO_1284_112_2_18_3_3_42_115,"TTSPlayer|Get audio format failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_1286_112_2_18_3_3_42_116,"TTSPlayer|SampleRate=%d Bitpersample=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1334_112_2_18_3_3_42_117,"TTSPlayer|Entry JTTS_Play audio_handle = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1348_112_2_18_3_3_42_118,"TTSPlayer|Entry JTTS_Play g_playsize = 0; g_bplayed = FALSE; g_bWrite = TRUE")
TRACE_MSG(EJTTSPLAYER_AUDIO_1353_112_2_18_3_3_42_119,"TTSPlayer|tts handle =%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1385_112_2_18_3_3_42_120,"TTSPlayer|Ready to Call jtTTS_SynthesizeText tick = %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1387_112_2_18_3_3_42_121,"TTSPlayer|jtTTS_SynthesizeText dwErrCode =%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1394_112_2_18_3_3_42_122,"TTSPlayer|play wait for play finish")
TRACE_MSG(EJTTSPLAYER_AUDIO_1405_112_2_18_3_3_42_123,"TTSPlayer|play wait for play all buffer finish %d size %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1420_112_2_18_3_3_42_124,"TTSPlayer|Leave JTTS_Play")
TRACE_MSG(EJTTSPLAYER_AUDIO_1453_112_2_18_3_3_42_125,"TTSPlayer|file name len is %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1459_112_2_18_3_3_42_126,"Entry JTTS_PlayToFile")
TRACE_MSG(EJTTSPLAYER_AUDIO_1463_112_2_18_3_3_42_127,"TTSPlayer|NOT INIT")
TRACE_MSG(EJTTSPLAYER_AUDIO_1472_112_2_18_3_3_42_128,"TTSPlayer|Set Wave format failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_1478_112_2_18_3_3_42_129,"TTSPlayer|Get audio format failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_1480_112_2_18_3_3_42_130,"TTSPlayer|SampleRate=%d Bitpersample=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1487_112_2_18_3_3_42_131,"TTSPlayer|Can not open file")
TRACE_MSG(EJTTSPLAYER_AUDIO_1495_112_2_18_3_3_42_132,"TTSPlayer|Get audio format failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_1502_112_2_18_3_3_42_133,"TTSPlayer|Get audio format failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_1508_112_2_18_3_3_42_134,"TTSPlayer|Get audio format failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_1515_112_2_18_3_3_42_135,"TTSPlayer|Get audio format failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_1525_112_2_18_3_3_42_136,"TTSPlayer|Leave JTTS_Playtofile")
TRACE_MSG(EJTTSPLAYER_AUDIO_1589_112_2_18_3_3_42_137,"** JTTS_Event_Wait event_id=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1683_112_2_18_3_3_42_138,"JTTS_AudioNotifyCallback enter")
TRACE_MSG(EJTTSPLAYER_AUDIO_1694_112_2_18_3_3_42_139,"TTS_GetAudiohandle s_tts_audio_handle= %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1713_112_2_18_3_3_42_140,"** TTS_IsCommandStatusNone nStatus=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1731_112_2_18_3_3_43_141,"** TTS_IsPlayStatus nStatus=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1830_112_2_18_3_3_43_142,"TTSPlayer|ejTTSPlayer_Init in")
TRACE_MSG(EJTTSPLAYER_AUDIO_1834_112_2_18_3_3_43_143,"TTSPlayer|lSize=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1835_112_2_18_3_3_43_144,"TTSPlayer|GetExtBufSize|dwErrCode=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1840_112_2_18_3_3_43_145,"TTSPlayer|alloc memory failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_1843_112_2_18_3_3_43_146,"TTSPlayer|global data address is %d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1847_112_2_18_3_3_43_147,"TTSPlayer|Init errcode=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1851_112_2_18_3_3_43_148,"TTSPlayer|ejTTSPlayer_Init success")
TRACE_MSG(EJTTSPLAYER_AUDIO_1853_112_2_18_3_3_43_149,"TTSPlayer|Entry JTTS_PlayToFile")
TRACE_MSG(EJTTSPLAYER_AUDIO_1857_112_2_18_3_3_43_150,"TTSPlayer|TTSPlayer|NOT INIT")
TRACE_MSG(EJTTSPLAYER_AUDIO_1864_112_2_18_3_3_43_151,"TTSPlayer|Set Wave format failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_1870_112_2_18_3_3_43_152,"TTSPlayer|Get audio format failed")
TRACE_MSG(EJTTSPLAYER_AUDIO_1872_112_2_18_3_3_43_153,"TTSPlayer|SampleRate=%d Bitpersample=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1878_112_2_18_3_3_43_154,"TTSPlayer|Can not open file")
TRACE_MSG(EJTTSPLAYER_AUDIO_1885_112_2_18_3_3_43_155,"TTSPlayer|INPUTTXT_MODE=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1890_112_2_18_3_3_43_156,"TTSPlayer|OUTPUT_CALLBACK=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1894_112_2_18_3_3_43_157,"TTSPlayer|CALLBACK_USERDATA=%d")
TRACE_MSG(EJTTSPLAYER_AUDIO_1909_112_2_18_3_3_43_158,"TTSPlayer|Leave JTTS_Playtofile")
TRACE_MSG(TTS_IFLYTEK_API_307_112_2_18_3_3_44_159,"AUDIO_PcmStreamPlayCallback()  enter1 time:%u used =%d")
TRACE_MSG(TTS_IFLYTEK_API_310_112_2_18_3_3_44_160,"[MMITTS]: AUDIO_PcmStreamPlayCallback s_tts_stream_buffer ERROR!!!!!!!!!!!!!!!")
TRACE_MSG(TTS_IFLYTEK_API_327_112_2_18_3_3_44_161,"AUDIO_PcmStreamPlayCallback()  waiting time:%u used =%d")
TRACE_MSG(TTS_IFLYTEK_API_343_112_2_18_3_3_44_162,"AUDIO_PcmStreamPlayCallback()  enter2 time:%u used =%d")
TRACE_MSG(TTS_IFLYTEK_API_355_112_2_18_3_3_44_163,"AUDIO_PcmStreamPlayCallback()  g_tts_pcm_stream_is_over SET TRUE!!!")
TRACE_MSG(TTS_IFLYTEK_API_373_112_2_18_3_3_44_164,"KiTTS_PcmStreamOutputCallback()  enter time:%u counter =%d")
TRACE_MSG(TTS_IFLYTEK_API_381_112_2_18_3_3_44_165,"KiTTS_PcmStreamOutputCallback()  waiting time:%u empty =%d")
TRACE_MSG(TTS_IFLYTEK_API_396_112_2_18_3_3_44_166,"KiTTS_PcmStreamOutputCallback()  error exit time:%u counter =%d")
TRACE_MSG(TTS_IFLYTEK_API_404_112_2_18_3_3_44_167,"KiTTS_PcmStreamOutputCallback()  s_kitts_callback_count==TTS_CALLBACK_CHECKPOINT ")
TRACE_MSG(TTS_IFLYTEK_API_405_112_2_18_3_3_44_168,"KiTTS_PcmStreamOutputCallback() begin to play:%u used =%d")
TRACE_MSG(TTS_IFLYTEK_API_412_112_2_18_3_3_44_169,"KiTTS_PcmStreamOutputCallback()  enter time:%u used =%d")
TRACE_MSG(TTS_IFLYTEK_API_530_112_2_18_3_3_44_170,"TTS_GetQueueAvilable: %d")
TRACE_MSG(TTS_IFLYTEK_API_551_112_2_18_3_3_44_171,"[MMITTS]:MMITTS_SendSignaltoTask sig_ptr error!!!!!!!!!!!!!!!")
TRACE_MSG(TTS_IFLYTEK_API_562_112_2_18_3_3_44_172,"[MMITTS]:MMITTS_SendSignaltoTask SCI_SendSignal error!!!!!!!!!!!!!!!")
TRACE_MSG(TTS_IFLYTEK_API_576_112_2_18_3_3_44_173,"TTS_Task start")
TRACE_MSG(TTS_IFLYTEK_API_588_112_2_18_3_3_45_174,"TTS_Task :recieve MMITTS_MSG_EXIT")
TRACE_MSG(TTS_IFLYTEK_API_595_112_2_18_3_3_45_175,"TTS_Task :recieve MMITTS_MSG_START")
TRACE_MSG(TTS_IFLYTEK_API_618_112_2_18_3_3_45_176,"TTS_Task :MMIAPIFMM_CreateFile: handle=%u")
TRACE_MSG(TTS_IFLYTEK_API_644_112_2_18_3_3_45_177,"TTS_Task :recieve MMITTS_MSG_STOP")
TRACE_MSG(TTS_IFLYTEK_API_651_112_2_18_3_3_45_178,"TTS_Task :recieve MMITTS_MSG_OVER")
TRACE_MSG(TTS_IFLYTEK_API_683_112_2_18_3_3_45_179,"TTS_Task exit")
TRACE_MSG(TTS_IFLYTEK_API_711_112_2_18_3_3_45_180,"TTSAPI_SynthTextByTask enter")
TRACE_MSG(TTS_IFLYTEK_API_716_112_2_18_3_3_45_181,"TTSAPI_SynthTextByTask no res file exit")
TRACE_MSG(TTS_IFLYTEK_API_721_112_2_18_3_3_45_182,"TTSAPI_SynthTextByTask res file find")
TRACE_MSG(TTS_IFLYTEK_API_732_112_2_18_3_3_45_183,"TTSAPI_SynthTextByTask no memory exit")
TRACE_MSG(TTS_IFLYTEK_API_766_112_2_18_3_3_45_184,"TTSAPI_SynthTextByTask task created")
TRACE_MSG(TTS_IFLYTEK_API_773_112_2_18_3_3_45_185,"TTSAPI_SynthTextByTask task while")
TRACE_MSG(TTS_IFLYTEK_API_778_112_2_18_3_3_45_186,"TTSAPI_SynthTextByTask task quue exit")
TRACE_MSG(TTS_IFLYTEK_API_791_112_2_18_3_3_45_187,"TTSAPI_SynthTextByTask task exit")
TRACE_MSG(TTS_IFLYTEK_API_843_112_2_18_3_3_45_188," tts tts_resDataCathe  is not null")
TRACE_MSG(TTS_IFLYTEK_API_857_112_2_18_3_3_45_189," tts KiTTS_HeapBuffer   is not null")
TRACE_MSG(TTS_IFLYTEK_API_870_112_2_18_3_3_45_190,"tts s_tts_voice_data   is not null")
TRACE_MSG(TTS_IFLYTEK_API_884_112_2_18_3_3_45_191,"[MMITTS]:AllocTTS_StreamBuffer PNULL != s_tts_stream_buffer ERROR!!!!!!!!!!!!!")
TRACE_MSG(TTS_IFLYTEK_API_899_112_2_18_3_3_45_192,"[MMITTS]:FreeTTS_StreamBuffer")
TRACE_MSG(TTS_IFLYTEK_API_913_112_2_18_3_3_45_193," [MMITTS]: TTS_MemoryAlloc entered")
TRACE_MSG(TTS_IFLYTEK_API_916_112_2_18_3_3_45_194," [MMITTS]: MemoryAlloc HeapBuffer failed!!")
TRACE_MSG(TTS_IFLYTEK_API_922_112_2_18_3_3_45_195," [MMITTS]:  AllocTTS_VoiceData failed!!")
TRACE_MSG(TTS_IFLYTEK_API_928_112_2_18_3_3_45_196," [MMITTS]:  AllocTTS_StreamBuffer failed!!")
TRACE_MSG(TTS_IFLYTEK_API_935_112_2_18_3_3_45_197,"[MMITTS]:  AllocTTS_ResDataCathe failed!!")
TRACE_MSG(TTS_IFLYTEK_API_941_112_2_18_3_3_45_198,"[MMITTS]:KiTTS_HeapBuffer address = %x.")
TRACE_MSG(TTS_IFLYTEK_API_942_112_2_18_3_3_45_199,"[MMITTS]:KiTTS_voice data address = %x.")
TRACE_MSG(TTS_IFLYTEK_API_962_112_2_18_3_3_45_200," tts TTS_MemoryRelease entered")
TRACE_MSG(TTS_IFLYTEK_API_981_112_2_18_3_3_45_201,"MMITTS TTSAPI_StopSynthTextByTask entry time: %u")
TRACE_MSG(TTS_IFLYTEK_API_1022_112_2_18_3_3_45_202,"TTSAPI_StopSynthTextByTask exit time: %u")
TRACE_MSG(TTS_IFLYTEK_API_1031_112_2_18_3_3_46_203,"MMITTS TTSAPI_StopASynchTextByTask entry time: %u")
TRACE_MSG(TTS_IFLYTEK_API_1087_112_2_18_3_3_46_204,"[MMITTS]:ReadResCB FAIL!!!!!!!!")
TRACE_MSG(TTS_IFLYTEK_API_1119_112_2_18_3_3_46_205,"MMITTS: InputCB pParameter= %u, txtPar=%d ,copyLen=%d,pParam=%u,txtPar->position=%d")
TRACE_MSG(TTS_IFLYTEK_API_1141_112_2_18_3_3_46_206,"MMITTS: TTS_SynthText txtPar= %u, HeapBuffer=%d ,HEAP_BUFFER_SIZE=%d,ResData=%u")
TRACE_MSG(TTS_IFLYTEK_API_1222_112_2_18_3_3_46_207,"[MMITTS]:MMITTS_SynthText ivTTS_SynthStart")
TRACE_MSG(TTS_IFLYTEK_API_1235_112_2_18_3_3_46_208,"[MMITTS]:MMITTS_SynthText jump_value=%d")
TRACE_MSG(TTS_IFLYTEK_API_1259_112_2_18_3_3_46_209,"MMITTS, HandleTTSPlayOverTimerMsg entered")
TRACE_MSG(TTS_IFLYTEK_API_1262_112_2_18_3_3_46_210,"TTS_HandleTTSPlayOverTimerMsg:handle s_tts_playover_timer_id!")
TRACE_MSG(TTS_IFLYTEK_API_1299_112_2_18_3_3_46_211,"TTSAPI_PlayText() txt_len_in_byte= %d")
TRACE_MSG(TTS_IFLYTEK_API_1300_112_2_18_3_3_46_212,"TTSAPI_PlayText()  s_tts_text.tts_start_pos = %d,s_tts_text.tts_str_len = %d")
TRACE_MSG(TTS_IFLYTEK_API_1417_112_2_18_3_3_46_213,"TTSAPI_PlayText()  s_tts_text.tts_start_pos = %d,s_tts_text.tts_str_len = %d")
TRACE_MSG(TTS_IFLYTEK_API_1445_112_2_18_3_3_46_214,"TTSAPI_StopPlayTextByType ring_type=%d")
TRACE_MSG(TTS_IFLYTEK_API_1465_112_2_18_3_3_46_215,"TTSAPI_StopPlayText entry time: %u")
TRACE_MSG(TTS_IFLYTEK_API_1472_112_2_18_3_3_46_216,"TTSAPI_StopPlayText exit time: %u")
TRACE_MSG(TTS_IFLYTEK_API_1506_112_2_18_3_3_46_217,"TTSAPI_StopPlayTextByType ring_type=%d")
TRACE_MSG(TTS_IFLYTEK_API_1575_112_2_18_3_3_47_218,"MMIDCD create dcd main dir fail")
TRACE_MSG(TTS_IFLYTEK_API_1608_112_2_18_3_3_47_219,"TTS GetTTSResouceFileFullName error")
TRACE_MSG(TTS_IFLYTEK_API_1643_112_2_18_3_3_47_220,"TTS c: disk not exist resouce file")
TRACE_MSG(TTS_IFLYTEK_API_1670_112_2_18_3_3_47_221,"TTS %d disk not exist resouce file")
TRACE_MSG(TTS_IFLYTEK_API_1703_112_2_18_3_3_47_222,"TTS TTSAPI_checkResFile has passed ")
TRACE_MSG(TTS_IFLYTEK_API_1712_112_2_18_3_3_47_223,"TTS TTSAPI_checkResFile not find the resouce file  ")
TRACE_MSG(TTS_IFLYTEK_API_1732_112_2_18_3_3_47_224,"[MMITTS]:MMIAPITTS_PlayTextForControl ERROR!!!!!!!!!!!!")
TRACE_MSG(TTS_IFLYTEK_AUDIO_66_112_2_18_3_3_48_225,"enter funciton TTS_CreatePcmStreamHandle()")
TRACE_MSG(TTS_IFLYTEK_AUDIO_190_112_2_18_3_3_48_226,"enter funciton TTS_STOP_CALLBACK() g_tts_is_running=%d")
TRACE_MSG(TTS_IFLYTEK_AUDIO_228_112_2_18_3_3_48_227,"play_stream_id = %d, cur_stream_id = %d")
TRACE_MSG(TTS_IFLYTEK_AUDIO_232_112_2_18_3_3_48_228,"enter funciton VTTSAPI_PlayPcmStream()")
TRACE_MSG(TTS_IFLYTEK_AUDIO_236_112_2_18_3_3_48_229,"TTSAPI_PlayPcmStream() Handle error !!!!")
TRACE_MSG(TTS_IFLYTEK_AUDIO_295_112_2_18_3_3_48_230,"enter funciton TTS_StopAndClosePcmStreamHandle()")
TRACE_MSG(TTS_IFLYTEK_AUDIO_355_112_2_18_3_3_48_231,"[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x")
TRACE_MSG(TTS_IFLYTEK_AUDIO_360_112_2_18_3_3_48_232,"[MMISRV]: HandleNotify() report_ptr=0x%x")
TRACE_MSG(TTS_IFLYTEK_AUDIO_364_112_2_18_3_3_48_233,"[MMISRV]: HandleNotify(), report_ptr->report=%d")
TRACE_MSG(TTS_YOUNGTONG_API_390_112_2_18_3_3_49_234,"AUDIO_PcmStreamPlayCallback()  enter1 time:%u used =%d")
TRACE_MSG(TTS_YOUNGTONG_API_393_112_2_18_3_3_49_235,"[MMITTS]: AUDIO_PcmStreamPlayCallback s_tts_stream_buffer ERROR!!!!!!!!!!!!!!!")
TRACE_MSG(TTS_YOUNGTONG_API_410_112_2_18_3_3_49_236,"AUDIO_PcmStreamPlayCallback()  waiting time:%u used =%d")
TRACE_MSG(TTS_YOUNGTONG_API_426_112_2_18_3_3_49_237,"AUDIO_PcmStreamPlayCallback()  enter2 time:%u used =%d")
TRACE_MSG(TTS_YOUNGTONG_API_438_112_2_18_3_3_49_238,"AUDIO_PcmStreamPlayCallback()  g_tts_pcm_stream_is_over SET TRUE!!!")
TRACE_MSG(TTS_YOUNGTONG_API_460_112_2_18_3_3_49_239,"call back enter save file")
TRACE_MSG(TTS_YOUNGTONG_API_471_112_2_18_3_3_49_240,"create file")
TRACE_MSG(TTS_YOUNGTONG_API_481_112_2_18_3_3_49_241,"callback_function_for_yt_tts() enter")
TRACE_MSG(TTS_YOUNGTONG_API_498_112_2_18_3_3_49_242,"nSampleNumber:%d,write_size:%d")
TRACE_MSG(TTS_YOUNGTONG_API_511_112_2_18_3_3_49_243,"callback_function_for_yt_tts()  enter time:%u counter =%d dataLen= %d")
TRACE_MSG(TTS_YOUNGTONG_API_520_112_2_18_3_3_49_244,"callback_function_for_yt_tts()  cycle error exit time:%u counter =%d")
TRACE_MSG(TTS_YOUNGTONG_API_529_112_2_18_3_3_50_245,"callback_function_for_yt_tts()  waiting time:%u empty =%d")
TRACE_MSG(TTS_YOUNGTONG_API_537_112_2_18_3_3_50_246,"callback_function_for_yt_tts()  error exit time:%u counter =%d")
TRACE_MSG(TTS_YOUNGTONG_API_545_112_2_18_3_3_50_247,"callback_function_for_yt_tts()  s_kitts_callback_count==TTS_CALLBACK_CHECKPOINT ")
TRACE_MSG(TTS_YOUNGTONG_API_553_112_2_18_3_3_50_248,"callback_function_for_yt_tts()  enter time:%u used =%d")
TRACE_MSG(TTS_YOUNGTONG_API_559_112_2_18_3_3_50_249,"callback_function_for_yt_tts()  enter time:%u used =%d")
TRACE_MSG(TTS_YOUNGTONG_API_569_112_2_18_3_3_50_250,"YT LOG: %s")
TRACE_MSG(TTS_YOUNGTONG_API_782_112_2_18_3_3_50_251,"TTS_GetQueueAvilable: %d")
TRACE_MSG(TTS_YOUNGTONG_API_803_112_2_18_3_3_50_252,"[MMITTS]:MMITTS_SendSignaltoTask sig_ptr error!!!!!!!!!!!!!!!")
TRACE_MSG(TTS_YOUNGTONG_API_814_112_2_18_3_3_50_253,"[MMITTS]:MMITTS_SendSignaltoTask SCI_SendSignal error!!!!!!!!!!!!!!!")
TRACE_MSG(TTS_YOUNGTONG_API_830_112_2_18_3_3_50_254,"TTS_Task start")
TRACE_MSG(TTS_YOUNGTONG_API_842_112_2_18_3_3_50_255,"TTS_Task :recieve MMITTS_MSG_EXIT")
TRACE_MSG(TTS_YOUNGTONG_API_849_112_2_18_3_3_50_256,"TTS_Task :recieve MMITTS_MSG_START")
TRACE_MSG(TTS_YOUNGTONG_API_873_112_2_18_3_3_50_257,"TTS_Task :MMIAPIFMM_CreateFile: handle=%u")
TRACE_MSG(TTS_YOUNGTONG_API_904_112_2_18_3_3_50_258,"TTS_Task :recieve MMITTS_MSG_STOP")
TRACE_MSG(TTS_YOUNGTONG_API_911_112_2_18_3_3_50_259,"TTS_Task :recieve MMITTS_MSG_OVER")
TRACE_MSG(TTS_YOUNGTONG_API_943_112_2_18_3_3_50_260,"TTS_Task exit")
TRACE_MSG(TTS_YOUNGTONG_API_971_112_2_18_3_3_50_261,"TTSAPI_SynthTextByTask enter")
TRACE_MSG(TTS_YOUNGTONG_API_976_112_2_18_3_3_50_262,"TTSAPI_SynthTextByTask no res file exit")
TRACE_MSG(TTS_YOUNGTONG_API_981_112_2_18_3_3_50_263,"TTSAPI_SynthTextByTask res file find")
TRACE_MSG(TTS_YOUNGTONG_API_992_112_2_18_3_3_50_264,"TTSAPI_SynthTextByTask no memory exit")
TRACE_MSG(TTS_YOUNGTONG_API_1003_112_2_18_3_3_50_265,"TTSAPI_SynthTextByTask :MMIAPIFMM_CreateFile: handle=%u")
TRACE_MSG(TTS_YOUNGTONG_API_1022_112_2_18_3_3_51_266,"TTSAPI_SynthTextByTask :MMIAPIFMM_GetFileSize=%u")
TRACE_MSG(TTS_YOUNGTONG_API_1075_112_2_18_3_3_51_267,"TTSAPI_SynthTextByTask task created")
TRACE_MSG(TTS_YOUNGTONG_API_1082_112_2_18_3_3_51_268,"TTSAPI_SynthTextByTask task while")
TRACE_MSG(TTS_YOUNGTONG_API_1087_112_2_18_3_3_51_269,"TTSAPI_SynthTextByTask task quue exit")
TRACE_MSG(TTS_YOUNGTONG_API_1099_112_2_18_3_3_51_270,"TTSAPI_SynthTextByTask task exit")
TRACE_MSG(TTS_YOUNGTONG_API_1151_112_2_18_3_3_51_271," tts tts_resDataCathe  is not null")
TRACE_MSG(TTS_YOUNGTONG_API_1165_112_2_18_3_3_51_272," tts KiTTS_HeapBuffer   is not null")
TRACE_MSG(TTS_YOUNGTONG_API_1178_112_2_18_3_3_51_273,"tts s_tts_voice_data   is not null")
TRACE_MSG(TTS_YOUNGTONG_API_1192_112_2_18_3_3_51_274,"[MMITTS]:AllocTTS_StreamBuffer PNULL != s_tts_stream_buffer ERROR!!!!!!!!!!!!!")
TRACE_MSG(TTS_YOUNGTONG_API_1207_112_2_18_3_3_51_275,"[MMITTS]:FreeTTS_StreamBuffer")
TRACE_MSG(TTS_YOUNGTONG_API_1221_112_2_18_3_3_51_276," [MMITTS]: TTS_MemoryAlloc entered")
TRACE_MSG(TTS_YOUNGTONG_API_1224_112_2_18_3_3_51_277," [MMITTS]: MemoryAlloc HeapBuffer failed!!")
TRACE_MSG(TTS_YOUNGTONG_API_1230_112_2_18_3_3_51_278," [MMITTS]:  AllocTTS_VoiceData failed!!")
TRACE_MSG(TTS_YOUNGTONG_API_1236_112_2_18_3_3_51_279," [MMITTS]:  AllocTTS_StreamBuffer failed!!")
TRACE_MSG(TTS_YOUNGTONG_API_1244_112_2_18_3_3_51_280,"[MMITTS]:  AllocTTS_ResDataCathe failed!!")
TRACE_MSG(TTS_YOUNGTONG_API_1251_112_2_18_3_3_51_281,"[MMITTS]:KiTTS_HeapBuffer address = %x.")
TRACE_MSG(TTS_YOUNGTONG_API_1252_112_2_18_3_3_51_282,"[MMITTS]:KiTTS_voice data address = %x.")
TRACE_MSG(TTS_YOUNGTONG_API_1274_112_2_18_3_3_51_283," tts TTS_MemoryRelease entered")
TRACE_MSG(TTS_YOUNGTONG_API_1295_112_2_18_3_3_51_284,"MMITTS TTSAPI_StopSynthTextByTask entry time: %u")
TRACE_MSG(TTS_YOUNGTONG_API_1336_112_2_18_3_3_51_285,"TTSAPI_StopSynthTextByTask exit time: %u")
TRACE_MSG(TTS_YOUNGTONG_API_1345_112_2_18_3_3_51_286,"MMITTS TTSAPI_StopASynchTextByTask entry time: %u")
TRACE_MSG(TTS_YOUNGTONG_API_1401_112_2_18_3_3_51_287,"[MMITTS]:ReadResCB FAIL!!!!!!!!")
TRACE_MSG(TTS_YOUNGTONG_API_1433_112_2_18_3_3_51_288,"MMITTS: InputCB pParameter= %u, txtPar=%d ,copyLen=%d,pParam=%u,txtPar->position=%d")
TRACE_MSG(TTS_YOUNGTONG_API_1455_112_2_18_3_3_51_289,"TTS_SynthText enter")
TRACE_MSG(TTS_YOUNGTONG_API_1461_112_2_18_3_3_51_290,"TTS_SynthText set mem")
TRACE_MSG(TTS_YOUNGTONG_API_1465_112_2_18_3_3_51_291,"TTS_SynthText yt init")
TRACE_MSG(TTS_YOUNGTONG_API_1480_112_2_18_3_3_51_292,"hidi length %d")
TRACE_MSG(TTS_YOUNGTONG_API_1486_112_2_18_3_3_51_293,"hidi times %d,time:%u")
TRACE_MSG(TTS_YOUNGTONG_API_1494_112_2_18_3_3_51_294,"hidi stop after time:%u")
TRACE_MSG(TTS_YOUNGTONG_API_1500_112_2_18_3_3_51_295,"1:%d,2:%d,3:%d,4:%d")
TRACE_MSG(TTS_YOUNGTONG_API_1523_112_2_18_3_3_52_296,"TTS_SynthText enter")
TRACE_MSG(TTS_YOUNGTONG_API_1526_112_2_18_3_3_52_297,"TTS_SynthText set mem")
TRACE_MSG(TTS_YOUNGTONG_API_1534_112_2_18_3_3_52_298,"string ptr before ini:%x")
TRACE_MSG(TTS_YOUNGTONG_API_1538_112_2_18_3_3_52_299,"string ptr after ini:%x")
TRACE_MSG(TTS_YOUNGTONG_API_1540_112_2_18_3_3_52_300,"TTS_SynthText yt init")
TRACE_MSG(TTS_YOUNGTONG_API_1554_112_2_18_3_3_52_301,"hidi length %d")
TRACE_MSG(TTS_YOUNGTONG_API_1556_112_2_18_3_3_52_302,"before yt_tts_input_text_utf16")
TRACE_MSG(TTS_YOUNGTONG_API_1563_112_2_18_3_3_52_303,"after yt_tts_input_text_utf16")
TRACE_MSG(TTS_YOUNGTONG_API_1570_112_2_18_3_3_52_304,"hidi times %d,time:%u")
TRACE_MSG(TTS_YOUNGTONG_API_1578_112_2_18_3_3_52_305,"hidi stop after time:%u")
TRACE_MSG(TTS_YOUNGTONG_API_1584_112_2_18_3_3_52_306,"1:%d,2:%d,3:%d,4:%d")
TRACE_MSG(TTS_YOUNGTONG_API_1590_112_2_18_3_3_52_307,"close file handle:%x")
TRACE_MSG(TTS_YOUNGTONG_API_1595_112_2_18_3_3_52_308,"SAVE_PCM_STREAM close file")
TRACE_MSG(TTS_YOUNGTONG_API_1603_112_2_18_3_3_52_309,"[MMITTS]:MMITTS_SynthText ivTTS_SynthStart")
TRACE_MSG(TTS_YOUNGTONG_API_1615_112_2_18_3_3_52_310,"[MMITTS]:MMITTS_SynthText jump_value=%d")
TRACE_MSG(TTS_YOUNGTONG_API_1631_112_2_18_3_3_52_311,"MMITTS, HandleTTSPlayOverTimerMsg entered")
TRACE_MSG(TTS_YOUNGTONG_API_1634_112_2_18_3_3_52_312,"TTS_HandleTTSPlayOverTimerMsg:handle s_tts_playover_timer_id!")
TRACE_MSG(TTS_YOUNGTONG_API_1671_112_2_18_3_3_52_313,"TTSAPI_PlayText() txt_len_in_byte= %d")
TRACE_MSG(TTS_YOUNGTONG_API_1672_112_2_18_3_3_52_314,"TTSAPI_PlayText()  s_tts_text.tts_start_pos = %d,s_tts_text.tts_str_len = %d")
TRACE_MSG(TTS_YOUNGTONG_API_1766_112_2_18_3_3_52_315,"s_tts_text.str_arr:%x")
TRACE_MSG(TTS_YOUNGTONG_API_1790_112_2_18_3_3_52_316,"TTSAPI_PlayText()  s_tts_text.tts_start_pos = %d,s_tts_text.tts_str_len = %d")
TRACE_MSG(TTS_YOUNGTONG_API_1818_112_2_18_3_3_52_317,"TTSAPI_StopPlayTextByType ring_type=%d")
TRACE_MSG(TTS_YOUNGTONG_API_1838_112_2_18_3_3_52_318,"TTSAPI_StopPlayText entry time: %u")
TRACE_MSG(TTS_YOUNGTONG_API_1845_112_2_18_3_3_52_319,"TTSAPI_StopPlayText exit time: %u")
TRACE_MSG(TTS_YOUNGTONG_API_1879_112_2_18_3_3_52_320,"TTSAPI_StopPlayTextByType ring_type=%d")
TRACE_MSG(TTS_YOUNGTONG_API_1948_112_2_18_3_3_52_321,"MMIDCD create dcd main dir fail")
TRACE_MSG(TTS_YOUNGTONG_API_1982_112_2_18_3_3_52_322,"[MMITTS]:GetTTSResouceFileFullName error!!!!!!!!!!!!")
TRACE_MSG(TTS_YOUNGTONG_API_1999_112_2_18_3_3_52_323,"TTS GetTTSResouceFileFullName entry")
TRACE_MSG(TTS_YOUNGTONG_API_2032_112_2_18_3_3_53_324,"TTS GetTTSResouceFileFullName error")
TRACE_MSG(TTS_YOUNGTONG_API_2035_112_2_18_3_3_53_325,"TTS GetTTSResouceFileFullName + full_path_name_ptr:%s")
TRACE_MSG(TTS_YOUNGTONG_API_2072_112_2_18_3_3_53_326,"TTS c: disk not exist resouce file")
TRACE_MSG(TTS_YOUNGTONG_API_2099_112_2_18_3_3_53_327,"TTS %d disk not exist resouce file")
TRACE_MSG(TTS_YOUNGTONG_API_2131_112_2_18_3_3_53_328,"TTS TTSAPI_checkResFile not find the resouce file  ")
TRACE_MSG(TTS_YOUNGTONG_API_2153_112_2_18_3_3_53_329,"TTS_copyFileInfo tts_language_type:%x")
TRACE_MSG(TTS_YOUNGTONG_API_2239_112_2_18_3_3_53_330,"TTS_copyFileInfo s_mmitts_res_file_name_1:%s")
TRACE_MSG(TTS_YOUNGTONG_API_2240_112_2_18_3_3_53_331,"TTS_copyFileInfo s_mmitts_default_file_path_1:%s")
TRACE_MSG(TTS_YOUNGTONG_API_2241_112_2_18_3_3_53_332,"TTS_copyFileInfo s_char_mmitts_default_file_path_1:%s")
TRACE_MSG(TTS_YOUNGTONG_API_2243_112_2_18_3_3_53_333,"TTS_copyFileInfo s_mmitts_res_file_name_2:%s")
TRACE_MSG(TTS_YOUNGTONG_API_2244_112_2_18_3_3_53_334,"TTS_copyFileInfo s_mmitts_default_file_path_2:%s")
TRACE_MSG(TTS_YOUNGTONG_API_2245_112_2_18_3_3_53_335,"TTS_copyFileInfo s_char_mmitts_default_file_path_2:%s")
TRACE_MSG(TTS_YOUNGTONG_API_2350_112_2_18_3_3_53_336,"TTSAPI_checkResFile: TTS Udisk exist resouce file")
TRACE_MSG(TTS_YOUNGTONG_API_2363_112_2_18_3_3_53_337,"TTSAPI_checkResFile: TTS Udisk not exist resouce file")
TRACE_MSG(TTS_YOUNGTONG_API_2385_112_2_18_3_3_53_338,"TTSAPI_checkResFile: TTS SDCard exist resouce file")
TRACE_MSG(TTS_YOUNGTONG_API_2398_112_2_18_3_3_53_339,"TTS SDCard not exist resouce file")
TRACE_MSG(TTS_YOUNGTONG_API_2420_112_2_18_3_3_53_340,"[MMITTS]:MMIAPITTS_PlayTextForControl ERROR!!!!!!!!!!!!")
TRACE_MSG(TTS_YOUNGTONG_AUDIO_71_112_2_18_3_3_54_341,"enter funciton TTS_CreatePcmStreamHandle()")
TRACE_MSG(TTS_YOUNGTONG_AUDIO_193_112_2_18_3_3_54_342,"enter funciton TTS_STOP_CALLBACK() tts_is_running=%d")
TRACE_MSG(TTS_YOUNGTONG_AUDIO_230_112_2_18_3_3_55_343,"play_stream_id = %d, cur_stream_id = %d")
TRACE_MSG(TTS_YOUNGTONG_AUDIO_234_112_2_18_3_3_55_344,"enter funciton TTSAPI_PlayPcmStream()")
TRACE_MSG(TTS_YOUNGTONG_AUDIO_238_112_2_18_3_3_55_345,"TTSAPI_PlayPcmStream() Handle error !!!!")
TRACE_MSG(TTS_YOUNGTONG_AUDIO_297_112_2_18_3_3_55_346,"enter funciton TTS_StopAndClosePcmStreamHandle()")
TRACE_MSG(TTS_YOUNGTONG_AUDIO_359_112_2_18_3_3_55_347,"[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x")
TRACE_MSG(TTS_YOUNGTONG_AUDIO_364_112_2_18_3_3_55_348,"[MMISRV]: HandleNotify() report_ptr=0x%x")
TRACE_MSG(TTS_YOUNGTONG_AUDIO_368_112_2_18_3_3_55_349,"[MMISRV]: HandleNotify(), report_ptr->report=%d")
END_TRACE_MAP(MMI_APP_TTS_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_TTS_TRC_H_

