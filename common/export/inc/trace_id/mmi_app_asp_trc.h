/******************************************************************************
 ** File Name:      mmi_app_asp_trc.h                                         *
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
//trace_id:148
#ifndef _MMI_APP_ASP_TRC_H_
#define _MMI_APP_ASP_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIASP_API_77_112_2_18_2_2_34_0 "[MMIASP]: MMIAPIASP_CreatWaitTimer stop wait_timer_id = %d"
#define MMIASP_API_82_112_2_18_2_2_34_1 "[MMIASP]: MMIAPIASP_CreatWaitTimer wait_timer_id = %d"
#define MMIASP_API_86_112_2_18_2_2_34_2 "[MMIASP]: MMIAPIASP_CreatWaitTimer applet_ptr == PNULL"
#define MMIASP_API_91_112_2_18_2_2_34_3 "[MMIASP]: MMIAPIASP_CreatWaitTimer not suitable for asp !!"
#define MMIASP_API_96_112_2_18_2_2_34_4 "[MMIASP]: machine not turn on"
#define MMIASP_API_112_112_2_18_2_2_34_5 "[MMIASP]: MMIAPIASP_StopWaitTimer wait_timer_id = %d"
#define MMIASP_API_136_112_2_18_2_2_34_6 "MMIASP MMIAPIASP_IsWaitTimeout APPLET has not started"
#define MMIASP_API_149_112_2_18_2_2_34_7 "MMIASP MMIAPIASP_IsMachineWorking work = %d"
#define MMIASP_API_183_112_2_18_2_2_34_8 "MMIASP MMIAPIASP_EnterIntoAsp entered"
#define MMIASP_API_217_112_2_18_2_2_35_9 "MMIASP MMIAPIASP_WorkOneTime not suitable for answer asp"
#define MMIASP_API_238_112_2_18_2_2_35_10 "MMIASP MMIAPIASP_CloseRecordingWin entered"
#define MMIASP_API_291_112_2_18_2_2_35_11 "[MMIASP]: MMIAPIASP_StopCallAnswer enter !"
#define MMIASP_API_301_112_2_18_2_2_35_12 "[MMIASP]: MMIAPIASP_RealseCall enter !"
#define MMIASP_API_314_112_2_18_2_2_35_13 "MMIASP MMIAPIASP_lsCallSpeakedByOwner call state = %d"
#define MMIASP_API_331_112_2_18_2_2_35_14 "MMIASP MMIAPIASP_lsAspCallSpeaking call state = %d"
#define MMIASP_API_478_112_2_18_2_2_35_15 "MMIASP applet is not set answer ring as recording ring "
#define MMIASP_API_517_112_2_18_2_2_35_16 "[MMIASP]: MMIAPIASP_StopAspRecording enter !"
#define MMIASP_CORTROL_LAYER_245_112_2_18_2_2_36_17 "MMIASP MMIASP_NvInit NULL applet"
#define MMIASP_CORTROL_LAYER_260_112_2_18_2_2_36_18 "[MMIASP]: GetPlayStaus: 0x%.2x"
#define MMIASP_CORTROL_LAYER_265_112_2_18_2_2_36_19 "[MMIASP]: GetPlayStaus: no applet"
#define MMIASP_CORTROL_LAYER_281_112_2_18_2_2_36_20 "[MMIASP]: SetPlayStatus: 0x%.2x"
#define MMIASP_CORTROL_LAYER_286_112_2_18_2_2_36_21 "[MMIASP]: SetPlayStatus: no applet"
#define MMIASP_CORTROL_LAYER_296_112_2_18_2_2_36_22 "[MMIASP] PlayCallBack enter ."
#define MMIASP_CORTROL_LAYER_316_112_2_18_2_2_36_23 "[MMIASP] Pause enter"
#define MMIASP_CORTROL_LAYER_333_112_2_18_2_2_36_24 "[MMIASP] Resume enter"
#define MMIASP_CORTROL_LAYER_350_112_2_18_2_2_36_25 "[MMIASP] Stop enter"
#define MMIASP_CORTROL_LAYER_371_112_2_18_2_2_36_26 "[MMIASP]: Play enter "
#define MMIASP_CORTROL_LAYER_393_112_2_18_2_2_36_27 "[MMIASP]: Play creat handle fail "
#define MMIASP_CORTROL_LAYER_403_112_2_18_2_2_36_28 "[MMIASP]: Play audio fail "
#define MMIASP_CORTROL_LAYER_417_112_2_18_2_2_36_29 "[MMIASP] MMIASP_Play() result=%d"
#define MMIASP_CORTROL_LAYER_565_112_2_18_2_2_37_30 "[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x"
#define MMIASP_CORTROL_LAYER_613_112_2_18_2_2_37_31 "[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x"
#define MMIASP_CORTROL_LAYER_663_112_2_18_2_2_37_32 "[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x"
#define MMIASP_CORTROL_LAYER_714_112_2_18_2_2_37_33 "[MMISRV]: MMIASP_AudioPlay() entry, handle=%d"
#define MMIASP_CORTROL_LAYER_722_112_2_18_2_2_37_34 "[MMISRV]: MMIASP_AudioPlay() exit, res=%d"
#define MMIASP_CORTROL_LAYER_738_112_2_18_2_2_37_35 "[MMIASP]: MMIASP_StopRecordTimer record_timer_id     = %d "
#define MMIASP_CORTROL_LAYER_745_112_2_18_2_2_37_36 "[MMIASP]: MMIASP_StopRecordTimer record_timer_id     = %d "
#define MMIASP_CORTROL_LAYER_752_112_2_18_2_2_37_37 "[MMIASP]: MMIASP_StopRecordTimer: no applet!!!"
#define MMIASP_CORTROL_LAYER_766_112_2_18_2_2_37_38 "[MMIASP] MMIASP_Prev() enter!"
#define MMIASP_CORTROL_LAYER_803_112_2_18_2_2_37_39 "[MMIASP] MMIASP_Next() enter!"
#define MMIASP_CORTROL_LAYER_854_112_2_18_2_2_37_40 "[MMIASP]: MMIASP_MsgListFree enter !"
#define MMIASP_CORTROL_LAYER_862_112_2_18_2_2_37_41 "MMIASP MMIASP_MsgListFree no applet"
#define MMIASP_CORTROL_LAYER_873_112_2_18_2_2_37_42 "[MMIASP]: MMIASP_MsgListFree record_path !"
#define MMIASP_CORTROL_LAYER_897_112_2_18_2_2_38_43 "[MMIASP]: MMIASP_MsgListFree enter !"
#define MMIASP_CORTROL_LAYER_954_112_2_18_2_2_38_44 " [MMIASP]:MMIASP_AddMsg2ListEnd enter!"
#define MMIASP_CORTROL_LAYER_990_112_2_18_2_2_38_45 " [MMIASP]:AddMsgToListHeader enter!"
#define MMIASP_CORTROL_LAYER_1030_112_2_18_2_2_38_46 " [MMIASP] MMIASP_GetListMsgTotalNum list_num = %d"
#define MMIASP_CORTROL_LAYER_1036_112_2_18_2_2_38_47 " [MMIASP] MMIASP_GetListMsgTotalNum no applet!!"
#define MMIASP_CORTROL_LAYER_1095_112_2_18_2_2_38_48 " [MMIASP] MMIASP_DelMarkNodeFromList no applet!!"
#define MMIASP_CORTROL_LAYER_1119_112_2_18_2_2_38_49 "MMIASP GetCurNodeByGuiIndex list_index = %d"
#define MMIASP_CORTROL_LAYER_1131_112_2_18_2_2_38_50 " [MMIASP] MMIASP_GetCurNodeByGuiIndex no applet!!"
#define MMIASP_CORTROL_LAYER_1185_112_2_18_2_2_38_51 "[MMIASP]: MMIASP_CheckVersionNumber result = %d "
#define MMIASP_CORTROL_LAYER_1229_112_2_18_2_2_38_52 "[MMIASP]:MMIASP_CreateNewSysFile file_handle = %d,result = %d"
#define MMIASP_CORTROL_LAYER_1281_112_2_18_2_2_38_53 " [MMIASP]: MMIASP_ReadSysFile list_num = %d"
#define MMIASP_CORTROL_LAYER_1288_112_2_18_2_2_38_54 " [MMIASP]: MMIASP_ReadSysFile read filename fail ---"
#define MMIASP_CORTROL_LAYER_1293_112_2_18_2_2_38_55 " [MMIASP]: MMIASP_ReadSysFile read file_path_len:%d  ---"
#define MMIASP_CORTROL_LAYER_1301_112_2_18_2_2_38_56 " [MMIASP]: MMIASP_ReadSysFile read file_path_len fail = %d"
#define MMIASP_CORTROL_LAYER_1309_112_2_18_2_2_38_57 "ReadASPSysFile alloc failed"
#define MMIASP_CORTROL_LAYER_1316_112_2_18_2_2_38_58 " [MMIASP]: read filename fail ---"
#define MMIASP_CORTROL_LAYER_1324_112_2_18_2_2_38_59 " [MMIASP]: MMIASP_ReadSysFile read file_path_str:%s  ---"
#define MMIASP_CORTROL_LAYER_1341_112_2_18_2_2_38_60 " [MMIASP]: MMIASP_ReadSysFile read structure fail ---"
#define MMIASP_CORTROL_LAYER_1360_112_2_18_2_2_38_61 " [MMIASP]: MMIASP_ReadSysFile write_offset = %d,total_file_size = %d"
#define MMIASP_CORTROL_LAYER_1373_112_2_18_2_2_38_62 " [MMIASP]: MMIASP_ReadSysFile read sys file head fail ---"
#define MMIASP_CORTROL_LAYER_1388_112_2_18_2_2_39_63 "MMIASP MMIASP_ReadSysFileWithApplet no applet"
#define MMIASP_CORTROL_LAYER_1433_112_2_18_2_2_39_64 "MMIASP MMIASP_WriteSysFile no applet"
#define MMIASP_CORTROL_LAYER_1456_112_2_18_2_2_39_65 " [MMIASP]: MMIASP_WriteSysFile s_answer_machine_version fail ---"
#define MMIASP_CORTROL_LAYER_1481_112_2_18_2_2_39_66 " [MMIASP]: MMIASP_WriteSysFile list_num = %d"
#define MMIASP_CORTROL_LAYER_1492_112_2_18_2_2_39_67 " [MMIASP]: MMIASP_WriteSysFile write filename fail ---"
#define MMIASP_CORTROL_LAYER_1501_112_2_18_2_2_39_68 " [MMIASP]: MMIASP_WriteSysFile write filename fail ---"
#define MMIASP_CORTROL_LAYER_1509_112_2_18_2_2_39_69 " [MMIASP]: MMIASP_WriteSysFile write structure fail ---"
#define MMIASP_CORTROL_LAYER_1550_112_2_18_2_2_39_70 " [MMIASP] MMIASP_GetRecordFileDev file_dev=%d "
#define MMIASP_CORTROL_LAYER_1573_112_2_18_2_2_39_71 "[MMIASP]:  no suitable dev"
#define MMIASP_CORTROL_LAYER_1585_112_2_18_2_2_39_72 "[MMIASP]: MMIASP_StartRecord file_dev = %d"
#define MMIASP_CORTROL_LAYER_1588_112_2_18_2_2_39_73 "MMIASP StartRecord file_dev= %d"
#define MMIASP_CORTROL_LAYER_1596_112_2_18_2_2_39_74 "[MMIASP]: MMIASP_StartRecord cur_full_path = %s,cur_file_name_len = %d"
#define MMIASP_CORTROL_LAYER_1605_112_2_18_2_2_39_75 "[MMIASP]: MMIASP_StartRecord MMISRVAUD_SetUpLinkMute success"
#define MMIASP_CORTROL_LAYER_1638_112_2_18_2_2_39_76 "MMIASP StartRecord malloc memory fail ERROR!!!!!!!!!"
#define MMIASP_CORTROL_LAYER_1644_112_2_18_2_2_39_77 "MMIASP StartRecord fail, please check"
#define MMIASP_CORTROL_LAYER_1651_112_2_18_2_2_39_78 "[MMIASP]: MMIASP_StartRecord result = %d"
#define MMIASP_CORTROL_LAYER_1674_112_2_18_2_2_39_79 "MMIASP GetDefaultAnswerRingFullName entered"
#define MMIASP_CORTROL_LAYER_1677_112_2_18_2_2_39_80 "MMIASP nv set file_name_len = %d"
#define MMIASP_CORTROL_LAYER_1711_112_2_18_2_2_39_81 "[MMIASP]:MMIASP_PlayAudioInCall open background music!"
#define MMIASP_CORTROL_LAYER_1714_112_2_18_2_2_39_82 "[MMIASP]:MMIASP_PlayAudioInCall  PARA HAS error !!"
#define MMIASP_CORTROL_LAYER_1720_112_2_18_2_2_39_83 "[MMIASP]:MMIAPIASP_Play4Client:call_num is null!"
#define MMIASP_CORTROL_LAYER_1727_112_2_18_2_2_39_84 "MMIASP_PlayAudioInCall:MMICC_EnableMute SET NOT MUTE FAIL!"
#define MMIASP_CORTROL_LAYER_1741_112_2_18_2_2_39_85 "[MMIASP]:MMIASP_PlayAudioInCall audio_type = %d"
#define MMIASP_CORTROL_LAYER_1747_112_2_18_2_2_39_86 "MMIASP has set own answer ring"
#define MMIASP_CORTROL_LAYER_1755_112_2_18_2_2_39_87 "[MMIASP]: is_fixed_ring = %d, ring_id = %d"
#define MMIASP_CORTROL_LAYER_1761_112_2_18_2_2_39_88 "MMIASP ring id = %d"
#define MMIASP_CORTROL_LAYER_1771_112_2_18_2_2_39_89 "[MMIASP]:MMIASP_PlayAudioInCall data_len=%d, data_ptr=0x%x"
#define MMIASP_CORTROL_LAYER_1787_112_2_18_2_2_39_90 "MMIASP name len = 0"
#define MMIASP_CORTROL_LAYER_1792_112_2_18_2_2_39_91 "[MMIASP]:  play MORE ring file name = %s"
#define MMIASP_CORTROL_LAYER_1793_112_2_18_2_2_39_92 "[MMIASP]:  play MORE ring file name len= %d"
#define MMIASP_CORTROL_LAYER_1804_112_2_18_2_2_39_93 "[MMIASP]:applet_ptr->bkmusic_audio_handle = %d"
#define MMIASP_CORTROL_LAYER_1828_112_2_18_2_2_39_94 "[MMIASP]:MMIAPIASP_Play4Client play_ret = %d"
#define MMIASP_CORTROL_LAYER_1872_112_2_18_2_2_40_95 "[MMIASP]: MMIASP_GetNewMsgNum new_msg_num=%d"
#define MMIASP_CORTROL_LAYER_1936_112_2_18_2_2_40_96 "MMIASP GetPbNumIdByTelNum find NULL head OR call number error = %x"
#define MMIASP_CORTROL_LAYER_2024_112_2_18_2_2_40_97 "MMIASP CombineASPFilePath entered"
#define MMIASP_CORTROL_LAYER_2046_112_2_18_2_2_40_98 "MMIASP suffix_id = %d"
#define MMIASP_CORTROL_LAYER_2050_112_2_18_2_2_40_99 "MMIASP pb_num_len = %d"
#define MMIASP_CORTROL_LAYER_2149_112_2_18_2_2_40_100 "MMIASP MMIASP_SetCurMsgInfo entered"
#define MMIASP_CORTROL_LAYER_2247_112_2_18_2_2_40_101 "[MMIASP] start asp applet return null!!"
#define MMIASP_CORTROL_LAYER_2272_112_2_18_2_2_40_102 " [MMIASP] MMIASP_CloseAspApplet exit !"
#define MMIASP_CORTROL_LAYER_2354_112_2_18_2_2_40_103 "MMIASP GetFixedAnswerRing GET NV "
#define MMIASP_CORTROL_LAYER_2420_112_2_18_2_2_41_104 "MMIASP GetCustomerAnswerRing has not set"
#define MMIASP_CORTROL_LAYER_2425_112_2_18_2_2_41_105 "MMIASP GetCustomerAnswerRing convert to bcd fail"
#define MMIASP_CORTROL_LAYER_2486_112_2_18_2_2_41_106 "[MMIASP]: MMIASP_StopAnswerCall enter !"
#define MMIASP_CORTROL_LAYER_2487_112_2_18_2_2_41_107 "[MMIASP]: MMIASP_StopAnswerCall woring flag = %d"
#define MMIASP_CORTROL_LAYER_2491_112_2_18_2_2_41_108 "[MMIASP]: MMIASP_StopAnswerCall really enter!!"
#define MMIASP_CORTROL_LAYER_2529_112_2_18_2_2_41_109 "[MMIASP]: CreateRecordTimer record_time     = %d "
#define MMIASP_CORTROL_LAYER_2530_112_2_18_2_2_41_110 "[MMIASP]: CreateRecordTimer record_timer_id = %d "
#define MMIASP_CORTROL_LAYER_2534_112_2_18_2_2_41_111 "[MMIASP]: CreateRecordTimer applet is NULL !!"
#define MMIASP_CORTROL_LAYER_2549_112_2_18_2_2_41_112 "[MMIASP]: StopPlayAudio enter !"
#define MMIASP_CORTROL_LAYER_2560_112_2_18_2_2_41_113 "[MMIASP]: MMIASP_StopAudioForClient MMISRVAUD_Stop result = %d,MMISRVMGR_Free result:%d"
#define MMIASP_CORTROL_LAYER_2598_112_2_18_2_2_41_114 "[MMIASP] GetSysFileFullNameForWrite result = %d"
#define MMIASP_CORTROL_LAYER_2626_112_2_18_2_2_41_115 "[MMIASP] GetSysFileFullNameForRead result = %d"
#define MMIASP_CORTROL_LAYER_2652_112_2_18_2_2_41_116 "MMIASP SetMechineWorkingFlag = %d"
#define MMIASP_CORTROL_LAYER_2660_112_2_18_2_2_41_117 "[MMIASP]: MMIAPIASP_RealseCall enter !"
#define MMIASP_CORTROL_LAYER_2673_112_2_18_2_2_41_118 "MMIASP MMIASP_SetCallState set call_state = %d"
#define MMIASP_CORTROL_LAYER_2680_112_2_18_2_2_41_119 "MMIASP applet is not worked "
#define MMIASP_CORTROL_LAYER_2691_112_2_18_2_2_41_120 "MMIASP MMIASP_GetCallState enterd"
#define MMIASP_CORTROL_LAYER_2699_112_2_18_2_2_41_121 "MMIASP applet is not worked "
#define MMIASP_CORTROL_LAYER_2715_112_2_18_2_2_41_122 "GetAllPathEnoughFreeSpace false"
#define MMIASP_CORTROL_LAYER_2719_112_2_18_2_2_41_123 "GetAllPathEnoughFreeSpace true"
#define MMIASP_CORTROL_LAYER_2735_112_2_18_2_2_41_124 "[MMIASP]: GetCuurPathEnoughFreeSpace:file_dev = %d"
#define MMIASP_CORTROL_LAYER_2749_112_2_18_2_2_41_125 "[MMIASP]: GetCuurPathEnoughFreeSpace:free_enough = %d"
#define MMIASP_CORTROL_LAYER_2815_112_2_18_2_2_41_126 "[MMIASP]: Record_CallBack:MMISRVAUD_REPORT_RESULT_E = %d"
#define MMIASP_CORTROL_LAYER_2869_112_2_18_2_2_42_127 "InsertReadRecordList fail"
#define MMIASP_CORTROL_LAYER_2932_112_2_18_2_2_42_128 "MMIASP MMIASP_SortRecordList entered"
#define MMIASP_CORTROL_LAYER_2936_112_2_18_2_2_42_129 "MMIASP no applet!!"
#define MMIASP_CORTROL_LAYER_2942_112_2_18_2_2_42_130 "MMIASP find null list"
#define MMIASP_CORTROL_LAYER_2947_112_2_18_2_2_42_131 "MMIASP no new node"
#define MMIASP_CORTROL_LAYER_3160_112_2_18_2_2_42_132 "[MMIASP]: MMIASP_StartPlayAnswerMusic enter !"
#define MMIASP_CORTROL_LAYER_3163_112_2_18_2_2_42_133 " [MMIASP] MMIASP_StartAnswerCall Not applet !"
#define MMIASP_CORTROL_LAYER_3265_112_2_18_2_2_42_134 "[MMIASP]: CreateRecordTimer applet is NULL !!"
#define MMIASP_CORTROL_LAYER_3285_112_2_18_2_2_42_135 "[MMIASP]: MMIASP_StopRecordTimer: no applet!!!"
#define MMIASP_WINTAB_869_112_2_18_2_2_45_136 "MMIASP InitButtonOperation no applet"
#define MMIASP_WINTAB_935_112_2_18_2_2_45_137 "[MMIASP]:OKCallback, cur index = %d "
#define MMIASP_WINTAB_940_112_2_18_2_2_45_138 "[MMIASP]:PlayPauseKeyMsg, cur state = %d error!"
#define MMIASP_WINTAB_1075_112_2_18_2_2_46_139 "MMIASP UpdateBackwardForwardButtonFg no aopplet"
#define MMIASP_WINTAB_1180_112_2_18_2_2_46_140 "MMIASP HandleMsgListWinMsg MSGid = %x"
#define MMIASP_WINTAB_1378_112_2_18_2_2_46_141 "MMIASP UpdatePlayDetailInfo no applet!!!"
#define MMIASP_WINTAB_1386_112_2_18_2_2_46_142 "MMIASP UpdatePlayDetailInfo cur index= %d!!!"
#define MMIASP_WINTAB_1446_112_2_18_2_2_46_143 "MMIASP total time = %d"
#define MMIASP_WINTAB_1665_112_2_18_2_2_47_144 "MMIASP UpdatePlayDetailInfo no applet!!!"
#define MMIASP_WINTAB_2039_112_2_18_2_2_47_145 "[MMIASP]: HandleProgressBarTpKey, tp_x= %d"
#define MMIASP_WINTAB_2050_112_2_18_2_2_47_146 "[MMIASP]: HandleProgressBarTpKey, percent = %d,seek_offset = %d, result = %d"
#define MMIASP_WINTAB_2074_112_2_18_2_2_48_147 "[MMIASP]: StartTpDragTimer"
#define MMIASP_WINTAB_2082_112_2_18_2_2_48_148 "[MMIASP]: StartTpDragTimer: the timer has started!"
#define MMIASP_WINTAB_2099_112_2_18_2_2_48_149 "[MMIASP]: StopTpDragTimer: the timer has stop!"
#define MMIASP_WINTAB_2116_112_2_18_2_2_48_150 "[MMIASP]: HandlePlayWinTpKeyDown"
#define MMIASP_WINTAB_2159_112_2_18_2_2_48_151 "MMIASP HandlePlayWinMsg MSG_ID = %d"
#define MMIASP_WINTAB_2172_112_2_18_2_2_48_152 "MMIASP play win null code , index = %d"
#define MMIASP_WINTAB_2255_112_2_18_2_2_48_153 "[MMIASP]: HandlePlayWinMsg MSG_RECORD_PLAY_CALL_BACK "
#define MMIASP_WINTAB_2262_112_2_18_2_2_48_154 "[MMIASP]: HandlePlayWinMsg *result_ptr = %d"
#define MMIASP_WINTAB_2293_112_2_18_2_2_48_155 "MMIASP MSG_ASP_PLAY_CALL_BACK return msg"
#define MMIASP_WINTAB_2384_112_2_18_2_2_48_156 "MMIASP please check applet!!"
#define MMIASP_WINTAB_2984_112_2_18_2_2_49_157 "MMIASP please check applet"
#define MMIASP_WINTAB_3026_112_2_18_2_2_49_158 "MMIASP AppendRecordList no applet"
#define MMIASP_WINTAB_3032_112_2_18_2_2_49_159 "AppendRecordList ASP read sys file fail"
#define MMIASP_WINTAB_3174_112_2_18_2_2_50_160 "MMIASP MMIASP_ShowNewAspMsgWin msg_count = %d"
#define MMIASP_WINTAB_3306_112_2_18_2_2_50_161 "MMIASP SetListMarkMenu has error!"
#define MMIASP_WINTAB_3471_112_2_18_2_2_50_162 "MMIASP HandleAspMenuOperation index = %d"
#define MMIASP_WINTAB_3480_112_2_18_2_2_50_163 "HandleAspMenuOperation null Node"
#define MMIASP_WINTAB_3483_112_2_18_2_2_50_164 "MMIASP num_len  = %d"
#define MMIASP_WINTAB_4243_112_2_18_2_2_52_165 "no applet, please check!!"
#define MMIASP_WINTAB_4252_112_2_18_2_2_52_166 "MMIASP HandleRingSelectWinMsg has callback function now"
#define MMIASP_WINTAB_4258_112_2_18_2_2_52_167 "MMIASP HandleRingSelectWinMsg MULTIM_SELECTED_RETURN null para"
#define MMIASP_WINTAB_4466_112_2_18_2_2_52_168 "[MMIASP] AspAppletHandleEvent start app only..."
#define MMIASP_WINTAB_4472_112_2_18_2_2_52_169 "[MMIASP] AspAppletHandleEvent MSG_APPLET_STOP"
#define MMIASP_WINTAB_4625_112_2_18_2_2_53_170 "MMIASP HandleRecordingWinMsg OPEN but applet is null"
#define MMIASP_WINTAB_4693_112_2_18_2_2_53_171 "HandleRecordingWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID"
#define MMIASP_WINTAB_4702_112_2_18_2_2_53_172 "HandleRecordingWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID"
#define MMIASP_WINTAB_4721_112_2_18_2_2_53_173 "HandleRecordingWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID"
#define MMIASP_WINTAB_5128_112_2_18_2_2_54_174 "MMIASP HandleMSGDeleteOperation no applet"
#define MMIASP_WINTAB_5162_112_2_18_2_2_54_175 "MMIASP MMIASP_OpenPlayWinFromExternal node_index = %d"
#define MMIASP_WINTAB_5166_112_2_18_2_2_54_176 "MMIASP MMIASP_OpenPlayWinFromExternal find no applet"
#define MMIASP_WINTAB_5173_112_2_18_2_2_54_177 "MMIASP error index"
#define MMIASP_WINTAB_5192_112_2_18_2_2_54_178 "MMIASP MMIASP_OpenPlayWinFromExternal no node"
#define MMIASP_WINTAB_5224_112_2_18_2_2_54_179 "MMIASP AppendOneListItem item_index = %d"
#define MMIASP_WINTAB_5227_112_2_18_2_2_54_180 "MMIASP AppendOneListItem no applet"
#define MMIASP_WINTAB_5234_112_2_18_2_2_54_181 "MMIASP AppendOneListItem no node item_index = %d"
#define MMIASP_WINTAB_5258_112_2_18_2_2_54_182 "MMIASP this number has find a name "
#define MMIASP_WINTAB_5305_112_2_18_2_2_54_183 "MMIASP_AutoPlayRecordMessages find no applet"
#define MMIASP_WINTAB_5314_112_2_18_2_2_54_184 "MMIASP error index"
#define MMIASP_WINTAB_5377_112_2_18_2_2_54_185 "MMIASP HandleAspMenuOperation index = %d"
#define MMIASP_WINTAB_5386_112_2_18_2_2_54_186 "HandleAspMenuOperation null Node"
#define MMIASP_WINTAB_5389_112_2_18_2_2_54_187 "MMIASP num_len  = %d"
#define MMIASP_WINTAB_5458_112_2_18_2_2_54_188 "[ASP] HandleLocalListLongOkOptMenuWinMsg menu_id = %x"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_ASP_TRC)
TRACE_MSG(MMIASP_API_77_112_2_18_2_2_34_0,"[MMIASP]: MMIAPIASP_CreatWaitTimer stop wait_timer_id = %d")
TRACE_MSG(MMIASP_API_82_112_2_18_2_2_34_1,"[MMIASP]: MMIAPIASP_CreatWaitTimer wait_timer_id = %d")
TRACE_MSG(MMIASP_API_86_112_2_18_2_2_34_2,"[MMIASP]: MMIAPIASP_CreatWaitTimer applet_ptr == PNULL")
TRACE_MSG(MMIASP_API_91_112_2_18_2_2_34_3,"[MMIASP]: MMIAPIASP_CreatWaitTimer not suitable for asp !!")
TRACE_MSG(MMIASP_API_96_112_2_18_2_2_34_4,"[MMIASP]: machine not turn on")
TRACE_MSG(MMIASP_API_112_112_2_18_2_2_34_5,"[MMIASP]: MMIAPIASP_StopWaitTimer wait_timer_id = %d")
TRACE_MSG(MMIASP_API_136_112_2_18_2_2_34_6,"MMIASP MMIAPIASP_IsWaitTimeout APPLET has not started")
TRACE_MSG(MMIASP_API_149_112_2_18_2_2_34_7,"MMIASP MMIAPIASP_IsMachineWorking work = %d")
TRACE_MSG(MMIASP_API_183_112_2_18_2_2_34_8,"MMIASP MMIAPIASP_EnterIntoAsp entered")
TRACE_MSG(MMIASP_API_217_112_2_18_2_2_35_9,"MMIASP MMIAPIASP_WorkOneTime not suitable for answer asp")
TRACE_MSG(MMIASP_API_238_112_2_18_2_2_35_10,"MMIASP MMIAPIASP_CloseRecordingWin entered")
TRACE_MSG(MMIASP_API_291_112_2_18_2_2_35_11,"[MMIASP]: MMIAPIASP_StopCallAnswer enter !")
TRACE_MSG(MMIASP_API_301_112_2_18_2_2_35_12,"[MMIASP]: MMIAPIASP_RealseCall enter !")
TRACE_MSG(MMIASP_API_314_112_2_18_2_2_35_13,"MMIASP MMIAPIASP_lsCallSpeakedByOwner call state = %d")
TRACE_MSG(MMIASP_API_331_112_2_18_2_2_35_14,"MMIASP MMIAPIASP_lsAspCallSpeaking call state = %d")
TRACE_MSG(MMIASP_API_478_112_2_18_2_2_35_15,"MMIASP applet is not set answer ring as recording ring ")
TRACE_MSG(MMIASP_API_517_112_2_18_2_2_35_16,"[MMIASP]: MMIAPIASP_StopAspRecording enter !")
TRACE_MSG(MMIASP_CORTROL_LAYER_245_112_2_18_2_2_36_17,"MMIASP MMIASP_NvInit NULL applet")
TRACE_MSG(MMIASP_CORTROL_LAYER_260_112_2_18_2_2_36_18,"[MMIASP]: GetPlayStaus: 0x%.2x")
TRACE_MSG(MMIASP_CORTROL_LAYER_265_112_2_18_2_2_36_19,"[MMIASP]: GetPlayStaus: no applet")
TRACE_MSG(MMIASP_CORTROL_LAYER_281_112_2_18_2_2_36_20,"[MMIASP]: SetPlayStatus: 0x%.2x")
TRACE_MSG(MMIASP_CORTROL_LAYER_286_112_2_18_2_2_36_21,"[MMIASP]: SetPlayStatus: no applet")
TRACE_MSG(MMIASP_CORTROL_LAYER_296_112_2_18_2_2_36_22,"[MMIASP] PlayCallBack enter .")
TRACE_MSG(MMIASP_CORTROL_LAYER_316_112_2_18_2_2_36_23,"[MMIASP] Pause enter")
TRACE_MSG(MMIASP_CORTROL_LAYER_333_112_2_18_2_2_36_24,"[MMIASP] Resume enter")
TRACE_MSG(MMIASP_CORTROL_LAYER_350_112_2_18_2_2_36_25,"[MMIASP] Stop enter")
TRACE_MSG(MMIASP_CORTROL_LAYER_371_112_2_18_2_2_36_26,"[MMIASP]: Play enter ")
TRACE_MSG(MMIASP_CORTROL_LAYER_393_112_2_18_2_2_36_27,"[MMIASP]: Play creat handle fail ")
TRACE_MSG(MMIASP_CORTROL_LAYER_403_112_2_18_2_2_36_28,"[MMIASP]: Play audio fail ")
TRACE_MSG(MMIASP_CORTROL_LAYER_417_112_2_18_2_2_36_29,"[MMIASP] MMIASP_Play() result=%d")
TRACE_MSG(MMIASP_CORTROL_LAYER_565_112_2_18_2_2_37_30,"[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x")
TRACE_MSG(MMIASP_CORTROL_LAYER_613_112_2_18_2_2_37_31,"[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x")
TRACE_MSG(MMIASP_CORTROL_LAYER_663_112_2_18_2_2_37_32,"[MMISRV]: HandleNotify() entry, handle=0x%x, param=0x%x")
TRACE_MSG(MMIASP_CORTROL_LAYER_714_112_2_18_2_2_37_33,"[MMISRV]: MMIASP_AudioPlay() entry, handle=%d")
TRACE_MSG(MMIASP_CORTROL_LAYER_722_112_2_18_2_2_37_34,"[MMISRV]: MMIASP_AudioPlay() exit, res=%d")
TRACE_MSG(MMIASP_CORTROL_LAYER_738_112_2_18_2_2_37_35,"[MMIASP]: MMIASP_StopRecordTimer record_timer_id     = %d ")
TRACE_MSG(MMIASP_CORTROL_LAYER_745_112_2_18_2_2_37_36,"[MMIASP]: MMIASP_StopRecordTimer record_timer_id     = %d ")
TRACE_MSG(MMIASP_CORTROL_LAYER_752_112_2_18_2_2_37_37,"[MMIASP]: MMIASP_StopRecordTimer: no applet!!!")
TRACE_MSG(MMIASP_CORTROL_LAYER_766_112_2_18_2_2_37_38,"[MMIASP] MMIASP_Prev() enter!")
TRACE_MSG(MMIASP_CORTROL_LAYER_803_112_2_18_2_2_37_39,"[MMIASP] MMIASP_Next() enter!")
TRACE_MSG(MMIASP_CORTROL_LAYER_854_112_2_18_2_2_37_40,"[MMIASP]: MMIASP_MsgListFree enter !")
TRACE_MSG(MMIASP_CORTROL_LAYER_862_112_2_18_2_2_37_41,"MMIASP MMIASP_MsgListFree no applet")
TRACE_MSG(MMIASP_CORTROL_LAYER_873_112_2_18_2_2_37_42,"[MMIASP]: MMIASP_MsgListFree record_path !")
TRACE_MSG(MMIASP_CORTROL_LAYER_897_112_2_18_2_2_38_43,"[MMIASP]: MMIASP_MsgListFree enter !")
TRACE_MSG(MMIASP_CORTROL_LAYER_954_112_2_18_2_2_38_44," [MMIASP]:MMIASP_AddMsg2ListEnd enter!")
TRACE_MSG(MMIASP_CORTROL_LAYER_990_112_2_18_2_2_38_45," [MMIASP]:AddMsgToListHeader enter!")
TRACE_MSG(MMIASP_CORTROL_LAYER_1030_112_2_18_2_2_38_46," [MMIASP] MMIASP_GetListMsgTotalNum list_num = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1036_112_2_18_2_2_38_47," [MMIASP] MMIASP_GetListMsgTotalNum no applet!!")
TRACE_MSG(MMIASP_CORTROL_LAYER_1095_112_2_18_2_2_38_48," [MMIASP] MMIASP_DelMarkNodeFromList no applet!!")
TRACE_MSG(MMIASP_CORTROL_LAYER_1119_112_2_18_2_2_38_49,"MMIASP GetCurNodeByGuiIndex list_index = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1131_112_2_18_2_2_38_50," [MMIASP] MMIASP_GetCurNodeByGuiIndex no applet!!")
TRACE_MSG(MMIASP_CORTROL_LAYER_1185_112_2_18_2_2_38_51,"[MMIASP]: MMIASP_CheckVersionNumber result = %d ")
TRACE_MSG(MMIASP_CORTROL_LAYER_1229_112_2_18_2_2_38_52,"[MMIASP]:MMIASP_CreateNewSysFile file_handle = %d,result = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1281_112_2_18_2_2_38_53," [MMIASP]: MMIASP_ReadSysFile list_num = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1288_112_2_18_2_2_38_54," [MMIASP]: MMIASP_ReadSysFile read filename fail ---")
TRACE_MSG(MMIASP_CORTROL_LAYER_1293_112_2_18_2_2_38_55," [MMIASP]: MMIASP_ReadSysFile read file_path_len:%d  ---")
TRACE_MSG(MMIASP_CORTROL_LAYER_1301_112_2_18_2_2_38_56," [MMIASP]: MMIASP_ReadSysFile read file_path_len fail = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1309_112_2_18_2_2_38_57,"ReadASPSysFile alloc failed")
TRACE_MSG(MMIASP_CORTROL_LAYER_1316_112_2_18_2_2_38_58," [MMIASP]: read filename fail ---")
TRACE_MSG(MMIASP_CORTROL_LAYER_1324_112_2_18_2_2_38_59," [MMIASP]: MMIASP_ReadSysFile read file_path_str:%s  ---")
TRACE_MSG(MMIASP_CORTROL_LAYER_1341_112_2_18_2_2_38_60," [MMIASP]: MMIASP_ReadSysFile read structure fail ---")
TRACE_MSG(MMIASP_CORTROL_LAYER_1360_112_2_18_2_2_38_61," [MMIASP]: MMIASP_ReadSysFile write_offset = %d,total_file_size = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1373_112_2_18_2_2_38_62," [MMIASP]: MMIASP_ReadSysFile read sys file head fail ---")
TRACE_MSG(MMIASP_CORTROL_LAYER_1388_112_2_18_2_2_39_63,"MMIASP MMIASP_ReadSysFileWithApplet no applet")
TRACE_MSG(MMIASP_CORTROL_LAYER_1433_112_2_18_2_2_39_64,"MMIASP MMIASP_WriteSysFile no applet")
TRACE_MSG(MMIASP_CORTROL_LAYER_1456_112_2_18_2_2_39_65," [MMIASP]: MMIASP_WriteSysFile s_answer_machine_version fail ---")
TRACE_MSG(MMIASP_CORTROL_LAYER_1481_112_2_18_2_2_39_66," [MMIASP]: MMIASP_WriteSysFile list_num = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1492_112_2_18_2_2_39_67," [MMIASP]: MMIASP_WriteSysFile write filename fail ---")
TRACE_MSG(MMIASP_CORTROL_LAYER_1501_112_2_18_2_2_39_68," [MMIASP]: MMIASP_WriteSysFile write filename fail ---")
TRACE_MSG(MMIASP_CORTROL_LAYER_1509_112_2_18_2_2_39_69," [MMIASP]: MMIASP_WriteSysFile write structure fail ---")
TRACE_MSG(MMIASP_CORTROL_LAYER_1550_112_2_18_2_2_39_70," [MMIASP] MMIASP_GetRecordFileDev file_dev=%d ")
TRACE_MSG(MMIASP_CORTROL_LAYER_1573_112_2_18_2_2_39_71,"[MMIASP]:  no suitable dev")
TRACE_MSG(MMIASP_CORTROL_LAYER_1585_112_2_18_2_2_39_72,"[MMIASP]: MMIASP_StartRecord file_dev = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1588_112_2_18_2_2_39_73,"MMIASP StartRecord file_dev= %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1596_112_2_18_2_2_39_74,"[MMIASP]: MMIASP_StartRecord cur_full_path = %s,cur_file_name_len = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1605_112_2_18_2_2_39_75,"[MMIASP]: MMIASP_StartRecord MMISRVAUD_SetUpLinkMute success")
TRACE_MSG(MMIASP_CORTROL_LAYER_1638_112_2_18_2_2_39_76,"MMIASP StartRecord malloc memory fail ERROR!!!!!!!!!")
TRACE_MSG(MMIASP_CORTROL_LAYER_1644_112_2_18_2_2_39_77,"MMIASP StartRecord fail, please check")
TRACE_MSG(MMIASP_CORTROL_LAYER_1651_112_2_18_2_2_39_78,"[MMIASP]: MMIASP_StartRecord result = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1674_112_2_18_2_2_39_79,"MMIASP GetDefaultAnswerRingFullName entered")
TRACE_MSG(MMIASP_CORTROL_LAYER_1677_112_2_18_2_2_39_80,"MMIASP nv set file_name_len = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1711_112_2_18_2_2_39_81,"[MMIASP]:MMIASP_PlayAudioInCall open background music!")
TRACE_MSG(MMIASP_CORTROL_LAYER_1714_112_2_18_2_2_39_82,"[MMIASP]:MMIASP_PlayAudioInCall  PARA HAS error !!")
TRACE_MSG(MMIASP_CORTROL_LAYER_1720_112_2_18_2_2_39_83,"[MMIASP]:MMIAPIASP_Play4Client:call_num is null!")
TRACE_MSG(MMIASP_CORTROL_LAYER_1727_112_2_18_2_2_39_84,"MMIASP_PlayAudioInCall:MMICC_EnableMute SET NOT MUTE FAIL!")
TRACE_MSG(MMIASP_CORTROL_LAYER_1741_112_2_18_2_2_39_85,"[MMIASP]:MMIASP_PlayAudioInCall audio_type = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1747_112_2_18_2_2_39_86,"MMIASP has set own answer ring")
TRACE_MSG(MMIASP_CORTROL_LAYER_1755_112_2_18_2_2_39_87,"[MMIASP]: is_fixed_ring = %d, ring_id = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1761_112_2_18_2_2_39_88,"MMIASP ring id = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1771_112_2_18_2_2_39_89,"[MMIASP]:MMIASP_PlayAudioInCall data_len=%d, data_ptr=0x%x")
TRACE_MSG(MMIASP_CORTROL_LAYER_1787_112_2_18_2_2_39_90,"MMIASP name len = 0")
TRACE_MSG(MMIASP_CORTROL_LAYER_1792_112_2_18_2_2_39_91,"[MMIASP]:  play MORE ring file name = %s")
TRACE_MSG(MMIASP_CORTROL_LAYER_1793_112_2_18_2_2_39_92,"[MMIASP]:  play MORE ring file name len= %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1804_112_2_18_2_2_39_93,"[MMIASP]:applet_ptr->bkmusic_audio_handle = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1828_112_2_18_2_2_39_94,"[MMIASP]:MMIAPIASP_Play4Client play_ret = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1872_112_2_18_2_2_40_95,"[MMIASP]: MMIASP_GetNewMsgNum new_msg_num=%d")
TRACE_MSG(MMIASP_CORTROL_LAYER_1936_112_2_18_2_2_40_96,"MMIASP GetPbNumIdByTelNum find NULL head OR call number error = %x")
TRACE_MSG(MMIASP_CORTROL_LAYER_2024_112_2_18_2_2_40_97,"MMIASP CombineASPFilePath entered")
TRACE_MSG(MMIASP_CORTROL_LAYER_2046_112_2_18_2_2_40_98,"MMIASP suffix_id = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_2050_112_2_18_2_2_40_99,"MMIASP pb_num_len = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_2149_112_2_18_2_2_40_100,"MMIASP MMIASP_SetCurMsgInfo entered")
TRACE_MSG(MMIASP_CORTROL_LAYER_2247_112_2_18_2_2_40_101,"[MMIASP] start asp applet return null!!")
TRACE_MSG(MMIASP_CORTROL_LAYER_2272_112_2_18_2_2_40_102," [MMIASP] MMIASP_CloseAspApplet exit !")
TRACE_MSG(MMIASP_CORTROL_LAYER_2354_112_2_18_2_2_40_103,"MMIASP GetFixedAnswerRing GET NV ")
TRACE_MSG(MMIASP_CORTROL_LAYER_2420_112_2_18_2_2_41_104,"MMIASP GetCustomerAnswerRing has not set")
TRACE_MSG(MMIASP_CORTROL_LAYER_2425_112_2_18_2_2_41_105,"MMIASP GetCustomerAnswerRing convert to bcd fail")
TRACE_MSG(MMIASP_CORTROL_LAYER_2486_112_2_18_2_2_41_106,"[MMIASP]: MMIASP_StopAnswerCall enter !")
TRACE_MSG(MMIASP_CORTROL_LAYER_2487_112_2_18_2_2_41_107,"[MMIASP]: MMIASP_StopAnswerCall woring flag = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_2491_112_2_18_2_2_41_108,"[MMIASP]: MMIASP_StopAnswerCall really enter!!")
TRACE_MSG(MMIASP_CORTROL_LAYER_2529_112_2_18_2_2_41_109,"[MMIASP]: CreateRecordTimer record_time     = %d ")
TRACE_MSG(MMIASP_CORTROL_LAYER_2530_112_2_18_2_2_41_110,"[MMIASP]: CreateRecordTimer record_timer_id = %d ")
TRACE_MSG(MMIASP_CORTROL_LAYER_2534_112_2_18_2_2_41_111,"[MMIASP]: CreateRecordTimer applet is NULL !!")
TRACE_MSG(MMIASP_CORTROL_LAYER_2549_112_2_18_2_2_41_112,"[MMIASP]: StopPlayAudio enter !")
TRACE_MSG(MMIASP_CORTROL_LAYER_2560_112_2_18_2_2_41_113,"[MMIASP]: MMIASP_StopAudioForClient MMISRVAUD_Stop result = %d,MMISRVMGR_Free result:%d")
TRACE_MSG(MMIASP_CORTROL_LAYER_2598_112_2_18_2_2_41_114,"[MMIASP] GetSysFileFullNameForWrite result = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_2626_112_2_18_2_2_41_115,"[MMIASP] GetSysFileFullNameForRead result = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_2652_112_2_18_2_2_41_116,"MMIASP SetMechineWorkingFlag = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_2660_112_2_18_2_2_41_117,"[MMIASP]: MMIAPIASP_RealseCall enter !")
TRACE_MSG(MMIASP_CORTROL_LAYER_2673_112_2_18_2_2_41_118,"MMIASP MMIASP_SetCallState set call_state = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_2680_112_2_18_2_2_41_119,"MMIASP applet is not worked ")
TRACE_MSG(MMIASP_CORTROL_LAYER_2691_112_2_18_2_2_41_120,"MMIASP MMIASP_GetCallState enterd")
TRACE_MSG(MMIASP_CORTROL_LAYER_2699_112_2_18_2_2_41_121,"MMIASP applet is not worked ")
TRACE_MSG(MMIASP_CORTROL_LAYER_2715_112_2_18_2_2_41_122,"GetAllPathEnoughFreeSpace false")
TRACE_MSG(MMIASP_CORTROL_LAYER_2719_112_2_18_2_2_41_123,"GetAllPathEnoughFreeSpace true")
TRACE_MSG(MMIASP_CORTROL_LAYER_2735_112_2_18_2_2_41_124,"[MMIASP]: GetCuurPathEnoughFreeSpace:file_dev = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_2749_112_2_18_2_2_41_125,"[MMIASP]: GetCuurPathEnoughFreeSpace:free_enough = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_2815_112_2_18_2_2_41_126,"[MMIASP]: Record_CallBack:MMISRVAUD_REPORT_RESULT_E = %d")
TRACE_MSG(MMIASP_CORTROL_LAYER_2869_112_2_18_2_2_42_127,"InsertReadRecordList fail")
TRACE_MSG(MMIASP_CORTROL_LAYER_2932_112_2_18_2_2_42_128,"MMIASP MMIASP_SortRecordList entered")
TRACE_MSG(MMIASP_CORTROL_LAYER_2936_112_2_18_2_2_42_129,"MMIASP no applet!!")
TRACE_MSG(MMIASP_CORTROL_LAYER_2942_112_2_18_2_2_42_130,"MMIASP find null list")
TRACE_MSG(MMIASP_CORTROL_LAYER_2947_112_2_18_2_2_42_131,"MMIASP no new node")
TRACE_MSG(MMIASP_CORTROL_LAYER_3160_112_2_18_2_2_42_132,"[MMIASP]: MMIASP_StartPlayAnswerMusic enter !")
TRACE_MSG(MMIASP_CORTROL_LAYER_3163_112_2_18_2_2_42_133," [MMIASP] MMIASP_StartAnswerCall Not applet !")
TRACE_MSG(MMIASP_CORTROL_LAYER_3265_112_2_18_2_2_42_134,"[MMIASP]: CreateRecordTimer applet is NULL !!")
TRACE_MSG(MMIASP_CORTROL_LAYER_3285_112_2_18_2_2_42_135,"[MMIASP]: MMIASP_StopRecordTimer: no applet!!!")
TRACE_MSG(MMIASP_WINTAB_869_112_2_18_2_2_45_136,"MMIASP InitButtonOperation no applet")
TRACE_MSG(MMIASP_WINTAB_935_112_2_18_2_2_45_137,"[MMIASP]:OKCallback, cur index = %d ")
TRACE_MSG(MMIASP_WINTAB_940_112_2_18_2_2_45_138,"[MMIASP]:PlayPauseKeyMsg, cur state = %d error!")
TRACE_MSG(MMIASP_WINTAB_1075_112_2_18_2_2_46_139,"MMIASP UpdateBackwardForwardButtonFg no aopplet")
TRACE_MSG(MMIASP_WINTAB_1180_112_2_18_2_2_46_140,"MMIASP HandleMsgListWinMsg MSGid = %x")
TRACE_MSG(MMIASP_WINTAB_1378_112_2_18_2_2_46_141,"MMIASP UpdatePlayDetailInfo no applet!!!")
TRACE_MSG(MMIASP_WINTAB_1386_112_2_18_2_2_46_142,"MMIASP UpdatePlayDetailInfo cur index= %d!!!")
TRACE_MSG(MMIASP_WINTAB_1446_112_2_18_2_2_46_143,"MMIASP total time = %d")
TRACE_MSG(MMIASP_WINTAB_1665_112_2_18_2_2_47_144,"MMIASP UpdatePlayDetailInfo no applet!!!")
TRACE_MSG(MMIASP_WINTAB_2039_112_2_18_2_2_47_145,"[MMIASP]: HandleProgressBarTpKey, tp_x= %d")
TRACE_MSG(MMIASP_WINTAB_2050_112_2_18_2_2_47_146,"[MMIASP]: HandleProgressBarTpKey, percent = %d,seek_offset = %d, result = %d")
TRACE_MSG(MMIASP_WINTAB_2074_112_2_18_2_2_48_147,"[MMIASP]: StartTpDragTimer")
TRACE_MSG(MMIASP_WINTAB_2082_112_2_18_2_2_48_148,"[MMIASP]: StartTpDragTimer: the timer has started!")
TRACE_MSG(MMIASP_WINTAB_2099_112_2_18_2_2_48_149,"[MMIASP]: StopTpDragTimer: the timer has stop!")
TRACE_MSG(MMIASP_WINTAB_2116_112_2_18_2_2_48_150,"[MMIASP]: HandlePlayWinTpKeyDown")
TRACE_MSG(MMIASP_WINTAB_2159_112_2_18_2_2_48_151,"MMIASP HandlePlayWinMsg MSG_ID = %d")
TRACE_MSG(MMIASP_WINTAB_2172_112_2_18_2_2_48_152,"MMIASP play win null code , index = %d")
TRACE_MSG(MMIASP_WINTAB_2255_112_2_18_2_2_48_153,"[MMIASP]: HandlePlayWinMsg MSG_RECORD_PLAY_CALL_BACK ")
TRACE_MSG(MMIASP_WINTAB_2262_112_2_18_2_2_48_154,"[MMIASP]: HandlePlayWinMsg *result_ptr = %d")
TRACE_MSG(MMIASP_WINTAB_2293_112_2_18_2_2_48_155,"MMIASP MSG_ASP_PLAY_CALL_BACK return msg")
TRACE_MSG(MMIASP_WINTAB_2384_112_2_18_2_2_48_156,"MMIASP please check applet!!")
TRACE_MSG(MMIASP_WINTAB_2984_112_2_18_2_2_49_157,"MMIASP please check applet")
TRACE_MSG(MMIASP_WINTAB_3026_112_2_18_2_2_49_158,"MMIASP AppendRecordList no applet")
TRACE_MSG(MMIASP_WINTAB_3032_112_2_18_2_2_49_159,"AppendRecordList ASP read sys file fail")
TRACE_MSG(MMIASP_WINTAB_3174_112_2_18_2_2_50_160,"MMIASP MMIASP_ShowNewAspMsgWin msg_count = %d")
TRACE_MSG(MMIASP_WINTAB_3306_112_2_18_2_2_50_161,"MMIASP SetListMarkMenu has error!")
TRACE_MSG(MMIASP_WINTAB_3471_112_2_18_2_2_50_162,"MMIASP HandleAspMenuOperation index = %d")
TRACE_MSG(MMIASP_WINTAB_3480_112_2_18_2_2_50_163,"HandleAspMenuOperation null Node")
TRACE_MSG(MMIASP_WINTAB_3483_112_2_18_2_2_50_164,"MMIASP num_len  = %d")
TRACE_MSG(MMIASP_WINTAB_4243_112_2_18_2_2_52_165,"no applet, please check!!")
TRACE_MSG(MMIASP_WINTAB_4252_112_2_18_2_2_52_166,"MMIASP HandleRingSelectWinMsg has callback function now")
TRACE_MSG(MMIASP_WINTAB_4258_112_2_18_2_2_52_167,"MMIASP HandleRingSelectWinMsg MULTIM_SELECTED_RETURN null para")
TRACE_MSG(MMIASP_WINTAB_4466_112_2_18_2_2_52_168,"[MMIASP] AspAppletHandleEvent start app only...")
TRACE_MSG(MMIASP_WINTAB_4472_112_2_18_2_2_52_169,"[MMIASP] AspAppletHandleEvent MSG_APPLET_STOP")
TRACE_MSG(MMIASP_WINTAB_4625_112_2_18_2_2_53_170,"MMIASP HandleRecordingWinMsg OPEN but applet is null")
TRACE_MSG(MMIASP_WINTAB_4693_112_2_18_2_2_53_171,"HandleRecordingWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID")
TRACE_MSG(MMIASP_WINTAB_4702_112_2_18_2_2_53_172,"HandleRecordingWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID")
TRACE_MSG(MMIASP_WINTAB_4721_112_2_18_2_2_53_173,"HandleRecordingWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID")
TRACE_MSG(MMIASP_WINTAB_5128_112_2_18_2_2_54_174,"MMIASP HandleMSGDeleteOperation no applet")
TRACE_MSG(MMIASP_WINTAB_5162_112_2_18_2_2_54_175,"MMIASP MMIASP_OpenPlayWinFromExternal node_index = %d")
TRACE_MSG(MMIASP_WINTAB_5166_112_2_18_2_2_54_176,"MMIASP MMIASP_OpenPlayWinFromExternal find no applet")
TRACE_MSG(MMIASP_WINTAB_5173_112_2_18_2_2_54_177,"MMIASP error index")
TRACE_MSG(MMIASP_WINTAB_5192_112_2_18_2_2_54_178,"MMIASP MMIASP_OpenPlayWinFromExternal no node")
TRACE_MSG(MMIASP_WINTAB_5224_112_2_18_2_2_54_179,"MMIASP AppendOneListItem item_index = %d")
TRACE_MSG(MMIASP_WINTAB_5227_112_2_18_2_2_54_180,"MMIASP AppendOneListItem no applet")
TRACE_MSG(MMIASP_WINTAB_5234_112_2_18_2_2_54_181,"MMIASP AppendOneListItem no node item_index = %d")
TRACE_MSG(MMIASP_WINTAB_5258_112_2_18_2_2_54_182,"MMIASP this number has find a name ")
TRACE_MSG(MMIASP_WINTAB_5305_112_2_18_2_2_54_183,"MMIASP_AutoPlayRecordMessages find no applet")
TRACE_MSG(MMIASP_WINTAB_5314_112_2_18_2_2_54_184,"MMIASP error index")
TRACE_MSG(MMIASP_WINTAB_5377_112_2_18_2_2_54_185,"MMIASP HandleAspMenuOperation index = %d")
TRACE_MSG(MMIASP_WINTAB_5386_112_2_18_2_2_54_186,"HandleAspMenuOperation null Node")
TRACE_MSG(MMIASP_WINTAB_5389_112_2_18_2_2_54_187,"MMIASP num_len  = %d")
TRACE_MSG(MMIASP_WINTAB_5458_112_2_18_2_2_54_188,"[ASP] HandleLocalListLongOkOptMenuWinMsg menu_id = %x")
END_TRACE_MAP(MMI_APP_ASP_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_ASP_TRC_H_

