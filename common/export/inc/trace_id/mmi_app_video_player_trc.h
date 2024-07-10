/******************************************************************************
 ** File Name:      mmi_app_video_player_trc.h                                         *
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
//trace_id:235
#ifndef _MMI_APP_VIDEO_PLAYER_TRC_H_
#define _MMI_APP_VIDEO_PLAYER_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIVP_API_57_112_2_18_3_5_0_0 "[VP] SdpFile_otacalback param_ptr->mime_type = %d"
#define MMIVP_BOOKMARKLIST_116_112_2_18_3_5_1_1 "[VP] SaveBookmarklistToFile write bookmark successfully"
#define MMIVP_BOOKMARKLIST_139_112_2_18_3_5_1_2 "[VP] SaveBookmarklistToFile bookmarklist is null"
#define MMIVP_BOOKMARKLIST_144_112_2_18_3_5_1_3 "[VP] SaveBookmarklistToFile save_result = %d"
#define MMIVP_BOOKMARKLIST_171_112_2_18_3_5_1_4 "[VP] GetBookmarkListFileNameForRead result = %d"
#define MMIVP_BOOKMARKLIST_200_112_2_18_3_5_1_5 "[VP] GetBookmarkListFileNameForWrite result = %d"
#define MMIVP_BOOKMARKLIST_237_112_2_18_3_5_1_6 "[VP] VP_BookmarkList_Load readlist_result = %d"
#define MMIVP_BOOKMARKLIST_247_112_2_18_3_5_1_7 "[VP] VP_BookmarkList_Load s_visit_bookmark_que_ptr = %x"
#define MMIVP_BOOKMARKLIST_314_112_2_18_3_5_1_8 "[VP] VP_BookmarkList_GetNum total_bookmark_num = %d"
#define MMIVP_BOOKMARKLIST_325_112_2_18_3_5_1_9 "[VP] VP_BookmarkList_GetInfo list_index = %d"
#define MMIVP_BOOKMARKLIST_329_112_2_18_3_5_1_10 "[VP] VP_BookmarkList_GetInfo param error"
#define MMIVP_BOOKMARKLIST_345_112_2_18_3_5_1_11 "[VP] VP_BookmarkList_DelItem index = %d"
#define MMIVP_BOOKMARKLIST_364_112_2_18_3_5_1_12 "[VP] VP_BookmarkList_DelItem result = %d, s_visit_bookmark_que_ptr->item_count = %d"
#define MMIVP_BOOKMARKLIST_399_112_2_18_3_5_2_13 "[VP] VP_BookmarkList_AddItem list too much bookmark "
#define MMIVP_BOOKMARKLIST_420_112_2_18_3_5_2_14 "[VP] VP_BookmarkList_AddItem index = %d, count = %d "
#define MMIVP_BOOKMARKLIST_429_112_2_18_3_5_2_15 "[VP] VP_BookmarkList_AddItem add_result = %d"
#define MMIVP_BOOKMARKLIST_444_112_2_18_3_5_2_16 "[VP] VP_BookmarkList_ModifyItem  index is worng list_index = %d "
#define MMIVP_BOOKMARKLIST_455_112_2_18_3_5_2_17 "[VP] VP_BookmarkList_ModifyItem modify_result = %d"
#define MMIVP_FSM_539_112_2_18_3_5_4_18 "IsVideoPlayerCollision ret = %d"
#define MMIVP_FSM_563_112_2_18_3_5_4_19 "[VP] CreatMsg play_param_ptr->play_type = %d, msg_body_size = %d"
#define MMIVP_FSM_570_112_2_18_3_5_4_20 "[VP] CreatMsg full path ptr is null"
#define MMIVP_FSM_579_112_2_18_3_5_4_21 "[VP] CreatMsg buffer is null"
#define MMIVP_FSM_588_112_2_18_3_5_4_22 "[VP] CreatMsg url is null"
#define MMIVP_FSM_591_112_2_18_3_5_4_23 "[VP] CreatMsg msg_body_size = %d"
#define MMIVP_FSM_593_112_2_18_3_5_4_24 "[VP] CreatMsg msg_body_size = %d"
#define MMIVP_FSM_607_112_2_18_3_5_4_25 "[VP] msg_ptr = %x, len = %d"
#define MMIVP_FSM_619_112_2_18_3_5_4_26 "apple play_param2_ptr = %x, msg_body_size =%d"
#define MMIVP_FSM_632_112_2_18_3_5_4_27 "[VP] CreatMsg play_param2_ptr->url_ptr = %x, len = %x"
#define MMIVP_FSM_636_112_2_18_3_5_4_28 "apple play_param2_ptr->full_path_ptr = %x"
#define MMIVP_FSM_638_112_2_18_3_5_4_29 "apple play_param2_ptr->full_path_ptr = %x"
#define MMIVP_FSM_639_112_2_18_3_5_4_30 "apple play_param_ptr->full_path_ptr = %x, play_param_ptr->full_path_len = %d"
#define MMIVP_FSM_641_112_2_18_3_5_4_31 "apple play_param2_ptr->full_path_ptr = %x"
#define MMIVP_FSM_724_112_2_18_3_5_4_32 "[VP] CreatMsg frame height = %d, rate = %d, base = %d,width = %d, cfg_len = %d"
#define MMIVP_FSM_741_112_2_18_3_5_4_33 "[VP] CreatMsg strm_init_info_ptr->sdp_info.video_param.config_len = %d"
#define MMIVP_FSM_761_112_2_18_3_5_4_34 "[VP] CreatMsg sprop_param_sets_num = %d, enc_type = %d, sprop_sets_arr = %x"
#define MMIVP_FSM_969_112_2_18_3_5_4_35 "[VP] CreateFSM there is a fsm"
#define MMIVP_FSM_977_112_2_18_3_5_5_36 "[VP] CreateFSM no space for fsm"
#define MMIVP_FSM_1015_112_2_18_3_5_5_37 "[VP] CreateFSM cur fsm is %x"
#define MMIVP_FSM_1038_112_2_18_3_5_5_38 "[VP] CreateFSM cur fsm is %x"
#define MMIVP_FSM_1066_112_2_18_3_5_5_39 "[VP] DestroyFSM there is no fsm list"
#define MMIVP_FSM_1074_112_2_18_3_5_5_40 "[VP] DestroyFSM current_fsm_ptr = %x, current_fsm_ptr->fsm_info_ptr = %x"
#define MMIVP_FSM_1130_112_2_18_3_5_5_41 "[VP] DestroyFSM  fsm vp_handle = %x"
#define MMIVP_FSM_1179_112_2_18_3_5_5_42 "[VP] FsmDispatch something wrong with fsm_ptr = %x"
#define MMIVP_FSM_1199_112_2_18_3_5_5_43 "[VP] FsmStateTrans fsm_ptr = %x, state = %d"
#define MMIVP_FSM_1227_112_2_18_3_5_5_44 "[VP] SendPrepareIndToUI param is error"
#define MMIVP_FSM_1243_112_2_18_3_5_5_45 "[VP] SendPrepareIndToUI send prepare ind to ui"
#define MMIVP_FSM_1260_112_2_18_3_5_5_46 "[VP] SendAUTHIndToUI param is error"
#define MMIVP_FSM_1269_112_2_18_3_5_5_47 "[VP] SendAUTHIndToUI send auth ind to ui"
#define MMIVP_FSM_1286_112_2_18_3_5_5_48 "[VP] SendFatalErrorIndTOUI param is error"
#define MMIVP_FSM_1296_112_2_18_3_5_5_49 "[VP] SendFatalErrorIndTOUI send fatal error to ui error_code = %d"
#define MMIVP_FSM_1313_112_2_18_3_5_5_50 "[VP] SendProcessIndToUI param is error"
#define MMIVP_FSM_1340_112_2_18_3_5_5_51 "[VP] SendBufferingIndToUI param is error"
#define MMIVP_FSM_1349_112_2_18_3_5_5_52 "[VP] SendBufferingIndToUI send VP_BUFFERING_IND to ui"
#define MMIVP_FSM_1367_112_2_18_3_5_5_53 "[VP] SendBufferEndIndToUI param is error"
#define MMIVP_FSM_1377_112_2_18_3_5_5_54 "[VP] SendAUTHIndToUI send VP_BUFFER_END_IND to ui"
#define MMIVP_FSM_1395_112_2_18_3_5_5_55 "[VP] SendBufferEndIndToUI param is error"
#define MMIVP_FSM_1404_112_2_18_3_5_5_56 "[VP] SendAUTHIndToUI send VP_BUFFER_END_IND to ui"
#define MMIVP_FSM_1421_112_2_18_3_5_5_57 "[VP] SendPlayEndToUI param is error"
#define MMIVP_FSM_1431_112_2_18_3_5_6_58 "[VP] SendAUTHIndToUI send VP_PLAY_END_IND to ui"
#define MMIVP_FSM_1447_112_2_18_3_5_6_59 "[VP] SendStopIndToUI param is error"
#define MMIVP_FSM_1462_112_2_18_3_5_6_60 "[VP] SendAUTHIndToUI send VP_STOP_IND to ui"
#define MMIVP_FSM_1478_112_2_18_3_5_6_61 "[VP] SendPlayCnfToUI param is error"
#define MMIVP_FSM_1488_112_2_18_3_5_6_62 "[VP] SendPlayCnfToUI send VP_PLAY_CNFto ui error_code = %d"
#define MMIVP_FSM_1504_112_2_18_3_5_6_63 "[VP] SendPauseCnfToUI param is error"
#define MMIVP_FSM_1514_112_2_18_3_5_6_64 "[VP] SendAUTHIndToUI send VP_PAUSE_CNF to ui"
#define MMIVP_FSM_1530_112_2_18_3_5_6_65 "[VP] SendSeekCnfToUI param is error"
#define MMIVP_FSM_1542_112_2_18_3_5_6_66 "[VP] SendAUTHIndToUI send VP_SEEK_CNF to ui"
#define MMIVP_FSM_1576_112_2_18_3_5_6_67 "[VP]:GetFrameDataCallback enter!"
#define MMIVP_FSM_1678_112_2_18_3_5_6_68 "[VP]FrameToTime handle = 0x%x, info_result = %d,frame_index = %d, time_offset = 0x%x, time_lenth = %d,base = %d"
#define MMIVP_FSM_1701_112_2_18_3_5_6_69 "[VP] PlayLocalFile fsm is error"
#define MMIVP_FSM_1709_112_2_18_3_5_6_70 "[VP] PlayLocalFile full_path_len = %d, full_path_ptr = %x, %x, %x, %x"
#define MMIVP_FSM_1768_112_2_18_3_5_6_71 "[VP] PlayLocalFile fsm_ptr->dplayer_handle = %x"
#define MMIVP_FSM_1793_112_2_18_3_5_6_72 "[VP] PlayLocalFile play_param_ptr->is_scal_up = %d, fsm_ptr->display_param.Width = %d, p_info.video_info.width = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d"
#define MMIVP_FSM_1803_112_2_18_3_5_6_73 "[VP] PlayLocalFile total_time = %d, process_time = %d, support_type = %d"
#define MMIVP_FSM_1823_112_2_18_3_5_6_74 "[VP] PlayLocalFile is_cr_video_layer"
#define MMIVP_FSM_1837_112_2_18_3_5_6_75 "[VP] PlayLocalFile fsm_ptr->is_save_history = %d"
#define MMIVP_FSM_1865_112_2_18_3_5_6_76 "[VP] PlayLocalFile play error"
#define MMIVP_FSM_1913_112_2_18_3_5_6_77 "[VP] PlayLocalBuf param is error"
#define MMIVP_FSM_1923_112_2_18_3_5_7_78 "[VP] PlayLocalBuf play_param_ptr->video_buf_info.type = %d"
#define MMIVP_FSM_1952_112_2_18_3_5_7_79 "[VP] PlayLocalBuf fsm_ptr->dplayer_handle = %x"
#define MMIVP_FSM_1967_112_2_18_3_5_7_80 "[VP] PlayLocalBuf vidot_type = %d, vidoe_support = %d"
#define MMIVP_FSM_1972_112_2_18_3_5_7_81 "[VP] PlayLocalBuf play_param_ptr->is_scal_up = %d, fsm_ptr->display_param.Width = %d, p_info.video_info.width = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d"
#define MMIVP_FSM_1984_112_2_18_3_5_7_82 "[VP] PlayLocalBuf total_time = %d, process_time = %d, support_type = %d"
#define MMIVP_FSM_2008_112_2_18_3_5_7_83 "[VP] PlayLocalBuf play success"
#define MMIVP_FSM_2016_112_2_18_3_5_7_84 "[VP] PlayLocalBuf play error"
#define MMIVP_FSM_2048_112_2_18_3_5_7_85 "[VP] PlayFromNet param is error"
#define MMIVP_FSM_2061_112_2_18_3_5_7_86 "[VP] PlayFromNet fsm_ptr->dplayer_handle = %x"
#define MMIVP_FSM_2089_112_2_18_3_5_7_87 "[VP] PlayFromNet play_param_ptr->is_scal_up = %d, fsm_ptr->display_param.Width = %d, p_info.video_info.width = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d"
#define MMIVP_FSM_2101_112_2_18_3_5_7_88 "[VP] PlayFromNet total_time = %d, process_time = %d, support_type = %d"
#define MMIVP_FSM_2128_112_2_18_3_5_7_89 "[VP] PlayFromNet play error"
#define MMIVP_FSM_2196_112_2_18_3_5_7_90 "[VP] HandlePDPMsg param is error"
#define MMIVP_FSM_2200_112_2_18_3_5_7_91 "[VP] HandlePDPMsg msg_ptr->msg_id = %x"
#define MMIVP_FSM_2240_112_2_18_3_5_7_92 "[VP] ActivePdp param is error"
#define MMIVP_FSM_2245_112_2_18_3_5_7_93 "[VP] ActivePdp dual_sys = %d"
#define MMIVP_FSM_2253_112_2_18_3_5_7_94 "[VP] ActivePdp sim casd is not ok dual_sys = %d"
#define MMIVP_FSM_2262_112_2_18_3_5_7_95 "[VP] ActivePdp param is setting_index = %d"
#define MMIVP_FSM_2285_112_2_18_3_5_7_96 "[VP] ActivePdp sim casd is not ok dual_sys = %d"
#define MMIVP_FSM_2317_112_2_18_3_5_7_97 "[VP] FsmIdle param is error"
#define MMIVP_FSM_2321_112_2_18_3_5_7_98 "[VP] FsmIdle msg_ptr->msg_id = %x"
#define MMIVP_FSM_2329_112_2_18_3_5_7_99 "IDLE, got wrong msg"
#define MMIVP_FSM_2350_112_2_18_3_5_7_100 "[VP] InitFsmInfo param is null"
#define MMIVP_FSM_2353_112_2_18_3_5_7_101 "[VP] InitFsmInfo state = %d, vpcallback = %x"
#define MMIVP_FSM_2391_112_2_18_3_5_7_102 "[VP] UpdateFsmInfo param is null"
#define MMIVP_FSM_2436_112_2_18_3_5_8_103 "[VP] UpdateFsmInfo fsm_ptr->full_path_ptr = %x, play_param_ptr->full_path_ptr = %x"
#define MMIVP_FSM_2500_112_2_18_3_5_8_104 "[VP] PlayNextFile param is null"
#define MMIVP_FSM_2503_112_2_18_3_5_8_105 "[VP] PlayNextFile i_type = %d, fsm_ptr->mode = %d"
#define MMIVP_FSM_2571_112_2_18_3_5_8_106 "[VP] PlayNextFile result = %d"
#define MMIVP_FSM_2594_112_2_18_3_5_8_107 "[VP] FsmReady play param is null"
#define MMIVP_FSM_2598_112_2_18_3_5_8_108 "[VP] FsmReady msg_ptr->msg_id = %x, fsm_ptr->has_play_end_reason = %d, fsm_ptr->is_to_exit = %d"
#define MMIVP_FSM_2621_112_2_18_3_5_8_109 "[VP] FsmReady play_param_ptr->play_type = %d"
#define MMIVP_FSM_2728_112_2_18_3_5_8_110 "[VP] FsmReady ready, got wrong msg"
#define MMIVP_FSM_2750_112_2_18_3_5_8_111 "[VP] InitStreaming param is error"
#define MMIVP_FSM_2863_112_2_18_3_5_8_112 "[VP] FsmPdpActiving fsm or msg error"
#define MMIVP_FSM_2867_112_2_18_3_5_8_113 "[VP] FsmPdpActiving msg_ptr->msg_id = %x"
#define MMIVP_FSM_2875_112_2_18_3_5_8_114 "[VP] FsmPdpActiving pdp active error"
#define MMIVP_FSM_2931_112_2_18_3_5_9_115 "[VP] FsmInit param is error"
#define MMIVP_FSM_2935_112_2_18_3_5_9_116 "[VP] FsmInit msg_ptr->msg_id = %x"
#define MMIVP_FSM_2956_112_2_18_3_5_9_117 "[VP] PlayFromNet play error"
#define MMIVP_FSM_3013_112_2_18_3_5_9_118 "[VP] FsmInit param is error"
#define MMIVP_FSM_3017_112_2_18_3_5_9_119 "[VP] FsmInit msg_ptr->msg_id = %x"
#define MMIVP_FSM_3022_112_2_18_3_5_9_120 "[VP] FsmInit str_ini_result = %d, is_success_play = %d"
#define MMIVP_FSM_3133_112_2_18_3_5_9_121 "[VP] FsmInitAuth param is error"
#define MMIVP_FSM_3137_112_2_18_3_5_9_122 "[VP] FsmInitAuth msg_ptr->msg_id = %x"
#define MMIVP_FSM_3203_112_2_18_3_5_9_123 "[VP] FsmPlayPeding param is error"
#define MMIVP_FSM_3207_112_2_18_3_5_9_124 "[VP] FsmPlayPeding msg_ptr->msg_id = %x"
#define MMIVP_FSM_3212_112_2_18_3_5_9_125 "[VP] FsmPlayPeding strm_result = %d"
#define MMIVP_FSM_3332_112_2_18_3_5_10_126 "[VP] FsmPlayAuth param is error"
#define MMIVP_FSM_3337_112_2_18_3_5_10_127 "[VP] FsmPlayAuth msg_ptr->msg_id = %x"
#define MMIVP_FSM_3353_112_2_18_3_5_10_128 "[VP] FsmPlayAuth auth_info.password_ptr= %x, auth_info.username_ptr = %x"
#define MMIVP_FSM_3409_112_2_18_3_5_10_129 "[VP] FsmPlaying param is error"
#define MMIVP_FSM_3418_112_2_18_3_5_10_130 "[VP] FsmPlaying VP_ENTRY_REQ fsm_ptr->is_to_pause %x,fsm_ptr->has_play_end_reason = %d"
#define MMIVP_FSM_3443_112_2_18_3_5_10_131 "[VP] FsmPlaying VP_STRM_BUFFERING_IND"
#define MMIVP_FSM_3450_112_2_18_3_5_10_132 "[VP] FsmPlaying VP_CALLBACK_END_IND"
#define MMIVP_FSM_3466_112_2_18_3_5_10_133 "[VP] FsmPlaying fsm_ptr->play_type = %d , fsm_ptr->mode = %d, fsm_ptr->is_random_play = %d"
#define MMIVP_FSM_3582_112_2_18_3_5_10_134 "[VP] FsmPlaying VP_PAUSE_REQ, fsm_ptr->is_not_notify_ui = %d, fsm_ptr->is_to_pause = %d"
#define MMIVP_FSM_3619_112_2_18_3_5_10_135 "[VP] FsmPlaying VP_SEEK_REQ"
#define MMIVP_FSM_3657_112_2_18_3_5_10_136 "[VP] FsmPlaying VP_STOP_REQ"
#define MMIVP_FSM_3673_112_2_18_3_5_10_137 "[VP] FsmPlaying VP_STRM_DATA_TIMEOUT_IND"
#define MMIVP_FSM_3682_112_2_18_3_5_10_138 "[VP] FsmPlaying VP_RESUME_REQ"
#define MMIVP_FSM_3724_112_2_18_3_5_10_139 "[VP] FsmBuffering param is error"
#define MMIVP_FSM_3729_112_2_18_3_5_10_140 "[VP] FsmBuffering msg_ptr->msg_id = %x"
#define MMIVP_FSM_3865_112_2_18_3_5_11_141 "[VP] FsmSeeking param is error"
#define MMIVP_FSM_3869_112_2_18_3_5_11_142 "[VP] FsmSeeking  msg_ptr->msg_id = %x"
#define MMIVP_FSM_3882_112_2_18_3_5_11_143 "[VP] FsmSeeking strm_result = %d"
#define MMIVP_FSM_3965_112_2_18_3_5_11_144 "[VP] FsmPausing param is error"
#define MMIVP_FSM_3969_112_2_18_3_5_11_145 "[VP] FsmPausing msg_ptr->msg_id = %x"
#define MMIVP_FSM_3982_112_2_18_3_5_11_146 "[VP] FsmPausing strm_result = %d, fsm_ptr->is_not_notify_ui = %d"
#define MMIVP_FSM_3991_112_2_18_3_5_11_147 "[VP] FsmPausing send pause cnf"
#define MMIVP_FSM_4057_112_2_18_3_5_11_148 "[VP] FsmPaused param is error"
#define MMIVP_FSM_4061_112_2_18_3_5_11_149 "[VP] FsmPaused msg_ptr->msg_id = %x, is_to_pause = %d, is_to_resume = %d"
#define MMIVP_FSM_4109_112_2_18_3_5_11_150 "[VP] FsmPaused VP_SEEK_REQ"
#define MMIVP_FSM_4218_112_2_18_3_5_11_151 "[VP] FsmPausedSeeking param is error"
#define MMIVP_FSM_4222_112_2_18_3_5_11_152 "[VP] FsmPausedSeeking  msg_ptr->msg_id = %x"
#define MMIVP_FSM_4244_112_2_18_3_5_11_153 "[VP] FsmSeeking strm_result = %d"
#define MMIVP_FSM_4325_112_2_18_3_5_12_154 "[VP] FsmPaused param is error"
#define MMIVP_FSM_4329_112_2_18_3_5_12_155 "[VP] FsmPaused msg_ptr->msg_id = %x"
#define MMIVP_FSM_4402_112_2_18_3_5_12_156 "[VP] FsmResuming msg_ptr->msg_id = %x"
#define MMIVP_FSM_4414_112_2_18_3_5_12_157 "[VP] FsmResuming strm_result = %d"
#define MMIVP_FSM_4490_112_2_18_3_5_12_158 "[VP] HandleInitCnf param is null"
#define MMIVP_FSM_4495_112_2_18_3_5_12_159 "[VP] HandleInitCnf strm_init_ptr->result = %d"
#define MMIVP_FSM_4584_112_2_18_3_5_12_160 "[VP] HandleInitCnf frame height = %d, rate = %d, base = %d,width = %d, cfg_len = %d, fsm_ptr->total_time= %d, strm_cfg.video_cfg.cfg_ptr = %x"
#define MMIVP_FSM_4627_112_2_18_3_5_12_161 "[VP]:HandleGetFrameData no video_layer!!!!!!!!!"
#define MMIVP_FSM_4653_112_2_18_3_5_12_162 "[VP] HandleUpdateFrameReq param is null"
#define MMIVP_FSM_4670_112_2_18_3_5_12_163 "[VP]:GetFrameDataCallback height=%d,width=%d!"
#define MMIVP_FSM_4681_112_2_18_3_5_12_164 "[VP]HandleUpdateFrameReq,ERROR BUFFER1!!"
#define MMIVP_FSM_4700_112_2_18_3_5_12_165 "[VP]:GetFrameDataCallback l_height=%d,l_width=%d!"
#define MMIVP_FSM_4706_112_2_18_3_5_12_166 "[VP]HandleUpdateFrameReq,ERROR BUFFER2!!"
#define MMIVP_FSM_4746_112_2_18_3_5_13_167 "[VP]:HandleUpdateFrameReq ERROR!!!!! height=%d,width=%d,layer_height=%d,layer_width=%d"
#define MMIVP_FSM_4752_112_2_18_3_5_13_168 "[VP]:HandleUpdateFrameReq DPLAYER_UpdateNextFrame"
#define MMIVP_FSM_4767_112_2_18_3_5_13_169 "[VP] HandlePauseReq param is null"
#define MMIVP_FSM_4789_112_2_18_3_5_13_170 "[VP] HandlePauseReq can not pause, so stop"
#define MMIVP_FSM_4812_112_2_18_3_5_13_171 "[VP] HandleResumeReq param is null"
#define MMIVP_FSM_4844_112_2_18_3_5_13_172 "[VP] HandleSeekReq param is null"
#define MMIVP_FSM_4849_112_2_18_3_5_13_173 "[VP] FsmPlaying seek_time = %x, seek_param->seek_frame = %x"
#define MMIVP_FSM_4876_112_2_18_3_5_13_174 "[VP] HandleSeekReq streaming see error"
#define MMIVP_FSM_4890_112_2_18_3_5_13_175 "[VP] HandleStopReq param is null"
#define MMIVP_FSM_4893_112_2_18_3_5_13_176 "[VP] HandleStopReq is_begin_play = %d, fsm_ptr->is_save_history = %d"
#define MMIVP_FSM_4939_112_2_18_3_5_13_177 "[VP] FsmStopping param is error"
#define MMIVP_FSM_4943_112_2_18_3_5_13_178 "[VP] FsmStopping msg_ptr->msg_id = %x"
#define MMIVP_FSM_4975_112_2_18_3_5_13_179 "[VP] FsmStopped param is error"
#define MMIVP_FSM_5015_112_2_18_3_5_13_180 "[VP] FsmClosing param is error"
#define MMIVP_FSM_5019_112_2_18_3_5_13_181 "[VP] FsmClosing msg_ptr->msg_id = %x"
#define MMIVP_FSM_5068_112_2_18_3_5_13_182 "[VP] MMIVP_DispatchSignalToFsmMsg   APP_VP_ONE_FRAME_REARCH"
#define MMIVP_FSM_5072_112_2_18_3_5_13_183 "[VP] MMIVP_DispatchSignalToFsmMsg   APP_VP_ONE_FRAME_END"
#define MMIVP_FSM_5092_112_2_18_3_5_13_184 "[VP] MMIVP_DispatchSignalToFsmMsg STRM_INIT_CNF"
#define MMIVP_FSM_5110_112_2_18_3_5_13_185 "[VP] MMIVP_DispatchSignalToFsmMsg STRM_PLAY_CNF"
#define MMIVP_FSM_5126_112_2_18_3_5_13_186 "[VP] MMIVP_DispatchSignalToFsmMsg STRM_BUFFERING_IND"
#define MMIVP_FSM_5134_112_2_18_3_5_13_187 "[VP] MMIVP_DispatchSignalToFsmMsg STRM_BUFFER_END_IND"
#define MMIVP_FSM_5142_112_2_18_3_5_13_188 "[VP] MMIVP_DispatchSignalToFsmMsg STRM_DATA_TIMEOUT_IND"
#define MMIVP_FSM_5151_112_2_18_3_5_13_189 "[VP] MMIVP_DispatchSignalToFsmMsg strm_buf_process_ptr = %x"
#define MMIVP_FSM_5157_112_2_18_3_5_13_190 "[VP] MMIVP_DispatchSignalToFsmMsg process = %d, total = %d"
#define MMIVP_FSM_5164_112_2_18_3_5_13_191 "[VP] MMIVP_DispatchSignalToFsmMsg percent = %d"
#define MMIVP_FSM_5169_112_2_18_3_5_13_192 "[VP] MMIVP_DispatchSignalToFsmMsg STRM_GOODBYE_IND"
#define MMIVP_FSM_5178_112_2_18_3_5_13_193 "[VP] MMIVP_DispatchSignalToFsmMsg STRM_SEEK_CNF"
#define MMIVP_FSM_5196_112_2_18_3_5_13_194 "[VP] MMIVP_DispatchSignalToFsmMsg STRM_PAUSE_CNF"
#define MMIVP_FSM_5214_112_2_18_3_5_14_195 "[VP] MMIVP_DispatchSignalToFsmMsg STRM_RESUME_CNF"
#define MMIVP_FSM_5233_112_2_18_3_5_14_196 "[VP] MMIVP_DispatchSignalToFsmMsg STRM_STOP_CNF"
#define MMIVP_FSM_5250_112_2_18_3_5_14_197 "[VP] MMIVP_DispatchSignalToFsmMsg STRM_CLOSE_CNF"
#define MMIVP_FSM_5281_112_2_18_3_5_14_198 "[VP] MMIVP_DispatchSignalToFsmMsg APP_VP_END_IND"
#define MMIVP_FSM_5295_112_2_18_3_5_14_199 "[VP] MMIVP_DispatchSignalToFsmMsg APP_VP_PLAY_NEXT_FILE_REQ"
#define MMIVP_FSM_5331_112_2_18_3_5_14_200 "[VP] MMIAPIVP_FSMInit ui callback is null"
#define MMIVP_FSM_5339_112_2_18_3_5_14_201 "[VP] MMIAPIVP_FSMInit create fsm fail"
#define MMIVP_FSM_5342_112_2_18_3_5_14_202 "[VP ] MMIAPIVP_FSMInit create fsm handle = %x"
#define MMIVP_FSM_5346_112_2_18_3_5_14_203 "[VP] MMIAPIVP_FSMInit dispath msg fail"
#define MMIVP_FSM_5363_112_2_18_3_5_14_204 "[VP] MMIAPIVP_FSMRelease vp_handle = %x"
#define MMIVP_FSM_5367_112_2_18_3_5_14_205 "[VP] MMIAPIVP_FSMRelease some wrong with vp_handle = %x"
#define MMIVP_FSM_5394_112_2_18_3_5_14_206 "[VP] MMIAPIVP_FSMPlay some wrong with vp_handle = %x,play_param_ptr = %x"
#define MMIVP_FSM_5406_112_2_18_3_5_14_207 "[VP] MMIAPIVP_FSMPlay begin"
#define MMIVP_FSM_5409_112_2_18_3_5_14_208 "[VP] MMIAPIVP_FSMPlay result = %d"
#define MMIVP_FSM_5430_112_2_18_3_5_14_209 "[VP] MMIAPIVP_FSMStop some wrong with vp_handle = %x"
#define MMIVP_FSM_5436_112_2_18_3_5_14_210 "[VP] MMIAPIVP_FSMStop begin"
#define MMIVP_FSM_5438_112_2_18_3_5_14_211 "[VP] MMIAPIVP_FSMStop result = %d"
#define MMIVP_FSM_5455_112_2_18_3_5_14_212 "[VP] MMIAPIVP_FSMPause some wrong with vp_handle = %x"
#define MMIVP_FSM_5458_112_2_18_3_5_14_213 "[VP] MMIAPIVP_FSMPause begin"
#define MMIVP_FSM_5462_112_2_18_3_5_14_214 "[VP] MMIAPIVP_FSMPause result = %d"
#define MMIVP_FSM_5483_112_2_18_3_5_14_215 "[VP] MMIAPIVP_FSMResume some wrong with vp_handle = %x"
#define MMIVP_FSM_5486_112_2_18_3_5_14_216 "[VP] MMIAPIVP_FSMResume begin"
#define MMIVP_FSM_5489_112_2_18_3_5_14_217 "[VP] MMIAPIVP_FSMResume result = %d"
#define MMIVP_FSM_5511_112_2_18_3_5_14_218 "[VP] MMIAPIVP_FSMSeek some wrong with vp_handle = %x"
#define MMIVP_FSM_5514_112_2_18_3_5_14_219 "[VP] MMIAPIVP_FSMSeek begin"
#define MMIVP_FSM_5518_112_2_18_3_5_14_220 "[VP] MMIAPIVP_FSMSeek result = %d"
#define MMIVP_FSM_5540_112_2_18_3_5_14_221 "[VP] VP_StopSetRate some wrong with vp_handle = %x"
#define MMIVP_FSM_5543_112_2_18_3_5_14_222 "[VP] VP_StopSetRate begin"
#define MMIVP_FSM_5547_112_2_18_3_5_14_223 "[VP] VP_StopSetRate result = %d"
#define MMIVP_FSM_5565_112_2_18_3_5_14_224 "[VP] VP_StartSetRate some wrong with vp_handle = %x"
#define MMIVP_FSM_5568_112_2_18_3_5_14_225 "[VP] VP_StartSetRate begin"
#define MMIVP_FSM_5572_112_2_18_3_5_14_226 "[VP] VP_StartSetRate result = %d"
#define MMIVP_FSM_5591_112_2_18_3_5_14_227 "[VP] VP_AuthRsp some wrong with vp_handle = %x"
#define MMIVP_FSM_5597_112_2_18_3_5_14_228 "[VP] VP_AuthRsp result = %d"
#define MMIVP_FSM_5618_112_2_18_3_5_14_229 "[VP] MMIAPIVP_FSMSwitchPlayDevice the fsm is wrong fsm_ptr = %x"
#define MMIVP_FSM_5622_112_2_18_3_5_14_230 "[VP] MMIAPIVP_FSMSwitchPlayDevice is_bt_support = %d, is_a2dp_support = %d"
#define MMIVP_FSM_5660_112_2_18_3_5_14_231 "[VP] MMIAPIVP_FSMSwitchDisplayMode the fsm is wrong fsm_ptr = %x"
#define MMIVP_FSM_5665_112_2_18_3_5_14_232 "[VP] MMIAPIVP_FSMSwitchDisplayMode fsm_ptr->state = %d, is_only_rect = %d"
#define MMIVP_FSM_5669_112_2_18_3_5_15_233 "[VP] MMIAPIVP_FSMSwitchDisplayMode lcd_id = %d, block_id = %d, x= %d"
#define MMIVP_FSM_5701_112_2_18_3_5_15_234 "[VP] MMIAPIVP_FSMSwitchDisplayMode result = %d"
#define MMIVP_FSM_5719_112_2_18_3_5_15_235 "[VP] MMIAPIVP_FSMGetState state = %d"
#define MMIVP_FSM_5734_112_2_18_3_5_15_236 "[VP] MMIAPIVP_FSMExit the fsm is wrong vp_handle = %x"
#define MMIVP_FSM_5755_112_2_18_3_5_15_237 "[VP] MMIAPIVP_FSMCompulsivePause the fsm is wrong vp_handle = %x"
#define MMIVP_FSM_5759_112_2_18_3_5_15_238 "[VP] MMIAPIVP_FSMCompulsivePause begin fsm_ptr->is_to_resume = %d"
#define MMIVP_FSM_5778_112_2_18_3_5_15_239 "[VP] MMIAPIVP_FSMCompulsivePause end fsm_ptr->is_to_pause = %d"
#define MMIVP_FSM_5796_112_2_18_3_5_15_240 "[VP] MMIAPIVP_FSMCompulsiveResume the fsm is wrong vp_handle = %x"
#define MMIVP_FSM_5799_112_2_18_3_5_15_241 "[VP] MMIAPIVP_FSMCompulsiveResume begin fsm_ptr->is_to_pause = %d"
#define MMIVP_FSM_5852_112_2_18_3_5_15_242 "[VP] MMIAPIVP_FSMCompulsiveResume end fsm_ptr->is_to_resume = %d"
#define MMIVP_FSM_5902_112_2_18_3_5_15_243 "[VP] MMIAPIVP_FSMGetFrameOffset info_result = %d, frame_index = %d, time_offset = %d, media_info.video_info.frame_rate = %d, base = %d"
#define MMIVP_FSM_5924_112_2_18_3_5_15_244 "[VP] VP_GetMediaInfo param is error"
#define MMIVP_FSM_5962_112_2_18_3_5_15_245 "[VP] MMIAPIVP_Play param is null"
#define MMIVP_FSM_5966_112_2_18_3_5_15_246 "[VP] MMIAPIVP_Play  start_time = %d"
#define MMIVP_FSM_5978_112_2_18_3_5_15_247 "[VP] MMIAPIVP_Play play_param_ptr = %x, full_path_len = %d, full_path_ptr = %x, is_random_play = %d, mode = %d, play_type = %d, url_ptr = %x, video_buf_info.type = %d, video_buf_len = %d, video_buf_ptr = %x"
#define MMIVP_FSM_5987_112_2_18_3_5_15_248 "[VP] MMIAPIVP_Play audioSupport = %d, Height = %d, RotateAngle = %d, videoSupport = %d,Width = %d,xOffset = %d, yOffset = %d"
#define MMIVP_FSM_5997_112_2_18_3_5_15_249 "[VP] MMIAPIVP_Play fsm is not ready now, player wait"
#define MMIVP_FSM_6003_112_2_18_3_5_15_250 "[VP] MMIAPIVP_Play call_back_func = %x ,start_time = %d"
#define MMIVP_FSM_6029_112_2_18_3_5_15_251 "[VP] MMIAPIVP_ExitPlay s_current_fsm_ptr = %x"
#define MMIVP_FSM_6066_112_2_18_3_5_15_252 "[VP]: MMIAPIVP_GetInfoFromFullPathName() param error"
#define MMIVP_FSM_6078_112_2_18_3_5_15_253 "[VP]: MMIAPIVP_GetInfoFromFullPathName: open dplayer fail! "
#define MMIVP_FSM_6100_112_2_18_3_5_15_254 "[VP]: MMIAPIVP_GetInfoFromFullPathName() return = 0x%.2x, full_name_len = %d"
#define MMIVP_FSM_6111_112_2_18_3_5_15_255 "[VP]: MMIAPIVP_GetInfoFromFullPathName():ntfile_type = %d,ntlen = %d,ntv_spt = %d,ntf_rate = %d,ntheight = %d,ntwidth = %d,ntvideo_type = %d,ntaudio_type = %d"
#define MMIVP_FSM_6141_112_2_18_3_5_15_256 "[VP]: MMIAPIVP_GetMediaInfoFromBuf param error"
#define MMIVP_FSM_6145_112_2_18_3_5_15_257 "[VP]: MMIAPIVP_GetMediaInfoFromBuf: video_type = %d, video_length = %d"
#define MMIVP_FSM_6180_112_2_18_3_5_16_258 "[VP]: MMIAPIVP_GetMediaInfoFromBuf: open dplayer fail! "
#define MMIVP_FSM_6207_112_2_18_3_5_16_259 "[VP] MMIAPIVP_GetSupportedStatus param is null"
#define MMIVP_FSM_6235_112_2_18_3_5_16_260 "[VP] CheckSupportedTypeFromMediaInfo param is null"
#define MMIVP_FSM_6244_112_2_18_3_5_16_261 "[VP] CheckSupportedTypeFromMediaInfo video_support = %d, audio_support = %d , video_type = %d, audio_type = %d, height = %d, width = %d"
#define MMIVP_FSM_6297_112_2_18_3_5_16_262 "[VP] MMIVP_RegTaskID vp_handle = %x, task_id = %d"
#define MMIVP_FSM_6315_112_2_18_3_5_16_263 "[VP] MMIVP_UnRegTaskID vp_handle = %x, task_id = %d"
#define MMIVP_FSM_6547_112_2_18_3_5_16_264 "[VP] s_switch_timer_id = 0x%x"
#define MMIVP_FSM_6564_112_2_18_3_5_16_265 "[VP] StopDelayPlayVideoPlayerTimer Stop Timer"
#define MMIVP_FSM_6616_112_2_18_3_5_16_266 "[VP] VP_dplayerOpenSecurty handle=0x%x"
#define MMIVP_FSM_6630_112_2_18_3_5_16_267 "VP_dplayerOpenSecurty, VPlayerSecurity_GetVDecSecPlugInfo,FID=%d,plug_handle=0x%x"
#define MMIVP_FSM_6640_112_2_18_3_5_16_268 "[VP] VP_dplayerOpenSecurty handle=0x%x"
#define MMIVP_FSM_6645_112_2_18_3_5_16_269 "VP_dplayerOpenSecurty, VPlayerSecurity_GetVDecSecPlugInfo,FID=%d,plug_handle=0x%x"
#define MMIVP_FSM_6664_112_2_18_3_5_16_270 "VP_dplayerCloseSecurty,FID=%d,handle=0x%x,plug_handle=0x%x"
#define MMIVP_FSM_6686_112_2_18_3_5_17_271 "VP_dplayerCloseSecurty,FID=%d,handle=0x%x,plug_handle=0x%x"
#define MMIVP_FSM_6713_112_2_18_3_5_17_272 "VP_dplayerExtCtlAddSecurty entry,s_vdec_sec_argv[0]=0x%x"
#define MMIVP_FSM_6735_112_2_18_3_5_17_273 "VP_dplayerExtCtlDeltSecurty handle = 0x%x,s_vdec_sec_argv[0]=0x%x"
#define MMIVP_FSM_6890_112_2_18_3_5_17_274 "[VP]:MMIAPIVP_CurrentVideoLcdSwitch result=%d,Angle=%d"
#define MMIVP_FSM_6908_112_2_18_3_5_17_275 "[VP]:MMIAPIVP_SetBgVideoAudioSupport audioSupport=%d,audioSupport=%d"
#define MMIVP_FSM_6936_112_2_18_3_5_17_276 "[VP]:MMIAPIVP_Pause pause current video"
#define MMIVP_FSM_6950_112_2_18_3_5_17_277 "[VP]:MMIAPIVP_Resume resume current video"
#define MMIVP_FSM_6968_112_2_18_3_5_17_278 "[VP]:VPPlayCallBack event=%d"
#define MMIVP_FSM_6985_112_2_18_3_5_17_279 "[VP]:VPPlayCallBack report=%d,data=%d"
#define MMIVP_FSM_7088_112_2_18_3_5_17_280 "[VP]:ReqPlaySrv handle is already exist!!"
#define MMIVP_FSM_7104_112_2_18_3_5_17_281 "[VP]: MMISRVAUD_SetAudioInfo error!!!!!!"
#define MMIVP_FSM_7135_112_2_18_3_5_17_282 "[VP]:ReqPlaySrv handle = %d"
#define MMIVP_HISTORYLIST_114_112_2_18_3_5_18_283 "[VP] GetHistoryListFileNameForWrite min_free_size = %d"
#define MMIVP_HISTORYLIST_158_112_2_18_3_5_18_284 "[VP] GetHistoryListSpaceSize load_size = %x"
#define MMIVP_HISTORYLIST_173_112_2_18_3_5_18_285 "[VP] FreeHistoryListItem index = %d"
#define MMIVP_HISTORYLIST_216_112_2_18_3_5_18_286 "[VP] FreeHistoryListItem  result = %d"
#define MMIVP_HISTORYLIST_285_112_2_18_3_5_18_287 "[VP] SaveHistorylistToFile load_size = %x, buffer_ptr = %x"
#define MMIVP_HISTORYLIST_306_112_2_18_3_5_18_288 "[VP]SaveHistorylistToFile pathlen==0  error!!!!!!!"
#define MMIVP_HISTORYLIST_322_112_2_18_3_5_18_289 "[VP] SaveHistorylistToFile write_size = %d"
#define MMIVP_HISTORYLIST_462_112_2_18_3_5_19_290 "[VP] VP_HistoryList_Load readlist_result = %d"
#define MMIVP_HISTORYLIST_471_112_2_18_3_5_19_291 "[VP] VP_HistoryList_Load s_visit_his_que_ptr = %x"
#define MMIVP_HISTORYLIST_479_112_2_18_3_5_19_292 "[VP] VP_HistoryList_Load s_visit_his_que_ptr->item_count = %d"
#define MMIVP_HISTORYLIST_483_112_2_18_3_5_19_293 "[VP] VP_HistoryList_Load cur_item_info_ptr = %x"
#define MMIVP_HISTORYLIST_513_112_2_18_3_5_19_294 "[VP] VP_HistoryList_Load read_size = %d"
#define MMIVP_HISTORYLIST_632_112_2_18_3_5_19_295 "[VP] VP_HistoryList_GetInfo list_index = %d"
#define MMIVP_HISTORYLIST_645_112_2_18_3_5_19_296 "[VP] VP_HistoryList_GetInfo i=%d"
#define MMIVP_HISTORYLIST_659_112_2_18_3_5_19_297 "[VP] VP_HistoryList_DelItem index = %d"
#define MMIVP_HISTORYLIST_662_112_2_18_3_5_19_298 "[VP] VP_HistoryList_DelItem error param"
#define MMIVP_HISTORYLIST_671_112_2_18_3_5_19_299 "[VP] VP_HistoryList_DelItem result = %d, s_visit_his_que_ptr->item_count = %d"
#define MMIVP_HISTORYLIST_707_112_2_18_3_5_19_300 "[VP] VP_HistoryList_AddItem there is no enough space"
#define MMIVP_HISTORYLIST_750_112_2_18_3_5_19_301 "[VP] VP_HistoryList_AddItem current_history_item_ptr = %x"
#define MMIVP_HISTORYLIST_766_112_2_18_3_5_19_302 "[VP] VP_HistoryList_AddItem s_visit_his_que_ptr = %x"
#define MMIVP_HISTORYLIST_772_112_2_18_3_5_19_303 "[VP] VP_HistoryList_AddItem there is no enough space"
#define MMIVP_HISTORYLIST_814_112_2_18_3_5_19_304 "[VP] VP_HistoryList_SetLastPlayPoint last_play_point = %d"
#define MMIVP_HISTORYLIST_818_112_2_18_3_5_19_305 "[VP] VP_HistoryList_SetLastPlayPoint list error"
#define MMIVP_HISTORYLIST_849_112_2_18_3_5_19_306 "[VP] VP_HistoryList_ModifyItem  last_play_point = %d, video_height = %d, video_width = %d, media_length = %d, title_len = %d"
#define MMIVP_LOCALLIST_120_112_2_18_3_5_20_307 "[VP] MMIAPIVP_GetFilerInfo param is null"
#define MMIVP_LOCALLIST_166_112_2_18_3_5_20_308 "[VP] AddFileToLocalListCallBack is_success = %d, file_num = %d, file_array = %x, s_oplist_info.locallist_type = %d"
#define MMIVP_LOCALLIST_174_112_2_18_3_5_20_309 "[VP] AddFileToLocalListCallBack total_num 1 = %d"
#define MMIVP_LOCALLIST_176_112_2_18_3_5_20_310 "[VP] AddFileToLocalListCallBack total_num = %d"
#define MMIVP_LOCALLIST_193_112_2_18_3_5_20_311 "[VP] AddFileToLocalListCallBack is_success = %d, s_oplist_info.locallist_type = %d"
#define MMIVP_LOCALLIST_245_112_2_18_3_5_20_312 "[VP] GetLocalListFileNameForRead locallist_type = %d, result = %d"
#define MMIVP_LOCALLIST_262_112_2_18_3_5_20_313 "[VP] GetLocalListFileNameForWrite locallist_type = %d"
#define MMIVP_LOCALLIST_295_112_2_18_3_5_20_314 "[VP] GetLocalListFileNameForWrite result = %d"
#define MMIVP_LOCALLIST_310_112_2_18_3_5_20_315 "[VP] SaveLocallistToFile locallist_type = %d"
#define MMIVP_LOCALLIST_353_112_2_18_3_5_20_316 "[VP] SaveLocallistToFile save_result = %d"
#define MMIVP_LOCALLIST_477_112_2_18_3_5_21_317 "[VP] MMIVP_GetDefaultMoviePath, file_dev_type = %d"
#define MMIVP_LOCALLIST_497_112_2_18_3_5_21_318 "[VP] GetDifferentRandom locallist_type = %d, locallist_index = %d"
#define MMIVP_LOCALLIST_570_112_2_18_3_5_21_319 "[VP] VP_LocalList_BeginLoad locallist_type = %d,file_dev_type=%d"
#define MMIVP_LOCALLIST_594_112_2_18_3_5_21_320 "[VP] MMIVP_GetDefaultMoviePath is failed!"
#define MMIVP_LOCALLIST_625_112_2_18_3_5_21_321 "[VP] VP_LocalList_BeginLoad load_result = %d, locallist_type = %d"
#define MMIVP_LOCALLIST_636_112_2_18_3_5_21_322 "[VP] VP_LocalList_EndLoad locallist_type = %d"
#define MMIVP_LOCALLIST_652_112_2_18_3_5_21_323 "[VP] VP_LocalList_Release locallist_type = %d"
#define MMIVP_LOCALLIST_672_112_2_18_3_5_21_324 "[VP] VP_LocalList_GetInfo locallist_type = %d, locallist_index = %d"
#define MMIVP_LOCALLIST_677_112_2_18_3_5_21_325 "[VP] VP_LocalList_GetInfo locallist_type = %d, result = %d, locallist_index = %d"
#define MMIVP_LOCALLIST_695_112_2_18_3_5_21_326 "[VP] VP_LocalList_GetNum locallist_type = %d, locallist_num = %d"
#define MMIVP_LOCALLIST_712_112_2_18_3_5_21_327 "[VP] VP_LocalList_AddItem locallist_type = %d"
#define MMIVP_LOCALLIST_778_112_2_18_3_5_21_328 "[VP] VP_LocalList_BeginUpdate locallist_type = %d"
#define MMIVP_LOCALLIST_821_112_2_18_3_5_21_329 "[VP] VP_LocalList_EndUpdate locallist_type = %d"
#define MMIVP_LOCALLIST_835_112_2_18_3_5_21_330 "[VP] VP_LocalList_DelItemWithouSaveFile locallist_type= %d"
#define MMIVP_LOCALLIST_850_112_2_18_3_5_22_331 "[VP] VP_Locallist_SaveToFile locallist_type = %d"
#define MMIVP_LOCALLIST_866_112_2_18_3_5_22_332 "[VP] VP_LocalList_GetNext cur_index = %d, locallist_index = %d"
#define MMIVP_LOCALLIST_919_112_2_18_3_5_22_333 "[VP] VP_LocalList_GetPrev cur_index = %d, total_num = %d, locallist_type = %d"
#define MMIVP_LOCALLIST_979_112_2_18_3_5_22_334 "[VP] VP_LocalList_ResetCurPlayList is_random_play = %d, total_num = %d"
#define MMIVP_LOCALLIST_1029_112_2_18_3_5_22_335 "[VP] VP_LocalList_SetIsCurPlayListItemOK is_ok = %d , s_playing_list.active_index = %d"
#define MMIVP_LOCALLIST_1072_112_2_18_3_5_22_336 "[VP] VP_LocalList_GetPreByMode mode = %d, is_random = %d, total_num = %d,locallist_type = %d"
#define MMIVP_LOCALLIST_1083_112_2_18_3_5_22_337 "[VP] VP_LocalList_GetPreByMode param is error"
#define MMIVP_LOCALLIST_1123_112_2_18_3_5_22_338 "[VP] VP_LocalList_GetPreByMode one circle"
#define MMIVP_LOCALLIST_1150_112_2_18_3_5_22_339 "[VP] VP_LocalList_GetPreByMode one circle"
#define MMIVP_LOCALLIST_1154_112_2_18_3_5_22_340 "[VP] VP_LocalList_GetPreByMode s_playing_list.playing_list_ptr[i].is_ok = %d"
#define MMIVP_LOCALLIST_1187_112_2_18_3_5_22_341 "[VP] VP_LocalList_GetPreByMode result = %d,next_index = %d"
#define MMIVP_LOCALLIST_1206_112_2_18_3_5_22_342 "[VP] VP_LocalList_GetPreByMode result = %d"
#define MMIVP_LOCALLIST_1230_112_2_18_3_5_22_343 "[VP] VP_LocalList_GetNextByMode mode = %d, is_random = %d, total_num = %d,locallist_type = %d"
#define MMIVP_LOCALLIST_1241_112_2_18_3_5_22_344 "[VP] VP_LocalList_GetNextByMode param is error"
#define MMIVP_LOCALLIST_1282_112_2_18_3_5_22_345 "[VP] VP_LocalList_GetNextByMode one circle"
#define MMIVP_LOCALLIST_1309_112_2_18_3_5_22_346 "[VP] VP_LocalList_GetNextByMode one circle"
#define MMIVP_LOCALLIST_1313_112_2_18_3_5_22_347 "[VP] VP_LocalList_GetNextByMode s_playing_list.playing_list_ptr[i].is_ok = %d"
#define MMIVP_LOCALLIST_1346_112_2_18_3_5_23_348 "[VP] VP_LocalList_GetNextByMode result = %d,next_index = %d"
#define MMIVP_LOCALLIST_1365_112_2_18_3_5_23_349 "[VP] VP_LocalList_GetNextByMode result = %d"
#define MMIVP_LOCALLIST_1377_112_2_18_3_5_23_350 "[VP] VP_LocalList_GetCurrentIndex locallist_type = %d, s_current_list_info.locallist_type = %d"
#define MMIVP_LOCALLIST_1391_112_2_18_3_5_23_351 "[VP] VP_LocalList_SetCurrentIndex index = %d"
#define MMIVP_LOCALLIST_1411_112_2_18_3_5_23_352 "[VP] VP_LocalList_SetCurListType type = %d"
#define MMIVP_SETTING_136_112_2_18_3_5_24_353 "[VP] VP_Setting_GetBrightness nv_setting_info.brightness = %d"
#define MMIVP_SETTING_147_112_2_18_3_5_24_354 "[VP] VP_Setting_SetBrightness brightness = %d"
#define MMIVP_SETTING_161_112_2_18_3_5_24_355 "[VP] VP_Setting_GetPlaybackMode nv_setting_info.playback_mode = %d"
#define MMIVP_SETTING_172_112_2_18_3_5_24_356 "[VP] VP_Setting_SetRepeatPlayMode playback_mode = %d"
#define MMIVP_SETTING_186_112_2_18_3_5_24_357 "[VP] VP_Setting_GetRepeatPlayMode nv_setting_info.mode = %d"
#define MMIVP_SETTING_197_112_2_18_3_5_24_358 "[VP] VP_Setting_SetRepeatPlayMode mode = %d"
#define MMIVP_SETTING_211_112_2_18_3_5_25_359 "[VP] VP_Setting_GetIsRandomPlayMode nv_setting_info.is_random_play = %d"
#define MMIVP_SETTING_222_112_2_18_3_5_25_360 "[VP] VP_Setting_SetIsRandomPlayMode is_open = %d"
#define MMIVP_SETTING_238_112_2_18_3_5_25_361 "[VP] VP_Setting_GetNetIndex dual_sys = %d"
#define MMIVP_SETTING_243_112_2_18_3_5_25_362 "[VP] VP_Setting_GetNetIndex dual_sys = %d, index = %d"
#define MMIVP_SETTING_255_112_2_18_3_5_25_363 "[VP] VP_Setting_SetNetIndex dual_sys = %d, index = %d"
#define MMIVP_SETTING_277_112_2_18_3_5_25_364 "[VP] VP_Setting_SetRTPPort lowest_port = %d, highest_port = %d"
#define MMIVP_SETTING_296_112_2_18_3_5_25_365 "[VP] VP_Setting_GetRTPPort param is error"
#define MMIVP_SETTING_319_112_2_18_3_5_25_366 "[VP] VP_Setting_SetAgent param is error"
#define MMIVP_SETTING_351_112_2_18_3_5_25_367 "[VP] VP_Setting_GetAgent param is error"
#define MMIVP_THUMBNAIL_94_112_2_18_3_5_25_368 "[VP]:HandleOpenNextVideo!"
#define MMIVP_THUMBNAIL_126_112_2_18_3_5_25_369 "[VP]:HandleOpenNextVideo  error state , not video queue"
#define MMIVP_THUMBNAIL_143_112_2_18_3_5_26_370 "[VP]:HandleGetThumbnaiData stopping!"
#define MMIVP_THUMBNAIL_148_112_2_18_3_5_26_371 "[VP]:HandleGetThumbnaiData error msg!"
#define MMIVP_THUMBNAIL_161_112_2_18_3_5_26_372 "[VP]:HandleGetThumbnaiData height=%d,width=%d,datalen=%d!"
#define MMIVP_THUMBNAIL_212_112_2_18_3_5_26_373 "[VP]:VP_ThumbnailGetVideoPathname"
#define MMIVP_THUMBNAIL_230_112_2_18_3_5_26_374 "[VP]:MMIAPIVP_IsThumbnailRunning isloading:%d"
#define MMIVP_THUMBNAIL_246_112_2_18_3_5_26_375 "[VP]:MMIAPIVP_IsThumbnailOpen isopen:%d"
#define MMIVP_THUMBNAIL_264_112_2_18_3_5_26_376 "[VP]:MMIAPIVP_ThumbnailVideoAdd  !"
#define MMIVP_THUMBNAIL_272_112_2_18_3_5_26_377 "[VP]:VP_ThumbnailAddVideo full queue!"
#define MMIVP_THUMBNAIL_287_112_2_18_3_5_26_378 "[VP]:VP_ThumbnailAddVideo videonum:%d!"
#define MMIVP_THUMBNAIL_306_112_2_18_3_5_26_379 "[VP]:MMIAPIVP_ThumbnailOpen !"
#define MMIVP_THUMBNAIL_315_112_2_18_3_5_26_380 "[VP]:MMIAPIVP_ThumbnailOpen is open now!"
#define MMIVP_THUMBNAIL_325_112_2_18_3_5_26_381 "[VP]:MMIVP_GetSeekPointImgeData list_file_name :0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x"
#define MMIVP_THUMBNAIL_344_112_2_18_3_5_26_382 "[VP]:MMIAPIVP_ThumbnailOpen alloc mem error!"
#define MMIVP_THUMBNAIL_384_112_2_18_3_5_26_383 "[VP]:MMIAPIVP_ThumbnailResume !"
#define MMIVP_THUMBNAIL_388_112_2_18_3_5_26_384 "[VP]:MMIAPIVP_ThumbnailResume  thumnail is loading!!"
#define MMIVP_THUMBNAIL_407_112_2_18_3_5_26_385 "[VP]:MMIAPIVP_ThumbnailStop !"
#define MMIVP_THUMBNAIL_430_112_2_18_3_5_26_386 "[VP]:MMIAPIVP_ThumbnailClose!"
#define MMIVP_THUMBNAIL_458_112_2_18_3_5_26_387 "[VP]:StopCurrentDPLAYER!"
#define MMIVP_THUMBNAIL_473_112_2_18_3_5_26_388 "[VP]:Thumbnail_VideoQueueRun  !"
#define MMIVP_THUMBNAIL_477_112_2_18_3_5_26_389 "[VP]:Thumbnail_VideoQueueRun is not videoqueue !"
#define MMIVP_THUMBNAIL_482_112_2_18_3_5_26_390 "[VP]:Thumbnail_VideoQueueRun  queue empty !"
#define MMIVP_THUMBNAIL_494_112_2_18_3_5_26_391 "[VP]:Thumbnail_VideoQueueRun   fail !"
#define MMIVP_THUMBNAIL_513_112_2_18_3_5_26_392 "[VP] CheckSupportedTypeFromMedia param is null"
#define MMIVP_THUMBNAIL_522_112_2_18_3_5_26_393 "[VP] CheckSupportedTypeFromMedia video_support = %d, audio_support = %d , video_type = %d, audio_type = %d, height = %d, width = %d"
#define MMIVP_THUMBNAIL_557_112_2_18_3_5_26_394 "[VP]:Thumbnail_OpenDplayer !"
#define MMIVP_THUMBNAIL_560_112_2_18_3_5_26_395 "[VP]:Thumbnail_OpenDplayer is loading now!"
#define MMIVP_THUMBNAIL_582_112_2_18_3_5_26_396 "[VP]:Thumbnail_OpenDplayer   s_thumbnail_info.dphandle:0X%0x"
#define MMIVP_THUMBNAIL_600_112_2_18_3_5_26_397 "[VP]:Thumbnail_OpenDplayer CheckSupportedTypeFromMediaInfo fail !"
#define MMIVP_THUMBNAIL_625_112_2_18_3_5_27_398 "[VP]:Thumbnail_OpenDplayer DPLAYER_Play fail !"
#define MMIVP_THUMBNAIL_640_112_2_18_3_5_27_399 "[VP]:Thumbnail_OpenDplayer DPLAYER_SetTime fail !"
#define MMIVP_THUMBNAIL_656_112_2_18_3_5_27_400 "[VP]:Thumbnail_OpenDplayer DPLAYER_SetDisplayParam fail !"
#define MMIVP_THUMBNAIL_672_112_2_18_3_5_27_401 "[VP]:Thumbnail_OpenDplayer DPLAYER_Open fail !"
#define MMIVP_THUMBNAIL_692_112_2_18_3_5_27_402 "[VP]:Thumbnail_VideoDelete Video pre num:%d !"
#define MMIVP_THUMBNAIL_695_112_2_18_3_5_27_403 "[VP]:Thumbnail_VideoDelete  queue empty !"
#define MMIVP_THUMBNAIL_705_112_2_18_3_5_27_404 "[VP]:Thumbnail_VideoDelete Video post num:%d !"
#define MMIVP_THUMBNAIL_732_112_2_18_3_5_27_405 "[VP]:CallbackThumbnaiEndNotify,dphandle:0X%0x,is_loading:%d,is_stopping:%d"
#define MMIVP_THUMBNAIL_746_112_2_18_3_5_27_406 "[VP]:CallbackOneFrameEndNotify,dphandle:0X%0x,is_loading:%d,is_stopping:%d"
#define MMIVP_WINTABLE_2091_112_2_18_3_5_32_407 "[VP] StartProcessTimer before s_process_timer_id = %d"
#define MMIVP_WINTABLE_2105_112_2_18_3_5_32_408 "[VP] StartProcessTimer s_process_timer_id = %d"
#define MMIVP_WINTABLE_2133_112_2_18_3_5_32_409 "[VP] StopProcessTimer s_process_timer_id = %d"
#define MMIVP_WINTABLE_2175_112_2_18_3_5_32_410 "[VP] HandleFsmMsg param is error"
#define MMIVP_WINTABLE_2178_112_2_18_3_5_32_411 "[VP] HandleFsmMsg msg_ptr->msg_id = %x"
#define MMIVP_WINTABLE_2199_112_2_18_3_5_32_412 "[VP] HandleFsmMsg play_type = %d, total_time = %d,support_type = %d"
#define MMIVP_WINTABLE_2227_112_2_18_3_5_32_413 "[VP] HandleFsmMsg buffering s_video_player_control_info.state = %d"
#define MMIVP_WINTABLE_2255_112_2_18_3_5_32_414 "[VP] HandleFsmMsg buffer end s_video_player_control_info.state = %d"
#define MMIVP_WINTABLE_2285_112_2_18_3_5_32_415 "[VP] HandleFsmMsg play_result.error_code = %d, s_video_player_control_info.to_do_type = %d, s_video_player_control_info.process_time = %d"
#define MMIVP_WINTABLE_2290_112_2_18_3_5_32_416 "[VP] HandleFsmMsg play cnf s_video_player_control_info.state = %d"
#define MMIVP_WINTABLE_2314_112_2_18_3_5_32_417 "[VP] HandleFsmMsg s_tp_down_ctrl_id = %d, s_key_down_msg_id = %d"
#define MMIVP_WINTABLE_2361_112_2_18_3_5_32_418 "[VP] HandleFsmMsg play cnf error s_video_player_control_info.state = %d"
#define MMIVP_WINTABLE_2399_112_2_18_3_5_33_419 "[VP] HandleFsmMsg pause_result.error_code = %d"
#define MMIVP_WINTABLE_2402_112_2_18_3_5_33_420 "[VP] HandleFsmMsg pause cnf s_video_player_control_info.state = %d"
#define MMIVP_WINTABLE_2424_112_2_18_3_5_33_421 "[VP] HandleFsmMsg seek_result.error_code = %d"
#define MMIVP_WINTABLE_2452_112_2_18_3_5_33_422 "[VP] HandleFsmMsg fatal_error.error_code = %d"
#define MMIVP_WINTABLE_2496_112_2_18_3_5_33_423 "[VP] HandleFsmMsg play_end_ind.result = %d"
#define MMIVP_WINTABLE_2565_112_2_18_3_5_33_424 "[VP] HandleFsmMsg stop cnf s_video_player_control_info.state = %d"
#define MMIVP_WINTABLE_2600_112_2_18_3_5_33_425 "[VP] VP_STOP_IND s_video_player_control_info.to_do_type = %d"
#define MMIVP_WINTABLE_2633_112_2_18_3_5_33_426 "[VP] VP_STOP_IND full_path_len = %d"
#define MMIVP_WINTABLE_2701_112_2_18_3_5_33_427 "[VP] SaveHistory locallist_type = %d, locallist_index = %d"
#define MMIVP_WINTABLE_2774_112_2_18_3_5_33_428 "[VP] NotifyPlayerForPlayFile full_path_name_ptr = %x, process_time = %d, play_type = %d, full_path_name_len = %d"
#define MMIVP_WINTABLE_2820_112_2_18_3_5_33_429 "[VP] PlayFromNet process_time = %d"
#define MMIVP_WINTABLE_2887_112_2_18_3_5_34_430 "[VP] NotifyPlayerForPlayNet full_path_name_ptr = %x, url_ptr=%x, process_time = %d, full_path_name_len = %d"
#define MMIVP_WINTABLE_3045_112_2_18_3_5_34_431 "MMIAPIVP_PlayVideoFromVideoPlayer java running =%d"
#define MMIVP_WINTABLE_3053_112_2_18_3_5_34_432 "[VP] MMIAPIVP_PlayVideoFromVideoPlayer vp_entry = %d"
#define MMIVP_WINTABLE_3061_112_2_18_3_5_34_433 "[VP] MMIAPIVP_PlayVideoFromVideoPlayer MMIAPIBROWSER_IsRunning()"
#define MMIVP_WINTABLE_3113_112_2_18_3_5_34_434 "[VP] MMIAPIVP_MainEntry cannot creat fsm"
#define MMIVP_WINTABLE_3168_112_2_18_3_5_34_435 "[VP] MMIAPIVP_ExitVideoPlayer"
#define MMIVP_WINTABLE_3210_112_2_18_3_5_34_436 "[VP] HandleVPSearchWaitingWin msg_id = %x, is_open_player_win = %d, locallist_type = %d"
#define MMIVP_WINTABLE_3242_112_2_18_3_5_34_437 "[VP] HandleVPSearchWaitingWin list_index = %d"
#define MMIVP_WINTABLE_3646_112_2_18_3_5_35_438 "[VP] DisplayVPWinStation display_station.wstr_len = %d,text_string.wstr_len = %d"
#define MMIVP_WINTABLE_3767_112_2_18_3_5_35_439 "[VP] DisplayVPWinRepeatModeIcon display_type = %d"
#define MMIVP_WINTABLE_3869_112_2_18_3_5_36_440 "[VP] DisplayVPWinRandomModeIcon display_type = %d"
#define MMIVP_WINTABLE_4048_112_2_18_3_5_36_441 "[VP] DisplayVPWinBackwardOrForwardIcon s_video_player_control_info.state = %d"
#define MMIVP_WINTABLE_4111_112_2_18_3_5_36_442 "[VP] DisplayVPWinPlayOrPauseIcon s_video_player_control_info.state = %d"
#define MMIVP_WINTABLE_4163_112_2_18_3_5_36_443 "[vp] DisplayVPWinPlayOrPauseIcon error"
#define MMIVP_WINTABLE_4211_112_2_18_3_5_36_444 "[VP] DisplayVpWinDefaultPic not display default pic"
#define MMIVP_WINTABLE_4261_112_2_18_3_5_36_445 "[VP] DisplayVpWinVideoArea s_video_player_control_info.state = %d"
#define MMIVP_WINTABLE_4481_112_2_18_3_5_37_446 "[VP] DisplayVPWinProgressEx process_time = %d, total_time = %d"
#define MMIVP_WINTABLE_4739_112_2_18_3_5_37_447 "[VP] DisappearVPWin is_disappear = %d, s_video_player_control_info.is_display_win = %d"
#define MMIVP_WINTABLE_4776_112_2_18_3_5_37_448 "[VP] DisappearVPWin s_video_player_control_info.support_type = %d"
#define MMIVP_WINTABLE_4835_112_2_18_3_5_37_449 "[VP] LoadLastPlayingFile history_info_ptr->locallist_type = %d, history_info_ptr->locallist_index = %d"
#define MMIVP_WINTABLE_4885_112_2_18_3_5_38_450 "[VP] LoadLastPlayingFile process = %d, total_time = %d"
#define MMIVP_WINTABLE_5066_112_2_18_3_5_38_451 "[VP] VideoPlayerResume s_video_player_control_info.state = %d"
#define MMIVP_WINTABLE_5171_112_2_18_3_5_38_452 "[VP] VideoPlayerPause s_video_player_control_info.state = %d"
#define MMIVP_WINTABLE_5189_112_2_18_3_5_38_453 "[VP] VideoPlayerPause s_video_player_control_info.state = %d"
#define MMIVP_WINTABLE_5202_112_2_18_3_5_38_454 "[VP] VideoPlayerPause PAUSE FAIL s_video_player_control_info.state = %d"
#define MMIVP_WINTABLE_5290_112_2_18_3_5_38_455 "[VP] NotifyFsmPlay is_bt_support = %d, is_a2dp_support = %d"
#define MMIVP_WINTABLE_5580_112_2_18_3_5_39_456 "[VP] PlayPreFile failed ! "
#define MMIVP_WINTABLE_5601_112_2_18_3_5_39_457 "[VP] Backward process_time =%d"
#define MMIVP_WINTABLE_5699_112_2_18_3_5_39_458 "[VP] PlayNextFile failed ! "
#define MMIVP_WINTABLE_5719_112_2_18_3_5_39_459 "[VP] BackWardOrPreFile seek_time = %d, s_video_player_control_info.process_time = %d"
#define MMIVP_WINTABLE_5796_112_2_18_3_5_39_460 "[VP] AdjustVolCallBack volue = %d"
#define MMIVP_WINTABLE_5811_112_2_18_3_5_39_461 "[VP] AdjustBrightCallBack volue = %d"
#define MMIVP_WINTABLE_5855_112_2_18_3_5_39_462 "[VP] StartVPWinDisplayerTimer state = %d"
#define MMIVP_WINTABLE_5896_112_2_18_3_5_40_463 "[VP] StartVPTimer timer_type = %d, s_video_player_control_info.timer.timer_type  = %d, s_video_player_control_info.timer.timer_id = %d"
#define MMIVP_WINTABLE_5919_112_2_18_3_5_40_464 "[VP] StartVPTimer s_video_player_control_info.timer.timer_id = %d"
#define MMIVP_WINTABLE_5951_112_2_18_3_5_40_465 "[VP] HandleVPTimerMsg s_video_player_control_info.timer.timer_type = %d"
#define MMIVP_WINTABLE_5984_112_2_18_3_5_40_466 "[VP] HandleDirectionKeyMsgForHalfScreen msg_id = %x, s_video_player_control_info.timer.timer_type = %d"
#define MMIVP_WINTABLE_6070_112_2_18_3_5_40_467 "[VP] HandleShortCutMsg msg_id = %x"
#define MMIVP_WINTABLE_6762_112_2_18_3_5_41_468 "[VP]: HandleVPWinMsg(), msg_id = %x"
#define MMIVP_WINTABLE_6857_112_2_18_3_5_41_469 "[VP] HandleVPWinMsg param = %d, s_video_player_control_info.timer.timer_id= %d"
#define MMIVP_WINTABLE_6963_112_2_18_3_5_42_470 "[VP] handle vp win web key s_video_player_control_info.state = %d"
#define MMIVP_WINTABLE_7080_112_2_18_3_5_42_471 "[VP]: HandleVPWinMsg(), key msg_id = %x, s_video_player_control_info.is_display_win =%d"
#define MMIVP_WINTABLE_7198_112_2_18_3_5_42_472 "[VP] OpenSeekWin x = %d"
#define MMIVP_WINTABLE_7269_112_2_18_3_5_42_473 "[VP] OpenRepeatModeWin repeat_mode = %d"
#define MMIVP_WINTABLE_7303_112_2_18_3_5_42_474 "[VP] OpenRandomModeWin is_random = %d"
#define MMIVP_WINTABLE_7323_112_2_18_3_5_42_475 "[VP] OpenControlPanel is not focus and not open control panel"
#define MMIVP_WINTABLE_7386_112_2_18_3_5_43_476 "[VP] PlayOrPause s_video_player_control_info.state = %d"
#define MMIVP_WINTABLE_7468_112_2_18_3_5_43_477 "[VP] SwitchScreen is_full = %d"
#define MMIVP_WINTABLE_7513_112_2_18_3_5_43_478 "[VP] AddFileToPlayerCallBack is_success = %d"
#define MMIVP_WINTABLE_7596_112_2_18_3_5_43_479 "[VP] SaveBookMark add_result = %d"
#define MMIVP_WINTABLE_8131_112_2_18_3_5_44_480 "[VP] HandleNetOptMenuWinMsg menu_id = %x"
#define MMIVP_WINTABLE_8246_112_2_18_3_5_44_481 "[VP] HandleOptMenuWinMsg menu_id = %x"
#define MMIVP_WINTABLE_8327_112_2_18_3_5_45_482 "[VP] HandleLocalListOptMenuWinMsg net alloca memory error"
#define MMIVP_WINTABLE_8729_112_2_18_3_5_46_483 "[VP]HandleLinkListWinMsg() msg_id=0x%04x"
#define MMIVP_WINTABLE_9142_112_2_18_3_5_47_484 "[VP] RichText_AddItem param is error"
#define MMIVP_WINTABLE_9337_112_2_18_3_5_48_485 "[VP] HandleListAppendData type = %d"
#define MMIVP_WINTABLE_9344_112_2_18_3_5_48_486 "[VP] HandleListAppendData need_item_data_ptr is null"
#define MMIVP_WINTABLE_9356_112_2_18_3_5_48_487 "[VP] HandleListAppendData need_item_content_ptr is null"
#define MMIVP_WINTABLE_9416_112_2_18_3_5_48_488 "[VP] GetBookmarkListItemContent param error"
#define MMIVP_WINTABLE_9452_112_2_18_3_5_48_489 "[VP] UpdateListData total_num = %d, max_num = %d"
#define MMIVP_WINTABLE_9541_112_2_18_3_5_48_490 "[VP] HandleListAppendData need_item_data_ptr is null"
#define MMIVP_WINTABLE_9554_112_2_18_3_5_49_491 "[VP] HandleListAppendData need_item_content_ptr is null"
#define MMIVP_WINTABLE_9594_112_2_18_3_5_49_492 "[VP] HandleBookmarkListWinMsg list_num = %d, list_index = %d"
#define MMIVP_WINTABLE_9603_112_2_18_3_5_49_493 "[VP] HandleBookmarkListWinMsg bookmark_item_ptr->title_arr = %x"
#define MMIVP_WINTABLE_9646_112_2_18_3_5_49_494 "[VP] GetHistoryListItemContent param is error"
#define MMIVP_WINTABLE_9704_112_2_18_3_5_49_495 "[VP] PlayCurrentHistoryVIdeo history_info_ptr = %x,  is_query_ptr = %x"
#define MMIVP_WINTABLE_9710_112_2_18_3_5_49_496 "[VP] PlayCurrentHistoryVIdeo history_info_ptr->last_play_point = %d, history_info_ptr->total_time = %x, history_info_ptr->type = %d"
#define MMIVP_WINTABLE_9729_112_2_18_3_5_49_497 "[VP] PlayCurrentHistoryVIdeo history_info_ptr->type = %d, process_time = %d"
#define MMIVP_WINTABLE_9735_112_2_18_3_5_49_498 "[VP] PlayCurrentHistoryVIdeo net info pnull"
#define MMIVP_WINTABLE_9871_112_2_18_3_5_49_499 "[VP] HandleHistoryListWinMsg need_item_data_ptr is null"
#define MMIVP_WINTABLE_9884_112_2_18_3_5_49_500 "[VP] HandleHistoryListWinMsg need_item_content_ptr is null"
#define MMIVP_WINTABLE_10083_112_2_18_3_5_50_501 "[VP] HandleShortcutMenuWinMsg menu_id = %x"
#define MMIVP_WINTABLE_10416_112_2_18_3_5_50_502 "[VP] GetLocalListItemContent param is error"
#define MMIVP_WINTABLE_10605_112_2_18_3_5_51_503 "[VP] HandleLocalListWinMsg need_item_data_ptr is null"
#define MMIVP_WINTABLE_10618_112_2_18_3_5_51_504 "[VP] HandleLocalListWinMsg need_item_content_ptr is null"
#define MMIVP_WINTABLE_10776_112_2_18_3_5_51_505 "[VP] HandleLocalListWinMsg need_item_data_ptr is null"
#define MMIVP_WINTABLE_10789_112_2_18_3_5_51_506 "[VP] HandleLocalListWinMsg need_item_content_ptr is null"
#define MMIVP_WINTABLE_10974_112_2_18_3_5_51_507 "[VP] HandleLocalListLongOkOptMenuWinMsg delete FAIL"
#define MMIVP_WINTABLE_11357_112_2_18_3_5_52_508 "[VP] HandleAddBookmarkWinMsg msg_id = %x"
#define MMIVP_WINTABLE_11443_112_2_18_3_5_52_509 "[VP] bookmark_data.title_arr = %x, list_index = %d"
#define MMIVP_WINTABLE_11579_112_2_18_3_5_53_510 "[VP] HandleBookmarkOptMenuWinMsg menu_id = %x"
#define MMIVP_WINTABLE_11610_112_2_18_3_5_53_511 "[VP] HandleBookmarkOptMenuWinMsg cur_list_index = %d"
#define MMIVP_WINTABLE_11623_112_2_18_3_5_53_512 "[VP] HandleBookmarkOptMenuWinMsg no memory"
#define MMIVP_WINTABLE_11663_112_2_18_3_5_53_513 "[VP] HandleBookmarkOptMenuWinMsg menu_id = %x"
#define MMIVP_WINTABLE_11730_112_2_18_3_5_53_514 "[VP] HandleHistoryOptMenuWinMsg msg_id = %x"
#define MMIVP_WINTABLE_11755_112_2_18_3_5_53_515 "[VP] HandleHistoryOptMenuWinMsg menu_id = %x"
#define MMIVP_WINTABLE_11800_112_2_18_3_5_53_516 "[VP] HandleHistoryOptMenuWinMsg no memory"
#define MMIVP_WINTABLE_11864_112_2_18_3_5_53_517 "[VP] HandleHistoryOptMenuWinMsg menu_id = %x"
#define MMIVP_WINTABLE_11920_112_2_18_3_5_53_518 "[VP] AddLocalListCallBack locallist_type = %d, list_index = %d, is_success = %d"
#define MMIVP_WINTABLE_11995_112_2_18_3_5_53_519 "[VP] HandleLocalListOptMenuWinMsg msg_id = %x, locallist_type = %d"
#define MMIVP_WINTABLE_12050_112_2_18_3_5_54_520 "[VP] HandleLocalListOptMenuWinMsg menu_id = %x"
#define MMIVP_WINTABLE_12068_112_2_18_3_5_54_521 "[VP] HandleLocalListOptMenuWinMsg cur_list_index = %d, locallist_type = %d"
#define MMIVP_WINTABLE_12103_112_2_18_3_5_54_522 "[VP] HandleLocalListOptMenuWinMsg get file info error"
#define MMIVP_WINTABLE_12114_112_2_18_3_5_54_523 "[VP] HandleLocalListOptMenuWinMsg cur_list_index = %d, locallist_type = %d"
#define MMIVP_WINTABLE_12197_112_2_18_3_5_54_524 "[VP] HandleLocalListOptMenuWinMsg delete FAIL"
#define MMIVP_WINTABLE_12413_112_2_18_3_5_54_525 "[VP] SettingAsScreensaver full_name_ptr = %x, full_name_len = %d"
#define MMIVP_WINTABLE_12462_112_2_18_3_5_54_526 "[VP] SettingAsVideoWallpaper full_name_ptr = %x, full_name_len = %d"
#define MMIVP_WINTABLE_12525_112_2_18_3_5_55_527 "[VP] SettingAsPowerOnOffAnimation is_power_on = %d, full_name_ptr = %x,full_name_len = %d"
#define MMIVP_WINTABLE_12593_112_2_18_3_5_55_528 "[VP] HandleFileOptionWinMsg menu_id = %x"
#define MMIVP_WINTABLE_12664_112_2_18_3_5_55_529 "[VP] MMIAPIVP_IsLiving result = %d"
#define MMIVP_WINTABLE_12711_112_2_18_3_5_55_530 "[VP] DisplayVPWinSoftkey s_video_player_control_info.total_time = %d"
#define MMIVP_WINTABLE_13070_112_2_18_3_5_56_531 "[VP] HandleSIMSelectWinMsg msg_id= 0x%x"
#define MMIVP_WINTABLE_13163_112_2_18_3_5_56_532 "[VP] HandleTPDownMsg x= %d, y = %d, rect.left = %d,right = %d,top = %d,bottom = %d, s_tp_down_ctrl_id = %d"
#define MMIVP_WINTABLE_13168_112_2_18_3_5_56_533 "[VP] HandleTPDownMsg s_tp_down_ctrl_id = %d"
#define MMIVP_WINTABLE_13191_112_2_18_3_5_56_534 "[VP] HandleTPUpMsg s_tp_down_ctrl_id = %d, point.x = %d, y= %d"
#define MMIVP_WINTABLE_13250_112_2_18_3_5_56_535 "[VP] HandleCtrlTpLongMsg ctrl_id = %d, s_tp_down_ctrl_id = %D"
#define MMIVP_WINTABLE_13319_112_2_18_3_5_56_536 "[VP] HandleCtrlTpMsg ctrl_id = %d, s_tp_down_ctrl_id = %d"
#define MMIVP_WINTABLE_13414_112_2_18_3_5_56_537 "[VP] HandleTPMsg msg_id = %x"
#define MMIVP_WINTABLE_13482_112_2_18_3_5_56_538 "[VP] MMIAPIVP_FillRectWithTransparenceColor before x= %d, y=%d,width = %d, height = %d"
#define MMIVP_WINTABLE_13495_112_2_18_3_5_56_539 "[VP] MMIAPIVP_FillRectWithTransparenceColor after x= %d, y=%d,width = %d, height = %d"
#define MMIVP_WINTABLE_13554_112_2_18_3_5_57_540 "[VP] MMIAPIVP_FillRectWithTransparenceColor after x= %d, y=%d,width = %d, height = %d"
#define MMIVP_WINTABLE_13625_112_2_18_3_5_57_541 "[VP] VP_BTEventNext enter"
#define MMIVP_WINTABLE_13636_112_2_18_3_5_57_542 "[VP] VP_BTEventPrev enter"
#define MMIVP_WINTABLE_13647_112_2_18_3_5_57_543 "[VP] VP_BTEventPlayOrPause enter"
#define MMIVP_WINTABLE_13660_112_2_18_3_5_57_544 "[VP] MMIAPIVP_HandleFileChange s_video_player_control_info.play_type = %d"
#define MMIVP_WINTABLE_13683_112_2_18_3_5_57_545 "[VP]MMIAPIVP_GetPlayingFilename playingname = %x"
#define MMIVP_WINTABLE_13702_112_2_18_3_5_57_546 "[VP]:CreateLandscapeThumbnailLayer "
#define MMIVP_WINTABLE_13726_112_2_18_3_5_57_547 "[VP]:CreateLandscapeThumbnailLayer success "
#define MMIVP_WINTABLE_13749_112_2_18_3_5_57_548 "[VP]:DestroyLandscapeThumbnailLayer  "
#define MMIVP_WINTABLE_13768_112_2_18_3_5_57_549 "[VP]:VPThumbnailResetMainLayer  "
#define MMIVP_WINTABLE_13784_112_2_18_3_5_57_550 "[VP]:VPThumbnailSetMainLayer  "
#define MMIVP_WINTABLE_13804_112_2_18_3_5_57_551 "[VP]:VPThumbnailSetDPLAYERParam   RotateAngle:%d,s_video_player_control_info.cur_angle:%d"
#define MMIVP_WINTABLE_13857_112_2_18_3_5_57_552 "[VP]:DisplayThumbnail MMIAPIVP_ThumbnailGetAngle !=GUILCD_GetLogicAngle "
#define MMIVP_WINTABLE_13868_112_2_18_3_5_57_553 "[VP]:DisplayThumbnail   image_data->coordinate.w:%d,image_data->coordinate.h:%d,image_data->coordinate.trim_x:%d,image_data->coordinate.trim_y:%d,image_data->coordinate.trim_w:%d,image_data->coordinate.trim_h:%d ,rotate:%d"
#define MMIVP_WINTABLE_13942_112_2_18_3_5_57_554 "[VP]:NotifyDPLAYERGetTumbnail   RotateAngle:%d,s_video_player_control_info.cur_angle:%d"
#define MMIVP_WINTABLE_13973_112_2_18_3_5_57_555 "[VP]:NotifyDPLAYERGetTumbnail   fail !"
#define MMIVP_WINTABLE_14034_112_2_18_3_5_58_556 "[VP] HandleBookmarkLongOkOptMenuWinMsg menu_id = %x"
#define MMIVP_WINTABLE_14039_112_2_18_3_5_58_557 "[VP] HandleBookmarkLongOkOptMenuWinMsg cur_list_index = %d"
#define MMIVP_WINTABLE_14052_112_2_18_3_5_58_558 "[VP] HandleBookmarkLongOkOptMenuWinMsg no memory"
#define MMIVP_WINTABLE_14092_112_2_18_3_5_58_559 "[VP] HandleBookmarkLongOkOptMenuWinMsg menu_id = %x"
#define MMIVP_WINTABLE_14162_112_2_18_3_5_58_560 "[VP] HandleHistoryLongOkOptMenuWinMsg msg_id = %x"
#define MMIVP_WINTABLE_14191_112_2_18_3_5_58_561 "[VP] HandleHistoryLongOkOptMenuWinMsg menu_id = %x"
#define MMIVP_WINTABLE_14211_112_2_18_3_5_58_562 "[VP] HandleHistoryLongOkOptMenuWinMsg no memory"
#define MMIVP_WINTABLE_14275_112_2_18_3_5_58_563 "[VP] HandleHistoryLongOkOptMenuWinMsg menu_id = %x"
#define MMIVP_WINTABLE_14360_112_2_18_3_5_58_564 "[VP] HandleLocalListLongOkOptMenuWinMsg msg_id = %x, locallist_type = %d"
#define MMIVP_WINTABLE_14406_112_2_18_3_5_58_565 "[VP] HandleLocalListLongOkOptMenuWinMsg menu_id = %x"
#define MMIVP_WINTABLE_14411_112_2_18_3_5_58_566 "[VP] HandleLocalListLongOkOptMenuWinMsg cur_list_index = %d, locallist_type = %d"
#define MMIVP_WINTABLE_14445_112_2_18_3_5_58_567 "[VP] HandleLocalListLongOkOptMenuWinMsg get file info error"
#define MMIVP_WINTABLE_14456_112_2_18_3_5_58_568 "[VP] HandleLocalListLongOkOptMenuWinMsg cur_list_index = %d, locallist_type = %d"
#define MMIVP_WINTABLE_14539_112_2_18_3_5_59_569 "[VP] HandleLocalListLongOkOptMenuWinMsg delete FAIL"
#define MMIVP_WINTABLE_14668_112_2_18_3_5_59_570 "[VP]:NetLinkButtonCallback"
#define MMIVP_WINTABLE_14701_112_2_18_3_5_59_571 "[VP]:NetLinkButtonCallback"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_VIDEO_PLAYER_TRC)
TRACE_MSG(MMIVP_API_57_112_2_18_3_5_0_0,"[VP] SdpFile_otacalback param_ptr->mime_type = %d")
TRACE_MSG(MMIVP_BOOKMARKLIST_116_112_2_18_3_5_1_1,"[VP] SaveBookmarklistToFile write bookmark successfully")
TRACE_MSG(MMIVP_BOOKMARKLIST_139_112_2_18_3_5_1_2,"[VP] SaveBookmarklistToFile bookmarklist is null")
TRACE_MSG(MMIVP_BOOKMARKLIST_144_112_2_18_3_5_1_3,"[VP] SaveBookmarklistToFile save_result = %d")
TRACE_MSG(MMIVP_BOOKMARKLIST_171_112_2_18_3_5_1_4,"[VP] GetBookmarkListFileNameForRead result = %d")
TRACE_MSG(MMIVP_BOOKMARKLIST_200_112_2_18_3_5_1_5,"[VP] GetBookmarkListFileNameForWrite result = %d")
TRACE_MSG(MMIVP_BOOKMARKLIST_237_112_2_18_3_5_1_6,"[VP] VP_BookmarkList_Load readlist_result = %d")
TRACE_MSG(MMIVP_BOOKMARKLIST_247_112_2_18_3_5_1_7,"[VP] VP_BookmarkList_Load s_visit_bookmark_que_ptr = %x")
TRACE_MSG(MMIVP_BOOKMARKLIST_314_112_2_18_3_5_1_8,"[VP] VP_BookmarkList_GetNum total_bookmark_num = %d")
TRACE_MSG(MMIVP_BOOKMARKLIST_325_112_2_18_3_5_1_9,"[VP] VP_BookmarkList_GetInfo list_index = %d")
TRACE_MSG(MMIVP_BOOKMARKLIST_329_112_2_18_3_5_1_10,"[VP] VP_BookmarkList_GetInfo param error")
TRACE_MSG(MMIVP_BOOKMARKLIST_345_112_2_18_3_5_1_11,"[VP] VP_BookmarkList_DelItem index = %d")
TRACE_MSG(MMIVP_BOOKMARKLIST_364_112_2_18_3_5_1_12,"[VP] VP_BookmarkList_DelItem result = %d, s_visit_bookmark_que_ptr->item_count = %d")
TRACE_MSG(MMIVP_BOOKMARKLIST_399_112_2_18_3_5_2_13,"[VP] VP_BookmarkList_AddItem list too much bookmark ")
TRACE_MSG(MMIVP_BOOKMARKLIST_420_112_2_18_3_5_2_14,"[VP] VP_BookmarkList_AddItem index = %d, count = %d ")
TRACE_MSG(MMIVP_BOOKMARKLIST_429_112_2_18_3_5_2_15,"[VP] VP_BookmarkList_AddItem add_result = %d")
TRACE_MSG(MMIVP_BOOKMARKLIST_444_112_2_18_3_5_2_16,"[VP] VP_BookmarkList_ModifyItem  index is worng list_index = %d ")
TRACE_MSG(MMIVP_BOOKMARKLIST_455_112_2_18_3_5_2_17,"[VP] VP_BookmarkList_ModifyItem modify_result = %d")
TRACE_MSG(MMIVP_FSM_539_112_2_18_3_5_4_18,"IsVideoPlayerCollision ret = %d")
TRACE_MSG(MMIVP_FSM_563_112_2_18_3_5_4_19,"[VP] CreatMsg play_param_ptr->play_type = %d, msg_body_size = %d")
TRACE_MSG(MMIVP_FSM_570_112_2_18_3_5_4_20,"[VP] CreatMsg full path ptr is null")
TRACE_MSG(MMIVP_FSM_579_112_2_18_3_5_4_21,"[VP] CreatMsg buffer is null")
TRACE_MSG(MMIVP_FSM_588_112_2_18_3_5_4_22,"[VP] CreatMsg url is null")
TRACE_MSG(MMIVP_FSM_591_112_2_18_3_5_4_23,"[VP] CreatMsg msg_body_size = %d")
TRACE_MSG(MMIVP_FSM_593_112_2_18_3_5_4_24,"[VP] CreatMsg msg_body_size = %d")
TRACE_MSG(MMIVP_FSM_607_112_2_18_3_5_4_25,"[VP] msg_ptr = %x, len = %d")
TRACE_MSG(MMIVP_FSM_619_112_2_18_3_5_4_26,"apple play_param2_ptr = %x, msg_body_size =%d")
TRACE_MSG(MMIVP_FSM_632_112_2_18_3_5_4_27,"[VP] CreatMsg play_param2_ptr->url_ptr = %x, len = %x")
TRACE_MSG(MMIVP_FSM_636_112_2_18_3_5_4_28,"apple play_param2_ptr->full_path_ptr = %x")
TRACE_MSG(MMIVP_FSM_638_112_2_18_3_5_4_29,"apple play_param2_ptr->full_path_ptr = %x")
TRACE_MSG(MMIVP_FSM_639_112_2_18_3_5_4_30,"apple play_param_ptr->full_path_ptr = %x, play_param_ptr->full_path_len = %d")
TRACE_MSG(MMIVP_FSM_641_112_2_18_3_5_4_31,"apple play_param2_ptr->full_path_ptr = %x")
TRACE_MSG(MMIVP_FSM_724_112_2_18_3_5_4_32,"[VP] CreatMsg frame height = %d, rate = %d, base = %d,width = %d, cfg_len = %d")
TRACE_MSG(MMIVP_FSM_741_112_2_18_3_5_4_33,"[VP] CreatMsg strm_init_info_ptr->sdp_info.video_param.config_len = %d")
TRACE_MSG(MMIVP_FSM_761_112_2_18_3_5_4_34,"[VP] CreatMsg sprop_param_sets_num = %d, enc_type = %d, sprop_sets_arr = %x")
TRACE_MSG(MMIVP_FSM_969_112_2_18_3_5_4_35,"[VP] CreateFSM there is a fsm")
TRACE_MSG(MMIVP_FSM_977_112_2_18_3_5_5_36,"[VP] CreateFSM no space for fsm")
TRACE_MSG(MMIVP_FSM_1015_112_2_18_3_5_5_37,"[VP] CreateFSM cur fsm is %x")
TRACE_MSG(MMIVP_FSM_1038_112_2_18_3_5_5_38,"[VP] CreateFSM cur fsm is %x")
TRACE_MSG(MMIVP_FSM_1066_112_2_18_3_5_5_39,"[VP] DestroyFSM there is no fsm list")
TRACE_MSG(MMIVP_FSM_1074_112_2_18_3_5_5_40,"[VP] DestroyFSM current_fsm_ptr = %x, current_fsm_ptr->fsm_info_ptr = %x")
TRACE_MSG(MMIVP_FSM_1130_112_2_18_3_5_5_41,"[VP] DestroyFSM  fsm vp_handle = %x")
TRACE_MSG(MMIVP_FSM_1179_112_2_18_3_5_5_42,"[VP] FsmDispatch something wrong with fsm_ptr = %x")
TRACE_MSG(MMIVP_FSM_1199_112_2_18_3_5_5_43,"[VP] FsmStateTrans fsm_ptr = %x, state = %d")
TRACE_MSG(MMIVP_FSM_1227_112_2_18_3_5_5_44,"[VP] SendPrepareIndToUI param is error")
TRACE_MSG(MMIVP_FSM_1243_112_2_18_3_5_5_45,"[VP] SendPrepareIndToUI send prepare ind to ui")
TRACE_MSG(MMIVP_FSM_1260_112_2_18_3_5_5_46,"[VP] SendAUTHIndToUI param is error")
TRACE_MSG(MMIVP_FSM_1269_112_2_18_3_5_5_47,"[VP] SendAUTHIndToUI send auth ind to ui")
TRACE_MSG(MMIVP_FSM_1286_112_2_18_3_5_5_48,"[VP] SendFatalErrorIndTOUI param is error")
TRACE_MSG(MMIVP_FSM_1296_112_2_18_3_5_5_49,"[VP] SendFatalErrorIndTOUI send fatal error to ui error_code = %d")
TRACE_MSG(MMIVP_FSM_1313_112_2_18_3_5_5_50,"[VP] SendProcessIndToUI param is error")
TRACE_MSG(MMIVP_FSM_1340_112_2_18_3_5_5_51,"[VP] SendBufferingIndToUI param is error")
TRACE_MSG(MMIVP_FSM_1349_112_2_18_3_5_5_52,"[VP] SendBufferingIndToUI send VP_BUFFERING_IND to ui")
TRACE_MSG(MMIVP_FSM_1367_112_2_18_3_5_5_53,"[VP] SendBufferEndIndToUI param is error")
TRACE_MSG(MMIVP_FSM_1377_112_2_18_3_5_5_54,"[VP] SendAUTHIndToUI send VP_BUFFER_END_IND to ui")
TRACE_MSG(MMIVP_FSM_1395_112_2_18_3_5_5_55,"[VP] SendBufferEndIndToUI param is error")
TRACE_MSG(MMIVP_FSM_1404_112_2_18_3_5_5_56,"[VP] SendAUTHIndToUI send VP_BUFFER_END_IND to ui")
TRACE_MSG(MMIVP_FSM_1421_112_2_18_3_5_5_57,"[VP] SendPlayEndToUI param is error")
TRACE_MSG(MMIVP_FSM_1431_112_2_18_3_5_6_58,"[VP] SendAUTHIndToUI send VP_PLAY_END_IND to ui")
TRACE_MSG(MMIVP_FSM_1447_112_2_18_3_5_6_59,"[VP] SendStopIndToUI param is error")
TRACE_MSG(MMIVP_FSM_1462_112_2_18_3_5_6_60,"[VP] SendAUTHIndToUI send VP_STOP_IND to ui")
TRACE_MSG(MMIVP_FSM_1478_112_2_18_3_5_6_61,"[VP] SendPlayCnfToUI param is error")
TRACE_MSG(MMIVP_FSM_1488_112_2_18_3_5_6_62,"[VP] SendPlayCnfToUI send VP_PLAY_CNFto ui error_code = %d")
TRACE_MSG(MMIVP_FSM_1504_112_2_18_3_5_6_63,"[VP] SendPauseCnfToUI param is error")
TRACE_MSG(MMIVP_FSM_1514_112_2_18_3_5_6_64,"[VP] SendAUTHIndToUI send VP_PAUSE_CNF to ui")
TRACE_MSG(MMIVP_FSM_1530_112_2_18_3_5_6_65,"[VP] SendSeekCnfToUI param is error")
TRACE_MSG(MMIVP_FSM_1542_112_2_18_3_5_6_66,"[VP] SendAUTHIndToUI send VP_SEEK_CNF to ui")
TRACE_MSG(MMIVP_FSM_1576_112_2_18_3_5_6_67,"[VP]:GetFrameDataCallback enter!")
TRACE_MSG(MMIVP_FSM_1678_112_2_18_3_5_6_68,"[VP]FrameToTime handle = 0x%x, info_result = %d,frame_index = %d, time_offset = 0x%x, time_lenth = %d,base = %d")
TRACE_MSG(MMIVP_FSM_1701_112_2_18_3_5_6_69,"[VP] PlayLocalFile fsm is error")
TRACE_MSG(MMIVP_FSM_1709_112_2_18_3_5_6_70,"[VP] PlayLocalFile full_path_len = %d, full_path_ptr = %x, %x, %x, %x")
TRACE_MSG(MMIVP_FSM_1768_112_2_18_3_5_6_71,"[VP] PlayLocalFile fsm_ptr->dplayer_handle = %x")
TRACE_MSG(MMIVP_FSM_1793_112_2_18_3_5_6_72,"[VP] PlayLocalFile play_param_ptr->is_scal_up = %d, fsm_ptr->display_param.Width = %d, p_info.video_info.width = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d")
TRACE_MSG(MMIVP_FSM_1803_112_2_18_3_5_6_73,"[VP] PlayLocalFile total_time = %d, process_time = %d, support_type = %d")
TRACE_MSG(MMIVP_FSM_1823_112_2_18_3_5_6_74,"[VP] PlayLocalFile is_cr_video_layer")
TRACE_MSG(MMIVP_FSM_1837_112_2_18_3_5_6_75,"[VP] PlayLocalFile fsm_ptr->is_save_history = %d")
TRACE_MSG(MMIVP_FSM_1865_112_2_18_3_5_6_76,"[VP] PlayLocalFile play error")
TRACE_MSG(MMIVP_FSM_1913_112_2_18_3_5_6_77,"[VP] PlayLocalBuf param is error")
TRACE_MSG(MMIVP_FSM_1923_112_2_18_3_5_7_78,"[VP] PlayLocalBuf play_param_ptr->video_buf_info.type = %d")
TRACE_MSG(MMIVP_FSM_1952_112_2_18_3_5_7_79,"[VP] PlayLocalBuf fsm_ptr->dplayer_handle = %x")
TRACE_MSG(MMIVP_FSM_1967_112_2_18_3_5_7_80,"[VP] PlayLocalBuf vidot_type = %d, vidoe_support = %d")
TRACE_MSG(MMIVP_FSM_1972_112_2_18_3_5_7_81,"[VP] PlayLocalBuf play_param_ptr->is_scal_up = %d, fsm_ptr->display_param.Width = %d, p_info.video_info.width = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d")
TRACE_MSG(MMIVP_FSM_1984_112_2_18_3_5_7_82,"[VP] PlayLocalBuf total_time = %d, process_time = %d, support_type = %d")
TRACE_MSG(MMIVP_FSM_2008_112_2_18_3_5_7_83,"[VP] PlayLocalBuf play success")
TRACE_MSG(MMIVP_FSM_2016_112_2_18_3_5_7_84,"[VP] PlayLocalBuf play error")
TRACE_MSG(MMIVP_FSM_2048_112_2_18_3_5_7_85,"[VP] PlayFromNet param is error")
TRACE_MSG(MMIVP_FSM_2061_112_2_18_3_5_7_86,"[VP] PlayFromNet fsm_ptr->dplayer_handle = %x")
TRACE_MSG(MMIVP_FSM_2089_112_2_18_3_5_7_87,"[VP] PlayFromNet play_param_ptr->is_scal_up = %d, fsm_ptr->display_param.Width = %d, p_info.video_info.width = %d, fsm_ptr->display_param.Height = %d, p_info.video_info.height = %d")
TRACE_MSG(MMIVP_FSM_2101_112_2_18_3_5_7_88,"[VP] PlayFromNet total_time = %d, process_time = %d, support_type = %d")
TRACE_MSG(MMIVP_FSM_2128_112_2_18_3_5_7_89,"[VP] PlayFromNet play error")
TRACE_MSG(MMIVP_FSM_2196_112_2_18_3_5_7_90,"[VP] HandlePDPMsg param is error")
TRACE_MSG(MMIVP_FSM_2200_112_2_18_3_5_7_91,"[VP] HandlePDPMsg msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_2240_112_2_18_3_5_7_92,"[VP] ActivePdp param is error")
TRACE_MSG(MMIVP_FSM_2245_112_2_18_3_5_7_93,"[VP] ActivePdp dual_sys = %d")
TRACE_MSG(MMIVP_FSM_2253_112_2_18_3_5_7_94,"[VP] ActivePdp sim casd is not ok dual_sys = %d")
TRACE_MSG(MMIVP_FSM_2262_112_2_18_3_5_7_95,"[VP] ActivePdp param is setting_index = %d")
TRACE_MSG(MMIVP_FSM_2285_112_2_18_3_5_7_96,"[VP] ActivePdp sim casd is not ok dual_sys = %d")
TRACE_MSG(MMIVP_FSM_2317_112_2_18_3_5_7_97,"[VP] FsmIdle param is error")
TRACE_MSG(MMIVP_FSM_2321_112_2_18_3_5_7_98,"[VP] FsmIdle msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_2329_112_2_18_3_5_7_99,"IDLE, got wrong msg")
TRACE_MSG(MMIVP_FSM_2350_112_2_18_3_5_7_100,"[VP] InitFsmInfo param is null")
TRACE_MSG(MMIVP_FSM_2353_112_2_18_3_5_7_101,"[VP] InitFsmInfo state = %d, vpcallback = %x")
TRACE_MSG(MMIVP_FSM_2391_112_2_18_3_5_7_102,"[VP] UpdateFsmInfo param is null")
TRACE_MSG(MMIVP_FSM_2436_112_2_18_3_5_8_103,"[VP] UpdateFsmInfo fsm_ptr->full_path_ptr = %x, play_param_ptr->full_path_ptr = %x")
TRACE_MSG(MMIVP_FSM_2500_112_2_18_3_5_8_104,"[VP] PlayNextFile param is null")
TRACE_MSG(MMIVP_FSM_2503_112_2_18_3_5_8_105,"[VP] PlayNextFile i_type = %d, fsm_ptr->mode = %d")
TRACE_MSG(MMIVP_FSM_2571_112_2_18_3_5_8_106,"[VP] PlayNextFile result = %d")
TRACE_MSG(MMIVP_FSM_2594_112_2_18_3_5_8_107,"[VP] FsmReady play param is null")
TRACE_MSG(MMIVP_FSM_2598_112_2_18_3_5_8_108,"[VP] FsmReady msg_ptr->msg_id = %x, fsm_ptr->has_play_end_reason = %d, fsm_ptr->is_to_exit = %d")
TRACE_MSG(MMIVP_FSM_2621_112_2_18_3_5_8_109,"[VP] FsmReady play_param_ptr->play_type = %d")
TRACE_MSG(MMIVP_FSM_2728_112_2_18_3_5_8_110,"[VP] FsmReady ready, got wrong msg")
TRACE_MSG(MMIVP_FSM_2750_112_2_18_3_5_8_111,"[VP] InitStreaming param is error")
TRACE_MSG(MMIVP_FSM_2863_112_2_18_3_5_8_112,"[VP] FsmPdpActiving fsm or msg error")
TRACE_MSG(MMIVP_FSM_2867_112_2_18_3_5_8_113,"[VP] FsmPdpActiving msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_2875_112_2_18_3_5_8_114,"[VP] FsmPdpActiving pdp active error")
TRACE_MSG(MMIVP_FSM_2931_112_2_18_3_5_9_115,"[VP] FsmInit param is error")
TRACE_MSG(MMIVP_FSM_2935_112_2_18_3_5_9_116,"[VP] FsmInit msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_2956_112_2_18_3_5_9_117,"[VP] PlayFromNet play error")
TRACE_MSG(MMIVP_FSM_3013_112_2_18_3_5_9_118,"[VP] FsmInit param is error")
TRACE_MSG(MMIVP_FSM_3017_112_2_18_3_5_9_119,"[VP] FsmInit msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_3022_112_2_18_3_5_9_120,"[VP] FsmInit str_ini_result = %d, is_success_play = %d")
TRACE_MSG(MMIVP_FSM_3133_112_2_18_3_5_9_121,"[VP] FsmInitAuth param is error")
TRACE_MSG(MMIVP_FSM_3137_112_2_18_3_5_9_122,"[VP] FsmInitAuth msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_3203_112_2_18_3_5_9_123,"[VP] FsmPlayPeding param is error")
TRACE_MSG(MMIVP_FSM_3207_112_2_18_3_5_9_124,"[VP] FsmPlayPeding msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_3212_112_2_18_3_5_9_125,"[VP] FsmPlayPeding strm_result = %d")
TRACE_MSG(MMIVP_FSM_3332_112_2_18_3_5_10_126,"[VP] FsmPlayAuth param is error")
TRACE_MSG(MMIVP_FSM_3337_112_2_18_3_5_10_127,"[VP] FsmPlayAuth msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_3353_112_2_18_3_5_10_128,"[VP] FsmPlayAuth auth_info.password_ptr= %x, auth_info.username_ptr = %x")
TRACE_MSG(MMIVP_FSM_3409_112_2_18_3_5_10_129,"[VP] FsmPlaying param is error")
TRACE_MSG(MMIVP_FSM_3418_112_2_18_3_5_10_130,"[VP] FsmPlaying VP_ENTRY_REQ fsm_ptr->is_to_pause %x,fsm_ptr->has_play_end_reason = %d")
TRACE_MSG(MMIVP_FSM_3443_112_2_18_3_5_10_131,"[VP] FsmPlaying VP_STRM_BUFFERING_IND")
TRACE_MSG(MMIVP_FSM_3450_112_2_18_3_5_10_132,"[VP] FsmPlaying VP_CALLBACK_END_IND")
TRACE_MSG(MMIVP_FSM_3466_112_2_18_3_5_10_133,"[VP] FsmPlaying fsm_ptr->play_type = %d , fsm_ptr->mode = %d, fsm_ptr->is_random_play = %d")
TRACE_MSG(MMIVP_FSM_3582_112_2_18_3_5_10_134,"[VP] FsmPlaying VP_PAUSE_REQ, fsm_ptr->is_not_notify_ui = %d, fsm_ptr->is_to_pause = %d")
TRACE_MSG(MMIVP_FSM_3619_112_2_18_3_5_10_135,"[VP] FsmPlaying VP_SEEK_REQ")
TRACE_MSG(MMIVP_FSM_3657_112_2_18_3_5_10_136,"[VP] FsmPlaying VP_STOP_REQ")
TRACE_MSG(MMIVP_FSM_3673_112_2_18_3_5_10_137,"[VP] FsmPlaying VP_STRM_DATA_TIMEOUT_IND")
TRACE_MSG(MMIVP_FSM_3682_112_2_18_3_5_10_138,"[VP] FsmPlaying VP_RESUME_REQ")
TRACE_MSG(MMIVP_FSM_3724_112_2_18_3_5_10_139,"[VP] FsmBuffering param is error")
TRACE_MSG(MMIVP_FSM_3729_112_2_18_3_5_10_140,"[VP] FsmBuffering msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_3865_112_2_18_3_5_11_141,"[VP] FsmSeeking param is error")
TRACE_MSG(MMIVP_FSM_3869_112_2_18_3_5_11_142,"[VP] FsmSeeking  msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_3882_112_2_18_3_5_11_143,"[VP] FsmSeeking strm_result = %d")
TRACE_MSG(MMIVP_FSM_3965_112_2_18_3_5_11_144,"[VP] FsmPausing param is error")
TRACE_MSG(MMIVP_FSM_3969_112_2_18_3_5_11_145,"[VP] FsmPausing msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_3982_112_2_18_3_5_11_146,"[VP] FsmPausing strm_result = %d, fsm_ptr->is_not_notify_ui = %d")
TRACE_MSG(MMIVP_FSM_3991_112_2_18_3_5_11_147,"[VP] FsmPausing send pause cnf")
TRACE_MSG(MMIVP_FSM_4057_112_2_18_3_5_11_148,"[VP] FsmPaused param is error")
TRACE_MSG(MMIVP_FSM_4061_112_2_18_3_5_11_149,"[VP] FsmPaused msg_ptr->msg_id = %x, is_to_pause = %d, is_to_resume = %d")
TRACE_MSG(MMIVP_FSM_4109_112_2_18_3_5_11_150,"[VP] FsmPaused VP_SEEK_REQ")
TRACE_MSG(MMIVP_FSM_4218_112_2_18_3_5_11_151,"[VP] FsmPausedSeeking param is error")
TRACE_MSG(MMIVP_FSM_4222_112_2_18_3_5_11_152,"[VP] FsmPausedSeeking  msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_4244_112_2_18_3_5_11_153,"[VP] FsmSeeking strm_result = %d")
TRACE_MSG(MMIVP_FSM_4325_112_2_18_3_5_12_154,"[VP] FsmPaused param is error")
TRACE_MSG(MMIVP_FSM_4329_112_2_18_3_5_12_155,"[VP] FsmPaused msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_4402_112_2_18_3_5_12_156,"[VP] FsmResuming msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_4414_112_2_18_3_5_12_157,"[VP] FsmResuming strm_result = %d")
TRACE_MSG(MMIVP_FSM_4490_112_2_18_3_5_12_158,"[VP] HandleInitCnf param is null")
TRACE_MSG(MMIVP_FSM_4495_112_2_18_3_5_12_159,"[VP] HandleInitCnf strm_init_ptr->result = %d")
TRACE_MSG(MMIVP_FSM_4584_112_2_18_3_5_12_160,"[VP] HandleInitCnf frame height = %d, rate = %d, base = %d,width = %d, cfg_len = %d, fsm_ptr->total_time= %d, strm_cfg.video_cfg.cfg_ptr = %x")
TRACE_MSG(MMIVP_FSM_4627_112_2_18_3_5_12_161,"[VP]:HandleGetFrameData no video_layer!!!!!!!!!")
TRACE_MSG(MMIVP_FSM_4653_112_2_18_3_5_12_162,"[VP] HandleUpdateFrameReq param is null")
TRACE_MSG(MMIVP_FSM_4670_112_2_18_3_5_12_163,"[VP]:GetFrameDataCallback height=%d,width=%d!")
TRACE_MSG(MMIVP_FSM_4681_112_2_18_3_5_12_164,"[VP]HandleUpdateFrameReq,ERROR BUFFER1!!")
TRACE_MSG(MMIVP_FSM_4700_112_2_18_3_5_12_165,"[VP]:GetFrameDataCallback l_height=%d,l_width=%d!")
TRACE_MSG(MMIVP_FSM_4706_112_2_18_3_5_12_166,"[VP]HandleUpdateFrameReq,ERROR BUFFER2!!")
TRACE_MSG(MMIVP_FSM_4746_112_2_18_3_5_13_167,"[VP]:HandleUpdateFrameReq ERROR!!!!! height=%d,width=%d,layer_height=%d,layer_width=%d")
TRACE_MSG(MMIVP_FSM_4752_112_2_18_3_5_13_168,"[VP]:HandleUpdateFrameReq DPLAYER_UpdateNextFrame")
TRACE_MSG(MMIVP_FSM_4767_112_2_18_3_5_13_169,"[VP] HandlePauseReq param is null")
TRACE_MSG(MMIVP_FSM_4789_112_2_18_3_5_13_170,"[VP] HandlePauseReq can not pause, so stop")
TRACE_MSG(MMIVP_FSM_4812_112_2_18_3_5_13_171,"[VP] HandleResumeReq param is null")
TRACE_MSG(MMIVP_FSM_4844_112_2_18_3_5_13_172,"[VP] HandleSeekReq param is null")
TRACE_MSG(MMIVP_FSM_4849_112_2_18_3_5_13_173,"[VP] FsmPlaying seek_time = %x, seek_param->seek_frame = %x")
TRACE_MSG(MMIVP_FSM_4876_112_2_18_3_5_13_174,"[VP] HandleSeekReq streaming see error")
TRACE_MSG(MMIVP_FSM_4890_112_2_18_3_5_13_175,"[VP] HandleStopReq param is null")
TRACE_MSG(MMIVP_FSM_4893_112_2_18_3_5_13_176,"[VP] HandleStopReq is_begin_play = %d, fsm_ptr->is_save_history = %d")
TRACE_MSG(MMIVP_FSM_4939_112_2_18_3_5_13_177,"[VP] FsmStopping param is error")
TRACE_MSG(MMIVP_FSM_4943_112_2_18_3_5_13_178,"[VP] FsmStopping msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_4975_112_2_18_3_5_13_179,"[VP] FsmStopped param is error")
TRACE_MSG(MMIVP_FSM_5015_112_2_18_3_5_13_180,"[VP] FsmClosing param is error")
TRACE_MSG(MMIVP_FSM_5019_112_2_18_3_5_13_181,"[VP] FsmClosing msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_FSM_5068_112_2_18_3_5_13_182,"[VP] MMIVP_DispatchSignalToFsmMsg   APP_VP_ONE_FRAME_REARCH")
TRACE_MSG(MMIVP_FSM_5072_112_2_18_3_5_13_183,"[VP] MMIVP_DispatchSignalToFsmMsg   APP_VP_ONE_FRAME_END")
TRACE_MSG(MMIVP_FSM_5092_112_2_18_3_5_13_184,"[VP] MMIVP_DispatchSignalToFsmMsg STRM_INIT_CNF")
TRACE_MSG(MMIVP_FSM_5110_112_2_18_3_5_13_185,"[VP] MMIVP_DispatchSignalToFsmMsg STRM_PLAY_CNF")
TRACE_MSG(MMIVP_FSM_5126_112_2_18_3_5_13_186,"[VP] MMIVP_DispatchSignalToFsmMsg STRM_BUFFERING_IND")
TRACE_MSG(MMIVP_FSM_5134_112_2_18_3_5_13_187,"[VP] MMIVP_DispatchSignalToFsmMsg STRM_BUFFER_END_IND")
TRACE_MSG(MMIVP_FSM_5142_112_2_18_3_5_13_188,"[VP] MMIVP_DispatchSignalToFsmMsg STRM_DATA_TIMEOUT_IND")
TRACE_MSG(MMIVP_FSM_5151_112_2_18_3_5_13_189,"[VP] MMIVP_DispatchSignalToFsmMsg strm_buf_process_ptr = %x")
TRACE_MSG(MMIVP_FSM_5157_112_2_18_3_5_13_190,"[VP] MMIVP_DispatchSignalToFsmMsg process = %d, total = %d")
TRACE_MSG(MMIVP_FSM_5164_112_2_18_3_5_13_191,"[VP] MMIVP_DispatchSignalToFsmMsg percent = %d")
TRACE_MSG(MMIVP_FSM_5169_112_2_18_3_5_13_192,"[VP] MMIVP_DispatchSignalToFsmMsg STRM_GOODBYE_IND")
TRACE_MSG(MMIVP_FSM_5178_112_2_18_3_5_13_193,"[VP] MMIVP_DispatchSignalToFsmMsg STRM_SEEK_CNF")
TRACE_MSG(MMIVP_FSM_5196_112_2_18_3_5_13_194,"[VP] MMIVP_DispatchSignalToFsmMsg STRM_PAUSE_CNF")
TRACE_MSG(MMIVP_FSM_5214_112_2_18_3_5_14_195,"[VP] MMIVP_DispatchSignalToFsmMsg STRM_RESUME_CNF")
TRACE_MSG(MMIVP_FSM_5233_112_2_18_3_5_14_196,"[VP] MMIVP_DispatchSignalToFsmMsg STRM_STOP_CNF")
TRACE_MSG(MMIVP_FSM_5250_112_2_18_3_5_14_197,"[VP] MMIVP_DispatchSignalToFsmMsg STRM_CLOSE_CNF")
TRACE_MSG(MMIVP_FSM_5281_112_2_18_3_5_14_198,"[VP] MMIVP_DispatchSignalToFsmMsg APP_VP_END_IND")
TRACE_MSG(MMIVP_FSM_5295_112_2_18_3_5_14_199,"[VP] MMIVP_DispatchSignalToFsmMsg APP_VP_PLAY_NEXT_FILE_REQ")
TRACE_MSG(MMIVP_FSM_5331_112_2_18_3_5_14_200,"[VP] MMIAPIVP_FSMInit ui callback is null")
TRACE_MSG(MMIVP_FSM_5339_112_2_18_3_5_14_201,"[VP] MMIAPIVP_FSMInit create fsm fail")
TRACE_MSG(MMIVP_FSM_5342_112_2_18_3_5_14_202,"[VP ] MMIAPIVP_FSMInit create fsm handle = %x")
TRACE_MSG(MMIVP_FSM_5346_112_2_18_3_5_14_203,"[VP] MMIAPIVP_FSMInit dispath msg fail")
TRACE_MSG(MMIVP_FSM_5363_112_2_18_3_5_14_204,"[VP] MMIAPIVP_FSMRelease vp_handle = %x")
TRACE_MSG(MMIVP_FSM_5367_112_2_18_3_5_14_205,"[VP] MMIAPIVP_FSMRelease some wrong with vp_handle = %x")
TRACE_MSG(MMIVP_FSM_5394_112_2_18_3_5_14_206,"[VP] MMIAPIVP_FSMPlay some wrong with vp_handle = %x,play_param_ptr = %x")
TRACE_MSG(MMIVP_FSM_5406_112_2_18_3_5_14_207,"[VP] MMIAPIVP_FSMPlay begin")
TRACE_MSG(MMIVP_FSM_5409_112_2_18_3_5_14_208,"[VP] MMIAPIVP_FSMPlay result = %d")
TRACE_MSG(MMIVP_FSM_5430_112_2_18_3_5_14_209,"[VP] MMIAPIVP_FSMStop some wrong with vp_handle = %x")
TRACE_MSG(MMIVP_FSM_5436_112_2_18_3_5_14_210,"[VP] MMIAPIVP_FSMStop begin")
TRACE_MSG(MMIVP_FSM_5438_112_2_18_3_5_14_211,"[VP] MMIAPIVP_FSMStop result = %d")
TRACE_MSG(MMIVP_FSM_5455_112_2_18_3_5_14_212,"[VP] MMIAPIVP_FSMPause some wrong with vp_handle = %x")
TRACE_MSG(MMIVP_FSM_5458_112_2_18_3_5_14_213,"[VP] MMIAPIVP_FSMPause begin")
TRACE_MSG(MMIVP_FSM_5462_112_2_18_3_5_14_214,"[VP] MMIAPIVP_FSMPause result = %d")
TRACE_MSG(MMIVP_FSM_5483_112_2_18_3_5_14_215,"[VP] MMIAPIVP_FSMResume some wrong with vp_handle = %x")
TRACE_MSG(MMIVP_FSM_5486_112_2_18_3_5_14_216,"[VP] MMIAPIVP_FSMResume begin")
TRACE_MSG(MMIVP_FSM_5489_112_2_18_3_5_14_217,"[VP] MMIAPIVP_FSMResume result = %d")
TRACE_MSG(MMIVP_FSM_5511_112_2_18_3_5_14_218,"[VP] MMIAPIVP_FSMSeek some wrong with vp_handle = %x")
TRACE_MSG(MMIVP_FSM_5514_112_2_18_3_5_14_219,"[VP] MMIAPIVP_FSMSeek begin")
TRACE_MSG(MMIVP_FSM_5518_112_2_18_3_5_14_220,"[VP] MMIAPIVP_FSMSeek result = %d")
TRACE_MSG(MMIVP_FSM_5540_112_2_18_3_5_14_221,"[VP] VP_StopSetRate some wrong with vp_handle = %x")
TRACE_MSG(MMIVP_FSM_5543_112_2_18_3_5_14_222,"[VP] VP_StopSetRate begin")
TRACE_MSG(MMIVP_FSM_5547_112_2_18_3_5_14_223,"[VP] VP_StopSetRate result = %d")
TRACE_MSG(MMIVP_FSM_5565_112_2_18_3_5_14_224,"[VP] VP_StartSetRate some wrong with vp_handle = %x")
TRACE_MSG(MMIVP_FSM_5568_112_2_18_3_5_14_225,"[VP] VP_StartSetRate begin")
TRACE_MSG(MMIVP_FSM_5572_112_2_18_3_5_14_226,"[VP] VP_StartSetRate result = %d")
TRACE_MSG(MMIVP_FSM_5591_112_2_18_3_5_14_227,"[VP] VP_AuthRsp some wrong with vp_handle = %x")
TRACE_MSG(MMIVP_FSM_5597_112_2_18_3_5_14_228,"[VP] VP_AuthRsp result = %d")
TRACE_MSG(MMIVP_FSM_5618_112_2_18_3_5_14_229,"[VP] MMIAPIVP_FSMSwitchPlayDevice the fsm is wrong fsm_ptr = %x")
TRACE_MSG(MMIVP_FSM_5622_112_2_18_3_5_14_230,"[VP] MMIAPIVP_FSMSwitchPlayDevice is_bt_support = %d, is_a2dp_support = %d")
TRACE_MSG(MMIVP_FSM_5660_112_2_18_3_5_14_231,"[VP] MMIAPIVP_FSMSwitchDisplayMode the fsm is wrong fsm_ptr = %x")
TRACE_MSG(MMIVP_FSM_5665_112_2_18_3_5_14_232,"[VP] MMIAPIVP_FSMSwitchDisplayMode fsm_ptr->state = %d, is_only_rect = %d")
TRACE_MSG(MMIVP_FSM_5669_112_2_18_3_5_15_233,"[VP] MMIAPIVP_FSMSwitchDisplayMode lcd_id = %d, block_id = %d, x= %d")
TRACE_MSG(MMIVP_FSM_5701_112_2_18_3_5_15_234,"[VP] MMIAPIVP_FSMSwitchDisplayMode result = %d")
TRACE_MSG(MMIVP_FSM_5719_112_2_18_3_5_15_235,"[VP] MMIAPIVP_FSMGetState state = %d")
TRACE_MSG(MMIVP_FSM_5734_112_2_18_3_5_15_236,"[VP] MMIAPIVP_FSMExit the fsm is wrong vp_handle = %x")
TRACE_MSG(MMIVP_FSM_5755_112_2_18_3_5_15_237,"[VP] MMIAPIVP_FSMCompulsivePause the fsm is wrong vp_handle = %x")
TRACE_MSG(MMIVP_FSM_5759_112_2_18_3_5_15_238,"[VP] MMIAPIVP_FSMCompulsivePause begin fsm_ptr->is_to_resume = %d")
TRACE_MSG(MMIVP_FSM_5778_112_2_18_3_5_15_239,"[VP] MMIAPIVP_FSMCompulsivePause end fsm_ptr->is_to_pause = %d")
TRACE_MSG(MMIVP_FSM_5796_112_2_18_3_5_15_240,"[VP] MMIAPIVP_FSMCompulsiveResume the fsm is wrong vp_handle = %x")
TRACE_MSG(MMIVP_FSM_5799_112_2_18_3_5_15_241,"[VP] MMIAPIVP_FSMCompulsiveResume begin fsm_ptr->is_to_pause = %d")
TRACE_MSG(MMIVP_FSM_5852_112_2_18_3_5_15_242,"[VP] MMIAPIVP_FSMCompulsiveResume end fsm_ptr->is_to_resume = %d")
TRACE_MSG(MMIVP_FSM_5902_112_2_18_3_5_15_243,"[VP] MMIAPIVP_FSMGetFrameOffset info_result = %d, frame_index = %d, time_offset = %d, media_info.video_info.frame_rate = %d, base = %d")
TRACE_MSG(MMIVP_FSM_5924_112_2_18_3_5_15_244,"[VP] VP_GetMediaInfo param is error")
TRACE_MSG(MMIVP_FSM_5962_112_2_18_3_5_15_245,"[VP] MMIAPIVP_Play param is null")
TRACE_MSG(MMIVP_FSM_5966_112_2_18_3_5_15_246,"[VP] MMIAPIVP_Play  start_time = %d")
TRACE_MSG(MMIVP_FSM_5978_112_2_18_3_5_15_247,"[VP] MMIAPIVP_Play play_param_ptr = %x, full_path_len = %d, full_path_ptr = %x, is_random_play = %d, mode = %d, play_type = %d, url_ptr = %x, video_buf_info.type = %d, video_buf_len = %d, video_buf_ptr = %x")
TRACE_MSG(MMIVP_FSM_5987_112_2_18_3_5_15_248,"[VP] MMIAPIVP_Play audioSupport = %d, Height = %d, RotateAngle = %d, videoSupport = %d,Width = %d,xOffset = %d, yOffset = %d")
TRACE_MSG(MMIVP_FSM_5997_112_2_18_3_5_15_249,"[VP] MMIAPIVP_Play fsm is not ready now, player wait")
TRACE_MSG(MMIVP_FSM_6003_112_2_18_3_5_15_250,"[VP] MMIAPIVP_Play call_back_func = %x ,start_time = %d")
TRACE_MSG(MMIVP_FSM_6029_112_2_18_3_5_15_251,"[VP] MMIAPIVP_ExitPlay s_current_fsm_ptr = %x")
TRACE_MSG(MMIVP_FSM_6066_112_2_18_3_5_15_252,"[VP]: MMIAPIVP_GetInfoFromFullPathName() param error")
TRACE_MSG(MMIVP_FSM_6078_112_2_18_3_5_15_253,"[VP]: MMIAPIVP_GetInfoFromFullPathName: open dplayer fail! ")
TRACE_MSG(MMIVP_FSM_6100_112_2_18_3_5_15_254,"[VP]: MMIAPIVP_GetInfoFromFullPathName() return = 0x%.2x, full_name_len = %d")
TRACE_MSG(MMIVP_FSM_6111_112_2_18_3_5_15_255,"[VP]: MMIAPIVP_GetInfoFromFullPathName():ntfile_type = %d,ntlen = %d,ntv_spt = %d,ntf_rate = %d,ntheight = %d,ntwidth = %d,ntvideo_type = %d,ntaudio_type = %d")
TRACE_MSG(MMIVP_FSM_6141_112_2_18_3_5_15_256,"[VP]: MMIAPIVP_GetMediaInfoFromBuf param error")
TRACE_MSG(MMIVP_FSM_6145_112_2_18_3_5_15_257,"[VP]: MMIAPIVP_GetMediaInfoFromBuf: video_type = %d, video_length = %d")
TRACE_MSG(MMIVP_FSM_6180_112_2_18_3_5_16_258,"[VP]: MMIAPIVP_GetMediaInfoFromBuf: open dplayer fail! ")
TRACE_MSG(MMIVP_FSM_6207_112_2_18_3_5_16_259,"[VP] MMIAPIVP_GetSupportedStatus param is null")
TRACE_MSG(MMIVP_FSM_6235_112_2_18_3_5_16_260,"[VP] CheckSupportedTypeFromMediaInfo param is null")
TRACE_MSG(MMIVP_FSM_6244_112_2_18_3_5_16_261,"[VP] CheckSupportedTypeFromMediaInfo video_support = %d, audio_support = %d , video_type = %d, audio_type = %d, height = %d, width = %d")
TRACE_MSG(MMIVP_FSM_6297_112_2_18_3_5_16_262,"[VP] MMIVP_RegTaskID vp_handle = %x, task_id = %d")
TRACE_MSG(MMIVP_FSM_6315_112_2_18_3_5_16_263,"[VP] MMIVP_UnRegTaskID vp_handle = %x, task_id = %d")
TRACE_MSG(MMIVP_FSM_6547_112_2_18_3_5_16_264,"[VP] s_switch_timer_id = 0x%x")
TRACE_MSG(MMIVP_FSM_6564_112_2_18_3_5_16_265,"[VP] StopDelayPlayVideoPlayerTimer Stop Timer")
TRACE_MSG(MMIVP_FSM_6616_112_2_18_3_5_16_266,"[VP] VP_dplayerOpenSecurty handle=0x%x")
TRACE_MSG(MMIVP_FSM_6630_112_2_18_3_5_16_267,"VP_dplayerOpenSecurty, VPlayerSecurity_GetVDecSecPlugInfo,FID=%d,plug_handle=0x%x")
TRACE_MSG(MMIVP_FSM_6640_112_2_18_3_5_16_268,"[VP] VP_dplayerOpenSecurty handle=0x%x")
TRACE_MSG(MMIVP_FSM_6645_112_2_18_3_5_16_269,"VP_dplayerOpenSecurty, VPlayerSecurity_GetVDecSecPlugInfo,FID=%d,plug_handle=0x%x")
TRACE_MSG(MMIVP_FSM_6664_112_2_18_3_5_16_270,"VP_dplayerCloseSecurty,FID=%d,handle=0x%x,plug_handle=0x%x")
TRACE_MSG(MMIVP_FSM_6686_112_2_18_3_5_17_271,"VP_dplayerCloseSecurty,FID=%d,handle=0x%x,plug_handle=0x%x")
TRACE_MSG(MMIVP_FSM_6713_112_2_18_3_5_17_272,"VP_dplayerExtCtlAddSecurty entry,s_vdec_sec_argv[0]=0x%x")
TRACE_MSG(MMIVP_FSM_6735_112_2_18_3_5_17_273,"VP_dplayerExtCtlDeltSecurty handle = 0x%x,s_vdec_sec_argv[0]=0x%x")
TRACE_MSG(MMIVP_FSM_6890_112_2_18_3_5_17_274,"[VP]:MMIAPIVP_CurrentVideoLcdSwitch result=%d,Angle=%d")
TRACE_MSG(MMIVP_FSM_6908_112_2_18_3_5_17_275,"[VP]:MMIAPIVP_SetBgVideoAudioSupport audioSupport=%d,audioSupport=%d")
TRACE_MSG(MMIVP_FSM_6936_112_2_18_3_5_17_276,"[VP]:MMIAPIVP_Pause pause current video")
TRACE_MSG(MMIVP_FSM_6950_112_2_18_3_5_17_277,"[VP]:MMIAPIVP_Resume resume current video")
TRACE_MSG(MMIVP_FSM_6968_112_2_18_3_5_17_278,"[VP]:VPPlayCallBack event=%d")
TRACE_MSG(MMIVP_FSM_6985_112_2_18_3_5_17_279,"[VP]:VPPlayCallBack report=%d,data=%d")
TRACE_MSG(MMIVP_FSM_7088_112_2_18_3_5_17_280,"[VP]:ReqPlaySrv handle is already exist!!")
TRACE_MSG(MMIVP_FSM_7104_112_2_18_3_5_17_281,"[VP]: MMISRVAUD_SetAudioInfo error!!!!!!")
TRACE_MSG(MMIVP_FSM_7135_112_2_18_3_5_17_282,"[VP]:ReqPlaySrv handle = %d")
TRACE_MSG(MMIVP_HISTORYLIST_114_112_2_18_3_5_18_283,"[VP] GetHistoryListFileNameForWrite min_free_size = %d")
TRACE_MSG(MMIVP_HISTORYLIST_158_112_2_18_3_5_18_284,"[VP] GetHistoryListSpaceSize load_size = %x")
TRACE_MSG(MMIVP_HISTORYLIST_173_112_2_18_3_5_18_285,"[VP] FreeHistoryListItem index = %d")
TRACE_MSG(MMIVP_HISTORYLIST_216_112_2_18_3_5_18_286,"[VP] FreeHistoryListItem  result = %d")
TRACE_MSG(MMIVP_HISTORYLIST_285_112_2_18_3_5_18_287,"[VP] SaveHistorylistToFile load_size = %x, buffer_ptr = %x")
TRACE_MSG(MMIVP_HISTORYLIST_306_112_2_18_3_5_18_288,"[VP]SaveHistorylistToFile pathlen==0  error!!!!!!!")
TRACE_MSG(MMIVP_HISTORYLIST_322_112_2_18_3_5_18_289,"[VP] SaveHistorylistToFile write_size = %d")
TRACE_MSG(MMIVP_HISTORYLIST_462_112_2_18_3_5_19_290,"[VP] VP_HistoryList_Load readlist_result = %d")
TRACE_MSG(MMIVP_HISTORYLIST_471_112_2_18_3_5_19_291,"[VP] VP_HistoryList_Load s_visit_his_que_ptr = %x")
TRACE_MSG(MMIVP_HISTORYLIST_479_112_2_18_3_5_19_292,"[VP] VP_HistoryList_Load s_visit_his_que_ptr->item_count = %d")
TRACE_MSG(MMIVP_HISTORYLIST_483_112_2_18_3_5_19_293,"[VP] VP_HistoryList_Load cur_item_info_ptr = %x")
TRACE_MSG(MMIVP_HISTORYLIST_513_112_2_18_3_5_19_294,"[VP] VP_HistoryList_Load read_size = %d")
TRACE_MSG(MMIVP_HISTORYLIST_632_112_2_18_3_5_19_295,"[VP] VP_HistoryList_GetInfo list_index = %d")
TRACE_MSG(MMIVP_HISTORYLIST_645_112_2_18_3_5_19_296,"[VP] VP_HistoryList_GetInfo i=%d")
TRACE_MSG(MMIVP_HISTORYLIST_659_112_2_18_3_5_19_297,"[VP] VP_HistoryList_DelItem index = %d")
TRACE_MSG(MMIVP_HISTORYLIST_662_112_2_18_3_5_19_298,"[VP] VP_HistoryList_DelItem error param")
TRACE_MSG(MMIVP_HISTORYLIST_671_112_2_18_3_5_19_299,"[VP] VP_HistoryList_DelItem result = %d, s_visit_his_que_ptr->item_count = %d")
TRACE_MSG(MMIVP_HISTORYLIST_707_112_2_18_3_5_19_300,"[VP] VP_HistoryList_AddItem there is no enough space")
TRACE_MSG(MMIVP_HISTORYLIST_750_112_2_18_3_5_19_301,"[VP] VP_HistoryList_AddItem current_history_item_ptr = %x")
TRACE_MSG(MMIVP_HISTORYLIST_766_112_2_18_3_5_19_302,"[VP] VP_HistoryList_AddItem s_visit_his_que_ptr = %x")
TRACE_MSG(MMIVP_HISTORYLIST_772_112_2_18_3_5_19_303,"[VP] VP_HistoryList_AddItem there is no enough space")
TRACE_MSG(MMIVP_HISTORYLIST_814_112_2_18_3_5_19_304,"[VP] VP_HistoryList_SetLastPlayPoint last_play_point = %d")
TRACE_MSG(MMIVP_HISTORYLIST_818_112_2_18_3_5_19_305,"[VP] VP_HistoryList_SetLastPlayPoint list error")
TRACE_MSG(MMIVP_HISTORYLIST_849_112_2_18_3_5_19_306,"[VP] VP_HistoryList_ModifyItem  last_play_point = %d, video_height = %d, video_width = %d, media_length = %d, title_len = %d")
TRACE_MSG(MMIVP_LOCALLIST_120_112_2_18_3_5_20_307,"[VP] MMIAPIVP_GetFilerInfo param is null")
TRACE_MSG(MMIVP_LOCALLIST_166_112_2_18_3_5_20_308,"[VP] AddFileToLocalListCallBack is_success = %d, file_num = %d, file_array = %x, s_oplist_info.locallist_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_174_112_2_18_3_5_20_309,"[VP] AddFileToLocalListCallBack total_num 1 = %d")
TRACE_MSG(MMIVP_LOCALLIST_176_112_2_18_3_5_20_310,"[VP] AddFileToLocalListCallBack total_num = %d")
TRACE_MSG(MMIVP_LOCALLIST_193_112_2_18_3_5_20_311,"[VP] AddFileToLocalListCallBack is_success = %d, s_oplist_info.locallist_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_245_112_2_18_3_5_20_312,"[VP] GetLocalListFileNameForRead locallist_type = %d, result = %d")
TRACE_MSG(MMIVP_LOCALLIST_262_112_2_18_3_5_20_313,"[VP] GetLocalListFileNameForWrite locallist_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_295_112_2_18_3_5_20_314,"[VP] GetLocalListFileNameForWrite result = %d")
TRACE_MSG(MMIVP_LOCALLIST_310_112_2_18_3_5_20_315,"[VP] SaveLocallistToFile locallist_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_353_112_2_18_3_5_20_316,"[VP] SaveLocallistToFile save_result = %d")
TRACE_MSG(MMIVP_LOCALLIST_477_112_2_18_3_5_21_317,"[VP] MMIVP_GetDefaultMoviePath, file_dev_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_497_112_2_18_3_5_21_318,"[VP] GetDifferentRandom locallist_type = %d, locallist_index = %d")
TRACE_MSG(MMIVP_LOCALLIST_570_112_2_18_3_5_21_319,"[VP] VP_LocalList_BeginLoad locallist_type = %d,file_dev_type=%d")
TRACE_MSG(MMIVP_LOCALLIST_594_112_2_18_3_5_21_320,"[VP] MMIVP_GetDefaultMoviePath is failed!")
TRACE_MSG(MMIVP_LOCALLIST_625_112_2_18_3_5_21_321,"[VP] VP_LocalList_BeginLoad load_result = %d, locallist_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_636_112_2_18_3_5_21_322,"[VP] VP_LocalList_EndLoad locallist_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_652_112_2_18_3_5_21_323,"[VP] VP_LocalList_Release locallist_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_672_112_2_18_3_5_21_324,"[VP] VP_LocalList_GetInfo locallist_type = %d, locallist_index = %d")
TRACE_MSG(MMIVP_LOCALLIST_677_112_2_18_3_5_21_325,"[VP] VP_LocalList_GetInfo locallist_type = %d, result = %d, locallist_index = %d")
TRACE_MSG(MMIVP_LOCALLIST_695_112_2_18_3_5_21_326,"[VP] VP_LocalList_GetNum locallist_type = %d, locallist_num = %d")
TRACE_MSG(MMIVP_LOCALLIST_712_112_2_18_3_5_21_327,"[VP] VP_LocalList_AddItem locallist_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_778_112_2_18_3_5_21_328,"[VP] VP_LocalList_BeginUpdate locallist_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_821_112_2_18_3_5_21_329,"[VP] VP_LocalList_EndUpdate locallist_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_835_112_2_18_3_5_21_330,"[VP] VP_LocalList_DelItemWithouSaveFile locallist_type= %d")
TRACE_MSG(MMIVP_LOCALLIST_850_112_2_18_3_5_22_331,"[VP] VP_Locallist_SaveToFile locallist_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_866_112_2_18_3_5_22_332,"[VP] VP_LocalList_GetNext cur_index = %d, locallist_index = %d")
TRACE_MSG(MMIVP_LOCALLIST_919_112_2_18_3_5_22_333,"[VP] VP_LocalList_GetPrev cur_index = %d, total_num = %d, locallist_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_979_112_2_18_3_5_22_334,"[VP] VP_LocalList_ResetCurPlayList is_random_play = %d, total_num = %d")
TRACE_MSG(MMIVP_LOCALLIST_1029_112_2_18_3_5_22_335,"[VP] VP_LocalList_SetIsCurPlayListItemOK is_ok = %d , s_playing_list.active_index = %d")
TRACE_MSG(MMIVP_LOCALLIST_1072_112_2_18_3_5_22_336,"[VP] VP_LocalList_GetPreByMode mode = %d, is_random = %d, total_num = %d,locallist_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_1083_112_2_18_3_5_22_337,"[VP] VP_LocalList_GetPreByMode param is error")
TRACE_MSG(MMIVP_LOCALLIST_1123_112_2_18_3_5_22_338,"[VP] VP_LocalList_GetPreByMode one circle")
TRACE_MSG(MMIVP_LOCALLIST_1150_112_2_18_3_5_22_339,"[VP] VP_LocalList_GetPreByMode one circle")
TRACE_MSG(MMIVP_LOCALLIST_1154_112_2_18_3_5_22_340,"[VP] VP_LocalList_GetPreByMode s_playing_list.playing_list_ptr[i].is_ok = %d")
TRACE_MSG(MMIVP_LOCALLIST_1187_112_2_18_3_5_22_341,"[VP] VP_LocalList_GetPreByMode result = %d,next_index = %d")
TRACE_MSG(MMIVP_LOCALLIST_1206_112_2_18_3_5_22_342,"[VP] VP_LocalList_GetPreByMode result = %d")
TRACE_MSG(MMIVP_LOCALLIST_1230_112_2_18_3_5_22_343,"[VP] VP_LocalList_GetNextByMode mode = %d, is_random = %d, total_num = %d,locallist_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_1241_112_2_18_3_5_22_344,"[VP] VP_LocalList_GetNextByMode param is error")
TRACE_MSG(MMIVP_LOCALLIST_1282_112_2_18_3_5_22_345,"[VP] VP_LocalList_GetNextByMode one circle")
TRACE_MSG(MMIVP_LOCALLIST_1309_112_2_18_3_5_22_346,"[VP] VP_LocalList_GetNextByMode one circle")
TRACE_MSG(MMIVP_LOCALLIST_1313_112_2_18_3_5_22_347,"[VP] VP_LocalList_GetNextByMode s_playing_list.playing_list_ptr[i].is_ok = %d")
TRACE_MSG(MMIVP_LOCALLIST_1346_112_2_18_3_5_23_348,"[VP] VP_LocalList_GetNextByMode result = %d,next_index = %d")
TRACE_MSG(MMIVP_LOCALLIST_1365_112_2_18_3_5_23_349,"[VP] VP_LocalList_GetNextByMode result = %d")
TRACE_MSG(MMIVP_LOCALLIST_1377_112_2_18_3_5_23_350,"[VP] VP_LocalList_GetCurrentIndex locallist_type = %d, s_current_list_info.locallist_type = %d")
TRACE_MSG(MMIVP_LOCALLIST_1391_112_2_18_3_5_23_351,"[VP] VP_LocalList_SetCurrentIndex index = %d")
TRACE_MSG(MMIVP_LOCALLIST_1411_112_2_18_3_5_23_352,"[VP] VP_LocalList_SetCurListType type = %d")
TRACE_MSG(MMIVP_SETTING_136_112_2_18_3_5_24_353,"[VP] VP_Setting_GetBrightness nv_setting_info.brightness = %d")
TRACE_MSG(MMIVP_SETTING_147_112_2_18_3_5_24_354,"[VP] VP_Setting_SetBrightness brightness = %d")
TRACE_MSG(MMIVP_SETTING_161_112_2_18_3_5_24_355,"[VP] VP_Setting_GetPlaybackMode nv_setting_info.playback_mode = %d")
TRACE_MSG(MMIVP_SETTING_172_112_2_18_3_5_24_356,"[VP] VP_Setting_SetRepeatPlayMode playback_mode = %d")
TRACE_MSG(MMIVP_SETTING_186_112_2_18_3_5_24_357,"[VP] VP_Setting_GetRepeatPlayMode nv_setting_info.mode = %d")
TRACE_MSG(MMIVP_SETTING_197_112_2_18_3_5_24_358,"[VP] VP_Setting_SetRepeatPlayMode mode = %d")
TRACE_MSG(MMIVP_SETTING_211_112_2_18_3_5_25_359,"[VP] VP_Setting_GetIsRandomPlayMode nv_setting_info.is_random_play = %d")
TRACE_MSG(MMIVP_SETTING_222_112_2_18_3_5_25_360,"[VP] VP_Setting_SetIsRandomPlayMode is_open = %d")
TRACE_MSG(MMIVP_SETTING_238_112_2_18_3_5_25_361,"[VP] VP_Setting_GetNetIndex dual_sys = %d")
TRACE_MSG(MMIVP_SETTING_243_112_2_18_3_5_25_362,"[VP] VP_Setting_GetNetIndex dual_sys = %d, index = %d")
TRACE_MSG(MMIVP_SETTING_255_112_2_18_3_5_25_363,"[VP] VP_Setting_SetNetIndex dual_sys = %d, index = %d")
TRACE_MSG(MMIVP_SETTING_277_112_2_18_3_5_25_364,"[VP] VP_Setting_SetRTPPort lowest_port = %d, highest_port = %d")
TRACE_MSG(MMIVP_SETTING_296_112_2_18_3_5_25_365,"[VP] VP_Setting_GetRTPPort param is error")
TRACE_MSG(MMIVP_SETTING_319_112_2_18_3_5_25_366,"[VP] VP_Setting_SetAgent param is error")
TRACE_MSG(MMIVP_SETTING_351_112_2_18_3_5_25_367,"[VP] VP_Setting_GetAgent param is error")
TRACE_MSG(MMIVP_THUMBNAIL_94_112_2_18_3_5_25_368,"[VP]:HandleOpenNextVideo!")
TRACE_MSG(MMIVP_THUMBNAIL_126_112_2_18_3_5_25_369,"[VP]:HandleOpenNextVideo  error state , not video queue")
TRACE_MSG(MMIVP_THUMBNAIL_143_112_2_18_3_5_26_370,"[VP]:HandleGetThumbnaiData stopping!")
TRACE_MSG(MMIVP_THUMBNAIL_148_112_2_18_3_5_26_371,"[VP]:HandleGetThumbnaiData error msg!")
TRACE_MSG(MMIVP_THUMBNAIL_161_112_2_18_3_5_26_372,"[VP]:HandleGetThumbnaiData height=%d,width=%d,datalen=%d!")
TRACE_MSG(MMIVP_THUMBNAIL_212_112_2_18_3_5_26_373,"[VP]:VP_ThumbnailGetVideoPathname")
TRACE_MSG(MMIVP_THUMBNAIL_230_112_2_18_3_5_26_374,"[VP]:MMIAPIVP_IsThumbnailRunning isloading:%d")
TRACE_MSG(MMIVP_THUMBNAIL_246_112_2_18_3_5_26_375,"[VP]:MMIAPIVP_IsThumbnailOpen isopen:%d")
TRACE_MSG(MMIVP_THUMBNAIL_264_112_2_18_3_5_26_376,"[VP]:MMIAPIVP_ThumbnailVideoAdd  !")
TRACE_MSG(MMIVP_THUMBNAIL_272_112_2_18_3_5_26_377,"[VP]:VP_ThumbnailAddVideo full queue!")
TRACE_MSG(MMIVP_THUMBNAIL_287_112_2_18_3_5_26_378,"[VP]:VP_ThumbnailAddVideo videonum:%d!")
TRACE_MSG(MMIVP_THUMBNAIL_306_112_2_18_3_5_26_379,"[VP]:MMIAPIVP_ThumbnailOpen !")
TRACE_MSG(MMIVP_THUMBNAIL_315_112_2_18_3_5_26_380,"[VP]:MMIAPIVP_ThumbnailOpen is open now!")
TRACE_MSG(MMIVP_THUMBNAIL_325_112_2_18_3_5_26_381,"[VP]:MMIVP_GetSeekPointImgeData list_file_name :0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x")
TRACE_MSG(MMIVP_THUMBNAIL_344_112_2_18_3_5_26_382,"[VP]:MMIAPIVP_ThumbnailOpen alloc mem error!")
TRACE_MSG(MMIVP_THUMBNAIL_384_112_2_18_3_5_26_383,"[VP]:MMIAPIVP_ThumbnailResume !")
TRACE_MSG(MMIVP_THUMBNAIL_388_112_2_18_3_5_26_384,"[VP]:MMIAPIVP_ThumbnailResume  thumnail is loading!!")
TRACE_MSG(MMIVP_THUMBNAIL_407_112_2_18_3_5_26_385,"[VP]:MMIAPIVP_ThumbnailStop !")
TRACE_MSG(MMIVP_THUMBNAIL_430_112_2_18_3_5_26_386,"[VP]:MMIAPIVP_ThumbnailClose!")
TRACE_MSG(MMIVP_THUMBNAIL_458_112_2_18_3_5_26_387,"[VP]:StopCurrentDPLAYER!")
TRACE_MSG(MMIVP_THUMBNAIL_473_112_2_18_3_5_26_388,"[VP]:Thumbnail_VideoQueueRun  !")
TRACE_MSG(MMIVP_THUMBNAIL_477_112_2_18_3_5_26_389,"[VP]:Thumbnail_VideoQueueRun is not videoqueue !")
TRACE_MSG(MMIVP_THUMBNAIL_482_112_2_18_3_5_26_390,"[VP]:Thumbnail_VideoQueueRun  queue empty !")
TRACE_MSG(MMIVP_THUMBNAIL_494_112_2_18_3_5_26_391,"[VP]:Thumbnail_VideoQueueRun   fail !")
TRACE_MSG(MMIVP_THUMBNAIL_513_112_2_18_3_5_26_392,"[VP] CheckSupportedTypeFromMedia param is null")
TRACE_MSG(MMIVP_THUMBNAIL_522_112_2_18_3_5_26_393,"[VP] CheckSupportedTypeFromMedia video_support = %d, audio_support = %d , video_type = %d, audio_type = %d, height = %d, width = %d")
TRACE_MSG(MMIVP_THUMBNAIL_557_112_2_18_3_5_26_394,"[VP]:Thumbnail_OpenDplayer !")
TRACE_MSG(MMIVP_THUMBNAIL_560_112_2_18_3_5_26_395,"[VP]:Thumbnail_OpenDplayer is loading now!")
TRACE_MSG(MMIVP_THUMBNAIL_582_112_2_18_3_5_26_396,"[VP]:Thumbnail_OpenDplayer   s_thumbnail_info.dphandle:0X%0x")
TRACE_MSG(MMIVP_THUMBNAIL_600_112_2_18_3_5_26_397,"[VP]:Thumbnail_OpenDplayer CheckSupportedTypeFromMediaInfo fail !")
TRACE_MSG(MMIVP_THUMBNAIL_625_112_2_18_3_5_27_398,"[VP]:Thumbnail_OpenDplayer DPLAYER_Play fail !")
TRACE_MSG(MMIVP_THUMBNAIL_640_112_2_18_3_5_27_399,"[VP]:Thumbnail_OpenDplayer DPLAYER_SetTime fail !")
TRACE_MSG(MMIVP_THUMBNAIL_656_112_2_18_3_5_27_400,"[VP]:Thumbnail_OpenDplayer DPLAYER_SetDisplayParam fail !")
TRACE_MSG(MMIVP_THUMBNAIL_672_112_2_18_3_5_27_401,"[VP]:Thumbnail_OpenDplayer DPLAYER_Open fail !")
TRACE_MSG(MMIVP_THUMBNAIL_692_112_2_18_3_5_27_402,"[VP]:Thumbnail_VideoDelete Video pre num:%d !")
TRACE_MSG(MMIVP_THUMBNAIL_695_112_2_18_3_5_27_403,"[VP]:Thumbnail_VideoDelete  queue empty !")
TRACE_MSG(MMIVP_THUMBNAIL_705_112_2_18_3_5_27_404,"[VP]:Thumbnail_VideoDelete Video post num:%d !")
TRACE_MSG(MMIVP_THUMBNAIL_732_112_2_18_3_5_27_405,"[VP]:CallbackThumbnaiEndNotify,dphandle:0X%0x,is_loading:%d,is_stopping:%d")
TRACE_MSG(MMIVP_THUMBNAIL_746_112_2_18_3_5_27_406,"[VP]:CallbackOneFrameEndNotify,dphandle:0X%0x,is_loading:%d,is_stopping:%d")
TRACE_MSG(MMIVP_WINTABLE_2091_112_2_18_3_5_32_407,"[VP] StartProcessTimer before s_process_timer_id = %d")
TRACE_MSG(MMIVP_WINTABLE_2105_112_2_18_3_5_32_408,"[VP] StartProcessTimer s_process_timer_id = %d")
TRACE_MSG(MMIVP_WINTABLE_2133_112_2_18_3_5_32_409,"[VP] StopProcessTimer s_process_timer_id = %d")
TRACE_MSG(MMIVP_WINTABLE_2175_112_2_18_3_5_32_410,"[VP] HandleFsmMsg param is error")
TRACE_MSG(MMIVP_WINTABLE_2178_112_2_18_3_5_32_411,"[VP] HandleFsmMsg msg_ptr->msg_id = %x")
TRACE_MSG(MMIVP_WINTABLE_2199_112_2_18_3_5_32_412,"[VP] HandleFsmMsg play_type = %d, total_time = %d,support_type = %d")
TRACE_MSG(MMIVP_WINTABLE_2227_112_2_18_3_5_32_413,"[VP] HandleFsmMsg buffering s_video_player_control_info.state = %d")
TRACE_MSG(MMIVP_WINTABLE_2255_112_2_18_3_5_32_414,"[VP] HandleFsmMsg buffer end s_video_player_control_info.state = %d")
TRACE_MSG(MMIVP_WINTABLE_2285_112_2_18_3_5_32_415,"[VP] HandleFsmMsg play_result.error_code = %d, s_video_player_control_info.to_do_type = %d, s_video_player_control_info.process_time = %d")
TRACE_MSG(MMIVP_WINTABLE_2290_112_2_18_3_5_32_416,"[VP] HandleFsmMsg play cnf s_video_player_control_info.state = %d")
TRACE_MSG(MMIVP_WINTABLE_2314_112_2_18_3_5_32_417,"[VP] HandleFsmMsg s_tp_down_ctrl_id = %d, s_key_down_msg_id = %d")
TRACE_MSG(MMIVP_WINTABLE_2361_112_2_18_3_5_32_418,"[VP] HandleFsmMsg play cnf error s_video_player_control_info.state = %d")
TRACE_MSG(MMIVP_WINTABLE_2399_112_2_18_3_5_33_419,"[VP] HandleFsmMsg pause_result.error_code = %d")
TRACE_MSG(MMIVP_WINTABLE_2402_112_2_18_3_5_33_420,"[VP] HandleFsmMsg pause cnf s_video_player_control_info.state = %d")
TRACE_MSG(MMIVP_WINTABLE_2424_112_2_18_3_5_33_421,"[VP] HandleFsmMsg seek_result.error_code = %d")
TRACE_MSG(MMIVP_WINTABLE_2452_112_2_18_3_5_33_422,"[VP] HandleFsmMsg fatal_error.error_code = %d")
TRACE_MSG(MMIVP_WINTABLE_2496_112_2_18_3_5_33_423,"[VP] HandleFsmMsg play_end_ind.result = %d")
TRACE_MSG(MMIVP_WINTABLE_2565_112_2_18_3_5_33_424,"[VP] HandleFsmMsg stop cnf s_video_player_control_info.state = %d")
TRACE_MSG(MMIVP_WINTABLE_2600_112_2_18_3_5_33_425,"[VP] VP_STOP_IND s_video_player_control_info.to_do_type = %d")
TRACE_MSG(MMIVP_WINTABLE_2633_112_2_18_3_5_33_426,"[VP] VP_STOP_IND full_path_len = %d")
TRACE_MSG(MMIVP_WINTABLE_2701_112_2_18_3_5_33_427,"[VP] SaveHistory locallist_type = %d, locallist_index = %d")
TRACE_MSG(MMIVP_WINTABLE_2774_112_2_18_3_5_33_428,"[VP] NotifyPlayerForPlayFile full_path_name_ptr = %x, process_time = %d, play_type = %d, full_path_name_len = %d")
TRACE_MSG(MMIVP_WINTABLE_2820_112_2_18_3_5_33_429,"[VP] PlayFromNet process_time = %d")
TRACE_MSG(MMIVP_WINTABLE_2887_112_2_18_3_5_34_430,"[VP] NotifyPlayerForPlayNet full_path_name_ptr = %x, url_ptr=%x, process_time = %d, full_path_name_len = %d")
TRACE_MSG(MMIVP_WINTABLE_3045_112_2_18_3_5_34_431,"MMIAPIVP_PlayVideoFromVideoPlayer java running =%d")
TRACE_MSG(MMIVP_WINTABLE_3053_112_2_18_3_5_34_432,"[VP] MMIAPIVP_PlayVideoFromVideoPlayer vp_entry = %d")
TRACE_MSG(MMIVP_WINTABLE_3061_112_2_18_3_5_34_433,"[VP] MMIAPIVP_PlayVideoFromVideoPlayer MMIAPIBROWSER_IsRunning()")
TRACE_MSG(MMIVP_WINTABLE_3113_112_2_18_3_5_34_434,"[VP] MMIAPIVP_MainEntry cannot creat fsm")
TRACE_MSG(MMIVP_WINTABLE_3168_112_2_18_3_5_34_435,"[VP] MMIAPIVP_ExitVideoPlayer")
TRACE_MSG(MMIVP_WINTABLE_3210_112_2_18_3_5_34_436,"[VP] HandleVPSearchWaitingWin msg_id = %x, is_open_player_win = %d, locallist_type = %d")
TRACE_MSG(MMIVP_WINTABLE_3242_112_2_18_3_5_34_437,"[VP] HandleVPSearchWaitingWin list_index = %d")
TRACE_MSG(MMIVP_WINTABLE_3646_112_2_18_3_5_35_438,"[VP] DisplayVPWinStation display_station.wstr_len = %d,text_string.wstr_len = %d")
TRACE_MSG(MMIVP_WINTABLE_3767_112_2_18_3_5_35_439,"[VP] DisplayVPWinRepeatModeIcon display_type = %d")
TRACE_MSG(MMIVP_WINTABLE_3869_112_2_18_3_5_36_440,"[VP] DisplayVPWinRandomModeIcon display_type = %d")
TRACE_MSG(MMIVP_WINTABLE_4048_112_2_18_3_5_36_441,"[VP] DisplayVPWinBackwardOrForwardIcon s_video_player_control_info.state = %d")
TRACE_MSG(MMIVP_WINTABLE_4111_112_2_18_3_5_36_442,"[VP] DisplayVPWinPlayOrPauseIcon s_video_player_control_info.state = %d")
TRACE_MSG(MMIVP_WINTABLE_4163_112_2_18_3_5_36_443,"[vp] DisplayVPWinPlayOrPauseIcon error")
TRACE_MSG(MMIVP_WINTABLE_4211_112_2_18_3_5_36_444,"[VP] DisplayVpWinDefaultPic not display default pic")
TRACE_MSG(MMIVP_WINTABLE_4261_112_2_18_3_5_36_445,"[VP] DisplayVpWinVideoArea s_video_player_control_info.state = %d")
TRACE_MSG(MMIVP_WINTABLE_4481_112_2_18_3_5_37_446,"[VP] DisplayVPWinProgressEx process_time = %d, total_time = %d")
TRACE_MSG(MMIVP_WINTABLE_4739_112_2_18_3_5_37_447,"[VP] DisappearVPWin is_disappear = %d, s_video_player_control_info.is_display_win = %d")
TRACE_MSG(MMIVP_WINTABLE_4776_112_2_18_3_5_37_448,"[VP] DisappearVPWin s_video_player_control_info.support_type = %d")
TRACE_MSG(MMIVP_WINTABLE_4835_112_2_18_3_5_37_449,"[VP] LoadLastPlayingFile history_info_ptr->locallist_type = %d, history_info_ptr->locallist_index = %d")
TRACE_MSG(MMIVP_WINTABLE_4885_112_2_18_3_5_38_450,"[VP] LoadLastPlayingFile process = %d, total_time = %d")
TRACE_MSG(MMIVP_WINTABLE_5066_112_2_18_3_5_38_451,"[VP] VideoPlayerResume s_video_player_control_info.state = %d")
TRACE_MSG(MMIVP_WINTABLE_5171_112_2_18_3_5_38_452,"[VP] VideoPlayerPause s_video_player_control_info.state = %d")
TRACE_MSG(MMIVP_WINTABLE_5189_112_2_18_3_5_38_453,"[VP] VideoPlayerPause s_video_player_control_info.state = %d")
TRACE_MSG(MMIVP_WINTABLE_5202_112_2_18_3_5_38_454,"[VP] VideoPlayerPause PAUSE FAIL s_video_player_control_info.state = %d")
TRACE_MSG(MMIVP_WINTABLE_5290_112_2_18_3_5_38_455,"[VP] NotifyFsmPlay is_bt_support = %d, is_a2dp_support = %d")
TRACE_MSG(MMIVP_WINTABLE_5580_112_2_18_3_5_39_456,"[VP] PlayPreFile failed ! ")
TRACE_MSG(MMIVP_WINTABLE_5601_112_2_18_3_5_39_457,"[VP] Backward process_time =%d")
TRACE_MSG(MMIVP_WINTABLE_5699_112_2_18_3_5_39_458,"[VP] PlayNextFile failed ! ")
TRACE_MSG(MMIVP_WINTABLE_5719_112_2_18_3_5_39_459,"[VP] BackWardOrPreFile seek_time = %d, s_video_player_control_info.process_time = %d")
TRACE_MSG(MMIVP_WINTABLE_5796_112_2_18_3_5_39_460,"[VP] AdjustVolCallBack volue = %d")
TRACE_MSG(MMIVP_WINTABLE_5811_112_2_18_3_5_39_461,"[VP] AdjustBrightCallBack volue = %d")
TRACE_MSG(MMIVP_WINTABLE_5855_112_2_18_3_5_39_462,"[VP] StartVPWinDisplayerTimer state = %d")
TRACE_MSG(MMIVP_WINTABLE_5896_112_2_18_3_5_40_463,"[VP] StartVPTimer timer_type = %d, s_video_player_control_info.timer.timer_type  = %d, s_video_player_control_info.timer.timer_id = %d")
TRACE_MSG(MMIVP_WINTABLE_5919_112_2_18_3_5_40_464,"[VP] StartVPTimer s_video_player_control_info.timer.timer_id = %d")
TRACE_MSG(MMIVP_WINTABLE_5951_112_2_18_3_5_40_465,"[VP] HandleVPTimerMsg s_video_player_control_info.timer.timer_type = %d")
TRACE_MSG(MMIVP_WINTABLE_5984_112_2_18_3_5_40_466,"[VP] HandleDirectionKeyMsgForHalfScreen msg_id = %x, s_video_player_control_info.timer.timer_type = %d")
TRACE_MSG(MMIVP_WINTABLE_6070_112_2_18_3_5_40_467,"[VP] HandleShortCutMsg msg_id = %x")
TRACE_MSG(MMIVP_WINTABLE_6762_112_2_18_3_5_41_468,"[VP]: HandleVPWinMsg(), msg_id = %x")
TRACE_MSG(MMIVP_WINTABLE_6857_112_2_18_3_5_41_469,"[VP] HandleVPWinMsg param = %d, s_video_player_control_info.timer.timer_id= %d")
TRACE_MSG(MMIVP_WINTABLE_6963_112_2_18_3_5_42_470,"[VP] handle vp win web key s_video_player_control_info.state = %d")
TRACE_MSG(MMIVP_WINTABLE_7080_112_2_18_3_5_42_471,"[VP]: HandleVPWinMsg(), key msg_id = %x, s_video_player_control_info.is_display_win =%d")
TRACE_MSG(MMIVP_WINTABLE_7198_112_2_18_3_5_42_472,"[VP] OpenSeekWin x = %d")
TRACE_MSG(MMIVP_WINTABLE_7269_112_2_18_3_5_42_473,"[VP] OpenRepeatModeWin repeat_mode = %d")
TRACE_MSG(MMIVP_WINTABLE_7303_112_2_18_3_5_42_474,"[VP] OpenRandomModeWin is_random = %d")
TRACE_MSG(MMIVP_WINTABLE_7323_112_2_18_3_5_42_475,"[VP] OpenControlPanel is not focus and not open control panel")
TRACE_MSG(MMIVP_WINTABLE_7386_112_2_18_3_5_43_476,"[VP] PlayOrPause s_video_player_control_info.state = %d")
TRACE_MSG(MMIVP_WINTABLE_7468_112_2_18_3_5_43_477,"[VP] SwitchScreen is_full = %d")
TRACE_MSG(MMIVP_WINTABLE_7513_112_2_18_3_5_43_478,"[VP] AddFileToPlayerCallBack is_success = %d")
TRACE_MSG(MMIVP_WINTABLE_7596_112_2_18_3_5_43_479,"[VP] SaveBookMark add_result = %d")
TRACE_MSG(MMIVP_WINTABLE_8131_112_2_18_3_5_44_480,"[VP] HandleNetOptMenuWinMsg menu_id = %x")
TRACE_MSG(MMIVP_WINTABLE_8246_112_2_18_3_5_44_481,"[VP] HandleOptMenuWinMsg menu_id = %x")
TRACE_MSG(MMIVP_WINTABLE_8327_112_2_18_3_5_45_482,"[VP] HandleLocalListOptMenuWinMsg net alloca memory error")
TRACE_MSG(MMIVP_WINTABLE_8729_112_2_18_3_5_46_483,"[VP]HandleLinkListWinMsg() msg_id=0x%04x")
TRACE_MSG(MMIVP_WINTABLE_9142_112_2_18_3_5_47_484,"[VP] RichText_AddItem param is error")
TRACE_MSG(MMIVP_WINTABLE_9337_112_2_18_3_5_48_485,"[VP] HandleListAppendData type = %d")
TRACE_MSG(MMIVP_WINTABLE_9344_112_2_18_3_5_48_486,"[VP] HandleListAppendData need_item_data_ptr is null")
TRACE_MSG(MMIVP_WINTABLE_9356_112_2_18_3_5_48_487,"[VP] HandleListAppendData need_item_content_ptr is null")
TRACE_MSG(MMIVP_WINTABLE_9416_112_2_18_3_5_48_488,"[VP] GetBookmarkListItemContent param error")
TRACE_MSG(MMIVP_WINTABLE_9452_112_2_18_3_5_48_489,"[VP] UpdateListData total_num = %d, max_num = %d")
TRACE_MSG(MMIVP_WINTABLE_9541_112_2_18_3_5_48_490,"[VP] HandleListAppendData need_item_data_ptr is null")
TRACE_MSG(MMIVP_WINTABLE_9554_112_2_18_3_5_49_491,"[VP] HandleListAppendData need_item_content_ptr is null")
TRACE_MSG(MMIVP_WINTABLE_9594_112_2_18_3_5_49_492,"[VP] HandleBookmarkListWinMsg list_num = %d, list_index = %d")
TRACE_MSG(MMIVP_WINTABLE_9603_112_2_18_3_5_49_493,"[VP] HandleBookmarkListWinMsg bookmark_item_ptr->title_arr = %x")
TRACE_MSG(MMIVP_WINTABLE_9646_112_2_18_3_5_49_494,"[VP] GetHistoryListItemContent param is error")
TRACE_MSG(MMIVP_WINTABLE_9704_112_2_18_3_5_49_495,"[VP] PlayCurrentHistoryVIdeo history_info_ptr = %x,  is_query_ptr = %x")
TRACE_MSG(MMIVP_WINTABLE_9710_112_2_18_3_5_49_496,"[VP] PlayCurrentHistoryVIdeo history_info_ptr->last_play_point = %d, history_info_ptr->total_time = %x, history_info_ptr->type = %d")
TRACE_MSG(MMIVP_WINTABLE_9729_112_2_18_3_5_49_497,"[VP] PlayCurrentHistoryVIdeo history_info_ptr->type = %d, process_time = %d")
TRACE_MSG(MMIVP_WINTABLE_9735_112_2_18_3_5_49_498,"[VP] PlayCurrentHistoryVIdeo net info pnull")
TRACE_MSG(MMIVP_WINTABLE_9871_112_2_18_3_5_49_499,"[VP] HandleHistoryListWinMsg need_item_data_ptr is null")
TRACE_MSG(MMIVP_WINTABLE_9884_112_2_18_3_5_49_500,"[VP] HandleHistoryListWinMsg need_item_content_ptr is null")
TRACE_MSG(MMIVP_WINTABLE_10083_112_2_18_3_5_50_501,"[VP] HandleShortcutMenuWinMsg menu_id = %x")
TRACE_MSG(MMIVP_WINTABLE_10416_112_2_18_3_5_50_502,"[VP] GetLocalListItemContent param is error")
TRACE_MSG(MMIVP_WINTABLE_10605_112_2_18_3_5_51_503,"[VP] HandleLocalListWinMsg need_item_data_ptr is null")
TRACE_MSG(MMIVP_WINTABLE_10618_112_2_18_3_5_51_504,"[VP] HandleLocalListWinMsg need_item_content_ptr is null")
TRACE_MSG(MMIVP_WINTABLE_10776_112_2_18_3_5_51_505,"[VP] HandleLocalListWinMsg need_item_data_ptr is null")
TRACE_MSG(MMIVP_WINTABLE_10789_112_2_18_3_5_51_506,"[VP] HandleLocalListWinMsg need_item_content_ptr is null")
TRACE_MSG(MMIVP_WINTABLE_10974_112_2_18_3_5_51_507,"[VP] HandleLocalListLongOkOptMenuWinMsg delete FAIL")
TRACE_MSG(MMIVP_WINTABLE_11357_112_2_18_3_5_52_508,"[VP] HandleAddBookmarkWinMsg msg_id = %x")
TRACE_MSG(MMIVP_WINTABLE_11443_112_2_18_3_5_52_509,"[VP] bookmark_data.title_arr = %x, list_index = %d")
TRACE_MSG(MMIVP_WINTABLE_11579_112_2_18_3_5_53_510,"[VP] HandleBookmarkOptMenuWinMsg menu_id = %x")
TRACE_MSG(MMIVP_WINTABLE_11610_112_2_18_3_5_53_511,"[VP] HandleBookmarkOptMenuWinMsg cur_list_index = %d")
TRACE_MSG(MMIVP_WINTABLE_11623_112_2_18_3_5_53_512,"[VP] HandleBookmarkOptMenuWinMsg no memory")
TRACE_MSG(MMIVP_WINTABLE_11663_112_2_18_3_5_53_513,"[VP] HandleBookmarkOptMenuWinMsg menu_id = %x")
TRACE_MSG(MMIVP_WINTABLE_11730_112_2_18_3_5_53_514,"[VP] HandleHistoryOptMenuWinMsg msg_id = %x")
TRACE_MSG(MMIVP_WINTABLE_11755_112_2_18_3_5_53_515,"[VP] HandleHistoryOptMenuWinMsg menu_id = %x")
TRACE_MSG(MMIVP_WINTABLE_11800_112_2_18_3_5_53_516,"[VP] HandleHistoryOptMenuWinMsg no memory")
TRACE_MSG(MMIVP_WINTABLE_11864_112_2_18_3_5_53_517,"[VP] HandleHistoryOptMenuWinMsg menu_id = %x")
TRACE_MSG(MMIVP_WINTABLE_11920_112_2_18_3_5_53_518,"[VP] AddLocalListCallBack locallist_type = %d, list_index = %d, is_success = %d")
TRACE_MSG(MMIVP_WINTABLE_11995_112_2_18_3_5_53_519,"[VP] HandleLocalListOptMenuWinMsg msg_id = %x, locallist_type = %d")
TRACE_MSG(MMIVP_WINTABLE_12050_112_2_18_3_5_54_520,"[VP] HandleLocalListOptMenuWinMsg menu_id = %x")
TRACE_MSG(MMIVP_WINTABLE_12068_112_2_18_3_5_54_521,"[VP] HandleLocalListOptMenuWinMsg cur_list_index = %d, locallist_type = %d")
TRACE_MSG(MMIVP_WINTABLE_12103_112_2_18_3_5_54_522,"[VP] HandleLocalListOptMenuWinMsg get file info error")
TRACE_MSG(MMIVP_WINTABLE_12114_112_2_18_3_5_54_523,"[VP] HandleLocalListOptMenuWinMsg cur_list_index = %d, locallist_type = %d")
TRACE_MSG(MMIVP_WINTABLE_12197_112_2_18_3_5_54_524,"[VP] HandleLocalListOptMenuWinMsg delete FAIL")
TRACE_MSG(MMIVP_WINTABLE_12413_112_2_18_3_5_54_525,"[VP] SettingAsScreensaver full_name_ptr = %x, full_name_len = %d")
TRACE_MSG(MMIVP_WINTABLE_12462_112_2_18_3_5_54_526,"[VP] SettingAsVideoWallpaper full_name_ptr = %x, full_name_len = %d")
TRACE_MSG(MMIVP_WINTABLE_12525_112_2_18_3_5_55_527,"[VP] SettingAsPowerOnOffAnimation is_power_on = %d, full_name_ptr = %x,full_name_len = %d")
TRACE_MSG(MMIVP_WINTABLE_12593_112_2_18_3_5_55_528,"[VP] HandleFileOptionWinMsg menu_id = %x")
TRACE_MSG(MMIVP_WINTABLE_12664_112_2_18_3_5_55_529,"[VP] MMIAPIVP_IsLiving result = %d")
TRACE_MSG(MMIVP_WINTABLE_12711_112_2_18_3_5_55_530,"[VP] DisplayVPWinSoftkey s_video_player_control_info.total_time = %d")
TRACE_MSG(MMIVP_WINTABLE_13070_112_2_18_3_5_56_531,"[VP] HandleSIMSelectWinMsg msg_id= 0x%x")
TRACE_MSG(MMIVP_WINTABLE_13163_112_2_18_3_5_56_532,"[VP] HandleTPDownMsg x= %d, y = %d, rect.left = %d,right = %d,top = %d,bottom = %d, s_tp_down_ctrl_id = %d")
TRACE_MSG(MMIVP_WINTABLE_13168_112_2_18_3_5_56_533,"[VP] HandleTPDownMsg s_tp_down_ctrl_id = %d")
TRACE_MSG(MMIVP_WINTABLE_13191_112_2_18_3_5_56_534,"[VP] HandleTPUpMsg s_tp_down_ctrl_id = %d, point.x = %d, y= %d")
TRACE_MSG(MMIVP_WINTABLE_13250_112_2_18_3_5_56_535,"[VP] HandleCtrlTpLongMsg ctrl_id = %d, s_tp_down_ctrl_id = %D")
TRACE_MSG(MMIVP_WINTABLE_13319_112_2_18_3_5_56_536,"[VP] HandleCtrlTpMsg ctrl_id = %d, s_tp_down_ctrl_id = %d")
TRACE_MSG(MMIVP_WINTABLE_13414_112_2_18_3_5_56_537,"[VP] HandleTPMsg msg_id = %x")
TRACE_MSG(MMIVP_WINTABLE_13482_112_2_18_3_5_56_538,"[VP] MMIAPIVP_FillRectWithTransparenceColor before x= %d, y=%d,width = %d, height = %d")
TRACE_MSG(MMIVP_WINTABLE_13495_112_2_18_3_5_56_539,"[VP] MMIAPIVP_FillRectWithTransparenceColor after x= %d, y=%d,width = %d, height = %d")
TRACE_MSG(MMIVP_WINTABLE_13554_112_2_18_3_5_57_540,"[VP] MMIAPIVP_FillRectWithTransparenceColor after x= %d, y=%d,width = %d, height = %d")
TRACE_MSG(MMIVP_WINTABLE_13625_112_2_18_3_5_57_541,"[VP] VP_BTEventNext enter")
TRACE_MSG(MMIVP_WINTABLE_13636_112_2_18_3_5_57_542,"[VP] VP_BTEventPrev enter")
TRACE_MSG(MMIVP_WINTABLE_13647_112_2_18_3_5_57_543,"[VP] VP_BTEventPlayOrPause enter")
TRACE_MSG(MMIVP_WINTABLE_13660_112_2_18_3_5_57_544,"[VP] MMIAPIVP_HandleFileChange s_video_player_control_info.play_type = %d")
TRACE_MSG(MMIVP_WINTABLE_13683_112_2_18_3_5_57_545,"[VP]MMIAPIVP_GetPlayingFilename playingname = %x")
TRACE_MSG(MMIVP_WINTABLE_13702_112_2_18_3_5_57_546,"[VP]:CreateLandscapeThumbnailLayer ")
TRACE_MSG(MMIVP_WINTABLE_13726_112_2_18_3_5_57_547,"[VP]:CreateLandscapeThumbnailLayer success ")
TRACE_MSG(MMIVP_WINTABLE_13749_112_2_18_3_5_57_548,"[VP]:DestroyLandscapeThumbnailLayer  ")
TRACE_MSG(MMIVP_WINTABLE_13768_112_2_18_3_5_57_549,"[VP]:VPThumbnailResetMainLayer  ")
TRACE_MSG(MMIVP_WINTABLE_13784_112_2_18_3_5_57_550,"[VP]:VPThumbnailSetMainLayer  ")
TRACE_MSG(MMIVP_WINTABLE_13804_112_2_18_3_5_57_551,"[VP]:VPThumbnailSetDPLAYERParam   RotateAngle:%d,s_video_player_control_info.cur_angle:%d")
TRACE_MSG(MMIVP_WINTABLE_13857_112_2_18_3_5_57_552,"[VP]:DisplayThumbnail MMIAPIVP_ThumbnailGetAngle !=GUILCD_GetLogicAngle ")
TRACE_MSG(MMIVP_WINTABLE_13868_112_2_18_3_5_57_553,"[VP]:DisplayThumbnail   image_data->coordinate.w:%d,image_data->coordinate.h:%d,image_data->coordinate.trim_x:%d,image_data->coordinate.trim_y:%d,image_data->coordinate.trim_w:%d,image_data->coordinate.trim_h:%d ,rotate:%d")
TRACE_MSG(MMIVP_WINTABLE_13942_112_2_18_3_5_57_554,"[VP]:NotifyDPLAYERGetTumbnail   RotateAngle:%d,s_video_player_control_info.cur_angle:%d")
TRACE_MSG(MMIVP_WINTABLE_13973_112_2_18_3_5_57_555,"[VP]:NotifyDPLAYERGetTumbnail   fail !")
TRACE_MSG(MMIVP_WINTABLE_14034_112_2_18_3_5_58_556,"[VP] HandleBookmarkLongOkOptMenuWinMsg menu_id = %x")
TRACE_MSG(MMIVP_WINTABLE_14039_112_2_18_3_5_58_557,"[VP] HandleBookmarkLongOkOptMenuWinMsg cur_list_index = %d")
TRACE_MSG(MMIVP_WINTABLE_14052_112_2_18_3_5_58_558,"[VP] HandleBookmarkLongOkOptMenuWinMsg no memory")
TRACE_MSG(MMIVP_WINTABLE_14092_112_2_18_3_5_58_559,"[VP] HandleBookmarkLongOkOptMenuWinMsg menu_id = %x")
TRACE_MSG(MMIVP_WINTABLE_14162_112_2_18_3_5_58_560,"[VP] HandleHistoryLongOkOptMenuWinMsg msg_id = %x")
TRACE_MSG(MMIVP_WINTABLE_14191_112_2_18_3_5_58_561,"[VP] HandleHistoryLongOkOptMenuWinMsg menu_id = %x")
TRACE_MSG(MMIVP_WINTABLE_14211_112_2_18_3_5_58_562,"[VP] HandleHistoryLongOkOptMenuWinMsg no memory")
TRACE_MSG(MMIVP_WINTABLE_14275_112_2_18_3_5_58_563,"[VP] HandleHistoryLongOkOptMenuWinMsg menu_id = %x")
TRACE_MSG(MMIVP_WINTABLE_14360_112_2_18_3_5_58_564,"[VP] HandleLocalListLongOkOptMenuWinMsg msg_id = %x, locallist_type = %d")
TRACE_MSG(MMIVP_WINTABLE_14406_112_2_18_3_5_58_565,"[VP] HandleLocalListLongOkOptMenuWinMsg menu_id = %x")
TRACE_MSG(MMIVP_WINTABLE_14411_112_2_18_3_5_58_566,"[VP] HandleLocalListLongOkOptMenuWinMsg cur_list_index = %d, locallist_type = %d")
TRACE_MSG(MMIVP_WINTABLE_14445_112_2_18_3_5_58_567,"[VP] HandleLocalListLongOkOptMenuWinMsg get file info error")
TRACE_MSG(MMIVP_WINTABLE_14456_112_2_18_3_5_58_568,"[VP] HandleLocalListLongOkOptMenuWinMsg cur_list_index = %d, locallist_type = %d")
TRACE_MSG(MMIVP_WINTABLE_14539_112_2_18_3_5_59_569,"[VP] HandleLocalListLongOkOptMenuWinMsg delete FAIL")
TRACE_MSG(MMIVP_WINTABLE_14668_112_2_18_3_5_59_570,"[VP]:NetLinkButtonCallback")
TRACE_MSG(MMIVP_WINTABLE_14701_112_2_18_3_5_59_571,"[VP]:NetLinkButtonCallback")
END_TRACE_MAP(MMI_APP_VIDEO_PLAYER_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_VIDEO_PLAYER_TRC_H_

