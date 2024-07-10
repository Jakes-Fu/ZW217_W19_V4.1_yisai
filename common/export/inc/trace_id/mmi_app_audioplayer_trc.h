/******************************************************************************
 ** File Name:      mmi_app_audioplayer_trc.h                                         *
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
//trace_id:152
#ifndef _MMI_APP_AUDIOPLAYER_TRC_H_
#define _MMI_APP_AUDIOPLAYER_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIMP3_977_112_2_18_2_3_50_0 "[MMIMP3_ReqPlaySrv]: s_mp3bg_play_handle exist!"
#define MMIMP3_989_112_2_18_2_3_50_1 "[MMIMP3_ReqPlaySrv]: MMISRVAUD_SetAudioInfo error!!!!!!"
#define MMIMP3_1014_112_2_18_2_3_50_2 "[MMIMP3_ReqPlaySrv]: MMISRVMGR_Request error!!!!!!"
#define MMIMP3_1018_112_2_18_2_3_50_3 "MMIMP3_ReqPlaySrv: s_mp3bg_play_handle=0x%x"
#define MMIMP3_1175_112_2_18_2_3_50_4 "[MMIMP3] MMIMP3_SetDisplayStyle style = %d"
#define MMIMP3_1212_112_2_18_2_3_50_5 "[MMIMP3] MMIMP3_GetDisplayStyle style = %d"
#define MMIMP3_1229_112_2_18_2_3_50_6 "[MMIMP3]:MMIMP3_SetRepeatMode, param error!"
#define MMIMP3_1259_112_2_18_2_3_50_7 "[MMIMP3]:MMIMP3_SetShuffleMode, param error!"
#define MMIMP3_1354_112_2_18_2_3_50_8 "[MMIMP3] MMIAPIMP3_IsThroughBt = %d"
#define MMIMP3_1494_112_2_18_2_3_51_9 "[MMIMP3]:MMIAPIMP3_SetMp3EqMode, eq_mode = %d!"
#define MMIMP3_1534_112_2_18_2_3_51_10 "[MMIMP3] MMIMP3_DelayPlayMp3 encounter MMIMP3_PLAYER_SWITCH_PAUSED"
#define MMIMP3_1553_112_2_18_2_3_51_11 "[MMIMP3] MMIMP3_DelayPlayMp3 Start Timer = %d, offset = %d"
#define MMIMP3_1575_112_2_18_2_3_51_12 "[MMIMP3] MMIMP3_StopDelayPlayTimer Stop Timer"
#define MMIMP3_1602_112_2_18_2_3_51_13 "[MMIMP3] MMIAPIMP3_HandleDelayPlayTimer encounter MMIMP3_PLAYER_SWITCH_PAUSED"
#define MMIMP3_1610_112_2_18_2_3_51_14 "[MMIMP3] MMIAPIMP3_HandleDelayPlayTimer result = %d, p_state = %d, a_state = %d!!!"
#define MMIMP3_1629_112_2_18_2_3_51_15 "[MMIMP3] MMIMP3_IsPausedByUser result = %d!!!"
#define MMIMP3_1646_112_2_18_2_3_51_16 "[MMIMP3]: ConsumeDrmRight index = %d"
#define MMIMP3_1696_112_2_18_2_3_51_17 "[MMIMP3] Mp3AlertCallback enter"
#define MMIMP3_1907_112_2_18_2_3_51_18 "[MMIMP3 DRM] MMIAPIMP3_IsDrmFile :drm_audio_type = %d,is_audio_drm_file = %d, is_drm_valid = %d, result = %d"
#define MMIMP3_1940_112_2_18_2_3_51_19 "[MMIMP3] MMIMP3_PlayMp3 active_index = %d, total_num = %d"
#define MMIMP3_1961_112_2_18_2_3_51_20 "[MMIMP3] MMIMP3_PlayMp3 is drm file"
#define MMIMP3_1973_112_2_18_2_3_51_21 "[MMIMP3] MMIMP3_PlayMp3 :is_file_exist = %d,file_size = %d, is_a2dp = %d, file_support_a2sp = %d"
#define MMIMP3_2070_112_2_18_2_3_52_22 "[MMIMP3]:MMIMP3_PlayMp3: uiTotalTime = %d, total_length = %d, bit_rate = %d"
#define MMIMP3_2144_112_2_18_2_3_52_23 "[MMIMP3]:MMIMP3_PlayMp3, result = %d"
#define MMIMP3_2157_112_2_18_2_3_52_24 "[MMIMP3] MMIAPIMP3_PauseMp3 player_state = %d, audio_state = %d, s_mp3_pause_type = %d!!!"
#define MMIMP3_2187_112_2_18_2_3_52_25 "[MMIMP3]:MMIAPIMP3_PauseMp3:AUDIO FAILED!"
#define MMIMP3_2214_112_2_18_2_3_52_26 "[MMIMP3]:MMIAPIMP3_PauseMp3: result = %d"
#define MMIMP3_2241_112_2_18_2_3_52_27 "[MMIMP3] MMIAPIMP3_ResumeMp3 player_state = %d, audio_state = %d"
#define MMIMP3_2275_112_2_18_2_3_52_28 "[MMIMP3]:MMIAPIMP3_ResumeMp3:AUDIO FAILED!"
#define MMIMP3_2368_112_2_18_2_3_52_29 "[MMIMP3] MMIMP3_StopMp3 player_state = %d, audio_state = %d"
#define MMIMP3_2439_112_2_18_2_3_52_30 "StartMp3PlayTimer:mp3_time = %d,time_out = %d,s_mp3_progress_timer_id = %d"
#define MMIMP3_2663_112_2_18_2_3_53_31 "[MMIMP3]:PlayCallBack handle=0x%x, s_mp3bg_play_handle = 0x%x"
#define MMIMP3_2666_112_2_18_2_3_53_32 "[MMIMP3]:PlayCallBack event=%d"
#define MMIMP3_2683_112_2_18_2_3_53_33 "[MMIMP3]:PlayCallBack report=%d,data=%d"
#define MMIMP3_2760_112_2_18_2_3_53_34 "[MMIMP3]:MMIMP3_CallBackAfterPlay:result = %d"
#define MMIMP3_2802_112_2_18_2_3_53_35 "[MMIMP3] MMIAPIMP3_PlayMp3CallBack cur_play_mode = %d,review_state = %d,PlayerState = %d"
#define MMIMP3_2925_112_2_18_2_3_53_36 "[MMIMP3] MMIAPIMP3_GoToPreOrNextMp3 cur_play_mode = %d,review_state = %d, player_state = %d,audio_state = %d"
#define MMIMP3_2929_112_2_18_2_3_53_37 "[MMIMP3] MMIAPIMP3_GoToPreOrNextMp3 Total_num = %d"
#define MMIMP3_3069_112_2_18_2_3_54_38 "[MMIMP3]:GetNextMp3Index, next_index_ptr = %x, random_info_ptr = %x!"
#define MMIMP3_3149_112_2_18_2_3_54_39 "GetNextMp3Index:Don't call this function!"
#define MMIMP3_3220_112_2_18_2_3_54_40 "[MMIMP3]:MMIAPIMP3_GetCurMp3Name, mp3_name_ptr = %x!"
#define MMIMP3_3353_112_2_18_2_3_54_41 "MMIAPIMP3_ForwardMp3:get mp3 current offset result %d is error!"
#define MMIMP3_3363_112_2_18_2_3_54_42 "[MMIMP3]:MMIMP3_ReviewForwardMp3 ReviewEnd"
#define MMIMP3_3373_112_2_18_2_3_54_43 "MMIMP3_ReviewForwardMp3:get mp3 current offset result %d is error!"
#define MMIMP3_3381_112_2_18_2_3_54_44 "MMIAPIMP3_ForwardMp3:cur_state %d don't handle!"
#define MMIMP3_3408_112_2_18_2_3_54_45 "[MMIMP3]:GetReviewMp3CurrentOffset, mp3_offset_ptr = %x!"
#define MMIMP3_3415_112_2_18_2_3_54_46 "GetReviewMp3CurrentOffset:file_total_time = %d,file_total_len = %d,bitrate = %d"
#define MMIMP3_3435_112_2_18_2_3_54_47 "GetReviewMp3CurrentOffset:forward_backward_offset = %ld"
#define MMIMP3_3468_112_2_18_2_3_54_48 "MMIAPIMP3_BackwardMp3:get mp3 current offset result %d is error!"
#define MMIMP3_3482_112_2_18_2_3_55_49 "MMIMP3_ReviewBackwardMp3:get mp3 current offset result %d is error!"
#define MMIMP3_3490_112_2_18_2_3_55_50 "MMIAPIMP3_BackwardMp3:cur_state %d don't handle!"
#define MMIMP3_3509_112_2_18_2_3_55_51 "[MMIMP3]:GetMp3CurrentOffset, mp3_offset_ptr = %x!"
#define MMIMP3_3516_112_2_18_2_3_55_52 "GetMp3CurrentOffset:file_total_time = %d,file_total_len = %d,bitrate = %d"
#define MMIMP3_3532_112_2_18_2_3_55_53 "GetMp3CurrentOffset:forward_backward_offset = %ld"
#define MMIMP3_3554_112_2_18_2_3_55_54 "[MMIMP3] ForwardOrBackwardMp3 mp3_offset = %d, cur_state = %d"
#define MMIMP3_3589_112_2_18_2_3_55_55 "ForwardOrBackwardMp3: MMIMP3_AUDIO_PLAYING audio seek result = %d"
#define MMIMP3_3622_112_2_18_2_3_55_56 "ForwardOrBackwardMp3: MMIMP3_AUDIO_PAUSED audio seek result = %d"
#define MMIMP3_3699_112_2_18_2_3_55_57 "GetMp3CurrentTotalOffset:mp3_total_offset =%ld"
#define MMIMP3_3735_112_2_18_2_3_55_58 "MMIMP3_HandleProgressBarTpKey:tp_x_offet = %d,tp_x = %d,progress_bar_rect.left = %d"
#define MMIMP3_3759_112_2_18_2_3_55_59 "MMIMP3_HandleProgressBarTpKey:get mp3 current offset result %d is error!"
#define MMIMP3_3795_112_2_18_2_3_55_60 "[MMIMP3]:GetMp3CurrentOffsetByTp, is_forward_ptr = %x,mp3_total_offset_ptr = %x,mp3_bitrate_ptr = %x!"
#define MMIMP3_3805_112_2_18_2_3_55_61 "GetMp3CurrentOffsetByTp:cur_slide_x_offset = %d,tp_x_offset = %d"
#define MMIMP3_3826_112_2_18_2_3_55_62 "GetMp3CurrentOffsetByTp:is_forward = %d,mp3_total_offset = %ld"
#define MMIMP3_3853_112_2_18_2_3_55_63 "[MMIMP3]:MMIAPIMP3_IsMp3PlayerLoadType, full_path_ptr = %x,full_path_len = %d!"
#define MMIMP3_3955_112_2_18_2_3_55_64 "[MMIMP3]: MMIMP3_IsFileSeekSupport, audio_type = %d, result = %d"
#define MMIMP3_3980_112_2_18_2_3_56_65 "[MMIMP3]: MMIMP3_OpenFileCallBack, Enter,is_success = %d, file_num = %d, file_array = 0x%x!"
#define MMIMP3_4020_112_2_18_2_3_56_66 "[MMIMP3]: MMIMP3_OpenFileCallBack, MMIMP3_CreateEmptyListFile ok"
#define MMIMP3_4028_112_2_18_2_3_56_67 "[MMIMP3]: MMIMP3_OpenFileCallBack,AddFileInfoToListFromFMMArray, success!"
#define MMIMP3_4034_112_2_18_2_3_56_68 "[MMIMP3]: AddFileInfoToListFromFMMArray, Fail!"
#define MMIMP3_4084_112_2_18_2_3_56_69 "[MMIMP3]: MMIMP3_AddFileToListCallBack, Enter,is_success = %d, file_num = %d"
#define MMIMP3_4150_112_2_18_2_3_56_70 "[MMIMP3]: SetListFileWrittenCheckInfo, file_handle = 0x%x,is_written_ok = %d,result = %d"
#define MMIMP3_4184_112_2_18_2_3_56_71 "[MMIMP3]: GetListFileWrittenCheckInfo, file_handle = 0x%x,is_written_ok = %d,result = %d"
#define MMIMP3_4218_112_2_18_2_3_56_72 "[MMIMP3]: ReadListFileHeadInfo, file_handle = 0x%x,oper_offset = %d, oper_size = %d, total_num = %d, total_num_alloc = %d, result = %d"
#define MMIMP3_4252_112_2_18_2_3_56_73 "[MMIMP3]: WriteListFileHeadInfo, file_handle = 0x%x,oper_offset = %d, oper_size = %d, total_num = %d, total_num_alloc = %d, result = %d"
#define MMIMP3_4289_112_2_18_2_3_56_74 "[MMIMP3]: ReadListFileEndInfo, file_handle = 0x%x,oper_offset = %d, oper_size = %d, total_num = %d, total_num_alloc = %d, result = %d"
#define MMIMP3_4326_112_2_18_2_3_56_75 "[MMIMP3]: WriteListFileEndInfo, file_handle = 0x%x,oper_offset = %d, oper_size = %d, total_num = %d, total_num_alloc = %d, result = %d"
#define MMIMP3_4354_112_2_18_2_3_56_76 "[MMIMP3]: ReadListFileIndexInfo, file_handle = 0x%x,start_index = %d, index_num = %d, total_num = %d, total_num_alloc = %d"
#define MMIMP3_4363_112_2_18_2_3_56_77 "[MMIMP3]: ReadListFileIndexInfo, oper_offset = %d, oper_size = %d, *index_num_ptr = %d "
#define MMIMP3_4367_112_2_18_2_3_56_78 "[MMIMP3]: ReadListFileIndexInfo, ERROR, *index_num_ptr = %d, index_num = %d "
#define MMIMP3_4381_112_2_18_2_3_56_79 "[MMIMP3]: ReadListFileIndexInfo, file_handle = 0x%x,oper_offset = %d, oper_size = %d, total_num = %d, total_num_alloc = %d, file_index = %d, result = %d"
#define MMIMP3_4430_112_2_18_2_3_56_80 "[MMIMP3]: WriteListFileIndexInfo, file_handle = 0x%x,oper_offset = %d,oper_size = %d,result = %d"
#define MMIMP3_4466_112_2_18_2_3_57_81 "[MMIMP3]: ReadListFileItemInfo, file_handle = 0x%x,oper_offset = %d,oper_size = %d,result = %d"
#define MMIMP3_4516_112_2_18_2_3_57_82 "[MMIMP3]: WriteListFileItemInfoInSequence, file_handle = 0x%x,item_num = %d,oper_offset = %d,oper_size = %d,result = %d"
#define MMIMP3_4808_112_2_18_2_3_57_83 "[MMIMP3]: AddFileInfoToListFromFMMArray, open file, list_name_len = %d"
#define MMIMP3_4838_112_2_18_2_3_57_84 "[MMIMP3]: AddFileInfoToListFromFMMArray, file_handle = %d,total_num_alloc = %d,total_num = %d"
#define MMIMP3_4912_112_2_18_2_3_57_85 "[MMIMP3]: AddFileInfoToListFromFMMArray, append_body_num = %d,add_body_num = %d"
#define MMIMP3_4933_112_2_18_2_3_57_86 "[MMIMP3]: AddFileInfoToListFromFMMArray, writing body info"
#define MMIMP3_4973_112_2_18_2_3_58_87 "[MMIMP3]: AddFileInfoToListFromFMMArray,MMIAPIFMM_WriteFile Fails"
#define MMIMP3_4990_112_2_18_2_3_58_88 "[MMIMP3]: AddFileInfoToListFromFMMArray, FILE_LIMIT = %d, alloc = %d"
#define MMIMP3_4996_112_2_18_2_3_58_89 "[MMIMP3]: AddFileInfoToListFromFMMArray,MMIAPIFMM_WriteFile Fails"
#define MMIMP3_5058_112_2_18_2_3_58_90 "[MMIMP3]: AddFileInfoToListFromFMMArray, FILE_LIMIT = %d, alloc = %d"
#define MMIMP3_5064_112_2_18_2_3_58_91 "[MMIMP3]: AddFileInfoToListFromFMMArray,MMIAPIFMM_WriteFile Fails"
#define MMIMP3_5082_112_2_18_2_3_58_92 "[MMIMP3]: AddFileInfoToListFromFMMArray, writing head info, total_num_alloc = %d,total_num = %d"
#define MMIMP3_5091_112_2_18_2_3_58_93 "[MMIMP3]: AddFileInfoToListFromFMMArray, writing index info"
#define MMIMP3_5170_112_2_18_2_3_58_94 "[MMIMP3]: MMIMP3_CreateEmptyListFile, fail at entry"
#define MMIMP3_5227_112_2_18_2_3_58_95 "[MMIMP3]: MMIMP3_CreateEmptyListFile, sfs_error = %d, result = %d!"
#define MMIMP3_5251_112_2_18_2_3_58_96 "[MMIMP3] GetFileNamePos fails OUT = 0x%x, IN_Name = 0x%x, In_len = %d"
#define MMIMP3_5256_112_2_18_2_3_58_97 "[MMIMP3] GetFileNamePos fails, path head error!!!!!!!!!!!!!!!"
#define MMIMP3_5272_112_2_18_2_3_58_98 "[MMIMP3] GetFileNamePos, result = %d, OUT = %d, IN_Name = 0x%x, In_len = %d"
#define MMIMP3_5352_112_2_18_2_3_58_99 "[MMIMP3]: MMIMP3_AddFileInfoFromListToList,param error"
#define MMIMP3_5534_112_2_18_2_3_59_100 "[MMIMP3]: MMIMP3_AddFileInfoFromListToList,MMIAPIFMM_WriteFile Fails"
#define MMIMP3_5551_112_2_18_2_3_59_101 "[MMIMP3]: MMIMP3_AddFileInfoFromListToList, FILE_LIMIT = %d, alloc = %d"
#define MMIMP3_5557_112_2_18_2_3_59_102 "[MMIMP3]: MMIMP3_AddFileInfoFromListToList,MMIAPIFMM_WriteFile Fails"
#define MMIMP3_5565_112_2_18_2_3_59_103 "[MMIMP3]: MMIMP3_AddFileInfoFromListToList,ReadListFileItemInfo Fails, file_index = %d"
#define MMIMP3_5597_112_2_18_2_3_59_104 "[MMIMP3]: MMIMP3_AddFileInfoFromListToList,ReadListFileItemInfo Fails, file_index = %d"
#define MMIMP3_5618_112_2_18_2_3_59_105 "[MMIMP3]: MMIMP3_AddFileInfoFromListToList, FILE_LIMIT = %d, alloc = %d"
#define MMIMP3_5624_112_2_18_2_3_59_106 "[MMIMP3]: MMIMP3_AddFileInfoFromListToList,MMIAPIFMM_WriteFile Fails"
#define MMIMP3_5701_112_2_18_2_3_59_107 "[MMIMP3]: MMIMP3_AddFileInfoFromListToList, file_num = %d,added_num = %d, result = %d"
#define MMIMP3_5741_112_2_18_2_3_59_108 "[MMIMP3]: MMIMP3_AddFileInfoToList,param error"
#define MMIMP3_5878_112_2_18_2_3_59_109 "[MMIMP3]: MMIMP3_AddFileInfoToList,MMIAPIFMM_WriteFile Fails"
#define MMIMP3_5895_112_2_18_2_3_59_110 "[MMIMP3]: MMIMP3_AddFileInfoToList, FILE_LIMIT = %d, alloc = %d"
#define MMIMP3_5901_112_2_18_2_3_59_111 "[MMIMP3]: MMIMP3_AddFileInfoToList,MMIAPIFMM_WriteFile Fails"
#define MMIMP3_5933_112_2_18_2_4_0_112 "[MMIMP3]: MMIMP3_AddFileInfoToList, FILE_LIMIT = %d, alloc = %d"
#define MMIMP3_5939_112_2_18_2_4_0_113 "[MMIMP3]: MMIMP3_AddFileInfoToList,MMIAPIFMM_WriteFile Fails"
#define MMIMP3_5999_112_2_18_2_4_0_114 "[MMIMP3]: MMIMP3_AddFileInfoToList,list_len = %d, file_num = %d,added_num = %d, result = %d"
#define MMIMP3_6021_112_2_18_2_4_0_115 "[MMIMP3]: MMIMP3_InitCurPlayInfoByMp3List, file head error"
#define MMIMP3_6037_112_2_18_2_4_0_116 "[MMIMP3]: MMIMP3_InitCurPlayInfoByMp3List,total_num = %d, result = %d"
#define MMIMP3_6164_112_2_18_2_4_0_117 "[MMIMP3]: MMIMP3_ResetCurPlayInfo exit"
#define MMIMP3_6227_112_2_18_2_4_0_118 "[MMIMP3]: MMIMP3_LoadCurPlayInfoByIndex, entered! file_index = %d,total_num = %d,list_type = %d"
#define MMIMP3_6369_112_2_18_2_4_0_119 "[MMIMP3]: HandleMp3LoadAllListWaitingWin, msg=%x"
#define MMIMP3_6398_112_2_18_2_4_0_120 "[MMIMP3]: HandleMp3LoadAllListWaitingWin, MMIAPIFILEARRAY_Combine FAIL"
#define MMIMP3_6494_112_2_18_2_4_1_121 "[MMIMP3]MMIMP3_AddNewListToAllLists MMIAPIFILEARRAY_Add, result = %d"
#define MMIMP3_6512_112_2_18_2_4_1_122 "[MMIMP3]MMIMP3_DeleteListFromAllLists MMIAPIFILEARRAY_Delete, list_index = %d, result = %d"
#define MMIMP3_6525_112_2_18_2_4_1_123 "[MMIMP3]MMIMP3_GetTotalListNum MMIAPIFILEARRAY_GetArraySize, total_num = %d"
#define MMIMP3_6542_112_2_18_2_4_1_124 "[MMIMP3]MMIMP3_GetArrayListInfo MMIAPIFILEARRAY_Read, list_index = %d, result = %d"
#define MMIMP3_6636_112_2_18_2_4_1_125 "[MMIMP3]MMIMP3_GetListInfo list_index = %d, result = %d"
#define MMIMP3_6756_112_2_18_2_4_1_126 "[MMIMP3]MMIMP3_GetListIndexByName list_index = %d, result = %d"
#define MMIMP3_6837_112_2_18_2_4_1_127 "[MMIMP3]MMIMP3_CheckCurPlayListIndex  list_index = %d, result = %d"
#define MMIMP3_6953_112_2_18_2_4_2_128 "[MMIMP3]: MMIMP3_ResetMyMusic, MMIAPIFMM_DeleteFile, failed"
#define MMIMP3_6978_112_2_18_2_4_2_129 "[MMIMP3]: MMIMP3_LoadMyMusic, entered"
#define MMIMP3_7011_112_2_18_2_4_2_130 "[MMIMP3]: HandleMp3LoadMyMusicWaitingWin, msg=%x"
#define MMIMP3_7038_112_2_18_2_4_2_131 "[MMIMP3]: HandleMp3LoadMyMusicWaitingWin, MMIAPIFILEARRAY_Combine FAIL"
#define MMIMP3_7172_112_2_18_2_4_2_132 "[MMIMP3]: MMIMP3_InitMyMusicByFMMArray, total_num = %d, result =%d"
#define MMIMP3_7344_112_2_18_2_4_2_133 "[MMIMP3]MMIMP3_InitCurViewInfoByMyMusic:total_num = %d, list_type = %d"
#define MMIMP3_7398_112_2_18_2_4_2_134 "[MMIMP3]MMIMP3_InitCurViewInfoByReserveList: total_num = %d, list_type = %d"
#define MMIMP3_7908_112_2_18_2_4_3_135 "[MMIMP3]: MMIMP3_LoadAllMp3Files, entered"
#define MMIMP3_7936_112_2_18_2_4_4_136 "[MMIMP3]: HandleMp3LoadAllMusicWaitingWin, msg=%x"
#define MMIMP3_7961_112_2_18_2_4_4_137 "[MMIMP3]: HandleMp3LoadAllMusicWaitingWin, MMIAPIFILEARRAY_GetArraySize = %d"
#define MMIMP3_7990_112_2_18_2_4_4_138 "[MMIMP3]: HandleMp3LoadAllMusicWaitingWin, to_add_num = %d, added_num = %d!"
#define MMIMP3_8006_112_2_18_2_4_4_139 "[MMIMP3]: HandleMp3LoadAllMusicWaitingWin, AddFileInfoToListFromFMMArray, Fail!"
#define MMIMP3_8114_112_2_18_2_4_4_140 "[MMIMP3] MMIMP3_GetCurPlayListNameInfo, PNULL == full_path_name_ptr || NULL == full_path_len_ptr"
#define MMIMP3_8131_112_2_18_2_4_4_141 "[MMIMP3] MMIMP3_GetCurPlayListNameInfo, init_list_type = %d, list_name_len = %d, result = %d"
#define MMIMP3_8508_112_2_18_2_4_5_142 "[MMIMP3]: MMIMP3_GetCurViewFileInfo, cur_list_type = %d, file_index = %d"
#define MMIMP3_8570_112_2_18_2_4_5_143 "[MMIMP3] MMIMP3_GetCurViewFileInfo cur_list_type = %d"
#define MMIMP3_8578_112_2_18_2_4_5_144 "[MMIMP3]: MMIMP3_GetCurViewFileInfo, full_path_len = %d"
#define MMIMP3_8625_112_2_18_2_4_5_145 "[MMIMP3]: LoadListSectionFromMp3List, open file, list_name_len = %d,file_index = %d"
#define MMIMP3_8633_112_2_18_2_4_5_146 "[MMIMP3]: LoadListSectionFromMp3List, file_handle = %d,total_num_alloc = %d,total_num = %d"
#define MMIMP3_8662_112_2_18_2_4_5_147 "[MMIMP3]: LoadListSectionFromMp3List, reading body info"
#define MMIMP3_8725_112_2_18_2_4_6_148 "[MMIMP3]: LoadFilesInfoFromMp3List, open file, list_name_len = %d"
#define MMIMP3_8733_112_2_18_2_4_6_149 "[MMIMP3]: LoadFilesInfoFromMp3List, file_handle = %d,total_num_alloc = %d,total_num = %d"
#define MMIMP3_8762_112_2_18_2_4_6_150 "[MMIMP3]: LoadFilesInfoFromMp3List, reading body info"
#define MMIMP3_8885_112_2_18_2_4_6_151 "[MMIMP3]MMIMP3_InitCurViewInfoByListIndex:total_num = %d, list_type = %d"
#define MMIMP3_8950_112_2_18_2_4_6_152 "[MMIMP3]MMIMP3_InitCurViewInfoByListIndex:total_num = %d, list_type = %d"
#define MMIMP3_9030_112_2_18_2_4_7_153 "[MMIMP3] MMIMP3_UpdateCurViewInfo cur_list_type = %d"
#define MMIMP3_9096_112_2_18_2_4_7_154 "[MMIMP3]: MMIMP3_DelFileInfoFromArray, del real file success, index = %d"
#define MMIMP3_9100_112_2_18_2_4_7_155 "[MMIMP3]: MMIMP3_DelFileInfoFromArray, del real file fails, index = %d, sfs_del_rlt = %d"
#define MMIMP3_9138_112_2_18_2_4_7_156 "[MMIMP3]:MMIMP3_DelFileInfo result = %d"
#define MMIMP3_9164_112_2_18_2_4_7_157 "[MMIMP3]: MMIMP3_DelFileInfoFromList, open file, list_name_len = %d, file_index = %d, index_num = %d, is_del_real_file = %d"
#define MMIMP3_9181_112_2_18_2_4_7_158 "[MMIMP3]: MMIMP3_DelFileInfoFromList, file_handle = %d,total_num_alloc = %d,total_num = %d"
#define MMIMP3_9206_112_2_18_2_4_7_159 "[MMIMP3]: MMIMP3_DelFileInfoFromList, rearrange index info"
#define MMIMP3_9220_112_2_18_2_4_7_160 "[MMIMP3]: MMIMP3_DelFileInfoFromList, del real file success, index = %d"
#define MMIMP3_9224_112_2_18_2_4_7_161 "[MMIMP3]: MMIMP3_DelFileInfoFromList, del real file fails, index = %d, sfs_del_rlt = %d"
#define MMIMP3_9256_112_2_18_2_4_7_162 "[MMIMP3]: MMIMP3_DelFileInfoFromList, writing head info, total_num_alloc = %d,total_num = %d"
#define MMIMP3_9299_112_2_18_2_4_7_163 "[MMIMP3]: MMIMP3_DelFileInfoFromList, ClearWasteSpaceFromList fails"
#define MMIMP3_9327_112_2_18_2_4_7_164 "[MMIMP3]: ClearWasteSpaceFromList, entered"
#define MMIMP3_9385_112_2_18_2_4_8_165 "[MMIMP3]: ClearWasteSpaceFromList, temp file created, and all info added!"
#define MMIMP3_9404_112_2_18_2_4_8_166 "[MMIMP3]: ClearWasteSpaceFromList, no entry, no space or no list!"
#define MMIMP3_9413_112_2_18_2_4_8_167 "[MMIMP3]: ClearWasteSpaceFromList, file rename failed"
#define MMIMP3_9419_112_2_18_2_4_8_168 "[MMIMP3]: ClearWasteSpaceFromList, del original file failed!"
#define MMIMP3_9451_112_2_18_2_4_8_169 "[MMIMP3]: GetTemptListName, entered!, dev_type= %d"
#define MMIMP3_9455_112_2_18_2_4_8_170 "PNULL == file_name_ptr || NULL == file_name_len_ptr"
#define MMIMP3_9495_112_2_18_2_4_8_171 "[MMIMP3]: GetTemptListName ok!"
#define MMIMP3_9500_112_2_18_2_4_8_172 "[MMIMP3]: GetTemptListName, name repeated!"
#define MMIMP3_9610_112_2_18_2_4_8_173 "[MMIMP3]: MMIMP3_InitCurPlayByEmptyList, result = %d!"
#define MMIMP3_9659_112_2_18_2_4_8_174 "[MMIMP3]: MMIMP3_CheckDeviceOfEnoughSpace: space_size = %d, In_dev = %d, Out_dev = %d, result = %d"
#define MMIMP3_9737_112_2_18_2_4_9_175 "[MMI MP3] MMIMP3_GetCurPlayFileInfo result = %d"
#define MMIMP3_10023_112_2_18_2_4_9_176 "[MMIMP3]: MMIMP3_GenNewMarkInfo, total_time = %ld, offset_in_time = %ld,offset_byte = %ld, total_len = %ld"
#define MMIMP3_10114_112_2_18_2_4_10_177 "[MMIMP3]: MMIMP3_LoadAllMusicMarkInfo, read_offset = %d"
#define MMIMP3_10123_112_2_18_2_4_10_178 "[MMIMP3]: MMIMP3_LoadAllMusicMarkInfo,MMIAPIFMM_ReadFile Fails"
#define MMIMP3_10167_112_2_18_2_4_10_179 "[MMIMP3] MMIMP3_LoadMusicDetailMarkInfo s_music_mark_info_ptr = %x,index = %d, total = %d"
#define MMIMP3_10204_112_2_18_2_4_10_180 "[MMIMP3]: MMIMP3_LoadMusicDetailMarkInfo, read_offset = %d"
#define MMIMP3_10213_112_2_18_2_4_10_181 "[MMIMP3]: MMIMP3_LoadMusicDetailMarkInfo,MMIAPIFMM_ReadFile Fails"
#define MMIMP3_10371_112_2_18_2_4_10_182 "[MMIMP3] MMIMP3_IsMarkInfoFileExist, result = %d, *mark_dev_ptr = %d!"
#define MMIMP3_10395_112_2_18_2_4_10_183 "[MMIMP3] MMIMP3_CreateMusicMarkInfoFile entered!"
#define MMIMP3_10485_112_2_18_2_4_11_184 "[MMIMP3] MMIMP3_CreateMusicMarkInfoFile result = %d!"
#define MMIMP3_10569_112_2_18_2_4_11_185 "[MMIMP3] MMIMP3_AddMusicMark, total_mark_num = %d,total_num_alloc = %d, result = %d"
#define MMIMP3_10582_112_2_18_2_4_11_186 "[MMIMP3] MMIMP3_AddMusicMark, write_offset = %d"
#define MMIMP3_10598_112_2_18_2_4_11_187 "[MMIMP3] MMIMP3_AddMusicMark, MMIAPIFMM_WriteFile Fails"
#define MMIMP3_10603_112_2_18_2_4_11_188 "[MMIMP3] MMIMP3_AddMusicMark, MMIAPIFMM_SetFilePointer Fails"
#define MMIMP3_10609_112_2_18_2_4_11_189 "[MMIMP3] MMIMP3_AddMusicMark, write_offset = %d"
#define MMIMP3_10627_112_2_18_2_4_11_190 "[MMIMP3] MMIMP3_AddMusicMark, MMIAPIFMM_WriteFile Fails"
#define MMIMP3_10632_112_2_18_2_4_11_191 "[MMIMP3] MMIMP3_AddMusicMark, MMIAPIFMM_SetFilePointer Fails"
#define MMIMP3_10637_112_2_18_2_4_11_192 "[MMIMP3] MMIMP3_AddMusicMark, begin to write head, result = %d"
#define MMIMP3_10665_112_2_18_2_4_11_193 "[MMIMP3] MMIMP3_AddMusicMark, result = %d"
#define MMIMP3_10690_112_2_18_2_4_11_194 "[MMIMP3]: MMIMP3_DelMusicMark, entered!"
#define MMIMP3_10722_112_2_18_2_4_11_195 "[MMIMP3]: MMIMP3_DelMusicMark, file_handle = %d,total_num_alloc = %d,total_num = %d"
#define MMIMP3_10733_112_2_18_2_4_11_196 "[MMIMP3]: MMIMP3_DelMusicMark, rearrange index info"
#define MMIMP3_10763_112_2_18_2_4_11_197 "[MMIMP3]: MMIMP3_DelMusicMark, writing head info, total_num_alloc = %d,total_num = %d"
#define MMIMP3_10772_112_2_18_2_4_11_198 "[MMIMP3]: MMIMP3_DelMusicMark, write_offset = %d"
#define MMIMP3_10856_112_2_18_2_4_12_199 "[MMIMP3] MMIMP3_UpdateMusicMark, total_mark_num = %d,total_num_alloc = %d, result = %d"
#define MMIMP3_10866_112_2_18_2_4_12_200 "[MMIMP3] MMIMP3_UpdateMusicMark, write_offset = %d"
#define MMIMP3_10881_112_2_18_2_4_12_201 "[MMIMP3] MMIMP3_UpdateMusicMark, MMIAPIFMM_WriteFile Fails"
#define MMIMP3_10886_112_2_18_2_4_12_202 "[MMIMP3] MMIMP3_UpdateMusicMark, MMIAPIFMM_SetFilePointer Fails"
#define MMIMP3_10898_112_2_18_2_4_12_203 "[MMIMP3] MMIMP3_UpdateMusicMark, result = %d"
#define MMIMP3_10923_112_2_18_2_4_12_204 "[MMIMP3]: MMIMP3_GetFileIndexInList, entered"
#define MMIMP3_10930_112_2_18_2_4_12_205 "[MMIMP3]: MMIMP3_GetFileIndexInList, file_handle = %d,total_num_alloc = %d,total_num = %d"
#define MMIMP3_10953_112_2_18_2_4_12_206 "[MMIMP3]: MMIMP3_GetFileIndexInList, reading body info"
#define MMIMP3_11010_112_2_18_2_4_12_207 "[MMIMP3]: MMIMP3_GetMusicFilter, filter = %s, orig_filter = %s"
#define MMIMP3_11120_112_2_18_2_4_12_208 "[MMIMP3]: MMIMP3_CheckDir: In_dev = %d, sfs_error = %d, result = %d"
#define MMIMP3_11178_112_2_18_2_4_12_209 "[MMIMP3]:MMIMP3_ResetReviewInfo entry"
#define MMIMP3_11209_112_2_18_2_4_12_210 "[MMIMP3]:MMIMP3_SetReviewStartPoint result = %d"
#define MMIMP3_11270_112_2_18_2_4_12_211 "[MMIMP3]:MMIMP3_SetReviewEndPoint result = %d"
#define MMIMP3_11303_112_2_18_2_4_13_212 "[MMIMP3]:MMIMP3_StartReview audio state = %d, player_state = %d"
#define MMIMP3_11328_112_2_18_2_4_13_213 "MMIMP3_StartReview:audio seek result = %d"
#define MMIMP3_11364_112_2_18_2_4_13_214 "[MMIMP3]:MMIMP3_ExitReview audio state = %d, player_state = %d"
#define MMIMP3_11405_112_2_18_2_4_13_215 "MMIMP3_ExitReview:audio seek result = %d"
#define MMIMP3_11414_112_2_18_2_4_13_216 "MMIMP3_ExitReview:audio MMIMP3_PlayMp3 FAILS"
#define MMIMP3_11440_112_2_18_2_4_13_217 "[MMIMP3]:MMIMP3_HandleMp3ReviewPlay entry"
#define MMIMP3_11533_112_2_18_2_4_13_218 "[MMIMP3]: MMIMP3_InitCurPlayInfoByFileInfo, file_num = %d, file_index = %d"
#define MMIMP3_11573_112_2_18_2_4_13_219 "[MMIMP3] MMIMP3_InitCurPlayInfoByFileInfo MMIAPIFILEARRAY_Create fails"
#define MMIMP3_11691_112_2_18_2_4_13_220 "[MMIMP3]: MMIMP3_InitCurPlayInfoByFileInfo, result = %d, music_index = %d"
#define MMIMP3_11719_112_2_18_2_4_14_221 "[MMIMP3] MMIMP3_InitCurPlayInfoByFile MMIAPIFILEARRAY_Create fails"
#define MMIMP3_11868_112_2_18_2_4_14_222 "[MMIMP3]: FATAL ERROR"
#define MMIMP3_11901_112_2_18_2_4_14_223 "[MMIMP3]: MMIMP3_IsSupportA2DPForCurPlay, result = %d"
#define MMIMP3_11971_112_2_18_2_4_14_224 "[MMIMP3]: MMIAPIMP3_IsFileSupportA2DP, audio_type = %d, result = %d"
#define MMIMP3_12398_112_2_18_2_4_15_225 "[MMIMP3]: NotifyInitPlayerFinish"
#define MMIMP3_12417_112_2_18_2_4_15_226 "[MMIMP3]: NotifyInitPlayerNextStep"
#define MMIMP3_12562_112_2_18_2_4_15_227 "[MMIMP3]: MMIMP3_StopInitPlayer, step =%x"
#define MMIMP3_12646_112_2_18_2_4_15_228 "[MMIMP3]: MMIMP3_GetCurPlayListFileInfo, file_index = %d,total_num = %d,list_type = %d"
#define MMIMP3_12697_112_2_18_2_4_15_229 "[MMIMP3] MMIAPIMP3_GetPreOrNextMp3Index Total_num=%d active_index=%d"
#define MMIMP3_12701_112_2_18_2_4_15_230 "[MMIMP3] MMIAPIMP3_GetPreOrNextMp3Index Total_num = %d"
#define MMIMP3_BT_70_112_2_18_2_4_17_231 "MMIMP3 MMIMP3_BTEventPlayOrPause, player_state = %d, audio_state = %d "
#define MMIMP3_BT_74_112_2_18_2_4_17_232 "MMIMP3 MMIMP3_BTEventPlayOrPause, Not through bt!!!! "
#define MMIMP3_BT_119_112_2_18_2_4_17_233 "MMIMP3  MMIMP3_BTEventPre"
#define MMIMP3_BT_123_112_2_18_2_4_17_234 "MMIMP3  MMIMP3_BTEventPre, Not through bt!!!! "
#define MMIMP3_BT_142_112_2_18_2_4_17_235 "MMIMP3  MMIMP3_BTEventNext"
#define MMIMP3_BT_146_112_2_18_2_4_17_236 "MMIMP3  MMIMP3_BTEventNext, Not through bt!!!! "
#define MMIMP3_LYRIC_451_112_2_18_2_4_18_237 "[MMIMP3]:FreeLyricBuffer"
#define MMIMP3_LYRIC_513_112_2_18_2_4_18_238 "[MMIMP3]:lyric AllocTagInfoBuffer"
#define MMIMP3_LYRIC_668_112_2_18_2_4_19_239 "[MMIMP3] Alloc lyric section_id = %d "
#define MMIMP3_LYRIC_746_112_2_18_2_4_19_240 "FindNextTag,tag_value_ptr1 = 0x%x,*tag_len_ptr1 = %d ,tag_value_ptr2 = 0x%x,*tag_len_ptr2 = %d"
#define MMIMP3_LYRIC_1029_112_2_18_2_4_19_241 "FindDigitalPart, input error, tag_value_ptr = %x, tag_len = %d,integer_value_ptr = 0x%x,*integer_len_ptr = %d ,decimal_value_ptr = 0x%x,*decimal_len_ptr = %d"
#define MMIMP3_LYRIC_1444_112_2_18_2_4_21_242 "[MMIMP3]:GetOtherCodeLenFromUsc,wstr_len = %d,ERROR!!!!!!!"
#define MMIMP3_LYRIC_1589_112_2_18_2_4_21_243 "[MMIMP3]: GetLyricTextFileType, type = %d"
#define MMIMP3_LYRIC_1642_112_2_18_2_4_21_244 "[MMIMP3]: GetLyricTextFileType, type = %d, result = %d"
#define MMIMP3_LYRIC_1836_112_2_18_2_4_21_245 "[MMIMP3]:ReadLyricSentence, exit, sentence_index = %d, next_pos =%d, num = %d, sentence_id = %d, %d, %d, %d, %d ,%d, %d,length = %d, array_index = %d"
#define MMIMP3_LYRIC_1969_112_2_18_2_4_22_246 "[MMIMP3]: OpenLyricFile: No music file!"
#define MMIMP3_LYRIC_2055_112_2_18_2_4_22_247 "[MMIMP3]: lyric AllocParseBuffer"
#define MMIMP3_LYRIC_2110_112_2_18_2_4_22_248 "PNULL == lyric_name_ptr"
#define MMIMP3_LYRIC_2147_112_2_18_2_4_22_249 "PNULL == tag_ptr1 || PNULL == tag_ptr2"
#define MMIMP3_LYRIC_2645_112_2_18_2_4_23_250 "[MMIMP3]:MMIMP3 DisplayLyricImmediately  cur_time_tag_index = %d!"
#define MMIMP3_LYRIC_3093_112_2_18_2_4_24_251 "[MMIMP3]:StartLyricTimer: timer_id = %d,time_differ = %d!"
#define MMIMP3_LYRIC_3139_112_2_18_2_4_24_252 "MMIMP3_PauseLyric: pause timer fail!"
#define MMIMP3_LYRIC_3158_112_2_18_2_4_24_253 "MMK_ResumeTimer: resume timer fail!"
#define MMIMP3_LYRIC_3205_112_2_18_2_4_24_254 "MMIMP3_SeekLyric:mp3_offset = %d,time_total_offset = %d,time_tag_num = %d"
#define MMIMP3_LYRIC_3210_112_2_18_2_4_24_255 "MMIMP3_SeekLyric:i = %d,time = %d,lyric_time_offset = %d"
#define MMIMP3_LYRIC_3229_112_2_18_2_4_24_256 "MMIMP3_SeekLyric:lyric_cur_time_tag_index = %d,time_differ = %d"
#define MMIMP3_MUSIC_1643_112_2_18_2_4_29_257 "PNULL == music_file_info_ptr"
#define MMIMP3_MUSIC_1722_112_2_18_2_4_29_258 "[MMMIMP3 MUSIC] MMIAPIMP3_PlayMyDocumentMusic:memory = %d,name_len = %d"
#define MMIMP3_MUSIC_1747_112_2_18_2_4_29_259 "[MMIMP3_MUSIC]: ConsumeDrmRight"
#define MMIMP3_MUSIC_1759_112_2_18_2_4_29_260 "MMIMP3 MusicBTEventPlayOrPause"
#define MMIMP3_MUSIC_1773_112_2_18_2_4_29_261 "MMIMP3  MusicBTEventPre"
#define MMIMP3_MUSIC_1783_112_2_18_2_4_29_262 "MMIMP3  MusicBTEventNext"
#define MMIMP3_MUSIC_1827_112_2_18_2_4_29_263 "[MMMIMP3 MUSIC]  IsMusicA2dpSupport result = %d"
#define MMIMP3_MUSIC_1842_112_2_18_2_4_29_264 "[MMMIMP3 MUSIC] PauseMusic p_state = %d, a_state = %d, s_music_pause_type = %d!!!"
#define MMIMP3_MUSIC_1865_112_2_18_2_4_29_265 "[MMIMP3]:PauseMusic:AUDIO FAILED!"
#define MMIMP3_MUSIC_1910_112_2_18_2_4_29_266 "[MMIMP3 MUSIC] ResumeMusic player_state = %d, audio_state = %d"
#define MMIMP3_MUSIC_1944_112_2_18_2_4_29_267 "[MMIMP3 MUSIC]:ResumeMusic:AUDIO FAILED!"
#define MMIMP3_MUSIC_1975_112_2_18_2_4_29_268 "[MMIMP3 MUSIC] StopMusic p_state = %d, a_state = %d, s_music_pause_type = %d!"
#define MMIMP3_MUSIC_2034_112_2_18_2_4_30_269 "[MMIMP3] MMIMP3_IsMusicPausedByUser result = %d!!!"
#define MMIMP3_MUSIC_2064_112_2_18_2_4_30_270 "[MMIMP3]:HandleMusicPlayWinMsg:msg_id = 0x%x"
#define MMIMP3_MUSIC_2351_112_2_18_2_4_30_271 "PlayMusicFile param error!"
#define MMIMP3_MUSIC_2412_112_2_18_2_4_30_272 "PlayMusicFile, call_back = 0x%x,music_type = %d, result = %d"
#define MMIMP3_MUSIC_2438_112_2_18_2_4_30_273 "[MMIMP3 MUSIC] PlayMusic entered"
#define MMIMP3_MUSIC_2488_112_2_18_2_4_30_274 "[MMIMP3]PlayMusic, is_update = %d, is_a2dp = %d, play_offset = %d, cur_time = %d, cur_offset = %d, t_time = %d, t_len = %d"
#define MMIMP3_MUSIC_2694_112_2_18_2_4_31_275 "[MMIMP3 MUSIC] HandleMusicPlayWinWebKey:focus_button %d is error!"
#define MMIMP3_MUSIC_2962_112_2_18_2_4_31_276 "[MMIMP3 MUSIC] HandleMusicProgressBarTpKey:tp_x_offet = %d,tp_x = %d,progress_bar_rect.left = %d, mp3_offset = %d"
#define MMIMP3_MUSIC_2988_112_2_18_2_4_31_277 "[MMIMP3 MUSIC]MusicSeek: offset = %d"
#define MMIMP3_MUSIC_3006_112_2_18_2_4_31_278 "[MMIMP3]:GetMusicSeekStep, param error!"
#define MMIMP3_MUSIC_3026_112_2_18_2_4_32_279 "[MMIMP3 MUSIC]: GetMusicSeekStep:file_total_time = %d,file_total_len = %d,bitrate = %d, step_ptr = %ld"
#define MMIMP3_MUSIC_3067_112_2_18_2_4_32_280 "[MMIMP3]: MusicBackwardSeek:result = %d"
#define MMIMP3_MUSIC_3108_112_2_18_2_4_32_281 "[MMIMP3]: MusicForwardSeek:result = %d"
#define MMIMP3_MUSIC_3119_112_2_18_2_4_32_282 "[MMIMP3]: StartMusicSeekTimer"
#define MMIMP3_MUSIC_3133_112_2_18_2_4_32_283 "[MMIMP3]: StopMusicSeekTimer"
#define MMIMP3_MUSIC_3183_112_2_18_2_4_32_284 "[MMIMP3]: music  HandleMusicTpDragTimer enter"
#define MMIMP3_MUSIC_3205_112_2_18_2_4_32_285 "[MMIMP3]: music StartTpDragTimer"
#define MMIMP3_MUSIC_3213_112_2_18_2_4_32_286 "[MMIMP3]: music StartTpDragTimer: the timer has started!"
#define MMIMP3_MUSIC_3230_112_2_18_2_4_32_287 "[MMIMP3]: music StopTpDragTimer: the timer has stop!"
#define MMIMP3_MUSIC_3957_112_2_18_2_4_34_288 "[MMIMP3]:OpenMusicOsdPanel is already opened!"
#define MMIMP3_MUSIC_4054_112_2_18_2_4_34_289 "[MMIMP3]:MMIMP3_IsOpenMusicOsdPanel osd panel not open"
#define MMIMP3_MUSIC_4149_112_2_18_2_4_34_290 "[MMIMP3] SetAnimParam addr:0x%x  size%d"
#define MMIMP3_MUSIC_4424_112_2_18_2_4_35_291 "[MMIMP3]: HandleOsdPanelTimer enter"
#define MMIMP3_SORT_311_112_2_18_2_4_36_292 "[MMIMP3]: MMIMP3_SortMp3List, entered"
#define MMIMP3_SORT_381_112_2_18_2_4_36_293 "[MMIMP3]: HandleMp3SortMp3ListWaitingWin, msg=%x"
#define MMIMP3_SORT_512_112_2_18_2_4_36_294 "[MMIMP3]: SortMp3ListByType, open file, list_name_len = %d"
#define MMIMP3_SORT_527_112_2_18_2_4_36_295 "[MMIMP3]: SortMp3ListByType, file_handle = %d,total_num_alloc = %d,total_num = %d"
#define MMIMP3_SORT_606_112_2_18_2_4_36_296 "[MMIMP3]: SortMp3ListByType, writing index info"
#define MMIMP3_SORT_631_112_2_18_2_4_36_297 "MMIMP3_EndSort is_success = %d, file_handle = %x, list_index_ptr = %x"
#define MMIMP3_WINTAB_2595_112_2_18_2_4_42_298 "[MMIMP3]:HandlePlayerMenuWinMsg, mmenu_id = %d error!"
#define MMIMP3_WINTAB_2639_112_2_18_2_4_42_299 "OpenMp3MainWin"
#define MMIMP3_WINTAB_2953_112_2_18_2_4_43_300 "[MMIMP3]:NextBtnCallback "
#define MMIMP3_WINTAB_3590_112_2_18_2_4_44_301 "[MMIMP3] HandleMp3OpenWaitingWin: msg = 0x%x"
#define MMIMP3_WINTAB_3887_112_2_18_2_4_45_302 "[MMIMP3] HandlePenLongOk: MSG_CTL_PENLONGOK"
#define MMIMP3_WINTAB_4336_112_2_18_2_4_45_303 "[MMIMP3] HandleReviewMenu seek not supportted"
#define MMIMP3_WINTAB_4492_112_2_18_2_4_46_304 "[MMIMP3] SetMp3AlertInfo type = %d, callback = 0x%x, win_id = %x"
#define MMIMP3_WINTAB_4543_112_2_18_2_4_46_305 "[MMIMP3]: StartMp3AlertTimer enter"
#define MMIMP3_WINTAB_4550_112_2_18_2_4_46_306 "[MMIMP3]: StartMp3AlertTimer: timer started!"
#define MMIMP3_WINTAB_4562_112_2_18_2_4_46_307 "[MMIMP3]: StopMp3AlertTimer enter"
#define MMIMP3_WINTAB_4570_112_2_18_2_4_46_308 "[MMIMP3]: StopMp3AlertTimer: timer stopped already!"
#define MMIMP3_WINTAB_4597_112_2_18_2_4_46_309 "[MMIMP3]: HandleMp3AlertTimer enter"
#define MMIMP3_WINTAB_5924_112_2_18_2_4_49_310 "MMIMP3_DisplayProgress:uiCurOffset = %d,uiTotalLength = %d,uiCurTime = %d,uiTotalTime = %d,cur_slide = %d,review_start = %d, review_end = %d"
#define MMIMP3_WINTAB_6173_112_2_18_2_4_49_311 "[MMI MP3] HandleMp3MainWinWebKey"
#define MMIMP3_WINTAB_6301_112_2_18_2_4_50_312 "[MMIMP3]: HandleMp3TpDragTimer enter"
#define MMIMP3_WINTAB_6323_112_2_18_2_4_50_313 "[MMIMP3]: StartTpDragTimer"
#define MMIMP3_WINTAB_6331_112_2_18_2_4_50_314 "[MMIMP3]: StartTpDragTimer: the timer has started!"
#define MMIMP3_WINTAB_6348_112_2_18_2_4_50_315 "[MMIMP3]: StopTpDragTimer: the timer has stop!"
#define MMIMP3_WINTAB_6621_112_2_18_2_4_50_316 "left_index < btn_index, left_index = %d, btn_index = %d"
#define MMIMP3_WINTAB_6636_112_2_18_2_4_50_317 "right_index > btn_index, right_index = %d, btn_index = %d"
#define MMIMP3_WINTAB_7876_112_2_18_2_4_53_318 "PNULL == item_ptr"
#define MMIMP3_WINTAB_7974_112_2_18_2_4_53_319 "MMIAPIMP3_StopOperaSDPlug"
#define MMIMP3_WINTAB_7994_112_2_18_2_4_53_320 "MMIAPIMP3_StopOperaBtAction"
#define MMIMP3_WINTAB_8019_112_2_18_2_4_53_321 "[MMIMP3]: HandleMp3SeekTimer enter"
#define MMIMP3_WINTAB_8050_112_2_18_2_4_53_322 "[MMI MP3]: StartForwardOrBackwardTimer"
#define MMIMP3_WINTAB_8058_112_2_18_2_4_53_323 "StartForwardOrBackwardTimer: the key timer has started!"
#define MMIMP3_WINTAB_8068_112_2_18_2_4_53_324 "[MMIMP3]: StopForwardOrBackwardTimer s_mp3_forward_backward_timer_id=%d"
#define MMIMP3_WINTAB_8076_112_2_18_2_4_53_325 "StopForwardOrBackwardTimer: the key timer has stop!"
#define MMIMP3_WINTAB_8568_112_2_18_2_4_54_326 "[MMIMP3] PDA MMIMP3_GetDisplayStyle error!"
#define MMIMP3_WINTAB_8999_112_2_18_2_4_56_327 "[MMIMP3] HandleMp3PlayListListWinMsg, cur_list_index = %d,total_num = %d, msg = 0x%x"
#define MMIMP3_WINTAB_9021_112_2_18_2_4_56_328 " PNULL != need_item_data_ptr "
#define MMIMP3_WINTAB_9026_112_2_18_2_4_56_329 "[MMIMP3] HandleMp3PlayListList,MSG_CTL_LIST_NEED_ITEM_DATA; index = %d"
#define MMIMP3_WINTAB_9338_112_2_18_2_4_57_330 "[MMIMP3] HandleMp3PlayListListOptWinMsg, cur_list_index = %d,total_num = %d, msg = 0x%x"
#define MMIMP3_WINTAB_9613_112_2_18_2_4_58_331 "[MMIMP3]: HandleMp3CopyListWaitingWin, msg=%x"
#define MMIMP3_WINTAB_9638_112_2_18_2_4_58_332 "[MMIMP3]:HandleMp3CopyListWaitingWin s_mp3_list_dest_dev=%d,error!!!!!!!!!!"
#define MMIMP3_WINTAB_9720_112_2_18_2_4_58_333 "[MMIMP3]: MMIMP3_UpdatelistDisplay, list mark enabled"
#define MMIMP3_WINTAB_9769_112_2_18_2_4_58_334 "[MMIMP3] HandleMp3PlayList, msg = 0x%x"
#define MMIMP3_WINTAB_9884_112_2_18_2_4_58_335 " PNULL != need_item_data_ptr "
#define MMIMP3_WINTAB_9903_112_2_18_2_4_58_336 "[MMIMP3]:HandleMp3PlayList; MSG_MP3_UPDATE_LIST cur_list_selection = %d"
#define MMIMP3_WINTAB_9930_112_2_18_2_4_58_337 "[MMIMP3]: HandleMp3PlayListWinMsg, WRONG!!! new_sel = %d, total_num = %d"
#define MMIMP3_WINTAB_11840_112_2_18_2_5_3_338 "[MMIMP3]: ConstructAddToSrcInfoFromList, list_sel = %d, select_num = %d, result = %d"
#define MMIMP3_WINTAB_11900_112_2_18_2_5_3_339 "[MMIMP3]: ConstructAddToSrcInfoFromCurPlay, list_sel = %d,result = %d"
#define MMIMP3_WINTAB_11931_112_2_18_2_5_3_340 "[MMIMP3]: ConstructlistAddToDstInfo, list_index = %d, result = %d"
#define MMIMP3_WINTAB_11966_112_2_18_2_5_3_341 "[MMIMP3]: HandleMp3ListAddToWaitingWin, msg=%x"
#define MMIMP3_WINTAB_11985_112_2_18_2_5_3_342 "[MMIMP3]: HandleMp3ListAddToWaitingWin,param error"
#define MMIMP3_WINTAB_12160_112_2_18_2_5_3_343 "[MMIMP3]: ConstructListDeleteInfo, list_sel = %d, select_num = %d, is_del = %d, result = %d"
#define MMIMP3_WINTAB_12184_112_2_18_2_5_4_344 "[MMIMP3]: GetCurOprType, opr_type = %d"
#define MMIMP3_WINTAB_12195_112_2_18_2_5_4_345 "[MMIMP3]: ClearOprInfo, opr_type = %d"
#define MMIMP3_WINTAB_12273_112_2_18_2_5_4_346 "[MMIMP3]: HandleMp3ListDeleteWaitingWin, msg=%x"
#define MMIMP3_WINTAB_12313_112_2_18_2_5_4_347 "[MMIMP3]: HandleMp3ListDeleteWaitingWin,param error"
#define MMIMP3_WINTAB_12517_112_2_18_2_5_4_348 " PNULL != need_item_data_ptr "
#define MMIMP3_WINTAB_12544_112_2_18_2_5_4_349 "[MMIMP3]: HandleMp3AddToListWinMsg, WRONG!!! cur_sel = %d, total_num = %d"
#define MMIMP3_WINTAB_12883_112_2_18_2_5_5_350 "*file_dev_ptr = %d is error!"
#define MMIMP3_WINTAB_13010_112_2_18_2_5_5_351 "[MMIMP3]: HandleMp3NewlistAlertWin, msg=%x"
#define MMIMP3_WINTAB_13308_112_2_18_2_5_6_352 " PNULL != need_item_data_ptr "
#define MMIMP3_WINTAB_13341_112_2_18_2_5_6_353 "[MMIMP3]:HandleMp3MusicMarkList, MSG_MP3_UPDATE_MARK_LIST,new_selection = %d"
#define MMIMP3_WINTAB_13491_112_2_18_2_5_6_354 "[MMIMP3]Begin to play mark, offset = %ld"
#define MMIMP3_WINTAB_14489_112_2_18_2_5_8_355 "[MMIMP3]: ConstructMarkDeleteInfo, list_sel = %d, select_num = %d, is_del = %d, result = %d"
#define MMIMP3_WINTAB_14526_112_2_18_2_5_8_356 "[MMIMP3]: HandleMarkDeleteWaitingWin, msg=%x"
#define MMIMP3_WINTAB_14542_112_2_18_2_5_8_357 "[MMIMP3]: HandleMarkDeleteWaitingWin,param error"
#define MMIMP3_WINTAB_15322_112_2_18_2_5_10_358 "[MMIMP3] InitPlayListItem, is_path_ok = %d, list_num = %d"
#define MMIMP3_WINTAB_15831_112_2_18_2_5_11_359 "[MMIMP3] HandleMp3PlayListListLongOkOptWinMsg, cur_list_index = %d,total_num = %d, msg = 0x%x"
#define MMIMP3_WINTAB_17660_112_2_18_2_5_15_360 "[MMIMP3]:OpenMp3OsdPanel is already opened!"
#define MMIMP3_WINTAB_17760_112_2_18_2_5_15_361 "[MMIMP3]:OpenMp3OsdPanel osd panel not open"
#define MMIMP3_WINTAB_18043_112_2_18_2_5_15_362 "[MMIMP3]: GetAlbumInfo check file error! file_index=%d"
#define MMIMP3_WINTAB_18080_112_2_18_2_5_16_363 "[MMIMP3]: MMIMP3_GetAlbumInfo img_data_ptr not NULL !, addr=0x%x"
#define MMIMP3_WINTAB_18086_112_2_18_2_5_16_364 "[MMIMP3]: MMIMP3_GetAlbumInfo apic_offset=%d apic_length=%d"
#define MMIMP3_WINTAB_18107_112_2_18_2_5_16_365 "[MMIMP3]: MMIMP3_GetAlbumInfo GetAlbumImgDataFromFile fail!"
#define MMIMP3_WINTAB_18116_112_2_18_2_5_16_366 "[MMIMP3]: MMIMP3_GetAlbumInfo img_data_ptr alloc failed!!!"
#define MMIMP3_WINTAB_18123_112_2_18_2_5_16_367 "[MMIMP3]: MMIMP3_GetAlbumInfo file not exist!"
#define MMIMP3_WINTAB_18126_112_2_18_2_5_16_368 "[MMIMP3]: MMIMP3_GetAlbumInfo result=%d"
#define MMIMP3_WINTAB_18158_112_2_18_2_5_16_369 "[MMIMP3]: GetAlbumWcharDescInfo artist str_len=%d"
#define MMIMP3_WINTAB_18178_112_2_18_2_5_16_370 "[MMIMP3]: GetAlbumWcharDescInfo album str_len=%d"
#define MMIMP3_WINTAB_18211_112_2_18_2_5_16_371 "[MMIMP3]: GetAlbumImgDataFromFile apic_offset=%d, apic_len=%d buf_len=%d"
#define MMIMP3_WINTAB_18217_112_2_18_2_5_16_372 "[MMIMP3]: GetAlbumImgDataFromFile create file error!"
#define MMIMP3_WINTAB_18224_112_2_18_2_5_16_373 "[MMIMP3]: GetAlbumImgDataFromFile set pos error!"
#define MMIMP3_WINTAB_18231_112_2_18_2_5_16_374 "[MMIMP3]: GetAlbumImgDataFromFile min=%d"
#define MMIMP3_WINTAB_18236_112_2_18_2_5_16_375 "[MMIMP3]: GetAlbumImgDataFromFile data len =%d"
#define MMIMP3_WINTAB_18241_112_2_18_2_5_16_376 "[MMIMP3]: GetAlbumImgDataFromFile bytes to read error!"
#define MMIMP3_WINTAB_18246_112_2_18_2_5_16_377 "[MMIMP3]: GetAlbumImgDataFromFile read file error!"
#define MMIMP3_WINTAB_18278_112_2_18_2_5_16_378 "[MMIMP3] GetPreMp3AlbumInfo"
#define MMIMP3_WINTAB_18286_112_2_18_2_5_16_379 "[MMIMP3] GetPreMp3AlbumInfo get pre index error"
#define MMIMP3_WINTAB_18299_112_2_18_2_5_16_380 "[MMIMP3] GetCurMp3AlbumInfo"
#define MMIMP3_WINTAB_18307_112_2_18_2_5_16_381 "[MMIMP3] GetCurMp3AlbumInfo total_num == 0"
#define MMIMP3_WINTAB_18319_112_2_18_2_5_16_382 "[MMIMP3] GetNextMp3AlbumInfo"
#define MMIMP3_WINTAB_18327_112_2_18_2_5_16_383 "[MMIMP3] GetNextMp3AlbumInfo get next index error"
#define MMIMP3_WINTAB_18394_112_2_18_2_5_16_384 "[MMIMP3] ReleaseAllAlbumInfo pre-addr:0x%x size%d cur-addr:0x%x size%d next-addr:0x%x size%d"
#define MMIMP3_WINTAB_18427_112_2_18_2_5_16_385 "[MMIMP3] ReleaseAlbumInfo, is_prev = %d"
#define MMIMP3_WINTAB_18486_112_2_18_2_5_16_386 "[MMIMP3] SetAnimParam addr:0x%x  size%d"
#define MMIMP3_WINTAB_18639_112_2_18_2_5_17_387 "[MMIMP3]: HandleAlbumImgGlideTpDown style=%d total_num=%d"
#define MMIMP3_WINTAB_19156_112_2_18_2_5_18_388 "[MMIMP3]: HandleOsdPanelTimer enter"
#define MMIMP3_WINTAB_19240_112_2_18_2_5_18_389 "[MMIMP3]: OpenCurPlayList is already opened!"
#define MMIMP3_WINTAB_19337_112_2_18_2_5_18_390 "[MMIMP3] HandleMp3CurPlayListWinMsg, msg = 0x%x"
#define MMIMP3_WINTAB_19451_112_2_18_2_5_18_391 " PNULL != need_item_data_ptr "
#define MMIMP3_WINTAB_19470_112_2_18_2_5_18_392 "[MMIMP3]:HandleMp3CurPlayListWinMsg; MSG_MP3_UPDATE_LIST cur_list_selection = %d"
#define MMIMP3_WINTAB_19497_112_2_18_2_5_18_393 "[MMIMP3]: HandleMp3CurPlayListWinMsg, WRONG!!! new_sel = %d, total_num = %d"
#define MMIMP3_WINTAB_19781_112_2_18_2_5_19_394 "mmi MP3Applet_HandleEvent msg_id = 0x%04x"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_AUDIOPLAYER_TRC)
TRACE_MSG(MMIMP3_977_112_2_18_2_3_50_0,"[MMIMP3_ReqPlaySrv]: s_mp3bg_play_handle exist!")
TRACE_MSG(MMIMP3_989_112_2_18_2_3_50_1,"[MMIMP3_ReqPlaySrv]: MMISRVAUD_SetAudioInfo error!!!!!!")
TRACE_MSG(MMIMP3_1014_112_2_18_2_3_50_2,"[MMIMP3_ReqPlaySrv]: MMISRVMGR_Request error!!!!!!")
TRACE_MSG(MMIMP3_1018_112_2_18_2_3_50_3,"MMIMP3_ReqPlaySrv: s_mp3bg_play_handle=0x%x")
TRACE_MSG(MMIMP3_1175_112_2_18_2_3_50_4,"[MMIMP3] MMIMP3_SetDisplayStyle style = %d")
TRACE_MSG(MMIMP3_1212_112_2_18_2_3_50_5,"[MMIMP3] MMIMP3_GetDisplayStyle style = %d")
TRACE_MSG(MMIMP3_1229_112_2_18_2_3_50_6,"[MMIMP3]:MMIMP3_SetRepeatMode, param error!")
TRACE_MSG(MMIMP3_1259_112_2_18_2_3_50_7,"[MMIMP3]:MMIMP3_SetShuffleMode, param error!")
TRACE_MSG(MMIMP3_1354_112_2_18_2_3_50_8,"[MMIMP3] MMIAPIMP3_IsThroughBt = %d")
TRACE_MSG(MMIMP3_1494_112_2_18_2_3_51_9,"[MMIMP3]:MMIAPIMP3_SetMp3EqMode, eq_mode = %d!")
TRACE_MSG(MMIMP3_1534_112_2_18_2_3_51_10,"[MMIMP3] MMIMP3_DelayPlayMp3 encounter MMIMP3_PLAYER_SWITCH_PAUSED")
TRACE_MSG(MMIMP3_1553_112_2_18_2_3_51_11,"[MMIMP3] MMIMP3_DelayPlayMp3 Start Timer = %d, offset = %d")
TRACE_MSG(MMIMP3_1575_112_2_18_2_3_51_12,"[MMIMP3] MMIMP3_StopDelayPlayTimer Stop Timer")
TRACE_MSG(MMIMP3_1602_112_2_18_2_3_51_13,"[MMIMP3] MMIAPIMP3_HandleDelayPlayTimer encounter MMIMP3_PLAYER_SWITCH_PAUSED")
TRACE_MSG(MMIMP3_1610_112_2_18_2_3_51_14,"[MMIMP3] MMIAPIMP3_HandleDelayPlayTimer result = %d, p_state = %d, a_state = %d!!!")
TRACE_MSG(MMIMP3_1629_112_2_18_2_3_51_15,"[MMIMP3] MMIMP3_IsPausedByUser result = %d!!!")
TRACE_MSG(MMIMP3_1646_112_2_18_2_3_51_16,"[MMIMP3]: ConsumeDrmRight index = %d")
TRACE_MSG(MMIMP3_1696_112_2_18_2_3_51_17,"[MMIMP3] Mp3AlertCallback enter")
TRACE_MSG(MMIMP3_1907_112_2_18_2_3_51_18,"[MMIMP3 DRM] MMIAPIMP3_IsDrmFile :drm_audio_type = %d,is_audio_drm_file = %d, is_drm_valid = %d, result = %d")
TRACE_MSG(MMIMP3_1940_112_2_18_2_3_51_19,"[MMIMP3] MMIMP3_PlayMp3 active_index = %d, total_num = %d")
TRACE_MSG(MMIMP3_1961_112_2_18_2_3_51_20,"[MMIMP3] MMIMP3_PlayMp3 is drm file")
TRACE_MSG(MMIMP3_1973_112_2_18_2_3_51_21,"[MMIMP3] MMIMP3_PlayMp3 :is_file_exist = %d,file_size = %d, is_a2dp = %d, file_support_a2sp = %d")
TRACE_MSG(MMIMP3_2070_112_2_18_2_3_52_22,"[MMIMP3]:MMIMP3_PlayMp3: uiTotalTime = %d, total_length = %d, bit_rate = %d")
TRACE_MSG(MMIMP3_2144_112_2_18_2_3_52_23,"[MMIMP3]:MMIMP3_PlayMp3, result = %d")
TRACE_MSG(MMIMP3_2157_112_2_18_2_3_52_24,"[MMIMP3] MMIAPIMP3_PauseMp3 player_state = %d, audio_state = %d, s_mp3_pause_type = %d!!!")
TRACE_MSG(MMIMP3_2187_112_2_18_2_3_52_25,"[MMIMP3]:MMIAPIMP3_PauseMp3:AUDIO FAILED!")
TRACE_MSG(MMIMP3_2214_112_2_18_2_3_52_26,"[MMIMP3]:MMIAPIMP3_PauseMp3: result = %d")
TRACE_MSG(MMIMP3_2241_112_2_18_2_3_52_27,"[MMIMP3] MMIAPIMP3_ResumeMp3 player_state = %d, audio_state = %d")
TRACE_MSG(MMIMP3_2275_112_2_18_2_3_52_28,"[MMIMP3]:MMIAPIMP3_ResumeMp3:AUDIO FAILED!")
TRACE_MSG(MMIMP3_2368_112_2_18_2_3_52_29,"[MMIMP3] MMIMP3_StopMp3 player_state = %d, audio_state = %d")
TRACE_MSG(MMIMP3_2439_112_2_18_2_3_52_30,"StartMp3PlayTimer:mp3_time = %d,time_out = %d,s_mp3_progress_timer_id = %d")
TRACE_MSG(MMIMP3_2663_112_2_18_2_3_53_31,"[MMIMP3]:PlayCallBack handle=0x%x, s_mp3bg_play_handle = 0x%x")
TRACE_MSG(MMIMP3_2666_112_2_18_2_3_53_32,"[MMIMP3]:PlayCallBack event=%d")
TRACE_MSG(MMIMP3_2683_112_2_18_2_3_53_33,"[MMIMP3]:PlayCallBack report=%d,data=%d")
TRACE_MSG(MMIMP3_2760_112_2_18_2_3_53_34,"[MMIMP3]:MMIMP3_CallBackAfterPlay:result = %d")
TRACE_MSG(MMIMP3_2802_112_2_18_2_3_53_35,"[MMIMP3] MMIAPIMP3_PlayMp3CallBack cur_play_mode = %d,review_state = %d,PlayerState = %d")
TRACE_MSG(MMIMP3_2925_112_2_18_2_3_53_36,"[MMIMP3] MMIAPIMP3_GoToPreOrNextMp3 cur_play_mode = %d,review_state = %d, player_state = %d,audio_state = %d")
TRACE_MSG(MMIMP3_2929_112_2_18_2_3_53_37,"[MMIMP3] MMIAPIMP3_GoToPreOrNextMp3 Total_num = %d")
TRACE_MSG(MMIMP3_3069_112_2_18_2_3_54_38,"[MMIMP3]:GetNextMp3Index, next_index_ptr = %x, random_info_ptr = %x!")
TRACE_MSG(MMIMP3_3149_112_2_18_2_3_54_39,"GetNextMp3Index:Don't call this function!")
TRACE_MSG(MMIMP3_3220_112_2_18_2_3_54_40,"[MMIMP3]:MMIAPIMP3_GetCurMp3Name, mp3_name_ptr = %x!")
TRACE_MSG(MMIMP3_3353_112_2_18_2_3_54_41,"MMIAPIMP3_ForwardMp3:get mp3 current offset result %d is error!")
TRACE_MSG(MMIMP3_3363_112_2_18_2_3_54_42,"[MMIMP3]:MMIMP3_ReviewForwardMp3 ReviewEnd")
TRACE_MSG(MMIMP3_3373_112_2_18_2_3_54_43,"MMIMP3_ReviewForwardMp3:get mp3 current offset result %d is error!")
TRACE_MSG(MMIMP3_3381_112_2_18_2_3_54_44,"MMIAPIMP3_ForwardMp3:cur_state %d don't handle!")
TRACE_MSG(MMIMP3_3408_112_2_18_2_3_54_45,"[MMIMP3]:GetReviewMp3CurrentOffset, mp3_offset_ptr = %x!")
TRACE_MSG(MMIMP3_3415_112_2_18_2_3_54_46,"GetReviewMp3CurrentOffset:file_total_time = %d,file_total_len = %d,bitrate = %d")
TRACE_MSG(MMIMP3_3435_112_2_18_2_3_54_47,"GetReviewMp3CurrentOffset:forward_backward_offset = %ld")
TRACE_MSG(MMIMP3_3468_112_2_18_2_3_54_48,"MMIAPIMP3_BackwardMp3:get mp3 current offset result %d is error!")
TRACE_MSG(MMIMP3_3482_112_2_18_2_3_55_49,"MMIMP3_ReviewBackwardMp3:get mp3 current offset result %d is error!")
TRACE_MSG(MMIMP3_3490_112_2_18_2_3_55_50,"MMIAPIMP3_BackwardMp3:cur_state %d don't handle!")
TRACE_MSG(MMIMP3_3509_112_2_18_2_3_55_51,"[MMIMP3]:GetMp3CurrentOffset, mp3_offset_ptr = %x!")
TRACE_MSG(MMIMP3_3516_112_2_18_2_3_55_52,"GetMp3CurrentOffset:file_total_time = %d,file_total_len = %d,bitrate = %d")
TRACE_MSG(MMIMP3_3532_112_2_18_2_3_55_53,"GetMp3CurrentOffset:forward_backward_offset = %ld")
TRACE_MSG(MMIMP3_3554_112_2_18_2_3_55_54,"[MMIMP3] ForwardOrBackwardMp3 mp3_offset = %d, cur_state = %d")
TRACE_MSG(MMIMP3_3589_112_2_18_2_3_55_55,"ForwardOrBackwardMp3: MMIMP3_AUDIO_PLAYING audio seek result = %d")
TRACE_MSG(MMIMP3_3622_112_2_18_2_3_55_56,"ForwardOrBackwardMp3: MMIMP3_AUDIO_PAUSED audio seek result = %d")
TRACE_MSG(MMIMP3_3699_112_2_18_2_3_55_57,"GetMp3CurrentTotalOffset:mp3_total_offset =%ld")
TRACE_MSG(MMIMP3_3735_112_2_18_2_3_55_58,"MMIMP3_HandleProgressBarTpKey:tp_x_offet = %d,tp_x = %d,progress_bar_rect.left = %d")
TRACE_MSG(MMIMP3_3759_112_2_18_2_3_55_59,"MMIMP3_HandleProgressBarTpKey:get mp3 current offset result %d is error!")
TRACE_MSG(MMIMP3_3795_112_2_18_2_3_55_60,"[MMIMP3]:GetMp3CurrentOffsetByTp, is_forward_ptr = %x,mp3_total_offset_ptr = %x,mp3_bitrate_ptr = %x!")
TRACE_MSG(MMIMP3_3805_112_2_18_2_3_55_61,"GetMp3CurrentOffsetByTp:cur_slide_x_offset = %d,tp_x_offset = %d")
TRACE_MSG(MMIMP3_3826_112_2_18_2_3_55_62,"GetMp3CurrentOffsetByTp:is_forward = %d,mp3_total_offset = %ld")
TRACE_MSG(MMIMP3_3853_112_2_18_2_3_55_63,"[MMIMP3]:MMIAPIMP3_IsMp3PlayerLoadType, full_path_ptr = %x,full_path_len = %d!")
TRACE_MSG(MMIMP3_3955_112_2_18_2_3_55_64,"[MMIMP3]: MMIMP3_IsFileSeekSupport, audio_type = %d, result = %d")
TRACE_MSG(MMIMP3_3980_112_2_18_2_3_56_65,"[MMIMP3]: MMIMP3_OpenFileCallBack, Enter,is_success = %d, file_num = %d, file_array = 0x%x!")
TRACE_MSG(MMIMP3_4020_112_2_18_2_3_56_66,"[MMIMP3]: MMIMP3_OpenFileCallBack, MMIMP3_CreateEmptyListFile ok")
TRACE_MSG(MMIMP3_4028_112_2_18_2_3_56_67,"[MMIMP3]: MMIMP3_OpenFileCallBack,AddFileInfoToListFromFMMArray, success!")
TRACE_MSG(MMIMP3_4034_112_2_18_2_3_56_68,"[MMIMP3]: AddFileInfoToListFromFMMArray, Fail!")
TRACE_MSG(MMIMP3_4084_112_2_18_2_3_56_69,"[MMIMP3]: MMIMP3_AddFileToListCallBack, Enter,is_success = %d, file_num = %d")
TRACE_MSG(MMIMP3_4150_112_2_18_2_3_56_70,"[MMIMP3]: SetListFileWrittenCheckInfo, file_handle = 0x%x,is_written_ok = %d,result = %d")
TRACE_MSG(MMIMP3_4184_112_2_18_2_3_56_71,"[MMIMP3]: GetListFileWrittenCheckInfo, file_handle = 0x%x,is_written_ok = %d,result = %d")
TRACE_MSG(MMIMP3_4218_112_2_18_2_3_56_72,"[MMIMP3]: ReadListFileHeadInfo, file_handle = 0x%x,oper_offset = %d, oper_size = %d, total_num = %d, total_num_alloc = %d, result = %d")
TRACE_MSG(MMIMP3_4252_112_2_18_2_3_56_73,"[MMIMP3]: WriteListFileHeadInfo, file_handle = 0x%x,oper_offset = %d, oper_size = %d, total_num = %d, total_num_alloc = %d, result = %d")
TRACE_MSG(MMIMP3_4289_112_2_18_2_3_56_74,"[MMIMP3]: ReadListFileEndInfo, file_handle = 0x%x,oper_offset = %d, oper_size = %d, total_num = %d, total_num_alloc = %d, result = %d")
TRACE_MSG(MMIMP3_4326_112_2_18_2_3_56_75,"[MMIMP3]: WriteListFileEndInfo, file_handle = 0x%x,oper_offset = %d, oper_size = %d, total_num = %d, total_num_alloc = %d, result = %d")
TRACE_MSG(MMIMP3_4354_112_2_18_2_3_56_76,"[MMIMP3]: ReadListFileIndexInfo, file_handle = 0x%x,start_index = %d, index_num = %d, total_num = %d, total_num_alloc = %d")
TRACE_MSG(MMIMP3_4363_112_2_18_2_3_56_77,"[MMIMP3]: ReadListFileIndexInfo, oper_offset = %d, oper_size = %d, *index_num_ptr = %d ")
TRACE_MSG(MMIMP3_4367_112_2_18_2_3_56_78,"[MMIMP3]: ReadListFileIndexInfo, ERROR, *index_num_ptr = %d, index_num = %d ")
TRACE_MSG(MMIMP3_4381_112_2_18_2_3_56_79,"[MMIMP3]: ReadListFileIndexInfo, file_handle = 0x%x,oper_offset = %d, oper_size = %d, total_num = %d, total_num_alloc = %d, file_index = %d, result = %d")
TRACE_MSG(MMIMP3_4430_112_2_18_2_3_56_80,"[MMIMP3]: WriteListFileIndexInfo, file_handle = 0x%x,oper_offset = %d,oper_size = %d,result = %d")
TRACE_MSG(MMIMP3_4466_112_2_18_2_3_57_81,"[MMIMP3]: ReadListFileItemInfo, file_handle = 0x%x,oper_offset = %d,oper_size = %d,result = %d")
TRACE_MSG(MMIMP3_4516_112_2_18_2_3_57_82,"[MMIMP3]: WriteListFileItemInfoInSequence, file_handle = 0x%x,item_num = %d,oper_offset = %d,oper_size = %d,result = %d")
TRACE_MSG(MMIMP3_4808_112_2_18_2_3_57_83,"[MMIMP3]: AddFileInfoToListFromFMMArray, open file, list_name_len = %d")
TRACE_MSG(MMIMP3_4838_112_2_18_2_3_57_84,"[MMIMP3]: AddFileInfoToListFromFMMArray, file_handle = %d,total_num_alloc = %d,total_num = %d")
TRACE_MSG(MMIMP3_4912_112_2_18_2_3_57_85,"[MMIMP3]: AddFileInfoToListFromFMMArray, append_body_num = %d,add_body_num = %d")
TRACE_MSG(MMIMP3_4933_112_2_18_2_3_57_86,"[MMIMP3]: AddFileInfoToListFromFMMArray, writing body info")
TRACE_MSG(MMIMP3_4973_112_2_18_2_3_58_87,"[MMIMP3]: AddFileInfoToListFromFMMArray,MMIAPIFMM_WriteFile Fails")
TRACE_MSG(MMIMP3_4990_112_2_18_2_3_58_88,"[MMIMP3]: AddFileInfoToListFromFMMArray, FILE_LIMIT = %d, alloc = %d")
TRACE_MSG(MMIMP3_4996_112_2_18_2_3_58_89,"[MMIMP3]: AddFileInfoToListFromFMMArray,MMIAPIFMM_WriteFile Fails")
TRACE_MSG(MMIMP3_5058_112_2_18_2_3_58_90,"[MMIMP3]: AddFileInfoToListFromFMMArray, FILE_LIMIT = %d, alloc = %d")
TRACE_MSG(MMIMP3_5064_112_2_18_2_3_58_91,"[MMIMP3]: AddFileInfoToListFromFMMArray,MMIAPIFMM_WriteFile Fails")
TRACE_MSG(MMIMP3_5082_112_2_18_2_3_58_92,"[MMIMP3]: AddFileInfoToListFromFMMArray, writing head info, total_num_alloc = %d,total_num = %d")
TRACE_MSG(MMIMP3_5091_112_2_18_2_3_58_93,"[MMIMP3]: AddFileInfoToListFromFMMArray, writing index info")
TRACE_MSG(MMIMP3_5170_112_2_18_2_3_58_94,"[MMIMP3]: MMIMP3_CreateEmptyListFile, fail at entry")
TRACE_MSG(MMIMP3_5227_112_2_18_2_3_58_95,"[MMIMP3]: MMIMP3_CreateEmptyListFile, sfs_error = %d, result = %d!")
TRACE_MSG(MMIMP3_5251_112_2_18_2_3_58_96,"[MMIMP3] GetFileNamePos fails OUT = 0x%x, IN_Name = 0x%x, In_len = %d")
TRACE_MSG(MMIMP3_5256_112_2_18_2_3_58_97,"[MMIMP3] GetFileNamePos fails, path head error!!!!!!!!!!!!!!!")
TRACE_MSG(MMIMP3_5272_112_2_18_2_3_58_98,"[MMIMP3] GetFileNamePos, result = %d, OUT = %d, IN_Name = 0x%x, In_len = %d")
TRACE_MSG(MMIMP3_5352_112_2_18_2_3_58_99,"[MMIMP3]: MMIMP3_AddFileInfoFromListToList,param error")
TRACE_MSG(MMIMP3_5534_112_2_18_2_3_59_100,"[MMIMP3]: MMIMP3_AddFileInfoFromListToList,MMIAPIFMM_WriteFile Fails")
TRACE_MSG(MMIMP3_5551_112_2_18_2_3_59_101,"[MMIMP3]: MMIMP3_AddFileInfoFromListToList, FILE_LIMIT = %d, alloc = %d")
TRACE_MSG(MMIMP3_5557_112_2_18_2_3_59_102,"[MMIMP3]: MMIMP3_AddFileInfoFromListToList,MMIAPIFMM_WriteFile Fails")
TRACE_MSG(MMIMP3_5565_112_2_18_2_3_59_103,"[MMIMP3]: MMIMP3_AddFileInfoFromListToList,ReadListFileItemInfo Fails, file_index = %d")
TRACE_MSG(MMIMP3_5597_112_2_18_2_3_59_104,"[MMIMP3]: MMIMP3_AddFileInfoFromListToList,ReadListFileItemInfo Fails, file_index = %d")
TRACE_MSG(MMIMP3_5618_112_2_18_2_3_59_105,"[MMIMP3]: MMIMP3_AddFileInfoFromListToList, FILE_LIMIT = %d, alloc = %d")
TRACE_MSG(MMIMP3_5624_112_2_18_2_3_59_106,"[MMIMP3]: MMIMP3_AddFileInfoFromListToList,MMIAPIFMM_WriteFile Fails")
TRACE_MSG(MMIMP3_5701_112_2_18_2_3_59_107,"[MMIMP3]: MMIMP3_AddFileInfoFromListToList, file_num = %d,added_num = %d, result = %d")
TRACE_MSG(MMIMP3_5741_112_2_18_2_3_59_108,"[MMIMP3]: MMIMP3_AddFileInfoToList,param error")
TRACE_MSG(MMIMP3_5878_112_2_18_2_3_59_109,"[MMIMP3]: MMIMP3_AddFileInfoToList,MMIAPIFMM_WriteFile Fails")
TRACE_MSG(MMIMP3_5895_112_2_18_2_3_59_110,"[MMIMP3]: MMIMP3_AddFileInfoToList, FILE_LIMIT = %d, alloc = %d")
TRACE_MSG(MMIMP3_5901_112_2_18_2_3_59_111,"[MMIMP3]: MMIMP3_AddFileInfoToList,MMIAPIFMM_WriteFile Fails")
TRACE_MSG(MMIMP3_5933_112_2_18_2_4_0_112,"[MMIMP3]: MMIMP3_AddFileInfoToList, FILE_LIMIT = %d, alloc = %d")
TRACE_MSG(MMIMP3_5939_112_2_18_2_4_0_113,"[MMIMP3]: MMIMP3_AddFileInfoToList,MMIAPIFMM_WriteFile Fails")
TRACE_MSG(MMIMP3_5999_112_2_18_2_4_0_114,"[MMIMP3]: MMIMP3_AddFileInfoToList,list_len = %d, file_num = %d,added_num = %d, result = %d")
TRACE_MSG(MMIMP3_6021_112_2_18_2_4_0_115,"[MMIMP3]: MMIMP3_InitCurPlayInfoByMp3List, file head error")
TRACE_MSG(MMIMP3_6037_112_2_18_2_4_0_116,"[MMIMP3]: MMIMP3_InitCurPlayInfoByMp3List,total_num = %d, result = %d")
TRACE_MSG(MMIMP3_6164_112_2_18_2_4_0_117,"[MMIMP3]: MMIMP3_ResetCurPlayInfo exit")
TRACE_MSG(MMIMP3_6227_112_2_18_2_4_0_118,"[MMIMP3]: MMIMP3_LoadCurPlayInfoByIndex, entered! file_index = %d,total_num = %d,list_type = %d")
TRACE_MSG(MMIMP3_6369_112_2_18_2_4_0_119,"[MMIMP3]: HandleMp3LoadAllListWaitingWin, msg=%x")
TRACE_MSG(MMIMP3_6398_112_2_18_2_4_0_120,"[MMIMP3]: HandleMp3LoadAllListWaitingWin, MMIAPIFILEARRAY_Combine FAIL")
TRACE_MSG(MMIMP3_6494_112_2_18_2_4_1_121,"[MMIMP3]MMIMP3_AddNewListToAllLists MMIAPIFILEARRAY_Add, result = %d")
TRACE_MSG(MMIMP3_6512_112_2_18_2_4_1_122,"[MMIMP3]MMIMP3_DeleteListFromAllLists MMIAPIFILEARRAY_Delete, list_index = %d, result = %d")
TRACE_MSG(MMIMP3_6525_112_2_18_2_4_1_123,"[MMIMP3]MMIMP3_GetTotalListNum MMIAPIFILEARRAY_GetArraySize, total_num = %d")
TRACE_MSG(MMIMP3_6542_112_2_18_2_4_1_124,"[MMIMP3]MMIMP3_GetArrayListInfo MMIAPIFILEARRAY_Read, list_index = %d, result = %d")
TRACE_MSG(MMIMP3_6636_112_2_18_2_4_1_125,"[MMIMP3]MMIMP3_GetListInfo list_index = %d, result = %d")
TRACE_MSG(MMIMP3_6756_112_2_18_2_4_1_126,"[MMIMP3]MMIMP3_GetListIndexByName list_index = %d, result = %d")
TRACE_MSG(MMIMP3_6837_112_2_18_2_4_1_127,"[MMIMP3]MMIMP3_CheckCurPlayListIndex  list_index = %d, result = %d")
TRACE_MSG(MMIMP3_6953_112_2_18_2_4_2_128,"[MMIMP3]: MMIMP3_ResetMyMusic, MMIAPIFMM_DeleteFile, failed")
TRACE_MSG(MMIMP3_6978_112_2_18_2_4_2_129,"[MMIMP3]: MMIMP3_LoadMyMusic, entered")
TRACE_MSG(MMIMP3_7011_112_2_18_2_4_2_130,"[MMIMP3]: HandleMp3LoadMyMusicWaitingWin, msg=%x")
TRACE_MSG(MMIMP3_7038_112_2_18_2_4_2_131,"[MMIMP3]: HandleMp3LoadMyMusicWaitingWin, MMIAPIFILEARRAY_Combine FAIL")
TRACE_MSG(MMIMP3_7172_112_2_18_2_4_2_132,"[MMIMP3]: MMIMP3_InitMyMusicByFMMArray, total_num = %d, result =%d")
TRACE_MSG(MMIMP3_7344_112_2_18_2_4_2_133,"[MMIMP3]MMIMP3_InitCurViewInfoByMyMusic:total_num = %d, list_type = %d")
TRACE_MSG(MMIMP3_7398_112_2_18_2_4_2_134,"[MMIMP3]MMIMP3_InitCurViewInfoByReserveList: total_num = %d, list_type = %d")
TRACE_MSG(MMIMP3_7908_112_2_18_2_4_3_135,"[MMIMP3]: MMIMP3_LoadAllMp3Files, entered")
TRACE_MSG(MMIMP3_7936_112_2_18_2_4_4_136,"[MMIMP3]: HandleMp3LoadAllMusicWaitingWin, msg=%x")
TRACE_MSG(MMIMP3_7961_112_2_18_2_4_4_137,"[MMIMP3]: HandleMp3LoadAllMusicWaitingWin, MMIAPIFILEARRAY_GetArraySize = %d")
TRACE_MSG(MMIMP3_7990_112_2_18_2_4_4_138,"[MMIMP3]: HandleMp3LoadAllMusicWaitingWin, to_add_num = %d, added_num = %d!")
TRACE_MSG(MMIMP3_8006_112_2_18_2_4_4_139,"[MMIMP3]: HandleMp3LoadAllMusicWaitingWin, AddFileInfoToListFromFMMArray, Fail!")
TRACE_MSG(MMIMP3_8114_112_2_18_2_4_4_140,"[MMIMP3] MMIMP3_GetCurPlayListNameInfo, PNULL == full_path_name_ptr || NULL == full_path_len_ptr")
TRACE_MSG(MMIMP3_8131_112_2_18_2_4_4_141,"[MMIMP3] MMIMP3_GetCurPlayListNameInfo, init_list_type = %d, list_name_len = %d, result = %d")
TRACE_MSG(MMIMP3_8508_112_2_18_2_4_5_142,"[MMIMP3]: MMIMP3_GetCurViewFileInfo, cur_list_type = %d, file_index = %d")
TRACE_MSG(MMIMP3_8570_112_2_18_2_4_5_143,"[MMIMP3] MMIMP3_GetCurViewFileInfo cur_list_type = %d")
TRACE_MSG(MMIMP3_8578_112_2_18_2_4_5_144,"[MMIMP3]: MMIMP3_GetCurViewFileInfo, full_path_len = %d")
TRACE_MSG(MMIMP3_8625_112_2_18_2_4_5_145,"[MMIMP3]: LoadListSectionFromMp3List, open file, list_name_len = %d,file_index = %d")
TRACE_MSG(MMIMP3_8633_112_2_18_2_4_5_146,"[MMIMP3]: LoadListSectionFromMp3List, file_handle = %d,total_num_alloc = %d,total_num = %d")
TRACE_MSG(MMIMP3_8662_112_2_18_2_4_5_147,"[MMIMP3]: LoadListSectionFromMp3List, reading body info")
TRACE_MSG(MMIMP3_8725_112_2_18_2_4_6_148,"[MMIMP3]: LoadFilesInfoFromMp3List, open file, list_name_len = %d")
TRACE_MSG(MMIMP3_8733_112_2_18_2_4_6_149,"[MMIMP3]: LoadFilesInfoFromMp3List, file_handle = %d,total_num_alloc = %d,total_num = %d")
TRACE_MSG(MMIMP3_8762_112_2_18_2_4_6_150,"[MMIMP3]: LoadFilesInfoFromMp3List, reading body info")
TRACE_MSG(MMIMP3_8885_112_2_18_2_4_6_151,"[MMIMP3]MMIMP3_InitCurViewInfoByListIndex:total_num = %d, list_type = %d")
TRACE_MSG(MMIMP3_8950_112_2_18_2_4_6_152,"[MMIMP3]MMIMP3_InitCurViewInfoByListIndex:total_num = %d, list_type = %d")
TRACE_MSG(MMIMP3_9030_112_2_18_2_4_7_153,"[MMIMP3] MMIMP3_UpdateCurViewInfo cur_list_type = %d")
TRACE_MSG(MMIMP3_9096_112_2_18_2_4_7_154,"[MMIMP3]: MMIMP3_DelFileInfoFromArray, del real file success, index = %d")
TRACE_MSG(MMIMP3_9100_112_2_18_2_4_7_155,"[MMIMP3]: MMIMP3_DelFileInfoFromArray, del real file fails, index = %d, sfs_del_rlt = %d")
TRACE_MSG(MMIMP3_9138_112_2_18_2_4_7_156,"[MMIMP3]:MMIMP3_DelFileInfo result = %d")
TRACE_MSG(MMIMP3_9164_112_2_18_2_4_7_157,"[MMIMP3]: MMIMP3_DelFileInfoFromList, open file, list_name_len = %d, file_index = %d, index_num = %d, is_del_real_file = %d")
TRACE_MSG(MMIMP3_9181_112_2_18_2_4_7_158,"[MMIMP3]: MMIMP3_DelFileInfoFromList, file_handle = %d,total_num_alloc = %d,total_num = %d")
TRACE_MSG(MMIMP3_9206_112_2_18_2_4_7_159,"[MMIMP3]: MMIMP3_DelFileInfoFromList, rearrange index info")
TRACE_MSG(MMIMP3_9220_112_2_18_2_4_7_160,"[MMIMP3]: MMIMP3_DelFileInfoFromList, del real file success, index = %d")
TRACE_MSG(MMIMP3_9224_112_2_18_2_4_7_161,"[MMIMP3]: MMIMP3_DelFileInfoFromList, del real file fails, index = %d, sfs_del_rlt = %d")
TRACE_MSG(MMIMP3_9256_112_2_18_2_4_7_162,"[MMIMP3]: MMIMP3_DelFileInfoFromList, writing head info, total_num_alloc = %d,total_num = %d")
TRACE_MSG(MMIMP3_9299_112_2_18_2_4_7_163,"[MMIMP3]: MMIMP3_DelFileInfoFromList, ClearWasteSpaceFromList fails")
TRACE_MSG(MMIMP3_9327_112_2_18_2_4_7_164,"[MMIMP3]: ClearWasteSpaceFromList, entered")
TRACE_MSG(MMIMP3_9385_112_2_18_2_4_8_165,"[MMIMP3]: ClearWasteSpaceFromList, temp file created, and all info added!")
TRACE_MSG(MMIMP3_9404_112_2_18_2_4_8_166,"[MMIMP3]: ClearWasteSpaceFromList, no entry, no space or no list!")
TRACE_MSG(MMIMP3_9413_112_2_18_2_4_8_167,"[MMIMP3]: ClearWasteSpaceFromList, file rename failed")
TRACE_MSG(MMIMP3_9419_112_2_18_2_4_8_168,"[MMIMP3]: ClearWasteSpaceFromList, del original file failed!")
TRACE_MSG(MMIMP3_9451_112_2_18_2_4_8_169,"[MMIMP3]: GetTemptListName, entered!, dev_type= %d")
TRACE_MSG(MMIMP3_9455_112_2_18_2_4_8_170,"PNULL == file_name_ptr || NULL == file_name_len_ptr")
TRACE_MSG(MMIMP3_9495_112_2_18_2_4_8_171,"[MMIMP3]: GetTemptListName ok!")
TRACE_MSG(MMIMP3_9500_112_2_18_2_4_8_172,"[MMIMP3]: GetTemptListName, name repeated!")
TRACE_MSG(MMIMP3_9610_112_2_18_2_4_8_173,"[MMIMP3]: MMIMP3_InitCurPlayByEmptyList, result = %d!")
TRACE_MSG(MMIMP3_9659_112_2_18_2_4_8_174,"[MMIMP3]: MMIMP3_CheckDeviceOfEnoughSpace: space_size = %d, In_dev = %d, Out_dev = %d, result = %d")
TRACE_MSG(MMIMP3_9737_112_2_18_2_4_9_175,"[MMI MP3] MMIMP3_GetCurPlayFileInfo result = %d")
TRACE_MSG(MMIMP3_10023_112_2_18_2_4_9_176,"[MMIMP3]: MMIMP3_GenNewMarkInfo, total_time = %ld, offset_in_time = %ld,offset_byte = %ld, total_len = %ld")
TRACE_MSG(MMIMP3_10114_112_2_18_2_4_10_177,"[MMIMP3]: MMIMP3_LoadAllMusicMarkInfo, read_offset = %d")
TRACE_MSG(MMIMP3_10123_112_2_18_2_4_10_178,"[MMIMP3]: MMIMP3_LoadAllMusicMarkInfo,MMIAPIFMM_ReadFile Fails")
TRACE_MSG(MMIMP3_10167_112_2_18_2_4_10_179,"[MMIMP3] MMIMP3_LoadMusicDetailMarkInfo s_music_mark_info_ptr = %x,index = %d, total = %d")
TRACE_MSG(MMIMP3_10204_112_2_18_2_4_10_180,"[MMIMP3]: MMIMP3_LoadMusicDetailMarkInfo, read_offset = %d")
TRACE_MSG(MMIMP3_10213_112_2_18_2_4_10_181,"[MMIMP3]: MMIMP3_LoadMusicDetailMarkInfo,MMIAPIFMM_ReadFile Fails")
TRACE_MSG(MMIMP3_10371_112_2_18_2_4_10_182,"[MMIMP3] MMIMP3_IsMarkInfoFileExist, result = %d, *mark_dev_ptr = %d!")
TRACE_MSG(MMIMP3_10395_112_2_18_2_4_10_183,"[MMIMP3] MMIMP3_CreateMusicMarkInfoFile entered!")
TRACE_MSG(MMIMP3_10485_112_2_18_2_4_11_184,"[MMIMP3] MMIMP3_CreateMusicMarkInfoFile result = %d!")
TRACE_MSG(MMIMP3_10569_112_2_18_2_4_11_185,"[MMIMP3] MMIMP3_AddMusicMark, total_mark_num = %d,total_num_alloc = %d, result = %d")
TRACE_MSG(MMIMP3_10582_112_2_18_2_4_11_186,"[MMIMP3] MMIMP3_AddMusicMark, write_offset = %d")
TRACE_MSG(MMIMP3_10598_112_2_18_2_4_11_187,"[MMIMP3] MMIMP3_AddMusicMark, MMIAPIFMM_WriteFile Fails")
TRACE_MSG(MMIMP3_10603_112_2_18_2_4_11_188,"[MMIMP3] MMIMP3_AddMusicMark, MMIAPIFMM_SetFilePointer Fails")
TRACE_MSG(MMIMP3_10609_112_2_18_2_4_11_189,"[MMIMP3] MMIMP3_AddMusicMark, write_offset = %d")
TRACE_MSG(MMIMP3_10627_112_2_18_2_4_11_190,"[MMIMP3] MMIMP3_AddMusicMark, MMIAPIFMM_WriteFile Fails")
TRACE_MSG(MMIMP3_10632_112_2_18_2_4_11_191,"[MMIMP3] MMIMP3_AddMusicMark, MMIAPIFMM_SetFilePointer Fails")
TRACE_MSG(MMIMP3_10637_112_2_18_2_4_11_192,"[MMIMP3] MMIMP3_AddMusicMark, begin to write head, result = %d")
TRACE_MSG(MMIMP3_10665_112_2_18_2_4_11_193,"[MMIMP3] MMIMP3_AddMusicMark, result = %d")
TRACE_MSG(MMIMP3_10690_112_2_18_2_4_11_194,"[MMIMP3]: MMIMP3_DelMusicMark, entered!")
TRACE_MSG(MMIMP3_10722_112_2_18_2_4_11_195,"[MMIMP3]: MMIMP3_DelMusicMark, file_handle = %d,total_num_alloc = %d,total_num = %d")
TRACE_MSG(MMIMP3_10733_112_2_18_2_4_11_196,"[MMIMP3]: MMIMP3_DelMusicMark, rearrange index info")
TRACE_MSG(MMIMP3_10763_112_2_18_2_4_11_197,"[MMIMP3]: MMIMP3_DelMusicMark, writing head info, total_num_alloc = %d,total_num = %d")
TRACE_MSG(MMIMP3_10772_112_2_18_2_4_11_198,"[MMIMP3]: MMIMP3_DelMusicMark, write_offset = %d")
TRACE_MSG(MMIMP3_10856_112_2_18_2_4_12_199,"[MMIMP3] MMIMP3_UpdateMusicMark, total_mark_num = %d,total_num_alloc = %d, result = %d")
TRACE_MSG(MMIMP3_10866_112_2_18_2_4_12_200,"[MMIMP3] MMIMP3_UpdateMusicMark, write_offset = %d")
TRACE_MSG(MMIMP3_10881_112_2_18_2_4_12_201,"[MMIMP3] MMIMP3_UpdateMusicMark, MMIAPIFMM_WriteFile Fails")
TRACE_MSG(MMIMP3_10886_112_2_18_2_4_12_202,"[MMIMP3] MMIMP3_UpdateMusicMark, MMIAPIFMM_SetFilePointer Fails")
TRACE_MSG(MMIMP3_10898_112_2_18_2_4_12_203,"[MMIMP3] MMIMP3_UpdateMusicMark, result = %d")
TRACE_MSG(MMIMP3_10923_112_2_18_2_4_12_204,"[MMIMP3]: MMIMP3_GetFileIndexInList, entered")
TRACE_MSG(MMIMP3_10930_112_2_18_2_4_12_205,"[MMIMP3]: MMIMP3_GetFileIndexInList, file_handle = %d,total_num_alloc = %d,total_num = %d")
TRACE_MSG(MMIMP3_10953_112_2_18_2_4_12_206,"[MMIMP3]: MMIMP3_GetFileIndexInList, reading body info")
TRACE_MSG(MMIMP3_11010_112_2_18_2_4_12_207,"[MMIMP3]: MMIMP3_GetMusicFilter, filter = %s, orig_filter = %s")
TRACE_MSG(MMIMP3_11120_112_2_18_2_4_12_208,"[MMIMP3]: MMIMP3_CheckDir: In_dev = %d, sfs_error = %d, result = %d")
TRACE_MSG(MMIMP3_11178_112_2_18_2_4_12_209,"[MMIMP3]:MMIMP3_ResetReviewInfo entry")
TRACE_MSG(MMIMP3_11209_112_2_18_2_4_12_210,"[MMIMP3]:MMIMP3_SetReviewStartPoint result = %d")
TRACE_MSG(MMIMP3_11270_112_2_18_2_4_12_211,"[MMIMP3]:MMIMP3_SetReviewEndPoint result = %d")
TRACE_MSG(MMIMP3_11303_112_2_18_2_4_13_212,"[MMIMP3]:MMIMP3_StartReview audio state = %d, player_state = %d")
TRACE_MSG(MMIMP3_11328_112_2_18_2_4_13_213,"MMIMP3_StartReview:audio seek result = %d")
TRACE_MSG(MMIMP3_11364_112_2_18_2_4_13_214,"[MMIMP3]:MMIMP3_ExitReview audio state = %d, player_state = %d")
TRACE_MSG(MMIMP3_11405_112_2_18_2_4_13_215,"MMIMP3_ExitReview:audio seek result = %d")
TRACE_MSG(MMIMP3_11414_112_2_18_2_4_13_216,"MMIMP3_ExitReview:audio MMIMP3_PlayMp3 FAILS")
TRACE_MSG(MMIMP3_11440_112_2_18_2_4_13_217,"[MMIMP3]:MMIMP3_HandleMp3ReviewPlay entry")
TRACE_MSG(MMIMP3_11533_112_2_18_2_4_13_218,"[MMIMP3]: MMIMP3_InitCurPlayInfoByFileInfo, file_num = %d, file_index = %d")
TRACE_MSG(MMIMP3_11573_112_2_18_2_4_13_219,"[MMIMP3] MMIMP3_InitCurPlayInfoByFileInfo MMIAPIFILEARRAY_Create fails")
TRACE_MSG(MMIMP3_11691_112_2_18_2_4_13_220,"[MMIMP3]: MMIMP3_InitCurPlayInfoByFileInfo, result = %d, music_index = %d")
TRACE_MSG(MMIMP3_11719_112_2_18_2_4_14_221,"[MMIMP3] MMIMP3_InitCurPlayInfoByFile MMIAPIFILEARRAY_Create fails")
TRACE_MSG(MMIMP3_11868_112_2_18_2_4_14_222,"[MMIMP3]: FATAL ERROR")
TRACE_MSG(MMIMP3_11901_112_2_18_2_4_14_223,"[MMIMP3]: MMIMP3_IsSupportA2DPForCurPlay, result = %d")
TRACE_MSG(MMIMP3_11971_112_2_18_2_4_14_224,"[MMIMP3]: MMIAPIMP3_IsFileSupportA2DP, audio_type = %d, result = %d")
TRACE_MSG(MMIMP3_12398_112_2_18_2_4_15_225,"[MMIMP3]: NotifyInitPlayerFinish")
TRACE_MSG(MMIMP3_12417_112_2_18_2_4_15_226,"[MMIMP3]: NotifyInitPlayerNextStep")
TRACE_MSG(MMIMP3_12562_112_2_18_2_4_15_227,"[MMIMP3]: MMIMP3_StopInitPlayer, step =%x")
TRACE_MSG(MMIMP3_12646_112_2_18_2_4_15_228,"[MMIMP3]: MMIMP3_GetCurPlayListFileInfo, file_index = %d,total_num = %d,list_type = %d")
TRACE_MSG(MMIMP3_12697_112_2_18_2_4_15_229,"[MMIMP3] MMIAPIMP3_GetPreOrNextMp3Index Total_num=%d active_index=%d")
TRACE_MSG(MMIMP3_12701_112_2_18_2_4_15_230,"[MMIMP3] MMIAPIMP3_GetPreOrNextMp3Index Total_num = %d")
TRACE_MSG(MMIMP3_BT_70_112_2_18_2_4_17_231,"MMIMP3 MMIMP3_BTEventPlayOrPause, player_state = %d, audio_state = %d ")
TRACE_MSG(MMIMP3_BT_74_112_2_18_2_4_17_232,"MMIMP3 MMIMP3_BTEventPlayOrPause, Not through bt!!!! ")
TRACE_MSG(MMIMP3_BT_119_112_2_18_2_4_17_233,"MMIMP3  MMIMP3_BTEventPre")
TRACE_MSG(MMIMP3_BT_123_112_2_18_2_4_17_234,"MMIMP3  MMIMP3_BTEventPre, Not through bt!!!! ")
TRACE_MSG(MMIMP3_BT_142_112_2_18_2_4_17_235,"MMIMP3  MMIMP3_BTEventNext")
TRACE_MSG(MMIMP3_BT_146_112_2_18_2_4_17_236,"MMIMP3  MMIMP3_BTEventNext, Not through bt!!!! ")
TRACE_MSG(MMIMP3_LYRIC_451_112_2_18_2_4_18_237,"[MMIMP3]:FreeLyricBuffer")
TRACE_MSG(MMIMP3_LYRIC_513_112_2_18_2_4_18_238,"[MMIMP3]:lyric AllocTagInfoBuffer")
TRACE_MSG(MMIMP3_LYRIC_668_112_2_18_2_4_19_239,"[MMIMP3] Alloc lyric section_id = %d ")
TRACE_MSG(MMIMP3_LYRIC_746_112_2_18_2_4_19_240,"FindNextTag,tag_value_ptr1 = 0x%x,*tag_len_ptr1 = %d ,tag_value_ptr2 = 0x%x,*tag_len_ptr2 = %d")
TRACE_MSG(MMIMP3_LYRIC_1029_112_2_18_2_4_19_241,"FindDigitalPart, input error, tag_value_ptr = %x, tag_len = %d,integer_value_ptr = 0x%x,*integer_len_ptr = %d ,decimal_value_ptr = 0x%x,*decimal_len_ptr = %d")
TRACE_MSG(MMIMP3_LYRIC_1444_112_2_18_2_4_21_242,"[MMIMP3]:GetOtherCodeLenFromUsc,wstr_len = %d,ERROR!!!!!!!")
TRACE_MSG(MMIMP3_LYRIC_1589_112_2_18_2_4_21_243,"[MMIMP3]: GetLyricTextFileType, type = %d")
TRACE_MSG(MMIMP3_LYRIC_1642_112_2_18_2_4_21_244,"[MMIMP3]: GetLyricTextFileType, type = %d, result = %d")
TRACE_MSG(MMIMP3_LYRIC_1836_112_2_18_2_4_21_245,"[MMIMP3]:ReadLyricSentence, exit, sentence_index = %d, next_pos =%d, num = %d, sentence_id = %d, %d, %d, %d, %d ,%d, %d,length = %d, array_index = %d")
TRACE_MSG(MMIMP3_LYRIC_1969_112_2_18_2_4_22_246,"[MMIMP3]: OpenLyricFile: No music file!")
TRACE_MSG(MMIMP3_LYRIC_2055_112_2_18_2_4_22_247,"[MMIMP3]: lyric AllocParseBuffer")
TRACE_MSG(MMIMP3_LYRIC_2110_112_2_18_2_4_22_248,"PNULL == lyric_name_ptr")
TRACE_MSG(MMIMP3_LYRIC_2147_112_2_18_2_4_22_249,"PNULL == tag_ptr1 || PNULL == tag_ptr2")
TRACE_MSG(MMIMP3_LYRIC_2645_112_2_18_2_4_23_250,"[MMIMP3]:MMIMP3 DisplayLyricImmediately  cur_time_tag_index = %d!")
TRACE_MSG(MMIMP3_LYRIC_3093_112_2_18_2_4_24_251,"[MMIMP3]:StartLyricTimer: timer_id = %d,time_differ = %d!")
TRACE_MSG(MMIMP3_LYRIC_3139_112_2_18_2_4_24_252,"MMIMP3_PauseLyric: pause timer fail!")
TRACE_MSG(MMIMP3_LYRIC_3158_112_2_18_2_4_24_253,"MMK_ResumeTimer: resume timer fail!")
TRACE_MSG(MMIMP3_LYRIC_3205_112_2_18_2_4_24_254,"MMIMP3_SeekLyric:mp3_offset = %d,time_total_offset = %d,time_tag_num = %d")
TRACE_MSG(MMIMP3_LYRIC_3210_112_2_18_2_4_24_255,"MMIMP3_SeekLyric:i = %d,time = %d,lyric_time_offset = %d")
TRACE_MSG(MMIMP3_LYRIC_3229_112_2_18_2_4_24_256,"MMIMP3_SeekLyric:lyric_cur_time_tag_index = %d,time_differ = %d")
TRACE_MSG(MMIMP3_MUSIC_1643_112_2_18_2_4_29_257,"PNULL == music_file_info_ptr")
TRACE_MSG(MMIMP3_MUSIC_1722_112_2_18_2_4_29_258,"[MMMIMP3 MUSIC] MMIAPIMP3_PlayMyDocumentMusic:memory = %d,name_len = %d")
TRACE_MSG(MMIMP3_MUSIC_1747_112_2_18_2_4_29_259,"[MMIMP3_MUSIC]: ConsumeDrmRight")
TRACE_MSG(MMIMP3_MUSIC_1759_112_2_18_2_4_29_260,"MMIMP3 MusicBTEventPlayOrPause")
TRACE_MSG(MMIMP3_MUSIC_1773_112_2_18_2_4_29_261,"MMIMP3  MusicBTEventPre")
TRACE_MSG(MMIMP3_MUSIC_1783_112_2_18_2_4_29_262,"MMIMP3  MusicBTEventNext")
TRACE_MSG(MMIMP3_MUSIC_1827_112_2_18_2_4_29_263,"[MMMIMP3 MUSIC]  IsMusicA2dpSupport result = %d")
TRACE_MSG(MMIMP3_MUSIC_1842_112_2_18_2_4_29_264,"[MMMIMP3 MUSIC] PauseMusic p_state = %d, a_state = %d, s_music_pause_type = %d!!!")
TRACE_MSG(MMIMP3_MUSIC_1865_112_2_18_2_4_29_265,"[MMIMP3]:PauseMusic:AUDIO FAILED!")
TRACE_MSG(MMIMP3_MUSIC_1910_112_2_18_2_4_29_266,"[MMIMP3 MUSIC] ResumeMusic player_state = %d, audio_state = %d")
TRACE_MSG(MMIMP3_MUSIC_1944_112_2_18_2_4_29_267,"[MMIMP3 MUSIC]:ResumeMusic:AUDIO FAILED!")
TRACE_MSG(MMIMP3_MUSIC_1975_112_2_18_2_4_29_268,"[MMIMP3 MUSIC] StopMusic p_state = %d, a_state = %d, s_music_pause_type = %d!")
TRACE_MSG(MMIMP3_MUSIC_2034_112_2_18_2_4_30_269,"[MMIMP3] MMIMP3_IsMusicPausedByUser result = %d!!!")
TRACE_MSG(MMIMP3_MUSIC_2064_112_2_18_2_4_30_270,"[MMIMP3]:HandleMusicPlayWinMsg:msg_id = 0x%x")
TRACE_MSG(MMIMP3_MUSIC_2351_112_2_18_2_4_30_271,"PlayMusicFile param error!")
TRACE_MSG(MMIMP3_MUSIC_2412_112_2_18_2_4_30_272,"PlayMusicFile, call_back = 0x%x,music_type = %d, result = %d")
TRACE_MSG(MMIMP3_MUSIC_2438_112_2_18_2_4_30_273,"[MMIMP3 MUSIC] PlayMusic entered")
TRACE_MSG(MMIMP3_MUSIC_2488_112_2_18_2_4_30_274,"[MMIMP3]PlayMusic, is_update = %d, is_a2dp = %d, play_offset = %d, cur_time = %d, cur_offset = %d, t_time = %d, t_len = %d")
TRACE_MSG(MMIMP3_MUSIC_2694_112_2_18_2_4_31_275,"[MMIMP3 MUSIC] HandleMusicPlayWinWebKey:focus_button %d is error!")
TRACE_MSG(MMIMP3_MUSIC_2962_112_2_18_2_4_31_276,"[MMIMP3 MUSIC] HandleMusicProgressBarTpKey:tp_x_offet = %d,tp_x = %d,progress_bar_rect.left = %d, mp3_offset = %d")
TRACE_MSG(MMIMP3_MUSIC_2988_112_2_18_2_4_31_277,"[MMIMP3 MUSIC]MusicSeek: offset = %d")
TRACE_MSG(MMIMP3_MUSIC_3006_112_2_18_2_4_31_278,"[MMIMP3]:GetMusicSeekStep, param error!")
TRACE_MSG(MMIMP3_MUSIC_3026_112_2_18_2_4_32_279,"[MMIMP3 MUSIC]: GetMusicSeekStep:file_total_time = %d,file_total_len = %d,bitrate = %d, step_ptr = %ld")
TRACE_MSG(MMIMP3_MUSIC_3067_112_2_18_2_4_32_280,"[MMIMP3]: MusicBackwardSeek:result = %d")
TRACE_MSG(MMIMP3_MUSIC_3108_112_2_18_2_4_32_281,"[MMIMP3]: MusicForwardSeek:result = %d")
TRACE_MSG(MMIMP3_MUSIC_3119_112_2_18_2_4_32_282,"[MMIMP3]: StartMusicSeekTimer")
TRACE_MSG(MMIMP3_MUSIC_3133_112_2_18_2_4_32_283,"[MMIMP3]: StopMusicSeekTimer")
TRACE_MSG(MMIMP3_MUSIC_3183_112_2_18_2_4_32_284,"[MMIMP3]: music  HandleMusicTpDragTimer enter")
TRACE_MSG(MMIMP3_MUSIC_3205_112_2_18_2_4_32_285,"[MMIMP3]: music StartTpDragTimer")
TRACE_MSG(MMIMP3_MUSIC_3213_112_2_18_2_4_32_286,"[MMIMP3]: music StartTpDragTimer: the timer has started!")
TRACE_MSG(MMIMP3_MUSIC_3230_112_2_18_2_4_32_287,"[MMIMP3]: music StopTpDragTimer: the timer has stop!")
TRACE_MSG(MMIMP3_MUSIC_3957_112_2_18_2_4_34_288,"[MMIMP3]:OpenMusicOsdPanel is already opened!")
TRACE_MSG(MMIMP3_MUSIC_4054_112_2_18_2_4_34_289,"[MMIMP3]:MMIMP3_IsOpenMusicOsdPanel osd panel not open")
TRACE_MSG(MMIMP3_MUSIC_4149_112_2_18_2_4_34_290,"[MMIMP3] SetAnimParam addr:0x%x  size%d")
TRACE_MSG(MMIMP3_MUSIC_4424_112_2_18_2_4_35_291,"[MMIMP3]: HandleOsdPanelTimer enter")
TRACE_MSG(MMIMP3_SORT_311_112_2_18_2_4_36_292,"[MMIMP3]: MMIMP3_SortMp3List, entered")
TRACE_MSG(MMIMP3_SORT_381_112_2_18_2_4_36_293,"[MMIMP3]: HandleMp3SortMp3ListWaitingWin, msg=%x")
TRACE_MSG(MMIMP3_SORT_512_112_2_18_2_4_36_294,"[MMIMP3]: SortMp3ListByType, open file, list_name_len = %d")
TRACE_MSG(MMIMP3_SORT_527_112_2_18_2_4_36_295,"[MMIMP3]: SortMp3ListByType, file_handle = %d,total_num_alloc = %d,total_num = %d")
TRACE_MSG(MMIMP3_SORT_606_112_2_18_2_4_36_296,"[MMIMP3]: SortMp3ListByType, writing index info")
TRACE_MSG(MMIMP3_SORT_631_112_2_18_2_4_36_297,"MMIMP3_EndSort is_success = %d, file_handle = %x, list_index_ptr = %x")
TRACE_MSG(MMIMP3_WINTAB_2595_112_2_18_2_4_42_298,"[MMIMP3]:HandlePlayerMenuWinMsg, mmenu_id = %d error!")
TRACE_MSG(MMIMP3_WINTAB_2639_112_2_18_2_4_42_299,"OpenMp3MainWin")
TRACE_MSG(MMIMP3_WINTAB_2953_112_2_18_2_4_43_300,"[MMIMP3]:NextBtnCallback ")
TRACE_MSG(MMIMP3_WINTAB_3590_112_2_18_2_4_44_301,"[MMIMP3] HandleMp3OpenWaitingWin: msg = 0x%x")
TRACE_MSG(MMIMP3_WINTAB_3887_112_2_18_2_4_45_302,"[MMIMP3] HandlePenLongOk: MSG_CTL_PENLONGOK")
TRACE_MSG(MMIMP3_WINTAB_4336_112_2_18_2_4_45_303,"[MMIMP3] HandleReviewMenu seek not supportted")
TRACE_MSG(MMIMP3_WINTAB_4492_112_2_18_2_4_46_304,"[MMIMP3] SetMp3AlertInfo type = %d, callback = 0x%x, win_id = %x")
TRACE_MSG(MMIMP3_WINTAB_4543_112_2_18_2_4_46_305,"[MMIMP3]: StartMp3AlertTimer enter")
TRACE_MSG(MMIMP3_WINTAB_4550_112_2_18_2_4_46_306,"[MMIMP3]: StartMp3AlertTimer: timer started!")
TRACE_MSG(MMIMP3_WINTAB_4562_112_2_18_2_4_46_307,"[MMIMP3]: StopMp3AlertTimer enter")
TRACE_MSG(MMIMP3_WINTAB_4570_112_2_18_2_4_46_308,"[MMIMP3]: StopMp3AlertTimer: timer stopped already!")
TRACE_MSG(MMIMP3_WINTAB_4597_112_2_18_2_4_46_309,"[MMIMP3]: HandleMp3AlertTimer enter")
TRACE_MSG(MMIMP3_WINTAB_5924_112_2_18_2_4_49_310,"MMIMP3_DisplayProgress:uiCurOffset = %d,uiTotalLength = %d,uiCurTime = %d,uiTotalTime = %d,cur_slide = %d,review_start = %d, review_end = %d")
TRACE_MSG(MMIMP3_WINTAB_6173_112_2_18_2_4_49_311,"[MMI MP3] HandleMp3MainWinWebKey")
TRACE_MSG(MMIMP3_WINTAB_6301_112_2_18_2_4_50_312,"[MMIMP3]: HandleMp3TpDragTimer enter")
TRACE_MSG(MMIMP3_WINTAB_6323_112_2_18_2_4_50_313,"[MMIMP3]: StartTpDragTimer")
TRACE_MSG(MMIMP3_WINTAB_6331_112_2_18_2_4_50_314,"[MMIMP3]: StartTpDragTimer: the timer has started!")
TRACE_MSG(MMIMP3_WINTAB_6348_112_2_18_2_4_50_315,"[MMIMP3]: StopTpDragTimer: the timer has stop!")
TRACE_MSG(MMIMP3_WINTAB_6621_112_2_18_2_4_50_316,"left_index < btn_index, left_index = %d, btn_index = %d")
TRACE_MSG(MMIMP3_WINTAB_6636_112_2_18_2_4_50_317,"right_index > btn_index, right_index = %d, btn_index = %d")
TRACE_MSG(MMIMP3_WINTAB_7876_112_2_18_2_4_53_318,"PNULL == item_ptr")
TRACE_MSG(MMIMP3_WINTAB_7974_112_2_18_2_4_53_319,"MMIAPIMP3_StopOperaSDPlug")
TRACE_MSG(MMIMP3_WINTAB_7994_112_2_18_2_4_53_320,"MMIAPIMP3_StopOperaBtAction")
TRACE_MSG(MMIMP3_WINTAB_8019_112_2_18_2_4_53_321,"[MMIMP3]: HandleMp3SeekTimer enter")
TRACE_MSG(MMIMP3_WINTAB_8050_112_2_18_2_4_53_322,"[MMI MP3]: StartForwardOrBackwardTimer")
TRACE_MSG(MMIMP3_WINTAB_8058_112_2_18_2_4_53_323,"StartForwardOrBackwardTimer: the key timer has started!")
TRACE_MSG(MMIMP3_WINTAB_8068_112_2_18_2_4_53_324,"[MMIMP3]: StopForwardOrBackwardTimer s_mp3_forward_backward_timer_id=%d")
TRACE_MSG(MMIMP3_WINTAB_8076_112_2_18_2_4_53_325,"StopForwardOrBackwardTimer: the key timer has stop!")
TRACE_MSG(MMIMP3_WINTAB_8568_112_2_18_2_4_54_326,"[MMIMP3] PDA MMIMP3_GetDisplayStyle error!")
TRACE_MSG(MMIMP3_WINTAB_8999_112_2_18_2_4_56_327,"[MMIMP3] HandleMp3PlayListListWinMsg, cur_list_index = %d,total_num = %d, msg = 0x%x")
TRACE_MSG(MMIMP3_WINTAB_9021_112_2_18_2_4_56_328," PNULL != need_item_data_ptr ")
TRACE_MSG(MMIMP3_WINTAB_9026_112_2_18_2_4_56_329,"[MMIMP3] HandleMp3PlayListList,MSG_CTL_LIST_NEED_ITEM_DATA; index = %d")
TRACE_MSG(MMIMP3_WINTAB_9338_112_2_18_2_4_57_330,"[MMIMP3] HandleMp3PlayListListOptWinMsg, cur_list_index = %d,total_num = %d, msg = 0x%x")
TRACE_MSG(MMIMP3_WINTAB_9613_112_2_18_2_4_58_331,"[MMIMP3]: HandleMp3CopyListWaitingWin, msg=%x")
TRACE_MSG(MMIMP3_WINTAB_9638_112_2_18_2_4_58_332,"[MMIMP3]:HandleMp3CopyListWaitingWin s_mp3_list_dest_dev=%d,error!!!!!!!!!!")
TRACE_MSG(MMIMP3_WINTAB_9720_112_2_18_2_4_58_333,"[MMIMP3]: MMIMP3_UpdatelistDisplay, list mark enabled")
TRACE_MSG(MMIMP3_WINTAB_9769_112_2_18_2_4_58_334,"[MMIMP3] HandleMp3PlayList, msg = 0x%x")
TRACE_MSG(MMIMP3_WINTAB_9884_112_2_18_2_4_58_335," PNULL != need_item_data_ptr ")
TRACE_MSG(MMIMP3_WINTAB_9903_112_2_18_2_4_58_336,"[MMIMP3]:HandleMp3PlayList; MSG_MP3_UPDATE_LIST cur_list_selection = %d")
TRACE_MSG(MMIMP3_WINTAB_9930_112_2_18_2_4_58_337,"[MMIMP3]: HandleMp3PlayListWinMsg, WRONG!!! new_sel = %d, total_num = %d")
TRACE_MSG(MMIMP3_WINTAB_11840_112_2_18_2_5_3_338,"[MMIMP3]: ConstructAddToSrcInfoFromList, list_sel = %d, select_num = %d, result = %d")
TRACE_MSG(MMIMP3_WINTAB_11900_112_2_18_2_5_3_339,"[MMIMP3]: ConstructAddToSrcInfoFromCurPlay, list_sel = %d,result = %d")
TRACE_MSG(MMIMP3_WINTAB_11931_112_2_18_2_5_3_340,"[MMIMP3]: ConstructlistAddToDstInfo, list_index = %d, result = %d")
TRACE_MSG(MMIMP3_WINTAB_11966_112_2_18_2_5_3_341,"[MMIMP3]: HandleMp3ListAddToWaitingWin, msg=%x")
TRACE_MSG(MMIMP3_WINTAB_11985_112_2_18_2_5_3_342,"[MMIMP3]: HandleMp3ListAddToWaitingWin,param error")
TRACE_MSG(MMIMP3_WINTAB_12160_112_2_18_2_5_3_343,"[MMIMP3]: ConstructListDeleteInfo, list_sel = %d, select_num = %d, is_del = %d, result = %d")
TRACE_MSG(MMIMP3_WINTAB_12184_112_2_18_2_5_4_344,"[MMIMP3]: GetCurOprType, opr_type = %d")
TRACE_MSG(MMIMP3_WINTAB_12195_112_2_18_2_5_4_345,"[MMIMP3]: ClearOprInfo, opr_type = %d")
TRACE_MSG(MMIMP3_WINTAB_12273_112_2_18_2_5_4_346,"[MMIMP3]: HandleMp3ListDeleteWaitingWin, msg=%x")
TRACE_MSG(MMIMP3_WINTAB_12313_112_2_18_2_5_4_347,"[MMIMP3]: HandleMp3ListDeleteWaitingWin,param error")
TRACE_MSG(MMIMP3_WINTAB_12517_112_2_18_2_5_4_348," PNULL != need_item_data_ptr ")
TRACE_MSG(MMIMP3_WINTAB_12544_112_2_18_2_5_4_349,"[MMIMP3]: HandleMp3AddToListWinMsg, WRONG!!! cur_sel = %d, total_num = %d")
TRACE_MSG(MMIMP3_WINTAB_12883_112_2_18_2_5_5_350,"*file_dev_ptr = %d is error!")
TRACE_MSG(MMIMP3_WINTAB_13010_112_2_18_2_5_5_351,"[MMIMP3]: HandleMp3NewlistAlertWin, msg=%x")
TRACE_MSG(MMIMP3_WINTAB_13308_112_2_18_2_5_6_352," PNULL != need_item_data_ptr ")
TRACE_MSG(MMIMP3_WINTAB_13341_112_2_18_2_5_6_353,"[MMIMP3]:HandleMp3MusicMarkList, MSG_MP3_UPDATE_MARK_LIST,new_selection = %d")
TRACE_MSG(MMIMP3_WINTAB_13491_112_2_18_2_5_6_354,"[MMIMP3]Begin to play mark, offset = %ld")
TRACE_MSG(MMIMP3_WINTAB_14489_112_2_18_2_5_8_355,"[MMIMP3]: ConstructMarkDeleteInfo, list_sel = %d, select_num = %d, is_del = %d, result = %d")
TRACE_MSG(MMIMP3_WINTAB_14526_112_2_18_2_5_8_356,"[MMIMP3]: HandleMarkDeleteWaitingWin, msg=%x")
TRACE_MSG(MMIMP3_WINTAB_14542_112_2_18_2_5_8_357,"[MMIMP3]: HandleMarkDeleteWaitingWin,param error")
TRACE_MSG(MMIMP3_WINTAB_15322_112_2_18_2_5_10_358,"[MMIMP3] InitPlayListItem, is_path_ok = %d, list_num = %d")
TRACE_MSG(MMIMP3_WINTAB_15831_112_2_18_2_5_11_359,"[MMIMP3] HandleMp3PlayListListLongOkOptWinMsg, cur_list_index = %d,total_num = %d, msg = 0x%x")
TRACE_MSG(MMIMP3_WINTAB_17660_112_2_18_2_5_15_360,"[MMIMP3]:OpenMp3OsdPanel is already opened!")
TRACE_MSG(MMIMP3_WINTAB_17760_112_2_18_2_5_15_361,"[MMIMP3]:OpenMp3OsdPanel osd panel not open")
TRACE_MSG(MMIMP3_WINTAB_18043_112_2_18_2_5_15_362,"[MMIMP3]: GetAlbumInfo check file error! file_index=%d")
TRACE_MSG(MMIMP3_WINTAB_18080_112_2_18_2_5_16_363,"[MMIMP3]: MMIMP3_GetAlbumInfo img_data_ptr not NULL !, addr=0x%x")
TRACE_MSG(MMIMP3_WINTAB_18086_112_2_18_2_5_16_364,"[MMIMP3]: MMIMP3_GetAlbumInfo apic_offset=%d apic_length=%d")
TRACE_MSG(MMIMP3_WINTAB_18107_112_2_18_2_5_16_365,"[MMIMP3]: MMIMP3_GetAlbumInfo GetAlbumImgDataFromFile fail!")
TRACE_MSG(MMIMP3_WINTAB_18116_112_2_18_2_5_16_366,"[MMIMP3]: MMIMP3_GetAlbumInfo img_data_ptr alloc failed!!!")
TRACE_MSG(MMIMP3_WINTAB_18123_112_2_18_2_5_16_367,"[MMIMP3]: MMIMP3_GetAlbumInfo file not exist!")
TRACE_MSG(MMIMP3_WINTAB_18126_112_2_18_2_5_16_368,"[MMIMP3]: MMIMP3_GetAlbumInfo result=%d")
TRACE_MSG(MMIMP3_WINTAB_18158_112_2_18_2_5_16_369,"[MMIMP3]: GetAlbumWcharDescInfo artist str_len=%d")
TRACE_MSG(MMIMP3_WINTAB_18178_112_2_18_2_5_16_370,"[MMIMP3]: GetAlbumWcharDescInfo album str_len=%d")
TRACE_MSG(MMIMP3_WINTAB_18211_112_2_18_2_5_16_371,"[MMIMP3]: GetAlbumImgDataFromFile apic_offset=%d, apic_len=%d buf_len=%d")
TRACE_MSG(MMIMP3_WINTAB_18217_112_2_18_2_5_16_372,"[MMIMP3]: GetAlbumImgDataFromFile create file error!")
TRACE_MSG(MMIMP3_WINTAB_18224_112_2_18_2_5_16_373,"[MMIMP3]: GetAlbumImgDataFromFile set pos error!")
TRACE_MSG(MMIMP3_WINTAB_18231_112_2_18_2_5_16_374,"[MMIMP3]: GetAlbumImgDataFromFile min=%d")
TRACE_MSG(MMIMP3_WINTAB_18236_112_2_18_2_5_16_375,"[MMIMP3]: GetAlbumImgDataFromFile data len =%d")
TRACE_MSG(MMIMP3_WINTAB_18241_112_2_18_2_5_16_376,"[MMIMP3]: GetAlbumImgDataFromFile bytes to read error!")
TRACE_MSG(MMIMP3_WINTAB_18246_112_2_18_2_5_16_377,"[MMIMP3]: GetAlbumImgDataFromFile read file error!")
TRACE_MSG(MMIMP3_WINTAB_18278_112_2_18_2_5_16_378,"[MMIMP3] GetPreMp3AlbumInfo")
TRACE_MSG(MMIMP3_WINTAB_18286_112_2_18_2_5_16_379,"[MMIMP3] GetPreMp3AlbumInfo get pre index error")
TRACE_MSG(MMIMP3_WINTAB_18299_112_2_18_2_5_16_380,"[MMIMP3] GetCurMp3AlbumInfo")
TRACE_MSG(MMIMP3_WINTAB_18307_112_2_18_2_5_16_381,"[MMIMP3] GetCurMp3AlbumInfo total_num == 0")
TRACE_MSG(MMIMP3_WINTAB_18319_112_2_18_2_5_16_382,"[MMIMP3] GetNextMp3AlbumInfo")
TRACE_MSG(MMIMP3_WINTAB_18327_112_2_18_2_5_16_383,"[MMIMP3] GetNextMp3AlbumInfo get next index error")
TRACE_MSG(MMIMP3_WINTAB_18394_112_2_18_2_5_16_384,"[MMIMP3] ReleaseAllAlbumInfo pre-addr:0x%x size%d cur-addr:0x%x size%d next-addr:0x%x size%d")
TRACE_MSG(MMIMP3_WINTAB_18427_112_2_18_2_5_16_385,"[MMIMP3] ReleaseAlbumInfo, is_prev = %d")
TRACE_MSG(MMIMP3_WINTAB_18486_112_2_18_2_5_16_386,"[MMIMP3] SetAnimParam addr:0x%x  size%d")
TRACE_MSG(MMIMP3_WINTAB_18639_112_2_18_2_5_17_387,"[MMIMP3]: HandleAlbumImgGlideTpDown style=%d total_num=%d")
TRACE_MSG(MMIMP3_WINTAB_19156_112_2_18_2_5_18_388,"[MMIMP3]: HandleOsdPanelTimer enter")
TRACE_MSG(MMIMP3_WINTAB_19240_112_2_18_2_5_18_389,"[MMIMP3]: OpenCurPlayList is already opened!")
TRACE_MSG(MMIMP3_WINTAB_19337_112_2_18_2_5_18_390,"[MMIMP3] HandleMp3CurPlayListWinMsg, msg = 0x%x")
TRACE_MSG(MMIMP3_WINTAB_19451_112_2_18_2_5_18_391," PNULL != need_item_data_ptr ")
TRACE_MSG(MMIMP3_WINTAB_19470_112_2_18_2_5_18_392,"[MMIMP3]:HandleMp3CurPlayListWinMsg; MSG_MP3_UPDATE_LIST cur_list_selection = %d")
TRACE_MSG(MMIMP3_WINTAB_19497_112_2_18_2_5_18_393,"[MMIMP3]: HandleMp3CurPlayListWinMsg, WRONG!!! new_sel = %d, total_num = %d")
TRACE_MSG(MMIMP3_WINTAB_19781_112_2_18_2_5_19_394,"mmi MP3Applet_HandleEvent msg_id = 0x%04x")
END_TRACE_MAP(MMI_APP_AUDIOPLAYER_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_AUDIOPLAYER_TRC_H_

