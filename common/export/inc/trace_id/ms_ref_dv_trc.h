/******************************************************************************
 ** File Name:      ms_ref_dv_trc.h                                         *
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
//trace_id:45
#ifndef _MS_REF_DV_TRC_H_
#define _MS_REF_DV_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define H264_DEC_V0_288_112_2_18_1_27_21_0 "h264_dec task, MP4_DECODER_PAUSE_EVENT"
#define H264_DEC_V0_345_112_2_18_1_27_21_1 "h264_dec,video is too slow,seek one I VOP,syn_time[%d]"
#define H264_DEC_V0_349_112_2_18_1_27_21_2 "h264_dec,video is too slow, but no I VOP in next 1000ms. syn_time[%d]"
#define H264_DEC_V0_402_112_2_18_1_27_21_3 "h264_dec, DecoderTaskEntry, failed to call set event"
#define H264_DEC_V0_418_112_2_18_1_27_21_4 "h264_dec, disp_skip=%d, time_offset_a_v %d, b_expected_IVOP %d"
#define H264_DEC_V0_423_112_2_18_1_27_21_5 "h264_dec, _Peek_NextDispFrameContain false,wait for next time"
#define H264_DEC_V0_453_112_2_18_1_27_21_6 "h264_dec, DecoderTaskEntry, frame_width=%d, frame_height=%d, extra_buffer_size=%d,"
#define H264_DEC_V0_457_112_2_18_1_27_21_7 "h264_dec, DecoderTaskEntry, H264DecMemSizeCal return error"
#define H264_DEC_V0_469_112_2_18_1_27_21_8 "h264_dec, DecoderTaskEntry, frame_width=%d, frame_height=%d, Error or unsupport"
#define H264_DEC_V0_479_112_2_18_1_27_21_9 "h264_dec, DecoderTaskEntry, p_decoder_buffer_extra == PNULL, return MMDEC_NOT_SUPPORTED"
#define H264_DEC_V0_491_112_2_18_1_27_21_10 "h264_dec.c, DecoderTaskEntry, H264DecMemInit failed!"
#define H264_DEC_V0_500_112_2_18_1_27_21_11 "h264_dec, DecoderTaskEntry, malloc OK, frame_width=%d, frame_height=%d, extra_buffer_size=%d,"
#define H264_DEC_V0_539_112_2_18_1_27_22_12 "h264_dec,[w,h]=[%d,%d],result=%d"
#define H264_DEC_V0_556_112_2_18_1_27_22_13 "h264_dec task, decode_result, MMDEC_OUTPUT_BUFFER_OVERFLOW"
#define H264_DEC_V0_625_112_2_18_1_27_22_14 "h264_dec, block_new failed."
#define H264_DEC_V0_638_112_2_18_1_27_22_15 "h264_dec task, decode_result, MMDEC_ERROR"
#define H264_DEC_V0_644_112_2_18_1_27_22_16 "h264_dec task, decode_result, MMDEC_PARAM_ERROR"
#define H264_DEC_V0_650_112_2_18_1_27_22_17 "h264_dec task, decode_result, MMDEC_MEMORY_ERROR"
#define H264_DEC_V0_654_112_2_18_1_27_22_18 "h264_dec task, decode_result, MMDEC_INVALID_STATUS"
#define H264_DEC_V0_658_112_2_18_1_27_22_19 "h264_dec task, decode_result, MMDEC_STREAM_ERROR"
#define H264_DEC_V0_664_112_2_18_1_27_22_20 "h264_dec task, decode_result, MMDEC_OUTPUT_BUFFER_OVERFLOW"
#define H264_DEC_V0_668_112_2_18_1_27_22_21 "h264_dec task, decode_result, MMDEC_HW_ERROR"
#define H264_DEC_V0_673_112_2_18_1_27_22_22 "h264_dec task, decode_result, MMDEC_NOT_SUPPORTED"
#define H264_DEC_V0_678_112_2_18_1_27_22_23 "h264_dec task, decode_result, MMDEC_FRAME_SEEK_IVOP"
#define H264_DEC_V0_682_112_2_18_1_27_22_24 "h264_dec task, decode_result=%d, return err!"
#define H264_DEC_V0_698_112_2_18_1_27_22_25 "mpeg4 decoder task, encount undefined event,i_event_flag[0x%x],status[0x%x]"
#define H264_DEC_V0_708_112_2_18_1_27_22_26 "h264_dec, Active()"
#define H264_DEC_V0_721_112_2_18_1_27_22_27 "h264_dec, Deactive()"
#define H264_DEC_V0_735_112_2_18_1_27_22_28 "h264_dec.c,close,status error!"
#define H264_DEC_V0_741_112_2_18_1_27_22_29 "h264_dec.c, open, movie size is too large."
#define H264_DEC_V0_777_112_2_18_1_27_22_30 "failed to call H264DecInit()"
#define H264_DEC_V0_791_112_2_18_1_27_22_31 "h264_dec, Open(), failed to create event"
#define H264_DEC_V0_816_112_2_18_1_27_22_32 "h264_dec, Open(), failed to create task"
#define H264_DEC_V0_871_112_2_18_1_27_22_33 "h264_dec, close(),status[0x%x]"
#define H264_DEC_V0_875_112_2_18_1_27_22_34 "h264_dec.c,close,status error!"
#define H264_DEC_V0_936_112_2_18_1_27_22_35 "h264_dec,p_buffer=0x%x, i_buffer=%d, depth=%d"
#define H264_DEC_V0_946_112_2_18_1_27_22_36 "h264_dec, Send(), failed to call set event"
#define H264_DEC_V0_952_112_2_18_1_27_22_37 "h264_dec, send(),fail"
#define H264_DEC_V0_983_112_2_18_1_27_22_38 "h264_dec, Resume, failed to call set event"
#define H264_DEC_V0_1028_112_2_18_1_27_23_39 "h264_dec, Pause(),status[0x%x]"
#define H264_DEC_V0_1038_112_2_18_1_27_23_40 "h264_dec, Pause()"
#define H264_DEC_V0_1061_112_2_18_1_27_23_41 "h264_dec, Resume(),status[0x%x]"
#define H264_DEC_V0_1074_112_2_18_1_27_23_42 "__NotifyH264YUVBufferIsFree, PNULL BUFFER."
#define H264_DEC_V0_1111_112_2_18_1_27_23_43 "h264_dec.c, SetConfig, re sync fail,status[%d]"
#define H264_DEC_V0_1125_112_2_18_1_27_23_44 "h264_dec CMD_DEC_ROTATION set %d"
#define H264_DEC_V0_1137_112_2_18_1_27_23_45 "h264_dec CMD_DEC_ADD_PLUG set,cfg=0x%x"
#define H264_DEC_V0_1168_112_2_18_1_27_23_46 "h264_dec, SetConfig, invalid cmd"
#define H264_DEC_V0_1198_112_2_18_1_27_23_47 "h264_dec, GetInfo, invalid cmd"
#define H264_DEC_V1_198_112_2_18_1_27_23_48 "h264_dec task, error happen when waiting for event"
#define H264_DEC_V1_234_112_2_18_1_27_23_49 "H264DecoderTaskEntry,cur_time=%d,first_pos=%d"
#define H264_DEC_V1_261_112_2_18_1_27_23_50 "h264_dec,video is too slow,seek one I VOP,syn_time[%d]"
#define H264_DEC_V1_361_112_2_18_1_27_24_51 "h264_dec, cur_time=%d, bef_interval=%d, aft_interval=%d"
#define H264_DEC_V1_402_112_2_18_1_27_24_52 "h264_dec, DecoderTaskEntry, failed to call set event"
#define H264_DEC_V1_417_112_2_18_1_27_24_53 "H264DecoderTaskEntry,expect_I=%d,disp_skip=%d,i_frame=%d"
#define H264_DEC_V1_426_112_2_18_1_27_24_54 "h264_dec, yuv buffer overflow!"
#define H264_DEC_V1_432_112_2_18_1_27_24_55 "h264_dec, DecoderTaskEntry, failed to call set event"
#define H264_DEC_V1_459_112_2_18_1_27_24_56 "h264_dec, DecoderTaskEntry, frame_width=%d, frame_height=%d, extra_buffer_size=%d,"
#define H264_DEC_V1_463_112_2_18_1_27_24_57 "h264_dec, DecoderTaskEntry, H264DecMemSizeCal return error"
#define H264_DEC_V1_474_112_2_18_1_27_24_58 "h264_dec, DecoderTaskEntry, frame_width=%d, frame_height=%d, Error or unsupport"
#define H264_DEC_V1_483_112_2_18_1_27_24_59 "h264_dec, DecoderTaskEntry, p_decoder_buffer_extra == PNULL, return MMDEC_NOT_SUPPORTED"
#define H264_DEC_V1_494_112_2_18_1_27_24_60 "h264_dec, DecoderTaskEntry, H264DecMemInit failed!"
#define H264_DEC_V1_502_112_2_18_1_27_24_61 "h264_dec, DecoderTaskEntry, malloc OK, frame_width=%d, frame_height=%d, extra_buffer_size=%d,"
#define H264_DEC_V1_549_112_2_18_1_27_24_62 "h264_dec, H264DecDecode time=%d,ret=%d, size = %d, pts = %d,exp_IVOP=%d,display_skip=%d"
#define H264_DEC_V1_624_112_2_18_1_27_24_63 "h264_dec, block_new failed."
#define H264_DEC_V1_634_112_2_18_1_27_24_64 "h264_dec task, stream err,ret=%d"
#define H264_DEC_V1_647_112_2_18_1_27_24_65 "h264_dec task, unsupport,ret=%d"
#define H264_DEC_V1_659_112_2_18_1_27_24_66 "h264_dec task, dec err,ret=%d"
#define H264_DEC_V1_679_112_2_18_1_27_24_67 "h264 decoder task, encount undefined event,i_event_flag[0x%x],status[0x%x]"
#define H264_DEC_V1_692_112_2_18_1_27_24_68 "h264_dec, Active()"
#define H264_DEC_V1_705_112_2_18_1_27_24_69 "h264_dec, Deactive()"
#define H264_DEC_V1_717_112_2_18_1_27_24_70 "h264_dec, Open()"
#define H264_DEC_V1_721_112_2_18_1_27_24_71 "h264_dec.c,close,status error!"
#define H264_DEC_V1_739_112_2_18_1_27_24_72 "h264_dec.c, Open, movie size is too large."
#define H264_DEC_V1_755_112_2_18_1_27_24_73 "h264_dec.c,mutex_Init error!"
#define H264_DEC_V1_760_112_2_18_1_27_24_74 "h264_dec.c,MpegInitYUVCtrl fail"
#define H264_DEC_V1_768_112_2_18_1_27_24_75 "h264_dec, Open(), failed to create event"
#define H264_DEC_V1_793_112_2_18_1_27_25_76 "h264_dec, Open(), failed to create task"
#define H264_DEC_V1_808_112_2_18_1_27_25_77 "failed to call MPEG4_DecodeInit()"
#define H264_DEC_V1_826_112_2_18_1_27_25_78 "h264_dec.c, open, err!"
#define H264_DEC_V1_854_112_2_18_1_27_25_79 "h264_dec, close(),status[0x%x]"
#define H264_DEC_V1_858_112_2_18_1_27_25_80 "h264_dec.c,close,status error!"
#define H264_DEC_V1_918_112_2_18_1_27_25_81 "h264_dec, send()"
#define H264_DEC_V1_926_112_2_18_1_27_25_82 "h264_dec, Send(), failed to call set event"
#define H264_DEC_V1_932_112_2_18_1_27_25_83 "h264_dec, send(),fail"
#define H264_DEC_V1_938_112_2_18_1_27_25_84 "h264_dec, send(), frame len is zero."
#define H264_DEC_V1_948_112_2_18_1_27_25_85 "h264_dec, Start(),status[0x%x]"
#define H264_DEC_V1_956_112_2_18_1_27_25_86 "h264_dec, Resume, failed to call set event"
#define H264_DEC_V1_968_112_2_18_1_27_25_87 "h264_dec, Stop(),status[0x%x]"
#define H264_DEC_V1_992_112_2_18_1_27_25_88 "h264_dec, Pause(),status[0x%x]"
#define H264_DEC_V1_999_112_2_18_1_27_25_89 "h264_dec, Pause()"
#define H264_DEC_V1_1010_112_2_18_1_27_25_90 "h264_dec, Resume()"
#define H264_DEC_V1_1018_112_2_18_1_27_25_91 "h264_dec, Resume, failed to call set event"
#define H264_DEC_V1_1064_112_2_18_1_27_25_92 "h264_dec, SetConfig, re sync OK"
#define H264_DEC_V1_1068_112_2_18_1_27_25_93 "h264_dec.c, SetConfig, re sync fail,status[%d]"
#define H264_DEC_V1_1080_112_2_18_1_27_25_94 "h264_dec, SetConfig, playback_mode = MODE_SYNC"
#define H264_DEC_V1_1086_112_2_18_1_27_25_95 "h264_dec, SetConfig, playback_mode = MODE_RATE"
#define H264_DEC_V1_1091_112_2_18_1_27_25_96 "h264_dec, SetConfig, b_skip_display = SCI_TRUE"
#define H264_DEC_V1_1097_112_2_18_1_27_25_97 "h264_dec CMD_DEC_ADD_PLUG set,cfg=0x%x, plug_mem_size=%d"
#define H264_DEC_V1_1128_112_2_18_1_27_25_98 "h264_dec.c, SetConfig, invalid cmd"
#define H264_DEC_V1_1152_112_2_18_1_27_25_99 "h264_dec.c, GetInfo, invalid cmd"
#define H264_DEC_V1_1162_112_2_18_1_27_25_100 "h264 dec,GetQueueLen[%d]"
#define H264_DEC_V1_1169_112_2_18_1_27_25_101 "MpegInitYUVCtrl, len 0x%x"
#define H264_DEC_V1_1181_112_2_18_1_27_25_102 "MpegInitYUVCtrl,g_frameYUVCtr[%d],malloc fail!"
#define H264_DEC_V1_1187_112_2_18_1_27_25_103 "MpegInitYUVCtrl, yuv ptr 0x%x, yuv len 0x%x"
#define H264_DEC_V1_1197_112_2_18_1_27_25_104 "MpegReleaseYUVBuffer"
#define H264_DEC_V1_1223_112_2_18_1_27_25_105 "MpegSetYUVDataFlag,pframe NULL!"
#define H264_DEC_V1_1238_112_2_18_1_27_25_106 "MpegSetYUVDisplayParam,i_width[0x%x],i_height[0x%x]"
#define H264_DEC_V1_1242_112_2_18_1_27_25_107 "MpegSetYUVDisplayParam,pframe NULL!"
#define H264_DEC_V1_1263_112_2_18_1_27_25_108 "MpegGetFreeYUVBuffer,yuv buffer is full!"
#define MJPG_DEC_V0_220_112_2_18_1_27_26_109 "MjpgDecoderTaskEntry,cur_time=%d,first_pos=%d"
#define MJPG_DEC_V0_230_112_2_18_1_27_26_110 "MjpgDecoderTaskEntry,get block error"
#define MJPG_DEC_V0_256_112_2_18_1_27_26_111 "MjpgDecoderTaskEntry,get block error"
#define MJPG_DEC_V0_272_112_2_18_1_27_26_112 "MjpgDecoderTaskEntry,get block error"
#define MJPG_DEC_V0_279_112_2_18_1_27_26_113 "MjpgDecoderTaskEntry,skip the frame"
#define MJPG_DEC_V0_292_112_2_18_1_27_26_114 "mjpg_dec,AllocateVideoDecFrameBuf fail."
#define MJPG_DEC_V0_319_112_2_18_1_27_26_115 "mjpg_dec task, decode_result=%d, return widht err!"
#define MJPG_DEC_V0_326_112_2_18_1_27_26_116 "mjpg_dec task, decode_result=%d, return err!"
#define MJPG_DEC_V0_397_112_2_18_1_27_26_117 "mjpg_dec,block new failed."
#define MJPG_DEC_V0_410_112_2_18_1_27_26_118 "mjpg decoder task, encount undefined event,i_event_flag[0x%x]"
#define MJPG_DEC_V0_449_112_2_18_1_27_27_119 "mpeg4_dec.c,close,status error!"
#define MJPG_DEC_V0_455_112_2_18_1_27_27_120 "mpjpg_dec.c, open, movie size is too large."
#define MJPG_DEC_V0_467_112_2_18_1_27_27_121 "mjpg_dec, Open(), failed to create event"
#define MJPG_DEC_V0_502_112_2_18_1_27_27_122 "mpeg4_dec.c,close,status error!"
#define MJPG_DEC_V0_561_112_2_18_1_27_27_123 "mjpg_dec, Send(), failed to call set event"
#define MJPG_DEC_V0_705_112_2_18_1_27_27_124 "mjpeg_dec.c, SetConfig, CMD_DEC_RE_SYN,p_event[%d]"
#define MJPG_DEC_V0_711_112_2_18_1_27_27_125 "mjpeg_dec.c, SetConfig, re sync fail,p_event[%d]"
#define MJPG_DEC_V0_719_112_2_18_1_27_27_126 "mjpeg_dec.c, SetConfig, invalid cmd"
#define MJPG_DEC_V0_743_112_2_18_1_27_27_127 "mjpeg_dec.c, GetInfo, invalid cmd"
#define MJPG_DEC_V0_782_112_2_18_1_27_27_128 "mjpg_dec.c,mutex_Init error!"
#define MJPG_DEC_V0_822_112_2_18_1_27_27_129 "SetVideoDecFrameBufStatus,p_frame NULL!"
#define MJPG_DEC_V0_897_112_2_18_1_27_27_130 "FreeOutBuf,p_buf=0x%x"
#define MJPG_DEC_V0_900_112_2_18_1_27_27_131 "FreeOutBuf, PNULL BUFFER."
#define MJPG_DEC_V1_210_112_2_18_1_27_28_132 "MjpgDecoderTaskEntry,cur_time=%d,first_pos=%d"
#define MJPG_DEC_V1_292_112_2_18_1_27_28_133 "mpjpg_dec, yuv buffer overflow!"
#define MJPG_DEC_V1_328_112_2_18_1_27_28_134 "---JPEG_DecodeJpeg,ret=%d,datatype=%d,out_y=0x%x,w,h=%d,%d, pts=%d, frm_num=%d"
#define MJPG_DEC_V1_381_112_2_18_1_27_28_135 "MjpgDecoderTaskEntry,block_New error"
#define MJPG_DEC_V1_387_112_2_18_1_27_28_136 "MjpgDecoderTaskEntry,dec error=%d"
#define MJPG_DEC_V1_396_112_2_18_1_27_28_137 "MjpgDecoderTaskEntry,block_FifoRemove error"
#define MJPG_DEC_V1_415_112_2_18_1_27_29_138 "mjpg_dec, active"
#define MJPG_DEC_V1_430_112_2_18_1_27_29_139 "---mjpg_dec, open,codec=0x%x,wh=%d,%d"
#define MJPG_DEC_V1_434_112_2_18_1_27_29_140 "---mjpg_dec,open error, size is too large"
#define MJPG_DEC_V1_444_112_2_18_1_27_29_141 "---mjpg_dec,mem init error"
#define MJPG_DEC_V1_452_112_2_18_1_27_29_142 "mjpg_dec, Open(), failed to create event"
#define MJPG_DEC_V1_474_112_2_18_1_27_29_143 "mjpg_dec, Open(), failed to create task"
#define MJPG_DEC_V1_487_112_2_18_1_27_29_144 "mjpg, open, err!"
#define MJPG_DEC_V1_545_112_2_18_1_27_29_145 "mjpg_dec, Send(), failed to call set event"
#define MJPG_DEC_V1_641_112_2_18_1_27_29_146 "mjpeg_dec, SetConfig, re sync OK"
#define MJPG_DEC_V1_645_112_2_18_1_27_29_147 "mjpeg_dec.c, SetConfig, re sync fail,p_event[%d]"
#define MJPG_DEC_V1_657_112_2_18_1_27_29_148 "mjpeg_dec, SetConfig, playback_mode = MODE_RATE"
#define MJPG_DEC_V1_663_112_2_18_1_27_29_149 "mjpeg_dec, SetConfig, playback_mode = MODE_RATE"
#define MJPG_DEC_V1_669_112_2_18_1_27_29_150 "mjpeg_dec, SetConfig, b_skip_display = SCI_TRUE"
#define MJPG_DEC_V1_673_112_2_18_1_27_29_151 "mjpeg_dec.c, SetConfig, invalid cmd"
#define MJPG_DEC_V1_697_112_2_18_1_27_29_152 "mjpeg_dec.c, GetInfo, invalid cmd"
#define MJPG_DEC_V1_730_112_2_18_1_27_29_153 "mjpg_dec.c,mutex_Init error!"
#define MJPG_DEC_V1_745_112_2_18_1_27_29_154 "MjpgInitYUVCtrl,malloc fail!,%d"
#define MJPG_DEC_V1_797_112_2_18_1_27_29_155 "MjpgSetYUVDataFlag,pframe NULL!"
#define MJPG_DEC_V1_812_112_2_18_1_27_29_156 "MjpgSetYUVDisplayParam,i_width[0x%x],i_height[0x%x]"
#define MJPG_DEC_V1_816_112_2_18_1_27_29_157 "MjpgSetYUVDisplayParam,pframe NULL!"
#define MJPG_DEC_V1_869_112_2_18_1_27_29_158 "FreeVideoDecFrameBuf,group[%d].addr=0x%x"
#define MJPG_DEC_V1_890_112_2_18_1_27_29_159 "__FreeMjpgOutBuf, PNULL BUFFER."
#define MPEG4_DEC_V0_257_112_2_18_1_27_30_160 "mpeg4_dec task, error happen when waiting for event,%d"
#define MPEG4_DEC_V0_311_112_2_18_1_27_30_161 "Mpeg4DecoderTaskEntry,cur_time=%d, first_pos=%d, vide_fifo_start_time=%d"
#define MPEG4_DEC_V0_340_112_2_18_1_27_31_162 "mpeg4_dec,video is too slow,seek one I VOP,syn_time[%d]"
#define MPEG4_DEC_V0_344_112_2_18_1_27_31_163 "mpeg4_dec,video is too slow, but no I VOP in next 1000ms. syn_time[%d]"
#define MPEG4_DEC_V0_397_112_2_18_1_27_31_164 "mpeg4_dec, DecoderTaskEntry, failed to call set event"
#define MPEG4_DEC_V0_412_112_2_18_1_27_31_165 "mpeg4_dec, disp_skip=%d, time_offset_a_v %d, b_expected_IVOP %d"
#define MPEG4_DEC_V0_416_112_2_18_1_27_31_166 "mpeg4_dec, _Peek_NextDispFrameContain false,wait for next time"
#define MPEG4_DEC_V0_449_112_2_18_1_27_31_167 "Mpeg4DecoderTaskEntry, frame_width=%d, frame_height=%d, extra_buffer_size=%d,"
#define MPEG4_DEC_V0_453_112_2_18_1_27_31_168 "Mpeg4DecoderTaskEntry, MP4DecMemSizeCal return error"
#define MPEG4_DEC_V0_465_112_2_18_1_27_31_169 "Mpeg4DecoderTaskEntry, frame_width=%d, frame_height=%d, Error or unsupport"
#define MPEG4_DEC_V0_475_112_2_18_1_27_31_170 "Mpeg4DecoderTaskEntry, p_decoder_buffer_extra == PNULL, return MMDEC_NOT_SUPPORTED"
#define MPEG4_DEC_V0_488_112_2_18_1_27_31_171 "Mpeg4DecoderTaskEntry, MPEG4DecMemInit failed"
#define MPEG4_DEC_V0_497_112_2_18_1_27_31_172 "Mpeg4DecoderTaskEntry, malloc OK, frame_width=%d, frame_height=%d, extra_buffer_size=%d,"
#define MPEG4_DEC_V0_580_112_2_18_1_27_31_173 "mpeg4_dec task, decode_result, MMDEC_OUTPUT_BUFFER_OVERFLOW"
#define MPEG4_DEC_V0_607_112_2_18_1_27_31_174 "mpeg4_dec, _Get_NextDispFrameContain PNULL, no frame contain"
#define MPEG4_DEC_V0_656_112_2_18_1_27_31_175 "mpeg4_dec, block_new failed."
#define MPEG4_DEC_V0_669_112_2_18_1_27_31_176 "mpeg4_dec task, decode_result, MMDEC_ERROR"
#define MPEG4_DEC_V0_677_112_2_18_1_27_31_177 "mpeg4_dec task, decode_result, MMDEC_PARAM_ERROR"
#define MPEG4_DEC_V0_686_112_2_18_1_27_31_178 "mpeg4_dec task, decode_result, MMDEC_MEMORY_ERROR"
#define MPEG4_DEC_V0_690_112_2_18_1_27_31_179 "mpeg4_dec task, decode_result, MMDEC_INVALID_STATUS"
#define MPEG4_DEC_V0_694_112_2_18_1_27_31_180 "mpeg4_dec task, decode_result, MMDEC_STREAM_ERROR"
#define MPEG4_DEC_V0_702_112_2_18_1_27_31_181 "mpeg4_dec task, decode_result, MMDEC_OUTPUT_BUFFER_OVERFLOW"
#define MPEG4_DEC_V0_706_112_2_18_1_27_31_182 "mpeg4_dec task, decode_result, MMDEC_HW_ERROR"
#define MPEG4_DEC_V0_713_112_2_18_1_27_31_183 "mpeg4_dec task, decode_result, MMDEC_NOT_SUPPORTED"
#define MPEG4_DEC_V0_718_112_2_18_1_27_31_184 "mpeg4_dec task, decode_result, MMDEC_FRAME_SEEK_IVOP"
#define MPEG4_DEC_V0_722_112_2_18_1_27_31_185 "mpeg4_dec task, decode_result=%d, return err!"
#define MPEG4_DEC_V0_741_112_2_18_1_27_31_186 "mpeg4 decoder task, encount undefined event,i_event_flag[0x%x],status[0x%x]"
#define MPEG4_DEC_V0_779_112_2_18_1_27_31_187 "mpeg4_dec.c,close,status error!"
#define MPEG4_DEC_V0_785_112_2_18_1_27_31_188 "mpeg4_dec.c, open, movie size is too large."
#define MPEG4_DEC_V0_845_112_2_18_1_27_32_189 "failed to call REFMP4DecInit()"
#define MPEG4_DEC_V0_852_112_2_18_1_27_32_190 "mpeg4_dec, Open(), failed to create event"
#define MPEG4_DEC_V0_876_112_2_18_1_27_32_191 "mpeg4_dec, Open(), failed to create task"
#define MPEG4_DEC_V0_914_112_2_18_1_27_32_192 "mpeg4_dec.c,close,status error!"
#define MPEG4_DEC_V0_981_112_2_18_1_27_32_193 "mpeg4_dec, Send(), failed to call set event"
#define MPEG4_DEC_V0_987_112_2_18_1_27_32_194 "mpeg4_dec, send(),fail"
#define MPEG4_DEC_V0_1118_112_2_18_1_27_32_195 "__NotifyYUVBufferIsFree, PNULL BUFFER."
#define MPEG4_DEC_V0_1161_112_2_18_1_27_32_196 "mp4_dec.c, SetConfig, re sync fail,status[%d]"
#define MPEG4_DEC_V0_1182_112_2_18_1_27_32_197 "mpeg4_dec, SetConfig, invalid cmd"
#define MPEG4_DEC_V0_1214_112_2_18_1_27_32_198 "mpeg4_dec, GetInfo, invalid cmd"
#define MPEG4_DEC_V1_199_112_2_18_1_27_33_199 "mpeg4_dec task, error happen when waiting for event"
#define MPEG4_DEC_V1_256_112_2_18_1_27_33_200 "Mpeg4DecoderTaskEntry,cur_time=%d,first_pos=%d, start_time=%d, offset=%d"
#define MPEG4_DEC_V1_277_112_2_18_1_27_33_201 "mpeg4_dec,video is too slow,seek one I VOP,syn_time[%d]"
#define MPEG4_DEC_V1_294_112_2_18_1_27_33_202 "mpeg4_dec, frame_rate=%d, div4,8,16=[%d,%d,%d]"
#define MPEG4_DEC_V1_377_112_2_18_1_27_33_203 "mpeg4_dec, cur_time=%d, bef_interval=%d, aft_interval=%d"
#define MPEG4_DEC_V1_443_112_2_18_1_27_34_204 "mpeg4_dec, yuv buffer overflow!"
#define MPEG4_DEC_V1_480_112_2_18_1_27_34_205 "Mpeg4DecoderTaskEntry, interval=%d, frame_width=%d, frame_height=%d, extra_buffer_size=%d,"
#define MPEG4_DEC_V1_484_112_2_18_1_27_34_206 "Mpeg4DecoderTaskEntry, MP4DecMemSizeCal return error"
#define MPEG4_DEC_V1_496_112_2_18_1_27_34_207 "Mpeg4DecoderTaskEntry, frame_width=%d, frame_height=%d, Error or unsupport"
#define MPEG4_DEC_V1_506_112_2_18_1_27_34_208 "Mpeg4DecoderTaskEntry, p_decoder_buffer_extra == PNULL, return MMDEC_NOT_SUPPORTED"
#define MPEG4_DEC_V1_518_112_2_18_1_27_34_209 "Mpeg4DecoderTaskEntry, MPEG4DecMemInit failed"
#define MPEG4_DEC_V1_527_112_2_18_1_27_34_210 "Mpeg4DecoderTaskEntry, malloc OK, frame_width=%d, frame_height=%d, extra_buffer_size=%d,"
#define MPEG4_DEC_V1_555_112_2_18_1_27_34_211 "mpeg4_dec, MPEG4_Decode time=%3d,ret=%d,exp_I=%d,pts=%d,frm_num=%d,display_skip=%d,eff=%d"
#define MPEG4_DEC_V1_564_112_2_18_1_27_34_212 "mpeg4_dec task, decode_result, MMDEC_OUTPUT_BUFFER_OVERFLOW"
#define MPEG4_DEC_V1_638_112_2_18_1_27_34_213 "mpeg4_dec, block_new failed."
#define MPEG4_DEC_V1_649_112_2_18_1_27_34_214 "mpeg4_dec task, stream err,ret=%d"
#define MPEG4_DEC_V1_656_112_2_18_1_27_34_215 "mpeg4_dec task, unsupport,ret=%d"
#define MPEG4_DEC_V1_661_112_2_18_1_27_34_216 "mpeg4_dec task, dec err,ret=%d"
#define MPEG4_DEC_V1_675_112_2_18_1_27_34_217 "mpeg4 decoder task, encount undefined event,i_event_flag[0x%x],status[0x%x]"
#define MPEG4_DEC_V1_717_112_2_18_1_27_34_218 "mpeg4_dec.c,close,status error!"
#define MPEG4_DEC_V1_772_112_2_18_1_27_34_219 "mpeg4_dec.c,mutex_Init error!"
#define MPEG4_DEC_V1_777_112_2_18_1_27_34_220 "mpeg4_dec.c,MpegInitYUVCtrl fail"
#define MPEG4_DEC_V1_785_112_2_18_1_27_34_221 "mpeg4_dec, Open(), failed to create event"
#define MPEG4_DEC_V1_810_112_2_18_1_27_34_222 "mpeg4_dec, Open(), failed to create task"
#define MPEG4_DEC_V1_835_112_2_18_1_27_34_223 "mpeg4_dec, Open(), failed to create task"
#define MPEG4_DEC_V1_849_112_2_18_1_27_34_224 "failed to call MP4DecInit()"
#define MPEG4_DEC_V1_867_112_2_18_1_27_34_225 "mpeg4_dec.c, open, err!"
#define MPEG4_DEC_V1_895_112_2_18_1_27_34_226 "mpeg4_dec, close(),status[0x%x]"
#define MPEG4_DEC_V1_899_112_2_18_1_27_34_227 "mpeg4_dec.c,close,status error!"
#define MPEG4_DEC_V1_955_112_2_18_1_27_35_228 "mpeg4_dec, send(), i_depth=%d"
#define MPEG4_DEC_V1_963_112_2_18_1_27_35_229 "mpeg4_dec, Send(), failed to call set event"
#define MPEG4_DEC_V1_969_112_2_18_1_27_35_230 "mpeg4_dec, send(),fail"
#define MPEG4_DEC_V1_1006_112_2_18_1_27_35_231 "mpeg4_dec, Stop(),status[0x%x]"
#define MPEG4_DEC_V1_1040_112_2_18_1_27_35_232 "mpeg4_dec, Pause(),status[0x%x]"
#define MPEG4_DEC_V1_1059_112_2_18_1_27_35_233 "mpeg4_dec, Resume(),status[0x%x]"
#define MPEG4_DEC_V1_1084_112_2_18_1_27_35_234 "__NotifyYUVBufferIsFree, pframe=PNULL"
#define MPEG4_DEC_V1_1087_112_2_18_1_27_35_235 "mpeg4_dec, __NotifyYUVBufferIsFree, pframe=0x%x"
#define MPEG4_DEC_V1_1118_112_2_18_1_27_35_236 "mpeg4_dec, SetConfig, re sync OK"
#define MPEG4_DEC_V1_1122_112_2_18_1_27_35_237 "mpeg4_dec.c, SetConfig, re sync fail,status[%d]"
#define MPEG4_DEC_V1_1137_112_2_18_1_27_35_238 "mpeg4_dec, SetConfig, playback_mode = MODE_SYNC"
#define MPEG4_DEC_V1_1143_112_2_18_1_27_35_239 "mpeg4_dec, SetConfig, playback_mode = MODE_RATE"
#define MPEG4_DEC_V1_1148_112_2_18_1_27_35_240 "mpeg4_dec, SetConfig, b_skip_display = SCI_TRUE"
#define MPEG4_DEC_V1_1157_112_2_18_1_27_35_241 "mpeg4_dec CMD_DEC_ADD_PLUG set,cfg=0x%x"
#define MPEG4_DEC_V1_1166_112_2_18_1_27_35_242 "mpeg4_dec, CMD_DEC_ADD_PLUG error"
#define MPEG4_DEC_V1_1174_112_2_18_1_27_35_243 "mpeg4_dec.c, SetConfig, invalid cmd"
#define MPEG4_DEC_V1_1198_112_2_18_1_27_35_244 "mpeg4_dec.c, GetInfo, invalid cmd"
#define MPEG4_DEC_V1_1230_112_2_18_1_27_35_245 "MpegInitYUVCtrl,g_frameYUVCtr[%d],malloc fail!"
#define MPEG4_DEC_V1_1272_112_2_18_1_27_35_246 "MpegSetYUVDataFlag,pframe NULL!"
#define MPEG4_DEC_V1_1275_112_2_18_1_27_35_247 "mpeg4_dec, MpegSetYUVDataFlag, pframe=0x%x, data_status=%d"
#define MPEG4_DEC_V1_1292_112_2_18_1_27_35_248 "MpegSetYUVDisplayParam,pframe NULL!"
#define MPEG4_DEC_V1_1309_112_2_18_1_27_35_249 "mpeg4_dec, MpegGetFreeYUVBuffer, yuv_num=%d, pframe=0x%x"
#define MJPG_ENC_V0_149_112_2_18_1_27_36_250 "Video,Captured,discard,i_state=%d"
#define MJPG_ENC_V0_210_112_2_18_1_27_36_251 "Video,Captured,adjust the frame cycle,i_deta_time=0x%x,frame_dts_dither=0x%x"
#define MJPG_ENC_V0_261_112_2_18_1_27_36_252 "Mjpeg,Captured,block new failed,fifo_depth=%d"
#define MJPG_ENC_V1_141_112_2_18_1_27_37_253 "InitEsBuffer, memory leaking"
#define MJPG_ENC_V1_254_112_2_18_1_27_37_254 "mjpg_enc task, error happen when waiting for event"
#define MJPG_ENC_V1_304_112_2_18_1_27_37_255 "mjpg_enc,in_fifo_depth=%d,out_fifo_depth=%d"
#define MJPG_ENC_V1_318_112_2_18_1_27_37_256 "mjpg_enc has stopped!"
#define MJPG_ENC_V1_326_112_2_18_1_27_37_257 "mjpg_enc,failed to alloc blocks"
#define MJPG_ENC_V1_331_112_2_18_1_27_37_258 "mjpg_enc has stopped!"
#define MJPG_ENC_V1_338_112_2_18_1_27_37_259 "mjpg_enc,failed to call set event"
#define MJPG_ENC_V1_343_112_2_18_1_27_37_260 "mjpg_enc,AllocEsBuffer,block=0x%x,addr=0x%x"
#define MJPG_ENC_V1_392_112_2_18_1_27_37_261 "mjpg_enc,ret_enc=%d,out_size=%d,Q=%d"
#define MJPG_ENC_V1_401_112_2_18_1_27_37_262 "mjpg_enc, failed to add to recorder fifo"
#define MJPG_ENC_V1_407_112_2_18_1_27_37_263 "mjpg_enc,enc failed."
#define MJPG_ENC_V1_414_112_2_18_1_27_37_264 "mjpg_enc, encount undefined event"
#define MJPG_ENC_V1_449_112_2_18_1_27_37_265 "mjpg_enc,Open failed, size is too large"
#define MJPG_ENC_V1_457_112_2_18_1_27_37_266 "mjpg_enc,open,failed to initial es buf"
#define MJPG_ENC_V1_477_112_2_18_1_27_37_267 "mpeg4_enc, Open(), failed to create event"
#define MJPG_ENC_V1_496_112_2_18_1_27_37_268 "%s(Line%d), SCI_INVALID_BLOCK_ID == s_enc_ctx.p_task"
#define MJPG_ENC_V1_504_112_2_18_1_27_37_269 "mjpg_enc,failed to open"
#define MJPG_ENC_V1_563_112_2_18_1_27_37_270 "Video,Captured,discard,i_state=%d"
#define MJPG_ENC_V1_624_112_2_18_1_27_38_271 "Video,Captured,adjust the frame cycle,i_deta_time=0x%x,frame_dts_dither=0x%x"
#define MJPG_ENC_V1_640_112_2_18_1_27_38_272 "Mjpeg,Captured,block new failed,fifo_depth=%d"
#define MJPG_ENC_V1_663_112_2_18_1_27_38_273 "mjpg_enc, Capture, block_FifoAdd, i_time=%d, first_frame_time=%d, pts=%d"
#define MJPG_ENC_V1_667_112_2_18_1_27_38_274 "mjpg_enc,capture,failed to call set event"
#define MPEG4_ENC_V0_149_112_2_18_1_27_38_275 "MPEG4 ENC, memory leaking"
#define MPEG4_ENC_V0_266_112_2_18_1_27_38_276 "mpeg4_enc task, error happen when waiting for event"
#define MPEG4_ENC_V0_299_112_2_18_1_27_38_277 "mpeg4_enc,in_fifo_depth=%d,out_fifo_depth=%d"
#define MPEG4_ENC_V0_303_112_2_18_1_27_38_278 "mpeg4_enc has stopped,state=%d!"
#define MPEG4_ENC_V0_323_112_2_18_1_27_38_279 "mpeg4_enc,failed to alloc blocks"
#define MPEG4_ENC_V0_345_112_2_18_1_27_38_280 "MP4EncStrmEncode return %d, size = %d"
#define MPEG4_ENC_V0_373_112_2_18_1_27_39_281 "mpeg4_enc, failed to add to recorder fifo"
#define MPEG4_ENC_V0_379_112_2_18_1_27_39_282 "mpeg4_enc,enc failed."
#define MPEG4_ENC_V0_386_112_2_18_1_27_39_283 "mpeg4_enc, encount undefined event"
#define MPEG4_ENC_V0_421_112_2_18_1_27_39_284 "mpeg4_enc_sc6800h.c Open()"
#define MPEG4_ENC_V0_429_112_2_18_1_27_39_285 "mpeg4_enc,Open failed, size is too large"
#define MPEG4_ENC_V0_459_112_2_18_1_27_39_286 "mpeg4_enc, Open(), fail to alloc buffer for encoder"
#define MPEG4_ENC_V0_470_112_2_18_1_27_39_287 "mpeg4_enc, Open(), fail to init the encoder"
#define MPEG4_ENC_V0_515_112_2_18_1_27_39_288 "mpeg4_enc, Open(), failed to create event"
#define MPEG4_ENC_V0_540_112_2_18_1_27_39_289 "mpeg4_enc, Open(), failed to create task"
#define MPEG4_ENC_V0_551_112_2_18_1_27_39_290 "mpeg4_enc,failed to open"
#define MPEG4_ENC_V0_624_112_2_18_1_27_39_291 "Video,Captured,discard,i_state=%d"
#define MPEG4_ENC_V0_683_112_2_18_1_27_39_292 "Video,Captured,adjust the frame cycle,i_deta_time=0x%x,frame_dts_dither=0x%x"
#define MPEG4_ENC_V0_699_112_2_18_1_27_39_293 "MPEG4,Captured,block new failed,fifo_depth=%d"
#define MPEG4_ENC_V0_718_112_2_18_1_27_39_294 "mpeg4_enc,capture,failed to call set event"
#define MPEG4_ENC_V1_156_112_2_18_1_27_40_295 "InitEsBuffer, memory leaking"
#define MPEG4_ENC_V1_270_112_2_18_1_27_40_296 "mpeg4_enc task, error happen when waiting for event"
#define MPEG4_ENC_V1_303_112_2_18_1_27_40_297 "mpeg4_enc,in_fifo_depth=%d,out_fifo_depth=%d"
#define MPEG4_ENC_V1_307_112_2_18_1_27_40_298 "mpeg4_enc has stopped,state=%d!"
#define MPEG4_ENC_V1_327_112_2_18_1_27_40_299 "mpeg4_enc,failed to alloc blocks"
#define MPEG4_ENC_V1_370_112_2_18_1_27_40_300 "mpeg4_enc,ret_enc=%d,out_size=%d"
#define MPEG4_ENC_V1_398_112_2_18_1_27_40_301 "mpeg4_enc, failed to add to recorder fifo"
#define MPEG4_ENC_V1_404_112_2_18_1_27_40_302 "mpeg4_enc,enc failed."
#define MPEG4_ENC_V1_411_112_2_18_1_27_40_303 "mpeg4_enc, encount undefined event"
#define MPEG4_ENC_V1_455_112_2_18_1_27_40_304 "mpeg4_enc,Open failed, size is too large"
#define MPEG4_ENC_V1_477_112_2_18_1_27_40_305 "mpeg4_enc, Open(), fail to alloc buffer for encoder"
#define MPEG4_ENC_V1_489_112_2_18_1_27_40_306 "mpeg4_enc, Open(), fail to init the encoder"
#define MPEG4_ENC_V1_517_112_2_18_1_27_40_307 "--para error,video_quality_level=%d"
#define MPEG4_ENC_V1_528_112_2_18_1_27_40_308 "mpeg4_enc, open, CAL_VideoEncSetConfig return error"
#define MPEG4_ENC_V1_537_112_2_18_1_27_40_309 "mpeg4_enc,open,failed to initial es buf"
#define MPEG4_ENC_V1_549_112_2_18_1_27_40_310 "mpeg4_enc, open, CAL_VideoEncGenHeader return error"
#define MPEG4_ENC_V1_554_112_2_18_1_27_40_311 "mpeg4_enc,open, invalid header bitstream_size"
#define MPEG4_ENC_V1_561_112_2_18_1_27_40_312 "mpeg4_enc,open, es_format memory alloc fail!"
#define MPEG4_ENC_V1_584_112_2_18_1_27_41_313 "mpeg4_enc, Open(), failed to create event"
#define MPEG4_ENC_V1_608_112_2_18_1_27_41_314 "mpeg4_enc, Open(), failed to create task"
#define MPEG4_ENC_V1_619_112_2_18_1_27_41_315 "mpeg4_enc,failed to open"
#define MPEG4_ENC_V1_695_112_2_18_1_27_41_316 "Video,Captured,discard,i_state=%d"
#define MPEG4_ENC_V1_754_112_2_18_1_27_41_317 "Video,Captured,adjust the frame cycle,i_deta_time=0x%x,frame_dts_dither=0x%x"
#define MPEG4_ENC_V1_770_112_2_18_1_27_41_318 "MPEG4,Captured,block new failed,fifo_depth=%d"
#define MPEG4_ENC_V1_789_112_2_18_1_27_41_319 "mpeg4_enc,capture,failed to call set event"
#define PLAYER_DISPLAY_V0_108_112_2_18_1_27_41_320 "Video_Display_Open,is_open=%d"
#define PLAYER_DISPLAY_V0_118_112_2_18_1_27_41_321 "Video_Display_Open,DC_Open fail"
#define PLAYER_DISPLAY_V0_138_112_2_18_1_27_41_322 "Video_Display_Close,is_open=%d"
#define PLAYER_DISPLAY_V0_177_112_2_18_1_27_41_323 "Video_Display_Setparam,rect=[%d,%d,%d,%d]"
#define PLAYER_DISPLAY_V0_181_112_2_18_1_27_41_324 "Video_Display_Setparam,para error!"
#define PLAYER_DISPLAY_V0_207_112_2_18_1_27_42_325 "Video_Display_Oneframe, isn't open yet,do nothing!"
#define PLAYER_DISPLAY_V0_213_112_2_18_1_27_42_326 "Video_Display_Oneframe, p_frame == PNULL !"
#define PLAYER_DISPLAY_V0_218_112_2_18_1_27_42_327 "Video_Display_Oneframe,w(%d) or h(%d) is zero!"
#define PLAYER_DISPLAY_V0_225_112_2_18_1_27_42_328 "Video_Display_Oneframe,update_display_param error"
#define PLAYER_DISPLAY_V0_274_112_2_18_1_27_42_329 "Video_Display_Oneframe,not support yuv%d"
#define PLAYER_DISPLAY_V0_280_112_2_18_1_27_42_330 "Video_Display_Oneframe, DC_DisplayAFrame fail"
#define PLAYER_DISPLAY_V0_290_112_2_18_1_27_42_331 "Video_Display_Oneframe, wait time out"
#define PLAYER_DISPLAY_V0_320_112_2_18_1_27_42_332 "Video_PostProcess_Oneframe,isn't open(%d) or busy(%d)"
#define PLAYER_DISPLAY_V0_325_112_2_18_1_27_42_333 "Video_PostProcess_Oneframe, p_frame == PNULL !"
#define PLAYER_DISPLAY_V0_343_112_2_18_1_27_42_334 "Video_PostProcess_Oneframe,in,data_wh=%d,%d,trim_wh=%d,%d"
#define PLAYER_DISPLAY_V0_349_112_2_18_1_27_42_335 "Video_PostProcess_Oneframe,w,h error"
#define PLAYER_DISPLAY_V0_358_112_2_18_1_27_42_336 "Video_PostProcess_Oneframe,update_display_param error"
#define PLAYER_DISPLAY_V0_408_112_2_18_1_27_42_337 "Video_PostProcess_Oneframe,not support yuv%d"
#define PLAYER_DISPLAY_V0_417_112_2_18_1_27_42_338 "Video_PostProcess_Oneframe, DC_DisplayAFrame fail"
#define PLAYER_DISPLAY_V0_434_112_2_18_1_27_42_339 "Video_PostProcess_Oneframe, wait time out"
#define PLAYER_DISPLAY_V0_465_112_2_18_1_27_42_340 "display_WaitStatus, wait time out[%d]"
#define PLAYER_DISPLAY_V0_485_112_2_18_1_27_42_341 "Video_Display_Getparam,para error!"
#define PLAYER_DISPLAY_V0_514_112_2_18_1_27_42_342 "update_display_param,org(w,h)=(%d,%d),disp_rect(x,y,w,h)=(%d,%d,%d,%d),angle=%d,distortion=%d"
#define PLAYER_DISPLAY_V0_576_112_2_18_1_27_42_343 "update_display_param,DC_VideoSetDisplayParam fail"
#define PLAYER_DISPLAY_V0_597_112_2_18_1_27_42_344 "video_pp_callback_frm_dc,callback,sx,ex=0x(%x,%x),sy,ey=0x(%x,%x)%d"
#define PLAYER_DISPLAY_V1_134_112_2_18_1_27_43_345 "mplayer_display_Open,duplicate"
#define PLAYER_DISPLAY_V1_167_112_2_18_1_27_43_346 "Video_Display_Close,duplicate."
#define PLAYER_DISPLAY_V1_208_112_2_18_1_27_43_347 "Video_Display_Setparam,para error!"
#define PLAYER_DISPLAY_V1_235_112_2_18_1_27_43_348 "Video_Display_Oneframe,not open yet"
#define PLAYER_DISPLAY_V1_240_112_2_18_1_27_43_349 "Video_Display_Oneframe,p_frame == PNULL"
#define PLAYER_DISPLAY_V1_245_112_2_18_1_27_43_350 "Video_Display_Oneframe,w(%d) or h(%d) is zero!"
#define PLAYER_DISPLAY_V1_251_112_2_18_1_27_43_351 "mplayer_display, wait time out"
#define PLAYER_DISPLAY_V1_264_112_2_18_1_27_43_352 "Video_Display_Oneframe, update_display_param fail"
#define PLAYER_DISPLAY_V1_287_112_2_18_1_27_43_353 "mplayer_display, wait time out"
#define PLAYER_DISPLAY_V1_315_112_2_18_1_27_43_354 "Video_PostProcess_Oneframe,not open yet"
#define PLAYER_DISPLAY_V1_339_112_2_18_1_27_43_355 "mplayer_display, wait time out"
#define PLAYER_DISPLAY_V1_354_112_2_18_1_27_43_356 "Video_Display_Oneframe, update_display_param fail"
#define PLAYER_DISPLAY_V1_380_112_2_18_1_27_43_357 "mplayer_display, wait time out"
#define PLAYER_DISPLAY_V1_437_112_2_18_1_27_43_358 "video display, isp_callback_display"
#define PLAYER_DISPLAY_V1_482_112_2_18_1_27_43_359 "display_WaitStatus, wait time out[%d]"
#define PLAYER_DISPLAY_V1_500_112_2_18_1_27_43_360 "Video_Display_Getparam,para error!"
#define PLAYER_DISPLAY_V1_524_112_2_18_1_27_44_361 "update_display_param,w,h =[%d,%d]"
#define PLAYER_DISPLAY_V1_581_112_2_18_1_27_44_362 "----display_param,input_wh=%d,%d,range_wh=%d,%d,addr_yuv=0x%x"
#define PLAYER_DISPLAY_V1_608_112_2_18_1_27_44_363 "----display_param,Angle=%d;lcdID:%d-%d;xy:%d,%d;wh:%d,%d"
#define PLAYER_DISPLAY_V1_614_112_2_18_1_27_44_364 "update_display_param,ISP_ServiceSetReviewParam fail"
#define ACCESS_MEM_112_112_2_18_1_27_45_365 "access_mem, Open(), failed to alloc stream CB"
#define ACCESS_MEM_138_112_2_18_1_27_45_366 "access_meme, Close(), invalidate parameter"
#define ACCESS_STREAM_290_112_2_18_1_27_47_367 "stream info: %d ms"
#define ACCESS_STREAM_304_112_2_18_1_27_47_368 "    |-- Audio Config: codec = %4.4s, duration = %d ms, channel = %d, samplerate = %d Hz"
#define ACCESS_STREAM_547_112_2_18_1_27_47_369 "access_stream, Open() failed"
#define ACCESS_STREAM_673_112_2_18_1_27_47_370 "access_stream,p_player_ctx is NULL."
#define BLOCK_200_112_2_18_1_27_49_371 "un expected frame buffer type"
#define BLOCK_218_112_2_18_1_27_49_372 "block_Init, len %d"
#define BLOCK_228_112_2_18_1_27_49_373 "block_Init,duplicate1"
#define BLOCK_239_112_2_18_1_27_49_374 "block_Init, p_blk_append_buf_base malloc failed."
#define BLOCK_253_112_2_18_1_27_49_375 "block_Init, p_sft_blk_fifo malloc failed."
#define BLOCK_310_112_2_18_1_27_49_376 "block_Init,duplicate2"
#define BLOCK_333_112_2_18_1_27_49_377 "block_Release"
#define BLOCK_342_112_2_18_1_27_49_378 "block_Release,p_sft_blk_fifo is empty or release != create"
#define BLOCK_396_112_2_18_1_27_49_379 "block_Alloc,fifo doesn't create yet"
#define BLOCK_406_112_2_18_1_27_49_380 "block_Alloc,i_size(%d)>fifo_width_size(%d)"
#define BLOCK_411_112_2_18_1_27_49_381 "block_Alloc,xmalloc failed"
#define BLOCK_427_112_2_18_1_27_49_382 "block_Alloc,fifo has been realeased."
#define BLOCK_441_112_2_18_1_27_49_383 "block_Alloc,no free blocks"
#define BLOCK_504_112_2_18_1_27_50_384 "block_New,fifo doesn't create yet"
#define BLOCK_513_112_2_18_1_27_50_385 "block_New, there are no free blocks"
#define BLOCK_578_112_2_18_1_27_50_386 "block_Free,fifo doesn't create yet"
#define BLOCK_588_112_2_18_1_27_50_387 "block_Free, re-free a block"
#define BLOCK_630_112_2_18_1_27_50_388 "block_FifoAdd, re-add a block"
#define BLOCK_788_112_2_18_1_27_50_389 "block_FifoGetPTS, paramater is error"
#define FILE_CACHE_RD_34_112_2_18_1_27_50_390 "Xfileopen,file_name=%s,access_mode=%s"
#define FILE_CACHE_RD_142_112_2_18_1_27_51_391 "video cache,slot %d, user num %d"
#define FILE_CACHE_RD_152_112_2_18_1_27_51_392 "audio cache,slot %d, user num %d"
#define FILE_CACHE_RD_283_112_2_18_1_27_51_393 "file_cache, failed to all stream CB"
#define FILE_CACHE_RD_309_112_2_18_1_27_51_394 "file_cache, Open(), invlidate file name"
#define FILE_CACHE_RD_319_112_2_18_1_27_51_395 "file_cache, failed to open file %s"
#define FILE_CACHE_RD_404_112_2_18_1_27_51_396 "file_cache, peek, invalidate stream"
#define FILE_CACHE_RD_426_112_2_18_1_27_51_397 "file_cache, peek, xmalloc failed"
#define FILE_CACHE_RD_455_112_2_18_1_27_51_398 "file_cache, invalidate stream or read len zero"
#define FILE_CACHE_RD_485_112_2_18_1_27_51_399 "file_cache, write, not support when on read mode, stream 0x%x, buf 0x%x, len %d"
#define FILE_CACHE_RD_557_112_2_18_1_27_51_400 "file cache, buffer overflow"
#define FILE_CACHE_RD_564_112_2_18_1_27_51_401 "file cache, buffer overflow"
#define FILE_CACHE_RD_583_112_2_18_1_27_51_402 "file cache, buffer overflow"
#define FILE_CACHE_RD_633_112_2_18_1_27_52_403 "CacheLoading,XFREAD fail,i_len[0x%x],i_append[0x%x], hal_pos[0x%x], pos[0x%x], file size[0x%x]"
#define FILE_CACHE_RD_650_112_2_18_1_27_52_404 "Readblock11111,file_cache, invalid port"
#define FILE_CACHE_RD_663_112_2_18_1_27_52_405 "Readblock33333333 file_ca"
#define FILE_CACHE_RD_668_112_2_18_1_27_52_406 "Readblock33333333 file_ca"
#define FILE_CACHE_RD_675_112_2_18_1_27_52_407 "Readblock222222file_cache, readblock, len %d > max frame size %d"
#define FILE_CACHE_RD_740_112_2_18_1_27_52_408 "Readblock 444444"
#define FILE_CACHE_RD_771_112_2_18_1_27_52_409 "Readblock 555555"
#define FILE_CACHE_RD_809_112_2_18_1_27_52_410 "Readblock 6666666"
#define FILE_CACHE_RD_836_112_2_18_1_27_52_411 "file_cache, writeblock, not support when on read mode, stream0x%x, block 0x%x, len %d"
#define FILE_CACHE_RD_850_112_2_18_1_27_52_412 "udisk access, seek, invalidate stream"
#define FILE_CACHE_RD_861_112_2_18_1_27_52_413 "file_cache, seek, pos < 0"
#define FILE_CACHE_RD_869_112_2_18_1_27_52_414 "file_cache, seek, pos > size"
#define FILE_CACHE_RD_885_112_2_18_1_27_52_415 "file_cache, size, invalidate stream"
#define FILE_CACHE_RD_899_112_2_18_1_27_52_416 "file_cache, tell, invalidate stream"
#define FILE_CACHE_RD_914_112_2_18_1_27_52_417 "file_cache_read, ConfigPort fail!"
#define FILE_CACHE_RD_920_112_2_18_1_27_52_418 "file_cache_read, invalid port id: %d"
#define FILE_CACHE_RD_946_112_2_18_1_27_52_419 "file_cache, initcache, alloc memory failed\n"
#define FILE_CACHE_RD_997_112_2_18_1_27_52_420 "file_cache_rd.c, p_cache->i_slot_user_num[slot_start] error"
#define FILE_CACHE_WR_223_112_2_18_1_27_53_421 "file_cache, readblock, not support when on write mode, stream 0x%x, len %d"
#define FILE_CACHE_WR_291_112_2_18_1_27_53_422 "file_cache, __write, invalid lenght <= 0"
#define FILE_CACHE_WR_301_112_2_18_1_27_53_423 "file_cache, error happen"
#define FILE_CACHE_WR_391_112_2_18_1_27_53_424 "file_cache, write, invalid parameters"
#define FILE_CACHE_WR_411_112_2_18_1_27_53_425 "file_cache, writeblock, invalid parameters"
#define FILE_CACHE_WR_450_112_2_18_1_27_53_426 "%s(Line%d), DV_WR_PADDING_LEN <= abs(i_length_left), i_length_left = %d"
#define FILE_CACHE_WR_464_112_2_18_1_27_53_427 "file_cache, writeblock, stream err"
#define FILE_CACHE_WR_478_112_2_18_1_27_53_428 "udisk access, seek, invalidate stream"
#define FILE_CACHE_WR_489_112_2_18_1_27_53_429 "file_cache, seek, pos < 0"
#define FILE_CACHE_WR_494_112_2_18_1_27_53_430 "file_cache, seek, pos > size"
#define FILE_CACHE_WR_510_112_2_18_1_27_53_431 "file_cache, size, invalidate stream"
#define FILE_CACHE_WR_524_112_2_18_1_27_53_432 "file_cache, tell, invalidate stream"
#define FILE_CACHE_WR_633_112_2_18_1_27_54_433 "mrecorder_GenerateTmpFileName,string_unit_len=%d,error"
#define FILE_CACHE_WR_640_112_2_18_1_27_54_434 "mrecorder_GenerateTmpFileName,malloc[%d,%d] error"
#define FILE_CACHE_WR_663_112_2_18_1_27_54_435 "mrecorder_JudgeStringUnitLen,error,p_string=0x%x,len=%d"
#define FILE_CACHE_WR_676_112_2_18_1_27_54_436 "mrecorder_JudgeStringUnitLen,error,offset=%d"
#define FILE_CACHE_WR_710_112_2_18_1_27_54_437 "mrecorder_JudgeStringUnitLen,unicode error,offset=%d"
#define FILE_CACHE_WR_722_112_2_18_1_27_54_438 "mrecorder_JudgeStringUnitLen,unicode,byte0-7=0x[%x,%x,%x,%x;%x,%x,%x,%x]"
#define FILE_CACHE_WR_756_112_2_18_1_27_54_439 "suffix_support,char,0x[%x,%x,%x,%x];0x[%x,%x,%x,%x];0x[%x,%x,%x,%x]"
#define FILE_CACHE_WR_761_112_2_18_1_27_54_440 "mrecorder_JudgeStringUnitLen,char error,offset=%d"
#define FILE_CACHE_WR_772_112_2_18_1_27_54_441 "mrecorder_JudgeStringUnitLen,char,byte0-3=0x[%x,%x,%x,%x]"
#define FILE_CACHE_WR_808_112_2_18_1_27_54_442 "mrecorder_string_char2unicode,char string=%s"
#define FILE_CACHE_WR_900_112_2_18_1_27_54_443 "file_cache, failed to all stream CB"
#define FILE_CACHE_WR_913_112_2_18_1_27_54_444 "file_cache,Open(),file name is too long,byte_len=%d>size=%d"
#define FILE_CACHE_WR_921_112_2_18_1_27_54_445 "file_cache,open,mrecorder_JudgeStringUnitLen,%d"
#define FILE_CACHE_WR_931_112_2_18_1_27_54_446 "file_cache,open,mrecorder_GenerateTmpFileName for av error"
#define FILE_CACHE_WR_938_112_2_18_1_27_54_447 "file_cache,open,mrecorder_GenerateTmpFileName for list error"
#define FILE_CACHE_WR_947_112_2_18_1_27_54_448 "file_cache,open,av old file name=%s"
#define FILE_CACHE_WR_951_112_2_18_1_27_54_449 "file_cache,open,mrecorder_GenerateTmpFileName for av error"
#define FILE_CACHE_WR_954_112_2_18_1_27_54_450 "file_cache,open,av tmp file name=%s"
#define FILE_CACHE_WR_959_112_2_18_1_27_54_451 "file_cache,open,mrecorder_GenerateTmpFileName for list error"
#define FILE_CACHE_WR_962_112_2_18_1_27_54_452 "file_cache,open,list tmp file name=%s"
#define FILE_CACHE_WR_966_112_2_18_1_27_54_453 "file_cache,open,string_unit_byte_len[%d] error"
#define FILE_CACHE_WR_984_112_2_18_1_27_54_454 "file_cache_wr,av file open,name=%s"
#define FILE_CACHE_WR_992_112_2_18_1_27_54_455 "file_cache_wr, failed to open file %s"
#define FILE_CACHE_WR_1011_112_2_18_1_27_54_456 "failed to open tmpfile"
#define FILE_CACHE_WR_1019_112_2_18_1_27_54_457 "failed to open tmpfile"
#define FILE_CACHE_WR_1028_112_2_18_1_27_54_458 "file_cache_wr,tmp file open,name=%s"
#define FILE_CACHE_WR_1033_112_2_18_1_27_54_459 "failed to open tmpfile,%s"
#define FILE_CACHE_WR_1096_112_2_18_1_27_54_460 "file_cache_wr,rename file name,old=%s,new%s"
#define TMPFILE_138_112_2_18_1_27_55_461 "memtmpfile_AppendWrite, malloc failed"
#define TMPFILE_167_112_2_18_1_27_55_462 "tmpfile_Read, read failed"
#define TMPFILE_185_112_2_18_1_27_55_463 "tmpfile_AppendWrite, write failed"
#define TMPFILE_272_112_2_18_1_27_55_464 "tmpfile_Close,close failed."
#define TMPFILE_277_112_2_18_1_27_55_465 "tmpfile_Close,delete failed."
#define AUDIO_DEC_199_112_2_18_1_27_57_466 "Failed to AUDIO_Play, return %d"
#define RMVB_DEC_225_112_2_18_1_28_1_467 "rmvb_dec, _SyncTable_Add, table_end=%d"
#define RMVB_DEC_235_112_2_18_1_28_1_468 "rmvb_dec, _SyncTable_Remove, table_begin=%d"
#define RMVB_DEC_247_112_2_18_1_28_1_469 "rmvb_dec, _SyncTable_GetSync, time=%d, [begin, end]=[%d, %d]: [%d, %d]"
#define RMVB_DEC_319_112_2_18_1_28_1_470 "rmvb_dec, _Seek_To_IVOP_InFifo, sync_id[%d], begin end[%d, %d], depth[%d]"
#define RMVB_DEC_332_112_2_18_1_28_1_471 "_Seek_To_IVOP_InFifo, free pt_bakblock, flags=%d"
#define RMVB_DEC_344_112_2_18_1_28_1_472 "_Seek_To_IVOP_InFifo, I_FRAME, i=%d"
#define RMVB_DEC_357_112_2_18_1_28_1_473 "_Seek_To_IVOP_InFifo, get I_FRAME"
#define RMVB_DEC_363_112_2_18_1_28_1_474 "rmvb_dec, _Seek_To_IVOP_InFifo, block_FifoRemove depth=0"
#define RMVB_DEC_404_112_2_18_1_28_2_475 "rmvb_dec task, error happen when waiting for event"
#define RMVB_DEC_410_112_2_18_1_28_2_476 "rmvb_dec task, receieve exit event"
#define RMVB_DEC_438_112_2_18_1_28_2_477 "rmvb_dec task, MP4_DECODER_PAUSE_EVENT"
#define RMVB_DEC_467_112_2_18_1_28_2_478 "RMVB_DecoderTaskEntry, cur_time=%d, first_pos=%d, offset=%d"
#define RMVB_DEC_494_112_2_18_1_28_2_479 "rmvb_dec, b_expected_IVOP, block is not I frame, free p_block 0x%x"
#define RMVB_DEC_529_112_2_18_1_28_2_480 "rmvb_dec,video is too slow,seek one I VOP,syn_time[%d]"
#define RMVB_DEC_533_112_2_18_1_28_2_481 "rmvb_dec,video is too slow, but no I VOP in next 1000ms. syn_time[%d]"
#define RMVB_DEC_615_112_2_18_1_28_2_482 "rmvb_dec, cur_time=%d, bef_time=%d, aft_time=%d, interval=[%d, %d]"
#define RMVB_DEC_652_112_2_18_1_28_2_483 "rmvb_dec, time_offset_a_v[%d]"
#define RMVB_DEC_659_112_2_18_1_28_2_484 "rmvb_dec, DecoderTaskEntry, failed to call set event"
#define RMVB_DEC_684_112_2_18_1_28_2_485 "rmvb_dec, yuv buffer overflow!"
#define RMVB_DEC_690_112_2_18_1_28_2_486 "rmvb_dec, DecoderTaskEntry, failed to call set event"
#define RMVB_DEC_713_112_2_18_1_28_2_487 "the pre_sample is special and skipped,finding one I frame now."
#define RMVB_DEC_719_112_2_18_1_28_2_488 "rmvb_dec, fifo block num > 60, seek IVOP"
#define RMVB_DEC_724_112_2_18_1_28_2_489 "rmvb_dec, block 0x%x, expected_IVOP %d, i_flags %d, disp_skip %d,i_depth %d,i_frame %d"
#define RMVB_DEC_750_112_2_18_1_28_2_490 "rmvb_dec, frame: 0x%x, data ptr[0x%x], len[%d], segment num %d"
#define RMVB_DEC_761_112_2_18_1_28_2_491 "RMVB_DecDecode,ret=%d,frm_num[%ld],usedTime[%ld],i_flags[%d],pts=%d,frm_eff=%d"
#define RMVB_DEC_789_112_2_18_1_28_2_492 "rmvb_dec task, decode_result, MMDEC_OUTPUT_BUFFER_OVERFLOW"
#define RMVB_DEC_869_112_2_18_1_28_3_493 "rmvb_dec, block_new failed."
#define RMVB_DEC_882_112_2_18_1_28_3_494 "rmvb_dec task, decode_result, MMDEC_ERROR"
#define RMVB_DEC_888_112_2_18_1_28_3_495 "rmvb_dec task, decode_result, MMDEC_PARAM_ERROR"
#define RMVB_DEC_894_112_2_18_1_28_3_496 "rmvb_dec task, decode_result, MMDEC_MEMORY_ERROR"
#define RMVB_DEC_898_112_2_18_1_28_3_497 "rmvb_dec task, decode_result, MMDEC_INVALID_STATUS"
#define RMVB_DEC_902_112_2_18_1_28_3_498 "rmvb_dec task, decode_result, MMDEC_STREAM_ERROR"
#define RMVB_DEC_908_112_2_18_1_28_3_499 "rmvb_dec task, decode_result, MMDEC_OUTPUT_BUFFER_OVERFLOW"
#define RMVB_DEC_912_112_2_18_1_28_3_500 "rmvb_dec task, decode_result, MMDEC_HW_ERROR"
#define RMVB_DEC_917_112_2_18_1_28_3_501 "rmvb_dec task, decode_result, MMDEC_NOT_SUPPORTED"
#define RMVB_DEC_922_112_2_18_1_28_3_502 "rmvb_dec task, decode_result, MMDEC_FRAME_SEEK_IVOP"
#define RMVB_DEC_926_112_2_18_1_28_3_503 "rmvb_dec task, decode_result=%d, return err!"
#define RMVB_DEC_932_112_2_18_1_28_3_504 "rmvb_dec task, (SCI_TRUE == decode_ctrl_flag.b_expected_IVOP), free this frame and break"
#define RMVB_DEC_942_112_2_18_1_28_3_505 "rmvb_dec decoder task, encount undefined event,i_event_flag[0x%x],status[0x%x]"
#define RMVB_DEC_952_112_2_18_1_28_3_506 "rmvb_dec, Active()"
#define RMVB_DEC_965_112_2_18_1_28_3_507 "rmvb_dec, Deactive()"
#define RMVB_DEC_978_112_2_18_1_28_3_508 "rmvb_dec, Open(),status[0x%x]"
#define RMVB_DEC_981_112_2_18_1_28_3_509 "rmvb_dec.c,close,status error!"
#define RMVB_DEC_987_112_2_18_1_28_3_510 "rmvb_dec.c, open, movie size is too large."
#define RMVB_DEC_997_112_2_18_1_28_3_511 "rmvb_dec, open, RV40"
#define RMVB_DEC_1002_112_2_18_1_28_3_512 "rmvb_dec, open, RV30, unsupported"
#define RMVB_DEC_1008_112_2_18_1_28_3_513 "rmvb_dec, open, unsupported video codec: 0x%x"
#define RMVB_DEC_1024_112_2_18_1_28_3_514 "rmvb_dec, open, RVDecMemSizeCal w,h=[%d,%d], memsize=%d"
#define RMVB_DEC_1034_112_2_18_1_28_3_515 "rmvb_dec, open, p_decoder_buffer_inter alloca fail"
#define RMVB_DEC_1043_112_2_18_1_28_3_516 "failed to call RvDecInit()"
#define RMVB_DEC_1051_112_2_18_1_28_3_517 "rmvb_dec, open, p_decoder_buffer_extra alloca fail"
#define RMVB_DEC_1060_112_2_18_1_28_3_518 "rmvb_dec.c, open, RvDecMemInit failed!"
#define RMVB_DEC_1068_112_2_18_1_28_3_519 "rmvb_dec, Open(), failed to create event"
#define RMVB_DEC_1093_112_2_18_1_28_3_520 "rmvb_dec, Open(), failed to create task"
#define RMVB_DEC_1114_112_2_18_1_28_3_521 "rmvb_dec.c, open, err!"
#define RMVB_DEC_1152_112_2_18_1_28_3_522 "rmvb_dec, close(),status[0x%x]"
#define RMVB_DEC_1156_112_2_18_1_28_3_523 "rmvb_dec.c,close,status error!"
#define RMVB_DEC_1162_112_2_18_1_28_3_524 "rmvb_dec.c, RvDecRelease failed!"
#define RMVB_DEC_1217_112_2_18_1_28_3_525 "rmvb_dec, Send, p_block 0x%x, i_datalen=%d, frame_num=%d, flag=%d, depth=%d"
#define RMVB_DEC_1227_112_2_18_1_28_3_526 "rmvb_dec, Send(), failed to call set event"
#define RMVB_DEC_1238_112_2_18_1_28_3_527 "rmvb_dec, send(),fail"
#define RMVB_DEC_1261_112_2_18_1_28_3_528 "rmvb_dec, Start(),status[0x%x]"
#define RMVB_DEC_1273_112_2_18_1_28_3_529 "rmvb_dec.c, RvDecMemInit failed!"
#define RMVB_DEC_1281_112_2_18_1_28_3_530 "rmvb_dec, Resume, failed to call set event"
#define RMVB_DEC_1293_112_2_18_1_28_3_531 "rmvb_dec, Stop(),status[0x%x]"
#define RMVB_DEC_1315_112_2_18_1_28_3_532 "rmvb_dec, Pause(),status[0x%x]"
#define RMVB_DEC_1324_112_2_18_1_28_3_533 "rmvb_dec, Pause() OK"
#define RMVB_DEC_1347_112_2_18_1_28_3_534 "rmvb_dec, Resume(),status[0x%x]"
#define RMVB_DEC_1360_112_2_18_1_28_3_535 "__NotifyRMVBYUVBufferIsFree, PNULL BUFFER."
#define RMVB_DEC_1397_112_2_18_1_28_4_536 "rmvb_dec, SetConfig, re sync OK"
#define RMVB_DEC_1402_112_2_18_1_28_4_537 "rmvb_dec, SetConfig, re sync fail,status[%d]"
#define RMVB_DEC_1418_112_2_18_1_28_4_538 "rmvb_dec CMD_DEC_ROTATION set %d"
#define RMVB_DEC_1431_112_2_18_1_28_4_539 "rmvb_dec, SetConfig, playback_mode = MODE_SYNC"
#define RMVB_DEC_1437_112_2_18_1_28_4_540 "rmvb_dec, SetConfig, playback_mode = MODE_RATE"
#define RMVB_DEC_1443_112_2_18_1_28_4_541 "rmvb_dec, SetConfig, b_skip_display = SCI_TRUE"
#define RMVB_DEC_1447_112_2_18_1_28_4_542 "rmvb_dec, SetConfig, invalid cmd"
#define RMVB_DEC_1477_112_2_18_1_28_4_543 "rmvb_dec, GetInfo, invalid cmd"
#define AVI_382_112_2_18_1_28_5_544 "cannot peek()"
#define AVI_397_112_2_18_1_28_5_545 "avi module discarded (invalid header)"
#define AVI_406_112_2_18_1_28_5_546 "avi, p_demux->p_sys, malloc fail!"
#define AVI_426_112_2_18_1_28_5_547 "avi module discarded (invalid file)"
#define AVI_456_112_2_18_1_28_5_548 "avi module discarded (invalid file)"
#define AVI_462_112_2_18_1_28_5_549 "cannot find avih chunk"
#define AVI_474_112_2_18_1_28_5_550 "no stream defined!"
#define AVI_491_112_2_18_1_28_5_551 "avi.c, p_sys->track, malloc fail!"
#define AVI_518_112_2_18_1_28_5_552 "avi.c, avi_track_t, malloc fail!"
#define AVI_580_112_2_18_1_28_5_553 "avi.c, tk->p_index , malloc fail!"
#define AVI_644_112_2_18_1_28_6_554 "video config port fail"
#define AVI_652_112_2_18_1_28_6_555 "avi.c, vids tk->p_index , malloc fail!"
#define AVI_677_112_2_18_1_28_6_556 "avi.c, p_sys->p_syn_table, malloc fail!"
#define AVI_749_112_2_18_1_28_6_557 "i_duration info error"
#define AVI_809_112_2_18_1_28_6_558 "i_rate, or i_scale info error"
#define AVI_831_112_2_18_1_28_6_559 "no valid track"
#define AVI_861_112_2_18_1_28_6_560 "avi open, not find one index or no vide track, exit..."
#define AVI_983_112_2_18_1_28_6_561 "avi demux close, p_sys is NULL"
#define AVI_1122_112_2_18_1_28_6_562 "Demux, i_track[0x%x],[0x%x],[0x%x], i_idxposc out of range!"
#define AVI_1134_112_2_18_1_28_6_563 "Demux, i_track[%d],i_dpts[%d] out of range!"
#define AVI_1192_112_2_18_1_28_7_564 "Demux,no tracks[%d] need demuxing!"
#define AVI_1214_112_2_18_1_28_7_565 "Demux,video fifo overflow!"
#define AVI_1227_112_2_18_1_28_7_566 "Demux,audio fifo overflow!"
#define AVI_1236_112_2_18_1_28_7_567 "Demux,track[%d],seek[0x%x] failed!"
#define AVI_1282_112_2_18_1_28_7_568 "Demux,stream_ReadBlock,failed reading data,track[%d],result[%d]"
#define AVI_1454_112_2_18_1_28_7_569 "Demux, i_track[%ld],i_idxposc out of range after load index"
#define AVI_1531_112_2_18_1_28_7_570 "shouldn't yet be executed"
#define AVI_1646_112_2_18_1_28_7_571 "AVI_TrackSeek,less than the first sync,after_sync_id=%d, after_sync_time=%d"
#define AVI_1674_112_2_18_1_28_8_572 "AVI_TrackSeek,in table ,low=%d,time=%d, high=%d, time=%d"
#define AVI_1685_112_2_18_1_28_8_573 "AVI_TrackSeek,out of table , high=%d, time=%d"
#define AVI_1698_112_2_18_1_28_8_574 "GetVideoSyncPoint, i_time %d"
#define AVI_1737_112_2_18_1_28_8_575 "SeekVideoToSyncPoint, invalid syn point,i_idxposc[%d] "
#define AVI_1750_112_2_18_1_28_8_576 "SeekVideoToSyncPoint, i_idxposc=%d, i_idxnb_base=%d, i_idxnb_cur_num=%d"
#define AVI_1766_112_2_18_1_28_8_577 "SeekVideoToSyncPoint, out of table, i_idxnb_base=%d, i_idxnb_cur_num=%d"
#define AVI_1782_112_2_18_1_28_8_578 "SeekVideoToSyncPoint, seek done, video pts=%d, sys time=%d, target_time=%d,i_idxnb_base=%d, i_idxnb_cur_num=%d"
#define AVI_1792_112_2_18_1_28_8_579 "SeekVideoToSyncPoint, shouldn't yet be executed"
#define AVI_1805_112_2_18_1_28_8_580 "SetTimeVideo, i_time %d"
#define AVI_2018_112_2_18_1_28_8_581 "AVI_TrackSeek,less than the first sync,vide_chun_num=%d"
#define AVI_2068_112_2_18_1_28_8_582 "AVI_TrackSeek,in table, vide_chun_num=%d,low=%d,high=%d,vide_chun_dest=%d"
#define AVI_2073_112_2_18_1_28_8_583 "AVI_TrackSeek,out of table, vide_chun_num=%d"
#define AVI_2088_112_2_18_1_28_8_584 "AVI_TrackSeek,check forward,tmp=%d < idxposc=%d"
#define AVI_2098_112_2_18_1_28_8_585 "AVI_TrackSeek,check backward,tmp=%d > idxposc=%d"
#define AVI_2119_112_2_18_1_28_8_586 "AVI_TrackSeek, audio check forward, tmp_index %d<%d, or tmp_block %d<%d"
#define AVI_2128_112_2_18_1_28_8_587 "AVI_TrackSeek, audio check backward, tmp_index %d>%d, or tmp_block %d>%d"
#define AVI_2144_112_2_18_1_28_8_588 " AVI_TrackSeek, audo syn point is invalid!"
#define AVI_2167_112_2_18_1_28_9_589 "AVI_TrackSeek, AVI_IndexDynamicLoad, invalid syn point,stream[%d] "
#define AVI_2505_112_2_18_1_28_9_590 "%s(Line%d), tk->i_idxnb_cur_num(%u) >= INDEX_ENTRY_TABLE_NUM"
#define AVI_2607_112_2_18_1_28_9_591 "cannot find indx (misdetect/broken OpenDML file?)"
#define AVI_2635_112_2_18_1_28_9_592 "AVI_IndexLoad_indx, super_indx_subchunk malloc fail!"
#define AVI_2646_112_2_18_1_28_9_593 "AVI_IndexLoad_indx, super_indx_data_ptr malloc fail!"
#define AVI_2666_112_2_18_1_28_10_594 "AVI_IndexLoad_indx, p_super_indx_subchunk malloc fail!"
#define AVI_2676_112_2_18_1_28_10_595 "AVI_IndexLoad_indx, AVI_ChunkRead_ix error, no super index data"
#define AVI_2689_112_2_18_1_28_10_596 "AVI_IndexLoad_indx, AVI_ChunkRead_ix error, i_super_indx_subchunk %d"
#define AVI_2698_112_2_18_1_28_10_597 "AVI_IndexLoad_indx, super index err."
#define AVI_2833_112_2_18_1_28_10_598 "AVI_IndexLoad_idx1,return err"
#define AVI_2917_112_2_18_1_28_10_599 "AVI_ChunkDynamicRead_idx1,return err"
#define AVI_3125_112_2_18_1_28_10_600 "AVI_IndexDynamicLoad_indx, cannot find indx (misdetect/broken OpenDML file?)"
#define AVI_3148_112_2_18_1_28_10_601 "AVI_IndexDynamicLoad_indx,return err"
#define AVI_3167_112_2_18_1_28_10_602 "AVI_IndexDynamicLoad_indx, super index,i_subchunk_num[%d] err"
#define AVI_3202_112_2_18_1_28_11_603 "AVI_IndexDynamicLoad_indx, load to the end."
#define AVI_3239_112_2_18_1_28_11_604 "AVI_IndexDynamicLoad_indx, unknown type index(0x%x)"
#define AVI_3292_112_2_18_1_28_11_605 "AVI_IndexDynamicLoad_indx, cannot find indx (misdetect/broken OpenDML file?)"
#define AVI_3316_112_2_18_1_28_11_606 "AVI_IndexDynamicLoad_indx,return err"
#define AVI_3336_112_2_18_1_28_11_607 "AVI_IndexDynamicLoad_indx, super index,i_subchunk_num[%d] err"
#define AVI_3356_112_2_18_1_28_11_608 "AVI_IndexDynamicLoad_indx,return err"
#define AVI_3493_112_2_18_1_28_11_609 "AVI_SynTableCreate_idx1,return err"
#define AVI_3811_112_2_18_1_28_12_610 "AVI_SynTableCreateEx_indx, no vide exist!"
#define AVI_3844_112_2_18_1_28_12_611 "AVI_IndexDynamicLoad_indx, cannot find indx (misdetect/broken OpenDML file?)"
#define AVI_3892_112_2_18_1_28_12_612 "AVI_IndexDynamicLoad_indx,return err"
#define AVI_3957_112_2_18_1_28_12_613 "AVI_IndexDynamicLoad_indx, super index,i_subchunk_num[%d] err"
#define AVI_3983_112_2_18_1_28_12_614 "AVI_IndexDynamicLoad_indx,return err"
#define AVI_4199_112_2_18_1_28_13_615 "AVI_SynIndxAddEntryEx, super index,i_subchunk_num[%d] err"
#define AVI_4338_112_2_18_1_28_13_616 "AVI_SynTableCreate_idx1,return err"
#define LIBAVI_93_112_2_18_1_28_14_617 "AVI_NextChunk,file or chunk size is larger than 4GB.chunk_pos=0x%x,size=0x%x"
#define LIBAVI_98_112_2_18_1_28_14_618 "AVI_NextChunk,out of file size."
#define LIBAVI_161_112_2_18_1_28_14_619 "AVI_ChunkRead_list, avi chunk malloc fail."
#define LIBAVI_318_112_2_18_1_28_14_620 "malformed avi file"
#define LIBAVI_323_112_2_18_1_28_14_621 "malformed avi file"
#define LIBAVI_473_112_2_18_1_28_14_622 "AVI_ChunkRead_idx1,read fail!"
#define LIBAVI_539_112_2_18_1_28_14_623 "AVI_ChunkDynamicRead_idx1,out of range err, left %d"
#define LIBAVI_573_112_2_18_1_28_15_624 "AVI_ChunkDynamicRead_idx1,read fail!"
#define LIBAVI_631_112_2_18_1_28_15_625 "AVI_ChunkDynamicRead_indx, p_index == PNULL"
#define LIBAVI_647_112_2_18_1_28_15_626 "AVI_ChunkDynamicRead_indx, invalid index sub type %d"
#define LIBAVI_654_112_2_18_1_28_15_627 "AVI_ChunkDynamicRead_indx,out of range err, left %d"
#define LIBAVI_688_112_2_18_1_28_15_628 "AVI_ChunkDynamicRead_indx,read fail!"
#define LIBAVI_723_112_2_18_1_28_15_629 "AVI_ChunkDynamicRead_indx, invalid index type!"
#define LIBAVI_785_112_2_18_1_28_15_630 "AVI_ChunkRead_idx1,read fail!"
#define LIBAVI_856_112_2_18_1_28_15_631 "AVI_ChunkRead_ix, error happened, unknow index chunk."
#define LIBAVI_893_112_2_18_1_28_15_632 "AVI_ChunkRead_idx1,read fail!"
#define LIBAVI_936_112_2_18_1_28_16_633 "AVI_ChunkRead_idx1,read fail!"
#define LIBAVI_986_112_2_18_1_28_16_634 "AVI_ChunkRead_idx1,read fail!"
#define LIBAVI_1039_112_2_18_1_28_16_635 "AVI_ChunkRead_idx1,read fail!"
#define LIBAVI_1396_112_2_18_1_28_16_636 "AVI_ChunkReadRoot, chunk malloc fail. "
#define DEMUX_FLV_288_112_2_18_1_28_17_637 "demux_flv,flv_read_header,failed."
#define DEMUX_FLV_297_112_2_18_1_28_17_638 "demux_flv,xcalloc failed."
#define DEMUX_FLV_394_112_2_18_1_28_18_639 "FLV_TAG_TYPE_META,object_name_type=%d"
#define DEMUX_FLV_429_112_2_18_1_28_18_640 "demux flv open, neither audio nor video is supported,audio_found=%d,video_found=%d"
#define DEMUX_FLV_547_112_2_18_1_28_18_641 "Demux,video fifo overflow!"
#define DEMUX_FLV_565_112_2_18_1_28_18_642 "demux_flv,read next header error."
#define DEMUX_FLV_589_112_2_18_1_28_18_643 "demux_flv,ACCESS_CACHE_OVERFLOW_ERR"
#define DEMUX_FLV_597_112_2_18_1_28_18_644 "demux_flv,ACCESS_SAMPLE_TOOBIG"
#define DEMUX_FLV_608_112_2_18_1_28_18_645 "demux_flv,error=%d,p_blk=0x%x"
#define DEMUX_FLV_666_112_2_18_1_28_18_646 " demux_flv,SetTime,i_time=%d,cur_time=%d"
#define DEMUX_FLV_675_112_2_18_1_28_18_647 "SeekToSyncPoint,time used=%d"
#define DEMUX_FLV_742_112_2_18_1_28_18_648 "VideoSeekToSyncPoint,cur_pos=%d,cur_time=%d;target_time=%d"
#define DEMUX_FLV_1335_112_2_18_1_28_20_649 "Read Block,err type=%d"
#define DEMUX_FLV_1364_112_2_18_1_28_20_650 "Read Block failed,acc_ret=%d"
#define LIBMP4_OPT2_51_112_2_18_1_28_21_651 "MP4_LibMemAlloc failed,offset=%d,size=%d"
#define LIBMP4_OPT2_585_112_2_18_1_28_22_652 "MP4_ReadBox_ftyp, not enough data[%ld][0x%x]."
#define LIBMP4_OPT2_1411_112_2_18_1_28_24_653 "discarding drms"
#define LIBMP4_OPT2_1437_112_2_18_1_28_24_654 "discarding calling drms_free()"
#define LIBMP4_OPT2_1668_112_2_18_1_28_24_655 "stco, not support"
#define LIBMP4_OPT2_2015_112_2_18_1_28_25_656 "out of memory"
#define LIBMP4_OPT2_2220_112_2_18_1_28_25_657 "discarding read drms box"
#define LIBMP4_OPT2_2512_112_2_18_1_28_26_658 "MP4_ReadBox,MP4_LibMemAlloc failed"
#define LIBMP4_OPT2_2624_112_2_18_1_28_26_659 "MP4_BoxGetRoot,MP4_LibMemAlloc failed"
#define MP4_OPT2_313_112_2_18_1_28_28_660 "mp4 demux open,MP4_InitLib fail"
#define MP4_OPT2_325_112_2_18_1_28_28_661 "mp4 demux open,MP4 plugin discarded (cannot peek) fail"
#define MP4_OPT2_345_112_2_18_1_28_28_662 "mp4 demux open,MP4 plugin discarded (not a valid file) fail"
#define MP4_OPT2_402_112_2_18_1_28_28_663 "MP4 plugin discarded (no moov box)"
#define MP4_OPT2_421_112_2_18_1_28_28_664 "cannot find /moov/mvhd"
#define MP4_OPT2_437_112_2_18_1_28_28_665 "cannot find any /moov/trak"
#define MP4_OPT2_468_112_2_18_1_28_28_666 "video config port fail"
#define MP4_OPT2_613_112_2_18_1_28_29_667 "MP4_TrackUnselect111111,track[0x%x] will be disabled"
#define MP4_OPT2_665_112_2_18_1_28_29_668 "Demux, video fifo overflow!"
#define MP4_OPT2_675_112_2_18_1_28_29_669 "Demux, audio fifo overflow!"
#define MP4_OPT2_681_112_2_18_1_28_29_670 "mp4_opt2, demux, audio_demux_time > media_time + 2500, break!"
#define MP4_OPT2_694_112_2_18_1_28_29_671 "MP4_TrackUnselect2222222track[0x%x] will be disabled (eof?)"
#define MP4_OPT2_716_112_2_18_1_28_29_672 "track[0x%x] block is too big, skip and seek next sample."
#define MP4_OPT2_781_112_2_18_1_28_29_673 "MP4_TrackUnselect4444444mp4_demux, at the end of the track"
#define MP4_OPT2_811_112_2_18_1_28_29_674 "Demux, i_track[%d], es_out_Send fail!"
#define MP4_OPT2_965_112_2_18_1_28_29_675 "TrackToSyncPoint, target outrange, reload stss backward"
#define MP4_OPT2_1033_112_2_18_1_28_29_676 "TrackToSyncPoint, target outrange, reload stss forward"
#define MP4_OPT2_1238_112_2_18_1_28_30_677 "SearchSyncPoint, target outrange, reload stss backward"
#define MP4_OPT2_1315_112_2_18_1_28_30_678 "SearchSyncPoint, target outrange, reload stss forward"
#define MP4_OPT2_1519_112_2_18_1_28_30_679 "SeekToSyncPoint,backward,TrackToSyncPoint faild"
#define MP4_OPT2_1530_112_2_18_1_28_30_680 "mp4_demux,SeekToSyncPoint,do nothing,des_time=%d,cur_tim=%d,select=%d"
#define MP4_OPT2_1558_112_2_18_1_28_30_681 "SeekToSyncPoint, MP4_TrackNextSample return error"
#define MP4_OPT2_1569_112_2_18_1_28_31_682 "SeekToSyncPoint, MP4_TrackPreviousSample return error"
#define MP4_OPT2_1733_112_2_18_1_28_31_683 "SearchVideoSyncPoint, no stss"
#define MP4_OPT2_1739_112_2_18_1_28_31_684 "SearchVideoSyncPoint, not selected"
#define MP4_OPT2_1745_112_2_18_1_28_31_685 "SearchVideoSyncPoint, current %d > target %d"
#define MP4_OPT2_1759_112_2_18_1_28_31_686 "SearchVideoSyncPoint, SearchSyncPoint out of table"
#define MP4_OPT2_1777_112_2_18_1_28_31_687 "SearchVideoSyncPoint, before_sync_id %d, before_destination %d, after_sync_id %d, after_destination %d"
#define MP4_OPT2_1778_112_2_18_1_28_31_688 "SearchVideoSyncPoint, before interva %d, after interval %d, tk->i_sample=%d"
#define MP4_OPT2_1808_112_2_18_1_28_31_689 "GetVideoSyncPoint, i_time %d"
#define MP4_OPT2_1859_112_2_18_1_28_31_690 "SeekVideoToSyncPoint, p_sys->i_time %d, i_time %d, i_new_time %d, tk->i_sample %d"
#define MP4_OPT2_1878_112_2_18_1_28_31_691 "SetTimeVideo, i_time=%d"
#define MP4_OPT2_2546_112_2_18_1_28_32_692 "missing avcC"
#define MP4_OPT2_2703_112_2_18_1_28_33_693 "Mp4_opt2.c : fix frames per sample: new=%d"
#define MP4_OPT2_2790_112_2_18_1_28_33_694 "MP4_TrackCreate,timescale of p_track(0x%x) is zero"
#define MP4_OPT2_3313_112_2_18_1_28_34_695 "the STTS cache is not consistent with the track"
#define MP4_OPT2_3367_112_2_18_1_28_34_696 "the STTS cache is not consistent with the track"
#define DEMUX_RMVB_185_112_2_18_1_28_39_697 "Demux_rmvb, ra_block_available, p_ra_block is null"
#define DEMUX_RMVB_195_112_2_18_1_28_39_698 "Demux_rmvb, ra_block_available, audio fifo overflow!"
#define DEMUX_RMVB_202_112_2_18_1_28_39_699 "Demux_rmvb, ra_block_available, block new fail"
#define DEMUX_RMVB_226_112_2_18_1_28_39_700 "Demux_rmvb, ra_block_available, block 0x%x, pData 0x%x, ulDataLen 0x%x, i_pos %d, ra_interval %d, CurFileOffset %d, , frame_num %d"
#define DEMUX_RMVB_242_112_2_18_1_28_39_701 "Demux_rmvb, ra_block_available,es_out_Send fail!"
#define DEMUX_RMVB_266_112_2_18_1_28_39_702 "Demux_rmvb, rv_frame_available, p_frame is null"
#define DEMUX_RMVB_274_112_2_18_1_28_39_703 "Demux_rmvb, rv_frame_available, num segments > 60"
#define DEMUX_RMVB_285_112_2_18_1_28_39_704 "Demux_rmvb, rv_frame_available, discard_b_frame, timestamp %d, frame_num %d"
#define DEMUX_RMVB_301_112_2_18_1_28_39_705 "Demux_rmvb, rv_frame_available, discard_b_frame2, timestamp %d, frame_num %d"
#define DEMUX_RMVB_312_112_2_18_1_28_39_706 "Demux_rmvb, rv_frame_available, video fifo overflow!"
#define DEMUX_RMVB_323_112_2_18_1_28_39_707 "Demux_rmvb, rv_frame_available, b_seek_IVOP = SCI_TRUE, this frame is I frame"
#define DEMUX_RMVB_328_112_2_18_1_28_39_708 "Demux_rmvb, rv_frame_available, b_seek_IVOP = SCI_TRUE, this frame is not a I frame"
#define DEMUX_RMVB_336_112_2_18_1_28_39_709 "Demux_rmvb, rv_frame_available, block new fail"
#define DEMUX_RMVB_362_112_2_18_1_28_39_710 "demux_rmvb, rv_frame_available, block 0x%x, pData 0x%x, ulDataLen 0x%x, i_pos %d, interval %d, frame_num %d, i_flags %d, i_interval %d"
#define DEMUX_RMVB_386_112_2_18_1_28_39_711 "Demux_rmvb, rv_frame_available,es_out_Send fail!"
#define DEMUX_RMVB_397_112_2_18_1_28_39_712 "rmvb_demux_error pError=0x%08x result=%d msg=%s"
#define DEMUX_RMVB_470_112_2_18_1_28_39_713 "Demux_rmvb, Open"
#define DEMUX_RMVB_474_112_2_18_1_28_39_714 "Demux_rmvb, open,rmvb plugin discarded (cannot peek) fail"
#define DEMUX_RMVB_481_112_2_18_1_28_39_715 "Demux_rmvb, This file is not an .rm file."
#define DEMUX_RMVB_489_112_2_18_1_28_39_716 "Demux_rmvb, Open, p_sys malloc fail"
#define DEMUX_RMVB_499_112_2_18_1_28_39_717 "Demux_rmvb, Open, parser create fail"
#define DEMUX_RMVB_508_112_2_18_1_28_39_718 "Demux_rmvb, Open, packet buffer malloc fail"
#define DEMUX_RMVB_519_112_2_18_1_28_39_719 "Demux_rmvb, Open, parser init io fail"
#define DEMUX_RMVB_527_112_2_18_1_28_40_720 "Demux_rmvb, Open, parser read headers fail"
#define DEMUX_RMVB_535_112_2_18_1_28_40_721 "Demux_rmvb, Error: rm_parser_get_num_streams() returns 0"
#define DEMUX_RMVB_583_112_2_18_1_28_40_722 "Demux_rmvb, audio and video track do not exist"
#define DEMUX_RMVB_586_112_2_18_1_28_40_723 "Demux_rmvb, audio track %d, video track %d"
#define DEMUX_RMVB_592_112_2_18_1_28_40_724 "Demux_rmvb, Open, track malloc fail"
#define DEMUX_RMVB_604_112_2_18_1_28_40_725 "Demux_rmvb, Open, tk malloc fail"
#define DEMUX_RMVB_619_112_2_18_1_28_40_726 "Demux_rmvb, Open,  pRaDepack malloc fail"
#define DEMUX_RMVB_631_112_2_18_1_28_40_727 "Demux_rmvb, Open, ra_depack_init fail"
#define DEMUX_RMVB_637_112_2_18_1_28_40_728 "Demux_rmvb, Open, rm_parser_get_stream_header fail"
#define DEMUX_RMVB_647_112_2_18_1_28_40_729 "Demux_rmvb, Open, Audio Codec4CC: 0x%x"
#define DEMUX_RMVB_657_112_2_18_1_28_40_730 "Demux_rmvb, Open, ra_depack_get_codec_init_info  fail"
#define DEMUX_RMVB_679_112_2_18_1_28_40_731 "Demux_rmvb, Open, fmt.p_extra malloc fail"
#define DEMUX_RMVB_718_112_2_18_1_28_40_732 "Demux_rmvb, Open, tk malloc fail"
#define DEMUX_RMVB_733_112_2_18_1_28_40_733 "Demux_rmvb, Open, rv_depack_create2  fail"
#define DEMUX_RMVB_745_112_2_18_1_28_40_734 "Demux_rmvb, Open, rv_depack_init  fail"
#define DEMUX_RMVB_751_112_2_18_1_28_40_735 "Demux_rmvb, Open, rm_parser_get_stream_header  fail"
#define DEMUX_RMVB_759_112_2_18_1_28_40_736 "Demux_rmvb, Open, rv_depack_get_codec_init_info  fail"
#define DEMUX_RMVB_768_112_2_18_1_28_40_737 "Demux_rmvb, Open, Video Codec4CC: 0x%x"
#define DEMUX_RMVB_777_112_2_18_1_28_40_738 "Demux_rmvb, Open, ufFramesPerSecond %d, frame_rate %d"
#define DEMUX_RMVB_782_112_2_18_1_28_40_739 "Demux_rmvb, Open, discard_b_frame on"
#define DEMUX_RMVB_810_112_2_18_1_28_40_740 "Demux_rmvb, Open, file have not video or audio track"
#define DEMUX_RMVB_820_112_2_18_1_28_40_741 "Demux_rmvb, Open, error handle"
#define DEMUX_RMVB_888_112_2_18_1_28_40_742 "Demux_rmvb, Close"
#define DEMUX_RMVB_1006_112_2_18_1_28_40_743 "Demux_rmvb, video or audio dec in fifo overflow: [%d, %d]"
#define DEMUX_RMVB_1016_112_2_18_1_28_40_744 "Demux_rmvb, Demux, rm_parser_get_packet fail, timestamp %d, stream_num %d"
#define DEMUX_RMVB_1023_112_2_18_1_28_40_745 "Demux_rmvb, Demux, invalid stream num, i_stream=%d, i_tracks=%d"
#define DEMUX_RMVB_1047_112_2_18_1_28_41_746 "Demux_rmvb, Demux, undefined packet "
#define DEMUX_RMVB_1054_112_2_18_1_28_41_747 "Demux_rmvb, Demux, track is not activated "
#define DEMUX_RMVB_1091_112_2_18_1_28_41_748 "Demux_rmvb, Seek, time: %d, ulCurFileOffset %d"
#define DEMUX_RMVB_1107_112_2_18_1_28_41_749 "Demux_rmvb, Seek,rmvb plugin discarded (cannot peek) fail"
#define DEMUX_RMVB_1135_112_2_18_1_28_41_750 "Demux_rmvb, Seek, after seek, ulCurFileOffset %d, sys time %d, ret 0x%x"
#define DEMUX_RMVB_1162_112_2_18_1_28_41_751 "Demux_rmvb, SetTime, time: %d"
#define DEMUX_RMVB_1233_112_2_18_1_28_41_752 "demux_rmvb, RMVB_FreeRvFrame, p_block 0x%x, len %d, frm_num %d, i_flags %d"
#define DEMUX_RMVB_1244_112_2_18_1_28_41_753 "demux_rmvb, RMVB_FreeRaBlock, p_block 0x%x"
#define DEMUX_STREAM_345_112_2_18_1_29_1_754 "stream demux,p_player_ctx is NULL."
#define DEMUX_STREAM_393_112_2_18_1_29_1_755 "Demux, video fifo overflow!"
#define DEMUX_STREAM_403_112_2_18_1_29_1_756 "Demux, audio fifo overflow!"
#define AMR_ENC_176_112_2_18_1_29_2_757 "DumpAmrCallbackData,xmalloc failed"
#define AMR_ENC_236_112_2_18_1_29_2_758 "AmrCallBack,block_Alloc failed,fifo_depth=%d"
#define AMR_ENC_242_112_2_18_1_29_2_759 "AmrCallBack,fifo_overflow,fifo_depth=%d"
#define AMR_ENC_260_112_2_18_1_29_2_760 "amr_enc, current amr only support mono-channel, channels %d"
#define AMR_ENC_314_112_2_18_1_29_2_761 "amr_enc, start, invalid hDspMp4Record"
#define PCM_ENC_225_112_2_18_1_29_3_762 "pcm_enc.c, encode_block, block_Alloc error"
#define PCM_ENC_233_112_2_18_1_29_3_763 "pcm_enc.c, AdpcmEncoder error!"
#define PCM_ENC_249_112_2_18_1_29_3_764 "pcm_enc.c, enter EncoderTaskEntry"
#define PCM_ENC_257_112_2_18_1_29_3_765 "mjpg_enc task, error happen when waiting for event"
#define PCM_ENC_300_112_2_18_1_29_3_766 "pcm_enc has stopped!"
#define PCM_ENC_317_112_2_18_1_29_3_767 "pcm_enc.c, EncoderTaskEntry, block fifo is empty!"
#define PCM_ENC_334_112_2_18_1_29_3_768 "pcm_enc, encode_block, failed to add to mux fifo1"
#define PCM_ENC_351_112_2_18_1_29_3_769 "pcm_enc, encode_block, failed to add to mux fifo2"
#define PCM_ENC_358_112_2_18_1_29_3_770 "pcm_enc.c, pcm encode format is not supported"
#define PCM_ENC_368_112_2_18_1_29_3_771 "pcm_enc, encount undefined event"
#define PCM_ENC_509_112_2_18_1_29_4_772 "PcmCallBack, status error."
#define PCM_ENC_545_112_2_18_1_29_4_773 "pcm_enc,capture,failed to call set event"
#define PCM_ENC_593_112_2_18_1_29_4_774 "pcm_cap_start,src_dec=%d"
#define PCM_ENC_598_112_2_18_1_29_4_775 "pcm_cap_start,s_h_pcm_capture has been opened already = 0x%x"
#define PCM_ENC_620_112_2_18_1_29_4_776 "pcm_cap_start,s_h_pcm_capture opened fail"
#define PCM_ENC_665_112_2_18_1_29_4_777 "pcm_cap_start,ATV s_h_pcm_capture opened fail"
#define PCM_ENC_702_112_2_18_1_29_4_778 "pcm_cap_stop,src_dec=%d"
#define PCM_ENC_740_112_2_18_1_29_4_779 "pcm_cap_stop,error,s_h_pcm_capture=0x%x,src_dec=%d"
#define PCM_ENC_766_112_2_18_1_29_4_780 "pcm_enc, current pcm only support mono-channel, channels %d"
#define PCM_ENC_801_112_2_18_1_29_4_781 "pcm_enc, open, invalid codec type"
#define PCM_ENC_853_112_2_18_1_29_4_782 "pcm_enc, Open(), failed to create event"
#define PCM_ENC_856_112_2_18_1_29_4_783 "pcm_enc.c, Open, create event successfully "
#define PCM_ENC_874_112_2_18_1_29_4_784 "%s(Line%d), SCI_INVALID_BLOCK_ID == s_enc_ctx.p_task"
#define PCM_ENC_881_112_2_18_1_29_4_785 "pcm_enc.c, Open, create encoder task successfully"
#define PCM_ENC_951_112_2_18_1_29_4_786 "pcm_enc,failed to trigger the encoder"
#define MODULE_95_112_2_18_1_29_6_787 "module_InitBank, error, duplicate"
#define AVI_MUX_313_112_2_18_1_29_8_788 "avi mux,open,free_space=0x%x"
#define AVI_MUX_317_112_2_18_1_29_8_789 "avi mux, open, no enough space."
#define AVI_MUX_332_112_2_18_1_29_8_790 "avi mux open, malloc sys fail!"
#define AVI_MUX_352_112_2_18_1_29_8_791 "avi mux open, malloc p_indx_buf fail!"
#define AVI_MUX_366_112_2_18_1_29_8_792 "avi mux open, malloc idx1.entry fail!"
#define AVI_MUX_421_112_2_18_1_29_8_793 "avi mux close,stream_Write error."
#define AVI_MUX_432_112_2_18_1_29_8_794 "avi mux close,stream_Write error."
#define AVI_MUX_494_112_2_18_1_29_8_795 "avi mux,finish std indx error."
#define AVI_MUX_530_112_2_18_1_29_8_796 "avi mux close,write p_hdr error."
#define AVI_MUX_581_112_2_18_1_29_9_797 "avi mux, control, cmd=%d,param=0x%x"
#define AVI_MUX_607_112_2_18_1_29_9_798 "too many streams"
#define AVI_MUX_886_112_2_18_1_29_9_799 "avi mux mux,write p_hdr error."
#define AVI_MUX_976_112_2_18_1_29_9_800 "avi_mux.c, mux,i_cat=%d,sum=%d,depth=%d,empty_need=%d,empty_interval=%d"
#define AVI_MUX_1004_112_2_18_1_29_9_801 "avi_mux.c, mux,useless frame,insert empty frame."
#define AVI_MUX_1016_112_2_18_1_29_9_802 "Mux,avi,block fifo is empty2!"
#define AVI_MUX_1035_112_2_18_1_29_9_803 "mux, expect_free_space=0x%x,free_space=0x%x"
#define AVI_MUX_1048_112_2_18_1_29_9_804 "avi mux,file size reach the limited size,%d + %d + %d >= %d"
#define AVI_MUX_1102_112_2_18_1_29_10_805 "avi mux,write block header error."
#define AVI_MUX_1118_112_2_18_1_29_10_806 "avi mux,write vol error."
#define AVI_MUX_1140_112_2_18_1_29_10_807 "avi mux,write block error."
#define AVI_MUX_1182_112_2_18_1_29_10_808 "mux, expect_free_space=0x%x,free_space=0x%x"
#define AVI_MUX_1192_112_2_18_1_29_10_809 "avi mux,file size reach the limited size,%d + %d + %d(*84) >= %d"
#define AVI_MUX_1202_112_2_18_1_29_10_810 "avi mux,write block header error."
#define AVI_MUX_1222_112_2_18_1_29_10_811 "Mux,avi,block fifo is empty2!"
#define AVI_MUX_1259_112_2_18_1_29_10_812 "avi mux,write block error."
#define AVI_MUX_1266_112_2_18_1_29_10_813 "avi_mux, mux, audio i_chunk_len=%d"
#define AVI_MUX_1324_112_2_18_1_29_10_814 "avi mux,write std indx error."
#define AVI_MUX_1348_112_2_18_1_29_10_815 "avi mux,super indx full."
#define AVI_MUX_1389_112_2_18_1_29_10_816 "Mux,avi, table in tmpfile is full,exit"
#define AVI_MUX_1794_112_2_18_1_29_11_817 "failed to allocate a block"
#define AVI_MUX_1846_112_2_18_1_29_11_818 "CreatEmptyVideFrame, block_New fail."
#define MP4_MUX_OPT_288_112_2_18_1_29_12_819 "mux,open,free_space=0x%x"
#define MP4_MUX_OPT_445_112_2_18_1_29_12_820 "mp4 mux, control, cmd=%d,param=0x%x"
#define MP4_MUX_OPT_487_112_2_18_1_29_12_821 "unsupported codec %4.4s in mp4"
#define MP4_MUX_OPT_975_112_2_18_1_29_13_822 "mux, expect_free_space=0x%x,free_space=0x%x"
#define MP4_MUX_OPT_988_112_2_18_1_29_13_823 "mp4 mux,file size reach the limited size,%d + %d + %d >= %d"
#define MP4_MUX_OPT_1737_112_2_18_1_29_15_824 "FIXME ignoring trak (noaudio&&novideo)"
#define PLAYER_INNER_1051_112_2_18_1_29_18_825 "mplayer_GetVideoSyncPoint, i_time %d"
#define PLAYER_MAIN_94_112_2_18_1_29_19_826 "DPLAYER_Open in!"
#define PLAYER_MAIN_106_112_2_18_1_29_19_827 "DPLAYER_Open, failed to create obj"
#define PLAYER_MAIN_112_112_2_18_1_29_19_828 "DPLAYER_Open, failed to active obj"
#define PLAYER_MAIN_143_112_2_18_1_29_19_829 "DPLAYER_Open, failed to create task"
#define PLAYER_MAIN_155_112_2_18_1_29_19_830 "DPLAYER_Open return succ handle,0x%x"
#define PLAYER_MAIN_172_112_2_18_1_29_19_831 "DPLAYER_Close,handle=0x%x"
#define PLAYER_MAIN_177_112_2_18_1_29_19_832 "DPLAYER_Close, invalid handle=0x%x"
#define PLAYER_MAIN_183_112_2_18_1_29_19_833 "DPLAYER_Close,obj=0x%x,is_open=%d"
#define PLAYER_MAIN_187_112_2_18_1_29_19_834 "DPLAYER is not open yet !"
#define PLAYER_MAIN_215_112_2_18_1_29_19_835 "DPLAYER_Play(),handle=0x%x"
#define PLAYER_MAIN_220_112_2_18_1_29_19_836 "DPLAYER_Play, invalid handle=0x%x"
#define PLAYER_MAIN_226_112_2_18_1_29_19_837 "DPLAYER_Play(),obj=0x%x,is_open=%d"
#define PLAYER_MAIN_230_112_2_18_1_29_19_838 "DPLAYER_Play, demux is not open yet"
#define PLAYER_MAIN_255_112_2_18_1_29_19_839 "DPLAYER_Play,ADD_DEC_SECURITY,argc=%d,argv[0]=0x%x"
#define PLAYER_MAIN_293_112_2_18_1_29_19_840 "DPLAYER_Stop,handle=0x%x"
#define PLAYER_MAIN_298_112_2_18_1_29_19_841 "DPLAYER_Stop, invalid handle=0x%x"
#define PLAYER_MAIN_304_112_2_18_1_29_19_842 "DPLAYER_Stop, demux is not open yet"
#define PLAYER_MAIN_310_112_2_18_1_29_19_843 "DPLAYER_Stop Fail!"
#define PLAYER_MAIN_342_112_2_18_1_29_19_844 "DPLAYER_Pause,handle=0x%x"
#define PLAYER_MAIN_347_112_2_18_1_29_19_845 "DPLAYER_Pause, invalid handle=0x%x"
#define PLAYER_MAIN_353_112_2_18_1_29_19_846 "DPLAYER_Pause, demux is not open yet"
#define PLAYER_MAIN_359_112_2_18_1_29_19_847 "DPLAYER_Pause Fail!"
#define PLAYER_MAIN_376_112_2_18_1_29_19_848 "DPLAYER_PauseEx(),handle=0x%x"
#define PLAYER_MAIN_381_112_2_18_1_29_19_849 "DPLAYER_PauseEx, invalid handle=0x%x"
#define PLAYER_MAIN_387_112_2_18_1_29_19_850 "DPLAYER_PauseEx, demux is not open yet"
#define PLAYER_MAIN_394_112_2_18_1_29_19_851 "DPLAYER_PauseEx, player_Stop failed!"
#define PLAYER_MAIN_430_112_2_18_1_29_19_852 "DPLAYER_Resume,handle=0x%x"
#define PLAYER_MAIN_435_112_2_18_1_29_19_853 "DPLAYER_Resume, invalid handle=0x%x"
#define PLAYER_MAIN_441_112_2_18_1_29_19_854 "DPLAYER_Resume, demux is not open yet"
#define PLAYER_MAIN_463_112_2_18_1_29_19_855 "DPLAYER_ResumeEx,handle=0x%x"
#define PLAYER_MAIN_468_112_2_18_1_29_19_856 "DPLAYER_ResumeEx, invalid handle=0x%x"
#define PLAYER_MAIN_474_112_2_18_1_29_19_857 "DPLAYER_ResumeEx, demux is re_opened"
#define PLAYER_MAIN_482_112_2_18_1_29_20_858 "DPLAYER_ResumeEx, _player_set_active_obj failed."
#define PLAYER_MAIN_494_112_2_18_1_29_20_859 "DPLAYER_ResumeEx, _player_load_submodule failed %d"
#define PLAYER_MAIN_503_112_2_18_1_29_20_860 "DPLAYER_ResumeEx, failed to create task"
#define PLAYER_MAIN_538_112_2_18_1_29_20_861 "DPLAYER_ResumeEx, ADD_DEC_SECURITY,argc=%d,argv[0]=0x%x"
#define PLAYER_MAIN_561_112_2_18_1_29_20_862 "DPLAYER_Pause Fail!"
#define PLAYER_MAIN_588_112_2_18_1_29_20_863 "DPLAYER_SetPlayRate: rate is %d"
#define PLAYER_MAIN_593_112_2_18_1_29_20_864 "DPLAYER_SetPlayRate, invalid handle=0x%x"
#define PLAYER_MAIN_670_112_2_18_1_29_20_865 "DPLAYER_GetLength, invalid handle=0x%x"
#define PLAYER_MAIN_695_112_2_18_1_29_20_866 "DPLAYER_SetTime(),i_time=%d"
#define PLAYER_MAIN_700_112_2_18_1_29_20_867 "DPLAYER_SetTime, invalid handle=0x%x"
#define PLAYER_MAIN_706_112_2_18_1_29_20_868 "DPLAYER_SetTime, demux is not open yet"
#define PLAYER_MAIN_736_112_2_18_1_29_20_869 "DPLAYER_GetTime, invalid handle=0x%x"
#define PLAYER_MAIN_756_112_2_18_1_29_20_870 "DPLAYER_UpdateCurrentFrame,in"
#define PLAYER_MAIN_761_112_2_18_1_29_20_871 "DPLAYER_UpdateCurrentFrame, invalid handle=0x%x"
#define PLAYER_MAIN_768_112_2_18_1_29_20_872 "DPLAYER_UpdateCurrentFrame, no invalid video frame."
#define PLAYER_MAIN_775_112_2_18_1_29_20_873 "DPLAYER_UpdateCurrentFrame,fail,endnotify to stop."
#define PLAYER_MAIN_798_112_2_18_1_29_20_874 "DPLAYER_UpdateNextFrame()"
#define PLAYER_MAIN_803_112_2_18_1_29_20_875 "DPLAYER_UpdateNextFrame, invalid handle=0x%x"
#define PLAYER_MAIN_821_112_2_18_1_29_20_876 "DPLAYER_UpdateNextFrame,fail,endnotify to stop."
#define PLAYER_MAIN_846_112_2_18_1_29_20_877 "DPLAYER_GetMovieInfo,invalidate para,0x%x,0x%x"
#define PLAYER_MAIN_852_112_2_18_1_29_20_878 "DPLAYER_UpdateNextFrame, invalid handle=0x%x"
#define PLAYER_MAIN_863_112_2_18_1_29_20_879 "DPLAYER_GetMovieInfo,video:[w,h]=[%d,%d],bitrate=%d,frame_rate=%d;audio:samplerate=%d,bitrate=%d;stream_bitrate=%d"
#define PLAYER_MAIN_887_112_2_18_1_29_20_880 "DPLAYER_SetFrameEndNotify, invalid handle=0x%x"
#define PLAYER_MAIN_918_112_2_18_1_29_20_881 "DPLAYER_SetEndNotify, invalid handle=0x%x"
#define PLAYER_MAIN_952_112_2_18_1_29_21_882 "DPLAYER_GetCurrentFrameData()"
#define PLAYER_MAIN_957_112_2_18_1_29_21_883 "DPLAYER_GetCurrentFrameData, invalid handle=0x%x"
#define PLAYER_MAIN_963_112_2_18_1_29_21_884 "DPLAYER_GetCurrentFrameData, no invalid video frame."
#define PLAYER_MAIN_970_112_2_18_1_29_21_885 "DPLAYER_GetCurrentFrameData, invalid param."
#define PLAYER_MAIN_998_112_2_18_1_29_21_886 "DPLAYER_GetCurrentFrameData,fail, notify to stop."
#define PLAYER_MAIN_1041_112_2_18_1_29_21_887 "DPLAYER_GetNextFrameData, invalid handle=0x%x"
#define PLAYER_MAIN_1046_112_2_18_1_29_21_888 "DPLAYER_GetNextFrameData,pf_frm_rdy_callback=0x%x"
#define PLAYER_MAIN_1050_112_2_18_1_29_21_889 "DPLAYER_GetNextFrameData,busy,discard next frame"
#define PLAYER_MAIN_1092_112_2_18_1_29_21_890 "DPLAYER_GetNextFrameData,post process error,notify to stop."
#define PLAYER_MAIN_1113_112_2_18_1_29_21_891 "DPLAYER_SetAudioParam,new_dev=%d"
#define PLAYER_MAIN_1151_112_2_18_1_29_21_892 "DPLAYER_SetDisplayParam,%d,%d;%d,%d;%d"
#define PLAYER_MAIN_1162_112_2_18_1_29_21_893 "DPLAYER_SetDisplayParam,invalid [w,h]=[%d,%d]"
#define PLAYER_MAIN_1168_112_2_18_1_29_21_894 "DPLAYER_SetDisplayParam, invalid handle=0x%x"
#define PLAYER_MAIN_1227_112_2_18_1_29_21_895 "DPLAYER_ForceAVSync, invalid handle=0x%x"
#define PLAYER_MAIN_1257_112_2_18_1_29_21_896 "DPLAYER_SetPlaybackMode, invalid handle=0x%x"
#define PLAYER_MAIN_1271_112_2_18_1_29_21_897 "DPLAYER_SetPlaybackMode, invalid playback_mode=%d"
#define PLAYER_MAIN_1290_112_2_18_1_29_21_898 "DPLAYER_StreamRegistRxDataPath, invalid handle=0x%x"
#define PLAYER_MAIN_1310_112_2_18_1_29_21_899 "DPLAYER_StreamRxStart, invalid handle=0x%x"
#define PLAYER_MAIN_1328_112_2_18_1_29_21_900 "DPLAYER_StreamRxStart, invalid handle=0x%x"
#define PLAYER_MAIN_1360_112_2_18_1_29_21_901 "DPLAYER_ExternalPlugControl, cmd=%d,player_handle=0x%x,argv=0x%x"
#define PLAYER_MAIN_1365_112_2_18_1_29_21_902 "DPLAYER_ExternalPlugControl, invalid handle=0x%x"
#define PLAYER_MAIN_1400_112_2_18_1_29_21_903 "DPLAYER_ExternalPlugControl, end,plug_type=0x%x,argv=0x%x"
#define PLAYER_MAIN_1423_112_2_18_1_29_21_904 "_disp_notify_callback,p_player_ctx is NULL."
#define PLAYER_MAIN_1455_112_2_18_1_29_22_905 "_disp_notify_callback,disp fail,endnotify to stop."
#define PLAYER_MAIN_1537_112_2_18_1_29_22_906 "_CheckAndUpdateMovieInf, neigther video nor audio is supported!"
#define PLAYER_MAIN_1559_112_2_18_1_29_22_907 "DPLAYER,_get_video_dec_out_frm, no frame"
#define PLAYER_MAIN_1692_112_2_18_1_29_22_908 "DPLAYER,_video_postprocess_callback,p_v_frm_pp_out=0x%x"
#define PLAYER_MAIN_1696_112_2_18_1_29_22_909 "_video_postprocess_callback,p_player_ctx is NULL."
#define PLAYER_MAIN_1745_112_2_18_1_29_22_910 "DPLAYER,_set_pp_in_frame_info,unsupport fmt=%d"
#define PLAYER_MAIN_1759_112_2_18_1_29_22_911 "DPLAYER,_send2_postproc_frm,[%d x %d] - [%d x %d]"
#define PLAYER_NEW_232_112_2_18_1_29_23_912 "_player_save_open_param,open_type=%d, argc=%d,argv[1]=%d"
#define PLAYER_NEW_425_112_2_18_1_29_23_913 "_player_free_open_param,open_type=%d"
#define PLAYER_NEW_504_112_2_18_1_29_23_914 "_player_create_obj, object num =%d"
#define PLAYER_NEW_559_112_2_18_1_29_23_915 "_player_create_obj,obj list error--000"
#define PLAYER_NEW_586_112_2_18_1_29_23_916 "_player_create_obj,obj list error---111"
#define PLAYER_NEW_604_112_2_18_1_29_23_917 "_player_create_obj,ret, p_player_obj =0x%x,handle=0x%x,objs in list=%d"
#define PLAYER_NEW_621_112_2_18_1_29_24_918 "_player_destroy_obj,obj = 0x%x,obj in list=%d"
#define PLAYER_NEW_677_112_2_18_1_29_24_919 "_player_destroy_obj,cann't find the matched obj=0x%x"
#define PLAYER_NEW_800_112_2_18_1_29_24_920 "DPLAYER_Open, from file, argc=%d,argv[1]=%d"
#define PLAYER_NEW_843_112_2_18_1_29_24_921 "DPLAYER_Open, open_type(%d) is unsupport"
#define PLAYER_NEW_848_112_2_18_1_29_24_922 "DPLAYER_Open, PNULL == psz_access_type!"
#define PLAYER_NEW_855_112_2_18_1_29_24_923 "DPLAYER_Open, PNULL == psz_suffix"
#define PLAYER_NEW_862_112_2_18_1_29_24_924 "DPLAYER_Open, block_Init faild"
#define PLAYER_NEW_869_112_2_18_1_29_24_925 "DPLAYER_Open, module_LoadAll faild"
#define PLAYER_NEW_879_112_2_18_1_29_24_926 "DPLAYER_Open, failed to acquire access module:%s"
#define PLAYER_NEW_886_112_2_18_1_29_24_927 "DPLAYER_Open, invalidate access symbols"
#define PLAYER_NEW_896_112_2_18_1_29_24_928 "DPLAYER_Open, failed to open the file "
#define PLAYER_NEW_912_112_2_18_1_29_24_929 "DPLAYER_Open, get_file_type,=%s, peek_file_header,=%s"
#define PLAYER_NEW_921_112_2_18_1_29_24_930 "DPLAYER_Open, failed to acquire demux module:%s"
#define PLAYER_NEW_929_112_2_18_1_29_24_931 "DPLAYER_Open, invalidate demux symbols"
#define PLAYER_NEW_938_112_2_18_1_29_24_932 "DPLAYER_Open, demux_open, ret=%d"
#define PLAYER_NEW_971_112_2_18_1_29_24_933 "_player_unload_submodule"
#define PLAYER_NEW_976_112_2_18_1_29_24_934 "_player_unload_submodule,close demux"
#define PLAYER_NEW_983_112_2_18_1_29_24_935 "_player_unload_submodule,close stream"
#define PLAYER_STATE_120_112_2_18_1_29_25_936 "sci player, RunEngine(), current in error state"
#define PLAYER_STATE_270_112_2_18_1_29_25_937 "mplayer_DoDemux, error = %d"
#define PLAYER_STATE_468_112_2_18_1_29_25_938 "prepare_play_handle,start_time=%d,setted time=%d"
#define PLAYER_STATE_721_112_2_18_1_29_26_939 "player_state.c, demux_SetTimeVideo return SCI_ERROR"
#define PLAYER_STATE_832_112_2_18_1_29_26_940 "player_CreateTask, failed to create event"
#define PLAYER_STATE_850_112_2_18_1_29_26_941 "player_CreateTask, failed to create demux timer"
#define PLAYER_STATE_872_112_2_18_1_29_26_942 "player_Open, failed to create task"
#define PLAYER_STATE_961_112_2_18_1_29_26_943 "player_Play, current not on stop state"
#define PLAYER_STATE_987_112_2_18_1_29_26_944 "mplayer_Stop, all ready stop"
#define PLAYER_STATE_1014_112_2_18_1_29_26_945 "player_Pause, not on play state"
#define PLAYER_STATE_1037_112_2_18_1_29_27_946 "player_Resume, not on pause state"
#define PLAYER_STATE_1117_112_2_18_1_29_27_947 "player_SetTimetoSyncPoint, i_time=%d, i_destination=%d"
#define RECORDER_APP_180_112_2_18_1_29_27_948 "%s(Line%d), p_frame == PNULL"
#define RECORDER_APP_190_112_2_18_1_29_27_949 "_mrecorder_frame_callback,not ready,s_RECORD_State=%d"
#define RECORDER_APP_222_112_2_18_1_29_27_950 "%s(Line%d), p_frame == PNULL"
#define RECORDER_APP_230_112_2_18_1_29_27_951 "_mrecorder_frame_callback,not ready,s_RECORD_State=%d"
#define RECORDER_APP_308_112_2_18_1_29_27_952 "_atv_scan_channel_callback,p_info=null!"
#define RECORDER_APP_311_112_2_18_1_29_27_953 "_atv_scan_channel_callback:cmd=%d"
#define RECORDER_APP_476_112_2_18_1_29_28_954 "_camera_param_ctl,cmd=%d,arg=%d,callback=0x%x"
#define RECORDER_APP_483_112_2_18_1_29_28_955 "RECORDER_APP,DCAMERA_ParamCtrl,return error,"
#define RECORDER_APP_556_112_2_18_1_29_28_956 "RECORDER_APP,DCAMERA_GetInfo(), returns error"
#define RECORDER_APP_656_112_2_18_1_29_28_957 "convert_yuv2rgb, set ISP param error!"
#define RECORDER_APP_660_112_2_18_1_29_28_958 "convert_yuv2rgb blk buffer = 0x%x, size = %d"
#define RECORDER_APP_688_112_2_18_1_29_28_959 "convert_yuv2rgb, convert fail!"
#define RECORDER_APP_747_112_2_18_1_29_28_960 "_atv_turnoff_audio_play_path,audio_handle=0x%x"
#define RECORDER_APP_788_112_2_18_1_29_28_961 "_atv_turnon_audio_play_path,audio_handle=0x%x"
#define RECORDER_APP_794_112_2_18_1_29_28_962 "_atv_turnon_audio_play_path,play handle is active,0x%x"
#define RECORDER_APP_813_112_2_18_1_29_29_963 "_atv_turnon_audio_play_path,speaker handle=NULL."
#define RECORDER_APP_847_112_2_18_1_29_29_964 "_atv_turnon_audio_play_path,bt handle=NULL."
#define RECORDER_APP_879_112_2_18_1_29_29_965 "_atv_turnon_audio_play_path,unsupoort device=%d"
#define RECORDER_APP_932_112_2_18_1_29_29_966 "DRECORDER_Open, %d"
#define RECORDER_APP_946_112_2_18_1_29_29_967 "DRECORDER_Open,error,%d"
#define RECORDER_APP_974_112_2_18_1_29_29_968 "DRECORDER_Open,DCAMERA_Open failed"
#define RECORDER_APP_993_112_2_18_1_29_29_969 "DRECORDER_Close"
#define RECORDER_APP_1013_112_2_18_1_29_29_970 "DRECORDER_Close,error,%d"
#define RECORDER_APP_1053_112_2_18_1_29_29_971 "DRECORDER_SelectSrcDevice,new=%d,old=%d"
#define RECORDER_APP_1076_112_2_18_1_29_29_972 "DRECORDER_SelectSrcType,error"
#define RECORDER_APP_1108_112_2_18_1_29_29_973 "DRECORDER_SetVideoDispParam,0x%x"
#define RECORDER_APP_1135_112_2_18_1_29_29_974 "DRECORDER_ATV_SetAudioOutParam"
#define RECORDER_APP_1182_112_2_18_1_29_29_975 "DRECORDER_ATV_StartScanChn"
#define RECORDER_APP_1217_112_2_18_1_29_29_976 "DRECORDER_ATV_StopScanChn"
#define RECORDER_APP_1249_112_2_18_1_29_29_977 "DRECORDER_StartPreview, %d"
#define RECORDER_APP_1263_112_2_18_1_29_29_978 "StartPreview,error,%d"
#define RECORDER_APP_1282_112_2_18_1_29_29_979 "StartPreview,camera error"
#define RECORDER_APP_1290_112_2_18_1_29_29_980 "DRECORDER_StartPreview,_camera_start_preview error"
#define RECORDER_APP_1307_112_2_18_1_29_30_981 "DRECORDER_StartPreview,ATV _atv_turnon_audio_play_path error"
#define RECORDER_APP_1326_112_2_18_1_29_30_982 "DRECORDER_StopPreview"
#define RECORDER_APP_1341_112_2_18_1_29_30_983 "StopPreview,%d"
#define RECORDER_APP_1380_112_2_18_1_29_30_984 "DRECORDER_ParamCtrl,cmd=%d,arg=%d,callback=0x%x"
#define RECORDER_APP_1422_112_2_18_1_29_30_985 "DRECORDER_ParamCtrl,camera error"
#define RECORDER_APP_1437_112_2_18_1_29_30_986 "DRECORDER_ParamCtrl,_camera_stop_preview error"
#define RECORDER_APP_1444_112_2_18_1_29_30_987 "DRECORDER_ParamCtrl,_camera_param_ctl error"
#define RECORDER_APP_1451_112_2_18_1_29_30_988 "DRECORDER_ParamCtrl,_camera_start_preview error"
#define RECORDER_APP_1457_112_2_18_1_29_30_989 "DRECORDER_ParamCtrl,_atv_turnon_audio_play_path error"
#define RECORDER_APP_1469_112_2_18_1_29_30_990 "DRECORDER_ParamCtrl,_camera_param_ctl error"
#define RECORDER_APP_1492_112_2_18_1_29_30_991 "DRECORDER_GetInfo,,cmd=%d"
#define RECORDER_APP_1534_112_2_18_1_29_30_992 "DRECORDER_GetInfo,camera error"
#define RECORDER_APP_1565_112_2_18_1_29_30_993 "DRECORDER_RegisterGetLastFrameCallback,callback=0x%x,data_type=%d"
#define RECORDER_APP_1608_112_2_18_1_29_30_994 "DRECORDER_StartRecord, [%d][%d]"
#define RECORDER_APP_1615_112_2_18_1_29_30_995 "DRECORDER_StartRecord,Not support file type"
#define RECORDER_APP_1634_112_2_18_1_29_30_996 "StartRecord,error,%d"
#define RECORDER_APP_1641_112_2_18_1_29_30_997 "DRECORDER_StartRecord,camera error"
#define RECORDER_APP_1658_112_2_18_1_29_30_998 "DRECORDER_StartRecord,ATV _camera_start_capture error"
#define RECORDER_APP_1664_112_2_18_1_29_30_999 "DRECORDER_StartRecord,ATV _atv_turnon_audio_play_path error"
#define RECORDER_APP_1679_112_2_18_1_29_30_1000 "_REF_MRECORDER_Start() failed,%d"
#define RECORDER_APP_1701_112_2_18_1_29_30_1001 "_REF_MRECORDER_Start() failed,%d"
#define RECORDER_APP_1711_112_2_18_1_29_30_1002 "DRECORDER_StartRecord,DC _camera_start_capture error"
#define RECORDER_APP_1715_112_2_18_1_29_30_1003 "recorder_app.c, _REF_MRECORDER_Start() succ!"
#define RECORDER_APP_1733_112_2_18_1_29_30_1004 "DRECORDER_Pause"
#define RECORDER_APP_1800_112_2_18_1_29_30_1005 "DRECORDER_Resume"
#define RECORDER_APP_1850_112_2_18_1_29_31_1006 "DRECORDER_GetRecordInfo: data_size=%d,time=%d,frame=%d;"
#define RECORDER_APP_1866_112_2_18_1_29_31_1007 "DRECORDER_StopRecord"
#define RECORDER_APP_1881_112_2_18_1_29_31_1008 "StopRecord,invalid status"
#define RECORDER_APP_1907_112_2_18_1_29_31_1009 "DRECORDER_StopRecord,mrecorder_Stop error."
#define RECORDER_APP_1985_112_2_18_1_29_31_1010 "Not support recorder audio type,%d"
#define RECORDER_APP_2015_112_2_18_1_29_31_1011 "Not support recorder video type,%d"
#define RECORDER_APP_2021_112_2_18_1_29_31_1012 "recorder_app.c, both video and audio do not support!"
#define RECORDER_APP_2053_112_2_18_1_29_31_1013 "recorder_app.c,mrecorder_Open() error"
#define RECORDER_APP_2060_112_2_18_1_29_31_1014 "recorder_app.c:set end notify wrong."
#define RECORDER_APP_2068_112_2_18_1_29_31_1015 "recorder_app.c:mrecorder_Start wrong."
#define RECORDER_APP_2077_112_2_18_1_29_31_1016 "recorder_app.c,error_end:%d"
#define RECORDER_APP_2097_112_2_18_1_29_31_1017 "_mrecorder_getlastframe_callback,Y/UV=0x[%x,%x],w/h=[%d,%d]"
#define RECORDER_APP_2129_112_2_18_1_29_31_1018 "_mrecorder_atv_report,report type=%d,chn=%d,valid=%d"
#define RECORDER_NEW_166_112_2_18_1_29_32_1019 "recorder, error happen when waiting for event"
#define RECORDER_NEW_244_112_2_18_1_29_32_1020 "failed to stop audio encoding"
#define RECORDER_NEW_253_112_2_18_1_29_32_1021 "failed to stop video encoding"
#define RECORDER_NEW_301_112_2_18_1_29_32_1022 "--error:mrecoder_SetVideoQuality, %d"
#define RECORDER_NEW_319_112_2_18_1_29_32_1023 "mrecorder_init_task, failed to create event"
#define RECORDER_NEW_334_112_2_18_1_29_32_1024 "mrecorder_init_task, failed to create demux timer"
#define RECORDER_NEW_353_112_2_18_1_29_32_1025 "mrecorder_init_task, failed to create task"
#define RECORDER_NEW_375_112_2_18_1_29_32_1026 "mplayer, deactive, terminate the task fail, result[%d]"
#define RECORDER_NEW_381_112_2_18_1_29_32_1027 "mplayer, deactive, SCI_DeleteThread fail, result[%d]"
#define RECORDER_NEW_558_112_2_18_1_29_33_1028 "mrecorder_Open"
#define RECORDER_NEW_605_112_2_18_1_29_33_1029 "_mrecorder_open"
#define RECORDER_NEW_627_112_2_18_1_29_33_1030 "failed to acquire access module:%s"
#define RECORDER_NEW_632_112_2_18_1_29_33_1031 "invalidate access symbols"
#define RECORDER_NEW_649_112_2_18_1_29_33_1032 "failed to open stream"
#define RECORDER_NEW_657_112_2_18_1_29_33_1033 "failed to acquire mux module:%s"
#define RECORDER_NEW_662_112_2_18_1_29_33_1034 "invalidate demux symbols"
#define RECORDER_NEW_668_112_2_18_1_29_33_1035 "failed to open mux module"
#define RECORDER_NEW_683_112_2_18_1_29_33_1036 "failed to acquire audio encoder:0x%x"
#define RECORDER_NEW_706_112_2_18_1_29_33_1037 "invalidate audio codec symbols"
#define RECORDER_NEW_711_112_2_18_1_29_33_1038 "failed to open audio codec"
#define RECORDER_NEW_729_112_2_18_1_29_33_1039 "failed to acquire video encoder:0x%x"
#define RECORDER_NEW_750_112_2_18_1_29_33_1040 "invalidate video codec symbols"
#define RECORDER_NEW_755_112_2_18_1_29_33_1041 "failed to open video codec"
#define RECORDER_NEW_791_112_2_18_1_29_33_1042 "mrecorder_Close"
#define RECORDER_NEW_815_112_2_18_1_29_33_1043 "_mrecorder_close"
#define RECORDER_NEW_862_112_2_18_1_29_34_1044 "mrecorder_Start"
#define RECORDER_NEW_877_112_2_18_1_29_34_1045 "_mrecorder_start"
#define RECORDER_NEW_881_112_2_18_1_29_34_1046 "mrecorder_Start(), mux is not open"
#define RECORDER_NEW_926_112_2_18_1_29_34_1047 "mrecorder_Stop"
#define RECORDER_NEW_965_112_2_18_1_29_34_1048 "_mrecorder_stop_step1_sync"
#define RECORDER_NEW_998_112_2_18_1_29_34_1049 "_mrecorder_stop_step2_async"
#define RECORDER_NEW_1055_112_2_18_1_29_34_1050 "mrecorder_Pause"
#define RECORDER_NEW_1070_112_2_18_1_29_34_1051 "_mrecorder_pause"
#define RECORDER_NEW_1093_112_2_18_1_29_34_1052 "mrecorder_Pause"
#define RECORDER_NEW_1108_112_2_18_1_29_34_1053 "_mrecorder_resume"
#define RECORDER_NEW_1135_112_2_18_1_29_34_1054 "mrecorder_GetRecordInfo"
#define RECORDER_NEW_1172_112_2_18_1_29_34_1055 "recoder_new.c, getrecoderInfo error!"
#define RECORDER_NEW_1442_112_2_18_1_29_35_1056 "_mrecorder_send_cmd,ctl_task hasn't been created yet."
#define RECORDER_NEW_1453_112_2_18_1_29_35_1057 "_mrecorder_send_cmd, discard MRECORDER_CTL_TIMER_TRIGGER."
#define RECORDER_NEW_1472_112_2_18_1_29_35_1058 "_mrecorder_send_cmd failed,cmd =%d"
#define VIDEOCODEC_TASK_158_112_2_18_1_29_36_1059 "SendCommand, videocodec task is not exit. cmd =%d"
#define VIDEOCODEC_TASK_177_112_2_18_1_29_36_1060 "videocodec, SendCommand, SCI_SendSignal failed, cmd=%d"
#define VIDEOCODEC_TASK_442_112_2_18_1_29_36_1061 "VideoEnc_MP4_H263_Init,pBuffer->size=%d,too small"
#define VIDEOCODEC_TASK_544_112_2_18_1_29_37_1062 "video_enc:period=%d,dec=[%d,%d,%d,%d],enc=[%d,%d,%d,%d]"
#define VIDEOCODEC_TASK_682_112_2_18_1_29_37_1063 "VideoEncGetConfig:CAL_VIDEO_TYPE_H264"
#define VIDEOCODEC_TASK_687_112_2_18_1_29_37_1064 "VideoEncGetConfig:Default"
#define VIDEOCODEC_TASK_896_112_2_18_1_29_37_1065 "video_dec:period=%d,dec=[%d,%d,%d,%d],enc=[%d,%d,%d,%d]"
#define VIDEOCODEC_TASK_1156_112_2_18_1_29_38_1066 "VideoDecDecode,pOutFrameYuv =0x(%x,%x,%x)"
#define VIDEOCODEC_TASK_1236_112_2_18_1_29_38_1067 "VideoCodecTask_Destroy,task_id =%d,event=0x%x"
#define VT_INTG_593_112_2_18_1_29_40_1068 "_malloc_local_replace_blkbuf,failed"
#define VT_INTG_778_112_2_18_1_29_40_1069 "VT open vt senor err"
#define VT_INTG_807_112_2_18_1_29_40_1070 "_check_vt_sensor_id,sensor_id=%d,sensor_num=%d"
#define VT_INTG_819_112_2_18_1_29_40_1071 "_get_cur_vt_sensor_id=%d"
#define VT_INTG_843_112_2_18_1_29_40_1072 "_set_cur_vt_sensor_id=%d"
#define VT_INTG_874_112_2_18_1_29_40_1073 "_get_img_rotate_angle,screen2sensor_angle=%d,img_out_angle=%d"
#define VT_INTG_901_112_2_18_1_29_40_1074 "--vt_intg,_start_DCamera_preview,lcd:%d,%d,disp_rect=%d,%d,%d,%d"
#define VT_INTG_956_112_2_18_1_29_40_1075 "--vt_intg,_stop_DCamera_preview"
#define VT_INTG_977_112_2_18_1_29_40_1076 "_isp_control_init failed"
#define VT_INTG_991_112_2_18_1_29_40_1077 "_isp_control_release failed"
#define VT_INTG_1005_112_2_18_1_29_40_1078 "_isp_control_lock failed"
#define VT_INTG_1019_112_2_18_1_29_40_1079 "_isp_control_unlock failed"
#define VT_INTG_1034_112_2_18_1_29_40_1080 "--isp_callback_vtcapture,fifo_depth=%d,y/u/v=0x(%x,%x,%x)"
#define VT_INTG_1054_112_2_18_1_29_40_1081 "----display,ISP callback,p_rgb565_frame=0x%x"
#define VT_INTG_1084_112_2_18_1_29_41_1082 "--vt_intg,_set_ISP_vt_encode_param,disp_range(x,y;w,h)=(%d,%d;%d,%d)"
#define VT_INTG_1221_112_2_18_1_29_41_1083 "--vt_intg,_set_ISP_vt_decode_param:range(x,y;w,h)=(%d,%d;%d,%d)"
#define VT_INTG_1277_112_2_18_1_29_41_1084 "_vtmw, _start_isp_vt"
#define VT_INTG_1296_112_2_18_1_29_41_1085 "_vtmw, _stop_isp_vt"
#define VT_INTG_1350_112_2_18_1_29_41_1086 "----display_param--yuv=0x(%x,%x,%x);size(w,h)=(%d,%d);range(x0,y0;w,h)=(%d,%d;%d,%d)"
#define VT_INTG_1367_112_2_18_1_29_41_1087 "_send_to_disp_remote_frame,is_isp_ready=0x%x,is_remote_disp=%d"
#define VT_INTG_1416_112_2_18_1_29_41_1088 "_send_to_disp_local_frame,is_isp_ready=0x%x,is_local_disp=%d"
#define VT_INTG_1452_112_2_18_1_29_41_1089 "_send_to_disp_local_frame,rgb565_convert_time out:%d"
#define VT_INTG_1456_112_2_18_1_29_41_1090 "_send_to_disp_local_frame,rgb565_convert_time:%d"
#define VT_INTG_1482_112_2_18_1_29_41_1091 "_send_to_disp_local_frame, set ISP param error!"
#define VT_INTG_1486_112_2_18_1_29_41_1092 "vt replace scale lcd blk buffer=0x%x,input_addr=0x%x, size=%d"
#define VT_INTG_1559_112_2_18_1_29_41_1093 "_vtmw_err_notify,%d"
#define VT_INTG_1610_112_2_18_1_29_42_1094 "_video_stream_frm_malloc,len=%d is too large"
#define VT_INTG_1666_112_2_18_1_29_42_1095 "_video_stream_frm_free,strm_frm=0x%x not found"
#define VT_INTG_1749_112_2_18_1_29_42_1096 "DVT_MW_Open"
#define VT_INTG_1796_112_2_18_1_29_42_1097 "[DVT_MW_Close],status=%d"
#define VT_INTG_1858_112_2_18_1_29_42_1098 "DVT_MW_SelectSensor"
#define VT_INTG_1881_112_2_18_1_29_42_1099 "DVT_MW_SelectSensor_Ex"
#define VT_INTG_1909_112_2_18_1_29_42_1100 "DVT_MW_GetSensorParam"
#define VT_INTG_1938_112_2_18_1_29_42_1101 "DVT_MW_SetSensorParam"
#define VT_INTG_2002_112_2_18_1_29_42_1102 "DVT_MW_PreviewStart"
#define VT_INTG_2028_112_2_18_1_29_42_1103 "DVT_MW_PreviewStop"
#define VT_INTG_2055_112_2_18_1_29_42_1104 "DVT_MW_SetDisplayArea"
#define VT_INTG_2083_112_2_18_1_29_43_1105 "DVT_MW_DialStart"
#define VT_INTG_2107_112_2_18_1_29_43_1106 "DVT_MW_DialStop"
#define VT_INTG_2132_112_2_18_1_29_43_1107 "DVT_MW_RecordStart"
#define VT_INTG_2158_112_2_18_1_29_43_1108 "DVT_MW_RecordStop"
#define VT_INTG_2183_112_2_18_1_29_43_1109 "DVT_MW_ReplaceLocalStart"
#define VT_INTG_2211_112_2_18_1_29_43_1110 "DVT_MW_ReplaceLocalStop"
#define VT_INTG_2239_112_2_18_1_29_43_1111 "DVT_MW_EnableMute,en=%d,obj=0x%x"
#define VT_INTG_2403_112_2_18_1_29_43_1112 "_vt_codec_getvideotype,bit[31:37]=0x%x"
#define VT_INTG_2543_112_2_18_1_29_43_1113 "[DVT_CODEC_VideoEncInit]"
#define VT_INTG_2559_112_2_18_1_29_43_1114 "DVT_CODEC_VideoEncInit,p_vol_enc malloc failed."
#define VT_INTG_2585_112_2_18_1_29_44_1115 "[DVT_CODEC_VideoEncRelease]"
#define VT_INTG_2608_112_2_18_1_29_44_1116 "[DVT_CODEC_GetVideoEncAbility]"
#define VT_INTG_2626_112_2_18_1_29_44_1117 "DVT_CODEC_GetVideoEncCfg"
#define VT_INTG_2647_112_2_18_1_29_44_1118 "[DVT_CODEC_SetVideoEncCfg],codec_type=%d"
#define VT_INTG_2651_112_2_18_1_29_44_1119 "[DVT_CODEC_SetVideoEncCfg],release the ENC before reconfiguration.EncStatus=%d"
#define VT_INTG_2716_112_2_18_1_29_44_1120 "DVT_CODEC_GetVideoEncHeader,0x%x,%d"
#define VT_INTG_2729_112_2_18_1_29_44_1121 "DVT_CODEC_GetVideoEncHeader,len=%d"
#define VT_INTG_2747_112_2_18_1_29_44_1122 "DVT_CODEC_GetVideoEncHeader,header=0x[%x,%x,%x,%x]"
#define VT_INTG_2778_112_2_18_1_29_44_1123 "DVT_CODEC_VideoEncHostDil,IntraFrame=%d,buffer_size=%d,tick(cur-last)=%d,fifo_depth=%d"
#define VT_INTG_2862_112_2_18_1_29_44_1124 "[DVT_CODEC_VideoDecInit]"
#define VT_INTG_2875_112_2_18_1_29_44_1125 "DVT_CODEC_VideoDecInit,p_vol_dec malloc failed."
#define VT_INTG_2897_112_2_18_1_29_44_1126 "DVT_CODEC_VideoDecRelease"
#define VT_INTG_2923_112_2_18_1_29_44_1127 "[DVT_CODEC_GetVideoDecAbility]"
#define VT_INTG_2942_112_2_18_1_29_44_1128 "DVT_CODEC_GetVideoDecConf"
#define VT_INTG_2959_112_2_18_1_29_44_1129 "DVT_CODEC_SetVideoDecConf"
#define VT_INTG_2987_112_2_18_1_29_44_1130 "[DVT_CODEC_VideoDecHostDil],pdata=0x%x,len=%d,err=%d"
#define VT_INTG_3007_112_2_18_1_29_44_1131 "DVT_CODEC_VideoDecHostDil,_vt_codec_getvideotype failed,video_type=%d,vop_type=%d"
#define VT_INTG_3019_112_2_18_1_29_44_1132 "DVT_CODEC_VideoDecHostDil,_video_stream_frm_malloc failed,depth=%d"
#define VT_INTG_3030_112_2_18_1_29_44_1133 "DVT_CODEC_VideoDecHostDil,block_New failed,depth=%d"
#define VT_INTG_3141_112_2_18_1_29_45_1134 "_amr_enc_callback,"
#define VT_INTG_3188_112_2_18_1_29_45_1135 "_amr_downlink_callback,i_buffer=%d > len_max"
#define VT_INTG_3204_112_2_18_1_29_45_1136 "_amr_downlink_callback,block_FifoRemove failed."
#define VT_INTG_3215_112_2_18_1_29_45_1137 "_amr_downlink_callback,data is not ready."
#define VT_INTG_3237_112_2_18_1_29_45_1138 "_amr_downlink_callback,VT_EXT_WRITEVOICEDATA err, ret=%d,%d"
#define VT_INTG_3284_112_2_18_1_29_45_1139 "_amr_uplink_callback,VT_EXT_READVOICEDATA err, ret=%d,%d"
#define VT_INTG_3320_112_2_18_1_29_45_1140 "_amr_uplink_callback,block alloc failed.fifo_depth=%d"
#define VT_INTG_3365_112_2_18_1_29_45_1141 "_pcm_record_callback,VT_EXT_READRECORDDATA err, ret=%d,%d"
#define VT_INTG_3409_112_2_18_1_29_45_1142 "_vtmw_audio_start"
#define VT_INTG_3415_112_2_18_1_29_45_1143 "_vtmw_audio_start,duplicate"
#define VT_INTG_3429_112_2_18_1_29_45_1144 "_vtmw_audio_start,AUDIO_CreateNormalHandle,invalid handle"
#define VT_INTG_3449_112_2_18_1_29_45_1145 "_vtmw_audio_start,VT_EXT_START err, ret=%d,%d"
#define VT_INTG_3467_112_2_18_1_29_45_1146 "_vtmw_audio_stop"
#define VT_INTG_3482_112_2_18_1_29_45_1147 "_vtmw_audio_stop,VT_EXT_STOP err, ret=%d,%d"
#define VT_INTG_3490_112_2_18_1_29_45_1148 "_vtmw_audio_stop,AUDIO_CloseHandle err,%d"
#define VT_INTG_3513_112_2_18_1_29_45_1149 "_vtmw_start_record_audio"
#define VT_INTG_3532_112_2_18_1_29_45_1150 "_vtmw_start_record_audio,VT_EXT_STARTRECORD err, ret=%d,%d"
#define VT_INTG_3546_112_2_18_1_29_45_1151 "_vtmw_stop_record_audio"
#define VT_INTG_3560_112_2_18_1_29_46_1152 "_vtmw_stop_record_audio,VT_EXT_STOPRECORD err, ret=%d,%d"
#define VT_INTG_3600_112_2_18_1_29_46_1153 "_amr_byte2halfword,len_src=%d > 31"
#define VT_INTG_3656_112_2_18_1_29_46_1154 "_amr_convert_IF2_to_MMS,frame type=%d,size=%d,"
#define VT_INTG_3660_112_2_18_1_29_46_1155 "_amr_convert_IF2_to_MMS,parametr check failed"
#define VT_INTG_3729_112_2_18_1_29_46_1156 "_amr_convert_IF2_to_MMS,silence frame"
#define VT_INTG_3808_112_2_18_1_29_46_1157 "DVT_CODEC_AmrEncOpen"
#define VT_INTG_3824_112_2_18_1_29_46_1158 "DVT_CODEC_AmrEncClose"
#define VT_INTG_3840_112_2_18_1_29_46_1159 "DVT_CODEC_AmrEncSetMode"
#define VT_INTG_3852_112_2_18_1_29_46_1160 "DVT_CODEC_AmrEncFlush"
#define VT_INTG_3865_112_2_18_1_29_46_1161 "DVT_CODEC_AmrEncStartCapture"
#define VT_INTG_3910_112_2_18_1_29_46_1162 "DVT_CODEC_AmrEncReadAFrame,i_buffer=%d > buf_len=%d"
#define VT_INTG_3959_112_2_18_1_29_46_1163 "DVT_CODEC_AmrEncReadAFrame,FifoRemove err."
#define VT_INTG_3977_112_2_18_1_29_46_1164 "DVT_CODEC_AmrDecOpen"
#define VT_INTG_3990_112_2_18_1_29_46_1165 "DVT_CODEC_AmrDecClose"
#define VT_INTG_4020_112_2_18_1_29_46_1166 "DVT_CODEC_AmrDecWriteAFrame,fifo_depth=%d"
#define VT_INTG_4070_112_2_18_1_29_47_1167 "DVT_CODEC_AmrDecWriteAFrame,block alloc failed,discard the frame"
#define VT_INTG_4209_112_2_18_1_29_47_1168 "_vtmw_open,pf_err_notify=0x%x"
#define VT_INTG_4229_112_2_18_1_29_47_1169 "_vtmw_open,_vtmw_modules_load error"
#define VT_INTG_4277_112_2_18_1_29_47_1170 "_vtmw_close,status=%d"
#define VT_INTG_4358_112_2_18_1_29_47_1171 "_vtmw_set_display_area,status=%d,p_remote=0x%x,p_local=0x%x"
#define VT_INTG_4364_112_2_18_1_29_47_1172 "_vtmw_set_display_area,remote_disp_block=[%d,%d,%d]"
#define VT_INTG_4370_112_2_18_1_29_47_1173 "_vtmw_set_display_area,local_disp_block=[%d,%d,%d]"
#define VT_INTG_4517_112_2_18_1_29_47_1174 "_vtmw_set_display_area,exit"
#define VT_INTG_4525_112_2_18_1_29_47_1175 "_vtmw_preview_start"
#define VT_INTG_4529_112_2_18_1_29_47_1176 "_vtmw_preview_start,local disp rect is invalid"
#define VT_INTG_4558_112_2_18_1_29_47_1177 "_vtmw_preview_stop"
#define VT_INTG_4585_112_2_18_1_29_48_1178 "_vtmw_dial_start"
#define VT_INTG_4634_112_2_18_1_29_48_1179 "_vtmw_dial_start,exit"
#define VT_INTG_4643_112_2_18_1_29_48_1180 "_vtmw_dial_stop,status=%d"
#define VT_INTG_4746_112_2_18_1_29_48_1181 "_vtmw_sensor_para_get,ret=%d,cmd=%d,para=%d"
#define VT_INTG_4763_112_2_18_1_29_48_1182 "_vtmw_sensor_para_set,invalid param,cmdwparam=%d,cmdlparam=0x%x"
#define VT_INTG_4769_112_2_18_1_29_48_1183 "_vtmw_sensor_para_set,cmd=%d,para_ptr=0x%x"
#define VT_INTG_4851_112_2_18_1_29_48_1184 "_vtmw_sensor_select_ex,cmdwparam=%d,cmdlparam=0x%x"
#define VT_INTG_4862_112_2_18_1_29_48_1185 "_vtmw_sensor_select_ex,id_expect=%d,angle=%d"
#define VT_INTG_4876_112_2_18_1_29_48_1186 "_vtmw_sensor_select,vt_sensor_id_expect is useless"
#define VT_INTG_4899_112_2_18_1_29_48_1187 "_vtmw_sensor_select,vt_sensor_id_expect is useless"
#define VT_INTG_5145_112_2_18_1_29_49_1188 "_rec_audio_send2mux,failed"
#define VT_INTG_5166_112_2_18_1_29_49_1189 "_rec_vido_send2mux,failed"
#define VT_INTG_5210_112_2_18_1_29_49_1190 "_vtmw_record_start,p_record_info=0x%x:[%d,%d]"
#define VT_INTG_5228_112_2_18_1_29_49_1191 "VT,failed to AcquireMuxAccessModule"
#define VT_INTG_5236_112_2_18_1_29_49_1192 "VT,invalidate access symbols"
#define VT_INTG_5259_112_2_18_1_29_49_1193 "VT,failed to stream_Open"
#define VT_INTG_5272_112_2_18_1_29_49_1194 "VT,failed to acquire mux module"
#define VT_INTG_5281_112_2_18_1_29_49_1195 "VT,invalidate mux symbols"
#define VT_INTG_5290_112_2_18_1_29_49_1196 "VT,failed to open mux module"
#define VT_INTG_5316_112_2_18_1_29_49_1197 "vt_intg.c, recorder start, audio111 error!"
#define VT_INTG_5349_112_2_18_1_29_49_1198 "vt_intg.c, recorder start, audio222 error"
#define VT_INTG_5386_112_2_18_1_29_49_1199 "vt_intg.c, recorder start, video size error!"
#define VT_INTG_5404_112_2_18_1_29_49_1200 "vt_intg.c, recorder start, video error!"
#define VT_INTG_5415_112_2_18_1_29_49_1201 "_vtmw_record_start,no video nor audio"
#define VT_INTG_5451_112_2_18_1_29_49_1202 "_vtmw_record_start,rec_flag set:0x%x"
#define VT_INTG_5463_112_2_18_1_29_49_1203 "_vtmw_record_stop"
#define VT_INTG_5577_112_2_18_1_29_49_1204 "_read_jpeg_file2buf,file open error."
#define VT_INTG_5584_112_2_18_1_29_50_1205 "jpg_stream_size=%d,too large file"
#define VT_INTG_5593_112_2_18_1_29_50_1206 "vt_intg.c, _read_jpeg_file2buf, alloc buffer fail!"
#define VT_INTG_5604_112_2_18_1_29_50_1207 "vt_intg.c, _read_jpeg_file2buf, read file error!"
#define VT_INTG_5642_112_2_18_1_29_50_1208 "_check_jpeg_info, invalid JPG"
#define VT_INTG_5668_112_2_18_1_29_50_1209 "_dec_scale_jpeg,stream=0x%x,len=%d,target size=[%dx%d]"
#define VT_INTG_5677_112_2_18_1_29_50_1210 "vt_intg.c, _dec_scale_jpeg, alloc common buffer error!"
#define VT_INTG_5695_112_2_18_1_29_50_1211 "vt_intg.c, _dec_scale_jpeg, alloc yuv buffer error!"
#define VT_INTG_5721_112_2_18_1_29_50_1212 "vt_intg.c, _dec_scale_jpeg, decode fail"
#define VT_INTG_5731_112_2_18_1_29_50_1213 "_dec_scale_jpeg,dec out: size=%dx%d,fmt=%d,%d"
#define VT_INTG_6026_112_2_18_1_29_50_1214 "_convert_yuv_frame,format:0x%x,%d"
#define VT_INTG_6031_112_2_18_1_29_50_1215 "vt_intg.c, _convert_yuv_frame, width or height is invalid"
#define VT_INTG_6114_112_2_18_1_29_51_1216 "_vtmw_replace_start,p_replace_info=0x%x:[%d,%d,%d,%d]"
#define VT_INTG_6194_112_2_18_1_29_51_1217 "_vtmw_replace_start,alloc p_replace_yuv420_buf_local failed"
#define VT_INTG_6212_112_2_18_1_29_51_1218 "_vtmw_replace_start,alloc p_replace_yuv420_buf_remote failed"
#define VT_INTG_6250_112_2_18_1_29_51_1219 "_vtmw_replace_start,_convert_yuv_frame failed"
#define VT_INTG_6355_112_2_18_1_29_51_1220 "_vtmw_replace_stop"
#define VT_INTG_6432_112_2_18_1_29_51_1221 "_vtmw_enablemute,cmdwparam=%d,cmdlparam=0x%x"
#define VT_INTG_6480_112_2_18_1_29_51_1222 "_video_enc_save_vol,too large,%d"
#define VT_INTG_6504_112_2_18_1_29_51_1223 "_video_dec_save_vol,p_vol=0x%x,len=%d,data=0x[%x,%x,%x,%x]"
#define VT_INTG_6516_112_2_18_1_29_51_1224 "_video_dec_save_vol,too large,%d"
#define VT_INTG_6545_112_2_18_1_29_52_1225 "_video_decode_one_frame,_vt_codec_getvideotype failed,video_type=%d,vop_type=%d"
#define VT_INTG_6581_112_2_18_1_29_52_1226 "_video_decode_one_frame,MPEG4 configure,len=%d"
#define VT_INTG_6594_112_2_18_1_29_52_1227 "failed to call CAL_VideoDecInit()"
#define VT_INTG_6609_112_2_18_1_29_52_1228 "_video_decode_one_frame,H.263 configure"
#define VT_INTG_6618_112_2_18_1_29_52_1229 "failed to call CAL_VideoDecInit()"
#define VT_INTG_6637_112_2_18_1_29_52_1230 "_video_decode_one_frame,discard cur frame,wait for one I-vop,vop_type=%d"
#define VT_INTG_6685_112_2_18_1_29_52_1231 "_video_decode_one_frame,DecOutput,y_addr =0x%x,err_MB_num=%d"
#define VT_INTG_6693_112_2_18_1_29_52_1232 "_video_decode_one_frame,DecOutput null y_plane"
#define VT_INTG_6739_112_2_18_1_29_52_1233 "_video_decode_one_frame,decoder failed.ret=%d"
#define VT_INTG_6790_112_2_18_1_29_52_1234 "_vtmw_video_enc_one_frame,local video is muted"
#define VT_INTG_6802_112_2_18_1_29_52_1235 "_vtmw_video_enc_one_frame,enc is not ready yet,%d"
#define VT_INTG_6808_112_2_18_1_29_52_1236 "_vtmw_video_enc_one_frame,up_fifo is alomost full,%d"
#define VT_INTG_6816_112_2_18_1_29_52_1237 "_vtmw_video_enc_one_frame,stream_frm_malloc failed"
#define VT_INTG_6821_112_2_18_1_29_52_1238 "_vtmw_video_enc_one_frame,block_New failed"
#define VT_INTG_6862_112_2_18_1_29_52_1239 "_vtmw_video_enc_one_frame,block remove failed[%d]"
#define VT_INTG_6903_112_2_18_1_29_52_1240 "_video_enc_one_frame,cache_len_bit=%d,tx_bit_fract=%d"
#define VT_INTG_6919_112_2_18_1_29_52_1241 "_vtmw_video_enc_one_frame:ret=%d,enc_size=%d"
#define VT_INTG_6948_112_2_18_1_29_52_1242 "_vtmw_video_enc_one_frame:buf_size(%d) < enc_size(%d)"
#define VT_INTG_7084_112_2_18_1_29_53_1243 "_vtmw_timer_trigger_process,adpcm_enc,fifo_depth=%d"
#define VT_INTG_7128_112_2_18_1_29_53_1244 "_vtmw_timer_trigger_process,mux failed,i_ret[%d]"
#define VT_INTG_7231_112_2_18_1_29_53_1245 "_vtmw_ctl_send_cmd,ctl_task hasn't been created yet[0x%x]."
#define VT_INTG_7242_112_2_18_1_29_53_1246 "_vtmw_ctl_send_cmd, discard VTMW_CTL_TIMER_TRIGGER."
#define VT_INTG_7261_112_2_18_1_29_53_1247 "_vtmw_ctl_send_cmd failed,cmd =%d"
#define VT_INTG_7381_112_2_18_1_29_53_1248 "_vtmw_TaskInit,in,task_id =%d"
#define VT_INTG_7405_112_2_18_1_29_53_1249 "_vtmw_TaskInit,create failed"
#define VT_INTG_7412_112_2_18_1_29_53_1250 "_vtmw_TaskInit,out,task_id =%d"
#define VT_INTG_7422_112_2_18_1_29_53_1251 "_vtmw_TaskRelease,in,task_id =%d"
#define VT_INTG_7542_112_2_18_1_29_53_1252 "_clear_lcd_rect,[%d,%d,%d,%d]"
#define VT_INTG_7555_112_2_18_1_29_54_1253 "VT_Loop_Err_callback,err type=%d"
#define VT_INTG_7615_112_2_18_1_29_54_1254 "VT_loop_start_codec,set audio device mode error,%d"
#define VT_INTG_7653_112_2_18_1_29_54_1255 "VT_Loop_Change_mode,cur_mode=%d"
#define VT_INTG_7857_112_2_18_1_29_54_1256 " VT_Loop_Self_Test_Entry, ret=%d,event=%d"
#define VT_INTG_7866_112_2_18_1_29_54_1257 "VT_Loop_Self_Test_Entry, receive a start event"
#define VT_INTG_7880_112_2_18_1_29_54_1258 "VT_Loop_Self_Test_Entry, receive a exit event"
#define VT_INTG_7913_112_2_18_1_29_54_1259 "VT_Loop_Self_Test_Entry, EVENT_TIME_VT"
#define VT_INTG_7950_112_2_18_1_29_54_1260 "VT_Loop_Self_Test_Entry,read uplink amr,%d,0x[%x,%x,%x,%x]"
#define VT_INTG_7975_112_2_18_1_29_54_1261 "VT_Loop_Self_Start"
#define VT_INTG_7979_112_2_18_1_29_54_1262 "VT_Loop_Self_Start,open duplicate"
#define VT_INTG_8020_112_2_18_1_29_54_1263 "VT_Loop_Self_Stop"
#define VT_INTG_8060_112_2_18_1_29_55_1264 "VT_Loop_Self_Change"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_DV_TRC)
TRACE_MSG(H264_DEC_V0_288_112_2_18_1_27_21_0,"h264_dec task, MP4_DECODER_PAUSE_EVENT")
TRACE_MSG(H264_DEC_V0_345_112_2_18_1_27_21_1,"h264_dec,video is too slow,seek one I VOP,syn_time[%d]")
TRACE_MSG(H264_DEC_V0_349_112_2_18_1_27_21_2,"h264_dec,video is too slow, but no I VOP in next 1000ms. syn_time[%d]")
TRACE_MSG(H264_DEC_V0_402_112_2_18_1_27_21_3,"h264_dec, DecoderTaskEntry, failed to call set event")
TRACE_MSG(H264_DEC_V0_418_112_2_18_1_27_21_4,"h264_dec, disp_skip=%d, time_offset_a_v %d, b_expected_IVOP %d")
TRACE_MSG(H264_DEC_V0_423_112_2_18_1_27_21_5,"h264_dec, _Peek_NextDispFrameContain false,wait for next time")
TRACE_MSG(H264_DEC_V0_453_112_2_18_1_27_21_6,"h264_dec, DecoderTaskEntry, frame_width=%d, frame_height=%d, extra_buffer_size=%d,")
TRACE_MSG(H264_DEC_V0_457_112_2_18_1_27_21_7,"h264_dec, DecoderTaskEntry, H264DecMemSizeCal return error")
TRACE_MSG(H264_DEC_V0_469_112_2_18_1_27_21_8,"h264_dec, DecoderTaskEntry, frame_width=%d, frame_height=%d, Error or unsupport")
TRACE_MSG(H264_DEC_V0_479_112_2_18_1_27_21_9,"h264_dec, DecoderTaskEntry, p_decoder_buffer_extra == PNULL, return MMDEC_NOT_SUPPORTED")
TRACE_MSG(H264_DEC_V0_491_112_2_18_1_27_21_10,"h264_dec.c, DecoderTaskEntry, H264DecMemInit failed!")
TRACE_MSG(H264_DEC_V0_500_112_2_18_1_27_21_11,"h264_dec, DecoderTaskEntry, malloc OK, frame_width=%d, frame_height=%d, extra_buffer_size=%d,")
TRACE_MSG(H264_DEC_V0_539_112_2_18_1_27_22_12,"h264_dec,[w,h]=[%d,%d],result=%d")
TRACE_MSG(H264_DEC_V0_556_112_2_18_1_27_22_13,"h264_dec task, decode_result, MMDEC_OUTPUT_BUFFER_OVERFLOW")
TRACE_MSG(H264_DEC_V0_625_112_2_18_1_27_22_14,"h264_dec, block_new failed.")
TRACE_MSG(H264_DEC_V0_638_112_2_18_1_27_22_15,"h264_dec task, decode_result, MMDEC_ERROR")
TRACE_MSG(H264_DEC_V0_644_112_2_18_1_27_22_16,"h264_dec task, decode_result, MMDEC_PARAM_ERROR")
TRACE_MSG(H264_DEC_V0_650_112_2_18_1_27_22_17,"h264_dec task, decode_result, MMDEC_MEMORY_ERROR")
TRACE_MSG(H264_DEC_V0_654_112_2_18_1_27_22_18,"h264_dec task, decode_result, MMDEC_INVALID_STATUS")
TRACE_MSG(H264_DEC_V0_658_112_2_18_1_27_22_19,"h264_dec task, decode_result, MMDEC_STREAM_ERROR")
TRACE_MSG(H264_DEC_V0_664_112_2_18_1_27_22_20,"h264_dec task, decode_result, MMDEC_OUTPUT_BUFFER_OVERFLOW")
TRACE_MSG(H264_DEC_V0_668_112_2_18_1_27_22_21,"h264_dec task, decode_result, MMDEC_HW_ERROR")
TRACE_MSG(H264_DEC_V0_673_112_2_18_1_27_22_22,"h264_dec task, decode_result, MMDEC_NOT_SUPPORTED")
TRACE_MSG(H264_DEC_V0_678_112_2_18_1_27_22_23,"h264_dec task, decode_result, MMDEC_FRAME_SEEK_IVOP")
TRACE_MSG(H264_DEC_V0_682_112_2_18_1_27_22_24,"h264_dec task, decode_result=%d, return err!")
TRACE_MSG(H264_DEC_V0_698_112_2_18_1_27_22_25,"mpeg4 decoder task, encount undefined event,i_event_flag[0x%x],status[0x%x]")
TRACE_MSG(H264_DEC_V0_708_112_2_18_1_27_22_26,"h264_dec, Active()")
TRACE_MSG(H264_DEC_V0_721_112_2_18_1_27_22_27,"h264_dec, Deactive()")
TRACE_MSG(H264_DEC_V0_735_112_2_18_1_27_22_28,"h264_dec.c,close,status error!")
TRACE_MSG(H264_DEC_V0_741_112_2_18_1_27_22_29,"h264_dec.c, open, movie size is too large.")
TRACE_MSG(H264_DEC_V0_777_112_2_18_1_27_22_30,"failed to call H264DecInit()")
TRACE_MSG(H264_DEC_V0_791_112_2_18_1_27_22_31,"h264_dec, Open(), failed to create event")
TRACE_MSG(H264_DEC_V0_816_112_2_18_1_27_22_32,"h264_dec, Open(), failed to create task")
TRACE_MSG(H264_DEC_V0_871_112_2_18_1_27_22_33,"h264_dec, close(),status[0x%x]")
TRACE_MSG(H264_DEC_V0_875_112_2_18_1_27_22_34,"h264_dec.c,close,status error!")
TRACE_MSG(H264_DEC_V0_936_112_2_18_1_27_22_35,"h264_dec,p_buffer=0x%x, i_buffer=%d, depth=%d")
TRACE_MSG(H264_DEC_V0_946_112_2_18_1_27_22_36,"h264_dec, Send(), failed to call set event")
TRACE_MSG(H264_DEC_V0_952_112_2_18_1_27_22_37,"h264_dec, send(),fail")
TRACE_MSG(H264_DEC_V0_983_112_2_18_1_27_22_38,"h264_dec, Resume, failed to call set event")
TRACE_MSG(H264_DEC_V0_1028_112_2_18_1_27_23_39,"h264_dec, Pause(),status[0x%x]")
TRACE_MSG(H264_DEC_V0_1038_112_2_18_1_27_23_40,"h264_dec, Pause()")
TRACE_MSG(H264_DEC_V0_1061_112_2_18_1_27_23_41,"h264_dec, Resume(),status[0x%x]")
TRACE_MSG(H264_DEC_V0_1074_112_2_18_1_27_23_42,"__NotifyH264YUVBufferIsFree, PNULL BUFFER.")
TRACE_MSG(H264_DEC_V0_1111_112_2_18_1_27_23_43,"h264_dec.c, SetConfig, re sync fail,status[%d]")
TRACE_MSG(H264_DEC_V0_1125_112_2_18_1_27_23_44,"h264_dec CMD_DEC_ROTATION set %d")
TRACE_MSG(H264_DEC_V0_1137_112_2_18_1_27_23_45,"h264_dec CMD_DEC_ADD_PLUG set,cfg=0x%x")
TRACE_MSG(H264_DEC_V0_1168_112_2_18_1_27_23_46,"h264_dec, SetConfig, invalid cmd")
TRACE_MSG(H264_DEC_V0_1198_112_2_18_1_27_23_47,"h264_dec, GetInfo, invalid cmd")
TRACE_MSG(H264_DEC_V1_198_112_2_18_1_27_23_48,"h264_dec task, error happen when waiting for event")
TRACE_MSG(H264_DEC_V1_234_112_2_18_1_27_23_49,"H264DecoderTaskEntry,cur_time=%d,first_pos=%d")
TRACE_MSG(H264_DEC_V1_261_112_2_18_1_27_23_50,"h264_dec,video is too slow,seek one I VOP,syn_time[%d]")
TRACE_MSG(H264_DEC_V1_361_112_2_18_1_27_24_51,"h264_dec, cur_time=%d, bef_interval=%d, aft_interval=%d")
TRACE_MSG(H264_DEC_V1_402_112_2_18_1_27_24_52,"h264_dec, DecoderTaskEntry, failed to call set event")
TRACE_MSG(H264_DEC_V1_417_112_2_18_1_27_24_53,"H264DecoderTaskEntry,expect_I=%d,disp_skip=%d,i_frame=%d")
TRACE_MSG(H264_DEC_V1_426_112_2_18_1_27_24_54,"h264_dec, yuv buffer overflow!")
TRACE_MSG(H264_DEC_V1_432_112_2_18_1_27_24_55,"h264_dec, DecoderTaskEntry, failed to call set event")
TRACE_MSG(H264_DEC_V1_459_112_2_18_1_27_24_56,"h264_dec, DecoderTaskEntry, frame_width=%d, frame_height=%d, extra_buffer_size=%d,")
TRACE_MSG(H264_DEC_V1_463_112_2_18_1_27_24_57,"h264_dec, DecoderTaskEntry, H264DecMemSizeCal return error")
TRACE_MSG(H264_DEC_V1_474_112_2_18_1_27_24_58,"h264_dec, DecoderTaskEntry, frame_width=%d, frame_height=%d, Error or unsupport")
TRACE_MSG(H264_DEC_V1_483_112_2_18_1_27_24_59,"h264_dec, DecoderTaskEntry, p_decoder_buffer_extra == PNULL, return MMDEC_NOT_SUPPORTED")
TRACE_MSG(H264_DEC_V1_494_112_2_18_1_27_24_60,"h264_dec, DecoderTaskEntry, H264DecMemInit failed!")
TRACE_MSG(H264_DEC_V1_502_112_2_18_1_27_24_61,"h264_dec, DecoderTaskEntry, malloc OK, frame_width=%d, frame_height=%d, extra_buffer_size=%d,")
TRACE_MSG(H264_DEC_V1_549_112_2_18_1_27_24_62,"h264_dec, H264DecDecode time=%d,ret=%d, size = %d, pts = %d,exp_IVOP=%d,display_skip=%d")
TRACE_MSG(H264_DEC_V1_624_112_2_18_1_27_24_63,"h264_dec, block_new failed.")
TRACE_MSG(H264_DEC_V1_634_112_2_18_1_27_24_64,"h264_dec task, stream err,ret=%d")
TRACE_MSG(H264_DEC_V1_647_112_2_18_1_27_24_65,"h264_dec task, unsupport,ret=%d")
TRACE_MSG(H264_DEC_V1_659_112_2_18_1_27_24_66,"h264_dec task, dec err,ret=%d")
TRACE_MSG(H264_DEC_V1_679_112_2_18_1_27_24_67,"h264 decoder task, encount undefined event,i_event_flag[0x%x],status[0x%x]")
TRACE_MSG(H264_DEC_V1_692_112_2_18_1_27_24_68,"h264_dec, Active()")
TRACE_MSG(H264_DEC_V1_705_112_2_18_1_27_24_69,"h264_dec, Deactive()")
TRACE_MSG(H264_DEC_V1_717_112_2_18_1_27_24_70,"h264_dec, Open()")
TRACE_MSG(H264_DEC_V1_721_112_2_18_1_27_24_71,"h264_dec.c,close,status error!")
TRACE_MSG(H264_DEC_V1_739_112_2_18_1_27_24_72,"h264_dec.c, Open, movie size is too large.")
TRACE_MSG(H264_DEC_V1_755_112_2_18_1_27_24_73,"h264_dec.c,mutex_Init error!")
TRACE_MSG(H264_DEC_V1_760_112_2_18_1_27_24_74,"h264_dec.c,MpegInitYUVCtrl fail")
TRACE_MSG(H264_DEC_V1_768_112_2_18_1_27_24_75,"h264_dec, Open(), failed to create event")
TRACE_MSG(H264_DEC_V1_793_112_2_18_1_27_25_76,"h264_dec, Open(), failed to create task")
TRACE_MSG(H264_DEC_V1_808_112_2_18_1_27_25_77,"failed to call MPEG4_DecodeInit()")
TRACE_MSG(H264_DEC_V1_826_112_2_18_1_27_25_78,"h264_dec.c, open, err!")
TRACE_MSG(H264_DEC_V1_854_112_2_18_1_27_25_79,"h264_dec, close(),status[0x%x]")
TRACE_MSG(H264_DEC_V1_858_112_2_18_1_27_25_80,"h264_dec.c,close,status error!")
TRACE_MSG(H264_DEC_V1_918_112_2_18_1_27_25_81,"h264_dec, send()")
TRACE_MSG(H264_DEC_V1_926_112_2_18_1_27_25_82,"h264_dec, Send(), failed to call set event")
TRACE_MSG(H264_DEC_V1_932_112_2_18_1_27_25_83,"h264_dec, send(),fail")
TRACE_MSG(H264_DEC_V1_938_112_2_18_1_27_25_84,"h264_dec, send(), frame len is zero.")
TRACE_MSG(H264_DEC_V1_948_112_2_18_1_27_25_85,"h264_dec, Start(),status[0x%x]")
TRACE_MSG(H264_DEC_V1_956_112_2_18_1_27_25_86,"h264_dec, Resume, failed to call set event")
TRACE_MSG(H264_DEC_V1_968_112_2_18_1_27_25_87,"h264_dec, Stop(),status[0x%x]")
TRACE_MSG(H264_DEC_V1_992_112_2_18_1_27_25_88,"h264_dec, Pause(),status[0x%x]")
TRACE_MSG(H264_DEC_V1_999_112_2_18_1_27_25_89,"h264_dec, Pause()")
TRACE_MSG(H264_DEC_V1_1010_112_2_18_1_27_25_90,"h264_dec, Resume()")
TRACE_MSG(H264_DEC_V1_1018_112_2_18_1_27_25_91,"h264_dec, Resume, failed to call set event")
TRACE_MSG(H264_DEC_V1_1064_112_2_18_1_27_25_92,"h264_dec, SetConfig, re sync OK")
TRACE_MSG(H264_DEC_V1_1068_112_2_18_1_27_25_93,"h264_dec.c, SetConfig, re sync fail,status[%d]")
TRACE_MSG(H264_DEC_V1_1080_112_2_18_1_27_25_94,"h264_dec, SetConfig, playback_mode = MODE_SYNC")
TRACE_MSG(H264_DEC_V1_1086_112_2_18_1_27_25_95,"h264_dec, SetConfig, playback_mode = MODE_RATE")
TRACE_MSG(H264_DEC_V1_1091_112_2_18_1_27_25_96,"h264_dec, SetConfig, b_skip_display = SCI_TRUE")
TRACE_MSG(H264_DEC_V1_1097_112_2_18_1_27_25_97,"h264_dec CMD_DEC_ADD_PLUG set,cfg=0x%x, plug_mem_size=%d")
TRACE_MSG(H264_DEC_V1_1128_112_2_18_1_27_25_98,"h264_dec.c, SetConfig, invalid cmd")
TRACE_MSG(H264_DEC_V1_1152_112_2_18_1_27_25_99,"h264_dec.c, GetInfo, invalid cmd")
TRACE_MSG(H264_DEC_V1_1162_112_2_18_1_27_25_100,"h264 dec,GetQueueLen[%d]")
TRACE_MSG(H264_DEC_V1_1169_112_2_18_1_27_25_101,"MpegInitYUVCtrl, len 0x%x")
TRACE_MSG(H264_DEC_V1_1181_112_2_18_1_27_25_102,"MpegInitYUVCtrl,g_frameYUVCtr[%d],malloc fail!")
TRACE_MSG(H264_DEC_V1_1187_112_2_18_1_27_25_103,"MpegInitYUVCtrl, yuv ptr 0x%x, yuv len 0x%x")
TRACE_MSG(H264_DEC_V1_1197_112_2_18_1_27_25_104,"MpegReleaseYUVBuffer")
TRACE_MSG(H264_DEC_V1_1223_112_2_18_1_27_25_105,"MpegSetYUVDataFlag,pframe NULL!")
TRACE_MSG(H264_DEC_V1_1238_112_2_18_1_27_25_106,"MpegSetYUVDisplayParam,i_width[0x%x],i_height[0x%x]")
TRACE_MSG(H264_DEC_V1_1242_112_2_18_1_27_25_107,"MpegSetYUVDisplayParam,pframe NULL!")
TRACE_MSG(H264_DEC_V1_1263_112_2_18_1_27_25_108,"MpegGetFreeYUVBuffer,yuv buffer is full!")
TRACE_MSG(MJPG_DEC_V0_220_112_2_18_1_27_26_109,"MjpgDecoderTaskEntry,cur_time=%d,first_pos=%d")
TRACE_MSG(MJPG_DEC_V0_230_112_2_18_1_27_26_110,"MjpgDecoderTaskEntry,get block error")
TRACE_MSG(MJPG_DEC_V0_256_112_2_18_1_27_26_111,"MjpgDecoderTaskEntry,get block error")
TRACE_MSG(MJPG_DEC_V0_272_112_2_18_1_27_26_112,"MjpgDecoderTaskEntry,get block error")
TRACE_MSG(MJPG_DEC_V0_279_112_2_18_1_27_26_113,"MjpgDecoderTaskEntry,skip the frame")
TRACE_MSG(MJPG_DEC_V0_292_112_2_18_1_27_26_114,"mjpg_dec,AllocateVideoDecFrameBuf fail.")
TRACE_MSG(MJPG_DEC_V0_319_112_2_18_1_27_26_115,"mjpg_dec task, decode_result=%d, return widht err!")
TRACE_MSG(MJPG_DEC_V0_326_112_2_18_1_27_26_116,"mjpg_dec task, decode_result=%d, return err!")
TRACE_MSG(MJPG_DEC_V0_397_112_2_18_1_27_26_117,"mjpg_dec,block new failed.")
TRACE_MSG(MJPG_DEC_V0_410_112_2_18_1_27_26_118,"mjpg decoder task, encount undefined event,i_event_flag[0x%x]")
TRACE_MSG(MJPG_DEC_V0_449_112_2_18_1_27_27_119,"mpeg4_dec.c,close,status error!")
TRACE_MSG(MJPG_DEC_V0_455_112_2_18_1_27_27_120,"mpjpg_dec.c, open, movie size is too large.")
TRACE_MSG(MJPG_DEC_V0_467_112_2_18_1_27_27_121,"mjpg_dec, Open(), failed to create event")
TRACE_MSG(MJPG_DEC_V0_502_112_2_18_1_27_27_122,"mpeg4_dec.c,close,status error!")
TRACE_MSG(MJPG_DEC_V0_561_112_2_18_1_27_27_123,"mjpg_dec, Send(), failed to call set event")
TRACE_MSG(MJPG_DEC_V0_705_112_2_18_1_27_27_124,"mjpeg_dec.c, SetConfig, CMD_DEC_RE_SYN,p_event[%d]")
TRACE_MSG(MJPG_DEC_V0_711_112_2_18_1_27_27_125,"mjpeg_dec.c, SetConfig, re sync fail,p_event[%d]")
TRACE_MSG(MJPG_DEC_V0_719_112_2_18_1_27_27_126,"mjpeg_dec.c, SetConfig, invalid cmd")
TRACE_MSG(MJPG_DEC_V0_743_112_2_18_1_27_27_127,"mjpeg_dec.c, GetInfo, invalid cmd")
TRACE_MSG(MJPG_DEC_V0_782_112_2_18_1_27_27_128,"mjpg_dec.c,mutex_Init error!")
TRACE_MSG(MJPG_DEC_V0_822_112_2_18_1_27_27_129,"SetVideoDecFrameBufStatus,p_frame NULL!")
TRACE_MSG(MJPG_DEC_V0_897_112_2_18_1_27_27_130,"FreeOutBuf,p_buf=0x%x")
TRACE_MSG(MJPG_DEC_V0_900_112_2_18_1_27_27_131,"FreeOutBuf, PNULL BUFFER.")
TRACE_MSG(MJPG_DEC_V1_210_112_2_18_1_27_28_132,"MjpgDecoderTaskEntry,cur_time=%d,first_pos=%d")
TRACE_MSG(MJPG_DEC_V1_292_112_2_18_1_27_28_133,"mpjpg_dec, yuv buffer overflow!")
TRACE_MSG(MJPG_DEC_V1_328_112_2_18_1_27_28_134,"---JPEG_DecodeJpeg,ret=%d,datatype=%d,out_y=0x%x,w,h=%d,%d, pts=%d, frm_num=%d")
TRACE_MSG(MJPG_DEC_V1_381_112_2_18_1_27_28_135,"MjpgDecoderTaskEntry,block_New error")
TRACE_MSG(MJPG_DEC_V1_387_112_2_18_1_27_28_136,"MjpgDecoderTaskEntry,dec error=%d")
TRACE_MSG(MJPG_DEC_V1_396_112_2_18_1_27_28_137,"MjpgDecoderTaskEntry,block_FifoRemove error")
TRACE_MSG(MJPG_DEC_V1_415_112_2_18_1_27_29_138,"mjpg_dec, active")
TRACE_MSG(MJPG_DEC_V1_430_112_2_18_1_27_29_139,"---mjpg_dec, open,codec=0x%x,wh=%d,%d")
TRACE_MSG(MJPG_DEC_V1_434_112_2_18_1_27_29_140,"---mjpg_dec,open error, size is too large")
TRACE_MSG(MJPG_DEC_V1_444_112_2_18_1_27_29_141,"---mjpg_dec,mem init error")
TRACE_MSG(MJPG_DEC_V1_452_112_2_18_1_27_29_142,"mjpg_dec, Open(), failed to create event")
TRACE_MSG(MJPG_DEC_V1_474_112_2_18_1_27_29_143,"mjpg_dec, Open(), failed to create task")
TRACE_MSG(MJPG_DEC_V1_487_112_2_18_1_27_29_144,"mjpg, open, err!")
TRACE_MSG(MJPG_DEC_V1_545_112_2_18_1_27_29_145,"mjpg_dec, Send(), failed to call set event")
TRACE_MSG(MJPG_DEC_V1_641_112_2_18_1_27_29_146,"mjpeg_dec, SetConfig, re sync OK")
TRACE_MSG(MJPG_DEC_V1_645_112_2_18_1_27_29_147,"mjpeg_dec.c, SetConfig, re sync fail,p_event[%d]")
TRACE_MSG(MJPG_DEC_V1_657_112_2_18_1_27_29_148,"mjpeg_dec, SetConfig, playback_mode = MODE_RATE")
TRACE_MSG(MJPG_DEC_V1_663_112_2_18_1_27_29_149,"mjpeg_dec, SetConfig, playback_mode = MODE_RATE")
TRACE_MSG(MJPG_DEC_V1_669_112_2_18_1_27_29_150,"mjpeg_dec, SetConfig, b_skip_display = SCI_TRUE")
TRACE_MSG(MJPG_DEC_V1_673_112_2_18_1_27_29_151,"mjpeg_dec.c, SetConfig, invalid cmd")
TRACE_MSG(MJPG_DEC_V1_697_112_2_18_1_27_29_152,"mjpeg_dec.c, GetInfo, invalid cmd")
TRACE_MSG(MJPG_DEC_V1_730_112_2_18_1_27_29_153,"mjpg_dec.c,mutex_Init error!")
TRACE_MSG(MJPG_DEC_V1_745_112_2_18_1_27_29_154,"MjpgInitYUVCtrl,malloc fail!,%d")
TRACE_MSG(MJPG_DEC_V1_797_112_2_18_1_27_29_155,"MjpgSetYUVDataFlag,pframe NULL!")
TRACE_MSG(MJPG_DEC_V1_812_112_2_18_1_27_29_156,"MjpgSetYUVDisplayParam,i_width[0x%x],i_height[0x%x]")
TRACE_MSG(MJPG_DEC_V1_816_112_2_18_1_27_29_157,"MjpgSetYUVDisplayParam,pframe NULL!")
TRACE_MSG(MJPG_DEC_V1_869_112_2_18_1_27_29_158,"FreeVideoDecFrameBuf,group[%d].addr=0x%x")
TRACE_MSG(MJPG_DEC_V1_890_112_2_18_1_27_29_159,"__FreeMjpgOutBuf, PNULL BUFFER.")
TRACE_MSG(MPEG4_DEC_V0_257_112_2_18_1_27_30_160,"mpeg4_dec task, error happen when waiting for event,%d")
TRACE_MSG(MPEG4_DEC_V0_311_112_2_18_1_27_30_161,"Mpeg4DecoderTaskEntry,cur_time=%d, first_pos=%d, vide_fifo_start_time=%d")
TRACE_MSG(MPEG4_DEC_V0_340_112_2_18_1_27_31_162,"mpeg4_dec,video is too slow,seek one I VOP,syn_time[%d]")
TRACE_MSG(MPEG4_DEC_V0_344_112_2_18_1_27_31_163,"mpeg4_dec,video is too slow, but no I VOP in next 1000ms. syn_time[%d]")
TRACE_MSG(MPEG4_DEC_V0_397_112_2_18_1_27_31_164,"mpeg4_dec, DecoderTaskEntry, failed to call set event")
TRACE_MSG(MPEG4_DEC_V0_412_112_2_18_1_27_31_165,"mpeg4_dec, disp_skip=%d, time_offset_a_v %d, b_expected_IVOP %d")
TRACE_MSG(MPEG4_DEC_V0_416_112_2_18_1_27_31_166,"mpeg4_dec, _Peek_NextDispFrameContain false,wait for next time")
TRACE_MSG(MPEG4_DEC_V0_449_112_2_18_1_27_31_167,"Mpeg4DecoderTaskEntry, frame_width=%d, frame_height=%d, extra_buffer_size=%d,")
TRACE_MSG(MPEG4_DEC_V0_453_112_2_18_1_27_31_168,"Mpeg4DecoderTaskEntry, MP4DecMemSizeCal return error")
TRACE_MSG(MPEG4_DEC_V0_465_112_2_18_1_27_31_169,"Mpeg4DecoderTaskEntry, frame_width=%d, frame_height=%d, Error or unsupport")
TRACE_MSG(MPEG4_DEC_V0_475_112_2_18_1_27_31_170,"Mpeg4DecoderTaskEntry, p_decoder_buffer_extra == PNULL, return MMDEC_NOT_SUPPORTED")
TRACE_MSG(MPEG4_DEC_V0_488_112_2_18_1_27_31_171,"Mpeg4DecoderTaskEntry, MPEG4DecMemInit failed")
TRACE_MSG(MPEG4_DEC_V0_497_112_2_18_1_27_31_172,"Mpeg4DecoderTaskEntry, malloc OK, frame_width=%d, frame_height=%d, extra_buffer_size=%d,")
TRACE_MSG(MPEG4_DEC_V0_580_112_2_18_1_27_31_173,"mpeg4_dec task, decode_result, MMDEC_OUTPUT_BUFFER_OVERFLOW")
TRACE_MSG(MPEG4_DEC_V0_607_112_2_18_1_27_31_174,"mpeg4_dec, _Get_NextDispFrameContain PNULL, no frame contain")
TRACE_MSG(MPEG4_DEC_V0_656_112_2_18_1_27_31_175,"mpeg4_dec, block_new failed.")
TRACE_MSG(MPEG4_DEC_V0_669_112_2_18_1_27_31_176,"mpeg4_dec task, decode_result, MMDEC_ERROR")
TRACE_MSG(MPEG4_DEC_V0_677_112_2_18_1_27_31_177,"mpeg4_dec task, decode_result, MMDEC_PARAM_ERROR")
TRACE_MSG(MPEG4_DEC_V0_686_112_2_18_1_27_31_178,"mpeg4_dec task, decode_result, MMDEC_MEMORY_ERROR")
TRACE_MSG(MPEG4_DEC_V0_690_112_2_18_1_27_31_179,"mpeg4_dec task, decode_result, MMDEC_INVALID_STATUS")
TRACE_MSG(MPEG4_DEC_V0_694_112_2_18_1_27_31_180,"mpeg4_dec task, decode_result, MMDEC_STREAM_ERROR")
TRACE_MSG(MPEG4_DEC_V0_702_112_2_18_1_27_31_181,"mpeg4_dec task, decode_result, MMDEC_OUTPUT_BUFFER_OVERFLOW")
TRACE_MSG(MPEG4_DEC_V0_706_112_2_18_1_27_31_182,"mpeg4_dec task, decode_result, MMDEC_HW_ERROR")
TRACE_MSG(MPEG4_DEC_V0_713_112_2_18_1_27_31_183,"mpeg4_dec task, decode_result, MMDEC_NOT_SUPPORTED")
TRACE_MSG(MPEG4_DEC_V0_718_112_2_18_1_27_31_184,"mpeg4_dec task, decode_result, MMDEC_FRAME_SEEK_IVOP")
TRACE_MSG(MPEG4_DEC_V0_722_112_2_18_1_27_31_185,"mpeg4_dec task, decode_result=%d, return err!")
TRACE_MSG(MPEG4_DEC_V0_741_112_2_18_1_27_31_186,"mpeg4 decoder task, encount undefined event,i_event_flag[0x%x],status[0x%x]")
TRACE_MSG(MPEG4_DEC_V0_779_112_2_18_1_27_31_187,"mpeg4_dec.c,close,status error!")
TRACE_MSG(MPEG4_DEC_V0_785_112_2_18_1_27_31_188,"mpeg4_dec.c, open, movie size is too large.")
TRACE_MSG(MPEG4_DEC_V0_845_112_2_18_1_27_32_189,"failed to call REFMP4DecInit()")
TRACE_MSG(MPEG4_DEC_V0_852_112_2_18_1_27_32_190,"mpeg4_dec, Open(), failed to create event")
TRACE_MSG(MPEG4_DEC_V0_876_112_2_18_1_27_32_191,"mpeg4_dec, Open(), failed to create task")
TRACE_MSG(MPEG4_DEC_V0_914_112_2_18_1_27_32_192,"mpeg4_dec.c,close,status error!")
TRACE_MSG(MPEG4_DEC_V0_981_112_2_18_1_27_32_193,"mpeg4_dec, Send(), failed to call set event")
TRACE_MSG(MPEG4_DEC_V0_987_112_2_18_1_27_32_194,"mpeg4_dec, send(),fail")
TRACE_MSG(MPEG4_DEC_V0_1118_112_2_18_1_27_32_195,"__NotifyYUVBufferIsFree, PNULL BUFFER.")
TRACE_MSG(MPEG4_DEC_V0_1161_112_2_18_1_27_32_196,"mp4_dec.c, SetConfig, re sync fail,status[%d]")
TRACE_MSG(MPEG4_DEC_V0_1182_112_2_18_1_27_32_197,"mpeg4_dec, SetConfig, invalid cmd")
TRACE_MSG(MPEG4_DEC_V0_1214_112_2_18_1_27_32_198,"mpeg4_dec, GetInfo, invalid cmd")
TRACE_MSG(MPEG4_DEC_V1_199_112_2_18_1_27_33_199,"mpeg4_dec task, error happen when waiting for event")
TRACE_MSG(MPEG4_DEC_V1_256_112_2_18_1_27_33_200,"Mpeg4DecoderTaskEntry,cur_time=%d,first_pos=%d, start_time=%d, offset=%d")
TRACE_MSG(MPEG4_DEC_V1_277_112_2_18_1_27_33_201,"mpeg4_dec,video is too slow,seek one I VOP,syn_time[%d]")
TRACE_MSG(MPEG4_DEC_V1_294_112_2_18_1_27_33_202,"mpeg4_dec, frame_rate=%d, div4,8,16=[%d,%d,%d]")
TRACE_MSG(MPEG4_DEC_V1_377_112_2_18_1_27_33_203,"mpeg4_dec, cur_time=%d, bef_interval=%d, aft_interval=%d")
TRACE_MSG(MPEG4_DEC_V1_443_112_2_18_1_27_34_204,"mpeg4_dec, yuv buffer overflow!")
TRACE_MSG(MPEG4_DEC_V1_480_112_2_18_1_27_34_205,"Mpeg4DecoderTaskEntry, interval=%d, frame_width=%d, frame_height=%d, extra_buffer_size=%d,")
TRACE_MSG(MPEG4_DEC_V1_484_112_2_18_1_27_34_206,"Mpeg4DecoderTaskEntry, MP4DecMemSizeCal return error")
TRACE_MSG(MPEG4_DEC_V1_496_112_2_18_1_27_34_207,"Mpeg4DecoderTaskEntry, frame_width=%d, frame_height=%d, Error or unsupport")
TRACE_MSG(MPEG4_DEC_V1_506_112_2_18_1_27_34_208,"Mpeg4DecoderTaskEntry, p_decoder_buffer_extra == PNULL, return MMDEC_NOT_SUPPORTED")
TRACE_MSG(MPEG4_DEC_V1_518_112_2_18_1_27_34_209,"Mpeg4DecoderTaskEntry, MPEG4DecMemInit failed")
TRACE_MSG(MPEG4_DEC_V1_527_112_2_18_1_27_34_210,"Mpeg4DecoderTaskEntry, malloc OK, frame_width=%d, frame_height=%d, extra_buffer_size=%d,")
TRACE_MSG(MPEG4_DEC_V1_555_112_2_18_1_27_34_211,"mpeg4_dec, MPEG4_Decode time=%3d,ret=%d,exp_I=%d,pts=%d,frm_num=%d,display_skip=%d,eff=%d")
TRACE_MSG(MPEG4_DEC_V1_564_112_2_18_1_27_34_212,"mpeg4_dec task, decode_result, MMDEC_OUTPUT_BUFFER_OVERFLOW")
TRACE_MSG(MPEG4_DEC_V1_638_112_2_18_1_27_34_213,"mpeg4_dec, block_new failed.")
TRACE_MSG(MPEG4_DEC_V1_649_112_2_18_1_27_34_214,"mpeg4_dec task, stream err,ret=%d")
TRACE_MSG(MPEG4_DEC_V1_656_112_2_18_1_27_34_215,"mpeg4_dec task, unsupport,ret=%d")
TRACE_MSG(MPEG4_DEC_V1_661_112_2_18_1_27_34_216,"mpeg4_dec task, dec err,ret=%d")
TRACE_MSG(MPEG4_DEC_V1_675_112_2_18_1_27_34_217,"mpeg4 decoder task, encount undefined event,i_event_flag[0x%x],status[0x%x]")
TRACE_MSG(MPEG4_DEC_V1_717_112_2_18_1_27_34_218,"mpeg4_dec.c,close,status error!")
TRACE_MSG(MPEG4_DEC_V1_772_112_2_18_1_27_34_219,"mpeg4_dec.c,mutex_Init error!")
TRACE_MSG(MPEG4_DEC_V1_777_112_2_18_1_27_34_220,"mpeg4_dec.c,MpegInitYUVCtrl fail")
TRACE_MSG(MPEG4_DEC_V1_785_112_2_18_1_27_34_221,"mpeg4_dec, Open(), failed to create event")
TRACE_MSG(MPEG4_DEC_V1_810_112_2_18_1_27_34_222,"mpeg4_dec, Open(), failed to create task")
TRACE_MSG(MPEG4_DEC_V1_835_112_2_18_1_27_34_223,"mpeg4_dec, Open(), failed to create task")
TRACE_MSG(MPEG4_DEC_V1_849_112_2_18_1_27_34_224,"failed to call MP4DecInit()")
TRACE_MSG(MPEG4_DEC_V1_867_112_2_18_1_27_34_225,"mpeg4_dec.c, open, err!")
TRACE_MSG(MPEG4_DEC_V1_895_112_2_18_1_27_34_226,"mpeg4_dec, close(),status[0x%x]")
TRACE_MSG(MPEG4_DEC_V1_899_112_2_18_1_27_34_227,"mpeg4_dec.c,close,status error!")
TRACE_MSG(MPEG4_DEC_V1_955_112_2_18_1_27_35_228,"mpeg4_dec, send(), i_depth=%d")
TRACE_MSG(MPEG4_DEC_V1_963_112_2_18_1_27_35_229,"mpeg4_dec, Send(), failed to call set event")
TRACE_MSG(MPEG4_DEC_V1_969_112_2_18_1_27_35_230,"mpeg4_dec, send(),fail")
TRACE_MSG(MPEG4_DEC_V1_1006_112_2_18_1_27_35_231,"mpeg4_dec, Stop(),status[0x%x]")
TRACE_MSG(MPEG4_DEC_V1_1040_112_2_18_1_27_35_232,"mpeg4_dec, Pause(),status[0x%x]")
TRACE_MSG(MPEG4_DEC_V1_1059_112_2_18_1_27_35_233,"mpeg4_dec, Resume(),status[0x%x]")
TRACE_MSG(MPEG4_DEC_V1_1084_112_2_18_1_27_35_234,"__NotifyYUVBufferIsFree, pframe=PNULL")
TRACE_MSG(MPEG4_DEC_V1_1087_112_2_18_1_27_35_235,"mpeg4_dec, __NotifyYUVBufferIsFree, pframe=0x%x")
TRACE_MSG(MPEG4_DEC_V1_1118_112_2_18_1_27_35_236,"mpeg4_dec, SetConfig, re sync OK")
TRACE_MSG(MPEG4_DEC_V1_1122_112_2_18_1_27_35_237,"mpeg4_dec.c, SetConfig, re sync fail,status[%d]")
TRACE_MSG(MPEG4_DEC_V1_1137_112_2_18_1_27_35_238,"mpeg4_dec, SetConfig, playback_mode = MODE_SYNC")
TRACE_MSG(MPEG4_DEC_V1_1143_112_2_18_1_27_35_239,"mpeg4_dec, SetConfig, playback_mode = MODE_RATE")
TRACE_MSG(MPEG4_DEC_V1_1148_112_2_18_1_27_35_240,"mpeg4_dec, SetConfig, b_skip_display = SCI_TRUE")
TRACE_MSG(MPEG4_DEC_V1_1157_112_2_18_1_27_35_241,"mpeg4_dec CMD_DEC_ADD_PLUG set,cfg=0x%x")
TRACE_MSG(MPEG4_DEC_V1_1166_112_2_18_1_27_35_242,"mpeg4_dec, CMD_DEC_ADD_PLUG error")
TRACE_MSG(MPEG4_DEC_V1_1174_112_2_18_1_27_35_243,"mpeg4_dec.c, SetConfig, invalid cmd")
TRACE_MSG(MPEG4_DEC_V1_1198_112_2_18_1_27_35_244,"mpeg4_dec.c, GetInfo, invalid cmd")
TRACE_MSG(MPEG4_DEC_V1_1230_112_2_18_1_27_35_245,"MpegInitYUVCtrl,g_frameYUVCtr[%d],malloc fail!")
TRACE_MSG(MPEG4_DEC_V1_1272_112_2_18_1_27_35_246,"MpegSetYUVDataFlag,pframe NULL!")
TRACE_MSG(MPEG4_DEC_V1_1275_112_2_18_1_27_35_247,"mpeg4_dec, MpegSetYUVDataFlag, pframe=0x%x, data_status=%d")
TRACE_MSG(MPEG4_DEC_V1_1292_112_2_18_1_27_35_248,"MpegSetYUVDisplayParam,pframe NULL!")
TRACE_MSG(MPEG4_DEC_V1_1309_112_2_18_1_27_35_249,"mpeg4_dec, MpegGetFreeYUVBuffer, yuv_num=%d, pframe=0x%x")
TRACE_MSG(MJPG_ENC_V0_149_112_2_18_1_27_36_250,"Video,Captured,discard,i_state=%d")
TRACE_MSG(MJPG_ENC_V0_210_112_2_18_1_27_36_251,"Video,Captured,adjust the frame cycle,i_deta_time=0x%x,frame_dts_dither=0x%x")
TRACE_MSG(MJPG_ENC_V0_261_112_2_18_1_27_36_252,"Mjpeg,Captured,block new failed,fifo_depth=%d")
TRACE_MSG(MJPG_ENC_V1_141_112_2_18_1_27_37_253,"InitEsBuffer, memory leaking")
TRACE_MSG(MJPG_ENC_V1_254_112_2_18_1_27_37_254,"mjpg_enc task, error happen when waiting for event")
TRACE_MSG(MJPG_ENC_V1_304_112_2_18_1_27_37_255,"mjpg_enc,in_fifo_depth=%d,out_fifo_depth=%d")
TRACE_MSG(MJPG_ENC_V1_318_112_2_18_1_27_37_256,"mjpg_enc has stopped!")
TRACE_MSG(MJPG_ENC_V1_326_112_2_18_1_27_37_257,"mjpg_enc,failed to alloc blocks")
TRACE_MSG(MJPG_ENC_V1_331_112_2_18_1_27_37_258,"mjpg_enc has stopped!")
TRACE_MSG(MJPG_ENC_V1_338_112_2_18_1_27_37_259,"mjpg_enc,failed to call set event")
TRACE_MSG(MJPG_ENC_V1_343_112_2_18_1_27_37_260,"mjpg_enc,AllocEsBuffer,block=0x%x,addr=0x%x")
TRACE_MSG(MJPG_ENC_V1_392_112_2_18_1_27_37_261,"mjpg_enc,ret_enc=%d,out_size=%d,Q=%d")
TRACE_MSG(MJPG_ENC_V1_401_112_2_18_1_27_37_262,"mjpg_enc, failed to add to recorder fifo")
TRACE_MSG(MJPG_ENC_V1_407_112_2_18_1_27_37_263,"mjpg_enc,enc failed.")
TRACE_MSG(MJPG_ENC_V1_414_112_2_18_1_27_37_264,"mjpg_enc, encount undefined event")
TRACE_MSG(MJPG_ENC_V1_449_112_2_18_1_27_37_265,"mjpg_enc,Open failed, size is too large")
TRACE_MSG(MJPG_ENC_V1_457_112_2_18_1_27_37_266,"mjpg_enc,open,failed to initial es buf")
TRACE_MSG(MJPG_ENC_V1_477_112_2_18_1_27_37_267,"mpeg4_enc, Open(), failed to create event")
TRACE_MSG(MJPG_ENC_V1_496_112_2_18_1_27_37_268,"%s(Line%d), SCI_INVALID_BLOCK_ID == s_enc_ctx.p_task")
TRACE_MSG(MJPG_ENC_V1_504_112_2_18_1_27_37_269,"mjpg_enc,failed to open")
TRACE_MSG(MJPG_ENC_V1_563_112_2_18_1_27_37_270,"Video,Captured,discard,i_state=%d")
TRACE_MSG(MJPG_ENC_V1_624_112_2_18_1_27_38_271,"Video,Captured,adjust the frame cycle,i_deta_time=0x%x,frame_dts_dither=0x%x")
TRACE_MSG(MJPG_ENC_V1_640_112_2_18_1_27_38_272,"Mjpeg,Captured,block new failed,fifo_depth=%d")
TRACE_MSG(MJPG_ENC_V1_663_112_2_18_1_27_38_273,"mjpg_enc, Capture, block_FifoAdd, i_time=%d, first_frame_time=%d, pts=%d")
TRACE_MSG(MJPG_ENC_V1_667_112_2_18_1_27_38_274,"mjpg_enc,capture,failed to call set event")
TRACE_MSG(MPEG4_ENC_V0_149_112_2_18_1_27_38_275,"MPEG4 ENC, memory leaking")
TRACE_MSG(MPEG4_ENC_V0_266_112_2_18_1_27_38_276,"mpeg4_enc task, error happen when waiting for event")
TRACE_MSG(MPEG4_ENC_V0_299_112_2_18_1_27_38_277,"mpeg4_enc,in_fifo_depth=%d,out_fifo_depth=%d")
TRACE_MSG(MPEG4_ENC_V0_303_112_2_18_1_27_38_278,"mpeg4_enc has stopped,state=%d!")
TRACE_MSG(MPEG4_ENC_V0_323_112_2_18_1_27_38_279,"mpeg4_enc,failed to alloc blocks")
TRACE_MSG(MPEG4_ENC_V0_345_112_2_18_1_27_38_280,"MP4EncStrmEncode return %d, size = %d")
TRACE_MSG(MPEG4_ENC_V0_373_112_2_18_1_27_39_281,"mpeg4_enc, failed to add to recorder fifo")
TRACE_MSG(MPEG4_ENC_V0_379_112_2_18_1_27_39_282,"mpeg4_enc,enc failed.")
TRACE_MSG(MPEG4_ENC_V0_386_112_2_18_1_27_39_283,"mpeg4_enc, encount undefined event")
TRACE_MSG(MPEG4_ENC_V0_421_112_2_18_1_27_39_284,"mpeg4_enc_sc6800h.c Open()")
TRACE_MSG(MPEG4_ENC_V0_429_112_2_18_1_27_39_285,"mpeg4_enc,Open failed, size is too large")
TRACE_MSG(MPEG4_ENC_V0_459_112_2_18_1_27_39_286,"mpeg4_enc, Open(), fail to alloc buffer for encoder")
TRACE_MSG(MPEG4_ENC_V0_470_112_2_18_1_27_39_287,"mpeg4_enc, Open(), fail to init the encoder")
TRACE_MSG(MPEG4_ENC_V0_515_112_2_18_1_27_39_288,"mpeg4_enc, Open(), failed to create event")
TRACE_MSG(MPEG4_ENC_V0_540_112_2_18_1_27_39_289,"mpeg4_enc, Open(), failed to create task")
TRACE_MSG(MPEG4_ENC_V0_551_112_2_18_1_27_39_290,"mpeg4_enc,failed to open")
TRACE_MSG(MPEG4_ENC_V0_624_112_2_18_1_27_39_291,"Video,Captured,discard,i_state=%d")
TRACE_MSG(MPEG4_ENC_V0_683_112_2_18_1_27_39_292,"Video,Captured,adjust the frame cycle,i_deta_time=0x%x,frame_dts_dither=0x%x")
TRACE_MSG(MPEG4_ENC_V0_699_112_2_18_1_27_39_293,"MPEG4,Captured,block new failed,fifo_depth=%d")
TRACE_MSG(MPEG4_ENC_V0_718_112_2_18_1_27_39_294,"mpeg4_enc,capture,failed to call set event")
TRACE_MSG(MPEG4_ENC_V1_156_112_2_18_1_27_40_295,"InitEsBuffer, memory leaking")
TRACE_MSG(MPEG4_ENC_V1_270_112_2_18_1_27_40_296,"mpeg4_enc task, error happen when waiting for event")
TRACE_MSG(MPEG4_ENC_V1_303_112_2_18_1_27_40_297,"mpeg4_enc,in_fifo_depth=%d,out_fifo_depth=%d")
TRACE_MSG(MPEG4_ENC_V1_307_112_2_18_1_27_40_298,"mpeg4_enc has stopped,state=%d!")
TRACE_MSG(MPEG4_ENC_V1_327_112_2_18_1_27_40_299,"mpeg4_enc,failed to alloc blocks")
TRACE_MSG(MPEG4_ENC_V1_370_112_2_18_1_27_40_300,"mpeg4_enc,ret_enc=%d,out_size=%d")
TRACE_MSG(MPEG4_ENC_V1_398_112_2_18_1_27_40_301,"mpeg4_enc, failed to add to recorder fifo")
TRACE_MSG(MPEG4_ENC_V1_404_112_2_18_1_27_40_302,"mpeg4_enc,enc failed.")
TRACE_MSG(MPEG4_ENC_V1_411_112_2_18_1_27_40_303,"mpeg4_enc, encount undefined event")
TRACE_MSG(MPEG4_ENC_V1_455_112_2_18_1_27_40_304,"mpeg4_enc,Open failed, size is too large")
TRACE_MSG(MPEG4_ENC_V1_477_112_2_18_1_27_40_305,"mpeg4_enc, Open(), fail to alloc buffer for encoder")
TRACE_MSG(MPEG4_ENC_V1_489_112_2_18_1_27_40_306,"mpeg4_enc, Open(), fail to init the encoder")
TRACE_MSG(MPEG4_ENC_V1_517_112_2_18_1_27_40_307,"--para error,video_quality_level=%d")
TRACE_MSG(MPEG4_ENC_V1_528_112_2_18_1_27_40_308,"mpeg4_enc, open, CAL_VideoEncSetConfig return error")
TRACE_MSG(MPEG4_ENC_V1_537_112_2_18_1_27_40_309,"mpeg4_enc,open,failed to initial es buf")
TRACE_MSG(MPEG4_ENC_V1_549_112_2_18_1_27_40_310,"mpeg4_enc, open, CAL_VideoEncGenHeader return error")
TRACE_MSG(MPEG4_ENC_V1_554_112_2_18_1_27_40_311,"mpeg4_enc,open, invalid header bitstream_size")
TRACE_MSG(MPEG4_ENC_V1_561_112_2_18_1_27_40_312,"mpeg4_enc,open, es_format memory alloc fail!")
TRACE_MSG(MPEG4_ENC_V1_584_112_2_18_1_27_41_313,"mpeg4_enc, Open(), failed to create event")
TRACE_MSG(MPEG4_ENC_V1_608_112_2_18_1_27_41_314,"mpeg4_enc, Open(), failed to create task")
TRACE_MSG(MPEG4_ENC_V1_619_112_2_18_1_27_41_315,"mpeg4_enc,failed to open")
TRACE_MSG(MPEG4_ENC_V1_695_112_2_18_1_27_41_316,"Video,Captured,discard,i_state=%d")
TRACE_MSG(MPEG4_ENC_V1_754_112_2_18_1_27_41_317,"Video,Captured,adjust the frame cycle,i_deta_time=0x%x,frame_dts_dither=0x%x")
TRACE_MSG(MPEG4_ENC_V1_770_112_2_18_1_27_41_318,"MPEG4,Captured,block new failed,fifo_depth=%d")
TRACE_MSG(MPEG4_ENC_V1_789_112_2_18_1_27_41_319,"mpeg4_enc,capture,failed to call set event")
TRACE_MSG(PLAYER_DISPLAY_V0_108_112_2_18_1_27_41_320,"Video_Display_Open,is_open=%d")
TRACE_MSG(PLAYER_DISPLAY_V0_118_112_2_18_1_27_41_321,"Video_Display_Open,DC_Open fail")
TRACE_MSG(PLAYER_DISPLAY_V0_138_112_2_18_1_27_41_322,"Video_Display_Close,is_open=%d")
TRACE_MSG(PLAYER_DISPLAY_V0_177_112_2_18_1_27_41_323,"Video_Display_Setparam,rect=[%d,%d,%d,%d]")
TRACE_MSG(PLAYER_DISPLAY_V0_181_112_2_18_1_27_41_324,"Video_Display_Setparam,para error!")
TRACE_MSG(PLAYER_DISPLAY_V0_207_112_2_18_1_27_42_325,"Video_Display_Oneframe, isn't open yet,do nothing!")
TRACE_MSG(PLAYER_DISPLAY_V0_213_112_2_18_1_27_42_326,"Video_Display_Oneframe, p_frame == PNULL !")
TRACE_MSG(PLAYER_DISPLAY_V0_218_112_2_18_1_27_42_327,"Video_Display_Oneframe,w(%d) or h(%d) is zero!")
TRACE_MSG(PLAYER_DISPLAY_V0_225_112_2_18_1_27_42_328,"Video_Display_Oneframe,update_display_param error")
TRACE_MSG(PLAYER_DISPLAY_V0_274_112_2_18_1_27_42_329,"Video_Display_Oneframe,not support yuv%d")
TRACE_MSG(PLAYER_DISPLAY_V0_280_112_2_18_1_27_42_330,"Video_Display_Oneframe, DC_DisplayAFrame fail")
TRACE_MSG(PLAYER_DISPLAY_V0_290_112_2_18_1_27_42_331,"Video_Display_Oneframe, wait time out")
TRACE_MSG(PLAYER_DISPLAY_V0_320_112_2_18_1_27_42_332,"Video_PostProcess_Oneframe,isn't open(%d) or busy(%d)")
TRACE_MSG(PLAYER_DISPLAY_V0_325_112_2_18_1_27_42_333,"Video_PostProcess_Oneframe, p_frame == PNULL !")
TRACE_MSG(PLAYER_DISPLAY_V0_343_112_2_18_1_27_42_334,"Video_PostProcess_Oneframe,in,data_wh=%d,%d,trim_wh=%d,%d")
TRACE_MSG(PLAYER_DISPLAY_V0_349_112_2_18_1_27_42_335,"Video_PostProcess_Oneframe,w,h error")
TRACE_MSG(PLAYER_DISPLAY_V0_358_112_2_18_1_27_42_336,"Video_PostProcess_Oneframe,update_display_param error")
TRACE_MSG(PLAYER_DISPLAY_V0_408_112_2_18_1_27_42_337,"Video_PostProcess_Oneframe,not support yuv%d")
TRACE_MSG(PLAYER_DISPLAY_V0_417_112_2_18_1_27_42_338,"Video_PostProcess_Oneframe, DC_DisplayAFrame fail")
TRACE_MSG(PLAYER_DISPLAY_V0_434_112_2_18_1_27_42_339,"Video_PostProcess_Oneframe, wait time out")
TRACE_MSG(PLAYER_DISPLAY_V0_465_112_2_18_1_27_42_340,"display_WaitStatus, wait time out[%d]")
TRACE_MSG(PLAYER_DISPLAY_V0_485_112_2_18_1_27_42_341,"Video_Display_Getparam,para error!")
TRACE_MSG(PLAYER_DISPLAY_V0_514_112_2_18_1_27_42_342,"update_display_param,org(w,h)=(%d,%d),disp_rect(x,y,w,h)=(%d,%d,%d,%d),angle=%d,distortion=%d")
TRACE_MSG(PLAYER_DISPLAY_V0_576_112_2_18_1_27_42_343,"update_display_param,DC_VideoSetDisplayParam fail")
TRACE_MSG(PLAYER_DISPLAY_V0_597_112_2_18_1_27_42_344,"video_pp_callback_frm_dc,callback,sx,ex=0x(%x,%x),sy,ey=0x(%x,%x)%d")
TRACE_MSG(PLAYER_DISPLAY_V1_134_112_2_18_1_27_43_345,"mplayer_display_Open,duplicate")
TRACE_MSG(PLAYER_DISPLAY_V1_167_112_2_18_1_27_43_346,"Video_Display_Close,duplicate.")
TRACE_MSG(PLAYER_DISPLAY_V1_208_112_2_18_1_27_43_347,"Video_Display_Setparam,para error!")
TRACE_MSG(PLAYER_DISPLAY_V1_235_112_2_18_1_27_43_348,"Video_Display_Oneframe,not open yet")
TRACE_MSG(PLAYER_DISPLAY_V1_240_112_2_18_1_27_43_349,"Video_Display_Oneframe,p_frame == PNULL")
TRACE_MSG(PLAYER_DISPLAY_V1_245_112_2_18_1_27_43_350,"Video_Display_Oneframe,w(%d) or h(%d) is zero!")
TRACE_MSG(PLAYER_DISPLAY_V1_251_112_2_18_1_27_43_351,"mplayer_display, wait time out")
TRACE_MSG(PLAYER_DISPLAY_V1_264_112_2_18_1_27_43_352,"Video_Display_Oneframe, update_display_param fail")
TRACE_MSG(PLAYER_DISPLAY_V1_287_112_2_18_1_27_43_353,"mplayer_display, wait time out")
TRACE_MSG(PLAYER_DISPLAY_V1_315_112_2_18_1_27_43_354,"Video_PostProcess_Oneframe,not open yet")
TRACE_MSG(PLAYER_DISPLAY_V1_339_112_2_18_1_27_43_355,"mplayer_display, wait time out")
TRACE_MSG(PLAYER_DISPLAY_V1_354_112_2_18_1_27_43_356,"Video_Display_Oneframe, update_display_param fail")
TRACE_MSG(PLAYER_DISPLAY_V1_380_112_2_18_1_27_43_357,"mplayer_display, wait time out")
TRACE_MSG(PLAYER_DISPLAY_V1_437_112_2_18_1_27_43_358,"video display, isp_callback_display")
TRACE_MSG(PLAYER_DISPLAY_V1_482_112_2_18_1_27_43_359,"display_WaitStatus, wait time out[%d]")
TRACE_MSG(PLAYER_DISPLAY_V1_500_112_2_18_1_27_43_360,"Video_Display_Getparam,para error!")
TRACE_MSG(PLAYER_DISPLAY_V1_524_112_2_18_1_27_44_361,"update_display_param,w,h =[%d,%d]")
TRACE_MSG(PLAYER_DISPLAY_V1_581_112_2_18_1_27_44_362,"----display_param,input_wh=%d,%d,range_wh=%d,%d,addr_yuv=0x%x")
TRACE_MSG(PLAYER_DISPLAY_V1_608_112_2_18_1_27_44_363,"----display_param,Angle=%d;lcdID:%d-%d;xy:%d,%d;wh:%d,%d")
TRACE_MSG(PLAYER_DISPLAY_V1_614_112_2_18_1_27_44_364,"update_display_param,ISP_ServiceSetReviewParam fail")
TRACE_MSG(ACCESS_MEM_112_112_2_18_1_27_45_365,"access_mem, Open(), failed to alloc stream CB")
TRACE_MSG(ACCESS_MEM_138_112_2_18_1_27_45_366,"access_meme, Close(), invalidate parameter")
TRACE_MSG(ACCESS_STREAM_290_112_2_18_1_27_47_367,"stream info: %d ms")
TRACE_MSG(ACCESS_STREAM_304_112_2_18_1_27_47_368,"    |-- Audio Config: codec = %4.4s, duration = %d ms, channel = %d, samplerate = %d Hz")
TRACE_MSG(ACCESS_STREAM_547_112_2_18_1_27_47_369,"access_stream, Open() failed")
TRACE_MSG(ACCESS_STREAM_673_112_2_18_1_27_47_370,"access_stream,p_player_ctx is NULL.")
TRACE_MSG(BLOCK_200_112_2_18_1_27_49_371,"un expected frame buffer type")
TRACE_MSG(BLOCK_218_112_2_18_1_27_49_372,"block_Init, len %d")
TRACE_MSG(BLOCK_228_112_2_18_1_27_49_373,"block_Init,duplicate1")
TRACE_MSG(BLOCK_239_112_2_18_1_27_49_374,"block_Init, p_blk_append_buf_base malloc failed.")
TRACE_MSG(BLOCK_253_112_2_18_1_27_49_375,"block_Init, p_sft_blk_fifo malloc failed.")
TRACE_MSG(BLOCK_310_112_2_18_1_27_49_376,"block_Init,duplicate2")
TRACE_MSG(BLOCK_333_112_2_18_1_27_49_377,"block_Release")
TRACE_MSG(BLOCK_342_112_2_18_1_27_49_378,"block_Release,p_sft_blk_fifo is empty or release != create")
TRACE_MSG(BLOCK_396_112_2_18_1_27_49_379,"block_Alloc,fifo doesn't create yet")
TRACE_MSG(BLOCK_406_112_2_18_1_27_49_380,"block_Alloc,i_size(%d)>fifo_width_size(%d)")
TRACE_MSG(BLOCK_411_112_2_18_1_27_49_381,"block_Alloc,xmalloc failed")
TRACE_MSG(BLOCK_427_112_2_18_1_27_49_382,"block_Alloc,fifo has been realeased.")
TRACE_MSG(BLOCK_441_112_2_18_1_27_49_383,"block_Alloc,no free blocks")
TRACE_MSG(BLOCK_504_112_2_18_1_27_50_384,"block_New,fifo doesn't create yet")
TRACE_MSG(BLOCK_513_112_2_18_1_27_50_385,"block_New, there are no free blocks")
TRACE_MSG(BLOCK_578_112_2_18_1_27_50_386,"block_Free,fifo doesn't create yet")
TRACE_MSG(BLOCK_588_112_2_18_1_27_50_387,"block_Free, re-free a block")
TRACE_MSG(BLOCK_630_112_2_18_1_27_50_388,"block_FifoAdd, re-add a block")
TRACE_MSG(BLOCK_788_112_2_18_1_27_50_389,"block_FifoGetPTS, paramater is error")
TRACE_MSG(FILE_CACHE_RD_34_112_2_18_1_27_50_390,"Xfileopen,file_name=%s,access_mode=%s")
TRACE_MSG(FILE_CACHE_RD_142_112_2_18_1_27_51_391,"video cache,slot %d, user num %d")
TRACE_MSG(FILE_CACHE_RD_152_112_2_18_1_27_51_392,"audio cache,slot %d, user num %d")
TRACE_MSG(FILE_CACHE_RD_283_112_2_18_1_27_51_393,"file_cache, failed to all stream CB")
TRACE_MSG(FILE_CACHE_RD_309_112_2_18_1_27_51_394,"file_cache, Open(), invlidate file name")
TRACE_MSG(FILE_CACHE_RD_319_112_2_18_1_27_51_395,"file_cache, failed to open file %s")
TRACE_MSG(FILE_CACHE_RD_404_112_2_18_1_27_51_396,"file_cache, peek, invalidate stream")
TRACE_MSG(FILE_CACHE_RD_426_112_2_18_1_27_51_397,"file_cache, peek, xmalloc failed")
TRACE_MSG(FILE_CACHE_RD_455_112_2_18_1_27_51_398,"file_cache, invalidate stream or read len zero")
TRACE_MSG(FILE_CACHE_RD_485_112_2_18_1_27_51_399,"file_cache, write, not support when on read mode, stream 0x%x, buf 0x%x, len %d")
TRACE_MSG(FILE_CACHE_RD_557_112_2_18_1_27_51_400,"file cache, buffer overflow")
TRACE_MSG(FILE_CACHE_RD_564_112_2_18_1_27_51_401,"file cache, buffer overflow")
TRACE_MSG(FILE_CACHE_RD_583_112_2_18_1_27_51_402,"file cache, buffer overflow")
TRACE_MSG(FILE_CACHE_RD_633_112_2_18_1_27_52_403,"CacheLoading,XFREAD fail,i_len[0x%x],i_append[0x%x], hal_pos[0x%x], pos[0x%x], file size[0x%x]")
TRACE_MSG(FILE_CACHE_RD_650_112_2_18_1_27_52_404,"Readblock11111,file_cache, invalid port")
TRACE_MSG(FILE_CACHE_RD_663_112_2_18_1_27_52_405,"Readblock33333333 file_ca")
TRACE_MSG(FILE_CACHE_RD_668_112_2_18_1_27_52_406,"Readblock33333333 file_ca")
TRACE_MSG(FILE_CACHE_RD_675_112_2_18_1_27_52_407,"Readblock222222file_cache, readblock, len %d > max frame size %d")
TRACE_MSG(FILE_CACHE_RD_740_112_2_18_1_27_52_408,"Readblock 444444")
TRACE_MSG(FILE_CACHE_RD_771_112_2_18_1_27_52_409,"Readblock 555555")
TRACE_MSG(FILE_CACHE_RD_809_112_2_18_1_27_52_410,"Readblock 6666666")
TRACE_MSG(FILE_CACHE_RD_836_112_2_18_1_27_52_411,"file_cache, writeblock, not support when on read mode, stream0x%x, block 0x%x, len %d")
TRACE_MSG(FILE_CACHE_RD_850_112_2_18_1_27_52_412,"udisk access, seek, invalidate stream")
TRACE_MSG(FILE_CACHE_RD_861_112_2_18_1_27_52_413,"file_cache, seek, pos < 0")
TRACE_MSG(FILE_CACHE_RD_869_112_2_18_1_27_52_414,"file_cache, seek, pos > size")
TRACE_MSG(FILE_CACHE_RD_885_112_2_18_1_27_52_415,"file_cache, size, invalidate stream")
TRACE_MSG(FILE_CACHE_RD_899_112_2_18_1_27_52_416,"file_cache, tell, invalidate stream")
TRACE_MSG(FILE_CACHE_RD_914_112_2_18_1_27_52_417,"file_cache_read, ConfigPort fail!")
TRACE_MSG(FILE_CACHE_RD_920_112_2_18_1_27_52_418,"file_cache_read, invalid port id: %d")
TRACE_MSG(FILE_CACHE_RD_946_112_2_18_1_27_52_419,"file_cache, initcache, alloc memory failed\n")
TRACE_MSG(FILE_CACHE_RD_997_112_2_18_1_27_52_420,"file_cache_rd.c, p_cache->i_slot_user_num[slot_start] error")
TRACE_MSG(FILE_CACHE_WR_223_112_2_18_1_27_53_421,"file_cache, readblock, not support when on write mode, stream 0x%x, len %d")
TRACE_MSG(FILE_CACHE_WR_291_112_2_18_1_27_53_422,"file_cache, __write, invalid lenght <= 0")
TRACE_MSG(FILE_CACHE_WR_301_112_2_18_1_27_53_423,"file_cache, error happen")
TRACE_MSG(FILE_CACHE_WR_391_112_2_18_1_27_53_424,"file_cache, write, invalid parameters")
TRACE_MSG(FILE_CACHE_WR_411_112_2_18_1_27_53_425,"file_cache, writeblock, invalid parameters")
TRACE_MSG(FILE_CACHE_WR_450_112_2_18_1_27_53_426,"%s(Line%d), DV_WR_PADDING_LEN <= abs(i_length_left), i_length_left = %d")
TRACE_MSG(FILE_CACHE_WR_464_112_2_18_1_27_53_427,"file_cache, writeblock, stream err")
TRACE_MSG(FILE_CACHE_WR_478_112_2_18_1_27_53_428,"udisk access, seek, invalidate stream")
TRACE_MSG(FILE_CACHE_WR_489_112_2_18_1_27_53_429,"file_cache, seek, pos < 0")
TRACE_MSG(FILE_CACHE_WR_494_112_2_18_1_27_53_430,"file_cache, seek, pos > size")
TRACE_MSG(FILE_CACHE_WR_510_112_2_18_1_27_53_431,"file_cache, size, invalidate stream")
TRACE_MSG(FILE_CACHE_WR_524_112_2_18_1_27_53_432,"file_cache, tell, invalidate stream")
TRACE_MSG(FILE_CACHE_WR_633_112_2_18_1_27_54_433,"mrecorder_GenerateTmpFileName,string_unit_len=%d,error")
TRACE_MSG(FILE_CACHE_WR_640_112_2_18_1_27_54_434,"mrecorder_GenerateTmpFileName,malloc[%d,%d] error")
TRACE_MSG(FILE_CACHE_WR_663_112_2_18_1_27_54_435,"mrecorder_JudgeStringUnitLen,error,p_string=0x%x,len=%d")
TRACE_MSG(FILE_CACHE_WR_676_112_2_18_1_27_54_436,"mrecorder_JudgeStringUnitLen,error,offset=%d")
TRACE_MSG(FILE_CACHE_WR_710_112_2_18_1_27_54_437,"mrecorder_JudgeStringUnitLen,unicode error,offset=%d")
TRACE_MSG(FILE_CACHE_WR_722_112_2_18_1_27_54_438,"mrecorder_JudgeStringUnitLen,unicode,byte0-7=0x[%x,%x,%x,%x;%x,%x,%x,%x]")
TRACE_MSG(FILE_CACHE_WR_756_112_2_18_1_27_54_439,"suffix_support,char,0x[%x,%x,%x,%x];0x[%x,%x,%x,%x];0x[%x,%x,%x,%x]")
TRACE_MSG(FILE_CACHE_WR_761_112_2_18_1_27_54_440,"mrecorder_JudgeStringUnitLen,char error,offset=%d")
TRACE_MSG(FILE_CACHE_WR_772_112_2_18_1_27_54_441,"mrecorder_JudgeStringUnitLen,char,byte0-3=0x[%x,%x,%x,%x]")
TRACE_MSG(FILE_CACHE_WR_808_112_2_18_1_27_54_442,"mrecorder_string_char2unicode,char string=%s")
TRACE_MSG(FILE_CACHE_WR_900_112_2_18_1_27_54_443,"file_cache, failed to all stream CB")
TRACE_MSG(FILE_CACHE_WR_913_112_2_18_1_27_54_444,"file_cache,Open(),file name is too long,byte_len=%d>size=%d")
TRACE_MSG(FILE_CACHE_WR_921_112_2_18_1_27_54_445,"file_cache,open,mrecorder_JudgeStringUnitLen,%d")
TRACE_MSG(FILE_CACHE_WR_931_112_2_18_1_27_54_446,"file_cache,open,mrecorder_GenerateTmpFileName for av error")
TRACE_MSG(FILE_CACHE_WR_938_112_2_18_1_27_54_447,"file_cache,open,mrecorder_GenerateTmpFileName for list error")
TRACE_MSG(FILE_CACHE_WR_947_112_2_18_1_27_54_448,"file_cache,open,av old file name=%s")
TRACE_MSG(FILE_CACHE_WR_951_112_2_18_1_27_54_449,"file_cache,open,mrecorder_GenerateTmpFileName for av error")
TRACE_MSG(FILE_CACHE_WR_954_112_2_18_1_27_54_450,"file_cache,open,av tmp file name=%s")
TRACE_MSG(FILE_CACHE_WR_959_112_2_18_1_27_54_451,"file_cache,open,mrecorder_GenerateTmpFileName for list error")
TRACE_MSG(FILE_CACHE_WR_962_112_2_18_1_27_54_452,"file_cache,open,list tmp file name=%s")
TRACE_MSG(FILE_CACHE_WR_966_112_2_18_1_27_54_453,"file_cache,open,string_unit_byte_len[%d] error")
TRACE_MSG(FILE_CACHE_WR_984_112_2_18_1_27_54_454,"file_cache_wr,av file open,name=%s")
TRACE_MSG(FILE_CACHE_WR_992_112_2_18_1_27_54_455,"file_cache_wr, failed to open file %s")
TRACE_MSG(FILE_CACHE_WR_1011_112_2_18_1_27_54_456,"failed to open tmpfile")
TRACE_MSG(FILE_CACHE_WR_1019_112_2_18_1_27_54_457,"failed to open tmpfile")
TRACE_MSG(FILE_CACHE_WR_1028_112_2_18_1_27_54_458,"file_cache_wr,tmp file open,name=%s")
TRACE_MSG(FILE_CACHE_WR_1033_112_2_18_1_27_54_459,"failed to open tmpfile,%s")
TRACE_MSG(FILE_CACHE_WR_1096_112_2_18_1_27_54_460,"file_cache_wr,rename file name,old=%s,new%s")
TRACE_MSG(TMPFILE_138_112_2_18_1_27_55_461,"memtmpfile_AppendWrite, malloc failed")
TRACE_MSG(TMPFILE_167_112_2_18_1_27_55_462,"tmpfile_Read, read failed")
TRACE_MSG(TMPFILE_185_112_2_18_1_27_55_463,"tmpfile_AppendWrite, write failed")
TRACE_MSG(TMPFILE_272_112_2_18_1_27_55_464,"tmpfile_Close,close failed.")
TRACE_MSG(TMPFILE_277_112_2_18_1_27_55_465,"tmpfile_Close,delete failed.")
TRACE_MSG(AUDIO_DEC_199_112_2_18_1_27_57_466,"Failed to AUDIO_Play, return %d")
TRACE_MSG(RMVB_DEC_225_112_2_18_1_28_1_467,"rmvb_dec, _SyncTable_Add, table_end=%d")
TRACE_MSG(RMVB_DEC_235_112_2_18_1_28_1_468,"rmvb_dec, _SyncTable_Remove, table_begin=%d")
TRACE_MSG(RMVB_DEC_247_112_2_18_1_28_1_469,"rmvb_dec, _SyncTable_GetSync, time=%d, [begin, end]=[%d, %d]: [%d, %d]")
TRACE_MSG(RMVB_DEC_319_112_2_18_1_28_1_470,"rmvb_dec, _Seek_To_IVOP_InFifo, sync_id[%d], begin end[%d, %d], depth[%d]")
TRACE_MSG(RMVB_DEC_332_112_2_18_1_28_1_471,"_Seek_To_IVOP_InFifo, free pt_bakblock, flags=%d")
TRACE_MSG(RMVB_DEC_344_112_2_18_1_28_1_472,"_Seek_To_IVOP_InFifo, I_FRAME, i=%d")
TRACE_MSG(RMVB_DEC_357_112_2_18_1_28_1_473,"_Seek_To_IVOP_InFifo, get I_FRAME")
TRACE_MSG(RMVB_DEC_363_112_2_18_1_28_1_474,"rmvb_dec, _Seek_To_IVOP_InFifo, block_FifoRemove depth=0")
TRACE_MSG(RMVB_DEC_404_112_2_18_1_28_2_475,"rmvb_dec task, error happen when waiting for event")
TRACE_MSG(RMVB_DEC_410_112_2_18_1_28_2_476,"rmvb_dec task, receieve exit event")
TRACE_MSG(RMVB_DEC_438_112_2_18_1_28_2_477,"rmvb_dec task, MP4_DECODER_PAUSE_EVENT")
TRACE_MSG(RMVB_DEC_467_112_2_18_1_28_2_478,"RMVB_DecoderTaskEntry, cur_time=%d, first_pos=%d, offset=%d")
TRACE_MSG(RMVB_DEC_494_112_2_18_1_28_2_479,"rmvb_dec, b_expected_IVOP, block is not I frame, free p_block 0x%x")
TRACE_MSG(RMVB_DEC_529_112_2_18_1_28_2_480,"rmvb_dec,video is too slow,seek one I VOP,syn_time[%d]")
TRACE_MSG(RMVB_DEC_533_112_2_18_1_28_2_481,"rmvb_dec,video is too slow, but no I VOP in next 1000ms. syn_time[%d]")
TRACE_MSG(RMVB_DEC_615_112_2_18_1_28_2_482,"rmvb_dec, cur_time=%d, bef_time=%d, aft_time=%d, interval=[%d, %d]")
TRACE_MSG(RMVB_DEC_652_112_2_18_1_28_2_483,"rmvb_dec, time_offset_a_v[%d]")
TRACE_MSG(RMVB_DEC_659_112_2_18_1_28_2_484,"rmvb_dec, DecoderTaskEntry, failed to call set event")
TRACE_MSG(RMVB_DEC_684_112_2_18_1_28_2_485,"rmvb_dec, yuv buffer overflow!")
TRACE_MSG(RMVB_DEC_690_112_2_18_1_28_2_486,"rmvb_dec, DecoderTaskEntry, failed to call set event")
TRACE_MSG(RMVB_DEC_713_112_2_18_1_28_2_487,"the pre_sample is special and skipped,finding one I frame now.")
TRACE_MSG(RMVB_DEC_719_112_2_18_1_28_2_488,"rmvb_dec, fifo block num > 60, seek IVOP")
TRACE_MSG(RMVB_DEC_724_112_2_18_1_28_2_489,"rmvb_dec, block 0x%x, expected_IVOP %d, i_flags %d, disp_skip %d,i_depth %d,i_frame %d")
TRACE_MSG(RMVB_DEC_750_112_2_18_1_28_2_490,"rmvb_dec, frame: 0x%x, data ptr[0x%x], len[%d], segment num %d")
TRACE_MSG(RMVB_DEC_761_112_2_18_1_28_2_491,"RMVB_DecDecode,ret=%d,frm_num[%ld],usedTime[%ld],i_flags[%d],pts=%d,frm_eff=%d")
TRACE_MSG(RMVB_DEC_789_112_2_18_1_28_2_492,"rmvb_dec task, decode_result, MMDEC_OUTPUT_BUFFER_OVERFLOW")
TRACE_MSG(RMVB_DEC_869_112_2_18_1_28_3_493,"rmvb_dec, block_new failed.")
TRACE_MSG(RMVB_DEC_882_112_2_18_1_28_3_494,"rmvb_dec task, decode_result, MMDEC_ERROR")
TRACE_MSG(RMVB_DEC_888_112_2_18_1_28_3_495,"rmvb_dec task, decode_result, MMDEC_PARAM_ERROR")
TRACE_MSG(RMVB_DEC_894_112_2_18_1_28_3_496,"rmvb_dec task, decode_result, MMDEC_MEMORY_ERROR")
TRACE_MSG(RMVB_DEC_898_112_2_18_1_28_3_497,"rmvb_dec task, decode_result, MMDEC_INVALID_STATUS")
TRACE_MSG(RMVB_DEC_902_112_2_18_1_28_3_498,"rmvb_dec task, decode_result, MMDEC_STREAM_ERROR")
TRACE_MSG(RMVB_DEC_908_112_2_18_1_28_3_499,"rmvb_dec task, decode_result, MMDEC_OUTPUT_BUFFER_OVERFLOW")
TRACE_MSG(RMVB_DEC_912_112_2_18_1_28_3_500,"rmvb_dec task, decode_result, MMDEC_HW_ERROR")
TRACE_MSG(RMVB_DEC_917_112_2_18_1_28_3_501,"rmvb_dec task, decode_result, MMDEC_NOT_SUPPORTED")
TRACE_MSG(RMVB_DEC_922_112_2_18_1_28_3_502,"rmvb_dec task, decode_result, MMDEC_FRAME_SEEK_IVOP")
TRACE_MSG(RMVB_DEC_926_112_2_18_1_28_3_503,"rmvb_dec task, decode_result=%d, return err!")
TRACE_MSG(RMVB_DEC_932_112_2_18_1_28_3_504,"rmvb_dec task, (SCI_TRUE == decode_ctrl_flag.b_expected_IVOP), free this frame and break")
TRACE_MSG(RMVB_DEC_942_112_2_18_1_28_3_505,"rmvb_dec decoder task, encount undefined event,i_event_flag[0x%x],status[0x%x]")
TRACE_MSG(RMVB_DEC_952_112_2_18_1_28_3_506,"rmvb_dec, Active()")
TRACE_MSG(RMVB_DEC_965_112_2_18_1_28_3_507,"rmvb_dec, Deactive()")
TRACE_MSG(RMVB_DEC_978_112_2_18_1_28_3_508,"rmvb_dec, Open(),status[0x%x]")
TRACE_MSG(RMVB_DEC_981_112_2_18_1_28_3_509,"rmvb_dec.c,close,status error!")
TRACE_MSG(RMVB_DEC_987_112_2_18_1_28_3_510,"rmvb_dec.c, open, movie size is too large.")
TRACE_MSG(RMVB_DEC_997_112_2_18_1_28_3_511,"rmvb_dec, open, RV40")
TRACE_MSG(RMVB_DEC_1002_112_2_18_1_28_3_512,"rmvb_dec, open, RV30, unsupported")
TRACE_MSG(RMVB_DEC_1008_112_2_18_1_28_3_513,"rmvb_dec, open, unsupported video codec: 0x%x")
TRACE_MSG(RMVB_DEC_1024_112_2_18_1_28_3_514,"rmvb_dec, open, RVDecMemSizeCal w,h=[%d,%d], memsize=%d")
TRACE_MSG(RMVB_DEC_1034_112_2_18_1_28_3_515,"rmvb_dec, open, p_decoder_buffer_inter alloca fail")
TRACE_MSG(RMVB_DEC_1043_112_2_18_1_28_3_516,"failed to call RvDecInit()")
TRACE_MSG(RMVB_DEC_1051_112_2_18_1_28_3_517,"rmvb_dec, open, p_decoder_buffer_extra alloca fail")
TRACE_MSG(RMVB_DEC_1060_112_2_18_1_28_3_518,"rmvb_dec.c, open, RvDecMemInit failed!")
TRACE_MSG(RMVB_DEC_1068_112_2_18_1_28_3_519,"rmvb_dec, Open(), failed to create event")
TRACE_MSG(RMVB_DEC_1093_112_2_18_1_28_3_520,"rmvb_dec, Open(), failed to create task")
TRACE_MSG(RMVB_DEC_1114_112_2_18_1_28_3_521,"rmvb_dec.c, open, err!")
TRACE_MSG(RMVB_DEC_1152_112_2_18_1_28_3_522,"rmvb_dec, close(),status[0x%x]")
TRACE_MSG(RMVB_DEC_1156_112_2_18_1_28_3_523,"rmvb_dec.c,close,status error!")
TRACE_MSG(RMVB_DEC_1162_112_2_18_1_28_3_524,"rmvb_dec.c, RvDecRelease failed!")
TRACE_MSG(RMVB_DEC_1217_112_2_18_1_28_3_525,"rmvb_dec, Send, p_block 0x%x, i_datalen=%d, frame_num=%d, flag=%d, depth=%d")
TRACE_MSG(RMVB_DEC_1227_112_2_18_1_28_3_526,"rmvb_dec, Send(), failed to call set event")
TRACE_MSG(RMVB_DEC_1238_112_2_18_1_28_3_527,"rmvb_dec, send(),fail")
TRACE_MSG(RMVB_DEC_1261_112_2_18_1_28_3_528,"rmvb_dec, Start(),status[0x%x]")
TRACE_MSG(RMVB_DEC_1273_112_2_18_1_28_3_529,"rmvb_dec.c, RvDecMemInit failed!")
TRACE_MSG(RMVB_DEC_1281_112_2_18_1_28_3_530,"rmvb_dec, Resume, failed to call set event")
TRACE_MSG(RMVB_DEC_1293_112_2_18_1_28_3_531,"rmvb_dec, Stop(),status[0x%x]")
TRACE_MSG(RMVB_DEC_1315_112_2_18_1_28_3_532,"rmvb_dec, Pause(),status[0x%x]")
TRACE_MSG(RMVB_DEC_1324_112_2_18_1_28_3_533,"rmvb_dec, Pause() OK")
TRACE_MSG(RMVB_DEC_1347_112_2_18_1_28_3_534,"rmvb_dec, Resume(),status[0x%x]")
TRACE_MSG(RMVB_DEC_1360_112_2_18_1_28_3_535,"__NotifyRMVBYUVBufferIsFree, PNULL BUFFER.")
TRACE_MSG(RMVB_DEC_1397_112_2_18_1_28_4_536,"rmvb_dec, SetConfig, re sync OK")
TRACE_MSG(RMVB_DEC_1402_112_2_18_1_28_4_537,"rmvb_dec, SetConfig, re sync fail,status[%d]")
TRACE_MSG(RMVB_DEC_1418_112_2_18_1_28_4_538,"rmvb_dec CMD_DEC_ROTATION set %d")
TRACE_MSG(RMVB_DEC_1431_112_2_18_1_28_4_539,"rmvb_dec, SetConfig, playback_mode = MODE_SYNC")
TRACE_MSG(RMVB_DEC_1437_112_2_18_1_28_4_540,"rmvb_dec, SetConfig, playback_mode = MODE_RATE")
TRACE_MSG(RMVB_DEC_1443_112_2_18_1_28_4_541,"rmvb_dec, SetConfig, b_skip_display = SCI_TRUE")
TRACE_MSG(RMVB_DEC_1447_112_2_18_1_28_4_542,"rmvb_dec, SetConfig, invalid cmd")
TRACE_MSG(RMVB_DEC_1477_112_2_18_1_28_4_543,"rmvb_dec, GetInfo, invalid cmd")
TRACE_MSG(AVI_382_112_2_18_1_28_5_544,"cannot peek()")
TRACE_MSG(AVI_397_112_2_18_1_28_5_545,"avi module discarded (invalid header)")
TRACE_MSG(AVI_406_112_2_18_1_28_5_546,"avi, p_demux->p_sys, malloc fail!")
TRACE_MSG(AVI_426_112_2_18_1_28_5_547,"avi module discarded (invalid file)")
TRACE_MSG(AVI_456_112_2_18_1_28_5_548,"avi module discarded (invalid file)")
TRACE_MSG(AVI_462_112_2_18_1_28_5_549,"cannot find avih chunk")
TRACE_MSG(AVI_474_112_2_18_1_28_5_550,"no stream defined!")
TRACE_MSG(AVI_491_112_2_18_1_28_5_551,"avi.c, p_sys->track, malloc fail!")
TRACE_MSG(AVI_518_112_2_18_1_28_5_552,"avi.c, avi_track_t, malloc fail!")
TRACE_MSG(AVI_580_112_2_18_1_28_5_553,"avi.c, tk->p_index , malloc fail!")
TRACE_MSG(AVI_644_112_2_18_1_28_6_554,"video config port fail")
TRACE_MSG(AVI_652_112_2_18_1_28_6_555,"avi.c, vids tk->p_index , malloc fail!")
TRACE_MSG(AVI_677_112_2_18_1_28_6_556,"avi.c, p_sys->p_syn_table, malloc fail!")
TRACE_MSG(AVI_749_112_2_18_1_28_6_557,"i_duration info error")
TRACE_MSG(AVI_809_112_2_18_1_28_6_558,"i_rate, or i_scale info error")
TRACE_MSG(AVI_831_112_2_18_1_28_6_559,"no valid track")
TRACE_MSG(AVI_861_112_2_18_1_28_6_560,"avi open, not find one index or no vide track, exit...")
TRACE_MSG(AVI_983_112_2_18_1_28_6_561,"avi demux close, p_sys is NULL")
TRACE_MSG(AVI_1122_112_2_18_1_28_6_562,"Demux, i_track[0x%x],[0x%x],[0x%x], i_idxposc out of range!")
TRACE_MSG(AVI_1134_112_2_18_1_28_6_563,"Demux, i_track[%d],i_dpts[%d] out of range!")
TRACE_MSG(AVI_1192_112_2_18_1_28_7_564,"Demux,no tracks[%d] need demuxing!")
TRACE_MSG(AVI_1214_112_2_18_1_28_7_565,"Demux,video fifo overflow!")
TRACE_MSG(AVI_1227_112_2_18_1_28_7_566,"Demux,audio fifo overflow!")
TRACE_MSG(AVI_1236_112_2_18_1_28_7_567,"Demux,track[%d],seek[0x%x] failed!")
TRACE_MSG(AVI_1282_112_2_18_1_28_7_568,"Demux,stream_ReadBlock,failed reading data,track[%d],result[%d]")
TRACE_MSG(AVI_1454_112_2_18_1_28_7_569,"Demux, i_track[%ld],i_idxposc out of range after load index")
TRACE_MSG(AVI_1531_112_2_18_1_28_7_570,"shouldn't yet be executed")
TRACE_MSG(AVI_1646_112_2_18_1_28_7_571,"AVI_TrackSeek,less than the first sync,after_sync_id=%d, after_sync_time=%d")
TRACE_MSG(AVI_1674_112_2_18_1_28_8_572,"AVI_TrackSeek,in table ,low=%d,time=%d, high=%d, time=%d")
TRACE_MSG(AVI_1685_112_2_18_1_28_8_573,"AVI_TrackSeek,out of table , high=%d, time=%d")
TRACE_MSG(AVI_1698_112_2_18_1_28_8_574,"GetVideoSyncPoint, i_time %d")
TRACE_MSG(AVI_1737_112_2_18_1_28_8_575,"SeekVideoToSyncPoint, invalid syn point,i_idxposc[%d] ")
TRACE_MSG(AVI_1750_112_2_18_1_28_8_576,"SeekVideoToSyncPoint, i_idxposc=%d, i_idxnb_base=%d, i_idxnb_cur_num=%d")
TRACE_MSG(AVI_1766_112_2_18_1_28_8_577,"SeekVideoToSyncPoint, out of table, i_idxnb_base=%d, i_idxnb_cur_num=%d")
TRACE_MSG(AVI_1782_112_2_18_1_28_8_578,"SeekVideoToSyncPoint, seek done, video pts=%d, sys time=%d, target_time=%d,i_idxnb_base=%d, i_idxnb_cur_num=%d")
TRACE_MSG(AVI_1792_112_2_18_1_28_8_579,"SeekVideoToSyncPoint, shouldn't yet be executed")
TRACE_MSG(AVI_1805_112_2_18_1_28_8_580,"SetTimeVideo, i_time %d")
TRACE_MSG(AVI_2018_112_2_18_1_28_8_581,"AVI_TrackSeek,less than the first sync,vide_chun_num=%d")
TRACE_MSG(AVI_2068_112_2_18_1_28_8_582,"AVI_TrackSeek,in table, vide_chun_num=%d,low=%d,high=%d,vide_chun_dest=%d")
TRACE_MSG(AVI_2073_112_2_18_1_28_8_583,"AVI_TrackSeek,out of table, vide_chun_num=%d")
TRACE_MSG(AVI_2088_112_2_18_1_28_8_584,"AVI_TrackSeek,check forward,tmp=%d < idxposc=%d")
TRACE_MSG(AVI_2098_112_2_18_1_28_8_585,"AVI_TrackSeek,check backward,tmp=%d > idxposc=%d")
TRACE_MSG(AVI_2119_112_2_18_1_28_8_586,"AVI_TrackSeek, audio check forward, tmp_index %d<%d, or tmp_block %d<%d")
TRACE_MSG(AVI_2128_112_2_18_1_28_8_587,"AVI_TrackSeek, audio check backward, tmp_index %d>%d, or tmp_block %d>%d")
TRACE_MSG(AVI_2144_112_2_18_1_28_8_588," AVI_TrackSeek, audo syn point is invalid!")
TRACE_MSG(AVI_2167_112_2_18_1_28_9_589,"AVI_TrackSeek, AVI_IndexDynamicLoad, invalid syn point,stream[%d] ")
TRACE_MSG(AVI_2505_112_2_18_1_28_9_590,"%s(Line%d), tk->i_idxnb_cur_num(%u) >= INDEX_ENTRY_TABLE_NUM")
TRACE_MSG(AVI_2607_112_2_18_1_28_9_591,"cannot find indx (misdetect/broken OpenDML file?)")
TRACE_MSG(AVI_2635_112_2_18_1_28_9_592,"AVI_IndexLoad_indx, super_indx_subchunk malloc fail!")
TRACE_MSG(AVI_2646_112_2_18_1_28_9_593,"AVI_IndexLoad_indx, super_indx_data_ptr malloc fail!")
TRACE_MSG(AVI_2666_112_2_18_1_28_10_594,"AVI_IndexLoad_indx, p_super_indx_subchunk malloc fail!")
TRACE_MSG(AVI_2676_112_2_18_1_28_10_595,"AVI_IndexLoad_indx, AVI_ChunkRead_ix error, no super index data")
TRACE_MSG(AVI_2689_112_2_18_1_28_10_596,"AVI_IndexLoad_indx, AVI_ChunkRead_ix error, i_super_indx_subchunk %d")
TRACE_MSG(AVI_2698_112_2_18_1_28_10_597,"AVI_IndexLoad_indx, super index err.")
TRACE_MSG(AVI_2833_112_2_18_1_28_10_598,"AVI_IndexLoad_idx1,return err")
TRACE_MSG(AVI_2917_112_2_18_1_28_10_599,"AVI_ChunkDynamicRead_idx1,return err")
TRACE_MSG(AVI_3125_112_2_18_1_28_10_600,"AVI_IndexDynamicLoad_indx, cannot find indx (misdetect/broken OpenDML file?)")
TRACE_MSG(AVI_3148_112_2_18_1_28_10_601,"AVI_IndexDynamicLoad_indx,return err")
TRACE_MSG(AVI_3167_112_2_18_1_28_10_602,"AVI_IndexDynamicLoad_indx, super index,i_subchunk_num[%d] err")
TRACE_MSG(AVI_3202_112_2_18_1_28_11_603,"AVI_IndexDynamicLoad_indx, load to the end.")
TRACE_MSG(AVI_3239_112_2_18_1_28_11_604,"AVI_IndexDynamicLoad_indx, unknown type index(0x%x)")
TRACE_MSG(AVI_3292_112_2_18_1_28_11_605,"AVI_IndexDynamicLoad_indx, cannot find indx (misdetect/broken OpenDML file?)")
TRACE_MSG(AVI_3316_112_2_18_1_28_11_606,"AVI_IndexDynamicLoad_indx,return err")
TRACE_MSG(AVI_3336_112_2_18_1_28_11_607,"AVI_IndexDynamicLoad_indx, super index,i_subchunk_num[%d] err")
TRACE_MSG(AVI_3356_112_2_18_1_28_11_608,"AVI_IndexDynamicLoad_indx,return err")
TRACE_MSG(AVI_3493_112_2_18_1_28_11_609,"AVI_SynTableCreate_idx1,return err")
TRACE_MSG(AVI_3811_112_2_18_1_28_12_610,"AVI_SynTableCreateEx_indx, no vide exist!")
TRACE_MSG(AVI_3844_112_2_18_1_28_12_611,"AVI_IndexDynamicLoad_indx, cannot find indx (misdetect/broken OpenDML file?)")
TRACE_MSG(AVI_3892_112_2_18_1_28_12_612,"AVI_IndexDynamicLoad_indx,return err")
TRACE_MSG(AVI_3957_112_2_18_1_28_12_613,"AVI_IndexDynamicLoad_indx, super index,i_subchunk_num[%d] err")
TRACE_MSG(AVI_3983_112_2_18_1_28_12_614,"AVI_IndexDynamicLoad_indx,return err")
TRACE_MSG(AVI_4199_112_2_18_1_28_13_615,"AVI_SynIndxAddEntryEx, super index,i_subchunk_num[%d] err")
TRACE_MSG(AVI_4338_112_2_18_1_28_13_616,"AVI_SynTableCreate_idx1,return err")
TRACE_MSG(LIBAVI_93_112_2_18_1_28_14_617,"AVI_NextChunk,file or chunk size is larger than 4GB.chunk_pos=0x%x,size=0x%x")
TRACE_MSG(LIBAVI_98_112_2_18_1_28_14_618,"AVI_NextChunk,out of file size.")
TRACE_MSG(LIBAVI_161_112_2_18_1_28_14_619,"AVI_ChunkRead_list, avi chunk malloc fail.")
TRACE_MSG(LIBAVI_318_112_2_18_1_28_14_620,"malformed avi file")
TRACE_MSG(LIBAVI_323_112_2_18_1_28_14_621,"malformed avi file")
TRACE_MSG(LIBAVI_473_112_2_18_1_28_14_622,"AVI_ChunkRead_idx1,read fail!")
TRACE_MSG(LIBAVI_539_112_2_18_1_28_14_623,"AVI_ChunkDynamicRead_idx1,out of range err, left %d")
TRACE_MSG(LIBAVI_573_112_2_18_1_28_15_624,"AVI_ChunkDynamicRead_idx1,read fail!")
TRACE_MSG(LIBAVI_631_112_2_18_1_28_15_625,"AVI_ChunkDynamicRead_indx, p_index == PNULL")
TRACE_MSG(LIBAVI_647_112_2_18_1_28_15_626,"AVI_ChunkDynamicRead_indx, invalid index sub type %d")
TRACE_MSG(LIBAVI_654_112_2_18_1_28_15_627,"AVI_ChunkDynamicRead_indx,out of range err, left %d")
TRACE_MSG(LIBAVI_688_112_2_18_1_28_15_628,"AVI_ChunkDynamicRead_indx,read fail!")
TRACE_MSG(LIBAVI_723_112_2_18_1_28_15_629,"AVI_ChunkDynamicRead_indx, invalid index type!")
TRACE_MSG(LIBAVI_785_112_2_18_1_28_15_630,"AVI_ChunkRead_idx1,read fail!")
TRACE_MSG(LIBAVI_856_112_2_18_1_28_15_631,"AVI_ChunkRead_ix, error happened, unknow index chunk.")
TRACE_MSG(LIBAVI_893_112_2_18_1_28_15_632,"AVI_ChunkRead_idx1,read fail!")
TRACE_MSG(LIBAVI_936_112_2_18_1_28_16_633,"AVI_ChunkRead_idx1,read fail!")
TRACE_MSG(LIBAVI_986_112_2_18_1_28_16_634,"AVI_ChunkRead_idx1,read fail!")
TRACE_MSG(LIBAVI_1039_112_2_18_1_28_16_635,"AVI_ChunkRead_idx1,read fail!")
TRACE_MSG(LIBAVI_1396_112_2_18_1_28_16_636,"AVI_ChunkReadRoot, chunk malloc fail. ")
TRACE_MSG(DEMUX_FLV_288_112_2_18_1_28_17_637,"demux_flv,flv_read_header,failed.")
TRACE_MSG(DEMUX_FLV_297_112_2_18_1_28_17_638,"demux_flv,xcalloc failed.")
TRACE_MSG(DEMUX_FLV_394_112_2_18_1_28_18_639,"FLV_TAG_TYPE_META,object_name_type=%d")
TRACE_MSG(DEMUX_FLV_429_112_2_18_1_28_18_640,"demux flv open, neither audio nor video is supported,audio_found=%d,video_found=%d")
TRACE_MSG(DEMUX_FLV_547_112_2_18_1_28_18_641,"Demux,video fifo overflow!")
TRACE_MSG(DEMUX_FLV_565_112_2_18_1_28_18_642,"demux_flv,read next header error.")
TRACE_MSG(DEMUX_FLV_589_112_2_18_1_28_18_643,"demux_flv,ACCESS_CACHE_OVERFLOW_ERR")
TRACE_MSG(DEMUX_FLV_597_112_2_18_1_28_18_644,"demux_flv,ACCESS_SAMPLE_TOOBIG")
TRACE_MSG(DEMUX_FLV_608_112_2_18_1_28_18_645,"demux_flv,error=%d,p_blk=0x%x")
TRACE_MSG(DEMUX_FLV_666_112_2_18_1_28_18_646," demux_flv,SetTime,i_time=%d,cur_time=%d")
TRACE_MSG(DEMUX_FLV_675_112_2_18_1_28_18_647,"SeekToSyncPoint,time used=%d")
TRACE_MSG(DEMUX_FLV_742_112_2_18_1_28_18_648,"VideoSeekToSyncPoint,cur_pos=%d,cur_time=%d;target_time=%d")
TRACE_MSG(DEMUX_FLV_1335_112_2_18_1_28_20_649,"Read Block,err type=%d")
TRACE_MSG(DEMUX_FLV_1364_112_2_18_1_28_20_650,"Read Block failed,acc_ret=%d")
TRACE_MSG(LIBMP4_OPT2_51_112_2_18_1_28_21_651,"MP4_LibMemAlloc failed,offset=%d,size=%d")
TRACE_MSG(LIBMP4_OPT2_585_112_2_18_1_28_22_652,"MP4_ReadBox_ftyp, not enough data[%ld][0x%x].")
TRACE_MSG(LIBMP4_OPT2_1411_112_2_18_1_28_24_653,"discarding drms")
TRACE_MSG(LIBMP4_OPT2_1437_112_2_18_1_28_24_654,"discarding calling drms_free()")
TRACE_MSG(LIBMP4_OPT2_1668_112_2_18_1_28_24_655,"stco, not support")
TRACE_MSG(LIBMP4_OPT2_2015_112_2_18_1_28_25_656,"out of memory")
TRACE_MSG(LIBMP4_OPT2_2220_112_2_18_1_28_25_657,"discarding read drms box")
TRACE_MSG(LIBMP4_OPT2_2512_112_2_18_1_28_26_658,"MP4_ReadBox,MP4_LibMemAlloc failed")
TRACE_MSG(LIBMP4_OPT2_2624_112_2_18_1_28_26_659,"MP4_BoxGetRoot,MP4_LibMemAlloc failed")
TRACE_MSG(MP4_OPT2_313_112_2_18_1_28_28_660,"mp4 demux open,MP4_InitLib fail")
TRACE_MSG(MP4_OPT2_325_112_2_18_1_28_28_661,"mp4 demux open,MP4 plugin discarded (cannot peek) fail")
TRACE_MSG(MP4_OPT2_345_112_2_18_1_28_28_662,"mp4 demux open,MP4 plugin discarded (not a valid file) fail")
TRACE_MSG(MP4_OPT2_402_112_2_18_1_28_28_663,"MP4 plugin discarded (no moov box)")
TRACE_MSG(MP4_OPT2_421_112_2_18_1_28_28_664,"cannot find /moov/mvhd")
TRACE_MSG(MP4_OPT2_437_112_2_18_1_28_28_665,"cannot find any /moov/trak")
TRACE_MSG(MP4_OPT2_468_112_2_18_1_28_28_666,"video config port fail")
TRACE_MSG(MP4_OPT2_613_112_2_18_1_28_29_667,"MP4_TrackUnselect111111,track[0x%x] will be disabled")
TRACE_MSG(MP4_OPT2_665_112_2_18_1_28_29_668,"Demux, video fifo overflow!")
TRACE_MSG(MP4_OPT2_675_112_2_18_1_28_29_669,"Demux, audio fifo overflow!")
TRACE_MSG(MP4_OPT2_681_112_2_18_1_28_29_670,"mp4_opt2, demux, audio_demux_time > media_time + 2500, break!")
TRACE_MSG(MP4_OPT2_694_112_2_18_1_28_29_671,"MP4_TrackUnselect2222222track[0x%x] will be disabled (eof?)")
TRACE_MSG(MP4_OPT2_716_112_2_18_1_28_29_672,"track[0x%x] block is too big, skip and seek next sample.")
TRACE_MSG(MP4_OPT2_781_112_2_18_1_28_29_673,"MP4_TrackUnselect4444444mp4_demux, at the end of the track")
TRACE_MSG(MP4_OPT2_811_112_2_18_1_28_29_674,"Demux, i_track[%d], es_out_Send fail!")
TRACE_MSG(MP4_OPT2_965_112_2_18_1_28_29_675,"TrackToSyncPoint, target outrange, reload stss backward")
TRACE_MSG(MP4_OPT2_1033_112_2_18_1_28_29_676,"TrackToSyncPoint, target outrange, reload stss forward")
TRACE_MSG(MP4_OPT2_1238_112_2_18_1_28_30_677,"SearchSyncPoint, target outrange, reload stss backward")
TRACE_MSG(MP4_OPT2_1315_112_2_18_1_28_30_678,"SearchSyncPoint, target outrange, reload stss forward")
TRACE_MSG(MP4_OPT2_1519_112_2_18_1_28_30_679,"SeekToSyncPoint,backward,TrackToSyncPoint faild")
TRACE_MSG(MP4_OPT2_1530_112_2_18_1_28_30_680,"mp4_demux,SeekToSyncPoint,do nothing,des_time=%d,cur_tim=%d,select=%d")
TRACE_MSG(MP4_OPT2_1558_112_2_18_1_28_30_681,"SeekToSyncPoint, MP4_TrackNextSample return error")
TRACE_MSG(MP4_OPT2_1569_112_2_18_1_28_31_682,"SeekToSyncPoint, MP4_TrackPreviousSample return error")
TRACE_MSG(MP4_OPT2_1733_112_2_18_1_28_31_683,"SearchVideoSyncPoint, no stss")
TRACE_MSG(MP4_OPT2_1739_112_2_18_1_28_31_684,"SearchVideoSyncPoint, not selected")
TRACE_MSG(MP4_OPT2_1745_112_2_18_1_28_31_685,"SearchVideoSyncPoint, current %d > target %d")
TRACE_MSG(MP4_OPT2_1759_112_2_18_1_28_31_686,"SearchVideoSyncPoint, SearchSyncPoint out of table")
TRACE_MSG(MP4_OPT2_1777_112_2_18_1_28_31_687,"SearchVideoSyncPoint, before_sync_id %d, before_destination %d, after_sync_id %d, after_destination %d")
TRACE_MSG(MP4_OPT2_1778_112_2_18_1_28_31_688,"SearchVideoSyncPoint, before interva %d, after interval %d, tk->i_sample=%d")
TRACE_MSG(MP4_OPT2_1808_112_2_18_1_28_31_689,"GetVideoSyncPoint, i_time %d")
TRACE_MSG(MP4_OPT2_1859_112_2_18_1_28_31_690,"SeekVideoToSyncPoint, p_sys->i_time %d, i_time %d, i_new_time %d, tk->i_sample %d")
TRACE_MSG(MP4_OPT2_1878_112_2_18_1_28_31_691,"SetTimeVideo, i_time=%d")
TRACE_MSG(MP4_OPT2_2546_112_2_18_1_28_32_692,"missing avcC")
TRACE_MSG(MP4_OPT2_2703_112_2_18_1_28_33_693,"Mp4_opt2.c : fix frames per sample: new=%d")
TRACE_MSG(MP4_OPT2_2790_112_2_18_1_28_33_694,"MP4_TrackCreate,timescale of p_track(0x%x) is zero")
TRACE_MSG(MP4_OPT2_3313_112_2_18_1_28_34_695,"the STTS cache is not consistent with the track")
TRACE_MSG(MP4_OPT2_3367_112_2_18_1_28_34_696,"the STTS cache is not consistent with the track")
TRACE_MSG(DEMUX_RMVB_185_112_2_18_1_28_39_697,"Demux_rmvb, ra_block_available, p_ra_block is null")
TRACE_MSG(DEMUX_RMVB_195_112_2_18_1_28_39_698,"Demux_rmvb, ra_block_available, audio fifo overflow!")
TRACE_MSG(DEMUX_RMVB_202_112_2_18_1_28_39_699,"Demux_rmvb, ra_block_available, block new fail")
TRACE_MSG(DEMUX_RMVB_226_112_2_18_1_28_39_700,"Demux_rmvb, ra_block_available, block 0x%x, pData 0x%x, ulDataLen 0x%x, i_pos %d, ra_interval %d, CurFileOffset %d, , frame_num %d")
TRACE_MSG(DEMUX_RMVB_242_112_2_18_1_28_39_701,"Demux_rmvb, ra_block_available,es_out_Send fail!")
TRACE_MSG(DEMUX_RMVB_266_112_2_18_1_28_39_702,"Demux_rmvb, rv_frame_available, p_frame is null")
TRACE_MSG(DEMUX_RMVB_274_112_2_18_1_28_39_703,"Demux_rmvb, rv_frame_available, num segments > 60")
TRACE_MSG(DEMUX_RMVB_285_112_2_18_1_28_39_704,"Demux_rmvb, rv_frame_available, discard_b_frame, timestamp %d, frame_num %d")
TRACE_MSG(DEMUX_RMVB_301_112_2_18_1_28_39_705,"Demux_rmvb, rv_frame_available, discard_b_frame2, timestamp %d, frame_num %d")
TRACE_MSG(DEMUX_RMVB_312_112_2_18_1_28_39_706,"Demux_rmvb, rv_frame_available, video fifo overflow!")
TRACE_MSG(DEMUX_RMVB_323_112_2_18_1_28_39_707,"Demux_rmvb, rv_frame_available, b_seek_IVOP = SCI_TRUE, this frame is I frame")
TRACE_MSG(DEMUX_RMVB_328_112_2_18_1_28_39_708,"Demux_rmvb, rv_frame_available, b_seek_IVOP = SCI_TRUE, this frame is not a I frame")
TRACE_MSG(DEMUX_RMVB_336_112_2_18_1_28_39_709,"Demux_rmvb, rv_frame_available, block new fail")
TRACE_MSG(DEMUX_RMVB_362_112_2_18_1_28_39_710,"demux_rmvb, rv_frame_available, block 0x%x, pData 0x%x, ulDataLen 0x%x, i_pos %d, interval %d, frame_num %d, i_flags %d, i_interval %d")
TRACE_MSG(DEMUX_RMVB_386_112_2_18_1_28_39_711,"Demux_rmvb, rv_frame_available,es_out_Send fail!")
TRACE_MSG(DEMUX_RMVB_397_112_2_18_1_28_39_712,"rmvb_demux_error pError=0x%08x result=%d msg=%s")
TRACE_MSG(DEMUX_RMVB_470_112_2_18_1_28_39_713,"Demux_rmvb, Open")
TRACE_MSG(DEMUX_RMVB_474_112_2_18_1_28_39_714,"Demux_rmvb, open,rmvb plugin discarded (cannot peek) fail")
TRACE_MSG(DEMUX_RMVB_481_112_2_18_1_28_39_715,"Demux_rmvb, This file is not an .rm file.")
TRACE_MSG(DEMUX_RMVB_489_112_2_18_1_28_39_716,"Demux_rmvb, Open, p_sys malloc fail")
TRACE_MSG(DEMUX_RMVB_499_112_2_18_1_28_39_717,"Demux_rmvb, Open, parser create fail")
TRACE_MSG(DEMUX_RMVB_508_112_2_18_1_28_39_718,"Demux_rmvb, Open, packet buffer malloc fail")
TRACE_MSG(DEMUX_RMVB_519_112_2_18_1_28_39_719,"Demux_rmvb, Open, parser init io fail")
TRACE_MSG(DEMUX_RMVB_527_112_2_18_1_28_40_720,"Demux_rmvb, Open, parser read headers fail")
TRACE_MSG(DEMUX_RMVB_535_112_2_18_1_28_40_721,"Demux_rmvb, Error: rm_parser_get_num_streams() returns 0")
TRACE_MSG(DEMUX_RMVB_583_112_2_18_1_28_40_722,"Demux_rmvb, audio and video track do not exist")
TRACE_MSG(DEMUX_RMVB_586_112_2_18_1_28_40_723,"Demux_rmvb, audio track %d, video track %d")
TRACE_MSG(DEMUX_RMVB_592_112_2_18_1_28_40_724,"Demux_rmvb, Open, track malloc fail")
TRACE_MSG(DEMUX_RMVB_604_112_2_18_1_28_40_725,"Demux_rmvb, Open, tk malloc fail")
TRACE_MSG(DEMUX_RMVB_619_112_2_18_1_28_40_726,"Demux_rmvb, Open,  pRaDepack malloc fail")
TRACE_MSG(DEMUX_RMVB_631_112_2_18_1_28_40_727,"Demux_rmvb, Open, ra_depack_init fail")
TRACE_MSG(DEMUX_RMVB_637_112_2_18_1_28_40_728,"Demux_rmvb, Open, rm_parser_get_stream_header fail")
TRACE_MSG(DEMUX_RMVB_647_112_2_18_1_28_40_729,"Demux_rmvb, Open, Audio Codec4CC: 0x%x")
TRACE_MSG(DEMUX_RMVB_657_112_2_18_1_28_40_730,"Demux_rmvb, Open, ra_depack_get_codec_init_info  fail")
TRACE_MSG(DEMUX_RMVB_679_112_2_18_1_28_40_731,"Demux_rmvb, Open, fmt.p_extra malloc fail")
TRACE_MSG(DEMUX_RMVB_718_112_2_18_1_28_40_732,"Demux_rmvb, Open, tk malloc fail")
TRACE_MSG(DEMUX_RMVB_733_112_2_18_1_28_40_733,"Demux_rmvb, Open, rv_depack_create2  fail")
TRACE_MSG(DEMUX_RMVB_745_112_2_18_1_28_40_734,"Demux_rmvb, Open, rv_depack_init  fail")
TRACE_MSG(DEMUX_RMVB_751_112_2_18_1_28_40_735,"Demux_rmvb, Open, rm_parser_get_stream_header  fail")
TRACE_MSG(DEMUX_RMVB_759_112_2_18_1_28_40_736,"Demux_rmvb, Open, rv_depack_get_codec_init_info  fail")
TRACE_MSG(DEMUX_RMVB_768_112_2_18_1_28_40_737,"Demux_rmvb, Open, Video Codec4CC: 0x%x")
TRACE_MSG(DEMUX_RMVB_777_112_2_18_1_28_40_738,"Demux_rmvb, Open, ufFramesPerSecond %d, frame_rate %d")
TRACE_MSG(DEMUX_RMVB_782_112_2_18_1_28_40_739,"Demux_rmvb, Open, discard_b_frame on")
TRACE_MSG(DEMUX_RMVB_810_112_2_18_1_28_40_740,"Demux_rmvb, Open, file have not video or audio track")
TRACE_MSG(DEMUX_RMVB_820_112_2_18_1_28_40_741,"Demux_rmvb, Open, error handle")
TRACE_MSG(DEMUX_RMVB_888_112_2_18_1_28_40_742,"Demux_rmvb, Close")
TRACE_MSG(DEMUX_RMVB_1006_112_2_18_1_28_40_743,"Demux_rmvb, video or audio dec in fifo overflow: [%d, %d]")
TRACE_MSG(DEMUX_RMVB_1016_112_2_18_1_28_40_744,"Demux_rmvb, Demux, rm_parser_get_packet fail, timestamp %d, stream_num %d")
TRACE_MSG(DEMUX_RMVB_1023_112_2_18_1_28_40_745,"Demux_rmvb, Demux, invalid stream num, i_stream=%d, i_tracks=%d")
TRACE_MSG(DEMUX_RMVB_1047_112_2_18_1_28_41_746,"Demux_rmvb, Demux, undefined packet ")
TRACE_MSG(DEMUX_RMVB_1054_112_2_18_1_28_41_747,"Demux_rmvb, Demux, track is not activated ")
TRACE_MSG(DEMUX_RMVB_1091_112_2_18_1_28_41_748,"Demux_rmvb, Seek, time: %d, ulCurFileOffset %d")
TRACE_MSG(DEMUX_RMVB_1107_112_2_18_1_28_41_749,"Demux_rmvb, Seek,rmvb plugin discarded (cannot peek) fail")
TRACE_MSG(DEMUX_RMVB_1135_112_2_18_1_28_41_750,"Demux_rmvb, Seek, after seek, ulCurFileOffset %d, sys time %d, ret 0x%x")
TRACE_MSG(DEMUX_RMVB_1162_112_2_18_1_28_41_751,"Demux_rmvb, SetTime, time: %d")
TRACE_MSG(DEMUX_RMVB_1233_112_2_18_1_28_41_752,"demux_rmvb, RMVB_FreeRvFrame, p_block 0x%x, len %d, frm_num %d, i_flags %d")
TRACE_MSG(DEMUX_RMVB_1244_112_2_18_1_28_41_753,"demux_rmvb, RMVB_FreeRaBlock, p_block 0x%x")
TRACE_MSG(DEMUX_STREAM_345_112_2_18_1_29_1_754,"stream demux,p_player_ctx is NULL.")
TRACE_MSG(DEMUX_STREAM_393_112_2_18_1_29_1_755,"Demux, video fifo overflow!")
TRACE_MSG(DEMUX_STREAM_403_112_2_18_1_29_1_756,"Demux, audio fifo overflow!")
TRACE_MSG(AMR_ENC_176_112_2_18_1_29_2_757,"DumpAmrCallbackData,xmalloc failed")
TRACE_MSG(AMR_ENC_236_112_2_18_1_29_2_758,"AmrCallBack,block_Alloc failed,fifo_depth=%d")
TRACE_MSG(AMR_ENC_242_112_2_18_1_29_2_759,"AmrCallBack,fifo_overflow,fifo_depth=%d")
TRACE_MSG(AMR_ENC_260_112_2_18_1_29_2_760,"amr_enc, current amr only support mono-channel, channels %d")
TRACE_MSG(AMR_ENC_314_112_2_18_1_29_2_761,"amr_enc, start, invalid hDspMp4Record")
TRACE_MSG(PCM_ENC_225_112_2_18_1_29_3_762,"pcm_enc.c, encode_block, block_Alloc error")
TRACE_MSG(PCM_ENC_233_112_2_18_1_29_3_763,"pcm_enc.c, AdpcmEncoder error!")
TRACE_MSG(PCM_ENC_249_112_2_18_1_29_3_764,"pcm_enc.c, enter EncoderTaskEntry")
TRACE_MSG(PCM_ENC_257_112_2_18_1_29_3_765,"mjpg_enc task, error happen when waiting for event")
TRACE_MSG(PCM_ENC_300_112_2_18_1_29_3_766,"pcm_enc has stopped!")
TRACE_MSG(PCM_ENC_317_112_2_18_1_29_3_767,"pcm_enc.c, EncoderTaskEntry, block fifo is empty!")
TRACE_MSG(PCM_ENC_334_112_2_18_1_29_3_768,"pcm_enc, encode_block, failed to add to mux fifo1")
TRACE_MSG(PCM_ENC_351_112_2_18_1_29_3_769,"pcm_enc, encode_block, failed to add to mux fifo2")
TRACE_MSG(PCM_ENC_358_112_2_18_1_29_3_770,"pcm_enc.c, pcm encode format is not supported")
TRACE_MSG(PCM_ENC_368_112_2_18_1_29_3_771,"pcm_enc, encount undefined event")
TRACE_MSG(PCM_ENC_509_112_2_18_1_29_4_772,"PcmCallBack, status error.")
TRACE_MSG(PCM_ENC_545_112_2_18_1_29_4_773,"pcm_enc,capture,failed to call set event")
TRACE_MSG(PCM_ENC_593_112_2_18_1_29_4_774,"pcm_cap_start,src_dec=%d")
TRACE_MSG(PCM_ENC_598_112_2_18_1_29_4_775,"pcm_cap_start,s_h_pcm_capture has been opened already = 0x%x")
TRACE_MSG(PCM_ENC_620_112_2_18_1_29_4_776,"pcm_cap_start,s_h_pcm_capture opened fail")
TRACE_MSG(PCM_ENC_665_112_2_18_1_29_4_777,"pcm_cap_start,ATV s_h_pcm_capture opened fail")
TRACE_MSG(PCM_ENC_702_112_2_18_1_29_4_778,"pcm_cap_stop,src_dec=%d")
TRACE_MSG(PCM_ENC_740_112_2_18_1_29_4_779,"pcm_cap_stop,error,s_h_pcm_capture=0x%x,src_dec=%d")
TRACE_MSG(PCM_ENC_766_112_2_18_1_29_4_780,"pcm_enc, current pcm only support mono-channel, channels %d")
TRACE_MSG(PCM_ENC_801_112_2_18_1_29_4_781,"pcm_enc, open, invalid codec type")
TRACE_MSG(PCM_ENC_853_112_2_18_1_29_4_782,"pcm_enc, Open(), failed to create event")
TRACE_MSG(PCM_ENC_856_112_2_18_1_29_4_783,"pcm_enc.c, Open, create event successfully ")
TRACE_MSG(PCM_ENC_874_112_2_18_1_29_4_784,"%s(Line%d), SCI_INVALID_BLOCK_ID == s_enc_ctx.p_task")
TRACE_MSG(PCM_ENC_881_112_2_18_1_29_4_785,"pcm_enc.c, Open, create encoder task successfully")
TRACE_MSG(PCM_ENC_951_112_2_18_1_29_4_786,"pcm_enc,failed to trigger the encoder")
TRACE_MSG(MODULE_95_112_2_18_1_29_6_787,"module_InitBank, error, duplicate")
TRACE_MSG(AVI_MUX_313_112_2_18_1_29_8_788,"avi mux,open,free_space=0x%x")
TRACE_MSG(AVI_MUX_317_112_2_18_1_29_8_789,"avi mux, open, no enough space.")
TRACE_MSG(AVI_MUX_332_112_2_18_1_29_8_790,"avi mux open, malloc sys fail!")
TRACE_MSG(AVI_MUX_352_112_2_18_1_29_8_791,"avi mux open, malloc p_indx_buf fail!")
TRACE_MSG(AVI_MUX_366_112_2_18_1_29_8_792,"avi mux open, malloc idx1.entry fail!")
TRACE_MSG(AVI_MUX_421_112_2_18_1_29_8_793,"avi mux close,stream_Write error.")
TRACE_MSG(AVI_MUX_432_112_2_18_1_29_8_794,"avi mux close,stream_Write error.")
TRACE_MSG(AVI_MUX_494_112_2_18_1_29_8_795,"avi mux,finish std indx error.")
TRACE_MSG(AVI_MUX_530_112_2_18_1_29_8_796,"avi mux close,write p_hdr error.")
TRACE_MSG(AVI_MUX_581_112_2_18_1_29_9_797,"avi mux, control, cmd=%d,param=0x%x")
TRACE_MSG(AVI_MUX_607_112_2_18_1_29_9_798,"too many streams")
TRACE_MSG(AVI_MUX_886_112_2_18_1_29_9_799,"avi mux mux,write p_hdr error.")
TRACE_MSG(AVI_MUX_976_112_2_18_1_29_9_800,"avi_mux.c, mux,i_cat=%d,sum=%d,depth=%d,empty_need=%d,empty_interval=%d")
TRACE_MSG(AVI_MUX_1004_112_2_18_1_29_9_801,"avi_mux.c, mux,useless frame,insert empty frame.")
TRACE_MSG(AVI_MUX_1016_112_2_18_1_29_9_802,"Mux,avi,block fifo is empty2!")
TRACE_MSG(AVI_MUX_1035_112_2_18_1_29_9_803,"mux, expect_free_space=0x%x,free_space=0x%x")
TRACE_MSG(AVI_MUX_1048_112_2_18_1_29_9_804,"avi mux,file size reach the limited size,%d + %d + %d >= %d")
TRACE_MSG(AVI_MUX_1102_112_2_18_1_29_10_805,"avi mux,write block header error.")
TRACE_MSG(AVI_MUX_1118_112_2_18_1_29_10_806,"avi mux,write vol error.")
TRACE_MSG(AVI_MUX_1140_112_2_18_1_29_10_807,"avi mux,write block error.")
TRACE_MSG(AVI_MUX_1182_112_2_18_1_29_10_808,"mux, expect_free_space=0x%x,free_space=0x%x")
TRACE_MSG(AVI_MUX_1192_112_2_18_1_29_10_809,"avi mux,file size reach the limited size,%d + %d + %d(*84) >= %d")
TRACE_MSG(AVI_MUX_1202_112_2_18_1_29_10_810,"avi mux,write block header error.")
TRACE_MSG(AVI_MUX_1222_112_2_18_1_29_10_811,"Mux,avi,block fifo is empty2!")
TRACE_MSG(AVI_MUX_1259_112_2_18_1_29_10_812,"avi mux,write block error.")
TRACE_MSG(AVI_MUX_1266_112_2_18_1_29_10_813,"avi_mux, mux, audio i_chunk_len=%d")
TRACE_MSG(AVI_MUX_1324_112_2_18_1_29_10_814,"avi mux,write std indx error.")
TRACE_MSG(AVI_MUX_1348_112_2_18_1_29_10_815,"avi mux,super indx full.")
TRACE_MSG(AVI_MUX_1389_112_2_18_1_29_10_816,"Mux,avi, table in tmpfile is full,exit")
TRACE_MSG(AVI_MUX_1794_112_2_18_1_29_11_817,"failed to allocate a block")
TRACE_MSG(AVI_MUX_1846_112_2_18_1_29_11_818,"CreatEmptyVideFrame, block_New fail.")
TRACE_MSG(MP4_MUX_OPT_288_112_2_18_1_29_12_819,"mux,open,free_space=0x%x")
TRACE_MSG(MP4_MUX_OPT_445_112_2_18_1_29_12_820,"mp4 mux, control, cmd=%d,param=0x%x")
TRACE_MSG(MP4_MUX_OPT_487_112_2_18_1_29_12_821,"unsupported codec %4.4s in mp4")
TRACE_MSG(MP4_MUX_OPT_975_112_2_18_1_29_13_822,"mux, expect_free_space=0x%x,free_space=0x%x")
TRACE_MSG(MP4_MUX_OPT_988_112_2_18_1_29_13_823,"mp4 mux,file size reach the limited size,%d + %d + %d >= %d")
TRACE_MSG(MP4_MUX_OPT_1737_112_2_18_1_29_15_824,"FIXME ignoring trak (noaudio&&novideo)")
TRACE_MSG(PLAYER_INNER_1051_112_2_18_1_29_18_825,"mplayer_GetVideoSyncPoint, i_time %d")
TRACE_MSG(PLAYER_MAIN_94_112_2_18_1_29_19_826,"DPLAYER_Open in!")
TRACE_MSG(PLAYER_MAIN_106_112_2_18_1_29_19_827,"DPLAYER_Open, failed to create obj")
TRACE_MSG(PLAYER_MAIN_112_112_2_18_1_29_19_828,"DPLAYER_Open, failed to active obj")
TRACE_MSG(PLAYER_MAIN_143_112_2_18_1_29_19_829,"DPLAYER_Open, failed to create task")
TRACE_MSG(PLAYER_MAIN_155_112_2_18_1_29_19_830,"DPLAYER_Open return succ handle,0x%x")
TRACE_MSG(PLAYER_MAIN_172_112_2_18_1_29_19_831,"DPLAYER_Close,handle=0x%x")
TRACE_MSG(PLAYER_MAIN_177_112_2_18_1_29_19_832,"DPLAYER_Close, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_183_112_2_18_1_29_19_833,"DPLAYER_Close,obj=0x%x,is_open=%d")
TRACE_MSG(PLAYER_MAIN_187_112_2_18_1_29_19_834,"DPLAYER is not open yet !")
TRACE_MSG(PLAYER_MAIN_215_112_2_18_1_29_19_835,"DPLAYER_Play(),handle=0x%x")
TRACE_MSG(PLAYER_MAIN_220_112_2_18_1_29_19_836,"DPLAYER_Play, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_226_112_2_18_1_29_19_837,"DPLAYER_Play(),obj=0x%x,is_open=%d")
TRACE_MSG(PLAYER_MAIN_230_112_2_18_1_29_19_838,"DPLAYER_Play, demux is not open yet")
TRACE_MSG(PLAYER_MAIN_255_112_2_18_1_29_19_839,"DPLAYER_Play,ADD_DEC_SECURITY,argc=%d,argv[0]=0x%x")
TRACE_MSG(PLAYER_MAIN_293_112_2_18_1_29_19_840,"DPLAYER_Stop,handle=0x%x")
TRACE_MSG(PLAYER_MAIN_298_112_2_18_1_29_19_841,"DPLAYER_Stop, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_304_112_2_18_1_29_19_842,"DPLAYER_Stop, demux is not open yet")
TRACE_MSG(PLAYER_MAIN_310_112_2_18_1_29_19_843,"DPLAYER_Stop Fail!")
TRACE_MSG(PLAYER_MAIN_342_112_2_18_1_29_19_844,"DPLAYER_Pause,handle=0x%x")
TRACE_MSG(PLAYER_MAIN_347_112_2_18_1_29_19_845,"DPLAYER_Pause, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_353_112_2_18_1_29_19_846,"DPLAYER_Pause, demux is not open yet")
TRACE_MSG(PLAYER_MAIN_359_112_2_18_1_29_19_847,"DPLAYER_Pause Fail!")
TRACE_MSG(PLAYER_MAIN_376_112_2_18_1_29_19_848,"DPLAYER_PauseEx(),handle=0x%x")
TRACE_MSG(PLAYER_MAIN_381_112_2_18_1_29_19_849,"DPLAYER_PauseEx, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_387_112_2_18_1_29_19_850,"DPLAYER_PauseEx, demux is not open yet")
TRACE_MSG(PLAYER_MAIN_394_112_2_18_1_29_19_851,"DPLAYER_PauseEx, player_Stop failed!")
TRACE_MSG(PLAYER_MAIN_430_112_2_18_1_29_19_852,"DPLAYER_Resume,handle=0x%x")
TRACE_MSG(PLAYER_MAIN_435_112_2_18_1_29_19_853,"DPLAYER_Resume, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_441_112_2_18_1_29_19_854,"DPLAYER_Resume, demux is not open yet")
TRACE_MSG(PLAYER_MAIN_463_112_2_18_1_29_19_855,"DPLAYER_ResumeEx,handle=0x%x")
TRACE_MSG(PLAYER_MAIN_468_112_2_18_1_29_19_856,"DPLAYER_ResumeEx, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_474_112_2_18_1_29_19_857,"DPLAYER_ResumeEx, demux is re_opened")
TRACE_MSG(PLAYER_MAIN_482_112_2_18_1_29_20_858,"DPLAYER_ResumeEx, _player_set_active_obj failed.")
TRACE_MSG(PLAYER_MAIN_494_112_2_18_1_29_20_859,"DPLAYER_ResumeEx, _player_load_submodule failed %d")
TRACE_MSG(PLAYER_MAIN_503_112_2_18_1_29_20_860,"DPLAYER_ResumeEx, failed to create task")
TRACE_MSG(PLAYER_MAIN_538_112_2_18_1_29_20_861,"DPLAYER_ResumeEx, ADD_DEC_SECURITY,argc=%d,argv[0]=0x%x")
TRACE_MSG(PLAYER_MAIN_561_112_2_18_1_29_20_862,"DPLAYER_Pause Fail!")
TRACE_MSG(PLAYER_MAIN_588_112_2_18_1_29_20_863,"DPLAYER_SetPlayRate: rate is %d")
TRACE_MSG(PLAYER_MAIN_593_112_2_18_1_29_20_864,"DPLAYER_SetPlayRate, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_670_112_2_18_1_29_20_865,"DPLAYER_GetLength, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_695_112_2_18_1_29_20_866,"DPLAYER_SetTime(),i_time=%d")
TRACE_MSG(PLAYER_MAIN_700_112_2_18_1_29_20_867,"DPLAYER_SetTime, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_706_112_2_18_1_29_20_868,"DPLAYER_SetTime, demux is not open yet")
TRACE_MSG(PLAYER_MAIN_736_112_2_18_1_29_20_869,"DPLAYER_GetTime, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_756_112_2_18_1_29_20_870,"DPLAYER_UpdateCurrentFrame,in")
TRACE_MSG(PLAYER_MAIN_761_112_2_18_1_29_20_871,"DPLAYER_UpdateCurrentFrame, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_768_112_2_18_1_29_20_872,"DPLAYER_UpdateCurrentFrame, no invalid video frame.")
TRACE_MSG(PLAYER_MAIN_775_112_2_18_1_29_20_873,"DPLAYER_UpdateCurrentFrame,fail,endnotify to stop.")
TRACE_MSG(PLAYER_MAIN_798_112_2_18_1_29_20_874,"DPLAYER_UpdateNextFrame()")
TRACE_MSG(PLAYER_MAIN_803_112_2_18_1_29_20_875,"DPLAYER_UpdateNextFrame, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_821_112_2_18_1_29_20_876,"DPLAYER_UpdateNextFrame,fail,endnotify to stop.")
TRACE_MSG(PLAYER_MAIN_846_112_2_18_1_29_20_877,"DPLAYER_GetMovieInfo,invalidate para,0x%x,0x%x")
TRACE_MSG(PLAYER_MAIN_852_112_2_18_1_29_20_878,"DPLAYER_UpdateNextFrame, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_863_112_2_18_1_29_20_879,"DPLAYER_GetMovieInfo,video:[w,h]=[%d,%d],bitrate=%d,frame_rate=%d;audio:samplerate=%d,bitrate=%d;stream_bitrate=%d")
TRACE_MSG(PLAYER_MAIN_887_112_2_18_1_29_20_880,"DPLAYER_SetFrameEndNotify, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_918_112_2_18_1_29_20_881,"DPLAYER_SetEndNotify, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_952_112_2_18_1_29_21_882,"DPLAYER_GetCurrentFrameData()")
TRACE_MSG(PLAYER_MAIN_957_112_2_18_1_29_21_883,"DPLAYER_GetCurrentFrameData, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_963_112_2_18_1_29_21_884,"DPLAYER_GetCurrentFrameData, no invalid video frame.")
TRACE_MSG(PLAYER_MAIN_970_112_2_18_1_29_21_885,"DPLAYER_GetCurrentFrameData, invalid param.")
TRACE_MSG(PLAYER_MAIN_998_112_2_18_1_29_21_886,"DPLAYER_GetCurrentFrameData,fail, notify to stop.")
TRACE_MSG(PLAYER_MAIN_1041_112_2_18_1_29_21_887,"DPLAYER_GetNextFrameData, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_1046_112_2_18_1_29_21_888,"DPLAYER_GetNextFrameData,pf_frm_rdy_callback=0x%x")
TRACE_MSG(PLAYER_MAIN_1050_112_2_18_1_29_21_889,"DPLAYER_GetNextFrameData,busy,discard next frame")
TRACE_MSG(PLAYER_MAIN_1092_112_2_18_1_29_21_890,"DPLAYER_GetNextFrameData,post process error,notify to stop.")
TRACE_MSG(PLAYER_MAIN_1113_112_2_18_1_29_21_891,"DPLAYER_SetAudioParam,new_dev=%d")
TRACE_MSG(PLAYER_MAIN_1151_112_2_18_1_29_21_892,"DPLAYER_SetDisplayParam,%d,%d;%d,%d;%d")
TRACE_MSG(PLAYER_MAIN_1162_112_2_18_1_29_21_893,"DPLAYER_SetDisplayParam,invalid [w,h]=[%d,%d]")
TRACE_MSG(PLAYER_MAIN_1168_112_2_18_1_29_21_894,"DPLAYER_SetDisplayParam, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_1227_112_2_18_1_29_21_895,"DPLAYER_ForceAVSync, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_1257_112_2_18_1_29_21_896,"DPLAYER_SetPlaybackMode, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_1271_112_2_18_1_29_21_897,"DPLAYER_SetPlaybackMode, invalid playback_mode=%d")
TRACE_MSG(PLAYER_MAIN_1290_112_2_18_1_29_21_898,"DPLAYER_StreamRegistRxDataPath, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_1310_112_2_18_1_29_21_899,"DPLAYER_StreamRxStart, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_1328_112_2_18_1_29_21_900,"DPLAYER_StreamRxStart, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_1360_112_2_18_1_29_21_901,"DPLAYER_ExternalPlugControl, cmd=%d,player_handle=0x%x,argv=0x%x")
TRACE_MSG(PLAYER_MAIN_1365_112_2_18_1_29_21_902,"DPLAYER_ExternalPlugControl, invalid handle=0x%x")
TRACE_MSG(PLAYER_MAIN_1400_112_2_18_1_29_21_903,"DPLAYER_ExternalPlugControl, end,plug_type=0x%x,argv=0x%x")
TRACE_MSG(PLAYER_MAIN_1423_112_2_18_1_29_21_904,"_disp_notify_callback,p_player_ctx is NULL.")
TRACE_MSG(PLAYER_MAIN_1455_112_2_18_1_29_22_905,"_disp_notify_callback,disp fail,endnotify to stop.")
TRACE_MSG(PLAYER_MAIN_1537_112_2_18_1_29_22_906,"_CheckAndUpdateMovieInf, neigther video nor audio is supported!")
TRACE_MSG(PLAYER_MAIN_1559_112_2_18_1_29_22_907,"DPLAYER,_get_video_dec_out_frm, no frame")
TRACE_MSG(PLAYER_MAIN_1692_112_2_18_1_29_22_908,"DPLAYER,_video_postprocess_callback,p_v_frm_pp_out=0x%x")
TRACE_MSG(PLAYER_MAIN_1696_112_2_18_1_29_22_909,"_video_postprocess_callback,p_player_ctx is NULL.")
TRACE_MSG(PLAYER_MAIN_1745_112_2_18_1_29_22_910,"DPLAYER,_set_pp_in_frame_info,unsupport fmt=%d")
TRACE_MSG(PLAYER_MAIN_1759_112_2_18_1_29_22_911,"DPLAYER,_send2_postproc_frm,[%d x %d] - [%d x %d]")
TRACE_MSG(PLAYER_NEW_232_112_2_18_1_29_23_912,"_player_save_open_param,open_type=%d, argc=%d,argv[1]=%d")
TRACE_MSG(PLAYER_NEW_425_112_2_18_1_29_23_913,"_player_free_open_param,open_type=%d")
TRACE_MSG(PLAYER_NEW_504_112_2_18_1_29_23_914,"_player_create_obj, object num =%d")
TRACE_MSG(PLAYER_NEW_559_112_2_18_1_29_23_915,"_player_create_obj,obj list error--000")
TRACE_MSG(PLAYER_NEW_586_112_2_18_1_29_23_916,"_player_create_obj,obj list error---111")
TRACE_MSG(PLAYER_NEW_604_112_2_18_1_29_23_917,"_player_create_obj,ret, p_player_obj =0x%x,handle=0x%x,objs in list=%d")
TRACE_MSG(PLAYER_NEW_621_112_2_18_1_29_24_918,"_player_destroy_obj,obj = 0x%x,obj in list=%d")
TRACE_MSG(PLAYER_NEW_677_112_2_18_1_29_24_919,"_player_destroy_obj,cann't find the matched obj=0x%x")
TRACE_MSG(PLAYER_NEW_800_112_2_18_1_29_24_920,"DPLAYER_Open, from file, argc=%d,argv[1]=%d")
TRACE_MSG(PLAYER_NEW_843_112_2_18_1_29_24_921,"DPLAYER_Open, open_type(%d) is unsupport")
TRACE_MSG(PLAYER_NEW_848_112_2_18_1_29_24_922,"DPLAYER_Open, PNULL == psz_access_type!")
TRACE_MSG(PLAYER_NEW_855_112_2_18_1_29_24_923,"DPLAYER_Open, PNULL == psz_suffix")
TRACE_MSG(PLAYER_NEW_862_112_2_18_1_29_24_924,"DPLAYER_Open, block_Init faild")
TRACE_MSG(PLAYER_NEW_869_112_2_18_1_29_24_925,"DPLAYER_Open, module_LoadAll faild")
TRACE_MSG(PLAYER_NEW_879_112_2_18_1_29_24_926,"DPLAYER_Open, failed to acquire access module:%s")
TRACE_MSG(PLAYER_NEW_886_112_2_18_1_29_24_927,"DPLAYER_Open, invalidate access symbols")
TRACE_MSG(PLAYER_NEW_896_112_2_18_1_29_24_928,"DPLAYER_Open, failed to open the file ")
TRACE_MSG(PLAYER_NEW_912_112_2_18_1_29_24_929,"DPLAYER_Open, get_file_type,=%s, peek_file_header,=%s")
TRACE_MSG(PLAYER_NEW_921_112_2_18_1_29_24_930,"DPLAYER_Open, failed to acquire demux module:%s")
TRACE_MSG(PLAYER_NEW_929_112_2_18_1_29_24_931,"DPLAYER_Open, invalidate demux symbols")
TRACE_MSG(PLAYER_NEW_938_112_2_18_1_29_24_932,"DPLAYER_Open, demux_open, ret=%d")
TRACE_MSG(PLAYER_NEW_971_112_2_18_1_29_24_933,"_player_unload_submodule")
TRACE_MSG(PLAYER_NEW_976_112_2_18_1_29_24_934,"_player_unload_submodule,close demux")
TRACE_MSG(PLAYER_NEW_983_112_2_18_1_29_24_935,"_player_unload_submodule,close stream")
TRACE_MSG(PLAYER_STATE_120_112_2_18_1_29_25_936,"sci player, RunEngine(), current in error state")
TRACE_MSG(PLAYER_STATE_270_112_2_18_1_29_25_937,"mplayer_DoDemux, error = %d")
TRACE_MSG(PLAYER_STATE_468_112_2_18_1_29_25_938,"prepare_play_handle,start_time=%d,setted time=%d")
TRACE_MSG(PLAYER_STATE_721_112_2_18_1_29_26_939,"player_state.c, demux_SetTimeVideo return SCI_ERROR")
TRACE_MSG(PLAYER_STATE_832_112_2_18_1_29_26_940,"player_CreateTask, failed to create event")
TRACE_MSG(PLAYER_STATE_850_112_2_18_1_29_26_941,"player_CreateTask, failed to create demux timer")
TRACE_MSG(PLAYER_STATE_872_112_2_18_1_29_26_942,"player_Open, failed to create task")
TRACE_MSG(PLAYER_STATE_961_112_2_18_1_29_26_943,"player_Play, current not on stop state")
TRACE_MSG(PLAYER_STATE_987_112_2_18_1_29_26_944,"mplayer_Stop, all ready stop")
TRACE_MSG(PLAYER_STATE_1014_112_2_18_1_29_26_945,"player_Pause, not on play state")
TRACE_MSG(PLAYER_STATE_1037_112_2_18_1_29_27_946,"player_Resume, not on pause state")
TRACE_MSG(PLAYER_STATE_1117_112_2_18_1_29_27_947,"player_SetTimetoSyncPoint, i_time=%d, i_destination=%d")
TRACE_MSG(RECORDER_APP_180_112_2_18_1_29_27_948,"%s(Line%d), p_frame == PNULL")
TRACE_MSG(RECORDER_APP_190_112_2_18_1_29_27_949,"_mrecorder_frame_callback,not ready,s_RECORD_State=%d")
TRACE_MSG(RECORDER_APP_222_112_2_18_1_29_27_950,"%s(Line%d), p_frame == PNULL")
TRACE_MSG(RECORDER_APP_230_112_2_18_1_29_27_951,"_mrecorder_frame_callback,not ready,s_RECORD_State=%d")
TRACE_MSG(RECORDER_APP_308_112_2_18_1_29_27_952,"_atv_scan_channel_callback,p_info=null!")
TRACE_MSG(RECORDER_APP_311_112_2_18_1_29_27_953,"_atv_scan_channel_callback:cmd=%d")
TRACE_MSG(RECORDER_APP_476_112_2_18_1_29_28_954,"_camera_param_ctl,cmd=%d,arg=%d,callback=0x%x")
TRACE_MSG(RECORDER_APP_483_112_2_18_1_29_28_955,"RECORDER_APP,DCAMERA_ParamCtrl,return error,")
TRACE_MSG(RECORDER_APP_556_112_2_18_1_29_28_956,"RECORDER_APP,DCAMERA_GetInfo(), returns error")
TRACE_MSG(RECORDER_APP_656_112_2_18_1_29_28_957,"convert_yuv2rgb, set ISP param error!")
TRACE_MSG(RECORDER_APP_660_112_2_18_1_29_28_958,"convert_yuv2rgb blk buffer = 0x%x, size = %d")
TRACE_MSG(RECORDER_APP_688_112_2_18_1_29_28_959,"convert_yuv2rgb, convert fail!")
TRACE_MSG(RECORDER_APP_747_112_2_18_1_29_28_960,"_atv_turnoff_audio_play_path,audio_handle=0x%x")
TRACE_MSG(RECORDER_APP_788_112_2_18_1_29_28_961,"_atv_turnon_audio_play_path,audio_handle=0x%x")
TRACE_MSG(RECORDER_APP_794_112_2_18_1_29_28_962,"_atv_turnon_audio_play_path,play handle is active,0x%x")
TRACE_MSG(RECORDER_APP_813_112_2_18_1_29_29_963,"_atv_turnon_audio_play_path,speaker handle=NULL.")
TRACE_MSG(RECORDER_APP_847_112_2_18_1_29_29_964,"_atv_turnon_audio_play_path,bt handle=NULL.")
TRACE_MSG(RECORDER_APP_879_112_2_18_1_29_29_965,"_atv_turnon_audio_play_path,unsupoort device=%d")
TRACE_MSG(RECORDER_APP_932_112_2_18_1_29_29_966,"DRECORDER_Open, %d")
TRACE_MSG(RECORDER_APP_946_112_2_18_1_29_29_967,"DRECORDER_Open,error,%d")
TRACE_MSG(RECORDER_APP_974_112_2_18_1_29_29_968,"DRECORDER_Open,DCAMERA_Open failed")
TRACE_MSG(RECORDER_APP_993_112_2_18_1_29_29_969,"DRECORDER_Close")
TRACE_MSG(RECORDER_APP_1013_112_2_18_1_29_29_970,"DRECORDER_Close,error,%d")
TRACE_MSG(RECORDER_APP_1053_112_2_18_1_29_29_971,"DRECORDER_SelectSrcDevice,new=%d,old=%d")
TRACE_MSG(RECORDER_APP_1076_112_2_18_1_29_29_972,"DRECORDER_SelectSrcType,error")
TRACE_MSG(RECORDER_APP_1108_112_2_18_1_29_29_973,"DRECORDER_SetVideoDispParam,0x%x")
TRACE_MSG(RECORDER_APP_1135_112_2_18_1_29_29_974,"DRECORDER_ATV_SetAudioOutParam")
TRACE_MSG(RECORDER_APP_1182_112_2_18_1_29_29_975,"DRECORDER_ATV_StartScanChn")
TRACE_MSG(RECORDER_APP_1217_112_2_18_1_29_29_976,"DRECORDER_ATV_StopScanChn")
TRACE_MSG(RECORDER_APP_1249_112_2_18_1_29_29_977,"DRECORDER_StartPreview, %d")
TRACE_MSG(RECORDER_APP_1263_112_2_18_1_29_29_978,"StartPreview,error,%d")
TRACE_MSG(RECORDER_APP_1282_112_2_18_1_29_29_979,"StartPreview,camera error")
TRACE_MSG(RECORDER_APP_1290_112_2_18_1_29_29_980,"DRECORDER_StartPreview,_camera_start_preview error")
TRACE_MSG(RECORDER_APP_1307_112_2_18_1_29_30_981,"DRECORDER_StartPreview,ATV _atv_turnon_audio_play_path error")
TRACE_MSG(RECORDER_APP_1326_112_2_18_1_29_30_982,"DRECORDER_StopPreview")
TRACE_MSG(RECORDER_APP_1341_112_2_18_1_29_30_983,"StopPreview,%d")
TRACE_MSG(RECORDER_APP_1380_112_2_18_1_29_30_984,"DRECORDER_ParamCtrl,cmd=%d,arg=%d,callback=0x%x")
TRACE_MSG(RECORDER_APP_1422_112_2_18_1_29_30_985,"DRECORDER_ParamCtrl,camera error")
TRACE_MSG(RECORDER_APP_1437_112_2_18_1_29_30_986,"DRECORDER_ParamCtrl,_camera_stop_preview error")
TRACE_MSG(RECORDER_APP_1444_112_2_18_1_29_30_987,"DRECORDER_ParamCtrl,_camera_param_ctl error")
TRACE_MSG(RECORDER_APP_1451_112_2_18_1_29_30_988,"DRECORDER_ParamCtrl,_camera_start_preview error")
TRACE_MSG(RECORDER_APP_1457_112_2_18_1_29_30_989,"DRECORDER_ParamCtrl,_atv_turnon_audio_play_path error")
TRACE_MSG(RECORDER_APP_1469_112_2_18_1_29_30_990,"DRECORDER_ParamCtrl,_camera_param_ctl error")
TRACE_MSG(RECORDER_APP_1492_112_2_18_1_29_30_991,"DRECORDER_GetInfo,,cmd=%d")
TRACE_MSG(RECORDER_APP_1534_112_2_18_1_29_30_992,"DRECORDER_GetInfo,camera error")
TRACE_MSG(RECORDER_APP_1565_112_2_18_1_29_30_993,"DRECORDER_RegisterGetLastFrameCallback,callback=0x%x,data_type=%d")
TRACE_MSG(RECORDER_APP_1608_112_2_18_1_29_30_994,"DRECORDER_StartRecord, [%d][%d]")
TRACE_MSG(RECORDER_APP_1615_112_2_18_1_29_30_995,"DRECORDER_StartRecord,Not support file type")
TRACE_MSG(RECORDER_APP_1634_112_2_18_1_29_30_996,"StartRecord,error,%d")
TRACE_MSG(RECORDER_APP_1641_112_2_18_1_29_30_997,"DRECORDER_StartRecord,camera error")
TRACE_MSG(RECORDER_APP_1658_112_2_18_1_29_30_998,"DRECORDER_StartRecord,ATV _camera_start_capture error")
TRACE_MSG(RECORDER_APP_1664_112_2_18_1_29_30_999,"DRECORDER_StartRecord,ATV _atv_turnon_audio_play_path error")
TRACE_MSG(RECORDER_APP_1679_112_2_18_1_29_30_1000,"_REF_MRECORDER_Start() failed,%d")
TRACE_MSG(RECORDER_APP_1701_112_2_18_1_29_30_1001,"_REF_MRECORDER_Start() failed,%d")
TRACE_MSG(RECORDER_APP_1711_112_2_18_1_29_30_1002,"DRECORDER_StartRecord,DC _camera_start_capture error")
TRACE_MSG(RECORDER_APP_1715_112_2_18_1_29_30_1003,"recorder_app.c, _REF_MRECORDER_Start() succ!")
TRACE_MSG(RECORDER_APP_1733_112_2_18_1_29_30_1004,"DRECORDER_Pause")
TRACE_MSG(RECORDER_APP_1800_112_2_18_1_29_30_1005,"DRECORDER_Resume")
TRACE_MSG(RECORDER_APP_1850_112_2_18_1_29_31_1006,"DRECORDER_GetRecordInfo: data_size=%d,time=%d,frame=%d;")
TRACE_MSG(RECORDER_APP_1866_112_2_18_1_29_31_1007,"DRECORDER_StopRecord")
TRACE_MSG(RECORDER_APP_1881_112_2_18_1_29_31_1008,"StopRecord,invalid status")
TRACE_MSG(RECORDER_APP_1907_112_2_18_1_29_31_1009,"DRECORDER_StopRecord,mrecorder_Stop error.")
TRACE_MSG(RECORDER_APP_1985_112_2_18_1_29_31_1010,"Not support recorder audio type,%d")
TRACE_MSG(RECORDER_APP_2015_112_2_18_1_29_31_1011,"Not support recorder video type,%d")
TRACE_MSG(RECORDER_APP_2021_112_2_18_1_29_31_1012,"recorder_app.c, both video and audio do not support!")
TRACE_MSG(RECORDER_APP_2053_112_2_18_1_29_31_1013,"recorder_app.c,mrecorder_Open() error")
TRACE_MSG(RECORDER_APP_2060_112_2_18_1_29_31_1014,"recorder_app.c:set end notify wrong.")
TRACE_MSG(RECORDER_APP_2068_112_2_18_1_29_31_1015,"recorder_app.c:mrecorder_Start wrong.")
TRACE_MSG(RECORDER_APP_2077_112_2_18_1_29_31_1016,"recorder_app.c,error_end:%d")
TRACE_MSG(RECORDER_APP_2097_112_2_18_1_29_31_1017,"_mrecorder_getlastframe_callback,Y/UV=0x[%x,%x],w/h=[%d,%d]")
TRACE_MSG(RECORDER_APP_2129_112_2_18_1_29_31_1018,"_mrecorder_atv_report,report type=%d,chn=%d,valid=%d")
TRACE_MSG(RECORDER_NEW_166_112_2_18_1_29_32_1019,"recorder, error happen when waiting for event")
TRACE_MSG(RECORDER_NEW_244_112_2_18_1_29_32_1020,"failed to stop audio encoding")
TRACE_MSG(RECORDER_NEW_253_112_2_18_1_29_32_1021,"failed to stop video encoding")
TRACE_MSG(RECORDER_NEW_301_112_2_18_1_29_32_1022,"--error:mrecoder_SetVideoQuality, %d")
TRACE_MSG(RECORDER_NEW_319_112_2_18_1_29_32_1023,"mrecorder_init_task, failed to create event")
TRACE_MSG(RECORDER_NEW_334_112_2_18_1_29_32_1024,"mrecorder_init_task, failed to create demux timer")
TRACE_MSG(RECORDER_NEW_353_112_2_18_1_29_32_1025,"mrecorder_init_task, failed to create task")
TRACE_MSG(RECORDER_NEW_375_112_2_18_1_29_32_1026,"mplayer, deactive, terminate the task fail, result[%d]")
TRACE_MSG(RECORDER_NEW_381_112_2_18_1_29_32_1027,"mplayer, deactive, SCI_DeleteThread fail, result[%d]")
TRACE_MSG(RECORDER_NEW_558_112_2_18_1_29_33_1028,"mrecorder_Open")
TRACE_MSG(RECORDER_NEW_605_112_2_18_1_29_33_1029,"_mrecorder_open")
TRACE_MSG(RECORDER_NEW_627_112_2_18_1_29_33_1030,"failed to acquire access module:%s")
TRACE_MSG(RECORDER_NEW_632_112_2_18_1_29_33_1031,"invalidate access symbols")
TRACE_MSG(RECORDER_NEW_649_112_2_18_1_29_33_1032,"failed to open stream")
TRACE_MSG(RECORDER_NEW_657_112_2_18_1_29_33_1033,"failed to acquire mux module:%s")
TRACE_MSG(RECORDER_NEW_662_112_2_18_1_29_33_1034,"invalidate demux symbols")
TRACE_MSG(RECORDER_NEW_668_112_2_18_1_29_33_1035,"failed to open mux module")
TRACE_MSG(RECORDER_NEW_683_112_2_18_1_29_33_1036,"failed to acquire audio encoder:0x%x")
TRACE_MSG(RECORDER_NEW_706_112_2_18_1_29_33_1037,"invalidate audio codec symbols")
TRACE_MSG(RECORDER_NEW_711_112_2_18_1_29_33_1038,"failed to open audio codec")
TRACE_MSG(RECORDER_NEW_729_112_2_18_1_29_33_1039,"failed to acquire video encoder:0x%x")
TRACE_MSG(RECORDER_NEW_750_112_2_18_1_29_33_1040,"invalidate video codec symbols")
TRACE_MSG(RECORDER_NEW_755_112_2_18_1_29_33_1041,"failed to open video codec")
TRACE_MSG(RECORDER_NEW_791_112_2_18_1_29_33_1042,"mrecorder_Close")
TRACE_MSG(RECORDER_NEW_815_112_2_18_1_29_33_1043,"_mrecorder_close")
TRACE_MSG(RECORDER_NEW_862_112_2_18_1_29_34_1044,"mrecorder_Start")
TRACE_MSG(RECORDER_NEW_877_112_2_18_1_29_34_1045,"_mrecorder_start")
TRACE_MSG(RECORDER_NEW_881_112_2_18_1_29_34_1046,"mrecorder_Start(), mux is not open")
TRACE_MSG(RECORDER_NEW_926_112_2_18_1_29_34_1047,"mrecorder_Stop")
TRACE_MSG(RECORDER_NEW_965_112_2_18_1_29_34_1048,"_mrecorder_stop_step1_sync")
TRACE_MSG(RECORDER_NEW_998_112_2_18_1_29_34_1049,"_mrecorder_stop_step2_async")
TRACE_MSG(RECORDER_NEW_1055_112_2_18_1_29_34_1050,"mrecorder_Pause")
TRACE_MSG(RECORDER_NEW_1070_112_2_18_1_29_34_1051,"_mrecorder_pause")
TRACE_MSG(RECORDER_NEW_1093_112_2_18_1_29_34_1052,"mrecorder_Pause")
TRACE_MSG(RECORDER_NEW_1108_112_2_18_1_29_34_1053,"_mrecorder_resume")
TRACE_MSG(RECORDER_NEW_1135_112_2_18_1_29_34_1054,"mrecorder_GetRecordInfo")
TRACE_MSG(RECORDER_NEW_1172_112_2_18_1_29_34_1055,"recoder_new.c, getrecoderInfo error!")
TRACE_MSG(RECORDER_NEW_1442_112_2_18_1_29_35_1056,"_mrecorder_send_cmd,ctl_task hasn't been created yet.")
TRACE_MSG(RECORDER_NEW_1453_112_2_18_1_29_35_1057,"_mrecorder_send_cmd, discard MRECORDER_CTL_TIMER_TRIGGER.")
TRACE_MSG(RECORDER_NEW_1472_112_2_18_1_29_35_1058,"_mrecorder_send_cmd failed,cmd =%d")
TRACE_MSG(VIDEOCODEC_TASK_158_112_2_18_1_29_36_1059,"SendCommand, videocodec task is not exit. cmd =%d")
TRACE_MSG(VIDEOCODEC_TASK_177_112_2_18_1_29_36_1060,"videocodec, SendCommand, SCI_SendSignal failed, cmd=%d")
TRACE_MSG(VIDEOCODEC_TASK_442_112_2_18_1_29_36_1061,"VideoEnc_MP4_H263_Init,pBuffer->size=%d,too small")
TRACE_MSG(VIDEOCODEC_TASK_544_112_2_18_1_29_37_1062,"video_enc:period=%d,dec=[%d,%d,%d,%d],enc=[%d,%d,%d,%d]")
TRACE_MSG(VIDEOCODEC_TASK_682_112_2_18_1_29_37_1063,"VideoEncGetConfig:CAL_VIDEO_TYPE_H264")
TRACE_MSG(VIDEOCODEC_TASK_687_112_2_18_1_29_37_1064,"VideoEncGetConfig:Default")
TRACE_MSG(VIDEOCODEC_TASK_896_112_2_18_1_29_37_1065,"video_dec:period=%d,dec=[%d,%d,%d,%d],enc=[%d,%d,%d,%d]")
TRACE_MSG(VIDEOCODEC_TASK_1156_112_2_18_1_29_38_1066,"VideoDecDecode,pOutFrameYuv =0x(%x,%x,%x)")
TRACE_MSG(VIDEOCODEC_TASK_1236_112_2_18_1_29_38_1067,"VideoCodecTask_Destroy,task_id =%d,event=0x%x")
TRACE_MSG(VT_INTG_593_112_2_18_1_29_40_1068,"_malloc_local_replace_blkbuf,failed")
TRACE_MSG(VT_INTG_778_112_2_18_1_29_40_1069,"VT open vt senor err")
TRACE_MSG(VT_INTG_807_112_2_18_1_29_40_1070,"_check_vt_sensor_id,sensor_id=%d,sensor_num=%d")
TRACE_MSG(VT_INTG_819_112_2_18_1_29_40_1071,"_get_cur_vt_sensor_id=%d")
TRACE_MSG(VT_INTG_843_112_2_18_1_29_40_1072,"_set_cur_vt_sensor_id=%d")
TRACE_MSG(VT_INTG_874_112_2_18_1_29_40_1073,"_get_img_rotate_angle,screen2sensor_angle=%d,img_out_angle=%d")
TRACE_MSG(VT_INTG_901_112_2_18_1_29_40_1074,"--vt_intg,_start_DCamera_preview,lcd:%d,%d,disp_rect=%d,%d,%d,%d")
TRACE_MSG(VT_INTG_956_112_2_18_1_29_40_1075,"--vt_intg,_stop_DCamera_preview")
TRACE_MSG(VT_INTG_977_112_2_18_1_29_40_1076,"_isp_control_init failed")
TRACE_MSG(VT_INTG_991_112_2_18_1_29_40_1077,"_isp_control_release failed")
TRACE_MSG(VT_INTG_1005_112_2_18_1_29_40_1078,"_isp_control_lock failed")
TRACE_MSG(VT_INTG_1019_112_2_18_1_29_40_1079,"_isp_control_unlock failed")
TRACE_MSG(VT_INTG_1034_112_2_18_1_29_40_1080,"--isp_callback_vtcapture,fifo_depth=%d,y/u/v=0x(%x,%x,%x)")
TRACE_MSG(VT_INTG_1054_112_2_18_1_29_40_1081,"----display,ISP callback,p_rgb565_frame=0x%x")
TRACE_MSG(VT_INTG_1084_112_2_18_1_29_41_1082,"--vt_intg,_set_ISP_vt_encode_param,disp_range(x,y;w,h)=(%d,%d;%d,%d)")
TRACE_MSG(VT_INTG_1221_112_2_18_1_29_41_1083,"--vt_intg,_set_ISP_vt_decode_param:range(x,y;w,h)=(%d,%d;%d,%d)")
TRACE_MSG(VT_INTG_1277_112_2_18_1_29_41_1084,"_vtmw, _start_isp_vt")
TRACE_MSG(VT_INTG_1296_112_2_18_1_29_41_1085,"_vtmw, _stop_isp_vt")
TRACE_MSG(VT_INTG_1350_112_2_18_1_29_41_1086,"----display_param--yuv=0x(%x,%x,%x);size(w,h)=(%d,%d);range(x0,y0;w,h)=(%d,%d;%d,%d)")
TRACE_MSG(VT_INTG_1367_112_2_18_1_29_41_1087,"_send_to_disp_remote_frame,is_isp_ready=0x%x,is_remote_disp=%d")
TRACE_MSG(VT_INTG_1416_112_2_18_1_29_41_1088,"_send_to_disp_local_frame,is_isp_ready=0x%x,is_local_disp=%d")
TRACE_MSG(VT_INTG_1452_112_2_18_1_29_41_1089,"_send_to_disp_local_frame,rgb565_convert_time out:%d")
TRACE_MSG(VT_INTG_1456_112_2_18_1_29_41_1090,"_send_to_disp_local_frame,rgb565_convert_time:%d")
TRACE_MSG(VT_INTG_1482_112_2_18_1_29_41_1091,"_send_to_disp_local_frame, set ISP param error!")
TRACE_MSG(VT_INTG_1486_112_2_18_1_29_41_1092,"vt replace scale lcd blk buffer=0x%x,input_addr=0x%x, size=%d")
TRACE_MSG(VT_INTG_1559_112_2_18_1_29_41_1093,"_vtmw_err_notify,%d")
TRACE_MSG(VT_INTG_1610_112_2_18_1_29_42_1094,"_video_stream_frm_malloc,len=%d is too large")
TRACE_MSG(VT_INTG_1666_112_2_18_1_29_42_1095,"_video_stream_frm_free,strm_frm=0x%x not found")
TRACE_MSG(VT_INTG_1749_112_2_18_1_29_42_1096,"DVT_MW_Open")
TRACE_MSG(VT_INTG_1796_112_2_18_1_29_42_1097,"[DVT_MW_Close],status=%d")
TRACE_MSG(VT_INTG_1858_112_2_18_1_29_42_1098,"DVT_MW_SelectSensor")
TRACE_MSG(VT_INTG_1881_112_2_18_1_29_42_1099,"DVT_MW_SelectSensor_Ex")
TRACE_MSG(VT_INTG_1909_112_2_18_1_29_42_1100,"DVT_MW_GetSensorParam")
TRACE_MSG(VT_INTG_1938_112_2_18_1_29_42_1101,"DVT_MW_SetSensorParam")
TRACE_MSG(VT_INTG_2002_112_2_18_1_29_42_1102,"DVT_MW_PreviewStart")
TRACE_MSG(VT_INTG_2028_112_2_18_1_29_42_1103,"DVT_MW_PreviewStop")
TRACE_MSG(VT_INTG_2055_112_2_18_1_29_42_1104,"DVT_MW_SetDisplayArea")
TRACE_MSG(VT_INTG_2083_112_2_18_1_29_43_1105,"DVT_MW_DialStart")
TRACE_MSG(VT_INTG_2107_112_2_18_1_29_43_1106,"DVT_MW_DialStop")
TRACE_MSG(VT_INTG_2132_112_2_18_1_29_43_1107,"DVT_MW_RecordStart")
TRACE_MSG(VT_INTG_2158_112_2_18_1_29_43_1108,"DVT_MW_RecordStop")
TRACE_MSG(VT_INTG_2183_112_2_18_1_29_43_1109,"DVT_MW_ReplaceLocalStart")
TRACE_MSG(VT_INTG_2211_112_2_18_1_29_43_1110,"DVT_MW_ReplaceLocalStop")
TRACE_MSG(VT_INTG_2239_112_2_18_1_29_43_1111,"DVT_MW_EnableMute,en=%d,obj=0x%x")
TRACE_MSG(VT_INTG_2403_112_2_18_1_29_43_1112,"_vt_codec_getvideotype,bit[31:37]=0x%x")
TRACE_MSG(VT_INTG_2543_112_2_18_1_29_43_1113,"[DVT_CODEC_VideoEncInit]")
TRACE_MSG(VT_INTG_2559_112_2_18_1_29_43_1114,"DVT_CODEC_VideoEncInit,p_vol_enc malloc failed.")
TRACE_MSG(VT_INTG_2585_112_2_18_1_29_44_1115,"[DVT_CODEC_VideoEncRelease]")
TRACE_MSG(VT_INTG_2608_112_2_18_1_29_44_1116,"[DVT_CODEC_GetVideoEncAbility]")
TRACE_MSG(VT_INTG_2626_112_2_18_1_29_44_1117,"DVT_CODEC_GetVideoEncCfg")
TRACE_MSG(VT_INTG_2647_112_2_18_1_29_44_1118,"[DVT_CODEC_SetVideoEncCfg],codec_type=%d")
TRACE_MSG(VT_INTG_2651_112_2_18_1_29_44_1119,"[DVT_CODEC_SetVideoEncCfg],release the ENC before reconfiguration.EncStatus=%d")
TRACE_MSG(VT_INTG_2716_112_2_18_1_29_44_1120,"DVT_CODEC_GetVideoEncHeader,0x%x,%d")
TRACE_MSG(VT_INTG_2729_112_2_18_1_29_44_1121,"DVT_CODEC_GetVideoEncHeader,len=%d")
TRACE_MSG(VT_INTG_2747_112_2_18_1_29_44_1122,"DVT_CODEC_GetVideoEncHeader,header=0x[%x,%x,%x,%x]")
TRACE_MSG(VT_INTG_2778_112_2_18_1_29_44_1123,"DVT_CODEC_VideoEncHostDil,IntraFrame=%d,buffer_size=%d,tick(cur-last)=%d,fifo_depth=%d")
TRACE_MSG(VT_INTG_2862_112_2_18_1_29_44_1124,"[DVT_CODEC_VideoDecInit]")
TRACE_MSG(VT_INTG_2875_112_2_18_1_29_44_1125,"DVT_CODEC_VideoDecInit,p_vol_dec malloc failed.")
TRACE_MSG(VT_INTG_2897_112_2_18_1_29_44_1126,"DVT_CODEC_VideoDecRelease")
TRACE_MSG(VT_INTG_2923_112_2_18_1_29_44_1127,"[DVT_CODEC_GetVideoDecAbility]")
TRACE_MSG(VT_INTG_2942_112_2_18_1_29_44_1128,"DVT_CODEC_GetVideoDecConf")
TRACE_MSG(VT_INTG_2959_112_2_18_1_29_44_1129,"DVT_CODEC_SetVideoDecConf")
TRACE_MSG(VT_INTG_2987_112_2_18_1_29_44_1130,"[DVT_CODEC_VideoDecHostDil],pdata=0x%x,len=%d,err=%d")
TRACE_MSG(VT_INTG_3007_112_2_18_1_29_44_1131,"DVT_CODEC_VideoDecHostDil,_vt_codec_getvideotype failed,video_type=%d,vop_type=%d")
TRACE_MSG(VT_INTG_3019_112_2_18_1_29_44_1132,"DVT_CODEC_VideoDecHostDil,_video_stream_frm_malloc failed,depth=%d")
TRACE_MSG(VT_INTG_3030_112_2_18_1_29_44_1133,"DVT_CODEC_VideoDecHostDil,block_New failed,depth=%d")
TRACE_MSG(VT_INTG_3141_112_2_18_1_29_45_1134,"_amr_enc_callback,")
TRACE_MSG(VT_INTG_3188_112_2_18_1_29_45_1135,"_amr_downlink_callback,i_buffer=%d > len_max")
TRACE_MSG(VT_INTG_3204_112_2_18_1_29_45_1136,"_amr_downlink_callback,block_FifoRemove failed.")
TRACE_MSG(VT_INTG_3215_112_2_18_1_29_45_1137,"_amr_downlink_callback,data is not ready.")
TRACE_MSG(VT_INTG_3237_112_2_18_1_29_45_1138,"_amr_downlink_callback,VT_EXT_WRITEVOICEDATA err, ret=%d,%d")
TRACE_MSG(VT_INTG_3284_112_2_18_1_29_45_1139,"_amr_uplink_callback,VT_EXT_READVOICEDATA err, ret=%d,%d")
TRACE_MSG(VT_INTG_3320_112_2_18_1_29_45_1140,"_amr_uplink_callback,block alloc failed.fifo_depth=%d")
TRACE_MSG(VT_INTG_3365_112_2_18_1_29_45_1141,"_pcm_record_callback,VT_EXT_READRECORDDATA err, ret=%d,%d")
TRACE_MSG(VT_INTG_3409_112_2_18_1_29_45_1142,"_vtmw_audio_start")
TRACE_MSG(VT_INTG_3415_112_2_18_1_29_45_1143,"_vtmw_audio_start,duplicate")
TRACE_MSG(VT_INTG_3429_112_2_18_1_29_45_1144,"_vtmw_audio_start,AUDIO_CreateNormalHandle,invalid handle")
TRACE_MSG(VT_INTG_3449_112_2_18_1_29_45_1145,"_vtmw_audio_start,VT_EXT_START err, ret=%d,%d")
TRACE_MSG(VT_INTG_3467_112_2_18_1_29_45_1146,"_vtmw_audio_stop")
TRACE_MSG(VT_INTG_3482_112_2_18_1_29_45_1147,"_vtmw_audio_stop,VT_EXT_STOP err, ret=%d,%d")
TRACE_MSG(VT_INTG_3490_112_2_18_1_29_45_1148,"_vtmw_audio_stop,AUDIO_CloseHandle err,%d")
TRACE_MSG(VT_INTG_3513_112_2_18_1_29_45_1149,"_vtmw_start_record_audio")
TRACE_MSG(VT_INTG_3532_112_2_18_1_29_45_1150,"_vtmw_start_record_audio,VT_EXT_STARTRECORD err, ret=%d,%d")
TRACE_MSG(VT_INTG_3546_112_2_18_1_29_45_1151,"_vtmw_stop_record_audio")
TRACE_MSG(VT_INTG_3560_112_2_18_1_29_46_1152,"_vtmw_stop_record_audio,VT_EXT_STOPRECORD err, ret=%d,%d")
TRACE_MSG(VT_INTG_3600_112_2_18_1_29_46_1153,"_amr_byte2halfword,len_src=%d > 31")
TRACE_MSG(VT_INTG_3656_112_2_18_1_29_46_1154,"_amr_convert_IF2_to_MMS,frame type=%d,size=%d,")
TRACE_MSG(VT_INTG_3660_112_2_18_1_29_46_1155,"_amr_convert_IF2_to_MMS,parametr check failed")
TRACE_MSG(VT_INTG_3729_112_2_18_1_29_46_1156,"_amr_convert_IF2_to_MMS,silence frame")
TRACE_MSG(VT_INTG_3808_112_2_18_1_29_46_1157,"DVT_CODEC_AmrEncOpen")
TRACE_MSG(VT_INTG_3824_112_2_18_1_29_46_1158,"DVT_CODEC_AmrEncClose")
TRACE_MSG(VT_INTG_3840_112_2_18_1_29_46_1159,"DVT_CODEC_AmrEncSetMode")
TRACE_MSG(VT_INTG_3852_112_2_18_1_29_46_1160,"DVT_CODEC_AmrEncFlush")
TRACE_MSG(VT_INTG_3865_112_2_18_1_29_46_1161,"DVT_CODEC_AmrEncStartCapture")
TRACE_MSG(VT_INTG_3910_112_2_18_1_29_46_1162,"DVT_CODEC_AmrEncReadAFrame,i_buffer=%d > buf_len=%d")
TRACE_MSG(VT_INTG_3959_112_2_18_1_29_46_1163,"DVT_CODEC_AmrEncReadAFrame,FifoRemove err.")
TRACE_MSG(VT_INTG_3977_112_2_18_1_29_46_1164,"DVT_CODEC_AmrDecOpen")
TRACE_MSG(VT_INTG_3990_112_2_18_1_29_46_1165,"DVT_CODEC_AmrDecClose")
TRACE_MSG(VT_INTG_4020_112_2_18_1_29_46_1166,"DVT_CODEC_AmrDecWriteAFrame,fifo_depth=%d")
TRACE_MSG(VT_INTG_4070_112_2_18_1_29_47_1167,"DVT_CODEC_AmrDecWriteAFrame,block alloc failed,discard the frame")
TRACE_MSG(VT_INTG_4209_112_2_18_1_29_47_1168,"_vtmw_open,pf_err_notify=0x%x")
TRACE_MSG(VT_INTG_4229_112_2_18_1_29_47_1169,"_vtmw_open,_vtmw_modules_load error")
TRACE_MSG(VT_INTG_4277_112_2_18_1_29_47_1170,"_vtmw_close,status=%d")
TRACE_MSG(VT_INTG_4358_112_2_18_1_29_47_1171,"_vtmw_set_display_area,status=%d,p_remote=0x%x,p_local=0x%x")
TRACE_MSG(VT_INTG_4364_112_2_18_1_29_47_1172,"_vtmw_set_display_area,remote_disp_block=[%d,%d,%d]")
TRACE_MSG(VT_INTG_4370_112_2_18_1_29_47_1173,"_vtmw_set_display_area,local_disp_block=[%d,%d,%d]")
TRACE_MSG(VT_INTG_4517_112_2_18_1_29_47_1174,"_vtmw_set_display_area,exit")
TRACE_MSG(VT_INTG_4525_112_2_18_1_29_47_1175,"_vtmw_preview_start")
TRACE_MSG(VT_INTG_4529_112_2_18_1_29_47_1176,"_vtmw_preview_start,local disp rect is invalid")
TRACE_MSG(VT_INTG_4558_112_2_18_1_29_47_1177,"_vtmw_preview_stop")
TRACE_MSG(VT_INTG_4585_112_2_18_1_29_48_1178,"_vtmw_dial_start")
TRACE_MSG(VT_INTG_4634_112_2_18_1_29_48_1179,"_vtmw_dial_start,exit")
TRACE_MSG(VT_INTG_4643_112_2_18_1_29_48_1180,"_vtmw_dial_stop,status=%d")
TRACE_MSG(VT_INTG_4746_112_2_18_1_29_48_1181,"_vtmw_sensor_para_get,ret=%d,cmd=%d,para=%d")
TRACE_MSG(VT_INTG_4763_112_2_18_1_29_48_1182,"_vtmw_sensor_para_set,invalid param,cmdwparam=%d,cmdlparam=0x%x")
TRACE_MSG(VT_INTG_4769_112_2_18_1_29_48_1183,"_vtmw_sensor_para_set,cmd=%d,para_ptr=0x%x")
TRACE_MSG(VT_INTG_4851_112_2_18_1_29_48_1184,"_vtmw_sensor_select_ex,cmdwparam=%d,cmdlparam=0x%x")
TRACE_MSG(VT_INTG_4862_112_2_18_1_29_48_1185,"_vtmw_sensor_select_ex,id_expect=%d,angle=%d")
TRACE_MSG(VT_INTG_4876_112_2_18_1_29_48_1186,"_vtmw_sensor_select,vt_sensor_id_expect is useless")
TRACE_MSG(VT_INTG_4899_112_2_18_1_29_48_1187,"_vtmw_sensor_select,vt_sensor_id_expect is useless")
TRACE_MSG(VT_INTG_5145_112_2_18_1_29_49_1188,"_rec_audio_send2mux,failed")
TRACE_MSG(VT_INTG_5166_112_2_18_1_29_49_1189,"_rec_vido_send2mux,failed")
TRACE_MSG(VT_INTG_5210_112_2_18_1_29_49_1190,"_vtmw_record_start,p_record_info=0x%x:[%d,%d]")
TRACE_MSG(VT_INTG_5228_112_2_18_1_29_49_1191,"VT,failed to AcquireMuxAccessModule")
TRACE_MSG(VT_INTG_5236_112_2_18_1_29_49_1192,"VT,invalidate access symbols")
TRACE_MSG(VT_INTG_5259_112_2_18_1_29_49_1193,"VT,failed to stream_Open")
TRACE_MSG(VT_INTG_5272_112_2_18_1_29_49_1194,"VT,failed to acquire mux module")
TRACE_MSG(VT_INTG_5281_112_2_18_1_29_49_1195,"VT,invalidate mux symbols")
TRACE_MSG(VT_INTG_5290_112_2_18_1_29_49_1196,"VT,failed to open mux module")
TRACE_MSG(VT_INTG_5316_112_2_18_1_29_49_1197,"vt_intg.c, recorder start, audio111 error!")
TRACE_MSG(VT_INTG_5349_112_2_18_1_29_49_1198,"vt_intg.c, recorder start, audio222 error")
TRACE_MSG(VT_INTG_5386_112_2_18_1_29_49_1199,"vt_intg.c, recorder start, video size error!")
TRACE_MSG(VT_INTG_5404_112_2_18_1_29_49_1200,"vt_intg.c, recorder start, video error!")
TRACE_MSG(VT_INTG_5415_112_2_18_1_29_49_1201,"_vtmw_record_start,no video nor audio")
TRACE_MSG(VT_INTG_5451_112_2_18_1_29_49_1202,"_vtmw_record_start,rec_flag set:0x%x")
TRACE_MSG(VT_INTG_5463_112_2_18_1_29_49_1203,"_vtmw_record_stop")
TRACE_MSG(VT_INTG_5577_112_2_18_1_29_49_1204,"_read_jpeg_file2buf,file open error.")
TRACE_MSG(VT_INTG_5584_112_2_18_1_29_50_1205,"jpg_stream_size=%d,too large file")
TRACE_MSG(VT_INTG_5593_112_2_18_1_29_50_1206,"vt_intg.c, _read_jpeg_file2buf, alloc buffer fail!")
TRACE_MSG(VT_INTG_5604_112_2_18_1_29_50_1207,"vt_intg.c, _read_jpeg_file2buf, read file error!")
TRACE_MSG(VT_INTG_5642_112_2_18_1_29_50_1208,"_check_jpeg_info, invalid JPG")
TRACE_MSG(VT_INTG_5668_112_2_18_1_29_50_1209,"_dec_scale_jpeg,stream=0x%x,len=%d,target size=[%dx%d]")
TRACE_MSG(VT_INTG_5677_112_2_18_1_29_50_1210,"vt_intg.c, _dec_scale_jpeg, alloc common buffer error!")
TRACE_MSG(VT_INTG_5695_112_2_18_1_29_50_1211,"vt_intg.c, _dec_scale_jpeg, alloc yuv buffer error!")
TRACE_MSG(VT_INTG_5721_112_2_18_1_29_50_1212,"vt_intg.c, _dec_scale_jpeg, decode fail")
TRACE_MSG(VT_INTG_5731_112_2_18_1_29_50_1213,"_dec_scale_jpeg,dec out: size=%dx%d,fmt=%d,%d")
TRACE_MSG(VT_INTG_6026_112_2_18_1_29_50_1214,"_convert_yuv_frame,format:0x%x,%d")
TRACE_MSG(VT_INTG_6031_112_2_18_1_29_50_1215,"vt_intg.c, _convert_yuv_frame, width or height is invalid")
TRACE_MSG(VT_INTG_6114_112_2_18_1_29_51_1216,"_vtmw_replace_start,p_replace_info=0x%x:[%d,%d,%d,%d]")
TRACE_MSG(VT_INTG_6194_112_2_18_1_29_51_1217,"_vtmw_replace_start,alloc p_replace_yuv420_buf_local failed")
TRACE_MSG(VT_INTG_6212_112_2_18_1_29_51_1218,"_vtmw_replace_start,alloc p_replace_yuv420_buf_remote failed")
TRACE_MSG(VT_INTG_6250_112_2_18_1_29_51_1219,"_vtmw_replace_start,_convert_yuv_frame failed")
TRACE_MSG(VT_INTG_6355_112_2_18_1_29_51_1220,"_vtmw_replace_stop")
TRACE_MSG(VT_INTG_6432_112_2_18_1_29_51_1221,"_vtmw_enablemute,cmdwparam=%d,cmdlparam=0x%x")
TRACE_MSG(VT_INTG_6480_112_2_18_1_29_51_1222,"_video_enc_save_vol,too large,%d")
TRACE_MSG(VT_INTG_6504_112_2_18_1_29_51_1223,"_video_dec_save_vol,p_vol=0x%x,len=%d,data=0x[%x,%x,%x,%x]")
TRACE_MSG(VT_INTG_6516_112_2_18_1_29_51_1224,"_video_dec_save_vol,too large,%d")
TRACE_MSG(VT_INTG_6545_112_2_18_1_29_52_1225,"_video_decode_one_frame,_vt_codec_getvideotype failed,video_type=%d,vop_type=%d")
TRACE_MSG(VT_INTG_6581_112_2_18_1_29_52_1226,"_video_decode_one_frame,MPEG4 configure,len=%d")
TRACE_MSG(VT_INTG_6594_112_2_18_1_29_52_1227,"failed to call CAL_VideoDecInit()")
TRACE_MSG(VT_INTG_6609_112_2_18_1_29_52_1228,"_video_decode_one_frame,H.263 configure")
TRACE_MSG(VT_INTG_6618_112_2_18_1_29_52_1229,"failed to call CAL_VideoDecInit()")
TRACE_MSG(VT_INTG_6637_112_2_18_1_29_52_1230,"_video_decode_one_frame,discard cur frame,wait for one I-vop,vop_type=%d")
TRACE_MSG(VT_INTG_6685_112_2_18_1_29_52_1231,"_video_decode_one_frame,DecOutput,y_addr =0x%x,err_MB_num=%d")
TRACE_MSG(VT_INTG_6693_112_2_18_1_29_52_1232,"_video_decode_one_frame,DecOutput null y_plane")
TRACE_MSG(VT_INTG_6739_112_2_18_1_29_52_1233,"_video_decode_one_frame,decoder failed.ret=%d")
TRACE_MSG(VT_INTG_6790_112_2_18_1_29_52_1234,"_vtmw_video_enc_one_frame,local video is muted")
TRACE_MSG(VT_INTG_6802_112_2_18_1_29_52_1235,"_vtmw_video_enc_one_frame,enc is not ready yet,%d")
TRACE_MSG(VT_INTG_6808_112_2_18_1_29_52_1236,"_vtmw_video_enc_one_frame,up_fifo is alomost full,%d")
TRACE_MSG(VT_INTG_6816_112_2_18_1_29_52_1237,"_vtmw_video_enc_one_frame,stream_frm_malloc failed")
TRACE_MSG(VT_INTG_6821_112_2_18_1_29_52_1238,"_vtmw_video_enc_one_frame,block_New failed")
TRACE_MSG(VT_INTG_6862_112_2_18_1_29_52_1239,"_vtmw_video_enc_one_frame,block remove failed[%d]")
TRACE_MSG(VT_INTG_6903_112_2_18_1_29_52_1240,"_video_enc_one_frame,cache_len_bit=%d,tx_bit_fract=%d")
TRACE_MSG(VT_INTG_6919_112_2_18_1_29_52_1241,"_vtmw_video_enc_one_frame:ret=%d,enc_size=%d")
TRACE_MSG(VT_INTG_6948_112_2_18_1_29_52_1242,"_vtmw_video_enc_one_frame:buf_size(%d) < enc_size(%d)")
TRACE_MSG(VT_INTG_7084_112_2_18_1_29_53_1243,"_vtmw_timer_trigger_process,adpcm_enc,fifo_depth=%d")
TRACE_MSG(VT_INTG_7128_112_2_18_1_29_53_1244,"_vtmw_timer_trigger_process,mux failed,i_ret[%d]")
TRACE_MSG(VT_INTG_7231_112_2_18_1_29_53_1245,"_vtmw_ctl_send_cmd,ctl_task hasn't been created yet[0x%x].")
TRACE_MSG(VT_INTG_7242_112_2_18_1_29_53_1246,"_vtmw_ctl_send_cmd, discard VTMW_CTL_TIMER_TRIGGER.")
TRACE_MSG(VT_INTG_7261_112_2_18_1_29_53_1247,"_vtmw_ctl_send_cmd failed,cmd =%d")
TRACE_MSG(VT_INTG_7381_112_2_18_1_29_53_1248,"_vtmw_TaskInit,in,task_id =%d")
TRACE_MSG(VT_INTG_7405_112_2_18_1_29_53_1249,"_vtmw_TaskInit,create failed")
TRACE_MSG(VT_INTG_7412_112_2_18_1_29_53_1250,"_vtmw_TaskInit,out,task_id =%d")
TRACE_MSG(VT_INTG_7422_112_2_18_1_29_53_1251,"_vtmw_TaskRelease,in,task_id =%d")
TRACE_MSG(VT_INTG_7542_112_2_18_1_29_53_1252,"_clear_lcd_rect,[%d,%d,%d,%d]")
TRACE_MSG(VT_INTG_7555_112_2_18_1_29_54_1253,"VT_Loop_Err_callback,err type=%d")
TRACE_MSG(VT_INTG_7615_112_2_18_1_29_54_1254,"VT_loop_start_codec,set audio device mode error,%d")
TRACE_MSG(VT_INTG_7653_112_2_18_1_29_54_1255,"VT_Loop_Change_mode,cur_mode=%d")
TRACE_MSG(VT_INTG_7857_112_2_18_1_29_54_1256," VT_Loop_Self_Test_Entry, ret=%d,event=%d")
TRACE_MSG(VT_INTG_7866_112_2_18_1_29_54_1257,"VT_Loop_Self_Test_Entry, receive a start event")
TRACE_MSG(VT_INTG_7880_112_2_18_1_29_54_1258,"VT_Loop_Self_Test_Entry, receive a exit event")
TRACE_MSG(VT_INTG_7913_112_2_18_1_29_54_1259,"VT_Loop_Self_Test_Entry, EVENT_TIME_VT")
TRACE_MSG(VT_INTG_7950_112_2_18_1_29_54_1260,"VT_Loop_Self_Test_Entry,read uplink amr,%d,0x[%x,%x,%x,%x]")
TRACE_MSG(VT_INTG_7975_112_2_18_1_29_54_1261,"VT_Loop_Self_Start")
TRACE_MSG(VT_INTG_7979_112_2_18_1_29_54_1262,"VT_Loop_Self_Start,open duplicate")
TRACE_MSG(VT_INTG_8020_112_2_18_1_29_54_1263,"VT_Loop_Self_Stop")
TRACE_MSG(VT_INTG_8060_112_2_18_1_29_55_1264,"VT_Loop_Self_Change")
END_TRACE_MAP(MS_REF_DV_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_DV_TRC_H_

