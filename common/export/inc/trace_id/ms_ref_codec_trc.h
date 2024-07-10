/******************************************************************************
 ** File Name:      ms_ref_codec_trc.h                                         *
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
//trace_id:41
#ifndef _MS_REF_CODEC_TRC_H_
#define _MS_REF_CODEC_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define AAC_ADP_267_112_2_18_1_8_9_0 "%s, %d. alloc failed! len:%d."
#define AAC_ADP_274_112_2_18_1_8_9_1 "%s, %d. alloc failed! len:%d."
#define AAC_ADP_281_112_2_18_1_8_9_2 "%s, %d. alloc failed! len:%d."
#define AAC_ADP_288_112_2_18_1_8_9_3 "%s, %d. alloc failed! len:%d."
#define AAC_ADP_301_112_2_18_1_8_9_4 "%s, %d. alloc failed! len:%d."
#define AAC_ADP_312_112_2_18_1_8_9_5 "%s, %d. alloc failed! len:%d."
#define AAC_ADP_322_112_2_18_1_8_9_6 "%s, %d. alloc failed! len:%d."
#define AAC_ADP_344_112_2_18_1_8_9_7 "AAC_Construct NOT support stream buffer play in aac codec."
#define AAC_ADP_607_112_2_18_1_8_10_8 "%s, %d. alloc failed! audio handle:0x%08x."
#define AAC_ADP_647_112_2_18_1_8_10_9 "%s, %d. alloc failed! audio handle:0x%08x."
#define AAC_ADP_688_112_2_18_1_8_10_10 "%s, %d. alloc failed! audio handle:0x%08x."
#define AAC_ADP_731_112_2_18_1_8_10_11 "%s, %d. alloc failed! audio handle:0x%08x."
#define AAC_ADP_779_112_2_18_1_8_10_12 "AAC_Seek aac codec can't support seek in streambuffer play."
#define AAC_ADP_787_112_2_18_1_8_10_13 "%s, %d. alloc failed! audio handle:0x%08x."
#define AAC_ADP_826_112_2_18_1_8_10_14 "AAC_GetPlayingInfo, audio_obj->tPlayingInfo.uiTotalDataLength=0."
#define AAC_ADP_895_112_2_18_1_8_10_15 "%s, %d, %d."
#define AAC_ADP_901_112_2_18_1_8_10_16 "%s, %d, %d."
#define AAC_ADP_927_112_2_18_1_8_10_17 "AAC_IdentifyAudioFormat, uiSamplerate;%d, total_time:%d."
#define AAC_ADP_940_112_2_18_1_8_10_18 "%s, %d can't identify audio format in streambuffer_playing."
#define AAC_ADP_1096_112_2_18_1_8_11_19 "%s, %d, get src data error:%d, frame_id:%d."
#define AAC_ADP_1115_112_2_18_1_8_11_20 "%s, %d, decode error:%d, frame_id: %d."
#define AAC_ADP_1164_112_2_18_1_8_11_21 "AAC_SEEK ui_offset(%d)>ui_total_len(%d), aac codec will set ui_offset=ui_total_len."
#define AAC_ADP_1174_112_2_18_1_8_11_22 "AAC_SEEK success! ori_offset:%d, dest_offset:%d."
#define AAC_ADP_1179_112_2_18_1_8_11_23 "AAC_SEEK failed! ori_offset:%d, dest_offset:%d."
#define AAC_ADP_1185_112_2_18_1_8_11_24 "%s, %d get src proc req, audio handle:0x%08x."
#define AAC_DEMUX_INTERFACE_307_112_2_18_1_8_35_25 "aac_demux_interface.c [DEMUX_AAC_Init], allocLen:%d."
#define AAC_DEMUX_INTERFACE_509_112_2_18_1_8_36_26 "ReadAdtsStreamFile byte_read != aac_adts_data_struc_ptr->bytes_consumed, %d, %d"
#define AAC_DEMUX_INTERFACE_980_112_2_18_1_8_37_27 "aac_demux_interface.c [DEMUX_AAC_MemoryAlloc], allocLen:%d."
#define AAC_ADP_249_112_2_18_1_8_45_28 "[AAC_ConfigDec] NULL == codec_ext_res_ptr"
#define AAC_ADP_291_112_2_18_1_8_45_29 "aac_adp.c:AAC_FreeInConstruct:error:SCI_NULL == audio_obj"
#define AAC_ADP_415_112_2_18_1_8_46_30 "AAC_GetDemuxSrcData len of data is zero, %d, %d, %d."
#define AAC_ADP_423_112_2_18_1_8_46_31 "AAC_GetDemuxSrcData len of data: %d, %d, %d."
#define AAC_ADP_469_112_2_18_1_8_46_32 "aac_adp.c:AAC_Construct:error:SCI_NULL == audio_obj"
#define AAC_ADP_481_112_2_18_1_8_46_33 "%s, %d. alloc failed! len:%d."
#define AAC_ADP_486_112_2_18_1_8_46_34 "aac_adp.c [AAC_Construct], allocLen:%d."
#define AAC_ADP_495_112_2_18_1_8_46_35 "%s, %d. alloc failed! len:%d."
#define AAC_ADP_500_112_2_18_1_8_46_36 "aac_adp.c [AAC_Construct], allocLen:%d."
#define AAC_ADP_507_112_2_18_1_8_46_37 "%s, %d. alloc failed! len:%d."
#define AAC_ADP_512_112_2_18_1_8_46_38 "aac_adp.c [AAC_Construct], allocLen:%d."
#define AAC_ADP_519_112_2_18_1_8_46_39 "%s, %d. alloc failed! len:%d."
#define AAC_ADP_524_112_2_18_1_8_46_40 "aac_adp.c [AAC_Construct], allocLen:%d."
#define AAC_ADP_531_112_2_18_1_8_46_41 "%s, %d. alloc failed! len:%d."
#define AAC_ADP_536_112_2_18_1_8_46_42 "aac_adp.c [AAC_Construct], allocLen:%d."
#define AAC_ADP_549_112_2_18_1_8_46_43 "%s, %d. alloc failed! len:%d."
#define AAC_ADP_554_112_2_18_1_8_46_44 "aac_adp.c [AAC_Construct], allocLen:%d."
#define AAC_ADP_565_112_2_18_1_8_46_45 "%s, %d. alloc failed! len:%d."
#define AAC_ADP_574_112_2_18_1_8_46_46 "%s, %d. alloc failed! len:%d."
#define AAC_ADP_598_112_2_18_1_8_46_47 "aac_adp.c:AAC_Construct:error:PNULL!=audio_obj->pGetSrcCallbackFunc"
#define AAC_ADP_609_112_2_18_1_8_46_48 "aac_adp.c:AAC_Construct:error:error: Unknown Handle type!"
#define AAC_ADP_655_112_2_18_1_8_46_49 "aac_adp.c:AAC_Deconstruct:error:SCI_NULL == audio_obj"
#define AAC_ADP_679_112_2_18_1_8_46_50 "aac_adp.c:AAC_Deconstruct:error:||(audio_obj->tCodecProcessResouce.left_exp_data_ptr[0]==PNULL)"
#define AAC_ADP_751_112_2_18_1_8_46_51 "aac_adp.c:AAC_Deconstruct:error:SCI_NULL == audio_obj"
#define AAC_ADP_763_112_2_18_1_8_46_52 "aac_adp.c:AAC_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_ui_chng_freq_req_handler"
#define AAC_ADP_791_112_2_18_1_8_46_53 "aac_adp.c:AAC_PauseDeconstruct:error:SCI_NULL == audio_obj"
#define AAC_ADP_825_112_2_18_1_8_46_54 "aac_adp.c:AAC_ResumeConstruct:error:SCI_NULL == audio_obj"
#define AAC_ADP_837_112_2_18_1_8_46_55 "aac_adp.c:AAC_ResumeConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_ui_chng_freq_req_handler"
#define AAC_ADP_865_112_2_18_1_8_46_56 "aac_adp.c:AAC_StopDeconstruct:error:SCI_NULL == audio_obj"
#define AAC_ADP_900_112_2_18_1_8_46_57 "aac_adp.c:AAC_StopDeconstruct:error:SCI_NULL == audio_obj"
#define AAC_ADP_908_112_2_18_1_8_46_58 "%s, %d. alloc failed! audio handle:0x%08x."
#define AAC_ADP_943_112_2_18_1_8_47_59 "aac_adp.c:AAC_Stop:error:SCI_NULL == audio_obj"
#define AAC_ADP_951_112_2_18_1_8_47_60 "%s, %d. alloc failed! audio handle:0x%08x."
#define AAC_ADP_987_112_2_18_1_8_47_61 "aac_adp.c:AAC_Pause:error:SCI_NULL == audio_obj"
#define AAC_ADP_995_112_2_18_1_8_47_62 "%s, %d. alloc failed! audio handle:0x%08x."
#define AAC_ADP_1027_112_2_18_1_8_47_63 "aac_adp.c:AAC_Resume:error:SCI_NULL == audio_obj"
#define AAC_ADP_1037_112_2_18_1_8_47_64 "%s, %d. alloc failed! audio handle:0x%08x."
#define AAC_ADP_1063_112_2_18_1_8_47_65 "[AAC_TimeSeek] ui_offset=%d, uiTime=%d"
#define AAC_ADP_1098_112_2_18_1_8_47_66 "aac_adp.c:AAC_Seek:error:SCI_NULL == audio_obj"
#define AAC_ADP_1105_112_2_18_1_8_47_67 "AAC_Seek aac codec can't seek, because codec_ext_res_ptr==PNULL."
#define AAC_ADP_1115_112_2_18_1_8_47_68 "[AAC_Seek] seek signal not processed."
#define AAC_ADP_1121_112_2_18_1_8_47_69 "AAC_Seek aac codec can't support seek in neither file_play nor buffer_play. subtype:%d."
#define AAC_ADP_1131_112_2_18_1_8_47_70 "%s, %d. alloc failed! audio handle:0x%08x."
#define AAC_ADP_1165_112_2_18_1_8_47_71 "aac_adp.c:AAC_GetPlayingInfo:error:SCI_NULL == audio_obj"
#define AAC_ADP_1243_112_2_18_1_8_47_72 "aac_adp.c[AAC_GetPlayingInfo] stream, curTime:%d, %d should not less than:%d(=%d+%d)."
#define AAC_ADP_1266_112_2_18_1_8_47_73 "aac_adp.c:AAC_GetOutputSamplerate:error:SCI_NULL == audio_obj"
#define AAC_ADP_1301_112_2_18_1_8_47_74 "aac_adp.c:AAC_IdentifyAudioFormat:error:SCI_NULL == audio_obj"
#define AAC_ADP_1311_112_2_18_1_8_47_75 "aac_adp.c:AAC_IdentifyAudioFormat:error:SCI_NULL == audio_obj"
#define AAC_ADP_1343_112_2_18_1_8_47_76 "AAC_IdentifyAudioFormat format is not surpoort."
#define AAC_ADP_1359_112_2_18_1_8_47_77 "[AAC_CODEC]AAC_DEMUX_InitAACStream: sample_rate=%d,channels=%d,time=%ds,bitrate =%d"
#define AAC_ADP_1364_112_2_18_1_8_47_78 "AAC_IdentifyAudioFormat format is not surpoort."
#define AAC_ADP_1374_112_2_18_1_8_47_79 "AAC_IdentifyAudioFormat format is not surpoort."
#define AAC_ADP_1392_112_2_18_1_8_47_80 "AAC_IdentifyAudioFormat format is not surpoort."
#define AAC_ADP_1404_112_2_18_1_8_47_81 "AAC_IdentifyAudioFormat no enough space to save header data."
#define AAC_ADP_1426_112_2_18_1_8_47_82 "AAC_IdentifyAudioFormat, stream format is not supported."
#define AAC_ADP_1429_112_2_18_1_8_47_83 "%s, %d can't identify audio format in streambuffer_playing."
#define AAC_ADP_1434_112_2_18_1_8_47_84 "aac_adp.c:AAC_IdentifyAudioFormat:error:unknown file handle type!"
#define AAC_ADP_1449_112_2_18_1_8_48_85 "AAC_IdentifyAudioFormat, uiSamplerate;%d, total_time:%d."
#define AAC_ADP_1533_112_2_18_1_8_48_86 "aac_adp.c:AAC_SendSig: error: Unknown Command!"
#define AAC_ADP_1604_112_2_18_1_8_48_87 "AAC_Thread_Entry  audio_obj->tAudioFormatInfo.uiTotalTime = 0."
#define AAC_ADP_1626_112_2_18_1_8_48_88 "aac_adp.c:AAC_PLAY: Unknown Handle type!"
#define AAC_ADP_1674_112_2_18_1_8_48_89 "%s, %d, get src data error:%d, frame_id:%d."
#define AAC_ADP_1680_112_2_18_1_8_48_90 "%s, %d, len of src data is zero, frame_id:%d."
#define AAC_ADP_1740_112_2_18_1_8_48_91 "%s, %d, decode error:%d, frame_id: %d."
#define AAC_ADP_1764_112_2_18_1_8_48_92 "pGetSrcCallbackFunc over 0 == codec_ext_res_ptr->ui_aac_stream_data_len."
#define AAC_ADP_1794_112_2_18_1_8_48_93 "aac_adp.c: AAC_FRAME_DEC: error: AAC_Thread_Entry not support asyn get stream src data."
#define AAC_ADP_1800_112_2_18_1_8_48_94 "pGetSrcCallbackFunc result:AUDIO_STREAM_GET_SRC_ERROR."
#define AAC_ADP_1808_112_2_18_1_8_48_95 "aac_adp.c: AAC_FRAME_DEC: error: AAC_Thread_Entry pGetSrcCallbackFunc result is not invalid."
#define AAC_ADP_1834_112_2_18_1_8_48_96 "%s, %d, \"# decode error:%d, frame_id: %d."
#define AAC_ADP_1843_112_2_18_1_8_48_97 "%s, %d, decode error:%d, frame_id: %d."
#define AAC_ADP_1917_112_2_18_1_8_48_98 "aac_adp.c:AAC_RESUME: error: Unknown Handle type!"
#define AAC_ADP_1923_112_2_18_1_8_48_99 "AAC_Thread_Entry AAC_RESUME AAC_DecInit error."
#define AAC_ADP_1955_112_2_18_1_8_48_100 "AAC_SEEK ui_offset(%d)>ui_total_len(%d), aac codec will set ui_offset=ui_total_len."
#define AAC_ADP_1968_112_2_18_1_8_48_101 "AAC_SEEK success! ori_offset:%d, dest_offset:%d."
#define AAC_ADP_1973_112_2_18_1_8_49_102 "AAC_SEEK failed! ori_offset:%d, dest_offset:%d."
#define AAC_ADP_1979_112_2_18_1_8_49_103 "%s, %d get src proc req, audio handle:0x%08x."
#define AAC_ADP_1986_112_2_18_1_8_49_104 "aac_adp.c:AAC_Thread_Entry: error: Unknown Command!"
#define AAC_DECODER_197_112_2_18_1_9_23_105 "aac_decoder:error:result is %d"
#define AAC_DECODER_354_112_2_18_1_9_23_106 "aac_decoder.c [AAC_MemoryAlloc], allocLen:%d."
#define MAIN_208_112_2_18_1_10_42_107 "audio_type:%d."
#define AAC_INTERFACE_107_112_2_18_1_10_44_108 "AAC_DEMUX_Construct ALLOC SIZE:%d."
#define AAC_INTERFACE_113_112_2_18_1_10_44_109 "aac_interface.c:AAC_DEMUX_Construct:error:PNULL == demux_mem_ptr"
#define AAC_INTERFACE_119_112_2_18_1_10_44_110 "aac_interface.c [AAC_DEMUX_Construct], allocLen:%d."
#define AAC_INTERFACE_136_112_2_18_1_10_44_111 "aac_interface.c:AAC_DEMUX_Deconstruct:error:demux_mem_ptr==NULL"
#define AAC_INTERFACE_177_112_2_18_1_10_44_112 "aac_interface.c:AAC_DEMUX_InitAACStream:error:format_info_ptr==PNULL)||(header_info_ptr==PNULL"
#define AAC_INTERFACE_215_112_2_18_1_10_44_113 "aac_interface.c:AAC_DEMUX_GetFrameData:error:demux_mem_ptr is %d,frame_info_ptr is %d"
#define AAC_INTERFACE_242_112_2_18_1_10_44_114 "aac_interface.c:AAC_DEMUX_Seek:error:demux_mem_ptr==PNULL"
#define AAC_INTERFACE_277_112_2_18_1_10_44_115 "aac_interface.c:AAC_DEMUX_GetCurPos:error:demux_mem_ptr==PNULL"
#define AAC_INTERFACE_440_112_2_18_1_10_44_116 "aac_interface.c:AACLC_InitAACStream:error:tr)||(init_pata_ptr==PNULL)||(id3_stuc_ptr=="
#define AAC_INTERFACE_460_112_2_18_1_10_44_117 "AACLC_InitAACStream read head info error(should be 10)!, %d"
#define AAC_INTERFACE_468_112_2_18_1_10_44_118 "AAC_LC_DEBUG:m4a format"
#define AAC_INTERFACE_480_112_2_18_1_10_44_119 "AAC_LC_DEBUG:aac format"
#define AAC_INTERFACE_496_112_2_18_1_10_44_120 "aac_interface.c:AACLC_InitAACStream:error:PNULL == init_pata_ptr->aac_lc_info.head_data_ptr "
#define AAC_INTERFACE_507_112_2_18_1_10_44_121 "aac_interface.c:AACLC_InitAACStream:error:PNULL == demux_mem_ptr->aac_adts_da"
#define AAC_INTERFACE_520_112_2_18_1_10_44_122 "AACLC_InitAACStream read byte, %d"
#define AAC_INTERFACE_523_112_2_18_1_10_44_123 "AACLC_InitAACStream 1 == adts_m4a_file."
#define AAC_INTERFACE_541_112_2_18_1_10_45_124 "aac_interface.c:AACLC_InitAACStream:error:tag is too large!tagsize=%d \n"
#define AAC_INTERFACE_544_112_2_18_1_10_45_125 "AACLC_InitAACStream 1 == adts_m4a_file ID3."
#define AAC_INTERFACE_572_112_2_18_1_10_45_126 "AACLC_InitAACStream read byte:%d"
#define AAC_INTERFACE_589_112_2_18_1_10_45_127 "AAC_LC_DEBUG:the ADIF format"
#define AAC_INTERFACE_599_112_2_18_1_10_45_128 "AAC_LC_DEBUG:not AAC-LC"
#define AAC_INTERFACE_619_112_2_18_1_10_45_129 "AAC_LC_DEBUG:sr_index > 16"
#define AAC_INTERFACE_637_112_2_18_1_10_45_130 "AAC_LC_DEBUG:sr_index > 16"
#define AAC_INTERFACE_677_112_2_18_1_10_45_131 "AACLC_InitAACStream m4a after AAC_MEMSET."
#define AAC_INTERFACE_682_112_2_18_1_10_45_132 "AACLC_InitAACStream m4a demux_mem_ptr->aaclc_infile_ptr == PNULL. "
#define AAC_INTERFACE_685_112_2_18_1_10_45_133 "AACLC_InitAACStream m4a aaclc_infile_ptr AAC_MALLOC size:%d."
#define AAC_INTERFACE_703_112_2_18_1_10_45_134 "%s, %d"
#define AAC_INTERFACE_724_112_2_18_1_10_45_135 "aac_interface.c:AACLC_InitAACStream:error:0==demux_mem_ptr->m4a_info.total_frame"
#define AAC_INTERFACE_818_112_2_18_1_10_45_136 "aac_lc_Adts_GetData read byte:%d"
#define AAC_INTERFACE_996_112_2_18_1_10_46_137 "aac_interface.c:AACLC_ReadAdtsStreamFile:error:PNULL == demux_mem_ptr->aac_adts_data_struc.s"
#define AAC_INTERFACE_1012_112_2_18_1_10_46_138 "AACLC_ReadAdtsStreamFile read byte:%d"
#define AAC_INTERFACE_1050_112_2_18_1_10_46_139 "aac_interface.c:aac_lc_M4a_GetData:error:em_ptr)||(PNULL == ppucDataAddr)||(PNULL== audio"
#define AAC_INTERFACE_1079_112_2_18_1_10_46_140 "aac_interface.c:aac_lc_M4a_GetData:error:(rel==-1)"
#define AAC_INTERFACE_1097_112_2_18_1_10_46_141 "aac_interface.c:aac_lc_M4a_GetData:error:0 == read_len"
#define AAC_INTERFACE_1106_112_2_18_1_10_46_142 "aac_lc_M4a_GetData frame_id;%d, total_frame:%d."
#define AAC_INTERFACE_1117_112_2_18_1_10_46_143 "aac_interface.c:aac_lc_M4a_GetData:error:0==(*puiDataLength) || 2048<(*puiDataLength) is %d"
#define AAC_INTERFACE_1142_112_2_18_1_10_46_144 "aac_lc_M4a_GetData 0xfff == sync, %d, %d."
#define AAC_INTERFACE_1150_112_2_18_1_10_46_145 "aac_lc_M4a_GetData: %d, %d."
#define AAC_INTERFACE_1170_112_2_18_1_10_46_146 "aac_interface.c:aac_lc_Adts_Seek:error:PNULL == demux_mem_ptr"
#define AAC_INTERFACE_1202_112_2_18_1_10_46_147 "aac_interface.c:aac_lc_m4a_Seek:error:PNULL == demux_mem_ptr"
#define AAC_INTERFACE_1245_112_2_18_1_10_46_148 "aac_interface.c:aac_Adts_GetCurrentPos:error:PNULL == demux_mem_ptr"
#define AAC_INTERFACE_1268_112_2_18_1_10_46_149 "aac_interface.c:aac_m4a_GetCurrentPos:error:PNULL == demux_mem_ptr"
#define AAC_INTERFACE_1544_112_2_18_1_10_47_150 "AACLC_ADTSParseTime read sync last len != 8, %d"
#define AAC_INTERFACE_1653_112_2_18_1_10_47_151 "aac_interface.c:aac_Average_Frame_size:error:header==NULL"
#define AAC_INTERFACE_1901_112_2_18_1_10_47_152 "end of file is %d"
#define AAC_LC_ADP_446_112_2_18_1_10_49_153 "aac_lc_adp.c:AAC_LC_FreeInConstruct:error:NULL==audio_obj_ptr"
#define AAC_LC_ADP_497_112_2_18_1_10_49_154 "aac_lc_adp.c:AAC_LC_FreeInConstruct:error:NULL==audio_obj_ptr"
#define AAC_LC_ADP_543_112_2_18_1_10_49_155 "aac_lc_adp.c:AAC_LC_Construct:error:res_ptr->src_data_ptr == PNULL"
#define AAC_LC_ADP_548_112_2_18_1_10_49_156 "aac_lc_adp.c [AAC_LC_Construct], allocLen:%d."
#define AAC_LC_ADP_558_112_2_18_1_10_49_157 "aac_lc_adp.c [AAC_LC_Construct], allocLen:%d."
#define AAC_LC_ADP_568_112_2_18_1_10_49_158 "aac_lc_adp.c [AAC_LC_Construct], allocLen:%d."
#define AAC_LC_ADP_582_112_2_18_1_10_49_159 "aac_lc_adp.c:AAC_LC_Construct:error:0x%08x,0x%08x,0x%08x,0x%08x,0x%08x"
#define AAC_LC_ADP_614_112_2_18_1_10_49_160 "aac_lc_adp.c:AAC_LC_Construct:error:PNULL == adp_context_ptr"
#define AAC_LC_ADP_620_112_2_18_1_10_49_161 "aac_lc_adp.c [AAC_LC_Construct], allocLen:%d."
#define AAC_LC_ADP_630_112_2_18_1_10_49_162 "aac_lc_adp.c:AAC_LC_Construct:error:E_EXP_LEN < AAC_LC_F"
#define AAC_LC_ADP_647_112_2_18_1_10_49_163 "AAC_LC_Construct SCI_CreateMutex failed!"
#define AAC_LC_ADP_658_112_2_18_1_10_49_164 "aac_lc_adp.c:AAC_LC_Construct:error:PNULL == demux_context_ptr->tAACLCDemuxFrameBuf[i].uc_frame_data_ptr"
#define AAC_LC_ADP_665_112_2_18_1_10_49_165 "aac_lc_adp.c [AAC_LC_Construct], allocLen:%d."
#define AAC_LC_ADP_732_112_2_18_1_10_49_166 "aac_lc_adp.c:AAC_LC_PlayConstruct:error:PNULL == res_ptr->right_exp_data_ptr[0]......"
#define AAC_LC_ADP_748_112_2_18_1_10_49_167 "aac_lc_adp.c:AAC_LC_PlayConstruct:error:PNULL == adp_context_ptr->pusPcmROutBuf......"
#define AAC_LC_ADP_765_112_2_18_1_10_49_168 "aac_lc_adp.c:AAC_LC_PlayConstruct:error:PNULL == demux_context_ptr->tAACLCDemuxFrameBuf[i].uc_frame_data_ptr......"
#define AAC_LC_ADP_779_112_2_18_1_10_49_169 "aac_lc_adp.c:AAC_LC_PlayConstruct:error:AUDIO_DEFAULT_TASK_ID != audio_obj_ptr->tAssiThreadId)......"
#define AAC_LC_ADP_817_112_2_18_1_10_50_170 "[AAC_LC_CODEC]AAC_LC_Play, ThreadID:0x%x,0x%x"
#define AAC_LC_ADP_828_112_2_18_1_10_50_171 "aac_lc_adp.c:AAC_LC_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_ui_chng_freq_req_handler"
#define AAC_LC_ADP_1013_112_2_18_1_10_50_172 "aac_lc_adp.c:AAC_LC_ResumeConstruct:error:0x%08x,0x%08x,0x%08x,0x%08x,0x%08x"
#define AAC_LC_ADP_1044_112_2_18_1_10_50_173 "aac_lc_adp.c:AAC_LC_ResumeConstruct:error:PNULL == adp_context_ptr"
#define AAC_LC_ADP_1067_112_2_18_1_10_50_174 "aac_lc_adp.c:AAC_LC_ResumeConstruct:error:AUDIO_DEFAULT_TASK_ID != audio_obj_ptr->tThreadId"
#define AAC_LC_ADP_1104_112_2_18_1_10_50_175 "[AAC_LC_CODEC]AAC_LC_Play, ThreadID:0x%x,0x%x"
#define AAC_LC_ADP_1142_112_2_18_1_10_50_176 "aac_lc_adp.c:AAC_LC_StopDeconstruct:error:PNULL == adp_context_ptr"
#define AAC_LC_ADP_1253_112_2_18_1_10_50_177 "aac_lc_adp.c:AAC_LC_Play:error:AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId"
#define AAC_LC_ADP_1293_112_2_18_1_10_50_178 "aac_lc_adp.c:AAC_LC_Stop:error:AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId"
#define AAC_LC_ADP_1318_112_2_18_1_10_51_179 "aac_lc_adp.c:AAC_LC_Pause:error:AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId"
#define AAC_LC_ADP_1343_112_2_18_1_10_51_180 "aac_lc_adp.c:AAC_LC_Pause:error:AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId"
#define AAC_LC_ADP_1388_112_2_18_1_10_51_181 "aac_lc_adp.c:AAC_LC_Seek:error:||(NULL==adp_context_ptr )"
#define AAC_LC_ADP_1414_112_2_18_1_10_51_182 "[AAC_LC] SEEK IS NO MEANING FOR THIS TIME"
#define AAC_LC_ADP_1434_112_2_18_1_10_51_183 "[AAC_LC_TimeSeek] PNULL == adp_context_ptr"
#define AAC_LC_ADP_1458_112_2_18_1_10_51_184 "[AAC_TimeSeek] SEEK IS NO MEANING FOR THIS TIME"
#define AAC_LC_ADP_1460_112_2_18_1_10_51_185 "[AAC_TimeSeek] ui_offset=%d, uiTime=%d"
#define AAC_LC_ADP_1608_112_2_18_1_10_51_186 "aac_lc_adp.c[AAC_LC_GetPlayingInfo] stream, curTime:%d, %d should not less than:%d(=%d+%d)."
#define AAC_LC_ADP_1625_112_2_18_1_10_51_187 "aac_lc_adp.c:AAC_LC_SendStreamData:error:AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tAssiThreadId"
#define AAC_LC_ADP_1753_112_2_18_1_10_51_188 "[AAC_LC_PLAY] aac_decoder_init failed!!!sample_rate=%d, result=%d"
#define AAC_LC_ADP_1760_112_2_18_1_10_51_189 "[AAC_LC_PLAY] aac decoder init finished."
#define AAC_LC_ADP_1777_112_2_18_1_10_51_190 "_AAC_LC_ReadDemuxFrame other task has been playing"
#define AAC_LC_ADP_1787_112_2_18_1_10_51_191 "[AAC_LC]PLAY END!"
#define AAC_LC_ADP_1830_112_2_18_1_10_52_192 "[AAC_LC_FRAME_DEC] aac decode failed."
#define AAC_LC_ADP_1847_112_2_18_1_10_52_193 "[AAC_LC]PLAY END!"
#define AAC_LC_ADP_1863_112_2_18_1_10_52_194 "[AAC_LC]PAUSE BUT NOW DEMUX IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN"
#define AAC_LC_ADP_1869_112_2_18_1_10_52_195 "[AAC_LC]PAUSE SUCCESS!"
#define AAC_LC_ADP_1884_112_2_18_1_10_52_196 "[AAC_LC] stop waiting for track buffer empty!"
#define AAC_LC_ADP_1892_112_2_18_1_10_52_197 "[AAC_LC]STOP BUT NOW DEMUX IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN"
#define AAC_LC_ADP_1898_112_2_18_1_10_52_198 "[AAC_LC]STOP SUCCESS!"
#define AAC_LC_ADP_1906_112_2_18_1_10_52_199 "[_AAC_LC_Thread_Entry] Unknown Message!!!"
#define AAC_LC_ADP_1962_112_2_18_1_10_52_200 "[AAC_LC] DECODE INIT"
#define AAC_LC_ADP_1968_112_2_18_1_10_52_201 "[AAC_LC]DSP DECODE INIT DONE"
#define AAC_LC_ADP_1971_112_2_18_1_10_52_202 "[AAC_LC]DSP DECODE INIT DONE,return_result: %d"
#define AAC_LC_ADP_1985_112_2_18_1_10_52_203 "aac_lc_adp.c:AAC_LC_HEADER_IND:error:tAudioFormatInfo.uiSamplerate !=sig_decode_ptr->uiSampleRate"
#define AAC_LC_ADP_2008_112_2_18_1_10_52_204 "[AAC_LC]PLAY END!"
#define AAC_LC_ADP_2037_112_2_18_1_10_52_205 "[AAC_LC]PLAY END!"
#define AAC_LC_ADP_2124_112_2_18_1_10_52_206 "[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:1."
#define AAC_LC_ADP_2130_112_2_18_1_10_52_207 "[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:2."
#define AAC_LC_ADP_2138_112_2_18_1_10_52_208 "[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:default"
#define AAC_LC_ADP_2162_112_2_18_1_10_52_209 "[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:1."
#define AAC_LC_ADP_2168_112_2_18_1_10_52_210 "[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:2."
#define AAC_LC_ADP_2176_112_2_18_1_10_52_211 "[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:default"
#define AAC_LC_ADP_2212_112_2_18_1_10_52_212 "[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:1."
#define AAC_LC_ADP_2218_112_2_18_1_10_52_213 "[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:2."
#define AAC_LC_ADP_2226_112_2_18_1_10_52_214 "[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:default"
#define AAC_LC_ADP_2240_112_2_18_1_10_52_215 "[AAC_LC]PLAY END!"
#define AAC_LC_ADP_2256_112_2_18_1_10_52_216 "[AAC_LC]PAUSE BUT NOW DEMUX IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN"
#define AAC_LC_ADP_2265_112_2_18_1_10_52_217 "[AAC_LC]PAUSE SUCCESS!"
#define AAC_LC_ADP_2273_112_2_18_1_10_52_218 "[AAC_LC]PAUSE BUT NOW DSP IS DECODE, WAIT SOME TIME THEN TRY AGAIN"
#define AAC_LC_ADP_2289_112_2_18_1_10_52_219 "[AAC_LC] stop waiting for track buffer empty!"
#define AAC_LC_ADP_2297_112_2_18_1_10_52_220 "[AAC_LC]STOP BUT NOW DEMUX IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN"
#define AAC_LC_ADP_2306_112_2_18_1_10_52_221 "[AAC_LC]STOP SUCCESS!"
#define AAC_LC_ADP_2314_112_2_18_1_10_52_222 "[AAC_LC]STOP BUT NOW DSP IS DECODE, WAIT SOME TIME THEN TRY AGAIN"
#define AAC_LC_ADP_2384_112_2_18_1_10_53_223 "[AAC_LC]:DEMUX INIT OK!"
#define AAC_LC_ADP_2404_112_2_18_1_10_53_224 "[AAC_LC]:DEMUX Resume INIT OK!"
#define AAC_LC_ADP_2462_112_2_18_1_10_53_225 "[AAC_LC]:DEMUX THREAD RECV A BAD SIGNAL, DISCARD IT"
#define AAC_LC_ADP_2524_112_2_18_1_10_53_226 "aac_lc_adp.c:_AAC_LC_DEMUX_Init:error:PNULL == adp_context_ptr"
#define AAC_LC_ADP_2549_112_2_18_1_10_53_227 "aac_lc_adp.c:_AAC_LC_DEMUX_Init:error:demux_context_ptr->hAACLCDemuxHandle == NULL"
#define AAC_LC_ADP_2613_112_2_18_1_10_53_228 "[AAC_LC_CODEC]AAC_LC_demux_get_info: sample_rate=%d,channels=%d,time=%ds,bitrate =%d"
#define AAC_LC_ADP_2643_112_2_18_1_10_53_229 "aac_lc_adp.c: AUDIO_STREAM_BUFFER_HANDLE demux_init sample rate(%d) is not support, count:%d, index:%d."
#define AAC_LC_ADP_2662_112_2_18_1_10_53_230 "aac_lc_adp.c: AUDIO_STREAM_BUFFER_HANDLE demux header data:%d, %d."
#define AAC_LC_ADP_2668_112_2_18_1_10_53_231 "aac_lc_adp.c: AUDIO_STREAM_BUFFER_HANDLE demux_init failed."
#define AAC_LC_ADP_2692_112_2_18_1_10_53_232 "aac_lc_adp.c:_AAC_LC_DEMUX_Init:error:aac_lc_head_info.uc_head_data_ptr == NULL"
#define AAC_LC_ADP_2756_112_2_18_1_10_53_233 "aac_lc_adp.c:_AAC_LC_ReadDemuxFrame:error:ject_ptr == PNULL)||(aacDemuxOut_ptrptr == PNU"
#define AAC_LC_ADP_2765_112_2_18_1_10_53_234 "aac_lc_adp.c:_AAC_LC_ReadDemuxFrame:error:adp_context_ptr == PNULL"
#define AAC_LC_ADP_2861_112_2_18_1_10_54_235 "aac_lc_adp.c:_AAC_LC_WriteDemuxFrame,get frame len ==0: demux is end"
#define AAC_LC_ADP_2867_112_2_18_1_10_54_236 "aac_lc_adp.c:_AAC_LC_WriteDemuxFrame: demux is end"
#define AAC_LC_ADP_3030_112_2_18_1_10_54_237 "_AAC_LC_ResetDemuxFrames other task has been playing"
#define AAC_LC_ADP_3129_112_2_18_1_10_54_238 "[_AAC_LC_Frame_Ind_Callback] NULL == s_cur_audio_object_ptr"
#define AAC_LC_ADP_3135_112_2_18_1_10_54_239 "[_AAC_LC_Frame_Ind_Callback] stop waiting for track buffer empty"
#define AAC_LC_ADP_3270_112_2_18_1_10_54_240 "[AAC_LC]PLAY END!"
#define AAC_LC_ADP_3296_112_2_18_1_10_55_241 "[AAC_LC]PLAY END!"
#define AAC_LC_ADP_3448_112_2_18_1_10_55_242 "aac_lc_adp.c:_AAC_LC_SendSig:error:PNULL == sig_out_ptr"
#define AAC_LC_ADP_3467_112_2_18_1_10_55_243 "aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_FRAME_DEC:error:uiReceiver == audio_obj_ptr->tThreadId"
#define AAC_LC_ADP_3488_112_2_18_1_10_55_244 "aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_FRAME_IND:error:uiReceiver == audio_obj_ptr->tThreadId"
#define AAC_LC_ADP_3507_112_2_18_1_10_55_245 "aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_DEMUX_FRAME:error:uiReceiver == audio_obj_ptr->tThreadId"
#define AAC_LC_ADP_3527_112_2_18_1_10_55_246 "aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_DEMUX_START:error:uiReceiver != audio_obj_ptr->tAssiThreadId"
#define AAC_LC_ADP_3546_112_2_18_1_10_55_247 "aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_DEMUX_START:error:uiReceiver != audio_obj_ptr->tAssiThreadId"
#define AAC_LC_ADP_3566_112_2_18_1_10_55_248 "aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_DEMUX_RESUME:error:uiReceiver != audio_obj_ptr->tAssiThreadId"
#define AAC_LC_ADP_3586_112_2_18_1_10_55_249 "aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_DEMUX_STOP:error:uiReceiver != audio_obj_ptr->tAssiThreadId"
#define AAC_LC_ADP_3604_112_2_18_1_10_55_250 "aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_PLAY:error:uiReceiver == audio_obj_ptr->tThreadId"
#define AAC_LC_ADP_3626_112_2_18_1_10_55_251 "aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_HEADER_IND:error:uiReceiver == audio_obj_ptr->tThreadId"
#define AAC_LC_ADP_3644_112_2_18_1_10_55_252 "aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_PAUSE:error:uiReceiver == audio_obj_ptr->tThreadId"
#define AAC_LC_ADP_3664_112_2_18_1_10_55_253 "aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_STOP:error:uiReceiver == audio_obj_ptr->tThreadId"
#define AAC_LC_ADP_3684_112_2_18_1_10_55_254 "aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_STOP:error:uiReceiver == audio_obj_ptr->tAssiThreadId"
#define AAC_LC_ADP_3692_112_2_18_1_10_55_255 "[AAC_LC]:ERROR :_AAC_LC_SendSig SEND A UNKOWNED SIG!"
#define MP4ATOM_338_112_2_18_1_10_58_256 "_mp4ff_read_esds, decoderConfig. ALLOC SIZE:%d."
#define MP4ATOM_355_112_2_18_1_10_58_257 "_mp4ff_read_esds _mp4ff_read_data len:%d != %d"
#define MP4ATOM_474_112_2_18_1_10_58_258 "_mp4ff_read_stsc, stsc_first_chunk. ALLOC SIZE:%d."
#define MP4ATOM_484_112_2_18_1_10_58_259 "_mp4ff_read_stsc, stsc_samples_per_chunk. ALLOC SIZE:%d."
#define MP4ATOM_494_112_2_18_1_10_58_260 "_mp4ff_read_stsc,  stsc_sample_desc_index. ALLOC SIZE:%d."
#define MP4ATOM_535_112_2_18_1_10_58_261 "_mp4ff_read_stco f->track_ptr[f->total_tracks - 1]->stco_chunk_offset. ALLOC SIZE:%d."
#define MP4ATOM_569_112_2_18_1_10_58_262 "_mp4ff_read_ctts, ctts_sample_count. ALLOC SIZE:%d."
#define MP4ATOM_576_112_2_18_1_10_58_263 "_mp4ff_read_ctts,  ctts_sample_offset. ALLOC SIZE:%d."
#define MP4ATOM_646_112_2_18_1_10_59_264 "_mp4ff_read_stts, stts_sample_count. ALLOC SIZE:%d."
#define MP4ATOM_652_112_2_18_1_10_59_265 "_mp4ff_read_stts, stts_sample_delta. ALLOC SIZE:%d."
#define MP4FF_269_112_2_18_1_11_0_266 "_mp4ff_track_add f->track_ptr[f->total_tracks - 1] . ALLOC SIZE:%d."
#define MP4FF_519_112_2_18_1_11_0_267 "mp4ff.c:_mp4ff_num_samples:error:0 == read_len"
#define MP4FF_525_112_2_18_1_11_0_268 "_mp4ff_num_samples: chunk_buf size= %d"
#define MP4FF_538_112_2_18_1_11_0_269 "mp4ff.c:_mp4ff_num_samples:error:0 == read_len"
#define MP4FF_544_112_2_18_1_11_1_270 "_mp4ff_num_samples: stream_buf size= %d"
#define MP4FF_559_112_2_18_1_11_1_271 "mp4ff.c:_mp4ff_num_samples:error:0 == read_len"
#define MP4FF_565_112_2_18_1_11_1_272 "_mp4ff_num_samples: audio_buf size= %d"
#define MP4FF_725_112_2_18_1_11_1_273 "_mp4ff_read_sample: IO error, audio_buf size = 0"
#define MP4FF_733_112_2_18_1_11_1_274 "_mp4ff_read_sample: audio_buf size= %d"
#define MP4FF_763_112_2_18_1_11_1_275 "mp4ff.c:_mp4seek_read_sample:error:(result==-1)||(result==0)"
#define MP4FF_776_112_2_18_1_11_1_276 "mp4ff.c:_mp4seek_read_sample:error:result==-1"
#define MP4SAMPLE_123_112_2_18_1_11_1_277 "_mp4ff_sample_to_offset: chunk_buf = %d"
#define MP4SAMPLE_195_112_2_18_1_11_2_278 "mp4sample.c:_mp4ff_audio_frame_size:error:(0 == read_len"
#define MP4SAMPLE_199_112_2_18_1_11_2_279 "_mp4ff_sample_to_offset: stream_buf size= %d"
#define MP4SAMPLE_241_112_2_18_1_11_2_280 "mp4sample.c:_mp4ff_set_sample_position:error:offset==-1"
#define MP4UTIL_35_112_2_18_1_11_2_281 "_mp4ff_read_data: read_len:%d < %d(bytes)."
#define MP4UTIL_70_112_2_18_1_11_3_282 "_mp4ff_read_int64 _mp4ff_read_data len:%d"
#define MP4UTIL_95_112_2_18_1_11_3_283 "_mp4ff_read_data _mp4ff_read_data len:%d"
#define MP4UTIL_114_112_2_18_1_11_3_284 "_mp4ff_read_int24 _mp4ff_read_data len:%d"
#define MP4UTIL_137_112_2_18_1_11_3_285 "_mp4ff_read_int16 _mp4ff_read_data len:%d"
#define AD_OUTPUT_ADP_151_112_2_18_1_11_3_286 "_Ad_Output_OfferAdDevice, error. hAudioHandle:0x%08x."
#define AD_OUTPUT_ADP_235_112_2_18_1_11_4_287 "_Ad_Output_Play is playing."
#define AD_OUTPUT_ADP_259_112_2_18_1_11_4_288 "ad_output_adp.c:_Ad_Output_StartRecord:error:0x%x, 0x%x, 0x%x, 0x%x."
#define AD_OUTPUT_ADP_266_112_2_18_1_11_4_289 "ad_output_adp.c:_Ad_Output_StartRecord:s_record_device error:0x%x."
#define AD_OUTPUT_ADP_276_112_2_18_1_11_4_290 "ad_output_adp.c:_Ad_Output_StartRecord:error:0x%x, 0x%x, 0x%x."
#define AD_OUTPUT_ADP_317_112_2_18_1_11_4_291 "_Ad_Output_Stop is not playing."
#define AD_OUTPUT_ADP_348_112_2_18_1_11_4_292 "ad_output_adp.c:_Ad_Output_IdentifyFormat:error:ptAudioObject == SCI_NULL"
#define AMR_ARM_ADP_110_112_2_18_1_11_4_293 "AMR_Construct in"
#define AMR_ARM_ADP_116_112_2_18_1_11_4_294 "amr_adp.c:AMR_Construct:error:PNULL == ptAudioObject"
#define AMR_ARM_ADP_125_112_2_18_1_11_4_295 "ERROR amr no memory 2."
#define AMR_ARM_ADP_162_112_2_18_1_11_4_296 "AMR_Construct:ptAmrProcRes->frame_buf is NULL"
#define AMR_ARM_ADP_168_112_2_18_1_11_4_297 "AMR_Construct:ptAmrProcRes->demux_handle is NULL"
#define AMR_ARM_ADP_174_112_2_18_1_11_4_298 "AMR_Construct:ptAmrProcRes->demux_sym_handle is NULL"
#define AMR_ARM_ADP_222_112_2_18_1_11_5_299 "amr_adp.c:AMR_Deconstruct in"
#define AMR_ARM_ADP_228_112_2_18_1_11_5_300 "amr_adp.c:AMR_Play:error:PNULL == ptAudioObject"
#define AMR_ARM_ADP_266_112_2_18_1_11_5_301 "AMR_PlayConstruct in"
#define AMR_ARM_ADP_271_112_2_18_1_11_5_302 "amr_adp.c:AMR_Play:error:PNULL == ptAudioObject"
#define AMR_ARM_ADP_279_112_2_18_1_11_5_303 "amr_adp.c:AMR_PlayConstruct:ptAmrProcRes->audio_out is null"
#define AMR_ARM_ADP_296_112_2_18_1_11_5_304 "amr_adp.c:AMR_PlayConstruct:ptAmrProcRes->decode_handle is null"
#define AMR_ARM_ADP_334_112_2_18_1_11_5_305 "amr_adp.c:AMR_Play:error:PNULL == ptAudioObject"
#define AMR_ARM_ADP_375_112_2_18_1_11_5_306 "amr_arm_adp.c:AMR_pause: demux error"
#define AMR_ARM_ADP_381_112_2_18_1_11_5_307 "amr_arm_adp.c:AMR_pause: decode error"
#define AMR_ARM_ADP_410_112_2_18_1_11_5_308 "AMR_Seek in offset is %d"
#define AMR_ARM_ADP_415_112_2_18_1_11_5_309 "amr_adp.c:AMR_Play:error:PNULL == ptAudioObject"
#define AMR_ARM_ADP_467_112_2_18_1_11_5_310 "amr_adp.c:AMR_Play:error:PNULL == ptAudioObject"
#define AMR_ARM_ADP_475_112_2_18_1_11_5_311 "amr_adp.c:sbc_pcm_data is %d"
#define AMR_ARM_ADP_496_112_2_18_1_11_5_312 "peter: amr_adp.c: decode_time is %d"
#define AMR_ARM_ADP_517_112_2_18_1_11_5_313 "amr_adp.c:AMR_GetPlayingInfo:AMR_Demux_GetPlayingPoint result is %d"
#define AMR_ARM_ADP_561_112_2_18_1_11_5_314 "amr_adp.c:AMR_Play in"
#define AMR_ARM_ADP_566_112_2_18_1_11_5_315 "amr_adp.c:AMR_Play:error:PNULL == ptAudioObject"
#define AMR_ARM_ADP_577_112_2_18_1_11_5_316 "amr_adp.c:AMR_Play:ptAmrProcRes->demux_handle is NULL"
#define AMR_ARM_ADP_584_112_2_18_1_11_5_317 "amr_adp.c:AMR_Play:AMR_Demux_start failed %d"
#define AMR_ARM_ADP_598_112_2_18_1_11_5_318 "amr_adp.c:AMR_Play:ptAmrProcRes->decode_handle is NULL"
#define AMR_ARM_ADP_607_112_2_18_1_11_5_319 "amr_adp.c:AMR_Play:AMR_Demux_start failed %d"
#define AMR_ARM_ADP_630_112_2_18_1_11_5_320 "amr_arm_adp.c:AMR_Stop: demux error"
#define AMR_ARM_ADP_636_112_2_18_1_11_5_321 "amr_arm_adp.c:AMR_Stop: decode error"
#define AMR_ARM_ADP_659_112_2_18_1_11_5_322 "amr_adp.c:AMR_IdentifyFormat:error:PNULL == ptAudioObject"
#define AMR_DSP_ADP_110_112_2_18_1_11_6_323 "AMR_Construct in"
#define AMR_DSP_ADP_116_112_2_18_1_11_6_324 "amr_adp.c:AMR_Construct:error:PNULL == ptAudioObject"
#define AMR_DSP_ADP_125_112_2_18_1_11_6_325 "ERROR amr no memory 2."
#define AMR_DSP_ADP_162_112_2_18_1_11_6_326 "AMR_Construct:ptAmrProcRes->frame_buf is NULL"
#define AMR_DSP_ADP_168_112_2_18_1_11_6_327 "AMR_Construct:ptAmrProcRes->demux_handle is NULL"
#define AMR_DSP_ADP_174_112_2_18_1_11_6_328 "AMR_Construct:ptAmrProcRes->demux_sym_handle is NULL"
#define AMR_DSP_ADP_222_112_2_18_1_11_6_329 "amr_adp.c:AMR_Deconstruct in"
#define AMR_DSP_ADP_228_112_2_18_1_11_6_330 "amr_adp.c:AMR_Play:error:PNULL == ptAudioObject"
#define AMR_DSP_ADP_266_112_2_18_1_11_6_331 "AMR_PlayConstruct in"
#define AMR_DSP_ADP_271_112_2_18_1_11_6_332 "amr_adp.c:AMR_Play:error:PNULL == ptAudioObject"
#define AMR_DSP_ADP_279_112_2_18_1_11_6_333 "amr_adp.c:AMR_PlayConstruct:ptAmrProcRes->audio_out is null"
#define AMR_DSP_ADP_296_112_2_18_1_11_6_334 "amr_adp.c:AMR_PlayConstruct:ptAmrProcRes->decode_handle is null"
#define AMR_DSP_ADP_334_112_2_18_1_11_6_335 "amr_adp.c:AMR_Play:error:PNULL == ptAudioObject"
#define AMR_DSP_ADP_375_112_2_18_1_11_6_336 "amr_arm_adp.c:AMR_pause: demux error"
#define AMR_DSP_ADP_381_112_2_18_1_11_6_337 "amr_arm_adp.c:AMR_pause: decode error"
#define AMR_DSP_ADP_410_112_2_18_1_11_7_338 "AMR_Seek in offset is %d"
#define AMR_DSP_ADP_415_112_2_18_1_11_7_339 "amr_adp.c:AMR_Play:error:PNULL == ptAudioObject"
#define AMR_DSP_ADP_467_112_2_18_1_11_7_340 "amr_adp.c:AMR_Play:error:PNULL == ptAudioObject"
#define AMR_DSP_ADP_477_112_2_18_1_11_7_341 "peter: amr_adp.c: decode_time is %d"
#define AMR_DSP_ADP_498_112_2_18_1_11_7_342 "amr_adp.c:AMR_GetPlayingInfo:AMR_Demux_GetPlayingPoint result is %d"
#define AMR_DSP_ADP_541_112_2_18_1_11_7_343 "amr_adp.c:AMR_Play in"
#define AMR_DSP_ADP_546_112_2_18_1_11_7_344 "amr_adp.c:AMR_Play:error:PNULL == ptAudioObject"
#define AMR_DSP_ADP_557_112_2_18_1_11_7_345 "amr_adp.c:AMR_Play:ptAmrProcRes->demux_handle is NULL"
#define AMR_DSP_ADP_564_112_2_18_1_11_7_346 "amr_adp.c:AMR_Play:AMR_Demux_start failed %d"
#define AMR_DSP_ADP_578_112_2_18_1_11_7_347 "amr_adp.c:AMR_Play:ptAmrProcRes->decode_handle is NULL"
#define AMR_DSP_ADP_587_112_2_18_1_11_7_348 "amr_adp.c:AMR_Play:AMR_Demux_start failed %d"
#define AMR_DSP_ADP_610_112_2_18_1_11_7_349 "amr_arm_adp.c:AMR_Stop: demux error"
#define AMR_DSP_ADP_616_112_2_18_1_11_7_350 "amr_arm_adp.c:AMR_Stop: decode error"
#define AMR_DSP_ADP_639_112_2_18_1_11_7_351 "amr_adp.c:AMR_IdentifyFormat:error:PNULL == ptAudioObject"
#define MP3_NEW_ADP_115_112_2_18_1_11_8_352 "MP3_Construct in"
#define MP3_NEW_ADP_121_112_2_18_1_11_8_353 "MP3_adp.c:MP3_Construct:error:PNULL == ptAudioObject"
#define MP3_NEW_ADP_130_112_2_18_1_11_8_354 "ERROR MP3 no memory 2."
#define MP3_NEW_ADP_170_112_2_18_1_11_8_355 "MP3_Construct:ptMP3ProcRes->frame_buf is NULL"
#define MP3_NEW_ADP_176_112_2_18_1_11_8_356 "MP3_Construct:ptMP3ProcRes->demux_handle is NULL"
#define MP3_NEW_ADP_182_112_2_18_1_11_8_357 "MP3_Construct:ptMP3ProcRes->demux_sym_handle  is NULL"
#define MP3_NEW_ADP_229_112_2_18_1_11_8_358 "MP3_adp.c:MP3_Deconstruct in"
#define MP3_NEW_ADP_235_112_2_18_1_11_8_359 "MP3_adp.c:MP3_Play:error:PNULL == ptAudioObject"
#define MP3_NEW_ADP_275_112_2_18_1_11_8_360 "MP3_PlayConstruct in"
#define MP3_NEW_ADP_280_112_2_18_1_11_8_361 "MP3_adp.c:MP3_Play:error:PNULL == ptAudioObject"
#define MP3_NEW_ADP_288_112_2_18_1_11_8_362 "MP3_adp.c:MP3_PlayConstruct:ptMP3ProcRes->audio_out is null"
#define MP3_NEW_ADP_295_112_2_18_1_11_8_363 "MP3_adp.c:MP3_PlayConstruct:ptMP3ProcRes->audio_out is null"
#define MP3_NEW_ADP_304_112_2_18_1_11_8_364 "MP3_adp.c:MP3_PlayConstruct:ptMP3ProcRes->decode_handle is null"
#define MP3_NEW_ADP_341_112_2_18_1_11_8_365 "MP3_adp.c:MP3_Play:error:PNULL == ptAudioObject"
#define MP3_NEW_ADP_383_112_2_18_1_11_8_366 "mp3_new_adp.c: MP3_Pause demux error!"
#define MP3_NEW_ADP_389_112_2_18_1_11_8_367 "mp3_new_adp.c: MP3_Pause decode error!"
#define MP3_NEW_ADP_417_112_2_18_1_11_8_368 "MP3_Seek in offset is %d"
#define MP3_NEW_ADP_422_112_2_18_1_11_8_369 "MP3_adp.c:MP3_Play:error:PNULL == ptAudioObject"
#define MP3_NEW_ADP_474_112_2_18_1_11_8_370 "MP3_adp.c:MP3_Play:error:PNULL == ptAudioObject"
#define MP3_NEW_ADP_499_112_2_18_1_11_8_371 "peter: mp3 decode time is %d"
#define MP3_NEW_ADP_520_112_2_18_1_11_8_372 "MP3_adp.c:MP3_GetPlayingInfo:MP3_Demux_GetPlayingPoint result is %d"
#define MP3_NEW_ADP_564_112_2_18_1_11_9_373 "MP3_adp.c:MP3_Play in"
#define MP3_NEW_ADP_569_112_2_18_1_11_9_374 "MP3_adp.c:MP3_Play:error:PNULL == ptAudioObject"
#define MP3_NEW_ADP_581_112_2_18_1_11_9_375 "MP3_adp.c:MP3_Play:ptMP3ProcRes->demux_handle is null"
#define MP3_NEW_ADP_587_112_2_18_1_11_9_376 "MP3_adp.c:MP3_Play:MP3_Demux_start failed %d"
#define MP3_NEW_ADP_601_112_2_18_1_11_9_377 "MP3_adp.c:MP3_Play:ptMP3ProcRes->decode_handle is null"
#define MP3_NEW_ADP_610_112_2_18_1_11_9_378 "MP3_adp.c:MP3_Play:MP3_Demux_start failed %d"
#define MP3_NEW_ADP_633_112_2_18_1_11_9_379 "mp3_new_adp.c: MP3_stop demux error!"
#define MP3_NEW_ADP_639_112_2_18_1_11_9_380 "mp3_new_adp.c: MP3_stop decode error!"
#define MP3_NEW_ADP_662_112_2_18_1_11_9_381 "MP3_adp.c:MP3_IdentifyFormat:error:PNULL == ptAudioObject"
#define RECORD_ADP_248_112_2_18_1_11_10_382 "RECORD_adp.c:RECORD_Construct:error:PNULL == ptAudioObject"
#define RECORD_ADP_287_112_2_18_1_11_10_383 "RECORD_adp.c:RECORD_Play:error:PNULL == ptAudioObject"
#define RECORD_ADP_310_112_2_18_1_11_10_384 "RECORD_adp.c:RECORD_Play:error:PNULL == ptAudioObject"
#define RECORD_ADP_328_112_2_18_1_11_10_385 "record_adp.c:RECORD_PlayConstruct:dsp_source_open failed"
#define RECORD_ADP_337_112_2_18_1_11_10_386 "record_adp.c:RECORD_PlayConstruct:arm_source_open failed"
#define RECORD_ADP_525_112_2_18_1_11_10_387 "RECORD_adp.c:RECORD_Play:error:PNULL == ptAudioObject"
#define RECORD_ADP_586_112_2_18_1_11_10_388 "RECORD_arm_adp.c:RECORD_Stop: encode error"
#define RECORD_ADP_616_112_2_18_1_11_10_389 "RECORD_adp.c:RECORD_IdentifyFormat:error:PNULL == ptAudioObject"
#define RECORD_ADP_635_112_2_18_1_11_10_390 "RECORD_adp.c:RECORD_Play:error:PNULL == ptAudioObject"
#define RECORD_ADP_660_112_2_18_1_11_11_391 "RECORD_arm_adp.c:RECORD_pause: encode error"
#define RECORD_ADP_670_112_2_18_1_11_11_392 "RECORD_arm_adp.c:RECORD_pause: encode error"
#define RECORD_ADP_692_112_2_18_1_11_11_393 "RECORD_arm_adp.c:RECORD_Resume: aud_mux_start:encode error"
#define RECORD_ADP_702_112_2_18_1_11_11_394 "record_adp.c:RECORD_Resume:aud_encode_start: result is %d"
#define RECORD_ADP_745_112_2_18_1_11_11_395 "record_adp.c:Record_GetRecordTime:error:ME_COUNT)||(argv==SCI_NULL"
#define RECORD_ADP_782_112_2_18_1_11_11_396 "adpcmrecord.c:Record_SetParam:error:SET_COUNT)||(argv==SCI_NULL"
#define RECORD_ADP_797_112_2_18_1_11_11_397 "adpcmrecord.c:Record_SetParam:error:0x%x, 0x%x, 0x%x, 0x%x, 0x%x"
#define RECORD_ADP_869_112_2_18_1_11_11_398 "adpcmrecord.c:Record_GetVoiceDB:error:ME_COUNT)||(argv==SCI_NULL"
#define RECORD_ADP_897_112_2_18_1_11_11_399 "adpcmrecord.c:Record_GetVoiceDB:error:ME_COUNT)||(argv==SCI_NULL"
#define RECORD_ADP_925_112_2_18_1_11_11_400 "record_adp.c:[Record_SetEndTime] INPUT PARAM ERROR!"
#define RECORD_ADP_955_112_2_18_1_11_11_401 "[Record_SetEndSize] INPUT PARAM ERROR!"
#define RECORD_ADP_992_112_2_18_1_11_11_402 "[Record_SetCBTime] INPUT PARAM ERROR!"
#define RECORD_ADP_1021_112_2_18_1_11_11_403 "[Record_GetCurFileSize] INPUT PARAM ERROR!"
#define RECORD_ADP_1055_112_2_18_1_11_11_404 "adpcmrecord.c:Record_SetIfFromNet:error:ERROR_COUNT)||(argv==SCI_NULL"
#define AMR_ADP_281_112_2_18_1_11_12_405 "amr_adp.c:AMR_Construct:error:PNULL == ptAudioObject"
#define AMR_ADP_290_112_2_18_1_11_12_406 "ERROR amr no memory 1."
#define AMR_ADP_295_112_2_18_1_11_12_407 "amr_adp.c AMR_Construct allocLen:%d."
#define AMR_ADP_301_112_2_18_1_11_12_408 "ERROR amr no memory 2."
#define AMR_ADP_307_112_2_18_1_11_12_409 "amr_adp.c AMR_Construct allocLen:%d."
#define AMR_ADP_348_112_2_18_1_11_12_410 "amr_adp.c:AMR_Deconstruct:error:PNULL == ptAudioObject"
#define AMR_ADP_415_112_2_18_1_11_13_411 "amr_adp.c:AMR_PlayConstruct:error:PNULL == ptAudioObject"
#define AMR_ADP_436_112_2_18_1_11_13_412 "amr_adp.c:AMR_ResumeConstruct:error:PNULL == ptAudioObject"
#define AMR_ADP_468_112_2_18_1_11_13_413 "amr_adp.c:AMR_PauseDeconstruct:error:PNULL == ptAudioObject"
#define AMR_ADP_501_112_2_18_1_11_13_414 "amr_adp.c:AMR_StopDeconstruct:error:PNULL == ptAudioObject"
#define AMR_ADP_536_112_2_18_1_11_13_415 "amr_adp.c:AMR_Play:error:PNULL == ptAudioObject"
#define AMR_ADP_544_112_2_18_1_11_13_416 "ERROR amr no memory 3."
#define AMR_ADP_570_112_2_18_1_11_13_417 "amr_adp.c:AMR_Stop:error:PNULL == ptAudioObject"
#define AMR_ADP_580_112_2_18_1_11_13_418 "ERROR amr no memory 4."
#define AMR_ADP_608_112_2_18_1_11_13_419 "amr_adp.c:AMR_Pause:error:PNULL == ptAudioObject"
#define AMR_ADP_618_112_2_18_1_11_13_420 "ERROR amr no memory 5."
#define AMR_ADP_645_112_2_18_1_11_13_421 "amr_adp.c:AMR_Resume:error:PNULL == ptAudioObject"
#define AMR_ADP_653_112_2_18_1_11_13_422 "ERROR amr no memory 6."
#define AMR_ADP_680_112_2_18_1_11_13_423 "amr_adp.c:AMR_Seek:error:PNULL == ptAudioObject"
#define AMR_ADP_688_112_2_18_1_11_13_424 "AMR_Seek ERROR not support seek operation in this play type:%d."
#define AMR_ADP_696_112_2_18_1_11_13_425 "ERROR amr no memory 7."
#define AMR_ADP_722_112_2_18_1_11_13_426 "AMR_SendStreamData send stream data to codec task!"
#define AMR_ADP_729_112_2_18_1_11_13_427 "AMR_SendStreamData:error:SCI_NULL == ptAudioObj"
#define AMR_ADP_735_112_2_18_1_11_13_428 "AMR_SendStreamData AUDIO_DEFAULT_TASK_ID==ptAudioObj->tThreadId."
#define AMR_ADP_743_112_2_18_1_11_13_429 "AMR_SendStreamData ERROR amr no memory."
#define AMR_ADP_767_112_2_18_1_11_13_430 "amr_adp.c:AMR_GetPlayingInfo:error:PNULL == ptAudioObject"
#define AMR_ADP_812_112_2_18_1_11_13_431 "amr_adp.c[AMR_GetPlayingInfo] stream, curTime:%d, %d should not less than:%d(=%d+%d)."
#define AMR_ADP_836_112_2_18_1_11_13_432 "amr_adp.c:AMR_IdentifyFormat:error:PNULL == ptAudioObject"
#define AMR_ADP_849_112_2_18_1_11_14_433 "AMR: SFS_Seek ERROR"
#define AMR_ADP_860_112_2_18_1_11_14_434 "AMR: SFS_Read ERROR"
#define AMR_ADP_871_112_2_18_1_11_14_435 "AMR: header data error."
#define AMR_ADP_885_112_2_18_1_11_14_436 "AMR: header data error."
#define AMR_ADP_897_112_2_18_1_11_14_437 "amr_adp.c:AMR_IdentifyFormat:error:Unknown Handle type!"
#define AMR_ADP_910_112_2_18_1_11_14_438 "AMR: Header ERROR"
#define AMR_ADP_926_112_2_18_1_11_14_439 "AMR_IdentifyFormat total time:%d s"
#define AMR_ADP_996_112_2_18_1_11_14_440 "AMR_Thread_Entry AMR_PLAY!"
#define AMR_ADP_1051_112_2_18_1_11_14_441 "AMR_Thread_Entry AMR_PAUSE!"
#define AMR_ADP_1059_112_2_18_1_11_14_442 "AMR_Thread_Entry AMR_RESUME!"
#define AMR_ADP_1077_112_2_18_1_11_14_443 "AMR_Thread_Entry AMR_DECODE!"
#define AMR_ADP_1101_112_2_18_1_11_14_444 "AMR_Thread_Entry stream:End of music!"
#define AMR_ADP_1108_112_2_18_1_11_14_445 "AMR_Thread_Entry wrong frame length. %d."
#define AMR_ADP_1112_112_2_18_1_11_14_446 "AMR_Thread_Entry wrong frame. length:%d, should be: %d."
#define AMR_ADP_1125_112_2_18_1_11_14_447 "AMR_Thread_Entry get_stream_data return AUDIO_STREAM_GET_SRC_WAIT!"
#define AMR_ADP_1131_112_2_18_1_11_14_448 "AMR_Thread_Entry get_stream_data return AUDIO_STREAM_GET_SRC_ASY!"
#define AMR_ADP_1137_112_2_18_1_11_14_449 "AMR_Thread_Entry get_stream_data return AUDIO_STREAM_GET_SRC_ERROR!"
#define AMR_ADP_1143_112_2_18_1_11_14_450 "AMR_Thread_Entry get_stream_data return result:%d!wrong type!"
#define AMR_ADP_1158_112_2_18_1_11_14_451 "AMR: stream buffer count > %d!"
#define AMR_ADP_1206_112_2_18_1_11_14_452 "AMR: Buffer > %d!"
#define AMR_ADP_1236_112_2_18_1_11_14_453 "AMR: while_count: %d"
#define AMR_ADP_1258_112_2_18_1_11_14_454 "AMR_Thread_Entry AMR_SENDSTREAMDATA!"
#define AMR_ADP_1262_112_2_18_1_11_14_455 "AMR_Thread_Entry wrong frame length. %d."
#define AMR_ADP_1266_112_2_18_1_11_14_456 "AMR_Thread_Entry wrong frame. length:%d, should be: %d."
#define AMR_ADP_1285_112_2_18_1_11_14_457 "AMR_Thread_Entry AMR_STOP!"
#define AMR_ADP_1295_112_2_18_1_11_14_458 "AMR_Thread_Entry AMR_SEEK result:%d, offset:%d!"
#define AMR_ADP_1335_112_2_18_1_11_14_459 "amr_adp.c:AMR_SetPlayPoint:error:PNULL == ptAudioObject"
#define AMR_ADP_1356_112_2_18_1_11_14_460 "AMR: SFS_Seek ERROR"
#define AMR_ADP_1372_112_2_18_1_11_15_461 "AMR: SFS_Seek ERROR if uiFirstFrameLength==0"
#define AMR_ADP_1382_112_2_18_1_11_15_462 "AMR: SFS_ReadFile ERROR if uiFirstFrameLength==0"
#define AMR_ADP_1395_112_2_18_1_11_15_463 "AMR: SFS_Seek ERROR"
#define AMR_ADP_1405_112_2_18_1_11_15_464 "AMR: SFS_ReadFile ERROR"
#define AMR_ADP_1411_112_2_18_1_11_15_465 "AMR:  bVbr is SCI_TRUE"
#define AMR_ADP_1419_112_2_18_1_11_15_466 "AMR: SFS_Seek ERROR again!"
#define AMR_ADP_1481_112_2_18_1_11_15_467 "amr_adp.c:AMR_FileParseOneFrame:error:PNULL == ptAudioObject"
#define AMR_ADP_1494_112_2_18_1_11_15_468 "AMR_FileParseOneFrameLength SFS_GetFilePointer ERROR."
#define AMR_ADP_1504_112_2_18_1_11_15_469 "AMR_FileParseOneFrameLength SFS_ReadFile ERROR."
#define AMR_ADP_1510_112_2_18_1_11_15_470 "AMR_FileParseOneFrameLength SFS_ReadFile frame_header %d, %d."
#define AMR_ADP_1518_112_2_18_1_11_15_471 "AMR_FileParseOneFrameLength SFS_SetFilePointer ERROR."
#define AMR_ADP_1550_112_2_18_1_11_15_472 "amr_adp.c:AMR_FileParseOneFrame:error:PNULL == ptAudioObject"
#define AMR_ADP_1564_112_2_18_1_11_15_473 "AMR_FileParseOneFrame SFS_ReadFile ERROR."
#define AMR_ADP_1574_112_2_18_1_11_15_474 "AMR_FileParseOneFrame SFS_ReadFile ERROR."
#define AMR_DSP_ADP_463_112_2_18_1_11_17_475 "_AMR_DSP_DecOutputBuf AUDIO_DP_WriteRingBuffer l:0x%x, r:0x%x, len:%d(word)."
#define AMR_DSP_ADP_547_112_2_18_1_11_17_476 "AMR_DSP_DECODE_FRAME SENDER:0x%x receiver:0x%x."
#define AMR_DSP_ADP_665_112_2_18_1_11_17_477 "_AMR_DSP_SendSig: SEND A UNKOWNED SIG:%d."
#define AMR_DSP_ADP_717_112_2_18_1_11_17_478 "_AMR_DSP_GetFirstFrameLength: SFS_Seek ERROR"
#define AMR_DSP_ADP_728_112_2_18_1_11_17_479 "_AMR_DSP_GetFirstFrameLength: SFS_ReadFile ERROR"
#define AMR_DSP_ADP_735_112_2_18_1_11_17_480 "_AMR_DSP_GetFirstFrameLength: SFS_ReadFile len error:%d, %d."
#define AMR_DSP_ADP_743_112_2_18_1_11_17_481 "_AMR_DSP_GetFirstFrameLength: src data len is %d."
#define AMR_DSP_ADP_779_112_2_18_1_11_17_482 "_AMR_DSP_SetPlayPoint failed to seek in vbr."
#define AMR_DSP_ADP_796_112_2_18_1_11_17_483 "AMR: SFS_Seek ERROR"
#define AMR_DSP_ADP_803_112_2_18_1_11_17_484 "_AMR_DSP_SetPlayPoint failed to seek offset:%d, total:%d."
#define AMR_DSP_ADP_819_112_2_18_1_11_17_485 "_AMR_DSP_SetPlayPoint SFS_GetFilePointer: ERROR"
#define AMR_DSP_ADP_828_112_2_18_1_11_17_486 "_AMR_DSP_SetPlayPoint SFS_SetFilePointer: SFS_Seek ERROR"
#define AMR_DSP_ADP_839_112_2_18_1_11_17_487 "_AMR_DSP_SetPlayPoint SFS_ReadFile"
#define AMR_DSP_ADP_846_112_2_18_1_11_18_488 "_AMR_DSP_SetPlayPoint vbr."
#define AMR_DSP_ADP_852_112_2_18_1_11_18_489 "_AMR_DSP_SetPlayPoint SFS_SetFilePointer: SFS_Seek ERROR"
#define AMR_DSP_ADP_863_112_2_18_1_11_18_490 "_AMR_DSP_SetPlayPoint SFS_SetFilePointer: SFS_Seek ERROR"
#define AMR_DSP_ADP_896_112_2_18_1_11_18_491 "_AMR_DSP_SetPlayPoint vbr."
#define AMR_DSP_ADP_978_112_2_18_1_11_18_492 "_AMR_DSP_GetStreamDemuxSrcData stream:End of music!"
#define AMR_DSP_ADP_993_112_2_18_1_11_18_493 "amr_dsp_adp.c GetStreamDemuxSrcData wait!"
#define AMR_DSP_ADP_999_112_2_18_1_11_18_494 "amr_dsp_adp.c GetStreamDemuxSrcData _ASY!"
#define AMR_DSP_ADP_1004_112_2_18_1_11_18_495 "amr_dsp_adp.c GetStreamDemuxSrcData ERROR!"
#define AMR_DSP_ADP_1009_112_2_18_1_11_18_496 "amr_dsp_adp.c get_stream_data return result:%d!wrong type!"
#define AMR_DSP_ADP_1154_112_2_18_1_11_18_497 "_AMR_DSP_WriteDemuxFrame SFS_ReadFile ERROR."
#define AMR_DSP_ADP_1266_112_2_18_1_11_18_498 "_AMR_DSP_WriteDemuxFrame some unnormal 1."
#define AMR_DSP_ADP_1279_112_2_18_1_11_18_499 "_AMR_DSP_WriteDemuxFrame some unnormal 2."
#define AMR_DSP_ADP_1423_112_2_18_1_11_19_500 "SET bIsDspReq FALSE 0"
#define AMR_DSP_ADP_1448_112_2_18_1_11_19_501 "_AMR_DSP_Decode_Trig_By_SBC_Callback isStop is TRUE."
#define AMR_DSP_ADP_1455_112_2_18_1_11_19_502 "_AMR_DSP_Decode_Trig_By_SBC_Callback eDecodeTaskState:%d."
#define AMR_DSP_ADP_1461_112_2_18_1_11_19_503 "SET bIsDspReq FALSE 1"
#define AMR_DSP_ADP_1514_112_2_18_1_11_19_504 "SET bIsDspReq FALSE 2"
#define AMR_DSP_ADP_1570_112_2_18_1_11_19_505 "SET bIsDspReq TRUE 4"
#define AMR_DSP_ADP_1602_112_2_18_1_11_19_506 "SET bIsDspReq TRUE 5"
#define AMR_DSP_ADP_1701_112_2_18_1_11_19_507 "_AMR_DSP_TransFrame bIsDspReq is true, unnormal."
#define AMR_DSP_ADP_1705_112_2_18_1_11_19_508 "SET bIsDspReq TRUE 6"
#define AMR_DSP_ADP_1707_112_2_18_1_11_19_509 "_AMR_DSP_TransFrame start decode time:0x%x."
#define AMR_DSP_ADP_1732_112_2_18_1_11_19_510 "_AMR_DSP_DecodeFrame decode end!"
#define AMR_DSP_ADP_1736_112_2_18_1_11_19_511 "_AMR_DSP_DecodeFrame curFrameCount:%d."
#define AMR_DSP_ADP_1772_112_2_18_1_11_19_512 "_AMR_DSP_DecodeFrame _AMR_DSP_TransFrame frameCount:%d."
#define AMR_DSP_ADP_1814_112_2_18_1_11_19_513 "_AMR_DSP_DecodeFrame _AMR_DSP_SetFrameData frameCount:%d."
#define AMR_DSP_ADP_1845_112_2_18_1_11_19_514 "_AMR_DSP_DecodeFrame _AMR_DSP_TransFrame frameCount:%d, offset:%d."
#define AMR_DSP_ADP_1888_112_2_18_1_11_20_515 "amr_dsp_adp.c AMR_DSP_Construct allocLen:%d*4=%d."
#define AMR_DSP_ADP_1903_112_2_18_1_11_20_516 "amr_dsp_adp.c AMR_DSP_Construct allocLen:%d."
#define AMR_DSP_ADP_1909_112_2_18_1_11_20_517 "amr_dsp_adp.c AMR_DSP_Construct allocLen:%d."
#define AMR_DSP_ADP_1915_112_2_18_1_11_20_518 "pusPcmDataL:0x%x, pusPcmDataR:0x%x, len:%d(bytes)."
#define AMR_DSP_ADP_1921_112_2_18_1_11_20_519 "AMR_DSP_Construct alloc pusPcmData failed. size:%d."
#define AMR_DSP_ADP_1934_112_2_18_1_11_20_520 "amr_dsp_adp.c AMR_DSP_Construct allocLen:%d."
#define AMR_DSP_ADP_1945_112_2_18_1_11_20_521 "AMR_DSP_Construct alloc pucDemuxSrcData failed. size:%d."
#define AMR_DSP_ADP_1965_112_2_18_1_11_20_522 "AMR_DSP_Construct alloc other_info_ptr failed. size:%d."
#define AMR_DSP_ADP_2004_112_2_18_1_11_20_523 "[AMR_DSP_CODEC]AMR_DSP_Play, ThreadID:0x%x,0x%x"
#define AMR_DSP_ADP_2409_112_2_18_1_11_21_524 "AMR_DSP_Seek ERROR not support seek operation in this play type:%d."
#define AMR_DSP_ADP_2447_112_2_18_1_11_21_525 "AMR_DSP_IdentifyAudioFormat: SFS_Seek ERROR"
#define AMR_DSP_ADP_2459_112_2_18_1_11_21_526 "AMR_DSP_IdentifyAudioFormat: SFS_Read ERROR"
#define AMR_DSP_ADP_2465_112_2_18_1_11_21_527 "AMR_DSP_IdentifyAudioFormat: header data error."
#define AMR_DSP_ADP_2476_112_2_18_1_11_21_528 "AMR_DSP_IdentifyAudioFormat: header data error."
#define AMR_DSP_ADP_2490_112_2_18_1_11_21_529 "AMR_DSP_IdentifyAudioFormat: header data error."
#define AMR_DSP_ADP_2513_112_2_18_1_11_21_530 "AMR_DSP_IdentifyAudioFormat: Header ERROR"
#define AMR_DSP_ADP_2529_112_2_18_1_11_21_531 "AMR_DSP_IdentifyAudioFormat FirstFrameLen is zero."
#define AMR_DSP_ADP_2535_112_2_18_1_11_21_532 "AMR_IdentifyFormat total time:%d s"
#define AMR_DSP_ADP_2575_112_2_18_1_11_21_533 "AMR_DSP_GetPlayingInfo ERROR not support this operation in this play type:%d."
#define AMR_DSP_ADP_2592_112_2_18_1_11_21_534 "AMR_DSP_GetPlayingInfo: read:%d, other:%d, src_buf:%d, src_to_dsp:%d, src_to_decode:%d, decode:%d."
#define AMR_DSP_ADP_2713_112_2_18_1_11_21_535 "_AMR_DSP_Thread_Entry AMR_DSP_DECODE_PLAY"
#define AMR_DSP_ADP_2718_112_2_18_1_11_21_536 "SET bIsDspReq TRUE 7"
#define AMR_DSP_ADP_2726_112_2_18_1_11_21_537 "[AMR_DSP]DSP DECODE INIT DONE result:%d"
#define AMR_DSP_ADP_2730_112_2_18_1_11_21_538 "SET bIsDspReq 8"
#define AMR_DSP_ADP_2761_112_2_18_1_11_21_539 "_AMR_DSP_Thread_Entry AMR_DSP_DECODE_FRAME decode end!"
#define AMR_DSP_ADP_2775_112_2_18_1_11_21_540 "_AMR_DSP_Thread_Entry Wait time is too long to Get frame!"
#define AMR_DSP_ADP_2829_112_2_18_1_11_21_541 "_AMR_DSP_Thread_Entry AMR_DSP_DECODE_FRAME_IND decode end!"
#define AMR_DSP_ADP_2886_112_2_18_1_11_21_542 "_AMR_DSP_Thread_Entry AMR_DSP_DECODE_FRAME_IND decode end!"
#define AMR_DSP_ADP_2930_112_2_18_1_11_22_543 "[AMR_DSP]PAUSE SUCCESS, waitCount:%d, MAX:%d!"
#define AMR_DSP_ADP_2939_112_2_18_1_11_22_544 "[AMR_DSP]PAUSE BUT NOW DEMUX TASK IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN"
#define AMR_DSP_ADP_2949_112_2_18_1_11_22_545 "[AMR_DSP]FORCE PAUSE SUCCESS DEMUX NOT STOP!"
#define AMR_DSP_ADP_2960_112_2_18_1_11_22_546 "[AMR_DSP]PAUSE BUT NOW DSP IS DECODE, WAIT SOME TIME THEN TRY AGAIN"
#define AMR_DSP_ADP_2970_112_2_18_1_11_22_547 "[AMR_DSP]FORCE PAUSE SUCCESS!"
#define AMR_DSP_ADP_2993_112_2_18_1_11_22_548 "[AMR_DSP]STOP SUCCESS, waitCount:%d, maxCount:%d!"
#define AMR_DSP_ADP_3003_112_2_18_1_11_22_549 "[AMR_DSP]STOP BUT NOW DEMUX TASK IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN"
#define AMR_DSP_ADP_3016_112_2_18_1_11_22_550 "[AMR_DSP]FORCE STOP, waitCount:%d, maxCount:%d!"
#define AMR_DSP_ADP_3028_112_2_18_1_11_22_551 "[AMR_DSP]STOP BUT NOW DSP IS DECODE, WAIT SOME TIME THEN TRY AGAIN"
#define AMR_DSP_ADP_3040_112_2_18_1_11_22_552 "[AMR_DSP]FORCE STOP SUCCESS!"
#define AMR_DSP_ADP_3240_112_2_18_1_11_22_553 "[AMR_DSP]:seek result:%d, offset:%d."
#define AMR_DSP_ADP_3269_112_2_18_1_11_22_554 "[AMR_DSP]:DEMUX THREAD RECV A BAD SIGNAL:%d, DISCARD IT"
#define BASEBAND_AMRCODEC_133_112_2_18_1_11_23_555 "AMR frame len %d: %s"
#define BASEBAND_AMRCODEC_269_112_2_18_1_11_23_556 "WriteFrameToFifo(), fifo overflow"
#define BASEBAND_AMRCODEC_303_112_2_18_1_11_23_557 "SendDataFromFifoToDevice(), is empty"
#define BASEBAND_AMRCODEC_435_112_2_18_1_11_24_558 "[AMR] Error:Dev->Fifo, len[%d]"
#define BASEBAND_AMRCODEC_476_112_2_18_1_11_24_559 "SendFrameFromDeviceToFifo(), fifo overflow"
#define BASEBAND_AMRCODEC_693_112_2_18_1_11_24_560 "BBAMR_ReadAFrame(), error happen when wait for event"
#define BASEBAND_AMRCODEC_700_112_2_18_1_11_24_561 "BBAMR_ReadAFrame(), encount exit event"
#define BASEBAND_AMRCODEC_787_112_2_18_1_11_24_562 "BBAMR_OpenDecoder(), failed to create event"
#define BASEBAND_AMRCODEC_822_112_2_18_1_11_24_563 "BBAMR_SetAmrPackMode()%d , done1"
#define BASEBAND_AMRCODEC_849_112_2_18_1_11_24_564 "BBAMR_CloseDecoder Start"
#define BASEBAND_AMRCODEC_863_112_2_18_1_11_24_565 "BBAMR_CloseDecoder End"
#define BASEBAND_AMRCODEC_870_112_2_18_1_11_24_566 "BBAMR_CloseDecoder(), done"
#define BASEBAND_AMRCODEC_892_112_2_18_1_11_24_567 "BBAMR_WriteAFrame(), error happen when wait for event"
#define BASEBAND_AMRCODEC_898_112_2_18_1_11_24_568 "BBAMR_WriteAFrame(), encount exit event"
#define BASEBAND_AMRCODEC_1119_112_2_18_1_11_25_569 "AP_VT_AMR_OpenDecoder()%d , done1"
#define BASEBAND_AMRCODEC_1170_112_2_18_1_11_25_570 "AP_VT_AMR_OpenDecoder()%d , done1"
#define ARM_AD_ADP_112_112_2_18_1_11_26_571 "_Arm_Ad_OfferCallback, error. hAudioHandle:0x%08x."
#define ARM_AD_ADP_208_112_2_18_1_11_26_572 "arm_ad_adp.c:_Arm_Ad_Play:error:PNULL == s_record_device->tDe"
#define ARM_AD_ADP_260_112_2_18_1_11_26_573 "arm_ad_adp.c:_Arm_Ad_IdentifyFormat:error:audio_obj == SCI_NULL"
#define AUD_DECODE_172_112_2_18_1_11_27_574 "AUD_decode.c:AUD_Decode_Process:aud_empty_frame_put sleep(50)"
#define AUD_DECODE_184_112_2_18_1_11_27_575 "AUD_decode.c:AUD_Decode_Process:aud_empty_frame_put sleep(50)"
#define AUD_DECODE_198_112_2_18_1_11_27_576 "AUD_decode.c:AUD_Decode_Process:aud_empty_frame_put sleep(50)"
#define AUD_DECODE_213_112_2_18_1_11_27_577 "AUD_decode.c:AUD_Decode_Process:aud_empty_frame_put sleep(50)"
#define AUD_DECODE_242_112_2_18_1_11_27_578 "AUD_decode.c:AUD_Decode_Process:aud_valid_frame_get sleep(100)"
#define AUD_DECODE_253_112_2_18_1_11_27_579 "AUD_decode.c:AUD_Decode_Open in"
#define AUD_DECODE_256_112_2_18_1_11_27_580 "AUD_decode.c:AUD_Decode_Open:(!param)||(NULL==*param))"
#define AUD_DECODE_263_112_2_18_1_11_27_581 "AUD_decode.c:AUD_Decode_Open priority is %d"
#define AUD_DECODE_268_112_2_18_1_11_27_582 "AUD_decode.c:AUD_Decode_Open:decode is null"
#define AUD_DECODE_281_112_2_18_1_11_27_583 "AUD_decode.c:AUD_Decode_Open:frame_buffer is null"
#define AUD_DECODE_304_112_2_18_1_11_27_584 "AUD_decode.c:AUD_Decode, AUD_Decode_Open, failed to create event"
#define AUD_DECODE_335_112_2_18_1_11_27_585 "AUD_decode.c:AUD_Decode_Close in"
#define AUD_DECODE_357_112_2_18_1_11_27_586 "AUD_decode.c:AUD_Decode_start in"
#define AUD_DECODE_366_112_2_18_1_11_27_587 "AUD_decode.c:AUD_Decode_start ok"
#define AUD_DECODE_373_112_2_18_1_11_27_588 "aud_decode.c:aud_decode_start: result is %d"
#define AUD_DECODE_386_112_2_18_1_11_27_589 "AUD_decode.c:AUD_Decode_stop in"
#define AUD_DECODE_397_112_2_18_1_11_27_590 "aud_decode.c:aud_decode_stop:result is %d"
#define AUD_DECODE_405_112_2_18_1_11_27_591 "AUD_decode.c:AUD_Decode_stop out ok"
#define AUD_DECODE_409_112_2_18_1_11_27_592 "AUD_decode.c:AUD_Decode_stop out failed"
#define AUD_DECODE_420_112_2_18_1_11_27_593 "AUD_decode.c:AUD_Decode_pause in"
#define AUD_DECODE_431_112_2_18_1_11_27_594 "AUD_decode.c:AUD_Decode_pause result is %d "
#define AUD_DECODE_439_112_2_18_1_11_27_595 "AUD_decode.c:AUD_Decode_pause out ok"
#define AUD_DECODE_444_112_2_18_1_11_27_596 "AUD_decode.c:AUD_Decode_pause out failed"
#define AUD_DECODE_471_112_2_18_1_11_27_597 "amr_decode.c:AMR_Decode_Clean out"
#define AUD_DECODE_536_112_2_18_1_11_27_598 "aud_decode.c:AudioOut_AllocBuffer: result %d"
#define AUD_DECODE_585_112_2_18_1_11_27_599 "AUD_decode.c:CTRL_PAUSE in"
#define AUD_DECODE_588_112_2_18_1_11_28_600 "AUD_decode.c:AUD_DECODE_PAUSE in"
#define AUD_DECODE_595_112_2_18_1_11_28_601 "aud_decode.c:AudioOut_FreeBuffer: result %d"
#define AUD_DECODE_599_112_2_18_1_11_28_602 "AUD_decode.c:AUD_DECODE_PAUSE out"
#define AUD_DECODE_604_112_2_18_1_11_28_603 "aud_decode.c : CTRL_STOP received"
#define AUD_DECODE_607_112_2_18_1_11_28_604 "aud_decode.c: CTRL_STOP received and AUDIO_SM_PlayEnd"
#define AUD_DEMUX_77_112_2_18_1_11_28_605 "arm_demux.c:aud_framebuffer_is_empty: file end but seek again"
#define AUD_DEMUX_84_112_2_18_1_11_28_606 "arm_demux.c:aud_framebuffer_is_empty: demux_stop call back"
#define AUD_DEMUX_95_112_2_18_1_11_28_607 "MP3_demux.c:aud_empty_frame_get:MP3_frame is null"
#define AUD_DEMUX_146_112_2_18_1_11_28_608 "  demux.c:Demux_Process: aud_valid_frame_put result is %dl and SCI_Sleep(20)"
#define AUD_DEMUX_180_112_2_18_1_11_28_609 "  demux.c:Seek_Process:stream_seek failed result is %d"
#define AUD_DEMUX_210_112_2_18_1_11_28_610 "  demux.c:  Demux_Open in"
#define AUD_DEMUX_214_112_2_18_1_11_28_611 "  demux.c:  Demux_Open(!param)||(NULL==*param)"
#define AUD_DEMUX_221_112_2_18_1_11_28_612 "  decode.c:  Decode_Open priority is %d"
#define AUD_DEMUX_229_112_2_18_1_11_28_613 "  demux.c:demux is null"
#define AUD_DEMUX_239_112_2_18_1_11_28_614 "  demux.c:frame_buffer is null"
#define AUD_DEMUX_255_112_2_18_1_11_28_615 "  demux.c:stream_new failed"
#define AUD_DEMUX_261_112_2_18_1_11_28_616 "  demux.c:  Demux_Open:demux->p_event is %d"
#define AUD_DEMUX_264_112_2_18_1_11_28_617 "  Demux,   Demux_Open, failed to create event"
#define AUD_DEMUX_291_112_2_18_1_11_28_618 "  demux.c:  Demux_Close in"
#define AUD_DEMUX_298_112_2_18_1_11_28_619 "  demux.c:  Demux_Close:demux->p_event is %d"
#define AUD_DEMUX_310_112_2_18_1_11_28_620 "  demux.c:  Demux_start in"
#define AUD_DEMUX_329_112_2_18_1_11_28_621 "  demux.c:  Demux_stop in"
#define AUD_DEMUX_350_112_2_18_1_11_28_622 "  demux.c:  Demux_stop out"
#define AUD_DEMUX_354_112_2_18_1_11_28_623 "  demux.c:  Demux_stop out error"
#define AUD_DEMUX_364_112_2_18_1_11_28_624 "amr_demux.c:AMR_Demux_seek in offset type is %d, value  is %d"
#define AUD_DEMUX_439_112_2_18_1_11_29_625 "  demux.c:Demux_Process: frame is null and SCI_Sleep(20)"
#define AUD_ENCODE_157_112_2_18_1_11_29_626 "aud_encode.c:aud_encode_DumpRecordData aud_empty_frame_get error : reuslt is %d"
#define AUD_ENCODE_162_112_2_18_1_11_29_627 "aud_encode.c:  aud_encode_DumpRecordData: dec_common->encode error: result is %d"
#define AUD_ENCODE_191_112_2_18_1_11_29_628 "aud_encode.c: mux_one_frame result is error %d"
#define AUD_ENCODE_221_112_2_18_1_11_29_629 "AUD_Encode_Process aud_empty_frame_get error : reuslt is %d"
#define AUD_ENCODE_231_112_2_18_1_11_29_630 "AUD_Encode_Process aud_valid_frame_get error : reuslt is %d"
#define AUD_ENCODE_243_112_2_18_1_11_29_631 "aud_encode.c:   dec_common->encode error: result is %d"
#define AUD_ENCODE_281_112_2_18_1_11_29_632 "AUD_Encode_Process aud_valid_frame_get error : reuslt is %d"
#define AUD_ENCODE_295_112_2_18_1_11_29_633 "aud_encode.c:   dec_common->encode error: result is %d"
#define AUD_ENCODE_326_112_2_18_1_11_29_634 "aud_encode.c:mux_one_frame: result error %d"
#define AUD_ENCODE_349_112_2_18_1_11_29_635 "AUD_encode.c:AUD_Encode_Open in"
#define AUD_ENCODE_352_112_2_18_1_11_29_636 "AUD_encode.c:AUD_Encode_Open:(!param)||(NULL==*param))"
#define AUD_ENCODE_359_112_2_18_1_11_29_637 "AUD_encode.c:AUD_Encode_Open priority is %d"
#define AUD_ENCODE_364_112_2_18_1_11_29_638 "AUD_encode.c:AUD_Encode_Open:encode is null"
#define AUD_ENCODE_391_112_2_18_1_11_30_639 "AUD_encode.c:AUD_Encode, AUD_Encode_Open, failed to create event"
#define AUD_ENCODE_430_112_2_18_1_11_30_640 "AUD_encode.c:AUD_Encode_Close in"
#define AUD_ENCODE_457_112_2_18_1_11_30_641 "AUD_encode.c:AUD_Encode_start in"
#define AUD_ENCODE_463_112_2_18_1_11_30_642 "AUD_encode.c:AUD_Encode_start ok"
#define AUD_ENCODE_474_112_2_18_1_11_30_643 "AUD_encode.c:AUD_Encode_stop in"
#define AUD_ENCODE_493_112_2_18_1_11_30_644 "AUD_encode.c:AUD_Encode_stop out ok"
#define AUD_ENCODE_497_112_2_18_1_11_30_645 "AUD_encode.c:AUD_Encode_stop out failed"
#define AUD_ENCODE_506_112_2_18_1_11_30_646 "AUD_encode.c:AUD_Encode_pause in"
#define AUD_ENCODE_523_112_2_18_1_11_30_647 "AUD_encode.c:AUD_Encode_pause  wait"
#define AUD_ENCODE_526_112_2_18_1_11_30_648 "AUD_encode.c:AUD_Encode_pause out ok"
#define AUD_ENCODE_531_112_2_18_1_11_30_649 "AUD_encode.c:AUD_Encode_pause out failed"
#define AUD_ENCODE_589_112_2_18_1_11_30_650 "amr_encode.c:AMR_Encode_Clean out"
#define AUD_ENCODE_685_112_2_18_1_11_30_651 "aud_encode.c: mux_end_pf called"
#define AUD_ENCODE_698_112_2_18_1_11_30_652 "aud_encode.c:AUD_Encode_Process: result is %d"
#define AUD_ENCODE_715_112_2_18_1_11_30_653 "AUD_encode.c:CTRL_PAUSE in"
#define AUD_ENCODE_725_112_2_18_1_11_30_654 "AUD_encode.c:AUD_ENCODE_PAUSE in thread "
#define AUD_ENCODE_736_112_2_18_1_11_30_655 "AUD_encode.c:AUD_ENCODE_PAUSE out thread"
#define AUD_ENCODE_741_112_2_18_1_11_30_656 "aud_encode.c : CTRL_STOP received"
#define AUD_ENCODE_754_112_2_18_1_11_31_657 "aud_encode.c: CTRL_STOP received and AUDIO_SM_PlayEnd"
#define AUD_MUX_131_112_2_18_1_11_31_658 "  mux.c:  Mux_Open in"
#define AUD_MUX_135_112_2_18_1_11_31_659 "  mux.c:  Mux_Open(!param)||(NULL==*param)"
#define AUD_MUX_142_112_2_18_1_11_31_660 "  decode.c:  Decode_Open priority is %d"
#define AUD_MUX_148_112_2_18_1_11_31_661 "  mux.c:mux is null"
#define AUD_MUX_158_112_2_18_1_11_31_662 "  mux.c:frame_buffer is null"
#define AUD_MUX_172_112_2_18_1_11_31_663 "  mux.c:  Mux_Open:mux->p_event is %d"
#define AUD_MUX_175_112_2_18_1_11_31_664 "  Mux,   Mux_Open, failed to create event"
#define AUD_MUX_203_112_2_18_1_11_31_665 "  mux.c:  Mux_Close in"
#define AUD_MUX_210_112_2_18_1_11_31_666 "  mux.c:  Mux_Close:mux->p_event is %d"
#define AUD_MUX_222_112_2_18_1_11_31_667 "  mux.c:  Mux_start in"
#define AUD_MUX_241_112_2_18_1_11_31_668 "aud_mu.c:aud_mux_pause in"
#define AUD_MUX_249_112_2_18_1_11_31_669 "aud_mux.c:aud_mux_pause out ok"
#define AUD_MUX_254_112_2_18_1_11_31_670 "aud_mux.c:aud_mux_pause out failed"
#define AUD_MUX_263_112_2_18_1_11_31_671 "  mux.c:  Mux_stop in"
#define AUD_MUX_271_112_2_18_1_11_31_672 "  mux.c:  Mux_stop out"
#define AUD_MUX_275_112_2_18_1_11_31_673 "  mux.c:  Mux_stop out error"
#define AUD_MUX_356_112_2_18_1_11_32_674 "  mux.c:Mux_Process: result is %d"
#define AUD_MUX_370_112_2_18_1_11_32_675 "aud_mux.c: mux_end_pf called"
#define AUD_MUX_383_112_2_18_1_11_32_676 "aud_mux.c:CTRL_PAUSE in"
#define AUD_MUX_386_112_2_18_1_11_32_677 "aud_mux.c:STATE_MUX_PAUSE in thread"
#define AUD_MUX_389_112_2_18_1_11_32_678 "aud_mux.c:STATE_MUX_PAUSE out thread"
#define AUD_MUX_394_112_2_18_1_11_32_679 "aud_mux.c : CTRL_STOP received"
#define AUD_MUX_399_112_2_18_1_11_32_680 "aud_mux.c: CTRL_STOP received and AUDIO_SM_PlayEnd"
#define AUDIO_INPUT_80_112_2_18_1_11_32_681 "audio_input.c: Audio_Input_DataOkNotify: end _call back"
#define AUDIO_INPUT_111_112_2_18_1_11_32_682 "audio_input.c:Read_Layer1_Data_Callback:aud_empty_frame_get failed lost data  count is %d"
#define AUDIO_INPUT_158_112_2_18_1_11_32_683 "audio_input.c:Read_Layer1_Data_Callback:aud_empty_frame_get failed 2"
#define AUDIO_INPUT_204_112_2_18_1_11_32_684 "audio_output.c:AudioOut_Open:audio_out is null"
#define AUDIO_INPUT_225_112_2_18_1_11_32_685 "audio_input.c:DSP_CODEC_Ext_StartUpLoadData:SCI_ALLOC_APP TEMP_FRAME_T failed"
#define AUDIO_INPUT_236_112_2_18_1_11_32_686 "peter: audio_input.c: frame size is %d"
#define AUDIO_INPUT_241_112_2_18_1_11_32_687 "audio_input.c:DSP_CODEC_Ext_StartUpLoadData:SCI_ALLOC_APP TEMP_FRAME_T buffer failed"
#define AUDIO_INPUT_255_112_2_18_1_11_32_688 "audio_input.c:DSP_CODEC_Ext_StartUpLoadData:SCI_ALLOC_APP s_free_frame failed"
#define AUDIO_INPUT_265_112_2_18_1_11_32_689 "audio_input.c:DSP_CODEC_Ext_StartUpLoadData:SCI_ALLOC_APP s_free_frame buffer failed"
#define AUDIO_INPUT_298_112_2_18_1_11_32_690 "audio_input.c: Audio_Input_Start: in h_aud_input is %x"
#define AUDIO_INPUT_316_112_2_18_1_11_32_691 "audio_input.c: Audio_Input_Stop: in h_aud_input is %x"
#define AUDIO_OUTPUT_146_112_2_18_1_11_33_692 "audio_output.c:Aout_Buffer_Manager_Init:SCI_ALLOCA failed i is %d"
#define AUDIO_OUTPUT_185_112_2_18_1_11_33_693 "aduio_output.c:AudioOut_Open in"
#define AUDIO_OUTPUT_189_112_2_18_1_11_33_694 "audio_output.c:AudioOut_Open:audio_out is null"
#define AUDIO_OUTPUT_228_112_2_18_1_11_33_695 "audio_output.c:AudioOut_FreeBuffer:audio_out is null"
#define AUDIO_OUTPUT_256_112_2_18_1_11_34_696 "audio_output.c:AudioOut_AllocBuffer:audio_out is null"
#define AUDIO_OUTPUT_295_112_2_18_1_11_34_697 "aduio_output.c:AudioOut_Close in"
#define AUDIO_OUTPUT_347_112_2_18_1_11_34_698 "audio_output.c:AudioOut_Buffer_Get NULL"
#define AUDIO_FRAMEBUFFER_38_112_2_18_1_11_34_699 "audio_framebuffer.c:Init_BufferQueue:buf_Q->frame is null"
#define AUDIO_FRAMEBUFFER_44_112_2_18_1_11_34_700 "audio_framebuffer.c:Init_BufferQueue:buf_Q->buffer_header is null"
#define AUDIO_FRAMEBUFFER_53_112_2_18_1_11_34_701 "audio_framebuffer.c:Init_BufferQueue:SCI_CreateMutex failed!"
#define AUDIO_FRAMEBUFFER_61_112_2_18_1_11_34_702 "audio_framebuffer.c:Init_BufferQueue:SCI_CreateEvent failed!"
#define AUDIO_FRAMEBUFFER_289_112_2_18_1_11_35_703 "audio_farmebuffer.c:Init_FrameQueue:Queue_ptr->Frame is null"
#define AUDIO_FRAMEBUFFER_302_112_2_18_1_11_35_704 "audio_framebuffer.c:Init_FrameQueue:SCI_CreateEvent failed!"
#define AUDIO_FRAMEBUFFER_558_112_2_18_1_11_35_705 "audio_framebuffer.c:aud_framebuffer_new frame_buffer is null"
#define AUDIO_FRAMEBUFFER_570_112_2_18_1_11_35_706 "audio_framebuffer.c:Init_BufferQueue failed"
#define AUDIO_FRAMEBUFFER_578_112_2_18_1_11_35_707 "audio_framebuffer.c:Init_FrameQueue failed"
#define AUDIO_STREAM_97_112_2_18_1_11_36_708 "audio_stream.c:DataBuffer_Fill:SFS_ReadFile:error result is %d"
#define AUDIO_STREAM_137_112_2_18_1_11_36_709 "audio_stream.c: buffer fill erororoororor"
#define AUDIO_STREAM_148_112_2_18_1_11_36_710 "audio_stream.c: buffer_fill,result is %d"
#define AUDIO_STREAM_494_112_2_18_1_11_37_711 "audio_stream.c:DataBuffer_Seek resource type is %d,offset is %d"
#define AUDIO_STREAM_502_112_2_18_1_11_37_712 "audio_stream.c:SFS_SetFilePointer:result is %d"
#define AUDIO_STREAM_577_112_2_18_1_11_37_713 "audio_stream.c:DataBuffer_GetLength:file_size is %d,length is %d"
#define AUDIO_STREAM_588_112_2_18_1_11_37_714 "audio_stream.c:stream_get_length:DataBuffer_GetLength: is %d"
#define AUDIO_STREAM_669_112_2_18_1_11_38_715 "audio_stream.c:stream_read:DataBuffer_Fill:stream is end"
#define AUDIO_STREAM_805_112_2_18_1_11_38_716 "audio_stream.c: stream_peek erro3 result is %d"
#define AUDIO_STREAM_813_112_2_18_1_11_38_717 "audio_stream.c: stream_offset_peek: peek out of range !!!!!"
#define AUDIO_STREAM_935_112_2_18_1_11_38_718 "audio_stream.c: DataBuffer_Offset_Peek erro result is %d"
#define AUDIO_STREAM_943_112_2_18_1_11_38_719 "audio_stream.c: stream_offset_peek: peek out of range !!!!!"
#define AUD_DSP_INTERFACE_40_112_2_18_1_11_39_720 "aud_dsp_interface.c:mp3_decoder_decode: result is %d,pcm_samples is %d"
#define AUD_DSP_MNG_439_112_2_18_1_11_41_721 "aud_dsp_mng.c:aac_dec_process: frame len error > 0x800 or decode error"
#define AUD_DSP_MNG_474_112_2_18_1_11_41_722 "peter: AMR_WriteOneFrameToDsp no data................"
#define AUD_DSP_MNG_904_112_2_18_1_11_42_723 "aud_dsp_mng.c:capability_check_and_set failed type is %d,aud_dsp_res_status is %d"
#define AUD_DSP_MNG_1001_112_2_18_1_11_42_724 "aud_dsp_mng.c:capability_check_and_clean failed type is %d,aud_dsp_res_status is %d"
#define AUD_DSP_MNG_1109_112_2_18_1_11_42_725 "aud_dsp_mng.c:aud_dsp_open in type is %d"
#define AUD_DSP_MNG_1113_112_2_18_1_11_42_726 "aud_dsp_mng.c: aud_dsp_open: capability_check_and_set %d"
#define AUD_DSP_MNG_1120_112_2_18_1_11_42_727 "aud_dsp_mng.c:aud_dsp_open failed  "
#define AUD_DSP_MNG_1129_112_2_18_1_11_42_728 "aud_dsp_mng.c:, Open(), aud_dsp_open to create event"
#define AUD_DSP_MNG_1136_112_2_18_1_11_42_729 "aud_dsp_mng.c:, Open(), aud_dsp_open to SCI_CreateMutex"
#define AUD_DSP_MNG_1147_112_2_18_1_11_42_730 "aud_dsp_mng.c:aud_dsp_open out type is %d"
#define AUD_DSP_MNG_1179_112_2_18_1_11_42_731 "aud_dsp_mng.c:aud_dsp_closein handle is %d"
#define AUD_DSP_MNG_1185_112_2_18_1_11_42_732 "aud_dsp_mng.c: aud_dsp_open: capability_check_and_set %d"
#define AUD_DSP_MNG_1192_112_2_18_1_11_43_733 "aud_dsp_mng.c:aud_dsp_close  error  wait state is %d"
#define AUD_DSP_MNG_1216_112_2_18_1_11_43_734 "aud_dsp_mng.c:aud_dsp_close: out"
#define AUD_DSP_MNG_1218_112_2_18_1_11_43_735 "aud_dsp_mng.c:aud_dsp_close out handle is %d"
#define AUD_DSP_MNG_1230_112_2_18_1_11_43_736 "aud_dsp_mng.c:aud_dsp_process in handle is %d"
#define AUD_DSP_MNG_1250_112_2_18_1_11_43_737 "aud_dsp_mng.c:aud_dsp_process out handle is %d,type is %d"
#define ARM_SOURCE_138_112_2_18_1_11_44_738 "arm_source.c :arm_source_open failed"
#define DSP_SOURCE_129_112_2_18_1_11_45_739 "dsp_source.c :dsp_source_open failed"
#define CAT_MVOICE_198_112_2_18_1_11_45_740 "[Cat_mvoice]: MicSample_GetMicdB get_db=%d"
#define CAT_MVOICE_215_112_2_18_1_11_45_741 "[Cat_mvoice]: CatMv_MicSample_CheckIfEnd end_type=%d"
#define CAT_MVOICE_226_112_2_18_1_11_45_742 "[Cat_mvoice]: CatMv_MicSample_CheckIfEnd cat_mic_process.pre_mute_count=%d"
#define CAT_MVOICE_228_112_2_18_1_11_45_743 "[Cat_mvoice]: CatMv_MicSample_CheckIfEnd needed_time=%d,base_value=%d"
#define CAT_MVOICE_243_112_2_18_1_11_45_744 "[Cat_mvoice]: CatMv_MicSample_CheckIfEnd get_db=%d,base_value=%d buffer_pos=%d"
#define CAT_MVOICE_259_112_2_18_1_11_45_745 "[Cat_mvoice]: CatMv_MicSample_CheckIfEnd get_db=%d,base_value=%d,mute_count=%d"
#define CAT_MVOICE_296_112_2_18_1_11_45_746 "[Cat_mvoice]: CatMv_MicSample_CheckIfEnd test_count=%d"
#define CAT_MVOICE_328_112_2_18_1_11_45_747 "[Cat_mvoice]: CatMv_MicData_Validsize first_get_date=%d"
#define CAT_MVOICE_368_112_2_18_1_11_46_748 "[Cat_mvoice]: GET MIC W=%d,R=%d,valid_size=%d "
#define CAT_MVOICE_543_112_2_18_1_11_46_749 "[Cat_mvoice]: CatMv_Process_Data enter"
#define CAT_MVOICE_551_112_2_18_1_11_46_750 "[Cat_mvoice]: CatMv_Process_Data uiSrcCount=%d,  read_p=%d"
#define CAT_MVOICE_558_112_2_18_1_11_46_751 "[Cat_mvoice]: CatMv_Process_Data ret_value=%d,valid_count=%d"
#define CAT_MVOICE_636_112_2_18_1_11_46_752 "[Cat_mvoice]: CatMv_Process_Data scale=%d,cat_mic_process.mute_index=%d"
#define CAT_MVOICE_642_112_2_18_1_11_46_753 "[Cat_mvoice]: CatMv_Process_Data ret_value=%d,cat_mic_process.valid_count=%d"
#define CAT_MVOICE_766_112_2_18_1_11_46_754 "[Cat_mvoice]: MicSample_Play have opened"
#define CAT_MVOICE_787_112_2_18_1_11_46_755 "[Cat_mvoice]: MicSample_Play!"
#define CAT_MVOICE_817_112_2_18_1_11_46_756 "[Cat_mvoice]: MicSample_Stop have Closed"
#define CAT_MVOICE_878_112_2_18_1_11_47_757 "[Cat_mvoice]: AUDIO_PM_RegCodecPlugger "
#define CAT_MVOICE_916_112_2_18_1_11_47_758 "[Cat_mvoice]: CatMv_Process_Data uiSrcCount=%d "
#define CAT_MVOICE_920_112_2_18_1_11_47_759 "[Cat_mvoice]: CatMv_Process_Data enter callback "
#define CAT_MVOICE_991_112_2_18_1_11_47_760 "[Cat_mvoice]: CatMv_Process *puiDestCount=%d, uiCurrentSamplerate=%d"
#define CAT_MVOICE_1062_112_2_18_1_11_47_761 "[Cat_mvoice]: CatMv_RegExpOnOff s_catmv_expression_on_flag=%d"
#define CUSTOM_TONE_ADP_190_112_2_18_1_11_50_762 "CUSTOM_TONE_Construct SCI_CreateMutex pTonePlayMutex failed."
#define CUSTOM_TONE_ADP_200_112_2_18_1_11_50_763 "CUSTOM_TONE_Construct SCI_CreateMutex pToneStopMutex failed."
#define CUSTOM_TONE_ADP_210_112_2_18_1_11_50_764 "CUSTOM_TONE_Construct SCI_CreateTimer failed."
#define CUSTOM_TONE_ADP_230_112_2_18_1_11_50_765 "custome_tone_adp.c:CUSTOM_TONE_IdentifyFormat:error:PNULL == ptAudioObject"
#define CUSTOM_TONE_ADP_258_112_2_18_1_11_50_766 "custom_tone_adp.c: CUSTOM_TONE_Ext_Play other task has been playing"
#define CUSTOM_TONE_ADP_266_112_2_18_1_11_50_767 "custom_tone_adp.c:CUSTOM_TONE_Ext_Play:error:PNULL == ptAudioObject"
#define CUSTOM_TONE_ADP_272_112_2_18_1_11_50_768 "custom_tone_adp.c:CUSTOM_TONE_Ext_Play:error:0==ptPara->duration."
#define CUSTOM_TONE_ADP_278_112_2_18_1_11_50_769 "custom_tone_adp.c:CUSTOM_TONE_Ext_Play: is playing."
#define CUSTOM_TONE_ADP_296_112_2_18_1_11_50_770 "[CUSTOM_TONE_Ext_Play] AUDIO_DM_Open failed!"
#define CUSTOM_TONE_ADP_304_112_2_18_1_11_50_771 "custom_tone_adp.c: CUSTOM_TONE_Ext_Play other task has been playing"
#define CUSTOM_TONE_ADP_345_112_2_18_1_11_50_772 "custome_tone_adp.c:CUSTOM_TONE_Ext_Stop:error:uiArgc != 0"
#define CUSTOM_TONE_ADP_351_112_2_18_1_11_50_773 "custom_tone_adp.c: CUSTOM_TONE_Ext_Stop other task has been stopping"
#define CUSTOM_TONE_ADP_356_112_2_18_1_11_50_774 "custom_tone_adp.c:CUSTOM_TONE_Ext_Stop: is not playing."
#define CUSTOM_TONE_ADP_365_112_2_18_1_11_50_775 "custom_tone_adp.c: CUSTOM_TONE_Ext_Stop the tone is never played."
#define CUSTOM_TONE_ADP_374_112_2_18_1_11_50_776 "custome_tone_adp.c:CUSTOM_TONE_Ext_Stop:error:PNULL == ptAudioObject"
#define CUSTOM_TONE_ADP_395_112_2_18_1_11_50_777 "%s, %d. CUSTOM_TONE_Stop"
#define CUSTOM_TONE_ADP_407_112_2_18_1_11_50_778 "custom_tone_adp.c:CUSTOM_TONE_Stop:error:PNULL == ptAudioObject"
#define CUSTOM_TONE_ADP_414_112_2_18_1_11_50_779 "custom_tone_adp.c: CUSTOM_TONE_Stop other task has been stopping"
#define CUSTOM_TONE_ADP_419_112_2_18_1_11_50_780 "custom_tone_adp.c:CUSTOM_TONE_Stop: is not playing."
#define CUSTOM_TONE_ADP_445_112_2_18_1_11_51_781 "custom_tone_adp.c: AUD_CustomToneStop other task has been stopping"
#define AMR_DECODE_369_112_2_18_1_12_31_782 "mp3_decode:mp3_decode_open failed"
#define AMR_DECODE_392_112_2_18_1_12_31_783 "amr_decode.c:AMR_Decode_Open:decode->serial_buffer is null"
#define AMR_DSP_DECODE_192_112_2_18_1_12_32_784 "amr_dsp_decode:amr_decode_open failed"
#define MP3_ARM_DECODE_67_112_2_18_1_12_32_785 "mp3_decode:mp3_decode_open failed"
#define MP3_DSP_DECODE_27_112_2_18_1_12_32_786 "mp3_decode_peter.c:mp3_decode_init:mp3_decoder_open:failed"
#define MP3_DSP_DECODE_107_112_2_18_1_12_32_787 "mp3_decode:mp3_decode_open failed"
#define AMR_DEMUX_119_112_2_18_1_12_33_788 "amr_demux.c:amr_demux_open:(bytes_read != sizeof(AMR_HEADER))  result is %d"
#define AMR_DEMUX_149_112_2_18_1_12_33_789 "amr_demux.c:AMR_Demux_IdentifyFormat:stream_length is 0"
#define AMR_DEMUX_153_112_2_18_1_12_33_790 "amr_demux.c:AMR_IdentifyFormat total time:%d s"
#define AMR_DEMUX_187_112_2_18_1_12_33_791 "amr_demux.c:Demux_One_Frame:stream_peek:result is %d, bytes_read is %d)"
#define AMR_DEMUX_204_112_2_18_1_12_33_792 "amr_demux.c:Demux_One_Frame:stream_read:stream_result==DATA_END)"
#define AMR_DEMUX_229_112_2_18_1_12_33_793 "amr_demux.c:Demux_One_Frame flush one byte"
#define AMR_DEMUX_379_112_2_18_1_12_33_794 "MP3_demux.c:stream_new failed"
#define MP3_DEMUX_41_112_2_18_1_12_34_795 "[MP3_ParseFileInfo,2total time=%ds]"
#define MP3_DEMUX_46_112_2_18_1_12_34_796 "MP3_ParseFileInfo,3total time=%ds]"
#define MP3_DEMUX_80_112_2_18_1_12_34_797 "[MP3_ParseFileInfo,1total time=%ds]"
#define MP3_DEMUX_122_112_2_18_1_12_34_798 "mp3_demux.c: identify_format result is %d"
#define MP3_DEMUX_555_112_2_18_1_12_35_799 "MP3_demux.c:stream_new failed"
#define MP3_DMX_BIT_238_112_2_18_1_12_35_800 "mp3_new_demux.c:decode_header:error:header->flags & MAD_FLAG_MPEG_2_5_EXT"
#define MP3_DMX_BIT_247_112_2_18_1_12_35_801 "mp3_new_demux.c:decode_header:error layer is 4"
#define MP3_DMX_BIT_263_112_2_18_1_12_35_802 "mp3_new_demux.c:decode_header:error MAD_ERROR_BADBITRATE"
#define MP3_DMX_BIT_276_112_2_18_1_12_36_803 "mp3_new_demux.c:decode_header:error MAD_ERROR_BADSAMPLERATE"
#define MP3_DMX_FORMAT_593_112_2_18_1_12_37_804 "mp3_dmx_format.c:MP3_Dmx_iToc:error:puiBuf == PNULL"
#define MP3_DMX_FORMAT_598_112_2_18_1_12_37_805 "[MP3_Dmx_iToc]:input number: %d is not valid,(should be less than 9999) !"
#define MP3_DMX_FORMAT_634_112_2_18_1_12_37_806 "mp3_dmx_format.c:MP3_Dmx_cToi:(puiBuf == PNULL)||(puiNum== 0)"
#define MP3_DMX_FORMAT_655_112_2_18_1_12_37_807 "[MP3_DMX_FORMAT][MP3_CaculateNum]:i_calc:%d!not number!"
#define MP3_DMX_FORMAT_679_112_2_18_1_12_37_808 "mp3_dmx_format.c:MP3_Dmx_CheckheaderInfo:(SCI_NULL == puiHeader)||(SCI_NULL == ptMp3HeaderInfo)"
#define MP3_DMX_FORMAT_815_112_2_18_1_12_38_809 "mp3_dmx_format.c:MP3_Dmx_CheckheaderInfo:(SCI_NULL == puiHeader)||(SCI_NULL == ptMp3HeaderInfo)"
#define MP3_DMX_FORMAT_896_112_2_18_1_12_38_810 "mp3_dmx_format.c: [MP3_CheckID3V2Tag]: ID3 FLAG NOT EXIST"
#define MP3_DMX_FORMAT_908_112_2_18_1_12_38_811 "[MP3_CheckID3V2Tag]:tag version:%d,tag size:0x%08x"
#define MP3_DMX_FORMAT_923_112_2_18_1_12_38_812 "[MP3_CheckID3V2Tag]:End of tag frames !"
#define MP3_DMX_FORMAT_951_112_2_18_1_12_38_813 "[MP3_CheckID3V2Tag]:frame id :%d ,frame_content_size:%d"
#define MP3_DMX_FORMAT_962_112_2_18_1_12_38_814 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_988_112_2_18_1_12_38_815 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_1014_112_2_18_1_12_38_816 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_1040_112_2_18_1_12_38_817 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_1066_112_2_18_1_12_38_818 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_1092_112_2_18_1_12_38_819 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_1118_112_2_18_1_12_38_820 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_1163_112_2_18_1_12_38_821 "[MP3_CheckID3V1Tag] V1.0:no valid year info!"
#define MP3_DMX_FORMAT_1168_112_2_18_1_12_38_822 "[MP3_CheckID3V2Tag]:no valid genre index!"
#define MP3_DMX_FORMAT_1197_112_2_18_1_12_38_823 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_1215_112_2_18_1_12_38_824 "[MP3_CheckID3V1Tag] V1.0:no valid year info!"
#define MP3_DMX_FORMAT_1219_112_2_18_1_12_38_825 "[MP3_CheckID3V2Tag]:year:impossible value!"
#define MP3_DMX_FORMAT_1231_112_2_18_1_12_38_826 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_1249_112_2_18_1_12_38_827 "mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!"
#define MP3_DMX_FORMAT_1253_112_2_18_1_12_38_828 "mp3_format.c,[MP3_CheckID3V2Tag]:track:impossible value!"
#define MP3_DMX_FORMAT_1267_112_2_18_1_12_38_829 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_1303_112_2_18_1_12_39_830 "mp3_format.c,[MP3_CheckID3V2Tag]:other frame ID !We do not need the frame content!"
#define MP3_DMX_FORMAT_1311_112_2_18_1_12_39_831 "[MP3_CheckID3V2Tag]find invalid frame ID!"
#define MP3_DMX_FORMAT_1421_112_2_18_1_12_39_832 "mp3_dmx_format.c:mp3_stream_fast_sync:result is %d"
#define MP3_DMX_FORMAT_1434_112_2_18_1_12_39_833 "mp3_dmx_format.c:state_cur_syn:stream_peek:result is %d"
#define MP3_DMX_FORMAT_1442_112_2_18_1_12_39_834 "mp3_dmx_format.c:state_cur_syn:MP3_Dmx_CheckheaderInfo:result is %d"
#define MP3_DMX_FORMAT_1449_112_2_18_1_12_39_835 "peter:MP3_frame->frame_len is %d"
#define MP3_DMX_FORMAT_1460_112_2_18_1_12_39_836 "mp3_dmx_format.c:state_cur_syn::frame_len is %d"
#define MP3_DMX_FORMAT_1599_112_2_18_1_12_39_837 "mp3_dmx_format.c:::valid_frame_count   is %d"
#define MP3_DMX_FORMAT_1629_112_2_18_1_12_39_838 "mp3_dmx_format.c:MP3_Dmx_CheckAPETagHeader:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)"
#define MP3_DMX_FORMAT_1664_112_2_18_1_12_39_839 "[MP3_CheckLameTag] no valid version info!"
#define MP3_DMX_FORMAT_1809_112_2_18_1_12_40_840 "[MP3_CheckID3V1Tag] V1.1:no valid year info!"
#define MP3_DMX_FORMAT_1826_112_2_18_1_12_40_841 "[MP3_CheckID3V1Tag] V1.1:no valid genre info!"
#define MP3_DMX_FORMAT_1843_112_2_18_1_12_40_842 "mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!"
#define MP3_DMX_FORMAT_1857_112_2_18_1_12_40_843 "[MP3_CheckID3V1Tag]V1.0: no valid genre info!"
#define MP3_DMX_FORMAT_1891_112_2_18_1_12_40_844 "mp3_dmx_format.c:MP3_Dmx_CheckXingHeader:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)"
#define DOWNLINK_CODEC_ADP_93_112_2_18_1_12_40_845 "[BT]downlink_read_data:Data Full:0x%X, 0x%X\r\n"
#define DOWNLINK_CODEC_ADP_120_112_2_18_1_12_41_846 "[BT]PCM:0x%X,0x%X,0x%X,0x%X\r\n"
#define DOWNLINK_CODEC_ADP_137_112_2_18_1_12_41_847 "[BT]downlink_read_data:track_occupied is zero\r\n"
#define DOWNLINK_CODEC_ADP_148_112_2_18_1_12_41_848 "[DOWNLINK_Play] IN"
#define DOWNLINK_CODEC_ADP_158_112_2_18_1_12_41_849 "[DOWNLINK_Play] this codec is playing."
#define DOWNLINK_CODEC_ADP_165_112_2_18_1_12_41_850 "[DOWNLINK_Play] Have opened"
#define DOWNLINK_CODEC_ADP_193_112_2_18_1_12_41_851 "[DOWNLINK_Stop] IN"
#define DOWNLINK_CODEC_ADP_203_112_2_18_1_12_41_852 "[DOWNLINK_Stop] this codec is not playing."
#define DOWNLINK_CODEC_ADP_224_112_2_18_1_12_41_853 "[DATA_Stop] has closed"
#define DOWNLINK_CODEC_ADP_251_112_2_18_1_12_41_854 "DOWNLINK_IdentifyFormat:error:SCI_NULL == ptAudioObject"
#define DRA_ADP_293_112_2_18_1_12_42_855 "[DRA_Construct] Alloc failed, left_exp_data_ptr==NULL"
#define DRA_ADP_304_112_2_18_1_12_42_856 "[DRA_Construct] Alloc failed, right_exp_data_ptr==NULL"
#define DRA_ADP_315_112_2_18_1_12_42_857 "[DRA_Construct] Alloc failed, dra_res_ptr==NULL"
#define DRA_ADP_331_112_2_18_1_12_42_858 "[DRA_Construct] aud_obj_ptr->pGetSrcCallbackFunc==NULL"
#define DRA_ADP_339_112_2_18_1_12_42_859 "[DRA_Construct] dra_res_ptr->sub_type error"
#define DRA_ADP_362_112_2_18_1_12_42_860 "[DRA_Construct] Create DRA thread failed."
#define DRA_ADP_441_112_2_18_1_12_42_861 "[DRA_PlayConstruct] CHNG_FREQ_REQ_HANDLER_NULL == s_ui_dra_chng_freq_req_handler"
#define DRA_ADP_486_112_2_18_1_12_42_862 "[DRA_ResumeConstruct] CHNG_FREQ_REQ_HANDLER_NULL == s_ui_dra_chng_freq_req_handler"
#define DRA_ADP_530_112_2_18_1_12_42_863 "[DRA_Play] NULL == sig_ptr"
#define DRA_ADP_538_112_2_18_1_12_42_864 "\"[DRA_Play] start to play."
#define DRA_ADP_556_112_2_18_1_12_42_865 "[DRA_Stop] NULL == sig_ptr"
#define DRA_ADP_564_112_2_18_1_12_42_866 "[DRA_Stop] stopped."
#define DRA_ADP_582_112_2_18_1_12_42_867 "[DRA_Pause] NULL == sig_ptr"
#define DRA_ADP_590_112_2_18_1_12_42_868 "[DRA_Pause] pause."
#define DRA_ADP_609_112_2_18_1_12_42_869 "[DRA_Resume] NULL == sig_ptr"
#define DRA_ADP_617_112_2_18_1_12_42_870 "[DRA_Resume] resume."
#define DRA_ADP_651_112_2_18_1_12_42_871 "[DRA_IdentifyFormat] error:NULL == dra_res_ptr"
#define DRA_ADP_663_112_2_18_1_12_42_872 "[DRA_IdentifyFormat] error:NULL == cfg_info_ptr"
#define DRA_ADP_672_112_2_18_1_12_43_873 "[DRA_IdentifyFormat] dra_res_ptr->sub_type error"
#define DRA_ADP_702_112_2_18_1_12_43_874 "[DRA_GetOutputSamplerate] OutputSampleRate=%d"
#define DRA_ADP_758_112_2_18_1_12_43_875 "[DRA_SendSig] default error"
#define DRA_ADP_781_112_2_18_1_12_43_876 "[DRA_InitDecoder] Error: dec_result=%d"
#define DRA_ADP_784_112_2_18_1_12_43_877 "[DRA_InitDecoder] Init successfully."
#define DRA_ADP_807_112_2_18_1_12_43_878 "[DRA_TermDecoder] Error: dec_result=%d"
#define DRA_ADP_810_112_2_18_1_12_43_879 "[DRA_TermDecoder] terminate successfully."
#define DRA_ADP_852_112_2_18_1_12_43_880 "[DRA_DecodeAudFrame] Error: dec_result=%d"
#define DRA_ADP_920_112_2_18_1_12_43_881 "[pGetSrcCallbackFunc] 0 == dra_res_ptr->src_data_len"
#define DRA_ADP_930_112_2_18_1_12_43_882 "[pGetSrcCallbackFunc] AUDIO_STREAM_GET_SRC_WAIT"
#define DRA_ADP_935_112_2_18_1_12_43_883 "[pGetSrcCallbackFunc] AUDIO_STREAM_GET_SRC_ASY"
#define DRA_ADP_939_112_2_18_1_12_43_884 "[pGetSrcCallbackFunc] AUDIO_STREAM_GET_SRC_ERROR"
#define DRA_ADP_944_112_2_18_1_12_43_885 "[pGetSrcCallbackFunc] default error"
#define DRA_ADP_1012_112_2_18_1_12_43_886 "[DRA_Thread_Entry] default error"
#define DSP_CODEC_ADP_121_112_2_18_1_13_1_887 "dsp_codec_adp.c:Read_Layer1_Data_Callback:aud_empty_frame_get failed 1"
#define DSP_CODEC_ADP_167_112_2_18_1_13_1_888 "dsp_codec_adp.c:Read_Layer1_Data_Callback:aud_empty_frame_get failed 2"
#define DSP_CODEC_ADP_289_112_2_18_1_13_1_889 "dsp_codec_adp.c:DSP_CODEC_Ext_Play:error: || (NULL==pvArgv)"
#define DSP_CODEC_ADP_302_112_2_18_1_13_1_890 "dsp_codec_adp.c:DSP_CODEC_Ext_Play:error:audio_obj == SCI_NULL"
#define DSP_CODEC_ADP_352_112_2_18_1_13_1_891 "dsp_codec_adp.c:DSP_CODEC_Ext_Stop:error:audio_obj == SCI_NULL"
#define DSP_CODEC_ADP_389_112_2_18_1_13_1_892 "dsp_codec_adp.c:DSP_CODEC_Ext_WriteVoiceData:error: || (NULL==pvArgv)"
#define DSP_CODEC_ADP_397_112_2_18_1_13_1_893 "dsp_codec_adp.c:DSP_CODEC_Ext_WriteVoiceData: PNULL==ptPara->source_buf_ptr"
#define DSP_CODEC_ADP_424_112_2_18_1_13_1_894 "dsp_codec_adp.c:DSP_CODEC_Ext_StartRecord:error: || (NULL==pvArgv)"
#define DSP_CODEC_ADP_438_112_2_18_1_13_2_895 "dsp_codec_adp.c:DSP_CODEC_Ext_StartRecord:error:audio_obj == SCI_NULL"
#define DSP_CODEC_ADP_485_112_2_18_1_13_2_896 "dsp_codec_adp.c:DSP_CODEC_Ext_StopRecord:error:audio_obj == SCI_NULL"
#define DSP_CODEC_ADP_520_112_2_18_1_13_2_897 "dsp_codec_adp.c:DSP_CODEC_Ext_ReadVoiceData:error: || (NULL==pvArgv)"
#define DSP_CODEC_ADP_528_112_2_18_1_13_2_898 "dsp_codec_adp.c:DSP_CODEC_Ext_ReadVoiceData: PNULL==ptPara->source_buf_ptr"
#define DSP_CODEC_ADP_583_112_2_18_1_13_2_899 "dsp_codec_adp.c:DSP_CODEC_Ext_StartUpLoadData:ptAudioObject is NULL"
#define DSP_CODEC_ADP_592_112_2_18_1_13_2_900 "dsp_codec_adp.c:DSP_CODEC_Ext_StartUpLoadData:aud_framebuffer_new failed"
#define DSP_CODEC_ADP_602_112_2_18_1_13_2_901 "dsp_codec_adp.c:DSP_CODEC_Ext_StartUpLoadData:SCI_ALLOC_APP TEMP_FRAME_T failed"
#define DSP_CODEC_ADP_612_112_2_18_1_13_2_902 "dsp_codec_adp.c:DSP_CODEC_Ext_StartUpLoadData:SCI_ALLOC_APP TEMP_FRAME_T buffer failed"
#define DSP_CODEC_ADP_632_112_2_18_1_13_2_903 "dsp_codec_adp.c:DSP_CODEC_Ext_StartUpLoadData:SCI_CreateEvent failed"
#define DSP_CODEC_ADP_688_112_2_18_1_13_2_904 "dsp_codec_adp.c:DSP_CODEC_Ext_StopUpLoadData:error:audio_obj == SCI_NULL"
#define DSP_CODEC_ADP_720_112_2_18_1_13_2_905 "dsp_codec_adp.c:DSP_CODEC_Ext_StopRecord:error:audio_obj == SCI_NULL"
#define DSP_CODEC_ADP_760_112_2_18_1_13_2_906 "dsp_codec_adp.c:DSP_CODEC_IdentifyFormat:error:audio_obj == SCI_NULL"
#define DTMF_TONE_ADP_188_112_2_18_1_13_3_907 "DTMF_TONE_Construct SCI_CreateMutex pTonePlayMutex failed."
#define DTMF_TONE_ADP_198_112_2_18_1_13_3_908 "DTMF_TONE_Construct SCI_CreateMutex pToneStopMutex failed."
#define DTMF_TONE_ADP_208_112_2_18_1_13_3_909 "DTMF_TONE_Construct SCI_CreateTimer failed."
#define DTMF_TONE_ADP_228_112_2_18_1_13_3_910 "dtmf_tone_adp.c:DTMF_TONE_IdentifyFormat:error:PNULL != ptAudioObject"
#define DTMF_TONE_ADP_259_112_2_18_1_13_3_911 "dtmf_tone_adp.c:DTMF_TONE_Ext_Play:error:PNULL != ptAudioObject"
#define DTMF_TONE_ADP_266_112_2_18_1_13_3_912 "dtmf_tone_adp.c: DTMF_TONE_Ext_Play other task has been playing"
#define DTMF_TONE_ADP_274_112_2_18_1_13_3_913 "dtmf_tone_adp.c:DTMF_TONE_Ext_Play:error:PNULL == ptAudioObject"
#define DTMF_TONE_ADP_280_112_2_18_1_13_3_914 "dtmf_tone_adp.c:DTMF_TONE_Ext_Play:error:0==ptPara->duration."
#define DTMF_TONE_ADP_286_112_2_18_1_13_3_915 "dtmf_tone_adp.c:DTMF_TONE_Ext_Play: is playing."
#define DTMF_TONE_ADP_304_112_2_18_1_13_3_916 "[DTMF_TONE_Ext_Play] AUDIO_DM_Open failed!"
#define DTMF_TONE_ADP_317_112_2_18_1_13_3_917 "dtmf_tone_adp.c: DTMF_TONE_Ext_Play other task has been playing"
#define DTMF_TONE_ADP_356_112_2_18_1_13_3_918 "dtmf_tone_adp.c:DTMF_TONE_Ext_Stop:error:uiArgc != 0"
#define DTMF_TONE_ADP_363_112_2_18_1_13_3_919 "dtmf_tone_adp.c: DTMF_TONE_Ext_Stop other task has been stopping"
#define DTMF_TONE_ADP_368_112_2_18_1_13_3_920 "dtmf_tone_adp.c:DTMF_TONE_Ext_Stop: is not playing."
#define DTMF_TONE_ADP_377_112_2_18_1_13_3_921 "dtmf_tone_adp.c DTMF_TONE_Ext_Stop the tone is never played."
#define DTMF_TONE_ADP_383_112_2_18_1_13_3_922 "dtmf_tone_adp.c:DTMF_TONE_Ext_Stop:error:PNULL != ptAudioObject"
#define DTMF_TONE_ADP_389_112_2_18_1_13_3_923 "dtmf_tone_adp.c: DTMF_TONE_Ext_Stop other task has been stopping"
#define DTMF_TONE_ADP_416_112_2_18_1_13_3_924 "%s, %d. DTMF_TONE_Stop"
#define DTMF_TONE_ADP_428_112_2_18_1_13_4_925 "dtmf_tone_adp.c:DTMF_TONE_Stop:error:PNULL == ptAudioObject"
#define DTMF_TONE_ADP_435_112_2_18_1_13_4_926 "dtmf_tone_adp.c: DTMF_TONE_StopDeconstruct other task has been stopping"
#define DTMF_TONE_ADP_440_112_2_18_1_13_4_927 "dtmf_tone_adp.c:DTMF_TONE_StopDeconstruct: is not playing."
#define ADPCM_ENCODER_249_112_2_18_1_13_4_928 "error in IMA ADPCM encoding--low_4bits:%d, high_4bits:%d!\n"
#define ADPCM_ENCODER_357_112_2_18_1_13_4_929 "AdpcmEncoderSetSamplerate Error : %d"
#define ADPCM_ENCODER_451_112_2_18_1_13_5_930 "The header size of wav file for ADCPM is %d, not 60 bytes!\n"
#define ADPCM_ENCODE_96_112_2_18_1_13_5_931 "ADPCM_ENCode:adpcm_encode_open failed"
#define MP3_ENCODE_22_112_2_18_1_13_35_932 "mp3_encode.c:mp3_encode_start"
#define MP3_ENCODE_26_112_2_18_1_13_35_933 "peter: mp3_decode.c:MP3_ENC_MemoryAlloc:error"
#define MP3_ENCODE_45_112_2_18_1_13_36_934 "peter: mp3_encode_dec in frame->frame_len is %d"
#define MP3_ENCODE_53_112_2_18_1_13_36_935 "peter: mp3_encode_dec  result is %d,in frame_out->frame_len is %d"
#define MP3_ENCODE_100_112_2_18_1_13_36_936 "MP3_ENCode:mp3_encode_open failed"
#define GENERIC_TONE_ADP_287_112_2_18_1_13_37_937 "GENERIC_TONE_Construct SCI_CreateMutex pTonePlayMutex failed."
#define GENERIC_TONE_ADP_297_112_2_18_1_13_37_938 "GENERIC_TONE_Construct SCI_CreateMutex pToneStopMutex failed."
#define GENERIC_TONE_ADP_307_112_2_18_1_13_37_939 "GENERIC_TONE_Construct SCI_CreateTimer GenericToneTimer failed."
#define GENERIC_TONE_ADP_317_112_2_18_1_13_37_940 "GENERIC_TONE_Construct SCI_CreateTimer GenericDurationToneTimer failed."
#define GENERIC_TONE_ADP_395_112_2_18_1_13_37_941 "generic_tone_adp.c:GENERIC_TONE_IdentifyFormat:error:PNULL == ptAudioObject"
#define GENERIC_TONE_ADP_426_112_2_18_1_13_37_942 "generic_tone_adp.c:GENERIC_TONE_Ext_Play:error:NULL==pvArgv"
#define GENERIC_TONE_ADP_433_112_2_18_1_13_37_943 "generic_tone_adp.c: GENERIC_TONE_Ext_Play other task has been playing"
#define GENERIC_TONE_ADP_446_112_2_18_1_13_37_944 "generic_tone_adp.c:GENERIC_TONE_Ext_Play:error:PNULL == ptAudioObject"
#define GENERIC_TONE_ADP_453_112_2_18_1_13_37_945 "generic_tone_adp.c:GENERIC_TONE_Ext_Play: is playing."
#define GENERIC_TONE_ADP_471_112_2_18_1_13_37_946 "[GENERIC_TONE_Ext_Play] AUDIO_DM_Open failed!"
#define GENERIC_TONE_ADP_504_112_2_18_1_13_37_947 "generic_tone_adp.c:GENERIC_TONE_Ext_Stop:error:PNULL == ptAudioObject"
#define GENERIC_TONE_ADP_511_112_2_18_1_13_37_948 "generic_tone_adp.c: GENERIC_TONE_Ext_Stop other task has been stopping"
#define GENERIC_TONE_ADP_516_112_2_18_1_13_37_949 "generic_tone_adp.c:GENERIC_TONE_Ext_Stop: is not playing."
#define GENERIC_TONE_ADP_525_112_2_18_1_13_37_950 "generic_tone_adp.c:GENERIC_TONE_Ext_Stop the tone is never played."
#define GENERIC_TONE_ADP_534_112_2_18_1_13_37_951 "generic_tone_adp.c:GENERIC_TONE_Ext_Stop:error:PNULL == ptAudioObject"
#define GENERIC_TONE_ADP_560_112_2_18_1_13_37_952 "%s, %d. GENERIC_TONE_Stop"
#define GENERIC_TONE_ADP_574_112_2_18_1_13_37_953 "generic_tone_adp.c:GENERIC_TONE_Ext_Play:error:PNULL == ptAudioObject"
#define GENERIC_TONE_ADP_581_112_2_18_1_13_37_954 "generic_tone_adp.c: GENERIC_TONE_StopDeconstruct other task has been stopping"
#define GENERIC_TONE_ADP_586_112_2_18_1_13_37_955 "generic_tone_adp.c:GENERIC_TONE_StopDeconstruct: is not playing."
#define GENERIC_TONE_ADP_611_112_2_18_1_13_37_956 "AUD_PlayGenericTone: tone_id = %d,  duration = %d"
#define GENERIC_TONE_ADP_619_112_2_18_1_13_37_957 "generic_tone_adp.c:AUD_GenericTonePlay:error:NE_ID_MAX) || (0 == dura"
#define GENERIC_TONE_ADP_626_112_2_18_1_13_37_958 "generic_tone_adp.c:AUD_GenericTonePlay:error:type != AUD_SINGLE_TONE_RING"
#define GENERIC_TONE_ADP_633_112_2_18_1_13_37_959 "generic_tone_adp.c: GENERIC_TONE_Ext_Play other task has been playing"
#define GENERIC_TONE_ADP_674_112_2_18_1_13_37_960 "AUD_GenericToneStop: suiTonePlayState = %d !"
#define GENERIC_TONE_ADP_681_112_2_18_1_13_37_961 "generic_tone_adp.c:GENERIC_TONE_Ext_Play:error:PNULL == ptAudioObject"
#define GENERIC_TONE_ADP_688_112_2_18_1_13_37_962 "generic_tone_adp.c: GENERIC_TONE_Ext_Stop other task has been stopping"
#define MIC_SAMPLE_212_112_2_18_1_13_38_963 "mic_sample.c[MicSample_Play]: Have opened"
#define MIC_SAMPLE_229_112_2_18_1_13_38_964 "mic_sample.c[MicSample_Play]: MicSample Play!"
#define MIC_SAMPLE_252_112_2_18_1_13_38_965 "mic_sample.c[MicSample_Stop]: Have Closed"
#define MIC_SAMPLE_255_112_2_18_1_13_38_966 "mic_sample.c[MicSample_Stop]: MicSample_Stop!"
#define MIC_SAMPLE_270_112_2_18_1_13_38_967 "mic_sample.c: [MicSample_GetOutputSampleRate]:error:SCI_NULL == ptAudioObj"
#define MIC_SAMPLE_335_112_2_18_1_13_38_968 "mic_sample.c[MicSample_RegCodecPlugger]: register mic sample codec! codec handle is 0x%08x"
#define MIC_SAMPLE_374_112_2_18_1_13_39_969 "mic_sample.c[MicSample_GetMicdB]:  db_average=%d"
#define MIC_SAMPLE_382_112_2_18_1_13_39_970 "mic_sample.c[MicSample_GetMicdB]:  db_square_sum=%d"
#define MIC_SAMPLE_420_112_2_18_1_13_39_971 "mic_sample.c[MicSample_GetMic_dB]:  group_index=%d"
#define MIC_SAMPLE_439_112_2_18_1_13_39_972 "mic_sample.c[MicSample_GetMic_dB]:  get_db=%d"
#define MIC_SAMPLE_460_112_2_18_1_13_39_973 "mic_sample.c[AUD_GetMic_dB]:  rt_db=%d"
#define MIDI_ADP_532_112_2_18_1_14_14_974 "midi_adp.c:MIDI_Construct:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_546_112_2_18_1_14_14_975 "ERROR midi no memory 1."
#define MIDI_ADP_553_112_2_18_1_14_14_976 "ERROR midi no memory 2."
#define MIDI_ADP_561_112_2_18_1_14_14_977 "ERROR midi no memory 3."
#define MIDI_ADP_569_112_2_18_1_14_14_978 "ERROR midi no memory 4."
#define MIDI_ADP_579_112_2_18_1_14_14_979 "ERROR midi no memory 5."
#define MIDI_ADP_589_112_2_18_1_14_14_980 "ERROR midi no memory 6."
#define MIDI_ADP_661_112_2_18_1_14_14_981 "midi_adp.c:MIDI_Construct:error:stack_area) != (uint32)&Image"
#define MIDI_ADP_674_112_2_18_1_14_14_982 "ERROR midi no memory 7."
#define MIDI_ADP_687_112_2_18_1_14_14_983 "ERROR midi no memory 8."
#define MIDI_ADP_696_112_2_18_1_14_14_984 "ERROR midi no memory 9."
#define MIDI_ADP_706_112_2_18_1_14_14_985 "ERROR midi no memory 10."
#define MIDI_ADP_719_112_2_18_1_14_14_986 "ERROR midi no memory 11."
#define MIDI_ADP_727_112_2_18_1_14_14_987 "ERROR midi no memory 12."
#define MIDI_ADP_738_112_2_18_1_14_14_988 "midi_adp.c:MIDI_Construct:error:E * 4) != (uint32)&Image$"
#define MIDI_ADP_745_112_2_18_1_14_14_989 "ERROR midi no memory 13."
#define MIDI_ADP_760_112_2_18_1_14_14_990 "ERROR midi no memory 14."
#define MIDI_ADP_773_112_2_18_1_14_14_991 "midi_adp.c [MIDI_Construct] psPcmL:0x%x,len:0x%x(bytes)."
#define MIDI_ADP_777_112_2_18_1_14_14_992 "midi_adp.c [MIDI_Construct] psPcmR:0x%x."
#define MIDI_ADP_785_112_2_18_1_14_14_993 "ERROR midi no memory 15.! len:%d."
#define MIDI_ADP_794_112_2_18_1_14_14_994 "midi_adp.c [MIDI_Construct] psExpProDataL:0x%x,len:0x%x(bytes)."
#define MIDI_ADP_798_112_2_18_1_14_14_995 "midi_adp.c [MIDI_Construct] psExpProDataR:0x%x."
#define MIDI_ADP_803_112_2_18_1_14_14_996 "ERROR midi no memory  16! len:%d."
#define MIDI_ADP_829_112_2_18_1_14_14_997 "midi_adp.c [MIDI_Construct]  create midi assistant dynamic task!"
#define MIDI_ADP_834_112_2_18_1_14_14_998 "midi_adp.c [MIDI_Construct]  can not create midi assistant dynamic task!"
#define MIDI_ADP_884_112_2_18_1_14_14_999 "midi_adp.c:MIDI_Deconstruct:error:ptMidiProcRes==PNULL"
#define MIDI_ADP_893_112_2_18_1_14_14_1000 "midi_adp.c:MIDI_Deconstruct:error:tpDspProRes==PNULL"
#define MIDI_ADP_963_112_2_18_1_14_14_1001 "midi_adp.c:MIDI_PlayConstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_990_112_2_18_1_14_14_1002 "midi_adp.c:MIDI_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_midi_chng_freq_req_handler"
#define MIDI_ADP_996_112_2_18_1_14_14_1003 "midi_adp.c,[MIDI_PlayConstruct] CHNG_FREQ_SetArmClk()!"
#define MIDI_ADP_1061_112_2_18_1_14_15_1004 "midi_adp.c,[MIDI_PlayConstruct]set bus monitor!"
#define MIDI_ADP_1078_112_2_18_1_14_15_1005 "midi_adp.c,[MIDI_PlayConstruct]set bus monitor!"
#define MIDI_ADP_1114_112_2_18_1_14_15_1006 "midi_adp.c:MIDI_ResumeConstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1141_112_2_18_1_14_15_1007 "midi_adp.c:MIDI_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_midi_chng_freq_req_handler"
#define MIDI_ADP_1147_112_2_18_1_14_15_1008 "midi_adp.c,[MIDI_ResumeConstruct] CHNG_FREQ_SetArmClk()!"
#define MIDI_ADP_1181_112_2_18_1_14_15_1009 "midi_adp.c:MIDI_PauseDeconstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1229_112_2_18_1_14_15_1010 "midi_adp.c:MIDI_StopDeconstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1312_112_2_18_1_14_15_1011 "midi_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1331_112_2_18_1_14_15_1012 "MIDI_Stop"
#define MIDI_ADP_1337_112_2_18_1_14_15_1013 "midi_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1369_112_2_18_1_14_15_1014 "midi_adp.c:MIDI_Pause:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1395_112_2_18_1_14_15_1015 "midi_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1469_112_2_18_1_14_16_1016 "midi_adp.c:MIDI_IdentifyFormat:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1478_112_2_18_1_14_16_1017 "midi_adp.c:MIDI_IdentifyFormat:error:PNULL == mididata"
#define MIDI_ADP_1514_112_2_18_1_14_16_1018 "midi_adp.c:Midi_IdentifyFormat:error:Unknown Handle!"
#define MIDI_ADP_1562_112_2_18_1_14_16_1019 "MIDI_IdentifyFormat: Pre checking ERROR! track %d is out of range"
#define MIDI_ADP_1569_112_2_18_1_14_16_1020 "MIDI_IdentifyFormat: Track %d ERROR. MTrk not found"
#define MIDI_ADP_1587_112_2_18_1_14_16_1021 "MIDI_IdentifyFormat: ERROR! track %d is out of range"
#define MIDI_ADP_1606_112_2_18_1_14_16_1022 "[MIDI_IdentifyFormat] ptAudioObject->tAudioFormatInfo.uiTotalTime == 0!"
#define MIDI_ADP_1610_112_2_18_1_14_16_1023 "[MIDI_IdentifyFormat] ptAudioObject->tAudioFormatInfo.uiTotalTime return error!"
#define MIDI_ADP_1695_112_2_18_1_14_16_1024 "MIDI_GetSampleRate outSamplerate:%d."
#define MIDI_ADP_1732_112_2_18_1_14_16_1025 "midi_adp.c:MIDI_SetPolyNum:error:audio_obj == SCI_NULL"
#define MIDI_ADP_2085_112_2_18_1_14_17_1026 "MIDI_Thread_Entry CODEC_SendSignal CODEC_STOP_REQ"
#define MIDI_ADP_2128_112_2_18_1_14_17_1027 "MidiPlayFile ERROR MIDI_SC_Init:%d."
#define MIDI_ADP_2138_112_2_18_1_14_17_1028 "MidiPlayFile ERROR MIDI_SC_Play:%d."
#define MIDI_ADP_2205_112_2_18_1_14_17_1029 "offset(%d) is bigger than file_size(%d)."
#define MIDI_ADP_2226_112_2_18_1_14_17_1030 "ERROR s_midi_data_buffer no memory."
#define MIDI_ADP_2239_112_2_18_1_14_17_1031 "ERROR FFS_Read, %d, %d, %d."
#define MIDI_ADP_2248_112_2_18_1_14_17_1032 "ERROR FFS_fseek, %d, %d."
#define MIDI_ADP_2259_112_2_18_1_14_17_1033 "midi not support streambuffer play."
#define MIDI_ADP_2264_112_2_18_1_14_17_1034 "MidiGetSrcData Invalid Handle Type."
#define MIDI_ADP_2300_112_2_18_1_14_17_1035 "midi_adp.c:Write_MIDIDecOutputBuf:error, l:0x%x, r:0x%x."
#define MIDI_ADP_2316_112_2_18_1_14_17_1036 "MIDI:BUSY!"
#define MIDI_ADP_2323_112_2_18_1_14_17_1037 "MIDI:FREE!"
#define MIDI_ADP_2341_112_2_18_1_14_17_1038 "Write_MIDIDecOutputBuf 0 discard pcm data count:%d."
#define MIDI_ADP_2347_112_2_18_1_14_17_1039 "CODEC_SendSignal CODEC_ASSI_PLAY_REQ in write buf."
#define MIDI_ADP_2368_112_2_18_1_14_17_1040 "Write_MIDIDecOutputBuf 1 discard pcm data count:%d."
#define MIDI_ADP_2483_112_2_18_1_14_18_1041 "Write_MIDIDecOutputBuf 2 discard pcm data count:%d."
#define MIDI_ADP_2495_112_2_18_1_14_18_1042 "Write_MIDIDecOutputBuf dataLen ready to Dsp:%d"
#define MIDI_ADP_2550_112_2_18_1_14_18_1043 "midi_adp.c: error::AUDIO_DEFAULT_TASK_ID == tThreadId"
#define MIDI_ADP_2564_112_2_18_1_14_18_1044 "midi_adp.c [CODEC_SendSignal]-0-  alloc failed!"
#define MIDI_ADP_2580_112_2_18_1_14_18_1045 "midi_adp.c [CODEC_SendSignal]-01-  alloc failed!"
#define MIDI_ADP_2596_112_2_18_1_14_18_1046 "midi_adp.c [CODEC_SendSignal]-1-  alloc failed!"
#define MIDI_ADP_2612_112_2_18_1_14_18_1047 "midi_adp.c [CODEC_SendSignal]-2-  alloc failed!"
#define MIDI_ADP_2628_112_2_18_1_14_18_1048 "midi_adp.c [CODEC_SendSignal]-9-  alloc failed!"
#define MIDI_ADP_2636_112_2_18_1_14_18_1049 "midi_adp.c [CODEC_SendSignal]CODEC_ASSI_PLAY_REQ from 0x%x to 0x%x."
#define MIDI_ADP_2647_112_2_18_1_14_18_1050 "midi_adp.c [CODEC_SendSignal]-10-  alloc failed!"
#define MIDI_ADP_2663_112_2_18_1_14_18_1051 "midi_adp.c [CODEC_SendSignal]-11-  alloc failed!"
#define MIDI_ADP_2679_112_2_18_1_14_18_1052 "midi_adp.c [CODEC_SendSignal]-12-  alloc failed!"
#define MIDI_ADP_2692_112_2_18_1_14_18_1053 "midi_adp.c  [CODEC_SendSignal]error eSigCode:0x%08x!"
#define MIDI_ADP_2761_112_2_18_1_14_18_1054 "midi_adp.c[CODEC_AssiTask_DspExpPros] begin to dsp express process wav!"
#define MIDI_ADP_2831_112_2_18_1_14_18_1055 "CODEC_ASSI_STOP_REQ force to exit from pausing."
#define MIDI_ADP_2844_112_2_18_1_14_18_1056 "midi_adp.c CODEC_AssiTask_DspExpPros midi decode has stopped."
#define MIDI_ADP_2853_112_2_18_1_14_18_1057 "midi_adp.c CODEC_AssiTask_DspExpPros midi decode exit from pausing state."
#define MIDI_ADP_2862_112_2_18_1_14_18_1058 "midi_adp.c CODEC_AssiTask_DspExpPros MIDI_SC_Stop."
#define MIDI_ADP_2869_112_2_18_1_14_18_1059 "midi_adp.c CODEC_AssiTask_DspExpPros exit."
#define MIDI_ADP_2878_112_2_18_1_14_18_1060 "midi_adp.c CODEC_AssiTask_DspExpPros stop waiting %d."
#define MIDI_ADP_2884_112_2_18_1_14_18_1061 "CODEC_ASSI_STOP_REQ force to exit from stopping."
#define MIDI_ADP_2899_112_2_18_1_14_18_1062 "CODEC_ASSI_PLAY_REQ receive again sender:0x%x state:%d."
#define MIDI_ADP_2932_112_2_18_1_14_18_1063 "CODEC_ASSI_STOP_REQ wait to get dsp ind too long."
#define MIDI_ADP_2950_112_2_18_1_14_19_1064 "midi decode task %d."
#define MIDI_ADP_2974_112_2_18_1_14_19_1065 "CODEC_ASSI_DSP_EXP_PRO_REQ wait to get pcm data too long."
#define MIDI_ADP_3011_112_2_18_1_14_19_1066 "CODEC_ASSI LAYER1_AUDIO_CODEC_SetData 1 dest_addr:0x%x, src:0x%x: 0x%x + outIndex:0x%x, len:0x%x."
#define MIDI_ADP_3026_112_2_18_1_14_19_1067 "CODEC_ASSI LAYER1_AUDIO_CODEC_CopyData 2 dest_addr:0x%x, src:0x%x: 0x%x + outIndex:0x%x, len:0x%x."
#define MIDI_ADP_3080_112_2_18_1_14_19_1068 "CODEC_ASSI LAYER1_AUDIO_CODEC_CopyData 3 dest_addr:0x%x, src:0x%x=0x%x+outIndex:0x%x, len:0x%x."
#define MIDI_ADP_3094_112_2_18_1_14_19_1069 "CODEC_ASSI LAYER1_AUDIO_CODEC_CopyData 4 dest_addr:0x%x, 0x%x, len:0x%x."
#define MIDI_ADP_3109_112_2_18_1_14_19_1070 "CODEC_ASSI LAYER1_AUDIO_CODEC_CopyData 5 dest_addr:0x%x, src:0x%x=0x%x+outIndex:0x%x, len:0x%x."
#define MIDI_ADP_3118_112_2_18_1_14_19_1071 "CODEC_ASSI LAYER1_AUDIO_CODEC_CopyData 6 dest_addr:0x%x, 0x%x, len:0x%x."
#define MIDI_ADP_3142_112_2_18_1_14_19_1072 "CODEC_ASSI_DSP_EXP_PRO_REQ sample:%d, src samplerate:%d, time:%d."
#define MIDI_ADP_3153_112_2_18_1_14_19_1073 "CODEC_ASSI_DSP_EXP_PRO_REQ exit from tran of dsp data process."
#define MIDI_ADP_3158_112_2_18_1_14_19_1074 "CODEC_ASSI_DSP_EXP_PRO_REQ exit from sbc or tran of dsp data process."
#define MIDI_ADP_3168_112_2_18_1_14_19_1075 "midi decode task %d."
#define MIDI_ADP_3185_112_2_18_1_14_19_1076 "CODEC_ASSI_DSP_EXP_PRO_IND sample:%d, time:%d."
#define MIDI_ADP_3220_112_2_18_1_14_19_1077 "midi_adp.c [CODEC_AssiTask_DspExpPros]other invalid signal code:%d!"
#define MIDI_API_130_112_2_18_1_14_20_1078 "MIDI_Play: ring_type = %d, ring_data_len = %d, ring_data_ptr = %d, play_times = %d!"
#define MIDI_API_140_112_2_18_1_14_20_1079 "MIDI_Play: It does not support this ring type!"
#define MIDI_API_256_112_2_18_1_14_20_1080 "MidiPlayFile: Playing midi."
#define MIDI_API_347_112_2_18_1_14_20_1081 "\r\nWrite_MIDIDecOutputBuf: midi is stopped"
#define MIDI_API_373_112_2_18_1_14_20_1082 "MIDI:BUSY!"
#define MIDI_API_379_112_2_18_1_14_20_1083 "MIDI:FREE!"
#define MIDI_SC_API_375_112_2_18_1_14_29_1084 "midi_sc_api.c:MIDI_SC_Init:error:PNULL==sound_bank"
#define MIDI_SC_API_476_112_2_18_1_14_29_1085 "midi_sc_api.c:MIDI_SC_Decoder_Lib_Init:error:PNULL==pt_midi_proc_res"
#define MIDI_SC_API_554_112_2_18_1_14_29_1086 "midi_sc_api.c, Init midi lib for L1"
#define MIDI_SC_API_564_112_2_18_1_14_29_1087 "midi_sc_api.c, Init midi lib for L2/H3/W2"
#define MIDI_SC_API_575_112_2_18_1_14_29_1088 "midi_sc_api Get IRAM Res Fail!"
#define MIDI_SC_API_604_112_2_18_1_14_29_1089 "s_midi_ilock_handle is in unlocking."
#define MIDI_SC_API_616_112_2_18_1_14_29_1090 "\r\nMIDI_SC_DeInit:unable to unlock IRAM"
#define MIDI_SC_API_652_112_2_18_1_14_29_1091 "midi_sc_api.c:MIDI_SC_Play:error:PNULL==midi_addr"
#define MIDI_SC_API_681_112_2_18_1_14_29_1092 "Play midi time : %d\n"
#define MIDI_SC_API_695_112_2_18_1_14_29_1093 "MIDI_SC_Stop: bIsMidiLibInited %d\n"
#define MIDI_SC_API_809_112_2_18_1_14_29_1094 "MIDI_SC_GetPlayTime: Enter"
#define MIDI_SC_API_815_112_2_18_1_14_29_1095 "midi_sc_api.c:MIDI_SC_GetPlayTime:error:PNULL==midi_addr"
#define MIDI_SC_API_832_112_2_18_1_14_30_1096 "Scan midi time : %d, Midi Sample:%d"
#define MIDI_SC_API_836_112_2_18_1_14_30_1097 "[MIDI_SC_GetPlayTime] error occours when scanniing the whole midi file!"
#define MIDI_SC_API_1143_112_2_18_1_14_30_1098 "midi_sc_api.c,[MIDI_SC_SetVibrator]%d,%d,%d"
#define MIDI_ADP_580_112_2_18_1_14_32_1099 "midi_adp.c:MIDI_Construct:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_601_112_2_18_1_14_32_1100 "ERROR midi no memory 1."
#define MIDI_ADP_607_112_2_18_1_14_32_1101 "MIDI_Construct memmory:%d."
#define MIDI_ADP_613_112_2_18_1_14_32_1102 "ERROR midi no memory 2."
#define MIDI_ADP_619_112_2_18_1_14_32_1103 "MIDI_Construct memmory:%d."
#define MIDI_ADP_625_112_2_18_1_14_32_1104 "ERROR midi no memory 3."
#define MIDI_ADP_631_112_2_18_1_14_32_1105 "MIDI_Construct memmory:%d."
#define MIDI_ADP_637_112_2_18_1_14_32_1106 "ERROR midi no memory 4."
#define MIDI_ADP_643_112_2_18_1_14_32_1107 "MIDI_Construct memmory:%d."
#define MIDI_ADP_650_112_2_18_1_14_32_1108 "ERROR midi no memory 5."
#define MIDI_ADP_656_112_2_18_1_14_32_1109 "MIDI_Construct memmory:%d."
#define MIDI_ADP_664_112_2_18_1_14_32_1110 "ERROR midi no memory 6."
#define MIDI_ADP_670_112_2_18_1_14_32_1111 "MIDI_Construct memmory:%d."
#define MIDI_ADP_687_112_2_18_1_14_32_1112 "midi_adp.c:, Open(), midi construct failed to SCI_CreateMutex"
#define MIDI_ADP_695_112_2_18_1_14_32_1113 "midi_adp.c:, Open(), midi construct failed to SCI_CreateMutex"
#define MIDI_ADP_719_112_2_18_1_14_32_1114 "ERROR midi no memory 12."
#define MIDI_ADP_725_112_2_18_1_14_32_1115 "MIDI_Construct memmory:0x%x."
#define MIDI_ADP_737_112_2_18_1_14_32_1116 "Can't create midi"
#define MIDI_ADP_768_112_2_18_1_14_32_1117 "ERROR midi no memory 13."
#define MIDI_ADP_774_112_2_18_1_14_32_1118 "MIDI_Construct memmory:%d."
#define MIDI_ADP_783_112_2_18_1_14_32_1119 "MIDI_Construct memmory:%d."
#define MIDI_ADP_787_112_2_18_1_14_32_1120 "ERROR midi no memory 14."
#define MIDI_ADP_798_112_2_18_1_14_32_1121 "midi_adp.c [MIDI_Construct] psPcmL:0x%x,len:%d(bytes)."
#define MIDI_ADP_802_112_2_18_1_14_32_1122 "midi_adp.c [MIDI_Construct] psPcmR:0x%x."
#define MIDI_ADP_809_112_2_18_1_14_32_1123 "ERROR midi no memory 15.! len:%d."
#define MIDI_ADP_819_112_2_18_1_14_32_1124 "midi_adp.c [MIDI_Construct] psExpProDataL:0x%x,len:0x%x(bytes)."
#define MIDI_ADP_823_112_2_18_1_14_32_1125 "midi_adp.c [MIDI_Construct] psExpProDataR:0x%x."
#define MIDI_ADP_827_112_2_18_1_14_32_1126 "ERROR midi no memory  16! len:%d."
#define MIDI_ADP_854_112_2_18_1_14_32_1127 "midi_adp.c [MIDI_Construct]  create midi assistant dynamic task!"
#define MIDI_ADP_858_112_2_18_1_14_32_1128 "midi_adp.c [MIDI_Construct]  can not create midi assistant dynamic task!"
#define MIDI_ADP_908_112_2_18_1_14_32_1129 "midi_adp.c:MIDI_Deconstruct:error:ptMidiProcRes==PNULL"
#define MIDI_ADP_968_112_2_18_1_14_33_1130 "midi_adp.c:MIDI_PlayConstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_994_112_2_18_1_14_33_1131 "midi_adp.c:MIDI_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL"
#define MIDI_ADP_1000_112_2_18_1_14_33_1132 "midi_adp.c,[MIDI_PlayConstruct] CHNG_FREQ_SetArmClk()!"
#define MIDI_ADP_1081_112_2_18_1_14_33_1133 "midi_adp.c:MIDI_ResumeConstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1097_112_2_18_1_14_33_1134 "midi_adp.c:MIDI_ResumeConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL"
#define MIDI_ADP_1102_112_2_18_1_14_33_1135 "midi_adp.c,[MIDI_ResumeConstruct] CHNG_FREQ_SetArmClk()!"
#define MIDI_ADP_1138_112_2_18_1_14_33_1136 "midi_adp.c:MIDI_PauseDeconstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1182_112_2_18_1_14_33_1137 "midi_adp.c:MIDI_StopDeconstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1189_112_2_18_1_14_33_1138 "midi_adp.c:MIDI_StopDeconstruct:error:ptMidiProcRes==PNULL"
#define MIDI_ADP_1268_112_2_18_1_14_33_1139 "midi_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1295_112_2_18_1_14_33_1140 "midi_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1328_112_2_18_1_14_33_1141 "midi_adp.c:MIDI_Pause:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1356_112_2_18_1_14_33_1142 "midi_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1390_112_2_18_1_14_33_1143 "midi_adp.c:MIDI_Seek:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1395_112_2_18_1_14_33_1144 "midi_adp.c:MIDI_Seek:error:uiOffset > uiTotalDataLength"
#define MIDI_ADP_1422_112_2_18_1_14_33_1145 "midi_adp.c:MIDI_SetSpeed:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1467_112_2_18_1_14_34_1146 "midi_adp.c:MIDI_IdentifyFormat:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1474_112_2_18_1_14_34_1147 "midi_adp.c:MIDI_IdentifyFormat:error:ptMidiProcRes==PNULL"
#define MIDI_ADP_1483_112_2_18_1_14_34_1148 "midi_adp.c:MIDI_IdentifyFormat:error:PNULL == mididata"
#define MIDI_ADP_1519_112_2_18_1_14_34_1149 "midi_adp.c:Midi_IdentifyFormat:error:Unknown Handle!"
#define MIDI_ADP_1567_112_2_18_1_14_34_1150 "[MIDI_IdentifyFormat] ptAudioObject->tAudioFormatInfo.uiTotalTime == 0!"
#define MIDI_ADP_1653_112_2_18_1_14_34_1151 "MIDI_GetSampleRate outSamplerate:%d."
#define MIDI_ADP_1693_112_2_18_1_14_34_1152 "midi_adp.c:MIDI_SetPolyNum:error:audio_obj == SCI_NULL"
#define MIDI_ADP_1701_112_2_18_1_14_34_1153 "midi_adp.c:MIDI_Set_Stereo: stereo  is %d"
#define MIDI_ADP_1738_112_2_18_1_14_34_1154 "midi_adp.c:MIDI_Set_Stereo:unknown soundbank type!"
#define MIDI_ADP_1772_112_2_18_1_14_34_1155 "midi_adp.c:MIDI_SetPolyNum:error:audio_obj == SCI_NULL"
#define MIDI_ADP_1789_112_2_18_1_14_34_1156 "midi_adp.c:MIDI_SetPolyNum: poly number  is %d"
#define MIDI_ADP_2238_112_2_18_1_14_35_1157 "midi_adp.c:MIDI_Set_Stereo:unknown soundbank type!"
#define MIDI_ADP_2291_112_2_18_1_14_35_1158 "MIDI_Thread_Entry CODEC_SendSignal CODEC_STOP_REQ"
#define MIDI_ADP_2352_112_2_18_1_14_35_1159 "MidiPlayFile ERROR found:%d."
#define MIDI_ADP_2424_112_2_18_1_14_35_1160 "offset(%d) is bigger than file_size(%d)."
#define MIDI_ADP_2445_112_2_18_1_14_35_1161 "ERROR s_midi_data_buffer no memory."
#define MIDI_ADP_2458_112_2_18_1_14_35_1162 "ERROR FFS_Read, %d, %d, %d."
#define MIDI_ADP_2467_112_2_18_1_14_35_1163 "ERROR FFS_fseek, %d, %d."
#define MIDI_ADP_2478_112_2_18_1_14_35_1164 "midi not support streambuffer play."
#define MIDI_ADP_2483_112_2_18_1_14_36_1165 "MidiGetSrcData Invalid Handle Type."
#define MIDI_ADP_2514_112_2_18_1_14_36_1166 "midi_adp.c:Write_MIDIDecOutputBuf:error, l:0x%x, r:0x%x."
#define MIDI_ADP_2523_112_2_18_1_14_36_1167 "MIDI:BUSY!"
#define MIDI_ADP_2530_112_2_18_1_14_36_1168 "MIDI:FREE!"
#define MIDI_ADP_2543_112_2_18_1_14_36_1169 "MIDI:BUSY!"
#define MIDI_ADP_2550_112_2_18_1_14_36_1170 "MIDI:FREE!"
#define MIDI_ADP_2568_112_2_18_1_14_36_1171 "Write_MIDIDecOutputBuf 0 discard pcm data count:%d."
#define MIDI_ADP_2574_112_2_18_1_14_36_1172 "CODEC_SendSignal CODEC_ASSI_PLAY_REQ in write buf."
#define MIDI_ADP_2595_112_2_18_1_14_36_1173 "Write_MIDIDecOutputBuf 1 discard pcm data count:%d."
#define MIDI_ADP_2677_112_2_18_1_14_36_1174 "Write_MIDIDecOutputBuf 2 discard pcm data count:%d."
#define MIDI_ADP_2687_112_2_18_1_14_36_1175 "Write_MIDIDecOutputBuf dataLen ready to Dsp:%d"
#define MIDI_ADP_2735_112_2_18_1_14_36_1176 "MIDIGetPlayTime: Enter"
#define MIDI_ADP_2742_112_2_18_1_14_36_1177 "Scanmidi time :%d, play time:%dms"
#define MIDI_ADP_2746_112_2_18_1_14_36_1178 "[MIDI_SC_GetPlayTime] error occours when scanniing the whole midi file!"
#define MIDI_ADP_2800_112_2_18_1_14_36_1179 "midi_adp.c,[MIDI_VibratorCallback]%d,%d,%d"
#define MIDI_ADP_2824_112_2_18_1_14_36_1180 "midi_adp.c: error::AUDIO_DEFAULT_TASK_ID == tAssiThreadId"
#define MIDI_ADP_2838_112_2_18_1_14_36_1181 "midi_adp.c [CODEC_SendSignal]-0-  alloc failed!"
#define MIDI_ADP_2854_112_2_18_1_14_36_1182 "midi_adp.c [CODEC_SendSignal]-01-  alloc failed!"
#define MIDI_ADP_2870_112_2_18_1_14_36_1183 "midi_adp.c [CODEC_SendSignal]-1-  alloc failed!"
#define MIDI_ADP_2886_112_2_18_1_14_36_1184 "midi_adp.c [CODEC_SendSignal]-2-  alloc failed!"
#define MIDI_ADP_2902_112_2_18_1_14_36_1185 "midi_adp.c [CODEC_SendSignal]-9-  alloc failed!"
#define MIDI_ADP_2910_112_2_18_1_14_36_1186 "midi_adp.c [CODEC_SendSignal]CODEC_ASSI_PLAY_REQ from 0x%x to 0x%x."
#define MIDI_ADP_2921_112_2_18_1_14_36_1187 "midi_adp.c [CODEC_SendSignal]-10-  alloc failed!"
#define MIDI_ADP_2937_112_2_18_1_14_36_1188 "midi_adp.c [CODEC_SendSignal]-11-  alloc failed!"
#define MIDI_ADP_2953_112_2_18_1_14_36_1189 "midi_adp.c [CODEC_SendSignal]-12-  alloc failed!"
#define MIDI_ADP_2966_112_2_18_1_14_36_1190 "midi_adp.c  [CODEC_SendSignal]error eSigCode:0x%08x!"
#define MIDI_ADP_3035_112_2_18_1_14_37_1191 "midi_adp.c[CODEC_AssiTask_DspExpPros] begin to dsp express process wav!"
#define MIDI_ADP_3133_112_2_18_1_14_37_1192 "CODEC_ASSI_STOP_REQ force to exit from pausing."
#define MIDI_ADP_3146_112_2_18_1_14_37_1193 "midi_adp.c CODEC_AssiTask_DspExpPros midi decode has stopped."
#define MIDI_ADP_3156_112_2_18_1_14_37_1194 "midi_adp.c CODEC_AssiTask_DspExpPros midi decode exit from pausing state."
#define MIDI_ADP_3165_112_2_18_1_14_37_1195 "midi_adp.c CODEC_AssiTask_DspExpPros MIDI_SC_Stop."
#define MIDI_ADP_3173_112_2_18_1_14_37_1196 "midi_adp.c CODEC_AssiTask_DspExpPros exit."
#define MIDI_ADP_3182_112_2_18_1_14_37_1197 "midi_adp.c CODEC_AssiTask_DspExpPros stop waiting %d."
#define MIDI_ADP_3188_112_2_18_1_14_37_1198 "CODEC_ASSI_STOP_REQ force to exit from stopping."
#define MIDI_ADP_3203_112_2_18_1_14_37_1199 "CODEC_ASSI_PLAY_REQ receive again sender:0x%x state:%d."
#define MIDI_ADP_3236_112_2_18_1_14_37_1200 "CODEC_ASSI_STOP_REQ wait to get dsp ind too long."
#define MIDI_ADP_3252_112_2_18_1_14_37_1201 "midi decode task %d."
#define MIDI_ADP_3270_112_2_18_1_14_37_1202 "CODEC_ASSI_DSP_EXP_PRO_REQ wait %d."
#define MIDI_ADP_3277_112_2_18_1_14_37_1203 "CODEC_ASSI_DSP_EXP_PRO_REQ wait to get pcm data too long."
#define MIDI_ADP_3445_112_2_18_1_14_37_1204 "CODEC_ASSI_DSP_EXP_PRO_REQ sample:%d, src samplerate:%d, time:%d."
#define MIDI_ADP_3456_112_2_18_1_14_37_1205 "CODEC_ASSI_DSP_EXP_PRO_REQ exit from tran of dsp data process."
#define MIDI_ADP_3461_112_2_18_1_14_37_1206 "CODEC_ASSI_DSP_EXP_PRO_REQ exit from sbc or tran of dsp data process."
#define MIDI_ADP_3471_112_2_18_1_14_37_1207 "midi decode task %d."
#define MIDI_ADP_3488_112_2_18_1_14_37_1208 "CODEC_ASSI_DSP_EXP_PRO_IND sample:%d, time:%d."
#define MIDI_ADP_3523_112_2_18_1_14_38_1209 "midi_adp.c [CODEC_AssiTask_DspExpPros]other invalid signal code:%d!"
#define MIDI_OPT_ADP_339_112_2_18_1_14_39_1210 "MidiPlayFile ERROR MIDI_SC_Play:%d."
#define MIDI_OPT_ADP_374_112_2_18_1_14_39_1211 "MIDI_Thread_Entry CODEC_SendSignal CODEC_STOP_REQ"
#define MIDI_OPT_ADP_425_112_2_18_1_14_39_1212 "Write_MIDIDecOutputBuf 0 discard pcm data count:%d."
#define MIDI_OPT_ADP_431_112_2_18_1_14_39_1213 "CODEC_SendSignal CODEC_ASSI_PLAY_REQ in write buf."
#define MIDI_OPT_ADP_452_112_2_18_1_14_39_1214 "Write_MIDIDecOutputBuf 1 discard pcm data count:%d."
#define MIDI_OPT_ADP_534_112_2_18_1_14_39_1215 "Write_MIDIDecOutputBuf 2 discard pcm data count:%d."
#define MIDI_OPT_ADP_544_112_2_18_1_14_39_1216 "Write_MIDIDecOutputBuf dataLen ready to Dsp:%d"
#define MIDI_OPT_ADP_608_112_2_18_1_14_39_1217 "offset(%d) is bigger than file_size(%d)."
#define MIDI_OPT_ADP_629_112_2_18_1_14_40_1218 "ERROR s_midi_data_buffer no memory."
#define MIDI_OPT_ADP_646_112_2_18_1_14_40_1219 "ERROR FFS_Read, %d, %d, %d."
#define MIDI_OPT_ADP_655_112_2_18_1_14_40_1220 "ERROR FFS_fseek, %d, %d."
#define MIDI_OPT_ADP_666_112_2_18_1_14_40_1221 "midi not support streambuffer play."
#define MIDI_OPT_ADP_671_112_2_18_1_14_40_1222 "MidiGetSrcData Invalid Handle Type %d."
#define MIDI_OPT_ADP_701_112_2_18_1_14_40_1223 "midi_opt_adp.c:MIDI_Construct:error:SCI_NULL == ptAudioObject"
#define MIDI_OPT_ADP_721_112_2_18_1_14_40_1224 "ERROR midi no memory 1."
#define MIDI_OPT_ADP_727_112_2_18_1_14_40_1225 "midi_opt_adp.c MIDI_Construct allocLen:%d."
#define MIDI_OPT_ADP_734_112_2_18_1_14_40_1226 "ERROR midi no memory 2."
#define MIDI_OPT_ADP_740_112_2_18_1_14_40_1227 "midi_opt_adp.c MIDI_Construct allocLen:%d."
#define MIDI_OPT_ADP_747_112_2_18_1_14_40_1228 "ERROR midi no memory 3."
#define MIDI_OPT_ADP_753_112_2_18_1_14_40_1229 "midi_opt_adp.c MIDI_Construct allocLen:%d."
#define MIDI_OPT_ADP_760_112_2_18_1_14_40_1230 "ERROR midi no memory 4."
#define MIDI_OPT_ADP_766_112_2_18_1_14_40_1231 "midi_opt_adp.c MIDI_Construct allocLen:%d."
#define MIDI_OPT_ADP_773_112_2_18_1_14_40_1232 "ERROR midi no memory 5."
#define MIDI_OPT_ADP_779_112_2_18_1_14_40_1233 "midi_opt_adp.c MIDI_Construct allocLen:%d."
#define MIDI_OPT_ADP_786_112_2_18_1_14_40_1234 "ERROR midi no memory 6."
#define MIDI_OPT_ADP_792_112_2_18_1_14_40_1235 "midi_opt_adp.c MIDI_Construct allocLen:%d."
#define MIDI_OPT_ADP_804_112_2_18_1_14_40_1236 "midi_opt_adp.c MIDI_Construct mode:%d."
#define MIDI_OPT_ADP_811_112_2_18_1_14_40_1237 "midi_opt_adp.c MIDI_Construct alloc failed, size:%d."
#define MIDI_OPT_ADP_817_112_2_18_1_14_40_1238 "ERROR midi failed to get mode param."
#define MIDI_OPT_ADP_848_112_2_18_1_14_40_1239 "midi_opt_adp.c,[MIDI_Construct]:midi_control:0x%x,samplerate:%d,polynum:%d"
#define MIDI_OPT_ADP_886_112_2_18_1_14_40_1240 "midi_opt_adp.c MIDI_Construct allocLen:%d."
#define MIDI_OPT_ADP_890_112_2_18_1_14_40_1241 "ERROR midi no memory 7."
#define MIDI_OPT_ADP_900_112_2_18_1_14_40_1242 "midi_opt_adp.c MIDI_Construct allocLen:%d."
#define MIDI_OPT_ADP_902_112_2_18_1_14_40_1243 "midi_opt_adp.c [MIDI_Construct] psPcmL:0x%x,len:%d(bytes)."
#define MIDI_OPT_ADP_906_112_2_18_1_14_40_1244 "midi_opt_adp.c [MIDI_Construct] psPcmR:0x%x."
#define MIDI_OPT_ADP_913_112_2_18_1_14_40_1245 "ERROR midi no memory 8.! len:%d."
#define MIDI_OPT_ADP_922_112_2_18_1_14_40_1246 "midi_opt_adp.c MIDI_Construct allocLen:%d."
#define MIDI_OPT_ADP_924_112_2_18_1_14_40_1247 "midi_opt_adp.c [MIDI_Construct] psExpProDataL:0x%x,len:0x%x(bytes)."
#define MIDI_OPT_ADP_928_112_2_18_1_14_40_1248 "midi_opt_adp.c [MIDI_Construct] psExpProDataR:0x%x."
#define MIDI_OPT_ADP_932_112_2_18_1_14_40_1249 "ERROR midi no memory  9! len:%d."
#define MIDI_OPT_ADP_959_112_2_18_1_14_40_1250 "midi_opt_adp.c [MIDI_Construct]  create midi assistant dynamic task!"
#define MIDI_OPT_ADP_963_112_2_18_1_14_40_1251 "midi_opt_adp.c [MIDI_Construct]  can not create midi assistant dynamic task!"
#define MIDI_OPT_ADP_1015_112_2_18_1_14_40_1252 "midi_opt_adp.c:MIDI_Deconstruct:error:ptMidiProcRes==PNULL"
#define MIDI_OPT_ADP_1075_112_2_18_1_14_40_1253 "midi_opt_adp.c:MIDI_PlayConstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_OPT_ADP_1088_112_2_18_1_14_40_1254 "midi_opt_adp.c:MIDI_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_midi_chng_freq_req_handler"
#define MIDI_OPT_ADP_1094_112_2_18_1_14_40_1255 "midi_opt_adp.c,[MIDI_PlayConstruct] CHNG_FREQ_SetArmClk()!"
#define MIDI_OPT_ADP_1139_112_2_18_1_14_41_1256 "midi_opt_adp.c,[MIDI_PlayConstruct]:MIDI_OPT_Init return error!i_status:%d"
#define MIDI_OPT_ADP_1160_112_2_18_1_14_41_1257 "midi_opt_adp.c:MIDI_ResumeConstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_OPT_ADP_1178_112_2_18_1_14_41_1258 "midi_opt_adp.c:MIDI_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_midi_chng_freq_req_handler"
#define MIDI_OPT_ADP_1186_112_2_18_1_14_41_1259 "midi_opt_adp.c,[MIDI_ResumeConstruct] CHNG_FREQ_SetArmClk()!"
#define MIDI_OPT_ADP_1221_112_2_18_1_14_41_1260 "midi_opt_adp.c:MIDI_PauseDeconstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_OPT_ADP_1255_112_2_18_1_14_41_1261 "midi_opt_adp.c:MIDI_StopDeconstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_OPT_ADP_1320_112_2_18_1_14_41_1262 "midi_opt_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject"
#define MIDI_OPT_ADP_1348_112_2_18_1_14_41_1263 "midi_opt_adp.c:MIDI_Pause:error:SCI_NULL == ptAudioObject"
#define MIDI_OPT_ADP_1374_112_2_18_1_14_41_1264 "midi_opt_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject"
#define MIDI_OPT_ADP_1398_112_2_18_1_14_41_1265 "midi_opt_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject"
#define MIDI_OPT_ADP_1441_112_2_18_1_14_41_1266 "midi_opt_adp.c:MIDI_IdentifyFormat:error:SCI_NULL == ptAudioObject"
#define MIDI_OPT_ADP_1450_112_2_18_1_14_41_1267 "midi_opt_adp.c:MIDI_IdentifyFormat:error:PNULL == mididata"
#define MIDI_OPT_ADP_1486_112_2_18_1_14_41_1268 "midi_opt_adp.c:Midi_IdentifyFormat:error:Unknown Handle!"
#define MIDI_OPT_ADP_1534_112_2_18_1_14_41_1269 "MIDI_IdentifyFormat: Pre checking ERROR! track %d is out of range"
#define MIDI_OPT_ADP_1541_112_2_18_1_14_41_1270 "MIDI_IdentifyFormat: Track %d ERROR. MTrk not found"
#define MIDI_OPT_ADP_1559_112_2_18_1_14_41_1271 "MIDI_IdentifyFormat: ERROR! track %d is out of range"
#define MIDI_OPT_ADP_1569_112_2_18_1_14_41_1272 "[MIDI_IdentifyFormat] ptAudioObject->tAudioFormatInfo.uiTotalTime == 0!"
#define MIDI_OPT_ADP_1573_112_2_18_1_14_41_1273 "[MIDI_IdentifyFormat] ptAudioObject->tAudioFormatInfo.uiTotalTime return error!"
#define MIDI_OPT_ADP_1680_112_2_18_1_14_42_1274 "MIDI_SetSampleRate outputSamplerate:%d, srcSampleRate:%d. "
#define MIDI_OPT_ADP_1696_112_2_18_1_14_42_1275 "midi_opt_adp.c MIDI_SetSampleRate midi doesn't support this sample rate:%d."
#define MIDI_OPT_ADP_1761_112_2_18_1_14_42_1276 "midi_opt_adp.c,[MIDI_SetCodecPara] para type:%d"
#define MIDI_OPT_ADP_1782_112_2_18_1_14_42_1277 "midi_opt_adp.c,[MIDI_SetCodecPara]:invalid para type!"
#define MIDI_OPT_ADP_1805_112_2_18_1_14_42_1278 "midi_opt_adp.c: error::AUDIO_DEFAULT_TASK_ID == tAssiThreadId"
#define MIDI_OPT_ADP_1819_112_2_18_1_14_42_1279 "midi_opt_adp.c [CODEC_SendSignal]-0-  alloc failed!"
#define MIDI_OPT_ADP_1835_112_2_18_1_14_42_1280 "midi_opt_adp.c [CODEC_SendSignal]-01-  alloc failed!"
#define MIDI_OPT_ADP_1851_112_2_18_1_14_42_1281 "midi_opt_adp.c [CODEC_SendSignal]-1-  alloc failed!"
#define MIDI_OPT_ADP_1867_112_2_18_1_14_42_1282 "midi_opt_adp.c [CODEC_SendSignal]-2-  alloc failed!"
#define MIDI_OPT_ADP_1883_112_2_18_1_14_42_1283 "midi_opt_adp.c [CODEC_SendSignal]-9-  alloc failed!"
#define MIDI_OPT_ADP_1891_112_2_18_1_14_42_1284 "midi_opt_adp.c [CODEC_SendSignal]CODEC_ASSI_PLAY_REQ from 0x%x to 0x%x."
#define MIDI_OPT_ADP_1902_112_2_18_1_14_42_1285 "midi_opt_adp.c [CODEC_SendSignal]-10-  alloc failed!"
#define MIDI_OPT_ADP_1918_112_2_18_1_14_42_1286 "midi_opt_adp.c [CODEC_SendSignal]-11-  alloc failed!"
#define MIDI_OPT_ADP_1934_112_2_18_1_14_42_1287 "midi_opt_adp.c [CODEC_SendSignal]-12-  alloc failed!"
#define MIDI_OPT_ADP_1947_112_2_18_1_14_42_1288 "midi_opt_adp.c  [CODEC_SendSignal]error eSigCode:0x%08x!"
#define MIDI_OPT_ADP_2016_112_2_18_1_14_42_1289 "midi_opt_adp.c[CODEC_AssiTask_DspExpPros] begin to dsp express process wav!"
#define MIDI_OPT_ADP_2113_112_2_18_1_14_42_1290 "CODEC_ASSI_STOP_REQ force to exit from pausing."
#define MIDI_OPT_ADP_2126_112_2_18_1_14_42_1291 "midi_opt_adp.c CODEC_AssiTask_DspExpPros midi decode has stopped."
#define MIDI_OPT_ADP_2136_112_2_18_1_14_42_1292 "midi_opt_adp.c CODEC_AssiTask_DspExpPros midi decode exit from pausing state."
#define MIDI_OPT_ADP_2145_112_2_18_1_14_42_1293 "midi_opt_adp.c CODEC_AssiTask_DspExpPros MIDI_SC_Stop."
#define MIDI_OPT_ADP_2153_112_2_18_1_14_43_1294 "midi_opt_adp.c CODEC_AssiTask_DspExpPros exit."
#define MIDI_OPT_ADP_2162_112_2_18_1_14_43_1295 "midi_opt_adp.c CODEC_AssiTask_DspExpPros stop waiting %d."
#define MIDI_OPT_ADP_2168_112_2_18_1_14_43_1296 "CODEC_ASSI_STOP_REQ force to exit from stopping."
#define MIDI_OPT_ADP_2183_112_2_18_1_14_43_1297 "CODEC_ASSI_PLAY_REQ receive again sender:0x%x state:%d."
#define MIDI_OPT_ADP_2216_112_2_18_1_14_43_1298 "CODEC_ASSI_STOP_REQ wait to get dsp ind too long."
#define MIDI_OPT_ADP_2234_112_2_18_1_14_43_1299 "midi decode task %d."
#define MIDI_OPT_ADP_2251_112_2_18_1_14_43_1300 "CODEC_ASSI_DSP_EXP_PRO_REQ wait count:%d."
#define MIDI_OPT_ADP_2258_112_2_18_1_14_43_1301 "CODEC_ASSI_DSP_EXP_PRO_REQ wait to get pcm data too long."
#define MIDI_OPT_ADP_2426_112_2_18_1_14_43_1302 "CODEC_ASSI_DSP_EXP_PRO_REQ sample:%d, src samplerate:%d, time:%d."
#define MIDI_OPT_ADP_2437_112_2_18_1_14_43_1303 "CODEC_ASSI_DSP_EXP_PRO_REQ exit from tran of dsp data process."
#define MIDI_OPT_ADP_2442_112_2_18_1_14_43_1304 "CODEC_ASSI_DSP_EXP_PRO_REQ exit from sbc or tran of dsp data process."
#define MIDI_OPT_ADP_2452_112_2_18_1_14_43_1305 "midi decode task %d."
#define MIDI_OPT_ADP_2469_112_2_18_1_14_43_1306 "CODEC_ASSI_DSP_EXP_PRO_IND sample:%d, time:%d."
#define MIDI_OPT_ADP_2504_112_2_18_1_14_43_1307 "midi_opt_adp.c [CODEC_AssiTask_DspExpPros]other invalid signal code:%d!"
#define MIDI_OPT_API_59_112_2_18_1_14_44_1308 "midi_opt_api.c,[MIDI_OPT_SetVibrator]%d,%d,%d"
#define MIDI_OPT_API_114_112_2_18_1_14_44_1309 "MIDI_OPT_Init\n stereoMode:%d"
#define MIDI_OPT_API_115_112_2_18_1_14_44_1310 "\n splayRate:%d, spolyNum:%d"
#define MIDI_OPT_API_116_112_2_18_1_14_44_1311 "\n speed:%d"
#define MIDI_OPT_API_117_112_2_18_1_14_44_1312 "\n eWorkingMode:%d"
#define MIDI_OPT_API_118_112_2_18_1_14_44_1313 "\n psMelody:%d psDrum:%d"
#define MIDI_OPT_API_119_112_2_18_1_14_44_1314 "\n noteSift:%d"
#define MIDI_OPT_API_120_112_2_18_1_14_44_1315 "\n sdbHalfSamplerate:%d,sdbAlaw8bits:%d"
#define MIDI_OPT_API_191_112_2_18_1_14_44_1316 "MIDI_SC_GetPlayTime: Enter"
#define MIDI_OPT_API_197_112_2_18_1_14_44_1317 "midi_sc_api.c:MIDI_SC_GetPlayTime:error:PNULL==midi_addr"
#define MIDI_OPT_API_209_112_2_18_1_14_44_1318 "Scan midi time : %d, Midi Sample:%d"
#define MIDI_OPT_API_213_112_2_18_1_14_44_1319 "[MIDI_SC_GetPlayTime] error occours when scanniing the whole midi file!"
#define MIDI_OPT_API_235_112_2_18_1_14_44_1320 "midi_sc_api.c:MIDI_SC_Play:error:PNULL==midi_addr"
#define MIDI_OPT_API_241_112_2_18_1_14_44_1321 "[MIDI_OPT_Play]:%d"
#define MIDI_OPT_API_262_112_2_18_1_14_44_1322 "Play midi time : %d,i_status:%d"
#define MIDI_ADP_382_112_2_18_1_14_57_1323 "peter:midi..MIDI_SC_Decoder_Lib_Init......start .."
#define MIDI_ADP_393_112_2_18_1_14_57_1324 "peter:MIDI_SC_Decoder_Lib_Init address is %x, length is %x"
#define MIDI_ADP_400_112_2_18_1_14_57_1325 "peter:midi..MIDI_SC_Decoder_Lib_Init......end .."
#define MIDI_ADP_403_112_2_18_1_14_57_1326 "midi_sc_api Get IRAM Res Fail!"
#define MIDI_ADP_420_112_2_18_1_14_57_1327 "peter:midi..MIDI_SC_Decoder_Lib_Deinit......start .."
#define MIDI_ADP_423_112_2_18_1_14_57_1328 "s_midi_ilock_handle is in unlocking."
#define MIDI_ADP_437_112_2_18_1_14_57_1329 "peter:midi..MIDI_SC_Decoder_Lib_Deinit......end .."
#define MIDI_ADP_441_112_2_18_1_14_57_1330 "MIDI_SC_DeInit:unable to unlock IRAM"
#define MIDI_ADP_571_112_2_18_1_14_58_1331 "MIDI_SendMessage: to TASK %d, wait for event %d"
#define MIDI_ADP_577_112_2_18_1_14_58_1332 "MIDI_SendMessage: TASK resp OK %d"
#define MIDI_ADP_619_112_2_18_1_14_58_1333 "midi_adp.c:MIDI_Construct:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_714_112_2_18_1_14_58_1334 "midi lib internal error"
#define MIDI_ADP_728_112_2_18_1_14_58_1335 "Can't create midi mix buffer"
#define MIDI_ADP_736_112_2_18_1_14_58_1336 "Can't create midi"
#define MIDI_ADP_749_112_2_18_1_14_58_1337 "Bad midi file"
#define MIDI_ADP_757_112_2_18_1_14_58_1338 "midi preparing failed"
#define MIDI_ADP_775_112_2_18_1_14_58_1339 "Can't parse midi"
#define MIDI_ADP_780_112_2_18_1_14_58_1340 "Midi parsed OK, playtime = %d.%03d"
#define MIDI_ADP_787_112_2_18_1_14_58_1341 "ERROR midi no memory 13."
#define MIDI_ADP_802_112_2_18_1_14_58_1342 "peter:midi:MIDI_Deconstruct start "
#define MIDI_ADP_813_112_2_18_1_14_58_1343 "midi_adp.c:MIDI_Deconstruct:error:ptMidiProcRes==PNULL"
#define MIDI_ADP_823_112_2_18_1_14_58_1344 "peter:midi:MIDI_Deconstruct end "
#define MIDI_ADP_842_112_2_18_1_14_58_1345 "midi_adp.c:MIDI_PlayConstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_845_112_2_18_1_14_58_1346 "peter:midi:MIDI_PlayConstruct start "
#define MIDI_ADP_868_112_2_18_1_14_58_1347 "midi_adp.c:MIDI_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL"
#define MIDI_ADP_874_112_2_18_1_14_58_1348 "midi_adp.c,[MIDI_PlayConstruct] CHNG_FREQ_SetArmClk()!"
#define MIDI_ADP_886_112_2_18_1_14_58_1349 "peter:MIDI_Core_GetContextSize start"
#define MIDI_ADP_909_112_2_18_1_14_58_1350 "midi_adp.c:midi_playconstruct: task_id_get faild "
#define MIDI_ADP_938_112_2_18_1_14_58_1351 "peter:midi:MIDI_PlayConstruct end "
#define MIDI_ADP_982_112_2_18_1_14_59_1352 "midi_adp.c:MIDI_ResumeConstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1007_112_2_18_1_14_59_1353 "midi_adp.c:MIDI_ResumeConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL"
#define MIDI_ADP_1014_112_2_18_1_14_59_1354 "midi_adp.c,[MIDI_ResumeConstruct] CHNG_FREQ_SetArmClk()!"
#define MIDI_ADP_1034_112_2_18_1_14_59_1355 "midi_adp.c:MIDI_PauseDeconstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1041_112_2_18_1_14_59_1356 "peter:MidiProtectContext"
#define MIDI_ADP_1082_112_2_18_1_14_59_1357 "midi_adp.c:MIDI_StopDeconstruct:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1089_112_2_18_1_14_59_1358 "midi_adp.c:MIDI_StopDeconstruct:error:ptMidiProcRes==PNULL"
#define MIDI_ADP_1100_112_2_18_1_14_59_1359 "midi_adp.c:MIDI_StopDeconstruct:suspend & kill thread"
#define MIDI_ADP_1154_112_2_18_1_14_59_1360 "peter:midi:MIDI_Play start "
#define MIDI_ADP_1192_112_2_18_1_14_59_1361 "midi_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1196_112_2_18_1_14_59_1362 "peter:midi:MIDI_Stop start "
#define MIDI_ADP_1203_112_2_18_1_14_59_1363 "midi_adp.c:MIDI_Stop: resume task"
#define MIDI_ADP_1231_112_2_18_1_14_59_1364 "midi_adp.c:MIDI_Pause:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1239_112_2_18_1_14_59_1365 "MIDI_Pause: suspend task"
#define MIDI_ADP_1262_112_2_18_1_14_59_1366 "midi_adp.c:MIDI_Pause:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1271_112_2_18_1_14_59_1367 "midi_adp.c:MIDI_Resume: resume task"
#define MIDI_ADP_1292_112_2_18_1_14_59_1368 "midi_adp.c:MIDI_TimeSeek:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1297_112_2_18_1_14_59_1369 "midi_adp.c:MIDI_TimeSeek:time=%d, state=%d"
#define MIDI_ADP_1328_112_2_18_1_14_59_1370 "midi_adp.c:MIDI_Seek:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1334_112_2_18_1_14_59_1371 "midi_adp.c:MIDI_Seek:off=%d,len=%d"
#define MIDI_ADP_1357_112_2_18_1_14_59_1372 "midi_adp.c:MIDI_SetSpeed:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1372_112_2_18_1_14_59_1373 "midi_adp.c:MIDI_SetSpeed speed=%d, eas_speed=0x%08x\n"
#define MIDI_ADP_1406_112_2_18_1_14_59_1374 "midi_adp.c:MIDI_IdentifyFormat:error:SCI_NULL == ptAudioObject"
#define MIDI_ADP_1413_112_2_18_1_14_59_1375 "midi_adp.c:MIDI_IdentifyFormat:error:ptMidiProcRes==PNULL"
#define MIDI_ADP_1430_112_2_18_1_14_59_1376 "midi_adp.c:Midi_IdentifyFormat:error:Unknown Handle!"
#define MIDI_ADP_1438_112_2_18_1_14_59_1377 "[MIDI_IdentifyFormat] ptAudioObject->tAudioFormatInfo.uiTotalTime == 0!"
#define MIDI_ADP_1474_112_2_18_1_15_0_1378 "MIDI_GetPlayingInfo: CurTime:%d"
#define MIDI_ADP_1493_112_2_18_1_15_0_1379 "MIDI_GetPlayingInfo: DataOff:%d, TotalLen:%d, TotalTime:%d"
#define MIDI_ADP_1548_112_2_18_1_15_0_1380 "midi_adp.c:MIDI_SetPolyNum:error:audio_obj == SCI_NULL"
#define MIDI_ADP_1556_112_2_18_1_15_0_1381 "midi_adp.c:MIDI_SetPolyNum %d"
#define MIDI_ADP_1666_112_2_18_1_15_0_1382 "para error in MIDI_SetLpfFilter."
#define MIDI_ADP_1697_112_2_18_1_15_0_1383 "para error in MIDI_SetLpfCutCentFreq."
#define MIDI_ADP_1730_112_2_18_1_15_0_1384 "para error in MIDI_SetEQFilter."
#define MIDI_ADP_1773_112_2_18_1_15_0_1385 "para error in MIDI_SetLCFFilter."
#define MIDI_ADP_1812_112_2_18_1_15_0_1386 "para error in MIDI_SetWorkingMode."
#define MIDI_ADP_1869_112_2_18_1_15_0_1387 "MIDITASK: get signal from[%08X], cmd=%d, param=%d, event %d, object %08X, thread id %08X"
#define MIDI_ADP_1994_112_2_18_1_15_1_1388 "MIDI_Thread_Entry Quit"
#define MIDI_ADP_2057_112_2_18_1_15_1_1389 "midi file_size is invalid: %d."
#define MIDI_ADP_2070_112_2_18_1_15_1_1390 "ERROR s_midi_data_buffer no memory."
#define MIDI_ADP_2082_112_2_18_1_15_1_1391 "ERROR FFS_Read, %d, %d."
#define MIDI_ADP_2092_112_2_18_1_15_1_1392 "ERROR FFS_fseek, %d."
#define MIDI_ADP_2103_112_2_18_1_15_1_1393 "midi not support streambuffer play."
#define MIDI_ADP_2108_112_2_18_1_15_1_1394 "MidiGetSrcData Invalid Handle Type."
#define MIDI_ADP_2155_112_2_18_1_15_1_1395 "midi_adp.c,[MIDI_VibratorCallback]%d"
#define EAS_REPORT_153_112_2_18_1_15_7_1396 "%s"
#define EAS_REPORT_158_112_2_18_1_15_7_1397 "Unrecognized error: Severity=%d; HashCode=%lu; SerialNum=%d"
#define EAS_REPORT_181_112_2_18_1_15_7_1398 "%s"
#define EAS_REPORT_204_112_2_18_1_15_7_1399 "%s"
#define MP3_ADP_548_112_2_18_1_18_8_1400 "mp3_adp.c[MP3_RegCodecPlugger]register mp3 codec! codec handle is:0x%08x"
#define MP3_ADP_576_112_2_18_1_18_8_1401 "mp3_adp.c [MP3_SetHalfSamplerate] s_mp3_halfsamplerate_on:%d"
#define MP3_ADP_602_112_2_18_1_18_8_1402 "mp3_adp.c[MP3_Construct] construct resource!"
#define MP3_ADP_609_112_2_18_1_18_8_1403 "mp3_adp.c:MP3_Construct:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_621_112_2_18_1_18_8_1404 "mp3_adp.c alloc len:%d."
#define MP3_ADP_627_112_2_18_1_18_8_1405 "mp3_adp.c [MP3_Construct] -1- alloc failed! len:%d."
#define MP3_ADP_640_112_2_18_1_18_9_1406 "mp3_adp.c alloc len:%d."
#define MP3_ADP_647_112_2_18_1_18_9_1407 "mp3_adp.c [MP3_Construct] -2- alloc failed! len:%d."
#define MP3_ADP_657_112_2_18_1_18_9_1408 "mp3_adp.c alloc len:%d."
#define MP3_ADP_664_112_2_18_1_18_9_1409 "mp3_adp.c [MP3_Construct] -3- alloc failed! len:%d."
#define MP3_ADP_683_112_2_18_1_18_9_1410 "mp3_adp.c alloc len:%d."
#define MP3_ADP_689_112_2_18_1_18_9_1411 "mp3_adp.c [MP3_Construct] -4- alloc failed! len:%d."
#define MP3_ADP_698_112_2_18_1_18_9_1412 "mp3_adp.c alloc len:%d."
#define MP3_ADP_717_112_2_18_1_18_9_1413 "mp3_adp.c [MP3_Construct] -5- alloc failed! len:%d."
#define MP3_ADP_726_112_2_18_1_18_9_1414 "mp3_adp.c alloc len:%d."
#define MP3_ADP_728_112_2_18_1_18_9_1415 "mp3_adp.c [MP3_Construct] -5.1- alloc swap_buff: %08X."
#define MP3_ADP_734_112_2_18_1_18_9_1416 "mp3_adp.c [MP3_Construct] -5.1- alloc swap_buff: %08X."
#define MP3_ADP_740_112_2_18_1_18_9_1417 "mp3_adp.c [MP3_Construct] -5.1- alloc failed! len:%d."
#define MP3_ADP_749_112_2_18_1_18_9_1418 "mp3_adp.c alloc len:%d."
#define MP3_ADP_755_112_2_18_1_18_9_1419 "mp3_adp.c [MP3_Construct] -6- alloc failed! len:%d."
#define MP3_ADP_782_112_2_18_1_18_9_1420 "mp3_adp.c[MP3_Construct] Failed to create mp3 dynamic task!"
#define MP3_ADP_802_112_2_18_1_18_9_1421 "mp3_adp.c[MP3_Deconstruct] deconstruct resource!"
#define MP3_ADP_809_112_2_18_1_18_9_1422 "mp3_adp.c:MP3_Deconstruct:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_869_112_2_18_1_18_9_1423 "mp3_adp.c[MP3_PlayConstruct] construct resource before play!"
#define MP3_ADP_876_112_2_18_1_18_9_1424 "mp3_adp.c:MP3_PlayConstruct:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_889_112_2_18_1_18_9_1425 "mp3_adp.c[MP3_PlayConstruct] MP3_Decoder_Lib_Init error!"
#define MP3_ADP_918_112_2_18_1_18_9_1426 "mp3_adp.c[MP3_PlayConstruct] create mp3 static task!"
#define MP3_ADP_931_112_2_18_1_18_9_1427 "mp3_adp.c[MP3_PlayConstruct]:error: CHNG_FREQ_REQ_HANDLER_NULL==s_mp3_chng_freq_req_handler!"
#define MP3_ADP_963_112_2_18_1_18_9_1428 "mp3_adp.c[MP3_StopDeconstruct] deconstruct resource after stop!"
#define MP3_ADP_970_112_2_18_1_18_9_1429 "mp3_adp.c:MP3_PlayConstruct:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_978_112_2_18_1_18_9_1430 "mp3_adp.c:MP3_PlayConstruct:error:SCI_NULL==ptAudioObj->tCodecProcessResouce.other_info_ptr"
#define MP3_ADP_1036_112_2_18_1_18_10_1431 "mp3_adp.c[MP3_StopDeconstruct] MP3_Decoder_Lib_Deinit error!"
#define MP3_ADP_1054_112_2_18_1_18_10_1432 "mp3_adp.c[MP3_ResumeConstruct] construct resource before resume!"
#define MP3_ADP_1061_112_2_18_1_18_10_1433 "mp3_adp.c:MP3_ResumeConstruct:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_1074_112_2_18_1_18_10_1434 "mp3_adp.c[MP3_ResumeConstruct] MP3_Decoder_Lib_Init error!"
#define MP3_ADP_1114_112_2_18_1_18_10_1435 "mp3_adp.c[MP3_ResumeConstruct]:error: CHNG_FREQ_REQ_HANDLER_NULL==s_mp3_chng_freq_req_handler!"
#define MP3_ADP_1144_112_2_18_1_18_10_1436 "mp3_adp.c[MP3_PauseDeconstruct] deconstruct resource after pause!"
#define MP3_ADP_1151_112_2_18_1_18_10_1437 "mp3_adp.c:MP3_PauseDeconstruct:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_1159_112_2_18_1_18_10_1438 "mp3_adp.c:MP3_PauseDeconstruct:error:SCI_NULL==ptAudioObj->tCodecProcessResouce.other_info_ptr"
#define MP3_ADP_1217_112_2_18_1_18_10_1439 "mp3_adp.c[MP3_PauseDeconstruct] MP3_Decoder_Lib_Deinit error!"
#define MP3_ADP_1234_112_2_18_1_18_10_1440 "mp3_adp.c[MP3_Play] send MP3_PLAY_REQ to codec task!"
#define MP3_ADP_1246_112_2_18_1_18_10_1441 "mp3_adp.c:MP3_Play:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_1252_112_2_18_1_18_10_1442 "mp3_adp.c[MP3_Play] AUDIO_DEFAULT_TASK_ID==ptAudioObj->tThreadId."
#define MP3_ADP_1272_112_2_18_1_18_10_1443 "mp3_adp.c[MP3_Stop] send MP3_STOP_REQ to codec task!"
#define MP3_ADP_1285_112_2_18_1_18_10_1444 "mp3_adp.c:MP3_Stop:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_1292_112_2_18_1_18_10_1445 "mp3_adp.c,[MP3_Stop] task id is invalid !mp3 is stopped already!"
#define MP3_ADP_1312_112_2_18_1_18_10_1446 "mp3_adp.c[MP3_Pause] send MP3_PAUSE_REQ to codec task!"
#define MP3_ADP_1319_112_2_18_1_18_10_1447 "mp3_adp.c:MP3_Pause:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_1326_112_2_18_1_18_10_1448 "mp3_adp.c,[MP3_Pause] task id is invalid !mp3 is stopped already!"
#define MP3_ADP_1345_112_2_18_1_18_10_1449 "mp3_adp.c[MP3_Resume] send MP3_RESUME_REQ to codec task!"
#define MP3_ADP_1352_112_2_18_1_18_10_1450 "mp3_adp.c:MP3_Resume:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_1358_112_2_18_1_18_10_1451 "mp3_adp.c[MP3_Resume] AUDIO_DEFAULT_TASK_ID==ptAudioObj->tThreadId."
#define MP3_ADP_1382_112_2_18_1_18_11_1452 "mp3_adp.c[MP3_Seek] send MP3_SEEK_REQ to codec task!uiOffset:%d"
#define MP3_ADP_1389_112_2_18_1_18_11_1453 "mp3_adp.c:MP3_Seek:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_1395_112_2_18_1_18_11_1454 "mp3_adp.c [MP3_Seek] AUDIO_DEFAULT_TASK_ID == ptAudioObj->tThreadId."
#define MP3_ADP_1475_112_2_18_1_18_11_1455 "mp3_adp.c[MP3_SendStreamData] send MP3_SRCPROC_REQ to codec task!"
#define MP3_ADP_1482_112_2_18_1_18_11_1456 "mp3_adp.c:MP3_SendStreamData:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_1488_112_2_18_1_18_11_1457 "mp3_adp.c[MP3_SendStreamData] AUDIO_DEFAULT_TASK_ID==ptAudioObj->tThreadId."
#define MP3_ADP_1511_112_2_18_1_18_11_1458 "mp3_adp.c:MP3_IdentifyFormat:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_1519_112_2_18_1_18_11_1459 "mp3_adp.c[MP3_IdentifyFormat] identify format:result:0x%08x!"
#define MP3_ADP_1546_112_2_18_1_18_11_1460 "mp3_adp.c:MP3_GetPlayingInfo:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_1557_112_2_18_1_18_11_1461 "mp3_adp.c[MP3_GetPlayingInfo]: error:SCI_NULL == mp3_ext_format_info_ptr"
#define MP3_ADP_1565_112_2_18_1_18_11_1462 "mp3_adp.c[MP3_GetPlayingInfo]: error:SCI_NULL == mp3_ext_res_ptr"
#define MP3_ADP_1611_112_2_18_1_18_11_1463 "mp3_adp.c[MP3_GetPlayingInfo] Total time is:0x%08x"
#define MP3_ADP_1630_112_2_18_1_18_11_1464 "mp3_adp.c[MP3_GetPlayingInfo] cur_offset_temp_1:%d(cur_decode:%d, first_frame:%d), cur_offset_temp_2:%d."
#define MP3_ADP_1640_112_2_18_1_18_11_1465 "mp3_adp.c:MP3_GetPlayingInfo:error:(0 == mp3_format_info_ptr->uiBitrate)||(0 == ptAudioObj->uiOutputSamplerate)"
#define MP3_ADP_1647_112_2_18_1_18_11_1466 "mp3_adp.c[MP3_GetPlayingInfo] cbr, cur_time_1:%d, cur_time_2:%d."
#define MP3_ADP_1657_112_2_18_1_18_11_1467 "mp3_adp.c:MP3_GetPlayingInfo:error:(0 == mp3_ext_format_info_ptr->uiMusicDataLen)||(0 == ptAudioObj->uiOutputSamplerate)"
#define MP3_ADP_1688_112_2_18_1_18_11_1468 "mp3_adp.c[MP3_GetPlayingInfo] vbr, cur_time_1:%d, cur_time_2:%d."
#define MP3_ADP_1699_112_2_18_1_18_11_1469 "mp3_adp.c[MP3_GetPlayingInfo] exception: cur_time_1 < cur_time_2!"
#define MP3_ADP_1735_112_2_18_1_18_11_1470 "mp3_adp.c[MP3_GetPlayingInfo], len--total:%d, cur:%d(byte), time--total:%d, cur:%d(s)."
#define MP3_ADP_1757_112_2_18_1_18_11_1471 "mp3_adp.c[MP3_GetPlayingInfo] stream, %d should not less than:%d(=%d+%d), InfoList:%d, %d, %d, %d."
#define MP3_ADP_1768_112_2_18_1_18_11_1472 "mp3_adp.c: error: Unknown Handle Type!"
#define MP3_ADP_1787_112_2_18_1_18_11_1473 "mp3_adp.c:MP3_GetOutputSampleRate:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_1825_112_2_18_1_18_11_1474 "mp3_adp.c:MP3_FreeAsPause:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_1835_112_2_18_1_18_11_1475 "mp3_adp.c [MP3_FreeAsPause]:error:ext_resouce_ptr==SCI_NULL"
#define MP3_ADP_1900_112_2_18_1_18_12_1476 "mp3_adp.c:MP3_AllocAsResume:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_1911_112_2_18_1_18_12_1477 "mp3_adp.c [MP3_AllocAsResume]:error:ext_resouce_ptr==SCI_NULL"
#define MP3_ADP_1922_112_2_18_1_18_12_1478 "mp3_adp.c alloc len:%d."
#define MP3_ADP_1928_112_2_18_1_18_12_1479 "mp3_adp.c [MP3_AllocAsResume] -1- alloc failed! len:%d."
#define MP3_ADP_1937_112_2_18_1_18_12_1480 "mp3_adp.c alloc len:%d."
#define MP3_ADP_1943_112_2_18_1_18_12_1481 "mp3_adp.c [MP3_AllocAsResume] -2- alloc failed! len:%d."
#define MP3_ADP_1963_112_2_18_1_18_12_1482 "mp3_adp.c alloc len:%d."
#define MP3_ADP_1968_112_2_18_1_18_12_1483 "mp3_adp.c [MP3_AllocAsResume] -3- alloc failed! len:%d."
#define MP3_ADP_1983_112_2_18_1_18_12_1484 "mp3_adp.c alloc len:%d."
#define MP3_ADP_1985_112_2_18_1_18_12_1485 "mp3_adp.c [MP3_AllocAsResume] -4- alloc swap_buff: %08X."
#define MP3_ADP_1991_112_2_18_1_18_12_1486 "mp3_adp.c [MP3_AllocAsResume] -4- alloc swap_buff: %08X."
#define MP3_ADP_1996_112_2_18_1_18_12_1487 "mp3_adp.c [MP3_AllocAsResume] -4- alloc failed! len:%d."
#define MP3_ADP_2008_112_2_18_1_18_12_1488 "mp3_adp.c alloc len:%d."
#define MP3_ADP_2013_112_2_18_1_18_12_1489 "mp3_adp.c [MP3_AllocAsResume] -5- alloc failed! len:%d."
#define MP3_ADP_2040_112_2_18_1_18_12_1490 "mp3_adp.c:MP3_FreeInConstruct:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_2051_112_2_18_1_18_12_1491 "mp3_adp.c [MP3_FreeInConstruct]:error:ext_resouce_ptr==SCI_NULL"
#define MP3_ADP_2100_112_2_18_1_18_12_1492 "mp3_adp.c[MP3_SendSignal]AUDIO_DEFAULT_TASK_ID == tThreadId"
#define MP3_ADP_2113_112_2_18_1_18_12_1493 "mp3_adp.c  [MP3_SendSignal] -1- alloc failed!"
#define MP3_ADP_2135_112_2_18_1_18_12_1494 "mp3_adp.c  [MP3_SendSignal]-2- alloc failed!"
#define MP3_ADP_2152_112_2_18_1_18_12_1495 "mp3_adp.c  [MP3_SendSignal]-3- alloc failed!"
#define MP3_ADP_2168_112_2_18_1_18_12_1496 "mp3_adp.c  [MP3_SendSignal]-4- alloc failed!"
#define MP3_ADP_2190_112_2_18_1_18_12_1497 "mp3_adp.c  [MP3_SendSignal]-5- alloc failed!"
#define MP3_ADP_2213_112_2_18_1_18_12_1498 "mp3_adp.c  [MP3_SendSignal]-6- alloc failed!"
#define MP3_ADP_2230_112_2_18_1_18_12_1499 "mp3_adp.c  [MP3_SendSignal]-7- alloc failed!"
#define MP3_ADP_2246_112_2_18_1_18_12_1500 "mp3_adp.c  [MP3_SendSignal]-8- alloc failed!"
#define MP3_ADP_2258_112_2_18_1_18_12_1501 "mp3_adp.c  [MP3_SendSignal]error eSigCode:0x%08x!"
#define MP3_ADP_2286_112_2_18_1_18_12_1502 "MP3_Stream_CastOff_SrcDataInfo para error:%d, %d."
#define MP3_ADP_2299_112_2_18_1_18_12_1503 "MP3_Stream_CastOff_SrcDataInfo CastOffDataLen:%d, %d, %d."
#define MP3_ADP_2311_112_2_18_1_18_12_1504 "MP3_Stream_CastOff_SrcDataInfo samplerate:%d<%d."
#define MP3_ADP_2330_112_2_18_1_18_12_1505 "MP3_Stream_CastOff_SrcDataInfo 0 CastOffDataInfo:%d, %d, %d."
#define MP3_ADP_2348_112_2_18_1_18_12_1506 "MP3_Stream_CastOff_SrcDataInfo 1 CastOffDataInfo:%d, %d, %d."
#define MP3_ADP_2366_112_2_18_1_18_12_1507 "MP3_Stream_CastOff_SrcDataInfo 2 CastOffDataInfo:%d, %d, %d."
#define MP3_ADP_2391_112_2_18_1_18_12_1508 "MP3_Stream_CastOff_SrcDataInfo 3 CastOffDataInfo:%d, %d, %d, leftDataLen:%d."
#define MP3_ADP_2406_112_2_18_1_18_13_1509 "MP3_Stream_CastOff_SrcDataInfo realCastoff:%d, wantToCastOff:%d, lastPlayTime:%d."
#define MP3_ADP_2415_112_2_18_1_18_13_1510 "MP3_Stream_CastOff_SrcDataInfo, ListInfo:%d, %d, %d, %d."
#define MP3_ADP_2475_112_2_18_1_18_13_1511 "mp3_adp.c[MP3_Thread_Entry]  begin to decode mp3!"
#define MP3_ADP_2503_112_2_18_1_18_13_1512 "mp3_adp.c[MP3_Thread_Entry] receive MP3_PLAY_REQ!decoder state:0x%08x"
#define MP3_ADP_2577_112_2_18_1_18_13_1513 "MP3_Thread_Entry uiCurrentDecodePos:%d in MP3_PLAY_REQ."
#define MP3_ADP_2619_112_2_18_1_18_13_1514 "mp3_adp.c [MP3_Thread_Entry] receive MP3_STOP_REQ!decoder state:0x%08x"
#define MP3_ADP_2623_112_2_18_1_18_13_1515 "mp3_adp.c [MP3_Thread_Entry] case MP3_STOP_REQ:the decoder is stopped already!"
#define MP3_ADP_2654_112_2_18_1_18_13_1516 "mp3_adp.c [MP3_Thread_Entry] receive MP3_PAUSE_REQ!decoder state:0x%08x"
#define MP3_ADP_2658_112_2_18_1_18_13_1517 "mp3_adp.c [MP3_Thread_Entry] the decoder is stopped already!"
#define MP3_ADP_2703_112_2_18_1_18_13_1518 "mp3_adp.c[MP3_Thread_Entry] receive MP3_RESUME_REQ!decoder state:0x%08x,pausing offset:0x%08x"
#define MP3_ADP_2711_112_2_18_1_18_13_1519 "mp3_adp.c[MP3_Thread_Entry] uiCurrentDecodePos:%d in MP3_RESUME_REQ."
#define MP3_ADP_2758_112_2_18_1_18_13_1520 "mp3_adp.c[MP3_Thread_Entry] receive MP3_SEEK_REQ!decoder state:0x%08x,dest_offset:0x%08x,uiFirstFrameOffset:%d"
#define MP3_ADP_2763_112_2_18_1_18_13_1521 "mp3_adp.c [MP3_Thread_Entry] the decoder is stopped already!"
#define MP3_ADP_2773_112_2_18_1_18_13_1522 "MP3_Thread_Entry uiCurrentDecodePos:%d in MP3_SEEK_REQ."
#define MP3_ADP_2800_112_2_18_1_18_13_1523 "mp3_adp.c[MP3_Thread_Entry] error happens in seek operation!"
#define MP3_ADP_2811_112_2_18_1_18_13_1524 "mp3_adp.c[MP3_Thread_Entry] receive MP3_SRCPROC_REQ!decoder state:0x%08x"
#define MP3_ADP_2815_112_2_18_1_18_13_1525 "mp3_adp.c [MP3_Thread_Entry]MP3_SRCPROC_REQ: the decoder is stopped already!"
#define MP3_ADP_2831_112_2_18_1_18_13_1526 "mp3_adp.c[MP3_Thread_Entry] case MP3_SRCPROC_REQ:error:(MP3_DECODER_GETTINGSRC != mp3_ext_res_ptr->eDecoderState)......."
#define MP3_ADP_2851_112_2_18_1_18_13_1527 "mp3_adp.c,[MP3_SRCPROC_REQ]stream: buf is not enough,curCount:%d!"
#define MP3_ADP_2877_112_2_18_1_18_13_1528 "mp3_adp.c[MP3_Thread_Entry] case MP3_SRCPROC_REQ:stream!"
#define MP3_ADP_2884_112_2_18_1_18_13_1529 "mp3_adp.c[MP3_Thread_Entry] case MP3_SRCPROC_REQ:end of music !"
#define MP3_ADP_2895_112_2_18_1_18_13_1530 "mp3_adp.c[MP3_Thread_Entry] receive MP3_GETSRC_REQ!decoder state:0x%08x"
#define MP3_ADP_2900_112_2_18_1_18_13_1531 "mp3_adp.c [MP3_Thread_Entry]MP3_GETSRC_REQ: the decoder is stopped already!"
#define MP3_ADP_2956_112_2_18_1_18_14_1532 "mp3_adp.c[MP3_Thread_Entry] case MP3_GETSRC_REQ:mp3_get_src_count == 0! uiMusicDataLen:0, TotalLenInList:%d!"
#define MP3_ADP_2971_112_2_18_1_18_14_1533 "mp3_adp.c[MP3_Thread_Entry] case MP3_GETSRC_REQ:mp3_get_src_count == 0! sleep and GoOnGetSrcData, %d, %d!"
#define MP3_ADP_2978_112_2_18_1_18_14_1534 "mp3_adp.c[MP3_Thread_Entry] case MP3_GETSRC_REQ:mp3_get_src_count == 0! End of music!"
#define MP3_ADP_2988_112_2_18_1_18_14_1535 "mp3_adp.c[MP3_Thread_Entry] case MP3_GETSRC_REQ:MP3_GET_SRC_WAIT:have to wait for a while and get src again!"
#define MP3_ADP_2998_112_2_18_1_18_14_1536 "mp3_adp.c[MP3_Thread_Entry] case MP3_GETSRC_REQ:MP3_GET_SRC_ASY:have to wait for the MP3_SRCPROC_REQ!"
#define MP3_ADP_3005_112_2_18_1_18_14_1537 "mp3_adp.c[MP3_Thread_Entry] case MP3_GETSRC_REQ:error happened when getting source!"
#define MP3_ADP_3015_112_2_18_1_18_14_1538 "mp3_adp.c: Unknown Command!"
#define MP3_ADP_3027_112_2_18_1_18_14_1539 "mp3_adp.c[MP3_Thread_Entry]case MP3_DECODE_REQ: the decoder state is MP3_DECODER_STOPPED"
#define MP3_ADP_3033_112_2_18_1_18_14_1540 "mp3_adp.c [MP3_Thread_Entry]MP3_DECODE_REQ: no data left in the swap buffer!"
#define MP3_ADP_3074_112_2_18_1_18_14_1541 "mp3_adp.c[MP3_Thread_Entry]  mp3 current decode pos:%d, ori_sam:%d."
#define MP3_ADP_3096_112_2_18_1_18_14_1542 "mp3_adp.c[MP3_Thread_Entry] MP3_DECODE_REQ:%d, %d, %d, %d, %d, bitrate:%d."
#define MP3_ADP_3115_112_2_18_1_18_14_1543 "mp3_adp.c[MP3_Thread_Entry] case MP3_DECODE_REQ End of music, streamList:%d, %d, %d, %d!"
#define MP3_ADP_3122_112_2_18_1_18_14_1544 "mp3_adp.c[MP3_Thread_Entry] case MP3_DECODE_REQ End of music!"
#define MP3_ADP_3134_112_2_18_1_18_14_1545 "mp3_adp.c[MP3_Thread_Entry] case MP3_DECODE_REQ End of music!"
#define MP3_ADP_3150_112_2_18_1_18_14_1546 "mp3_adp.c[MP3_Thread_Entry] Lack MP3 data in MP3_ERROR_NONE frameLen:%d should less than:%d."
#define MP3_ADP_3200_112_2_18_1_18_14_1547 "mp3_adp.c[MP3_Thread_Entry] the no.0x%08x MP3_ERROR_BUFLEN in this song"
#define MP3_ADP_3213_112_2_18_1_18_14_1548 "MP3_Thread_Entry uiCurrentDecodePos:%d after lossing %d (byte)"
#define MP3_ADP_3244_112_2_18_1_18_14_1549 "MP3_Thread_Entry uiCurrentDecodePos:%d = src_offset after lossing %d(byte)."
#define MP3_ADP_3267_112_2_18_1_18_14_1550 "mp3_adp.c[MP3_Thread_Entry] the no.0x%08x MP3_ERROR_FATAL in this song"
#define MP3_ADP_3272_112_2_18_1_18_14_1551 "MP3_Thread_Entry uiCurrentDecodePos:%d in MP3_DECODE_REQ MP3_ERROR_FATAL."
#define MP3_ADP_3292_112_2_18_1_18_14_1552 "mp3_adp.c [MP3_Thread_Entry] unknown decode error happened!"
#define MP3_ADP_3359_112_2_18_1_18_14_1553 "mp3_adp.c [MP3_GetSrcData] SCI_NULL == file_handle!"
#define MP3_ADP_3387_112_2_18_1_18_14_1554 "mp3_adp.c [MP3_GetSrcData] input para error!0x%08x,0x%08x"
#define MP3_ADP_3453_112_2_18_1_18_15_1555 "mp3_adp.c,[MP3_GetSrcData]stream:get_stream_data return result:%d,ptr:0x%08x,len:%d!"
#define MP3_ADP_3465_112_2_18_1_18_15_1556 "mp3_adp.c,[MP3_GetSrcData]stream:End of music!"
#define MP3_ADP_3487_112_2_18_1_18_15_1557 "mp3_adp.c,[MP3_GetSrcData]stream: buf is not enough,curCount:%d!"
#define MP3_ADP_3504_112_2_18_1_18_15_1558 "mp3_adp.c,[MP3_GetSrcData]StreamList:%d, %d, %d, %d, thisLen:%d, thisStartTime:%d, totalLen:%d."
#define MP3_ADP_3541_112_2_18_1_18_15_1559 "mp3_adp.c,[MP3_GetSrcData]get_stream_data return AUDIO_STREAM_GET_SRC_WAIT!"
#define MP3_ADP_3546_112_2_18_1_18_15_1560 "mp3_adp.c,[MP3_GetSrcData]get_stream_data return AUDIO_STREAM_GET_SRC_ASY!"
#define MP3_ADP_3551_112_2_18_1_18_15_1561 "mp3_adp.c,[MP3_GetSrcData]:get_stream_data return AUDIO_STREAM_GET_SRC_ERROR!"
#define MP3_ADP_3556_112_2_18_1_18_15_1562 "mp3_adp.c,[MP3_GetSrcData]:get_stream_data return result:%d!wrong type!"
#define MP3_ADP_3566_112_2_18_1_18_15_1563 "mp3_adp.c,[MP3_GetSrcData]:wrong handle type!sub_type:%d"
#define MP3_ADP_3589_112_2_18_1_18_15_1564 "mp3_adp.c:MP3_SeekAudio:error:SCI_NULL == ptAudioObj"
#define MP3_ADP_3629_112_2_18_1_18_15_1565 "mp3_adp.c [MP3_SeekAudio] buffer seek:the position is larger than the total length!"
#define MP3_ADP_3637_112_2_18_1_18_15_1566 "mp3_adp.c [MP3_SeekAudio] stream, reset uiMusicDataLen!"
#define MP3_ADP_3642_112_2_18_1_18_15_1567 "mp3_adp.c [MP3_SeekAudio] Invalid handle type!"
#define MP3_ADP_3692_112_2_18_1_18_15_1568 "mp3_adp.c [MP3_InitDecoder] out put sample rate :0x%08x"
#define MP3_DECODE_393_112_2_18_1_18_16_1569 "\r\nMP3_Decoder_Lib_Deinit:unable to unlock IRAM"
#define MP3_FFS_76_112_2_18_1_18_20_1570 "mp3_ffs.c,[MP3_ReadData] (0 == ptMp3Handle)||(SCI_NULL == puiBuf)||(SCI_NULL == puiDataGot)!"
#define MP3_FFS_87_112_2_18_1_18_20_1571 "mp3_ffs.c,[MP3_ReadData] 0 == ptMp3Handle->unMp3data.fFileHandle!"
#define MP3_FFS_94_112_2_18_1_18_20_1572 "mp3_ffs.c,[MP3_ReadData] read file encounters error!"
#define MP3_FFS_112_112_2_18_1_18_20_1573 "mp3_ffs.c,[ MP3_ReadData]  (SCI_NULL == data_buffer)||(0 == data_len)||(cur_offset >data_len)"
#define MP3_FFS_125_112_2_18_1_18_20_1574 "mp3_ffs.c,[MP3_ReadData]:Unknown Handle Type!"
#define MP3_FFS_129_112_2_18_1_18_20_1575 "mp3_ffs.c,[MP3_ReadData]type:%d,uiLen:0x%08x,data got:0x%08x"
#define MP3_FFS_150_112_2_18_1_18_20_1576 "mp3_ffs.c,[MP3_ReadData] SCI_NULL == ptMp3Handle!"
#define MP3_FFS_161_112_2_18_1_18_20_1577 "mp3_ffs.c,[MP3_SetDataPointer] 0 == ptMp3Handle->unMp3data.fFileHandle!"
#define MP3_FFS_167_112_2_18_1_18_20_1578 "mp3_ffs.c,[MP3_SetDataPointer] seek file encounters error!"
#define MP3_FFS_187_112_2_18_1_18_21_1579 "mp3_ffs.c,[MP3_SetDataPointer]  (SCI_NULL == data_buffer)||(0 == data_len)||(cur_offset >data_len)"
#define MP3_FFS_207_112_2_18_1_18_21_1580 "mp3_ffs.c,[MP3_SetDataPointer]  invalid BasePos!"
#define MP3_FFS_213_112_2_18_1_18_21_1581 "mp3_ffs.c,[MP3_SetDataPointer]  buffer seek failed!dest_Offset:0x%x,data_len:0x%x."
#define MP3_FFS_223_112_2_18_1_18_21_1582 "mp3_ffs.c,[MP3_SetDataPointer]  Unknown Handle Type!"
#define MP3_FFS_227_112_2_18_1_18_21_1583 "mp3_ffs.c,[MP3_SetDataPointer]:type:%d,eBasePos:%d,offset:0x%08x"
#define MP3_FFS_260_112_2_18_1_18_21_1584 "mp3_ffs.c,[MP3_GetDataSize] get file size encounters error!"
#define MP3_FFS_272_112_2_18_1_18_21_1585 "mp3_ffs.c:Unknown File Handle Type!"
#define MP3_FFS_276_112_2_18_1_18_21_1586 "mp3_ffs.c,[MP3_GetDataSize]:type:%d,size:0x%08x"
#define MP3_FFS_308_112_2_18_1_18_21_1587 "mp3_ffs.c,[MP3_GetDataPointer] get file data pointer encounters error!"
#define MP3_FFS_326_112_2_18_1_18_21_1588 "mp3_ffs.c,[MP3_GetDataPointer]  (SCI_NULL == data_buffer)||(0 == data_len)||(cur_offset >data_len)"
#define MP3_FFS_345_112_2_18_1_18_21_1589 "mp3_ffs.c,[MP3_GetDataPointer]  invalid BasePos!"
#define MP3_FFS_354_112_2_18_1_18_21_1590 "mp3_ffs.c:MP3_GetDataPointer:Unknown File Handle Type!"
#define MP3_FFS_358_112_2_18_1_18_21_1591 "mp3_ffs.c,[MP3_GetDataPointer]:type:%d,eBasePos:%d,cur pos:0x%08x"
#define MP3_FORMAT_587_112_2_18_1_18_23_1592 "mp3_format.c:MP3_CaculateNum:error:(puiBuf = SCI_NULL)||(puiNum= SCI_NULL)"
#define MP3_FORMAT_608_112_2_18_1_18_23_1593 "mp3_format.c,[MP3_CaculateNum]:i_calc:%d!not number!"
#define MP3_FORMAT_781_112_2_18_1_18_23_1594 "mp3_format.c:MP3_CheckXingHeader:error:(SCI_NULL == ptMp3Handle)||(SCI_NULL == ptXingHeaderInfo)..."
#define MP3_FORMAT_913_112_2_18_1_18_23_1595 "mp3_format.c:MP3_CheckID3V1Tag:error:(SCI_NULL == ptMp3Handle)||(SCI_NULL == ptXingHeaderInfo)..."
#define MP3_FORMAT_964_112_2_18_1_18_23_1596 "mp3_format.c [MP3_CheckID3V1Tag] V1.1:no valid year info!"
#define MP3_FORMAT_981_112_2_18_1_18_23_1597 "mp3_format.c [MP3_CheckID3V1Tag] V1.1:no valid genre info!"
#define MP3_FORMAT_998_112_2_18_1_18_23_1598 "mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!"
#define MP3_FORMAT_1012_112_2_18_1_18_23_1599 "mp3_format.c [MP3_CheckID3V1Tag]V1.0: no valid genre info!"
#define MP3_FORMAT_1044_112_2_18_1_18_23_1600 "mp3_format.c:MP3_CheckID3V2Tag:error:(SCI_NULL == ptMp3Handle)||(SCI_NULL == ptXingHeaderInfo)..."
#define MP3_FORMAT_1074_112_2_18_1_18_24_1601 "mp3_format.c,[MP3_CheckID3V2Tag]:tag version:%d,tag size:0x%08x"
#define MP3_FORMAT_1094_112_2_18_1_18_24_1602 "mp3_format.c,[MP3_CheckID3V2Tag]:End of tag frames !"
#define MP3_FORMAT_1122_112_2_18_1_18_24_1603 "mp3_format.c,[MP3_CheckID3V2Tag]:frame id :%d ,frame_content_size:%d"
#define MP3_FORMAT_1133_112_2_18_1_18_24_1604 "mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_FORMAT_1159_112_2_18_1_18_24_1605 "mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_FORMAT_1186_112_2_18_1_18_24_1606 "mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_FORMAT_1213_112_2_18_1_18_24_1607 "mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_FORMAT_1240_112_2_18_1_18_24_1608 "mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_FORMAT_1267_112_2_18_1_18_24_1609 "mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_FORMAT_1294_112_2_18_1_18_24_1610 "mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_FORMAT_1341_112_2_18_1_18_24_1611 "mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!"
#define MP3_FORMAT_1346_112_2_18_1_18_24_1612 "mp3_format.c,[MP3_CheckID3V2Tag]:no valid genre index!"
#define MP3_FORMAT_1375_112_2_18_1_18_24_1613 "mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_FORMAT_1395_112_2_18_1_18_24_1614 "mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!"
#define MP3_FORMAT_1399_112_2_18_1_18_24_1615 "mp3_format.c,[MP3_CheckID3V2Tag]:year:impossible value!"
#define MP3_FORMAT_1415_112_2_18_1_18_24_1616 "mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_FORMAT_1435_112_2_18_1_18_24_1617 "mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!"
#define MP3_FORMAT_1439_112_2_18_1_18_24_1618 "mp3_format.c,[MP3_CheckID3V2Tag]:track:impossible value!"
#define MP3_FORMAT_1450_112_2_18_1_18_24_1619 "mp3_format.c,[MP3_CheckID3V2Tag]:failed to jump out this frame!"
#define MP3_FORMAT_1455_112_2_18_1_18_24_1620 "mp3_format.c,[MP3_CheckID3V2Tag]:other frame ID !We do not need the frame content!"
#define MP3_FORMAT_1463_112_2_18_1_18_24_1621 "mp3_format.c,[MP3_CheckID3V2Tag] we find invalid frame ID!"
#define MP3_FORMAT_1492_112_2_18_1_18_24_1622 "mp3_format.c:MP3_CheckID3V2Tag:error:(SCI_NULL == ptMp3Handle)||(SCI_NULL == ptXingHeaderInfo)..."
#define MP3_FORMAT_1571_112_2_18_1_18_25_1623 "mp3_format.c:MP3_CheckLameTag:error:(SCI_NULL == ptMp3Handle)||(SCI_NULL == ptXingHeaderInfo)..."
#define MP3_FORMAT_1597_112_2_18_1_18_25_1624 "mp3_format.c [MP3_CheckLameTag] no valid version info!"
#define MP3_FORMAT_1683_112_2_18_1_18_25_1625 "mp3_format.c:MP3_CheckLyrics3Tag:error:(SCI_NULL == ptMp3Handle)||(SCI_NULL == ptXingHeaderInfo)..."
#define MP3_FORMAT_1728_112_2_18_1_18_25_1626 "mp3_format.c,[MP3_CheckLyrics3Tag] tag size info error!can not get tag size!"
#define MP3_FORMAT_1762_112_2_18_1_18_25_1627 "mp3_format.c,[MP3_CheckLyrics3Tag] can not alloc memory!"
#define MP3_GETFILEINFO_162_112_2_18_1_18_25_1628 "mp3_getfileinfo.c,[MP3_CalcAveBitrate]uiFirstFrameOffset :0x%08x >= file_len :0x%08x"
#define MP3_GETFILEINFO_239_112_2_18_1_18_26_1629 "peter:mp3_format_info.uiBitrate is %d,ave_bitrate_sum is %d"
#define MP3_GETFILEINFO_282_112_2_18_1_18_26_1630 "mp3_getfileinfo.c [MP3_CalcAveBitrate]frame_count:%d,ave_bitrate_sum:%d kbps"
#define MP3_GETFILEINFO_316_112_2_18_1_18_26_1631 "mp3_getfileinfo.c:MP3_Dmx_iToc:error:puiBuf == PNULL"
#define MP3_GETFILEINFO_322_112_2_18_1_18_26_1632 "[MP3_Dmx_iToc]:input number: %d is not valid,(should be less than 9999) !"
#define MP3_GETFILEINFO_463_112_2_18_1_18_26_1633 "mp3_getfileinfo.c,[MP3_IsVBR]uiFirstFrameOffset :0x%08x >= file_len :0x%08x"
#define MP3_GETFILEINFO_614_112_2_18_1_18_26_1634 "mp3_getfileinfo.c,[MP3_CheckFileValidity]uiId3v2TagSize :0x%08x >= file_len :0x%08x"
#define MP3_GETFILEINFO_639_112_2_18_1_18_26_1635 "mp3_getfileinfo.c,[MP3_CheckFileValidity]total_len:0x%08x,id3v2 size:0x%08x,total_check_len:0x%08x"
#define MP3_GETFILEINFO_793_112_2_18_1_18_27_1636 "mp3_getfileinfo.c:MP3_ParseFileInfo:error:SCI_NULL == ptAudioObj"
#define MP3_GETFILEINFO_837_112_2_18_1_18_27_1637 "mp3_getfileinfo.c [MP3_ParseFileInfo] AUDIO_FILE_HANDLE !file handle:0x%0x"
#define MP3_GETFILEINFO_842_112_2_18_1_18_27_1638 "mp3_getfileinfo.c [MP3_ParseFileInfo] AUDIO_FILE_HANDLE !can not get music size!"
#define MP3_GETFILEINFO_855_112_2_18_1_18_27_1639 "mp3_getfileinfo.c [MP3_ParseFileInfo] AUDIO_BUFFER_HANDLE !buf ptr:0x%0x,buf len:0x%0x,buf cur offset:0x%0x"
#define MP3_GETFILEINFO_860_112_2_18_1_18_27_1640 "mp3_getfileinfo.c [MP3_ParseFileInfo] AUDIO_BUFFER_HANDLE !can not get music size!"
#define MP3_GETFILEINFO_886_112_2_18_1_18_27_1641 "mp3_getfileinfo.c [MP3_ParseFileInfo] mp3_format_info_ptr->uiSamplerate is %d, mp3_format_info_ptr->uiChannelNum is %d"
#define MP3_GETFILEINFO_901_112_2_18_1_18_27_1642 "mp3_getfileinfo.c [MP3_ParseFileInfo] AUDIO_STREAM_BUFFER_HANDLE !buf ptr:0x%0x,buf len:0x%0x,buf cur offset:0x%0x"
#define MP3_GETFILEINFO_907_112_2_18_1_18_27_1643 "mp3_getfileinfo.c: Unknown Handle Type!"
#define MP3_GETFILEINFO_932_112_2_18_1_18_27_1644 "mp3_getfileinfo.c,[MP3_ParseFileInfo] id3v2 tag offset:0,size:0x%08x"
#define MP3_GETFILEINFO_1018_112_2_18_1_18_27_1645 "mp3_getfileinfo.c,[MP3_ParseFileInfo] fake mp3!"
#define MP3_GETFILEINFO_1036_112_2_18_1_18_27_1646 "mp3_getfileinfo.c,[MP3_ParseFileInfo] id3v1 tag :offset:0x%08x,size:0x%08x"
#define MP3_GETFILEINFO_1057_112_2_18_1_18_27_1647 "mp3_getfileinfo.c,[MP3_ParseFileInfo] lyrics3 tag :offset:0x%08x,size:0x%08x"
#define MP3_GETFILEINFO_1079_112_2_18_1_18_27_1648 "mp3_getfileinfo.c,[MP3_ParseFileInfo]  ape tag :offset:0x%08x,size:0x%08x"
#define MP3_GETFILEINFO_1096_112_2_18_1_18_27_1649 "mp3_getfileinfo.c,[MP3_ParseFileInfo]   xing header :offset:0x%08x,size:0x%08x"
#define MP3_GETFILEINFO_1112_112_2_18_1_18_27_1650 "mp3_getfileinfo.c,[MP3_ParseFileInfo] lame tag  offset:0x%08x,size:0x%08x"
#define MP3_GETFILEINFO_1130_112_2_18_1_18_27_1651 "mp3_getfileinfo.c,[MP3_ParseFileInfo] encounter error when parsing vbr info!"
#define MP3_GETFILEINFO_1145_112_2_18_1_18_27_1652 "mp3_getfileinfo.c,[MP3_ParseFileInfo] mp3_format_info_ptr->uiTotalDataLength <= (file_tail_tag_size + mp3_ext_format_info_ptr->uiFirstFrameOffset"
#define MP3_GETFILEINFO_1159_112_2_18_1_18_27_1653 "mp3_getfileinfo.c,[MP3_ParseFileInfo] 0 == mp3_format_info_ptr->uiBitrate"
#define MP3_GETFILEINFO_1165_112_2_18_1_18_27_1654 "%s, %d, %d."
#define MP3_GETFILEINFO_1175_112_2_18_1_18_27_1655 "%s, %d, %d."
#define MP3_GETFILEINFO_1181_112_2_18_1_18_27_1656 "%s, %d, %d."
#define MP3_GETFILEINFO_1195_112_2_18_1_18_28_1657 "mp3_getfileinfo.c:CHNG_FREQ_REQ_HANDLER_NULL== s_mp3_parse_info_freq_handler"
#define MP3_GETFILEINFO_1211_112_2_18_1_18_28_1658 "peter:peter: MP3_CalcAveBitrate is %d"
#define MP3_GETFILEINFO_1214_112_2_18_1_18_28_1659 "mp3_getfileinfo.c,[MP3_ParseFileInfo] 0 == mp3_format_info_ptr->uiBitrate"
#define MP3_GETFILEINFO_1219_112_2_18_1_18_28_1660 "%s, %d, %d."
#define MP3_GETFILEINFO_1220_112_2_18_1_18_28_1661 "mp3_getfileinfo.c,[MP3_ParseFileInfo]  get playing time by average bitrate:%d! -1-!"
#define MP3_GETFILEINFO_1244_112_2_18_1_18_28_1662 "mp3_getfileinfo.c:CHNG_FREQ_REQ_HANDLER_NULL== s_mp3_parse_info_freq_handler"
#define MP3_GETFILEINFO_1262_112_2_18_1_18_28_1663 "mp3_getfileinfo.c,[MP3_ParseFileInfo] 0 == mp3_format_info_ptr->uiBitrate"
#define MP3_GETFILEINFO_1267_112_2_18_1_18_28_1664 "%s, %d, %d."
#define MP3_GETFILEINFO_1269_112_2_18_1_18_28_1665 "mp3_getfileinfo.c,[MP3_ParseFileInfo]  get playing time by average bitrate:%d! -2-!"
#define MP3_GETFILEINFO_1296_112_2_18_1_18_28_1666 "mp3_getfileinfo.c,[MP3_ParseFileInfo] format info printing BEGIN\n"
#define MP3_GETFILEINFO_1303_112_2_18_1_18_28_1667 "mp3_getfileinfo.c,--- mpegversion:%d,uiSamplesPerFrame:%d,uiFirstFrameOffset:0x%08x,vbr:%d,uiMusicDataLen:0x%08xn"
#define MP3_GETFILEINFO_1311_112_2_18_1_18_28_1668 "mp3_getfileinfo.c,--- uiTotalDataLength::0x%08x,uiTotalTime:%d(s),samplerate:%d,bitrate:%d,layer:%d,eChannelMode0x%08xn"
#define MP3_GETFILEINFO_1313_112_2_18_1_18_28_1669 "mp3_getfileinfo.c,[MP3_ParseFileInfo] format info printing END\n"
#define MP3_DECODER_PROXY_108_112_2_18_1_18_47_1670 "[MP3_ARM] dec_ret=%d, frame_buf_ptr=%d, len=%d, next_begin_%d, bitrate=%d"
#define MP3_DECODER_PROXY_109_112_2_18_1_18_47_1671 "[MP3_ARM]output_param_t.pcm_bytes=%d, channel_num=%d"
#define MP3_DECODER_PROXY_119_112_2_18_1_18_47_1672 "[MP3_ARM] MP3_ARM_DEC_ERROR_DECODING"
#define MP3_DECODER_PROXY_126_112_2_18_1_18_47_1673 "[MP3_ARM] MP3_ARM_DEC_ERROR_FRAMELEN"
#define MP3_DECODER_PROXY_129_112_2_18_1_18_47_1674 "[MP3_ARM] MP3_ARM_DEC_ERROR_NEXT_BEGIN"
#define MP3_DECODER_PROXY_132_112_2_18_1_18_47_1675 "[MP3_ARM] MP3_ARM_DEC_ERROR_BITRATE"
#define MP3_DEMUX_26_112_2_18_1_18_47_1676 "mp3_demux.c mp3_demux_open allocLen:%d."
#define MP3_DEMUX_53_112_2_18_1_18_48_1677 "mp3_demux.c mp3_demux_get_info allocLen:1536."
#define MP3_DMX_FORMAT_724_112_2_18_1_18_51_1678 "mp3_dmx_format.c:MP3_Dmx_GetPlayInfo:error:ptAudioObj == PNULL"
#define MP3_DMX_FORMAT_753_112_2_18_1_18_51_1679 "[MP3_GetPlayingInfo] Total time is:0x%08x"
#define MP3_DMX_FORMAT_780_112_2_18_1_18_51_1680 "mp3_adp.c:(0 == mp3_format_info_ptr->uiBitrate)||(0 == ptAudioObj->uiOutputSamplerate)"
#define MP3_DMX_FORMAT_796_112_2_18_1_18_51_1681 "(0 == mp3_ext_format_info_ptr->uiMusicDataLen)||(0 == ptAudioObj->uiOutputSamplerate)"
#define MP3_DMX_FORMAT_830_112_2_18_1_18_52_1682 "[MP3_GetPlayingInfo] exception: cur_time_1 < cur_time_2!"
#define MP3_DMX_FORMAT_865_112_2_18_1_18_52_1683 "[MP3_GetPlayingInfo], len--total:%d, cur:%d(byte), time--total:%d, cur:%d(s)."
#define MP3_DMX_FORMAT_981_112_2_18_1_18_52_1684 "mp3_dmx_format.c:MP3_Dmx_GetPlayInfo:error:mx_info_ptr == PNULL)||(ptAudioULL"
#define MP3_DMX_FORMAT_1004_112_2_18_1_18_52_1685 "mp3_dmx_format.c:MP3_Dmx_ParseFileInfo:error:PNULL == mp3_ext_format_info_ptr"
#define MP3_DMX_FORMAT_1016_112_2_18_1_18_52_1686 "[MP3_DMX_FORMAT] AUDIO_FILE_HANDLE !file handle:0x%0x"
#define MP3_DMX_FORMAT_1021_112_2_18_1_18_52_1687 "[MP3_DMX_FORMAT]:FILE SIZE =0"
#define MP3_DMX_FORMAT_1029_112_2_18_1_18_52_1688 "[MP3_DMX_FORMAT] AUDIO_BUFFER_HANDLE !file handle:buf ptr:0x%0x,buf len:0x%0x"
#define MP3_DMX_FORMAT_1034_112_2_18_1_18_52_1689 "[MP3_DMX_FORMAT]:BUFFER SIZE =0"
#define MP3_DMX_FORMAT_1048_112_2_18_1_18_52_1690 "[MP3_DMX_FORMAT]:STREAM BUF ,SCI_NULL == stream_data_ptr"
#define MP3_DMX_FORMAT_1056_112_2_18_1_18_52_1691 "[MP3_DMX_FORMAT]:STREAM BUF ,Get Format Info From APP,uiSamplerate=%d,uiChannelNum=%d"
#define MP3_DMX_FORMAT_1076_112_2_18_1_18_52_1692 "[MP3_DMX_FORMAT]:STREAM BUF ,Get Only Stream Data form APP,buf ptr=0x%0x,buf len=0x%0x"
#define MP3_DMX_FORMAT_1098_112_2_18_1_18_52_1693 "[MP3_ParseFileInfo] id3v2 tag offset:0,size:0x%08x"
#define MP3_DMX_FORMAT_1102_112_2_18_1_18_52_1694 "[MP3_ParseFileInfo] id3v2 tag no found"
#define MP3_DMX_FORMAT_1186_112_2_18_1_18_52_1695 "[MP3_ParseFileInfo] fake mp3!"
#define MP3_DMX_FORMAT_1205_112_2_18_1_18_52_1696 "[MP3_ParseFileInfo] id3v1 tag :offset:0x%08x,size:0x%08x"
#define MP3_DMX_FORMAT_1224_112_2_18_1_18_52_1697 "[MP3_ParseFileInfo] lyrics3 tag :offset:0x%08x,size:0x%08x"
#define MP3_DMX_FORMAT_1243_112_2_18_1_18_52_1698 "[MP3_ParseFileInfo]  ape tag :offset:0x%08x,size:0x%08x"
#define MP3_DMX_FORMAT_1262_112_2_18_1_18_52_1699 "[MP3_ParseFileInfo]   xing header :offset:0x%08x,size:0x%08x"
#define MP3_DMX_FORMAT_1279_112_2_18_1_18_52_1700 "[MP3_ParseFileInfo] lame tag  offset:0x%08x,size:0x%08x"
#define MP3_DMX_FORMAT_1297_112_2_18_1_18_52_1701 "[MP3_ParseFileInfo] encounter error when parsing vbr info!"
#define MP3_DMX_FORMAT_1311_112_2_18_1_18_52_1702 "[MP3_ParseFileInfo] encounter error mp3_format_info_ptr->uiTotalDataLength <=(file_tail_tag_size + mp3_ext_format_info_ptr->uiFirstFrameOffset)"
#define MP3_DMX_FORMAT_1324_112_2_18_1_18_53_1703 "[MP3_ParseFileInfo] encounter error 0 == mp3_format_info_ptr->uiBitrate)"
#define MP3_DMX_FORMAT_1329_112_2_18_1_18_53_1704 "[MP3_ParseFileInfo,1total time=%ds]"
#define MP3_DMX_FORMAT_1339_112_2_18_1_18_53_1705 "[MP3_ParseFileInfo,2total time=%ds]"
#define MP3_DMX_FORMAT_1345_112_2_18_1_18_53_1706 "MP3_ParseFileInfo,3total time=%ds]"
#define MP3_DMX_FORMAT_1363_112_2_18_1_18_53_1707 "[MP3_ParseFileInfo] encounter error 0 == aver_bitrate)"
#define MP3_DMX_FORMAT_1368_112_2_18_1_18_53_1708 "MP3_ParseFileInfo,4total time=%ds]"
#define MP3_DMX_FORMAT_1369_112_2_18_1_18_53_1709 "[MP3_ParseFileInfo]  get playing time by average bitrate:%d! -1-!"
#define MP3_DMX_FORMAT_1390_112_2_18_1_18_53_1710 "[MP3_ParseFileInfo] encounter error 0 == aver_bitrate)"
#define MP3_DMX_FORMAT_1396_112_2_18_1_18_53_1711 "MP3_ParseFileInfo,5total time=%ds]"
#define MP3_DMX_FORMAT_1397_112_2_18_1_18_53_1712 "[MP3_ParseFileInfo]  get playing time by average bitrate:%d! -1-!"
#define MP3_DMX_FORMAT_1421_112_2_18_1_18_53_1713 "mp3_demux_get_info: bitrate=%d, sample_rate=%d, layer=%d, channels=%d"
#define MP3_DMX_FORMAT_1438_112_2_18_1_18_53_1714 "[MP3_ParseFileInfo] format info printing BEGIN\n"
#define MP3_DMX_FORMAT_1445_112_2_18_1_18_53_1715 " mpegversion:%d,uiSamplesPerFrame:%d,uiFirstFrameOffset:0x%08x,vbr:%d,uiMusicDataLen:0x%08xn"
#define MP3_DMX_FORMAT_1453_112_2_18_1_18_53_1716 "uiTotalDataLength::0x%08x,uiTotalTime:%d(s),samplerate:%d,bitrate:%d,layer:%d,eChannelMode0x%08xn"
#define MP3_DMX_FORMAT_1455_112_2_18_1_18_53_1717 "[MP3_ParseFileInfo] format info printing END\n"
#define MP3_DMX_FORMAT_1514_112_2_18_1_18_53_1718 "[MP3_IsVBR]uiFirstFrameOffset :0x%08x >= file_len :0x%08x"
#define MP3_DMX_FORMAT_1618_112_2_18_1_18_53_1719 "[MP3_CalcAveBitrate]frame_count:%d,ave_bitrate_sum:%d kbps"
#define MP3_DMX_FORMAT_1679_112_2_18_1_18_53_1720 "[MP3_IsVBR]uiFirstFrameOffset :0x%08x >= file_len :0x%08x"
#define MP3_DMX_FORMAT_1824_112_2_18_1_18_53_1721 "[MP3_CheckFileValidity]uiId3v2TagSize :0x%08x >= file_len :0x%08x"
#define MP3_DMX_FORMAT_1847_112_2_18_1_18_54_1722 "[MP3_CheckFileValidity]total_len:0x%08x,id3v2 size:0x%08x,total_check_len:0x%08x"
#define MP3_DMX_FORMAT_1975_112_2_18_1_18_54_1723 "mp3_dmx_format.c:MP3_Dmx_iToc:error:puiBuf == PNULL"
#define MP3_DMX_FORMAT_1980_112_2_18_1_18_54_1724 "[MP3_Dmx_iToc]:input number: %d is not valid,(should be less than 9999) !"
#define MP3_DMX_FORMAT_2016_112_2_18_1_18_54_1725 "mp3_dmx_format.c:MP3_Dmx_cToi:(puiBuf == PNULL)||(puiNum== 0)"
#define MP3_DMX_FORMAT_2037_112_2_18_1_18_54_1726 "[MP3_DMX_FORMAT][MP3_CaculateNum]:i_calc:%d!not number!"
#define MP3_DMX_FORMAT_2066_112_2_18_1_18_54_1727 "mp3_dmx_format.c:MP3_Dmx_CheckheaderInfo:(SCI_NULL == puiHeader)||(SCI_NULL == ptMp3HeaderInfo)"
#define MP3_DMX_FORMAT_2213_112_2_18_1_18_54_1728 "mp3_dmx_format.c:MP3_Dmx_CheckXingHeader:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)"
#define MP3_DMX_FORMAT_2341_112_2_18_1_18_55_1729 "mp3_dmx_format.c:MP3_Dmx_CheckID3V1Tag:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)"
#define MP3_DMX_FORMAT_2391_112_2_18_1_18_55_1730 "[MP3_CheckID3V1Tag] V1.1:no valid year info!"
#define MP3_DMX_FORMAT_2408_112_2_18_1_18_55_1731 "[MP3_CheckID3V1Tag] V1.1:no valid genre info!"
#define MP3_DMX_FORMAT_2425_112_2_18_1_18_55_1732 "mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!"
#define MP3_DMX_FORMAT_2439_112_2_18_1_18_55_1733 "[MP3_CheckID3V1Tag]V1.0: no valid genre info!"
#define MP3_DMX_FORMAT_2475_112_2_18_1_18_55_1734 "mp3_dmx_format.c:MP3_Dmx_CheckID3V2Tag:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)"
#define MP3_DMX_FORMAT_2495_112_2_18_1_18_55_1735 "[MP3_CheckID3V2Tag]: ID3 FLAG NOT EXIST"
#define MP3_DMX_FORMAT_2507_112_2_18_1_18_55_1736 "[MP3_CheckID3V2Tag]:tag version:%d,tag size:0x%08x"
#define MP3_DMX_FORMAT_2526_112_2_18_1_18_55_1737 "[MP3_CheckID3V2Tag]:End of tag frames !"
#define MP3_DMX_FORMAT_2554_112_2_18_1_18_55_1738 "[MP3_CheckID3V2Tag]:frame id :%d ,frame_content_size:%d"
#define MP3_DMX_FORMAT_2565_112_2_18_1_18_55_1739 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_2591_112_2_18_1_18_55_1740 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_2617_112_2_18_1_18_55_1741 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_2643_112_2_18_1_18_55_1742 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_2669_112_2_18_1_18_55_1743 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_2695_112_2_18_1_18_55_1744 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_2721_112_2_18_1_18_55_1745 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_2766_112_2_18_1_18_55_1746 "[MP3_CheckID3V1Tag] V1.0:no valid year info!"
#define MP3_DMX_FORMAT_2771_112_2_18_1_18_55_1747 "[MP3_CheckID3V2Tag]:no valid genre index!"
#define MP3_DMX_FORMAT_2800_112_2_18_1_18_55_1748 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_2818_112_2_18_1_18_55_1749 "[MP3_CheckID3V1Tag] V1.0:no valid year info!"
#define MP3_DMX_FORMAT_2822_112_2_18_1_18_55_1750 "[MP3_CheckID3V2Tag]:year:impossible value!"
#define MP3_DMX_FORMAT_2834_112_2_18_1_18_55_1751 "[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,"
#define MP3_DMX_FORMAT_2852_112_2_18_1_18_56_1752 "mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!"
#define MP3_DMX_FORMAT_2856_112_2_18_1_18_56_1753 "mp3_format.c,[MP3_CheckID3V2Tag]:track:impossible value!"
#define MP3_DMX_FORMAT_2864_112_2_18_1_18_56_1754 "mp3_format.c,[MP3_CheckID3V2Tag]:other frame ID !We do not need the frame content!"
#define MP3_DMX_FORMAT_2872_112_2_18_1_18_56_1755 "[MP3_CheckID3V2Tag]find invalid frame ID!"
#define MP3_DMX_FORMAT_2905_112_2_18_1_18_56_1756 "mp3_dmx_format.c:MP3_Dmx_CheckAPETagHeader:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)"
#define MP3_DMX_FORMAT_2985_112_2_18_1_18_56_1757 "mp3_dmx_format.c:MP3_Dmx_CheckAPETagHeader:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)"
#define MP3_DMX_FORMAT_3007_112_2_18_1_18_56_1758 "[MP3_CheckLameTag] no valid version info!"
#define MP3_DMX_FORMAT_3097_112_2_18_1_18_56_1759 "mp3_dmx_format.c:MP3_Dmx_CheckAPETagHeader:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)"
#define MP3_DMX_FORMAT_3140_112_2_18_1_18_56_1760 "[MP3_CheckLyrics3Tag] tag size info error!can not get tag size!"
#define MP3_DMX_FORMAT_3178_112_2_18_1_18_56_1761 "[MP3_CheckLyrics3Tag] can not alloc memory!"
#define MP3_DSP_ADP_518_112_2_18_1_18_58_1762 "[MP3_DSP_CODEC]MP3_DSP_Construct, PNULL == adp_res_ptr"
#define MP3_DSP_ADP_523_112_2_18_1_18_58_1763 "[MP3_DSP_CODEC]MP3_DSP_Construct, allocLen:%d."
#define MP3_DSP_ADP_533_112_2_18_1_18_58_1764 "[MP3_DSP_CODEC]MP3_DSP_Construct, PNULL == audio_format_info_ptr->pvOtherInfo"
#define MP3_DSP_ADP_539_112_2_18_1_18_58_1765 "[MP3_DSP_CODEC]MP3_DSP_Construct, allocLen:%d."
#define MP3_DSP_ADP_549_112_2_18_1_18_58_1766 "[MP3_DSP_CODEC]MP3_DSP_Construct,MP3_DECODE_MAX_PCM_SIZE < MP3_DEMUX_MAX_FRAME_SIZE"
#define MP3_DSP_ADP_572_112_2_18_1_18_58_1767 "[MP3_DSP_CODEC]MP3_DSP_Construct, allocLen:%d."
#define MP3_DSP_ADP_683_112_2_18_1_18_58_1768 "[MP3_DSP_CODEC]MP3_DSP_PlayConstruct,AUDIO_DEFAULT_TASK_ID != audio_obj_ptr->tThreadId)||(AUDIO_DEFAULT_TASK_ID== audio_obj_ptr->tAssiThreadId)"
#define MP3_DSP_ADP_721_112_2_18_1_18_58_1769 "[MP3_DSP_CODEC]MP3_DSP_Play, ThreadID:0x%x,0x%x"
#define MP3_DSP_ADP_731_112_2_18_1_18_58_1770 "[MP3_DSP_CODEC]MP3_DSP_Play, CHNG_FREQ_REQ_HANDLER_NULL == s_ui_chng_freq_req_handler!"
#define MP3_DSP_ADP_953_112_2_18_1_18_59_1771 "[MP3_DSP_CODEC]MP3_DSP_ResumeConstruct,AUDIO_DEFAULT_TASK_ID != audio_obj_ptr->tThreadId)||(AUDIO_DEFAULT_TASK_ID== audio_obj_ptr->tAssiThreadId)"
#define MP3_DSP_ADP_990_112_2_18_1_18_59_1772 "[MP3_DSP_CODEC]MP3_DSP_Play, ThreadID:0x%x,0x%x"
#define MP3_DSP_ADP_1000_112_2_18_1_18_59_1773 "[MP3_DSP_CODEC]MP3_DSP_Play, CHNG_FREQ_REQ_HANDLER_NULL == s_ui_chng_freq_req_handler!"
#define MP3_DSP_ADP_1169_112_2_18_1_18_59_1774 "[MP3_DSP] MP3_DSP_Play (AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId)||(AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tAssiThreadId)"
#define MP3_DSP_ADP_1192_112_2_18_1_18_59_1775 "MP3_DSP_Stop,caller=0x%x"
#define MP3_DSP_ADP_1208_112_2_18_1_18_59_1776 "[MP3_DSP] MP3_DSP_Stop (AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId)||(AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tAssiThreadId)"
#define MP3_DSP_ADP_1235_112_2_18_1_18_59_1777 "[MP3_DSP] MP3_DSP_Pause (AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId)||(AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tAssiThreadId)"
#define MP3_DSP_ADP_1261_112_2_18_1_18_59_1778 "[MP3_DSP] MP3_DSP_Resume (AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId)||(AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tAssiThreadId)"
#define MP3_DSP_ADP_1288_112_2_18_1_19_0_1779 "[MP3_DSP] MP3_TimeSeek (AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tAssiThreadId)"
#define MP3_DSP_ADP_1339_112_2_18_1_19_0_1780 "[MP3_TimeSeek] seek_offset=%d, seek_time=%d"
#define MP3_DSP_ADP_1353_112_2_18_1_19_0_1781 "[MP3_DSP] SEEK IS NO MEANING FOR THIS TIME"
#define MP3_DSP_ADP_1374_112_2_18_1_19_0_1782 "[MP3_DSP] MP3_DSP_Seek,uiOffset=%d"
#define MP3_DSP_ADP_1382_112_2_18_1_19_0_1783 "[MP3_DSP] MP3_DSP_Seek,mp3_ext_format_info_ptr is null"
#define MP3_DSP_ADP_1389_112_2_18_1_19_0_1784 "[MP3_DSP] MP3_DSP_Seek, clean the cache."
#define MP3_DSP_ADP_1455_112_2_18_1_19_0_1785 "[MP3_DSP] SEEK is not support by tAssiThreadId now"
#define MP3_DSP_ADP_1462_112_2_18_1_19_0_1786 "[MP3_DSP] MP3_DSP_Seek, state=%d"
#define MP3_DSP_ADP_1469_112_2_18_1_19_0_1787 "[MP3_DSP] SEEK IS NO MEANING FOR THIS TIME"
#define MP3_DSP_ADP_1500_112_2_18_1_19_0_1788 "mp3_dsp_adp.c:MP3_DSP_IdentifyAudioFormat:error:PNULL == adp_res_ptr"
#define MP3_DSP_ADP_1564_112_2_18_1_19_0_1789 "mp3_dsp_adp.c[MP3_DSP_GetPlayingInfo] stream, curTime:%d=%d(=%d+%d)-%d(=%d(=%d*%d)+%d(=%d+%d)).uiOutPos %d, uiDecPos %d"
#define MP3_DSP_ADP_1713_112_2_18_1_19_0_1790 "[MP3_DSP]DSP DECODE INIT"
#define MP3_DSP_ADP_1718_112_2_18_1_19_0_1791 "[MP3_DSP]DSP DECODE INIT DONE"
#define MP3_DSP_ADP_1754_112_2_18_1_19_0_1792 "[MP3_DSP]PLAY END!"
#define MP3_DSP_ADP_1779_112_2_18_1_19_0_1793 "[MP3_DSP]PLAY END!"
#define MP3_DSP_ADP_1822_112_2_18_1_19_1_1794 "[MP3_DSP]Error Frame Length %d, channel %d!"
#define MP3_DSP_ADP_1830_112_2_18_1_19_1_1795 "[MP3_DSP]PLAY END!"
#define MP3_DSP_ADP_1889_112_2_18_1_19_1_1796 "[MP3_DSP]Error Frame Length %d, channel %d!"
#define MP3_DSP_ADP_1897_112_2_18_1_19_1_1797 "[MP3_DSP]PLAY END!"
#define MP3_DSP_ADP_1947_112_2_18_1_19_1_1798 "[MP3_DSP]PLAY END!"
#define MP3_DSP_ADP_1961_112_2_18_1_19_1_1799 "[MP3_DSP]PAUSE BUT NOW DEMUX TASK IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN"
#define MP3_DSP_ADP_1969_112_2_18_1_19_1_1800 "[MP3_DSP]PAUSE SUCCESS!"
#define MP3_DSP_ADP_1977_112_2_18_1_19_1_1801 "[MP3_DSP]PAUSE BUT NOW DSP IS DECODE, WAIT SOME TIME THEN TRY AGAIN"
#define MP3_DSP_ADP_1994_112_2_18_1_19_1_1802 "[MP3_DSP] stop waiting for track buffer empty"
#define MP3_DSP_ADP_2002_112_2_18_1_19_1_1803 "[MP3_DSP]STOP BUT NOW DEMUX TASK IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN"
#define MP3_DSP_ADP_2010_112_2_18_1_19_1_1804 "[MP3_DSP]STOP SUCCESS!"
#define MP3_DSP_ADP_2018_112_2_18_1_19_1_1805 "[MP3_DSP]STOP BUT NOW DSP IS DECODE, WAIT SOME TIME THEN TRY AGAIN"
#define MP3_DSP_ADP_2075_112_2_18_1_19_1_1806 "[MP3_DSP]:demux_res_ptr->bIsDemuxStop!"
#define MP3_DSP_ADP_2081_112_2_18_1_19_1_1807 "[MP3_DSP]:MP3_DSP_DEMUX_FRAME!"
#define MP3_DSP_ADP_2092_112_2_18_1_19_1_1808 "[MP3_DSP]:DEMUX INIT OK!"
#define MP3_DSP_ADP_2160_112_2_18_1_19_1_1809 "[MP3_DSP]:DEMUX Resume INIT OK!"
#define MP3_DSP_ADP_2197_112_2_18_1_19_1_1810 "[MP3_DSP_DEMUX]:MP3_DSP_SEEK,clean stream buffer,%d"
#define MP3_DSP_ADP_2234_112_2_18_1_19_1_1811 "[MP3_DSP]:DEMUX THREAD RECV A BAD SIGNAL, DISCARD IT"
#define MP3_DSP_ADP_2344_112_2_18_1_19_2_1812 "[MP3_DSP]:DEMUX _MP3_DSP_DEMUX_Init CHNG_FREQ_REQ_HANDLER_NULL == s_ui_chng_freq_req_handler"
#define MP3_DSP_ADP_2426_112_2_18_1_19_2_1813 "_MP3_DSP_ReadDemuxFrame no mp3 data."
#define MP3_DSP_ADP_2450_112_2_18_1_19_2_1814 "_MP3_DSP_Stream_CastOff_SrcDataInfo para error:%d, %d."
#define MP3_DSP_ADP_2467_112_2_18_1_19_2_1815 "_MP3_DSP_Stream_CastOff_SrcDataInfo,CastOffDataLen:%d;info_list=(%d,%d,%d,%d)"
#define MP3_DSP_ADP_2487_112_2_18_1_19_2_1816 "_MP3_DSP_Stream_CastOff_SrcDataInfo 0 CastOffDataInfo:%d, %d, %d."
#define MP3_DSP_ADP_2506_112_2_18_1_19_2_1817 "_MP3_DSP_Stream_CastOff_SrcDataInfo 1 CastOffDataInfo:%d, %d, %d."
#define MP3_DSP_ADP_2526_112_2_18_1_19_2_1818 "_MP3_DSP_Stream_CastOff_SrcDataInfo 2 CastOffDataInfo:%d, %d, %d."
#define MP3_DSP_ADP_2551_112_2_18_1_19_2_1819 "_MP3_DSP_Stream_CastOff_SrcDataInfo 3 CastOffDataInfo:%d, %d, %d, leftDataLen:%d."
#define MP3_DSP_ADP_2568_112_2_18_1_19_2_1820 "_MP3_DSP_Stream_CastOff_SrcDataInfo realCastoff:%d, wantToCastOff:%d, lastPlayTime:%d."
#define MP3_DSP_ADP_2577_112_2_18_1_19_2_1821 "_MP3_DSP_Stream_CastOff_SrcDataInfo, ListInfo:%d, %d, %d, %d."
#define MP3_DSP_ADP_2591_112_2_18_1_19_2_1822 "_MP3_DSP_WriteDemuxFrame, iMp3Demuxresult = %d"
#define MP3_DSP_ADP_2628_112_2_18_1_19_2_1823 "_MP3_DSP_WriteDemuxFrame, demux frame out is full."
#define MP3_DSP_ADP_2649_112_2_18_1_19_2_1824 "_MP3_DSP_ResetDemuxFrames, uiOffsetTime %d, uiOutPos %d"
#define MP3_DSP_ADP_2743_112_2_18_1_19_2_1825 "[_MP3_DSP_DecodeFrame] NULL == s_cur_audio_object_ptr"
#define MP3_DSP_ADP_2761_112_2_18_1_19_2_1826 "[_MP3_DSP_DecodeFrame] dec_result_error=%d"
#define MP3_DSP_ADP_2785_112_2_18_1_19_2_1827 "[_MP3_DSP_DecodeFrameCallback] NULL == s_cur_audio_object_ptr"
#define MP3_DSP_ADP_2792_112_2_18_1_19_2_1828 "[_MP3_DSP_DecodeFrameCallback] stop waiting for track buffer empty"
#define MP3_DSP_ADP_2799_112_2_18_1_19_2_1829 "[_MP3_DSP_DecodeFrameCallback] NULL == mp3_ext_format_info_ptr"
#define MP3_DSP_ADP_2807_112_2_18_1_19_3_1830 "[MP3_DSP]Error Frame Length %d, channel %d!"
#define MP3_DSP_ADP_2966_112_2_18_1_19_3_1831 "[MP3_DSP]PLAY END!"
#define MP3_DSP_ADP_2992_112_2_18_1_19_3_1832 "[MP3_DSP]PLAY END!"
#define MP3_DSP_ADP_3046_112_2_18_1_19_3_1833 "_MP3_DSP_Stream_AddDataToCache,cache_offset:%d,packet_size=%d"
#define MP3_DSP_ADP_3075_112_2_18_1_19_3_1834 "_MP3_DSP_Stream_AddInfoToList, StreamList:(%d;%d,%d;%d);cur_packet(%d,%d)"
#define MP3_DSP_ADP_3160_112_2_18_1_19_3_1835 "mp3_dsp_adp.c:_MP3_DSP_Demux_GetData:error:fileCacheBuf== PNULL"
#define MP3_DSP_ADP_3266_112_2_18_1_19_3_1836 "mp3_dsp_adp.c:_MP3_DSP_Demux_GetData:error:(pucsBuffer == PNULL)||(uiBufSize == 0)"
#define MP3_DSP_ADP_3318_112_2_18_1_19_4_1837 "_MP3_DSP_Demux_GetData:error:ui_cb_offset(%d) < uiSrcBufBegin(%d)"
#define MP3_DSP_ADP_3342_112_2_18_1_19_4_1838 "_MP3_DSP_Demux_GetData,list is full,(in,out,totallen)=(%d,%d,%d)"
#define MP3_DSP_ADP_3406_112_2_18_1_19_4_1839 "_MP3_DSP_Demux_GetData,get_src_result=%d"
#define MP3_DSP_ADP_3466_112_2_18_1_19_4_1840 "[MP3_DSP]:ERROR :this singal is sent to decode task"
#define MP3_DSP_ADP_3488_112_2_18_1_19_4_1841 "[MP3_DSP]:ERROR :this singal is sent to decode task"
#define MP3_DSP_ADP_3507_112_2_18_1_19_4_1842 "[MP3_DSP]:ERROR :this singal is sent to decode task"
#define MP3_DSP_ADP_3527_112_2_18_1_19_4_1843 "[MP3_DSP]:ERROR :this singal is sent to decode task"
#define MP3_DSP_ADP_3546_112_2_18_1_19_4_1844 "[MP3_DSP]:ERROR :this singal is sent to decode task"
#define MP3_DSP_ADP_3565_112_2_18_1_19_4_1845 "[MP3_DSP]:ERROR :this singal is sent to decode task"
#define MP3_DSP_ADP_3584_112_2_18_1_19_4_1846 "[MP3_DSP]:ERROR :this singal is sent to decode task"
#define MP3_DSP_ADP_3602_112_2_18_1_19_4_1847 "[MP3_DSP]:ERROR :this singal is sent to decode task"
#define MP3_DSP_ADP_3622_112_2_18_1_19_4_1848 "[MP3_DSP]:ERROR :this singal is sent to decode task"
#define MP3_DSP_ADP_3640_112_2_18_1_19_4_1849 "[MP3_DSP]:ERROR :this singal is sent to decode task"
#define MP3_DSP_ADP_3660_112_2_18_1_19_4_1850 "[MP3_DSP]:ERROR :this singal is sent to decode task"
#define MP3_DSP_ADP_3680_112_2_18_1_19_4_1851 "[MP3_DSP]:ERROR :this singal is sent to decode task"
#define MP3_DSP_ADP_3688_112_2_18_1_19_4_1852 "[MP3_DSP]:ERROR :_MP3_DSP_SendSig SEND A UNKOWNED SIG!"
#define MP3_MUX_38_112_2_18_1_19_26_1853 "peter: mp3_mux_one_frame in frame len is %d, mp3_mux->file_handle is %d"
#define MP3_MUX_47_112_2_18_1_19_26_1854 "peter: mp3_mux_one_frame in 2 frame len  is %d, bytes_written is %d"
#define MP3_MUX_81_112_2_18_1_19_26_1855 "mp3_mux.c: mp3_mux_open in"
#define MP3_MUX_111_112_2_18_1_19_26_1856 "mp3_mux.c:stream_new failed"
#define WAVE_MUX_142_112_2_18_1_19_26_1857 "wave_mux.c: wave_mux_open in"
#define WAVE_MUX_162_112_2_18_1_19_26_1858 "wave_mux.c: error:mux_res null"
#define WAVE_MUX_174_112_2_18_1_19_26_1859 "wave_mux.c:stream_new failed"
#define NULL_CODEC_ADP_90_112_2_18_1_19_27_1860 "null_codec_adp.c:NULL_CODEC_IdentifyFormat:error:ptAudioObject == SCI_NULL"
#define RA8LBR_ADP_288_112_2_18_1_19_43_1861 "[RA8LBR_Construct] Alloc failed, left_exp_data_ptr==NULL"
#define RA8LBR_ADP_299_112_2_18_1_19_43_1862 "[RA8LBR_Construct] Alloc failed, right_exp_data_ptr==NULL"
#define RA8LBR_ADP_310_112_2_18_1_19_43_1863 "[RA8LBR_Construct] Alloc failed, ra8lbr_res_ptr==NULL"
#define RA8LBR_ADP_320_112_2_18_1_19_43_1864 "[RA8LBR_Construct] Alloc failed, ra8lbr_res_ptr->lbr_mem_ptr==NULL"
#define RA8LBR_ADP_333_112_2_18_1_19_43_1865 "[RA8LBR_Construct] aud_obj_ptr->pGetSrcCallbackFunc==NULL"
#define RA8LBR_ADP_341_112_2_18_1_19_43_1866 "[RA8LBR_Construct] ra8lbr_res_ptr->sub_type==default"
#define RA8LBR_ADP_365_112_2_18_1_19_43_1867 "[RA8LBR_Construct] Create RA8LBR thread failed."
#define RA8LBR_ADP_444_112_2_18_1_19_43_1868 "[RA8LBR_PlayConstruct] CHNG_FREQ_REQ_HANDLER_NULL == s_ui_ra8lbr_chng_freq_req_handler"
#define RA8LBR_ADP_493_112_2_18_1_19_43_1869 "[RA8LBR_ResumeConstruct] CHNG_FREQ_REQ_HANDLER_NULL == s_ui_ra8lbr_chng_freq_req_handler"
#define RA8LBR_ADP_537_112_2_18_1_19_43_1870 "[RA8LBR_Play] NULL == sig_ptr"
#define RA8LBR_ADP_550_112_2_18_1_19_43_1871 "[RA8LBR_Play] start to play."
#define RA8LBR_ADP_568_112_2_18_1_19_43_1872 "[RA8LBR_Stop] NULL == sig_ptr"
#define RA8LBR_ADP_576_112_2_18_1_19_43_1873 "[RA8LBR_Stop] stopped."
#define RA8LBR_ADP_594_112_2_18_1_19_43_1874 "[RA8LBR_Pause] NULL == sig_ptr"
#define RA8LBR_ADP_602_112_2_18_1_19_43_1875 "[RA8LBR_Pause] pause."
#define RA8LBR_ADP_621_112_2_18_1_19_43_1876 "[RA8LBR_Resume] NULL == sig_ptr"
#define RA8LBR_ADP_629_112_2_18_1_19_43_1877 "[RA8LBR_Resume] resume."
#define RA8LBR_ADP_663_112_2_18_1_19_43_1878 "[RA8LBR_IdentifyFormat] error:NULL == ra8lbr_res_ptr"
#define RA8LBR_ADP_675_112_2_18_1_19_43_1879 "[RA8LBR_IdentifyFormat] error:NULL == cfg_info_ptr"
#define RA8LBR_ADP_723_112_2_18_1_19_43_1880 "[RA8LBR_GetPlayingInfo] stream, curTime:%d, %d should not less than:%d(=%d+%d)."
#define RA8LBR_ADP_741_112_2_18_1_19_43_1881 "[RA8LBR_GetOutputSamplerate] OutputSampleRate=%d"
#define RA8LBR_ADP_769_112_2_18_1_19_44_1882 "[RA8LBR_SendSig] RA8LBR_DECODING, SCI_ALLOCA fail"
#define RA8LBR_ADP_790_112_2_18_1_19_44_1883 "[RA8LBR_SendSig] RA8LBR_STOP, SCI_ALLOCA fail"
#define RA8LBR_ADP_839_112_2_18_1_19_44_1884 "[RA8LBR_InitDecoder] Error: dec_result=%d"
#define RA8LBR_ADP_850_112_2_18_1_19_44_1885 "sample_num=%d, frame_size=%d, sample_rate=%d, region_num=%d, channel_num=%d, cplStart=%d, cplQBits=%d"
#define RA8LBR_ADP_852_112_2_18_1_19_44_1886 "[RA8LBR_InitDecoder] Init successfully."
#define RA8LBR_ADP_915_112_2_18_1_19_44_1887 "[pGetSrcCallbackFunc] 0 == ra8lbr_res_ptr->src_data_len"
#define RA8LBR_ADP_936_112_2_18_1_19_44_1888 "[pGetSrcCallbackFunc] AUDIO_STREAM_GET_SRC_WAIT"
#define RA8LBR_ADP_941_112_2_18_1_19_44_1889 "[pGetSrcCallbackFunc] AUDIO_STREAM_GET_SRC_ASY"
#define RA8LBR_ADP_945_112_2_18_1_19_44_1890 "[pGetSrcCallbackFunc] AUDIO_STREAM_GET_SRC_ERROR"
#define RA8LBR_ADP_975_112_2_18_1_19_44_1891 "LBR_DEC_FrameDecode ERROR!!!"
#define ADPCM_ENCODER_362_112_2_18_1_19_45_1892 "AdpcmEncoderSetSamplerate Error : %d"
#define ADPCM_ENCODER_451_112_2_18_1_19_45_1893 "The header size of wav file for ADCPM is %d, not 60 bytes!\n"
#define ADPCMRECORD_457_112_2_18_1_19_46_1894 "[Record_audio_init]: 0x%x, %x, %x."
#define ADPCMRECORD_529_112_2_18_1_19_47_1895 "adpcmrecord.c:Record_SetParam:error:SET_COUNT)||(argv==SCI_NULL"
#define ADPCMRECORD_535_112_2_18_1_19_47_1896 "[Record_SetParam]:It is recording."
#define ADPCMRECORD_555_112_2_18_1_19_47_1897 "[Record_SetParam] mp3 record set param DONE."
#define ADPCMRECORD_575_112_2_18_1_19_47_1898 "adpcmrecord.c:Record_SetParam:error:0x%x, 0x%x, 0x%x, 0x%x, 0x%x"
#define ADPCMRECORD_593_112_2_18_1_19_47_1899 "adpcmrecord.c:Record_SetParam:0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x"
#define ADPCMRECORD_615_112_2_18_1_19_47_1900 "adpcmrecord.c:Record_SetIfFromNet:error:ERROR_COUNT)||(argv==SCI_NULL"
#define ADPCMRECORD_621_112_2_18_1_19_47_1901 "[Record_SetIfFromNet]:It is recording."
#define ADPCMRECORD_627_112_2_18_1_19_47_1902 "[Record_SetIfFromNet]:%d."
#define ADPCMRECORD_647_112_2_18_1_19_47_1903 "adpcmrecord.c:Record_GetRecordTime:error:ME_COUNT)||(argv==SCI_NULL"
#define ADPCMRECORD_660_112_2_18_1_19_47_1904 "[Record_GetRecordTime] NOT RECORDING."
#define ADPCMRECORD_679_112_2_18_1_19_47_1905 "adpcmrecord.c:Record_GetVoiceDB:error:ME_COUNT)||(argv==SCI_NULL"
#define ADPCMRECORD_692_112_2_18_1_19_47_1906 "[Record_GetVoiceDB] NOT RECORDING."
#define ADPCMRECORD_709_112_2_18_1_19_47_1907 "[Record_GetCurFileSize] INPUT PARAM ERROR!"
#define ADPCMRECORD_729_112_2_18_1_19_47_1908 "[Record_SetEndTime] INPUT PARAM ERROR!"
#define ADPCMRECORD_733_112_2_18_1_19_47_1909 "[Record_SetEndTime]  %d !"
#define ADPCMRECORD_751_112_2_18_1_19_47_1910 "[Record_SetCBTime] INPUT PARAM ERROR!"
#define ADPCMRECORD_757_112_2_18_1_19_47_1911 "[Record_SetCBTime] %d, 0x%x!"
#define ADPCMRECORD_774_112_2_18_1_19_47_1912 "[Record_SetEndSize] INPUT PARAM ERROR!"
#define ADPCMRECORD_794_112_2_18_1_19_47_1913 "[AUD_RecordStart] entry point!"
#define ADPCMRECORD_799_112_2_18_1_19_47_1914 "AUD_RecordStart]: hf is null"
#define ADPCMRECORD_805_112_2_18_1_19_47_1915 "[AUD_RecordStart] error: it is recording"
#define ADPCMRECORD_829_112_2_18_1_19_47_1916 "AUD_RecordStart input para error, %d <= %d (byte)."
#define ADPCMRECORD_838_112_2_18_1_19_47_1917 "AUD_RecordStart input para error, %d < %d * %d(byte)."
#define ADPCMRECORD_857_112_2_18_1_19_47_1918 "[AUD_RecordStart] AUD_REC_AMR_SW"
#define ADPCMRECORD_880_112_2_18_1_19_47_1919 "[AUD_RecordStart] AUD_REC_MP3"
#define ADPCMRECORD_891_112_2_18_1_19_47_1920 "[AUD_RecordStart] AUD_REC_PCM"
#define ADPCMRECORD_932_112_2_18_1_19_47_1921 "AUD_RecordStart SCI_CreateMutex RecordTimer failed."
#define ADPCMRECORD_946_112_2_18_1_19_47_1922 "[AUD_RecordStart] SCI_NULL == s_notify_timer_ptr"
#define ADPCMRECORD_956_112_2_18_1_19_47_1923 "AUD_RecordStart SCI_CreateMutex failed."
#define ADPCMRECORD_986_112_2_18_1_19_47_1924 "AdpcmRecord.c Record: Send signal to start record."
#define ADPCMRECORD_1008_112_2_18_1_19_48_1925 "mp3 encoder init start here..."
#define ADPCMRECORD_1023_112_2_18_1_19_48_1926 "sample_rate=%d, bit_rate=%d, ch_count=%d, ms_sign=%d, vbr_sign=%d, cut_off=%d"
#define ADPCMRECORD_1028_112_2_18_1_19_48_1927 "MP3_ENC_MemoryAlloc failed."
#define ADPCMRECORD_1034_112_2_18_1_19_48_1928 "mp3 encoder init DONE"
#define ADPCMRECORD_1084_112_2_18_1_19_48_1929 "[Record_Mp3BufAlloc] mp3_rec_buf=0x%x, mp3_rec_buf_len=%d, mp3_ref_src_buf=0x%x, mp3_ref_src_buf_len=%d,mp3_temp_buf=0x%x, mp3_temp_buf_len=%d"
#define ADPCMRECORD_1099_112_2_18_1_19_48_1930 "[Record_Mp3BufFree] Error: NULL == s_mp3_enc_buf."
#define ADPCMRECORD_1105_112_2_18_1_19_48_1931 "[Record_Mp3BufFree] Free DONE."
#define ADPCMRECORD_1118_112_2_18_1_19_48_1932 "Record_Mp3Finished done here..."
#define ADPCMRECORD_1136_112_2_18_1_19_48_1933 "[Record_Mp3EncExit] stream_len=%d"
#define ADPCMRECORD_1154_112_2_18_1_19_48_1934 "[Record_Mp3EncFrame] temp_count =%d, read_count=%d,ref_buf_get_count=%d"
#define ADPCMRECORD_1195_112_2_18_1_19_48_1935 "[Record_Mp3EncFrame] temp_count=%d"
#define ADPCMRECORD_1210_112_2_18_1_19_48_1936 "[Record_Mp3EncFrame]  enc_done, enc_result=%d, stream_len=%d"
#define ADPCMRECORD_1242_112_2_18_1_19_48_1937 "[AUD_Record_StartRecord] Entry point!!"
#define ADPCMRECORD_1244_112_2_18_1_19_48_1938 "[AUD_Record_StartRecord] minBufsize=%d, trackbufsize=%d (bytes)."
#define ADPCMRECORD_1250_112_2_18_1_19_48_1939 "adpcmrecord.c:AUD_Record_StartRecord:error:	SCI_NULL == ptAudioObjBak->puiTrackBuffer"
#define ADPCMRECORD_1257_112_2_18_1_19_48_1940 "adpcmrecord.c:AUD_Record_StartRecord:error:%d,%d"
#define ADPCMRECORD_1266_112_2_18_1_19_48_1941 "[AUD_Record_StartRecord] Init buf error!"
#define ADPCMRECORD_1281_112_2_18_1_19_48_1942 "[AUD_Record_StartRecord] set zero before 0x %x, len:%d(bytes)."
#define ADPCMRECORD_1282_112_2_18_1_19_48_1943 "[AUD_Record_StartRecord] set zero before 0x %x, len:%d(bytes)."
#define ADPCMRECORD_1283_112_2_18_1_19_48_1944 "[AUD_Record_StartRecord] set zero before 0x %x, len:%d(bytes)"
#define ADPCMRECORD_1340_112_2_18_1_19_48_1945 "[AUD_Record_StartRecord] set record samplerate %d."
#define ADPCMRECORD_1348_112_2_18_1_19_48_1946 "adpcmrecord.c:AUD_Record_StartRecord:error:8000 != ptAudioObjBak->uiOutputSamplerate"
#define ADPCMRECORD_1368_112_2_18_1_19_48_1947 "adpcmrecord.c:AUD_Record_StartRecord:error:CHNG_FREQ_REQ_HANDLER_NULL == s_record_chng_freq_req_handler"
#define ADPCMRECORD_1391_112_2_18_1_19_48_1948 "adpcmrecord.c:AMR_ENC_Init:error!"
#define ADPCMRECORD_1414_112_2_18_1_19_48_1949 "[AUD_Record_StartRecord] s_notify_timer_ptr IS ACTIVE."
#define ADPCMRECORD_1449_112_2_18_1_19_48_1950 "adpcmrecord.c:AUD_Record_StartRecord:error:PNULL == s_record_device->tDe"
#define ADPCMRECORD_1488_112_2_18_1_19_48_1951 "[AUD_Record_RecordResume] s_notify_timer_ptr IS NOT ACTIVE."
#define ADPCMRECORD_1525_112_2_18_1_19_49_1952 "[AUD_Record_RecordResume] s_notify_timer_ptr IS ACTIVE."
#define ADPCMRECORD_1564_112_2_18_1_19_49_1953 "[AUD_Record_RecordStop] set s_mmi_record_stop"
#define ADPCMRECORD_1568_112_2_18_1_19_49_1954 "s_mmi_record_stop=TRUE, MULTI STOP!!!"
#define ADPCMRECORD_1603_112_2_18_1_19_49_1955 "[ AUD_Record_RecordStop] write fail, then send signal to write header"
#define ADPCMRECORD_1614_112_2_18_1_19_49_1956 "[AUD_Record_RecordStop] s_is_start_read_data:%d"
#define ADPCMRECORD_1669_112_2_18_1_19_49_1957 "[AUD_L1RecordCallback] record codec has stop recording."
#define ADPCMRECORD_1675_112_2_18_1_19_49_1958 "%s, %d, data_len:%d, should be:%d."
#define ADPCMRECORD_1715_112_2_18_1_19_49_1959 "[Record_SendCodeSignal] entry point!"
#define ADPCMRECORD_1730_112_2_18_1_19_49_1960 "[Record_SendCodeSignal] state: %d, %d, %d"
#define ADPCMRECORD_1750_112_2_18_1_19_49_1961 "[Record_TimerNotify] SCI_NULL == s_notify_callback"
#define ADPCMRECORD_1856_112_2_18_1_19_49_1962 "REF_WriteVoiceData, %04x,%04x,%04x,%04x,%04x,%04x,%04x,%04x."
#define ADPCMRECORD_1969_112_2_18_1_19_49_1963 "[REF_GetNewVoiceData] s_mmi_record_stop=1"
#define ADPCMRECORD_1980_112_2_18_1_19_49_1964 "[REF_GetNewVoiceData]write last buffer"
#define ADPCMRECORD_2024_112_2_18_1_19_49_1965 "[AUD_RecordPause] Send signal to pause record."
#define ADPCMRECORD_2064_112_2_18_1_19_50_1966 "[AUD_RecordResume] Send signal to resume record."
#define ADPCMRECORD_2068_112_2_18_1_19_50_1967 "[AUD_RecordResume] :s_mmi_record_stop  has stopped"
#define ADPCMRECORD_2111_112_2_18_1_19_50_1968 "AdpcmRecord.c Record: Send signal to stop record."
#define ADPCMRECORD_2132_112_2_18_1_19_50_1969 "[AUD_RecordStop] s_mmi_record_stop=%d, s_is_recording=%d"
#define ADPCMRECORD_2138_112_2_18_1_19_50_1970 "[AUD_RecordStop] 2 s_mmi_record_stop=%d, s_is_recording=%d, lost:%d(frames), frameCountIQHeaderNo5a5a:%d."
#define ADPCMRECORD_2158_112_2_18_1_19_50_1971 "Record_Output_Print len:%d, common log"
#define ADPCMRECORD_2180_112_2_18_1_19_50_1972 "Voice_Dump_Pcm_Data_Callback data:%02d,%02d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d"
#define ADPCMRECORD_2184_112_2_18_1_19_50_1973 "%05d"
#define ADPCMRECORD_2190_112_2_18_1_19_50_1974 "Record_Output_Print len:%d of dsp log."
#define ADPCMRECORD_2197_112_2_18_1_19_50_1975 "Record_Output_Print len:%d, curOffset:%d, totalBuf:%d."
#define ADPCMRECORD_2249_112_2_18_1_19_50_1976 "[Record_ChangeBufToWrite] rec_ctrl_writing->available_data: %d"
#define ADPCMRECORD_2253_112_2_18_1_19_50_1977 "[Record_ChangeBufToWrite] state: %d, %d, %d"
#define ADPCMRECORD_2278_112_2_18_1_19_50_1978 "AdpcmRecord.c  Can't get file position"
#define ADPCMRECORD_2281_112_2_18_1_19_50_1979 "record pcm current_file_pos 1=%d"
#define ADPCMRECORD_2285_112_2_18_1_19_50_1980 "AdpcmRecord.c  Can't Seek to positon: %d"
#define ADPCMRECORD_2302_112_2_18_1_19_50_1981 "AdpcmRecord.c  Can't Seek to positon: %d"
#define ADPCMRECORD_2305_112_2_18_1_19_50_1982 "record pcm current_file_pos 2=%d"
#define ADPCMRECORD_2309_112_2_18_1_19_50_1983 "AdpcmRecord.c  Can't Flush file system"
#define ADPCMRECORD_2318_112_2_18_1_19_50_1984 "[Record_ChangeBufToWrite] write_err_code = %d"
#define ADPCMRECORD_2378_112_2_18_1_19_50_1985 "[AudRecord] %d, s_is_recording: %d, s_ref_record_stop: %d"
#define ADPCMRECORD_2389_112_2_18_1_19_50_1986 "[AudRecord] SCI_NULL != rec_ctrl_writing"
#define ADPCMRECORD_2482_112_2_18_1_19_50_1987 "[AudRecord] s_cur_file_size =%d, s_end_size=%d"
#define ADPCMRECORD_2561_112_2_18_1_19_50_1988 "[Record_WriteFSCallback]last buffer written, send to write header, %d, %d, %d, %d."
#define ADPCMRECORD_2581_112_2_18_1_19_51_1989 "[Record_WriteFSCallback]write error, send event 1"
#define ADPCMRECORD_2608_112_2_18_1_19_51_1990 "[Record_WriteFSCallback] last buffer written, send to write header, %d, %d, %d, %d"
#define ADPCMRECORD_2624_112_2_18_1_19_51_1991 "[Record_WriteFSCallback]write error, send event 2"
#define ADPCMRECORD_2650_112_2_18_1_19_51_1992 "Record_WriteFSCallback set s_mmi_record_stop true, space_len:%d."
#define ADPCMRECORD_2669_112_2_18_1_19_51_1993 "[Record_SendSigWriteHeader] Send signal to write header"
#define ADPCMRECORD_2674_112_2_18_1_19_51_1994 "Record_SendSigWriteHeader other task has been playing"
#define ADPCMRECORD_2766_112_2_18_1_19_51_1995 "create new file handle failed."
#define ADPCMRECORD_2793_112_2_18_1_19_51_1996 "[Record_WriteHeader] Entry point!"
#define ADPCMRECORD_2802_112_2_18_1_19_51_1997 "[Record_WriteHeader]set zero 0x %x, %x, %x."
#define ADPCMRECORD_2831_112_2_18_1_19_51_1998 "adpcmrecord.c:Record_WriteHeader:AUD_REC_PCM success."
#define ADPCMRECORD_2846_112_2_18_1_19_51_1999 "adpcmrecord.c:Record_WriteHeader:error:AUD_REC_AMR == s_record_param_t.format"
#define ADPCMRECORD_2857_112_2_18_1_19_51_2000 "AdpcmRecord.c  Can't Seek to Begin.result: %d"
#define ADPCMRECORD_2888_112_2_18_1_19_51_2001 "AdpcmRecord.c 2 write_err_code = %d"
#define ADPCMRECORD_2942_112_2_18_1_19_51_2002 "[Record_WriteHeaderCallback] write header finish"
#define ADPCMRECORD_2992_112_2_18_1_19_52_2003 "[Record_HandleWriteHeaderFinish] set s_is_recording=0"
#define ADPCMRECORD_2997_112_2_18_1_19_52_2004 "Record_HandleWriteHeaderFinish Record_audio_init."
#define ADPCMRECORD_3028_112_2_18_1_19_52_2005 "THREAD_ENTRY(RECORD): Received signal: 0x%04x, ori signal:0x%04x."
#define ADPCMRECORD_3080_112_2_18_1_19_52_2006 "[EnableVoiceRecord] layer1 record has start."
#define ADPCMRECORD_3091_112_2_18_1_19_52_2007 "[EnableVoiceRecord] format:0x%08x."
#define ADPCMRECORD_3141_112_2_18_1_19_52_2008 "[EnableVoiceRecord] layer1 record has stop."
#define ADPCMRECORD_ADP_68_112_2_18_1_19_52_2009 "adpcmrecord_adp.c:AdpcmRecord_Construct:error:SCI_NULL == ptAudioObj"
#define ADPCMRECORD_ADP_86_112_2_18_1_19_52_2010 "adpcmrecord_adp.c:AdpcmRecord_Construct:error:SCI_NULL == ptAudioObj"
#define ADPCMRECORD_ADP_118_112_2_18_1_19_52_2011 "adpcmrecord_adp.c:AdpcmRecord_StopDeconstruct:error:SCI_NULL == ptAudioObj"
#define ADPCMRECORD_ADP_130_112_2_18_1_19_52_2012 "AdpcmRecord_StopDeconstruct, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d."
#define ADPCMRECORD_ADP_159_112_2_18_1_19_53_2013 "adpcmrecord_adp.c:AdpcmRecord_StopDeconstruct:error:SCI_NULL == ptAudioObj"
#define ADPCMRECORD_ADP_170_112_2_18_1_19_53_2014 "AdpcmRecord_PauseDeconstruct, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d."
#define ADPCMRECORD_ADP_207_112_2_18_1_19_53_2015 "adpcmrecord_adp.c:AdpcmRecord_StopDeconstruct:error:SCI_NULL == ptAudioObj"
#define ADPCMRECORD_ADP_240_112_2_18_1_19_53_2016 "adpcmrecord_adp.c:AdpcmRecord_StartRecord:error:SCI_NULL == ptAudioObj"
#define ADPCMRECORD_ADP_255_112_2_18_1_19_53_2017 "[AdpcmRecord_StartRecord] error."
#define ADPCMRECORD_ADP_271_112_2_18_1_19_53_2018 "adpcmrecord_adp.c:AdpcmRecord_StopRecord:error:SCI_NULL == ptAudioObj"
#define ADPCMRECORD_ADP_278_112_2_18_1_19_53_2019 "[AdpcmRecord_StopRecord] entry point!audio_handle:0x%08x.cur_state:%d"
#define ADPCMRECORD_ADP_284_112_2_18_1_19_53_2020 "[AdpcmRecord_StopRecord]error!audio_handle:0x%08x.cur_state:%d"
#define ADPCMRECORD_ADP_300_112_2_18_1_19_53_2021 "adpcmrecord_adp.c:AdpcmRecord_PauseRecord:error:SCI_NULL == ptAudioObj"
#define ADPCMRECORD_ADP_326_112_2_18_1_19_53_2022 "adpcmrecord_adp.c:AdpcmRecord_ResumeRecord:error:SCI_NULL == ptAudioObj"
#define SBC_BY_DSP_114_112_2_18_1_21_16_2023 "sbc_by_dsp.c:PcmBuf_Init:SCI_CreateEvent failed!"
#define SBC_BY_DSP_564_112_2_18_1_21_17_2024 "sbc_by_dsp.c:SBC_ENCODE_Process:sci_sleep(20)"
#define SBC_BY_DSP_632_112_2_18_1_21_17_2025 "sbc_by_dsp.c:PCM_BUFFER_Alloc:buffer_l is %x,buffer_r %x"
#define SBC_BY_DSP_654_112_2_18_1_21_17_2026 "sbc_by_dsp.c:MemRes_Free : 1"
#define SBC_BY_DSP_660_112_2_18_1_21_17_2027 "sbc_by_dsp.c:MemRes_Free : 2"
#define SBC_BY_DSP_666_112_2_18_1_21_17_2028 "sbc_by_dsp.c:MemRes_Free : 3"
#define SBC_BY_DSP_671_112_2_18_1_21_17_2029 "sbc_by_dsp.c:MemRes_Free : 4"
#define SBC_BY_DSP_677_112_2_18_1_21_17_2030 "sbc_by_dsp.c:MemRes_Free : 5"
#define SBC_BY_DSP_682_112_2_18_1_21_17_2031 "sbc_by_dsp.c:MemRes_Free : 6"
#define SBC_BY_DSP_686_112_2_18_1_21_17_2032 "sbc_by_dsp.c:MemRes_Free : deinit buffer"
#define SBC_BY_DSP_804_112_2_18_1_21_18_2033 "sbc_by_dsp.c:DSP_Proc_Open"
#define SBC_BY_DSP_808_112_2_18_1_21_18_2034 "sbc_by_dsp.c:DSP_Proc_Open:sbc_res is null"
#define SBC_BY_DSP_817_112_2_18_1_21_18_2035 "sbc_by_dsp.c:DSP_Proc_Open:MemRes_Alloc fail"
#define SBC_BY_DSP_824_112_2_18_1_21_18_2036 "sbc_by_dsp.c:, Open(), failed to create event"
#define SBC_BY_DSP_833_112_2_18_1_21_18_2037 "sbc_by_dsp.c:DSP_Proc_Open:uiPriority is %d"
#define SBC_BY_DSP_865_112_2_18_1_21_18_2038 "sbc_by_dsp.c:DSP_Proc_Close"
#define SBC_BY_DSP_878_112_2_18_1_21_18_2039 "sbc_by_dsp.c:DSP_Proc_Close:MemRes_Free in"
#define UPLINK_CODEC_ADP_115_112_2_18_1_21_18_2040 "[BT]Voice:0x%X,0x%X,0x%X,0x%X\r\n"
#define UPLINK_CODEC_ADP_142_112_2_18_1_21_19_2041 "[BT]uplink_write_data:data_count=0x%X\r\n"
#define UPLINK_CODEC_ADP_188_112_2_18_1_21_19_2042 "s_uplink_buf->data_count=%d"
#define UPLINK_CODEC_ADP_229_112_2_18_1_21_19_2043 "time:%d."
#define UPLINK_CODEC_ADP_267_112_2_18_1_21_19_2044 "[uplink_play_construct] this codec is playing."
#define UPLINK_CODEC_ADP_371_112_2_18_1_21_19_2045 "UPLINK_Construct alloc failed, sizeof:%d bytes."
#define UPLINK_CODEC_ADP_425_112_2_18_1_21_19_2046 "[UPLINK_Play] IN"
#define UPLINK_CODEC_ADP_452_112_2_18_1_21_19_2047 "[UPLINK_Stop] IN"
#define UPLINK_CODEC_ADP_460_112_2_18_1_21_19_2048 "[UPLINK_Stop] this codec is not playing."
#define UPLINK_CODEC_ADP_477_112_2_18_1_21_19_2049 "[UPLINK_Stop] this audio has been stopped."
#define UPLINK_CODEC_ADP_487_112_2_18_1_21_19_2050 "[UPLINK_Pause] IN"
#define UPLINK_CODEC_ADP_496_112_2_18_1_21_19_2051 "[UPLINK_Pause] this codec is not playing."
#define UPLINK_CODEC_ADP_513_112_2_18_1_21_19_2052 "[UPLINK_Pause] this audio is not playing."
#define UPLINK_CODEC_ADP_523_112_2_18_1_21_19_2053 "[UPLINK_Resume] IN"
#define UPLINK_CODEC_ADP_541_112_2_18_1_21_19_2054 "[UPLINK_Resume] this audio is not in pausing."
#define UPLINK_CODEC_ADP_553_112_2_18_1_21_19_2055 "UPLINK_IdentifyFormat:error:SCI_NULL == ptAudioObject"
#define AUD_LVVE_CTRL_268_112_2_18_1_21_20_2056 "[Aud_LVVE_Construct] Failed! s_aud_lvve_thread_id=0x%x"
#define AUD_LVVE_CTRL_272_112_2_18_1_21_20_2057 "Aud_LVVE_Construct Success! s_aud_lvve_thread_id=0x%x"
#define AUD_LVVE_CTRL_298_112_2_18_1_21_20_2058 "[Aud_LVVE_Start] NULL == sig_ptr"
#define AUD_LVVE_CTRL_306_112_2_18_1_21_20_2059 "Aud_LVVE_Start"
#define AUD_LVVE_CTRL_322_112_2_18_1_21_20_2060 "[Aud_LVVE_ConfigParam] Input params error: config_param_ptr == NULL."
#define AUD_LVVE_CTRL_333_112_2_18_1_21_21_2061 "[Aud_LVVE_ConfigParam] Aud_LVVE_SaveParam error."
#define AUD_LVVE_CTRL_339_112_2_18_1_21_21_2062 "s_b_LVVE_ON = FALSE, LVVE not running."
#define AUD_LVVE_CTRL_346_112_2_18_1_21_21_2063 "[Aud_LVVE_ConfigParam] NULL == sig_ptr"
#define AUD_LVVE_CTRL_399_112_2_18_1_21_21_2064 "[Aud_LVVE_SendData] NULL == sig_ptr"
#define AUD_LVVE_CTRL_417_112_2_18_1_21_21_2065 "[Aud_LVVE_SendData] NULL == sig_ptr"
#define AUD_LVVE_CTRL_440_112_2_18_1_21_21_2066 "[Aud_LVVE_Uninit] NULL == sig_ptr"
#define AUD_LVVE_CTRL_494_112_2_18_1_21_21_2067 "[LVVE_Init] Version is %s"
#define AUD_LVVE_CTRL_510_112_2_18_1_21_21_2068 "Parameter error - null pointer error returned by LVVE_Tx_GetMemoryTable"
#define AUD_LVVE_CTRL_515_112_2_18_1_21_21_2069 "Parameter error - out of range error returned by LVVE_Tx_GetMemoryTable"
#define AUD_LVVE_CTRL_520_112_2_18_1_21_21_2070 "LVVE_Tx_GetMemoryTable Failed, LVVE_Status=%d"
#define AUD_LVVE_CTRL_532_112_2_18_1_21_21_2071 "Failed to allocate %d bytes for region %d\n"
#define AUD_LVVE_CTRL_538_112_2_18_1_21_21_2072 "s_MemoryTable_Tx.Region[i].Size = 0!"
#define AUD_LVVE_CTRL_548_112_2_18_1_21_21_2073 "Parameter error - null pointer error returned by LVVE_Rx_GetMemoryTable\n"
#define AUD_LVVE_CTRL_553_112_2_18_1_21_21_2074 "Parameter error - out of range error returned by LVVE_Rx_GetMemoryTable\n"
#define AUD_LVVE_CTRL_558_112_2_18_1_21_21_2075 "LVVE_Rx_GetMemoryTable Failed, LVVE_Status=%d"
#define AUD_LVVE_CTRL_570_112_2_18_1_21_21_2076 "Failed to allocate %d bytes for region %d\n"
#define AUD_LVVE_CTRL_583_112_2_18_1_21_21_2077 "Parameter error - null pointer error returned by LVVE_Tx_GetInstanceHandle\n"
#define AUD_LVVE_CTRL_588_112_2_18_1_21_21_2078 "Parameter error - out of range error returned by LVVE_Tx_GetInstanceHandle\n"
#define AUD_LVVE_CTRL_593_112_2_18_1_21_21_2079 "LVVE_Tx_GetInstanceHandle Failed, LVVE_Status=%d"
#define AUD_LVVE_CTRL_603_112_2_18_1_21_21_2080 "Parameter error - null pointer error returned by LVVE_Rx_GetInstanceHandle\n"
#define AUD_LVVE_CTRL_608_112_2_18_1_21_21_2081 "Parameter error - out of range error returned by LVVE_Rx_GetInstanceHandle\n"
#define AUD_LVVE_CTRL_613_112_2_18_1_21_21_2082 "LVVE_Rx_GetInstanceHandle Failed, LVVE_Status=%d"
#define AUD_LVVE_CTRL_623_112_2_18_1_21_21_2083 "[LVVE_SetControlParam] Failed!"
#define AUD_LVVE_CTRL_627_112_2_18_1_21_21_2084 "[LVVE_Init] Init LVVE Success."
#define AUD_LVVE_CTRL_646_112_2_18_1_21_21_2085 "[LVVE_SetControlParam] Rx operMode:%d, Mute=%d, AVL_MaxGainLin=%d, AVL_Target_level_lin=%d, FENS_limit_NS=%d"
#define AUD_LVVE_CTRL_653_112_2_18_1_21_21_2086 "Parameter error - null pointer error returned by LVVE_Rx_SetControlParameters\n"
#define AUD_LVVE_CTRL_659_112_2_18_1_21_21_2087 "Parameter error - out of range error returned by LVVE_Rx_SetControlParameters\n"
#define AUD_LVVE_CTRL_665_112_2_18_1_21_21_2088 "LVVE_Rx_SetControlParameters Failed, LVVE_Status=%d"
#define AUD_LVVE_CTRL_674_112_2_18_1_21_21_2089 "[LVVE_SetControlParam] Tx mode:%d, mute:%d, HF_DENS_tail_alpha=%d, HF_DENS_gamma_e_high=%d, WM_AVL_Limit_MaxOutputLin=%d"
#define AUD_LVVE_CTRL_681_112_2_18_1_21_21_2090 "Parameter error - null pointer error returned by LVVE_Tx_SetControlParameters\n"
#define AUD_LVVE_CTRL_687_112_2_18_1_21_21_2091 "Parameter error - out of range error returned by LVVE_Tx_SetControlParameters\n"
#define AUD_LVVE_CTRL_693_112_2_18_1_21_21_2092 "Parameter error - invalid algorithm configuration returned by LVVE_Tx_SetControlParameters\n"
#define AUD_LVVE_CTRL_699_112_2_18_1_21_21_2093 "LVVE_Tx_SetControlParameters Failed, LVVE_Status=%d"
#define AUD_LVVE_CTRL_703_112_2_18_1_21_21_2094 "[LVVE_SetControlParam] Success!"
#define AUD_LVVE_CTRL_732_112_2_18_1_21_21_2095 "Parameter error - null pointer returned from LVVE_Tx_Process\n"
#define AUD_LVVE_CTRL_738_112_2_18_1_21_21_2096 "Invalid number of samples, check value or range returned from LVVE_Tx_Process\n"
#define AUD_LVVE_CTRL_744_112_2_18_1_21_21_2097 "Alignment error, check input and/or output buffer alignment returned from LVVE_Tx_Process\n"
#define AUD_LVVE_CTRL_750_112_2_18_1_21_21_2098 "Error while processing returned from LVVE_Tx_Process: %d\n"
#define AUD_LVVE_CTRL_786_112_2_18_1_21_21_2099 "Aud_Ctrl_Process_Rx rx time:%d"
#define AUD_LVVE_CTRL_791_112_2_18_1_21_21_2100 "Parameter error - null pointer returned from LVVE_Rx_Process\n"
#define AUD_LVVE_CTRL_797_112_2_18_1_21_21_2101 "Invalid number of samples, check value or range returned from LVVE_Rx_Process\n"
#define AUD_LVVE_CTRL_803_112_2_18_1_21_21_2102 "Alignment error, check input and/or output buffer alignment returned from LVVE_Rx_Process\n"
#define AUD_LVVE_CTRL_809_112_2_18_1_21_21_2103 "Error while processing returned from LVVE_Rx_Process: %d\n"
#define AUD_LVVE_CTRL_837_112_2_18_1_21_22_2104 "Parameter error - null pointer error returned by LVVE_Tx_GetMemoryTable\n"
#define AUD_LVVE_CTRL_843_112_2_18_1_21_22_2105 "Parameter error - out of range error returned by LVVE_Tx_GetMemoryTable\n"
#define AUD_LVVE_CTRL_863_112_2_18_1_21_22_2106 "Parameter error - null pointer error returned by LVVE_Rx_GetMemoryTable\n"
#define AUD_LVVE_CTRL_868_112_2_18_1_21_22_2107 "Parameter error - out of range error returned by LVVE_Rx_GetMemoryTable\n"
#define AUD_LVVE_CTRL_882_112_2_18_1_21_22_2108 "[LVVE_Unit] Free LVVE Success."
#define AUD_LVVE_CTRL_901_112_2_18_1_21_22_2109 "[Aud_LVVE_SaveParam] rx operatingMode:%d, mute:%d."
#define AUD_LVVE_CTRL_908_112_2_18_1_21_22_2110 "[Aud_LVVE_SaveParam] tx operatingMode:%d, mute:%d."
#define AUD_LVVE_CTRL_941_112_2_18_1_21_22_2111 "Parameter error - us_signal_code=%d"
#define AUD_LVVE_CTRL_983_112_2_18_1_21_22_2112 "Parameter error - config_param_ptr == NULL"
#define AUD_LVVE_CTRL_996_112_2_18_1_21_22_2113 "CMD_LVVE_PROCESS_Tx:%d, %d, %d."
#define AUD_LVVE_CTRL_1018_112_2_18_1_21_22_2114 "Parameter error - SignalCode=%d"
#define VOICE_ADP_148_112_2_18_1_21_22_2115 "[VOICE_Play] Success."
#define VOICE_ADP_151_112_2_18_1_21_22_2116 "[VOICE_Play] Failed."
#define VOICE_ADP_173_112_2_18_1_21_22_2117 "[VOICE_Stop] Success."
#define VOICE_ADP_178_112_2_18_1_21_22_2118 "[VOICE_Stop] Failed."
#define VOICEPROCESS_ADP_251_112_2_18_1_21_23_2119 "voiceprocess_adp.c _Voice_Downlink_Init_local init in"
#define VOICEPROCESS_ADP_258_112_2_18_1_21_23_2120 "voiceprocess_adp.c _Voice_Downlink_Init_local init"
#define VOICEPROCESS_ADP_261_112_2_18_1_21_23_2121 "voiceprocess_adp.c _Voice_Downlink_Init_local is 0x%x"
#define VOICEPROCESS_ADP_277_112_2_18_1_21_23_2122 "voiceprocess_adp.c _Voice_Uplink_Init_local init"
#define VOICEPROCESS_ADP_310_112_2_18_1_21_23_2123 "voiceprocess_adp.c _Voice_Write_Data_To_File modem index:%d, lose one frame, total:%d frames until now, time:%d(ms)."
#define VOICEPROCESS_ADP_345_112_2_18_1_21_23_2124 "voiceprocess_adp.c _Voice_Write_Data_To_File index:%d, lose one frame, total:%d frames until now, time:%d(ms)."
#define VOICEPROCESS_ADP_361_112_2_18_1_21_23_2125 "voiceprocess_adp.c _Voice_Write_Data_To_File index:%d, start to save to file, time:%d(ms)."
#define VOICEPROCESS_ADP_458_112_2_18_1_21_24_2126 "voiceprocess_adp.c voice down:%d, %d, %d."
#define VOICEPROCESS_ADP_472_112_2_18_1_21_24_2127 "voiceprocess_adp.c _Voice_Process_Downlink_Thread_Entry loopback curDelayCount/totalDelayCount, down:%d, %d, %d, %d, up:%d, %d, %d, %d."
#define VOICEPROCESS_ADP_545_112_2_18_1_21_24_2128 "voiceprocess_adp.c voice up:%d, %d, %d."
#define VOICEPROCESS_ADP_805_112_2_18_1_21_24_2129 "voiceprocess_adp.c _Voice_Process_RecordDebugData_Thread_Entry config para error: callback:0x%x."
#define VOICEPROCESS_ADP_812_112_2_18_1_21_24_2130 "voiceprocess_adp.c _Voice_Write_Data_To_File index:%d, len:%d(bytes), cacheIndex:%d, send to ap over, time:%d(ms)."
#define VOICEPROCESS_ADP_839_112_2_18_1_21_24_2131 "voiceprocess_adp.c _Voice_Process_RecordDebugData_Thread_Entry fail to write file, dataSize:%d, index:%d,"
#define VOICEPROCESS_ADP_843_112_2_18_1_21_24_2132 "voiceprocess_adp.c _Voice_Write_Data_To_File index:%d, len:%d(bytes), save to file over, result:%d, time:%d(ms)."
#define VOICEPROCESS_ADP_868_112_2_18_1_21_24_2133 "voiceprocess_adp.c _Voice_Downlink_Pro_Reg in, config:0x%x, exe:0x%x"
#define VOICEPROCESS_ADP_887_112_2_18_1_21_24_2134 "voiceprocess_adp.c _Voice_Uplink_Pro_Reg in, config:0x%x, exe:0x%x"
#define VOICEPROCESS_ADP_904_112_2_18_1_21_24_2135 "voiceprocess_adp.c _Voice_ExtOpe_Get_NVConfigInfo curMode:%d."
#define VOICEPROCESS_ADP_918_112_2_18_1_21_24_2136 "voiceprocess_adp.c _Voice_ExtOpe_Get_NVConfigInfo alloc failed in loopback, curMode:%d, size:%d."
#define VOICEPROCESS_ADP_955_112_2_18_1_21_25_2137 "voiceprocess_adp.c NVConfigInfo, loopback pro_enable:%d, voiceBoostEnable:%d, nxpEnable:%d, aec:0x%x, extend2[64]:0x%x, iftd:%d."
#define VOICEPROCESS_ADP_978_112_2_18_1_21_25_2138 "voiceprocess_adp.c _Voice_ExtOpe_Get_NVConfigInfo alloc failed 1, curMode:%d, size:%d."
#define VOICEPROCESS_ADP_992_112_2_18_1_21_25_2139 "voiceprocess_adp.c _Voice_ExtOpe_Get_NVConfigInfo alloc failed 2, curMode:%d, size:%d."
#define VOICEPROCESS_ADP_1088_112_2_18_1_21_25_2140 "voiceprocess_adp.c NVConfigInfo, pro_enable:%d, voiceBoostEnable:%d, nxpEnable:%d, aec:0x%x(gsm), 0x%x(td), extend2[64]:0x%x(gsm), 0x%x(td), iftd:%d."
#define VOICEPROCESS_ADP_1101_112_2_18_1_21_25_2141 "voiceprocess_adp.c _Voice_ExtOpe_Get_NVConfigInfo alloc failed in loopback, curMode:%d, size:%d."
#define VOICEPROCESS_ADP_1163_112_2_18_1_21_25_2142 "voiceprocess_adp.c NVConfigInfo, pro_enable:%d, voiceBoostEnable:%d, nxpEnable:%d, aec:0x%x, extend2[64]:0x%x."
#define VOICEPROCESS_ADP_1180_112_2_18_1_21_25_2143 "voiceprocess_adp.c _Voice_IfLoopback sbIfLoopback input para error."
#define VOICEPROCESS_ADP_1186_112_2_18_1_21_25_2144 "voiceprocess_adp.c _Voice_IfLoopback sbIfLoopback:%d."
#define VOICEPROCESS_ADP_1202_112_2_18_1_21_25_2145 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData input para error."
#define VOICEPROCESS_ADP_1211_112_2_18_1_21_25_2146 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData input para error, uiRecordPos(%d, min:0, max:%d) is invalid."
#define VOICEPROCESS_ADP_1221_112_2_18_1_21_25_2147 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData the data of this pos has been recording"
#define VOICEPROCESS_ADP_1233_112_2_18_1_21_25_2148 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData config para error: callback:0x%x, cachFrameCount:%d, samplesOnce:%d."
#define VOICEPROCESS_ADP_1240_112_2_18_1_21_25_2149 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData alloc ptCachData failed, size: %d."
#define VOICEPROCESS_ADP_1245_112_2_18_1_21_25_2150 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData alloc ptCachData success, size: %d."
#define VOICEPROCESS_ADP_1252_112_2_18_1_21_25_2151 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData alloc psTotalBuffer failed, size: %d."
#define VOICEPROCESS_ADP_1258_112_2_18_1_21_25_2152 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData alloc psTotalBuffer success, size: %d."
#define VOICEPROCESS_ADP_1268_112_2_18_1_21_25_2153 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData modem start, index:%d."
#define VOICEPROCESS_ADP_1277_112_2_18_1_21_25_2154 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData the data of this pos:%d has not been recording"
#define VOICEPROCESS_ADP_1290_112_2_18_1_21_25_2155 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData modem stop, index:%d,lostData:%d(frames), sendData:%d(bytes)."
#define VOICEPROCESS_ADP_1299_112_2_18_1_21_25_2156 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData input para error, name is null."
#define VOICEPROCESS_ADP_1305_112_2_18_1_21_25_2157 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData the data of this pos has been recording"
#define VOICEPROCESS_ADP_1315_112_2_18_1_21_25_2158 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData hFileHanlde create failed. name:%s."
#define VOICEPROCESS_ADP_1322_112_2_18_1_21_25_2159 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData alloc failed, size: %d."
#define VOICEPROCESS_ADP_1332_112_2_18_1_21_25_2160 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData alloc ping-pang buf:0x%x, 0x%x, total size: %d."
#define VOICEPROCESS_ADP_1339_112_2_18_1_21_25_2161 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData start, index:%d."
#define VOICEPROCESS_ADP_1351_112_2_18_1_21_25_2162 "voiceprocess_adp.c: start to record; CHNG_FREQ_REQ_HANDLER_NULL == s_voice_record_chng_freq_req_handler"
#define VOICEPROCESS_ADP_1365_112_2_18_1_21_25_2163 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData the data of this pos:%d has not been recording"
#define VOICEPROCESS_ADP_1385_112_2_18_1_21_25_2164 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData sleep to wait last write file over, time:%d(ms), force to break."
#define VOICEPROCESS_ADP_1389_112_2_18_1_21_25_2165 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData sleep to wait last write file over, time:%d(ms)."
#define VOICEPROCESS_ADP_1419_112_2_18_1_21_25_2166 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData fail to write file 1, dataSize:%d, index:%d,"
#define VOICEPROCESS_ADP_1434_112_2_18_1_21_25_2167 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData fail to write file 2, dataSize:%d, index:%d,"
#define VOICEPROCESS_ADP_1443_112_2_18_1_21_25_2168 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData fail to write file, dataSize:%d, index:%d,"
#define VOICEPROCESS_ADP_1456_112_2_18_1_21_26_2169 "voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData stop, index:%d, lost:%d(bytes), save:%d(frames)."
#define VOICEPROCESS_ADP_1485_112_2_18_1_21_26_2170 "voiceprocess_adp.c _Voice_ExtOpe_ConfigRecordDebugData input para error."
#define VOICEPROCESS_ADP_1496_112_2_18_1_21_26_2171 "voiceprocess_adp.c _Voice_ExtOpe_ConfigRecordDebugData input para error: 0x%x, %d, %d."
#define VOICEPROCESS_ADP_1502_112_2_18_1_21_26_2172 "voiceprocess_adp.c _Voice_ExtOpe_ConfigRecordDebugData this codec is playing, can't reConfig parameters."
#define VOICEPROCESS_ADP_1511_112_2_18_1_21_26_2173 "voiceprocess_adp.c _Voice_ExtOpe_ConfigRecordDebugData exe successfully, samplesOnce:%d, cachFrameCount:%d, callback:0x%x."
#define VOICEPROCESS_ADP_1531_112_2_18_1_21_26_2174 "voiceprocess_adp.c  _Voice_ConfigFromNv_DelayTimeInLoopback to get delay time."
#define VOICEPROCESS_ADP_1536_112_2_18_1_21_26_2175 "voiceprocess_adp.c   _Voice_ConfigFromNv_DelayTimeInLoopback, alloc fail, size: %d"
#define VOICEPROCESS_ADP_1543_112_2_18_1_21_26_2176 "voiceprocess_adp.c   _Voice_ConfigFromNv_DelayTimeInLoopback mode:%d."
#define VOICEPROCESS_ADP_1551_112_2_18_1_21_26_2177 "voiceprocess_adp.c  _Voice_ConfigFromNv_DelayTimeInLoopback failed to get mode param."
#define VOICEPROCESS_ADP_1577_112_2_18_1_21_26_2178 "voiceprocess_adp.c  _Voice_ConfigFromNv_DelayTimeInLoopback failed to alloc info, size:%d."
#define VOICEPROCESS_ADP_1594_112_2_18_1_21_26_2179 "voiceprocess_adp.c  _Voice_ConfigFromNv_DelayTimeInLoopback failed to alloc data buffer, size:%d."
#define VOICEPROCESS_ADP_1608_112_2_18_1_21_26_2180 "voiceprocess_adp.c  _Voice_ConfigFromNv_DelayTimeInLoopback failed to alloc data buffer, size:%d."
#define VOICEPROCESS_ADP_1627_112_2_18_1_21_26_2181 "voiceprocess_adp.c  _Voice_ConfigFromNv_DelayTimeInLoopback enable arm delay time %d,  nv:%d, unit:ms, delayFrameCount:%d."
#define VOICEPROCESS_ADP_1643_112_2_18_1_21_26_2182 "voiceprocess_adp.c  _Voice_ConfigFromNv_DelayTimeInLoopback to disable to delay."
#define VOICEPROCESS_ADP_1665_112_2_18_1_21_26_2183 "_Voice_ExtOpe_Get_NVConfigInfo, alloc fail, size: %d"
#define VOICEPROCESS_ADP_1672_112_2_18_1_21_26_2184 "_Voice_ExtOpe_Get_NVConfigInfo mode:%d."
#define VOICEPROCESS_ADP_1680_112_2_18_1_21_26_2185 "_Voice_ExtOpe_Get_NVConfigInfo failed to get mode param."
#define VOICEPROCESS_ADP_1757_112_2_18_1_21_26_2186 "voiceprocess_adp.c [_VOICE_RegetInfoOnceAudioModeChanged], NO enough memory failed."
#define VOICEPROCESS_ADP_1787_112_2_18_1_21_26_2187 "voiceprocess_adp.c [_VOICE_RegetInfoOnceVolumeChanged], not support mode "
#define VOICEPROCESS_ADP_1792_112_2_18_1_21_26_2188 "voiceprocess_adp.c [_VOICE_RegetInfoOnceAudioModeChanged], LVVE_CTRL_PARAM_GetDevModeName error mode = %d, name:%s"
#define VOICEPROCESS_ADP_1799_112_2_18_1_21_26_2189 "voiceprocess_adp.c [_VOICE_RegetInfoOnceAudioModeChanged], LVVE_CTRL_PARAM_GetDevModeParam error mode = %d, name:%s"
#define VOICEPROCESS_ADP_1805_112_2_18_1_21_26_2190 "voiceprocess_adp.c [_VOICE_RegetInfoOnceAudioModeChanged], current device mode = %d, name:%s, %d"
#define VOICEPROCESS_ADP_1809_112_2_18_1_21_26_2191 "voiceprocess_adp.c [_VOICE_RegetInfoOnceAudioModeChanged], Aud_LVVE_ConfigParam error."
#define VOICEPROCESS_ADP_1828_112_2_18_1_21_26_2192 "voiceprocess_adp.c [_VOICE_RegetInfoOnceAudioModeChanged], curDown:(*%d/4096), ifcurDown:%d"
#define VOICEPROCESS_ADP_1850_112_2_18_1_21_27_2193 "voiceprocess_adp.c [_VOICE_RegetInfoOnceVolumeChanged], NO enough memory failed."
#define VOICEPROCESS_ADP_1880_112_2_18_1_21_27_2194 "voiceprocess_adp.c [_VOICE_RegetInfoOnceVolumeChanged], not support mode "
#define VOICEPROCESS_ADP_1885_112_2_18_1_21_27_2195 "voiceprocess_adp.c [_VOICE_RegetInfoOnceVolumeChanged], LVVE_CTRL_PARAM_GetDevModeName error."
#define VOICEPROCESS_ADP_1893_112_2_18_1_21_27_2196 "voiceprocess_adp.c [_VOICE_RegetInfoOnceVolumeChanged], LVVE_CTRL_PARAM_GetDevModeName error mode = %d, name:%s."
#define VOICEPROCESS_ADP_1903_112_2_18_1_21_27_2197 "voiceprocess_adp.c [_VOICE_RegetInfoOnceVolumeChanged], current device mode = %d, vol:%d, name:%s."
#define VOICEPROCESS_ADP_1907_112_2_18_1_21_27_2198 "voiceprocess_adp.c [_VOICE_RegetInfoOnceVolumeChanged], Aud_LVVE_ConfigParam error."
#define VOICEPROCESS_ADP_2039_112_2_18_1_21_27_2199 "VoiceProcess_Ext_Play finished..."
#define VOICEPROCESS_ADP_2125_112_2_18_1_21_27_2200 "voiceprocess_adp.c:VOICE_IdentifyFormat:error:ptAudioObject == SCI_NULL"
#define VT_ADP_116_112_2_18_1_21_28_2201 "[VT_ADP_Stop] not playing."
#define VT_ADP_124_112_2_18_1_21_28_2202 "[VT_ADP_Stop] 0x%x, 0x%x."
#define VT_ADP_131_112_2_18_1_21_28_2203 "[VT_ADP_Stop] wrong state: recording but not playing."
#define VT_ADP_203_112_2_18_1_21_28_2204 "[VT_Ext_Start] be playing, handle:0x%x."
#define VT_ADP_237_112_2_18_1_21_28_2205 "[VT_Ext_Start] alloc failed, handle:0x%x, sizeof:%d."
#define VT_ADP_274_112_2_18_1_21_28_2206 "[VT_Ext_Stop] not be playing."
#define VT_ADP_279_112_2_18_1_21_28_2207 "[VT_Ext_Stop] please stop record first."
#define VT_ADP_293_112_2_18_1_21_28_2208 "[VT_Ext_Stop] 0x%x, 0x%x."
#define VT_ADP_342_112_2_18_1_21_28_2209 "[VT_Ext_WriteVoiceData] PNULL==ptPara->write_voicedata_t.us_source_buf_ptr"
#define VT_ADP_381_112_2_18_1_21_29_2210 "[VT_Ext_WriteVoiceData] PNULL==read_byte_data->uc_source_buf_ptr"
#define VT_ADP_400_112_2_18_1_21_29_2211 "[VT_Ext_WriteVoiceData] PNULL==read_voicedata_2byets->us_source_buf_ptr"
#define VT_ADP_429_112_2_18_1_21_29_2212 "[VT_Ext_StartRecord] not be playing."
#define VT_ADP_435_112_2_18_1_21_29_2213 "[VT_Ext_StartRecord] has been in recording."
#define VT_ADP_446_112_2_18_1_21_29_2214 "[VT_Ext_StartRecord] 0x%x, 0x%x."
#define VT_ADP_489_112_2_18_1_21_29_2215 "[VT_Ext_WriteVoiceData] PNULL==ptPara->read_recorddata_t.us_source_buf_ptr"
#define VT_ADP_518_112_2_18_1_21_29_2216 "[VT_Ext_StopRecord] not be playing."
#define VT_ADP_523_112_2_18_1_21_29_2217 "[VT_Ext_StopRecord] not be recording."
#define VT_ADP_534_112_2_18_1_21_29_2218 "[VT_Ext_StopRecord] 0x%x, 0x%x."
#define VT_ADP_579_112_2_18_1_21_29_2219 "[VT_Ext_AppSendDownlinkData] PNULL==ptPara->read_voicedata_t.us_source_buf_ptr"
#define VT_ADP_586_112_2_18_1_21_29_2220 "[VT_Ext_AppSendDownlinkData] can't put downlnk data, state:%d, %d."
#define VT_ADP_618_112_2_18_1_21_29_2221 "[VT_Ext_AppSendDownlinkData] there is no space currently."
#define WAV_FFS_78_112_2_18_1_21_42_2222 "wav_ffs.c,[WAV_ReadData] read file encounters error!"
#define WAV_FFS_134_112_2_18_1_21_42_2223 "wav_ffs.c,[WAV_SetDataPointer] seek file encounters error!"
#define WAV_FFS_170_112_2_18_1_21_42_2224 "wav_ffs.c,[WAV_SetDataPointer]  buffer seek error:dest_Offset > data_len!dest_Offset:%x,data_len:%x"
#define WAV_FFS_211_112_2_18_1_21_42_2225 "wav_ffs.c,[WAV_GetDataSize] get file size encounters error!"
#define WAV_FFS_252_112_2_18_1_21_42_2226 "wav_ffs.c,[WAV_GetDataPointer] get file data pointer encounters error!"
#define WMA_ADP_259_112_2_18_1_23_3_2227 "wma_adp.c:WMA_FreeInConstruct:error:PNULL==audio_obj"
#define WMA_ADP_309_112_2_18_1_23_3_2228 "wma_adp.c:WMA_FreeInConstruct:error:PNULL==audio_obj"
#define WMA_ADP_393_112_2_18_1_23_3_2229 "wma_adp.c:WMA_Construct:error:HEADER_TYPE_DATA == header_info_ptr->hea"
#define WMA_ADP_536_112_2_18_1_23_4_2230 "wma_adp.c:WMA_Deconstruct:error:PNULL==audio_obj"
#define WMA_ADP_543_112_2_18_1_23_4_2231 "wma_adp.c:WMA_PlayConstruct:error:PNULL==audio_obj->tCodecProcessResouce.other_info_ptr"
#define WMA_ADP_554_112_2_18_1_23_4_2232 "wma_adp.c:WMA_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_ui_chng_freq_req_handler"
#define WMA_ADP_586_112_2_18_1_23_4_2233 "wma_adp.c:WMA_Deconstruct:error:PNULL==audio_obj"
#define WMA_ADP_593_112_2_18_1_23_4_2234 "wma_adp.c:WMA_PlayConstruct:error:PNULL==audio_obj->tCodecProcessResouce.other_info_ptr"
#define WMA_ADP_624_112_2_18_1_23_4_2235 "wma_adp.c:WMA_ResumeConstruct:error:PNULL==audio_obj"
#define WMA_ADP_631_112_2_18_1_23_4_2236 "wma_adp.c:WMA_ResumeConstruct:error:PNULL==audio_obj->tCodecProcessResouce.other_info_ptr"
#define WMA_ADP_641_112_2_18_1_23_4_2237 "wma_adp.c:WMA_PlayConstruct:error:WMA_ResumeConstruct == s_ui_chng_freq_req_handler"
#define WMA_ADP_667_112_2_18_1_23_4_2238 "wma_adp.c:WMA_StopDeconstruct:error:PNULL==audio_obj"
#define WMA_ADP_674_112_2_18_1_23_4_2239 "wma_adp.c:WMA_StopDeconstruct:error:PNULL==audio_obj->tCodecProcessResouce.other_info_ptr"
#define WMA_ADP_714_112_2_18_1_23_4_2240 "wma_adp.c:WMA_Play:error:PNULL==audio_obj"
#define WMA_ADP_721_112_2_18_1_23_4_2241 "wma_adp.c:WMA_Play:error:PNULL==audio_obj->tCodecProcessResouce.other_info_ptr"
#define WMA_ADP_763_112_2_18_1_23_4_2242 "wma_adp.c:WMA_Stop:error:PNULL==audio_obj"
#define WMA_ADP_770_112_2_18_1_23_4_2243 "wma_adp.c:WMA_Stop:error:AUDIO_DEFAULT_TASK_ID==audio_obj->tThreadId"
#define WMA_ADP_813_112_2_18_1_23_4_2244 "wma_adp.c:WMA_Pause:error:PNULL==audio_obj"
#define WMA_ADP_820_112_2_18_1_23_4_2245 "wma_adp.c:WMA_Pause:error:AUDIO_DEFAULT_TASK_ID==audio_obj->tThreadId"
#define WMA_ADP_858_112_2_18_1_23_4_2246 "wma_adp.c:WMA_Resume:error:PNULL==audio_obj"
#define WMA_ADP_865_112_2_18_1_23_4_2247 "wma_adp.c:WMA_Resume:error:AUDIO_DEFAULT_TASK_ID==audio_obj->tThreadId"
#define WMA_ADP_942_112_2_18_1_23_4_2248 "wma_adp.c:WMA_Seek:error:PNULL==audio_obj"
#define WMA_ADP_949_112_2_18_1_23_4_2249 "wma_adp.c:WMA_Seek:error:AUDIO_DEFAULT_TASK_ID==audio_obj->tThreadId"
#define WMA_ADP_972_112_2_18_1_23_5_2250 "wma_adp.c:WMA_Seek:error:(ui_total_len==0)||(ui_total_time==0)"
#define WMA_ADP_1025_112_2_18_1_23_5_2251 "wma_adp.c:WMA_SendStreamData:error:PNULL==audio_obj"
#define WMA_ADP_1032_112_2_18_1_23_5_2252 "wma_adp.c:WMA_SendStreamData:error:AUDIO_DEFAULT_TASK_ID==audio_obj->tThreadId"
#define WMA_ADP_1074_112_2_18_1_23_5_2253 "wma_adp.c:WMA_IdentifyAudioFormat:error:PNULL==audio_obj"
#define WMA_ADP_1095_112_2_18_1_23_5_2254 "wma_adp.c:WMA_IdentifyAudioFormat:error:WMA_STREAMBUFFER_HEADER_TYPE_FORMAT != header_info_ptr->header_type"
#define WMA_ADP_1144_112_2_18_1_23_5_2255 "wma_adp.c:WMA_IdentifyAudioFormat:error:PNULL==audio_obj"
#define WMA_ADP_1153_112_2_18_1_23_5_2256 "wma_adp.c:WMA_GetPlayingInfo:error:ext_process_resouce_ptr->wma_demux_ptr == PNULL"
#define WMA_ADP_1200_112_2_18_1_23_5_2257 "wma_adp.c:WMA_GetOutputSamplerate:error:PNULL==audio_obj"
#define WMA_ADP_1328_112_2_18_1_23_5_2258 "wma_adp.c:WMA_GetDemuxSrcData:error:WMA_STREAMBUFFER_HEADER_TYPE_FORMAT != header_info_ptr->header_type"
#define WMA_ADP_1366_112_2_18_1_23_5_2259 "wma_adp.c:WMA_GetDemuxSrcData:error:default"
#define WMA_ADP_1726_112_2_18_1_23_6_2260 "wam_adp.c:thread_entry: default:"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_CODEC_TRC)
TRACE_MSG(AAC_ADP_267_112_2_18_1_8_9_0,"%s, %d. alloc failed! len:%d.")
TRACE_MSG(AAC_ADP_274_112_2_18_1_8_9_1,"%s, %d. alloc failed! len:%d.")
TRACE_MSG(AAC_ADP_281_112_2_18_1_8_9_2,"%s, %d. alloc failed! len:%d.")
TRACE_MSG(AAC_ADP_288_112_2_18_1_8_9_3,"%s, %d. alloc failed! len:%d.")
TRACE_MSG(AAC_ADP_301_112_2_18_1_8_9_4,"%s, %d. alloc failed! len:%d.")
TRACE_MSG(AAC_ADP_312_112_2_18_1_8_9_5,"%s, %d. alloc failed! len:%d.")
TRACE_MSG(AAC_ADP_322_112_2_18_1_8_9_6,"%s, %d. alloc failed! len:%d.")
TRACE_MSG(AAC_ADP_344_112_2_18_1_8_9_7,"AAC_Construct NOT support stream buffer play in aac codec.")
TRACE_MSG(AAC_ADP_607_112_2_18_1_8_10_8,"%s, %d. alloc failed! audio handle:0x%08x.")
TRACE_MSG(AAC_ADP_647_112_2_18_1_8_10_9,"%s, %d. alloc failed! audio handle:0x%08x.")
TRACE_MSG(AAC_ADP_688_112_2_18_1_8_10_10,"%s, %d. alloc failed! audio handle:0x%08x.")
TRACE_MSG(AAC_ADP_731_112_2_18_1_8_10_11,"%s, %d. alloc failed! audio handle:0x%08x.")
TRACE_MSG(AAC_ADP_779_112_2_18_1_8_10_12,"AAC_Seek aac codec can't support seek in streambuffer play.")
TRACE_MSG(AAC_ADP_787_112_2_18_1_8_10_13,"%s, %d. alloc failed! audio handle:0x%08x.")
TRACE_MSG(AAC_ADP_826_112_2_18_1_8_10_14,"AAC_GetPlayingInfo, audio_obj->tPlayingInfo.uiTotalDataLength=0.")
TRACE_MSG(AAC_ADP_895_112_2_18_1_8_10_15,"%s, %d, %d.")
TRACE_MSG(AAC_ADP_901_112_2_18_1_8_10_16,"%s, %d, %d.")
TRACE_MSG(AAC_ADP_927_112_2_18_1_8_10_17,"AAC_IdentifyAudioFormat, uiSamplerate;%d, total_time:%d.")
TRACE_MSG(AAC_ADP_940_112_2_18_1_8_10_18,"%s, %d can't identify audio format in streambuffer_playing.")
TRACE_MSG(AAC_ADP_1096_112_2_18_1_8_11_19,"%s, %d, get src data error:%d, frame_id:%d.")
TRACE_MSG(AAC_ADP_1115_112_2_18_1_8_11_20,"%s, %d, decode error:%d, frame_id: %d.")
TRACE_MSG(AAC_ADP_1164_112_2_18_1_8_11_21,"AAC_SEEK ui_offset(%d)>ui_total_len(%d), aac codec will set ui_offset=ui_total_len.")
TRACE_MSG(AAC_ADP_1174_112_2_18_1_8_11_22,"AAC_SEEK success! ori_offset:%d, dest_offset:%d.")
TRACE_MSG(AAC_ADP_1179_112_2_18_1_8_11_23,"AAC_SEEK failed! ori_offset:%d, dest_offset:%d.")
TRACE_MSG(AAC_ADP_1185_112_2_18_1_8_11_24,"%s, %d get src proc req, audio handle:0x%08x.")
TRACE_MSG(AAC_DEMUX_INTERFACE_307_112_2_18_1_8_35_25,"aac_demux_interface.c [DEMUX_AAC_Init], allocLen:%d.")
TRACE_MSG(AAC_DEMUX_INTERFACE_509_112_2_18_1_8_36_26,"ReadAdtsStreamFile byte_read != aac_adts_data_struc_ptr->bytes_consumed, %d, %d")
TRACE_MSG(AAC_DEMUX_INTERFACE_980_112_2_18_1_8_37_27,"aac_demux_interface.c [DEMUX_AAC_MemoryAlloc], allocLen:%d.")
TRACE_MSG(AAC_ADP_249_112_2_18_1_8_45_28,"[AAC_ConfigDec] NULL == codec_ext_res_ptr")
TRACE_MSG(AAC_ADP_291_112_2_18_1_8_45_29,"aac_adp.c:AAC_FreeInConstruct:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_415_112_2_18_1_8_46_30,"AAC_GetDemuxSrcData len of data is zero, %d, %d, %d.")
TRACE_MSG(AAC_ADP_423_112_2_18_1_8_46_31,"AAC_GetDemuxSrcData len of data: %d, %d, %d.")
TRACE_MSG(AAC_ADP_469_112_2_18_1_8_46_32,"aac_adp.c:AAC_Construct:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_481_112_2_18_1_8_46_33,"%s, %d. alloc failed! len:%d.")
TRACE_MSG(AAC_ADP_486_112_2_18_1_8_46_34,"aac_adp.c [AAC_Construct], allocLen:%d.")
TRACE_MSG(AAC_ADP_495_112_2_18_1_8_46_35,"%s, %d. alloc failed! len:%d.")
TRACE_MSG(AAC_ADP_500_112_2_18_1_8_46_36,"aac_adp.c [AAC_Construct], allocLen:%d.")
TRACE_MSG(AAC_ADP_507_112_2_18_1_8_46_37,"%s, %d. alloc failed! len:%d.")
TRACE_MSG(AAC_ADP_512_112_2_18_1_8_46_38,"aac_adp.c [AAC_Construct], allocLen:%d.")
TRACE_MSG(AAC_ADP_519_112_2_18_1_8_46_39,"%s, %d. alloc failed! len:%d.")
TRACE_MSG(AAC_ADP_524_112_2_18_1_8_46_40,"aac_adp.c [AAC_Construct], allocLen:%d.")
TRACE_MSG(AAC_ADP_531_112_2_18_1_8_46_41,"%s, %d. alloc failed! len:%d.")
TRACE_MSG(AAC_ADP_536_112_2_18_1_8_46_42,"aac_adp.c [AAC_Construct], allocLen:%d.")
TRACE_MSG(AAC_ADP_549_112_2_18_1_8_46_43,"%s, %d. alloc failed! len:%d.")
TRACE_MSG(AAC_ADP_554_112_2_18_1_8_46_44,"aac_adp.c [AAC_Construct], allocLen:%d.")
TRACE_MSG(AAC_ADP_565_112_2_18_1_8_46_45,"%s, %d. alloc failed! len:%d.")
TRACE_MSG(AAC_ADP_574_112_2_18_1_8_46_46,"%s, %d. alloc failed! len:%d.")
TRACE_MSG(AAC_ADP_598_112_2_18_1_8_46_47,"aac_adp.c:AAC_Construct:error:PNULL!=audio_obj->pGetSrcCallbackFunc")
TRACE_MSG(AAC_ADP_609_112_2_18_1_8_46_48,"aac_adp.c:AAC_Construct:error:error: Unknown Handle type!")
TRACE_MSG(AAC_ADP_655_112_2_18_1_8_46_49,"aac_adp.c:AAC_Deconstruct:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_679_112_2_18_1_8_46_50,"aac_adp.c:AAC_Deconstruct:error:||(audio_obj->tCodecProcessResouce.left_exp_data_ptr[0]==PNULL)")
TRACE_MSG(AAC_ADP_751_112_2_18_1_8_46_51,"aac_adp.c:AAC_Deconstruct:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_763_112_2_18_1_8_46_52,"aac_adp.c:AAC_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_ui_chng_freq_req_handler")
TRACE_MSG(AAC_ADP_791_112_2_18_1_8_46_53,"aac_adp.c:AAC_PauseDeconstruct:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_825_112_2_18_1_8_46_54,"aac_adp.c:AAC_ResumeConstruct:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_837_112_2_18_1_8_46_55,"aac_adp.c:AAC_ResumeConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_ui_chng_freq_req_handler")
TRACE_MSG(AAC_ADP_865_112_2_18_1_8_46_56,"aac_adp.c:AAC_StopDeconstruct:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_900_112_2_18_1_8_46_57,"aac_adp.c:AAC_StopDeconstruct:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_908_112_2_18_1_8_46_58,"%s, %d. alloc failed! audio handle:0x%08x.")
TRACE_MSG(AAC_ADP_943_112_2_18_1_8_47_59,"aac_adp.c:AAC_Stop:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_951_112_2_18_1_8_47_60,"%s, %d. alloc failed! audio handle:0x%08x.")
TRACE_MSG(AAC_ADP_987_112_2_18_1_8_47_61,"aac_adp.c:AAC_Pause:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_995_112_2_18_1_8_47_62,"%s, %d. alloc failed! audio handle:0x%08x.")
TRACE_MSG(AAC_ADP_1027_112_2_18_1_8_47_63,"aac_adp.c:AAC_Resume:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_1037_112_2_18_1_8_47_64,"%s, %d. alloc failed! audio handle:0x%08x.")
TRACE_MSG(AAC_ADP_1063_112_2_18_1_8_47_65,"[AAC_TimeSeek] ui_offset=%d, uiTime=%d")
TRACE_MSG(AAC_ADP_1098_112_2_18_1_8_47_66,"aac_adp.c:AAC_Seek:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_1105_112_2_18_1_8_47_67,"AAC_Seek aac codec can't seek, because codec_ext_res_ptr==PNULL.")
TRACE_MSG(AAC_ADP_1115_112_2_18_1_8_47_68,"[AAC_Seek] seek signal not processed.")
TRACE_MSG(AAC_ADP_1121_112_2_18_1_8_47_69,"AAC_Seek aac codec can't support seek in neither file_play nor buffer_play. subtype:%d.")
TRACE_MSG(AAC_ADP_1131_112_2_18_1_8_47_70,"%s, %d. alloc failed! audio handle:0x%08x.")
TRACE_MSG(AAC_ADP_1165_112_2_18_1_8_47_71,"aac_adp.c:AAC_GetPlayingInfo:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_1243_112_2_18_1_8_47_72,"aac_adp.c[AAC_GetPlayingInfo] stream, curTime:%d, %d should not less than:%d(=%d+%d).")
TRACE_MSG(AAC_ADP_1266_112_2_18_1_8_47_73,"aac_adp.c:AAC_GetOutputSamplerate:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_1301_112_2_18_1_8_47_74,"aac_adp.c:AAC_IdentifyAudioFormat:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_1311_112_2_18_1_8_47_75,"aac_adp.c:AAC_IdentifyAudioFormat:error:SCI_NULL == audio_obj")
TRACE_MSG(AAC_ADP_1343_112_2_18_1_8_47_76,"AAC_IdentifyAudioFormat format is not surpoort.")
TRACE_MSG(AAC_ADP_1359_112_2_18_1_8_47_77,"[AAC_CODEC]AAC_DEMUX_InitAACStream: sample_rate=%d,channels=%d,time=%ds,bitrate =%d")
TRACE_MSG(AAC_ADP_1364_112_2_18_1_8_47_78,"AAC_IdentifyAudioFormat format is not surpoort.")
TRACE_MSG(AAC_ADP_1374_112_2_18_1_8_47_79,"AAC_IdentifyAudioFormat format is not surpoort.")
TRACE_MSG(AAC_ADP_1392_112_2_18_1_8_47_80,"AAC_IdentifyAudioFormat format is not surpoort.")
TRACE_MSG(AAC_ADP_1404_112_2_18_1_8_47_81,"AAC_IdentifyAudioFormat no enough space to save header data.")
TRACE_MSG(AAC_ADP_1426_112_2_18_1_8_47_82,"AAC_IdentifyAudioFormat, stream format is not supported.")
TRACE_MSG(AAC_ADP_1429_112_2_18_1_8_47_83,"%s, %d can't identify audio format in streambuffer_playing.")
TRACE_MSG(AAC_ADP_1434_112_2_18_1_8_47_84,"aac_adp.c:AAC_IdentifyAudioFormat:error:unknown file handle type!")
TRACE_MSG(AAC_ADP_1449_112_2_18_1_8_48_85,"AAC_IdentifyAudioFormat, uiSamplerate;%d, total_time:%d.")
TRACE_MSG(AAC_ADP_1533_112_2_18_1_8_48_86,"aac_adp.c:AAC_SendSig: error: Unknown Command!")
TRACE_MSG(AAC_ADP_1604_112_2_18_1_8_48_87,"AAC_Thread_Entry  audio_obj->tAudioFormatInfo.uiTotalTime = 0.")
TRACE_MSG(AAC_ADP_1626_112_2_18_1_8_48_88,"aac_adp.c:AAC_PLAY: Unknown Handle type!")
TRACE_MSG(AAC_ADP_1674_112_2_18_1_8_48_89,"%s, %d, get src data error:%d, frame_id:%d.")
TRACE_MSG(AAC_ADP_1680_112_2_18_1_8_48_90,"%s, %d, len of src data is zero, frame_id:%d.")
TRACE_MSG(AAC_ADP_1740_112_2_18_1_8_48_91,"%s, %d, decode error:%d, frame_id: %d.")
TRACE_MSG(AAC_ADP_1764_112_2_18_1_8_48_92,"pGetSrcCallbackFunc over 0 == codec_ext_res_ptr->ui_aac_stream_data_len.")
TRACE_MSG(AAC_ADP_1794_112_2_18_1_8_48_93,"aac_adp.c: AAC_FRAME_DEC: error: AAC_Thread_Entry not support asyn get stream src data.")
TRACE_MSG(AAC_ADP_1800_112_2_18_1_8_48_94,"pGetSrcCallbackFunc result:AUDIO_STREAM_GET_SRC_ERROR.")
TRACE_MSG(AAC_ADP_1808_112_2_18_1_8_48_95,"aac_adp.c: AAC_FRAME_DEC: error: AAC_Thread_Entry pGetSrcCallbackFunc result is not invalid.")
TRACE_MSG(AAC_ADP_1834_112_2_18_1_8_48_96,"%s, %d, \"# decode error:%d, frame_id: %d.")
TRACE_MSG(AAC_ADP_1843_112_2_18_1_8_48_97,"%s, %d, decode error:%d, frame_id: %d.")
TRACE_MSG(AAC_ADP_1917_112_2_18_1_8_48_98,"aac_adp.c:AAC_RESUME: error: Unknown Handle type!")
TRACE_MSG(AAC_ADP_1923_112_2_18_1_8_48_99,"AAC_Thread_Entry AAC_RESUME AAC_DecInit error.")
TRACE_MSG(AAC_ADP_1955_112_2_18_1_8_48_100,"AAC_SEEK ui_offset(%d)>ui_total_len(%d), aac codec will set ui_offset=ui_total_len.")
TRACE_MSG(AAC_ADP_1968_112_2_18_1_8_48_101,"AAC_SEEK success! ori_offset:%d, dest_offset:%d.")
TRACE_MSG(AAC_ADP_1973_112_2_18_1_8_49_102,"AAC_SEEK failed! ori_offset:%d, dest_offset:%d.")
TRACE_MSG(AAC_ADP_1979_112_2_18_1_8_49_103,"%s, %d get src proc req, audio handle:0x%08x.")
TRACE_MSG(AAC_ADP_1986_112_2_18_1_8_49_104,"aac_adp.c:AAC_Thread_Entry: error: Unknown Command!")
TRACE_MSG(AAC_DECODER_197_112_2_18_1_9_23_105,"aac_decoder:error:result is %d")
TRACE_MSG(AAC_DECODER_354_112_2_18_1_9_23_106,"aac_decoder.c [AAC_MemoryAlloc], allocLen:%d.")
TRACE_MSG(MAIN_208_112_2_18_1_10_42_107,"audio_type:%d.")
TRACE_MSG(AAC_INTERFACE_107_112_2_18_1_10_44_108,"AAC_DEMUX_Construct ALLOC SIZE:%d.")
TRACE_MSG(AAC_INTERFACE_113_112_2_18_1_10_44_109,"aac_interface.c:AAC_DEMUX_Construct:error:PNULL == demux_mem_ptr")
TRACE_MSG(AAC_INTERFACE_119_112_2_18_1_10_44_110,"aac_interface.c [AAC_DEMUX_Construct], allocLen:%d.")
TRACE_MSG(AAC_INTERFACE_136_112_2_18_1_10_44_111,"aac_interface.c:AAC_DEMUX_Deconstruct:error:demux_mem_ptr==NULL")
TRACE_MSG(AAC_INTERFACE_177_112_2_18_1_10_44_112,"aac_interface.c:AAC_DEMUX_InitAACStream:error:format_info_ptr==PNULL)||(header_info_ptr==PNULL")
TRACE_MSG(AAC_INTERFACE_215_112_2_18_1_10_44_113,"aac_interface.c:AAC_DEMUX_GetFrameData:error:demux_mem_ptr is %d,frame_info_ptr is %d")
TRACE_MSG(AAC_INTERFACE_242_112_2_18_1_10_44_114,"aac_interface.c:AAC_DEMUX_Seek:error:demux_mem_ptr==PNULL")
TRACE_MSG(AAC_INTERFACE_277_112_2_18_1_10_44_115,"aac_interface.c:AAC_DEMUX_GetCurPos:error:demux_mem_ptr==PNULL")
TRACE_MSG(AAC_INTERFACE_440_112_2_18_1_10_44_116,"aac_interface.c:AACLC_InitAACStream:error:tr)||(init_pata_ptr==PNULL)||(id3_stuc_ptr==")
TRACE_MSG(AAC_INTERFACE_460_112_2_18_1_10_44_117,"AACLC_InitAACStream read head info error(should be 10)!, %d")
TRACE_MSG(AAC_INTERFACE_468_112_2_18_1_10_44_118,"AAC_LC_DEBUG:m4a format")
TRACE_MSG(AAC_INTERFACE_480_112_2_18_1_10_44_119,"AAC_LC_DEBUG:aac format")
TRACE_MSG(AAC_INTERFACE_496_112_2_18_1_10_44_120,"aac_interface.c:AACLC_InitAACStream:error:PNULL == init_pata_ptr->aac_lc_info.head_data_ptr ")
TRACE_MSG(AAC_INTERFACE_507_112_2_18_1_10_44_121,"aac_interface.c:AACLC_InitAACStream:error:PNULL == demux_mem_ptr->aac_adts_da")
TRACE_MSG(AAC_INTERFACE_520_112_2_18_1_10_44_122,"AACLC_InitAACStream read byte, %d")
TRACE_MSG(AAC_INTERFACE_523_112_2_18_1_10_44_123,"AACLC_InitAACStream 1 == adts_m4a_file.")
TRACE_MSG(AAC_INTERFACE_541_112_2_18_1_10_45_124,"aac_interface.c:AACLC_InitAACStream:error:tag is too large!tagsize=%d \n")
TRACE_MSG(AAC_INTERFACE_544_112_2_18_1_10_45_125,"AACLC_InitAACStream 1 == adts_m4a_file ID3.")
TRACE_MSG(AAC_INTERFACE_572_112_2_18_1_10_45_126,"AACLC_InitAACStream read byte:%d")
TRACE_MSG(AAC_INTERFACE_589_112_2_18_1_10_45_127,"AAC_LC_DEBUG:the ADIF format")
TRACE_MSG(AAC_INTERFACE_599_112_2_18_1_10_45_128,"AAC_LC_DEBUG:not AAC-LC")
TRACE_MSG(AAC_INTERFACE_619_112_2_18_1_10_45_129,"AAC_LC_DEBUG:sr_index > 16")
TRACE_MSG(AAC_INTERFACE_637_112_2_18_1_10_45_130,"AAC_LC_DEBUG:sr_index > 16")
TRACE_MSG(AAC_INTERFACE_677_112_2_18_1_10_45_131,"AACLC_InitAACStream m4a after AAC_MEMSET.")
TRACE_MSG(AAC_INTERFACE_682_112_2_18_1_10_45_132,"AACLC_InitAACStream m4a demux_mem_ptr->aaclc_infile_ptr == PNULL. ")
TRACE_MSG(AAC_INTERFACE_685_112_2_18_1_10_45_133,"AACLC_InitAACStream m4a aaclc_infile_ptr AAC_MALLOC size:%d.")
TRACE_MSG(AAC_INTERFACE_703_112_2_18_1_10_45_134,"%s, %d")
TRACE_MSG(AAC_INTERFACE_724_112_2_18_1_10_45_135,"aac_interface.c:AACLC_InitAACStream:error:0==demux_mem_ptr->m4a_info.total_frame")
TRACE_MSG(AAC_INTERFACE_818_112_2_18_1_10_45_136,"aac_lc_Adts_GetData read byte:%d")
TRACE_MSG(AAC_INTERFACE_996_112_2_18_1_10_46_137,"aac_interface.c:AACLC_ReadAdtsStreamFile:error:PNULL == demux_mem_ptr->aac_adts_data_struc.s")
TRACE_MSG(AAC_INTERFACE_1012_112_2_18_1_10_46_138,"AACLC_ReadAdtsStreamFile read byte:%d")
TRACE_MSG(AAC_INTERFACE_1050_112_2_18_1_10_46_139,"aac_interface.c:aac_lc_M4a_GetData:error:em_ptr)||(PNULL == ppucDataAddr)||(PNULL== audio")
TRACE_MSG(AAC_INTERFACE_1079_112_2_18_1_10_46_140,"aac_interface.c:aac_lc_M4a_GetData:error:(rel==-1)")
TRACE_MSG(AAC_INTERFACE_1097_112_2_18_1_10_46_141,"aac_interface.c:aac_lc_M4a_GetData:error:0 == read_len")
TRACE_MSG(AAC_INTERFACE_1106_112_2_18_1_10_46_142,"aac_lc_M4a_GetData frame_id;%d, total_frame:%d.")
TRACE_MSG(AAC_INTERFACE_1117_112_2_18_1_10_46_143,"aac_interface.c:aac_lc_M4a_GetData:error:0==(*puiDataLength) || 2048<(*puiDataLength) is %d")
TRACE_MSG(AAC_INTERFACE_1142_112_2_18_1_10_46_144,"aac_lc_M4a_GetData 0xfff == sync, %d, %d.")
TRACE_MSG(AAC_INTERFACE_1150_112_2_18_1_10_46_145,"aac_lc_M4a_GetData: %d, %d.")
TRACE_MSG(AAC_INTERFACE_1170_112_2_18_1_10_46_146,"aac_interface.c:aac_lc_Adts_Seek:error:PNULL == demux_mem_ptr")
TRACE_MSG(AAC_INTERFACE_1202_112_2_18_1_10_46_147,"aac_interface.c:aac_lc_m4a_Seek:error:PNULL == demux_mem_ptr")
TRACE_MSG(AAC_INTERFACE_1245_112_2_18_1_10_46_148,"aac_interface.c:aac_Adts_GetCurrentPos:error:PNULL == demux_mem_ptr")
TRACE_MSG(AAC_INTERFACE_1268_112_2_18_1_10_46_149,"aac_interface.c:aac_m4a_GetCurrentPos:error:PNULL == demux_mem_ptr")
TRACE_MSG(AAC_INTERFACE_1544_112_2_18_1_10_47_150,"AACLC_ADTSParseTime read sync last len != 8, %d")
TRACE_MSG(AAC_INTERFACE_1653_112_2_18_1_10_47_151,"aac_interface.c:aac_Average_Frame_size:error:header==NULL")
TRACE_MSG(AAC_INTERFACE_1901_112_2_18_1_10_47_152,"end of file is %d")
TRACE_MSG(AAC_LC_ADP_446_112_2_18_1_10_49_153,"aac_lc_adp.c:AAC_LC_FreeInConstruct:error:NULL==audio_obj_ptr")
TRACE_MSG(AAC_LC_ADP_497_112_2_18_1_10_49_154,"aac_lc_adp.c:AAC_LC_FreeInConstruct:error:NULL==audio_obj_ptr")
TRACE_MSG(AAC_LC_ADP_543_112_2_18_1_10_49_155,"aac_lc_adp.c:AAC_LC_Construct:error:res_ptr->src_data_ptr == PNULL")
TRACE_MSG(AAC_LC_ADP_548_112_2_18_1_10_49_156,"aac_lc_adp.c [AAC_LC_Construct], allocLen:%d.")
TRACE_MSG(AAC_LC_ADP_558_112_2_18_1_10_49_157,"aac_lc_adp.c [AAC_LC_Construct], allocLen:%d.")
TRACE_MSG(AAC_LC_ADP_568_112_2_18_1_10_49_158,"aac_lc_adp.c [AAC_LC_Construct], allocLen:%d.")
TRACE_MSG(AAC_LC_ADP_582_112_2_18_1_10_49_159,"aac_lc_adp.c:AAC_LC_Construct:error:0x%08x,0x%08x,0x%08x,0x%08x,0x%08x")
TRACE_MSG(AAC_LC_ADP_614_112_2_18_1_10_49_160,"aac_lc_adp.c:AAC_LC_Construct:error:PNULL == adp_context_ptr")
TRACE_MSG(AAC_LC_ADP_620_112_2_18_1_10_49_161,"aac_lc_adp.c [AAC_LC_Construct], allocLen:%d.")
TRACE_MSG(AAC_LC_ADP_630_112_2_18_1_10_49_162,"aac_lc_adp.c:AAC_LC_Construct:error:E_EXP_LEN < AAC_LC_F")
TRACE_MSG(AAC_LC_ADP_647_112_2_18_1_10_49_163,"AAC_LC_Construct SCI_CreateMutex failed!")
TRACE_MSG(AAC_LC_ADP_658_112_2_18_1_10_49_164,"aac_lc_adp.c:AAC_LC_Construct:error:PNULL == demux_context_ptr->tAACLCDemuxFrameBuf[i].uc_frame_data_ptr")
TRACE_MSG(AAC_LC_ADP_665_112_2_18_1_10_49_165,"aac_lc_adp.c [AAC_LC_Construct], allocLen:%d.")
TRACE_MSG(AAC_LC_ADP_732_112_2_18_1_10_49_166,"aac_lc_adp.c:AAC_LC_PlayConstruct:error:PNULL == res_ptr->right_exp_data_ptr[0]......")
TRACE_MSG(AAC_LC_ADP_748_112_2_18_1_10_49_167,"aac_lc_adp.c:AAC_LC_PlayConstruct:error:PNULL == adp_context_ptr->pusPcmROutBuf......")
TRACE_MSG(AAC_LC_ADP_765_112_2_18_1_10_49_168,"aac_lc_adp.c:AAC_LC_PlayConstruct:error:PNULL == demux_context_ptr->tAACLCDemuxFrameBuf[i].uc_frame_data_ptr......")
TRACE_MSG(AAC_LC_ADP_779_112_2_18_1_10_49_169,"aac_lc_adp.c:AAC_LC_PlayConstruct:error:AUDIO_DEFAULT_TASK_ID != audio_obj_ptr->tAssiThreadId)......")
TRACE_MSG(AAC_LC_ADP_817_112_2_18_1_10_50_170,"[AAC_LC_CODEC]AAC_LC_Play, ThreadID:0x%x,0x%x")
TRACE_MSG(AAC_LC_ADP_828_112_2_18_1_10_50_171,"aac_lc_adp.c:AAC_LC_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_ui_chng_freq_req_handler")
TRACE_MSG(AAC_LC_ADP_1013_112_2_18_1_10_50_172,"aac_lc_adp.c:AAC_LC_ResumeConstruct:error:0x%08x,0x%08x,0x%08x,0x%08x,0x%08x")
TRACE_MSG(AAC_LC_ADP_1044_112_2_18_1_10_50_173,"aac_lc_adp.c:AAC_LC_ResumeConstruct:error:PNULL == adp_context_ptr")
TRACE_MSG(AAC_LC_ADP_1067_112_2_18_1_10_50_174,"aac_lc_adp.c:AAC_LC_ResumeConstruct:error:AUDIO_DEFAULT_TASK_ID != audio_obj_ptr->tThreadId")
TRACE_MSG(AAC_LC_ADP_1104_112_2_18_1_10_50_175,"[AAC_LC_CODEC]AAC_LC_Play, ThreadID:0x%x,0x%x")
TRACE_MSG(AAC_LC_ADP_1142_112_2_18_1_10_50_176,"aac_lc_adp.c:AAC_LC_StopDeconstruct:error:PNULL == adp_context_ptr")
TRACE_MSG(AAC_LC_ADP_1253_112_2_18_1_10_50_177,"aac_lc_adp.c:AAC_LC_Play:error:AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId")
TRACE_MSG(AAC_LC_ADP_1293_112_2_18_1_10_50_178,"aac_lc_adp.c:AAC_LC_Stop:error:AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId")
TRACE_MSG(AAC_LC_ADP_1318_112_2_18_1_10_51_179,"aac_lc_adp.c:AAC_LC_Pause:error:AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId")
TRACE_MSG(AAC_LC_ADP_1343_112_2_18_1_10_51_180,"aac_lc_adp.c:AAC_LC_Pause:error:AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId")
TRACE_MSG(AAC_LC_ADP_1388_112_2_18_1_10_51_181,"aac_lc_adp.c:AAC_LC_Seek:error:||(NULL==adp_context_ptr )")
TRACE_MSG(AAC_LC_ADP_1414_112_2_18_1_10_51_182,"[AAC_LC] SEEK IS NO MEANING FOR THIS TIME")
TRACE_MSG(AAC_LC_ADP_1434_112_2_18_1_10_51_183,"[AAC_LC_TimeSeek] PNULL == adp_context_ptr")
TRACE_MSG(AAC_LC_ADP_1458_112_2_18_1_10_51_184,"[AAC_TimeSeek] SEEK IS NO MEANING FOR THIS TIME")
TRACE_MSG(AAC_LC_ADP_1460_112_2_18_1_10_51_185,"[AAC_TimeSeek] ui_offset=%d, uiTime=%d")
TRACE_MSG(AAC_LC_ADP_1608_112_2_18_1_10_51_186,"aac_lc_adp.c[AAC_LC_GetPlayingInfo] stream, curTime:%d, %d should not less than:%d(=%d+%d).")
TRACE_MSG(AAC_LC_ADP_1625_112_2_18_1_10_51_187,"aac_lc_adp.c:AAC_LC_SendStreamData:error:AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tAssiThreadId")
TRACE_MSG(AAC_LC_ADP_1753_112_2_18_1_10_51_188,"[AAC_LC_PLAY] aac_decoder_init failed!!!sample_rate=%d, result=%d")
TRACE_MSG(AAC_LC_ADP_1760_112_2_18_1_10_51_189,"[AAC_LC_PLAY] aac decoder init finished.")
TRACE_MSG(AAC_LC_ADP_1777_112_2_18_1_10_51_190,"_AAC_LC_ReadDemuxFrame other task has been playing")
TRACE_MSG(AAC_LC_ADP_1787_112_2_18_1_10_51_191,"[AAC_LC]PLAY END!")
TRACE_MSG(AAC_LC_ADP_1830_112_2_18_1_10_52_192,"[AAC_LC_FRAME_DEC] aac decode failed.")
TRACE_MSG(AAC_LC_ADP_1847_112_2_18_1_10_52_193,"[AAC_LC]PLAY END!")
TRACE_MSG(AAC_LC_ADP_1863_112_2_18_1_10_52_194,"[AAC_LC]PAUSE BUT NOW DEMUX IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN")
TRACE_MSG(AAC_LC_ADP_1869_112_2_18_1_10_52_195,"[AAC_LC]PAUSE SUCCESS!")
TRACE_MSG(AAC_LC_ADP_1884_112_2_18_1_10_52_196,"[AAC_LC] stop waiting for track buffer empty!")
TRACE_MSG(AAC_LC_ADP_1892_112_2_18_1_10_52_197,"[AAC_LC]STOP BUT NOW DEMUX IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN")
TRACE_MSG(AAC_LC_ADP_1898_112_2_18_1_10_52_198,"[AAC_LC]STOP SUCCESS!")
TRACE_MSG(AAC_LC_ADP_1906_112_2_18_1_10_52_199,"[_AAC_LC_Thread_Entry] Unknown Message!!!")
TRACE_MSG(AAC_LC_ADP_1962_112_2_18_1_10_52_200,"[AAC_LC] DECODE INIT")
TRACE_MSG(AAC_LC_ADP_1968_112_2_18_1_10_52_201,"[AAC_LC]DSP DECODE INIT DONE")
TRACE_MSG(AAC_LC_ADP_1971_112_2_18_1_10_52_202,"[AAC_LC]DSP DECODE INIT DONE,return_result: %d")
TRACE_MSG(AAC_LC_ADP_1985_112_2_18_1_10_52_203,"aac_lc_adp.c:AAC_LC_HEADER_IND:error:tAudioFormatInfo.uiSamplerate !=sig_decode_ptr->uiSampleRate")
TRACE_MSG(AAC_LC_ADP_2008_112_2_18_1_10_52_204,"[AAC_LC]PLAY END!")
TRACE_MSG(AAC_LC_ADP_2037_112_2_18_1_10_52_205,"[AAC_LC]PLAY END!")
TRACE_MSG(AAC_LC_ADP_2124_112_2_18_1_10_52_206,"[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:1.")
TRACE_MSG(AAC_LC_ADP_2130_112_2_18_1_10_52_207,"[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:2.")
TRACE_MSG(AAC_LC_ADP_2138_112_2_18_1_10_52_208,"[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:default")
TRACE_MSG(AAC_LC_ADP_2162_112_2_18_1_10_52_209,"[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:1.")
TRACE_MSG(AAC_LC_ADP_2168_112_2_18_1_10_52_210,"[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:2.")
TRACE_MSG(AAC_LC_ADP_2176_112_2_18_1_10_52_211,"[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:default")
TRACE_MSG(AAC_LC_ADP_2212_112_2_18_1_10_52_212,"[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:1.")
TRACE_MSG(AAC_LC_ADP_2218_112_2_18_1_10_52_213,"[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:2.")
TRACE_MSG(AAC_LC_ADP_2226_112_2_18_1_10_52_214,"[AAC_LC_CODEC]Thread_Entry, AAC_LC_FRAME_IND error:default")
TRACE_MSG(AAC_LC_ADP_2240_112_2_18_1_10_52_215,"[AAC_LC]PLAY END!")
TRACE_MSG(AAC_LC_ADP_2256_112_2_18_1_10_52_216,"[AAC_LC]PAUSE BUT NOW DEMUX IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN")
TRACE_MSG(AAC_LC_ADP_2265_112_2_18_1_10_52_217,"[AAC_LC]PAUSE SUCCESS!")
TRACE_MSG(AAC_LC_ADP_2273_112_2_18_1_10_52_218,"[AAC_LC]PAUSE BUT NOW DSP IS DECODE, WAIT SOME TIME THEN TRY AGAIN")
TRACE_MSG(AAC_LC_ADP_2289_112_2_18_1_10_52_219,"[AAC_LC] stop waiting for track buffer empty!")
TRACE_MSG(AAC_LC_ADP_2297_112_2_18_1_10_52_220,"[AAC_LC]STOP BUT NOW DEMUX IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN")
TRACE_MSG(AAC_LC_ADP_2306_112_2_18_1_10_52_221,"[AAC_LC]STOP SUCCESS!")
TRACE_MSG(AAC_LC_ADP_2314_112_2_18_1_10_52_222,"[AAC_LC]STOP BUT NOW DSP IS DECODE, WAIT SOME TIME THEN TRY AGAIN")
TRACE_MSG(AAC_LC_ADP_2384_112_2_18_1_10_53_223,"[AAC_LC]:DEMUX INIT OK!")
TRACE_MSG(AAC_LC_ADP_2404_112_2_18_1_10_53_224,"[AAC_LC]:DEMUX Resume INIT OK!")
TRACE_MSG(AAC_LC_ADP_2462_112_2_18_1_10_53_225,"[AAC_LC]:DEMUX THREAD RECV A BAD SIGNAL, DISCARD IT")
TRACE_MSG(AAC_LC_ADP_2524_112_2_18_1_10_53_226,"aac_lc_adp.c:_AAC_LC_DEMUX_Init:error:PNULL == adp_context_ptr")
TRACE_MSG(AAC_LC_ADP_2549_112_2_18_1_10_53_227,"aac_lc_adp.c:_AAC_LC_DEMUX_Init:error:demux_context_ptr->hAACLCDemuxHandle == NULL")
TRACE_MSG(AAC_LC_ADP_2613_112_2_18_1_10_53_228,"[AAC_LC_CODEC]AAC_LC_demux_get_info: sample_rate=%d,channels=%d,time=%ds,bitrate =%d")
TRACE_MSG(AAC_LC_ADP_2643_112_2_18_1_10_53_229,"aac_lc_adp.c: AUDIO_STREAM_BUFFER_HANDLE demux_init sample rate(%d) is not support, count:%d, index:%d.")
TRACE_MSG(AAC_LC_ADP_2662_112_2_18_1_10_53_230,"aac_lc_adp.c: AUDIO_STREAM_BUFFER_HANDLE demux header data:%d, %d.")
TRACE_MSG(AAC_LC_ADP_2668_112_2_18_1_10_53_231,"aac_lc_adp.c: AUDIO_STREAM_BUFFER_HANDLE demux_init failed.")
TRACE_MSG(AAC_LC_ADP_2692_112_2_18_1_10_53_232,"aac_lc_adp.c:_AAC_LC_DEMUX_Init:error:aac_lc_head_info.uc_head_data_ptr == NULL")
TRACE_MSG(AAC_LC_ADP_2756_112_2_18_1_10_53_233,"aac_lc_adp.c:_AAC_LC_ReadDemuxFrame:error:ject_ptr == PNULL)||(aacDemuxOut_ptrptr == PNU")
TRACE_MSG(AAC_LC_ADP_2765_112_2_18_1_10_53_234,"aac_lc_adp.c:_AAC_LC_ReadDemuxFrame:error:adp_context_ptr == PNULL")
TRACE_MSG(AAC_LC_ADP_2861_112_2_18_1_10_54_235,"aac_lc_adp.c:_AAC_LC_WriteDemuxFrame,get frame len ==0: demux is end")
TRACE_MSG(AAC_LC_ADP_2867_112_2_18_1_10_54_236,"aac_lc_adp.c:_AAC_LC_WriteDemuxFrame: demux is end")
TRACE_MSG(AAC_LC_ADP_3030_112_2_18_1_10_54_237,"_AAC_LC_ResetDemuxFrames other task has been playing")
TRACE_MSG(AAC_LC_ADP_3129_112_2_18_1_10_54_238,"[_AAC_LC_Frame_Ind_Callback] NULL == s_cur_audio_object_ptr")
TRACE_MSG(AAC_LC_ADP_3135_112_2_18_1_10_54_239,"[_AAC_LC_Frame_Ind_Callback] stop waiting for track buffer empty")
TRACE_MSG(AAC_LC_ADP_3270_112_2_18_1_10_54_240,"[AAC_LC]PLAY END!")
TRACE_MSG(AAC_LC_ADP_3296_112_2_18_1_10_55_241,"[AAC_LC]PLAY END!")
TRACE_MSG(AAC_LC_ADP_3448_112_2_18_1_10_55_242,"aac_lc_adp.c:_AAC_LC_SendSig:error:PNULL == sig_out_ptr")
TRACE_MSG(AAC_LC_ADP_3467_112_2_18_1_10_55_243,"aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_FRAME_DEC:error:uiReceiver == audio_obj_ptr->tThreadId")
TRACE_MSG(AAC_LC_ADP_3488_112_2_18_1_10_55_244,"aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_FRAME_IND:error:uiReceiver == audio_obj_ptr->tThreadId")
TRACE_MSG(AAC_LC_ADP_3507_112_2_18_1_10_55_245,"aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_DEMUX_FRAME:error:uiReceiver == audio_obj_ptr->tThreadId")
TRACE_MSG(AAC_LC_ADP_3527_112_2_18_1_10_55_246,"aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_DEMUX_START:error:uiReceiver != audio_obj_ptr->tAssiThreadId")
TRACE_MSG(AAC_LC_ADP_3546_112_2_18_1_10_55_247,"aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_DEMUX_START:error:uiReceiver != audio_obj_ptr->tAssiThreadId")
TRACE_MSG(AAC_LC_ADP_3566_112_2_18_1_10_55_248,"aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_DEMUX_RESUME:error:uiReceiver != audio_obj_ptr->tAssiThreadId")
TRACE_MSG(AAC_LC_ADP_3586_112_2_18_1_10_55_249,"aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_DEMUX_STOP:error:uiReceiver != audio_obj_ptr->tAssiThreadId")
TRACE_MSG(AAC_LC_ADP_3604_112_2_18_1_10_55_250,"aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_PLAY:error:uiReceiver == audio_obj_ptr->tThreadId")
TRACE_MSG(AAC_LC_ADP_3626_112_2_18_1_10_55_251,"aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_HEADER_IND:error:uiReceiver == audio_obj_ptr->tThreadId")
TRACE_MSG(AAC_LC_ADP_3644_112_2_18_1_10_55_252,"aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_PAUSE:error:uiReceiver == audio_obj_ptr->tThreadId")
TRACE_MSG(AAC_LC_ADP_3664_112_2_18_1_10_55_253,"aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_STOP:error:uiReceiver == audio_obj_ptr->tThreadId")
TRACE_MSG(AAC_LC_ADP_3684_112_2_18_1_10_55_254,"aac_lc_adp.c:_AAC_LC_SendSig:AAC_LC_STOP:error:uiReceiver == audio_obj_ptr->tAssiThreadId")
TRACE_MSG(AAC_LC_ADP_3692_112_2_18_1_10_55_255,"[AAC_LC]:ERROR :_AAC_LC_SendSig SEND A UNKOWNED SIG!")
TRACE_MSG(MP4ATOM_338_112_2_18_1_10_58_256,"_mp4ff_read_esds, decoderConfig. ALLOC SIZE:%d.")
TRACE_MSG(MP4ATOM_355_112_2_18_1_10_58_257,"_mp4ff_read_esds _mp4ff_read_data len:%d != %d")
TRACE_MSG(MP4ATOM_474_112_2_18_1_10_58_258,"_mp4ff_read_stsc, stsc_first_chunk. ALLOC SIZE:%d.")
TRACE_MSG(MP4ATOM_484_112_2_18_1_10_58_259,"_mp4ff_read_stsc, stsc_samples_per_chunk. ALLOC SIZE:%d.")
TRACE_MSG(MP4ATOM_494_112_2_18_1_10_58_260,"_mp4ff_read_stsc,  stsc_sample_desc_index. ALLOC SIZE:%d.")
TRACE_MSG(MP4ATOM_535_112_2_18_1_10_58_261,"_mp4ff_read_stco f->track_ptr[f->total_tracks - 1]->stco_chunk_offset. ALLOC SIZE:%d.")
TRACE_MSG(MP4ATOM_569_112_2_18_1_10_58_262,"_mp4ff_read_ctts, ctts_sample_count. ALLOC SIZE:%d.")
TRACE_MSG(MP4ATOM_576_112_2_18_1_10_58_263,"_mp4ff_read_ctts,  ctts_sample_offset. ALLOC SIZE:%d.")
TRACE_MSG(MP4ATOM_646_112_2_18_1_10_59_264,"_mp4ff_read_stts, stts_sample_count. ALLOC SIZE:%d.")
TRACE_MSG(MP4ATOM_652_112_2_18_1_10_59_265,"_mp4ff_read_stts, stts_sample_delta. ALLOC SIZE:%d.")
TRACE_MSG(MP4FF_269_112_2_18_1_11_0_266,"_mp4ff_track_add f->track_ptr[f->total_tracks - 1] . ALLOC SIZE:%d.")
TRACE_MSG(MP4FF_519_112_2_18_1_11_0_267,"mp4ff.c:_mp4ff_num_samples:error:0 == read_len")
TRACE_MSG(MP4FF_525_112_2_18_1_11_0_268,"_mp4ff_num_samples: chunk_buf size= %d")
TRACE_MSG(MP4FF_538_112_2_18_1_11_0_269,"mp4ff.c:_mp4ff_num_samples:error:0 == read_len")
TRACE_MSG(MP4FF_544_112_2_18_1_11_1_270,"_mp4ff_num_samples: stream_buf size= %d")
TRACE_MSG(MP4FF_559_112_2_18_1_11_1_271,"mp4ff.c:_mp4ff_num_samples:error:0 == read_len")
TRACE_MSG(MP4FF_565_112_2_18_1_11_1_272,"_mp4ff_num_samples: audio_buf size= %d")
TRACE_MSG(MP4FF_725_112_2_18_1_11_1_273,"_mp4ff_read_sample: IO error, audio_buf size = 0")
TRACE_MSG(MP4FF_733_112_2_18_1_11_1_274,"_mp4ff_read_sample: audio_buf size= %d")
TRACE_MSG(MP4FF_763_112_2_18_1_11_1_275,"mp4ff.c:_mp4seek_read_sample:error:(result==-1)||(result==0)")
TRACE_MSG(MP4FF_776_112_2_18_1_11_1_276,"mp4ff.c:_mp4seek_read_sample:error:result==-1")
TRACE_MSG(MP4SAMPLE_123_112_2_18_1_11_1_277,"_mp4ff_sample_to_offset: chunk_buf = %d")
TRACE_MSG(MP4SAMPLE_195_112_2_18_1_11_2_278,"mp4sample.c:_mp4ff_audio_frame_size:error:(0 == read_len")
TRACE_MSG(MP4SAMPLE_199_112_2_18_1_11_2_279,"_mp4ff_sample_to_offset: stream_buf size= %d")
TRACE_MSG(MP4SAMPLE_241_112_2_18_1_11_2_280,"mp4sample.c:_mp4ff_set_sample_position:error:offset==-1")
TRACE_MSG(MP4UTIL_35_112_2_18_1_11_2_281,"_mp4ff_read_data: read_len:%d < %d(bytes).")
TRACE_MSG(MP4UTIL_70_112_2_18_1_11_3_282,"_mp4ff_read_int64 _mp4ff_read_data len:%d")
TRACE_MSG(MP4UTIL_95_112_2_18_1_11_3_283,"_mp4ff_read_data _mp4ff_read_data len:%d")
TRACE_MSG(MP4UTIL_114_112_2_18_1_11_3_284,"_mp4ff_read_int24 _mp4ff_read_data len:%d")
TRACE_MSG(MP4UTIL_137_112_2_18_1_11_3_285,"_mp4ff_read_int16 _mp4ff_read_data len:%d")
TRACE_MSG(AD_OUTPUT_ADP_151_112_2_18_1_11_3_286,"_Ad_Output_OfferAdDevice, error. hAudioHandle:0x%08x.")
TRACE_MSG(AD_OUTPUT_ADP_235_112_2_18_1_11_4_287,"_Ad_Output_Play is playing.")
TRACE_MSG(AD_OUTPUT_ADP_259_112_2_18_1_11_4_288,"ad_output_adp.c:_Ad_Output_StartRecord:error:0x%x, 0x%x, 0x%x, 0x%x.")
TRACE_MSG(AD_OUTPUT_ADP_266_112_2_18_1_11_4_289,"ad_output_adp.c:_Ad_Output_StartRecord:s_record_device error:0x%x.")
TRACE_MSG(AD_OUTPUT_ADP_276_112_2_18_1_11_4_290,"ad_output_adp.c:_Ad_Output_StartRecord:error:0x%x, 0x%x, 0x%x.")
TRACE_MSG(AD_OUTPUT_ADP_317_112_2_18_1_11_4_291,"_Ad_Output_Stop is not playing.")
TRACE_MSG(AD_OUTPUT_ADP_348_112_2_18_1_11_4_292,"ad_output_adp.c:_Ad_Output_IdentifyFormat:error:ptAudioObject == SCI_NULL")
TRACE_MSG(AMR_ARM_ADP_110_112_2_18_1_11_4_293,"AMR_Construct in")
TRACE_MSG(AMR_ARM_ADP_116_112_2_18_1_11_4_294,"amr_adp.c:AMR_Construct:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ARM_ADP_125_112_2_18_1_11_4_295,"ERROR amr no memory 2.")
TRACE_MSG(AMR_ARM_ADP_162_112_2_18_1_11_4_296,"AMR_Construct:ptAmrProcRes->frame_buf is NULL")
TRACE_MSG(AMR_ARM_ADP_168_112_2_18_1_11_4_297,"AMR_Construct:ptAmrProcRes->demux_handle is NULL")
TRACE_MSG(AMR_ARM_ADP_174_112_2_18_1_11_4_298,"AMR_Construct:ptAmrProcRes->demux_sym_handle is NULL")
TRACE_MSG(AMR_ARM_ADP_222_112_2_18_1_11_5_299,"amr_adp.c:AMR_Deconstruct in")
TRACE_MSG(AMR_ARM_ADP_228_112_2_18_1_11_5_300,"amr_adp.c:AMR_Play:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ARM_ADP_266_112_2_18_1_11_5_301,"AMR_PlayConstruct in")
TRACE_MSG(AMR_ARM_ADP_271_112_2_18_1_11_5_302,"amr_adp.c:AMR_Play:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ARM_ADP_279_112_2_18_1_11_5_303,"amr_adp.c:AMR_PlayConstruct:ptAmrProcRes->audio_out is null")
TRACE_MSG(AMR_ARM_ADP_296_112_2_18_1_11_5_304,"amr_adp.c:AMR_PlayConstruct:ptAmrProcRes->decode_handle is null")
TRACE_MSG(AMR_ARM_ADP_334_112_2_18_1_11_5_305,"amr_adp.c:AMR_Play:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ARM_ADP_375_112_2_18_1_11_5_306,"amr_arm_adp.c:AMR_pause: demux error")
TRACE_MSG(AMR_ARM_ADP_381_112_2_18_1_11_5_307,"amr_arm_adp.c:AMR_pause: decode error")
TRACE_MSG(AMR_ARM_ADP_410_112_2_18_1_11_5_308,"AMR_Seek in offset is %d")
TRACE_MSG(AMR_ARM_ADP_415_112_2_18_1_11_5_309,"amr_adp.c:AMR_Play:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ARM_ADP_467_112_2_18_1_11_5_310,"amr_adp.c:AMR_Play:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ARM_ADP_475_112_2_18_1_11_5_311,"amr_adp.c:sbc_pcm_data is %d")
TRACE_MSG(AMR_ARM_ADP_496_112_2_18_1_11_5_312,"peter: amr_adp.c: decode_time is %d")
TRACE_MSG(AMR_ARM_ADP_517_112_2_18_1_11_5_313,"amr_adp.c:AMR_GetPlayingInfo:AMR_Demux_GetPlayingPoint result is %d")
TRACE_MSG(AMR_ARM_ADP_561_112_2_18_1_11_5_314,"amr_adp.c:AMR_Play in")
TRACE_MSG(AMR_ARM_ADP_566_112_2_18_1_11_5_315,"amr_adp.c:AMR_Play:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ARM_ADP_577_112_2_18_1_11_5_316,"amr_adp.c:AMR_Play:ptAmrProcRes->demux_handle is NULL")
TRACE_MSG(AMR_ARM_ADP_584_112_2_18_1_11_5_317,"amr_adp.c:AMR_Play:AMR_Demux_start failed %d")
TRACE_MSG(AMR_ARM_ADP_598_112_2_18_1_11_5_318,"amr_adp.c:AMR_Play:ptAmrProcRes->decode_handle is NULL")
TRACE_MSG(AMR_ARM_ADP_607_112_2_18_1_11_5_319,"amr_adp.c:AMR_Play:AMR_Demux_start failed %d")
TRACE_MSG(AMR_ARM_ADP_630_112_2_18_1_11_5_320,"amr_arm_adp.c:AMR_Stop: demux error")
TRACE_MSG(AMR_ARM_ADP_636_112_2_18_1_11_5_321,"amr_arm_adp.c:AMR_Stop: decode error")
TRACE_MSG(AMR_ARM_ADP_659_112_2_18_1_11_5_322,"amr_adp.c:AMR_IdentifyFormat:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_DSP_ADP_110_112_2_18_1_11_6_323,"AMR_Construct in")
TRACE_MSG(AMR_DSP_ADP_116_112_2_18_1_11_6_324,"amr_adp.c:AMR_Construct:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_DSP_ADP_125_112_2_18_1_11_6_325,"ERROR amr no memory 2.")
TRACE_MSG(AMR_DSP_ADP_162_112_2_18_1_11_6_326,"AMR_Construct:ptAmrProcRes->frame_buf is NULL")
TRACE_MSG(AMR_DSP_ADP_168_112_2_18_1_11_6_327,"AMR_Construct:ptAmrProcRes->demux_handle is NULL")
TRACE_MSG(AMR_DSP_ADP_174_112_2_18_1_11_6_328,"AMR_Construct:ptAmrProcRes->demux_sym_handle is NULL")
TRACE_MSG(AMR_DSP_ADP_222_112_2_18_1_11_6_329,"amr_adp.c:AMR_Deconstruct in")
TRACE_MSG(AMR_DSP_ADP_228_112_2_18_1_11_6_330,"amr_adp.c:AMR_Play:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_DSP_ADP_266_112_2_18_1_11_6_331,"AMR_PlayConstruct in")
TRACE_MSG(AMR_DSP_ADP_271_112_2_18_1_11_6_332,"amr_adp.c:AMR_Play:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_DSP_ADP_279_112_2_18_1_11_6_333,"amr_adp.c:AMR_PlayConstruct:ptAmrProcRes->audio_out is null")
TRACE_MSG(AMR_DSP_ADP_296_112_2_18_1_11_6_334,"amr_adp.c:AMR_PlayConstruct:ptAmrProcRes->decode_handle is null")
TRACE_MSG(AMR_DSP_ADP_334_112_2_18_1_11_6_335,"amr_adp.c:AMR_Play:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_DSP_ADP_375_112_2_18_1_11_6_336,"amr_arm_adp.c:AMR_pause: demux error")
TRACE_MSG(AMR_DSP_ADP_381_112_2_18_1_11_6_337,"amr_arm_adp.c:AMR_pause: decode error")
TRACE_MSG(AMR_DSP_ADP_410_112_2_18_1_11_7_338,"AMR_Seek in offset is %d")
TRACE_MSG(AMR_DSP_ADP_415_112_2_18_1_11_7_339,"amr_adp.c:AMR_Play:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_DSP_ADP_467_112_2_18_1_11_7_340,"amr_adp.c:AMR_Play:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_DSP_ADP_477_112_2_18_1_11_7_341,"peter: amr_adp.c: decode_time is %d")
TRACE_MSG(AMR_DSP_ADP_498_112_2_18_1_11_7_342,"amr_adp.c:AMR_GetPlayingInfo:AMR_Demux_GetPlayingPoint result is %d")
TRACE_MSG(AMR_DSP_ADP_541_112_2_18_1_11_7_343,"amr_adp.c:AMR_Play in")
TRACE_MSG(AMR_DSP_ADP_546_112_2_18_1_11_7_344,"amr_adp.c:AMR_Play:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_DSP_ADP_557_112_2_18_1_11_7_345,"amr_adp.c:AMR_Play:ptAmrProcRes->demux_handle is NULL")
TRACE_MSG(AMR_DSP_ADP_564_112_2_18_1_11_7_346,"amr_adp.c:AMR_Play:AMR_Demux_start failed %d")
TRACE_MSG(AMR_DSP_ADP_578_112_2_18_1_11_7_347,"amr_adp.c:AMR_Play:ptAmrProcRes->decode_handle is NULL")
TRACE_MSG(AMR_DSP_ADP_587_112_2_18_1_11_7_348,"amr_adp.c:AMR_Play:AMR_Demux_start failed %d")
TRACE_MSG(AMR_DSP_ADP_610_112_2_18_1_11_7_349,"amr_arm_adp.c:AMR_Stop: demux error")
TRACE_MSG(AMR_DSP_ADP_616_112_2_18_1_11_7_350,"amr_arm_adp.c:AMR_Stop: decode error")
TRACE_MSG(AMR_DSP_ADP_639_112_2_18_1_11_7_351,"amr_adp.c:AMR_IdentifyFormat:error:PNULL == ptAudioObject")
TRACE_MSG(MP3_NEW_ADP_115_112_2_18_1_11_8_352,"MP3_Construct in")
TRACE_MSG(MP3_NEW_ADP_121_112_2_18_1_11_8_353,"MP3_adp.c:MP3_Construct:error:PNULL == ptAudioObject")
TRACE_MSG(MP3_NEW_ADP_130_112_2_18_1_11_8_354,"ERROR MP3 no memory 2.")
TRACE_MSG(MP3_NEW_ADP_170_112_2_18_1_11_8_355,"MP3_Construct:ptMP3ProcRes->frame_buf is NULL")
TRACE_MSG(MP3_NEW_ADP_176_112_2_18_1_11_8_356,"MP3_Construct:ptMP3ProcRes->demux_handle is NULL")
TRACE_MSG(MP3_NEW_ADP_182_112_2_18_1_11_8_357,"MP3_Construct:ptMP3ProcRes->demux_sym_handle  is NULL")
TRACE_MSG(MP3_NEW_ADP_229_112_2_18_1_11_8_358,"MP3_adp.c:MP3_Deconstruct in")
TRACE_MSG(MP3_NEW_ADP_235_112_2_18_1_11_8_359,"MP3_adp.c:MP3_Play:error:PNULL == ptAudioObject")
TRACE_MSG(MP3_NEW_ADP_275_112_2_18_1_11_8_360,"MP3_PlayConstruct in")
TRACE_MSG(MP3_NEW_ADP_280_112_2_18_1_11_8_361,"MP3_adp.c:MP3_Play:error:PNULL == ptAudioObject")
TRACE_MSG(MP3_NEW_ADP_288_112_2_18_1_11_8_362,"MP3_adp.c:MP3_PlayConstruct:ptMP3ProcRes->audio_out is null")
TRACE_MSG(MP3_NEW_ADP_295_112_2_18_1_11_8_363,"MP3_adp.c:MP3_PlayConstruct:ptMP3ProcRes->audio_out is null")
TRACE_MSG(MP3_NEW_ADP_304_112_2_18_1_11_8_364,"MP3_adp.c:MP3_PlayConstruct:ptMP3ProcRes->decode_handle is null")
TRACE_MSG(MP3_NEW_ADP_341_112_2_18_1_11_8_365,"MP3_adp.c:MP3_Play:error:PNULL == ptAudioObject")
TRACE_MSG(MP3_NEW_ADP_383_112_2_18_1_11_8_366,"mp3_new_adp.c: MP3_Pause demux error!")
TRACE_MSG(MP3_NEW_ADP_389_112_2_18_1_11_8_367,"mp3_new_adp.c: MP3_Pause decode error!")
TRACE_MSG(MP3_NEW_ADP_417_112_2_18_1_11_8_368,"MP3_Seek in offset is %d")
TRACE_MSG(MP3_NEW_ADP_422_112_2_18_1_11_8_369,"MP3_adp.c:MP3_Play:error:PNULL == ptAudioObject")
TRACE_MSG(MP3_NEW_ADP_474_112_2_18_1_11_8_370,"MP3_adp.c:MP3_Play:error:PNULL == ptAudioObject")
TRACE_MSG(MP3_NEW_ADP_499_112_2_18_1_11_8_371,"peter: mp3 decode time is %d")
TRACE_MSG(MP3_NEW_ADP_520_112_2_18_1_11_8_372,"MP3_adp.c:MP3_GetPlayingInfo:MP3_Demux_GetPlayingPoint result is %d")
TRACE_MSG(MP3_NEW_ADP_564_112_2_18_1_11_9_373,"MP3_adp.c:MP3_Play in")
TRACE_MSG(MP3_NEW_ADP_569_112_2_18_1_11_9_374,"MP3_adp.c:MP3_Play:error:PNULL == ptAudioObject")
TRACE_MSG(MP3_NEW_ADP_581_112_2_18_1_11_9_375,"MP3_adp.c:MP3_Play:ptMP3ProcRes->demux_handle is null")
TRACE_MSG(MP3_NEW_ADP_587_112_2_18_1_11_9_376,"MP3_adp.c:MP3_Play:MP3_Demux_start failed %d")
TRACE_MSG(MP3_NEW_ADP_601_112_2_18_1_11_9_377,"MP3_adp.c:MP3_Play:ptMP3ProcRes->decode_handle is null")
TRACE_MSG(MP3_NEW_ADP_610_112_2_18_1_11_9_378,"MP3_adp.c:MP3_Play:MP3_Demux_start failed %d")
TRACE_MSG(MP3_NEW_ADP_633_112_2_18_1_11_9_379,"mp3_new_adp.c: MP3_stop demux error!")
TRACE_MSG(MP3_NEW_ADP_639_112_2_18_1_11_9_380,"mp3_new_adp.c: MP3_stop decode error!")
TRACE_MSG(MP3_NEW_ADP_662_112_2_18_1_11_9_381,"MP3_adp.c:MP3_IdentifyFormat:error:PNULL == ptAudioObject")
TRACE_MSG(RECORD_ADP_248_112_2_18_1_11_10_382,"RECORD_adp.c:RECORD_Construct:error:PNULL == ptAudioObject")
TRACE_MSG(RECORD_ADP_287_112_2_18_1_11_10_383,"RECORD_adp.c:RECORD_Play:error:PNULL == ptAudioObject")
TRACE_MSG(RECORD_ADP_310_112_2_18_1_11_10_384,"RECORD_adp.c:RECORD_Play:error:PNULL == ptAudioObject")
TRACE_MSG(RECORD_ADP_328_112_2_18_1_11_10_385,"record_adp.c:RECORD_PlayConstruct:dsp_source_open failed")
TRACE_MSG(RECORD_ADP_337_112_2_18_1_11_10_386,"record_adp.c:RECORD_PlayConstruct:arm_source_open failed")
TRACE_MSG(RECORD_ADP_525_112_2_18_1_11_10_387,"RECORD_adp.c:RECORD_Play:error:PNULL == ptAudioObject")
TRACE_MSG(RECORD_ADP_586_112_2_18_1_11_10_388,"RECORD_arm_adp.c:RECORD_Stop: encode error")
TRACE_MSG(RECORD_ADP_616_112_2_18_1_11_10_389,"RECORD_adp.c:RECORD_IdentifyFormat:error:PNULL == ptAudioObject")
TRACE_MSG(RECORD_ADP_635_112_2_18_1_11_10_390,"RECORD_adp.c:RECORD_Play:error:PNULL == ptAudioObject")
TRACE_MSG(RECORD_ADP_660_112_2_18_1_11_11_391,"RECORD_arm_adp.c:RECORD_pause: encode error")
TRACE_MSG(RECORD_ADP_670_112_2_18_1_11_11_392,"RECORD_arm_adp.c:RECORD_pause: encode error")
TRACE_MSG(RECORD_ADP_692_112_2_18_1_11_11_393,"RECORD_arm_adp.c:RECORD_Resume: aud_mux_start:encode error")
TRACE_MSG(RECORD_ADP_702_112_2_18_1_11_11_394,"record_adp.c:RECORD_Resume:aud_encode_start: result is %d")
TRACE_MSG(RECORD_ADP_745_112_2_18_1_11_11_395,"record_adp.c:Record_GetRecordTime:error:ME_COUNT)||(argv==SCI_NULL")
TRACE_MSG(RECORD_ADP_782_112_2_18_1_11_11_396,"adpcmrecord.c:Record_SetParam:error:SET_COUNT)||(argv==SCI_NULL")
TRACE_MSG(RECORD_ADP_797_112_2_18_1_11_11_397,"adpcmrecord.c:Record_SetParam:error:0x%x, 0x%x, 0x%x, 0x%x, 0x%x")
TRACE_MSG(RECORD_ADP_869_112_2_18_1_11_11_398,"adpcmrecord.c:Record_GetVoiceDB:error:ME_COUNT)||(argv==SCI_NULL")
TRACE_MSG(RECORD_ADP_897_112_2_18_1_11_11_399,"adpcmrecord.c:Record_GetVoiceDB:error:ME_COUNT)||(argv==SCI_NULL")
TRACE_MSG(RECORD_ADP_925_112_2_18_1_11_11_400,"record_adp.c:[Record_SetEndTime] INPUT PARAM ERROR!")
TRACE_MSG(RECORD_ADP_955_112_2_18_1_11_11_401,"[Record_SetEndSize] INPUT PARAM ERROR!")
TRACE_MSG(RECORD_ADP_992_112_2_18_1_11_11_402,"[Record_SetCBTime] INPUT PARAM ERROR!")
TRACE_MSG(RECORD_ADP_1021_112_2_18_1_11_11_403,"[Record_GetCurFileSize] INPUT PARAM ERROR!")
TRACE_MSG(RECORD_ADP_1055_112_2_18_1_11_11_404,"adpcmrecord.c:Record_SetIfFromNet:error:ERROR_COUNT)||(argv==SCI_NULL")
TRACE_MSG(AMR_ADP_281_112_2_18_1_11_12_405,"amr_adp.c:AMR_Construct:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_290_112_2_18_1_11_12_406,"ERROR amr no memory 1.")
TRACE_MSG(AMR_ADP_295_112_2_18_1_11_12_407,"amr_adp.c AMR_Construct allocLen:%d.")
TRACE_MSG(AMR_ADP_301_112_2_18_1_11_12_408,"ERROR amr no memory 2.")
TRACE_MSG(AMR_ADP_307_112_2_18_1_11_12_409,"amr_adp.c AMR_Construct allocLen:%d.")
TRACE_MSG(AMR_ADP_348_112_2_18_1_11_12_410,"amr_adp.c:AMR_Deconstruct:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_415_112_2_18_1_11_13_411,"amr_adp.c:AMR_PlayConstruct:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_436_112_2_18_1_11_13_412,"amr_adp.c:AMR_ResumeConstruct:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_468_112_2_18_1_11_13_413,"amr_adp.c:AMR_PauseDeconstruct:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_501_112_2_18_1_11_13_414,"amr_adp.c:AMR_StopDeconstruct:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_536_112_2_18_1_11_13_415,"amr_adp.c:AMR_Play:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_544_112_2_18_1_11_13_416,"ERROR amr no memory 3.")
TRACE_MSG(AMR_ADP_570_112_2_18_1_11_13_417,"amr_adp.c:AMR_Stop:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_580_112_2_18_1_11_13_418,"ERROR amr no memory 4.")
TRACE_MSG(AMR_ADP_608_112_2_18_1_11_13_419,"amr_adp.c:AMR_Pause:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_618_112_2_18_1_11_13_420,"ERROR amr no memory 5.")
TRACE_MSG(AMR_ADP_645_112_2_18_1_11_13_421,"amr_adp.c:AMR_Resume:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_653_112_2_18_1_11_13_422,"ERROR amr no memory 6.")
TRACE_MSG(AMR_ADP_680_112_2_18_1_11_13_423,"amr_adp.c:AMR_Seek:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_688_112_2_18_1_11_13_424,"AMR_Seek ERROR not support seek operation in this play type:%d.")
TRACE_MSG(AMR_ADP_696_112_2_18_1_11_13_425,"ERROR amr no memory 7.")
TRACE_MSG(AMR_ADP_722_112_2_18_1_11_13_426,"AMR_SendStreamData send stream data to codec task!")
TRACE_MSG(AMR_ADP_729_112_2_18_1_11_13_427,"AMR_SendStreamData:error:SCI_NULL == ptAudioObj")
TRACE_MSG(AMR_ADP_735_112_2_18_1_11_13_428,"AMR_SendStreamData AUDIO_DEFAULT_TASK_ID==ptAudioObj->tThreadId.")
TRACE_MSG(AMR_ADP_743_112_2_18_1_11_13_429,"AMR_SendStreamData ERROR amr no memory.")
TRACE_MSG(AMR_ADP_767_112_2_18_1_11_13_430,"amr_adp.c:AMR_GetPlayingInfo:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_812_112_2_18_1_11_13_431,"amr_adp.c[AMR_GetPlayingInfo] stream, curTime:%d, %d should not less than:%d(=%d+%d).")
TRACE_MSG(AMR_ADP_836_112_2_18_1_11_13_432,"amr_adp.c:AMR_IdentifyFormat:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_849_112_2_18_1_11_14_433,"AMR: SFS_Seek ERROR")
TRACE_MSG(AMR_ADP_860_112_2_18_1_11_14_434,"AMR: SFS_Read ERROR")
TRACE_MSG(AMR_ADP_871_112_2_18_1_11_14_435,"AMR: header data error.")
TRACE_MSG(AMR_ADP_885_112_2_18_1_11_14_436,"AMR: header data error.")
TRACE_MSG(AMR_ADP_897_112_2_18_1_11_14_437,"amr_adp.c:AMR_IdentifyFormat:error:Unknown Handle type!")
TRACE_MSG(AMR_ADP_910_112_2_18_1_11_14_438,"AMR: Header ERROR")
TRACE_MSG(AMR_ADP_926_112_2_18_1_11_14_439,"AMR_IdentifyFormat total time:%d s")
TRACE_MSG(AMR_ADP_996_112_2_18_1_11_14_440,"AMR_Thread_Entry AMR_PLAY!")
TRACE_MSG(AMR_ADP_1051_112_2_18_1_11_14_441,"AMR_Thread_Entry AMR_PAUSE!")
TRACE_MSG(AMR_ADP_1059_112_2_18_1_11_14_442,"AMR_Thread_Entry AMR_RESUME!")
TRACE_MSG(AMR_ADP_1077_112_2_18_1_11_14_443,"AMR_Thread_Entry AMR_DECODE!")
TRACE_MSG(AMR_ADP_1101_112_2_18_1_11_14_444,"AMR_Thread_Entry stream:End of music!")
TRACE_MSG(AMR_ADP_1108_112_2_18_1_11_14_445,"AMR_Thread_Entry wrong frame length. %d.")
TRACE_MSG(AMR_ADP_1112_112_2_18_1_11_14_446,"AMR_Thread_Entry wrong frame. length:%d, should be: %d.")
TRACE_MSG(AMR_ADP_1125_112_2_18_1_11_14_447,"AMR_Thread_Entry get_stream_data return AUDIO_STREAM_GET_SRC_WAIT!")
TRACE_MSG(AMR_ADP_1131_112_2_18_1_11_14_448,"AMR_Thread_Entry get_stream_data return AUDIO_STREAM_GET_SRC_ASY!")
TRACE_MSG(AMR_ADP_1137_112_2_18_1_11_14_449,"AMR_Thread_Entry get_stream_data return AUDIO_STREAM_GET_SRC_ERROR!")
TRACE_MSG(AMR_ADP_1143_112_2_18_1_11_14_450,"AMR_Thread_Entry get_stream_data return result:%d!wrong type!")
TRACE_MSG(AMR_ADP_1158_112_2_18_1_11_14_451,"AMR: stream buffer count > %d!")
TRACE_MSG(AMR_ADP_1206_112_2_18_1_11_14_452,"AMR: Buffer > %d!")
TRACE_MSG(AMR_ADP_1236_112_2_18_1_11_14_453,"AMR: while_count: %d")
TRACE_MSG(AMR_ADP_1258_112_2_18_1_11_14_454,"AMR_Thread_Entry AMR_SENDSTREAMDATA!")
TRACE_MSG(AMR_ADP_1262_112_2_18_1_11_14_455,"AMR_Thread_Entry wrong frame length. %d.")
TRACE_MSG(AMR_ADP_1266_112_2_18_1_11_14_456,"AMR_Thread_Entry wrong frame. length:%d, should be: %d.")
TRACE_MSG(AMR_ADP_1285_112_2_18_1_11_14_457,"AMR_Thread_Entry AMR_STOP!")
TRACE_MSG(AMR_ADP_1295_112_2_18_1_11_14_458,"AMR_Thread_Entry AMR_SEEK result:%d, offset:%d!")
TRACE_MSG(AMR_ADP_1335_112_2_18_1_11_14_459,"amr_adp.c:AMR_SetPlayPoint:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_1356_112_2_18_1_11_14_460,"AMR: SFS_Seek ERROR")
TRACE_MSG(AMR_ADP_1372_112_2_18_1_11_15_461,"AMR: SFS_Seek ERROR if uiFirstFrameLength==0")
TRACE_MSG(AMR_ADP_1382_112_2_18_1_11_15_462,"AMR: SFS_ReadFile ERROR if uiFirstFrameLength==0")
TRACE_MSG(AMR_ADP_1395_112_2_18_1_11_15_463,"AMR: SFS_Seek ERROR")
TRACE_MSG(AMR_ADP_1405_112_2_18_1_11_15_464,"AMR: SFS_ReadFile ERROR")
TRACE_MSG(AMR_ADP_1411_112_2_18_1_11_15_465,"AMR:  bVbr is SCI_TRUE")
TRACE_MSG(AMR_ADP_1419_112_2_18_1_11_15_466,"AMR: SFS_Seek ERROR again!")
TRACE_MSG(AMR_ADP_1481_112_2_18_1_11_15_467,"amr_adp.c:AMR_FileParseOneFrame:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_1494_112_2_18_1_11_15_468,"AMR_FileParseOneFrameLength SFS_GetFilePointer ERROR.")
TRACE_MSG(AMR_ADP_1504_112_2_18_1_11_15_469,"AMR_FileParseOneFrameLength SFS_ReadFile ERROR.")
TRACE_MSG(AMR_ADP_1510_112_2_18_1_11_15_470,"AMR_FileParseOneFrameLength SFS_ReadFile frame_header %d, %d.")
TRACE_MSG(AMR_ADP_1518_112_2_18_1_11_15_471,"AMR_FileParseOneFrameLength SFS_SetFilePointer ERROR.")
TRACE_MSG(AMR_ADP_1550_112_2_18_1_11_15_472,"amr_adp.c:AMR_FileParseOneFrame:error:PNULL == ptAudioObject")
TRACE_MSG(AMR_ADP_1564_112_2_18_1_11_15_473,"AMR_FileParseOneFrame SFS_ReadFile ERROR.")
TRACE_MSG(AMR_ADP_1574_112_2_18_1_11_15_474,"AMR_FileParseOneFrame SFS_ReadFile ERROR.")
TRACE_MSG(AMR_DSP_ADP_463_112_2_18_1_11_17_475,"_AMR_DSP_DecOutputBuf AUDIO_DP_WriteRingBuffer l:0x%x, r:0x%x, len:%d(word).")
TRACE_MSG(AMR_DSP_ADP_547_112_2_18_1_11_17_476,"AMR_DSP_DECODE_FRAME SENDER:0x%x receiver:0x%x.")
TRACE_MSG(AMR_DSP_ADP_665_112_2_18_1_11_17_477,"_AMR_DSP_SendSig: SEND A UNKOWNED SIG:%d.")
TRACE_MSG(AMR_DSP_ADP_717_112_2_18_1_11_17_478,"_AMR_DSP_GetFirstFrameLength: SFS_Seek ERROR")
TRACE_MSG(AMR_DSP_ADP_728_112_2_18_1_11_17_479,"_AMR_DSP_GetFirstFrameLength: SFS_ReadFile ERROR")
TRACE_MSG(AMR_DSP_ADP_735_112_2_18_1_11_17_480,"_AMR_DSP_GetFirstFrameLength: SFS_ReadFile len error:%d, %d.")
TRACE_MSG(AMR_DSP_ADP_743_112_2_18_1_11_17_481,"_AMR_DSP_GetFirstFrameLength: src data len is %d.")
TRACE_MSG(AMR_DSP_ADP_779_112_2_18_1_11_17_482,"_AMR_DSP_SetPlayPoint failed to seek in vbr.")
TRACE_MSG(AMR_DSP_ADP_796_112_2_18_1_11_17_483,"AMR: SFS_Seek ERROR")
TRACE_MSG(AMR_DSP_ADP_803_112_2_18_1_11_17_484,"_AMR_DSP_SetPlayPoint failed to seek offset:%d, total:%d.")
TRACE_MSG(AMR_DSP_ADP_819_112_2_18_1_11_17_485,"_AMR_DSP_SetPlayPoint SFS_GetFilePointer: ERROR")
TRACE_MSG(AMR_DSP_ADP_828_112_2_18_1_11_17_486,"_AMR_DSP_SetPlayPoint SFS_SetFilePointer: SFS_Seek ERROR")
TRACE_MSG(AMR_DSP_ADP_839_112_2_18_1_11_17_487,"_AMR_DSP_SetPlayPoint SFS_ReadFile")
TRACE_MSG(AMR_DSP_ADP_846_112_2_18_1_11_18_488,"_AMR_DSP_SetPlayPoint vbr.")
TRACE_MSG(AMR_DSP_ADP_852_112_2_18_1_11_18_489,"_AMR_DSP_SetPlayPoint SFS_SetFilePointer: SFS_Seek ERROR")
TRACE_MSG(AMR_DSP_ADP_863_112_2_18_1_11_18_490,"_AMR_DSP_SetPlayPoint SFS_SetFilePointer: SFS_Seek ERROR")
TRACE_MSG(AMR_DSP_ADP_896_112_2_18_1_11_18_491,"_AMR_DSP_SetPlayPoint vbr.")
TRACE_MSG(AMR_DSP_ADP_978_112_2_18_1_11_18_492,"_AMR_DSP_GetStreamDemuxSrcData stream:End of music!")
TRACE_MSG(AMR_DSP_ADP_993_112_2_18_1_11_18_493,"amr_dsp_adp.c GetStreamDemuxSrcData wait!")
TRACE_MSG(AMR_DSP_ADP_999_112_2_18_1_11_18_494,"amr_dsp_adp.c GetStreamDemuxSrcData _ASY!")
TRACE_MSG(AMR_DSP_ADP_1004_112_2_18_1_11_18_495,"amr_dsp_adp.c GetStreamDemuxSrcData ERROR!")
TRACE_MSG(AMR_DSP_ADP_1009_112_2_18_1_11_18_496,"amr_dsp_adp.c get_stream_data return result:%d!wrong type!")
TRACE_MSG(AMR_DSP_ADP_1154_112_2_18_1_11_18_497,"_AMR_DSP_WriteDemuxFrame SFS_ReadFile ERROR.")
TRACE_MSG(AMR_DSP_ADP_1266_112_2_18_1_11_18_498,"_AMR_DSP_WriteDemuxFrame some unnormal 1.")
TRACE_MSG(AMR_DSP_ADP_1279_112_2_18_1_11_18_499,"_AMR_DSP_WriteDemuxFrame some unnormal 2.")
TRACE_MSG(AMR_DSP_ADP_1423_112_2_18_1_11_19_500,"SET bIsDspReq FALSE 0")
TRACE_MSG(AMR_DSP_ADP_1448_112_2_18_1_11_19_501,"_AMR_DSP_Decode_Trig_By_SBC_Callback isStop is TRUE.")
TRACE_MSG(AMR_DSP_ADP_1455_112_2_18_1_11_19_502,"_AMR_DSP_Decode_Trig_By_SBC_Callback eDecodeTaskState:%d.")
TRACE_MSG(AMR_DSP_ADP_1461_112_2_18_1_11_19_503,"SET bIsDspReq FALSE 1")
TRACE_MSG(AMR_DSP_ADP_1514_112_2_18_1_11_19_504,"SET bIsDspReq FALSE 2")
TRACE_MSG(AMR_DSP_ADP_1570_112_2_18_1_11_19_505,"SET bIsDspReq TRUE 4")
TRACE_MSG(AMR_DSP_ADP_1602_112_2_18_1_11_19_506,"SET bIsDspReq TRUE 5")
TRACE_MSG(AMR_DSP_ADP_1701_112_2_18_1_11_19_507,"_AMR_DSP_TransFrame bIsDspReq is true, unnormal.")
TRACE_MSG(AMR_DSP_ADP_1705_112_2_18_1_11_19_508,"SET bIsDspReq TRUE 6")
TRACE_MSG(AMR_DSP_ADP_1707_112_2_18_1_11_19_509,"_AMR_DSP_TransFrame start decode time:0x%x.")
TRACE_MSG(AMR_DSP_ADP_1732_112_2_18_1_11_19_510,"_AMR_DSP_DecodeFrame decode end!")
TRACE_MSG(AMR_DSP_ADP_1736_112_2_18_1_11_19_511,"_AMR_DSP_DecodeFrame curFrameCount:%d.")
TRACE_MSG(AMR_DSP_ADP_1772_112_2_18_1_11_19_512,"_AMR_DSP_DecodeFrame _AMR_DSP_TransFrame frameCount:%d.")
TRACE_MSG(AMR_DSP_ADP_1814_112_2_18_1_11_19_513,"_AMR_DSP_DecodeFrame _AMR_DSP_SetFrameData frameCount:%d.")
TRACE_MSG(AMR_DSP_ADP_1845_112_2_18_1_11_19_514,"_AMR_DSP_DecodeFrame _AMR_DSP_TransFrame frameCount:%d, offset:%d.")
TRACE_MSG(AMR_DSP_ADP_1888_112_2_18_1_11_20_515,"amr_dsp_adp.c AMR_DSP_Construct allocLen:%d*4=%d.")
TRACE_MSG(AMR_DSP_ADP_1903_112_2_18_1_11_20_516,"amr_dsp_adp.c AMR_DSP_Construct allocLen:%d.")
TRACE_MSG(AMR_DSP_ADP_1909_112_2_18_1_11_20_517,"amr_dsp_adp.c AMR_DSP_Construct allocLen:%d.")
TRACE_MSG(AMR_DSP_ADP_1915_112_2_18_1_11_20_518,"pusPcmDataL:0x%x, pusPcmDataR:0x%x, len:%d(bytes).")
TRACE_MSG(AMR_DSP_ADP_1921_112_2_18_1_11_20_519,"AMR_DSP_Construct alloc pusPcmData failed. size:%d.")
TRACE_MSG(AMR_DSP_ADP_1934_112_2_18_1_11_20_520,"amr_dsp_adp.c AMR_DSP_Construct allocLen:%d.")
TRACE_MSG(AMR_DSP_ADP_1945_112_2_18_1_11_20_521,"AMR_DSP_Construct alloc pucDemuxSrcData failed. size:%d.")
TRACE_MSG(AMR_DSP_ADP_1965_112_2_18_1_11_20_522,"AMR_DSP_Construct alloc other_info_ptr failed. size:%d.")
TRACE_MSG(AMR_DSP_ADP_2004_112_2_18_1_11_20_523,"[AMR_DSP_CODEC]AMR_DSP_Play, ThreadID:0x%x,0x%x")
TRACE_MSG(AMR_DSP_ADP_2409_112_2_18_1_11_21_524,"AMR_DSP_Seek ERROR not support seek operation in this play type:%d.")
TRACE_MSG(AMR_DSP_ADP_2447_112_2_18_1_11_21_525,"AMR_DSP_IdentifyAudioFormat: SFS_Seek ERROR")
TRACE_MSG(AMR_DSP_ADP_2459_112_2_18_1_11_21_526,"AMR_DSP_IdentifyAudioFormat: SFS_Read ERROR")
TRACE_MSG(AMR_DSP_ADP_2465_112_2_18_1_11_21_527,"AMR_DSP_IdentifyAudioFormat: header data error.")
TRACE_MSG(AMR_DSP_ADP_2476_112_2_18_1_11_21_528,"AMR_DSP_IdentifyAudioFormat: header data error.")
TRACE_MSG(AMR_DSP_ADP_2490_112_2_18_1_11_21_529,"AMR_DSP_IdentifyAudioFormat: header data error.")
TRACE_MSG(AMR_DSP_ADP_2513_112_2_18_1_11_21_530,"AMR_DSP_IdentifyAudioFormat: Header ERROR")
TRACE_MSG(AMR_DSP_ADP_2529_112_2_18_1_11_21_531,"AMR_DSP_IdentifyAudioFormat FirstFrameLen is zero.")
TRACE_MSG(AMR_DSP_ADP_2535_112_2_18_1_11_21_532,"AMR_IdentifyFormat total time:%d s")
TRACE_MSG(AMR_DSP_ADP_2575_112_2_18_1_11_21_533,"AMR_DSP_GetPlayingInfo ERROR not support this operation in this play type:%d.")
TRACE_MSG(AMR_DSP_ADP_2592_112_2_18_1_11_21_534,"AMR_DSP_GetPlayingInfo: read:%d, other:%d, src_buf:%d, src_to_dsp:%d, src_to_decode:%d, decode:%d.")
TRACE_MSG(AMR_DSP_ADP_2713_112_2_18_1_11_21_535,"_AMR_DSP_Thread_Entry AMR_DSP_DECODE_PLAY")
TRACE_MSG(AMR_DSP_ADP_2718_112_2_18_1_11_21_536,"SET bIsDspReq TRUE 7")
TRACE_MSG(AMR_DSP_ADP_2726_112_2_18_1_11_21_537,"[AMR_DSP]DSP DECODE INIT DONE result:%d")
TRACE_MSG(AMR_DSP_ADP_2730_112_2_18_1_11_21_538,"SET bIsDspReq 8")
TRACE_MSG(AMR_DSP_ADP_2761_112_2_18_1_11_21_539,"_AMR_DSP_Thread_Entry AMR_DSP_DECODE_FRAME decode end!")
TRACE_MSG(AMR_DSP_ADP_2775_112_2_18_1_11_21_540,"_AMR_DSP_Thread_Entry Wait time is too long to Get frame!")
TRACE_MSG(AMR_DSP_ADP_2829_112_2_18_1_11_21_541,"_AMR_DSP_Thread_Entry AMR_DSP_DECODE_FRAME_IND decode end!")
TRACE_MSG(AMR_DSP_ADP_2886_112_2_18_1_11_21_542,"_AMR_DSP_Thread_Entry AMR_DSP_DECODE_FRAME_IND decode end!")
TRACE_MSG(AMR_DSP_ADP_2930_112_2_18_1_11_22_543,"[AMR_DSP]PAUSE SUCCESS, waitCount:%d, MAX:%d!")
TRACE_MSG(AMR_DSP_ADP_2939_112_2_18_1_11_22_544,"[AMR_DSP]PAUSE BUT NOW DEMUX TASK IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN")
TRACE_MSG(AMR_DSP_ADP_2949_112_2_18_1_11_22_545,"[AMR_DSP]FORCE PAUSE SUCCESS DEMUX NOT STOP!")
TRACE_MSG(AMR_DSP_ADP_2960_112_2_18_1_11_22_546,"[AMR_DSP]PAUSE BUT NOW DSP IS DECODE, WAIT SOME TIME THEN TRY AGAIN")
TRACE_MSG(AMR_DSP_ADP_2970_112_2_18_1_11_22_547,"[AMR_DSP]FORCE PAUSE SUCCESS!")
TRACE_MSG(AMR_DSP_ADP_2993_112_2_18_1_11_22_548,"[AMR_DSP]STOP SUCCESS, waitCount:%d, maxCount:%d!")
TRACE_MSG(AMR_DSP_ADP_3003_112_2_18_1_11_22_549,"[AMR_DSP]STOP BUT NOW DEMUX TASK IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN")
TRACE_MSG(AMR_DSP_ADP_3016_112_2_18_1_11_22_550,"[AMR_DSP]FORCE STOP, waitCount:%d, maxCount:%d!")
TRACE_MSG(AMR_DSP_ADP_3028_112_2_18_1_11_22_551,"[AMR_DSP]STOP BUT NOW DSP IS DECODE, WAIT SOME TIME THEN TRY AGAIN")
TRACE_MSG(AMR_DSP_ADP_3040_112_2_18_1_11_22_552,"[AMR_DSP]FORCE STOP SUCCESS!")
TRACE_MSG(AMR_DSP_ADP_3240_112_2_18_1_11_22_553,"[AMR_DSP]:seek result:%d, offset:%d.")
TRACE_MSG(AMR_DSP_ADP_3269_112_2_18_1_11_22_554,"[AMR_DSP]:DEMUX THREAD RECV A BAD SIGNAL:%d, DISCARD IT")
TRACE_MSG(BASEBAND_AMRCODEC_133_112_2_18_1_11_23_555,"AMR frame len %d: %s")
TRACE_MSG(BASEBAND_AMRCODEC_269_112_2_18_1_11_23_556,"WriteFrameToFifo(), fifo overflow")
TRACE_MSG(BASEBAND_AMRCODEC_303_112_2_18_1_11_23_557,"SendDataFromFifoToDevice(), is empty")
TRACE_MSG(BASEBAND_AMRCODEC_435_112_2_18_1_11_24_558,"[AMR] Error:Dev->Fifo, len[%d]")
TRACE_MSG(BASEBAND_AMRCODEC_476_112_2_18_1_11_24_559,"SendFrameFromDeviceToFifo(), fifo overflow")
TRACE_MSG(BASEBAND_AMRCODEC_693_112_2_18_1_11_24_560,"BBAMR_ReadAFrame(), error happen when wait for event")
TRACE_MSG(BASEBAND_AMRCODEC_700_112_2_18_1_11_24_561,"BBAMR_ReadAFrame(), encount exit event")
TRACE_MSG(BASEBAND_AMRCODEC_787_112_2_18_1_11_24_562,"BBAMR_OpenDecoder(), failed to create event")
TRACE_MSG(BASEBAND_AMRCODEC_822_112_2_18_1_11_24_563,"BBAMR_SetAmrPackMode()%d , done1")
TRACE_MSG(BASEBAND_AMRCODEC_849_112_2_18_1_11_24_564,"BBAMR_CloseDecoder Start")
TRACE_MSG(BASEBAND_AMRCODEC_863_112_2_18_1_11_24_565,"BBAMR_CloseDecoder End")
TRACE_MSG(BASEBAND_AMRCODEC_870_112_2_18_1_11_24_566,"BBAMR_CloseDecoder(), done")
TRACE_MSG(BASEBAND_AMRCODEC_892_112_2_18_1_11_24_567,"BBAMR_WriteAFrame(), error happen when wait for event")
TRACE_MSG(BASEBAND_AMRCODEC_898_112_2_18_1_11_24_568,"BBAMR_WriteAFrame(), encount exit event")
TRACE_MSG(BASEBAND_AMRCODEC_1119_112_2_18_1_11_25_569,"AP_VT_AMR_OpenDecoder()%d , done1")
TRACE_MSG(BASEBAND_AMRCODEC_1170_112_2_18_1_11_25_570,"AP_VT_AMR_OpenDecoder()%d , done1")
TRACE_MSG(ARM_AD_ADP_112_112_2_18_1_11_26_571,"_Arm_Ad_OfferCallback, error. hAudioHandle:0x%08x.")
TRACE_MSG(ARM_AD_ADP_208_112_2_18_1_11_26_572,"arm_ad_adp.c:_Arm_Ad_Play:error:PNULL == s_record_device->tDe")
TRACE_MSG(ARM_AD_ADP_260_112_2_18_1_11_26_573,"arm_ad_adp.c:_Arm_Ad_IdentifyFormat:error:audio_obj == SCI_NULL")
TRACE_MSG(AUD_DECODE_172_112_2_18_1_11_27_574,"AUD_decode.c:AUD_Decode_Process:aud_empty_frame_put sleep(50)")
TRACE_MSG(AUD_DECODE_184_112_2_18_1_11_27_575,"AUD_decode.c:AUD_Decode_Process:aud_empty_frame_put sleep(50)")
TRACE_MSG(AUD_DECODE_198_112_2_18_1_11_27_576,"AUD_decode.c:AUD_Decode_Process:aud_empty_frame_put sleep(50)")
TRACE_MSG(AUD_DECODE_213_112_2_18_1_11_27_577,"AUD_decode.c:AUD_Decode_Process:aud_empty_frame_put sleep(50)")
TRACE_MSG(AUD_DECODE_242_112_2_18_1_11_27_578,"AUD_decode.c:AUD_Decode_Process:aud_valid_frame_get sleep(100)")
TRACE_MSG(AUD_DECODE_253_112_2_18_1_11_27_579,"AUD_decode.c:AUD_Decode_Open in")
TRACE_MSG(AUD_DECODE_256_112_2_18_1_11_27_580,"AUD_decode.c:AUD_Decode_Open:(!param)||(NULL==*param))")
TRACE_MSG(AUD_DECODE_263_112_2_18_1_11_27_581,"AUD_decode.c:AUD_Decode_Open priority is %d")
TRACE_MSG(AUD_DECODE_268_112_2_18_1_11_27_582,"AUD_decode.c:AUD_Decode_Open:decode is null")
TRACE_MSG(AUD_DECODE_281_112_2_18_1_11_27_583,"AUD_decode.c:AUD_Decode_Open:frame_buffer is null")
TRACE_MSG(AUD_DECODE_304_112_2_18_1_11_27_584,"AUD_decode.c:AUD_Decode, AUD_Decode_Open, failed to create event")
TRACE_MSG(AUD_DECODE_335_112_2_18_1_11_27_585,"AUD_decode.c:AUD_Decode_Close in")
TRACE_MSG(AUD_DECODE_357_112_2_18_1_11_27_586,"AUD_decode.c:AUD_Decode_start in")
TRACE_MSG(AUD_DECODE_366_112_2_18_1_11_27_587,"AUD_decode.c:AUD_Decode_start ok")
TRACE_MSG(AUD_DECODE_373_112_2_18_1_11_27_588,"aud_decode.c:aud_decode_start: result is %d")
TRACE_MSG(AUD_DECODE_386_112_2_18_1_11_27_589,"AUD_decode.c:AUD_Decode_stop in")
TRACE_MSG(AUD_DECODE_397_112_2_18_1_11_27_590,"aud_decode.c:aud_decode_stop:result is %d")
TRACE_MSG(AUD_DECODE_405_112_2_18_1_11_27_591,"AUD_decode.c:AUD_Decode_stop out ok")
TRACE_MSG(AUD_DECODE_409_112_2_18_1_11_27_592,"AUD_decode.c:AUD_Decode_stop out failed")
TRACE_MSG(AUD_DECODE_420_112_2_18_1_11_27_593,"AUD_decode.c:AUD_Decode_pause in")
TRACE_MSG(AUD_DECODE_431_112_2_18_1_11_27_594,"AUD_decode.c:AUD_Decode_pause result is %d ")
TRACE_MSG(AUD_DECODE_439_112_2_18_1_11_27_595,"AUD_decode.c:AUD_Decode_pause out ok")
TRACE_MSG(AUD_DECODE_444_112_2_18_1_11_27_596,"AUD_decode.c:AUD_Decode_pause out failed")
TRACE_MSG(AUD_DECODE_471_112_2_18_1_11_27_597,"amr_decode.c:AMR_Decode_Clean out")
TRACE_MSG(AUD_DECODE_536_112_2_18_1_11_27_598,"aud_decode.c:AudioOut_AllocBuffer: result %d")
TRACE_MSG(AUD_DECODE_585_112_2_18_1_11_27_599,"AUD_decode.c:CTRL_PAUSE in")
TRACE_MSG(AUD_DECODE_588_112_2_18_1_11_28_600,"AUD_decode.c:AUD_DECODE_PAUSE in")
TRACE_MSG(AUD_DECODE_595_112_2_18_1_11_28_601,"aud_decode.c:AudioOut_FreeBuffer: result %d")
TRACE_MSG(AUD_DECODE_599_112_2_18_1_11_28_602,"AUD_decode.c:AUD_DECODE_PAUSE out")
TRACE_MSG(AUD_DECODE_604_112_2_18_1_11_28_603,"aud_decode.c : CTRL_STOP received")
TRACE_MSG(AUD_DECODE_607_112_2_18_1_11_28_604,"aud_decode.c: CTRL_STOP received and AUDIO_SM_PlayEnd")
TRACE_MSG(AUD_DEMUX_77_112_2_18_1_11_28_605,"arm_demux.c:aud_framebuffer_is_empty: file end but seek again")
TRACE_MSG(AUD_DEMUX_84_112_2_18_1_11_28_606,"arm_demux.c:aud_framebuffer_is_empty: demux_stop call back")
TRACE_MSG(AUD_DEMUX_95_112_2_18_1_11_28_607,"MP3_demux.c:aud_empty_frame_get:MP3_frame is null")
TRACE_MSG(AUD_DEMUX_146_112_2_18_1_11_28_608,"  demux.c:Demux_Process: aud_valid_frame_put result is %dl and SCI_Sleep(20)")
TRACE_MSG(AUD_DEMUX_180_112_2_18_1_11_28_609,"  demux.c:Seek_Process:stream_seek failed result is %d")
TRACE_MSG(AUD_DEMUX_210_112_2_18_1_11_28_610,"  demux.c:  Demux_Open in")
TRACE_MSG(AUD_DEMUX_214_112_2_18_1_11_28_611,"  demux.c:  Demux_Open(!param)||(NULL==*param)")
TRACE_MSG(AUD_DEMUX_221_112_2_18_1_11_28_612,"  decode.c:  Decode_Open priority is %d")
TRACE_MSG(AUD_DEMUX_229_112_2_18_1_11_28_613,"  demux.c:demux is null")
TRACE_MSG(AUD_DEMUX_239_112_2_18_1_11_28_614,"  demux.c:frame_buffer is null")
TRACE_MSG(AUD_DEMUX_255_112_2_18_1_11_28_615,"  demux.c:stream_new failed")
TRACE_MSG(AUD_DEMUX_261_112_2_18_1_11_28_616,"  demux.c:  Demux_Open:demux->p_event is %d")
TRACE_MSG(AUD_DEMUX_264_112_2_18_1_11_28_617,"  Demux,   Demux_Open, failed to create event")
TRACE_MSG(AUD_DEMUX_291_112_2_18_1_11_28_618,"  demux.c:  Demux_Close in")
TRACE_MSG(AUD_DEMUX_298_112_2_18_1_11_28_619,"  demux.c:  Demux_Close:demux->p_event is %d")
TRACE_MSG(AUD_DEMUX_310_112_2_18_1_11_28_620,"  demux.c:  Demux_start in")
TRACE_MSG(AUD_DEMUX_329_112_2_18_1_11_28_621,"  demux.c:  Demux_stop in")
TRACE_MSG(AUD_DEMUX_350_112_2_18_1_11_28_622,"  demux.c:  Demux_stop out")
TRACE_MSG(AUD_DEMUX_354_112_2_18_1_11_28_623,"  demux.c:  Demux_stop out error")
TRACE_MSG(AUD_DEMUX_364_112_2_18_1_11_28_624,"amr_demux.c:AMR_Demux_seek in offset type is %d, value  is %d")
TRACE_MSG(AUD_DEMUX_439_112_2_18_1_11_29_625,"  demux.c:Demux_Process: frame is null and SCI_Sleep(20)")
TRACE_MSG(AUD_ENCODE_157_112_2_18_1_11_29_626,"aud_encode.c:aud_encode_DumpRecordData aud_empty_frame_get error : reuslt is %d")
TRACE_MSG(AUD_ENCODE_162_112_2_18_1_11_29_627,"aud_encode.c:  aud_encode_DumpRecordData: dec_common->encode error: result is %d")
TRACE_MSG(AUD_ENCODE_191_112_2_18_1_11_29_628,"aud_encode.c: mux_one_frame result is error %d")
TRACE_MSG(AUD_ENCODE_221_112_2_18_1_11_29_629,"AUD_Encode_Process aud_empty_frame_get error : reuslt is %d")
TRACE_MSG(AUD_ENCODE_231_112_2_18_1_11_29_630,"AUD_Encode_Process aud_valid_frame_get error : reuslt is %d")
TRACE_MSG(AUD_ENCODE_243_112_2_18_1_11_29_631,"aud_encode.c:   dec_common->encode error: result is %d")
TRACE_MSG(AUD_ENCODE_281_112_2_18_1_11_29_632,"AUD_Encode_Process aud_valid_frame_get error : reuslt is %d")
TRACE_MSG(AUD_ENCODE_295_112_2_18_1_11_29_633,"aud_encode.c:   dec_common->encode error: result is %d")
TRACE_MSG(AUD_ENCODE_326_112_2_18_1_11_29_634,"aud_encode.c:mux_one_frame: result error %d")
TRACE_MSG(AUD_ENCODE_349_112_2_18_1_11_29_635,"AUD_encode.c:AUD_Encode_Open in")
TRACE_MSG(AUD_ENCODE_352_112_2_18_1_11_29_636,"AUD_encode.c:AUD_Encode_Open:(!param)||(NULL==*param))")
TRACE_MSG(AUD_ENCODE_359_112_2_18_1_11_29_637,"AUD_encode.c:AUD_Encode_Open priority is %d")
TRACE_MSG(AUD_ENCODE_364_112_2_18_1_11_29_638,"AUD_encode.c:AUD_Encode_Open:encode is null")
TRACE_MSG(AUD_ENCODE_391_112_2_18_1_11_30_639,"AUD_encode.c:AUD_Encode, AUD_Encode_Open, failed to create event")
TRACE_MSG(AUD_ENCODE_430_112_2_18_1_11_30_640,"AUD_encode.c:AUD_Encode_Close in")
TRACE_MSG(AUD_ENCODE_457_112_2_18_1_11_30_641,"AUD_encode.c:AUD_Encode_start in")
TRACE_MSG(AUD_ENCODE_463_112_2_18_1_11_30_642,"AUD_encode.c:AUD_Encode_start ok")
TRACE_MSG(AUD_ENCODE_474_112_2_18_1_11_30_643,"AUD_encode.c:AUD_Encode_stop in")
TRACE_MSG(AUD_ENCODE_493_112_2_18_1_11_30_644,"AUD_encode.c:AUD_Encode_stop out ok")
TRACE_MSG(AUD_ENCODE_497_112_2_18_1_11_30_645,"AUD_encode.c:AUD_Encode_stop out failed")
TRACE_MSG(AUD_ENCODE_506_112_2_18_1_11_30_646,"AUD_encode.c:AUD_Encode_pause in")
TRACE_MSG(AUD_ENCODE_523_112_2_18_1_11_30_647,"AUD_encode.c:AUD_Encode_pause  wait")
TRACE_MSG(AUD_ENCODE_526_112_2_18_1_11_30_648,"AUD_encode.c:AUD_Encode_pause out ok")
TRACE_MSG(AUD_ENCODE_531_112_2_18_1_11_30_649,"AUD_encode.c:AUD_Encode_pause out failed")
TRACE_MSG(AUD_ENCODE_589_112_2_18_1_11_30_650,"amr_encode.c:AMR_Encode_Clean out")
TRACE_MSG(AUD_ENCODE_685_112_2_18_1_11_30_651,"aud_encode.c: mux_end_pf called")
TRACE_MSG(AUD_ENCODE_698_112_2_18_1_11_30_652,"aud_encode.c:AUD_Encode_Process: result is %d")
TRACE_MSG(AUD_ENCODE_715_112_2_18_1_11_30_653,"AUD_encode.c:CTRL_PAUSE in")
TRACE_MSG(AUD_ENCODE_725_112_2_18_1_11_30_654,"AUD_encode.c:AUD_ENCODE_PAUSE in thread ")
TRACE_MSG(AUD_ENCODE_736_112_2_18_1_11_30_655,"AUD_encode.c:AUD_ENCODE_PAUSE out thread")
TRACE_MSG(AUD_ENCODE_741_112_2_18_1_11_30_656,"aud_encode.c : CTRL_STOP received")
TRACE_MSG(AUD_ENCODE_754_112_2_18_1_11_31_657,"aud_encode.c: CTRL_STOP received and AUDIO_SM_PlayEnd")
TRACE_MSG(AUD_MUX_131_112_2_18_1_11_31_658,"  mux.c:  Mux_Open in")
TRACE_MSG(AUD_MUX_135_112_2_18_1_11_31_659,"  mux.c:  Mux_Open(!param)||(NULL==*param)")
TRACE_MSG(AUD_MUX_142_112_2_18_1_11_31_660,"  decode.c:  Decode_Open priority is %d")
TRACE_MSG(AUD_MUX_148_112_2_18_1_11_31_661,"  mux.c:mux is null")
TRACE_MSG(AUD_MUX_158_112_2_18_1_11_31_662,"  mux.c:frame_buffer is null")
TRACE_MSG(AUD_MUX_172_112_2_18_1_11_31_663,"  mux.c:  Mux_Open:mux->p_event is %d")
TRACE_MSG(AUD_MUX_175_112_2_18_1_11_31_664,"  Mux,   Mux_Open, failed to create event")
TRACE_MSG(AUD_MUX_203_112_2_18_1_11_31_665,"  mux.c:  Mux_Close in")
TRACE_MSG(AUD_MUX_210_112_2_18_1_11_31_666,"  mux.c:  Mux_Close:mux->p_event is %d")
TRACE_MSG(AUD_MUX_222_112_2_18_1_11_31_667,"  mux.c:  Mux_start in")
TRACE_MSG(AUD_MUX_241_112_2_18_1_11_31_668,"aud_mu.c:aud_mux_pause in")
TRACE_MSG(AUD_MUX_249_112_2_18_1_11_31_669,"aud_mux.c:aud_mux_pause out ok")
TRACE_MSG(AUD_MUX_254_112_2_18_1_11_31_670,"aud_mux.c:aud_mux_pause out failed")
TRACE_MSG(AUD_MUX_263_112_2_18_1_11_31_671,"  mux.c:  Mux_stop in")
TRACE_MSG(AUD_MUX_271_112_2_18_1_11_31_672,"  mux.c:  Mux_stop out")
TRACE_MSG(AUD_MUX_275_112_2_18_1_11_31_673,"  mux.c:  Mux_stop out error")
TRACE_MSG(AUD_MUX_356_112_2_18_1_11_32_674,"  mux.c:Mux_Process: result is %d")
TRACE_MSG(AUD_MUX_370_112_2_18_1_11_32_675,"aud_mux.c: mux_end_pf called")
TRACE_MSG(AUD_MUX_383_112_2_18_1_11_32_676,"aud_mux.c:CTRL_PAUSE in")
TRACE_MSG(AUD_MUX_386_112_2_18_1_11_32_677,"aud_mux.c:STATE_MUX_PAUSE in thread")
TRACE_MSG(AUD_MUX_389_112_2_18_1_11_32_678,"aud_mux.c:STATE_MUX_PAUSE out thread")
TRACE_MSG(AUD_MUX_394_112_2_18_1_11_32_679,"aud_mux.c : CTRL_STOP received")
TRACE_MSG(AUD_MUX_399_112_2_18_1_11_32_680,"aud_mux.c: CTRL_STOP received and AUDIO_SM_PlayEnd")
TRACE_MSG(AUDIO_INPUT_80_112_2_18_1_11_32_681,"audio_input.c: Audio_Input_DataOkNotify: end _call back")
TRACE_MSG(AUDIO_INPUT_111_112_2_18_1_11_32_682,"audio_input.c:Read_Layer1_Data_Callback:aud_empty_frame_get failed lost data  count is %d")
TRACE_MSG(AUDIO_INPUT_158_112_2_18_1_11_32_683,"audio_input.c:Read_Layer1_Data_Callback:aud_empty_frame_get failed 2")
TRACE_MSG(AUDIO_INPUT_204_112_2_18_1_11_32_684,"audio_output.c:AudioOut_Open:audio_out is null")
TRACE_MSG(AUDIO_INPUT_225_112_2_18_1_11_32_685,"audio_input.c:DSP_CODEC_Ext_StartUpLoadData:SCI_ALLOC_APP TEMP_FRAME_T failed")
TRACE_MSG(AUDIO_INPUT_236_112_2_18_1_11_32_686,"peter: audio_input.c: frame size is %d")
TRACE_MSG(AUDIO_INPUT_241_112_2_18_1_11_32_687,"audio_input.c:DSP_CODEC_Ext_StartUpLoadData:SCI_ALLOC_APP TEMP_FRAME_T buffer failed")
TRACE_MSG(AUDIO_INPUT_255_112_2_18_1_11_32_688,"audio_input.c:DSP_CODEC_Ext_StartUpLoadData:SCI_ALLOC_APP s_free_frame failed")
TRACE_MSG(AUDIO_INPUT_265_112_2_18_1_11_32_689,"audio_input.c:DSP_CODEC_Ext_StartUpLoadData:SCI_ALLOC_APP s_free_frame buffer failed")
TRACE_MSG(AUDIO_INPUT_298_112_2_18_1_11_32_690,"audio_input.c: Audio_Input_Start: in h_aud_input is %x")
TRACE_MSG(AUDIO_INPUT_316_112_2_18_1_11_32_691,"audio_input.c: Audio_Input_Stop: in h_aud_input is %x")
TRACE_MSG(AUDIO_OUTPUT_146_112_2_18_1_11_33_692,"audio_output.c:Aout_Buffer_Manager_Init:SCI_ALLOCA failed i is %d")
TRACE_MSG(AUDIO_OUTPUT_185_112_2_18_1_11_33_693,"aduio_output.c:AudioOut_Open in")
TRACE_MSG(AUDIO_OUTPUT_189_112_2_18_1_11_33_694,"audio_output.c:AudioOut_Open:audio_out is null")
TRACE_MSG(AUDIO_OUTPUT_228_112_2_18_1_11_33_695,"audio_output.c:AudioOut_FreeBuffer:audio_out is null")
TRACE_MSG(AUDIO_OUTPUT_256_112_2_18_1_11_34_696,"audio_output.c:AudioOut_AllocBuffer:audio_out is null")
TRACE_MSG(AUDIO_OUTPUT_295_112_2_18_1_11_34_697,"aduio_output.c:AudioOut_Close in")
TRACE_MSG(AUDIO_OUTPUT_347_112_2_18_1_11_34_698,"audio_output.c:AudioOut_Buffer_Get NULL")
TRACE_MSG(AUDIO_FRAMEBUFFER_38_112_2_18_1_11_34_699,"audio_framebuffer.c:Init_BufferQueue:buf_Q->frame is null")
TRACE_MSG(AUDIO_FRAMEBUFFER_44_112_2_18_1_11_34_700,"audio_framebuffer.c:Init_BufferQueue:buf_Q->buffer_header is null")
TRACE_MSG(AUDIO_FRAMEBUFFER_53_112_2_18_1_11_34_701,"audio_framebuffer.c:Init_BufferQueue:SCI_CreateMutex failed!")
TRACE_MSG(AUDIO_FRAMEBUFFER_61_112_2_18_1_11_34_702,"audio_framebuffer.c:Init_BufferQueue:SCI_CreateEvent failed!")
TRACE_MSG(AUDIO_FRAMEBUFFER_289_112_2_18_1_11_35_703,"audio_farmebuffer.c:Init_FrameQueue:Queue_ptr->Frame is null")
TRACE_MSG(AUDIO_FRAMEBUFFER_302_112_2_18_1_11_35_704,"audio_framebuffer.c:Init_FrameQueue:SCI_CreateEvent failed!")
TRACE_MSG(AUDIO_FRAMEBUFFER_558_112_2_18_1_11_35_705,"audio_framebuffer.c:aud_framebuffer_new frame_buffer is null")
TRACE_MSG(AUDIO_FRAMEBUFFER_570_112_2_18_1_11_35_706,"audio_framebuffer.c:Init_BufferQueue failed")
TRACE_MSG(AUDIO_FRAMEBUFFER_578_112_2_18_1_11_35_707,"audio_framebuffer.c:Init_FrameQueue failed")
TRACE_MSG(AUDIO_STREAM_97_112_2_18_1_11_36_708,"audio_stream.c:DataBuffer_Fill:SFS_ReadFile:error result is %d")
TRACE_MSG(AUDIO_STREAM_137_112_2_18_1_11_36_709,"audio_stream.c: buffer fill erororoororor")
TRACE_MSG(AUDIO_STREAM_148_112_2_18_1_11_36_710,"audio_stream.c: buffer_fill,result is %d")
TRACE_MSG(AUDIO_STREAM_494_112_2_18_1_11_37_711,"audio_stream.c:DataBuffer_Seek resource type is %d,offset is %d")
TRACE_MSG(AUDIO_STREAM_502_112_2_18_1_11_37_712,"audio_stream.c:SFS_SetFilePointer:result is %d")
TRACE_MSG(AUDIO_STREAM_577_112_2_18_1_11_37_713,"audio_stream.c:DataBuffer_GetLength:file_size is %d,length is %d")
TRACE_MSG(AUDIO_STREAM_588_112_2_18_1_11_37_714,"audio_stream.c:stream_get_length:DataBuffer_GetLength: is %d")
TRACE_MSG(AUDIO_STREAM_669_112_2_18_1_11_38_715,"audio_stream.c:stream_read:DataBuffer_Fill:stream is end")
TRACE_MSG(AUDIO_STREAM_805_112_2_18_1_11_38_716,"audio_stream.c: stream_peek erro3 result is %d")
TRACE_MSG(AUDIO_STREAM_813_112_2_18_1_11_38_717,"audio_stream.c: stream_offset_peek: peek out of range !!!!!")
TRACE_MSG(AUDIO_STREAM_935_112_2_18_1_11_38_718,"audio_stream.c: DataBuffer_Offset_Peek erro result is %d")
TRACE_MSG(AUDIO_STREAM_943_112_2_18_1_11_38_719,"audio_stream.c: stream_offset_peek: peek out of range !!!!!")
TRACE_MSG(AUD_DSP_INTERFACE_40_112_2_18_1_11_39_720,"aud_dsp_interface.c:mp3_decoder_decode: result is %d,pcm_samples is %d")
TRACE_MSG(AUD_DSP_MNG_439_112_2_18_1_11_41_721,"aud_dsp_mng.c:aac_dec_process: frame len error > 0x800 or decode error")
TRACE_MSG(AUD_DSP_MNG_474_112_2_18_1_11_41_722,"peter: AMR_WriteOneFrameToDsp no data................")
TRACE_MSG(AUD_DSP_MNG_904_112_2_18_1_11_42_723,"aud_dsp_mng.c:capability_check_and_set failed type is %d,aud_dsp_res_status is %d")
TRACE_MSG(AUD_DSP_MNG_1001_112_2_18_1_11_42_724,"aud_dsp_mng.c:capability_check_and_clean failed type is %d,aud_dsp_res_status is %d")
TRACE_MSG(AUD_DSP_MNG_1109_112_2_18_1_11_42_725,"aud_dsp_mng.c:aud_dsp_open in type is %d")
TRACE_MSG(AUD_DSP_MNG_1113_112_2_18_1_11_42_726,"aud_dsp_mng.c: aud_dsp_open: capability_check_and_set %d")
TRACE_MSG(AUD_DSP_MNG_1120_112_2_18_1_11_42_727,"aud_dsp_mng.c:aud_dsp_open failed  ")
TRACE_MSG(AUD_DSP_MNG_1129_112_2_18_1_11_42_728,"aud_dsp_mng.c:, Open(), aud_dsp_open to create event")
TRACE_MSG(AUD_DSP_MNG_1136_112_2_18_1_11_42_729,"aud_dsp_mng.c:, Open(), aud_dsp_open to SCI_CreateMutex")
TRACE_MSG(AUD_DSP_MNG_1147_112_2_18_1_11_42_730,"aud_dsp_mng.c:aud_dsp_open out type is %d")
TRACE_MSG(AUD_DSP_MNG_1179_112_2_18_1_11_42_731,"aud_dsp_mng.c:aud_dsp_closein handle is %d")
TRACE_MSG(AUD_DSP_MNG_1185_112_2_18_1_11_42_732,"aud_dsp_mng.c: aud_dsp_open: capability_check_and_set %d")
TRACE_MSG(AUD_DSP_MNG_1192_112_2_18_1_11_43_733,"aud_dsp_mng.c:aud_dsp_close  error  wait state is %d")
TRACE_MSG(AUD_DSP_MNG_1216_112_2_18_1_11_43_734,"aud_dsp_mng.c:aud_dsp_close: out")
TRACE_MSG(AUD_DSP_MNG_1218_112_2_18_1_11_43_735,"aud_dsp_mng.c:aud_dsp_close out handle is %d")
TRACE_MSG(AUD_DSP_MNG_1230_112_2_18_1_11_43_736,"aud_dsp_mng.c:aud_dsp_process in handle is %d")
TRACE_MSG(AUD_DSP_MNG_1250_112_2_18_1_11_43_737,"aud_dsp_mng.c:aud_dsp_process out handle is %d,type is %d")
TRACE_MSG(ARM_SOURCE_138_112_2_18_1_11_44_738,"arm_source.c :arm_source_open failed")
TRACE_MSG(DSP_SOURCE_129_112_2_18_1_11_45_739,"dsp_source.c :dsp_source_open failed")
TRACE_MSG(CAT_MVOICE_198_112_2_18_1_11_45_740,"[Cat_mvoice]: MicSample_GetMicdB get_db=%d")
TRACE_MSG(CAT_MVOICE_215_112_2_18_1_11_45_741,"[Cat_mvoice]: CatMv_MicSample_CheckIfEnd end_type=%d")
TRACE_MSG(CAT_MVOICE_226_112_2_18_1_11_45_742,"[Cat_mvoice]: CatMv_MicSample_CheckIfEnd cat_mic_process.pre_mute_count=%d")
TRACE_MSG(CAT_MVOICE_228_112_2_18_1_11_45_743,"[Cat_mvoice]: CatMv_MicSample_CheckIfEnd needed_time=%d,base_value=%d")
TRACE_MSG(CAT_MVOICE_243_112_2_18_1_11_45_744,"[Cat_mvoice]: CatMv_MicSample_CheckIfEnd get_db=%d,base_value=%d buffer_pos=%d")
TRACE_MSG(CAT_MVOICE_259_112_2_18_1_11_45_745,"[Cat_mvoice]: CatMv_MicSample_CheckIfEnd get_db=%d,base_value=%d,mute_count=%d")
TRACE_MSG(CAT_MVOICE_296_112_2_18_1_11_45_746,"[Cat_mvoice]: CatMv_MicSample_CheckIfEnd test_count=%d")
TRACE_MSG(CAT_MVOICE_328_112_2_18_1_11_45_747,"[Cat_mvoice]: CatMv_MicData_Validsize first_get_date=%d")
TRACE_MSG(CAT_MVOICE_368_112_2_18_1_11_46_748,"[Cat_mvoice]: GET MIC W=%d,R=%d,valid_size=%d ")
TRACE_MSG(CAT_MVOICE_543_112_2_18_1_11_46_749,"[Cat_mvoice]: CatMv_Process_Data enter")
TRACE_MSG(CAT_MVOICE_551_112_2_18_1_11_46_750,"[Cat_mvoice]: CatMv_Process_Data uiSrcCount=%d,  read_p=%d")
TRACE_MSG(CAT_MVOICE_558_112_2_18_1_11_46_751,"[Cat_mvoice]: CatMv_Process_Data ret_value=%d,valid_count=%d")
TRACE_MSG(CAT_MVOICE_636_112_2_18_1_11_46_752,"[Cat_mvoice]: CatMv_Process_Data scale=%d,cat_mic_process.mute_index=%d")
TRACE_MSG(CAT_MVOICE_642_112_2_18_1_11_46_753,"[Cat_mvoice]: CatMv_Process_Data ret_value=%d,cat_mic_process.valid_count=%d")
TRACE_MSG(CAT_MVOICE_766_112_2_18_1_11_46_754,"[Cat_mvoice]: MicSample_Play have opened")
TRACE_MSG(CAT_MVOICE_787_112_2_18_1_11_46_755,"[Cat_mvoice]: MicSample_Play!")
TRACE_MSG(CAT_MVOICE_817_112_2_18_1_11_46_756,"[Cat_mvoice]: MicSample_Stop have Closed")
TRACE_MSG(CAT_MVOICE_878_112_2_18_1_11_47_757,"[Cat_mvoice]: AUDIO_PM_RegCodecPlugger ")
TRACE_MSG(CAT_MVOICE_916_112_2_18_1_11_47_758,"[Cat_mvoice]: CatMv_Process_Data uiSrcCount=%d ")
TRACE_MSG(CAT_MVOICE_920_112_2_18_1_11_47_759,"[Cat_mvoice]: CatMv_Process_Data enter callback ")
TRACE_MSG(CAT_MVOICE_991_112_2_18_1_11_47_760,"[Cat_mvoice]: CatMv_Process *puiDestCount=%d, uiCurrentSamplerate=%d")
TRACE_MSG(CAT_MVOICE_1062_112_2_18_1_11_47_761,"[Cat_mvoice]: CatMv_RegExpOnOff s_catmv_expression_on_flag=%d")
TRACE_MSG(CUSTOM_TONE_ADP_190_112_2_18_1_11_50_762,"CUSTOM_TONE_Construct SCI_CreateMutex pTonePlayMutex failed.")
TRACE_MSG(CUSTOM_TONE_ADP_200_112_2_18_1_11_50_763,"CUSTOM_TONE_Construct SCI_CreateMutex pToneStopMutex failed.")
TRACE_MSG(CUSTOM_TONE_ADP_210_112_2_18_1_11_50_764,"CUSTOM_TONE_Construct SCI_CreateTimer failed.")
TRACE_MSG(CUSTOM_TONE_ADP_230_112_2_18_1_11_50_765,"custome_tone_adp.c:CUSTOM_TONE_IdentifyFormat:error:PNULL == ptAudioObject")
TRACE_MSG(CUSTOM_TONE_ADP_258_112_2_18_1_11_50_766,"custom_tone_adp.c: CUSTOM_TONE_Ext_Play other task has been playing")
TRACE_MSG(CUSTOM_TONE_ADP_266_112_2_18_1_11_50_767,"custom_tone_adp.c:CUSTOM_TONE_Ext_Play:error:PNULL == ptAudioObject")
TRACE_MSG(CUSTOM_TONE_ADP_272_112_2_18_1_11_50_768,"custom_tone_adp.c:CUSTOM_TONE_Ext_Play:error:0==ptPara->duration.")
TRACE_MSG(CUSTOM_TONE_ADP_278_112_2_18_1_11_50_769,"custom_tone_adp.c:CUSTOM_TONE_Ext_Play: is playing.")
TRACE_MSG(CUSTOM_TONE_ADP_296_112_2_18_1_11_50_770,"[CUSTOM_TONE_Ext_Play] AUDIO_DM_Open failed!")
TRACE_MSG(CUSTOM_TONE_ADP_304_112_2_18_1_11_50_771,"custom_tone_adp.c: CUSTOM_TONE_Ext_Play other task has been playing")
TRACE_MSG(CUSTOM_TONE_ADP_345_112_2_18_1_11_50_772,"custome_tone_adp.c:CUSTOM_TONE_Ext_Stop:error:uiArgc != 0")
TRACE_MSG(CUSTOM_TONE_ADP_351_112_2_18_1_11_50_773,"custom_tone_adp.c: CUSTOM_TONE_Ext_Stop other task has been stopping")
TRACE_MSG(CUSTOM_TONE_ADP_356_112_2_18_1_11_50_774,"custom_tone_adp.c:CUSTOM_TONE_Ext_Stop: is not playing.")
TRACE_MSG(CUSTOM_TONE_ADP_365_112_2_18_1_11_50_775,"custom_tone_adp.c: CUSTOM_TONE_Ext_Stop the tone is never played.")
TRACE_MSG(CUSTOM_TONE_ADP_374_112_2_18_1_11_50_776,"custome_tone_adp.c:CUSTOM_TONE_Ext_Stop:error:PNULL == ptAudioObject")
TRACE_MSG(CUSTOM_TONE_ADP_395_112_2_18_1_11_50_777,"%s, %d. CUSTOM_TONE_Stop")
TRACE_MSG(CUSTOM_TONE_ADP_407_112_2_18_1_11_50_778,"custom_tone_adp.c:CUSTOM_TONE_Stop:error:PNULL == ptAudioObject")
TRACE_MSG(CUSTOM_TONE_ADP_414_112_2_18_1_11_50_779,"custom_tone_adp.c: CUSTOM_TONE_Stop other task has been stopping")
TRACE_MSG(CUSTOM_TONE_ADP_419_112_2_18_1_11_50_780,"custom_tone_adp.c:CUSTOM_TONE_Stop: is not playing.")
TRACE_MSG(CUSTOM_TONE_ADP_445_112_2_18_1_11_51_781,"custom_tone_adp.c: AUD_CustomToneStop other task has been stopping")
TRACE_MSG(AMR_DECODE_369_112_2_18_1_12_31_782,"mp3_decode:mp3_decode_open failed")
TRACE_MSG(AMR_DECODE_392_112_2_18_1_12_31_783,"amr_decode.c:AMR_Decode_Open:decode->serial_buffer is null")
TRACE_MSG(AMR_DSP_DECODE_192_112_2_18_1_12_32_784,"amr_dsp_decode:amr_decode_open failed")
TRACE_MSG(MP3_ARM_DECODE_67_112_2_18_1_12_32_785,"mp3_decode:mp3_decode_open failed")
TRACE_MSG(MP3_DSP_DECODE_27_112_2_18_1_12_32_786,"mp3_decode_peter.c:mp3_decode_init:mp3_decoder_open:failed")
TRACE_MSG(MP3_DSP_DECODE_107_112_2_18_1_12_32_787,"mp3_decode:mp3_decode_open failed")
TRACE_MSG(AMR_DEMUX_119_112_2_18_1_12_33_788,"amr_demux.c:amr_demux_open:(bytes_read != sizeof(AMR_HEADER))  result is %d")
TRACE_MSG(AMR_DEMUX_149_112_2_18_1_12_33_789,"amr_demux.c:AMR_Demux_IdentifyFormat:stream_length is 0")
TRACE_MSG(AMR_DEMUX_153_112_2_18_1_12_33_790,"amr_demux.c:AMR_IdentifyFormat total time:%d s")
TRACE_MSG(AMR_DEMUX_187_112_2_18_1_12_33_791,"amr_demux.c:Demux_One_Frame:stream_peek:result is %d, bytes_read is %d)")
TRACE_MSG(AMR_DEMUX_204_112_2_18_1_12_33_792,"amr_demux.c:Demux_One_Frame:stream_read:stream_result==DATA_END)")
TRACE_MSG(AMR_DEMUX_229_112_2_18_1_12_33_793,"amr_demux.c:Demux_One_Frame flush one byte")
TRACE_MSG(AMR_DEMUX_379_112_2_18_1_12_33_794,"MP3_demux.c:stream_new failed")
TRACE_MSG(MP3_DEMUX_41_112_2_18_1_12_34_795,"[MP3_ParseFileInfo,2total time=%ds]")
TRACE_MSG(MP3_DEMUX_46_112_2_18_1_12_34_796,"MP3_ParseFileInfo,3total time=%ds]")
TRACE_MSG(MP3_DEMUX_80_112_2_18_1_12_34_797,"[MP3_ParseFileInfo,1total time=%ds]")
TRACE_MSG(MP3_DEMUX_122_112_2_18_1_12_34_798,"mp3_demux.c: identify_format result is %d")
TRACE_MSG(MP3_DEMUX_555_112_2_18_1_12_35_799,"MP3_demux.c:stream_new failed")
TRACE_MSG(MP3_DMX_BIT_238_112_2_18_1_12_35_800,"mp3_new_demux.c:decode_header:error:header->flags & MAD_FLAG_MPEG_2_5_EXT")
TRACE_MSG(MP3_DMX_BIT_247_112_2_18_1_12_35_801,"mp3_new_demux.c:decode_header:error layer is 4")
TRACE_MSG(MP3_DMX_BIT_263_112_2_18_1_12_35_802,"mp3_new_demux.c:decode_header:error MAD_ERROR_BADBITRATE")
TRACE_MSG(MP3_DMX_BIT_276_112_2_18_1_12_36_803,"mp3_new_demux.c:decode_header:error MAD_ERROR_BADSAMPLERATE")
TRACE_MSG(MP3_DMX_FORMAT_593_112_2_18_1_12_37_804,"mp3_dmx_format.c:MP3_Dmx_iToc:error:puiBuf == PNULL")
TRACE_MSG(MP3_DMX_FORMAT_598_112_2_18_1_12_37_805,"[MP3_Dmx_iToc]:input number: %d is not valid,(should be less than 9999) !")
TRACE_MSG(MP3_DMX_FORMAT_634_112_2_18_1_12_37_806,"mp3_dmx_format.c:MP3_Dmx_cToi:(puiBuf == PNULL)||(puiNum== 0)")
TRACE_MSG(MP3_DMX_FORMAT_655_112_2_18_1_12_37_807,"[MP3_DMX_FORMAT][MP3_CaculateNum]:i_calc:%d!not number!")
TRACE_MSG(MP3_DMX_FORMAT_679_112_2_18_1_12_37_808,"mp3_dmx_format.c:MP3_Dmx_CheckheaderInfo:(SCI_NULL == puiHeader)||(SCI_NULL == ptMp3HeaderInfo)")
TRACE_MSG(MP3_DMX_FORMAT_815_112_2_18_1_12_38_809,"mp3_dmx_format.c:MP3_Dmx_CheckheaderInfo:(SCI_NULL == puiHeader)||(SCI_NULL == ptMp3HeaderInfo)")
TRACE_MSG(MP3_DMX_FORMAT_896_112_2_18_1_12_38_810,"mp3_dmx_format.c: [MP3_CheckID3V2Tag]: ID3 FLAG NOT EXIST")
TRACE_MSG(MP3_DMX_FORMAT_908_112_2_18_1_12_38_811,"[MP3_CheckID3V2Tag]:tag version:%d,tag size:0x%08x")
TRACE_MSG(MP3_DMX_FORMAT_923_112_2_18_1_12_38_812,"[MP3_CheckID3V2Tag]:End of tag frames !")
TRACE_MSG(MP3_DMX_FORMAT_951_112_2_18_1_12_38_813,"[MP3_CheckID3V2Tag]:frame id :%d ,frame_content_size:%d")
TRACE_MSG(MP3_DMX_FORMAT_962_112_2_18_1_12_38_814,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_988_112_2_18_1_12_38_815,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_1014_112_2_18_1_12_38_816,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_1040_112_2_18_1_12_38_817,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_1066_112_2_18_1_12_38_818,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_1092_112_2_18_1_12_38_819,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_1118_112_2_18_1_12_38_820,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_1163_112_2_18_1_12_38_821,"[MP3_CheckID3V1Tag] V1.0:no valid year info!")
TRACE_MSG(MP3_DMX_FORMAT_1168_112_2_18_1_12_38_822,"[MP3_CheckID3V2Tag]:no valid genre index!")
TRACE_MSG(MP3_DMX_FORMAT_1197_112_2_18_1_12_38_823,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_1215_112_2_18_1_12_38_824,"[MP3_CheckID3V1Tag] V1.0:no valid year info!")
TRACE_MSG(MP3_DMX_FORMAT_1219_112_2_18_1_12_38_825,"[MP3_CheckID3V2Tag]:year:impossible value!")
TRACE_MSG(MP3_DMX_FORMAT_1231_112_2_18_1_12_38_826,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_1249_112_2_18_1_12_38_827,"mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!")
TRACE_MSG(MP3_DMX_FORMAT_1253_112_2_18_1_12_38_828,"mp3_format.c,[MP3_CheckID3V2Tag]:track:impossible value!")
TRACE_MSG(MP3_DMX_FORMAT_1267_112_2_18_1_12_38_829,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_1303_112_2_18_1_12_39_830,"mp3_format.c,[MP3_CheckID3V2Tag]:other frame ID !We do not need the frame content!")
TRACE_MSG(MP3_DMX_FORMAT_1311_112_2_18_1_12_39_831,"[MP3_CheckID3V2Tag]find invalid frame ID!")
TRACE_MSG(MP3_DMX_FORMAT_1421_112_2_18_1_12_39_832,"mp3_dmx_format.c:mp3_stream_fast_sync:result is %d")
TRACE_MSG(MP3_DMX_FORMAT_1434_112_2_18_1_12_39_833,"mp3_dmx_format.c:state_cur_syn:stream_peek:result is %d")
TRACE_MSG(MP3_DMX_FORMAT_1442_112_2_18_1_12_39_834,"mp3_dmx_format.c:state_cur_syn:MP3_Dmx_CheckheaderInfo:result is %d")
TRACE_MSG(MP3_DMX_FORMAT_1449_112_2_18_1_12_39_835,"peter:MP3_frame->frame_len is %d")
TRACE_MSG(MP3_DMX_FORMAT_1460_112_2_18_1_12_39_836,"mp3_dmx_format.c:state_cur_syn::frame_len is %d")
TRACE_MSG(MP3_DMX_FORMAT_1599_112_2_18_1_12_39_837,"mp3_dmx_format.c:::valid_frame_count   is %d")
TRACE_MSG(MP3_DMX_FORMAT_1629_112_2_18_1_12_39_838,"mp3_dmx_format.c:MP3_Dmx_CheckAPETagHeader:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)")
TRACE_MSG(MP3_DMX_FORMAT_1664_112_2_18_1_12_39_839,"[MP3_CheckLameTag] no valid version info!")
TRACE_MSG(MP3_DMX_FORMAT_1809_112_2_18_1_12_40_840,"[MP3_CheckID3V1Tag] V1.1:no valid year info!")
TRACE_MSG(MP3_DMX_FORMAT_1826_112_2_18_1_12_40_841,"[MP3_CheckID3V1Tag] V1.1:no valid genre info!")
TRACE_MSG(MP3_DMX_FORMAT_1843_112_2_18_1_12_40_842,"mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!")
TRACE_MSG(MP3_DMX_FORMAT_1857_112_2_18_1_12_40_843,"[MP3_CheckID3V1Tag]V1.0: no valid genre info!")
TRACE_MSG(MP3_DMX_FORMAT_1891_112_2_18_1_12_40_844,"mp3_dmx_format.c:MP3_Dmx_CheckXingHeader:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)")
TRACE_MSG(DOWNLINK_CODEC_ADP_93_112_2_18_1_12_40_845,"[BT]downlink_read_data:Data Full:0x%X, 0x%X\r\n")
TRACE_MSG(DOWNLINK_CODEC_ADP_120_112_2_18_1_12_41_846,"[BT]PCM:0x%X,0x%X,0x%X,0x%X\r\n")
TRACE_MSG(DOWNLINK_CODEC_ADP_137_112_2_18_1_12_41_847,"[BT]downlink_read_data:track_occupied is zero\r\n")
TRACE_MSG(DOWNLINK_CODEC_ADP_148_112_2_18_1_12_41_848,"[DOWNLINK_Play] IN")
TRACE_MSG(DOWNLINK_CODEC_ADP_158_112_2_18_1_12_41_849,"[DOWNLINK_Play] this codec is playing.")
TRACE_MSG(DOWNLINK_CODEC_ADP_165_112_2_18_1_12_41_850,"[DOWNLINK_Play] Have opened")
TRACE_MSG(DOWNLINK_CODEC_ADP_193_112_2_18_1_12_41_851,"[DOWNLINK_Stop] IN")
TRACE_MSG(DOWNLINK_CODEC_ADP_203_112_2_18_1_12_41_852,"[DOWNLINK_Stop] this codec is not playing.")
TRACE_MSG(DOWNLINK_CODEC_ADP_224_112_2_18_1_12_41_853,"[DATA_Stop] has closed")
TRACE_MSG(DOWNLINK_CODEC_ADP_251_112_2_18_1_12_41_854,"DOWNLINK_IdentifyFormat:error:SCI_NULL == ptAudioObject")
TRACE_MSG(DRA_ADP_293_112_2_18_1_12_42_855,"[DRA_Construct] Alloc failed, left_exp_data_ptr==NULL")
TRACE_MSG(DRA_ADP_304_112_2_18_1_12_42_856,"[DRA_Construct] Alloc failed, right_exp_data_ptr==NULL")
TRACE_MSG(DRA_ADP_315_112_2_18_1_12_42_857,"[DRA_Construct] Alloc failed, dra_res_ptr==NULL")
TRACE_MSG(DRA_ADP_331_112_2_18_1_12_42_858,"[DRA_Construct] aud_obj_ptr->pGetSrcCallbackFunc==NULL")
TRACE_MSG(DRA_ADP_339_112_2_18_1_12_42_859,"[DRA_Construct] dra_res_ptr->sub_type error")
TRACE_MSG(DRA_ADP_362_112_2_18_1_12_42_860,"[DRA_Construct] Create DRA thread failed.")
TRACE_MSG(DRA_ADP_441_112_2_18_1_12_42_861,"[DRA_PlayConstruct] CHNG_FREQ_REQ_HANDLER_NULL == s_ui_dra_chng_freq_req_handler")
TRACE_MSG(DRA_ADP_486_112_2_18_1_12_42_862,"[DRA_ResumeConstruct] CHNG_FREQ_REQ_HANDLER_NULL == s_ui_dra_chng_freq_req_handler")
TRACE_MSG(DRA_ADP_530_112_2_18_1_12_42_863,"[DRA_Play] NULL == sig_ptr")
TRACE_MSG(DRA_ADP_538_112_2_18_1_12_42_864,"\"[DRA_Play] start to play.")
TRACE_MSG(DRA_ADP_556_112_2_18_1_12_42_865,"[DRA_Stop] NULL == sig_ptr")
TRACE_MSG(DRA_ADP_564_112_2_18_1_12_42_866,"[DRA_Stop] stopped.")
TRACE_MSG(DRA_ADP_582_112_2_18_1_12_42_867,"[DRA_Pause] NULL == sig_ptr")
TRACE_MSG(DRA_ADP_590_112_2_18_1_12_42_868,"[DRA_Pause] pause.")
TRACE_MSG(DRA_ADP_609_112_2_18_1_12_42_869,"[DRA_Resume] NULL == sig_ptr")
TRACE_MSG(DRA_ADP_617_112_2_18_1_12_42_870,"[DRA_Resume] resume.")
TRACE_MSG(DRA_ADP_651_112_2_18_1_12_42_871,"[DRA_IdentifyFormat] error:NULL == dra_res_ptr")
TRACE_MSG(DRA_ADP_663_112_2_18_1_12_42_872,"[DRA_IdentifyFormat] error:NULL == cfg_info_ptr")
TRACE_MSG(DRA_ADP_672_112_2_18_1_12_43_873,"[DRA_IdentifyFormat] dra_res_ptr->sub_type error")
TRACE_MSG(DRA_ADP_702_112_2_18_1_12_43_874,"[DRA_GetOutputSamplerate] OutputSampleRate=%d")
TRACE_MSG(DRA_ADP_758_112_2_18_1_12_43_875,"[DRA_SendSig] default error")
TRACE_MSG(DRA_ADP_781_112_2_18_1_12_43_876,"[DRA_InitDecoder] Error: dec_result=%d")
TRACE_MSG(DRA_ADP_784_112_2_18_1_12_43_877,"[DRA_InitDecoder] Init successfully.")
TRACE_MSG(DRA_ADP_807_112_2_18_1_12_43_878,"[DRA_TermDecoder] Error: dec_result=%d")
TRACE_MSG(DRA_ADP_810_112_2_18_1_12_43_879,"[DRA_TermDecoder] terminate successfully.")
TRACE_MSG(DRA_ADP_852_112_2_18_1_12_43_880,"[DRA_DecodeAudFrame] Error: dec_result=%d")
TRACE_MSG(DRA_ADP_920_112_2_18_1_12_43_881,"[pGetSrcCallbackFunc] 0 == dra_res_ptr->src_data_len")
TRACE_MSG(DRA_ADP_930_112_2_18_1_12_43_882,"[pGetSrcCallbackFunc] AUDIO_STREAM_GET_SRC_WAIT")
TRACE_MSG(DRA_ADP_935_112_2_18_1_12_43_883,"[pGetSrcCallbackFunc] AUDIO_STREAM_GET_SRC_ASY")
TRACE_MSG(DRA_ADP_939_112_2_18_1_12_43_884,"[pGetSrcCallbackFunc] AUDIO_STREAM_GET_SRC_ERROR")
TRACE_MSG(DRA_ADP_944_112_2_18_1_12_43_885,"[pGetSrcCallbackFunc] default error")
TRACE_MSG(DRA_ADP_1012_112_2_18_1_12_43_886,"[DRA_Thread_Entry] default error")
TRACE_MSG(DSP_CODEC_ADP_121_112_2_18_1_13_1_887,"dsp_codec_adp.c:Read_Layer1_Data_Callback:aud_empty_frame_get failed 1")
TRACE_MSG(DSP_CODEC_ADP_167_112_2_18_1_13_1_888,"dsp_codec_adp.c:Read_Layer1_Data_Callback:aud_empty_frame_get failed 2")
TRACE_MSG(DSP_CODEC_ADP_289_112_2_18_1_13_1_889,"dsp_codec_adp.c:DSP_CODEC_Ext_Play:error: || (NULL==pvArgv)")
TRACE_MSG(DSP_CODEC_ADP_302_112_2_18_1_13_1_890,"dsp_codec_adp.c:DSP_CODEC_Ext_Play:error:audio_obj == SCI_NULL")
TRACE_MSG(DSP_CODEC_ADP_352_112_2_18_1_13_1_891,"dsp_codec_adp.c:DSP_CODEC_Ext_Stop:error:audio_obj == SCI_NULL")
TRACE_MSG(DSP_CODEC_ADP_389_112_2_18_1_13_1_892,"dsp_codec_adp.c:DSP_CODEC_Ext_WriteVoiceData:error: || (NULL==pvArgv)")
TRACE_MSG(DSP_CODEC_ADP_397_112_2_18_1_13_1_893,"dsp_codec_adp.c:DSP_CODEC_Ext_WriteVoiceData: PNULL==ptPara->source_buf_ptr")
TRACE_MSG(DSP_CODEC_ADP_424_112_2_18_1_13_1_894,"dsp_codec_adp.c:DSP_CODEC_Ext_StartRecord:error: || (NULL==pvArgv)")
TRACE_MSG(DSP_CODEC_ADP_438_112_2_18_1_13_2_895,"dsp_codec_adp.c:DSP_CODEC_Ext_StartRecord:error:audio_obj == SCI_NULL")
TRACE_MSG(DSP_CODEC_ADP_485_112_2_18_1_13_2_896,"dsp_codec_adp.c:DSP_CODEC_Ext_StopRecord:error:audio_obj == SCI_NULL")
TRACE_MSG(DSP_CODEC_ADP_520_112_2_18_1_13_2_897,"dsp_codec_adp.c:DSP_CODEC_Ext_ReadVoiceData:error: || (NULL==pvArgv)")
TRACE_MSG(DSP_CODEC_ADP_528_112_2_18_1_13_2_898,"dsp_codec_adp.c:DSP_CODEC_Ext_ReadVoiceData: PNULL==ptPara->source_buf_ptr")
TRACE_MSG(DSP_CODEC_ADP_583_112_2_18_1_13_2_899,"dsp_codec_adp.c:DSP_CODEC_Ext_StartUpLoadData:ptAudioObject is NULL")
TRACE_MSG(DSP_CODEC_ADP_592_112_2_18_1_13_2_900,"dsp_codec_adp.c:DSP_CODEC_Ext_StartUpLoadData:aud_framebuffer_new failed")
TRACE_MSG(DSP_CODEC_ADP_602_112_2_18_1_13_2_901,"dsp_codec_adp.c:DSP_CODEC_Ext_StartUpLoadData:SCI_ALLOC_APP TEMP_FRAME_T failed")
TRACE_MSG(DSP_CODEC_ADP_612_112_2_18_1_13_2_902,"dsp_codec_adp.c:DSP_CODEC_Ext_StartUpLoadData:SCI_ALLOC_APP TEMP_FRAME_T buffer failed")
TRACE_MSG(DSP_CODEC_ADP_632_112_2_18_1_13_2_903,"dsp_codec_adp.c:DSP_CODEC_Ext_StartUpLoadData:SCI_CreateEvent failed")
TRACE_MSG(DSP_CODEC_ADP_688_112_2_18_1_13_2_904,"dsp_codec_adp.c:DSP_CODEC_Ext_StopUpLoadData:error:audio_obj == SCI_NULL")
TRACE_MSG(DSP_CODEC_ADP_720_112_2_18_1_13_2_905,"dsp_codec_adp.c:DSP_CODEC_Ext_StopRecord:error:audio_obj == SCI_NULL")
TRACE_MSG(DSP_CODEC_ADP_760_112_2_18_1_13_2_906,"dsp_codec_adp.c:DSP_CODEC_IdentifyFormat:error:audio_obj == SCI_NULL")
TRACE_MSG(DTMF_TONE_ADP_188_112_2_18_1_13_3_907,"DTMF_TONE_Construct SCI_CreateMutex pTonePlayMutex failed.")
TRACE_MSG(DTMF_TONE_ADP_198_112_2_18_1_13_3_908,"DTMF_TONE_Construct SCI_CreateMutex pToneStopMutex failed.")
TRACE_MSG(DTMF_TONE_ADP_208_112_2_18_1_13_3_909,"DTMF_TONE_Construct SCI_CreateTimer failed.")
TRACE_MSG(DTMF_TONE_ADP_228_112_2_18_1_13_3_910,"dtmf_tone_adp.c:DTMF_TONE_IdentifyFormat:error:PNULL != ptAudioObject")
TRACE_MSG(DTMF_TONE_ADP_259_112_2_18_1_13_3_911,"dtmf_tone_adp.c:DTMF_TONE_Ext_Play:error:PNULL != ptAudioObject")
TRACE_MSG(DTMF_TONE_ADP_266_112_2_18_1_13_3_912,"dtmf_tone_adp.c: DTMF_TONE_Ext_Play other task has been playing")
TRACE_MSG(DTMF_TONE_ADP_274_112_2_18_1_13_3_913,"dtmf_tone_adp.c:DTMF_TONE_Ext_Play:error:PNULL == ptAudioObject")
TRACE_MSG(DTMF_TONE_ADP_280_112_2_18_1_13_3_914,"dtmf_tone_adp.c:DTMF_TONE_Ext_Play:error:0==ptPara->duration.")
TRACE_MSG(DTMF_TONE_ADP_286_112_2_18_1_13_3_915,"dtmf_tone_adp.c:DTMF_TONE_Ext_Play: is playing.")
TRACE_MSG(DTMF_TONE_ADP_304_112_2_18_1_13_3_916,"[DTMF_TONE_Ext_Play] AUDIO_DM_Open failed!")
TRACE_MSG(DTMF_TONE_ADP_317_112_2_18_1_13_3_917,"dtmf_tone_adp.c: DTMF_TONE_Ext_Play other task has been playing")
TRACE_MSG(DTMF_TONE_ADP_356_112_2_18_1_13_3_918,"dtmf_tone_adp.c:DTMF_TONE_Ext_Stop:error:uiArgc != 0")
TRACE_MSG(DTMF_TONE_ADP_363_112_2_18_1_13_3_919,"dtmf_tone_adp.c: DTMF_TONE_Ext_Stop other task has been stopping")
TRACE_MSG(DTMF_TONE_ADP_368_112_2_18_1_13_3_920,"dtmf_tone_adp.c:DTMF_TONE_Ext_Stop: is not playing.")
TRACE_MSG(DTMF_TONE_ADP_377_112_2_18_1_13_3_921,"dtmf_tone_adp.c DTMF_TONE_Ext_Stop the tone is never played.")
TRACE_MSG(DTMF_TONE_ADP_383_112_2_18_1_13_3_922,"dtmf_tone_adp.c:DTMF_TONE_Ext_Stop:error:PNULL != ptAudioObject")
TRACE_MSG(DTMF_TONE_ADP_389_112_2_18_1_13_3_923,"dtmf_tone_adp.c: DTMF_TONE_Ext_Stop other task has been stopping")
TRACE_MSG(DTMF_TONE_ADP_416_112_2_18_1_13_3_924,"%s, %d. DTMF_TONE_Stop")
TRACE_MSG(DTMF_TONE_ADP_428_112_2_18_1_13_4_925,"dtmf_tone_adp.c:DTMF_TONE_Stop:error:PNULL == ptAudioObject")
TRACE_MSG(DTMF_TONE_ADP_435_112_2_18_1_13_4_926,"dtmf_tone_adp.c: DTMF_TONE_StopDeconstruct other task has been stopping")
TRACE_MSG(DTMF_TONE_ADP_440_112_2_18_1_13_4_927,"dtmf_tone_adp.c:DTMF_TONE_StopDeconstruct: is not playing.")
TRACE_MSG(ADPCM_ENCODER_249_112_2_18_1_13_4_928,"error in IMA ADPCM encoding--low_4bits:%d, high_4bits:%d!\n")
TRACE_MSG(ADPCM_ENCODER_357_112_2_18_1_13_4_929,"AdpcmEncoderSetSamplerate Error : %d")
TRACE_MSG(ADPCM_ENCODER_451_112_2_18_1_13_5_930,"The header size of wav file for ADCPM is %d, not 60 bytes!\n")
TRACE_MSG(ADPCM_ENCODE_96_112_2_18_1_13_5_931,"ADPCM_ENCode:adpcm_encode_open failed")
TRACE_MSG(MP3_ENCODE_22_112_2_18_1_13_35_932,"mp3_encode.c:mp3_encode_start")
TRACE_MSG(MP3_ENCODE_26_112_2_18_1_13_35_933,"peter: mp3_decode.c:MP3_ENC_MemoryAlloc:error")
TRACE_MSG(MP3_ENCODE_45_112_2_18_1_13_36_934,"peter: mp3_encode_dec in frame->frame_len is %d")
TRACE_MSG(MP3_ENCODE_53_112_2_18_1_13_36_935,"peter: mp3_encode_dec  result is %d,in frame_out->frame_len is %d")
TRACE_MSG(MP3_ENCODE_100_112_2_18_1_13_36_936,"MP3_ENCode:mp3_encode_open failed")
TRACE_MSG(GENERIC_TONE_ADP_287_112_2_18_1_13_37_937,"GENERIC_TONE_Construct SCI_CreateMutex pTonePlayMutex failed.")
TRACE_MSG(GENERIC_TONE_ADP_297_112_2_18_1_13_37_938,"GENERIC_TONE_Construct SCI_CreateMutex pToneStopMutex failed.")
TRACE_MSG(GENERIC_TONE_ADP_307_112_2_18_1_13_37_939,"GENERIC_TONE_Construct SCI_CreateTimer GenericToneTimer failed.")
TRACE_MSG(GENERIC_TONE_ADP_317_112_2_18_1_13_37_940,"GENERIC_TONE_Construct SCI_CreateTimer GenericDurationToneTimer failed.")
TRACE_MSG(GENERIC_TONE_ADP_395_112_2_18_1_13_37_941,"generic_tone_adp.c:GENERIC_TONE_IdentifyFormat:error:PNULL == ptAudioObject")
TRACE_MSG(GENERIC_TONE_ADP_426_112_2_18_1_13_37_942,"generic_tone_adp.c:GENERIC_TONE_Ext_Play:error:NULL==pvArgv")
TRACE_MSG(GENERIC_TONE_ADP_433_112_2_18_1_13_37_943,"generic_tone_adp.c: GENERIC_TONE_Ext_Play other task has been playing")
TRACE_MSG(GENERIC_TONE_ADP_446_112_2_18_1_13_37_944,"generic_tone_adp.c:GENERIC_TONE_Ext_Play:error:PNULL == ptAudioObject")
TRACE_MSG(GENERIC_TONE_ADP_453_112_2_18_1_13_37_945,"generic_tone_adp.c:GENERIC_TONE_Ext_Play: is playing.")
TRACE_MSG(GENERIC_TONE_ADP_471_112_2_18_1_13_37_946,"[GENERIC_TONE_Ext_Play] AUDIO_DM_Open failed!")
TRACE_MSG(GENERIC_TONE_ADP_504_112_2_18_1_13_37_947,"generic_tone_adp.c:GENERIC_TONE_Ext_Stop:error:PNULL == ptAudioObject")
TRACE_MSG(GENERIC_TONE_ADP_511_112_2_18_1_13_37_948,"generic_tone_adp.c: GENERIC_TONE_Ext_Stop other task has been stopping")
TRACE_MSG(GENERIC_TONE_ADP_516_112_2_18_1_13_37_949,"generic_tone_adp.c:GENERIC_TONE_Ext_Stop: is not playing.")
TRACE_MSG(GENERIC_TONE_ADP_525_112_2_18_1_13_37_950,"generic_tone_adp.c:GENERIC_TONE_Ext_Stop the tone is never played.")
TRACE_MSG(GENERIC_TONE_ADP_534_112_2_18_1_13_37_951,"generic_tone_adp.c:GENERIC_TONE_Ext_Stop:error:PNULL == ptAudioObject")
TRACE_MSG(GENERIC_TONE_ADP_560_112_2_18_1_13_37_952,"%s, %d. GENERIC_TONE_Stop")
TRACE_MSG(GENERIC_TONE_ADP_574_112_2_18_1_13_37_953,"generic_tone_adp.c:GENERIC_TONE_Ext_Play:error:PNULL == ptAudioObject")
TRACE_MSG(GENERIC_TONE_ADP_581_112_2_18_1_13_37_954,"generic_tone_adp.c: GENERIC_TONE_StopDeconstruct other task has been stopping")
TRACE_MSG(GENERIC_TONE_ADP_586_112_2_18_1_13_37_955,"generic_tone_adp.c:GENERIC_TONE_StopDeconstruct: is not playing.")
TRACE_MSG(GENERIC_TONE_ADP_611_112_2_18_1_13_37_956,"AUD_PlayGenericTone: tone_id = %d,  duration = %d")
TRACE_MSG(GENERIC_TONE_ADP_619_112_2_18_1_13_37_957,"generic_tone_adp.c:AUD_GenericTonePlay:error:NE_ID_MAX) || (0 == dura")
TRACE_MSG(GENERIC_TONE_ADP_626_112_2_18_1_13_37_958,"generic_tone_adp.c:AUD_GenericTonePlay:error:type != AUD_SINGLE_TONE_RING")
TRACE_MSG(GENERIC_TONE_ADP_633_112_2_18_1_13_37_959,"generic_tone_adp.c: GENERIC_TONE_Ext_Play other task has been playing")
TRACE_MSG(GENERIC_TONE_ADP_674_112_2_18_1_13_37_960,"AUD_GenericToneStop: suiTonePlayState = %d !")
TRACE_MSG(GENERIC_TONE_ADP_681_112_2_18_1_13_37_961,"generic_tone_adp.c:GENERIC_TONE_Ext_Play:error:PNULL == ptAudioObject")
TRACE_MSG(GENERIC_TONE_ADP_688_112_2_18_1_13_37_962,"generic_tone_adp.c: GENERIC_TONE_Ext_Stop other task has been stopping")
TRACE_MSG(MIC_SAMPLE_212_112_2_18_1_13_38_963,"mic_sample.c[MicSample_Play]: Have opened")
TRACE_MSG(MIC_SAMPLE_229_112_2_18_1_13_38_964,"mic_sample.c[MicSample_Play]: MicSample Play!")
TRACE_MSG(MIC_SAMPLE_252_112_2_18_1_13_38_965,"mic_sample.c[MicSample_Stop]: Have Closed")
TRACE_MSG(MIC_SAMPLE_255_112_2_18_1_13_38_966,"mic_sample.c[MicSample_Stop]: MicSample_Stop!")
TRACE_MSG(MIC_SAMPLE_270_112_2_18_1_13_38_967,"mic_sample.c: [MicSample_GetOutputSampleRate]:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MIC_SAMPLE_335_112_2_18_1_13_38_968,"mic_sample.c[MicSample_RegCodecPlugger]: register mic sample codec! codec handle is 0x%08x")
TRACE_MSG(MIC_SAMPLE_374_112_2_18_1_13_39_969,"mic_sample.c[MicSample_GetMicdB]:  db_average=%d")
TRACE_MSG(MIC_SAMPLE_382_112_2_18_1_13_39_970,"mic_sample.c[MicSample_GetMicdB]:  db_square_sum=%d")
TRACE_MSG(MIC_SAMPLE_420_112_2_18_1_13_39_971,"mic_sample.c[MicSample_GetMic_dB]:  group_index=%d")
TRACE_MSG(MIC_SAMPLE_439_112_2_18_1_13_39_972,"mic_sample.c[MicSample_GetMic_dB]:  get_db=%d")
TRACE_MSG(MIC_SAMPLE_460_112_2_18_1_13_39_973,"mic_sample.c[AUD_GetMic_dB]:  rt_db=%d")
TRACE_MSG(MIDI_ADP_532_112_2_18_1_14_14_974,"midi_adp.c:MIDI_Construct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_546_112_2_18_1_14_14_975,"ERROR midi no memory 1.")
TRACE_MSG(MIDI_ADP_553_112_2_18_1_14_14_976,"ERROR midi no memory 2.")
TRACE_MSG(MIDI_ADP_561_112_2_18_1_14_14_977,"ERROR midi no memory 3.")
TRACE_MSG(MIDI_ADP_569_112_2_18_1_14_14_978,"ERROR midi no memory 4.")
TRACE_MSG(MIDI_ADP_579_112_2_18_1_14_14_979,"ERROR midi no memory 5.")
TRACE_MSG(MIDI_ADP_589_112_2_18_1_14_14_980,"ERROR midi no memory 6.")
TRACE_MSG(MIDI_ADP_661_112_2_18_1_14_14_981,"midi_adp.c:MIDI_Construct:error:stack_area) != (uint32)&Image")
TRACE_MSG(MIDI_ADP_674_112_2_18_1_14_14_982,"ERROR midi no memory 7.")
TRACE_MSG(MIDI_ADP_687_112_2_18_1_14_14_983,"ERROR midi no memory 8.")
TRACE_MSG(MIDI_ADP_696_112_2_18_1_14_14_984,"ERROR midi no memory 9.")
TRACE_MSG(MIDI_ADP_706_112_2_18_1_14_14_985,"ERROR midi no memory 10.")
TRACE_MSG(MIDI_ADP_719_112_2_18_1_14_14_986,"ERROR midi no memory 11.")
TRACE_MSG(MIDI_ADP_727_112_2_18_1_14_14_987,"ERROR midi no memory 12.")
TRACE_MSG(MIDI_ADP_738_112_2_18_1_14_14_988,"midi_adp.c:MIDI_Construct:error:E * 4) != (uint32)&Image$")
TRACE_MSG(MIDI_ADP_745_112_2_18_1_14_14_989,"ERROR midi no memory 13.")
TRACE_MSG(MIDI_ADP_760_112_2_18_1_14_14_990,"ERROR midi no memory 14.")
TRACE_MSG(MIDI_ADP_773_112_2_18_1_14_14_991,"midi_adp.c [MIDI_Construct] psPcmL:0x%x,len:0x%x(bytes).")
TRACE_MSG(MIDI_ADP_777_112_2_18_1_14_14_992,"midi_adp.c [MIDI_Construct] psPcmR:0x%x.")
TRACE_MSG(MIDI_ADP_785_112_2_18_1_14_14_993,"ERROR midi no memory 15.! len:%d.")
TRACE_MSG(MIDI_ADP_794_112_2_18_1_14_14_994,"midi_adp.c [MIDI_Construct] psExpProDataL:0x%x,len:0x%x(bytes).")
TRACE_MSG(MIDI_ADP_798_112_2_18_1_14_14_995,"midi_adp.c [MIDI_Construct] psExpProDataR:0x%x.")
TRACE_MSG(MIDI_ADP_803_112_2_18_1_14_14_996,"ERROR midi no memory  16! len:%d.")
TRACE_MSG(MIDI_ADP_829_112_2_18_1_14_14_997,"midi_adp.c [MIDI_Construct]  create midi assistant dynamic task!")
TRACE_MSG(MIDI_ADP_834_112_2_18_1_14_14_998,"midi_adp.c [MIDI_Construct]  can not create midi assistant dynamic task!")
TRACE_MSG(MIDI_ADP_884_112_2_18_1_14_14_999,"midi_adp.c:MIDI_Deconstruct:error:ptMidiProcRes==PNULL")
TRACE_MSG(MIDI_ADP_893_112_2_18_1_14_14_1000,"midi_adp.c:MIDI_Deconstruct:error:tpDspProRes==PNULL")
TRACE_MSG(MIDI_ADP_963_112_2_18_1_14_14_1001,"midi_adp.c:MIDI_PlayConstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_990_112_2_18_1_14_14_1002,"midi_adp.c:MIDI_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_midi_chng_freq_req_handler")
TRACE_MSG(MIDI_ADP_996_112_2_18_1_14_14_1003,"midi_adp.c,[MIDI_PlayConstruct] CHNG_FREQ_SetArmClk()!")
TRACE_MSG(MIDI_ADP_1061_112_2_18_1_14_15_1004,"midi_adp.c,[MIDI_PlayConstruct]set bus monitor!")
TRACE_MSG(MIDI_ADP_1078_112_2_18_1_14_15_1005,"midi_adp.c,[MIDI_PlayConstruct]set bus monitor!")
TRACE_MSG(MIDI_ADP_1114_112_2_18_1_14_15_1006,"midi_adp.c:MIDI_ResumeConstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1141_112_2_18_1_14_15_1007,"midi_adp.c:MIDI_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_midi_chng_freq_req_handler")
TRACE_MSG(MIDI_ADP_1147_112_2_18_1_14_15_1008,"midi_adp.c,[MIDI_ResumeConstruct] CHNG_FREQ_SetArmClk()!")
TRACE_MSG(MIDI_ADP_1181_112_2_18_1_14_15_1009,"midi_adp.c:MIDI_PauseDeconstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1229_112_2_18_1_14_15_1010,"midi_adp.c:MIDI_StopDeconstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1312_112_2_18_1_14_15_1011,"midi_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1331_112_2_18_1_14_15_1012,"MIDI_Stop")
TRACE_MSG(MIDI_ADP_1337_112_2_18_1_14_15_1013,"midi_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1369_112_2_18_1_14_15_1014,"midi_adp.c:MIDI_Pause:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1395_112_2_18_1_14_15_1015,"midi_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1469_112_2_18_1_14_16_1016,"midi_adp.c:MIDI_IdentifyFormat:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1478_112_2_18_1_14_16_1017,"midi_adp.c:MIDI_IdentifyFormat:error:PNULL == mididata")
TRACE_MSG(MIDI_ADP_1514_112_2_18_1_14_16_1018,"midi_adp.c:Midi_IdentifyFormat:error:Unknown Handle!")
TRACE_MSG(MIDI_ADP_1562_112_2_18_1_14_16_1019,"MIDI_IdentifyFormat: Pre checking ERROR! track %d is out of range")
TRACE_MSG(MIDI_ADP_1569_112_2_18_1_14_16_1020,"MIDI_IdentifyFormat: Track %d ERROR. MTrk not found")
TRACE_MSG(MIDI_ADP_1587_112_2_18_1_14_16_1021,"MIDI_IdentifyFormat: ERROR! track %d is out of range")
TRACE_MSG(MIDI_ADP_1606_112_2_18_1_14_16_1022,"[MIDI_IdentifyFormat] ptAudioObject->tAudioFormatInfo.uiTotalTime == 0!")
TRACE_MSG(MIDI_ADP_1610_112_2_18_1_14_16_1023,"[MIDI_IdentifyFormat] ptAudioObject->tAudioFormatInfo.uiTotalTime return error!")
TRACE_MSG(MIDI_ADP_1695_112_2_18_1_14_16_1024,"MIDI_GetSampleRate outSamplerate:%d.")
TRACE_MSG(MIDI_ADP_1732_112_2_18_1_14_16_1025,"midi_adp.c:MIDI_SetPolyNum:error:audio_obj == SCI_NULL")
TRACE_MSG(MIDI_ADP_2085_112_2_18_1_14_17_1026,"MIDI_Thread_Entry CODEC_SendSignal CODEC_STOP_REQ")
TRACE_MSG(MIDI_ADP_2128_112_2_18_1_14_17_1027,"MidiPlayFile ERROR MIDI_SC_Init:%d.")
TRACE_MSG(MIDI_ADP_2138_112_2_18_1_14_17_1028,"MidiPlayFile ERROR MIDI_SC_Play:%d.")
TRACE_MSG(MIDI_ADP_2205_112_2_18_1_14_17_1029,"offset(%d) is bigger than file_size(%d).")
TRACE_MSG(MIDI_ADP_2226_112_2_18_1_14_17_1030,"ERROR s_midi_data_buffer no memory.")
TRACE_MSG(MIDI_ADP_2239_112_2_18_1_14_17_1031,"ERROR FFS_Read, %d, %d, %d.")
TRACE_MSG(MIDI_ADP_2248_112_2_18_1_14_17_1032,"ERROR FFS_fseek, %d, %d.")
TRACE_MSG(MIDI_ADP_2259_112_2_18_1_14_17_1033,"midi not support streambuffer play.")
TRACE_MSG(MIDI_ADP_2264_112_2_18_1_14_17_1034,"MidiGetSrcData Invalid Handle Type.")
TRACE_MSG(MIDI_ADP_2300_112_2_18_1_14_17_1035,"midi_adp.c:Write_MIDIDecOutputBuf:error, l:0x%x, r:0x%x.")
TRACE_MSG(MIDI_ADP_2316_112_2_18_1_14_17_1036,"MIDI:BUSY!")
TRACE_MSG(MIDI_ADP_2323_112_2_18_1_14_17_1037,"MIDI:FREE!")
TRACE_MSG(MIDI_ADP_2341_112_2_18_1_14_17_1038,"Write_MIDIDecOutputBuf 0 discard pcm data count:%d.")
TRACE_MSG(MIDI_ADP_2347_112_2_18_1_14_17_1039,"CODEC_SendSignal CODEC_ASSI_PLAY_REQ in write buf.")
TRACE_MSG(MIDI_ADP_2368_112_2_18_1_14_17_1040,"Write_MIDIDecOutputBuf 1 discard pcm data count:%d.")
TRACE_MSG(MIDI_ADP_2483_112_2_18_1_14_18_1041,"Write_MIDIDecOutputBuf 2 discard pcm data count:%d.")
TRACE_MSG(MIDI_ADP_2495_112_2_18_1_14_18_1042,"Write_MIDIDecOutputBuf dataLen ready to Dsp:%d")
TRACE_MSG(MIDI_ADP_2550_112_2_18_1_14_18_1043,"midi_adp.c: error::AUDIO_DEFAULT_TASK_ID == tThreadId")
TRACE_MSG(MIDI_ADP_2564_112_2_18_1_14_18_1044,"midi_adp.c [CODEC_SendSignal]-0-  alloc failed!")
TRACE_MSG(MIDI_ADP_2580_112_2_18_1_14_18_1045,"midi_adp.c [CODEC_SendSignal]-01-  alloc failed!")
TRACE_MSG(MIDI_ADP_2596_112_2_18_1_14_18_1046,"midi_adp.c [CODEC_SendSignal]-1-  alloc failed!")
TRACE_MSG(MIDI_ADP_2612_112_2_18_1_14_18_1047,"midi_adp.c [CODEC_SendSignal]-2-  alloc failed!")
TRACE_MSG(MIDI_ADP_2628_112_2_18_1_14_18_1048,"midi_adp.c [CODEC_SendSignal]-9-  alloc failed!")
TRACE_MSG(MIDI_ADP_2636_112_2_18_1_14_18_1049,"midi_adp.c [CODEC_SendSignal]CODEC_ASSI_PLAY_REQ from 0x%x to 0x%x.")
TRACE_MSG(MIDI_ADP_2647_112_2_18_1_14_18_1050,"midi_adp.c [CODEC_SendSignal]-10-  alloc failed!")
TRACE_MSG(MIDI_ADP_2663_112_2_18_1_14_18_1051,"midi_adp.c [CODEC_SendSignal]-11-  alloc failed!")
TRACE_MSG(MIDI_ADP_2679_112_2_18_1_14_18_1052,"midi_adp.c [CODEC_SendSignal]-12-  alloc failed!")
TRACE_MSG(MIDI_ADP_2692_112_2_18_1_14_18_1053,"midi_adp.c  [CODEC_SendSignal]error eSigCode:0x%08x!")
TRACE_MSG(MIDI_ADP_2761_112_2_18_1_14_18_1054,"midi_adp.c[CODEC_AssiTask_DspExpPros] begin to dsp express process wav!")
TRACE_MSG(MIDI_ADP_2831_112_2_18_1_14_18_1055,"CODEC_ASSI_STOP_REQ force to exit from pausing.")
TRACE_MSG(MIDI_ADP_2844_112_2_18_1_14_18_1056,"midi_adp.c CODEC_AssiTask_DspExpPros midi decode has stopped.")
TRACE_MSG(MIDI_ADP_2853_112_2_18_1_14_18_1057,"midi_adp.c CODEC_AssiTask_DspExpPros midi decode exit from pausing state.")
TRACE_MSG(MIDI_ADP_2862_112_2_18_1_14_18_1058,"midi_adp.c CODEC_AssiTask_DspExpPros MIDI_SC_Stop.")
TRACE_MSG(MIDI_ADP_2869_112_2_18_1_14_18_1059,"midi_adp.c CODEC_AssiTask_DspExpPros exit.")
TRACE_MSG(MIDI_ADP_2878_112_2_18_1_14_18_1060,"midi_adp.c CODEC_AssiTask_DspExpPros stop waiting %d.")
TRACE_MSG(MIDI_ADP_2884_112_2_18_1_14_18_1061,"CODEC_ASSI_STOP_REQ force to exit from stopping.")
TRACE_MSG(MIDI_ADP_2899_112_2_18_1_14_18_1062,"CODEC_ASSI_PLAY_REQ receive again sender:0x%x state:%d.")
TRACE_MSG(MIDI_ADP_2932_112_2_18_1_14_18_1063,"CODEC_ASSI_STOP_REQ wait to get dsp ind too long.")
TRACE_MSG(MIDI_ADP_2950_112_2_18_1_14_19_1064,"midi decode task %d.")
TRACE_MSG(MIDI_ADP_2974_112_2_18_1_14_19_1065,"CODEC_ASSI_DSP_EXP_PRO_REQ wait to get pcm data too long.")
TRACE_MSG(MIDI_ADP_3011_112_2_18_1_14_19_1066,"CODEC_ASSI LAYER1_AUDIO_CODEC_SetData 1 dest_addr:0x%x, src:0x%x: 0x%x + outIndex:0x%x, len:0x%x.")
TRACE_MSG(MIDI_ADP_3026_112_2_18_1_14_19_1067,"CODEC_ASSI LAYER1_AUDIO_CODEC_CopyData 2 dest_addr:0x%x, src:0x%x: 0x%x + outIndex:0x%x, len:0x%x.")
TRACE_MSG(MIDI_ADP_3080_112_2_18_1_14_19_1068,"CODEC_ASSI LAYER1_AUDIO_CODEC_CopyData 3 dest_addr:0x%x, src:0x%x=0x%x+outIndex:0x%x, len:0x%x.")
TRACE_MSG(MIDI_ADP_3094_112_2_18_1_14_19_1069,"CODEC_ASSI LAYER1_AUDIO_CODEC_CopyData 4 dest_addr:0x%x, 0x%x, len:0x%x.")
TRACE_MSG(MIDI_ADP_3109_112_2_18_1_14_19_1070,"CODEC_ASSI LAYER1_AUDIO_CODEC_CopyData 5 dest_addr:0x%x, src:0x%x=0x%x+outIndex:0x%x, len:0x%x.")
TRACE_MSG(MIDI_ADP_3118_112_2_18_1_14_19_1071,"CODEC_ASSI LAYER1_AUDIO_CODEC_CopyData 6 dest_addr:0x%x, 0x%x, len:0x%x.")
TRACE_MSG(MIDI_ADP_3142_112_2_18_1_14_19_1072,"CODEC_ASSI_DSP_EXP_PRO_REQ sample:%d, src samplerate:%d, time:%d.")
TRACE_MSG(MIDI_ADP_3153_112_2_18_1_14_19_1073,"CODEC_ASSI_DSP_EXP_PRO_REQ exit from tran of dsp data process.")
TRACE_MSG(MIDI_ADP_3158_112_2_18_1_14_19_1074,"CODEC_ASSI_DSP_EXP_PRO_REQ exit from sbc or tran of dsp data process.")
TRACE_MSG(MIDI_ADP_3168_112_2_18_1_14_19_1075,"midi decode task %d.")
TRACE_MSG(MIDI_ADP_3185_112_2_18_1_14_19_1076,"CODEC_ASSI_DSP_EXP_PRO_IND sample:%d, time:%d.")
TRACE_MSG(MIDI_ADP_3220_112_2_18_1_14_19_1077,"midi_adp.c [CODEC_AssiTask_DspExpPros]other invalid signal code:%d!")
TRACE_MSG(MIDI_API_130_112_2_18_1_14_20_1078,"MIDI_Play: ring_type = %d, ring_data_len = %d, ring_data_ptr = %d, play_times = %d!")
TRACE_MSG(MIDI_API_140_112_2_18_1_14_20_1079,"MIDI_Play: It does not support this ring type!")
TRACE_MSG(MIDI_API_256_112_2_18_1_14_20_1080,"MidiPlayFile: Playing midi.")
TRACE_MSG(MIDI_API_347_112_2_18_1_14_20_1081,"\r\nWrite_MIDIDecOutputBuf: midi is stopped")
TRACE_MSG(MIDI_API_373_112_2_18_1_14_20_1082,"MIDI:BUSY!")
TRACE_MSG(MIDI_API_379_112_2_18_1_14_20_1083,"MIDI:FREE!")
TRACE_MSG(MIDI_SC_API_375_112_2_18_1_14_29_1084,"midi_sc_api.c:MIDI_SC_Init:error:PNULL==sound_bank")
TRACE_MSG(MIDI_SC_API_476_112_2_18_1_14_29_1085,"midi_sc_api.c:MIDI_SC_Decoder_Lib_Init:error:PNULL==pt_midi_proc_res")
TRACE_MSG(MIDI_SC_API_554_112_2_18_1_14_29_1086,"midi_sc_api.c, Init midi lib for L1")
TRACE_MSG(MIDI_SC_API_564_112_2_18_1_14_29_1087,"midi_sc_api.c, Init midi lib for L2/H3/W2")
TRACE_MSG(MIDI_SC_API_575_112_2_18_1_14_29_1088,"midi_sc_api Get IRAM Res Fail!")
TRACE_MSG(MIDI_SC_API_604_112_2_18_1_14_29_1089,"s_midi_ilock_handle is in unlocking.")
TRACE_MSG(MIDI_SC_API_616_112_2_18_1_14_29_1090,"\r\nMIDI_SC_DeInit:unable to unlock IRAM")
TRACE_MSG(MIDI_SC_API_652_112_2_18_1_14_29_1091,"midi_sc_api.c:MIDI_SC_Play:error:PNULL==midi_addr")
TRACE_MSG(MIDI_SC_API_681_112_2_18_1_14_29_1092,"Play midi time : %d\n")
TRACE_MSG(MIDI_SC_API_695_112_2_18_1_14_29_1093,"MIDI_SC_Stop: bIsMidiLibInited %d\n")
TRACE_MSG(MIDI_SC_API_809_112_2_18_1_14_29_1094,"MIDI_SC_GetPlayTime: Enter")
TRACE_MSG(MIDI_SC_API_815_112_2_18_1_14_29_1095,"midi_sc_api.c:MIDI_SC_GetPlayTime:error:PNULL==midi_addr")
TRACE_MSG(MIDI_SC_API_832_112_2_18_1_14_30_1096,"Scan midi time : %d, Midi Sample:%d")
TRACE_MSG(MIDI_SC_API_836_112_2_18_1_14_30_1097,"[MIDI_SC_GetPlayTime] error occours when scanniing the whole midi file!")
TRACE_MSG(MIDI_SC_API_1143_112_2_18_1_14_30_1098,"midi_sc_api.c,[MIDI_SC_SetVibrator]%d,%d,%d")
TRACE_MSG(MIDI_ADP_580_112_2_18_1_14_32_1099,"midi_adp.c:MIDI_Construct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_601_112_2_18_1_14_32_1100,"ERROR midi no memory 1.")
TRACE_MSG(MIDI_ADP_607_112_2_18_1_14_32_1101,"MIDI_Construct memmory:%d.")
TRACE_MSG(MIDI_ADP_613_112_2_18_1_14_32_1102,"ERROR midi no memory 2.")
TRACE_MSG(MIDI_ADP_619_112_2_18_1_14_32_1103,"MIDI_Construct memmory:%d.")
TRACE_MSG(MIDI_ADP_625_112_2_18_1_14_32_1104,"ERROR midi no memory 3.")
TRACE_MSG(MIDI_ADP_631_112_2_18_1_14_32_1105,"MIDI_Construct memmory:%d.")
TRACE_MSG(MIDI_ADP_637_112_2_18_1_14_32_1106,"ERROR midi no memory 4.")
TRACE_MSG(MIDI_ADP_643_112_2_18_1_14_32_1107,"MIDI_Construct memmory:%d.")
TRACE_MSG(MIDI_ADP_650_112_2_18_1_14_32_1108,"ERROR midi no memory 5.")
TRACE_MSG(MIDI_ADP_656_112_2_18_1_14_32_1109,"MIDI_Construct memmory:%d.")
TRACE_MSG(MIDI_ADP_664_112_2_18_1_14_32_1110,"ERROR midi no memory 6.")
TRACE_MSG(MIDI_ADP_670_112_2_18_1_14_32_1111,"MIDI_Construct memmory:%d.")
TRACE_MSG(MIDI_ADP_687_112_2_18_1_14_32_1112,"midi_adp.c:, Open(), midi construct failed to SCI_CreateMutex")
TRACE_MSG(MIDI_ADP_695_112_2_18_1_14_32_1113,"midi_adp.c:, Open(), midi construct failed to SCI_CreateMutex")
TRACE_MSG(MIDI_ADP_719_112_2_18_1_14_32_1114,"ERROR midi no memory 12.")
TRACE_MSG(MIDI_ADP_725_112_2_18_1_14_32_1115,"MIDI_Construct memmory:0x%x.")
TRACE_MSG(MIDI_ADP_737_112_2_18_1_14_32_1116,"Can't create midi")
TRACE_MSG(MIDI_ADP_768_112_2_18_1_14_32_1117,"ERROR midi no memory 13.")
TRACE_MSG(MIDI_ADP_774_112_2_18_1_14_32_1118,"MIDI_Construct memmory:%d.")
TRACE_MSG(MIDI_ADP_783_112_2_18_1_14_32_1119,"MIDI_Construct memmory:%d.")
TRACE_MSG(MIDI_ADP_787_112_2_18_1_14_32_1120,"ERROR midi no memory 14.")
TRACE_MSG(MIDI_ADP_798_112_2_18_1_14_32_1121,"midi_adp.c [MIDI_Construct] psPcmL:0x%x,len:%d(bytes).")
TRACE_MSG(MIDI_ADP_802_112_2_18_1_14_32_1122,"midi_adp.c [MIDI_Construct] psPcmR:0x%x.")
TRACE_MSG(MIDI_ADP_809_112_2_18_1_14_32_1123,"ERROR midi no memory 15.! len:%d.")
TRACE_MSG(MIDI_ADP_819_112_2_18_1_14_32_1124,"midi_adp.c [MIDI_Construct] psExpProDataL:0x%x,len:0x%x(bytes).")
TRACE_MSG(MIDI_ADP_823_112_2_18_1_14_32_1125,"midi_adp.c [MIDI_Construct] psExpProDataR:0x%x.")
TRACE_MSG(MIDI_ADP_827_112_2_18_1_14_32_1126,"ERROR midi no memory  16! len:%d.")
TRACE_MSG(MIDI_ADP_854_112_2_18_1_14_32_1127,"midi_adp.c [MIDI_Construct]  create midi assistant dynamic task!")
TRACE_MSG(MIDI_ADP_858_112_2_18_1_14_32_1128,"midi_adp.c [MIDI_Construct]  can not create midi assistant dynamic task!")
TRACE_MSG(MIDI_ADP_908_112_2_18_1_14_32_1129,"midi_adp.c:MIDI_Deconstruct:error:ptMidiProcRes==PNULL")
TRACE_MSG(MIDI_ADP_968_112_2_18_1_14_33_1130,"midi_adp.c:MIDI_PlayConstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_994_112_2_18_1_14_33_1131,"midi_adp.c:MIDI_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL")
TRACE_MSG(MIDI_ADP_1000_112_2_18_1_14_33_1132,"midi_adp.c,[MIDI_PlayConstruct] CHNG_FREQ_SetArmClk()!")
TRACE_MSG(MIDI_ADP_1081_112_2_18_1_14_33_1133,"midi_adp.c:MIDI_ResumeConstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1097_112_2_18_1_14_33_1134,"midi_adp.c:MIDI_ResumeConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL")
TRACE_MSG(MIDI_ADP_1102_112_2_18_1_14_33_1135,"midi_adp.c,[MIDI_ResumeConstruct] CHNG_FREQ_SetArmClk()!")
TRACE_MSG(MIDI_ADP_1138_112_2_18_1_14_33_1136,"midi_adp.c:MIDI_PauseDeconstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1182_112_2_18_1_14_33_1137,"midi_adp.c:MIDI_StopDeconstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1189_112_2_18_1_14_33_1138,"midi_adp.c:MIDI_StopDeconstruct:error:ptMidiProcRes==PNULL")
TRACE_MSG(MIDI_ADP_1268_112_2_18_1_14_33_1139,"midi_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1295_112_2_18_1_14_33_1140,"midi_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1328_112_2_18_1_14_33_1141,"midi_adp.c:MIDI_Pause:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1356_112_2_18_1_14_33_1142,"midi_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1390_112_2_18_1_14_33_1143,"midi_adp.c:MIDI_Seek:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1395_112_2_18_1_14_33_1144,"midi_adp.c:MIDI_Seek:error:uiOffset > uiTotalDataLength")
TRACE_MSG(MIDI_ADP_1422_112_2_18_1_14_33_1145,"midi_adp.c:MIDI_SetSpeed:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1467_112_2_18_1_14_34_1146,"midi_adp.c:MIDI_IdentifyFormat:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1474_112_2_18_1_14_34_1147,"midi_adp.c:MIDI_IdentifyFormat:error:ptMidiProcRes==PNULL")
TRACE_MSG(MIDI_ADP_1483_112_2_18_1_14_34_1148,"midi_adp.c:MIDI_IdentifyFormat:error:PNULL == mididata")
TRACE_MSG(MIDI_ADP_1519_112_2_18_1_14_34_1149,"midi_adp.c:Midi_IdentifyFormat:error:Unknown Handle!")
TRACE_MSG(MIDI_ADP_1567_112_2_18_1_14_34_1150,"[MIDI_IdentifyFormat] ptAudioObject->tAudioFormatInfo.uiTotalTime == 0!")
TRACE_MSG(MIDI_ADP_1653_112_2_18_1_14_34_1151,"MIDI_GetSampleRate outSamplerate:%d.")
TRACE_MSG(MIDI_ADP_1693_112_2_18_1_14_34_1152,"midi_adp.c:MIDI_SetPolyNum:error:audio_obj == SCI_NULL")
TRACE_MSG(MIDI_ADP_1701_112_2_18_1_14_34_1153,"midi_adp.c:MIDI_Set_Stereo: stereo  is %d")
TRACE_MSG(MIDI_ADP_1738_112_2_18_1_14_34_1154,"midi_adp.c:MIDI_Set_Stereo:unknown soundbank type!")
TRACE_MSG(MIDI_ADP_1772_112_2_18_1_14_34_1155,"midi_adp.c:MIDI_SetPolyNum:error:audio_obj == SCI_NULL")
TRACE_MSG(MIDI_ADP_1789_112_2_18_1_14_34_1156,"midi_adp.c:MIDI_SetPolyNum: poly number  is %d")
TRACE_MSG(MIDI_ADP_2238_112_2_18_1_14_35_1157,"midi_adp.c:MIDI_Set_Stereo:unknown soundbank type!")
TRACE_MSG(MIDI_ADP_2291_112_2_18_1_14_35_1158,"MIDI_Thread_Entry CODEC_SendSignal CODEC_STOP_REQ")
TRACE_MSG(MIDI_ADP_2352_112_2_18_1_14_35_1159,"MidiPlayFile ERROR found:%d.")
TRACE_MSG(MIDI_ADP_2424_112_2_18_1_14_35_1160,"offset(%d) is bigger than file_size(%d).")
TRACE_MSG(MIDI_ADP_2445_112_2_18_1_14_35_1161,"ERROR s_midi_data_buffer no memory.")
TRACE_MSG(MIDI_ADP_2458_112_2_18_1_14_35_1162,"ERROR FFS_Read, %d, %d, %d.")
TRACE_MSG(MIDI_ADP_2467_112_2_18_1_14_35_1163,"ERROR FFS_fseek, %d, %d.")
TRACE_MSG(MIDI_ADP_2478_112_2_18_1_14_35_1164,"midi not support streambuffer play.")
TRACE_MSG(MIDI_ADP_2483_112_2_18_1_14_36_1165,"MidiGetSrcData Invalid Handle Type.")
TRACE_MSG(MIDI_ADP_2514_112_2_18_1_14_36_1166,"midi_adp.c:Write_MIDIDecOutputBuf:error, l:0x%x, r:0x%x.")
TRACE_MSG(MIDI_ADP_2523_112_2_18_1_14_36_1167,"MIDI:BUSY!")
TRACE_MSG(MIDI_ADP_2530_112_2_18_1_14_36_1168,"MIDI:FREE!")
TRACE_MSG(MIDI_ADP_2543_112_2_18_1_14_36_1169,"MIDI:BUSY!")
TRACE_MSG(MIDI_ADP_2550_112_2_18_1_14_36_1170,"MIDI:FREE!")
TRACE_MSG(MIDI_ADP_2568_112_2_18_1_14_36_1171,"Write_MIDIDecOutputBuf 0 discard pcm data count:%d.")
TRACE_MSG(MIDI_ADP_2574_112_2_18_1_14_36_1172,"CODEC_SendSignal CODEC_ASSI_PLAY_REQ in write buf.")
TRACE_MSG(MIDI_ADP_2595_112_2_18_1_14_36_1173,"Write_MIDIDecOutputBuf 1 discard pcm data count:%d.")
TRACE_MSG(MIDI_ADP_2677_112_2_18_1_14_36_1174,"Write_MIDIDecOutputBuf 2 discard pcm data count:%d.")
TRACE_MSG(MIDI_ADP_2687_112_2_18_1_14_36_1175,"Write_MIDIDecOutputBuf dataLen ready to Dsp:%d")
TRACE_MSG(MIDI_ADP_2735_112_2_18_1_14_36_1176,"MIDIGetPlayTime: Enter")
TRACE_MSG(MIDI_ADP_2742_112_2_18_1_14_36_1177,"Scanmidi time :%d, play time:%dms")
TRACE_MSG(MIDI_ADP_2746_112_2_18_1_14_36_1178,"[MIDI_SC_GetPlayTime] error occours when scanniing the whole midi file!")
TRACE_MSG(MIDI_ADP_2800_112_2_18_1_14_36_1179,"midi_adp.c,[MIDI_VibratorCallback]%d,%d,%d")
TRACE_MSG(MIDI_ADP_2824_112_2_18_1_14_36_1180,"midi_adp.c: error::AUDIO_DEFAULT_TASK_ID == tAssiThreadId")
TRACE_MSG(MIDI_ADP_2838_112_2_18_1_14_36_1181,"midi_adp.c [CODEC_SendSignal]-0-  alloc failed!")
TRACE_MSG(MIDI_ADP_2854_112_2_18_1_14_36_1182,"midi_adp.c [CODEC_SendSignal]-01-  alloc failed!")
TRACE_MSG(MIDI_ADP_2870_112_2_18_1_14_36_1183,"midi_adp.c [CODEC_SendSignal]-1-  alloc failed!")
TRACE_MSG(MIDI_ADP_2886_112_2_18_1_14_36_1184,"midi_adp.c [CODEC_SendSignal]-2-  alloc failed!")
TRACE_MSG(MIDI_ADP_2902_112_2_18_1_14_36_1185,"midi_adp.c [CODEC_SendSignal]-9-  alloc failed!")
TRACE_MSG(MIDI_ADP_2910_112_2_18_1_14_36_1186,"midi_adp.c [CODEC_SendSignal]CODEC_ASSI_PLAY_REQ from 0x%x to 0x%x.")
TRACE_MSG(MIDI_ADP_2921_112_2_18_1_14_36_1187,"midi_adp.c [CODEC_SendSignal]-10-  alloc failed!")
TRACE_MSG(MIDI_ADP_2937_112_2_18_1_14_36_1188,"midi_adp.c [CODEC_SendSignal]-11-  alloc failed!")
TRACE_MSG(MIDI_ADP_2953_112_2_18_1_14_36_1189,"midi_adp.c [CODEC_SendSignal]-12-  alloc failed!")
TRACE_MSG(MIDI_ADP_2966_112_2_18_1_14_36_1190,"midi_adp.c  [CODEC_SendSignal]error eSigCode:0x%08x!")
TRACE_MSG(MIDI_ADP_3035_112_2_18_1_14_37_1191,"midi_adp.c[CODEC_AssiTask_DspExpPros] begin to dsp express process wav!")
TRACE_MSG(MIDI_ADP_3133_112_2_18_1_14_37_1192,"CODEC_ASSI_STOP_REQ force to exit from pausing.")
TRACE_MSG(MIDI_ADP_3146_112_2_18_1_14_37_1193,"midi_adp.c CODEC_AssiTask_DspExpPros midi decode has stopped.")
TRACE_MSG(MIDI_ADP_3156_112_2_18_1_14_37_1194,"midi_adp.c CODEC_AssiTask_DspExpPros midi decode exit from pausing state.")
TRACE_MSG(MIDI_ADP_3165_112_2_18_1_14_37_1195,"midi_adp.c CODEC_AssiTask_DspExpPros MIDI_SC_Stop.")
TRACE_MSG(MIDI_ADP_3173_112_2_18_1_14_37_1196,"midi_adp.c CODEC_AssiTask_DspExpPros exit.")
TRACE_MSG(MIDI_ADP_3182_112_2_18_1_14_37_1197,"midi_adp.c CODEC_AssiTask_DspExpPros stop waiting %d.")
TRACE_MSG(MIDI_ADP_3188_112_2_18_1_14_37_1198,"CODEC_ASSI_STOP_REQ force to exit from stopping.")
TRACE_MSG(MIDI_ADP_3203_112_2_18_1_14_37_1199,"CODEC_ASSI_PLAY_REQ receive again sender:0x%x state:%d.")
TRACE_MSG(MIDI_ADP_3236_112_2_18_1_14_37_1200,"CODEC_ASSI_STOP_REQ wait to get dsp ind too long.")
TRACE_MSG(MIDI_ADP_3252_112_2_18_1_14_37_1201,"midi decode task %d.")
TRACE_MSG(MIDI_ADP_3270_112_2_18_1_14_37_1202,"CODEC_ASSI_DSP_EXP_PRO_REQ wait %d.")
TRACE_MSG(MIDI_ADP_3277_112_2_18_1_14_37_1203,"CODEC_ASSI_DSP_EXP_PRO_REQ wait to get pcm data too long.")
TRACE_MSG(MIDI_ADP_3445_112_2_18_1_14_37_1204,"CODEC_ASSI_DSP_EXP_PRO_REQ sample:%d, src samplerate:%d, time:%d.")
TRACE_MSG(MIDI_ADP_3456_112_2_18_1_14_37_1205,"CODEC_ASSI_DSP_EXP_PRO_REQ exit from tran of dsp data process.")
TRACE_MSG(MIDI_ADP_3461_112_2_18_1_14_37_1206,"CODEC_ASSI_DSP_EXP_PRO_REQ exit from sbc or tran of dsp data process.")
TRACE_MSG(MIDI_ADP_3471_112_2_18_1_14_37_1207,"midi decode task %d.")
TRACE_MSG(MIDI_ADP_3488_112_2_18_1_14_37_1208,"CODEC_ASSI_DSP_EXP_PRO_IND sample:%d, time:%d.")
TRACE_MSG(MIDI_ADP_3523_112_2_18_1_14_38_1209,"midi_adp.c [CODEC_AssiTask_DspExpPros]other invalid signal code:%d!")
TRACE_MSG(MIDI_OPT_ADP_339_112_2_18_1_14_39_1210,"MidiPlayFile ERROR MIDI_SC_Play:%d.")
TRACE_MSG(MIDI_OPT_ADP_374_112_2_18_1_14_39_1211,"MIDI_Thread_Entry CODEC_SendSignal CODEC_STOP_REQ")
TRACE_MSG(MIDI_OPT_ADP_425_112_2_18_1_14_39_1212,"Write_MIDIDecOutputBuf 0 discard pcm data count:%d.")
TRACE_MSG(MIDI_OPT_ADP_431_112_2_18_1_14_39_1213,"CODEC_SendSignal CODEC_ASSI_PLAY_REQ in write buf.")
TRACE_MSG(MIDI_OPT_ADP_452_112_2_18_1_14_39_1214,"Write_MIDIDecOutputBuf 1 discard pcm data count:%d.")
TRACE_MSG(MIDI_OPT_ADP_534_112_2_18_1_14_39_1215,"Write_MIDIDecOutputBuf 2 discard pcm data count:%d.")
TRACE_MSG(MIDI_OPT_ADP_544_112_2_18_1_14_39_1216,"Write_MIDIDecOutputBuf dataLen ready to Dsp:%d")
TRACE_MSG(MIDI_OPT_ADP_608_112_2_18_1_14_39_1217,"offset(%d) is bigger than file_size(%d).")
TRACE_MSG(MIDI_OPT_ADP_629_112_2_18_1_14_40_1218,"ERROR s_midi_data_buffer no memory.")
TRACE_MSG(MIDI_OPT_ADP_646_112_2_18_1_14_40_1219,"ERROR FFS_Read, %d, %d, %d.")
TRACE_MSG(MIDI_OPT_ADP_655_112_2_18_1_14_40_1220,"ERROR FFS_fseek, %d, %d.")
TRACE_MSG(MIDI_OPT_ADP_666_112_2_18_1_14_40_1221,"midi not support streambuffer play.")
TRACE_MSG(MIDI_OPT_ADP_671_112_2_18_1_14_40_1222,"MidiGetSrcData Invalid Handle Type %d.")
TRACE_MSG(MIDI_OPT_ADP_701_112_2_18_1_14_40_1223,"midi_opt_adp.c:MIDI_Construct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_OPT_ADP_721_112_2_18_1_14_40_1224,"ERROR midi no memory 1.")
TRACE_MSG(MIDI_OPT_ADP_727_112_2_18_1_14_40_1225,"midi_opt_adp.c MIDI_Construct allocLen:%d.")
TRACE_MSG(MIDI_OPT_ADP_734_112_2_18_1_14_40_1226,"ERROR midi no memory 2.")
TRACE_MSG(MIDI_OPT_ADP_740_112_2_18_1_14_40_1227,"midi_opt_adp.c MIDI_Construct allocLen:%d.")
TRACE_MSG(MIDI_OPT_ADP_747_112_2_18_1_14_40_1228,"ERROR midi no memory 3.")
TRACE_MSG(MIDI_OPT_ADP_753_112_2_18_1_14_40_1229,"midi_opt_adp.c MIDI_Construct allocLen:%d.")
TRACE_MSG(MIDI_OPT_ADP_760_112_2_18_1_14_40_1230,"ERROR midi no memory 4.")
TRACE_MSG(MIDI_OPT_ADP_766_112_2_18_1_14_40_1231,"midi_opt_adp.c MIDI_Construct allocLen:%d.")
TRACE_MSG(MIDI_OPT_ADP_773_112_2_18_1_14_40_1232,"ERROR midi no memory 5.")
TRACE_MSG(MIDI_OPT_ADP_779_112_2_18_1_14_40_1233,"midi_opt_adp.c MIDI_Construct allocLen:%d.")
TRACE_MSG(MIDI_OPT_ADP_786_112_2_18_1_14_40_1234,"ERROR midi no memory 6.")
TRACE_MSG(MIDI_OPT_ADP_792_112_2_18_1_14_40_1235,"midi_opt_adp.c MIDI_Construct allocLen:%d.")
TRACE_MSG(MIDI_OPT_ADP_804_112_2_18_1_14_40_1236,"midi_opt_adp.c MIDI_Construct mode:%d.")
TRACE_MSG(MIDI_OPT_ADP_811_112_2_18_1_14_40_1237,"midi_opt_adp.c MIDI_Construct alloc failed, size:%d.")
TRACE_MSG(MIDI_OPT_ADP_817_112_2_18_1_14_40_1238,"ERROR midi failed to get mode param.")
TRACE_MSG(MIDI_OPT_ADP_848_112_2_18_1_14_40_1239,"midi_opt_adp.c,[MIDI_Construct]:midi_control:0x%x,samplerate:%d,polynum:%d")
TRACE_MSG(MIDI_OPT_ADP_886_112_2_18_1_14_40_1240,"midi_opt_adp.c MIDI_Construct allocLen:%d.")
TRACE_MSG(MIDI_OPT_ADP_890_112_2_18_1_14_40_1241,"ERROR midi no memory 7.")
TRACE_MSG(MIDI_OPT_ADP_900_112_2_18_1_14_40_1242,"midi_opt_adp.c MIDI_Construct allocLen:%d.")
TRACE_MSG(MIDI_OPT_ADP_902_112_2_18_1_14_40_1243,"midi_opt_adp.c [MIDI_Construct] psPcmL:0x%x,len:%d(bytes).")
TRACE_MSG(MIDI_OPT_ADP_906_112_2_18_1_14_40_1244,"midi_opt_adp.c [MIDI_Construct] psPcmR:0x%x.")
TRACE_MSG(MIDI_OPT_ADP_913_112_2_18_1_14_40_1245,"ERROR midi no memory 8.! len:%d.")
TRACE_MSG(MIDI_OPT_ADP_922_112_2_18_1_14_40_1246,"midi_opt_adp.c MIDI_Construct allocLen:%d.")
TRACE_MSG(MIDI_OPT_ADP_924_112_2_18_1_14_40_1247,"midi_opt_adp.c [MIDI_Construct] psExpProDataL:0x%x,len:0x%x(bytes).")
TRACE_MSG(MIDI_OPT_ADP_928_112_2_18_1_14_40_1248,"midi_opt_adp.c [MIDI_Construct] psExpProDataR:0x%x.")
TRACE_MSG(MIDI_OPT_ADP_932_112_2_18_1_14_40_1249,"ERROR midi no memory  9! len:%d.")
TRACE_MSG(MIDI_OPT_ADP_959_112_2_18_1_14_40_1250,"midi_opt_adp.c [MIDI_Construct]  create midi assistant dynamic task!")
TRACE_MSG(MIDI_OPT_ADP_963_112_2_18_1_14_40_1251,"midi_opt_adp.c [MIDI_Construct]  can not create midi assistant dynamic task!")
TRACE_MSG(MIDI_OPT_ADP_1015_112_2_18_1_14_40_1252,"midi_opt_adp.c:MIDI_Deconstruct:error:ptMidiProcRes==PNULL")
TRACE_MSG(MIDI_OPT_ADP_1075_112_2_18_1_14_40_1253,"midi_opt_adp.c:MIDI_PlayConstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_OPT_ADP_1088_112_2_18_1_14_40_1254,"midi_opt_adp.c:MIDI_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_midi_chng_freq_req_handler")
TRACE_MSG(MIDI_OPT_ADP_1094_112_2_18_1_14_40_1255,"midi_opt_adp.c,[MIDI_PlayConstruct] CHNG_FREQ_SetArmClk()!")
TRACE_MSG(MIDI_OPT_ADP_1139_112_2_18_1_14_41_1256,"midi_opt_adp.c,[MIDI_PlayConstruct]:MIDI_OPT_Init return error!i_status:%d")
TRACE_MSG(MIDI_OPT_ADP_1160_112_2_18_1_14_41_1257,"midi_opt_adp.c:MIDI_ResumeConstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_OPT_ADP_1178_112_2_18_1_14_41_1258,"midi_opt_adp.c:MIDI_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_midi_chng_freq_req_handler")
TRACE_MSG(MIDI_OPT_ADP_1186_112_2_18_1_14_41_1259,"midi_opt_adp.c,[MIDI_ResumeConstruct] CHNG_FREQ_SetArmClk()!")
TRACE_MSG(MIDI_OPT_ADP_1221_112_2_18_1_14_41_1260,"midi_opt_adp.c:MIDI_PauseDeconstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_OPT_ADP_1255_112_2_18_1_14_41_1261,"midi_opt_adp.c:MIDI_StopDeconstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_OPT_ADP_1320_112_2_18_1_14_41_1262,"midi_opt_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_OPT_ADP_1348_112_2_18_1_14_41_1263,"midi_opt_adp.c:MIDI_Pause:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_OPT_ADP_1374_112_2_18_1_14_41_1264,"midi_opt_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_OPT_ADP_1398_112_2_18_1_14_41_1265,"midi_opt_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_OPT_ADP_1441_112_2_18_1_14_41_1266,"midi_opt_adp.c:MIDI_IdentifyFormat:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_OPT_ADP_1450_112_2_18_1_14_41_1267,"midi_opt_adp.c:MIDI_IdentifyFormat:error:PNULL == mididata")
TRACE_MSG(MIDI_OPT_ADP_1486_112_2_18_1_14_41_1268,"midi_opt_adp.c:Midi_IdentifyFormat:error:Unknown Handle!")
TRACE_MSG(MIDI_OPT_ADP_1534_112_2_18_1_14_41_1269,"MIDI_IdentifyFormat: Pre checking ERROR! track %d is out of range")
TRACE_MSG(MIDI_OPT_ADP_1541_112_2_18_1_14_41_1270,"MIDI_IdentifyFormat: Track %d ERROR. MTrk not found")
TRACE_MSG(MIDI_OPT_ADP_1559_112_2_18_1_14_41_1271,"MIDI_IdentifyFormat: ERROR! track %d is out of range")
TRACE_MSG(MIDI_OPT_ADP_1569_112_2_18_1_14_41_1272,"[MIDI_IdentifyFormat] ptAudioObject->tAudioFormatInfo.uiTotalTime == 0!")
TRACE_MSG(MIDI_OPT_ADP_1573_112_2_18_1_14_41_1273,"[MIDI_IdentifyFormat] ptAudioObject->tAudioFormatInfo.uiTotalTime return error!")
TRACE_MSG(MIDI_OPT_ADP_1680_112_2_18_1_14_42_1274,"MIDI_SetSampleRate outputSamplerate:%d, srcSampleRate:%d. ")
TRACE_MSG(MIDI_OPT_ADP_1696_112_2_18_1_14_42_1275,"midi_opt_adp.c MIDI_SetSampleRate midi doesn't support this sample rate:%d.")
TRACE_MSG(MIDI_OPT_ADP_1761_112_2_18_1_14_42_1276,"midi_opt_adp.c,[MIDI_SetCodecPara] para type:%d")
TRACE_MSG(MIDI_OPT_ADP_1782_112_2_18_1_14_42_1277,"midi_opt_adp.c,[MIDI_SetCodecPara]:invalid para type!")
TRACE_MSG(MIDI_OPT_ADP_1805_112_2_18_1_14_42_1278,"midi_opt_adp.c: error::AUDIO_DEFAULT_TASK_ID == tAssiThreadId")
TRACE_MSG(MIDI_OPT_ADP_1819_112_2_18_1_14_42_1279,"midi_opt_adp.c [CODEC_SendSignal]-0-  alloc failed!")
TRACE_MSG(MIDI_OPT_ADP_1835_112_2_18_1_14_42_1280,"midi_opt_adp.c [CODEC_SendSignal]-01-  alloc failed!")
TRACE_MSG(MIDI_OPT_ADP_1851_112_2_18_1_14_42_1281,"midi_opt_adp.c [CODEC_SendSignal]-1-  alloc failed!")
TRACE_MSG(MIDI_OPT_ADP_1867_112_2_18_1_14_42_1282,"midi_opt_adp.c [CODEC_SendSignal]-2-  alloc failed!")
TRACE_MSG(MIDI_OPT_ADP_1883_112_2_18_1_14_42_1283,"midi_opt_adp.c [CODEC_SendSignal]-9-  alloc failed!")
TRACE_MSG(MIDI_OPT_ADP_1891_112_2_18_1_14_42_1284,"midi_opt_adp.c [CODEC_SendSignal]CODEC_ASSI_PLAY_REQ from 0x%x to 0x%x.")
TRACE_MSG(MIDI_OPT_ADP_1902_112_2_18_1_14_42_1285,"midi_opt_adp.c [CODEC_SendSignal]-10-  alloc failed!")
TRACE_MSG(MIDI_OPT_ADP_1918_112_2_18_1_14_42_1286,"midi_opt_adp.c [CODEC_SendSignal]-11-  alloc failed!")
TRACE_MSG(MIDI_OPT_ADP_1934_112_2_18_1_14_42_1287,"midi_opt_adp.c [CODEC_SendSignal]-12-  alloc failed!")
TRACE_MSG(MIDI_OPT_ADP_1947_112_2_18_1_14_42_1288,"midi_opt_adp.c  [CODEC_SendSignal]error eSigCode:0x%08x!")
TRACE_MSG(MIDI_OPT_ADP_2016_112_2_18_1_14_42_1289,"midi_opt_adp.c[CODEC_AssiTask_DspExpPros] begin to dsp express process wav!")
TRACE_MSG(MIDI_OPT_ADP_2113_112_2_18_1_14_42_1290,"CODEC_ASSI_STOP_REQ force to exit from pausing.")
TRACE_MSG(MIDI_OPT_ADP_2126_112_2_18_1_14_42_1291,"midi_opt_adp.c CODEC_AssiTask_DspExpPros midi decode has stopped.")
TRACE_MSG(MIDI_OPT_ADP_2136_112_2_18_1_14_42_1292,"midi_opt_adp.c CODEC_AssiTask_DspExpPros midi decode exit from pausing state.")
TRACE_MSG(MIDI_OPT_ADP_2145_112_2_18_1_14_42_1293,"midi_opt_adp.c CODEC_AssiTask_DspExpPros MIDI_SC_Stop.")
TRACE_MSG(MIDI_OPT_ADP_2153_112_2_18_1_14_43_1294,"midi_opt_adp.c CODEC_AssiTask_DspExpPros exit.")
TRACE_MSG(MIDI_OPT_ADP_2162_112_2_18_1_14_43_1295,"midi_opt_adp.c CODEC_AssiTask_DspExpPros stop waiting %d.")
TRACE_MSG(MIDI_OPT_ADP_2168_112_2_18_1_14_43_1296,"CODEC_ASSI_STOP_REQ force to exit from stopping.")
TRACE_MSG(MIDI_OPT_ADP_2183_112_2_18_1_14_43_1297,"CODEC_ASSI_PLAY_REQ receive again sender:0x%x state:%d.")
TRACE_MSG(MIDI_OPT_ADP_2216_112_2_18_1_14_43_1298,"CODEC_ASSI_STOP_REQ wait to get dsp ind too long.")
TRACE_MSG(MIDI_OPT_ADP_2234_112_2_18_1_14_43_1299,"midi decode task %d.")
TRACE_MSG(MIDI_OPT_ADP_2251_112_2_18_1_14_43_1300,"CODEC_ASSI_DSP_EXP_PRO_REQ wait count:%d.")
TRACE_MSG(MIDI_OPT_ADP_2258_112_2_18_1_14_43_1301,"CODEC_ASSI_DSP_EXP_PRO_REQ wait to get pcm data too long.")
TRACE_MSG(MIDI_OPT_ADP_2426_112_2_18_1_14_43_1302,"CODEC_ASSI_DSP_EXP_PRO_REQ sample:%d, src samplerate:%d, time:%d.")
TRACE_MSG(MIDI_OPT_ADP_2437_112_2_18_1_14_43_1303,"CODEC_ASSI_DSP_EXP_PRO_REQ exit from tran of dsp data process.")
TRACE_MSG(MIDI_OPT_ADP_2442_112_2_18_1_14_43_1304,"CODEC_ASSI_DSP_EXP_PRO_REQ exit from sbc or tran of dsp data process.")
TRACE_MSG(MIDI_OPT_ADP_2452_112_2_18_1_14_43_1305,"midi decode task %d.")
TRACE_MSG(MIDI_OPT_ADP_2469_112_2_18_1_14_43_1306,"CODEC_ASSI_DSP_EXP_PRO_IND sample:%d, time:%d.")
TRACE_MSG(MIDI_OPT_ADP_2504_112_2_18_1_14_43_1307,"midi_opt_adp.c [CODEC_AssiTask_DspExpPros]other invalid signal code:%d!")
TRACE_MSG(MIDI_OPT_API_59_112_2_18_1_14_44_1308,"midi_opt_api.c,[MIDI_OPT_SetVibrator]%d,%d,%d")
TRACE_MSG(MIDI_OPT_API_114_112_2_18_1_14_44_1309,"MIDI_OPT_Init\n stereoMode:%d")
TRACE_MSG(MIDI_OPT_API_115_112_2_18_1_14_44_1310,"\n splayRate:%d, spolyNum:%d")
TRACE_MSG(MIDI_OPT_API_116_112_2_18_1_14_44_1311,"\n speed:%d")
TRACE_MSG(MIDI_OPT_API_117_112_2_18_1_14_44_1312,"\n eWorkingMode:%d")
TRACE_MSG(MIDI_OPT_API_118_112_2_18_1_14_44_1313,"\n psMelody:%d psDrum:%d")
TRACE_MSG(MIDI_OPT_API_119_112_2_18_1_14_44_1314,"\n noteSift:%d")
TRACE_MSG(MIDI_OPT_API_120_112_2_18_1_14_44_1315,"\n sdbHalfSamplerate:%d,sdbAlaw8bits:%d")
TRACE_MSG(MIDI_OPT_API_191_112_2_18_1_14_44_1316,"MIDI_SC_GetPlayTime: Enter")
TRACE_MSG(MIDI_OPT_API_197_112_2_18_1_14_44_1317,"midi_sc_api.c:MIDI_SC_GetPlayTime:error:PNULL==midi_addr")
TRACE_MSG(MIDI_OPT_API_209_112_2_18_1_14_44_1318,"Scan midi time : %d, Midi Sample:%d")
TRACE_MSG(MIDI_OPT_API_213_112_2_18_1_14_44_1319,"[MIDI_SC_GetPlayTime] error occours when scanniing the whole midi file!")
TRACE_MSG(MIDI_OPT_API_235_112_2_18_1_14_44_1320,"midi_sc_api.c:MIDI_SC_Play:error:PNULL==midi_addr")
TRACE_MSG(MIDI_OPT_API_241_112_2_18_1_14_44_1321,"[MIDI_OPT_Play]:%d")
TRACE_MSG(MIDI_OPT_API_262_112_2_18_1_14_44_1322,"Play midi time : %d,i_status:%d")
TRACE_MSG(MIDI_ADP_382_112_2_18_1_14_57_1323,"peter:midi..MIDI_SC_Decoder_Lib_Init......start ..")
TRACE_MSG(MIDI_ADP_393_112_2_18_1_14_57_1324,"peter:MIDI_SC_Decoder_Lib_Init address is %x, length is %x")
TRACE_MSG(MIDI_ADP_400_112_2_18_1_14_57_1325,"peter:midi..MIDI_SC_Decoder_Lib_Init......end ..")
TRACE_MSG(MIDI_ADP_403_112_2_18_1_14_57_1326,"midi_sc_api Get IRAM Res Fail!")
TRACE_MSG(MIDI_ADP_420_112_2_18_1_14_57_1327,"peter:midi..MIDI_SC_Decoder_Lib_Deinit......start ..")
TRACE_MSG(MIDI_ADP_423_112_2_18_1_14_57_1328,"s_midi_ilock_handle is in unlocking.")
TRACE_MSG(MIDI_ADP_437_112_2_18_1_14_57_1329,"peter:midi..MIDI_SC_Decoder_Lib_Deinit......end ..")
TRACE_MSG(MIDI_ADP_441_112_2_18_1_14_57_1330,"MIDI_SC_DeInit:unable to unlock IRAM")
TRACE_MSG(MIDI_ADP_571_112_2_18_1_14_58_1331,"MIDI_SendMessage: to TASK %d, wait for event %d")
TRACE_MSG(MIDI_ADP_577_112_2_18_1_14_58_1332,"MIDI_SendMessage: TASK resp OK %d")
TRACE_MSG(MIDI_ADP_619_112_2_18_1_14_58_1333,"midi_adp.c:MIDI_Construct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_714_112_2_18_1_14_58_1334,"midi lib internal error")
TRACE_MSG(MIDI_ADP_728_112_2_18_1_14_58_1335,"Can't create midi mix buffer")
TRACE_MSG(MIDI_ADP_736_112_2_18_1_14_58_1336,"Can't create midi")
TRACE_MSG(MIDI_ADP_749_112_2_18_1_14_58_1337,"Bad midi file")
TRACE_MSG(MIDI_ADP_757_112_2_18_1_14_58_1338,"midi preparing failed")
TRACE_MSG(MIDI_ADP_775_112_2_18_1_14_58_1339,"Can't parse midi")
TRACE_MSG(MIDI_ADP_780_112_2_18_1_14_58_1340,"Midi parsed OK, playtime = %d.%03d")
TRACE_MSG(MIDI_ADP_787_112_2_18_1_14_58_1341,"ERROR midi no memory 13.")
TRACE_MSG(MIDI_ADP_802_112_2_18_1_14_58_1342,"peter:midi:MIDI_Deconstruct start ")
TRACE_MSG(MIDI_ADP_813_112_2_18_1_14_58_1343,"midi_adp.c:MIDI_Deconstruct:error:ptMidiProcRes==PNULL")
TRACE_MSG(MIDI_ADP_823_112_2_18_1_14_58_1344,"peter:midi:MIDI_Deconstruct end ")
TRACE_MSG(MIDI_ADP_842_112_2_18_1_14_58_1345,"midi_adp.c:MIDI_PlayConstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_845_112_2_18_1_14_58_1346,"peter:midi:MIDI_PlayConstruct start ")
TRACE_MSG(MIDI_ADP_868_112_2_18_1_14_58_1347,"midi_adp.c:MIDI_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL")
TRACE_MSG(MIDI_ADP_874_112_2_18_1_14_58_1348,"midi_adp.c,[MIDI_PlayConstruct] CHNG_FREQ_SetArmClk()!")
TRACE_MSG(MIDI_ADP_886_112_2_18_1_14_58_1349,"peter:MIDI_Core_GetContextSize start")
TRACE_MSG(MIDI_ADP_909_112_2_18_1_14_58_1350,"midi_adp.c:midi_playconstruct: task_id_get faild ")
TRACE_MSG(MIDI_ADP_938_112_2_18_1_14_58_1351,"peter:midi:MIDI_PlayConstruct end ")
TRACE_MSG(MIDI_ADP_982_112_2_18_1_14_59_1352,"midi_adp.c:MIDI_ResumeConstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1007_112_2_18_1_14_59_1353,"midi_adp.c:MIDI_ResumeConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL")
TRACE_MSG(MIDI_ADP_1014_112_2_18_1_14_59_1354,"midi_adp.c,[MIDI_ResumeConstruct] CHNG_FREQ_SetArmClk()!")
TRACE_MSG(MIDI_ADP_1034_112_2_18_1_14_59_1355,"midi_adp.c:MIDI_PauseDeconstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1041_112_2_18_1_14_59_1356,"peter:MidiProtectContext")
TRACE_MSG(MIDI_ADP_1082_112_2_18_1_14_59_1357,"midi_adp.c:MIDI_StopDeconstruct:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1089_112_2_18_1_14_59_1358,"midi_adp.c:MIDI_StopDeconstruct:error:ptMidiProcRes==PNULL")
TRACE_MSG(MIDI_ADP_1100_112_2_18_1_14_59_1359,"midi_adp.c:MIDI_StopDeconstruct:suspend & kill thread")
TRACE_MSG(MIDI_ADP_1154_112_2_18_1_14_59_1360,"peter:midi:MIDI_Play start ")
TRACE_MSG(MIDI_ADP_1192_112_2_18_1_14_59_1361,"midi_adp.c:MIDI_Stop:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1196_112_2_18_1_14_59_1362,"peter:midi:MIDI_Stop start ")
TRACE_MSG(MIDI_ADP_1203_112_2_18_1_14_59_1363,"midi_adp.c:MIDI_Stop: resume task")
TRACE_MSG(MIDI_ADP_1231_112_2_18_1_14_59_1364,"midi_adp.c:MIDI_Pause:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1239_112_2_18_1_14_59_1365,"MIDI_Pause: suspend task")
TRACE_MSG(MIDI_ADP_1262_112_2_18_1_14_59_1366,"midi_adp.c:MIDI_Pause:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1271_112_2_18_1_14_59_1367,"midi_adp.c:MIDI_Resume: resume task")
TRACE_MSG(MIDI_ADP_1292_112_2_18_1_14_59_1368,"midi_adp.c:MIDI_TimeSeek:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1297_112_2_18_1_14_59_1369,"midi_adp.c:MIDI_TimeSeek:time=%d, state=%d")
TRACE_MSG(MIDI_ADP_1328_112_2_18_1_14_59_1370,"midi_adp.c:MIDI_Seek:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1334_112_2_18_1_14_59_1371,"midi_adp.c:MIDI_Seek:off=%d,len=%d")
TRACE_MSG(MIDI_ADP_1357_112_2_18_1_14_59_1372,"midi_adp.c:MIDI_SetSpeed:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1372_112_2_18_1_14_59_1373,"midi_adp.c:MIDI_SetSpeed speed=%d, eas_speed=0x%08x\n")
TRACE_MSG(MIDI_ADP_1406_112_2_18_1_14_59_1374,"midi_adp.c:MIDI_IdentifyFormat:error:SCI_NULL == ptAudioObject")
TRACE_MSG(MIDI_ADP_1413_112_2_18_1_14_59_1375,"midi_adp.c:MIDI_IdentifyFormat:error:ptMidiProcRes==PNULL")
TRACE_MSG(MIDI_ADP_1430_112_2_18_1_14_59_1376,"midi_adp.c:Midi_IdentifyFormat:error:Unknown Handle!")
TRACE_MSG(MIDI_ADP_1438_112_2_18_1_14_59_1377,"[MIDI_IdentifyFormat] ptAudioObject->tAudioFormatInfo.uiTotalTime == 0!")
TRACE_MSG(MIDI_ADP_1474_112_2_18_1_15_0_1378,"MIDI_GetPlayingInfo: CurTime:%d")
TRACE_MSG(MIDI_ADP_1493_112_2_18_1_15_0_1379,"MIDI_GetPlayingInfo: DataOff:%d, TotalLen:%d, TotalTime:%d")
TRACE_MSG(MIDI_ADP_1548_112_2_18_1_15_0_1380,"midi_adp.c:MIDI_SetPolyNum:error:audio_obj == SCI_NULL")
TRACE_MSG(MIDI_ADP_1556_112_2_18_1_15_0_1381,"midi_adp.c:MIDI_SetPolyNum %d")
TRACE_MSG(MIDI_ADP_1666_112_2_18_1_15_0_1382,"para error in MIDI_SetLpfFilter.")
TRACE_MSG(MIDI_ADP_1697_112_2_18_1_15_0_1383,"para error in MIDI_SetLpfCutCentFreq.")
TRACE_MSG(MIDI_ADP_1730_112_2_18_1_15_0_1384,"para error in MIDI_SetEQFilter.")
TRACE_MSG(MIDI_ADP_1773_112_2_18_1_15_0_1385,"para error in MIDI_SetLCFFilter.")
TRACE_MSG(MIDI_ADP_1812_112_2_18_1_15_0_1386,"para error in MIDI_SetWorkingMode.")
TRACE_MSG(MIDI_ADP_1869_112_2_18_1_15_0_1387,"MIDITASK: get signal from[%08X], cmd=%d, param=%d, event %d, object %08X, thread id %08X")
TRACE_MSG(MIDI_ADP_1994_112_2_18_1_15_1_1388,"MIDI_Thread_Entry Quit")
TRACE_MSG(MIDI_ADP_2057_112_2_18_1_15_1_1389,"midi file_size is invalid: %d.")
TRACE_MSG(MIDI_ADP_2070_112_2_18_1_15_1_1390,"ERROR s_midi_data_buffer no memory.")
TRACE_MSG(MIDI_ADP_2082_112_2_18_1_15_1_1391,"ERROR FFS_Read, %d, %d.")
TRACE_MSG(MIDI_ADP_2092_112_2_18_1_15_1_1392,"ERROR FFS_fseek, %d.")
TRACE_MSG(MIDI_ADP_2103_112_2_18_1_15_1_1393,"midi not support streambuffer play.")
TRACE_MSG(MIDI_ADP_2108_112_2_18_1_15_1_1394,"MidiGetSrcData Invalid Handle Type.")
TRACE_MSG(MIDI_ADP_2155_112_2_18_1_15_1_1395,"midi_adp.c,[MIDI_VibratorCallback]%d")
TRACE_MSG(EAS_REPORT_153_112_2_18_1_15_7_1396,"%s")
TRACE_MSG(EAS_REPORT_158_112_2_18_1_15_7_1397,"Unrecognized error: Severity=%d; HashCode=%lu; SerialNum=%d")
TRACE_MSG(EAS_REPORT_181_112_2_18_1_15_7_1398,"%s")
TRACE_MSG(EAS_REPORT_204_112_2_18_1_15_7_1399,"%s")
TRACE_MSG(MP3_ADP_548_112_2_18_1_18_8_1400,"mp3_adp.c[MP3_RegCodecPlugger]register mp3 codec! codec handle is:0x%08x")
TRACE_MSG(MP3_ADP_576_112_2_18_1_18_8_1401,"mp3_adp.c [MP3_SetHalfSamplerate] s_mp3_halfsamplerate_on:%d")
TRACE_MSG(MP3_ADP_602_112_2_18_1_18_8_1402,"mp3_adp.c[MP3_Construct] construct resource!")
TRACE_MSG(MP3_ADP_609_112_2_18_1_18_8_1403,"mp3_adp.c:MP3_Construct:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_621_112_2_18_1_18_8_1404,"mp3_adp.c alloc len:%d.")
TRACE_MSG(MP3_ADP_627_112_2_18_1_18_8_1405,"mp3_adp.c [MP3_Construct] -1- alloc failed! len:%d.")
TRACE_MSG(MP3_ADP_640_112_2_18_1_18_9_1406,"mp3_adp.c alloc len:%d.")
TRACE_MSG(MP3_ADP_647_112_2_18_1_18_9_1407,"mp3_adp.c [MP3_Construct] -2- alloc failed! len:%d.")
TRACE_MSG(MP3_ADP_657_112_2_18_1_18_9_1408,"mp3_adp.c alloc len:%d.")
TRACE_MSG(MP3_ADP_664_112_2_18_1_18_9_1409,"mp3_adp.c [MP3_Construct] -3- alloc failed! len:%d.")
TRACE_MSG(MP3_ADP_683_112_2_18_1_18_9_1410,"mp3_adp.c alloc len:%d.")
TRACE_MSG(MP3_ADP_689_112_2_18_1_18_9_1411,"mp3_adp.c [MP3_Construct] -4- alloc failed! len:%d.")
TRACE_MSG(MP3_ADP_698_112_2_18_1_18_9_1412,"mp3_adp.c alloc len:%d.")
TRACE_MSG(MP3_ADP_717_112_2_18_1_18_9_1413,"mp3_adp.c [MP3_Construct] -5- alloc failed! len:%d.")
TRACE_MSG(MP3_ADP_726_112_2_18_1_18_9_1414,"mp3_adp.c alloc len:%d.")
TRACE_MSG(MP3_ADP_728_112_2_18_1_18_9_1415,"mp3_adp.c [MP3_Construct] -5.1- alloc swap_buff: %08X.")
TRACE_MSG(MP3_ADP_734_112_2_18_1_18_9_1416,"mp3_adp.c [MP3_Construct] -5.1- alloc swap_buff: %08X.")
TRACE_MSG(MP3_ADP_740_112_2_18_1_18_9_1417,"mp3_adp.c [MP3_Construct] -5.1- alloc failed! len:%d.")
TRACE_MSG(MP3_ADP_749_112_2_18_1_18_9_1418,"mp3_adp.c alloc len:%d.")
TRACE_MSG(MP3_ADP_755_112_2_18_1_18_9_1419,"mp3_adp.c [MP3_Construct] -6- alloc failed! len:%d.")
TRACE_MSG(MP3_ADP_782_112_2_18_1_18_9_1420,"mp3_adp.c[MP3_Construct] Failed to create mp3 dynamic task!")
TRACE_MSG(MP3_ADP_802_112_2_18_1_18_9_1421,"mp3_adp.c[MP3_Deconstruct] deconstruct resource!")
TRACE_MSG(MP3_ADP_809_112_2_18_1_18_9_1422,"mp3_adp.c:MP3_Deconstruct:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_869_112_2_18_1_18_9_1423,"mp3_adp.c[MP3_PlayConstruct] construct resource before play!")
TRACE_MSG(MP3_ADP_876_112_2_18_1_18_9_1424,"mp3_adp.c:MP3_PlayConstruct:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_889_112_2_18_1_18_9_1425,"mp3_adp.c[MP3_PlayConstruct] MP3_Decoder_Lib_Init error!")
TRACE_MSG(MP3_ADP_918_112_2_18_1_18_9_1426,"mp3_adp.c[MP3_PlayConstruct] create mp3 static task!")
TRACE_MSG(MP3_ADP_931_112_2_18_1_18_9_1427,"mp3_adp.c[MP3_PlayConstruct]:error: CHNG_FREQ_REQ_HANDLER_NULL==s_mp3_chng_freq_req_handler!")
TRACE_MSG(MP3_ADP_963_112_2_18_1_18_9_1428,"mp3_adp.c[MP3_StopDeconstruct] deconstruct resource after stop!")
TRACE_MSG(MP3_ADP_970_112_2_18_1_18_9_1429,"mp3_adp.c:MP3_PlayConstruct:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_978_112_2_18_1_18_9_1430,"mp3_adp.c:MP3_PlayConstruct:error:SCI_NULL==ptAudioObj->tCodecProcessResouce.other_info_ptr")
TRACE_MSG(MP3_ADP_1036_112_2_18_1_18_10_1431,"mp3_adp.c[MP3_StopDeconstruct] MP3_Decoder_Lib_Deinit error!")
TRACE_MSG(MP3_ADP_1054_112_2_18_1_18_10_1432,"mp3_adp.c[MP3_ResumeConstruct] construct resource before resume!")
TRACE_MSG(MP3_ADP_1061_112_2_18_1_18_10_1433,"mp3_adp.c:MP3_ResumeConstruct:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_1074_112_2_18_1_18_10_1434,"mp3_adp.c[MP3_ResumeConstruct] MP3_Decoder_Lib_Init error!")
TRACE_MSG(MP3_ADP_1114_112_2_18_1_18_10_1435,"mp3_adp.c[MP3_ResumeConstruct]:error: CHNG_FREQ_REQ_HANDLER_NULL==s_mp3_chng_freq_req_handler!")
TRACE_MSG(MP3_ADP_1144_112_2_18_1_18_10_1436,"mp3_adp.c[MP3_PauseDeconstruct] deconstruct resource after pause!")
TRACE_MSG(MP3_ADP_1151_112_2_18_1_18_10_1437,"mp3_adp.c:MP3_PauseDeconstruct:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_1159_112_2_18_1_18_10_1438,"mp3_adp.c:MP3_PauseDeconstruct:error:SCI_NULL==ptAudioObj->tCodecProcessResouce.other_info_ptr")
TRACE_MSG(MP3_ADP_1217_112_2_18_1_18_10_1439,"mp3_adp.c[MP3_PauseDeconstruct] MP3_Decoder_Lib_Deinit error!")
TRACE_MSG(MP3_ADP_1234_112_2_18_1_18_10_1440,"mp3_adp.c[MP3_Play] send MP3_PLAY_REQ to codec task!")
TRACE_MSG(MP3_ADP_1246_112_2_18_1_18_10_1441,"mp3_adp.c:MP3_Play:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_1252_112_2_18_1_18_10_1442,"mp3_adp.c[MP3_Play] AUDIO_DEFAULT_TASK_ID==ptAudioObj->tThreadId.")
TRACE_MSG(MP3_ADP_1272_112_2_18_1_18_10_1443,"mp3_adp.c[MP3_Stop] send MP3_STOP_REQ to codec task!")
TRACE_MSG(MP3_ADP_1285_112_2_18_1_18_10_1444,"mp3_adp.c:MP3_Stop:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_1292_112_2_18_1_18_10_1445,"mp3_adp.c,[MP3_Stop] task id is invalid !mp3 is stopped already!")
TRACE_MSG(MP3_ADP_1312_112_2_18_1_18_10_1446,"mp3_adp.c[MP3_Pause] send MP3_PAUSE_REQ to codec task!")
TRACE_MSG(MP3_ADP_1319_112_2_18_1_18_10_1447,"mp3_adp.c:MP3_Pause:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_1326_112_2_18_1_18_10_1448,"mp3_adp.c,[MP3_Pause] task id is invalid !mp3 is stopped already!")
TRACE_MSG(MP3_ADP_1345_112_2_18_1_18_10_1449,"mp3_adp.c[MP3_Resume] send MP3_RESUME_REQ to codec task!")
TRACE_MSG(MP3_ADP_1352_112_2_18_1_18_10_1450,"mp3_adp.c:MP3_Resume:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_1358_112_2_18_1_18_10_1451,"mp3_adp.c[MP3_Resume] AUDIO_DEFAULT_TASK_ID==ptAudioObj->tThreadId.")
TRACE_MSG(MP3_ADP_1382_112_2_18_1_18_11_1452,"mp3_adp.c[MP3_Seek] send MP3_SEEK_REQ to codec task!uiOffset:%d")
TRACE_MSG(MP3_ADP_1389_112_2_18_1_18_11_1453,"mp3_adp.c:MP3_Seek:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_1395_112_2_18_1_18_11_1454,"mp3_adp.c [MP3_Seek] AUDIO_DEFAULT_TASK_ID == ptAudioObj->tThreadId.")
TRACE_MSG(MP3_ADP_1475_112_2_18_1_18_11_1455,"mp3_adp.c[MP3_SendStreamData] send MP3_SRCPROC_REQ to codec task!")
TRACE_MSG(MP3_ADP_1482_112_2_18_1_18_11_1456,"mp3_adp.c:MP3_SendStreamData:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_1488_112_2_18_1_18_11_1457,"mp3_adp.c[MP3_SendStreamData] AUDIO_DEFAULT_TASK_ID==ptAudioObj->tThreadId.")
TRACE_MSG(MP3_ADP_1511_112_2_18_1_18_11_1458,"mp3_adp.c:MP3_IdentifyFormat:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_1519_112_2_18_1_18_11_1459,"mp3_adp.c[MP3_IdentifyFormat] identify format:result:0x%08x!")
TRACE_MSG(MP3_ADP_1546_112_2_18_1_18_11_1460,"mp3_adp.c:MP3_GetPlayingInfo:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_1557_112_2_18_1_18_11_1461,"mp3_adp.c[MP3_GetPlayingInfo]: error:SCI_NULL == mp3_ext_format_info_ptr")
TRACE_MSG(MP3_ADP_1565_112_2_18_1_18_11_1462,"mp3_adp.c[MP3_GetPlayingInfo]: error:SCI_NULL == mp3_ext_res_ptr")
TRACE_MSG(MP3_ADP_1611_112_2_18_1_18_11_1463,"mp3_adp.c[MP3_GetPlayingInfo] Total time is:0x%08x")
TRACE_MSG(MP3_ADP_1630_112_2_18_1_18_11_1464,"mp3_adp.c[MP3_GetPlayingInfo] cur_offset_temp_1:%d(cur_decode:%d, first_frame:%d), cur_offset_temp_2:%d.")
TRACE_MSG(MP3_ADP_1640_112_2_18_1_18_11_1465,"mp3_adp.c:MP3_GetPlayingInfo:error:(0 == mp3_format_info_ptr->uiBitrate)||(0 == ptAudioObj->uiOutputSamplerate)")
TRACE_MSG(MP3_ADP_1647_112_2_18_1_18_11_1466,"mp3_adp.c[MP3_GetPlayingInfo] cbr, cur_time_1:%d, cur_time_2:%d.")
TRACE_MSG(MP3_ADP_1657_112_2_18_1_18_11_1467,"mp3_adp.c:MP3_GetPlayingInfo:error:(0 == mp3_ext_format_info_ptr->uiMusicDataLen)||(0 == ptAudioObj->uiOutputSamplerate)")
TRACE_MSG(MP3_ADP_1688_112_2_18_1_18_11_1468,"mp3_adp.c[MP3_GetPlayingInfo] vbr, cur_time_1:%d, cur_time_2:%d.")
TRACE_MSG(MP3_ADP_1699_112_2_18_1_18_11_1469,"mp3_adp.c[MP3_GetPlayingInfo] exception: cur_time_1 < cur_time_2!")
TRACE_MSG(MP3_ADP_1735_112_2_18_1_18_11_1470,"mp3_adp.c[MP3_GetPlayingInfo], len--total:%d, cur:%d(byte), time--total:%d, cur:%d(s).")
TRACE_MSG(MP3_ADP_1757_112_2_18_1_18_11_1471,"mp3_adp.c[MP3_GetPlayingInfo] stream, %d should not less than:%d(=%d+%d), InfoList:%d, %d, %d, %d.")
TRACE_MSG(MP3_ADP_1768_112_2_18_1_18_11_1472,"mp3_adp.c: error: Unknown Handle Type!")
TRACE_MSG(MP3_ADP_1787_112_2_18_1_18_11_1473,"mp3_adp.c:MP3_GetOutputSampleRate:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_1825_112_2_18_1_18_11_1474,"mp3_adp.c:MP3_FreeAsPause:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_1835_112_2_18_1_18_11_1475,"mp3_adp.c [MP3_FreeAsPause]:error:ext_resouce_ptr==SCI_NULL")
TRACE_MSG(MP3_ADP_1900_112_2_18_1_18_12_1476,"mp3_adp.c:MP3_AllocAsResume:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_1911_112_2_18_1_18_12_1477,"mp3_adp.c [MP3_AllocAsResume]:error:ext_resouce_ptr==SCI_NULL")
TRACE_MSG(MP3_ADP_1922_112_2_18_1_18_12_1478,"mp3_adp.c alloc len:%d.")
TRACE_MSG(MP3_ADP_1928_112_2_18_1_18_12_1479,"mp3_adp.c [MP3_AllocAsResume] -1- alloc failed! len:%d.")
TRACE_MSG(MP3_ADP_1937_112_2_18_1_18_12_1480,"mp3_adp.c alloc len:%d.")
TRACE_MSG(MP3_ADP_1943_112_2_18_1_18_12_1481,"mp3_adp.c [MP3_AllocAsResume] -2- alloc failed! len:%d.")
TRACE_MSG(MP3_ADP_1963_112_2_18_1_18_12_1482,"mp3_adp.c alloc len:%d.")
TRACE_MSG(MP3_ADP_1968_112_2_18_1_18_12_1483,"mp3_adp.c [MP3_AllocAsResume] -3- alloc failed! len:%d.")
TRACE_MSG(MP3_ADP_1983_112_2_18_1_18_12_1484,"mp3_adp.c alloc len:%d.")
TRACE_MSG(MP3_ADP_1985_112_2_18_1_18_12_1485,"mp3_adp.c [MP3_AllocAsResume] -4- alloc swap_buff: %08X.")
TRACE_MSG(MP3_ADP_1991_112_2_18_1_18_12_1486,"mp3_adp.c [MP3_AllocAsResume] -4- alloc swap_buff: %08X.")
TRACE_MSG(MP3_ADP_1996_112_2_18_1_18_12_1487,"mp3_adp.c [MP3_AllocAsResume] -4- alloc failed! len:%d.")
TRACE_MSG(MP3_ADP_2008_112_2_18_1_18_12_1488,"mp3_adp.c alloc len:%d.")
TRACE_MSG(MP3_ADP_2013_112_2_18_1_18_12_1489,"mp3_adp.c [MP3_AllocAsResume] -5- alloc failed! len:%d.")
TRACE_MSG(MP3_ADP_2040_112_2_18_1_18_12_1490,"mp3_adp.c:MP3_FreeInConstruct:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_2051_112_2_18_1_18_12_1491,"mp3_adp.c [MP3_FreeInConstruct]:error:ext_resouce_ptr==SCI_NULL")
TRACE_MSG(MP3_ADP_2100_112_2_18_1_18_12_1492,"mp3_adp.c[MP3_SendSignal]AUDIO_DEFAULT_TASK_ID == tThreadId")
TRACE_MSG(MP3_ADP_2113_112_2_18_1_18_12_1493,"mp3_adp.c  [MP3_SendSignal] -1- alloc failed!")
TRACE_MSG(MP3_ADP_2135_112_2_18_1_18_12_1494,"mp3_adp.c  [MP3_SendSignal]-2- alloc failed!")
TRACE_MSG(MP3_ADP_2152_112_2_18_1_18_12_1495,"mp3_adp.c  [MP3_SendSignal]-3- alloc failed!")
TRACE_MSG(MP3_ADP_2168_112_2_18_1_18_12_1496,"mp3_adp.c  [MP3_SendSignal]-4- alloc failed!")
TRACE_MSG(MP3_ADP_2190_112_2_18_1_18_12_1497,"mp3_adp.c  [MP3_SendSignal]-5- alloc failed!")
TRACE_MSG(MP3_ADP_2213_112_2_18_1_18_12_1498,"mp3_adp.c  [MP3_SendSignal]-6- alloc failed!")
TRACE_MSG(MP3_ADP_2230_112_2_18_1_18_12_1499,"mp3_adp.c  [MP3_SendSignal]-7- alloc failed!")
TRACE_MSG(MP3_ADP_2246_112_2_18_1_18_12_1500,"mp3_adp.c  [MP3_SendSignal]-8- alloc failed!")
TRACE_MSG(MP3_ADP_2258_112_2_18_1_18_12_1501,"mp3_adp.c  [MP3_SendSignal]error eSigCode:0x%08x!")
TRACE_MSG(MP3_ADP_2286_112_2_18_1_18_12_1502,"MP3_Stream_CastOff_SrcDataInfo para error:%d, %d.")
TRACE_MSG(MP3_ADP_2299_112_2_18_1_18_12_1503,"MP3_Stream_CastOff_SrcDataInfo CastOffDataLen:%d, %d, %d.")
TRACE_MSG(MP3_ADP_2311_112_2_18_1_18_12_1504,"MP3_Stream_CastOff_SrcDataInfo samplerate:%d<%d.")
TRACE_MSG(MP3_ADP_2330_112_2_18_1_18_12_1505,"MP3_Stream_CastOff_SrcDataInfo 0 CastOffDataInfo:%d, %d, %d.")
TRACE_MSG(MP3_ADP_2348_112_2_18_1_18_12_1506,"MP3_Stream_CastOff_SrcDataInfo 1 CastOffDataInfo:%d, %d, %d.")
TRACE_MSG(MP3_ADP_2366_112_2_18_1_18_12_1507,"MP3_Stream_CastOff_SrcDataInfo 2 CastOffDataInfo:%d, %d, %d.")
TRACE_MSG(MP3_ADP_2391_112_2_18_1_18_12_1508,"MP3_Stream_CastOff_SrcDataInfo 3 CastOffDataInfo:%d, %d, %d, leftDataLen:%d.")
TRACE_MSG(MP3_ADP_2406_112_2_18_1_18_13_1509,"MP3_Stream_CastOff_SrcDataInfo realCastoff:%d, wantToCastOff:%d, lastPlayTime:%d.")
TRACE_MSG(MP3_ADP_2415_112_2_18_1_18_13_1510,"MP3_Stream_CastOff_SrcDataInfo, ListInfo:%d, %d, %d, %d.")
TRACE_MSG(MP3_ADP_2475_112_2_18_1_18_13_1511,"mp3_adp.c[MP3_Thread_Entry]  begin to decode mp3!")
TRACE_MSG(MP3_ADP_2503_112_2_18_1_18_13_1512,"mp3_adp.c[MP3_Thread_Entry] receive MP3_PLAY_REQ!decoder state:0x%08x")
TRACE_MSG(MP3_ADP_2577_112_2_18_1_18_13_1513,"MP3_Thread_Entry uiCurrentDecodePos:%d in MP3_PLAY_REQ.")
TRACE_MSG(MP3_ADP_2619_112_2_18_1_18_13_1514,"mp3_adp.c [MP3_Thread_Entry] receive MP3_STOP_REQ!decoder state:0x%08x")
TRACE_MSG(MP3_ADP_2623_112_2_18_1_18_13_1515,"mp3_adp.c [MP3_Thread_Entry] case MP3_STOP_REQ:the decoder is stopped already!")
TRACE_MSG(MP3_ADP_2654_112_2_18_1_18_13_1516,"mp3_adp.c [MP3_Thread_Entry] receive MP3_PAUSE_REQ!decoder state:0x%08x")
TRACE_MSG(MP3_ADP_2658_112_2_18_1_18_13_1517,"mp3_adp.c [MP3_Thread_Entry] the decoder is stopped already!")
TRACE_MSG(MP3_ADP_2703_112_2_18_1_18_13_1518,"mp3_adp.c[MP3_Thread_Entry] receive MP3_RESUME_REQ!decoder state:0x%08x,pausing offset:0x%08x")
TRACE_MSG(MP3_ADP_2711_112_2_18_1_18_13_1519,"mp3_adp.c[MP3_Thread_Entry] uiCurrentDecodePos:%d in MP3_RESUME_REQ.")
TRACE_MSG(MP3_ADP_2758_112_2_18_1_18_13_1520,"mp3_adp.c[MP3_Thread_Entry] receive MP3_SEEK_REQ!decoder state:0x%08x,dest_offset:0x%08x,uiFirstFrameOffset:%d")
TRACE_MSG(MP3_ADP_2763_112_2_18_1_18_13_1521,"mp3_adp.c [MP3_Thread_Entry] the decoder is stopped already!")
TRACE_MSG(MP3_ADP_2773_112_2_18_1_18_13_1522,"MP3_Thread_Entry uiCurrentDecodePos:%d in MP3_SEEK_REQ.")
TRACE_MSG(MP3_ADP_2800_112_2_18_1_18_13_1523,"mp3_adp.c[MP3_Thread_Entry] error happens in seek operation!")
TRACE_MSG(MP3_ADP_2811_112_2_18_1_18_13_1524,"mp3_adp.c[MP3_Thread_Entry] receive MP3_SRCPROC_REQ!decoder state:0x%08x")
TRACE_MSG(MP3_ADP_2815_112_2_18_1_18_13_1525,"mp3_adp.c [MP3_Thread_Entry]MP3_SRCPROC_REQ: the decoder is stopped already!")
TRACE_MSG(MP3_ADP_2831_112_2_18_1_18_13_1526,"mp3_adp.c[MP3_Thread_Entry] case MP3_SRCPROC_REQ:error:(MP3_DECODER_GETTINGSRC != mp3_ext_res_ptr->eDecoderState).......")
TRACE_MSG(MP3_ADP_2851_112_2_18_1_18_13_1527,"mp3_adp.c,[MP3_SRCPROC_REQ]stream: buf is not enough,curCount:%d!")
TRACE_MSG(MP3_ADP_2877_112_2_18_1_18_13_1528,"mp3_adp.c[MP3_Thread_Entry] case MP3_SRCPROC_REQ:stream!")
TRACE_MSG(MP3_ADP_2884_112_2_18_1_18_13_1529,"mp3_adp.c[MP3_Thread_Entry] case MP3_SRCPROC_REQ:end of music !")
TRACE_MSG(MP3_ADP_2895_112_2_18_1_18_13_1530,"mp3_adp.c[MP3_Thread_Entry] receive MP3_GETSRC_REQ!decoder state:0x%08x")
TRACE_MSG(MP3_ADP_2900_112_2_18_1_18_13_1531,"mp3_adp.c [MP3_Thread_Entry]MP3_GETSRC_REQ: the decoder is stopped already!")
TRACE_MSG(MP3_ADP_2956_112_2_18_1_18_14_1532,"mp3_adp.c[MP3_Thread_Entry] case MP3_GETSRC_REQ:mp3_get_src_count == 0! uiMusicDataLen:0, TotalLenInList:%d!")
TRACE_MSG(MP3_ADP_2971_112_2_18_1_18_14_1533,"mp3_adp.c[MP3_Thread_Entry] case MP3_GETSRC_REQ:mp3_get_src_count == 0! sleep and GoOnGetSrcData, %d, %d!")
TRACE_MSG(MP3_ADP_2978_112_2_18_1_18_14_1534,"mp3_adp.c[MP3_Thread_Entry] case MP3_GETSRC_REQ:mp3_get_src_count == 0! End of music!")
TRACE_MSG(MP3_ADP_2988_112_2_18_1_18_14_1535,"mp3_adp.c[MP3_Thread_Entry] case MP3_GETSRC_REQ:MP3_GET_SRC_WAIT:have to wait for a while and get src again!")
TRACE_MSG(MP3_ADP_2998_112_2_18_1_18_14_1536,"mp3_adp.c[MP3_Thread_Entry] case MP3_GETSRC_REQ:MP3_GET_SRC_ASY:have to wait for the MP3_SRCPROC_REQ!")
TRACE_MSG(MP3_ADP_3005_112_2_18_1_18_14_1537,"mp3_adp.c[MP3_Thread_Entry] case MP3_GETSRC_REQ:error happened when getting source!")
TRACE_MSG(MP3_ADP_3015_112_2_18_1_18_14_1538,"mp3_adp.c: Unknown Command!")
TRACE_MSG(MP3_ADP_3027_112_2_18_1_18_14_1539,"mp3_adp.c[MP3_Thread_Entry]case MP3_DECODE_REQ: the decoder state is MP3_DECODER_STOPPED")
TRACE_MSG(MP3_ADP_3033_112_2_18_1_18_14_1540,"mp3_adp.c [MP3_Thread_Entry]MP3_DECODE_REQ: no data left in the swap buffer!")
TRACE_MSG(MP3_ADP_3074_112_2_18_1_18_14_1541,"mp3_adp.c[MP3_Thread_Entry]  mp3 current decode pos:%d, ori_sam:%d.")
TRACE_MSG(MP3_ADP_3096_112_2_18_1_18_14_1542,"mp3_adp.c[MP3_Thread_Entry] MP3_DECODE_REQ:%d, %d, %d, %d, %d, bitrate:%d.")
TRACE_MSG(MP3_ADP_3115_112_2_18_1_18_14_1543,"mp3_adp.c[MP3_Thread_Entry] case MP3_DECODE_REQ End of music, streamList:%d, %d, %d, %d!")
TRACE_MSG(MP3_ADP_3122_112_2_18_1_18_14_1544,"mp3_adp.c[MP3_Thread_Entry] case MP3_DECODE_REQ End of music!")
TRACE_MSG(MP3_ADP_3134_112_2_18_1_18_14_1545,"mp3_adp.c[MP3_Thread_Entry] case MP3_DECODE_REQ End of music!")
TRACE_MSG(MP3_ADP_3150_112_2_18_1_18_14_1546,"mp3_adp.c[MP3_Thread_Entry] Lack MP3 data in MP3_ERROR_NONE frameLen:%d should less than:%d.")
TRACE_MSG(MP3_ADP_3200_112_2_18_1_18_14_1547,"mp3_adp.c[MP3_Thread_Entry] the no.0x%08x MP3_ERROR_BUFLEN in this song")
TRACE_MSG(MP3_ADP_3213_112_2_18_1_18_14_1548,"MP3_Thread_Entry uiCurrentDecodePos:%d after lossing %d (byte)")
TRACE_MSG(MP3_ADP_3244_112_2_18_1_18_14_1549,"MP3_Thread_Entry uiCurrentDecodePos:%d = src_offset after lossing %d(byte).")
TRACE_MSG(MP3_ADP_3267_112_2_18_1_18_14_1550,"mp3_adp.c[MP3_Thread_Entry] the no.0x%08x MP3_ERROR_FATAL in this song")
TRACE_MSG(MP3_ADP_3272_112_2_18_1_18_14_1551,"MP3_Thread_Entry uiCurrentDecodePos:%d in MP3_DECODE_REQ MP3_ERROR_FATAL.")
TRACE_MSG(MP3_ADP_3292_112_2_18_1_18_14_1552,"mp3_adp.c [MP3_Thread_Entry] unknown decode error happened!")
TRACE_MSG(MP3_ADP_3359_112_2_18_1_18_14_1553,"mp3_adp.c [MP3_GetSrcData] SCI_NULL == file_handle!")
TRACE_MSG(MP3_ADP_3387_112_2_18_1_18_14_1554,"mp3_adp.c [MP3_GetSrcData] input para error!0x%08x,0x%08x")
TRACE_MSG(MP3_ADP_3453_112_2_18_1_18_15_1555,"mp3_adp.c,[MP3_GetSrcData]stream:get_stream_data return result:%d,ptr:0x%08x,len:%d!")
TRACE_MSG(MP3_ADP_3465_112_2_18_1_18_15_1556,"mp3_adp.c,[MP3_GetSrcData]stream:End of music!")
TRACE_MSG(MP3_ADP_3487_112_2_18_1_18_15_1557,"mp3_adp.c,[MP3_GetSrcData]stream: buf is not enough,curCount:%d!")
TRACE_MSG(MP3_ADP_3504_112_2_18_1_18_15_1558,"mp3_adp.c,[MP3_GetSrcData]StreamList:%d, %d, %d, %d, thisLen:%d, thisStartTime:%d, totalLen:%d.")
TRACE_MSG(MP3_ADP_3541_112_2_18_1_18_15_1559,"mp3_adp.c,[MP3_GetSrcData]get_stream_data return AUDIO_STREAM_GET_SRC_WAIT!")
TRACE_MSG(MP3_ADP_3546_112_2_18_1_18_15_1560,"mp3_adp.c,[MP3_GetSrcData]get_stream_data return AUDIO_STREAM_GET_SRC_ASY!")
TRACE_MSG(MP3_ADP_3551_112_2_18_1_18_15_1561,"mp3_adp.c,[MP3_GetSrcData]:get_stream_data return AUDIO_STREAM_GET_SRC_ERROR!")
TRACE_MSG(MP3_ADP_3556_112_2_18_1_18_15_1562,"mp3_adp.c,[MP3_GetSrcData]:get_stream_data return result:%d!wrong type!")
TRACE_MSG(MP3_ADP_3566_112_2_18_1_18_15_1563,"mp3_adp.c,[MP3_GetSrcData]:wrong handle type!sub_type:%d")
TRACE_MSG(MP3_ADP_3589_112_2_18_1_18_15_1564,"mp3_adp.c:MP3_SeekAudio:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_ADP_3629_112_2_18_1_18_15_1565,"mp3_adp.c [MP3_SeekAudio] buffer seek:the position is larger than the total length!")
TRACE_MSG(MP3_ADP_3637_112_2_18_1_18_15_1566,"mp3_adp.c [MP3_SeekAudio] stream, reset uiMusicDataLen!")
TRACE_MSG(MP3_ADP_3642_112_2_18_1_18_15_1567,"mp3_adp.c [MP3_SeekAudio] Invalid handle type!")
TRACE_MSG(MP3_ADP_3692_112_2_18_1_18_15_1568,"mp3_adp.c [MP3_InitDecoder] out put sample rate :0x%08x")
TRACE_MSG(MP3_DECODE_393_112_2_18_1_18_16_1569,"\r\nMP3_Decoder_Lib_Deinit:unable to unlock IRAM")
TRACE_MSG(MP3_FFS_76_112_2_18_1_18_20_1570,"mp3_ffs.c,[MP3_ReadData] (0 == ptMp3Handle)||(SCI_NULL == puiBuf)||(SCI_NULL == puiDataGot)!")
TRACE_MSG(MP3_FFS_87_112_2_18_1_18_20_1571,"mp3_ffs.c,[MP3_ReadData] 0 == ptMp3Handle->unMp3data.fFileHandle!")
TRACE_MSG(MP3_FFS_94_112_2_18_1_18_20_1572,"mp3_ffs.c,[MP3_ReadData] read file encounters error!")
TRACE_MSG(MP3_FFS_112_112_2_18_1_18_20_1573,"mp3_ffs.c,[ MP3_ReadData]  (SCI_NULL == data_buffer)||(0 == data_len)||(cur_offset >data_len)")
TRACE_MSG(MP3_FFS_125_112_2_18_1_18_20_1574,"mp3_ffs.c,[MP3_ReadData]:Unknown Handle Type!")
TRACE_MSG(MP3_FFS_129_112_2_18_1_18_20_1575,"mp3_ffs.c,[MP3_ReadData]type:%d,uiLen:0x%08x,data got:0x%08x")
TRACE_MSG(MP3_FFS_150_112_2_18_1_18_20_1576,"mp3_ffs.c,[MP3_ReadData] SCI_NULL == ptMp3Handle!")
TRACE_MSG(MP3_FFS_161_112_2_18_1_18_20_1577,"mp3_ffs.c,[MP3_SetDataPointer] 0 == ptMp3Handle->unMp3data.fFileHandle!")
TRACE_MSG(MP3_FFS_167_112_2_18_1_18_20_1578,"mp3_ffs.c,[MP3_SetDataPointer] seek file encounters error!")
TRACE_MSG(MP3_FFS_187_112_2_18_1_18_21_1579,"mp3_ffs.c,[MP3_SetDataPointer]  (SCI_NULL == data_buffer)||(0 == data_len)||(cur_offset >data_len)")
TRACE_MSG(MP3_FFS_207_112_2_18_1_18_21_1580,"mp3_ffs.c,[MP3_SetDataPointer]  invalid BasePos!")
TRACE_MSG(MP3_FFS_213_112_2_18_1_18_21_1581,"mp3_ffs.c,[MP3_SetDataPointer]  buffer seek failed!dest_Offset:0x%x,data_len:0x%x.")
TRACE_MSG(MP3_FFS_223_112_2_18_1_18_21_1582,"mp3_ffs.c,[MP3_SetDataPointer]  Unknown Handle Type!")
TRACE_MSG(MP3_FFS_227_112_2_18_1_18_21_1583,"mp3_ffs.c,[MP3_SetDataPointer]:type:%d,eBasePos:%d,offset:0x%08x")
TRACE_MSG(MP3_FFS_260_112_2_18_1_18_21_1584,"mp3_ffs.c,[MP3_GetDataSize] get file size encounters error!")
TRACE_MSG(MP3_FFS_272_112_2_18_1_18_21_1585,"mp3_ffs.c:Unknown File Handle Type!")
TRACE_MSG(MP3_FFS_276_112_2_18_1_18_21_1586,"mp3_ffs.c,[MP3_GetDataSize]:type:%d,size:0x%08x")
TRACE_MSG(MP3_FFS_308_112_2_18_1_18_21_1587,"mp3_ffs.c,[MP3_GetDataPointer] get file data pointer encounters error!")
TRACE_MSG(MP3_FFS_326_112_2_18_1_18_21_1588,"mp3_ffs.c,[MP3_GetDataPointer]  (SCI_NULL == data_buffer)||(0 == data_len)||(cur_offset >data_len)")
TRACE_MSG(MP3_FFS_345_112_2_18_1_18_21_1589,"mp3_ffs.c,[MP3_GetDataPointer]  invalid BasePos!")
TRACE_MSG(MP3_FFS_354_112_2_18_1_18_21_1590,"mp3_ffs.c:MP3_GetDataPointer:Unknown File Handle Type!")
TRACE_MSG(MP3_FFS_358_112_2_18_1_18_21_1591,"mp3_ffs.c,[MP3_GetDataPointer]:type:%d,eBasePos:%d,cur pos:0x%08x")
TRACE_MSG(MP3_FORMAT_587_112_2_18_1_18_23_1592,"mp3_format.c:MP3_CaculateNum:error:(puiBuf = SCI_NULL)||(puiNum= SCI_NULL)")
TRACE_MSG(MP3_FORMAT_608_112_2_18_1_18_23_1593,"mp3_format.c,[MP3_CaculateNum]:i_calc:%d!not number!")
TRACE_MSG(MP3_FORMAT_781_112_2_18_1_18_23_1594,"mp3_format.c:MP3_CheckXingHeader:error:(SCI_NULL == ptMp3Handle)||(SCI_NULL == ptXingHeaderInfo)...")
TRACE_MSG(MP3_FORMAT_913_112_2_18_1_18_23_1595,"mp3_format.c:MP3_CheckID3V1Tag:error:(SCI_NULL == ptMp3Handle)||(SCI_NULL == ptXingHeaderInfo)...")
TRACE_MSG(MP3_FORMAT_964_112_2_18_1_18_23_1596,"mp3_format.c [MP3_CheckID3V1Tag] V1.1:no valid year info!")
TRACE_MSG(MP3_FORMAT_981_112_2_18_1_18_23_1597,"mp3_format.c [MP3_CheckID3V1Tag] V1.1:no valid genre info!")
TRACE_MSG(MP3_FORMAT_998_112_2_18_1_18_23_1598,"mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!")
TRACE_MSG(MP3_FORMAT_1012_112_2_18_1_18_23_1599,"mp3_format.c [MP3_CheckID3V1Tag]V1.0: no valid genre info!")
TRACE_MSG(MP3_FORMAT_1044_112_2_18_1_18_23_1600,"mp3_format.c:MP3_CheckID3V2Tag:error:(SCI_NULL == ptMp3Handle)||(SCI_NULL == ptXingHeaderInfo)...")
TRACE_MSG(MP3_FORMAT_1074_112_2_18_1_18_24_1601,"mp3_format.c,[MP3_CheckID3V2Tag]:tag version:%d,tag size:0x%08x")
TRACE_MSG(MP3_FORMAT_1094_112_2_18_1_18_24_1602,"mp3_format.c,[MP3_CheckID3V2Tag]:End of tag frames !")
TRACE_MSG(MP3_FORMAT_1122_112_2_18_1_18_24_1603,"mp3_format.c,[MP3_CheckID3V2Tag]:frame id :%d ,frame_content_size:%d")
TRACE_MSG(MP3_FORMAT_1133_112_2_18_1_18_24_1604,"mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_FORMAT_1159_112_2_18_1_18_24_1605,"mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_FORMAT_1186_112_2_18_1_18_24_1606,"mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_FORMAT_1213_112_2_18_1_18_24_1607,"mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_FORMAT_1240_112_2_18_1_18_24_1608,"mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_FORMAT_1267_112_2_18_1_18_24_1609,"mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_FORMAT_1294_112_2_18_1_18_24_1610,"mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_FORMAT_1341_112_2_18_1_18_24_1611,"mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!")
TRACE_MSG(MP3_FORMAT_1346_112_2_18_1_18_24_1612,"mp3_format.c,[MP3_CheckID3V2Tag]:no valid genre index!")
TRACE_MSG(MP3_FORMAT_1375_112_2_18_1_18_24_1613,"mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_FORMAT_1395_112_2_18_1_18_24_1614,"mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!")
TRACE_MSG(MP3_FORMAT_1399_112_2_18_1_18_24_1615,"mp3_format.c,[MP3_CheckID3V2Tag]:year:impossible value!")
TRACE_MSG(MP3_FORMAT_1415_112_2_18_1_18_24_1616,"mp3_format.c,[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_FORMAT_1435_112_2_18_1_18_24_1617,"mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!")
TRACE_MSG(MP3_FORMAT_1439_112_2_18_1_18_24_1618,"mp3_format.c,[MP3_CheckID3V2Tag]:track:impossible value!")
TRACE_MSG(MP3_FORMAT_1450_112_2_18_1_18_24_1619,"mp3_format.c,[MP3_CheckID3V2Tag]:failed to jump out this frame!")
TRACE_MSG(MP3_FORMAT_1455_112_2_18_1_18_24_1620,"mp3_format.c,[MP3_CheckID3V2Tag]:other frame ID !We do not need the frame content!")
TRACE_MSG(MP3_FORMAT_1463_112_2_18_1_18_24_1621,"mp3_format.c,[MP3_CheckID3V2Tag] we find invalid frame ID!")
TRACE_MSG(MP3_FORMAT_1492_112_2_18_1_18_24_1622,"mp3_format.c:MP3_CheckID3V2Tag:error:(SCI_NULL == ptMp3Handle)||(SCI_NULL == ptXingHeaderInfo)...")
TRACE_MSG(MP3_FORMAT_1571_112_2_18_1_18_25_1623,"mp3_format.c:MP3_CheckLameTag:error:(SCI_NULL == ptMp3Handle)||(SCI_NULL == ptXingHeaderInfo)...")
TRACE_MSG(MP3_FORMAT_1597_112_2_18_1_18_25_1624,"mp3_format.c [MP3_CheckLameTag] no valid version info!")
TRACE_MSG(MP3_FORMAT_1683_112_2_18_1_18_25_1625,"mp3_format.c:MP3_CheckLyrics3Tag:error:(SCI_NULL == ptMp3Handle)||(SCI_NULL == ptXingHeaderInfo)...")
TRACE_MSG(MP3_FORMAT_1728_112_2_18_1_18_25_1626,"mp3_format.c,[MP3_CheckLyrics3Tag] tag size info error!can not get tag size!")
TRACE_MSG(MP3_FORMAT_1762_112_2_18_1_18_25_1627,"mp3_format.c,[MP3_CheckLyrics3Tag] can not alloc memory!")
TRACE_MSG(MP3_GETFILEINFO_162_112_2_18_1_18_25_1628,"mp3_getfileinfo.c,[MP3_CalcAveBitrate]uiFirstFrameOffset :0x%08x >= file_len :0x%08x")
TRACE_MSG(MP3_GETFILEINFO_239_112_2_18_1_18_26_1629,"peter:mp3_format_info.uiBitrate is %d,ave_bitrate_sum is %d")
TRACE_MSG(MP3_GETFILEINFO_282_112_2_18_1_18_26_1630,"mp3_getfileinfo.c [MP3_CalcAveBitrate]frame_count:%d,ave_bitrate_sum:%d kbps")
TRACE_MSG(MP3_GETFILEINFO_316_112_2_18_1_18_26_1631,"mp3_getfileinfo.c:MP3_Dmx_iToc:error:puiBuf == PNULL")
TRACE_MSG(MP3_GETFILEINFO_322_112_2_18_1_18_26_1632,"[MP3_Dmx_iToc]:input number: %d is not valid,(should be less than 9999) !")
TRACE_MSG(MP3_GETFILEINFO_463_112_2_18_1_18_26_1633,"mp3_getfileinfo.c,[MP3_IsVBR]uiFirstFrameOffset :0x%08x >= file_len :0x%08x")
TRACE_MSG(MP3_GETFILEINFO_614_112_2_18_1_18_26_1634,"mp3_getfileinfo.c,[MP3_CheckFileValidity]uiId3v2TagSize :0x%08x >= file_len :0x%08x")
TRACE_MSG(MP3_GETFILEINFO_639_112_2_18_1_18_26_1635,"mp3_getfileinfo.c,[MP3_CheckFileValidity]total_len:0x%08x,id3v2 size:0x%08x,total_check_len:0x%08x")
TRACE_MSG(MP3_GETFILEINFO_793_112_2_18_1_18_27_1636,"mp3_getfileinfo.c:MP3_ParseFileInfo:error:SCI_NULL == ptAudioObj")
TRACE_MSG(MP3_GETFILEINFO_837_112_2_18_1_18_27_1637,"mp3_getfileinfo.c [MP3_ParseFileInfo] AUDIO_FILE_HANDLE !file handle:0x%0x")
TRACE_MSG(MP3_GETFILEINFO_842_112_2_18_1_18_27_1638,"mp3_getfileinfo.c [MP3_ParseFileInfo] AUDIO_FILE_HANDLE !can not get music size!")
TRACE_MSG(MP3_GETFILEINFO_855_112_2_18_1_18_27_1639,"mp3_getfileinfo.c [MP3_ParseFileInfo] AUDIO_BUFFER_HANDLE !buf ptr:0x%0x,buf len:0x%0x,buf cur offset:0x%0x")
TRACE_MSG(MP3_GETFILEINFO_860_112_2_18_1_18_27_1640,"mp3_getfileinfo.c [MP3_ParseFileInfo] AUDIO_BUFFER_HANDLE !can not get music size!")
TRACE_MSG(MP3_GETFILEINFO_886_112_2_18_1_18_27_1641,"mp3_getfileinfo.c [MP3_ParseFileInfo] mp3_format_info_ptr->uiSamplerate is %d, mp3_format_info_ptr->uiChannelNum is %d")
TRACE_MSG(MP3_GETFILEINFO_901_112_2_18_1_18_27_1642,"mp3_getfileinfo.c [MP3_ParseFileInfo] AUDIO_STREAM_BUFFER_HANDLE !buf ptr:0x%0x,buf len:0x%0x,buf cur offset:0x%0x")
TRACE_MSG(MP3_GETFILEINFO_907_112_2_18_1_18_27_1643,"mp3_getfileinfo.c: Unknown Handle Type!")
TRACE_MSG(MP3_GETFILEINFO_932_112_2_18_1_18_27_1644,"mp3_getfileinfo.c,[MP3_ParseFileInfo] id3v2 tag offset:0,size:0x%08x")
TRACE_MSG(MP3_GETFILEINFO_1018_112_2_18_1_18_27_1645,"mp3_getfileinfo.c,[MP3_ParseFileInfo] fake mp3!")
TRACE_MSG(MP3_GETFILEINFO_1036_112_2_18_1_18_27_1646,"mp3_getfileinfo.c,[MP3_ParseFileInfo] id3v1 tag :offset:0x%08x,size:0x%08x")
TRACE_MSG(MP3_GETFILEINFO_1057_112_2_18_1_18_27_1647,"mp3_getfileinfo.c,[MP3_ParseFileInfo] lyrics3 tag :offset:0x%08x,size:0x%08x")
TRACE_MSG(MP3_GETFILEINFO_1079_112_2_18_1_18_27_1648,"mp3_getfileinfo.c,[MP3_ParseFileInfo]  ape tag :offset:0x%08x,size:0x%08x")
TRACE_MSG(MP3_GETFILEINFO_1096_112_2_18_1_18_27_1649,"mp3_getfileinfo.c,[MP3_ParseFileInfo]   xing header :offset:0x%08x,size:0x%08x")
TRACE_MSG(MP3_GETFILEINFO_1112_112_2_18_1_18_27_1650,"mp3_getfileinfo.c,[MP3_ParseFileInfo] lame tag  offset:0x%08x,size:0x%08x")
TRACE_MSG(MP3_GETFILEINFO_1130_112_2_18_1_18_27_1651,"mp3_getfileinfo.c,[MP3_ParseFileInfo] encounter error when parsing vbr info!")
TRACE_MSG(MP3_GETFILEINFO_1145_112_2_18_1_18_27_1652,"mp3_getfileinfo.c,[MP3_ParseFileInfo] mp3_format_info_ptr->uiTotalDataLength <= (file_tail_tag_size + mp3_ext_format_info_ptr->uiFirstFrameOffset")
TRACE_MSG(MP3_GETFILEINFO_1159_112_2_18_1_18_27_1653,"mp3_getfileinfo.c,[MP3_ParseFileInfo] 0 == mp3_format_info_ptr->uiBitrate")
TRACE_MSG(MP3_GETFILEINFO_1165_112_2_18_1_18_27_1654,"%s, %d, %d.")
TRACE_MSG(MP3_GETFILEINFO_1175_112_2_18_1_18_27_1655,"%s, %d, %d.")
TRACE_MSG(MP3_GETFILEINFO_1181_112_2_18_1_18_27_1656,"%s, %d, %d.")
TRACE_MSG(MP3_GETFILEINFO_1195_112_2_18_1_18_28_1657,"mp3_getfileinfo.c:CHNG_FREQ_REQ_HANDLER_NULL== s_mp3_parse_info_freq_handler")
TRACE_MSG(MP3_GETFILEINFO_1211_112_2_18_1_18_28_1658,"peter:peter: MP3_CalcAveBitrate is %d")
TRACE_MSG(MP3_GETFILEINFO_1214_112_2_18_1_18_28_1659,"mp3_getfileinfo.c,[MP3_ParseFileInfo] 0 == mp3_format_info_ptr->uiBitrate")
TRACE_MSG(MP3_GETFILEINFO_1219_112_2_18_1_18_28_1660,"%s, %d, %d.")
TRACE_MSG(MP3_GETFILEINFO_1220_112_2_18_1_18_28_1661,"mp3_getfileinfo.c,[MP3_ParseFileInfo]  get playing time by average bitrate:%d! -1-!")
TRACE_MSG(MP3_GETFILEINFO_1244_112_2_18_1_18_28_1662,"mp3_getfileinfo.c:CHNG_FREQ_REQ_HANDLER_NULL== s_mp3_parse_info_freq_handler")
TRACE_MSG(MP3_GETFILEINFO_1262_112_2_18_1_18_28_1663,"mp3_getfileinfo.c,[MP3_ParseFileInfo] 0 == mp3_format_info_ptr->uiBitrate")
TRACE_MSG(MP3_GETFILEINFO_1267_112_2_18_1_18_28_1664,"%s, %d, %d.")
TRACE_MSG(MP3_GETFILEINFO_1269_112_2_18_1_18_28_1665,"mp3_getfileinfo.c,[MP3_ParseFileInfo]  get playing time by average bitrate:%d! -2-!")
TRACE_MSG(MP3_GETFILEINFO_1296_112_2_18_1_18_28_1666,"mp3_getfileinfo.c,[MP3_ParseFileInfo] format info printing BEGIN\n")
TRACE_MSG(MP3_GETFILEINFO_1303_112_2_18_1_18_28_1667,"mp3_getfileinfo.c,--- mpegversion:%d,uiSamplesPerFrame:%d,uiFirstFrameOffset:0x%08x,vbr:%d,uiMusicDataLen:0x%08xn")
TRACE_MSG(MP3_GETFILEINFO_1311_112_2_18_1_18_28_1668,"mp3_getfileinfo.c,--- uiTotalDataLength::0x%08x,uiTotalTime:%d(s),samplerate:%d,bitrate:%d,layer:%d,eChannelMode0x%08xn")
TRACE_MSG(MP3_GETFILEINFO_1313_112_2_18_1_18_28_1669,"mp3_getfileinfo.c,[MP3_ParseFileInfo] format info printing END\n")
TRACE_MSG(MP3_DECODER_PROXY_108_112_2_18_1_18_47_1670,"[MP3_ARM] dec_ret=%d, frame_buf_ptr=%d, len=%d, next_begin_%d, bitrate=%d")
TRACE_MSG(MP3_DECODER_PROXY_109_112_2_18_1_18_47_1671,"[MP3_ARM]output_param_t.pcm_bytes=%d, channel_num=%d")
TRACE_MSG(MP3_DECODER_PROXY_119_112_2_18_1_18_47_1672,"[MP3_ARM] MP3_ARM_DEC_ERROR_DECODING")
TRACE_MSG(MP3_DECODER_PROXY_126_112_2_18_1_18_47_1673,"[MP3_ARM] MP3_ARM_DEC_ERROR_FRAMELEN")
TRACE_MSG(MP3_DECODER_PROXY_129_112_2_18_1_18_47_1674,"[MP3_ARM] MP3_ARM_DEC_ERROR_NEXT_BEGIN")
TRACE_MSG(MP3_DECODER_PROXY_132_112_2_18_1_18_47_1675,"[MP3_ARM] MP3_ARM_DEC_ERROR_BITRATE")
TRACE_MSG(MP3_DEMUX_26_112_2_18_1_18_47_1676,"mp3_demux.c mp3_demux_open allocLen:%d.")
TRACE_MSG(MP3_DEMUX_53_112_2_18_1_18_48_1677,"mp3_demux.c mp3_demux_get_info allocLen:1536.")
TRACE_MSG(MP3_DMX_FORMAT_724_112_2_18_1_18_51_1678,"mp3_dmx_format.c:MP3_Dmx_GetPlayInfo:error:ptAudioObj == PNULL")
TRACE_MSG(MP3_DMX_FORMAT_753_112_2_18_1_18_51_1679,"[MP3_GetPlayingInfo] Total time is:0x%08x")
TRACE_MSG(MP3_DMX_FORMAT_780_112_2_18_1_18_51_1680,"mp3_adp.c:(0 == mp3_format_info_ptr->uiBitrate)||(0 == ptAudioObj->uiOutputSamplerate)")
TRACE_MSG(MP3_DMX_FORMAT_796_112_2_18_1_18_51_1681,"(0 == mp3_ext_format_info_ptr->uiMusicDataLen)||(0 == ptAudioObj->uiOutputSamplerate)")
TRACE_MSG(MP3_DMX_FORMAT_830_112_2_18_1_18_52_1682,"[MP3_GetPlayingInfo] exception: cur_time_1 < cur_time_2!")
TRACE_MSG(MP3_DMX_FORMAT_865_112_2_18_1_18_52_1683,"[MP3_GetPlayingInfo], len--total:%d, cur:%d(byte), time--total:%d, cur:%d(s).")
TRACE_MSG(MP3_DMX_FORMAT_981_112_2_18_1_18_52_1684,"mp3_dmx_format.c:MP3_Dmx_GetPlayInfo:error:mx_info_ptr == PNULL)||(ptAudioULL")
TRACE_MSG(MP3_DMX_FORMAT_1004_112_2_18_1_18_52_1685,"mp3_dmx_format.c:MP3_Dmx_ParseFileInfo:error:PNULL == mp3_ext_format_info_ptr")
TRACE_MSG(MP3_DMX_FORMAT_1016_112_2_18_1_18_52_1686,"[MP3_DMX_FORMAT] AUDIO_FILE_HANDLE !file handle:0x%0x")
TRACE_MSG(MP3_DMX_FORMAT_1021_112_2_18_1_18_52_1687,"[MP3_DMX_FORMAT]:FILE SIZE =0")
TRACE_MSG(MP3_DMX_FORMAT_1029_112_2_18_1_18_52_1688,"[MP3_DMX_FORMAT] AUDIO_BUFFER_HANDLE !file handle:buf ptr:0x%0x,buf len:0x%0x")
TRACE_MSG(MP3_DMX_FORMAT_1034_112_2_18_1_18_52_1689,"[MP3_DMX_FORMAT]:BUFFER SIZE =0")
TRACE_MSG(MP3_DMX_FORMAT_1048_112_2_18_1_18_52_1690,"[MP3_DMX_FORMAT]:STREAM BUF ,SCI_NULL == stream_data_ptr")
TRACE_MSG(MP3_DMX_FORMAT_1056_112_2_18_1_18_52_1691,"[MP3_DMX_FORMAT]:STREAM BUF ,Get Format Info From APP,uiSamplerate=%d,uiChannelNum=%d")
TRACE_MSG(MP3_DMX_FORMAT_1076_112_2_18_1_18_52_1692,"[MP3_DMX_FORMAT]:STREAM BUF ,Get Only Stream Data form APP,buf ptr=0x%0x,buf len=0x%0x")
TRACE_MSG(MP3_DMX_FORMAT_1098_112_2_18_1_18_52_1693,"[MP3_ParseFileInfo] id3v2 tag offset:0,size:0x%08x")
TRACE_MSG(MP3_DMX_FORMAT_1102_112_2_18_1_18_52_1694,"[MP3_ParseFileInfo] id3v2 tag no found")
TRACE_MSG(MP3_DMX_FORMAT_1186_112_2_18_1_18_52_1695,"[MP3_ParseFileInfo] fake mp3!")
TRACE_MSG(MP3_DMX_FORMAT_1205_112_2_18_1_18_52_1696,"[MP3_ParseFileInfo] id3v1 tag :offset:0x%08x,size:0x%08x")
TRACE_MSG(MP3_DMX_FORMAT_1224_112_2_18_1_18_52_1697,"[MP3_ParseFileInfo] lyrics3 tag :offset:0x%08x,size:0x%08x")
TRACE_MSG(MP3_DMX_FORMAT_1243_112_2_18_1_18_52_1698,"[MP3_ParseFileInfo]  ape tag :offset:0x%08x,size:0x%08x")
TRACE_MSG(MP3_DMX_FORMAT_1262_112_2_18_1_18_52_1699,"[MP3_ParseFileInfo]   xing header :offset:0x%08x,size:0x%08x")
TRACE_MSG(MP3_DMX_FORMAT_1279_112_2_18_1_18_52_1700,"[MP3_ParseFileInfo] lame tag  offset:0x%08x,size:0x%08x")
TRACE_MSG(MP3_DMX_FORMAT_1297_112_2_18_1_18_52_1701,"[MP3_ParseFileInfo] encounter error when parsing vbr info!")
TRACE_MSG(MP3_DMX_FORMAT_1311_112_2_18_1_18_52_1702,"[MP3_ParseFileInfo] encounter error mp3_format_info_ptr->uiTotalDataLength <=(file_tail_tag_size + mp3_ext_format_info_ptr->uiFirstFrameOffset)")
TRACE_MSG(MP3_DMX_FORMAT_1324_112_2_18_1_18_53_1703,"[MP3_ParseFileInfo] encounter error 0 == mp3_format_info_ptr->uiBitrate)")
TRACE_MSG(MP3_DMX_FORMAT_1329_112_2_18_1_18_53_1704,"[MP3_ParseFileInfo,1total time=%ds]")
TRACE_MSG(MP3_DMX_FORMAT_1339_112_2_18_1_18_53_1705,"[MP3_ParseFileInfo,2total time=%ds]")
TRACE_MSG(MP3_DMX_FORMAT_1345_112_2_18_1_18_53_1706,"MP3_ParseFileInfo,3total time=%ds]")
TRACE_MSG(MP3_DMX_FORMAT_1363_112_2_18_1_18_53_1707,"[MP3_ParseFileInfo] encounter error 0 == aver_bitrate)")
TRACE_MSG(MP3_DMX_FORMAT_1368_112_2_18_1_18_53_1708,"MP3_ParseFileInfo,4total time=%ds]")
TRACE_MSG(MP3_DMX_FORMAT_1369_112_2_18_1_18_53_1709,"[MP3_ParseFileInfo]  get playing time by average bitrate:%d! -1-!")
TRACE_MSG(MP3_DMX_FORMAT_1390_112_2_18_1_18_53_1710,"[MP3_ParseFileInfo] encounter error 0 == aver_bitrate)")
TRACE_MSG(MP3_DMX_FORMAT_1396_112_2_18_1_18_53_1711,"MP3_ParseFileInfo,5total time=%ds]")
TRACE_MSG(MP3_DMX_FORMAT_1397_112_2_18_1_18_53_1712,"[MP3_ParseFileInfo]  get playing time by average bitrate:%d! -1-!")
TRACE_MSG(MP3_DMX_FORMAT_1421_112_2_18_1_18_53_1713,"mp3_demux_get_info: bitrate=%d, sample_rate=%d, layer=%d, channels=%d")
TRACE_MSG(MP3_DMX_FORMAT_1438_112_2_18_1_18_53_1714,"[MP3_ParseFileInfo] format info printing BEGIN\n")
TRACE_MSG(MP3_DMX_FORMAT_1445_112_2_18_1_18_53_1715," mpegversion:%d,uiSamplesPerFrame:%d,uiFirstFrameOffset:0x%08x,vbr:%d,uiMusicDataLen:0x%08xn")
TRACE_MSG(MP3_DMX_FORMAT_1453_112_2_18_1_18_53_1716,"uiTotalDataLength::0x%08x,uiTotalTime:%d(s),samplerate:%d,bitrate:%d,layer:%d,eChannelMode0x%08xn")
TRACE_MSG(MP3_DMX_FORMAT_1455_112_2_18_1_18_53_1717,"[MP3_ParseFileInfo] format info printing END\n")
TRACE_MSG(MP3_DMX_FORMAT_1514_112_2_18_1_18_53_1718,"[MP3_IsVBR]uiFirstFrameOffset :0x%08x >= file_len :0x%08x")
TRACE_MSG(MP3_DMX_FORMAT_1618_112_2_18_1_18_53_1719,"[MP3_CalcAveBitrate]frame_count:%d,ave_bitrate_sum:%d kbps")
TRACE_MSG(MP3_DMX_FORMAT_1679_112_2_18_1_18_53_1720,"[MP3_IsVBR]uiFirstFrameOffset :0x%08x >= file_len :0x%08x")
TRACE_MSG(MP3_DMX_FORMAT_1824_112_2_18_1_18_53_1721,"[MP3_CheckFileValidity]uiId3v2TagSize :0x%08x >= file_len :0x%08x")
TRACE_MSG(MP3_DMX_FORMAT_1847_112_2_18_1_18_54_1722,"[MP3_CheckFileValidity]total_len:0x%08x,id3v2 size:0x%08x,total_check_len:0x%08x")
TRACE_MSG(MP3_DMX_FORMAT_1975_112_2_18_1_18_54_1723,"mp3_dmx_format.c:MP3_Dmx_iToc:error:puiBuf == PNULL")
TRACE_MSG(MP3_DMX_FORMAT_1980_112_2_18_1_18_54_1724,"[MP3_Dmx_iToc]:input number: %d is not valid,(should be less than 9999) !")
TRACE_MSG(MP3_DMX_FORMAT_2016_112_2_18_1_18_54_1725,"mp3_dmx_format.c:MP3_Dmx_cToi:(puiBuf == PNULL)||(puiNum== 0)")
TRACE_MSG(MP3_DMX_FORMAT_2037_112_2_18_1_18_54_1726,"[MP3_DMX_FORMAT][MP3_CaculateNum]:i_calc:%d!not number!")
TRACE_MSG(MP3_DMX_FORMAT_2066_112_2_18_1_18_54_1727,"mp3_dmx_format.c:MP3_Dmx_CheckheaderInfo:(SCI_NULL == puiHeader)||(SCI_NULL == ptMp3HeaderInfo)")
TRACE_MSG(MP3_DMX_FORMAT_2213_112_2_18_1_18_54_1728,"mp3_dmx_format.c:MP3_Dmx_CheckXingHeader:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)")
TRACE_MSG(MP3_DMX_FORMAT_2341_112_2_18_1_18_55_1729,"mp3_dmx_format.c:MP3_Dmx_CheckID3V1Tag:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)")
TRACE_MSG(MP3_DMX_FORMAT_2391_112_2_18_1_18_55_1730,"[MP3_CheckID3V1Tag] V1.1:no valid year info!")
TRACE_MSG(MP3_DMX_FORMAT_2408_112_2_18_1_18_55_1731,"[MP3_CheckID3V1Tag] V1.1:no valid genre info!")
TRACE_MSG(MP3_DMX_FORMAT_2425_112_2_18_1_18_55_1732,"mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!")
TRACE_MSG(MP3_DMX_FORMAT_2439_112_2_18_1_18_55_1733,"[MP3_CheckID3V1Tag]V1.0: no valid genre info!")
TRACE_MSG(MP3_DMX_FORMAT_2475_112_2_18_1_18_55_1734,"mp3_dmx_format.c:MP3_Dmx_CheckID3V2Tag:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)")
TRACE_MSG(MP3_DMX_FORMAT_2495_112_2_18_1_18_55_1735,"[MP3_CheckID3V2Tag]: ID3 FLAG NOT EXIST")
TRACE_MSG(MP3_DMX_FORMAT_2507_112_2_18_1_18_55_1736,"[MP3_CheckID3V2Tag]:tag version:%d,tag size:0x%08x")
TRACE_MSG(MP3_DMX_FORMAT_2526_112_2_18_1_18_55_1737,"[MP3_CheckID3V2Tag]:End of tag frames !")
TRACE_MSG(MP3_DMX_FORMAT_2554_112_2_18_1_18_55_1738,"[MP3_CheckID3V2Tag]:frame id :%d ,frame_content_size:%d")
TRACE_MSG(MP3_DMX_FORMAT_2565_112_2_18_1_18_55_1739,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_2591_112_2_18_1_18_55_1740,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_2617_112_2_18_1_18_55_1741,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_2643_112_2_18_1_18_55_1742,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_2669_112_2_18_1_18_55_1743,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_2695_112_2_18_1_18_55_1744,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_2721_112_2_18_1_18_55_1745,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_2766_112_2_18_1_18_55_1746,"[MP3_CheckID3V1Tag] V1.0:no valid year info!")
TRACE_MSG(MP3_DMX_FORMAT_2771_112_2_18_1_18_55_1747,"[MP3_CheckID3V2Tag]:no valid genre index!")
TRACE_MSG(MP3_DMX_FORMAT_2800_112_2_18_1_18_55_1748,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_2818_112_2_18_1_18_55_1749,"[MP3_CheckID3V1Tag] V1.0:no valid year info!")
TRACE_MSG(MP3_DMX_FORMAT_2822_112_2_18_1_18_55_1750,"[MP3_CheckID3V2Tag]:year:impossible value!")
TRACE_MSG(MP3_DMX_FORMAT_2834_112_2_18_1_18_55_1751,"[MP3_CheckID3V2Tag]:failed to get frame content!j:%d,copy_count:%d,")
TRACE_MSG(MP3_DMX_FORMAT_2852_112_2_18_1_18_56_1752,"mp3_format.c [MP3_CheckID3V1Tag] V1.0:no valid year info!")
TRACE_MSG(MP3_DMX_FORMAT_2856_112_2_18_1_18_56_1753,"mp3_format.c,[MP3_CheckID3V2Tag]:track:impossible value!")
TRACE_MSG(MP3_DMX_FORMAT_2864_112_2_18_1_18_56_1754,"mp3_format.c,[MP3_CheckID3V2Tag]:other frame ID !We do not need the frame content!")
TRACE_MSG(MP3_DMX_FORMAT_2872_112_2_18_1_18_56_1755,"[MP3_CheckID3V2Tag]find invalid frame ID!")
TRACE_MSG(MP3_DMX_FORMAT_2905_112_2_18_1_18_56_1756,"mp3_dmx_format.c:MP3_Dmx_CheckAPETagHeader:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)")
TRACE_MSG(MP3_DMX_FORMAT_2985_112_2_18_1_18_56_1757,"mp3_dmx_format.c:MP3_Dmx_CheckAPETagHeader:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)")
TRACE_MSG(MP3_DMX_FORMAT_3007_112_2_18_1_18_56_1758,"[MP3_CheckLameTag] no valid version info!")
TRACE_MSG(MP3_DMX_FORMAT_3097_112_2_18_1_18_56_1759,"mp3_dmx_format.c:MP3_Dmx_CheckAPETagHeader:(SCI_NULL == hdmx)||(SCI_NULL == ptXingHeaderInfo)")
TRACE_MSG(MP3_DMX_FORMAT_3140_112_2_18_1_18_56_1760,"[MP3_CheckLyrics3Tag] tag size info error!can not get tag size!")
TRACE_MSG(MP3_DMX_FORMAT_3178_112_2_18_1_18_56_1761,"[MP3_CheckLyrics3Tag] can not alloc memory!")
TRACE_MSG(MP3_DSP_ADP_518_112_2_18_1_18_58_1762,"[MP3_DSP_CODEC]MP3_DSP_Construct, PNULL == adp_res_ptr")
TRACE_MSG(MP3_DSP_ADP_523_112_2_18_1_18_58_1763,"[MP3_DSP_CODEC]MP3_DSP_Construct, allocLen:%d.")
TRACE_MSG(MP3_DSP_ADP_533_112_2_18_1_18_58_1764,"[MP3_DSP_CODEC]MP3_DSP_Construct, PNULL == audio_format_info_ptr->pvOtherInfo")
TRACE_MSG(MP3_DSP_ADP_539_112_2_18_1_18_58_1765,"[MP3_DSP_CODEC]MP3_DSP_Construct, allocLen:%d.")
TRACE_MSG(MP3_DSP_ADP_549_112_2_18_1_18_58_1766,"[MP3_DSP_CODEC]MP3_DSP_Construct,MP3_DECODE_MAX_PCM_SIZE < MP3_DEMUX_MAX_FRAME_SIZE")
TRACE_MSG(MP3_DSP_ADP_572_112_2_18_1_18_58_1767,"[MP3_DSP_CODEC]MP3_DSP_Construct, allocLen:%d.")
TRACE_MSG(MP3_DSP_ADP_683_112_2_18_1_18_58_1768,"[MP3_DSP_CODEC]MP3_DSP_PlayConstruct,AUDIO_DEFAULT_TASK_ID != audio_obj_ptr->tThreadId)||(AUDIO_DEFAULT_TASK_ID== audio_obj_ptr->tAssiThreadId)")
TRACE_MSG(MP3_DSP_ADP_721_112_2_18_1_18_58_1769,"[MP3_DSP_CODEC]MP3_DSP_Play, ThreadID:0x%x,0x%x")
TRACE_MSG(MP3_DSP_ADP_731_112_2_18_1_18_58_1770,"[MP3_DSP_CODEC]MP3_DSP_Play, CHNG_FREQ_REQ_HANDLER_NULL == s_ui_chng_freq_req_handler!")
TRACE_MSG(MP3_DSP_ADP_953_112_2_18_1_18_59_1771,"[MP3_DSP_CODEC]MP3_DSP_ResumeConstruct,AUDIO_DEFAULT_TASK_ID != audio_obj_ptr->tThreadId)||(AUDIO_DEFAULT_TASK_ID== audio_obj_ptr->tAssiThreadId)")
TRACE_MSG(MP3_DSP_ADP_990_112_2_18_1_18_59_1772,"[MP3_DSP_CODEC]MP3_DSP_Play, ThreadID:0x%x,0x%x")
TRACE_MSG(MP3_DSP_ADP_1000_112_2_18_1_18_59_1773,"[MP3_DSP_CODEC]MP3_DSP_Play, CHNG_FREQ_REQ_HANDLER_NULL == s_ui_chng_freq_req_handler!")
TRACE_MSG(MP3_DSP_ADP_1169_112_2_18_1_18_59_1774,"[MP3_DSP] MP3_DSP_Play (AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId)||(AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tAssiThreadId)")
TRACE_MSG(MP3_DSP_ADP_1192_112_2_18_1_18_59_1775,"MP3_DSP_Stop,caller=0x%x")
TRACE_MSG(MP3_DSP_ADP_1208_112_2_18_1_18_59_1776,"[MP3_DSP] MP3_DSP_Stop (AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId)||(AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tAssiThreadId)")
TRACE_MSG(MP3_DSP_ADP_1235_112_2_18_1_18_59_1777,"[MP3_DSP] MP3_DSP_Pause (AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId)||(AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tAssiThreadId)")
TRACE_MSG(MP3_DSP_ADP_1261_112_2_18_1_18_59_1778,"[MP3_DSP] MP3_DSP_Resume (AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tThreadId)||(AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tAssiThreadId)")
TRACE_MSG(MP3_DSP_ADP_1288_112_2_18_1_19_0_1779,"[MP3_DSP] MP3_TimeSeek (AUDIO_DEFAULT_TASK_ID == audio_obj_ptr->tAssiThreadId)")
TRACE_MSG(MP3_DSP_ADP_1339_112_2_18_1_19_0_1780,"[MP3_TimeSeek] seek_offset=%d, seek_time=%d")
TRACE_MSG(MP3_DSP_ADP_1353_112_2_18_1_19_0_1781,"[MP3_DSP] SEEK IS NO MEANING FOR THIS TIME")
TRACE_MSG(MP3_DSP_ADP_1374_112_2_18_1_19_0_1782,"[MP3_DSP] MP3_DSP_Seek,uiOffset=%d")
TRACE_MSG(MP3_DSP_ADP_1382_112_2_18_1_19_0_1783,"[MP3_DSP] MP3_DSP_Seek,mp3_ext_format_info_ptr is null")
TRACE_MSG(MP3_DSP_ADP_1389_112_2_18_1_19_0_1784,"[MP3_DSP] MP3_DSP_Seek, clean the cache.")
TRACE_MSG(MP3_DSP_ADP_1455_112_2_18_1_19_0_1785,"[MP3_DSP] SEEK is not support by tAssiThreadId now")
TRACE_MSG(MP3_DSP_ADP_1462_112_2_18_1_19_0_1786,"[MP3_DSP] MP3_DSP_Seek, state=%d")
TRACE_MSG(MP3_DSP_ADP_1469_112_2_18_1_19_0_1787,"[MP3_DSP] SEEK IS NO MEANING FOR THIS TIME")
TRACE_MSG(MP3_DSP_ADP_1500_112_2_18_1_19_0_1788,"mp3_dsp_adp.c:MP3_DSP_IdentifyAudioFormat:error:PNULL == adp_res_ptr")
TRACE_MSG(MP3_DSP_ADP_1564_112_2_18_1_19_0_1789,"mp3_dsp_adp.c[MP3_DSP_GetPlayingInfo] stream, curTime:%d=%d(=%d+%d)-%d(=%d(=%d*%d)+%d(=%d+%d)).uiOutPos %d, uiDecPos %d")
TRACE_MSG(MP3_DSP_ADP_1713_112_2_18_1_19_0_1790,"[MP3_DSP]DSP DECODE INIT")
TRACE_MSG(MP3_DSP_ADP_1718_112_2_18_1_19_0_1791,"[MP3_DSP]DSP DECODE INIT DONE")
TRACE_MSG(MP3_DSP_ADP_1754_112_2_18_1_19_0_1792,"[MP3_DSP]PLAY END!")
TRACE_MSG(MP3_DSP_ADP_1779_112_2_18_1_19_0_1793,"[MP3_DSP]PLAY END!")
TRACE_MSG(MP3_DSP_ADP_1822_112_2_18_1_19_1_1794,"[MP3_DSP]Error Frame Length %d, channel %d!")
TRACE_MSG(MP3_DSP_ADP_1830_112_2_18_1_19_1_1795,"[MP3_DSP]PLAY END!")
TRACE_MSG(MP3_DSP_ADP_1889_112_2_18_1_19_1_1796,"[MP3_DSP]Error Frame Length %d, channel %d!")
TRACE_MSG(MP3_DSP_ADP_1897_112_2_18_1_19_1_1797,"[MP3_DSP]PLAY END!")
TRACE_MSG(MP3_DSP_ADP_1947_112_2_18_1_19_1_1798,"[MP3_DSP]PLAY END!")
TRACE_MSG(MP3_DSP_ADP_1961_112_2_18_1_19_1_1799,"[MP3_DSP]PAUSE BUT NOW DEMUX TASK IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN")
TRACE_MSG(MP3_DSP_ADP_1969_112_2_18_1_19_1_1800,"[MP3_DSP]PAUSE SUCCESS!")
TRACE_MSG(MP3_DSP_ADP_1977_112_2_18_1_19_1_1801,"[MP3_DSP]PAUSE BUT NOW DSP IS DECODE, WAIT SOME TIME THEN TRY AGAIN")
TRACE_MSG(MP3_DSP_ADP_1994_112_2_18_1_19_1_1802,"[MP3_DSP] stop waiting for track buffer empty")
TRACE_MSG(MP3_DSP_ADP_2002_112_2_18_1_19_1_1803,"[MP3_DSP]STOP BUT NOW DEMUX TASK IS NOT STOP, WAIT SOME TIME THEN TRY AGAIN")
TRACE_MSG(MP3_DSP_ADP_2010_112_2_18_1_19_1_1804,"[MP3_DSP]STOP SUCCESS!")
TRACE_MSG(MP3_DSP_ADP_2018_112_2_18_1_19_1_1805,"[MP3_DSP]STOP BUT NOW DSP IS DECODE, WAIT SOME TIME THEN TRY AGAIN")
TRACE_MSG(MP3_DSP_ADP_2075_112_2_18_1_19_1_1806,"[MP3_DSP]:demux_res_ptr->bIsDemuxStop!")
TRACE_MSG(MP3_DSP_ADP_2081_112_2_18_1_19_1_1807,"[MP3_DSP]:MP3_DSP_DEMUX_FRAME!")
TRACE_MSG(MP3_DSP_ADP_2092_112_2_18_1_19_1_1808,"[MP3_DSP]:DEMUX INIT OK!")
TRACE_MSG(MP3_DSP_ADP_2160_112_2_18_1_19_1_1809,"[MP3_DSP]:DEMUX Resume INIT OK!")
TRACE_MSG(MP3_DSP_ADP_2197_112_2_18_1_19_1_1810,"[MP3_DSP_DEMUX]:MP3_DSP_SEEK,clean stream buffer,%d")
TRACE_MSG(MP3_DSP_ADP_2234_112_2_18_1_19_1_1811,"[MP3_DSP]:DEMUX THREAD RECV A BAD SIGNAL, DISCARD IT")
TRACE_MSG(MP3_DSP_ADP_2344_112_2_18_1_19_2_1812,"[MP3_DSP]:DEMUX _MP3_DSP_DEMUX_Init CHNG_FREQ_REQ_HANDLER_NULL == s_ui_chng_freq_req_handler")
TRACE_MSG(MP3_DSP_ADP_2426_112_2_18_1_19_2_1813,"_MP3_DSP_ReadDemuxFrame no mp3 data.")
TRACE_MSG(MP3_DSP_ADP_2450_112_2_18_1_19_2_1814,"_MP3_DSP_Stream_CastOff_SrcDataInfo para error:%d, %d.")
TRACE_MSG(MP3_DSP_ADP_2467_112_2_18_1_19_2_1815,"_MP3_DSP_Stream_CastOff_SrcDataInfo,CastOffDataLen:%d;info_list=(%d,%d,%d,%d)")
TRACE_MSG(MP3_DSP_ADP_2487_112_2_18_1_19_2_1816,"_MP3_DSP_Stream_CastOff_SrcDataInfo 0 CastOffDataInfo:%d, %d, %d.")
TRACE_MSG(MP3_DSP_ADP_2506_112_2_18_1_19_2_1817,"_MP3_DSP_Stream_CastOff_SrcDataInfo 1 CastOffDataInfo:%d, %d, %d.")
TRACE_MSG(MP3_DSP_ADP_2526_112_2_18_1_19_2_1818,"_MP3_DSP_Stream_CastOff_SrcDataInfo 2 CastOffDataInfo:%d, %d, %d.")
TRACE_MSG(MP3_DSP_ADP_2551_112_2_18_1_19_2_1819,"_MP3_DSP_Stream_CastOff_SrcDataInfo 3 CastOffDataInfo:%d, %d, %d, leftDataLen:%d.")
TRACE_MSG(MP3_DSP_ADP_2568_112_2_18_1_19_2_1820,"_MP3_DSP_Stream_CastOff_SrcDataInfo realCastoff:%d, wantToCastOff:%d, lastPlayTime:%d.")
TRACE_MSG(MP3_DSP_ADP_2577_112_2_18_1_19_2_1821,"_MP3_DSP_Stream_CastOff_SrcDataInfo, ListInfo:%d, %d, %d, %d.")
TRACE_MSG(MP3_DSP_ADP_2591_112_2_18_1_19_2_1822,"_MP3_DSP_WriteDemuxFrame, iMp3Demuxresult = %d")
TRACE_MSG(MP3_DSP_ADP_2628_112_2_18_1_19_2_1823,"_MP3_DSP_WriteDemuxFrame, demux frame out is full.")
TRACE_MSG(MP3_DSP_ADP_2649_112_2_18_1_19_2_1824,"_MP3_DSP_ResetDemuxFrames, uiOffsetTime %d, uiOutPos %d")
TRACE_MSG(MP3_DSP_ADP_2743_112_2_18_1_19_2_1825,"[_MP3_DSP_DecodeFrame] NULL == s_cur_audio_object_ptr")
TRACE_MSG(MP3_DSP_ADP_2761_112_2_18_1_19_2_1826,"[_MP3_DSP_DecodeFrame] dec_result_error=%d")
TRACE_MSG(MP3_DSP_ADP_2785_112_2_18_1_19_2_1827,"[_MP3_DSP_DecodeFrameCallback] NULL == s_cur_audio_object_ptr")
TRACE_MSG(MP3_DSP_ADP_2792_112_2_18_1_19_2_1828,"[_MP3_DSP_DecodeFrameCallback] stop waiting for track buffer empty")
TRACE_MSG(MP3_DSP_ADP_2799_112_2_18_1_19_2_1829,"[_MP3_DSP_DecodeFrameCallback] NULL == mp3_ext_format_info_ptr")
TRACE_MSG(MP3_DSP_ADP_2807_112_2_18_1_19_3_1830,"[MP3_DSP]Error Frame Length %d, channel %d!")
TRACE_MSG(MP3_DSP_ADP_2966_112_2_18_1_19_3_1831,"[MP3_DSP]PLAY END!")
TRACE_MSG(MP3_DSP_ADP_2992_112_2_18_1_19_3_1832,"[MP3_DSP]PLAY END!")
TRACE_MSG(MP3_DSP_ADP_3046_112_2_18_1_19_3_1833,"_MP3_DSP_Stream_AddDataToCache,cache_offset:%d,packet_size=%d")
TRACE_MSG(MP3_DSP_ADP_3075_112_2_18_1_19_3_1834,"_MP3_DSP_Stream_AddInfoToList, StreamList:(%d;%d,%d;%d);cur_packet(%d,%d)")
TRACE_MSG(MP3_DSP_ADP_3160_112_2_18_1_19_3_1835,"mp3_dsp_adp.c:_MP3_DSP_Demux_GetData:error:fileCacheBuf== PNULL")
TRACE_MSG(MP3_DSP_ADP_3266_112_2_18_1_19_3_1836,"mp3_dsp_adp.c:_MP3_DSP_Demux_GetData:error:(pucsBuffer == PNULL)||(uiBufSize == 0)")
TRACE_MSG(MP3_DSP_ADP_3318_112_2_18_1_19_4_1837,"_MP3_DSP_Demux_GetData:error:ui_cb_offset(%d) < uiSrcBufBegin(%d)")
TRACE_MSG(MP3_DSP_ADP_3342_112_2_18_1_19_4_1838,"_MP3_DSP_Demux_GetData,list is full,(in,out,totallen)=(%d,%d,%d)")
TRACE_MSG(MP3_DSP_ADP_3406_112_2_18_1_19_4_1839,"_MP3_DSP_Demux_GetData,get_src_result=%d")
TRACE_MSG(MP3_DSP_ADP_3466_112_2_18_1_19_4_1840,"[MP3_DSP]:ERROR :this singal is sent to decode task")
TRACE_MSG(MP3_DSP_ADP_3488_112_2_18_1_19_4_1841,"[MP3_DSP]:ERROR :this singal is sent to decode task")
TRACE_MSG(MP3_DSP_ADP_3507_112_2_18_1_19_4_1842,"[MP3_DSP]:ERROR :this singal is sent to decode task")
TRACE_MSG(MP3_DSP_ADP_3527_112_2_18_1_19_4_1843,"[MP3_DSP]:ERROR :this singal is sent to decode task")
TRACE_MSG(MP3_DSP_ADP_3546_112_2_18_1_19_4_1844,"[MP3_DSP]:ERROR :this singal is sent to decode task")
TRACE_MSG(MP3_DSP_ADP_3565_112_2_18_1_19_4_1845,"[MP3_DSP]:ERROR :this singal is sent to decode task")
TRACE_MSG(MP3_DSP_ADP_3584_112_2_18_1_19_4_1846,"[MP3_DSP]:ERROR :this singal is sent to decode task")
TRACE_MSG(MP3_DSP_ADP_3602_112_2_18_1_19_4_1847,"[MP3_DSP]:ERROR :this singal is sent to decode task")
TRACE_MSG(MP3_DSP_ADP_3622_112_2_18_1_19_4_1848,"[MP3_DSP]:ERROR :this singal is sent to decode task")
TRACE_MSG(MP3_DSP_ADP_3640_112_2_18_1_19_4_1849,"[MP3_DSP]:ERROR :this singal is sent to decode task")
TRACE_MSG(MP3_DSP_ADP_3660_112_2_18_1_19_4_1850,"[MP3_DSP]:ERROR :this singal is sent to decode task")
TRACE_MSG(MP3_DSP_ADP_3680_112_2_18_1_19_4_1851,"[MP3_DSP]:ERROR :this singal is sent to decode task")
TRACE_MSG(MP3_DSP_ADP_3688_112_2_18_1_19_4_1852,"[MP3_DSP]:ERROR :_MP3_DSP_SendSig SEND A UNKOWNED SIG!")
TRACE_MSG(MP3_MUX_38_112_2_18_1_19_26_1853,"peter: mp3_mux_one_frame in frame len is %d, mp3_mux->file_handle is %d")
TRACE_MSG(MP3_MUX_47_112_2_18_1_19_26_1854,"peter: mp3_mux_one_frame in 2 frame len  is %d, bytes_written is %d")
TRACE_MSG(MP3_MUX_81_112_2_18_1_19_26_1855,"mp3_mux.c: mp3_mux_open in")
TRACE_MSG(MP3_MUX_111_112_2_18_1_19_26_1856,"mp3_mux.c:stream_new failed")
TRACE_MSG(WAVE_MUX_142_112_2_18_1_19_26_1857,"wave_mux.c: wave_mux_open in")
TRACE_MSG(WAVE_MUX_162_112_2_18_1_19_26_1858,"wave_mux.c: error:mux_res null")
TRACE_MSG(WAVE_MUX_174_112_2_18_1_19_26_1859,"wave_mux.c:stream_new failed")
TRACE_MSG(NULL_CODEC_ADP_90_112_2_18_1_19_27_1860,"null_codec_adp.c:NULL_CODEC_IdentifyFormat:error:ptAudioObject == SCI_NULL")
TRACE_MSG(RA8LBR_ADP_288_112_2_18_1_19_43_1861,"[RA8LBR_Construct] Alloc failed, left_exp_data_ptr==NULL")
TRACE_MSG(RA8LBR_ADP_299_112_2_18_1_19_43_1862,"[RA8LBR_Construct] Alloc failed, right_exp_data_ptr==NULL")
TRACE_MSG(RA8LBR_ADP_310_112_2_18_1_19_43_1863,"[RA8LBR_Construct] Alloc failed, ra8lbr_res_ptr==NULL")
TRACE_MSG(RA8LBR_ADP_320_112_2_18_1_19_43_1864,"[RA8LBR_Construct] Alloc failed, ra8lbr_res_ptr->lbr_mem_ptr==NULL")
TRACE_MSG(RA8LBR_ADP_333_112_2_18_1_19_43_1865,"[RA8LBR_Construct] aud_obj_ptr->pGetSrcCallbackFunc==NULL")
TRACE_MSG(RA8LBR_ADP_341_112_2_18_1_19_43_1866,"[RA8LBR_Construct] ra8lbr_res_ptr->sub_type==default")
TRACE_MSG(RA8LBR_ADP_365_112_2_18_1_19_43_1867,"[RA8LBR_Construct] Create RA8LBR thread failed.")
TRACE_MSG(RA8LBR_ADP_444_112_2_18_1_19_43_1868,"[RA8LBR_PlayConstruct] CHNG_FREQ_REQ_HANDLER_NULL == s_ui_ra8lbr_chng_freq_req_handler")
TRACE_MSG(RA8LBR_ADP_493_112_2_18_1_19_43_1869,"[RA8LBR_ResumeConstruct] CHNG_FREQ_REQ_HANDLER_NULL == s_ui_ra8lbr_chng_freq_req_handler")
TRACE_MSG(RA8LBR_ADP_537_112_2_18_1_19_43_1870,"[RA8LBR_Play] NULL == sig_ptr")
TRACE_MSG(RA8LBR_ADP_550_112_2_18_1_19_43_1871,"[RA8LBR_Play] start to play.")
TRACE_MSG(RA8LBR_ADP_568_112_2_18_1_19_43_1872,"[RA8LBR_Stop] NULL == sig_ptr")
TRACE_MSG(RA8LBR_ADP_576_112_2_18_1_19_43_1873,"[RA8LBR_Stop] stopped.")
TRACE_MSG(RA8LBR_ADP_594_112_2_18_1_19_43_1874,"[RA8LBR_Pause] NULL == sig_ptr")
TRACE_MSG(RA8LBR_ADP_602_112_2_18_1_19_43_1875,"[RA8LBR_Pause] pause.")
TRACE_MSG(RA8LBR_ADP_621_112_2_18_1_19_43_1876,"[RA8LBR_Resume] NULL == sig_ptr")
TRACE_MSG(RA8LBR_ADP_629_112_2_18_1_19_43_1877,"[RA8LBR_Resume] resume.")
TRACE_MSG(RA8LBR_ADP_663_112_2_18_1_19_43_1878,"[RA8LBR_IdentifyFormat] error:NULL == ra8lbr_res_ptr")
TRACE_MSG(RA8LBR_ADP_675_112_2_18_1_19_43_1879,"[RA8LBR_IdentifyFormat] error:NULL == cfg_info_ptr")
TRACE_MSG(RA8LBR_ADP_723_112_2_18_1_19_43_1880,"[RA8LBR_GetPlayingInfo] stream, curTime:%d, %d should not less than:%d(=%d+%d).")
TRACE_MSG(RA8LBR_ADP_741_112_2_18_1_19_43_1881,"[RA8LBR_GetOutputSamplerate] OutputSampleRate=%d")
TRACE_MSG(RA8LBR_ADP_769_112_2_18_1_19_44_1882,"[RA8LBR_SendSig] RA8LBR_DECODING, SCI_ALLOCA fail")
TRACE_MSG(RA8LBR_ADP_790_112_2_18_1_19_44_1883,"[RA8LBR_SendSig] RA8LBR_STOP, SCI_ALLOCA fail")
TRACE_MSG(RA8LBR_ADP_839_112_2_18_1_19_44_1884,"[RA8LBR_InitDecoder] Error: dec_result=%d")
TRACE_MSG(RA8LBR_ADP_850_112_2_18_1_19_44_1885,"sample_num=%d, frame_size=%d, sample_rate=%d, region_num=%d, channel_num=%d, cplStart=%d, cplQBits=%d")
TRACE_MSG(RA8LBR_ADP_852_112_2_18_1_19_44_1886,"[RA8LBR_InitDecoder] Init successfully.")
TRACE_MSG(RA8LBR_ADP_915_112_2_18_1_19_44_1887,"[pGetSrcCallbackFunc] 0 == ra8lbr_res_ptr->src_data_len")
TRACE_MSG(RA8LBR_ADP_936_112_2_18_1_19_44_1888,"[pGetSrcCallbackFunc] AUDIO_STREAM_GET_SRC_WAIT")
TRACE_MSG(RA8LBR_ADP_941_112_2_18_1_19_44_1889,"[pGetSrcCallbackFunc] AUDIO_STREAM_GET_SRC_ASY")
TRACE_MSG(RA8LBR_ADP_945_112_2_18_1_19_44_1890,"[pGetSrcCallbackFunc] AUDIO_STREAM_GET_SRC_ERROR")
TRACE_MSG(RA8LBR_ADP_975_112_2_18_1_19_44_1891,"LBR_DEC_FrameDecode ERROR!!!")
TRACE_MSG(ADPCM_ENCODER_362_112_2_18_1_19_45_1892,"AdpcmEncoderSetSamplerate Error : %d")
TRACE_MSG(ADPCM_ENCODER_451_112_2_18_1_19_45_1893,"The header size of wav file for ADCPM is %d, not 60 bytes!\n")
TRACE_MSG(ADPCMRECORD_457_112_2_18_1_19_46_1894,"[Record_audio_init]: 0x%x, %x, %x.")
TRACE_MSG(ADPCMRECORD_529_112_2_18_1_19_47_1895,"adpcmrecord.c:Record_SetParam:error:SET_COUNT)||(argv==SCI_NULL")
TRACE_MSG(ADPCMRECORD_535_112_2_18_1_19_47_1896,"[Record_SetParam]:It is recording.")
TRACE_MSG(ADPCMRECORD_555_112_2_18_1_19_47_1897,"[Record_SetParam] mp3 record set param DONE.")
TRACE_MSG(ADPCMRECORD_575_112_2_18_1_19_47_1898,"adpcmrecord.c:Record_SetParam:error:0x%x, 0x%x, 0x%x, 0x%x, 0x%x")
TRACE_MSG(ADPCMRECORD_593_112_2_18_1_19_47_1899,"adpcmrecord.c:Record_SetParam:0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x")
TRACE_MSG(ADPCMRECORD_615_112_2_18_1_19_47_1900,"adpcmrecord.c:Record_SetIfFromNet:error:ERROR_COUNT)||(argv==SCI_NULL")
TRACE_MSG(ADPCMRECORD_621_112_2_18_1_19_47_1901,"[Record_SetIfFromNet]:It is recording.")
TRACE_MSG(ADPCMRECORD_627_112_2_18_1_19_47_1902,"[Record_SetIfFromNet]:%d.")
TRACE_MSG(ADPCMRECORD_647_112_2_18_1_19_47_1903,"adpcmrecord.c:Record_GetRecordTime:error:ME_COUNT)||(argv==SCI_NULL")
TRACE_MSG(ADPCMRECORD_660_112_2_18_1_19_47_1904,"[Record_GetRecordTime] NOT RECORDING.")
TRACE_MSG(ADPCMRECORD_679_112_2_18_1_19_47_1905,"adpcmrecord.c:Record_GetVoiceDB:error:ME_COUNT)||(argv==SCI_NULL")
TRACE_MSG(ADPCMRECORD_692_112_2_18_1_19_47_1906,"[Record_GetVoiceDB] NOT RECORDING.")
TRACE_MSG(ADPCMRECORD_709_112_2_18_1_19_47_1907,"[Record_GetCurFileSize] INPUT PARAM ERROR!")
TRACE_MSG(ADPCMRECORD_729_112_2_18_1_19_47_1908,"[Record_SetEndTime] INPUT PARAM ERROR!")
TRACE_MSG(ADPCMRECORD_733_112_2_18_1_19_47_1909,"[Record_SetEndTime]  %d !")
TRACE_MSG(ADPCMRECORD_751_112_2_18_1_19_47_1910,"[Record_SetCBTime] INPUT PARAM ERROR!")
TRACE_MSG(ADPCMRECORD_757_112_2_18_1_19_47_1911,"[Record_SetCBTime] %d, 0x%x!")
TRACE_MSG(ADPCMRECORD_774_112_2_18_1_19_47_1912,"[Record_SetEndSize] INPUT PARAM ERROR!")
TRACE_MSG(ADPCMRECORD_794_112_2_18_1_19_47_1913,"[AUD_RecordStart] entry point!")
TRACE_MSG(ADPCMRECORD_799_112_2_18_1_19_47_1914,"AUD_RecordStart]: hf is null")
TRACE_MSG(ADPCMRECORD_805_112_2_18_1_19_47_1915,"[AUD_RecordStart] error: it is recording")
TRACE_MSG(ADPCMRECORD_829_112_2_18_1_19_47_1916,"AUD_RecordStart input para error, %d <= %d (byte).")
TRACE_MSG(ADPCMRECORD_838_112_2_18_1_19_47_1917,"AUD_RecordStart input para error, %d < %d * %d(byte).")
TRACE_MSG(ADPCMRECORD_857_112_2_18_1_19_47_1918,"[AUD_RecordStart] AUD_REC_AMR_SW")
TRACE_MSG(ADPCMRECORD_880_112_2_18_1_19_47_1919,"[AUD_RecordStart] AUD_REC_MP3")
TRACE_MSG(ADPCMRECORD_891_112_2_18_1_19_47_1920,"[AUD_RecordStart] AUD_REC_PCM")
TRACE_MSG(ADPCMRECORD_932_112_2_18_1_19_47_1921,"AUD_RecordStart SCI_CreateMutex RecordTimer failed.")
TRACE_MSG(ADPCMRECORD_946_112_2_18_1_19_47_1922,"[AUD_RecordStart] SCI_NULL == s_notify_timer_ptr")
TRACE_MSG(ADPCMRECORD_956_112_2_18_1_19_47_1923,"AUD_RecordStart SCI_CreateMutex failed.")
TRACE_MSG(ADPCMRECORD_986_112_2_18_1_19_47_1924,"AdpcmRecord.c Record: Send signal to start record.")
TRACE_MSG(ADPCMRECORD_1008_112_2_18_1_19_48_1925,"mp3 encoder init start here...")
TRACE_MSG(ADPCMRECORD_1023_112_2_18_1_19_48_1926,"sample_rate=%d, bit_rate=%d, ch_count=%d, ms_sign=%d, vbr_sign=%d, cut_off=%d")
TRACE_MSG(ADPCMRECORD_1028_112_2_18_1_19_48_1927,"MP3_ENC_MemoryAlloc failed.")
TRACE_MSG(ADPCMRECORD_1034_112_2_18_1_19_48_1928,"mp3 encoder init DONE")
TRACE_MSG(ADPCMRECORD_1084_112_2_18_1_19_48_1929,"[Record_Mp3BufAlloc] mp3_rec_buf=0x%x, mp3_rec_buf_len=%d, mp3_ref_src_buf=0x%x, mp3_ref_src_buf_len=%d,mp3_temp_buf=0x%x, mp3_temp_buf_len=%d")
TRACE_MSG(ADPCMRECORD_1099_112_2_18_1_19_48_1930,"[Record_Mp3BufFree] Error: NULL == s_mp3_enc_buf.")
TRACE_MSG(ADPCMRECORD_1105_112_2_18_1_19_48_1931,"[Record_Mp3BufFree] Free DONE.")
TRACE_MSG(ADPCMRECORD_1118_112_2_18_1_19_48_1932,"Record_Mp3Finished done here...")
TRACE_MSG(ADPCMRECORD_1136_112_2_18_1_19_48_1933,"[Record_Mp3EncExit] stream_len=%d")
TRACE_MSG(ADPCMRECORD_1154_112_2_18_1_19_48_1934,"[Record_Mp3EncFrame] temp_count =%d, read_count=%d,ref_buf_get_count=%d")
TRACE_MSG(ADPCMRECORD_1195_112_2_18_1_19_48_1935,"[Record_Mp3EncFrame] temp_count=%d")
TRACE_MSG(ADPCMRECORD_1210_112_2_18_1_19_48_1936,"[Record_Mp3EncFrame]  enc_done, enc_result=%d, stream_len=%d")
TRACE_MSG(ADPCMRECORD_1242_112_2_18_1_19_48_1937,"[AUD_Record_StartRecord] Entry point!!")
TRACE_MSG(ADPCMRECORD_1244_112_2_18_1_19_48_1938,"[AUD_Record_StartRecord] minBufsize=%d, trackbufsize=%d (bytes).")
TRACE_MSG(ADPCMRECORD_1250_112_2_18_1_19_48_1939,"adpcmrecord.c:AUD_Record_StartRecord:error:	SCI_NULL == ptAudioObjBak->puiTrackBuffer")
TRACE_MSG(ADPCMRECORD_1257_112_2_18_1_19_48_1940,"adpcmrecord.c:AUD_Record_StartRecord:error:%d,%d")
TRACE_MSG(ADPCMRECORD_1266_112_2_18_1_19_48_1941,"[AUD_Record_StartRecord] Init buf error!")
TRACE_MSG(ADPCMRECORD_1281_112_2_18_1_19_48_1942,"[AUD_Record_StartRecord] set zero before 0x %x, len:%d(bytes).")
TRACE_MSG(ADPCMRECORD_1282_112_2_18_1_19_48_1943,"[AUD_Record_StartRecord] set zero before 0x %x, len:%d(bytes).")
TRACE_MSG(ADPCMRECORD_1283_112_2_18_1_19_48_1944,"[AUD_Record_StartRecord] set zero before 0x %x, len:%d(bytes)")
TRACE_MSG(ADPCMRECORD_1340_112_2_18_1_19_48_1945,"[AUD_Record_StartRecord] set record samplerate %d.")
TRACE_MSG(ADPCMRECORD_1348_112_2_18_1_19_48_1946,"adpcmrecord.c:AUD_Record_StartRecord:error:8000 != ptAudioObjBak->uiOutputSamplerate")
TRACE_MSG(ADPCMRECORD_1368_112_2_18_1_19_48_1947,"adpcmrecord.c:AUD_Record_StartRecord:error:CHNG_FREQ_REQ_HANDLER_NULL == s_record_chng_freq_req_handler")
TRACE_MSG(ADPCMRECORD_1391_112_2_18_1_19_48_1948,"adpcmrecord.c:AMR_ENC_Init:error!")
TRACE_MSG(ADPCMRECORD_1414_112_2_18_1_19_48_1949,"[AUD_Record_StartRecord] s_notify_timer_ptr IS ACTIVE.")
TRACE_MSG(ADPCMRECORD_1449_112_2_18_1_19_48_1950,"adpcmrecord.c:AUD_Record_StartRecord:error:PNULL == s_record_device->tDe")
TRACE_MSG(ADPCMRECORD_1488_112_2_18_1_19_48_1951,"[AUD_Record_RecordResume] s_notify_timer_ptr IS NOT ACTIVE.")
TRACE_MSG(ADPCMRECORD_1525_112_2_18_1_19_49_1952,"[AUD_Record_RecordResume] s_notify_timer_ptr IS ACTIVE.")
TRACE_MSG(ADPCMRECORD_1564_112_2_18_1_19_49_1953,"[AUD_Record_RecordStop] set s_mmi_record_stop")
TRACE_MSG(ADPCMRECORD_1568_112_2_18_1_19_49_1954,"s_mmi_record_stop=TRUE, MULTI STOP!!!")
TRACE_MSG(ADPCMRECORD_1603_112_2_18_1_19_49_1955,"[ AUD_Record_RecordStop] write fail, then send signal to write header")
TRACE_MSG(ADPCMRECORD_1614_112_2_18_1_19_49_1956,"[AUD_Record_RecordStop] s_is_start_read_data:%d")
TRACE_MSG(ADPCMRECORD_1669_112_2_18_1_19_49_1957,"[AUD_L1RecordCallback] record codec has stop recording.")
TRACE_MSG(ADPCMRECORD_1675_112_2_18_1_19_49_1958,"%s, %d, data_len:%d, should be:%d.")
TRACE_MSG(ADPCMRECORD_1715_112_2_18_1_19_49_1959,"[Record_SendCodeSignal] entry point!")
TRACE_MSG(ADPCMRECORD_1730_112_2_18_1_19_49_1960,"[Record_SendCodeSignal] state: %d, %d, %d")
TRACE_MSG(ADPCMRECORD_1750_112_2_18_1_19_49_1961,"[Record_TimerNotify] SCI_NULL == s_notify_callback")
TRACE_MSG(ADPCMRECORD_1856_112_2_18_1_19_49_1962,"REF_WriteVoiceData, %04x,%04x,%04x,%04x,%04x,%04x,%04x,%04x.")
TRACE_MSG(ADPCMRECORD_1969_112_2_18_1_19_49_1963,"[REF_GetNewVoiceData] s_mmi_record_stop=1")
TRACE_MSG(ADPCMRECORD_1980_112_2_18_1_19_49_1964,"[REF_GetNewVoiceData]write last buffer")
TRACE_MSG(ADPCMRECORD_2024_112_2_18_1_19_49_1965,"[AUD_RecordPause] Send signal to pause record.")
TRACE_MSG(ADPCMRECORD_2064_112_2_18_1_19_50_1966,"[AUD_RecordResume] Send signal to resume record.")
TRACE_MSG(ADPCMRECORD_2068_112_2_18_1_19_50_1967,"[AUD_RecordResume] :s_mmi_record_stop  has stopped")
TRACE_MSG(ADPCMRECORD_2111_112_2_18_1_19_50_1968,"AdpcmRecord.c Record: Send signal to stop record.")
TRACE_MSG(ADPCMRECORD_2132_112_2_18_1_19_50_1969,"[AUD_RecordStop] s_mmi_record_stop=%d, s_is_recording=%d")
TRACE_MSG(ADPCMRECORD_2138_112_2_18_1_19_50_1970,"[AUD_RecordStop] 2 s_mmi_record_stop=%d, s_is_recording=%d, lost:%d(frames), frameCountIQHeaderNo5a5a:%d.")
TRACE_MSG(ADPCMRECORD_2158_112_2_18_1_19_50_1971,"Record_Output_Print len:%d, common log")
TRACE_MSG(ADPCMRECORD_2180_112_2_18_1_19_50_1972,"Voice_Dump_Pcm_Data_Callback data:%02d,%02d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d,%05d")
TRACE_MSG(ADPCMRECORD_2184_112_2_18_1_19_50_1973,"%05d")
TRACE_MSG(ADPCMRECORD_2190_112_2_18_1_19_50_1974,"Record_Output_Print len:%d of dsp log.")
TRACE_MSG(ADPCMRECORD_2197_112_2_18_1_19_50_1975,"Record_Output_Print len:%d, curOffset:%d, totalBuf:%d.")
TRACE_MSG(ADPCMRECORD_2249_112_2_18_1_19_50_1976,"[Record_ChangeBufToWrite] rec_ctrl_writing->available_data: %d")
TRACE_MSG(ADPCMRECORD_2253_112_2_18_1_19_50_1977,"[Record_ChangeBufToWrite] state: %d, %d, %d")
TRACE_MSG(ADPCMRECORD_2278_112_2_18_1_19_50_1978,"AdpcmRecord.c  Can't get file position")
TRACE_MSG(ADPCMRECORD_2281_112_2_18_1_19_50_1979,"record pcm current_file_pos 1=%d")
TRACE_MSG(ADPCMRECORD_2285_112_2_18_1_19_50_1980,"AdpcmRecord.c  Can't Seek to positon: %d")
TRACE_MSG(ADPCMRECORD_2302_112_2_18_1_19_50_1981,"AdpcmRecord.c  Can't Seek to positon: %d")
TRACE_MSG(ADPCMRECORD_2305_112_2_18_1_19_50_1982,"record pcm current_file_pos 2=%d")
TRACE_MSG(ADPCMRECORD_2309_112_2_18_1_19_50_1983,"AdpcmRecord.c  Can't Flush file system")
TRACE_MSG(ADPCMRECORD_2318_112_2_18_1_19_50_1984,"[Record_ChangeBufToWrite] write_err_code = %d")
TRACE_MSG(ADPCMRECORD_2378_112_2_18_1_19_50_1985,"[AudRecord] %d, s_is_recording: %d, s_ref_record_stop: %d")
TRACE_MSG(ADPCMRECORD_2389_112_2_18_1_19_50_1986,"[AudRecord] SCI_NULL != rec_ctrl_writing")
TRACE_MSG(ADPCMRECORD_2482_112_2_18_1_19_50_1987,"[AudRecord] s_cur_file_size =%d, s_end_size=%d")
TRACE_MSG(ADPCMRECORD_2561_112_2_18_1_19_50_1988,"[Record_WriteFSCallback]last buffer written, send to write header, %d, %d, %d, %d.")
TRACE_MSG(ADPCMRECORD_2581_112_2_18_1_19_51_1989,"[Record_WriteFSCallback]write error, send event 1")
TRACE_MSG(ADPCMRECORD_2608_112_2_18_1_19_51_1990,"[Record_WriteFSCallback] last buffer written, send to write header, %d, %d, %d, %d")
TRACE_MSG(ADPCMRECORD_2624_112_2_18_1_19_51_1991,"[Record_WriteFSCallback]write error, send event 2")
TRACE_MSG(ADPCMRECORD_2650_112_2_18_1_19_51_1992,"Record_WriteFSCallback set s_mmi_record_stop true, space_len:%d.")
TRACE_MSG(ADPCMRECORD_2669_112_2_18_1_19_51_1993,"[Record_SendSigWriteHeader] Send signal to write header")
TRACE_MSG(ADPCMRECORD_2674_112_2_18_1_19_51_1994,"Record_SendSigWriteHeader other task has been playing")
TRACE_MSG(ADPCMRECORD_2766_112_2_18_1_19_51_1995,"create new file handle failed.")
TRACE_MSG(ADPCMRECORD_2793_112_2_18_1_19_51_1996,"[Record_WriteHeader] Entry point!")
TRACE_MSG(ADPCMRECORD_2802_112_2_18_1_19_51_1997,"[Record_WriteHeader]set zero 0x %x, %x, %x.")
TRACE_MSG(ADPCMRECORD_2831_112_2_18_1_19_51_1998,"adpcmrecord.c:Record_WriteHeader:AUD_REC_PCM success.")
TRACE_MSG(ADPCMRECORD_2846_112_2_18_1_19_51_1999,"adpcmrecord.c:Record_WriteHeader:error:AUD_REC_AMR == s_record_param_t.format")
TRACE_MSG(ADPCMRECORD_2857_112_2_18_1_19_51_2000,"AdpcmRecord.c  Can't Seek to Begin.result: %d")
TRACE_MSG(ADPCMRECORD_2888_112_2_18_1_19_51_2001,"AdpcmRecord.c 2 write_err_code = %d")
TRACE_MSG(ADPCMRECORD_2942_112_2_18_1_19_51_2002,"[Record_WriteHeaderCallback] write header finish")
TRACE_MSG(ADPCMRECORD_2992_112_2_18_1_19_52_2003,"[Record_HandleWriteHeaderFinish] set s_is_recording=0")
TRACE_MSG(ADPCMRECORD_2997_112_2_18_1_19_52_2004,"Record_HandleWriteHeaderFinish Record_audio_init.")
TRACE_MSG(ADPCMRECORD_3028_112_2_18_1_19_52_2005,"THREAD_ENTRY(RECORD): Received signal: 0x%04x, ori signal:0x%04x.")
TRACE_MSG(ADPCMRECORD_3080_112_2_18_1_19_52_2006,"[EnableVoiceRecord] layer1 record has start.")
TRACE_MSG(ADPCMRECORD_3091_112_2_18_1_19_52_2007,"[EnableVoiceRecord] format:0x%08x.")
TRACE_MSG(ADPCMRECORD_3141_112_2_18_1_19_52_2008,"[EnableVoiceRecord] layer1 record has stop.")
TRACE_MSG(ADPCMRECORD_ADP_68_112_2_18_1_19_52_2009,"adpcmrecord_adp.c:AdpcmRecord_Construct:error:SCI_NULL == ptAudioObj")
TRACE_MSG(ADPCMRECORD_ADP_86_112_2_18_1_19_52_2010,"adpcmrecord_adp.c:AdpcmRecord_Construct:error:SCI_NULL == ptAudioObj")
TRACE_MSG(ADPCMRECORD_ADP_118_112_2_18_1_19_52_2011,"adpcmrecord_adp.c:AdpcmRecord_StopDeconstruct:error:SCI_NULL == ptAudioObj")
TRACE_MSG(ADPCMRECORD_ADP_130_112_2_18_1_19_52_2012,"AdpcmRecord_StopDeconstruct, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d.")
TRACE_MSG(ADPCMRECORD_ADP_159_112_2_18_1_19_53_2013,"adpcmrecord_adp.c:AdpcmRecord_StopDeconstruct:error:SCI_NULL == ptAudioObj")
TRACE_MSG(ADPCMRECORD_ADP_170_112_2_18_1_19_53_2014,"AdpcmRecord_PauseDeconstruct, success to ChangeThreadPriority:src_pri:%d, dest_pri:%d.")
TRACE_MSG(ADPCMRECORD_ADP_207_112_2_18_1_19_53_2015,"adpcmrecord_adp.c:AdpcmRecord_StopDeconstruct:error:SCI_NULL == ptAudioObj")
TRACE_MSG(ADPCMRECORD_ADP_240_112_2_18_1_19_53_2016,"adpcmrecord_adp.c:AdpcmRecord_StartRecord:error:SCI_NULL == ptAudioObj")
TRACE_MSG(ADPCMRECORD_ADP_255_112_2_18_1_19_53_2017,"[AdpcmRecord_StartRecord] error.")
TRACE_MSG(ADPCMRECORD_ADP_271_112_2_18_1_19_53_2018,"adpcmrecord_adp.c:AdpcmRecord_StopRecord:error:SCI_NULL == ptAudioObj")
TRACE_MSG(ADPCMRECORD_ADP_278_112_2_18_1_19_53_2019,"[AdpcmRecord_StopRecord] entry point!audio_handle:0x%08x.cur_state:%d")
TRACE_MSG(ADPCMRECORD_ADP_284_112_2_18_1_19_53_2020,"[AdpcmRecord_StopRecord]error!audio_handle:0x%08x.cur_state:%d")
TRACE_MSG(ADPCMRECORD_ADP_300_112_2_18_1_19_53_2021,"adpcmrecord_adp.c:AdpcmRecord_PauseRecord:error:SCI_NULL == ptAudioObj")
TRACE_MSG(ADPCMRECORD_ADP_326_112_2_18_1_19_53_2022,"adpcmrecord_adp.c:AdpcmRecord_ResumeRecord:error:SCI_NULL == ptAudioObj")
TRACE_MSG(SBC_BY_DSP_114_112_2_18_1_21_16_2023,"sbc_by_dsp.c:PcmBuf_Init:SCI_CreateEvent failed!")
TRACE_MSG(SBC_BY_DSP_564_112_2_18_1_21_17_2024,"sbc_by_dsp.c:SBC_ENCODE_Process:sci_sleep(20)")
TRACE_MSG(SBC_BY_DSP_632_112_2_18_1_21_17_2025,"sbc_by_dsp.c:PCM_BUFFER_Alloc:buffer_l is %x,buffer_r %x")
TRACE_MSG(SBC_BY_DSP_654_112_2_18_1_21_17_2026,"sbc_by_dsp.c:MemRes_Free : 1")
TRACE_MSG(SBC_BY_DSP_660_112_2_18_1_21_17_2027,"sbc_by_dsp.c:MemRes_Free : 2")
TRACE_MSG(SBC_BY_DSP_666_112_2_18_1_21_17_2028,"sbc_by_dsp.c:MemRes_Free : 3")
TRACE_MSG(SBC_BY_DSP_671_112_2_18_1_21_17_2029,"sbc_by_dsp.c:MemRes_Free : 4")
TRACE_MSG(SBC_BY_DSP_677_112_2_18_1_21_17_2030,"sbc_by_dsp.c:MemRes_Free : 5")
TRACE_MSG(SBC_BY_DSP_682_112_2_18_1_21_17_2031,"sbc_by_dsp.c:MemRes_Free : 6")
TRACE_MSG(SBC_BY_DSP_686_112_2_18_1_21_17_2032,"sbc_by_dsp.c:MemRes_Free : deinit buffer")
TRACE_MSG(SBC_BY_DSP_804_112_2_18_1_21_18_2033,"sbc_by_dsp.c:DSP_Proc_Open")
TRACE_MSG(SBC_BY_DSP_808_112_2_18_1_21_18_2034,"sbc_by_dsp.c:DSP_Proc_Open:sbc_res is null")
TRACE_MSG(SBC_BY_DSP_817_112_2_18_1_21_18_2035,"sbc_by_dsp.c:DSP_Proc_Open:MemRes_Alloc fail")
TRACE_MSG(SBC_BY_DSP_824_112_2_18_1_21_18_2036,"sbc_by_dsp.c:, Open(), failed to create event")
TRACE_MSG(SBC_BY_DSP_833_112_2_18_1_21_18_2037,"sbc_by_dsp.c:DSP_Proc_Open:uiPriority is %d")
TRACE_MSG(SBC_BY_DSP_865_112_2_18_1_21_18_2038,"sbc_by_dsp.c:DSP_Proc_Close")
TRACE_MSG(SBC_BY_DSP_878_112_2_18_1_21_18_2039,"sbc_by_dsp.c:DSP_Proc_Close:MemRes_Free in")
TRACE_MSG(UPLINK_CODEC_ADP_115_112_2_18_1_21_18_2040,"[BT]Voice:0x%X,0x%X,0x%X,0x%X\r\n")
TRACE_MSG(UPLINK_CODEC_ADP_142_112_2_18_1_21_19_2041,"[BT]uplink_write_data:data_count=0x%X\r\n")
TRACE_MSG(UPLINK_CODEC_ADP_188_112_2_18_1_21_19_2042,"s_uplink_buf->data_count=%d")
TRACE_MSG(UPLINK_CODEC_ADP_229_112_2_18_1_21_19_2043,"time:%d.")
TRACE_MSG(UPLINK_CODEC_ADP_267_112_2_18_1_21_19_2044,"[uplink_play_construct] this codec is playing.")
TRACE_MSG(UPLINK_CODEC_ADP_371_112_2_18_1_21_19_2045,"UPLINK_Construct alloc failed, sizeof:%d bytes.")
TRACE_MSG(UPLINK_CODEC_ADP_425_112_2_18_1_21_19_2046,"[UPLINK_Play] IN")
TRACE_MSG(UPLINK_CODEC_ADP_452_112_2_18_1_21_19_2047,"[UPLINK_Stop] IN")
TRACE_MSG(UPLINK_CODEC_ADP_460_112_2_18_1_21_19_2048,"[UPLINK_Stop] this codec is not playing.")
TRACE_MSG(UPLINK_CODEC_ADP_477_112_2_18_1_21_19_2049,"[UPLINK_Stop] this audio has been stopped.")
TRACE_MSG(UPLINK_CODEC_ADP_487_112_2_18_1_21_19_2050,"[UPLINK_Pause] IN")
TRACE_MSG(UPLINK_CODEC_ADP_496_112_2_18_1_21_19_2051,"[UPLINK_Pause] this codec is not playing.")
TRACE_MSG(UPLINK_CODEC_ADP_513_112_2_18_1_21_19_2052,"[UPLINK_Pause] this audio is not playing.")
TRACE_MSG(UPLINK_CODEC_ADP_523_112_2_18_1_21_19_2053,"[UPLINK_Resume] IN")
TRACE_MSG(UPLINK_CODEC_ADP_541_112_2_18_1_21_19_2054,"[UPLINK_Resume] this audio is not in pausing.")
TRACE_MSG(UPLINK_CODEC_ADP_553_112_2_18_1_21_19_2055,"UPLINK_IdentifyFormat:error:SCI_NULL == ptAudioObject")
TRACE_MSG(AUD_LVVE_CTRL_268_112_2_18_1_21_20_2056,"[Aud_LVVE_Construct] Failed! s_aud_lvve_thread_id=0x%x")
TRACE_MSG(AUD_LVVE_CTRL_272_112_2_18_1_21_20_2057,"Aud_LVVE_Construct Success! s_aud_lvve_thread_id=0x%x")
TRACE_MSG(AUD_LVVE_CTRL_298_112_2_18_1_21_20_2058,"[Aud_LVVE_Start] NULL == sig_ptr")
TRACE_MSG(AUD_LVVE_CTRL_306_112_2_18_1_21_20_2059,"Aud_LVVE_Start")
TRACE_MSG(AUD_LVVE_CTRL_322_112_2_18_1_21_20_2060,"[Aud_LVVE_ConfigParam] Input params error: config_param_ptr == NULL.")
TRACE_MSG(AUD_LVVE_CTRL_333_112_2_18_1_21_21_2061,"[Aud_LVVE_ConfigParam] Aud_LVVE_SaveParam error.")
TRACE_MSG(AUD_LVVE_CTRL_339_112_2_18_1_21_21_2062,"s_b_LVVE_ON = FALSE, LVVE not running.")
TRACE_MSG(AUD_LVVE_CTRL_346_112_2_18_1_21_21_2063,"[Aud_LVVE_ConfigParam] NULL == sig_ptr")
TRACE_MSG(AUD_LVVE_CTRL_399_112_2_18_1_21_21_2064,"[Aud_LVVE_SendData] NULL == sig_ptr")
TRACE_MSG(AUD_LVVE_CTRL_417_112_2_18_1_21_21_2065,"[Aud_LVVE_SendData] NULL == sig_ptr")
TRACE_MSG(AUD_LVVE_CTRL_440_112_2_18_1_21_21_2066,"[Aud_LVVE_Uninit] NULL == sig_ptr")
TRACE_MSG(AUD_LVVE_CTRL_494_112_2_18_1_21_21_2067,"[LVVE_Init] Version is %s")
TRACE_MSG(AUD_LVVE_CTRL_510_112_2_18_1_21_21_2068,"Parameter error - null pointer error returned by LVVE_Tx_GetMemoryTable")
TRACE_MSG(AUD_LVVE_CTRL_515_112_2_18_1_21_21_2069,"Parameter error - out of range error returned by LVVE_Tx_GetMemoryTable")
TRACE_MSG(AUD_LVVE_CTRL_520_112_2_18_1_21_21_2070,"LVVE_Tx_GetMemoryTable Failed, LVVE_Status=%d")
TRACE_MSG(AUD_LVVE_CTRL_532_112_2_18_1_21_21_2071,"Failed to allocate %d bytes for region %d\n")
TRACE_MSG(AUD_LVVE_CTRL_538_112_2_18_1_21_21_2072,"s_MemoryTable_Tx.Region[i].Size = 0!")
TRACE_MSG(AUD_LVVE_CTRL_548_112_2_18_1_21_21_2073,"Parameter error - null pointer error returned by LVVE_Rx_GetMemoryTable\n")
TRACE_MSG(AUD_LVVE_CTRL_553_112_2_18_1_21_21_2074,"Parameter error - out of range error returned by LVVE_Rx_GetMemoryTable\n")
TRACE_MSG(AUD_LVVE_CTRL_558_112_2_18_1_21_21_2075,"LVVE_Rx_GetMemoryTable Failed, LVVE_Status=%d")
TRACE_MSG(AUD_LVVE_CTRL_570_112_2_18_1_21_21_2076,"Failed to allocate %d bytes for region %d\n")
TRACE_MSG(AUD_LVVE_CTRL_583_112_2_18_1_21_21_2077,"Parameter error - null pointer error returned by LVVE_Tx_GetInstanceHandle\n")
TRACE_MSG(AUD_LVVE_CTRL_588_112_2_18_1_21_21_2078,"Parameter error - out of range error returned by LVVE_Tx_GetInstanceHandle\n")
TRACE_MSG(AUD_LVVE_CTRL_593_112_2_18_1_21_21_2079,"LVVE_Tx_GetInstanceHandle Failed, LVVE_Status=%d")
TRACE_MSG(AUD_LVVE_CTRL_603_112_2_18_1_21_21_2080,"Parameter error - null pointer error returned by LVVE_Rx_GetInstanceHandle\n")
TRACE_MSG(AUD_LVVE_CTRL_608_112_2_18_1_21_21_2081,"Parameter error - out of range error returned by LVVE_Rx_GetInstanceHandle\n")
TRACE_MSG(AUD_LVVE_CTRL_613_112_2_18_1_21_21_2082,"LVVE_Rx_GetInstanceHandle Failed, LVVE_Status=%d")
TRACE_MSG(AUD_LVVE_CTRL_623_112_2_18_1_21_21_2083,"[LVVE_SetControlParam] Failed!")
TRACE_MSG(AUD_LVVE_CTRL_627_112_2_18_1_21_21_2084,"[LVVE_Init] Init LVVE Success.")
TRACE_MSG(AUD_LVVE_CTRL_646_112_2_18_1_21_21_2085,"[LVVE_SetControlParam] Rx operMode:%d, Mute=%d, AVL_MaxGainLin=%d, AVL_Target_level_lin=%d, FENS_limit_NS=%d")
TRACE_MSG(AUD_LVVE_CTRL_653_112_2_18_1_21_21_2086,"Parameter error - null pointer error returned by LVVE_Rx_SetControlParameters\n")
TRACE_MSG(AUD_LVVE_CTRL_659_112_2_18_1_21_21_2087,"Parameter error - out of range error returned by LVVE_Rx_SetControlParameters\n")
TRACE_MSG(AUD_LVVE_CTRL_665_112_2_18_1_21_21_2088,"LVVE_Rx_SetControlParameters Failed, LVVE_Status=%d")
TRACE_MSG(AUD_LVVE_CTRL_674_112_2_18_1_21_21_2089,"[LVVE_SetControlParam] Tx mode:%d, mute:%d, HF_DENS_tail_alpha=%d, HF_DENS_gamma_e_high=%d, WM_AVL_Limit_MaxOutputLin=%d")
TRACE_MSG(AUD_LVVE_CTRL_681_112_2_18_1_21_21_2090,"Parameter error - null pointer error returned by LVVE_Tx_SetControlParameters\n")
TRACE_MSG(AUD_LVVE_CTRL_687_112_2_18_1_21_21_2091,"Parameter error - out of range error returned by LVVE_Tx_SetControlParameters\n")
TRACE_MSG(AUD_LVVE_CTRL_693_112_2_18_1_21_21_2092,"Parameter error - invalid algorithm configuration returned by LVVE_Tx_SetControlParameters\n")
TRACE_MSG(AUD_LVVE_CTRL_699_112_2_18_1_21_21_2093,"LVVE_Tx_SetControlParameters Failed, LVVE_Status=%d")
TRACE_MSG(AUD_LVVE_CTRL_703_112_2_18_1_21_21_2094,"[LVVE_SetControlParam] Success!")
TRACE_MSG(AUD_LVVE_CTRL_732_112_2_18_1_21_21_2095,"Parameter error - null pointer returned from LVVE_Tx_Process\n")
TRACE_MSG(AUD_LVVE_CTRL_738_112_2_18_1_21_21_2096,"Invalid number of samples, check value or range returned from LVVE_Tx_Process\n")
TRACE_MSG(AUD_LVVE_CTRL_744_112_2_18_1_21_21_2097,"Alignment error, check input and/or output buffer alignment returned from LVVE_Tx_Process\n")
TRACE_MSG(AUD_LVVE_CTRL_750_112_2_18_1_21_21_2098,"Error while processing returned from LVVE_Tx_Process: %d\n")
TRACE_MSG(AUD_LVVE_CTRL_786_112_2_18_1_21_21_2099,"Aud_Ctrl_Process_Rx rx time:%d")
TRACE_MSG(AUD_LVVE_CTRL_791_112_2_18_1_21_21_2100,"Parameter error - null pointer returned from LVVE_Rx_Process\n")
TRACE_MSG(AUD_LVVE_CTRL_797_112_2_18_1_21_21_2101,"Invalid number of samples, check value or range returned from LVVE_Rx_Process\n")
TRACE_MSG(AUD_LVVE_CTRL_803_112_2_18_1_21_21_2102,"Alignment error, check input and/or output buffer alignment returned from LVVE_Rx_Process\n")
TRACE_MSG(AUD_LVVE_CTRL_809_112_2_18_1_21_21_2103,"Error while processing returned from LVVE_Rx_Process: %d\n")
TRACE_MSG(AUD_LVVE_CTRL_837_112_2_18_1_21_22_2104,"Parameter error - null pointer error returned by LVVE_Tx_GetMemoryTable\n")
TRACE_MSG(AUD_LVVE_CTRL_843_112_2_18_1_21_22_2105,"Parameter error - out of range error returned by LVVE_Tx_GetMemoryTable\n")
TRACE_MSG(AUD_LVVE_CTRL_863_112_2_18_1_21_22_2106,"Parameter error - null pointer error returned by LVVE_Rx_GetMemoryTable\n")
TRACE_MSG(AUD_LVVE_CTRL_868_112_2_18_1_21_22_2107,"Parameter error - out of range error returned by LVVE_Rx_GetMemoryTable\n")
TRACE_MSG(AUD_LVVE_CTRL_882_112_2_18_1_21_22_2108,"[LVVE_Unit] Free LVVE Success.")
TRACE_MSG(AUD_LVVE_CTRL_901_112_2_18_1_21_22_2109,"[Aud_LVVE_SaveParam] rx operatingMode:%d, mute:%d.")
TRACE_MSG(AUD_LVVE_CTRL_908_112_2_18_1_21_22_2110,"[Aud_LVVE_SaveParam] tx operatingMode:%d, mute:%d.")
TRACE_MSG(AUD_LVVE_CTRL_941_112_2_18_1_21_22_2111,"Parameter error - us_signal_code=%d")
TRACE_MSG(AUD_LVVE_CTRL_983_112_2_18_1_21_22_2112,"Parameter error - config_param_ptr == NULL")
TRACE_MSG(AUD_LVVE_CTRL_996_112_2_18_1_21_22_2113,"CMD_LVVE_PROCESS_Tx:%d, %d, %d.")
TRACE_MSG(AUD_LVVE_CTRL_1018_112_2_18_1_21_22_2114,"Parameter error - SignalCode=%d")
TRACE_MSG(VOICE_ADP_148_112_2_18_1_21_22_2115,"[VOICE_Play] Success.")
TRACE_MSG(VOICE_ADP_151_112_2_18_1_21_22_2116,"[VOICE_Play] Failed.")
TRACE_MSG(VOICE_ADP_173_112_2_18_1_21_22_2117,"[VOICE_Stop] Success.")
TRACE_MSG(VOICE_ADP_178_112_2_18_1_21_22_2118,"[VOICE_Stop] Failed.")
TRACE_MSG(VOICEPROCESS_ADP_251_112_2_18_1_21_23_2119,"voiceprocess_adp.c _Voice_Downlink_Init_local init in")
TRACE_MSG(VOICEPROCESS_ADP_258_112_2_18_1_21_23_2120,"voiceprocess_adp.c _Voice_Downlink_Init_local init")
TRACE_MSG(VOICEPROCESS_ADP_261_112_2_18_1_21_23_2121,"voiceprocess_adp.c _Voice_Downlink_Init_local is 0x%x")
TRACE_MSG(VOICEPROCESS_ADP_277_112_2_18_1_21_23_2122,"voiceprocess_adp.c _Voice_Uplink_Init_local init")
TRACE_MSG(VOICEPROCESS_ADP_310_112_2_18_1_21_23_2123,"voiceprocess_adp.c _Voice_Write_Data_To_File modem index:%d, lose one frame, total:%d frames until now, time:%d(ms).")
TRACE_MSG(VOICEPROCESS_ADP_345_112_2_18_1_21_23_2124,"voiceprocess_adp.c _Voice_Write_Data_To_File index:%d, lose one frame, total:%d frames until now, time:%d(ms).")
TRACE_MSG(VOICEPROCESS_ADP_361_112_2_18_1_21_23_2125,"voiceprocess_adp.c _Voice_Write_Data_To_File index:%d, start to save to file, time:%d(ms).")
TRACE_MSG(VOICEPROCESS_ADP_458_112_2_18_1_21_24_2126,"voiceprocess_adp.c voice down:%d, %d, %d.")
TRACE_MSG(VOICEPROCESS_ADP_472_112_2_18_1_21_24_2127,"voiceprocess_adp.c _Voice_Process_Downlink_Thread_Entry loopback curDelayCount/totalDelayCount, down:%d, %d, %d, %d, up:%d, %d, %d, %d.")
TRACE_MSG(VOICEPROCESS_ADP_545_112_2_18_1_21_24_2128,"voiceprocess_adp.c voice up:%d, %d, %d.")
TRACE_MSG(VOICEPROCESS_ADP_805_112_2_18_1_21_24_2129,"voiceprocess_adp.c _Voice_Process_RecordDebugData_Thread_Entry config para error: callback:0x%x.")
TRACE_MSG(VOICEPROCESS_ADP_812_112_2_18_1_21_24_2130,"voiceprocess_adp.c _Voice_Write_Data_To_File index:%d, len:%d(bytes), cacheIndex:%d, send to ap over, time:%d(ms).")
TRACE_MSG(VOICEPROCESS_ADP_839_112_2_18_1_21_24_2131,"voiceprocess_adp.c _Voice_Process_RecordDebugData_Thread_Entry fail to write file, dataSize:%d, index:%d,")
TRACE_MSG(VOICEPROCESS_ADP_843_112_2_18_1_21_24_2132,"voiceprocess_adp.c _Voice_Write_Data_To_File index:%d, len:%d(bytes), save to file over, result:%d, time:%d(ms).")
TRACE_MSG(VOICEPROCESS_ADP_868_112_2_18_1_21_24_2133,"voiceprocess_adp.c _Voice_Downlink_Pro_Reg in, config:0x%x, exe:0x%x")
TRACE_MSG(VOICEPROCESS_ADP_887_112_2_18_1_21_24_2134,"voiceprocess_adp.c _Voice_Uplink_Pro_Reg in, config:0x%x, exe:0x%x")
TRACE_MSG(VOICEPROCESS_ADP_904_112_2_18_1_21_24_2135,"voiceprocess_adp.c _Voice_ExtOpe_Get_NVConfigInfo curMode:%d.")
TRACE_MSG(VOICEPROCESS_ADP_918_112_2_18_1_21_24_2136,"voiceprocess_adp.c _Voice_ExtOpe_Get_NVConfigInfo alloc failed in loopback, curMode:%d, size:%d.")
TRACE_MSG(VOICEPROCESS_ADP_955_112_2_18_1_21_25_2137,"voiceprocess_adp.c NVConfigInfo, loopback pro_enable:%d, voiceBoostEnable:%d, nxpEnable:%d, aec:0x%x, extend2[64]:0x%x, iftd:%d.")
TRACE_MSG(VOICEPROCESS_ADP_978_112_2_18_1_21_25_2138,"voiceprocess_adp.c _Voice_ExtOpe_Get_NVConfigInfo alloc failed 1, curMode:%d, size:%d.")
TRACE_MSG(VOICEPROCESS_ADP_992_112_2_18_1_21_25_2139,"voiceprocess_adp.c _Voice_ExtOpe_Get_NVConfigInfo alloc failed 2, curMode:%d, size:%d.")
TRACE_MSG(VOICEPROCESS_ADP_1088_112_2_18_1_21_25_2140,"voiceprocess_adp.c NVConfigInfo, pro_enable:%d, voiceBoostEnable:%d, nxpEnable:%d, aec:0x%x(gsm), 0x%x(td), extend2[64]:0x%x(gsm), 0x%x(td), iftd:%d.")
TRACE_MSG(VOICEPROCESS_ADP_1101_112_2_18_1_21_25_2141,"voiceprocess_adp.c _Voice_ExtOpe_Get_NVConfigInfo alloc failed in loopback, curMode:%d, size:%d.")
TRACE_MSG(VOICEPROCESS_ADP_1163_112_2_18_1_21_25_2142,"voiceprocess_adp.c NVConfigInfo, pro_enable:%d, voiceBoostEnable:%d, nxpEnable:%d, aec:0x%x, extend2[64]:0x%x.")
TRACE_MSG(VOICEPROCESS_ADP_1180_112_2_18_1_21_25_2143,"voiceprocess_adp.c _Voice_IfLoopback sbIfLoopback input para error.")
TRACE_MSG(VOICEPROCESS_ADP_1186_112_2_18_1_21_25_2144,"voiceprocess_adp.c _Voice_IfLoopback sbIfLoopback:%d.")
TRACE_MSG(VOICEPROCESS_ADP_1202_112_2_18_1_21_25_2145,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData input para error.")
TRACE_MSG(VOICEPROCESS_ADP_1211_112_2_18_1_21_25_2146,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData input para error, uiRecordPos(%d, min:0, max:%d) is invalid.")
TRACE_MSG(VOICEPROCESS_ADP_1221_112_2_18_1_21_25_2147,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData the data of this pos has been recording")
TRACE_MSG(VOICEPROCESS_ADP_1233_112_2_18_1_21_25_2148,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData config para error: callback:0x%x, cachFrameCount:%d, samplesOnce:%d.")
TRACE_MSG(VOICEPROCESS_ADP_1240_112_2_18_1_21_25_2149,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData alloc ptCachData failed, size: %d.")
TRACE_MSG(VOICEPROCESS_ADP_1245_112_2_18_1_21_25_2150,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData alloc ptCachData success, size: %d.")
TRACE_MSG(VOICEPROCESS_ADP_1252_112_2_18_1_21_25_2151,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData alloc psTotalBuffer failed, size: %d.")
TRACE_MSG(VOICEPROCESS_ADP_1258_112_2_18_1_21_25_2152,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData alloc psTotalBuffer success, size: %d.")
TRACE_MSG(VOICEPROCESS_ADP_1268_112_2_18_1_21_25_2153,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData modem start, index:%d.")
TRACE_MSG(VOICEPROCESS_ADP_1277_112_2_18_1_21_25_2154,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData the data of this pos:%d has not been recording")
TRACE_MSG(VOICEPROCESS_ADP_1290_112_2_18_1_21_25_2155,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData modem stop, index:%d,lostData:%d(frames), sendData:%d(bytes).")
TRACE_MSG(VOICEPROCESS_ADP_1299_112_2_18_1_21_25_2156,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData input para error, name is null.")
TRACE_MSG(VOICEPROCESS_ADP_1305_112_2_18_1_21_25_2157,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData the data of this pos has been recording")
TRACE_MSG(VOICEPROCESS_ADP_1315_112_2_18_1_21_25_2158,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData hFileHanlde create failed. name:%s.")
TRACE_MSG(VOICEPROCESS_ADP_1322_112_2_18_1_21_25_2159,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData alloc failed, size: %d.")
TRACE_MSG(VOICEPROCESS_ADP_1332_112_2_18_1_21_25_2160,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData alloc ping-pang buf:0x%x, 0x%x, total size: %d.")
TRACE_MSG(VOICEPROCESS_ADP_1339_112_2_18_1_21_25_2161,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData start, index:%d.")
TRACE_MSG(VOICEPROCESS_ADP_1351_112_2_18_1_21_25_2162,"voiceprocess_adp.c: start to record; CHNG_FREQ_REQ_HANDLER_NULL == s_voice_record_chng_freq_req_handler")
TRACE_MSG(VOICEPROCESS_ADP_1365_112_2_18_1_21_25_2163,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData the data of this pos:%d has not been recording")
TRACE_MSG(VOICEPROCESS_ADP_1385_112_2_18_1_21_25_2164,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData sleep to wait last write file over, time:%d(ms), force to break.")
TRACE_MSG(VOICEPROCESS_ADP_1389_112_2_18_1_21_25_2165,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData sleep to wait last write file over, time:%d(ms).")
TRACE_MSG(VOICEPROCESS_ADP_1419_112_2_18_1_21_25_2166,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData fail to write file 1, dataSize:%d, index:%d,")
TRACE_MSG(VOICEPROCESS_ADP_1434_112_2_18_1_21_25_2167,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData fail to write file 2, dataSize:%d, index:%d,")
TRACE_MSG(VOICEPROCESS_ADP_1443_112_2_18_1_21_25_2168,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData fail to write file, dataSize:%d, index:%d,")
TRACE_MSG(VOICEPROCESS_ADP_1456_112_2_18_1_21_26_2169,"voiceprocess_adp.c _Voice_ExtOpe_EnableRecordDebugData stop, index:%d, lost:%d(bytes), save:%d(frames).")
TRACE_MSG(VOICEPROCESS_ADP_1485_112_2_18_1_21_26_2170,"voiceprocess_adp.c _Voice_ExtOpe_ConfigRecordDebugData input para error.")
TRACE_MSG(VOICEPROCESS_ADP_1496_112_2_18_1_21_26_2171,"voiceprocess_adp.c _Voice_ExtOpe_ConfigRecordDebugData input para error: 0x%x, %d, %d.")
TRACE_MSG(VOICEPROCESS_ADP_1502_112_2_18_1_21_26_2172,"voiceprocess_adp.c _Voice_ExtOpe_ConfigRecordDebugData this codec is playing, can't reConfig parameters.")
TRACE_MSG(VOICEPROCESS_ADP_1511_112_2_18_1_21_26_2173,"voiceprocess_adp.c _Voice_ExtOpe_ConfigRecordDebugData exe successfully, samplesOnce:%d, cachFrameCount:%d, callback:0x%x.")
TRACE_MSG(VOICEPROCESS_ADP_1531_112_2_18_1_21_26_2174,"voiceprocess_adp.c  _Voice_ConfigFromNv_DelayTimeInLoopback to get delay time.")
TRACE_MSG(VOICEPROCESS_ADP_1536_112_2_18_1_21_26_2175,"voiceprocess_adp.c   _Voice_ConfigFromNv_DelayTimeInLoopback, alloc fail, size: %d")
TRACE_MSG(VOICEPROCESS_ADP_1543_112_2_18_1_21_26_2176,"voiceprocess_adp.c   _Voice_ConfigFromNv_DelayTimeInLoopback mode:%d.")
TRACE_MSG(VOICEPROCESS_ADP_1551_112_2_18_1_21_26_2177,"voiceprocess_adp.c  _Voice_ConfigFromNv_DelayTimeInLoopback failed to get mode param.")
TRACE_MSG(VOICEPROCESS_ADP_1577_112_2_18_1_21_26_2178,"voiceprocess_adp.c  _Voice_ConfigFromNv_DelayTimeInLoopback failed to alloc info, size:%d.")
TRACE_MSG(VOICEPROCESS_ADP_1594_112_2_18_1_21_26_2179,"voiceprocess_adp.c  _Voice_ConfigFromNv_DelayTimeInLoopback failed to alloc data buffer, size:%d.")
TRACE_MSG(VOICEPROCESS_ADP_1608_112_2_18_1_21_26_2180,"voiceprocess_adp.c  _Voice_ConfigFromNv_DelayTimeInLoopback failed to alloc data buffer, size:%d.")
TRACE_MSG(VOICEPROCESS_ADP_1627_112_2_18_1_21_26_2181,"voiceprocess_adp.c  _Voice_ConfigFromNv_DelayTimeInLoopback enable arm delay time %d,  nv:%d, unit:ms, delayFrameCount:%d.")
TRACE_MSG(VOICEPROCESS_ADP_1643_112_2_18_1_21_26_2182,"voiceprocess_adp.c  _Voice_ConfigFromNv_DelayTimeInLoopback to disable to delay.")
TRACE_MSG(VOICEPROCESS_ADP_1665_112_2_18_1_21_26_2183,"_Voice_ExtOpe_Get_NVConfigInfo, alloc fail, size: %d")
TRACE_MSG(VOICEPROCESS_ADP_1672_112_2_18_1_21_26_2184,"_Voice_ExtOpe_Get_NVConfigInfo mode:%d.")
TRACE_MSG(VOICEPROCESS_ADP_1680_112_2_18_1_21_26_2185,"_Voice_ExtOpe_Get_NVConfigInfo failed to get mode param.")
TRACE_MSG(VOICEPROCESS_ADP_1757_112_2_18_1_21_26_2186,"voiceprocess_adp.c [_VOICE_RegetInfoOnceAudioModeChanged], NO enough memory failed.")
TRACE_MSG(VOICEPROCESS_ADP_1787_112_2_18_1_21_26_2187,"voiceprocess_adp.c [_VOICE_RegetInfoOnceVolumeChanged], not support mode ")
TRACE_MSG(VOICEPROCESS_ADP_1792_112_2_18_1_21_26_2188,"voiceprocess_adp.c [_VOICE_RegetInfoOnceAudioModeChanged], LVVE_CTRL_PARAM_GetDevModeName error mode = %d, name:%s")
TRACE_MSG(VOICEPROCESS_ADP_1799_112_2_18_1_21_26_2189,"voiceprocess_adp.c [_VOICE_RegetInfoOnceAudioModeChanged], LVVE_CTRL_PARAM_GetDevModeParam error mode = %d, name:%s")
TRACE_MSG(VOICEPROCESS_ADP_1805_112_2_18_1_21_26_2190,"voiceprocess_adp.c [_VOICE_RegetInfoOnceAudioModeChanged], current device mode = %d, name:%s, %d")
TRACE_MSG(VOICEPROCESS_ADP_1809_112_2_18_1_21_26_2191,"voiceprocess_adp.c [_VOICE_RegetInfoOnceAudioModeChanged], Aud_LVVE_ConfigParam error.")
TRACE_MSG(VOICEPROCESS_ADP_1828_112_2_18_1_21_26_2192,"voiceprocess_adp.c [_VOICE_RegetInfoOnceAudioModeChanged], curDown:(*%d/4096), ifcurDown:%d")
TRACE_MSG(VOICEPROCESS_ADP_1850_112_2_18_1_21_27_2193,"voiceprocess_adp.c [_VOICE_RegetInfoOnceVolumeChanged], NO enough memory failed.")
TRACE_MSG(VOICEPROCESS_ADP_1880_112_2_18_1_21_27_2194,"voiceprocess_adp.c [_VOICE_RegetInfoOnceVolumeChanged], not support mode ")
TRACE_MSG(VOICEPROCESS_ADP_1885_112_2_18_1_21_27_2195,"voiceprocess_adp.c [_VOICE_RegetInfoOnceVolumeChanged], LVVE_CTRL_PARAM_GetDevModeName error.")
TRACE_MSG(VOICEPROCESS_ADP_1893_112_2_18_1_21_27_2196,"voiceprocess_adp.c [_VOICE_RegetInfoOnceVolumeChanged], LVVE_CTRL_PARAM_GetDevModeName error mode = %d, name:%s.")
TRACE_MSG(VOICEPROCESS_ADP_1903_112_2_18_1_21_27_2197,"voiceprocess_adp.c [_VOICE_RegetInfoOnceVolumeChanged], current device mode = %d, vol:%d, name:%s.")
TRACE_MSG(VOICEPROCESS_ADP_1907_112_2_18_1_21_27_2198,"voiceprocess_adp.c [_VOICE_RegetInfoOnceVolumeChanged], Aud_LVVE_ConfigParam error.")
TRACE_MSG(VOICEPROCESS_ADP_2039_112_2_18_1_21_27_2199,"VoiceProcess_Ext_Play finished...")
TRACE_MSG(VOICEPROCESS_ADP_2125_112_2_18_1_21_27_2200,"voiceprocess_adp.c:VOICE_IdentifyFormat:error:ptAudioObject == SCI_NULL")
TRACE_MSG(VT_ADP_116_112_2_18_1_21_28_2201,"[VT_ADP_Stop] not playing.")
TRACE_MSG(VT_ADP_124_112_2_18_1_21_28_2202,"[VT_ADP_Stop] 0x%x, 0x%x.")
TRACE_MSG(VT_ADP_131_112_2_18_1_21_28_2203,"[VT_ADP_Stop] wrong state: recording but not playing.")
TRACE_MSG(VT_ADP_203_112_2_18_1_21_28_2204,"[VT_Ext_Start] be playing, handle:0x%x.")
TRACE_MSG(VT_ADP_237_112_2_18_1_21_28_2205,"[VT_Ext_Start] alloc failed, handle:0x%x, sizeof:%d.")
TRACE_MSG(VT_ADP_274_112_2_18_1_21_28_2206,"[VT_Ext_Stop] not be playing.")
TRACE_MSG(VT_ADP_279_112_2_18_1_21_28_2207,"[VT_Ext_Stop] please stop record first.")
TRACE_MSG(VT_ADP_293_112_2_18_1_21_28_2208,"[VT_Ext_Stop] 0x%x, 0x%x.")
TRACE_MSG(VT_ADP_342_112_2_18_1_21_28_2209,"[VT_Ext_WriteVoiceData] PNULL==ptPara->write_voicedata_t.us_source_buf_ptr")
TRACE_MSG(VT_ADP_381_112_2_18_1_21_29_2210,"[VT_Ext_WriteVoiceData] PNULL==read_byte_data->uc_source_buf_ptr")
TRACE_MSG(VT_ADP_400_112_2_18_1_21_29_2211,"[VT_Ext_WriteVoiceData] PNULL==read_voicedata_2byets->us_source_buf_ptr")
TRACE_MSG(VT_ADP_429_112_2_18_1_21_29_2212,"[VT_Ext_StartRecord] not be playing.")
TRACE_MSG(VT_ADP_435_112_2_18_1_21_29_2213,"[VT_Ext_StartRecord] has been in recording.")
TRACE_MSG(VT_ADP_446_112_2_18_1_21_29_2214,"[VT_Ext_StartRecord] 0x%x, 0x%x.")
TRACE_MSG(VT_ADP_489_112_2_18_1_21_29_2215,"[VT_Ext_WriteVoiceData] PNULL==ptPara->read_recorddata_t.us_source_buf_ptr")
TRACE_MSG(VT_ADP_518_112_2_18_1_21_29_2216,"[VT_Ext_StopRecord] not be playing.")
TRACE_MSG(VT_ADP_523_112_2_18_1_21_29_2217,"[VT_Ext_StopRecord] not be recording.")
TRACE_MSG(VT_ADP_534_112_2_18_1_21_29_2218,"[VT_Ext_StopRecord] 0x%x, 0x%x.")
TRACE_MSG(VT_ADP_579_112_2_18_1_21_29_2219,"[VT_Ext_AppSendDownlinkData] PNULL==ptPara->read_voicedata_t.us_source_buf_ptr")
TRACE_MSG(VT_ADP_586_112_2_18_1_21_29_2220,"[VT_Ext_AppSendDownlinkData] can't put downlnk data, state:%d, %d.")
TRACE_MSG(VT_ADP_618_112_2_18_1_21_29_2221,"[VT_Ext_AppSendDownlinkData] there is no space currently.")
TRACE_MSG(WAV_FFS_78_112_2_18_1_21_42_2222,"wav_ffs.c,[WAV_ReadData] read file encounters error!")
TRACE_MSG(WAV_FFS_134_112_2_18_1_21_42_2223,"wav_ffs.c,[WAV_SetDataPointer] seek file encounters error!")
TRACE_MSG(WAV_FFS_170_112_2_18_1_21_42_2224,"wav_ffs.c,[WAV_SetDataPointer]  buffer seek error:dest_Offset > data_len!dest_Offset:%x,data_len:%x")
TRACE_MSG(WAV_FFS_211_112_2_18_1_21_42_2225,"wav_ffs.c,[WAV_GetDataSize] get file size encounters error!")
TRACE_MSG(WAV_FFS_252_112_2_18_1_21_42_2226,"wav_ffs.c,[WAV_GetDataPointer] get file data pointer encounters error!")
TRACE_MSG(WMA_ADP_259_112_2_18_1_23_3_2227,"wma_adp.c:WMA_FreeInConstruct:error:PNULL==audio_obj")
TRACE_MSG(WMA_ADP_309_112_2_18_1_23_3_2228,"wma_adp.c:WMA_FreeInConstruct:error:PNULL==audio_obj")
TRACE_MSG(WMA_ADP_393_112_2_18_1_23_3_2229,"wma_adp.c:WMA_Construct:error:HEADER_TYPE_DATA == header_info_ptr->hea")
TRACE_MSG(WMA_ADP_536_112_2_18_1_23_4_2230,"wma_adp.c:WMA_Deconstruct:error:PNULL==audio_obj")
TRACE_MSG(WMA_ADP_543_112_2_18_1_23_4_2231,"wma_adp.c:WMA_PlayConstruct:error:PNULL==audio_obj->tCodecProcessResouce.other_info_ptr")
TRACE_MSG(WMA_ADP_554_112_2_18_1_23_4_2232,"wma_adp.c:WMA_PlayConstruct:error:CHNG_FREQ_REQ_HANDLER_NULL == s_ui_chng_freq_req_handler")
TRACE_MSG(WMA_ADP_586_112_2_18_1_23_4_2233,"wma_adp.c:WMA_Deconstruct:error:PNULL==audio_obj")
TRACE_MSG(WMA_ADP_593_112_2_18_1_23_4_2234,"wma_adp.c:WMA_PlayConstruct:error:PNULL==audio_obj->tCodecProcessResouce.other_info_ptr")
TRACE_MSG(WMA_ADP_624_112_2_18_1_23_4_2235,"wma_adp.c:WMA_ResumeConstruct:error:PNULL==audio_obj")
TRACE_MSG(WMA_ADP_631_112_2_18_1_23_4_2236,"wma_adp.c:WMA_ResumeConstruct:error:PNULL==audio_obj->tCodecProcessResouce.other_info_ptr")
TRACE_MSG(WMA_ADP_641_112_2_18_1_23_4_2237,"wma_adp.c:WMA_PlayConstruct:error:WMA_ResumeConstruct == s_ui_chng_freq_req_handler")
TRACE_MSG(WMA_ADP_667_112_2_18_1_23_4_2238,"wma_adp.c:WMA_StopDeconstruct:error:PNULL==audio_obj")
TRACE_MSG(WMA_ADP_674_112_2_18_1_23_4_2239,"wma_adp.c:WMA_StopDeconstruct:error:PNULL==audio_obj->tCodecProcessResouce.other_info_ptr")
TRACE_MSG(WMA_ADP_714_112_2_18_1_23_4_2240,"wma_adp.c:WMA_Play:error:PNULL==audio_obj")
TRACE_MSG(WMA_ADP_721_112_2_18_1_23_4_2241,"wma_adp.c:WMA_Play:error:PNULL==audio_obj->tCodecProcessResouce.other_info_ptr")
TRACE_MSG(WMA_ADP_763_112_2_18_1_23_4_2242,"wma_adp.c:WMA_Stop:error:PNULL==audio_obj")
TRACE_MSG(WMA_ADP_770_112_2_18_1_23_4_2243,"wma_adp.c:WMA_Stop:error:AUDIO_DEFAULT_TASK_ID==audio_obj->tThreadId")
TRACE_MSG(WMA_ADP_813_112_2_18_1_23_4_2244,"wma_adp.c:WMA_Pause:error:PNULL==audio_obj")
TRACE_MSG(WMA_ADP_820_112_2_18_1_23_4_2245,"wma_adp.c:WMA_Pause:error:AUDIO_DEFAULT_TASK_ID==audio_obj->tThreadId")
TRACE_MSG(WMA_ADP_858_112_2_18_1_23_4_2246,"wma_adp.c:WMA_Resume:error:PNULL==audio_obj")
TRACE_MSG(WMA_ADP_865_112_2_18_1_23_4_2247,"wma_adp.c:WMA_Resume:error:AUDIO_DEFAULT_TASK_ID==audio_obj->tThreadId")
TRACE_MSG(WMA_ADP_942_112_2_18_1_23_4_2248,"wma_adp.c:WMA_Seek:error:PNULL==audio_obj")
TRACE_MSG(WMA_ADP_949_112_2_18_1_23_4_2249,"wma_adp.c:WMA_Seek:error:AUDIO_DEFAULT_TASK_ID==audio_obj->tThreadId")
TRACE_MSG(WMA_ADP_972_112_2_18_1_23_5_2250,"wma_adp.c:WMA_Seek:error:(ui_total_len==0)||(ui_total_time==0)")
TRACE_MSG(WMA_ADP_1025_112_2_18_1_23_5_2251,"wma_adp.c:WMA_SendStreamData:error:PNULL==audio_obj")
TRACE_MSG(WMA_ADP_1032_112_2_18_1_23_5_2252,"wma_adp.c:WMA_SendStreamData:error:AUDIO_DEFAULT_TASK_ID==audio_obj->tThreadId")
TRACE_MSG(WMA_ADP_1074_112_2_18_1_23_5_2253,"wma_adp.c:WMA_IdentifyAudioFormat:error:PNULL==audio_obj")
TRACE_MSG(WMA_ADP_1095_112_2_18_1_23_5_2254,"wma_adp.c:WMA_IdentifyAudioFormat:error:WMA_STREAMBUFFER_HEADER_TYPE_FORMAT != header_info_ptr->header_type")
TRACE_MSG(WMA_ADP_1144_112_2_18_1_23_5_2255,"wma_adp.c:WMA_IdentifyAudioFormat:error:PNULL==audio_obj")
TRACE_MSG(WMA_ADP_1153_112_2_18_1_23_5_2256,"wma_adp.c:WMA_GetPlayingInfo:error:ext_process_resouce_ptr->wma_demux_ptr == PNULL")
TRACE_MSG(WMA_ADP_1200_112_2_18_1_23_5_2257,"wma_adp.c:WMA_GetOutputSamplerate:error:PNULL==audio_obj")
TRACE_MSG(WMA_ADP_1328_112_2_18_1_23_5_2258,"wma_adp.c:WMA_GetDemuxSrcData:error:WMA_STREAMBUFFER_HEADER_TYPE_FORMAT != header_info_ptr->header_type")
TRACE_MSG(WMA_ADP_1366_112_2_18_1_23_5_2259,"wma_adp.c:WMA_GetDemuxSrcData:error:default")
TRACE_MSG(WMA_ADP_1726_112_2_18_1_23_6_2260,"wam_adp.c:thread_entry: default:")
END_TRACE_MAP(MS_REF_CODEC_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_CODEC_TRC_H_

