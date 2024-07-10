/******************************************************************************
 ** File Name:      ms_ref_streaming_codec_trc.h                                         *
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
//trace_id:70
#ifndef _MS_REF_STREAMING_CODEC_TRC_H_
#define _MS_REF_STREAMING_CODEC_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define DUMMY_82_112_2_18_1_54_31_0 "wxf group 0."
#define DUMMY_88_112_2_18_1_54_31_1 "wxf group 1."
#define DUMMY_95_112_2_18_1_54_31_2 "wxf group 2."
#define DUMMY_149_112_2_18_1_54_31_3 "wxf: AAC_callback."
#define DUMMY_153_112_2_18_1_54_31_4 "wxf: s_cur_len_pos is over aac_data_length."
#define DUMMY_163_112_2_18_1_54_31_5 "wxf: last_len is over."
#define DUMMY_179_112_2_18_1_54_31_6 "wxf: puiDataLength <= AAC_SRC_BUF_SIZE."
#define DUMMY_203_112_2_18_1_54_31_7 "wxf: AAC_ConfigHeader=%d."
#define DUMMY_207_112_2_18_1_54_31_8 "wxf: Open_AAC_Codec."
#define DUMMY_218_112_2_18_1_54_31_9 "wxf: AUDIO_CreateStreamBufferHandle=%d"
#define DUMMY_225_112_2_18_1_54_31_10 "wxf: test_aac_audio_data."
#define DUMMY_228_112_2_18_1_54_31_11 "wxf: AUDIO_Play."
#define DUMMY_280_112_2_18_1_54_31_12 "wxf: the CAL_VideoDecInit finish!"
#define DUMMY_285_112_2_18_1_54_31_13 "wxf: the SCI_ALLOC_APP(500*1024) error!"
#define DUMMY_290_112_2_18_1_54_31_14 "wxf: the CAL_VideoDecMallocBuffer finish!"
#define DUMMY_299_112_2_18_1_54_31_15 "wxf: the Video_Display_Open finish!"
#define DUMMY_313_112_2_18_1_54_31_16 "wxf: the Video_Display_Setparam finish!"
#define DUMMY_347_112_2_18_1_54_31_17 "wxf: the CAL_VideoDecDecodeAndLockFrame error!"
#define DUMMY_371_112_2_18_1_54_31_18 "wxf: the Video_Display_Oneframe finish!"
#define DUMMY_377_112_2_18_1_54_31_19 "wxf: the CAL_VideoDecUnlockFrame finish!"
#define SM_AAC_CODEC_70_112_2_18_1_54_32_20 "sm_codec: current play frame number=%d"
#define SM_AAC_CODEC_73_112_2_18_1_54_32_21 "sm_codec: AAC_PlayCallback, Copy new data! framenum=%d"
#define SM_AAC_CODEC_88_112_2_18_1_54_32_22 "sm_codec: AAC_PlayCallback, None data to play!!"
#define SM_AAC_CODEC_100_112_2_18_1_54_32_23 "sm_codec: AAC_PlayCallback, cur data length = %d"
#define SM_AAC_CODEC_106_112_2_18_1_54_32_24 "sm_codec: puiDataLength <= AAC_SRC_BUF_SIZE."
#define SM_AAC_CODEC_112_112_2_18_1_54_32_25 "sm_codec: AMR_PlayCallBack PutSema request the new data!"
#define SM_AAC_CODEC_144_112_2_18_1_54_32_26 "sm_codec: head data length=%d"
#define SM_AAC_CODEC_147_112_2_18_1_54_32_27 "sm_codec: AAC_AudioCodecInit AAC_ConfigHeader ERROR=%d"
#define SM_AAC_CODEC_154_112_2_18_1_54_32_28 "sm_codec: AAC_AudioCodecInit sample_rate=%d, channel_count=%d"
#define SM_AAC_CODEC_202_112_2_18_1_54_32_29 "sm_codec: the frame number = %d"
#define SM_AAC_CODEC_215_112_2_18_1_54_32_30 "sm_codec: AAC_AudioPutData s_aacdata_allframe_size=%d"
#define SM_AAC_CODEC_235_112_2_18_1_54_32_31 "sm_codec: AAC_AudioPlay! result=%d"
#define SM_AAC_CODEC_252_112_2_18_1_54_32_32 "sm_codec: AAC_AudioStop! result=%d"
#define SM_AAC_CODEC_272_112_2_18_1_54_32_33 "sm_codec: AAC_AudioPause! result=%d"
#define SM_AAC_CODEC_293_112_2_18_1_54_32_34 "sm_codec: AAC_AudioResume! result=%d"
#define SM_AAC_CODEC_306_112_2_18_1_54_32_35 "sm_codec: AAC_AudioClose"
#define SM_AMR_CODEC_88_112_2_18_1_54_32_36 "sm_amr: current play frame num=%d, newdata=%d"
#define SM_AMR_CODEC_102_112_2_18_1_54_32_37 "sm_amr: AMR_PlayCallBack None data!"
#define SM_AMR_CODEC_116_112_2_18_1_54_32_38 "sm_amr: the amr data is error, i_len=%d, s_amrframe_play_size=%d"
#define SM_AMR_CODEC_140_112_2_18_1_54_32_39 "sm_amr: AMR_PlayCallBack PutSema request the new data!"
#define SM_AMR_CODEC_158_112_2_18_1_54_33_40 "sm_amr: AMR_AudioOpen!!!"
#define SM_AMR_CODEC_185_112_2_18_1_54_33_41 "sm_amr: AMR_AudioPutData allsize=%d"
#define SM_AMR_CODEC_200_112_2_18_1_54_33_42 "sm_amr: entry AMR_AudioPlay !!!"
#define SM_AMR_CODEC_242_112_2_18_1_54_33_43 "sm_amr: entry AMR_AudioStop !!!"
#define SM_AMR_CODEC_256_112_2_18_1_54_33_44 "sm_amr: entry AMR_AudioPause !!!"
#define SM_AMR_CODEC_282_112_2_18_1_54_33_45 "sm_amr: entry AMR_AudioResume !!!"
#define SM_AMR_CODEC_318_112_2_18_1_54_33_46 "sm_amr: AMR_AudioClose!!!"
#define SM_AUDIO_CODEC_107_112_2_18_1_54_33_47 "sm_audio: The Identify = %d"
#define SM_AUDIO_CODEC_118_112_2_18_1_54_33_48 "sm_audio: The Identify = %d"
#define SM_AUDIO_CODEC_153_112_2_18_1_54_33_49 "SM_AudioDecSendFrameBuffer.half position,data request ,frame index=%d"
#define SM_AUDIO_CODEC_190_112_2_18_1_54_33_50 "SM_AudioDecSendFrameBuffer.switch buffer node,frame index=%d"
#define SM_AUDIO_CODEC_256_112_2_18_1_54_34_51 "SM_AudioDecSendFrameBuffer.send to dsp another times ,frame index=%d"
#define SM_AUDIO_CODEC_293_112_2_18_1_54_34_52 "SM_AudioDecSendFrameBuffer .NO DATA"
#define SM_AUDIO_CODEC_320_112_2_18_1_54_34_53 "sm_audio: SM_AudioDecInputData, First data input!"
#define SM_AUDIO_CODEC_328_112_2_18_1_54_34_54 "sm_audio: SM_AudioDecInputData, write first data=%x,len=%d"
#define SM_AUDIO_CODEC_353_112_2_18_1_54_34_55 "sm_audio: SM_AudioDecInputData, AMR_AudioPutData and AudioPlay, g_audioType=%d"
#define SM_AUDIO_CODEC_465_112_2_18_1_54_34_56 "sm_audio: SM_AudioDecResume, g_audioType=%d"
#define SM_AUDIO_CODEC_527_112_2_18_1_54_34_57 "sm_audio: SM_AudioDecOpen, header cfg first data=%x,len=%d"
#define SM_AUDIO_CODEC_567_112_2_18_1_54_34_58 "sm_audio: SM_AudioDecClose, g_audioType=%d"
#define SM_CODEC_TASK_125_112_2_18_1_54_35_59 "sm_codec: Create PVCodec Task failed!"
#define SM_VIDEO_CODEC_79_112_2_18_1_54_35_60 "sm_video:the SM_VideoDecOpen video_type=%d,headerlen=%d"
#define SM_VIDEO_CODEC_100_112_2_18_1_54_35_61 "sm_video:the VideoCodecTask_Init success!"
#define SM_VIDEO_CODEC_104_112_2_18_1_54_35_62 "sm_video:the videocodecmemory.buffer fail!"
#define SM_VIDEO_CODEC_134_112_2_18_1_54_35_63 "sm_video:the CAL_VideoDecInit fail. result=%d"
#define SM_VIDEO_CODEC_141_112_2_18_1_54_35_64 "sm_video:the videoframebuffer.buffer fail!"
#define SM_VIDEO_CODEC_150_112_2_18_1_54_35_65 "sm_video:the CAL_VideoDecMallocBuffer fail! result=%d"
#define SM_VIDEO_CODEC_159_112_2_18_1_54_35_66 "sm_codec:the SM_VideoDecOpen finished!!!Exit"
#define SM_VIDEO_CODEC_180_112_2_18_1_54_35_67 "sm_video: SM_VideoSetDisplayParam. rorate=%d"
#define SM_VIDEO_CODEC_183_112_2_18_1_54_35_68 "sm_video:the Video_Display_Setparam fail!"
#define SM_VIDEO_CODEC_211_112_2_18_1_54_36_69 "sm_video: SM_VideoDecode. Video Data Frame Decode, Len==0"
#define SM_VIDEO_CODEC_231_112_2_18_1_54_36_70 "sm_video: SM_VideoShowFrame. isDrop=%d, video_can_show=%d"
#define SM_VIDEO_CODEC_239_112_2_18_1_54_36_71 "sm_video:the SM_VideoShowFrame is_transposed =%d!"
#define SM_VIDEO_CODEC_281_112_2_18_1_54_36_72 "sm_video: SM_VideoDecClose"
#define SM_VIDEO_CODEC_301_112_2_18_1_54_36_73 "sm_video:SM_VideoDisplayOpen!"
#define SM_VIDEO_CODEC_305_112_2_18_1_54_36_74 "sm_video:the Video_Display_Open fail!result=%d"
#define SM_VIDEO_CODEC_332_112_2_18_1_54_36_75 "sm_video: SM_VideoDecode. Video Data Frame Decode, size = %d"
#define SM_VIDEO_CODEC_335_112_2_18_1_54_36_76 "sm_video:the CAL_VideoDecDecodeAndLockFrame frameEffective =%d,result=%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_STREAMING_CODEC_TRC)
TRACE_MSG(DUMMY_82_112_2_18_1_54_31_0,"wxf group 0.")
TRACE_MSG(DUMMY_88_112_2_18_1_54_31_1,"wxf group 1.")
TRACE_MSG(DUMMY_95_112_2_18_1_54_31_2,"wxf group 2.")
TRACE_MSG(DUMMY_149_112_2_18_1_54_31_3,"wxf: AAC_callback.")
TRACE_MSG(DUMMY_153_112_2_18_1_54_31_4,"wxf: s_cur_len_pos is over aac_data_length.")
TRACE_MSG(DUMMY_163_112_2_18_1_54_31_5,"wxf: last_len is over.")
TRACE_MSG(DUMMY_179_112_2_18_1_54_31_6,"wxf: puiDataLength <= AAC_SRC_BUF_SIZE.")
TRACE_MSG(DUMMY_203_112_2_18_1_54_31_7,"wxf: AAC_ConfigHeader=%d.")
TRACE_MSG(DUMMY_207_112_2_18_1_54_31_8,"wxf: Open_AAC_Codec.")
TRACE_MSG(DUMMY_218_112_2_18_1_54_31_9,"wxf: AUDIO_CreateStreamBufferHandle=%d")
TRACE_MSG(DUMMY_225_112_2_18_1_54_31_10,"wxf: test_aac_audio_data.")
TRACE_MSG(DUMMY_228_112_2_18_1_54_31_11,"wxf: AUDIO_Play.")
TRACE_MSG(DUMMY_280_112_2_18_1_54_31_12,"wxf: the CAL_VideoDecInit finish!")
TRACE_MSG(DUMMY_285_112_2_18_1_54_31_13,"wxf: the SCI_ALLOC_APP(500*1024) error!")
TRACE_MSG(DUMMY_290_112_2_18_1_54_31_14,"wxf: the CAL_VideoDecMallocBuffer finish!")
TRACE_MSG(DUMMY_299_112_2_18_1_54_31_15,"wxf: the Video_Display_Open finish!")
TRACE_MSG(DUMMY_313_112_2_18_1_54_31_16,"wxf: the Video_Display_Setparam finish!")
TRACE_MSG(DUMMY_347_112_2_18_1_54_31_17,"wxf: the CAL_VideoDecDecodeAndLockFrame error!")
TRACE_MSG(DUMMY_371_112_2_18_1_54_31_18,"wxf: the Video_Display_Oneframe finish!")
TRACE_MSG(DUMMY_377_112_2_18_1_54_31_19,"wxf: the CAL_VideoDecUnlockFrame finish!")
TRACE_MSG(SM_AAC_CODEC_70_112_2_18_1_54_32_20,"sm_codec: current play frame number=%d")
TRACE_MSG(SM_AAC_CODEC_73_112_2_18_1_54_32_21,"sm_codec: AAC_PlayCallback, Copy new data! framenum=%d")
TRACE_MSG(SM_AAC_CODEC_88_112_2_18_1_54_32_22,"sm_codec: AAC_PlayCallback, None data to play!!")
TRACE_MSG(SM_AAC_CODEC_100_112_2_18_1_54_32_23,"sm_codec: AAC_PlayCallback, cur data length = %d")
TRACE_MSG(SM_AAC_CODEC_106_112_2_18_1_54_32_24,"sm_codec: puiDataLength <= AAC_SRC_BUF_SIZE.")
TRACE_MSG(SM_AAC_CODEC_112_112_2_18_1_54_32_25,"sm_codec: AMR_PlayCallBack PutSema request the new data!")
TRACE_MSG(SM_AAC_CODEC_144_112_2_18_1_54_32_26,"sm_codec: head data length=%d")
TRACE_MSG(SM_AAC_CODEC_147_112_2_18_1_54_32_27,"sm_codec: AAC_AudioCodecInit AAC_ConfigHeader ERROR=%d")
TRACE_MSG(SM_AAC_CODEC_154_112_2_18_1_54_32_28,"sm_codec: AAC_AudioCodecInit sample_rate=%d, channel_count=%d")
TRACE_MSG(SM_AAC_CODEC_202_112_2_18_1_54_32_29,"sm_codec: the frame number = %d")
TRACE_MSG(SM_AAC_CODEC_215_112_2_18_1_54_32_30,"sm_codec: AAC_AudioPutData s_aacdata_allframe_size=%d")
TRACE_MSG(SM_AAC_CODEC_235_112_2_18_1_54_32_31,"sm_codec: AAC_AudioPlay! result=%d")
TRACE_MSG(SM_AAC_CODEC_252_112_2_18_1_54_32_32,"sm_codec: AAC_AudioStop! result=%d")
TRACE_MSG(SM_AAC_CODEC_272_112_2_18_1_54_32_33,"sm_codec: AAC_AudioPause! result=%d")
TRACE_MSG(SM_AAC_CODEC_293_112_2_18_1_54_32_34,"sm_codec: AAC_AudioResume! result=%d")
TRACE_MSG(SM_AAC_CODEC_306_112_2_18_1_54_32_35,"sm_codec: AAC_AudioClose")
TRACE_MSG(SM_AMR_CODEC_88_112_2_18_1_54_32_36,"sm_amr: current play frame num=%d, newdata=%d")
TRACE_MSG(SM_AMR_CODEC_102_112_2_18_1_54_32_37,"sm_amr: AMR_PlayCallBack None data!")
TRACE_MSG(SM_AMR_CODEC_116_112_2_18_1_54_32_38,"sm_amr: the amr data is error, i_len=%d, s_amrframe_play_size=%d")
TRACE_MSG(SM_AMR_CODEC_140_112_2_18_1_54_32_39,"sm_amr: AMR_PlayCallBack PutSema request the new data!")
TRACE_MSG(SM_AMR_CODEC_158_112_2_18_1_54_33_40,"sm_amr: AMR_AudioOpen!!!")
TRACE_MSG(SM_AMR_CODEC_185_112_2_18_1_54_33_41,"sm_amr: AMR_AudioPutData allsize=%d")
TRACE_MSG(SM_AMR_CODEC_200_112_2_18_1_54_33_42,"sm_amr: entry AMR_AudioPlay !!!")
TRACE_MSG(SM_AMR_CODEC_242_112_2_18_1_54_33_43,"sm_amr: entry AMR_AudioStop !!!")
TRACE_MSG(SM_AMR_CODEC_256_112_2_18_1_54_33_44,"sm_amr: entry AMR_AudioPause !!!")
TRACE_MSG(SM_AMR_CODEC_282_112_2_18_1_54_33_45,"sm_amr: entry AMR_AudioResume !!!")
TRACE_MSG(SM_AMR_CODEC_318_112_2_18_1_54_33_46,"sm_amr: AMR_AudioClose!!!")
TRACE_MSG(SM_AUDIO_CODEC_107_112_2_18_1_54_33_47,"sm_audio: The Identify = %d")
TRACE_MSG(SM_AUDIO_CODEC_118_112_2_18_1_54_33_48,"sm_audio: The Identify = %d")
TRACE_MSG(SM_AUDIO_CODEC_153_112_2_18_1_54_33_49,"SM_AudioDecSendFrameBuffer.half position,data request ,frame index=%d")
TRACE_MSG(SM_AUDIO_CODEC_190_112_2_18_1_54_33_50,"SM_AudioDecSendFrameBuffer.switch buffer node,frame index=%d")
TRACE_MSG(SM_AUDIO_CODEC_256_112_2_18_1_54_34_51,"SM_AudioDecSendFrameBuffer.send to dsp another times ,frame index=%d")
TRACE_MSG(SM_AUDIO_CODEC_293_112_2_18_1_54_34_52,"SM_AudioDecSendFrameBuffer .NO DATA")
TRACE_MSG(SM_AUDIO_CODEC_320_112_2_18_1_54_34_53,"sm_audio: SM_AudioDecInputData, First data input!")
TRACE_MSG(SM_AUDIO_CODEC_328_112_2_18_1_54_34_54,"sm_audio: SM_AudioDecInputData, write first data=%x,len=%d")
TRACE_MSG(SM_AUDIO_CODEC_353_112_2_18_1_54_34_55,"sm_audio: SM_AudioDecInputData, AMR_AudioPutData and AudioPlay, g_audioType=%d")
TRACE_MSG(SM_AUDIO_CODEC_465_112_2_18_1_54_34_56,"sm_audio: SM_AudioDecResume, g_audioType=%d")
TRACE_MSG(SM_AUDIO_CODEC_527_112_2_18_1_54_34_57,"sm_audio: SM_AudioDecOpen, header cfg first data=%x,len=%d")
TRACE_MSG(SM_AUDIO_CODEC_567_112_2_18_1_54_34_58,"sm_audio: SM_AudioDecClose, g_audioType=%d")
TRACE_MSG(SM_CODEC_TASK_125_112_2_18_1_54_35_59,"sm_codec: Create PVCodec Task failed!")
TRACE_MSG(SM_VIDEO_CODEC_79_112_2_18_1_54_35_60,"sm_video:the SM_VideoDecOpen video_type=%d,headerlen=%d")
TRACE_MSG(SM_VIDEO_CODEC_100_112_2_18_1_54_35_61,"sm_video:the VideoCodecTask_Init success!")
TRACE_MSG(SM_VIDEO_CODEC_104_112_2_18_1_54_35_62,"sm_video:the videocodecmemory.buffer fail!")
TRACE_MSG(SM_VIDEO_CODEC_134_112_2_18_1_54_35_63,"sm_video:the CAL_VideoDecInit fail. result=%d")
TRACE_MSG(SM_VIDEO_CODEC_141_112_2_18_1_54_35_64,"sm_video:the videoframebuffer.buffer fail!")
TRACE_MSG(SM_VIDEO_CODEC_150_112_2_18_1_54_35_65,"sm_video:the CAL_VideoDecMallocBuffer fail! result=%d")
TRACE_MSG(SM_VIDEO_CODEC_159_112_2_18_1_54_35_66,"sm_codec:the SM_VideoDecOpen finished!!!Exit")
TRACE_MSG(SM_VIDEO_CODEC_180_112_2_18_1_54_35_67,"sm_video: SM_VideoSetDisplayParam. rorate=%d")
TRACE_MSG(SM_VIDEO_CODEC_183_112_2_18_1_54_35_68,"sm_video:the Video_Display_Setparam fail!")
TRACE_MSG(SM_VIDEO_CODEC_211_112_2_18_1_54_36_69,"sm_video: SM_VideoDecode. Video Data Frame Decode, Len==0")
TRACE_MSG(SM_VIDEO_CODEC_231_112_2_18_1_54_36_70,"sm_video: SM_VideoShowFrame. isDrop=%d, video_can_show=%d")
TRACE_MSG(SM_VIDEO_CODEC_239_112_2_18_1_54_36_71,"sm_video:the SM_VideoShowFrame is_transposed =%d!")
TRACE_MSG(SM_VIDEO_CODEC_281_112_2_18_1_54_36_72,"sm_video: SM_VideoDecClose")
TRACE_MSG(SM_VIDEO_CODEC_301_112_2_18_1_54_36_73,"sm_video:SM_VideoDisplayOpen!")
TRACE_MSG(SM_VIDEO_CODEC_305_112_2_18_1_54_36_74,"sm_video:the Video_Display_Open fail!result=%d")
TRACE_MSG(SM_VIDEO_CODEC_332_112_2_18_1_54_36_75,"sm_video: SM_VideoDecode. Video Data Frame Decode, size = %d")
TRACE_MSG(SM_VIDEO_CODEC_335_112_2_18_1_54_36_76,"sm_video:the CAL_VideoDecDecodeAndLockFrame frameEffective =%d,result=%d")
END_TRACE_MAP(MS_REF_STREAMING_CODEC_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_STREAMING_CODEC_TRC_H_

