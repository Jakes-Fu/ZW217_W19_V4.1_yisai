/******************************************************************************
 ** File Name:      mmi_app_kuro_trc.h                                         *
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
//trace_id:189
#ifndef _MMI_APP_KURO_TRC_H_
#define _MMI_APP_KURO_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIKUR_332_112_2_18_2_31_31_0 "[MMIKURO]: MMIAPIKUR_InitKurFolder"
#define MMIKUR_573_112_2_18_2_31_32_1 "[MMIKURO]: MMIKUR_GetKurFilter, filter = %s, orig_filter = %s"
#define MMIKUR_586_112_2_18_2_31_32_2 "[MMIKURO]: MMIKUR_LoadAllKuro, is_loaded = %d"
#define MMIKUR_745_112_2_18_2_31_32_3 "[MMIKUR]: HandleLoadAllKuroWaitingWin, msg=%x"
#define MMIKUR_772_112_2_18_2_31_32_4 "[MMIMP3]: HandleMp3LoadMyMusicWaitingWin, MMIAPIFILEARRAY_Combine FAIL"
#define MMIKUR_924_112_2_18_2_31_32_5 "[MMIKURO] MMIKUR_GetKurInfoByIndex index = %d,result = %d"
#define MMIKUR_1038_112_2_18_2_31_33_6 "[MMIKURO]: MMIKUR_InitKuroListByFileInfo result = %d"
#define MMIKUR_1048_112_2_18_2_31_33_7 "[MMIKURO]: MMIAPIKUR_ResetAllKur"
#define MMIKUR_1168_112_2_18_2_31_33_8 "[MMIKURO] MMIKUR_DelayPlayKur encounter MMIKUR_PLAYER_SWITCH_PAUSED"
#define MMIKUR_1183_112_2_18_2_31_33_9 "[MMIKURO] MMIKUR_DelayPlayKur Start Timer = %d"
#define MMIKUR_1205_112_2_18_2_31_33_10 "[MMIKUR] MMIKUR_StopDelayPlayTimer Stop Timer"
#define MMIKUR_1232_112_2_18_2_31_33_11 "[MMIKUR] MMIAPIKUR_HandleDelayPlayTimer encounter MMIKUR_PLAYER_SWITCH_PAUSED"
#define MMIKUR_1239_112_2_18_2_31_33_12 "[MMIKUR] MMIAPIKUR_HandleDelayPlayTimer result = %d!!!"
#define MMIKUR_1306_112_2_18_2_31_33_13 "[MMIKUR] MMIAPIKUR_ResumeKurPlayer s_kur_pause_type = %d, pause_type = %d!!!"
#define MMIKUR_1366_112_2_18_2_31_33_14 "[MMIKURO]: MMIKUR_PlayKur, cur_index = %d, total_num = %d"
#define MMIKUR_1392_112_2_18_2_31_33_15 "[MMIKURO]: MMIKUR_PlayKur, kur_exist = %d, klr_exist = %d, kur_size = %d, klr_size = %d, Kur Head Size = %d"
#define MMIKUR_1439_112_2_18_2_31_33_16 "[MMIKUR]:MMIKUR_PlayKur, result = %d"
#define MMIKUR_1457_112_2_18_2_31_33_17 "[MMIKUR] MMIKUR_PauseKur player_state = %d, audio_state = %d, s_kur_pause_type = %d, pause_type = %d!!!"
#define MMIKUR_1487_112_2_18_2_31_34_18 "[MMIKUR]:MMIKUR_PauseKur :kuro_pause_pos=%d"
#define MMIKUR_1499_112_2_18_2_31_34_19 "[MMIKUR]:MMIKUR_PauseKur:AUDIO FAILED!"
#define MMIKUR_1557_112_2_18_2_31_34_20 "[MMIKUR] MMIKUR_ResumeKur player_state = %d, audio_state = %d"
#define MMIKUR_1600_112_2_18_2_31_34_21 "[MMIKUR]:MMIKUR_ResumeKur:AUDIO FAILED!"
#define MMIKUR_1653_112_2_18_2_31_34_22 "[MMIKUR] MMIKUR_StopKur player_state = %d, audio_state = %d"
#define MMIKUR_1713_112_2_18_2_31_34_23 "[MMIKURO]: MMIAPIKUR_PlayMyDocumentMusic"
#define MMIKUR_1750_112_2_18_2_31_34_24 "MMIAPIKUR_PlayMyDocumentMusic:memory = %d,name_len = %d"
#define MMIKUR_1766_112_2_18_2_31_34_25 "[MMIKURO]: MMIAPIKUR_PlayKurCallBack:cur_play_mode = %d"
#define MMIKUR_1843_112_2_18_2_31_34_26 "[MMIKURO]: MMIKUR_CallBackAfterPlay, MMIAUDIO_RESULT = %d"
#define MMIKUR_1890_112_2_18_2_31_34_27 "[MMIKURO]: MMIKUR_PlayKurFile, is_active_a2sp = %d"
#define MMIKUR_1983_112_2_18_2_31_34_28 "[MMIKURO]: MMIKUR_PlayKlr get_result = %d, open_result = %d,read_result = %d"
#define MMIKUR_2076_112_2_18_2_31_35_29 "[MMIKURO]GetNextKurIndex:Don't call this function!"
#define MMIKUR_2098_112_2_18_2_31_35_30 "[MMIKURO]: MMIAPIKUR_GoToPreOrNextKur is_prev = %d,cur_play_mode = %d"
#define MMIKUR_2194_112_2_18_2_31_35_31 "[MMIKURO]: MMIKUR_HandleKurError, msg_id = 0x%x"
#define MMIKUR_2392_112_2_18_2_31_36_32 "MMIAPIKUR_GetkurSampleRate s_kur_play_sample_rate = %d"
#define MMIKUR_2403_112_2_18_2_31_36_33 "MMIAPIKUR_RecordkurSampleRate s_kur_play_sample_rate = %d"
#define MMIKUR_2584_112_2_18_2_31_36_34 "[MMIKURO]: MMIAPIKUR_GetSampleRateBeforePlay, cur_index = %d, total_num = %d,sample_rate = %d"
#define MMIKUR_2619_112_2_18_2_31_36_35 "[MMIKURO]: MMIKURO_GetKurPlayedTime uiCurrentDataOffset = %d, kur_id3tag_size =%d,kur_bitrate = %d,cur_time = %d"
#define MMIKUR_2645_112_2_18_2_31_36_36 "[MMIKURO]:MMIKUR_GetSrcCallback----stop, s_kuro_read_counter=%d, s_kuro_file_length=%d, audio_handle=0x%x"
#define MMIKUR_2649_112_2_18_2_31_36_37 "[MMIKURO]:MMIKUR_GetSrcCallback----after AUDIO_Stop and before AUDIO_CloseHandle g_audio_handle=0x%x."
#define MMIKUR_2665_112_2_18_2_31_36_38 "[MMIKURO]:MMIKUR_GetSrcCallback----SFS_SetFilePointer error."
#define MMIKUR_2677_112_2_18_2_31_36_39 "[MMIKURO]:MMIKUR_GetSrcCallback----SFS_SetFilePointer error."
#define MMIKUR_2701_112_2_18_2_31_36_40 "[MMIKURO]:MMIKUR_GetSrcCallback----FFS_Read error."
#define MMIKUR_2749_112_2_18_2_31_36_41 "[MMIKURO]:MMIKUR_CreateAudioHandle----SFS_Create:0x%x"
#define MMIKUR_2818_112_2_18_2_31_36_42 "[MMIKUR]: FATAL ERROR"
#define MMIKUR_LYRIC_364_112_2_18_2_31_38_43 "[MMIKURO]: MMIKLR_DisplayBMPImageExt PMI Decode: Display JPG image staus: %d"
#define MMIKUR_LYRIC_373_112_2_18_2_31_38_44 "[MMIKURO]: MMIKUR_InitLyric"
#define MMIKUR_LYRIC_387_112_2_18_2_31_39_45 "[MMIKURO]: MMIKUR_StopLyric"
#define MMIKUR_LYRIC_400_112_2_18_2_31_39_46 "[MMIKURO]: AllocFileDataBuf, file_size = %d, s_kur_pic_buffer = %x"
#define MMIKUR_LYRIC_433_112_2_18_2_31_39_47 "[MMIKURO]: MMIKUR_FreeKlrBuffer"
#define MMIKUR_LYRIC_463_112_2_18_2_31_39_48 "[MMIKURO]: MMIKUR_AllocKlrBuffer"
#define MMIKUR_LYRIC_649_112_2_18_2_31_39_49 "[MMIKURO]: DisplayKlrLyric,s_nLastLine1 = %d,s_nLastPercent1 = %d,s_nLastLine2 = %d,s_nLastPercent2 = %d,s_nLastDisplayPos1 = %d,s_nLastDisplayPos2 = %d"
#define MMIKUR_LYRIC_868_112_2_18_2_31_39_50 "[MMIKURO]: MSG_KUR_START_READ_PIC finished !!! pic_index = %d"
#define MMIKUR_LYRIC_879_112_2_18_2_31_39_51 "[MMIKURO]: MMIKUR_DisplayKlr, bRepaintAll = %d,bDspToolBar = %d,nMode = %d,player_state = %d, audio_state = %d,read_finish = %d,is_first_read = %d"
#define MMIKUR_LYRIC_983_112_2_18_2_31_40_52 "[MMIKURO]: MMIKUR_DisplayKlr, nCurPicIndex = %d,pic_dsp_index = %d, pic_reading_index = %d, pic_readed_index1 = %d, pic_readed_index2 = %d,read_finish = %d, bPicChanged = %d"
#define MMIKUR_LYRIC_1104_112_2_18_2_31_40_53 "[MMIKURO]: MMIKUR_ReSetPicReadInfo"
#define MMIKUR_LYRIC_1146_112_2_18_2_31_40_54 "[MMIKURO]: MMIKUR_ReadFirstPic, nPicIndex = %d, nPicCount = %d, result = %d"
#define MMIKUR_LYRIC_1192_112_2_18_2_31_40_55 "[MMIKURO]: MMIKUR_IsBackgroundReady, result = %d"
#define MMIKUR_NV_132_112_2_18_2_31_43_56 "SMS: MMIAPISMS_NV_WriteSmsItem nv_result = %d, rid = %d"
#define MMIKUR_NV_177_112_2_18_2_31_43_57 "------MMIKURO_NV_ReadKgcInfo:%s,%d"
#define MMIKUR_WINTAB_274_112_2_18_2_31_44_58 "StartTuneVolumeKeyTimer: the key timer has started!"
#define MMIKUR_WINTAB_291_112_2_18_2_31_44_59 "StopTuneVolumeKeyTimer: the key timer has stop!"
#define MMIKUR_WINTAB_407_112_2_18_2_31_44_60 "[MMIKURO]:MMIAPIKUR_OpenKurPlayer, Kur Or Klr Not Exits!"
#define MMIKUR_WINTAB_717_112_2_18_2_31_45_61 "[MMIKURO]: HandleKurPlayWinMsg msg_id = 0x%x"
#define MMIKUR_WINTAB_721_112_2_18_2_31_45_62 "[MMIKURO]: KUR:MSG_KUR_PLAY_CALL_BACK"
#define MMIKUR_WINTAB_726_112_2_18_2_31_45_63 "[MMIKURO]: KUR:MSG_KUR_PLAY_START"
#define MMIKUR_WINTAB_763_112_2_18_2_31_45_64 "[MMIKURO]: HandleKurPlayWinMsg close volume and toolbar"
#define MMIKUR_WINTAB_1053_112_2_18_2_31_45_65 "[MMIKURO]:MMIKUR_StartAnimTimer s_kur_anim_timer_id= %d,result = %d"
#define MMIKUR_WINTAB_1324_112_2_18_2_31_46_66 "[MMIKURO] InitKurListItem MMIKUR_GetKurInfoByIndex failed"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_KURO_TRC)
TRACE_MSG(MMIKUR_332_112_2_18_2_31_31_0,"[MMIKURO]: MMIAPIKUR_InitKurFolder")
TRACE_MSG(MMIKUR_573_112_2_18_2_31_32_1,"[MMIKURO]: MMIKUR_GetKurFilter, filter = %s, orig_filter = %s")
TRACE_MSG(MMIKUR_586_112_2_18_2_31_32_2,"[MMIKURO]: MMIKUR_LoadAllKuro, is_loaded = %d")
TRACE_MSG(MMIKUR_745_112_2_18_2_31_32_3,"[MMIKUR]: HandleLoadAllKuroWaitingWin, msg=%x")
TRACE_MSG(MMIKUR_772_112_2_18_2_31_32_4,"[MMIMP3]: HandleMp3LoadMyMusicWaitingWin, MMIAPIFILEARRAY_Combine FAIL")
TRACE_MSG(MMIKUR_924_112_2_18_2_31_32_5,"[MMIKURO] MMIKUR_GetKurInfoByIndex index = %d,result = %d")
TRACE_MSG(MMIKUR_1038_112_2_18_2_31_33_6,"[MMIKURO]: MMIKUR_InitKuroListByFileInfo result = %d")
TRACE_MSG(MMIKUR_1048_112_2_18_2_31_33_7,"[MMIKURO]: MMIAPIKUR_ResetAllKur")
TRACE_MSG(MMIKUR_1168_112_2_18_2_31_33_8,"[MMIKURO] MMIKUR_DelayPlayKur encounter MMIKUR_PLAYER_SWITCH_PAUSED")
TRACE_MSG(MMIKUR_1183_112_2_18_2_31_33_9,"[MMIKURO] MMIKUR_DelayPlayKur Start Timer = %d")
TRACE_MSG(MMIKUR_1205_112_2_18_2_31_33_10,"[MMIKUR] MMIKUR_StopDelayPlayTimer Stop Timer")
TRACE_MSG(MMIKUR_1232_112_2_18_2_31_33_11,"[MMIKUR] MMIAPIKUR_HandleDelayPlayTimer encounter MMIKUR_PLAYER_SWITCH_PAUSED")
TRACE_MSG(MMIKUR_1239_112_2_18_2_31_33_12,"[MMIKUR] MMIAPIKUR_HandleDelayPlayTimer result = %d!!!")
TRACE_MSG(MMIKUR_1306_112_2_18_2_31_33_13,"[MMIKUR] MMIAPIKUR_ResumeKurPlayer s_kur_pause_type = %d, pause_type = %d!!!")
TRACE_MSG(MMIKUR_1366_112_2_18_2_31_33_14,"[MMIKURO]: MMIKUR_PlayKur, cur_index = %d, total_num = %d")
TRACE_MSG(MMIKUR_1392_112_2_18_2_31_33_15,"[MMIKURO]: MMIKUR_PlayKur, kur_exist = %d, klr_exist = %d, kur_size = %d, klr_size = %d, Kur Head Size = %d")
TRACE_MSG(MMIKUR_1439_112_2_18_2_31_33_16,"[MMIKUR]:MMIKUR_PlayKur, result = %d")
TRACE_MSG(MMIKUR_1457_112_2_18_2_31_33_17,"[MMIKUR] MMIKUR_PauseKur player_state = %d, audio_state = %d, s_kur_pause_type = %d, pause_type = %d!!!")
TRACE_MSG(MMIKUR_1487_112_2_18_2_31_34_18,"[MMIKUR]:MMIKUR_PauseKur :kuro_pause_pos=%d")
TRACE_MSG(MMIKUR_1499_112_2_18_2_31_34_19,"[MMIKUR]:MMIKUR_PauseKur:AUDIO FAILED!")
TRACE_MSG(MMIKUR_1557_112_2_18_2_31_34_20,"[MMIKUR] MMIKUR_ResumeKur player_state = %d, audio_state = %d")
TRACE_MSG(MMIKUR_1600_112_2_18_2_31_34_21,"[MMIKUR]:MMIKUR_ResumeKur:AUDIO FAILED!")
TRACE_MSG(MMIKUR_1653_112_2_18_2_31_34_22,"[MMIKUR] MMIKUR_StopKur player_state = %d, audio_state = %d")
TRACE_MSG(MMIKUR_1713_112_2_18_2_31_34_23,"[MMIKURO]: MMIAPIKUR_PlayMyDocumentMusic")
TRACE_MSG(MMIKUR_1750_112_2_18_2_31_34_24,"MMIAPIKUR_PlayMyDocumentMusic:memory = %d,name_len = %d")
TRACE_MSG(MMIKUR_1766_112_2_18_2_31_34_25,"[MMIKURO]: MMIAPIKUR_PlayKurCallBack:cur_play_mode = %d")
TRACE_MSG(MMIKUR_1843_112_2_18_2_31_34_26,"[MMIKURO]: MMIKUR_CallBackAfterPlay, MMIAUDIO_RESULT = %d")
TRACE_MSG(MMIKUR_1890_112_2_18_2_31_34_27,"[MMIKURO]: MMIKUR_PlayKurFile, is_active_a2sp = %d")
TRACE_MSG(MMIKUR_1983_112_2_18_2_31_34_28,"[MMIKURO]: MMIKUR_PlayKlr get_result = %d, open_result = %d,read_result = %d")
TRACE_MSG(MMIKUR_2076_112_2_18_2_31_35_29,"[MMIKURO]GetNextKurIndex:Don't call this function!")
TRACE_MSG(MMIKUR_2098_112_2_18_2_31_35_30,"[MMIKURO]: MMIAPIKUR_GoToPreOrNextKur is_prev = %d,cur_play_mode = %d")
TRACE_MSG(MMIKUR_2194_112_2_18_2_31_35_31,"[MMIKURO]: MMIKUR_HandleKurError, msg_id = 0x%x")
TRACE_MSG(MMIKUR_2392_112_2_18_2_31_36_32,"MMIAPIKUR_GetkurSampleRate s_kur_play_sample_rate = %d")
TRACE_MSG(MMIKUR_2403_112_2_18_2_31_36_33,"MMIAPIKUR_RecordkurSampleRate s_kur_play_sample_rate = %d")
TRACE_MSG(MMIKUR_2584_112_2_18_2_31_36_34,"[MMIKURO]: MMIAPIKUR_GetSampleRateBeforePlay, cur_index = %d, total_num = %d,sample_rate = %d")
TRACE_MSG(MMIKUR_2619_112_2_18_2_31_36_35,"[MMIKURO]: MMIKURO_GetKurPlayedTime uiCurrentDataOffset = %d, kur_id3tag_size =%d,kur_bitrate = %d,cur_time = %d")
TRACE_MSG(MMIKUR_2645_112_2_18_2_31_36_36,"[MMIKURO]:MMIKUR_GetSrcCallback----stop, s_kuro_read_counter=%d, s_kuro_file_length=%d, audio_handle=0x%x")
TRACE_MSG(MMIKUR_2649_112_2_18_2_31_36_37,"[MMIKURO]:MMIKUR_GetSrcCallback----after AUDIO_Stop and before AUDIO_CloseHandle g_audio_handle=0x%x.")
TRACE_MSG(MMIKUR_2665_112_2_18_2_31_36_38,"[MMIKURO]:MMIKUR_GetSrcCallback----SFS_SetFilePointer error.")
TRACE_MSG(MMIKUR_2677_112_2_18_2_31_36_39,"[MMIKURO]:MMIKUR_GetSrcCallback----SFS_SetFilePointer error.")
TRACE_MSG(MMIKUR_2701_112_2_18_2_31_36_40,"[MMIKURO]:MMIKUR_GetSrcCallback----FFS_Read error.")
TRACE_MSG(MMIKUR_2749_112_2_18_2_31_36_41,"[MMIKURO]:MMIKUR_CreateAudioHandle----SFS_Create:0x%x")
TRACE_MSG(MMIKUR_2818_112_2_18_2_31_36_42,"[MMIKUR]: FATAL ERROR")
TRACE_MSG(MMIKUR_LYRIC_364_112_2_18_2_31_38_43,"[MMIKURO]: MMIKLR_DisplayBMPImageExt PMI Decode: Display JPG image staus: %d")
TRACE_MSG(MMIKUR_LYRIC_373_112_2_18_2_31_38_44,"[MMIKURO]: MMIKUR_InitLyric")
TRACE_MSG(MMIKUR_LYRIC_387_112_2_18_2_31_39_45,"[MMIKURO]: MMIKUR_StopLyric")
TRACE_MSG(MMIKUR_LYRIC_400_112_2_18_2_31_39_46,"[MMIKURO]: AllocFileDataBuf, file_size = %d, s_kur_pic_buffer = %x")
TRACE_MSG(MMIKUR_LYRIC_433_112_2_18_2_31_39_47,"[MMIKURO]: MMIKUR_FreeKlrBuffer")
TRACE_MSG(MMIKUR_LYRIC_463_112_2_18_2_31_39_48,"[MMIKURO]: MMIKUR_AllocKlrBuffer")
TRACE_MSG(MMIKUR_LYRIC_649_112_2_18_2_31_39_49,"[MMIKURO]: DisplayKlrLyric,s_nLastLine1 = %d,s_nLastPercent1 = %d,s_nLastLine2 = %d,s_nLastPercent2 = %d,s_nLastDisplayPos1 = %d,s_nLastDisplayPos2 = %d")
TRACE_MSG(MMIKUR_LYRIC_868_112_2_18_2_31_39_50,"[MMIKURO]: MSG_KUR_START_READ_PIC finished !!! pic_index = %d")
TRACE_MSG(MMIKUR_LYRIC_879_112_2_18_2_31_39_51,"[MMIKURO]: MMIKUR_DisplayKlr, bRepaintAll = %d,bDspToolBar = %d,nMode = %d,player_state = %d, audio_state = %d,read_finish = %d,is_first_read = %d")
TRACE_MSG(MMIKUR_LYRIC_983_112_2_18_2_31_40_52,"[MMIKURO]: MMIKUR_DisplayKlr, nCurPicIndex = %d,pic_dsp_index = %d, pic_reading_index = %d, pic_readed_index1 = %d, pic_readed_index2 = %d,read_finish = %d, bPicChanged = %d")
TRACE_MSG(MMIKUR_LYRIC_1104_112_2_18_2_31_40_53,"[MMIKURO]: MMIKUR_ReSetPicReadInfo")
TRACE_MSG(MMIKUR_LYRIC_1146_112_2_18_2_31_40_54,"[MMIKURO]: MMIKUR_ReadFirstPic, nPicIndex = %d, nPicCount = %d, result = %d")
TRACE_MSG(MMIKUR_LYRIC_1192_112_2_18_2_31_40_55,"[MMIKURO]: MMIKUR_IsBackgroundReady, result = %d")
TRACE_MSG(MMIKUR_NV_132_112_2_18_2_31_43_56,"SMS: MMIAPISMS_NV_WriteSmsItem nv_result = %d, rid = %d")
TRACE_MSG(MMIKUR_NV_177_112_2_18_2_31_43_57,"------MMIKURO_NV_ReadKgcInfo:%s,%d")
TRACE_MSG(MMIKUR_WINTAB_274_112_2_18_2_31_44_58,"StartTuneVolumeKeyTimer: the key timer has started!")
TRACE_MSG(MMIKUR_WINTAB_291_112_2_18_2_31_44_59,"StopTuneVolumeKeyTimer: the key timer has stop!")
TRACE_MSG(MMIKUR_WINTAB_407_112_2_18_2_31_44_60,"[MMIKURO]:MMIAPIKUR_OpenKurPlayer, Kur Or Klr Not Exits!")
TRACE_MSG(MMIKUR_WINTAB_717_112_2_18_2_31_45_61,"[MMIKURO]: HandleKurPlayWinMsg msg_id = 0x%x")
TRACE_MSG(MMIKUR_WINTAB_721_112_2_18_2_31_45_62,"[MMIKURO]: KUR:MSG_KUR_PLAY_CALL_BACK")
TRACE_MSG(MMIKUR_WINTAB_726_112_2_18_2_31_45_63,"[MMIKURO]: KUR:MSG_KUR_PLAY_START")
TRACE_MSG(MMIKUR_WINTAB_763_112_2_18_2_31_45_64,"[MMIKURO]: HandleKurPlayWinMsg close volume and toolbar")
TRACE_MSG(MMIKUR_WINTAB_1053_112_2_18_2_31_45_65,"[MMIKURO]:MMIKUR_StartAnimTimer s_kur_anim_timer_id= %d,result = %d")
TRACE_MSG(MMIKUR_WINTAB_1324_112_2_18_2_31_46_66,"[MMIKURO] InitKurListItem MMIKUR_GetKurInfoByIndex failed")
END_TRACE_MAP(MMI_APP_KURO_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_KURO_TRC_H_

