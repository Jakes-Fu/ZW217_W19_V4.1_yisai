/******************************************************************************
 ** File Name:      ms_ref_aud_config_trc.h                                         *
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
//trace_id:32
#ifndef _MS_REF_AUD_CONFIG_TRC_H_
#define _MS_REF_AUD_CONFIG_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define ACM_CLASS_LIB_78_112_2_18_1_1_59_0 "acm_class_lib.c:ACM_Policy_DifferentLevelCollide:error:NULL == ptCurAcmClassObject"
#define ACM_CLASS_LIB_101_112_2_18_1_1_59_1 "acm_class_lib.c:ACM_Policy_DifferentLevelCollide:error:NULL == ptCurAcmClassObject"
#define ACM_CLASS_LIB_120_112_2_18_1_1_59_2 "acm_class_lib.c:ACM_Policy_DifferentLevelCollide:error:AUDIO_NO_ERROR != AUDIO_CM_SetNeedStopFlag"
#define ACM_CLASS_LIB_181_112_2_18_1_1_59_3 "acm_class_lib.c:ACM_Policy_Exclusive:error:AUDIO_NO_ERROR != AUDIO_CM_SetNeedStopFlag"
#define ACM_CLASS_LIB_255_112_2_18_1_1_59_4 "acm_class_lib.c:ACM_Policy_SinglePlay:error:NULL == ptCurAcmClassObject"
#define ACM_CLASS_LIB_278_112_2_18_1_1_59_5 "acm_class_lib.c:ACM_Policy_Exclusive:error:AUDIO_NO_ERROR != AUDIO_CM_SetNeedStopFlag"
#define ACM_CLASS_LIB_296_112_2_18_1_1_59_6 "acm_class_lib.c:ACM_Policy_SinglePlay:error:AUDIO_NO_ERROR != AUDIO_CM_SetNeedStopFlag"
#define AUDIO_APP_89_112_2_18_1_1_59_7 "[AUDIO_EnablePcm] be enable buf:0x%x."
#define AUDIO_APP_94_112_2_18_1_1_59_8 "[AUDIO_EnablePcm] be enable handle:0x%x, 0x%x."
#define AUDIO_APP_100_112_2_18_1_1_59_9 "[AUDIO_EnablePcm]ui_pcm_buffer alloc failed."
#define AUDIO_APP_109_112_2_18_1_2_0_10 "[AUDIO_EnablePcm]h_data_play create failed."
#define AUDIO_APP_118_112_2_18_1_2_0_11 "[AUDIO_EnablePcm]h_data_record create failed."
#define AUDIO_APP_127_112_2_18_1_2_0_12 "[AUDIO_EnablePcm]h_data_play play failed. result=0x%x."
#define AUDIO_APP_136_112_2_18_1_2_0_13 "[AUDIO_EnablePcm]h_data_record play failed. result=0x%x."
#define AUDIO_APP_148_112_2_18_1_2_0_14 "[AUDIO_EnablePcm]h_data_play stop failed. result=0x%x."
#define AUDIO_APP_154_112_2_18_1_2_0_15 "[AUDIO_EnablePcm]h_data_record stop failed. result=0x%x."
#define AUDIO_APP_160_112_2_18_1_2_0_16 "[AUDIO_EnablePcm]h_data_play close failed. result=0x%x."
#define AUDIO_APP_166_112_2_18_1_2_0_17 "[AUDIO_EnablePcm]h_data_record close failed. result=0x%x."
#define AUDIO_CONFIG_1396_112_2_18_1_2_3_18 "[AUD_EnableMicSample]: is_enable = %d"
#define AUDIO_CONFIG_1407_112_2_18_1_2_3_19 "[AUD_EnableMicSample]: Have open!!"
#define AUDIO_CONFIG_1415_112_2_18_1_2_3_20 "[AUD_EnableMicSample]: Have close!!"
#define AUDIO_CONFIG_1424_112_2_18_1_2_3_21 "[AUD_EnableMicSample]: mic default sample rate is=%d"
#define AUDIO_CONFIG_1436_112_2_18_1_2_3_22 "[AUD_EnableMicSample]: mic default sample volume is=%d"
#define AUDIO_CONFIG_1700_112_2_18_1_2_3_23 "[AUD_EnableMicSample]: is_enable = %d"
#define AUDIO_CONFIG_1706_112_2_18_1_2_3_24 "[AUD_EnableMicSample]: Have open!!"
#define AUDIO_CONFIG_1714_112_2_18_1_2_3_25 "[AUD_EnableMicSample]: Have close!!"
#define AUDIO_CONFIG_1723_112_2_18_1_2_3_26 "[AUD_EnableMicSample]: mic default sample rate is=%d"
#define AUDIO_CONFIG_1735_112_2_18_1_2_3_27 "[AUD_EnableMicSample]: mic default sample volume is=%d"
#define AUDIO_CONFIG_1788_112_2_18_1_2_3_28 "AUD_SetEngineeringParameter: parameter_type = %d"
#define AUDIO_CONFIG_1805_112_2_18_1_2_3_29 "AUD_GetEngineeringParameter: parameter_type = %d"
#define AUDIO_CONFIG_1820_112_2_18_1_2_3_30 "AUD_SwitchTestMode: is_test_mode = %d"
#define AUDIO_CONFIG_1839_112_2_18_1_2_3_31 "AUD_EnableVoiceLoopback: is_enable = %d, delay_time = %d"
#define AUDIO_CONFIG_1849_112_2_18_1_2_3_32 "AUD_EnableVoiceLoopback play failed:%d."
#define AUDIO_CONFIG_1859_112_2_18_1_2_3_33 "AUD_EnableVoiceLoopback stop failed:%d."
#define AUDIO_CONFIG_1994_112_2_18_1_2_4_34 "AUD_GetAudioDeviceModeName can't get right name, type:%d."
#define AUDIO_CONFIG_2011_112_2_18_1_2_4_35 "audio_prod.c,[AUD_SetBkMusicVolume]:param type:0x%08x,uiUplinkLevel:0x%08x,uiDownlinkLevel:0x%08x"
#define AUDIO_CONFIG_2057_112_2_18_1_2_4_36 "AUD_SetDG_Express hAUDPROCExp: dac_gain = %d"
#define AUDIO_CONFIG_2073_112_2_18_1_2_4_37 "AUD_SetDG_Express hAUDPROCExp: dac_gain = %d"
#define AUDIO_CONFIG_2080_112_2_18_1_2_4_38 "AUD_SetDgGain hAGCExp :dac_gain = %d"
#define AUDIO_CONFIG_2302_112_2_18_1_2_4_39 "Voice_Dump_Pcm_Data_Callback index:%d, %d"
#define AUDIO_CONFIG_2325_112_2_18_1_2_4_40 "Voice_Dump_Pcm_Data_Callback data:%02d,%02d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d"
#define AUDIO_CONFIG_2364_112_2_18_1_2_4_41 "AUD_EnableVoiceProcessConfig is playing and needs not to play again."
#define AUDIO_CONFIG_2386_112_2_18_1_2_4_42 "AUD_EnableVoiceProcessConfig SFS_CreateFile failed."
#define AUDIO_CONFIG_2408_112_2_18_1_2_5_43 "\"aud_result=%d, ext_result=%d"
#define AUDIO_CONFIG_2418_112_2_18_1_2_5_44 "AUD_EnableVoiceProcessConfig is stopped and needs not to stop again."
#define AUDIO_CONFIG_2428_112_2_18_1_2_5_45 "\"stop aud_result=%d, ext_result=%d"
#define AUDIO_CONFIG_2443_112_2_18_1_2_5_46 "AUD_EnableVoiceProcessConfig stop. data size should save:%d(bytes), index:%d."
#define AUDIO_CONFIG_2461_112_2_18_1_2_5_47 "AUD_EnableVoiceCodec: is_enable = %d"
#define AUDIO_EXTPCM_141_112_2_18_1_2_5_48 "InfoNES_Sound_threadx.cpp: NES_Play"
#define AUDIO_EXTPCM_160_112_2_18_1_2_5_49 "InfoNES_Sound_threadx.cpp: NES_Stop"
#define AUDIO_EXTPCM_226_112_2_18_1_2_5_50 "[AUDIO_EXTPCM]:AudioEXTPCMNotifyCallback:enter:hAudio = 0x%X, notify_info = 0x%X, affix_info = %ld"
#define AUDIO_EXTPCM_238_112_2_18_1_2_6_51 "\rExtPCM_SoundInit"
#define AUDIO_EXTPCM_286_112_2_18_1_2_6_52 "\rExtPCM_SoundExit"
#define AUDIO_EXTPCM_309_112_2_18_1_2_6_53 "\rExtPCM_SoundOpen samples %d, sample_rate %d"
#define AUDIO_EXTPCM_334_112_2_18_1_2_6_54 "\rExtPCM_SoundClose"
#define AUDIO_EXTPCM_350_112_2_18_1_2_6_55 "ExtPCM_SoundOutput error, SCI_NULL == buff_ptr."
#define AUDIO_EXTPCM_374_112_2_18_1_2_6_56 "ExtPCM_SoundOutputStereo error, SCI_NULL == lbuff_ptr."
#define AUDIO_EXTPCM_379_112_2_18_1_2_6_57 "ExtPCM_SoundOutputStereo error, SCI_NULL == rbuff_ptr."
#define AUDIO_MTV_80_112_2_18_1_2_6_58 "audio_mtv.c,[MTV_Play]! ptAudioObj:0x%08x,uiOffset:0x%08x"
#define AUDIO_MTV_99_112_2_18_1_2_6_59 "InfoNES_Sound_threadx.cpp: NES_Stop"
#define AUDIO_MTV_176_112_2_18_1_2_6_60 "[AUDIO_MTV]:AudioMTVNotifyCallback:enter:hAudio = 0x%X, notify_info = 0x%X, affix_info = %ld"
#define AUDIO_MTV_239_112_2_18_1_2_7_61 "audio_mtv.c,[MTV_SoundInit] samples:%d,sample_rate:%d"
#define AUDIO_MTV_244_112_2_18_1_2_7_62 "audio_mtv.c,[MTV_SoundInit] error 0==sample_rate."
#define AUDIO_MTV_293_112_2_18_1_2_7_63 "audio_mtv.c,[MTV_SoundExit]hMtvRing:0x%08x,puiTrackBuffer:0x%08x!"
#define AUDIO_MTV_329_112_2_18_1_2_7_64 "MTV_SoundOpen error 0==sample_rate."
#define AUDIO_MTV_332_112_2_18_1_2_7_65 "audio_mtv.c,[MTV_SoundOpen] samples %d, sample_rate %d"
#define AUDIO_MTV_355_112_2_18_1_2_7_66 "audio_mtv.c,[MTV_SoundClose]hMtvRing:0x%08x!"
#define AUDIO_MTV_368_112_2_18_1_2_7_67 "audio_mtv.c,[MTV_SoundClose] hMtvRing == INVALID_HANDLE!maybe mtv is stopped already!"
#define AUDIO_MTV_385_112_2_18_1_2_7_68 "MTV_SoundOutput SCI_NULL==buff_ptr"
#define AUDIO_MTV_410_112_2_18_1_2_7_69 "MTV_SoundOutputStereo 0x%x, 0x%x."
#define BKMUSIC_AS_142_112_2_18_1_2_8_70 "bkmusic_as.c,[BKMUSIC_SetGain]: the state and gain value! s_is_background_music_on:%d,eParamType:0x%08x,uiUplinkGain:0x%08x,uiDownlinkGain:0x%08x"
#define BKMUSIC_AS_147_112_2_18_1_2_8_71 "bkmusic_as.c,[BKMUSIC_SetGain]:set the gains down!"
#define BKMUSIC_AS_169_112_2_18_1_2_8_72 "BKMUSIC_Play input para error. uiSampleRate:0."
#define BKMUSIC_AS_177_112_2_18_1_2_8_73 "bkmusic_as.c,[BKMUSIC_Play]:call LAYER1_StartMagicVoiceFunc!s_magic_gain_type:0x%08x,s_magic_uplink_gain:0x%08x,s_magic_downlink_gain:0x%08x"
#define BKMUSIC_AS_186_112_2_18_1_2_8_74 "bkmusic_as.c,[BKMUSIC_Play]:call LAYER1_StartMixVoiceFunc!s_magic_gain_type:0x%08x,s_magic_uplink_gain:0x%08x,s_magic_downlink_gain:0x%08x"
#define BKMUSIC_AS_211_112_2_18_1_2_8_75 "bkmusic_as.c,[BKMUSIC_Play]:already open!"
#define BKMUSIC_AS_232_112_2_18_1_2_8_76 "bkmusic_as.c,[BKMUSIC_Stop]:call LAYER1_StopMixVoiceFunc!"
#define BKMUSIC_AS_237_112_2_18_1_2_8_77 "bkmusic_as.c,[BKMUSIC_Stop]:already off!"
#define BKMUSIC_AS_257_112_2_18_1_2_8_78 "bkmusic_as.c,[BKMUSIC_Pause]:call LAYER1_StopMixVoiceFunc!"
#define BKMUSIC_AS_262_112_2_18_1_2_8_79 "bkmusic_as.c,[BKMUSIC_Pause]:already off!"
#define BKMUSIC_AS_275_112_2_18_1_2_8_80 "BKMUSIC_Resume input para error. uiSampleRate:0."
#define BKMUSIC_AS_284_112_2_18_1_2_8_81 "bkmusic_as.c,[BKMUSIC_Resume]:call LAYER1_StartMagicVoiceFunc!s_magic_gain_type:0x%08x,s_magic_uplink_gain:0x%08x,s_magic_downlink_gain:0x%08x"
#define BKMUSIC_AS_294_112_2_18_1_2_8_82 "bkmusic_as.c,[BKMUSIC_Resume]:call LAYER1_StartMixVoiceFunc!s_magic_gain_type:0x%08x,s_magic_uplink_gain:0x%08x,s_magic_downlink_gain:0x%08x"
#define BKMUSIC_AS_309_112_2_18_1_2_8_83 "bkmusic_as.c,[BKMUSIC_Resume]:already open!"
#define BKMUSIC_AS_365_112_2_18_1_2_8_84 "bkmusic_as.c,[BKMUSIC_ReadDataFromBuffer]: pcm_data_buf is NULL"
#define BKMUSIC_AS_372_112_2_18_1_2_8_85 "bkmusic_as.c,[BKMUSIC_ReadDataFromBuffer]: The num of pcm data is less than 160 words!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_AUD_CONFIG_TRC)
TRACE_MSG(ACM_CLASS_LIB_78_112_2_18_1_1_59_0,"acm_class_lib.c:ACM_Policy_DifferentLevelCollide:error:NULL == ptCurAcmClassObject")
TRACE_MSG(ACM_CLASS_LIB_101_112_2_18_1_1_59_1,"acm_class_lib.c:ACM_Policy_DifferentLevelCollide:error:NULL == ptCurAcmClassObject")
TRACE_MSG(ACM_CLASS_LIB_120_112_2_18_1_1_59_2,"acm_class_lib.c:ACM_Policy_DifferentLevelCollide:error:AUDIO_NO_ERROR != AUDIO_CM_SetNeedStopFlag")
TRACE_MSG(ACM_CLASS_LIB_181_112_2_18_1_1_59_3,"acm_class_lib.c:ACM_Policy_Exclusive:error:AUDIO_NO_ERROR != AUDIO_CM_SetNeedStopFlag")
TRACE_MSG(ACM_CLASS_LIB_255_112_2_18_1_1_59_4,"acm_class_lib.c:ACM_Policy_SinglePlay:error:NULL == ptCurAcmClassObject")
TRACE_MSG(ACM_CLASS_LIB_278_112_2_18_1_1_59_5,"acm_class_lib.c:ACM_Policy_Exclusive:error:AUDIO_NO_ERROR != AUDIO_CM_SetNeedStopFlag")
TRACE_MSG(ACM_CLASS_LIB_296_112_2_18_1_1_59_6,"acm_class_lib.c:ACM_Policy_SinglePlay:error:AUDIO_NO_ERROR != AUDIO_CM_SetNeedStopFlag")
TRACE_MSG(AUDIO_APP_89_112_2_18_1_1_59_7,"[AUDIO_EnablePcm] be enable buf:0x%x.")
TRACE_MSG(AUDIO_APP_94_112_2_18_1_1_59_8,"[AUDIO_EnablePcm] be enable handle:0x%x, 0x%x.")
TRACE_MSG(AUDIO_APP_100_112_2_18_1_1_59_9,"[AUDIO_EnablePcm]ui_pcm_buffer alloc failed.")
TRACE_MSG(AUDIO_APP_109_112_2_18_1_2_0_10,"[AUDIO_EnablePcm]h_data_play create failed.")
TRACE_MSG(AUDIO_APP_118_112_2_18_1_2_0_11,"[AUDIO_EnablePcm]h_data_record create failed.")
TRACE_MSG(AUDIO_APP_127_112_2_18_1_2_0_12,"[AUDIO_EnablePcm]h_data_play play failed. result=0x%x.")
TRACE_MSG(AUDIO_APP_136_112_2_18_1_2_0_13,"[AUDIO_EnablePcm]h_data_record play failed. result=0x%x.")
TRACE_MSG(AUDIO_APP_148_112_2_18_1_2_0_14,"[AUDIO_EnablePcm]h_data_play stop failed. result=0x%x.")
TRACE_MSG(AUDIO_APP_154_112_2_18_1_2_0_15,"[AUDIO_EnablePcm]h_data_record stop failed. result=0x%x.")
TRACE_MSG(AUDIO_APP_160_112_2_18_1_2_0_16,"[AUDIO_EnablePcm]h_data_play close failed. result=0x%x.")
TRACE_MSG(AUDIO_APP_166_112_2_18_1_2_0_17,"[AUDIO_EnablePcm]h_data_record close failed. result=0x%x.")
TRACE_MSG(AUDIO_CONFIG_1396_112_2_18_1_2_3_18,"[AUD_EnableMicSample]: is_enable = %d")
TRACE_MSG(AUDIO_CONFIG_1407_112_2_18_1_2_3_19,"[AUD_EnableMicSample]: Have open!!")
TRACE_MSG(AUDIO_CONFIG_1415_112_2_18_1_2_3_20,"[AUD_EnableMicSample]: Have close!!")
TRACE_MSG(AUDIO_CONFIG_1424_112_2_18_1_2_3_21,"[AUD_EnableMicSample]: mic default sample rate is=%d")
TRACE_MSG(AUDIO_CONFIG_1436_112_2_18_1_2_3_22,"[AUD_EnableMicSample]: mic default sample volume is=%d")
TRACE_MSG(AUDIO_CONFIG_1700_112_2_18_1_2_3_23,"[AUD_EnableMicSample]: is_enable = %d")
TRACE_MSG(AUDIO_CONFIG_1706_112_2_18_1_2_3_24,"[AUD_EnableMicSample]: Have open!!")
TRACE_MSG(AUDIO_CONFIG_1714_112_2_18_1_2_3_25,"[AUD_EnableMicSample]: Have close!!")
TRACE_MSG(AUDIO_CONFIG_1723_112_2_18_1_2_3_26,"[AUD_EnableMicSample]: mic default sample rate is=%d")
TRACE_MSG(AUDIO_CONFIG_1735_112_2_18_1_2_3_27,"[AUD_EnableMicSample]: mic default sample volume is=%d")
TRACE_MSG(AUDIO_CONFIG_1788_112_2_18_1_2_3_28,"AUD_SetEngineeringParameter: parameter_type = %d")
TRACE_MSG(AUDIO_CONFIG_1805_112_2_18_1_2_3_29,"AUD_GetEngineeringParameter: parameter_type = %d")
TRACE_MSG(AUDIO_CONFIG_1820_112_2_18_1_2_3_30,"AUD_SwitchTestMode: is_test_mode = %d")
TRACE_MSG(AUDIO_CONFIG_1839_112_2_18_1_2_3_31,"AUD_EnableVoiceLoopback: is_enable = %d, delay_time = %d")
TRACE_MSG(AUDIO_CONFIG_1849_112_2_18_1_2_3_32,"AUD_EnableVoiceLoopback play failed:%d.")
TRACE_MSG(AUDIO_CONFIG_1859_112_2_18_1_2_3_33,"AUD_EnableVoiceLoopback stop failed:%d.")
TRACE_MSG(AUDIO_CONFIG_1994_112_2_18_1_2_4_34,"AUD_GetAudioDeviceModeName can't get right name, type:%d.")
TRACE_MSG(AUDIO_CONFIG_2011_112_2_18_1_2_4_35,"audio_prod.c,[AUD_SetBkMusicVolume]:param type:0x%08x,uiUplinkLevel:0x%08x,uiDownlinkLevel:0x%08x")
TRACE_MSG(AUDIO_CONFIG_2057_112_2_18_1_2_4_36,"AUD_SetDG_Express hAUDPROCExp: dac_gain = %d")
TRACE_MSG(AUDIO_CONFIG_2073_112_2_18_1_2_4_37,"AUD_SetDG_Express hAUDPROCExp: dac_gain = %d")
TRACE_MSG(AUDIO_CONFIG_2080_112_2_18_1_2_4_38,"AUD_SetDgGain hAGCExp :dac_gain = %d")
TRACE_MSG(AUDIO_CONFIG_2302_112_2_18_1_2_4_39,"Voice_Dump_Pcm_Data_Callback index:%d, %d")
TRACE_MSG(AUDIO_CONFIG_2325_112_2_18_1_2_4_40,"Voice_Dump_Pcm_Data_Callback data:%02d,%02d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d")
TRACE_MSG(AUDIO_CONFIG_2364_112_2_18_1_2_4_41,"AUD_EnableVoiceProcessConfig is playing and needs not to play again.")
TRACE_MSG(AUDIO_CONFIG_2386_112_2_18_1_2_4_42,"AUD_EnableVoiceProcessConfig SFS_CreateFile failed.")
TRACE_MSG(AUDIO_CONFIG_2408_112_2_18_1_2_5_43,"\"aud_result=%d, ext_result=%d")
TRACE_MSG(AUDIO_CONFIG_2418_112_2_18_1_2_5_44,"AUD_EnableVoiceProcessConfig is stopped and needs not to stop again.")
TRACE_MSG(AUDIO_CONFIG_2428_112_2_18_1_2_5_45,"\"stop aud_result=%d, ext_result=%d")
TRACE_MSG(AUDIO_CONFIG_2443_112_2_18_1_2_5_46,"AUD_EnableVoiceProcessConfig stop. data size should save:%d(bytes), index:%d.")
TRACE_MSG(AUDIO_CONFIG_2461_112_2_18_1_2_5_47,"AUD_EnableVoiceCodec: is_enable = %d")
TRACE_MSG(AUDIO_EXTPCM_141_112_2_18_1_2_5_48,"InfoNES_Sound_threadx.cpp: NES_Play")
TRACE_MSG(AUDIO_EXTPCM_160_112_2_18_1_2_5_49,"InfoNES_Sound_threadx.cpp: NES_Stop")
TRACE_MSG(AUDIO_EXTPCM_226_112_2_18_1_2_5_50,"[AUDIO_EXTPCM]:AudioEXTPCMNotifyCallback:enter:hAudio = 0x%X, notify_info = 0x%X, affix_info = %ld")
TRACE_MSG(AUDIO_EXTPCM_238_112_2_18_1_2_6_51,"\rExtPCM_SoundInit")
TRACE_MSG(AUDIO_EXTPCM_286_112_2_18_1_2_6_52,"\rExtPCM_SoundExit")
TRACE_MSG(AUDIO_EXTPCM_309_112_2_18_1_2_6_53,"\rExtPCM_SoundOpen samples %d, sample_rate %d")
TRACE_MSG(AUDIO_EXTPCM_334_112_2_18_1_2_6_54,"\rExtPCM_SoundClose")
TRACE_MSG(AUDIO_EXTPCM_350_112_2_18_1_2_6_55,"ExtPCM_SoundOutput error, SCI_NULL == buff_ptr.")
TRACE_MSG(AUDIO_EXTPCM_374_112_2_18_1_2_6_56,"ExtPCM_SoundOutputStereo error, SCI_NULL == lbuff_ptr.")
TRACE_MSG(AUDIO_EXTPCM_379_112_2_18_1_2_6_57,"ExtPCM_SoundOutputStereo error, SCI_NULL == rbuff_ptr.")
TRACE_MSG(AUDIO_MTV_80_112_2_18_1_2_6_58,"audio_mtv.c,[MTV_Play]! ptAudioObj:0x%08x,uiOffset:0x%08x")
TRACE_MSG(AUDIO_MTV_99_112_2_18_1_2_6_59,"InfoNES_Sound_threadx.cpp: NES_Stop")
TRACE_MSG(AUDIO_MTV_176_112_2_18_1_2_6_60,"[AUDIO_MTV]:AudioMTVNotifyCallback:enter:hAudio = 0x%X, notify_info = 0x%X, affix_info = %ld")
TRACE_MSG(AUDIO_MTV_239_112_2_18_1_2_7_61,"audio_mtv.c,[MTV_SoundInit] samples:%d,sample_rate:%d")
TRACE_MSG(AUDIO_MTV_244_112_2_18_1_2_7_62,"audio_mtv.c,[MTV_SoundInit] error 0==sample_rate.")
TRACE_MSG(AUDIO_MTV_293_112_2_18_1_2_7_63,"audio_mtv.c,[MTV_SoundExit]hMtvRing:0x%08x,puiTrackBuffer:0x%08x!")
TRACE_MSG(AUDIO_MTV_329_112_2_18_1_2_7_64,"MTV_SoundOpen error 0==sample_rate.")
TRACE_MSG(AUDIO_MTV_332_112_2_18_1_2_7_65,"audio_mtv.c,[MTV_SoundOpen] samples %d, sample_rate %d")
TRACE_MSG(AUDIO_MTV_355_112_2_18_1_2_7_66,"audio_mtv.c,[MTV_SoundClose]hMtvRing:0x%08x!")
TRACE_MSG(AUDIO_MTV_368_112_2_18_1_2_7_67,"audio_mtv.c,[MTV_SoundClose] hMtvRing == INVALID_HANDLE!maybe mtv is stopped already!")
TRACE_MSG(AUDIO_MTV_385_112_2_18_1_2_7_68,"MTV_SoundOutput SCI_NULL==buff_ptr")
TRACE_MSG(AUDIO_MTV_410_112_2_18_1_2_7_69,"MTV_SoundOutputStereo 0x%x, 0x%x.")
TRACE_MSG(BKMUSIC_AS_142_112_2_18_1_2_8_70,"bkmusic_as.c,[BKMUSIC_SetGain]: the state and gain value! s_is_background_music_on:%d,eParamType:0x%08x,uiUplinkGain:0x%08x,uiDownlinkGain:0x%08x")
TRACE_MSG(BKMUSIC_AS_147_112_2_18_1_2_8_71,"bkmusic_as.c,[BKMUSIC_SetGain]:set the gains down!")
TRACE_MSG(BKMUSIC_AS_169_112_2_18_1_2_8_72,"BKMUSIC_Play input para error. uiSampleRate:0.")
TRACE_MSG(BKMUSIC_AS_177_112_2_18_1_2_8_73,"bkmusic_as.c,[BKMUSIC_Play]:call LAYER1_StartMagicVoiceFunc!s_magic_gain_type:0x%08x,s_magic_uplink_gain:0x%08x,s_magic_downlink_gain:0x%08x")
TRACE_MSG(BKMUSIC_AS_186_112_2_18_1_2_8_74,"bkmusic_as.c,[BKMUSIC_Play]:call LAYER1_StartMixVoiceFunc!s_magic_gain_type:0x%08x,s_magic_uplink_gain:0x%08x,s_magic_downlink_gain:0x%08x")
TRACE_MSG(BKMUSIC_AS_211_112_2_18_1_2_8_75,"bkmusic_as.c,[BKMUSIC_Play]:already open!")
TRACE_MSG(BKMUSIC_AS_232_112_2_18_1_2_8_76,"bkmusic_as.c,[BKMUSIC_Stop]:call LAYER1_StopMixVoiceFunc!")
TRACE_MSG(BKMUSIC_AS_237_112_2_18_1_2_8_77,"bkmusic_as.c,[BKMUSIC_Stop]:already off!")
TRACE_MSG(BKMUSIC_AS_257_112_2_18_1_2_8_78,"bkmusic_as.c,[BKMUSIC_Pause]:call LAYER1_StopMixVoiceFunc!")
TRACE_MSG(BKMUSIC_AS_262_112_2_18_1_2_8_79,"bkmusic_as.c,[BKMUSIC_Pause]:already off!")
TRACE_MSG(BKMUSIC_AS_275_112_2_18_1_2_8_80,"BKMUSIC_Resume input para error. uiSampleRate:0.")
TRACE_MSG(BKMUSIC_AS_284_112_2_18_1_2_8_81,"bkmusic_as.c,[BKMUSIC_Resume]:call LAYER1_StartMagicVoiceFunc!s_magic_gain_type:0x%08x,s_magic_uplink_gain:0x%08x,s_magic_downlink_gain:0x%08x")
TRACE_MSG(BKMUSIC_AS_294_112_2_18_1_2_8_82,"bkmusic_as.c,[BKMUSIC_Resume]:call LAYER1_StartMixVoiceFunc!s_magic_gain_type:0x%08x,s_magic_uplink_gain:0x%08x,s_magic_downlink_gain:0x%08x")
TRACE_MSG(BKMUSIC_AS_309_112_2_18_1_2_8_83,"bkmusic_as.c,[BKMUSIC_Resume]:already open!")
TRACE_MSG(BKMUSIC_AS_365_112_2_18_1_2_8_84,"bkmusic_as.c,[BKMUSIC_ReadDataFromBuffer]: pcm_data_buf is NULL")
TRACE_MSG(BKMUSIC_AS_372_112_2_18_1_2_8_85,"bkmusic_as.c,[BKMUSIC_ReadDataFromBuffer]: The num of pcm data is less than 160 words!")
END_TRACE_MAP(MS_REF_AUD_CONFIG_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_AUD_CONFIG_TRC_H_

