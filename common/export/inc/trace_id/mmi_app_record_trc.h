/******************************************************************************
 ** File Name:      mmi_app_record_trc.h                                         *
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
//trace_id:217
#ifndef _MMI_APP_RECORD_TRC_H_
#define _MMI_APP_RECORD_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIRECORD_223_112_2_18_2_49_17_0 "MMIRECRD get file full path name file_name_len = %d"
#define MMIRECORD_264_112_2_18_2_49_17_1 "[IQ]MMIAPIRECORD_HandleRecordEnd has_enough_space %d"
#define MMIRECORD_411_112_2_18_2_49_17_2 "[MMIRECORD] MMIRECORD_CreateFileName()"
#define MMIRECORD_565_112_2_18_2_49_18_3 "[MMIRECORD] MMIAPIRECORD_StartRecordIncall current_state = %d"
#define MMIRECORD_592_112_2_18_2_49_18_4 "[MMIRECORD] MMIAPIRECORD_StartRecordIncall fs_ret = %d"
#define MMIRECORD_607_112_2_18_2_49_18_5 "[MMIRECORD] MMIAPIRECORD_StartRecordIncall() start record!"
#define MMIRECORD_622_112_2_18_2_49_18_6 "[MMIRECORD]:MMIAPIRECORD_StartRecordIncall SFS_DeleteFile ERROR!!!!!!!!!!"
#define MMIRECORD_640_112_2_18_2_49_18_7 "[MMIRECORD] MMIAPIRECORD_StartRecordIncall() cur_mode=%d"
#define MMIRECORD_672_112_2_18_2_49_18_8 "[MMIRECORD] MMIAPIRECORD_StartRecordIncall() aud_error = %d"
#define MMIRECORD_695_112_2_18_2_49_18_9 "[MMIRECORD] enter funciton MMIRECORD_StopRecord()"
#define MMIRECORD_699_112_2_18_2_49_18_10 "[MMIRECORD] MMIRECORD_StopRecord() error"
#define MMIRECORD_729_112_2_18_2_49_18_11 "[MMIRECORD] MMIRECORD_StopRecord() error: recording handle is 0!"
#define MMIRECORD_745_112_2_18_2_49_18_12 "[MMIRECORD]:MMIRECORD_CallBack:MMISRVAUD_REPORT_RESULT_E = %d"
#define MMIRECORD_767_112_2_18_2_49_18_13 "[MMIRECORD] enter funciton MMIRECORD_PauseRecord()"
#define MMIRECORD_782_112_2_18_2_49_18_14 "[MMIRECORD] enter funciton MMIRECORD_ResumeRecord()"
#define MMIRECORD_797_112_2_18_2_49_18_15 "[MMIRECORD] enter funciton CallbackWavPlay()"
#define MMIRECORD_812_112_2_18_2_49_18_16 "[MMIRECORD] enter funciton MMIRECORD_PausePlay()"
#define MMIRECORD_828_112_2_18_2_49_18_17 "[MMIRECORD] enter funciton MMIRECORD_ResumePlay()"
#define MMIRECORD_843_112_2_18_2_49_18_18 "[MMIRECORD] enter funciton MMIRECORD_StopPlay()"
#define MMIRECORD_865_112_2_18_2_49_18_19 "[MMIRECORD] MMIRECORD_PlayCurrent()"
#define MMIRECORD_931_112_2_18_2_49_18_20 "[MMIRECORD]:MMIRECORD_SetRecordStorage storage=%d "
#define MMIRECORD_941_112_2_18_2_49_18_21 "[MMIRECORD]:MMIRECORD_SetRecordFileFormatType file_formattype=%d "
#define MMIRECORD_951_112_2_18_2_49_18_22 "[MMIRECORD] MMIAPIRECORD_IsRecording: %d"
#define MMIRECORD_1097_112_2_18_2_49_19_23 "[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x"
#define MMIRECORD_1102_112_2_18_2_49_19_24 "[MMISRV]: HandleNotify(), report_ptr=0x%x"
#define MMIRECORD_1106_112_2_18_2_49_19_25 "[MMISRV]: HandleNotify(), report_ptr->report=%d"
#define MMIRECORD_1139_112_2_18_2_49_19_26 "[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x"
#define MMIRECORD_1144_112_2_18_2_49_19_27 "[MMISRV]: HandleNotify() report_ptr=0x%x"
#define MMIRECORD_1148_112_2_18_2_49_19_28 "[MMISRV]: HandleNotify(), report_ptr->report=%d"
#define MMIRECORD_1220_112_2_18_2_49_19_29 "[MMIRECORD] MMIRECORD_StartRecordNormal"
#define MMIRECORD_1225_112_2_18_2_49_19_30 "[MMIRECORD] MMIRECORD_StartRecordNormal current_state = %d"
#define MMIRECORD_1242_112_2_18_2_49_19_31 "[MMIRECORD] MMIRECORD_StartRecordNormal error_code=%d"
#define MMIRECORD_1266_112_2_18_2_49_19_32 "[MMIRECORD] record file name = %s"
#define MMIRECORD_1314_112_2_18_2_49_19_33 "[MMIRECORD] MMIAPIRECORD_GetRecordStatus(), status = %d"
#define MMIRECORD_1382_112_2_18_2_49_19_34 "[MMIRECORD] GetCurFileDeleteFlag(), s_is_cur_file_allow_deleted = %d"
#define MMIRECORD_1457_112_2_18_2_49_19_35 "[MMIRECORD]MMIRECORD_CheckSpace: UDISK free spaced high value is = %d free space low value is = %d"
#define MMIRECORD_1472_112_2_18_2_49_19_36 "[MMIRECORD]MMIRECORD_CheckSpace sfs_result = %d"
#define MMIRECORD_1481_112_2_18_2_49_19_37 "[MMIRECORD]MMIRECORD_CheckSpace record_result = %d"
#define MMIRECORD_1553_112_2_18_2_49_20_38 "[MMIRECORD] GetPreferDev1:file_dev=%d"
#define MMIRECORD_1560_112_2_18_2_49_20_39 "[MMIRECORD] GetPreferDev2:file_dev=%d"
#define MMIRECORD_1561_112_2_18_2_49_20_40 "[MMIRECORD] GetPreferDev3:file_max_dev=%d"
#define MMIRECORD_1565_112_2_18_2_49_20_41 "[MMIRECORD] GetPreferDev4:file_dev=%d"
#define MMIRECORD_1566_112_2_18_2_49_20_42 "[MMIRECORD] GetPreferDev5:file_max_dev=%d"
#define MMIRECORD_1584_112_2_18_2_49_20_43 "[MMIRECORD] GetFileIDString id >= id_max"
#define MMIRECORD_1615_112_2_18_2_49_20_44 "[MMIRECORD] GetCurrentDev:s_record_file_path=NULL"
#define MMIRECORD_WINTAB_645_112_2_18_2_49_22_45 "[MMIRECORD] HandleRecordMainPlayWinMsg() msg_id = 0x%x"
#define MMIRECORD_WINTAB_808_112_2_18_2_49_22_46 "[MMIRECORD]:MSG_RECORD_PLAY_CALL_BACK ERROR!!!!!!!!!!!!!1"
#define MMIRECORD_WINTAB_886_112_2_18_2_49_22_47 "[MMIRECORD]:HandleRecordMainPlayWinMsg time:%d"
#define MMIRECORD_WINTAB_901_112_2_18_2_49_22_48 "[MMIRECORD]:HandleRecordMainPlayWinMsg size:%d"
#define MMIRECORD_WINTAB_1205_112_2_18_2_49_23_49 "[MMIRECORD]:DisplayRecordTime s_is_play_end s_record_cur_timer_elapsed = %d"
#define MMIRECORD_WINTAB_1550_112_2_18_2_49_24_50 "[MMIRECORD]:MMIRECORD_UpdateButtonStatus RecordStatus ERROR!!!!!!!!!!!!!!"
#define MMIRECORD_WINTAB_1661_112_2_18_2_49_24_51 "[MMIRECORD]:MMIRECORD_UpdateButtonStatus RecordStatus ERROR!!!!!!!!!!!!!!"
#define MMIRECORD_WINTAB_1806_112_2_18_2_49_24_52 "[MMIRECORD]:DisplayRecordButton btn_id ERROR!!!!!!!!!!!!!!!!!!!"
#define MMIRECORD_WINTAB_1921_112_2_18_2_49_25_53 "[MMIRECORD]:DisplayRecordButton btn_id ERROR!!!!!!!!!!!!!!!!!!!"
#define MMIRECORD_WINTAB_2260_112_2_18_2_49_25_54 "[MMIRECORD] StartRecord() is_reRecord = %d"
#define MMIRECORD_WINTAB_3113_112_2_18_2_49_27_55 "[MMIRECORD]:SetLabelText RecordStatus s_record_cur_timer_elapsed = %d"
#define MMIRECORD_WINTAB_3114_112_2_18_2_49_27_56 "[MMIRECORD]:SetLabelText RecordStatus s_record_total_time = %d"
#define MMIRECORD_WINTAB_3765_112_2_18_2_49_28_57 "HandleFormSwitchActiveMsg  ERROR!"
#define MMIRECORD_WINTAB_3925_112_2_18_2_49_29_58 "MMIRECORD applet does not get win_id form the setting window "
#define MMIRECORD_WINTAB_4017_112_2_18_2_49_29_59 "[MMIRECORD]:DisplayRecordPointer record_param.db = %d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_RECORD_TRC)
TRACE_MSG(MMIRECORD_223_112_2_18_2_49_17_0,"MMIRECRD get file full path name file_name_len = %d")
TRACE_MSG(MMIRECORD_264_112_2_18_2_49_17_1,"[IQ]MMIAPIRECORD_HandleRecordEnd has_enough_space %d")
TRACE_MSG(MMIRECORD_411_112_2_18_2_49_17_2,"[MMIRECORD] MMIRECORD_CreateFileName()")
TRACE_MSG(MMIRECORD_565_112_2_18_2_49_18_3,"[MMIRECORD] MMIAPIRECORD_StartRecordIncall current_state = %d")
TRACE_MSG(MMIRECORD_592_112_2_18_2_49_18_4,"[MMIRECORD] MMIAPIRECORD_StartRecordIncall fs_ret = %d")
TRACE_MSG(MMIRECORD_607_112_2_18_2_49_18_5,"[MMIRECORD] MMIAPIRECORD_StartRecordIncall() start record!")
TRACE_MSG(MMIRECORD_622_112_2_18_2_49_18_6,"[MMIRECORD]:MMIAPIRECORD_StartRecordIncall SFS_DeleteFile ERROR!!!!!!!!!!")
TRACE_MSG(MMIRECORD_640_112_2_18_2_49_18_7,"[MMIRECORD] MMIAPIRECORD_StartRecordIncall() cur_mode=%d")
TRACE_MSG(MMIRECORD_672_112_2_18_2_49_18_8,"[MMIRECORD] MMIAPIRECORD_StartRecordIncall() aud_error = %d")
TRACE_MSG(MMIRECORD_695_112_2_18_2_49_18_9,"[MMIRECORD] enter funciton MMIRECORD_StopRecord()")
TRACE_MSG(MMIRECORD_699_112_2_18_2_49_18_10,"[MMIRECORD] MMIRECORD_StopRecord() error")
TRACE_MSG(MMIRECORD_729_112_2_18_2_49_18_11,"[MMIRECORD] MMIRECORD_StopRecord() error: recording handle is 0!")
TRACE_MSG(MMIRECORD_745_112_2_18_2_49_18_12,"[MMIRECORD]:MMIRECORD_CallBack:MMISRVAUD_REPORT_RESULT_E = %d")
TRACE_MSG(MMIRECORD_767_112_2_18_2_49_18_13,"[MMIRECORD] enter funciton MMIRECORD_PauseRecord()")
TRACE_MSG(MMIRECORD_782_112_2_18_2_49_18_14,"[MMIRECORD] enter funciton MMIRECORD_ResumeRecord()")
TRACE_MSG(MMIRECORD_797_112_2_18_2_49_18_15,"[MMIRECORD] enter funciton CallbackWavPlay()")
TRACE_MSG(MMIRECORD_812_112_2_18_2_49_18_16,"[MMIRECORD] enter funciton MMIRECORD_PausePlay()")
TRACE_MSG(MMIRECORD_828_112_2_18_2_49_18_17,"[MMIRECORD] enter funciton MMIRECORD_ResumePlay()")
TRACE_MSG(MMIRECORD_843_112_2_18_2_49_18_18,"[MMIRECORD] enter funciton MMIRECORD_StopPlay()")
TRACE_MSG(MMIRECORD_865_112_2_18_2_49_18_19,"[MMIRECORD] MMIRECORD_PlayCurrent()")
TRACE_MSG(MMIRECORD_931_112_2_18_2_49_18_20,"[MMIRECORD]:MMIRECORD_SetRecordStorage storage=%d ")
TRACE_MSG(MMIRECORD_941_112_2_18_2_49_18_21,"[MMIRECORD]:MMIRECORD_SetRecordFileFormatType file_formattype=%d ")
TRACE_MSG(MMIRECORD_951_112_2_18_2_49_18_22,"[MMIRECORD] MMIAPIRECORD_IsRecording: %d")
TRACE_MSG(MMIRECORD_1097_112_2_18_2_49_19_23,"[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x")
TRACE_MSG(MMIRECORD_1102_112_2_18_2_49_19_24,"[MMISRV]: HandleNotify(), report_ptr=0x%x")
TRACE_MSG(MMIRECORD_1106_112_2_18_2_49_19_25,"[MMISRV]: HandleNotify(), report_ptr->report=%d")
TRACE_MSG(MMIRECORD_1139_112_2_18_2_49_19_26,"[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x")
TRACE_MSG(MMIRECORD_1144_112_2_18_2_49_19_27,"[MMISRV]: HandleNotify() report_ptr=0x%x")
TRACE_MSG(MMIRECORD_1148_112_2_18_2_49_19_28,"[MMISRV]: HandleNotify(), report_ptr->report=%d")
TRACE_MSG(MMIRECORD_1220_112_2_18_2_49_19_29,"[MMIRECORD] MMIRECORD_StartRecordNormal")
TRACE_MSG(MMIRECORD_1225_112_2_18_2_49_19_30,"[MMIRECORD] MMIRECORD_StartRecordNormal current_state = %d")
TRACE_MSG(MMIRECORD_1242_112_2_18_2_49_19_31,"[MMIRECORD] MMIRECORD_StartRecordNormal error_code=%d")
TRACE_MSG(MMIRECORD_1266_112_2_18_2_49_19_32,"[MMIRECORD] record file name = %s")
TRACE_MSG(MMIRECORD_1314_112_2_18_2_49_19_33,"[MMIRECORD] MMIAPIRECORD_GetRecordStatus(), status = %d")
TRACE_MSG(MMIRECORD_1382_112_2_18_2_49_19_34,"[MMIRECORD] GetCurFileDeleteFlag(), s_is_cur_file_allow_deleted = %d")
TRACE_MSG(MMIRECORD_1457_112_2_18_2_49_19_35,"[MMIRECORD]MMIRECORD_CheckSpace: UDISK free spaced high value is = %d free space low value is = %d")
TRACE_MSG(MMIRECORD_1472_112_2_18_2_49_19_36,"[MMIRECORD]MMIRECORD_CheckSpace sfs_result = %d")
TRACE_MSG(MMIRECORD_1481_112_2_18_2_49_19_37,"[MMIRECORD]MMIRECORD_CheckSpace record_result = %d")
TRACE_MSG(MMIRECORD_1553_112_2_18_2_49_20_38,"[MMIRECORD] GetPreferDev1:file_dev=%d")
TRACE_MSG(MMIRECORD_1560_112_2_18_2_49_20_39,"[MMIRECORD] GetPreferDev2:file_dev=%d")
TRACE_MSG(MMIRECORD_1561_112_2_18_2_49_20_40,"[MMIRECORD] GetPreferDev3:file_max_dev=%d")
TRACE_MSG(MMIRECORD_1565_112_2_18_2_49_20_41,"[MMIRECORD] GetPreferDev4:file_dev=%d")
TRACE_MSG(MMIRECORD_1566_112_2_18_2_49_20_42,"[MMIRECORD] GetPreferDev5:file_max_dev=%d")
TRACE_MSG(MMIRECORD_1584_112_2_18_2_49_20_43,"[MMIRECORD] GetFileIDString id >= id_max")
TRACE_MSG(MMIRECORD_1615_112_2_18_2_49_20_44,"[MMIRECORD] GetCurrentDev:s_record_file_path=NULL")
TRACE_MSG(MMIRECORD_WINTAB_645_112_2_18_2_49_22_45,"[MMIRECORD] HandleRecordMainPlayWinMsg() msg_id = 0x%x")
TRACE_MSG(MMIRECORD_WINTAB_808_112_2_18_2_49_22_46,"[MMIRECORD]:MSG_RECORD_PLAY_CALL_BACK ERROR!!!!!!!!!!!!!1")
TRACE_MSG(MMIRECORD_WINTAB_886_112_2_18_2_49_22_47,"[MMIRECORD]:HandleRecordMainPlayWinMsg time:%d")
TRACE_MSG(MMIRECORD_WINTAB_901_112_2_18_2_49_22_48,"[MMIRECORD]:HandleRecordMainPlayWinMsg size:%d")
TRACE_MSG(MMIRECORD_WINTAB_1205_112_2_18_2_49_23_49,"[MMIRECORD]:DisplayRecordTime s_is_play_end s_record_cur_timer_elapsed = %d")
TRACE_MSG(MMIRECORD_WINTAB_1550_112_2_18_2_49_24_50,"[MMIRECORD]:MMIRECORD_UpdateButtonStatus RecordStatus ERROR!!!!!!!!!!!!!!")
TRACE_MSG(MMIRECORD_WINTAB_1661_112_2_18_2_49_24_51,"[MMIRECORD]:MMIRECORD_UpdateButtonStatus RecordStatus ERROR!!!!!!!!!!!!!!")
TRACE_MSG(MMIRECORD_WINTAB_1806_112_2_18_2_49_24_52,"[MMIRECORD]:DisplayRecordButton btn_id ERROR!!!!!!!!!!!!!!!!!!!")
TRACE_MSG(MMIRECORD_WINTAB_1921_112_2_18_2_49_25_53,"[MMIRECORD]:DisplayRecordButton btn_id ERROR!!!!!!!!!!!!!!!!!!!")
TRACE_MSG(MMIRECORD_WINTAB_2260_112_2_18_2_49_25_54,"[MMIRECORD] StartRecord() is_reRecord = %d")
TRACE_MSG(MMIRECORD_WINTAB_3113_112_2_18_2_49_27_55,"[MMIRECORD]:SetLabelText RecordStatus s_record_cur_timer_elapsed = %d")
TRACE_MSG(MMIRECORD_WINTAB_3114_112_2_18_2_49_27_56,"[MMIRECORD]:SetLabelText RecordStatus s_record_total_time = %d")
TRACE_MSG(MMIRECORD_WINTAB_3765_112_2_18_2_49_28_57,"HandleFormSwitchActiveMsg  ERROR!")
TRACE_MSG(MMIRECORD_WINTAB_3925_112_2_18_2_49_29_58,"MMIRECORD applet does not get win_id form the setting window ")
TRACE_MSG(MMIRECORD_WINTAB_4017_112_2_18_2_49_29_59,"[MMIRECORD]:DisplayRecordPointer record_param.db = %d")
END_TRACE_MAP(MMI_APP_RECORD_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_RECORD_TRC_H_

