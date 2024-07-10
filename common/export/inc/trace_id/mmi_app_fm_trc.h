/******************************************************************************
 ** File Name:      mmi_app_fm_trc.h                                         *
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
//trace_id:178
#ifndef _MMI_APP_FM_TRC_H_
#define _MMI_APP_FM_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIFM_APP_229_112_2_18_2_19_21_0 "FM RecordCallBack(), report_ptr->report=%d"
#define MMIFM_APP_288_112_2_18_2_19_21_1 "MMIFM_ReqPlaySrv: s_fm_play_handle=0x%x"
#define MMIFM_APP_340_112_2_18_2_19_21_2 "MMIFM_ReqRecSrv: s_fm_record_handle=0x%x"
#define MMIFM_APP_402_112_2_18_2_19_21_3 "[MMIFM] MMIFM_AdjustVolume step %d"
#define MMIFM_APP_436_112_2_18_2_19_21_4 "[MMIFM] MMIFM_AdjustVolumeTP step %d"
#define MMIFM_APP_473_112_2_18_2_19_21_5 "[MMIFM] MMIFM_Play init_result %d !"
#define MMIFM_APP_511_112_2_18_2_19_21_6 "[MMIFM] MMIAPIFM_EngTestPlay init_result %d !"
#define MMIFM_APP_533_112_2_18_2_19_21_7 "[MMIFM] MMIFM_EngTestStop"
#define MMIFM_APP_543_112_2_18_2_19_22_8 "[MMIFM] MMIFM_GetStatus s_fm_cur_info.status %d"
#define MMIFM_APP_560_112_2_18_2_19_22_9 "[MMIFM] MMIFM_GetStatus s_fm_cur_info.fm_record_file_id %d"
#define MMIFM_APP_594_112_2_18_2_19_22_10 "[MMIFM] MMIFM_IsValidFre ret %d"
#define MMIFM_APP_612_112_2_18_2_19_22_11 "[MMIFM] MMIFM_AdjustFrequency step %d"
#define MMIFM_APP_645_112_2_18_2_19_22_12 "[MMIFM] MMIFM_AdjustFrequency FM_ManualSeek failed !"
#define MMIFM_APP_665_112_2_18_2_19_22_13 "[MMIFM] MMIFM_AdjustFrequency step %d"
#define MMIFM_APP_710_112_2_18_2_19_22_14 "[MMIFM] MMIFM_AdjustFrequency FM_ManualSeek failed !"
#define MMIFM_APP_731_112_2_18_2_19_22_15 "[MMIFM] MMIFM_AdjustChannel step %d channel index %d"
#define MMIFM_APP_746_112_2_18_2_19_22_16 "MMIFM] MMIFM_AdjustChannel s_fm_cur_info.cur_channel_index %d"
#define MMIFM_APP_752_112_2_18_2_19_22_17 "[MMIFM]MMIFM_AdjustChannel channel.freqency %d !"
#define MMIFM_APP_779_112_2_18_2_19_22_18 "[MMIFM] MMIFM_SwitchAudioDevice  status %d sound_mode "
#define MMIFM_APP_794_112_2_18_2_19_22_19 "MMIFM_SwitchAudioDevice s_fm_cur_info.sound_mode %d"
#define MMIFM_APP_818_112_2_18_2_19_22_20 "[MMIFM] MMIFM_SearchValidFreq  status %d direct %d"
#define MMIFM_APP_879_112_2_18_2_19_22_21 "[MMIFM] MMIFM_Stop"
#define MMIFM_APP_899_112_2_18_2_19_22_22 "[MMIFM] MMIFM_Pause"
#define MMIFM_APP_947_112_2_18_2_19_22_23 "[MMIFM] MMIFM_Exit is_set_bg_play %d s_fm_cur_info.is_bgplay 2 %d"
#define MMIFM_APP_989_112_2_18_2_19_22_24 "[MMIFM] MMIFM_ExitFMByOthers is_set_bg_play %d s_fm_cur_info.is_bgplay %d"
#define MMIFM_APP_1019_112_2_18_2_19_23_25 "[MMIFM] MMIFM_GetFMCurrentDeviceMode sound_mode %d"
#define MMIFM_APP_1028_112_2_18_2_19_23_26 "[MMIFM] MMIFM_SetFMDeviceMode sound_mode %d"
#define MMIFM_APP_1044_112_2_18_2_19_23_27 "[MMIFM] MMIFM_AuotSearchFreq cur_search_channel %d"
#define MMIFM_APP_1055_112_2_18_2_19_23_28 "[MMIFM] MMIFM_AuotSearchFreq one_channel.freqency %d !"
#define MMIFM_APP_1062_112_2_18_2_19_23_29 "[MMIFM] MMIFM_AuotSearchFreq one_channel.freqency %d seeked freqency %d, ret = %d"
#define MMIFM_APP_1076_112_2_18_2_19_23_30 "[MMIFM] MMIFM_AuotSearchFreq duplicate frequency!!!"
#define MMIFM_APP_1219_112_2_18_2_19_23_31 "[MMIFM] MMIFM_PlayChannel ch_index %d"
#define MMIFM_APP_1223_112_2_18_2_19_23_32 "[MMIFM] MMIFM_PlayChannel MMIFM_GetOneChannel fail !"
#define MMIFM_APP_1228_112_2_18_2_19_23_33 "[MMIFM] MMIFM_PlayChannel channel.freqency %d !"
#define MMIFM_APP_1258_112_2_18_2_19_23_34 "[MMIFM] MMIFM_GetChannelIndexByFreq ch_index %d"
#define MMIFM_APP_1300_112_2_18_2_19_23_35 "[MMIFM]GetFMDeviceByAudioMode device %d"
#define MMIFM_APP_1341_112_2_18_2_19_23_36 "[MMIFM] MMIFM_IsChannelExist ret %d"
#define MMIFM_APP_1361_112_2_18_2_19_23_37 "[MMIFM] MMIFM_IsChannelValid channel_ptr->freqency %d !"
#define MMIFM_APP_1365_112_2_18_2_19_23_38 "[MMIFM] MMIFM_IsChannelValid ret %d"
#define MMIFM_APP_1386_112_2_18_2_19_23_39 "[MMIFM] MMIFM_GetValidChannelCount ch_count %d"
#define MMIFM_APP_1420_112_2_18_2_19_23_40 "[MMIFM] MMIFM_SetCurrrentChannel ch_index %d"
#define MMIFM_APP_1495_112_2_18_2_19_23_41 "[MMIFM] MMIFM_PlayInputFreq ret %d !"
#define MMIFM_APP_1510_112_2_18_2_19_24_42 "[MMIFM] MMIFM_GetRecordStatus(), status = %d"
#define MMIFM_APP_1541_112_2_18_2_19_24_43 "[MMIFM] MMIFM_StartRecord"
#define MMIFM_APP_1573_112_2_18_2_19_24_44 "[MMIFMRECORD]CreateFileName() s_fm_record_file_path = 0x%x"
#define MMIFM_APP_1603_112_2_18_2_19_24_45 "[MMIFMRECORD]:MMIFMRECORD_SetRecordStorage storage=%d "
#define MMIFM_APP_1623_112_2_18_2_19_24_46 "[MMIFMRECORD] CreateFileName()"
#define MMIFM_APP_1627_112_2_18_2_19_24_47 "CreateFileName PNULL == input_file_path!"
#define MMIFM_APP_1679_112_2_18_2_19_24_48 "[MMIFMRECORD]CreateFileName() full_path = 0x%x"
#define MMIFM_APP_1731_112_2_18_2_19_24_49 "[MMIFM] enter funciton MMIFM_RecordPauseRecord()"
#define MMIFM_APP_1745_112_2_18_2_19_24_50 "[MMIFM] enter funciton MMIFM_RecordResumeRecord()"
#define MMIFM_APP_1769_112_2_18_2_19_24_51 "[MMIRECORD] enter funciton MMIRECORD_StopRecord()"
#define MMIFM_APP_1773_112_2_18_2_19_24_52 "[MMIRECORD] MMIRECORD_StopRecord() error"
#define MMIFM_APP_1793_112_2_18_2_19_24_53 "MMIFM_RecordGetFinallySlashIndex PNULL == file_name_ptr!"
#define MMIFM_APP_1800_112_2_18_2_19_24_54 "MMIFM_RecordGetFinallySlashIndex i =%d"
#define MMIFM_APP_2022_112_2_18_2_19_25_55 "[MMIFM]:MMIFM_RECORDCallBack:result = %d"
#define MMIFM_APP_2174_112_2_18_2_19_25_56 "[MMIFM] ConvertStringToInt ret %d"
#define MMIFM_APP_2303_112_2_18_2_19_25_57 "[MMIFM] MMIFM_GetNextValidChannelByFreq freq %d,step %d"
#define MMIFM_EXPORT_179_112_2_18_2_19_26_58 "[MMIFM] MMIAPIFM_GetCurFreqName  str_ptr valid !"
#define MMIFM_EXPORT_239_112_2_18_2_19_26_59 "[MMIFM] MMIAPIFM_StartFMTimerRecord  enter"
#define MMIFM_EXPORT_277_112_2_18_2_19_26_60 "isplayFmName:fm_status %d is error!"
#define MMIFM_EXPORT_293_112_2_18_2_19_26_61 "[MMIFM] MMIAPIFM_IsFmRecordActive  result %d"
#define MMIFM_WINTAB_1655_112_2_18_2_19_30_62 "MMIFM_OpenMainWin"
#define MMIFM_WINTAB_1678_112_2_18_2_19_30_63 "[MMIFM] [HandleFmMainMsg] msg_id = %x"
#define MMIFM_WINTAB_1974_112_2_18_2_19_31_64 "[MMIFM] [HandleFmMainMsg] msg_id = %x"
#define MMIFM_WINTAB_2332_112_2_18_2_19_32_65 "[MMIFM] HandleFmMainOptionMsg menu_id %d"
#define MMIFM_WINTAB_2482_112_2_18_2_19_32_66 "[MMIFM] HandleChannelListWinMsg MSG_APP_OK s_current_sel_channel %d"
#define MMIFM_WINTAB_2508_112_2_18_2_19_32_67 "[MMIFM] HandleChannelListWinMsg MSG_APP_WEB s_current_sel_channel %d"
#define MMIFM_WINTAB_2642_112_2_18_2_19_33_68 "[MMIFM] HandleChannelListWinMsg MSG_APP_OK s_current_sel_channel %d"
#define MMIFM_WINTAB_2672_112_2_18_2_19_33_69 "[MMIFM] HandleChannelListWinMsg MSG_APP_WEB s_current_sel_channel %d"
#define MMIFM_WINTAB_2947_112_2_18_2_19_33_70 "[MMIFM] HandleEditBoxWinMsg MSG_APP_WEB s_current_operator %d"
#define MMIFM_WINTAB_3011_112_2_18_2_19_33_71 "HandleEditBoxWinMsg OK s_current_operator %d !"
#define MMIFM_WINTAB_3259_112_2_18_2_19_34_72 "[MMIFM] ConvertFloatStrToInt ret %d"
#define MMIFM_WINTAB_3347_112_2_18_2_19_34_73 "[MMIFM] HandleFMAction act_type %d"
#define MMIFM_WINTAB_3387_112_2_18_2_19_34_74 "[MMIFM] HandleFMAction act_type %d"
#define MMIFM_WINTAB_3599_112_2_18_2_19_35_75 "[MMIFM] GetChannelDispName channel_ptr %d disp_name_ptr %d max_name_len %d !"
#define MMIFM_WINTAB_4118_112_2_18_2_19_36_76 "HandleAutoSearchWinMsg  MSG_APP_CANCEL channel_index %d"
#define MMIFM_WINTAB_4131_112_2_18_2_19_36_77 "[MMIFM]HandleAutoSearchWinMsg channel_index %d, s_search_ret %d"
#define MMIFM_WINTAB_4210_112_2_18_2_19_36_78 "HandleSearchValidFreqWinMsg  MSG_APP_CANCEL"
#define MMIFM_WINTAB_4220_112_2_18_2_19_36_79 "[MMIFM]HandleSearchValidFreqWinMsg s_search_ret %d"
#define MMIFM_WINTAB_4258_112_2_18_2_19_36_80 "[MMIFM] DrawFrqqencyText PNULL == cur_info_ptr !"
#define MMIFM_WINTAB_4307_112_2_18_2_19_36_81 "[MMIFM] DrawChannelName PNULL == cur_info_ptr !"
#define MMIFM_WINTAB_4370_112_2_18_2_19_36_82 "[MMIFM] DrawChannelNumber PNULL == num_str_ptr !"
#define MMIFM_WINTAB_4401_112_2_18_2_19_36_83 "[MMIFM] DrawFrqqencyText PNULL == cur_info_ptr !"
#define MMIFM_WINTAB_5185_112_2_18_2_19_38_84 "[MMIFM] MMIFM_HandleHeadsetAction msg_id %d"
#define MMIFM_WINTAB_5257_112_2_18_2_19_38_85 "SetCurrentSelChannel ch_no %d"
#define MMIFM_WINTAB_5266_112_2_18_2_19_38_86 "GetCurrentSelChannel s_current_sel_channel %d"
#define MMIFM_WINTAB_5277_112_2_18_2_19_38_87 "SetCurrentOperator s_current_operator %d"
#define MMIFM_WINTAB_5286_112_2_18_2_19_38_88 "GetCurrentOperator s_current_operator %d"
#define MMIFM_WINTAB_5631_112_2_18_2_19_39_89 "[MMIFM] HandleAlarmChannelListWinMsg MSG_APP_WEB s_current_sel_channel %d"
#define MMIFM_WINTAB_5668_112_2_18_2_19_39_90 "[MMIFM] HandleAlarmChannelListWinMsg MSG_APP_WEB s_current_sel_channel %d"
#define MMIFM_WINTAB_5874_112_2_18_2_19_39_91 "[MMIFMRECORD] HandleFmRecordMsg() msg_id = 0x%x"
#define MMIFM_WINTAB_6052_112_2_18_2_19_40_92 "[MMIFMRECORD] HandleFmTimerRecordMsg() msg_id = 0x%x"
#define MMIFM_WINTAB_6462_112_2_18_2_19_40_93 "FMGetRecordTime fail ! time_ptr = PNULL"
#define MMIFM_WINTAB_6496_112_2_18_2_19_40_94 "[MMIFM] FMRecordStartRecord()"
#define MMIFM_WINTAB_6536_112_2_18_2_19_41_95 "[MMIFM] FMRecordStartRecord() fs_error %d,error_code %d"
#define MMIFM_WINTAB_6624_112_2_18_2_19_41_96 "MMIFM: HandleFmRecordRenameWinMsg, win_id =0x%x, msg_id = 0x%x."
#define MMIFM_WINTAB_8122_112_2_18_2_19_44_97 "mmi FMApplet_HandleEvent msg_id = 0x%04x"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_FM_TRC)
TRACE_MSG(MMIFM_APP_229_112_2_18_2_19_21_0,"FM RecordCallBack(), report_ptr->report=%d")
TRACE_MSG(MMIFM_APP_288_112_2_18_2_19_21_1,"MMIFM_ReqPlaySrv: s_fm_play_handle=0x%x")
TRACE_MSG(MMIFM_APP_340_112_2_18_2_19_21_2,"MMIFM_ReqRecSrv: s_fm_record_handle=0x%x")
TRACE_MSG(MMIFM_APP_402_112_2_18_2_19_21_3,"[MMIFM] MMIFM_AdjustVolume step %d")
TRACE_MSG(MMIFM_APP_436_112_2_18_2_19_21_4,"[MMIFM] MMIFM_AdjustVolumeTP step %d")
TRACE_MSG(MMIFM_APP_473_112_2_18_2_19_21_5,"[MMIFM] MMIFM_Play init_result %d !")
TRACE_MSG(MMIFM_APP_511_112_2_18_2_19_21_6,"[MMIFM] MMIAPIFM_EngTestPlay init_result %d !")
TRACE_MSG(MMIFM_APP_533_112_2_18_2_19_21_7,"[MMIFM] MMIFM_EngTestStop")
TRACE_MSG(MMIFM_APP_543_112_2_18_2_19_22_8,"[MMIFM] MMIFM_GetStatus s_fm_cur_info.status %d")
TRACE_MSG(MMIFM_APP_560_112_2_18_2_19_22_9,"[MMIFM] MMIFM_GetStatus s_fm_cur_info.fm_record_file_id %d")
TRACE_MSG(MMIFM_APP_594_112_2_18_2_19_22_10,"[MMIFM] MMIFM_IsValidFre ret %d")
TRACE_MSG(MMIFM_APP_612_112_2_18_2_19_22_11,"[MMIFM] MMIFM_AdjustFrequency step %d")
TRACE_MSG(MMIFM_APP_645_112_2_18_2_19_22_12,"[MMIFM] MMIFM_AdjustFrequency FM_ManualSeek failed !")
TRACE_MSG(MMIFM_APP_665_112_2_18_2_19_22_13,"[MMIFM] MMIFM_AdjustFrequency step %d")
TRACE_MSG(MMIFM_APP_710_112_2_18_2_19_22_14,"[MMIFM] MMIFM_AdjustFrequency FM_ManualSeek failed !")
TRACE_MSG(MMIFM_APP_731_112_2_18_2_19_22_15,"[MMIFM] MMIFM_AdjustChannel step %d channel index %d")
TRACE_MSG(MMIFM_APP_746_112_2_18_2_19_22_16,"MMIFM] MMIFM_AdjustChannel s_fm_cur_info.cur_channel_index %d")
TRACE_MSG(MMIFM_APP_752_112_2_18_2_19_22_17,"[MMIFM]MMIFM_AdjustChannel channel.freqency %d !")
TRACE_MSG(MMIFM_APP_779_112_2_18_2_19_22_18,"[MMIFM] MMIFM_SwitchAudioDevice  status %d sound_mode ")
TRACE_MSG(MMIFM_APP_794_112_2_18_2_19_22_19,"MMIFM_SwitchAudioDevice s_fm_cur_info.sound_mode %d")
TRACE_MSG(MMIFM_APP_818_112_2_18_2_19_22_20,"[MMIFM] MMIFM_SearchValidFreq  status %d direct %d")
TRACE_MSG(MMIFM_APP_879_112_2_18_2_19_22_21,"[MMIFM] MMIFM_Stop")
TRACE_MSG(MMIFM_APP_899_112_2_18_2_19_22_22,"[MMIFM] MMIFM_Pause")
TRACE_MSG(MMIFM_APP_947_112_2_18_2_19_22_23,"[MMIFM] MMIFM_Exit is_set_bg_play %d s_fm_cur_info.is_bgplay 2 %d")
TRACE_MSG(MMIFM_APP_989_112_2_18_2_19_22_24,"[MMIFM] MMIFM_ExitFMByOthers is_set_bg_play %d s_fm_cur_info.is_bgplay %d")
TRACE_MSG(MMIFM_APP_1019_112_2_18_2_19_23_25,"[MMIFM] MMIFM_GetFMCurrentDeviceMode sound_mode %d")
TRACE_MSG(MMIFM_APP_1028_112_2_18_2_19_23_26,"[MMIFM] MMIFM_SetFMDeviceMode sound_mode %d")
TRACE_MSG(MMIFM_APP_1044_112_2_18_2_19_23_27,"[MMIFM] MMIFM_AuotSearchFreq cur_search_channel %d")
TRACE_MSG(MMIFM_APP_1055_112_2_18_2_19_23_28,"[MMIFM] MMIFM_AuotSearchFreq one_channel.freqency %d !")
TRACE_MSG(MMIFM_APP_1062_112_2_18_2_19_23_29,"[MMIFM] MMIFM_AuotSearchFreq one_channel.freqency %d seeked freqency %d, ret = %d")
TRACE_MSG(MMIFM_APP_1076_112_2_18_2_19_23_30,"[MMIFM] MMIFM_AuotSearchFreq duplicate frequency!!!")
TRACE_MSG(MMIFM_APP_1219_112_2_18_2_19_23_31,"[MMIFM] MMIFM_PlayChannel ch_index %d")
TRACE_MSG(MMIFM_APP_1223_112_2_18_2_19_23_32,"[MMIFM] MMIFM_PlayChannel MMIFM_GetOneChannel fail !")
TRACE_MSG(MMIFM_APP_1228_112_2_18_2_19_23_33,"[MMIFM] MMIFM_PlayChannel channel.freqency %d !")
TRACE_MSG(MMIFM_APP_1258_112_2_18_2_19_23_34,"[MMIFM] MMIFM_GetChannelIndexByFreq ch_index %d")
TRACE_MSG(MMIFM_APP_1300_112_2_18_2_19_23_35,"[MMIFM]GetFMDeviceByAudioMode device %d")
TRACE_MSG(MMIFM_APP_1341_112_2_18_2_19_23_36,"[MMIFM] MMIFM_IsChannelExist ret %d")
TRACE_MSG(MMIFM_APP_1361_112_2_18_2_19_23_37,"[MMIFM] MMIFM_IsChannelValid channel_ptr->freqency %d !")
TRACE_MSG(MMIFM_APP_1365_112_2_18_2_19_23_38,"[MMIFM] MMIFM_IsChannelValid ret %d")
TRACE_MSG(MMIFM_APP_1386_112_2_18_2_19_23_39,"[MMIFM] MMIFM_GetValidChannelCount ch_count %d")
TRACE_MSG(MMIFM_APP_1420_112_2_18_2_19_23_40,"[MMIFM] MMIFM_SetCurrrentChannel ch_index %d")
TRACE_MSG(MMIFM_APP_1495_112_2_18_2_19_23_41,"[MMIFM] MMIFM_PlayInputFreq ret %d !")
TRACE_MSG(MMIFM_APP_1510_112_2_18_2_19_24_42,"[MMIFM] MMIFM_GetRecordStatus(), status = %d")
TRACE_MSG(MMIFM_APP_1541_112_2_18_2_19_24_43,"[MMIFM] MMIFM_StartRecord")
TRACE_MSG(MMIFM_APP_1573_112_2_18_2_19_24_44,"[MMIFMRECORD]CreateFileName() s_fm_record_file_path = 0x%x")
TRACE_MSG(MMIFM_APP_1603_112_2_18_2_19_24_45,"[MMIFMRECORD]:MMIFMRECORD_SetRecordStorage storage=%d ")
TRACE_MSG(MMIFM_APP_1623_112_2_18_2_19_24_46,"[MMIFMRECORD] CreateFileName()")
TRACE_MSG(MMIFM_APP_1627_112_2_18_2_19_24_47,"CreateFileName PNULL == input_file_path!")
TRACE_MSG(MMIFM_APP_1679_112_2_18_2_19_24_48,"[MMIFMRECORD]CreateFileName() full_path = 0x%x")
TRACE_MSG(MMIFM_APP_1731_112_2_18_2_19_24_49,"[MMIFM] enter funciton MMIFM_RecordPauseRecord()")
TRACE_MSG(MMIFM_APP_1745_112_2_18_2_19_24_50,"[MMIFM] enter funciton MMIFM_RecordResumeRecord()")
TRACE_MSG(MMIFM_APP_1769_112_2_18_2_19_24_51,"[MMIRECORD] enter funciton MMIRECORD_StopRecord()")
TRACE_MSG(MMIFM_APP_1773_112_2_18_2_19_24_52,"[MMIRECORD] MMIRECORD_StopRecord() error")
TRACE_MSG(MMIFM_APP_1793_112_2_18_2_19_24_53,"MMIFM_RecordGetFinallySlashIndex PNULL == file_name_ptr!")
TRACE_MSG(MMIFM_APP_1800_112_2_18_2_19_24_54,"MMIFM_RecordGetFinallySlashIndex i =%d")
TRACE_MSG(MMIFM_APP_2022_112_2_18_2_19_25_55,"[MMIFM]:MMIFM_RECORDCallBack:result = %d")
TRACE_MSG(MMIFM_APP_2174_112_2_18_2_19_25_56,"[MMIFM] ConvertStringToInt ret %d")
TRACE_MSG(MMIFM_APP_2303_112_2_18_2_19_25_57,"[MMIFM] MMIFM_GetNextValidChannelByFreq freq %d,step %d")
TRACE_MSG(MMIFM_EXPORT_179_112_2_18_2_19_26_58,"[MMIFM] MMIAPIFM_GetCurFreqName  str_ptr valid !")
TRACE_MSG(MMIFM_EXPORT_239_112_2_18_2_19_26_59,"[MMIFM] MMIAPIFM_StartFMTimerRecord  enter")
TRACE_MSG(MMIFM_EXPORT_277_112_2_18_2_19_26_60,"isplayFmName:fm_status %d is error!")
TRACE_MSG(MMIFM_EXPORT_293_112_2_18_2_19_26_61,"[MMIFM] MMIAPIFM_IsFmRecordActive  result %d")
TRACE_MSG(MMIFM_WINTAB_1655_112_2_18_2_19_30_62,"MMIFM_OpenMainWin")
TRACE_MSG(MMIFM_WINTAB_1678_112_2_18_2_19_30_63,"[MMIFM] [HandleFmMainMsg] msg_id = %x")
TRACE_MSG(MMIFM_WINTAB_1974_112_2_18_2_19_31_64,"[MMIFM] [HandleFmMainMsg] msg_id = %x")
TRACE_MSG(MMIFM_WINTAB_2332_112_2_18_2_19_32_65,"[MMIFM] HandleFmMainOptionMsg menu_id %d")
TRACE_MSG(MMIFM_WINTAB_2482_112_2_18_2_19_32_66,"[MMIFM] HandleChannelListWinMsg MSG_APP_OK s_current_sel_channel %d")
TRACE_MSG(MMIFM_WINTAB_2508_112_2_18_2_19_32_67,"[MMIFM] HandleChannelListWinMsg MSG_APP_WEB s_current_sel_channel %d")
TRACE_MSG(MMIFM_WINTAB_2642_112_2_18_2_19_33_68,"[MMIFM] HandleChannelListWinMsg MSG_APP_OK s_current_sel_channel %d")
TRACE_MSG(MMIFM_WINTAB_2672_112_2_18_2_19_33_69,"[MMIFM] HandleChannelListWinMsg MSG_APP_WEB s_current_sel_channel %d")
TRACE_MSG(MMIFM_WINTAB_2947_112_2_18_2_19_33_70,"[MMIFM] HandleEditBoxWinMsg MSG_APP_WEB s_current_operator %d")
TRACE_MSG(MMIFM_WINTAB_3011_112_2_18_2_19_33_71,"HandleEditBoxWinMsg OK s_current_operator %d !")
TRACE_MSG(MMIFM_WINTAB_3259_112_2_18_2_19_34_72,"[MMIFM] ConvertFloatStrToInt ret %d")
TRACE_MSG(MMIFM_WINTAB_3347_112_2_18_2_19_34_73,"[MMIFM] HandleFMAction act_type %d")
TRACE_MSG(MMIFM_WINTAB_3387_112_2_18_2_19_34_74,"[MMIFM] HandleFMAction act_type %d")
TRACE_MSG(MMIFM_WINTAB_3599_112_2_18_2_19_35_75,"[MMIFM] GetChannelDispName channel_ptr %d disp_name_ptr %d max_name_len %d !")
TRACE_MSG(MMIFM_WINTAB_4118_112_2_18_2_19_36_76,"HandleAutoSearchWinMsg  MSG_APP_CANCEL channel_index %d")
TRACE_MSG(MMIFM_WINTAB_4131_112_2_18_2_19_36_77,"[MMIFM]HandleAutoSearchWinMsg channel_index %d, s_search_ret %d")
TRACE_MSG(MMIFM_WINTAB_4210_112_2_18_2_19_36_78,"HandleSearchValidFreqWinMsg  MSG_APP_CANCEL")
TRACE_MSG(MMIFM_WINTAB_4220_112_2_18_2_19_36_79,"[MMIFM]HandleSearchValidFreqWinMsg s_search_ret %d")
TRACE_MSG(MMIFM_WINTAB_4258_112_2_18_2_19_36_80,"[MMIFM] DrawFrqqencyText PNULL == cur_info_ptr !")
TRACE_MSG(MMIFM_WINTAB_4307_112_2_18_2_19_36_81,"[MMIFM] DrawChannelName PNULL == cur_info_ptr !")
TRACE_MSG(MMIFM_WINTAB_4370_112_2_18_2_19_36_82,"[MMIFM] DrawChannelNumber PNULL == num_str_ptr !")
TRACE_MSG(MMIFM_WINTAB_4401_112_2_18_2_19_36_83,"[MMIFM] DrawFrqqencyText PNULL == cur_info_ptr !")
TRACE_MSG(MMIFM_WINTAB_5185_112_2_18_2_19_38_84,"[MMIFM] MMIFM_HandleHeadsetAction msg_id %d")
TRACE_MSG(MMIFM_WINTAB_5257_112_2_18_2_19_38_85,"SetCurrentSelChannel ch_no %d")
TRACE_MSG(MMIFM_WINTAB_5266_112_2_18_2_19_38_86,"GetCurrentSelChannel s_current_sel_channel %d")
TRACE_MSG(MMIFM_WINTAB_5277_112_2_18_2_19_38_87,"SetCurrentOperator s_current_operator %d")
TRACE_MSG(MMIFM_WINTAB_5286_112_2_18_2_19_38_88,"GetCurrentOperator s_current_operator %d")
TRACE_MSG(MMIFM_WINTAB_5631_112_2_18_2_19_39_89,"[MMIFM] HandleAlarmChannelListWinMsg MSG_APP_WEB s_current_sel_channel %d")
TRACE_MSG(MMIFM_WINTAB_5668_112_2_18_2_19_39_90,"[MMIFM] HandleAlarmChannelListWinMsg MSG_APP_WEB s_current_sel_channel %d")
TRACE_MSG(MMIFM_WINTAB_5874_112_2_18_2_19_39_91,"[MMIFMRECORD] HandleFmRecordMsg() msg_id = 0x%x")
TRACE_MSG(MMIFM_WINTAB_6052_112_2_18_2_19_40_92,"[MMIFMRECORD] HandleFmTimerRecordMsg() msg_id = 0x%x")
TRACE_MSG(MMIFM_WINTAB_6462_112_2_18_2_19_40_93,"FMGetRecordTime fail ! time_ptr = PNULL")
TRACE_MSG(MMIFM_WINTAB_6496_112_2_18_2_19_40_94,"[MMIFM] FMRecordStartRecord()")
TRACE_MSG(MMIFM_WINTAB_6536_112_2_18_2_19_41_95,"[MMIFM] FMRecordStartRecord() fs_error %d,error_code %d")
TRACE_MSG(MMIFM_WINTAB_6624_112_2_18_2_19_41_96,"MMIFM: HandleFmRecordRenameWinMsg, win_id =0x%x, msg_id = 0x%x.")
TRACE_MSG(MMIFM_WINTAB_8122_112_2_18_2_19_44_97,"mmi FMApplet_HandleEvent msg_id = 0x%04x")
END_TRACE_MAP(MMI_APP_FM_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_FM_TRC_H_

